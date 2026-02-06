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
// LIBZHL_API void __stdcall LOCK_RVALUE_MUTEX();
// LIBZHL_API void __stdcall UNLOCK_RVALUE_MUTEX();
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

	std::string_view type_name;
	std::string_view type_name_raw;
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
// void Error_Show_Action(bool mustCrash, bool manErr, const char* fmt, ...);


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
