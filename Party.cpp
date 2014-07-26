#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define _CRT_SECURE_NO_WARNINGS
#include "ErrorHandling.h"
#include "mm7_unsorted_subs.h"
#include "Party.h"
#include "MapInfo.h"
#include "Timer.h"
#include "AudioPlayer.h"
#include "IconFrameTable.h"
#include "Mouse.h"
#include "PlayerFrameTable.h"
#include "TurnEngine.h"
#include "Viewport.h"
#include "Actor.h"
#include "GUIWindow.h"
#include "texts.h"

#include "mm7_data.h"
#include "MM7.h"
#include "Outdoor.h"
#include "Indoor.h"
#include "Texture.h"
#include "LOD.h"
#include "Sprites.h"
#include "SpriteObject.h"
#include "ObjectList.h"
#include "Conditions.h"

Party *pParty; // idb

struct ActionQueue *pPartyActionQueue = new ActionQueue;


std::array<bool, 4> playerAlreadyPicked; // byte_AE3368_
char PickedPlayer2_unused; // byte_AE3369_
char PickedPlayer3_unused; // byte_AE336A_
char PickedPlayer4_unused; // byte_AE336B_



//----- (0044A56A) --------------------------------------------------------
void Party::CountHirelings()
{
  field_70A = 0;

  for (unsigned int i = 0; i < pNPCStats->uNumNewNPCs; ++i)
  {
    NPCData* npc = &pNPCStats->pNewNPCData[i];
    if (npc->Hired() &&
        (!pHirelings[0].pName || strcmp(npc->pName, pHirelings[0].pName)))
    {
      if (!pHirelings[1].pName || strcmp(npc->pName, pHirelings[1].pName))
        ++field_70A;
    }
  }
}


// inlined
//----- (mm6c::004858D0) --------------------------------------------------
void Party::Zero()
{
  uFlags2 = 0;
  uNumGoldInBank = 0;

  uCurrentYear = 0;
  uCurrentMonth = 0;
  uCurrentMonthWeek = 0;
  uDaysPlayed = 0;
  uCurrentHour = 0;
  uCurrentMinute = 0;
  uCurrentTimeSecond = 0;

  field_6FC = 0;
  days_played_without_rest = 0;
  vPosition.x = 0;
  vPosition.y = 0;
  vPosition.z = 0;
  uFallStartY = 0;
  sRotationY = 0;
  sRotationX = 0;
  uFallSpeed = 0;
  field_28 = 0;
  uDefaultPartyHeight = 192;
  field_14_radius = 37;
  y_rotation_granularity = 25;
  y_rotation_speed = 90;

  uWalkSpeed = 384;
  walk_sound_timer = 0;

  field_24 = 5;
  field_6FC = 0;
  field_708 = 15;
  field_0 = 25;

  uNumDeaths = 0;
  uNumPrisonTerms = 0;
  uNumBountiesCollected = 0;
  monster_for_hunting_killed.fill(0);
  monster_id_for_hunting.fill(0);
  memset(_quest_bits, 0, sizeof(_quest_bits));
  pArcomageWins.fill(0);
  uNumArenaPageWins = 0;
  uNumArenaSquireWins = 0;
  uNumArenaKnightWins = 0;
  uNumArenaLordWins = 0;
}

//inlined
//----- (mm6c::0045BE90) --------------------------------------------------
void ActionQueue::Reset()
{
  uNumActions = 0;
}

//----- (004760C1) --------------------------------------------------------
void ActionQueue::Add(PartyAction action)
{
  if (uNumActions < 30)
    pActions[uNumActions++] = action;
}

//----- (00497FC5) --------------------------------------------------------
bool Party::_497FC5_check_party_perception_against_level()
{
  int uMaxPerception; // edi@1
  signed int v5; // eax@3
  bool result; // eax@7

  uMaxPerception = 0;
  for (int i = 0; i < 4; i++)
  {
    if ( this->pPlayers[i].CanAct() )
    {
      v5 = this->pPlayers[i].GetPerception();
      if ( v5 > uMaxPerception )
        uMaxPerception = v5;
    }
  }
  if ( uLevelMapStatsID && (signed int)uLevelMapStatsID < 77 )
    result = uMaxPerception >= 2 * pMapStats->pInfos[uLevelMapStatsID]._per;
  else
    result = 0;
  return result;
}

//----- (004936E1) --------------------------------------------------------
void Party::SetHoldingItem(ItemGen *pItem)
{
  sub_421B2C_PlaceInInventory_or_DropPickedItem();
  memcpy(&pPickedItem, pItem, sizeof(pPickedItem));
  pMouse->SetCursorBitmapFromItemID(pPickedItem.uItemID);
}

//----- (0049370F) --------------------------------------------------------
int Party::GetNextActiveCharacter()
{
  int v2; // eax@4
  signed int v8; // esi@23
  int v12; // [sp+Ch] [bp-4h]@1

  v12 = 0;
  if ( pParty->bTurnBasedModeOn == 1 )
  {
    if ( pTurnEngine->turn_stage != TE_ATTACK || PID_TYPE(pTurnEngine->pQueue[0].uPackedID) != OBJECT_Player)
      return 0;
    v2 = PID_ID(pTurnEngine->pQueue[0].uPackedID);
    return v2 + 1;
  }

  if ( playerAlreadyPicked[0] && playerAlreadyPicked[1] && playerAlreadyPicked[2] && playerAlreadyPicked[3] )
    playerAlreadyPicked.fill(false);
  for (int i = 0; i < 4; i++)
  {
    if ( !this->pPlayers[i].CanAct() || this->pPlayers[i].uTimeToRecovery > 0)
      playerAlreadyPicked[i] = true;
    else if ( !playerAlreadyPicked[i] )
    {
      playerAlreadyPicked[i] = true;
      if (i > 0)          //TODO check if this condition really should be here. it is equal to the original source but still seems kind of weird
        return i + 1;
      break;
    }
  }

  for (int i = 0; i < 4; i++)
  {
    if ( this->pPlayers[i].CanAct() && this->pPlayers[i].uTimeToRecovery == 0 )
    {
      if ( v12 == 0 || this->pPlayers[i].uSpeedBonus > v8 )
      {
        v8 = this->pPlayers[i].uSpeedBonus;
        v12 = i + 1;
      }
    }
  }
  return v12;
}


//----- (00493244) --------------------------------------------------------
bool Party::HasItem(unsigned int uItemID)
{
  for (int player = 0; player < 4; player++)
  {
    for (int itemPos = 0; itemPos < 138; itemPos++)
    {
      if (pParty->pPlayers[player].pOwnItems[itemPos].uItemID == uItemID)
        return true;
    }
  }
  return false;
}


//----- (00492AD5) --------------------------------------------------------
void Party::SetFood(unsigned int uNumFood)
{
  pUIAnim_Food->uAnimTime = 0;
  pParty->uNumFoodRations = uNumFood;
  pUIAnim_Food->uAnimLength = 8 * pIconsFrameTable->pIcons[pUIAnim_Food->uIconID].uAnimLength;
}

//----- (00492B03) --------------------------------------------------------
void Party::TakeFood(unsigned int uNumFood)
{
  if (pParty->uNumFoodRations <= uNumFood)
    pParty->uNumFoodRations = 0;
  else
    pParty->uNumFoodRations -= uNumFood;

  pUIAnim_Food->uAnimTime = 0;
  pUIAnim_Food->uAnimLength = 8 * pIconsFrameTable->pIcons[pUIAnim_Food->uIconID].uAnimLength;
}

//----- (00492B42) --------------------------------------------------------
void Party::GiveFood(unsigned int _this)
{
  pParty->uNumFoodRations += _this;
  pUIAnim_Food->uAnimTime = 0;
  pUIAnim_Food->uAnimLength = 8 * pIconsFrameTable->pIcons[pUIAnim_Food->uIconID].uAnimLength;
}

//----- (00492B70) --------------------------------------------------------
void Party::SetGold(unsigned int uNumGold)
{
  pParty->uNumGold = uNumGold;
  pUIAnim_Gold->uAnimTime = 0;
  pUIAnim_Gold->uAnimLength = 8 * pIconsFrameTable->pIcons[pUIAnim_Gold->uIconID].uAnimLength;
  pAudioPlayer->PlaySound(SOUND_GoldReceived, 0, 0, -1, 0, 0, 0, 0);
}

//----- (00492BB6) --------------------------------------------------------
void Party::TakeGold(unsigned int uNumGold)
{
  if ( uNumGold <= pParty->uNumGold )
    pParty->uNumGold -= uNumGold;
  else
    pParty->uNumGold = 0;
  pUIAnim_Gold->uAnimTime = 0;
  pUIAnim_Gold->uAnimLength = 8 * pIconsFrameTable->pIcons[pUIAnim_Gold->uIconID].uAnimLength;
  pAudioPlayer->PlaySound(SOUND_GoldReceived, 0, 0, -1, 0, 0, 0, 0);
}

//----- (0049135E) --------------------------------------------------------
unsigned int Party::GetPartyFame()
{
  unsigned __int64 total_exp = 0;
  for (uint i = 0; i < 4; ++i)
    total_exp += pPlayers[i].uExperience;
  return (unsigned int)(min(total_exp / 1000, UINT_MAX));     //min wasn't present, but could be incorrect without it
}

//----- (0049137D) --------------------------------------------------------
void Party::CreateDefaultParty(char bGiveItems)
{
  Player *pCharacter; // esi@3
  int uSkillIdx; // eax@11
  unsigned int v16; // [sp-4h] [bp-44h]@26
  signed int uNumPlayers; // [sp+18h] [bp-28h]@1
  ItemGen Dst; // [sp+1Ch] [bp-24h]@10

  pHireling1Name[0] = 0;
  pHireling2Name[0] = 0;
  this->hirelingScrollPosition = 0;
  memset(&pHirelings, 0, sizeof(pHirelings));

  strcpy(this->pPlayers[0].pName, pGlobalTXT_LocalizationStrings[509]); //Zoltan
  this->pPlayers[0].uPrevFace = 17;
  this->pPlayers[0].uCurrentFace = 17;
  this->pPlayers[0].uPrevVoiceID = 17;
  this->pPlayers[0].uVoiceID = 17;
  this->pPlayers[0].uMight = 30;
  this->pPlayers[0].uIntelligence = 5;
  this->pPlayers[0].uWillpower = 5;
  this->pPlayers[0].uEndurance = 13;
  this->pPlayers[0].uAccuracy = 13;
  this->pPlayers[0].uSpeed = 14;
  this->pPlayers[0].uLuck = 7;
  this->pPlayers[0].pActiveSkills[PLAYER_SKILL_LEATHER] = 1;         // leather
  this->pPlayers[0].pActiveSkills[PLAYER_SKILL_ARMSMASTER] = 1;        // armsmaster
  this->pPlayers[0].pActiveSkills[PLAYER_SKILL_BOW] = 1;         // bow
  this->pPlayers[0].pActiveSkills[PLAYER_SKILL_SWORD] = 1;         // sword
  this->pPlayers[1].uPrevFace = 3;
  this->pPlayers[1].uCurrentFace = 3;
  this->pPlayers[1].uPrevVoiceID = 3;
  this->pPlayers[1].uVoiceID = 3;
  strcpy(this->pPlayers[1].pName, pGlobalTXT_LocalizationStrings[506]); //Roderic
  this->pPlayers[1].uMight = 13;
  this->pPlayers[1].uIntelligence = 9;
  this->pPlayers[1].uWillpower = 9;
  this->pPlayers[1].uEndurance = 13;
  this->pPlayers[1].uAccuracy = 13;
  this->pPlayers[1].uSpeed = 13;
  this->pPlayers[1].uLuck = 13;
  this->pPlayers[1].pActiveSkills[PLAYER_SKILL_LEATHER] = 1;         // leather
  this->pPlayers[1].pActiveSkills[PLAYER_SKILL_STEALING] = 1;        // stealing
  this->pPlayers[1].pActiveSkills[PLAYER_SKILL_DAGGER] = 1;         // dagger
  this->pPlayers[1].pActiveSkills[PLAYER_SKILL_TRAP_DISARM] = 1;        // disarm trap
  this->pPlayers[2].uPrevFace = 14;
  this->pPlayers[2].uCurrentFace = 14;
  this->pPlayers[2].uPrevVoiceID = 14;
  this->pPlayers[2].uVoiceID = 14;
  strcpy(this->pPlayers[2].pName, pGlobalTXT_LocalizationStrings[508]); // Serena
  this->pPlayers[2].uMight = 12;
  this->pPlayers[2].uIntelligence = 9;
  this->pPlayers[2].uWillpower = 20;
  this->pPlayers[2].uEndurance = 22;
  this->pPlayers[2].uAccuracy = 7;
  this->pPlayers[2].uSpeed = 13;
  this->pPlayers[2].uLuck = 7;
  this->pPlayers[2].pActiveSkills[PLAYER_SKILL_ALCHEMY] = 1;        // alchemy
  this->pPlayers[2].pActiveSkills[PLAYER_SKILL_LEATHER] = 1;         // leather
  this->pPlayers[2].pActiveSkills[PLAYER_SKILL_BODY] = 1;        // body
  this->pPlayers[2].pActiveSkills[PLAYER_SKILL_MACE] = 1;         // mace
  strcpy(this->pPlayers[3].pName, pGlobalTXT_LocalizationStrings[507]); // Alexis
  this->pPlayers[3].uPrevFace = 10;
  this->pPlayers[3].uCurrentFace = 10;
  this->pPlayers[3].uEndurance = 13;
  this->pPlayers[3].uAccuracy = 13;
  this->pPlayers[3].uSpeed = 13;
  this->pPlayers[3].uPrevVoiceID = 10;
  this->pPlayers[3].uVoiceID = 10;
  this->pPlayers[3].uMight = 5;
  this->pPlayers[3].uIntelligence = 30;
  this->pPlayers[3].uWillpower = 9;
  this->pPlayers[3].uLuck = 7;
  this->pPlayers[3].pActiveSkills[PLAYER_SKILL_LEATHER] = 1;         // leather
  this->pPlayers[3].pActiveSkills[PLAYER_SKILL_AIR] = 1;        // air
  this->pPlayers[3].pActiveSkills[PLAYER_SKILL_FIRE] = 1;        // fire
  this->pPlayers[3].pActiveSkills[PLAYER_SKILL_STAFF] = 1;         // staff
  for (uNumPlayers = 0; uNumPlayers < 4; uNumPlayers++)
  {
    pCharacter = &pParty->pPlayers[uNumPlayers];
    if (pCharacter->classType == PLAYER_CLASS_KNIGHT)
      pCharacter->sResMagicBase = 10; //player[i].pResMagicBase
    pCharacter->lastOpenedSpellbookPage = 0;
    for (int i = 0; i < 9; i++)//for Magic Book
    {
	    if (pPlayers[uNumPlayers].pActiveSkills[12+i])
      {
        pCharacter->lastOpenedSpellbookPage = i;
        break;
      }
    }
    pCharacter->uExpressionTimePassed = 0;
    Dst.Reset();
    if ( bGiveItems )
    {
      pItemsTable->GenerateItem(2, 40, &Dst); //ring
      pCharacter->AddItem2(-1, &Dst);
	    for (uSkillIdx = 0; uSkillIdx < 36; uSkillIdx++)
      {
        if ( pCharacter->pActiveSkills[uSkillIdx] )
        {
          switch ( uSkillIdx )
          {
            case PLAYER_SKILL_STAFF:
              pCharacter->WearItem(ITEM_STAFF_1);
              break;
            case PLAYER_SKILL_SWORD:
              pCharacter->WearItem(ITEM_LONGSWORD_1);
              break;
            case PLAYER_SKILL_DAGGER:
              pCharacter->WearItem(ITEM_DAGGER_1);
              break;
            case PLAYER_SKILL_AXE:
              pCharacter->WearItem(ITEM_AXE_1);
              break;
            case PLAYER_SKILL_SPEAR:
              pCharacter->WearItem(ITEM_SPEAR_1);
              break;
            case PLAYER_SKILL_BOW:
              pCharacter->WearItem(ITEM_CROSSBOW_1);
              break;
            case PLAYER_SKILL_MACE:
              pCharacter->WearItem(ITEM_MACE_1);
              break;
            case PLAYER_SKILL_SHIELD:
              pCharacter->WearItem(ITEM_BUCKLER_1);
              break;
            case PLAYER_SKILL_LEATHER:
              pCharacter->WearItem(ITEM_LEATHER_1);
              break;
            case PLAYER_SKILL_CHAIN:
              pCharacter->WearItem(ITEM_CHAINMAIL_1);
              break;
            case PLAYER_SKILL_PLATE:
              pCharacter->WearItem(ITEM_PLATE_1);
              break;
            case PLAYER_SKILL_FIRE:
              pCharacter->AddItem(-1, ITEM_SPELLBOOK_FIRE_STRIKE);
              break;
            case PLAYER_SKILL_AIR:
              pCharacter->AddItem(-1, ITEM_SPELLBOOK_AIR_FEATHER_FALL);
              break;
            case PLAYER_SKILL_WATER:
              pCharacter->AddItem(-1, ITEM_SPELLBOOK_WATER_POISON_SPRAY);
              break;
            case PLAYER_SKILL_EARTH:
              pCharacter->AddItem(-1, ITEM_SPELLBOOK_EARTH_SLOW);
              break;
            case PLAYER_SKILL_SPIRIT:
              pCharacter->AddItem(-1, ITEM_SPELLBOOK_SPIRIT_BLESS);
              break;
            case PLAYER_SKILL_MIND:
              pCharacter->AddItem(-1, ITEM_SPELLBOOK_MIND_MIND_BLAST);
              break;
            case PLAYER_SKILL_BODY:
              pCharacter->AddItem(-1, ITEM_SPELLBOOK_BODY_FIRST_AID);
              break;
            case PLAYER_SKILL_ITEM_ID:
            case PLAYER_SKILL_REPAIR:
            case PLAYER_SKILL_MEDITATION:
            case PLAYER_SKILL_PERCEPTION:
            case PLAYER_SKILL_DIPLOMACY:
            case PLAYER_SKILL_TRAP_DISARM:
            case PLAYER_SKILL_LEARNING:
              pCharacter->AddItem(-1, ITEM_POTION_BOTTLE);
              v16 = 5 * (rand() % 3 + 40);      // simple reagent
              pCharacter->AddItem(-1, v16);
              break;
            case PLAYER_SKILL_DODGE:
              pCharacter->AddItem(-1, ITEM_BOOTS_1);
              break;
            case PLAYER_SKILL_UNARMED:
              pCharacter->AddItem(-1, ITEM_GAUNTLETS_1);
              break;
            default:
              break;
          }
        }
      }
      for (int i = 0; i < 138; i++)
      {
	      if ( pCharacter->pInventoryItemList[i].uItemID != 0)
          pCharacter->pInventoryItemList[i].SetIdentified();
      }
    }
    pCharacter->sHealth = pCharacter->GetMaxHealth();
    pCharacter->sMana = pCharacter->GetMaxMana();
  }
}

//----- (004917CE) --------------------------------------------------------
int Party::Reset()
{  
  Zero();

  field_708 = 15;
  sEyelevel = 160;
  uNumGold = 200;
  uNumFoodRations = 7;

  
  alignment = PartyAlignment_Neutral;
  SetUserInterface(alignment, true);

  uTimePlayed = 0x21C00u;
  uLastRegenerationTime = 0x21C00;

  bTurnBasedModeOn = false;

  uActiveCharacter = 1;
  ::pPlayers.ZerothIndex() = &pPlayers[0];
  for (uint i = 0; i < 4; ++i)
    ::pPlayers[i + 1] = &pPlayers[i];

  pPlayers[0].Reset(PLAYER_CLASS_KNIGHT);
  pPlayers[1].Reset(PLAYER_CLASS_THEIF);
  pPlayers[2].Reset(PLAYER_CLASS_CLERIC);
  pPlayers[3].Reset(PLAYER_CLASS_SORCERER);
  pPlayers[0].uCurrentFace = 17;
  pPlayers[0].uPrevVoiceID = 17;
  pPlayers[0].uVoiceID = 17;
  pPlayers[0].SetInitialStats();

  pPlayers[0].uSex = pPlayers[0].GetSexByVoice();
  pPlayers[0].RandomizeName();
  strcpy(pPlayers[0].pName, pGlobalTXT_LocalizationStrings[509]);


  pPlayers[1].uCurrentFace = 3;
  pPlayers[1].uPrevVoiceID = 3;
  pPlayers[1].uVoiceID = 3;
  pPlayers[1].SetInitialStats();
  pPlayers[1].uSex = pPlayers[1].GetSexByVoice();
  pPlayers[1].RandomizeName();
  strcpy(pPlayers[1].pName, pGlobalTXT_LocalizationStrings[506]);
  pPlayers[2].uCurrentFace = 14;
  pPlayers[2].uPrevVoiceID = 14;
  pPlayers[2].uVoiceID = 14;
  pPlayers[2].SetInitialStats();
  pPlayers[2].uSex = pPlayers[3].GetSexByVoice();
  pPlayers[2].RandomizeName();
  strcpy(pPlayers[2].pName, pGlobalTXT_LocalizationStrings[508]);
  pPlayers[3].uCurrentFace = 10;
  pPlayers[3].uPrevVoiceID = 10;
  pPlayers[3].uVoiceID = 10;
  pPlayers[3].SetInitialStats();
  pPlayers[3].uSex = pPlayers[3].GetSexByVoice();
  pPlayers[3].RandomizeName();
  strcpy(pPlayers[3].pName, pGlobalTXT_LocalizationStrings[507]);
  
  for (uint i = 0; i < 4; ++i)
  {
    pPlayers[i].uTimeToRecovery = 0;
    for (uint j = 0; j < 20; ++j)
      pPlayers[i].pConditions[j] = 0;

    for (uint j = 0; j < 24; ++j)
      pPlayers[i].pPlayerBuffs[j].Reset();

    pPlayers[i].expression = CHARACTER_EXPRESSION_1;
    pPlayers[i].uExpressionTimePassed = 0;
    pPlayers[i].uExpressionTimeLength = rand() % 256 + 128;
  }

  for (uint i = 1; i < 20; ++i)
    pPartyBuffs[i].Reset();


  pWindowList_at_506F50_minus1_indexing_buttons____and_an_int_[0] = 100;  // default character ui - stats
  uFlags = 0;
  memset(_autonote_bits, 0, sizeof(_autonote_bits));
  memset(_quest_bits, 0, sizeof(_quest_bits));
  pIsArtifactFound.fill(0);
  _449B7E_toggle_bit(_quest_bits, PARTY_QUEST_EMERALD_RED_POTION_ACTIVE, 1);
  _449B7E_toggle_bit(_quest_bits, PARTY_QUEST_EMERALD_SEASHELL_ACTIVE, 1);
  _449B7E_toggle_bit(_quest_bits, PARTY_QUEST_EMERALD_LONGBOW_ACTIVE, 1);
  _449B7E_toggle_bit(_quest_bits, PARTY_QUEST_EMERALD_PLATE_ACTIVE, 1);
  _449B7E_toggle_bit(_quest_bits, PARTY_QUEST_EMERALD_LUTE_ACTIVE, 1);
  _449B7E_toggle_bit(_quest_bits, PARTY_QUEST_EMERALD_HAT_ACTIVE, 1);

  PartyTimes._shop_ban_times.fill(0);

  memcpy(pNPCStats->pNewNPCData, pNPCStats->pNPCData, 0x94BCu);
  memcpy(pNPCStats->pGroups_copy, pNPCStats->pGroups, 0x66u);
  pNPCStats->pNewNPCData[3].uFlags |= 128;//|= 0x80u; Lady Margaret
  _494035_timed_effects__water_walking_damage__etc();
  pEventTimer->Pause();
  return 0;
}


//----- (0043AD34) --------------------------------------------------------
void Party::Yell()
{
  Actor *v0; // esi@5
  int v1; // edi@9
  int v2; // ebx@9
  int v3; // eax@9

  if ( (signed __int64)pParty->pPartyBuffs[PARTY_BUFF_INVISIBILITY].uExpireTime > 0 )
    pParty->pPartyBuffs[PARTY_BUFF_INVISIBILITY].Reset();
  if ( pParty->bTurnBasedModeOn != 1 )
  {
    for (unsigned int i = 0; i < uNumActors; i++)
    {
      v0 = &pActors[i];
      if ( v0->Actor::CanAct() && v0->pMonsterInfo.uHostilityType != MonsterInfo::Hostility_Long && v0->pMonsterInfo.uMovementType != 5 )
      {
        v1 = abs(v0->vPosition.x - pParty->vPosition.x);
        v2 = abs(v0->vPosition.y - pParty->vPosition.y);
        v3 = abs(v0->vPosition.z - pParty->vPosition.z);
        if (int_get_vector_length(v1, v2, v3) < 512)
          Actor::AI_Flee(i, 4, 0, 0);
      }
    }
  }
}

//----- (00491BF9) --------------------------------------------------------
void Party::ResetPosMiscAndSpellBuffs()
{
  this->vPosition.y = 0;
  this->vPosition.z = 0;
  this->vPosition.x = 0;
  this->uFallStartY = 0;
  this->sRotationY = 0;
  this->sRotationX = 0;
  this->uFallSpeed = 0;
  this->field_28 = 0;
  this->uDefaultPartyHeight = 120;
  this->field_14_radius = 37;
  this->y_rotation_granularity = 25;
  this->uWalkSpeed = 384;
  this->y_rotation_speed = 90;
  this->field_24 = 5;
  this->field_6FC = 0;
  this->field_708 = 15;
  this->field_0 = 25;

  for (int playerId = 0; playerId < 4; playerId++)
  {
    for (int buffId = 0; buffId < 24; buffId++)
    {
      this->pPlayers[playerId].pPlayerBuffs[buffId].Reset();
    }
  }
  for (int buffId = 0; buffId < 20; buffId++)
  {
    this->pPartyBuffs[buffId].Reset();
  }
}

//----- (004909F4) --------------------------------------------------------
void Party::UpdatePlayersAndHirelingsEmotions()
{
  int v4; // edx@27

  for (int i = 0; i < 4; ++i)
  {
    Player* player = &pPlayers[i];
    player->uExpressionTimePassed += (unsigned short)pMiscTimer->uTimeElapsed;

    uint condition = player->GetMajorConditionIdx();
    if (condition == Condition_Good || condition == Condition_Zombie)
    {
      if (player->uExpressionTimePassed < player->uExpressionTimeLength)
        continue;
        
      player->uExpressionTimePassed = 0;
      if (player->expression != 1 || rand() % 5)
      {
        player->expression = CHARACTER_EXPRESSION_1;
        player->uExpressionTimeLength = rand() % 256 + 32;
      }
      else
      {
        v4 = rand() % 100;
        if (v4 < 25)      player->expression = CHARACTER_EXPRESSION_13;
        else if (v4 < 31) player->expression = CHARACTER_EXPRESSION_14;
        else if (v4 < 37) player->expression = CHARACTER_EXPRESSION_15;
        else if (v4 < 43) player->expression = CHARACTER_EXPRESSION_16;
        else if (v4 < 46) player->expression = CHARACTER_EXPRESSION_17;
        else if (v4 < 52) player->expression = CHARACTER_EXPRESSION_18;
        else if (v4 < 58) player->expression = CHARACTER_EXPRESSION_19;
        else if (v4 < 64) player->expression = CHARACTER_EXPRESSION_20;
        else if (v4 < 70) player->expression = CHARACTER_EXPRESSION_54;
        else if (v4 < 76) player->expression = CHARACTER_EXPRESSION_55;
        else if (v4 < 82) player->expression = CHARACTER_EXPRESSION_56;
        else if (v4 < 88) player->expression = CHARACTER_EXPRESSION_57;
        else if (v4 < 94) player->expression = CHARACTER_EXPRESSION_29;
        else              player->expression = CHARACTER_EXPRESSION_30;
      }

      for (unsigned int j = 0; j < pPlayerFrameTable->uNumFrames; ++j)
      {
        PlayerFrame* frame = &pPlayerFrameTable->pFrames[j];
        if (frame->expression == player->expression)
        {
          player->uExpressionTimeLength = 8 * frame->uAnimLength;
          break;
        }
      }
    }
    else if (player->expression != CHARACTER_EXPRESSION_DMGRECVD_MINOR &&
             player->expression != CHARACTER_EXPRESSION_DMGRECVD_MODERATE &&
             player->expression != CHARACTER_EXPRESSION_DMGRECVD_MAJOR ||
             player->uExpressionTimePassed >= player->uExpressionTimeLength)
    {
      player->uExpressionTimeLength = 0;
      player->uExpressionTimePassed = 0;

      switch (condition)
      {
        case Condition_Dead:        player->expression = CHARACTER_EXPRESSION_DEAD;      break;
        case Condition_Pertified:   player->expression = CHARACTER_EXPRESSION_PERTIFIED; break;
        case Condition_Eradicated:  player->expression = CHARACTER_EXPRESSION_ERADICATED; break;
        case Condition_Cursed:      player->expression = CHARACTER_EXPRESSION_CURSED; break;
        case Condition_Weak:        player->expression = CHARACTER_EXPRESSION_WEAK; break;
        case Condition_Sleep:       player->expression = CHARACTER_EXPRESSION_SLEEP; break;
        case Condition_Fear:        player->expression = CHARACTER_EXPRESSION_FEAR; break;
        case Condition_Drunk:       player->expression = CHARACTER_EXPRESSION_DRUNK; break;
        case Condition_Insane:      player->expression = CHARACTER_EXPRESSION_INSANE; break;
        case Condition_Poison_Weak:
        case Condition_Poison_Medium:
        case Condition_Poison_Severe:     player->expression = CHARACTER_EXPRESSION_POISONED; break;
        case Condition_Disease_Weak:
        case Condition_Disease_Medium:
        case Condition_Disease_Severe:    player->expression = CHARACTER_EXPRESSION_DISEASED; break;
        case Condition_Paralyzed:   player->expression = CHARACTER_EXPRESSION_PARALYZED; break;
        case Condition_Unconcious:  player->expression = CHARACTER_EXPRESSION_UNCONCIOUS; break;
        default:
          Error("Invalid condition: %u", condition);
      }
    }
  }

  for (int i = 0; i < 2; ++i)
  {
    NPCData* hireling = &pParty->pHirelings[i];
    if (!hireling->evt_C)
      continue;

    hireling->evt_B += pMiscTimer->uTimeElapsed;
    if (hireling->evt_B >= hireling->evt_C)
    {
      hireling->evt_A = 0;
      hireling->evt_B = 0;
      hireling->evt_C = 0;

      Assert(sizeof(NPCData) == 0x4C);
      memset(hireling, 0, sizeof(*hireling));

      pParty->hirelingScrollPosition = 0;
      pParty->CountHirelings();
      viewparams->bRedrawGameUI = true;
    }
  }
}

//----- (00490D02) --------------------------------------------------------
void Party::RestAndHeal()
{
  Player *pPlayer; // esi@4
  bool have_vessels_soul; // [sp+10h] [bp-8h]@10

  for ( uint i = 0; i < 20; ++i )
    pParty->pPartyBuffs[i].Reset();

  for ( int pPlayerID = 0; pPlayerID < 4; ++pPlayerID )
  {
    pPlayer = &pParty->pPlayers[pPlayerID];
    for ( uint i = 0; i < 20; ++i )
      pPlayer->pPlayerBuffs[i].Reset();

    pPlayer->Zero();
    if ( pPlayer->pConditions[Condition_Dead] || pPlayer->pConditions[Condition_Pertified] || pPlayer->pConditions[Condition_Eradicated] )//Dead/Petrified/Eradicated
      continue;
    pPlayer->pConditions[Condition_Unconcious] = 0;//Unconcious
    pPlayer->pConditions[Condition_Drunk] = 0;//Drunk
    pPlayer->pConditions[Condition_Fear] = 0;//Fear
    pPlayer->pConditions[Condition_Sleep] = 0;//Sleep
    pPlayer->pConditions[Condition_Weak] = 0;//Weak
    pPlayer->uTimeToRecovery = 0;
    pPlayer->sHealth = pPlayer->GetMaxHealth();
    pPlayer->sMana = pPlayer->GetMaxMana();
    if ( pPlayer->classType == PLAYER_CLASS_LICH )
    {
      have_vessels_soul = false;
      for ( uint i = 0; i < 126; i++ )
      {
        if ( pPlayer->pInventoryItemList[i].uItemID == ITEM_LICH_JAR_FULL && pPlayer->pInventoryItemList[i].uHolderPlayer == pPlayerID + 1 )
          have_vessels_soul = true;
      }
      if ( !have_vessels_soul )
      {
        pPlayer->sHealth = pPlayer->GetMaxHealth() / 2;
        pPlayer->sMana = pPlayer->GetMaxMana() / 2;
      }
    }

    if (pPlayer->pConditions[Condition_Zombie])
    {
      pPlayer->sMana = 0;
      pPlayer->sHealth /= 2;
    }
    else if ( pPlayer->pConditions[Condition_Poison_Severe] || pPlayer->pConditions[Condition_Disease_Severe] )
    {
      pPlayer->sHealth /= 4;
      pPlayer->sMana /= 4;
    }
    else if ( pPlayer->pConditions[Condition_Poison_Medium] || pPlayer->pConditions[Condition_Disease_Medium] )
    {
      pPlayer->sHealth /= 3;
      pPlayer->sMana /= 3;
    }
    else if ( pPlayer->pConditions[Condition_Poison_Weak] || pPlayer->pConditions[Condition_Disease_Weak] )
    {
      pPlayer->sHealth /= 2;
      pPlayer->sMana /=  2;
    }
    if ( pPlayer->pConditions[Condition_Insane] )
      pPlayer->sMana = 0;
    UpdatePlayersAndHirelingsEmotions();
  }
  pParty->days_played_without_rest = 0;
}

//----- (004938D1) --------------------------------------------------------
void __fastcall Rest(unsigned int uHoursToSleep)
{
  signed __int64 v2; // st7@3

  if ( uHoursToSleep > 240 )
    Actor::InitializeActors();
  v2 = (signed __int64)((7680 * uHoursToSleep) * 0.033333335);
  pParty->uTimePlayed += v2;
  for (int i = 1; i <= 4; i++)
  {
    pPlayers[i]->Recover((int)v2);
  }
  _494035_timed_effects__water_walking_damage__etc();
}
//----- (004B1BDB) --------------------------------------------------------
void RestAndHeal(__int64 uNumMinutes)
{
  signed __int64 v1; // ST2C_8@1
  signed __int64 v2; // qax@1
  unsigned __int64 v4; // qax@1
  unsigned int v5; // ebx@1

  pParty->pHirelings[0].bHasUsedTheAbility = 0;
  pParty->pHirelings[1].bHasUsedTheAbility = 0;
  pParty->uTimePlayed += (signed __int64)((double)(7680 * uNumMinutes) * 0.033333335);
  v1 = (signed __int64)((double)(signed __int64)pParty->uTimePlayed * 0.234375);
  v2 = v1 / 60 / 60;
  v4 = (unsigned int)v2 / 0x18;
  v5 = (unsigned int)(v4 / 7) >> 2;
  pParty->uCurrentTimeSecond = v1 % 60;
  pParty->uCurrentMinute = v1 / 60 % 60;
  pParty->uCurrentHour = v2 % 24;
  pParty->uCurrentMonthWeek = v4 / 7 & 3;
  pParty->uDaysPlayed = (unsigned int)v4 % 0x1C;
  pParty->uCurrentMonth = v5 % 0xC;
  pParty->uCurrentYear = v5 / 0xC + game_starting_year;
  pParty->RestAndHeal();

  for (int i = 0; i < 4; i++)
  {
    pParty->pPlayers[i].uTimeToRecovery = 0;
    pParty->pPlayers[i].uNumDivineInterventionCastsThisDay = 0;
    pParty->pPlayers[i].uNumArmageddonCasts = 0;
    pParty->pPlayers[i].uNumFireSpikeCasts = 0;
    pParty->pPlayers[i].field_1B3B = 0;
  }
  pParty->UpdatePlayersAndHirelingsEmotions();
}
//----- (0041F5BE) --------------------------------------------------------
void  Party::Sleep6Hours()
{
  if ( _506F18_num_minutes_to_sleep < 6 )
  {
    if ( _506F18_num_minutes_to_sleep )
    {
      Rest(_506F18_num_minutes_to_sleep);
      _506F18_num_minutes_to_sleep = 0;
      OutdoorLocation::LoadActualSkyFrame();
    }
    if ( dword_506F14 == 2 )
    {
      pMessageQueue_50CBD0->AddGUIMessage(UIMSG_Escape, 0, 0);
    }
  }
  else
  {
    Rest(6u);
    _506F18_num_minutes_to_sleep -= 6;
    OutdoorLocation::LoadActualSkyFrame();
  }
  viewparams->bRedrawGameUI = 1;
}

bool TestPartyQuestBit( PARTY_QUEST_BITS bit )
{
  return _449B57_test_bit(pParty->_quest_bits, bit);
}

//----- (0047752B) --------------------------------------------------------
int Party::GetPartyReputation()
{
  DDM_DLV_Header *v0; // ebx@1
  signed int v1; // esi@3

  v0 = &pOutdoor->ddm;
  if ( uCurrentlyLoadedLevelType != LEVEL_Outdoor )
    v0 = &pIndoor->dlv;
  v1 = 0;
  if ( CheckHiredNPCSpeciality(Pirate) )
    v1 += 5;
  if ( CheckHiredNPCSpeciality(Burglar) )
    v1 += 5;
  if ( CheckHiredNPCSpeciality(Gypsy) )
    v1 += 5;
  if ( CheckHiredNPCSpeciality(Duper) )
    v1 += 5;
  if ( CheckHiredNPCSpeciality(FallenWizard) )
    v1 += 5;
  return v1 + v0->uReputation;
}
//----- (004269A2) --------------------------------------------------------
void Party::GivePartyExp(unsigned int pEXPNum)
{
  signed int pActivePlayerCount; // ecx@1
  int pLearningPercent; // eax@13

  if ( pEXPNum > 0)
  {
    pActivePlayerCount = 0;
    for ( uint i = 0; i < 4; ++i )
    {
      if ( !pParty->pPlayers[i].pConditions[Condition_Unconcious] && 
        !pParty->pPlayers[i].pConditions[Condition_Dead] && 
        !pParty->pPlayers[i].pConditions[Condition_Pertified] && 
        !pParty->pPlayers[i].pConditions[Condition_Eradicated] )
        pActivePlayerCount ++;
    }
    if ( pActivePlayerCount )
    {
      pEXPNum = pEXPNum / pActivePlayerCount;
      for ( uint i = 0; i < 4; ++i )
      {
        if ( !pParty->pPlayers[i].pConditions[Condition_Unconcious] && 
          !pParty->pPlayers[i].pConditions[Condition_Dead] && 
          !pParty->pPlayers[i].pConditions[Condition_Pertified] && 
          !pParty->pPlayers[i].pConditions[Condition_Eradicated] )
        {
          pLearningPercent = pParty->pPlayers[i].GetLearningPercent();
          pEXPNum = pEXPNum + pEXPNum * pLearningPercent / 100;
          pParty->pPlayers[i].uExperience += pEXPNum;
          if ( pParty->pPlayers[i].uExperience > 4000000000i64 )
          {
            pParty->pPlayers[i].uExperience = 0;
          }
        }
      }
    }
  }
}
//----- (00420C05) --------------------------------------------------------
void Party::PartyFindsGold(unsigned int uNumGold, int _1_dont_share_with_followers___2_the_same_but_without_a_message__else_normal)
{
  int hirelingSalaries; // ebp@1
  unsigned int goldToGain; // esi@1
  NPCData *v12; // ecx@21
  unsigned int v13; // ecx@23
  signed int hirelingCount; // [sp+Ch] [bp-4h]@6

  hirelingSalaries = 0;
  goldToGain = uNumGold;

  if ( _1_dont_share_with_followers___2_the_same_but_without_a_message__else_normal == 2 )
    pTmpBuf2[0] = 0;
  else if ( _1_dont_share_with_followers___2_the_same_but_without_a_message__else_normal == 1 )
  {
    sprintf(pTmpBuf2.data(), pGlobalTXT_LocalizationStrings[467], uNumGold);// You found %lu gold!
  } 
  else
  {
    hirelingCount = 0;
    for (int i = 0; i < 2; i++)
    {
      if (this->pHirelings[i].pName)
      {
        hirelingCount++;
        pTmpBuf[hirelingCount] = i;
      }
    }
    for (uint i = 0; i < pNPCStats->uNumNewNPCs; i++)
    {
      if ( pNPCStats->pNewNPCData[i].uFlags & 0x80
        && (!this->pHirelings[0].pName || strcmp(pNPCStats->pNewNPCData[i].pName, this->pHirelings[0].pName))
        && (!this->pHirelings[1].pName || strcmp(pNPCStats->pNewNPCData[i].pName, this->pHirelings[1].pName)) )
      {
        hirelingCount++;
        pTmpBuf[hirelingCount] = i + 2;
      }
    }
    for (int i = 0; i < hirelingCount; i++)
    {
      uchar thisBufId = (uchar)pTmpBuf[i];
      if (thisBufId < 2)
        v12 = &this->pHirelings[thisBufId];
      else
        v12 = &pNPCStats->pNPCData[thisBufId + 499];
      v13 = v12->uProfession;
      if ( v13 )
        hirelingSalaries += pNPCStats->pProfessions[v13].uHirePrice;//*(&pNPCStats->field_13A58 + 5 * v13);
    }
    if ( CheckHiredNPCSpeciality(Factor) )
      goldToGain += (signed int)(10 * goldToGain) / 100;
    if ( CheckHiredNPCSpeciality(Banker) )
      goldToGain += (signed int)(20 * goldToGain) / 100;
    if ( CheckHiredNPCSpeciality(Pirate) )
      goldToGain += (signed int)(10 * goldToGain) / 100;
    if ( hirelingSalaries )
    {
      hirelingSalaries = (signed int)(goldToGain * hirelingSalaries / 100) / 100;
      if ( hirelingSalaries < 1 )
        hirelingSalaries = 1;
      sprintf(pTmpBuf2.data(), pGlobalTXT_LocalizationStrings[466], goldToGain, hirelingSalaries);// You found %lu gold (followers take %lu)!
    }
    else
    {
      sprintf(pTmpBuf2.data(), pGlobalTXT_LocalizationStrings[467], goldToGain);// You found %lu gold!
    }
  }
  this->uNumGold += goldToGain - hirelingSalaries;
  pUIAnim_Gold->uAnimTime = 0;
  pUIAnim_Gold->uAnimLength = 8 * pIconsFrameTable->pIcons[(signed __int16)pUIAnim_Gold->uIconID].uAnimLength;
  if ( pTmpBuf2[0] )
    ShowStatusBarString(pTmpBuf2.data(), 2u);
  pAudioPlayer->PlaySound(SOUND_GoldReceived, 0, 0, -1, 0, 0, 0, 0);
}
//----- (00421B2C) --------------------------------------------------------
void Party::sub_421B2C_PlaceInInventory_or_DropPickedItem()
{
  unsigned int v0; // eax@2
  Texture *v1; // ebx@2
  int v2; // eax@3
  int v4; // eax@6
  unsigned __int16 v5; // dx@11
  signed int v6; // eax@11
  __int16 v8; // ax@16
  SpriteObject a1; // [sp+4h] [bp-78h]@11
  int v11; // [sp+74h] [bp-8h]@2
  int v12; // [sp+78h] [bp-4h]@5

  if ( !pParty->pPickedItem.uItemID )
    return;
  v0 = pIcons_LOD->LoadTexture(
         pParty->pPickedItem.GetIconName(),
         TEXTURE_16BIT_PALETTE);
  v1 = pIcons_LOD->GetTexture(v0);
  v11 = areWeLoadingTexture;
  if ( uActiveCharacter
    && (v2 = ::pPlayers[uActiveCharacter]->AddItem(-1, pParty->pPickedItem.uItemID)) != 0 )
  {
    memcpy(&::pPlayers[uActiveCharacter]->pInventoryItemList[v2-1], &pParty->pPickedItem, 0x24u);
	  pMouse->RemoveHoldingItem();
  }
  else
  {
    for (v12 = 0; v12 < 4; v12++)
    {
      v4 = pParty->pPlayers[v12].AddItem(-1, pParty->pPickedItem.uItemID);
      if ( v4 )
      {
        memcpy(&pParty->pPlayers[v12].pInventoryItemList[v4 - 1], &pParty->pPickedItem, sizeof(ItemGen));
        pMouse->RemoveHoldingItem();
        break;
      }
    }
    if ( v12 == 4 )
    {
      v5 = pItemsTable->pItems[pParty->pPickedItem.uItemID].uSpriteID;
      v6 = 0;
      a1.uType = pItemsTable->pItems[pParty->pPickedItem.uItemID].uSpriteID;
      for ( uint i = 0; i < pObjectList->uNumObjects; i++ )
      {
        if ( v5 == pObjectList->pObjects[i].uObjectID )
        {
          v6 = i;
          break;
        }
      }
      a1.spell_caster_pid = OBJECT_Player;
      a1.uObjectDescID = v6;
      a1.vPosition.y = pParty->vPosition.y;
      a1.vPosition.x = pParty->vPosition.x;
      a1.vPosition.z = pParty->sEyelevel + pParty->vPosition.z;
      a1.uSoundID = 0;
      a1.uFacing = 0;
      a1.uAttributes = 8;
      v8 = pIndoor->GetSector(
        pParty->vPosition.x,
        pParty->vPosition.y,
        pParty->sEyelevel + pParty->vPosition.z);
      a1.uSpriteFrameID = 0;
      a1.uSectorID = v8;
      memcpy(&a1.stru_24, &pParty->pPickedItem, sizeof(a1.stru_24));
      a1.Create(pParty->sRotationY, 184, 200, 0);
      pMouse->RemoveHoldingItem();
    }
  }
  if ( !v11 )
  {
    v1->Release();
    pIcons_LOD->SyncLoadedFilesCount();
  }
  return;
}


//----- (0048C6F6) --------------------------------------------------------
bool Party::AddItemToParty(ItemGen *pItem) 
{
  unsigned int v2; // eax@1
  char *v5; // eax@8
  Texture *v7; // ebx@10
  signed int v8; // esi@10
  Player *v9; // edi@11
  int v10; // eax@11
  int v21; // [sp+24h] [bp-4h]@10

  v2 = pItem->uItemID;
  if ( !pItemsTable->pItems[v2].uItemID_Rep_St )
    pItem->SetIdentified();

  v5 = pItemsTable->pItems[v2].pIconName;
  if ( v5 )
  {
    v7 = pIcons_LOD->LoadTexturePtr(v5, TEXTURE_16BIT_PALETTE);
    v21 = areWeLoadingTexture;
    v8 = 0;
    uint current_player = uActiveCharacter;
    for (int i = 0; i < 4; i++)
    {
     current_player = current_player + i;
     if ( current_player > 4 )
       current_player = current_player - 4;
      v9 = ::pPlayers[current_player];
      v10 = v9->AddItem(-1, pItem->uItemID);
      if ( v10 )
      {
        memcpy(&v9->pInventoryItemList[v10-1], pItem, 0x24u);
        pItem->Reset();
        pAudioPlayer->PlaySound(SOUND_GoldReceived, 0, 0, -1, 0, 0, 0, 0);
        v9->PlaySound(SPEECH_60, 0);
        if ( !v21 )
        {
          v7->Release();
          pIcons_LOD->SyncLoadedFilesCount();
        }
        return true;
      }
    }
    if ( !v21 )
    {
      v7->Release();
      pIcons_LOD->SyncLoadedFilesCount();
    }
    return false;
  }
  else
  {
    MessageBoxW(nullptr, L"Invalid picture_name detected ::addItem()", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Party.cpp:795", 0);
    return false;
  }
}
// 506128: using guessed type int areWeLoadingTexture;

bool Party::IsPartyEvil()
{
  return _449B57_test_bit(_quest_bits, 100);
}

bool Party::IsPartyGood()
{
  return _449B57_test_bit(_quest_bits, 99);
}



//----- (0046A89E) --------------------------------------------------------
int Party::_46A89E_immolation_effect(int* affected, int affectedArrSize, int effectRange)
{
  int v5; // ebx@3
  int v17; // [sp+Ch] [bp-10h]@3
  int v18; // [sp+10h] [bp-Ch]@3
  int affectedCount; // [sp+18h] [bp-4h]@1

  affectedCount = 0;
  for ( size_t i = 0; i < uNumActors; ++i )
  {
    v5 = abs(pActors[i].vPosition.x - this->vPosition.x);
    v17 = abs(pActors[i].vPosition.y - this->vPosition.y);
    v18 = abs(pActors[i].vPosition.z - this->vPosition.z);
    if ( int_get_vector_length(v5, v17, v18) <= effectRange )
    {
      if ( pActors[i].uAIState != Dead && pActors[i].uAIState != Dying && pActors[i].uAIState != Removed
        && pActors[i].uAIState != Disabled && pActors[i].uAIState != Summoned )
      {
        affected[affectedCount] = i;
        affectedCount++;
        if ( affectedCount >= affectedArrSize )
          break;
      }
    }
  }
  return affectedCount;
}

//----- (00444D80) --------------------------------------------------------
int GetTravelTime()
{
  signed int new_travel_time; // esi@1

  new_travel_time = uDefaultTravelTime_ByFoot;
  if ( CheckHiredNPCSpeciality(Guide) )
    --new_travel_time;
  if ( CheckHiredNPCSpeciality(Tracker) )
    new_travel_time -= 2;
  if ( CheckHiredNPCSpeciality(Pathfinder) )
    new_travel_time -= 3;
  if ( CheckHiredNPCSpeciality(Explorer) )
    --new_travel_time;
  if ( new_travel_time < 1 )
    new_travel_time = 1;
  return new_travel_time;
}
// 6BD07C: using guessed type int uDefaultTravelTime_ByFoot;