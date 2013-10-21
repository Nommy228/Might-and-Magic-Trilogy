#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "mm7_data.h"
#include "DecalBuilder.h"

#include "Sprites.h"
#include "stru6.h"


#include "Actor.h"
#include "Math.h"
#include "Outdoor.h"
#include "AudioPlayer.h"
#include "Game.h"
#include "ObjectList.h"
#include "Overlays.h"
#include "Events.h"
#include "FactionTable.h"
#include "TurnEngine.h"
#include "MapInfo.h"
#include "CastSpellInfo.h"
#include "Time.h"
#include "LOD.h"
#include "Party.h"
#include "GUIWindow.h"
#include "Outdoor_stuff.h"

#include "MM7.h"
#include "SpriteObject.h"
#include "stru298.h"
#include "Log.h"
#include "Texts.h"
#include "Level/Decoration.h"





std::array<Actor, 500> pActors;
size_t uNumActors;

stru319 stru_50C198; // idb


std::array<uint, 5> dword_4DF380_hostilityRanges = {0,1024,2560,5120,10240};


//----- (0041AF52) --------------------------------------------------------
void Actor::DrawHealthBar(Actor *actor, GUIWindow *window)
{
  unsigned int bar_length; // esi@1
  unsigned int uX; // ebx@10
  unsigned int v9; // [sp+14h] [bp-Ch]@4
  unsigned int v10; // [sp+1Ch] [bp-4h]@4

  if (actor->pMonsterInfo.uHP <= 25)
    bar_length = 25;
  else if ( actor->pMonsterInfo.uHP < 200 )
    bar_length = actor->pMonsterInfo.uHP;
  else
    bar_length = 200;

  v10 = bar_length;
  if ( actor->sCurrentHP <= (0.34 * actor->pMonsterInfo.uHP) )
    v9 = uTextureID_mhp_red;
  else if ( actor->sCurrentHP <= ( 0.67 * actor->pMonsterInfo.uHP) )
    v9 = uTextureID_mhp_yel;
  else
    v9 = uTextureID_mhp_grn;

  if ( actor->sCurrentHP < (int)actor->pMonsterInfo.uHP )
  {
    v10 = bar_length / actor->pMonsterInfo.uHP * actor->sCurrentHP;
  }

  uX = window->uFrameX + (signed int)(window->uFrameWidth - bar_length) / 2;

  pRenderer->SetTextureClipRect(uX, window->uFrameY + 32, uX + bar_length, window->uFrameY + 52);
  pRenderer->DrawTextureIndexed(uX, window->uFrameY + 32, pIcons_LOD->GetTexture(uTextureID_mhp_bd));
  pRenderer->SetTextureClipRect(uX, window->uFrameY + 32, uX + v10, window->uFrameY + 52);
  pRenderer->DrawTextureIndexed(uX, window->uFrameY + 34, pIcons_LOD->GetTexture(v9));

  pRenderer->ResetTextureClipRect();
  pRenderer->DrawTextureIndexed(uX - 5, window->uFrameY + 32, pIcons_LOD->GetTexture(uTextureID_mhp_capl));
  pRenderer->DrawTextureIndexed(uX + bar_length, window->uFrameY + 32, pIcons_LOD->GetTexture(uTextureID_mhp_capr));
}

//----- (00448A40) --------------------------------------------------------
void Actor::ToggleFlag(signed int uActorID, unsigned int uFlag, int bToggle)
{
  if ( uActorID >= 0 && uActorID <= (signed int)(uNumActors - 1) )
  {
    if ( bToggle )
    {
      pActors[uActorID].uAttributes |= uFlag;
    }
    else
    {
      if ( uFlag == 0x10000 )
      {
        if (pActors[uActorID].uAIState == Disabled )
          pActors[uActorID].uAIState = Standing;
      }
      pActors[uActorID].uAttributes &= ~uFlag;
    }
  }
}

//----- (00448518) --------------------------------------------------------
void __fastcall sub_448518_npc_set_item(int npc, unsigned int item, int a3)
{
  for (uint i = 0; i < uNumActors; i++)
  {
    if (pActors[uNumActors].sNPC_ID == npc)
    {
      Actor::GiveItem(i, item, a3);
    }
  }
}

//----- (004485A7) --------------------------------------------------------
void Actor::GiveItem(signed int uActorID, unsigned int uItemID, unsigned int bGive)
{
  if ( (uActorID >= 0) && (signed int)uActorID <= (signed int)(uNumActors - 1) )
  {
    Actor* currActor = &pActors[uActorID];
    if ( bGive )
    {
      if ( currActor->uCarriedItemID == 0)
        currActor->uCarriedItemID = uItemID;
      else if ( currActor->array_000234[0].uItemID == 0)
        currActor->array_000234[0].uItemID = uItemID;
      else if ( currActor->array_000234[1].uItemID == 0)
        currActor->array_000234[1].uItemID = uItemID;
    }
    else
    {
      if ( currActor->uCarriedItemID == uItemID )
        currActor->uCarriedItemID = 0;
      else if ( currActor->array_000234[0].uItemID == uItemID )
        currActor->array_000234[0].Reset();
      else if ( currActor->array_000234[1].uItemID == uItemID )
        currActor->array_000234[1].Reset();
    }
  }
}

//----- (0040894B) --------------------------------------------------------
bool Actor::CanAct()
{
  bool isparalyzed; // esi@1
  bool isstoned; // edi@2
  AIState v3; // ax@6

  isstoned = (signed __int64)this->pActorBuffs[5].uExpireTime > 0;// stoned
  isparalyzed = (signed __int64)this->pActorBuffs[6].uExpireTime > 0;// paralyzed
  v3 = this->uAIState;
  return !(isstoned || isparalyzed || v3 == Dying || v3 == Dead || v3 == Removed || v3 == Summoned || v3 == Disabled);
}

//----- (004089C7) --------------------------------------------------------
bool Actor::IsNotAlive()
{
  bool isstoned; // esi@1

  isstoned = (signed __int64)this->pActorBuffs[5].uExpireTime > 0;// stoned
  return (isstoned || (uAIState == Dying) || (uAIState == Dead) || (uAIState == Removed) || (uAIState == Summoned) || (uAIState == Disabled));
}

//----- (004086E9) --------------------------------------------------------
void Actor::SetRandomGoldIfTheresNoItem()
{
  int v2; // edi@1
  unsigned __int8 v4; // al@7

  v2 = 0;
  if ( !this->array_000234[3].uItemID )
  {
    if ( this->pMonsterInfo.uTreasureDiceRolls )
    {
      for (int i = 0; i < this->pMonsterInfo.uTreasureDiceRolls; i++)
      {
        v2 += rand() % this->pMonsterInfo.uTreasureDiceSides + 1;
      }
      if ( v2 )
      {
        this->array_000234[3].uItemID = 197;
        this->array_000234[3].uSpecEnchantmentType = v2;    //actual gold amount
      }
    }
  }
  if ( rand() % 100 < this->pMonsterInfo.uTreasureDropChance )
  {
    v4 = this->pMonsterInfo.uTreasureLevel;
    if ( v4 )
      pItemsTable->GenerateItem(v4, this->pMonsterInfo.uTreasureType, &this->array_000234[2]);
  }
  this->uAttributes |= 0x800000;
}

//----- (00404AC7) --------------------------------------------------------
void Actor::AI_SpellAttack(unsigned int uActorID, AIDirection *pDir, int uSpellID, int a4, unsigned int uSkillLevel)
{
  Actor *actorPtr; // esi@1
  unsigned int realPoints; // edi@1
  unsigned int masteryLevel; // eax@1
  int v8; // edi@16
  signed int v10; // ecx@22
  int v19; // edi@34
  int v20; // eax@35
  signed int v23; // eax@41
  int v28; // st6@50
  int v30; // esi@50
  int v31; // ST3C_4@51
  unsigned int v32; // edi@51
  signed int v36; // eax@67
  int v39; // ecx@75
  int v42; // ecx@91
  int v44; // ecx@100
  int v48; // ecx@110
  int v51; // ecx@130
  int v54; // ecx@138
  Player *v57; // esi@145
  int v58; // eax@146
  int v59; // edi@146
  int v60; // eax@146
  int v61; // edi@146
  int v62; // eax@146
  signed int v63; // edi@146
  int v68; // edi@168
  signed int v70; // ecx@172
  int v79; // edx@185
  int v80; // eax@185
  signed int v89; // ecx@192
  signed int v91; // eax@200
  int v94; // ecx@208
  int v96; // ecx@217
  int pitch; // [sp+2Ch] [bp-A4h]@51
  int v114; // [sp+48h] [bp-88h]@41
  SpriteObject a1; // [sp+4Ch] [bp-84h]@1
  int v116; // [sp+BCh] [bp-14h]@49
  int v118; // [sp+C4h] [bp-Ch]@29
  int v119; // [sp+C8h] [bp-8h]@48
  int v120; // [sp+CCh] [bp-4h]@1
  int spellnuma; // [sp+D8h] [bp+8h]@29
  int spellnumb; // [sp+D8h] [bp+8h]@48
  int spellnumc; // [sp+D8h] [bp+8h]@50
  int spellnume; // [sp+D8h] [bp+8h]@179
  int a1a; // [sp+E0h] [bp+10h]@34
  int a1c; // [sp+E0h] [bp+10h]@184


  actorPtr = &pActors[uActorID];
  realPoints = uSkillLevel & 0x3F;
  masteryLevel = SkillToMastery(uSkillLevel);
  
  switch (uSpellID)
  {
    case SPELL_FIRE_FIRE_BOLT:
    case SPELL_FIRE_FIREBALL:
    case SPELL_FIRE_INCINERATE:
    case SPELL_AIR_LIGHNING_BOLT:
    case SPELL_WATER_ICE_BOLT:
    case SPELL_WATER_ACID_BURST:
    case SPELL_EARTH_BLADES:
    case SPELL_EARTH_ROCK_BLAST:
    case SPELL_MIND_MIND_BLAST:
    case SPELL_MIND_PSYCHIC_SHOCK:
    case SPELL_BODY_HARM:
    case SPELL_LIGHT_LIGHT_BOLT:
    case SPELL_DARK_TOXIC_CLOUD:
    case SPELL_DARK_DRAGON_BREATH:
      a1.uType = stru_4E3ACC[uSpellID].uType;
      a1.uObjectDescID = GetObjDescId(uSpellID);
      a1.stru_24.Reset();
      a1.spell_id = uSpellID;
      a1.spell_level = uSkillLevel;
      a1.vPosition.x = actorPtr->vPosition.x;
      a1.spell_skill = 0;
      a1.vPosition.y = actorPtr->vPosition.y;
      a1.vPosition.z = actorPtr->vPosition.z + ((signed int)actorPtr->uActorHeight >> 1);
      a1.uFacing = LOWORD(pDir->uYawAngle);
      a1.uSoundID = 0;
      a1.uAttributes = 0;
      v89 = pDir->uDistance;
      a1.uSectorID = pIndoor->GetSector(a1.vPosition.x, a1.vPosition.y, a1.vPosition.z);
      a1.uSpriteFrameID = 0;
      a1.spell_caster_pid = PID(OBJECT_Actor, uActorID);
      a1.spell_target_pid = 0;
      if ((double)v89 < 307.2 )
        a1.field_60_distance_related_prolly_lod = 0;
      else if ( v89 < 1024 )
        a1.field_60_distance_related_prolly_lod = 1;
      else if ( v89 < 2560 )
        a1.field_60_distance_related_prolly_lod = 2;
      else 
        a1.field_60_distance_related_prolly_lod = 3;

      a1.field_61 = 2;
      v91 = a1.Create(
        pDir->uYawAngle,
        pDir->uPitchAngle,
        pObjectList->pObjects[(signed __int16)a1.uObjectDescID].uSpeed,
        0);
      if ( v91 != -1 )
      {
        pAudioPlayer->PlaySound((SoundID)word_4EE088_sound_ids[uSpellID], PID(OBJECT_Item, v91), 0, -1, 0, 0, 0, 0);
        return;
      }
      return;
      break;

    case SPELL_FIRE_HASTE: 
      if (masteryLevel == 1 || masteryLevel == 2)
        v39 = 60 * (realPoints + 60);
      else if (masteryLevel == 3 )
        v39 = 180 * (realPoints + 20);
      else if (masteryLevel == 4 ) 
        v39 = 240 * (realPoints + 15);
      else
        v39 = 0;
      actorPtr->pActorBuffs[19].Apply(
        pParty->uTimePlayed + (signed int)(signed __int64)((double)(v39 << 7) * 0.033333335),
        masteryLevel,
        0,
        0,
        0);
      pGame->pStru6Instance->_4A7E89_sparkles_on_actor_after_it_casts_buff(actorPtr, 0xFF3C1Eu);
      pAudioPlayer->PlaySound((SoundID)10040, PID(OBJECT_Actor, uActorID), 0, -1, 0, 0, 0, 0);
      return;

    case SPELL_FIRE_METEOR_SHOWER:
      if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
        return;
      v114 = pParty->vPosition.z + 2500;
      v23 = 8;
      if (masteryLevel == 2)
        v23 = 10;
      else if (masteryLevel == 3)
        v23 = 12;
      else if (masteryLevel == 4)
        v23 = 14;
      spellnumb = 0;
      v28 = 0;
      for ( int i = 0; i < v23; i++)
      {
        v30 = rand() % 1000;
        spellnumc = v30 - 2500;
        v120 = v28 * v28;
        v119 = spellnumb * spellnumb;
        if ( sqrt((float)(spellnumc * spellnumc + v119 + v120)) <= 1.0 )
        {
          v32 = 0;
          pitch = 0;
        }
        else
        {
          v31 = (signed __int64)sqrt((float)(v119 + v120));
          v32 = stru_5C6E00->Atan2(spellnumb, (int)v28);
          pitch = stru_5C6E00->Atan2(v31, (int)spellnumc);
        }
        a1.stru_24.Reset();
        a1.uType = stru_4E3ACC[uSpellID].uType;
        a1.uObjectDescID = GetObjDescId(uSpellID);
        a1.spell_level = uSkillLevel;
        a1.vPosition.x = pParty->vPosition.x;
        a1.vPosition.y = pParty->vPosition.y;
        a1.vPosition.z = v30 + v114;
        a1.spell_id = SPELL_FIRE_METEOR_SHOWER;
        a1.spell_skill = 0;
        a1.uAttributes = 0;
        a1.uSectorID = 0;
        a1.uSpriteFrameID = 0;
        a1.spell_caster_pid = PID(OBJECT_Actor, uActorID);
        a1.spell_target_pid = 0;
        a1.field_60_distance_related_prolly_lod = stru_50C198._427546(v30 + 2500);
        a1.uFacing = v32;
        a1.uSoundID = 0;
        if (pDir->uDistance < 307.2 )
          a1.field_60_distance_related_prolly_lod = 0;
        else if ( pDir->uDistance < 1024 )
          a1.field_60_distance_related_prolly_lod = 1;
        else if ( pDir->uDistance < 2560 )
          a1.field_60_distance_related_prolly_lod = 2;
        else 
          a1.field_60_distance_related_prolly_lod = 3;
        a1.field_61 = 2;
        v36 = a1.Create(
          v32,
          pitch,
          pObjectList->pObjects[(signed __int16)a1.uObjectDescID].uSpeed,
          0);
        if ( v36 != -1 )
        {
          pAudioPlayer->PlaySound((SoundID)word_4EE088_sound_ids[9], PID(OBJECT_Item, v36), 0, -1, 0, 0, 0, 0);
        }
        spellnumb = rand() % 1024 - 512;
        v28 = rand() % 1024 - 512;
      }
      return;
      break;

    case SPELL_AIR_SPARKS:
      if (masteryLevel == 2 )
        v10 = 5;
      else if (masteryLevel == 3 )
        v10 = 7;
      else if (masteryLevel == 4 ) 
        v10 = 9;
      else 
        v10 = 3;
      spellnuma = (signed int)(60 * stru_5C6E00->uIntegerDoublePi) / 360;
      a1.uType = stru_4E3ACC[uSpellID].uType;
      v118 = (signed int)(60 * stru_5C6E00->uIntegerDoublePi) / 360 / (v10 - 1);
      a1.uObjectDescID = GetObjDescId(uSpellID);

      a1.stru_24.Reset();
      a1.spell_id = SPELL_AIR_SPARKS;
      a1.spell_level = uSkillLevel;
      a1.vPosition.x = actorPtr->vPosition.x;
      a1.spell_skill = 0;
      a1.vPosition.y = actorPtr->vPosition.y;
      a1.vPosition.z = actorPtr->vPosition.z + ((signed int)actorPtr->uActorHeight >> 1);
      a1.uFacing = pDir->uYawAngle;
      a1.uSoundID = 0;
      a1.uAttributes = 0;
      a1.uSectorID = pIndoor->GetSector(a1.vPosition.x, a1.vPosition.y, a1.vPosition.z);
      a1.spell_caster_pid = PID(OBJECT_Actor, uActorID);
      a1.uSpriteFrameID = 0;
      a1.spell_target_pid = 0;
      a1.field_60_distance_related_prolly_lod = 3;
      v19 = spellnuma / -2;
      a1a = spellnuma / 2;
      if ( spellnuma / -2 > spellnuma / 2 )
      {
        v20 = spellnuma / 2;
      }
      else
      {
        do
        {
          a1.uFacing = v19 + LOWORD(pDir->uYawAngle);
          v20 = a1.Create(
            (signed __int16)a1.uFacing,
            pDir->uPitchAngle,
            pObjectList->pObjects[(signed __int16)a1.uObjectDescID].uSpeed,
            0);
          v19 += v118;
        }
        while ( v19 <= a1a );
      }
      if ( v20 != -1 )
      {
        pAudioPlayer->PlaySound((SoundID)word_4EE088_sound_ids[15], PID(OBJECT_Item, v20), 0, -1, 0, 0, 0, 0);
        return;
      }
      return;
      break;

    case SPELL_AIR_SHIELD:
      if (masteryLevel == 1 || masteryLevel == 2)
        v8 = 300 * realPoints + 3840;
      else if (masteryLevel == 3 )
        v8 = 900 * realPoints + 3840;
      else if (masteryLevel == 4 ) 
        v8 = 3600 * (realPoints + 64);
      else
        v8 = 0;
      actorPtr->pActorBuffs[15].Apply(
        pParty->uTimePlayed + (signed int)(signed __int64)((double)(v8 << 7) * 0.033333335),
        masteryLevel,
        0,
        0,
        0);
      return;

    case SPELL_EARTH_STONESKIN:
      if (masteryLevel == 1 || masteryLevel == 2)
        v44 = 300 * realPoints + 3840;
      else if (masteryLevel == 3 )
        v44 = 900 * realPoints + 3840;
      else if (masteryLevel == 4 ) 
        v44 = 3600 * (realPoints + 64);
      else
          v44 = 0;
      actorPtr->pActorBuffs[16].Apply(
        pParty->uTimePlayed + (signed int)(signed __int64)((double)(v44 << 7) * 0.033333335),
        masteryLevel,
        realPoints + 5,
        0,
        0);
      pGame->pStru6Instance->_4A7E89_sparkles_on_actor_after_it_casts_buff(actorPtr,0x5C310Eu);
      pAudioPlayer->PlaySound((SoundID)13040, PID(OBJECT_Actor,uActorID), 0, -1, 0, 0, 0, 0);
      return;

    case SPELL_SPIRIT_BLESS:
      if (masteryLevel == 1 || masteryLevel == 2)
        v42 = 300 * realPoints + 3840;
      else if (masteryLevel == 3 )
        v42 = 900 * realPoints + 3840;
      else if (masteryLevel == 4 ) 
        v42 = 1200 * realPoints + 3840;
      else
        v42 = 0;
      actorPtr->pActorBuffs[17].Apply(
        pParty->uTimePlayed + (signed int)(signed __int64)((double)(v42 << 7) * 0.033333335),
        masteryLevel,
        realPoints + 5,
        0,
        0);
      pGame->pStru6Instance->_4A7E89_sparkles_on_actor_after_it_casts_buff(actorPtr,0xC8C805u);
      pAudioPlayer->PlaySound((SoundID)14010, PID(OBJECT_Actor,uActorID), 0, -1, 0, 0, 0, 0);
      return;
      break;

    case SPELL_SPIRIT_FATE:
      if (masteryLevel == 1 || masteryLevel == 2)
        v48 = 2 * realPoints + 40;
      else if (masteryLevel == 3 )
        v48 = 3 * realPoints + 60;
      else if (masteryLevel == 4 ) 
        v48 = 2 * (3 * realPoints + 60);
      else
        v48 = 0;
      actorPtr->pActorBuffs[11].Apply(pParty->uTimePlayed + 1280, masteryLevel, v48, 0, 0);
      pGame->pStru6Instance->_4A7E89_sparkles_on_actor_after_it_casts_buff(actorPtr,0xC8C805u);
      pAudioPlayer->PlaySound((SoundID)14020, PID(OBJECT_Actor, uActorID), 0, -1, 0, 0, 0, 0);
      return;

    case SPELL_SPIRIT_HEROISM:
      if (masteryLevel == 1 || masteryLevel == 2)
        v54 = 300 * realPoints + 3840;
      else if (masteryLevel == 3 )
        v54 = 900 * realPoints + 3840;
      else if (masteryLevel == 4 ) 
        v54 = 1200 * realPoints + 3840;
      else
        v54 = 0;
      actorPtr->pActorBuffs[18].Apply(
        pParty->uTimePlayed + (signed int)(signed __int64)((double)(v54 << 7) * 0.033333335),
        masteryLevel,
        realPoints + 5,
        0,
        0);
      pGame->pStru6Instance->_4A7E89_sparkles_on_actor_after_it_casts_buff(actorPtr,0xC8C805u);
      pAudioPlayer->PlaySound((SoundID)14060, PID(OBJECT_Actor,uActorID), 0, -1, 0, 0, 0, 0);
      return;

    case SPELL_BODY_HAMMERHANDS:
      if ( (signed int)masteryLevel <= 0 || (signed int)masteryLevel > 4 )
        v51 = 0;
      else
        v51 = 3600 * realPoints;
      actorPtr->pActorBuffs[21].Apply(
        pParty->uTimePlayed + (signed int)(signed __int64)((double)(v51 << 7) * 0.033333335),
        masteryLevel,
        realPoints,
        0,
        0);
      pGame->pStru6Instance->_4A7E89_sparkles_on_actor_after_it_casts_buff(actorPtr, 0xA81376u);
      pAudioPlayer->PlaySound((SoundID)16060, PID(OBJECT_Actor, uActorID), 0, -1, 0, 0, 0, 0);
      return;

    case SPELL_BODY_POWER_CURE:
      actorPtr->sCurrentHP += 5 * realPoints + 10;
      if ( actorPtr->sCurrentHP >= (signed int)actorPtr->pMonsterInfo.uHP )
        actorPtr->sCurrentHP = LOWORD(actorPtr->pMonsterInfo.uHP);
      pGame->pStru6Instance->_4A7E89_sparkles_on_actor_after_it_casts_buff(actorPtr, 0xA81376u);
      pAudioPlayer->PlaySound((SoundID)14020, PID(OBJECT_Actor, uActorID), 0, -1, 0, 0, 0, 0);
      return;

    case SPELL_LIGHT_DISPEL_MAGIC:
      for (int i = 0; i < 20; i++ )
      {
        pParty->pPartyBuffs[i].Reset();
      }
      for (int i = 1; i <= 4; i++)
      {
        v57 = pPlayers[i];
        v58 = v57->GetActualWillpower();
        v59 = v57->GetParameterBonus(v58);
        v60 = v57->GetActualIntelligence();
        v61 = (v57->GetParameterBonus(v60) + v59) / 2;
        v62 = v57->GetActualLuck();
        v63 = v61 + v57->GetParameterBonus(v62) + 30;
        if ( rand() % v63 < 30 )
        {
          for (uint k = 0; k < v57->pPlayerBuffs.size(); k++)
          {
            v57->pPlayerBuffs[k].Reset();
          }
          pOtherOverlayList->_4418B1(11210, i + 99, 0, 65536);
        }
      }
      pAudioPlayer->PlaySound((SoundID)word_4EE088_sound_ids[80], PID(OBJECT_Actor, uActorID), 0, -1, 0, 0, 0, 0);
      return;

    case SPELL_LIGHT_DAY_OF_PROTECTION:
       if (masteryLevel == 1 || masteryLevel == 2)
      {
        v96 = 300 * realPoints + 3840;
      }
      else if (masteryLevel == 3 )
      {
        LOWORD(realPoints) = 3 * realPoints;
        v96 = 900 * (uSkillLevel & 0x3F) + 3840;
      }
      else if (masteryLevel == 4 ) 
      {
        v96 = 1200 * realPoints + 3840;
        LOWORD(realPoints) = 4 * realPoints;
      }
      else
      {
        LOWORD(realPoints) = uSkillLevel;
        v96 = 0;
      }
      actorPtr->pActorBuffs[13].Apply(
        pParty->uTimePlayed + (signed int)(signed __int64)((double)(v96 << 7) * 0.033333335),
        masteryLevel,
        realPoints,
        0,
        0);
      pGame->pStru6Instance->_4A7E89_sparkles_on_actor_after_it_casts_buff(actorPtr, 0xFFFFFFu);
      pAudioPlayer->PlaySound((SoundID)17070, PID(OBJECT_Actor, uActorID), 0, -1, 0, 0, 0, 0);
      return;

    case SPELL_LIGHT_HOUR_OF_POWER:
      if (masteryLevel == 1 || masteryLevel == 2)
        v94 = 300 * realPoints + 3840;
      else if (masteryLevel == 3)
        v94 = 900 * realPoints + 3840;
      else if (masteryLevel == 4)
        v94 = 1200 * realPoints + 3840;
      else
        v94 = 0;
      actorPtr->pActorBuffs[14].Apply(
        pParty->uTimePlayed + (signed int)(signed __int64)((double)(v94 << 7) * 0.033333335),
        masteryLevel,
        realPoints + 5,
        0,
        0);
      pGame->pStru6Instance->_4A7E89_sparkles_on_actor_after_it_casts_buff(actorPtr, 0xFFFFFFu);
      pAudioPlayer->PlaySound((SoundID)17080, PID(OBJECT_Actor, uActorID), 0, -1, 0, 0, 0, 0);
      return;

    case SPELL_DARK_SHARPMETAL:
      if (masteryLevel == 2)
        v70 = 5;
      else if (masteryLevel == 3)
        v70 = 7;
      else if (masteryLevel == 4)
        v70 = 9;
      else
        v70 = 3;

      spellnume = (signed int)(60 * stru_5C6E00->uIntegerDoublePi) / 360;
      a1.uType = stru_4E3ACC[uSpellID].uType;
      v116 = (signed int)(60 * stru_5C6E00->uIntegerDoublePi) / 360 / (v70 - 1);
      a1.uObjectDescID = GetObjDescId(uSpellID);
      a1.stru_24.Reset();
      a1.spell_id = uSpellID;
      a1.spell_level = uSkillLevel;
      a1.vPosition.x = actorPtr->vPosition.x;
      a1.spell_skill = 0;
      a1.vPosition.y = actorPtr->vPosition.y;
      a1.vPosition.z = actorPtr->vPosition.z + ((signed int)actorPtr->uActorHeight >> 1);
      a1.uFacing = pDir->uYawAngle;
      a1.uSoundID = 0;
      a1.uAttributes = 0;
      a1.uSectorID = pIndoor->GetSector(a1.vPosition.x, a1.vPosition.y, a1.vPosition.z);
      a1.spell_caster_pid = PID(OBJECT_Actor, uActorID);
      a1.uSpriteFrameID = 0;
      a1.spell_target_pid = 0;
      a1.field_60_distance_related_prolly_lod = 3;
      a1c = spellnume / -2;
      if ( spellnume / -2 > spellnume / 2 )
      {
        v80 = spellnume / -2;
      }
      else
      {
        do
        {
          v79 = pDir->uYawAngle;
          a1.uFacing = a1c + LOWORD(pDir->uYawAngle);
          v80 = a1.Create(
            v79,
            pDir->uPitchAngle,
            pObjectList->pObjects[(signed __int16)a1.uObjectDescID].uSpeed,
            0);
          a1c += v116;
        }
        while ( a1c <= spellnume / 2 );
      }
      if ( v80 != -1 )
      {
        pAudioPlayer->PlaySound((SoundID)word_4EE088_sound_ids[93], PID(OBJECT_Item, v80), 0, -1, 0, 0, 0, 0);
        return;
      }
      return;
      break;

    case SPELL_DARK_PAIN_REFLECTION:
      if (masteryLevel == 0)
        v68 = 0;
      else if (masteryLevel == 1 || (masteryLevel == 2) || (masteryLevel == 3))
        v68 = 300 * realPoints + 3840;
      else
        v68 = 900 * realPoints + 3840;
      actorPtr->pActorBuffs[20].Apply(
        pParty->uTimePlayed + (signed int)(signed __int64)((double)(v68 << 7) * 0.033333335),
        masteryLevel,
        0,
        0,
        0);
      pGame->pStru6Instance->_4A7E89_sparkles_on_actor_after_it_casts_buff(actorPtr,0x7E7E7Eu);
      pAudioPlayer->PlaySound((SoundID)18060, PID(OBJECT_Actor, uActorID), 0, -1, 0, 0, 0, 0);
      return;
  }
}


//----- (new func) --------------------------------------------------------
unsigned short Actor::GetObjDescId( int spellId )
{
  for (unsigned int i = 0; i < pObjectList->uNumObjects; i++)
  {
    if (stru_4E3ACC[spellId].uType == pObjectList->pObjects[i].uObjectID)
    {
      return i;
      break;
    }
  }
  return 0;
}


//----- (0043ABB0) --------------------------------------------------------
bool Actor::ArePeasantsOfSameFaction(Actor *a1, Actor *a2)
{
  unsigned int v2; // esi@1
  unsigned int v3; // edi@1

  v2 = a1->uAlly;
  v3 = a2->uAlly;
  if ( !v2 )
    v2 = (a1->pMonsterInfo.uID - 1) / 3 + 1;
  if ( !v3 )
    v3 = (a2->pMonsterInfo.uID - 1) / 3 + 1;
  if ( v2 >= 39 && v2 <= 44 && v3 >= 39 && v3 <= 44
    || v2 >= 45 && v2 <= 50 && v3 >= 45 && v3 <= 50
    || v2 >= 51 && v2 <= 62 && v3 >= 51 && v3 <= 62
    || v2 >= 78 && v2 <= 83 && v3 >= 78 && v3 <= 83 
    || v2 == v3
    )
    return true;
  else
    return false;
}

//----- (0043AC45) --------------------------------------------------------
void Actor::AggroSurroundingPeasants(unsigned int uActorID, int a2)
{
  int v4; // ebx@8
  int v5; // ST1C_4@8
  int v6; // eax@8

  int x = 0; BYTE2(x) |= 8u;
  int y = 0; y |= 0x80000;
  Actor* victim = &pActors[uActorID];
  if ( a2 == 1 )
    victim->uAttributes |= 0x80000;

  for (uint i = 0; i < uNumActors; ++i)
  {
    Actor* actor = &pActors[i];
    if (!actor->CanAct() || i == uActorID)
      continue;

    if (Actor::ArePeasantsOfSameFaction(victim, actor))
    {
      v4 = abs(actor->vPosition.x - victim->vPosition.x);
      v5 = abs(actor->vPosition.y - victim->vPosition.y);
      v6 = abs(actor->vPosition.z - victim->vPosition.z);
      if (int_get_vector_length(v4, v5, v6) < 4096)
      {
        actor->pMonsterInfo.uHostilityType = MonsterInfo::Hostility_Long;
        if ( a2 == 1 )
          actor->uAttributes |= 0x80000;

      }
    }
  }
}

//----- (00404874) --------------------------------------------------------
void Actor::AI_RangedAttack( unsigned int uActorID, struct AIDirection *pDir, int type, char a4 )
{
  Actor *actPtr; // esi@1
  char specAb; // al@1
  int v13; // edx@28

  actPtr = &pActors[uActorID];

  SpriteObject a1; // [sp+Ch] [bp-74h]@1

  switch ( type )
  {
    case 1:
      a1.uType = 545;
      break;
    case 2:
      a1.uType = 550;
      break;
    case 3:
      a1.uType = 510;
      break;
    case 4:
      a1.uType = 500;
      break;
    case 5:
      a1.uType = 515;
      break;
    case 6:
      a1.uType = 505;
      break;
    case 7:
      a1.uType = 530;
      break;
    case 8:
      a1.uType = 525;
      break;
    case 9:
      a1.uType = 520;
      break;
    case 10:
      a1.uType = 535;
      break;
    case 11:
      a1.uType = 540;
      break;
    case 13:
      a1.uType = 555;
      break;
    default:
      return;
  }
  a1.uObjectDescID = GetObjDescId(a1.uType);
  a1.stru_24.Reset();
  a1.vPosition.x = actPtr->vPosition.x;
  a1.spell_id = 0;
  a1.vPosition.y = actPtr->vPosition.y;
  a1.spell_level = 0;
  a1.spell_skill = 0;
  a1.vPosition.z = actPtr->vPosition.z - (unsigned int)(actPtr->uActorHeight * -0.75);
  a1.uFacing = pDir->uYawAngle;
  a1.uSoundID = 0;
  a1.uAttributes = 0;
  a1.uSectorID = pIndoor->GetSector(a1.vPosition.x, a1.vPosition.y, a1.vPosition.z);
  a1.uSpriteFrameID = 0;
  a1.spell_caster_pid = PID(OBJECT_Actor, uActorID);
  a1.spell_target_pid = 0;
  if (pDir->uDistance < 307.2 )
    a1.field_60_distance_related_prolly_lod = 0;
  else if ( pDir->uDistance < 1024 )
    a1.field_60_distance_related_prolly_lod = 1;
  else if ( pDir->uDistance < 2560 )
    a1.field_60_distance_related_prolly_lod = 2;
  else 
    a1.field_60_distance_related_prolly_lod = 3;

  a1.field_61 = a4;
  a1.Create(
             pDir->uYawAngle,
             pDir->uPitchAngle,
             pObjectList->pObjects[(signed __int16)a1.uObjectDescID].uSpeed,
             0);
  if ( actPtr->pMonsterInfo.uSpecialAbilityType == 1 )
  {
    specAb = actPtr->pMonsterInfo.uSpecialAbilityDamageDiceBonus;
    if ( specAb == 2 )
    {
      a1.vPosition.z += 40;      
      v13 = pDir->uYawAngle;
    }
    else
    {
      if ( specAb != 3 )
        return;
      a1.Create(
        pDir->uYawAngle + 30,      //TODO find out why the YawAngle change
        pDir->uPitchAngle,
        pObjectList->pObjects[(signed __int16)a1.uObjectDescID].uSpeed,
        0);
      v13 = pDir->uYawAngle - 30;
    }
    a1.Create(
               v13,
               pDir->uPitchAngle,
               pObjectList->pObjects[(signed __int16)a1.uObjectDescID].uSpeed,
               0);
  }
  return;
}

//----- (00404736) --------------------------------------------------------
void Actor::Explode( unsigned int uActorID )
{
  Actor *v1; // esi@1
  SpriteObject a1; // [sp+Ch] [bp-78h]@1

  v1 = &pActors[uActorID];
  a1.uType = 600;
  a1.uObjectDescID = GetObjDescId(a1.uType);
  a1.stru_24.Reset();
  a1.vPosition.y = v1->vPosition.y;
  a1.spell_id = 0;
  a1.spell_level = 0;
  a1.spell_skill = 0;
  a1.vPosition.x = v1->vPosition.x;
  a1.uFacing = 0;
  a1.vPosition.z = v1->vPosition.z - (unsigned int)(v1->uActorHeight * -0.75);
  a1.uSoundID = 0;
  a1.uAttributes = 0;
  a1.uSectorID = pIndoor->GetSector(a1.vPosition.x, a1.vPosition.y, a1.vPosition.z);
  a1.uSpriteFrameID = 0;
  a1.spell_caster_pid = PID(OBJECT_Actor, uActorID);
  a1.spell_target_pid = 0;
  a1.field_60_distance_related_prolly_lod = 3;
  a1.field_61 = 4;
  a1.Create(0, 0, 0, 0);
  return;
}

//----- (004040E9) --------------------------------------------------------
// // Get direction vector from object1 to object2,
// // distance from object1 to object2 and Euler angles of the direction vector
// //
// //
// // object1 & object2 format :  objectType | (objectID << 3)
// //    objectType == 2 - SpriteObject
// //    objectType == 3 - Actor
// //    objectType == 4 - Party
// //    objectType == 5 - Decoration
// //
// // originally this function had following prototype:
// // struct DirectionInfo GetDirectionInfo(signed int object1, signed int object2, signed int a4)
// // but compiler converts functions returning structures by value in the such way
struct AIDirection * Actor::GetDirectionInfo( unsigned int uObj1ID, unsigned int uObj2ID, struct AIDirection *pOut, int a4 )
{
  signed int v4; // eax@1
  signed int v5; // ecx@1
  int v18; // edx@15
  float v31; // st7@45
  float v32; // st6@45
  float v33; // st7@45
  AIDirection *result; // eax@48
  Vec3_int_ v37; // [sp-10h] [bp-5Ch]@15
  AIDirection v41; // [sp+14h] [bp-38h]@46
  float outy2; // [sp+38h] [bp-14h]@33
  float outx2; // [sp+3Ch] [bp-10h]@33
  int outz; // [sp+40h] [bp-Ch]@6
  int outy; // [sp+44h] [bp-8h]@6
  int outx; // [sp+48h] [bp-4h]@6
  float a4a; // [sp+58h] [bp+Ch]@45

  v4 = PID_ID(uObj1ID);
  //v6 = uObj2ID;
  v5 = PID_ID(uObj2ID);
  switch( PID_TYPE(uObj1ID) )
  {
    case OBJECT_Item:
    {
      outx = pSpriteObjects[v4].vPosition.x;
      outy = pSpriteObjects[v4].vPosition.y;
      outz = pSpriteObjects[v4].vPosition.z;
      break;
    }
    case OBJECT_Actor:
    {
      outx = pActors[v4].vPosition.x;
      outy = pActors[v4].vPosition.y;
      outz = pActors[v4].vPosition.z - (unsigned int)(signed __int64)((double)pActors[v4].uActorHeight * -0.75);
      break;
    }
    case OBJECT_Player:
    {
      if ( !v4 )
      {
        outx = pParty->vPosition.x;
        outy = pParty->vPosition.y;
        outz = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3;
        break;
      }
      if ( v4 == 4 )
      {
        v18 = pParty->sRotationY - stru_5C6E00->uIntegerHalfPi;
        v37.z = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3;
        v37.x = pParty->vPosition.x;
        v37.y = pParty->vPosition.y;
        Vec3_int_::Rotate(24, v18, 0, v37, &outx, &outy, &outz);
        break;
      }
      if ( v4 == 3 )
      {
        v18 = pParty->sRotationY - stru_5C6E00->uIntegerHalfPi;
        v37.z = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3;
        v37.x = pParty->vPosition.x;
        v37.y = pParty->vPosition.y;
        Vec3_int_::Rotate(8, v18, 0, v37, &outx, &outy, &outz);
        break;
      }
      if ( v4 == 2 )
      {
        v37.z = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3;
        v18 = stru_5C6E00->uIntegerHalfPi + pParty->sRotationY;
        v37.x = pParty->vPosition.x;
        v37.y = pParty->vPosition.y;
        Vec3_int_::Rotate(8, v18, 0, v37, &outx, &outy, &outz);
        break;
      }
      if ( v4 == 1 )
      {
        v37.z = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3;
        v18 = stru_5C6E00->uIntegerHalfPi + pParty->sRotationY;
        v37.x = pParty->vPosition.x;
        v37.y = pParty->vPosition.y;
        Vec3_int_::Rotate(24, v18, 0, v37, &outx, &outy, &outz);
        break;
      }
    }
    case OBJECT_Decoration:
    {
      outx = pLevelDecorations[v4].vPosition.x;
      outy = pLevelDecorations[v4].vPosition.y;
      outz = pLevelDecorations[v4].vPosition.z;
      break;
    }
    default:
    {
      outz = 0;
      outy = 0;
      outx = 0;
      break;
    }
    case OBJECT_BModel:
    {
      if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
      {
        outx = (pIndoor->pFaces[v4].pBounding.x1 + pIndoor->pFaces[v4].pBounding.x2) >> 1;
        outy = (pIndoor->pFaces[v4].pBounding.y1 + pIndoor->pFaces[v4].pBounding.y2) >> 1;
        outz = (pIndoor->pFaces[v4].pBounding.z1 + pIndoor->pFaces[v4].pBounding.z2) >> 1;
      }
      break;
    }
  }

  switch( PID_TYPE(uObj2ID) )
  {
    case OBJECT_Item:
    {
      outx2 = (float)pSpriteObjects[v5].vPosition.x;
      outy2 =(float) pSpriteObjects[v5].vPosition.y;
      a4 = pSpriteObjects[v5].vPosition.z;
      break;
    }
    case OBJECT_Actor:
    {
      outx2 = (float)pActors[v5].vPosition.x;
      outy2 = (float)pActors[v5].vPosition.y;
      a4 = pActors[v5].vPosition.z - (unsigned int)(signed __int64)((double)pActors[v5].uActorHeight * -0.75);
      break;
    }
    case OBJECT_Player:
    {
      outx2 = (float)pParty->vPosition.x;
      outy2 = (float)pParty->vPosition.y;
      if ( !a4 )
        a4 = pParty->sEyelevel;
      a4 = pParty->vPosition.z + a4;
      break;
    }
    case OBJECT_Decoration:
    {
      outx2 = (float)pLevelDecorations[v5].vPosition.x;
      outy2 = (float)pLevelDecorations[v5].vPosition.y;
      a4 = pLevelDecorations[v5].vPosition.z;
      break;
    }
    default:
    {
      outx2 = 0.0;
      outy2 = 0.0;
      a4 = 0;
      break;
    }
    case OBJECT_BModel:
    {
      if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
      {
        outx2 = (float)((pIndoor->pFaces[v5].pBounding.x1 + pIndoor->pFaces[v5].pBounding.x2) >> 1);
        outy2 = (float)((pIndoor->pFaces[v5].pBounding.y1 + pIndoor->pFaces[v5].pBounding.y2) >> 1);
        a4 = (pIndoor->pFaces[v5].pBounding.z1 + pIndoor->pFaces[v5].pBounding.z2) >> 1;
      }
      break;
    }
  }
  
  v31 = (float)outx2 - (float)outx;
  v32 = (float)outy2 - (float)outy;
  a4a = (float)a4 - (float)outz;
  outx2 = v32 * v32;
  outy2 = v31 * v31;
  v33 = sqrt(a4a * a4a + outy2 + outx2);
  if ( v33 <= 1.0 )
  {
    v41.vDirection.x = 65536;
    v41.vDirection.y = 0;
    v41.vDirection.z = 0;
    v41.uDistance = 1;
    v41.uDistanceXZ = 1;
    v41.uYawAngle = 0;
    v41.uPitchAngle = 0;
  }
  else
  {
    v41.vDirection.x = (int32_t)(1.0 / v33 * v31 * 65536.0);
    v41.vDirection.y = (int32_t)(1.0 / v33 * v32 * 65536.0);
    v41.vDirection.z = (int32_t)(1.0 / v33 * a4a * 65536.0);
    v41.uDistance = (uint)v33;
    v41.uDistanceXZ = (uint)sqrt(outy2 + outx2);
    v41.uYawAngle = stru_5C6E00->Atan2((signed __int64)v31, (signed __int64)v32);
    v41.uPitchAngle = stru_5C6E00->Atan2(v41.uDistanceXZ, (signed __int64)a4a);
  }
  result = pOut;
  memcpy(pOut, &v41, sizeof(AIDirection));
  return result;
}

//----- (00404030) --------------------------------------------------------
void Actor::AI_FaceObject(unsigned int uActorID, unsigned int uObjID, int _48, AIDirection *a4)
{
  AIDirection *v7; // eax@3
  Actor *v9; // ebx@3
  AIDirection a3; // [sp+8h] [bp-38h]@4

  if ( rand() % 100 >= 5 )
  {
    v9 = &pActors[uActorID];
    if ( !a4 )
    {
      v7 = Actor::GetDirectionInfo(PID(OBJECT_Actor, uActorID), uObjID, &a3, 0);
    }
    else
    {
      v7 = a4;
    }
    v9->uYawAngle = v7->uYawAngle;
    v9->uCurrentActionTime = 0;
    v9->vVelocity.z = 0;
    v9->vVelocity.y = 0;
    v9->vVelocity.x = 0;
    v9->uPitchAngle = v7->uPitchAngle;
    v9->uCurrentActionLength = 256;
    v9->uAIState = Interacting;
    v9->UpdateAnimation();
  }
  else
    Actor::AI_Bored(uActorID, uObjID, a4);
}

//----- (00403F58) --------------------------------------------------------
void Actor::AI_StandOrBored(unsigned int uActorID, signed int uObjID, int uActionLength, AIDirection *a4)
{
  if (rand() % 2)
    AI_Bored(uActorID, uObjID, a4);
  else
    AI_Stand(uActorID, uObjID, uActionLength, a4);
}

//----- (00403EB6) --------------------------------------------------------
void Actor::AI_Stand(unsigned int uActorID, unsigned int object_to_face_pid, unsigned int uActionLength, AIDirection *a4)
{
  assert(uActorID < uNumActors);
  auto actor = &pActors[uActorID];
  
  AIDirection a3;
  if (!a4)
    a4 = Actor::GetDirectionInfo(PID(OBJECT_Actor, uActorID), object_to_face_pid, &a3, 0);

  actor->uAIState = Standing;
  if (!uActionLength)
    actor->uCurrentActionLength = rand() % 256 + 256;
  else
    actor->uCurrentActionLength = uActionLength;
  actor->uCurrentActionTime = 0;
  actor->uYawAngle = a4->uYawAngle;
  actor->uPitchAngle = a4->uPitchAngle;
  actor->vVelocity.z = 0;
  actor->vVelocity.y = 0;
  actor->vVelocity.x = 0;
  actor->UpdateAnimation();
}

//----- (00403E61) --------------------------------------------------------
void __fastcall Actor::StandAwhile(unsigned int uActorID)
{
  pActors[uActorID].uCurrentActionLength = rand() % 128 + 128;
  pActors[uActorID].uCurrentActionTime = 0;
  pActors[uActorID].uAIState = Standing;
  pActors[uActorID].vVelocity.z = 0;
  pActors[uActorID].vVelocity.y = 0;
  pActors[uActorID].vVelocity.x = 0;
  pActors[uActorID].UpdateAnimation();
}

//----- (00403C6C) --------------------------------------------------------
void Actor::AI_MeleeAttack(unsigned int uActorID, signed int sTargetPid, struct AIDirection *arg0)
{
  Actor *v3; // ebx@1
  int16_t v6; // esi@6
  int16_t v7; // edi@6
  signed int v8; // eax@7
  Vec3_int_ v10; // ST04_12@9
  AIDirection *v12; // eax@11
  AIDirection a3; // [sp+Ch] [bp-48h]@12
  AIDirection v20; // [sp+28h] [bp-2Ch]@12
  int v23; // [sp+4Ch] [bp-8h]@6
  unsigned int v25; // [sp+5Ch] [bp+8h]@13

  v3 = &pActors[uActorID];
  if ( v3->pMonsterInfo.uMovementType == MONSTER_MOVEMENT_TYPE_STAIONARY && v3->pMonsterInfo.uAIType == 1 )
  {
    Actor::AI_Stand(uActorID, sTargetPid, 0, arg0);
    return;
  }

  if ( PID_TYPE(sTargetPid) == OBJECT_Actor)
  {
	  v8 = PID_ID(sTargetPid);
    v6 = pActors[v8].vPosition.x;
    v7 = pActors[v8].vPosition.y;
    v23 = (int)(pActors[v8].uActorHeight * 0.75 + pActors[v8].vPosition.z);
  }
  else if ( PID_TYPE(sTargetPid) == OBJECT_Player)
  {
	  v6 = pParty->vPosition.x;
    v7 = pParty->vPosition.y;
    v23 = pParty->vPosition.z + pParty->sEyelevel;
  }
  else
  {
    Error("Should not get here");
    return;
  }
  v10.z = (int32_t)(v3->uActorHeight * 0.75 + v3->vPosition.z);
  v10.y = v3->vPosition.y;
  v10.x = v3->vPosition.x;

  if ( sub_407A1C((int)v6, (int)v7, v23, v10) )
  {
    if (arg0 != nullptr)
    {
      v12 = arg0;
    }
    else
    {
		  v12 = Actor::GetDirectionInfo(PID(OBJECT_Actor, uActorID), sTargetPid, &a3, 0);
    }
    v3->uYawAngle = LOWORD(v12->uYawAngle);
    v3->uCurrentActionLength = pSpriteFrameTable->pSpriteSFrames[v3->pSpriteIDs[ANIM_AtkMelee]].uAnimLength * 8;
    v3->uCurrentActionTime = 0;
    v3->uAIState = AttackingMelee;
    Actor::PlaySound(uActorID, 0);
    v25 = pMonsterStats->pInfos[v3->pMonsterInfo.uID].uRecoveryTime;
    if ( v3->pActorBuffs[7].uExpireTime > 0 )
    {
      v25 *= 2;
    }
    if ( pParty->bTurnBasedModeOn != 1 )
    {
      v3->pMonsterInfo.uRecoveryTime = (int)(flt_6BE3A8_debug_recmod2 * v25 * 2.133333333333333);
    }
    else
    {
      v3->pMonsterInfo.uRecoveryTime = v25;
    }
    v3->vVelocity.z = 0;
    v3->vVelocity.y = 0;
    v3->vVelocity.x = 0;
    v3->UpdateAnimation();
  }
  else
	Actor::AI_Pursue1(uActorID, sTargetPid, rand() % 2, 64, arg0);
}

//----- (00438CF3) --------------------------------------------------------
void Actor::ApplyFineForKillingPeasant(unsigned int uActorID)
{
  if ( uLevelMapStatsID == 0 || !pActors[uActorID].IsPeasant())
    return;

  if ( (uLevelMapStatsID == 6 || uLevelMapStatsID == 7) && pParty->IsPartyEvil())   //celeste and bracada
    return;

  if ( (uLevelMapStatsID == 5 || uLevelMapStatsID == 8) && pParty->IsPartyGood())   // the pit and deyja
    return;

  pParty->uFine += 100 * (pMapStats->pInfos[uLevelMapStatsID]._steal_perm + pActors[uActorID].pMonsterInfo.uLevel + pParty->GetPartyReputation());
  if ( pParty->uFine < 0 )
    pParty->uFine = 0;
  if ( pParty->uFine > 4000000 )
    pParty->uFine = 4000000;

  if (uCurrentlyLoadedLevelType == LEVEL_Outdoor)
  {
    if (pOutdoor->ddm.uReputation < 10000)
      pOutdoor->ddm.uReputation++;
  }
  else if (uCurrentlyLoadedLevelType == LEVEL_Indoor)
  {
    if (pIndoor->dlv.uReputation < 10000)
      pIndoor->dlv.uReputation++;
  }
  else assert(false);

  if ( pParty->uFine )
  {
    for ( int i = 1; i <= 4; i++)
    {
      if ( !_449B57_test_bit(pPlayers[i]->_achieved_awards_bits, 1) )
        _449B7E_toggle_bit(pPlayers[i]->_achieved_awards_bits, 1, 1u);
    }
  }
}

//----- (0043AE80) --------------------------------------------------------
void Actor::AddBloodsplatOnDamageOverlay(unsigned int uActorID, int a2, signed int a3)
{
  unsigned int v4; // esi@1

  v4 = PID(OBJECT_Actor,uActorID);
  switch ( a2 )
  {
    case 1:
      if ( a3 )
      {
        pOtherOverlayList->_4418B6(904, v4, 0, (int)(sub_43AE12(a3) * 65536.0), 0);
      }
      return;
    case 2:
      if ( a3 )
      {
        pOtherOverlayList->_4418B6(905, v4, 0, (int)(sub_43AE12(a3) * 65536.0), 0);
      }
      return;
    case 3:
      if ( a3 )
      {
        pOtherOverlayList->_4418B6(906, v4, 0, (int)(sub_43AE12(a3) * 65536.0), 0);
      }
      return;
    case 4:
      if ( a3 )
      {
        pOtherOverlayList->_4418B6(907, v4, 0, (int)(sub_43AE12(a3) * 65536.0), 0);
      }
      return;
    case 5:
      pOtherOverlayList->_4418B6(901, v4, 0, PID(OBJECT_Actor,uActorID), 0);
      return;
    case 6:
      pOtherOverlayList->_4418B6(902, v4, 0, PID(OBJECT_Actor,uActorID), 0);
      return;
    case 7:
      pOtherOverlayList->_4418B6(903, v4, 0, PID(OBJECT_Actor,uActorID), 0);
      return;
    case 8:
      pOtherOverlayList->_4418B6(900, v4, 0, PID(OBJECT_Actor,uActorID), 0);
      return;
    case 9:
      pOtherOverlayList->_4418B6(909, v4, 0, PID(OBJECT_Actor,uActorID), 0);
      return;
    case 10:
      pOtherOverlayList->_4418B6(908, v4, 0, PID(OBJECT_Actor,uActorID), 0);
      return;
    default:
      return;
  }
  return;
}

//----- (0043B3E0) --------------------------------------------------------
int Actor::_43B3E0_CalcDamage( signed int dmgSource )
{
  signed int v2; // ebp@1
  int v3; // eax@9
  signed int v4; // edi@9
  int v5; // esi@9
  unsigned __int16 v8; // si@21
  int v9; // edi@21
  signed int v10; // eax@23
  int v11; // [sp+10h] [bp-4h]@1

  v2 = 0;
  v11 = 0;

  switch( dmgSource )
  {
    case 0: 
      if ( this->pActorBuffs[14].uExpireTime > 0 )
        v2 = this->pActorBuffs[14].uPower;
      if ( this->pActorBuffs[18].uExpireTime > 0 && this->pActorBuffs[18].uPower > v2 )
        v2 = this->pActorBuffs[18].uPower;
      if ( this->pActorBuffs[21].uExpireTime > 0 )
        v2 += this->pActorBuffs[21].uPower;
      v3 = this->pMonsterInfo.uAttack1DamageDiceRolls;
      v4 = this->pMonsterInfo.uAttack1DamageDiceSides;
      v5 = this->pMonsterInfo.uAttack1DamageBonus;
      break;
    case 1: 
      v3 = this->pMonsterInfo.uAttack2DamageDiceRolls;
      v4 = this->pMonsterInfo.uAttack2DamageDiceSides;
      v5 = this->pMonsterInfo.uAttack2DamageBonus;
      break;
    case 2: 
      v8 = this->pMonsterInfo.uSpellSkillAndMastery1;
      v9 = this->pMonsterInfo.uSpell1ID;
      v10 = SkillToMastery(v8);
      return _43AFE3_calc_spell_damage(v9, v8 & 0x3F, v10, 0);
      break;
    case 3: 
      v8 = this->pMonsterInfo.uSpellSkillAndMastery2;
      v9 = this->pMonsterInfo.uSpell2ID;
      v10 = SkillToMastery(v8);
      return _43AFE3_calc_spell_damage(v9, v8 & 0x3F, v10, 0);
      break;
    case 4:
      v3 = this->pMonsterInfo.uSpecialAbilityDamageDiceRolls;
      v4 = this->pMonsterInfo.uSpecialAbilityDamageDiceSides;
      v5 = this->pMonsterInfo.uSpecialAbilityDamageDiceBonus;
    default:
      return 0;
  }
  for ( int i = 0; i < v3; i++)
  {
    v11 += rand() % v4 + 1;
  }
  return v11 + v5 + v2;
}

//----- (00438B9B) --------------------------------------------------------
bool Actor::IsPeasant()
{
  unsigned int v1; // eax@1

  v1 = this->uAlly;
  if ( !v1 )
    v1 = (this->pMonsterInfo.uID - 1) / 3 + 1;
  return (signed int)v1 >= 39 && (signed int)v1 <= 44
      || (signed int)v1 >= 45 && (signed int)v1 <= 50
      || (signed int)v1 >= 51 && (signed int)v1 <= 62
      || (signed int)v1 >= 78 && (signed int)v1 <= 83;
}

//----- (0042EBEE) --------------------------------------------------------
void Actor::StealFrom( unsigned int uActorID )
{
  Player *pPlayer; // edi@1
  int v4; // ebx@2
  unsigned int v5; // eax@2
  DDM_DLV_Header *v6; // esi@4
  int v8; // [sp+8h] [bp-4h]@6

  pPlayer = &pParty->pPlayers[uActiveCharacter-1];
  if ( pPlayer->CanAct() )
  {
    CastSpellInfoHelpers::_427D48();
    v4 = 0;
    v5 = pMapStats->GetMapInfo(pCurrentMapName);
    if ( v5 )
      v4 = pMapStats->pInfos[v5]._steal_perm;
    v6 = &pOutdoor->ddm;
    if ( uCurrentlyLoadedLevelType != LEVEL_Outdoor)
      v6 = &pIndoor->dlv;
    pPlayer->StealFromActor(uActorID, v4, v6->uReputation++);
    v8 = pPlayer->GetAttackRecoveryTime(0);
    if ( v8 < 30 )
      v8 = 30;
    if ( !pParty->bTurnBasedModeOn )
      pPlayer->SetRecoveryTime((int)(flt_6BE3A4_debug_recmod1 * v8 * 2.133333333333333));
    pTurnEngine->ApplyPlayerAction();
  }
  return;
}

//----- (00403A60) --------------------------------------------------------
void Actor::AI_SpellAttack2(unsigned int uActorID, signed int edx0, AIDirection *pDir)
{
  Actor *v3; // ebx@1
  int16_t v4; // esi@3
  int16_t v5; // edi@3
  signed int v6; // eax@4
  Vec3_int_ v7; // ST04_12@6
  AIDirection *v9; // eax@8
  __int16 v13; // ax@10
  AIDirection a3; // [sp+Ch] [bp-48h]@9
  AIDirection v18; // [sp+28h] [bp-2Ch]@9
  int v19; // [sp+44h] [bp-10h]@6
  signed int a2; // [sp+48h] [bp-Ch]@1
  int v21; // [sp+4Ch] [bp-8h]@3
  unsigned int pDira; // [sp+5Ch] [bp+8h]@10

  v3 = &pActors[uActorID];
  a2 = edx0;
  if ( PID_TYPE(edx0) == OBJECT_Actor)
  {
    v6 = PID_ID(edx0);
    v4 = pActors[v6].vPosition.x;
    v5 = pActors[v6].vPosition.y;
    v21 = (int)(pActors[v6].uActorHeight * 0.75 + pActors[v6].vPosition.z);
  }
  else if ( PID_TYPE(edx0) == OBJECT_Player)
  {
    v4 = pParty->vPosition.x;
    v5 = pParty->vPosition.y;
    v21 = pParty->vPosition.z + pParty->sEyelevel;
  }
  else
  {
    Error("Should not get here");
    return;
  }
  v19 = v3->uActorHeight;
  v7.z = v3->vPosition.z - (int)(v19 * -0.75);
  v7.y = v3->vPosition.y;
  v7.x = v3->vPosition.x;
  if ( sub_407A1C(v4, v5, v21, v7) )
  {
    if ( pDir == nullptr)
    {
      v9 = Actor::GetDirectionInfo(PID(OBJECT_Actor,uActorID), a2, &a3, 0);
    }
    else
    {
      v9 = pDir;
    }
    v3->uYawAngle = LOWORD(v9->uYawAngle);
    v13 = pSpriteFrameTable->pSpriteSFrames[v3->pSpriteIDs[ANIM_AtkRanged]].uAnimLength;
    v3->uCurrentActionLength = 8 * v13;
    v3->uCurrentActionTime = 0;
    v3->uAIState = AttackingRanged4;
    Actor::PlaySound(uActorID, 0);
    pDira = pMonsterStats->pInfos[v3->pMonsterInfo.uID].uRecoveryTime;
    if (v3->pActorBuffs[7].uExpireTime > 0)
    {
      pDira *= 2;
    }
    if ( pParty->bTurnBasedModeOn == 1 )
      v3->pMonsterInfo.uRecoveryTime = pDira;
    else
      v3->pMonsterInfo.uRecoveryTime = v3->uCurrentActionLength + (int)(flt_6BE3A8_debug_recmod2 * pDira * 2.133333333333333);
    v3->vVelocity.z = 0;
    v3->vVelocity.y = 0;
    v3->vVelocity.x = 0;
    if ( _42FB5C_check_spell(v3->pMonsterInfo.uSpell2ID) )
    {
      v3->uCurrentActionLength = 64;
      v3->uCurrentActionTime = 0;
      v3->uAIState = Fidgeting;
      v3->UpdateAnimation();
      v3->uAIState = AttackingRanged4;
    }
    else
      v3->UpdateAnimation();
  }
  else
    Actor::AI_Pursue1(uActorID, a2, uActorID, 64, pDir);
}

//----- (00403854) --------------------------------------------------------
void Actor::AI_SpellAttack1(unsigned int uActorID, signed int sTargetPid, AIDirection *pDir)
{
  Actor *v3; // ebx@1
  int16_t v4; // esi@3
  int16_t v5; // edi@3
  signed int v6; // eax@4
  Vec3_int_ v7; // ST04_12@6
  AIDirection *v9; // eax@8
  __int16 v13; // ax@10
  signed int v16; // ecx@17
  AIDirection a3; // [sp+Ch] [bp-48h]@9
  AIDirection v18; // [sp+28h] [bp-2Ch]@9
  int v19; // [sp+44h] [bp-10h]@6
  int v21; // [sp+4Ch] [bp-8h]@3
  unsigned int pDira; // [sp+5Ch] [bp+8h]@10

  v3 = &pActors[uActorID];
  if ( PID_TYPE(sTargetPid) == OBJECT_Actor)
  {
    v6 = PID_ID(sTargetPid);
    v4 = pActors[v6].vPosition.x;
    v5 = pActors[v6].vPosition.y;
    v21 = (int)(pActors[v6].uActorHeight * 0.75 + pActors[v6].vPosition.z);
  }
  else if ( PID_TYPE(sTargetPid) == OBJECT_Player)
  {
    v4 = pParty->vPosition.x;
    v5 = pParty->vPosition.y;
    v21 = pParty->vPosition.z + pParty->sEyelevel;
  }
  else
  {
    Error("Should not get here");
    return;
  }
  v19 = v3->uActorHeight;
  v7.z = v3->vPosition.z - (int)(v19 * -0.75);
  v7.y = v3->vPosition.y;
  v7.x = v3->vPosition.x;
  if ( sub_407A1C(v4, v5, v21, v7) )
  {
    if ( pDir == nullptr )
    {
      v9 = Actor::GetDirectionInfo(PID(OBJECT_Actor,uActorID), sTargetPid, &a3, 0);
    }
    else
    {
      v9 = pDir;
    }
    v3->uYawAngle = LOWORD(v9->uYawAngle);
    v13 = pSpriteFrameTable->pSpriteSFrames[v3->pSpriteIDs[ANIM_AtkRanged]].uAnimLength;
    v3->uCurrentActionLength = 8 * v13;
    v3->uCurrentActionTime = 0;
    v3->uAIState = AttackingRanged3;
    Actor::PlaySound(uActorID, 0);
    pDira = pMonsterStats->pInfos[v3->pMonsterInfo.uID].uRecoveryTime;
    if (v3->pActorBuffs[7].uExpireTime > 0)
    {
      pDira *= 2;
    }
    if ( pParty->bTurnBasedModeOn == 1 )
      v3->pMonsterInfo.uRecoveryTime = pDira;
    else
      v3->pMonsterInfo.uRecoveryTime = v3->uCurrentActionLength + (int)(flt_6BE3A8_debug_recmod2 * pDira * 2.133333333333333);
    v16 = v3->pMonsterInfo.uSpell1ID;
    v3->vVelocity.z = 0;
    v3->vVelocity.y = 0;
    v3->vVelocity.x = 0;
    if ( _42FB5C_check_spell(v16) )
    {
      v3->uCurrentActionLength = 64;
      v3->uCurrentActionTime = 0;
      v3->uAIState = Fidgeting;
      v3->UpdateAnimation();
      v3->uAIState = AttackingRanged3;
    }
    else
      v3->UpdateAnimation();
  }
  else
    Actor::AI_Pursue1(uActorID, sTargetPid, uActorID, 64, pDir);
}

//----- (0040368B) --------------------------------------------------------
void Actor::AI_MissileAttack2(unsigned int uActorID, signed int sTargetPid, AIDirection *pDir)
{
  Actor *v3; // ebx@1
  int16_t v4; // esi@3
  int16_t v5; // edi@3
  signed int v6; // eax@4
  Vec3_int_ v7; // ST04_12@6
  AIDirection *v9; // eax@8
  __int16 v13; // ax@10
  AIDirection a3; // [sp+Ch] [bp-48h]@9
  AIDirection v17; // [sp+28h] [bp-2Ch]@9
  int v18; // [sp+44h] [bp-10h]@6
  int v20; // [sp+4Ch] [bp-8h]@3
  unsigned int pDira; // [sp+5Ch] [bp+8h]@10

  v3 = &pActors[uActorID];
  if ( PID_TYPE(sTargetPid) == OBJECT_Actor)
  {
    v6 = PID_ID(sTargetPid);
    v4 = pActors[v6].vPosition.x;
    v5 = pActors[v6].vPosition.y;
    v20 = (int)(pActors[v6].uActorHeight * 0.75 + pActors[v6].vPosition.z);
  }
  else if ( PID_TYPE(sTargetPid) == OBJECT_Player)
  {
    v4 = pParty->vPosition.x;
    v5 = pParty->vPosition.y;
    v20 = pParty->vPosition.z + pParty->sEyelevel;
  }
  else
  {
    Error("Should not get here");
    return;
  }
  v18 = v3->uActorHeight;
  v7.z = v3->vPosition.z - (int)(v18 * -0.75);
  v7.y = v3->vPosition.y;
  v7.x = v3->vPosition.x;
  if ( sub_407A1C(v4, v5, v20, v7) )
  {
    if ( pDir == nullptr )
    {
      v9 = Actor::GetDirectionInfo(PID(OBJECT_Actor,uActorID), sTargetPid, &a3, 0);
    }
    else
    {
      v9 = pDir;
    }
    v3->uYawAngle = LOWORD(v9->uYawAngle);
    v13 = pSpriteFrameTable->pSpriteSFrames[v3->pSpriteIDs[ANIM_AtkRanged]].uAnimLength;
    v3->uCurrentActionLength = 8 * v13;
    v3->uCurrentActionTime = 0;
    v3->uAIState = AttackingRanged2;
    Actor::PlaySound(uActorID, 0);
    pDira = pMonsterStats->pInfos[v3->pMonsterInfo.uID].uRecoveryTime;
    if ( v3->pActorBuffs[7].uExpireTime > 0 )
    {
      pDira *= 2;
    }
    if ( pParty->bTurnBasedModeOn != 1 )
      v3->pMonsterInfo.uRecoveryTime = (int)(flt_6BE3A8_debug_recmod2 * pDira * 2.133333333333333);
    else
    {
      v3->pMonsterInfo.uRecoveryTime = pDira;
    }
    v3->vVelocity.z = 0;
    v3->vVelocity.y = 0;
    v3->vVelocity.x = 0;
    v3->UpdateAnimation();
  }
  else
    Actor::AI_Pursue1(uActorID, sTargetPid, uActorID, 64, pDir);
}

//----- (00403476) --------------------------------------------------------
void Actor::AI_MissileAttack1(unsigned int uActorID, signed int sTargetPid, AIDirection *pDir)
{
  Actor *v3; // ebx@1
  int v4; // esi@3
  int v5; // edi@3
  signed int v6; // eax@4
  Vec3_int_ v7; // ST04_12@6
  AIDirection *v10; // eax@9
  __int16 v14; // ax@11
  AIDirection a3; // [sp+Ch] [bp-48h]@10
  AIDirection v18; // [sp+28h] [bp-2Ch]@10
  int v19; // [sp+44h] [bp-10h]@6
  //signed int a2; // [sp+48h] [bp-Ch]@1
  int v22; // [sp+50h] [bp-4h]@3
  unsigned int pDira; // [sp+5Ch] [bp+8h]@11

  v3 = &pActors[uActorID];
  //a2 = edx0;
  if ( PID_TYPE(sTargetPid) == OBJECT_Actor)
  {
    v6 = PID_ID(sTargetPid);
    v4 = pActors[v6].vPosition.x;
    v5 = pActors[v6].vPosition.y;
    v22 = (int)(pActors[v6].uActorHeight * 0.75 + pActors[v6].vPosition.z);
  }
  else
  {
    if ( PID_TYPE(sTargetPid) == OBJECT_Player)
    {
      v4 = pParty->vPosition.x;
      v5 = pParty->vPosition.y;
      v22 = pParty->vPosition.z + pParty->sEyelevel;
    }
    else
    {
      v4 = (int)pDir;
      v5 = (int)pDir;
    }
  }
  v19 = v3->uActorHeight;
  v7.z = v3->vPosition.z - (unsigned int)(signed __int64)((double)v19 * -0.75);
  v7.y = v3->vPosition.y;
  v7.x = v3->vPosition.x;
  if ( sub_407A1C(v4, v5, v22, v7) || sub_407A1C(v7.x, v7.y, v7.z, Vec3_int_(v4, v5, v22)))
  {
    if ( pDir == nullptr )
    {
      v10 = Actor::GetDirectionInfo(PID(OBJECT_Actor,uActorID), sTargetPid, &a3, 0);
    }
    else
    {
      v10 = pDir;
    }
    v3->uYawAngle = LOWORD(v10->uYawAngle);
    v14 = pSpriteFrameTable->pSpriteSFrames[v3->pSpriteIDs[ANIM_AtkRanged]].uAnimLength;
    v3->uCurrentActionLength = 8 * v14;
    v3->uCurrentActionTime = 0;
    v3->uAIState = AttackingRanged1;
    Actor::PlaySound(uActorID, 0);
    pDira = pMonsterStats->pInfos[v3->pMonsterInfo.uID].uRecoveryTime;
    if ( v3->pActorBuffs[7].uExpireTime > 0 )
    {
      pDira *= 2;
    }
    if ( pParty->bTurnBasedModeOn == 1 )
      v3->pMonsterInfo.uRecoveryTime = pDira;
    else
      v3->pMonsterInfo.uRecoveryTime = v3->uCurrentActionLength - (int)(flt_6BE3A8_debug_recmod2 * pDira * -2.133333333333333);
    v3->vVelocity.z = 0;
    v3->vVelocity.y = 0;
    v3->vVelocity.x = 0;
    v3->UpdateAnimation();
  }
  else
    Actor::AI_Pursue1(uActorID, sTargetPid, uActorID, 64, pDir);
}

//----- (004032B2) --------------------------------------------------------
void Actor::AI_RandomMove( unsigned int uActor_id, unsigned int uTarget_id, int radius, int uActionLength )
{
  Actor *v5; // esi@1
  int x; // ebx@1
  int absy; // eax@1
  unsigned int v9; // ebx@11
  int v10; // ebx@13
  AIDirection doNotInitializeBecauseShouldBeRandom; // [sp+Ch] [bp-30h]@7
  unsigned int v16; // [sp+2Ch] [bp-10h]@1
  int y; // [sp+30h] [bp-Ch]@1
  int absx; // [sp+38h] [bp-4h]@1
  
  v5 = &pActors[uActor_id];
  v16 = uTarget_id;
  x = v5->vInitialPosition.x - v5->vPosition.x;
  y = v5->vInitialPosition.y - v5->vPosition.y;
  absx = abs(x);
  absy = abs(y);
  if ( absx <= absy )
    absx = absy + (absx / 2 );
  else
    absx = absx + absy / 2;
  if ( MonsterStats::BelongsToSupertype(v5->pMonsterInfo.uID, MONSTER_SUPERTYPE_TREANT) )
  {
    if ( !uActionLength )
      uActionLength = 256;
    Actor::AI_StandOrBored(uActor_id, OBJECT_Player, uActionLength, &doNotInitializeBecauseShouldBeRandom);
    return;
  }
  if ( pActors[uActor_id].pMonsterInfo.uMovementType == 3 && absx < 128 )
  {
    Actor::AI_Stand(uActor_id, uTarget_id, 256, &doNotInitializeBecauseShouldBeRandom);
    return;
  }
  absx += ((rand() & 0xF) * radius) / 16;
  v9 = (stru_5C6E00->uIntegerDoublePi - 1) & stru_5C6E00->Atan2(x, y);
  if ( rand() % 100 < 25 )
  {
    Actor::StandAwhile(uActor_id);
    return;
  }
  v10 = v9 + rand() % 256 - 128;
  if ( abs(v10 - v5->uYawAngle) > 256 && !(v5->uAttributes & 0x200000) )
  {
    Actor::AI_Stand(uActor_id, uTarget_id, 256, &doNotInitializeBecauseShouldBeRandom);
    return;
  }
  v5->uYawAngle = v10;
  if ( v5->uMovementSpeed)
    v5->uCurrentActionLength = 32 * absx / v5->uMovementSpeed;
  else
    v5->uCurrentActionLength = 0;
  v5->uCurrentActionTime = 0;
  v5->uAIState = Tethered;
  if ( rand() % 100 < 2 )
    Actor::PlaySound(uActor_id, 3u);
  v5->UpdateAnimation();
}

//----- (004031C1) --------------------------------------------------------
char __fastcall Actor::_4031C1_update_job_never_gets_called(unsigned int uActorID, signed int a2, int a3)   //attempted to implement something like jobs for actors, but apparently was never finished
{
  return 0;
  /*unsigned int v3; // edi@1
  Actor *v4; // esi@1
  ActorJob *v5; // eax@1
  signed int v6; // edx@2
  ActorJob *v7; // eax@2
  signed int v8; // edi@2
  ActorJob *v9; // ecx@2
  __int16 v10; // cx@15
  signed int v12; // [sp+8h] [bp-4h]@1

  v3 = uActorID;
  v12 = a2;
  v4 = &pActors[uActorID];
  v5 = (ActorJob *)pActors[uActorID].CanAct();
  if ( v5 )
  {
    v6 = 65535;
    v7 = &v4->pScheduledJobs[v3];
    v8 = 7;
    v9 = &v7[7];//(char *)&v7[7].uHour;
	while ( !(v9->uAttributes & 1) || v9->uHour > v12 )
    {
      --v8;
      --v9;
      if ( v8 < 0 )
        break;
    }
	if( v8 >= 0 )
		v6 = v8;
    if ( !v8 && v6 == 65535 )
      v6 = 7;
    v5 = &v7[v6];
    if ( v4->vInitialPosition.x != v5->vPos.x
      || v4->vInitialPosition.y != v5->vPos.y
      || v4->vInitialPosition.z != v5->vPos.z
      || v4->pMonsterInfo.uMovementType != v5->uAction )
    {
      v4->vInitialPosition.x = v5->vPos.x;
      v4->vInitialPosition.y = v5->vPos.y;
      v10 = v5->vPos.z;
      v4->vInitialPosition.z = v10;
      LOBYTE(v5) = v5->uAction;
      v4->pMonsterInfo.uMovementType = MONSTER_MOVEMENT_TYPE_STAIONARY;
      if ( a3 == 1 )
      {
        v4->vPosition.x = v4->vInitialPosition.x;
        v4->vPosition.y = v4->vInitialPosition.y;
        LOBYTE(v5) = v10;
        v4->vPosition.z = v10;
      }
    }
  }
  return (char)v5;*/
}

//----- (004030AD) --------------------------------------------------------
void Actor::AI_Stun(unsigned int uActorID, signed int edx0, int stunRegardlessOfState)
{
  Actor *v4; // ebx@1
  __int16 v7; // ax@16
  AIDirection a3; // [sp+Ch] [bp-40h]@16
  AIDirection* v10; // [sp+28h] [bp-24h]@16

  v4 = &pActors[uActorID];
  if ( v4->uAIState == Fleeing )
    BYTE2(v4->uAttributes) |= 2u;
  if ( v4->pMonsterInfo.uHostilityType != 4 )
  {
    v4->uAttributes &= 0xFFFFFFFBu;
    v4->pMonsterInfo.uHostilityType = MonsterInfo::Hostility_Long;
  }
  if ( v4->pActorBuffs[1].uExpireTime > 0 )
    v4->pActorBuffs[1].Reset();
  if ( v4->pActorBuffs[4].uExpireTime > 0 )
    v4->pActorBuffs[4].Reset();
  if ( stunRegardlessOfState
    || (v4->uAIState != Stunned
    && v4->uAIState != AttackingRanged1
    && v4->uAIState != AttackingRanged2
    && v4->uAIState != AttackingRanged3
    && v4->uAIState != AttackingRanged4
    && v4->uAIState != AttackingMelee))
  {
    v10 = Actor::GetDirectionInfo(PID(OBJECT_Actor,uActorID), edx0, &a3, 0), sizeof(v10);
    v4->uYawAngle = LOWORD(v10->uYawAngle);
    v7 = pSpriteFrameTable->pSpriteSFrames[v4->pSpriteIDs[ANIM_GotHit]].uAnimLength;
    v4->uCurrentActionTime = 0;
    v4->uAIState = Stunned;
    v4->uCurrentActionLength = 8 * v7;
    Actor::PlaySound(uActorID, 2u);
    v4->UpdateAnimation();
  }
}

//----- (00402F87) --------------------------------------------------------
void Actor::AI_Bored(unsigned int uActorID, unsigned int uObjID, AIDirection *a4)
{
  unsigned int v7; // eax@3
  unsigned int v9; // eax@3
  
  Actor* actor = &pActors[uActorID];
  
  AIDirection a3; // [sp+Ch] [bp-5Ch]@2
  if (!a4)
    a4 = Actor::GetDirectionInfo(PID(OBJECT_Actor,uActorID), uObjID, &a3, 0);

  actor->uCurrentActionLength = 8 * pSpriteFrameTable->pSpriteSFrames[actor->pSpriteIDs[ANIM_Bored]].uAnimLength;

  v7 = stru_5C6E00->Atan2(actor->vPosition.x - pGame->pIndoorCameraD3D->vPartyPos.x, actor->vPosition.y - pGame->pIndoorCameraD3D->vPartyPos.y);
  v9 = stru_5C6E00->uIntegerPi + actor->uYawAngle + ((signed int)stru_5C6E00->uIntegerPi >> 3) - v7;

  if ( BYTE1(v9) & 7 )      // turned away - just stand
    Actor::AI_Stand(uActorID, uObjID, actor->uCurrentActionLength, a4);
  else                      // facing player - play bored anim
  {
    actor->uAIState = Fidgeting;
    actor->uCurrentActionTime = 0;
    actor->uYawAngle = a4->uYawAngle;
    actor->vVelocity.z = 0;
    actor->vVelocity.y = 0;
    actor->vVelocity.x = 0;
    if ( rand() % 100 < 5 )
      Actor::PlaySound(uActorID, 3);
    actor->UpdateAnimation();
  }
}

//----- (00402F27) --------------------------------------------------------
void Actor::Resurrect(unsigned int uActorID)
{
  Actor *pActor; // esi@1

  pActor = &pActors[uActorID];
  pActor->uCurrentActionTime = 0;
  pActor->uAIState = Resurrected;
  pActor->uCurrentActionAnimation = ANIM_Dying;
  pActor->uCurrentActionLength = 8 * pSpriteFrameTable->pSpriteSFrames[pActor->pSpriteIDs[ANIM_Dying]].uAnimLength;
  pActor->sCurrentHP = LOWORD(pActor->pMonsterInfo.uHP);
  Actor::PlaySound(uActorID, 1u);
  pActor->UpdateAnimation();
}

//----- (00402D6E) --------------------------------------------------------
void Actor::Die(unsigned int uActorID)
{
  auto actor = &pActors[uActorID];

  actor->uCurrentActionTime = 0;
  actor->uAIState = Dying;
  actor->uCurrentActionAnimation = ANIM_Dying;
  actor->sCurrentHP = 0;
  actor->uCurrentActionLength = 8 * pSpriteFrameTable->pSpriteSFrames[actor->pSpriteIDs[ANIM_Dying]].uAnimLength;
  actor->pActorBuffs[6].Reset();
  actor->pActorBuffs[5].Reset();
  Actor::PlaySound(uActorID, 1);
  actor->UpdateAnimation();

  for (uint i = 0; i < 5; ++i)
    if (pParty->monster_id_for_hunting[i] == actor->pMonsterInfo.uID)
      pParty->monster_for_hunting_killed[i] = true;

  for (uint i = 0; i < 22; ++i)
    actor->pActorBuffs[i].Reset();

  ItemGen drop;
  drop.Reset();
  switch (actor->pMonsterInfo.uID)
  {
    case MONSTER_HARPY_1: case MONSTER_HARPY_2: case MONSTER_HARPY_3:
      drop.uItemID = ITEM_HARPY_FEATHER;
    break;

    case MONSTER_OOZE_1: case MONSTER_OOZE_2: case MONSTER_OOZE_3:
      drop.uItemID = ITEM_OOZE_ECTOPLASM_BOTTLE;
    break;

    case MONSTER_TROLL_1: case MONSTER_TROLL_2: case MONSTER_TROLL_3:
      drop.uItemID = ITEM_TROLL_BLOOD;
    break;

    case MONSTER_DEVIL_1: case MONSTER_DEVIL_2: case MONSTER_DEVIL_3:
      drop.uItemID = ITEM_DEVIL_ICHOR;
    break;

    case MONSTER_DRAGON_1: case MONSTER_DRAGON_2: case MONSTER_DRAGON_3:
      drop.uItemID = ITEM_DRAGON_EYE;
    break;
  }

  if (rand() % 100 < 20 && drop.uItemID != 0)
  {
    sub_42F7EB_DropItemAt(pItemsTable->pItems[drop.uItemID].uSpriteID,
      actor->vPosition.x,
      actor->vPosition.y,
      actor->vPosition.z + 16,
      rand() % 200 + 200,
      1,
      1,
      0,
      &drop);
  }

  if (actor->pMonsterInfo.uSpecialAbilityType == MONSTER_SPECIAL_ABILITY_EXPLODE)
    Actor::Explode(uActorID);
}

//----- (00402CED) --------------------------------------------------------
void Actor::PlaySound(unsigned int uActorID, unsigned int uSoundID)
{
  unsigned __int16 v3; // dx@1

  v3 = pActors[uActorID].pSoundSampleIDs[uSoundID];
  if ( v3 )
  {
    if ( pActors[uActorID].pActorBuffs[3].uExpireTime <= 0 )
    {
      pAudioPlayer->PlaySound((SoundID)v3, PID(OBJECT_Actor, uActorID), 0, -1, 0, 0, 0, 0);
    }
    else
    {
      switch(pActors[uActorID].pActorBuffs[3].uPower)
      {
        case 1: 
          pAudioPlayer->PlaySound((SoundID)v3, PID(OBJECT_Actor, uActorID), 0, 0, 0, 0, 0, 33075);
          break;
        case 2: 
          pAudioPlayer->PlaySound((SoundID)v3, PID(OBJECT_Actor, uActorID), 0, 0, 0, 0, 0, 33075);
          break;
        case 3: 
        case 4: 
          pAudioPlayer->PlaySound((SoundID)v3, PID(OBJECT_Actor, uActorID), 0, 0, 0, 0, 0, 33075);
          break;
        default:
          pAudioPlayer->PlaySound((SoundID)v3, PID(OBJECT_Actor, uActorID), 0, -1, 0, 0, 0, 0);
          break;
      }
    }
  }
}

//----- (00402AD7) --------------------------------------------------------
void Actor::AI_Pursue1(unsigned int uActorID, unsigned int a2, signed int arg0, signed int uActionLength, AIDirection *pDir)
{
  int v6; // eax@1
  Actor *v7; // ebx@1
  unsigned int v8; // ecx@1
  AIDirection *v10; // esi@6
  AIDirection a3; // [sp+Ch] [bp-5Ch]@7
  unsigned int v18; // [sp+64h] [bp-4h]@1

  v6 = 0;
  v7 = &pActors[uActorID];
  v8 = PID(OBJECT_Actor,uActorID);
  if ( v7->pMonsterInfo.uFlying != 0 && !pParty->bFlying )                //TODO: Does v6 have a point?
  {
    if ( v7->pMonsterInfo.uMissleAttack1Type )
      v6 = v7->uActorRadius + 512;
    else
      v6 = pParty->uPartyHeight;
  }

  if ( pDir == nullptr )
  {
    v10 = Actor::GetDirectionInfo(v8, a2, &a3, v6);
  }
  else
  {
    v10 = pDir;
  }
  if ( MonsterStats::BelongsToSupertype(v7->pMonsterInfo.uID, MONSTER_SUPERTYPE_TREANT) )
  {
    if ( !uActionLength )
      uActionLength = 256;
    Actor::AI_StandOrBored(uActorID, 4, uActionLength, v10);
    return;
  }
  if ( v10->uDistance < 307.2 )
  {
    if ( !uActionLength )
      uActionLength = 256;
    Actor::AI_Stand(uActorID, a2, uActionLength, v10);
    return;
  }
  if ( v7->uMovementSpeed == 0 )
  {
    Actor::AI_Stand(uActorID, a2, uActionLength, v10);
    return;
  }
  if ( arg0 % 2 )
    v18 = -16;
  else
    v18 = 16;

  v7->uYawAngle = stru_5C6E00->Atan2(
                    pParty->vPosition.x + (int)fixpoint_mul(stru_5C6E00->Cos(v18 + stru_5C6E00->uIntegerPi + v10->uYawAngle), v10->uDistanceXZ) - v7->vPosition.x,
                    pParty->vPosition.y + (int)fixpoint_mul(stru_5C6E00->Sin(v18 + stru_5C6E00->uIntegerPi + v10->uYawAngle), v10->uDistanceXZ) - v7->vPosition.y);
  if ( uActionLength )
    v7->uCurrentActionLength = uActionLength;
  else
    v7->uCurrentActionLength = 128;
  v7->uPitchAngle = LOWORD(v10->uPitchAngle);
  v7->uAIState = Pursuing;
  v7->UpdateAnimation();
}

//----- (00402968) --------------------------------------------------------
void Actor::AI_Flee(unsigned int uActorID, signed int sTargetPid, int uActionLength, AIDirection *a4)
{
  Actor *v5; // ebx@1
  int v7; // ecx@2
  unsigned __int16 v9; // ax@15
  AIDirection* v10 = nullptr; // [sp+8h] [bp-7Ch]@4
  AIDirection* a3 = nullptr; // [sp+24h] [bp-60h]@3
  AIDirection* v13; // [sp+5Ch] [bp-28h]@4

  v5 = &pActors[uActorID];
  if ( v5->CanAct() )
  {
    v7 = PID(OBJECT_Actor,uActorID);
    if ( !a4 )
    {
      a4 = Actor::GetDirectionInfo(v7, sTargetPid, a3, v5->pMonsterInfo.uFlying);
    }
    v13 = Actor::GetDirectionInfo(v7, 4u, v10, 0);
    if ( MonsterStats::BelongsToSupertype(v5->pMonsterInfo.uID, MONSTER_SUPERTYPE_TREANT)
      || PID_TYPE(sTargetPid) == OBJECT_Actor && v13->uDistance < 307.2 )
    {
      if ( !uActionLength )
        uActionLength = 256;
      Actor::AI_StandOrBored(uActorID, 4, uActionLength, v13);
    }
    else
    {
      if ( v5->uMovementSpeed )
        v5->uCurrentActionLength = (signed int)(a4->uDistanceXZ << 7) / v5->uMovementSpeed;
      else
        v5->uCurrentActionLength = 0;
      if ( v5->uCurrentActionLength > 256 )
        v5->uCurrentActionLength = 256;
      v5->uYawAngle = LOWORD(stru_5C6E00->uIntegerHalfPi) + LOWORD(a4->uYawAngle);
      v5->uYawAngle = LOWORD(stru_5C6E00->uDoublePiMask) & (v5->uYawAngle + rand() % (signed int)stru_5C6E00->uIntegerPi);
      v9 = LOWORD(a4->uPitchAngle);
      v5->uCurrentActionTime = 0;
      v5->uPitchAngle = v9;
      v5->uAIState = Fleeing;
      v5->UpdateAnimation();
    }
  }
}

//----- (0040281C) --------------------------------------------------------
void Actor::AI_Pursue2(unsigned int uActorID, unsigned int a2, signed int uActionLength, AIDirection *pDir, int a5)
{
  int v6; // eax@1
  Actor *v7; // ebx@1
  unsigned int v8; // ecx@1
  AIDirection *v10; // esi@7
  signed __int16 v13; // cx@19
  unsigned __int16 v14; // ax@25
  AIDirection a3; // [sp+Ch] [bp-40h]@8
  AIDirection v18; // [sp+28h] [bp-24h]@8

  v6 = 0;
  v7 = &pActors[uActorID];
  v8 = PID(OBJECT_Actor,uActorID);
  if ( v7->pMonsterInfo.uFlying != 0 && !pParty->bFlying )
  {
    if ( v7->pMonsterInfo.uMissleAttack1Type && uCurrentlyLoadedLevelType == LEVEL_Outdoor )
      v6 = v7->uActorRadius + 512;
    else
      v6 = pParty->uPartyHeight;
  }
  v10 = pDir;
  if ( !pDir )
  {
    v10 = Actor::GetDirectionInfo(v8, a2, &a3, v6);
  }
  if ( MonsterStats::BelongsToSupertype(v7->pMonsterInfo.uID, MONSTER_SUPERTYPE_TREANT) )
  {
    if ( !uActionLength )
      uActionLength = 256;
    Actor::AI_StandOrBored(uActorID, 4, uActionLength, v10);
    return;
  }
  if ( (signed int)v10->uDistance < a5 )
  {
    if ( !uActionLength )
      uActionLength = 256;
    Actor::AI_StandOrBored(uActorID, a2, uActionLength, v10);
    return;
  }
  if ( uActionLength )
  {
    v7->uCurrentActionLength = uActionLength;
  }
  else
  {
    v13 = v7->uMovementSpeed;
    if ( v13 )
      v7->uCurrentActionLength = (signed int)(v10->uDistanceXZ << 7) / v13;
    else
      v7->uCurrentActionLength = 0;
    if ( v7->uCurrentActionLength > 32 )
      v7->uCurrentActionLength = 32;
  }
  v7->uYawAngle = LOWORD(v10->uYawAngle);
  v14 = LOWORD(v10->uPitchAngle);
  v7->uCurrentActionTime = 0;
  v7->uPitchAngle = v14;
  v7->uAIState = Pursuing;
  v7->UpdateAnimation();
}

//----- (00402686) --------------------------------------------------------
void Actor::AI_Pursue3(unsigned int uActorID, unsigned int a2, signed int uActionLength, AIDirection *a4)
{
  int v5; // eax@1
  Actor *v6; // ebx@1
  int v7; // ecx@1
  signed __int16 v12; // cx@19
  __int16 v14; // ax@25
  unsigned __int16 v16; // ax@28
  AIDirection a3; // [sp+Ch] [bp-40h]@8
  AIDirection* v20; // [sp+28h] [bp-24h]@8

  v5 = 0;
  v6 = &pActors[uActorID];
  v7 = PID(OBJECT_Actor,uActorID);
  if ( v6->pMonsterInfo.uFlying != 0 && !pParty->bFlying )
  {
    if ( v6->pMonsterInfo.uMissleAttack1Type && uCurrentlyLoadedLevelType == LEVEL_Outdoor )
      v5 = v6->uActorRadius + 512;
    else
      v5 = pParty->uPartyHeight;
  }
  if ( !a4 )
  {
    v20 = Actor::GetDirectionInfo(v7, a2, &a3, v5);
  }
  if ( MonsterStats::BelongsToSupertype(v6->pMonsterInfo.uID, MONSTER_SUPERTYPE_TREANT) )
  {
    if ( !uActionLength )
      uActionLength = 256;
    return Actor::AI_StandOrBored(uActorID, 4, uActionLength, a4);
  }
  if ( a4->uDistance < 307.2 )
  {
    if ( !uActionLength )
      uActionLength = 256;
    return Actor::AI_StandOrBored(uActorID, a2, uActionLength, a4);
  }
  if ( uActionLength )
  {
    v6->uCurrentActionLength = uActionLength + rand() % uActionLength;
  }
  else
  {
    v12 = v6->uMovementSpeed;
    if ( v12 )
      v6->uCurrentActionLength = (signed int)(a4->uDistanceXZ << 7) / v12;
    else
      v6->uCurrentActionLength = 0;
    if ( v6->uCurrentActionLength > 128 )
      v6->uCurrentActionLength = 128;
  }
  v14 = LOWORD(a4->uYawAngle);
  if ( rand() % 2 )
    v14 += 256;
  else
    v14 -= 256;
  v6->uYawAngle = v14;
  v16 = LOWORD(a4->uPitchAngle);
  v6->uCurrentActionTime = 0;
  v6->uPitchAngle = v16;
  v6->uAIState = Pursuing;
  if ( rand() % 100 < 2 )
    Actor::PlaySound(uActorID, 2u);
  v6->UpdateAnimation();
}

//----- (00401221) --------------------------------------------------------
void Actor::_SelectTarget(unsigned int uActorID, int *a2, bool can_target_party)
{
  int v5; // ecx@1
  signed int v10; // eax@13
  uint v11; // ebx@16
  uint v12; // eax@16
  signed int v14; // eax@31
  uint v15; // edi@43
  uint v16; // ebx@45
  uint v17; // eax@45
  signed int closestId; // [sp+14h] [bp-1Ch]@1
  uint v23; // [sp+1Ch] [bp-14h]@16
  unsigned int lowestRadius; // [sp+24h] [bp-Ch]@1
  uint v27; // [sp+2Ch] [bp-4h]@16
  uint v28; // [sp+2Ch] [bp-4h]@45

  lowestRadius = UINT_MAX;
  v5 = 0;
  *a2 = 0;
  closestId = 0;
  assert(uActorID < uNumActors);
  Actor* thisActor = &pActors[uActorID];

  for (uint i = 0; i < uNumActors; ++i)
  {
    Actor* actor = &pActors[i];
    if (actor->uAIState == Dead || actor->uAIState == Dying ||
        actor->uAIState == Removed || actor->uAIState == Summoned || actor->uAIState == Disabled || uActorID == i )
      continue;

		if (thisActor->uLastCharacterIDToHit == 0 || PID(OBJECT_Actor,v5) != thisActor->uLastCharacterIDToHit )
		{
		  v10 = thisActor->GetActorsRelation(actor);
		  if ( v10 == 0 )
			continue;
		}
		else if (thisActor->IsNotAlive())
		{
		  thisActor->uLastCharacterIDToHit = 0;
		  v10 = thisActor->GetActorsRelation(actor);
		  if ( v10 == 0 )
			continue;
		}
		else
		{
			if ( (actor->uGroup != 0 || thisActor->uGroup != 0) && actor->uGroup == thisActor->uGroup )
				continue;
			v10 = 4;
		}
		if ( thisActor->pMonsterInfo.uHostilityType )
		  v10 = pMonsterStats->pInfos[thisActor->pMonsterInfo.uID].uHostilityType;
		v11 = dword_4DF380_hostilityRanges[v10];
		v23 = abs(thisActor->vPosition.x - actor->vPosition.x);
		v27 = abs(thisActor->vPosition.y - actor->vPosition.y);
		v12 = abs(thisActor->vPosition.z - actor->vPosition.z);
		if ( v23 <= v11
		  && v27 <= v11
		  && v12 <= v11
		  && sub_4070EF_prolly_detect_player(PID(OBJECT_Actor, i), PID(OBJECT_Actor, uActorID))
		  && v23 * v23 + v27 * v27 + v12 * v12 < lowestRadius )
		{
		  lowestRadius = v23 * v23 + v27 * v27 + v12 * v12;
		  closestId = i;
		}
  }

  if ( lowestRadius != UINT_MAX )
  {
    *a2 = PID(OBJECT_Actor, closestId);
  }
  
  if (can_target_party && !pParty->Invisible())
  {
    if ( thisActor->uAttributes & 0x80000
      && thisActor->pActorBuffs[12].uExpireTime <= 0
      && thisActor->pActorBuffs[1].uExpireTime <= 0
      && thisActor->pActorBuffs[2].uExpireTime <= 0 )
      v14 = 4;
    else
      v14 = thisActor->GetActorsRelation(0);
    if ( v14 != 0 )
    {
      if ( !thisActor->pMonsterInfo.uHostilityType )
        v15 = dword_4DF380_hostilityRanges[v14];
      else
        v15 = dword_4DF380_hostilityRanges[4];
      v16 = abs(thisActor->vPosition.x - pParty->vPosition.x);
      v28 = abs(thisActor->vPosition.y - pParty->vPosition.y);
      v17 = abs(thisActor->vPosition.z - pParty->vPosition.z);
      if ( v16 <= v15 && v28 <= v15 && v17 <= v15 && (v16 * v16 + v28 * v28 + v17 * v17 < lowestRadius))
      {
        *a2 = OBJECT_Player;
      }
    }
  }
}
// 4DF380: using guessed type int dword_4DF380[];
// 4DF390: using guessed type int dword_4DF390;

//----- (0040104C) --------------------------------------------------------
signed int Actor::GetActorsRelation(Actor *otherActPtr)
{
  unsigned int v5; // edx@15
  unsigned int v6; // eax@16
  unsigned int thisGroup; // ebp@19
  int otherGroup; // eax@22
  unsigned int thisAlly; // edx@25
  unsigned int otherAlly; // edx@33

  if ( otherActPtr)
  {
    v5 = otherActPtr->uGroup;
    v6 = this->uGroup;
    if ( v5 != 0 && v6 != 0 && v5 == v6 )
      return 0;
  }

  if (this->pActorBuffs[ACTOR_BUFF_BERSERK].uExpireTime > 0)
    return 4;
  thisAlly = this->uAlly;
  if ( this->pActorBuffs[ACTOR_BUFF_ENSLAVED].uExpireTime > 0 || thisAlly == 9999)
    thisGroup = 0;
  else if ( thisAlly > 0 )
  {
    thisGroup = thisAlly;
  }
  else
  {
    thisGroup = (this->pMonsterInfo.uID - 1) / 3 + 1;
  }

  if ( otherActPtr )
  {
    if (otherActPtr->pActorBuffs[ACTOR_BUFF_BERSERK].uExpireTime > 0)
      return 4;
    otherAlly = otherActPtr->uAlly;
    if ( otherActPtr->pActorBuffs[ACTOR_BUFF_ENSLAVED].uExpireTime > 0 || otherAlly == 9999)
      otherGroup = 0;
    else  if ( otherAlly > 0 )
    {
      otherGroup = otherAlly;
    }
    else
    {
      otherGroup = (otherActPtr->pMonsterInfo.uID - 1) / 3 + 1;
    }
  }
  else
  {
    otherGroup = 0;
  }

  if ( this->pActorBuffs[ACTOR_BUFF_CHARM].uExpireTime > 0 && !otherGroup
    || otherActPtr && otherActPtr->pActorBuffs[ACTOR_BUFF_CHARM].uExpireTime > 0 && !thisGroup )
    return 0;
  if ( this->pActorBuffs[ACTOR_BUFF_ENSLAVED].uExpireTime <= 0 && this->uAttributes & 0x80000 && !otherGroup )
    return 4;
  if (thisGroup >= 89 || otherGroup >= 89)
    return 0;

  if ( thisGroup == 0  )
  {
    if ( (!otherActPtr || this->pActorBuffs[ACTOR_BUFF_ENSLAVED].uExpireTime > 0 && !(otherActPtr->uAttributes & 0x80000)) && !pFactionTable->relations[otherGroup][0])
      return pFactionTable->relations[0][otherGroup];
    else
      return 4;
  }
  else
  {
    return pFactionTable->relations[thisGroup][otherGroup];
  }
}

//----- (0045976D) --------------------------------------------------------
void Actor::UpdateAnimation()
{
  //AIState state; // edx@1
  //unsigned int result; // eax@1

  //state = (AIState)this->;
  uAttributes &= 0xFFDFFFFF;
  //result = this->uAttributes;
  switch (uAIState)
  {
    case Tethered:
      uCurrentActionAnimation = ANIM_Walking;
    break;

    case AttackingMelee:
      uCurrentActionAnimation = ANIM_AtkMelee;
      uAttributes |= 0x200000u;
    break;

    case AttackingRanged1:
    case AttackingRanged2:
    case AttackingRanged3:
    case AttackingRanged4:
      uCurrentActionAnimation = ANIM_AtkRanged;
      uAttributes |= 0x200000u;
    break;

    case Dying:
    case Resurrected:
      uCurrentActionAnimation = ANIM_Dying;
      uAttributes |= 0x200000u;
    break;

    case Pursuing:
    case Fleeing:
      uCurrentActionAnimation = ANIM_Walking;
      uAttributes |= 0x200000u;
    break;

    case Stunned:
      uCurrentActionAnimation = ANIM_GotHit;
      uAttributes |= 0x200000u;
    break;

    case Fidgeting:
      uCurrentActionAnimation = ANIM_Bored;
      uAttributes |= 0x200000u;
    break;

    case Standing:
    case Interacting:
    case Summoned:
      uCurrentActionAnimation = ANIM_Standing;
      uAttributes |= 0x200000u;
    break;

    case Dead:
      if (pSpriteFrameTable->pSpriteSFrames[pSpriteIDs[ANIM_Dead]].pHwSpriteIDs[0] <= 0)
        uAIState = Removed;
      else
        uCurrentActionAnimation = ANIM_Dead;
    break;

    case Removed:
    case Disabled:
      return;

    default:
      assert(false);
  }
}

//----- (00459671) --------------------------------------------------------
void Actor::Reset()
{
  this->pActorName[0] = 0;
  this->word_000086_some_monster_id = 0;
  this->sNPC_ID = 0;
  this->vPosition.z = 0;
  this->vPosition.y = 0;
  this->vPosition.x = 0;
  this->vVelocity.z = 0;
  this->vVelocity.y = 0;
  this->vVelocity.x = 0;
  this->uYawAngle = 0;
  this->uPitchAngle = 0;
  this->uAttributes = 0;
  this->uSectorID = 0;
  this->uCurrentActionTime = 0;
  this->vInitialPosition.z = 0;
  this->vInitialPosition.y = 0;
  this->vInitialPosition.x = 0;
  this->vGuardingPosition.z = 0;
  this->vGuardingPosition.y = 0;
  this->vGuardingPosition.x = 0;
  this->uTetherDistance = 256;
  this->uActorRadius = 32;
  this->uActorHeight = 128;
  this->uAIState = Standing;
  this->uCurrentActionAnimation = ANIM_Standing;
  this->uMovementSpeed = 200;
  this->uCarriedItemID = 0;
  this->uGroup = 0;
  this->uAlly = 0;
  this->uSummonerID = 0;
  this->uLastCharacterIDToHit = 0;
  this->dword_000334_unique_name = 0;
  memset(this->pSpriteIDs, 0, sizeof(pSpriteIDs));
  memset(this->pActorBuffs, 0, 0x160u);
}

//----- (0045959A) --------------------------------------------------------
void Actor::PrepareSprites(char load_sounds_if_bit1_set)
{
  
  MonsterDesc *v3; // esi@1 
  MonsterInfo *v9; // [sp+84h] [bp-10h]@1
 
  v3 = &pMonsterList->pMonsters[pMonsterInfo.uID - 1];
  v9 = &pMonsterStats->pInfos[pMonsterInfo.uID - 1 + 1];
  //v12 = pSpriteIDs;
  //Source = (char *)v3->pSpriteNames;
  //do
  for (uint i = 0; i < 8; ++i)
  {
    //strcpy(pSpriteName, v3->pSpriteNames[i]);
    pSpriteIDs[i] = pSpriteFrameTable->FastFindSprite(v3->pSpriteNames[i]);
    pSpriteFrameTable->InitializeSprite(pSpriteIDs[i]);
  }
  uActorHeight = v3->uMonsterHeight;
  uActorRadius = v3->uMonsterRadius;
  uMovementSpeed = v9->uBaseSpeed;
  if ( !(load_sounds_if_bit1_set & 1) )
  {
    for (int i=0;i<4;++i )
       pSoundSampleIDs[i]=v3->pSoundSampleIDs[i];
  }
}

//----- (00459667) --------------------------------------------------------
void Actor::Remove()
{
  this->uAIState = Removed;
}

//----- (0044FD29) --------------------------------------------------------
void Actor::SummonMinion( int summonerId )
{
  unsigned __int8 extraSummonLevel; // al@1
  int summonMonsterBaseType; // esi@1
  int v5; // edx@2
  int v7; // edi@10
  Actor *v8; // esi@10
  MonsterInfo *v9; // ebx@10
  MonsterDesc *v10; // edi@10
  int v13; // ebx@10
  int v15; // edi@10
  int v17; // ebx@10
  unsigned int v19; // qax@10
  int result; // eax@13
  unsigned int monsterId; // [sp+10h] [bp-18h]@8
  int v27; // [sp+18h] [bp-10h]@10
  int actorSector; // [sp+1Ch] [bp-Ch]@8


  actorSector = 0;
  if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
    actorSector = pIndoor->GetSector(this->vPosition.x, this->vPosition.y, this->vPosition.z);

  v19 = this->uAlly;
  if ( !v19 )
  {
    monsterId = this->pMonsterInfo.uID - 1;
    v19 = (uint)(monsterId * 0.33333334);
  }
  v27 = uCurrentlyLoadedLevelType == LEVEL_Outdoor ? 128 : 64;
  v13 = rand() % 2048;
  v15 = ((stru_5C6E00->Cos(v13) * (signed __int64)v27) >> 16) + this->vPosition.x;
  v17 = ((stru_5C6E00->Sin(v13) * (signed __int64)v27) >> 16) + this->vPosition.y;

  if (uCurrentlyLoadedLevelType != LEVEL_Outdoor)
  {
    result = pIndoor->GetSector(v15, v17, this->vPosition.z);
    if (result != actorSector)
      return;
    result = BLV_GetFloorLevel(v15, v17, v27, result, &monsterId);
    if (result != -30000)
      return;
    if (abs(result - v27) > 1024)
      return;
  }

  extraSummonLevel = this->pMonsterInfo.uSpecialAbilityDamageDiceRolls;
  summonMonsterBaseType = this->pMonsterInfo.field_3C_some_special_attack;
  if ( extraSummonLevel )
  {
    if ( extraSummonLevel >= 1u && extraSummonLevel <= 3u )
      summonMonsterBaseType = summonMonsterBaseType + extraSummonLevel - 1;
  }
  else
  {
    v5 = rand() % 100;
    if ( v5 >= 90 )
      summonMonsterBaseType += 2;
    else if ( v5 >= 60 )
    {
      summonMonsterBaseType += 1;
    }
  }
  v7 = summonMonsterBaseType - 1;
  v8 = &pActors[uNumActors];
  v9 = &pMonsterStats->pInfos[v7 + 1];
  pActors[uNumActors].Reset();
  v10 = &pMonsterList->pMonsters[v7];
  strcpy(v8->pActorName, v9->pName);
  v8->sCurrentHP = LOWORD(v9->uHP);
  memcpy(&v8->pMonsterInfo, v9, sizeof(v8->pMonsterInfo));
  v8->word_000086_some_monster_id = summonMonsterBaseType;
  v8->uActorRadius = v10->uMonsterRadius;
  v8->uActorHeight = v10->uMonsterHeight;
  v8->pMonsterInfo.uTreasureDiceRolls = 0;
  v8->pMonsterInfo.uTreasureType = 0;
  v8->pMonsterInfo.uExp = 0;
  v8->uMovementSpeed = v10->uMovementSpeed;
  v8->vInitialPosition.z = this->vPosition.z;
  v8->vPosition.z = this->vPosition.z;
  v8->vInitialPosition.x = v15;
  v8->vPosition.x = v15;
  v8->vInitialPosition.y = v17;
  v8->vPosition.y = v17;
  v8->uTetherDistance = 256;
  v8->uSectorID = actorSector;
  v8->PrepareSprites(0);
  v8->pMonsterInfo.uHostilityType = MonsterInfo::Hostility_Friendly;
  v8->uAlly = v19;
  v8->uCurrentActionTime = 0;
  v8->uGroup = this->uGroup;
  v8->uAIState = Summoned;
  v8->uCurrentActionLength = 256;
  v8->UpdateAnimation();

  ++uNumActors;
  ++this->pMonsterInfo.uSpecialAbilityDamageDiceBonus;
  if ( this->uAttributes & 0x80000 )
    v8->uAttributes |= 0x80000u;
  v8->uSummonerID = PID(OBJECT_Actor,summonerId);

}
// 46DF1A: using guessed type int __fastcall 46DF1A_collide_against_actor(int, int);
//----- (0046DF1A) --------------------------------------------------------
bool Actor::_46DF1A_collide_against_actor( int a1, int a2 )
{
  Actor *v2; // edi@1
  unsigned __int16 v3; // ax@1
  int v4; // esi@6
  int v8; // ecx@14
  int v9; // eax@14
  int v10; // ebx@14
  int v11; // esi@14
  int v12; // ebx@15
  int v13; // ebx@17

  v2 = &pActors[a1];
  v3 = v2->uAIState;
  if ( v3 == Removed || v3 == Dying || v3 == Disabled || v3 == Dead || v3 == Summoned )
    return 0;
  v4 = v2->uActorRadius;
  if ( a2 )
    v4 = a2;

  if (stru_721530.sMaxX > v2->vPosition.x + v4 || 
      stru_721530.sMinX < v2->vPosition.x - v4 || 
      stru_721530.sMaxY > v2->vPosition.y + v4 || 
      stru_721530.sMinY < v2->vPosition.y - v4 ||
      stru_721530.sMaxZ > v2->vPosition.z + v2->uActorHeight ||
      stru_721530.sMinZ < v2->vPosition.z)
  {
    return false;
  }
  v8 = v2->vPosition.x - stru_721530.normal.x;
  v9 = v2->vPosition.y - stru_721530.normal.y;
  v10 = stru_721530.prolly_normal_d + v4;
  v11 = (v8 * stru_721530.direction.y - v9 * stru_721530.direction.x) >> 16;
  v12 = (v8 * stru_721530.direction.x + v9 * stru_721530.direction.y) >> 16;
  if ( abs(v11) > v10 || v12 <= 0)
  {
    return false;
  }
  if (((stru_721530.direction.z * (signed __int64)v12) >> 16) + stru_721530.normal.z < v2->vPosition.z)
  {
    return false;
  }

  v13 = v12 - integer_sqrt(v10 * v10 - v11 * v11);
  if ( v13 < 0 )
    v13 = 0;
  if ( v13 < stru_721530.field_7C )
  {
    stru_721530.field_7C = v13;
    stru_721530.uFaceID = PID(OBJECT_Actor,a1);
  }
  return true;
}
//----- (00401A91) --------------------------------------------------------
void  UpdateActorAI()
{
	//unsigned int v0; // esi@4
	int v1; // eax@7
	//int v2; // ecx@7
	//int v3; // eax@7
	signed int v4; // edi@10
	Actor *v5; // esi@12
	signed int sDmg; // eax@14
	__int16 v7; // cx@14
	//Player **v8; // esi@20
	Player *pPlayer; // ecx@21
	Actor *pActor; // esi@34
	//__int16 v11; // ax@34
	//unsigned int v12; // eax@47
	//signed int v13; // edi@47
	//SpellBuff *v14; // ebx@47
	//unsigned int v15; // edi@67
	//char *v16; // eax@67
	//unsigned int v17; // edx@67
	//unsigned int v18; // ecx@67
	//unsigned __int16 v19; // ax@72
	//int *v20; // esi@80
	//Actor *v21; // ebx@80
	unsigned __int16 v22; // ax@86
	//signed int v23; // eax@94
	//unsigned int v24; // eax@102
	//signed int v25; // edi@102
	//SpellBuff *v26; // esi@102
	unsigned int v27; // ecx@123
	unsigned int v28; // eax@123
	//unsigned int v29; // eax@127
	AIDirection *v30; // eax@129
	unsigned __int16 v31; // ax@132
	//unsigned int v32; // esi@142
	int v33; // eax@144
	int v34; // eax@147
	char v35; // al@150
	unsigned int v36; // edi@152
	signed int v37; // eax@154
	//unsigned __int8 v38; // sf@158
	//unsigned __int8 v39; // of@158
	//signed int v40; // edx@166
	//unsigned int v41; // ecx@166
	double v42; // st7@176
	double v43; // st6@176
	//bool v44; // eax@189
	int v45; // eax@192
	unsigned __int8 v46; // cl@197
	double v47; // st7@206
	//double v48; // st7@207
	//char v49; // zf@208
	//char v50; // zf@214
	//signed int v51; // edx@219
	//unsigned int v52; // ecx@219
	__int16 v53; // fps@224
	//unsigned __int8 v54; // c0@224
	//unsigned __int8 v55; // c3@224
	//double v56; // st7@226
	AIDirection *v57; // eax@246
	double v58; // st7@246
	//signed int v59; // [sp-18h] [bp-C8h]@213
	//int v60; // [sp-14h] [bp-C4h]@144
	//int v61; // [sp-14h] [bp-C4h]@168
	//AIDirection *v62; // [sp-14h] [bp-C4h]@213
	//signed int v63; // [sp-14h] [bp-C4h]@216
	//unsigned int v64; // [sp-14h] [bp-C4h]@219
	unsigned int v65; // [sp-10h] [bp-C0h]@144
	char v66; // [sp-10h] [bp-C0h]@147
	//AIDirection *v67; // [sp-10h] [bp-C0h]@167
	//int v68; // [sp-10h] [bp-C0h]@168
	//AIDirection *v69; // [sp-10h] [bp-C0h]@206
	int v70; // [sp-10h] [bp-C0h]@213
	//AIDirection *v71; // [sp-10h] [bp-C0h]@216
	AIDirection v72; // [sp+0h] [bp-B0h]@246
	AIDirection a3; // [sp+1Ch] [bp-94h]@129
	AIDirection v74; // [sp+38h] [bp-78h]@246
	AIDirection v75; // [sp+54h] [bp-5Ch]@129
	int target_pid_type; // [sp+70h] [bp-40h]@83
	signed int a1; // [sp+74h] [bp-3Ch]@129
	int v78; // [sp+78h] [bp-38h]@79
	AIDirection pDir; // [sp+7Ch] [bp-34h]@129
	float v80; // [sp+98h] [bp-18h]@33
	int v81; // [sp+9Ch] [bp-14h]@100
	//int v82; // [sp+A0h] [bp-10h]@45
	//unsigned int uActorID; // [sp+A4h] [bp-Ch]@32
	unsigned int v84; // [sp+A8h] [bp-8h]@11
	signed int target_pid; // [sp+ACh] [bp-4h]@83
	AIState uAIState;
	int v38;
	
	if ( uCurrentlyLoadedLevelType == LEVEL_Outdoor)
		MakeActorAIList_ODM();
	else
		MakeActorAIList_BLV();
	
	//v0 = 0;
	if ( uCurrentlyLoadedLevelType != LEVEL_Indoor && pParty->armageddon_timer > 0 )
	{
		if ( pParty->armageddon_timer > 417 )
		{
			pParty->armageddon_timer = 0;
		}
		else
		{
			pParty->sRotationY = (stru_5C6E00->uIntegerDoublePi - 1) & (pParty->sRotationY + rand() % 16 - 8);
			pParty->sRotationX = pParty->sRotationX + rand() % 16 - 8;
			if ( pParty->sRotationX > 128) 
				pParty->sRotationX = 128;
			else if ( pParty->sRotationX < -128 )
				pParty->sRotationX = -128;
				
			pParty->uFlags |= 2u;
			pParty->armageddon_timer -= pMiscTimer->uTimeElapsed;
			v4 = pParty->field_16140 + 50;
			if ( pParty->armageddon_timer <= 0 )
			{
				pParty->armageddon_timer = 0;
				for(int i = 0; i < uNumActors; i++)
				{
					pActor=&pActors[i];
					if ( pActor->CanAct() )
					{
						sDmg = stru_50C198.CalcMagicalDamageToActor(pActor, 5, v4);
						pActor->sCurrentHP -= stru_50C198.CalcMagicalDamageToActor(pActor, 5, v4);
						if ( sDmg )
						{
							if ( pActor->sCurrentHP >= 0 )
							{
								Actor::AI_Stun(i, 4, 0);
							}
							else
							{
								Actor::Die(i);
								if ( pActor->pMonsterInfo.uExp )
									pParty->GivePartyExp(pMonsterStats->pInfos[pActor->pMonsterInfo.uID].uExp);
							}
						}
					}
				}
				for(int i = 1; i <= 4; i++)
				{
					pPlayer = pPlayers[i];
					if ( !pPlayer->pConditions[14] && !pPlayer->pConditions[15] && !pPlayer->pConditions[16] )
						pPlayer->ReceiveDamage(v4, DMGT_5);
				}
			}
			if (pTurnEngine->pending_actions)
				--pTurnEngine->pending_actions;
		}
	}
	
	if (pParty->bTurnBasedModeOn)
	{
		pTurnEngine->AITurnBasedAction();
		return;
	}
	
	
	//uActorID = v0;
	for (uint i = 0; i < uNumActors; ++i)
	{
		pActor = &pActors[i];
		//LODWORD(v80) = (int)(char *)pActors + 176; // uAIState
		//do
		//{
			//pActor = (Actor *)(LODWORD(v80) - 176);
			//v11 = *(unsigned int *)LODWORD(v80);
			//v49 = *(unsigned int *)LODWORD(v80) == 5;
			ai_near_actors_targets_pid[i] = OBJECT_Player;
			if (pActor->uAIState == Dead || pActor->uAIState == Removed || pActor->uAIState == Disabled || pActor->uAttributes & 0x0400)
				continue;
			
			if (!pActor->sCurrentHP && pActor->uAIState != Dying)
				Actor::Die(i);
			
			//v84 = *(_QWORD *)(LODWORD(v80) + 84) <= 0i64 ? 0 : 1;
			//v82 = *(_QWORD *)(LODWORD(v80) + 52) <= 0i64 ? 0 : 1;
			//v12 = 0;
			//v13 = 0;
			//v14 = (SpellBuff *)(LODWORD(v80) + 36);
			for (uint j = 0; j < 22; ++j)
			{
				if (j != 10)
				pActor->pActorBuffs[j].IsBuffExpiredToTime(pParty->uTimePlayed);
			}
			/*do
			{
				if ( v13 != 10 )
				{
					v14->_4585CA(pParty->uTimePlayed);
					v12 = 0;
				}
				++v13;
				++v14;
			}
			while ( v13 < 22 );*/
			if (pActor->pActorBuffs[ACTOR_BUFF_SHRINK].uExpireTime < 0)
			//&& SHIDWORD(pActor->pActorBuffs[3].uExpireTime) <= (signed int)v12 && (SHIDWORD(pActor->pActorBuffs[3].uExpireTime) < (signed int)v12
			// || LODWORD(pActor->pActorBuffs[3].uExpireTime) <= v12) )
				pActor->uActorHeight = pMonsterList->pMonsters[pActor->pMonsterInfo.uID - 1].uMonsterHeight;
			if (pActor->pActorBuffs[ACTOR_BUFF_CHARM].uExpireTime > 0)
				pActor->pMonsterInfo.uHostilityType = MonsterInfo::Hostility_Friendly;
			// not sure
			else if (pActor->pActorBuffs[ACTOR_BUFF_CHARM].uExpireTime < 0)
				pActor->pMonsterInfo.uHostilityType = pMonsterStats->pInfos[pActor->pMonsterInfo.uID].uHostilityType;
			
			if (pActor->pActorBuffs[ACTOR_BUFF_PARALYZED].uExpireTime > 0 ||
				pActor->pActorBuffs[ACTOR_BUFF_STONED].uExpireTime > 0)
		      continue;
			
			//v15 = pMiscTimer->uTimeElapsed;
			//v16 = (char *)&pActor->pMonsterInfo.uRecoveryTime;
			//v17 = pActor->uCurrentActionTime;
			//v18 = pActor->pMonsterInfo.uRecoveryTime;
			if (pActor->pMonsterInfo.uRecoveryTime)
			{
				if (pActor->pMonsterInfo.uRecoveryTime < pMiscTimer->uTimeElapsed)
					pActor->pMonsterInfo.uRecoveryTime = 0;
				else 
					pActor->pMonsterInfo.uRecoveryTime -= pMiscTimer->uTimeElapsed;
			}
			
			pActor->uCurrentActionTime += pMiscTimer->uTimeElapsed;
			if (pActor->uCurrentActionTime < pActor->uCurrentActionLength)
				continue;
			
			//v19 = actor->uAIState;
			if (pActor->uAIState == Dying)
				pActor->uAIState = Dead;
			else
			{
				if (pActor->uAIState != Summoned)
				{
					Actor::AI_StandOrBored(i, OBJECT_Player, 256, nullptr);
					continue;
				}
				pActor->uAIState = Standing;
			}
			
			pActor->uCurrentActionTime = 0;
			pActor->uCurrentActionLength = 0;
			pActor->UpdateAnimation();
			//LABEL_78:
			//++uActorID;
			//LODWORD(v80) += 836;
		//}
		//while ( (signed int)uActorID < (signed int)uNumActors );
	}
	
	
	v78 = 0;
	int actor_id = -1;
	if ( ai_arrays_size > 0 )
	{
		//while ( 1 )
		for(v78 = 0; v78 < ai_arrays_size; ++v78)
		{
			actor_id = ai_near_actors_ids[v78];
			assert(actor_id < uNumActors);
			
			//v20 = &ai_near_actors_targets_pid[actor_id];
			pActor = &pActors[actor_id];
			Actor::_SelectTarget(actor_id, &ai_near_actors_targets_pid[actor_id], true);
			if (pActor->pMonsterInfo.uHostilityType && !ai_near_actors_targets_pid[actor_id])
				pActor->pMonsterInfo.uHostilityType = MonsterInfo::Hostility_Friendly;
			target_pid = ai_near_actors_targets_pid[actor_id];
			target_pid_type = PID_TYPE(target_pid);
			if ( target_pid_type == OBJECT_Actor)
				v80 = 0.5;
			else
				v80 = 1.0;
			v22 = pActor->uAIState;
			if ( v22 == Dying || v22 == Dead || v22 == Removed || v22 == Disabled || v22 == Summoned)
			{
				continue;
			}
			if ( !pActor->sCurrentHP )
				Actor::Die(actor_id);
			for(int i=0;i<22;i++)
			{
				if ( i != 10 )
				{
					pActor->pActorBuffs[i].IsBuffExpiredToTime(pParty->uTimePlayed);
				}
			}
			if ( (signed __int64)pActor->pActorBuffs[ACTOR_BUFF_SHRINK].uExpireTime < 0 )
				pActor->uActorHeight = pMonsterList->pMonsters[pActor->pMonsterInfo.uID - 1].uMonsterHeight;
			if ( (signed __int64)pActor->pActorBuffs[ACTOR_BUFF_CHARM].uExpireTime > 0 )
				pActor->pMonsterInfo.uHostilityType = MonsterInfo::Hostility_Friendly;
			// not sure
			else if ( (signed __int64)pActor->pActorBuffs[ACTOR_BUFF_CHARM].uExpireTime < 0 )
				pActor->pMonsterInfo.uHostilityType = pMonsterStats->pInfos[pActor->pMonsterInfo.uID].uHostilityType;
			if ( (signed __int64)pActor->pActorBuffs[2].uExpireTime < 0 )
			{
				pActor->uAIState = Removed;
				continue;
			}
			if ( (signed __int64)pActor->pActorBuffs[5].uExpireTime > 0
				|| (signed __int64)pActor->pActorBuffs[6].uExpireTime > 0)
			{
				continue;
			}
			v27 = pMiscTimer->uTimeElapsed;
			v28 = pActor->pMonsterInfo.uRecoveryTime;
			pActor->uCurrentActionTime += pMiscTimer->uTimeElapsed;
			if ( (signed int)v28 > 0 )
				pActor->pMonsterInfo.uRecoveryTime = v28 - v27;
			if ( pActor->pMonsterInfo.uRecoveryTime < 0 )
				pActor->pMonsterInfo.uRecoveryTime = 0;
			if ( !(pActor->uAttributes & 0x8000) )
				pActor->uAttributes |= 0x8000;
			a1 = PID(OBJECT_Actor,actor_id);
			v30 = Actor::GetDirectionInfo(PID(OBJECT_Actor,actor_id), target_pid, &a3, 0);
			memcpy(&v75, v30, sizeof(v75));
			memcpy(&pDir, &v75, sizeof(pDir));
			uAIState = pActor->uAIState; 
			/*if ( v21->pMonsterInfo.uHostilityType != MonsterInfo::Hostility_Friendly
				&& (signed int)v21->pMonsterInfo.uRecoveryTime <= 0
				&& v80 * 307.2 >= (double)(signed int)v75.uDistance
				&& (uAIState == Pursuing || uAIState == Standing || uAIState == Tethered || uAIState == Fidgeting)
				|| ( v21->pMonsterInfo.uMissleAttack1Type && uAIState == Stunned ) )
			{
				v32 = actor_id;
			}
			else
			*/
			if ( pActor->pMonsterInfo.uHostilityType == MonsterInfo::Hostility_Friendly
				|| (signed int)pActor->pMonsterInfo.uRecoveryTime > 0
				|| v80 * 307.2 < (double)(signed int)v75.uDistance
				|| uAIState != Pursuing && uAIState != Standing && uAIState != Tethered && uAIState != Fidgeting
				&&  !pActor->pMonsterInfo.uMissleAttack1Type || uAIState != Stunned )
			{
				if ( (signed int)pActor->uCurrentActionTime < pActor->uCurrentActionLength )
				{
					continue;
				}
				else if ( pActor->uAIState == AttackingMelee )
				{
					v35 = stru_50C198.special_ability_use_check(pActor, actor_id);
					stru_50FE08.Add(
						a1,
						5120,
						pActor->vPosition.x,
						pActor->vPosition.y,
						pActor->vPosition.z + ((signed int)pActor->uActorHeight >> 1),
						v35,
						1
					);
				}
				else if ( pActor->uAIState == AttackingRanged1 )
				{
					v34 = pActor->pMonsterInfo.uMissleAttack1Type;
					Actor::AI_RangedAttack(actor_id, &pDir, v34, 0);
				}
				else if ( pActor->uAIState == AttackingRanged2 )
				{
					v34 = pActor->pMonsterInfo.uMissleAttack2Type;
					Actor::AI_RangedAttack(actor_id, &pDir, v34, 1);
				}
				else if ( pActor->uAIState == AttackingRanged3 )
				{
					v65 = pActor->pMonsterInfo.uSpellSkillAndMastery1;
					v33 = pActor->pMonsterInfo.uSpell1ID;
					Actor::AI_SpellAttack(actor_id, &pDir, v33, 2, v65);
				}
				else if ( pActor->uAIState == AttackingRanged4 )
				{
					v65 = pActor->pMonsterInfo.uSpellSkillAndMastery2;
					v33 = pActor->pMonsterInfo.uSpell2ID;
					Actor::AI_SpellAttack(actor_id, &pDir, v33, 3, v65);
				}
			}

			v36 = v75.uDistance;
			if ( pActor->pMonsterInfo.uHostilityType == MonsterInfo::Hostility_Friendly)
			{
				if ( target_pid_type == OBJECT_Actor )
				{
					v36 = v75.uDistance;
					//v37 = (unsigned __int8)*(&byte_5C8D1A[89 * (pActor->pMonsterInfo.uID - 1) / 3]
					//	+ (pActors[PID_ID(target_pid)].pMonsterInfo.uID - 1) / 3);
                    v37 =pFactionTable->relations[(pActor->pMonsterInfo.uID-1) / 3 + 1][(pActors[PID_ID(target_pid)].pMonsterInfo.uID - 1) / 3 + 1];
				}
				else
				{
					v37 = 4;
				}
				v38=0;
				if ( v37 == 2 )
				{
					//v39 = __OFSUB__(v36, 1024);
					//v38 = ((v36 - 1024) & 0x80000000u) != 0;
					v38 = 1024;
				}
				else if ( v37 == 3 )
				{
					//v39 = __OFSUB__(v36, 2560);
					//v38 = ((v36 - 2560) & 0x80000000u) != 0;
					v38 = 2560;
				}
				else if ( v37 == 4 )
				{
					//v39 = __OFSUB__(v36, 5120);
					//v38 = ((v36 - 5120) & 0x80000000u) != 0;
					v38 = 5120;
				}
				if ( v37 >= 1 && v37 <= 4 && v36 < v38  || v37 == 1 )
					pActor->pMonsterInfo.uHostilityType = MonsterInfo::Hostility_Long;
			}

			if (pActor->pActorBuffs[4].uExpireTime > 0)
			{
				if ( (signed int)v36 >= 10240 )
				{
					Actor::AI_RandomMove(actor_id, target_pid, 1024, 0);
				}
				else
				{
					//peasents after attacked
					//guard after attacked
					Actor::AI_Flee(actor_id, target_pid, 0, &pDir);
				}
				continue;
			}

			if ( pActor->pMonsterInfo.uHostilityType == MonsterInfo::Hostility_Long && target_pid )
			{

				if ( pActor->pMonsterInfo.uAIType == 1 )
				{
					if ( pActor->pMonsterInfo.uMovementType == MONSTER_MOVEMENT_TYPE_STAIONARY )
					{
						Actor::AI_Stand(actor_id, target_pid, pActor->pMonsterInfo.uRecoveryTime * 2.133333333333333,	&pDir);
					}
					else
					{
						Actor::AI_Flee(actor_id, target_pid, 0, &pDir);
						continue;
					}
						
				}
				if ( !(pActor->uAttributes & 0x020000) )
				{
					if ( pActor->pMonsterInfo.uAIType == 2 || pActor->pMonsterInfo.uAIType == 3)
					{
						if ( pActor->pMonsterInfo.uAIType == 2 )
							v43 = (double)(signed int)pActor->pMonsterInfo.uHP * 0.2;
						if ( pActor->pMonsterInfo.uAIType == 3 )
							v43 = (double)(signed int)pActor->pMonsterInfo.uHP * 0.1;
						v84 = pActor->sCurrentHP;
						v42 = (double)(signed int)v84;
						if ( v43 > v42 && (signed int)v36 < 10240 )
						{
							Actor::AI_Flee(actor_id, target_pid, 0, &pDir);
							continue;
						}
					}
				}
				
				v81 = v36 - pActor->uActorRadius;
				if ( target_pid_type == OBJECT_Actor )
					v81 -= pActors[PID_ID(target_pid)].uActorRadius;
				if ( v81 < 0 )
					v81 = 0;
				rand();
				pActor->uAttributes &= 0xFFFBFFFF;
				if ( v81 < 5120 )
				{
					v45 = stru_50C198.special_ability_use_check(pActor, actor_id);
					if ( v45 == 0 )
					{
						if ( pActor->pMonsterInfo.uMissleAttack1Type )
						{
							if ( (signed int)pActor->pMonsterInfo.uRecoveryTime <= 0 )
							{
								Actor::AI_MissileAttack1(actor_id, target_pid, &pDir);
							}
							else if ( pActor->pMonsterInfo.uMovementType == MONSTER_MOVEMENT_TYPE_STAIONARY )
							{
								v47 = pActor->pMonsterInfo.uRecoveryTime * 2.133333333333333;
								Actor::AI_Stand(actor_id, target_pid, v47, &pDir);
							}
							else
							{
								v47 = pActor->pMonsterInfo.uRecoveryTime * 2.133333333333333;
								if ( v80 * 307.2 > (double)v81 )
								{
									Actor::AI_Stand(actor_id, target_pid, v47, &pDir);
								}
								else
								{
									Actor::AI_Pursue1(actor_id, target_pid, actor_id, v47, &pDir);
								}
							}
						}
						else
						{
							if ( (double)v81 >= v80 * 307.2 )
							{
								if ( v81 >= 1024 )
								{
									if ( pActor->pMonsterInfo.uMovementType == MONSTER_MOVEMENT_TYPE_STAIONARY )
									{
										v47 = pActor->pMonsterInfo.uRecoveryTime * 2.133333333333333;
										Actor::AI_Stand(actor_id, target_pid, v47, &pDir);
									}
									else
									{
										//monsters
										Actor::AI_Pursue3(actor_id, target_pid, 0, &pDir);
									}
								}
								else if ( pActor->pMonsterInfo.uMovementType == MONSTER_MOVEMENT_TYPE_STAIONARY )
								{
									v47 = pActor->pMonsterInfo.uRecoveryTime * 2.133333333333333;
									Actor::AI_Stand(actor_id, target_pid, v47, &pDir);
								}
								else
								{
									v70 = (signed __int64)v80 * 307.2;
									//monsters
									//guard after player runs away
									// follow player
									Actor::AI_Pursue2(actor_id, target_pid, 0, &pDir, v70);
								}
							}
							else if ( (signed int)pActor->pMonsterInfo.uRecoveryTime > 0 )
							{
								v47 = pActor->pMonsterInfo.uRecoveryTime * 2.133333333333333;
								Actor::AI_Stand(actor_id, target_pid, v47, &pDir);
							}
							else
							{
								//monsters
								Actor::AI_MeleeAttack(actor_id, target_pid, &pDir);
							}
						}
						continue;
					}
					else if ( v45 == 2 || v45 == 3 )
					{
						if ( v45 == 2 )
							v46 = pActor->pMonsterInfo.uSpell1ID;
						else
							v46 = pActor->pMonsterInfo.uSpell2ID;
						if ( v46 )
						{
							if ( (signed int)pActor->pMonsterInfo.uRecoveryTime <= 0 )
							{
								if ( v45 == 2 )
									Actor::AI_SpellAttack1(actor_id, target_pid, &pDir);
								else
									Actor::AI_SpellAttack2(actor_id, target_pid, &pDir);
							}
							else if ( v80 * 307.2 > (double)v81 || pActor->pMonsterInfo.uMovementType == MONSTER_MOVEMENT_TYPE_STAIONARY )
							{
								v47 = pActor->pMonsterInfo.uRecoveryTime * 2.133333333333333;
								Actor::AI_Stand(actor_id, target_pid, v47, &pDir);
							}
							else
							{
								v47 = pActor->pMonsterInfo.uRecoveryTime * 2.133333333333333;
								Actor::AI_Pursue1(actor_id, target_pid, actor_id, v47, &pDir);
							}
						}
						else
						{
							if ( (double)v81 >= v80 * 307.2 )
							{
								if ( v81 >= 1024 )
								{
									if ( pActor->pMonsterInfo.uMovementType == MONSTER_MOVEMENT_TYPE_STAIONARY )
									{
										v47 = pActor->pMonsterInfo.uRecoveryTime * 2.133333333333333;
										Actor::AI_Stand(actor_id, target_pid, v47, &pDir);
									}
									else
									{
										Actor::AI_Pursue3(actor_id, target_pid, 256, &pDir);
									}
								}
								else if ( pActor->pMonsterInfo.uMovementType == MONSTER_MOVEMENT_TYPE_STAIONARY )
								{
									v47 = pActor->pMonsterInfo.uRecoveryTime * 2.133333333333333;
									Actor::AI_Stand(actor_id, target_pid, v47, &pDir);
								}
								else
								{
									v70 = (signed __int64)v80 * 307.2;
									Actor::AI_Pursue2(actor_id, target_pid, 0, &pDir, v70);
								}
							}
							else if ( (signed int)pActor->pMonsterInfo.uRecoveryTime > 0 )
							{
								v47 = pActor->pMonsterInfo.uRecoveryTime * 2.133333333333333;
								Actor::AI_Stand(actor_id, target_pid, v47, &pDir);
							}
							else
							{
								Actor::AI_MeleeAttack(actor_id, target_pid, &pDir);
							}
						}
						continue;
					}
				}
			}
			
			if ( pActor->pMonsterInfo.uHostilityType != MonsterInfo::Hostility_Long || !target_pid || v81 >= 5120 || v45 != 1 )
			{
				if ( pActor->pMonsterInfo.uMovementType == MONSTER_MOVEMENT_TYPE_SHORT )
				{
					Actor::AI_RandomMove(actor_id, 4, 1024, 0);
				}
				else if ( pActor->pMonsterInfo.uMovementType == 1 )
				{
					Actor::AI_RandomMove(actor_id, 4, 2560, 0);
				}
				else if ( pActor->pMonsterInfo.uMovementType == 2 )
				{
					Actor::AI_RandomMove(actor_id, 4, 5120, 0);
				}
				else if ( pActor->pMonsterInfo.uMovementType == 4 )
				{
					Actor::AI_RandomMove(actor_id, 4, 10240, 0);
				}
				else if ( pActor->pMonsterInfo.uMovementType == MONSTER_MOVEMENT_TYPE_STAIONARY )
				{
					v57 = Actor::GetDirectionInfo(a1, 4u, &v72, 0);
					v58 = (double)(signed int)pActor->pMonsterInfo.uRecoveryTime * 2.133333333333333;
					memcpy(&v74, v57, sizeof(v74));
					memcpy(&pDir, &v74, sizeof(pDir));
					Actor::AI_Stand(actor_id, 4, v58, &pDir);
				}				
			}
			else if ( !pActor->pMonsterInfo.uMissleAttack2Type )
			{
				if ( (double)v81 >= v80 * 307.2 )
				{
					if ( v81 >= 1024 )
					{
						if ( pActor->pMonsterInfo.uMovementType == MONSTER_MOVEMENT_TYPE_STAIONARY )
						{
							v47 = pActor->pMonsterInfo.uRecoveryTime * 2.133333333333333;
							Actor::AI_Stand(actor_id, target_pid, v47, &pDir);
						}
						else
						{
							Actor::AI_Pursue3(actor_id, target_pid, 256, &pDir);
						}
					}
					else if ( pActor->pMonsterInfo.uMovementType == MONSTER_MOVEMENT_TYPE_STAIONARY )
					{
						v47 = pActor->pMonsterInfo.uRecoveryTime * 2.133333333333333;
						Actor::AI_Stand(actor_id, target_pid, v47, &pDir);
					}
					else
					{
						v70 = (signed __int64)v80 * 307.2;
						Actor::AI_Pursue2(actor_id, target_pid, 0, &pDir, v70);
					}
				}
				else if ( (signed int)pActor->pMonsterInfo.uRecoveryTime > 0 )
				{
					v47 = pActor->pMonsterInfo.uRecoveryTime * 2.133333333333333;
					Actor::AI_Stand(actor_id, target_pid, v47, &pDir);
				}
				else
				{
					Actor::AI_MeleeAttack(actor_id, target_pid, &pDir);
				}
			}
			else if ( (signed int)pActor->pMonsterInfo.uRecoveryTime > 0 )
			{
				v47 = (double)(signed int)pActor->pMonsterInfo.uRecoveryTime * 2.133333333333333;
				if ( v80 * 307.2 > (double)v81 || pActor->pMonsterInfo.uMovementType == MONSTER_MOVEMENT_TYPE_STAIONARY )
					Actor::AI_Stand(actor_id, target_pid, v47, &pDir);
				else
					Actor::AI_Pursue1(actor_id, target_pid, actor_id, v47, &pDir);
			}
			else
			{
				Actor::AI_MissileAttack2(actor_id, target_pid, &pDir);
			}		
		}
	}
}
//----- (0044665D) --------------------------------------------------------
// uType:     0 -> any monster
//            1 -> uParam is GroupID
//            2 -> uParam is MonsterID
//            3 -> uParam is ActorID
// uNumAlive: 0 -> all must be alive
int __fastcall IsActorAlive(unsigned int uType, unsigned int uParam, unsigned int uNumAlive)
{
  unsigned int uAliveActors; // eax@6
  unsigned int uTotalActors; // [sp+0h] [bp-4h]@1

  uTotalActors = 0;
  if ( uType )
  {
    if ( uType == 1 )
    {
      uAliveActors = SearchActorByGroup(&uTotalActors, uParam);
    }
    else
    {
      if ( uType == 2 )
      {
        uAliveActors = SearchActorByMonsterID(&uTotalActors, uParam);
      }
      else
      {
        if ( uType != 3 )
          return 0;
        uAliveActors = SearchActorByID(&uTotalActors, uParam);
      }
    }
  }
  else
  {
    uAliveActors = SearchAliveActors(&uTotalActors);
  }

  if (uNumAlive)
    return uAliveActors >= uNumAlive;
  else
    return uTotalActors == uAliveActors;
}
//----- (00408B54) --------------------------------------------------------
unsigned int SearchActorByID(unsigned int *pTotalActors, unsigned int a2)
{
  unsigned int v2; // edi@1
  unsigned int *v3; // esi@1
  int v4; // eax@1
  unsigned int v5; // ebx@1
  unsigned int v6; // edx@1

  v2 = a2;
  v3 = pTotalActors;
  v4 = GetAlertStatus();
  v5 = 0;
  *v3 = 0;
  v6 = pActors[v2].uAttributes;
  if ( (v6 & 0x100000) == v4 )
  {
    *v3 = 1;
    if ( pActors[v2].IsNotAlive() == 1 )
      v5 = 1;
  }
  return v5;
}
//----- (00408AE7) --------------------------------------------------------
unsigned int SearchActorByGroup(unsigned int *pTotalActors, unsigned int uGroup)
{
  unsigned int *v2; // esi@1
  signed int v3; // ebx@1
  Actor *v4; // edi@2
  int v5; // eax@3
  unsigned int v7; // [sp+8h] [bp-Ch]@1
  int v8; // [sp+Ch] [bp-8h]@1
  unsigned int v9; // [sp+10h] [bp-4h]@1

  v7 = uGroup;
  v2 = pTotalActors;
  v3 = 0;
  v8 = GetAlertStatus();
  *v2 = 0;
  v9 = 0;
  if ( (signed int)uNumActors > 0 )
  {
    v4 = pActors.data();//[0].uGroup;
    do
    {
	  v5 = v4->uAttributes;
      if ( (v5 & 0x100000) == v8 )
      {
		if ( v4->uGroup == v7 )
        {
          ++*v2;
          if ( v4->IsNotAlive() == 1 )
            ++v9;
        }
      }
      ++v3;
      ++v4;
    }
    while ( v3 < (signed int)uNumActors );
  }
  return v9;
}
//----- (00408A7E) --------------------------------------------------------
unsigned int SearchActorByMonsterID(unsigned int *pTotalActors, int uMonsterID)
{
  unsigned int *v2; // esi@1
  signed int v3; // ebx@1
  Actor *v4; // edi@2
  int v5; // eax@3
  int v7; // [sp+8h] [bp-Ch]@1
  int v8; // [sp+Ch] [bp-8h]@1
  unsigned int v9; // [sp+10h] [bp-4h]@1

  v7 = uMonsterID;
  v2 = pTotalActors;
  v3 = 0;
  v8 = GetAlertStatus();
  *v2 = 0;
  v9 = 0;
  if ( (signed int)uNumActors > 0 )
  {
    v4 = pActors.data();//[0].pMonsterInfo.uID;
    do
    {
	  v5 = v4->uAttributes;                // actor::attributes
      if ( (v5 & 0x100000) == v8 )
      {
		if ( v4->pMonsterInfo.field_33 == v7 )
        {
          ++*v2;
          if ( v4->IsNotAlive() == 1 )
            ++v9;
        }
      }
      ++v3;
      ++v4;
    }
    while ( v3 < (signed int)uNumActors );
  }
  return v9;
}
//----- (00408A27) --------------------------------------------------------
unsigned int SearchAliveActors(unsigned int *pTotalActors)
{
  unsigned int *v1; // esi@1
  int v2; // eax@1
  unsigned int v3; // ebp@1
  signed int v4; // ebx@1
  Actor *v5; // edi@2
  unsigned int v6; // eax@3
  int v8; // [sp+Ch] [bp-4h]@1

  v1 = pTotalActors;
  v2 = GetAlertStatus();
  v3 = 0;
  v4 = 0;
  *v1 = 0;
  v8 = v2;
  if ( (signed int)uNumActors > 0 )
  {
    v5 = pActors.data();
    do
    {
      v6 = v5->uAttributes;
      if ( (v6 & 0x100000) == v8 )
      {
        ++*v1;
        if ( v5->IsNotAlive() == 1 )
          ++v3;
      }
      ++v4;
      ++v5;
    }
    while ( v4 < (signed int)uNumActors );
  }
  return v3;
}
//----- (00408768) --------------------------------------------------------
void InitializeActors()
{
  signed int v5; // [sp+Ch] [bp-10h]@1
  signed int v6; // [sp+10h] [bp-Ch]@1
  signed int v7; // [sp+14h] [bp-8h]@1
  signed int v8; // [sp+18h] [bp-4h]@1

  v8 = 0;
  v6 = 0;
  v7 = 0;
  v5 = 0;
  if ( !_stricmp(pCurrentMapName, "d25.blv") )
    v8 = 1;
  if ( !_stricmp(pCurrentMapName, "d26.blv") )
    v6 = 1;
  if (pParty->IsPartyGood())
    v7 = 1;
  if (pParty->IsPartyEvil())
    v5 = 1;

  Log::Warning(L"%S %S %u", __FILE__, __FUNCTION__, __LINE__); // ai_near_actors_targets_pid[i] for AI_Stand seems always 0;  original code behaviour is identical
  for (uint i = 0; i < uNumActors; ++i)
  {
    auto actor = &pActors[i];

    if (actor->CanAct() || actor->uAIState == Disabled)
    {
      actor->vPosition.x = actor->vInitialPosition.x;
      actor->vPosition.y = actor->vInitialPosition.y;
      actor->vPosition.z = actor->vInitialPosition.z;
      actor->sCurrentHP = actor->pMonsterInfo.uHP;
      if (actor->uAIState != Disabled)
      {
        Actor::AI_Stand(i, ai_near_actors_targets_pid[i], actor->pMonsterInfo.uRecoveryTime, 0);
      }
    }

    actor->pMonsterInfo.uHostilityType = MonsterInfo::Hostility_Friendly;

    if (!v8 || v7)
      if (!v6 || v5)
        if (actor->IsPeasant())
          BYTE2(actor->uAttributes) &= 0xF7u;

    BYTE2(actor->uAttributes) &= 0x7Fu;
    if (BYTE2(actor->uAttributes) & 0x40)
        Actor::_4031C1_update_job_never_gets_called(i, pParty->uCurrentHour, 1);
  }
}
//----- (00439474) --------------------------------------------------------
void DamageMonsterFromParty(signed int a1, unsigned int uActorID_Monster, Vec3_int_ *pVelocity)
{
  //signed int v3; // eax@1
  SpriteObject *v4; // ebx@1
  //int v5; // edx@3
  //bool uPlayerID; // eax@3
  //Player *pPlayer; // edi@4
  Actor *pMonster; // esi@7
  //SpriteObject *v9; // ebx@12
  int v10; // eax@12
  int v11; // ebx@12
  unsigned int v12; // ecx@12
  int v13; // edx@15
  int v14; // edx@17
  int v15; // eax@24
  unsigned __int16 v16; // cx@25
  int v17; // eax@29
  int v18; // eax@38
  unsigned __int8 v19; // zf@38
  unsigned __int8 v20; // sf@38
  int v21; // edx@44
  int v22; // eax@44
  unsigned __int8 v23; // zf@44
  unsigned __int8 v24; // sf@44
  int v25; // edx@51
  int v26; // ecx@51
  //signed int v27; // eax@51
  //int v28; // eax@53
  signed int v29; // eax@76
  signed int v30; // eax@84
  signed int v31; // eax@92
  int v32; // eax@94
  int v33; // eax@100
  int v34; // eax@104
  signed int v35; // eax@104
  double v36; // st7@104
  float v37; // ST08_4@104
  float v38; // ST04_4@104
  float v39; // ST00_4@104
  int v40; // ebx@107
  unsigned int v41; // ebx@109
  signed __int64 v42; // qax@125
  unsigned __int16 v43; // ax@132
  char v44; // bl@132
  unsigned __int16 v45; // ax@132
  unsigned __int64 v46; // [sp+Ch] [bp-60h]@6
  const char *v47; // [sp+14h] [bp-58h]@104
  char *pPlayerName; // [sp+18h] [bp-54h]@12
  char *pMonsterName; // [sp+1Ch] [bp-50h]@6
  int v50; // [sp+20h] [bp-4Ch]@6
  //unsigned __int64 *v51; // [sp+30h] [bp-3Ch]@6
  int v52; // [sp+34h] [bp-38h]@12
  //int v53; // [sp+38h] [bp-34h]@10
  //int v54; // [sp+3Ch] [bp-30h]@1
  //int v55; // [sp+40h] [bp-2Ch]@12
  signed int a4; // [sp+44h] [bp-28h]@1
  PlayerEquipment *v57; // [sp+48h] [bp-24h]@10
  //int v58; // [sp+4Ch] [bp-20h]@10
  int v59; // [sp+50h] [bp-1Ch]@1
  unsigned int uActorID_Monster_; // [sp+54h] [bp-18h]@1
  int v61; // [sp+58h] [bp-14h]@1
  bool v62; // [sp+5Ch] [bp-10h]@1
  int uDamageAmount; // [sp+60h] [bp-Ch]@1
  int a2; // [sp+64h] [bp-8h]@27
  int a3; // [sp+6Bh] [bp-1h]@6

  //v3 = a1;
  v4 = 0;
  uActorID_Monster_ = uActorID_Monster;
  //v54 = a1;
  uDamageAmount = 0;
  a4 = 0;
  v61 = 0;
  v59 = 0;
  v62 = 0;
  if ( PID_TYPE(a1) == OBJECT_Item)
  {
    v4 = &pSpriteObjects[PID_ID(a1)];
    //uDamageAmount = (int)v4;
    v61 = v4->field_60_distance_related_prolly_lod;
    a1 = v4->spell_caster_pid;
    //v54 = v4->field_58_pid;
  }
  //v5 = a1 & 7;
  //uPlayerID = a1 >> 3;
  if (PID_TYPE(a1) != OBJECT_Player)
    return;

  assert(PID_ID(abs(a1)) < 4);
  auto player = &pParty->pPlayers[PID_ID(a1)];
  pMonster = &pActors[uActorID_Monster_];
  //uPlayerID = pMonster->IsAlive();
  if (pMonster->IsNotAlive())
    return;

  pMonster->uAttributes |= 0xC000;
  if ( pMonster->uAIState == Fleeing )
    pMonster->uAttributes |= 0x20000u;
  //v57 = 0;
  //v53 = 0;
  //v58 = 0;
  bool hit_will_stun = false,
       hit_will_paralyze = false;
  if ( !v4 )
  {
    //v51 = (unsigned __int64 *)player->pEquipment.uMainHand;
    int main_hand_idx = player->pEquipment.uMainHand;
    v59 = 1;
    if ( player->HasItemEquipped(EQUIP_MAIN_HAND) )
    {
      auto main_hand_skill = player->GetMainHandItem()->GetPlayerSkillType();
      //v55 = pItemsTable->pItems[player->pInventoryItems[main_hand_idx - 1].uItemID].uSkillType;
      //v28 = SkillToMastery(player->pActiveSkills[v55]);
      auto main_hand_mastery = SkillToMastery(player->pActiveSkills[main_hand_skill]);
      //uDamageAmount = v28;
      switch (main_hand_skill)
      {
        case PLAYER_SKILL_STAFF:
          if (main_hand_mastery >= 3)
          {
            if (rand() % 100 < (player->GetActualSkillLevel(PLAYER_SKILL_STAFF) & 0x3F))  // stun chance when mastery >= 3
              hit_will_stun = true;
          }
        break;

        case PLAYER_SKILL_MACE:
          if (main_hand_mastery >= 3)
          {
            if (rand() % 100 < (player->GetActualSkillLevel(PLAYER_SKILL_MACE) & 0x3F))
              hit_will_stun = true;
          }
          if (main_hand_mastery >= 4)
          {
            if (rand() % 100 < (player->GetActualSkillLevel(PLAYER_SKILL_MACE) & 0x3F))
              hit_will_paralyze = true;
          }
        break;
      }
    }
    v50 = pMonster->pMonsterInfo.uID;
    a2 = 4;
    //v27 = player->CalculateMeleeDamageTo(0, 0, v50);
    uDamageAmount = player->CalculateMeleeDamageTo(false, false, v50);
    //if ( !v57 )
      goto LABEL_67;
    //goto LABEL_69;
  }


  v19 = v4->spell_id == SPELL_DARK_SOULDRINKER;
  v61 = v4->field_60_distance_related_prolly_lod;
  if ( !v19 )
  {
    //v9 = (SpriteObject *)uDamageAmount;
	v50 = pParty->vPosition.x - v4->vPosition.x;
    //v55 = abs(v50);
    pMonsterName = (char *)(pParty->vPosition.y - v4->vPosition.y);
    //v51 = (unsigned __int64 *)abs((int)pMonsterName);
    pPlayerName = (char *)(pParty->vPosition.z - v4->vPosition.z);
    v52 = abs((int)pPlayerName);
    v61 = abs(v50);
    v10 = abs(v50);
    v11 = (int)abs((int)pMonsterName);
    v12 = v52;
    if ( v10 < v11)
    {
      v10 = (int)v11;
      v11 = v10;
    }
    if ( v10 < v52 )
    {
      v13 = v10;
      v10 = v52;
      v12 = v13;
    }
    if ( v11 < (signed int)v12 )
    {
      v14 = v12;
      v12 = v11;
      v11 = v14;
    }
    //uPlayerID = ((unsigned int)(11 * v11) >> 5) + (v12 >> 2) + v10;
    v61 = ((unsigned int)(11 * v11) >> 5) + (v12 >> 2) + v10;
    if ( v61 >= 2560 )
    {
      if ( v61 >= 5120 && !(BYTE1(pMonster->uAttributes) & 4) )
        return;
      v61 = 2;
    }
    else
    {
      v61 = 1;
    }
    //v4 = (SpriteObject *)uDamageAmount;
  }

  v15 = v4->spell_id;
  if ( v15 == SPELL_LASER_PROJECTILE )
  {
    v16 = player->pActiveSkills[7];
    v61 = 1;
    if ( (signed int)SkillToMastery(v16) >= 3 )
      a4 = player->pActiveSkills[7] & 0x3F;
    a2 = 4;
    uDamageAmount = player->CalculateMeleeDamageTo(true, true, 0);
    goto LABEL_67;
  }
  if ( v15 != SPELL_BOW_ARROW )
  {
    if ( v15 == SPELL_101 )
    {
      a2 = 0;
      v18 = player->CalculateRangedDamageTo(0);
      v19 = HIDWORD(pMonster->pActorBuffs[15].uExpireTime) == 0;
      v20 = SHIDWORD(pMonster->pActorBuffs[15].uExpireTime) < 0;
      uDamageAmount = v18;
      if ( !v20 && (!(v20 | v19) || LODWORD(pMonster->pActorBuffs[15].uExpireTime)) )
        uDamageAmount >>= 1;
      v59 = 1;
      goto LABEL_67;
    }
    if ( v15 == SPELL_EARTH_BLADES )
    {
      a4 = 5 * v4->spell_level;
      a2 = player->GetSpellSchool(0x27u);
      v21 = v4->spell_level;
      v50 = pMonster->sCurrentHP;
      pMonsterName = (char *)v4->spell_skill;
      v22 = _43AFE3_calc_spell_damage(39, v21, v4->spell_skill, v50);
      v23 = HIDWORD(pMonster->pActorBuffs[15].uExpireTime) == 0;
      v24 = SHIDWORD(pMonster->pActorBuffs[15].uExpireTime) < 0;
      uDamageAmount = v22;
      if ( !v24 && (!(v24 | v23) || LODWORD(pMonster->pActorBuffs[15].uExpireTime)) )
        uDamageAmount >>= 1;
      v59 = 0;
LABEL_67:
      if ( !stru_50C198.PlayerHitOrMiss(player, pMonster, v61, a4) )
      {
//LABEL_68:
        player->PlaySound(SPEECH_52, 0);
        return;
      }
      goto LABEL_69;
    }
    if ( v15 == SPELL_EARTH_STUN )
    {
      uDamageAmount = 0;
      a2 = 4;
      hit_will_stun = 1;
      goto LABEL_67;
    }
    a2 = player->GetSpellSchool(v4->spell_id);
    v25 = v4->spell_level;
    v26 = v4->spell_id;
    v50 = pMonster->sCurrentHP;
    pMonsterName = (char *)v4->spell_skill;
    //v27 = _43AFE3_calc_spell_damage(v26, v25, (signed int)pMonsterName, v50);
    v59 = 0;
    //v57 = (PlayerEquipment *)1;
//LABEL_65:
    uDamageAmount = _43AFE3_calc_spell_damage(v26, v25, v4->spell_skill, v50);
    //if ( !v57 )
    //  goto LABEL_67;
    goto LABEL_69;
  }
  v50 = pMonster->word_000086_some_monster_id;
  a2 = 4;
  v17 = player->CalculateRangedDamageTo(v50);
  v19 = v4->stru_24.uItemID == 0;
  uDamageAmount = v17;
  v57 = 0;
  if ( !v19 && v4->stru_24.uSpecEnchantmentType == 3 )
  {
    a2 = 0;
    v57 = (PlayerEquipment *)1;
  }
  if ( SHIDWORD(pMonster->pActorBuffs[15].uExpireTime) >= 0
    && (SHIDWORD(pMonster->pActorBuffs[15].uExpireTime) > 0 || LODWORD(pMonster->pActorBuffs[15].uExpireTime)) )
    uDamageAmount >>= 1;
  v59 = 1;
//LABEL_66:
  if ( !v57 )
    goto LABEL_67;
LABEL_69:
  if (player->IsWeak())
    uDamageAmount /= 1;
  if ( (signed __int64)pMonster->pActorBuffs[5].uExpireTime > 0 )
    uDamageAmount = 0;
  v61 = stru_50C198.CalcMagicalDamageToActor(pMonster, a2, uDamageAmount);
  if ( !v4 && player->IsUnarmed() && (signed __int64)player->pPlayerBuffs[6].uExpireTime > 0 )
  {
    v50 = player->pPlayerBuffs[6].uPower;
    v29 = stru_50C198.CalcMagicalDamageToActor(pMonster, 8, v50);
    v61 += v29;
  }
  uDamageAmount = v61;
  if ( v59 )
  {
    if ( v4 )
    {
      a4 = v4->stru_24._439DF3_get_additional_damage(&a2, &v62);
      if ( v62 && pMonster->sCurrentHP > 0 )
      {
        player->sHealth += v61 / 5;
        if ( player->sHealth > player->GetMaxHealth() )
          player->sHealth = player->GetMaxHealth();
        v62 = 0;
      }
      v30 = stru_50C198.CalcMagicalDamageToActor(pMonster, a2, a4);
      uDamageAmount = v61 + v30;
    }
    else
    {
      v59 = 0;
      v57 = &player->pEquipment;
      do
      {
        if ( player->HasItemEquipped((ITEM_EQUIP_TYPE)v59) )
        {
          auto _s = (ItemGen *)&player->pInventoryItemList[v57->uShield - 1];
          a4 = _s->_439DF3_get_additional_damage(&a2, &v62);
          if ( v62 && pMonster->sCurrentHP > 0 )
          {
            player->sHealth += v61 / 5;
            if ( player->sHealth > player->GetMaxHealth() )
              player->sHealth = player->GetMaxHealth();
            v62 = 0;
          }
          v31 = stru_50C198.CalcMagicalDamageToActor(pMonster, a2, a4);
          uDamageAmount += v31;
        }
        ++v59;
        v57 = (PlayerEquipment *)((char *)v57 + 4);
      }
      while ( v59 <= 1 );
    }
  }
  v32 = uDamageAmount;
  pMonster->sCurrentHP -= uDamageAmount;
  if ( !v32 && !hit_will_stun )
  {
    player->PlaySound(SPEECH_52, 0);
    return;
  }
  if ( pMonster->sCurrentHP > 0 )
  {
    Actor::AI_Stun(uActorID_Monster_, a1, 0);
    Actor::AggroSurroundingPeasants(uActorID_Monster_, 1);
    if ( bShowDamage )
    {
      v50 = uDamageAmount;
      pMonsterName = (char *)pMonster;
      pPlayerName = player->pName;
      if ( v4 )
        v47 = pGlobalTXT_LocalizationStrings[189];// "%s shoots %s for %lu points"
      else
        v47 = pGlobalTXT_LocalizationStrings[164];// "%s hits %s for %lu damage"
      sprintfex(pTmpBuf.data(), v47, pPlayerName, pMonsterName, v50);
      ShowStatusBarString(pTmpBuf.data(), 2u);
    }
    v41 = 0;
  }
  else
  {
    if ( pMonsterStats->pInfos[pMonster->pMonsterInfo.uID].bQuestMonster & 1 )
    {
      v33 = byte_4D864C && BYTE2(pGame->uFlags) & 8 ? 10 * pMonster->uActorRadius : pMonster->uActorRadius;
      //v55 = v33;
      if ( pRenderer->pRenderD3D )
      {
        if ( pGame->uFlags2 & GAME_FLAGS_2_DRAW_BLOODSPLATS )
        {
          v50 = 0;
          pMonsterName = 0;
          v34 = pMonster->vPosition.z;
          *(float *)&pPlayerName = (double)v33;
          //v51 = (unsigned __int64 *)v34;
          *(float *)&v47 = 0.0;
          v35 = pMonster->vPosition.y;
          *((float *)&v46 + 1) = 0.0;
          *(float *)&v46 = 1.0;
          v36 = (double)(signed int)pMonster->vPosition.z;
          //v51 = (unsigned __int64 *)v35;
          v37 = v36;
          //v51 = (unsigned __int64 *)pMonster->vPosition.x;
          v38 = (double)v35;
          v39 = (double)(signed int)pMonster->vPosition.x;
          pDecalBuilder->AddBloodsplat(v39, v38, v37, 1.0, 0.0, 0.0, *(float *)&pPlayerName, 0, 0);
        }
      }
    }
    Actor::Die(uActorID_Monster_);
    Actor::ApplyFineForKillingPeasant(uActorID_Monster_);
    Actor::AggroSurroundingPeasants(uActorID_Monster_, 1);
    if ( pMonster->pMonsterInfo.uExp )
      pParty->GivePartyExp(pMonsterStats->pInfos[pMonster->pMonsterInfo.uID].uExp);
    v40 = SPEECH_51;
    if ( rand() % 100 < 20 )
      v40 = ((signed int)pMonster->pMonsterInfo.uHP >= 100) + 1;
    player->PlaySound((PlayerSpeech)v40, 0);
    v41 = 0;
    if ( bShowDamage )
    {
      v50 = (int)pMonster;
      pMonsterName = (char *)uDamageAmount;
      pPlayerName = player->pName;             // "%s inflicts %lu points killing %s"
      sprintfex(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[175], player->pName, uDamageAmount, pMonster);
      ShowStatusBarString(pTmpBuf.data(), 2u);
    }
  }
  if ( SHIDWORD(pMonster->pActorBuffs[20].uExpireTime) >= (signed int)v41
    && (SHIDWORD(pMonster->pActorBuffs[20].uExpireTime) > (signed int)v41
     || LODWORD(pMonster->pActorBuffs[20].uExpireTime) > v41)
    && uDamageAmount != v41 )
    player->ReceiveDamage(uDamageAmount, (DAMAGE_TYPE)a2);
  v50 = 24;
  v59 = 20 * v61 / (signed int)pMonster->pMonsterInfo.uHP;
  if ( (player->GetSpecialItemBonus(24) || hit_will_stun != v41)
    && stru_50C198.GetMagicalResistance(pMonster, 3u) )
  {
    LODWORD(v42) = 20;
    v59 = 10;
    if ( pParty->bTurnBasedModeOn == v41 )
      v42 = (signed __int64)(flt_6BE3A8_debug_recmod2 * 42.66666666666666);
    pMonster->pMonsterInfo.uRecoveryTime += v42;
    if ( bShowDamage != v41 )
    {
      v50 = (int)pMonster;
      pMonsterName = player->pName;            // "%s stuns %s"
      sprintfex(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[635], player->pName, pMonster);
      ShowStatusBarString(pTmpBuf.data(), 2u);
    }
  }
  if ( hit_will_paralyze != v41 )
  {
    if ( pMonster->CanAct() )
    {
      if ( stru_50C198.GetMagicalResistance(pMonster, 3) )
      {
        LOBYTE(v43) = player->GetActualSkillLevel(PLAYER_SKILL_MACE);
        v44 = v43;
        v45 = SkillToMastery(v43);
        //v51 = (unsigned __int64 *)(7680 * (v44 & 0x3F));
        v46 = pParty->uTimePlayed + (signed int)(signed __int64)((double)(signed int)(7680 * (v44 & 0x3F)) * 0.033333335);
        pMonster->pActorBuffs[6].Apply(v46, v45, 0, 0, 0);
        if ( bShowDamage )
        {
          v50 = (int)pMonster;
          pMonsterName = player->pName;        // "%s paralyzes %s"
          sprintfex(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[636], player->pName, pMonster);
          ShowStatusBarString(pTmpBuf.data(), 2u);
        }
      }
    }
  }
  if ( v59 > 10 )
    v59 = 10;
  if ( !MonsterStats::BelongsToSupertype(pMonster->pMonsterInfo.uID, MONSTER_SUPERTYPE_TREANT) )
  {
    pVelocity->x = (unsigned __int64)(v59 * (signed __int64)pVelocity->x) >> 16;
    pVelocity->y = (unsigned __int64)(v59 * (signed __int64)pVelocity->y) >> 16;
    pVelocity->z = (unsigned __int64)(v59 * (signed __int64)pVelocity->z) >> 16;
    pMonster->vVelocity.x = 50 * LOWORD(pVelocity->x);
    pMonster->vVelocity.y = 50 * LOWORD(pVelocity->y);
    pMonster->vVelocity.z = 50 * LOWORD(pVelocity->z);
  }
  Actor::AddBloodsplatOnDamageOverlay(uActorID_Monster_, 1, v61);
}
//----- (004BBF61) --------------------------------------------------------
void __fastcall _4BBF61_summon_actor(int a1, __int16 x, int y, int z)
{
  size_t v4; // esi@1
  int monster_id; // edi@1
  __int16 v6; // ax@4
  Actor *v7; // esi@5
  int v8; // eax@5
  MonsterInfo *v9; // edi@5
  MonsterDesc *v10; // ebx@5
  unsigned __int16 *v11; // ebx@5
  int v12; // ebx@7
  int v13; // eax@8
  __int16 x_; // [sp+8h] [bp-Ch]@1
  __int16 v15; // [sp+Ch] [bp-8h]@1
  __int16 v16; // [sp+10h] [bp-4h]@3
  signed int ya; // [sp+1Ch] [bp+8h]@5

  v4 = uNumActors;
  monster_id = a1;
  x_ = x;
  v15 = a1;
  if (uNumActors < 500)
  {
    v16 = 0;
    if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
    {
      v6 = pIndoor->GetSector(x, y, z);
      v4 = uNumActors;
      v16 = v6;
    }
    v7 = &pActors[v4];
    v7->Reset();
    v8 = monster_id;
    v9 = &pMonsterStats->pInfos[monster_id];
    v10 = &pMonsterList->pMonsters[v8 - 1];
    strcpy(v7->pActorName, v9->pName);
    v7->sCurrentHP = LOWORD(v9->uHP);
    memcpy(&v7->pMonsterInfo, v9, 0x58u);
    v7->word_000086_some_monster_id = v15;
    v7->uActorRadius = v10->uMonsterRadius;
    v7->uActorHeight = v10->uMonsterHeight;
    v7->uMovementSpeed = v10->uMovementSpeed;
    v7->vInitialPosition.x = x_;
    v7->vPosition.x = x_;
    BYTE2(v7->uAttributes) |= 8u;
    v7->pMonsterInfo.uTreasureType = 0;
    v7->pMonsterInfo.uTreasureLevel = 0;
    v7->pMonsterInfo.uTreasureDiceSides = 0;
    v7->pMonsterInfo.uTreasureDiceRolls = 0;
    v7->pMonsterInfo.uTreasureDropChance = 0;
    v7->vInitialPosition.y = y;
    v7->vPosition.y = y;
    v7->vInitialPosition.z = z;
    v7->vPosition.z = z;
    v7->uTetherDistance = 256;
    v7->uSectorID = v16;
    v7->uGroup = 1;
    v7->pMonsterInfo.uHostilityType = MonsterInfo::Hostility_Long;
    v7->PrepareSprites(0);
    v11 = v10->pSoundSampleIDs;
    ya = 4;
    do
    {
      pSoundList->LoadSound((signed __int16)*v11, 0);
      ++v11;
      --ya;
    }
    while ( ya );
    v12 = 0;
    do
      LOWORD(v13) = pSoundList->LoadSound(v12++ + word_4EE088_sound_ids[v9->uSpell1ID], 1u);
    while ( v13 );
    ++uNumActors;
  }
}