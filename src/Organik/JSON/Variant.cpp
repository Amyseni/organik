#include "Synthetik.h"
#include "zhl_internal.h"
#include "Globals.h"
#include "Variant.h"
#include "Action.h"

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
void LoadVariants() {
    getUserVariants()->clear();
    auto pwd = std::filesystem::current_path();
    Organik::GetLogger()->LogFormatted("Current working directory (variant init/reload start): %s", pwd.generic_string().c_str());
    if (!std::filesystem::exists(pwd / "Mods"))
    {
        std::filesystem::create_directory(pwd / "Mods");
    }
    auto modsFldr = pwd / "Mods";
    if (!std::filesystem::exists(modsFldr / "Variants"))
    {
        std::filesystem::create_directory(modsFldr / "Variants");
    }
    auto it = std::filesystem::directory_iterator(modsFldr / "Variants");
    for (const auto& entry : it)
    {
        if (entry.is_directory() && std::filesystem::exists(entry.path() / "variant.json"))
        {
            RValue variantObject;
            {
                std::ifstream inFile((entry.path() / "variant.json").generic_string());
                std::ostringstream strStream;
                std::string line;
                while (std::getline(inFile, line))
                {
                    strStream << line << "\n";
                }
                RValue fileContentsRV;
                YYCreateString(&fileContentsRV, strStream.str().c_str());
                variantObject = DoBuiltin(&gml_json_parse, { fileContentsRV });
                if (variantObject.m_Kind != VALUE_OBJECT && variantObject.m_Kind != VALUE_ARRAY)
                {
                    Error_Show_Action("Failed to parse variant.json file.", true, true);
                    //! program terminates
                }
            }
            Organik::GetLogger()->LogFormatted("Loading variant from %s", entry.path().generic_string().c_str());
            
            {
                Organik::GetLogger()->LogFormatted("Variant object kind: %d", variantObject.m_Kind);
                auto UserVariants = (getUserVariants());
                if (variantObject.m_Kind == VALUE_ARRAY)
                {

                    Organik::GetLogger()->LogFormatted("Loading variants from array, current size: %d", (int) UserVariants->size());
                    auto arr = variantObject.ToRefVector();
                    int32_t count = variantObject.GetArrayLength();
                    for (int i = 0; i < count; i++)
                    {
                        auto item = arr[i];
                        if (!item) break;

                        Organik::GetLogger()->LogFormatted("Checking item kind: %d", item->GetKind());
                        if (item->GetKind() == VALUE_OBJECT)
                        {
                            Variant::LoadVariant(*item, UserVariants);
                            RValue *triggers = variantObject.ToObject()->InternalReadYYVar(
                            VAR_HASH(triggers));

                            if (triggers && triggers->m_Kind == VALUE_ARRAY)
                            {
                                for (auto& triggersObj : triggers->ToRefVector())
                                {
                                    if (triggersObj->GetKind() != VALUE_OBJECT)
                                    {
                                        Error_Show_Action("Expected trigger to be an object", true, true);
                                        continue;
                                    }
                                    ParseEventTriggers(*triggersObj, entry);
                                }
                            }
                        }
                    }
                }
                else if (variantObject.m_Kind == VALUE_OBJECT)
                {

                    Organik::GetLogger()->LogFormatted("Loading variant from: %s", (entry.path() / "variant.json").parent_path().generic_string().c_str());
                    Variant::LoadVariant(variantObject, UserVariants);
                    if (variantObject.ToObject()->InternalReadYYVar(
                        VAR_HASH(triggers)))
                    {
                        RValue *triggers = variantObject.ToObject()->InternalReadYYVar(
                            VAR_HASH(triggers));

                        if (triggers && triggers->m_Kind == VALUE_ARRAY)
                        {
                            for (auto& triggersObj : triggers->ToRefVector())
                            {
                                if (triggersObj->GetKind() != VALUE_OBJECT)
                                {
                                    Error_Show_Action("Expected trigger to be an object", true, true);
                                    continue;
                                }
                                ParseEventTriggers(*triggersObj, entry);
                            }
                        }
                    }
                }
            }
            
            Organik::GetLogger()->LogFormatted("Finished loading variant from %s", entry.path().generic_string().c_str());
                
        }
    }
}
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
            else
            {
                variantHash = Code_Variable_FindAlloc_Slot_From_Name(nullptr, name.data());
            }
            
            (*variantList)[variantHash] = Variant();
            newVariantPtr = &((*variantList)[variantHash]);
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
            newVariantPtr->rarity = 1.0;
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
    
    return newVariantPtr->variantHash;
}

void ParseEventTriggers(const RValue& trigger, const std::filesystem::directory_entry& entry)
{
    if (trigger.GetKind() != VALUE_OBJECT)
    {
        Error_Show_Action("Expected triggers to be an object", true, true);
        return;
    }

    RValue objParsed;
    StructCreate(&objParsed); // not a memory leak, we keep the yyobjectbase pointer

    Organik::GetLogger()->LogFormatted("ParseTriggers: Parsing trigger of type %s (flags=%d)", 
        trigger.GetKindName(), trigger.m_Flags);

    YYObjectBase* triggersObj = trigger.ToObject();
    
    //! ### type ###
    RValue typeRV = *(triggersObj->InternalGetYYVarRef(VAR_HASH(type)));
    if (!(typeRV.GetKind() == VALUE_STRING))
        Error_Show_Action("ParseTriggers: 'type' is missing or not a string.", true, true);
    
    int32_t typeHash = Code_Variable_FindAlloc_Slot_From_Name(nullptr, typeRV.ToCString());

    if (typeHash == VAR_HASH(event)) {
        Organik::GetLogger()->LogFormatted("ParseTriggers: Trigger is an event trigger");
    }
    else {
        Error_Show_Action("ParseTriggers: 'type' is not a valid trigger type.", true, true);
        return;
    }
    
    //! ### object ###
    RValue objectRV = *(triggersObj->InternalGetYYVarRef(VAR_HASH(object)));
    int32_t objectHash = -1;
    if (objectRV.GetKind() == VALUE_STRING)
        objectHash = parseRValueNumber<int32_t>(DoBuiltin(&gml_asset_get_index, { objectRV }));
    else
        objectHash = parseRValueNumber<int32_t>(objectRV);
    if (objectHash <= 0)
        Error_Show_Action("ParseTriggers: 'object' is missing or not a string (or number).", true, true);
    
    //! ### when ### !//
    RValue whenRV = *(triggersObj->InternalGetYYVarRef(VAR_HASH(when)));
    if (!(whenRV.GetKind() == VALUE_STRING))
        Error_Show_Action("ParseTriggers: 'when' is missing or not a string.", true, true);
    bool after = (Code_Variable_FindAlloc_Slot_From_Name(nullptr, whenRV.ToCString())
                    == VAR_HASH(after)) ? true : false;

    //! ### event ### !//
    RValue eventRV = *(triggersObj->InternalGetYYVarRef(VAR_HASH(event)));
    if (!(eventRV.GetKind() == VALUE_STRING))
        Error_Show_Action("ParseTriggers: 'event' is missing or not a string.", true, true);
    int32_t eventHash = Code_Variable_FindAlloc_Slot_From_Name(nullptr, eventRV.ToCString());

    const std::pair<const int32_t, const int32_t> eventCodePair = GetEventHashes()->contains(eventHash) ? 
        (*GetEventHashes())[eventHash] : std::make_pair(-1,-1);

    if (eventCodePair.first == -1)
    {
        Error_Show_Action("ParseTriggers: 'event' is not a valid event name.", true, true);
        return;
    }

    RValue actionsRV = *(triggersObj->InternalGetYYVarRef(VAR_HASH(action)));
    if (actionsRV.GetKind() == VALUE_STRING)
    {
        Organik::GetLogger()->LogFormatted("ParseTriggers: Parsing single action for object %d, event %d/%d, when %s", 
            objectHash, eventCodePair.first, eventCodePair.second, after ? "after" : "before");
        
        RValue variantObject;
        {
            std::ifstream inFile((entry.path() / actionsRV.ToCString()).generic_string());
            std::ostringstream strStream;
            std::string line;
            while (std::getline(inFile, line))
            {
                strStream << line << "\n";
            }
            RValue fileContentsRV;
            RValue actionObject;
            YYCreateString(&fileContentsRV, strStream.str().c_str());
            actionObject = DoBuiltin(&gml_json_parse, { fileContentsRV });
            if (actionObject.m_Kind != VALUE_OBJECT && actionObject.m_Kind != VALUE_ARRAY)
            {
                Error_Show_Action("Failed to parse action JSON file.", true, true);
                //! program terminates
            }
            // ParseActions(actionObject.ToObject(), objectHash, eventCodePair.first, eventCodePair.second, after);
        }

    }
    
}
