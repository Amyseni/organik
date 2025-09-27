#include "Synthetik.h"
#include "zhl_internal.h"
#include "zhl.h"
#include "DefinitionHelpers/InstanceHelper.h"
#include "VariableHelper.h"
#include "VariableViewer.h"
#include "ItemFilter.h"
#include "Globals.h"
#include "Events.h"
#include "Action.h"
#include "UIManager.h"
#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_internal.h"
#include "Logging.h"
#include "imgui/imgui_impl_dx11.h"

struct ActionVar;
struct ActionNodeLink;
struct ActionNode;
struct ActionLink;
struct ImNodesEditorContext;

bool InstanceVariableViewer::showPopup = false;
bool InstanceVariableViewer::showArrayPopup = false;
bool matchAllVariables = false; // false = ay, true = all
#define OBJ_TREE_INDENT 18.0f
std::map<int32_t, bool> variablesToMatchMap = std::map<int32_t, bool>();
std::map<int32_t, std::unordered_map<int32_t, const char*>> varMapCache;
std::string objectHasVariableFilter;
std::string variableNameFitler;
std::string variableTypeFilter;
#define VARHASH_FROMNAME(name) Organik::Variables::Hashes[Organik::Variables::##name]
#define VARNAME_FROMHASH(hash) Organik::Variables::HashToVariableMap[hash]


static auto instanceFilter = ItemFilter();

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
    make(obj_player) \


#define PARENTS_DO(make) \
    make(obj_weapon_PARENT) \
    make(obj_enm_parent_all) \
    make(obj_all_solid) \
    make(obj_mainmenu) \
    make(obj_cursor) \
    make(obj_cursor_mainmenu) \
    make(obj_perk_parent) \
    make(obj_shell_parentOLD) \
    make(obj_lightemitter_parent) \
    make(obj_buffs_debuffs) \
    make(obj_moving_entity_PARENT) \
    make(obj_headshot_collision_parent) \
    make(obj_all_bullets) \
    make(obj_all_solid) \
    make(obj_trap_parent) \
    make(obj_trap_mine_parent) \
    make(obj_trap_mine_stealthparent) \
    make(obj_props_notsolid_parent) \
    make(obj_pickup_parent) \
    make(obj_questcontrol_parent) \
    make(obj_item_parent) \
    make(obj_moving_entity_PARENT) \
    make(obj_effect_parent) \
    make(obj_enm_budget_parent) \
    make(obj_chunk_parent) \
    make(obj_networking_parent) \
    make(obj_chat_parent) \
    make(obj_patchpage_parent) \
    make(obj_bullet_mortar_parent) \
    make(obj_arena_parent) \
    make(obj_research_button_PARENT) \
    make(T_PARENT_ALL_BONI) \
    make(obj_chip_parent) \
    make(obj_turret_boss_spawn_parent) \
    make(obj_followunit_parent) \
    make(obj_voidzone_parent) \
    make(obj_mortarshot_parent) \
    make(obj_ground_text_parent) \
    make(obj_door_parent) \
    make(obj_concback_parent) \
    make(obj_mm_menu_parent) \
    make(obj_mm_acc_input_parent) \
    make(obj_railing_parent) \
    make(obj_floor_blockout_parent) \
    make(obj_field_parent) \
    make(obj_fields_parent) \
    
#define OBJINDEX(name) Organik::Objects::ObjIndexes[Organik::Objects::##name]
#define TMAPOBJ(name) {Object_Data(OBJINDEX(name)), true},
#define FMAPOBJ(name) {Object_Data(OBJINDEX(name)), false},
std::unordered_map<CObjectGM*, bool> *GetParentObjects()
{
    static std::unordered_map<CObjectGM*, bool> parentObjects;
    if (parentObjects.size() == 0)
    {
        parentObjects = {
            PARENTS_DO(FMAPOBJ)
        };
    }
    return &parentObjects;
}
std::unordered_map<CObjectGM*, bool> *GetGlobalObjects()
{
    static std::unordered_map<CObjectGM*, bool> globalObjects;
    if (globalObjects.size() == 0)
    {
        globalObjects = {
            GLOBALS_DO(TMAPOBJ)
        };
    }
    return &globalObjects;
}
std::unordered_map<CObjectGM*, bool> *GetTrackedObjects()
{
    static std::unordered_map<CObjectGM*, bool> trackedObjects;
    if (!trackedObjects.size())
    {
        trackedObjects.merge(*GetGlobalObjects());
        trackedObjects.merge(*GetParentObjects());
    }
    return &trackedObjects;
}
void recurse(CObjectGM* obj, std::function<void(CObjectGM*)> pred) {
    if (obj)
        pred(obj);
    if (obj->m_ChildrenMap && obj->m_ChildrenMap->m_UsedCount)
    {
        obj->m_ChildrenMap->ForEach([&](const int32_t& key, const CHashMapHash& hash, CObjectGM* elem) {
            recurse(elem, pred);
        });
    }
}
auto GetObjectsForTabs() -> std::unordered_map<CObjectGM*, bool>*
{
    static std::unordered_map<CObjectGM*, bool> objectsForTabs;
    if (!objectsForTabs.size()) objectsForTabs = {
        GLOBALS_DO(FMAPOBJ)
        PARENTS_DO(FMAPOBJ)
    };
    return &objectsForTabs;
}



std::unordered_map<CObjectGM*, bool> *GetTreeNodeOpen()
{
    static std::unordered_map<CObjectGM*, bool> treeNodeOpen;
    if (!treeNodeOpen.size())
    {
        treeNodeOpen = {
            GLOBALS_DO(FMAPOBJ)
            PARENTS_DO(FMAPOBJ)
        };
    }
    return &treeNodeOpen;
}

typedef void (*objectPred)(CObjectGM*, std::function<void(CObjectGM*)>);
static std::string objNameFilterBuf;
void InstanceVariableViewer::displayFunc(CObjectGM* child, bool& tracked){
    if (!child) return;
    ImGui::PushID(child);
    if (ImGui::TreeNodeEx((const void*)(child), 
        ((child->m_ChildrenMap && child->m_ChildrenMap->m_UsedCount)
            ? ImGuiTreeNodeFlags_None : ImGuiTreeNodeFlags_Leaf)
        | ImGuiTreeNodeFlags_SpanAllColumns | ImGuiTreeNodeFlags_DrawLinesFull
        | (tracked ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None)
        | ImGuiTreeNodeFlags_AllowItemOverlap
        | ImGuiTreeNodeFlags_NavLeftJumpsToParent,
        "%s (#%d, %d children)", child->m_Name ? child->m_Name : "<no_name>", 
            child->m_ID, 
            child->m_ChildrenMap ? child->m_ChildrenMap->m_UsedCount : 0))
    {
        if (child->m_ChildrenMap && child->m_ChildrenMap->m_UsedCount)
        {
            child->m_ChildrenMap->ForEach([&](int key, CHashMapHash hash, CObjectGM* obj) {
                if (!obj || (!hash) || (hash & HASH_DELETED) || (obj->m_ID < 0)) {
                    return;
                };
                if (obj->m_ChildrenMap && obj->m_ChildrenMap->m_UsedCount)
                    displayFunc(obj, ((*GetTrackedObjects())[obj]));
            });
        }
        ImGui::TreePop();
    }
    
    if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
        { ImGui::OpenPopup("ObjectContextMenu"); }
    if (ImGui::BeginPopup("ObjectContextMenu", ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings 
        | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_Popup | ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::MenuItem("Show Object Details", "", &(*GetObjectsForTabs())[child], true);
        ImGui::MenuItem("Track Instances (this)", "", &tracked, true);
        if (ImGui::MenuItem("Track Instances (all children)", "", &tracked, true))
        {
            recurse(child, [tracked](CObjectGM* obj) {
                if (obj)
                    (*GetTrackedObjects())[obj] = tracked;
            });
        }
        ImGui::EndPopup();
    }
    ImGui::PopID();
}
void InstanceVariableViewer::displayFuncNoButton(CObjectGM* child, std::function<void(SLinkedList<CInstance>)> leafFunc){
    if (!child) return;
    if (!child->m_ChildrenMap || !(child->m_ChildrenMap->m_UsedCount))
    {
        leafFunc(child->m_Instances);
        return;
    }
    if (ImGui::TreeNodeEx((const void*)(child), 
        ((child->m_ChildrenMap && child->m_ChildrenMap->m_UsedCount)
            ? ImGuiTreeNodeFlags_None : ImGuiTreeNodeFlags_Leaf)
        | ImGuiTreeNodeFlags_DrawLinesFull 
        | ImGuiTreeNodeFlags_NavLeftJumpsToParent,
        "%s (#%d, %d children)", child->m_Name ? child->m_Name : "<no_name>", 
            child->m_ID, 
            child->m_ChildrenMap ? child->m_ChildrenMap->m_UsedCount : 0))
    {
        
        child->m_ChildrenMap->ForEach([&](int key, CHashMapHash hash, CObjectGM* obj) {
            if (!obj || (!hash) || (hash & HASH_DELETED) || (obj->m_ID < 0)) {
                return;
            }
            displayFuncNoButton(obj, leafFunc);
        });
        ImGui::TreePop();
    }
}
void InstanceVariableViewer::DrawObjectParentsFilterList()
{
    static ImGuiTextFilter objNameFilter;
    static bool filterObjects = false;
    std::vector<int32_t> toErase;
    auto trackedObjects = GetTrackedObjects();
    for(auto &[obj, show] : (*trackedObjects))
    {
        auto tmpObj = obj->m_ParentObject;
        auto noParentsDisplayed = [&]() -> auto {
            while (tmpObj)
            {
                if (trackedObjects->contains(tmpObj))
                    return false;
                if (!tmpObj->m_ParentObject || tmpObj->m_ParentObject == tmpObj)
                    break;
                tmpObj = tmpObj->m_ParentObject;
            }
            return tmpObj != obj && !(trackedObjects->contains(tmpObj));
        }();
        
        if (noParentsDisplayed)
            displayFunc(obj, show); // only show root parents here.
    }
}
bool FakeEvent(CInstance* self, CInstance* other, YYObjectBase* context)
{
    static RValue variantpredestined;

    if (!(variantpredestined.GetKind() == VALUE_REAL))
        variantpredestined = RValue(20.00);
    (*self->InternalGetYYVarRef(VAR_HASH(variantpredestined))) = variantpredestined;

    if (context)
    {
        RValue *from = context->InternalGetYYVarRef(VAR_HASH(from));
        YYCreateString(from, "obj_stats.mod_luck");
        StructCreate(context->InternalGetYYVarRef(VAR_HASH(scale)));
        MessageBoxA(NULL, from->ToCString(), "Test", MB_OK);
        bool ret = doGetFrom(self, other, context);
        if (ret)
            Organik::GetLogger()->LogFormatted("FakeEvent: doGetFrom succeeded, result: %.f", parseRValueNumber<double>(context->InternalGetYYVarRef(VAR_HASH(result))));
        else
            Error_Show_Action("FakeEvent: doGetFrom failed", true, true);
        return ret;
    }
    return false;
}

void InstanceVariableViewer::CObjectDetailsPane(CObjectGM* toggleObj)
{
    static CObjectGM* toAdd = nullptr;
    static Action *toAddEvent = nullptr;
    
    static struct {
        struct Tab {
            YYObjectBase*(*getParams)() = []() -> YYObjectBase* {
                static RValue params;
                if (!(params.GetKind() == VALUE_OBJECT)) {
                    StructCreate(&params);
                    RValue* variables = params.ToObject()->InternalGetYYVarRef(VAR_HASH(variables));
                    RValue* parameters = params.ToObject()->InternalGetYYVarRef(VAR_HASH(parameters));
                    StructCreate(variables);
                    StructCreate(parameters);
                    *(variables->ToObject()->InternalGetYYVarRef(VAR_HASH(debug))) = RValue("Hello from Direktor!");
                    *(parameters->ToObject()->InternalGetYYVarRef(VAR_HASH(self))) = RValue(-2);
                    *(parameters->ToObject()->InternalGetYYVarRef(VAR_HASH(other))) = RValue(-2);
                }
                return params.ToObject();
            };
            Tab() : m_Object(nullptr), m_ObjectID(-1), m_Name("<no_name>"), m_ParentID(-1), m_ParentName("<no_parent>") {};
            Tab(CObjectGM* obj) : m_Object(obj), m_ObjectID(obj ? obj->m_ID : 7), m_Name(obj ? obj->m_Name : "<no_name>") {
                m_ParentID = (obj && obj->m_ParentObject && obj->m_ParentObject != obj) ? obj->m_ParentObject->m_ID : -1;
                m_ParentName = (obj && obj->m_ParentObject && obj->m_ParentObject != obj) ? (obj->m_ParentObject->m_Name ? obj->m_ParentObject->m_Name : "<no_name>") : "<no_parent>";
            };
            Tab(int32_t id) : m_ObjectID(id), m_Name("<no_name>"), m_ParentID(-1), m_ParentName("<no_parent>") {
                m_Object = Object_Data(id);
                if (m_Object) {
                    m_Name = m_Object->m_Name ? m_Object->m_Name : "<no_name>";
                    m_ParentID = (m_Object->m_ParentObject && m_Object->m_ParentObject != m_Object) ? m_Object->m_ParentObject->m_ID : -1;
                    m_ParentName = (m_Object->m_ParentObject && m_Object->m_ParentObject != m_Object) ? (m_Object->m_ParentObject->m_Name ? m_Object->m_ParentObject->m_Name : "<no_name>") : "<no_parent>";
                }
            };
            CObjectGM* m_Object;
            int32_t m_ObjectID;
            const char* m_Name;
            int32_t m_ParentID;
            const char* m_ParentName;
            //map event main code (0-14) to list of events
            std::unordered_map<int32_t, std::vector<MapDisplayEntry<int64_t, CEvent, 3>*>> m_Events;
            std::vector<CObjectGM*> m_ChildObjects;
            inline void CacheEvents() {
                m_Events.clear();
                if (m_ObjectID < 0) return;
                if (!m_Object)
                    m_Object = Object_Data(m_ObjectID);
                if (!m_Object) return;

                m_Object->m_EventsMap->ForEachWithIndex([&](const int32_t& index, const int64_t& key, const CHashMapHash& hash, CEvent* value) -> void
                {
                    if (value == nullptr 
                            || (hash & HASH_DELETED))
                        return;
                    if (!m_Events.contains(key >> 32)) // main event code
                        m_Events[key >> 32] = std::vector<MapDisplayEntry<int64_t, CEvent, 3>*>();
                    m_Events[key >> 32].push_back(new MapDisplayEntry<int64_t, CEvent, 3>(
                        value ? (value->m_Code->m_Name ? value->m_Code->m_Name : "<no_name>") : "<no_name>",
                        (key & 0xFFFFFFFF) | ((key & 0xFFFF0000) >> 16),
                        index,
                        m_Object->m_EventsMap
                    ));
                });
            }
            inline CObjectGM* GetObject()  {
                return m_Object = Object_Data(m_ObjectID);
            }

            inline ~Tab() {
                for(auto &entry : m_Events)
                    for(auto &entry2 : entry.second)
                        delete entry2;
                m_Events.clear();
                m_ChildObjects.clear();
            };

            inline void ImGuiDoTableRowChar (const char* label, const char* value) {
                ImGui::TableNextRow(ImGuiTableRowFlags_None, 10.0f);
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%s", label);
                ImGui::TableNextColumn();
                ImGui::Text("%s", value);
            }
            inline void ImGuiDoTableRowNum (const char* label, double value) {
                ImGui::TableNextRow(ImGuiTableRowFlags_None, 10.0f);
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%s", label);
                ImGui::TableNextColumn();
                ImGui::Text("%.5f", value);
            }
            inline void DisplayMainTable()
            {
                if (ImGui::BeginTable("object_details_table##tab", 2, ImGuiTableFlags_Borders))
                {
                    ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed, 80.0f);
                    ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
                    ImGui::TableHeadersRow();

                    ImGuiDoTableRowChar("Name", m_Object->m_Name ? m_Object->m_Name : "<no_name>");
                    ImGuiDoTableRowNum("ID", m_Object->m_ID);
                    ImGuiDoTableRowNum("Instances (direct)", m_Object->m_Instances.m_Count);
                    ImGui::SetItemTooltip("Includes instances of this object only, not children.");
                    ImGuiDoTableRowNum("Instances (all)", m_Object->m_InstancesRecursive.m_Count);
                    ImGuiDoTableRowChar("Parent", m_ParentName ? m_ParentName : "<no_parent>");
                    ImGuiDoTableRowNum("Parent ID", m_ParentID);
                    ImGuiDoTableRowNum("Depth", m_Object->m_Depth);
                    ImGuiDoTableRowNum("Children", m_Object->m_ChildrenMap ? m_Object->m_ChildrenMap->m_UsedCount : 0);
                    ImGuiDoTableRowNum("Events", m_Events.size());
                    ImGui::EndTable();
                }
            }
            inline void DisplayEventsTable()
            {
                if (!m_Object->m_EventsMap || (m_Object->m_EventsMap->m_UsedCount < 1))
                {
                    ImGui::TextDisabled("No events found.");
                    return;
                }
                if (ImGui::BeginTable("##events_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_HighlightHoveredColumn))
                {
                    //ImGui::TableSetupColumn("##button", ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_NoHeaderLabel, 10.0f);
                    ImGui::TableSetupColumn("Code", ImGuiTableColumnFlags_WidthFixed, 15.0f);
                    ImGui::TableSetupColumn("Owner", ImGuiTableColumnFlags_WidthFixed, 25.0f);
                    ImGui::TableSetupColumn("Details", ImGuiTableColumnFlags_WidthStretch, 160.0f);
                    ImGui::TableHeadersRow();

                    GetObject()->m_EventsMap->ForEach([&](const int64_t& key, const CHashMapHash& hash, CEvent*& value) -> void
                    {
                        ImGui::PushID(value);
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        if (ImGui::Selectable("##eventrow##", false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap))                    
                            ImGui::OpenPopup("##event_details", ImGuiPopupFlags_MouseButtonLeft);
                        ImGui::TableSetColumnIndex(0);
                        int32_t code = key >> 32;
                        int32_t subcode = key & 0xFFFFFFFF;

                        ImGui::Text("%s", eventSpecificName(code, subcode).c_str());
                        ImGui::TableNextColumn();

                        ImGui::Text("%s", Object_Data(value->m_OwnerObjectID)->m_Name);
                        ImGui::TableNextColumn();

                        ImGui::SameLine(0,0);
                        ImGui::Text("%p -- %s", value, 
                            (value->m_Code && value->m_Code->m_Name)
                                ? value->m_Code->m_Name
                                : ((value->m_Code->m_Functions && value->m_Code->m_Functions->m_Name)
                                    ? value->m_Code->m_Functions->m_Name 
                                    : "<no_name>"
                                )
                            );
                        if (ImGui::BeginPopup("event_details", ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings 
                            | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_Popup | ImGuiWindowFlags_AlwaysAutoResize))
                        {
                            ImGui::TextDisabled("Event Details");
                            ImGui::Separator();
                            ImGui::Text("Owner Object: %d", Object_Data(value->m_OwnerObjectID) 
                                    ? (Object_Data(value->m_OwnerObjectID)->m_Name 
                                    ? Object_Data(value->m_OwnerObjectID)->m_Name : "<no_name>") : "<no_object>");
                            ImGui::Text("Event Code: (%d, %d)", code, subcode);
                            ImGui::Text("Event Name: %s", eventSpecificName(code, subcode).c_str());

                            if (value->m_Code)
                            {
                                ImGui::Text("Name: %s", value->m_Code->m_Name ? value->m_Code->m_Name : "<no_name>");
                                ImGui::Text("Functions: %p", value->m_Code->m_Functions);
                                if (value->m_Code->m_Functions)
                                {
                                    ImGui::Text("Function Name: %s", value->m_Code->m_Functions->m_Name ? value->m_Code->m_Functions->m_Name : "<no_name>");
                                    ImGui::Text("Local Variables: %d", value->m_Code->m_LocalsCount);
                                }
                            }

                            ImGui::EndPopup();
                        }
                        ImGui::PopID();
                    });
                    ImGui::EndTable();
                }
            }
            inline void Display()
            {
                if (!(*Run_Room))
                {
                    ImGui::TextDisabled("Room transition likely running");
                    return;
                }
                
                if ((*Run_Room) == Room_Data(ROOM_ID(roomtransistion))
                    || (*Run_Room) == Room_Data(ROOM_ID(Load)))
                {
                    ImGui::TextDisabled("Room transition likely running");
                    return;
                }

                if (!GetObject())
                    return;

                if (!ImGui::BeginTabItem(m_Name ? m_Name : "<no_name>##tab")) return;

                DisplayMainTable();
                
                ImGui::Separator();
                    
                DisplayEventsTable();
                
                ImGui::EndTabItem();
            }
        };
        std::unordered_map<int32_t, Tab> tabs;
        void ToggleTab(CObjectGM* obj) {
            if (tabs.contains(obj->m_ID)) {
                tabs.erase(obj->m_ID);
                (*GetObjectsForTabs())[obj] = false;
                return;
            }
            (*GetObjectsForTabs())[obj] = true;
            tabs[obj->m_ID] = Tab(obj);
        }
    } objectDetailsTabs;    



    for (auto& [obj, show] : (*GetObjectsForTabs())) {
        if (!show) continue;
        if (!objectDetailsTabs.tabs.contains(obj->m_ID))
            objectDetailsTabs.ToggleTab(obj);

        objectDetailsTabs.tabs[obj->m_ID].Display();
    }
    if (toAdd)
    {
        objectDetailsTabs.ToggleTab(toAdd);
        toAdd = nullptr;
    }
    
}
void InstanceVariableViewer::RecurseObjectInstanceTree(CObjectGM* obj)
{
    if (!obj) return;
    if (!obj->m_InstancesRecursive.m_Count)
        return;
    if (!(*GetTrackedObjects()).contains(obj))
        return;
    ImGuiTreeNodeFlags child_obj_flags = 0
                        | ImGuiTreeNodeFlags_DrawLinesFull 
                        | ImGuiTreeNodeFlags_NavLeftJumpsBackHere;
    
    displayFuncNoButton(obj, [&](SLinkedList<CInstance> instances) -> void {
        if (!instances.m_Count) return;
        if (
            ImGui::TreeNodeEx(
                (const void*)(instances.m_First->m_Object), 
                child_obj_flags, 
                "%s (%d)", instances.m_First->m_Object->m_Object->m_Name ? instances.m_First->m_Object->m_Object->m_Name : "<no_name>", 
                instances.m_First->m_Object->m_YYVarsMap->m_UsedCount
            ))
        {
            auto _node = instances.m_First;
            while (_node && (_node->m_Next || _node == instances.m_Last) && _node->m_Object)
            {
                CInstance* inst = reinterpret_cast<CInstance*>(
                    (_node ? ( _node->m_Object ? _node->m_Object : nullptr) : nullptr)
                );
                
                if (!inst) break;
                int32_t ID = inst->m_ID;
                if (ImGui::TreeNodeEx((const void*)(inst),
                    child_obj_flags
                    | ((VisibleInstance && VisibleInstance == inst) ? ImGuiTreeNodeFlags_Selected : 0)
                    | ImGuiTreeNodeFlags_Leaf,
                    "#%d (%d vars)", ID,inst->m_YYVarsMap->m_UsedCount
                )) { ImGui::TreePop(); };
                
                if (ImGui::IsItemClicked())
                {
                    VisibleInstance = inst;
                }

                if (!_node->m_Next || (_node == instances.m_Last))
                    break;
                _node = _node->m_Next;
            }
            ImGui::TreePop();
        }
    });
}
void InstanceVariableViewer::DrawObjectList()
{
    auto trackedObjects = GetTrackedObjects();
    ImGuiTreeNodeFlags child_obj_flags = 0
                        | ImGuiTreeNodeFlags_DrawLinesFull 
                        | ImGuiTreeNodeFlags_NavLeftJumpsBackHere;
    auto globInst = GetGlobalInstance();
    int32_t mID = globInst->m_ID;
    if (ImGui::TreeNodeEx((const void*)(globInst),
        child_obj_flags
        | ((VisibleInstance && VisibleInstance == globInst) ? ImGuiTreeNodeFlags_Selected : 0)
        | ImGuiTreeNodeFlags_Leaf,
        "#%d - global (%d vars)", mID,globInst->m_YYVarsMap->m_UsedCount
    )) {  };
    
    if (ImGui::IsItemClicked())
    {
        VisibleInstance = globInst;
    }
    ImGui::TreePop();
    for(auto &[obj, show] : (*trackedObjects))
    {
        if (obj->m_ParentObject && obj->m_ParentObject != obj)
        {
            auto tmpObj = obj->m_ParentObject;
            auto noParentsDisplayed = [&]() -> auto {
                while (tmpObj)
                {
                    if (trackedObjects->contains(tmpObj))
                        return false;
                    if (!tmpObj->m_ParentObject || tmpObj->m_ParentObject == tmpObj)
                        break;
                    tmpObj = tmpObj->m_ParentObject;
                }
                return tmpObj != obj && !(trackedObjects->contains(tmpObj));
            }();
        
            if (!noParentsDisplayed)
                continue; // only show root parents here.
        }
        RecurseObjectInstanceTree(obj);
    }
}

std::unordered_map<int32_t, std::unordered_map<int32_t, std::pair<int32_t, const char*>>> *GetMapCache()
{
    static std::unordered_map<int32_t, std::unordered_map<int32_t, std::pair<int32_t, const char*>>> mapCache;
    return &mapCache;
}

constexpr const int regexConsts = std::regex_constants::match_continuous | std::regex_constants::icase | std::regex_constants::optimize;
void InstanceVariableViewer::DrawInner()
{
    static std::regex pattern("", regexConsts);
    CInstance* instance;

    struct {
        std::unordered_map<int32_t, std::pair<int32_t, const char*>> varMap;
        bool cached = false;
    } static sL_InstanceMapCache;
    if (instance = VisibleInstance)
    {
        // basically both of these checks should be unreachable code, but you know how I get down.
	    if (!instance)
            return;
        int id = instance->m_ID;

        if (!instance->m_YYVarsMap)
        {
            ImGui::Text("Instance has no variable map.");    
            return;
        }
        if (instance->m_YYVarsMap->m_UsedCount == 0)
        {
            ImGui::Text("Instance has no variable map.");
            return;
        }
        int32_t objectIndex = instance->m_ObjectIndex;
        const char* objectName = instance->m_Object ? instance->m_Object->m_Name : "<no_name>";
        bool rowClicked = false;
        ImGui::Text("Variables for: %s (ID: %d)", 
                    objectName ? (objectName[0] ? objectName : "<no_name>") : "<no_name>", 
                    id);
        ImGui::SameLine(0,5.0);
        static std::string varNameFilterBuf;
        auto textEntered = ImGui::InputTextWithHint("##obj_name_filter", "Search Variable Name. (partial matching, regex allowed.)", &varNameFilterBuf, ImGuiInputTextFlags_EnterReturnsTrue);
        ImGui::SameLine(0,5.0);

        textEntered = (textEntered || ImGui::Button("Search"));
        if (textEntered)
        {
            if (!varNameFilterBuf.empty()){
                pattern.operator=(varNameFilterBuf);
            }
            else
            {
                pattern.operator=(std::regex(""));
            }
        }
        
        ImGui::Separator();

        // Display all variables in a table
        if (ImGui::BeginTable("Variables", 3, 
                    ImGuiTableFlags_SortMulti | 
                    ImGuiTableFlags_Borders | 
                    ImGuiTableFlags_RowBg | 
                    ImGuiTableFlags_Resizable |
                    ImGuiTableFlags_ScrollY))
        {
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_PreferSortAscending, 150.0f);
            ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_PreferSortAscending, 80.0f);
            ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableHeadersRow();
            
            if (!((*GetMapCache()).contains(id)))
            {
                (*GetMapCache())[id] = std::unordered_map<int32_t, std::pair<int32_t, const char*>>();
            }
            if (instance->m_YYVarsMap->m_UsedCount != (*GetMapCache())[id].size())
            {
                (*GetMapCache())[id].reserve(instance->m_YYVarsMap->m_UsedCount);
                Organik::GetLogger()->LogFormatted("%s: %s -- %d RECACHING VARMAP", __FILE__, __FUNCTION__, __LINE__);
                
                instance->m_YYVarsMap->ForEach([&](const int32_t& key, const CHashMapHash& hash, RValue*& value) -> void {
                    if (value == nullptr 
                        || (hash & HASH_DELETED)
                        || (value->m_Kind == VALUE_NULL 
                            || value->m_Kind == VALUE_UNDEFINED 
                            || value->m_Kind == VALUE_UNSET 
                            || value->m_Kind >= VALUE_PARAMS))
                    {
                        return;
                    }
                    int32_t varid = key;
                    
                    const char* varname = Organik::Variables::HashToVariableMap.contains(varid) ? Organik::Variables::HashToVariableMap[varid] : "<no_name>";
                    if (!varname[0])
                        varname = "<no_name>";
                    Organik::GetLogger()->LogFormatted("%s: %s -- %s (%d)", __FILE__, __FUNCTION__, varname, varid);
                    
                    DisplayVariableValue(varname, value);
                });
            }
           
            ImGui::EndTable();
        }
    }
    else
    {
        ImGui::TextDisabled("Select an instance to view its variables");
    }
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
void InstanceVariableViewer::DisplayArrayElement(size_t index, RValue* element, const char* name)
{
    const char* dispName = name ? name : "Array";
    if (!(element->m_Kind == VALUE_ARRAY))
    {
        DisplayVariableValue((GetLogger()->ParseFormatting("%s[%zu]", dispName, index)).c_str(), element);
    }
    else
    {
        // Nested array - show size and allow drilling down
        int arrayLength = element->GetArrayLength();
        std::vector<RValue*> arrayValues = element->ToRefVector();
        if (!arrayLength) 
        {
            ImGui::Text("%s[%zu] (empty)", dispName, index);
            return;
        }
        if (ImGui::TreeNode(element, "%s[%zu]", dispName, arrayLength))
        {
           
            int j = 0;
            for (RValue *rV : arrayValues)
            {
                DisplayArrayElement(j, rV);
                j++;
            }
            ImGui::TreePop();
        }
    }
}
void InstanceVariableViewer::DisplayVariableValue(MapDisplayEntry<int32_t, RValue, 3>* entry)
{
    DisplayVariableValue(entry->m_Name, entry->m_Value);
}

void InstanceVariableViewer::DisplayVariableValue(const char* name, RValue* value)
{
    if (!value) {
        return;
    }
    GetLogger()->LogSimple("Displaying variable value");
    const char* realName = (name && strnlen_s(name, 64)) ? name : "<no_name>";
    bool rowClicked = false;
    ImGui::TableNextRow();
    ImGui::PushID(value);
    ImGui::TableSetColumnIndex(0);
    if (ImGui::Selectable(realName, (editingValue == value),
        ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap))
    {
        if (!(value->m_Kind == VALUE_ARRAY))
        {
            rowClicked = true;
        }
    }

    ImGui::TableNextColumn();
    ImGui::Text(value->GetKindName());
    ImGui::TableNextColumn();
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
        ImGui::Text("\"%s\"", value->ToCString());
    }
    else if (kind == VALUE_BOOL)
    {
        bool boolVal = value->ToBoolean();
        ImGui::Text("%s", boolVal ? "true" : "false");
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
    }
    ImGui::PopID();
}

void InstanceVariableViewer::Draw(bool& out_mousedOver, bool* p_open, const std::string &title)
{
    ImGuiID dockspaceId = ImGui::GetID("VariableViewerDockspace");
    ImGui::SetNextWindowSize(ImVec2(1200, 750), ImGuiCond_FirstUseEver);
    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_AutoHideTabBar;
    
    static bool first_time = true;
    if (first_time)
    {
        first_time = false;
        ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspaceId, ImGui::GetWindowSize());

        ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Left, 0.4f, &dock_left_id, &dock_right_id);
        
        ImGui::DockBuilderSetNodeSize(dock_left_id, ImVec2(300, 750));
        ImGui::DockBuilderSetNodeSize(dock_right_id, ImVec2(900, 750));

        ImGui::DockBuilderSplitNode(dock_left_id, ImGuiDir_Up, 0.5f, &dock_left_top_id, &dock_left_bottom_id);
        ImGui::DockBuilderSplitNode(dock_right_id, ImGuiDir_Up, 0.5f, &dock_right_top_id, &dock_right_bottom_id);

        ImGui::DockBuilderDockWindow("Object Tree", dock_left_top_id);
        ImGui::DockBuilderDockWindow("Instance Tree", dock_left_bottom_id);
        ImGui::DockBuilderDockWindow("Object Details", dock_right_top_id);
        ImGui::DockBuilderDockWindow("Variables Table", dock_right_bottom_id);

        ImGui::DockBuilderFinish(dockspaceId);
    }
    
    if (!ImGui::Begin("Organik Variable Viewer", p_open, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NavFlattened | ImGuiWindowFlags_NoCollapse))
    {
        ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_KeepAliveOnly);
        ImGui::End();
        return;
    }
    
    ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspace_flags);


    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_ChildWindows))
        out_mousedOver = true;

    DrawObjectTreeWindow(&out_mousedOver);
    DrawInstanceTreeWindow(&out_mousedOver);
    DrawObjectDetailsWindow(&out_mousedOver);

    DrawVariablesPanel(&out_mousedOver);
    if (showPopup)
    {
        ImGui::OpenPopup("VarEdit");
        showPopup = false;
    }
    DisplayEditPopup();
    ImGui::End();
}

void InstanceVariableViewer::DrawObjectTreeWindow(bool* out_mousedOver)
{
    ImGui::SetNextWindowDockID(dock_left_top_id, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(200, 450), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Object Tree", nullptr, 
        ImGuiWindowFlags_NoScrollbar 
        | ImGuiWindowFlags_NavFlattened ))
    {
        DrawObjectParentsFilterList();
        if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_ChildWindows))
        {
            *out_mousedOver = true;
        }
    }
    ImGui::End();
}

void InstanceVariableViewer::DrawInstanceTreeWindow(bool* out_mousedOver)
{
    ImGui::SetNextWindowDockID(dock_left_bottom_id, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(200, 450), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Instance Tree", nullptr, ImGuiWindowFlags_NoScrollbar 
        | ImGuiWindowFlags_NavFlattened ))
    {
        DrawObjectList();
        if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_ChildWindows))
        {
            *out_mousedOver = true;
        }
    }
    ImGui::End();
}

void InstanceVariableViewer::DrawObjectDetailsWindow(bool* out_mousedOver)
{
    ImGui::SetNextWindowDockID(dock_right_top_id, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(350, 350), ImGuiCond_FirstUseEver);
    
    if (ImGui::Begin("Object Details", nullptr, ImGuiWindowFlags_NoScrollbar 
        | ImGuiWindowFlags_NavFlattened ))
    {
        if (ImGui::BeginTabBar("ObjectDetailsTabs", ImGuiTabBarFlags_Reorderable
            | ImGuiTabBarFlags_AutoSelectNewTabs
            | ImGuiTabBarFlags_FittingPolicyScroll))
        {
            CObjectDetailsPane(nullptr);
            ImGui::EndTabBar();
        }
        if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_ChildWindows))
        {
            *out_mousedOver = true;
        }
    }
    ImGui::End();
}

void InstanceVariableViewer::DrawVariablesPanel(bool* out_mousedOver)
{
    ImGui::SetNextWindowDockID(dock_right_bottom_id, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(350, 350), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Variables Table", nullptr, ImGuiWindowFlags_NoScrollbar 
        | ImGuiWindowFlags_NavFlattened  ))
    {
        DrawInner();
        if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_ChildWindows))
        {
            *out_mousedOver = true;
        }
    }
    ImGui::End();
}