#pragma once
#include <fstream>
#include <mutex>
#include <string>
#include <filesystem>
#include <cstdarg>
#include <CallbackManager/CallbackManagerInterface.h>

namespace Organik {
    
    class Logger {
    public:
        ~Logger();
        bool Init(const std::string& filename);
        void Cleanup(void);
        
        // Add more helpers as needed
        bool LogSimple(const char* text);
        bool LogFormatted(const char* fmt, ...);
        bool LogEventCallback(const char *sourceFile, const int line, const char* callbackName, CodeEventArgs &args, bool logArgv=false);
        static bool InitLogging();
    private:
        Logger(std::filesystem::path path=std::filesystem::current_path(), const char* filename="organik.log");
        std::ofstream outFile;
        std::mutex logMutex;
        std::string ParseFormatting(const char* fmt, va_list args);
        std::string ParseFormatting(const char* fmt, ...);
        bool WriteToLog(const std::string& message, bool flushLine=true);
    };
    
    Logger* GetLogger(void) ;
}