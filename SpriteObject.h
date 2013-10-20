#pragma once
#include "Items.h"

enum
{
    OBJECT_40 = 0x40
  , OBJECT_ATTACHED_TO_ACTOR = 0x80
};

#define MAX_SPRITE_OBJECTS 1000
/*   72 */
#pragma pack(push, 1)
struct SpriteObject
{
  inline bool AttachedToActor() const {return (uAttributes & OBJECT_ATTACHED_TO_ACTOR) != 0;}

  SpriteObject();
  int Create(int yaw, int pitch, int a4, int a5);
  void _46BEF1_apply_spells_aoe();
  void ExplosionTraps();

  static void UpdateObject_fn0_BLV(unsigned int uLayingItemID);
  static void UpdateObject_fn0_ODM(unsigned int uLayingItemID);
  static void OnInteraction(unsigned int uLayingItemID);


  unsigned __int16 uType;
  unsigned __int16 uObjectDescID;
  struct Vec3_int_ vPosition;
  struct Vec3_short_ vVelocity;
  unsigned __int16 uFacing;
  unsigned __int16 uSoundID;
  unsigned __int16 uAttributes;
  __int16 uSectorID;
  unsigned __int16 uSpriteFrameID;
  __int16 field_20;
  __int16 field_22_glow_radius_multiplier;
  struct ItemGen stru_24;
  int spell_id;
  int spell_level;
  int spell_skill;
  int field_54;
  int spell_caster_pid;
  int spell_target_pid;
  char field_60_distance_related_prolly_lod;
  char field_61;
  char field_62[2];
  Vec3_int_ field_64;
};
#pragma pack(pop)



extern size_t uNumSpriteObjects;
extern std::array<SpriteObject, MAX_SPRITE_OBJECTS> pSpriteObjects;