#pragma once
#include <fstream>
#include <mutex>
#include <string>
#include <filesystem>
#include <cstdarg>
#include <shared_mutex>

namespace Organik
{

    class Logger
    {
    public:
        ~Logger();
        bool Init(const std::string& filename);
        void Cleanup(void);

        bool LogSimple(const char* text, bool flushLine = true);
        bool LogFormatted(const char* fmt, ...);
        const std::string_view& ParseFormatting(const char* fmt, ...);
        const std::string_view& ParseFormatting(const char* fmt, va_list args);
        static bool InitLogging();

    private:
        Logger(std::filesystem::path path = std::filesystem::current_path( ), const char* filename = "organik.log");
        std::ofstream outFile;
        std::shared_mutex logMutex;
        bool WriteToLog(const std::string_view& message, bool flushLine = true);
    };
    Logger* GetLogger( );
}
