#pragma once

#include <array>

#include "VectorTypes.h"

/*  360 */
enum SPELL_TYPE
{
  SPELL_FIRE_TORCH_LIGHT = 1,
  SPELL_FIRE_FIRE_BOLT = 2,
  SPELL_FIRE_PROTECTION_FROM_FIRE = 3,
  SPELL_FIRE_FIRE_AURA = 4,
  SPELL_FIRE_HASTE = 5,
  SPELL_FIRE_FIREBALL = 6,
  SPELL_FIRE_FIRE_SPIKE = 7,
  SPELL_FIRE_IMMOLATION = 8,
  SPELL_FIRE_METEOR_SHOWER = 9,
  SPELL_FIRE_INFERNO = 10,
  SPELL_FIRE_INCINERATE = 11,

  SPELL_AIR_WIZARD_EYE = 12,
  SPELL_AIR_FEATHER_FALL = 13,
  SPELL_AIR_PROTECTION_FROM_AIR = 14,
  SPELL_AIR_SPARKS = 15,
  SPELL_AIR_JUMP = 16,
  SPELL_AIR_SHIELD = 17,
  SPELL_AIR_LIGHNING_BOLT = 18,
  SPELL_AIR_INVISIBILITY = 19,
  SPELL_AIR_IMPLOSION = 20,
  SPELL_AIR_FLY = 21,
  SPELL_AIR_STARBURST = 22,

  SPELL_WATER_AWAKEN = 23,
  SPELL_WATER_POISON_SPRAY = 24,
  SPELL_WATER_PROTECTION_FROM_WATER = 25,
  SPELL_WATER_ICE_BOLT = 26,
  SPELL_WATER_WATER_WALK = 27,
  SPELL_WATER_RECHARGE_ITEM = 28,
  SPELL_WATER_ACID_BURST = 29,
  SPELL_WATER_ENCHANT_ITEM = 30,
  SPELL_WATER_TOWN_PORTAL = 31,
  SPELL_WATER_ICE_BLAST = 32,
  SPELL_WATER_LLOYDS_BEACON = 33,

  SPELL_EARTH_STUN = 34,
  SPELL_EARTH_SLOW = 35,
  SPELL_EARTH_PROTECTION_FROM_EARTH = 36,
  SPELL_EARTH_DEADLY_SWARM = 37,
  SPELL_EARTH_STONESKIN = 38,
  SPELL_EARTH_BLADES = 39,
  SPELL_EARTH_STONE_TO_FLESH = 40,
  SPELL_EARTH_ROCK_BLAST = 41,
  SPELL_EARTH_TELEKINESIS = 42,
  SPELL_EARTH_DEATH_BLOSSOM = 43,
  SPELL_EARTH_MASS_DISTORTION = 44,

  SPELL_SPIRIT_DETECT_LIFE = 45,
  SPELL_SPIRIT_BLESS = 46,
  SPELL_SPIRIT_FATE = 47,
  SPELL_SPIRIT_TURN_UNDEAD = 48,
  SPELL_SPIRIT_REMOVE_CURSE = 49,
  SPELL_SPIRIT_PRESERVATION = 50,
  SPELL_SPIRIT_HEROISM = 51,
  SPELL_SPIRIT_SPIRIT_LASH = 52,
  SPELL_SPIRIT_RAISE_DEAD = 53,
  SPELL_SPIRIT_SHARED_LIFE = 54,
  SPELL_SPIRIT_RESSURECTION = 55,

  SPELL_MIND_REMOVE_FEAR = 56,
  SPELL_MIND_MIND_BLAST = 57,
  SPELL_MIND_PROTECTION_FROM_MIND = 58,
  SPELL_MIND_TELEPATHY = 59,
  SPELL_MIND_CHARM = 60,
  SPELL_MIND_CURE_PARALYSIS = 61,
  SPELL_MIND_BERSERK = 62,
  SPELL_MIND_MASS_FEAR = 63,
  SPELL_MIND_CURE_INSANITY = 64,
  SPELL_MIND_PSYCHIC_SHOCK = 65,
  SPELL_MIND_ENSLAVE = 66,

  SPELL_BODY_CURE_WEAKNESS = 67,
  SPELL_BODY_FIRST_AID = 68,
  SPELL_BODY_PROTECTION_FROM_BODY = 69,
  SPELL_BODY_HARM = 70,
  SPELL_BODY_REGENERATION = 71,
  SPELL_BODY_CURE_POISON = 72,
  SPELL_BODY_HAMMERHANDS = 73,
  SPELL_BODY_CURE_DISEASE = 74,
  SPELL_BODY_PROTECTION_FROM_MAGIC = 75,
  SPELL_BODY_FLYING_FIST = 76,
  SPELL_BODY_POWER_CURE = 77,

  SPELL_LIGHT_LIGHT_BOLT = 78,
  SPELL_LIGHT_DESTROY_UNDEAD = 79,
  SPELL_LIGHT_DISPEL_MAGIC = 80,
  SPELL_LIGHT_PARALYZE = 81,
  SPELL_LIGHT_SUMMON_ELEMENTAL = 82,
  SPELL_LIGHT_DAY_OF_THE_GODS = 83,
  SPELL_LIGHT_PRISMATIC_LIGHT = 84,
  SPELL_LIGHT_DAY_OF_PROTECTION = 85,
  SPELL_LIGHT_HOUR_OF_POWER = 86,
  SPELL_LIGHT_SUNRAY = 87,
  SPELL_LIGHT_DIVINE_INTERVENTION = 88,

  SPELL_DARK_REANIMATE = 89,
  SPELL_DARK_TOXIC_CLOUD = 90,
  SPELL_DARK_VAMPIRIC_WEAPON = 91,
  SPELL_DARK_SHRINKING_RAY = 92,
  SPELL_DARK_SHARPMETAL = 93,
  SPELL_DARK_CONTROL_UNDEAD = 94,
  SPELL_DARK_PAIN_REFLECTION = 95,
  SPELL_DARK_SACRIFICE = 96,
  SPELL_DARK_DRAGON_BREATH = 97,
  SPELL_DARK_ARMAGEDDON = 98,
  SPELL_DARK_SOULDRINKER = 99,

  SPELL_BOW_ARROW = 100,
  SPELL_101 = 101,
  SPELL_LASER_PROJECTILE = 102,

  BECOME_MAGIC_GUILD_MEMBER = 150,
  SPELL_QUEST_COMPLETED = 151,
  SPELL_152 = 152,
  SPELL_DISEASE = 153
};


enum SPELL_SCHOOL : int
{
  SPELL_SCHOOL_FIRE =  0,
  SPELL_SCHOOL_AIR  =  1,
  SPELL_SCHOOL_WATER = 2,
  SPELL_SCHOOL_EARTH = 3,
  SPELL_SCHOOL_NONE =  4,
  SPELL_SCHOOL_MAGIC =  5,
  SPELL_SCHOOL_SPIRIT = 6,
  SPELL_SCHOOL_MIND   = 7,
  SPELL_SCHOOL_BODY   = 8,
  SPELL_SCHOOL_LIGHT   = 9,
  SPELL_SCHOOL_DARK =  10
};



/*   68 */
#pragma pack(push, 1)
struct SpellBuff
{
  inline SpellBuff()
  {
    uExpireTime = 0;
    uPower = 0;
    uSkill = 0;
    uOverlayID = 0;
    uCaster = 0;
    uFlags = 0;
  }

  bool Apply(signed __int64 uExpireTime, unsigned __int16 uSkillLevel, unsigned __int16 uPower, int uOverlayID, unsigned __int8 caster);
  void Reset();
  bool IsBuffExpiredToTime(__int64 time_end);
  bool NotExpired();

  signed __int64 uExpireTime;
  unsigned __int16 uPower;
  unsigned __int16 uSkill;
  unsigned __int16 uOverlayID;
  unsigned __int8 uCaster;
  unsigned __int8 uFlags;
};
#pragma pack(pop)


#pragma pack(push, 1)
struct SpellInfo
{
  char *pName;
  char *pShortName;
  char *pDescription;
  char *pBasicSkillDesc;
  char *pExpertSkillDesc;
  char *pMasterSkillDesc;
  char *pGrandmasterSkillDesc;
  SPELL_SCHOOL uSchool;
  int field_20;
};
#pragma pack(pop)



#pragma pack(push, 1)
struct SpellStats
{
  void Initialize();

  SpellInfo pInfos[100];
};
#pragma pack(pop)









/*  364 */
#pragma pack(push, 1)
struct stru324_spell
{
  __int16 uType;
  __int16 field_2;
};
#pragma pack(pop)

/*  151 */
#pragma pack(push, 1)
struct SpellData
{
  union
  {
    unsigned __int16 mana_per_skill[4];
    struct
    {
      unsigned __int16 uNormalLevelMana;
      unsigned __int16 uExpertLevelMana;
      unsigned __int16 uMasterLevelMana;
      unsigned __int16 uMagisterLevelMana;
    };
  };
  union
  {
    unsigned __int16 recovery_per_skill[4];
    struct
    {
      unsigned __int16 uNormalLevelRecovery;
      unsigned __int16 uExpertLevelRecovery;
      unsigned __int16 uMasterLevelRecovery;
      unsigned __int16 uMagisterLevelRecovery;
    };
  };
  __int16 field_10;
  __int16 field_12;
 // char field_12;
 // char field_13;
 // __int16 field_14;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct SpellBookIconPos 
{
	unsigned int Xpos;
	unsigned int Ypos;
};

#pragma pack(pop)

/*  154 */
#pragma pack(push, 1)
struct TownPortalData
{
  Vec3_int_ pos;
  __int16 rot_y;
  __int16 rot_x;
  unsigned __int16 uMapInfoID;
  __int16 field_12;
};
#pragma pack(pop)
extern std::array<TownPortalData, 6> TownPortalList; //4ECBB8


extern struct SpellStats *pSpellStats;

extern std::array<std::array<struct SpellBookIconPos, 12>, 9> pIconPos;

extern std::array<stru324_spell, 103> stru_4E3ACC;
extern std::array<SpellData, 100> pSpellDatas;
extern std::array<unsigned int, 25> wand_spell_ids;