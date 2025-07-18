#include "YYToolkit/YYTK_Shared.hpp"
#include "Aurie/shared.hpp"
#include "UIElement.h" 
#include "../ModuleMain.h"
#include "OrganikConsole.h"
#include "../Utils.h"
#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "../obj_list.h"
#include <lua.hpp>
#include <luajit.h>

namespace Organik
{
    namespace BuiltinCommands
    {
        void AddCommandHandler(Command* cmd);
        std::map<std::string, Command*> GetCommands();

    };
    OrganikConsole::OrganikConsole(const std::string& name)
        : UIElement(name)
    {
        ClearLog();
        InputBuf.clear();
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

    void OrganikConsole::Draw(bool& out_mousedOver, bool* p_open, const std::string& title)
    {
        ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
        if (!ImGui::Begin(title.c_str(), p_open))
        {
            ImGui::End();
            return;
        }

        if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_ChildWindows))
            out_mousedOver = true;
        
        
        ImGui::SameLine();
        bool copy_to_clipboard = ImGui::SmallButton("Copy");
        ImGuiInputTextFlags obj_index_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll | ImGuiInputTextFlags_CallbackCompletion;
        
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

    void OrganikConsole::ExecCommand(const std::string& command_line)
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
}