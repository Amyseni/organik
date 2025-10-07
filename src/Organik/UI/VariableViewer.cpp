#include "Synthetik.h"
#include "zhl.h"
#include "DefinitionHelpers/InstanceHelper.h"
#include "VariableHelper.h"
#include "VariableViewer.h"
#include "ItemFilter.h"
#include "Globals.h"

#include "Action.h"
#include "UIManager.h"
#include "VariableDefHelper.h"
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

std::unordered_map<int32_t, bool> *GetTrackedObjects()
{
    static std::unordered_map<int32_t, bool> trackedObjects;
    if (!trackedObjects.size())
    {
        GetLogger()->LogSimple("Initializing tracked objects");
        trackedObjects.clear();
        trackedObjects.reserve(Objects::ObjIndexes.size());
        for(size_t i=0; i< Objects::ObjIndexes.size(); i++)
        {
            if (Objects::ObjIndexes[i] <= 0) continue;
            if (i == Objects::obj_player || i == Objects::obj_control
                || i == Objects::obj_player || i == Objects::outgame || i == Objects::obj_stats
                || i == Objects::obj_item_parent || i == Objects::obj_allgame || i == Objects::obj_chest_parent || i == Objects::obj_item_parent
                || i == Objects::obj_trap_parent || i == Objects::obj_trap_slow || i == Objects::obj_item_parent || i == Objects::obj_stats
                || i == Objects::obj_weapon_PARENT || i == Objects::obj_item_parent
                    || i == Objects::obj_perk_parent
                )
            {
                trackedObjects[Organik::Objects::ObjIndexes[i]] = true;
                continue;
            }
            trackedObjects[Organik::Objects::ObjIndexes[i]] = false;
        }
        GetLogger()->LogSimple("Done init");
        
    }
    return &trackedObjects;
}
void recurse(CObjectGM* obj, std::function<void(CObjectGM*)> pred) {
    if (obj)
        pred(obj);
    if (obj->m_ChildrenMap && obj->m_ChildrenMap->m_UsedCount)
    {
        auto it = obj->m_ChildrenMap->begin();
        const CHashMap<int32_t, CObjectGM*, 2>::Element *elem = nullptr;
        while (obj->m_ChildrenMap->FindNextValue(it, &elem))
        {
            CObjectGM* child = elem->m_Value;
            recurse(child, pred);
        }
    }
}
auto noParentsDisplayed = [](CObjectGM* Object) -> auto {
    if (!Object) return false; // error, don't show object. should be unreachable
    if (!(Object->m_ParentObject)) return true; // no parents, highest level object. always show.
    CObjectGM* currentObject = Object->m_ParentObject; // don't modify objects
    while (currentObject) // allow to be null, exit once it is
    {
        if ((*GetTrackedObjects())[currentObject->m_ID] == true)
            return Object !=currentObject;
        if (!currentObject->m_ParentObject || currentObject->m_ParentObject == currentObject)
            break;
        currentObject = currentObject->m_ParentObject;
    }
    return currentObject != Object && !(*GetTrackedObjects())[currentObject->m_ID];
};
auto GetObjectsForTabs() -> std::unordered_map<CObjectGM*, bool>*
{
    static std::unordered_map<CObjectGM*, bool> objectsForTabs;
    if (!objectsForTabs.size()) {
        GetLogger()->LogSimple("Initializing objects for tabs");
        objectsForTabs = {
            GLOBALS_DO(FMAPOBJ)
            PARENTS_DO(FMAPOBJ)
        };
        GetLogger()->LogSimple("Done init tab objects");
    }
    return &objectsForTabs;
}

typedef void (*objectPred)(CObjectGM*, std::function<void(CObjectGM*)>);
static std::string objNameFilterBuf;
void InstanceVariableViewer::displayFunc(CObjectGM* child, bool& tracked){
    if (!child) return;
    ImGui::PushID(child);
    if (ImGui::TreeNodeEx((const void*)(child), 
        (ImGuiTreeNodeFlags_SpanAllColumns | ImGuiTreeNodeFlags_DrawLinesFull
        | (tracked ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None)
        | ImGuiTreeNodeFlags_AllowItemOverlap
        | ImGuiTreeNodeFlags_NavLeftJumpsToParent),
        "%s (#%d, %d children)", child->m_Name ? child->m_Name : "<no_name>",
            child->m_ID,
            child->m_ChildrenMap ? child->m_ChildrenMap->m_UsedCount : 0))
    {
        if (child->m_ChildrenMap && child->m_ChildrenMap->m_UsedCount)
        {
            auto it = child->m_ChildrenMap->begin();
            const CHashMap<int32_t, CObjectGM*, 2>::Element *elem = nullptr;
            while (child->m_ChildrenMap->FindNextValue(it, &elem))
            {
                CObjectGM* obj = elem->m_Value;
                displayFunc(obj, ((*GetTrackedObjects())[obj->m_ID]));
                
            }
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
                    (*GetTrackedObjects())[obj->m_ID] = tracked;
            });
        }
        ImGui::EndPopup();
    }
    ImGui::PopID();
}
static bool showColorEditPopup = false;
static bool showNoEditPopup = false;
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
    auto trackedObjects = GetTrackedObjects();
    for(auto &[objID, show] : (*trackedObjects))
    {
        CObjectGM* obj = Object_Data(objID);

        if (!obj->m_ParentObject)
            displayFunc(obj, show);
    }
}
void InstanceVariableViewer::CObjectDetailsPane(CObjectGM* toggleObj)
{
    static CObjectGM* toAdd = nullptr;
    static Action *toAddEvent = nullptr;
    
    static struct {
        struct Tab {
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
            inline CObjectGM* GetObject()  {
                return m_Object ? m_Object : (m_Object = Object_Data(m_ObjectID));
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
                    ImGui::TableSetupColumn("Event Name", ImGuiTableColumnFlags_WidthFixed, 15.0f);
                    ImGui::TableSetupColumn("Owner", ImGuiTableColumnFlags_WidthFixed, 25.0f);
                    ImGui::TableSetupColumn("Event Function", ImGuiTableColumnFlags_WidthStretch, 160.0f);
                    ImGui::TableHeadersRow();

                    GetObject()->m_EventsMap->ForEach([&](const int64_t& key, const CHashMapHash& hash, CEvent*& value) -> void
                    {
                        ImGui::PushID(value);
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        bool open = ImGui::Selectable("##eventrow##", false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap);
                        if (open || ImGui::IsItemClicked())
                            ImGui::OpenPopup("event_details");

                        ImGui::TableSetColumnIndex(0);
                        int32_t code = key >> 32;
                        int32_t subcode = key & 0xFFFFFFFF;
                        ImGui::SetItemTooltip("Click for details");
                        ImGui::SameLine(0,0);
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



    for (const auto& [obj, show] : (*GetObjectsForTabs())) {
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
    if (!(*GetTrackedObjects()).contains(obj->m_ID))
        return;
    if (!(*GetTrackedObjects())[obj->m_ID])
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
    for(auto &[objID, show] : (*trackedObjects))
    {
        CObjectGM* obj = Object_Data(objID);
        if (!show && obj->m_ParentObject) continue;
        if (obj->m_ParentObject != obj)
        {
            if (!noParentsDisplayed(obj))
                continue; // only show root parents here.
        }
        RecurseObjectInstanceTree(obj);
    }
}

struct CachedRValue
{
    const char* m_Name;
    int32_t m_ID;
    RValueType m_Kind;
    RValue lastKnownValue;
    const VariableDef* m_Definition;
    const CInstance* owner;

    CInstance** ownerRef;

    CachedRValue(): m_Name("<no_name>"), m_ID(0), m_Kind(VALUE_UNDEFINED), lastKnownValue(), owner(nullptr), ownerRef(nullptr) {}
    CachedRValue(CInstance* inst, int32_t id, const char* name)
        : m_Name(name), m_ID(id), m_Kind(VALUE_UNDEFINED), lastKnownValue(), owner(inst), ownerRef(&inst)
    {
        if (owner)
        {
            RValue* val = owner->InternalGetYYVarRef(m_ID);
            if (val)
                m_Kind = (RValueType) val->GetKind();
            lastKnownValue = val ? *val : RValue();
        }
    }
    explicit operator bool() const { 
        if (!(owner && ownerRef)) return false;
        if (*ownerRef != owner) return false;
        return true;
    }

    RValue* operator->() { return owner->InternalGetYYVarRef(m_ID); }
    RValue& operator*() { return *(owner->InternalGetYYVarRef(m_ID)); }
    RValue* operator->() const { return owner->InternalGetYYVarRef(m_ID); }
    RValue& operator*() const { return *(owner->InternalGetYYVarRef(m_ID)); }

    template<typename MemberType>
    MemberType& operator->*( MemberType RValue::* member) { return (*(owner->InternalGetYYVarRef(m_ID))).*member; }
    template<typename MemberType>
    MemberType& operator->*(const MemberType RValue::* member) const { return (*(owner->InternalGetYYVarRef(m_ID))).*member; }

    operator RValue*() { return owner->InternalGetYYVarRef(m_ID); }
    operator RValue*() const { return owner->InternalGetYYVarRef(m_ID); }

    bool DoCache() {
        if (!owner) return false;
        if (!ownerRef) return false;
        if (*ownerRef != owner) return false;
        RValue* current = owner->InternalReadYYVar(m_ID);
        if (!current) return false;
        const RValue copy = current ? *current : RValue(1.0);
        if (parseRValueNumber<int64_t>(lastKnownValue) != parseRValueNumber<int64_t>(copy)) {
            lastKnownValue = copy;
            return true;
        }
        return false;
    }
};
struct CacheState {
    CInstance* instance = nullptr;
    enum : ImGuiID {
        COL_NAME = 0,
        COL_TYPE = 1,
        COL_VALUE = 2
    };
    
    CachedRValue* cachedValues = nullptr;
    CHashMap<int32_t, RValue*, 3>* hashMapPtr = nullptr;
    int32_t usedCount = 0;
    int32_t actualCount = 0;
    int32_t instId = 0;
    
    inline static CacheState* Get() {
        static CacheState instance;
        return &instance;
    }
    
    static int __cdecl compare_func(const void* lhs, const void* rhs) {
        auto specs = *GetTableSortSpecs();
        int delta = 0;
        auto* a = (const CachedRValue*)lhs;
        auto* b = (const CachedRValue*)rhs;
        if (!specs || specs->SpecsCount == 0)
            return 0;
        int sort_dir = 1;
        for (int i=0; i < specs->SpecsCount; i++)
        {
            const ImGuiTableColumnSortSpecs* spec = &specs->Specs[i];
            sort_dir = (spec->SortDirection == ImGuiSortDirection_Ascending) ? 1 : -1;
            switch (spec->ColumnUserID)
            {
            case COL_NAME: // Name
                delta += strcmp(a->m_Name, b->m_Name);
                break;
            case COL_TYPE: // Type
                delta += (a->m_Kind - b->m_Kind);
                break;
            case COL_VALUE: // Value
                delta += (parseRValueNumber<int32_t>(a->operator*()) - parseRValueNumber<int32_t>(b->operator*()));
                break;
            }
        }
        return (delta == 0 ? 0 : (delta * sort_dir));
    }
    static void SortEntries(CachedRValue* entries, size_t count, ImGuiTableSortSpecs* sort_specs)
    {
        (*GetTableSortSpecs()) = sort_specs;
        if (!sort_specs || sort_specs->SpecsCount == 0)
            return;


        std::qsort(entries, count, sizeof(CachedRValue), compare_func);
    }
    bool Is(CInstance* inst) 
    {
        if (!inst) return false;
        if (!(this->operator bool())) return false;
        if (instance != inst) return false;
        if (instId != inst->m_ID) return false;
        if (hashMapPtr != inst->m_YYVarsMap) return false;
        return true;
    }
    CInstance* GetInstance() {
        static auto* lastValidInstance = (CInstance*)nullptr;
        if (instance && instance->m_ID > 0 && instance->m_YYVarsMap) 
        {
            lastValidInstance = instance;
        }
        return lastValidInstance;
    }
    void Cache(CInstance* inst) {
        if (!inst || !(inst->m_YYVarsMap) || !(inst->m_YYVarsMap->m_UsedCount)) 
        {
            return;
        }

        instance = inst;
        instId = instance->m_ID;
        
        hashMapPtr = instance->m_YYVarsMap;

        if (cachedValues)
        {
            delete[] cachedValues;
            cachedValues = nullptr;
        }

        if (!hashMapPtr) return;
        usedCount = instance->m_YYVarsMap->m_UsedCount;
        if (usedCount < 1) return;
        
        cachedValues = new CachedRValue[usedCount];
        
        auto it = hashMapPtr->begin();
        static const CHashMap<int32_t, RValue*, 3>::Element* elem = nullptr;
        int32_t i=0;
        while (hashMapPtr->FindNextValue(it, &elem))
        {
            const auto elemn = *elem;
            if (!(*elem).m_Value) continue;
            if ((*elem).m_Hash & HASH_DELETED) continue;
            switch((*elem).m_Value->m_Kind)
            {
                case VALUE_NULL:
                case VALUE_UNDEFINED:
                case VALUE_UNSET:
                case VALUE_PARAMS:
                    continue;
                default: break;
            }
            int32_t varid = (*elem).m_Key;
            const char* varname = nullptr;
            if (Organik::Variables::HashToVariableMap.contains(varid)) {
                varname = Organik::Variables::HashToVariableMap[varid];
            } else {
                varname = "<no_name>";
            }
            if (!varname || !(varname[0]))
                varname = "<no_name>";
            
            ::new (&cachedValues[i]) CachedRValue(instance, varid, varname);
            i++;
        }
        actualCount = i;
    }

    operator bool() {
        if (hashMapPtr) {
            bool result = hashMapPtr->m_UsedCount > 0;
            return result;
        }
        return false;
    }
    ~CacheState() {
        if (cachedValues)
        {
            delete[] cachedValues;
            cachedValues = nullptr;
        }
        instance = nullptr;
        hashMapPtr = nullptr;
        usedCount = 0;
        instId = 0;
    }
};
static std::string* renameBuffer;
static std::string* descBuffer;
void InstanceVariableViewer::DisplayEditDetailsPopup()
{
    ImGui::SetNextWindowSize(ImVec2(400, 525), ImGuiCond_FirstUseEver);
    if (ImGui::BeginPopup("variable_name_popup", 
        ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoSavedSettings
    ))
    {
        renameBuffer = &editingVariableDef->m_Name;
        descBuffer = &editingVariableDef->m_Description;
        if (editingVariableID > 0)
        {
            ImGui::TextDisabled("Variable: %s (ID: %d)", editingVariable.c_str(), editingVariableID);
            ImGui::Separator();
            ImGui::InputTextWithHint("Rename: ", "Enter a more helpful name...", renameBuffer, ImGuiInputTextFlags_EnterReturnsTrue);
            unsigned char *currentFlagsBytes = ((unsigned char*) &(editingVariableDef->m_DisplayFlags));

            unsigned char *displayTypeBuffer = &currentFlagsBytes[1];
            const char* currentType = VarDefHelper->GetTypeListForDisplay()[max(signed char(*displayTypeBuffer-1), 0)];
            if (ImGui::BeginCombo("Type", currentType, ImGuiComboFlags_None))
            {
                for (int n = 0; n < VarDefHelper->GetDisplayTypeCount(); n++)
                {
                    bool is_selected = (n == (*displayTypeBuffer) - 1);
                    if (ImGui::Selectable(VarDefHelper->GetTypeListForDisplay()[n], is_selected))
                    {
                        (*displayTypeBuffer) = (uint8_t)(n + 1);
                        if (*displayTypeBuffer == VariableDef::TYPE_COLOR)
                        {
                            *((uint32_t*)currentFlagsBytes) = (VariableDef::TYPE_COLOR | VariableDef::DISPLAY_HEX | VariableDef::TYPE_SPECIAL);
                        }
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            ImGui::SetItemTooltip("What type of value is represented by this variable?\n\nNote: This does NOT affect how the variable is stored, only how it is displayed.");
            unsigned char *displayFormatBuffer = &currentFlagsBytes[0];
            const char* currentFormat = VarDefHelper->GetFormatListForDisplay()[max(signed char(*displayFormatBuffer)-1,0)];
            if (ImGui::BeginCombo("Format", currentFormat, ImGuiComboFlags_None))
            {
                for (int n = 0; n < VarDefHelper->GetDisplayFormatCount(); n++)
                {
                    bool is_selected = (n == (*displayFormatBuffer) - 1);
                    if (ImGui::Selectable(VarDefHelper->GetFormatListForDisplay()[n], is_selected))
                    {
                        (*displayFormatBuffer) = (uint8_t)(n+1);
                        if (*displayFormatBuffer == VariableDef::DISPLAY_ROTATION)
                        {
                            *(uint32_t*)currentFlagsBytes = (VariableDef::TYPE_INT | VariableDef::DISPLAY_ROTATION | (editingVariableDef->m_DisplayFlags & 0xFF0000));
                        }
                        else if (*displayFormatBuffer == VariableDef::DISPLAY_PERCENT)
                        {
                            *currentFlagsBytes = (VariableDef::TYPE_REAL | VariableDef::DISPLAY_PERCENT | (editingVariableDef->m_DisplayFlags & 0xFF0000));
                        }
                        else if (*displayFormatBuffer == VariableDef::DISPLAY_HEX)
                        {
                            *(uint32_t*)currentFlagsBytes = (VariableDef::TYPE_INT | VariableDef::DISPLAY_HEX | (VariableDef::TYPE_SPECIAL));
                        }
                        else if (*displayFormatBuffer == VariableDef::DISPLAY_TIME)
                        {
                            *(uint32_t*)currentFlagsBytes = (VariableDef::TYPE_INT | VariableDef::DISPLAY_TIME | (VariableDef::TYPE_SPECIAL));
                        }
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            ImGui::SetItemTooltip("What formatting should be applied when displaying this e.g. in your buffs in the HUD?\n\nNote: This does NOT affect how the variable is stored, only how it is displayed.");
            unsigned char *displaySpecialBuffer = &currentFlagsBytes[2];
            const char* currentSpecial = VarDefHelper->GetPositiveNegativeListForDisplay()[max(signed char(*displaySpecialBuffer)-1, 0)];
            if (ImGui::BeginCombo("Special", currentSpecial, ImGuiComboFlags_None))
            {
                for (int n = 0; n < VarDefHelper->GetDisplaySpecialCount(); n++)
                {
                    bool is_selected = (n == (*displaySpecialBuffer) - 1);
                    if (ImGui::Selectable(VarDefHelper->GetPositiveNegativeListForDisplay()[n], is_selected))
                    {
                        (*displaySpecialBuffer) = (uint8_t)(n + 1);
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            ImGui::SetItemTooltip("When is this value considered a 'buff'?\n\nNote: Positive means above 1.00 for multipliers, above 0.00%% for percentages.\nSpecial/Neutral means don't show as a buff or debuff.");
            ImGui::InputTextWithHint("Description: ", "What does this variable do?", descBuffer, ImGuiInputTextFlags_Multiline);
            ImGui::SetItemTooltip("Saves automatically as you type, press enter for a new line as normal.");
            if (ImGui::Button("Close"))
            {
                ImGui::CloseCurrentPopup();
                showPopup = false;
                renameBuffer = nullptr;
                descBuffer = nullptr;
                editingVariableID = -1;
                editingVariableDef = nullptr;
                editingVariable = "";
                editingValue = nullptr;
            }
        }
        else
        {
            ImGui::TextDisabled("Error: No variable selected");
        }
        ImGui::EndPopup();
    }
}
void InstanceVariableViewer::DrawInner()
{
//     Organik::GetLogger()->LogFormatted("DrawInner: Starting");
    static std::regex pattern("", std::regex_constants::match_continuous | std::regex_constants::icase | std::regex_constants::optimize);
    static CInstance* viewingInstance = nullptr;

    if (!VisibleInstance) {
        ImGui::TextDisabled("Select an instance to view its variables"); 
        return;
    }
    int id = VisibleInstance->m_ID;
    if (!VisibleInstance->m_YYVarsMap)
    {
        ImGui::Text("Instance has no variable map.");    
        return;
    }
    
    if (VisibleInstance->m_YYVarsMap->m_UsedCount == 0)
    {
        ImGui::Text("Instance has no variable map.");
        return;
    }
    
    if (!(CacheState::Get()) || !(CacheState::Get()->Is(VisibleInstance)))
        CacheState::Get()->Cache(VisibleInstance);
    
    CInstance* cachedInstance = CacheState::Get()->GetInstance();
    
    if (!cachedInstance) {
        ImGui::Text("Error: Cached instance is null");
        return;
    }
    int32_t objectIndex = cachedInstance->m_ObjectIndex;
    const char* objectName = objectIndex ? (Object_Data(objectIndex) ? Object_Data(objectIndex)->m_Name : "<no_name>") : "<no_name>";
    
    bool rowClicked = false;
    ImGui::Text("Variables for: %s (ID: %d)", 
                objectName ? (objectName[0] ? objectName : "<no_name>") : "<no_name>", 
                id);
    ImGui::SameLine(0,5.0);
    static std::string varNameFilterBuf;
    static std::string lastVarNameFilterBuf;
    
    bool pressedEnter = ImGui::InputTextWithHint("##obj_name_filter", "Search Variable Name. (partial matching, regex allowed.)", &varNameFilterBuf, ImGuiInputTextFlags_EnterReturnsTrue);
    ImGui::SameLine(0,5.0);

    if (pressedEnter && !(varNameFilterBuf._Equal(lastVarNameFilterBuf)))
    {
        pattern.assign(varNameFilterBuf, std::regex_constants::icase | std::regex_constants::optimize);
    }
    bool showEditDetailsPopup = false;
    lastVarNameFilterBuf = varNameFilterBuf;
    ImGui::Separator();
    if (ImGui::BeginTable("Variables", 3, 
                ImGuiTableFlags_Sortable |
                ImGuiTableFlags_BordersOuter |
                ImGuiTableFlags_BordersV |
                ImGuiTableFlags_NoBordersInBody |
                ImGuiTableFlags_Borders | 
                ImGuiTableFlags_RowBg | 
                ImGuiTableFlags_Resizable |
                ImGuiTableFlags_ScrollY))
    {
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_PreferSortAscending, 150.0f);
        ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_PreferSortAscending, 80.0f);
        ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupScrollFreeze(0, 1); 

        ImGui::TableHeadersRow();
        
                    // Sort our data if sort specs have been changed!
        if (ImGuiTableSortSpecs* sort_specs = ImGui::TableGetSortSpecs())
        if (sort_specs->SpecsDirty)
        {
            CacheState::Get()->SortEntries(CacheState::Get()->cachedValues, CacheState::Get()->actualCount, ImGui::TableGetSortSpecs());
            sort_specs->SpecsDirty = false;
        }

        for (int32_t i=0;i < CacheState::Get()->actualCount; i++)
        {
            CachedRValue* entry = &CacheState::Get()->cachedValues[i];
            auto value = cachedInstance->InternalGetYYVarRef(entry->m_ID);
            auto name = GetFriendlyName(entry->m_ID);
            name = (name && name[0]) ? name : "<no_name>";
            if (varNameFilterBuf.size() && !(std::regex_search(name, pattern, std::regex_constants::match_any)))
                continue;
            if (!value) {
                return;
            }
            bool rowClicked = false;
            auto rowID = ImGui::GetID(GetLogger()->ParseFormatting("%p##%s##", value, name).c_str());
            ImGui::TableNextRow();
            ImGui::PushID(rowID);
            ImGui::TableSetColumnIndex(0);
            if (ImGui::Selectable(name, (editingValue == value),
                ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap))
            {
                if (!(value->m_Kind == VALUE_ARRAY))
                {
                    editingVariableID = entry->m_ID;
                    editingVariable = name;
                    editingValue = value;
                    showPopup = true;
                    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
                    {
                        
                    }
                    PrepareEditValue(value);
                }
            }
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::TableGetHoveredRow() == i+1)
            {
                editingVariableID = entry->m_ID;
                editingVariableDef = &(VarDefHelper->m_VariableDefs[entry->m_ID]);
                editingVariable = name;
                showEditDetailsPopup = true;
            }
            ImGui::SetItemTooltip("Click to edit variable\nRight-click to edit variable details (Confused? Try this!).");
            ImGui::TableNextColumn();
            ImGui::Text(value->GetKindName());
            ImGui::TableNextColumn();
            uint32_t kind = value->GetKind();

            DisplayVariableValue(name, value);
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
    if (showPopup)
    {
        ImGui::OpenPopup("VarEdit");
        showPopup = false;
    }
    if (showEditDetailsPopup){
        ImGui::OpenPopup("variable_name_popup");
        showEditDetailsPopup = false;
    }
    if (showColorEditPopup)
    {
        ImGui::OpenPopup("color_edit");
        showColorEditPopup = false;
    }
    DisplayEditPopup();
    DisplayEditDetailsPopup();

        
}

void InstanceVariableViewer::PrepareEditValue(RValue* value, int32_t varId)
{
    if (!value) return;
    RValueType kind = (RValueType) value->m_Kind;   
    if (varId > 0)
    {
        editingVariableID = varId;
        auto &varDef = VarDefHelper->GetVarDefForID(varId);
        auto [displayFlags, typeFlags, specFlags] = std::tuple<uint8_t,uint8_t,uint8_t>({
            (uint8_t) (varDef.m_DisplayFlags & 0x000000FF),         //! display flags
            (uint8_t)((varDef.m_DisplayFlags & 0x0000FF00) >> 8),   //! type flags
            (uint8_t)((varDef.m_DisplayFlags & 0x00FF0000) >> 16)   //! special flags
        });
        kind = (RValueType)0;
        if (varDef.m_DisplayFlags & VariableDef::TYPE_ARRAY)
        {
            kind = VALUE_ARRAY;   
        }
        switch (typeFlags)
        {
            case VariableDef::TYPE_BOOL:
                reinterpret_cast<uint32_t&>(kind) |= (uint32_t) VALUE_BOOL;
                break;
            case VariableDef::TYPE_COLOR:
                reinterpret_cast<uint32_t&>(kind) |= (uint32_t) VALUE_VEC3;
                break;
            case VariableDef::TYPE_INT:
                reinterpret_cast<uint32_t&>(kind) |= (uint32_t) VALUE_INT32;
                break;
            case VariableDef::TYPE_REAL:
                reinterpret_cast<uint32_t&>(kind) |= (uint32_t) VALUE_REAL;
                break;
            case VariableDef::TYPE_STRING:
                reinterpret_cast<uint32_t&>(kind) |= (uint32_t) VALUE_STRING;
                break;
        }
        if (specFlags & VariableDef::TYPE_SPECIAL)
        {
            switch (value->GetKind())
            {
            case VALUE_FUNCTION:
            case VALUE_UNDEFINED:
            case VALUE_NULL:
            case VALUE_UNSET:
                showPopup = false;
                showNoEditPopup = true;
                ImGui::OpenPopup("noedit_error", ImGuiPopupFlags_MouseButtonLeft | ImGuiPopupFlags_AnyPopupLevel );
                editingVariableID = 0;
                editingVariable = "";
                editingValue = nullptr;
                return;
            case VALUE_INT32:
                reinterpret_cast<uint32_t&>(kind) &= ~VALUE_VEC3;
                reinterpret_cast<uint32_t&>(kind) |= VALUE_VEC4;
                break;
            }
        }
        editKindSpecial = kind;
        editingVariableID = varId;
        bool retEarly = false;  
        if (kind & VALUE_VEC3 || kind & VALUE_VEC4)
        {
            retEarly = true;
            showPopup = false;
            showColorEditPopup = true;
            if (kind & VALUE_VEC3)
            {
                editingValue = value;
            }
            else if (kind & VALUE_VEC4)
            {
                editingValue = value;
            }
            editingVariable = varDef.m_Name.length() ? varDef.m_Name : Code_Variable_Find_Name(nullptr, 0, varId);
            editingVariable += GetLogger()->ParseFormatting(" - Color %s", (kind & VALUE_VEC4) ? "(RGBA)" : "(RGB)");
        }
        if (retEarly)
            return;
    
        editingVariable = varDef.m_Name.length() ? varDef.m_Name : Code_Variable_Find_Name(nullptr, 0, varId);
        *editingValue = *value;
    }
    
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
    if (ImGui::BeginPopup("noedit_error", ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup ))
    {
        ImGui::TextDisabled("This variable type cannot be edited.");
        ImGui::Separator();
        ImGui::Text("Variable: %s", editingVariable.c_str());
        ImGui::Separator();
        if (ImGui::Button("OK"))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
        return;
    }
    if (ImGui::BeginPopup("color_edit", ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup ))
    {
        if (editColorRGB + editColorRGBA == 0) 
        {
            ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
            showColorEditPopup = false;
            return;
        }
        ImGui::Text("Type: %s", editingValue->GetKindName());
        ImGui::Separator();
        
        uint32_t kind = 0;
        bool valueChanged = false;
        if (editingVariableDef->m_ID > 0)
        {
            kind = editKindSpecial;
        }
    
        bool edited = false;
        if (kind & VALUE_VEC3)
        {
            edited = (ImGui::ColorEdit3("Color (RGB)", &((vec3&)editingValue).x,
                ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_DisplayRGB
             | ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayHex));
        }
        else if (kind & VALUE_VEC4)
        {
            edited = (ImGui::ColorEdit4("Color (RGBA)", &((vec4&)editingValue).x,
                ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_PickerHueWheel
                | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_DisplayHex));
        }
        ImGui::Separator();

        // ImGui::SetKeyOwner((ImGuiKey)( ImGuiKey_Enter | ImGuiKey_Escape), ImGui::GetItemID(), ImGuiInputFlags_CondActive | ImGuiInputFlags_RouteUnlessBgFocused);
        ImGui::SameLine();
        return;
    }
    if (ImGui::BeginPopup("VarEdit", ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup )) {
        if (editingValue == nullptr)
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
                    if (ImGui::IsKeyDown(ImGuiKey_Enter))
                    {
                        ImGui::CloseCurrentPopup();
                    }
                }
            }
            else
            {
                if (ImGui::InputDouble("Value", &editDoubleValue, 0.1, 10.0, "%.6f", ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    *editingValue = RValue(static_cast<double>(editDoubleValue));
                    valueChanged = true;
                    if (ImGui::IsKeyDown(ImGuiKey_Enter))
                    {
                        ImGui::CloseCurrentPopup();
                    }
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
                YYSetString(editingValue, buffer);
                if (ImGui::IsKeyDown(ImGuiKey_Enter))
                {
                    ImGui::CloseCurrentPopup();
                }
            }
        }
        else if (kind == VALUE_BOOL)
        {
            if (ImGui::Checkbox("Value", &editBoolValue))
            {
                *editingValue = editBoolValue ? RValue(true): RValue(false);
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
    if (element->GetKind() != VALUE_ARRAY)
    {
        DisplayVariableValue((GetLogger()->ParseFormatting("%s[%zu]", dispName, index)).c_str(), element);
    }
    else
    {
        int32_t arrayLength = std::launder((*element).m_RefArray)->m_Length;
        std::vector<RValue*> elementRefArray = element->ToRefVector();
        if (ImGui::TreeNodeEx((void*)element, ImGuiTreeNodeFlags_SpanAllColumns | ImGuiTreeNodeFlags_AllowItemOverlap, "%s[%zu]", dispName, arrayLength))
        {
            
            if (ImGui::BeginTable(name, 3, 
                ImGuiTableFlags_BordersOuter |
                ImGuiTableFlags_HighlightHoveredColumn |
                ImGuiTableFlags_BordersV |
                ImGuiTableFlags_NoBordersInBody |
                ImGuiTableFlags_NoKeepColumnsVisible |
                ImGuiTableFlags_RowBg | 
                ImGuiTableFlags_Resizable |
                ImGuiTableFlags_ScrollY, ImVec2(0.0, 
                    (ImGui::GetTextLineHeightWithSpacing() * 1.20f) * 
                    max(
                        min(14.0, (float)arrayLength), 
                        5.0f
                )   )
            ))
            {
                ImGui::TableSetupColumn("Index", ImGuiTableColumnFlags_WidthFixed, 50.0f);
                ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 120.0f);
                ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableHeadersRow();
                for (int j = 0; j < arrayLength; j++)
                {
                    ImGui::PushID(elementRefArray[j]);
                    
                    ImGui::TableNextRow(ImGuiTableRowFlags_None, ImGui::GetTextLineHeightWithSpacing()*1.20f);
                    ImGui::TableSetColumnIndex(0);
                    char arr[21] = "##arrayElement## ##";
                    arr[18] = '0' + (j % 10);
                    if (ImGui::Selectable(&(arr[0]), (editingValue == elementRefArray[j]),
                        ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap)
                    )
                    {
                        // if (!(elementRefArray[j]->m_Kind == VALUE_ARRAY))
                        // {
                        //     editingVariable = name;
                        //     editingValue = elementRefArray[j];
                        //     showPopup = true;
                        //     rowClicked = false;
                        //     PrepareEditValue(elementRefArray[j], -1);
                        // }
                    }
                    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::TableGetHoveredRow() == j+1)
                    {
                        if (ImGui::MenuItem("Append Value"))
                        {
                            (*element).push_back(RValue(0));
                            ::new (&(*element)[j]) RValue(*(elementRefArray[j]));
                            element->back().m_Kind = elementRefArray[j]->m_Kind;
                        }
                    }
                    ImGui::SameLine(0,0);
                    ImGui::Text("[%zu]", j);
                    
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%s", elementRefArray[j]->GetKindName());
                    ImGui::TableSetColumnIndex(2);

                    DisplayArrayElement(j, elementRefArray[j]);
                    ImGui::PopID();
                }
                ImGui::EndTable();
            }
            ImGui::TreePop();
        }
    }
}
const char* InstanceVariableViewer::GetFriendlyName(int32_t varId)
{
    const auto& varDef = VarDefHelper->GetVarDefForID(varId);
    return varDef.m_Name.c_str();
}
void InstanceVariableViewer::DisplayVariableValue(MapDisplayEntry<int32_t, RValue, 3>* entry)
{
    DisplayVariableValue(entry->m_Name, entry->m_Value);
}
void InstanceVariableViewer::DisplayVariableValue(const char* name, RValue* value, int32_t id)
{
    if (!value) {
        ImGui::TextDisabled("<null>");
        return;
    }
    if (id > 0)
    {
        const auto& thisVarDef = VarDefHelper->GetVarDefForID(id);
        if (thisVarDef.m_DisplayFlags != 0)
        {
            ImGui::Text("%s", thisVarDef.GetRepresentation(*value).c_str());
            return;
        }
    }
    auto kind = value->GetKind();
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
        const char* strVal = nullptr;
        if ((*std::launder(&value->m_RefString)) != nullptr)
            strVal = (*std::launder(&value->m_RefString))->m_String;
        if (!strVal)
            strVal = "<null>";
        if (!strVal[0])
            strVal = "<empty>";
        ImGui::Text("\"%s\"", std::string(strVal).data());
    }
    else if (kind == VALUE_ARRAY)
    {
        DisplayArrayElement(0, value, name); 
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
    else 
    {
        ImGui::TextDisabled("Unknown. Value hidden for safety.");
    }
    if (rowClicked)
    {
        editingVariable = name;
        editingValue = value;
        showPopup = true;
        PrepareEditValue(value);
    }
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