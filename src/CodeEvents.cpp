#include "CodeEvents.h"
#include <YYToolkit/YYTK_Shared.hpp>
#include <Windows.h> // For VirtualQuery
#include "CallbackManager/CallbackManagerInterface.h"
#include "ModuleMain.h"
#include <chrono>
#include <iomanip>
#include <ctime>
#include "Logging.h"

using namespace Organik;
// Each CodeEvent is a game engine call to ExecuteIt on a CInstance *self object (equivalent to someObject->someFunction)
// ALL Code event callbacks should match the following function signature
/* 
    void MyHookFunction(
        std::tuple<
            CInstance *self, // effectively *this
            CInstance *other, // may be nullptr
            CCode *code, // the code object that is being executed
            int argc, // number of arguments passed to the function
            RValue *argv // array of arguments passed to the function  
        >& Args
    )
    {
        // Note that this function cannot return a value, as it has athe return type of void.
        // However, since we have access to self, other, and all args, we can modify the global state of the game engine.
        // Additionally, we can modify argv array being passed to the function, or
        // if necessary, detour the CCode function further (though this can have unwanted side effects)
    }
*/

/// @brief called before the internal outgame constructor is called.
/// creates something referred to as gml_Object_outgame, which seems to be a global state machine of some kind
/// @param Args is a tuple containing the following:
/// - CInstance *self: The reference that will become gml_Object_outgame
void OutgameCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    CInstance* self = std::get<0>(Args);
    if (GetLogger())
    {
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    }
    else
    {
        std::cerr << "[ERROR]" << "["<< __FILE__ << ":" << __LINE__ << "]" "Error: GetLogger() is null.";
    }
}

/// @brief called after the internal outgame constructor is called, which is after the OutgameCreateBefore callback.
/// @param Args is a tuple containing the following:
/// - CInstance *self: Reference to the newly created CInstance to our object.
void OutgameCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    CInstance* self = std::get<0>(Args);

    objOutgameIndex = GetInterface()->CallBuiltin("asset_get_index", { "obj_outgame" }).ToInt32();
    if (objOutgameIndex < 0)
    {
        GetLogger()->LogSimple("Error: obj_outgame index is negative, indicating it was not found.");
        return;
    }
    if (GetLogger())
    {
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    }
    else
    {
        std::cerr << "[ERROR]" << "["<< __FILE__ << ":" << __LINE__ << "]" "Error: GetLogger() is null.";
    }
}


void EnmBaseParentCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void EnmBaseParentCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void CursorCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void CursorCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void BuffTimedCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void BuffTimedCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void CursorCameraDrawBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void BuffStatboostsCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void UpgradeMenuCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void LastKillCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void LastKillDrawBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void LastKillDrawAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void MainUiS1PreCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void MainUiS1PreCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void MainUiWeaponPreCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void MainUiWeaponPreCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void RollClassItemCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void RollClassItemCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void SelectionMenuCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void SelectionMenuCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void SelectionMenuPerkCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void SelectionMenuPerkCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void AndroidConsoleCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void AndroidConsoleCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void SelectionMenuStatCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void SelectionMenuStatCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void EscapeSettingsCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void DialogUiCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void PauseMenuCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void ReloadingPreCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void ReloadingCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void ReloadingCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void ReloadingStep2Before(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void ReloadingStep2After(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void ItemStatsCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void ItemStatsCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void ItemStatsDrawBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void ItemStatsDrawAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void StatisticsCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void StatisticsCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void OverlaysAboveUiCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void OverlaysAboveUiCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void MainUiS1DrawAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void MainUiWeaponDrawAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void SelectionMenuDrawAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void EscapeSettingsDrawAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void AndroidConsoleDrawAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void DialogUiDrawAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void PauseMenuDrawAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void ReloadingDrawAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}

void OverlaysAboveUiDrawAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    if (GetLogger())
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args, false);
    else
        std::cerr << "[ERROR][" << __FILE__ << ":" << __LINE__ << "] Error: GetLogger() is null.";
}
