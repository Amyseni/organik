#include <Arkitekt/Shared.hpp>
#include <fstream>

std::shared_mutex Arkitekt::Logger::logMutex = std::shared_mutex();
/**
 * @brief Singleton instance of the Logger.
 */
Arkitekt::Logger* Arkitekt::GetLogger(void)
{
    static Logger g_LoggerInstance;
    return &g_LoggerInstance;
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
}

bool Logger::Init(const std::string& filename)
{
    std::lock_guard<std::shared_mutex> lock(Arkitekt::Logger::logMutex);
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
    std::lock_guard<std::shared_mutex> lock(logMutex);
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
    size_t length = strnlen_s(Format, max_length);

    if (length >= max_length)
        return "<string too long to print>";

    char buffer[max_length] = { 0 };
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
    std::lock_guard<std::shared_mutex> lock(logMutex);
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
    std::string formatted_output = ParseFormatting(fmt, va_args);
    va_end(va_args);
    return WriteToLog(formatted_output, true);
}