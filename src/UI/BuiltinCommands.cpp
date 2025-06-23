#include "BuiltinCommands.h"
#include "DevConsole.h"
#include "../Utils.h"

namespace Organik
{

    namespace BuiltinCommands
    {
        static auto layerCache = std::unordered_map<int, std::unordered_map<int, std::string>*>();
        COMMAND(room_goto, "Usage: room_goto [room_name] - Attempt to go to [room_name]. Probably pretty high chance this crashes something.")
        {
            console->WriteLine("Attempting to go to room: " + args[0]);
            CInstance *global;
            Aurie::AurieStatus last_status = g_ModuleInterface->GetGlobalInstance(&global);
            if (last_status != Aurie::AURIE_SUCCESS || !global) {
                console->WriteLine("Error: Global instance not found");
                return;
            }
            RValue roomName = RValue(args[0]);
            RValue roomIndex = g_ModuleInterface->CallBuiltin("asset_get_index", {roomName});
            if (roomIndex.ToInt32() == -1) {
                console->WriteLine("Error: Room '" + args[0] + "' not found");
                return;
            }
            RValue newRoomID = RValue(false);
            console->WriteLine("Found room #: " + std::to_string(roomIndex.ToInt32()));
            if (!g_ModuleInterface->CallBuiltin("room_exists", {roomIndex}).ToBoolean()) {
                console->WriteLine("Error: Room with index " + std::to_string(roomIndex.ToInt32()) + " does not exist. Creating it now");
                newRoomID = g_ModuleInterface->CallBuiltin("room_add", {});
                if (newRoomID.ToInt32() == -1) {
                    console->WriteLine("Error: Failed to create new room");
                    return;
                }
                console->WriteLine("Created new room with index: " + std::to_string(newRoomID.ToInt32()));
                g_ModuleInterface->CallBuiltin("room_assign", {newRoomID, roomIndex});
                console->WriteLine("Assigned new room. Going to it now.");
            }
            if (!newRoomID.ToBoolean()) {
                newRoomID = roomIndex;
            }
            console->WriteLine("Going to room with index: " + std::to_string(newRoomID.ToInt32()));
            RValue result = g_ModuleInterface->CallBuiltin("room_goto", {newRoomID});
            g_devConsole->SetVisible(false);
        }  
        COMMAND(asset_get_tags, "Usage: asset_get_tags [asset_name] - Get tags for the specified asset.")
        {
            if (args.size() < 1) {
                console->WriteLine("Error: No asset name provided");
                return;
            }
            RValue assetName = RValue(args[0]);
            RValue tags = g_ModuleInterface->CallBuiltin("asset_get_tags", {assetName});
            std::vector<RValue> tagsV = tags.ToVector();

            console->WriteLine("Tags for asset '" + assetName.ToString() + "': " + Utils::joinRValStrings(tagsV, ", "));
        }
        COMMAND(asset_get_index, "Usage: asset_get_index [asset_name] - Get index for the specified asset.")
        {
            if (args.size() < 1) {
                console->WriteLine("Error: No asset name provided");
                return;
            }
            RValue assetName = RValue(args[0]);
            RValue assetIndex = g_ModuleInterface->CallBuiltin("asset_get_index", {assetName});
            if (assetIndex.ToInt32() == -1) {
                console->WriteLine("Error: Asset '" + assetName.ToString() + "' not found");
                return;
            }
            console->WriteLine("Asset '" + assetName.ToString() + "' has index: " + std::to_string(assetIndex.ToInt32()));
        }
        COMMAND(layer_get_all, "Usage: layer_get_all - Get all layers in the current room.")
        {
            CInstance *global;
            Aurie::AurieStatus last_status = g_ModuleInterface->GetGlobalInstance(&global);
            if (last_status != Aurie::AURIE_SUCCESS || !global) {
                console->WriteLine("Error: Global instance not found");
                return;
            }
            RValue layers = g_ModuleInterface->CallBuiltin("layer_get_all", {});
            std::vector<RValue> layersV = layers.ToVector();
            if (layersV.empty()) {
                console->WriteLine("No layers found in the current room.");
                return;
            }
            bool outToFile = ((args.size() > 1) || console->HasOutputFile());
            if (outToFile) {
                if (args.size() > 1) console->SetOutputFile(args[1]);
                console->WriteLine("Outputting layer information to file: " + console->GetOutputFile());
            }
            auto result = std::unordered_map<int, std::string>();
            std::string output = "";
            for (size_t i = 0; i < layersV.size(); ++i) {
                std::string layerName = g_ModuleInterface->CallBuiltin("layer_get_name", {layersV[i]}).ToString();
                std::string layerDepth = std::to_string(g_ModuleInterface->CallBuiltin("layer_get_depth", {layersV[i]}).ToInt32());
                std::string layerInfo = std::to_string(layersV[i].ToInt32()) + " ==> " + layerName + " (" + layerDepth + ")";
                result.insert(std::pair<int, std::string>(layersV[i].ToInt32(), layerInfo));
                
                if (i < layersV.size() - 1) {
                    console->WriteLine(layerInfo);
                }
            }

            console->WriteLine("Layers in the current room: " + Utils::joinRValStrings(layersV, ", "));
        }
        COMMAND(spawn_object_here, "Usage: spawn_object_here [object_index] (optional [layer_id]) - Spawn an instance of [object_name] at the current MOUSE position)")
        {
            CInstance * global;
            Aurie::AurieStatus last_status = g_ModuleInterface->GetGlobalInstance(&global);
            if (last_status != Aurie::AURIE_SUCCESS || !global) {
                console->WriteLine("Error: Global instance not found");
                return;
            }
            RValue mouseX = RValue(0);
            RValue mouseY = RValue(0);
//            console->WriteLine(std::to_string(__LINE__));
            g_ModuleInterface->GetBuiltin("mouse_x", global, NULL_INDEX, mouseX);
//            console->WriteLine(std::to_string(__LINE__));
            g_ModuleInterface->GetBuiltin("mouse_y", global, NULL_INDEX, mouseY);
            if (args.size() < 1) {
                console->WriteLine("Error: Not enough arguments.");
                this->Help(console, args);
                return;
            }
            int layerID;
            if (args.size() < 2) 
            {
//            console->WriteLine(std::to_string(__LINE__));
                layerID = g_ModuleInterface->CallBuiltin("layer_get_id", {RValue("Instances")}).ToInt32();
            }
            else
            {
                layerID = std::stoi(args[1]);
            }   
            int objectIndex = std::stoi(args[0]);
            
            console->WriteLine("Spawning object #" + std::to_string(objectIndex) + " at (" + 
                std::to_string(mouseX.ToInt32()) + ", " + std::to_string(mouseY.ToInt32()) + ") on layer " + std::to_string(layerID));
//            console->WriteLine(std::to_string(__LINE__));
            RValue newInstance = g_ModuleInterface->CallBuiltin("instance_create_layer", 
            {
                RValue(mouseX.ToInt32()), 
                RValue(mouseY.ToInt32()), 
                RValue(layerID),
                RValue(objectIndex),
                {}
            });
            console->WriteLine("Spawned instance "  + std::to_string(newInstance.ToInt32()) + " of object #" + std::to_string(objectIndex) + " at (" + 
                std::to_string(mouseX.ToInt32()) + ", " + std::to_string(mouseY.ToInt32()) + ") on layer " + std::to_string(layerID));
        }
        COMMAND(instance_vars, "Usage: instance_vars [instance_id] (optional: [out_filename]) - Get variable names for the specified instance.")
        {
            if (args.size() < 1) {
                console->WriteLine("Error: No instance ID provided");
                return;
            }
            RValue instanceID = RValue(-1);
            if (args[0] == "global") 
            {
                CInstance *global;
//            console->WriteLine(std::to_string(__LINE__));
                Aurie::AurieStatus last_status = g_ModuleInterface->GetGlobalInstance(&global);
                if (last_status != Aurie::AURIE_SUCCESS || !global) {
                    console->WriteLine("Error: Global instance not found");
                    return;
                }
                instanceID = global->ToRValue();
            }
            instanceID = (instanceID.ToInt32() > -1) ? instanceID : RValue(std::stoi(args[0]));
//            console->WriteLine(std::to_string(__LINE__));
            RValue varNames = g_ModuleInterface->CallBuiltin("variable_instance_get_names", {instanceID});
            std::vector<RValue> varNamesV = varNames.ToVector();
            bool outToFile = ((args.size() > 1) || console->HasOutputFile());
            if (outToFile) {
                if (args.size() > 1) console->SetOutputFile(args[1]);
                console->WriteLine("Outputting instance information to file: " + console->GetOutputFile());
            }
            if (varNamesV.empty()) {
                console->WriteLine("No variables found for instance " + std::to_string(instanceID.ToInt32()));
                return;
            }

            for (size_t i = 0; i < varNamesV.size(); ++i) {
//                console->WriteLine(std::to_string(__LINE__));
                RValue varType = g_ModuleInterface->CallBuiltin("typeof", {
                    g_ModuleInterface->CallBuiltin("variable_instance_get", {instanceID, varNamesV[i]})
                });
                console->WriteLine(varNamesV[i].ToString() + " (" + varType.ToString() + ")");
            }
        }
        COMMAND(variable_instance_get, "Usage: variable_instance_get [instance_id] [variable_name] - Get the value of a variable for the specified instance.")
        {
            if (args.size() < 2) {
                console->WriteLine("Error: Not enough arguments. Usage: variable_instance_get [instance_id] [variable_name]");
                return;
            }
            int instanceID = std::stoi(args[0]);
            console->WriteLine("Getting variable '" + args[1] + "' for instance " + std::to_string(instanceID));
            if (instanceID < 0) {
                console->WriteLine("Error: Invalid instance ID " + std::to_string(instanceID));
                return;
            }
            if (args[1].empty()) {
                console->WriteLine("Error: Variable name cannot be empty");
                return;
            }
            RValue instance = RValue(instanceID);
            RValue varName = RValue(args[1]);
//            console->WriteLine(std::to_string(__LINE__));
            RValue varValue = g_ModuleInterface->CallBuiltin("variable_instance_get", {instance, varName});
            std::string varValueStr;
            if (varValue.m_Kind == VALUE_INT32 || varValue.m_Kind == VALUE_INT64) {
                varValueStr = std::to_string(varValue.m_Kind == VALUE_INT64 ? varValue.ToInt64() : varValue.ToInt32());
            } else if (varValue.m_Kind == VALUE_REAL) {
                varValueStr = std::to_string(varValue.ToDouble());
            } else if (varValue.m_Kind == VALUE_STRING) {
                varValueStr = varValue.ToString();
            } else if (varValue.m_Kind == VALUE_BOOL) {
                varValueStr = varValue.ToBoolean() ? "true" : "false";
            } else {
//                console->WriteLine(std::to_string(__LINE__));
                varValueStr = "Unsupported type: " + varValue.GetKindName();
            }
//            console->WriteLine(std::to_string(__LINE__));
            console->WriteLine("Variable '" + args[1] + "' for instance " + std::to_string(instanceID) + ": " + varValueStr);
        }
        COMMAND(variable_instance_set, "Usage: variable_instance_set [instance_id] [variable_name] [value_type] [value] - Set the value of a variable for the specified instance.")
        {
            if (args.size() < 4) {
                console->WriteLine("Error: Not enough arguments.");
                this->Help(console, {});
                return;
            }
            int instanceID = std::stoi(args[0]);
            RValue instance = RValue(instanceID);
            RValue varName = RValue(args[1]);
            std::string value_type = args[2];
            if (value_type == "int") {
                int value = std::stoi(args[3]);
                g_ModuleInterface->CallBuiltin("variable_instance_set", {instance, varName, RValue(value)});
            } else if (value_type == "double") {
                double value = std::stod(args[3]);
                g_ModuleInterface->CallBuiltin("variable_instance_set", {instance, varName, RValue(value)});
            } else if (value_type == "string") {
                std::string value = args[3];
                g_ModuleInterface->CallBuiltin("variable_instance_set", {instance, varName, RValue(value)});
            } else if (value_type == "bool") {
                bool value = (args[3] == "true" || args[3] == "1");
                g_ModuleInterface->CallBuiltin("variable_instance_set", {instance, varName, RValue(value)});
            } else {
                console->WriteLine("Error: Unsupported value type '" + value_type + "'");
                return;
            }
            console->WriteLine("Set variable '" + args[1] + "' for instance " + std::to_string(instanceID) + " to " + args[3]);
        }
        COMMAND(instance_find, "Usage: instance_find [object_name] - list all known instances of [object_name].")
        {
            if (args.size() < 1) {
                console->WriteLine("Error: No object name provided");
                return;
            }
//            console->WriteLine(std::to_string(__LINE__));
            CInstance *global;
            Aurie::AurieStatus last_status = g_ModuleInterface->GetGlobalInstance(&global);
            if (last_status != Aurie::AURIE_SUCCESS || !global) {
                console->WriteLine("Error: Global instance not found");
                return;
            }
            RValue objIndex = ref_noone;
            if (args[0][0] > '0' && args[0][0] <= '9') {
//                console->WriteLine(std::to_string(__LINE__));
                objIndex = RValue(std::stoi(args[0]));
            } else {
//                console->WriteLine(std::to_string(__LINE__));
                objIndex = g_ModuleInterface->CallBuiltin("asset_get_index", {RValue(args[0])});
            }
            if (objIndex.ToInt32() == ref_noone.ToInt32()) {
                console->WriteLine("Error: Object '" + args[0] + "' not found");
                return;
            }
            console->WriteLine("Finding instances of object #" + std::to_string(objIndex.ToInt32()));
//            console->WriteLine(std::to_string(__LINE__));
            RValue instance_count = g_ModuleInterface->CallBuiltin("instance_number", {objIndex});
            if (instance_count.ToInt32() <= 0) {
                console->WriteLine("No instances of object #" + std::to_string(objIndex.ToInt32()) + " found.");
                return;
            }
            console->WriteLine("Found " + std::to_string(instance_count.ToInt32()) + " instances of object #" + std::to_string(objIndex.ToInt32()));
            for (int i=0; i< instance_count.ToInt32(); ++i) {
//                console->WriteLine(std::to_string(__LINE__));
                RValue instanceID = g_ModuleInterface->CallBuiltin("instance_find", {objIndex, RValue(i)});
                if (instanceID.ToInt32() == ref_noone.ToInt32()) { // shouldn't be possible, according to GM docs
                    console->WriteLine("Error: Instance #" + std::to_string(i) + " of object #" + std::to_string(objIndex.ToInt32()) + " not found");
                    continue;
                }
                console->WriteLine("Instance #" + std::to_string(i) + " - ID: " + std::to_string(instanceID.ToInt32()));
                
            }
        }
        COMMAND(help, "Usage: help [command_name] - Get help for a specific command or list all commands.")
        {
            if (args.size() > 0) {
                std::string commandName = args[0];
                auto it = DevConsole::Commands().find(commandName);
                if (it != DevConsole::Commands().end()) {
                    it->second->Help(console, args);
                } else {
                    console->WriteLine("Error: Command '" + commandName + "' not found");
                }
            } else {
                console->WriteLine("Available commands:");
                for (const auto& cmd : DevConsole::Commands()) {
                    console->WriteLine(cmd.first);
                    cmd.second->Help(console, {});
                }
            }
        }
    }
}
using namespace Organik::BuiltinCommands;
void Organik::BuiltinCommands::InstallCommands()
{
    static room_goto roomGotoCommand = room_goto();
    static asset_get_tags asset_get_tags_cmd = asset_get_tags();
    static asset_get_index asset_get_index_cmd = asset_get_index();
    static layer_get_all layer_get_all_cmd = layer_get_all();
    static spawn_object_here spawn_object_here_cmd = spawn_object_here();
    static instance_vars instance_vars_cmd = instance_vars();
    static variable_instance_get variable_instance_get_cmd = variable_instance_get();
    static variable_instance_set variable_instance_set_cmd = variable_instance_set();
    static instance_find instance_find_cmd = instance_find();
    static help help_cmd = help();
    DevConsole::AddCommandHandler(&roomGotoCommand);
    DevConsole::AddCommandHandler(&asset_get_tags_cmd);
    DevConsole::AddCommandHandler(&asset_get_index_cmd);
    DevConsole::AddCommandHandler(&layer_get_all_cmd);
    DevConsole::AddCommandHandler(&spawn_object_here_cmd);
    DevConsole::AddCommandHandler(&instance_vars_cmd);
    DevConsole::AddCommandHandler(&variable_instance_get_cmd);
    DevConsole::AddCommandHandler(&variable_instance_set_cmd);
    DevConsole::AddCommandHandler(&instance_find_cmd);
    DevConsole::AddCommandHandler(&help_cmd);
}