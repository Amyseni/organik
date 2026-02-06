#pragma once


#ifndef WIN32
#define WIN32 1
#endif
#ifndef _WIN32
#define _WIN32 1
#endif
#if defined(WIN64)
#undef WIN64
#endif
#if defined(_WIN64)
#undef _WIN64
#endif
#if defined(AMD64)
#undef AMD64
#endif
#if defined(_AMD64)
#undef _AMD64
#endif
#ifndef __i386__
#define __i386__ 1
#endif
#ifdef __amd64__
#undef __amd64__
#endif
#ifdef __AMD64__
#undef __AMD64__
#endif
#include "Windows.h"
#include <Arkitekt/Compiler.h>
#include <stdint.h>
#include <stack>
#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>
#include <map>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <shared_mutex>
#include <concepts>
#include <print>
#include <deque>
#include <thread>
struct Action;
namespace Arkitekt {
    struct FnBlock;
    using ::Action;
    struct ExecContext;
    struct Compiler;
    struct MemoryManager;
    constexpr std::hash<void*>* GetHasher();
    struct Blob;
    struct MemBlob;
};
using namespace Arkitekt;



#define GAME_EXE "SYNTHETIK.exe"
#define FUNC_NAKED __declspec(naked)
#define JUMP_INSTRUCTION(target) jmp [target]

// forward declaring
namespace MologieDetours{
    template <class FnType> class Detour;
};
namespace Arkitekt{
    struct Logger 
    {
    public:
        ~Logger();
        bool Init(const std::string& filename);
        void Cleanup(void);
        
        bool LogSimple(const char* text, bool flushLine = true);
        bool LogFormatted(const char* fmt, ...);
        std::string ParseFormatting(const char* fmt, ...);
        std::string ParseFormatting(const char* fmt, va_list args);
        Logger(std::filesystem::path path = std::filesystem::current_path(), const char* filename="organik.log");
    private:
        std::ofstream outFile;
        static std::shared_mutex logMutex;
        bool WriteToLog(const std::string& message, bool flushLine=true);
    };
    extern Arkitekt::Logger* GetLogger();
    extern void Init();
    extern void Done(bool status);
}


struct Definition {
public:
    virtual ~Definition() {}
    virtual const std::string_view GetName() const noexcept = 0;
    virtual void *__stdcall GetAddress() noexcept = 0;
};
struct FunctionDefinition : public Definition {
public:
    virtual const std::string_view GetName() const noexcept = 0;
    virtual void *__stdcall GetAddress() noexcept = 0;
    virtual std::vector<std::pair<void*, size_t>>& GetAllocatedHooks() 
    {
        static auto ret = std::vector<std::pair<void*, size_t>>();
        return ret;
    };
    virtual void* GetHook(void* hook) = 0;
    virtual void* GetSuper(void* super) = 0;
    virtual constexpr size_t GetArgCount() const noexcept = 0;
};

template<typename FnType>
struct FnBinding;

#define MaxSize MAX_DEFINITIONS_COUNT

template <typename T>
struct VariableDefinition;

template <typename T>
struct VariableDefinition<T*> : public Definition
{
public:
    using variable_type = std::add_lvalue_reference_t<std::remove_pointer_t<T*>>;
    using ptr_type = T;
private: 
    std::string_view m_Name;
    uint32_t m_Offset;
    ptr_type *_var = nullptr;
public:
    constexpr VariableDefinition(std::string_view name, uint32_t offset) : m_Name(name), m_Offset(offset), _var(nullptr) {}

    virtual constexpr const std::string_view GetName() const noexcept final override {
        return m_Name;
    }
    inline ptr_type* operator*() noexcept
    {
        if (_var) return _var;
        GetAddress();
        return _var;
    }
    inline const ptr_type* operator*() const noexcept
    {
        if (_var) return _var;
        GetAddress();
        return _var;
    }
    virtual void *__stdcall GetAddress() noexcept override
    {
        if (_var) return reinterpret_cast<void*>(_var);
        static auto AppBaseAddr = (uintptr_t) GetModuleHandleA("Direktor.dll");
        memcpy_s(&_var, sizeof(ptr_type), reinterpret_cast<void*>(AppBaseAddr + m_Offset), sizeof(ptr_type));
        return _var;
    }
};

struct FunctionHook
{
public:
    FunctionHook() = default;
    static std::multimap<int, FunctionHook*, std::greater<int>>& GetHookMap()
    {
        static std::multimap<int, FunctionHook*, std::greater<int>> HookMap;
        return HookMap;
    }
    FunctionDefinition* def;
    std::string_view name;
    void* address;
    void* _hook;
    void **_outInternalSuper;
    MologieDetours::Detour<void*>* _detour;
    unsigned long lblHook;
    
    template <class T> FunctionHook(std::string_view _name, FunctionDefinition* _def, T Hook, void*& outInternalSuper, int32_t priority)
        : name(_name), def(_def), _detour(nullptr), _outInternalSuper(&outInternalSuper), _hook(*(void**)&Hook)  {
            GetHookMap().emplace(std::pair(priority, this));
        }

    const std::string_view& GetName() const noexcept;
    void * GetAddress() const noexcept;
    void Install () ;
    virtual ~FunctionHook();

};
template <typename Ret>
struct FnBinding<Ret(__cdecl*)()> : public FunctionDefinition
{
public:
    using function_type = Ret(__cdecl*)();
private:
    void *m_Address = nullptr;
    std::string_view m_Name;
    uintptr_t m_Offset;
    std::string_view m_ModuleName;
public:
    FnBinding() = default;
    constexpr FnBinding(const char* name, uintptr_t offset, const char* sig) : m_Name(name), m_Offset(offset), m_ModuleName(sig) { }

    constexpr FnBinding(const std::string_view& name, uintptr_t offset, const char* sig) : m_Name(name), m_Offset(offset), m_ModuleName(sig) { }
    
    FnBinding(const FnBinding& other) : m_Name(other.name), m_Offset(other.offset), m_ModuleName(other.m_ModuleName) {}
    constexpr ~FnBinding() = default;

    constexpr size_t GetArgCount() const noexcept {
        return 0;
    }
    virtual const std::string_view GetName() const noexcept final override {
        return m_Name;
    }
    virtual void* GetSuper(void* super) override
    {  
        auto _compiler = Arkitekt::Compiler::Get();
        void** labels;
        _compiler->Begin(&labels);
        _compiler->EmitHookPrologue(0, std::is_same<Ret,void>::value, true);
        _compiler->EmitCall(super);
        _compiler->EmitHookEpilogue(0, std::is_same<Ret,void>::value, true);
        FnBlock* result = _compiler->FinalizeFunction(GetLogger()->ParseFormatting("super_%s_%p", GetName().data(), this->GetAddress()), labels);
        _compiler->End();
        return result->m_Fn;
    }
    virtual void* GetHook(void* hook) override
    {  
        auto _compiler = Arkitekt::Compiler::Get();
        void** labels;
        _compiler->Begin(&labels);
        _compiler->EmitHookPrologue(0, std::is_same<Ret,void>::value, true);
        _compiler->EmitCall(hook);
        _compiler->EmitHookEpilogue(0, std::is_same<Ret,void>::value, true);
        FnBlock* result = _compiler->FinalizeFunction(GetLogger()->ParseFormatting("hook_%s_%p", GetName().data(), this->GetAddress()), labels);
        _compiler->End();
        return result->m_Fn;
    }

    inline operator function_type()
    {
        return reinterpret_cast<function_type>(GetAddress());
    }

    virtual void *__stdcall GetAddress() noexcept override
    {
        if (m_Address) return m_Address;
        return m_Address = reinterpret_cast<void*>((uint32_t) GetModuleHandleA(nullptr) + (uint32_t) m_Offset);
    }

};
static inline std::map<std::string_view, Definition*>& GetDefinitions()
{
    static std::map<std::string_view, Definition*> _defs = std::map<std::string_view, Definition*>();
    return _defs;
}

template <typename Ret, typename... Args>
struct FnBinding<Ret(__cdecl*)(Args...)> : public FunctionDefinition
{
public:
    using function_type = Ret(__cdecl*)(Args...);
private:
    void *m_Address = nullptr;
    std::string_view m_Name;
    uintptr_t m_Offset;
    std::string_view m_ModuleName;
public:
    FnBinding() = default;
    constexpr FnBinding(const char* name, uintptr_t offset, const char* sig) : m_Name(name), m_Offset(offset), m_ModuleName(sig) { }
    constexpr FnBinding(const std::string_view& name, uintptr_t offset, const char* sig) : m_Name(name), m_Offset(offset), m_ModuleName(sig) { }
    FnBinding(const FnBinding& other) : m_Name(other.name), m_Offset(other.offset), m_ModuleName(other.m_ModuleName) {}
    constexpr ~FnBinding() = default;

    constexpr size_t GetArgCount() const noexcept {
        return (sizeof...(Args));
    }

    virtual void* GetSuper(void* super) override
    {  
        auto _compiler = Arkitekt::Compiler::Get();
        void** labels;
        _compiler->Begin(&labels);
        constexpr size_t argsSize = sizeof...(Args);
        _compiler->EmitHookPrologue(argsSize, std::is_same<Ret,void>::value, true);
        _compiler->EmitCall(super);
        _compiler->EmitHookEpilogue(argsSize, std::is_same<Ret,void>::value, true);
        FnBlock* result = _compiler->FinalizeFunction(GetLogger()->ParseFormatting("super_%s_%p", GetName().data(), this->GetAddress()), labels);
        _compiler->End();
        return result->m_Fn;
    }

    virtual void* GetHook(void* hook) override
    {  
        auto _compiler = Arkitekt::Compiler::Get();
        void** labels;
        _compiler->Begin(&labels);
        constexpr size_t argsSize = sizeof...(Args);
        _compiler->EmitHookPrologue(argsSize, std::is_same<Ret,void>::value, true);
        _compiler->EmitCall(hook);
        _compiler->EmitHookEpilogue(argsSize, std::is_same<Ret,void>::value, true);
        FnBlock* result = _compiler->FinalizeFunction(GetLogger()->ParseFormatting("hook_%s_%p", GetName().data(), this->GetAddress()), labels);

        _compiler->End();
        return result->m_Fn;
    }

    virtual const std::string_view GetName() const noexcept final override {
        return m_Name;
    }

    inline operator function_type()
    {
        return reinterpret_cast<function_type>(GetAddress());
    }
    
    virtual void *__stdcall GetAddress() noexcept override
    {
        if (m_Address) return m_Address;
        return m_Address = reinterpret_cast<void*>((uint32_t) GetModuleHandleA(nullptr) + (uint32_t) m_Offset);
    }
};
template <typename Ret, typename... Args>
struct FnBinding<Ret(__stdcall*)(Args...)> : public FunctionDefinition
{
public:
    using function_type = Ret(__stdcall*)(Args...);
private:
    void *m_Address = nullptr;
    std::string_view m_Name;
    uintptr_t m_Offset;
    std::string_view m_ModuleName;

public:
    FnBinding() = default;
    constexpr FnBinding(const char* name, uintptr_t offset, const char* sig) : m_Name(name), m_Offset(offset), m_ModuleName(sig) { }
    constexpr FnBinding(const std::string_view& name, uintptr_t offset, const char* sig) : m_Name(name), m_Offset(offset), m_ModuleName(sig) { }
    FnBinding(const FnBinding& other) : m_Name(other.name), m_Offset(other.offset), m_ModuleName(other.m_ModuleName) {}
    constexpr ~FnBinding() = default;

    virtual void* GetSuper(void* super) override
    {  
        auto _compiler = Arkitekt::Compiler::Get();
        void** labels;
        _compiler->Begin(&labels);
        constexpr size_t argsSize = sizeof...(Args);
        _compiler->EmitHookPrologue(argsSize, std::is_same<Ret,void>::value, false);
        _compiler->EmitCall(super);
        _compiler->EmitHookEpilogue(argsSize, std::is_same<Ret,void>::value, false);
        FnBlock* result = _compiler->FinalizeFunction(GetLogger()->ParseFormatting("super_%s_%p", this->GetName().data(), this->GetAddress()), labels);        _compiler->End();
        return result->m_Fn;
    }

    virtual void* GetHook(void* hook) override
    {  
        auto _compiler = Arkitekt::Compiler::Get();
        void** labels;
        _compiler->Begin(&labels);
        constexpr size_t argsSize = sizeof...(Args);
        _compiler->EmitHookPrologue(argsSize, std::is_same<Ret,void>::value, false);
        _compiler->EmitCall(hook);
        _compiler->EmitHookEpilogue(argsSize, std::is_same<Ret,void>::value, false);
        FnBlock* result = _compiler->FinalizeFunction(GetLogger()->ParseFormatting("hook_%s_%p", this->GetName().data(), this->GetAddress()), labels);
        _compiler->End();
        return result->m_Fn;
    }
    constexpr size_t GetArgCount() const noexcept {
        return (sizeof...(Args));
    }

    virtual const std::string_view GetName() const noexcept final override {
        return m_Name;
    }

    inline operator function_type()
    {
        return reinterpret_cast<function_type>(GetAddress());
    }

    virtual void *__stdcall GetAddress() noexcept override
    {
        if (m_Address) return m_Address;
        return m_Address = reinterpret_cast<void*>((uint32_t) GetModuleHandleA(nullptr) + (uint32_t) m_Offset);
    }
};

template <typename Ret, class classname>
struct FnBinding<Ret(classname::*)()> : public FunctionDefinition
{
public:
    using function_type = Ret(classname::*)(void);
    using function_type_const = Ret(classname::*)(void) const;
private:
    void *m_Address = nullptr;
    std::string_view m_Name;
    uintptr_t m_Offset;
    std::string_view m_ModuleName;

public:
    FnBinding() = default;
    constexpr FnBinding(const char* name, uintptr_t offset, const char* sig) : m_Name(name), m_Offset(offset), m_ModuleName(sig) { }
    constexpr FnBinding(const std::string_view& name, uintptr_t offset, const char* sig) : m_Name(name), m_Offset(offset), m_ModuleName(sig) { }
    FnBinding(const FnBinding& other) : m_Name(other.name), m_Offset(other.offset), m_ModuleName(other.m_ModuleName) {}
    constexpr ~FnBinding() = default;
    virtual void* GetSuper(void* super) override
    {  
        auto _compiler = Arkitekt::Compiler::Get();
        void** labels;
        _compiler->Begin(&labels);
        _compiler->EmitHookPrologue(0, std::is_same<Ret,void>::value, false);
        _compiler->EmitCall(super);
        _compiler->EmitHookEpilogue(0, std::is_same<Ret,void>::value, false);
        FnBlock* result = _compiler->FinalizeFunction(GetLogger()->ParseFormatting("super_%s_%p", GetName().data(), this->GetAddress()), labels);        _compiler->End();
        return result->m_Fn;
    }

    virtual void* GetHook(void* hook) override
    {  
        auto _compiler = Arkitekt::Compiler::Get();
        void** labels;
        _compiler->Begin(&labels);
        _compiler->EmitHookPrologue(0, std::is_same<Ret,void>::value, false);
        _compiler->EmitCall(hook);
        _compiler->EmitHookEpilogue(0, std::is_same<Ret,void>::value, false);
        FnBlock* result = _compiler->FinalizeFunction(GetLogger()->ParseFormatting("hook_%s_%p", GetName().data(), this->GetAddress()), labels);        _compiler->End();
        return result->m_Fn;
    }    
    constexpr size_t GetArgCount() const noexcept {
        return 0;
    }
    virtual const std::string_view GetName() const noexcept final override {
        return m_Name;
    }

    inline operator function_type()
    {
        GetAddress();
        return *reinterpret_cast<function_type*>(&m_Address);
    }

    virtual void *__stdcall GetAddress() noexcept override
    {
        if (m_Address) return m_Address;
        return m_Address = reinterpret_cast<void*>((uint32_t) GetModuleHandleA(nullptr) + (uint32_t) m_Offset);
    }

};

template <typename Ret, class classname, typename... Args>
struct FnBinding<Ret(classname::*)(Args...)> : public FunctionDefinition
{
public:
    using function_type = Ret(classname::*)(Args...);
    void *m_Address = nullptr;
private:
    std::string_view m_Name;
    uintptr_t m_Offset;
    std::string_view m_ModuleName;

public:
    FnBinding() = default;
    constexpr FnBinding(const char* name, uintptr_t offset, const char* sig) : m_Name(name), m_Offset(offset), m_ModuleName(sig) { }
    constexpr FnBinding(const std::string_view& name, uintptr_t offset, const char* sig) : m_Name(name), m_Offset(offset), m_ModuleName(sig) { }
    FnBinding(const FnBinding& other) : m_Name(other.name), m_Offset(other.offset), m_ModuleName(other.m_ModuleName) {}
    constexpr ~FnBinding() = default;

    constexpr size_t GetArgCount() const noexcept {
        return (sizeof...(Args));
    }
    virtual const std::string_view GetName() const noexcept final override {
        return m_Name;
    }
    virtual void* GetSuper(void* super) override
    {  
        auto _compiler = Arkitekt::Compiler::Get();
        void** labels;
        _compiler->Begin(&labels);
        constexpr size_t argsSize = sizeof...(Args);
        _compiler->EmitHookPrologue(argsSize, std::is_same<Ret,void>::value, false);
        _compiler->EmitCall(super);
        _compiler->EmitHookEpilogue(argsSize, std::is_same<Ret,void>::value, false);
        FnBlock* result = _compiler->FinalizeFunction(GetLogger()->ParseFormatting("super_%s_%p", GetName().data(), this->GetAddress()), labels);        _compiler->End();
        return result->m_Fn;
    }

    virtual void* GetHook(void* hook) override
    {  
        auto _compiler = Arkitekt::Compiler::Get();
        void** labels;
        _compiler->Begin(&labels);
        constexpr size_t argsSize = sizeof...(Args);
        _compiler->EmitHookPrologue(argsSize, std::is_same<Ret,void>::value, false);
        _compiler->EmitCall(hook);
        _compiler->EmitHookEpilogue(argsSize, std::is_same<Ret,void>::value, false);
        FnBlock* result = _compiler->FinalizeFunction(GetLogger()->ParseFormatting("hook_%s_%p", GetName().data(), this->GetAddress()), labels);        _compiler->End();
        return result->m_Fn;
    }    
    inline operator function_type()
    {
        GetAddress();
        return *reinterpret_cast<function_type*>(&m_Address);
    }

    virtual void *__stdcall GetAddress() noexcept override
    {
        if (m_Address) return m_Address;
        return m_Address = reinterpret_cast<void*>((uint32_t) GetModuleHandleA(nullptr) + (uint32_t) m_Offset);
    }
};


//
/* The following (until EOF) is from LibZHL by Kilburn (@FixItVinh) */
/* It is licensed under MIT 2.0 */
/* More details available in LICENSE.md */
//

//=================================================================================================
#define _DEFINE_METHOD_HOOK1(_id, _classname, _name, _priority, _type) \
	namespace { namespace Hook_##_classname##_name##_id##_priority { \
		static void *internalSuper = NULL; \
		struct wrapper : public _classname { \
			auto hook _type; \
			auto super _type ; \
		}; \
		static FunctionHook hookObj = FunctionHook(#_classname "::" #_name, &( _classname:: _func ## _ ## _name ), &wrapper::hook, internalSuper, _priority); \
	} } \
	auto FUNC_NAKED Hook_##_classname##_name##_id##_priority :: wrapper::super _type {__asm {JUMP_INSTRUCTION(Hook_##_classname##_name##_id##_priority ::internalSuper)}; } \
	auto Hook_##_classname##_name##_id##_priority ::wrapper::hook _type

#define _DEFINE_METHOD_HOOK0(_id, _classname, _name, _priority, ...) _DEFINE_METHOD_HOOK1(_id, _classname, _name, _priority, __VA_ARGS__)

#define HOOK_METHOD(_classname, _name, ...) _DEFINE_METHOD_HOOK0(__LINE__, _classname, _name, 0, __VA_ARGS__)
#define HOOK_METHOD_PRIORITY(_classname, _name, _priority, ...) _DEFINE_METHOD_HOOK0(__LINE__, _classname, _name, _priority, __VA_ARGS__)

//=================================================================================================

#define _DEFINE_STATIC_HOOK1(_id, _classname, _name, _callConv, _priority, _type) \
	namespace { namespace Hook_##_classname##_name##_id##_priority { \
		static void *internalSuper = NULL; \
		struct wrapper : public _classname { \
			static auto _callConv hook _type ; \
			static auto _callConv super _type ; \
		}; \
		static FunctionHook hookObj(#_classname "::" #_name, &_classname ## :: ## _name, &wrapper::hook, internalSuper, _priority); \
	} } \
	auto FUNC_NAKED _callConv Hook_##_classname##_name##_id##_priority :: wrapper::super _type {__asm {JUMP_INSTRUCTION(Hook_##_classname##_name##_id##_priority::internalSuper)}; } \
	auto _callConv Hook_##_classname##_name##_id##_priority ::wrapper::hook _type

#define _DEFINE_STATIC_HOOK0(_id, _classname, _name, _callConv, _priority, _type) _DEFINE_STATIC_HOOK1(_id, _classname, _name, _callConv, _priority, _type)

#define HOOK_STATIC_CONV(_classname, _name, _callConv, _type) _DEFINE_STATIC_HOOK0(__LINE__, _classname, _name, _callConv, 0, _type)
#define HOOK_STATIC(_classname, _name, _type) _DEFINE_STATIC_HOOK0(__LINE__, _classname, _name, __cdecl, 0, _type)
#define HOOK_STATIC_PRIORITY(_classname, _name, _priority, _type) _DEFINE_STATIC_HOOK0(__LINE__, _classname, _name, __cdecl, _priority, _type)

//====================================================== ===========================================

#define _DEFINE_GLOBAL_HOOK1(_id, _name, _callConv, _priority, _type) \
	namespace { namespace Hook_##_name##_id##_priority { \
		static void *internalSuper = NULL; \
		static auto _callConv hook _type ; \
		static auto _callConv super _type ; \
		\
		static FunctionHook hookObj(#_name, &_name, &hook, internalSuper, _priority); \
	} } \
	auto FUNC_NAKED _callConv Hook_##_name##_id##_priority ::super _type {__asm {JUMP_INSTRUCTION(Hook_##_name##_id##_priority::internalSuper)}; } \
	auto _callConv Hook_##_name##_id##_priority ::hook _type

#define _DEFINE_GLOBAL_HOOK0(_id, _name, _callConv, _priority, _type) _DEFINE_GLOBAL_HOOK1(_id, _name, _callConv, _priority, _type)

#define HOOK_GLOBAL_CONV(_name, _callConv, _type) _DEFINE_GLOBAL_HOOK0(__LINE__, _name, _callConv, 0, _type)
#define HOOK_GLOBAL(_name, _type) HOOK_GLOBAL_CONV(_name, __cdecl, _type)
#define HOOK_GLOBAL_PRIORITY(_name, _priority, _type) _DEFINE_GLOBAL_HOOK0(__LINE__, _name, __cdecl, _priority, _type)
namespace Direktor {
    enum LockStatus;
    struct Lock;
    struct RValueLock;
    class GMHook;
}
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

	static inline std::multimap<uint32_t, Direktor::GMHook*, std::greater<uint32_t>>& s_MapHookToInstallByPtrAndPriority() {
		static std::multimap<uint32_t, Direktor::GMHook*, std::greater<uint32_t>> m;
		return m;
	};
	static inline std::vector<Direktor::GMHook*>& s_InstalledHooks() {
		static std::vector<Direktor::GMHook*> m;
		return m;
	};
    class GMHook {
	public:
		virtual ~GMHook() = default;
		virtual void Init() = 0;
		virtual bool Install() = 0;
		virtual void Uninstall() = 0;

	};
}
static uintptr_t GameModuleBase = []() -> auto {
    return (uintptr_t) GetModuleHandleA(nullptr);
}();
#define PAREN_OPEN(x) \
   (
#define PAREN_CLOSE(x) \
   )
   
#define _DEFINE_EVENT_HOOK1(_id, _obj, _event, _subevent, _priority) \
namespace { namespace EventHooks { \
		class _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id final : public ::Direktor::GMHook {  \
			static PVOID func; \
			static PVOID s_InternalSuper; \
			static const int32_t eventCode = _event;  \
			static const int32_t subCode = _subevent;  \
			static auto __cdecl hook (CInstance *self, CInstance *other) -> void;  \
			static auto __cdecl super (CInstance *self, CInstance *other) -> void; \
			const char* const objName; \
			const char* const functionName = #_obj ## "Hook" ## #_event ## #_subevent ## #_priority ## #_id;  \
			PVOID *m_OutInternalSuper; \
			FnBinding<void (__cdecl*)(CInstance*, CInstance*)> m_FnDef;  \
			FunctionHook *m_HookObj;  \
			PVOID m_InternalFunc; \
			int32_t m_ObjIndex;  \
			const int32_t m_Priority = _priority; \
			 \
		public: \
			virtual ~ ## ## _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id PAREN_OPEN(x) PAREN_CLOSE(x) final override; \
			## _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id PAREN_OPEN(x) PAREN_CLOSE(x) \
			: ::Direktor::GMHook(), objName(#_obj), m_ObjIndex(0) , m_FnDef(), m_HookObj(nullptr), m_InternalFunc(0), m_Priority(_priority) { \
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
	if (!this->m_HookObj){ \
		this->m_HookObj = new FunctionHook( \
			this->functionName, \
			&this->m_FnDef, \
			EventHooks::## _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id ::hook, \
			*this->m_OutInternalSuper, \
			this->m_Priority \
		); \
		this->m_HookObj->Install(); \
        return true; \
	} \
	return false; \
} \
void EventHooks::## _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id ::Uninstall() { \
	Log("EventHooks::_" ## #_obj ## "Hook" ## #_event ## #_subevent ## #_priority ## #_id ## \
		"::Uninstall %p\n",reinterpret_cast<void*>(this)); \
	if (this->m_HookObj) \
	{ \
		Log("EventHooks::_" ## #_obj ## "Hook" ## #_event ## #_subevent ## #_priority ## #_id ## \
			"::Uninstall - Actually Uninstalling something %p\n",reinterpret_cast<void*>(this)); \
		delete this->m_HookObj; \
		this->m_HookObj = nullptr; \
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
	::new (&m_FnDef) FnBinding<void (__cdecl*)(CInstance*, CInstance*)>( \
			functionName, \
			(uintptr_t) EventHooks::## _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id ::func - GameModuleBase, \
            "SYNTHETIK.exe"\
		); \
} \
EventHooks::## _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id \
	::~ ## ## _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id PAREN_OPEN(x) PAREN_CLOSE(x) \
{ \
	Log("EventHooks::_" ## #_obj ## "Hook" ## #_event ## #_subevent ## #_priority ## #_id ## \
		"::dtor %p\n", reinterpret_cast<void*>(this)); \
	if (this->m_HookObj) \
	{ \
		delete this->m_HookObj; \
		this->m_HookObj = nullptr; \
	} \
} \
PVOID EventHooks::## _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id ::s_InternalSuper = nullptr; \
PVOID EventHooks::## _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id ::func = nullptr; \
__declspec(naked) auto __cdecl EventHooks::## _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id \
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
auto __cdecl EventHooks::## _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id ::hook (CInstance *self, CInstance *other) -> void


#define _DEFINE_EVENT_HOOK0(_id, _object, _event, _subevent, _priority) _DEFINE_EVENT_HOOK1(_id, _object, _event, _subevent, _priority)

#define HOOK_EVENT(_object, _event, _subevent) _DEFINE_EVENT_HOOK0(__LINE__, _object, _event, _subevent, 0)
   
#define _DEFINE_SCRIPT_HOOK1(_id, _script, _priority) \
namespace { namespace ScriptHooks { \
		class _ ## _script ## Hook ## _priority ## _id final : public ::Direktor::GMHook {  \
			static PVOID func; \
			static PVOID s_InternalSuper; \
			static const int32_t nArgs = 2; \
			static auto __cdecl hook (CInstance* Self, CInstance* Other, RValue* Result, int ArgumentCount, RValue** Arguments) -> RValue*;  \
			static auto __cdecl super (CInstance* Self, CInstance* Other, RValue* Result, int ArgumentCount, RValue** Arguments) -> RValue*; \
			static CScript* s_Script; \
			const char* const scriptName = #_script; \
			const char* const functionName = #_script ## "Hook" ## #_priority ## #_id;  \
			const short argdata[5] = { 0x1ff, 0x1ff, 0x1ff, 0x1ff, 0x1ff }; \
			PVOID *m_OutInternalSuper; \
			FnBinding<RValue* (__cdecl*)(CInstance* Self, CInstance* Other, RValue* Result, int ArgumentCount, RValue** Arguments)> m_FnDef;  \
			FunctionHook *m_HookObj;  \
			PVOID m_InternalFunc; \
			const int32_t m_Priority = _priority; \
			 \
		public: \
			virtual ~ ## _ ## _script ## Hook ## _priority ## _id PAREN_OPEN(x) PAREN_CLOSE(x) final override; \
			_ ## _script ## Hook ## _priority ## _id PAREN_OPEN(x) PAREN_CLOSE(x) \
			: ::Direktor::GMHook(), m_FnDef(), m_HookObj(nullptr), m_InternalFunc(0), m_Priority(_priority) { \
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
	if (!this->m_HookObj){ \
		this->m_HookObj = new FunctionHook( \
			this->functionName, \
			&this->m_FnDef, \
			ScriptHooks::## _ ## _script ## Hook ## _priority ## _id ::hook, \
			*this->m_OutInternalSuper, \
			this->m_Priority \
		); \
		this->m_HookObj->Install(); \
        return true; \
	} \
	return false; \
} \
void ScriptHooks::## _ ## _script ## Hook ## _priority ## _id ::Uninstall() { \
	Log("ScriptHooks::_" ## #_script ## "Hook" ## #_priority ## #_id ## \
		"::Uninstall %p\n",reinterpret_cast<void*>(this)); \
	if (this->m_HookObj) \
	{ \
		Log("ScriptHooks::_" ## #_script ## "Hook" ## #_priority ## #_id ## \
			"::Uninstall - Actually Uninstalling something %p\n",reinterpret_cast<void*>(this)); \
		delete this->m_HookObj; \
		this->m_HookObj = nullptr; \
	} \
} \
void ScriptHooks::## _ ## _script ## Hook ## _priority ## _id ::Init() { \
	Log("ScriptHooks::_" ## #_script ## "Hook" ## #_priority ## #_id ## "::%s\n", "Init"); \
	int scriptID = Script_Find_Id(#_script) - 100000; \
	Log("ScriptHooks:Add - Script ID %d", scriptID); \
	CScript* theScript = ScriptFromId(scriptID); \
	if (!theScript)  \
		Error_Show_Action_Alt(true, true, "ScriptHooks::Add: Invalid script or index. Found ID %d", scriptID); \
	ScriptHooks::## _ ## _script ## Hook ## _priority ## _id ::s_Script = theScript; \
	Log("ScriptHooks::_" ## #_script ## "Hook" ## #_priority ## #_id ## \
		"::Init got script data %p, function %p\n", reinterpret_cast<void*>(theScript), reinterpret_cast<void*>(theScript->m_Functions->m_ScriptFunction)); \
	ScriptHooks::## _ ## _script ## Hook ## _priority ## _id ::func = reinterpret_cast<void*>(theScript->m_Functions->m_ScriptFunction); \
	if (!ScriptHooks::## _ ## _script ## Hook ## _priority ## _id ::func) \
		Error_Show_Action("EventHook::Add: Invalid event code or subcode", true, true); \
	::new (&m_FnDef) FnBinding<RValue* (__cdecl*)(CInstance* Self, CInstance* Other, RValue* Result, int ArgumentCount, RValue** Arguments)>( \
			functionName, \
			(uintptr_t) ScriptHooks::## _ ## _script ## Hook ## _priority ## _id ::func - GameModuleBase, \
            "SYNTHETIK.exe" \
		); \
} \
ScriptHooks::## _ ## _script ## Hook ## _priority ## _id \
	::~ ## ## _ ## _script ## Hook ## _priority ## _id () \
{ \
	Log("ScriptHooks::_" ## #_script ## "Hook" ## #_priority ## #_id ## \
		"::dtor %p\n", reinterpret_cast<void*>(this)); \
	if (this->m_HookObj) \
	{ \
		delete this->m_HookObj; \
		this->m_HookObj = nullptr; \
	} \
} \
PVOID ScriptHooks::## _ ## _script ## Hook ## _priority ## _id ::s_InternalSuper = nullptr; \
PVOID ScriptHooks::## _ ## _script ## Hook ## _priority ## _id ::func = nullptr; \
CScript* ScriptHooks::## _ ## _script ## Hook ## _priority ## _id ::s_Script = nullptr; \
__declspec(naked) auto __cdecl ScriptHooks::## _ ## _script ## Hook ## _priority ## _id \
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
auto __cdecl ScriptHooks::## _ ## _script ## Hook ## _priority ## _id ::hook (CInstance* Self, CInstance* Other, RValue* Result, int ArgumentCount, RValue** Arguments) -> RValue*


#define _DEFINE_SCRIPT_HOOK0(_id, _script, _priority) _DEFINE_SCRIPT_HOOK1(_id, _script, _priority)

#define HOOK_SCRIPT(_script) _DEFINE_SCRIPT_HOOK0(__LINE__, _script, 0)