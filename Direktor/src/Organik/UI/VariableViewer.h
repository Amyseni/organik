#pragma once

#include "Synthetik.h"
#include "zhl_internal.h"
#include "zhl.h"
#include "imgui/imgui.h"
#include <vector>
#include <string>
#include <sstream>

inline ImGuiTableSortSpecs** GetTableSortSpecs()
{
    static ImGuiTableSortSpecs* sortSpecs = nullptr;
    return &sortSpecs;
}
template<typename kT, typename T, int32_t N>
struct MapDisplayEntry {

    enum : ImGuiID {
        COL_NAME = 0,
        COL_TYPE = 1,
        COL_VALUE = 2
    };
    const char* m_Name;
    uint32_t m_Kind;
    struct
    {
        int32_t m_Index;
        CHashMap<kT, T*, N>* m_Map;

        T* operator->() { return m_Map->m_Elements[m_Index].m_Value; }
        T& operator*() { return *(m_Map->m_Elements[m_Index].m_Value); }
        T* operator->() const { return m_Map->m_Elements[m_Index].m_Value; }
        T& operator*() const { return *(m_Map->m_Elements[m_Index].m_Value); }

        template<typename MemberType>
        MemberType& operator->*( MemberType T::* member) { return (*(m_Map->m_Elements[m_Index].m_Value)).*member; }
        template<typename MemberType>
        MemberType& operator->*(const MemberType T::* member) const { return (*(m_Map->m_Elements[m_Index].m_Value)).*member; }

        operator T*() { return m_Map->m_Elements[m_Index].m_Value ? m_Map->m_Elements[m_Index].m_Value : nullptr; }
        operator T*() const { return m_Map->m_Elements[m_Index] ? m_Map->m_Elements[m_Index].m_Value : nullptr; }

    } m_Value;
    CHashMap<kT, T*, N>* currentVarMap;

    MapDisplayEntry(const char* name, uint32_t kind, int32_t index, CHashMap<kT, T*, N>* map)
        : m_Name(name), m_Kind(kind), m_Value{ index, map } {
                currentVarMap = map;
        }

    static int __cdecl compare_func(const void* lhs, const void* rhs) {
        const MapDisplayEntry* a = (const MapDisplayEntry*)lhs;
        const MapDisplayEntry* b = (const MapDisplayEntry*)rhs;
        auto specs = *GetTableSortSpecs();
        int delta = 0;
        for (int i=0; i < specs->SpecsCount; i++)
        {
            const ImGuiTableColumnSortSpecs* spec = &specs->Specs[i];
            int sort_dir = (spec->SortDirection == ImGuiSortDirection_Ascending) ? 1 : -1;
            switch (spec->ColumnUserID)
            {
            case COL_NAME: // Name
                delta += sort_dir * strcmp(a->m_Name, b->m_Name);
            case COL_TYPE: // Type
                delta += sort_dir * (a->m_Kind - b->m_Kind);
            case COL_VALUE: // Value
                delta = delta & currentVarMap->m_CurrentMask;
                // my atttempt at making the type column into a sort of grouping
                switch (a->m_Kind) 
                {
                    case VALUE_STRING:
                        delta += sort_dir * strcmp((*std::launder(&a->m_Value->m_RefString))->m_String, 
                            b->m_Kind == VALUE_STRING 
                            ? (*std::launder(&b->m_Value->m_RefString))->m_String
                            : ""); //std::to_string(parseRValueNumber<double>(b->m_Value)).data());
                    case VALUE_BOOL:
                        delta += sort_dir * static_cast<int32_t>((*std::launder<int64_t>(&(a->m_Value->m_i64)) - *std::launder<int64_t>(&(b->m_Value->m_i64))) & INT_MAX);
                    case VALUE_INT32:
                    case VALUE_INT64:
                        delta += sort_dir * static_cast<int32_t>((*std::launder<int64_t>(&(a->m_Value->m_i64)) - *std::launder<int64_t>(&(b->m_Value->m_i64))) & INT_MAX);
                    case VALUE_REAL:
                        delta += sort_dir * static_cast<int32_t>(parseRValueNumber<double>(a->m_Value) - parseRValueNumber<double>(b->m_Value));
                    case VALUE_ARRAY:
                        delta += sort_dir * (a->m_Value->GetArrayLength() - (b->m_Kind == VALUE_ARRAY 
                            ? b->m_Value->GetArrayLength() 
                            : (b->m_Kind == VALUE_MAP ? b->m_Value->GetMemberCount() : 0)));
                    case VALUE_MAP:
                        delta += sort_dir * (a->m_Value->GetMemberCount() - (b->m_Kind == VALUE_ARRAY 
                            ? b->m_Value->GetArrayLength() 
                            : (b->m_Kind == VALUE_MAP ? b->m_Value->GetMemberCount() : 0)));
                    default:
                        delta += sort_dir * (a->m_Value - b->m_Value);
                }
                delta += sort_dir * (a->m_Value - b->m_Value);
            default:
                delta += 0;
            }
        }
        return delta ? delta : (a->m_Value - b->m_Value);
    };
    static void SortEntries(std::vector<MapDisplayEntry<kT, T*, N>>* entries, ImGuiTableSortSpecs* sort_specs)
    {
        (*GetTableSortSpecs()) = sort_specs;
        if (!sort_specs || sort_specs->SpecsCount == 0)
            return;


        std::qsort(entries->data(), entries->size(), sizeof(MapDisplayEntry<kT, T*, N>), compare_func);
    }
};
namespace Organik
{
    struct InstanceVariableViewer : public UIElement
    {
        enum {
            SomeValue = 0,
            AnotherValue = 1,
            YetAnotherValue = 2,
        };
        CInstance* VisibleInstance = 0; // Currently visible instance
        InstanceVariableViewer(std::string name = "Instance Variable Viewer") : UIElement(name) {};
        ~InstanceVariableViewer() = default;
        void Draw(bool& out_mousedOver, bool* p_open = NULL, const std::string &title = "");
        int32_t GetHash() override {
            static int32_t hash = typeid(InstanceVariableViewer).hash_code();
            return hash;
        }
        void GetName(char* buf) override
        {
            sprintf(buf, "Instance Variable Viewer (%p)", reinterpret_cast<void*>(VisibleInstance));
        }
        void DrawObjectParentsFilterList();
        void DrawObjectList();
        void DisplayVarTableEntry(CInstance* instance, const std::string& memberName, RValue* memberValue);
        void RecurseObjectInstanceTree(CObjectGM* obj);
        void CObjectDetailsPane(CObjectGM* obj = nullptr);
    private:
        CObjectGM* selectedObject = nullptr;
        void DrawInner();
        void DisplayVariableValue(MapDisplayEntry<int32_t, RValue, 3>* entry);
        void DisplayVariableValue(const char* name, RValue* value);
        std::string GetValuePreview(RValue* value);
        std::string editingVariable;
        int32_t editingInstanceId;
        RValue* editingValue = nullptr;
        RValue* editingValueArray = nullptr;
        bool rowClicked = false;
        static bool showPopup;
        static bool showArrayPopup;
        
        ImGuiID dock_left_id, dock_right_id;

        ImGuiID dock_left_top_id, dock_left_bottom_id;
        ImGuiID dock_right_top_id, dock_right_bottom_id;

        void DrawObjectTreeWindow(bool* out_mousedOver);
        void DrawInstanceTreeWindow(bool* out_mousedOver);
        void DrawObjectDetailsWindow(bool* out_mousedOver);
        void DrawVariablesPanel(bool* out_mousedOver);
        void displayFunc(CObjectGM* child, bool& tracked);
        void displayFuncNoButton(CObjectGM* child, std::function<void(CObjectGM*)> leafFunc);
        // Edit value buffers
        double editDoubleValue = 0.0;
        int32_t editIntValue = 0;
        int64_t editLongLongValue = 0;
        std::string editStringValue;
        bool editBoolValue = false;

        void PrepareEditValue(RValue* value);
        void DisplayArrayElement(size_t index, RValue* element, const char* name = nullptr);
        void DisplayEditPopup();
    };
}