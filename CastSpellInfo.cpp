#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define _CRT_SECURE_NO_WARNINGS
#include "CastSpellInfo.h"
#include "Actor.h"
#include "Party.h"
#include "mm7_data.h"
#include "MM7.h"
#include "BSPModel.h"
#include "Mouse.h"
#include "stru6.h"
#include "Game.h"
#include "GUIWindow.h"
#include "GUIFont.h"
#include "AudioPlayer.h"
#include "Outdoor.h"
#include "Overlays.h"
#include "Events.h"
#include "Viewport.h"
#include "OurMath.h"
#include "SpriteObject.h"
#include "ObjectList.h"
#include "stru123.h"
#include "Timer.h"
#include "IconFrameTable.h"
#include "Awards.h"
#include "TurnEngine.h"
#include "texts.h"
#include "LOD.h"
#include "Level/Decoration.h"
#include "mm7_unsorted_subs.h"
#include "UI\UIPartyCreation.h"

const size_t CastSpellInfoCount = 10;
std::array<CastSpellInfo, CastSpellInfoCount> pCastSpellInfo;

//----- (00427E01) --------------------------------------------------------
void CastSpellInfoHelpers::_427E01_cast_spell()
{
  int spell_level; // edi@1
  CastSpellInfo *pCastSpell; // ebx@2
  signed int v6; // eax@14
  unsigned __int16 v11; // cx@45
  signed int i; // esi@76
//  Actor *pActor; // edi@177
  int v61; // ecx@184
  int v111; // eax@274
  int dist_X; // eax@278
//  int new_dist_Y;
  int dist_Z;
//  int v116; // edx@279
//  int v117; // edx@281
//  int v118; // edx@283
  int v169; // eax@344
  int v188; // esi@369
  int v189; // edi@369
  int v206; // eax@407
  double v241; // st7@478
  ItemGen *v245; // edi@492
  int v258; // ecx@514
  char v259; // al@516
  int v260; // eax@518
  int v261; // esi@519
  int v262; // edx@521
  int *v263; // ecx@521
  int v264; // esi@521
  int v265; // edx@521
  int *ii; // eax@522
  int v267; // eax@524
  int v268; // eax@524
  int v278; // ecx@548
  char v279; // al@550
  int v280; // eax@552
  int *v281; // esi@553
  int v282; // edx@555
  int *v283; // ecx@555
  int v284; // esi@555
  int v285; // edx@555
  int *l; // eax@556
  int v295; // edx@575
  char v313; // al@606pGame->GetStru6()
  const char *v317; // ecx@617
//  int v396; // eax@752
  __int16 v448; // ax@864
  DDM_DLV_Header *v613; // eax@1108
  int v642; // edi@1156
  int v643; // eax@1156
  int v659; // [sp+0h] [bp-E84h]@123
  unsigned __int64 v663; // [sp+0h] [bp-E84h]@639
  int v666; // [sp+4h] [bp-E80h]@12
  PLAYER_SKILL_TYPE v667; // [sp+4h] [bp-E80h]@25
  int v679[800]; // [sp+14h] [bp-E70h]@515
  unsigned __int64 v685; // [sp+D08h] [bp-17Ch]@416
  unsigned __int64 v687; // [sp+D24h] [bp-160h]@327
  Vec3_int_ v688; // [sp+D2Ch] [bp-158h]@943
  Vec3_int_ v691; // [sp+D38h] [bp-14Ch]@137
  Vec3_int_ v694; // [sp+D44h] [bp-140h]@982
  Vec3_int_ v697; // [sp+D50h] [bp-134h]@129
  Vec3_int_ v700; // [sp+D5Ch] [bp-128h]@339
  Vec3_int_ v701; // [sp+D68h] [bp-11Ch]@286
  Vec3_int_ v704; // [sp+D74h] [bp-110h]@132
  Vec3_int_ v707; // [sp+D80h] [bp-104h]@1127
  int v710; // [sp+D8Ch] [bp-F8h]@1156
  int n; // [sp+DA0h] [bp-E4h]@1
  AIDirection v715; // [sp+DA4h] [bp-E0h]@21
  int mon_id;
  int dist_Y; // [sp+DD8h] [bp-ACh]@163
  int v723; // [sp+E4Ch] [bp-38h]@1
  ItemGen *_this; // [sp+E50h] [bp-34h]@23
  int v725; // [sp+E54h] [bp-30h]@23
  int buff_resist;
  bool spell_sound_flag; // [sp+E5Ch] [bp-28h]@1
  Player *pPlayer; // [sp+E64h] [bp-20h]@8
  int v730; // [sp+E68h] [bp-1Ch]@53
  ItemGen *v730c;
  int skill_level; // [sp+E6Ch] [bp-18h]@48
  signed int v732; // [sp+E70h] [bp-14h]@325
  unsigned __int64 v733; // [sp+E74h] [bp-10h]@1
  int duration;
  signed int a2; // [sp+E7Ch] [bp-8h]@14
  int amount; // [sp+E80h] [bp-4h]@1
  int obj_type;
  ItemDesc* _item;

  SpriteObject pSpellSprite; // [sp+DDCh] [bp-A8h]@1

  spell_level = 0;
  amount = 0;
  LODWORD(v733) = 0;
  v723 = 0;
  spell_sound_flag = false;
  for(n = 0; n < CastSpellInfoCount; ++n)
  {
    pCastSpell = &pCastSpellInfo[n];
    HIDWORD(v733) = (int)pCastSpell;
    if ( pCastSpell->uSpellID == 0 )
      continue;

    if (pParty->Invisible())
      pParty->pPartyBuffs[PARTY_BUFF_INVISIBILITY].Reset();

    if ( pCastSpell->uFlags & 0x3CA )
    {
      if ( !pParty->pPlayers[pCastSpell->uPlayerID].CanAct() )
        _427D48();
      continue;
    }
    pPlayer = &pParty->pPlayers[pCastSpell->uPlayerID];

    a2 = pCastSpell->spell_target_pid;
    if (!pCastSpell->spell_target_pid)
    {
      if (pCastSpell->uSpellID == SPELL_LIGHT_DESTROY_UNDEAD ||
          pCastSpell->uSpellID == SPELL_SPIRIT_TURN_UNDEAD ||
          pCastSpell->uSpellID == SPELL_DARK_CONTROL_UNDEAD )
        v666 = 1;
      else
        v666 = 0;

      a2 = stru_50C198.FindClosestActor(5120, 1, v666);
      v6 = pMouse->uPointingObjectID;
      if ( pMouse->uPointingObjectID && PID_TYPE(v6) == OBJECT_Actor && pActors[PID_ID(v6)].CanAct() )
        a2 = pMouse->uPointingObjectID;
    }


    pSpellSprite.uType = stru_4E3ACC[pCastSpell->uSpellID].uType;
    if (pSpellSprite.uType)
    {
      if (PID_TYPE(a2) == OBJECT_Actor)
      {
        Actor::GetDirectionInfo(PID(OBJECT_Player, pCastSpell->uPlayerID + 1), a2, &v715, 0);
        spell_level = v723;
      }
      else
      {
        v715.uYawAngle = pParty->sRotationY;
        v715.uPitchAngle = pParty->sRotationX;
      }
    }

    v725 = 0;
    _this = 0;
    if (pCastSpell->forced_spell_skill_level)
    {
      v11 = pCastSpell->forced_spell_skill_level;
      v723 = v11 & 0x3F; // 6 bytes
      spell_level = v723;
    }
    else
    {
      //v667 = PLAYER_SKILL_STAFF;
      if (pCastSpell->uSpellID < SPELL_AIR_WIZARD_EYE)
        v667 = PLAYER_SKILL_FIRE;
      else if (pCastSpell->uSpellID < SPELL_WATER_AWAKEN)
        v667 = PLAYER_SKILL_AIR;
      else if (pCastSpell->uSpellID < SPELL_EARTH_STUN)
        v667 = PLAYER_SKILL_WATER;
      else if (pCastSpell->uSpellID < SPELL_SPIRIT_DETECT_LIFE)
        v667 = PLAYER_SKILL_EARTH;
      else if (pCastSpell->uSpellID < SPELL_MIND_REMOVE_FEAR)
        v667 = PLAYER_SKILL_SPIRIT;
      else if (pCastSpell->uSpellID < SPELL_BODY_CURE_WEAKNESS)
        v667 = PLAYER_SKILL_MIND;
      else if (pCastSpell->uSpellID < SPELL_LIGHT_LIGHT_BOLT)
        v667 = PLAYER_SKILL_BODY;
      else if (pCastSpell->uSpellID < SPELL_DARK_REANIMATE)
        v667 = PLAYER_SKILL_LIGHT;
      else if (pCastSpell->uSpellID < SPELL_BOW_ARROW)
        v667 = PLAYER_SKILL_DARK;
      else if (pCastSpell->uSpellID == SPELL_BOW_ARROW)
        v667 = PLAYER_SKILL_BOW;
      else if (pCastSpell->uSpellID == SPELL_101 || pCastSpell->uSpellID == SPELL_LASER_PROJECTILE )
        v667 = PLAYER_SKILL_BLASTER;
      else assert(false && "Unknown spell");

      v725 = v667;
      v723 = pPlayer->GetActualSkillLevel(v667) & 0x3F;
      spell_level = v723;
      v11 = pPlayer->pActiveSkills[v725];
    }

    skill_level = SkillToMastery(v11);
    if (pCastSpell->uSpellID < SPELL_BOW_ARROW )
    {
      if (pCastSpell->forced_spell_skill_level)
        uRequiredMana = 0;
      else 
        uRequiredMana = pSpellDatas[pCastSpell->uSpellID].mana_per_skill[skill_level - 1];
      sRecoveryTime = pSpellDatas[pCastSpell->uSpellID].recovery_per_skill[skill_level - 1];
    }

    if (v725 == PLAYER_SKILL_DARK && pParty->uCurrentHour == 0 && pParty->uCurrentMinute == 0 ||
        v725 == PLAYER_SKILL_LIGHT && pParty->uCurrentHour == 12 && pParty->uCurrentMinute == 0)
      uRequiredMana = 0;

    if (pCastSpell->uSpellID < SPELL_BOW_ARROW && pPlayer->sMana < uRequiredMana)
    {
      ShowStatusBarString(pGlobalTXT_LocalizationStrings[586], 2); // "Not enough spell points"
      pCastSpell->uSpellID = 0;
      continue;
    }
    v730 = pCastSpell->uSpellID;
    if (pPlayer->IsCursed() && pCastSpell->uSpellID < SPELL_BOW_ARROW && rand() % 100 < 50)//��������� ����������
    {
      if (!pParty->bTurnBasedModeOn)
        pPlayer->SetRecoveryTime((signed __int64)(flt_6BE3A4_debug_recmod1 * 213.3333333333333));
      else
      {
        pParty->pTurnBasedPlayerRecoveryTimes[pCastSpellInfo[n].uPlayerID] = 100;
        pPlayer->SetRecoveryTime(sRecoveryTime);
        pTurnEngine->ApplyPlayerAction();
      }
      ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2); // "Spell failed"
      pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
      pCastSpellInfo[n].uSpellID = 0;
      pPlayer->sMana -= uRequiredMana;
      return;
    }

    switch ( pCastSpell->uSpellID )
    {
      case SPELL_101:
        assert(false && "Unknown spell effect #101 (prolly flaming bow arrow");
      case SPELL_BOW_ARROW://�������� �� ����
      {
        amount = 1;
        if ( SkillToMastery(pPlayer->pActiveSkills[PLAYER_SKILL_BOW]) >= 3 )
          amount = 2;
        sRecoveryTime = pPlayer->GetAttackRecoveryTime(true);
        pSpellSprite.stru_24.Reset();
        pSpellSprite.spell_level = spell_level;
        pSpellSprite.spell_id = pCastSpell->uSpellID;
        pSpellSprite.spell_skill = skill_level;
        pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
        if ( pPlayer->WearsItem(ITEM_ARTEFACT_ULLYSES, EQUIP_BOW) )
          pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(0xBD6u);
        pSpellSprite.vPosition.x = pParty->vPosition.x;
        pSpellSprite.vPosition.y = pParty->vPosition.y;
        pSpellSprite.uAttributes = 0;
        pSpellSprite.uSpriteFrameID = 0;
        pSpellSprite.vPosition.z = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3;
        pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
        pSpellSprite.spell_target_pid = a2;
        pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
        pSpellSprite.uFacing = LOWORD(v715.uYawAngle);
        pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
        pPlayer = &pParty->pPlayers[pCastSpell->uPlayerID];
        memcpy(&pSpellSprite.stru_24, &pPlayer->pInventoryItemList[pPlayer->pEquipment.uBow-1], sizeof(pSpellSprite.stru_24));
        pSpellSprite.uAttributes = 256;
        if ( pParty->bTurnBasedModeOn == 1 )
          pSpellSprite.uAttributes = 260;
        for ( i = 0; i < amount; ++i )
        {
          if ( i )
            pSpellSprite.vPosition.z += 32;
          pSpellSprite.uSectorID = pIndoor->GetSector(pSpellSprite.vPosition.x, pSpellSprite.vPosition.y, pSpellSprite.vPosition.z);
          if ( pSpellSprite.Create(v715.uYawAngle, v715.uPitchAngle, pObjectList->pObjects[(signed __int16)pSpellSprite.uObjectDescID].uSpeed,
            pCastSpell->uPlayerID + 1) != -1 && pParty->bTurnBasedModeOn == 1 )
          ++pTurnEngine->pending_actions;
        }
        break;
      }
      case SPELL_LASER_PROJECTILE://�������� �� ��������
      {
        sRecoveryTime = pPlayer->GetAttackRecoveryTime(0);
        pSpellSprite.stru_24.Reset();
        pSpellSprite.spell_id = pCastSpell->uSpellID;
        pSpellSprite.spell_level = v723;
        pSpellSprite.spell_skill = skill_level;
        pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
        pSpellSprite.vPosition.x = pParty->vPosition.x;
        pSpellSprite.vPosition.y = pParty->vPosition.y;
        pSpellSprite.uAttributes = 0;
        pSpellSprite.uSpriteFrameID = 0;
        pSpellSprite.vPosition.z = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 2;
        pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
        pSpellSprite.spell_target_pid = a2;
        pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
        pSpellSprite.uFacing = LOWORD(v715.uYawAngle);
        pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
        pPlayer = &pParty->pPlayers[pCastSpell->uPlayerID];
        memcpy(&pSpellSprite.stru_24, &pPlayer->pInventoryItemList[pPlayer->pEquipment.uMainHand - 1],sizeof(pSpellSprite.stru_24));
        //	&pParty->pPlayers[pCastSpell->uPlayerID].spellbook.pDarkSpellbook.bIsSpellAvailable[36
        //		* pParty->pPlayers[pCastSpell->uPlayerID].pEquipment.uMainHand + 5], );
        pSpellSprite.uSectorID = pIndoor->GetSector(pParty->vPosition.x, pParty->vPosition.y, pSpellSprite.vPosition.z);
        HIBYTE(pSpellSprite.uAttributes) |= 1;
        if ( pParty->bTurnBasedModeOn == 1 )
          LOBYTE(pSpellSprite.uAttributes) |= 4;
        if ( pSpellSprite.Create(v715.uYawAngle, v715.uPitchAngle, pObjectList->pObjects[(signed __int16)pSpellSprite.uObjectDescID].uSpeed,
                                 pCastSpell->uPlayerID + 1) != -1 && pParty->bTurnBasedModeOn == 1 )
          ++pTurnEngine->pending_actions;
        break;
      }
      case SPELL_FIRE_TORCH_LIGHT://���� ������
      {
        switch (skill_level)
        {
          case 1: amount = 2; break;
          case 2: amount = 3; break;
          case 3:
          case 4: amount = 4; break;
          default:
            assert(false);
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        pParty->pPartyBuffs[PARTY_BUFF_TORCHLIGHT].Apply(pParty->uTimePlayed + (signed __int64)((double)(signed int)3600 * spell_level * 4.2666669), skill_level, amount, 0, 0);
        spell_sound_flag = true;
        break;
      }
      case SPELL_FIRE_FIRE_SPIKE://�������� ���
      {
        switch (skill_level)
        {
          case 1: amount = 3; break;
          case 2: amount = 5; break;
          case 3: amount = 7; break;
          case 4: amount = 9; break;
          default:
            assert(false);
        }
        int _v733 = 0;
        for (uint i = 0; i < uNumSpriteObjects; ++i)
        {
          SpriteObject* object = &pSpriteObjects[i];
          if (object->uType && object->spell_id == SPELL_FIRE_FIRE_SPIKE && object->spell_caster_pid == PID(OBJECT_Player, pCastSpell->uPlayerID))
            ++_v733;
        }
        if ( _v733 > amount )
        {
          ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2); // Spell failed
          pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
          pCastSpell->uSpellID = 0;
          continue;
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        pSpellSprite.stru_24.Reset();
        pSpellSprite.spell_id = pCastSpell->uSpellID;
        pSpellSprite.spell_level = spell_level;
        pSpellSprite.spell_skill = skill_level;
        pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
        pSpellSprite.vPosition.y = pParty->vPosition.y;
        pSpellSprite.vPosition.x = pParty->vPosition.x;
        pSpellSprite.uAttributes = 0;
        pSpellSprite.vPosition.z = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3;
        pSpellSprite.uSectorID = pIndoor->GetSector(pParty->vPosition.x, pParty->vPosition.y, pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3);
        pSpellSprite.uSpriteFrameID = 0;
        pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
        pSpellSprite.spell_target_pid = a2;
        pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
        pSpellSprite.uFacing = LOWORD(v715.uYawAngle);
        pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
        if ( pParty->bTurnBasedModeOn == 1 )
          LOBYTE(pSpellSprite.uAttributes) |= 4;
        v659 = pObjectList->pObjects[(signed __int16)pSpellSprite.uObjectDescID].uSpeed;
        if ( pSpellSprite.Create(pParty->sRotationY, pParty->sRotationX + 10, v659, pCastSpell->uPlayerID + 1) != -1 && pParty->bTurnBasedModeOn == 1 )
          ++pTurnEngine->pending_actions;
        spell_sound_flag = true;
        break;
      }
      case SPELL_AIR_IMPLOSION://������ �����
      {
        mon_id = PID_ID(a2);
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        if (!a2)
        {
          ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2); // Spell failed
          pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
          pCastSpell->uSpellID = 0;
          continue;
        }
        if (PID_TYPE(a2) == OBJECT_Actor)
        {
          v697.x = 0;
          v697.y = 0;
          v697.z = 0;
          pSpellSprite.stru_24.Reset();
          pSpellSprite.spell_id = pCastSpell->uSpellID;
          pSpellSprite.spell_level = spell_level;
          pSpellSprite.spell_skill = skill_level;
          pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
          pSpellSprite.uAttributes = 0;
          pSpellSprite.uSectorID = 0;
          pSpellSprite.uSpriteFrameID = 0;
          pSpellSprite.field_60_distance_related_prolly_lod = 0;
          pSpellSprite.uFacing = 0;
          pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
          pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
          pSpellSprite.vPosition.x = pActors[mon_id].vPosition.x;
          pSpellSprite.vPosition.z = pActors[mon_id].vPosition.z;
          pSpellSprite.vPosition.y = pActors[mon_id].vPosition.y;
          pSpellSprite.spell_target_pid = PID(OBJECT_Actor, mon_id);
          Actor::DamageMonsterFromParty(PID(OBJECT_Item, pSpellSprite.Create(0, 0, 0, 0)), mon_id, &v697);
        }
        spell_sound_flag = true;
        break;
      }
      case SPELL_EARTH_MASS_DISTORTION://��������� ����
      {
        mon_id = PID_ID(a2);
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        if ( pActors[mon_id].DoesDmgTypeDoDamage((DAMAGE_TYPE)3) )
        {
          pActors[mon_id].pActorBuffs[ACTOR_BUFF_MASS_DISTORTION].Apply(pMiscTimer->uTotalGameTimeElapsed + 128, 0, 0, 0, 0);
          v704.x = 0;
          v704.y = 0;
          v704.z = 0;
          pSpellSprite.stru_24.Reset();
          pSpellSprite.spell_id = pCastSpell->uSpellID;
          pSpellSprite.spell_level = spell_level;
          pSpellSprite.spell_skill = skill_level;
          pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
          pSpellSprite.uAttributes = 0;
          pSpellSprite.uSectorID = 0;
          pSpellSprite.uSpriteFrameID = 0;
          pSpellSprite.field_60_distance_related_prolly_lod = 0;
          pSpellSprite.uFacing = 0;
          pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
          pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
          pSpellSprite.vPosition.x = pActors[mon_id].vPosition.x;
          pSpellSprite.vPosition.y = pActors[mon_id].vPosition.y;
          pSpellSprite.vPosition.z = pActors[mon_id].vPosition.z;
          pSpellSprite.spell_target_pid = PID(OBJECT_Actor, mon_id);
          Actor::DamageMonsterFromParty(PID(OBJECT_Item, pSpellSprite.Create(0, 0, 0, 0)), mon_id, &v704);
        }
        spell_sound_flag = true;
        break;
      }
      case SPELL_LIGHT_DESTROY_UNDEAD://����������� ������
      {
        if ( !pPlayer->CanCastSpell(uRequiredMana) || !a2 || PID_TYPE(a2) != OBJECT_Actor)
          break;
        //v730 = a2 >> 3;
        //HIDWORD(v733) = (int)&pActors[PID_ID(a2)];
        v691.x = 0;
        v691.y = 0;
        v691.z = 0;
        pSpellSprite.stru_24.Reset();
        pSpellSprite.spell_id = pCastSpell->uSpellID;
        pSpellSprite.spell_level = spell_level;
        pSpellSprite.spell_skill = skill_level;
        pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
        pSpellSprite.vPosition.x = pActors[PID_ID(a2)].vPosition.x;
        pSpellSprite.vPosition.y = pActors[PID_ID(a2)].vPosition.y;
        pSpellSprite.vPosition.z = pActors[PID_ID(a2)].vPosition.z;
        pSpellSprite.uAttributes = 0;
        pSpellSprite.uSectorID = pIndoor->GetSector(pSpellSprite.vPosition.x, pSpellSprite.vPosition.y, pSpellSprite.vPosition.z);
        pSpellSprite.uSpriteFrameID = 0;
        pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
        pSpellSprite.spell_target_pid = a2;
        pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
        pSpellSprite.uFacing = LOWORD(v715.uYawAngle);
        pSpellSprite.uAttributes |= 0x80u;
        pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
        int obj_id = pSpellSprite.Create(0, 0, 0, 0);
        if ( !MonsterStats::BelongsToSupertype(pActors[PID_ID(a2)].pMonsterInfo.uID, MONSTER_SUPERTYPE_UNDEAD) )
        {
          ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2); // Spell failed
          pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
          pCastSpell->uSpellID = 0;
          continue;
        }
        Actor::DamageMonsterFromParty(PID(OBJECT_Item, obj_id), PID_ID(a2), &v691);
        spell_sound_flag = true;
        break;
      }
      case SPELL_FIRE_FIRE_BOLT://���� ����
      case SPELL_FIRE_FIREBALL://�������� ���
      case SPELL_FIRE_INCINERATE://�����������
      case SPELL_AIR_LIGHNING_BOLT://���� ������
      case SPELL_WATER_ICE_BOLT://������� ������
      case SPELL_WATER_ICE_BLAST://������� �����
      case SPELL_EARTH_STUN://��������
      case SPELL_EARTH_DEADLY_SWARM://��� ������
      case SPELL_MIND_MIND_BLAST://���� �����
      case SPELL_MIND_PSYCHIC_SHOCK://����������� ���
      case SPELL_BODY_HARM://����
      case SPELL_LIGHT_LIGHT_BOLT://��� �����
      case SPELL_DARK_DRAGON_BREATH://������� �������
      {
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        pSpellSprite.stru_24.Reset();
        pSpellSprite.spell_id = pCastSpell->uSpellID;
        pSpellSprite.spell_level = spell_level;
        pSpellSprite.spell_skill = skill_level;
        pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
        pSpellSprite.vPosition.y = pParty->vPosition.y;
        pSpellSprite.vPosition.x = pParty->vPosition.x;
        pSpellSprite.uAttributes = 0;
        pSpellSprite.vPosition.z = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3;
        if (uCurrentlyLoadedLevelType == LEVEL_Indoor)
          pSpellSprite.uSectorID = pIndoor->GetSector(pParty->vPosition.x, pParty->vPosition.y, pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3);
        else
          pSpellSprite.uSectorID = 0;
        pSpellSprite.uSpriteFrameID = 0;
        pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
        pSpellSprite.spell_target_pid = a2;
        pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
        pSpellSprite.uFacing = LOWORD(v715.uYawAngle);
        pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
        if ( pParty->bTurnBasedModeOn == 1 )
          LOBYTE(pSpellSprite.uAttributes) |= 4;
        if ( pCastSpell->uSpellID == SPELL_AIR_LIGHNING_BOLT )
          LOBYTE(pSpellSprite.uAttributes) |= 0x40;
        v659 = pObjectList->pObjects[(signed __int16)pSpellSprite.uObjectDescID].uSpeed;
        if ( pSpellSprite.Create(v715.uYawAngle, v715.uPitchAngle, v659, pCastSpell->uPlayerID + 1) != -1 && pParty->bTurnBasedModeOn == 1 )
          ++pTurnEngine->pending_actions;
        spell_sound_flag = true;
        break;
      }
      case SPELL_WATER_ACID_BURST://������� �������
      case SPELL_EARTH_BLADES://������
      case SPELL_BODY_FLYING_FIST://�������� �����
      case SPELL_DARK_TOXIC_CLOUD://������-������
      {
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        pSpellSprite.stru_24.Reset();
        pSpellSprite.spell_id = pCastSpell->uSpellID;
        pSpellSprite.spell_level = spell_level;
        pSpellSprite.spell_skill = skill_level;
        pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
        pSpellSprite.uAttributes = 0;
        pSpellSprite.vPosition.x = pParty->vPosition.x;
        pSpellSprite.vPosition.y = pParty->vPosition.y;
        pSpellSprite.vPosition.z = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 2;
        pSpellSprite.uSectorID = pIndoor->GetSector(pParty->vPosition.x, pParty->vPosition.y, pSpellSprite.vPosition.z);
        pSpellSprite.uSpriteFrameID = 0;
        pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
        pSpellSprite.spell_target_pid = a2;
        pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
        pSpellSprite.uFacing = LOWORD(v715.uYawAngle);
        pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
        if ( pParty->bTurnBasedModeOn == 1 )
          LOBYTE(pSpellSprite.uAttributes) |= 4;
        v659 = pObjectList->pObjects[(signed __int16)pSpellSprite.uObjectDescID].uSpeed;
        if ( pSpellSprite.Create(v715.uYawAngle, v715.uPitchAngle, v659, pCastSpell->uPlayerID + 1) != -1 && pParty->bTurnBasedModeOn == 1 )
          ++pTurnEngine->pending_actions;
        spell_sound_flag = true;
        break;
      }
      case SPELL_LIGHT_SUNRAY://��� ������
      {
        if ( uCurrentlyLoadedLevelType == LEVEL_Indoor
          || uCurrentlyLoadedLevelType == LEVEL_Outdoor && (pParty->uCurrentHour < 5 || pParty->uCurrentHour >= 21) )//������ � ������� �����
        {
          ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2); // Spell failed
          pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
          pCastSpell->uSpellID = 0;
          continue;
        }
        if ( pPlayer->CanCastSpell(uRequiredMana) )
        {
          pSpellSprite.stru_24.Reset();
          pSpellSprite.spell_id = pCastSpell->uSpellID;
          pSpellSprite.spell_skill = skill_level;
          pSpellSprite.spell_level = spell_level;
          pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
          pSpellSprite.uAttributes = 0;
          pSpellSprite.vPosition.x = pParty->vPosition.x;
          pSpellSprite.vPosition.y = pParty->vPosition.y;
          pSpellSprite.vPosition.z = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3;
          pSpellSprite.uSectorID = pIndoor->GetSector(pParty->vPosition.x, pParty->vPosition.y, pSpellSprite.vPosition.z);
          pSpellSprite.uSpriteFrameID = 0;
          pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
          pSpellSprite.spell_target_pid = a2;
          pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
          pSpellSprite.uFacing = LOWORD(v715.uYawAngle);
          pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
          if ( pParty->bTurnBasedModeOn == 1 )
            LOBYTE(pSpellSprite.uAttributes) |= 4;
          v659 = pObjectList->pObjects[(signed __int16)pSpellSprite.uObjectDescID].uSpeed;
          if ( pSpellSprite.Create(v715.uYawAngle, v715.uPitchAngle, v659, pCastSpell->uPlayerID + 1) != -1 && pParty->bTurnBasedModeOn == 1 )
            ++pTurnEngine->pending_actions;
          spell_sound_flag = true;
        }
        break;
      }
      case SPELL_LIGHT_PARALYZE://�������
      {
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        mon_id = PID_ID(a2);
        if (PID_TYPE(a2) == OBJECT_Actor && pActors[mon_id].DoesDmgTypeDoDamage((DAMAGE_TYPE)9) )
        {
          Actor::AI_Stand(PID_ID(a2), 4, 0x80, 0);
          pActors[mon_id].pActorBuffs[ACTOR_BUFF_PARALYZED].Apply(pParty->uTimePlayed + (signed __int64)((double)(signed int)23040 * spell_level * 0.033333335), skill_level, 0, 0, 0);
          pActors[mon_id].uAttributes |= 0x80000;
          pActors[mon_id].vVelocity.x = 0;
          pActors[mon_id].vVelocity.y = 0;
          pGame->GetStru6()->_4A7E89_sparkles_on_actor_after_it_casts_buff(&pActors[mon_id], 0);
        }
        spell_sound_flag = true;
        break;
      }
      case SPELL_EARTH_SLOW://����������
      {
        switch (skill_level)
        {
          case 1: LODWORD(v733) = 180 * spell_level; amount = 2; break;//LODWORD(v733)???�� ����������� �����
          case 2: LODWORD(v733) = 300 * spell_level; amount = 2; break;
          case 3: LODWORD(v733) = 300 * spell_level; amount = 4; break;
          case 4: LODWORD(v733) = 300 * spell_level; amount = 8; break;
          default:
            assert(false);
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        //v721 = 836 * PID_ID(a2);
        mon_id = PID_ID(a2);
        if (PID_TYPE(a2) == OBJECT_Actor && pActors[mon_id].DoesDmgTypeDoDamage((DAMAGE_TYPE)3) )
        {
          pActors[mon_id].pActorBuffs[ACTOR_BUFF_SLOWED].Apply(pParty->uTimePlayed + (signed __int64)((double)(signed int)23040 * spell_level * 0.033333335), skill_level, amount, 0, 0);
          pActors[mon_id].uAttributes |= 0x80000;
          pGame->GetStru6()->_4A7E89_sparkles_on_actor_after_it_casts_buff(&pActors[mon_id], 0);
        }
        spell_sound_flag = true;
        break;
      }
      case SPELL_MIND_CHARM:// ����������
      {
        v61 = PID_ID(a2);
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        if ( pActors[v61].DoesDmgTypeDoDamage((DAMAGE_TYPE)7) )
        {
          uint power = 300 * spell_level;
          if ( skill_level == 2 )
            power = 600 * spell_level;
          else if ( skill_level == 3 )
            power  = 29030400;

          pActors[v61].pActorBuffs[ACTOR_BUFF_BERSERK].Reset();
          pActors[v61].pActorBuffs[ACTOR_BUFF_ENSLAVED].Reset();
          pActors[v61].pActorBuffs[ACTOR_BUFF_CHARM].Apply(pParty->uTimePlayed + (signed int)(signed __int64)((double)(signed int)(power << 7) * 0.033333335), skill_level, 0, 0, 0);
          pSpellSprite.stru_24.Reset();
          pSpellSprite.spell_id = pCastSpell->uSpellID;
          pSpellSprite.spell_level = spell_level;
          pSpellSprite.spell_skill = skill_level;
          pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
          pSpellSprite.vPosition.x = pActors[v61].vPosition.x;
          pSpellSprite.vPosition.y = pActors[v61].vPosition.y;
          pSpellSprite.vPosition.z = pActors[v61].vPosition.z + pActors[v61].uActorHeight;
          pSpellSprite.uAttributes = 0;
          pSpellSprite.uSectorID = pIndoor->GetSector(pActors[v61].vPosition.x, pActors[v61].vPosition.y, pSpellSprite.vPosition.z);
          pSpellSprite.uSpriteFrameID = 0;
          pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
          pSpellSprite.spell_target_pid = a2;
          pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
          pSpellSprite.uFacing = LOWORD(v715.uYawAngle);
          LOBYTE(pSpellSprite.uAttributes) |= 0x80u;
          pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
          pSpellSprite.Create(0, 0, 0, pCastSpell->uPlayerID + 1);
        }
        spell_sound_flag = true;
        break;
      }
      case SPELL_DARK_SHRINKING_RAY://��������� ���
      {
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        pSpellSprite.stru_24.Reset();
        pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
        pSpellSprite.vPosition.x = pParty->vPosition.x;
        pSpellSprite.vPosition.y = pParty->vPosition.y;
        pSpellSprite.uAttributes = 0;
        pSpellSprite.vPosition.z = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3;
        pSpellSprite.uSectorID = pIndoor->GetSector(pParty->vPosition.x, pParty->vPosition.y, pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3);
        pSpellSprite.uSpriteFrameID = 0;
        pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
        pSpellSprite.spell_target_pid = a2;
        pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
        pSpellSprite.uFacing = LOWORD(v715.uYawAngle);
        pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
        pSpellSprite.spell_skill = skill_level;
        pSpellSprite.spell_id = SPELL_FIRE_PROTECTION_FROM_FIRE;
        pSpellSprite.spell_level = 300 * spell_level;
        if ( pParty->bTurnBasedModeOn == 1 )
          LOBYTE(pSpellSprite.uAttributes) |= 4;
        v659 = pObjectList->pObjects[(signed __int16)pSpellSprite.uObjectDescID].uSpeed;
        if ( pSpellSprite.Create(v715.uYawAngle, v715.uPitchAngle, v659, pCastSpell->uPlayerID + 1) != -1 && pParty->bTurnBasedModeOn == 1 )
          ++pTurnEngine->pending_actions;
        spell_sound_flag = true;
        break;
      }
      case SPELL_FIRE_FIRE_AURA: //���� ����
      {
        switch (skill_level)
        {
          case 1: duration = 3600 * spell_level; amount = 10; break; //����
          case 2: duration = 3600 * spell_level; amount = 11; break; //��������� ����
          case 3: duration = 3600 * spell_level; amount = 12; break;
          case 4: duration = 0; amount = 12; break;
          default:
            assert(false);
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        v730c = &pParty->pPlayers[pCastSpell->uPlayerID_2].pInventoryItemList[a2];
        _item = &pItemsTable->pItems[v730c->uItemID];
        v730c->UpdateTempBonus(pParty->uTimePlayed);
        if ( v730c->uItemID < 64 || v730c->uItemID > 65 
         && !v730c->IsBroken()
         && !v730c->uSpecEnchantmentType
         && !v730c->uEnchantmentType
         && ( _item->uEquipType == EQUIP_SINGLE_HANDED || _item->uEquipType == EQUIP_TWO_HANDED || _item->uEquipType == EQUIP_BOW)
         && !pItemsTable->IsMaterialNonCommon(v730c) )
        {
          v730c->uSpecEnchantmentType = amount;
          if ( skill_level != 4 )
          {
            v730c->uExpireTime = pParty->uTimePlayed + (signed int)(signed __int64)((double)(signed int)((int)duration << 7) * 0.033333335);
            v730c->uAttributes |= ITEM_TEMP_BONUS;
          }
          v730c->uAttributes |= ITEM_AURA_EFFECT_RED;
          _50C9A8_item_enchantment_timer = 256;
          spell_sound_flag = true;
          break;
        }
        _50C9D0_AfterEnchClickEventId = 113;
        _50C9D4_AfterEnchClickEventSecondParam = 0;
        _50C9D8_AfterEnchClickEventTimeout = 1;
        ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2); // Spell failed
        pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
        pCastSpell->uSpellID = 0;
        continue;
      }
      case SPELL_BODY_REGENERATION://�����������
      {
        switch (skill_level)
        {
          case 1: amount = 1; break;
          case 2: amount = 1; break;
          case 3: amount = 3; break;
          case 4: amount = 10; break;
          default:
            assert(false);
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pCastSpell->uPlayerID_2);
        pParty->pPlayers[pCastSpell->uPlayerID_2].pPlayerBuffs[PLAYER_BUFF_REGENERATION].Apply(pParty->uTimePlayed + (signed int)(signed __int64)((double)(3600 * spell_level) * 4.2666669), skill_level, amount, 0, 0);
        spell_sound_flag = true;
        break;
      }
      case SPELL_FIRE_PROTECTION_FROM_FIRE://������ �� ����
      case SPELL_AIR_PROTECTION_FROM_AIR://������ �� �������
      case SPELL_WATER_PROTECTION_FROM_WATER://������ �� ����
      case SPELL_EARTH_PROTECTION_FROM_EARTH://������ �� �����
      case SPELL_MIND_PROTECTION_FROM_MIND://������ �� �����
      case SPELL_BODY_PROTECTION_FROM_BODY://������ �� ����
      {
        switch (skill_level)
        {
          case 1:
          case 2:
          case 3: 
          case 4: amount = skill_level * spell_level; break;
          default:
            assert(false);
        }
        switch (pCastSpell->uSpellID)
        {
          case SPELL_FIRE_PROTECTION_FROM_FIRE:
            buff_resist = PARTY_BUFF_RESIST_FIRE;
            break;
          case SPELL_AIR_PROTECTION_FROM_AIR:
            buff_resist = PARTY_BUFF_RESIST_AIR;
            break;
          case SPELL_WATER_PROTECTION_FROM_WATER:
            buff_resist = PARTY_BUFF_RESIST_WATER;
            break;
          case SPELL_EARTH_PROTECTION_FROM_EARTH:
            buff_resist = PARTY_BUFF_RESIST_EARTH;
            break;
          case SPELL_MIND_PROTECTION_FROM_MIND:
            buff_resist = PARTY_BUFF_RESIST_MIND;
            break;
          case SPELL_BODY_PROTECTION_FROM_BODY:
            buff_resist = PARTY_BUFF_RESIST_BODY;
            break;
          default:
            assert(false);
          continue;
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 0);
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 1);
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 2);
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 3);
        //v90 = (double)(signed int)(3600 * spell_level) * 4.2666669;
        pParty->pPartyBuffs[buff_resist].Apply(pParty->uTimePlayed + (signed int)(signed __int64)((double)(signed int)(3600 * spell_level) * 4.2666669), skill_level, amount, 0, 0);
        spell_sound_flag = true;
        break;
      }
      case SPELL_FIRE_HASTE://������
      {
        switch (skill_level)
        {
          case 1: duration = 60 * (spell_level + 60); break;
          case 2: duration = 60 * (spell_level + 60); break;
          case 3: duration = 180 * (spell_level + 20); break;
          case 4: duration = 240 * (spell_level + 15); break;
          default:
            assert(false);
        }
        if ( pPlayer->CanCastSpell(uRequiredMana) )
        {
          spell_sound_flag = true;
          for ( uint pl_id = 0; pl_id < 4; pl_id++ )
          {
            if ( pParty->pPlayers[pl_id].pConditions[Condition_Weak] )
              spell_sound_flag = false;
          }
          if ( spell_sound_flag )
          {
            pParty->pPartyBuffs[PARTY_BUFF_HASTE].Apply(pParty->uTimePlayed + (signed int)(signed __int64)((double)(signed int)((int)duration << 7) * 0.033333335), skill_level, 0, 0, 0);
            pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 0);
            pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 1);
            pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 2);
            pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 3);
          }
        }
        break;
      }
      case SPELL_SPIRIT_BLESS://�������������
      {
        switch (skill_level)
        {
          case 1: duration = 300 * (spell_level + 12); break;
          case 2: duration = 300 * (spell_level + 12); break;
          case 3: duration = 900 * (spell_level + 4); break;
          case 4: duration = 3600 * (spell_level + 1); break;
          default:
            assert(false);
        }
        amount = spell_level + 5;
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        if ( skill_level == 1 )
        {
          pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pCastSpell->uPlayerID_2);
          v111 = pOtherOverlayList->_4418B1(10000, pCastSpell->uPlayerID_2 + 310, 0, 65536);
          pParty->pPlayers[pCastSpell->uPlayerID_2].pPlayerBuffs[PLAYER_BUFF_BLESS].Apply(pParty->uTimePlayed +
                                (signed int)(signed __int64)((double)(signed int)((int)duration << 7) * 0.033333335), 1, amount, v111, 0);
          spell_sound_flag = true;
          break;
        }
        for ( uint pl_id = 0; pl_id < 4; pl_id++ )
        {
          pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pl_id);
          v111 = pOtherOverlayList->_4418B1(10000, pl_id + 310, 0, 65536);
          pParty->pPlayers[pl_id].pPlayerBuffs[1].Apply(pParty->uTimePlayed +
                             (signed int)(signed __int64)((double)(signed int)((int)duration << 7) * 0.033333335), skill_level, amount, v111, 0);
        }
        spell_sound_flag = true;
        break;
      }
      case SPELL_SPIRIT_SPIRIT_LASH://����� ����
      {
        if ( pPlayer->CanCastSpell(uRequiredMana) && a2 && PID_TYPE(a2) == OBJECT_Actor)
        {
          int mon_id = PID_ID(a2);
          dist_X = abs(pActors[mon_id].vPosition.x - pParty->vPosition.x);
          dist_Y = abs(pActors[mon_id].vPosition.y - pParty->vPosition.y);
          dist_Z = abs(pActors[mon_id].vPosition.z - pParty->vPosition.z);
          int count = int_get_vector_length(dist_X, dist_Y, dist_Z);
          if ( (double)count <= 307.2 )
          {
            v701.x = 0;
            v701.y = 0;
            v701.z = 0;
            pSpellSprite.stru_24.Reset();
            pSpellSprite.spell_id = pCastSpell->uSpellID;
            pSpellSprite.spell_level = v723;
            pSpellSprite.spell_skill = skill_level;
            pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
            pSpellSprite.uAttributes = 0;
            pSpellSprite.uSectorID = 0;
            pSpellSprite.uSpriteFrameID = 0;
            pSpellSprite.field_60_distance_related_prolly_lod = 0;
            pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
            pSpellSprite.uFacing = 0;
            pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
            pSpellSprite.vPosition.x = pActors[mon_id].vPosition.x;
            pSpellSprite.vPosition.y = pActors[mon_id].vPosition.y;
            pSpellSprite.vPosition.z = pActors[mon_id].vPosition.z - (unsigned int)(signed __int64)((double)pActors[mon_id].uActorHeight * -0.8);
            pSpellSprite.spell_target_pid = PID(OBJECT_Actor, a2);
            Actor::DamageMonsterFromParty(PID(OBJECT_Item, pSpellSprite.Create(0, 0, 0, 0)), mon_id, &v701);
            spell_sound_flag = true;
          }
          else
          {
            ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2);//"�� ����������!!!"
            pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
            pCastSpell->uSpellID = 0;
          }
        }
        break;
      }
      case SPELL_AIR_SHIELD://���
      case SPELL_EARTH_STONESKIN://�������� ����
      case SPELL_SPIRIT_HEROISM://�������
      {
        switch (skill_level)
        {
          case 1: duration = 300 * (spell_level + 12); break;
          case 2: duration = 300 * (spell_level + 12); break;
          case 3: duration = 900 * (spell_level + 4); break;
          case 4: duration = 3600 * (spell_level + 1); break;
          default:
            assert(false);
        }
        switch (pCastSpell->uSpellID)
        {
          case SPELL_AIR_SHIELD:
            amount = 0;
            buff_resist = PARTY_BUFF_SHIELD;
            break;
          case SPELL_EARTH_STONESKIN:
            amount = spell_level + 5;
            buff_resist = PARTY_BUFF_STONE_SKIN;
            break;
          case SPELL_SPIRIT_HEROISM:
            amount = spell_level + 5;
            buff_resist = PARTY_BUFF_HEROISM;
            break;
          default:
            assert(false);
          continue;
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 0);
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 1);
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 2);
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 3);
        pParty->pPartyBuffs[buff_resist].Apply(pParty->uTimePlayed +
                       (signed int)(signed __int64)((double)(signed int)((int)duration << 7) * 0.033333335), skill_level, amount, 0, 0);
        spell_sound_flag = true;
        break;
      }
      case SPELL_FIRE_IMMOLATION://������ ����
      {
        if ( skill_level == 4 )
          duration = 600 * spell_level;
        else
          duration = 60 * spell_level;
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 0);
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 1);
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 2);
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 3);
        pParty->pPartyBuffs[PARTY_BUFF_IMMOLATION].Apply(pParty->uTimePlayed + 
                               (signed __int64)((double)(signed int)((int)duration << 7) * 0.033333335), skill_level, spell_level, 0, 0);
        spell_sound_flag = true;
        break;
      }
      case SPELL_FIRE_METEOR_SHOWER://����� ��������
      {
        //if (skill_level < 3)//��� ������� � ��������
          //break;
        int meteor_num;
        if ( skill_level == 4 )
          meteor_num = 20;
        else
          meteor_num = 16;
        if ( uCurrentlyLoadedLevelType == LEVEL_Indoor)
        {
          ShowStatusBarString(pGlobalTXT_LocalizationStrings[491], 2);  // Can't cast Meteor Shower indoors!
          pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
          pCastSpell->uSpellID = 0;
          continue;
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        obj_type = PID_TYPE(a2);
        mon_id = PID_ID(a2);
        if (obj_type == OBJECT_Actor)//���� ���� ��������� �� ������� ����., �� ����� ��������� ���� ������ �������
        {
          dist_X = pActors[mon_id].vPosition.x;
          dist_Y = pActors[mon_id].vPosition.y;
          dist_Z = pActors[mon_id].vPosition.z;
        }
        else
        {
          dist_X = pParty->vPosition.x + fixpoint_mul(2048, stru_5C6E00->Cos(pParty->sRotationY));
          dist_Y = pParty->vPosition.y + fixpoint_mul(2048, stru_5C6E00->Sin(pParty->sRotationY));
          dist_Z = pParty->vPosition.z;
        }
        unsigned __int64 k = 0;
        int j = 0;
        if ( meteor_num > 0 )
        {
          v730 = obj_type == OBJECT_Actor ? a2 : 0;
          for ( meteor_num; meteor_num; meteor_num-- )
          {
            a2 = rand() % 1000;
            if ( sqrt(((double)a2 - 2500) * ((double)a2 - 2500) + j * j + k * k) <= 1.0 )
            {
              LODWORD(v687) = 0;
              HIDWORD(v687) = 0;
            }
            else
            {
              HIDWORD(v687) = stru_5C6E00->Atan2((signed __int64)sqrt((float)(j * j + k * k)), (double)a2 - 2500);
              LODWORD(v687) = stru_5C6E00->Atan2(j, k);
            }
            pSpellSprite.stru_24.Reset();
            pSpellSprite.spell_id = pCastSpell->uSpellID;
            pSpellSprite.spell_level = spell_level;
            pSpellSprite.spell_skill = skill_level;
            pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
            pSpellSprite.uAttributes = 0;
            pSpellSprite.vPosition.x = dist_X;
            pSpellSprite.vPosition.y = dist_Y;
            pSpellSprite.vPosition.z = a2 + dist_Z;
            pSpellSprite.uSectorID = 0;
            pSpellSprite.uSpriteFrameID = 0;
            pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
            pSpellSprite.spell_target_pid = v730;
            pSpellSprite.field_60_distance_related_prolly_lod = stru_50C198._427546(a2 + 2500);
            pSpellSprite.uFacing = v687;
            pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
            if ( pParty->bTurnBasedModeOn == 1 )
              pSpellSprite.uAttributes = 4;
            if ( pSpellSprite.Create(v687, SHIDWORD(v687), pObjectList->pObjects[(signed __int16)pSpellSprite.uObjectDescID].uSpeed, 0) != -1 && pParty->bTurnBasedModeOn == 1 )
              ++pTurnEngine->pending_actions;
            j = rand() % 1024 - 512;
            k = rand() % 1024 - 512;
          }
        }
        spell_sound_flag = true;
        break;
      }
      case SPELL_FIRE_INFERNO://������ �����
      {
        if ( uCurrentlyLoadedLevelType == LEVEL_Outdoor)
        {
          ShowStatusBarString(pGlobalTXT_LocalizationStrings[492], 2); // Can't cast Inferno outdoors!
          pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
          pCastSpell->uSpellID = 0;
          continue;
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        int mon_num = pRenderer->_46�6��_GetActorsInViewport(4096);
        v700.x = 0;
        v700.y = 0;
        v700.z = 0;
        pSpellSprite.stru_24.Reset();
        pSpellSprite.spell_id = pCastSpell->uSpellID;
        pSpellSprite.spell_level = spell_level;
        pSpellSprite.spell_skill = skill_level;
        pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
        pSpellSprite.uAttributes = 0;
        pSpellSprite.uSectorID = 0;
        pSpellSprite.uSpriteFrameID = 0;
        pSpellSprite.field_60_distance_related_prolly_lod = 0;
        pSpellSprite.uFacing = 0;
        pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
        pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
        for ( uint i = 0; i < mon_num; i++ )
        {
          pSpellSprite.vPosition.x = pActors[_50BF30_actors_in_viewport_ids[i]].vPosition.x;
          pSpellSprite.vPosition.y = pActors[_50BF30_actors_in_viewport_ids[i]].vPosition.y;
          pSpellSprite.vPosition.z = pActors[_50BF30_actors_in_viewport_ids[i]].vPosition.z - (unsigned int)(signed __int64)((double)pActors[_50BF30_actors_in_viewport_ids[i]].uActorHeight * -0.8);
          pSpellSprite.spell_target_pid = PID(OBJECT_Actor, _50BF30_actors_in_viewport_ids[i]);
          Actor::DamageMonsterFromParty(PID(OBJECT_Item, pSpellSprite.Create(0, 0, 0, 0)), _50BF30_actors_in_viewport_ids[i], &v700);
          pGame->GetStru6()->_4A81CA(&pSpellSprite);
          pGame->GetStru6()->FadeScreen__like_Turn_Undead_and_mb_Armageddon(0xFF3C1E, 0x40);
        }
        spell_sound_flag = true;
        break;
      }
      case SPELL_AIR_WIZARD_EYE://��� �������
      {
        duration = 3600 * spell_level;
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        for ( uint pl_id = 0; pl_id < 4; pl_id++ )
          v169 = pOtherOverlayList->_4418B1(2000, pl_id + 100, 0, 65536);
        pParty->pPartyBuffs[PARTY_BUFF_WIZARD_EYE].Apply(pParty->uTimePlayed + (signed __int64)((double)(signed int)((int)duration << 7) * 0.033333335), skill_level, 0, v169, 0);
        spell_sound_flag = true;
        break;
      }
      case SPELL_AIR_FEATHER_FALL://������� ����(�������)
      {
        switch (skill_level)
        {
          case 1: duration = 300 * spell_level; break;
          case 2: duration = 600 * spell_level; break;
          case 3: duration = 3600 * spell_level; break;
          case 4: duration = 3600 * spell_level; break;
          default:
            assert(false);
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        for ( uint pl_id = 0; pl_id < 4; pl_id++ )
          pOtherOverlayList->_4418B1(2010, pl_id + 100, 0, 65536);
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 0);
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 1);
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 2);
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 3);

        pParty->pPartyBuffs[PARTY_BUFF_FEATHER_FALL].Apply(pParty->uTimePlayed + 
          (signed int)(signed __int64)((double)(signed int)((int)duration << 7) * 0.033333335), skill_level, 0, 0, 0);
        spell_sound_flag = true;
        break;
      }
      case SPELL_AIR_SPARKS://�����
      {
        switch (skill_level)
        {
          case 1: amount = 3; break;
          case 2: amount = 5; break;
          case 3: amount = 7; break;
          case 4: amount = 9; break;
          default:
            assert(false);
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        int _v726 = (signed int)(60 * stru_5C6E00->uIntegerDoublePi) / 360;
        pSpellSprite.stru_24.Reset();
        pSpellSprite.spell_id = pCastSpell->uSpellID;
        pSpellSprite.spell_level = spell_level;
        pSpellSprite.spell_skill = skill_level;
        pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
        pSpellSprite.vPosition.y = pParty->vPosition.y;
        pSpellSprite.vPosition.x = pParty->vPosition.x;
        pSpellSprite.uAttributes = 0;
        pSpellSprite.vPosition.z = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3;
        pSpellSprite.uSectorID = pIndoor->GetSector(pParty->vPosition.x, pParty->vPosition.y,
                                                     pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3);
        pSpellSprite.uSpriteFrameID = 0;
        pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
        pSpellSprite.spell_target_pid = a2;
        pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
        pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
        if ( pParty->bTurnBasedModeOn == 1 )
          LOBYTE(pSpellSprite.uAttributes) |= 4;
        v188 = (signed int)_v726 / -2;
        v189 = (signed int)_v726 / 2;
        while ( v188 <= v189 )
        {
          pSpellSprite.uFacing = v188 + LOWORD(v715.uYawAngle);
          if ( pSpellSprite.Create((signed __int16)(v188 + LOWORD(v715.uYawAngle)), v715.uPitchAngle,
               pObjectList->pObjects[(signed __int16)pSpellSprite.uObjectDescID].uSpeed, pCastSpell->uPlayerID + 1) != -1
             && pParty->bTurnBasedModeOn == 1 )
            ++pTurnEngine->pending_actions;
          v188 += _v726 / (amount - 1);
        }
        spell_sound_flag = true;
        break;
      }
      case SPELL_AIR_JUMP://������
      {
        if ( pParty->uFlags & PARTY_FLAGS_1_FALLING)
        {
          ShowStatusBarString(pGlobalTXT_LocalizationStrings[493], 2);  // Can't cast Jump while airborne!
          pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
          break;
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        for ( uint pl_id = 0; pl_id < 4; pl_id++ )
          pOtherOverlayList->_4418B1(2040, pl_id + 100, 0, 65536);
        pParty->uFlags |= 0x100;
        pParty->uFallSpeed = 1000;
        spell_sound_flag = true;
        break;
      }
      case SPELL_AIR_INVISIBILITY://�����������
      {
        switch (skill_level)
        {
          case 1: duration = 600 * spell_level; amount = spell_level; break;
          case 2: duration = 600 * spell_level; amount = 2 * spell_level; break;
          case 3: duration = 600 * spell_level; amount = 3 * spell_level; break;
          case 4: duration = 3600 * spell_level; amount = 4 * spell_level; break;
          default:
            assert(false);
        }
        if (pParty->uFlags & (PARTY_FLAGS_1_ALERT_RED | PARTY_FLAGS_1_ALERT_YELLOW))
        {
          ShowStatusBarString(pGlobalTXT_LocalizationStrings[638], 2);  // There are hostile creatures nearby!
          pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
          pCastSpell->uSpellID = 0;
          continue;
        }
        if ( pPlayer->CanCastSpell(uRequiredMana) )
        {
          pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 0);
          pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 1);
          pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 2);
          pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 3);
          pParty->pPartyBuffs[PARTY_BUFF_INVISIBILITY].Apply(pParty->uTimePlayed +
                 (signed int)(signed __int64)((double)(signed int)((int)duration << 7) * 0.033333335), skill_level, amount, 0, 0);
          spell_sound_flag = true;
        }
        break;
      }
      case SPELL_AIR_FLY://����
      {
        if ( uCurrentlyLoadedLevelType == LEVEL_Indoor)
        {
          ShowStatusBarString(pGlobalTXT_LocalizationStrings[494], 2);  // Can not cast Fly indoors!
          pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
          break;
        }
        if ( !pPlayers[pCastSpell->uPlayerID + 1]->GetMaxMana() )
        {
          ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2);  // Spell failed
          pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
          break;
        }
        if ( skill_level == 1 || skill_level == 2 || skill_level == 3 )
          amount = 1;
        else
          amount = 0;
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        for ( uint pl_id = 0; pl_id < 4; pl_id++)
          pOtherOverlayList->_4418B1(2090, pl_id + 100, 0, 65536);
        v206 = pOtherOverlayList->_4418B1(10008, 203, 0, 65536);
        pParty->pPartyBuffs[PARTY_BUFF_FLY].Apply(pParty->uTimePlayed + (signed __int64)((double)(signed int)((int)(3600 * spell_level) << 7)
                   * 0.033333335), skill_level, amount, v206, pCastSpell->uPlayerID + 1);
        spell_sound_flag = true;
        break;
      }
      case SPELL_AIR_STARBURST://���������
      {
        if ( uCurrentlyLoadedLevelType == LEVEL_Indoor)
        {
          ShowStatusBarString(pGlobalTXT_LocalizationStrings[495], 2);  // Can't cast Starburst indoors!
          pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
          pCastSpell->uSpellID = 0;
          continue;
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        obj_type = PID_TYPE(a2);
        mon_id = PID_ID(a2);
        if ( obj_type == OBJECT_Actor )
        {
          dist_X = pActors[mon_id].vPosition.x;
          dist_Y = pActors[mon_id].vPosition.y;
          dist_Z = pActors[mon_id].vPosition.z;
        }
        else
        {
          dist_X = pParty->vPosition.x + fixpoint_mul(2048, stru_5C6E00->Cos(pParty->sRotationY));
          dist_Y = pParty->vPosition.y + fixpoint_mul(2048, stru_5C6E00->Sin(pParty->sRotationY));
          dist_Z = pParty->vPosition.z;
        }
        unsigned __int64 k = 0;
        int j = 0;
        v730 = obj_type == OBJECT_Actor ? a2 : 0;
        for ( uint star_num = 20; star_num; star_num-- )
        {
          a2 = rand() % 1000;
          if ( sqrt(((double)a2 + (double)dist_Z - (double)(dist_Z + 2500)) * ((double)a2 + (double)dist_Z - (double)(dist_Z + 2500))
                     + j * j + k * k) <= 1.0 )
          {
            LODWORD(v685) = 0;
            HIDWORD(v685) = 0;
          }
          else
          {
            HIDWORD(v685) = stru_5C6E00->Atan2((signed __int64)sqrt((float)(j * j + k * k)), ((double)a2 + (double)dist_Z - (double)(dist_Z + 2500)));
            LODWORD(v685) = stru_5C6E00->Atan2(j, k);
          }
          pSpellSprite.stru_24.Reset();
          pSpellSprite.spell_id = pCastSpell->uSpellID;
          pSpellSprite.spell_level = spell_level;
          pSpellSprite.spell_skill = skill_level;
          pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
          pSpellSprite.uAttributes = 0;
          pSpellSprite.vPosition.x = dist_X;
          pSpellSprite.vPosition.y = dist_Y;
          pSpellSprite.vPosition.z = (int)(a2 + (dist_Z + 2500));
          pSpellSprite.uSectorID = 0;
          pSpellSprite.uSpriteFrameID = 0;
          pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
          pSpellSprite.spell_target_pid = v730;
          pSpellSprite.field_60_distance_related_prolly_lod = stru_50C198._427546(a2 + 2500);
          pSpellSprite.uFacing = v685;
          pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
          if ( pParty->bTurnBasedModeOn == 1 )
            pSpellSprite.uAttributes = 4;
          if ( pSpellSprite.Create(v685, SHIDWORD(v685), pObjectList->pObjects[(signed __int16)pSpellSprite.uObjectDescID].uSpeed, 0) != -1
               && pParty->bTurnBasedModeOn == 1 )
            ++pTurnEngine->pending_actions;
          j = rand() % 1024 - 512;
          k = rand() % 1024 - 512;
        }
        spell_sound_flag = true;
        break;
      }
      case SPELL_WATER_AWAKEN://�����������
      {
        switch (skill_level)
        {
          case 1: amount = 180 * spell_level; break;
          case 2: amount = 3600 * spell_level; break;
          case 3: amount = 86400 * spell_level; break;
          case 4: amount = 0; break;
          default:
            assert(false);
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        for( int i=0; i < 4; i++ )
        {
          if ( skill_level == 4 )
          {
            if ( pParty->pPlayers[i].pConditions[Condition_Sleep] )
            {
              pParty->pPlayers[i].pConditions[Condition_Sleep] = 0;
              pParty->pPlayers[i].PlaySound(SPEECH_103, 0);
            }
          }
          else
          {
            if ( pParty->pPlayers[i].DiscardConditionIfLastsLongerThan(Condition_Sleep, pParty->uTimePlayed -
                             (signed int)(signed __int64)((double)(amount << 7) * 0.033333335)) )
              pParty->pPlayers[i].PlaySound(SPEECH_103, 0);
          }
        }
        spell_sound_flag = true;
        break;
      }
      case SPELL_WATER_POISON_SPRAY://���������� ���
      {
        switch (skill_level)
        {
          case 1: amount = 1; break;
          case 2: amount = 3; break;
          case 3: amount = 5; break;
          case 4: amount = 7; break;
          default:
            assert(false);
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        signed int _v733 = (signed int)(60 * stru_5C6E00->uIntegerDoublePi) / 360;
        if ( amount == 1 )
        {
          pSpellSprite.stru_24.Reset();
          pSpellSprite.spell_id = pCastSpell->uSpellID;
          pSpellSprite.spell_skill = skill_level;
          pSpellSprite.spell_level = spell_level;
          pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
          pSpellSprite.uAttributes = 0;
          pSpellSprite.vPosition.x = pParty->vPosition.x;
          pSpellSprite.vPosition.y = pParty->vPosition.y;
          pSpellSprite.vPosition.z = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3;
          pSpellSprite.uSectorID = pIndoor->GetSector(pParty->vPosition.x, pParty->vPosition.y, pSpellSprite.vPosition.z);
          pSpellSprite.uSpriteFrameID = 0;
          pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
          pSpellSprite.spell_target_pid = a2;
          pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
          pSpellSprite.uFacing = LOWORD(v715.uYawAngle);
          pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
          if ( pParty->bTurnBasedModeOn == 1 )
            LOBYTE(pSpellSprite.uAttributes) |= 4;
          v659 = pObjectList->pObjects[(signed __int16)pSpellSprite.uObjectDescID].uSpeed;
          if ( pSpellSprite.Create(v715.uYawAngle, v715.uPitchAngle, v659, pCastSpell->uPlayerID + 1) != -1 && pParty->bTurnBasedModeOn == 1 )
            ++pTurnEngine->pending_actions;
        }
        else
        {
          pSpellSprite.stru_24.Reset();
          pSpellSprite.spell_id = pCastSpell->uSpellID;
          pSpellSprite.spell_level = spell_level;
          pSpellSprite.spell_skill = skill_level;
          pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
          pSpellSprite.vPosition.y = pParty->vPosition.y;
          pSpellSprite.vPosition.x = pParty->vPosition.x;
          pSpellSprite.uAttributes = 0;
          pSpellSprite.vPosition.z = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3;
          pSpellSprite.uSectorID = pIndoor->GetSector(pParty->vPosition.x, pParty->vPosition.y, pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3);
          pSpellSprite.uSpriteFrameID = 0;
          pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
          pSpellSprite.spell_target_pid = a2;
          pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
          pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
          if ( pParty->bTurnBasedModeOn == 1 )
            LOBYTE(pSpellSprite.uAttributes) |= 4;
          v188 = _v733 / -2;
          v189 = _v733 / 2;
          if ( v188 <= v189 )
          {
            do
            {
              pSpellSprite.uFacing = v188 + v715.uYawAngle;
              if ( pSpellSprite.Create(pSpellSprite.uFacing, v715.uPitchAngle, pObjectList->pObjects[(signed __int16)pSpellSprite.uObjectDescID].uSpeed,
                         pCastSpell->uPlayerID + 1) != -1 && pParty->bTurnBasedModeOn == 1 )
                ++pTurnEngine->pending_actions;
              v188 += _v733 / (amount - 1);
            }
            while ( v188 <= v189 );
          }
        }
        spell_sound_flag = true;
        break;
      }
      case SPELL_WATER_WATER_WALK://�������� �� ����
      {
        if ( !pPlayers[pCastSpell->uPlayerID + 1]->GetMaxMana() )
        {
          ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2);  // Spell failed
          pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
          break;
        }
        switch (skill_level)
        {
          case 1: //break;
          case 2: duration = 600 * spell_level; break;
          case 3:
          case 4: duration = 3600 * spell_level; break;
          default:
            assert(false);
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        v169 = pOtherOverlayList->_4418B1(10005, 201, 0, 65536);
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 0);
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 1);
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 2);
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 3);
        pParty->pPartyBuffs[PARTY_BUFF_WATER_WALK].Apply(pParty->uTimePlayed + (signed int)(signed __int64)((double)(duration << 7) * 0.033333335),
                                       skill_level, amount, v169, pCastSpell->uPlayerID + 1);
        if ( skill_level == 4 )
          pParty->pPartyBuffs[PARTY_BUFF_WATER_WALK].uFlags = 1;
        spell_sound_flag = true;
        break;
      }
      case SPELL_WATER_RECHARGE_ITEM://�����������
      {
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        v730c = &pParty->pPlayers[pCastSpell->uPlayerID_2].pInventoryItemList[a2];
        if ( v730c->GetItemEquipType() != 12 || v730c->uAttributes & 2 )
        {
          _50C9D0_AfterEnchClickEventId = 113;
          _50C9D4_AfterEnchClickEventSecondParam = 0;
          _50C9D8_AfterEnchClickEventTimeout = 1;
          ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2);  // Spell failed
          pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
          pCastSpell->uSpellID = 0;
          continue;
        }
        if ( skill_level == 1 || skill_level == 2 )
          v241 = (double)v723 * 0.0099999998 + 0.5;//50 %
        else if ( skill_level == 3 )
          v241 = (double)v723 * 0.0099999998 + 0.69999999;//30 %
        else if ( skill_level == 4 )
          v241 = (double)v723 * 0.0099999998 + 0.80000001;//20 %
        else
          v241 = 0.0;
        if ( v241 > 1.0 )
          v241 = 1.0;
        int uNewCharges = v730c->uMaxCharges * v241;
        v730c->uMaxCharges = uNewCharges;
        v730c->uNumCharges = uNewCharges;
        if ( uNewCharges <= 0 )
        {
          v730c = 0;
          _50C9D0_AfterEnchClickEventId = 113;
          _50C9D4_AfterEnchClickEventSecondParam = 0;
          _50C9D8_AfterEnchClickEventTimeout = 1;
          ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2);//�� ����������!!!
          pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
          pCastSpell->uSpellID = 0;
          spell_level = v723;
          continue;
        }
        v730c->uAttributes |= 0x40u;
        _50C9A8_item_enchantment_timer = 256;
        spell_sound_flag = true;
        break;
      }
      case SPELL_WATER_ENCHANT_ITEM://��������
      {
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        uRequiredMana = 0;
        amount = 10 * spell_level;
        bool item_not_broken = true;
        int rnd = rand() % 100;
        pPlayer = &pParty->pPlayers[pCastSpell->uPlayerID_2];
        v245 = &pPlayer->pInventoryItemList[a2];
        ItemDesc *_v725 = &pItemsTable->pItems[v245->uItemID];
        if ((skill_level == 1 || skill_level == 2 /*&& _v725->uEquipType > EQUIP_BOW*/ || skill_level == 3 || skill_level == 4) && 
            v245->uItemID <= 134 &&
            v245->uSpecEnchantmentType == 0 && v245->uEnchantmentType == 0 &&
            v245->m_enchantmentStrength== 0 && !v245->IsBroken() ) //���������� � ��������
        {
          if ( (v245->GetValue() < 450 && ( skill_level == 1 || skill_level == 2 ))
          || (v245->GetValue() < 450 && (skill_level == 3|| skill_level == 4 ) && _v725->uEquipType >= EQUIP_SINGLE_HANDED && _v725->uEquipType <= EQUIP_BOW)
          || (v245->GetValue() < 250 && (skill_level == 3 || skill_level == 4 )&& _v725->uEquipType > EQUIP_BOW) )//������� �������
          {
            if ( !(v245->uAttributes & 0x200) )// ������� �� ������
            {
              v245->uAttributes |= 2;//������ ������
            }
            item_not_broken = false;
          }
          else
          {
            if ( rnd >= 10 * spell_level )//���� �� ������������
            {
              if ( !(v245->uAttributes & 0x200) )// ������� �� ������
                v245->uAttributes |= 2;//������ ������
            }
            else
            {
              if ( (rnd < 80 && (skill_level == 3 || skill_level == 4 )) || v245->GetValue() < 450 || (v245->GetValue() < 250
                && (skill_level == 3 || skill_level == 4) && _v725->uEquipType >= EQUIP_SINGLE_HANDED && _v725->uEquipType <= EQUIP_BOW))
              {
                v313 = _v725->uEquipType;
                if ( _v725->uEquipType >= EQUIP_ARMOUR && _v725->uEquipType <= EQUIP_AMULET )
                {
                  v295 = rand() % 10;// pItemsTable->field_116D8[pItemsTable->pItems[_this->uItemID].uEquipType];
                  /*v245->uEnchantmentType = 0;
                  __debugbreak(); // castspellinfo.cpp(1971): warning C4700: uninitialized local variable 'v294' used
                  for ( kk = pItemsTable->pEnchantments[0].to_item[v245->GetItemEquipType() + 1];
                        ;
                        kk += pItemsTable->pEnchantments[v294->uEnchantmentType].to_item[v245->GetItemEquipType() + 1] )
                  {
                    ++v245->uEnchantmentType;
                    if ( kk >= v295 )
                      break;
                  }
                  v255 = 10;//pItemsTable->field_116D8[17];
                  v256 = 10;//pItemsTable->field_116D8[16];
                  v245->m_enchantmentStrength = v256 + rand() % (v255 - v256 + 1);*/
                  v245->uEnchantmentType = v295;
                  v245->m_enchantmentStrength = pItemsTable->pEnchantments[v295].to_item[_v725->uEquipType - EQUIP_ARMOUR];
                  v245->uAttributes |= 0x20u;
                  _50C9A8_item_enchantment_timer = 256;
                  spell_sound_flag = true;
                  break;
                }
                else if ( skill_level == 3 || skill_level == 4)//for master & GM not refactored(��� ������� � ������ �� �������������)
                {
                  v258 = 0;
                  v725 = 0;
                  int _v733;
                  if ( pItemsTable->pSpecialEnchantments_count > 0 )
                  {
                    v730 = (int)&v679;
                    for ( _v733 = 0; _v733 < pItemsTable->pSpecialEnchantments_count; ++_v733 )
                    {
                      v259 = LOBYTE(pItemsTable->pSpecialEnchantments[v258 + 1].pBonusStatement);
                      if ( !v259 || v259 == 1 )
                      {
                        v260 = *(&pItemsTable->pSpecialEnchantments[0].to_item_apply[v245->GetItemEquipType() + 4] + v258 * 28);
                        v725 += v260;
                        if ( v260 )
                        {
                          v261 = v730;
                          v730 += 4;
                          *(int *)v261 = _v733;
                        }
                      }
                      ++v258;
                    }
                  }
                  v262 = rand() % v725;
                  v263 = v679;
                  v245->uSpecEnchantmentType = v679[0];
                  v264 = pItemsTable->pSpecialEnchantments[*v263].to_item_apply[v245->GetItemEquipType() + 4];
                  v265 = v262 + 1;
                  if ( v264 < v265 )
                  {
                    for ( ii = v679; ; ii = (int *)v732 )
                    {
                      v267 = (int)(ii + 1);
                      v732 = v267;
                      v268 = *(int *)v267;
                      *(int *)(v245 + 12) = v268;
                      v264 += pItemsTable->pSpecialEnchantments[v268].to_item_apply[v245->GetItemEquipType() + 4];
                      if ( v264 >= v265 )
                        break;
                    }
                  }
                  v278 = 0;
                  v725 = 0;
                  if ( pItemsTable->pSpecialEnchantments_count > 0 )
                  {
                    int *_v730 = v679;
                    for ( _v733 = 0; _v733 < pItemsTable->pSpecialEnchantments_count; ++_v733 )
                    {
                      v279 = LOBYTE(pItemsTable->pSpecialEnchantments[v278].pBonusStatement);
                      if ( !v279 || v279 == 1 )
                      {
                        v280 = *(&pItemsTable->pSpecialEnchantments[v278].to_item_apply[v245->GetItemEquipType()]);
                        _v733 += v280;
                        if ( v280 )
                        {
                          v281 = _v730;
                          ++_v730;
                          *v281 = _v733;
                        }
                      }
                      ++v278;
                    }
                  }
                  v282 = rand() % _v733;
                  v283 = v679;
                  v245->uSpecEnchantmentType = v679[0];
                  v284 = pItemsTable->pSpecialEnchantments[*v283].to_item_apply[v245->GetItemEquipType() ];
                  v285 = v282 + 1;
                  for ( l = v679; v284 < v285; ++l )
                  {
                    v245->uSpecEnchantmentType = *(l+1);
                    v284 += pItemsTable->pSpecialEnchantments[*(l+1)].to_item_apply[v245->GetItemEquipType()];
                  }
                  ++v245->uSpecEnchantmentType;
                  v245->uAttributes |= 0x20u;
                  _50C9A8_item_enchantment_timer = 256;
                  spell_sound_flag = true;
                  break;
                }
              }
            }
          }
        }
        if ( spell_sound_flag == 0 )
        {
          v317 = pGlobalTXT_LocalizationStrings[428];//�� ����������!!!
          if ( item_not_broken == false )
            v317 = pGlobalTXT_LocalizationStrings[585];//������� ������������ �������� ��������
          ShowStatusBarString(v317, 2);
          pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
          //v318 =  &pParty->pPlayers[pCastSpell->uPlayerID_2];
          pCastSpell->uSpellID = 0;
          pParty->pPlayers[pCastSpell->uPlayerID_2].PlaySound(SPEECH_43, 0);
        }
        break;
      }
      case SPELL_WATER_TOWN_PORTAL://������ ������
      {
        amount = 10 * spell_level;
        if ( pPlayer->sMana < (signed int)uRequiredMana )
          break;
        if ( pParty->uFlags & (PARTY_FLAGS_1_ALERT_RED | PARTY_FLAGS_1_ALERT_YELLOW) && skill_level != 4 
          || rand() % 100 >= amount && skill_level != 4 )
        {
          ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2); // Spell failed
          pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
          pCastSpell->uSpellID = 0;
          continue;
        }
        town_portal_caster_id = pCastSpell->uPlayerID;
        pMessageQueue_50CBD0->AddGUIMessage(UIMSG_OnCastTownPortal, 0, 0);
        spell_sound_flag = true;
        break;
      }
      case SPELL_WATER_LLOYDS_BEACON://���� ������
      {
        if ( !_stricmp(pCurrentMapName, "d05.blv") ) // Arena
        {
          ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2); // Spell failed
          pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
          pCastSpell->uSpellID = 0;
          continue;
        }
        if ( pPlayer->sMana >= (signed int)uRequiredMana )
        {
          pEventTimer->Pause();
          pMessageQueue_50CBD0->AddGUIMessage(UIMSG_OnCastLloydsBeacon, 0, 0);
          lloyds_beacon_spell_level = (signed int)(604800 * spell_level);
          _506348_current_lloyd_playerid = pCastSpell->uPlayerID;
          ::uRequiredMana = uRequiredMana;
          ::sRecoveryTime = sRecoveryTime;
          lloyds_beacon_sound_id = pCastSpell->sound_id;
          lloyds_beacon_spell_id = pCastSpell->uSpellID;
          pCastSpell->uFlags |= 0x20u;
        }
        break;
      }
      case SPELL_EARTH_STONE_TO_FLESH:
      {
        switch (skill_level)
        {
          case 1: amount = 3600 * spell_level; break;
          case 2: amount = 3600 * spell_level; break;
          case 3: amount = 86400 * spell_level; break;
          case 4: break;
          default:
          assert(false);
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        if ( pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Pertified] )
        {
          if ( skill_level == 4 )//for GM
          {
            pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Pertified] = 0;
            spell_sound_flag = true;
            break;
          }
          v663 = (signed __int64)((double)(signed __int64)pParty->uTimePlayed - (double)(amount << 7) * 0.033333335);
          pParty->pPlayers[pCastSpell->uPlayerID_2].DiscardConditionIfLastsLongerThan(Condition_Pertified, v663);
        }
        spell_sound_flag = true;
        break;
      }
      case SPELL_EARTH_ROCK_BLAST://����� �����
      {
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        pSpellSprite.stru_24.Reset();
        pSpellSprite.spell_id = pCastSpell->uSpellID;
        pSpellSprite.spell_level = spell_level;
        pSpellSprite.spell_skill = skill_level;
        pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
        pSpellSprite.vPosition.y = pParty->vPosition.y;
        pSpellSprite.vPosition.x = pParty->vPosition.x;
        pSpellSprite.uAttributes = 0;
        pSpellSprite.vPosition.z = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3;
        pSpellSprite.uSectorID = pIndoor->GetSector(pParty->vPosition.x, pParty->vPosition.y, pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3);
        pSpellSprite.uSpriteFrameID = 0;
        pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
        pSpellSprite.spell_target_pid = a2;
        pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
        pSpellSprite.uFacing = LOWORD(pParty->sRotationY);
        pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
        if ( pParty->bTurnBasedModeOn == 1 )
          LOBYTE(pSpellSprite.uAttributes) |= 4;
        v659 = pObjectList->pObjects[(signed __int16)pSpellSprite.uObjectDescID].uSpeed;
        if ( pSpellSprite.Create(pParty->sRotationY, pParty->sRotationX, v659, pCastSpell->uPlayerID + 1) != -1
            && pParty->bTurnBasedModeOn == 1 )
          ++pTurnEngine->pending_actions;
        spell_sound_flag = true;
        break;
      }
      case SPELL_EARTH_DEATH_BLOSSOM: //������ ������
      {
        if ( uCurrentlyLoadedLevelType == LEVEL_Indoor)
        {
          ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2); // Spell failed
          pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
          pCastSpell->uSpellID = 0;
          continue;
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        pSpellSprite.uType = 4090;
        pSpellSprite.stru_24.Reset();
        pSpellSprite.spell_id = pCastSpell->uSpellID;
        pSpellSprite.spell_level = spell_level;
        pSpellSprite.spell_skill = skill_level;
        pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
        pSpellSprite.vPosition.x = pParty->vPosition.x;
        pSpellSprite.vPosition.y = pParty->vPosition.y;
        pSpellSprite.uAttributes = 0;
        pSpellSprite.uSectorID = 0;
        pSpellSprite.uSpriteFrameID = 0;
        pSpellSprite.vPosition.z = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3;
        pSpellSprite.field_60_distance_related_prolly_lod = 0;
        pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
        pSpellSprite.spell_target_pid = a2;
        pSpellSprite.uFacing = LOWORD(pParty->sRotationY);
        pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
        if ( pParty->bTurnBasedModeOn == 1 )
          pSpellSprite.uAttributes = 4;
        v659 = pObjectList->pObjects[(signed __int16)pSpellSprite.uObjectDescID].uSpeed;
        if ( pSpellSprite.Create(pParty->sRotationY, stru_5C6E00->uIntegerHalfPi / 2, v659, 0) != -1
            && pParty->bTurnBasedModeOn == 1 )
          ++pTurnEngine->pending_actions;
        spell_sound_flag = true;
        break;
      }
      case SPELL_SPIRIT_DETECT_LIFE: // �������� �����
      {
        switch (skill_level)
        {
          case 1: amount = 600 * spell_level; break;
          case 2: amount = 1800 * spell_level; break;
          case 3:
          case 4: amount = 3600 * spell_level; break;
          default:
          assert(false);
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 0);
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 1);
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 2);
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 3);
        pParty->pPartyBuffs[PARTY_BUFF_DETECT_LIFE].Apply(pParty->uTimePlayed + 
            (signed int)(signed __int64)((double)(signed int)((int)amount << 7) * 0.033333335), skill_level, 0, 0, 0);
        spell_sound_flag = true;
        break;
      }
      case SPELL_SPIRIT_FATE:// ������
      {
        switch (skill_level)
        {
          case 1: amount = spell_level; break;
          case 2: amount = 2 * spell_level; break;
          case 3: amount = 4 * spell_level; break;
          case 4: amount = 6 * spell_level; break;
          default:
            assert(false);
        }
        //LODWORD(v733) = 300;
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        if ( pCastSpell->spell_target_pid == 0 )
        {
          pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pCastSpell->uPlayerID_2);
          pParty->pPlayers[pCastSpell->uPlayerID_2].pPlayerBuffs[PLAYER_BUFF_FATE].Apply(pParty->uTimePlayed + 1280, skill_level, amount, 0, 0);
          spell_sound_flag = true;
          break;
        }
        if (PID_TYPE(pCastSpell->spell_target_pid) == OBJECT_Actor)
        {
          mon_id = PID_ID(pCastSpell->spell_target_pid);
          pActors[mon_id].pActorBuffs[ACTOR_BUFF_FATE].Apply(pParty->uTimePlayed + 1280, skill_level, amount, 0, 0);
          pActors[mon_id].uAttributes |= 0x80000;
          pGame->GetStru6()->_4A7E89_sparkles_on_actor_after_it_casts_buff(&pActors[mon_id], 0);
        }
        spell_sound_flag = true;
        break;
      }
      case SPELL_SPIRIT_REMOVE_CURSE:// ������ �����
      {
        switch (skill_level)
        {
          case 1: amount = 3600 * spell_level; break;
          case 2: amount = 3600 * spell_level; break;
          case 3: amount = 86400 * spell_level; break;
          case 4: amount = 0; break;
          default:
            assert(false);
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        if ( !pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Cursed] )
        {
          spell_sound_flag = true;
          break;
        }
        if ( skill_level == 4 )//GM
          pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Cursed] = 0;
        else
        {
          pParty->pPlayers[pCastSpell->uPlayerID_2].DiscardConditionIfLastsLongerThan(0,
                (signed __int64)((double)(signed __int64)pParty->uTimePlayed - (double)(amount << 7) * 0.033333335));
          if ( pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Cursed] )
          {
            spell_sound_flag = true;
            break;
          }
        }
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pCastSpell->uPlayerID_2);
        spell_sound_flag = true;
        break;
      }
      case SPELL_SPIRIT_PRESERVATION://����������
      {
        if ( skill_level == 4 )
          duration = 900 * (spell_level + 4);
        else
          duration = 300 * (spell_level + 12);
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        if ( skill_level == 1 || skill_level == 2 )
        {
          pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pCastSpell->uPlayerID_2);
          pParty->pPlayers[pCastSpell->uPlayerID_2].pPlayerBuffs[PLAYER_BUFF_PRESERVATION].Apply(pParty->uTimePlayed +
            (signed int)(signed __int64)((double)(signed int)((int)duration << 7) * 0.033333335), skill_level, 0, 0, 0);
          spell_sound_flag = true;
          break;
        }
        for ( uint pl_id = 0; pl_id < 4; pl_id++ )
        {
          pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pl_id);
          pParty->pPlayers[pl_id].pPlayerBuffs[PLAYER_BUFF_PRESERVATION].Apply(pParty->uTimePlayed + 
            (signed int)(signed __int64)((double)(signed int)((int)duration << 7) * 0.033333335), skill_level, 0, 0, 0);
        }
        spell_sound_flag = true;
        break;
      }
      case SPELL_SPIRIT_TURN_UNDEAD://��� ���������
      {
        if ( skill_level == 1 || skill_level == 2)
          duration = 60 * (spell_level + 3);
        else
          duration = 300 * spell_level + 180;
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        int mon_num = pRenderer->_46�6��_GetActorsInViewport(4096);
        pGame->GetStru6()->FadeScreen__like_Turn_Undead_and_mb_Armageddon(0xFFFFFF, 192);
        ++pSpellSprite.uType;
        pSpellSprite.stru_24.Reset();
        pSpellSprite.spell_id = pCastSpell->uSpellID;
        pSpellSprite.spell_level = spell_level;
        pSpellSprite.spell_skill = skill_level;
        pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
        pSpellSprite.uAttributes = 0;
        pSpellSprite.uSectorID = 0;
        pSpellSprite.uSpriteFrameID = 0;
        pSpellSprite.field_60_distance_related_prolly_lod = 0;
        pSpellSprite.uFacing = 0;
        pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
        pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
        for ( a2 = 0; a2 < mon_num; ++a2 )
        {
          if ( MonsterStats::BelongsToSupertype(pActors[_50BF30_actors_in_viewport_ids[a2]].pMonsterInfo.uID, MONSTER_SUPERTYPE_UNDEAD) )
          {
            pSpellSprite.vPosition.x = pActors[_50BF30_actors_in_viewport_ids[a2]].vPosition.x;
            pSpellSprite.vPosition.y = pActors[_50BF30_actors_in_viewport_ids[a2]].vPosition.y;
            pSpellSprite.vPosition.z = pActors[_50BF30_actors_in_viewport_ids[a2]].vPosition.z - (unsigned int)(signed __int64)((double)pActors[_50BF30_actors_in_viewport_ids[a2]].uActorHeight * -0.8);
            pSpellSprite.spell_target_pid = PID(OBJECT_Actor, _50BF30_actors_in_viewport_ids[a2]);
            pSpellSprite.Create(0, 0, 0, 0);
            pActors[_50BF30_actors_in_viewport_ids[a2]].pActorBuffs[ACTOR_BUFF_AFRAID].Apply(pParty->uTimePlayed + 
                      (signed __int64)((double)(signed int)((int)duration << 7) * 0.033333335), skill_level, 0, 0, 0);
          }
        }
        spell_sound_flag = true;
        break;
      }
      case SPELL_SPIRIT_RAISE_DEAD://���������
      {
        if ( skill_level == 4 )
          amount = 0;
        else
          amount = 86400 * spell_level;
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        pOtherOverlayList->_4418B1(5080, pCastSpell->uPlayerID_2 + 100, 0, 65536);
        if ( !pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Dead] )
        {
          spell_sound_flag = true;
          break;
        }
        pParty->pPlayers[pCastSpell->uPlayerID_2].sHealth = 1;
        if ( skill_level == 4 )
        {
          pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Dead] = 0;
          pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Unconcious] = 0;
        }
        else
        {
          pParty->pPlayers[pCastSpell->uPlayerID_2].DiscardConditionIfLastsLongerThan(Condition_Dead,
            (signed __int64)((double)(signed __int64)pParty->uTimePlayed - (double)(amount << 7) * 0.033333335));
          pParty->pPlayers[pCastSpell->uPlayerID_2].DiscardConditionIfLastsLongerThan(Condition_Unconcious,
            (signed __int64)((double)(signed __int64)pParty->uTimePlayed - (double)(amount << 7) * 0.033333335));
        }
        pParty->pPlayers[pCastSpell->uPlayerID_2].SetCondition(Condition_Weak, 0);
        spell_sound_flag = true;
        break;
      }
      case SPELL_SPIRIT_SHARED_LIFE://����� �����
      {
        if ( skill_level == 4 )
          amount = 4 * spell_level;
        else
          amount = 3 * spell_level;
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        int active_pl_num = 0;
        signed int shared_life_count = amount;
        int mean_life = 0;
        int pl_array[4];
        for ( uint pl_id = 1; pl_id <= 4; pl_id++ )
        {
          if ( !pPlayers[pl_id]->pConditions[Condition_Dead] && !pPlayers[pl_id]->pConditions[Condition_Pertified]
            && !pPlayers[pl_id]->pConditions[Condition_Eradicated] )
            pl_array[active_pl_num++] = pl_id;
        }
        for ( uint i = 0; i < active_pl_num; i++ )
          shared_life_count += pPlayers[pl_array[i]]->sHealth;
        mean_life = (signed __int64)((double)shared_life_count / (double)active_pl_num);
        for ( uint i = 0; i < active_pl_num; i++ )
        {
          pPlayers[pl_array[i]]->sHealth = mean_life;
          if ( pPlayers[pl_array[i]]->sHealth > pPlayers[pl_array[i]]->GetMaxHealth())
            pPlayers[pl_array[i]]->sHealth = pPlayers[pl_array[i]]->GetMaxHealth();
          if ( pPlayers[pl_array[i]]->sHealth > 0 )
            pPlayers[pl_array[i]]->SetUnconcious(0);
          pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pl_array[i] - 1);
        }
        spell_sound_flag = true;
        break;
      }
      case SPELL_SPIRIT_RESSURECTION://�����������
      {
        switch (skill_level)
        {
          case 1: amount = 180 * spell_level; break;
          case 2: amount = 10800 * spell_level; break;
          case 3: amount = 259200 * spell_level; break;
          case 4: amount = 0; break;
          default:
            assert(false);
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        if ( pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Eradicated]
          || pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Dead] )
        {
          if ( !(pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Weak]) )
            pParty->pPlayers[pCastSpell->uPlayerID_2].PlaySound(SPEECH_25, 0);
          if ( skill_level == 4 )
          {
            pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Eradicated] = 0;
            pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Dead] = 0;
            pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Unconcious] = 0;
          }
          else
          {
            pParty->pPlayers[pCastSpell->uPlayerID_2].DiscardConditionIfLastsLongerThan( Condition_Eradicated,
                   (signed __int64)((double)(signed __int64)pParty->uTimePlayed - (double)(amount << 7) * 0.033333335));
            pParty->pPlayers[pCastSpell->uPlayerID_2].DiscardConditionIfLastsLongerThan( Condition_Dead,
                  (signed __int64)((double)(signed __int64)pParty->uTimePlayed - (double)(amount << 7) * 0.033333335));
            pParty->pPlayers[pCastSpell->uPlayerID_2].DiscardConditionIfLastsLongerThan( Condition_Unconcious,
                  (signed __int64)((double)(signed __int64)pParty->uTimePlayed - (double)(amount << 7) * 0.033333335));
          }
          pParty->pPlayers[pCastSpell->uPlayerID_2].SetCondition(Condition_Weak, 1);
          pParty->pPlayers[pCastSpell->uPlayerID_2].sHealth = 1;
          pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pCastSpell->uPlayerID_2);
        }
        spell_sound_flag = true;
        break;
      }
      case SPELL_MIND_CURE_PARALYSIS://������� ��������
      {
        switch (skill_level)
        {
          case 1: amount = 3600 * spell_level; break;
          case 2: amount = 3600 * spell_level; break;
          case 3: amount = 86400 * spell_level; break;
          case 4: amount = 0; break;
          default:
            assert(false);
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pCastSpell->uPlayerID_2);
        if ( !pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Paralyzed] )
        {
          spell_sound_flag = true;
          break;
        }
        if ( skill_level == 4 )
        {
          pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Paralyzed] = 0;
          spell_sound_flag = true;
          break;
        }
        pParty->pPlayers[pCastSpell->uPlayerID_2].DiscardConditionIfLastsLongerThan(Condition_Paralyzed,
               (signed __int64)((double)(signed __int64)pParty->uTimePlayed - (double)(amount << 7) * 0.033333335));
        spell_sound_flag = true;
        break;
      }
      case SPELL_MIND_REMOVE_FEAR://����� �����
      {
        switch (skill_level)
        {
          case 1: amount = 180 * spell_level; break;
          case 2: amount = 3600 * spell_level; break;
          case 3: amount = 86400 * spell_level; break;
          case 4: amount = 0; break;
          default:
            assert(false);
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pCastSpell->uPlayerID_2);
        if ( !pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Fear] )
        {
          spell_sound_flag = true;
          break;
        }
        if ( skill_level == 4 )
        {
          pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Fear] = 0;
          spell_sound_flag = true;
          break;
        }
        pParty->pPlayers[pCastSpell->uPlayerID_2].DiscardConditionIfLastsLongerThan(Condition_Fear,
                (signed __int64)((double)(signed __int64)pParty->uTimePlayed - (double)(amount << 7) * 0.033333335));
        spell_sound_flag = true;
        break;
      }
      case SPELL_MIND_TELEPATHY://���������
      {
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        if (PID_TYPE(a2) == OBJECT_Actor)
        {
          mon_id = PID_ID(a2);
          if ( !(pActors[mon_id].uAttributes & 0x800000) )
            pActors[mon_id].SetRandomGoldIfTheresNoItem();
          int gold_num = 0;
          if ( pItemsTable->pItems[pActors[mon_id].ActorHasItems[3].uItemID].uEquipType == EQUIP_GOLD )
            gold_num = pActors[mon_id].ActorHasItems[3].uSpecEnchantmentType;
          ItemGen item;
          item.Reset();
          if (pActors[mon_id].uCarriedItemID)
            item.uItemID = pActors[mon_id].uCarriedItemID;
          else
          {
            for ( uint i = 0; i < 4; ++i )
            {
              if ( pActors[mon_id].ActorHasItems[i].uItemID > 0 && pItemsTable->pItems[pActors[mon_id].ActorHasItems[i].uItemID].uEquipType != EQUIP_GOLD )
              {
                memcpy(&item, &pActors[mon_id].ActorHasItems[i], sizeof(item));
                spell_level = v723;
              }
            }
          }
          if ( gold_num > 0 )
          {
            if (item.uItemID)
              sprintf(pTmpBuf2.data(), "(%s), and %d gold", item.GetDisplayName(), gold_num);
            else
              sprintf(pTmpBuf2.data(), "%d gold", gold_num);
          }
          else
          {
            if (item.uItemID)
              sprintf(pTmpBuf2.data(), "(%s)", item.GetDisplayName());
            else
            {
              strcpy(pTmpBuf2.data(), "nothing");
              ShowStatusBarString(pTmpBuf2.data(), 2);
            }
          }
          ShowStatusBarString(pTmpBuf2.data(), 2);
          pSpellSprite.stru_24.Reset();
          pSpellSprite.spell_id = pCastSpell->uSpellID;
          pSpellSprite.spell_level = spell_level;
          pSpellSprite.spell_skill = skill_level;
          pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
          pSpellSprite.vPosition.x = pActors[mon_id].vPosition.x;
          pSpellSprite.vPosition.y = pActors[mon_id].vPosition.y;
          pSpellSprite.vPosition.z = pActors[mon_id].uActorHeight;
          pSpellSprite.uAttributes = 0;
          pSpellSprite.uSectorID = pIndoor->GetSector(pSpellSprite.vPosition.x, pSpellSprite.vPosition.y, pSpellSprite.vPosition.z);
          pSpellSprite.uSpriteFrameID = 0;
          pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
          pSpellSprite.spell_target_pid = a2;
          pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
          pSpellSprite.uFacing = LOWORD(v715.uYawAngle);
          LOBYTE(pSpellSprite.uAttributes) |= 0x80;
          pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
          pSpellSprite.Create(0, 0, 0, pCastSpell->uPlayerID + 1);
        }
        spell_sound_flag = true;
        break;
      }
      case SPELL_MIND_BERSERK://�������
      {
        switch (skill_level)
        {
          case 1: amount = 300 * spell_level; break;
          case 2: amount = 300 * spell_level; break;
          case 3: amount = 600 * spell_level; break;
          case 4: amount = 3600; break;
          default:
            assert(false);
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        mon_id = PID_ID(a2);
        if (PID_TYPE(a2) == OBJECT_Actor)
        {
          //v730 = 836 * mon_id;
          if ( pActors[mon_id].DoesDmgTypeDoDamage((DAMAGE_TYPE)7) )
          {
            pActors[mon_id].pActorBuffs[ACTOR_BUFF_CHARM].Reset();
            pActors[mon_id].pActorBuffs[ACTOR_BUFF_ENSLAVED].Reset();
            pActors[mon_id].pActorBuffs[ACTOR_BUFF_BERSERK].Apply(pParty->uTimePlayed + (signed __int64)((double)(amount << 7) * 0.033333335), skill_level, 0, 0, 0);
            pActors[mon_id].pMonsterInfo.uHostilityType = MonsterInfo::Hostility_Long;
          }
          pSpellSprite.stru_24.Reset();
          pSpellSprite.spell_id = pCastSpell->uSpellID;
          pSpellSprite.spell_level = spell_level;
          pSpellSprite.spell_skill = skill_level;
          pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
          pSpellSprite.vPosition.x = pActors[mon_id].vPosition.x;
          pSpellSprite.vPosition.y = pActors[mon_id].vPosition.y;
          pSpellSprite.vPosition.z = pActors[mon_id].vPosition.z + pActors[mon_id].uActorHeight;
          pSpellSprite.uAttributes = 0;
          pSpellSprite.uSectorID = pIndoor->GetSector(pActors[mon_id].vPosition.x, pActors[mon_id].vPosition.y, pSpellSprite.vPosition.z);
          pSpellSprite.uSpriteFrameID = 0;
          pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
          pSpellSprite.spell_target_pid = a2;
          pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
          pSpellSprite.uFacing = LOWORD(v715.uYawAngle);
          LOBYTE(pSpellSprite.uAttributes) |= 0x80;
          pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
          pSpellSprite.Create(0, 0, 0, pCastSpell->uPlayerID + 1);
        }
        spell_sound_flag = true;
        break;
      }
      case SPELL_MIND_ENSLAVE://�����������
      {
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        amount = 600 * spell_level;
        if (PID_TYPE(a2) == OBJECT_Actor)
        {
          mon_id = PID_ID(a2);
          //v730 = 836 * mon_id;
          if ( MonsterStats::BelongsToSupertype(pActors[mon_id].pMonsterInfo.uID, MONSTER_SUPERTYPE_UNDEAD) )
            break;
          if ( pActors[mon_id].DoesDmgTypeDoDamage((DAMAGE_TYPE)7) )
          {
            pActors[mon_id].pActorBuffs[ACTOR_BUFF_BERSERK].Reset();
            pActors[mon_id].pActorBuffs[ACTOR_BUFF_CHARM].Reset();
            pActors[mon_id].pActorBuffs[ACTOR_BUFF_ENSLAVED].Apply(pParty->uTimePlayed + (signed __int64)((double)(amount << 7) * 0.033333335),
                skill_level, 0, 0, 0);
          }
          pSpellSprite.stru_24.Reset();
          pSpellSprite.spell_id = pCastSpell->uSpellID;
          pSpellSprite.spell_level = spell_level;
          pSpellSprite.spell_skill = skill_level;
          pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
          pSpellSprite.vPosition.x = pActors[mon_id].vPosition.x;
          pSpellSprite.vPosition.y = pActors[mon_id].vPosition.y;
          pSpellSprite.vPosition.z = pActors[mon_id].vPosition.z + pActors[mon_id].uActorHeight;
          pSpellSprite.uAttributes = 0;
          pSpellSprite.uSectorID = pIndoor->GetSector(pActors[mon_id].vPosition.x, pActors[mon_id].vPosition.y, pSpellSprite.vPosition.z);
          pSpellSprite.uSpriteFrameID = 0;
          pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
          pSpellSprite.spell_target_pid = a2;
          pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
          pSpellSprite.uFacing = LOWORD(v715.uYawAngle);
          LOBYTE(pSpellSprite.uAttributes) |= 0x80;
          pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
          pSpellSprite.Create(0, 0, 0, pCastSpell->uPlayerID + 1);
        }
        spell_sound_flag = true;
        break;
      }
      case SPELL_MIND_MASS_FEAR://�������� �����
      {
        if ( skill_level == 4 )
          amount = 300 * spell_level;
        else
          amount = 180 * spell_level;
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        int mon_num = pRenderer->_46�6��_GetActorsInViewport(4096);
        pGame->GetStru6()->FadeScreen__like_Turn_Undead_and_mb_Armageddon(0xA0A0A, 192);
        ++pSpellSprite.uType;
        pSpellSprite.stru_24.Reset();
        pSpellSprite.spell_id = pCastSpell->uSpellID;
        pSpellSprite.spell_level = spell_level;
        pSpellSprite.spell_skill = skill_level;
        pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
        pSpellSprite.uAttributes = 0;
        pSpellSprite.uSectorID = 0;
        pSpellSprite.uSpriteFrameID = 0;
        pSpellSprite.field_60_distance_related_prolly_lod = 0;
        pSpellSprite.uFacing = 0;
        pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
        pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
        for ( a2 = 0; a2 < mon_num; ++a2 )
        {
          if ( MonsterStats::BelongsToSupertype(pActors[_50BF30_actors_in_viewport_ids[a2]].pMonsterInfo.uID, MONSTER_SUPERTYPE_UNDEAD) )
            break;
          pSpellSprite.vPosition.x = pActors[_50BF30_actors_in_viewport_ids[a2]].vPosition.x;
          pSpellSprite.vPosition.y = pActors[_50BF30_actors_in_viewport_ids[a2]].vPosition.y;
          pSpellSprite.vPosition.z = pActors[_50BF30_actors_in_viewport_ids[a2]].vPosition.z - (unsigned int)(signed __int64)((double)pActors[_50BF30_actors_in_viewport_ids[a2]].uActorHeight * -0.8);
          pSpellSprite.spell_target_pid = PID(OBJECT_Actor, _50BF30_actors_in_viewport_ids[a2]);
          pSpellSprite.Create(0, 0, 0, 0);
          if ( pActors[_50BF30_actors_in_viewport_ids[a2]].DoesDmgTypeDoDamage((DAMAGE_TYPE)7) )
          {
            pActors[_50BF30_actors_in_viewport_ids[a2]].pActorBuffs[ACTOR_BUFF_AFRAID].Apply(pParty->uTimePlayed + (signed __int64)((double)(amount << 7) * 0.033333335),
                  skill_level, 0, 0, 0);
          }
        }
        spell_sound_flag = true;
        break;
      }
      case SPELL_MIND_CURE_INSANITY://������� �������
      {
        if ( skill_level == 4 )
          amount = 0;
        else
          amount = 86400 * spell_level;
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pCastSpell->uPlayerID_2);
        if ( pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Insane] )
        {
          if ( !(pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Weak]) )
            pParty->pPlayers[pCastSpell->uPlayerID_2].PlaySound(SPEECH_25, 0);
          if ( skill_level == 4 )
            pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Insane] = 0;
          else
            pParty->pPlayers[pCastSpell->uPlayerID_2].DiscardConditionIfLastsLongerThan(Condition_Insane,
                  (signed __int64)((double)(signed __int64)pParty->uTimePlayed - (double)(amount << 7) * 0.033333335));
          pParty->pPlayers[pCastSpell->uPlayerID_2].SetCondition(Condition_Weak, 0);
        }
        spell_sound_flag = true;
        break;
      }
      case SPELL_EARTH_TELEKINESIS://���������
      {
        switch (skill_level)
        {
          case 1: amount = 2 * spell_level; break;
          case 2: amount = 2 * spell_level; break;
          case 3: amount = 3 * spell_level; break;
          case 4: amount = 4 * spell_level; break;
          default:
            assert(false);
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        int obj_id = PID_ID(a2);
        if (PID_TYPE(a2) == OBJECT_Item)
        {
          if ( pItemsTable->pItems[pSpriteObjects[obj_id].stru_24.uItemID].uEquipType == EQUIP_GOLD )
          {
            pParty->PartyFindsGold(pSpriteObjects[obj_id].stru_24.uSpecEnchantmentType, 0);
            viewparams->bRedrawGameUI = true;
          }
          else
          {
            sprintfex(pTmpBuf2.data(), pGlobalTXT_LocalizationStrings[471], pItemsTable->pItems[pSpriteObjects[obj_id].stru_24.uItemID].pUnidentifiedName);//�� ����� ^Pv[%s]!
            ShowStatusBarString(pTmpBuf2.data(), 2);
            if ( !pParty->AddItemToParty(&pSpriteObjects[obj_id].stru_24) )
              pParty->SetHoldingItem(&pSpriteObjects[obj_id].stru_24);
          }
          SpriteObject::OnInteraction(obj_id);
        }
        if (PID_TYPE(a2) == OBJECT_Actor)
          pActors[obj_id].LootActor();
        if (PID_TYPE(a2) == OBJECT_Decoration)
        {
          OpenedTelekinesis = true;
          if ( pLevelDecorations[obj_id].uEventID )
            EventProcessor(pLevelDecorations[obj_id].uEventID, a2, 1);
          if ( pLevelDecorations[pSpriteObjects[obj_id].stru_24.uItemID].IsInteractive() )
          {
            activeLevelDecoration = &pLevelDecorations[obj_id];
            EventProcessor(stru_5E4C90_MapPersistVars._decor_events[pLevelDecorations[obj_id]._idx_in_stru123 - 75] + 380, 0, 1);
            activeLevelDecoration = nullptr;
          }
        }
        if (PID_TYPE(a2) == OBJECT_BModel)
        {
          OpenedTelekinesis = true;
          if ( uCurrentlyLoadedLevelType == LEVEL_Indoor)
            v448 = pIndoor->pFaceExtras[pIndoor->pFaces[obj_id].uFaceExtraID].uEventID;
          else
            v448 = pOutdoor->pBModels[a2 >> 9].pFaces[obj_id & 0x3F].sCogTriggeredID;
          EventProcessor(v448, a2, 1);
        }
        spell_sound_flag = true;
        break;
      }
      case SPELL_BODY_CURE_WEAKNESS://������ ��������
      {
        switch (skill_level)
        {
          case 1: amount = 180 * spell_level; break;//3 ������ * ���������� ����� ������
          case 2: amount = 3600 * spell_level; break;//1 ��� * ���������� ����� ������
          case 3: amount = 86400 * spell_level; break;
          case 4: amount = 0; break;
          default:
            assert(false);
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pCastSpell->uPlayerID_2);
        if ( pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Weak] )
        {
          if ( skill_level == 4 )
          {
            pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Weak] = 0;
            spell_sound_flag = true;
            break;
          }
          pParty->pPlayers[pCastSpell->uPlayerID_2].DiscardConditionIfLastsLongerThan(Condition_Weak, 
                             (signed __int64)((double)(signed __int64)pParty->uTimePlayed - (double)(amount << 7) * 0.033333335));
        }
        spell_sound_flag = true;
        break;
      }
      case SPELL_BODY_FIRST_AID://������ ������
      {
        switch (skill_level)
        {
          case 1: amount = 2 * spell_level + 5; break;
          case 2: amount = 3 * spell_level + 5; break;
          case 3: amount = 4 * spell_level + 5; break;
          case 4: amount = 5 * spell_level + 5; break;
          default:
            assert(false);
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        if ( !pCastSpell->spell_target_pid )
        {
          pParty->pPlayers[pCastSpell->uPlayerID_2].Heal(amount);
          pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pCastSpell->uPlayerID_2);
        }
        if (PID_TYPE(pCastSpell->spell_target_pid) == OBJECT_Actor)
        {
          mon_id = PID_ID(pCastSpell->spell_target_pid);
          if ( pActors[mon_id].uAIState != Dead && pActors[mon_id].uAIState != Dying && pActors[mon_id].uAIState != Disabled
            && pActors[mon_id].uAIState != Removed )
          {
            pActors[mon_id].sCurrentHP += amount;
            if ( pActors[mon_id].sCurrentHP > pActors[mon_id].pMonsterInfo.uHP )
              pActors[mon_id].sCurrentHP = pActors[mon_id].pMonsterInfo.uHP;
          }
        }
        spell_sound_flag = true;
        break;
      }
      case SPELL_BODY_CURE_POISON://������� ����������(�����������)
      {
        switch (skill_level)
        {
          case 1: amount = 3600 * spell_level; break;
          case 2: amount = 3600 * spell_level; break;
          case 3: amount = 86400 * spell_level; break;
          case 4: amount = 0; break;
          default:
            assert(false);
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pCastSpell->uPlayerID_2);
        if ( pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Poison_Weak] 
          || pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Poison_Medium] 
          || pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Poison_Severe] )
        {
          if ( skill_level == 4 )
          {
            pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Poison_Weak] = 0;
            pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Poison_Medium] = 0;
            pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Poison_Severe] = 0;
            spell_sound_flag = true;
            break;
          }
          pParty->pPlayers[pCastSpell->uPlayerID_2].DiscardConditionIfLastsLongerThan(Condition_Poison_Weak, (signed __int64)((double)(signed __int64)pParty->uTimePlayed - (double)(amount << 7) * 0.033333335));
          pParty->pPlayers[pCastSpell->uPlayerID_2].DiscardConditionIfLastsLongerThan(Condition_Poison_Medium, (signed __int64)((double)(signed __int64)pParty->uTimePlayed - (double)(amount << 7) * 0.033333335));
          pParty->pPlayers[pCastSpell->uPlayerID_2].DiscardConditionIfLastsLongerThan(Condition_Poison_Severe, (signed __int64)((double)(signed __int64)pParty->uTimePlayed - (double)(amount << 7) * 0.033333335));
        }
        spell_sound_flag = true;
        break;
      }
      case SPELL_BODY_PROTECTION_FROM_MAGIC://������ �� �����
      {
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 0);
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 1);
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 2);
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 3);
        pParty->pPartyBuffs[PARTY_BUFF_PROTECTION_FROM_MAGIC].Apply(pParty->uTimePlayed + 
                         (signed __int64)((double)(signed int)((int)(3600 * spell_level) << 7) * 0.033333335), skill_level, spell_level, 0, 0);
        spell_sound_flag = true;
        break;
      }
      case SPELL_BODY_HAMMERHANDS://����-������
      {
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        if ( skill_level == 4 )
        {
          pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 0);
          pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 1);
          pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 2);
          pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 3);
          for ( uint pl_id = 0; pl_id < 4; pl_id++)
            pParty->pPlayers[pl_id].pPlayerBuffs[PLAYER_BUFF_HAMMERHANDS].Apply(pParty->uTimePlayed + 
                (signed int)(signed __int64)((double)(signed int)((int)(3600 * spell_level) << 7) * 0.033333335), 4, spell_level, spell_level, 0);
          spell_sound_flag = true;
          break;
        }
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pCastSpell->uPlayerID_2);
        pParty->pPlayers[pCastSpell->uPlayerID_2].pPlayerBuffs[PLAYER_BUFF_HAMMERHANDS].Apply(pParty->uTimePlayed + 
           (signed int)(signed __int64)((double)(signed int)((int)(3600 * spell_level) << 7) * 0.033333335), skill_level, spell_level, spell_level, 0);
        spell_sound_flag = true;
        break;
      }
      case SPELL_BODY_CURE_DISEASE://������ �������
      {
        if ( skill_level == 4 )
          amount = 0;
        else
          amount = 86400 * spell_level;
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pCastSpell->uPlayerID_2);
        if ( pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Disease_Weak]
          || pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Disease_Medium]
          || pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Disease_Severe] )
        {
          if ( skill_level == 4 )
          {
            pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Disease_Weak] = 0;
            pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Disease_Medium] = 0;
            pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Disease_Severe] = 0;
          }
          pParty->pPlayers[pCastSpell->uPlayerID_2].DiscardConditionIfLastsLongerThan(Condition_Disease_Weak,
                                (signed __int64)((double)(signed __int64)pParty->uTimePlayed - (double)(amount << 7) * 0.033333335));
          pParty->pPlayers[pCastSpell->uPlayerID_2].DiscardConditionIfLastsLongerThan(Condition_Disease_Medium,
                                (signed __int64)((double)(signed __int64)pParty->uTimePlayed - (double)(amount << 7) * 0.033333335));
          pParty->pPlayers[pCastSpell->uPlayerID_2].DiscardConditionIfLastsLongerThan(Condition_Disease_Severe,
                                (signed __int64)((double)(signed __int64)pParty->uTimePlayed - (double)(amount << 7) * 0.033333335));
        }
        spell_sound_flag = true;
        break;
      }
      case SPELL_BODY_POWER_CURE://���������
      {
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        for ( uint pl_id = 0; pl_id < 4;  ++pl_id )
        {
          pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pl_id);
          pParty->pPlayers[pl_id].Heal(5 * spell_level + 10);
        }
        spell_sound_flag = true;
        break;
      }
      case SPELL_LIGHT_DISPEL_MAGIC://������ ���
      {
        sRecoveryTime -= spell_level;
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        pGame->GetStru6()->FadeScreen__like_Turn_Undead_and_mb_Armageddon(0xAFF0A, 192);
        int mon_num = pRenderer->_46�6��_GetActorsInViewport(4096);
        ++pSpellSprite.uType;
        v688.x = 0;
        v688.y = 0;
        v688.z = 0;
        pSpellSprite.stru_24.Reset();
        pSpellSprite.spell_id = pCastSpell->uSpellID;
        pSpellSprite.spell_level = spell_level;
        pSpellSprite.spell_skill = skill_level;
        pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
        pSpellSprite.uAttributes = 0;
        pSpellSprite.uSectorID = 0;
        pSpellSprite.uSpriteFrameID = 0;
        pSpellSprite.field_60_distance_related_prolly_lod = 0;
        pSpellSprite.uFacing = 0;
        pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
        pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
        for ( a2 = 0; a2 < mon_num; ++a2 )
        {
          pSpellSprite.vPosition.x = pActors[_50BF30_actors_in_viewport_ids[a2]].vPosition.x;
          pSpellSprite.vPosition.y = pActors[_50BF30_actors_in_viewport_ids[a2]].vPosition.y;
          pSpellSprite.vPosition.z = pActors[_50BF30_actors_in_viewport_ids[a2]].vPosition.z - 
                                   (unsigned int)(signed __int64)((double)pActors[_50BF30_actors_in_viewport_ids[a2]].uActorHeight * -0.8);
          pSpellSprite.spell_target_pid = PID(OBJECT_Actor, _50BF30_actors_in_viewport_ids[a2]);
          Actor::DamageMonsterFromParty(PID(OBJECT_Item, pSpellSprite.Create(0, 0, 0, 0)), _50BF30_actors_in_viewport_ids[a2], &v688);
        }
        for ( a2 = 0; a2 < mon_num; ++a2 )
        {
          pSpellSprite.vPosition.x = pActors[_50BF30_actors_in_viewport_ids[a2]].vPosition.x;
          pSpellSprite.vPosition.y = pActors[_50BF30_actors_in_viewport_ids[a2]].vPosition.y;
          pSpellSprite.vPosition.z = pActors[_50BF30_actors_in_viewport_ids[a2]].vPosition.z -
                                   (unsigned int)(signed __int64)((double)pActors[_50BF30_actors_in_viewport_ids[a2]].uActorHeight * -0.8);
          pSpellSprite.spell_target_pid = PID(OBJECT_Actor, _50BF30_actors_in_viewport_ids[a2]);
          pSpellSprite.Create(0, 0, 0, 0);
          for (int i = 0; i < 22; ++i)
            pActors[_50BF30_actors_in_viewport_ids[a2]].pActorBuffs[i].Reset();
        }
        spell_sound_flag = true;
        break;
      }
      case SPELL_LIGHT_SUMMON_ELEMENTAL://���������
      {
        switch (skill_level)
        {
          case 1: v733 = 300 * spell_level; amount = 1; break;
          case 2: v733 = 300 * spell_level; amount = 1; break;
          case 3: v733 = 900 * spell_level; amount = 3; break;
          case 4: v733 = 900 * spell_level; amount = 5; break;
          default:
            assert(false);
        }
        int mon_num = 0;
        for ( uint mon_id = 0; mon_id < uNumActors; mon_id++ )
        {
          if ( pActors[mon_id].uAIState != Dead && pActors[mon_id].uAIState != Removed && pActors[mon_id].uAIState != Disabled 
               && PID(OBJECT_Player, pCastSpell->uPlayerID) == pActors[mon_id].uSummonerID )
            ++mon_num;
        }
        if ( mon_num >= amount )
        {
          ShowStatusBarString(pGlobalTXT_LocalizationStrings[648], 2);  // This character can't summon any more monsters!
          pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
          pCastSpell->uSpellID = 0;
          continue;
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        sub_44FA4C_spawn_light_elemental(pCastSpell->uPlayerID, skill_level, v733);
        spell_sound_flag = true;
        break;
      }
      case SPELL_LIGHT_DAY_OF_THE_GODS://���� �����
      {
        switch (skill_level)
        {
          case 1: duration = 10800 * spell_level; amount = 3 * spell_level + 10; break;
          case 2: duration = 10800 * spell_level; amount = 3 * spell_level + 10; break;
          case 3: duration = 14400 * spell_level; amount = 4 * spell_level + 10; break;
          case 4: duration = 18000 * spell_level; amount = 5 * spell_level + 10; break;
          default:
            assert(false);
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 0);
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 1);
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 2);
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 3);
        pParty->pPartyBuffs[PARTY_BUFF_DAY_OF_GODS].Apply(pParty->uTimePlayed + 
                          (signed __int64)((double)(signed int)((int)duration << 7) * 0.033333335), skill_level, amount, 0, 0);
        spell_sound_flag = true;
        break;
      }
      case SPELL_LIGHT_PRISMATIC_LIGHT://���� ������
      {
        if ( uCurrentlyLoadedLevelType == LEVEL_Outdoor)
        {
          ShowStatusBarString(pGlobalTXT_LocalizationStrings[497], 2);  // Can't cast Prismatic Light outdoors!
          pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
          pCastSpell->uSpellID = 0;
          continue;
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        int mon_num = pRenderer->_46�6��_GetActorsInViewport(4096);
        ++pSpellSprite.uType;
        v694.x = 0;
        v694.y = 0;
        v694.z = 0;
        pSpellSprite.stru_24.Reset();
        pSpellSprite.spell_id = pCastSpell->uSpellID;
        pSpellSprite.spell_level = spell_level;
        pSpellSprite.spell_skill = skill_level;
        pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
        pSpellSprite.uAttributes = 0;
        pSpellSprite.uSectorID = 0;
        pSpellSprite.uSpriteFrameID = 0;
        pSpellSprite.field_60_distance_related_prolly_lod = 0;
        pSpellSprite.uFacing = 0;
        pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
        pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
        for ( uint mon_id = 0; mon_id < mon_num; mon_id++ )
        {
          pSpellSprite.vPosition.x = pActors[_50BF30_actors_in_viewport_ids[mon_id]].vPosition.x;
          pSpellSprite.vPosition.y = pActors[_50BF30_actors_in_viewport_ids[mon_id]].vPosition.y;
          pSpellSprite.vPosition.z = pActors[_50BF30_actors_in_viewport_ids[mon_id]].vPosition.z -
                (unsigned int)(signed __int64)((double)pActors[_50BF30_actors_in_viewport_ids[mon_id]].uActorHeight * -0.8);
          pSpellSprite.spell_target_pid = PID(OBJECT_Actor, _50BF30_actors_in_viewport_ids[mon_id]);
          Actor::DamageMonsterFromParty(PID(OBJECT_Item, pSpellSprite.Create(0, 0, 0, 0)), _50BF30_actors_in_viewport_ids[mon_id], &v694);
        }
        //v537 = pGame->GetStru6();
        pGame->GetStru6()->_4A8BFC();
        spell_sound_flag = true;
        break;
      }
      case SPELL_LIGHT_DAY_OF_PROTECTION://���� ������
      {
        switch (skill_level)
        {
          case 1: duration = 14400 * spell_level; amount = 4 * spell_level; break;
          case 2: duration = 14400 * spell_level; amount = 4 * spell_level; break;
          case 3: duration = 14400 * spell_level; amount = 4 * spell_level; break;
          case 4: duration = 18000 * spell_level; amount = 5 * spell_level; break;
          default:
            assert(false);
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 0);
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 1);
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 2);
        pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 3);
        pParty->pPartyBuffs[PARTY_BUFF_RESIST_BODY].Apply(pParty->uTimePlayed + 
                           (signed __int64)((double)(duration << 7) * 0.033333335), skill_level, amount, 0, 0);
        pParty->pPartyBuffs[PARTY_BUFF_RESIST_MIND].Apply(pParty->uTimePlayed + 
                           (signed __int64)((double)(duration << 7) * 0.033333335), skill_level, amount, 0, 0);
        pParty->pPartyBuffs[PARTY_BUFF_RESIST_FIRE].Apply(pParty->uTimePlayed + 
                           (signed __int64)((double)(duration << 7) * 0.033333335), skill_level, amount, 0, 0);
        pParty->pPartyBuffs[PARTY_BUFF_RESIST_WATER].Apply(pParty->uTimePlayed + 
                           (signed __int64)((double)(duration << 7) * 0.033333335), skill_level, amount, 0, 0);
        pParty->pPartyBuffs[PARTY_BUFF_RESIST_AIR].Apply(pParty->uTimePlayed + 
                           (signed __int64)((double)(duration << 7) * 0.033333335), skill_level, amount, 0, 0);
        pParty->pPartyBuffs[PARTY_BUFF_RESIST_EARTH].Apply(pParty->uTimePlayed + 
                           (signed __int64)((double)(duration << 7) * 0.033333335), skill_level, amount, 0, 0);
        pParty->pPartyBuffs[PARTY_BUFF_FEATHER_FALL].Apply(pParty->uTimePlayed + 
                  (signed __int64)((double)(duration << 7) * 0.033333335), skill_level, spell_level + 5, 0, 0);
        pParty->pPartyBuffs[PARTY_BUFF_WIZARD_EYE].Apply(pParty->uTimePlayed +
                  (signed __int64)((double)(duration << 7) * 0.033333335), skill_level, spell_level + 5, 0, 0);
        spell_sound_flag = true;
        break;
      }
      case SPELL_LIGHT_HOUR_OF_POWER: //��� ����������
      {
        switch (skill_level)
        {
          case 1: duration = 4; amount = 4; break;
          case 2: duration = 4; amount = 4; break;
          case 3: duration = 12; amount = 12; break;
          case 4: duration = 20; amount = 15; break;
          default:
            assert(false);
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        bool player_weak = false;
        for ( uint pl_id = 0; pl_id < 4; pl_id++ )
        {
          pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 0);
          pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 1);
          pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 2);
          pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 3);
          pParty->pPlayers[pl_id].pPlayerBuffs[PLAYER_BUFF_BLESS].Apply(pParty->uTimePlayed + 
            (signed __int64)((double)((300 * amount * spell_level + 60) << 7) * 0.033333335), skill_level, spell_level + 5, 0, 0);
          if ( pParty->pPlayers[pl_id].pConditions[Condition_Weak] )
            player_weak = true;
        }
        pParty->pPartyBuffs[PARTY_BUFF_HEROISM].Apply(pParty->uTimePlayed +
          (signed __int64)((double)((300 * amount * spell_level + 60) << 7) * 0.033333335), skill_level, spell_level + 5, 0, 0);
        pParty->pPartyBuffs[PARTY_BUFF_SHIELD].Apply(pParty->uTimePlayed + 
          (signed __int64)((double)((300 * amount * spell_level + 60) << 7) * 0.033333335), skill_level, 0, 0, 0);
        pParty->pPartyBuffs[PARTY_BUFF_STONE_SKIN].Apply(pParty->uTimePlayed + 
          (signed __int64)((double)((300 * amount * spell_level + 60) << 7) * 0.033333335), skill_level, spell_level + 5, 0, 0);
        if ( !player_weak )
          pParty->pPartyBuffs[PARTY_BUFF_HASTE].Apply(pParty->uTimePlayed + 
            (signed __int64)((double)((60 * (spell_level * duration + 60)) << 7) * 0.033333335), skill_level, spell_level + 5, 0, 0);
        spell_sound_flag = true;
        break;
      }
      case SPELL_LIGHT_DIVINE_INTERVENTION://������������ �������������
      {
        //amount = 3;
        if ( pPlayer->uNumDivineInterventionCastsThisDay >= 3 )
        {
          ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2); // Spell failed
          pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
          pCastSpell->uSpellID = 0;
          continue;
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        for ( uint pl_id = 0; pl_id < 4; pl_id++ )
        {
          for ( uint buff_id = 0; buff_id <= 19; buff_id++ )
            pParty->pPlayers[pl_id].pConditions[buff_id] = 0;
          pParty->pPlayers[pl_id].sHealth = pParty->pPlayers[pl_id].GetMaxHealth();
          pParty->pPlayers[pl_id].sMana = pParty->pPlayers[pl_id].GetMaxMana();
          pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pl_id);
        }
        if ( pPlayer->sAgeModifier + 10 >= 120 )
          pPlayer->sAgeModifier = 120;
        else
          pPlayer->sAgeModifier = pPlayer->sAgeModifier + 10;
        sRecoveryTime += -5 * spell_level;
        ++pPlayer->uNumDivineInterventionCastsThisDay;
        spell_sound_flag = true;
        break;
      }
      case SPELL_DARK_REANIMATE: //����������
      {
        switch (skill_level)
        {
          case 1: amount = 2 * spell_level; break;
          case 2: amount = 3 * spell_level; break;
          case 3: amount = 4 * spell_level; break;
          case 4: amount = 5 * spell_level; break;
          default:
            assert(false);
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        if (!pCastSpell->spell_target_pid)
        {
          pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pCastSpell->uPlayerID_2);
          if ( pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Dead] )
          {
            pParty->pPlayers[pCastSpell->uPlayerID_2].SetCondition(Condition_Zombie, 1);
            ReloadPlayerPortraits(pCastSpell->uPlayerID_2, (pParty->pPlayers[pCastSpell->uPlayerID_2].GetSexByVoice() != 0) + 23);
            pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[Condition_Zombie] = pParty->uTimePlayed;
          }
          break;
        }
        mon_id = PID_ID(pCastSpell->spell_target_pid);
        if ( mon_id == -1 )
        {
          ShowStatusBarString(pGlobalTXT_LocalizationStrings[496], 2);  // No valid target exists!
          pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
          pCastSpell->uSpellID = 0;
          continue;
        }
        if ( pActors[mon_id].sCurrentHP > 0 
         ||  pActors[mon_id].uAIState != Dead && pActors[mon_id].uAIState != Dying )
        {
          ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2); // Spell failed
          pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
          pCastSpell->uSpellID = 0;
          continue;
        }
        ++pSpellSprite.uType;
        pSpellSprite.stru_24.Reset();
        pSpellSprite.spell_id = pCastSpell->uSpellID;
        pSpellSprite.spell_level = spell_level;
        pSpellSprite.spell_skill = skill_level;
        pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
        pSpellSprite.uAttributes = 0;
        pSpellSprite.uSectorID = 0;
        pSpellSprite.uSpriteFrameID = 0;
        pSpellSprite.field_60_distance_related_prolly_lod = 0;
        pSpellSprite.uFacing = 0;
        pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
        pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
        pSpellSprite.vPosition.x = pActors[mon_id].vPosition.x;
        pSpellSprite.vPosition.y = pActors[mon_id].vPosition.y;
        pSpellSprite.vPosition.z = pActors[mon_id].vPosition.z -
                                (unsigned int)(signed __int64)((double)pActors[mon_id].uActorHeight * -0.8);
        pSpellSprite.spell_target_pid = PID(OBJECT_Actor, mon_id);
        pSpellSprite.Create(0, 0, 0, 0);
        if ( pActors[mon_id].pMonsterInfo.uLevel > amount )
          break;
        Actor::Resurrect(mon_id);
        pActors[mon_id].pMonsterInfo.uHostilityType = (MonsterInfo::HostilityRadius)0;
        pActors[mon_id].pMonsterInfo.uTreasureDropChance = 0;
        pActors[mon_id].pMonsterInfo.uTreasureDiceRolls = 0;
        pActors[mon_id].pMonsterInfo.uTreasureDiceSides = 0;
        pActors[mon_id].pMonsterInfo.uTreasureLevel = 0;
        pActors[mon_id].pMonsterInfo.uTreasureType = 0;
        pActors[mon_id].uAlly = 9999;
        pActors[mon_id].uAttributes &= 0xF70000;
        pActors[mon_id].uGroup = 0;
        pActors[mon_id].pActorBuffs[ACTOR_BUFF_BERSERK].Reset();
        pActors[mon_id].pActorBuffs[ACTOR_BUFF_CHARM].Reset();
        pActors[mon_id].pActorBuffs[ACTOR_BUFF_ENSLAVED].Reset();
        if ( pActors[mon_id].sCurrentHP > 10 * amount )
          pActors[mon_id].sCurrentHP = 10 * amount;
        spell_sound_flag = true;
        break;
      }
      case SPELL_DARK_VAMPIRIC_WEAPON:// ������-������
      {
        amount = 16;
        if ( skill_level == 4 )
          duration = 0;
        else
          duration = 3600 * spell_level;
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        ItemGen *item = &pParty->pPlayers[pCastSpell->uPlayerID_2].pInventoryItemList[a2];
        item->UpdateTempBonus(pParty->uTimePlayed);
        if ( item->uItemID >= 64 && item->uItemID <= 65//blasters
            || LOBYTE(item->uAttributes) & 2
            || item->uSpecEnchantmentType != 0
            || item->uEnchantmentType != 0
            || pItemsTable->pItems[item->uItemID].uEquipType != EQUIP_SINGLE_HANDED 
            && pItemsTable->pItems[item->uItemID].uEquipType != EQUIP_TWO_HANDED
            && pItemsTable->pItems[item->uItemID].uEquipType != EQUIP_BOW
            || pItemsTable->IsMaterialNonCommon(item) )
        {
          _50C9D0_AfterEnchClickEventId = 113;
          _50C9D4_AfterEnchClickEventSecondParam = 0;
          _50C9D8_AfterEnchClickEventTimeout = 1;
          ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2); // Spell failed
          pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
          pCastSpell->uSpellID = 0;
          continue;
        }
        item->uSpecEnchantmentType = 16;
        if ( skill_level != 4 )
        {
          item->uExpireTime = pParty->uTimePlayed + (signed int)(signed __int64)((double)(signed int)((int)duration << 7) * 0.033333335);
          item->uAttributes |= 8;
        }
        LOBYTE(item->uAttributes) |= 0x80;
        _50C9A8_item_enchantment_timer = 256;
        spell_sound_flag = true;
        break;
      }
      case SPELL_DARK_SHARPMETAL://��������
      {
        switch (skill_level)
        {
          case 1: amount = 5; break;
          case 2: amount = 5; break;
          case 3: amount = 7; break;
          case 4: amount = 9; break;
          default:
            assert(false);
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        signed int _v726 = ((signed int)(60 * stru_5C6E00->uIntegerDoublePi) / 360);
        pSpellSprite.stru_24.Reset();
        pSpellSprite.spell_id = pCastSpell->uSpellID;
        pSpellSprite.spell_level = spell_level;
        pSpellSprite.spell_skill = skill_level;
        pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
        pSpellSprite.vPosition.x = pParty->vPosition.x;
        pSpellSprite.vPosition.y = pParty->vPosition.y;
        pSpellSprite.uAttributes = 0;
        pSpellSprite.vPosition.z = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 2;
        pSpellSprite.uSectorID = pIndoor->GetSector(pParty->vPosition.x, pParty->vPosition.y,
                                               pParty->vPosition.z + (signed int)pParty->uPartyHeight / 2);
        pSpellSprite.uSpriteFrameID = 0;
        pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
        pSpellSprite.spell_target_pid = a2;
        pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
        pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
        if ( pParty->bTurnBasedModeOn == 1 )
          LOBYTE(pSpellSprite.uAttributes) |= 4;
        v188 = (signed int)_v726 / -2;
        v189 = (signed int)_v726 / 2;
        if ( v188 <= v189 )
        {
          do
          {
            pSpellSprite.uFacing = v188 + v715.uYawAngle;
            if ( pSpellSprite.Create(pSpellSprite.uFacing, v715.uPitchAngle,
                        pObjectList->pObjects[(signed __int16)pSpellSprite.uObjectDescID].uSpeed, pCastSpell->uPlayerID + 1) != -1
                        && pParty->bTurnBasedModeOn == 1 )
              ++pTurnEngine->pending_actions;
            v188 += _v726 / (amount - 1);
          }
          while ( v188 <= v189 );
        }
        spell_sound_flag = true;
        break;
      }
      case SPELL_DARK_CONTROL_UNDEAD://����� ������
      {
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        switch (skill_level)
        {
          case 1: duration = 180 * spell_level; break;
          case 2: duration = 180 * spell_level; break;
          case 3: duration = 300 * spell_level; break;
          case 4: duration = 29030400; break;
          default:
            assert(false);
        }
        if (PID_TYPE(a2) == OBJECT_Actor)
        {
          mon_id = PID_ID(a2);
          if ( !MonsterStats::BelongsToSupertype(pActors[mon_id].pMonsterInfo.uID, MONSTER_SUPERTYPE_UNDEAD) )
            break;
          if ( !pActors[mon_id].DoesDmgTypeDoDamage((DAMAGE_TYPE)10) )
          {
            ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2); // Spell failed
            pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
            pCastSpell->uSpellID = 0;
            continue;
          }
          pActors[mon_id].pActorBuffs[ACTOR_BUFF_BERSERK].Reset();
          pActors[mon_id].pActorBuffs[ACTOR_BUFF_CHARM].Reset();
          pActors[mon_id].pActorBuffs[ACTOR_BUFF_ENSLAVED].Apply(pParty->uTimePlayed + 
                           (signed __int64)((double)(signed int)((int)duration << 7) * 0.033333335), skill_level, 0, 0, 0);
          pSpellSprite.stru_24.Reset();
          pSpellSprite.spell_id = pCastSpell->uSpellID;
          pSpellSprite.spell_level = spell_level;
          pSpellSprite.spell_skill = skill_level;
          pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
          pSpellSprite.vPosition.x = pActors[mon_id].vPosition.x;
          pSpellSprite.vPosition.y = pActors[mon_id].vPosition.y;
          pSpellSprite.vPosition.z = pActors[mon_id].vPosition.z + pActors[mon_id].uActorHeight;
          pSpellSprite.uAttributes = 0;
          pSpellSprite.uSectorID = pIndoor->GetSector(pActors[mon_id].vPosition.x, pActors[mon_id].vPosition.y, pSpellSprite.vPosition.z);
          pSpellSprite.uSpriteFrameID = 0;
          pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
          pSpellSprite.spell_target_pid = a2;
          pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
          pSpellSprite.uFacing = LOWORD(v715.uYawAngle);
          LOBYTE(pSpellSprite.uAttributes) |= 0x80;
          pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
          pSpellSprite.Create(0, 0, 0, pCastSpell->uPlayerID + 1);
        }
        spell_sound_flag = true;
        break;
      }
      case SPELL_DARK_SACRIFICE://������
      {
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        int hired_npc = 0;
        memset(&achieved_awards, 0, 4000);
        for ( uint npc_id = 0; npc_id < 2; npc_id++ ) //���������� ������� ���
        {
          if ( pParty->pHirelings[npc_id].pName != 0)
            achieved_awards[hired_npc++] = (AwardType)(npc_id + 1);
        }
        /*if ( (signed int)pNPCStats->uNumNewNPCs > 0)
        {
          //AwardType *_v734 = &achieved_awards[_v733];
          for ( int npc_id = 0; npc_id < pNPCStats->uNumNewNPCs; ++npc_id )
          {
            if ( pNPCStats->pNewNPCData[npc_id].uFlags & 0x80//hired(������)
              && (!pParty->pHirelings[0].pName
              || strcmp(pNPCStats->pNewNPCData[npc_id].pName, pParty->pHirelings[0].pName))
              && (!pParty->pHirelings[1].pName
              || strcmp(pNPCStats->pNewNPCData[npc_id].pName, pParty->pHirelings[1].pName)) )
            {
              //v607 = _v734;
              //++_v734;
              achieved_awards[hired_npc++] = (AwardType)(npc_id + 3);
            }
          }
        }*/
        if ( pCastSpell->uPlayerID_2 != 4 && pCastSpell->uPlayerID_2 != 5
           || achieved_awards[pCastSpell->uPlayerID_2 - 4] <= 0 || achieved_awards[pCastSpell->uPlayerID_2 - 4] >= 3 )
        {
          ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2); // Spell failed
          pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
          pCastSpell->uSpellID = 0;
          continue;
        }
        pParty->pHirelings[achieved_awards[pCastSpell->uPlayerID_2 - 4] - 1].evt_A = 1;
        pParty->pHirelings[achieved_awards[pCastSpell->uPlayerID_2 - 4] - 1].evt_B = 0;
        pParty->pHirelings[achieved_awards[pCastSpell->uPlayerID_2 - 4] - 1].evt_C = pIconsFrameTable->GetIconAnimLength(pIconsFrameTable->FindIcon("spell96"));
        for ( uint pl_id = 0; pl_id < 4; pl_id++ )
        {
          pParty->pPlayers[pl_id].sHealth = pParty->pPlayers[pl_id].GetMaxHealth();
          pParty->pPlayers[pl_id].sMana = pParty->pPlayers[pl_id].GetMaxMana();
        }
        v613 = &pOutdoor->ddm;
        if ( uCurrentlyLoadedLevelType != LEVEL_Outdoor)
          v613 = &pIndoor->dlv;
        v613->uReputation += 15;
        if ( v613->uReputation > 10000 )
          v613->uReputation = 10000;
        spell_sound_flag = true;
        break;
      }
      case SPELL_DARK_PAIN_REFLECTION://��������� ����
      {
        switch (skill_level)
        {
          case 1: duration = 300 * (spell_level + 12); break;
          case 2: duration = 300 * (spell_level + 12); break;
          case 3: duration = 300 * (spell_level + 12); break;
          case 4: duration = 900 * (spell_level + 4); break;
          default:
            assert(false);
        }
        amount = spell_level + 5;
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        if ( skill_level != 3 && skill_level != 4 )
        {
          pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pCastSpell->uPlayerID_2);
          pParty->pPlayers[pCastSpell->uPlayerID_2].pPlayerBuffs[PLAYER_BUFF_PAIN_REFLECTION].Apply(pParty->uTimePlayed + 
                               (signed int)(signed __int64)((double)(signed int)((int)duration << 7) * 0.033333335), skill_level, amount, 0, 0);
          spell_sound_flag = true;
          break;
        }
        for ( uint pl_id = 0; pl_id < 4; pl_id++ )
        {
          pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pl_id);
          pParty->pPlayers[pl_id].pPlayerBuffs[PLAYER_BUFF_PAIN_REFLECTION].Apply(pParty->uTimePlayed + 
                            (signed int)(signed __int64)((double)(signed int)((int)duration << 7) * 0.033333335), skill_level, amount, 0, 0);
        }
        spell_sound_flag = true;
        break;
      }
      case SPELL_DARK_SOULDRINKER://������ ����
      {
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        pGame->GetIndoorCamera();
        int mon_num = pRenderer->_46�6��_GetActorsInViewport((signed __int64)pGame->pIndoorCameraD3D->GetPickDepth());
        v707.x = 0;
        v707.y = 0;
        v707.z = 0;
        pSpellSprite.stru_24.Reset();
        pSpellSprite.spell_id = pCastSpell->uSpellID;
        pSpellSprite.spell_level = spell_level;
        pSpellSprite.spell_skill = skill_level;
        pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
        pSpellSprite.uAttributes = 0;
        pSpellSprite.uSectorID = 0;
        pSpellSprite.uSpriteFrameID = 0;
        pSpellSprite.field_60_distance_related_prolly_lod = 0;
        pSpellSprite.uFacing = 0;
        pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
        pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
        amount = 0;
        if ( mon_num > 0 )
        {
          amount = (mon_num * (7 * spell_level + 25));
          for ( uint mon_id = 0; mon_id < mon_num; mon_id++ )
          {
            pSpellSprite.vPosition.x = pActors[_50BF30_actors_in_viewport_ids[mon_id]].vPosition.x;
            pSpellSprite.vPosition.y = pActors[_50BF30_actors_in_viewport_ids[mon_id]].vPosition.y;
            pSpellSprite.vPosition.z = pActors[_50BF30_actors_in_viewport_ids[mon_id]].vPosition.z - 
                       (unsigned int)(signed __int64)((double)pActors[_50BF30_actors_in_viewport_ids[mon_id]].uActorHeight * -0.8);
            pSpellSprite.spell_target_pid = PID(OBJECT_Actor, _50BF30_actors_in_viewport_ids[mon_id]);
            Actor::DamageMonsterFromParty(PID(OBJECT_Item, pSpellSprite.Create(0, 0, 0, 0)), _50BF30_actors_in_viewport_ids[mon_id], &v707);
          }
        }
        int pl_num = 0;
        int pl_array[4];
        for ( uint pl_id = 1; pl_id <= 4; ++pl_id )
        {
          if ( !pPlayers[pl_id]->pConditions[Condition_Sleep] && !pPlayers[pl_id]->pConditions[Condition_Paralyzed]
            && !pPlayers[pl_id]->pConditions[Condition_Unconcious] && !pPlayers[pl_id]->pConditions[Condition_Dead]
            && !pPlayers[pl_id]->pConditions[Condition_Pertified] && !pPlayers[pl_id]->pConditions[Condition_Eradicated] )
          {
            pl_array[pl_num++] = pl_id;
          }
        }
        for ( uint j = 0; j < pl_num; j++ )
        {
          pPlayers[pl_array[j]]->sHealth += (signed __int64)((double)(signed int)amount / (double)pl_num);
          if ( pPlayers[pl_array[j]]->sHealth > pPlayers[pl_array[j]]->GetMaxHealth())
            pPlayers[pl_array[j]]->sHealth = pPlayers[pl_array[j]]->GetMaxHealth();
          pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pl_array[j]);
        }
        pGame->GetStru6()->FadeScreen__like_Turn_Undead_and_mb_Armageddon(0, 64);
        spell_sound_flag = true;
        break;
      }
      case SPELL_DARK_ARMAGEDDON://����������
      {
        if ( uCurrentlyLoadedLevelType == LEVEL_Indoor)
        {
          ShowStatusBarString(pGlobalTXT_LocalizationStrings[499], 2);  // Can't cast Armageddon indoors!
          pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
          pCastSpell->uSpellID = 0;
          continue;
        }
        if ( skill_level == 4)
          amount = 4;
        else
          amount = 3;
        if ( pPlayer->uNumArmageddonCasts >= amount || pParty->armageddon_timer > 0 )
        {
          ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2); // Spell failed
          pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
          pCastSpell->uSpellID = 0;
          continue;
        }
        if ( !pPlayer->CanCastSpell(uRequiredMana) )
          break;
        pParty->armageddon_timer = 256;
        pParty->armageddonDamage = spell_level;
        ++pPlayer->uNumArmageddonCasts;
        if ( pParty->bTurnBasedModeOn == 1 )
          ++pTurnEngine->pending_actions;
        for ( uint i = 0; i < 50; i++ )
        {
          v642 = rand() % 4096 - 2048;
          v643 = rand();
          v732 = GetTerrainHeightsAroundParty2(v642 + pParty->vPosition.x, pParty->vPosition.y + (v643 % 4096 - 2048), &v710, 0);
          SpriteObject::sub_42F7EB_DropItemAt(4070, v642 + pParty->vPosition.x, pParty->vPosition.y + (v643 % 4096 - 2048), v732 + 16, rand() % 500 + 500, 1, 0, 0, 0);
        }
        spell_sound_flag = true;
        break;
      }
      default:
        break;
    }
    if ( pCastSpell->uFlags & 0x20 )
    {
      if ( spell_sound_flag )
        pAudioPlayer->PlaySound((SoundID)word_4EE088_sound_ids[pCastSpell->uSpellID], 0, 0, -1, 0, pCastSpell->sound_id, 0, 0);
    }
    else
    {
      if ( sRecoveryTime < 0 )
        sRecoveryTime = 0;
      if ( pParty->bTurnBasedModeOn )
      {
        //v645 = sRecoveryTime;
        pParty->pTurnBasedPlayerRecoveryTimes[pCastSpell->uPlayerID] = sRecoveryTime;
        pPlayer->SetRecoveryTime(sRecoveryTime);
        if ( !some_active_character )
          pTurnEngine->ApplyPlayerAction();
      }
      else
        pPlayer->SetRecoveryTime((signed __int64)(flt_6BE3A4_debug_recmod1 * (double)sRecoveryTime * 2.133333333333333));
      if ( spell_sound_flag )
      {
        pPlayer->PlaySound(SPEECH_49, 0);
        //if ( spell_sound_flag )
          pAudioPlayer->PlaySound((SoundID)word_4EE088_sound_ids[pCastSpell->uSpellID], 0, 0, -1, 0, pCastSpell->sound_id, 0,	0);
      }
    }
    pCastSpell->uSpellID = 0;
    spell_level = v723;
    continue;
  }
  
}
//----- (00427DA0) --------------------------------------------------------
size_t PushCastSpellInfo(uint16_t uSpellID, uint16_t uPlayerID, __int16 skill_level, uint16_t uFlags, int spell_sound_id)
{
  for (size_t i = 0; i < CastSpellInfoCount; i++)
  {
    if (!pCastSpellInfo[i].uSpellID)
    {
      pCastSpellInfo[i].uSpellID = uSpellID;
      pCastSpellInfo[i].uPlayerID = uPlayerID;
      if (uFlags & 0x10)
        pCastSpellInfo[i].uPlayerID_2 = uPlayerID;
      pCastSpellInfo[i].field_6 = 0;
      pCastSpellInfo[i].spell_target_pid = 0;
      pCastSpellInfo[i].uFlags = uFlags;
      pCastSpellInfo[i].forced_spell_skill_level = skill_level;
      pCastSpellInfo[i].sound_id = spell_sound_id;
      return i;
    }
  }
  return -1;
}
//----- (00427D48) --------------------------------------------------------
void CastSpellInfoHelpers::_427D48()
{
  for (size_t i = 0; i < CastSpellInfoCount; i++)
  {
    if (pCastSpellInfo[i].uSpellID && pCastSpellInfo[i].uFlags & 0x3CA)
    {
      pCastSpellInfo[i].uSpellID = 0;
      pGUIWindow_Settings->Release();
      pGUIWindow_Settings = 0;
      pMouse->SetCursorBitmap("MICON1");
      GameUI_Footer_TimeLeft = 0;
      _50C9A0_IsEnchantingInProgress = 0;
      back_to_game();
    }
  }
}
//----- (0042777D) --------------------------------------------------------
void _42777D_CastSpell_UseWand_ShootArrow(int a1, unsigned int uPlayerID, unsigned int a4, __int16 a5, int a6)
{
  //bool result; // eax@2
  //__int16 v6; // bx@4
  //Player *v7; // eax@4
  //char v8; // zf@12
  unsigned __int16 v9; // cx@16
  unsigned int v10; // eax@18
  unsigned __int8 v11; // sf@18
  unsigned __int8 v12; // of@18
  unsigned __int16 v13; // cx@21
  unsigned int v14; // eax@23
  //stru277 *v15; // esi@27
  //stru277 *v16; // eax@28
  //GUIWindow *v17; // eax@43
  //GUIWindow *v18; // [sp-30h] [bp-48h]@44
  //unsigned int v19; // [sp-2Ch] [bp-44h]@44
  //unsigned int v20; // [sp-28h] [bp-40h]@44
  //unsigned int v21; // [sp-24h] [bp-3Ch]@44
  //unsigned int v22; // [sp-20h] [bp-38h]@44
  //int v23; // [sp-1Ch] [bp-34h]@44
  //int v24; // [sp-18h] [bp-30h]@44
  //unsigned int v25; // [sp-14h] [bp-2Ch]@43
  //unsigned int v26; // [sp-10h] [bp-28h]@43
  //unsigned __int8 v27; // [sp-Ch] [bp-24h]@43
  //char *v28; // [sp-8h] [bp-20h]@43
  //Texture *v29; // [sp-4h] [bp-1Ch]@43
  //__int16 a3a; // [sp+10h] [bp-8h]@1
  //__int16 a2a; // [sp+14h] [bp-4h]@1

  //a3a = uPlayerID;
  //a2a = a1;
  
  //if ( pParty->bTurnBasedModeOn != 1
  //  || (result = pTurnEngine->field_4, pTurnEngine->field_4 != 1) && pTurnEngine->field_4 != 3 )
  if (pParty->bTurnBasedModeOn)
  {
    if ( pTurnEngine->turn_stage == TE_WAIT || pTurnEngine->turn_stage == TE_MOVEMENT )
      return;
  }


    //v6 = a5;
    //v7 = &pParty->pPlayers[uPlayerID];
  assert(uPlayerID < 4);
  Player* player = &pParty->pPlayers[uPlayerID];
    if ( !(a5 & 0x10) )
    {
      switch ( a1 )
      {
        case SPELL_SPIRIT_FATE:
        case SPELL_BODY_FIRST_AID:
        case SPELL_DARK_REANIMATE:
          //HIBYTE(v6) = HIBYTE(a5) | 1;
          a5 |= 0x0100;
          break;

        case SPELL_FIRE_FIRE_AURA:
        case SPELL_WATER_RECHARGE_ITEM:
        case SPELL_WATER_ENCHANT_ITEM:
        case SPELL_DARK_VAMPIRIC_WEAPON:
          //LOBYTE(v6) = a5 | 0x80;
          a5 |= 0x0080;
          break;

        case SPELL_FIRE_FIRE_BOLT:
        case SPELL_FIRE_FIREBALL:
        case SPELL_FIRE_INCINERATE:
        case SPELL_AIR_LIGHNING_BOLT:
        case SPELL_AIR_IMPLOSION:
        case SPELL_WATER_POISON_SPRAY:
        case SPELL_WATER_ICE_BOLT:
        case SPELL_WATER_ACID_BURST:
        case SPELL_WATER_ICE_BLAST:
        case SPELL_EARTH_STUN:
        case SPELL_EARTH_SLOW:
        case SPELL_EARTH_DEADLY_SWARM:
        case SPELL_EARTH_BLADES:
        case SPELL_EARTH_MASS_DISTORTION:
        case SPELL_SPIRIT_SPIRIT_LASH:
        case SPELL_MIND_MIND_BLAST:
        case SPELL_MIND_CHARM:
        case SPELL_MIND_PSYCHIC_SHOCK:
        case SPELL_BODY_HARM:
        case SPELL_BODY_FLYING_FIST:
        case SPELL_LIGHT_LIGHT_BOLT:
        case SPELL_LIGHT_DESTROY_UNDEAD:
        case SPELL_LIGHT_SUNRAY:
        case SPELL_DARK_TOXIC_CLOUD:
        case SPELL_DARK_SHRINKING_RAY:
        case SPELL_DARK_SHARPMETAL:
        case SPELL_DARK_DRAGON_BREATH:
          if ( !a6 )
            a5 |= 0x0008;
          break;
        case SPELL_MIND_TELEPATHY:
        case SPELL_MIND_BERSERK:
        case SPELL_MIND_ENSLAVE:
        case SPELL_LIGHT_PARALYZE:
        case SPELL_DARK_CONTROL_UNDEAD:
//LABEL_9:
          //v6 = a5 | 8;
          a5 |= 0x0008;
          break;

        case SPELL_EARTH_TELEKINESIS:
          a5 |= 0x0040;
          break;

        case SPELL_SPIRIT_BLESS:
          if (a4 && ~a4 & 0x01C0)
            //goto LABEL_25;
          {
            a5 |= 0x0002;
            break;
          }
          else if ((player->pActiveSkills[PLAYER_SKILL_SPIRIT] & 0x1C0) == 0)
            //goto LABEL_25;
          {
            a5 |= 0x0002;
            break;
          }
          break;

        case SPELL_SPIRIT_PRESERVATION:
          v9 = a4;
          if ( !a4 )
            v9 = player->pActiveSkills[PLAYER_SKILL_SPIRIT];
          //goto LABEL_18;
          v10 = SkillToMastery(v9);
          v12 = __OFSUB__(v10, 3);
          v11 = ((v10 - 3) & 0x80000000u) != 0;
          //goto LABEL_24;
          if ( v11 ^ v12 )
          {
            a5 |= 0x0002;
            break;
          }
          break;

        case SPELL_DARK_PAIN_REFLECTION:
          v9 = a4;
          if ( !a4 )
            v9 = player->pActiveSkills[PLAYER_SKILL_DARK];
//LABEL_18:
          v10 = SkillToMastery(v9);
          v12 = __OFSUB__(v10, 3);
          v11 = ((v10 - 3) & 0x80000000u) != 0;
          //goto LABEL_24;
          if ( v11 ^ v12 )
          {
            a5 |= 0x0002;
            break;
          }
          break;

        case SPELL_BODY_HAMMERHANDS:
          v13 = a4;
          if ( !a4 )
            v13 = player->pActiveSkills[PLAYER_SKILL_BODY];
          v14 = SkillToMastery(v13);
          v12 = __OFSUB__(v14, 4);
          v11 = ((v14 - 4) & 0x80000000u) != 0;
//LABEL_24:
          if ( v11 ^ v12 )
            //goto LABEL_25;
          {
            a5 |= 0x0002;
            break;
          }
          break;

        case SPELL_EARTH_STONE_TO_FLESH:
        case SPELL_SPIRIT_REMOVE_CURSE:
        case SPELL_SPIRIT_RAISE_DEAD:
        case SPELL_SPIRIT_RESSURECTION:
        case SPELL_MIND_REMOVE_FEAR:
        case SPELL_MIND_CURE_PARALYSIS:
        case SPELL_MIND_CURE_INSANITY:
        case SPELL_BODY_CURE_WEAKNESS:
        case SPELL_BODY_REGENERATION:
        case SPELL_BODY_CURE_POISON:
        case SPELL_BODY_CURE_DISEASE:
//LABEL_25:
          //v6 = a5 | 2;
          a5 |= 0x0002;
          break;

        case SPELL_DARK_SACRIFICE:
          //HIBYTE(v6) = HIBYTE(a5) | 2;
          a5 |= 0x0200;
          break;
        default:
          break;
      }
    }


    if (a5 & 0x3CA)
    {
      for (uint i = 0; i < CastSpellInfoCount; ++i)
        if (pCastSpellInfo[i].uFlags & 0x3CA)
        {
          pCastSpellInfo[i].uSpellID = 0;
          break;
        }
    }
      
    for (uint i = 0; i < CastSpellInfoCount; ++i)
    {
      CastSpellInfo* spell = &pCastSpellInfo[i];
      if (!spell->uSpellID)
        continue;

      spell->uSpellID = 0;
      if (spell->uFlags & 0x3CA)
      {
        pGUIWindow_Settings->Release();
        pGUIWindow_Settings = nullptr;
        pMouse->SetCursorBitmap("MICON1");
        GameUI_Footer_TimeLeft = 0;
        _50C9A0_IsEnchantingInProgress = 0;
        back_to_game();
      }
    }

    int result = PushCastSpellInfo(a1, uPlayerID, a4, a5, a6);
    if ( result != -1 )
    {
      if ( a5 & 2 )
      {
        if ( pGUIWindow_Settings )
          return;
        pGUIWindow_Settings = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_CastSpell, (int)&pCastSpellInfo[result], 0);
        pGUIWindow_Settings->CreateButton(52, 422, 35, 0, 2, 0, UIMSG_CastSpell_Character_Big_Improvement, 0, 49, "", 0);
        pGUIWindow_Settings->CreateButton(165, 422, 35, 0, 2, 0, UIMSG_CastSpell_Character_Big_Improvement, 1, 50, "", 0);
        pGUIWindow_Settings->CreateButton(280, 422, 35, 0, 2, 0, UIMSG_CastSpell_Character_Big_Improvement, 2, 51, "", 0);
        pGUIWindow_Settings->CreateButton(390, 422, 35, 0, 2, 0, UIMSG_CastSpell_Character_Big_Improvement, 3, 52, "", 0);
        pParty->sub_421B2C_PlaceInInventory_or_DropPickedItem();
        return;
      }
      if ( a5 & 8 )
      {
        if ( pGUIWindow_Settings )
          return;

        pGUIWindow_Settings = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_CastSpell, (int)&pCastSpellInfo[result], 0);
        pGUIWindow_Settings->CreateButton(game_viewport_x, game_viewport_y, game_viewport_width, game_viewport_height, 1, 0, UIMSG_CastSpell_Shoot_Monster, 0, 0, "", 0);
        pParty->sub_421B2C_PlaceInInventory_or_DropPickedItem();
        return;
      }
      if ( a5 & 0x40 )
      {
        if ( pGUIWindow_Settings )
          return;

        pGUIWindow_Settings = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_CastSpell, (int)&pCastSpellInfo[result], 0);
        pGUIWindow_Settings->CreateButton(game_viewport_x, game_viewport_y, game_viewport_width, game_viewport_height, 1, 0, UIMSG_CastSpell_Telekinesis, 0, 0, "", 0);
        pParty->sub_421B2C_PlaceInInventory_or_DropPickedItem();
        return;
      }
      if ( (char)a5 < 0 )
      {
        if ( pGUIWindow_Settings )
          return;
        ++pIcons_LOD->uTexturePacksCount;
        if ( !pIcons_LOD->uNumPrevLoadedFiles )
          pIcons_LOD->uNumPrevLoadedFiles = pIcons_LOD->uNumLoadedFiles;
        pGUIWindow_Settings = pCastSpellInfo[result].GetCastSpellInInventoryWindow();
        _50C9A0_IsEnchantingInProgress = 1;
        some_active_character = uActiveCharacter;
        pParty->sub_421B2C_PlaceInInventory_or_DropPickedItem();
        return;
      }
      if ( HIBYTE(a5) & 1 )
      {
        if ( pGUIWindow_Settings )
          return;
        pGUIWindow_Settings = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_CastSpell, (int)&pCastSpellInfo[result], 0);
        pGUIWindow_Settings->CreateButton(0x34u, 0x1A6u, 0x23u, 0, 2, 0, UIMSG_CastSpell_Character_Small_Improvement, 0, 0x31u, "", 0);
        pGUIWindow_Settings->CreateButton(0xA5u, 0x1A6u, 0x23u, 0, 2, 0, UIMSG_CastSpell_Character_Small_Improvement, 1, 0x32u, "", 0);
        pGUIWindow_Settings->CreateButton(0x118u, 0x1A6u, 0x23u, 0, 2, 0, UIMSG_CastSpell_Character_Small_Improvement, 2, 0x33u, "", 0);
        pGUIWindow_Settings->CreateButton(0x186u, 0x1A6u, 0x23u, 0, 2, 0, UIMSG_CastSpell_Character_Small_Improvement, 3, 0x34u, "", 0);
        pGUIWindow_Settings->CreateButton(8, 8, game_viewport_width, game_viewport_height, 1, 0, UIMSG_CastSpell_Monster_Improvement, 0, 0, "", NULL);
        pParty->sub_421B2C_PlaceInInventory_or_DropPickedItem();
      }
      if ( HIBYTE(a5) & 2 && !pGUIWindow_Settings )
      {
        pGUIWindow_Settings = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_CastSpell, (int)&pCastSpellInfo[result], 0);
        pBtn_NPCLeft = pGUIWindow_Settings->CreateButton(469, 178,
                       pIcons_LOD->GetTexture(uTextureID_Btn_NPCLeft)->uTextureWidth,
                       pIcons_LOD->GetTexture(uTextureID_Btn_NPCLeft)->uTextureHeight,
                       1, 0, UIMSG_ScrollNPCPanel, 0, 0, "",
                       pIcons_LOD->GetTexture(uTextureID_Btn_NPCLeft), 0);
        pBtn_NPCRight = pGUIWindow_Settings->CreateButton(626, 178,
                       pIcons_LOD->GetTexture(uTextureID_Btn_NPCRight)->uTextureWidth,
                       pIcons_LOD->GetTexture(uTextureID_Btn_NPCRight)->uTextureHeight,
                       1, 0, UIMSG_ScrollNPCPanel, 1, 0, "",
                       pIcons_LOD->GetTexture(uTextureID_Btn_NPCRight), 0);
        pGUIWindow_Settings->CreateButton(491, 149, 64, 74, 1, 0, UIMSG_HiredNPC_CastSpell, 4, 0x35u, "", 0);
        pGUIWindow_Settings->CreateButton(561, 149, 64, 74, 1, 0, UIMSG_HiredNPC_CastSpell, 5, 0x36u, "", 0);
      }
    }
}
