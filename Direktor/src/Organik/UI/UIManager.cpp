#include "Synthetik.h"
#include "Logging.h"
#include "zhl.h"
#include "zhl_internal.h"
#include "UIManager.h"
#include "MainMenu.h"
#include "Logging.h"
#include "DefinitionHelpers/BuiltinHelper.h"
#include "DefinitionHelpers/InstanceHelper.h"
#include "VariableViewer.h"
#include "DefinitionHelpers/VariableHelper.h"
#include "UIElement.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "Utils.h"

static bool m_AwaitingLateStartup = false;
namespace Organik 
{
    UIManager* g_UIManager = nullptr;
    RValue* g_monoFonto = nullptr;
    static int frameCounter = 0;
    std::map<std::string, std::tuple<std::string, std::function<Organik::UIElement*()>, std::function<Organik::UIElement*()>>> UIManager::windowTypes;
    HWND UIManager::g_hWnd = nullptr;
    UIManager* UIManager::GetInstance()
    {
        if (!g_UIManager)
        {
            g_UIManager = new UIManager();
            UIManager::windowTypes.insert({
            //     WINDOW_TYPE(RoomViewer, "Ctrl+F10"),
            //     WINDOW_TYPE(BugReportWindow, "Ctrl+Alt+C"),
                   WINDOW_TYPE(InstanceVariableViewer, "Ctrl+F11"),
            //     WINDOW_TYPE(OrganikConsole, "Ctrl+Alt+T"),
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
        GetInstance();
    }

    UIManager::~UIManager()
    {
        for (const auto& [hash, element] : elements)
        {
            delete element;
        }
        elements.clear();
    }

    void UIManager::Shutdown()
    {
        if (g_UIManager)
        {
            delete g_UIManager;
            g_UIManager = nullptr;
        }
    }
}

bool doImGuiInit(ID3D11Device* d3d11device, ID3D11DeviceContext* d3d11context)
{
    Organik::GetLogger()->LogFormatted("Initializing ImGui... (d3d pointers): %p, %p ", d3d11device, d3d11context);
    if (!d3d11device || !d3d11context)
    {
        Organik::GetLogger()->LogSimple("D3D11 device or context is not set.");
        return false;
    }
    if (!Organik::UIManager::isImGuiInitialized())
    {        
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
        if (!ImGui_ImplWin32_Init(Organik::UIManager::g_hWnd))
            Error_Show_Action(const_cast<char*>("ImGui DX11 initialization failed."), true, true);

        if(!ImGui_ImplDX11_Init(d3d11device, d3d11context))
            Error_Show_Action(const_cast<char*>("ImGui DX11 initialization failed."), true, true);
        Organik::UIManager::isImGuiInitialized(true);
        
        // t.detach();
        Organik::GetLogger()->LogSimple("ImGui setup context grabbed successfully");
    }
    return Organik::UIManager::isImGuiInitialized();
}
HOOK_GLOBAL(GR_D3D_Start_Frame, (void) -> bool)
{
    if(ImGui::GetCurrentContext() == nullptr)
        return super();
    
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    return super();
}
int frameCount = 0;
HOOK_GLOBAL(GR_D3D_Finish_Frame, (bool present) -> bool)
{
    if (!Organik::UIManager::isImGuiInitialized())
    {
        if (!present)
            return super(present);
        
        if (frameCount == 0)
        {
            Organik::UIManager::Initialize();
            Organik::DoHelperSetup();
        }
        
        if (frameCount != 10)
        {
            frameCount++;
//             Organik::GetLogger()->LogFormatted("Waiting for ImGui to be initialized. Frame count: %d", frameCount);
            return super(present);
        }
//         Organik::GetLogger()->LogSimple("Waiting for context to be ready...");
        Organik::GetLogger()->LogSimple("Checking window handle.");
        Organik::UIManager::g_hWnd = GetWindowHandle();
        if (Organik::UIManager::g_hWnd == NULL)
        {
            Organik::GetLogger()->LogSimple("Window handle is NULL, waiting for it to be set.");
            return super(present);
        }
        
//         Organik::GetLogger()->LogSimple("Window is not minimized.");
        RValue infoDSMap = RValue(-4ll); 
        DoBuiltinRef(&gml_os_get_info, infoDSMap, {});
        int infoDSMapID = infoDSMap.ToInt32();

        RValue videod3ddevice = RValue();
        RValue videod3dcontext = RValue();
        YYCreateString(&videod3ddevice, const_cast<char*>("video_d3d11_device"));
        YYCreateString(&videod3dcontext, const_cast<char*>("video_d3d11_context"));

        RValue d3d11deviceRV;
        RValue d3d11contextRV;
        DoBuiltinRef(&gml_ds_map_find_value, d3d11deviceRV, { infoDSMap, videod3ddevice });
        DoBuiltinRef(&gml_ds_map_find_value, d3d11contextRV, { infoDSMap,  videod3dcontext });
        ID3D11Device* d3d11device = reinterpret_cast<ID3D11Device*>(d3d11deviceRV.ToPointer());
        ID3D11DeviceContext* d3d11context = reinterpret_cast<ID3D11DeviceContext*>(d3d11contextRV.ToPointer());
        
        Organik::GetLogger()->LogFormatted("D3D11 RValues: 0x%p, 0x%p", (d3d11device), (d3d11context));
        if (!d3d11device || !d3d11context)
        {
// 	        Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
            Error_Show_Action(const_cast<char*>("D3D device or context is not set in the DS map."), true, true);
        }
// 	    Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
        doImGuiInit(d3d11device, d3d11context);
// 	    Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
        bool ret = super(present);
        return super(present);
    }
    
    if(ImGui::GetCurrentContext() == nullptr)
    {
//         Organik::GetLogger()->LogSimple("ImGui context is null.");
        return super(present);
    }
    UIManager::GetInstance()->DrawEvent();
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    
    bool ret = super(present);
//     GetLogger()->LogFormatted("D3D frame finished. returned %s", (ret ? "true" : "false"));
    return ret;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static bool isFirstPresent = true;
HOOK_GLOBAL(WndProc, (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT)
{
// 	Organik::GetLogger()->LogFormatted("WndProc called with message: %d", msg);

    if (!Organik::g_UIManager)
        return super(hWnd, msg, wParam, lParam);

    if(ImGui::GetCurrentContext() == nullptr)
    {
//         Organik::GetLogger()->LogSimple("ImGui context is null.");
        return super(hWnd,msg,wParam,lParam);
    }

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
        return imgui_result;
    }

    // Call the original WndProc
    return super(hWnd, msg, wParam, lParam);
}

HOOK_STATIC(Graphics, OpenWindow, (HWND hWnd, int width, int height, int screenMode) -> bool)
{
	Organik::GetLogger()->LogFormatted("OpenWindow called with hWnd: %p, width: %d, height: %d, screenMode: %d", hWnd, width, height, screenMode);
    bool result = super(hWnd, width, height, screenMode);
    Organik::GetLogger()->LogFormatted("OpenWindow super called, result: %s", (result ? "true" : "false"));

    return result;
}

HOOK_GLOBAL(gml_window_set_fullscreen, (RValue* Result, CInstance* Self, CInstance* Other, int ArgumentCount, RValue* Arguments) -> void)
{
    Organik::GetLogger()->LogSimple("gml_window_set_fullscreen called");
    //when this function gets called, we know that our Window, D3D device and context are ready
    super(Result, Self, Other, ArgumentCount, Arguments);
    Organik::GetLogger()->LogSimple("gml_window_set_fullscreen super called");

}