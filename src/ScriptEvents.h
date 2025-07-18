#pragma once

#include "ScriptHelper.h"
#include "InstanceHelper.h"
#include "VariableHepler.h"


namespace Organik
{
    struct ScriptHook;    
}

namespace Organik
{
    struct ScriptHook 
    {
        typedef RValue& (*doScriptSimple)(std::vector<RValue*>& args, CInstance* self = nullptr);
        typedef RValue &(*ScriptFunction)(CInstance *self, CInstance *other, RValue& result, int argc, RValue *arguments[]);

        int32_t ScriptIndex;
        doScriptSimple super;

        [super]ScriptFunction(*doCall) = nullptr;
        

    private:
        ScriptFunction internalSuper;
        ScriptFunction innerFunction;
        std::function<RValue &(CInstance *)>
        const std::string& _name;
    }
    static auto ScriptHooks = std::unordered_map<std::pair<
    static void InitScriptHooks()
    {
        
    }
}