#pragma once

#include "YYToolkit/YYTK_Shared.hpp"
#include "Aurie/shared.hpp"
#include <string>
#include <vector>

namespace Organik
{
    // Forward declarations to avoid circular dependency
    class DevConsole;
    class UIManager;
    
    class Command
    {
    public:
        Command(const std::string& cmdName);
        virtual ~Command() = default;
        
        const std::string& Name() const { return _name; }
        
        virtual void Execute(DevConsole* console, const std::vector<std::string>& args) = 0;
        virtual void Help(DevConsole* console, const std::vector<std::string>& args) = 0;
        
    private:
        const std::string _name;
    };
}