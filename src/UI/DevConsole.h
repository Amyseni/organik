#pragma once

#include "YYToolkit/YYTK_Shared.hpp"
#include "Aurie/shared.hpp"
#include "UIManager.h"
#include "TextArea.h"
#include <string>
#include <mutex>
#include <queue>
#include <sstream>
#include <functional>
#include <unordered_map>

using namespace YYTK;

namespace Organik
{
    // Forward declaration
    class Command;

    // Thread-safe stream buffer for console output

    class ConsoleStreamBuffer : public std::streambuf
    {
        public:
            ConsoleStreamBuffer(class DevConsole* console);
            
        protected:
            int overflow(int c) override;
            std::streamsize xsputn(const char* s, std::streamsize count) override;
            
        private:
            class DevConsole* _console;
            std::mutex _bufferMutex;
            std::string _buffer;
    };

    class DevConsoleStream : public std::ostream
    {
        public:
            DevConsoleStream(DevConsole* console);
            
        private:
            ConsoleStreamBuffer _buffer;
    };

    class DevConsole : public UIElement
    {
        public:
        
        DevConsole(int x, int y, int width, int height);
        ~DevConsole();
        
        void Step() override;
        void Draw() override;
        bool IsCollidingWithMouse() override;
        static std::unordered_map<std::string, Command*> _commands;
        
        static std::unordered_map<std::string, Command*>& Commands()
        {
            return _commands;
        }
        
        // Stream interface for output
        DevConsoleStream& GetOutputStream() { return _outputStream; }
        
        // Thread-safe output methods
        void WriteLine(const std::string& line);
        void Write(const std::string& text);
        void SetOutputFile(const std::string& filename);
        void ClearOutputFile() { _outFilename.clear(); }
        const inline std::string& GetOutputFile() const { return _outFilename; };
        inline bool HasOutputFile() const { return !(_outFilename.empty()); }
        
        void Clear();
        
        // Command handling
        static void AddCommandHandler(Command* cmd);
        
        // Visibility toggle
        void Toggle() { SetVisible(!IsVisible()); }
        
        // Internal method for stream buffer
        void AppendToOutput(const std::string& text);
        
        private:
        void ProcessPendingOutput();
        void HandleCommand(const std::string& command);
        
        TextArea* _outputArea;
        TextArea* _inputArea;
        
        int _x, _y, _width, _height;
        
        // Thread-safe output queue
        std::mutex _outputMutex;
        std::queue<std::string> _pendingOutput;
        
        // Stream for output
        DevConsoleStream _outputStream;
        
        std::ofstream _outputFileStream;
        std::mutex _outputFileMutex;
        
        // Input handling
        std::string _lastInput;
        std::deque<std::string> _inputHistory;
        std::string _savedInput;
        int _currentHistoryIndex = -1;
        std::string _outFilename;
        bool _enterPressed;
    };
}