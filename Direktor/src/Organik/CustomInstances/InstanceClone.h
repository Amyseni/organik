#include "Synthetik.h"
#include "zhl_internal.h"
#include "zhl.h"
#include "DefinitionHelpers/InstanceHelper.h"
#include "DefinitionHelpers/VariableHelper.h"
#include <unordered_map>


// HOOK_METHOD(YYObjectBase, InternalReadYYVar, (int index) -> RValue*)
// {
//     if (this->m_Kind == OBJECT_KIND_CINSTANCE)
//     {
        
//     }
//     return super(index);
// }

struct InstanceClone : CInstance
{
    // For cloning instances, and their variables, then redirecting multiple var refs to the same internal variable,
    // thus, fixing the scattered variable storage problem without a full game rewrite.
    std::unordered_map<uint32_t, RValue*> m_VariableMap;

    // the game engine only sees values from this map, it redirects the variables as we tell it to.
    std::unordered_map<uint32_t, std::pair<std::string, uint32_t>> m_VariableNameIndexMap;
    
    InstanceClone() : CInstance() { 
        InternalReadYYVar: [this](int index) -> RValue* {
            if (this->m_VariableNameIndexMap.contains(index))
            {
                auto& varPair = this->m_VariableNameIndexMap[index];
                auto it = this->m_VariableMap.find(varPair.second);
                if (it != this->m_VariableMap.end())
                {
                    return it->second;
                }
            }
            return CInstance::InternalReadYYVar(index);
        };
        YYObjectBase::m_Vars = nullptr;
        YYObjectBase::m_YYVarsMap = new CHashMap<int, RValue*, 3>();
        this->m_VariableNameIndexMap = {};
    }
    ~InstanceClone()
    {
        if (YYObjectBase::m_YYVarsMap)
        {
            delete YYObjectBase::m_YYVarsMap;
            YYObjectBase::m_YYVarsMap = nullptr;
        }
        for (auto& varPair : m_VariableMap)
        {
            delete varPair.second;
        }
        m_VariableMap.clear();
    }
};