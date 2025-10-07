#pragma once
#include <shared_mutex>
#include <iterator>
#include <Windows.h>
#ifdef _WIN32
#define FUNC_NAKED __declspec(naked)
#ifdef LIBZHL_EXPORTS
	#define LIBZHL_API __declspec(dllexport)
#else
	#define LIBZHL_API __declspec(dllimport)
#endif
#define JUMP_INSTRUCTION(target) jmp [target]
#else
#error("where is my win32?")
#endif

inline static std::shared_mutex& zhlLogMutex() {
	static std::shared_mutex m;
	return m;
}
inline static auto _zhlInit = []() -> bool{
	{
		std::unique_lock<std::shared_mutex> lock(zhlLogMutex());
		WIN32_FIND_DATAA fd;
		HANDLE hFileInf = FindFirstFileA("./ZHL.log", &fd);
		if ((hFileInf) && hFileInf != HANDLE(-1))
			DeleteFileA("./ZHL.log");
		auto file = fopen("./ZHL.log", "w");
		if (file) {
			fprintf(file, "ZHL Log Initialized\n");
			fclose(file);
		}
	}
	return true;
}();
#define Log(fmt, ...) \
{ \
	std::unique_lock<std::shared_mutex> lock(zhlLogMutex()); \
	auto file = fopen("./ZHL.log", "a"); \
	if (file) { \
		fprintf(file, fmt, __VA_ARGS__); \
		fclose(file); \
	} \
}

#include <stdlib.h>
#include <unordered_map>
#include <typeinfo>
#include <cstdio>
#include <shared_mutex>
#include "GameMaker_Defs.h"
#include <map>
#include <vector>
#include <string>
#include <functional>
#include <alg.h>
struct CInstance;
struct CObjectGM;
namespace Direktor {
	class GMHook;
}

enum EventDetailType : int32_t
{
    EV_ENM,
    EV_WEAPON,
    EV_PLAYER
};
enum HookTiming : int32_t
{
    BEFORE = 0,
    AFTER = 1,
    REPLACE = 2
};
typedef void* PVOID;
typedef bool(__fastcall*FastAction)(CInstance*, std::unordered_map<int32_t, RValue*>*);
LIBZHL_API void __stdcall LOCK_RVALUE_MUTEX();
LIBZHL_API void __stdcall UNLOCK_RVALUE_MUTEX();
namespace ZHL
{

class Definition
{
public:
	static int Init();
	static const char *GetLastError();
	static Definition *Find(const char *name);

protected:
	static void Add(const char *name, Definition *def);

public:
	virtual int Load() = 0;

};

class FunctionDefinition : public Definition
{
public:
	FunctionDefinition() = default;

private:
	char _shortName[128];
	char _name[256];

	const short *_argdata;
	const char *_sig;
	int _nArgs;
	void **_outFunc;
	void *_address;

	unsigned int _flags;
private:
	void SetName(const char *name, const char *type);

public:
	FunctionDefinition(const char *name, const std::type_info &type, const char* sig, const short *argdata, int nArgs, unsigned int flags, void **outfunc);
	FunctionDefinition(const char *name, const std::type_info &type, void *addr, const short *argdata, int nArgs, unsigned int flags, void **outfunc);

	virtual int Load();

	bool IsThiscall() const {return (_flags & 1) != 0;}
	bool NeedsCallerCleanup() const {return (_flags & 2) != 0;}
	bool IsVoid() const {return (_flags & 4) != 0;}
	bool IsLongLong() const {return (_flags & 8) != 0;}
	bool isMemPassedStructPointer() const { return (_flags & 16) != 0;}
	bool forceDetourSize() const { return (_flags & 32) != 0; }

	const short *GetArgData() const {return _argdata;}
	int GetArgCount() const {return _nArgs;}
	void *GetAddress() const {return _address;}
};

//=================================================================================================

class VariableDefinition : public Definition
{
private:
	void *_outVar;
	const char *_name;
	const char *_sig;
	const bool _useValue;
	const bool _useOffset;

public:
	VariableDefinition(const char *name, const char *sig, void *outvar, bool useValue = true, bool useOffset = false) :
        _name(name),
        _sig(sig),
        _outVar(outvar),
        _useValue(useValue),
        _useOffset(useOffset)
    {
        Add(_name, this);
    }

	virtual int Load();
};

//=================================================================================================

class NoOpDefinition : public Definition
{
private:
	const char *_name;
	const char *_sig;

public:
	NoOpDefinition(const char *name, const char *sig) :
        _name(name),
        _sig(sig)
    {
        Add(_name, this);
    }

	virtual int Load();
};

}
using namespace ZHL;
#define FUNCTIONHOOK_MAXSIZE 128

class FunctionHook_private
{
public:
	FunctionHook_private() = default;
private:
	char _shortName[128];
	char _name[256];

	void **_outInternalSuper;

	unsigned char _internalHook[FUNCTIONHOOK_MAXSIZE];
	unsigned char _internalSuper[FUNCTIONHOOK_MAXSIZE];
	unsigned int _hSize;
	unsigned int _sSize;
	int _priority;

	void *_detour;

public:
	static int Init();
	static const char *GetLastError();
	void *_hook;
	int Install();

private:
	static void Add(FunctionHook_private *hook);
	void SetName(const char *name, const char *type);

public:
	FunctionHook_private(const char *name, const std::type_info &type, void *hook, void **outInternalSuper, int priority);
	~FunctionHook_private();
};
#ifndef direktor_fwd
#define direktor_fwd
namespace Direktor{
	class GMHook;
}

#endif
//? for generating event names for user display. Is intended to just return SOMETHING so we don't crash.
inline const std::string eventTypeName(int32_t eventCode)
{
    switch(eventCode)
    {
        case 0: return "Create";
        case 1: return "Destroy";
        case 2: return "Alarm";
        case 3: return "Step";
        case 4: return "Collision";
        case 5: return "Keyboard";
        case 6: return "Mouse";
        case 7: return "Other";
        case 8: return "Draw";
        case 9: return "Key Press";
        case 10: return "Key Release";
        case 12: return "Cleanup";
        case 14: return "Pre-Create";
        default: return "Unknown";
    }
}




inline int32_t EventCodeFromName(std::string const& name)
{
    switch(name[0] & ~32)
    {
        case 'C': 
            switch (name[1] & ~32)
            {
                case 'R': return EVENT_CREATE; 
                case 'O': return EVENT_COLLISION;
                case 'L': return EVENT_CLEAN_UP;
            }
            break;
        case 'D': 
            if ((name[1] & (~32)) == 'E') return EVENT_DESTROY;
            else if ((name[1] & (~32)) == 'R') return EVENT_DRAW;
        case 'A': return EVENT_ALARM;
        case 'S': return EVENT_STEP;
        case 'M': return EVENT_MOUSE;
        case 'K': 
            switch (name[1] & ~32) 
            {
                case 'B': return EVENT_KEYBOARD;
                case 'P': return EVENT_KEYPRESS;
                case 'R': return EVENT_KEYRELEASE;
            }
        case 'O': return EVENT_OTHER;
        case 'P': return EVENT_PRE_CREATE;
    }
    return -1;
}


//? for generating event names for user display. Is intended to just return SOMETHING so we don't crash.
inline std::string eventSpecificName(int32_t eventCode, int32_t subCode, EventDetailType detailType = EV_WEAPON)
{
    char subCodeBuf[] = { static_cast<char>(subCode & 0xFF), '\0' };
    switch(eventCode)
    {
        case 2: return "Alarm " + std::to_string(subCode);
        case 3:
        {
            switch(subCode)
            {
                case 0: return "Step - Begin";
                case 1: return "Step - Normal";
                case 2: return "Step - End";
                default: return "Step " + std::to_string(subCode);
            }
        }
        case 5: return "Key " + std::string(subCodeBuf) + " Press";
        case 6:
        {
            switch(subCode)
            {
                case 0: return "Mouse Left Button";
                case 1: return "Mouse Right Button";
                case 2: return "Mouse Middle Button";
                case 3: return "Mouse Wheel Up";
                case 4: return "Mouse Wheel Down";
                default: return "Mouse Button " + std::to_string(subCode);
            }
        }
        case 7:
        {
            switch(subCode)
            {
                case 4: return "Other - Room Start";
                case 5: return "Other - Room End";
                case 10: return "Other - User 1";
                case 11: return "Other - User 1";
                case 12: return "Other - User 2";
                case 13: return "Other - User 3";
                case 14: return "Other - Apply Variant (User 4)";
                case 15: return "Other - User 5";
                case 16: return "Other - User 6";
                case 17: return "Other - User 7";
                case 18: return "Other - User 8";
                case 19: return "Other - User 9";
                case 20: return "Other - User 10";
                case 21: return "Other - User 11";
                case 22: return "Other - User 12 (Network)";
                case 23: return "Other - User 13";
                case 24: return "Other - User 14";
                case 25: return "Other - User 15";
                default: return "Other " + std::to_string(subCode);
            }
        }
        case 9: return "Key " + std::string(subCodeBuf) + " Press";
        case 10: return "Key " + std::string(subCodeBuf) + " Release";
        default: return eventTypeName(eventCode) + " (" + std::to_string(subCode) + ")";
    }
}

CInstance* GetGlobalInstance();
//=================================================================================================

namespace ZHL
{
	LIBZHL_API bool Init();
	LIBZHL_API void Done(bool status);
}

class LIBZHL_API FunctionHook
{
private:
	static void Add(const char *name, const std::type_info &type, void *hook, void **outInternalSuper, int priority);

public:
	template <class T> FunctionHook(const char *name, const std::type_info &type, T hook, void **outInternalSuper, int priority)
	{
		Add(name, type, *(void**)&hook, outInternalSuper, priority);
	}
};
void Error_Show_Action(bool mustCrash, bool manErr, const char* fmt, ...);
bool ZHLDone();
namespace Direktor{ 
	enum LockStatus : int32_t 
	{
		LOCK_OKAY = 0,
		LOCK_ALREADY_LOCKED = 1<<0,
		LOCK_NOT_LOCKED = 1<<1,
		LOCK_UNKNOWN_ERROR = 1 << 30,
	};
	struct Lock {
		virtual bool* getLockRef() = 0;
		virtual LockStatus lock() = 0;
		virtual LockStatus unlock() = 0;
		virtual ~Lock() = default;
	};
	// struct HookLock
	// 	: public Lock
	// {
	// private:
	// 	inline static const int32_t LOCK_TIMEOUT_MS = 100000;
	// 	::std::shared_mutex *m_Mutex;
	// 	::std::unique_lock<std::shared_mutex> m_UniqueLock;
	// 	bool m_Locked = false;

	// 	//! compiler doesn't know how to take a hint, so let's just get rid of these. Would be a problem if they were ever called.
	// 	//! never use `=` on variables of this type. just make static variables and call the functions on them directly
	// 	HookLock(const HookLock&) = delete;
	// 	HookLock& operator=(const HookLock&) = delete;
	// 	HookLock(HookLock&&) = delete;
	// 	HookLock& operator=(HookLock&&) = delete;
	// public:

	// 	inline HookLock(void) :m_Locked(false) {
	// 		if (this->m_UniqueLock) m_Mutex = this->m_UniqueLock.release();
	// 		else m_Mutex = (std::shared_mutex*) malloc(sizeof(std::shared_mutex));
	// 		::new (&this->m_UniqueLock) std::unique_lock<std::shared_mutex>(*m_Mutex, std::defer_lock);
	// 	};
	// 	virtual ~HookLock() final override;
	// 	virtual bool* getLockRef() final override;
	// 	virtual LockStatus lock() final override;
	// 	virtual LockStatus unlock() final override;
	// };
	struct RValueLock final
		: public Lock
	{
	private:
		inline static const int32_t LOCK_TIMEOUT_MS = 100000;
		bool m_Locked = false;

		RValueLock(const RValueLock&) = delete;
		RValueLock& operator=(const RValueLock&) = delete;
		RValueLock(RValueLock&&) = delete;
		RValueLock& operator=(RValueLock&&) = delete;
	public:
		RValueLock() : m_Locked(false) { };
		virtual ~RValueLock() final override;
		virtual bool* getLockRef() final override;
		virtual LockStatus lock() final override;
		virtual LockStatus unlock() final override;
	};

	static inline RValueLock s_InstallerLock = RValueLock();
	inline std::thread** s_GetInstallerThread() {
		static std::thread* installerThread = nullptr;
		return &installerThread;
	}

	static inline const auto s_MapHookToInstallByPtrAndPriority = []() -> std::multimap<uint32_t, Direktor::GMHook*, std::greater<uint32_t>>& {
		static std::multimap<uint32_t, Direktor::GMHook*, std::greater<uint32_t>> m;
		return m;
	};
	static inline const auto s_InstalledHooks = []() -> std::vector<Direktor::GMHook*>& {
		static std::vector<Direktor::GMHook*> m;
		return m;
	};
	class Action {
        public:
            virtual ~Action() = default;
            virtual FastAction* GetAction() = 0;
    };
    class GMHook {
	public:
		virtual ~GMHook() = default;
		virtual void Init() = 0;
		virtual bool Install() = 0;
		virtual void Uninstall() = 0;

	};
}

//=================================================================================================
#define _DEFINE_METHOD_HOOK1(_id, _classname, _name, _priority, ...) \
	namespace { namespace Hook_##_classname##_name##_id##_priority { \
		static void *internalSuper = NULL; \
		struct wrapper : public _classname { \
			auto hook __VA_ARGS__ ; \
			auto super __VA_ARGS__ ; \
		}; \
		static FunctionHook hookObj = FunctionHook(#_classname "::" #_name, typeid(auto (_classname::*) __VA_ARGS__), &wrapper::hook, &internalSuper, _priority); \
	} } \
	auto FUNC_NAKED Hook_##_classname##_name##_id##_priority :: wrapper::super __VA_ARGS__ {__asm {JUMP_INSTRUCTION(Hook_##_classname##_name##_id##_priority ::internalSuper)}; } \
	auto Hook_##_classname##_name##_id##_priority ::wrapper::hook __VA_ARGS__

#define _DEFINE_METHOD_HOOK0(_id, _classname, _name, _priority, ...) _DEFINE_METHOD_HOOK1(_id, _classname, _name, _priority, __VA_ARGS__)

#define HOOK_METHOD(_classname, _name, ...) _DEFINE_METHOD_HOOK0(__LINE__, _classname, _name, 0, __VA_ARGS__)
#define HOOK_METHOD_PRIORITY(_classname, _name, _priority, ...) _DEFINE_METHOD_HOOK0(__LINE__, _classname, _name, _priority, __VA_ARGS__)

//=================================================================================================

#define _DEFINE_STATIC_HOOK1(_id, _classname, _name, _priority, _type) \
	namespace { namespace Hook_##_classname##_name##_id##_priority { \
		static void *internalSuper = NULL; \
		struct wrapper : public _classname { \
			static auto __stdcall hook _type ; \
			static auto __stdcall super _type ; \
		}; \
		static FunctionHook hookObj(#_classname "::" #_name, typeid(auto (*) _type), &wrapper::hook, &internalSuper, _priority); \
	} } \
	auto FUNC_NAKED Hook_##_classname##_name##_id##_priority :: wrapper::super _type {__asm {JUMP_INSTRUCTION(Hook_##_classname##_name##_id##_priority::internalSuper)}; } \
	auto Hook_##_classname##_name##_id##_priority ::wrapper::hook _type

#define _DEFINE_STATIC_HOOK0(_id, _classname, _name, _priority, _type) _DEFINE_STATIC_HOOK1(_id, _classname, _name, _priority, _type)

#define HOOK_STATIC(_classname, _name, _type) _DEFINE_STATIC_HOOK0(__LINE__, _classname, _name, 0, _type)
#define HOOK_STATIC_PRIORITY(_classname, _name, _priority, _type) _DEFINE_STATIC_HOOK0(__LINE__, _classname, _name, _priority, _type)

//====================================================== ===========================================

#define _DEFINE_GLOBAL_HOOK1(_id, _name, _priority, _type) \
	namespace { namespace Hook_##_name##_id##_priority { \
		static void *internalSuper = NULL; \
		static auto __stdcall hook _type ; \
		static auto __stdcall super _type ; \
		\
		static FunctionHook hookObj(#_name, typeid(auto (*) _type), &hook, &internalSuper, _priority); \
	} } \
	auto FUNC_NAKED __stdcall Hook_##_name##_id##_priority ::super _type {__asm {JUMP_INSTRUCTION(Hook_##_name##_id##_priority::internalSuper)}; } \
	auto __stdcall Hook_##_name##_id##_priority ::hook _type

#define _DEFINE_GLOBAL_HOOK0(_id, _name, _priority, _type) _DEFINE_GLOBAL_HOOK1(_id, _name, _priority, _type)

#define HOOK_GLOBAL(_name, _type) _DEFINE_GLOBAL_HOOK0(__LINE__, _name, 0, _type)
#define HOOK_GLOBAL_PRIORITY(_name, _priority, _type) _DEFINE_GLOBAL_HOOK0(__LINE__, _name, _priority, _type)

#define PAREN_OPEN(x) \
   (
#define PAREN_CLOSE(x) \
   )
   
#define _DEFINE_EVENT_HOOK1(_id, _obj, _event, _subevent, _priority) \
namespace { namespace EventHooks { \
		class _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id final : public ::Direktor::GMHook {  \
			static PVOID func; \
			static PVOID s_InternalSuper; \
			static const int32_t nArgs = 2; \
			static const int32_t eventCode = _event;  \
			static const int32_t subCode = _subevent;  \
			static auto __stdcall hook (CInstance *self, CInstance *other) -> void;  \
			static auto __stdcall super (CInstance *self, CInstance *other) -> void; \
			const char* const objName; \
			const char* const functionName = #_obj ## "Hook" ## #_event ## #_subevent ## #_priority ## #_id;  \
			const short argdata[2] = { 0x1ff, 0x1ff }; \
			PVOID *m_OutInternalSuper; \
			::ZHL::FunctionDefinition m_FnDef;  \
			FunctionHook_private *m_ZHLHookObj;  \
			PVOID m_InternalFunc; \
			int32_t m_ObjIndex;  \
			const int32_t m_Priority = _priority; \
			 \
		public: \
			virtual ~ ## ## _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id PAREN_OPEN(x) PAREN_CLOSE(x) final override; \
			## _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id PAREN_OPEN(x) PAREN_CLOSE(x) \
			: ::Direktor::GMHook(), objName(#_obj), m_ObjIndex(0) , m_FnDef(), m_ZHLHookObj(nullptr), m_InternalFunc(0), m_Priority(_priority) { \
				m_OutInternalSuper = &s_InternalSuper; \
			}; \
			virtual void Uninstall() override; \
			virtual bool Install() override; \
			virtual void Init() override; \
		}; \
	} \
} \
bool EventHooks::## _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id ::Install() { \
	Log("EventHooks::_" ## #_obj ## "Hook" ## #_event ## #_subevent ## #_priority ## #_id ## "::Install %p\n",reinterpret_cast<void*>(this)); \
	if (!this->m_ZHLHookObj){ \
		this->m_ZHLHookObj = new FunctionHook_private( \
			this->functionName, \
			typeid(auto (__cdecl*)(CInstance*, CInstance*) -> void), \
			EventHooks::## _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id ::hook, \
			this->m_OutInternalSuper, \
			this->m_Priority \
		); \
		return this->m_ZHLHookObj->Install() != 0; \
	} \
	return false; \
} \
void EventHooks::## _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id ::Uninstall() { \
	Log("EventHooks::_" ## #_obj ## "Hook" ## #_event ## #_subevent ## #_priority ## #_id ## \
		"::Uninstall %p\n",reinterpret_cast<void*>(this)); \
	if (this->m_ZHLHookObj) \
	{ \
		Log("EventHooks::_" ## #_obj ## "Hook" ## #_event ## #_subevent ## #_priority ## #_id ## \
			"::Uninstall - Actually Uninstalling something %p\n",reinterpret_cast<void*>(this)); \
		delete this->m_ZHLHookObj; \
		this->m_ZHLHookObj = nullptr; \
	} \
} \
void EventHooks::## _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id ::Init() { \
	Log("EventHooks::_" ## #_obj ## "Hook" ## #_event ## #_subevent ## #_priority ## #_id ## "::%s\n", "Init"); \
	RValue rv = DoBuiltin(&gml_asset_get_index, { RValue(#_obj) }); \
	m_ObjIndex = parseRValueNumber<int32_t>(rv); \
	if (m_ObjIndex < 1)  \
		Error_Show_Action("EventHook::Add: Invalid object index", true, true); \
	auto objectData = Object_Data(m_ObjIndex); \
	if (!objectData)  \
		Error_Show_Action("EventHook::Add: Invalid object data", true, true); \
	auto cEvent = objectData->GetEventRecursive(eventCode, subCode); \
	Log("EventHooks::_" ## #_obj ## "Hook" ## #_event ## #_subevent ## #_priority ## #_id ## \
		"::Init got object data %p, event data %p\n",reinterpret_cast<void*>(objectData), reinterpret_cast<void*>(cEvent)); \
	if (cEvent) \
	{ \
		EventHooks::## _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id ::func = reinterpret_cast<void*>(cEvent->m_Code ? cEvent->m_Code->m_Functions ? cEvent->m_Code->m_Functions->m_CodeFunction : nullptr : nullptr); \
		Log("EventHooks::_" ## #_obj ## "Hook" ## #_event ## #_subevent ## #_priority ## #_id ## \
			"::Init found event %p\n",reinterpret_cast<void*>(cEvent)); \
	} \
	if (!EventHooks::## _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id ::func) \
		Error_Show_Action("EventHook::Add: Invalid event code or subcode", true, true); \
	::new (&m_FnDef) ::ZHL::FunctionDefinition( \
			functionName, \
			typeid(auto (CInstance*, CInstance*) -> void), \
			EventHooks::## _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id ::func, \
			EventHooks::## _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id ::argdata, \
			2, 6, &m_InternalFunc \
		); \
		m_FnDef.Load(); \
} \
EventHooks::## _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id \
	::~ ## ## _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id PAREN_OPEN(x) PAREN_CLOSE(x) \
{ \
	Log("EventHooks::_" ## #_obj ## "Hook" ## #_event ## #_subevent ## #_priority ## #_id ## \
		"::dtor %p\n", reinterpret_cast<void*>(this)); \
	if (this->m_ZHLHookObj) \
	{ \
		delete this->m_ZHLHookObj; \
		this->m_ZHLHookObj = nullptr; \
	} \
} \
PVOID EventHooks::## _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id ::s_InternalSuper = nullptr; \
PVOID EventHooks::## _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id ::func = nullptr; \
__declspec(naked) auto __stdcall EventHooks::## _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id \
	::super PAREN_OPEN(x) CInstance *self, CInstance *other PAREN_CLOSE(x) -> void \
{ \
	__asm { jmp [EventHooks::## _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id ::s_InternalSuper] }; \
} \
static auto ___ ## _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id ## ___INITIALIZER = []() -> auto { \
	auto ret = std::thread([]() -> auto { \
		int retryCount = 0; \
		while (!ZHLDone()) { \
			std::this_thread::sleep_for(std::chrono::milliseconds(25)); \
			if (++retryCount > 5000) { \
				Log(#_obj ## "Hook" ## #_event ## #_subevent ## #_priority ## #_id ## \
					": %s", "Timed out waiting for ZHL Init to finish"\
				); \
				break; \
			} \
			continue; \
		} \
		std::this_thread::sleep_for( std::chrono::milliseconds( _priority * 3 ) ); \
		while (Direktor::s_InstallerLock.lock() != Direktor::LockStatus::LOCK_OKAY) { \
			std::this_thread::sleep_for(std::chrono::milliseconds(100)); \
		} \
		Direktor::s_InstalledHooks().push_back(new EventHooks:: _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id() ); \
		Direktor::s_InstalledHooks().back()->Init(); \
		Direktor::s_InstalledHooks().back()->Install(); \
		Direktor::s_InstallerLock.unlock(); \
	}); \
	ret.detach(); \
	return ret; \
}(); \
auto __stdcall EventHooks::## _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id ::hook (CInstance *self, CInstance *other) -> void


#define _DEFINE_EVENT_HOOK0(_id, _object, _event, _subevent, _priority) _DEFINE_EVENT_HOOK1(_id, _object, _event, _subevent, _priority)

#define HOOK_EVENT(_object, _event, _subevent) _DEFINE_EVENT_HOOK0(__LINE__, _object, _event, _subevent, 0)
   
#define _DEFINE_SCRIPT_HOOK1(_id, _script, _priority) \
namespace { namespace ScriptHooks { \
		class _ ## _script ## Hook ## _priority ## _id final : public ::Direktor::GMHook {  \
			static PVOID func; \
			static PVOID s_InternalSuper; \
			static const int32_t nArgs = 2; \
			static auto __stdcall hook (CInstance* Self, CInstance* Other, RValue* Result, int ArgumentCount, RValue** Arguments) -> RValue*;  \
			static auto __stdcall super (CInstance* Self, CInstance* Other, RValue* Result, int ArgumentCount, RValue** Arguments) -> RValue*; \
			static CScript* s_Script; \
			const char* const scriptName = #_script; \
			const char* const functionName = #_script ## "Hook" ## #_priority ## #_id;  \
			const short argdata[5] = { 0x1ff, 0x1ff, 0x1ff, 0x1ff, 0x1ff }; \
			PVOID *m_OutInternalSuper; \
			::ZHL::FunctionDefinition m_FnDef;  \
			FunctionHook_private *m_ZHLHookObj;  \
			PVOID m_InternalFunc; \
			const int32_t m_Priority = _priority; \
			 \
		public: \
			virtual ~ ## _ ## _script ## Hook ## _priority ## _id PAREN_OPEN(x) PAREN_CLOSE(x) final override; \
			_ ## _script ## Hook ## _priority ## _id PAREN_OPEN(x) PAREN_CLOSE(x) \
			: ::Direktor::GMHook(), m_FnDef(), m_ZHLHookObj(nullptr), m_InternalFunc(0), m_Priority(_priority) { \
				m_OutInternalSuper = &s_InternalSuper; \
			}; \
			virtual void Uninstall() override; \
			virtual bool Install() override; \
			virtual void Init() override; \
		}; \
	} \
} \
bool ScriptHooks::## _ ## _script ## Hook ## _priority ## _id ::Install() { \
	Log("ScriptHooks::_" ## #_script ## "Hook" ## #_priority ## #_id ## "::Install %p\n",reinterpret_cast<void*>(this)); \
	if (!this->m_ZHLHookObj){ \
		this->m_ZHLHookObj = new FunctionHook_private( \
			this->functionName, \
			typeid(auto (CInstance* Self, CInstance* Other, RValue* Result, int ArgumentCount, RValue** Arguments) -> RValue*), \
			ScriptHooks::## _ ## _script ## Hook ## _priority ## _id ::hook, \
			this->m_OutInternalSuper, \
			this->m_Priority \
		); \
		return this->m_ZHLHookObj->Install() != 0; \
	} \
	return false; \
} \
void ScriptHooks::## _ ## _script ## Hook ## _priority ## _id ::Uninstall() { \
	Log("ScriptHooks::_" ## #_script ## "Hook" ## #_priority ## #_id ## \
		"::Uninstall %p\n",reinterpret_cast<void*>(this)); \
	if (this->m_ZHLHookObj) \
	{ \
		Log("ScriptHooks::_" ## #_script ## "Hook" ## #_priority ## #_id ## \
			"::Uninstall - Actually Uninstalling something %p\n",reinterpret_cast<void*>(this)); \
		delete this->m_ZHLHookObj; \
		this->m_ZHLHookObj = nullptr; \
	} \
} \
void ScriptHooks::## _ ## _script ## Hook ## _priority ## _id ::Init() { \
	Log("ScriptHooks::_" ## #_script ## "Hook" ## #_priority ## #_id ## "::%s\n", "Init"); \
	int scriptID = Script_Find_Id(#_script) - 100000; \
	Log("ScriptHooks:Add - Script ID %d", scriptID); \
	CScript* theScript = ScriptFromId(scriptID); \
	if (!theScript)  \
		Error_Show_Action(true, true, "ScriptHooks::Add: Invalid script or index. Found ID %d", scriptID); \
	ScriptHooks::## _ ## _script ## Hook ## _priority ## _id ::s_Script = theScript; \
	Log("ScriptHooks::_" ## #_script ## "Hook" ## #_priority ## #_id ## \
		"::Init got script data %p, function %p\n", reinterpret_cast<void*>(theScript), reinterpret_cast<void*>(theScript->m_Functions->m_ScriptFunction)); \
	ScriptHooks::## _ ## _script ## Hook ## _priority ## _id ::func = reinterpret_cast<void*>(theScript->m_Functions->m_ScriptFunction); \
	if (!ScriptHooks::## _ ## _script ## Hook ## _priority ## _id ::func) \
		Error_Show_Action("EventHook::Add: Invalid event code or subcode", true, true); \
	::new (&m_FnDef) ::ZHL::FunctionDefinition( \
			functionName, \
			typeid(auto (CInstance* Self, CInstance* Other, RValue* Result, int ArgumentCount, RValue** Arguments) -> RValue*), \
			ScriptHooks::## _ ## _script ## Hook ## _priority ## _id ::func, \
			ScriptHooks::## _ ## _script ## Hook ## _priority ## _id ::argdata, \
			5, 2, &m_InternalFunc \
		); \
		m_FnDef.Load(); \
} \
ScriptHooks::## _ ## _script ## Hook ## _priority ## _id \
	::~ ## ## _ ## _script ## Hook ## _priority ## _id () \
{ \
	Log("ScriptHooks::_" ## #_script ## "Hook" ## #_priority ## #_id ## \
		"::dtor %p\n", reinterpret_cast<void*>(this)); \
	if (this->m_ZHLHookObj) \
	{ \
		delete this->m_ZHLHookObj; \
		this->m_ZHLHookObj = nullptr; \
	} \
} \
PVOID ScriptHooks::## _ ## _script ## Hook ## _priority ## _id ::s_InternalSuper = nullptr; \
PVOID ScriptHooks::## _ ## _script ## Hook ## _priority ## _id ::func = nullptr; \
CScript* ScriptHooks::## _ ## _script ## Hook ## _priority ## _id ::s_Script = nullptr; \
__declspec(naked) auto __stdcall ScriptHooks::## _ ## _script ## Hook ## _priority ## _id \
	::super (CInstance* Self, CInstance* Other, RValue* Result, int ArgumentCount, RValue** Arguments) -> RValue* \
{ \
	__asm { jmp [ScriptHooks::## _ ## _script ## Hook ## _priority ## _id ::s_InternalSuper] }; \
} \
static auto ___ ## _ ## _script ## Hook ## _priority ## _id ## ___INITIALIZER = []() -> auto { \
	auto ret = std::thread([]() -> auto { \
		int retryCount = 0; \
		while (!ZHLDone()) { \
			std::this_thread::sleep_for(std::chrono::milliseconds(25)); \
			if (++retryCount > 5000) { \
				Log(#_script ## "Hook" ## #_priority ## #_id ## \
					": %s", "Timed out waiting for ZHL Init to finish"\
				); \
				break; \
			} \
			continue; \
		} \
		std::this_thread::sleep_for( std::chrono::milliseconds( 10 + (_priority * 3) ) ); \
		while (Direktor::s_InstallerLock.lock() != Direktor::LockStatus::LOCK_OKAY) { \
			std::this_thread::sleep_for(std::chrono::milliseconds(100)); \
		} \
		Direktor::s_InstalledHooks().push_back(new ScriptHooks:: _ ## _script ## Hook ## _priority ## _id() ); \
		Direktor::s_InstalledHooks().back()->Init(); \
		Direktor::s_InstalledHooks().back()->Install(); \
		Direktor::s_InstallerLock.unlock(); \
	}); \
	ret.detach(); \
	return ret; \
}(); \
auto __stdcall ScriptHooks::## _ ## _script ## Hook ## _priority ## _id ::hook (CInstance* Self, CInstance* Other, RValue* Result, int ArgumentCount, RValue** Arguments) -> RValue*


#define _DEFINE_SCRIPT_HOOK0(_id, _script, _priority) _DEFINE_SCRIPT_HOOK1(_id, _script, _priority)

#define HOOK_SCRIPT(_script) _DEFINE_SCRIPT_HOOK0(__LINE__, _script, 0)