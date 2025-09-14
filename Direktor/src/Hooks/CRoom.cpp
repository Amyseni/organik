#include "Synthetik.h"
#include "zhl_internal.h"
#include "zhl.h"
#include <unordered_map>



static bool g_fake =  ([]() -> bool {
    return true;
})();
CRoom *m_RunRoom = nullptr;
// maybe it's best if you just don't look too hard at this, it may hurt your brain holes. I know it hurts mine.

static std::unordered_map<int32_t, CInstance*> g_ActiveInstances = {};
std::mutex g_ActiveInstancesMutex;
_NODISCARD_LOCK std::pair<std::unique_lock<std::mutex>, std::unordered_map<int32_t, CInstance*>> GetActiveInstances()
{
    std::unique_lock<std::mutex> lock(g_ActiveInstancesMutex);
    std::unordered_map<int32_t, CInstance*> instances = {};
    CInstance *i = nullptr;
    CRoom *room = nullptr;
    //Organik::Utils::GetCurrentRoom(room);
    if (m_RunRoom)
    {
        for(i = m_RunRoom->m_Active.m_First; i; i = i->m_Next)
        {
            if (!i)
                continue;
            instances.insert({i->m_ID, i});
        }
    }
    return std::make_pair(std::move(lock), instances);
}
HOOK_METHOD(CRoom, UpdateActive, (void) -> void)
{
    if (!this)
        Error_Show_Action(
            "CRoom::UpdateActive called with null 'this' pointer. (??? how)",  
            true /* requireCrash */, true /* manualError */
        );
    
    this->super();
    m_RunRoom = this;
    if (!Organik::Utils::isInitializationDone())
        return;
//     Organik::GetLogger()->LogFormatted("Updated active instances in the room. Total count: %d", m_Active.m_Count);
}
