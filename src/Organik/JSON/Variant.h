#pragma once
#include <string>
#include <unordered_map>
#include <vector>

void ParseEventTriggers(const RValue& trigger, const std::filesystem::directory_entry& entry);
struct Variant
{
    std::string name;
    std::string nameshort;
    std::string desc;
    std::string manufacturer;
    double color;
    double rarity = 1.0;
    double spriteID = -1.0;
    double soundID = -1.0;
    double spawnID = -1.0;
    std::unordered_map<int32_t, double> statMultipliers;
    std::unordered_map<int32_t, double> statIncreases;
    int32_t variantHash = 0;

    static int32_t LoadVariant(const RValue& variantObject, std::unordered_map<int32_t, Variant>*& variantList);
    
    enum : int32_t
    {
        ULTRA = 0,
        STANDARD = 1,
        STABILIZED = 2,
        PRINTING = 3,
        SCAV = 4,
        HEAVY = 5,
        TAIGA = 6,
        MILSPEC = 7,
        CHAOS = 8,
        ARTIFACT = 9,
        OVERCLOCKED = 10,
        LIGHTWEIGHT = 11,
        PROTOTYPE = 12,
        ALPHA = 13,
        SIEGE = 14,
        CURSED = 15,
        MODULAR = 16,
        BOUNCY = 17,
        TIMED = 18,
        DIVINE = 19,
        TIMELESS = 20,
        REACTIVE = 21,
        KAIDA = 22,
        CHARGED = 23,


        VARIANTS_END
    };


    static void Apply(CInstance* instance, int variant, int sClass, int subclass, int subclassLevel);  
    static const int32_t VariantIcon[VARIANTS_END];
};
std::vector<int32_t>* GetVariantSprites();
std::unordered_map<int32_t, Variant>* getUserVariants();

void LoadVariants();