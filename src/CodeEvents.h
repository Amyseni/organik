#pragma once
#include <Aurie/shared.hpp>
#include <YYToolkit/YYTK_Shared.hpp>
#include "CallbackManager/CallbackManagerInterface.h"

void OutgameCreateBefore(class std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);
void OutgameCreateAfter(class std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);
