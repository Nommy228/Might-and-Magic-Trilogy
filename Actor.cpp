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
#include "Viewport.h"





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

  isstoned = (signed __int64)this->pActorBuffs[ACTOR_BUFF_STONED].uExpireTime > 0;// stoned
  isparalyzed = (signed __int64)this->pActorBuffs[ACTOR_BUFF_PARALYZED].uExpireTime > 0;// paralyzed
  v3 = this->uAIState;
  return !(isstoned || isparalyzed || v3 == Dying || v3 == Dead || v3 == Removed || v3 == Summoned || v3 == Disabled);
}

//----- (004089C7) --------------------------------------------------------
bool Actor::IsNotAlive()
{
  bool isstoned; // esi@1

  isstoned = (signed __int64)this->pActorBuffs[ACTOR_BUFF_STONED].uExpireTime > 0;// stoned
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
      actorPtr->pActorBuffs[ACTOR_BUFF_HASTE].Apply(
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
      actorPtr->pActorBuffs[ACTOR_BUFF_SHIELD].Apply(
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
      actorPtr->pActorBuffs[ACTOR_BUFF_STONESKIN].Apply(
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
      actorPtr->pActorBuffs[ACTOR_BUFF_BLESS].Apply(
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
      actorPtr->pActorBuffs[ACTOR_BUFF_FATE].Apply(pParty->uTimePlayed + 1280, masteryLevel, v48, 0, 0);
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
      actorPtr->pActorBuffs[ACTOR_BUFF_HEROISM].Apply(
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
      actorPtr->pActorBuffs[ACTOR_BUFF_PAIN_HAMMERHANDS].Apply(
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
      actorPtr->pActorBuffs[ACTOR_BUFF_DAY_OF_PROTECTION].Apply(
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
      actorPtr->pActorBuffs[ACTOR_BUFF_HOUR_OF_POWER].Apply(
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
      actorPtr->pActorBuffs[ACTOR_BUFF_PAIN_REFLECTION].Apply(
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
  bool found = false;
  for ( uint i = 0; i < pObjectList->uNumObjects; i++)
  {
    if (pObjectList->pObjects[i].uObjectID == a1.uType)
    {
      a1.uObjectDescID = i;
      found = true;
      break;
    }
  }
  if (!found)
  {
    Error("Item not found");
    return;
  }
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
void Actor::GetDirectionInfo( unsigned int uObj1ID, unsigned int uObj2ID, struct AIDirection *pOut, int a4 )
{
  signed int v4; // eax@1
  signed int v5; // ecx@1
  int v18; // edx@15
  float v31; // st7@45
  float v32; // st6@45
  float v33; // st7@45
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
    pOut->vDirection.x = 65536;
    pOut->vDirection.y = 0;
    pOut->vDirection.z = 0;
    pOut->uDistance = 1;
    pOut->uDistanceXZ = 1;
    pOut->uYawAngle = 0;
    pOut->uPitchAngle = 0;
  }
  else
  {
    pOut->vDirection.x = (int32_t)(1.0 / v33 * v31 * 65536.0);
    pOut->vDirection.y = (int32_t)(1.0 / v33 * v32 * 65536.0);
    pOut->vDirection.z = (int32_t)(1.0 / v33 * a4a * 65536.0);
    pOut->uDistance = (uint)v33;
    pOut->uDistanceXZ = (uint)sqrt(outy2 + outx2);
    pOut->uYawAngle = stru_5C6E00->Atan2((signed __int64)v31, (signed __int64)v32);
    pOut->uPitchAngle = stru_5C6E00->Atan2(pOut->uDistanceXZ, (signed __int64)a4a);
  }
}

//----- (00404030) --------------------------------------------------------
void Actor::AI_FaceObject(unsigned int uActorID, unsigned int uObjID, int _48, AIDirection *a4)
{
  AIDirection *v7; // eax@3
  AIDirection v1; // eax@3
  Actor *v9; // ebx@3
  AIDirection a3; // [sp+8h] [bp-38h]@4

  if ( rand() % 100 >= 5 )
  {
    v9 = &pActors[uActorID];
    if ( !a4 )
    {
      Actor::GetDirectionInfo(PID(OBJECT_Actor, uActorID), uObjID, &v1, 0);
      v7 = &v1;
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
  Actor* actor = &pActors[uActorID];
  
  AIDirection a3;
  if (!a4)
  {
    Actor::GetDirectionInfo(PID(OBJECT_Actor, uActorID), object_to_face_pid, &a3, 0);
    a4 = &a3;
  }

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
		  Actor::GetDirectionInfo(PID(OBJECT_Actor, uActorID), sTargetPid, &a3, 0);
      v12 = &a3;
    }
    v3->uYawAngle = LOWORD(v12->uYawAngle);
    v3->uCurrentActionLength = pSpriteFrameTable->pSpriteSFrames[v3->pSpriteIDs[ANIM_AtkMelee]].uAnimLength * 8;
    v3->uCurrentActionTime = 0;
    v3->uAIState = AttackingMelee;
    Actor::PlaySound(uActorID, 0);
    v25 = pMonsterStats->pInfos[v3->pMonsterInfo.uID].uRecoveryTime;
    if ( v3->pActorBuffs[ACTOR_BUFF_SLOWED].uExpireTime > 0 )
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
      if ( this->pActorBuffs[ACTOR_BUFF_HOUR_OF_POWER].uExpireTime > 0 )
        v2 = this->pActorBuffs[ACTOR_BUFF_HOUR_OF_POWER].uPower;
      if ( this->pActorBuffs[ACTOR_BUFF_HEROISM].uExpireTime > 0 && this->pActorBuffs[ACTOR_BUFF_HEROISM].uPower > v2 )
        v2 = this->pActorBuffs[ACTOR_BUFF_HEROISM].uPower;
      if ( this->pActorBuffs[ACTOR_BUFF_PAIN_HAMMERHANDS].uExpireTime > 0 )
        v2 += this->pActorBuffs[ACTOR_BUFF_PAIN_HAMMERHANDS].uPower;
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
      Actor::GetDirectionInfo(PID(OBJECT_Actor,uActorID), a2, &a3, 0);
      v9 = &a3;
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
    if (v3->pActorBuffs[ACTOR_BUFF_SLOWED].uExpireTime > 0)
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
      Actor::GetDirectionInfo(PID(OBJECT_Actor,uActorID), sTargetPid, &a3, 0);
      v9 = &a3;
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
    if (v3->pActorBuffs[ACTOR_BUFF_SLOWED].uExpireTime > 0)
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
      Actor::GetDirectionInfo(PID(OBJECT_Actor,uActorID), sTargetPid, &a3, 0);
      v9 = &a3;
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
    if ( v3->pActorBuffs[ACTOR_BUFF_SLOWED].uExpireTime > 0 )
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
      Actor::GetDirectionInfo(PID(OBJECT_Actor,uActorID), sTargetPid, &a3, 0);
      v10 = &a3;
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
    if ( v3->pActorBuffs[ACTOR_BUFF_SLOWED].uExpireTime > 0 )
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
  if ( v4->pActorBuffs[ACTOR_BUFF_CHARM].uExpireTime > 0 )
    v4->pActorBuffs[ACTOR_BUFF_CHARM].Reset();
  if ( v4->pActorBuffs[ACTOR_BUFF_AFRAID].uExpireTime > 0 )
    v4->pActorBuffs[ACTOR_BUFF_AFRAID].Reset();
  if ( stunRegardlessOfState
    || (v4->uAIState != Stunned
    && v4->uAIState != AttackingRanged1
    && v4->uAIState != AttackingRanged2
    && v4->uAIState != AttackingRanged3
    && v4->uAIState != AttackingRanged4
    && v4->uAIState != AttackingMelee))
  {
    Actor::GetDirectionInfo(PID(OBJECT_Actor,uActorID), edx0, &a3, 0);
    v10 = &a3;
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
  {
    Actor::GetDirectionInfo(PID(OBJECT_Actor,uActorID), uObjID, &a3, 0);
    a4 = &a3;
  }

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
  Actor* actor = &pActors[uActorID];

  actor->uCurrentActionTime = 0;
  actor->uAIState = Dying;
  actor->uCurrentActionAnimation = ANIM_Dying;
  actor->sCurrentHP = 0;
  actor->uCurrentActionLength = 8 * pSpriteFrameTable->pSpriteSFrames[actor->pSpriteIDs[ANIM_Dying]].uAnimLength;
  actor->pActorBuffs[ACTOR_BUFF_PARALYZED].Reset();
  actor->pActorBuffs[ACTOR_BUFF_STONED].Reset();
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
    if ( pActors[uActorID].pActorBuffs[ACTOR_BUFF_SHRINK].uExpireTime <= 0 )
    {
      pAudioPlayer->PlaySound((SoundID)v3, PID(OBJECT_Actor, uActorID), 0, -1, 0, 0, 0, 0);
    }
    else
    {
      switch(pActors[uActorID].pActorBuffs[ACTOR_BUFF_SHRINK].uPower)
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
    Actor::GetDirectionInfo(v8, a2, &a3, v6);
    v10 = &a3;
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
  AIDirection v10; // [sp+8h] [bp-7Ch]@4
  AIDirection a3; // [sp+24h] [bp-60h]@3
  AIDirection* v13; // [sp+5Ch] [bp-28h]@4

  v5 = &pActors[uActorID];
  if ( v5->CanAct() )
  {
    v7 = PID(OBJECT_Actor,uActorID);
    if ( !a4 )
    {
      Actor::GetDirectionInfo(v7, sTargetPid, &a3, v5->pMonsterInfo.uFlying);
      a4 = &a3;
    }
    Actor::GetDirectionInfo(v7, 4u, &v10, 0);
    v13 = &v10;
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
    Actor::GetDirectionInfo(v8, a2, &a3, v6);
    v10 = &a3;
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
    Actor::GetDirectionInfo(v7, a2, &a3, v5);
    v20 = &a3;
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
      && thisActor->pActorBuffs[ACTOR_BUFF_ENSLAVED].uExpireTime <= 0
      && thisActor->pActorBuffs[ACTOR_BUFF_CHARM].uExpireTime <= 0
      && thisActor->pActorBuffs[ACTOR_BUFF_SUMMONED].uExpireTime <= 0 )
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
	signed int v4; // edi@10
	signed int sDmg; // eax@14
	Player *pPlayer; // ecx@21
	Actor *pActor; // esi@34
	unsigned __int16 v22; // ax@86
	unsigned int v27; // ecx@123
	unsigned int v28; // eax@123
	int v33; // eax@144
	int v34; // eax@147
	char v35; // al@150
	unsigned int v36; // edi@152
	signed int v37; // eax@154
	double v42; // st7@176
	double v43; // st6@176
	int v45; // eax@192
	unsigned __int8 v46; // cl@197
	signed int v47; // st7@206
	uint v58; // st7@246
	unsigned int v65; // [sp-10h] [bp-C0h]@144
	int v70; // [sp-10h] [bp-C0h]@213
	AIDirection v72; // [sp+0h] [bp-B0h]@246
	AIDirection a3; // [sp+1Ch] [bp-94h]@129
	int target_pid_type; // [sp+70h] [bp-40h]@83
	signed int a1; // [sp+74h] [bp-3Ch]@129
	int v78; // [sp+78h] [bp-38h]@79
	AIDirection* pDir; // [sp+7Ch] [bp-34h]@129
	float radiusMultiplier; // [sp+98h] [bp-18h]@33
	int v81; // [sp+9Ch] [bp-14h]@100
	signed int target_pid; // [sp+ACh] [bp-4h]@83
	AIState uAIState;
	uint v38;
	
	if ( uCurrentlyLoadedLevelType == LEVEL_Outdoor)
		MakeActorAIList_ODM();
	else
		MakeActorAIList_BLV();
	
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
			v4 = pParty->armageddonDamage + 50;
			if ( pParty->armageddon_timer <= 0 )
			{
				pParty->armageddon_timer = 0;
				for(size_t i = 0; i < uNumActors; i++)
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
					if ( !pPlayer->pConditions[Condition_Dead] && !pPlayer->pConditions[Condition_Pertified] && !pPlayer->pConditions[Condition_Eradicated] )
						pPlayer->ReceiveDamage(v4, DMGT_MAGICAL);
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
	
	
	for (uint i = 0; i < uNumActors; ++i)
	{
		pActor = &pActors[i];
			ai_near_actors_targets_pid[i] = OBJECT_Player;
			if (pActor->uAIState == Dead || pActor->uAIState == Removed || pActor->uAIState == Disabled || pActor->uAttributes & 0x0400)
				continue;
			
			if (!pActor->sCurrentHP && pActor->uAIState != Dying)
				Actor::Die(i);
			
			for (uint j = 0; j < 22; ++j)
			{
				if (j != 10)
				pActor->pActorBuffs[j].IsBuffExpiredToTime(pParty->uTimePlayed);
			}
			if (pActor->pActorBuffs[ACTOR_BUFF_SHRINK].uExpireTime < 0)
				pActor->uActorHeight = pMonsterList->pMonsters[pActor->pMonsterInfo.uID - 1].uMonsterHeight;
			if (pActor->pActorBuffs[ACTOR_BUFF_CHARM].uExpireTime > 0)
				pActor->pMonsterInfo.uHostilityType = MonsterInfo::Hostility_Friendly;
			else if (pActor->pActorBuffs[ACTOR_BUFF_CHARM].uExpireTime < 0)
				pActor->pMonsterInfo.uHostilityType = pMonsterStats->pInfos[pActor->pMonsterInfo.uID].uHostilityType;
			
			if (pActor->pActorBuffs[ACTOR_BUFF_PARALYZED].uExpireTime > 0 ||
				pActor->pActorBuffs[ACTOR_BUFF_STONED].uExpireTime > 0)
		      continue;
			
      pActor->pMonsterInfo.uRecoveryTime = max(pActor->pMonsterInfo.uRecoveryTime - pMiscTimer->uTimeElapsed, 0);
			
			pActor->uCurrentActionTime += pMiscTimer->uTimeElapsed;
			if (pActor->uCurrentActionTime < pActor->uCurrentActionLength)
				continue;
			
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
	}
	
	
	for(v78 = 0; v78 < ai_arrays_size; ++v78)
	{
		uint actor_id = ai_near_actors_ids[v78];
		assert(actor_id < uNumActors);
			
    pActor = &pActors[actor_id];

    v47 = (signed int)(pActor->pMonsterInfo.uRecoveryTime * 2.133333333333333);

		Actor::_SelectTarget(actor_id, &ai_near_actors_targets_pid[actor_id], true);
		if (pActor->pMonsterInfo.uHostilityType && !ai_near_actors_targets_pid[actor_id])
			pActor->pMonsterInfo.uHostilityType = MonsterInfo::Hostility_Friendly;
		target_pid = ai_near_actors_targets_pid[actor_id];
		target_pid_type = PID_TYPE(target_pid);
		if ( target_pid_type == OBJECT_Actor)
			radiusMultiplier = 0.5;
		else
			radiusMultiplier = 1.0;
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
		if ( pActor->pActorBuffs[ACTOR_BUFF_SHRINK].uExpireTime < 0 )
			pActor->uActorHeight = pMonsterList->pMonsters[pActor->pMonsterInfo.uID - 1].uMonsterHeight;
		if ( pActor->pActorBuffs[ACTOR_BUFF_CHARM].uExpireTime > 0 )
			pActor->pMonsterInfo.uHostilityType = MonsterInfo::Hostility_Friendly;
		// not sure
		else if ( pActor->pActorBuffs[ACTOR_BUFF_CHARM].uExpireTime < 0 )
			pActor->pMonsterInfo.uHostilityType = pMonsterStats->pInfos[pActor->pMonsterInfo.uID].uHostilityType;
		if ( pActor->pActorBuffs[ACTOR_BUFF_SUMMONED].uExpireTime < 0 )
		{
			pActor->uAIState = Removed;
			continue;
		}
		if ( (signed __int64)pActor->pActorBuffs[ACTOR_BUFF_STONED].uExpireTime > 0
			|| (signed __int64)pActor->pActorBuffs[ACTOR_BUFF_PARALYZED].uExpireTime > 0)
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
		Actor::GetDirectionInfo(PID(OBJECT_Actor,actor_id), target_pid, &a3, 0);
    pDir = &a3;
		uAIState = pActor->uAIState; 
		if ( pActor->pMonsterInfo.uHostilityType == MonsterInfo::Hostility_Friendly
			|| (signed int)pActor->pMonsterInfo.uRecoveryTime > 0
			|| radiusMultiplier * 307.2 < pDir->uDistance
			|| uAIState != Pursuing && uAIState != Standing && uAIState != Tethered && uAIState != Fidgeting
			&&  !pActor->pMonsterInfo.uMissleAttack1Type || uAIState != Stunned )
		{
			if ( (signed int)pActor->uCurrentActionTime < pActor->uCurrentActionLength )
			{
				continue;
			}
			else if ( pActor->uAIState == AttackingMelee )
			{
				v35 = pActor->special_ability_use_check(actor_id);
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
				Actor::AI_RangedAttack(actor_id, pDir, v34, 0);
			}
			else if ( pActor->uAIState == AttackingRanged2 )
			{
				v34 = pActor->pMonsterInfo.uMissleAttack2Type;
				Actor::AI_RangedAttack(actor_id, pDir, v34, 1);
			}
			else if ( pActor->uAIState == AttackingRanged3 )
			{
				v65 = pActor->pMonsterInfo.uSpellSkillAndMastery1;
				v33 = pActor->pMonsterInfo.uSpell1ID;
				Actor::AI_SpellAttack(actor_id, pDir, v33, 2, v65);
			}
			else if ( pActor->uAIState == AttackingRanged4 )
			{
				v65 = pActor->pMonsterInfo.uSpellSkillAndMastery2;
				v33 = pActor->pMonsterInfo.uSpell2ID;
				Actor::AI_SpellAttack(actor_id, pDir, v33, 3, v65);
			}
		}

		v36 = pDir->uDistance;
		if ( pActor->pMonsterInfo.uHostilityType == MonsterInfo::Hostility_Friendly)
		{
			if ( target_pid_type == OBJECT_Actor )
			{
				v36 = pDir->uDistance;
        v37 =pFactionTable->relations[(pActor->pMonsterInfo.uID-1) / 3 + 1][(pActors[PID_ID(target_pid)].pMonsterInfo.uID - 1) / 3 + 1];
			}
			else
			{
				v37 = 4;
			}
			v38=0;
			if ( v37 == 2 )
			{
				v38 = 1024;
			}
			else if ( v37 == 3 )
			{
				v38 = 2560;
			}
			else if ( v37 == 4 )
			{
				v38 = 5120;
			}
			if ( v37 >= 1 && v37 <= 4 && v36 < v38  || v37 == 1 )
				pActor->pMonsterInfo.uHostilityType = MonsterInfo::Hostility_Long;
		}

		if (pActor->pActorBuffs[ACTOR_BUFF_AFRAID].uExpireTime > 0)
		{
			if ( (signed int)v36 >= 10240 )
			{
				Actor::AI_RandomMove(actor_id, target_pid, 1024, 0);
			}
			else
			{
				//peasents after attacked
				//guard after attacked
				Actor::AI_Flee(actor_id, target_pid, 0, pDir);
			}
			continue;
		}

		if ( pActor->pMonsterInfo.uHostilityType == MonsterInfo::Hostility_Long && target_pid )
		{

			if ( pActor->pMonsterInfo.uAIType == 1 )
			{
				if ( pActor->pMonsterInfo.uMovementType == MONSTER_MOVEMENT_TYPE_STAIONARY )
				{
					Actor::AI_Stand(actor_id, target_pid, (uint)(pActor->pMonsterInfo.uRecoveryTime * 2.133333333333333),	pDir);
				}
				else
				{
					Actor::AI_Flee(actor_id, target_pid, 0, pDir);
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
					v42 = (double)pActor->sCurrentHP;
					if ( v43 > v42 && (signed int)v36 < 10240 )
					{
						Actor::AI_Flee(actor_id, target_pid, 0, pDir);
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
				v45 = pActor->special_ability_use_check(actor_id);
				if ( v45 == 0 )
				{
					if ( pActor->pMonsterInfo.uMissleAttack1Type )
					{
						if ( (signed int)pActor->pMonsterInfo.uRecoveryTime <= 0 )
						{
							Actor::AI_MissileAttack1(actor_id, target_pid, pDir);
						}
						else if ( pActor->pMonsterInfo.uMovementType == MONSTER_MOVEMENT_TYPE_STAIONARY )
						{
							Actor::AI_Stand(actor_id, target_pid, v47, pDir);
						}
						else
						{
							if ( radiusMultiplier * 307.2 > (double)v81 )
							{
								Actor::AI_Stand(actor_id, target_pid, v47, pDir);
							}
							else
							{
								Actor::AI_Pursue1(actor_id, target_pid, actor_id, v47, pDir);
							}
						}
					}
					else
          {
            if ( (double)v81 >= radiusMultiplier * 307.2 )
            {
              if (pActor->pMonsterInfo.uMovementType == MONSTER_MOVEMENT_TYPE_STAIONARY)
              {
                Actor::AI_Stand(actor_id, target_pid, v47, pDir);
              }
              else if ( v81 >= 1024 )
              {
				        //monsters
                Actor::AI_Pursue3(actor_id, target_pid, 0, pDir);
              }
              else
              {
                v70 = (signed int)(radiusMultiplier * 307.2);
				        //monsters
				        //guard after player runs away
				        // follow player
                Actor::AI_Pursue2(actor_id, target_pid, 0, pDir, v70);
              }
            }
            else if ( (signed int)pActor->pMonsterInfo.uRecoveryTime > 0 )
						{
							Actor::AI_Stand(actor_id, target_pid, v47, pDir);
						}
						else
						{
							//monsters
							Actor::AI_MeleeAttack(actor_id, target_pid, pDir);
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
								Actor::AI_SpellAttack1(actor_id, target_pid, pDir);
							else
								Actor::AI_SpellAttack2(actor_id, target_pid, pDir);
						}
						else if ( radiusMultiplier * 307.2 > (double)v81 || pActor->pMonsterInfo.uMovementType == MONSTER_MOVEMENT_TYPE_STAIONARY )
						{
							Actor::AI_Stand(actor_id, target_pid, v47, pDir);
						}
						else
						{
							Actor::AI_Pursue1(actor_id, target_pid, actor_id, v47, pDir);
						}
					}
					else
          {
            if ( (double)v81 >= radiusMultiplier * 307.2 ) 
            {
              if ( pActor->pMonsterInfo.uMovementType == MONSTER_MOVEMENT_TYPE_STAIONARY )
              {
                Actor::AI_Stand(actor_id, target_pid, v47, pDir);
              }
              else if ( v81 >= 1024 )
              {
                Actor::AI_Pursue3(actor_id, target_pid, 256, pDir);
              }
              else
              {
                v70 = (signed int)(radiusMultiplier * 307.2);
                Actor::AI_Pursue2(actor_id, target_pid, 0, pDir, v70);
              }
            }
						else if ( (signed int)pActor->pMonsterInfo.uRecoveryTime > 0 )
						{
							Actor::AI_Stand(actor_id, target_pid, v47, pDir);
						}
						else
						{								
							Actor::AI_MeleeAttack(actor_id, target_pid, pDir);
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
				Actor::GetDirectionInfo(a1, 4u, &v72, 0);
				v58 = (uint)(pActor->pMonsterInfo.uRecoveryTime * 2.133333333333333);
				Actor::AI_Stand(actor_id, 4, v58, &v72);
			}				
		}
		else if ( !pActor->pMonsterInfo.uMissleAttack2Type )
		{
			if ( (double)v81 >= radiusMultiplier * 307.2 )
			{
        if ( pActor->pMonsterInfo.uMovementType == MONSTER_MOVEMENT_TYPE_STAIONARY )
        {
          Actor::AI_Stand(actor_id, target_pid, v47, pDir);
        }
				else if ( v81 >= 1024 )
				{
					Actor::AI_Pursue3(actor_id, target_pid, 256, pDir);
				}
				else
				{
					v70 = (int)(radiusMultiplier * 307.2);
					Actor::AI_Pursue2(actor_id, target_pid, 0, pDir, v70);
				}
			}
			else if ( (signed int)pActor->pMonsterInfo.uRecoveryTime > 0 )
			{
				Actor::AI_Stand(actor_id, target_pid, v47, pDir);
			}
			else
			{
				Actor::AI_MeleeAttack(actor_id, target_pid, pDir);
			}
		}
		else if ( (signed int)pActor->pMonsterInfo.uRecoveryTime > 0 )
		{
			if ( radiusMultiplier * 307.2 > (double)v81 || pActor->pMonsterInfo.uMovementType == MONSTER_MOVEMENT_TYPE_STAIONARY )
				Actor::AI_Stand(actor_id, target_pid, v47, pDir);
			else
				Actor::AI_Pursue1(actor_id, target_pid, actor_id, v47, pDir);
		}
		else
		{
			Actor::AI_MissileAttack2(actor_id, target_pid, pDir);
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
  int v4; // eax@1
  unsigned int v5; // ebx@1

  v4 = GetAlertStatus();
  *pTotalActors = 0;
  if ( (pActors[a2].uAttributes & 0x100000) == GetAlertStatus() )
  {
    *pTotalActors = 1;
    if ( pActors[a2].IsNotAlive() == 1 )
      v5 = 1;
  }
  return v5;
}
//----- (00408AE7) --------------------------------------------------------
unsigned int SearchActorByGroup(unsigned int *pTotalActors, unsigned int uGroup)
{
  int v8; // [sp+Ch] [bp-8h]@1
  unsigned int v9; // [sp+10h] [bp-4h]@1
  Actor* v4;

  v8 = GetAlertStatus();
  *pTotalActors = 0;
  v9 = 0;
  for ( uint i = 0; i < uNumActors; i++)
  {
    v4 = &pActors[i];
    if ( (v4->uAttributes & 0x100000) == v8 && v4->uGroup == uGroup)
    {
      ++*pTotalActors;
      if ( v4->IsNotAlive() == 1 )
        ++v9;
    }
  }
  return v9;
}
//----- (00408A7E) --------------------------------------------------------
unsigned int SearchActorByMonsterID(unsigned int *pTotalActors, int uMonsterID)
{
  Actor *v4; // edi@2
  int v8; // [sp+Ch] [bp-8h]@1
  unsigned int v9; // [sp+10h] [bp-4h]@1

  v8 = GetAlertStatus();
  *pTotalActors = 0;
  v9 = 0;
  for ( uint i = 0; i < uNumActors; i++)
  {
    v4 = &pActors[i];
    if ( (v4->uAttributes & 0x100000) == v8 && v4->pMonsterInfo.field_33 == uMonsterID)
    {
      ++*pTotalActors;
      if ( v4->IsNotAlive() == 1 )
        ++v9;
    }
  }
  return v9;
}
//----- (00408A27) --------------------------------------------------------
unsigned int SearchAliveActors(unsigned int *pTotalActors)
{
  int v2; // eax@1
  unsigned int v3; // ebp@1
  Actor *v5; // edi@2

  v2 = GetAlertStatus();
  v3 = 0;
  *pTotalActors = 0;
  for ( uint i = 0; i < uNumActors; i++)
  {
    v5 = &pActors[i];
    if ( (v5->uAttributes & 0x100000) == v2 )
    {
      ++*pTotalActors;
      if ( v5->IsNotAlive() == 1 )
        ++v3;
    }
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
    Actor* actor = &pActors[i];

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
  SpriteObject *projectileSprite; // ebx@1
  Actor *pMonster; // esi@7
  unsigned __int16 v16; // cx@25
  int v33; // eax@100
  int v40; // ebx@107
  int extraRecoveryTime; // qax@125
  unsigned __int16 v43; // ax@132
  unsigned __int16 v45; // ax@132
  unsigned __int64 v46; // [sp+Ch] [bp-60h]@6
  char *pPlayerName; // [sp+18h] [bp-54h]@12
  char *pMonsterName; // [sp+1Ch] [bp-50h]@6
  signed int a4; // [sp+44h] [bp-28h]@1
  bool IsAdditionalDamagePossible; // [sp+50h] [bp-1Ch]@1
  int v61; // [sp+58h] [bp-14h]@1
  bool isLifeStealing; // [sp+5Ch] [bp-10h]@1
  int uDamageAmount; // [sp+60h] [bp-Ch]@1
  DAMAGE_TYPE attackElement; // [sp+64h] [bp-8h]@27

  projectileSprite = 0;
  uDamageAmount = 0;
  a4 = 0;
  v61 = 0;
  IsAdditionalDamagePossible = false;
  isLifeStealing = 0;
  if ( PID_TYPE(a1) == OBJECT_Item)
  {
    projectileSprite = &pSpriteObjects[PID_ID(a1)];
    v61 = projectileSprite->field_60_distance_related_prolly_lod;
    a1 = projectileSprite->spell_caster_pid;
  }
  if (PID_TYPE(a1) != OBJECT_Player)
    return;

  assert(PID_ID(abs(a1)) < 4);
  Player* player = &pParty->pPlayers[PID_ID(a1)];
  pMonster = &pActors[uActorID_Monster];
  if (pMonster->IsNotAlive())
    return;

  pMonster->uAttributes |= 0xC000;
  if ( pMonster->uAIState == Fleeing )
    pMonster->uAttributes |= 0x20000u;
  bool hit_will_stun = false,
       hit_will_paralyze = false;
  if ( !projectileSprite )
  {
    int main_hand_idx = player->pEquipment.uMainHand;
    IsAdditionalDamagePossible = true;
    if ( player->HasItemEquipped(EQUIP_TWO_HANDED) )
    {
      uint main_hand_skill = player->GetMainHandItem()->GetPlayerSkillType();
      uint main_hand_mastery = SkillToMastery(player->pActiveSkills[main_hand_skill]);
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
    attackElement = DMGT_PHISYCAL;
    uDamageAmount = player->CalculateMeleeDamageTo(false, false, pMonster->pMonsterInfo.uID);
    if ( !stru_50C198.PlayerHitOrMiss(player, pMonster, v61, a4) )
    {
      player->PlaySound(SPEECH_52, 0);
      return;
    }
  }
  else
  {
    v61 = projectileSprite->field_60_distance_related_prolly_lod;
    if ( projectileSprite->spell_id != SPELL_DARK_SOULDRINKER )
    {
      std::array<int, 3> distances;
      distances[0] = abs(pParty->vPosition.x - projectileSprite->vPosition.x);
      distances[1] = abs(pParty->vPosition.y - projectileSprite->vPosition.y);
      distances[2] = abs(pParty->vPosition.z - projectileSprite->vPosition.z);
      std::sort(distances.begin(), distances.end());
      v61 = ((unsigned int)(11 * distances[1]) >> 5) + (distances[0] >> 2) + distances[2];

      if ( v61 >= 5120 && !(BYTE1(pMonster->uAttributes) & 4) )
      {
        return;
      }
      else if ( v61 >= 2560 )
      {
        v61 = 2;
      }
      else
      {
        v61 = 1;
      }
    }

    switch (projectileSprite->spell_id)
    {
      case SPELL_LASER_PROJECTILE:
        v16 = player->pActiveSkills[PLAYER_SKILL_BLASTER];
        v61 = 1;
        if ( (signed int)SkillToMastery(v16) >= 3 )
          a4 = player->pActiveSkills[PLAYER_SKILL_BLASTER] & 0x3F;
        attackElement = DMGT_PHISYCAL;
        uDamageAmount = player->CalculateMeleeDamageTo(true, true, 0);
        if ( !stru_50C198.PlayerHitOrMiss(player, pMonster, v61, a4) )
        {
          player->PlaySound(SPEECH_52, 0);
          return;
        }
        break;
      case SPELL_101:
        attackElement = DMGT_FIRE;
        uDamageAmount = player->CalculateRangedDamageTo(0);
        if ( pMonster->pActorBuffs[ACTOR_BUFF_SHIELD].uExpireTime > 0 )
          uDamageAmount >>= 1;
        IsAdditionalDamagePossible = true;
        if ( !stru_50C198.PlayerHitOrMiss(player, pMonster, v61, a4) )
        {
          player->PlaySound(SPEECH_52, 0);
          return;
        }
        break;
      case SPELL_EARTH_BLADES:
        a4 = 5 * projectileSprite->spell_level;
        attackElement = (DAMAGE_TYPE)player->GetSpellSchool(SPELL_EARTH_BLADES);
        uDamageAmount = _43AFE3_calc_spell_damage(39, projectileSprite->spell_level, projectileSprite->spell_skill, pMonster->sCurrentHP);
        if ( pMonster->pActorBuffs[ACTOR_BUFF_SHIELD].uExpireTime > 0 )
          uDamageAmount >>= 1;
        IsAdditionalDamagePossible = false;
        if ( !stru_50C198.PlayerHitOrMiss(player, pMonster, v61, a4) )
        {
          player->PlaySound(SPEECH_52, 0);
          return;
        }
        break;
      case SPELL_EARTH_STUN:
        uDamageAmount = 0;
        attackElement = DMGT_PHISYCAL;
        hit_will_stun = 1;
        if ( !stru_50C198.PlayerHitOrMiss(player, pMonster, v61, a4) )
        {
          player->PlaySound(SPEECH_52, 0);
          return;
        }
        break;
      case SPELL_BOW_ARROW:
        attackElement = DMGT_PHISYCAL;
        uDamageAmount = player->CalculateRangedDamageTo(pMonster->word_000086_some_monster_id);
        if ( pMonster->pActorBuffs[ACTOR_BUFF_SHIELD].uExpireTime > 0 )
          uDamageAmount /= 2;
        IsAdditionalDamagePossible = true;
        if ( projectileSprite->stru_24.uItemID != 0 && projectileSprite->stru_24.uSpecEnchantmentType == 3 )  //of carnage
        {
          attackElement = DMGT_FIRE;
        }
        else if ( !stru_50C198.PlayerHitOrMiss(player, pMonster, v61, a4) )
        {
          player->PlaySound(SPEECH_52, 0);
          return;
        }
        break;

      default:
        attackElement = (DAMAGE_TYPE)player->GetSpellSchool(projectileSprite->spell_id);
        IsAdditionalDamagePossible = false;
        uDamageAmount = _43AFE3_calc_spell_damage(projectileSprite->spell_id, projectileSprite->spell_level, projectileSprite->spell_skill, pMonster->sCurrentHP);
        break;
    }
  }

  if (player->IsWeak())
    uDamageAmount /= 2;
  if ( pMonster->pActorBuffs[ACTOR_BUFF_STONED].uExpireTime > 0 )
    uDamageAmount = 0;
  v61 = stru_50C198.CalcMagicalDamageToActor(pMonster, attackElement, uDamageAmount);
  if ( !projectileSprite && player->IsUnarmed() && player->pPlayerBuffs[PLAYER_BUFF_HAMMERHANDS].uExpireTime > 0 )
  {
    v61 += stru_50C198.CalcMagicalDamageToActor(pMonster, 8, player->pPlayerBuffs[PLAYER_BUFF_HAMMERHANDS].uPower);
  }
  uDamageAmount = v61;
  if ( IsAdditionalDamagePossible )
  {
    if ( projectileSprite )
    {
      a4 = projectileSprite->stru_24._439DF3_get_additional_damage((int*)&attackElement, &isLifeStealing);
      if ( isLifeStealing && pMonster->sCurrentHP > 0 )
      {
        player->sHealth += v61 / 5;
        if ( player->sHealth > player->GetMaxHealth() )
          player->sHealth = player->GetMaxHealth();
      }
      uDamageAmount += stru_50C198.CalcMagicalDamageToActor(pMonster, attackElement, a4);
    }
    else
    {
      for (int i = 0; i < 2; i++)
      {
        if ( player->HasItemEquipped((ITEM_EQUIP_TYPE)i) )
        {
          ItemGen* item;
          if (i == 0)
            item = player->GetOffHandItem();
          else
            item = player->GetMainHandItem();
          a4 = item->_439DF3_get_additional_damage((int*)&attackElement, &isLifeStealing);
          if ( isLifeStealing && pMonster->sCurrentHP > 0 )
          {
            player->sHealth += v61 / 5;
            if ( player->sHealth > player->GetMaxHealth() )
              player->sHealth = player->GetMaxHealth();
          }
          uDamageAmount += stru_50C198.CalcMagicalDamageToActor(pMonster, attackElement, a4);
        }
      }
    }
  }
  pMonster->sCurrentHP -= uDamageAmount;
  if ( uDamageAmount == 0 && !hit_will_stun )
  {
    player->PlaySound(SPEECH_52, 0);
    return;
  }
  if ( pMonster->sCurrentHP > 0 )
  {
    Actor::AI_Stun(uActorID_Monster, a1, 0);
    Actor::AggroSurroundingPeasants(uActorID_Monster, 1);
    if ( bShowDamage )
    {
      if ( projectileSprite )
        sprintfex(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[189], player->pName, pMonster->pActorName, uDamageAmount);// "%s shoots %s for %lu points"
      else
        sprintfex(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[164], player->pName, pMonster->pActorName, uDamageAmount);// "%s hits %s for %lu damage"
      ShowStatusBarString(pTmpBuf.data(), 2u);
    }
  }
  else
  {
    if ( pMonsterStats->pInfos[pMonster->pMonsterInfo.uID].bQuestMonster & 1 )
    {
      if ( pRenderer->pRenderD3D && pGame->uFlags2 & GAME_FLAGS_2_DRAW_BLOODSPLATS )
      {
        v33 = byte_4D864C && pGame->uFlags & 0x80000 ? 10 * pMonster->uActorRadius : pMonster->uActorRadius;
        pDecalBuilder->AddBloodsplat((float)pMonster->vPosition.x, (float)pMonster->vPosition.y, (float)pMonster->vPosition.z, 1.0, 0.0, 0.0, (float)v33, 0, 0);
      }
    }
    Actor::Die(uActorID_Monster);
    Actor::ApplyFineForKillingPeasant(uActorID_Monster);
    Actor::AggroSurroundingPeasants(uActorID_Monster, 1);
    if ( pMonster->pMonsterInfo.uExp )
      pParty->GivePartyExp(pMonsterStats->pInfos[pMonster->pMonsterInfo.uID].uExp);
    v40 = SPEECH_51;
    if ( rand() % 100 < 20 )
      v40 = ((signed int)pMonster->pMonsterInfo.uHP >= 100) + 1;
    player->PlaySound((PlayerSpeech)v40, 0);
    if ( bShowDamage )
    {
      pMonsterName = (char *)uDamageAmount;
      pPlayerName = player->pName;             // "%s inflicts %lu points killing %s"
      sprintfex(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[175], player->pName, uDamageAmount, pMonster);
      ShowStatusBarString(pTmpBuf.data(), 2u);
    }
  }
  if ( pMonster->pActorBuffs[ACTOR_BUFF_PAIN_REFLECTION].uExpireTime > 0
    && uDamageAmount != 0 )
    player->ReceiveDamage(uDamageAmount, attackElement);
  int knockbackValue = 20 * v61 / (signed int)pMonster->pMonsterInfo.uHP;
  if ( (player->GetSpecialItemBonus(24) || hit_will_stun) && stru_50C198.GetMagicalResistance(pMonster, DMGT_EARTH) )
  {
    extraRecoveryTime = 20;
    knockbackValue = 10;
    if ( !pParty->bTurnBasedModeOn )
      extraRecoveryTime = (int)(flt_6BE3A8_debug_recmod2 * 42.66666666666666);
    pMonster->pMonsterInfo.uRecoveryTime += extraRecoveryTime;
    if ( bShowDamage  )
    {
      pMonsterName = player->pName;            // "%s stuns %s"
      sprintfex(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[635], player->pName, pMonster);
      ShowStatusBarString(pTmpBuf.data(), 2u);
    }
  }
  if ( hit_will_paralyze && pMonster->CanAct() && stru_50C198.GetMagicalResistance(pMonster, DMGT_EARTH))
  {
    v43 = player->GetActualSkillLevel(PLAYER_SKILL_MACE);
    v45 = SkillToMastery(v43);
    v46 = pParty->uTimePlayed + (signed int)(signed __int64)((double)(signed int)(7680 * (v43 & 0x3F)) * 0.033333335);
    pMonster->pActorBuffs[ACTOR_BUFF_PARALYZED].Apply(v46, v45, 0, 0, 0);
    if ( bShowDamage )
    {
      pMonsterName = player->pName;        // "%s paralyzes %s"
      sprintfex(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[636], player->pName, pMonster);
      ShowStatusBarString(pTmpBuf.data(), 2u);
    }
  }
  if ( knockbackValue > 10 )
    knockbackValue = 10;
  if ( !MonsterStats::BelongsToSupertype(pMonster->pMonsterInfo.uID, MONSTER_SUPERTYPE_TREANT) )
  {
    pVelocity->x = (unsigned __int64)(knockbackValue * (signed __int64)pVelocity->x) >> 16;
    pVelocity->y = (unsigned __int64)(knockbackValue * (signed __int64)pVelocity->y) >> 16;
    pVelocity->z = (unsigned __int64)(knockbackValue * (signed __int64)pVelocity->z) >> 16;
    pMonster->vVelocity.x = 50 * LOWORD(pVelocity->x);
    pMonster->vVelocity.y = 50 * LOWORD(pVelocity->y);
    pMonster->vVelocity.z = 50 * LOWORD(pVelocity->z);
  }
  Actor::AddBloodsplatOnDamageOverlay(uActorID_Monster, 1, v61);
}
//----- (004BBF61) --------------------------------------------------------
void Actor::_4BBF61_summon_actor( int a1, __int16 x, int y, int z )
{
  Actor *v7; // esi@5
  MonsterInfo *v9; // edi@5
  MonsterDesc *v10; // ebx@5
  int v12; // ebx@7
  int v13; // eax@8
  __int16 v16; // [sp+10h] [bp-4h]@3

  if (uNumActors < 500)
  {
    v16 = 0;
    if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
    {
      v16 = pIndoor->GetSector(x, y, z);
    }
    v7 = &pActors[uNumActors];
    v7->Reset();
    v9 = &pMonsterStats->pInfos[a1];
    v10 = &pMonsterList->pMonsters[a1 - 1];
    strcpy(v7->pActorName, v9->pName);
    v7->sCurrentHP = LOWORD(v9->uHP);
    memcpy(&v7->pMonsterInfo, v9, 0x58u);
    v7->word_000086_some_monster_id = a1;
    v7->uActorRadius = v10->uMonsterRadius;
    v7->uActorHeight = v10->uMonsterHeight;
    v7->uMovementSpeed = v10->uMovementSpeed;
    v7->vInitialPosition.x = x;
    v7->vPosition.x = x;
    v7->uAttributes |= 80000;
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
    for ( int i = 0; i < 4; i++)
    {
      pSoundList->LoadSound(v10->pSoundSampleIDs[i], 0);
    }
    v12 = 0;
    do
    {
      LOWORD(v13) = pSoundList->LoadSound(v12 + word_4EE088_sound_ids[v9->uSpell1ID], 1u);
      v12++;
    }
    while ( v13 );
    ++uNumActors;
  }
}
//----- (00426E10) --------------------------------------------------------
int stru319::which_player_to_attack(Actor *pActor)
{
  signed int v2; // ebx@1
  bool flag; // edi@37
  int v22; // [sp+8h] [bp-140h]@3
  int Victims_list[60]; // [sp+48h] [bp-100h]@48
  int for_sex; // [sp+13Ch] [bp-Ch]@1
  int for_race; // [sp+140h] [bp-8h]@1
  int for_class; // [sp+144h] [bp-4h]@1

  for_class = -1;
  for_race = -1;
  for_sex = -1;
  v2 = 0;
  if ( pActor->pMonsterInfo.uAttackPreference )
  {
    for ( uint i = 0; i < 16; i++ )
    {
      v22 = pActor->pMonsterInfo.uAttackPreference & (1 << i);
      if ( v22 )
      {
        switch ( v22 )
        {
          case 1:
            for_class = 0;
            break;
          case 2:
            for_class = 12;
            break;
          case 4:
            for_class = 16;
            break;
          case 8:
            for_class = 28;
            break;
          case 16:
            for_class = 24;
            break;
          case 32:
            for_class = 32;
            break;
          case 64:
            for_class = 20;
            break;
          case 128:
            for_class = 4;
            break;
          case 256:
            for_class = 8;
            break;
          case 512:
            for_sex = 0;
            break;
          case 1024:
            for_sex = 1;
            break;
          case 2048:
            for_race = 0;
            break;
          case 4096:
            for_race = 1;
            break;
          case 8192:
            for_race = 3;
            break;
          case 16384:
            for_race = 2;
            break;
        }
        v2 = 0;
        for ( uint j = 0; j < 4; ++j )
        {
          flag = 0;
          if ( for_class != -1 && for_class == pPlayers[j + 1]->classType )
            flag = true;
          if ( for_sex != -1 && for_sex == pPlayers[j + 1]->uSex )
            flag = true;
          if ( for_race != -1 && for_race == pPlayers[j + 1]->GetRace() )
            flag = true;
          if ( flag == true )
          {
            if ( !(pPlayers[j + 1]->pConditions[12] | pPlayers[j + 1]->pConditions[13]
                 | pPlayers[j + 1]->pConditions[14] | pPlayers[j + 1]->pConditions[15] | pPlayers[j + 1]->pConditions[16]) )
              Victims_list[v2++] = j;
          }
        }
      }
    }
    if ( v2 )
      return Victims_list[rand() % v2];
  }
  for ( uint i = 0; i < 4; ++i )
  {
    if ( !(pPlayers[i + 1]->pConditions[12] | pPlayers[i + 1]->pConditions[13]
         | pPlayers[i + 1]->pConditions[14] | pPlayers[i + 1]->pConditions[15] | pPlayers[i + 1]->pConditions[16]) )
      Victims_list[v2++] = i;
  }
  if ( v2 )
    return Victims_list[rand() % v2];
  else
    return 0;
}
//----- (0042F4DA) --------------------------------------------------------
bool CheckActors_proximity()
{
  signed int distance; // edi@1
  int for_x; // ebx@5
  int for_y; // [sp+Ch] [bp-10h]@5
  int for_z; // [sp+10h] [bp-Ch]@5
  int v3; // eax@5
  int v4; // ebx@5
  unsigned int v5; // ecx@5
  int v6; // edx@6
  unsigned int v7; // edx@8
  unsigned int v8; // edx@10


  distance = 5120;
  if ( uCurrentlyLoadedLevelType == LEVEL_Indoor)
    distance = 2560;
  
  if ( (signed int)uNumActors <= 0 )
    return false;
  for ( uint i = 0; i < (signed int)uNumActors; ++i )
  {
    for_x = abs(pActors[i].vInitialPosition.x - pParty->vPosition.x);
    for_y = abs(pActors[i].vInitialPosition.y - pParty->vPosition.y);
    for_z = abs(pActors[i].vInitialPosition.z - pParty->vPosition.z);
    v3 = for_x;
    v4 = for_y;
    v5 = for_z;
    if ( v3 < for_y )
    {
      v6 = v3;
      v3 = for_y;
      v4 = v6;
    }
    if ( v3 < for_z )
    {
      v7 = v3;
      v3 = for_z;
      v5 = v7;
    }
    if ( v4 < (signed int)v5 )
    {
      v8 = v5;
      v5 = v4;
      v4 = v8;
    }
    if ( (signed int)(((unsigned int)(11 * v4) >> 5) + (v5 >> 2) + v3) < distance )
    {
      if ( pActors[i].uAIState != Dead )
      {
        if ( pActors[i].uAIState != Dying && pActors[i].uAIState != Removed
          && pActors[i].uAIState != Disabled && pActors[i].uAIState != Summoned
          && (BYTE2(pActors[i].uAttributes) & 8 || pActors[i].GetActorsRelation(0) ) )
          return true;
      }
    }
  }
  return false;
}


//----- (00426A5A) --------------------------------------------------------
void Actor::LootActor()
{
  signed int v2; // edi@1
  unsigned __int8 v7; // al@30
  char *v9; // [sp-4h] [bp-3Ch]@10
  char *v10; // [sp-4h] [bp-3Ch]@31
  char *v11; // [sp-4h] [bp-3Ch]@38
  ItemGen Dst; // [sp+Ch] [bp-2Ch]@1
  bool itemFound; // [sp+30h] [bp-8h]@1
  int v14; // [sp+34h] [bp-4h]@1

  pParty->sub_421B2C_PlaceInInventory_or_DropPickedItem();
  Dst.Reset();
  v2 = 0;
  itemFound = false;
  v14 = 0;
  if ( !( this->uAttributes & 0x800000 ) )
  {
    for (uchar i = 0; i < this->pMonsterInfo.uTreasureDiceRolls; i++ )
    {
        v14 += rand() % this->pMonsterInfo.uTreasureDiceSides + 1;
      }
    if ( v14 != 0 )
      {
        pParty->PartyFindsGold(v14, 0);
        viewparams->bRedrawGameUI = 1;
      }
    }
  else
  {
    if ( this->array_000234[3].uItemID != 0 &&  this->array_000234[3].GetItemEquipType() == EQUIP_GOLD )
    {
      v14 = this->array_000234[3].uSpecEnchantmentType;
      this->array_000234[3].Reset();
      if ( v14 )
      {
        pParty->PartyFindsGold(v14, 0);
        viewparams->bRedrawGameUI = 1;
      }
    }
  }
  if ( this->uCarriedItemID )
  {
    Dst.Reset();
    Dst.uItemID = this->uCarriedItemID;
    v9 = pItemsTable->pItems[Dst.uItemID].pUnidentifiedName;
    if ( v14 )
      sprintfex(pTmpBuf2.data(), pGlobalTXT_LocalizationStrings[490], v14, v9);
    else
      sprintfex(pTmpBuf2.data(), pGlobalTXT_LocalizationStrings[471], v9);
    ShowStatusBarString(pTmpBuf2.data(), 2u);
    if ( Dst.GetItemEquipType() == 12 )
    {
      Dst.uNumCharges = rand() % 6 + Dst.GetDamageMod() + 1;
      Dst.uMaxCharges = Dst.uNumCharges;
    }
    if ( pItemsTable->pItems[Dst.uItemID].uEquipType == 14 && Dst.uItemID != 220 )
      Dst.uEnchantmentType = 2 * rand() % 4 + 2;
    pItemsTable->SetSpecialBonus(&Dst);
    if ( !pParty->AddItemToParty(&Dst) )
      pParty->SetHoldingItem(&Dst);
    this->uCarriedItemID = 0;
    if ( this->array_000234[0].uItemID )
    {
      if ( !pParty->AddItemToParty(this->array_000234) )
      {
        pParty->sub_421B2C_PlaceInInventory_or_DropPickedItem();
        pParty->SetHoldingItem(this->array_000234);
      }
      this->array_000234[0].Reset();
    }
    if ( this->array_000234[1].uItemID )
    {
      if ( !pParty->AddItemToParty(&this->array_000234[1]) )
      {
        pParty->sub_421B2C_PlaceInInventory_or_DropPickedItem();
        pParty->SetHoldingItem(&this->array_000234[1]);
      }
      this->array_000234[1].Reset();
    }
    this->Remove();
    return;
  }
  if ( ( this->uAttributes & 0x800000 ) )
  {
    if ( this->array_000234[3].uItemID )
    {
      memcpy(&Dst, &this->array_000234[3], sizeof(Dst));
      this->array_000234[3].Reset();
      v11 = pItemsTable->pItems[Dst.uItemID].pUnidentifiedName;
      if ( v14 )
        sprintfex(pTmpBuf2.data(), pGlobalTXT_LocalizationStrings[490], v14, v11);
      else
        sprintfex(pTmpBuf2.data(), pGlobalTXT_LocalizationStrings[471], v11);
      ShowStatusBarString(pTmpBuf2.data(), 2u);
      if ( !pParty->AddItemToParty(&Dst) )
        pParty->SetHoldingItem(&Dst);
      itemFound = true;
    }
  }
  else
  {
    if ( rand() % 100 < this->pMonsterInfo.uTreasureDropChance && (v7 = this->pMonsterInfo.uTreasureLevel) != 0 )
    {
  pItemsTable->GenerateItem(v7, this->pMonsterInfo.uTreasureType, &Dst);
  v10 = pItemsTable->pItems[Dst.uItemID].pUnidentifiedName;
  if ( v14 )
    sprintfex(pTmpBuf2.data(), pGlobalTXT_LocalizationStrings[490], v14, v10);//Вы нашли ^I[%d] золот^L[ой;ых;ых] и предмет (%s)!
  else
    sprintfex(pTmpBuf2.data(), pGlobalTXT_LocalizationStrings[471], v10);//Вы нашли ^Pv[%s]!
  ShowStatusBarString(pTmpBuf2.data(), 2);
  if ( !pParty->AddItemToParty(&Dst) )
    pParty->SetHoldingItem(&Dst);
      itemFound = true;
    }
  }
  if ( this->array_000234[0].uItemID )
  {
    if ( !pParty->AddItemToParty(this->array_000234) )
    {
      pParty->sub_421B2C_PlaceInInventory_or_DropPickedItem();
      pParty->SetHoldingItem(this->array_000234);
      itemFound = true;
    }
    this->array_000234[0].Reset();
  }
  if ( this->array_000234[1].uItemID )
  {
    if ( !pParty->AddItemToParty(&this->array_000234[1]) )
    {
      pParty->sub_421B2C_PlaceInInventory_or_DropPickedItem();
      pParty->SetHoldingItem(&this->array_000234[1]);
      itemFound = true;
    }
    this->array_000234[1].Reset();
  }
  if ( !itemFound || rand() % 100 < 90 )
  {
    this->Remove();
  }
}


//----- (00427102) --------------------------------------------------------
bool Actor::_427102_IsOkToCastSpell( signed int a2 )
{
  switch(a2)
  {
  case SPELL_BODY_POWER_CURE:
    {
      if ( this->sCurrentHP >= (signed int)this->pMonsterInfo.uHP )
        return false;
      return true;
    }
  case SPELL_LIGHT_DISPEL_MAGIC:
    {
      for (int i = 0; i < 20; i++)
      {
        if (pParty->pPartyBuffs[i].uExpireTime > 0)
        {
          return true;
        }
      }
      for ( int i = 1; i <= 4; i++ )
      {
        for ( int j = 0; j < 22; j++ )
        {
          if (pPlayers[i]->pPlayerBuffs[j].uExpireTime > 0)
          {
            return true;
          }
        }
      }
      return false;
    }
  case SPELL_LIGHT_DAY_OF_PROTECTION:
    {
      return this->pActorBuffs[ACTOR_BUFF_DAY_OF_PROTECTION].uExpireTime <= 0;
      break;
    }
  case SPELL_LIGHT_HOUR_OF_POWER:
    {
      return this->pActorBuffs[ACTOR_BUFF_HOUR_OF_POWER].uExpireTime <= 0;
      break;
    }
  case SPELL_DARK_PAIN_REFLECTION:
    {
      return this->pActorBuffs[ACTOR_BUFF_PAIN_REFLECTION].uExpireTime <= 0;
      break;
    }
  case SPELL_BODY_HAMMERHANDS:
    {
      return this->pActorBuffs[ACTOR_BUFF_PAIN_HAMMERHANDS].uExpireTime <= 0;
      break;
    }
  case SPELL_FIRE_HASTE:
    {
      return this->pActorBuffs[ACTOR_BUFF_HASTE].uExpireTime <= 0;
      break;
    }
  case SPELL_AIR_SHIELD:
    {
      return this->pActorBuffs[ACTOR_BUFF_SHIELD].uExpireTime <= 0;
      break;
    }
  case SPELL_EARTH_STONESKIN:
    {
      return this->pActorBuffs[ACTOR_BUFF_STONESKIN].uExpireTime <= 0;
      break;
    }
  case SPELL_SPIRIT_BLESS:
    {
      return this->pActorBuffs[ACTOR_BUFF_BLESS].uExpireTime <= 0;
      break;
    }
  case SPELL_SPIRIT_FATE:
    {
      return this->pActorBuffs[ACTOR_BUFF_FATE].uExpireTime <= 0;
      break;
    }
  case SPELL_SPIRIT_HEROISM:
    {
      return this->pActorBuffs[ACTOR_BUFF_HEROISM].uExpireTime <= 0;
      break;
    }
  default:
    {
      return true;
    }
  }
}


//----- (0042704B) --------------------------------------------------------
ABILITY_INDEX Actor::special_ability_use_check( int a2 )
{
  signed int okToCastSpell1; // ebx@5
  signed int okToCastSpell2; // edi@5

  if ( this->pMonsterInfo.uSpecialAbilityType == 2
    && this->pMonsterInfo.uSpecialAbilityDamageDiceBonus < 3u
    && rand() % 100 < 5 )
    this->SummonMinion(a2);
  okToCastSpell1 = this->_427102_IsOkToCastSpell(this->pMonsterInfo.uSpell1ID);
  okToCastSpell2 = this->_427102_IsOkToCastSpell(this->pMonsterInfo.uSpell2ID);
  if ( okToCastSpell1 && this->pMonsterInfo.uSpell1UseChance && rand() % 100 < this->pMonsterInfo.uSpell1UseChance )
    return ABILITY_SPELL1;
  if ( okToCastSpell2 && this->pMonsterInfo.uSpell2UseChance && rand() % 100 < this->pMonsterInfo.uSpell2UseChance )
    return ABILITY_SPELL2;
  if (this->pMonsterInfo.uAttack2Chance && rand() % 100 < this->pMonsterInfo.uAttack2Chance)
    return ABILITY_ATTACK2;
  return ABILITY_ATTACK1;
}



//----- (004273BB) --------------------------------------------------------
bool Actor::_4273BB_DoesHitOtherActor( Actor *defender, int a3, int a4 )
{
  signed int v6; // ebx@1
  signed int v7; // esi@1
  int armorSum; // ebx@10
  signed int a2a; // [sp+18h] [bp+Ch]@1

  v6 = defender->pMonsterInfo.uAC;
  v7 = 0;
  a2a = 0;
  if ( defender->pActorBuffs[ACTOR_BUFF_SOMETHING_THAT_HALVES_AC].uExpireTime > 0 )
    v6 /= 2;
  if ( defender->pActorBuffs[ACTOR_BUFF_HOUR_OF_POWER].uExpireTime > 0 )
    v7 = defender->pActorBuffs[ACTOR_BUFF_HOUR_OF_POWER].uPower;
  if ( defender->pActorBuffs[ACTOR_BUFF_STONESKIN].uExpireTime > 0 && defender->pActorBuffs[ACTOR_BUFF_STONESKIN].uPower > v7 )
    v7 = defender->pActorBuffs[ACTOR_BUFF_STONESKIN].uPower;
  armorSum = v7 + v6;
  if ( this->pActorBuffs[ACTOR_BUFF_HOUR_OF_POWER].uExpireTime > 0 )
    a2a = this->pActorBuffs[ACTOR_BUFF_HOUR_OF_POWER].uPower;
  if ( this->pActorBuffs[ACTOR_BUFF_BLESS].uExpireTime > 0 && this->pActorBuffs[ACTOR_BUFF_BLESS].uPower > a2a )
    a2a = this->pActorBuffs[ACTOR_BUFF_BLESS].uPower;
  if ( this->pActorBuffs[ACTOR_BUFF_FATE].uExpireTime > 0 )
  {
    a2a += this->pActorBuffs[ACTOR_BUFF_FATE].uPower;
    this->pActorBuffs[ACTOR_BUFF_FATE].Reset();
  }
  return rand() % (armorSum + 2 * this->pMonsterInfo.uLevel + 10) + a2a + 1 > armorSum + 5;
}