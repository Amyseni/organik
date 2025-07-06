#include "BuiltinCommands.h"
#include "../Utils.h"

namespace Organik
{
    namespace BuiltinCommands
    {
        static auto layerCache = std::unordered_map<int, std::unordered_map<int, std::string>*>();
        COMMAND(room_goto, "Usage: room_goto [room_name] - Attempt to go to [room_name]. Probably pretty high chance this crashes something.")
        {
            console->AddLog("Attempting to go to room: %s" + args[0]);
            Utils::roomGoto(args[0]);
        }
        COMMAND(asset_get_tags, "Usage: asset_get_tags [asset_name] - Get tags for the specified asset.")
        {
            if (args.size() < 1) {
                console->AddLog("Error: No asset name provided");
                return;
            }
            RValue assetName = RValue(args[0]);
            RValue tags = g_ModuleInterface->CallBuiltin("asset_get_tags", {assetName});
            std::vector<RValue> tagsV = tags.ToVector();

            console->AddLog("Tags for asset '%s': %s", args[0].c_str(), Utils::joinRValStrings(tagsV, ", ").c_str());
        }
        COMMAND(asset_get_index, "Usage: asset_get_index [asset_name] - Get index for the specified asset.")
        {
            if (args.size() < 1) {
                console->AddLog("Error: No asset name provided");
                return;
            }
            int assetIndex = Utils::assetGetIndex(args[0]);
            console->AddLog("Asset '%s' has index: %d", args[0].c_str(), assetIndex);
        }
        COMMAND(layer_get_all, "Usage: layer_get_all - Get all layers in the current room.")
        {
            CInstance *global;
            Aurie::AurieStatus last_status = g_ModuleInterface->GetGlobalInstance(&global);
            if (last_status != Aurie::AURIE_SUCCESS || !global) {
                console->AddLog("Error: Global instance not found");
                return;
            }
            RValue layers = g_ModuleInterface->CallBuiltin("layer_get_all", {});
            std::vector<RValue> layersV = layers.ToVector();
            if (layersV.empty()) {
                console->AddLog("No layers found in the current room.");
                return;
            }
            auto result = std::unordered_map<int, std::string>();
            std::string output = "";
            for (size_t i = 0; i < layersV.size(); ++i) {
                std::string layerName = g_ModuleInterface->CallBuiltin("layer_get_name", {layersV[i]}).ToString();
                std::string layerDepth = std::to_string(g_ModuleInterface->CallBuiltin("layer_get_depth", {layersV[i]}).ToInt32());
                std::string layerInfo = std::to_string(layersV[i].ToInt32()) + " ==> " + layerName + " (" + layerDepth + ")";

                console->AddLog("%s", layerInfo);
            }
        }
        COMMAND(spawn_object_here, "Usage: spawn_object_here [object_index] (optional [layer_id]) - Spawn an instance of [object_name] at the current MOUSE position)")
        {
            CInstance * global;
            Aurie::AurieStatus last_status = g_ModuleInterface->GetGlobalInstance(&global);
            if (last_status != Aurie::AURIE_SUCCESS || !global) {
                console->AddLog("Error: Global instance not found");
                return;
            }
            RValue mouseX = RValue(0);
            RValue mouseY = RValue(0);
            g_ModuleInterface->GetBuiltin("mouse_x", global, NULL_INDEX, mouseX);
            g_ModuleInterface->GetBuiltin("mouse_y", global, NULL_INDEX, mouseY);
            if (args.size() < 1) {
                console->AddLog("Error: Not enough arguments.");
                this->Help(console, args);
                return;
            }
            int layerID;
            if (args.size() < 2) 
            {
                layerID = g_ModuleInterface->CallBuiltin("layer_get_id", {RValue("Instances")}).ToInt32();
            }
            else
            {
                layerID = std::stoi(args[1]);
            }   
            int objectIndex = std::stoi(args[0]);
            
            console->AddLog("Spawning object #%d at (%d, %d) on layer %d", objectIndex, mouseX.ToInt32(), mouseY.ToInt32(), layerID);
            RValue newInstance = g_ModuleInterface->CallBuiltin("instance_create_layer", 
            {
                RValue(mouseX.ToInt32()), 
                RValue(mouseY.ToInt32()), 
                RValue(layerID),
                RValue(objectIndex),
                {}
            });
            console->AddLog("Spawned instance %d at (%d, %d) on layer %d",
                newInstance.ToInt32(),
                mouseX.ToInt32(),
                mouseY.ToInt32(),
                layerID);
        }
        COMMAND(instance_vars, "Usage: instance_vars [instance_id] (optional: [out_filename]) - Get variable names for the specified instance.")
        {
            if (args.size() < 1) {
                console->AddLog("Error: No instance ID provided");
                return;
            }
            RValue instanceID = RValue(std::stoi(args[0]));
            RValue varNames = g_ModuleInterface->CallBuiltin("variable_instance_get_names", {instanceID});
            std::vector<RValue> varNamesV = varNames.ToVector();

            for (size_t i = 0; i < varNamesV.size(); ++i) {
                RValue varType = g_ModuleInterface->CallBuiltin("typeof", {
                    g_ModuleInterface->CallBuiltin("variable_instance_get", {instanceID, varNamesV[i]})
                });
                console->AddLog("%s (%s)", varNamesV[i].ToString().c_str(), varType.ToString().c_str());
            }
        }
        COMMAND(variable_global_get, "Usage: variable_global_get [variable_name] - Get the value of a variable for the specified instance.")
        {
            if (args.size() < 1) {
                console->AddLog("Error: Not enough arguments.");
                this->Help(console, {});
                return;
            }
            RValue varName = RValue(args[0]);
            RValue varValue = g_ModuleInterface->CallBuiltin("variable_global_get", {varName});
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
                varValueStr = "Unsupported type: " + varValue.GetKindName();
            }
            console->AddLog("Variable '%s': %s", args[1].c_str(), varValueStr.c_str());
        }
        COMMAND(variable_global_set, "Usage: variable_global_set [variable_name] [value_type] [value]  - Set the value of a variable for the specified instance.")
        {
            if (args.size() < 3) {
                console->AddLog("Error: Not enough arguments.");
                this->Help(console, {});
                return;
            }
            RValue varName = RValue(args[0]);
            std::string value_type = args[1];
            if (value_type == "int") {
                int value = std::stoi(args[2]);
                g_ModuleInterface->CallBuiltin("variable_global_set", {varName, RValue(value)});
            } else if (value_type == "double") {
                double value = std::stod(args[2]);
                g_ModuleInterface->CallBuiltin("variable_global_set", {varName, RValue(value)});
            } else if (value_type == "string") {
                std::string value = args[2];
                g_ModuleInterface->CallBuiltin("variable_global_set", {varName, RValue(value)});
            } else if (value_type == "bool") {
                bool value = (args[2] == "true" || args[2] == "1");
                g_ModuleInterface->CallBuiltin("variable_global_set", {varName, RValue(value)});
            } else {
                console->AddLog("Error: Unsupported value type '%s'", value_type.c_str());
                return;
            }
            console->AddLog("Set variable '%s' to %s", args[1].c_str(), args[3].c_str());
        }
        COMMAND(variable_instance_get, "Usage: variable_instance_get [instance_id] [variable_name] - Get the value of a variable for the specified instance.")
        {
            if (args.size() < 2) {
                console->AddLog("Error: Not enough arguments. Usage: variable_instance_get [instance_id] [variable_name]");
                return;
            }
            int instanceID = std::stoi(args[0]);
            console->AddLog("Getting variable '%s' for instance %s", args[1].c_str(), std::to_string(instanceID).c_str());
            if (instanceID < 0) {
                console->AddLog("Error: Invalid instance ID %d", instanceID);
                return;
            }
            if (args[1].empty()) {
                console->AddLog("Error: Variable name cannot be empty");
                return;
            }
            RValue instance = RValue(instanceID);
            RValue varName = RValue(args[1]);
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
                varValueStr = "Unsupported type: " + varValue.GetKindName();
            }
            console->AddLog("Variable '%s' for instance %s: %s", args[1].c_str(), std::to_string(instanceID).c_str(), varValueStr.c_str());
        }
        COMMAND(variable_instance_set, "Usage: variable_instance_set [instance_id] [variable_name] [value_type] [value] - Set the value of a variable for the specified instance.")
        {
            if (args.size() < 4) {
                console->AddLog("Error: Not enough arguments.");
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
                console->AddLog("Error: Unsupported value type '%s'", value_type.c_str());
                return;
            }
            console->AddLog("Set variable '%s' for instance %s to %s", args[1].c_str(), std::to_string(instanceID).c_str(), args[3].c_str());
        }
        COMMAND(instance_find, "Usage: instance_find [object_name] - list all known instances of [object_name].")
        {
            if (args.size() < 1) {
                console->AddLog("Error: No object name provided");
                return;
            }
            CInstance *global;
            Aurie::AurieStatus last_status = g_ModuleInterface->GetGlobalInstance(&global);
            if (last_status != Aurie::AURIE_SUCCESS || !global) {
                console->AddLog("Error: Global instance not found");
                return;
            }
            RValue objIndex = ref_noone;
            if (args[0][0] > '0' && args[0][0] <= '9') {
                objIndex = RValue(std::stoi(args[0]));
            } else {
                objIndex = g_ModuleInterface->CallBuiltin("asset_get_index", {RValue(args[0])});
            }
            if (objIndex.ToInt32() == ref_noone.ToInt32()) {
                console->AddLog("Error: Object '%s' not found" + args[0]);
                return;
            }
            console->AddLog("Finding instances of object #%d", objIndex.ToInt32());
            RValue instance_count = g_ModuleInterface->CallBuiltin("instance_number", {objIndex});
            if (instance_count.ToInt32() <= 0) {
                console->AddLog("No instances of object #%d found.", objIndex.ToInt32());
                return;
            }
            console->AddLog("Found %d instances of object #%d", instance_count.ToInt32(), objIndex.ToInt32());
            for (int i=0; i< instance_count.ToInt32(); ++i) {
                RValue instanceID = g_ModuleInterface->CallBuiltin("instance_find", {objIndex, RValue(i)});
                if (instanceID.ToInt32() == ref_noone.ToInt32()) { // shouldn't be possible, according to GM docs
                    console->AddLog("Error: Instance #%d of object #%d not found", i, objIndex.ToInt32());;
                    continue;
                }
                console->AddLog("Instance #%d - ID: %d", i, instanceID.ToInt32());  
            }
        }
        COMMAND(variable_watch, "Usage: variable_watch [instance_id] [variable_name] - Watch a variable for changes.")
        {
            if (args.size() < 2) {
                console->AddLog("Error: Not enough arguments. Usage: variable_watch [instance_id] [variable_name]");
                return;
            }
            int instanceID = std::stoi(args[0]);
            RValue instance = RValue(instanceID);
            RValue varName = RValue(args[1]);
            RValue varValue = g_ModuleInterface->CallBuiltin("variable_instance_get", {instance, varName});
            console->AddLog("Watching variable '%s' for instance %d: initial value is %s", args[1].c_str(), instanceID, varValue.ToString().c_str());
            
            
        }
        COMMAND(help, "Usage: help [command_name] - Get help for a specific command or list all commands.")
        {
            if (args.size() > 0) {
                std::string commandName = args[0];
                auto it = console->Commands().find(commandName);
                if (it != console->Commands().end()) {
                    it->second->Help(console, args);
                } else {
                    console->AddLog("Error: Command '%s' not found", commandName.c_str());
                }
            } else {
                console->AddLog("Available commands:");
                for (const auto& cmd : console->Commands()) {
                    console->AddLog(cmd.first.c_str());
                    cmd.second->Help(console, {});
                }
            }
        }
    }
}
using namespace Organik::BuiltinCommands;

std::map<std::string, Command*> Organik::BuiltinCommands::GetCommands()
{
    static room_goto roomGotoCommand = room_goto();
    static asset_get_tags asset_get_tags_cmd = asset_get_tags();
    static asset_get_index asset_get_index_cmd = asset_get_index();
    static layer_get_all layer_get_all_cmd = layer_get_all();
    static spawn_object_here spawn_object_here_cmd = spawn_object_here();
    static instance_vars instance_vars_cmd = instance_vars();
    static variable_instance_get variable_instance_get_cmd = variable_instance_get();
    static variable_global_get variable_global_get_cmd = variable_global_get();
    static variable_instance_set variable_instance_set_cmd = variable_instance_set();
    static variable_global_set variable_global_set_cmd = variable_global_set();
    static instance_find instance_find_cmd = instance_find();
    static help help_cmd = help();
    std::map<std::string, Command*> commands;
    commands.insert(std::pair<std::string, Command*>(roomGotoCommand.Name(), &roomGotoCommand));
    commands.insert(std::pair<std::string, Command*>(asset_get_tags_cmd.Name(), &asset_get_tags_cmd));
    commands.insert(std::pair<std::string, Command*>(asset_get_index_cmd.Name(), &asset_get_index_cmd));
    commands.insert(std::pair<std::string, Command*>(layer_get_all_cmd.Name(), &layer_get_all_cmd));
    commands.insert(std::pair<std::string, Command*>(spawn_object_here_cmd.Name(), &spawn_object_here_cmd));
    commands.insert(std::pair<std::string, Command*>(instance_vars_cmd.Name(), &instance_vars_cmd));
    commands.insert(std::pair<std::string, Command*>(variable_instance_get_cmd.Name(), &variable_instance_get_cmd));
    commands.insert(std::pair<std::string, Command*>(variable_global_get_cmd.Name(), &variable_global_get_cmd));
    commands.insert(std::pair<std::string, Command*>(variable_instance_set_cmd.Name(), &variable_instance_set_cmd));
    commands.insert(std::pair<std::string, Command*>(variable_global_set_cmd.Name(), &variable_global_set_cmd));
    commands.insert(std::pair<std::string, Command*>(instance_find_cmd.Name(), &instance_find_cmd));
    commands.insert(std::pair<std::string, Command*>(help_cmd.Name(), &help_cmd));
    return commands;
}