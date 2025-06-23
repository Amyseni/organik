#include "Utils.h"
#include <string>
#include <vector>
#include <sstream>
#include "YYToolkit/YYTK_Shared.hpp"


using namespace YYTK;
using namespace Aurie;

std::vector<std::string> Organik::Utils::splitString(const std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::string Organik::Utils::joinStrings(const std::vector<std::string>& words, const std::string& delimiter)
{
    std::string result = "";

    for (size_t i = 0; i < words.size(); ++i) {
        result += words[i];
        if (i < words.size() - 1) {
            result += delimiter;
        }
    }
    return result;
}
std::string Organik::Utils::joinRValStrings(const std::vector<YYTK::RValue>& words, const std::string& delimiter)
{
    std::string result = "";

    for (size_t i = 0; i < words.size(); ++i) {
        result += words[i].ToString();
        if (i < words.size() - 1) {
            result += delimiter;
        }
    }
    return result;
}