#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "MM7.h"



#include "Weather.h"
#include "Sprites.h"
#include "BSPModel.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "stru6.h"

#include "Vis.h"
#include "Game.h"
#include "GUIWindow.h"
#include "GUIFont.h"
#include "Party.h"
#include "AudioPlayer.h"
#include "Outdoor.h"
#include "Outdoor_stuff.h"
#include "Overlays.h"
#include "LOD.h"
#include "Actor.h"
#include "Events.h"
#include "Viewport.h"
#include "Math.h"
#include "SpriteObject.h"
#include "ObjectList.h"
#include "stru123.h"
#include "Time.h"
#include "IconFrameTable.h"
#include "Awards.h"
#include "TurnEngine.h"
#include "CastSpellInfo.h"
#include "stru298.h"
#include "texts.h"

#include "mm7_data.h"





//----- (004272F5) --------------------------------------------------------
int stru319::PlayerHitOrMiss(Player *pPlayer, Actor *pActor, int a3, int a4)
{
  signed int v5; // esi@1
  signed int v6; // edi@1
  int v7; // esi@8
  int v8; // eax@9
  int v9; // edx@11
  int v10; // edx@13
  int v11; // esi@13
  unsigned __int8 v12; // sf@13
  unsigned __int8 v13; // of@13
  int v14; // edx@14
  int v15; // eax@14
  signed int result; // eax@17

  v5 = pActor->pMonsterInfo.uAC;
  v6 = 0;
  if ( (signed __int64)pActor->pActorBuffs[ACTOR_BUFF_SOMETHING_THAT_HALVES_AC].uExpireTime > 0 )
    v5 /= 2;
  if ( (signed __int64)pActor->pActorBuffs[ACTOR_BUFF_HOUR_OF_POWER].uExpireTime > 0 )
    v6 = pActor->pActorBuffs[ACTOR_BUFF_SHIELD].uPower;
  if ( (signed __int64)pActor->pActorBuffs[ACTOR_BUFF_STONESKIN].uExpireTime > 0 && pActor->pActorBuffs[ACTOR_BUFF_STONESKIN].uPower > v6 )
    v6 = pActor->pActorBuffs[ACTOR_BUFF_STONESKIN].uPower;
  v7 = v6 + v5;
  if ( a3 )
    v8 = pPlayer->GetRangedAttack();
  else
    v8 = pPlayer->GetActualAttack(false);
  v9 = rand() % (v7 + 2 * v8 + 30);
  if ( a3 == 2 )
  {
    v14 = a4 + v9;
    v15 = ((v7 + 15) >> 1) + v7 + 15;
	v13 = __OFSUB__(v14, v15);
    v12 = v14 - v15 < 0;
  }
  else
  {
    if ( a3 != 3 )
    {
      v10 = a4 + v9;
      v11 = v7 + 15;
      v13 = __OFSUB__(v10, v11);
      v12 = v10 - v11 < 0;
    }
	else
	{
      v14 = a4 + v9;
      v15 = 2 * v7 + 30;
	  v13 = __OFSUB__(v14, v15);
      v12 = v14 - v15 < 0;
	}
  }
  if ( v12 ^ v13 )
    result = 0;
  else
    result = 1;
  return result;
}

//----- (004274AD) --------------------------------------------------------
bool stru319::ActorHitOrMiss(Actor *pActor, Player *pPlayer)
{
  signed int v3; // edi@1
  signed int v4; // esi@8
  int v5; // esi@8

  v3 = 0;
  if ( (signed __int64)pActor->pActorBuffs[ACTOR_BUFF_HOUR_OF_POWER].uExpireTime > 0 )
    v3 = pActor->pActorBuffs[ACTOR_BUFF_HOUR_OF_POWER].uPower;
  if ( (signed __int64)pActor->pActorBuffs[ACTOR_BUFF_BLESS].uExpireTime > 0 && pActor->pActorBuffs[ACTOR_BUFF_BLESS].uPower > v3 )
    v3 = pActor->pActorBuffs[ACTOR_BUFF_BLESS].uPower;
  if ( (signed __int64)pActor->pActorBuffs[ACTOR_BUFF_FATE].uExpireTime > 0 )
  {
    v3 += pActor->pActorBuffs[ACTOR_BUFF_FATE].uPower;
    pActor->pActorBuffs[ACTOR_BUFF_FATE].Reset();
  }
  v4 = pPlayer->GetActualAC() + 2 * pActor->pMonsterInfo.uLevel + 10;
  v5 = rand() % v4 + 1;
  return v3 + v5 > pPlayer->GetActualAC() + 5;
}

//----- (00427546) --------------------------------------------------------
int stru319::_427546(int a2)
{
  int result; // eax@2

  if ( a2 >= 0 )
  {
    if ( a2 >= 1 )
      result = (a2 >= 2) + 2;
    else
      result = 1;
  }
  else
  {
    result = 0;
  }
  return result;
}

//----- (0042756B) --------------------------------------------------------
int stru319::CalcMagicalDamageToActor(Actor *pActor, int a2, signed int a3)
{
  int v4; // edx@1
  int v5; // ecx@1
  signed int v6; // eax@4
  signed int result; // eax@17
  signed int v8; // esi@18

  v4 = 0;
  v5 = 0;
  if ( (signed __int64)pActor->pActorBuffs[ACTOR_BUFF_HOUR_OF_POWER].uExpireTime > 0 )
    v5 = pActor->pActorBuffs[ACTOR_BUFF_HOUR_OF_POWER].uPower;
  switch ( a2 )
  {
    case 0:
      v6 = pActor->pMonsterInfo.uResFire;
      v4 = v5;
      break;
    case 1:
      v6 = pActor->pMonsterInfo.uResAir;
      v4 = v5;
      break;
    case 2:
      v6 = pActor->pMonsterInfo.uResWater;
      v4 = v5;
      break;
    case 3:
      v6 = pActor->pMonsterInfo.uResEarth;
      v4 = v5;
      break;
    case 4:
      v6 = pActor->pMonsterInfo.uResPhysical;
      break;
    case 6:
      v6 = pActor->pMonsterInfo.uResSpirit;
      break;
    case 7:
      v6 = pActor->pMonsterInfo.uResMind;
      v4 = v5;
      break;
    case 8:
      v6 = pActor->pMonsterInfo.uResBody;
      v4 = v5;
      break;
    case 9:
      v6 = pActor->pMonsterInfo.uResLight;
      break;
    case 10:
      v6 = pActor->pMonsterInfo.uResDark;
      break;
    default:
      v6 = 0;
      break;
  }
  if ( v6 < 200 )
  {
    v8 = v4 + v6 + 30;
    if ( rand() % v8 >= 30 )
    {
      a3 >>= 1;
      if ( rand() % v8 >= 30 )
      {
        a3 >>= 1;
        if ( rand() % v8 >= 30 )
        {
          a3 >>= 1;
          if ( rand() % v8 >= 30 )
            a3 >>= 1;
        }
      }
    }
    result = a3;
  }
  else
  {
    result = 0;
  }
  return result;
}

//----- (00427662) --------------------------------------------------------
bool stru319::GetMagicalResistance(Actor *pActor, unsigned int uType)
{
  signed int resist; // esi@2
  bool result; // eax@13

  switch ( uType )
  {
    case 0:
      resist = pActor->pMonsterInfo.uResFire;
      break;
    case 1:
      resist = pActor->pMonsterInfo.uResAir;
      break;
    case 2:
      resist = pActor->pMonsterInfo.uResWater;
      break;
    case 3:
      resist = pActor->pMonsterInfo.uResEarth;
      break;
    case 4:
      resist = pActor->pMonsterInfo.uResPhysical;
      break;
    case 6:
      resist = pActor->pMonsterInfo.uResSpirit;
      break;
    case 7:
      resist = pActor->pMonsterInfo.uResMind;
    case 8:
      resist = pActor->pMonsterInfo.uResBody;
      break;
    case 9:
      resist = pActor->pMonsterInfo.uResLight;
      break;
    case 10:
      resist = pActor->pMonsterInfo.uResDark;
      break;
    default:
      return 1;
  }
  if ( resist < 200 )
    result = rand() % (signed int)(((unsigned int)pActor->pMonsterInfo.uLevel >> 2) + resist + 30) < 30;
  else
    result = 0;
  return result;
}

//----- (0042EB8D) --------------------------------------------------------
void GUIMessageQueue::AddMessageImpl(UIMessageType msg, int param, unsigned int a4, const char *file, int line)
{
  //Log::Warning(L"%s @ (%S %u)", UIMessage2String(msg), file, line);
  if (uNumMessages < 40)
  {
    files[uNumMessages] = file;
    lines[uNumMessages] = line;

    pMessages[uNumMessages].eType = msg;
    pMessages[uNumMessages].param = param;
    pMessages[uNumMessages++].field_8 = a4;
  }
}


//----- (0042EBDB) --------------------------------------------------------
int stru193_math::Sin(int angle)
{
  return Cos(angle - this->uIntegerHalfPi);
}

//----- (0042ECB5) --------------------------------------------------------
void _42ECB5_PlayerAttacksActor()
{
  //unsigned int v0; // ebx@1
  //Player *v1; // esi@1
  //bool result; // eax@1
  //int v3; // edi@2
  //unsigned int v4; // eax@7
  char *v5; // eax@8
  //int v6; // ecx@9
  //signed int v7; // eax@16
  //Actor *v8; // edi@20
  unsigned int v9; // ecx@21
  //char *v10; // eax@26
  char *v11; // eax@26
  unsigned int v12; // eax@47
  //char *v13; // eax@47
  //char *v14; // eax@47
  //unsigned int v15; // ebx@54
  //int v16; // [sp-10h] [bp-4Ch]@24
  //int v17; // [sp-10h] [bp-4Ch]@44
  //unsigned int v18; // [sp-Ch] [bp-48h]@24
  //unsigned int v19; // [sp-Ch] [bp-48h]@44
  //__int16 v20; // [sp-8h] [bp-44h]@24
  //__int16 v21; // [sp-8h] [bp-44h]@44
  //int v22; // [sp-4h] [bp-40h]@24
  //int v23; // [sp-4h] [bp-40h]@44
  SoundID v24; // [sp-4h] [bp-40h]@58
  //Vec3_int_ a3; // [sp+Ch] [bp-30h]@19
  //unsigned int a2; // [sp+18h] [bp-24h]@20
  //unsigned int v27; // [sp+1Ch] [bp-20h]@1
  //int v28; // [sp+20h] [bp-1Ch]@9
  //unsigned int *v28b;
  //int v29; // [sp+24h] [bp-18h]@16
  //int v30; // [sp+28h] [bp-14h]@16
  //int v31; // [sp+2Ch] [bp-10h]@4
  //int v32; // [sp+30h] [bp-Ch]@7
  //int v33; // [sp+34h] [bp-8h]@7
  //int v34; // [sp+38h] [bp-4h]@17

  //v0 = uActiveCharacter;
  //v27 = 6972 * uActiveCharacter;
  //v1 = &pParty->pPlayers[uActiveCharacter-1];
  //result = pParty->pPlayers[uActiveCharacter-1].CanAct();
  Player* player = &pParty->pPlayers[uActiveCharacter - 1];
  if (!player->CanAct())
    return;

  CastSpellInfoHelpers::_427D48();
    //v3 = 0;
  if (pParty->Invisible())
    pParty->pPartyBuffs[PARTY_BUFF_INVISIBILITY].Reset();

    //v31 = player->pEquipment.uBow;
  int bow_idx = player->pEquipment.uBow;
  if (bow_idx && player->pInventoryItemList[bow_idx - 1].IsBroken())
    bow_idx = 0;

    //v32 = 0;
  int wand_item_id = 0;
    //v33 = 0;
    //v4 = v1->pEquipment.uMainHand;
  int laser_weapon_item_id = 0;

  int main_hand_idx = player->pEquipment.uMainHand;
  if (main_hand_idx)
  {
    ItemGen* item = &player->pInventoryItemList[main_hand_idx - 1];
      //v5 = (char *)v1 + 36 * v4;
    if (!item->IsBroken())
    {
		//v28b = &v1->pInventoryItems[v4].uItemID;
        //v6 = v1->pInventoryItems[v4].uItemID;//*((int *)v5 + 124);
      if (item->GetItemEquipType() == EQUIP_WAND)
      {
        if (item->uNumCharges <= 0)
          player->pEquipment.uMainHand = 0; // wand discharged - unequip
        else
          wand_item_id = item->uItemID;//*((int *)v5 + 124);
      }
      else if (item->uItemID == ITEM_BLASTER || item->uItemID == ITEM_LASER_RIFLE)
        laser_weapon_item_id = item->uItemID;//*((int *)v5 + 124);
    }
  }

    //v30 = 0;
    //v29 = 0;
    //v28 = 0;
    //v7 = pMouse->uPointingObjectID;

  int target_pid = pMouse->uPointingObjectID;
  int target_type = PID_TYPE(target_pid),
      target_id = PID_ID(target_pid);
  if (target_type != OBJECT_Actor || !pActors[target_id].CanAct())
  {
    target_pid = stru_50C198.FindClosestActor(5120, 0, 0);
    target_type = PID_TYPE(target_pid);
    target_id = PID_ID(target_pid);
  }

  Actor* actor = &pActors[target_id];
  int actor_distance = 0;
  if (target_type == OBJECT_Actor)
  {
    int distance_x = actor->vPosition.x - pParty->vPosition.x,
        distance_y = actor->vPosition.y - pParty->vPosition.y,
        distance_z = actor->vPosition.z - pParty->vPosition.z;
    actor_distance = integer_sqrt(distance_x * distance_x + distance_y * distance_y + distance_z * distance_z) - actor->uActorRadius;
    if (actor_distance < 0)
      actor_distance = 0;
  }

  bool shooting_bow = false,
       shotting_laser = false,
       shooting_wand = false,
       melee_attack = false;
  if (laser_weapon_item_id)
  {
    shotting_laser = true;
    _42777D_CastSpell_UseWand_ShootArrow(SPELL_LASER_PROJECTILE, uActiveCharacter - 1, 0, 0, uActiveCharacter + 8);
  }
  else if (wand_item_id)
  {
    shooting_wand = true;

    int main_hand_idx = player->pEquipment.uMainHand;
    _42777D_CastSpell_UseWand_ShootArrow(wand_spell_ids[player->pInventoryItemList[main_hand_idx - 1].uItemID - ITEM_WAND_FIRE], uActiveCharacter - 1, 8, 0, uActiveCharacter + 8);

    if (!--player->pInventoryItemList[main_hand_idx - 1].uNumCharges)
      player->pEquipment.uMainHand = 0;
  }
  else if (target_type == OBJECT_Actor && actor_distance <= 407.2)
  {
    melee_attack = true;

    Vec3_int_ a3;
    a3.x = actor->vPosition.x - pParty->vPosition.x;
    a3.y = actor->vPosition.y - pParty->vPosition.y;
    a3.z = actor->vPosition.z - pParty->vPosition.z;
    Vec3_int_::Normalize(&a3.x, &a3.y, &a3.z);

    DamageMonsterFromParty(PID(OBJECT_Player, uActiveCharacter - 1), target_id, &a3);
    if (player->WearsItem(ITEM_ARTIFACT_SPLITTER, EQUIP_TWO_HANDED) || player->WearsItem(ITEM_ARTIFACT_SPLITTER, EQUIP_SINGLE_HANDED))
          _42FA66_do_explosive_impact(
            actor->vPosition.x,
            actor->vPosition.y,
            actor->vPosition.z + actor->uActorHeight / 2,
            0, 512, uActiveCharacter);
  }
  else if (bow_idx)
  {
    shooting_bow = true;
    _42777D_CastSpell_UseWand_ShootArrow(SPELL_BOW_ARROW, uActiveCharacter - 1, 0, 0, 0);
  }
  else
  {
    melee_attack = true;
    ; // actor out of range or no actor; no ranged weapon so melee attacking air
  }

  if (!pParty->bTurnBasedModeOn && melee_attack) // wands, bows & lasers will add recovery while shooting spell effect
  {
    int recovery = player->GetAttackRecoveryTime(false);
    if (recovery < 30 )
      recovery = 30;
    player->SetRecoveryTime(flt_6BE3A4_debug_recmod1 * (double)recovery * 2.133333333333333);
  }

  
  int v34 = 0;
  if (shooting_wand)
    return;
  else if (shooting_bow)
  {
    v34 = 5;
    player->PlaySound(SPEECH_50, 0);
  }
  if (shotting_laser)
    v34 = 7;
  else
  {
    int main_hand_idx = player->pEquipment.uMainHand;
    if (player->HasItemEquipped(EQUIP_TWO_HANDED))
      v34 = player->pInventoryItemList[main_hand_idx - 1].GetPlayerSkillType();
    pTurnEngine->ApplyPlayerAction();
  }

  switch (v34)
  {
    case 0: pAudioPlayer->PlaySound(SOUND_81, 0, 0, -1, 0, 0, 0, 0); break;
    case 1: pAudioPlayer->PlaySound(SOUND_84, 0, 0, -1, 0, 0, 0, 0); break;
    case 2: pAudioPlayer->PlaySound(SOUND_85, 0, 0, -1, 0, 0, 0, 0); break;
    case 3: pAudioPlayer->PlaySound(SOUND_78, 0, 0, -1, 0, 0, 0, 0); break;
    case 4: pAudioPlayer->PlaySound(SOUND_80, 0, 0, -1, 0, 0, 0, 0); break;
    case 5: pAudioPlayer->PlaySound(SOUND_71, 0, 0, -1, 0, 0, 0, 0); break;
    case 6: pAudioPlayer->PlaySound(SOUND_83, 0, 0, -1, 0, 0, 0, 0); break;
    case 7: pAudioPlayer->PlaySound(SOUND_67, 0, 0, -1, 0, 0, 0, 0); break;
  }
}

//----- (0042F184) --------------------------------------------------------
int stru319::FindClosestActor(int pick_depth, int a3, int a4)
{
  int v4; // edi@1
  stru319 *v5; // esi@1
  int v6; // eax@2
  int v7; // eax@4
  int result; // eax@5
  int *v9; // edx@8
  signed int v10; // ebx@10
  int v11; // edi@11
  Actor *v12; // esi@12
  unsigned __int16 v13; // ax@12
  int v14; // eax@22
  char v15; // zf@30
  int v16; // esi@32
  int v17; // ecx@34
  stru319 *v18; // eax@39
  int v19; // edx@39
  int v20; // ecx@41
  unsigned __int16 v21; // ax@42
  unsigned int v22; // [sp+8h] [bp-24h]@11
  unsigned int v23; // [sp+Ch] [bp-20h]@7
  stru319 *v24; // [sp+10h] [bp-1Ch]@1
  unsigned int v25; // [sp+14h] [bp-18h]@8
  int *v26; // [sp+18h] [bp-14h]@8
  int v27; // [sp+1Ch] [bp-10h]@10
  int *v28; // [sp+20h] [bp-Ch]@10
  unsigned int v29; // [sp+24h] [bp-8h]@7
  int v30; // [sp+28h] [bp-4h]@6
  int i; // [sp+38h] [bp+Ch]@33
  signed int v32; // [sp+3Ch] [bp+10h]@32

  v4 = 0;
  v5 = this;
  v24 = this;
  if ( pRenderer->pRenderD3D )
  {
    v6 = a3 != 0;
    if ( a4 )
      LOBYTE(v6) = v6 | 8;
    v7 = pGame->pVisInstance->PickClosestActor(OBJECT_Actor, pick_depth, v6, 657456, -1);
    if ( v7 != -1 )
      return (unsigned __int16)v7;
    else return 0;
  }
  else
  {
    v30 = 0;
    if ( pRenderer->pActiveZBuffer )
    {
      v29 = viewparams->uScreen_topL_X;
      v23 = viewparams->uScreen_BttmR_X;
      if ( (signed int)viewparams->uScreen_topL_Y < (signed int)viewparams->uScreen_BttmR_Y )
      {
        v25 = viewparams->uScreen_BttmR_Y - viewparams->uScreen_topL_Y;
        v9 = &pRenderer->pActiveZBuffer[viewparams->uScreen_topL_X + 640 * viewparams->uScreen_topL_Y];
        v26 = &pRenderer->pActiveZBuffer[viewparams->uScreen_topL_X + 640 * viewparams->uScreen_topL_Y];
        do
        {
          if ( (signed int)v29 < (signed int)v23 )
          {
            v28 = v9;
            v10 = v4;
            v27 = v23 - v29;
            do
            {
              v22 = *v28;
              v11 = *v28 & 0xFFFF;
              if (PID_TYPE(v11) == OBJECT_Actor)
              {
                v12 = &pActors[PID_ID(v11)];
                v13 = v12->uAIState;
                if ( v13 != 5 )
                {
                  if ( v13 != 4
                    && v13 != 11
                    && v13 != 19
                    && v13 != 17
                    && (!a3 || pActors[PID_ID(v11)].GetActorsRelation(0)) )
                  {
                    if ( (!a4 || MonsterStats::BelongsToSupertype(v12->pMonsterInfo.uID, MONSTER_SUPERTYPE_UNDEAD))
                      && v22 <= pick_depth << 16 )
                    {
                      v14 = 0;
                      if ( v10 > 0 )
                      {
                        do
                        {
                          if ( dword_50BDA0[v14] == v11 )
                            break;
                          ++v14;
                        }
                        while ( v14 < v30 );
                      }
                      if ( v14 == v30 && v10 < 100 )
                      {
                        ++v30;
                        dword_50BC10[v10] = v22;
                        dword_50BDA0[v10] = v11;
                        ++v10;
                      }
                    }
                  }
                }
              }
              ++v28;
              --v27;
            }
            while ( v27 );
            v4 = v30;
            v5 = v24;
          }
          v9 = v26 + 640;
          v15 = v25-- == 1;
          v26 += 640;
        }
        while ( !v15 );
      }
      if ( v4 > 0 )
      {
        v32 = 1;
        v16 = (int)dword_50BC10.data();
        do
        {
          for ( i = v32; i < v4; ++i )
          {
            v17 = dword_50BC10[i];
            if ( v17 < *(int *)v16 )
            {
              dword_50BC10[i] = *(int *)v16;
              *(int *)v16 = v17;
            }
          }
          ++v32;
          v16 += 4;
        }
        while ( v32 - 1 < v4 );
        v5 = v24;
        if ( v4 > 0 )
        {
          v18 = v24;
          v19 = v4;
          do
          {
            *(int *)&v18->field_0 = (*(int *)&v18[(char *)dword_50BC10.data() - (char *)v24].field_0 >> 3) & 0x1FFF;
            v18 += 4;
            --v19;
          }
          while ( v19 );
        }
      }
      v20 = 0;
      for ( *(int *)&v5[2000].field_0 = v4; v20 < v4; ++v20 )
      {
        v21 = pActors[*(int *)&v5[4 * v20].field_0].uAIState;
        if ( v21 != 4 && v21 != 5 )
          break;
      }
      if ( v20 != v4 )
      {
        result = 8 * *(int *)&v5[4 * v20].field_0;
        LOBYTE(result) = result | 3;
        return result;
      }
    }
  }
  return 0;
}

//----- (0042F3D6) --------------------------------------------------------
void InitializeTurnBasedAnimations(void *_this)
{
  unsigned int *v1; // edi@1

  pIconIDs_Turn[0] = pIconsFrameTable->FindIcon("turn0");
  pIconIDs_Turn[1] = pIconsFrameTable->FindIcon("turn1");
  pIconIDs_Turn[2] = pIconsFrameTable->FindIcon("turn2");
  pIconIDs_Turn[3] = pIconsFrameTable->FindIcon("turn3");
  pIconIDs_Turn[4] = pIconsFrameTable->FindIcon("turn4");
  uIconID_TurnStop = pIconsFrameTable->FindIcon("turnstop");
  uIconID_TurnHour = pIconsFrameTable->FindIcon("turnhour");
  uIconID_TurnStart = pIconsFrameTable->FindIcon("turnstart");
  uIconID_CharacterFrame = pIconsFrameTable->FindIcon("aframe1");
  uSpriteID_Spell11 = pSpriteFrameTable->FastFindSprite("spell11");
  v1 = pIconIDs_Turn.data();
  do
  {
    pIconsFrameTable->InitializeAnimation(*v1);
    ++v1;
  }
  while ( (signed int)v1 < (signed int)&uIconID_TurnStop );
  pIconsFrameTable->InitializeAnimation(uIconID_TurnHour);
  pIconsFrameTable->InitializeAnimation(uIconID_TurnStop);
  pIconsFrameTable->InitializeAnimation(uIconID_TurnStart);
  pIconsFrameTable->InitializeAnimation(uIconID_CharacterFrame);
}

//----- (0042F7EB) --------------------------------------------------------
bool __fastcall sub_42F7EB_DropItemAt(unsigned int uSpriteID, int x, int y, int z, int a4, int count, int a7, unsigned __int16 attributes, ItemGen *a9)
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
  if ( (signed int)pObjectList->uNumObjects <= 0 )
    pObjectDescID = 0;
  else
  {
    pObjectDescID = 0;
    for ( uint i = 0; i < (signed int)pObjectList->uNumObjects; ++i )
    {
      if ( (short)uSpriteID == pObjectList->pObjects[i].uObjectID )
        pObjectDescID = i;
    }
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
void __fastcall sub_42F960_create_object(int x, int y, int z)
{
  int v3; // ebx@1
  int v4; // edi@1
  signed int v5; // edx@1
  char *v6; // ecx@2
  unsigned __int16 v7; // ax@5
  signed int v8; // eax@6
  signed int v9; // eax@7
  //SpriteObject a1; // [sp+Ch] [bp-70h]@1

  v3 = x;
  v4 = y;
  
  SpriteObject a1; // [sp+Ch] [bp-70h]@1
  //SpriteObject::SpriteObject(&a1);
  a1.stru_24.Reset();

  v5 = 0;
  a1.spell_skill = 0;
  a1.spell_level = 0;
  a1.spell_id = 0;
  a1.field_54 = 0;
  a1.uType = 800;
  if ( (signed int)pObjectList->uNumObjects <= 0 )
  {
LABEL_5:
    v7 = 0;
  }
  else
  {
    v6 = (char *)&pObjectList->pObjects->uObjectID;
    while ( *(short *)v6 != 800 )
    {
      ++v5;
      v6 += 56;
      if ( v5 >= (signed int)pObjectList->uNumObjects )
        goto LABEL_5;
    }
    v7 = v5;
  }
  a1.uObjectDescID = v7;
  a1.vPosition.x = v3;
  a1.vPosition.y = v4;
  a1.vPosition.z = z;
  a1.uSoundID = 0;
  a1.uAttributes = 0;
  a1.uSectorID = pIndoor->GetSector(v3, v4, z);
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



//----- (0042FA66) --------------------------------------------------------
void _42FA66_do_explosive_impact(int a1, int a2, int a3, int a4, __int16 a5, signed int a6)
{
  int v6; // edi@1
  int v7; // esi@1
  char *v8; // ecx@2
  unsigned __int16 v9; // ax@5
  //int v10; // eax@10
  //signed int result; // eax@11
  //__int16 v12; // ax@12
  //SpriteObject a1a; // [sp+Ch] [bp-74h]@1
  int v14; // [sp+7Ch] [bp-4h]@1

  v6 = a1;
  v7 = a2;
  SpriteObject a1a; // [sp+Ch] [bp-74h]@1
  //SpriteObject::SpriteObject(&a1a);
  a1a.uType = 600;
  a1a.stru_24.Reset();

  a1a.spell_id = SPELL_FIRE_FIREBALL;
  a1a.spell_level = 8;
  a1a.spell_skill = 3;
  v14 = 0;
  if ( (signed int)pObjectList->uNumObjects <= 0 )
  {
LABEL_5:
    v9 = 0;
  }
  else
  {
    v8 = (char *)&pObjectList->pObjects->uObjectID;
    while ( (short)a1a.uType != *(short *)v8 )
    {
      ++v14;
      v8 += 56;
      if ( v14 >= (signed int)pObjectList->uNumObjects )
        goto LABEL_5;
    }
    v9 = v14;
  }
  a1a.uObjectDescID = v9;
  a1a.vPosition.x = v6;
  a1a.vPosition.y = v7;
  a1a.vPosition.z = a3;
  a1a.uAttributes = 0;
  a1a.uSectorID = pIndoor->GetSector(v6, v7, a3);
  a1a.uSpriteFrameID = 0;
  a1a.spell_target_pid = 0;
  a1a.field_60_distance_related_prolly_lod = 0;
  a1a.uFacing = 0;
  a1a.uSoundID = 0;
  if ( a6 >= 1 || a6 <= 4 )
  {
    a1a.spell_caster_pid = PID(OBJECT_Player, a6 - 1);
  }
  else
  {
    a1a.spell_caster_pid = 0;
  }

  int id = a1a.Create(0, 0, 0, 0);
  if (id != -1)
    stru_50FE08.Add(PID(OBJECT_Item, id),
               a5,
               SLOWORD(a1a.vPosition.x),
               SLOWORD(a1a.vPosition.y),
               SLOWORD(a1a.vPosition.z),
               0,
               0);
}

//----- (0042FB5C) --------------------------------------------------------
bool _42FB5C_check_spell(signed int a1)
{
  int v1; // ecx@3
  int v2; // ecx@4
  int v3; // ecx@5
  int v4; // ecx@6
  int v5; // ecx@7
  int v6; // ecx@8
  char v7; // zf@9

  if ( a1 > 39 )
  {
    if ( a1 > 77 )
    {
      if ( a1 == 80 )
        return 0;
      if ( a1 > 84 )
      {
        if ( a1 <= 86 )
          return 0;
        v7 = a1 == 95;
        goto LABEL_20;
      }
    }
    else
    {
      if ( a1 == 77 )
        return 0;
      if ( a1 >= 46 )
      {
        if ( a1 <= 47 || a1 == 51 )
          return 0;
        v7 = a1 == 73;
        goto LABEL_20;
      }
    }
    return 1;
  }
  if ( a1 == 39 || (v1 = a1 - 2) == 0 )
    return 1;
  v2 = v1 - 3;
  if ( !v2 )
    return 0;
  v3 = v2 - 1;
  if ( !v3 || (v4 = v3 - 3) == 0 )
    return 1;
  v5 = v4 - 8;
  if ( !v5 )
    return 0;
  v6 = v5 - 1;
  if ( !v6 )
    return 1;
  v7 = v6 == 20;
LABEL_20:
  if ( !v7 )
    return 1;
  return 0;
}

//----- (0042FBDD) --------------------------------------------------------
void  sub_42FBDD()
{
  pAudioPlayer->PlaySound(SOUND_Button2, 0, 0, -1, 0, 0, 0, 0);
  pRenderer->DrawTextureTransparent(pBtn_YES->uX, pBtn_YES->uY, pBtn_YES->pTextures[0]);
  pRenderer->Present();
}

//----- (0042FC15) --------------------------------------------------------
void  CloseWindowBackground()
{
  pAudioPlayer->PlaySound(SOUND_Button2, -2, 0, -1, 0, 0, 0, 0);
  pRenderer->DrawTextureTransparent(pBtn_ExitCancel->uX, pBtn_ExitCancel->uY, pBtn_ExitCancel->pTextures[0]);
  pRenderer->Present();
}

//----- (0042FC4E) --------------------------------------------------------
void ProcessInputActions()
{
  char v4; // al@9
  char v8; // bl@100
  unsigned __int16 v9; // ax@102
  int v10; // eax@103
  char v14; // al@159
  unsigned int v15; // eax@168
  PartyAction partyAction; // [sp-14h] [bp-1Ch]@20
  InputAction inputAction; // [sp+0h] [bp-8h]@7
  int v24; // [sp+4h] [bp-4h]@87

  pGame->pKeyboardInstance->EnterCriticalSection();
  Keyboard* pKeyboard = pGame->pKeyboardInstance;
  if (!bAlwaysRun)
  {
    if (pKeyboard->IsShiftHeld())
      pParty->uFlags2 |= PARTY_FLAGS_2_RUNNING;
    else
      pParty->uFlags2 &= ~PARTY_FLAGS_2_RUNNING;
   }
  else
  {
    if (pKeyboard->IsShiftHeld())
      pParty->uFlags2 &= ~PARTY_FLAGS_2_RUNNING;
    else
      pParty->uFlags2 |= PARTY_FLAGS_2_RUNNING;
  }

  //pParty->uFlags2 |= PARTY_FLAGS_2_RUNNING;


    //  WUT? double event trigger
  /*for ( uint i = 0; i < 30; ++i )
  {
    if ( pKeyActionMap->pToggleTypes[i] )
      v14 = pGame->pKeyboardInstance->WasKeyPressed(pKeyActionMap->pVirtualKeyCodesMapping[i]);
    else
      v14 = pGame->pKeyboardInstance->IsKeyBeingHeld(pKeyActionMap->pVirtualKeyCodesMapping[i]);
    if ( v14 )
    {
      if (pCurrentScreen == SCREEN_GAME)
      {
        pMessageQueue_50CBD0->AddMessage(UIMSG_Game_Action, 0, 0);
        continue;
      }
      if ( pCurrentScreen == SCREEN_NPC_DIALOGUE || pCurrentScreen == SCREEN_BRANCHLESS_NPC_DIALOG )
      {
        v15 = pMessageQueue_50CBD0->uNumMessages;
        if ( pMessageQueue_50CBD0->uNumMessages )
        {
          v15 = 0;
          if ( pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].field_8 )
          {
            v15 = 1;
            pMessageQueue_50CBD0->uNumMessages = 0;
            pMessageQueue_50CBD0->pMessages[v15].eType = UIMSG_Escape;
            pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].param = 0;
            *(&pMessageQueue_50CBD0->uNumMessages + 3 * pMessageQueue_50CBD0->uNumMessages + 3) = 0;
            ++pMessageQueue_50CBD0->uNumMessages;
            continue;
          }
          pMessageQueue_50CBD0->uNumMessages = 0;
        }
        //pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 0, 0);
      }
    }
  }*/
  if ( !pEventTimer->bPaused )
  {
    for ( uint i = 0; i < 30; ++i )
    {
      inputAction = (InputAction)i;
      if ( pKeyActionMap->pToggleTypes[(InputAction)i] )
        v4 = pKeyboard->WasKeyPressed(pKeyActionMap->pVirtualKeyCodesMapping[(InputAction)i]);
      else
        v4 = pKeyboard->IsKeyBeingHeld(pKeyActionMap->pVirtualKeyCodesMapping[(InputAction)i]);
      if ( v4 )
      {
        switch ( inputAction )
        {
          case INPUT_MoveForward:
            if (pCurrentScreen  != SCREEN_GAME)
              break;
            if (!pParty->bTurnBasedModeOn)
            {
              if ( pParty->uFlags2 & PARTY_FLAGS_2_RUNNING)
                partyAction = PARTY_RunForward;
              else
                partyAction = PARTY_WalkForward;
              pPartyActionQueue->Add(partyAction);
              break;
            }
            if (pTurnEngine->turn_stage != 1 && pTurnEngine->turn_stage != 2 && pTurnEngine->uActionPointsLeft > 0 )
            {
              pTurnEngine->uActionPointsLeft -= 26;
              if ( pParty->uFlags2 & PARTY_FLAGS_2_RUNNING)
                partyAction = PARTY_RunForward;
              else
                partyAction = PARTY_WalkForward;
              pPartyActionQueue->Add(partyAction);
              break;
            }
            break;
          case INPUT_MoveBackwards:
            if (pCurrentScreen  != SCREEN_GAME)
              break;
            if (!pParty->bTurnBasedModeOn)
            {
              if ( pParty->uFlags2 & 2 )
                partyAction = PARTY_RunBackward;
              else
                partyAction = PARTY_WalkBackward;
              pPartyActionQueue->Add(partyAction);
              break;
            }
            if ( pTurnEngine->turn_stage != 1 && pTurnEngine->turn_stage != 2 && pTurnEngine->uActionPointsLeft > 0 )
            {
              pTurnEngine->uActionPointsLeft -= 26;
              if ( pParty->uFlags2 & 2 )
                partyAction = PARTY_RunBackward;
              else
                partyAction = PARTY_WalkBackward;
              pPartyActionQueue->Add(partyAction);
              break;
            }
            break;
          case INPUT_StrafeLeft:
            if (pCurrentScreen  != SCREEN_GAME)
              break;
            if (!pParty->bTurnBasedModeOn)
            {
              partyAction = PARTY_StrafeLeft;
              pPartyActionQueue->Add(partyAction);
              break;
            }
            if ( pTurnEngine->turn_stage == 1 || pTurnEngine->turn_stage == 2 || pTurnEngine->uActionPointsLeft <= 0 )
              break;
            pTurnEngine->uActionPointsLeft -= 26;
            partyAction = PARTY_StrafeLeft;
            pPartyActionQueue->Add(partyAction);
            break;
          case INPUT_StrafeRight:
            if (pCurrentScreen != SCREEN_GAME)
              break;
            if (!pParty->bTurnBasedModeOn)
            {
              partyAction = PARTY_StrafeRight;
              pPartyActionQueue->Add(partyAction);
              break;
            }
            if ( pTurnEngine->turn_stage == 1 || pTurnEngine->turn_stage == 2 || pTurnEngine->uActionPointsLeft <= 0 )
              break;
            pTurnEngine->uActionPointsLeft -= 26;
            partyAction = PARTY_StrafeRight;
            pPartyActionQueue->Add(partyAction);
            break;
          case INPUT_TurnLeft:
            if (pCurrentScreen != SCREEN_GAME)
              break;
            if ( GetAsyncKeyState(VK_CONTROL) ) // strafing
            {
              if (pParty->bTurnBasedModeOn)
              {
                if ( pTurnEngine->turn_stage == 1 || pTurnEngine->turn_stage == 2 || pTurnEngine->uActionPointsLeft <= 0 )
                  break;
                pTurnEngine->uActionPointsLeft -= 26;
              }
              partyAction = PARTY_StrafeLeft;
            }
            else
            {
              if ( pParty->uFlags2 & 2 )
                partyAction = PARTY_FastTurnLeft;
              else
                partyAction = PARTY_TurnLeft;
            }
            pPartyActionQueue->Add(partyAction);
            if (uCurrentlyLoadedLevelType == LEVEL_Outdoor && pWeather->bRenderSnow)
            {
              pWeather->OnPlayerTurn(10);
            }
            break;
          case INPUT_TurnRight:
            if (pCurrentScreen != SCREEN_GAME)
              break;
            if ( GetAsyncKeyState(17) )         // strafing
            {
              if (pParty->bTurnBasedModeOn)
              {
                if ( pTurnEngine->turn_stage == 1 || pTurnEngine->turn_stage == 2 || pTurnEngine->uActionPointsLeft <= 0 )
                  break;
                pTurnEngine->uActionPointsLeft -= 26;
              }
              partyAction = PARTY_StrafeRight;
            }
            else
            {
              if ( pParty->uFlags2 & 2 )
                partyAction = PARTY_FastTurnRight;
              else
                partyAction = PARTY_TurnRight;
            }
            pPartyActionQueue->Add(partyAction);
            if (uCurrentlyLoadedLevelType == LEVEL_Outdoor && pWeather->bRenderSnow)
            {
              pWeather->OnPlayerTurn(-10);
            }
            break;
          case INPUT_Jump:
            if (pCurrentScreen != SCREEN_GAME || pParty->bTurnBasedModeOn)
              break;
            partyAction = (PartyAction)12;
            pPartyActionQueue->Add(partyAction);
            break;
          case INPUT_Yell:
            if (!pCurrentScreen && uActiveCharacter)
            {
              pParty->Yell();
              pPlayers[uActiveCharacter]->PlaySound(SPEECH_Yell, 0);
            }
          break;
          case INPUT_Pass:
            if ( pCurrentScreen )
              break;
            if (pParty->bTurnBasedModeOn && pTurnEngine->turn_stage == 3)
            {
              pTurnEngine->field_18 |= TE_FLAG_8;
              break;
            }
            if ( uActiveCharacter )
            {
              if ( !pPlayers[uActiveCharacter]->uTimeToRecovery )
              {
                v24 = pPlayers[uActiveCharacter]->GetAttackRecoveryTime(false);
                if ( !pParty->bTurnBasedModeOn )
                  pPlayers[uActiveCharacter]->SetRecoveryTime((signed __int64)(flt_6BE3A4_debug_recmod1 * (double)v24 * 2.133333333333333));
                CastSpellInfoHelpers::_427D48();
                pTurnEngine->ApplyPlayerAction();
              }
            }
            break;
          case INPUT_Combat://if press ENTER
            if (pCurrentScreen == SCREEN_GAME)
            {
              if (pParty->bTurnBasedModeOn)
              {
                if (pTurnEngine->turn_stage == 3 || PID_TYPE(pTurnEngine->pQueue[0].uPackedID) == OBJECT_Player)
                {
                  pParty->bTurnBasedModeOn = 0;
                  pTurnEngine->End(true);
                }
              }
              else
              {
                pTurnEngine->Start();
                pParty->bTurnBasedModeOn = true;
              }
            }
            break;
          case INPUT_CastReady:
            if (pCurrentScreen != SCREEN_GAME)
              break;
            if (pParty->bTurnBasedModeOn && pTurnEngine->turn_stage == 3)
            {
              pTurnEngine->field_18 |= TE_FLAG_8;
              break;
            }
            if ( !uActiveCharacter )
              break;
            v8 = pPlayers[uActiveCharacter]->uQuickSpell;
            if ( !v8
              || bUnderwater
              || ((v9 = pPlayers[uActiveCharacter]->pActiveSkills[(unsigned __int8)v8 / 11 + 12], !(HIBYTE(v9) & 1)) ? 
                 ((v9 & 0x80u) == 0 ? (!(v9 & 0x40) ? (v10 = *(&pSpellDatas[0].uNormalLevelMana + 10 * (unsigned __int8)v8)) : 
                 (v10 = *(&pSpellDatas[0].uExpertLevelMana + 10 * (unsigned __int8)v8))) : 
                 (v10 = *(&pSpellDatas[0].uMasterLevelMana + 10 * (unsigned __int8)v8))) : 
                 (v10 = *(&pSpellDatas[0].uMagisterLevelMana + 10 * (unsigned __int8)v8)),
                  v10 > pPlayers[uActiveCharacter]->sMana) )
            {
              pPartyActionQueue = pPartyActionQueue;
              pMessageQueue_50CBD0->AddMessage(UIMSG_Attack, 0, 0);
              break;
            }
            else
            {
            pMessageQueue_50C9E8->AddMessage(UIMSG_CastQuickSpell, 0, 0);
            }
            break;
          case INPUT_Attack:
            if (pCurrentScreen != SCREEN_GAME)
              break;
            if (pParty->bTurnBasedModeOn == 1 && pTurnEngine->turn_stage == 3)
            {
              pTurnEngine->field_18 |= TE_FLAG_8;
              break;
            }
            pMessageQueue_50CBD0->AddMessage(UIMSG_Attack, 0, 0);
            break;
          case INPUT_EventTrigger:
            if (pCurrentScreen == SCREEN_GAME)
            {
              pMessageQueue_50CBD0->AddMessage(UIMSG_Game_Action, 0, 0);
              break;
            }
            if ( pCurrentScreen == SCREEN_NPC_DIALOGUE )
            {
              if ( pMessageQueue_50CBD0->uNumMessages )
              {
                pMessageQueue_50CBD0->uNumMessages = 0;
                if ( pMessageQueue_50CBD0->pMessages[0].field_8 )
                {
                  pMessageQueue_50CBD0->uNumMessages = 1;
                  pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].eType = UIMSG_Escape;
                  pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].param = 0;
                  pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].field_8 = 0;
                  ++pMessageQueue_50CBD0->uNumMessages;
                  break;
                }
                break;
              }
              pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 0, 0);
            }
            break;
          case INPUT_CharCycle:
            if ( pCurrentScreen == SCREEN_SPELL_BOOK  )
              break;

            pMessageQueue_50C9E8->AddMessage(UIMSG_CycleCharacters, 0, 0);
            break;
          case INPUT_LookUp:
            if ( pEventTimer->bPaused )
              break;
            partyAction = (PartyAction)7;
            pPartyActionQueue->Add(partyAction);
            break;
          case INPUT_CenterView:
            if ( pEventTimer->bPaused )
              break;
            partyAction = (PartyAction)9;
            pPartyActionQueue->Add(partyAction);
            break;
          case INPUT_LookDown:
            if ( pEventTimer->bPaused )
              break;
            partyAction = (PartyAction)8;
            pPartyActionQueue->Add(partyAction);
            break;
          case INPUT_FlyUp:
            if ( pCurrentScreen || pEventTimer->bPaused )
              break;
            partyAction = (PartyAction)13;
            pPartyActionQueue->Add(partyAction);
            break;
          case INPUT_Land:
            if ( pCurrentScreen || pEventTimer->bPaused )
              break;
            partyAction = (PartyAction)15;
            pPartyActionQueue->Add(partyAction);
            break;
          case INPUT_FlyDown:
            if ( !pCurrentScreen
              && !pEventTimer->bPaused )
            {
              partyAction = (PartyAction)14;
              pPartyActionQueue->Add(partyAction);
            }
            break;
          case INPUT_ZoomIn:
              pMessageQueue_50C9E8->AddMessage(UIMSG_ClickZoomOutBtn, 0, 0);
            break;
          case INPUT_ZoomOut:
              pMessageQueue_50C9E8->AddMessage(UIMSG_ClickZoomInBtn, 0, 0);
            break;
          case INPUT_AlwaysRun:
            bAlwaysRun = bAlwaysRun == 0;
            break;
          default:
            break;
        }
      }
    }
  }
}
