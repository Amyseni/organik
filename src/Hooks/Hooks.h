#pragma once

#include "Synthetik.h"
#include "Events.h"

enum HookTiming : int32_t
{
    BEFORE = 0,
    AFTER = 1,
    REPLACE = 2
};
typedef bool(__fastcall*FastAction)(CInstance*, std::unordered_map<int32_t, RValue*>*);
namespace Direktor {
    class Action {
        public:
            virtual ~Action() = default;
            virtual FastAction* GetAction() = 0;
    };
    class GMHook {
    protected:
        static std::unordered_map<int32_t, std::vector<GMHook*>> HooksByType;
        static std::shared_mutex s_HookMutex;

        //? Used for fast checking if new hooks need to be installed
        static std::atomic<uint64_t> s_HookGenerationCounter;
        //? thread-local context 
        static thread_local uint64_t tl_LastSeenGeneration;

    public:
        virtual ~GMHook() = 0;
        virtual bool Install() = 0;
        virtual void Uninstall() = 0;

        _NODISCARD_LOCK static std::shared_lock<std::shared_mutex> AcquireSharedLock()
        {
            return std::shared_lock<std::shared_mutex>(s_HookMutex);
        }
        
        template <typename T>
        requires std::is_base_of_v<GMHook, T>
        _NODISCARD_LOCK static std::pair<std::unique_lock<std::shared_mutex>, std::vector<GMHook*>*> GetHooksToInstall()
        {
            auto lock = std::unique_lock<std::shared_mutex>(s_HookMutex);
            
            if (!HooksByType.contains(typeid(T).hash_code()))
                HooksByType[typeid(T).hash_code()] = {};

            auto& hooks = HooksByType[typeid(T).hash_code()];
            return { std::move(lock), &hooks };
        }
        
        template <typename T>
        requires std::is_base_of_v<GMHook, T>
        _NODISCARD_LOCK static std::pair<std::unique_lock<std::shared_mutex>, std::vector<T*>*> 
            GetHooksToInstall(std::unique_lock<std::shared_mutex>&& existingLock)
        {
            if (!HooksByType.contains(typeid(T).hash_code()))
                HooksByType[typeid(T).hash_code()] = {};

            auto& hooks = HooksByType[typeid(T).hash_code()];
            return { std::move(existingLock), &hooks };
        }
        
        template <typename T>
        requires std::is_base_of_v<GMHook, T>
        static bool HasHooksToInstall()
        {
            std::shared_lock<std::shared_mutex> lock(s_HookMutex);
            return HooksByType.contains(typeid(T).hash_code()) && !HooksByType[typeid(T).hash_code()].empty();
        }

        template<class T>
        requires std::is_move_constructible_v<T> && std::is_base_of_v<GMHook, T>
        static T* Add(T&& hook) {
            auto [lock, hooks] = GetHooksToInstall<T>();
            if (!hooks)
                return nullptr;

            T* newHook = new T(std::move(hook));
            (*hooks).push_back(newHook);

            return newHook;
        }

        //? Use to notify all threads that hooks may have changed
        static void NotifyHookChange() {
            s_HookGenerationCounter.fetch_add(1, std::memory_order_release);
        }
        
        //? Use for checking if new hooks may need to be installed
        static bool MightHaveNewHooks() {
            return tl_LastSeenGeneration != s_HookGenerationCounter.load(std::memory_order_acquire);
        }

        //? Use to update the thread-local hook counter
        static void UpdateHookGenerationSeen() {
            tl_LastSeenGeneration = s_HookGenerationCounter.load(std::memory_order_acquire);
        }
        
        //! ALWAYS call this to modify hooks. Ensures lifetime of appropriate variables and lock.
        //! Failure to ensure lifetime of lock and any lock-critical variables is UNDEFINED BEHAVIOR. DANGER. BAD.
        template<typename Func, class tRet = std::invoke_result_t<Func, std::vector<GMHook*>*>>
        requires std::is_invocable_v<Func, std::vector<GMHook*>*>
        static void WithUniqueAccess(Func&& callback) {
            std::unique_lock<std::shared_mutex> uniqueLock(s_HookMutex);
            tRet result = callback();
            // Signal that hooks may have changed
            NotifyHookChange();
        }

        //! ALWAYS call this to install hooks. Ensures lifetime of appropriate variables and lock.
        //! Failure to ensure lifetime of lock and any lock-critical variables is UNDEFINED BEHAVIOR. DANGER. BAD.
        template<typename Func, class tRet = std::invoke_result_t<Func, std::vector<GMHook*>*>>
        requires std::is_invocable_v<Func, std::vector<GMHook*>*>
        static tRet WithHooksToInstall(Func&& callback) {
            //! Acquire ONE lock
            std::unique_lock<std::shared_mutex> uniqueLock(s_HookMutex);

            //! Perform lock-fu
            //! original uniqueLock IS INVALID AFTER THIS CALL.
            auto [lock, hooks] = GetHooksToInstall<GMHook>(std::move(uniqueLock));
            if (!hooks || hooks->empty())
                return tRet();

            //! Call the callback with the hooks
            tRet result = callback(hooks);

            // Signal that hooks may have changed
            NotifyHookChange();
            return result;
        }

        //! ALWAYS wrap any threaded hook access in this. Ensures lifetime of appropriate variables and lock.
        //! Failure to ensure lifetime of lock and any lock-critical variables is UNDEFINED BEHAVIOR. DANGER. BAD.
        template<typename Func>
        static void WithSharedAccess(Func&& callback) {
            std::shared_lock<std::shared_mutex> sharedLock(s_HookMutex);
            callback();
        }
    };
    class EventHook : public GMHook {
    private:
        static std::unordered_map<CCode*, EventHook*> s_EventToHookMap;
        static std::vector<EventHook*> s_InstalledHooks;
        EventHook() = default;
        EventHook(std::string const& objectName) : objName(objectName) {}
        EventHook(int32_t objectIndex) : objIndex(objectIndex) {}

        ~EventHook() {
            if (installed)
                Uninstall();
            
            auto data = Object_Data(objIndex);
            auto event = data ? data->GetEventRecursive(eventCode, subCode) : nullptr;
            auto code = event ? event->m_Code : nullptr;
            if (code && s_EventToHookMap.contains(code))
                s_EventToHookMap.erase(code);
        }
    public:
        std::string const objName;
        int32_t objIndex;
        int32_t eventCode;
        int32_t subCode;
        PFUNC_YYGML originalFunction;
        PFUNC_YYGML replaceFunction;
        std::vector<PFUNC_YYGML> callbacksBefore;
        std::vector<PFUNC_YYGML> callbacksAfter;
        std::function<void(CInstance*, CInstance*)> hookFunction;
        bool installed = false;
        bool Install() override;
        void Uninstall() override;
        
        
        static inline EventHook* Add(int32_t objectIndex, int32_t eventCode, int32_t subCode, HookTiming timing, PFUNC_YYGML hookFunction)
        {
            EventHook* hookObject = nullptr;
            auto objectData = Object_Data(objectIndex);
            if (!objectData)
                Error_Show_Action("EventHook::Add: Invalid object index", true, true);
            
            CCode* code = objectData->GetEventRecursive(eventCode, subCode)->m_Code;
            if (!code)
            {
                Error_Show_Action("EventHook::Add: Invalid event code or subcode", true, true);
                return nullptr;
            }

            if (s_EventToHookMap.contains(code))
            {
                hookObject = s_EventToHookMap[code];
            }
            else
            {
                hookObject = new EventHook(objectIndex);
                hookObject->objIndex = objectIndex;
                hookObject->eventCode = eventCode;
                hookObject->subCode = subCode;
                hookObject->originalFunction = code->m_Functions->m_CodeFunction;
                s_EventToHookMap[code] = hookObject;
            }
            switch (timing)
            {
            case HookTiming::BEFORE:
                hookObject->callbacksBefore.push_back(hookFunction);
                break;
            case HookTiming::AFTER:
                hookObject->callbacksAfter.push_back(hookFunction);
                break;
            case HookTiming::REPLACE:
                hookObject->replaceFunction = hookFunction;
                break;
            default:
                Error_Show_Action("EventHook::Add: Invalid HookTiming", true, true);
                return nullptr;
            }
            return hookObject;
        }
    };
}


