#include "Logging.h"
#include <Windows.h>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
// #include "UI/OrganikConsole.h"
// #include "UI/UIManager.h"
// #include "CallbackManager/CallbackManagerInterface.h"
// #include "ModuleMain.h"

namespace Organik
{
    /**
     * @brief Singleton instance of the Logger.
     */
    static Logger* g_LoggerInstance = nullptr;
    Logger* GetLogger(void)
    {
        return g_LoggerInstance;
    }
    /**
     * @brief Initializes the logging system.
     * This function creates and initializes the singleton Logger instance.
     * @return True if initialization was successful, false otherwise.
     */
    bool Logger::InitLogging()
    {
        if (g_LoggerInstance == nullptr)
        {
            g_LoggerInstance = new Logger(); // Constructor calls Init()
            if (g_LoggerInstance && g_LoggerInstance->outFile.is_open())
            {
                g_LoggerInstance->WriteToLog("Logger initialized successfully.");
                return true;
            }
            delete g_LoggerInstance; // Cleanup if construction or file opening failed
            g_LoggerInstance = nullptr;
            std::cerr << "Logger::InitLogging failed to initialize logger or open file." << std::endl;
            return false;
        }
        return true; // Already initialized
    }

    Logger::Logger(std::filesystem::path path, const char* filename)
    {
        if (!this->Init((path / filename).string()))
        {
            std::cerr << "Failed to open log file: " << (path / filename).string() << std::endl;
        }
    }

    Logger::~Logger()
    {
        Cleanup();
        if (g_LoggerInstance == this) // Clear the global instance if this is the one
        {
            g_LoggerInstance = nullptr;
        }
    }

    bool Logger::Init(const std::string& filename)
    {
        std::unique_lock<std::shared_mutex> lock(logMutex);
        if (outFile.is_open())
            return true;
        outFile.open(filename, std::ios::out | std::ios::trunc);
        return outFile.is_open();
    }

    /**
     * @brief Cleans up logger resources, primarily closing the log file.
     * Ensures the log file is flushed and closed if it was open.
     */
    void Logger::Cleanup(void)
    {
        std::unique_lock<std::shared_mutex> lock(logMutex);
        if (outFile.is_open())
        {
            outFile.flush();
            outFile.close();
        }
    }

    /**
     * @brief Parses a format string with a va_list of arguments.
     * @param Format The format string.
     * @param Arguments The va_list of arguments.
     * @return The formatted string.
     */
    const std::string_view& Logger::ParseFormatting(
        const char* Format,
        va_list Arguments)
    {
        constexpr size_t max_length = 4096;
        size_t length = strnlen_s(Format, max_length);

        if (length >= max_length)
            return std::string_view("<string too long to print>");

        char buffer[max_length] = { 0 };
        vsprintf_s(buffer, Format, Arguments);

        return std::string_view(buffer);
    }

    /**
     * @brief Parses a format string with variadic arguments.
     * @param fmt The format string.
     * @param ... Variadic arguments for the format string.
     * @return The formatted string.
     */
    const std::string_view& Logger::ParseFormatting(
        const char* fmt,
        ...)
    {
        using namespace std::chrono_literals;
        va_list args;
        va_start(args, fmt);
        auto ret = ParseFormatting(fmt, args).data( );
        va_end(args);
        return std::string_view(ret);
    }

    /**
     * @brief Writes a message to the log file.
     * @param message The message string to write.
     * @param flushLine If true, flushes the output stream after writing.
     * @return True if the message was written successfully, false otherwise.
     */
    bool Logger::WriteToLog(const std::string_view& message, bool flushLine)
    {
        std::unique_lock<std::shared_mutex> lock(logMutex);
        if (!outFile.is_open())
        {
            return false;
        }
        outFile << message;
        if (flushLine)
            outFile << std::endl;
        return true;
    }

    /**
     * @brief Logs a simple text message.
     * @param text The text message to log.
     * @return True if logging was successful, false otherwise.
     */
    bool Logger::LogSimple(const char* text, bool flushLine)
    {
        return WriteToLog(text, flushLine);
    }

    /**
     * @brief Logs a formatted message.
     * @param fmt The format string.
     * @param ... Variadic arguments for the format string.
     * @return True if logging was successful, false otherwise.
     */
    bool Logger::LogFormatted(const char* fmt, ...)
    {
        va_list va_args;
        va_start(va_args, fmt);
        auto ret = WriteToLog(ParseFormatting(fmt, va_args), true);
        va_end(va_args);
        return ret;
    }

    // bool Logger::TryLogConsole(const char* fmt, ...)
    // {
    //     va_list args;
    //     va_start(args, fmt);
    //     std::string formatted_output = ParseFormatting(fmt, args);
    //     va_end(args);

    // auto* console = Organik::UIManager::GetInstance()->GetElement<OrganikConsole>(false);
    // if (console)
    // {
    //     console->AddLog(formatted_output.c_str());
    //     return true;
    // }
    //     LogSimple(formatted_output.c_str());
    //     return false;
    // }
}
