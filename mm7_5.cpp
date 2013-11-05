#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "MapInfo.h"
#include "LightmapBuilder.h"
#include "mm7.h"
#include "GUIWindow.h"
#include "Party.h"
#include "Outdoor.h"
#include "LOD.h"
#include "Actor.h"
#include "Viewport.h"
#include "Math.h"
#include "SpriteObject.h"
#include "Time.h"
#include "stru298.h"
#include "Lights.h"
#include "Level/Decoration.h"

//----- (004356FF) --------------------------------------------------------
void back_to_game()
{
  dword_507BF0_is_there_popup_onscreen = 0;
  dword_4E455C = 1;

  extern int no_rightlick_in_inventory;
  no_rightlick_in_inventory = false;

  if ( pGUIWindow_ScrollWindow )
    free_book_subwindow();
  if ( !pCurrentScreen && !pGUIWindow_Settings )
    pEventTimer->Resume();
  viewparams->bRedrawGameUI = 1;
}

//----- (0043648F) --------------------------------------------------------
void Vec3_short__to_RenderVertexSoft(RenderVertexSoft *_this, Vec3_short_ *a2)
{
  _this->flt_2C = 0.0;

  _this->vWorldPosition.x = a2->x;
  _this->vWorldPosition.y = a2->y;
  _this->vWorldPosition.z = a2->z;
}

//----- (004369DB) --------------------------------------------------------
void Vec3_float_::Normalize()
{
  this->x = (1.0 / sqrt(this->x * this->x + this->y * this->y + this->z * this->z)) * this->x;
  this->y = (1.0 / sqrt(this->x * this->x + this->y * this->y + this->z * this->z)) * this->y;
  this->z = (1.0 / sqrt(this->x * this->x + this->y * this->y + this->z * this->z)) * this->z;
}

//----- (00438F8F) --------------------------------------------------------
void area_of_effect__damage_evaluate()
{
  int v0; // edx@1
  char *v1; // esi@2
  int v2; // ecx@3
  signed int v3; // eax@3
  SpriteObject *v4; // eax@4
  signed int v5; // eax@4
  signed int v6; // eax@6
  unsigned int v7; // edi@6
  int v8; // eax@6
  __int16 v9; // bx@8
  int v10; // edi@8
  Vec3_int_ v11; // ST04_12@9
  int v12; // ST0C_4@10
  Actor *v13; // edi@11
  int v14; // edx@15
  int v15; // edx@15
  int v16; // ebx@15
  signed int v17; // eax@15
  int v18; // edi@15
  int v19; // edi@15
  Vec3_int_ v20; // ST04_12@16
  Vec3_int_ *v21; // eax@17
  unsigned int v22; // edx@17
  int v23; // edx@18
  int v24; // eax@18
  Vec3_int_ v25; // ST04_12@19
  unsigned int v26; // ebx@20
  Player *v27; // edi@20
  Actor *v28; // edi@27
  int v29; // ebx@29
  int v30; // eax@29
  int v31; // edx@29
  int v32; // eax@29
  int v33; // ST24_4@29
  Vec3_int_ v34; // ST04_12@30
  Vec3_int_ *v35; // eax@31
  SpriteObject *v36; // [sp+0h] [bp-28h]@0
  signed int v37; // [sp+4h] [bp-24h]@5
  int v38; // [sp+4h] [bp-24h]@15
  signed int v39; // [sp+8h] [bp-20h]@3
  int v40; // [sp+8h] [bp-20h]@15
  int v41; // [sp+Ch] [bp-1Ch]@5
  int v42; // [sp+Ch] [bp-1Ch]@15
  int v43; // [sp+10h] [bp-18h]@1
  int v44; // [sp+14h] [bp-14h]@15
  unsigned int uActorID; // [sp+18h] [bp-10h]@6
  Vec3_int_ *pVelocity; // [sp+1Ch] [bp-Ch]@2
  int a1; // [sp+20h] [bp-8h]@8
  int v48; // [sp+24h] [bp-4h]@8

  v0 = 0;
  v43 = 0;
  if ( stru_50FE08.count > 0 )
  {
    pVelocity = &stru_50FE08.vec_4B4;
    v1 = (char *)stru_50FE08.pZs;
    do
    {
      v2 = PID_TYPE(*((short *)v1 - 300));
      v3 = PID_ID((signed int)*((short *)v1 - 300));
      v39 = PID_ID((signed int)*((short *)v1 - 300));
      if ( v2 == 2 )
      {
        v4 = &pSpriteObjects[v3];
        v36 = v4;
        v5 = v4->spell_caster_pid;
        v2 = PID_TYPE(v5);
        v3 = PID_ID(v5);
      }
      v41 = v2;
      v37 = v3;
      if ( stru_50FE08.field_3EC[v0] & 1 )
      {
        v6 = ai_near_actors_targets_pid[v3];
        v7 = PID_ID(v6);
        v8 = PID_TYPE(v6) - 3;
        uActorID = v7;
        if ( v8 )
        {
          if ( v8 == 1 )
          {
            v9 = *(short *)v1;
            v10 = pParty->vPosition.y - *((short *)v1 - 100);
            a1 = pParty->vPosition.x - *((short *)v1 - 200);
            v48 = v10;
            if ( a1 * a1
               + v10 * v10
               + ((signed int)(pParty->vPosition.z + pParty->uPartyHeight) >> (1 - v9))
               * ((signed int)(pParty->vPosition.z + pParty->uPartyHeight) >> (1 - v9)) < (unsigned int)((*((short *)v1 + 100) + 32) * (*((short *)v1 + 100) + 32)) )
            {
              v11.z = *(short *)v1;
              v11.y = *((short *)v1 - 100);
              v11.x = *((short *)v1 - 200);
              if ( sub_407A1C(pParty->vPosition.x, pParty->vPosition.y, pParty->vPosition.z + pParty->sEyelevel, v11) )
              {
                v12 = stru_50C198.which_player_to_attack(&pActors[v39]);
                DamagePlayerFromMonster(*((short *)v1 - 300), stru_50FE08.field_450[v43], pVelocity, v12);
              }
            }
          }
        }
        else
        {
          v13 = &pActors[v7];
          if ( SHIDWORD(v13->pActorBuffs[ACTOR_BUFF_PARALYZED].uExpireTime) > 0
            || SHIDWORD(v13->pActorBuffs[ACTOR_BUFF_PARALYZED].uExpireTime) >= 0 && LODWORD(v13->pActorBuffs[ACTOR_BUFF_PARALYZED].uExpireTime)
            || v13->CanAct() )
          {
            v14 = v13->vPosition.y;
            v40 = v13->vPosition.x;
            v38 = v14;
            v15 = v14 - *((short *)v1 - 100);
            v16 = *(short *)v1;
            v42 = v13->vPosition.z;
            a1 = v40 - *((short *)v1 - 200);
            v17 = v13->uActorHeight;
            v18 = v13->uActorRadius;
            v44 = v42;
            v19 = *((short *)v1 + 100) + v18;
            v48 = v15;
            if ( a1 * a1 + v15 * v15 + (v42 + (v17 >> 1) - v16) * (v42 + (v17 >> 1) - v16) < (unsigned int)(v19 * v19) )
            {
              v20.z = *(short *)v1;
              v20.y = *((short *)v1 - 100);
              v20.x = *((short *)v1 - 200);
              if ( sub_407A1C(v40, v38, v42 + 50, v20) )
              {
                Vec3_int_::Normalize(&a1, &v48, &v44);
                v21 = pVelocity;
                v22 = uActorID;
                pVelocity->x = a1;
                v21->y = v48;
                v21->z = v44;
                sub_43B1B0(*((short *)v1 - 300), v22, v21, stru_50FE08.field_450[v43]);
              }
            }
          }
        }
      }
      else
      {
        v23 = pParty->vPosition.y - *((short *)v1 - 100);
        v24 = ((signed int)pParty->uPartyHeight >> 1) - *(short *)v1;
        a1 = pParty->vPosition.x - *((short *)v1 - 200);
        v48 = v23;
        if ( a1 * a1 + v23 * v23 + (pParty->vPosition.z + v24) * (pParty->vPosition.z + v24) < (unsigned int)((*((short *)v1 + 100) + 32) * (*((short *)v1 + 100) + 32)) )
        {
          v25.z = *(short *)v1;
          v25.y = *((short *)v1 - 100);
          v25.x = *((short *)v1 - 200);
          if ( sub_407A1C(pParty->vPosition.x, pParty->vPosition.y, pParty->vPosition.z + pParty->sEyelevel, v25) )
          {
            v26 = 0;
            v27 = pParty->pPlayers.data();//[0].pConditions[15];
            do
            {
				if ( !(HIDWORD(v27->pConditions[14]) | LODWORD(v27->pConditions[14])) && !v27->pConditions[15] && !v27->pConditions[16] )
                DamagePlayerFromMonster(*((short *)v1 - 300), stru_50FE08.field_450[v43], pVelocity, v26);
              ++v27;
              ++v26;
            }
			while ( v27 <= &pParty->pPlayers[3] );
          }
        }
        uActorID = 0;
        if ( (signed int)uNumActors > 0 )
        {
          v28 = pActors.data();//[0].vPosition.z;
          do
          {
            if ( v28->CanAct() )
            {
              v29 = *(short *)v1;
			  v30 = v28->vPosition.y - *((short *)v1 - 100);
			  a1 = v28->vPosition.x - *((short *)v1 - 200);
			  v31 = v28->vPosition.z;
              v48 = v30;
              v44 = v31;
			  v32 = (v28->uActorHeight >> 1) - v29;
			  v33 = v28->uActorRadius + *((short *)v1 + 100);
              if ( a1 * a1 + v48 * v48 + (v31 + v32) * (v31 + v32) < (unsigned int)(v33 * v33) )
              {
                v34.z = *(short *)v1;
                v34.y = *((short *)v1 - 100);
                v34.x = *((short *)v1 - 200);
				if ( sub_407A1C(v28->vPosition.x, v28->vPosition.y, v28->vPosition.z + 50, v34) )
                {
                  Vec3_int_::Normalize(&a1, &v48, &v44);
                  v35 = pVelocity;
                  pVelocity->x = a1;
                  v35->y = v48;
                  v35->z = v44;
                  switch ( v41 )
                  {
                    case OBJECT_Player:
                      DamageMonsterFromParty(*((short *)v1 - 300), uActorID, v35);
                      break;
                    case OBJECT_Actor:
                      if ( v36 && pActors[v37].GetActorsRelation(v28) )
                        sub_43B1B0(*((short *)v1 - 300), uActorID, pVelocity, v36->field_61);
                      break;
                    case OBJECT_Item:
                      sub_43B057(*((short *)v1 - 300), uActorID, v35);
                      break;
                  }
                }
              }
            }
            ++uActorID;
            ++v28;
          }
          while ( (signed int)uActorID < (signed int)uNumActors );
        }
      }
      ++pVelocity;
      v0 = v43 + 1;
      v1 += 2;
      ++v43;
    }
    while ( v43 < stru_50FE08.count );
  }
  stru_50FE08.count = 0;
}


//----- (0043A97E) --------------------------------------------------------
void __fastcall sub_43A97E(unsigned int uLayingItemID, signed int a2)
{
  if (PID_TYPE(a2) == OBJECT_Player)
  {
    layingitem_vel_50FDFC.x = pSpriteObjects[uLayingItemID].vVelocity.x;
    layingitem_vel_50FDFC.y = pSpriteObjects[uLayingItemID].vVelocity.y;
    layingitem_vel_50FDFC.z = pSpriteObjects[uLayingItemID].vVelocity.z;

    Vec3_int_::Normalize(&layingitem_vel_50FDFC.x, &layingitem_vel_50FDFC.y, &layingitem_vel_50FDFC.z);
    DamagePlayerFromMonster(PID(OBJECT_Item, uLayingItemID), pSpriteObjects[uLayingItemID].field_61, &layingitem_vel_50FDFC, 0xFFFFFFFFu);
  }
  else if (PID_TYPE(a2) == OBJECT_Actor)
  {
    layingitem_vel_50FDFC.x = pSpriteObjects[uLayingItemID].vVelocity.x;
    layingitem_vel_50FDFC.y = pSpriteObjects[uLayingItemID].vVelocity.y;
    layingitem_vel_50FDFC.z = pSpriteObjects[uLayingItemID].vVelocity.z;

    Vec3_int_::Normalize(&layingitem_vel_50FDFC.x, &layingitem_vel_50FDFC.y, &layingitem_vel_50FDFC.z);
    switch (PID_TYPE(pSpriteObjects[uLayingItemID].spell_caster_pid))
    {
      case OBJECT_Actor:
        sub_43B1B0(PID(OBJECT_Item, uLayingItemID), PID_ID(a2), &layingitem_vel_50FDFC, pSpriteObjects[uLayingItemID].field_61);
        break;
      case OBJECT_Player:
        DamageMonsterFromParty(PID(OBJECT_Item, uLayingItemID), PID_ID(a2), &layingitem_vel_50FDFC);
        break;
      case OBJECT_Item:
        sub_43B057(PID(OBJECT_Item, uLayingItemID), PID_ID(a2), &layingitem_vel_50FDFC);
        break;
    }
  }
}

//----- (0043AE12) --------------------------------------------------------
double __fastcall sub_43AE12(signed int a1)
{
  signed int v1; // ST00_4@1
  signed int v2; // ecx@1
  double v3; // st7@1
  double result; // st7@6

  v1 = a1;
  v2 = 0;
  v3 = (double)v1;
  do
  {
    if ( v3 < flt_4E4A80[v2 + 5] )
      break;
    ++v2;
  }
  while ( v2 < 5 );
  if ( v2 <= 0 || v2 >= 5 )
  {
    if ( v2 )
      result = flt_4E4A80[4];
    else
      result = flt_4E4A80[0];
  }
  else
  {
    result = (flt_4E4A80[v2] - flt_4E4A80[v2 - 1]) * (v3 - flt_4E4A80[v2 + 4]) / (flt_4E4A80[v2 + 5] - flt_4E4A80[v2 + 4]) + flt_4E4A80[v2];
  }
  return result;
}

//----- (0043B057) --------------------------------------------------------
void __fastcall sub_43B057(unsigned int uObjID, unsigned int uActorID, Vec3_int_ *pVelocity)
{
  Actor *pActor; // esi@1
  SpriteObject *v4; // eax@3
  int v5; // ecx@3
  int v6; // eax@4
  int v7; // edi@4
  unsigned int uActorIDa; // [sp+8h] [bp-8h]@1
  unsigned int a2; // [sp+Ch] [bp-4h]@1
  int a2a; // [sp+Ch] [bp-4h]@8

  uActorIDa = uActorID;
  pActor = &pActors[uActorID];
  a2 = uObjID;
  if ( !pActors[uActorID].IsNotAlive() )
  {
    if ( PID_TYPE(a2) == OBJECT_Item)
    {
      v4 = &pSpriteObjects[PID_ID(a2)];
      v5 = v4->spell_id;
      if ( v5 )
      {
        v6 = _43AFE3_calc_spell_damage(v5, v4->spell_level, v4->spell_skill, pActor->sCurrentHP);
        v7 = stru_50C198.CalcMagicalDamageToActor(pActor, 0, v6);
        pActor->sCurrentHP -= v7;
        if ( v7 )
        {
          if ( pActor->sCurrentHP > 0 )
            Actor::AI_Stun(uActorIDa, a2, 0);
          else
            Actor::Die(uActorIDa);
          a2a = 20 * v7 / (signed int)pActor->pMonsterInfo.uHP;
          if ( 20 * v7 / (signed int)pActor->pMonsterInfo.uHP > 10 )
            a2a = 10;
          if ( !MonsterStats::BelongsToSupertype(pActor->pMonsterInfo.uID, MONSTER_SUPERTYPE_TREANT) )
          {
            pVelocity->x = (unsigned __int64)(a2a * (signed __int64)pVelocity->x) >> 16;
            pVelocity->y = (unsigned __int64)(a2a * (signed __int64)pVelocity->y) >> 16;
            pVelocity->z = (unsigned __int64)(a2a * (signed __int64)pVelocity->z) >> 16;
            pActor->vVelocity.x = 50 * LOWORD(pVelocity->x);
            pActor->vVelocity.y = 50 * LOWORD(pVelocity->y);
            pActor->vVelocity.z = 50 * LOWORD(pVelocity->z);
          }
          Actor::AddBloodsplatOnDamageOverlay(uActorIDa, 1, v7);
        }
        else
        {
          Actor::AI_Stun(uActorIDa, a2, 0);
        }
      }
    }
  }
}

//----- (0043B1B0) --------------------------------------------------------
void sub_43B1B0(signed int a1, unsigned int a2, Vec3_int_ *pVelocity, signed int a4)
{
  int v4; // ebx@1
  SpriteObject *v5; // eax@2
  int v6; // eax@3
  Actor *v7; // esi@4
  Actor *v8; // edi@4
  char v9; // zf@5
  __int64 v10; // qax@8
  signed __int16 v11; // cx@9
  signed int v12; // ecx@20
  int v13; // ecx@22
  int v14; // edi@30
  unsigned int uActorID; // [sp+Ch] [bp-8h]@1
  signed int v17; // [sp+10h] [bp-4h]@1
  int v18; // [sp+20h] [bp+Ch]@34

  v4 = 0;
  uActorID = a2;
  v17 = a1;
  if ( PID_TYPE(a1) == OBJECT_Item)
  {
    v5 = &pSpriteObjects[PID_ID(a1)];
    v4 = v5->field_60_distance_related_prolly_lod;
    v17 = v5->spell_caster_pid;
  }
  LOWORD(v6) = PID_TYPE(v17);
  if ( v6 == OBJECT_Actor)
  {
    v7 = &pActors[a2];
    v8 = &pActors[PID_ID(v17)];
    v6 = pActors[a2].IsNotAlive();
    if ( !v6 )
    {
      v9 = v7->uAIState == 7;
      v7->uLastCharacterIDToHit = v17;
      if ( v9 )
        BYTE2(v7->uAttributes) |= 2u;
      v6 = v8->_4273BB_DoesHitOtherActor(v7, v4, 0);
      if ( v6 )
      {
        v10 = (unsigned int)v8->_43B3E0_CalcDamage(a4);
        if ( (signed __int64)v8->pActorBuffs[ACTOR_BUFF_SHRINK].uExpireTime > 0 )
        {
          v11 = v8->pActorBuffs[ACTOR_BUFF_SHRINK].uPower;
          if ( v11 )
            v10 = (signed int)v10 / (unsigned __int16)v11;
        }
        if ( SHIDWORD(v7->pActorBuffs[ACTOR_BUFF_STONED].uExpireTime) >= SHIDWORD(v10)
          && (SHIDWORD(v7->pActorBuffs[ACTOR_BUFF_STONED].uExpireTime) > SHIDWORD(v10)
           || LODWORD(v7->pActorBuffs[ACTOR_BUFF_STONED].uExpireTime) > HIDWORD(v10)) )
          LODWORD(v10) = 0;
        if ( a4 != HIDWORD(v10) )
        {
          if ( a4 - HIDWORD(v10) == 1 )
          {
            v12 = v8->pMonsterInfo.uAttack2Type;
            if ( SHIDWORD(v7->pActorBuffs[ACTOR_BUFF_SHIELD].uExpireTime) >= SHIDWORD(v10)
              && (SHIDWORD(v7->pActorBuffs[ACTOR_BUFF_SHIELD].uExpireTime) > SHIDWORD(v10)
               || LODWORD(v7->pActorBuffs[ACTOR_BUFF_SHIELD].uExpireTime) > HIDWORD(v10)) )
              LODWORD(v10) = (signed int)v10 >> 1;
            goto LABEL_30;
          }
          if ( a4 - HIDWORD(v10) == 2 )
          {
            v13 = v7->pMonsterInfo.uSpell1ID;
          }
          else
          {
            if ( a4 - HIDWORD(v10) != 3 )
            {
              if ( a4 - HIDWORD(v10) == 4 )
                v12 = v8->pMonsterInfo.field_3C_some_special_attack;
              else
                v12 = 4;
LABEL_30:
              v14 = stru_50C198.CalcMagicalDamageToActor(v7, v12, v10);
              v7->sCurrentHP -= v14;
              if ( v14 )
              {
                if ( v7->sCurrentHP > 0 )
                  Actor::AI_Stun(uActorID, v17, 0);
                else
                  Actor::Die(uActorID);
                Actor::AggroSurroundingPeasants(uActorID, 0);
                v18 = 20 * v14 / (signed int)v7->pMonsterInfo.uHP;
                if ( 20 * v14 / (signed int)v7->pMonsterInfo.uHP > 10 )
                  v18 = 10;
                if ( !MonsterStats::BelongsToSupertype(v7->pMonsterInfo.uID, MONSTER_SUPERTYPE_TREANT) )
                {
                  pVelocity->x = (unsigned __int64)(v18 * (signed __int64)pVelocity->x) >> 16;
                  pVelocity->y = (unsigned __int64)(v18 * (signed __int64)pVelocity->y) >> 16;
                  pVelocity->z = (unsigned __int64)(v18 * (signed __int64)pVelocity->z) >> 16;
                  v7->vVelocity.x = 50 * LOWORD(pVelocity->x);
                  v7->vVelocity.y = 50 * LOWORD(pVelocity->y);
                  v7->vVelocity.z = 50 * LOWORD(pVelocity->z);
                }
                Actor::AddBloodsplatOnDamageOverlay(uActorID, 1, v14);
              }
              else
              {
                Actor::AI_Stun(uActorID, v17, 0);
              }
              return;
            }
            v13 = v7->pMonsterInfo.uSpell2ID;
          }
          v12 = LOBYTE(pSpellStats->pInfos[v13].uSchool);
          goto LABEL_30;
        }
        v12 = v8->pMonsterInfo.uAttack1Type;
        goto LABEL_30;
      }
    }
  }
}


//----- (0043F515) --------------------------------------------------------
void FindBillboardsLightLevels_BLV()
{
  for (uint i = 0; i < uNumBillboardsToDraw; ++i)
  {
    RenderBillboard* p = &pBillboardRenderList[i];

    if (p->field_1E & 2 || uCurrentlyLoadedLevelType == LEVEL_Indoor && !p->uIndoorSectorID)
      p->dimming_level = 0;
    else
      p->dimming_level = _43F55F_get_billboard_light_level(p, -1);
  }
}

//----- (0043F55F) --------------------------------------------------------
int __fastcall _43F55F_get_billboard_light_level(RenderBillboard *a1, int uBaseLightLevel)
{
  signed int v3; // ecx@2

  if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
  {
    v3 = pIndoor->pSectors[a1->uIndoorSectorID].uMinAmbientLightLevel;
  }
  else
  {
    if ( uBaseLightLevel == -1 )
      v3 = a1->dimming_level;
    else
      v3 = uBaseLightLevel;
  }
  return _43F5C8_get_point_light_level_with_respect_to_lights(v3, a1->uIndoorSectorID, a1->world_x, a1->world_y, a1->world_z);
}

//----- (0043F5C8) --------------------------------------------------------
int __fastcall _43F5C8_get_point_light_level_with_respect_to_lights(unsigned int uBaseLightLevel, int uSectorID, float x, float y, float z)
{
  int v5; // esi@1
  signed int v6; // edi@1
  int v8; // eax@6
  int v9; // ebx@6
  unsigned int v10; // ecx@6
  unsigned int v11; // edx@9
  unsigned int v12; // edx@11
  signed int v13; // ecx@12
  BLVLightMM7 *v16; // esi@20
  int v17; // ebx@21
  int v18; // eax@24
  int v19; // ebx@24
  unsigned int v20; // ecx@24
  int v21; // edx@25
  unsigned int v22; // edx@27
  unsigned int v23; // edx@29
  signed int v24; // ecx@30
  int v26; // ebx@35
  int v27; // eax@38
  int v28; // ebx@38
  unsigned int v29; // ecx@38
  int v30; // edx@39
  unsigned int v31; // edx@41
  unsigned int v32; // edx@43
  signed int v33; // ecx@44
  int v37; // [sp+Ch] [bp-18h]@37
  int v38; // [sp+10h] [bp-14h]@5
  int v39; // [sp+10h] [bp-14h]@23
  int v40; // [sp+10h] [bp-14h]@36
  int v42; // [sp+14h] [bp-10h]@22
  unsigned int v43; // [sp+18h] [bp-Ch]@12
  unsigned int v44; // [sp+18h] [bp-Ch]@30
  unsigned int v45; // [sp+18h] [bp-Ch]@44

  v6 = uBaseLightLevel;
  for (uint i = 0; i < pMobileLightsStack->uNumLightsActive; ++i)
  {
    MobileLight* p = &pMobileLightsStack->pLights[i];

      float distX = abs(p->vPosition.x - x);
      if ( distX <= p->uRadius)
      {
        float distY = abs(p->vPosition.y - y);
        if ( distY <= p->uRadius)
        {
          float distZ = abs(p->vPosition.z - z);
          if ( distZ <= p->uRadius)
          {
            v8 = distX;
            v9 = distY;
            v10 = distZ;
            if (distX < distY)
            {
              v8 = distY;
              v9 = distX;
            }
            if ( v8 < distZ )
            {
              v11 = v8;
              v8 = distZ;
              v10 = v11;
            }
            if ( v9 < (signed int)v10 )
            {
              v12 = v10;
              v10 = v9;
              v9 = v12;
            }
            v43 = ((unsigned int)(11 * v9) / 32) + (v10 / 4) + v8;
            v13 = p->uRadius;
            if ( (signed int)v43 < v13 )
              v6 += ((unsigned __int64)(30i64 * (signed int)(v43 << 16) / v13) >> 16) - 30;
          }
        }
      }
  }

  if ( uCurrentlyLoadedLevelType == LEVEL_Indoor)
  {
    BLVSector* pSector = &pIndoor->pSectors[uSectorID];

    for (uint i = 0; i < pSector->uNumLights; ++i)
    {
        v16 = pIndoor->pLights + pSector->pLights[i];
        if (~v16->uAtributes & 8)
        {
          v17 = abs(v16->vPosition.x - x);
          if ( v17 <= v16->uRadius )
          {
            v42 = abs(v16->vPosition.y - y);
            if ( v42 <= v16->uRadius )
            {
              v39 = abs(v16->vPosition.z - z);
              if ( v39 <= v16->uRadius )
              {
                v18 = v17;
                v19 = v42;
                v20 = v39;
                if ( v18 < v42 )
                {
                  v21 = v18;
                  v18 = v42;
                  v19 = v21;
                }
                if ( v18 < v39 )
                {
                  v22 = v18;
                  v18 = v39;
                  v20 = v22;
                }
                if ( v19 < (signed int)v20 )
                {
                  v23 = v20;
                  v20 = v19;
                  v19 = v23;
                }
                v44 = ((unsigned int)(11 * v19) >> 5) + (v20 >> 2) + v18;
                v24 = v16->uRadius;
                if ( (signed int)v44 < v24 )
                  v6 += ((unsigned __int64)(30i64 * (signed int)(v44 << 16) / v24) >> 16) - 30;
              }
            }
          }
        }
    }
  }

  for (uint i = 0; i < pStationaryLightsStack->uNumLightsActive; ++i)
  {
    StationaryLight* p = &pStationaryLightsStack->pLights[i];
      v26 = abs(p->vPosition.x - x);
      if ( v26 <= p->uRadius)
      {
        v40 = abs(p->vPosition.y - y);
        if ( v40 <= p->uRadius)
        {
          v37 = abs(p->vPosition.z - z);
          if ( v37 <= p->uRadius)
          {
            v27 = v26;
            v28 = v40;
            v29 = v37;
            if ( v27 < v40 )
            {
              v30 = v27;
              v27 = v40;
              v28 = v30;
            }
            if ( v27 < v37 )
            {
              v31 = v27;
              v27 = v37;
              v29 = v31;
            }
            if ( v28 < (signed int)v29 )
            {
              v32 = v29;
              v29 = v28;
              v28 = v32;
            }
            v45 = ((unsigned int)(11 * v28) >> 5) + (v29 >> 2) + v27;
            v33 = p->uRadius;
            if ( (signed int)v45 < v33 )
              v6 += ((unsigned __int64)(30i64 * (signed int)(v45 << 16) / v33) >> 16) - 30;
          }
        }
      }
  }

  if ( v6 <= 31 )
  {
    if ( v6 < 0 )
      v6 = 0;
  }
  else
  {
    v6 = 31;
  }
  return v6;
}


//----- (004070EF) --------------------------------------------------------
bool __fastcall sub_4070EF_prolly_detect_player(unsigned int uObjID, unsigned int uObj2ID)
{
  signed int v2; // eax@1
  int v5; // ecx@2
  int obj1_sector; // eax@4
  float v8; // ST24_4@5
  double v9; // ST18_8@5
  int v11; // ecx@6
  signed int v12; // eax@7
  int v13; // esi@7
  int v14; // esi@8
  int v15; // esi@9
  int obj2_z; // edi@11
  int obj2_x; // esi@11
  int obj2_sector; // eax@13
  float v20; // ST24_4@14
  double v21; // ST18_8@14
  int dist_x; // ebx@16
  signed int dist_3d; // ecx@16
  int v25; // eax@18
  BLVFace *v29; // ebx@32
  Vec3_short_ *v30; // esi@32
  int v31; // eax@32
  int v32; // ST50_4@44
  int v33; // ST54_4@44
  int v34; // eax@44
  signed int v38; // esi@45
  signed __int64 v40; // qtt@50
  __int16 next_sector; // bx@58
  int v43; // [sp-8h] [bp-70h]@11
  int v44; // [sp-4h] [bp-6Ch]@11
  int v47; // [sp+18h] [bp-50h]@20
  int v48; // [sp+1Ch] [bp-4Ch]@20
  int v49; // [sp+20h] [bp-48h]@20
  int dist_z; // [sp+24h] [bp-44h]@16
  signed int higher_z; // [sp+24h] [bp-44h]@27
  signed int lower_z; // [sp+28h] [bp-40h]@26
  signed int higher_y; // [sp+2Ch] [bp-3Ch]@23
  signed int lower_y; // [sp+30h] [bp-38h]@22
  signed int higher_x; // [sp+34h] [bp-34h]@21
  signed int lower_x; // [sp+38h] [bp-30h]@20
  signed int sectors_visited; // [sp+3Ch] [bp-2Ch]@28
  int v58; // [sp+44h] [bp-24h]@50
  int v59; // [sp+48h] [bp-20h]@44
  int obj2_y; // [sp+50h] [bp-18h]@11
  int obj1_x; // [sp+58h] [bp-10h]@4
  int obj1_y; // [sp+5Ch] [bp-Ch]@4
  int obj1_z; // [sp+60h] [bp-8h]@4
  int current_sector; // [sp+64h] [bp-4h]@7
  int dist_y;
  int v70;

  v2 = PID_ID(uObjID);
  switch( PID_TYPE(uObjID) )
  {
	case OBJECT_Decoration:
      obj1_x = pLevelDecorations[v2].vPosition.x;
      obj1_y = pLevelDecorations[v2].vPosition.y;
      obj1_z = pLevelDecorations[v2].vPosition.z;
      obj1_sector = pIndoor->GetSector(obj1_x, obj1_y, obj1_z);
	  break;
	case OBJECT_Actor:
      obj1_x = pActors[v2].vPosition.x;
      obj1_y = pActors[v2].vPosition.y;
      v8 = (double)pActors[v2].uActorHeight * 0.69999999;
      //v9 = v8 + 6.7553994e15;
      //obj1_z = LODWORD(v9) + pActors[v2].vPosition.z;
	  obj1_z = (int)v8 + pActors[v2].vPosition.z;
      obj1_sector = pActors[v2].uSectorID;
	  break;
	case OBJECT_Item:
      obj1_x = pSpriteObjects[v2].vPosition.x;
      obj1_y = pSpriteObjects[v2].vPosition.y;
      obj1_z = pSpriteObjects[v2].vPosition.z;
      obj1_sector = pSpriteObjects[v2].uSectorID;
	  break;
	default:
	  return 0;
  }
  v12 = PID_ID(uObj2ID);
  switch( PID_TYPE(uObj2ID) )
  {
    case OBJECT_Decoration:
      obj2_z = pLevelDecorations[v12].vPosition.z;
      obj2_x = pLevelDecorations[v12].vPosition.x;
      obj2_y = pLevelDecorations[v12].vPosition.y;
	  obj2_sector = pIndoor->GetSector(obj2_x, obj2_y, obj2_z);
	  break;
	case OBJECT_Player:
      obj2_x = pParty->vPosition.x;
      obj2_z = pParty->sEyelevel + pParty->vPosition.z;
      obj2_y = pParty->vPosition.y;
	  obj2_sector = pIndoor->GetSector(obj2_x, obj2_y, obj2_z);
      break;
	case OBJECT_Actor:
      obj2_y = pActors[v12].vPosition.y;
      obj2_x = pActors[v12].vPosition.x;
      v20 = (double)pActors[v12].uActorHeight * 0.69999999;
      //v21 = v20 + 6.7553994e15;
      //obj2_z = LODWORD(v21) + pActors[v12].vPosition.z;
	  obj2_z = (int)v20 + pActors[v12].vPosition.z;
      obj2_sector = pActors[v12].uSectorID;
	  break;
	case OBJECT_Item:
      obj2_x = pSpriteObjects[v12].vPosition.x;
      obj2_z = pSpriteObjects[v12].vPosition.z;
      obj2_y = pSpriteObjects[v12].vPosition.y;
      obj2_sector = pSpriteObjects[v12].uSectorID;
	  break;
	default:
	  return 0;
  }
  dist_x = obj2_x - obj1_x;
  dist_z = obj2_z - obj1_z;
  dist_y = obj2_y - obj1_y;
  dist_3d = integer_sqrt(dist_x * dist_x + dist_y * dist_y + dist_z * dist_z);
  //range check
  if ( dist_3d > 5120 )
    return 0;
  if ( uCurrentlyLoadedLevelType == LEVEL_Outdoor)
    return 1;
  v25 = 65536;
  if ( dist_3d )
    v25 = 65536 / dist_3d;
  v49 = dist_x * v25;
  v47 = dist_z * v25;
  v48 = dist_y * v25;
  if ( obj1_x < obj2_x )
  {
	lower_x = obj1_x;
    higher_x = obj2_x;
  }
  else
  {
    lower_x = obj2_x;
    higher_x = obj1_x;
  }
  if ( obj1_y < obj2_y )
  {
	lower_y = obj1_y;
    higher_y = obj2_y;
  }
  else
  {
    lower_y = obj2_y;
    higher_y = obj1_y;
  }
  if ( obj1_z < obj2_z )
  {
	lower_z = obj1_z;
    higher_z = obj2_z;
  }
  else
  {
    lower_z = obj2_z;
    higher_z = obj1_z;
  }
  sectors_visited = 0;
  //monster in same sector with player
  if ( obj1_sector == obj2_sector )
      return 1;
  //search starts from monster
  current_sector = obj1_sector;
  for( int current_portal = 0; current_portal < pIndoor->pSectors[current_sector].uNumPortals; current_portal++ )
  {
	v29 = &pIndoor->pFaces[pIndoor->pSectors[current_sector].pPortals[current_portal]];
	v30 = &pIndoor->pVertices[*v29->pVertexIDs];
	v31 = v29->pFacePlane_old.vNormal.z * (v30->z - obj1_z)
		+ v29->pFacePlane_old.vNormal.y * (v30->y - obj1_y)
		+ v29->pFacePlane_old.vNormal.x * (v30->x - obj1_x);

	if ( current_sector != v29->uSectorID )
		v31 = -v31;

	if ( v31 >= 0 && v30->x != obj1_x && v30->y != obj1_y && v30->z != obj1_z)
		continue;

	if(	lower_x > v29->pBounding.x2
		|| higher_x < v29->pBounding.x1
		|| lower_y > v29->pBounding.y2
		|| higher_y < v29->pBounding.y1
		|| lower_z > v29->pBounding.z2
		|| higher_z < v29->pBounding.z1 )
	{
		continue;
	}
	  
	v32 = fixpoint_mul(v29->pFacePlane_old.vNormal.x,v49);
	v33 = fixpoint_mul(v29->pFacePlane_old.vNormal.z,v47);
	v34 = fixpoint_mul(v29->pFacePlane_old.vNormal.y,v48);
	v59 = v32 + v33 + v34;
	if ( v59 )
	{
		v70 = v29->pFacePlane_old.dist 
			+ obj1_z * v29->pFacePlane_old.vNormal.z 
			+ obj1_x * v29->pFacePlane_old.vNormal.x 
			+ obj1_y * v29->pFacePlane_old.vNormal.y;
		v38 = -v70;

		// if ( v59 <= 0 ^ v70 <= 0 )
		
		/* TEMPORARY
		if ( v59 <= 0 && v70 <= 0 )
		{
			continue;
		}
		if ( !(v59 <= 0 && v70 <= 0) )
		{
			continue;
		}
		*/

		if( abs(v38) >> 14 > abs(v59) )
			continue;

		v58 = fixpoint_div(v38,v59);

		if( v58 < 0 )
		{
			continue;
		}

		if(!sub_4075DB(
				obj1_x + ((fixpoint_mul(v49,v58) + 32768) >> 16),
				obj1_y + ((fixpoint_mul(v48,v58) + 32768) >> 16),
				obj1_z + ((fixpoint_mul(v47,v58) + 32768) >> 16),
				v29) )
		{
			continue;
		}

		//if there is no next sector turn back
		if ( v29->uSectorID == current_sector )
			next_sector = v29->uBackSectorID;
		else
			next_sector = v29->uSectorID;

		//no more portals, quit
		if ( next_sector == current_sector )
		{
			break;
		}

		++sectors_visited;
		current_sector = next_sector;

		//found player, quit
		if ( next_sector == obj2_sector )
			return 1;

		current_sector = next_sector;

		//did we hit limit for portals?
		//does the next room have portals?
		if ( sectors_visited < 30 && pIndoor->pSectors[current_sector].uNumPortals > 0)
		{
				current_portal=-1;
				continue;
		}
		else
			break;
	}
  }
  //did we stop in the sector where player is?
  if ( current_sector != obj2_sector )
    return 0;
  return 1;
}

//----- (004075DB) --------------------------------------------------------
bool __fastcall sub_4075DB(int x, int y, int z, BLVFace *a4)
{
  unsigned int v5; // esi@1
  char v7; // zf@2
  int v8; // edi@2
  Vec3_short_ *v9; // ecx@3
  int v10; // edx@4
  Vec3_short_ *v12; // ecx@8
  int v13; // edx@9
  Vec3_short_ *v15; // ecx@12
  int v16; // edx@13
  bool v18; // esi@14
  int v19; // ecx@14
  int v20; // ecx@16
  int v21; // edx@16
  int v22; // eax@17
  signed int v23; // ebx@18
  int v24; // esi@20
  signed int v25; // eax@22
  signed __int64 v26; // qtt@22
  bool result; // eax@25
  int v29; // [sp+10h] [bp-Ch]@14
  signed int a3a; // [sp+24h] [bp+8h]@14
  int a4a; // [sp+28h] [bp+Ch]@2

  std::array<int, 52> dword_4F5CC8_ys; // idb
  std::array<int, 52> dword_4F5D98_xs; // idb

  //__debugbreak();

  v5 = a4->uAttributes;
  v9 = pIndoor->pVertices;
  if ( v5 & FACE_XY_PLANE )
  {
    a4a = x;
    v8 = y;
    for(int i = 0; i < a4->uNumVertices; i++)
	{
        dword_4F5D98_xs[i] = v9[a4->pVertexIDs[i]].x;
        dword_4F5CC8_ys[i] = v9[a4->pVertexIDs[i]].y;
	}
  }
  else
  {
    v8 = z;
    if ( v5 & FACE_XZ_PLANE )
    {
      a4a = x;
      for(int i = 0; i < a4->uNumVertices; i++)
	  {
		dword_4F5D98_xs[i] = v9[a4->pVertexIDs[i]].x;
		dword_4F5CC8_ys[i] = v9[a4->pVertexIDs[i]].z;
	  }
    }
    else
    {
      a4a = y;
      for(int i = 0; i < a4->uNumVertices; i++)
	  {
		dword_4F5D98_xs[i] = v9[a4->pVertexIDs[i]].y;
		dword_4F5CC8_ys[i] = v9[a4->pVertexIDs[i]].z;
	  }
    }
  }
  a3a = 0;
  dword_4F5D98_xs[a4->uNumVertices] = dword_4F5D98_xs[0];
  dword_4F5CC8_ys[a4->uNumVertices] = dword_4F5CC8_ys[0];
  for(int i = 0; i < a4->uNumVertices && a3a < 2; i++)
  {
    if ( dword_4F5CC8_ys[i] >= v8 ^ (dword_4F5CC8_ys[i + 1] >= v8) )
    {
	  //if( dword_4F5D98_xs[i + 1] >= a4a || dword_4F5D98_xs[i] >= a4a)
	  if( !(dword_4F5D98_xs[i + 1] >= a4a && dword_4F5D98_xs[i] < a4a))
      {
		  if ( (dword_4F5D98_xs[i + 1] < a4a && dword_4F5D98_xs[i] >= a4a) )
			  ++a3a;
		  //|| (v25 = dword_4F5D98_xs[i + 1] - dword_4F5D98_xs[i],LODWORD(v26) = v25 << 16, HIDWORD(v26) = v25 >> 16, 
		  //dword_4F5D98_xs[i] + ((signed int)(((unsigned __int64)(v26 / (dword_4F5CC4_ys[i + 2] - dword_4F5CC4_ys[i + 1])* ((v8 - dword_4F5CC4_ys[i + 1]) << 16)) >> 16)
          //                + 32768) >> 16) >= a4a) )
		  else
		  {
			v25 = fixpoint_div(dword_4F5D98_xs[i + 1] - dword_4F5D98_xs[i], dword_4F5CC8_ys[i + 1] - dword_4F5CC8_ys[i]);
			if( dword_4F5D98_xs[i] + (fixpoint_mul(v25, (v8 - dword_4F5CC8_ys[i]) << 16) + 32768 >> 16) >= a4a)
				++a3a;
		  }
      }
    }
  }
  result = 1;
  if ( a3a != 1 )
    result = 0;
  return result;
}

//----- (004077F1) --------------------------------------------------------
bool __fastcall sub_4077F1(int a1, int a2, int a3, ODMFace *a4, BSPVertexBuffer *a5)
{
  //ODMFace *v5; // eax@1
  Vec3_int_ *v9;
  unsigned int v5; // esi@1
  signed int v7; // edi@1
  //char v8; // zf@2
  //unsigned __int16 *v9; // edx@3
  int v10; // ecx@4
  unsigned __int16 *v11; // edx@8
  int v12; // ecx@9
  int v13; // edx@12
  int v14; // ecx@13
  int v15; // esi@14
  bool v16; // edi@14
  int v17; // ecx@16
  int v18; // edx@16
  int v19; // eax@17
  signed int v20; // ebx@18
  int v21; // edi@20
  signed int v22; // eax@22
  signed __int64 v23; // qtt@22
  //bool result; // eax@25
  //int v25; // [sp+14h] [bp-8h]@14
  int a4a; // [sp+28h] [bp+Ch]@2
  signed int a5a; // [sp+2Ch] [bp+10h]@14

  std::array<int, 52> dword_4F5B24_ys; // idb
  std::array<int, 52> dword_4F5BF4_xs; // idb
  
  __debugbreak();

  v5 = a4->uAttributes;
  v9 = a5->pVertices;
  if ( v5 & FACE_XY_PLANE )
  {
    a4a = a1;
    a3 = a2;
    for(int i = 0; i < a4->uNumVertices; i++)
	{
        dword_4F5BF4_xs[i+1] = v9[a4->pVertexIDs[i]].x;
        dword_4F5B24_ys[i+1] = v9[a4->pVertexIDs[i]].y;
	}
  }
  else
  {
    if ( v5 & FACE_XY_PLANE )
    {
      a4a = a1;
      for(int i = 0; i < a4->uNumVertices; i++)
	  {
		dword_4F5BF4_xs[i+1] = v9[a4->pVertexIDs[i]].x;
		dword_4F5B24_ys[i+1] = v9[a4->pVertexIDs[i]].z;
	  }
    }
    else
    {
      a4a = a2;
      for(int i = 0; i < a4->uNumVertices; i++)
	  {
		dword_4F5BF4_xs[i+1] = v9[a4->pVertexIDs[i]].y;
		dword_4F5B24_ys[i+1] = v9[a4->pVertexIDs[i]].z;
	  }
    }
  }
  a5a = 0;
  dword_4F5BF4_xs[a4->uNumVertices + 1] = dword_4F5BF4_xs[1];
  dword_4F5B24_ys[a4->uNumVertices + 1] = dword_4F5B24_ys[1];
  for(int i = 0; i < a4->uNumVertices; i++)
  {
    if ( a5a >= 2 )
      break;
    if ( dword_4F5B24_ys[i + 1] >= a3 ^ (dword_4F5B24_ys[i + 2] >= a3) )
    {
	  if( dword_4F5BF4_xs[i + 2] >= a4a || dword_4F5BF4_xs[i] >= a4a)
      {
		  if (dword_4F5BF4_xs[i + 2] >= a4a && dword_4F5BF4_xs[i + 1] >= a4a)
            ++a5a;
          else
          {
              v23 = (__int64)(dword_4F5BF4_xs[i + 2] - dword_4F5BF4_xs[i + 1]) << 16;
              __int64 _a = dword_4F5B24_ys[i + 2] - dword_4F5B24_ys[i + 1];
              __int64 _b = (__int64)(a3 - dword_4F5B24_ys[i + 1]) << 16;

              if (dword_4F5BF4_xs[i + 1] + ((((v23 / _a * _b) >> 16) + 32768) >> 16) >= a4a)
                ++a5a;
          }
	  }
    }
  }

  if ( a5a != 1 )
    return false;
  return true;

}

//----- (004088E9) --------------------------------------------------------
int __fastcall sub_4088E9(int x1, int y1, int x2, int y2, int x3, int y3)
{
  int v8; // eax@1
  signed int result; // eax@1
  int v11; // [sp+18h] [bp+8h]@1

  v11 = abs(x2 - x1);
  v8 = abs(y2 - y1);
  result = integer_sqrt(v11 * v11 + v8 * v8);
  if ( result )
    result = abs(((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / result);
  return result;
}

//----- (0040DEF3) --------------------------------------------------------
unsigned short TargetColor( unsigned __int16 r, unsigned __int16 g, unsigned __int16 b )
{
  return ((unsigned int)b >> (8 - LOBYTE(pRenderer->uTargetBBits))) | pRenderer->uTargetGMask & (g << (LOBYTE(pRenderer->uTargetGBits) + 
                           LOBYTE(pRenderer->uTargetBBits) - 8)) | pRenderer->uTargetRMask & (r << (LOBYTE(pRenderer->uTargetGBits) + 
                           LOBYTE(pRenderer->uTargetRBits) + LOBYTE(pRenderer->uTargetBBits) - 8));
}



//----- (0040F82D) --------------------------------------------------------
void __fastcall ZBuffer_Fill(int *pZBuffer, int uTextureId, int iZValue)
{
  assert(uTextureId != -1);
  ZBuffer_DoFill(pZBuffer, pIcons_LOD->GetTexture(uTextureId), iZValue);
}



//----- (0040F89C) --------------------------------------------------------
void __fastcall ZBuffer_DoFill(int *pZBuffer, Texture *pTex, int uZValue)
{
  void *v3; // eax@3
  void *v4; // esi@5
  int *v5; // edi@5
  int v6; // eax@5
  int v7; // ecx@6
  Texture *v8; // [sp+Ch] [bp-14h]@1
  int *v9; // [sp+10h] [bp-10h]@1
  int v10; // [sp+14h] [bp-Ch]@1
  int v11; // [sp+18h] [bp-8h]@1
  void *v12; // [sp+1Ch] [bp-4h]@5

  v10 = pTex->uTextureWidth;
  v8 = pTex;
  v9 = pZBuffer;
  v11 = pTex->uTextureHeight;
  if ( pIcons_LOD->dword_011BA4 && pTex->uDecompressedSize )
    v3 = pTex->UnzipPalette();
  else
    v3 = pTex->pLevelOfDetail0_prolly_alpha_mask;
  v12 = v3;
  v4 = v3;
  v5 = v9;
  v6 = 0;
  do
  {
    v7 = v10;
    do
    {
      LOBYTE(v6) = *(char *)v4;
      v4 = (char *)v4 + 1;
      if ( v6 )
        *v5 = uZValue;
      ++v5;
      --v7;
    }
    while ( v7 );
    v5 += 640 - v10;
    --v11;
  }
  while ( v11 );
  if ( pIcons_LOD->dword_011BA4 )
  {
    if ( v8->uDecompressedSize )
      free(v12);
  }
}

//----- (0040F92A) --------------------------------------------------------
void __fastcall ZBuffer_DoFill2(int *pZBuffer, Texture *a2, int a3)
{
  Texture *v3; // esi@1
  void *v4; // eax@3
  int *v5; // edi@5
  int v6; // ecx@6
  int *v7; // [sp+Ch] [bp-10h]@1
  int v8; // [sp+10h] [bp-Ch]@1
  int v9; // [sp+18h] [bp-4h]@1

  v3 = a2;
  v7 = pZBuffer;
  v8 = a2->uTextureWidth;
  v9 = a2->uTextureHeight;
  if ( pIcons_LOD->dword_011BA4 && a2->uDecompressedSize )
    v4 = a2->UnzipPalette();
  else
    v4 = a2->pLevelOfDetail0_prolly_alpha_mask;
  v5 = v7;
  do
  {
    v6 = v8;
    do
    {
      *v5 = a3;
      ++v5;
      --v6;
    }
    while ( v6 );
    v5 += 640 - v8;
    --v9;
  }
  while ( v9 );
  if ( pIcons_LOD->dword_011BA4 )
  {
    if ( v3->uDecompressedSize )
      free(v4);
  }
}

//----- (00410D99) --------------------------------------------------------
int __fastcall sub_410D99_get_map_index(int a1)
{
  int v1; // edi@1
  signed int v2; // ebp@1
  const char **v3; // esi@2
  char *v4; // ebx@2

  v1 = a1;
  v2 = 1;
  if ( _stricmp(pMapStats->pInfos[1].pFilename, (const char *)&pGames_LOD->pSubIndices[a1]) )
  {
    v3 = (const char **)&pMapStats->pInfos[1].pFilename;
    v4 = (char *)&pMapStats->pInfos[1].pFilename;
    do
    {
      if ( !*(int *)v4 )
        break;
      v3 += 17;
      ++v2;
      v4 = (char *)v3;
    }
    while ( _stricmp(*v3, (const char *)&pGames_LOD->pSubIndices[v1]) );
  }
  return v2;
}

// 4E28F8: using guessed type int pCurrentScreen;

//----- (00417AD4) --------------------------------------------------------
unsigned int GetSkillColor(unsigned int uPlayerClass, PLAYER_SKILL_TYPE uPlayerSkillType, signed int skill_level)
{  
  switch (uPlayerClass % 4)
  {
    case 0:
    {
      if (byte_4ED970_skill_learn_ability_by_class_table[uPlayerClass][uPlayerSkillType] >= skill_level)
        return ui_character_skillinfo_can_learn;
      if (byte_4ED970_skill_learn_ability_by_class_table[uPlayerClass + 1][uPlayerSkillType] < skill_level &&
          byte_4ED970_skill_learn_ability_by_class_table[uPlayerClass + 2][uPlayerSkillType] < skill_level)
      {
        if (byte_4ED970_skill_learn_ability_by_class_table[uPlayerClass + 3][uPlayerSkillType] < skill_level)
          return ui_character_skillinfo_cant_learn;
      }
      return ui_character_skillinfo_can_learn_gm;
    }
    break;

    case 1:
    {
      if (byte_4ED970_skill_learn_ability_by_class_table[uPlayerClass][uPlayerSkillType] >= skill_level)
        return ui_character_skillinfo_can_learn;
      if (byte_4ED970_skill_learn_ability_by_class_table[uPlayerClass + 1][uPlayerSkillType] < skill_level)
      {
        if (byte_4ED970_skill_learn_ability_by_class_table[uPlayerClass + 2][uPlayerSkillType] < skill_level)
          return ui_character_skillinfo_cant_learn;
      }
      return ui_character_skillinfo_can_learn_gm;
    }
    break;

    case 2:
    case 3:
    {
      if (byte_4ED970_skill_learn_ability_by_class_table[uPlayerClass][uPlayerSkillType] < skill_level)
        return ui_character_skillinfo_cant_learn;
      return ui_character_skillinfo_can_learn;
    }
    break;
  }
  Error("Invalid player class: %u", uPlayerClass);
}