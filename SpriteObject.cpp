#define _CRT_SECURE_NO_WARNINGS
#include "Sprites.h"
#include "BSPModel.h"
#include "SpriteObject.h"
#include "Party.h"
#include "TurnEngine.h"
#include "MapInfo.h"
#include "OurMath.h"
#include "ObjectList.h"
#include "Indoor.h"
#include "Outdoor.h"
#include "ParticleEngine.h"
#include "Timer.h"
#include "Game.h"
#include "LOD.h"
#include "Actor.h"
#include "Events.h"
#include "AudioPlayer.h"
#include "Level/Decoration.h"
#include "mm7_unsorted_subs.h"

#include "mm7_data.h"
#include "MM7.h"








size_t uNumSpriteObjects;
std::array<SpriteObject, MAX_SPRITE_OBJECTS> pSpriteObjects;

//----- (00404828) --------------------------------------------------------
SpriteObject::SpriteObject()
{
  field_22_glow_radius_multiplier = 1;
  uSoundID = 0;
  uFacing = 0;
  vVelocity.z = 0;
  vVelocity.y = 0;
  vVelocity.x = 0;
  uType = 0;
  uObjectDescID = 0;
  field_61 = 0;
  field_60_distance_related_prolly_lod = 0;
  field_20 = 0;
  uSpriteFrameID = 0;
  spell_skill = 0;
  spell_level = 0;
  spell_id = 0;
  field_54 = 0;
}

//----- (0042F5ED) --------------------------------------------------------
int SpriteObject::Create(int yaw, int pitch, int a4, int a5)
{
  signed int v6; // ebx@2
  int v13; // ST2C_4@20
  Vec3_int_ v17; // [sp-20h] [bp-30h]@11
  int angle; // [sp+Ch] [bp-4h]@1
  int a5a; // [sp+20h] [bp+10h]@20

  angle = yaw;
  if (!uObjectDescID)
    return -1;

  v6 = 1000;
  for (uint i = 0; i < MAX_SPRITE_OBJECTS; ++i)
    if (!pSpriteObjects[i].uObjectDescID)
    {
      v6 = i;
      break;
    }

  if ( v6 >= 1000 )
    return -1;
  field_64.x = vPosition.x;
  field_64.y = vPosition.y;
  field_64.z = vPosition.z;

  assert(sizeof(SpriteObject) == 0x70);
  memcpy(&pSpriteObjects[v6], this, sizeof(*this));
  if ( a5 == 0 )
  {
    pSpriteObjects[v6].vVelocity.z = 0;
    if ( a4 )
    {
      v13 = fixpoint_mul(stru_5C6E00->Cos(angle), stru_5C6E00->Cos(pitch));
      a5a = fixpoint_mul(stru_5C6E00->Sin(angle), stru_5C6E00->Cos(pitch));
      pSpriteObjects[v6].vVelocity.x = fixpoint_mul(v13, a4);
      pSpriteObjects[v6].vVelocity.y = fixpoint_mul(a5a, a4);
      pSpriteObjects[v6].vVelocity.z = fixpoint_mul(stru_5C6E00->Sin(pitch), a4);
    }
    else
    {
      pSpriteObjects[v6].vVelocity.y = 0;
      pSpriteObjects[v6].vVelocity.x = 0;
    }
    if ( v6 >= (signed int)uNumSpriteObjects )
      uNumSpriteObjects = v6 + 1;
    return v6;
  }
  if ( a5 == 1 )
  {
    v17.x = vPosition.x;
    v17.y = vPosition.y;
    v17.z = vPosition.z;
    Vec3_int_::Rotate(24, stru_5C6E00->uIntegerHalfPi + pSpriteObjects[v6].uFacing, 0, v17, &pSpriteObjects[v6].vPosition.x,
                      &pSpriteObjects[v6].vPosition.y, &pSpriteObjects[v6].vPosition.z);
    pSpriteObjects[v6].vVelocity.z = 0;
    if ( a4 )
    {
      v13 = fixpoint_mul(stru_5C6E00->Cos(angle), stru_5C6E00->Cos(pitch));
      a5a = fixpoint_mul(stru_5C6E00->Sin(angle), stru_5C6E00->Cos(pitch));
      pSpriteObjects[v6].vVelocity.x = fixpoint_mul(v13, a4);
      pSpriteObjects[v6].vVelocity.y = fixpoint_mul(a5a, a4);
      pSpriteObjects[v6].vVelocity.z = fixpoint_mul(stru_5C6E00->Sin(pitch), a4);
    }
    else
    {
      pSpriteObjects[v6].vVelocity.y = 0;
      pSpriteObjects[v6].vVelocity.x = 0;
    }
    if ( v6 >= (signed int)uNumSpriteObjects )
      uNumSpriteObjects = v6 + 1;
    return v6;
  }
  if ( a5 == 2 )
  {
    v17.x = vPosition.x;
    v17.y = vPosition.y;
    v17.z = vPosition.z;
    Vec3_int_::Rotate(8, stru_5C6E00->uIntegerHalfPi + pSpriteObjects[v6].uFacing, 0, v17, &pSpriteObjects[v6].vPosition.x,
                      &pSpriteObjects[v6].vPosition.y, &pSpriteObjects[v6].vPosition.z);
    pSpriteObjects[v6].vVelocity.z = 0;
    if ( a4 )
    {
      v13 = fixpoint_mul(stru_5C6E00->Cos(angle), stru_5C6E00->Cos(pitch)) >> 16;
      a5a = fixpoint_mul(stru_5C6E00->Sin(angle), stru_5C6E00->Cos(pitch)) >> 16;
      pSpriteObjects[v6].vVelocity.x = fixpoint_mul(v13, a4);
      pSpriteObjects[v6].vVelocity.y = fixpoint_mul(a5a, a4);
      pSpriteObjects[v6].vVelocity.z = fixpoint_mul(stru_5C6E00->Sin(pitch), a4);
    }
    else
    {
      pSpriteObjects[v6].vVelocity.y = 0;
      pSpriteObjects[v6].vVelocity.x = 0;
    }
    if ( v6 >= (signed int)uNumSpriteObjects )
      uNumSpriteObjects = v6 + 1;
    return v6;
  }
  if ( a5 == 3 )
  {
    v17.x = vPosition.x;
    v17.y = vPosition.y;
    v17.z = vPosition.z;
    Vec3_int_::Rotate(8, pSpriteObjects[v6].uFacing - stru_5C6E00->uIntegerHalfPi, 0, v17, &pSpriteObjects[v6].vPosition.x,
                      &pSpriteObjects[v6].vPosition.y, &pSpriteObjects[v6].vPosition.z);
    pSpriteObjects[v6].vVelocity.z = 0;
    if ( a4 )
    {
      v13 = fixpoint_mul(stru_5C6E00->Cos(angle), stru_5C6E00->Cos(pitch));
      a5a = fixpoint_mul(stru_5C6E00->Sin(angle), stru_5C6E00->Cos(pitch));
      pSpriteObjects[v6].vVelocity.x = fixpoint_mul(v13, a4);
      pSpriteObjects[v6].vVelocity.y = fixpoint_mul(a5a, a4);
      pSpriteObjects[v6].vVelocity.z = fixpoint_mul(stru_5C6E00->Sin(pitch), a4);
    }
    else
    {
      pSpriteObjects[v6].vVelocity.y = 0;
      pSpriteObjects[v6].vVelocity.x = 0;
    }
    if ( v6 >= (signed int)uNumSpriteObjects )
      uNumSpriteObjects = v6 + 1;
    return v6;
  }
  if ( a5 == 4 )
  {
    v17.x = vPosition.x;
    v17.y = vPosition.y;
    v17.z = vPosition.z;
    Vec3_int_::Rotate(24, pSpriteObjects[v6].uFacing - stru_5C6E00->uIntegerHalfPi, 0, v17, &pSpriteObjects[v6].vPosition.x,
                      &pSpriteObjects[v6].vPosition.y, &pSpriteObjects[v6].vPosition.z);
    pSpriteObjects[v6].vVelocity.z = 0;
    if ( a4 )
    {
      v13 = fixpoint_mul(stru_5C6E00->Cos(angle), stru_5C6E00->Cos(pitch));
      a5a = fixpoint_mul(stru_5C6E00->Sin(angle), stru_5C6E00->Cos(pitch));
      pSpriteObjects[v6].vVelocity.x = fixpoint_mul(v13, a4);
      pSpriteObjects[v6].vVelocity.y = fixpoint_mul(a5a, a4);
      pSpriteObjects[v6].vVelocity.z = fixpoint_mul(stru_5C6E00->Sin(pitch), a4);
    }
    else
    {
      pSpriteObjects[v6].vVelocity.y = 0;
      pSpriteObjects[v6].vVelocity.x = 0;
    }
    if ( v6 >= (signed int)uNumSpriteObjects )
      uNumSpriteObjects = v6 + 1;
    return v6;
  }

  assert(false);
  return 0;
}

//----- (00471C03) --------------------------------------------------------
void SpriteObject::UpdateObject_fn0_ODM(unsigned int uLayingItemID)
{
  ObjectDesc *object; // ebx@1
  int v6; // eax@1
  int v7; // ecx@1
  int v8; // edi@1
  int v9; // eax@4
//  int v17; // ST10_4@25
  //signed int v19; // eax@28
  //Actor *v20; // edi@31
  int v21; // eax@41
//  int v22; // ecx@43
//  __int16 v23; // bx@45
//  char v24; // al@46
  signed int i; // edi@50
  int v26; // edi@52
  int v27; // eax@52
  __int16 v28; // cx@55
  int v29; // eax@55
  //signed int v30; // edi@59
  BSPModel *bmodel; // ecx@61
  ODMFace *face; // edi@61
//  int v33; // eax@62
//  int v34; // ecx@62
  int v35; // eax@63
  int v36; // ecx@67
  __int16 v37; // ax@67
  int v38; // eax@72
  //int v39; // eax@72
//  unsigned __int64 v40; // qax@72
//  int v41; // eax@72
//  unsigned __int8 v42; // sf@74
//  unsigned __int8 v43; // of@74
  int v44; // eax@77
//  __int16 v45; // bx@81
//  int v46; // eax@85
//  const char *v47; // [sp-8h] [bp-B0h]@83
//  enum TEXTURE_TYPE v48; // [sp-4h] [bp-ACh]@46
  int v49; // [sp+Ch] [bp-9Ch]@52
  int v50; // [sp+10h] [bp-98h]@52
  Vec3_int_ v51; // [sp+14h] [bp-94h]@11
  Particle_sw Dst; // [sp+20h] [bp-88h]@45
  int v54; // [sp+8Ch] [bp-1Ch]@1
  int v55; // [sp+90h] [bp-18h]@1
  int v56; // [sp+94h] [bp-14h]@11
  int v57; // [sp+98h] [bp-10h]@1
  int v58; // [sp+9Ch] [bp-Ch]@1
  int on_water; // [sp+A0h] [bp-8h]@1
  int v60; // [sp+A4h] [bp-4h]@11

  v58 = 0;
  object = &pObjectList->pObjects[pSpriteObjects[uLayingItemID].uObjectDescID];
  v57 = IsTerrainSlopeTooHigh(pSpriteObjects[uLayingItemID].vPosition.x, pSpriteObjects[uLayingItemID].vPosition.y);
  v55 = 0;
  v6 = ODM_GetFloorLevel(pSpriteObjects[uLayingItemID].vPosition.x, pSpriteObjects[uLayingItemID].vPosition.y, pSpriteObjects[uLayingItemID].vPosition.z, object->uHeight, &on_water, &v55, 0);
  v7 = v6;
  v54 = v6;
  v8 = v6 + 1;
  if ( pSpriteObjects[uLayingItemID].vPosition.z <= v6 + 1 )
  {
    if ( on_water )
    {
      v9 = v6 + 60;
      if ( v55 )
        v9 = v6 + 30;
      sub_42F960_create_object(pSpriteObjects[uLayingItemID].vPosition.x, pSpriteObjects[uLayingItemID].vPosition.y, v9);
      SpriteObject::OnInteraction(uLayingItemID);
    }
  }
  else
    v58 = 1;
  if ( !(object->uFlags & OBJECT_DESC_NO_GRAVITY) )
  {
    if ( v58 )
    {
      pSpriteObjects[uLayingItemID].vVelocity.z -= LOWORD(pEventTimer->uTimeElapsed) * GetGravityStrength();
      goto LABEL_13;
    }
    if ( v57 )
    {
      pSpriteObjects[uLayingItemID].vPosition.z = v8;
      ODM_GetTerrainNormalAt(pSpriteObjects[uLayingItemID].vPosition.x, pSpriteObjects[uLayingItemID].vPosition.y, &v51);
      pSpriteObjects[uLayingItemID].vVelocity.z -= LOWORD(pEventTimer->uTimeElapsed) * GetGravityStrength();
      v56 = abs(v51.y * pSpriteObjects[uLayingItemID].vVelocity.y + v51.z * pSpriteObjects[uLayingItemID].vVelocity.z + v51.x * pSpriteObjects[uLayingItemID].vVelocity.x) >> 16;
      //v60 = ((unsigned __int64)(v56 * (signed __int64)v51.x) >> 16);
      pSpriteObjects[uLayingItemID].vVelocity.x += fixpoint_mul(v56, v51.x);
      //v60 = ((unsigned __int64)(v56 * (signed __int64)v51.y) >> 16);
      pSpriteObjects[uLayingItemID].vVelocity.y += fixpoint_mul(v56, v51.y);
      //v60 = ((unsigned __int64)(v56 * (signed __int64)v51.z) >> 16);
      pSpriteObjects[uLayingItemID].vVelocity.z += fixpoint_mul(v56, v51.z);
      v7 = v54;
      goto LABEL_13;
    }
    if ( object->uFlags & OBJECT_DESC_INTERACTABLE )
    {
      if ( pSpriteObjects[uLayingItemID].vPosition.z < v7 )
        pSpriteObjects[uLayingItemID].vPosition.z = v8;
      if ( !_46BFFA_check_object_intercept(uLayingItemID, 0) )
        return;
    }
    pSpriteObjects[uLayingItemID].vPosition.z = v8;
    if ( !(object->uFlags & OBJECT_DESC_BOUNCE) || (v21 = -pSpriteObjects[uLayingItemID].vVelocity.z >> 1, pSpriteObjects[uLayingItemID].vVelocity.z = v21, (signed __int16)v21 < 10) )
      pSpriteObjects[uLayingItemID].vVelocity.z = 0;

    pSpriteObjects[uLayingItemID].vVelocity.x = fixpoint_mul(58500, pSpriteObjects[uLayingItemID].vVelocity.x);
    pSpriteObjects[uLayingItemID].vVelocity.y = fixpoint_mul(58500, pSpriteObjects[uLayingItemID].vVelocity.y);
    pSpriteObjects[uLayingItemID].vVelocity.z = fixpoint_mul(58500, pSpriteObjects[uLayingItemID].vVelocity.z);
    if ( (pSpriteObjects[uLayingItemID].vVelocity.y * pSpriteObjects[uLayingItemID].vVelocity.y
        + pSpriteObjects[uLayingItemID].vVelocity.x * pSpriteObjects[uLayingItemID].vVelocity.x) < 400 )
    {
      pSpriteObjects[uLayingItemID].vVelocity.y = 0;
      pSpriteObjects[uLayingItemID].vVelocity.x = 0;
      memset(&Dst, 0, 0x68u);
      Dst.x = (double)pSpriteObjects[uLayingItemID].vPosition.x;
      Dst.y = (double)pSpriteObjects[uLayingItemID].vPosition.y;
      Dst.z = (double)pSpriteObjects[uLayingItemID].vPosition.z;
      Dst.r = 0.0;
      Dst.g = 0.0;
      Dst.b = 0.0;
      if (object->uFlags & OBJECT_DESC_TRIAL_FIRE )
      {
        Dst.type = ParticleType_Bitmap | ParticleType_Rotating | ParticleType_8;
        Dst.uDiffuse = 0xFF3C1E;
        Dst.timeToLive = (unsigned __int8)(rand() & 0x80) + 128;
        Dst.uTextureID = pBitmaps_LOD->LoadTexture("effpar01", TEXTURE_DEFAULT);
        Dst.flt_28 = 1.0;
        pGame->pParticleEngine->AddParticle(&Dst);
      }
      else if ( object->uFlags & OBJECT_DESC_TRIAL_LINE)
      {
        Dst.type = ParticleType_Line;
        Dst.uDiffuse = rand();
        Dst.timeToLive = 64;
        Dst.uTextureID = 0;
        Dst.flt_28 = 1.0;
        pGame->pParticleEngine->AddParticle(&Dst);
      }
      else if ( object->uFlags & OBJECT_DESC_TRIAL_PARTICLE )
      {
        Dst.type = ParticleType_Bitmap | ParticleType_8;
        Dst.uDiffuse = rand();
        Dst.timeToLive = (unsigned __int8)(rand() & 0x80) + 128;
        Dst.uTextureID = pBitmaps_LOD->LoadTexture("effpar03", TEXTURE_DEFAULT);
        Dst.flt_28 = 1.0;
        pGame->pParticleEngine->AddParticle(&Dst);
      }
      return;
    }
  }
LABEL_13:
  if ( pSpriteObjects[uLayingItemID].vPosition.x >= -0x8000 && pSpriteObjects[uLayingItemID].vPosition.x <= 0x8000
    && pSpriteObjects[uLayingItemID].vPosition.y >= -0x8000 && pSpriteObjects[uLayingItemID].vPosition.y <= 0x8000
    && pSpriteObjects[uLayingItemID].vPosition.z > v7 && pSpriteObjects[uLayingItemID].vPosition.z <= 13000
    || !(object->uFlags & OBJECT_DESC_INTERACTABLE) )
    goto LABEL_92;
  if ( pSpriteObjects[uLayingItemID].vPosition.z < v7 )
    pSpriteObjects[uLayingItemID].vPosition.z = v8;
  if ( _46BFFA_check_object_intercept(uLayingItemID, 0) )
  {
LABEL_92:
    stru_721530.field_0 = 0;
    stru_721530.prolly_normal_d = object->uRadius;
    stru_721530.height = object->uHeight;
    stru_721530.field_8_radius = 0;
    stru_721530.field_70 = 0;
    for ( v55 = 0; v55 < 100; ++v55 )
    {
      stru_721530.position.x = pSpriteObjects[uLayingItemID].vPosition.x;
      stru_721530.normal.x = stru_721530.position.x;
      stru_721530.uSectorID = 0;
      stru_721530.position.y = pSpriteObjects[uLayingItemID].vPosition.y;
      stru_721530.normal.y = pSpriteObjects[uLayingItemID].vPosition.y;
      stru_721530.position.z = pSpriteObjects[uLayingItemID].vPosition.z + stru_721530.prolly_normal_d + 1;
      stru_721530.normal.z = stru_721530.position.z;
      stru_721530.velocity.x = pSpriteObjects[uLayingItemID].vVelocity.x;
      stru_721530.velocity.y = pSpriteObjects[uLayingItemID].vVelocity.y;
      stru_721530.velocity.z = pSpriteObjects[uLayingItemID].vVelocity.z;
      if ( stru_721530._47050A(0) )
        return;
      _46E889_collide_against_bmodels(0);
      _46E26D_collide_against_sprites(WorldPosToGridCellX(pSpriteObjects[uLayingItemID].vPosition.x), WorldPosToGridCellZ(pSpriteObjects[uLayingItemID].vPosition.y));
      if (PID_TYPE(pSpriteObjects[uLayingItemID].spell_caster_pid) != OBJECT_Player)
        _46EF01_collision_chech_player(0);
      if (PID_TYPE(pSpriteObjects[uLayingItemID].spell_caster_pid) == OBJECT_Actor)
      {
        if (( PID_ID(pSpriteObjects[uLayingItemID].spell_caster_pid) >= 0 )
          &&( PID_ID(pSpriteObjects[uLayingItemID].spell_caster_pid) < (signed int)(uNumActors - 1) ))
        {
          for (v56 =0; v56 < uNumActors; ++v56)
          {
            if ( pActors[PID_ID(pSpriteObjects[uLayingItemID].spell_caster_pid)].GetActorsRelation(&pActors[v56]) )
              Actor::_46DF1A_collide_against_actor(v56, 0);
          }
        }
      }
      else
      {
        for ( i = 0; i < (signed int)uNumActors; ++i )
          Actor::_46DF1A_collide_against_actor(i, 0);
      }
      v26 = stru_721530.normal2.z - stru_721530.prolly_normal_d - 1;
      v27 = ODM_GetFloorLevel( stru_721530.normal2.x, stru_721530.normal2.y, stru_721530.normal2.z - stru_721530.prolly_normal_d - 1,
              object->uHeight, &v49, &v50, 0);
      if ( on_water && v26 < v27 + 60 )
      {
        if ( v50 )
          v44 = v27 + 30;
        else
          v44 = v54 + 60;
        sub_42F960_create_object(pSpriteObjects[uLayingItemID].vPosition.x, pSpriteObjects[uLayingItemID].vPosition.y, v44);
        SpriteObject::OnInteraction(uLayingItemID);
        return;
      }
      if ( stru_721530.field_7C >= stru_721530.field_6C )
      {
        pSpriteObjects[uLayingItemID].vPosition.x = stru_721530.normal2.x;
        pSpriteObjects[uLayingItemID].vPosition.y = stru_721530.normal2.y;
        pSpriteObjects[uLayingItemID].vPosition.z = stru_721530.normal2.z - stru_721530.prolly_normal_d - 1;
        pSpriteObjects[uLayingItemID].uSectorID = LOWORD(stru_721530.uSectorID);
        memset(&Dst, 0, 0x68u);
        Dst.x = (double)pSpriteObjects[uLayingItemID].vPosition.x;
        Dst.y = (double)pSpriteObjects[uLayingItemID].vPosition.y;
        Dst.z = (double)pSpriteObjects[uLayingItemID].vPosition.z;
        Dst.r = 0.0;
        Dst.g = 0.0;
        Dst.b = 0.0;
        if ( object->uFlags & OBJECT_DESC_TRIAL_FIRE )
        {
          Dst.type = ParticleType_Bitmap | ParticleType_Rotating | ParticleType_8;
          Dst.uDiffuse = 0xFF3C1E;
          Dst.timeToLive = (unsigned __int8)( rand() & 0x80) + 128;
          Dst.uTextureID = pBitmaps_LOD->LoadTexture("effpar01", TEXTURE_DEFAULT);
          Dst.flt_28 = 1.0;
          pGame->pParticleEngine->AddParticle(&Dst);
          return;
        }
        else if ( object->uFlags & OBJECT_DESC_TRIAL_LINE )
        {
          Dst.type = ParticleType_Line;
          Dst.uTextureID = 0;
          Dst.uDiffuse = rand();
          Dst.timeToLive = 64;
          Dst.flt_28 = 1.0;
          pGame->pParticleEngine->AddParticle(&Dst);
          return;
        }
        else if ( object->uFlags & OBJECT_DESC_TRIAL_PARTICLE)
        {
          Dst.type = ParticleType_Bitmap | ParticleType_8;
          Dst.uDiffuse = rand();
          Dst.timeToLive = (unsigned __int8)(rand() & 0x80) + 128;
          Dst.uTextureID = pBitmaps_LOD->LoadTexture("effpar03", TEXTURE_DEFAULT);
          Dst.flt_28 = 1.0;
          pGame->pParticleEngine->AddParticle(&Dst);
        }
        return;
      }
      //v60 = ((unsigned __int64)(stru_721530.field_7C * (signed __int64)stru_721530.direction.x) >> 16);
      pSpriteObjects[uLayingItemID].vPosition.x += fixpoint_mul(stru_721530.field_7C, stru_721530.direction.x);
      //v60 = ((unsigned __int64)(stru_721530.field_7C * (signed __int64)stru_721530.direction.y) >> 16);
      pSpriteObjects[uLayingItemID].vPosition.y += fixpoint_mul(stru_721530.field_7C, stru_721530.direction.y);
      //v60 = ((unsigned __int64)(stru_721530.field_7C * (signed __int64)stru_721530.direction.z) >> 16);
      v28 = LOWORD(stru_721530.uSectorID);
      pSpriteObjects[uLayingItemID].vPosition.z += fixpoint_mul(stru_721530.field_7C, stru_721530.direction.z);
      v29 = pSpriteObjects[uLayingItemID].vPosition.z;
      pSpriteObjects[uLayingItemID].uSectorID = v28;
      stru_721530.field_70 += stru_721530.field_7C;
      if ( object->uFlags & OBJECT_DESC_INTERACTABLE )
      {
        if ( v29 < v54 )
          pSpriteObjects[uLayingItemID].vPosition.z = v54 + 1;
        if ( !_46BFFA_check_object_intercept(uLayingItemID, stru_721530.uFaceID) )
          return;
      }
      if (PID_TYPE(stru_721530.uFaceID) == OBJECT_Decoration)
        break;
      if (PID_TYPE(stru_721530.uFaceID) == OBJECT_BModel)
      {
        bmodel = &pOutdoor->pBModels[(signed int)stru_721530.uFaceID >> 9];
        face = &bmodel->pFaces[PID_ID(stru_721530.uFaceID) & 0x3F];
        if ( face->uPolygonType != POLYGON_Floor )
        {
          v56 = abs(face->pFacePlane.vNormal.x * pSpriteObjects[uLayingItemID].vVelocity.x
                  + face->pFacePlane.vNormal.y * pSpriteObjects[uLayingItemID].vVelocity.y
                  + face->pFacePlane.vNormal.z * pSpriteObjects[uLayingItemID].vVelocity.z) >> 16;
          if ( (stru_721530.speed >> 3) > v56 )
            v56 = stru_721530.speed >> 3;
          //v57 = fixpoint_mul(v56, face->pFacePlane.vNormal.x);
          //v58 = fixpoint_mul(v56, face->pFacePlane.vNormal.y);
          v60 = fixpoint_mul(v56, face->pFacePlane.vNormal.z);
          pSpriteObjects[uLayingItemID].vVelocity.x += 2 * fixpoint_mul(v56, face->pFacePlane.vNormal.x);
          pSpriteObjects[uLayingItemID].vVelocity.y += 2 * fixpoint_mul(v56, face->pFacePlane.vNormal.y);
          if ( face->pFacePlane.vNormal.z <= 32000 )
            v37 = 2 * (short)v60;
          else
          {
            v36 = v60;
            pSpriteObjects[uLayingItemID].vVelocity.z += (signed __int16)v60;
            v58 = fixpoint_mul(0x7D00, v36);
            v37 = fixpoint_mul(32000, v36);
          }
          pSpriteObjects[uLayingItemID].vVelocity.z += v37;
          if ( BYTE3(face->uAttributes) & 0x10 )
            EventProcessor(face->sCogTriggeredID, 0, 1);
          goto LABEL_74;
        }
        pSpriteObjects[uLayingItemID].vPosition.z = bmodel->pVertices.pVertices[face->pVertexIDs[0]].z + 1;
        if ( pSpriteObjects[uLayingItemID].vVelocity.x * pSpriteObjects[uLayingItemID].vVelocity.x
           + pSpriteObjects[uLayingItemID].vVelocity.y * pSpriteObjects[uLayingItemID].vVelocity.y >= 400 )
        {
          if ( face->uAttributes & 0x10000000 )
            EventProcessor(face->sCogTriggeredID, 0, 1);
          goto LABEL_74;
        }
        LOWORD(v35) = 0;
        pSpriteObjects[uLayingItemID].vVelocity.z = 0;
        pSpriteObjects[uLayingItemID].vVelocity.x = 0;
        pSpriteObjects[uLayingItemID].vVelocity.y = v35;
      }
LABEL_74:
      pSpriteObjects[uLayingItemID].vVelocity.x = fixpoint_mul(58500, pSpriteObjects[uLayingItemID].vVelocity.x);
      pSpriteObjects[uLayingItemID].vVelocity.y = fixpoint_mul(58500, pSpriteObjects[uLayingItemID].vVelocity.y);
      pSpriteObjects[uLayingItemID].vVelocity.z = fixpoint_mul(58500, pSpriteObjects[uLayingItemID].vVelocity.z);
    }
    v57 = integer_sqrt(pSpriteObjects[uLayingItemID].vVelocity.x * pSpriteObjects[uLayingItemID].vVelocity.x
                     + pSpriteObjects[uLayingItemID].vVelocity.y * pSpriteObjects[uLayingItemID].vVelocity.y);
    v38 = stru_5C6E00->Atan2(pSpriteObjects[uLayingItemID].vPosition.x - pLevelDecorations[PID_ID(stru_721530.uFaceID)].vPosition.x,
                             pSpriteObjects[uLayingItemID].vPosition.y - pLevelDecorations[PID_ID(stru_721530.uFaceID)].vPosition.y);
    pSpriteObjects[uLayingItemID].vVelocity.x = fixpoint_mul(stru_5C6E00->Cos(v38), v57);
    pSpriteObjects[uLayingItemID].vVelocity.y = fixpoint_mul(stru_5C6E00->Sin(v38 - stru_5C6E00->uIntegerHalfPi), v57);
    goto LABEL_74;
  }
}

//----- (0047136C) --------------------------------------------------------
void SpriteObject::UpdateObject_fn0_BLV(unsigned int uLayingItemID)
{
  SpriteObject *pSpriteObject; // esi@1
  ObjectDesc *pObject; // edi@1
//  int v9; // ecx@16
//  __int16 v10; // di@18
//  int v14; // ebx@34
  signed int v15; // ebx@46
  int v17; // eax@50
//  int v18; // eax@52
//  int v19; // ecx@52
//  Vec3_short_ *v20; // ecx@53
  __int16 v22; // ax@57
  int v23; // edi@62
//  unsigned __int8 v27; // sf@64
//  unsigned __int8 v28; // of@64
//  __int16 v29; // di@67
//  char v30; // al@68
  Particle_sw Dst; // [sp+Ch] [bp-84h]@18
  unsigned int uFaceID; // [sp+7Ch] [bp-14h]@4
  int v39; // [sp+80h] [bp-10h]@33
  int v40; // [sp+84h] [bp-Ch]@28
   int v42; // [sp+8Ch] [bp-4h]@4

  pSpriteObject = &pSpriteObjects[uLayingItemID];
  pObject = &pObjectList->pObjects[pSpriteObject->uObjectDescID];
  pSpriteObject->uSectorID = pIndoor->GetSector(pSpriteObject->vPosition.x, pSpriteObject->vPosition.y, pSpriteObject->vPosition.z);
  v42 = BLV_GetFloorLevel(pSpriteObject->vPosition.x, pSpriteObject->vPosition.y, pSpriteObject->vPosition.z, pSpriteObject->uSectorID, &uFaceID);
  if ( abs(pSpriteObject->vPosition.x) > 32767
    || abs(pSpriteObject->vPosition.y) > 32767
    || abs(pSpriteObject->vPosition.z) > 20000
    || v42 <= -30000
    && (pSpriteObject->uSectorID == 0))
//     || (v42 = _46CEC3_get_floor_level(pSpriteObject->vPosition.x, pSpriteObject->vPosition.y, pSpriteObject->vPosition.z, v4, &uFaceID), v42 == -30000)) )
  {
    SpriteObject::OnInteraction(uLayingItemID);
    return;
  }
  if ( pObject->uFlags & OBJECT_DESC_NO_GRAVITY )//не падающие объекты
  {
LABEL_25:
    stru_721530.field_0 = 0;
    stru_721530.prolly_normal_d = pObject->uRadius;
    stru_721530.field_84 = -1;
    stru_721530.height = pObject->uHeight;
    stru_721530.field_8_radius = 0;
    stru_721530.field_70 = 0;
    for ( uFaceID = 0; uFaceID < 100; uFaceID++ )
    {
      stru_721530.position.x = pSpriteObject->vPosition.x;
      stru_721530.position.y = pSpriteObject->vPosition.y;
      stru_721530.position.z = stru_721530.prolly_normal_d + pSpriteObject->vPosition.z + 1;

      stru_721530.normal.x = stru_721530.position.x;
      stru_721530.normal.y = stru_721530.position.y;
      stru_721530.normal.z = stru_721530.position.z;

      stru_721530.velocity.x = pSpriteObject->vVelocity.x;
      stru_721530.velocity.y = pSpriteObject->vVelocity.y;
      stru_721530.velocity.z = pSpriteObject->vVelocity.z;

      stru_721530.uSectorID = pSpriteObject->uSectorID;
      if ( stru_721530._47050A(0) )
        return;

      for ( v40 = 0; v40 < 100; ++v40 )
      {
        _46E44E_collide_against_faces_and_portals(0);
        _46E0B2_collide_against_decorations();
        if (PID_TYPE(pSpriteObject->spell_caster_pid) != OBJECT_Player)
          _46EF01_collision_chech_player(1);
        if (PID_TYPE(pSpriteObject->spell_caster_pid) == OBJECT_Actor)
        {
          for ( v42 = 0; v42 < (signed int)uNumActors; ++v42 )
          {
            if( pActors[pSpriteObject->spell_caster_pid >> 3].pMonsterInfo.uID != pActors[v42].pMonsterInfo.uID )
             //not sure: pMonsterList->pMonsters[v39b->word_000086_some_monster_id-1].uToHitRadius
              Actor::_46DF1A_collide_against_actor(v42, pMonsterList->pMonsters[pActors[v42].word_000086_some_monster_id-1].uToHitRadius);
          }
        }
        else
        {
          for ( v42 = 0; v42 < (signed int)uNumActors; v42++ )
            Actor::_46DF1A_collide_against_actor(v42, pMonsterList->pMonsters[pActors[v42].word_000086_some_monster_id-1].uToHitRadius);
        }
        if ( _46F04E_collide_against_portals() )
          break;
      }
      if ( stru_721530.field_7C >= stru_721530.field_6C )
      {
        pSpriteObject->vPosition.x = stru_721530.normal2.x;
        pSpriteObject->vPosition.y = stru_721530.normal2.y;
        pSpriteObject->vPosition.z = stru_721530.normal2.z - stru_721530.prolly_normal_d - 1;
        pSpriteObject->uSectorID = LOWORD(stru_721530.uSectorID);
        if ( !(HIBYTE(pObject->uFlags) & 1) )
          return;
        memset(&Dst, 0, 0x68u);
        Dst.x = (double)pSpriteObject->vPosition.x;
        Dst.y = (double)pSpriteObject->vPosition.y;
        Dst.z = (double)pSpriteObject->vPosition.z;
        Dst.r = 0.0;
        Dst.g = 0.0;
        Dst.b = 0.0;
        if ( pObject->uFlags & OBJECT_DESC_TRIAL_FIRE )
        {
          Dst.type = ParticleType_Bitmap | ParticleType_Rotating | ParticleType_8;
          Dst.uDiffuse = 0xFF3C1E;
          Dst.timeToLive = (unsigned __int8)(rand() & 0x80) + 128;
          Dst.uTextureID = pBitmaps_LOD->LoadTexture("effpar01", TEXTURE_DEFAULT);
          Dst.flt_28 = 1.0;
          pGame->pParticleEngine->AddParticle(&Dst);
          return;
        }
        else if ( pObject->uFlags & OBJECT_DESC_TRIAL_LINE )
        {
          Dst.type = ParticleType_Line;
          Dst.uDiffuse = rand();
          Dst.timeToLive = 64;
          Dst.uTextureID = 0;
          Dst.flt_28 = 1.0;
          pGame->pParticleEngine->AddParticle(&Dst);
          return;
        }
        else if ( pObject->uFlags & OBJECT_DESC_TRIAL_PARTICLE)
        {
          Dst.type = ParticleType_Bitmap | ParticleType_8;
          Dst.uDiffuse = rand();
          Dst.timeToLive = (unsigned __int8)(rand() & 0x80) + 128;
          Dst.uTextureID = pBitmaps_LOD->LoadTexture("effpar03", TEXTURE_DEFAULT);
          Dst.flt_28 = 1.0;
          pGame->pParticleEngine->AddParticle(&Dst);
        }
        return;
      }
      //v40 = (unsigned __int64)(stru_721530.field_7C * (signed __int64)stru_721530.direction.x) >> 16;
      pSpriteObject->vPosition.x += fixpoint_mul(stru_721530.field_7C, stru_721530.direction.x);
      //v40 = (unsigned __int64)(stru_721530.field_7C * (signed __int64)stru_721530.direction.y) >> 16;
      pSpriteObject->vPosition.y += fixpoint_mul(stru_721530.field_7C, stru_721530.direction.y);
      //v40 = (unsigned __int64)(stru_721530.field_7C * (signed __int64)stru_721530.direction.z) >> 16;
      pSpriteObject->vPosition.z += fixpoint_mul(stru_721530.field_7C, stru_721530.direction.z);
      pSpriteObject->uSectorID = stru_721530.uSectorID;
      stru_721530.field_70 += stru_721530.field_7C;
      if ( pObject->uFlags & OBJECT_DESC_INTERACTABLE && !_46BFFA_check_object_intercept(uLayingItemID, stru_721530.uFaceID) )
        return;
      v15 = (signed int)stru_721530.uFaceID >> 3;
      if (PID_TYPE(stru_721530.uFaceID) == OBJECT_Decoration)
      {
        v40 = integer_sqrt(pSpriteObject->vVelocity.x * pSpriteObject->vVelocity.x + pSpriteObject->vVelocity.y * pSpriteObject->vVelocity.y);
        v23 = stru_5C6E00->Atan2(pSpriteObject->vPosition.x - pLevelDecorations[v15].vPosition.x,
                pSpriteObject->vPosition.y - pLevelDecorations[v15].vPosition.y);
        pSpriteObject->vVelocity.x = fixpoint_mul(stru_5C6E00->Cos(v23), v40);
        pSpriteObject->vVelocity.y = fixpoint_mul(stru_5C6E00->Sin(v23), v40);
      }
      if (PID_TYPE(stru_721530.uFaceID) == OBJECT_BModel)
      {
        stru_721530.field_84 = (signed int)PID_ID(stru_721530.uFaceID);
        if ( pIndoor->pFaces[v15].uPolygonType != POLYGON_Floor )
        {
          v42 = abs(pIndoor->pFaces[v15].pFacePlane_old.vNormal.x * pSpriteObject->vVelocity.x
                  + pIndoor->pFaces[v15].pFacePlane_old.vNormal.y * pSpriteObject->vVelocity.y
                  + pIndoor->pFaces[v15].pFacePlane_old.vNormal.z * pSpriteObject->vVelocity.z) >> 16;
          if ( (stru_721530.speed >> 3) > v42 )
            v42 = stru_721530.speed >> 3;
          pSpriteObject->vVelocity.x += 2 * fixpoint_mul(v42, pIndoor->pFaces[v15].pFacePlane_old.vNormal.x);
          pSpriteObject->vVelocity.y += 2 * fixpoint_mul(v42, pIndoor->pFaces[v15].pFacePlane_old.vNormal.y);
          v39 = fixpoint_mul(v42, pIndoor->pFaces[v15].pFacePlane_old.vNormal.z);
          if ( pIndoor->pFaces[v15].pFacePlane_old.vNormal.z <= 32000 )
            v22 = 2 * v39;
          else
          {
            pSpriteObject->vVelocity.z += v39;
            v22 = fixpoint_mul(32000, v39);
          }
          pSpriteObject->vVelocity.z += v22;
          if ( pIndoor->pFaces[v15].uAttributes & FACE_UNKNOW2 )
            EventProcessor(pIndoor->pFaceExtras[pIndoor->pFaces[v15].uFaceExtraID].uEventID, 0, 1);
          pSpriteObject->vVelocity.x = fixpoint_mul(58500, pSpriteObject->vVelocity.x);
          pSpriteObject->vVelocity.y = fixpoint_mul(58500, pSpriteObject->vVelocity.y);
          pSpriteObject->vVelocity.z = fixpoint_mul(58500, pSpriteObject->vVelocity.z);
          continue;
        }
        if ( pObject->uFlags & OBJECT_DESC_BOUNCE )
        {
          v17 = -pSpriteObject->vVelocity.z / 2;
          pSpriteObject->vVelocity.z = v17;
          if ( (signed __int16)v17 < 10 )
            pSpriteObject->vVelocity.z = 0;
          if ( pIndoor->pFaces[v15].uAttributes & FACE_UNKNOW2 )
            EventProcessor(pIndoor->pFaceExtras[pIndoor->pFaces[v15].uFaceExtraID].uEventID, 0, 1);
          pSpriteObject->vVelocity.x = fixpoint_mul(58500, pSpriteObject->vVelocity.x);
          pSpriteObject->vVelocity.y = fixpoint_mul(58500, pSpriteObject->vVelocity.y);
          pSpriteObject->vVelocity.z = fixpoint_mul(58500, pSpriteObject->vVelocity.z);
          continue;
        }
        pSpriteObject->vVelocity.z = 0;
        if ( pSpriteObject->vVelocity.x * pSpriteObject->vVelocity.x + pSpriteObject->vVelocity.y * pSpriteObject->vVelocity.y >= 400 )
        {
          if ( pIndoor->pFaces[v15].uAttributes & FACE_UNKNOW2 )
            EventProcessor(pIndoor->pFaceExtras[pIndoor->pFaces[v15].uFaceExtraID].uEventID, 0, 1);
          pSpriteObject->vVelocity.x = fixpoint_mul(58500, pSpriteObject->vVelocity.x);
          pSpriteObject->vVelocity.y = fixpoint_mul(58500, pSpriteObject->vVelocity.y);
          pSpriteObject->vVelocity.z = fixpoint_mul(58500, pSpriteObject->vVelocity.z);
          continue;
        }
        pSpriteObject->vVelocity.z = 0;
        pSpriteObject->vVelocity.y = 0;
        pSpriteObject->vVelocity.x = 0;
        pSpriteObject->vPosition.z = pIndoor->pVertices[*pIndoor->pFaces[v15].pVertexIDs].z + 1;
      }
      pSpriteObject->vVelocity.x = fixpoint_mul(58500, pSpriteObject->vVelocity.x);
      pSpriteObject->vVelocity.y = fixpoint_mul(58500, pSpriteObject->vVelocity.y);
      pSpriteObject->vVelocity.z = fixpoint_mul(58500, pSpriteObject->vVelocity.z);
    }
  }
  //для падающих объектов(для примера выброс вещи из инвентаря)
  if ( v42 <= pSpriteObject->vPosition.z - 3 )
  {
    pSpriteObject->vVelocity.z -= LOWORD(pEventTimer->uTimeElapsed) * GetGravityStrength();
    goto LABEL_25;
  }
  if ( !(pObject->uFlags & OBJECT_DESC_INTERACTABLE) || _46BFFA_check_object_intercept(uLayingItemID, 0) )
  {
    pSpriteObject->vPosition.z = v42 + 1;
    if ( pIndoor->pFaces[uFaceID].uPolygonType == POLYGON_Floor )
      pSpriteObject->vVelocity.z = 0;
    else
    {
      if ( pIndoor->pFaces[uFaceID].pFacePlane_old.vNormal.z < 45000 )
        pSpriteObject->vVelocity.z -= LOWORD(pEventTimer->uTimeElapsed) * GetGravityStrength();
    }
    pSpriteObject->vVelocity.x = fixpoint_mul(58500, pSpriteObject->vVelocity.x);
    pSpriteObject->vVelocity.y = fixpoint_mul(58500, pSpriteObject->vVelocity.y);
    pSpriteObject->vVelocity.z = fixpoint_mul(58500, pSpriteObject->vVelocity.z);
    if ( pSpriteObject->vVelocity.x * pSpriteObject->vVelocity.x + pSpriteObject->vVelocity.y * pSpriteObject->vVelocity.y < 400 )
    {
      pSpriteObject->vVelocity.x = 0;
      pSpriteObject->vVelocity.y = 0;
      pSpriteObject->vVelocity.z = 0;
      if ( !(pObject->uFlags & OBJECT_DESC_NO_SPRITE) )
        return;
      memset(&Dst, 0, 0x68u);
      Dst.x = (double)pSpriteObject->vPosition.x;
      Dst.y = (double)pSpriteObject->vPosition.y;
      Dst.z = (double)pSpriteObject->vPosition.z;
      Dst.r = 0.0;
      Dst.g = 0.0;
      Dst.b = 0.0;
      if ( pObject->uFlags & OBJECT_DESC_TRIAL_FIRE )
      {
        Dst.type = ParticleType_Bitmap | ParticleType_Rotating | ParticleType_8;
        Dst.uDiffuse = 0xFF3C1E;
        Dst.flt_28 = 1.0;
        Dst.timeToLive = (unsigned __int8)(rand() & 0x80) + 128;
        Dst.uTextureID = pBitmaps_LOD->LoadTexture("effpar01", TEXTURE_DEFAULT);
        pGame->pParticleEngine->AddParticle(&Dst);
        return;
      }
      else if ( pObject->uFlags & OBJECT_DESC_TRIAL_LINE )
      {
        Dst.type = ParticleType_Line;
        Dst.uDiffuse = rand();
        Dst.timeToLive = 64;
        Dst.uTextureID = 0;
        Dst.flt_28 = 1.0;
        pGame->pParticleEngine->AddParticle(&Dst);
        return;
      }
      else if ( pObject->uFlags & OBJECT_DESC_TRIAL_PARTICLE)
      {
        Dst.type = ParticleType_Bitmap | ParticleType_8;
        Dst.uDiffuse = rand();
        Dst.flt_28 = 1.0;
        Dst.timeToLive = (unsigned __int8)(rand() & 0x80) + 128;
        Dst.uTextureID = pBitmaps_LOD->LoadTexture("effpar03", TEXTURE_DEFAULT);
        pGame->pParticleEngine->AddParticle(&Dst);
      }
      return;
    }
    goto LABEL_25;
  }
}
// 46DF1A: using guessed type int __fastcall 46DF1A_collide_against_actor(int, int);

//----- (00438E35) --------------------------------------------------------
void SpriteObject::ExplosionTraps()
{
  MapInfo *pMapInfo; // esi@1
  int dir_x; // ebx@1
  int v7; // edx@2
  unsigned int v10; // eax@7
  signed int v11; // ebx@8
  signed int v13; // edi@20
  int dir_y; // [sp+Ch] [bp-Ch]@1
  int dir_z; // [sp+10h] [bp-8h]@1
  DAMAGE_TYPE pDamageType; // [sp+14h] [bp-4h]@14

  pMapInfo = &pMapStats->pInfos[pMapStats->GetMapInfo(pCurrentMapName)];
  dir_x = abs(pParty->vPosition.x - this->vPosition.x);
  dir_y = abs(pParty->vPosition.y - this->vPosition.y);
  dir_z = abs(pParty->vPosition.z + pParty->sEyelevel - this->vPosition.z);
  if ( dir_x < dir_y )
  {
    v7 = dir_x;
    dir_x = dir_y;
    dir_y = v7;
  }
  if ( dir_x < dir_z )
  {
    v7 = dir_x;
    dir_x = dir_z;
    dir_z = v7;
  }
  if ( dir_y < dir_z )
  {
    v7 = dir_z;
    dir_z = dir_y;
    dir_y = v7;
  }
  v10 = ((unsigned int)(11 * dir_y) >> 5) + (dir_z / 4) + dir_x;
  if ( (signed int)v10 <= 768 )
  {
    v11 = 5;
    if ( pMapInfo->Trap_D20 )
    {
      for ( uint i = 0; i < pMapInfo->Trap_D20; ++i )
        v11 += rand() % 20 + 1;
    }
    switch ( this->uType )
    {
      case 811:
        pDamageType = DMGT_FIRE;
        break;
      case 812:
        pDamageType = DMGT_ELECTR;
        break;
      case 813:
        pDamageType = DMGT_COLD;
        break;
      case 814:
        pDamageType = DMGT_BODY;
        break;
      default:
        return;
    }
    for ( uint i = 1; i <= 4; ++i )
    {
      if ( pPlayers[i]->CanAct() && (v13 = pPlayers[i]->GetPerception() + 20, rand() % v13 > 20) )
        pPlayers[i]->PlaySound(SPEECH_6, 0);
      else
        pPlayers[i]->ReceiveDamage(v11, pDamageType);
    }
  }
}

//----- (0042F933) --------------------------------------------------------
void SpriteObject::OnInteraction(unsigned int uLayingItemID)
{
  pSpriteObjects[uLayingItemID].uObjectDescID = 0;
  if ( pParty->bTurnBasedModeOn == 1 )
  {
    if (pSpriteObjects[uLayingItemID].uAttributes & 4 )
    {
      pSpriteObjects[uLayingItemID].uAttributes &= 0xFFFB;
      --pTurnEngine->pending_actions;
    }
  }
}


//----- (0042FA22) --------------------------------------------------------
void CompactLayingItemsList()
{
  int new_obj_pos = 0;

  for ( int i = 0; i < MAX_SPRITE_OBJECTS; ++i )
  {
    if ( pSpriteObjects[i].uObjectDescID )
    {
      if ( i != new_obj_pos )// 
      {
        memcpy(&pSpriteObjects[new_obj_pos], &pSpriteObjects[i],sizeof(SpriteObject));
        pSpriteObjects[i].uObjectDescID = 0;
      }
      new_obj_pos++;
    }
  }
  uNumSpriteObjects = new_obj_pos;
}
//----- (00408896) --------------------------------------------------------
void SpriteObject::InitializeSpriteObjects()
{
  for (uint i = 0; i < uNumSpriteObjects; ++i)
  {
    SpriteObject* item = &pSpriteObjects[i];

    if (item->uType && (item->uSoundID & 8 || pObjectList->pObjects[item->uType].uFlags & OBJECT_DESC_UNPICKABLE))
      SpriteObject::OnInteraction(i);
  }
}
//----- (0046BEF1) --------------------------------------------------------
void SpriteObject::_46BEF1_apply_spells_aoe()
{
  //SpriteObject *v1; // edi@1
  //Actor *v2; // esi@2
  //__int16 v3; // fps@4
  //unsigned __int8 v4; // c0@4
  //unsigned __int8 v5; // c3@4
  //signed int v6; // [sp+8h] [bp-4h]@1

  int v7,v9,v10,v11;
  __debugbreak();//Ritor1
  if ( (signed int)uNumActors > 0 )
  {
    for ( uint i = 0; i < uNumActors; ++i )
    {
      if ( pActors[i].CanAct() )
      {
        //UNDEF(v3);
		//.text:0046BF26                 movsx   eax, word ptr [esi-2]
		//.text:0046BF2A                 sub     eax, [edi+4]
		//.text:0046BF31                 mov     [ebp+var_8], eax
		//.text:0046BF37                 fild    [ebp+var_8]
		// v7 pushed to stack
		v7 = pActors[i].vPosition.x - this->vPosition.x;

		//.text:0046BF2D                 movsx   ecx, word ptr [esi+2]
		//v8 = pActors[i].vPosition.z;

		//.text:0046BF34                 movsx   eax, word ptr [esi]
		//.text:0046BF3A                 sub     eax, [edi+8]
		//.text:0046BF3D                 mov     [ebp+var_8], eax
		//.text:0046BF44                 fild    [ebp+var_8]
		// v9 pushed to stack
		v9 = pActors[i].vPosition.y - this->vPosition.y;

		//.text:0046BF40                 movsx   eax, word ptr [esi-6]
		//.text:0046BF47                 sar     eax, 1
		//.text:0046BF49                 add     eax, ecx
		//.text:0046BF4B                 sub     eax, [edi+0Ch]
		//.text:0046BF4E                 mov     [ebp+var_8], eax
		//.text:0046BF51                 fild    [ebp+var_8]
		//.text:0046BF58                 fld     st
		// v10 pushed to stack, two times
		v10 = pActors[i].uActorHeight / 2 + pActors[i].vPosition.z - this->vVelocity.y;

		//.text:0046BF54                 movsx   eax, word ptr [esi-8]
		//.text:0046BF5A                 add     eax, 100h
		//.text:0046BF63                 mov     ecx, eax
		//v11 = this->vVelocity.x;

		//.text:0046BF5F                 fmul    st, st(1)
		// stack: v10*v10, v10, v9, v7
		//.text:0046BF61                 fld     st(2)
		// stack: v7, v10*v10, v10, v9, v7
		

		//.text:0046BF65                 fmul    st, st(3)
		// stack: v7*v9, v10*v10, v10, v9, v7
		
		//.text:0046BF67                 imul    ecx, eax
		v11 = this->vVelocity.x * this->vVelocity.x;

		//.text:0046BF6A                 faddp   st(1), st
		// stack: v10*v10+v7*v9, v10, v9, v7
		//.text:0046BF6C                 fld     st(3)
		// stack: v7, v10*v10+v7*v9, v10, v9, v7
		//.text:0046BF6E                 fmul    st, st(4)
		// stack: v7*v7, v10*v10+v7*v9, v10, v9, v7
		//.text:0046BF70                 faddp   st(1), st
		// stack: v10*v10+v7*v9+v7*v7, v10, v9, v7
		
		//.text:0046BF72                 mov     [ebp+var_8], ecx
		//.text:0046BF75                 fild    [ebp+var_8]
		// v11 pushed to stack

		//.text:0046BF78                 fcompp
		// if ( v11 > v10*v10+v7*v9+v7*v7 )
		// stack: v10, v9, v7

		//.text:0046BF7A                 fstp    st
		// stack: v9, v7

		//.text:0046BF7C                 fnstsw  ax
		//.text:0046BF7E                 fstp    st
		// stack: v7

		//.text:0046BF80                 test    ah, 41h
		//.text:0046BF83                 fstp    st
		//.text:0046BF85                 jnz     short loc_46BFDD

		if ( v11 >= v7 * v7 + v9 * v9 + v10 * v10 )
        {
          if ( pActors[i].DoesDmgTypeDoDamage((DAMAGE_TYPE)0xAu) )
          {
            pActors[i].pActorBuffs[this->spell_id].Apply(pParty->uTimePlayed + (signed int)(signed __int64)((double)(this->spell_level << 7) * 0.033333335),
                   this->spell_skill, 4, 0, 0);
            HIWORD(pActors[i].uAttributes) |= 8;
          }
        }
      }
    }
  }
}


//----- (0042F7EB) --------------------------------------------------------
bool SpriteObject::sub_42F7EB_DropItemAt(unsigned int uSpriteID, int x, int y, int z, int a4, int count, int a7, unsigned __int16 attributes, ItemGen *a9)
{
  unsigned __int16 pObjectDescID; // ax@7
  SpriteObject pSpellObject; // [sp+Ch] [bp-78h]@1

  pSpellObject.stru_24.Reset();
  if ( a9 )
    memcpy(&pSpellObject.stru_24, a9, sizeof(pSpellObject.stru_24));
  pSpellObject.spell_skill = 0;
  pSpellObject.spell_level = 0;
  pSpellObject.spell_id = 0;
  pSpellObject.field_54 = 0;
  pSpellObject.uType = uSpriteID;
  pObjectDescID = 0;
  for ( uint i = 0; i < (signed int)pObjectList->uNumObjects; ++i )
  {
    if ( (short)uSpriteID == pObjectList->pObjects[i].uObjectID )
      pObjectDescID = i;
  }
  pSpellObject.uObjectDescID = pObjectDescID;
  pSpellObject.vPosition.x = x;
  pSpellObject.vPosition.y = y;
  pSpellObject.vPosition.z = z;
  pSpellObject.uSoundID = 0;
  pSpellObject.uAttributes = attributes;
  pSpellObject.uSectorID = pIndoor->GetSector(x, y, z);
  pSpellObject.uSpriteFrameID = 0;
  pSpellObject.spell_caster_pid = 0;
  pSpellObject.spell_target_pid = 0;
  if ( !(pSpellObject.uAttributes & 0x10) )
  {
    if ( pItemsTable->uAllItemsCount )
    {
      for ( uint i = 1; i < pItemsTable->uAllItemsCount; ++i )
      {
        if ( pItemsTable->pItems[i].uSpriteID == uSpriteID )
          pSpellObject.stru_24.uItemID = i;
      }
    }
  }
  if ( a7 )
  {
    if ( count > 0 )
    {
      for ( uint i = count; i; --i )
      {
        pSpellObject.uFacing = rand() % (signed int)stru_5C6E00->uIntegerDoublePi;
        pSpellObject.Create((signed __int16)pSpellObject.uFacing,
          ((signed int)stru_5C6E00->uIntegerHalfPi / 2) + (rand() % ((signed int)stru_5C6E00->uIntegerHalfPi / 2)), a4, 0);

      }
    }
  }
  else
  {
    pSpellObject.uFacing = 0;
    if ( count > 0 )
    {
      for ( uint i = count; i; --i )
      {
        pSpellObject.Create((signed __int16)pSpellObject.uFacing, stru_5C6E00->uIntegerHalfPi, a4, 0);
      }
    }
  }
  return true;
}

//----- (0042F960) --------------------------------------------------------
void SpriteObject::sub_42F960_create_object(int x, int y, int z)
{
  unsigned __int16 v7; // ax@5
  signed int v8; // eax@6
  signed int v9; // eax@7

  SpriteObject a1; // [sp+Ch] [bp-70h]@1
  //SpriteObject::SpriteObject(&a1);
  a1.stru_24.Reset();

  a1.spell_skill = 0;
  a1.spell_level = 0;
  a1.spell_id = 0;
  a1.field_54 = 0;
  a1.uType = 800;
  v7 = 0;
  for ( uint i = 0; i < (signed int)pObjectList->uNumObjects; ++i )
  {
    if ( a1.uType == pObjectList->pObjects[i].uObjectID  )
      v7 = i;
  }
  a1.uObjectDescID = v7;
  a1.vPosition.x = x;
  a1.vPosition.y = y;
  a1.vPosition.z = z;
  a1.uSoundID = 0;
  a1.uAttributes = 0;
  a1.uSectorID = pIndoor->GetSector(x, y, z);
  a1.uSpriteFrameID = 0;
  a1.spell_caster_pid = 0;
  a1.spell_target_pid = 0;
  v8 = a1.Create(0, 0, 0, 0);
  if ( v8 != -1 )
  {
    v9 = 8 * v8;
    LOBYTE(v9) = v9 | 2;
    pAudioPlayer->PlaySound((SoundID)(SOUND_GoldReceived|0x14), v9, 0, -1, 0, 0, 0, 0);
  }
}