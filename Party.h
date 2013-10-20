#pragma once
#include "Player.h"
#include "NPC.h"
#include "mm7_data.h"
#include <array>



#define PARTY_AUTONOTES_BIT__EMERALD_FIRE_FOUNTAIN 2

enum PARTY_QUEST_BITS: uint16
{
  PARTY_QUEST_EMERALD_RED_POTION_ACTIVE = 1,
  PARTY_QUEST_EMERALD_SEASHELL_ACTIVE = 2,
  PARTY_QUEST_EMERALD_LONGBOW_ACTIVE = 3,
  PARTY_QUEST_EMERALD_PLATE_ACTIVE = 4,
  PARTY_QUEST_EMERALD_LUTE_ACTIVE = 5,
  PARTY_QUEST_EMERALD_HAT_ACTIVE = 6,

  PARTY_QUEST_EMERALD_MARGARETH_OFF = 17,

  PARTY_QUEST_EVENMORN_MAP_FOUND = 64,
  PARTY_QUEST_FINISHED_EMERALD_ISLE = 136,

  PARTY_QUEST_OBELISK_HARMONDALE = 164,
  PARTY_QUEST_OBELISK_ERATHIA = 165,
  PARTY_QUEST_OBELISK_TULAREAN_FOREST = 166,
  PARTY_QUEST_OBELISK_DEYJA = 167,
  PARTY_QUEST_OBELISK_BRACADA_DESERT = 168,
  PARTY_QUEST_OBELISK_CELESTE = 169,
  PARTY_QUEST_OBELISK_THE_PIT = 170,
  PARTY_QUEST_OBELISK_EVENMORN_ISLAND = 171,
  PARTY_QUEST_OBELISK_MOUNT_NIGHON = 172,
  PARTY_QUEST_OBELISK_BARROW_DOWNS = 173,
  PARTY_QUEST_OBELISK_LAND_OF_THE_GIANTS = 174,
  PARTY_QUEST_OBELISK_TATALIA = 175,
  PARTY_QUEST_OBELISK_AVLEE = 176,
  PARTY_QUEST_OBELISK_STONE_CITY = 177,

  PARTY_QUEST_OBELISK_TREASURE_FOUND = 178,

  PARTY_QUEST_FOUNTAIN_HARMONDALE = 206,
  PARTY_QUEST_FOUNTAIN_NIGHON = 207,
  PARTY_QUEST_FOUNTAIN_PIERPONT = 208,
  PARTY_QUEST_FOUNTAIN_CELESTIA = 209,
  PARTY_QUEST_FOUNTAIN_THE_PIT = 210,
  PARTY_QUEST_FOUNTAIN_EVENMORN_ISLE = 211
};

/*  355 */
enum PARTY_FLAGS_1: __int32
{
  PARTY_FLAGS_1_WATER_DAMAGE = 0x0004,
  PARTY_FLAGS_1_FALLING = 0x0008,
  PARTY_FLAGS_1_ALERT_RED = 0x0010,
  PARTY_FLAGS_1_ALERT_YELLOW = 0x0020,
  PARTY_FLAGS_1_STANDING_ON_WATER = 0x0080,
  PARTY_FLAGS_1_LANDING = 0x0100,
  PARTY_FLAGS_1_BURNING = 0x200
};
enum PARTY_FLAGS_2: __int32
{
  PARTY_FLAGS_2_RUNNING = 0x2,
};

/*  347 */
enum PARTY_BUFF_INDEX
{
  PARTY_BUFF_RESIST_AIR = 0,
  PARTY_BUFF_RESIST_BODY = 1,
  PARTY_BUFF_DAY_OF_GODS = 2,
  PARTY_BUFF_DETECT_LIFE = 3,
  PARTY_BUFF_RESIST_EARTH = 4,
  PARTY_BUFF_FEATHER_FALL = 5,
  PARTY_BUFF_RESIST_FIRE = 6,
  PARTY_BUFF_FLY = 7,
  PARTY_BUFF_HASTE = 8,
  PARTY_BUFF_HEROISM = 9,
  PARTY_BUFF_IMMOLATION = 10,
  PARTY_BUFF_INVISIBILITY = 11,
  PARTY_BUFF_RESIST_MIND = 12,
  PARTY_BUFF_PROTECTION_FROM_MAGIC = 13,
  PARTY_BUFF_SHIELD = 14,
  PARTY_BUFF_STONE_SKIN = 15,
  PARTY_BUFF_TORCHLIGHT = 16,
  PARTY_BUFF_RESIST_WATER = 17,
  PARTY_BUFF_WATER_WALK = 18,
  PARTY_BUFF_WIZARD_EYE = 19,
};



/*  300 */
enum PartyAction: unsigned __int32
{
  PARTY_INVALID = 0,
  PARTY_TurnLeft = 0x1,
  PARTY_TurnRight = 0x2,
  PARTY_StrafeLeft = 0x3,
  PARTY_StrafeRight = 0x4,
  PARTY_WalkForward = 0x5,
  PARTY_WalkBackward = 0x6,
  PARTY_LookUp = 0x7,
  PARTY_LookDown = 0x8,
  PARTY_CenterView = 0x9,
  PARTY_unkA = 0xA,
  PARTY_unkB = 0xB,
  PARTY_Jump = 0xC,
  PARTY_FlyUp = 0xD,
  PARTY_FlyDown = 0xE,
  PARTY_Land = 0xF,
  PARTY_RunForward = 0x10,
  PARTY_RunBackward = 0x11,
  PARTY_FastTurnLeft = 0x12,
  PARTY_FastTurnRight = 0x13,

  PARTY_dword = 0xFFFFFFFF
};



/*  135 */
#pragma pack(push, 1)
struct ActionQueue
{
  void Add(PartyAction action);
  void Reset();
  PartyAction Next();

  unsigned int uNumActions;
  PartyAction  pActions[30];
};
#pragma pack(pop)





enum PartyAlignment: unsigned __int32
{
  PartyAlignment_Good = 0,
  PartyAlignment_Neutral = 1,
  PartyAlignment_Evil = 2
};





/*  208 */
#pragma pack(push, 1)
struct PartyTimeStruct
{
  std::array<__int64, 10> bountyHunting_next_generation_time;
  std::array<__int64, 85> Shops_next_generation_time;//field_50
  std::array<__int64, 53> _shop_ban_times;      
  std::array<unsigned __int64, 10> CounterEventValues;  // (0xACD314h in Silvo's binary)
  std::array<__int64, 29> HistoryEventTimes;   // (0xACD364h in Silvo's binary)
  std::array<unsigned __int64, 20> _s_times; //5d8 440h+8*51     //(0xACD44Ch in Silvo's binary)
};
#pragma pack(pop)

/*  207 */
#pragma pack(push, 1)
struct Party
{
  Party():
    uTimePlayed(0)
  {
    Zero();
  }

  void Zero();
  void UpdatePlayersAndHirelingsEmotions();
  void RestAndHeal();
  unsigned int GetPartyFame();
  void CreateDefaultParty(char bGiveItems);
  int Reset();
  void ResetPosMiscAndSpellBuffs();
  bool HasItem(unsigned int uItemID);
  void SetHoldingItem(ItemGen *pItem);
  int GetNextActiveCharacter();
  bool _497FC5_check_party_perception_against_level();
  bool AddItemToParty(ItemGen *pItem);
  void Yell();
  void CountHirelings();

  void GivePartyExp(unsigned int pEXPNum);
  int GetPartyReputation();


  void PartyFindsGold(unsigned int uNumGold, int _1_dont_share_with_followers___2_the_same_but_without_a_message__else_normal);
  void sub_421B2C_PlaceInInventory_or_DropPickedItem();

  static void SetGold(unsigned int uNumGold);
  static void TakeGold(unsigned int uNumGold);
  static void SetFood(unsigned int uNumFood);
  static void TakeFood(unsigned int uNumFood);
  static void GiveFood(unsigned int _this);

  inline bool WizardEyeActive()      {return pPartyBuffs[PARTY_BUFF_WIZARD_EYE].uExpireTime > 0;}
  inline int  WizardEyeSkillLevel()  {return pPartyBuffs[PARTY_BUFF_WIZARD_EYE].uSkill;}
  inline bool TorchlightActive()     {return pPartyBuffs[PARTY_BUFF_TORCHLIGHT].uExpireTime > 0;}
  inline bool FlyActive()            {return pPartyBuffs[PARTY_BUFF_FLY].uExpireTime > 0;}
  inline bool WaterWalkActive()      {return pPartyBuffs[PARTY_BUFF_WATER_WALK].uExpireTime > 0;}
  inline bool ImmolationActive()     {return pPartyBuffs[PARTY_BUFF_IMMOLATION].uExpireTime > 0;}
  inline int ImmolationSkillLevel() {return pPartyBuffs[PARTY_BUFF_IMMOLATION].uSkill;}
  inline bool FeatherFallActive()    {return pPartyBuffs[PARTY_BUFF_FEATHER_FALL].uExpireTime > 0;}
  inline bool Invisible()            {return pPartyBuffs[PARTY_BUFF_INVISIBILITY].uExpireTime > 0;}

  inline bool GetRedAlert()    {return (uFlags & PARTY_FLAGS_1_ALERT_RED) != 0;}
  inline void SetRedAlert()    {uFlags |= PARTY_FLAGS_1_ALERT_RED;}
  inline bool GetYellowAlert() {return (uFlags & PARTY_FLAGS_1_ALERT_YELLOW) != 0;}
  inline void SetYellowAlert() {uFlags |= PARTY_FLAGS_1_ALERT_YELLOW;}

  bool IsPartyEvil();
  bool IsPartyGood();

  int field_0;
  unsigned int uPartyHeight;
  unsigned int uDefaultPartyHeight;
  int sEyelevel;
  unsigned int uDefaultEyelevel;
  int field_14_radius;
  int y_rotation_granularity;
  unsigned int uWalkSpeed;
  int y_rotation_speed;  // deg/s
  int field_24;
  int field_28;
  unsigned __int64 uTimePlayed;
  __int64 uLastRegenerationTime;
  PartyTimeStruct PartyTimes;
  Vec3_int_ vPosition;
  int sRotationY;
  int sRotationX;
  Vec3_int_ vPrevPosition;
  int sPrevRotationY;
  int sPrevRotationX;
  int sPrevEyelevel;
  int field_6E0;
  int field_6E4;
  int uFallSpeed;
  int field_6EC;
  int field_6F0;
  int floor_face_pid; // face we are standing at
  int walk_sound_timer;
  int field_6FC;
  int uFallStartY;
  unsigned int bFlying;
  char field_708;
  unsigned __int8 hirelingScrollPosition;
  char field_70A;
  char field_70B;
  unsigned int uCurrentYear;
  unsigned int uCurrentMonth;
  unsigned int uCurrentMonthWeek;
  unsigned int uDaysPlayed;
  unsigned int uCurrentHour;
  unsigned int uCurrentMinute;
  unsigned int uCurrentTimeSecond;
  unsigned int uNumFoodRations;
  int field_72C;
  int field_730;
  unsigned int uNumGold;
  unsigned int uNumGoldInBank;
  unsigned int uNumDeaths;
  int field_740;
  int uNumPrisonTerms;
  unsigned int uNumBountiesCollected;
  int field_74C;
  std::array<__int16, 5> monster_id_for_hunting;
  std::array<__int16, 5> monster_for_hunting_killed;
  unsigned char   days_played_without_rest;
  unsigned __int8 _quest_bits[64];
  std::array<unsigned __int8, 16> pArcomageWins;
  char field_7B5_in_arena_quest;
  char uNumArenaPageWins;
  char uNumArenaSquireWins;
  char uNumArenaKnightWins;
  char uNumArenaLordWins;
  std::array<char, 29> pIsArtifactFound;  //7ba
  std::array<char, 39> field_7d7;
  unsigned char _autonote_bits[26];
  std::array<char, 60> field_818;
  std::array<char, 32> field_854;
  int uNumArcomageWins;
  int uNumArcomageLoses;
  unsigned int bTurnBasedModeOn;
  int field_880;
  int uFlags2;
  PartyAlignment alignment;
  std::array<SpellBuff, 20> pPartyBuffs;
  std::array<Player, 4> pPlayers;
  std::array<NPCData, 2> pHirelings;
  ItemGen pPickedItem;
  unsigned int uFlags;
  std::array<std::array<ItemGen, 12>, 53> StandartItemsInShops;
  std::array<std::array<ItemGen, 12>, 53> SpecialItemsInShops;   //D0EC
  std::array<std::array<ItemGen, 12>, 32> SpellBooksInGuilds;
  std::array<char, 24> field_1605C;
  char pHireling1Name[100];
  char pHireling2Name[100];
  int armageddon_timer;
  int field_16140;
  std::array<int, 4> pTurnBasedPlayerRecoveryTimes;
  std::array<int, 53> InTheShopFlags;
  int uFine;
  float flt_TorchlightColorR;
  float flt_TorchlightColorG;
  float flt_TorchlightColorB;
};
#pragma pack(pop)



extern Party *pParty; // idb


extern struct ActionQueue *pPartyActionQueue;

inline bool TestPartyQuestBit(PARTY_QUEST_BITS bit)
{
  return _449B57_test_bit(pParty->_quest_bits, bit);
}
