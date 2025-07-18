#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "RoomViewer.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "../Utils.h"
#include "UIElement.h"
#include "YYToolkit/YYTK_Shared.hpp"

void RoomViewer::Draw(bool& out_mousedOver, bool *p_open, const std::string &title)
{
    ImGui::SetNextWindowSize(ImVec2(320, 750), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(title.c_str(), p_open))
    {
        ImGui::End();
        return;
    }
    CRoom* CurrentRoom = Organik::Utils::GetCurrentRoom();
    if (!CurrentRoom)
    {
        ImGui::Text("No room selected or invalid room index.");
        return;
    }
    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_ChildWindows))
        out_mousedOver = true;
    
    if (ImGui::BeginChild("##tree", ImVec2(300, 0), ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_Borders))
    {
        // Example: Display current room nameGetLogger()->LogFormatted("Run_Room->m_LastTile: %d", CurrentRoom->m_LastTile);
        ImGui::Text("m_InstanceHandle: %p", CurrentRoom->m_InstanceHandle);
        ImGui::Separator();
        ImGui::Text("m_Caption: %s", CurrentRoom->m_Caption);
        ImGui::Separator();
        ImGui::Text("m_Speed: %d", CurrentRoom->m_Speed);
        ImGui::Separator();
        ImGui::Text("m_Width: %d", CurrentRoom->m_Width);
        ImGui::Separator();
        ImGui::Text("m_Height: %d", CurrentRoom->m_Height);
        ImGui::Separator();
        ImGui::Text("m_Persistent: %s", CurrentRoom->m_Persistent ? "true" : "false");
        ImGui::Separator();
        ImGui::Text("m_Color: %d", CurrentRoom->m_Color);
        ImGui::Separator();
        ImGui::Text("m_ShowColor: %s", CurrentRoom->m_ShowColor ? "true" : "false");
        if (ImGui::TreeNode("Backgrounds")) {
            for(int i = 0; i < 8; i++)
            { 
                ImGui::Text("Backgrounds[%d]: %p", i, CurrentRoom->Backgrounds[i]);
                ImGui::Separator();
            }
            ImGui::TreePop();
        }
        ImGui::Text("m_EnableViews: %s", CurrentRoom->m_EnableViews ? "true" : "false");
        ImGui::Separator();
        ImGui::Text("m_ClearScreen: %s", CurrentRoom->m_ClearScreen ? "true" : "false");
        ImGui::Separator();
        ImGui::Text("m_ClearDisplayBuffer: %s", CurrentRoom->m_ClearDisplayBuffer ? "true" : "false");
        if (ImGui::TreeNode("m_Views")) {
            for(int i = 0; i < 8; i++)
            { 
                ImGui::Text("m_Views[%d]: %p", i, CurrentRoom->m_Views[i]);
                ImGui::Separator();
            }
            ImGui::TreePop();
        }
        ImGui::Text("m_LegacyCode: %p", CurrentRoom->m_LegacyCode);
        ImGui::Separator();
        ImGui::Text("m_CCode: %p", CurrentRoom->m_CCode);
        ImGui::Separator();
        ImGui::Selectable("##physicsWorld", ImGui::IsItemHovered(), ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_Highlight | ImGuiSelectableFlags_SpanAllColumns);
        ImGui::SameLine(0,0);
        ImGui::Text("m_HasPhysicsWorld: %s", CurrentRoom->m_HasPhysicsWorld ? "true" : "false");
        if (ImGui::BeginPopupContextItem("PhysicsWorldOptions"))
        {
            ImGui::Text("4 bytes (pointer) %p", (void*) (static_cast<int>(CurrentRoom->m_HasPhysicsWorld)));
            ImGui::EndPopup();
        }
        ImGui::Separator();
        ImGui::Text("m_PhysicsGravityX: %d", CurrentRoom->m_PhysicsGravityX);
        ImGui::Separator();
        ImGui::Text("m_PhysicsGravityY: %d", CurrentRoom->m_PhysicsGravityY);
        ImGui::Separator();
        ImGui::Text("m_PhysicsPixelToMeters: %f", CurrentRoom->m_PhysicsPixelToMeters);
        ImGui::Separator();
        if (ImGui::TreeNode(&CurrentRoom->m_ActiveInstances, "m_ActiveInstances - %d", CurrentRoom->m_ActiveInstances.m_Count))
        {
            ImGui::Text("Pointer: %p", &CurrentRoom->m_ActiveInstances);
            ImGui::Separator();
            ImGui::Text("Count: %d", CurrentRoom->m_ActiveInstances.m_Count);
            ImGui::Separator();
            ImGui::Text("First: %p", CurrentRoom->m_ActiveInstances.m_First);
            ImGui::Separator();
            ImGui::Text("Last: %p", CurrentRoom->m_ActiveInstances.m_Last);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode(&CurrentRoom->m_InactiveInstances, "m_InactiveInstances - %d", CurrentRoom->m_InactiveInstances.m_Count)) 
        {
            ImGui::Text("Pointer: %p", &CurrentRoom->m_InactiveInstances);
            ImGui::Separator();
            ImGui::Text("Count: %d", CurrentRoom->m_InactiveInstances.m_Count);
            ImGui::Separator();
            ImGui::Text("First: %p", CurrentRoom->m_InactiveInstances.m_First);
            ImGui::Separator();
            ImGui::Text("Last: %p", CurrentRoom->m_InactiveInstances.m_Last);
            ImGui::Separator();
            ImGui::Text("DeleteType: %p", CurrentRoom->m_InactiveInstances.m_DeleteType);
            ImGui::TreePop();
        }
        ImGui::Text("m_MarkedFirst: %p", CurrentRoom->m_MarkedFirst);
        ImGui::Separator();
        ImGui::Text("m_MarkedLast: %p", CurrentRoom->m_MarkedLast);
        ImGui::Separator();
        ImGui::Text("m_CreationOrderList: %p", CurrentRoom->m_CreationOrderList);
        ImGui::Separator();
        ImGui::Text("m_CreationOrderListSize: %d", CurrentRoom->m_CreationOrderListSize);
        ImGui::Separator();
        ImGui::Text("m_WadRoom: %p", CurrentRoom->m_WadRoom);
        ImGui::Separator();
        ImGui::Text("m_WadBaseAddress: %u", CurrentRoom->m_WadBaseAddress);
        ImGui::Separator();
        ImGui::Text("m_PhysicsWorld: %p", CurrentRoom->m_PhysicsWorld);
        ImGui::Separator();
        ImGui::Text("m_TileCount: %d", CurrentRoom->m_TileCount);
        ImGui::Separator();
        ImGui::Text("m_Tiles: %d", CurrentRoom->m_Tiles.Length);
        ImGui::Separator();
        ImGui::Text("m_WadTiles: %p", CurrentRoom->m_WadTiles);
        ImGui::Separator();
        ImGui::Text("m_WadInstances: %p", CurrentRoom->m_WadInstances);
        ImGui::Separator();
        ImGui::Text("m_Name: %s", CurrentRoom->m_Name);
        ImGui::Separator();
        ImGui::Selectable("##isDuplicate", ImGui::IsItemHovered(), ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_Highlight | ImGuiSelectableFlags_SpanAllColumns);
        ImGui::SameLine(0,0);
        ImGui::Text("m_IsDuplicate: %s", CurrentRoom->m_IsDuplicate ? "true" : "false");if (ImGui::BeginPopupContextItem("PhysicsWorldOptions"))
        {
            ImGui::Text("4 bytes (pointer) %p", (void*) (static_cast<int>(CurrentRoom->m_IsDuplicate)));
        }
        if (ImGui::TreeNode(&CurrentRoom->m_Layers, "m_Layers - %d", CurrentRoom->m_Layers.m_Count)) 
        {
            ImGui::Text("Pointer: %p", &CurrentRoom->m_Layers);
            ImGui::Separator();
            ImGui::Text("Count: %d", CurrentRoom->m_Layers.m_Count);
            ImGui::Separator();
            ImGui::Text("First: %p", CurrentRoom->m_Layers.m_First);
            ImGui::Separator();
            ImGui::Text("Last: %p", CurrentRoom->m_Layers.m_Last);
            ImGui::Separator();
            ImGui::Text("DeleteType: %d", CurrentRoom->m_Layers.m_DeleteType);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode(&CurrentRoom->m_LayerLookup, "m_LayerLookup - %d", CurrentRoom->m_LayerLookup.m_UsedCount)) 
        {
            ImGui::Text("Pointer: %p", &CurrentRoom->m_LayerLookup);
            ImGui::Separator();
            ImGui::Text("CurrentSize: %d", CurrentRoom->m_LayerLookup.m_CurrentSize);
            ImGui::Separator();
            ImGui::Text("CurrentMask: %d", CurrentRoom->m_LayerLookup.m_CurrentMask);
            ImGui::Separator();
            ImGui::Text("UsedCount: %p", CurrentRoom->m_LayerLookup.m_UsedCount);
            ImGui::Separator();
            ImGui::Text("GrowThreshold: %p", CurrentRoom->m_LayerLookup.m_GrowThreshold);
            ImGui::Separator();
            ImGui::Text("DeleteValue: %d", CurrentRoom->m_LayerLookup.m_DeleteValue);
            if (ImGui::TreeNode("Elements"))
            {
                for (int i = 0; i < CurrentRoom->m_LayerLookup.m_CurrentSize; i++)
                {
                    CHashMap<int, CLayer*>::CHashMapElement element = CurrentRoom->m_LayerLookup.m_Elements[i];
                    ImGui::Text("%s - ID: %d, Depth: %d", element.value->m_Name, element.value->m_Id, element.value->m_Depth);
                    ImGui::Separator();
                }
            }
            ImGui::TreePop();
        }
        if (ImGui::TreeNode(&CurrentRoom->m_LayerElementLookup, "m_LayerElementLookup - %d", CurrentRoom->m_LayerElementLookup.m_UsedCount)) 
        {
            ImGui::Text("Pointer: %p", &CurrentRoom->m_LayerElementLookup);
            ImGui::Separator();
            ImGui::Text("CurrentSize: %d", CurrentRoom->m_LayerElementLookup.m_CurrentSize);
            ImGui::Separator();
            ImGui::Text("CurrentMask: %d", CurrentRoom->m_LayerElementLookup.m_CurrentMask);
            ImGui::Separator();
            ImGui::Text("UsedCount: %p", CurrentRoom->m_LayerElementLookup.m_UsedCount);
            ImGui::Separator();
            ImGui::Text("GrowThreshold: %p", CurrentRoom->m_LayerElementLookup.m_GrowThreshold);
            ImGui::Separator();
            ImGui::Text("DeleteValue: %d", CurrentRoom->m_LayerElementLookup.m_DeleteValue);
            if (ImGui::TreeNode("Elements"))
            {
                for (int i = 0; i < CurrentRoom->m_LayerElementLookup.m_CurrentSize; i++)
                {
                    CHashMap<int, CLayerElementBase*>::CHashMapElement element = CurrentRoom->m_LayerElementLookup.m_Elements[i];
                    ImGui::Text("%s - ID: %d, Type: %d", element.value->m_Name, element.value->m_ID, element.value->m_Type);
                    ImGui::Separator();
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
        ImGui::Text("m_LastLayerElementLookedUp %d", CurrentRoom->m_LastLayerElementLookedUp ? CurrentRoom->m_LastLayerElementLookedUp->m_ID : -1);
        if (ImGui::TreeNode(&CurrentRoom->m_InstanceElementLookup, "m_InstanceElementLookup - %d", CurrentRoom->m_InstanceElementLookup.m_UsedCount)) 
        {
            ImGui::Text("Pointer: %p", &CurrentRoom->m_InstanceElementLookup);
            ImGui::Separator();
            ImGui::Text("CurrentSize: %d", CurrentRoom->m_InstanceElementLookup.m_CurrentSize);
            ImGui::Separator();
            ImGui::Text("CurrentMask: %d", CurrentRoom->m_InstanceElementLookup.m_CurrentMask);
            ImGui::Separator();
            ImGui::Text("UsedCount: %p", CurrentRoom->m_InstanceElementLookup.m_UsedCount);
            ImGui::Separator();
            ImGui::Text("GrowThreshold: %p", CurrentRoom->m_InstanceElementLookup.m_GrowThreshold);
            ImGui::Separator();
            ImGui::Text("DeleteValue: %d", CurrentRoom->m_InstanceElementLookup.m_DeleteValue);
            if (ImGui::TreeNode("Elements"))
            {
                for (int i = 0; i < CurrentRoom->m_InstanceElementLookup.m_CurrentSize; i++)
                {
                    YYTK::CLayerInstanceElement* element = CurrentRoom->m_InstanceElementLookup.m_Elements[i].value;
                    ImGui::Text("%s - ID: %d, Layer: %d", element->m_Instance->m_Object->m_Name, element->m_Instance->m_ID, element->m_Instance->m_LayerID);
                    ImGui::Separator();
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
        ImGui::Text("m_SeqInstances: %p", CurrentRoom->m_SeqInstances);
        ImGui::Separator();
        ImGui::Text("m_SeqInstancesCount: %d", CurrentRoom->m_SeqInstancesCount);
        ImGui::Separator();
        ImGui::Text("m_SeqInstancesMax: %d", CurrentRoom->m_SeqInstancesMax);
        ImGui::Separator();
        ImGui::Text("m_EffectLayerIDs: %p", CurrentRoom->m_EffectLayerIDs);
        ImGui::Separator();
        ImGui::Text("m_EffectLayerIdCount: %d", CurrentRoom->m_EffectLayerIdCount);
        ImGui::Separator();
        ImGui::Text("m_EffectLayerIdMax: %d", CurrentRoom->m_EffectLayerIdMax);
        ImGui::EndChild();
    }

    ImGui::EndChild();
}