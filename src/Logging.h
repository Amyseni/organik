#pragma once
#include <fstream>
#include <mutex>
#include <string>
#include <filesystem>
#include <cstdarg>
#include <CallbackManager/CallbackManagerInterface.h>

namespace Organik 
{
    
    class Logger 
    {
    public:
        ~Logger();
        bool Init(const std::string& filename);
        void Cleanup(void);
        
        // Add more helpers as needed
        bool LogSimple(const char* text);
        bool LogFormatted(const char* fmt, ...);
        bool LogEventCallback(const char *sourceFile, const int line, const char* callbackName, CodeEventArgs args);
        std::string ParseFormatting(const char* fmt, ...);
        std::string ParseFormatting(const char* fmt, va_list args);
        static bool InitLogging();
    private:
        Logger(std::filesystem::path path=std::filesystem::current_path(), const char* filename="organik.log");
        std::ofstream outFile;
        std::mutex logMutex;
        bool WriteToLog(const std::string& message, bool flushLine=true);
    };
    Logger* GetLogger(void) ;
}