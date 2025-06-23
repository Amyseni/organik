#pragma once

#include <string>
#include <vector>
#include <functional>
#include "Command.h"
#include "UIManager.h"
#include "DevConsole.h"
#include "YYToolkit/YYTK_Shared.hpp"
#include "Aurie/shared.hpp"


#define COMMAND(name, helpDesc) \
    class name : public ::Organik::Command { \
    public: \
        name() : ::Organik::Command(#name) { } \
        void Execute(::Organik::DevConsole* console, const std::vector<std::string>& args) override; \
        void Help(::Organik::DevConsole* console, const std::vector<std::string>& args) override; \
    }; \
    void name::Help(::Organik::DevConsole* console, const std::vector<std::string>& args) { \
        console->WriteLine(helpDesc); \
    } \
    void name::Execute(::Organik::DevConsole* console, const std::vector<std::string>& args)

namespace Organik 
{
    namespace BuiltinCommands 
    {
        void InstallCommands();
    }
}