#pragma once
#include <shared_mutex>
#include <iterator>
#include <Windows.h>
#include <unordered_map>
#include <cstdio>
#include <functional>
#include <memory>
#include <map>
#include <string>
#include <iostream>
#include <type_traits>
#include <Windows.h>

#ifdef _WIN32
#define FUNC_NAKED __declspec(naked)
#ifdef LIBARK_EXPORTS
#define LIBARK_API __declspec(dllexport)
#else
#define LIBARK_API __declspec(dllimport)
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
		fprintf(file, fmt __VA_OPT__(,) __VA_ARGS__); \
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
#include <detours.h>
#include <functional>
#include <alg.h>
#include <variant>
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
__declspec(dllexport) void __cdecl LOCK_RVALUE_MUTEX();
__declspec(dllexport) void __cdecl UNLOCK_RVALUE_MUTEX();
namespace ZHL { class Definition; }
using namespace ZHL;
extern std::unordered_map<size_t, Definition*>& DefsByHash();

namespace ZHL
{

class Definition
{
public:
	static int Init();
	static const char *GetLastError();

	template<typename T>
	static Definition* Find(const char* name)
	{
		auto it = DefsByHash().find(CHashMapCalculateHash(name) ^ typeid(T).hash_code());
		if (it != DefsByHash().end())
			return (*it).second;
		else
			return NULL;
	}


protected:
	static void Add(const char* name, size_t typeHash, Definition* def);

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
	template <typename T>
	FunctionDefinition(const char* name, const std::type_identity<T>& type, const char* sig, const short* argdata, int nArgs, unsigned int flags, void** outfunc) :
		_sig(sig),
		_argdata(argdata),
		_nArgs(nArgs),
		_flags(flags),
		_outFunc(outfunc)
	{


		SetName(name, typeid(T).name());
		strcpy(_name, name);

		Add(_name, typeid(T).hash_code(), this);

	}
	// template<typename Ret, typename... Args, Operand ... operands, is_register retLoc, is_register... regsUsed>
	// FunctionDefinition(const char *name, const std::type_info &type, const char* sig, const short *argdata, int nArgs, unsigned int flags, void **outfunc);
	template <typename T>
	FunctionDefinition(const char* name, const std::type_identity<T>&, void* addr, const short* argdata, int nArgs, unsigned int flags, void** outfunc) :
		_address(addr),
		_argdata(argdata),
		_nArgs(nArgs),
		_flags(flags),
		_outFunc(outfunc)
	{


		SetName(name, typeid(T).name());
		strcpy(_name, name);

		Add(_name, typeid(T).hash_code(), this);

	}


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
		Add(_name, typeid(void***).hash_code(), this);
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
		Add(_name, typeid(void).hash_code(), this);
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
	int Install() { return 0; };

private:
	static void Add(FunctionHook_private *hook);
	void SetName(const char *name, const char *type);

public:
	FunctionHook_private(const char *name, const std::type_info &type, void *hook, void **outInternalSuper, int priority);
	~FunctionHook_private() {};
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
	LIBARK_API bool Init();
	LIBARK_API void Done(bool status);
}

class LIBARK_API FunctionHook
{
private:
	static void Add(const char *name, const std::type_info &type, void *hook, void **outInternalSuper, int priority);

public:
	template <class T> FunctionHook(const char *name, const std::type_info &type, T hook, void **outInternalSuper, int priority)
	{
		Add(name, type, *(void**)&hook, outInternalSuper, priority);
	}
};
void Error_Show_Action_Alt(bool mustCrash, bool manualError, const char* fmt, ...);
// void Error_Show_Action(bool mustCrash, bool manErr, const char* fmt, ...);
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
// namespace Arkitekt
// {
// 	struct FnBlock
// 	{
// 		void* m_Mem;
// 		size_t m_Size;
// 		/// @brief pointer to the function itself, used to call the function
// 		void* m_Fn;
// 		void** m_Labels;
// 		constexpr FnBlock();
// 		FnBlock(void* _mem, size_t _size, void* _fn, void*** labels);
// 		static Arkitekt::FnBlock& Create(FnBlock* ptr, const std::string_view& name, void* _mem, size_t _sz, void* _fn, void**& labels);
// 	};
// }
// using Arkitekt::FnBlock;
#include <Arkitekt/Assembler.win32.hpp>
#define __cdecl_cleanup true
#define __stdcall_cleanup false
struct InstalledHook;
extern std::vector<InstalledHook*>& GetInstalledHooks();
extern std::shared_mutex& GetInstallMapMutex();
extern std::multimap<int, std::function<auto ()->InstalledHook*>*, std::greater<int>>& GetHookMap();
extern std::thread* GetInstallerThread();

struct InstalledHook
{
	std::string_view name;
	MologieDetours::Detour<void*>* detour = nullptr;
	FnBlock* hookBlock = nullptr;
	FnBlock* superBlock = nullptr;

	InstalledHook(const std::string_view& _name, MologieDetours::Detour<void*>* _detour, FnBlock* _hookBlock, FnBlock* _superBlock)
		: name(_name.data()), detour(_detour), hookBlock(_hookBlock), superBlock(_superBlock) {}

	template <typename T, bool NeedCallerCleanup>
	static InstalledHook* Install(void** outInternalSuper, void* fn, void* hook)
	{
		std::string_view _name = typeid(T).name();
		auto hookWrapper = CreateWrapper<T, NeedCallerCleanup>((std::string(_name.data()) + "Hook").c_str(), hook);
		MologieDetours::Detour<void*>* _detour = new MologieDetours::Detour<void*>(fn, hookWrapper->m_Fn);
		auto superWrapper = CreateWrapper<T, NeedCallerCleanup>((std::string(_name.data()) + "Super").c_str(), _detour->GetOriginalFunction());
		if (outInternalSuper)
			*outInternalSuper = superWrapper->m_Fn;
		else
			throw new std::bad_function_call;

		return new InstalledHook(typeid(T).name(), _detour, hookWrapper, superWrapper);
	}

	template <typename T, bool NeedCallerCleanup = false>
		requires (!(std::same_as<void*, T>))
	static InstalledHook* Install(void** outInternalSuper, void* fn, T hook)
	{
		std::string_view _name = typeid(T).name();
		auto hookWrapper = CreateWrapper<T, NeedCallerCleanup>((std::string(_name.data()) + "Hook").c_str(), *(void**)(&hook));
		MologieDetours::Detour<void*>* _detour = new MologieDetours::Detour<void*>(*(void**)(&fn), hookWrapper->m_Fn);
		auto superWrapper = CreateWrapper<T, NeedCallerCleanup>((std::string(_name.data()) + "Super").c_str(), _detour->GetOriginalFunction());
		if (outInternalSuper)
			*outInternalSuper = superWrapper->m_Fn;
		else
			throw new std::bad_function_call;

		return new InstalledHook(typeid(T).name(), _detour, hookWrapper, superWrapper);
	}

	~InstalledHook()
	{
		if (detour)
			delete detour;
		if (hookBlock)
			delete hookBlock;
		if (superBlock)
			delete superBlock;
	}
};

template <typename T>
concept _is_func = std::is_member_function_pointer_v<T> || requires(T* fn)
{
	{ std::is_function_v<decltype(*fn)> } -> std::convertible_to<bool>;
};
void* GetAddr(_is_func auto fn)
{
	auto* pFn = &fn;
	auto** ppFn = &pFn;
	return *(void**)ppFn;
}

template <typename T>
struct MemberHook;

template <typename Ret, typename Classname, typename...Args>
struct MemberHook<Ret(__thiscall Classname::*)(Args...)> : public Classname
{};

//=================================================================================================
#define _DEFINE_METHOD_HOOK1(_id, _classname, _name, _priority, ...) \
	namespace { namespace Hook_##_classname##_name##_id##_priority { \
		static void *internalSuper = NULL; \
		struct wrapper : public _classname { \
			auto hook __VA_ARGS__ ; \
			auto super __VA_ARGS__ ; \
			constexpr wrapper() {}; \
		}; \
		auto installer = [&](){ \
			std::shared_lock<std::shared_mutex> lock(GetInstallMapMutex()); \
			GetHookMap().emplace(_priority, new std::function<auto () -> InstalledHook*>([=]() -> InstalledHook* { \
				FunctionDefinition *def = dynamic_cast<FunctionDefinition*>( \
					Definition::Find<decltype(&_classname:: _name) >( \
						#_classname"::" #_name \
					)); \
				if (!def) {  \
					MessageBoxA(NULL, #_classname "::" #_name, "Fuck", MB_OK); \
				} \
				Log("%s %p", #_name, def->GetAddress()); \
				return InstalledHook::Install(\
					&internalSuper, def->GetAddress(), &wrapper::hook \
				); \
				})); \
				return true; \
				}(); \
				\
	} } \
	auto FUNC_NAKED Hook_##_classname##_name##_id##_priority :: wrapper::super __VA_ARGS__ {__asm {JUMP_INSTRUCTION(Hook_##_classname##_name##_id##_priority ::internalSuper)}; } \
	auto Hook_##_classname##_name##_id##_priority ::wrapper::hook __VA_ARGS__

#define _DEFINE_METHOD_HOOK0(_id, _classname, _name, _priority, ...) _DEFINE_METHOD_HOOK1(_id, _classname, _name, _priority, __VA_ARGS__)

#define HOOK_METHOD(_classname, _name, ...) _DEFINE_METHOD_HOOK0(__LINE__, _classname, _name, 0, __VA_ARGS__)
#define HOOK_METHOD_PRIORITY(_classname, _name, _priority, ...) _DEFINE_METHOD_HOOK0(__LINE__, _classname, _name, _priority, __VA_ARGS__)

//=================================================================================================

#define _DEFINE_STATIC_HOOK1(_id, _classname, _name, _priority, _type, _conv) \
namespace { namespace Hook_##_name##_id##_priority { \
	static void* internalSuper = NULL; \
	struct wrapper : public _classname { \
		static auto _conv hook _type; \
		static auto _conv super _type; \
	}; \
	auto installer = [&](){ \
		std::shared_lock<std::shared_mutex> lock(GetInstallMapMutex()); \
		GetHookMap().emplace(_priority, new std::function<auto () -> InstalledHook*>([=]() -> InstalledHook* { \
			FunctionDefinition *def = dynamic_cast<FunctionDefinition*>( \
				Definition::Find<decltype(&_classname:: _name)>( \
					#_classname"::"#_name, \
				)); \
			Log("%s %p", #_name, def->GetAddress()); \
			return InstalledHook::Install<decltype(&_classname ::_name), EXPAND_THEN_CONCATENATE(_conv, _cleanup)>( \
				&internalSuper, def->GetAddress(), &wrapper::hook \
			); \
			return true; \
		})); \
	}(); } } \
	auto FUNC_NAKED _conv Hook_##_classname##_name##_id##_priority :: wrapper::super _type {__asm {JUMP_INSTRUCTION(Hook_##_classname##_name##_id##_priority::internalSuper)}; } \
	auto _conv Hook_##_classname##_name##_id##_priority ::wrapper::hook _type

#define _DEFINE_STATIC_HOOK0(_id, _classname, _name, _priority, _type) _DEFINE_STATIC_HOOK1(_id, _classname, _name, _priority, _type, __cdecl)

#define HOOK_STATIC(_classname, _name, _type) _DEFINE_STATIC_HOOK0(__LINE__, _classname, _name, 0, _type)
#define HOOK_STATIC_PRIORITY(_classname, _name, _priority, _type) _DEFINE_STATIC_HOOK0(__LINE__, _classname, _name, _priority, _type)

//====================================================== ===========================================

#define _DEFINE_GLOBAL_HOOK1(_id, _name, _priority, _type, _conv) \
	namespace { namespace Hook_##_name##_id##_priority { \
		static void* internalSuper = NULL; \
        static auto _conv hook _type; \
        static auto _conv super _type; \
        auto installer = [&](){ \
			std::shared_lock<std::shared_mutex> lock(GetInstallMapMutex()); \
			GetHookMap().emplace(_priority, new std::function<auto () -> InstalledHook*>([=]() -> InstalledHook* { \
				FunctionDefinition *def = dynamic_cast<FunctionDefinition*>( \
					Definition::Find<decltype(&_name)>( \
						#_name \
					)); \
				Log("%s %p", #_name, def->GetAddress()); \
				return InstalledHook::Install<decltype(&_name) , EXPAND_THEN_CONCATENATE(_conv, _cleanup)>( \
					&internalSuper, def->GetAddress(), &hook \
				); \
			})); \
			return true; \
		}(); \
		\
	} } \
	auto FUNC_NAKED _conv Hook_##_name##_id##_priority ::super _type {__asm {JUMP_INSTRUCTION(Hook_##_name##_id##_priority::internalSuper)}; } \
	auto _conv Hook_##_name##_id##_priority ::hook _type

#define _DEFINE_GLOBAL_HOOK0(_id, _name, _priority, _type) _DEFINE_GLOBAL_HOOK1(_id, _name, _priority, _type, __cdecl)

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
			const char* const functionName = #_obj  "Hook" #_event  #_subevent  #_priority  #_id;  \
			const short argdata[2] = { 0x1ff, 0x1ff }; \
			PVOID *m_OutInternalSuper; \
			::ZHL::FunctionDefinition m_FnDef;  \
			FunctionHook_private *m_ZHLHookObj;  \
			PVOID m_InternalFunc; \
			int32_t m_ObjIndex;  \
			const int32_t m_Priority = _priority; \
			 \
		public: \
			virtual ~ _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id PAREN_OPEN(x) PAREN_CLOSE(x) final override; \
			_ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id PAREN_OPEN(x) PAREN_CLOSE(x) \
			: ::Direktor::GMHook(), objName(#_obj), m_ObjIndex(0) , m_FnDef(), m_ZHLHookObj(nullptr), m_InternalFunc(0), m_Priority(_priority) { \
				m_OutInternalSuper = &s_InternalSuper; \
			}; \
			virtual void Uninstall() override; \
			virtual bool Install() override; \
			virtual void Init() override; \
		}; \
	} \
} \
bool EventHooks::  _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id ::Install() { \
	Log("EventHooks::_" #_obj "Hook" #_event #_subevent #_priority #_id "::Install %p\n",reinterpret_cast<void*>(this)); \
	if (!this->m_ZHLHookObj){ \
		this->m_ZHLHookObj = new FunctionHook_private( \
			this->functionName, \
			typeid(auto (__cdecl*)(CInstance*, CInstance*) -> void), \
			EventHooks::  _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id ::hook, \
			this->m_OutInternalSuper, \
			this->m_Priority \
		); \
		return this->m_ZHLHookObj->Install() != 0; \
	} \
	return false; \
} \
void EventHooks::  _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id ::Uninstall() { \
	Log("EventHooks::_" #_obj  "Hook" #_event  #_subevent  #_priority  #_id  \
		"::Uninstall %p\n",reinterpret_cast<void*>(this)); \
	if (this->m_ZHLHookObj) \
	{ \
		Log("EventHooks::_" #_obj  "Hook" #_event  #_subevent  #_priority  #_id  \
			"::Uninstall - Actually Uninstalling something %p\n",reinterpret_cast<void*>(this)); \
		delete this->m_ZHLHookObj; \
		this->m_ZHLHookObj = nullptr; \
	} \
} \
void EventHooks::  _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id ::Init() { \
	Log("EventHooks::_" #_obj  "Hook" #_event  #_subevent  #_priority  #_id  "::%s\n", "Init"); \
	RValue rv = DoBuiltin(&gml_asset_get_index, #_obj ); \
	m_ObjIndex = parseRValueNumber<int32_t>(rv); \
	if (m_ObjIndex < 1)  \
		Error_Show_Action("EventHook::Add: Invalid object index", true, true); \
	auto objectData = Object_Data(m_ObjIndex); \
	if (!objectData)  \
		Error_Show_Action("EventHook::Add: Invalid object data", true, true); \
	auto cEvent = objectData->GetEventRecursive(eventCode, subCode); \
	Log("EventHooks::_" #_obj  "Hook" #_event  #_subevent  #_priority  #_id  \
		"::Init got object data %p, event data %p\n",reinterpret_cast<void*>(objectData), reinterpret_cast<void*>(cEvent)); \
	if (cEvent) \
	{ \
		EventHooks::  _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id ::func = reinterpret_cast<void*>(cEvent->m_Code ? cEvent->m_Code->m_Functions ? cEvent->m_Code->m_Functions->m_CodeFunction : nullptr : nullptr); \
		Log("EventHooks::_" #_obj  "Hook" #_event  #_subevent  #_priority  #_id  \
			"::Init found event %p\n",reinterpret_cast<void*>(cEvent)); \
	} \
	if (!EventHooks::  _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id ::func) \
		Error_Show_Action("EventHook::Add: Invalid event code or subcode", true, true); \
	::new (&m_FnDef) ::ZHL::FunctionDefinition( \
			functionName, \
			typeid(auto (CInstance*, CInstance*) -> void), \
			EventHooks::  _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id ::func, \
			EventHooks::  _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id ::argdata, \
			2, 6, &m_InternalFunc \
		); \
		m_FnDef.Load(); \
} \
EventHooks::  _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id \
	::~ _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id PAREN_OPEN(x) PAREN_CLOSE(x) \
{ \
	Log("EventHooks::_" #_obj  "Hook" #_event  #_subevent  #_priority  #_id  \
		"::dtor %p\n", reinterpret_cast<void*>(this)); \
	if (this->m_ZHLHookObj) \
	{ \
		delete this->m_ZHLHookObj; \
		this->m_ZHLHookObj = nullptr; \
	} \
} \
PVOID EventHooks::  _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id ::s_InternalSuper = nullptr; \
PVOID EventHooks::  _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id ::func = nullptr; \
__declspec(naked) auto __stdcall EventHooks::  _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id \
	::super PAREN_OPEN(x) CInstance *self, CInstance *other PAREN_CLOSE(x) -> void \
{ \
	__asm { jmp [EventHooks::  _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id ::s_InternalSuper] }; \
} \
auto __stdcall EventHooks:: _ ## _obj ## Hook ## _event ## _subevent ## _priority ## _id ::hook (CInstance *self, CInstance *other) -> void


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
			const char* const functionName = #_script  "Hook" #_priority  #_id;  \
			const short argdata[5] = { 0x1ff, 0x1ff, 0x1ff, 0x1ff, 0x1ff }; \
			PVOID *m_OutInternalSuper; \
			::ZHL::FunctionDefinition m_FnDef;  \
			FunctionHook_private *m_ZHLHookObj;  \
			PVOID m_InternalFunc; \
			const int32_t m_Priority = _priority; \
			 \
		public: \
			virtual ~ _ ## _script ## Hook ## _priority ## _id PAREN_OPEN(x) PAREN_CLOSE(x) final override; \
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
bool ScriptHooks::  _ ## _script ## Hook ## _priority ## _id ::Install() { \
	Log("ScriptHooks::_" #_script  "Hook" #_priority  #_id  "::Install %p\n",reinterpret_cast<void*>(this)); \
	if (!this->m_ZHLHookObj){ \
		this->m_ZHLHookObj = new FunctionHook_private( \
			this->functionName, \
			typeid(auto (CInstance* Self, CInstance* Other, RValue* Result, int ArgumentCount, RValue** Arguments) -> RValue*), \
			ScriptHooks::  _ ## _script ## Hook ## _priority ## _id ::hook, \
			this->m_OutInternalSuper, \
			this->m_Priority \
		); \
		return this->m_ZHLHookObj->Install() != 0; \
	} \
	return false; \
} \
void ScriptHooks::  _ ## _script ## Hook ## _priority ## _id ::Uninstall() { \
	Log("ScriptHooks::_" #_script  "Hook" #_priority  #_id  \
		"::Uninstall %p\n",reinterpret_cast<void*>(this)); \
	if (this->m_ZHLHookObj) \
	{ \
		Log("ScriptHooks::_" #_script  "Hook" #_priority  #_id  \
			"::Uninstall - Actually Uninstalling something %p\n",reinterpret_cast<void*>(this)); \
		delete this->m_ZHLHookObj; \
		this->m_ZHLHookObj = nullptr; \
	} \
} \
void ScriptHooks::  _ ## _script ## Hook ## _priority ## _id ::Init() { \
	Log("ScriptHooks::_" #_script  "Hook" #_priority  #_id  "::%s\n", "Init"); \
	int scriptID = Script_Find_Id(#_script) - 100000; \
	Log("ScriptHooks:Add - Script ID %d", scriptID); \
	CScript* theScript = ScriptFromId(scriptID); \
	if (!theScript)  \
	Error_Show_Action_Alt(true, true, "ScriptHooks::Add: Invalid script or index. Found ID %d", scriptID); \
	ScriptHooks::  _ ## _script ## Hook ## _priority ## _id ::s_Script = theScript; \
	Log("ScriptHooks::_" #_script  "Hook" #_priority  #_id  \
		"::Init got script data %p, function %p\n", reinterpret_cast<void*>(theScript), reinterpret_cast<void*>(theScript->m_Functions->m_ScriptFunction)); \
	ScriptHooks::  _ ## _script ## Hook ## _priority ## _id ::func = reinterpret_cast<void*>(theScript->m_Functions->m_ScriptFunction); \
	if (!ScriptHooks::  _ ## _script ## Hook ## _priority ## _id ::func) \
		Error_Show_Action("EventHook::Add: Invalid event code or subcode", true, true); \
	::new (&m_FnDef) ::ZHL::FunctionDefinition( \
			functionName, \
			typeid(auto (CInstance* Self, CInstance* Other, RValue* Result, int ArgumentCount, RValue** Arguments) -> RValue*), \
			ScriptHooks::  _ ## _script ## Hook ## _priority ## _id ::func, \
			ScriptHooks::  _ ## _script ## Hook ## _priority ## _id ::argdata, \
			5, 2, &m_InternalFunc \
		); \
		m_FnDef.Load(); \
} \
ScriptHooks::  _ ## _script ## Hook ## _priority ## _id \
	::~ _ ## _script ## Hook ## _priority ## _id () \
{ \
	Log("ScriptHooks::_" #_script  "Hook" #_priority  #_id  \
		"::dtor %p\n", reinterpret_cast<void*>(this)); \
	if (this->m_ZHLHookObj) \
	{ \
		delete this->m_ZHLHookObj; \
		this->m_ZHLHookObj = nullptr; \
	} \
} \
PVOID ScriptHooks::  _ ## _script ## Hook ## _priority ## _id ::s_InternalSuper = nullptr; \
PVOID ScriptHooks::  _ ## _script ## Hook ## _priority ## _id ::func = nullptr; \
CScript* ScriptHooks::  _ ## _script ## Hook ## _priority ## _id ::s_Script = nullptr; \
__declspec(naked) auto __stdcall ScriptHooks::  _ ## _script ## Hook ## _priority ## _id \
	::super (CInstance* Self, CInstance* Other, RValue* Result, int ArgumentCount, RValue** Arguments) -> RValue* \
{ \
	__asm { jmp [ScriptHooks::  _ ## _script ## Hook ## _priority ## _id ::s_InternalSuper] }; \
} \
auto __stdcall ScriptHooks::  _ ## _script ## Hook ## _priority ## _id ::hook (CInstance* Self, CInstance* Other, RValue* Result, int ArgumentCount, RValue** Arguments) -> RValue*


#define _DEFINE_SCRIPT_HOOK0(_id, _script, _priority) _DEFINE_SCRIPT_HOOK1(_id, _script, _priority)

#define HOOK_SCRIPT(_script) _DEFINE_SCRIPT_HOOK0(__LINE__, _script, 0)


#define EXPAND(x)										x
#define GET_SCRIPTMACRO(_0, _1, _2, _3, _4, _5, _6,  \
						_7, _8, _9, _10, _11, _12, 	 \
						_13, _14, _15, _16, name, ...) name

#define __MAKEARGS0(_script) \
		_script ()
#define __MAKEPARAMS0(_script) \
		= std::vector<RValue*>();
#define __MAKEARGS1(_script, arg1) \
		_script (RValue arg1)
#define __MAKEPARAMS1(_script, arg1) \
		= { &arg1 };
#define __MAKEARGS2(_script, arg1, arg2) \
		_script (RValue arg1, RValue arg2)
#define __MAKEPARAMS2(_script, arg1, arg2) \
		= { &arg1, &arg2 };
#define __MAKEARGS3(_script, arg1, arg2, arg3) \
        _script(RValue arg1, RValue arg2, RValue arg3)
#define __MAKEPARAMS3(_script, arg1, arg2, arg3) \
        = { &arg1, &arg2, &arg3 };
#define __MAKEARGS4(_script, arg1, arg2, arg3, arg4) \
        _script(RValue arg1, RValue arg2, RValue arg3, RValue arg4)
#define __MAKEPARAMS4(_script, arg1, arg2, arg3, arg4) \
        = { &arg1, &arg2, &arg3, &arg4 };
#define __MAKEARGS5(_script, arg1, arg2, arg3, arg4, arg5) \
        _script(RValue arg1, RValue arg2, RValue arg3, RValue arg4, RValue arg5)
#define __MAKEPARAMS5(_script, arg1, arg2, arg3, arg4, arg5) \
        = { &arg1, &arg2, &arg3, &arg4, &arg5 };
#define __MAKEARGS6(_script, arg1, arg2, arg3, arg4, arg5, arg6) \
        _script(RValue arg1, RValue arg2, RValue arg3, RValue arg4, RValue arg5, RValue arg6)
#define __MAKEPARAMS6(_script, arg1, arg2, arg3, arg4, arg5, arg6) \
        = { &arg1, &arg2, &arg3, &arg4, &arg5, &arg6 };
#define __MAKEARGS7(_script, arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
        _script(RValue arg1, RValue arg2, RValue arg3, RValue arg4, RValue arg5, RValue arg6, RValue arg7)
#define __MAKEPARAMS7(_script, arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
        = { &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7 };
#define __MAKEARGS8(_script, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
        _script(RValue arg1, RValue arg2, RValue arg3, RValue arg4, RValue arg5, RValue arg6, RValue arg7, RValue arg8)
#define __MAKEPARAMS8(_script, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
        = { &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8 };
#define __MAKEARGS9(_script, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) \
        _script(RValue arg1, RValue arg2, RValue arg3, RValue arg4, RValue arg5, RValue arg6, RValue arg7, RValue arg8, RValue arg9)
#define __MAKEPARAMS9(_script, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) \
        = { &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8, &arg9 };
#define __MAKEARGS10(_script, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10) \
        _script(RValue arg1, RValue arg2, RValue arg3, RValue arg4, RValue arg5, RValue arg6, RValue arg7, RValue arg8, RValue arg9, RValue arg10)
#define __MAKEPARAMS10(_script, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10) \
        = { &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8, &arg9, &arg10 };
#define __MAKEARGS11(_script, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11) \
        _script(RValue arg1, RValue arg2, RValue arg3, RValue arg4, RValue arg5, RValue arg6, RValue arg7, RValue arg8, RValue arg9, RValue arg10, RValue arg11)
#define __MAKEPARAMS11(_script, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11) \
        = { &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8, &arg9, &arg10, &arg11 };
#define __MAKEARGS12(_script, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12) \
        _script(RValue arg1, RValue arg2, RValue arg3, RValue arg4, RValue arg5, RValue arg6, RValue arg7, RValue arg8, RValue arg9, RValue arg10, RValue arg11, RValue arg12)
#define __MAKEPARAMS12(_script, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12) \
        = { &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8, &arg9, &arg10, &arg11, &arg12 };
#define __MAKEARGS13(_script, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13) \
        _script(RValue arg1, RValue arg2, RValue arg3, RValue arg4, RValue arg5, RValue arg6, RValue arg7, RValue arg8, RValue arg9, RValue arg10, RValue arg11, RValue arg12, RValue arg13)
#define __MAKEPARAMS13(_script, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13) \
        = { &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8, &arg9, &arg10, &arg11, &arg12, &arg13 };
#define __MAKEARGS14(_script, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14) \
        _script(RValue arg1, RValue arg2, RValue arg3, RValue arg4, RValue arg5, RValue arg6, RValue arg7, RValue arg8, RValue arg9, RValue arg10, RValue arg11, RValue arg12, RValue arg13, RValue arg14)
#define __MAKEPARAMS14(_script, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14) \
        = { &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8, &arg9, &arg10, &arg11, &arg12, &arg13, &arg14 };
#define __MAKEARGS15(_script, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15) \
        _script(RValue arg1, RValue arg2, RValue arg3, RValue arg4, RValue arg5, RValue arg6, RValue arg7, RValue arg8, RValue arg9, RValue arg10, RValue arg11, RValue arg12, RValue arg13, RValue arg14, RValue arg15)
#define __MAKEPARAMS15(_script, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15) \
        = { &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8, &arg9, &arg10, &arg11, &arg12, &arg13, &arg14, &arg15 };
#define __MAKEARGS16(_script, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16) \
        _script(RValue arg1, RValue arg2, RValue arg3, RValue arg4, RValue arg5, RValue arg6, RValue arg7, RValue arg8, RValue arg9, RValue arg10, RValue arg11, RValue arg12, RValue arg13, RValue arg14, RValue arg15, RValue arg16)
#define __MAKEPARAMS16(_script, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16) \
        = { &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8, &arg9, &arg10, &arg11, &arg12, &arg13, &arg14, &arg15, &arg16 };

#define _DECLARE_SCRIPT1(_id, _script, ...) \
static inline PFUNC_YYGMLScript _script ## innerFunc ## _id = nullptr; \
static inline auto EXPAND(GET_SCRIPTMACRO(_script __VA_OPT__(,) __VA_ARGS__,  \
	__MAKEARGS16,__MAKEARGS15,__MAKEARGS14,__MAKEARGS13,__MAKEARGS12,__MAKEARGS11, \
	__MAKEARGS10,__MAKEARGS9,__MAKEARGS8,__MAKEARGS7,__MAKEARGS6,__MAKEARGS5, \
	__MAKEARGS4,__MAKEARGS3 ,__MAKEARGS2,__MAKEARGS1,__MAKEARGS0)(_script __VA_OPT__(,)  __VA_ARGS__) ) -> RValue { \
		if ( _script ## innerFunc ## _id != nullptr ) \
		{ \
			std::vector<RValue*> args EXPAND(GET_SCRIPTMACRO(_script __VA_OPT__(,) __VA_ARGS__, \
				__MAKEPARAMS16,__MAKEPARAMS15,__MAKEPARAMS14,__MAKEPARAMS13,__MAKEPARAMS12,__MAKEPARAMS11, \
				__MAKEPARAMS10,__MAKEPARAMS9,__MAKEPARAMS8,__MAKEPARAMS7,__MAKEPARAMS6,__MAKEPARAMS5, \
				__MAKEPARAMS4,__MAKEPARAMS3 ,__MAKEPARAMS2,__MAKEPARAMS1,__MAKEPARAMS0)(_script __VA_OPT__(,) __VA_ARGS__) )	\
			RValue outResultPtr; \
			auto result = _script ## innerFunc ## _id (GetGlobalInstance(), GetGlobalInstance(), &outResultPtr, args.size(), args.data()); \
			return RValue(std::move(*result)); \
		} \
		GetLogger()->LogFormatted("%s - script called before init finished", #_script); \
		return RValue(false); \
	} \
static inline auto __ ## _script ## DefinitionInit ## _id = []() -> auto { \
	auto ret = std::thread([]() -> void { \
		while (!UIManager::isImGuiInitialized()) \
			std::this_thread::sleep_for(std::chrono::milliseconds(25)); \
		GetLogger()->LogFormatted("ScriptDef:" #_script " - %s\n", "Init"); \
		int scriptID = Script_Find_Id(#_script) - 100000; \
		GetLogger()->LogFormatted("ScriptDef:Add - Script ID %d\n", scriptID); \
		CScript* theScript = ScriptFromId(scriptID); \
		if (!theScript)  \
			GetLogger()->LogFormatted("ScriptHooks::Add: Invalid script or index. Found ID %d", scriptID); \
		_script ## innerFunc ## _id = theScript->m_Functions->m_ScriptFunction; \
	}); \
    ret.detach(); \
    return true; \
}();


#define _DECLARE_SCRIPT0(_id, _script, ...) _DECLARE_SCRIPT1(_id, _script, __VA_ARGS__)

#define _DECLARE_SCRIPT(_script, ...) \
	_DECLARE_SCRIPT0(__LINE__, _script, __VA_ARGS__)

#define _DECLARE_VSCRIPT(_script) \
	_DECLARE_SCRIPT0(__LINE__, _script)

