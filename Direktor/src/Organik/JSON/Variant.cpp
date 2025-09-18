#include "Synthetik.h"
#include "zhl_internal.h"
#include "Globals.h"
#include "Variant.h"
#define SPRITE_INDEX(spr) Organik::Sprites::##spr
std::vector<int32_t>* GetVariantSprites()
{
    static std::vector<int32_t> variantSprites;
    if (variantSprites.size() == 0)
    {
        variantSprites = {
            SPRITE_INDEX(spr_variant_ultra),
            -1,
            SPRITE_INDEX(spr_variant_stabilized),
            SPRITE_INDEX(spr_variant_print),
            SPRITE_INDEX(spr_variant_scavenger),
            SPRITE_INDEX(spr_variant_heavy),
            SPRITE_INDEX(spr_variant_hunter),
            SPRITE_INDEX(spr_variant_milspec),
            SPRITE_INDEX(spr_variant_burstplus),
            SPRITE_INDEX(spr_variant_relic),
            SPRITE_INDEX(spr_variant_overclocked),
            SPRITE_INDEX(spr_variant_light),
            SPRITE_INDEX(spr_variant_prototype),
            SPRITE_INDEX(spr_variant_alpha),
            SPRITE_INDEX(spr_variant_siege),
            SPRITE_INDEX(spr_variant_cursed),
            SPRITE_INDEX(spr_variant_modular),
            SPRITE_INDEX(spr_variant_hyperburst),
            SPRITE_INDEX(spr_variant_timed),
            SPRITE_INDEX(spr_variant_divine),
            SPRITE_INDEX(spr_variant_timeless),
            SPRITE_INDEX(spr_variant_critical),
            SPRITE_INDEX(spr_variant_elite),
            SPRITE_INDEX(spr_variant_critical),
        };
    }
    return &variantSprites;
}
std::unordered_map<int32_t, Variant>* getUserVariants()
{
    static std::unordered_map<int32_t, Variant> userVariants;
    return &userVariants;
}
const int32_t Variant::VariantIcon[VARIANTS_END] = {
        /* ULTRA */         0,
        /* STANDARD */      0,
        /* STABILIZED */    0,
        /* PRINTING */      1,
        /* SCAV */          2,
        /* HEAVY */         3,
        /* TAIGA */         4,
        /* MILSPEC */       5,
        /* XBURST */        6,
        /* RELIC */         8,
        /* OVERCLOCKED */   0,
        /* LIGHTWEIGHT */   9,
        /* PROTOTYPE */     10,
        /* ALPHA */         11,
        /* SIEGE */         12,
        /* CURSED */        13,
        /* MODULAR */       14,
        /* BOUNCY */        15,
        /* TIMED */         0,
        /* DIVINE */        0,
        /* TIMELESS */      0,
        /* REACTIVE */      0,
        /* KAIDA */         0,
        /* CHARGED */       0
        /* END */
    };



// Organik::GetLogger()->LogSimple("gml_window_set_fullscreen called, initializing action handlers and variants.");
// RegisterActionTypeHandler(Code_Variable_FindAlloc_Slot_From_Name(nullptr, "apply_variables"), new DoAction([&](CInstance* self, CInstance* other, VariableMap* map, RValue* out) ->bool {
//     return doApplyVariables(self, other, map, out);
// }));
// RegisterActionTypeHandler(Code_Variable_FindAlloc_Slot_From_Name(nullptr, "call_script"), new DoAction([&](CInstance* self, CInstance* other, VariableMap* map, RValue* out) ->bool {
//     return doCallScript(self, other, map, out);
// }));
// RegisterActionTypeHandler(Code_Variable_FindAlloc_Slot_From_Name(nullptr, "call_builtin"), new DoAction([&](CInstance* self, CInstance* other, VariableMap* map, RValue* out) ->bool {
//     return doCallBuiltin(self, other, map, out);
// }));
// RegisterActionTypeHandler(Code_Variable_FindAlloc_Slot_From_Name(nullptr, "requires_variables"), new DoAction([&](CInstance* self, CInstance* other, VariableMap* map, RValue* out) ->bool {
//     return doRequiresVars(self, other, map, out);
// }));
// RegisterActionTypeHandler(Code_Variable_FindAlloc_Slot_From_Name(nullptr, "rng_chance"), new DoAction([&](CInstance* self, CInstance* other, VariableMap* map, RValue* out) ->bool {
//     return doRequiresChance(self, other, map, out);
// }));
// RegisterActionTypeHandler(Code_Variable_FindAlloc_Slot_From_Name(nullptr, "variable_get"), new DoAction([&](CInstance* self, CInstance* other, VariableMap* map, RValue* out) ->bool {
//     return doGetFrom(self, other, map, out);
// }));
// RegisterActionTypeHandler(Code_Variable_FindAlloc_Slot_From_Name(nullptr, "create_instance"), new DoAction([&](CInstance* self, CInstance* other, VariableMap* map, RValue* out) ->bool {
//     RValue outR;
//     std::vector<RValue*> args;
//     args.push_back(&(*map)[VAR_HASH(x)]);
//     args.push_back(&(*map)[VAR_HASH(y)]);
//     args.push_back(&(*map)[VAR_HASH(object)]);
//     return gml_Script_scr_instance_create(self, other, &outR, static_cast<int>(args.size()), args.data());
// }));
int32_t Variant::LoadVariant(const RValue& variantObject, std::unordered_map<int32_t, Variant>*& variantList)
{
    int32_t _hash = 0;
    if (!variantList)
        variantList = getUserVariants();
    
    YYObjectBase* convObj = (YYObjectBase*) variantObject.ToPointer();
    RValue *meta = convObj->InternalReadYYVar(
        Code_Variable_FindAlloc_Slot_From_Name(
				nullptr, const_cast<char*>("meta")
        )
    );
    RValue *statMultipliers = convObj->InternalReadYYVar(
        Code_Variable_FindAlloc_Slot_From_Name(
				nullptr, const_cast<char*>("stat_multipliers")
        )
    );
    RValue *statIncreases = convObj->InternalReadYYVar(
        Code_Variable_FindAlloc_Slot_From_Name(
				nullptr, const_cast<char*>("stat_increases")
        )
    );
    Variant* newVariantPtr = nullptr;
    if (meta)
    {
        if (!(meta->GetKind() == VALUE_OBJECT))
        {
            GetLogger()->LogFormatted("Variant meta is not an object, got kind %d", meta->GetKind());
            Error_Show_Action("Variant meta must be an object.", true, true);
            return -1;
        }
        Organik::GetLogger()->LogSimple("Loading variant metadata...");
        YYObjectBase* metaObj = meta->ToObject();
        Organik::GetLogger()->LogSimple("Loading variant metadata...");
        RValue *replaceVariantRV = metaObj->InternalReadYYVar(
            Code_Variable_FindAlloc_Slot_From_Name(
				nullptr, const_cast<char*>("replace_variant")
            )
        );
        RValue *nameRV = metaObj->InternalReadYYVar(
            Code_Variable_FindAlloc_Slot_From_Name(
				nullptr, const_cast<char*>("name")
            )
        );
        int32_t variantHash = 0;
        {
            if (!nameRV)
            {
                GetLogger()->LogFormatted("Variant in %p is missing a name.", (void*)metaObj);
                Error_Show_Action("Variant must have a name.", true, true);
                return -1;
            }
            std::string name = nameRV->ToCString();
            if (nameRV)
            {
                name = (nameRV->ToCString());
            }
            if (replaceVariantRV)
            {
                variantHash = parseRValueNumber<int32_t>(replaceVariantRV);
            }
            if (!variantHash)
            {
                variantHash = Code_Variable_FindAlloc_Slot_From_Name(nullptr, name.data());
            }

            if (variantHash >= 0)
            {
                (*variantList)[variantHash] = Variant();
                newVariantPtr = &((*variantList)[variantHash]);
            }
            else
            {
                    Error_Show_Action("Variant name hash is invalid.", true, true);
                    return -1;
            }
        }
        Organik::GetLogger()->LogFormatted("Creating variant with hash %d", variantHash);
        if (!newVariantPtr)
        {
            Error_Show_Action("Failed to create variant.", true, true);
            return 0;
        }
        RValue *nameshortRV = metaObj->InternalReadYYVar(
            Code_Variable_FindAlloc_Slot_From_Name(
				nullptr, const_cast<char*>("name_short")
            )
        );
        RValue *descRV = metaObj->InternalReadYYVar(
            Code_Variable_FindAlloc_Slot_From_Name(
				nullptr, const_cast<char*>("desc")
            )
        );
        RValue *manuRV = metaObj->InternalReadYYVar(
            Code_Variable_FindAlloc_Slot_From_Name(
				nullptr, const_cast<char*>("manufacturer")
            )
        );
        
        RValue *colorRV = metaObj->InternalReadYYVar(
            Code_Variable_FindAlloc_Slot_From_Name(
				nullptr, const_cast<char*>("color")
            )
        );
        RValue *rarityRV = metaObj->InternalReadYYVar(
            Code_Variable_FindAlloc_Slot_From_Name(
				nullptr, const_cast<char*>("rarity")
            )
        );
        RValue *iconRV = metaObj->InternalReadYYVar(
            Code_Variable_FindAlloc_Slot_From_Name(
				nullptr, const_cast<char*>("icon")
            )
        );
        RValue *soundRV = metaObj->InternalReadYYVar(
            Code_Variable_FindAlloc_Slot_From_Name(
				nullptr, const_cast<char*>("sound")
            )
        );
        RValue *spawnRV = metaObj->InternalReadYYVar(
            Code_Variable_FindAlloc_Slot_From_Name(
				nullptr, const_cast<char*>("spawn")
            )
        );
        std::string name = "ORGANIK";
        std::string nameshort = "ORG";
        std::string desc = "A fictional GUN product.";
        std::string manufacturer = "ORGANIK Corp.";
        
        if (nameRV)
        {
            name = std::string(nameRV->ToCString());
        }
        if (nameshortRV)
        {
            nameshort = std::string(nameshortRV->ToCString());
        }
        if (descRV)
        {
            if (descRV->GetKind() == VALUE_STRING)
            {
                desc = std::string(descRV->ToCString());
            }
            else if (descRV->GetKind() == VALUE_ARRAY)
            {
                auto descLines = descRV->ToRefVector();
                desc.clear();
                desc.reserve(VARIANT_DESC_MAX_LENGTH);
                for (const auto& line : descLines)
                {
                    desc += line->ToCString();
                    desc += "\n";
                    if (desc.length() > VARIANT_DESC_MAX_LENGTH)
                        break;
                }
            }
        }
        if (manuRV)
        {
            manufacturer = std::string(manuRV->ToCString());
        }
        if (iconRV)
        {
            int32_t spriteID = DoBuiltin(&gml_asset_get_index, { *iconRV }).ToInt32();
            newVariantPtr->spriteID = static_cast<double>(spriteID);
        }
        else
        {
            newVariantPtr->spriteID = -1;
        }
        Organik::GetLogger()->LogFormatted("Loaded variant icon: %f", newVariantPtr->spriteID);
        if (replaceVariantRV)
        {
            newVariantPtr->variantHash = parseRValueNumber<int32_t>(replaceVariantRV);
        }
        else
        {
            newVariantPtr->variantHash = Code_Variable_FindAlloc_Slot_From_Name(nullptr, const_cast<char*>(newVariantPtr->name.c_str()));
        }
        std::string fileContents;
        if (soundRV)
        {
            int32_t soundID = DoBuiltin(&gml_asset_get_index, { *soundRV }).ToInt32();
            newVariantPtr->soundID = static_cast<double>(soundID);
        }
        else
        {
            newVariantPtr->soundID = -1;
        }
        Organik::GetLogger()->LogFormatted("Loaded variant sound: %f", newVariantPtr->soundID);
        if (spawnRV)
        {
            int32_t spawnID = DoBuiltin(&gml_asset_get_index, { *spawnRV }).ToInt32();
            newVariantPtr->spawnID = static_cast<double>(spawnID);
        }
        else
        {
            newVariantPtr->spawnID = -1;
        }
        Organik::GetLogger()->LogFormatted("Loaded variant spawn: %f", newVariantPtr->spawnID);
        if (rarityRV)
        {
            newVariantPtr->rarity = parseRValueNumber<double>(rarityRV);
        }
        else
        {
            newVariantPtr->rarity = Rarities::COMMON;
        }
        Organik::GetLogger()->LogFormatted("Loaded variant rarity: %f", newVariantPtr->rarity);
        if (colorRV)
        {
            newVariantPtr->color = parseRValueNumber<double>(colorRV);
        }
        else
        {
            newVariantPtr->color = Rarities::RarityColor[static_cast<int32_t>(newVariantPtr->rarity)];
        }
        
        newVariantPtr->name = name;
        newVariantPtr->desc = desc;
        newVariantPtr->manufacturer = manufacturer;
        Organik::GetLogger()->LogFormatted("Loaded variant: %s -- %s, %s, %f %f %f", name.c_str(), desc.c_str(), manufacturer.c_str(), newVariantPtr->rarity, newVariantPtr->color, newVariantPtr->spriteID);
    }
    
    if (statMultipliers)
    {
        YYObjectBase* statMultipliersObj = (YYObjectBase*)statMultipliers->ToPointer();
        RValue names = DoBuiltin(&gml_variable_instance_get_names, { *statMultipliers });
        auto nameList = names.ToRefVector();
        for (const auto& name : nameList)
        {
            int32_t hash = Code_Variable_FindAlloc_Slot_From_Name(nullptr, const_cast<char*>(name->ToCString()));
            double value = parseRValueNumber<double>(statMultipliersObj->InternalGetYYVarRef(hash));
            newVariantPtr->statMultipliers[hash] = value;
            Organik::GetLogger()->LogFormatted("  Stat multiplier: %s -- %f", name->ToCString(), value);
        }
    }
    
    if (statIncreases)
    {
        YYObjectBase* statIncreasesObj = (YYObjectBase*)statIncreases->ToPointer();
        RValue names = DoBuiltin(&gml_variable_instance_get_names, { *statIncreases });
        auto nameList = names.ToRefVector();
        for (const auto& name : nameList)
        {
            int32_t hash = Code_Variable_FindAlloc_Slot_From_Name(nullptr, const_cast<char*>(name->ToCString()));
            double value = parseRValueNumber<double>(statIncreasesObj->InternalGetYYVarRef(hash));
            newVariantPtr->statIncreases[hash] = value;
            Organik::GetLogger()->LogFormatted("  Stat increase: %s -- %f", name->ToCString(), value);
        }
    }
    
    RValue* triggers = convObj->InternalReadYYVar(
        Code_Variable_FindAlloc_Slot_From_Name(
				nullptr, const_cast<char*>("triggers")
        )
    );
    if (triggers->GetKind() != VALUE_OBJECT)
    {
        Error_Show_Action("Expected triggers to be an object", true, true);
        return newVariantPtr->variantHash;
    }
    return newVariantPtr->variantHash;
}