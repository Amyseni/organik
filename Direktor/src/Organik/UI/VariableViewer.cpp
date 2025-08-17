#include "Synthetik.h"
#include "zhl_internal.h"
#include "zhl.h"
#include "DefinitionHelpers/InstanceHelper.h"
#include "VariableHelper.h"
#include "VariableViewer.h"
#include "ItemFilter.h"
#include "Globals.h"
#include "UIManager.h"
#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_impl_win32.h"
<<<<<<< HEAD
=======
#include "Logging.h"
#include "DefinitionHelpers/BuiltinHelper.h"
#include "Utils.h"
>>>>>>> 073bc0d (std::launder for squeaky clean RValues)
#include "imgui/imgui_impl_dx11.h"

bool InstanceVariableViewer::showPopup = false;
bool InstanceVariableViewer::showArrayPopup = false;
bool matchAllVariables = false; // false = ay, true = all

std::map<int32_t, bool> variablesToMatchMap = std::map<int32_t, bool>();
std::map<int32_t, std::unordered_map<int32_t, std::string>> varMapCache;
std::string objectHasVariableFilter;
std::string variableNameFitler;
std::string variableTypeFilter;
#define VARHASH_FROMNAME(name) Organik::Variables::Hashes[Organik::Variables::##name]
#define VARNAME_FROMHASH(hash) Organik::Variables::HashToVariableMap[hash]
static bool dummyDoTheTHING = []() -> bool {
    // Initialize the variablesToMatchMap with all variables
    for (const auto& hash : Organik::Variables::Hashes) {
        variablesToMatchMap[hash] = false; // Default to not matching
    }
    return true;
}();
bool InstanceVariableViewer::matchAll = false;
bool InstanceVariableViewer::matchNone = false;
bool InstanceVariableViewer::matchAny = true;

static auto instanceFilter = ItemFilter();

int32_t BeginVariablesChildAndTree(CInstance* instance)
{
    int32_t ret = 0 + ImGui::BeginChild("##variables", ImVec2(0, 0), ImGuiChildFlags_Borders);
    if (!ret) {
        ImGui::EndChild();
        return ret;
    }

    ImGui::Text("Variables for: %s (ID: %d)", 
        instance->m_Object->m_Name,
        instance->m_ID);

    bool beginTable = false;
    // Display all variables in a table
    beginTable = ImGui::BeginTable("Variables", 3, 
            ImGuiTableFlags_Borders | 
            ImGuiTableFlags_RowBg | 
            ImGuiTableFlags_Resizable |
            ImGuiTableFlags_ScrollY);
    if (beginTable)
    {
        ret++;
        // Setup columns
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 150.0f);
        ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 80.0f);
        ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();
    }
    return ret;
}
#define GLOBALS_DO(make) \
    make(outgame) \
    make(obj_stats) \
    make(obj_control) \
    make(obj_allgame) \
    make(obj_ingame_ctrl) \
    make(obj_room_ctrl) \
    make(obj_enemy_ctrl) \
    make(obj_weaponstats) \
    make(obj_itemstats) \
    make(obj_statistics) \
    make(obj_color) \
    make(obj_terrorlevel) \
    make(obj_console) \
    make(obj_player) \
    make(WeaponSystem) \
    make(obj_all_bullets) \
    make(obj_bullet_lists) \
    make(obj_player_bullets) \
    make(bulletSystem) \
    make(itemSystem) \
    make(perkSystem) \


#define PARENTS_DO(make) \
    make(obj_enm_parent_all) \
    make(obj_enm_base_parent) \
    make(obj_perk_parent) \
    make(obj_artefact_parent) \
    make(obj_enm_turret_parent) \
    make(obj_boss_parent) \
    make(obj_enm_bossweapon_parent) \
    make(obj_drone_parent) \
    make(obj_headshot_collision_parent) \
    make(obj_moving_entity_PARENT) \
    make(obj_weapon_PARENT) \
    make(obj_weapon_uber_parent) \
    make(obj_weapon_item_parent) \
    make(obj_weapon_buff_parent) \
    make(obj_bullet_rocket_parent) \
    make(obj_bullet_beam_parent) \
    make(obj_bullet_laser_parent) \
    make(obj_chest_parent) \
    make(obj_trap_mine_parent) \
    make(obj_trap_parent) \
    make(obj_props_solid_parent) \
    make(obj_props_notsolid_parent) \
    make(obj_pickup_parent) \
    make(obj_questcontrol_parent) \
    make(obj_item_parent) \
    make(obj_orb_parent) \
    make(obj_effect_parent) \
    make(obj_explosion_parent) \
    make(obj_enm_budget_parent) \
    make(obj_chunk_parent) \
    make(obj_networking_parent) \
    make(obj_chat_parent) \
    make(obj_patchpage_parent) \
    make(obj_patchlog_parent) \
    make(obj_patchbutton_parent) \
    make(obj_enm_custom_parent) \
    make(obj_enm_headhunter_arena_parent) \
    make(obj_enm_building_parent) \
    make(obj_sawbig_parent) \
    make(obj_bullet_mortar_parent) \
    make(obj_arena_parent) \
    make(T_perk_Riotguard_PARENT) \
    make(T_perk_Breacher_PARENT) \
    make(T_perk_SWAT_PARENT) \
    make(obj_research_button_PARENT) \
    make(T_PARENT_ALL_BONI) \
    make(T_Path_PARENT) \
    make(T_Path_start_PARENT) \
    make(T_path_god_start_PARENT) \
    make(T_item_PARENT) \
    make(T_slot_PARENT) \
    make(T_stat_PARENT) \
    make(T_weapon_PARENT) \
    make(T_perk_PARENT) \
    make(T_perk_general_PARENT) \
    make(T_perk_elite_PARENT) \
    make(T_perk_psyonic_PARENT) \
    make(T_perk_support_PARENT) \
    make(T_perk_alchemist_PARENT) \
    make(T_perk_rogue_PARENT) \
    make(T_perk_tank_PARENT) \
    make(obj_chip_parent) \
    make(obj_questlog_parent) \
    make(obj_turret_boss_spawn_parent) \
    make(obj_firingrange_dummy_parent) \
    make(obj_helicopterBoss_parent) \
    make(obj_noncombat_ground_Parent) \
    make(obj_enm_hover_parent) \
    make(obj_enm_tank_parent) \
    make(obj_shell_parentOLD) \
    make(obj_trap_mine_stealthparent) \
    make(obj_movingwall_parent) \
    make(obj_explosion_beam_parent) \
    make(obj_smoke_parent) \
    make(obj_particle_gravity_parent) \
    make(obj_followunit_parent) \
    make(obj_voidzone_parent) \
    make(obj_particle_parent) \
    make(obj_mortarshot_parent) \
    make(obj_lightemitter_parent) \
    make(obj_ground_text_parent) \
    make(obj_wall_parent) \
    make(obj_door_parent) \
    make(obj_pipe_center_parent) \
    make(obj_concback_parent) \
    make(obj_mm_menu_parent) \
    make(obj_mm_acc_input_parent) \
    make(obj_railing_parent) \
    make(obj_collision_blockout_parent) \
    make(obj_floor_blockout_parent) \
    make(obj_field_parent) \
    make(obj_fields_parent) \
    
#define OBJINDEX(name) Organik::Objects::ObjIndexes[Organik::Objects::##name]
#define MAPOBJ(name) {OBJINDEX(name), Object_Data(OBJINDEX(name))},
std::unordered_map<int32_t, CObjectGM*> *GetParentObjects()
{
    static std::unordered_map<int32_t, CObjectGM*> parentObjects;
    if (parentObjects.size() == 0)
    {
        parentObjects = {
            PARENTS_DO(MAPOBJ)
        };
    }
    return &parentObjects;
}
std::unordered_map<int32_t, CObjectGM*> *GetGlobalObjects()
{
    static std::unordered_map<int32_t, CObjectGM*> globalObjects;
    if (globalObjects.size() == 0)
    {
        globalObjects = {
            GLOBALS_DO(MAPOBJ)
        };
    }
    return &globalObjects;
}
std::unordered_map<int32_t, CObjectGM*> *GetTrackedObjects()
{
    static std::unordered_map<int32_t, CObjectGM*> trackedObjects;
    return &trackedObjects;
}
static std::string objNameFilterBuf;
void InstanceVariableViewer::DrawObjectParentsFilterList()
{
    static ImGuiTextFilter objNameFilter;
    static bool filterObjects = false;
    Organik::GetLogger()->LogFormatted("%s: %s -- %d", __FILE__, __FUNCTION__, __LINE__);
    auto trackedObjects = GetTrackedObjects();
    if (ImGui::InputTextWithHint("##obj_name_filter", "Search Parent Object Name.", &objNameFilterBuf))
    {
        if (!objNameFilterBuf.empty()){
            strncpy(&objNameFilter.InputBuf[0], objNameFilterBuf.data(), IM_ARRAYSIZE(objNameFilter.InputBuf));
            objNameFilter.Build();
            filterObjects = true;
            Organik::GetLogger()->LogFormatted("%s: %s -- %d", __FILE__, __FUNCTION__, __LINE__);
        }
        else
        {
            Organik::GetLogger()->LogFormatted("%s: %s -- %d", __FILE__, __FUNCTION__, __LINE__);
            objNameFilter.Clear();
            filterObjects = false;
        }
    }
    ImGui::NewLine();
    if (ImGui::CollapsingHeader("Object Filters"))
    {
        Organik::GetLogger()->LogFormatted("%s: %s -- %d", __FILE__, __FUNCTION__, __LINE__);
        if (ImGui::BeginChild("##trackedobj_list", ImVec2(0.0f, 350.0f), ImGuiChildFlags_ResizeX | ImGuiChildFlags_Borders))
        {
            Organik::GetLogger()->LogFormatted("%s: %s -- %d", __FILE__, __FUNCTION__, __LINE__);
            for (auto& pair : (*GetTrackedObjects()))
            {
                if (filterObjects && !objNameFilter.PassFilter(pair.second->m_Name))
                    continue;
                if (ImGui::Selectable(pair.second->m_Name, true, ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_Highlight))
                {
                    if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                    {
                        trackedObjects->erase(pair.first);
                    }
                }
            }
            Organik::GetLogger()->LogFormatted("%s: %s -- %d", __FILE__, __FUNCTION__, __LINE__);
        }
        ImGui::EndChild();
        ImGui::SameLine();
        Organik::GetLogger()->LogFormatted("%s: %s -- %d", __FILE__, __FUNCTION__, __LINE__);
        if (ImGui::BeginChild("##objparents_list", ImVec2(0.0f, 350.0f), ImGuiChildFlags_ResizeX | ImGuiChildFlags_Borders))
        {
            for (auto& pair : (*GetParentObjects()))
            {
                if ((filterObjects && !objNameFilter.PassFilter(pair.second->m_Name))
                    || trackedObjects->contains(pair.first))
                    continue;
                if (ImGui::Selectable(pair.second->m_Name, false, ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_Highlight))
                {
                    if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                    {
                        (*trackedObjects)[pair.first] = pair.second;
                    }
                }
            }
            ImGui::EndChild();
        }
        ImGui::EndChild();
        ImGui::SameLine();
        Organik::GetLogger()->LogFormatted("%s: %s -- %d", __FILE__, __FUNCTION__, __LINE__);
        if (ImGui::BeginChild("##objglobals_list", ImVec2(0.0f, 350.0f), ImGuiChildFlags_ResizeX | ImGuiChildFlags_Borders))
        {
            for (auto& pair : (*GetGlobalObjects()))
            {
                if ((filterObjects && !objNameFilter.PassFilter(pair.second->m_Name))
                    || trackedObjects->contains(pair.first))
                    continue;
                if (ImGui::Selectable(pair.second->m_Name, false, ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_Highlight))
                {
                    if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                    {
                        (*trackedObjects)[pair.first] = pair.second;
                    }
                }
            }
        }
        ImGui::EndChild();
        ImGui::NewLine();
        Organik::GetLogger()->LogFormatted("%s: %s -- %d", __FILE__, __FUNCTION__, __LINE__);
        ImGui::Text("Double click to add/remove from tracked objects.");
    }
}
void InstanceVariableViewer::DrawObjectList()
{
    auto trackedObjects = GetTrackedObjects();
    if (ImGui::BeginChild("##tree", ImVec2(300, 0), ImGuiChildFlags_ResizeX | ImGuiChildFlags_Borders | ImGuiChildFlags_NavFlattened))
    {
<<<<<<< HEAD
        Organik::GetLogger()->LogFormatted("%s: %s -- %d", __FILE__, __FUNCTION__, __LINE__);
        for (auto& pair : (*trackedObjects)) // pair = {objectID, vector<CInstance*>}
        {
            ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanFullWidth;
=======
        std::for_each(instanceMap.begin(), instanceMap.end(), [&](std::pair<int32_t, std::vector<CInstance*>> pair) {
            int32_t objectID = pair.first;
            RValue objectNameRVal = RValue(pair.second.at(0)->m_Object->m_Name);
            //DoBuiltinRef(&gml_object_get_name, objectNameRVal, {RValue(objectID)});
            std::string objectName = objectNameRVal.ToString();
>>>>>>> 073bc0d (std::launder for squeaky clean RValues)

            if (ImGui::TreeNodeEx((void*)pair.second, node_flags, "%s (%d)", pair.second->m_Name, pair.second->m_Instances.m_Count))
            {
                CInstance* inst = pair.second->m_Instances.m_First;

                while (inst)
                {
                    int32_t ID = inst->m_ID;
                    ImGui::PushID(inst);
                    if (ImGui::Selectable(
                        ("##instance_" + std::to_string(ID)).c_str(),
                        (VisibleInstance && VisibleInstance == inst),
                        ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap
                    ))
                    {
                        VisibleInstance = inst;
                    }
                    ImGui::SameLine(0,0);
                    ImGui::Text("#%d (%d vars)", 
                        ID, 
<<<<<<< HEAD
                        inst->m_YYVarsMap->m_CurrentSize
=======
                        instance->m_YYVarsMap->m_CurrentSize
>>>>>>> 073bc0d (std::launder for squeaky clean RValues)
                    );
                    ImGui::Separator();
                    ImGui::PopID();

                    if (inst == pair.second->m_InstancesRecursive.m_Last)
                        break;
                    inst = inst->m_Next;
                }
                ImGui::TreePop();
            }
        }
        Organik::GetLogger()->LogFormatted("%s: %s -- %d", __FILE__, __FUNCTION__, __LINE__);
    }
    ImGui::EndChild();
}
void InstanceVariableViewer::DisplayVarTableEntry(CInstance* instance, const std::string& memberName, RValue* memberValue)
{
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::PushID(memberName.c_str());

    DisplayVariableValue(memberName, memberValue);
}
void InstanceVariableViewer::DrawInner()
{
    DrawObjectParentsFilterList();
    ImGui::Separator();
    
    Organik::GetLogger()->LogFormatted("%s: %s -- %d", __FILE__, __FUNCTION__, __LINE__);

    DrawObjectList();
    Organik::GetLogger()->LogFormatted("%s: %s -- %d", __FILE__, __FUNCTION__, __LINE__);
    
    ImGui::SameLine();
    CInstance* instance;
    if (instance = VisibleInstance)
    {
        // basically both of these checks should be unreachable code, but you know how I get down.
	    if (!instance)
            return;
        int id = instance->m_ID;

        if (!instance->m_YYVarsMap || instance->m_YYVarsMap->m_UsedCount == 0)
        {
            ImGui::Text("Instance has no variable map.");
            return;
        }
        int32_t objectIndex = instance->m_ObjectIndex;
        std::string objectName = instance->m_Object->m_Name;
        bool rowClicked = false;
        if (ImGui::BeginChild("##variables", ImVec2(0, 0), ImGuiChildFlags_Borders))
        {
<<<<<<< HEAD
=======
            // Get object name for header
            int32_t objectIndex = instance->m_ObjectIndex;
            RValue objectNameRVal;
            DoBuiltinRef(&gml_object_get_name, objectNameRVal, {RValue(objectIndex)});
	        Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
            std::string objectName = objectNameRVal.ToString();
>>>>>>> 073bc0d (std::launder for squeaky clean RValues)
            ImGui::Text("Variables for: %s (ID: %d)", 
                       objectName.c_str(), 
                       id);
            ImGui::Separator();
<<<<<<< HEAD
=======
            
	        Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
            std::map<std::string, RValue*> memberMap;
            if (!varMapCache.contains(id) || (instance->GetMemberCount() != varMapCache[id].size()))
            {
                constexpr int szInst = sizeof(CInstance);
                GetLogger()->LogFormatted("Caching variable map for instance ID %d", id);
                // TODO:auto varmap = Organik::Utils::GetVariableMap(instance); == ALMOST WORKS BUT NOT QUITE
                // GetLogger()->LogFormatted("Returnd: %s: %d", refResult.GetKindName(), refResult.GetArrayLength());
                RValue instanceVarNames = DoBuiltin(&gml_variable_instance_get_names, { RValue(id) });
                std::vector<RValue*> refVector = instanceVarNames.ToRefVector();
                std::vector<std::string> memberNames;
                for (RValue *memberNameRVal : refVector)
                {
                    Organik::GetLogger()->LogFormatted("%d: %s", id, memberNameRVal->ToCString());
                    if (memberNameRVal->m_Kind == VALUE_STRING)
                    {
                        memberNames.push_back(memberNameRVal->ToString());
                    }
                }
                //varMapCache.insert_or_assign(id, varmap);
                for (const std::string& name : memberNames)
                {
                    int32_t slot = Code_Variable_FindAlloc_Slot_From_Name(GetGlobalInstance(), const_cast<char*>(name.c_str()));
                    if (!slot || slot < 0)
                    {
                        continue;
                    }
	                Organik::GetLogger()->LogFormatted("%s:%d --- %s %s", __FILE__, __LINE__, name.c_str(), __FUNCTION__);
                    RValue* member = instance->InternalReadYYVar(slot);
                    if (!member || (member->m_Kind == VALUE_NULL || 
                        member->m_Kind == VALUE_UNDEFINED || 
                        member->m_Kind == VALUE_UNSET))
                    {
                        // If member is null, skip it
                        continue;
                    }
                    memberMap.insert({name, member});
	                Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
                    varMapCache[id].insert({slot, name});
                }
            }
            else
            {
                Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
                GetLogger()->LogFormatted("Using cached varmap for instance ID %d", id);
                for (const auto& [slot, name] : varMapCache[id])
                {
	                Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
                    RValue* member = instance->InternalReadYYVar(slot);
                    if (!member || (member->m_Kind == VALUE_NULL || 
                        member->m_Kind == VALUE_UNDEFINED || 
                        member->m_Kind == VALUE_UNSET))
                    {
                        // If member is null, skip it
                        continue;
                    }
                    memberMap.insert({name, member});
                }
            }
            
>>>>>>> 073bc0d (std::launder for squeaky clean RValues)
            // Display all variables in a table
            if (ImGui::BeginTable("Variables", 3, 
                        ImGuiTableFlags_Borders | 
                        ImGuiTableFlags_RowBg | 
                        ImGuiTableFlags_Resizable |
                        ImGuiTableFlags_ScrollY))
            {
                // Setup columns
                ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 150.0f);
                ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 80.0f);
                ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableHeadersRow();
                    
                if ((!varMapCache.contains(id)) || (instance->GetMemberCount() != varMapCache[id].size()))
                {
                    varMapCache[id].clear();
                    instance->m_YYVarsMap->ForEach([&](const int32_t& slot, const CHashMapHash& hash, RValue* value) {
                        // Get variable name from slot
                        rowClicked = false;
                        std::string name = VARNAME_FROMHASH(slot);
                        if (name.empty())
                            return;
                        varMapCache[id].insert({slot, name});
                        DisplayVarTableEntry(
                            instance,
                            name,
                            value
                        );
                    });
                }
                else
                {
                    for(auto& [slot, name] : varMapCache[id])
                    {
                        rowClicked = false;
                        RValue* member = instance->InternalReadYYVar(slot);
                        if (!member || (member->m_Kind == VALUE_NULL || 
                            member->m_Kind == VALUE_UNDEFINED || 
                            member->m_Kind == VALUE_UNSET ||
                            member->m_Kind >= VALUE_PARAMS))
                        {
                            // If member is null, skip it
                            continue;
                        }
<<<<<<< HEAD
                        DisplayVarTableEntry(
                            instance,
                            name,
                            member
                        );
=======
                        ImGui::SameLine(0,0);
                        ImGui::Text("%s", memberName.c_str());
                        
                        // Variable type
                        ImGui::TableSetColumnIndex(1);
                        std::string typeName = memberValue->GetKindName();
                        ImGui::Text("%s", typeName.c_str());
                        
                        // Variable value
                        ImGui::TableSetColumnIndex(2);
                        
                        DisplayVariableValue(memberName, memberValue);
                        // Handle row click - open edit popup
                        if (rowClicked && !showPopup)
                        {
                            editingVariable = memberName;
                            editingValue = memberValue;
                            editingInstanceId = instance->m_ID;
                            
                            if (memberValue->m_Kind != VALUE_ARRAY)
                            {
                                editingInstanceId = instance->m_ID;
                                editingValue = memberValue;
                                editingVariable = memberName;
                                showPopup = true;
                                PrepareEditValue(memberValue);
                            }
                        }
	    Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
                        if (rowClicked && showPopup)
                        {
                            ImGui::OpenPopup("VarEdit");
                        }
                        DisplayEditPopup();
                        ImGui::PopID();
>>>>>>> 073bc0d (std::launder for squeaky clean RValues)
                    }
                }
            }
            ImGui::EndTable();
        }
        ImGui::EndChild();
    }
    else
    {
        ImGui::TextDisabled("Select an instance to view its variables");
    }
    Organik::GetLogger()->LogFormatted("%s: %s -- %d", __FILE__, __FUNCTION__, __LINE__);
}

void InstanceVariableViewer::PrepareEditValue(RValue* value)
{
    if (!value) return;
    RValueType kind = (RValueType) value->m_Kind;
    
    if (kind == VALUE_REAL)
    {
        editDoubleValue = parseRValueNumber<double>(value);
    }
    else if (kind == VALUE_INT32)
    {
        editIntValue = parseRValueNumber<int32_t>(value);
    }
    else if (kind == VALUE_INT64)
    {
        editLongLongValue = parseRValueNumber<int64_t>(value);
    }
    else if (kind == VALUE_STRING)
    {
        editStringValue = value->ToString();
    }
    else if (kind == VALUE_BOOL)
    {
        editBoolValue = value->ToBoolean();
    }
    else
    {
        editStringValue = value->ToString();
        Organik::GetLogger()->LogFormatted("%s:%d - unsupported type, name=%s", __FUNCTION__, __LINE__, KIND_NAME_RValue(value));
    }
}

void InstanceVariableViewer::DisplayEditPopup()
{
    if (ImGui::BeginPopup("VarEdit", ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup ))
    {
        if (!editingValue)
        {
            ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
            showPopup = false;
            return;
        }
        if (!(editingVariable.size()))
            ImGui::Text("Editing: %s", editingVariable.c_str());
        
        ImGui::Text("Type: %s", editingValue->GetKindName());
        ImGui::Separator();
        
        RValueType kind = (RValueType)editingValue->m_Kind;
        bool valueChanged = false;
        
        if (kind == VALUE_REAL || kind == VALUE_INT32 || kind == VALUE_INT64)
        {
            if (kind == VALUE_INT32)
            {
                if (ImGui::InputInt("Value", &editIntValue, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    *editingValue = RValue(static_cast<int32_t>(editIntValue));
                    valueChanged = true;
                    ImGui::CloseCurrentPopup();
                }
            }
            else if (kind == VALUE_INT64)
            {
                ImS64 one = (ImS64)1;
                if (ImGui::InputScalar("Value", ImGuiDataType_S64, &editLongLongValue, &one, NULL, "%lld", ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    *editingValue = RValue(static_cast<int64_t>(editLongLongValue));
                    valueChanged = true;
                    ImGui::CloseCurrentPopup();
                }
            }
            else
            {
                if (ImGui::InputDouble("Value", &editDoubleValue, 0.1, 10.0, "%.6f", ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    *editingValue = RValue(static_cast<double>(editDoubleValue));
                    valueChanged = true;
                    ImGui::CloseCurrentPopup();
                }
            }
	    Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
        }
        else if (kind == VALUE_STRING)
        {
            char buffer[1024];
            strncpy_s(buffer, editStringValue.c_str(), sizeof(buffer) - 1);
            buffer[sizeof(buffer) - 1] = '\0';
            
            if (ImGui::InputText("Value", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                RValue newStr = RValue(0);
                YYCreateString(&newStr, buffer);
                *editingValue =  newStr;
                valueChanged = true;
                    ImGui::CloseCurrentPopup();
            }
        }
        else if (kind == VALUE_BOOL)
        {
            if (ImGui::Checkbox("Value", &editBoolValue))
            {
                *editingValue = editBoolValue ? RValue(true): RValue(false);
                valueChanged = true;
                ImGui::CloseCurrentPopup();
            }
        }
        else
        {
            ImGui::TextDisabled("This type cannot be edited");
        }
        
        ImGui::Separator();
        
        ImGui::Text("Press Enter to apply changes, or Cancel to discard.");
        
        ImGui::SameLine();
        
        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
        }
        
        ImGui::EndPopup();
    }
    else 
    {
        showPopup = false;
    }
}
void InstanceVariableViewer::DisplayArrayElement(size_t index, RValue* element, std::string name)
{
    if (!(element->m_Kind == VALUE_ARRAY))
    {
        DisplayVariableValue(("Array[" + std::to_string(index) + "]"), element);
    }
    else
    {
        // Nested array - show size and allow drilling down
        int arrayLength = element->GetArrayLength();
        std::vector<RValue*> arrayValues = element->ToRefVector();
        if (!arrayLength) 
        {
            ImGui::Text("Array[%zu] (empty)", arrayLength);
            return;
        }
        if (ImGui::TreeNode(element, "Array[%zu]", arrayLength))
        {
            // Create nested table for sub-array
            if (ImGui::BeginTable("NestedArray", 3, 
                                 ImGuiTableFlags_Borders | 
                                 ImGuiTableFlags_RowBg))
            {
                ImGui::TableSetupColumn("Index", ImGuiTableColumnFlags_WidthFixed, 60.0f);
                ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 80.0f);
                ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableHeadersRow();
                int j = 0;
                for (RValue *rV : arrayValues)
                {
                    DisplayArrayElement(j, rV);
                    j++;
                }
                ImGui::EndTable();
            }
            ImGui::TreePop();
        }
    }
}

void InstanceVariableViewer::DisplayVariableValue(const std::string& name, RValue* value)
{
    if (!value) {
        return;
    }
    bool rowClicked = false;
    ImGui::PushID(value);
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    if (ImGui::Selectable(("##row_" + name).c_str(), (editingValue == value), 
        ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap | ImGuiSelectableFlags_Highlight))
    {
        if (!(value->m_Kind == VALUE_ARRAY))
        {
            rowClicked = true;
        }
    }
    
    ImGui::Text(name.c_str());
    ImGui::TableSetColumnIndex(1);
    ImGui::Text(value->GetKindName());
    ImGui::TableSetColumnIndex(2);

    uint32_t kind = value->GetKind();
    
    if (kind == VALUE_REAL)
    {
        double realVal = parseRValueNumber<double>(value);
        ImGui::Text("%.6f", realVal);
    }
    else if (kind == VALUE_INT32)
    {
        int32_t intVal = parseRValueNumber<int32_t>(value);
        ImGui::Text("%d", intVal);
    }
    else if (kind == VALUE_INT64)
    {
        int64_t longlongval = parseRValueNumber<int64_t>(value);
        ImGui::Text("%lld", longlongval);
    }
    else if (kind == VALUE_STRING)
    {
        std::string strVal = value->ToString();
        ImGui::Text("\"%s\"", strVal.c_str());
    }
    else if (kind == VALUE_BOOL)
    {
        bool boolVal = value->ToBoolean();
        ImGui::Text("%s", boolVal ? "true" : "false");
    }
    else if (kind == VALUE_ARRAY)
    {
        DisplayArrayElement(0, value, name);
    }
    else if (kind == VALUE_PTR)
    {
        void* ptr = value->ToPointer();
        ImGui::Text("0x%p", ptr);
    }
    else if (kind == VALUE_OBJECT)
    {
        YYObjectBase* obj = value->ToObject();
        ImGui::Text("Object: 0x%p", obj);
    }
    else if (kind == VALUE_UNDEFINED || kind == VALUE_UNSET)
    {
        ImGui::TextDisabled("undefined");
    }
    else if (kind == VALUE_NULL)
    {
        ImGui::TextDisabled("null");
    }
    else
    {
        int64_t intVal = parseRValueNumber<int64_t>(value);
        std::string kindName = value->GetKindName();
        ImGui::Text("%lld (%s)", intVal, kindName.c_str());
    }
    if (rowClicked)
    {
        editingVariable = name;
        editingValue = value;
        showPopup = true;
        PrepareEditValue(value);
        ImGui::OpenPopup("VarEdit");
    }
    ImGui::PopID();
}

// Demonstrate creating a simple property editor.
void InstanceVariableViewer::Draw(bool& out_mousedOver, bool* p_open, const std::string &title)
{
    ImGui::SetNextWindowSize(ImVec2(430, 750), ImGuiCond_FirstUseEver);
	    // Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
    if (!ImGui::Begin("Organik Variable Viewer", p_open))
    {
        ImGui::End();
        return;
    }

    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_ChildWindows))
        out_mousedOver = true;

    static InstanceVariableViewer variable_viewer;

    variable_viewer.DrawInner();
    ImGui::End();
}