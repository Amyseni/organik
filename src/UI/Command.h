#pragma once
#include "../ModuleMain.h"
#include "YYToolkit/YYTK_Shared.hpp"
#include "Aurie/shared.hpp"
#include <string>
#include <vector>

namespace Organik
{
    // Forward declarations to avoid circular dependency
    class UIManager;
    struct OrganikConsole;
    static char parserHistory[1024 * 16]; // 16KB history buffer for command parsing
    
    class Command
    {
        struct ParamToken 
        {
            enum 
            {
                Global,
                Builtin,
                Member,
                Local,
                API,        // our API
            } Scope;
            std::string text;


            std::vector<ParamToken*> subTokens;
            PVOID ptr = nullptr;

            ParamToken(const std::string& text, auto scope = Global) // command line already handles quotes, so text is always a single argument.
                : text(text), Scope(scope) 
            {
                // if (text.empty())
            }
        };
    public:
        Command(const std::string& cmdName);
        virtual ~Command() = default;
        
        const std::string& Name() const { return _name; }
        
        virtual void Execute(OrganikConsole* console, const std::vector<std::string>& args) = 0;
        virtual void Help(OrganikConsole* console, const std::vector<std::string>& args) = 0;
        
    private:
        const std::string _name;
    };
}
