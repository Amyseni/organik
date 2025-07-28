#pragma once

#ifndef MAKE_STRINGS
#define MAKE_STRINGS(THINGNAME) #THINGNAME,
#endif
#ifndef MAKE_ENUM
#define MAKE_ENUM(THINGNAME) THINGNAME,
#endif
#ifndef VARIABLES_DO
#include "var_list.h"
#endif
#ifndef ORGANIK_VARIABLES
#define ORGANIK_VARIABLES
namespace Organik
{
    struct Variables
    {
        enum : uint32_t
        {
            VARIABLES_DO(MAKE_ENUM)
        };
        static std::unordered_map<const char*, int> varToEnumMap;
        static std::vector<int32_t> Hashes;
        static const char* const VariableNamesArr[];
        static std::unordered_map<int32_t, const char*> HashToVariableMap;
        static int32_t GetHash(const char* name);
    };
}
#endif
