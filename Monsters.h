#pragma once



/*  334 */
enum MONSTER_TYPE
{
  MONSTER_DEVIL_1 = 0x16,
  MONSTER_DEVIL_2 = 0x17,
  MONSTER_DEVIL_3 = 0x18,
  MONSTER_DRAGON_1 = 0x19,
  MONSTER_DRAGON_2 = 0x1A,
  MONSTER_DRAGON_3 = 0x1B,
  MONSTER_ELEMENTAL_WATER_1 = 0x2E,
  MONSTER_ELEMENTAL_WATER_3 = 0x30,
  MONSTER_ELF_ARCHER_1 = 0x31,
  MONSTER_ELF_ARCHER_3 = 0x33,
  MONSTER_ELF_SPEARMAN_1 = 0x34,
  MONSTER_ELF_SPEARMAN_3 = 0x36,
  MONSTER_GHOST_1 = 0x46,
  MONSTER_GHOST_3 = 0x48,
  MONSTER_HARPY_1 = 0x55,
  MONSTER_HARPY_2 = 0x56,
  MONSTER_HARPY_3 = 0x57,
  MONSTER_LICH_1 = 0x5B,
  MONSTER_LICH_3 = 0x5D,
  MONSTER_OOZE_1 = 0x70,
  MONSTER_OOZE_2 = 0x71,
  MONSTER_OOZE_3 = 0x72,
  MONSTER_PEASANT_ELF_FEMALE_1_1 = 0x85,
  MONSTER_PEASANT_ELF_MALE_3_3 = 0x96,
  MONSTER_SKELETON_1 = 0xC7,
  MONSTER_SKELETON_3 = 0xC9,
  MONSTER_TITAN_1 = 0xD3,
  MONSTER_TITAN_3 = 0xD5,
  MONSTER_VAMPIRE_1 = 0xD9,
  MONSTER_VAMPIRE_3 = 0xDB,
  MONSTER_WIGHT_1 = 0xDF,
  MONSTER_WIGHT_3 = 0xE1,
  MONSTER_ZOMBIE_1 = 0xE5,
  MONSTER_ZOMBIE_3 = 0xE7,
  MONSTER_PEASANT_GOBLIN_MALE_3_3 = 0xF9,
  MONSTER_TROLL_1 = 0xFA,
  MONSTER_TROLL_2 = 0xFB,
  MONSTER_TROLL_3 = 0xFC,
  MONSTER_TREANT_1 = 0xFD,
  MONSTER_TREANT_3 = 0xFF,
  MONSTER_GHOUL_1 = 0x100,
  MONSTER_GHOUL_3 = 0x102,
};

/*  335 */
enum MONSTER_SPECIAL_ABILITY_TYPE
{
  MONSTER_SPECIAL_ABILITY_NONE = 0x0,
  MONSTER_SPECIAL_ABILITY_SHOT = 0x1,
  MONSTER_SPECIAL_ABILITY_SUMMON = 0x2,
  MONSTER_SPECIAL_ABILITY_EXPLODE = 0x3,
};

enum MONSTER_MOVEMENT_TYPE
    {
    MONSTER_MOVEMENT_TYPE_SHORT   = 0x0,
    MONSTER_MOVEMENT_TYPE_MEDIUM  = 0x1,
    MONSTER_MOVEMENT_TYPE_LONG    = 0x2,
    MONSTER_MOVEMENT_TYPE_GLOBAL  = 0x3,
    MONSTER_MOVEMENT_TYPE_FREE    = 0x4,
    MONSTER_MOVEMENT_TYPE_STAIONARY = 0x5,
    };


/*  336 */
enum MONSTER_SUPERTYPE
{
  MONSTER_SUPERTYPE_UNDEAD = 0x1,
  MONSTER_SUPERTYPE_KREEGAN = 0x2,
  MONSTER_SUPERTYPE_DRAGON = 0x3,
  MONSTER_SUPERTYPE_ELF = 0x4,
  MONSTER_SUPERTYPE_WATER_ELEMENTAL = 0x5,
  MONSTER_SUPERTYPE_TREANT = 0x6,
  MONSTER_SUPERTYPE_TITAN = 0x7,
  MONSTER_SUPERTYPE_8 = 0x8,
};

enum SPECIAL_ATTACK_TYPE : unsigned __int8
{
  SPECIAL_ATTACK_NONE = 0,
  SPECIAL_ATTACK_CURSE = 1,
  SPECIAL_ATTACK_WEAK = 2,
  SPECIAL_ATTACK_SLEEP = 3,
  SPECIAL_ATTACK_DRUNK = 4,
  SPECIAL_ATTACK_INSANE = 5,
  SPECIAL_ATTACK_POISON1 = 6,
  SPECIAL_ATTACK_POISON2 = 7,
  SPECIAL_ATTACK_POISON3 = 8,
  SPECIAL_ATTACK_DISEASE1 = 9,
  SPECIAL_ATTACK_DISEASE2 = 10,
  SPECIAL_ATTACK_DISEASE3 = 11,
  SPECIAL_ATTACK_PARALYZED = 12,
  SPECIAL_ATTACK_UNCONSCIOUS = 13,
  SPECIAL_ATTACK_DEAD = 14,
  SPECIAL_ATTACK_PETRIFIED = 15,
  SPECIAL_ATTACK_ERADICATED = 16,
  SPECIAL_ATTACK_BREAK_ANY = 17,
  SPECIAL_ATTACK_BREAK_ARMOR = 18,
  SPECIAL_ATTACK_BREAK_WEAPON = 19,
  SPECIAL_ATTACK_STEAL = 20,
  SPECIAL_ATTACK_AGING = 21,
  SPECIAL_ATTACK_MANA_DRAIN = 22,
  SPECIAL_ATTACK_FEAR = 23,
};


/*  187 */
#pragma pack(push, 1)
struct MonsterInfo
{
  enum HostilityRadius: unsigned __int8
  {
    Hostility_Friendly = 0,
    Hostility_Close = 1,
    Hostility_Short = 2,
    Hostility_Medium = 3,
    Hostility_Long = 4
  };

  inline MonsterInfo():
    pName(nullptr), pPictureName(nullptr)
  {}

  char *pName;
  char *pPictureName;
  unsigned __int8 uLevel;
  unsigned __int8 uTreasureDropChance;
  unsigned __int8 uTreasureDiceRolls;
  unsigned __int8 uTreasureDiceSides;
  unsigned __int8 uTreasureLevel;
  unsigned __int8 uTreasureType;
  unsigned __int8 uFlying;
  unsigned __int8 uMovementType;
  unsigned __int8 uAIType;
  HostilityRadius uHostilityType;
  char field_12;
  SPECIAL_ATTACK_TYPE uSpecialAttackType;
  unsigned __int8 uSpecialAttackLevel;
  unsigned __int8 uAttack1Type;
  unsigned __int8 uAttack1DamageDiceRolls;
  unsigned __int8 uAttack1DamageDiceSides;
  unsigned __int8 uAttack1DamageBonus;
  unsigned __int8 uMissleAttack1Type;
  unsigned __int8 uAttack2Chance;
  unsigned __int8 uAttack2Type;
  unsigned __int8 uAttack2DamageDiceRolls;
  unsigned __int8 uAttack2DamageDiceSides;
  unsigned __int8 uAttack2DamageBonus;
  unsigned __int8 uMissleAttack2Type;
  unsigned __int8 uSpell1UseChance;
  unsigned __int8 uSpell1ID;
  unsigned __int8 uSpell2UseChance;
  unsigned __int8 uSpell2ID;
  unsigned __int8 uResFire;
  unsigned __int8 uResAir;
  unsigned __int8 uResWater;
  unsigned __int8 uResEarth;
  unsigned __int8 uResMind;
  unsigned __int8 uResSpirit;
  unsigned __int8 uResBody;
  unsigned __int8 uResLight;
  unsigned __int8 uResDark;
  unsigned __int8 uResPhysical;
  unsigned __int8 uSpecialAbilityType;           // 0   SPECIAL_ABILITY_TYPE_NONE
                                                 // 1   SPECIAL_ABILITY_TYPE_SHOT
                                                 // 2   SPECIAL_ABILITY_TYPE_SUMMON
                                                 // 3   SPECIAL_ABILITY_TYPE_EXPLODE
  unsigned __int8 uSpecialAbilityDamageDiceRolls;
  unsigned __int8 uSpecialAbilityDamageDiceSides;
  unsigned __int8 uSpecialAbilityDamageDiceBonus;
  unsigned __int8 uNumCharactersAttackedPerSpecialAbility;
  char field_33;
  unsigned __int16 uID;
  unsigned __int16 bQuestMonster;
  unsigned __int16 uSpellSkillAndMastery1;
  unsigned __int16 uSpellSkillAndMastery2;
  __int16 field_3C_some_special_attack;
  __int16 field_3E;
  unsigned int uHP;
  unsigned int uAC;
  unsigned int uExp;
  unsigned int uBaseSpeed;
  signed int uRecoveryTime;
  unsigned int uAttackPreference;
};
#pragma pack(pop)


 
/*  189 */
#pragma pack(push, 1)
struct MonsterStats
{
  void Initialize();
  void InitializePlacements();
  signed int FindMonsterByTextureName(const char *Str2);

  static bool BelongsToSupertype(unsigned int uMonsterInfoID, enum MONSTER_SUPERTYPE eSupertype);

  MonsterInfo pInfos[265];  //0 - 5b18h
  char *pPlaceStrings[31]; //5B18h placement counts from 1
  unsigned int uNumMonsters;  //5B94h
  unsigned int uNumPlacements; //5B98h
  int field_5B9C;
};
#pragma pack(pop)



#pragma pack(push, 1)
struct MonsterDesc_mm6
{
  unsigned __int16 uMonsterHeight;
  unsigned __int16 uMonsterRadius;
  unsigned __int16 uMovementSpeed;
  __int16 uToHitRadius;
  unsigned __int16 pSoundSampleIDs[4];
  char pMonsterName[32];
  char pSpriteNames[10][10];
};

struct MonsterDesc
{
  unsigned __int16 uMonsterHeight;
  unsigned __int16 uMonsterRadius;
  unsigned __int16 uMovementSpeed;
  __int16 uToHitRadius;
  unsigned int uTintColor;
  unsigned __int16 pSoundSampleIDs[4];
  char pMonsterName[32];
  char pSpriteNames[10][10];
};
#pragma pack(pop)


#pragma pack(push, 1)
struct MonsterList
{
  inline MonsterList():  //----- (00458429)
    uNumMonsters(0), pMonsters(nullptr)
  {}
    signed __int16 GetMonsterIDByName(const char *pMonsterName);
  void ToFile();
  void FromFile(void *data_mm6, void *data_mm7, void *data_mm8);
  bool FromFileTxt(const char *Args);

  signed int uNumMonsters;
  struct MonsterDesc *pMonsters;
};
#pragma pack(pop)


extern struct MonsterStats *pMonsterStats;
extern struct MonsterList *pMonsterList;