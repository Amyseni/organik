#include "../ModuleMain.h"
#include "DevConsole.h"
#include "Command.h"  // Add this include
#include <algorithm> 
#include <cctype>    

namespace Organik
{
    // Define the static member
    std::unordered_map<std::string, Command*> DevConsole::_commands;
    
    // ConsoleStreamBuffer implementation
    ConsoleStreamBuffer::ConsoleStreamBuffer(DevConsole* console) : _console(console)
    {
        Organik::GetLogger()->LogFormatted("About to initialize ConsoleStreamBuffer");
    }
    
    void DevConsole::AddCommandHandler(Command* cmd) 
    { 
        Organik::GetLogger()->LogFormatted("Adding command handler for: %s", cmd->Name().c_str());

        Commands().insert(std::pair<std::string, Command*>(cmd->Name(), cmd)); 
    }

    int ConsoleStreamBuffer::overflow(int c)
    {
        std::lock_guard<std::mutex> lock(_bufferMutex);
        if (c != EOF) {
            _buffer += static_cast<char>(c);
            if (c == '\n') {
                if (_console) {
                    _console->AppendToOutput(_buffer);
                }
                _buffer.clear();
            }
        }
        return c;
    }

    std::streamsize ConsoleStreamBuffer::xsputn(const char* s, std::streamsize count)
    {
        std::lock_guard<std::mutex> lock(_bufferMutex);
        for (std::streamsize i = 0; i < count; ++i) {
            _buffer += s[i];
            if (s[i] == '\n') {
                if (_console) {
                    _console->AppendToOutput(_buffer);
                }
                _buffer.clear();
            }
        }
        return count;
    }

    // DevConsoleStream implementation
    DevConsoleStream::DevConsoleStream(DevConsole* console) 
        : std::ostream(&_buffer), _buffer(console)
    {
        Organik::GetLogger()->LogFormatted("About to initialize DevConsoleStream");
    }

    // DevConsole implementation
    DevConsole::DevConsole(int x, int y, int width, int height)
        : _x(x), _y(y), _width(width), _height(height), 
          _outputStream(this), _enterPressed(false)  
    {
        _name = "DevConsole";
        Organik::GetLogger()->LogFormatted("About to initialize DevConsole at (%d, %d) size (%d, %d)", x, y, width, height);
        
        // Create output area (multiline, read-only, with word wrap)
        int outputHeight = height - 40; // Leave space for input
        _outputArea = new TextArea(x, y, width, outputHeight, 
                                  RValue(0.9), RValue(0x1a1a1a), RValue(0x404040), 
                                  RValue(0x00ff00), RValue(12.0));
        if (!_outputArea) {
            Organik::GetLogger()->LogFormatted("DevConsole - Failed to create output area");
            return;
        }
        
        _outputArea->SetMultiline(true);
        _outputArea->SetWordWrap(true);
        _outputArea->SetEditable(false);
        
        // Create input area (single line, editable)
        _inputArea = new TextArea(x, y + outputHeight, width, 30,
                                 RValue(0.9), RValue(0x2a2a2a), RValue(0x606060), 
                                 RValue(0xffffff), RValue(12.0));
        if (!_inputArea) {
            Organik::GetLogger()->LogFormatted("DevConsole - Failed to create input area");
            delete _outputArea;
            _outputArea = nullptr;
            return;
        }
        
        _inputArea->SetMultiline(false);
        _inputArea->SetEditable(true);
        
        // Set bounding box
        _bbox = {x, y, 
                width, height};
        
        // Add welcome message
        WriteLine("Organik Dev Console initialized");
        WriteLine("Type commands and press Enter");
        
        Organik::GetLogger()->LogFormatted("DevConsole initialized successfully");
    }

    DevConsole::~DevConsole()
    {
        Organik::GetLogger()->LogFormatted("About to destroy DevConsole");

        if (_outputArea) {
            delete _outputArea;
            _outputArea = nullptr;
        }
        
        if (_inputArea) {
            delete _inputArea;
            _inputArea = nullptr;
        }
    }

    void DevConsole::Step()
    {
        if (!IsVisible()) {
            // Organik::GetLogger()->LogFormatted("DevConsole::Step - Console is not visible");
            return;
        }
        if (!_outputArea || !_inputArea) {
            Organik::GetLogger()->LogFormatted("DevConsole::Step - Text areas are null");
            return;
        }
        
        // Process any pending output from other threads
        ProcessPendingOutput();
        if (!g_ModuleInterface) {
            Organik::GetLogger()->LogFormatted("DevConsole::Step - g_ModuleInterface is null");
            return;
        }
        
        // Check for Enter key press
//         Organik::GetLogger()->LogFormatted("[TRACE] %s @ %s : %d ", __FUNCTION__, __FILE__, __LINE__);        
        RValue lastKey = RValue(0);
        std::string lastInput = std::string(_inputArea->GetText());
        
        g_ModuleInterface->GetBuiltin("keyboard_lastkey", globalInstance, NULL_INDEX, lastKey);
        bool enterPressed = (lastKey.ToInt32() == 13);
        bool upPressed = (lastKey.ToInt32() == vk_up.ToInt32());
        bool downPressed = (lastKey.ToInt32() == vk_down.ToInt32());
        if (enterPressed || upPressed || downPressed)
        {
            // Check if the key was pressed, not just held down
//            Organik::GetLogger()->LogFormatted("[TRACE] %s @ %s : %d ", __FUNCTION__, __FILE__, __LINE__);
            bool keyWasPressed = g_ModuleInterface->CallBuiltin("keyboard_check_pressed", {lastKey}).ToBoolean();
            enterPressed = enterPressed && keyWasPressed;
            upPressed = upPressed && keyWasPressed;
            downPressed = downPressed && keyWasPressed;
        }
        if (enterPressed) 
        {
            std::string whitespace = " \t\n\r\f\v"; // Define whitespace characters
            while((!lastInput.empty()) && whitespace.contains(lastInput.back())) {
                lastInput.pop_back(); // Remove trailing whitespace characters
            }
            if(lastInput.empty())
            {
                return;
            }
            HandleCommand(lastInput);
            _inputHistory.push_front(lastInput);
            if (_inputHistory.size() > 100) {
                _inputHistory.pop_back(); // Limit history size to 100 (pop oldest first)
            }
            _currentHistoryIndex = -1;
            _inputArea->ClearText();
            g_ModuleInterface->CallBuiltin("keyboard_clear", {vk_enter}).ToBoolean();
        }
        else if (upPressed) 
        {
            Organik::GetLogger()->LogFormatted("Up arrow pressed, attempting to access command history");
            if (!_inputHistory.empty()) {
                int previousIndex = _currentHistoryIndex;
                if (_currentHistoryIndex < 0) {
                    _currentHistoryIndex = 0;
                } else if (_currentHistoryIndex < static_cast<int>(_inputHistory.size()) - 1) {
                    _currentHistoryIndex++;
                }
                if (_currentHistoryIndex != previousIndex)
                {
                    _inputArea->SetText(_inputHistory[_currentHistoryIndex]);
                }
            }
        }
        else if (downPressed) 
        {
            // Handle down arrow key for command histor
            Organik::GetLogger()->LogFormatted("Down arrow pressed, attempting to access command history");
            if (!_inputHistory.empty()) {
                int previousIndex = _currentHistoryIndex;
                if (_currentHistoryIndex > 0) {
                    _currentHistoryIndex--;
                    _inputArea->SetText(_inputHistory[_currentHistoryIndex]);
                } else if (_currentHistoryIndex == 0) {
                    _inputArea->ClearText();
                    _currentHistoryIndex = -1; // Reset index
                }
            }
        }
        // Update child elements
        _inputArea->Step();
        _outputArea->Step();

    }

    void DevConsole::Draw()
    {
        // Organik::GetLogger()->LogFormatted("DevConsole::Draw %d", __LINE__);
        if (!IsVisible()) return;
        // Organik::GetLogger()->LogFormatted("DevConsole::Draw %d", __LINE__);
        
        if (!_outputArea || !_inputArea) {
            Organik::GetLogger()->LogFormatted("DevConsole::Draw - Text areas are null");
            return;
        }
        
        // Draw child elements
        // Organik::GetLogger()->LogFormatted("DevConsole::Draw %d", __LINE__);
        _outputArea->Draw();
        // Organik::GetLogger()->LogFormatted("DevConsole::Draw %d", __LINE__);
        _inputArea->Draw();
    }

    void DevConsole::SetOutputFile(const std::string& filename)
    {
        Organik::GetLogger()->LogFormatted("About to set output file: %s", filename.c_str());
        _outFilename = filename;
        
        // Open the file for writing
        if (!_outFilename.empty()) {
            _outputFileStream = std::ofstream(_outFilename, std::ios::app);
            if (!_outputFileStream) {
                Organik::GetLogger()->LogFormatted("Failed to open output file: %s", _outFilename.c_str());
            } else {
                _outputFileStream.close();
                Organik::GetLogger()->LogFormatted("Output file set successfully: %s", _outFilename.c_str());
            }
        }
    }

    bool DevConsole::IsCollidingWithMouse()
    {
        if (!_outputArea || !_inputArea) {
            Organik::GetLogger()->LogFormatted("DevConsole::IsCollidingWithMouse - Text areas are null");
            return false;
        }
        
        return _outputArea->IsCollidingWithMouse() || _inputArea->IsCollidingWithMouse();
    }

    void DevConsole::WriteLine(const std::string& line)
    {
        Organik::GetLogger()->LogFormatted("About to write line to console: %s", line.c_str());
        Write(line + "\n");
    }

    void DevConsole::Write(const std::string& text)
    {
        std::lock_guard<std::mutex> lock(_outputMutex);
        if (_outputFileStream.is_open()) {
            std::lock_guard<std::mutex> lock2(_outputFileMutex);
            _outputFileStream << text;
        }
            
        _pendingOutput.push(text);
    }

    void DevConsole::Clear()
    {
        Organik::GetLogger()->LogFormatted("About to clear console output");
        
        if (_outputArea) {
            _outputArea->ClearText();
        }
        
        // Clear pending output as well
        std::lock_guard<std::mutex> lock(_outputMutex);
        while (!_pendingOutput.empty()) {
            _pendingOutput.pop();
        }
    }

    void DevConsole::AppendToOutput(const std::string& text)
    {
        // This is called from the stream buffer, add to thread-safe queue
        std::lock_guard<std::mutex> lock(_outputMutex);
        _pendingOutput.push(text);
    }

    void DevConsole::ProcessPendingOutput()
    {
        if (!_outputArea) return;
        
        std::lock_guard<std::mutex> lock(_outputMutex);
        while (!_pendingOutput.empty()) {
            _outputArea->AppendText(_pendingOutput.front());
            _pendingOutput.pop();
        }
    }

    void DevConsole::HandleCommand(const std::string& command)
    {
        Organik::GetLogger()->LogFormatted("About to handle console command: %s", command.c_str());
        std::vector<std::string> words;
        std::string currentWord = std::string("");
        std::string cmd = std::string("");
        bool inQuote = false;
        WriteLine("> " + command);
        for (char c : command) {
            if (c == '\'') {
//                Organik::GetLogger()->LogFormatted("[TRACE] %d %c", __LINE__, c);
                inQuote = !inQuote;
                if (!inQuote && !currentWord.empty()) { // End of quote, add word
                    if (cmd.empty())
                    {
                        //not sure why this would happen, but it is user input, so....
                        cmd = currentWord;
                        currentWord.clear();
                        currentWord = "";
                        continue;
                    }
//                    Organik::GetLogger()->LogFormatted("[TRACE] %d %s", __LINE__, currentWord.c_str());
                    words.push_back(currentWord);
                    currentWord = "";
                }
            } else if (std::isspace(c) && !inQuote) {
//              Organik::GetLogger()->LogFormatted("[TRACE] %d %c", __LINE__, c);
                if (!currentWord.empty()) { // End of word, add it
                    if (cmd.empty())
                    {
                        cmd = currentWord;
                        currentWord.clear();
                        currentWord = "";
                        continue;
                    }
//                    Organik::GetLogger()->LogFormatted("[TRACE] %d %s", __LINE__, currentWord.c_str());
                    words.push_back(currentWord);
                    currentWord.clear();
                    currentWord = "";
                }
            } else {
//                Organik::GetLogger()->LogFormatted("[TRACE] %d %s + %c", __LINE__, currentWord.c_str(), c);
                currentWord += c;
            }
        }
        // Add any remaining word after the loop
        if (!currentWord.empty()) {
            if (cmd.empty())
            {
                cmd = currentWord;
            }
            else
            {
                words.push_back(currentWord);
            }
//            Organik::GetLogger()->LogFormatted("[TRACE] %s @ %d : %s ", __FUNCTION__, __LINE__, cmd.c_str());
        }

        if (cmd.empty())
        {
            Organik::GetLogger()->LogFormatted("No command entered");
            WriteLine("Error: No command entered");
            words.push_back("help"); // Default to help if no command
        }
        auto it = Commands().find(cmd);
        if (it != Commands().end()) {
            it->second->Execute(this, words);
        } else {
            WriteLine("Unknown command: " + cmd);
            WriteLine("Type 'help' for available commands");
        }
        // if (!_commandHandler) {
        //     _commandHandler = [](DevConsole *self, const std::string& cmd, std::vector<std::string> args) {
        //         if (cmd == "clear") {
        //             self->Clear();
        //         } else if (cmd == "objinfo") {
        //             if (args.size() < 1) {
        //                 self->WriteLine("Usage: objinfo [object]");
        //                 return;
        //             }
        //             std::string objectName = args[0];
        //             CInstance* globalInst = nullptr;
        //             g_ModuleInterface->GetGlobalInstance(&globalInst);
        //             if (!globalInst) {
        //                 self->WriteLine("Error: Global instance not found");
        //                 return;
        //             }
        //             RValue objType;
        //             RValue objInstance = g_ModuleInterface->CallBuiltin("", {globalInst, NULL_INDEX, objType});
        //             if (!objInstance.ToBoolean()) {
        //                 self->WriteLine("Error: Object '" + objectName + "' not found");
        //                 return;
        //             }
        //             self->WriteLine("Object: " + objectName + "\nType: " + 
        //                             objInstance.GetKindName() +
        //                             "\nMember count: " +
        //                             std::to_string(objInstance.GetMemberCount()));
        //         } else if (cmd == "help") {
        //             self->WriteLine("Available commands:");
        //             self->WriteLine("  clear - Clear the console");
        //             self->WriteLine("  help - Show this help");
        //             self->WriteLine("  objinfo [object] - Display some info about global GameMaker objects");
        //             self->WriteLine("  room_goto [room_name] - Attempt to go to [room_name]. Probably pretty high chance this crashes something.");
        //         } else {
        //             self->WriteLine("Unknown command: " + cmd);
        //             self->WriteLine("Type 'help' for available commands");
        //         }
        //     };
        // }
        // if (_commandHandler) {
//                 Organik::GetLogger()->LogFormatted("[TRACE] %s @ %s : %d ", __FUNCTION__, cmd.c_str(), words.size());
            
        //         _commandHandler(this, cmd, words);
        // } else {
        //     //should be unreachable
        //     Organik::GetLogger()->LogFormatted("Error: Command handler is not set");
        //     WriteLine("Error: Command handler is not set");
        //     exit(1);
        // }
    }
    
}