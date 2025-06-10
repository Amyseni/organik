#pragma once
#include <Aurie/shared.hpp>
#include <YYToolkit/YYTK_Shared.hpp>
#include "CallbackManager/CallbackManagerInterface.h"

void OutgameCreateBefore(class std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);
void OutgameCreateAfter(class std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void EnmBaseParentCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);
void EnmBaseParentCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void CursorCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);
void CursorCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void BuffTimedCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);
void BuffTimedCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void CursorCameraDrawBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void BuffStatboostsCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void UpgradeMenuCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void LastKillCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void LastKillDrawBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);
void LastKillDrawAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void MainUiS1PreCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);
void MainUiS1PreCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void MainUiWeaponPreCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);
void MainUiWeaponPreCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void RollClassItemCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);
void RollClassItemCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void SelectionMenuCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);
void SelectionMenuCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void SelectionMenuPerkCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);
void SelectionMenuPerkCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void AndroidConsoleCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);
void AndroidConsoleCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void SelectionMenuStatCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);
void SelectionMenuStatCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void EscapeSettingsCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void DialogUiCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void PauseMenuCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void ReloadingPreCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void ReloadingCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);
void ReloadingCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void ReloadingStep2Before(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);
void ReloadingStep2After(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void ItemStatsCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);
void ItemStatsCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void ItemStatsDrawBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);
void ItemStatsDrawAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void StatisticsCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);
void StatisticsCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void OverlaysAboveUiCreateBefore(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);
void OverlaysAboveUiCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void MainUiS1DrawAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void MainUiWeaponDrawAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void SelectionMenuDrawAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void EscapeSettingsDrawAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void AndroidConsoleDrawAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void DialogUiDrawAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void PauseMenuDrawAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void ReloadingDrawAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);

void OverlaysAboveUiDrawAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>&);
