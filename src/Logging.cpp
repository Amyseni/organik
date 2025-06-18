#include "Logging.h"
#include <Windows.h> 
#include <chrono>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <sstream>
#include "CallbackManager/CallbackManagerInterface.h"
#include "CodeEvents.h"
#include "ModuleMain.h"

namespace Organik
{
    /**
     * @brief Singleton instance of the Logger.
     */
    static Logger* g_LoggerInstance = nullptr;

    /**
     * @brief Retrieves the singleton Logger instance.
     * @return Pointer to the Logger instance, or nullptr if not initialized.
     */
    Logger* GetLogger() { return g_LoggerInstance; }

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
        std::lock_guard<std::mutex> lock(logMutex);
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
        std::lock_guard<std::mutex> lock(logMutex);
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
    std::string Logger::ParseFormatting(
        const char* Format, 
        va_list Arguments
    )
    {
        constexpr size_t max_length = 4096;
        size_t length = strlen(Format);

        if (length >= max_length)
            return "<string too long to print>";

        char buffer[max_length] = { 0 };

        // strncpy_s is not needed here as vsprintf_s will write the formatted string.
        vsprintf_s(buffer, Format, Arguments);

        return buffer;
    }

    /**
     * @brief Parses a format string with variadic arguments.
     * @param fmt The format string.
     * @param ... Variadic arguments for the format string.
     * @return The formatted string.
     */
    std::string Logger::ParseFormatting(
        const char* fmt, 
        ...
    )
    {
        va_list args;
        va_start(args, fmt);
        std::string result = ParseFormatting(fmt, args);
        va_end(args);
        return result;
    }

    /**
     * @brief Writes a message to the log file.
     * @param message The message string to write.
     * @param flushLine If true, flushes the output stream after writing.
     * @return True if the message was written successfully, false otherwise.
     */
    bool Logger::WriteToLog(const std::string& message, bool flushLine)
    {
        std::lock_guard<std::mutex> lock(logMutex);
        if (!outFile.is_open())
        {
            return false;
        }
        outFile << message << std::endl;
        return true;
    }

    /**
     * @brief Logs a simple text message.
     * @param text The text message to log.
     * @return True if logging was successful, false otherwise.
     */
    bool Logger::LogSimple(const char* text)
    {
        return WriteToLog(text);
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
        std::string formatted_output = ParseFormatting(fmt, va_args);
        va_end(va_args);
        return WriteToLog(formatted_output, true);
    }
    int tryDereference(int ptr)
    {
        LPCVOID int_hack = (LPCVOID)ptr;
        MEMORY_BASIC_INFORMATION mbi;
        if (VirtualQuery((LPCVOID)ptr, &mbi, sizeof(mbi)) != 0 && mbi.State == MEM_COMMIT &&
            (mbi.Protect == PAGE_READONLY || mbi.Protect == PAGE_READWRITE || mbi.Protect == PAGE_EXECUTE_READ || mbi.Protect == PAGE_EXECUTE_READWRITE))
        {
            return *(int*)int_hack;
        }
        return -1;
    }

    int tryDereferencePtrLoop(IN int ptr_trgt, OUT int& COUNT)
    {
        int i = 0;
        COUNT = 0;
        int last_val = ptr_trgt;
        int ptr_test = ptr_trgt;
        do 
        {
            i = i + 1;
            last_val = ptr_test;
            ptr_test = tryDereference(ptr_test);
            COUNT = i;
        } while (ptr_test != -1);
        return last_val;
    }

    

    /**
     * @brief Logs details of a CodeEvent callback.
     * @param sourceFile The source file where the log call originated.
     * @param line The line number in the source file.
     * @param callbackName The name of the callback function.
     * @param args The CodeEventArgs tuple.
     * @param logArgv If true, logs the arguments (argv) passed to the event.
     * @return True if logging was successful, false otherwise.
     */
    bool Logger::LogEventCallback(const char* sourceFile, const int line, const char* callbackName, CodeEventArgs args, bool logArgv)
    {
        bool success = true;
        CInstance* self = std::get<0>(args);
        // CInstance* other = std::get<1>(args);
        CCode* code = std::get<2>(args);
        int argc = std::get<4>(args);
        
        auto r_argv = (RValue**) std::get<3>(args);

        // Get current time for timestamp
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::tm local_tm;
        localtime_s(&local_tm, &now_time);
        std::ostringstream timestamp_ss;
        timestamp_ss << std::put_time(&local_tm, "%H:%M:%S");
        std::string timestamp = timestamp_ss.str();

        std::ostringstream infoStream;
        int& out_buf(argc);
        infoStream << g_LoggerInstance->ParseFormatting("[EVENT] [0x%p->%s() call 0x%p] [%s] @ [%s:%d]", self, callbackName, code, timestamp.c_str(), sourceFile, line) << "\n";
        argc = tryDereferencePtrLoop(argc, out_buf); // Use helper function
        infoStream << g_LoggerInstance->ParseFormatting("    Argument count: %d (dereferenced %d times)", argc,  out_buf) << "\n";

        success = g_LoggerInstance->LogFormatted("%s", infoStream.str().c_str());

        if (logArgv)
        {
            int current_argc = argc; // Use a local copy for argument count

            std::ostringstream argStream;
            g_LoggerInstance->LogSimple("    Arguments: { ");
            if (current_argc > 0)
            {
                bool use_multiline = current_argc > 3;
                for (int i = 0; i < current_argc; ++i)
                {
                    out_buf = 0;
                    // cast Rvalue* to int* as a pointer
                    int ptr_hak = tryDereferencePtrLoop((int)(r_argv[current_argc]), out_buf); 
                    // RValue& r_value = (RValue&)ptr_hak;

                    // RValue& r_value = *(RValue*)r_argv[i];
                    g_LoggerInstance->LogFormatted("\n (%p) %d: %s,",(void*) ptr_hak, current_argc, out_buf);

                    if (i < current_argc - 1)
                    {
                        
                    }
                }
                if (use_multiline) g_LoggerInstance->WriteToLog("   ");
            }
            //argStream << " }";
            //success = g_LoggerInstance->LogFormatted("%s", argStream.str().c_str()) && success;
        }
        return success;
    }
}