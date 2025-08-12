#include "Synthetik.h"
#include "zhl_internal.h"
#include "zhl.h"
#include <unordered_map>
#include "Logging.h"
#include "Utils.h"

static bool g_fake =  ([]() -> bool {
    return true;
})();

// maybe it's best if you just don't look too hard at this, it may hurt your brain holes. I know it hurts mine.

static std::unordered_map<int32_t, CInstance*> g_ActiveInstances = {};
std::mutex g_ActiveInstancesMutex;
_NODISCARD_LOCK std::pair<std::unique_lock<std::mutex>, std::unordered_map<int32_t, CInstance*>> GetActiveInstances()
{
    std::unique_lock<std::mutex> lock(g_ActiveInstancesMutex);
    return std::make_pair(std::move(lock), g_ActiveInstances);
}
HOOK_METHOD(CRoom, UpdateActive, (void) -> void)
{
    if (!this)
        Error_Show_Action(
            const_cast<char*>("CRoom::UpdateActive called with null 'this' pointer. (??? how)"), 
            true /* requireCrash */, true /* manualError */
        );
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
