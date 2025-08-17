#pragma once
#include <vector>
#include <unordered_map>
#ifndef MAKE_STRINGS
#define MAKE_STRINGS(THINGNAME) #THINGNAME,
#endif
#ifndef MAKE_ENUM
#define MAKE_ENUM(THINGNAME) THINGNAME,
#endif
#ifndef SCRIPTS_DO
#include "script_list.h"
#endif

#ifndef ORGANIK_SCRIPTS
#define ORGANIK_SCRIPTS
namespace Organik
{
    struct Scripts
    {
        enum
        {
            SCRIPTS_DO(MAKE_ENUM)
        };
        static std::vector<int32_t> Indexes;
        static const char* const ScriptNamesArr[];
        static std::unordered_map<int32_t, CScript*> scriptPointerMap; 
    };
}
#endif
