#include "Synthetik.h"
#include <unordered_map>
#include "InstanceHelper.h"
#include "VariableHelper.h"
#include "room_list.h"
#include "Globals.h"
#include "Variant.h"

#define MAKE_HASHES(var) Code_Variable_FindAlloc_Slot_From_Name(GetGlobalInstance(), const_cast<char*>(#var)),
#define MAKE_ASSETINDEXES(object) DoBuiltin(&gml_asset_get_index, {RValue(#object)}).ToInt32(),
#define MAKE_VARMAP(var) {Code_Variable_FindAlloc_Slot_From_Name(GetGlobalInstance(), const_cast<char*>(#var)), #var},
std::vector<int32_t> Organik::Variables::Hashes;
std::vector<int32_t> Organik::Objects::ObjIndexes;
std::vector<int32_t> Organik::Sprites::SpriteIndexes;
std::vector<int32_t> Organik::Sounds::SoundIndexes;
std::vector<int32_t> Organik::Rooms::AssetIndexes;
std::unordered_map<int32_t, const char*> Organik::Variables::HashToVariableMap;
std::unordered_map<const char*, int32_t> Organik::Variables::varToIndexMap;

const char* const Organik::Objects::ObjectNamesArr[] = 
{
    OBJECTS_DO(MAKE_STRINGS)
};
const char* const Organik::Sprites::SpriteNamesArr[] = 
{
    SPRITES_DO(MAKE_STRINGS)
};
const char* const Organik::Sounds::SoundNamesArr[] = 
{
    SPRITES_DO(MAKE_STRINGS)
};
const char* const Organik::Variables::VariableNamesArr[] = 
{
    VARIABLES_DO(MAKE_STRINGS)
};
const char* const Organik::Rooms::RoomNamesArr[] = 
{
    ROOMS_DO(MAKE_STRINGS)
};
constexpr int variableCount = sizeof(Organik::Variables::VariableNamesArr) / sizeof(char* );
constexpr int objectCount = sizeof(Organik::Objects::ObjectNamesArr) / sizeof(char* );
constexpr int roomCount = sizeof(Organik::Rooms::RoomNamesArr) / sizeof(char* );
constexpr int spriteCount = sizeof(Organik::Sprites::SpriteNamesArr) / sizeof(char* );
constexpr int soundCount = sizeof(Organik::Sounds::SoundNamesArr) / sizeof(char* );
void Organik::DoHelperSetup()
{
    Organik::Variables::HashToVariableMap.reserve(variableCount);
    Organik::Variables::varToIndexMap.reserve(variableCount);
    Organik::Variables::Hashes.reserve(variableCount);
    Organik::GetLogger()->LogSimple("Setting up helper definitions...");
    for (int i=0;i<variableCount;i++)
    {
        int32_t hash = Code_Variable_FindAlloc_Slot_From_Name(
            GetGlobalInstance(), 
            const_cast<char*>(
                Organik::Variables::VariableNamesArr[i]
            ));

        Organik::Variables::Hashes[i] = hash;
        Organik::Variables::HashToVariableMap.insert_or_assign(hash, reinterpret_cast<const char*>(Organik::Variables::VariableNamesArr[i]));
    }
    Organik::GetLogger()->LogSimple("Setting up object indexes...");
    for (int i=0;i<objectCount;i++)
    {
        RValue r = DoBuiltin(&gml_asset_get_index, { RValue(Organik::Objects::ObjectNamesArr[i]) });
        
        int32_t iVal = r.ToInt32();
        Organik::Objects::ObjIndexes.push_back(
            iVal
        );
    }
    Organik::GetLogger()->LogSimple("Object indexes initialized.");
    Organik::GetLogger()->LogSimple("Setting up sound indexes...");
    for (int i=0;i<soundCount;i++)
    {
        RValue r = DoBuiltin(&gml_asset_get_index, { RValue(Organik::Sounds::SoundNamesArr[i]) });

        int32_t iVal = r.ToInt32();
        Organik::Sounds::SoundIndexes.push_back(
            iVal
        );
    }
    Organik::GetLogger()->LogSimple("Sound indexes initialized.");
    Organik::GetLogger()->LogSimple("Setting up room indexes...");

    for (int i=0;i<roomCount;i++)
    {
        Organik::Rooms::AssetIndexes.push_back(
            DoBuiltin(&gml_asset_get_index, {RValue(Organik::Rooms::RoomNamesArr[i])}).ToInt32()
        );
    }
    Organik::GetLogger()->LogSimple("Room indexes initialized.");
    Organik::GetLogger()->LogSimple("Helper definitions set up successfully.");
    #define OBJ_HASH(obj) Organik::Objects::ObjIndexes[Organik::Objects::##obj]
    *getPossibleBullets() = {
        OBJ_HASH(obj_bullet_sniperturret),
        OBJ_HASH(obj_bullet_enemysniper),
        OBJ_HASH(obj_bullet_enemy_stun),
        OBJ_HASH(obj_bullet_enemy_poison),
        OBJ_HASH(obj_bullet_enemypistol),
        OBJ_HASH(obj_bullet_enemyrifle),
        OBJ_HASH(obj_bullet_laser_worker),
        OBJ_HASH(obj_bullet_turret_grenade),
        OBJ_HASH(obj_bullet_turret_mortar),
        OBJ_HASH(obj_bullet_drone_mortar),
        OBJ_HASH(obj_bullet_drone_mortar_ballistic),
        OBJ_HASH(obj_bullet_turret_crystalmortar),
        OBJ_HASH(obj_bullet_buff_crystalmortar),
        OBJ_HASH(obj_bullet_rocket_boss2_A),
        OBJ_HASH(obj_bullet_rocket_boss2_B),
        OBJ_HASH(obj_bullet_rocket_heart),
        OBJ_HASH(obj_bullet_rocket_turret),
        OBJ_HASH(obj_bullet_rocket_turret_2),
        OBJ_HASH(obj_bullet_rocket_turret_3),
        OBJ_HASH(obj_bullet_rocket_turret_crazy),
        OBJ_HASH(obj_bullet_rocket_upvote),
        OBJ_HASH(obj_bullet_rocket_particle),
        OBJ_HASH(obj_bullet_rocket_particle2),
        OBJ_HASH(obj_bullet_rocket_psyBall),
        OBJ_HASH(obj_bullet_rocket_rapidfire),
        OBJ_HASH(obj_bullet_rocket_rapidfire_xm29),
        OBJ_HASH(obj_bullet_rocket_homing),
        OBJ_HASH(obj_bullet_rocket_homingslow),
        OBJ_HASH(obj_bullet_bloodbolt),
        OBJ_HASH(obj_bullet_bloodbolt_chaos),
        OBJ_HASH(obj_bullet_bloodbolt_damn),
        OBJ_HASH(obj_bullet_bloodbolt_boss),
        OBJ_HASH(obj_bullet_flyingknife),
        OBJ_HASH(obj_bullet_sawblade_huge),
        OBJ_HASH(obj_bullet_seeker_item),
        OBJ_HASH(obj_bullet_needler),
        OBJ_HASH(obj_bullet_rocket_followmouse),
        OBJ_HASH(obj_bullet_jetmg),
        OBJ_HASH(obj_bullet_turretvierling),
        OBJ_HASH(obj_bullet_bullethell),
        OBJ_HASH(obj_bullet_beam_1),
        OBJ_HASH(obj_bullet_144_sc),
        OBJ_HASH(obj_bullet_144_krkn),
        OBJ_HASH(obj_bullet_144_sabot),
        OBJ_HASH(obj_bullet_127_sc),
        OBJ_HASH(obj_bullet_127_toxic),
        OBJ_HASH(obj_bullet_127_sabot),
        OBJ_HASH(obj_bullet_127_explo),
        OBJ_HASH(obj_bullet_arrow_sc),
        OBJ_HASH(obj_bullet_arrow_stun),
        OBJ_HASH(obj_bullet_arrow_sleep),
        OBJ_HASH(obj_bullet_arrow_toxic),
        OBJ_HASH(obj_bullet_88_sc),
        OBJ_HASH(obj_bullet_88_ap),
        OBJ_HASH(obj_bullet_88_explo),
        OBJ_HASH(obj_bullet_88_hs),
        OBJ_HASH(obj_bullet_88_cata),
        OBJ_HASH(obj_bullet_88_toxic),
        OBJ_HASH(obj_bullet_556_sc),
        OBJ_HASH(obj_bullet_556_toxic),
        OBJ_HASH(obj_bullet_556_hp),
        OBJ_HASH(obj_bullet_556_sabot),
        OBJ_HASH(obj_bullet_556_sp),
        OBJ_HASH(obj_bullet_473_sc),
        OBJ_HASH(obj_bullet_473_alt),
        OBJ_HASH(obj_bullet_9mm_sc),
        OBJ_HASH(obj_bullet_9mm_ap),
        OBJ_HASH(obj_bullet_9mm_hs),
        OBJ_HASH(obj_bullet_9mm_hp),
        OBJ_HASH(obj_bullet_9mm_sp),
        OBJ_HASH(obj_bullet_9mm_sp_magnum),
        OBJ_HASH(obj_bullet_vierling),
        OBJ_HASH(obj_bullet_10g_buck),
        OBJ_HASH(obj_bullet_10g_bird),
        OBJ_HASH(obj_bullet_10g_flech),
        OBJ_HASH(obj_bullet_10g_slug),
        OBJ_HASH(obj_bullet_10g_fire),
        OBJ_HASH(obj_bullet_10g_masterkey),
        OBJ_HASH(obj_bullet_10g_masterkey2),
        OBJ_HASH(obj_bullet_10g_bolt),
        OBJ_HASH(obj_bullet_10g_helsing),
        OBJ_HASH(obj_bullet_nail_sc),
        OBJ_HASH(obj_bullet_nail_bleed),
        OBJ_HASH(obj_bullet_coil_x4),
        OBJ_HASH(obj_bullet_coil_x1),
        OBJ_HASH(obj_bullet_fireball),
        OBJ_HASH(obj_bullet_flamethrower),
        OBJ_HASH(obj_bullet_flak1),
        OBJ_HASH(obj_bullet_scatter),
        OBJ_HASH(obj_bullet_grenade_impact),
        OBJ_HASH(obj_bullet_grenade_bounce),
        OBJ_HASH(obj_bullet_grenade_rubber),
        OBJ_HASH(obj_bullet_grenade_stickyitem),
        OBJ_HASH(obj_bullet_particle_explo),
        OBJ_HASH(obj_bullet_annihilator),
        OBJ_HASH(obj_bullet_grenade_enm),
        OBJ_HASH(obj_bullet_mineemp),
        OBJ_HASH(obj_bullet_minelaser),
        OBJ_HASH(obj_bullet_minelaseritem),
        OBJ_HASH(obj_bullet_mineaciditem),
        OBJ_HASH(obj_bullet_minestunitem),
        OBJ_HASH(obj_bullet_breachingcharge),
        OBJ_HASH(obj_bullet_grenade_enm_aircom),
        OBJ_HASH(obj_bullet_grenade_enm_stun),
        OBJ_HASH(obj_bullet_sawblade),
        OBJ_HASH(obj_bullet_sawblade_ultra),
        OBJ_HASH(obj_bullet_sawblade_item),
        OBJ_HASH(obj_bullet_kunai),
        OBJ_HASH(obj_bullet_tomahawk),
        OBJ_HASH(obj_bullet_fanknives),
        OBJ_HASH(obj_bullet_fanknives_pickup),
        OBJ_HASH(obj_bullet_shuriken),
        OBJ_HASH(obj_bullet_shuriken2),
        OBJ_HASH(obj_bullet_ironorb),
        OBJ_HASH(obj_bullet_ironorb_enemy),
        OBJ_HASH(obj_bullet_taser),
        OBJ_HASH(obj_bullet_taser_enm),
        OBJ_HASH(obj_bullet_laser_line),
        OBJ_HASH(obj_bullet_laser_line_drone),
        OBJ_HASH(obj_bullet_laser_line_item),
        OBJ_HASH(obj_bullet_laser_line_drone_challenge),
        OBJ_HASH(obj_bullet_laser_line_gold),
        OBJ_HASH(obj_bullet_laser_line_cannon),
        OBJ_HASH(obj_bullet_laser_line_gold_pen),
        OBJ_HASH(obj_bullet_laser_shieldnuke),
        OBJ_HASH(obj_bullet_laser_line_reducedDMG),
        OBJ_HASH(obj_bullet_laser_line_fire),
        OBJ_HASH(obj_bullet_laser_line_pellet),
        OBJ_HASH(obj_bullet_laser_line_pellet_fire),
        OBJ_HASH(obj_bullet_laser_spray),
        OBJ_HASH(obj_bullet_laser_beam),
        OBJ_HASH(obj_bullet_laser_crystal),
        OBJ_HASH(obj_bullet_laser_slow),
        OBJ_HASH(obj_bullet_laser_ion),
        OBJ_HASH(obj_bullet_laser_fusion),
        OBJ_HASH(obj_bullet_laser_fusion_slow),
        OBJ_HASH(obj_bullet_laser_railgun),
        OBJ_HASH(obj_bullet_laser_railgun2),
        OBJ_HASH(obj_bullet_laser_plasma),
        OBJ_HASH(obj_bullet_chain),
        OBJ_HASH(obj_bullet_chain_orb),
        OBJ_HASH(obj_bullet_chain_turret),
        OBJ_HASH(obj_bullet_chain_inverse),
        OBJ_HASH(obj_bullet_chain_beam),
        OBJ_HASH(obj_bullet_laser_railgun_item),
        OBJ_HASH(obj_bullet_laser_beam_item),
    };
    

}
