#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define _CRT_SECURE_NO_WARNINGS
#include "ErrorHandling.h"
#include "LightmapBuilder.h"
#include "mm7.h"
#include "GUIWindow.h"
#include "Party.h"
#include "Outdoor.h"
#include "LOD.h"
#include "Actor.h"
#include "Viewport.h"
#include "OurMath.h"
#include "SpriteObject.h"
#include "Timer.h"
#include "stru298.h"
#include "Lights.h"
#include "Level/Decoration.h"
#include "mm7_unsorted_subs.h"
#include "mm7_data.h"


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
  int attacker_type; // ecx@3
  signed int v3; // eax@3
  unsigned int target_id; // edi@6
  int target_type; // eax@6
  int v10; // edi@8
  Vec3_int_ attacker_coord; // ST04_12@9
//  int v12; // ST0C_4@10
  int v15; // edx@15
  int v19; // edi@15
  int v23; // edx@18
  int v24; // eax@18
//  int v30; // eax@29
  int v31; // edx@29
  int v32; // eax@29
  int v33; // ST24_4@29
  SpriteObject *v36; // [sp+0h] [bp-28h]@0
  int attacker_id; // [sp+10h] [bp-18h]@1
  int v44; // [sp+14h] [bp-14h]@15
  //Vec3_int_ *pVelocity; // [sp+1Ch] [bp-Ch]@2
  signed int a1; // [sp+20h] [bp-8h]@8
  int v48; // [sp+24h] [bp-4h]@8


  for ( attacker_id = 0; attacker_id < AttackerInfo.count; ++attacker_id )
  {
    attacker_type = PID_TYPE(AttackerInfo.pIDs[attacker_id]);
    v3 = PID_ID(AttackerInfo.pIDs[attacker_id]);

    if ( attacker_type == 2 )
    {
      v36 = &pSpriteObjects[v3];
      attacker_type = PID_TYPE(pSpriteObjects[v3].spell_caster_pid);
      v3 = PID_ID(pSpriteObjects[v3].spell_caster_pid);
    }

    if ( AttackerInfo.field_3EC[attacker_id] & 1 )
    {
      target_id = PID_ID(ai_near_actors_targets_pid[v3]);
      target_type = PID_TYPE(ai_near_actors_targets_pid[v3]) - 3;
      if ( target_type )
      {
        if ( target_type == 1 )//party damage from monsters(повреждения группе от монстров)
        {
          v10 = pParty->vPosition.y - AttackerInfo.pYs[attacker_id];
          a1 = pParty->vPosition.x - AttackerInfo.pXs[attacker_id];
          v48 = pParty->vPosition.y - AttackerInfo.pYs[attacker_id];
          if ( a1 * a1 + v10 * v10
             + ((signed int)(pParty->vPosition.z + pParty->uPartyHeight) >> (1 - AttackerInfo.pZs[attacker_id]))
             * ((signed int)(pParty->vPosition.z + pParty->uPartyHeight) >> (1 - AttackerInfo.pZs[attacker_id]))
             < (unsigned int)((AttackerInfo.field_324[attacker_id] + 32) * (AttackerInfo.field_324[attacker_id] + 32)) )
          {
            attacker_coord.x = AttackerInfo.pXs[attacker_id];
            attacker_coord.y = AttackerInfo.pYs[attacker_id];
            attacker_coord.z = AttackerInfo.pZs[attacker_id];
            if ( sub_407A1C(pParty->vPosition.x, pParty->vPosition.y, pParty->vPosition.z + pParty->sEyelevel, attacker_coord) )
              DamagePlayerFromMonster(AttackerInfo.pIDs[attacker_id], AttackerInfo.field_450[attacker_id], &AttackerInfo.vec_4B4[attacker_id], stru_50C198.which_player_to_attack(&pActors[v3]));
          }
        }
      }
      else//Actor damage from monsters(повреждение местного жителя)
      {
        if ( SHIDWORD(pActors[target_id].pActorBuffs[ACTOR_BUFF_PARALYZED].uExpireTime) > 0
          || SHIDWORD(pActors[target_id].pActorBuffs[ACTOR_BUFF_PARALYZED].uExpireTime) >= 0
           && LODWORD(pActors[target_id].pActorBuffs[ACTOR_BUFF_PARALYZED].uExpireTime)
          || pActors[target_id].CanAct() )
        {
          v15 = pActors[target_id].vPosition.y - AttackerInfo.pYs[attacker_id];
          a1 = pActors[target_id].vPosition.x - AttackerInfo.pXs[attacker_id];
          v44 = pActors[target_id].vPosition.z;
          v19 = AttackerInfo.field_324[attacker_id] + pActors[target_id].uActorRadius;
          v48 = v15;
          if ( a1 * a1 + v15 * v15 + (pActors[target_id].vPosition.z + (pActors[target_id].uActorHeight >> 1) - AttackerInfo.pZs[attacker_id])
           * (pActors[target_id].vPosition.z + (pActors[target_id].uActorHeight >> 1) - AttackerInfo.pZs[attacker_id]) < (unsigned int)(v19 * v19) )
          {
            attacker_coord.x = AttackerInfo.pXs[attacker_id];
            attacker_coord.y = AttackerInfo.pYs[attacker_id];
            attacker_coord.z = AttackerInfo.pZs[attacker_id];
            if ( sub_407A1C(pActors[target_id].vPosition.x, pActors[target_id].vPosition.y, pActors[target_id].vPosition.z + 50, attacker_coord) )
            {
              Vec3_int_::Normalize(&a1, &v48, &v44);
              AttackerInfo.vec_4B4[attacker_id].x = a1;
              AttackerInfo.vec_4B4[attacker_id].y = v48;
              AttackerInfo.vec_4B4[attacker_id].z = v44;
              Actor::ActorDamageFromMonster(AttackerInfo.pIDs[attacker_id], target_id, &AttackerInfo.vec_4B4[attacker_id], AttackerInfo.field_450[attacker_id]);
            }
          }
        }
      }
    }
    else //damage from spells(повреждения от заклов(метеоритный дождь))
    {
      v23 = pParty->vPosition.y - AttackerInfo.pYs[attacker_id];
      v24 = ((signed int)pParty->uPartyHeight / 2) - AttackerInfo.pZs[attacker_id];
      a1 = pParty->vPosition.x - AttackerInfo.pXs[attacker_id];
      v48 = pParty->vPosition.y - AttackerInfo.pYs[attacker_id];
      if ( a1 * a1 + v23 * v23 + (pParty->vPosition.z + v24) * (pParty->vPosition.z + v24) < (unsigned int)((AttackerInfo.field_324[attacker_id] + 32) * (AttackerInfo.field_324[attacker_id] + 32)) )
      {//party damage (повреждения группе)
        attacker_coord.x = AttackerInfo.pXs[attacker_id];
        attacker_coord.y = AttackerInfo.pYs[attacker_id];
        attacker_coord.z = AttackerInfo.pZs[attacker_id];
        if ( sub_407A1C(pParty->vPosition.x, pParty->vPosition.y, pParty->vPosition.z + pParty->sEyelevel, attacker_coord) )
        {
          for ( uint i = 0; i < 4; ++i )
          {
            if ( !(HIDWORD(pParty->pPlayers[i].pConditions[Condition_Dead]) | LODWORD(pParty->pPlayers[i].pConditions[Condition_Dead]))
              && !pParty->pPlayers[i].pConditions[Condition_Pertified] && !pParty->pPlayers[i].pConditions[Condition_Eradicated] )
              DamagePlayerFromMonster(AttackerInfo.pIDs[attacker_id], AttackerInfo.field_450[attacker_id], &AttackerInfo.vec_4B4[attacker_id], i);
          }
        }
      }
      if ( (signed int)uNumActors > 0 )
      {//actors damage(повреждения другим участникам)
        for ( int actorID = 0; (signed int)actorID < (signed int)uNumActors; ++actorID )
        {
          if ( pActors[actorID].CanAct() )
          {
            //v30 = pActors[actorID].vPosition.y - AttackerInfo.pYs[attacker_id];
            a1 = pActors[actorID].vPosition.x - AttackerInfo.pXs[attacker_id];
            v31 = pActors[actorID].vPosition.z;
            v48 = pActors[actorID].vPosition.y - AttackerInfo.pYs[attacker_id];
            v44 = pActors[actorID].vPosition.z;
            v32 = (pActors[actorID].uActorHeight / 2) - AttackerInfo.pZs[attacker_id];
            v33 = pActors[actorID].uActorRadius + AttackerInfo.field_324[attacker_id];
            if ( a1 * a1 + v48 * v48 + (v31 + v32) * (v31 + v32) < (unsigned int)(v33 * v33) )
            {
              attacker_coord.x = AttackerInfo.pXs[attacker_id];
              attacker_coord.y = AttackerInfo.pYs[attacker_id];
              attacker_coord.z = AttackerInfo.pZs[attacker_id];
              if ( sub_407A1C(pActors[actorID].vPosition.x, pActors[actorID].vPosition.y, pActors[actorID].vPosition.z + 50, attacker_coord) )//что делает ф-ция?
              {
                Vec3_int_::Normalize(&a1, &v48, &v44);
                AttackerInfo.vec_4B4[attacker_id].x = a1;
                AttackerInfo.vec_4B4[attacker_id].y = v48;
                AttackerInfo.vec_4B4[attacker_id].z = v44;
                switch ( attacker_type )
                {
                  case OBJECT_Player:
                    Actor::DamageMonsterFromParty(AttackerInfo.pIDs[attacker_id], actorID, &AttackerInfo.vec_4B4[attacker_id]);
                    break;
                  case OBJECT_Actor:
                    if ( v36 && pActors[v3].GetActorsRelation(&pActors[actorID]) )
                      Actor::ActorDamageFromMonster(AttackerInfo.pIDs[attacker_id], actorID, &AttackerInfo.vec_4B4[attacker_id], v36->field_61);
                    break;
                  case OBJECT_Item:
                    ItemDamageFromActor(AttackerInfo.pIDs[attacker_id], actorID, &AttackerInfo.vec_4B4[attacker_id]);
                    break;
                }
              }
            }
          }
        }
      }
    }
  }
  AttackerInfo.count = 0;
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
    DamagePlayerFromMonster(PID(OBJECT_Item, uLayingItemID), pSpriteObjects[uLayingItemID].field_61, &layingitem_vel_50FDFC, -1);
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
        Actor::ActorDamageFromMonster(PID(OBJECT_Item, uLayingItemID), PID_ID(a2), &layingitem_vel_50FDFC, pSpriteObjects[uLayingItemID].field_61);
        break;
      case OBJECT_Player:
        Actor::DamageMonsterFromParty(PID(OBJECT_Item, uLayingItemID), PID_ID(a2), &layingitem_vel_50FDFC);
        break;
      case OBJECT_Item:
        ItemDamageFromActor(PID(OBJECT_Item, uLayingItemID), PID_ID(a2), &layingitem_vel_50FDFC);
        break;
    }
  }
}

//----- (0043AE12) --------------------------------------------------------
double __fastcall sub_43AE12(signed int a1)
{
  //signed int v1; // ST00_4@1
  signed int v2; // ecx@1
  double v3; // st7@1
  double result; // st7@6

  v3 = (double)a1;
  for ( v2 = 0; v2 < 5; ++v2 )
  {
    if ( v3 < flt_4E4A80[v2 + 5] )
      break;
  }
  if ( v2 <= 0 || v2 >= 5 )
  {
    if ( v2 )
      result = flt_4E4A80[4];
    else
      result = flt_4E4A80[0];
  }
  else
    result = (flt_4E4A80[v2] - flt_4E4A80[v2 - 1]) * (v3 - flt_4E4A80[v2 + 4]) / (flt_4E4A80[v2 + 5] - flt_4E4A80[v2 + 4]) + flt_4E4A80[v2];
  return result;
}

//----- (0043B057) --------------------------------------------------------
void ItemDamageFromActor(unsigned int uObjID, unsigned int uActorID, Vec3_int_ *pVelocity)
{
  int v6; // eax@4
  int damage; // edi@4
  int a2a; // [sp+Ch] [bp-4h]@8

  if ( !pActors[uActorID].IsNotAlive() )
  {
    if ( PID_TYPE(uObjID) == OBJECT_Item)
    {
      if ( pSpriteObjects[PID_ID(uObjID)].spell_id )
      {
        v6 = _43AFE3_calc_spell_damage(pSpriteObjects[PID_ID(uObjID)].spell_id, pSpriteObjects[PID_ID(uObjID)].spell_level, pSpriteObjects[PID_ID(uObjID)].spell_skill, pActors[uActorID].sCurrentHP);
        damage = pActors[uActorID].CalcMagicalDamageToActor((DAMAGE_TYPE)0, v6);
        pActors[uActorID].sCurrentHP -= damage;
        if ( damage )
        {
          if ( pActors[uActorID].sCurrentHP > 0 )
            Actor::AI_Stun(uActorID, uObjID, 0);
          else
            Actor::Die(uActorID);
          a2a = 20 * damage / (signed int)pActors[uActorID].pMonsterInfo.uHP;
          if ( 20 * damage / (signed int)pActors[uActorID].pMonsterInfo.uHP > 10 )
            a2a = 10;
          if ( !MonsterStats::BelongsToSupertype(pActors[uActorID].pMonsterInfo.uID, MONSTER_SUPERTYPE_TREANT) )
          {
            pVelocity->x = fixpoint_mul(a2a, pVelocity->x);
            pVelocity->y = fixpoint_mul(a2a, pVelocity->y);
            pVelocity->z = fixpoint_mul(a2a, pVelocity->z);
            pActors[uActorID].vVelocity.x = 50 * LOWORD(pVelocity->x);
            pActors[uActorID].vVelocity.y = 50 * LOWORD(pVelocity->y);
            pActors[uActorID].vVelocity.z = 50 * LOWORD(pVelocity->z);
          }
          Actor::AddBloodsplatOnDamageOverlay(uActorID, 1, damage);
        }
        else
          Actor::AI_Stun(uActorID, uObjID, 0);
      }
    }
  }
}


//----- (0043F515) --------------------------------------------------------
void FindBillboardsLightLevels_BLV()
{
  for (uint i = 0; i < uNumBillboardsToDraw; ++i)
  {
    if (pBillboardRenderList[i].field_1E & 2 || uCurrentlyLoadedLevelType == LEVEL_Indoor && !pBillboardRenderList[i].uIndoorSectorID)
      pBillboardRenderList[i].dimming_level = 0;
    else
      pBillboardRenderList[i].dimming_level = _43F55F_get_billboard_light_level(&pBillboardRenderList[i], -1);
  }
}

//----- (0043F55F) --------------------------------------------------------
int __fastcall _43F55F_get_billboard_light_level(RenderBillboard *a1, int uBaseLightLevel)
{
  signed int v3; // ecx@2

  if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
    v3 = pIndoor->pSectors[a1->uIndoorSectorID].uMinAmbientLightLevel;
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
//  int v5; // esi@1
  signed int v6; // edi@1
  int v8; // eax@6
  int v9; // ebx@6
  unsigned int v10; // ecx@6
  unsigned int v11; // edx@9
  unsigned int v12; // edx@11
  signed int v13; // ecx@12
  BLVLightMM7 *v16; // esi@20
  int v17; // ebx@21
//  int v18; // eax@24
//  int v19; // ebx@24
//  unsigned int v20; // ecx@24
//  int v21; // edx@25
//  unsigned int v22; // edx@27
//  unsigned int v23; // edx@29
  signed int v24; // ecx@30
  int v26; // ebx@35
//  int v27; // eax@38
//  int v28; // ebx@38
//  unsigned int v29; // ecx@38
//  int v30; // edx@39
//  unsigned int v31; // edx@41
//  unsigned int v32; // edx@43
  //signed int v33; // ecx@44
  int v37; // [sp+Ch] [bp-18h]@37
//  int v38; // [sp+10h] [bp-14h]@5
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
                v44 = int_get_vector_length(v17, v42, v39);
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
    //StationaryLight* p = &pStationaryLightsStack->pLights[i];
      v26 = abs(pStationaryLightsStack->pLights[i].vPosition.x - x);
      if ( v26 <= pStationaryLightsStack->pLights[i].uRadius)
      {
        v40 = abs(pStationaryLightsStack->pLights[i].vPosition.y - y);
        if ( v40 <= pStationaryLightsStack->pLights[i].uRadius)
        {
          v37 = abs(pStationaryLightsStack->pLights[i].vPosition.z - z);
          if ( v37 <= pStationaryLightsStack->pLights[i].uRadius)
          {
            v45 = int_get_vector_length(v26, v40, v37);
            //v33 = pStationaryLightsStack->pLights[i].uRadius;
            if ( (signed int)v45 < pStationaryLightsStack->pLights[i].uRadius )
              v6 += ((unsigned __int64)(30i64 * (signed int)(v45 << 16) / pStationaryLightsStack->pLights[i].uRadius) >> 16) - 30;
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
    v6 = 31;
  return v6;
}



//----- (004075DB) --------------------------------------------------------
bool __fastcall sub_4075DB(int x, int y, int z, BLVFace *face)
{
  int v8; // edi@2
  signed int v25; // eax@22
  bool result; // eax@25
  signed int a3a; // [sp+24h] [bp+8h]@14
  int a4a; // [sp+28h] [bp+Ch]@2

  std::array<int, 52> dword_4F5CC8_ys; // idb
  std::array<int, 52> dword_4F5D98_xs; // idb

  //__debugbreak();

  if ( face->uAttributes & FACE_XY_PLANE )
  {
    a4a = x;
    v8 = y;
    for(int i = 0; i < face->uNumVertices; i++)
	{
        dword_4F5D98_xs[i] = pIndoor->pVertices[face->pVertexIDs[i]].x;
        dword_4F5CC8_ys[i] = pIndoor->pVertices[face->pVertexIDs[i]].y;
	}
  }
  else
  {
    v8 = z;
    if ( face->uAttributes & FACE_XZ_PLANE )
    {
      a4a = x;
      for(int i = 0; i < face->uNumVertices; i++)
	  {
		dword_4F5D98_xs[i] = pIndoor->pVertices[face->pVertexIDs[i]].x;
		dword_4F5CC8_ys[i] = pIndoor->pVertices[face->pVertexIDs[i]].z;
	  }
    }
    else
    {
      a4a = y;
      for(int i = 0; i < face->uNumVertices; i++)
	  {
		dword_4F5D98_xs[i] = pIndoor->pVertices[face->pVertexIDs[i]].y;
		dword_4F5CC8_ys[i] = pIndoor->pVertices[face->pVertexIDs[i]].z;
	  }
    }
  }
  a3a = 0;
  dword_4F5D98_xs[face->uNumVertices] = dword_4F5D98_xs[0];
  dword_4F5CC8_ys[face->uNumVertices] = dword_4F5CC8_ys[0];
  for(int i = 0; i < face->uNumVertices && a3a < 2; i++)
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
			if( dword_4F5D98_xs[i] + (fixpoint_mul(v25, (v8 - dword_4F5CC8_ys[i]) << 16) + 0x8000 >> 16) >= a4a)
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
bool __fastcall sub_4077F1(int a1, int a2, int a3, ODMFace *face, BSPVertexBuffer *a5)
{
  int a4a; // [sp+28h] [bp+Ch]@2
  signed int a5a; // [sp+2Ch] [bp+10h]@14

  std::array<int, 52> dword_4F5B24_ys; // idb
  std::array<int, 52> dword_4F5BF4_xs; // idb

  //__debugbreak();	//срабатывает при нападении стрекозавров с огнём

  if ( face->uAttributes & FACE_XY_PLANE )
  {
    a4a = a1;
    a3 = a2;
    for(int i = 0; i < face->uNumVertices; i++)
	{
        dword_4F5BF4_xs[i+1] = a5->pVertices[face->pVertexIDs[i]].x;
        dword_4F5B24_ys[i+1] = a5->pVertices[face->pVertexIDs[i]].y;
	}
  }
  else
  {
    if ( face->uAttributes & FACE_XY_PLANE )
    {
      a4a = a1;
      for(int i = 0; i < face->uNumVertices; i++)
	  {
		dword_4F5BF4_xs[i+1] = a5->pVertices[face->pVertexIDs[i]].x;
		dword_4F5B24_ys[i+1] = a5->pVertices[face->pVertexIDs[i]].z;
	  }
    }
    else
    {
      a4a = a2;
      for(int i = 0; i < face->uNumVertices; i++)
	  {
		dword_4F5BF4_xs[i+1] = a5->pVertices[face->pVertexIDs[i]].y;
		dword_4F5B24_ys[i+1] = a5->pVertices[face->pVertexIDs[i]].z;
	  }
    }
  }
  a5a = 0;
  dword_4F5BF4_xs[face->uNumVertices + 1] = dword_4F5BF4_xs[1];
  dword_4F5B24_ys[face->uNumVertices + 1] = dword_4F5B24_ys[1];
  for(int i = 0; i < face->uNumVertices; i++)
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
              //v23 = (__int64)(dword_4F5BF4_xs[i + 2] - dword_4F5BF4_xs[i + 1]) << 16;
              __int64 _a = dword_4F5B24_ys[i + 2] - dword_4F5B24_ys[i + 1];
              __int64 _b = (__int64)(a3 - dword_4F5B24_ys[i + 1]) << 16;

              if (dword_4F5BF4_xs[i + 1] + ((((((__int64)(dword_4F5BF4_xs[i + 2] - dword_4F5BF4_xs[i + 1]) << 16) / _a * _b) >> 16) + 0x8000) >> 16) >= a4a)
                ++a5a;
          }
	  }
    }
  }

  if ( a5a != 1 )
    return false;
  return true;

}


//----- (0040F82D) --------------------------------------------------------
void __fastcall ZBuffer_Fill(int *pZBuffer, int uTextureId, int iZValue)
{
  assert(uTextureId != -1);
  ZBuffer_DoFill(pZBuffer, pIcons_LOD->GetTexture(uTextureId), iZValue);
}

//----- (0040F89C) --------------------------------------------------------
void __fastcall ZBuffer_DoFill(int *pZBuffer, Texture *pTex, int uZValue)
{//срабатывает при продаже в магазине
  void *v3; // eax@3
  //void *v4; // esi@5
  //int *v5; // edi@5
  //int v6; // eax@5
//  int v7; // ecx@6
//  int v11; // [sp+18h] [bp-8h]@1
  //void *v12; // [sp+1Ch] [bp-4h]@5

  if ( pIcons_LOD->dword_011BA4 && pTex->uDecompressedSize )
    v3 = pTex->UnzipPalette();
  else
    v3 = pTex->pLevelOfDetail0_prolly_alpha_mask;
  //v12 = v3;
  //v4 = v3;
  //v5 = pZBuffer;
  //v6 = 0;
  for ( uint i = 0; i < pTex->uTextureHeight; i++ )
  {
    for ( uint j = 0; j < pTex->uTextureWidth; j++ )
    {
      //LOBYTE(v6) = *(char *)v4;
      //v4 = (char *)v4 + 1;
      //if ( v6 )
        *pZBuffer = uZValue;
      ++pZBuffer;
    }
    pZBuffer += window->GetWidth() - pTex->uTextureWidth;
  }
  if ( pIcons_LOD->dword_011BA4 )
  {
    if ( pTex->uDecompressedSize )
      free(v3);
  }
}

//----- (0040F92A) --------------------------------------------------------
void __fastcall ZBuffer_DoFill2(int *pZBuffer, Texture *a2, int a3)
{//срабатывает в покупке в магазине
  void *v4; // eax@3
  //int *v5; // edi@5
//  int v6; // ecx@6
//  int v9; // [sp+18h] [bp-4h]@1

  if ( pIcons_LOD->dword_011BA4 && a2->uDecompressedSize )
    v4 = a2->UnzipPalette();
  else
    v4 = a2->pLevelOfDetail0_prolly_alpha_mask;
  //v5 = pZBuffer;
  for ( uint i = 0; i < a2->uTextureHeight; i++ )
  {
    for ( uint j = 0; j < a2->uTextureWidth; j++ )
    {
      *pZBuffer = a3;
      ++pZBuffer;
    }
    pZBuffer += window->GetWidth() - a2->uTextureWidth;
  }
  if ( pIcons_LOD->dword_011BA4 )
  {
    if ( a2->uDecompressedSize )
      free(v4);
  }
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