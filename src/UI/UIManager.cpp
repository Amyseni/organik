#include "YYToolkit/YYTK_Shared.hpp"
#include "Aurie/shared.hpp"
#include "../ModuleMain.h"
#include "UIManager.h"
#include "BuiltinCommands.h"
#include "VariableViewer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "../Utils.h"
#include "../obj_list.h"


#define MENU_CLOSED 0
#define MENU_OPENING 1
#define MENU_CLOSING 2
#define MENU_OPEN 3

using namespace Organik;

namespace Organik 
{
    static int frameCounter = 0;
    
    OrganikConsole::OrganikConsole()
    {
        ClearLog();
        InputBuf.clear(); // Changed from memset
        HistoryPos = -1;

        auto commands = BuiltinCommands::GetCommands();
        for (auto& cmd : commands)
        {
            GetLogger()->LogFormatted("Adding command: %s", cmd.first.c_str());
            _commands.insert({ cmd.first, cmd.second });
        }
        AutoScroll = true;
        ScrollToBottom = false;
        AddLog("ORGANIK devtools initialized. What would you like to break today?");
    }
    OrganikConsole::~OrganikConsole()
    {
        ClearLog();
        for (int i = 0; i < History.Size; i++)
            delete History[i];
    }

    // Portable helpers
    static int   Stricmp(const std::string& s1, const std::string& s2)
            {
                if (s1.size() != s2.size())
                    return s1.size() < s2.size() ? -1 : 1;
                return s1._Equal(s2) ? 0 : (s1 < s2 ? -1 : 1);
            }
    static int  Strnicmp(const char* s1, const char* s2, int n) { int d = 0; while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; n--; } return d; }
    static void Strtrim(std::string* s)
    {
        static std::string whitespace = " \t\n\r\f\v"; // space, tab, newline, carriage return, form feed, vertical tab
        size_t start = s->find_first_not_of(whitespace);
        if (start == std::string::npos) {
            s->clear(); // string is all whitespace
            return;
        }
        size_t end = s->find_last_not_of(whitespace);
        *s = s->substr(start, end - start + 1);
    }
    void OrganikConsole::ClearLog()
    {
        for (int i = 0; i < Items.Size; i++)
            delete Items[i];
        Items.clear();
    }
    void OrganikConsole::AddLog(const char* fmt, ...)
    {
        // Determine required size
        va_list args;
        va_start(args, fmt);
        int size = std::vsnprintf(nullptr, 0, fmt, args) + 1;
        va_end(args);
        
        if (size <= 1) return; // Nothing to format
        
        // Format the string
        std::vector<char> buffer(size);
        va_start(args, fmt);
        std::vsnprintf(buffer.data(), size, fmt, args);
        va_end(args);
        
        // Create and store the formatted string
        std::string formatted(buffer.data());
        Strtrim(&formatted);
        
        // Add timestamp if desired
        Items.push_back(new std::string(formatted));
        
        // Auto-scroll
        if (AutoScroll)
            ScrollToBottom = true;
    }

    void OrganikConsole::AddLog(const std::string& message)
    {
        std::string formatted = message;
        Strtrim(&formatted);
        Items.push_back(new std::string(formatted));
        
        if (AutoScroll)
            ScrollToBottom = true;
    }

    void OrganikConsole::Draw(const std::string& title, bool* p_open)
    {
        ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
        if (!ImGui::Begin(title.c_str(), p_open))
        {
            ImGui::End();
            return;
        }

        if (!ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectSize()))
            ImGui::GetIO().WantCaptureMouse = false;
        else
            ImGui::GetIO().WantCaptureMouse = true;
        // As a specific feature guaranteed by the library, after calling Begin() the last Item represent the title bar.
        // So e.g. IsItemHovered() will return true when hovering the title bar.
        // Here we create a context menu only available from the title bar.
        

        ImGui::TextWrapped(
            "For Hacking");
        ImGui::TextWrapped("Enter 'HELP' for help.");

        // TODO: display items starting from the bottom
        if (ImGui::SmallButton("Toggle VariableViewer"))  
        {
               UIManager::GetInstance()->showVariableViewer = !(UIManager::GetInstance()->showVariableViewer);
        }
        ImGui::SameLine();
        if (ImGui::SmallButton("Clear"))           { ClearLog(); }
        ImGui::SameLine();
        bool copy_to_clipboard = ImGui::SmallButton("Copy");
        ImGui::Separator();
        ImGui::Checkbox("26.1 Multiplayer Patch", &Organik::Utils::g_EnableMultiplayerCompat);
        ImGui::SameLine();
        ImGui::Checkbox("Dev Mode (see README)", &Organik::Utils::g_EnableDevcom);
        ImGui::SameLine();
        ImGui::Checkbox("Invincibility", &Organik::Utils::g_EnableInvincibility);
        ImGui::Separator();
        ImGuiInputTextFlags obj_index_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll | ImGuiInputTextFlags_CallbackCompletion;
        if (ImGui::InputText("ObjIndexInput", &ObjInputBuf, obj_index_flags, &TextEditCallbackStub, (void*)this))
        {
            Strtrim(&ObjInputBuf);
            if (!ObjInputBuf.empty() && Organik::Utils::builtinAssetGetIndex(ObjInputBuf.c_str()) > 0)
            {
                Organik::Utils::spawnObjectHere(ObjInputBuf);
                ObjInputBuf.clear();
            }
            else
            {
                AddLog("[error] Invalid object name.");
            }
        }
        //static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t = ImGui::GetTime(); AddLog("Spam %f", t); }

        ImGui::Separator();

        // Options menu
        if (ImGui::BeginPopup("Options"))
        {
            ImGui::Checkbox("Auto-scroll", &AutoScroll);
            ImGui::EndPopup();
        }

        
        // Options, Filter
        ImGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_O, ImGuiInputFlags_Tooltip);
        if (ImGui::Button("Options"))
            ImGui::OpenPopup("Options");
        ImGui::SameLine();
        Filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
        ImGui::Separator();

        // Reserve enough left-over height for 1 separator + 1 input text
        const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
        if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_HorizontalScrollbar))
        {
            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::Selectable("Clear")) ClearLog();
                ImGui::EndPopup();
            }

            // Display every line as a separate entry so we can change their color or add custom widgets.
            // If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
            // NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping
            // to only process visible items. The clipper will automatically measure the height of your first item and then
            // "seek" to display only items in the visible area.
            // To use the clipper we can replace your standard loop:
            //      for (int i = 0; i < Items.Size; i++)
            //   With:
            //      ImGuiListClipper clipper;
            //      clipper.Begin(Items.Size);
            //      while (clipper.Step())
            //         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
            // - That your items are evenly spaced (same height)
            // - That you have cheap random access to your elements (you can access them given their index,
            //   without processing all the ones before)
            // You cannot this code as-is if a filter is active because it breaks the 'cheap random-access' property.
            // We would need random-access on the post-filtered list.
            // A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices
            // or offsets of items that passed the filtering test, recomputing this array when user changes the filter,
            // and appending newly elements as they are inserted. This is left as a task to the user until we can manage
            // to improve this example code!
            // If your items are of variable height:
            // - Split them into same height items would be simpler and facilitate random-seeking into your list.
            // - Consider using manual call to IsRectVisible() and skipping extraneous decoration from your items.
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
            if (copy_to_clipboard)
                ImGui::LogToClipboard();
            for (auto item : Items)
            {
                if (!Filter.PassFilter(item->c_str(), item->c_str() + item->size()))
                    continue;

                // Normally you would store more information in your item than just a string.
                // (e.g. make Items[] an array of structure, store color/type etc.)
                ImVec4 color;
                bool has_color = false;
                if (item->_Equal("[error]")) { color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true; }
                else if (item->starts_with("# ")) { color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); has_color = true; }
                if (has_color)
                    ImGui::PushStyleColor(ImGuiCol_Text, color);
                ImGui::TextUnformatted(item->c_str(), item->c_str() + item->size());
                if (has_color)
                    ImGui::PopStyleColor();
            }
            if (copy_to_clipboard)
                ImGui::LogFinish();

            // Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
            // Using a scrollbar or mouse-wheel will take away from the bottom edge.
            if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
                ImGui::SetScrollHereY(1.0f);
            ScrollToBottom = false;

            ImGui::PopStyleVar();
        }
        ImGui::EndChild();
        ImGui::Separator();

        // Command-line
        bool reclaim_focus = false;
        ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
        if (ImGui::InputText("Input", &InputBuf, input_text_flags, &TextEditCallbackStub, (void*)this))
        {
            Strtrim(&InputBuf);
            if (!InputBuf.empty())
                ExecCommand(InputBuf.c_str());
            InputBuf.clear();
            reclaim_focus = true;
        }
        // Auto-focus on window apparition
        ImGui::SetItemDefaultFocus();
        if (reclaim_focus)
            ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

        ImGui::End();
    }

    void    OrganikConsole::ExecCommand(const std::string& command_line)
    {
        AddLog("# %s\n", command_line.c_str()); // Add .c_str()

        // Insert into history. First find match and delete it so it can be pushed to the back.
        // This isn't trying to be smart or optimal.
        HistoryPos = -1;
        for (int i = History.size() - 1; i >= 0; i--)
            if (Stricmp(*History[i], command_line) == 0)
            {
                delete History[i];
                History.erase(History.begin() + i);
                break;
            }
        History.push_back(new std::string(command_line)); // Allocate on heap
        Organik::GetLogger()->LogFormatted("About to handle console command: %s", command_line.c_str());
        std::vector<std::string> words;
        std::string currentWord = std::string("");
        std::string cmd = std::string("");
        bool inQuote = false;
        for (char c : command_line) 
        {
            if (c == '\'') 
            {
                inQuote = !inQuote;
                if (!inQuote && !currentWord.empty()) 
                { // End of quote, add word
                    if (cmd.empty())
                    {
                        //not sure why this would happen, but it is user input, so....
                        cmd = currentWord;
                        currentWord.clear();
                        currentWord = "";
                        continue;
                    }
                    words.push_back(currentWord);
                    currentWord = "";
                }
            } 
            else if (std::isspace(c) && !inQuote) 
            {
                if (!currentWord.empty()) { // End of word, add it
                    if (cmd.empty())
                    {
                        cmd = currentWord;
                        currentWord.clear();
                        currentWord = "";
                        continue;
                    }
                    words.push_back(currentWord);
                    currentWord.clear();
                    currentWord = "";
                }
            } else {
                currentWord += c;
            }
        }
        // Add any remaining word after the loop
        if (!currentWord.empty()) 
        {
            if (cmd.empty())
            {
                cmd = currentWord;
            }
            else
            {
                words.push_back(currentWord);
            }
        }

        if (cmd.empty())
        {
            Organik::GetLogger()->LogFormatted("No command entered");
            AddLog("Error: No command entered");
            words.push_back("help"); // Default to help if no command
        }

        auto it = _commands.find(cmd);
        if (it != _commands.end()) 
        {
            it->second->Execute(this, words);
        } 
        else 
        {
            AddLog("Unknown command: '%s'", cmd.c_str());
            AddLog("Type 'help' for available commands");
        }
        // On command input, we scroll to bottom even if AutoScroll==false
        ScrollToBottom = true;
    }
        
        
    // In C++11 you'd be better off using lambdas for this sort of forwarding callbacks
    int OrganikConsole::TextEditCallbackStub(ImGuiInputTextCallbackData* data)
    {
        OrganikConsole* idk_Console = (OrganikConsole*)data->UserData;
        return idk_Console->TextEditCallback(data);
    }
    std::map<std::string, Command*>& OrganikConsole::Commands()
    {
        return _commands;
    }

    int     OrganikConsole::TextEditCallback(ImGuiInputTextCallbackData* data)
    {
        //AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
        switch (data->EventFlag)
        {
        case ImGuiInputTextFlags_CallbackCompletion:
        {
            // Example of TEXT COMPLETION
            
            // Locate beginning of current word
            const char* word_end = data->Buf + data->CursorPos;
            const char* word_start = word_end;
            while (word_start > data->Buf)
            {
                const char c = word_start[-1];
                if (c == ' ' || c == '\t' || c == ',' || c == ';')
                    break;
                word_start--;
            }

            // Build a list of candidates
            ImVector<const char*> candidates;
            for (const auto& cmd : _commands)
            {
                if (cmd.first.starts_with(word_start))
                    candidates.push_back(cmd.first.c_str());
            }
            if (word_start[0] > '0' && word_start[0] < '9')
            {
                for(const auto& id : Organik::Utils::knownInstanceIDs())
                {
                    if (std::to_string(id).starts_with(word_start))
                        candidates.push_back(std::to_string(id).c_str());
                }
            }
            else if (!std::string(word_start).starts_with("obj_") || strlen(word_start) >= 4)
            {
                for (const auto& str : objAndVarListFull)
                {
                    if (str.starts_with(word_start))
                        candidates.push_back(str.c_str());
                }
            }

            if (candidates.Size == 0)
            {
                // No match
                AddLog("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
            }
            else if (candidates.Size == 1)
            {
                // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing.
                data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                data->InsertChars(data->CursorPos, candidates[0]);
                data->InsertChars(data->CursorPos, " ");
            }
            else
            {
                // Multiple matches. Complete as much as we can..
                // So inputting "C"+Tab will complete to "CL" then display "CLEAR" and "CLASSIFY" as matches.
                int match_len = (int)(word_end - word_start);
                for (;;)
                {
                    int c = 0;
                    bool all_candidates_matches = true;
                    for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
                        if (i == 0)
                            c = toupper(candidates[i][match_len]);
                        else if (c == 0 || c != toupper(candidates[i][match_len]))
                            all_candidates_matches = false;
                    if (!all_candidates_matches)
                        break;
                    match_len++;
                }

                if (match_len > 0)
                {
                    data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                    data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
                }

                // List matches
                AddLog("Possible matches:\n");
                for (int i = 0; i < candidates.Size; i++)
                    AddLog("- %s\n", candidates[i]);
            }

            break;
        }
        case ImGuiInputTextFlags_CallbackHistory:
            {
                // Example of HISTORY
                const int prev_history_pos = HistoryPos;
                if (data->EventKey == ImGuiKey_UpArrow)
                {
                    if (HistoryPos == -1)
                        HistoryPos = History.Size - 1;
                    else if (HistoryPos > 0)
                        HistoryPos--;
                }
                else if (data->EventKey == ImGuiKey_DownArrow)
                {
                    if (HistoryPos != -1)
                        if (++HistoryPos >= History.Size)
                            HistoryPos = -1;
                }

                // A better implementation would preserve the data on the current input line along with cursor position.
                if (prev_history_pos != HistoryPos)
                {
                    const char* history_str = (HistoryPos >= 0) ? History[HistoryPos]->c_str() : "";
                    data->DeleteChars(0, data->BufTextLen);
                    data->InsertChars(0, history_str);
                }
            }
        }
        return 0;
    }
    void OrganikConsole::AddCommandHandler(Command* cmd) 
    { 
        Organik::GetLogger()->LogFormatted("Adding command handler for: %s", cmd->Name().c_str());

        _commands.insert(std::pair<std::string, Command*>(cmd->Name(), cmd)); 
    }

    UIManager* g_UIManager = nullptr;
    RValue* g_monoFonto = nullptr;
    OrganikConsole* g_Console = nullptr;
    static int g_DevMenuState = -1; // -1 means not initialized, 0 means closed, 1 means opening, 2 means closing, 3 means open

    int UIManager::GetMenuState()
    {
        if (g_DevMenuState == -1)
        {
            g_DevMenuState = MENU_CLOSED;
        }
        return g_DevMenuState;
    }
    UIManager* UIManager::GetInstance()
    {
        if (!g_UIManager)
        {
            g_UIManager = new UIManager();
        }
        return g_UIManager;
    }
    OrganikConsole* UIManager::GetConsole()
    {
        if (!g_Console)
        {
            g_Console = new OrganikConsole();
        }
        return g_Console;
    }
    RValue *UIManager::GetFont()
    {
        if (!g_monoFonto)
        {
            g_monoFonto = new RValue(g_ModuleInterface->CallBuiltin("font_add", {RValue("Inconsolata.ttf")}));
        }
        return g_monoFonto;
    }

    UIManager::~UIManager()
    {
        for (auto* element : _elements)
        {
            delete element;
        }
        _elements.clear();
    }

    void UIManager::ToggleConsole()
    {
        if (!g_Console)
        {
            UIManager::GetConsole();
            showConsole = true;
            return;
        }
        showConsole = !showConsole;
    }

    void UIManager::DrawEvent()
    {
    }


    RValue UIManager::Add(UIElement* target)
    {
        _elements.push_back(target);
        return RValue(static_cast<int>(_elements.size() - 1));
    }

    UIElement* UIManager::Find(bool(*pred)(const UIElement&))
    {
        int i = 0;
        for (UIElement* element : _elements) 
        {
            if (element && pred(*element)) 
            {
                return element;
            }
        }
        return nullptr;
    }
    int UIManager::FindIndex(bool(*pred)(const UIElement&))
    {
        int i = 0;
        for (UIElement* element : _elements) 
        {
            if (element && pred(*element)) 
            {
                return i;
            }
            i = i + 1;
        }
        return -1;
    }

    std::vector<UIElement*> UIManager::FindAll(bool(*pred)(const UIElement&))
    {
        std::vector<UIElement*> results;
        for (UIElement* element : _elements) 
        {
            if (element && pred(*element)) 
            {
                results.push_back(element);
            }
        }
        return results;
    }

    bool UIManager::Remove(int index)
    {
        if (index < 0 || index >= static_cast<int>(_elements.size())) 
        {
            return false;
        }
        
        delete _elements[index];
        _elements.erase(_elements.begin() + index);
        return true;
    }
    bool UIManager::Remove(bool(*pred)(const UIElement&))
    {
        int index = -1;
        index = this->FindIndex(pred);
        if (index < 0 || index >= static_cast<int>(_elements.size())) 
        {
            return false;
        }
        
        delete _elements[index];
        _elements.erase(_elements.begin() + index);
        return true;
    }

    void UIManager::Initialize() 
    {
    }

    void UIManager::Shutdown() 
    {
        delete g_UIManager;
        if (UIManager::GetConsole() != nullptr)
        {
            delete g_Console;
            g_Console = nullptr;
        }
        if (g_monoFonto != nullptr)
        {
            g_ModuleInterface->CallBuiltin("font_delete", {*g_monoFonto});
            delete g_monoFonto;
            g_monoFonto = nullptr;
        }
        g_UIManager = nullptr;
    }

    void ShowOrganikConsole(bool* p_open)
    {
        UIManager::GetConsole()->Draw("Example: Console", &g_UIManager->showConsole);
    }
    void UIManager::StepEvent()
    {
        // CInstance* globalInstance = nullptr;
        // Organik::GetLogger()->LogFormatted("%s - %s:%d", __FUNCTION__, __FILE__, __LINE__);

        // g_ModuleInterface->GetGlobalInstance(&globalInstance);
        // Organik::GetLogger()->LogFormatted("%s - %s:%d", __FUNCTION__, __FILE__, __LINE__);

        // ImGui_ImplDX11_NewFrame();
        // Organik::GetLogger()->LogFormatted("%s - %s:%d", __FUNCTION__, __FILE__, __LINE__);
        // ImGui_ImplWin32_NewFrame();
        // Organik::GetLogger()->LogFormatted("%s - %s:%d", __FUNCTION__, __FILE__, __LINE__);
        // ImGui::NewFrame();
        // Organik::GetLogger()->LogFormatted("%s - %s:%d", __FUNCTION__, __FILE__, __LINE__);
        // ImGui::ShowDemoWindow();
        // //ShowOrganikConsole(&showConsole);
        // Organik::GetLogger()->LogFormatted("%s - %s:%d", __FUNCTION__, __FILE__, __LINE__);
        // bool f12Pressed = ImGui::IsKeyPressed(ImGuiKey_F12);
        // if (f12Pressed) 
        // {
        //     ToggleConsole(); // Toggle console visibility
        // }
        if ((Organik::Utils::getOutgameIndex() > 0) && Organik::Utils::g_EnableDevcom)
        {
            std::vector<CInstance*> instanceVect = Organik::Utils::findInstances(Organik::Utils::getOutgameIndex());
            // GetLogger()->LogFormatted("EnableDevcom? %s", Organik::Utils::g_EnableDevcom ? "yes" : "no");
            if (!instanceVect.empty())
            {
                CInstance* outgameInstance = instanceVect[0];
                if (outgameInstance->ToRValue().ContainsValue("devcom"))
                {
                    RValue* g_devcom = outgameInstance->GetRefMember("devcom");
                    if (!(g_devcom->ToBoolean() == Organik::Utils::g_EnableDevcom))
                    {
                        *g_devcom = RValue(Organik::Utils::g_EnableDevcom);
                    }
                }
            }
            else 
            {
                GetLogger()->LogFormatted("Instance list empty (%d)", __LINE__);
            }
        }
        if (Organik::Utils::getOutgameIndex() > 0)
        {
            std::vector<CInstance*> instanceVect = Organik::Utils::findInstances(Organik::Utils::getOutgameIndex());
            if (!instanceVect.empty())
            {
                CInstance* outgameInstance = instanceVect[0];
                if (outgameInstance->ToRValue().ContainsValue("ClientVersion"))
                {
                    char lastChar = '2' - ((byte)Organik::Utils::g_EnableMultiplayerCompat);
                    RValue* clientVersion = outgameInstance->GetRefMember("ClientVersion");
                    if (clientVersion->ToString().back() != lastChar)
                    {
                        GetLogger()->LogSimple("Setting ClientVersion");
                        *clientVersion = RValue(std::string("26.") + lastChar);
                    }
                }
            }
            else 
            {
                GetLogger()->LogFormatted("Instance list empty (%d)", __LINE__);
            }
        }
        if ((Organik::Utils::getObj_PlayerIndex() > 0) && Organik::Utils::g_EnableInvincibility)
        {
            std::vector<CInstance*> instanceVect = Organik::Utils::findInstances(Organik::Utils::getObj_PlayerIndex());
            if (!instanceVect.empty())
            {
                GetLogger()->LogSimple("Enabling invincibility for player");
                CInstance* instance = instanceVect[0];
                if (instance->ToRValue().ContainsValue("invincible") && instance->ToRValue().ContainsValue("invincibletimer"))
                {
                    RValue* isInvincible = instance->GetRefMember("invincible");
                    RValue* invincibilityTimer = instance->GetRefMember("invincibletimer");
                    *isInvincible = RValue(true);
                    *invincibilityTimer = RValue(5.00);
                }
                else 
                {
                    GetLogger()->LogFormatted("Instance %d does not have invincibility member (%d)", instance->GetMembers().m_ID, __LINE__);
                }
            }
            else 
            {
                GetLogger()->LogFormatted("Instance list empty (%d)", __LINE__);
            }
        }
    }
}