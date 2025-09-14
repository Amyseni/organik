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