#define _CRT_SECURE_NO_WARNINGS

#include "ErrorHandling.h"
#include "VectorTypes.h"
#include "mm7_unsorted_subs.h"
#include "MM7.h"
#include "MapInfo.h"
#include "Game.h"
#include "GUIWindow.h"
#include "GUIFont.h"
#include "Party.h"
#include "AudioPlayer.h"
#include "Outdoor.h"
#include "Outdoor_stuff.h"
#include "LOD.h"
#include "Actor.h"
#include "Events.h"
#include "Viewport.h"
#include "FrameTableInc.h"
#include "OurMath.h"
#include "SpriteObject.h"
#include "ObjectList.h"
#include "DecorationList.h"
#include "Timer.h"
#include "PlayerFrameTable.h"
#include "Awards.h"
#include "texts.h"
#include "UI\UIHouses.h"
#include "Lights.h"
#include "Level/Decoration.h"
#include "mm7_data.h"
#include "UI\UIArena.h"

//----- (0046CC4B) --------------------------------------------------------
void check_event_triggers()
{
  for (size_t i = 0; i < num_event_triggers; i++)
  {
    if (pLevelDecorations[event_triggers[i]].uFlags & LEVEL_DECORATION_TRIGGERED_BY_TOUCH
        && pLevelDecorations[event_triggers[i]].vPosition.GetDistanceTo(pParty->vPosition) < pLevelDecorations[event_triggers[i]].uTriggerRange)
    {
      EventProcessor(pLevelDecorations[event_triggers[i]].uEventID, PID(OBJECT_Decoration,i), 1);
    }
    else if (pLevelDecorations[event_triggers[i]].uFlags & LEVEL_DECORATION_TRIGGERED_BY_MONSTER)
    {
      for (size_t j = 0; j < uNumActors; j++)
      {
        if (pLevelDecorations[event_triggers[i]].vPosition.GetDistanceTo(pActors[j].vPosition) < pLevelDecorations[event_triggers[i]].uTriggerRange)
          EventProcessor(pLevelDecorations[event_triggers[i]].uEventID, 0, 1);
      }
    }
    else if (pLevelDecorations[event_triggers[i]].uFlags & LEVEL_DECORATION_TRIGGERED_BY_OBJECT)
    {
      for (size_t j = 0; j < uNumSpriteObjects; j++)
      {
        if (pLevelDecorations[event_triggers[i]].vPosition.GetDistanceTo(pSpriteObjects[j].vPosition) < pLevelDecorations[event_triggers[i]].uTriggerRange)
          EventProcessor(pLevelDecorations[event_triggers[i]].uEventID, 0, 1);
      }
    }
  }
}
// 6836C8: using guessed type int 6836C8_num_decorations_6807E8;

//----- (0046DEF2) --------------------------------------------------------
unsigned int __fastcall sub_46DEF2(signed int a2, unsigned int uLayingItemID)
{
  unsigned int result; // eax@1

  result = uLayingItemID;
  if ( pObjectList->pObjects[pSpriteObjects[uLayingItemID].uObjectDescID].uFlags & 0x10 )
    result = _46BFFA_check_object_intercept(uLayingItemID, a2);
  return result;
}

//----- (0046E0B2) --------------------------------------------------------
void  _46E0B2_collide_against_decorations()
{
  BLVSector *sector; // ebp@1
  LevelDecoration *decor; // edi@2
  DecorationDesc *decor_desc; // esi@3
  int v8; // ebx@10
  int v9; // esi@11
  int v11; // eax@12
  int v12; // esi@14
  unsigned int v13; // eax@17
  signed int i; // [sp+4h] [bp-14h]@1
  int v15; // [sp+8h] [bp-10h]@10
  int v16; // [sp+Ch] [bp-Ch]@10
  int v17; // [sp+10h] [bp-8h]@10

  sector = &pIndoor->pSectors[stru_721530.uSectorID];
  for ( i = 0; i < sector->uNumDecorations; ++i )
  {
    decor = &pLevelDecorations[sector->pDecorationIDs[i]];
    if (!(decor->uFlags & LEVEL_DECORATION_INVISIBLE))
    {
      decor_desc = &pDecorationList->pDecorations[decor->uDecorationDescID];
      if (!decor_desc->CanMoveThrough())
      {
        if ( stru_721530.sMaxX <= decor->vPosition.x + decor_desc->uRadius && stru_721530.sMinX >= decor->vPosition.x - decor_desc->uRadius 
          && stru_721530.sMaxY <= decor->vPosition.y + decor_desc->uRadius && stru_721530.sMinY >= decor->vPosition.y - decor_desc->uRadius 
          && stru_721530.sMaxZ <= decor->vPosition.z + decor_desc->uDecorationHeight && stru_721530.sMinZ >= decor->vPosition.z )
        {
          v16 = decor->vPosition.x - stru_721530.normal.x;
          v15 = decor->vPosition.y - stru_721530.normal.y;
          v8 = stru_721530.prolly_normal_d + decor_desc->uRadius;
          v17 = ((decor->vPosition.x - stru_721530.normal.x) * stru_721530.direction.y
              - (decor->vPosition.y - stru_721530.normal.y) * stru_721530.direction.x) >> 16;
          if ( abs(v17) <= stru_721530.prolly_normal_d + decor_desc->uRadius )
          {
            v9 = (v16 * stru_721530.direction.x + v15 * stru_721530.direction.y) >> 16;
            if ( v9 > 0 )
            {
              v11 = stru_721530.normal.z + fixpoint_mul(stru_721530.direction.z, v9);
              if ( v11 >= decor->vPosition.z )
              {
                if ( v11 <= decor_desc->uDecorationHeight + decor->vPosition.z )
                {
                  v12 = v9 - integer_sqrt(v8 * v8 - v17 * v17);
                  if ( v12 < 0 )
                    v12 = 0;
                  if ( v12 < stru_721530.field_7C )
                  {
                    stru_721530.field_7C = v12;
                    v13 = 8 * sector->pDecorationIDs[i];
                    LOBYTE(v13) = v13 | 5;
                    stru_721530.uFaceID = v13;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

//----- (00487DA9) --------------------------------------------------------
void sub_487DA9()
{
  for (int i = 0; i < 20000; ++i)
    array_77EC08[i].field_108 = 0;
}

//----- (00491E3A) --------------------------------------------------------
void sub_491E3A()
{
  signed int v1; // esi@3
  unsigned int v3; // eax@7
  unsigned int v4; // edx@8
  int v6; // edi@17
  int v12; // eax@26

  //__debugbreak();//Ritor1
  for ( uint pl = 0; pl < 4; pl++ )
  {
    if (SoundSetAction[24][0])
    {
      v3 = 0;
      for ( v1 = 0; v1 < (signed int)pSoundList->sNumSounds; ++v1 )
      {
        int ps = 2 * (SoundSetAction[24][0] + 50 * pParty->pPlayers[pl].uVoiceID) + 4998;//6728
        if ( pSoundList->pSL_Sounds[v1].uSoundID == 2 * (SoundSetAction[24][0] + 50 * pParty->pPlayers[pl].uVoiceID) + 4998 )
          v3 = v1;
      }
      pSoundList->UnloadSound(v3, 1);
      for ( v4 = 0; (signed int)v4 < (signed int)pSoundList->sNumSounds; ++v4 )
      {
        if ( pSoundList->pSL_Sounds[v4].uSoundID == 2 * (SoundSetAction[24][0] + 50 * pParty->pPlayers[pl].uVoiceID) + 4999 )
          pSoundList->UnloadSound(v4, 1);
      }
    }
  }
  v6 = pIcons_LOD->uNumLoadedFiles - 1;
  if ( v6 >= pIcons_LOD->pFacesLock )
  {
    do
    {
      pIcons_LOD->pTextures[v6].Release();
      if ( pIcons_LOD->pHardwareTextures )
      {
        if ( pIcons_LOD->pHardwareTextures[v6] )
        {
          pIcons_LOD->pHardwareTextures[v6]->Release();
          pIcons_LOD->pHardwareTextures[v6] = 0;
        }
      }
      if ( pIcons_LOD->pHardwareSurfaces )
      {
        if ( pIcons_LOD->pHardwareSurfaces[v6] )
        {
          pIcons_LOD->pHardwareSurfaces[v6]->Release();
          pIcons_LOD->pHardwareSurfaces[v6] = 0;
        }
      }
      --v6;
    }
    while ( v6 >= pIcons_LOD->pFacesLock );
  }
  v12 = pIcons_LOD->pFacesLock;
  pIcons_LOD->pFacesLock = 0;
  pIcons_LOD->uNumLoadedFiles = v12;
}
// 4ED498: using guessed type char byte_4ED498;

//----- (00493938) --------------------------------------------------------
void _493938_regenerate()
{
  int current_time; // edi@1
  int last_reg_time; // qax@1
  int v4; // eax@2
  int v5; // edi@5
  bool cursed_flag; // ecx@5
  char v7; // sf@5
  int *v8; // ecx@10
  int v9; // edi@15
  signed int v10; // eax@15
//  __int16 *v11; // edx@16
//  int v12; // eax@20
  int numberOfActorsAffected; // ebx@20
  unsigned int v14; // esi@21
  //unsigned int v15; // ecx@21
  //unsigned int v16; // eax@21
//  int v18; // eax@21
  signed int v19; // eax@21
  bool recovery_HP; // ebx@25
//  ITEM_EQUIP_TYPE v22; // edi@30
  signed int v25; // eax@33
//  int v26; // eax@35
//  int v27; // eax@36
//  int v28; // eax@37
  signed int v31; // ecx@53
  int actorsAffectedByImmolation[100]; // [sp+4h] [bp-22Ch]@20
  SpriteObject a1; // [sp+194h] [bp-9Ch]@15
  Vec3_int_ a3; // [sp+204h] [bp-2Ch]@15
  bool has_dragon_flag; // [sp+210h] [bp-20h]@22
  bool lich_jar_flag; // [sp+214h] [bp-1Ch]@25
  bool zombie_flag; // [sp+218h] [bp-18h]@25
  bool decrease_HP; // [sp+21Ch] [bp-14h]@25
  bool lich_flag; // [sp+220h] [bp-10h]@25
  int v49; // [sp+224h] [bp-Ch]@24
  bool recovery_SP; // [sp+228h] [bp-8h]@25
  bool redraw_flag; // [sp+22Ch] [bp-4h]@2

  current_time = (signed int)(signed __int64)((double)(signed __int64)pParty->uTimePlayed * 0.234375) / 60;
  last_reg_time = (signed int)(signed __int64)((double)pParty->uLastRegenerationTime * 0.234375) / 60;
  if ( current_time >= (signed int)last_reg_time + 5 )
  {
    redraw_flag = false;
    v4 = (current_time - last_reg_time) / 5;
    if (pParty->FlyActive())
    {
      if ( pParty->bFlying )
      {
        if ( !(pParty->pPartyBuffs[PARTY_BUFF_FLY].uFlags & 1) )
        {
          v5 = v4 * pParty->pPartyBuffs[PARTY_BUFF_FLY].uPower;
          cursed_flag = pParty->pPlayers[pParty->pPartyBuffs[PARTY_BUFF_FLY].uCaster - 1].pConditions[Condition_Cursed];//cursed
          v7 = cursed_flag < v5;
          //cursed_flag -= v5;
          if ( !v7 )
          {
            pParty->uFlags &= 0xFFFFFFBFu;
            pParty->bFlying = false;
            redraw_flag = true;
          }
        }
      }
    }

    if (pParty->WaterWalkActive())
    {
      if (pParty->uFlags & PARTY_FLAGS_1_STANDING_ON_WATER )
      {
        if ( !(pParty->pPartyBuffs[PARTY_BUFF_WATER_WALK].uFlags & 1) )
        { // taking on water
          v8 = (int *)&pParty->pPlayers[pParty->pPartyBuffs[PARTY_BUFF_WATER_WALK].uCaster - 1].pConditions[Condition_Cursed];//&AA1058_PartyQuickSpellSound[4].pSounds[6972 * pParty->pPartyBuffs[PARTY_BUFF_WATER_WALK].uCaster + 2000];
          v7 = *v8 < v4;
          *v8 -= v4;
          if ( v7 )
          {
            *v8 = 0;
            pParty->uFlags &= ~PARTY_FLAGS_1_STANDING_ON_WATER;
            redraw_flag = true;
          }
        }
      }
    }

    if (pParty->ImmolationActive())//Жертва
    {
      a3.z = 0;
      a3.y = 0;
      a3.x = 0;
      a1.stru_24.Reset();
      a1.spell_level = pParty->pPartyBuffs[PARTY_BUFF_IMMOLATION].uPower;
      a1.spell_skill = pParty->ImmolationSkillLevel();
      v10 = 0;
      a1.uType = 1070;
      a1.spell_id = SPELL_FIRE_IMMOLATION;
      v10 = 0;
      for (uint i = 0; i > pObjectList->uNumObjects; i++)
      {
        if (pObjectList->pObjects[i].uObjectID == stru_4E3ACC[8].uType) 
          v10 = i;
      }
      a1.uObjectDescID = v10;
      a1.field_60_distance_related_prolly_lod = 0;
      a1.uAttributes = 0;
      a1.uSectorID = 0;
      a1.uSpriteFrameID = 0;
      a1.spell_caster_pid = PID(OBJECT_Player, pParty->pPartyBuffs[PARTY_BUFF_IMMOLATION].uCaster);
      a1.uFacing = 0;
      a1.uSoundID = 0;
      numberOfActorsAffected = pParty->_46A89E_immolation_effect(actorsAffectedByImmolation, 100, 307);
      for ( v9 = 0; v9 < numberOfActorsAffected; ++v9 )
      {
        v14 = actorsAffectedByImmolation[v9];
        a1.vPosition.x = pActors[v14].vPosition.x;
        a1.vPosition.y = pActors[v14].vPosition.y;
        a1.vPosition.z = pActors[v14].vPosition.z;
        a1.spell_target_pid = PID(OBJECT_Actor,v14);
        v19 = a1.Create(0, 0, 0, 0);
        Actor::DamageMonsterFromParty(PID(OBJECT_Item,v19), v14, &a3);
      }
    }

    has_dragon_flag = false;
    if (PartyHasDragon())
      has_dragon_flag = true;

    for ( v49 = 0; v49 < 4; v49++ )
    {
      recovery_HP = false;
      recovery_SP = false;
      decrease_HP = false;
      lich_flag = false;
      lich_jar_flag = false;
      zombie_flag = false;

      for ( int v22 = 0; (signed int)v22 < 16; v22++ )
      {
        if ( pParty->pPlayers[v49].HasItemEquipped((ITEM_EQUIP_TYPE)v22) )
        {
          uint _idx = pParty->pPlayers[v49].pEquipment.pIndices[v22];
          if ( pParty->pPlayers[v49].pInventoryItemList[_idx - 1].uItemID > 134 )
          {
            if ( pParty->pPlayers[v49].pInventoryItemList[_idx - 1].uItemID == ITEM_RELIC_ETHRICS_STAFF )
              decrease_HP = true;
            if ( pParty->pPlayers[v49].pInventoryItemList[_idx - 1].uItemID == ITEM_ARTIFACT_HERMES_SANDALS )
            {
              recovery_HP = true;
              recovery_SP = true;
            }
            if ( pParty->pPlayers[v49].pInventoryItemList[_idx - 1].uItemID == ITEM_ARTIFACT_MINDS_EYE )
              recovery_SP = true;
            if ( pParty->pPlayers[v49].pInventoryItemList[_idx - 1].uItemID == ITEM_ARTIFACT_HEROS_BELT )
              recovery_HP = true;
          }
          else
          {
            v25 = pParty->pPlayers[v49].pInventoryItemList[_idx - 1].uSpecEnchantmentType;
            if ( v25 == 37 //of Regeneration("Regenerate 1hp/x while walking, etc")
              || v25 == 44 //of Life("HP (+10), Regen hpts")
              || v25 == 50 //of The Phoenix("Fire Res (+30), Regen hpts") && 
              || v25 == 54 )// of The Troll("End (+15), Regen hpts")
               recovery_HP = true;
            if ( v25 == 38 //of Mana("Regenerate 1sp/x while walking, etc")
              || v25 == 47 //of The Eclipse("SP (+10), Regen spts")
              || v25 == 55 )//of The Unicorn("Luck (+15), Regen spts")
               recovery_SP = true;
            if ( v25 == 66 )// of Plenty("Regenerate 1 hp/x and 1 sp/x while walking, etc.")
            {
              recovery_HP = true;
              recovery_SP = true;
            }
          }

          if (recovery_HP &&
              !pParty->pPlayers[v49].pConditions[Condition_Dead] &&
              !pParty->pPlayers[v49].pConditions[Condition_Eradicated])
          {
            if ( pParty->pPlayers[v49].sHealth < pParty->pPlayers[v49].GetMaxHealth() )
              ++pParty->pPlayers[v49].sHealth;
            if ( pParty->pPlayers[v49].pConditions[Condition_Unconcious] && pParty->pPlayers[v49].sHealth > 0 )
              pParty->pPlayers[v49].pConditions[Condition_Unconcious] = 0;
            redraw_flag = true;
          }

          if (recovery_SP &&
              !pParty->pPlayers[v49].pConditions[Condition_Dead] &&
              !pParty->pPlayers[v49].pConditions[Condition_Eradicated])
          {
            if ( pParty->pPlayers[v49].sMana < pParty->pPlayers[v49].GetMaxMana() )
              ++pParty->pPlayers[v49].sMana;
            redraw_flag = true;
          }

          if (decrease_HP &&
              !pParty->pPlayers[v49].pConditions[Condition_Dead] &&
              !pParty->pPlayers[v49].pConditions[Condition_Eradicated] )
          {
            --pParty->pPlayers[v49].sHealth;
            if ( !(pParty->pPlayers[v49].pConditions[Condition_Unconcious]) && pParty->pPlayers[v49].sHealth < 0 )
              pParty->pPlayers[v49].pConditions[Condition_Unconcious] = pParty->uTimePlayed;
            if ( pParty->pPlayers[v49].sHealth < 1 )
            {
              if ( pParty->pPlayers[v49].sHealth + pParty->pPlayers[v49].uEndurance + pParty->pPlayers[v49].GetItemsBonus(CHARACTER_ATTRIBUTE_ENDURANCE) >= 1
                || (signed __int64)pParty->pPlayers[v49].pPlayerBuffs[PLAYER_BUFF_PRESERVATION].uExpireTime > 0 )
                 pParty->pPlayers[v49].pConditions[Condition_Unconcious] = pParty->uTimePlayed;
              else
              {
                if ( !pParty->pPlayers[v49].pConditions[Condition_Dead] )
                  pParty->pPlayers[v49].pConditions[Condition_Dead] = pParty->uTimePlayed;
              }
            }
            redraw_flag = true;
          }
        }
      }

      //regeneration
      if ( pParty->pPlayers[v49].pPlayerBuffs[PLAYER_BUFF_REGENERATION].uExpireTime > 0
        && !pParty->pPlayers[v49].pConditions[Condition_Dead]
        && !pParty->pPlayers[v49].pConditions[Condition_Eradicated] )
      {
        pParty->pPlayers[v49].sHealth += 5 * pParty->pPlayers[v49].pPlayerBuffs[PLAYER_BUFF_REGENERATION].uPower;
        if ( pParty->pPlayers[v49].sHealth > pParty->pPlayers[v49].GetMaxHealth() )
          pParty->pPlayers[v49].sHealth = pParty->pPlayers[v49].GetMaxHealth();
        if ( pParty->pPlayers[v49].pConditions[Condition_Unconcious] && pParty->pPlayers[v49].sHealth > 0 )
          pParty->pPlayers[v49].pConditions[Condition_Unconcious] = 0;
        redraw_flag = true;
      }

      //for warlock
      if ( has_dragon_flag && pParty->pPlayers[v49].classType == PLAYER_CLASS_WARLOCK )
      {
        if ( pParty->pPlayers[v49].sMana < pParty->pPlayers[v49].GetMaxMana() )
          ++pParty->pPlayers[v49].sMana;
        redraw_flag = true;
      }

      //for lich
      if ( pParty->pPlayers[v49].classType == PLAYER_CLASS_LICH )
      {
        for ( v31 = 0; v31 < 126; ++v31 )
        {
          if ( pParty->pPlayers[v49].pInventoryItemList[v31].uItemID == ITEM_LICH_JAR_FULL )
            lich_jar_flag = true;
        }
        lich_flag = true;
      }
      if ( lich_flag && !pParty->pPlayers[v49].pConditions[Condition_Dead]
                     && !pParty->pPlayers[v49].pConditions[Condition_Eradicated] )
      {
        if ( pParty->pPlayers[v49].sHealth > pParty->pPlayers[v49].GetMaxHealth() / 2 )
          pParty->pPlayers[v49].sHealth = pParty->pPlayers[v49].sHealth - 2;
        if ( pParty->pPlayers[v49].sMana > pParty->pPlayers[v49].GetMaxMana() / 2 )
          pParty->pPlayers[v49].sMana = pParty->pPlayers[v49].sMana - 2;
      }
      if ( lich_jar_flag )
      {
       if ( pParty->pPlayers[v49].sMana < pParty->pPlayers[v49].GetMaxMana() )
          ++pParty->pPlayers[v49].sMana;
      }

      //for zombie
      if ( pParty->pPlayers[v49].pConditions[Condition_Zombie] )
        zombie_flag = true;
      if ( zombie_flag && !pParty->pPlayers[v49].pConditions[Condition_Dead]
                       && !pParty->pPlayers[v49].pConditions[Condition_Eradicated] )
      {
        if ( pParty->pPlayers[v49].sHealth > pParty->pPlayers[v49].GetMaxHealth() / 2 )
          pParty->pPlayers[v49].sHealth = pParty->pPlayers[v49].sHealth - 1;
        if ( pParty->pPlayers[v49].sMana > 0 )
          pParty->pPlayers[v49].sMana = pParty->pPlayers[v49].sMana - 1;
      }
    }
    pParty->uLastRegenerationTime = pParty->uTimePlayed;
    if ( !viewparams->bRedrawGameUI )
      viewparams->bRedrawGameUI = redraw_flag;
  }
}

//----- (00493F79) --------------------------------------------------------
void init_summoned_item(stru351_summoned_item *_this, __int64 duration)
{
  signed __int64 v2; // ST2C_8@1
  signed __int64 v3; // qax@1
  //signed __int64 v4; // ST1C_8@1
  unsigned __int64 v5; // qax@1
  unsigned int v6; // ebx@1

  v2 = (signed __int64)((double)duration * 0.234375);
  v3 = v2 / 60 / 60;
  //v4 = v3;
  v5 = (unsigned int)v3 / 0x18;
  v6 = (unsigned int)(v5 / 7) >> 2;
  _this->field_0_expire_second = v2 % 60;
  _this->field_4_expire_minute = v2 / 60 % 60;
  _this->field_8_expire_hour = v3 % 24;
  _this->field_10_expire_week = v5 / 7 & 3;
  _this->field_C_expire_day = (unsigned int)v5 % 0x1C;
  _this->field_14_exprie_month = v6 % 0xC;
  _this->field_18_expire_year = v6 / 0xC + game_starting_year;
}

//----- (00494035) --------------------------------------------------------
void _494035_timed_effects__water_walking_damage__etc()
{
  signed __int64 v0; // qax@1
  unsigned int v4; // edi@1
//  signed int v12; // edi@29
  int v24; // ecx@60
  int v26; // ecx@64
  int v28; // ecx@68
  int v30; // ecx@72
  int v32; // ecx@76
  int v34; // ecx@80
  int v36; // ecx@84
  int v38; // ecx@88
  int v40; // ecx@92
  int v42; // ecx@96
  bool v43; // ebx@102
  bool v46; // edi@111
//  unsigned int v56; // [sp-8h] [bp-38h]@55
//  int v59; // [sp-4h] [bp-34h]@55
//  unsigned int v61; // [sp+14h] [bp-1Ch]@1
  signed int a2a; // [sp+18h] [bp-18h]@47
  signed int old_day; // [sp+1Ch] [bp-14h]@47
  signed int old_hour;

  old_day = pParty->uDaysPlayed;
  old_hour = pParty->uCurrentHour;
  //auto prev_time = pEventTimer->uTimeElapsed;
  pParty->uTimePlayed += pEventTimer->uTimeElapsed;
  v0 = ((signed __int64)(pParty->uTimePlayed * 0.234375) / 60)/60i64;
  v4 = (unsigned int)(((unsigned int)v0 / 24) / 7) >> 2;
  pParty->uCurrentTimeSecond = (signed __int64)((double)(signed __int64)pParty->uTimePlayed * 0.234375) % 60;
  pParty->uCurrentMinute = ((signed __int64)(pParty->uTimePlayed * 0.234375) / 60) % 60;
  pParty->uCurrentHour = v0 % 24;
  pParty->uCurrentMonthWeek = ((unsigned int)v0 / 24) / 7 & 3;
  pParty->uDaysPlayed = (unsigned int)((unsigned int)v0 / 24) % 28;
  pParty->uCurrentMonth = v4 % 12;
  pParty->uCurrentYear = v4 / 0xC + game_starting_year;
  if ( pParty->uCurrentHour >= 3 && (old_hour < 3 || pParty->uDaysPlayed > old_day) ) // new day dawns
  {
    pParty->pHirelings[0].bHasUsedTheAbility = false;
    pParty->pHirelings[1].bHasUsedTheAbility = false;

    for (uint i = 0; i < pNPCStats->uNumNewNPCs; ++i)
      pNPCStats->pNewNPCData[i].bHasUsedTheAbility = false;

    ++pParty->days_played_without_rest;
    if (pParty->days_played_without_rest > 1)
    {
      for (uint i = 0; i < 4; ++i)
        pParty->pPlayers[i].SetCondWeakWithBlockCheck(0);

      if (pParty->uNumFoodRations)
        Party::TakeFood(1);
      else
        for (uint i = 0; i < 4; ++i)
          pParty->pPlayers[i].sHealth = pParty->pPlayers[i].sHealth / (pParty->days_played_without_rest + 1) + 1;

      if (pParty->days_played_without_rest > 3)
      for ( uint i = 0; i < 4; ++i )
      {
        pParty->pPlayers[i].Zero();
        if (!pParty->pPlayers[i].IsPertified() && !pParty->pPlayers[i].IsEradicated()
         && !pParty->pPlayers[i].IsDead())
        {
          if (rand() % 100 < 5 * pParty->days_played_without_rest)
            pParty->pPlayers[i].SetCondDeadWithBlockCheck(0);
          if (rand() % 100 < 10 * pParty->days_played_without_rest)
            pParty->pPlayers[i].SetCondInsaneWithBlockCheck(0);
        }
      }
    }
    if (uCurrentlyLoadedLevelType == LEVEL_Outdoor)
      pOutdoor->SetFog();

    for (uint i = 0; i < 4; ++i)
      pParty->pPlayers[i].uNumDivineInterventionCastsThisDay = 0;
  }

  if ( pParty->uFlags & 4 && pParty->field_6FC < (signed __int64)pParty->uTimePlayed )//water damage
  {
    pParty->field_6FC = (signed __int64)pParty->uTimePlayed + 128;
    viewparams->bRedrawGameUI = true;
    for ( uint pl = 1; pl <= 4; ++pl )
    {
      if ( pPlayers[pl]->WearsItem(ITEM_RELIC_HARECS_LEATHER, EQUIP_ARMOUR)
        || pPlayers[pl]->HasEnchantedItemEquipped(71)
        || pPlayers[pl]->pPlayerBuffs[PLAYER_BUFF_WATER_WALK].uExpireTime > 0 )
         pPlayers[pl]->PlayEmotion(CHARACTER_EXPRESSION_37, 0);
      else
      {
        if ( !pPlayers[pl]->HasUnderwaterSuitEquipped() )
        {
          pPlayers[pl]->ReceiveDamage((signed __int64)pPlayers[pl]->GetMaxHealth() * 0.1, DMGT_FIRE);
          if ( pParty->uFlags & 4 )
          {
            strcpy(GameUI_Footer_TimedString.data(), pGlobalTXT_LocalizationStrings[660]);// Вы тонете!
            GameUI_Footer_TimeLeft = 128;
          }
        }
        else
          pPlayers[pl]->PlayEmotion(CHARACTER_EXPRESSION_37, 0);
      }
    }
  }
  if ( pParty->uFlags & 0x200 && pParty->field_6FC < (signed __int64)pParty->uTimePlayed ) //lava damage
  {
    viewparams->bRedrawGameUI = true;
    pParty->field_6FC = (signed __int64)pParty->uTimePlayed + 128;
    
    for ( uint pl = 1; pl <= 4; pl++ )
    {
      pPlayers[pl]->ReceiveDamage((signed __int64)pPlayers[pl]->GetMaxHealth() * 0.1, DMGT_FIRE);
      if ( pParty->uFlags & 0x200 )
      {
        strcpy(GameUI_Footer_TimedString.data(), pGlobalTXT_LocalizationStrings[661]); //Вы горите!
        GameUI_Footer_TimeLeft = 128;
      }
    }
  }
  _493938_regenerate();
  uint party_condition_flag = 4;
  a2a = pEventTimer->uTimeElapsed;
  if ( pParty->uFlags2 & PARTY_FLAGS_2_RUNNING )//замедление восстановления при беге
  {
    a2a *= 0.5f;
    if (a2a < 1)
      a2a = 1;
  }
  
  for ( uint pl = 1; pl <= 4; pl++ )
  {
    if ( pPlayers[pl]->uTimeToRecovery )
      pPlayers[pl]->Recover(a2a);//восстановление активности
    if ( pPlayers[pl]->GetItemsBonus(CHARACTER_ATTRIBUTE_ENDURANCE) + pPlayers[pl]->sHealth + pPlayers[pl]->uEndurance >= 1
      || (signed __int64)pPlayers[pl]->pPlayerBuffs[PLAYER_BUFF_PRESERVATION].uExpireTime > 0 )
    {
      if ( pPlayers[pl]->sHealth < 1 )
        pPlayers[pl]->SetCondition(Condition_Unconcious, 0);
    }
    else
      pPlayers[pl]->SetCondition(Condition_Dead, 0);
    if ( pPlayers[pl]->field_E0 )
    {
      v24 = pPlayers[pl]->field_E0 - pEventTimer->uTimeElapsed;
      if ( v24 > 0 )
        pPlayers[pl]->field_E0 = v24;
      else
      {
        pPlayers[pl]->field_E0 = 0;
        viewparams->bRedrawGameUI = true;
      }
    }
    if ( pPlayers[pl]->field_E4 )
    {
      v26 = pPlayers[pl]->field_E4 - pEventTimer->uTimeElapsed;
      if ( v26 > 0 )
        pPlayers[pl]->field_E4 = v26;
      else
      {
        pPlayers[pl]->field_E4 = 0;
        viewparams->bRedrawGameUI = true;
      }
    }
    if ( pPlayers[pl]->field_E8 )
    {
      v28 = pPlayers[pl]->field_E8 - pEventTimer->uTimeElapsed;
      if ( v28 > 0 )
        pPlayers[pl]->field_E8 = v28;
      else
      {
        pPlayers[pl]->field_E8 = 0;
        viewparams->bRedrawGameUI = true;
      }
    }
    if ( pPlayers[pl]->field_EC )
    {
      v30 = pPlayers[pl]->field_EC - pEventTimer->uTimeElapsed;
      if ( v30 > 0 )
        pPlayers[pl]->field_EC = v30;
      else
      {
        pPlayers[pl]->field_EC = 0;
        viewparams->bRedrawGameUI = true;
      }
    }
    if ( pPlayers[pl]->field_F0 )
    {
      v32 = pPlayers[pl]->field_F0 - pEventTimer->uTimeElapsed;
      if ( v32 > 0 )
        pPlayers[pl]->field_F0 = v32;
      else
      {
        pPlayers[pl]->field_F0 = 0;
        viewparams->bRedrawGameUI = true;
      }
    }
    if ( pPlayers[pl]->field_F4 )
    {
      v34 = pPlayers[pl]->field_F4 - pEventTimer->uTimeElapsed;
      if ( v34 > 0 )
        pPlayers[pl]->field_F4 = v34;
      else
      {
        pPlayers[pl]->field_F4 = 0;
        viewparams->bRedrawGameUI = true;
      }
    }
    if ( pPlayers[pl]->field_F8 )
    {
      v36 = pPlayers[pl]->field_F8 - pEventTimer->uTimeElapsed;
      if ( v36 > 0 )
        pPlayers[pl]->field_F8 = v36;
      else
      {
        pPlayers[pl]->field_F8 = 0;
        viewparams->bRedrawGameUI = true;
      }
    }
    if ( pPlayers[pl]->field_FC )
    {
      v38 = pPlayers[pl]->field_FC - pEventTimer->uTimeElapsed;
      if ( v38 > 0 )
        pPlayers[pl]->field_FC = v38;
      else
      {
        pPlayers[pl]->field_FC = 0;
        viewparams->bRedrawGameUI = true;
      }
    }
    if ( pPlayers[pl]->field_100 )
    {
      v40 = pPlayers[pl]->field_100 - pEventTimer->uTimeElapsed;
      if ( v40 > 0 )
        pPlayers[pl]->field_100 = v40;
      else
      {
        pPlayers[pl]->field_100 = 0;
        viewparams->bRedrawGameUI = true;
      }
    }
    if ( pPlayers[pl]->field_104 )
    {
      v42 = pPlayers[pl]->field_104 - pEventTimer->uTimeElapsed;
      if ( v42 > 0 )
        pPlayers[pl]->field_104 = v42;
      else
      {
        pPlayers[pl]->field_104 = 0;
        viewparams->bRedrawGameUI = true;
      }
    }
    if ( pPlayers[pl]->pConditions[Condition_Sleep] | pPlayers[pl]->pConditions[Condition_Paralyzed]
       | pPlayers[pl]->pConditions[Condition_Unconcious] | pPlayers[pl]->pConditions[Condition_Dead]
       | pPlayers[pl]->pConditions[Condition_Pertified] | pPlayers[pl]->pConditions[Condition_Eradicated] )
      --party_condition_flag;
    v43 = (signed __int64)pPlayers[pl]->pPlayerBuffs[PLAYER_BUFF_HASTE].uExpireTime > 0; //спешка

    for ( uint k = 0; k < 24; ++k )
      pPlayers[pl]->pPlayerBuffs[k].IsBuffExpiredToTime(pParty->uTimePlayed);

    if ( v43 && (signed __int64)pPlayers[pl]->pPlayerBuffs[7].uExpireTime <= 0 )
      pPlayers[pl]->SetCondition(Condition_Weak, 0);
  }

  v46 = (signed __int64)pParty->pPartyBuffs[PARTY_BUFF_HASTE].uExpireTime > 0;

  for (uint i = 0; i < 20; ++i)
  {
    if (pParty->pPartyBuffs[i].IsBuffExpiredToTime(pParty->uTimePlayed) == 1)
      viewparams->bRedrawGameUI = true;
  }

  if ( v46 && (signed __int64)pParty->pPartyBuffs[PARTY_BUFF_HASTE].uExpireTime <= 0 )
  {
    for (uint i = 0; i < 4; ++i)
      pParty->pPlayers[i].SetCondition(1, 0);
  }

  for (uint i = 0; i < 2; ++i)//Проверка в сознании ли перс сделавший закл на полёт и хождение по воде
  {
    SpellBuff* pBuf = &pParty->pPartyBuffs[Party_Spec_Motion_status_ids[i]];
    if (pBuf->uExpireTime == 0)
      continue;

    if ( !(pBuf->uFlags & 1) )
    {
      if (!pPlayers[pBuf->uCaster]->CanAct())
      {
        pBuf->Reset();
        if (Party_Spec_Motion_status_ids[i] == PARTY_BUFF_FLY )
          pParty->bFlying = false;
      }
    }
  }

  if ( !party_condition_flag )
  {
    if ( pCurrentScreen != SCREEN_REST )
    {
      for ( uint pl = 1; pl <= 4; pl++ )
      {
        if ( pPlayers[pl]->pConditions[Condition_Sleep] )
        {
          pPlayers[pl]->pConditions[Condition_Sleep] = 0;
          party_condition_flag = 1;
          break;
        }
      }
      if ( !party_condition_flag || _5C35C0_force_party_death )
        uGameState = GAME_STATE_PARTY_DIED;
    }
  }

  if ( uActiveCharacter )//выбор следующего после пропускающего ход
  {
    if ( pCurrentScreen != SCREEN_REST )
    {
      if ( pPlayers[uActiveCharacter]->pConditions[Condition_Sleep]
        || pPlayers[uActiveCharacter]->pConditions[Condition_Paralyzed]
        || pPlayers[uActiveCharacter]->pConditions[Condition_Unconcious]
        || pPlayers[uActiveCharacter]->pConditions[Condition_Dead]
        || pPlayers[uActiveCharacter]->pConditions[Condition_Pertified]
        || pPlayers[uActiveCharacter]->pConditions[Condition_Eradicated] )
      {
        viewparams->bRedrawGameUI = true;
        uActiveCharacter = pParty->GetNextActiveCharacter();
      }
    }
  }
}

//----- (00494820) --------------------------------------------------------
unsigned int __fastcall _494820_training_time(unsigned int a1)
{
  signed int v1; // eax@1

  v1 = 5;
  if ( a1 % 24 >= 5 )
    v1 = 29;
  return v1 - a1 % 24;
}

//----- (00494836) --------------------------------------------------------
int stru339_spell_sound::AddPartySpellSound(int uSoundID, int a6)
{
  int v3; // esi@1
  int result; // eax@1
  //stru339_spell_sound *v5; // ebx@1
  //int *v6; // edi@2
  unsigned int v7; // eax@3
  int v8; // [sp+Ch] [bp-8h]@3
  int v9; // [sp+10h] [bp-4h]@2
  int a2a; // [sp+1Ch] [bp+8h]@1
  //return 0;
  v3 = 0;
  result = word_4EE088_sound_ids[uSoundID];
  //v5 = this;
  a2a = word_4EE088_sound_ids[uSoundID];
  if ( word_4EE088_sound_ids[uSoundID] )
  {
    //v6 = this->pSoundsOffsets;
    for ( v9 = 0; v9 < 2; ++v9 )
    {
      v7 = a2a++;
      result = pSoundList->LoadSound(v7, (char *)this + v3, 44744 - v3, &v8, a6);
      if ( !result )
        break;
      a6 += 4;
      result = v8 + 256;
      this->pSoundsOffsets[v9] = v3;
      v3 += result;
      this->pSoundsSizes[v9] = v8 + 256;
      //++v6;
    }
  }
  return result;
}
// 4EE088: using guessed type __int16 word_4EE088_sound_ids[];

//----- (00494AED) --------------------------------------------------------
unsigned int PlayerFrameTable::GetFrameIdByExpression(CHARACTER_EXPRESSION_ID expression)
{
  for ( uint i = 0; i < this->uNumFrames; i++ )
  {
    if ( this->pFrames[i].expression == expression )
      return i;
  }
  return 0;
}

//----- (00494B10) --------------------------------------------------------
PlayerFrame *PlayerFrameTable::GetFrameBy_x(unsigned int uFramesetID, unsigned int uFrameID)
{
  unsigned int v3; // esi@1
  __int16 v6; // dx@2
  int v7; // edx@3
  char *i; // eax@3
  int v9; // ecx@5
  PlayerFrame *result; // eax@6

  v3 = uFramesetID;
  if ( this->pFrames[uFramesetID].uFlags & 1 && (v6 = this->pFrames[uFramesetID].uAnimLength) != 0 )
  {
    v7 = ((signed int)uFrameID >> 3) % (unsigned __int16)v6;
    for ( i = (char *)&this->pFrames[uFramesetID].uAnimTime; ; i += 10 )
    {
      v9 = *(short *)i;
      if ( v7 <= v9 )
        break;
      v7 -= v9;
      ++v3;
    }
    result = &this->pFrames[v3];
  }
  else
    result = &this->pFrames[uFramesetID];
  return result;
}

//----- (00494B5E) --------------------------------------------------------
PlayerFrame *PlayerFrameTable::GetFrameBy_y(int *pFramesetID, int *pAnimTime, int a4)
{
  int v5; // esi@1
  int v6; // eax@2

  v5 = a4 + *pAnimTime;
  if ( v5 < 8 * this->pFrames[*pFramesetID].uAnimTime )
    *pAnimTime = v5;
  else
  {
    v6 = rand() % 4 + 21;
    *pFramesetID = v6;
    *pAnimTime = 8 * v5 % this->pFrames[v6].uAnimTime;
  }
  return &this->pFrames[*pFramesetID];
}

//----- (00494BC3) --------------------------------------------------------
void PlayerFrameTable::ToFile()
{
  PlayerFrameTable *v1; // esi@1
  FILE *v2; // eax@1
  FILE *v3; // edi@1

  PlayerFrameTable* Str = this;

  v1 = Str;
  v2 = fopen("data\\dpft.bin", "wb");
  v3 = v2;
  if ( !v2 )
    Error("Unable to save dpft.bin");
  fwrite(v1, 4, 1, v2);
  fwrite(v1->pFrames, 0xAu, v1->uNumFrames, v3);
  fclose(v3);
}

//----- (00494C0F) --------------------------------------------------------
void PlayerFrameTable::FromFile(void *data_mm6, void *data_mm7, void *data_mm8)
{
  uint num_mm6_frames = data_mm6 ? *(int *)data_mm6 : 0,
       num_mm7_frames = data_mm7 ? *(int *)data_mm7 : 0,
       num_mm8_frames = data_mm8 ? *(int *)data_mm8 : 0;
  uNumFrames = num_mm6_frames + num_mm7_frames + num_mm8_frames;
  assert(uNumFrames);
  assert(!num_mm8_frames);

  pFrames = (PlayerFrame *)malloc(uNumFrames * sizeof(PlayerFrame));
  memcpy(pFrames,                                   (char *)data_mm7 + 4, num_mm7_frames * sizeof(PlayerFrame));
  memcpy(pFrames + num_mm7_frames,                  (char *)data_mm6 + 4, num_mm6_frames * sizeof(PlayerFrame));
  memcpy(pFrames + num_mm6_frames + num_mm7_frames, (char *)data_mm8 + 4, num_mm8_frames * sizeof(PlayerFrame));
}

//----- (00494C5A) --------------------------------------------------------
int PlayerFrameTable::FromFileTxt(const char *Args)
{
  //PlayerFrameTable *v2; // ebx@1
  FILE *v3; // eax@1
  int v4; // esi@3
  void *v5; // eax@10
  FILE *v6; // ST0C_4@12
  char *i; // eax@12
//  __int16 v8; // ax@15
//  const char *v9; // ST10_4@15
//  unsigned __int16 v10; // ax@15
//  const char *v11; // ST0C_4@15
  int j; // esi@15
//  int v13; // eax@17
  int v14; // edx@22
  int v15; // ecx@23
  int v16; // eax@24
  signed int k; // eax@27
  //PlayerFrame *v18; // edx@28
  int v19; // esi@28
  int l; // ecx@29
  char Buf; // [sp+Ch] [bp-2F8h]@3
  FrameTableTxtLine v23; // [sp+200h] [bp-104h]@4
  FrameTableTxtLine v24; // [sp+27Ch] [bp-88h]@4
  int v25; // [sp+2F8h] [bp-Ch]@3
  int v26; // [sp+2FCh] [bp-8h]@3
  FILE *File; // [sp+300h] [bp-4h]@1
  int Argsa; // [sp+30Ch] [bp+8h]@28

  __debugbreak();//Ritor1;
  //TileTable::dtor((TileTable *)this);
  v3 = fopen(Args, "r");
  File = v3;
  if ( !v3 )
    Error("PlayerFrameTable::load - Unable to open file: %s.", Args);
  v4 = 0;
  v25 = 0;
  v26 = 1;
  if ( fgets(&Buf, 490, v3) )
  {
    do
    {
      *strchr(&Buf, 10) = 0;
      memcpy(&v24, txt_file_frametable_parser(&Buf, &v23), sizeof(v24));
      if ( v24.uPropCount && *v24.pProperties[0] != 47 )
      {
        if ( v24.uPropCount < 3 )
          Error("PlayerFrameTable::load, too few arguments, %s line %i.", Args, v26);
        ++v25;
      }
      ++v26;
    }
    while ( fgets(&Buf, 490, File) );
    v4 = v25;
  }
  this->uNumFrames = v4;
  v5 = malloc(10 * v4);
  this->pFrames = (PlayerFrame *)v5;
  if ( !v5 )
    Error("PlayerFrameTable::load - Out of Memory!");
  v6 = File;
  this->uNumFrames = 0;
  fseek(v6, 0, 0);
  for ( i = fgets(&Buf, 490, File); i; i = fgets(&Buf, 490, File) )
  {
    *strchr(&Buf, 10) = 0;
    memcpy(&v24, txt_file_frametable_parser(&Buf, &v23), sizeof(v24));
    if ( v24.uPropCount && *v24.pProperties[0] != 47 )
    {
      //v8 = atoi(v24.pProperties[0]);
      //v9 = v24.pProperties[1];
      this->pFrames[this->uNumFrames].expression = (CHARACTER_EXPRESSION_ID)atoi(v24.pProperties[0]);
      //v10 = atoi(v9);
      //v11 = v24.pProperties[2];
      this->pFrames[this->uNumFrames].uTextureID = atoi(v24.pProperties[1]);
      this->pFrames[this->uNumFrames].uAnimTime = atoi(v24.pProperties[2]);
      this->pFrames[this->uNumFrames].uAnimLength = 0;
      this->pFrames[this->uNumFrames].uFlags = 0;
      for ( j = 3; j < v24.uPropCount; ++j )
      {
        if ( !_stricmp(v24.pProperties[j], "New") )
          this->pFrames[this->uNumFrames].uFlags |= 4;
      }
      ++this->uNumFrames;
    }
  }
  fclose(File);
  
  if ( (signed int)(this->uNumFrames - 1) > 0 )
  {
    v15 = 0;
    for ( v14 = 0; v14 < this->uNumFrames - 1; ++v14 )
    {
      v16 = (int)&this->pFrames[v15];
      if ( !(*(char *)(v16 + 18) & 4) )
        this->pFrames[v14].uFlags |= 1;
      ++v15;
    }
  }
  for ( k = 0; k < (signed int)this->uNumFrames; *(short *)(Argsa + 6) = v19 )
  {
    //v18 = this->pFrames;
    Argsa = (int)&this->pFrames[k];
    v19 = *(short *)(Argsa + 4);
    if ( this->pFrames[k].uFlags & 1 )
    {
      ++k;
      for ( l = (int)&this->pFrames[k]; this->pFrames[k].uFlags & 1; l += 10 )
      {
        v19 += *(short *)(l + 4);
        ++k;
      }
      LOWORD(v19) = this->pFrames[k].uAnimTime + v19;
    }
    ++k;
  }
  return 1;
}



//----- (00495430) --------------------------------------------------------
const char * GetReputationString( signed int a1 )
    {
  if (a1 >= 25)
    return pGlobalTXT_LocalizationStrings[379]; // Hated
  else if (a1 >= 6)
    return pGlobalTXT_LocalizationStrings[392]; // Unfriendly
  else if (a1 >= -5)
    return pGlobalTXT_LocalizationStrings[399]; // Neutral;
  else if (a1 >= -24)
    return pGlobalTXT_LocalizationStrings[402]; // Friendly
  else
    return pGlobalTXT_LocalizationStrings[434]; // Respected;
}

//----- (00495461) --------------------------------------------------------
char *BuildDialogueString(const char *lpsz, unsigned __int8 uPlayerID, ItemGen *a3, char *a4, int a5, __int64 *a6)
{
  Player *pPlayer; // ebx@3
  const char *pText; // esi@7
  int v17; // eax@10
  signed __int64 v18; // qax@18
  unsigned __int8 *v20; // ebx@32
  int v21; // ecx@34
  int pReputation; // eax@45
  int v29; // eax@68
  __int16 v55[56]; // [sp+10h] [bp-128h]@34
  stru351_summoned_item v56; // [sp+80h] [bp-B8h]@107
  char a1[100]; // [sp+B8h] [bp-80h]@3
  int v63; // [sp+12Ch] [bp-Ch]@32

  if ( IsBadStringPtrA(lpsz, 1) )
    return "Invalid String Passed";

  a1[0] = 0;
  pPlayer = &pParty->pPlayers[uPlayerID];
  memset(pTmpBuf2.data(), 0, sizeof(pTmpBuf2));

  NPCData *npc = nullptr;
  if ( dword_5C35D4 )
    npc = HouseNPCData[(unsigned int)((char *)pDialogueNPCCount + -(dword_591080 != 0))]; //- 1
  else
    npc = GetNPCData(sDialogue_SpeakingActorNPC_ID);

  //pText = a4;
  uint len = strlen(lpsz);
  for (int i = 0, dst = 0; i < len; ++i)
  {
    char c = lpsz[i];
    if (c != '%')
      pTmpBuf2[dst++] = c;
    else
    {
      v17 = 10 * (int)(lpsz[i + 1] - '0') + lpsz[i + 2] - '0';

      switch ( v17 )
      {
        case 1://Подробнее
          strcat(pTmpBuf2.data(), npc->pName);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 2:
          strcat(pTmpBuf2.data(), pPlayer->pName);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 3:
        case 4:
          strcat(pTmpBuf2.data(), a1);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 5:
          v18 = (signed __int64)((double)(signed __int64)pParty->uTimePlayed * 0.234375) / 60 / 60 % 24;
          pText = pGlobalTXT_LocalizationStrings[397];// "evening"
          if ( SHIDWORD(v18) <= 0 && SHIDWORD(v18) >= 0 && (unsigned int)v18 >= 5 && SHIDWORD(v18) <= 0 )
          {
            if ( SHIDWORD(v18) >= 0 && (unsigned int)v18 >= 11 )
            {
              if ( v18 < 20 )
                pText = pGlobalTXT_LocalizationStrings[396];// "day"
            }
            else
            {
              pText = pGlobalTXT_LocalizationStrings[395];// "morning"
            }
          }
          strcat(pTmpBuf2.data(), pText);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 6:
          if ( pPlayer->uSex )
            pText = pGlobalTXT_LocalizationStrings[387];// "lady"
          else
            pText = pGlobalTXT_LocalizationStrings[385];// "sir"
          strcat(pTmpBuf2.data(), pText);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 7:
          if ( pPlayer->uSex )
            pText = pGlobalTXT_LocalizationStrings[389];// "Lady"
          else
            pText = pGlobalTXT_LocalizationStrings[386];// "Sir"
          strcat(pTmpBuf2.data(), pText);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 8:
          v63 = 0;
          v20 = (unsigned __int8 *)pPlayer->_achieved_awards_bits;
          for ( uint _i = 0; _i < 28; ++_i )
          {
            if ( (unsigned __int16)_449B57_test_bit(v20, word_4EE150[i]) )
            {
              v21 = v63;
              ++v63;
              v55[v63] = word_4EE150[i];
            }
          }
          if ( v63 )
          {
            if ( dword_A74CDC == -1 )
              dword_A74CDC = rand() % v63;
            pText = (char *)pAwards[v55[dword_A74CDC]].pText;//(char *)dword_723E80_award_related[2 * v55[v24]];
          }
          else
            pText = (char *)pNPCTopics[55].pText;
          strcat(pTmpBuf2.data(), pText);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 9:
          if ( npc->uSex )
            pText = pGlobalTXT_LocalizationStrings[384];// "her"
          else
            pText = pGlobalTXT_LocalizationStrings[383];// "his"
          strcat(pTmpBuf2.data(), pText);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 10:
          if ( pPlayer->uSex )
            pText = pGlobalTXT_LocalizationStrings[389];// "Lady"
          else
            pText = pGlobalTXT_LocalizationStrings[388];// "Lord"
          strcat(pTmpBuf2.data(), pText);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 11:
          pReputation = pParty->GetPartyReputation();
          if ( pReputation >= 25 )
            pText = pGlobalTXT_LocalizationStrings[379];
          else//v25 < 25
          {
            if ( pReputation < 6 )
            {
              if ( pReputation >= -5 )//6 >= v25 >= -5
                pText = pGlobalTXT_LocalizationStrings[399];
              else// v25 < -5
              {
                if ( pReputation < -24 )//-24 > v25
                  pText = pGlobalTXT_LocalizationStrings[434];
                else// -5 > v25 > -24
                  pText = pGlobalTXT_LocalizationStrings[402];
              }
            }
            else//25 > v25 > 6
              pText = pGlobalTXT_LocalizationStrings[392];
          }
          strcat(pTmpBuf2.data(), pText);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 12:
          pReputation = npc->rep;
          if ( pReputation >= 25 )
            pText = pGlobalTXT_LocalizationStrings[379];//Ненавистный
          else
          {
            if ( pReputation < 6 )
            {
              if ( pReputation >= -5 )
                pText = pGlobalTXT_LocalizationStrings[399];//Нейтральная
              else
              {
                if ( pReputation < -24 )
                  pText = pGlobalTXT_LocalizationStrings[434];//Почтенная
                else
                  pText = pGlobalTXT_LocalizationStrings[402];//Дружелюбный
              }
            }
            else
              pText = pGlobalTXT_LocalizationStrings[392];//Недружелюбный
          }
          strcat(pTmpBuf2.data(), pText);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 13:
          strcat(pTmpBuf2.data(), pNPCStats->sub_495366_MispronounceName(pPlayer->pName[0], pPlayer->uSex));
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 14:
          if ( npc->uSex )
            pText = pGlobalTXT_LocalizationStrings[391];// "sister"
          else
            pText = pGlobalTXT_LocalizationStrings[390];// "brother"
          strcat(pTmpBuf2.data(), pText);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 15:
          strcat(pTmpBuf2.data(), pGlobalTXT_LocalizationStrings[393]);// "daughter"
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 16:
          if ( npc->uSex )
            pText = pGlobalTXT_LocalizationStrings[391];// "sister"
          else
            pText = pGlobalTXT_LocalizationStrings[390];// "brother"
          strcat(pTmpBuf2.data(), pText);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 17://текст наёмного НПС
        {
          uint pay_percentage = pNPCStats->pProfessions[npc->uProfession].uHirePrice / 100;
          if ( !pay_percentage )
            pay_percentage = 1;
          sprintf(a1, "%lu", pay_percentage);
          strcat(pTmpBuf2.data(), a1);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        }
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 26:
          strncpy(a1, lpsz + i + 1, 2);
          sprintf(a1, "%lu", atoi(a1));
          strcat(pTmpBuf2.data(), a1);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 23:
          if ( pMapStats->GetMapInfo(pCurrentMapName) )
            pText = pMapStats->pInfos[pMapStats->GetMapInfo(pCurrentMapName)].pName;
          else
            pText = pGlobalTXT_LocalizationStrings[394];// "Unknown"
          strcat(pTmpBuf2.data(), pText);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 24://название товара в продаже
          sprintfex(a1, format_4E2D80, Color16(255, 255, 155), a3->GetDisplayName());
          strcat(pTmpBuf2.data(), a1);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 25:
          v29 = pPlayer->GetBaseBuyingPrice(a3->GetValue(), p2DEvents[(signed int)a4 - 1].fPriceMultiplier);
          switch ( a5 )
            {
              case 3:
                v29 = pPlayer->GetBaseSellingPrice(a3->GetValue(), p2DEvents[(signed int)a4 - 1].fPriceMultiplier);
                break;
              case 4:
                v29 = pPlayer->GetBaseIdentifyPrice(p2DEvents[(signed int)a4 - 1].fPriceMultiplier);
                break;
              case 5:
                v29 = pPlayer->GetBaseRepairPrice(a3->GetValue(), p2DEvents[(signed int)a4 - 1].fPriceMultiplier);
                break;
              case 6:
                v29 = pPlayer->GetBaseSellingPrice(a3->GetValue(), p2DEvents[(signed int)a4 - 1].fPriceMultiplier) / 2;
                break;
            }
            sprintfex(a1, "%lu", v29);
            strcat(pTmpBuf2.data(), a1);
            dst = strlen(pTmpBuf2.data());
            i += 2;
            break;
          case 27://текст продажи
            v29 = pPlayer->GetBuyingPrice(a3->GetValue(), p2DEvents[(signed int)a4 - 1].fPriceMultiplier);
            if ( a5 == 3 )
            {
              v29 = pPlayer->GetPriceSell(a3->GetValue(), p2DEvents[(signed int)a4 - 1].fPriceMultiplier);
              if (a3->IsBroken())
                v29 = 1;
              sprintfex(a1, "%lu", v29);
              strcat(pTmpBuf2.data(), a1);
              dst = strlen(pTmpBuf2.data());
              i += 2;
              break;
            }
            if ( a5 != 4 )
            {
              if ( a5 == 5 )
                v29 = pPlayer->GetPriceRepair(a3->GetValue(), p2DEvents[(signed int)a4 - 1].fPriceMultiplier);
              else
              {
                if ( a5 == 6 )
                {
                  v29 = pPlayer->GetPriceSell(a3->GetValue(), p2DEvents[(signed int)a4 - 1].fPriceMultiplier) / 2;
                  if (a3->IsBroken())
                    v29 = 1;
                  if (!v29)
                    v29 = 1;
                  sprintfex(a1, "%lu", v29);
                  strcat(pTmpBuf2.data(), a1);
                  dst = strlen(pTmpBuf2.data());
                  i += 2;
                  break;
                }
              }
              sprintfex(a1, "%lu", v29);
              strcat(pTmpBuf2.data(), a1);
              dst = strlen(pTmpBuf2.data());
              i += 2;
              break;
            }
            sprintfex(a1, "%lu", pPlayer->GetPriceIdentification(p2DEvents[(signed int)a4 - 1].fPriceMultiplier));
            strcat(pTmpBuf2.data(), a1);
            dst = strlen(pTmpBuf2.data());
            i += 2;
            break;
          case 28://профессия
            strcat(pTmpBuf2.data(), (char *)p2DEvents[(signed int)a4 - 1].pProprieterTitle);
            dst = strlen(pTmpBuf2.data());
            i += 2;
            break;
          case 29:
            sprintfex(a1, "%lu", pPlayer->GetPriceIdentification(p2DEvents[(signed int)a4 - 1].fPriceMultiplier));
            strcat(pTmpBuf2.data(), a1);
            dst = strlen(pTmpBuf2.data());
            i += 2;
            break;
          case 30:
            if ( !a6 )
            {
              strcat(pTmpBuf2.data(), a4);
              dst = strlen(pTmpBuf2.data());
              i += 2;
              break;
            }
            init_summoned_item(&v56, *a6);
            sprintfex(a1, pGlobalTXT_LocalizationStrings[378], aMonthNames[v56.field_14_exprie_month], v56.field_C_expire_day + 1, v56.field_18_expire_year);
            strcat(pTmpBuf2.data(), a1);
            dst = strlen(pTmpBuf2.data());
            i += 2;
            break;
          case 31:
          case 32:
          case 33:
          case 34:
            strcat(pTmpBuf2.data(), pParty->pPlayers[v17 - 31].pName);
            dst = strlen(pTmpBuf2.data());
            i += 2;
            break;
          default:
            if ( v17 <= 50 || v17 > 70 )
            {
              strncpy(a1, lpsz + i + 1, 2);
              sprintf(a1, "%lu", atoi(a1));
              strcat(pTmpBuf2.data(), a1);
              dst = strlen(pTmpBuf2.data());
              i += 2;
              break;
            }
            if ( v17 - 51 >= 20 )
            {
              strcat(pTmpBuf2.data(), a4);
              dst = strlen(pTmpBuf2.data());
              i += 2;
              break;
            }
            init_summoned_item(&v56, pParty->PartyTimes._s_times[v17-51]);
            sprintfex(a1, pGlobalTXT_LocalizationStrings[378], aMonthNames[v56.field_14_exprie_month], v56.field_C_expire_day + 1, v56.field_18_expire_year);
            strcat(pTmpBuf2.data(), a1);
            dst = strlen(pTmpBuf2.data());
            i += 2;
          break;
        }
      }
  }
  return pTmpBuf2.data();
}

//----- (0049B04D) --------------------------------------------------------
void stru154::GetFacePlaneAndClassify(ODMFace *a2, BSPVertexBuffer *a3)
{
  Vec3_float_ v; // [sp+4h] [bp-Ch]@1
  float v7;

  v.x = 0.0;
  v.y = 0.0;
  v.z = 0.0;
  GetFacePlane(a2, a3, &v, &v7);

  if (fabsf(a2->pFacePlane.vNormal.z) < 1e-6f)
    polygonType = POLYGON_VerticalWall;
  else if (fabsf(a2->pFacePlane.vNormal.x) < 1e-6f &&
           fabsf(a2->pFacePlane.vNormal.y) < 1e-6f)
    polygonType = POLYGON_Floor;
  else
    polygonType = POLYGON_InBetweenFloorAndWall;

  face_plane.vNormal.x = v.x;
  face_plane.vNormal.y = v.y;
  face_plane.vNormal.z = v.z;
  face_plane.dist = v7;
}

//----- (0049B0C9) --------------------------------------------------------
void stru154::ClassifyPolygon(Vec3_float_ *pNormal, float dist)
{
  if (fabsf(pNormal->z) < 1e-6f)
    polygonType = POLYGON_VerticalWall;
  else if (fabsf(pNormal->x) < 1e-6f &&
           fabsf(pNormal->y) < 1e-6f)
    polygonType = POLYGON_Floor;
  else
    polygonType = POLYGON_InBetweenFloorAndWall;

  face_plane.vNormal.x = pNormal->x;
  face_plane.dist = dist;
  face_plane.vNormal.y = pNormal->y;
  face_plane.vNormal.z = pNormal->z;
}

//----- (0049B13D) --------------------------------------------------------
void stru154::GetFacePlane(ODMFace *pFace, BSPVertexBuffer *pVertices, Vec3_float_ *pOutNormal, float *pOutDist)
{
  Vec3_float_ *v19; // eax@3
  Vec3_float_ v2; // [sp+4h] [bp-64h]@3
  float v26; // [sp+1Ch] [bp-4Ch]@3
  float v27; // [sp+20h] [bp-48h]@3
  float v28; // [sp+24h] [bp-44h]@3
  Vec3_float_ v1; // [sp+40h] [bp-28h]@1
  Vec3_float_ v38; // [sp+58h] [bp-10h]@3

  v1.x = 0.0;
  v1.y = 0.0;
  v1.z = 0.0;

  if (pFace->uNumVertices >= 2)
  {
    for ( int i = 0; i < pFace->uNumVertices - 2; i++ )
    {
      v1.x = pVertices->pVertices[pFace->pVertexIDs[i + 1]].x - pVertices->pVertices[pFace->pVertexIDs[i]].x;
      v1.y = pVertices->pVertices[pFace->pVertexIDs[i + 1]].y - pVertices->pVertices[pFace->pVertexIDs[i]].y;
      v1.z = pVertices->pVertices[pFace->pVertexIDs[i + 1]].z - pVertices->pVertices[pFace->pVertexIDs[i]].z;

      v26 = pVertices->pVertices[pFace->pVertexIDs[i + 2]].x - pVertices->pVertices[pFace->pVertexIDs[i + 1]].x;
      v27 = pVertices->pVertices[pFace->pVertexIDs[i + 2]].y - pVertices->pVertices[pFace->pVertexIDs[i + 1]].y;
      v28 = pVertices->pVertices[pFace->pVertexIDs[i + 2]].z - pVertices->pVertices[pFace->pVertexIDs[i + 1]].z;

      v19 = Vec3_float_::Cross(&v1, &v2, v26, v27, v28);
      v38.x = v19->x;
      v38.y = v19->y;
      v38.z = v19->z;
      if ( v38.x != 0.0  || v38.y != 0.0 || v38.z != 0.0)
      {
        v38.Normalize();

        pOutNormal->x = v38.x;
        pOutNormal->y = v38.y;
        pOutNormal->z = v38.z;

        *pOutDist = -(pVertices->pVertices[pFace->pVertexIDs[i]].x * v38.x
                    + pVertices->pVertices[pFace->pVertexIDs[i]].y * v38.y
                    + pVertices->pVertices[pFace->pVertexIDs[i]].z * v38.z);
        return;
      }
    }
  }

  pOutNormal->x = (double)(pFace->pFacePlane.vNormal.x & 0xFFFF) / 65535.0f + (double)(pFace->pFacePlane.vNormal.x >> 16);
  pOutNormal->y = (double)(pFace->pFacePlane.vNormal.y & 0xFFFF) / 65535.0f + (double)(pFace->pFacePlane.vNormal.y >> 16);
  pOutNormal->z = (double)(pFace->pFacePlane.vNormal.z & 0xFFFF) / 65535.0f + (double)(pFace->pFacePlane.vNormal.z >> 16);
  *pOutDist = (double)(pFace->pFacePlane.dist & 0xFFFF) / 65535.0f + (double)(pFace->pFacePlane.dist >> 16);
}

//----- (0049D700) --------------------------------------------------------
unsigned int __fastcall GetMaxMipLevels(unsigned int uDim)
{
  int v2; // ecx@1
  unsigned int v3; // eax@1

  v2 = 0;
  v3 = uDim - 1;
  while ( v3 & 1 )
  {
    v3 >>= 1;
    ++v2;
  }
  return v3 == 0 ? v2 : 0;
}

//----- (004B1447) --------------------------------------------------------
void sub_4B1447_party_fine(int shopId, int stealingResult, int fineToAdd)
{
  signed int v3; // esi@1
  DDM_DLV_Header *v7; // eax@14

  if ( stealingResult == 0 || stealingResult == 1) 
  {
    if ( pParty->uFine < 4000000 )
    {
      if ( fineToAdd + pParty->uFine < 0 )
        pParty->uFine = 0;
      else if ( fineToAdd + pParty->uFine > 4000000 )
        pParty->uFine = 4000000;
      else 
        pParty->uFine += fineToAdd;
    }
    if ( pParty->uFine )
    {
      for ( uint i = 1; i <= 4; ++i )
      {
        if ( !_449B57_test_bit(pPlayers[i]->_achieved_awards_bits, 1) )
          _449B7E_toggle_bit(pPlayers[i]->_achieved_awards_bits, 1, 1);
      }
    }
    if (stealingResult == 1)
      v3 = 2;
    else
      v3 = 1;
  }
  else
    v3 = 2;
  pParty->PartyTimes._shop_ban_times[shopId] = pParty->uTimePlayed + 368640;
  pParty->InTheShopFlags[shopId] = 1;
  v7 = &pOutdoor->ddm;
  if ( uCurrentlyLoadedLevelType != LEVEL_Outdoor )
    v7 = &pIndoor->dlv;
  v7->uReputation += v3;
  if ( v7->uReputation > 10000 )
    v7->uReputation = 10000;
}


//----- (004B1ECE) --------------------------------------------------------
void OracleDialogue()
{
  __int16 *v0; // edi@1
  signed int v4; // eax@9
  int v5; // ebx@11
  signed int v8; // edi@14
  ItemGen *v9; // [sp+Ch] [bp-Ch]@11
  signed int v10; // [sp+10h] [bp-8h]@13
  int v11; // [sp+14h] [bp-4h]@1

  contract_approved = 0;
  v11 = 0;
  uDialogueType = 84;
  current_npc_text = (char *)pNPCTopics[667].pText;
  v0 = _4F0882_evt_VAR_PlayerItemInHands_vals.data();
  //while ( 1 )
  for ( uint i = 0; i <= 53; i++ )
  {
    if ( (unsigned __int16)_449B57_test_bit(pParty->_quest_bits, *v0) )
    {
      //v1 = 0;
      //v2 = pParty->pPlayers.data();
      for ( uint pl = 0; pl < 4; pl++ )
      {
        //LOBYTE(v3) = pParty->pPlayers[pl].CompareVariable(VAR_PlayerItemInHands, *(v0+1));
        if ( pParty->pPlayers[pl].CompareVariable(VAR_PlayerItemInHands, *(v0 + 1)) )
          break;
        //++v2;
        //++v1;
      }
      //while ( (signed int)v2 < (signed int)pParty->pHirelings.data() );
      //if ( v1 == 4 )
        //break;
    }
    ++v11;
    //v0 += 2;
    //if ( v0 > &_4F0882_evt_VAR_PlayerItemInHands_vals[53] )
	  //break;
  }
  if ( v0 <= &_4F0882_evt_VAR_PlayerItemInHands_vals[53] )
  {
	  current_npc_text = (char *)pNPCTopics[666].pText; // Here's %s that you lost. Be careful
	  v4 = _4F0882_evt_VAR_PlayerItemInHands_vals[2 * v11];
	  contract_approved = _4F0882_evt_VAR_PlayerItemInHands_vals[2 * v11];
	  pParty->pPlayers[0].AddVariable(VAR_PlayerItemInHands, v4);
  }
  if ( contract_approved == 601 )
  {
    v5 = 0;
    //v12 = pParty->pPlayers.data();//[0].uClass;
    v9 = 0;
    //while ( 1 )
    for ( uint i = 0; i < 4; i++ )
    {
      if ( pParty->pPlayers[i].classType == PLAYER_CLASS_LICH )
      {
        v10 = 0;
        //v6 = pParty->pPlayers.data();//[0].pInventoryItems[0].field_1A;
        for ( uint pl = 0; pl < 4; pl++ )
        {
          for ( v8 = 0; v8 < 126; v8++ )//138
          {
            if ( pParty->pPlayers[pl].pInventoryItemList[v8].uItemID == ITEM_LICH_JAR_FULL )
            {
              if ( !pParty->pPlayers[pl].pInventoryItemList[v8].uHolderPlayer )
                v9 = &pParty->pPlayers[pl].pInventoryItemList[v8];
              if ( pParty->pPlayers[pl].pInventoryItemList[v8].uHolderPlayer == v5 )
                v10 = 1;
            }
          }
        }
        if ( !v10 )
          break;
      }
//      ++v12;
      ++v5;
	//  if ( v12 > &pParty->pPlayers[3] )
      //  return;
    }
    if ( v9 )
	  v9->uHolderPlayer = v5;
  }
}

//----- (004B254D) --------------------------------------------------------
const char * _4B254D_SkillMasteryTeacher(int trainerInfo)
{
  int teacherLevel; // edx@1
  int skillBeingTaught; // ecx@1
  int pClassType; // eax@7
  int currClassMaxMastery; // eax@7
  int pointsInSkillWOutMastery; // ebx@7
  int classBaseId; // eax@8
  unsigned int skillMastery; // eax@29
  unsigned __int16 pointsInSkill; // [sp+1Ch] [bp-10h]@7
  int masteryLevelBeingTaught; // [sp+24h] [bp-8h]@7

  contract_approved = 0;
  teacherLevel = (trainerInfo - 200) % 3;
  skillBeingTaught = (trainerInfo - 200) / 3;
  Player* activePlayer = pPlayers[uActiveCharacter];
  pClassType = activePlayer->classType;
  currClassMaxMastery = byte_4ED970_skill_learn_ability_by_class_table[pClassType][skillBeingTaught];
  masteryLevelBeingTaught = teacherLevel + 2;
  dword_F8B1B0_MasteryBeingTaught = masteryLevelBeingTaught;
  if ( currClassMaxMastery < masteryLevelBeingTaught )
  {
    classBaseId = pClassType - pClassType % 4;
    if (byte_4ED970_skill_learn_ability_by_class_table[classBaseId + 1][skillBeingTaught] >= masteryLevelBeingTaught)
      sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[633], pClassNames[classBaseId + 1]);//Вы должны достичь звания %s для обучения этому уровню навыка. You have to be promoted to %s to learn this skill level.
    else if (byte_4ED970_skill_learn_ability_by_class_table[classBaseId + 2][skillBeingTaught] >= masteryLevelBeingTaught
      && byte_4ED970_skill_learn_ability_by_class_table[classBaseId + 3][skillBeingTaught] >= masteryLevelBeingTaught)
      sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[634], pClassNames[classBaseId + 2], pClassNames[classBaseId + 3]);//Вы должны достичь звания %s или %s для обучения этому уровню навыка. You have to be promoted to %s or %s to learn this skill level.
    else if (byte_4ED970_skill_learn_ability_by_class_table[classBaseId + 2][skillBeingTaught] >= masteryLevelBeingTaught)
      sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[633], pClassNames[classBaseId + 2]);//Вы должны достичь звания %s для обучения этому уровню навыка. You have to be promoted to %s to learn this skill level.
    else if (byte_4ED970_skill_learn_ability_by_class_table[classBaseId + 3][skillBeingTaught] >= masteryLevelBeingTaught)
      sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[633], pClassNames[classBaseId + 3]);//Вы должны достичь звания %s для обучения этому уровню навыка. You have to be promoted to %s to learn this skill level.
    else
      sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[632], pClassNames[pClassType]);//Этот уровень навыка не может быть постигнут классом %s. This skill level can not be learned by the %s class.
    return pTmpBuf.data();
  }
  if ( !activePlayer->CanAct() )
    return pNPCTopics[122].pText; //Not in your condition!
  pointsInSkill = activePlayer->pActiveSkills[skillBeingTaught];
  pointsInSkillWOutMastery = pointsInSkill & 0x3F;
  if ( !pointsInSkillWOutMastery )
    return pNPCTopics[131].pText; //You must know the skill before you can become an expert in it!
  skillMastery = SkillToMastery(pointsInSkill);
  if ( (signed int)skillMastery > teacherLevel + 1 )  
    return pNPCTopics[teacherLevel + 128].pText;    // You are already an SKILLLEVEL in this skill.	
  dword_F8B1AC_award_bit_number = skillBeingTaught;
  if ( masteryLevelBeingTaught == 2 && pointsInSkillWOutMastery < 4 
    || masteryLevelBeingTaught == 3 && pointsInSkillWOutMastery < 7
    || masteryLevelBeingTaught == 4 && pointsInSkillWOutMastery < 10
    )
    return pNPCTopics[127].pText;  //"You don't meet the requirements, and cannot be taught until you do."
  switch (dword_F8B1AC_award_bit_number)
  {
    case PLAYER_SKILL_STAFF:
    case PLAYER_SKILL_SWORD:
    case PLAYER_SKILL_DAGGER:
    case PLAYER_SKILL_AXE:
    case PLAYER_SKILL_SPEAR:
    case PLAYER_SKILL_BOW:
    case PLAYER_SKILL_MACE:
    case PLAYER_SKILL_ARMSMASTER:
      switch (masteryLevelBeingTaught)
      {
      case 2:
        gold_transaction_amount = 2000;
        break;
      case 3:
        gold_transaction_amount = 5000;
        break;
      case 4:
        gold_transaction_amount = 8000;
        break;
      }
      break;
    case PLAYER_SKILL_BLASTER:
      switch (masteryLevelBeingTaught)
      {
      case 2:
        gold_transaction_amount = 0;
        break;
      case 3:
        gold_transaction_amount = 0;
        break;
      case 4:
        gold_transaction_amount = 0;
        break;
      }
      break;
    case PLAYER_SKILL_SHIELD:
    case PLAYER_SKILL_LEATHER:
    case PLAYER_SKILL_CHAIN:
    case PLAYER_SKILL_PLATE:
      switch (masteryLevelBeingTaught)
      {
      case 2:
        gold_transaction_amount = 1000;
        break;
      case 3:
        gold_transaction_amount = 3000;
        break;
      case 4:
        gold_transaction_amount = 7000;
        break;
      }
      break;
    case PLAYER_SKILL_FIRE:
    case PLAYER_SKILL_AIR:
    case PLAYER_SKILL_WATER:
    case PLAYER_SKILL_EARTH:
    case PLAYER_SKILL_SPIRIT:
    case PLAYER_SKILL_MIND:
    case PLAYER_SKILL_BODY:
      switch (masteryLevelBeingTaught)
      {
      case 2:
        gold_transaction_amount = 1000;
        break;
      case 3:
        gold_transaction_amount = 4000;
        break;
      case 4:
        gold_transaction_amount = 8000;
        break;
      }
      break;
    case PLAYER_SKILL_LIGHT:
      switch (masteryLevelBeingTaught)
      {
      case 2:
        gold_transaction_amount = 2000;
        break;
      case 3:
        if ( !(unsigned __int16)_449B57_test_bit(pParty->_quest_bits, 114) )
          return pNPCTopics[127].pText;
        gold_transaction_amount = 5000;
        break;
      case 4:
        if ( !activePlayer->ProfessionOrGuildFlagsCorrect(0x22u, 1) ||
          !activePlayer->ProfessionOrGuildFlagsCorrect(0x1Au, 1))
          return pNPCTopics[127].pText;
        gold_transaction_amount = 8000;
        break;
      }
      break;
    case PLAYER_SKILL_DARK:
      switch (masteryLevelBeingTaught)
      {
      case 2:
        gold_transaction_amount = 2000;
        break;
      case 3:
        if ( !(unsigned __int16)_449B57_test_bit(pParty->_quest_bits, 110) )
          return pNPCTopics[127].pText;
        gold_transaction_amount = 5000;
        break;
      case 4:
        if ( !activePlayer->ProfessionOrGuildFlagsCorrect(0x23u, 1) 
          || !activePlayer->ProfessionOrGuildFlagsCorrect(0x1Bu, 1))
          return pNPCTopics[127].pText;
        gold_transaction_amount = 8000;
        break;
      }
      break;
    case PLAYER_SKILL_ITEM_ID:
    case PLAYER_SKILL_REPAIR:
    case PLAYER_SKILL_MEDITATION:
    case PLAYER_SKILL_PERCEPTION:
    case PLAYER_SKILL_TRAP_DISARM:
    case PLAYER_SKILL_MONSTER_ID:
    case PLAYER_SKILL_STEALING:
    case PLAYER_SKILL_ALCHEMY:
      switch (masteryLevelBeingTaught)
      {
      case 2:
        gold_transaction_amount = 500;
        break;
      case 3:
        gold_transaction_amount = 2500;
        break;
      case 4:
        gold_transaction_amount = 6000;
        break;
      }
      break;
    case PLAYER_SKILL_MERCHANT:
      switch (masteryLevelBeingTaught)
      {
      case 2:
        gold_transaction_amount = 2000;
        break;
      case 3:
        if ( activePlayer->GetBaseWillpower() < 50 )
          return pNPCTopics[127].pText;
        gold_transaction_amount = 5000;
        break;
      case 4:
        gold_transaction_amount = 8000;
        break;
      }
      break;
    case PLAYER_SKILL_BODYBUILDING:
      switch (masteryLevelBeingTaught)
      {
      case 2:
        gold_transaction_amount = 500;
        break;
      case 3:
        if ( activePlayer->GetBaseEndurance() < 50 )
          return pNPCTopics[127].pText;
        gold_transaction_amount = 2500;
        break;
      case 4:
        gold_transaction_amount = 6000;
        break;
      }
      break;
    case PLAYER_SKILL_DIPLOMACY:
      Error("Diplomacy not used");
      break;
    case PLAYER_SKILL_TIEVERY:
      Error("Thievery not used");
      break;
    case PLAYER_SKILL_DODGE:
      switch (masteryLevelBeingTaught)
      {
      case 2:
        gold_transaction_amount = 2000;
        break;
      case 3:
        gold_transaction_amount = 5000;
        break;
      case 4:
        if ( (activePlayer->pActiveSkills[PLAYER_SKILL_UNARMED] & 63) < 0xA )
          return pNPCTopics[127].pText;
        gold_transaction_amount = 8000;
        break;
      }
      break;
    case PLAYER_SKILL_UNARMED:
      switch (masteryLevelBeingTaught)
      {
      case 2:
        gold_transaction_amount = 2000;
        break;
      case 3:
        gold_transaction_amount = 5000;
        break;
      case 4:
        if ( (activePlayer->pActiveSkills[PLAYER_SKILL_DODGE] & 63) < 0xA )
          return pNPCTopics[127].pText;
        gold_transaction_amount = 8000;
        break;
      }
      break;
    case PLAYER_SKILL_LEARNING:
      switch (masteryLevelBeingTaught)
      {
      case 2:
        gold_transaction_amount = 2000;
        break;
      case 3:
        if ( activePlayer->GetBaseIntelligence() < 50 )
          return pNPCTopics[127].pText;
        gold_transaction_amount = 5000;
        break;
      case 4:
        gold_transaction_amount = 8000;
        break;
      }
      break;
    default:
      Error("Unknown skill");
  }
  if ( gold_transaction_amount > pParty->uNumGold )
    return pNPCTopics[124].pText;  //You don't have enough gold!
  contract_approved = 1;
  if ( masteryLevelBeingTaught == 2 )
  {
    sprintfex(pTmpBuf2.data(), pGlobalTXT_LocalizationStrings[534],//Получить степень ^Pr[%s] в навыке ^Pr[%s] за ^I[%lu] золот^L[ой;ых;ых]
              pGlobalTXT_LocalizationStrings[433], pSkillNames[dword_F8B1AC_award_bit_number], gold_transaction_amount);//Эксперт
  }
  else if ( masteryLevelBeingTaught == 3 )
  {
    sprintfex(pTmpBuf2.data(), pGlobalTXT_LocalizationStrings[534],
              pGlobalTXT_LocalizationStrings[432], pSkillNames[dword_F8B1AC_award_bit_number], gold_transaction_amount);//Мастер
  }
  else if ( masteryLevelBeingTaught == 4 )
    sprintfex(pTmpBuf2.data(), pGlobalTXT_LocalizationStrings[534],
              pGlobalTXT_LocalizationStrings[225], pSkillNames[dword_F8B1AC_award_bit_number], gold_transaction_amount);//Великий Магистр
  return pTmpBuf2.data();
}

//----- (004B3E1E) --------------------------------------------------------
void  sub_4B3E1E()
{
  NPCData *v0; // ST40_4@1
  signed int v1; // edi@1
  //GUIWindow *v2; // ecx@1

  __debugbreak();
  v0 = GetNPCData(sDialogue_SpeakingActorNPC_ID);
  v1 = 0;
  pDialogueWindow->eWindowType = WINDOW_MainMenu;
  pDialogueWindow->Release();
  pDialogueWindow = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_Dialogue, 1, 0);
  if ( pNPCStats->pProfessions[v0->uProfession].pBenefits)//*(&pNPCStats->field_13A5C + 5 * v0->uProfession) )
  {
    pDialogueWindow->CreateButton(480, 160, 140, 28, 1, 0, UIMSG_SelectNPCDialogueOption, 77, 0, pGlobalTXT_LocalizationStrings[407], 0);//Подробнее
    v1 = 1;
  }
  pDialogueWindow->CreateButton(480, 30 * v1 + 160, 140, 30, 1, 0, UIMSG_SelectNPCDialogueOption, 76, 0, pGlobalTXT_LocalizationStrings[406], 0);//Нанять
  pDialogueWindow->_41D08F_set_keyboard_control_group(v1 + 1, 1, 0, 1);
}


//----- (004B3FE5) --------------------------------------------------------
//Originally called _4B254D_SkillMasteryTeacher to have contract_approved assigned, to be able to set some button name. 
//But it the name gets immediately overwritten
void _4B3FE5_training_dialogue(int a4)
{
  const char *v2; // edi@1

  //__debugbreak();
  uDialogueType = DIALOGUE_SKILL_TRAINER;
  current_npc_text = (char *)pNPCTopics[a4 + 168].pText;
  _4B254D_SkillMasteryTeacher(a4);  //might be needed because of contract_approved ?
  pDialogueWindow->Release();
  pDialogueWindow = GUIWindow::Create(0, 0, window->GetWidth(), 350, WINDOW_MainMenu, a4, 0);
  pBtn_ExitCancel = pDialogueWindow->CreateButton( 471, 445, 169,  35,   1,  0, UIMSG_Escape, 0,   0,
                 pGlobalTXT_LocalizationStrings[34], pIcons_LOD->GetTexture(uExitCancelTextureId), 0);
  pDialogueWindow->CreateButton(0, 0, 0, 0, 1, 0, UIMSG_BuyInShop_Identify_Repair, 0, 0, "", 0);
  v2 = "";
  if ( contract_approved )
    v2 = pGlobalTXT_LocalizationStrings[535];
  pDialogueWindow->CreateButton(480, 160, 0x8Cu, 0x1Eu, 1, 0, UIMSG_ClickNPCTopic, 0x4Fu, 0, v2, 0);
  pDialogueWindow->_41D08F_set_keyboard_control_group(1, 1, 0, 2);
  dialog_menu_id = HOUSE_DIALOGUE_OTHER;
}
// F8B19C: using guessed type int dword_F8B19C;
// F8B1A8: using guessed type int dword_F8B1A8;

//----- (004B46A5) --------------------------------------------------------
void __fastcall DrawTextAtStatusBar( const char *Str, int a5 )
{
  pRenderer->DrawTextureRGB(0, 352, pTexture_StatusBar);
  pPrimaryWindow->DrawText(pFontLucida, pFontLucida->AlignText_Center(450, Str) + 11, 357, a5, Str, 0, 0, 0);
}

//----- (004B46F8) --------------------------------------------------------
__int64 GetExperienceRequiredForLevel(int level)
{
  __int64 v1; // eax@1
  int i; // edx@1

  v1 = 0;
  for ( i = 0; i < level; ++i )
    v1 += i + 1;
  return 1000 * v1;
}

//----- (004BC49B) --------------------------------------------------------
void OnSelectNPCDialogueOption(DIALOGUE_TYPE newDialogueType)
{
  NPCData *speakingNPC; // ebp@1
  int npc_event_id; // ecx@10
  char *v13; // [sp-8h] [bp-18h]@60

  speakingNPC = GetNPCData(sDialogue_SpeakingActorNPC_ID);
  uDialogueType = newDialogueType;
  if (!speakingNPC->uFlags)
    speakingNPC->uFlags = 1;
  if(newDialogueType == DIALOGUE_PROFESSION_DETAILS)
    dialogue_show_profession_details = ~dialogue_show_profession_details;
  else if(newDialogueType == DIALOGUE_76)
  {
    if (speakingNPC->Hired())
    {
      if ( (signed int)pNPCStats->uNumNewNPCs > 0 )
      {
        for ( uint i = 0; i < (unsigned int)pNPCStats->uNumNewNPCs; ++i )
        {
          if ( pNPCStats->pNewNPCData[i].uFlags & 0x80 && !strcmp(speakingNPC->pName, pNPCStats->pNewNPCData[i].pName) )
            pNPCStats->pNewNPCData[i].uFlags &= 0x7Fu;
        }
      }
      if ( pParty->pHirelings[0].pName && !_stricmp(pParty->pHirelings[0].pName, speakingNPC->pName) )
        memset(&pParty->pHirelings[0], 0, sizeof(NPCData));
      else if ( pParty->pHirelings[1].pName && !_stricmp(pParty->pHirelings[1].pName, speakingNPC->pName) )
        memset(&pParty->pHirelings[1], 0, sizeof(NPCData));
      pParty->hirelingScrollPosition = 0;
      pParty->CountHirelings();
      dword_591084 = 0;
      pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, 0);
      dword_7241C8 = 0;
      return;
    }
    if ( pParty->pHirelings[0].pName && pParty->pHirelings[1].pName )
      ShowStatusBarString(pGlobalTXT_LocalizationStrings[533], 2);// ""I cannot join you, you're party is full""
    else
    {
      if ( speakingNPC->uProfession != 51 ) //burglars have no hiring price
      {
        if ( pParty->uNumGold < pNPCStats->pProfessions[speakingNPC->uProfession].uHirePrice )
        {
          ShowStatusBarString(pGlobalTXT_LocalizationStrings[155], 2);// "You don't have enough gold"
          dialogue_show_profession_details = false;
          uDialogueType = 13;
          if ( uActiveCharacter )
            pPlayers[uActiveCharacter]->PlaySound(SPEECH_NotEnoughGold, 0);
          if ( !dword_7241C8 )
            pGame->Draw();
          dword_7241C8 = 0;
          return;
        }
        Party::TakeGold(pNPCStats->pProfessions[speakingNPC->uProfession].uHirePrice);
      }
      LOBYTE(speakingNPC->uFlags) |= 0x80u;
      if ( pParty->pHirelings[0].pName )
      {
        memcpy(&pParty->pHirelings[1], speakingNPC, sizeof(pParty->pHirelings[1]));
        v13 = pParty->pHireling2Name;
      }
      else
      {
        memcpy(&pParty->pHirelings[0], speakingNPC, sizeof(pParty->pHirelings[0]));
        v13 = pParty->pHireling1Name;
      }
      strcpy(v13, speakingNPC->pName);
      pParty->hirelingScrollPosition = 0;
      pParty->CountHirelings();
      pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, 0);
      if ( sDialogue_SpeakingActorNPC_ID >= 0 )
        pDialogue_SpeakingActor->uAIState = Removed;
      if ( uActiveCharacter )
        pPlayers[uActiveCharacter]->PlaySound(SPEECH_61, 0);
    }
  }
  else if ( (signed int)newDialogueType > DIALOGUE_84 && (signed int)newDialogueType <= DIALOGUE_ARENA_SELECT_CHAMPION ) //выбор уровня сложности боя
  {
    ArenaFight();
    return;
  }
  else if(newDialogueType == DIALOGUE_USE_NPC_ABILITY)
  {
    if (UseNPCSkill((NPCProf)speakingNPC->uProfession) == 0)
    {
      if ( speakingNPC->uProfession != GateMaster )
        speakingNPC->bHasUsedTheAbility = 1;
      pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, 0);
    }
    else
      ShowStatusBarString(pGlobalTXT_LocalizationStrings[140], 2); //"Your packs are already full!"
  }
  else if(newDialogueType == DIALOGUE_13)
  {
    if (!speakingNPC->Hired())
    {
      sub_4B3E1E();
      dialogue_show_profession_details = false;
    }
    else
    {
      for ( uint i = 0; i < (signed int)pNPCStats->uNumNewNPCs; ++i )
      {
        if ( pNPCStats->pNewNPCData[i].uFlags & 0x80 && !strcmp(speakingNPC->pName, pNPCStats->pNewNPCData[i].pName) )
          pNPCStats->pNewNPCData[i].uFlags &= 0x7Fu;
      }
      if ( pParty->pHirelings[0].pName && !_stricmp(pParty->pHirelings[0].pName, speakingNPC->pName) )
        memset(&pParty->pHirelings[0], 0, sizeof(NPCData));
      else if ( pParty->pHirelings[1].pName && !_stricmp(pParty->pHirelings[1].pName, speakingNPC->pName) )
        memset(&pParty->pHirelings[1], 0, sizeof(NPCData));
      pParty->hirelingScrollPosition = 0;
      pParty->CountHirelings();
      dword_591084 = 0;
      pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, 0);
      dword_7241C8 = 0;
      return;
    }
  }
  else if(newDialogueType >= DIALOGUE_EVT_A && newDialogueType <= DIALOGUE_EVT_F)
  {
    switch(newDialogueType)
    {
      case DIALOGUE_EVT_A:  npc_event_id = speakingNPC->evt_A; break;
      case DIALOGUE_EVT_B:  npc_event_id = speakingNPC->evt_B; break;
      case DIALOGUE_EVT_C:  npc_event_id = speakingNPC->evt_C; break;
      case DIALOGUE_EVT_D:  npc_event_id = speakingNPC->evt_D; break;
      case DIALOGUE_EVT_E:  npc_event_id = speakingNPC->evt_E; break;
      case DIALOGUE_EVT_F:  npc_event_id = speakingNPC->evt_F; break;
    }
    if ( (npc_event_id >= 200) && (npc_event_id <= 310) )
      _4B3FE5_training_dialogue(npc_event_id); //200-310
    else if (( npc_event_id >= 400) && (npc_event_id <= 410) )
    { //400-410
      dword_F8B1D8 = newDialogueType;
      DrawJoinGuildWindow(npc_event_id - 400);
    }
    else
    {
      switch ( npc_event_id )
      {
        case 139:
          OracleDialogue();
          break;
        case 311:
          CheckBountyRespawnAndAward();
          break;
        case 399:
          Arena_SelectionFightLevel();
          break;
        default:
          activeLevelDecoration = (LevelDecoration*)1;
          current_npc_text = 0;
          EventProcessor(npc_event_id, 0, 1);
          activeLevelDecoration = nullptr;
          break;
      }
    }
  }
  if ( !dword_7241C8 )
    pGame->Draw();
  dword_7241C8 = 0;
}

