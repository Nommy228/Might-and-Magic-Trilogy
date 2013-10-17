#pragma once
#include "Monsters.h"
#include "Spells.h"
#include "Items.h"






/*  357 */
#pragma pack(push, 1)
struct stru319
{
  void LootActor(struct Actor *pActor);
  int which_player_would_attack(struct Actor *pActor);
  int  special_ability_use_check(struct Actor *pActor, int a2);
  int _427102(struct Actor *pActor, signed int a2);
  int PlayerHitOrMiss(struct Player *pPlayer, struct Actor *pActor, int a3, int a4);
  bool _4273BB(struct Actor *pActor, struct Actor *a2, int a3, int a4);
  bool ActorHitOrMiss(struct Actor *pActor, struct Player *pPlayer);
  int _427546(int a2);
  int CalcMagicalDamageToActor(struct Actor *pActor, int a2, signed int a3);
  bool GetMagicalResistance(struct Actor *pActor, unsigned int uType);
  int FindClosestActor(int a2, int a3, int a4);

  char field_0;
};
#pragma pack(pop)


extern stru319 stru_50C198; // idb






/*  361 */
enum ACTOR_BUFF_INDEX
{
  ACTOR_BUFF_CHARM = 1,
  ACTOR_BUFF_SUMMONED = 2,
  ACTOR_BUFF_SHRINK = 3,
  ACTOR_BUFF_AFRAID = 4,
  ACTOR_BUFF_STONED = 5,
  ACTOR_BUFF_PARALYZED = 6,

  ACTOR_BUFF_BERSERK = 8,

  ACTOR_BUFF_FATE = 10,
  ACTOR_BUFF_ENSLAVED = 11,
  ACTOR_BUFF_DAY_OF_PROTECTION = 12,
  ACTOR_BUFF_HOUR_OF_POWER = 13,
  ACTOR_BUFF_SHIELD = 14,
  ACTOR_BUFF_STONESKIN = 15,
  ACTOR_BUFF_BLESS = 16,
  ACTOR_BUFF_HEROISM = 17,
  ACTOR_BUFF_HASTE = 18,
  ACTOR_BUFF_PAIN_REFLECTION = 19,
  ACTOR_BUFF_PAIN_HAMMERHANDS = 20,
};





/*  295 */
enum ObjectType
{
  OBJECT_Any = 0x0,
  OBJECT_BLVDoor = 0x1,
  OBJECT_Item = 0x2,
  OBJECT_Actor = 0x3,
  OBJECT_Player = 0x4,
  OBJECT_Decoration = 0x5,
  OBJECT_BModel = 0x6,
};







/*  264 */
enum AIState : unsigned __int16
{
  Standing = 0x0,
  Tethered = 0x1,
  AttackingMelee = 0x2,
  AttackingRanged1 = 0x3,
  Dying = 0x4,
  Dead = 0x5,
  Pursuing = 0x6,
  Fleeing = 0x7,
  Stunned = 0x8,
  Fidgeting = 0x9,
  Interacting = 10,
  Removed = 11,
  AttackingRanged2 = 0xC,
  AttackingRanged3 = 0xD,
  Stoned = 0xE,
  Paralyzed = 0xF,
  Resurrected = 16,
  Summoned = 17,
  AttackingRanged4 = 18,
  Disabled = 19,
};

/*  265 */
enum ActorAnimation : __int32
{
  ANIM_Standing = 0x0,
  ANIM_Walking = 0x1,
  ANIM_AtkMelee = 0x2,
  ANIM_AtkRanged = 0x3,
  ANIM_GotHit = 0x4,
  ANIM_Dying = 0x5,
  ANIM_Dead = 0x6,
  ANIM_Bored = 0x7,
};





/*  247 */
#pragma pack(push, 1)
struct AIDirection
{
  Vec3_int_ vDirection;
  unsigned int uDistance;
  unsigned int uDistanceXZ;
  unsigned int uYawAngle;
  unsigned int uPitchAngle;
};
#pragma pack(pop)





/*   71 */
#pragma pack(push, 1)
struct ActorJob
{
  struct Vec3_short_ vPos;
  unsigned __int16 uAttributes;
  unsigned __int8 uAction;
  unsigned __int8 uHour;
  unsigned __int8 uDay;
  unsigned __int8 uMonth;
};
#pragma pack(pop)


/*   66 */
#pragma pack(push, 1)
struct Actor
{
  //----- (0041F4C1) --------------------------------------------------------
  inline Actor()
  {
    Actor *v1; // esi@1
    SpellBuff *v2; // eax@1
    signed int v3; // edx@1
    ItemGen *v4; // edi@3
    signed int v5; // ebx@3

  v1 = this;
  v2 = this->pActorBuffs;
  v3 = 22;
  do
  {
    v2->uSkill = 0;
    v2->uPower = 0;
    v2->uExpireTime = 0;
    v2->uCaster = 0;
    v2->uFlags = 0;
    ++v2;
    --v3;
  }
  while ( v3 );
  v4 = this->array_000234;
  v5 = 4;
  do
  {
    v4->Reset();
    ++v4;
    --v5;
  }
  while ( v5 );
  Reset();
}

  int _44FD29(int a2);
  void Reset();
  void Remove();
  void PrepareSprites(char load_sounds_if_bit1_set);
  void UpdateAnimation();
  signed int GetActorsRelation(Actor *a2);
  void SetRandomGoldIfTheresNoItem();
  bool CanAct();
  bool IsNotAlive();
  bool IsPeasant();

  
  static void _SelectTarget(unsigned int uActorID, int *a2, bool can_target_party);
  static void AI_Pursue3(unsigned int uActorID, unsigned int a2, signed int uActionLength, struct AIDirection *a4);
  static void AI_Pursue2(unsigned int uActorID, unsigned int a2, signed int uActionLength, struct AIDirection *pDir, int a5);
  static void AI_Flee(unsigned int uActorID, signed int edx0, int uActionLength, struct AIDirection *a4);
  static void AI_Pursue1(unsigned int uActorID, unsigned int a2, signed int arg0, signed int uActionLength, struct AIDirection *pDir);
  static void PlaySound(unsigned int uActorID, unsigned int uSoundID);
  static void Die(unsigned int uActorID);
  static void Resurrect(unsigned int uActorID);
  static void AI_Bored(unsigned int uActorID, unsigned int uObjID, struct AIDirection *a4);
  static void AI_Stun(unsigned int uActorID, signed int edx0, int arg0);
  static char __fastcall _4031C1_update_job(unsigned int uActorID, signed int a2, int a3);
  static void AI_RandomMove(unsigned int uActor_id, unsigned int uTarget_id, int radius, int uActionLength);
  static void AI_MissileAttack1(unsigned int uActorID, signed int sTargetPid, struct AIDirection *pDir);
  static void AI_MissileAttack2(unsigned int uActorID, signed int sTargetPid, struct AIDirection *pDir);
  static void AI_SpellAttack1(unsigned int uActorID, signed int sTargetPid, struct AIDirection *pDir);
  static void AI_SpellAttack2(unsigned int uActorID, signed int sTargetPid, struct AIDirection *pDir);
  static void AI_MeleeAttack(unsigned int uActorID, signed int sTargetPid, struct AIDirection *arg0);
  static void __fastcall StandAwhile(unsigned int uActorID);
  static void AI_Stand(unsigned int uActorID, unsigned int object_to_face_pid, unsigned int uActionLength, struct AIDirection *a4);
  static void AI_StandOrBored(unsigned int uActorID, signed int uObjID, int uActionLength, struct AIDirection *a4);
  static void AI_FaceObject(unsigned int uActorID, unsigned int uObjID, int _48, struct AIDirection *a4);
  static struct AIDirection * GetDirectionInfo(unsigned int uObj1ID, unsigned int uObj2ID, struct AIDirection *pOut, int a4);
  static void Explode(unsigned int uActorID);
  static void AI_RangedAttack(unsigned int uActorID, struct AIDirection *a2, int type, char a4);
  static void AI_SpellAttack(unsigned int uActorID, struct AIDirection *pDir, int uSpellID, int a4, unsigned int uSkillLevel);

  static unsigned short GetObjDescId( int spellId );

  static void AggroSurroundingPeasants(unsigned int uActorID, int a2);
  static bool ArePeasantsOfSameFaction(Actor *a1, Actor *a2);
  static bool StealFrom(unsigned int uActorID);
  static void GiveItem(signed int uActorID, unsigned int uItemID, unsigned int bGive);
  static void ToggleFlag(signed int uActorID, unsigned int uFlag, int bToggle);
  static void ApplyFineForKillingPeasant(unsigned int uActorID);
  static void DrawHealthBar(Actor *actor, struct GUIWindow *window);
  static int _43B3E0_CalcDamage(Actor *a1, signed int a2);
  static void AddBloodsplatOnDamageOverlay(unsigned int uActorID, int a2, signed int a3);


  char pActorName[32];
  signed __int16 sNPC_ID;
  __int16 field_22;
  unsigned int uAttributes;
  __int16 sCurrentHP;
  char field_2A[2];
  struct MonsterInfo pMonsterInfo;
  __int16 word_000084_range_attack;
  __int16 word_000086_some_monster_id;
  unsigned __int16 uActorRadius;
  unsigned __int16 uActorHeight;
  unsigned __int16 uMovementSpeed;
  struct Vec3_short_ vPosition;
  struct Vec3_short_ vVelocity;
  unsigned __int16 uYawAngle;
  unsigned __int16 uPitchAngle;
  __int16 uSectorID;
  unsigned __int16 uCurrentActionLength;
  struct Vec3_short_ vInitialPosition;
  struct Vec3_short_ vGuardingPosition;
  unsigned __int16 uTetherDistance;
  AIState uAIState;
  unsigned __int16 uCurrentActionAnimation;
  unsigned __int16 uCarriedItemID;
  char field_B6;
  char field_B7;
  unsigned int uCurrentActionTime;
  unsigned __int16 pSpriteIDs[8];
  unsigned __int16 pSoundSampleIDs[4]; // 1 die     3 bored
  struct SpellBuff pActorBuffs[22];
  struct ItemGen array_000234[4];
  unsigned int uGroup;
  unsigned int uAlly;
  struct ActorJob pScheduledJobs[8];
  unsigned int uSummonerID;
  unsigned int uLastCharacterIDToHit;
  int dword_000334_unique_name;
  char field_338[12];
};
#pragma pack(pop)






//extern Actor pMonsterInfoUI_Doll;

extern std::array<Actor, 500> pActors;
extern size_t uNumActors;
