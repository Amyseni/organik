#include "Synthetik.h"

#include "zhl.h"
#include "zhl_internal.h"
#include "UIManager.h"
#include "MainMenu.h"
#include "ChatBox.h"
#include "Action.h"
#include "Globals.h"
#include "DefinitionHelpers/InstanceHelper.h"
#include "VariableViewer.h"
#include "DefinitionHelpers/VariableHelper.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "Variant.h"

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
            if (element && hash)
            {
                bool wantsToStayOpen = true;
                char name[256];
                element->GetName(name);
                element->Draw(mousedOver, &wantsToStayOpen, name);
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
//     Organik::GetLogger()->LogFormatted("Initializing ImGui... (d3d pointers): %p, %p ", d3d11device, d3d11context);
    if (!d3d11device || !d3d11context)
    {
//         Organik::GetLogger()->LogSimple("D3D11 device or context is not set.");
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
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        if (!ImGui_ImplWin32_Init(Organik::UIManager::g_hWnd))
            Error_Show_Action("ImGui DX11 initialization failed.", true, true);

        if(!ImGui_ImplDX11_Init(d3d11device, d3d11context))
            Error_Show_Action("ImGui DX11 initialization failed.", true, true);
        Organik::UIManager::isImGuiInitialized(true);
        
        // t.detach();
//         Organik::GetLogger()->LogSimple("ImGui setup context grabbed successfully");
        UIManager::GetInstance()->SetConsoleFont(ImGui::GetIO().Fonts->AddFontFromFileTTF("./Inconsolata.ttf", 16.0f));

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
bool loggingInit = false;
HOOK_GLOBAL(GR_D3D_Finish_Frame, (bool present) -> bool)
{
    
    if (!Organik::UIManager::isImGuiInitialized())
    {
        if (!present)
            return super(present);
        
        if (frameCount == 0)
        {
            Organik::UIManager::Initialize();
        }
        
        if (frameCount != 10)
        {
            frameCount++;
//             Organik::GetLogger()->LogFormatted("Waiting for ImGui to be initialized. Frame count: %d", frameCount);
            return super(present);
        }
//         Organik::GetLogger()->LogSimple("Waiting for context to be ready...");
//         Organik::GetLogger()->LogSimple("Checking window handle.");
        Organik::UIManager::g_hWnd = GetWindowHandle();
        if (Organik::UIManager::g_hWnd == NULL)
        {
//             Organik::GetLogger()->LogSimple("Window handle is NULL, waiting for it to be set.");
            return super(present);
        }
        
//         Organik::GetLogger()->LogSimple("Window is not minimized.");
        RValue infoDSMap = RValue(-4ll); 
        DoBuiltinRef(&gml_os_get_info, infoDSMap, {});
        int infoDSMapID = infoDSMap.ToInt32();

        RValue videod3ddevice = RValue();
        RValue videod3dcontext = RValue();
        YYCreateString(&videod3ddevice, "video_d3d11_device");
        YYCreateString(&videod3dcontext, "video_d3d11_context");

        RValue d3d11deviceRV;
        RValue d3d11contextRV;
        DoBuiltinRef(&gml_ds_map_find_value, d3d11deviceRV, { infoDSMap, videod3ddevice });
        DoBuiltinRef(&gml_ds_map_find_value, d3d11contextRV, { infoDSMap,  videod3dcontext });
        ID3D11Device* d3d11device = reinterpret_cast<ID3D11Device*>(d3d11deviceRV.ToPointer());
        ID3D11DeviceContext* d3d11context = reinterpret_cast<ID3D11DeviceContext*>(d3d11contextRV.ToPointer());
        
//         Organik::GetLogger()->LogFormatted("D3D11 RValues: 0x%p, 0x%p", (d3d11device), (d3d11context));
        if (!d3d11device || !d3d11context)
        {
// 	        Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
            Error_Show_Action("D3D device or context is not set in the DS map.", true, true);
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
    if (g_chatOpenCloseDelay)
        g_chatOpenCloseDelay--;

    ImGui::DockSpaceOverViewport(ImGui::GetID("MyDockspace"),ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
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
            if(wParam == VK_RETURN && !(g_chatOpenCloseDelay > 0) && !io.WantCaptureKeyboard)
            {
                if (!Organik::ChatBox::g_ShowGlobalChat)
                {
                    Organik::ChatBox::g_ShowGlobalChat = true;
                    g_chatOpenCloseDelay = 10; 
                }
                if (Organik::ChatBox::g_ShowGlobalChat) Organik::ChatBox::g_GrabInput = true;
                imgui_wants_input = Organik::ChatBox::g_ShowGlobalChat;
                return imgui_result;
            }
            else if (wParam == VK_ESCAPE)
            {
                if (Organik::ChatBox::g_ShowGlobalChat)
                {
                    Organik::ChatBox::g_ShowGlobalChat = false;
                }
            }
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
    bool result = super(hWnd, width, height, screenMode);

    return result;
}
HOOK_GLOBAL(gml_window_set_fullscreen, (RValue* Result, CInstance* Self, CInstance* Other, int ArgumentCount, RValue* Arguments) -> void)
{
    if (!loggingInit)
    {
        
        loggingInit = true;
        Organik::DoHelperSetup();
//         Organik::GetLogger()->LogSimple("gml_window_set_fullscreen called, initializing action handlers and variants.");
        // RegisterActionTypeHandler(Code_Variable_FindAlloc_Slot_From_Name(nullptr, "apply_variables"), new DoAction([&](CInstance* self, CInstance* other, VariableMap* map, RValue* out) ->bool {
        //     return doApplyVariables(self, other, map, out);
        // }));
        // RegisterActionTypeHandler(Code_Variable_FindAlloc_Slot_From_Name(nullptr, "call_script"), new DoAction([&](CInstance* self, CInstance* other, VariableMap* map, RValue* out) ->bool {
        //     return doCallScript(self, other, map, out);
        // }));
        // RegisterActionTypeHandler(Code_Variable_FindAlloc_Slot_From_Name(nullptr, "call_builtin"), new DoAction([&](CInstance* self, CInstance* other, VariableMap* map, RValue* out) ->bool {
        //     return doCallBuiltin(self, other, map, out);
        // }));
        // RegisterActionTypeHandler(Code_Variable_FindAlloc_Slot_From_Name(nullptr, "requires_variables"), new DoAction([&](CInstance* self, CInstance* other, VariableMap* map, RValue* out) ->bool {
        //     return doRequiresVars(self, other, map, out);
        // }));
        // RegisterActionTypeHandler(Code_Variable_FindAlloc_Slot_From_Name(nullptr, "rng_chance"), new DoAction([&](CInstance* self, CInstance* other, VariableMap* map, RValue* out) ->bool {
        //     return doRequiresChance(self, other, map, out);
        // }));
        // RegisterActionTypeHandler(Code_Variable_FindAlloc_Slot_From_Name(nullptr, "variable_get"), new DoAction([&](CInstance* self, CInstance* other, VariableMap* map, RValue* out) ->bool {
        //     return doGetFrom(self, other, map, out);
        // }));
        // RegisterActionTypeHandler(Code_Variable_FindAlloc_Slot_From_Name(nullptr, "create_instance"), new DoAction([&](CInstance* self, CInstance* other, VariableMap* map, RValue* out) ->bool {
        //     RValue outR;
        //     std::vector<RValue*> args;
        //     args.push_back(&(*map)[VAR_HASH(x)]);
        //     args.push_back(&(*map)[VAR_HASH(y)]);
        //     args.push_back(&(*map)[VAR_HASH(object)]);
        //     return gml_Script_scr_instance_create(self, other, &outR, static_cast<int>(args.size()), args.data());
        // }));

        auto pwd = std::filesystem::current_path();
        if (!std::filesystem::exists(pwd / "Mods"))
        {
            std::filesystem::create_directory(pwd / "Mods");
        }
        auto modsFldr = pwd / "Mods";
        if (!std::filesystem::exists(modsFldr / "Variants"))
        {
            std::filesystem::create_directory(modsFldr / "Variants");
        }
        auto it = std::filesystem::directory_iterator(modsFldr / "Variants");
        for (const auto& entry : it)
        {
            if (entry.is_directory() && std::filesystem::exists(entry.path() / "variant.json"))
            {
                RValue variantObject;
                {
                    std::ifstream inFile((entry.path() / "variant.json").generic_string());
                    std::ostringstream strStream;
                    std::string line;
                    while (std::getline(inFile, line))
                    {
                        strStream << line << "\n";
                    }
                    RValue fileContentsRV;
                    YYCreateString(&fileContentsRV, strStream.str().c_str());
                    variantObject = DoBuiltin(&gml_json_parse, { fileContentsRV });
                    if (variantObject.m_Kind != VALUE_OBJECT && variantObject.m_Kind != VALUE_ARRAY)
                    {
                        Error_Show_Action("Failed to parse variant.json file.", true, true);
                        //! program terminates
                    }
                }
                Organik::GetLogger()->LogFormatted("Loading variant from %s", entry.path().generic_string().c_str());
                
                {
                    Organik::GetLogger()->LogFormatted("Variant object kind: %d", variantObject.m_Kind);
                    auto UserVariants = (getUserVariants());
                    if (variantObject.m_Kind == VALUE_ARRAY)
                    {

                        Organik::GetLogger()->LogFormatted("Loading variants from array, current size: %d", (int) UserVariants->size());
                        auto arr = variantObject.ToRefVector();
                        int32_t count = variantObject.GetArrayLength();
                        for (int i = 0; i < count; i++)
                        {
                            auto item = arr[i];
                            if (!item) break;

                            Organik::GetLogger()->LogFormatted("Checking item kind: %d", item->GetKind());
                            if (item->GetKind() == VALUE_OBJECT)
                            {
                                Variant::LoadVariant(*item, UserVariants);
                            }
                        }
                    }
                    else if (variantObject.m_Kind == VALUE_OBJECT)
                    {

                        Organik::GetLogger()->LogFormatted("Loading variant from: %s", (entry.path() / "variant.json").parent_path().generic_string().c_str());
                        Variant::LoadVariant(variantObject, UserVariants);
                    }
                }
                
                
                // if (triggers && triggers->m_Kind == VALUE_ARRAY) {
                //     Organik::GetLogger()->LogFormatted("Found %d triggers for variant %s", (int) triggers->GetArrayLength(), newVariant.name.c_str());
                //     Organik::Actions::ActionManager::ParseTriggers(triggers);
                // }
                    
            }
        }
    }
    //when this function gets called, we know that our Window, D3D device and context are ready
    super(Result, Self, Other, ArgumentCount, Arguments);
}

int32_t Variant::LoadVariant(const RValue& variantObject, std::unordered_map<int32_t, Variant>*& variantList)
{
    int32_t _hash = 0;
    if (!variantList)
        variantList = getUserVariants();
    
    YYObjectBase* convObj = (YYObjectBase*) variantObject.ToPointer();
    RValue *meta = convObj->InternalReadYYVar(
        Code_Variable_FindAlloc_Slot_From_Name(
				nullptr, const_cast<char*>("meta")
        )
    );
    RValue *statMultipliers = convObj->InternalReadYYVar(
        Code_Variable_FindAlloc_Slot_From_Name(
				nullptr, const_cast<char*>("stat_multipliers")
        )
    );
    RValue *statIncreases = convObj->InternalReadYYVar(
        Code_Variable_FindAlloc_Slot_From_Name(
				nullptr, const_cast<char*>("stat_increases")
        )
    );
    Variant* newVariantPtr = nullptr;
    if (meta)
    {
        if (!(meta->GetKind() == VALUE_OBJECT))
        {
            GetLogger()->LogFormatted("Variant meta is not an object, got kind %d", meta->GetKind());
            Error_Show_Action("Variant meta must be an object.", true, true);
            return -1;
        }
        Organik::GetLogger()->LogSimple("Loading variant metadata...");
        YYObjectBase* metaObj = meta->ToObject();
        Organik::GetLogger()->LogSimple("Loading variant metadata...");
        RValue *replaceVariantRV = metaObj->InternalReadYYVar(
            Code_Variable_FindAlloc_Slot_From_Name(
				nullptr, const_cast<char*>("replace_variant")
            )
        );
        RValue *nameRV = metaObj->InternalReadYYVar(
            Code_Variable_FindAlloc_Slot_From_Name(
				nullptr, const_cast<char*>("name")
            )
        );
        int32_t variantHash = 0;
        {
            if (!nameRV)
            {
                GetLogger()->LogFormatted("Variant in %p is missing a name.", (void*)metaObj);
                Error_Show_Action("Variant must have a name.", true, true);
                return -1;
            }
            std::string name = nameRV->ToCString();
            if (nameRV)
            {
                name = (nameRV->ToCString());
            }
            if (replaceVariantRV)
            {
                variantHash = parseRValueNumber<int32_t>(replaceVariantRV);
            }
            if (!variantHash)
            {
                variantHash = Code_Variable_FindAlloc_Slot_From_Name(nullptr, name.data());
            }

            if (variantHash >= 0)
            {
                (*variantList)[variantHash] = Variant();
                newVariantPtr = &((*variantList)[variantHash]);
            }
            else
            {
                    Error_Show_Action("Variant name hash is invalid.", true, true);
                    return -1;
            }
        }
        Organik::GetLogger()->LogFormatted("Creating variant with hash %d", variantHash);
        if (!newVariantPtr)
        {
            Error_Show_Action("Failed to create variant.", true, true);
            return 0;
        }
        RValue *nameshortRV = metaObj->InternalReadYYVar(
            Code_Variable_FindAlloc_Slot_From_Name(
				nullptr, const_cast<char*>("name_short")
            )
        );
        RValue *descRV = metaObj->InternalReadYYVar(
            Code_Variable_FindAlloc_Slot_From_Name(
				nullptr, const_cast<char*>("desc")
            )
        );
        RValue *manuRV = metaObj->InternalReadYYVar(
            Code_Variable_FindAlloc_Slot_From_Name(
				nullptr, const_cast<char*>("manufacturer")
            )
        );
        
        RValue *colorRV = metaObj->InternalReadYYVar(
            Code_Variable_FindAlloc_Slot_From_Name(
				nullptr, const_cast<char*>("color")
            )
        );
        RValue *rarityRV = metaObj->InternalReadYYVar(
            Code_Variable_FindAlloc_Slot_From_Name(
				nullptr, const_cast<char*>("rarity")
            )
        );
        RValue *iconRV = metaObj->InternalReadYYVar(
            Code_Variable_FindAlloc_Slot_From_Name(
				nullptr, const_cast<char*>("icon")
            )
        );
        RValue *soundRV = metaObj->InternalReadYYVar(
            Code_Variable_FindAlloc_Slot_From_Name(
				nullptr, const_cast<char*>("sound")
            )
        );
        RValue *spawnRV = metaObj->InternalReadYYVar(
            Code_Variable_FindAlloc_Slot_From_Name(
				nullptr, const_cast<char*>("spawn")
            )
        );
        std::string name = "ORGANIK";
        std::string nameshort = "ORG";
        std::string desc = "A fictional GUN product.";
        std::string manufacturer = "ORGANIK Corp.";
        
        if (nameRV)
        {
            name = std::string(nameRV->ToCString());
        }
        if (nameshortRV)
        {
            nameshort = std::string(nameshortRV->ToCString());
        }
        if (descRV)
        {
            if (descRV->GetKind() == VALUE_STRING)
            {
                desc = std::string(descRV->ToCString());
            }
            else if (descRV->GetKind() == VALUE_ARRAY)
            {
                auto descLines = descRV->ToRefVector();
                desc.clear();
                desc.reserve(VARIANT_DESC_MAX_LENGTH);
                for (const auto& line : descLines)
                {
                    desc += line->ToCString();
                    desc += "\n";
                    if (desc.length() > VARIANT_DESC_MAX_LENGTH)
                        break;
                }
            }
        }
        if (manuRV)
        {
            manufacturer = std::string(manuRV->ToCString());
        }
        if (iconRV)
        {
            int32_t spriteID = DoBuiltin(&gml_asset_get_index, { *iconRV }).ToInt32();
            newVariantPtr->spriteID = static_cast<double>(spriteID);
        }
        else
        {
            newVariantPtr->spriteID = -1;
        }
        Organik::GetLogger()->LogFormatted("Loaded variant icon: %f", newVariantPtr->spriteID);
        if (replaceVariantRV)
        {
            newVariantPtr->variantHash = parseRValueNumber<int32_t>(replaceVariantRV);
        }
        else
        {
            newVariantPtr->variantHash = Code_Variable_FindAlloc_Slot_From_Name(nullptr, const_cast<char*>(newVariantPtr->name.c_str()));
        }
        std::string fileContents;
        if (soundRV)
        {
            int32_t soundID = DoBuiltin(&gml_asset_get_index, { *soundRV }).ToInt32();
            newVariantPtr->soundID = static_cast<double>(soundID);
        }
        else
        {
            newVariantPtr->soundID = -1;
        }
                Organik::GetLogger()->LogFormatted("Loaded variant sound: %f", newVariantPtr->soundID);
        if (spawnRV)
        {
            int32_t spawnID = DoBuiltin(&gml_asset_get_index, { *spawnRV }).ToInt32();
            newVariantPtr->spawnID = static_cast<double>(spawnID);
        }
        else
        {
            newVariantPtr->spawnID = -1;
        }
        Organik::GetLogger()->LogFormatted("Loaded variant spawn: %f", newVariantPtr->spawnID);
        if (rarityRV)
        {
            newVariantPtr->rarity = parseRValueNumber<double>(rarityRV);
        }
        else
        {
            newVariantPtr->rarity = Rarities::COMMON;
        }
        Organik::GetLogger()->LogFormatted("Loaded variant rarity: %f", newVariantPtr->rarity);
        if (colorRV)
        {
            newVariantPtr->color = parseRValueNumber<double>(colorRV);
        }
        else
        {
            newVariantPtr->color = Rarities::RarityColor[static_cast<int32_t>(newVariantPtr->rarity)];
        }
        
        newVariantPtr->name = name;
        newVariantPtr->desc = desc;
        newVariantPtr->manufacturer = manufacturer;
        Organik::GetLogger()->LogFormatted("Loaded variant: %s -- %s, %s, %f %f %f", name.c_str(), desc.c_str(), manufacturer.c_str(), newVariantPtr->rarity, newVariantPtr->color, newVariantPtr->spriteID);
    }
    
    if (statMultipliers)
    {
        YYObjectBase* statMultipliersObj = (YYObjectBase*)statMultipliers->ToPointer();
        RValue names = DoBuiltin(&gml_variable_instance_get_names, { *statMultipliers });
        auto nameList = names.ToRefVector();
        for (const auto& name : nameList)
        {
            int32_t hash = Code_Variable_FindAlloc_Slot_From_Name(nullptr, const_cast<char*>(name->ToCString()));
            double value = parseRValueNumber<double>(statMultipliersObj->InternalGetYYVarRef(hash));
            newVariantPtr->statMultipliers[hash] = value;
            Organik::GetLogger()->LogFormatted("  Stat multiplier: %s -- %f", name->ToCString(), value);
        }
    }
    
    if (statIncreases)
    {
        YYObjectBase* statIncreasesObj = (YYObjectBase*)statIncreases->ToPointer();
        RValue names = DoBuiltin(&gml_variable_instance_get_names, { *statIncreases });
        auto nameList = names.ToRefVector();
        for (const auto& name : nameList)
        {
            int32_t hash = Code_Variable_FindAlloc_Slot_From_Name(nullptr, const_cast<char*>(name->ToCString()));
            double value = parseRValueNumber<double>(statIncreasesObj->InternalGetYYVarRef(hash));
            newVariantPtr->statIncreases[hash] = value;
            Organik::GetLogger()->LogFormatted("  Stat increase: %s -- %f", name->ToCString(), value);
        }
    }
    
    RValue* triggers = convObj->InternalReadYYVar(
        Code_Variable_FindAlloc_Slot_From_Name(
				nullptr, const_cast<char*>("triggers")
        )
    );
    return newVariantPtr->variantHash;
}