#include <stdlib.h>
#include <vector>
#include "Synthetik.h"
#include "zhl_internal.h"
#include "zhl.h"
#include "Utils.h"
#include "Logging.h"

using namespace std;
namespace Organik
{
    extern bool test1234;
}

typedef void (*PFN_Builtin)(RValue *Result,CInstance* Self,CInstance* Other,int ArgumentCount,RValue *Arguments);

#pragma optimize("", off)
#ifndef DO_BUILTIN_H
#define DO_BUILTIN_H
template <typename T>
requires (std::is_pointer_v<T>)
RValue DoBuiltin(T fn, std::vector<RValue> args)
{
    RValue result;
    PFN_Builtin func = reinterpret_cast<PFN_Builtin>((PVOID)fn);
    if (!func || !GetGlobalInstance())
    {
        std::string errorTxt = Organik::GetLogger()->ParseFormatting("DoBuiltin (@ %p), GetGlobalInstance() or func is nullptr.\nGetGlobalInstance(): %p\nOriginal args: %p\nargs.data(): %p", 
                                                                            (void*)fn,                                              (void*)GetGlobalInstance(),  (void*)&args,   (void*)args.data());
        
        Error_Show_Action(const_cast<char*>(errorTxt.c_str()), true, true);
    } else {
        Organik::GetLogger()->LogFormatted("Result pointer: %p", &result);
        func(&result, GetGlobalInstance(), GetGlobalInstance(), static_cast<int>(args.size()), args.data());
        Organik::GetLogger()->LogFormatted("DoBuiltin: %lld", result.m_i64);
    }
	Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
    
    return std::move(result);
}
#endif // DO_BUILTIN_H
#ifndef DO_BUILTIN_REF_H
#define DO_BUILTIN_REF_H
template <typename T>
requires (std::is_pointer_v<T>)
static void DoBuiltinRef(T fn, RValue *out, std::vector<RValue> args)
{
    PFN_Builtin func = reinterpret_cast<PFN_Builtin>((PVOID)fn);
    if (!func || !GetGlobalInstance())
    {
        std::string errorTxt = Organik::GetLogger()->ParseFormatting("DoBuiltin (@ %p), GetGlobalInstance() or func is nullptr.\nGetGlobalInstance(): %p\nOriginal args: %p\nargs.data(): %p", 
                                                                            (void*)fn,                                              (void*)GetGlobalInstance(),  (void*)&args,   (void*)args.data());
        
        Error_Show_Action(const_cast<char*>(errorTxt.c_str()), true, true);
    } else {
        
        Organik::GetLogger()->LogFormatted("DoBuiltin (@ %p)", (void*)fn);
        Organik::GetLogger()->LogFormatted("Result pointer: %p", &out);
        func(out, GetGlobalInstance(), GetGlobalInstance(), static_cast<int>(args.size()), args.data());
        Organik::GetLogger()->LogFormatted("DoBuiltin: %lld", out->m_i64);

    }
	Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
    
    return;
}
#endif // DO_BUILTIN_REF_H
#pragma optimize("", on)