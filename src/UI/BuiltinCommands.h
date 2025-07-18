#pragma once

#include <string>
#include <vector>
#include <functional>
#include "Command.h"
#include "YYToolkit/YYTK_Shared.hpp"
#include "Aurie/shared.hpp"


namespace Organik 
{
    struct OrganikConsole;
    namespace BuiltinCommands 
    {
        void AddCommandHandler(Command* cmd);
        std::map<std::string, Command*> GetCommands();
    }
}
#define COMMAND(name, helpDesc) \
    class name : public ::Organik::Command { \
    public: \
        name() : ::Organik::Command(#name) { } \
        void Execute(::Organik::OrganikConsole* console, const std::vector<std::string>& args) override; \
        void Help(::Organik::OrganikConsole* console, const std::vector<std::string>& args) override; \
    }; \
    void name::Help(::Organik::OrganikConsole* console, const std::vector<std::string>& args) { \
        console->AddLog(helpDesc); \
    } \
    void name::Execute(::Organik::OrganikConsole* console, const std::vector<std::string>& args)
