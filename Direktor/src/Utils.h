#pragma once
#include "Synthetik.h"
#include <GameMaker_Defs.h>
#include "Logging.h"

struct BuiltInFunction
{
    union {
        TYYBuiltin routine;
        TRoutine fnPtr;
        union {
            void* pVoid;
            CInstance* pInstance;
            CScript* pScript;
            CEvent* pEvent;
            CCode* pCode;
            RValue* pRValue;
        };
    };
    operator TYYBuiltin() const {
        return routine;
    }
    operator TRoutine() const {
        return fnPtr;
    }
    operator CInstance*() const {
        return pInstance;
    }
    operator CScript*() const {
        return pScript;
    }
    operator CEvent*() const {
        return pEvent;
    }
    operator CCode*() const {
        return pCode;
    }
    operator RValue*() const {
        return pRValue;
    }
    operator PVOID() const {
        return pVoid;
    }
    RValue* operator()(std::vector<RValue> args)
    {
        RValue ret = -4;
        if (!routine)
            return &ret;
        routine(&ret, g_pGetRValueContainer, g_otherInstance, static_cast<int>(args.size()), args.data());
        return &ret;
    }
    static RValue* operator()(PVOID fn, std::vector<RValue> args)
    {
        BuiltInFunction func = { reinterpret_cast<TYYBuiltin>(fn) };
        func.routine = reinterpret_cast<BuiltInFunction&>(fn);
        if (!func.routine)
        {
            MessageBoxA(
                nullptr,
                "BuiltInFunction::routine null after cast!",
                "Error",
                MB_OK | MB_ICONERROR
            );
            return nullptr;
        }
        RValue result = RValue(-4);
        func.routine(&result, g_pGlobalInstance, g_pGlobalInstance, static_cast<int>(args.size()), args.data());
        return &result;
    }
    BuiltInFunction(std::invocable<RValue*, CInstance*, CInstance*, int, RValue*> auto&& func)
    {
        routine = reinterpret_cast<TYYBuiltin>(func);
    }
};
namespace Organik
{
    namespace Utils
    {
        
    }
}