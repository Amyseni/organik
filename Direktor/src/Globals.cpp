#include "Synthetik.h"
#include <xstring>
#include <string.h>
#include "Globals.h"
#include "Variant.h"
#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

double g_ReplaceBulletIndex = 0;
double* getReplaceBulletIndex() {
    return &g_ReplaceBulletIndex;
}
void DrawObjectSelector(std::function<void(int32_t)> callback)
{
    static ImGuiTextFilter filter;
    ImGui::Text("Object search: \n"
        "  Type to filter objects by name (ignore obj_)\n"
        "  \"^enm_\" - display objects starting with obj_enm_\"\n"
        "  \"grenade,missile\" display objects containing \"grenade\" or \"missile\"\n"
        "  \"-boss\" exclude objects containing \"beam\"\n"
        "Click on an object to select it.\n"
    );
    filter.Draw("Filter");
    if (ImGui::BeginChild("##itemList", ImVec2(0.0, 250), ImGuiChildFlags_Border))
    {
        for (uint32_t i = 0; i < Organik::Objects::ObjIndexes.size(); i++)
        {
            const char* objName = Organik::Objects::ObjectNamesArr[i];
            const int32_t objIndex = Organik::Objects::ObjIndexes[i];
            const uint32_t objNameLen = strlen(objName);
            if (strlen(filter.InputBuf))
            {
                if (!filter.PassFilter(objName))
                    continue;
            }
            if (ImGui::Selectable(objName, false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap))
            {
                if (callback)
                {
                    (callback)(objIndex);
                }
            }
        }
    }
    ImGui::EndChild();
}
std::vector<int32_t>* getPossibleBullets() {
    static std::vector<int32_t> possibleBullets;
    return &possibleBullets;
}



bool* GetLogAllCalls()
{
    static bool logAllCalls = false;
    return &logAllCalls;
}

std::unordered_map<int32_t, CInstance*>* GetGlobalInstances()
{
    static std::unordered_map<int32_t, CInstance*> globalInstances = {
        { 1, nullptr }, //? outgame
        { 2, nullptr }, //? obj_stats
        { 3, nullptr }, //? obj_control
        { 4, nullptr }, //? obj_allgame
        { 2, nullptr }, //? 
        { 2, nullptr }, //? 

    };
    return &globalInstances;
}
void Variant::Apply(CInstance* instance, int variant, int sClass, int subclass, int subclassLevel)
{
	GetLogger()->LogFormatted("%s -- %d", __FUNCTION__, __LINE__);
    if (!instance)
   	return;
        CInstance* statsObj = CInstance::FirstOrDefault([](CInstance* ci) -> bool {
        return ci && ci->m_ObjectIndex == Organik::Objects::ObjIndexes[Organik::Objects::obj_stats];
    });
    if (!statsObj)
        return;
	GetLogger()->LogFormatted("Applying variant %d to instance %d (class %d, subclass %d, subclass level %d)", variant, instance->m_ID, sClass, subclass, subclassLevel);
    RValue* weaponvarianticon = instance->InternalGetYYVarRef(VAR_HASH(weaponvarianticon));
    if ((weaponvarianticon->GetKind() & MASK_KIND_RVALUE)== VALUE_UNDEFINED)
        *weaponvarianticon = RValue(-1.0);
    RValue* weaponvarianticoncolor = instance->InternalGetYYVarRef(VAR_HASH(weaponvarianticoncolor));
    RValue* variantsprite = instance->InternalGetYYVarRef(VAR_HASH(variantsprite));
    RValue* variantcolor = instance->InternalGetYYVarRef(VAR_HASH(variantcolor));
    RValue* weaponvariantname = instance->InternalGetYYVarRef(VAR_HASH(weaponvariantname));
    RValue* weaponvariantnameshort = instance->InternalGetYYVarRef(VAR_HASH(weaponvariantnameshort));
    RValue* variantmanufacturer = instance->InternalGetYYVarRef(VAR_HASH(variantmanufacturer));
    RValue* varianttext = instance->InternalGetYYVarRef(VAR_HASH(varianttext));
    RValue* varianttextnegative = instance->InternalGetYYVarRef(VAR_HASH(varianttextnegative));
    RValue* mod_magazinebonus = statsObj->InternalGetYYVarRef(VAR_HASH(mod_magazinebonus));
    RValue* mod_magazinesize = statsObj->InternalGetYYVarRef(VAR_HASH(mod_magazinesize)); 
    RValue* default_magsize_rounding = instance->InternalGetYYVarRef(VAR_HASH(default_magsize_rounding));
    RValue* ammo_max = instance->InternalGetYYVarRef(VAR_HASH(ammo_max));
    RValue* screenshakeamount = instance->InternalGetYYVarRef(VAR_HASH(screenshakeamount));
    RValue* tooltip_rarity = instance->InternalGetYYVarRef(VAR_HASH(tooltip_rarity));
    RValue* rollvariant = instance->InternalGetYYVarRef(VAR_HASH(rollvariant));

    if ((weaponvarianticoncolor->GetKind() & MASK_KIND_RVALUE)== VALUE_UNDEFINED || ((weaponvarianticoncolor->GetKind() & MASK_KIND_RVALUE)& MASK_KIND_RVALUE)== VALUE_UNSET)
        *weaponvarianticoncolor = RValue(Rarities::RarityColor[Rarities::COMMON]);
    if ((variantsprite->GetKind() & MASK_KIND_RVALUE)== VALUE_UNDEFINED || (variantsprite->GetKind() & MASK_KIND_RVALUE)== VALUE_UNSET)
        *variantsprite = RValue(-1.0);
    if ((variantcolor->GetKind() & MASK_KIND_RVALUE)== VALUE_UNDEFINED || (variantcolor->GetKind() & MASK_KIND_RVALUE)== VALUE_UNSET)
        *variantcolor = RValue(RValue(Rarities::RarityColor[Rarities::COMMON]));
    if ((weaponvariantname->GetKind() & MASK_KIND_RVALUE) == VALUE_UNDEFINED || (weaponvariantname->GetKind() & MASK_KIND_RVALUE)== VALUE_UNSET)
        YYCreateString(weaponvariantname, "NAME"); ;
    if ((weaponvariantnameshort->GetKind() & MASK_KIND_RVALUE)== VALUE_UNDEFINED || (weaponvariantnameshort->GetKind() & MASK_KIND_RVALUE)== VALUE_UNSET)
        YYCreateString(weaponvariantnameshort, "GAY"); 
    if ((variantmanufacturer->GetKind() & MASK_KIND_RVALUE)== VALUE_UNDEFINED || (variantmanufacturer->GetKind() & MASK_KIND_RVALUE)== VALUE_UNSET)
        YYCreateString(variantmanufacturer, "ORGANIK"); 
    if ((varianttext->GetKind() & MASK_KIND_RVALUE)== VALUE_UNDEFINED || (varianttext->GetKind() & MASK_KIND_RVALUE)== VALUE_UNSET)
        YYCreateString(varianttext, "PLACEHOLDER");
    if ((varianttextnegative->GetKind() & MASK_KIND_RVALUE)== VALUE_UNDEFINED || (varianttextnegative->GetKind() & MASK_KIND_RVALUE)== VALUE_UNSET)
        YYCreateString(varianttextnegative, "PLACEHOLDER");
    if ((mod_magazinebonus->GetKind() & MASK_KIND_RVALUE)== VALUE_UNDEFINED || (mod_magazinebonus->GetKind() & MASK_KIND_RVALUE)== VALUE_UNSET)
        *mod_magazinebonus = RValue(0.01);
    if ((mod_magazinesize->GetKind() & MASK_KIND_RVALUE)== VALUE_UNDEFINED || (mod_magazinesize->GetKind() & MASK_KIND_RVALUE)== VALUE_UNSET)
        *mod_magazinesize = RValue(1.0);
    if ((default_magsize_rounding->GetKind() & MASK_KIND_RVALUE)== VALUE_UNDEFINED || (default_magsize_rounding->GetKind() & MASK_KIND_RVALUE)== VALUE_UNSET)
        *default_magsize_rounding = RValue(1.0);
    if ((ammo_max->GetKind() & MASK_KIND_RVALUE)== VALUE_UNDEFINED || (ammo_max->GetKind() & MASK_KIND_RVALUE)== VALUE_UNSET)
        *ammo_max = RValue(60.0);
    if ((screenshakeamount->GetKind() & MASK_KIND_RVALUE)== VALUE_UNDEFINED || (screenshakeamount->GetKind() & MASK_KIND_RVALUE)== VALUE_UNSET)
        *screenshakeamount = RValue(0.0);
    if ((tooltip_rarity->GetKind() & MASK_KIND_RVALUE)== VALUE_UNDEFINED || (tooltip_rarity->GetKind() & MASK_KIND_RVALUE)== VALUE_UNSET)
        *tooltip_rarity = RValue(1.0);
    if ((rollvariant->GetKind() & MASK_KIND_RVALUE)== VALUE_UNDEFINED || (rollvariant->GetKind() & MASK_KIND_RVALUE)== VALUE_UNSET)
        *rollvariant = RValue(variant);
    // RValue* spreadpershot = instance->InternalGetYYVarRef(VAR_HASH(spreadpershot));
    // RValue* heatrecoil = instance->InternalGetYYVarRef(VAR_HASH(heatrecoil));
    // RValue* spreadmaximum = instance->InternalGetYYVarRef(VAR_HASH(spreadmaximum));
    // RValue* reloadtimeinit = instance->InternalGetYYVarRef(VAR_HASH(reloadtimeinit));
    // RValue* bulletdamagemodifier2 = instance->InternalGetYYVarRef(VAR_HASH(bulletdamagemodifier2));
    // RValue* fireratemod = instance->InternalGetYYVarRef(VAR_HASH(fireratemod));
    // RValue* spreadminimum = instance->InternalGetYYVarRef(VAR_HASH(spreadminimum));
    // RValue* ammogainmulti = instance->InternalGetYYVarRef(VAR_HASH(ammogainmulti));
    // RValue* ejectiontimeinit = instance->InternalGetYYVarRef(VAR_HASH(ejectiontimeinit));
    // RValue* coolingmodifier = instance->InternalGetYYVarRef(VAR_HASH(coolingmodifier));
    // RValue* doubleshotchance = instance->InternalGetYYVarRef(VAR_HASH(doubleshotchance));
    // RValue* ammoregeninit = instance->InternalGetYYVarRef(VAR_HASH(ammoregeninit));
    // RValue* effectstext = instance->InternalGetYYVarRef(VAR_HASH(effectstext));
    // RValue* effecticon = instance->InternalGetYYVarRef(VAR_HASH(effecticon));
    // RValue* effectinfo = instance->InternalGetYYVarRef(VAR_HASH(effectinfo));
    // Apply the variant to the instance
    *screenshakeamount = RValue(0.0);
    if (variant < 100000)
    {
        *instance->InternalGetYYVarRef(VAR_HASH(weaponvarianticon)) = RValue(Variant::VariantIcon[variant]);
        *instance->InternalGetYYVarRef(VAR_HASH(variantsprite)) = RValue(static_cast<double>((*GetVariantSprites())[variant]));
    }
    std::vector<RValue*> localizationArgs;
    RValue localizationText;
    RValue wepX = RValue(static_cast<double>(instance->m_X));
    RValue wepY = RValue(static_cast<double>(instance->m_Y));
    RValue divineDropLightning = RValue(Organik::Objects::ObjIndexes[Organik::Objects::obj_lightning_strike_divine]);
    RValue unused;
    RValue arg;
    RValue arg1;
    RValue arg2;
    auto rng = Utils::getrng();
    double nextRoll = ((*rng)() % 200) / 100.0 + 0.25;
    RValue ach;
    std::vector<RValue*> args;
    std::unordered_map<int32_t, double> statIncreases;
    std::unordered_map<int32_t, double> statMultipliers;
	GetLogger()->LogFormatted("%s -- %d", __FUNCTION__, __LINE__);

    doLocalizationScriptAlt(localizationText, arg1, localizationArgs, variantmanufacturer, "@1", "ORGANIK");
    std::string variantDesc = "ORGANIK\n";
    if (getUserVariants()->contains(variant))
    {
        Variant userVariant = (*getUserVariants())[variant];
        // Organik::ActionRegistry::GetInstance()->ActivateActionsForVariant(variant, instance);
        // Organik::GetLogger()->LogFormatted("Activated triggers for user variant '%s' on instance %d", 
        //     userVariant.name.c_str(), instance->m_ID);
        doLocalizationScriptAlt(localizationText, arg1, localizationArgs, weaponvariantname, "@1", userVariant.name.c_str());
        doLocalizationScriptAlt(arg2, arg1, localizationArgs, weaponvariantnameshort, "@1", userVariant.name.substr(0, 5).c_str());
        doLocalizationScriptAlt(arg2, arg1, localizationArgs, variantmanufacturer, "@1", userVariant.manufacturer.c_str());
        *tooltip_rarity = min(7.0, RValue(static_cast<double>(userVariant.rarity)).ToDouble());
        *variantcolor = RValue(static_cast<double>(userVariant.color));
        *weaponvarianticon = RValue(-1);
        *rollvariant = RValue(variant);
        *variantsprite = RValue(static_cast<double>(userVariant.spriteID));
        RValue spawnID = RValue(static_cast<double>(userVariant.spawnID));
        RValue soundID = RValue(static_cast<double>(userVariant.soundID));
        RValue wepX = RValue(static_cast<double>(instance->m_X));
        RValue wepY = RValue(static_cast<double>(instance->m_Y));
        for( const auto& [statHash, increase] : userVariant.statIncreases)
        {
            if (statHash == VAR_HASH(bulletObj))
            {
                if (getPossibleBullets()->size() > 0)
                {
                    int32_t newBulletObj = (*getPossibleBullets())[(*rng)() % getPossibleBullets()->size()];
                    *instance->InternalGetYYVarRef(VAR_HASH(bulletObj)) = RValue(static_cast<double>(newBulletObj));
                    *instance->InternalGetYYVarRef(VAR_HASH(Organik_BulletObj)) = RValue(static_cast<double>(newBulletObj));
                }
                continue;
            }
            else if (increase == 0.0)
            {
                *instance->InternalGetYYVarRef(statHash) = RValue(0.0);
                continue;
            }
            statIncreases[statHash] = increase;
        }
        for( const auto& [statHash, multiplier] : userVariant.statMultipliers)
        {
            statMultipliers[statHash] = multiplier;
        }
        if (spawnID.ToDouble() != -1.0)
        {
            std::vector<RValue*> args = {&wepX, &wepY, &spawnID};
            gml_Script_scr_instance_create(instance, instance, &unused, static_cast<int>(args.size()), args.data());
        }
        if (soundID.ToDouble() != -1.0)
        {
            DoBuiltin(&gml_audio_play_sound, { soundID, RValue(15.0), RValue(false) });
        }
    }

	GetLogger()->LogFormatted("%s -- %d", __FUNCTION__, __LINE__);
    std::string variant_text;
    for (const auto& [statHash, increase] : statIncreases)
    {
        RValue* statVar = instance->InternalGetYYVarRef(statHash);
        if (statVar)
        {
            *statVar = RValue(statVar->ToDouble() + increase);

            variant_text += GetLogger()->ParseFormatting("%s + %.2f\n", Variables::HashToVariableMap[statHash], increase);
        }
    }
    for (const auto& [statHash, multiplier] : statMultipliers)
    {
        RValue* statVar = instance->InternalGetYYVarRef(statHash);
        if (statVar)
        {
            *statVar = RValue(statVar->ToDouble() * multiplier);

            variant_text += GetLogger()->ParseFormatting("%s x %.2f\n", Variables::HashToVariableMap[statHash], multiplier);
        }
    }
    
    std::vector<RValue*> argsVec;
    double ammoMaxVal = static_cast<int32_t>(parseRValueNumber<double>(ammo_max)) ? parseRValueNumber<double>(ammo_max) : 1.0;
    double defaultMagSizeRoundingVal = static_cast<int32_t>(parseRValueNumber<double>(default_magsize_rounding)) ? parseRValueNumber<double>(default_magsize_rounding) : 1.0;
    double modMagBonus = static_cast<int32_t>(parseRValueNumber<double>(mod_magazinebonus)) ? parseRValueNumber<double>(mod_magazinebonus) : 0.0;
    double modMagSize = static_cast<int32_t>(parseRValueNumber<double>(mod_magazinesize)) ? parseRValueNumber<double>(mod_magazinesize) : 1.0;
    if (modMagBonus < 0.0)
        modMagBonus = 0.01;
    if (modMagSize < 1.0)
        modMagSize  = 1.01;
    if (defaultMagSizeRoundingVal < 1.0)
        defaultMagSizeRoundingVal = 1.01;
    *mod_magazinebonus = RValue(modMagBonus);
    *mod_magazinesize = RValue(modMagSize);
    defaultMagSizeRoundingVal = round(defaultMagSizeRoundingVal);
    
    ammoMaxVal = round(defaultMagSizeRoundingVal);
    ammoMaxVal = round((ammoMaxVal + modMagBonus) * modMagSize);
    if (ammoMaxVal < 1.0)
        ammoMaxVal  = 1.01; 
    *default_magsize_rounding = RValue(defaultMagSizeRoundingVal);
    *ammo_max = RValue(ammoMaxVal);
    doLocalizationScriptAlt(arg, arg2, argsVec, varianttext, "@1", YYStrDup(const_cast<char*>(variant_text.length() ? variant_text.c_str() : "[BLANK]"))); //? I'm pretty sure this hacks the matrix
    doLocalizationScriptAlt(arg1, arg2, argsVec, varianttextnegative, "@1", YYStrDup(const_cast<char*>(
        "ORGANIK"
    )));
}
    
const std::vector<int32>& getNonArenaChests()
{
    static std::vector<int32> nonArenaChests;
    if (nonArenaChests.empty())
    {
        nonArenaChests = {
            Organik::Objects::ObjIndexes[Organik::Objects::obj_chest_weapon_metal],
            Organik::Objects::ObjIndexes[Organik::Objects::obj_chest_weapon_timed],
            Organik::Objects::ObjIndexes[Organik::Objects::obj_chest_weapon_challenge],
            Organik::Objects::ObjIndexes[Organik::Objects::obj_chest_weapon_metal_cost],
            Organik::Objects::ObjIndexes[Organik::Objects::obj_chest_classperk],
            Organik::Objects::ObjIndexes[Organik::Objects::obj_chest_artefact],
            Organik::Objects::ObjIndexes[Organik::Objects::obj_chest_weapon_wood],
            Organik::Objects::ObjIndexes[Organik::Objects::obj_chest_item_timed],
            Organik::Objects::ObjIndexes[Organik::Objects::obj_chest_item_boss],
            Organik::Objects::ObjIndexes[Organik::Objects::obj_chest_weapon_boss],
            Organik::Objects::ObjIndexes[Organik::Objects::obj_chest_mysteryboost],
            Organik::Objects::ObjIndexes[Organik::Objects::obj_chest_mysteryboost],
            Organik::Objects::ObjIndexes[Organik::Objects::obj_chest_mystery],
            Organik::Objects::ObjIndexes[Organik::Objects::obj_chest_unlockammo],
        };
    }
    return nonArenaChests;
}
CInstance* GetActionGlobalSelf()
{
    static CInstance* actionGlobalSelf = nullptr;
    return actionGlobalSelf;
}
CInstance* GetActionGlobalOther()
{
    static CInstance* actionGlobalOther = nullptr;
    return actionGlobalOther;
}