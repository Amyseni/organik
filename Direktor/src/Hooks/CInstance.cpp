#include "Synthetik.h"
#include "zhl_internal.h"
#include "zhl.h"
#include "Utils.h"
#include <unordered_map>


// HOOK_METHOD(CInstance, Activate, (void) -> void)
// {
//     std::cout << "Activating instance" << std::endl;
//     super();
//     if (GetActiveInstances().contains(m_ID))
//     {
//         std::cerr << "Instance with ID " << m_ID << " is already active. (???)" << std::endl;
//     }
//     else
//     {
//         GetActiveInstances()[m_ID] = this;
//         std::cout << "Instance with ID " << m_ID << " activated." << std::endl;
//     }
//     std::cout << "Activate didn't crash, yay!" << std::endl;
//     return;
// }

RValue* CInstance::operator[](const char* name) {
    return InternalReadYYVar(Code_Variable_FindAlloc_Slot_From_Name(GetGlobalInstance(), const_cast<char*>(name)));
}
RValue* CInstance::operator[](int32_t index) {
    return InternalReadYYVar(index);
}
std::vector<CInstance*> CInstance::Where(std::function<bool(CInstance*)> func)
{
	std::vector<CInstance*> ret;
    auto [lock, instances] = GetActiveInstances();
    // lock is automatically released when it goes out of scope
    for (auto [id,c] : instances)
    {
		if (func(c))
    		ret.push_back(c);
    }
	return ret;
}
CInstance* CInstance::FirstOrDefault(std::function<bool(CInstance*)> func)
{
	std::vector<CInstance*> ret;
    auto [lock, instances] = GetActiveInstances();
     
    for (auto [id,c] : instances)
    {
		if (func(c))
    		return c;
    }
	return nullptr;
}
bool CInstance::Any(std::function<bool(CInstance*)> func)
{
    auto [lock, instances] = GetActiveInstances();

    for (auto [id,c] : instances)
    {
		if (func(c))
    		return true;
    }
	return false;
}
void CInstance::ForEach(std::function<void(CInstance*)> func)
{
    auto [lock, instances] = GetActiveInstances();

    for (auto [id,c] : instances)
    	func(c);
}
// HOOK_GLOBAL(AddGlobalObject, (YYObjectBase *toAdd) -> void)
// {
//     std::cout << "Adding global object." << std::endl;
//     super(toAdd);
//     if (!toAdd)
//     {
//         std::cerr << "Attempted to add a null global object." << std::endl;
//     }
//     else if ((int32_t)toAdd < 0)
//     {
//         std::cerr << "AddGlobalObject returned a likely constant. Value " << toAdd;
//     }
//     std::cout << "Global object added successfully." << std::endl;
// }
// HOOK_METHOD(CInstance, Deactivate, (void) -> void)
// {
//     std::cout << "Deactivating instance with ID: " << m_ID << std::endl;
//     super();
//     auto it = GetActiveInstances().find(m_ID);
//     std::string idStr = std::to_string(m_ID);
//     if (it != GetActiveInstances().end())
//     {
//         GetActiveInstances().erase(it);
//         std::cout << "Instance with ID " << idStr << " deactivated." << std::endl;
//     }
//     else
//     {
//         std::cerr << "Instance with ID " << idStr << " was not active. (???)" << std::endl;
//     }
//     std::cout << "Deactivate didn't crash, yay!" << std::endl;
//     return;
// }