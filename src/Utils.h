#pragma once
#include <string>
#include <vector>
#include <sstream>
#include "YYToolkit/YYTK_Shared.hpp"

using namespace YYTK;
using namespace Aurie;

namespace Organik {
    namespace Utils {
        std::vector<std::string> splitString(const std::string& s, char delimiter);
        std::string joinStrings(const std::vector<std::string>& words, const std::string& delimiter = " ");
        std::string joinRValStrings(const std::vector<YYTK::RValue>& words, const std::string& delimiter = " ");
    }
}