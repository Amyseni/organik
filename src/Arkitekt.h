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

inline static std::shared_mutex& arkLogMutex()
{
	static std::shared_mutex m;
	return m;
}
inline static auto _arkInit = []() -> bool
{
	{
		std::unique_lock<std::shared_mutex> lock(arkLogMutex());
		WIN32_FIND_DATAA fd;
		HANDLE hFileInf = FindFirstFileA("./ARK.log", &fd);
		if ((hFileInf) && hFileInf != HANDLE(-1))
			DeleteFileA("./ARK.log");
		auto file = fopen("./ARK.log", "w");
		if (file)
		{
			fprintf(file, "ARK Log Initialized\n");
			fclose(file);
		}
	}
	return true;
}();
#define Log(fmt, ...) \
{ \
	std::unique_lock<std::shared_mutex> lock(arkLogMutex()); \
	auto file = fopen("./ARK.log", "a"); \
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

typedef void* PVOID;

__declspec(dllexport) void __cdecl LOCK_RVALUE_MUTEX();
__declspec(dllexport) void __cdecl UNLOCK_RVALUE_MUTEX();

namespace Arkitekt { class Definition; }

using namespace Arkitekt;
extern std::unordered_map<size_t, Definition*>& DefsByHash();

namespace Arkitekt
{

	class Definition
	{
	public:
		static int Init();
		static const char* GetLastError();

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
		std::string_view _name;
		const char* _sig;
		void** _outFunc;
		void* _address;

	public:
		template <typename T>
		FunctionDefinition(const char* name, const std::type_identity<T>& type, const char* sig, void** outfunc) :
			_sig(sig),
			_outFunc(outfunc)
		{
			_name =  std::string_view((std::string(typeid(T).name()) + std::string(name)).c_str()).data();
			Add(name, typeid(T).hash_code(), this);
		}

		template <typename T>
		FunctionDefinition(const char* name, const std::type_identity<T>&, void* addr, void** outfunc) :
			_address(addr),
			_outFunc(outfunc)
		{
			_name = std::string_view((std::string(typeid(T).name()) + std::string(name)).c_str()).data();
			Add(name, typeid(T).hash_code(), this);
		}


		virtual int Load();
		void* GetAddress() { return _address; }
	};

	//=================================================================================================

	class VariableDefinition : public Definition
	{
	private:
		void* _outVar;
		const char* _name;
		const char* _sig;
		const bool _useValue;
		const bool _useOffset;

	public:
		VariableDefinition(const char* name, const char* sig, void* outvar, bool useValue = true, bool useOffset = false) :
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
		const char* _name;
		const char* _sig;

	public:
		NoOpDefinition(const char* name, const char* sig) :
			_name(name),
			_sig(sig)
		{
			Add(_name, typeid(void).hash_code(), this);
		}

		virtual int Load();
	};

}
using namespace Arkitekt;

CInstance* GetGlobalInstance();
//=================================================================================================

namespace Arkitekt
{
	LIBARK_API bool Init();
}

void Error_Show_Action_Alt(bool mustCrash, bool manualError, const char* fmt, ...);


#include <Arkitekt/Assembler.win32.hpp>

#define __cdecl_cleanup true
#define __stdcall_cleanup false

struct FunctionHook;
extern std::vector<FunctionHook*>& GetInstalledHooks();
extern std::shared_mutex& GetInstallMapMutex();
extern std::multimap<int, std::function<auto ()->FunctionHook*>*, std::greater<int>>& GetHookMap();
extern std::multimap<int, std::function<auto ()->FunctionHook*>*, std::greater<int>>& GetDelayedInstallMap();
extern std::thread* GetInstallerThread();

struct FunctionHook
{
	std::string_view name;
	MologieDetours::Detour<void*>* detour = nullptr;
	FnBlock* hookBlock = nullptr;
	FnBlock* superBlock = nullptr;

	FunctionHook(const std::string_view& _name, MologieDetours::Detour<void*>* _detour, FnBlock* _hookBlock, FnBlock* _superBlock)
		: name(_name.data()), detour(_detour), hookBlock(_hookBlock), superBlock(_superBlock) {}

	template <typename T, bool NeedCallerCleanup>
	static FunctionHook* Install(void** outInternalSuper, void* fn, void* hook)
	{
		std::string_view _name = typeid(T).name();
		auto hookWrapper = CreateWrapper<T, NeedCallerCleanup>((std::string(_name.data()) + "Hook").c_str(), hook);

		MologieDetours::Detour<void*>* _detour = new MologieDetours::Detour<void*>(fn, hookWrapper->m_Fn);

		auto superWrapper = CreateWrapper<T, NeedCallerCleanup>((std::string(_name.data()) + "Super").c_str(), _detour->GetOriginalFunction());

		if (outInternalSuper)
			*outInternalSuper = superWrapper->m_Fn;
		else
			throw new std::bad_function_call;

		return new FunctionHook(typeid(T).name(), _detour, hookWrapper, superWrapper);
	}

	template <typename T, bool NeedCallerCleanup = false>
		requires (!(std::same_as<void*, T>))
	static FunctionHook* Install(void** outInternalSuper, void* fn, T hook)
	{
		std::string_view _name = typeid(T).name();

		auto hookWrapper = CreateWrapper<T, NeedCallerCleanup>(
			(std::string(_name.data()) + "Hook").c_str(),
			*(void**)(&hook)
		);

		MologieDetours::Detour<void*>* _detour = new MologieDetours::Detour<void*>(*(void**)(&fn), hookWrapper->m_Fn);

		auto superWrapper = CreateWrapper<T, NeedCallerCleanup>(
			(std::string(_name.data()) + "Super").c_str(),
			_detour->GetOriginalFunction()
		);

		if (outInternalSuper)
			*outInternalSuper = superWrapper->m_Fn;
		else
			throw new std::bad_function_call;

		return new FunctionHook(typeid(T).name(), _detour, hookWrapper, superWrapper);
	}

	~FunctionHook()
	{
		if (detour)
			delete detour;
		if (hookBlock)
			delete hookBlock;
		if (superBlock)
			delete superBlock;
	}
};

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
			std::unique_lock<std::shared_mutex> lock(GetInstallMapMutex()); \
			GetHookMap().emplace(_priority, new std::function<auto () -> FunctionHook*>([=]() -> FunctionHook* { \
				FunctionDefinition *def = dynamic_cast<FunctionDefinition*>( \
					Definition::Find<decltype(&_classname:: _name) >( \
						#_classname"::" #_name \
					) \
				); \
				if (!def) {  \
					MessageBoxA(NULL, #_classname "::" #_name, "Fuck", MB_OK); \
				} \
				Log("%s %p", #_name, def->GetAddress()); \
				return FunctionHook::Install( \
					&internalSuper, def->GetAddress(),  &wrapper::hook \
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
		GetHookMap().emplace(_priority, new std::function<auto () -> FunctionHook*>([=]() -> FunctionHook* { \
			FunctionDefinition *def = dynamic_cast<FunctionDefinition*>( \
				Definition::Find<decltype(&_classname:: _name)>( \
					#_classname"::"#_name, \
				)); \
			Log("%s %p", #_name, def->GetAddress()); \
			return FunctionHook::Install<decltype(&_classname ::_name), EXPAND_THEN_CONCATENATE(_conv, _cleanup)>( \
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
			GetHookMap().emplace(_priority, new std::function<auto () -> FunctionHook*>([=]() -> FunctionHook* { \
				FunctionDefinition *def = dynamic_cast<FunctionDefinition*>( \
					Definition::Find<decltype(&_name)>( \
						#_name \
					)); \
				Log("%s %p", #_name, def->GetAddress()); \
				return FunctionHook::Install<decltype(&_name) , EXPAND_THEN_CONCATENATE(_conv, _cleanup)>( \
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
	namespace { namespace _obj ## Hook ## _event ## _subevent ## _priority ## _id { \
		static void* internalSuper = NULL; \
        static auto __cdecl hook (CInstance*, CInstance*) -> void; \
        static auto __cdecl super (CInstance*, CInstance*) -> void; \
		FunctionDefinition* def; \
		auto installer = [&](){ \
			std::shared_lock<std::shared_mutex> lock(GetInstallMapMutex()); \
			GetDelayedInstallMap().emplace(_priority, new std::function<auto () -> FunctionHook*>([=]() -> FunctionHook* { \
				RValue rv = DoBuiltin(&gml_asset_get_index, #_obj ); \
				int32_t m_ObjIndex = parseRValueNumber<int32_t>(rv); \
				if (m_ObjIndex < 1)  \
					Error_Show_Action("EventHook::Add: Invalid object index", true, true); \
				auto objectData = Object_Data(m_ObjIndex); \
				if (!objectData)  \
					Error_Show_Action("EventHook::Add: Invalid object data", true, true); \
				auto cEvent = objectData->GetEventRecursive(_event, _subevent); \
				if (!cEvent || !cEvent->m_Code->m_Functions->m_CodeFunction) \
					Error_Show_Action("EventHook::Add: Invalid event handler", true, true); \
				Log("%s %p", #_obj "Hook" #_event #_subevent #_priority #_id, cEvent->m_Code->m_Functions->m_CodeFunction); \
				return FunctionHook::Install<void(__cdecl*)(CInstance*, CInstance*), true>( \
					&internalSuper, cEvent->m_Code->m_Functions->m_CodeFunction, &hook \
				); \
			})); \
			return true; \
		}(); \
		\
	} } \
	auto FUNC_NAKED __cdecl _obj ## Hook ## _event ## _subevent ## _priority ## _id::super(CInstance* self, CInstance* other) -> void { __asm {JUMP_INSTRUCTION(_obj ## Hook ## _event ## _subevent ## _priority ## _id::internalSuper)}; } \
	auto __cdecl _obj ## Hook ## _event ## _subevent ## _priority ## _id::hook(CInstance* self, CInstance* other) -> void

#define _DEFINE_EVENT_HOOK0(_id, _object, _event, _subevent, _priority) _DEFINE_EVENT_HOOK1(_id, _object, _event, _subevent, _priority)

#define HOOK_EVENT(_object, _event, _subevent) _DEFINE_EVENT_HOOK0(__LINE__, _object, _event, _subevent, 0)


#define _DEFINE_SCRIPT_HOOK1(_id, _script, _priority) \
namespace { namespace Hook ## _script ## _priority ## _id { \
	static void* internalSuper = NULL; \
	static auto __cdecl hook (CInstance* Self, CInstance* Other, RValue* Result, int ArgumentCount, RValue* Arguments[]) -> RValue *; \
	static auto __cdecl super (CInstance* Self, CInstance* Other, RValue* Result, int ArgumentCount, RValue* Arguments[]) -> RValue *; \
	CScript* theScript = nullptr; \
	void *func = nullptr; \
	FunctionDefinition* def; \
	auto installer = [&](){ \
		std::shared_lock<std::shared_mutex> lock(GetInstallMapMutex()); \
		GetDelayedInstallMap().emplace(_priority, new std::function<auto () -> FunctionHook*>([=]() -> FunctionHook* { \
			int scriptID = Script_Find_Id(#_script) - 100000; \
			CScript* theScript = ScriptFromId(scriptID); \
			if (!theScript)  \
			{ \
				Log("ScriptHooks::Add: Invalid script or index. Found ID %d", scriptID); \
				exit(1); \
			} \
			if (!theScript->m_Functions->m_ScriptFunction) \
			{ \
				Log("EventHook::Add: Invalid script function"); \
				exit(1); \
			} \
			Log("%s %p", #_script, theScript->m_Functions->m_ScriptFunction); \
			return FunctionHook::Install<PFUNC_YYGMLScript, true>( \
				&internalSuper, theScript->m_Functions->m_ScriptFunction, &hook \
			); \
		})); \
		return true; \
	}(); \
	\
} } \
auto FUNC_NAKED __cdecl Hook ## _script ## _priority ## _id::super (CInstance* Self, CInstance* Other, RValue* Result, int ArgumentCount, RValue* Arguments[]) -> RValue * {__asm {JUMP_INSTRUCTION(Hook ## _script ## _priority ## _id::internalSuper)}; } \
auto __cdecl Hook ## _script ## _priority ## _id::hook (CInstance* Self, CInstance* Other, RValue* Result, int ArgumentCount, RValue* Arguments[]) -> RValue *

#define _DEFINE_SCRIPT_HOOK0(_id, _script, _priority) _DEFINE_SCRIPT_HOOK1(_id, _script, _priority)

#define HOOK_SCRIPT(_script) _DEFINE_SCRIPT_HOOK0(__LINE__, _script, 0)



