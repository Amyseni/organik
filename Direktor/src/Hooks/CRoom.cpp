#include "Synthetik.h"
#include "zhl_internal.h"
#include "zhl.h"
#include <unordered_map>
#include "Logging.h"
#include "Utils.h"



// HOOK_METHOD(CRoom, SwitchRoom, (int newRoom) -> void)
// {
//     std::cout << "Switching room to: " << newRoom << std::endl;
//     super(newRoom);
//     std::cout << "Room switched successfully." << std::endl;
// }

static bool g_fake =  ([]() -> bool {
    return true;
})();
static std::unordered_map<int32_t, CInstance*> g_ActiveInstances = std::unordered_map<int32_t, CInstance*>();
std::mutex g_ActiveInstancesMutex;
std::unordered_map<int32_t, CInstance*> GetActiveInstances()
{
    std::lock_guard<std::mutex> lock(g_ActiveInstancesMutex);
    return std::unordered_map<int32_t, CInstance*>(g_ActiveInstances);
}
HOOK_METHOD(CRoom, UpdateActive, (void) -> void)
{
    if (!this)
        Error_Show_Action(const_cast<char*>("CRoom::UpdateActive called with null 'this' pointer."), true, true);
    this->super();
    if (!Organik::Utils::isInitializationDone())
        return;
    Organik::GetLogger()->LogSimple("CRoom::UpdateActive called");
    
    if (!m_Active.m_First || !m_Active.m_Count)
    {
        Organik::GetLogger()->LogSimple("No active instances found in the room.");
        std::lock_guard<std::mutex> lock(g_ActiveInstancesMutex);
        g_ActiveInstances.clear();
        return;
    }
    std::lock_guard<std::mutex> lock(g_ActiveInstancesMutex);
    g_ActiveInstances.clear();
    g_ActiveInstances.reserve(m_Active.m_Count);
    for (CInstance* in = m_Active.m_First; in; in = in->m_Next)
    {
        if (!in)
            continue;
        g_ActiveInstances.insert({in->m_ID, in});
    }
    Organik::GetLogger()->LogFormatted("Updated active instances in the room. Total count: %d", m_Active.m_Count);
}

// I'm pretty sure this is for collision detection? Since it references the sprites.
// HOOK_GLOBAL(MarkInstancesAsDirty, (int spriteIndex) -> void)
// {
//     // This function marks all instances of a given sprite as dirty
//     for (auto& [id, instance] : GetActiveInstances())
//     {
//         if (instance->m_SpriteIndex == spriteIndex)
//         {
//             instance->m_InstanceFlags |= 0x1; // Assuming 0x1 is the dirty flag
//             std::cout << "Marked instance with ID " << id << " as dirty." << std::endl;
//         }
//     }
// }