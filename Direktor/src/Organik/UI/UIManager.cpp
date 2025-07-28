#include "Synthetik.h"
#include "Logging.h"
#include "zhl.h"
#include "zhl_internal.h"
#include "UIManager.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "Utils.h"
HWND g_hWnd = nullptr;

namespace Organik 
{
    UIManager* g_UIManager = nullptr;
    RValue* g_monoFonto = nullptr;
    
    static int frameCounter = 0;
    std::map<std::string, std::tuple<std::string, std::function<Organik::UIElement*()>, std::function<Organik::UIElement*()>>> UIManager::windowTypes;

    UIManager* UIManager::GetInstance()
    {
        if (!g_UIManager)
        {
            g_UIManager = new UIManager();
            UIManager::windowTypes.insert({
                WINDOW_TYPE(RoomViewer, "Ctrl+F10"),
                WINDOW_TYPE(BugReportWindow, "Ctrl+Alt+C"),
                WINDOW_TYPE(InstanceVariableViewer, "Ctrl+F11"),
                WINDOW_TYPE(OrganikConsole, "Ctrl+Alt+T"),
            });
            g_UIManager->AddElement<MainMenu>("Main Menu");
        }
        return g_UIManager;
    }

    void UIManager::DrawEvent()
    {
        bool mousedOver = false;
        std::vector<size_t> toDelete;
        
        for (const auto& [hash, element] : elements)
        {
            if (element)
            {
                bool wantsToStayOpen = true;
                element->Draw(mousedOver, &wantsToStayOpen, element->name);
                if (!wantsToStayOpen)
                {
                    toDelete.push_back(hash);
                }
            }
        }
        if (mousedOver)
        {
            m_wasItemHoveredLastFrame = true;
        }
        else
        {
            m_wasItemHoveredLastFrame = false;
        }
        
        for (size_t elementHash : toDelete)
        {
            delete elements[elementHash];
            elements.erase(elementHash);
        }
    }

    bool UIManager::isAnyItemHovered()
    {
        return m_wasItemHoveredLastFrame;
    }

    void UIManager::Initialize() 
    {
        g_UIManager = GetInstance();
    }

    UIManager::~UIManager()
    {
        for (const auto& [hash, element] : elements)
        {
            delete element;
        }
        elements.clear();
    }

    void UIManager::StepEvent()
    {
        
    }
}

HOOK_GLOBAL(GR_D3D_Start_Frame, (void) -> bool)
{
    bool ret = super();
    if (!g_hWnd || !g_pd3dContext || !g_pd3dDevice)
    {
        // we skip tbh
        return ret;
    }
    
    ImGui::NewFrame();
}

HOOK_GLOBAL(GR_D3D_Finish_Frame, (bool param1) -> void)
{
    ImGui::Render();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HOOK_GLOBAL(WndProc, (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT)
{
    // Let ImGui handle the input first
    LRESULT imgui_result = ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
    
    // Check if ImGui wants to capture this input
    ImGuiIO& io = ImGui::GetIO();
    bool imgui_wants_input = false;
    
    switch (msg) {
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_CHAR:
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
            imgui_wants_input = io.WantCaptureKeyboard;
            break;
        case WM_LBUTTONDOWN:
			
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_MOUSEWHEEL:
        case WM_MOUSEMOVE:
			if (ImGui::IsAnyItemHovered() || UIManager::GetInstance()->isAnyItemHovered())
			{
				if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
				   ImGui::GetIO().ConfigFlags ^= ImGuiConfigFlags_NoMouseCursorChange;
				imgui_wants_input = true;
				ImGui::GetIO().MouseDrawCursor = true;
			}
			else 
			{
				if (!(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange))
					ImGui::GetIO().ConfigFlags ^= ImGuiConfigFlags_NoMouseCursorChange;
				ImGui::GetIO().MouseDrawCursor = false;
				imgui_wants_input = io.WantCaptureMouse;
			}
            break;
    }
    if (imgui_wants_input || imgui_result) {
        // ImGui handled it, don't pass to game
        return 0;
    }
    // Call the original WndProc
    return super(hWnd, msg, wParam, lParam);
}

HOOK_GLOBAL(window_set_fullscreen, (RValue* Result, CInstance* Self, CInstance* Other, int ArgumentCount, RValue* Arguments) -> void)
{
    //when this function gets called, we know that our Window, D3D device and context are ready
    super(Result, Self, Other, ArgumentCount, Arguments);
    if (!ImGui::GetCurrentContext())
        std::cout << "Window variables ready for ImGui Setup." << std::endl;

    if (!g_pd3dContext || !g_pd3dDevice)
    {
        std::cerr << "D3D device or context not initialized. Cannot set up ImGui." << std::endl;
        return;
    }
    std::cout << "window_set_fullscreen called. Grabbing ImGui setup context" << std::endl;
    IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

    g_hWnd = (HWND) BuiltInFunction::operator()(&window_handle, {})->m_Pointer;

    if (g_hWnd && g_pd3dContext && g_pd3dDevice)
    {
        ImGui_ImplWin32_Init(g_hWnd);
        ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dContext);
        std::cout << "ImGui setup context grabbed successfully" << std::endl;
    }
    else
    {
        std::cerr << "Failed to grab ImGui setup context" << std::endl;
    }
}