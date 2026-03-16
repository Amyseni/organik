#pragma once
#include "Synthetik.h"

namespace Brain {
    struct JSONMod {
        // These will represent a json file loaded from the user's Mods folders.
        // e.g Mods/Variant/MODNAME/variant.json or Mods/Item/MODNAME/item.json
        std::string m_Path;
        std::string m_Name;
        std::string m_Author;
    };
}