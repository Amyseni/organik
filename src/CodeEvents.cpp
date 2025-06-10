#include "CodeEvents.h"
#include <YYToolkit/YYTK_Shared.hpp>
#include "CallbackManager/CallbackManagerInterface.h"
#include "ModuleMain.h"

extern YYTKInterface* g_ModuleInterface;
extern std::ofstream outFile;

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

/// @brief This function is called before most other game code, creating a sort of proxy global game state object.
/// @param Args is a tuple containing the following:
/// - CInstance *self: The reference that will become gml_Object_outgame
void OutgameCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    // log values in Args, log pointer to CCode object (member function of CInstance *self), check pointer to other and log if present.
    CInstance* self = std::get<0>(Args);
    CInstance* other = std::get<1>(Args);
    CCode* code = std::get<2>(Args);
    int argc = std::get<3>(Args);
    RValue* argv = std::get<4>(Args);

    outFile << "OutgameCreateBefore called." << std::endl;
    outFile << "Self instance: " << (void*) self  << std::endl;
    if (other)
    {
        outFile << "Other instance: " << (void*) other  << std::endl;
    }
    outFile << "Code object: " << (void*) code  << std::endl;
    outFile << "Argument count: " << argc  << std::endl;
    for (int i = 0; i < argc; ++i)
    {
        outFile << "Argument " << i << ": " << argv[i].ToString() << std::endl;
    }
}

/// @brief called after the internal outgame constructor is called, which is after the OutgameCreateBefore callback.
/// @param Args is a tuple containing the following:
/// - CInstance *self: Reference to the newly created CInstance to our object.
void OutgameCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
    CInstance* self = std::get<0>(Args);
    CInstance* other = std::get<1>(Args);
    CCode* code = std::get<2>(Args);
    int argc = std::get<3>(Args);
    RValue* argv = std::get<4>(Args);

    objOutgameIndex = g_ModuleInterface->CallBuiltin("asset_get_index", { "obj_outgame" }).ToInt32();
    if (objOutgameIndex < 0)
    {
        outFile << "Error: obj_outgame index is negative, indicating it was not found.\n";
        return;
    }
    outFile << "OutgameCreateAfter called." << std::endl;
    outFile << "Self instance: " << self << std::endl;
    if (other)
    {
        outFile << "Other instance: " << other << std::endl;
    }
    outFile << "Code object: " << code << std::endl;
    outFile << "Argument count: " << argc << std::endl;
    for (int i = 0; i < argc; ++i)
    {
        outFile << "Argument " << i << ": " << argv[i].ToString() << std::endl;
    }
    
}