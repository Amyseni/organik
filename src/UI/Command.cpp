#include "Command.h"
#include "../ModuleMain.h"

namespace Organik
{
    Command::Command(const std::string& cmdName) : _name(cmdName)
    {
        Organik::GetLogger()->LogFormatted("%s - %s:%d", __FUNCTION__, __FILE__, __LINE__);
    }
}
