#include "Synthetik.h"
#include "zhl_internal.h"
#include "zhl.h"
#include <unordered_map>



HOOK_METHOD(CInstance, Activate, (void) -> void)
{
    if (GetActiveInstances().contains(m_ID))
    {
        std::cerr << "Instance with ID " << m_ID << " is already active. (???)" << std::endl;
    }
    else
    {
        GetActiveInstances()[m_ID] = this;
        std::cout << "Instance with ID " << m_ID << " activated." << std::endl;
    }
}

HOOK_GLOBAL(AddGlobalObject, (YYObjectBase *toAdd) -> void)
{

}
HOOK_METHOD(CInstance, Deactivate, (void) -> void)
{
    auto it = GetActiveInstances().find(m_ID);
    std::string idStr = std::to_string(m_ID);
    if (it != GetActiveInstances().end())
    {
        GetActiveInstances().erase(it);
        std::cout << "Instance with ID " << idStr << " deactivated." << std::endl;
    }
    else
    {
        std::cerr << "Instance with ID " << idStr << " was not active. (???)" << std::endl;
    }
    super();
    return;
}