#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <io.h>
#include <direct.h>

#include "OSAPI.h"

#include "Texture.h"
#include "mm7_data.h"
#include "VideoPlayer.h"
#include "Sprites.h"
#include "BSPModel.h"

#include "LightmapBuilder.h"
#include "DecalBuilder.h"
#include "ParticleEngine.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "CShow.h"
#include "GammaControl.h"
#include "stru6.h"

#include "Vis.h"
#include "MapInfo.h"
#include "Game.h"
#include "GUIWindow.h"
#include "GUIFont.h"
#include "Party.h"
#include "AudioPlayer.h"
#include "Outdoor.h"
#include "Outdoor_stuff.h"
#include "Overlays.h"
#include "Arcomage.h"
#include "LOD.h"
#include "Actor.h"
#include "Events.h"
#include "Viewport.h"
#include "FrameTableInc.h"
#include "Math.h"
#include "SpriteObject.h"
#include "ObjectList.h"
#include "Chest.h"
#include "PaletteManager.h"
#include "DecorationList.h"
#include "SaveLoad.h"
#include "stru123.h"
#include "stru176.h"
#include "Time.h"
#include "IconFrameTable.h"
#include "GUIProgressBar.h"
#include "Bink_Smacker.h"
#include "PlayerFrameTable.h"
#include "TurnEngine.h"
#include "FactionTable.h"
#include "StorylineTextTable.h"
#include "Random.h"
#include "CastSpellInfo.h"
#include "stru298.h"
#include "Events2D.h"
#include "Log.h"
#include "UI\UIHouses.h"
#include "texts.h"
#include "MM7.h"
#include "Lights.h"
#include "NewUI/MainMenu.h"
#include "Level/Decoration.h"

//----- (004BB756) --------------------------------------------------------
int UseNPCSkill(NPCProf profession)
{
  switch (profession)
  {
    case Healer:
    {
      for (int i = 0; i < 4; ++i)
        pParty->pPlayers[i].sHealth = pParty->pPlayers[i].GetMaxHealth();
    }
    break;

    case ExpertHealer:
    {
      for (int i = 0; i < 4; ++i)
      {
        auto player = &pParty->pPlayers[i];
        player->sHealth = player->GetMaxHealth();

        for (int j = 0; j < 14; ++j)
          player->pConditions[i] = 0;
        player->pConditions[18] = 0;
      }
    }
    break;

    case MasterHealer:
    {
      for (int i = 0; i < 4; ++i)
      {
        auto player = &pParty->pPlayers[i];
        player->sHealth = player->GetMaxHealth();

        auto v5 = LODWORD(player->pConditions[19]);//*((int *)v4 - 32);
        auto v6 = HIDWORD(player->pConditions[19]);//*((int *)v4 - 31);
        memset(player->pConditions.data(), 0, 0xA0u);

        __debugbreak();
        *(int *)&player->pActiveSkills[8] = v5;
        *(int *)&player->pActiveSkills[10] = v6;
      }
    }
    break;

    case Cook:
    {
      if (pParty->uNumFoodRations >= 13)
        return 1;

      Party::GiveFood(1);
    }
    break;

    case Chef:
    {
      if (pParty->uNumFoodRations >= 13)
        return 1;

      if (pParty->uNumFoodRations == 13)
        Party::GiveFood(1);
      else
        Party::GiveFood(2);
    }
    break;

    case WindMaster:
    {
      if (uCurrentlyLoadedLevelType == LEVEL_Indoor)
      {
        ShowStatusBarString(pGlobalTXT_LocalizationStrings[494], 2);
        pAudioPlayer->PlaySound(SOUND_203, 0, 0, -1, 0, 0, 0, 0);
      }
      else
      {
        auto v19 = pOtherOverlayList->_4418B1(10008, 203, 0, 65536);
        pParty->pPartyBuffs[PARTY_BUFF_FLY].Apply(pParty->uTimePlayed + 60 * (256 * 2), 3, 1, v19, 0);
        pParty->pPartyBuffs[PARTY_BUFF_FLY].uFlags |= 1u;
        pAudioPlayer->PlaySound(SOUND_11090, 0, 0, -1, 0, 0, 0, 0);
      }
    }
    break;

    case WaterMaster:
    {
      auto v20 = pOtherOverlayList->_4418B1(10005, 201, 0, 65536);
      pParty->pPartyBuffs[PARTY_BUFF_WATER_WALK].Apply(pParty->uTimePlayed + 60 * (256 * (2 + 1)), 3, 0, v20, 0);
      pParty->pPartyBuffs[PARTY_BUFF_WATER_WALK].uFlags |= 1u;
      pAudioPlayer->PlaySound(SOUND_12040, 0, 0, -1, 0, 0, 0, 0);
    }
    break;

    case GateMaster:
    {
      pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 0, 0);
      dword_50C9DC = 195;
      ptr_50C9E0 = GetNPCData(sDialogue_SpeakingActorNPC_ID);
    }
    break;

    case Acolyte:      _42777D_CastSpell_UseWand_ShootArrow(46, 0, 133, 0, 0); break;
    case Piper:        _42777D_CastSpell_UseWand_ShootArrow(51, 0, 133, 0, 0); break;
    case FallenWizard: _42777D_CastSpell_UseWand_ShootArrow(86, 0, 133, 0, 0); break;
      
    case Teacher:
    case Instructor:
    case Armsmaster:
    case Weaponsmaster:
    case Apprentice:
    case Mystic:
    case Spellmaster:
    case Trader:
    case Merchant:
    case Scout:
    case Herbalist:
    case Apothecary:
    case Tinker:
    case Locksmith:
    case Fool:
    case ChimneySweep:
    case Porter:
    case QuarterMaster:
    case Factor:
    case Banker:
    case Horseman:
    case Bard:
    case Enchanter:
    case Cartographer:
    case Explorer:
    case Pirate:
    case Squire:
    case Psychic:
    case Gypsy:
    case Diplomat:
    case Duper:
    case Burglar:
    case Acolyte2:
    case Initiate:
    case Prelate:
    case Monk:
    case Sage:
    case Hunter:
      break;

    default:
      assert(false && "Invalid enum value");
  }
  return 0;
}

//----- (004BBA85) --------------------------------------------------------
void CheckBountyRespawnAndAward()
{
  int i; // eax@2
  int rand_monster_id; // edx@3

  uDialogueType = 83;
  pDialogueWindow->Release();
  pDialogueWindow = GUIWindow::Create(0, 0, 640, 350, WINDOW_MainMenu, 0, 0);
  pBtn_ExitCancel = pDialogueWindow->CreateButton(471, 445, 169, 35, 1, 0, UIMSG_Escape, 0, 0, pGlobalTXT_LocalizationStrings[34],// "Cancel"
                 pIcons_LOD->GetTexture(uExitCancelTextureId), 0);
  pDialogueWindow->CreateButton(0, 0, 0, 0, 1, 0, UIMSG_BuyInShop_Identify_Repair, 0, 0, "", 0);
  pDialogueWindow->CreateButton(480, 160, 140, 30, 1, 0, UIMSG_0, 83, 0, "", 0);
  pDialogueWindow->_41D08F_set_keyboard_control_group(1, 1, 0, 2);
  dialog_menu_id = HOUSE_DIALOGUE_OTHER;
  //get new monster for hunting
  if ( pParty->PartyTimes.bountyHunting_next_generation_time[(int)((char *)window_SpeakInHouse->ptr_1C - 102)] < (signed __int64)pParty->uTimePlayed )
  {
    pParty->monster_for_hunting_killed[(int)((char *)window_SpeakInHouse->ptr_1C - 102)] = false;
    pParty->PartyTimes.bountyHunting_next_generation_time[(int)((char *)window_SpeakInHouse->ptr_1C - 102)] = (signed __int64)((double)(0x12750000 * (pParty->uCurrentMonth + 12i64 * pParty->uCurrentYear - 14015)) * 0.033333335);
    for ( i = rand(); ; i = rand() )
    {
      rand_monster_id = i % 258 + 1;
      pParty->monster_id_for_hunting[(int)((char *)window_SpeakInHouse->ptr_1C - 102)] = rand_monster_id;
      if ( (unsigned __int16)rand_monster_id < 0x73u || (unsigned __int16)rand_monster_id > 0x84u )
      {
        if ( ((unsigned __int16)rand_monster_id < 0xEBu || (unsigned __int16)rand_monster_id > 0xFCu)
          && ((unsigned __int16)rand_monster_id < 0x85u || (unsigned __int16)rand_monster_id > 0x96u)
          && ((unsigned __int16)rand_monster_id < 0x97u || (unsigned __int16)rand_monster_id > 0xBAu)
          && ((unsigned __int16)rand_monster_id < 0xC4u || (unsigned __int16)rand_monster_id > 0xC6u) )
          break;
      }
    }
  }
  bountyHunting_monster_id_for_hunting = pParty->monster_id_for_hunting[(int)((char *)window_SpeakInHouse->ptr_1C - 102)];
  if ( !pParty->monster_for_hunting_killed[(int)((char *)window_SpeakInHouse->ptr_1C - 102)] )
  {
    bountyHunting_text = pNPCTopics[351].pText;
    if ( !pParty->monster_id_for_hunting[(int)((char *)window_SpeakInHouse->ptr_1C - 102)] )
      bountyHunting_text = pNPCTopics[353].pText;
  }
  else//get prize
  {
    if ( pParty->monster_id_for_hunting[(int)((char *)window_SpeakInHouse->ptr_1C - 102)] )
    {
      pParty->PartyFindsGold(100 * pMonsterStats->pInfos[(unsigned __int16)pParty->monster_id_for_hunting[(int)((char *)window_SpeakInHouse->ptr_1C - 102)]].uLevel, 0);
      for ( uint i = 0; i < 4; ++i )
        pParty->pPlayers[i].SetVariable(VAR_Award, 86);
      pParty->uNumBountiesCollected += 100 * pMonsterStats->pInfos[pParty->monster_id_for_hunting[(int)((char *)window_SpeakInHouse->ptr_1C - 102)]].uLevel;
      pParty->monster_id_for_hunting[(int)((char *)window_SpeakInHouse->ptr_1C - 102)] = 0;
      pParty->monster_for_hunting_killed[(int)((char *)window_SpeakInHouse->ptr_1C - 102)] = false;
    }
    bountyHunting_text = pNPCTopics[352].pText;
  }
}

//----- (004BBCDD) --------------------------------------------------------
void  sub_4BBCDD()
{
  signed int v0; // ebp@3
  Actor *v1; // eax@4
  __int16 v2; // cx@5
  int v3; // esi@8
  Player *v4; // esi@14
  GUIButton *v5; // eax@18
  GUIButton *v6; // esi@19
  size_t v7; // [sp+10h] [bp-4h]@4

  if ( pParty->field_7B5_in_arena_quest )
  {
    if ( pParty->field_7B5_in_arena_quest == -1 )
    {
      uDialogueType = 92;
    }
    else
    {
      v0 = 0;
      if ( (signed int)uNumActors > 0 )
	  {
      v1 = pActors.data();//[0].uAIState;
		  v7 = uNumActors;
		  do
		  {
			v2 = v1->uAIState;
			if ( v1->uAIState == Dead || v2 == Removed || v2 == Disabled || (v3 = v1->uSummonerID) != 0 && PID_TYPE(v3) == OBJECT_Player)
			  ++v0;
			++v1;
			--v7;
		  }
		  while ( v7 );
	  }
      if ( v0 >= (signed int)uNumActors || (signed int)uNumActors <= 0)
      {
        uDialogueType = 91;
        v4 = pParty->pPlayers.data();
        ++*((char *)&pParty->monster_for_hunting_killed[3] + (unsigned __int8)pParty->field_7B5_in_arena_quest + 1);
        do
        {
          v4->SetVariable(VAR_Award, (unsigned __int8)pParty->field_7B5_in_arena_quest + 3);
          ++v4;
        }
        while ( (signed int)v4 < (signed int)pParty->pHirelings.data() );
        pParty->PartyFindsGold(gold_transaction_amount, 0);
        pAudioPlayer->PlaySound((SoundID)14060, 0, 0, -1, 0, 0, 0, 0);
        pParty->field_7B5_in_arena_quest = -1;
      }
      else
      {
        uDialogueType = 90;
        pParty->vPosition.x = 3849;
        pParty->vPosition.y = 5770;
        pParty->vPosition.z = 1;
        pParty->uFallStartY = 1;
        pParty->sRotationY = 512;
        pParty->sRotationX = 0;
        pParty->uFallSpeed = 0;
        pAudioPlayer->PlaySound((SoundID)14060, 0, 0, -1, 0, 0, 0, 0);
      }
    }
  }
  else
  {
    uDialogueType = 89;
    v5 = pDialogueWindow->pControlsHead;
    if ( v5 )
    {
      do
      {
        v6 = v5->pNext;
        free(v5);
        v5 = v6;
      }
      while ( v6 );
    }
    pDialogueWindow->pControlsHead = 0;
    pDialogueWindow->pControlsTail = 0;
    pDialogueWindow->uNumControls = 0;
    pBtn_ExitCancel = pDialogueWindow->CreateButton(0x1D7u, 0x1BDu, 0xA9u, 0x23u, 1, 0, UIMSG_Escape, 0, 0, pGlobalTXT_LocalizationStrings[79],
                   pIcons_LOD->GetTexture(uExitCancelTextureId), 0);
    pDialogueWindow->CreateButton(0x1E0u, 0xA0u, 0x8Cu, 0x1Eu, 1, 0, UIMSG_SelectNPCDialogueOption, 0x55u, 0, "", 0);
    pDialogueWindow->CreateButton(0x1E0u, 0xBEu, 0x8Cu, 0x1Eu, 1, 0, UIMSG_SelectNPCDialogueOption, 0x56u, 0, "", 0);
    pDialogueWindow->CreateButton(0x1E0u, 0xDCu, 0x8Cu, 0x1Eu, 1, 0, UIMSG_SelectNPCDialogueOption, 0x57u, 0, "", 0);
    pDialogueWindow->CreateButton(0x1E0u, 0xFAu, 0x8Cu, 0x1Eu, 1, 0, UIMSG_SelectNPCDialogueOption, 0x58u, 0, "", 0);
    pDialogueWindow->_41D08F_set_keyboard_control_group(4, 1, 0, 1);
  }
}

//----- (004BC109) --------------------------------------------------------
void  ArenaFight()
{
  int v0; // edi@1
  char *v1; // eax@7
  Player *v2; // edi@9
  int v3; // eax@10
  signed int v4; // esi@10
  signed __int64 v5; // qax@18
  signed int v6; // ebx@34
  char *v7; // edi@34
  int v8; // eax@37
  int v9; // esi@43
  int v10; // eax@44
  unsigned __int8 v11; // sf@44
  unsigned __int8 v12; // of@44
  signed int v13; // eax@49
  int v14; // esi@49
  int v15; // edx@50
  int v16; // eax@51
  signed int v17; // ecx@51
  int v18; // edx@53
  int i; // edi@55
  int monster_y; // ST18_4@56
  int v21; // eax@56
  signed int v22; // [sp-4h] [bp-144h]@51
  __int16 v23[100]; // [sp+Ch] [bp-134h]@39
  GUIWindow window; // [sp+D4h] [bp-6Ch]@1
  __int16 monster_ids[6]; // [sp+128h] [bp-18h]@56
  int v26; // [sp+134h] [bp-Ch]@1
  int v27; // [sp+138h] [bp-8h]@23
  int num_monsters; // [sp+13Ch] [bp-4h]@17

  v26 = 0;
  pParty->field_7B5_in_arena_quest = uDialogueType;
  memcpy(&window, pDialogueWindow, sizeof(window));
  window.uFrameWidth = game_viewport_width;
  window.uFrameZ = 452;
  v0 = pFontArrus->CalcTextHeight(pGlobalTXT_LocalizationStrings[575], &window, 13, 0) + 7;
  pRenderer->BeginSceneD3D();

  if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
    pIndoor->Draw();
  else if ( uCurrentlyLoadedLevelType == LEVEL_Outdoor )
      pOutdoor->Draw();

  pRenderer->DrawBillboards_And_MaybeRenderSpecialEffects_And_EndScene();
  pRenderer->BeginScene();
  if ( pRenderer->pRenderD3D )
    pRenderer->FillRectFast(
      pViewport->uViewportTL_X,
      pViewport->uViewportTL_Y,
      pViewport->uViewportBR_X - pViewport->uViewportTL_X,
      pViewport->uViewportBR_Y - pViewport->uViewportTL_Y + 1,
      pRenderer->uTargetGMask | pRenderer->uTargetBMask);

  auto pTex = pIcons_LOD->GetTexture(uTextureID_Leather);
  pRenderer->GetLeather(8, 352 - v0, pTex, pTex->uTextureHeight - v0);

  pRenderer->DrawTextureIndexed(8u, 347 - v0, pTexture_591428);
  v1 = FitTextInAWindow(pGlobalTXT_LocalizationStrings[575], pFontArrus, &window, 0xDu, 0);
  pDialogueWindow->DrawText(pFontArrus, 13, 354 - v0, 0, v1, 0, 0, 0);
  pRenderer->EndScene();
  pRenderer->Present();
  pParty->vPosition.x = 3849;
  pParty->vPosition.y = 5770;
  pParty->vPosition.z = 1;
  pParty->uFallStartY = 1;
  pParty->sRotationY = 512;
  pParty->sRotationX = 0;
  pParty->uFallSpeed = 0;
  /*if ( (signed int)pMessageQueue_50CBD0->uNumMessages < 40 )
  {
    pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].eType = UIMSG_Escape;
    pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].param = 1;
    *(&pMessageQueue_50CBD0->uNumMessages + 3 * pMessageQueue_50CBD0->uNumMessages + 3) = 0;
    ++pMessageQueue_50CBD0->uNumMessages;
  }*/
  pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, 0);
  v2 = pParty->pPlayers.data();
  do
  {
    v3 = v2->GetActualLevel();
    v4 = v26;
    if ( v3 > v26 )
    {
      v26 = v2->GetActualLevel();
      v4 = v26;
    }
    ++v2;
  }
  while ( (signed int)v2 < (signed int)pParty->pHirelings.data() );
  if ( uDialogueType == 85 )
  {
    num_monsters = v4;
    v4 /= 2;
  }
  else if ( uDialogueType == 86 )
  {
    v5 = (signed __int64)((double)v26 * 1.5);
    num_monsters = v5;
    v4 /= 2;
  }
  else if ( uDialogueType == 87 )
  {
    LODWORD(v5) = 2 * v4;
    num_monsters = v5;
    v4 /= 2;
  }
  else
  {
    __debugbreak(); // warning C4700: uninitialized local variable 'v27' used
    if ( uDialogueType == 88 )
      num_monsters = 2 * v4;
    else
      v4 = v27;
  }
  if ( v4 < 1 )
    v4 = 1;
  if ( v4 > 100 )
    v4 = 100;
  if ( num_monsters > 100 )
    num_monsters = 100;
  if ( v4 < 2 )
    v4 = 2;
  if ( num_monsters < 2 )
    num_monsters = 2;
  v6 = 0;
  v27 = 1;
  v7 = (char *)&pMonsterStats->pInfos[1].uLevel;
  do
  {
    if ( v7[8] != 1 )
    {
      if ( !MonsterStats::BelongsToSupertype(*((short *)v7 + 22), MONSTER_SUPERTYPE_8) )
      {
        v8 = (unsigned __int8)*v7;
        if ( v8 >= v4 )
        {
          if ( v8 <= num_monsters )
            v23[v6++] = v27;
        }
      }
    }
    ++v27;
    v7 += 88;
  }
  while ( (signed int)v7 <= (signed int)&pMonsterStats->pInfos[258].uLevel );
  num_monsters = 6;
  if ( v6 < 6 )
    num_monsters = v6;
  v9 = 0;
  if ( num_monsters > 0 )
  {
    do
    {
      v10 = rand();
      ++v9;
      v12 = __OFSUB__(v9, num_monsters);
      v11 = v9 - num_monsters < 0;
      *((short *)&window.pControlsTail + v9 + 1) = v23[v10 % v6];
    }
    while ( v11 ^ v12 );
  }
  if ( uDialogueType == 85 )
  {
    v16 = rand();
    v17 = 3;
    v22 = 50;
    v18 = v16 % v17;
    v13 = v22;
    v14 = v18 + 6;
  }
  else if ( uDialogueType == 86 )
  {
    v16 = rand();
    v17 = 7;
    v22 = 100;
    v18 = v16 % v17;
    v13 = v22;
    v14 = v18 + 6;
  }
  else if ( uDialogueType == 87 )
  {
    v15 = rand() % 11;
    v13 = 200;
    v14 = v15 + 10;
  }
  else
  {
    if ( uDialogueType == 88 )
    {
      v13 = 500;
      v14 = 20;
    }
    else
    {
      v14 = v27;
      v13 = gold_transaction_amount;
    }
  }
  i = 0;
  for ( gold_transaction_amount = v26 * v13; i < v14; ++i )
  {
    monster_y = pMonsterArenaPlacements[i].y;
    v21 = rand();
    _4BBF61_summon_actor((unsigned __int16)monster_ids[v21 % num_monsters], pMonsterArenaPlacements[i].x, monster_y, 1);
  }
  pAudioPlayer->PlaySound((SoundID)14060, 0, 0, -1, 0, 0, 0, 0);
}

//----- (004BD8B5) --------------------------------------------------------
int sub_4BD8B5()
{
  int v0; // eax@4

  if ( pMessageQueue_50CBD0->uNumMessages )
    pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
  pKeyActionMap->SetWindowInputStatus(WINDOW_INPUT_CANCELLED);
  pKeyActionMap->ResetKeys();
  activeLevelDecoration = NULL;
  current_npc_text = 0;
  if ( pDialogueNPCCount )
  {
    v0 = dialog_menu_id;
    if ( dialog_menu_id != HOUSE_DIALOGUE_SHOP_BUY_STANDARD && dialog_menu_id != HOUSE_DIALOGUE_SHOP_SELL
      && dialog_menu_id != HOUSE_DIALOGUE_97 && dialog_menu_id != HOUSE_DIALOGUE_SHOP_REPAIR
      && dialog_menu_id != HOUSE_DIALOGUE_SHOP_IDENTIFY && ShopTexture )
    {
      ShopTexture->Release();
      v0 = dialog_menu_id;
      ShopTexture = 0;
    }
    if ( v0 && v0 != 1 )
    {
      if ( v0 == -1 )
      {
        _4B4224_UpdateNPCTopics((int)((char *)pDialogueNPCCount - 1));
        pVideoPlayer->_4BF5B2();
        return 1;
      }
      if ( v0 != 94 && v0 != 96 && v0 != 101 )
      {
        if ( v0 == 3 || v0 == 5 || v0 == 4 )
        {
          UI_CreateEndConversationButton();
          dialog_menu_id = HOUSE_DIALOGUE_SHOP_DISPLAY_EQUIPMENT;
          InitializaDialogueOptions_Shops(in_current_building_type);
        }
        else
        {
          if ( v0 != 102 && v0 != 103 && v0 != 104 )
          {
            pVideoPlayer->_4BF5B2();
            dialog_menu_id = HOUSE_DIALOGUE_MAIN;
            InitializaDialogueOptions(in_current_building_type);
            return 1;
          }
          pVideoPlayer->_4BF5B2();
          UI_CreateEndConversationButton();
          dialog_menu_id = HOUSE_DIALOGUE_TAVERN_ARCOMAGE_MAIN;
          InitializaDialogueOptions_Tavern(in_current_building_type);
        }
        return 1;
      }
      pVideoPlayer->_4BF5B2();
      UI_CreateEndConversationButton();
	  dialog_menu_id = HOUSE_DIALOGUE_MAIN;
	  InitializaDialogueOptions(in_current_building_type);
	  return 1;
    }
    pDialogueNPCCount = 0;
    pDialogueWindow->Release();
    dialog_menu_id = HOUSE_DIALOGUE_NULL;
    pDialogueWindow = 0;
    pIcons_LOD->SyncLoadedFilesCount();
    if ( uNumDialogueNPCPortraits != 1 )
    {
      pBtn_ExitCancel = window_SpeakInHouse->pControlsHead;
      if ( uNumDialogueNPCPortraits > 0 )
      {
        for ( uint i = 0; i < (unsigned int)uNumDialogueNPCPortraits; ++i )
        {
          HouseNPCPortraitsButtonsList[i] = window_SpeakInHouse->CreateButton(pNPCPortraits_x[uNumDialogueNPCPortraits - 1][i],
                                                                              pNPCPortraits_y[uNumDialogueNPCPortraits - 1][i],
                                              63, 73, 1, 0, UIMSG_ClickHouseNPCPortrait, i, 0, byte_591180[i].data(), 0, 0, 0);
        }
      }
      pVideoPlayer->_4BF5B2();
      return 1;
    }
  }
  return 0;
}

//----- (004BF91E) --------------------------------------------------------
unsigned int  GameOverMenu(void *ecx0)
{
  const char *v1; // eax@2
  unsigned int result; // eax@3
  Player *v3; // eax@7
  const char *v4; // ST24_4@9
  int v5; // eax@9
  const char *v6; // eax@10
  const char *v7; // edx@10
  const char *v8; // ecx@12
  const char *v9; // eax@14
  unsigned int v10; // eax@25
  GUIWindow pWindow; // [sp+34h] [bp-9Ch]@1
  //MSG Msg; // [sp+88h] [bp-48h]@22
  unsigned int v14; // [sp+A4h] [bp-2Ch]@5
  void *v15; // [sp+A8h] [bp-28h]@1
  const char *pInString; // [sp+ACh] [bp-24h]@5
  unsigned int v17; // [sp+B0h] [bp-20h]@5
  unsigned int v18; // [sp+B4h] [bp-1Ch]@5
  unsigned int v19; // [sp+B8h] [bp-18h]@5
  int v20; // [sp+BCh] [bp-14h]@7
  Player *i; // [sp+C0h] [bp-10h]@7
  GUIFont *pFont; // [sp+C4h] [bp-Ch]@1
  unsigned __int64 v23; // [sp+C8h] [bp-8h]@5

  v15 = ecx0;

  RGBTexture _this; // [sp+Ch] [bp-C4h]@1
  //RGBTexture::RGBTexture(&this);

  BYTE1(dword_6BE364_game_settings_1) &= 0xBFu;
  bGameoverLoop = 1;
  pVideoPlayer->bStopBeforeSchedule = 0;
  pAudioPlayer->StopChannels(-1, -1);
  pRenderer->BeginScene();
  pRenderer->ClearBlack();
  pRenderer->EndScene();
  pRenderer->Present();
  pVideoPlayer->pResetflag = 0;
  _449B57_test_bit(pParty->_quest_bits, 99);
  _this.Load("winbg.pcx", 2);
  pRenderer->BeginScene();
  pRenderer->DrawTextureRGB(0, 0, &_this);
  pRenderer->EndScene();
  free(_this.pPixels);
  _this.pPixels = 0;
  window_SpeakInHouse = GUIWindow::Create(0, 0, 640, 480, WINDOW_MainMenu, 0, 0);
  pWindow.uFrameX = 75;
  pWindow.uFrameY = 60;
  pWindow.uFrameWidth = 469;
  pWindow.uFrameHeight = 338;
  pWindow.uFrameZ = 543;
  pWindow.uFrameW = 397;
  pFont = LoadFont("endgame.fnt", "FONTPAL", NULL);
  if ( pParty->IsPartyGood() )
  {
    v1 = pGlobalTXT_LocalizationStrings[675];
  }
  else
  {
    result = pParty->IsPartyEvil();
    if ( !(short)result )
      return result;
    v1 = pGlobalTXT_LocalizationStrings[676];
  }
  pInString = v1;
  v23 = pParty->uTimePlayed - 138240;
  v19 = (unsigned int)((signed __int64)((double)(pParty->uTimePlayed - 138240) * 0.234375) / 60 / 60) / 0x18;
  v14 = (unsigned int)((signed __int64)((double)(pParty->uTimePlayed - 138240) * 0.234375) / 60 / 60) / 0x18 / 0x1C / 0xC;
  v18 = (unsigned int)((signed __int64)((double)(pParty->uTimePlayed - 138240) * 0.234375) / 60 / 60) / 0x18 / 0x1C % 0xC;
  v17 = v19 % 0x1C;
  if ( !v19 )
    v19 = 1;
  pRenderer->BeginScene();
  pWindow.DrawTitleText(pFont, 1u, 0x23u, 1u, pGlobalTXT_LocalizationStrings[9], 3u);
  v3 = pParty->pPlayers.data();//[0].pName;
  v23 = 0i64;
  v20 = 0;
  //for ( i = (int)pParty->pPlayers[0].pName; ; v3 = (char *)i )
  for ( i = pParty->pPlayers.data(); ; v3 = i )
  {
	v4 = pClassNames[v3->classType];
    v5 = v3->GetBaseLevel();
	sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[129], i->pName, v5, v4);
    pWindow.DrawTitleText(
      pFont,
      1u,
      v20 * (LOBYTE(pFont->uFontHeight) - 2) + LOBYTE(pFont->uFontHeight) + 46,
      1u,
      pTmpBuf.data(),
      3u);
	v23 += i->uExperience;//__PAIR__(*(int *)(i - 4), *(int *)(i - 8));
    ++v20;
    ++i;
	if ( i > &pParty->pPlayers[3] )
      break;
  }
  v23 = (signed __int64)v23 / v19;
  v6 = FitTextInAWindow(pInString, pFont, &pWindow, 0xCu, 0);
  pWindow.DrawTitleText(pFont, 1u, 5 * (LOBYTE(pFont->uFontHeight) + 11), 1u, v6, 0);
  strcpy(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[37]);
  v7 = pGlobalTXT_LocalizationStrings[56];
  if ( v17 != 1 )
    v7 = pGlobalTXT_LocalizationStrings[57];
  v8 = pGlobalTXT_LocalizationStrings[146];
  if ( v18 != 1 )
    v8 = pGlobalTXT_LocalizationStrings[148];
  v9 = pGlobalTXT_LocalizationStrings[245];
  if ( v14 != 1 )
    v9 = pGlobalTXT_LocalizationStrings[132];
  sprintf(pTmpBuf2.data(), " %lu %s, %lu %s, %lu %s ", v14, v9, v18, v8, v17, v7);
  strcat(pTmpBuf.data(), pTmpBuf2.data());
  pWindow.DrawTitleText(pFont, 1u, pWindow.uFrameHeight - 2 * LOBYTE(pFont->uFontHeight) - 5, 1u, pTmpBuf.data(), 3u);
  sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[94], v23);
  pWindow.DrawTitleText(pFont, 1u, pWindow.uFrameHeight, 1u, pTmpBuf.data(), 3u);
  BYTE1(dword_6BE364_game_settings_1) |= 0x40u;
  pRenderer->EndScene();
  pRenderer->Present();
  if ( pRenderer->pRenderD3D )
    pRenderer->pBeforePresentFunction();
  pRenderer->_49F1BC("MM7_Win.Pcx");
  free(pFont);
  window_SpeakInHouse->Release();
  window_SpeakInHouse = 0;
  if ( v15 == (void *)2 )
  {
    result = pMessageQueue_50CBD0->uNumMessages;
  }
  else
  {
    LODWORD(v23) = GetTickCount() + 5000;
    while ( (unsigned int)v23 > GetTickCount() )
      ;
    for (MSG msg; PeekMessage(&msg, 0, 0, 0, PM_REMOVE);)
    {
      if (msg.message == WM_QUIT)
        Game_DeinitializeAndTerminate(0);
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    if ( pMessageQueue_50CBD0->uNumMessages )
    {
      LOBYTE(v10) = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
      pMessageQueue_50CBD0->uNumMessages = v10;
    }
    pKeyActionMap->ResetKeys();
    pKeyActionMap->uLastKeyPressed = 0;
    do
    {
      for (MSG msg; PeekMessage(&msg, 0, 0, 0, PM_REMOVE);)
      {
        if (msg.message == WM_QUIT)
          Game_DeinitializeAndTerminate(0);
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
    while ( !pKeyActionMap->uLastKeyPressed );
    result = pMessageQueue_50CBD0->uNumMessages;
    if ( pMessageQueue_50CBD0->uNumMessages )
    {
      result = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
      pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
    }
  }
  if ( v15 )
  {
    /*if ( (signed int)result < 40 )
    {
      pMessageQueue_50CBD0->pMessages[result].eType = UIMSG_Quit;
    }*/
    pMessageQueue_50CBD0->AddMessage(UIMSG_Quit, 1, 0);
  }
  else
  {
    pMessageQueue_50CBD0->AddMessage(UIMSG_ShowFinalWindow, 1, 0);
    /*if ( (signed int)result < 40 )
    {
      pMessageQueue_50CBD0->pMessages[result].eType = UIMSG_C5;
      pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].param = 1;
      result = 3 * pMessageQueue_50CBD0->uNumMessages + 3;
      *(&pMessageQueue_50CBD0->uNumMessages + result) = 0;
      ++pMessageQueue_50CBD0->uNumMessages;
    }*/
  }
  bGameoverLoop = 0;
  return result;
}




//----- (00451007) --------------------------------------------------------
int stru350::sub_451007_scale_image_bicubic(unsigned short *pSrc, int srcWidth, int srcHeight, int srcPitch,          //changing this to some library function might be a good idea
                                            unsigned short *pDst, int dstWidth, int dstHeight, int dstPitch,
                                            int a10, int a11)
{
  int result; // eax@1
  float v17; // ST3C_4@12
  float v18; // ST38_4@12
  unsigned int v19; // esi@12
  int v21; // eax@18
  unsigned int v22; // ecx@25
  unsigned int v23; // eax@29
  unsigned int heightRatioPlusOne; // [sp+Ch] [bp-7Ch]@12
  unsigned int widthRatio; // [sp+Ch] [bp-7Ch]@218
  unsigned int heightRatio; // [sp+14h] [bp-74h]@12
  unsigned int widthRatioPlusOne; // [sp+14h] [bp-74h]@218
  int v160; // [sp+3Ch] [bp-4Ch]@13
  unsigned __int16 *v175; // [sp+4Ch] [bp-3Ch]@13
  unsigned __int16 *v193; // [sp+5Ch] [bp-2Ch]@7
  signed int v231; // [sp+78h] [bp-10h]@7
  __int64 v240; // [sp+7Ch] [bp-Ch]@12
  unsigned int v251; // [sp+80h] [bp-8h]@218
  unsigned int v252; // [sp+84h] [bp-4h]@218
  float a6s; // [sp+A0h] [bp+18h]@218
  float a6t; // [sp+A0h] [bp+18h]@218
  unsigned int a6b; // [sp+A0h] [bp+18h]@218
  int field_0_bits;
  int field_20_bits;
  
  int field0value = this->field_0.field_C;
  switch(field0value)
  {
  case 8: field_0_bits = 1;
    break;
  case 16: field_0_bits = 2;
    break;
  case 32: field_0_bits = 4;
    break;
  default:
    return field0value;
  }
  int field20value = this->field_20.field_C;
  switch(field20value)
  {
  case 8: field_20_bits = 1;
    break;
  case 16: field_20_bits = 2;
    break;
  case 32: field_20_bits = 4;
    break;
  default:
    return field20value;
  }

  result = (int)pDst;
  v193 = pDst;
  v231 = 0;
  if ( dstHeight <= 0 )
    return result;
  
  do
  {
    for (int counter = 0; counter < dstWidth; counter++)
    {
      a6s = (double)counter / (double)dstWidth * (double)srcWidth;
      widthRatio = bankersRounding(a6s);
      a6t = (double)(counter + 1) / (double)dstWidth * (double)srcWidth;
      widthRatioPlusOne = bankersRounding(a6t);
      v17 = (double)v231 / (double)dstHeight * (double)srcHeight;
      heightRatio = bankersRounding(v17);
      v18 = (double)(v231 + 1) / (double)dstHeight * (double)srcHeight;
      heightRatioPlusOne = bankersRounding(v18);
      v251 = 0;
      v19 = (heightRatioPlusOne - heightRatio) * (widthRatioPlusOne - widthRatio);
      v252 = 0;
      a6b = 0;
      v240 = 0i64;

      v175 = (unsigned short*)((char *)pSrc + field_0_bits * (widthRatio + srcPitch * heightRatio));
      for (int heightDiff = 0; heightDiff < heightRatioPlusOne - heightRatio; heightDiff++)
      {
        int ratioDiff = widthRatioPlusOne - widthRatio;
        for(int i = 0; i < ratioDiff; i++)
        {
          if(field0value == 32)
            v21 = _450FB1(((int*)v175)[i]);
          else if(field0value == 16)
            v21 = _450FB1(((_WORD*)v175)[i]);
          else
            v21 = _450FB1(((unsigned __int8*)v175)[i]);
          v240 += ((unsigned int)v21 >> 24);
          a6b += BYTE2(v21);
          v252 += BYTE1(v21);
          v251 += (unsigned __int8)v21;
        }
        if(field0value == 32)
          v175 += 2 * srcPitch;
        else if(field0value == 16)
          v175 += srcPitch;   
        else
          v175 = (unsigned short*)((char *)v175 + 2 * srcPitch);
      }
      v22 = (unsigned int)v240 / ((heightRatioPlusOne - heightRatio) * (widthRatioPlusOne - widthRatio));
      if ( v19 )
      {
        a6b /= v19;
        v252 /= v19;
        v251 /= v19;
      }
      if ( v22 != 255 )
        v22 &= 0x7FFFFFFFu;
      v23 = _450F55(v251 | ((v252 | ((a6b | (v22 << 8)) << 8)) << 8));
      *(_DWORD *)v193 = v23;
      v193 = (unsigned __int16 *)((char *)v193 + field_20_bits);
    }
    v193 = (unsigned __int16 *)((char *)v193 + field_20_bits * (dstPitch - dstWidth));
    ++v231;
    result = v231;
  }
  while(v231 < dstHeight);
  return result;
}

//----- (0044F57C) --------------------------------------------------------
void SpawnEncounter(MapInfo *pMapInfo, SpawnPointMM7 *spawn, int a3, int a4, int a5)
{
  //MapInfo *v5; // esi@1
  //SpawnPointMM7 *v6; // ebx@1
  int v7; // eax@2
  char v8; // zf@5
  int v9; // edi@9
  int v10; // eax@9
  int v11; // ecx@9
  int v12; // edx@9
  int v13; // eax@9
  int v14; // eax@14
  int v15; // ecx@14
  int v16; // eax@19
  int v17; // ecx@19
  int v18; // esi@31
  //int pPosX; // ecx@32
  //int v20; // edx@32
  //int v21; // eax@32
  Actor *pMonster; // esi@35
  int v23; // edx@36
  signed int v24; // edi@36
  int v25; // ecx@36
  unsigned __int16 v26; // ax@47
  MonsterDesc *v27; // edi@48
  signed int v28; // eax@48
  __int16 v29; // cx@50
  __int16 v30; // ax@50
  __int16 v31; // ax@50
  int v32; // eax@50
  int v33; // edi@50
  int v34; // eax@50
  int v35; // eax@50
  int v36; // eax@50
  int v37; // eax@51
  int v38; // eax@52
  int v39; // edi@52
  std::string v40; // [sp-18h] [bp-100h]@60
  void *v41; // [sp-14h] [bp-FCh]@50
  //void *v42; // [sp-10h] [bp-F8h]@50
  //size_t v43; // [sp-Ch] [bp-F4h]@50
  const char *v44; // [sp-8h] [bp-F0h]@13
  char *pTexture; // [sp-4h] [bp-ECh]@9
  char Str[32]; // [sp+Ch] [bp-DCh]@60
  char Str2[120]; // [sp+2Ch] [bp-BCh]@29
  unsigned int uFaceID; // [sp+A4h] [bp-44h]@52
  MonsterInfo *Src; // [sp+A8h] [bp-40h]@50
  int v50; // [sp+ACh] [bp-3Ch]@47
  char Source[32]; // [sp+B0h] [bp-38h]@20
  int v52; // [sp+D0h] [bp-18h]@34
  int v53; // [sp+D4h] [bp-14h]@34
  int pSector; // [sp+D8h] [bp-10h]@32
  int pPosX; // [sp+DCh] [bp-Ch]@32
  int v56; // [sp+E0h] [bp-8h]@8
  int v57; // [sp+E4h] [bp-4h]@1

  //auto a2 = spawn;
  v57 = 0;
  //v5 = pMapInfo;
  //v6 = spawn;
  if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
    v7 = pOutdoor->ddm.field_C_alert;
  else if (uCurrentlyLoadedLevelType == LEVEL_Outdoor)
    v7 = pIndoor->dlv.field_C_alert;
  else
    v7 = 0;
  if (v7)
    v8 = (spawn->uAttributes & 1) == 0;
  else
    v8 = (spawn->uAttributes & 1) == 1;
  if (v8)
    return;
    //result = (void *)(spawn->uIndex - 1);
  v56 = 1;
  switch (spawn->uIndex - 1)
  {
    case 0u:
        v9 = pMapInfo->uEncounterMonster1AtLeast;
        v10 = rand();
        v11 = pMapInfo->uEncounterMonster1AtMost;
        pTexture = pMapInfo->pEncounterMonster1Texture;
        v12 = v10 % (v11 - v9 + 1);
        v13 = pMapInfo->Dif_M1;
        v57 = v13;
        v56 = v9 + v12;
        strcpy(Source, pTexture);
        break;
    case 3u:
        pTexture = pMapInfo->pEncounterMonster1Texture;
        v44 = "%s A";
        sprintf(Source, v44, pTexture);
        break;
    case 4u:
        pTexture = pMapInfo->pEncounterMonster2Texture;
        v44 = "%s A";
        sprintf(Source, v44, pTexture);
        break;
    case 5u:
        pTexture = pMapInfo->pEncounterMonster3Texture;
        v44 = "%s A";
        sprintf(Source, v44, pTexture);
        break;
    case 1u:
        v9 = pMapInfo->uEncounterMonster2AtLeast;
        v14 = rand();
        v15 = pMapInfo->uEncounterMonster2AtMost;
        pTexture = pMapInfo->pEncounterMonster2Texture;
        v12 = v14 % (v15 - v9 + 1);
        v13 = pMapInfo->Dif_M2;
        v57 = v13;
        v56 = v9 + v12;
        strcpy(Source, pTexture);
        break;
    case 6u:
        pTexture = pMapInfo->pEncounterMonster1Texture;
        v44 = "%s B";
        sprintf(Source, v44, pTexture);
        break;
    case 7u:
        pTexture = pMapInfo->pEncounterMonster2Texture;
        v44 = "%s B";
        sprintf(Source, v44, pTexture);
        break;
    case 8u:
        pTexture = pMapInfo->pEncounterMonster3Texture;
        v44 = "%s B";
        sprintf(Source, v44, pTexture);
        break;
    case 2u:
        v9 = pMapInfo->uEncounterMonster3AtLeast;
        v16 = rand();
        v17 = pMapInfo->uEncounterMonster3AtMost;
        pTexture = pMapInfo->pEncounterMonster3Texture;
        v12 = v16 % (v17 - v9 + 1);
        v13 = pMapInfo->Dif_M3;
        v57 = v13;
        v56 = v9 + v12;
        strcpy(Source, pTexture);
        break;
    case 9u:
        pTexture = pMapInfo->pEncounterMonster1Texture;
        v44 = "%s C";
        sprintf(Source, v44, pTexture);
        break;
    case 0xAu:
        pTexture = pMapInfo->pEncounterMonster2Texture;
        v44 = "%s C";
        sprintf(Source, v44, pTexture);
        break;
    case 0xBu:
        pTexture = pMapInfo->pEncounterMonster3Texture;
        v44 = "%s C";
        sprintf(Source, v44, pTexture);
        break;
      default:
        return;
  }
    if (Source[0] == '0')
        return;
    v57 += a3;
    if ( v57 > 4 )
        v57 = 4;
    strcpy(Str2, Source);
    if ( a4 )
        v56 = a4;
    v18 = v56;
    if ( (signed int)(v56 + uNumActors) >= 500 )
        return;
    pSector = 0;
    pPosX = spawn->vPosition.x;
    a4 = spawn->vPosition.y;
    a3 = spawn->vPosition.z;
    if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
        pSector = pIndoor->GetSector(spawn->vPosition.x, spawn->vPosition.y, spawn->vPosition.z);
    v53 = 0;
    v52 = (((uCurrentlyLoadedLevelType != LEVEL_Outdoor) - 1) & 0x40) + 64;
    if ( v18 <= 0 )
        return;
  for (uint i = v53; i < v56; ++i)
  {
      pMonster = &pActors[uNumActors];
      pActors[uNumActors].Reset();
      if ( v57 )
      {
        v23 = rand() % 100;
        v24 = 3;
        v25 = (unsigned __int16)word_4E8152[3 * v57];
        if ( v23 >= v25 )
        {
          if ( v23 < v25 + (unsigned __int16)word_4E8152[3 * v57 + 1] )
            v24 = 2;
        }
        else
        {
          v24 = 1;
        }
        if ( v24 == 1 )
        {
          pTexture = Source;
          v44 = "%s A";
        }
        else
        {
          if ( v24 == 2 )
          {
            pTexture = Source;
            v44 = "%s B";
          }
          else
          {
            if ( v24 != 3 )
              continue;
            pTexture = Source;
            v44 = "%s C";
          }
        }
        sprintf(Str2, v44, pTexture);
      }
      v50 = pMonsterList->GetMonsterIDByName(Str2);
      pTexture = Str2;
      if ( (signed __int16)v50 == -1 )
      {
        sprintf(Str, "Can't create random monster: '%s'! See MapStats.txt and Monsters.txt!", pTexture);
        MessageBoxA(nullptr, Str, nullptr, 0);
        ExitProcess(0);
      }
      v27 = &pMonsterList->pMonsters[(signed __int16)v50];
      v28 = pMonsterStats->FindMonsterByTextureName(pTexture);
      if ( !v28 )
        v28 = 1;
      Src = &pMonsterStats->pInfos[v28];
      strcpy(pMonster->pActorName, Src->pName);
      pMonster->sCurrentHP = Src->uHP;
      assert(sizeof(MonsterInfo) == 88);
      memcpy(&pMonster->pMonsterInfo, Src, sizeof(MonsterInfo));
      pMonster->word_000086_some_monster_id = v50 + 1;
      pMonster->uActorRadius = v27->uMonsterRadius;
      pMonster->uActorHeight = v27->uMonsterHeight;
      pMonster->uMovementSpeed = v27->uMovementSpeed;
      pMonster->vInitialPosition.x = spawn->vPosition.x;
      pMonster->vPosition.x = spawn->vPosition.x;
      pMonster->uTetherDistance = 256;
      pMonster->vInitialPosition.y = a4;
      pMonster->vPosition.y = a4;
      pTexture = 0;
      pMonster->vInitialPosition.z = a3;
      pMonster->vPosition.z = a3;
      pMonster->uSectorID = pSector;
      pMonster->uGroup = spawn->uGroup;
      pMonster->PrepareSprites((char)pTexture);
      pMonster->pMonsterInfo.uHostilityType = MonsterInfo::Hostility_Friendly;
      v32 = rand();
      v33 = v32 % 2048;
      v34 = stru_5C6E00->Cos(v32 % 2048);
      a4 = v34;
      a3 = (unsigned __int64)(v34 * (signed __int64)v52) >> 16;
      pPosX = a3 + spawn->vPosition.x;
      v35 = stru_5C6E00->Sin(v33);
      a4 = v35;
      a3 = (unsigned __int64)(v35 * (signed __int64)v52) >> 16;
      a4 = a3 + spawn->vPosition.y;
      v36 = spawn->vPosition.z;
      a3 = spawn->vPosition.z;
      if ( uCurrentlyLoadedLevelType == LEVEL_Outdoor )
	  {
        if ( a5 )
            pMonster->uAttributes |= 0x080000;
        ++uNumActors;
        continue;
	  }
      v37 = pIndoor->GetSector(pPosX, a4, v36);
      if ( v37 == pSector )
      {
        v38 = BLV_GetFloorLevel(pPosX, a4, a3, v37, &uFaceID);
        v39 = v38;
        if ( v38 != -30000 )
        {
          if ( abs(v38 - a3) <= 1024 )
          {
            a3 = v39;
            if ( a5 )
              pMonster->uAttributes |= 0x080000;
            ++uNumActors;
            continue;
          }
        }
      }
      ;
      //v53 = (char *)v53 + 1;
      //result = v53;
    }
    //while ( (signed int)v53 < v56 );
}

//----- (0044FA4C) --------------------------------------------------------
signed int __fastcall sub_44FA4C_spawn_light_elemental(int a1, int a2, int a3)
{
  signed int v3; // ecx@6
  Actor *v4; // edx@7
  signed int result; // eax@13
  Actor *v6; // esi@16
  char *v7; // ebx@16
  MonsterDesc *v8; // edi@16
  unsigned __int16 v9; // ax@16
  int v10; // ebx@16
  //int v11; // edi@16
  //int v12; // eax@16
  //int v13; // ecx@16
  //int v14; // ebx@16
  const char *v15; // [sp-4h] [bp-24h]@2
  //unsigned __int16 v16; // [sp+0h] [bp-20h]@1
  //int v17; // [sp+4h] [bp-1Ch]@1
  unsigned int uFaceID; // [sp+8h] [bp-18h]@16
  int v19; // [sp+Ch] [bp-14h]@16
  size_t v20; // [sp+10h] [bp-10h]@6
  int v21; // [sp+14h] [bp-Ch]@14
  //int v22; // [sp+18h] [bp-8h]@14
  unsigned int v23; // [sp+1Ch] [bp-4h]@6

  //v16 = a2;
  //v17 = a1;
  if ( a2 == 4 )
  {
    v15 = "Elemental Light C";
  }
  else if ( a2 == 3 )
      v15 = "Elemental Light B";
    else
      v15 = "Elemental Light A";

  v23 = pMonsterList->GetMonsterIDByName(v15);
  v3 = 0;
  v20 = uNumActors;
  if ( (signed int)uNumActors > 0 )
  {
    v4 = pActors.data();//[0].uAIState;
    while ( v4->uAIState != Removed )
    {
      ++v3; 
      ++v4;
      if ( v3 >= (signed int)uNumActors )
        break;
    }
	if( v3 < (signed int)uNumActors )
		v20 = v3;
  }
  if ( v20 != uNumActors || (result = uNumActors + 1, (signed int)(uNumActors + 1) < 500) )
  {
    v21 = 0;
    //v22 = pParty->vPosition.z;
    if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
      v21 = pIndoor->GetSector(pParty->vPosition.x, pParty->vPosition.y, pParty->vPosition.z);
    v6 = &pActors[v20];
    v7 = (char *)&pMonsterStats->pInfos[v23 + 1];
    v19 = (((uCurrentlyLoadedLevelType != LEVEL_Outdoor) - 1) & 0x40) + 64;
    v6->Reset();
    v8 = &pMonsterList->pMonsters[v23];
    strcpy(v6->pActorName, *(const char **)v7);
    v6->sCurrentHP = *((short *)v7 + 32);
    memcpy(&v6->pMonsterInfo, v7, 0x58u);
    v6->word_000086_some_monster_id = v23 + 1;
    v6->uActorRadius = v8->uMonsterRadius;
    v6->uActorHeight = v8->uMonsterHeight;
    v9 = v8->uMovementSpeed;
    v6->pMonsterInfo.uTreasureDiceRolls = 0;
    v6->pMonsterInfo.uTreasureType = 0;
    v6->pMonsterInfo.uExp = 0;
    v6->uMovementSpeed = v9;
    v10 = rand() % 2048;
    //v11 = pParty->vPosition.x + fixpoint_mul(stru_5C6E00->Cos(v10), v19);
    uFaceID = stru_5C6E00->Sin(v10);
    //v12 = pParty->vPosition.y;
    //v13 = fixpoint_mul(uFaceID, v19);
    //v14 = pParty->vPosition.y + fixpoint_mul(uFaceID, v19);
    //LOWORD(v12) = v22;
    v6->vInitialPosition.x = pParty->vPosition.x + fixpoint_mul(stru_5C6E00->Cos(v10), v19);
    v6->vPosition.x = v6->vInitialPosition.x;
    v6->vInitialPosition.y = pParty->vPosition.y + fixpoint_mul(uFaceID, v19);
    v6->vPosition.y = v6->vInitialPosition.y;
    v6->vInitialPosition.z = pParty->vPosition.z;
    v6->vPosition.z = v6->vInitialPosition.z;
    //LOWORD(v12) = v21;
    v6->uTetherDistance = 256;
    v6->uSectorID = v21;
    v6->PrepareSprites(0);
    v6->pMonsterInfo.uHostilityType = MonsterInfo::Hostility_Friendly;
    v6->uAlly = 9999;
    v6->uGroup = 0;
    v6->uCurrentActionTime = 0;
    v6->uAIState = Summoned;
    v6->uCurrentActionLength = 256;
    v6->UpdateAnimation();
    if ( uCurrentlyLoadedLevelType == LEVEL_Outdoor
      || (result = pIndoor->GetSector(v6->vPosition.x, v6->vPosition.y, v6->vPosition.z),
          result == v21)
      && (result = BLV_GetFloorLevel(v6->vPosition.x, v6->vPosition.y, v6->vPosition.z, result, &uFaceID), result != -30000)
      && (result = abs(result - pParty->vPosition.z), result <= 1024) )
    {
      if ( v20 == uNumActors )
        ++uNumActors;
      v6->uSummonerID = PID(OBJECT_Player, a1);
      result = v6->pActorBuffs[2].Apply(pParty->uTimePlayed + (a3 * 128) / 30.0f,
                 a2,
                 a1,
                 0,
                 0);
    }
  }
  return result;
}

//----- (0044FFD8) --------------------------------------------------------
int MapInfo::SpawnRandomTreasure(SpawnPointMM7 *a2)
{
  //MapInfo *v2; // ebx@1
  //SpawnPointMM7 *v3; // esi@1
  //int v4; // eax@1
  int v5; // edx@1
  int v6; // eax@1
  int v7; // ecx@1
  int v8; // ebx@1
  int v9; // eax@1
  signed int v10; // ebx@1
  signed int result; // eax@1
  signed __int64 v12; // qtt@1
  int v13; // ebx@1
  int v14; // edx@10
  signed int v15; // ebx@20
  unsigned __int16 v16; // dx@20
  ObjectDesc *v17; // ecx@21
  unsigned __int16 v18; // ax@24
  int v19; // ST0C_4@27
  int v20; // ST08_4@27
  int v21; // ST04_4@27
  int v22; // eax@27
  signed int v23; // ebx@29
  unsigned __int16 v24; // dx@29
  ObjectDesc *v25; // ecx@30
  unsigned __int16 v26; // ax@33
  //int v27; // ecx@35
  //int v28; // eax@35
  //int v29; // esi@35
  //__int16 v30; // ax@35
  SpriteObject a1a; // [sp+Ch] [bp-7Ch]@1
  //int v32; // [sp+7Ch] [bp-Ch]@1
  //int v33; // [sp+80h] [bp-8h]@1
  int v34; // [sp+84h] [bp-4h]@1

  //auto a1 = this;
  //v2 = a1;
  //v3 = a2;
  //v4 = rand();
  v34 = 0;
  v5 = rand() % 100;
 // v6 = 2 * (v2->Treasure_prob + 7 * v3->uIndex) - 14;
  v7 = (unsigned __int8)byte_4E8168[a2->uIndex - 1][2 * Treasure_prob];
  v8 = (unsigned __int8)byte_4E8168[a2->uIndex - 1][2 * Treasure_prob + 1];
  //v32 = v5;
  //v33 = v7;
  v9 = rand();
  v10 = v8 - v7 + 1;
  v12 = v9;
  result = v9 / v10;
  v13 = v7 + (unsigned __int64)(v12 % v10);
  if ( v13 < 7 )
  {
    if ( v5 < 20 )
      return result;
    if ( v5 >= 60 )
    {
      v19 = a2->vPosition.z;
      v20 = a2->vPosition.y;
      v21 = a2->vPosition.x;
      v22 = rand();
      return sub_450521_ProllyDropItemAt(v13, v22 % 27 + 20, v21, v20, v19, 0);
    }
    if ( a2->uIndex == 1 )
    {
      v14 = rand() % 51 + 50;
	  a1a.stru_24.uItemID = 197;
	  v34 = v14;
	}
    else if ( a2->uIndex == 2 )
    {
		v14 = rand() % 101 + 100;
		a1a.stru_24.uItemID = 197;
		v34 = v14;
	}
        else if ( a2->uIndex == 3 )
        {
          v14 = rand() % 301 + 200;
          a1a.stru_24.uItemID = 198;
		  v34 = v14;
        }
        else if ( a2->uIndex == 4 )
        {
			  v14 = rand() % 501 + 500;
			  a1a.stru_24.uItemID = 198;
			  v34 = v14;
		}
		else if ( a2->uIndex == 5 )
        {
			v14 = rand() % 1001 + 1000;
            a1a.stru_24.uItemID = 199;
            v34 = v14;
        }
        else if ( a2->uIndex == 6 )
        {
				  v14 = rand() % 3001 + 2000;
				  a1a.stru_24.uItemID = 199;
				  v34 = v14;
		}	 
            v15 = 0;
            v16 = pItemsTable->pItems[a1a.stru_24.uItemID].uSpriteID;
            a1a.uType = pItemsTable->pItems[a1a.stru_24.uItemID].uSpriteID;
            v18 = 0;
			for( int i = 0; i < pObjectList->uNumObjects; i++ )
            {
				if ( pObjectList->pObjects[i].uObjectID == v16 )
				{
					v18 = i;
					break;
				}
            }
            a1a.stru_24.SetIdentified();
            a1a.uObjectDescID = v18;
            a1a.stru_24.uSpecEnchantmentType = v34;
  }
  else
  {
	  result = a1a.stru_24.GenerateArtifact();
	  if ( !result )
		return result;
	  v23 = 0;
	  v24 = pItemsTable->pItems[a1a.stru_24.uItemID].uSpriteID;
	  a1a.uType = pItemsTable->pItems[a1a.stru_24.uItemID].uSpriteID;
	  v26 = 0;
		for( int i = 0; i < pObjectList->uNumObjects; i++ )
		{
			if( v24 == pObjectList->pObjects[i].uObjectID )
			{
				v26 = i;
				break;
			}
		}
	  a1a.uObjectDescID = v26;
	  a1a.stru_24.Reset();
  }
  a1a.vPosition.y = a2->vPosition.y;
  a1a.uAttributes = 0;
  a1a.uSoundID = 0;
  a1a.uFacing = 0;
  a1a.vPosition.z = a2->vPosition.z;
  a1a.vPosition.x = a2->vPosition.x;
  a1a.spell_skill = 0;
  a1a.spell_level = 0;
  a1a.spell_id = 0;
  a1a.spell_target_pid = 0;
  a1a.spell_caster_pid = 0;
  a1a.uSpriteFrameID = 0;
  a1a.uSectorID = pIndoor->GetSector(a2->vPosition.x, a2->vPosition.y, a2->vPosition.z);;
  return a1a.Create(0, 0, 0, 0);
}

//----- (00450521) --------------------------------------------------------
int __fastcall sub_450521_ProllyDropItemAt(int ecx0, signed int a2, int a3, int a4, int a5, unsigned __int16 a6)
{
  int v6; // edi@1
  int v7; // esi@1
  signed int v8; // edi@1
  unsigned __int16 v9; // cx@1
  char *v10; // edx@2
  unsigned __int16 v11; // ax@5
  SpriteObject a1; // [sp+8h] [bp-70h]@1

  v6 = ecx0;
  v7 = a2;
  pItemsTable->GenerateItem(v6, v7, &a1.stru_24);
  v8 = 0;
  v9 = pItemsTable->pItems[a1.stru_24.uItemID].uSpriteID;
  a1.uType = pItemsTable->pItems[a1.stru_24.uItemID].uSpriteID;
  v11 = 0;
	for( int i = 0; i < pObjectList->uNumObjects; i++ )
	{
		if( v9 == pObjectList->pObjects[i].uObjectID )
		{
			v11 = i;
			break;
		}
	}
  a1.uObjectDescID = v11;
  a1.vPosition.y = a4;
  a1.vPosition.x = a3;
  a1.vPosition.z = a5;
  a1.uFacing = a6;
  a1.uAttributes = 0;
  a1.uSectorID = pIndoor->GetSector(a3, a4, a5);
  a1.uSpriteFrameID = 0;
  return a1.Create(0, 0, 0, 0);
}

//----- (0045063B) --------------------------------------------------------
int __fastcall _45063B_spawn_some_monster(MapInfo *a1, int a2)
{
  signed int v2; // edi@1
  Actor *v3; // esi@2
  //signed __int64 v4; // qax@3
  int result; // eax@8
  int v6; // edi@11
  int v7; // ebx@11
  int v8; // edi@11
  int v9; // ebx@12
  int v10; // eax@12
  char v11; // zf@16
  int v12; // edi@20
  int v13; // eax@20
  int v14; // ebx@20
  int v15; // eax@20
  int v16; // eax@20
  int v17; // eax@20
  int v18; // eax@21
  SpawnPointMM7 v19; // [sp+Ch] [bp-38h]@1
  int v20; // [sp+24h] [bp-20h]@11
  //MapInfo *v21; // [sp+28h] [bp-1Ch]@1
  int v22; // [sp+2Ch] [bp-18h]@3
  int v23; // [sp+30h] [bp-14h]@11
  //int v24; // [sp+34h] [bp-10h]@1
  unsigned int uFaceID; // [sp+38h] [bp-Ch]@10
  int v26; // [sp+3Ch] [bp-8h]@11
  int v27; // [sp+40h] [bp-4h]@11
  
  if (!uNumActors)
    return 0;

  //v21 = a1;
  //v24 = a2;
  v2 = 0;
  v3 = pActors.data();
  while ( 1 )
  {
    if (v3->pMonsterInfo.uID >= 121 && v3->pMonsterInfo.uID <= 123 || // Dwarf FemaleC A-C
        v3->pMonsterInfo.uID >= 124 && v3->pMonsterInfo.uID <= 126 || // Dwarf MaleA A-C
        v3->pMonsterInfo.uID >= 133 && v3->pMonsterInfo.uID <= 135 || // Peasant Elf FemaleA A-C
        !v3->CanAct())
    {
      ++v2;
      ++v3;

      if (v2 >= uNumActors)
        return 0;

      continue;
    }

    break;
    //v22 = v3->pMonsterInfo.uID - 1;
    //v4 = (signed __int64)((double)v22 * 0.3333333333333333);
    //if ( (int)v4 != 40 )
    //{
    //  if ( (int)v4 != 41 && (int)v4 != 44 && v3->CanAct() )
    //    break;
    //}
    //++v2;
    //++v3;
    //if ( v2 >= (signed int)uNumActors )
    //  goto LABEL_8;
  }
  if ( uCurrentlyLoadedLevelType == LEVEL_Outdoor )
  {
    v22 = 0;
    uFaceID = 0;
    while ( 1 )
    {
      ++uFaceID;
      v6 = rand() % 1024 + 512;
      v7 = rand() % (signed int)stru_5C6E00->uIntegerDoublePi;
      v20 = stru_5C6E00->Cos(v7);
      v23 = (unsigned __int64)(v20 * (signed __int64)v6) >> 16;
      v19.vPosition.x = pParty->vPosition.x + v23;
      v20 = stru_5C6E00->Sin(v7);
      v23 = (unsigned __int64)(v20 * (signed __int64)v6) >> 16;
      v8 = 0;
      v19.uIndex = a2;
      v19.vPosition.y = v23 + pParty->vPosition.y;
      v19.vPosition.z = pParty->vPosition.z;
      v26 = 0;
      v27 = 0;
      v19.vPosition.z = ODM_GetFloorLevel(
                          v19.vPosition.x,
                          v23 + pParty->vPosition.y,
                          pParty->vPosition.z,
                          0,
                          &v26,
                          &v27,
                          0);
      v23 = 0;
	  for( int i = 0; i < pOutdoor->uNumBModels; i++ )
	  {
		v9 = abs(v19.vPosition.y - pOutdoor->pBModels[i].vBoundingCenter.y);
		v10 = abs(v19.vPosition.x - pOutdoor->pBModels[i].vBoundingCenter.x);
		if ( int_get_vector_length(v10, v9, 0) < pOutdoor->pBModels[i].sBoundingRadius + 256 )
		{
		  v22 = 1;
		  break;
		}
	  }
      v11 = uFaceID == 100;
      if ( uFaceID >= 100 )
        break;
      if ( v22 )
      {
        v11 = uFaceID == 100;
        break;
      }
    }
  }
  else
  {
    v26 = 0;
    v22 = pIndoor->GetSector(pParty->vPosition.x, pParty->vPosition.y, pParty->vPosition.z);
    do
    {
      ++v26;
      v12 = rand() % 512 + 256;
      v13 = rand();
      v14 = v13 % (signed int)stru_5C6E00->uIntegerDoublePi;
      v15 = stru_5C6E00->Cos(v13 % (signed int)stru_5C6E00->uIntegerDoublePi);
      v20 = v15;
      v23 = (unsigned __int64)(v15 * (signed __int64)v12) >> 16;
      v19.vPosition.x = pParty->vPosition.x + v23;
      v16 = stru_5C6E00->Sin(v13 % (signed int)stru_5C6E00->uIntegerDoublePi);
      v20 = v16;
      v23 = (unsigned __int64)(v16 * (signed __int64)v12) >> 16;
      v19.vPosition.y = v23 + pParty->vPosition.y;
      v19.vPosition.z = pParty->vPosition.z;
      v19.uIndex = a2;
      v17 = pIndoor->GetSector(v19.vPosition.x, v23 + pParty->vPosition.y, pParty->vPosition.z);
      if ( v17 == v22 )
      {
        v18 = BLV_GetFloorLevel(v19.vPosition.x, v19.vPosition.y, v19.vPosition.z, v17, &uFaceID);
        v19.vPosition.z = v18;
        if ( v18 != -30000 )
        {
          if ( abs(v18 - pParty->vPosition.z) <= 1024 )
            break;
        }
      }
    }
    while ( v26 < 100 );
    v11 = v26 == 100;
  }
  if ( v11 )
  {
    result = 0;
  }
  else
  {
    SpawnEncounter(a1, &v19, 0, 0, 1);
    result = a2;
  }
  return result;
}

//----- (00450AAA) --------------------------------------------------------
void RespawnGlobalDecorations()
{
  memset(stru_5E4C90_MapPersistVars._decor_events.data(), 0, 125);

  uint decorEventIdx = 0;
  for (uint i = 0; i < uNumLevelDecorations; ++i)
  {
    auto decor = &pLevelDecorations[i];

    if (!decor->uEventID)
    {
      if (decor->IsInteractive())
      {
        if (decorEventIdx < 124)
        {
          decor->_idx_in_stru123 = decorEventIdx + 75;
          stru_5E4C90_MapPersistVars._decor_events[decorEventIdx++] = decor->GetGlobalEvent();
        }
      }
    }
  }
}

//----- (00450B0A) --------------------------------------------------------
bool __fastcall SpawnActor(unsigned int uMonsterID)
{
  unsigned int v1; // ebx@1
  bool result; // eax@2
  MonsterDesc *v3; // esi@5
  MonsterInfo *v4; // edi@5
  Vec3_int_ v5; // ST08_12@5
  unsigned int v6; // ecx@5
  Actor *v7; // eax@7
  Actor actor; // [sp+4h] [bp-350h]@5
  Vec3_int_ pOut; // [sp+348h] [bp-Ch]@5

  v1 = uMonsterID;
  if ( uNumActors == 499 )
  {
    result = 0;
  }
  else
  {
    if ( (signed int)uMonsterID >= (signed int)pMonsterList->uNumMonsters )
      v1 = 0;
    v3 = &pMonsterList->pMonsters[v1];
    v4 = &pMonsterStats->pInfos[v1 + 1];
    memset(&actor, 0, 0x344u);
    strcpy(actor.pActorName, v4->pName);
    actor.sCurrentHP = LOWORD(v4->uHP);
    memcpy(&actor.pMonsterInfo, &pMonsterStats->pInfos[v1 + 1], 0x58u);
    actor.word_000086_some_monster_id = v1 + 1;
    actor.uActorRadius = v3->uMonsterRadius;
    actor.uActorHeight = v3->uMonsterHeight;
    actor.uMovementSpeed = v3->uMovementSpeed;
    v5 = pParty->vPosition;
    Vec3_int_::Rotate(200, pParty->sRotationY, 0, v5, &pOut.x, &pOut.z, &pOut.y);
    actor.vInitialPosition.x = pOut.x;
    actor.vPosition.x = pOut.x;
    actor.uTetherDistance = 256;
    actor.vInitialPosition.y = LOWORD(pOut.z);
    actor.vPosition.y = LOWORD(pOut.z);
    actor.vInitialPosition.z = LOWORD(pOut.y);
    actor.vPosition.z = LOWORD(pOut.y);
    pSprites_LOD->DeleteSomeSprites();
    pPaletteManager->ResetNonTestLocked();
    v6 = uNumActors - 1;
    if ( dword_5C6DF8 == 1 )
    {
      dword_5C6DF8 = 0;
      v6 = uNumActors++;
    }
    v7 = &pActors[v6];
    memcpy(v7, &actor, 0x344u);
    v7->PrepareSprites(1);
    result = 1;
  }
  return result;
}
// 5C6DF8: using guessed type int dword_5C6DF8;

//----- (00450DA3) --------------------------------------------------------
int  GetAlertStatus()
{
  int result; // eax@2

  if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
    result = pOutdoor->ddm.field_C_alert;
  else
    result = uCurrentlyLoadedLevelType == LEVEL_Outdoor ? pIndoor->dlv.field_C_alert : 0;
  return result;
}

//----- (00450DDE) --------------------------------------------------------
stru350 *stru350::_450DDE()
{
  _450DF1(&stru_4E82A4, &stru_4E82A4);
  return this;
}

//----- (00450DF1) --------------------------------------------------------
bool stru350::_450DF1(const stru355 *p1, const stru355 *p2)
{
  //stru350 *v3; // esi@1
  //void *result; // eax@1
  unsigned int v5; // ecx@2
  int v6; // edi@2
  int v7; // edx@2
  unsigned int v8; // ecx@8
  int v9; // edi@8
  int v10; // edx@8
  int v11; // ecx@12
  int v12; // edi@12
  unsigned int v13; // ecx@12
  int v14; // edx@12
  int v15; // ecx@16
  unsigned int v16; // edx@16
  int v17; // ecx@16
  int v18; // edi@16
  char v19; // zf@20
  unsigned int v20; // ecx@21
  int v21; // edi@21
  int v22; // edx@21
  unsigned int v23; // ecx@27
  int v24; // edi@27
  int v25; // edx@27
  int v26; // ecx@31
  int v27; // edi@31
  unsigned int v28; // ecx@31
  int v29; // edx@31
  int v30; // ebx@35
  int v31; // ecx@35
  int v32; // edi@35
  int v33; // edx@35
  unsigned int i; // ecx@35
  int v35; // ecx@39
  unsigned int v36; // edx@39
  int v37; // ecx@39
  int v38; // ebx@39

  //v3 = this;
  memcpy(&field_0, p1, sizeof(stru355));
  memcpy(&field_20, p2, sizeof(stru355));
  //result = memcpy(&v3->field_20, p2, 0x20u);
  //LOBYTE(result) = 1;
  if (field_0.field_4 & 1)
  {
    v5 = field_0.field_1C;
    v6 = 0;
    v7 = 0;
    while ( !(v5 & 1) )
    {
      ++v6;
      v5 >>= 1;
    }
    do
    {
      v5 >>= 1;
      ++v7;
    }
    while ( v5 & 1 );
    field_40 = 32 - v7 - v6;
  }
  else
  {
    field_40 = 0;
  }
  v8 = field_0.field_10;
  v9 = 0;
  v10 = 0;
  while ( !(v8 & 1) )
  {
    ++v9;
    v8 >>= 1;
  }
  do
  {
    v8 >>= 1;
    ++v10;
  }
  while ( v8 & 1 );
  v11 = 24 - v10 - v9;
  v12 = 0;
  field_48 = v11;
  v13 = field_0.field_14;
  v14 = 0;
  while ( !(v13 & 1) )
  {
    ++v12;
    v13 >>= 1;
  }
  do
  {
    v13 >>= 1;
    ++v14;
  }
  while ( v13 & 1 );
  v15 = 16 - v14;
  v16 = field_0.field_18;
  field_50 = v15 - v12;
  v17 = 0;
  v18 = 0;
  while ( !(v16 & 1) )
  {
    ++v17;
    v16 >>= 1;
  }
  do
  {
    v16 >>= 1;
    ++v18;
  }
  while ( v16 & 1 );
  v19 = (field_20.field_4 & 1) == 0;
  field_58 = v17 - v18 + 8;
  if ( v19 )
  {
    field_44 = 0;
  }
  else
  {
    v20 = field_20.field_1C;
    v21 = 0;
    v22 = 0;
    while ( !(v20 & 1) )
    {
      ++v21;
      v20 >>= 1;
    }
    do
    {
      v20 >>= 1;
      ++v22;
    }
    while ( v20 & 1 );
    field_44 = 32 - v22 - v21;
  }
  v23 = field_20.field_10;
  v24 = 0;
  v25 = 0;
  while ( !(v23 & 1) )
  {
    ++v24;
    v23 >>= 1;
  }
  do
  {
    v23 >>= 1;
    ++v25;
  }
  while ( v23 & 1 );
  v26 = 24 - v25 - v24;
  v27 = 0;
  field_4C = v26;
  v28 = field_20.field_14;
  v29 = 0;
  while ( !(v28 & 1) )
  {
    ++v27;
    v28 >>= 1;
  }
  do
  {
    v28 >>= 1;
    ++v29;
  }
  while ( v28 & 1 );
  v30 = 0;
  v31 = 16 - v29 - v27;
  v32 = field_20.field_18;
  field_54 = v31;
  v33 = 0;
  for ( i = v32; !(i & 1); i >>= 1 )
    ++v30;
  do
  {
    i >>= 1;
    ++v33;
  }
  while ( i & 1 );
  v35 = 32 - v33;
  v36 = v32;
  field_5C = v35 - v30;
  v37 = 0;
  v38 = 0;
  while ( !(v36 & 1) )
  {
    ++v37;
    v36 >>= 1;
  }
  do
  {
    v36 >>= 1;
    ++v38;
  }
  while ( v36 & 1 );
  field_5C = v37 - v38 + 8;
  return true;
}

//----- (00450F55) --------------------------------------------------------
unsigned int stru350::_450F55(int a2)
{
  int v2 = a2 & stru_4E82A4.field_1C;
  if (field_20.field_4 & 1)
    v2 = (unsigned int)v2 >> this->field_44;
  return v2 & field_20.field_1C |
         field_20.field_10 & ((a2 & (unsigned int)stru_4E82A4.field_10) >> field_4C) |
         field_20.field_14 & ((a2 & (unsigned int)stru_4E82A4.field_14) >> field_54) |
         field_20.field_18 & ((a2 & (unsigned int)stru_4E82A4.field_18) >> field_5C);
}

//----- (00450FB1) --------------------------------------------------------
int stru350::_450FB1(int a2)
{
  int v2 = 0;
  int v4 = field_0.field_4 & 1;
  if ( v4 )
    v2 = a2 & field_0.field_1C;
  if ( v4 )
    v2 <<= field_40;
  return v2 | ((a2 & field_0.field_10) << field_48) | ((a2 & field_0.field_14) << field_50) | ((a2 & field_0.field_18) << field_58);
}

//----- (00452442) --------------------------------------------------------
unsigned int __fastcall _452442_color_cvt(unsigned __int16 a1, unsigned __int16 a2, int a3, int a4)
{
  int v4; // ebx@0
  __int16 v5; // ST14_2@1
  __int16 v6; // dx@1
  int v7; // ecx@1
  __int16 v8; // ST10_2@1
  int v9; // edi@1
  unsigned __int16 v10; // dh@1@1
  int v11; // ebx@1
  int v12; // ebx@1
  __int16 a3a; // [sp+1Ch] [bp+8h]@1

  v5 = a2 >> 2;
  v6 = (unsigned __int16)a4 >> 2;
  v8 = a1 >> 2;
  a3a = (unsigned __int16)a3 >> 2;
  LOWORD(v7) = a3a;
  v9 = v7;
  LOWORD(v4) = ((unsigned __int16)a4 >> 2) & 0xE0;
  LOWORD(v7) = a3a & 0xE0;
  LOWORD(v9) = v9 & 0x1C00;
  v11 = v7 + v4;
  LOWORD(v7) = v5 & 0xE0;
  v12 = v7 + v11;
  LOWORD(v7) = v8 & 0xE0;
  __debugbreak(); // warning C4700: uninitialized local variable 'v10' used
  return (PID_TYPE(v8) + PID_TYPE(v5) + PID_TYPE(a3a) + PID_TYPE(v6)) | (v7 + v12) | ((v8 & 0x1C00)
                                                                    + (v5 & 0x1C00)
                                                                    + v9
                                                                    + (__PAIR__(v10, (unsigned __int16)a4 >> 2) & 0x1C00));
}

//----- (0045281E) --------------------------------------------------------
//    Calculates atan2(y/x)
// return value: angle in integer format (multiplier of Pi/1024)  
unsigned int stru193_math::Atan2(int x, int y)
{
  signed int quadrant;
  __int64 dividend;
  int quotient;
  int lowIdx;
  int highIdx;
  int angle;

  auto X = x;
  auto Y = y;

  if ( abs(X) < 65536 )
  {
    if ( (abs(Y) >> 15) >= abs(X) )
      X = 0;
  }

  if ( !X )
  {
    if ( Y > 0 )
    {
      return uIntegerHalfPi;   //Pi/2
    }
    else
    {
      return uIntegerHalfPi + uIntegerPi; //3*(Pi/2)
    }
  }

  if ( Y )
  {
    if ( X < 0 )
    {
      X = -X;
      if ( Y > 0 )
      {
        quadrant = 4;        
      }
      else
      {
        quadrant = 3;        
      }      
    }
    else
    {
      if ( Y > 0 )
      {
        quadrant = 1;       
      }
      else
      {
        quadrant = 2;
      }      
    }

    if ( Y < 0 )
      Y = -Y;

    LODWORD(dividend) = Y << 16;
    HIDWORD(dividend) = Y >> 16;
    quotient = dividend / X;        

    //looks like binary search
    {
      int i;
      highIdx = uIntegerHalfPi;
      lowIdx = 0;

      for (i = 0; i < 6; ++i)
      {        
        if (quotient <= pTanTable[(lowIdx + highIdx) / 2])      
          highIdx = (lowIdx + highIdx) / 2;
        else
          lowIdx = (lowIdx + highIdx) / 2;    
      }
    }

    angle = lowIdx + 1;
    while ( angle < (highIdx - 1) && quotient >= pTanTable[angle] )
      ++angle;

    switch (quadrant)
    {
    case 1: //X > 0, Y > 0
      return angle;        

    case 2: //X > 0, Y < 0
      return uIntegerDoublePi - angle;   //2*Pi - angle

    case 3: //X > 0, Y < 0
      return uIntegerPi + angle;        //Pi + angle 

    case 4: //X < 0, Y > 0
      return uIntegerPi - angle;        //Pi - angle  
    }

    //should newer get here
    return 0;
  }

  if ( X < 0 )    //Y == 0, X < 0
    return uIntegerPi;  

  return 0;
}

//----- (00452969) --------------------------------------------------------
stru193_math::stru193_math()
{
  double v3; // ST18_8@2

  this->pTanTable[0] = 0;
  this->pCosTable[0] = 65536;
  this->pInvCosTable[0] = 65536;
  for(int i = 1; i < (signed int)this->uIntegerHalfPi; i++)
  {
    v3 = (double)i * 3.141592653589793 / (double)uIntegerPi;
    pTanTable[i] = (signed __int64)(tan(v3) * (double)this->pCosTable[0] + 0.5);
    pCosTable[i] = (signed __int64)(cos(v3) * (double)this->pCosTable[0] + 0.5);
    pInvCosTable[i] = (signed __int64)(1.0 / cos(v3) * (double)this->pCosTable[0] + 0.5);
  }
  for(int i = this->uIntegerHalfPi; i < 520; i++)
  {
    this->pTanTable[i] = 0xEFFFFFFFu;
    this->pCosTable[i] = 0;
    this->pInvCosTable[i] = 0xEFFFFFFFu;
  }
}

//----- (00452A9E) --------------------------------------------------------
int integer_sqrt(int val)
{
  signed int result; // eax@2
  unsigned int v2; // edx@3
  unsigned int v3; // edi@3
  //signed int v4; // ebx@3
  int v5; // esi@4

  if (val < 1)
    return val;


    v2 = 0;
    v3 = val;
    result = 0;
    //v4 = 16;
  for (uint i = 0; i < 16; ++i)
  {
      result *= 2;
      v2 = (v3 >> 30) | 4 * v2;
      v5 = 2 * result + 1;
      v3 *= 4;
      if ( v2 >= v5 )
      {
        ++result;
        v2 -= v5;
      }
      //--v4;
  }
    //while ( v4 );
    if ( val - result * result >= (unsigned int)(result - 1) )
      ++result;
    return result;
}

//----- (00452AE2) --------------------------------------------------------
int __fastcall MakeColorMaskFromBitDepth(int a1)
{
  signed __int64 v1; // qax@1

  v1 = 4294967296i64;
  if ( a1 > 0 )
  {
    do
    {
      LODWORD(v1) = HIDWORD(v1) + v1;
      HIDWORD(v1) *= 2;
      --a1;
    }
    while ( a1 );
  }
  return v1;
}

//----- (00452AF3) --------------------------------------------------------
void __fastcall fill_pixels_fast(unsigned int a1, unsigned __int16 *pPixels, unsigned int uNumPixels)
{
  void *v3; // edi@1
  unsigned int v4; // eax@1
  unsigned __int16 *v5; // edi@3
  unsigned int i; // ecx@3

  v3 = pPixels;
  v4 = a1 | (a1 << 16);
  if ( (unsigned __int8)pPixels & 2 )           // first 2 pixels
  {
    *pPixels = v4;
    v3 = pPixels + 1;
    --uNumPixels;
  }
  memset32(v3, v4, uNumPixels >> 1);            // 4 pixels at once
  v5 = (unsigned __int16 *)((char *)v3 + 4 * (uNumPixels >> 1));
  for ( i = uNumPixels & 1; i; --i )            // leftover pixels
  {
    *v5 = v4;
    ++v5;
  }
}

//----- (00452B2E) --------------------------------------------------------
int __fastcall GetDiceResult(unsigned int uNumDice, unsigned int uDiceSides)
{
  signed int v2; // edi@1
  int v3; // esi@1
  int result; // eax@2
  unsigned int v5; // ebx@4

  v2 = uDiceSides;
  v3 = 0;
  if ( uDiceSides )
  {
    if ( (signed int)uNumDice > 0 )
    {
      v5 = uNumDice;
      do
      {
        --v5;
        v3 += rand() % v2 + 1;
      }
      while ( v5 );
    }
    result = v3;
  }
  else
  {
    result = 0;
  }
  return result;
}

//----- (00453F62) --------------------------------------------------------
void MapStats::Initialize()
{
  char work_str[32]; // [sp+Ch] [bp-34h]@3
  int work_str_pos;
  int work_str_len;
  int i;
  char* test_string;
  unsigned char c;
  bool break_loop;
  unsigned int temp_str_len;
  char* tmp_pos;
  int decode_step;
  int item_counter;

  if ( pMapStatsTXT_Raw )
    free(pMapStatsTXT_Raw);
  pMapStatsTXT_Raw = NULL;
  pMapStatsTXT_Raw = (char *)pEvents_LOD->LoadRaw("MapStats.txt", 0);
  strtok(pMapStatsTXT_Raw, "\r");
  strtok(NULL, "\r");
  strtok(NULL, "\r");

 for (i=1; i<77; ++i)
	  {
	  test_string = strtok(NULL, "\r") + 1;
	  break_loop = false;
	  decode_step=0;
	  do 
		  {
		  c = *(unsigned char*)test_string;
		  temp_str_len = 0;
		  while((c!='\t')&&(c>0))
			  {
			  ++temp_str_len;
			  c=test_string[temp_str_len];
			  }		
		  tmp_pos=test_string+temp_str_len;
		  if (*tmp_pos == 0)
			  break_loop = true;
		  *tmp_pos = 0;
		  if (temp_str_len)
			  {
			  switch (decode_step)
				  {
				  case 1:
					  pInfos[i].pName = RemoveQuotes(test_string);
					  break;
				  case 2:
					  pInfos[i].pFilename = RemoveQuotes(test_string);
					  break;
				  case 3:
					  pInfos[i].uNumResets = atoi(test_string);
					  break;
				  case 4:
					  pInfos[i].uFirstVisitedAt = atoi(test_string);
					  break;
				  case 5:
					  pInfos[i]._per = atoi(test_string);
					  break;
				  case 6:
					  pInfos[i].uRespawnIntervalDays = atoi(test_string);
					  break;
				  case 7:
					  pInfos[i]._alert_days = atoi(test_string);
					  break;
				  case 8:
					  pInfos[i]._steal_perm = atoi(test_string);
					  break;
				  case 9:
					  pInfos[i].LockX5 = atoi(test_string);
					  break;
				  case 10:
					  pInfos[i].Trap_D20 = atoi(test_string);
					  break;
				  case 11:
					  pInfos[i].Treasure_prob = atoi(test_string);
					  break;
				  case 12:
					  pInfos[i].Encounter_percent = atoi(test_string);
					  break;
				  case 13:
					  pInfos[i].EncM1percent = atoi(test_string);
					  break;
				  case 14:
					  pInfos[i].EncM2percent = atoi(test_string);
					  break;
				  case 15:
					  pInfos[i].EncM3percent = atoi(test_string);
					  break;
				  case 16:
					  pInfos[i].pEncounterMonster1Texture = RemoveQuotes(test_string);
					  break;
				  case 18:
					  pInfos[i].Dif_M1 = atoi(test_string);
					  break;
				  case 19:
					  pInfos[i].uEncounterMonster1AtLeast = 1;
					  pInfos[i].uEncounterMonster1AtMost = 1;
					  strcpy(work_str, test_string);
					  work_str_pos = 0;
					  work_str_len=strlen(work_str);
					  if (work_str_len )
						  {
						  while (work_str[work_str_pos] != '-' )
							  {
							  ++work_str_pos;
							  if (work_str_pos >= work_str_len )
								  break;
							  }
						  work_str[work_str_pos] = 0;
						  pInfos[i].uEncounterMonster1AtLeast = atoi(work_str);
						  if ( work_str_pos < work_str_len )
							  pInfos[i].uEncounterMonster1AtMost = atoi(&work_str[work_str_pos + 1]);
						  else
							  pInfos[i].uEncounterMonster1AtMost = pInfos[i].uEncounterMonster1AtLeast;
						  }
					  break;
				  case 20:
					  pInfos[i].pEncounterMonster2Texture = RemoveQuotes(test_string);
					  break;
				  case 22:
					  pInfos[i].Dif_M2 = atoi(test_string);
					  break;
				  case 23:
					  pInfos[i].uEncounterMonster2AtLeast = 1;
					  pInfos[i].uEncounterMonster2AtMost = 1;
					  strcpy(work_str, test_string);
					  work_str_pos = 0;
					  work_str_len=strlen(work_str);
					  if (work_str_len )
						  {
						  while (work_str[work_str_pos] != '-' )
							  {
							  ++work_str_pos;
							  if (work_str_pos >= work_str_len )
								  break;
							  }
						  work_str[work_str_pos] = 0;
						  pInfos[i].uEncounterMonster2AtLeast = atoi(work_str);
						  if ( work_str_pos < work_str_len )
							  pInfos[i].uEncounterMonster2AtMost = atoi(&work_str[work_str_pos + 1]);
						  else
							  pInfos[i].uEncounterMonster2AtMost = pInfos[i].uEncounterMonster2AtLeast;
						  }
					  break;
				  case 24:
					  pInfos[i].pEncounterMonster3Texture = RemoveQuotes(test_string);
					  break;
				  case 26:
					  pInfos[i].Dif_M3 = atoi(test_string);
					  break;
				  case 27:
					  pInfos[i].uEncounterMonster3AtLeast = 1;
					  pInfos[i].uEncounterMonster3AtMost = 1;
					  strcpy(work_str, test_string);
					  work_str_pos = 0;
					  work_str_len=strlen(work_str);
					  if (work_str_len )
						  {
						  while (work_str[work_str_pos] != '-' )
							  {
							  ++work_str_pos;
							  if (work_str_pos >= work_str_len )
								  break;
							  }
						  work_str[work_str_pos] = 0;
						  pInfos[i].uEncounterMonster3AtLeast = atoi(work_str);
						  if ( work_str_pos < work_str_len )
							  pInfos[i].uEncounterMonster3AtMost = atoi(&work_str[work_str_pos + 1]);
						  else
							  pInfos[i].uEncounterMonster3AtMost = pInfos[i].uEncounterMonster3AtLeast;
						  }
					  break;
				  case 28:
					  pInfos[i].uRedbookTrackID = atoi(test_string);
					  break;
				  case 29:
					  {
					  if ( !strcmp(test_string, "CAVE") )
						  {
						  pInfos[i].uEAXEnv = 8;
						  break;
						  }
					  if ( !strcmp(test_string, "STONEROOM") )
						  {
						  pInfos[i].uEAXEnv = 5;
						  break;
						  }
					  if ( !strcmp(test_string, "MOUNTAINS") )
						  {
						  pInfos[i].uEAXEnv = 17;
						  break;
						  }
					  if ( !strcmp(test_string, "PLAIN") )
						  {
						  pInfos[i].uEAXEnv = 19;
						  break;
						  }
					  if ( !strcmp(test_string, "FOREST") )
						  {
						  pInfos[i].uEAXEnv = 15;
						  break;
						  }
					  if ( !strcmp(test_string, "CITY") )
						  {
						  pInfos[i].uEAXEnv = 16;
						  break;
						  }
					  if ( !strcmp(test_string, "UNDERWATER") )
						  {
						  pInfos[i].uEAXEnv = 22;
						  break;
						  }
					  if ( !strcmp(test_string, "ARENA") )
						  {
						  pInfos[i].uEAXEnv = 9;
						  break;
						  }
					  if ( !strcmp(test_string, "GENERIC") )
						  {
						  pInfos[i].uEAXEnv = 0;
						  break;
						  }
					  if ( !strcmp(test_string, "PADDEDCELL") )
						  {
						  pInfos[i].uEAXEnv = 1;
						  break;
						  }
					  if ( !strcmp(test_string, "ROOM") )
						  {
						  pInfos[i].uEAXEnv = 2;
						  break;
						  }
					  if ( !strcmp(test_string, "BATHROOM") )
						  {
						  pInfos[i].uEAXEnv = 3;
						  break;
						  }
					  if ( !strcmp(test_string, "LIVINGROOM") )
						  {
						  pInfos[i].uEAXEnv = 4;
						  break;
						  }
					  if ( !strcmp(test_string, "AUDITORIUM") )
						  {
						  pInfos[i].uEAXEnv = 6;
						  break;
						  }
					  if ( !strcmp(test_string, "CONCERTHALL") )
						  {
						  pInfos[i].uEAXEnv = 7;
						  break;
						  }
					  if ( !strcmp(test_string, "HANGAR") )
						  {
						  pInfos[i].uEAXEnv = 10;
						  break;
						  }
					  if ( !strcmp(test_string, "CARPETEDHALLWAY") )
						  {
						  pInfos[i].uEAXEnv = 11;
						  break;
						  }
					  if ( !strcmp(test_string, "HALLWAY") )
						  {
						  pInfos[i].uEAXEnv = 12;
						  break;
						  }
					  if ( !strcmp(test_string, "STONECORRIDOR") )
						  {
						  pInfos[i].uEAXEnv = 13;
						  break;
						  }
					  if ( !strcmp(test_string, "ALLEY") )
						  {
						  pInfos[i].uEAXEnv = 14;
						  break;
						  }
					  if ( !strcmp(test_string, "QUARRY") )
						  {
						  pInfos[i].uEAXEnv = 18;
						  break;
						  }
					  if ( !strcmp(test_string, "PARKINGLOT") )
						  {
						  pInfos[i].uEAXEnv = 20;
						  break;
						  }
					  if ( !strcmp(test_string, "SEWERPIPE") )
						  {
						  pInfos[i].uEAXEnv = 21;
						  break;
						  }
					  if ( !strcmp(test_string, "DRUGGED") )
						  {
						  pInfos[i].uEAXEnv = 23;
						  break;
						  }
					  if ( !strcmp(test_string, "DIZZY") )
						  {
						  pInfos[i].uEAXEnv = 24;
						  break;
						  }
					  if ( !strcmp(test_string, "PSYCHOTIC") )
						  {
						  pInfos[i].uEAXEnv = 25;
						  break;
						  }
					   pInfos[i].uEAXEnv = 26;
					
						  }
					  break;
				  }
			  }
		  else
			  { 
				  break_loop = true;
			  }
		  ++decode_step;
		  test_string=tmp_pos+1;
		  } while ((decode_step<31)&&!break_loop);
	  }
 
  uNumMaps = 77;
}
// 453F62: using guessed type char Str[32];

//----- (004547A3) --------------------------------------------------------
MAP_TYPE MapStats::GetMapInfo(const char *Str2)
{
  assert(uNumMaps >= 2);

  for (uint i = 1; i < uNumMaps; ++i)
    if (!_stricmp(pInfos[i].pFilename, Str2))
      return (MAP_TYPE)i;

  assert(false && "Map not found!");
}

//----- (004547E4) --------------------------------------------------------
void FactionTable::Initialize()
{
  int i;
  char* test_string;
  unsigned char c;
  bool break_loop;
  unsigned int temp_str_len;
  char* tmp_pos;
  int decode_step;
  int item_counter;

  if ( pHostileTXT_Raw )
    free(pHostileTXT_Raw);
  pHostileTXT_Raw = NULL;
  pHostileTXT_Raw = (char *)pEvents_LOD->LoadRaw("hostile.txt", 0);
  strtok(pHostileTXT_Raw, "\r");
  for (i=0; i<89; ++i)
	  {
	  test_string = strtok(NULL, "\r") + 1;
	  break_loop = false;
	  decode_step=0;
	  do 
		  {
		  c = *(unsigned char*)test_string;
		  temp_str_len = 0;
		  while((c!='\t')&&(c>0))
			  {
			  ++temp_str_len;
			  c=test_string[temp_str_len];
			  }		
		  tmp_pos=test_string+temp_str_len;
		  if (*tmp_pos == 0)
			  break_loop = true;
		  *tmp_pos = 0;
		  if (temp_str_len)
			  {
			  if ( decode_step >= 1 && decode_step < 90 )
				  relations[decode_step-1][i] = atoi(test_string);
			  }
		  else
			  { 
			  break_loop = true;
			  }
		  ++decode_step;
		  test_string=tmp_pos+1;
		  } while ((decode_step<92)&&!break_loop);
	  }
  if ( pHostileTXT_Raw)
  {
    free(pHostileTXT_Raw);
    pHostileTXT_Raw = NULL;
  }
}

//----- (00458244) --------------------------------------------------------
unsigned int SkillToMastery( unsigned int skill_value )
{
  switch (skill_value & 0x1C0)
  {
    case 0x100: return 4;     // Grandmaster
    case 0x80:  return 3;     // Master
    case 0x40:  return 2;     // Expert
    case 0x00:  return 1;     // Normal
  }
  assert(false);
  return 0;
}

//----- (0045828B) --------------------------------------------------------
unsigned int __fastcall GetSpellColor(signed int a1)
{
  if ( a1 == 0 )
    return TargetColor(0, 0, 0);
  if ( a1 < 12 )
    return TargetColor(255, 85, 0);
  if ( a1 < 23 )
    return TargetColor(150, 212, 255);
  if ( a1 < 34 )
    return TargetColor(0, 128, 255);
  if ( a1 < 45 )
    return TargetColor(128, 128, 128);
  if ( a1 < 56 )
    return TargetColor(225, 225, 225);
  if ( a1 < 67 )
    return TargetColor(235, 15, 255);
  if ( a1 < 78 )
    return TargetColor(255, 128, 0);
  if ( a1 < 89 )
    return TargetColor(255, 255, 155);
  if ( a1 < 100 )
    return TargetColor(192, 192, 240);
}

//----- (004610AA) --------------------------------------------------------
void __fastcall PrepareToLoadODM(unsigned int bLoading, ODMRenderParams *a2)
{
  unsigned int v2; // edi@1
  ODMRenderParams *v3; // esi@1

  v2 = bLoading;
  v3 = a2;
  pGameLoadingUI_ProgressBar->Reset(27);
  pSoundList->_4A9D79(0);
  uCurrentlyLoadedLevelType = LEVEL_Outdoor;
  ODM_LoadAndInitialize(pCurrentMapName, v3);
  if ( !v2 )
    TeleportToStartingPoint(uLevel_StartingPointType);
  viewparams->_443365();
  PlayLevelMusic();
}
// 6BE35C: using guessed type int uLevel_StartingPointType;

//----- (00461103) --------------------------------------------------------
void _461103_load_level_sub()
{
  //GUIProgressBar *v0; // ebx@1
  //signed int v1; // ebp@1
  //char *v2; // esi@2
  __int16 v3; // cx@3
  int v4; // edx@8
  //size_t v5; // edi@14
  signed int v6; // esi@14
  //char *v7; // edx@15
  signed int v8; // ecx@16
  int v9; // ecx@23
  MonsterInfo *v10; // ebx@23
  //int *v11; // esi@23
  int v12; // esi@25
  int v13; // eax@26
  __int16 v14; // ax@41
  //signed int v15; // [sp+10h] [bp-4Ch]@1
  //signed int v16; // [sp+10h] [bp-4Ch]@14
  signed int v17; // [sp+14h] [bp-48h]@3
  signed int v18; // [sp+14h] [bp-48h]@23
  int v19; // [sp+18h] [bp-44h]@1
  signed int v20; // [sp+18h] [bp-44h]@14
  int v21[16]; // [sp+1Ch] [bp-40h]@17

  GenerateItemsInChest();
  //v0 = pGameLoadingUI_ProgressBar;
  pGameLoadingUI_ProgressBar->Progress();
  pParty->uFlags |= 2u;
  pParty->field_7B5_in_arena_quest = 0;
  //v1 = 0;
  dword_5C6DF8 = 1;
  pNPCStats->uNewlNPCBufPos = 0;
  v19 = pMapStats->GetMapInfo(pCurrentMapName);

  //v15 = 0;
  for (uint i = 0; i < uNumActors; ++i)
  //if ( (signed int)uNumActors > 0 )
  {
    auto pActor = &pActors[i];
    //v2 = (char *)&pActors[0].uNPC_ID;
    //do
    //{
      v3 = pActor->pMonsterInfo.uID;
      v17 = 0;
      if ( pActor->pMonsterInfo.uID >= 115 && pActor->pMonsterInfo.uID <= 186
        || pActor->pMonsterInfo.uID >= 232 && pActor->pMonsterInfo.uID <= 249 )
        v17 = 1;
      //v1 = 0;
      v4 = (v3 - 1) % 3;
      if ( 2 == v4 )
      {
        if ( pActor->sNPC_ID && pActor->sNPC_ID < 5000 )
          continue;
      }
      else
      {
        if ( v4 != 1 )
        {
          if ( v4 == 0 && pActor->sNPC_ID == 0 )
			pActor->sNPC_ID = 0;
		  continue;
        }
      }
      if ( pActor->sNPC_ID > 0 && pActor->sNPC_ID < 5000 )
        continue;
      if ( v17 )
      {
        pNPCStats->InitializeAdditionalNPCs(&pNPCStats->pAdditionalNPC[pNPCStats->uNewlNPCBufPos], v3, 0, v19);
        v14 = LOWORD(pNPCStats->uNewlNPCBufPos) + 5000;
        ++pNPCStats->uNewlNPCBufPos;
        pActor->sNPC_ID = v14;
        continue;
      }
      pActor->sNPC_ID = 0;
      //++v15;
      //v2 += 836;
    //}
    //while ( v15 < (signed int)uNumActors );
  }

  pGameLoadingUI_ProgressBar->Progress();

  //v5 = uNumActors;
  v6 = 0;
  v20 = 0;
  //v16 = v1;

  for (uint i = 0; i < uNumActors; ++i)
  {
    auto pActor = &pActors[i];
    //v7 = (char *)&pActors[0].pMonsterInfo;
    //do
    //{
      v8 = 0;
      if ( v6 > 0 )
      {
        do
        {
          if ( v21[v8] == pActor->pMonsterInfo.uID - 1 )
            break;
          ++v8;
        }
        while ( v8 < v6 );
      }

      if ( v8 == v6 )
      {
        v21[v6++] = pActor->pMonsterInfo.uID - 1;
        v20 = v6;
        if ( v6 == 16 )
          break;
      }
      //++v16;
      //v7 += 836;
    //}
    //while ( v16 < (signed int)v5 );
  }

  pGameLoadingUI_ProgressBar->Progress();

  if ( v6 > 0 )
  {
    int _v0 = 0;
    do
    {
      v9 = v21[_v0];
      v18 = 4;
      v10 = &pMonsterStats->pInfos[v9 + 1];
      //v11 = (int *)pMonsterList->pMonsters[v9].pSoundSampleIDs;
      auto v11 = &pMonsterList->pMonsters[v9];
      do
      {
        pSoundList->LoadSound(v11->pSoundSampleIDs[4 - v18], 0);
        //v11 = (int *)((char *)v11 + 2);
        --v18;
      }
      while ( v18 );
      v12 = 0;
      do
        v13 = pSoundList->LoadSound(v12++ + word_4EE088_sound_ids[v10->uSpell1ID], 1);
      while ( v13 );
      ++_v0;
    }
    while ( _v0 < v6 );
    //v0 = pGameLoadingUI_ProgressBar;
    //v1 = 0;
  }

  pGameLoadingUI_ProgressBar->Progress();

  if ( dword_6BE368_debug_settings_2 & 4 )
    uNumActors = 0;
  if ( dword_6BE368_debug_settings_2 & 8 )
    uNumLevelDecorations = 0;
  init_event_triggers();

  pGameLoadingUI_ProgressBar->Progress();
  
  pGame->pIndoorCameraD3D->vPartyPos.x = 0;
  pGame->pIndoorCameraD3D->vPartyPos.y = 0;
  pGame->pIndoorCameraD3D->vPartyPos.z = 100;
  pGame->pIndoorCameraD3D->sRotationX = 0;
  pGame->pIndoorCameraD3D->sRotationY = 0;
  viewparams->bRedrawGameUI = true;
  uLevel_StartingPointType = MapStartPoint_Party;
  pSprites_LOD->_461397();
  pPaletteManager->LockTestAll();
  if ( pParty->pPickedItem.uItemID != 0 )
    pMouse->SetCursorBitmapFromItemID(pParty->pPickedItem.uItemID);
}

//----- (004613C4) --------------------------------------------------------
void init_event_triggers()
{
  uint id = pDecorationList->GetDecorIdByName("Event Trigger");
  
  num_event_triggers = 0;
  for (uint i = 0; i < uNumLevelDecorations; ++i)
    if (pLevelDecorations[i].uDecorationDescID == id)
      event_triggers[num_event_triggers++] = i;
}

//----- (004627B7) --------------------------------------------------------
void MainMenu_Loop()
{
  GUIButton *pButton; // eax@27
  unsigned int pControlParam; // ecx@35
  int v10; // ecx@36
  int v11; // ecx@37
  unsigned int pY; // [sp-18h] [bp-54h]@39
  Texture *pTexture; // [sp-14h] [bp-50h]@39
  GUIButton *pButton2; // [sp+0h] [bp-3Ch]@27
  GUIWindow *pWindow; // [sp+4h] [bp-38h]@11
  
  pCurrentScreen = SCREEN_GAME;

  pGUIWindow2 = 0;
  pAudioPlayer->StopChannels(-1, -1);
  pMouse->RemoveHoldingItem();

  pIcons_LOD->_inlined_sub2();

  pWindow_MainMenu = GUIWindow::Create(0, 0, 640, 480, WINDOW_MainMenu, 0, 0);
  auto pNew = pIcons_LOD->LoadTexturePtr("title_new", TEXTURE_16BIT_PALETTE);
  auto pLoad = pIcons_LOD->LoadTexturePtr("title_load", TEXTURE_16BIT_PALETTE);
  auto pCredits = pIcons_LOD->LoadTexturePtr("title_cred", TEXTURE_16BIT_PALETTE);
  auto pExit = pIcons_LOD->LoadTexturePtr("title_exit", TEXTURE_16BIT_PALETTE);

  pMainMenu_BtnNew     = pWindow_MainMenu->CreateButton(495, 172, pNew->uTextureWidth,     pNew->uTextureHeight,     1, 0, UIMSG_MainMenu_ShowPartyCreationWnd, 0, 'N', "", pNew, 0);
  pMainMenu_BtnLoad    = pWindow_MainMenu->CreateButton(495, 227, pLoad->uTextureWidth,    pLoad->uTextureHeight,    1, 0, UIMSG_MainMenu_ShowLoadWindow,       1, 'L', "", pLoad, 0);
  pMainMenu_BtnCredits = pWindow_MainMenu->CreateButton(495, 282, pCredits->uTextureWidth, pCredits->uTextureHeight, 1, 0, UIMSG_ShowCredits,                   2, 'C', "", pCredits, 0);
  pMainMenu_BtnExit    = pWindow_MainMenu->CreateButton(495, 337, pExit->uTextureWidth,    pExit->uTextureHeight,    1, 0, UIMSG_ExitToWindows,                 3, 0, "", pExit, 0);

  pTexture_PCX.Release();
  pTexture_PCX.Load("title.pcx", 0);
  SetCurrentMenuID(MENU_MAIN);
  //SetForegroundWindow(hWnd);
  //SendMessageW(hWnd, WM_ACTIVATEAPP, 1, 0);
  while (GetCurrentMenuID() == MENU_MAIN || GetCurrentMenuID() == MENU_SAVELOAD)
  {
    POINT pt;
    pMouse->GetCursorPos(&pt);
    pWindow = pWindow_MainMenu;
    if ( GetCurrentMenuID() == MENU_SAVELOAD)
    {
      if ( pCurrentScreen != SCREEN_LOADGAME )
      {
        pTexture_PCX.Release();
        pTexture_PCX.Load("lsave640.pcx", 0);
        pGUIWindow2 = GUIWindow::Create(0, 0, 640, 480, WINDOW_MainMenu, 0, 0);
        pCurrentScreen = SCREEN_LOADGAME;
        LoadUI_Load(0);
      }
      pWindow = pGUIWindow_CurrentMenu;
    }

    for (MSG msg; PeekMessage(&msg, 0, 0, 0, PM_REMOVE);)
    {
      if (msg.message == WM_QUIT)
        Game_DeinitializeAndTerminate(0);
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    if (dword_6BE364_game_settings_1 & 0x0100)
    {
      WaitMessage();
    }
    else
    {
      pRenderer->BeginScene();
      pRenderer->DrawTextureRGB(0, 0, &pTexture_PCX);

      GUI_MainMenuMessageProc();
      GUI_UpdateWindows();

      if (GetCurrentMenuID() != MENU_MAIN)
      {
        if (GetCurrentMenuID() == MENU_LoadingProcInMainMenu)
        {
          pIcons_LOD->RemoveTexturesPackFromTextureList();
          pGUIWindow_CurrentMenu->Release();
          pGUIWindow_CurrentMenu = 0;
          pCurrentScreen = SCREEN_GAME;
          viewparams->bRedrawGameUI = 1;
        }
      }
      else
      {
        if ( !pModalWindow )// ???
        {
          pButton = pWindow->pControlsHead;
          for ( pButton = pWindow->pControlsHead; pButton; pButton = pButton->pNext )
          {
           if ( pt.x >= (signed int)pButton->uX && pt.x <= (signed int)pButton->uZ
                && pt.y >= (signed int)pButton->uY && pt.y <= (signed int)pButton->uW
                && pWindow == pWindow_MainMenu )
           {
            pControlParam = pButton->msg_param;
            switch (pControlParam) // ïîäñâåòêà êíîïîê
            {
              case 0:
              pTexture = pNew;
              pY = 172;
              break;
              case 1:
              pTexture = pLoad;
              pY = 227;
              break;
              case 2:
              pTexture = pCredits;
              pY = 282;
              break;
              case 3:
              pTexture = pExit;
              pY = 337;
              break;
            }
            pRenderer->DrawTextureIndexed(495, pY, pTexture); //ïîäñâåòêà êíîïîê
           }
          }
        }
      }
      pRenderer->EndScene();
      pRenderer->Present();
    }
  }
  GUI_MainMenuMessageProc();
  pRenderer->BeginScene();
  GUI_UpdateWindows();
  pRenderer->EndScene();
  pRenderer->Present();
  pTexture_PCX.Release();
  if ( pGUIWindow2 )
  {
    pGUIWindow2->Release();
    pGUIWindow2 = 0;
  }
  pWindow_MainMenu->Release();
  pIcons_LOD->RemoveTexturesPackFromTextureList();
}


//----- (00464479) --------------------------------------------------------
void ResetCursor_Palettes_LODs_Level_Audio_SFT_Windows()
{
  int v0; // esi@9

  if (pMouse)
    pMouse->SetCursorBitmap("MICON1");

  pPaletteManager->ResetNonLocked();
  pBitmaps_LOD->ReleaseAll2();
  pSprites_LOD->DeleteSomeOtherSprites();
  pIcons_LOD->ReleaseAll2();
  if (pRenderer->pRenderD3D)
  {
    pRenderer->field_1036B8 = 0;
    pRenderer->_gpu_memory_used = 0;
  }
  sub_46080D();
  if (uCurrentlyLoadedLevelType == LEVEL_Indoor)
    pIndoor->Release();
  else if (uCurrentlyLoadedLevelType == LEVEL_Outdoor)
    pOutdoor->Release();

  pAudioPlayer->StopChannels(-1, -1);
  pSoundList->_4A9D79(0);
  uCurrentlyLoadedLevelType = LEVEL_null;
  pSpriteFrameTable->ResetSomeSpriteFlags();
  v0 = uNumVisibleWindows;
  pParty->armageddon_timer = 0;
  while ( v0 > 0 )
    pWindowList[pVisibleWindowsIdxs[v0--] - 1].Release();
}

//----- (004646F0) --------------------------------------------------------
void PrepareWorld(unsigned int _0_box_loading_1_fullscreen)
{
  if ( pRenderer->pRenderD3D )
    pGame->pVisInstance->_4C1A02();
  pEventTimer->Pause();
  pMiscTimer->Pause();
  pParty->uFlags = 2;
  CastSpellInfoHelpers::_427D48();
  ResetCursor_Palettes_LODs_Level_Audio_SFT_Windows();
  DoPrepareWorld(0, (_0_box_loading_1_fullscreen == 0) + 1);
  pMiscTimer->Resume();
  pEventTimer->Resume();
}

//----- (00464761) --------------------------------------------------------
void Game_DeinitializeAndTerminate(int exitCode)
{
  SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
  ResetCursor_Palettes_LODs_Level_Audio_SFT_Windows();
  pGame->Deinitialize();
  pRenderer->Release2();
  delete window;
  //if ( !DestroyWindow(hWnd) )
  //  GetLastError();
  exit(exitCode);
}

//----- (004647AB) --------------------------------------------------------
void FinalInitialization()
{
  pViewport->SetScreen(viewparams->uSomeX, viewparams->uSomeY, viewparams->uSomeZ, viewparams->uSomeW);
  pViewport->_4C02F8((signed __int64)(flt_6BE3A0 * 65536.0));

  //pIndoorCamera = new IndoorCamera;
  //pIndoorCamera->Initialize(65, viewparams->uScreen_BttmR_X - viewparams->uScreen_topL_X + 1,
  //                              viewparams->uScreen_BttmR_Y - viewparams->uScreen_topL_Y + 1);

  InitializeTurnBasedAnimations(&stru_50C198);
  pBitmaps_LOD->_inlined_sub1();
  pSprites_LOD->_inlined_sub1();
  pIcons_LOD->_inlined_sub1();
}
// 6BE3A0: using guessed type float flt_6BE3A0;

//----- (00464839) --------------------------------------------------------
char Is_out15odm_underwater()
{
  return _stricmp(pCurrentMapName, "out15.odm") == 0;
}

//----- (00464851) --------------------------------------------------------
void SetUnderwaterFog()
{
  day_fogrange_1 = 50;
  day_fogrange_2 = 5000;
}

//----- (00464866) --------------------------------------------------------
void DoPrepareWorld(unsigned int bLoading, int _1_fullscreen_loading_2_box)
{
  //int v2; // esi@1
  char *v3; // eax@1
  //char *v4; // eax@1
  unsigned int v5; // eax@3
  //size_t v6; // ecx@10
  //char *v7; // eax@11
  char Str1[20]; // [sp+Ch] [bp-18h]@1
  unsigned int v9; // [sp+20h] [bp-4h]@1

  v9 = bLoading;
  ResetCursor_Palettes_LODs_Level_Audio_SFT_Windows();
  pDecalBuilder->Reset(0);
  pGameLoadingUI_ProgressBar->Initialize(_1_fullscreen_loading_2_box == 1 ? GUIProgressBar::TYPE_Fullscreen :
                                                                            GUIProgressBar::TYPE_Box);
  strcpy(Str1, pCurrentMapName);
  v3 = strtok(Str1, ".");
  strcpy(Str1, v3);
  Level_LoadEvtAndStr(Str1);
  LoadLevel_InitializeLevelEvt();
  strcpy(Str1, pCurrentMapName);
  _strrev(Str1);
  strtok(Str1, ".");
  _strrev(Str1);

  for (uint i = 0; i < 1000; ++i)
    pSpriteObjects[i].uObjectDescID = 0;

  v5 = pMapStats->GetMapInfo(pCurrentMapName);
  bUnderwater = false;
  uLevelMapStatsID = v5;
  pGame->uFlags2 &= 0xFFFFFFF7u;
  if ( !_stricmp(pCurrentMapName, "out15.odm") )
  {
    bUnderwater = true;
    pGame->uFlags2 |= GAME_FLAGS_2_ALTER_GRAVITY;
  }
  pParty->floor_face_pid = 0;
  if ( _stricmp(Str1, "blv") )
    PrepareToLoadODM(v9, 0);
  else
    PrepareToLoadBLV(v9);
  pAudioPlayer->SetMapEAX();
  _461103_load_level_sub();
  if ( !_stricmp(pCurrentMapName, "d11.blv") || !_stricmp(pCurrentMapName, "d10.blv") )
  {
    //spawning grounds & walls of mist - no loot & exp from monsters

    for (uint i = 0; i < uNumActors; ++i)
    {
      pActors[i].pMonsterInfo.uTreasureType = 0;
      pActors[i].pMonsterInfo.uTreasureDiceRolls = 0;
      pActors[i].pMonsterInfo.uExp = 0;
    }
  }
  bDialogueUI_InitializeActor_NPC_ID = 0;
  OnMapLoad();
  pGameLoadingUI_ProgressBar->Progress();
  memset(&pRenderer->pBillboardRenderListD3D, 0, sizeof(pRenderer->pBillboardRenderListD3D));
  pGameLoadingUI_ProgressBar->Release();
  _flushall();
}

//----- (004649EF) --------------------------------------------------------
int __fastcall ReadWindowsRegistryInt(const char *pKey, int uDefValue)
{
  int v3; // [sp+4h] [bp-24h]@1
  DWORD cbData; // [sp+8h] [bp-20h]@1
  LPCSTR lpValueName; // [sp+Ch] [bp-1Ch]@1
  DWORD dwDisposition; // [sp+10h] [bp-18h]@2
  BYTE Data[4]; // [sp+14h] [bp-14h]@5
  HKEY hKey; // [sp+18h] [bp-10h]@1
  HKEY phkResult; // [sp+1Ch] [bp-Ch]@1
  HKEY v10; // [sp+20h] [bp-8h]@1
  HKEY v11; // [sp+24h] [bp-4h]@1

  v3 = uDefValue;
  lpValueName = pKey;
  v11 = 0;
  v10 = 0;
  hKey = 0;
  phkResult = 0;
  cbData = 4;
  if ( !RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE", 0, KEY_READ|KEY_WOW64_32KEY, &hKey) )// for 64 bit
  {
    if ( !RegCreateKeyExA(hKey, "New World Computing", 0, "", 0, 0xF003Fu, 0, &phkResult, &dwDisposition) )
    {
      if ( !RegCreateKeyExA(phkResult, "Might and Magic VII", 0, "", 0, 0xF003Fu, 0, &v10, &dwDisposition) )
      {
        if ( !RegCreateKeyExA(v10, "1.0", 0, "", 0, 0xF003Fu, 0, &v11, &dwDisposition) )
        {
          if ( RegQueryValueExA(v11, lpValueName, 0, 0, Data, &cbData) )
          {
            *(int *)Data = v3;
            RegSetValueExA(v11, lpValueName, 0, 4u, Data, 4u);
          }
          RegCloseKey(v11);
        }
        RegCloseKey(v10);
      }
      RegCloseKey(phkResult);
    }
    RegCloseKey(hKey);
  }
  return *(int *)Data;
}

//----- (00464B02) --------------------------------------------------------
void __fastcall WriteWindowsRegistryString(const char *pKey, const char *pString)
{
  size_t v2; // eax@5
  const char *lpValueName; // [sp+4h] [bp-1Ch]@1
  const char *Str; // [sp+8h] [bp-18h]@1
  DWORD dwDisposition; // [sp+Ch] [bp-14h]@2
  HKEY hKey; // [sp+10h] [bp-10h]@1
  HKEY phkResult; // [sp+14h] [bp-Ch]@1
  HKEY v8; // [sp+18h] [bp-8h]@1
  HKEY v9; // [sp+1Ch] [bp-4h]@1

  Str = pString;
  lpValueName = pKey;
  v9 = 0;
  v8 = 0;
  hKey = 0;
  phkResult = 0;
  if ( !RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE", 0, 0x2001Fu, &hKey) )
  {
    if ( !RegCreateKeyExA(hKey, "New World Computing", 0, "", 0, 0xF003Fu, 0, &phkResult, &dwDisposition) )
    {
      if ( !RegCreateKeyExA(phkResult, "Might and Magic VII", 0, "", 0, 0xF003Fu, 0, &v8, &dwDisposition) )
      {
        if ( !RegCreateKeyExA(v8, "1.0", 0, "", 0, 0xF003Fu, 0, &v9, &dwDisposition) )
        {
          v2 = strlen(Str);
          RegSetValueExA(v9, lpValueName, 0, 1u, (const BYTE *)Str, v2 + 1);
          RegCloseKey(v9);
        }
        RegCloseKey(v8);
      }
      RegCloseKey(phkResult);
    }
    RegCloseKey(hKey);
  }
}

//----- (00464BEF) --------------------------------------------------------
void __fastcall ReadWindowsRegistryString(const char *pKeyName, char *pOutString, int uBufLen, const char *pDefaultValue)
{
  LSTATUS (__stdcall *v4)(HKEY); // esi@1
  LSTATUS result; // eax@7
  DWORD Type; // [sp+Ch] [bp-24h]@5
  LPCSTR lpValueName; // [sp+10h] [bp-20h]@1
  DWORD cbData; // [sp+14h] [bp-1Ch]@1
  LPBYTE Dest; // [sp+18h] [bp-18h]@1
  DWORD dwDisposition; // [sp+1Ch] [bp-14h]@2
  HKEY phkResult; // [sp+20h] [bp-10h]@1
  HKEY hKey; // [sp+24h] [bp-Ch]@1
  HKEY v13; // [sp+28h] [bp-8h]@1
  HKEY v14; // [sp+2Ch] [bp-4h]@1

  cbData = uBufLen;
  Dest = (LPBYTE)pOutString;
  lpValueName = pKeyName;
  v14 = 0;
  v13 = 0;
  hKey = 0;
  phkResult = 0;
  v4 = RegCloseKey;
  if ( RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE", 0, 0x2001Fu, &hKey)
    || RegCreateKeyExA(hKey, "New World Computing", 0, "", 0, 0xF003Fu, 0, &phkResult, &dwDisposition)
    || RegCreateKeyExA(phkResult, "Might and Magic VII", 0, "", 0, 0xF003Fu, 0, &v13, &dwDisposition)
    || RegCreateKeyExA(v13, "1.0", 0, "", 0, 0xF003Fu, 0, &v14, &dwDisposition) )
  {
    result = (LSTATUS)strncpy((char *)Dest, pDefaultValue, uBufLen);
  }
  else if ( RegQueryValueExA(v14, lpValueName, 0, &Type, Dest, &cbData) )
  {
    RegCloseKey(v14);
    v14 = 0;
    result = (LSTATUS)strncpy((char *)Dest, pDefaultValue, uBufLen);
  }
  else
  {
	v4 = RegCloseKey;
	RegCloseKey(v14);
	v14 = 0;
	RegCloseKey(v13);
	v13 = 0;
	RegCloseKey(phkResult);
	phkResult = 0;
	result = RegCloseKey(hKey);
	hKey = 0;
  }
  if ( v14 )
    result = v4(v14);
  if ( v13 )
    result = v4(v13);
  if ( hKey )
    result = v4(hKey);
  if ( phkResult )
    result = v4(phkResult);
}

//----- (00464D32) --------------------------------------------------------
void __fastcall WriteWindowsRegistryInt(const char *pKey, int val)
{
  const char *lpValueName; // [sp+4h] [bp-1Ch]@1
  BYTE Data[4]; // [sp+8h] [bp-18h]@1
  DWORD dwDisposition; // [sp+Ch] [bp-14h]@2
  HKEY hKey; // [sp+10h] [bp-10h]@1
  HKEY phkResult; // [sp+14h] [bp-Ch]@1
  HKEY v7; // [sp+18h] [bp-8h]@1
  HKEY v8; // [sp+1Ch] [bp-4h]@1

  *(int *)Data = val;
  lpValueName = pKey;
  v8 = 0;
  v7 = 0;
  hKey = 0;
  phkResult = 0;
  if ( !RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE", 0, 0x2001Fu, &hKey) )
  {
    if ( !RegCreateKeyExA(hKey, "New World Computing", 0, "", 0, 0xF003Fu, 0, &phkResult, &dwDisposition) )
    {
      if ( !RegCreateKeyExA(phkResult, "Might and Magic VII", 0, "", 0, 0xF003Fu, 0, &v7, &dwDisposition) )
      {
        if ( !RegCreateKeyExA(v7, "1.0", 0, "", 0, 0xF003Fu, 0, &v8, &dwDisposition) )
        {
          RegSetValueExA(v8, lpValueName, 0, 4u, Data, 4u);
          RegCloseKey(v8);
        }
        RegCloseKey(v7);
      }
      RegCloseKey(phkResult);
    }
    RegCloseKey(hKey);
  }
}

//----- (00464E17) --------------------------------------------------------
bool __fastcall CheckMM7CD(char c)
{
  char DstBuf[256]; // [sp+Ch] [bp-218h]@1
  char strCommand[256]; // [sp+10Ch] [bp-118h]@1
  char Filename[20]; // [sp+20Ch] [bp-18h]@1


  wchar_t pMagicPath[1024];
  swprintf(pMagicPath, wcslen(L"%C:\\anims\\magic7.vid"), L"%C:\\anims\\magic7.vid", c);
  if (GetFileAttributesW(pMagicPath) == -1)
    return false;

  wsprintfA(strCommand, "open %c: type cdaudio alias CD", c);
  if (!mciSendStringA(strCommand, DstBuf, 0xFFu, 0))
  {
    wsprintfA(strCommand, "info CD UPC wait");
    mciSendStringA(strCommand, DstBuf, 0xFFu, 0);
    wsprintfA(strCommand, "close CD");
    mciSendStringA(strCommand, DstBuf, 0xFFu, 0);
  }

  memcpy(Filename, "X:\\anims\\magic7.vid", sizeof(Filename));
  *Filename = c;

  auto f = fopen(Filename, "rb");
  if (!f)
    return false;

  if (!fseek(f, 0, SEEK_END))
  {
    if (!fseek(f, -100, SEEK_CUR))
      fread(DstBuf, 1u, 0x64u, f);

    fclose(f);
    return true;
  }
  fclose(f);
  return false;
}

//----- (00464F1B) --------------------------------------------------------
signed int __stdcall InsertMM7CDDialogFunc(HWND hDlg, int a2, __int16 a3, int a4)
{
  char v4; // zf@3
  int v6; // eax@10
  int v7; // eax@11
  int v8; // eax@12
  int v9; // eax@13
  BOOL (__stdcall *v10)(HWND, int, LPCSTR); // edi@15
  const CHAR *v11; // [sp-Ch] [bp-Ch]@15
  INT_PTR v12; // [sp-4h] [bp-4h]@5

  if ( a2 == 272 )
  {
    hInsertCDWindow = hDlg;
    v6 = (GetUserDefaultLangID() & 0x3FF) - 7;
    if ( v6 )
    {
      v7 = v6 - 3;
      if ( v7 )
      {
        v8 = v7 - 2;
        if ( v8 )
        {
          v9 = v8 - 4;
          if ( v9 )
          {
            if ( v9 != 5 )
              return 0;
            SetWindowTextA(hDlg, "Wloz CD-ROM numer 2");
            v10 = SetDlgItemTextA;
            SetDlgItemTextA(hDlg, 1010, "Wloz CD-ROM numer 2 Might and Magic® VII.");
            v11 = "Odwolaj";
          }
          else
          {
            SetWindowTextA(hDlg, "Inserire il secondo CD");
            v10 = SetDlgItemTextA;
            SetDlgItemTextA(hDlg, 1010, "Inserire il secondo CD di Might and Magic® VII.");
            v11 = "Annulla";
          }
        }
        else
        {
          SetWindowTextA(hDlg, "Insérez le CD 2");
          v10 = SetDlgItemTextA;
          SetDlgItemTextA(hDlg, 1010, "Insérez Might & Magic® VII CD 2.");
          v11 = "Supprimer";
        }
      }
      else
      {
        SetWindowTextA(hDlg, "Por favor, inserte disco 2");
        v10 = SetDlgItemTextA;
        SetDlgItemTextA(hDlg, 1010, "Por favor, inserte disco 2 de Might & Magic® VII.");
        v11 = "Cancelar";
      }
    }
    else
    {
      SetWindowTextA(hDlg, "Bitte CD 2 einlegen");
      v10 = SetDlgItemTextA;
      SetDlgItemTextA(hDlg, 1010, "Bitte CD 2 von Might and Magic® VII einlegen.");
      v11 = "Abbrechen";
    }
    v10(hDlg, 2, v11);
    return 0;
  }
  if ( a2 == 273 )
  {
    if ( a3 == 2 )
    {
      v12 = 0;
      EndDialog(hDlg, v12);
      return 1;
    }
    v4 = a3 == 1;
  }
  else
  {
    v4 = a2 == 1025;
  }
  if ( v4 )
  {
    v12 = 1;
    EndDialog(hDlg, v12);
    return 1;
  }
  return 0;
}

//----- (00465061) --------------------------------------------------------
bool __fastcall FindMM7CD(HWND hWnd, char *pCDDrive)
{
  char drive[4] = {'X', ':', '\\', 0};

  bool bGotCDFromRegistry = false;

  HKEY hSoftware = nullptr,
       hNWC = nullptr,
       hMM7 = nullptr,
       hVersion = nullptr;
  if (!RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE", 0, 0x2001Fu, &hSoftware))
  {
    if (!RegOpenKeyExA(hSoftware, "New World Computing", 0, 0x2001Fu, &hNWC))
    {
      if (!RegOpenKeyExA(hNWC, "Might and Magic VII", 0, 0x2001Fu, &hMM7))
      {
        if (!RegOpenKeyExA(hMM7, "1.0", 0, 0x2001Fu, &hVersion))
        {
          DWORD cbData = 3;
          if (!RegQueryValueExA(hVersion, "CDDrive", 0, 0, (BYTE *)drive, &cbData))
            bGotCDFromRegistry = true;
        }
        RegCloseKey(hVersion);
      }
      RegCloseKey(hMM7);
    }
    RegCloseKey(hNWC);
  }
  RegCloseKey(hSoftware);

  if (bGotCDFromRegistry)
    if (CheckMM7CD(*drive))
    {
      cMM7GameCDDriveLetter = *drive;
      return true;
    }

  while (true)
  {
    for (uint i = 0; i < 26; ++i)
    {
      drive[0] = 'A' + i;

      if (GetDriveTypeA(drive) == DRIVE_CDROM)
        if (CheckMM7CD(*drive))
        {
          cMM7GameCDDriveLetter = *drive;
          WriteWindowsRegistryString("CDDrive", drive);
          return true;
        }
    }

    if (DialogBoxParamA(GetModuleHandleW(nullptr), "InsertCD", hWnd, (DLGPROC)InsertMM7CDDialogFunc, 0))
      continue;
    return false;
  }
}



//----- (004651F4) --------------------------------------------------------
bool MM7_Initialize(int game_width, int game_height)
{
  wchar_t pCurrentDir[1024];
  _wgetcwd(pCurrentDir, 1024);

  wchar_t pMM6IniFile[1024];
  wsprintfW(pMM6IniFile, L"%s\\mm6.ini", pCurrentDir);

  bCanLoadFromCD = GetPrivateProfileIntW(L"settings", L"use_cd", 1, pMM6IniFile);
  if (bNoCD)
    bCanLoadFromCD = false;
  if (bCanLoadFromCD)
  {
    if (!FindMM7CD(nullptr, &cMM7GameCDDriveLetter))
      return false;
  }


  srand(GetTickCount());

  window = OSWindow::Create(L"Might and Magic® Trilogy", game_width, game_height);

  pRenderer = Render::Create();
  if (!pRenderer)
  {
    Log::Warning(L"Render creation failed");
    return false;
  }
  else
  {
    bool bWindowMode = ReadWindowsRegistryInt("startinwindow", false);
    //uint uDefaultDevice = ReadWindowsRegistryInt("D3D Device", 1);
    bool bColoredLights = ReadWindowsRegistryInt("Colored Lights", false);
    uint uLevelOfDetail = ReadWindowsRegistryInt("Detail Level", 1);
    uint bTinting = ReadWindowsRegistryInt("Tinting", 1) != 0;

    if (!pRenderer->Initialize(bWindowMode, window, bColoredLights, uLevelOfDetail, bTinting))
    {
      Log::Warning(L"Render failed to initialize");
      return false;
    }
  }

  game_starting_year = 1168;

  pParty = new Party;
  memset(&pParty->pHirelings, 0, sizeof(pParty->pHirelings));
  pParty->uWalkSpeed = GetPrivateProfileIntW(L"debug", L"walkspeed", 384, pMM6IniFile);
  pParty->uDefaultEyelevel = GetPrivateProfileIntW(L"party", L"eyelevel", 160, pMM6IniFile);
  pParty->sEyelevel = pParty->uDefaultEyelevel;
  pParty->uDefaultPartyHeight = GetPrivateProfileIntW(L"party", L"height", 192, pMM6IniFile);
  pParty->uPartyHeight = pParty->uDefaultPartyHeight;
  
  bUseRegistry = GetPrivateProfileIntW(L"settings", L"registry", 1, pMM6IniFile);
  MM6_Initialize(pMM6IniFile);

  pKeyActionMap = new KeyboardActionMapping;

  pEventTimer = Timer::Create();
  pEventTimer->Initialize();

  OnTimer(1);
  GameUI_StatusBar_UpdateTimedString(1);
  pGame = Game::Create();
  pMouse = pGame->pMouseInstance;


  pIcons_LOD = new LODFile_IconsBitmaps;
  if (!pIcons_LOD->Load("data\\icons.lod", "icons"))
  {
    MessageBoxW(nullptr,
                L"Some files are missing\n\nPlease Reinstall.",
                L"Files Missing", MB_ICONEXCLAMATION);
    return false;
  }
  pIcons_LOD->dword_011BA4 = 0;
  
  pEvents_LOD = new LODFile_IconsBitmaps;
  if (!pEvents_LOD->Load("data\\events.lod", "icons"))
  {
    MessageBoxW(nullptr,
                L"Some files are missing\n\nPlease Reinstall.",
                L"Files Missing", MB_ICONEXCLAMATION);
    return false;
  }

  InitializeGameText();
  
  pBitmaps_LOD = new LODFile_IconsBitmaps;
  if (!pBitmaps_LOD->Load("data\\bitmaps.lod", "bitmaps"))
  {
    MessageBoxA(nullptr,
                pGlobalTXT_LocalizationStrings[63],
                pGlobalTXT_LocalizationStrings[184], MB_ICONEXCLAMATION);
    return false;
  }

  if (bUseRegistry)
    bUseLoResSprites = ReadWindowsRegistryInt("resolution", 0);
  else
    bUseLoResSprites = GetPrivateProfileIntW(L"settings", L"resolution", 0, pMM6IniFile);

  const char *pSpritesFile = nullptr;
  if (bUseLoResSprites)
    pSpritesFile = "data\\spriteLO.lod";
  else
    pSpritesFile = "data\\sprites.lod";

  pSprites_LOD = new LODFile_Sprites;
  if (!pSprites_LOD->LoadSprites(pSpritesFile))
  {
    MessageBoxA(nullptr,
                pGlobalTXT_LocalizationStrings[63],
                pGlobalTXT_LocalizationStrings[184], MB_ICONEXCLAMATION);
    return false;
  }


#if 0
  if (_access("../MM_VI/data/icons.lod", 0) == 0)
  {
    pIcons_LOD_mm6 = new LODFile_IconsBitmaps;
    if (!pIcons_LOD_mm6->Load("../MM_VI/data/icons.lod", "icons"))
    {
      delete pIcons_LOD_mm6;
      pIcons_LOD_mm6 = nullptr;
      Log::Warning(L"Unable to load mm6:icons.lod");
    }
  }
  else
    Log::Warning(L"Unable to find mm6:icons.lod");
  
  if (_access("../MM_VI/data/bitmaps.lod", 0) == 0)
  {
    pBitmaps_LOD_mm6 = new LODFile_IconsBitmaps;
    if (!pBitmaps_LOD_mm6->Load("../MM_VI/data/bitmaps.lod", "bitmaps"))
    {
      delete pBitmaps_LOD_mm6;
      pBitmaps_LOD_mm6 = nullptr;
      Log::Warning(L"Unable to load mm6:bitmaps.lod");
    }
  }
  else
    Log::Warning(L"Unable to find mm6:bitmaps.lod");

  auto mm6_sprite_container_name = bUseLoResSprites ? "../MM_VI/data/spriteLO.lod"
                                                    : "../MM_VI/data/sprites.lod";
  if (_access(mm6_sprite_container_name, 0) == 0)
  {
    pSprites_LOD_mm6 = new LODFile_Sprites;
    if (!pSprites_LOD_mm6->LoadSprites(mm6_sprite_container_name))
    {
      delete pSprites_LOD_mm6;
      pSprites_LOD_mm6 = nullptr;
      Log::Warning(L"Unable to load mm6:sprites.lod");
    }
  }
  else
    Log::Warning(L"Unable to find mm6:sprites.lod");


  if (_access("../mm8/data/icons.lod", 0) == 0)
  {
    pIcons_LOD_mm8 = new LODFile_IconsBitmaps;
    if (!pIcons_LOD_mm8->Load("../mm8/data/icons.lod", "icons"))
    {
      delete pIcons_LOD_mm8;
      pIcons_LOD_mm8 = nullptr;
      Log::Warning(L"Unable to load mm8:icons.lod");
    }
  }
  else
    Log::Warning(L"Unable to find mm8:icons.lod");


  if (_access("../mm8/data/bitmaps.lod", 0) == 0)
  {
    pBitmaps_LOD_mm8 = new LODFile_IconsBitmaps;
    if (!pBitmaps_LOD_mm8->Load("../mm8/data/bitmaps.lod", "bitmaps"))
    {
      delete pBitmaps_LOD_mm8;
      pBitmaps_LOD_mm8 = nullptr;
      Log::Warning(L"Unable to load mm8:bitmaps.lod");
    }
  }
  else
    Log::Warning(L"Unable to find mm8:bitmaps.lod");
  

  if (_access("../mm8/data/sprites.lod", 0) == 0)
  {
    pSprites_LOD_mm8 = new LODFile_Sprites;
    if (!pSprites_LOD_mm8->LoadSprites("../mm8/data/sprites.lod"))
    {
      delete pSprites_LOD_mm8;
      pSprites_LOD_mm8 = nullptr;
      Log::Warning(L"Unable to load mm8:sprites.lod");
    }
  }
  else
    Log::Warning(L"Unable to find mm8:sprites.lod");
#endif

  {
    void *sft_mm6 = pIcons_LOD_mm6 ? pIcons_LOD_mm6->LoadRaw("dsft.bin", 1) : nullptr,
         *sft_mm8 = nullptr;
    auto  sft_mm7 = pEvents_LOD->LoadRaw("dsft.bin", 1);
    pSpriteFrameTable = new SpriteFrameTable;
    pSpriteFrameTable->FromFile(sft_mm6, sft_mm7, sft_mm8);
    free(sft_mm6);
    free(sft_mm7);
    free(sft_mm8);
    
    void *tft_mm6 = pIcons_LOD_mm6 ? pIcons_LOD_mm6->LoadRaw("dtft.bin", 1) : nullptr,
         *tft_mm8 = nullptr;
    auto  tft_mm7 = pEvents_LOD->LoadRaw("dtft.bin", 1);
    pTextureFrameTable = new TextureFrameTable;
    pTextureFrameTable->FromFile(tft_mm6, tft_mm7, tft_mm8);
    free(tft_mm6);
    free(tft_mm7);
    free(tft_mm8);

    void *tiles_mm6 = pIcons_LOD_mm6 ? pIcons_LOD_mm6->LoadRaw("dtile.bin", 1) : nullptr,
         *tiles_mm8 = nullptr;
    auto  tiles_mm7 = pEvents_LOD->LoadRaw("dtile.bin", 1);
    pTileTable = new TileTable;
    pTileTable->FromFile(tiles_mm6, tiles_mm7, tiles_mm8);
    free(tiles_mm6);
    free(tiles_mm7);
    free(tiles_mm8);
    
    void *pft_mm6 = pIcons_LOD_mm6 ? pIcons_LOD_mm6->LoadRaw("dpft.bin", 1) : nullptr,
         *pft_mm8 = nullptr;
    auto  pft_mm7 = pEvents_LOD->LoadRaw("dpft.bin", 1);
    pPlayerFrameTable = new PlayerFrameTable;
    pPlayerFrameTable->FromFile(pft_mm6, pft_mm7, pft_mm8);
    free(pft_mm6);
    free(pft_mm7);
    free(pft_mm8);

    void *ift_mm6 = pIcons_LOD_mm6 ? pIcons_LOD_mm6->LoadRaw("dift.bin", 1) : nullptr,
         *ift_mm8 = nullptr;
    auto  ift_mm7 = pEvents_LOD->LoadRaw("dift.bin", 1);
    pIconsFrameTable = new IconFrameTable;
    pIconsFrameTable->FromFile(ift_mm6, ift_mm7, ift_mm8);
    free(ift_mm6);
    free(ift_mm7);
    free(ift_mm8);

    void *decs_mm6 = pIcons_LOD_mm6 ? pIcons_LOD_mm6->LoadRaw("ddeclist.bin", 1) : nullptr,
         *decs_mm8 = nullptr;
    auto  decs_mm7 = pEvents_LOD->LoadRaw("ddeclist.bin", 1);
    pDecorationList = new DecorationList;
    pDecorationList->FromFile(decs_mm6, decs_mm7, decs_mm8);
    free(decs_mm6);
    free(decs_mm7);
    free(decs_mm8);

    void *objs_mm6 = pIcons_LOD_mm6 ? pIcons_LOD_mm6->LoadRaw("dobjlist.bin", 1) : nullptr,
         *objs_mm8 = nullptr;
    auto  objs_mm7 = pEvents_LOD->LoadRaw("dobjlist.bin", 1);
    pObjectList = new ObjectList;
    pObjectList->FromFile(objs_mm6, objs_mm7, objs_mm8);
    free(objs_mm6);
    free(objs_mm7);
    free(objs_mm8);

    void *mons_mm6 = pIcons_LOD_mm6 ? pIcons_LOD_mm6->LoadRaw("dmonlist.bin", 1) : nullptr,
         *mons_mm8 = nullptr;
    auto  mons_mm7 = pEvents_LOD->LoadRaw("dmonlist.bin", 1);
    pMonsterList = new MonsterList;
    pMonsterList->FromFile(mons_mm6, mons_mm7, mons_mm8);
    free(mons_mm6);
    free(mons_mm7);
    free(mons_mm8);
    
    void *chests_mm6 = pIcons_LOD_mm6 ? pIcons_LOD_mm6->LoadRaw("dchest.bin", 1) : nullptr,
         *chests_mm8 = nullptr;
    auto  chests_mm7 = pEvents_LOD->LoadRaw("dchest.bin", 1);
    pChestList = new ChestList;
    pChestList->FromFile(chests_mm6, chests_mm7, chests_mm8);
    free(chests_mm6);
    free(chests_mm7);
    free(chests_mm8);

    void *overlays_mm6 = pIcons_LOD_mm6 ? pIcons_LOD_mm6->LoadRaw("doverlay.bin", 1) : nullptr,
         *overlays_mm8 = nullptr;
    auto  overlays_mm7 = pEvents_LOD->LoadRaw("doverlay.bin", 1);
    pOverlayList = new OverlayList;
    pOverlayList->FromFile(overlays_mm6, overlays_mm7, overlays_mm8);
    free(overlays_mm6);
    free(overlays_mm7);
    free(overlays_mm8);

    void *sounds_mm6 = pIcons_LOD_mm6 ? pIcons_LOD_mm6->LoadRaw("dsounds.bin", 1) : nullptr,
         *sounds_mm8 = nullptr;
    auto  sounds_mm7 = pEvents_LOD->LoadRaw("dsounds.bin", 1);
    pSoundList = new SoundList;
    pSoundList->FromFile(sounds_mm6, sounds_mm7, sounds_mm8);
    free(sounds_mm6);
    free(sounds_mm7);
    free(sounds_mm8);
  }



  
  if (dword_6BE364_game_settings_1 & 2 || !(dword_6BE368_debug_settings_2 & 1))// 
                                                //   color depth != 16
                                                // or
                                                //   not in window
  {
    window->SetFullscreenMode();
    pRenderer->InitializeFullscreen();
  }
  else
  {
    window->SetWindowedMode(game_width, game_height);
    pRenderer->SwitchToWindow();
  }

  uSoundVolumeMultiplier = ReadWindowsRegistryInt("soundflag", 9);
  if (uSoundVolumeMultiplier > 9)
    uSoundVolumeMultiplier = 9;
  uMusicVolimeMultiplier = ReadWindowsRegistryInt("musicflag", 9);
  if (uMusicVolimeMultiplier > 9)
    uMusicVolimeMultiplier = 9;
  uVoicesVolumeMultiplier = ReadWindowsRegistryInt("CharVoices", 9);
  if (uVoicesVolumeMultiplier > 9)
    uVoicesVolumeMultiplier = 9;
  bShowDamage = ReadWindowsRegistryInt("ShowDamage", 1) != 0;
  byte_6BE388_graphicsmode = ReadWindowsRegistryInt("graphicsmode", 1);
  if (byte_6BE388_graphicsmode < 0 || byte_6BE388_graphicsmode > 2)
    byte_6BE388_graphicsmode = 0;

  uGammaPos = ReadWindowsRegistryInt("GammaPos", 4);
  if (uGammaPos > 9)
    uGammaPos = 4;
  pGame->pGammaController->Initialize(uGammaPos * 0.1 + 0.6);

  if ( ReadWindowsRegistryInt("Bloodsplats", 1) )
    pGame->uFlags2 |= GAME_FLAGS_2_DRAW_BLOODSPLATS;
  else
    pGame->uFlags2 &= ~GAME_FLAGS_2_DRAW_BLOODSPLATS;

  uTurnSpeed = ReadWindowsRegistryInt("TurnDelta", 0);
  dword_6BE384_2dacceloff = ReadWindowsRegistryInt("2dacceloff", 0);

  if (!bNoSound)
    pAudioPlayer->Initialize();

  pVideoPlayer = new VideoPlayer();
  pVideoPlayer->Initialize(window);

  dword_6BE364_game_settings_1 |= 0x4000;

    switch ( uTurnSpeed )
    {
      case 0: // undefined turn option
        __debugbreak(); // really shouldn't use this mode
        uTurnSpeed = (unsigned int)uCPUSpeed < 199 ? 128 : 64; // adjust turn speed to estimated fps
      break;

      case 1u:             // 16x
        Log::Warning(L"x16 Turn Speed"); // really shouldn't use this mode
        uTurnSpeed = 128;
      break;

      case 2u:             // 32x
        Log::Warning(L"x32 Turn Speed"); // really shouldn't use this mode
        uTurnSpeed = 64;
      break;

      case 3u:             // smooth
        uTurnSpeed = 0;
      break;
    }

  return true;
}

//----- (00465D0B) --------------------------------------------------------
void SecondaryInitialization()
{
  __int16 v4; // ax@4
  signed int v5; // esi@5
  int v6; // ecx@6
  int v7; // edx@7
  ObjectDesc *v8; // eax@7
  char pContainer[32]; // [sp+10h] [bp-Ch]@9

  pMouse->Initialize(window);

  pItemsTable = new ItemsTable;
  pItemsTable->Initialize();

  pBitmaps_LOD->dword_011BA8 = 1;
  pBitmaps_LOD->SetupPalettes(pRenderer->uTargetRBits, pRenderer->uTargetGBits, pRenderer->uTargetBBits);
  pIcons_LOD->SetupPalettes(pRenderer->uTargetRBits, pRenderer->uTargetGBits, pRenderer->uTargetBBits);
  pPaletteManager->SetColorChannelInfo(pRenderer->uTargetRBits, pRenderer->uTargetGBits, pRenderer->uTargetBBits);

  pPaletteManager->SetMistColor(128, 128, 128);
  pPaletteManager->RecalculateAll();
  pSprites_LOD->field_ECAC = 1;
  pObjectList->InitializeSprites();
  pOverlayList->InitializeSprites();
  if (!bNoSound)
    pSoundList->Initialize();

  static const char *pUIAnimNames[4] =
  {
    "glow03", "glow05",
    "torchA", "wizeyeA"
  };
  static unsigned short _4E98D0[4][4] =
  {
    {479, 0, 329, 0},
    {585, 0, 332, 0},
    {468, 0,   0, 0},
    {606, 0,   0, 0}
  };


  for (uint i = 0; i < 4; ++i)
  {
    pUIAnims[i]->uIconID = pIconsFrameTable->FindIcon(pUIAnimNames[i]);
    pIconsFrameTable->InitializeAnimation(pUIAnims[i]->uIconID);

    pUIAnims[i]->uAnimLength = 0;
    pUIAnims[i]->uAnimTime = 0;
    pUIAnims[i]->x = _4E98D0[i][0];
    pUIAnims[i]->y = _4E98D0[i][2];
  }

  v5 = 0;
  if ( (signed int)pObjectList->uNumObjects > 0 )
  {
    v6 = 0;
    do
    {
      BYTE3(v7) = 0;
      v8 = &pObjectList->pObjects[v6];
      ++v6;
      *(short *)((char *)&v7 + 1) = v8->uParticleTrailColorR;
      LOBYTE(v7) = v8->uParticleTrailColorG;
      ++v5;
      v8->uParticleTrailColor = v8->uParticleTrailColorB | (v7 << 8);
    }
    while ( v5 < (signed int)pObjectList->uNumObjects );
  }
  flt_6BE3A0 = 0.55000001f;
  MainMenuUI_Create();
  pGame->pStru6Instance->LoadAnimations();

  for (uint i = 0; i < 7; ++i)
  {
    sprintf(pContainer, "HDWTR%03u", i);
    pRenderer->pHDWaterBitmapIDs[i] = pBitmaps_LOD->LoadTexture(pContainer);
  }

  pNPCStats = new NPCStats;
  memset(pNPCStats->pNPCData, 0, 0x94BCu);
  pNPCStats->Initialize();

  Initialize_GlobalEVT();
  pBitmaps_LOD->_inlined_sub0();
  pSprites_LOD->_inlined_sub0();
  pPaletteManager->LockAll();

  _mkdir("Saves");
  for (uint i = 0; i < 5; ++i)
    for (uint j = 0; j < 6; ++j)
    {
      sprintf(pTmpBuf.data(), "data\\lloyd%d%d.pcx", i, j);
      remove(pTmpBuf.data());
    }

  Initialize_GamesLOD_NewLOD();
  _576E2C_current_minimap_zoom = 512;
  dword_576E28 = 9;
}

void IntegrityTest()
{
  assert(sizeof(MovieHeader) == 44);
  assert(sizeof(SoundDesc_mm6) == 112);
  assert(sizeof(SoundDesc) == 120);
  assert(sizeof(OverlayDesc) == 8);
  assert(sizeof(ChestDesc) == 36);
  assert(sizeof(ObjectDesc_mm6) == 52);
  assert(sizeof(ObjectDesc) == 56);
  assert(sizeof(DecorationDesc) == 84);
  assert(sizeof(IconFrame) == 32);
  assert(sizeof(PlayerFrame) == 10);
  assert(sizeof(TextureFrame) == 20);
  assert(sizeof(SpriteFrame) == 60);
  assert(sizeof(RenderVertexSoft) == 0x30);
  assert(sizeof(RenderBillboard) == 0x34);
  assert(sizeof(Texture) == 0x48);
  assert(sizeof(RGBTexture) == 0x28);
  assert(sizeof(LODFile_IconsBitmaps) == 0x11BB8 + 4); // + virtual dtor ptr
  assert(sizeof(AudioPlayer) == 0xC84);
  assert(sizeof(SoundDesc) == 0x78);
  assert(sizeof(stru339_spell_sound) == 0xAFD8);
  assert(sizeof(VideoPlayer) == 0x108 + 4);
  assert(sizeof(MovieHeader) == 0x2C);
  assert(sizeof(DecorationDesc) == 0x54);
  assert(sizeof(ObjectDesc) == 0x38);
  assert(sizeof(OverlayDesc) == 0x8);
  assert(sizeof(ChestDesc) == 0x24);
  assert(sizeof(TileDesc) == 0x1A);
  assert(sizeof(MonsterDesc_mm6) == 148);
  assert(sizeof(MonsterDesc) == 152);
  assert(sizeof(Timer) == 0x28);
  assert(sizeof(OtherOverlay) == 0x14);
  assert(sizeof(ItemGen) == 0x24);
  assert(sizeof(SpriteObject) == 0x70);
  assert(sizeof(ItemDesc) == 0x30);
  assert(sizeof(ItemsTable) == 0x117A0);
  assert(sizeof(Chest) == 0x14CC);
  assert(sizeof(MapInfo) == 0x44);
  assert(sizeof(SpellInfo) == 0x24);
  assert(sizeof(SpellData) == 0x14);
  assert(sizeof(SpellBuff) == 0x10);
  assert(sizeof(AIDirection) == 0x1C);
  assert(sizeof(ActorJob) == 0xC);
  assert(sizeof(Actor) == 0x344);
  assert(sizeof(LevelDecoration) == 0x20);
  assert(sizeof(KeyboardActionMapping) == 0x20C);
  assert(sizeof(UIAnimation) == 0xD);
  assert(sizeof(SpawnPointMM7) == 0x18);
  assert(sizeof(ODMFace) == 0x134);
  assert(sizeof(BSPNode) == 0x8);
  assert(sizeof(BSPModel) == 0xBC);
  assert(sizeof(OutdoorLocation) == 0x1C28C);
  assert(sizeof(BLVFace) == 0x60);
  assert(sizeof(BLVFaceExtra) == 0x24);
  assert(sizeof(BLVSector) == 0x74);
  assert(sizeof(BLVLightMM7) == 0x10);
  assert(sizeof(BLVDoor) == 0x50);
  assert(sizeof(IndoorLocation) == 0x690);
  //assert(sizeof(ODMRenderParams) == 0x74);
  assert(sizeof(Mouse) == 0x114);
  assert(sizeof(Particle_sw) == 0x68);
  assert(sizeof(Particle) == 0x68);
  assert(sizeof(ParticleEngine) == 0xE430);
  assert(sizeof(Lightmap) == 0xC1C);
  assert(sizeof(LightmapBuilder) == 0x3CBC38);
  assert(sizeof(Vis_SelectionList) == 0x2008);
  assert(sizeof(Vis) == 0x20D0);
  assert(sizeof(PlayerBuffAnim) == 0x10);
  assert(sizeof(ProjectileAnim) == 0x1C);
  assert(sizeof(stru6) == 0x5F8);
  assert(sizeof(IndoorCameraD3D_Vec3) == 0x10);
  assert(sizeof(IndoorCameraD3D_Vec4) == 0x18); //should be 14 (10 vec3 + 4 vdtor)  but 18 coz of his +4 from own vdtor, but it is odd since vdtor already present from vec3
  //assert(sizeof(IndoorCameraD3D) == 0x1A1384);
  assert(sizeof(StationaryLight) == 0xC);
  assert(sizeof(LightsStack_StationaryLight_) == 0x12C8);
  assert(sizeof(MobileLight) == 0x12);
  assert(sizeof(LightsStack_MobileLight_) == 0x1C28);
  assert(sizeof(Game) == 0xE78);
  assert(sizeof(stru141_actor_collision_object) == 0xA8);
  assert(sizeof(ActionQueue) == 0x7C);
  assert(sizeof(NPCData) == 0x4C);
  assert(sizeof(NPCStats) == 0x17FFC);
  assert(sizeof(BspRenderer) == 0x53740);
  assert(sizeof(PaletteManager) == 0x267AF0);
  assert(sizeof(ViewingParams) == 0x26C);
  //assert(sizeof(IndoorCamera) == 0x50);
  assert(sizeof(Bloodsplat) == 0x28);
  assert(sizeof(BloodsplatContainer) == 0xA0C);
  assert(sizeof(TrailParticle) == 0x18);
  assert(sizeof(EventIndex) == 0xC);
  assert(sizeof(_2devent) == 0x34);
  assert(sizeof(stru176) == 0x20);
  assert(sizeof(SavegameHeader) == 0x64);
  assert(sizeof(SavegameList) == 0x3138);
  assert(sizeof(StorylineText) == 0x160);
  assert(sizeof(FactionTable) == 0x1EF1);
  assert(sizeof(Decal) == 0xC20);
  assert(sizeof(DecalBuilder) == 0x30C038);
  assert(sizeof(MonsterInfo) == 0x58);
  assert(sizeof(MonsterStats) == 0x5BA0);
  assert(sizeof(RenderD3D) == 0x148);
//  assert(sizeof(Render) == 0x129844);
  assert(sizeof(Player) == 0x1B3C);
  assert(sizeof(PartyTimeStruct) == 0x678);
  assert(sizeof(Party) == 0x16238);
  assert(sizeof(GUIButton) == 0xBC);
  assert(sizeof(GUIWindow) == 0x54);
  assert(sizeof(GUIProgressBar) == 0x1B8);
  assert(sizeof(GUIFont) == 0x1020);
 // assert(sizeof(stru262_TurnBased) == 0x40);
  assert(sizeof(ArcomageGame) == 0xFB);
  assert(sizeof(CastSpellInfo) == 0x14);
  assert(sizeof(ArcomageCard) == 0x6C);
  assert(sizeof(stru320) == 0x3FC);
  assert(sizeof(TravelInfo) == 0x20);
  assert(sizeof(stru336) == 0x798);
  assert(sizeof(Vec3_short_) == 6);
  assert(sizeof(BLVFace) == 96);
  assert(sizeof(BLVFaceExtra) == 36);
  assert(sizeof(BLVSector) == 116);
  assert(sizeof(LevelDecoration) == 32);
  assert(sizeof(BLVLightMM7) == 16);
  assert(sizeof(BSPNode) == 8);
  assert(sizeof(SpawnPointMM7) == 24);
  assert(sizeof(DDM_DLV_Header) == 40);
  assert(sizeof(Actor) == 836);
  assert(sizeof(SpriteObject) == 112);
  assert(sizeof(Chest) == 5324);
  assert(sizeof(stru123) == 0xC8);
  assert(sizeof(BLVMapOutline) == 12);
  assert(sizeof(LODSprite) == 0x28);
}

bool new_sky = false;
bool new_draw_object_dist = true;
bool change_seasons = false;
bool all_magic = true;
bool wizard_eye = false;
bool debug_information = false;
bool show_picked_face = false;
bool draw_debug_line = false;

//----- (00462C94) --------------------------------------------------------
bool MM_Main(const wchar_t *pCmdLine)
{
  IntegrityTest();

  char test[1024];
  sprintfex(test, "^Pi[%s]: çíàõàð^R[ü;êà;]", "Çîëòàí");

  bool bNoMargareth = false;
  if (pCmdLine && *pCmdLine)
  {
    if (wcsstr(pCmdLine, L"-usedefs"))
      bDebugResouces = 1;
    if (wcsstr(pCmdLine, L"-window"))
      dword_6BE368_debug_settings_2 |= 1u;

    if (wcsstr(pCmdLine, L"-nointro"))
      bNoIntro = true;//dword_6BE364_game_settings_1 |= 4;
    if (wcsstr(pCmdLine, L"-nologo"))
      bNoLogo = true;//dword_6BE364_game_settings_1 |= 8;
    if (wcsstr(pCmdLine, L"-nosound"))
      bNoSound = true; //dword_6BE364_game_settings_1 |= 0x10;

    bWalkSound = ReadWindowsRegistryInt("WalkSound", 1) != 0;
    if (wcsstr(pCmdLine, L"-nowalksound"))
      bWalkSound = false;//dword_6BE364_game_settings_1 |= 0x20;
    if (wcsstr(pCmdLine, L"-novideo"))
    {
      dword_6BE364_game_settings_1 |= 0x40;
      bNoVideo = true;
    }
    if (wcsstr(pCmdLine, L"-nocd"))
      bNoCD = true;
    if (wcsstr(pCmdLine, L"-new_sky"))
      new_sky = true;
    if (wcsstr(pCmdLine, L"-nomarg"))
      bNoMargareth = true;
  }


  /*v8 = _4AC1C9_get_cpu_speed(0, (Vec4_int_ *)a2);
  Rect.left = *(int *)v8;
  Rect.top = *(int *)(v8 + 4);
  Rect.right = *(int *)(v8 + 8);
  Rect.bottom = *(int *)(v8 + 12);
  uCPUSpeed = Rect.bottom;*/
  uCPUSpeed = 2048; // about 2GHz


  if (!MM7_Initialize(640, 480))
  {
    Log::Warning(L"MM init: failed");
    pGame->Deinitialize();
    return 1;
  }

    pEventTimer->Pause();

    SetUserInterface(PartyAlignment_Neutral, false);
    ShowIntroVideo_and_LoadingScreen();
    WriteWindowsRegistryInt("Ran once", 1);
    dword_6BE364_game_settings_1 |= 0x4000;
    pGame->InitializeGammaController();
    SecondaryInitialization();
    pRenderer->SetRasterClipRect(0, 0, 639, 479);
    FinalInitialization();

    Log::Warning(L"MM: entering main loop");
    while ( 1 )
    {
      auto main_menu_window = MainMenuWindow::Create();
      window->AddControl(main_menu_window);

      MainMenu_Loop();
      uGameState = GAME_STATE_PLAYING;
      while ( 1 )
      {
        if (uGameState == GAME_FINISHED ||
            GetCurrentMenuID() == MENU_EXIT_GAME)
        {
          pGame->Deinitialize();
          return true;
        }

        if (GetCurrentMenuID() == MENU_NEWGAME)
        {
          if ( pAudioPlayer->hAILRedbook )
            AIL_redbook_stop(pAudioPlayer->hAILRedbook);
          pParty->Reset();
          pOtherOverlayList->Reset();

          strcpy(pCurrentMapName, pStartingMapName);
          pParty->CreateDefaultParty(0);
          PlayerCreationUI_Initialize();
          if ( PlayerCreationUI_Loop() )
          {
            DeleteCCharFont();
            break;
          }
          DeleteCCharFont();
          bFlashQuestBook = true;
          PlayFullscreenMovie(MOVIE_Emerald);
          //pGame->pCShow->PlayMovie(MOVIE_Emerald, 0);
          SaveNewGame();

          if (bNoMargareth)
            _449B7E_toggle_bit(pParty->_quest_bits, PARTY_QUEST_EMERALD_MARGARETH_OFF, 1);
          pGame->Loop();

          if (uGameState == GAME_STATE_NEWGAME_OUT_GAMEMENU)
          {
            SetCurrentMenuID(MENU_NEWGAME);
            uGameState = GAME_STATE_PLAYING;
            continue;
          }
          else if (uGameState == GAME_STATE_GAME_QUITTING_TO_MAIN_MENU)
            break;

          assert(false && "Invalid game state");
        }
        if (GetCurrentMenuID() == MENU_CREDITS)
        {
         if ( pAudioPlayer->hAILRedbook )
          AIL_redbook_stop(pAudioPlayer->hAILRedbook);
         MainMenuUI_Credits_Loop();
         break;
        }
        if (GetCurrentMenuID() == MENU_5 || GetCurrentMenuID() == MENU_LoadingProcInMainMenu)
        {
          uGameState = GAME_STATE_PLAYING;
		  pGame->Loop();
        }
        else
        {
          if (GetCurrentMenuID() == MENU_DebugBLVLevel)
		  {
			  pMouse->Activate(0);

			  pParty->Reset();
			  pParty->CreateDefaultParty(1);
              
              __debugbreak();
              /*extern void CreateDefaultBLVLevel();
			  CreateDefaultBLVLevel();

			  OPENFILENAMEA ofn;
			  if ( !GetOpenFileNameA((LPOPENFILENAMEA)&ofn) )
			  {
				pMouse->Activate(1);
				break;
			  }
			  _chdir("..\\");
			  strcpy(pCurrentMapName, ofn.lpstrFileTitle);*/
			  pMouse->Activate(1);
			  pGame->Loop();
		  }
        }
        if ( uGameState == GAME_STATE_LOADING_GAME )
        {
          SetCurrentMenuID(MENU_5);
          uGameState = GAME_STATE_PLAYING;
          continue;
        }
        if ( uGameState == GAME_STATE_NEWGAME_OUT_GAMEMENU )
        {
          SetCurrentMenuID(MENU_NEWGAME);
          uGameState = GAME_STATE_PLAYING;
          continue;
        }
        if ( uGameState == GAME_STATE_GAME_QUITTING_TO_MAIN_MENU)// from the loaded game
        {
          pAudioPlayer->StopChannels(-1, -1);
          uGameState = GAME_STATE_PLAYING;
          break;
        }
      }
      if (!bNoSound && pAudioPlayer->hAILRedbook)
      {
        pAudioPlayer->SetMusicVolume(pSoundVolumeLevels[uMusicVolimeMultiplier] * 64.0f);
        AIL_redbook_stop(pAudioPlayer->hAILRedbook);
        unsigned int startms, end_ms;
        AIL_redbook_track_info(pAudioPlayer->hAILRedbook, 0xE, &startms, &end_ms);
        AIL_redbook_play(pAudioPlayer->hAILRedbook, startms + 1, end_ms);
      }
    }

  pGame->Deinitialize();
  return 1;
}

//----- (004637E0) --------------------------------------------------------
char  sub_4637E0_is_there_popup_onscreen()
{
  return dword_507BF0_is_there_popup_onscreen == 1;
}
// 507BF0: using guessed type int dword_507BF0_is_there_popup_onscreen;

//----- (00466082) --------------------------------------------------------
void MM6_Initialize(const wchar_t *pIniFilename)
{
  //int v0; // eax@1
  UINT v1; // eax@18
  size_t v2; // eax@31
  size_t v3; // ebx@32
  size_t v4; // edi@36
  //char v5[120]; // [sp+Ch] [bp-17Ch]@1
  //char String[120]; // [sp+84h] [bp-104h]@32
  char pDefaultGroundTexture[16]; // [sp+FCh] [bp-8Ch]@32
  //char pIniFilename[120]; // [sp+10Ch] [bp-7Ch]@1
  unsigned int v9; // [sp+184h] [bp-4h]@28

  //_getcwd(v5, 120);
  //sprintfex(pIniFilename, "%s\\mm6.ini", v5);
  viewparams = new ViewingParams;
  game_viewport_x = viewparams->uScreen_topL_X = GetPrivateProfileIntW(L"screen", L"vx1", 8, pIniFilename);
  game_viewport_y = viewparams->uScreen_topL_Y = GetPrivateProfileIntW(L"screen", L"vy1", 8, pIniFilename);
  game_viewport_z = viewparams->uScreen_BttmR_X = GetPrivateProfileIntW(L"screen", L"vx2", 468, pIniFilename);
  game_viewport_w = viewparams->uScreen_BttmR_Y = GetPrivateProfileIntW(L"screen", L"vy2", 351, pIniFilename);
  game_viewport_width = game_viewport_z - game_viewport_x;
  game_viewport_height = game_viewport_w - game_viewport_y + 1;


  pAudioPlayer = new AudioPlayer;
  pAudioPlayer->uMixerChannels = GetPrivateProfileIntW(L"settings", L"mixerchannels", 16, pIniFilename);
  if ( pAudioPlayer->uMixerChannels > 16 )
    pAudioPlayer->uMixerChannels = 16;


  if (GetPrivateProfileIntW(L"debug", L"nomonster", 0, pIniFilename))
    dword_6BE368_debug_settings_2 |= 4;
  if (bUseRegistry)
    v1 = ReadWindowsRegistryInt("startinwindow", 0);
  else
    v1 = GetPrivateProfileIntW(L"debug", L"startinwindow", 0, pIniFilename);
  if (v1)
    dword_6BE368_debug_settings_2 |= 1;
  if (GetPrivateProfileIntW(L"debug", L"showFR", 0, pIniFilename))
    dword_6BE368_debug_settings_2 |= 2;
  if (GetPrivateProfileIntW(L"debug", L"nodamage", 0, pIniFilename))
    dword_6BE368_debug_settings_2 |= 0x10;
  if (GetPrivateProfileIntW(L"debug", L"nodecoration", 0, pIniFilename))
    dword_6BE368_debug_settings_2 |= 8;

  wchar_t pStartingMapNameW[1024];
  GetPrivateProfileStringW(L"file", L"startmap", L"out01.odm", pStartingMapNameW, 0x20u, pIniFilename);
  sprintf(pStartingMapName, "%S", pStartingMapNameW);

  v9 = 0;
  if ( strlen(pStartingMapName) )
  {
    do
    {
      if ( pStartingMapName[v9] == ' ' )
        pStartingMapName[v9] = 0;
      ++v9;
      v2 = strlen(pStartingMapName);
    }
    while ( v9 < v2 );
  }

  pODMRenderParams = new ODMRenderParams;
  outdoor_day_top_r      = GetPrivateProfileIntW(L"outdoor", L"RGBDayTop.r", 81, pIniFilename);
  outdoor_day_top_g      = GetPrivateProfileIntW(L"outdoor", L"RGBDayTop.g", 121, pIniFilename);
  outdoor_day_top_b      = GetPrivateProfileIntW(L"outdoor", L"RGBDayTop.b", 236, pIniFilename);
  outdoor_day_bottom_r   = GetPrivateProfileIntW(L"outdoor", L"RGBDayBottom.r", 153, pIniFilename);
  outdoor_day_bottom_g   = GetPrivateProfileIntW(L"outdoor", L"RGBDayBottom.g", 193, pIniFilename);
  outdoor_day_bottom_b   = GetPrivateProfileIntW(L"outdoor", L"RGBDayBottom.b", 237, pIniFilename);
  outdoor_night_top_r    = GetPrivateProfileIntW(L"outdoor", L"RGBNightTop.r", 0, pIniFilename);
  outdoor_night_top_g    = GetPrivateProfileIntW(L"outdoor", L"RGBNightTop.g", 0, pIniFilename);
  outdoor_night_top_b    = GetPrivateProfileIntW(L"outdoor", L"RGBNightTop.b", 0, pIniFilename);
  outdoor_night_bottom_r = GetPrivateProfileIntW(L"outdoor", L"RGBNightBottom.r", 11, pIniFilename);
  outdoor_night_bottom_g = GetPrivateProfileIntW(L"outdoor", L"RGBNightBottom.g", 41, pIniFilename);
  outdoor_night_bottom_b = GetPrivateProfileIntW(L"outdoor", L"RGBNightBottom.b", 129, pIniFilename);
  pODMRenderParams->outdoor_no_mist = GetPrivateProfileIntW(L"debug", L"noMist", 0, pIniFilename);
  pODMRenderParams->bNoSky          = GetPrivateProfileIntW(L"outdoor", L"nosky", 0, pIniFilename);
  pODMRenderParams->bDoNotRenderDecorations = GetPrivateProfileIntW(L"render", L"nodecorations", 0, pIniFilename);
  pODMRenderParams->outdoor_no_wavy_water   = GetPrivateProfileIntW(L"outdoor", L"nowavywater", 0, pIniFilename);
  outdoor_grid_band_1 = GetPrivateProfileIntW(L"outdoor", L"gridband1", 10, pIniFilename);
  outdoor_grid_band_2 = GetPrivateProfileIntW(L"outdoor", L"gridband2", 15, pIniFilename);
  outdoor_grid_band_3 = GetPrivateProfileIntW(L"outdoor", L"gridband3", 25, pIniFilename);
  pODMRenderParams->terrain_gamma  = GetPrivateProfileIntW(L"outdoor", L"ter_gamma", 0, pIniFilename);
  pODMRenderParams->building_gamme = GetPrivateProfileIntW(L"outdoor", L"bld_gamma", 0, pIniFilename);
  mipmapping_terrain_mm1  = GetPrivateProfileIntW(L"mipmapping", L"ter_mm1", 2048, pIniFilename);
  mipmapping_terrain_mm2  = GetPrivateProfileIntW(L"mipmapping", L"ter_mm2", 4096, pIniFilename);
  mipmapping_terrain_mm3  = GetPrivateProfileIntW(L"mipmapping", L"ter_mm3", 8192, pIniFilename);
  mipmapping_building_mm1 = GetPrivateProfileIntW(L"mipmapping", L"bld_mm1", 1024, pIniFilename);
  mipmapping_building_mm2 = GetPrivateProfileIntW(L"mipmapping", L"bld_mm2", 2048, pIniFilename);
  mipmapping_building_mm3 = GetPrivateProfileIntW(L"mipmapping", L"bld_mm3", 4096, pIniFilename);
  pODMRenderParams->shading_dist_shade     = GetPrivateProfileIntW(L"shading", L"dist_shade", 2048, pIniFilename);
  pODMRenderParams->shading_dist_shademist = GetPrivateProfileIntW(L"shading", L"dist_shademist", 4096, pIniFilename);

  int dist = 0x2000;
  extern bool new_draw_object_dist;
  if ( new_draw_object_dist )
      dist = 0x6000;
  pODMRenderParams->shading_dist_mist      = GetPrivateProfileIntW(L"shading", L"dist_mist", dist, pIniFilename);

  wchar_t pDefaultSkyTextureW[1024];
  GetPrivateProfileStringW(L"textures", L"sky", L"plansky1", pDefaultSkyTextureW, 0x10u, pIniFilename);
  sprintf(pDefaultSkyTexture.data(), "%S", pDefaultSkyTextureW);

  wchar_t pDefaultGroundTextureW[1024];
  GetPrivateProfileStringW(L"textures", L"default", L"dirt", pDefaultGroundTextureW, 0x10u, pIniFilename);
  sprintf(pDefaultGroundTexture, "%S", pDefaultGroundTextureW);

  wchar_t pFloat[1024];
  GetPrivateProfileStringW(L"debug", L"recmod1", L"1.0", pFloat, 0x10u, pIniFilename);
  swscanf(pFloat, L"%f", &flt_6BE3A4_debug_recmod1);

  GetPrivateProfileStringW(L"debug", L"recmod2", L"1.0", pFloat, 0x10u, pIniFilename);
  swscanf(pFloat, L"%f", &flt_6BE3A8_debug_recmod2);

  flt_6BE3AC_debug_recmod1_x_1_6 = flt_6BE3A4_debug_recmod1 * 1.666666666666667;

  v3 = 0;
  if ( strlen(pDefaultSkyTexture.data()) )
  {
    do
    {
      if ( pDefaultSkyTexture[v3] == ' ' )
        pDefaultSkyTexture[v3] = 0;
      ++v3;
    }
    while ( v3 < strlen(pDefaultSkyTexture.data()) );
  }
  v4 = 0;
  if ( strlen(pDefaultGroundTexture) )
  {
    do
    {
      if ( pDefaultGroundTexture[v4] == ' ' )
        pDefaultGroundTexture[v4] = 0;
      ++v4;
    }
    while ( v4 < strlen(pDefaultGroundTexture) );
  }

  texmapping_terrain_subdivsize  = GetPrivateProfileIntW(L"texmapping", L"terrain_subdivsize", 16, pIniFilename);
  texmapping_terrain_subdivpow2  = GetPrivateProfileIntW(L"texmapping", L"terrain_subdivpow2", 4, pIniFilename);
  texmapping_building_subdivsize = GetPrivateProfileIntW(L"texmapping", L"building_subdivsize", 32, pIniFilename);
  texmapping_building_subdivpow2 = GetPrivateProfileIntW(L"texmapping", L"building_subdivpow2", 5, pIniFilename);

  MM7Initialization();
}

//----- (004666D5) --------------------------------------------------------
void  MM7Initialization()
{
  if ( uCurrentlyLoadedLevelType == LEVEL_Outdoor )
  {
    if ( !byte_6BE388_graphicsmode )
    {
      outdoor_grid_band_1 = 10;
      outdoor_grid_band_2 = 15;
      outdoor_grid_band_3 = 20;
      pODMRenderParams->shading_dist_mist = 8192;
      pODMRenderParams->bNoSky = false;
      LOBYTE(viewparams->field_20) = 0;
    }
    mipmapping_terrain_mm1 = 1024;
    mipmapping_terrain_mm2 = 2048;
    mipmapping_building_mm1 = 1024;
    mipmapping_building_mm2 = 2048;
    pODMRenderParams->shading_dist_shade = 2048;
    pODMRenderParams->terrain_gamma = 0;
    pODMRenderParams->building_gamme = 0;
    mipmapping_terrain_mm3 = 4096;
    mipmapping_building_mm3 = 4096;
    pODMRenderParams->shading_dist_shademist = 4096;
    pODMRenderParams->outdoor_no_wavy_water = 0;
    //_47F4D3_initialize_terrain_bezier_stuff(outdoor_grid_band_1, outdoor_grid_band_2, outdoor_grid_band_3);
    {
      pODMRenderParams->outdoor_grid_band_3 = outdoor_grid_band_3;
      pODMRenderParams->uPickDepth = outdoor_grid_band_3 * 512;
    }
  }
  else
  {
    LOBYTE(viewparams->field_20) = 0;
  }
  pParty->uFlags |= 2u;
  viewparams->uSomeY = viewparams->uScreen_topL_Y;
  viewparams->uSomeX = viewparams->uScreen_topL_X;
  viewparams->uSomeZ = viewparams->uScreen_BttmR_X;
  viewparams->uSomeW = viewparams->uScreen_BttmR_Y;

  pViewport->SetScreen(viewparams->uScreen_topL_X, viewparams->uScreen_topL_Y, viewparams->uScreen_BttmR_X, viewparams->uScreen_BttmR_Y);
  if ( uCurrentlyLoadedLevelType == LEVEL_Outdoor )
    pODMRenderParams->Initialize();
}

//----- (00466C40) --------------------------------------------------------
const wchar_t *MENU_STATE_to_string(MENU_STATE m)
{
  switch (m)
  {
    case -1:                return L"-1";
    case MENU_MAIN:         return L"MENU_MAIN";
    case MENU_NEWGAME:      return L"MENU_NEWGAME";
    case MENU_CREDITS:      return L"MENU_CREDITS";
    case MENU_SAVELOAD:     return L"MENU_SAVELOAD";
    case MENU_EXIT_GAME:    return L"MENU_EXIT_GAME";
    case MENU_5:            return L"MENU_5";
    case MENU_CREATEPARTY:  return L"MENU_CREATEPARTY";
    case MENU_NAMEPANELESC: return L"MENU_NAMEPANELESC";
    case MENU_CREDITSPROC:  return L"MENU_CREDITSPROC";
    case MENU_LoadingProcInMainMenu: return L"MENU_LoadingProcInMainMenu";
    case MENU_DebugBLVLevel:           return L"MENU_DebugBLVLevel";
    case MENU_CREDITSCLOSE: return L"MENU_CREDITSCLOSE";
    default:                return L"unk";
  };
};
void SetCurrentMenuID(MENU_STATE uMenu)
{
  uCurrentMenuID = uMenu;

  Log::Warning(L"CurrentMenu = %s", MENU_STATE_to_string(uMenu));
}

//----- (00466CA0) --------------------------------------------------------
MENU_STATE GetCurrentMenuID()
{
  return uCurrentMenuID;
}


//----- (0046A6AC) --------------------------------------------------------
int __fastcall _46A6AC_spell_render(int a1, int a2, int a3)
{
  int result; // eax@2
  unsigned int v4; // edx@3
  int *v5; // esi@6
  unsigned int v6; // ebx@6
  signed int v7; // edi@9
  unsigned __int16 v8; // ax@9
  int i; // eax@14
  int v10; // ecx@19
  int v11; // [sp+0h] [bp-18h]@1
  unsigned int v12; // [sp+4h] [bp-14h]@3
  unsigned int v13; // [sp+8h] [bp-10h]@4
  int v14; // [sp+Ch] [bp-Ch]@1
  int *v15; // [sp+10h] [bp-8h]@4
  int v16; // [sp+14h] [bp-4h]@3

  v11 = a2;
  v14 = a1;
  if ( pRenderer->pRenderD3D )
  {
    result = _46A6AC_spell_render_d3d(a1, a2, a3);
  }
  else
  {
    __debugbreak(); // SW render never called
    v16 = 0;
    v4 = viewparams->uScreen_topL_X;
    v12 = viewparams->uScreen_BttmR_X;
    if ( (signed int)viewparams->uScreen_topL_Y < (signed int)viewparams->uScreen_BttmR_Y )
    {
      v15 = &pRenderer->pActiveZBuffer[viewparams->uScreen_topL_X + 640 * viewparams->uScreen_topL_Y];
      v13 = ((viewparams->uScreen_BttmR_Y - viewparams->uScreen_topL_Y - 1) >> 1) + 1;
      do
      {
        if ( (signed int)v4 < (signed int)v12 )
        {
          v5 = v15;
          v6 = ((v12 - v4 - 1) >> 1) + 1;
          do
          {
            if ( PID_TYPE(*(char *)v5) == OBJECT_Actor)
            {
              if ( *v5 <= a3 << 16 )
              {
                v7 = PID_ID((signed int)(unsigned __int16)*v5);
                v8 = pActors[v7].uAIState;
                if ( v8 != 5 )
                {
                  if ( v8 != 4 && v8 != 11 && v8 != 17 && v8 != 19 )
                  {
                    for ( i = 0; i < v16; ++i )
                    {
                      if ( *(int *)(v14 + 4 * i) == v7 )
                        break;
                    }
                    if ( i == v16 )
                    {
                      if ( i < v11 - 1 )
                      {
                        v10 = v16++;
                        *(int *)(v14 + 4 * v10) = v7;
                      }
                    }
                  }
                }
              }
            }
            v5 += 2;
            --v6;
          }
          while ( v6 );
        }
        v15 += 1280;
        --v13;
      }
      while ( v13 );
    }
    result = v16;
  }
  return result;
}

//----- (0046A7C8) --------------------------------------------------------
int __fastcall _46A6AC_spell_render_d3d(int a1, int a2, int a3)
{
  unsigned int v3; // eax@2
  int v4; // ecx@2
  unsigned int v5; // eax@2
  unsigned int v6; // eax@4
  unsigned int v7; // edi@4
  unsigned __int16 v8; // ax@4
  float v9; // ST00_4@9
  int v10; // ecx@11
  unsigned int v12; // [sp+10h] [bp-14h]@1
  int v13; // [sp+14h] [bp-10h]@1
  int v14; // [sp+18h] [bp-Ch]@1
  int v15; // [sp+1Ch] [bp-8h]@1
  unsigned int a1a; // [sp+20h] [bp-4h]@1

  v13 = a1;
  v14 = a2;
  v15 = 0;
  a1a = 0;
  v12 = pRenderer->GetBillboardDrawListSize();
  if ( (signed int)v12 > 0 )
  {
    do
    {
      v3 = pRenderer->GetParentBillboardID(a1a);
      v4 = pBillboardRenderList[v3].sZValue;
      v5 = (unsigned __int16)pBillboardRenderList[v3].object_pid;
      if ( PID_TYPE(v5) == OBJECT_Actor)
      {
        if ( v4 <= (unsigned int)(a3 << 16) )
        {
          v6 = PID_ID(v5);
          v7 = v6;
          v8 = pActors[v6].uAIState;
          if ( v8 != Dead )
          {
            if ( v8 != Dying )
            {
              if ( v8 != Removed )
              {
                if ( v8 != Disabled )
                {
                  if ( v8 != Summoned )
                  {
                    v9 = (double)a3;
                    if ( pGame->pVisInstance->DoesRayIntersectBillboard(v9, a1a) )
                    {
                      if ( v15 < v14 - 1 )
                      {
                        v10 = v15++;
                        *(int *)(v13 + 4 * v10) = v7;
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
      ++a1a;
    }
    while ( (signed int)a1a < (signed int)v12 );
  }
  return v15;
}

//----- (0046A89E) --------------------------------------------------------
int __fastcall _46A89E_immolation_effect(int a1, int a2, int a3)
{
  signed int v3; // edi@1
  Actor *v4; // esi@2
  int v5; // ebx@3
  int v6; // eax@3
  int v7; // ebx@3
  unsigned int v8; // ecx@3
  int v9; // edx@4
  unsigned int v10; // edx@6
  unsigned int v11; // edx@8
  __int16 v12; // ax@10
  int v13; // ecx@15
  int v15; // [sp+4h] [bp-18h]@1
  int v16; // [sp+8h] [bp-14h]@1
  int v17; // [sp+Ch] [bp-10h]@3
  int v18; // [sp+10h] [bp-Ch]@3
  int v19; // [sp+18h] [bp-4h]@1

  v3 = 0;
  v15 = a2;
  v16 = a1;
  v19 = 0;
  if ( (signed int)uNumActors > 0 )
  {
    v4 = pActors.data();//[0].uAIState;
    do
    {
	  v5 = abs(v4->vPosition.x - pParty->vPosition.x);
	  v17 = abs(v4->vPosition.y - pParty->vPosition.y);
	  v18 = abs(v4->vPosition.z - pParty->vPosition.z);
      v6 = v5;
      v7 = v17;
      v8 = v18;
      if ( v6 < v17 )
      {
        v9 = v6;
        v6 = v17;
        v7 = v9;
      }
      if ( v6 < v18 )
      {
        v10 = v6;
        v6 = v18;
        v8 = v10;
      }
      if ( v7 < (signed int)v8 )
      {
        v11 = v8;
        v8 = v7;
        v7 = v11;
      }
      if ( (signed int)(((unsigned int)(11 * v7) >> 5) + (v8 >> 2) + v6) <= a3 )
      {
		v12 = v4->uAIState;
        if ( v4->uAIState != 5 )
        {
          if ( v12 != 4 )
          {
            if ( v12 != 11 )
            {
              if ( v12 != 19 )
              {
                if ( v12 != 17 )
                {
                  v13 = v19++;
                  *(int *)(v16 + 4 * v13) = v3;
                  if ( v19 >= v15 - 1 )
                    break;
                }
              }
            }
          }
        }
      }
      ++v3;
      ++v4;
    }
    while ( v3 < (signed int)uNumActors );
  }
  return v19;
}

//----- (0046A99B) --------------------------------------------------------
int  sub_46A99B()
{
  int v0; // ebx@1
  signed int v1; // ecx@1
  signed int v2; // esi@1
  int v3; // edx@1
  signed int v4; // eax@1
  int v5; // ecx@2
  int *v6; // eax@3
  int v7; // ecx@3
  int v8; // edx@5
  int v9; // edi@6
  unsigned __int8 v10; // zf@14
  char v11; // sf@14
  char v12; // of@14
  int *v14; // esi@19
  signed int v15; // ebx@19
  int i; // edi@20
  int v17; // ecx@21
  int v18; // edi@26
  int v19; // esi@27
  unsigned int v20; // ecx@27
  BLVFace *v21; // eax@32
  unsigned int v22; // ecx@33
  const char *v23; // eax@35
  const char *v24; // ecx@36
  LevelDecoration *v25; // ecx@43
  LevelDecoration *v26; // edi@43
  __int16 v27; // ax@43
  int v28; // [sp+Ch] [bp-18h]@1
  int v29; // [sp+14h] [bp-10h]@2
  int v30; // [sp+18h] [bp-Ch]@1
  signed int v31; // [sp+1Ch] [bp-8h]@3
  int v32; // [sp+20h] [bp-4h]@1

  v0 = 0;
  v1 = (signed int)(viewparams->uScreen_BttmR_Y + viewparams->uScreen_topL_Y) >> 1;
  v2 = (signed int)(viewparams->uScreen_topL_X + viewparams->uScreen_BttmR_X) >> 1;
  v3 = v1 - 50;
  v32 = 0;
  v4 = 100;
  v30 = v1 - 50;
  v28 = v1 + 50;
  if ( v1 - 50 >= v1 + 50 )
    return 0;
  v5 = 640 * v3;
  v29 = 640 * v3;
  while ( 2 )
  {
    v6 = &pRenderer->pActiveZBuffer[v2 + v5 - v4 / 2]-1;
    v7 = v0;
    //while ( 1 )
    for( int i = 0; i < 100; i++)
	{
	  ++v6;
      v8 = *v6;
      if ( (unsigned int)*v6 <= 0x2000000 )
      {
        v9 = 0;
        if ( v7 > 0 )
        {
          do
          {
            if ( dword_7201B0_zvalues[v9] == (unsigned __int16)v8 )
              break;
            ++v9;
          }
          while ( v9 < v0 );
        }
        if ( v9 == v0 )
        {
          if ( v8 & 7 )
          {
            dword_720020_zvalues[v7] = v8;
            dword_7201B0_zvalues[v7] = (unsigned __int16)v8;
            ++v7;
            ++v0;
            v32 = v0;
            if ( v7 == 99 )
            {
              v12 = 0;
              v10 = v0 == 0;
              v11 = v0 < 0;
              goto LABEL_18;
            }
          }
        }
      }
     }
    v4 = -1;
    ++v30;
    v5 = v29 + 640;
    v29 += 640;
    if ( v30 >= v28 )
      break;
  }
  v12 = 0;
  v10 = v0 == 0;
  v11 = v0 < 0;
  if ( !v0 )
    return 0;
LABEL_18:
  if ( !((unsigned __int8)(v11 ^ v12) | v10) )
  {
    v14 = dword_720020_zvalues.data();
    v15 = 1;
    do
    {
      for ( i = v15; i < v32; ++i )
      {
        v17 = dword_720020_zvalues[i];
        if ( v17 < *v14 )
        {
          dword_720020_zvalues[i] = *v14;
          *v14 = v17;
        }
      }
      ++v15;
      ++v14;
    }
    while ( v15 - 1 < v32 );
    v0 = v32;
  }
  v18 = 0;
  if ( v0 <= 0 )
    return 0;
  while ( 1 )
  {
    v19 = dword_720020_zvalues[v18] & 0xFFFF;
    v20 = PID_ID(dword_720020_zvalues[v18]);
    if ( PID_TYPE(dword_720020_zvalues[v18]) == OBJECT_Item)
    {
      if ( v20 >= 0x3E8 )
        return 0;
      if ( !(pSpriteObjects[v20].uAttributes & 0x10) )
      {
        v23 = pSpriteObjects[v20].stru_24.GetDisplayName();
        v24 = v23;
	    GameUI_SetFooterString(v24);
	    return v19;
      }
    }
    else
	{
		if ( PID_TYPE(dword_720020_zvalues[v18]) == OBJECT_Actor)
		{
		  if ( v20 >= 0xBB8 )
			return 0;
		  v24 = (const char *)&pActors[v20];
		  GameUI_SetFooterString(v24);
		  return v19;
		}
		if ( PID_TYPE(dword_720020_zvalues[v18]) == OBJECT_Decoration)
		  break;
		if ( PID_TYPE(dword_720020_zvalues[v18]) == OBJECT_BModel)
		{
		  if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
		  {
			v21 = &pIndoor->pFaces[v20];
			if ( BYTE3(v21->uAttributes) & 2 )
				v22 = pIndoor->pFaceExtras[v21->uFaceExtraID].uEventID;
		  }
		  else
		  {
			if ( (dword_720020_zvalues[v18] & 0xFFFFu) >> 9 < pOutdoor->uNumBModels )
			{
				v22 = pOutdoor->pBModels[(dword_720020_zvalues[v18] & 0xFFFFu) >> 9].pFaces[v20 & 0x3F].sCogTriggeredID;
				if ( v22 )
				{
					v23 = GetEventHintString(v22);
					if ( v23 )
					{
						v24 = v23;
						GameUI_SetFooterString(v24);
						return v19;
					}
				}
			}
		  }
		}
	}
	++v18;
    if ( v18 >= v0 )
      return 0;
  }
  v25 = &pLevelDecorations[v20];
  v26 = v25;
  v27 = v25->uEventID;
  if ( v27 )
  {
    v23 = GetEventHintString(v27);
    if ( !v23 )
      return v19;
	v24 = v23;
	GameUI_SetFooterString(v24);
	return v19;
  }
  if ( v25->IsInteractive() )
    v24 = pNPCTopics[stru_5E4C90_MapPersistVars._decor_events[v26->_idx_in_stru123 - 75] + 379].pTopic;
  else
    v24 = pDecorationList->pDecorations[v26->uDecorationDescID].field_20;
  GameUI_SetFooterString(v24);
  return v19;
}

//----- (0046BDA8) --------------------------------------------------------
unsigned int  GetGravityStrength()
{
  int v0; // eax@1

  v0 = ~LOBYTE(pGame->uFlags2) & 8;
  LOBYTE(v0) = v0 | 2;
  return (unsigned int)v0 >> 1;
}

//----- (0046BDC0) --------------------------------------------------------
void  UpdateUserInput_and_MapSpecificStuff()
{
  if ( dword_6BE364_game_settings_1 & 0x80 )
  {
    LOBYTE(dword_6BE364_game_settings_1) = dword_6BE364_game_settings_1 & 0x7F;
  }
  else
  {
    if (uCurrentlyLoadedLevelType == LEVEL_Indoor)
      BLV_UpdateUserInputAndOther();
    else if (uCurrentlyLoadedLevelType == LEVEL_Outdoor)
        ODM_UpdateUserInputAndOther();

    area_of_effect__damage_evaluate();
  }
}

//----- (0046BFFA) --------------------------------------------------------
bool __fastcall _46BFFA_check_object_intercept(unsigned int uLayingItemID, signed int a2)
{
  //SpriteObject *v2; // esi@1
  ObjectDesc *v3; // ebx@1
  unsigned __int16 v5; // cx@9
  //unsigned __int16 v6; // cx@14
  //signed int v7; // edx@14
  unsigned int v8; // eax@19
  unsigned int v9; // edi@19
  signed int v10; // ebx@19
  char *v11; // edx@20
  unsigned __int16 v12; // ax@23
  int v13; // eax@27
  unsigned int v14; // ebx@33
  //unsigned __int16 v15; // ax@35
  int v16; // eax@36
  unsigned int v17; // eax@37
  __int16 v18; // di@37
  signed int v19; // edx@37
  unsigned __int8 v20; // sf@37
  char *v21; // ecx@38
  unsigned __int16 v22; // ax@41
  unsigned int v23; // eax@46
  signed int v24; // ebx@46
  char *v25; // edx@47
  int v26; // edx@54
  int v27; // edx@55
  int v28; // edx@56
  int v29; // edx@57
  unsigned __int16 v30; // ax@60
  unsigned int v31; // ecx@60
  int v32; // edi@60
  unsigned int v33; // eax@65
  signed int v34; // edx@65
  char *v35; // ecx@66
  unsigned __int16 v36; // ax@69
  int v37; // ST14_4@72
  int v38; // eax@72
  int v39; // ST10_4@72
  int v40; // ST0C_4@72
  int v41; // edx@76
  int v42; // edx@77
  int v43; // edx@78
  unsigned __int8 v44; // zf@79
  int v45; // edx@79
  unsigned __int16 v46; // ax@80
  int v47; // eax@81
  int v48; // edx@87
  int v49; // edx@88
  int v50; // edx@89
  unsigned int v51; // eax@93
  signed int v52; // ebx@93
  char *v53; // edx@94
  unsigned __int16 v54; // ax@98
  unsigned int v55; // ecx@98
  signed int v56; // ebx@98
  char *v57; // edx@99
  unsigned __int16 v58; // ax@102
  unsigned __int16 v59; // ax@107
  unsigned int v60; // ecx@107
  signed int v61; // ebx@107
  char *v62; // edx@108
  unsigned __int16 v63; // ax@111
  int v64; // ebx@114
  signed int v65; // eax@114
  unsigned int v66; // edi@123
  unsigned __int16 v67; // ax@124
  unsigned int v68; // ecx@124
  signed int v69; // ebx@124
  char *v70; // edx@125
  unsigned __int16 v71; // ax@128
  unsigned int v72; // ebx@131
  //int v73; // ST14_4@132
  //int v74; // ST10_4@132
  //int v75; // ebx@132
  //int v76; // ST0C_4@132
  //unsigned __int16 v77; // ax@132
  int v78; // eax@133
  char v79; // zf@139
  unsigned int v80; // eax@140
  signed int v81; // edx@140
  char *v82; // ecx@141
  unsigned __int16 v83; // ax@144
  unsigned __int16 v84; // ax@151
  unsigned int v85; // ecx@151
  signed int v86; // ebx@151
  char *v87; // edx@152
  unsigned __int16 v88; // ax@155
  unsigned int v89; // eax@158
  int v90; // ST34_4@159
  int v91; // eax@159
  unsigned int v92; // eax@163
  unsigned int v93; // eax@177
  char *v94; // ecx@178
  unsigned __int16 v95; // ax@181
  unsigned __int16 v96; // ax@184
  int v97; // eax@185
  unsigned __int16 v98; // ax@191
  unsigned int v99; // ecx@191
  char v100; // ST18_1@198
  //int v101; // ST14_4@198
  int v102; // eax@198
  //int v103; // ST10_4@198
  //int v104; // ST0C_4@198
  //unsigned __int16 v105; // ax@200
  signed int v106; // eax@208
  unsigned int v107; // edx@220
  signed int v108; // ebx@225
  unsigned int v109; // eax@234
  signed int v110; // ebx@234
  char *v111; // ecx@235
  unsigned __int16 v112; // ax@238
  unsigned __int16 v113; // si@241
  int v114; // eax@242
  int v115; // eax@245
  //signed int v116; // eax@245
  unsigned __int16 v117; // ax@251
  unsigned int v118; // ecx@251
  signed int v119; // ebx@251
  char *v120; // edx@252
  unsigned __int16 v121; // ax@255
  unsigned int v122; // eax@260
  char *v123; // edx@261
  int v124; // eax@267
  int v125; // [sp-20h] [bp-4Ch]@28
  //signed int v126; // [sp-1Ch] [bp-48h]@27
  //unsigned int v127; // [sp-18h] [bp-44h]@27
  //signed int v128; // [sp-14h] [bp-40h]@27
  //signed int v129; // [sp-10h] [bp-3Ch]@27
  //int v130; // [sp-Ch] [bp-38h]@27
  //unsigned int v131; // [sp-8h] [bp-34h]@27
  char v132; // [sp-8h] [bp-34h]@131
  //int v133; // [sp-4h] [bp-30h]@27
  char v134; // [sp-4h] [bp-30h]@131
  signed int v135; // [sp-4h] [bp-30h]@217
  int v136; // [sp+Ch] [bp-20h]@208
  int v137; // [sp+10h] [bp-1Ch]@208
  signed int v138; // [sp+14h] [bp-18h]@207
  signed int v139; // [sp+18h] [bp-14h]@208
  char *v140; // [sp+1Ch] [bp-10h]@61
  signed int v141; // [sp+1Ch] [bp-10h]@117
  unsigned int v142; // [sp+1Ch] [bp-10h]@158
  signed int v143; // [sp+1Ch] [bp-10h]@172
  char *v144; // [sp+1Ch] [bp-10h]@192
  signed int v145; // [sp+20h] [bp-Ch]@1
  signed int v146; // [sp+20h] [bp-Ch]@60
  int v147; // [sp+20h] [bp-Ch]@72
  signed int v148; // [sp+20h] [bp-Ch]@158
  //int v149; // [sp+20h] [bp-Ch]@198
  unsigned __int16 v150; // [sp+20h] [bp-Ch]@208
  int v151; // [sp+24h] [bp-8h]@1
  signed int v152; // [sp+24h] [bp-8h]@208
  unsigned int v153; // [sp+28h] [bp-4h]@1

  v153 = uLayingItemID;
  //v2 = &pSpriteObjects[uLayingItemID];
  v3 = &pObjectList->pObjects[pSpriteObjects[uLayingItemID].uObjectDescID];
  v145 = a2;
  v151 = PID_TYPE(a2);
  if ( PID_TYPE(a2) == OBJECT_Actor)
  {
    if ( PID_TYPE(pSpriteObjects[uLayingItemID].spell_caster_pid) == OBJECT_Actor 
      && !pActors[PID_ID(pSpriteObjects[uLayingItemID].spell_caster_pid)].GetActorsRelation(&pActors[PID_ID(a2)]) )
      return 1;
  }
  else
  {
    if ( PID_TYPE(a2) == OBJECT_Player && PID_TYPE(pSpriteObjects[uLayingItemID].spell_caster_pid) == OBJECT_Player)
      return 1;
  }
  if ( pParty->bTurnBasedModeOn == 1 )
  {
    v5 = pSpriteObjects[uLayingItemID].uAttributes;
    if ( v5 & 4 )
    {
      --pTurnEngine->pending_actions;
      pSpriteObjects[uLayingItemID].uAttributes = v5 & 0xFFFB;
    }
  }
  if ( v151 == OBJECT_BModel && PID_TYPE(pSpriteObjects[uLayingItemID].spell_caster_pid) != OBJECT_Player)
      {
      if (PID_ID(pSpriteObjects[uLayingItemID].spell_caster_pid) < 500)  //bugfix  PID_ID(v2->spell_caster_pid)==1000
         BYTE2(pActors[PID_ID(pSpriteObjects[uLayingItemID].spell_caster_pid)].uAttributes) |= 4;
      }

  //v6 = v2->uType;
  //v7 = v2->uType;
  if ( pSpriteObjects[uLayingItemID].uType > 3060 )
  {
    if ( pSpriteObjects[uLayingItemID].uType > 6090 )
    {
      if ( pSpriteObjects[uLayingItemID].uType > 8090 )
      {
        if ( pSpriteObjects[uLayingItemID].uType == 9010 )
          goto LABEL_247;
        if ( pSpriteObjects[uLayingItemID].uType != 9030 )
        {
          if ( pSpriteObjects[uLayingItemID].uType != 9040 )
          {
            if ( pSpriteObjects[uLayingItemID].uType != 9080 )
              return 0;
LABEL_191:
            v98 = pSpriteObjects[uLayingItemID].uType + 1;
            v99 = pObjectList->uNumObjects;
            v32 = 0;
            pSpriteObjects[uLayingItemID].uType = v98;
            v146 = 0;
            if ( (signed int)v99 > 0 )
            {
              v144 = (char *)&pObjectList->pObjects->uObjectID;
              while ( v98 != *(short *)v144 )
              {
                ++v146;
                v144 += 56;
                if ( v146 >= (signed int)v99 )
                {
                  pSpriteObjects[uLayingItemID].uObjectDescID = 0;
                  if ( !v32 )
                    SpriteObject::OnInteraction(v153);
                  v100 = pSpriteObjects[uLayingItemID].field_61;
                  pSpriteObjects[uLayingItemID].uSpriteFrameID = v32;
                  v102 = 8 * v153;
                  LOBYTE(v102) = PID(OBJECT_Item,v153);
                  pSpriteObjects[uLayingItemID].vVelocity.x = v32;
                  pSpriteObjects[uLayingItemID].vVelocity.y = v32;
                  pSpriteObjects[uLayingItemID].vVelocity.z = v32;
                  stru_50FE08.Add(v102, 512, pSpriteObjects[uLayingItemID].vPosition.x, pSpriteObjects[uLayingItemID].vPosition.y, pSpriteObjects[uLayingItemID].vPosition.z, v100, v32);
                  if (v3->uFlags & OBJECT_DESC_TRIAL_PARTICLE)
                    trail_particle_generator.GenerateTrailParticles(pSpriteObjects[uLayingItemID].vPosition.x, pSpriteObjects[uLayingItemID].vPosition.y, pSpriteObjects[uLayingItemID].vPosition.z, v3->uParticleTrailColor);
                  if ( pSpriteObjects[uLayingItemID].uSoundID == (short)v32 )
                    v47 = 0;
                  else
                    v47 = (signed __int16)pSpriteObjects[uLayingItemID].uSoundID + 4;
                  v125 = word_4EE088_sound_ids[pSpriteObjects[uLayingItemID].spell_id - 1] + 1;
                  pAudioPlayer->PlaySound((SoundID)v125, v102, v32, -1, v32, v47, v32, v32);
                  return 0;
                }
              }
              pSpriteObjects[uLayingItemID].uObjectDescID = v146;
              if ( v146 == (short)v32 )
                SpriteObject::OnInteraction(v153);
              v100 = pSpriteObjects[uLayingItemID].field_61;
              pSpriteObjects[uLayingItemID].uSpriteFrameID = v32;
              v102 = 8 * v153;
              LOBYTE(v102) = PID(OBJECT_Item,v153);
              pSpriteObjects[uLayingItemID].vVelocity.x = v32;
              pSpriteObjects[uLayingItemID].vVelocity.y = v32;
              pSpriteObjects[uLayingItemID].vVelocity.z = v32;
              stru_50FE08.Add(v102, 512, pSpriteObjects[uLayingItemID].vPosition.x, pSpriteObjects[uLayingItemID].vPosition.y, pSpriteObjects[uLayingItemID].vPosition.z, v100, v32);
              if (v3->uFlags & OBJECT_DESC_TRIAL_PARTICLE)
                trail_particle_generator.GenerateTrailParticles(pSpriteObjects[uLayingItemID].vPosition.x, pSpriteObjects[uLayingItemID].vPosition.y, pSpriteObjects[uLayingItemID].vPosition.z, v3->uParticleTrailColor);
              if ( pSpriteObjects[uLayingItemID].uSoundID == (short)v32 )
                v47 = 0;
              else
                v47 = (signed __int16)pSpriteObjects[uLayingItemID].uSoundID + 4;
              v125 = word_4EE088_sound_ids[pSpriteObjects[uLayingItemID].spell_id - 1] + 1;
              pAudioPlayer->PlaySound((SoundID)v125, v102, v32, -1, v32, v47, v32, v32);
              return 0;
            }
            pSpriteObjects[uLayingItemID].uObjectDescID = 0;
            if ( !v32 )
              SpriteObject::OnInteraction(v153);
            v100 = pSpriteObjects[uLayingItemID].field_61;
            pSpriteObjects[uLayingItemID].uSpriteFrameID = v32;
            v102 = 8 * v153;
            LOBYTE(v102) = PID(OBJECT_Item,v153);
            pSpriteObjects[uLayingItemID].vVelocity.x = v32;
            pSpriteObjects[uLayingItemID].vVelocity.y = v32;
            pSpriteObjects[uLayingItemID].vVelocity.z = v32;
            stru_50FE08.Add(v102, 512, pSpriteObjects[uLayingItemID].vPosition.x, pSpriteObjects[uLayingItemID].vPosition.y, pSpriteObjects[uLayingItemID].vPosition.z, v100, v32);
            if (v3->uFlags & OBJECT_DESC_TRIAL_PARTICLE)
              trail_particle_generator.GenerateTrailParticles(pSpriteObjects[uLayingItemID].vPosition.x, pSpriteObjects[uLayingItemID].vPosition.y, pSpriteObjects[uLayingItemID].vPosition.z, v3->uParticleTrailColor);
            if ( pSpriteObjects[uLayingItemID].uSoundID == (short)v32 )
              v47 = 0;
            else
              v47 = (signed __int16)pSpriteObjects[uLayingItemID].uSoundID + 4;
            v125 = word_4EE088_sound_ids[pSpriteObjects[uLayingItemID].spell_id - 1] + 1;
            pAudioPlayer->PlaySound((SoundID)v125, v102, v32, -1, v32, v47, v32, v32);
            return 0;
          }
          goto LABEL_93;
        }
LABEL_172:
        v143 = 17030;
        switch ( pSpriteObjects[uLayingItemID].uType )
        {
          case 0x1798u:
            v143 = 15040;
            break;
          case 0xFAAu:
            v143 = 13010;
            break;
          case 0x2346u:
            v143 = 18030;
            break;
        }
        v138 = 1;
        if ( v151 != OBJECT_Actor)
        {
          if ( pSpriteObjects[uLayingItemID].uType != 9030 || pSpriteObjects[uLayingItemID].spell_skill != 4 )
		  {
			  SpriteObject::OnInteraction(v153);
			  return 0;
		  }
		  pSpriteObjects[uLayingItemID]._46BEF1_apply_spells_aoe();
LABEL_233:
          if ( !v138 )
          {
            v109 = pObjectList->uNumObjects;
            ++pSpriteObjects[uLayingItemID].uType;
            v110 = 0;
            if ( (signed int)v109 <= 0 )
            {
LABEL_238:
              v112 = 0;
            }
            else
            {
              v111 = (char *)&pObjectList->pObjects->uObjectID;
              while ( pSpriteObjects[uLayingItemID].uType != *(short *)v111 )
              {
                ++v110;
                v111 += 56;
                if ( v110 >= (signed int)v109 )
                  goto LABEL_238;
              }
              v112 = v110;
            }
            pSpriteObjects[uLayingItemID].uObjectDescID = v112;
            if ( !v112 )
              SpriteObject::OnInteraction(v153);
            pSpriteObjects[uLayingItemID].vVelocity.z = 0;
            pSpriteObjects[uLayingItemID].vVelocity.y = 0;
            pSpriteObjects[uLayingItemID].vVelocity.x = 0;
            pSpriteObjects[uLayingItemID].uSpriteFrameID = 0;
            v113 = pSpriteObjects[uLayingItemID].uSoundID;
            if ( v113 )
              v114 = (signed __int16)v113 + 4;
            else
              v114 = 0;
            v115 = 8 * v153;
            LOBYTE(v115) = PID(OBJECT_Item,v153);
            v125 = v143 + 1;
            pAudioPlayer->PlaySound((SoundID)v125, v115, 0, -1, 0, v114, 0, 0);
            return 0;
          }
          SpriteObject::OnInteraction(v153);
          return 0;
        }
        v106 = v145;
        v150 = 0;
        v139 = PID_ID(v106);
        v137 = pSpriteObjects[uLayingItemID].spell_level;
        v152 = pSpriteObjects[uLayingItemID].spell_skill;
        v136 = pSpriteObjects[uLayingItemID].spell_id;
        if ( pSpriteObjects[uLayingItemID].uType == 9030 )
        {
          v150 = 2;
          if ( v152 == 2 )
          {
            v150 = 3;
          }
          else
          {
            if ( v152 >= 3 )
              v150 = 4;
          }
          BYTE2(pActors[v139].uAttributes) |= 8u;
        }
        if ( pSpriteObjects[uLayingItemID].uType == 6040 )
        {
          v135 = 7;
        }
        else
        {
          if ( pSpriteObjects[uLayingItemID].uType == 8030 )
          {
            v135 = 9;
          }
          else
          {
            if ( pSpriteObjects[uLayingItemID].uType != 9030 )
            {
              v107 = v136;
LABEL_222:
              if ( pSpriteObjects[uLayingItemID].uType != 9030 || v152 != 4 )
              {
                v108 = v139;
                if ( stru_50C198.GetMagicalResistance(&pActors[v139], v107) )
                {
                  v138 = 0;
                  if ( pSpriteObjects[uLayingItemID].uType == 8030 )
                  {
                    pActors[v108].uAIState = Standing;
                    pActors[v108].UpdateAnimation();
                  }
                  pActors[v108].pActorBuffs[v136].Apply(pParty->uTimePlayed + (signed int)(signed __int64)((double)(v137 << 7) * 0.033333335),
                    v152, v150, 0, 0);
                }
              }
              else
              {
                pSpriteObjects[uLayingItemID]._46BEF1_apply_spells_aoe();
              }
              pSpriteObjects[uLayingItemID].spell_level = 0;
              pSpriteObjects[uLayingItemID].spell_skill = 0;
              pSpriteObjects[uLayingItemID].spell_id = 0;
              goto LABEL_233;
            }
            v135 = 10;
          }
        }
        v107 = v135;
        goto LABEL_222;
      }
      if ( pSpriteObjects[uLayingItemID].uType == 8090 || pSpriteObjects[uLayingItemID].uType == 7030 || pSpriteObjects[uLayingItemID].uType == 7090 || pSpriteObjects[uLayingItemID].uType == 8000 )
        goto LABEL_93;
      if ( pSpriteObjects[uLayingItemID].uType == 8010 )
      {
        if ( v151 == 3
          && MonsterStats::BelongsToSupertype(pActors[PID_ID(v145)].pMonsterInfo.uID, MONSTER_SUPERTYPE_UNDEAD) )
          sub_43A97E(v153, v145);
        v93 = pObjectList->uNumObjects;
        ++pSpriteObjects[uLayingItemID].uType;
        v9 = 0;
        v52 = 0;
        if ( (signed int)v93 > 0 )
        {
          v94 = (char *)&pObjectList->pObjects->uObjectID;
          while ( pSpriteObjects[uLayingItemID].uType != *(short *)v94 )
          {
            ++v52;
            v94 += 56;
            if ( v52 >= (signed int)v93 )
              goto LABEL_181;
          }
          v95 = v52;
          pSpriteObjects[uLayingItemID].uObjectDescID = v95;
			if ( v95 == (short)v9 )
				SpriteObject::OnInteraction(v153);
			v96 = pSpriteObjects[uLayingItemID].uSoundID;
			pSpriteObjects[uLayingItemID].vVelocity.z = v9;
			pSpriteObjects[uLayingItemID].vVelocity.y = v9;
			pSpriteObjects[uLayingItemID].vVelocity.x = v9;
			pSpriteObjects[uLayingItemID].uSpriteFrameID = v9;
			if ( v96 == (short)v9 )
				v97 = 0;
			else
				v97 = (signed __int16)v96 + 4;
			v92 = v153;
            v124 = 8 * v92;
            LOBYTE(v124) = v124 | 2;
            v125 = word_4EE088_sound_ids[pSpriteObjects[uLayingItemID].spell_id - 1] + 1;
            pAudioPlayer->PlaySound((SoundID)v125, v124, v9, -1, v9, v97, v9, v9);
            return 0;
        }
        goto LABEL_181;
      }
      v79 = pSpriteObjects[uLayingItemID].uType == 8030;
    }
    else
    {
      if ( pSpriteObjects[uLayingItemID].uType == 6090 )
        goto LABEL_93;
      if ( pSpriteObjects[uLayingItemID].uType <= 4070 )
      {
        if ( pSpriteObjects[uLayingItemID].uType != 4070 )
        {
          v48 = pSpriteObjects[uLayingItemID].uType - 3090;
          if ( v48 )
          {
            v49 = v48 - 2;
            if ( v49 )
            {
              v50 = v49 - 908;
              if ( !v50 )
                goto LABEL_93;
              v45 = v50 - 30;
              v44 = v45 == 0;
              goto LABEL_91;
            }
            v54 = pSpriteObjects[uLayingItemID].uType - 1;
            v55 = pObjectList->uNumObjects;
            v9 = 0;
            v56 = 0;
            v44 = pObjectList->uNumObjects == 0;
            v20 = (pObjectList->uNumObjects & 0x80000000u) != 0;
            pSpriteObjects[uLayingItemID].uType = v54;
            if ( v20 | v44 )
            {
LABEL_102:
              v58 = 0;
            }
            else
            {
              v57 = (char *)&pObjectList->pObjects->uObjectID;
              while ( v54 != *(short *)v57 )
              {
                ++v56;
                v57 += 56;
                if ( v56 >= (signed int)v55 )
                  goto LABEL_102;
              }
              v58 = v56;
            }
            pSpriteObjects[uLayingItemID].uObjectDescID = v58;
            if ( !v58 )
              SpriteObject::OnInteraction(v153);
            pSpriteObjects[uLayingItemID].vVelocity.z = 0;
            pSpriteObjects[uLayingItemID].vVelocity.y = 0;
            pSpriteObjects[uLayingItemID].vVelocity.x = 0;
            pSpriteObjects[uLayingItemID].uSpriteFrameID = 0;
            sub_43A97E(v153, v145);
            if ( pSpriteObjects[uLayingItemID].uSoundID == (short)v9 )
              v16 = 0;
            else
              v16 = (signed __int16)pSpriteObjects[uLayingItemID].uSoundID + 4;
            v124 = 8 * v153;
            LOBYTE(v124) = v124 | 2;
            v125 = word_4EE088_sound_ids[pSpriteObjects[uLayingItemID].spell_id - 1] + 1;
            pAudioPlayer->PlaySound((SoundID)v125, v124, v9, -1, v9, v16, v9, v9);
            return 0;
          }
          v59 = pSpriteObjects[uLayingItemID].uType + 2;
          v60 = pObjectList->uNumObjects;
          v9 = 0;
          v61 = 0;
          v44 = pObjectList->uNumObjects == 0;
          v20 = (pObjectList->uNumObjects & 0x80000000u) != 0;
          pSpriteObjects[uLayingItemID].uType = v59;
          if ( v20 | v44 )
          {
LABEL_111:
            v63 = 0;
          }
          else
          {
            v62 = (char *)&pObjectList->pObjects->uObjectID;
            while ( v59 != *(short *)v62 )
            {
              ++v61;
              v62 += 56;
              if ( v61 >= (signed int)v60 )
                goto LABEL_111;
            }
            v63 = v61;
          }
          pSpriteObjects[uLayingItemID].uObjectDescID = v63;
          if ( !v63 )
            SpriteObject::OnInteraction(v153);
          v64 = pSpriteObjects[uLayingItemID].uFacing - stru_5C6E00->uIntegerDoublePi;
          v44 = pSpriteObjects[uLayingItemID].spell_skill == 4;
          pSpriteObjects[uLayingItemID].vVelocity.z = 0;
          pSpriteObjects[uLayingItemID].vVelocity.y = 0;
          pSpriteObjects[uLayingItemID].vVelocity.x = 0;
          v65 = 7;
          if ( v44 )
            v65 = 9;
          if ( v65 > 0 )
          {
            v141 = v65;
            do
            {
              v64 += (signed int)stru_5C6E00->uIntegerHalfPi >> 1;
              pSpriteObjects[uLayingItemID].Create(v64, 0, 1000, 0);
              --v141;
            }
            while ( v141 );
          }
          SpriteObject::OnInteraction(v153);
          if ( pSpriteObjects[uLayingItemID].uSoundID == (short)v9 )
            v16 = 0;
          else
            v16 = (signed __int16)pSpriteObjects[uLayingItemID].uSoundID + 4;
          v124 = 8 * v153;
          LOBYTE(v124) = v124 | 2;
          v125 = word_4EE088_sound_ids[pSpriteObjects[uLayingItemID].spell_id - 1] + 1;
          pAudioPlayer->PlaySound((SoundID)v125, v124, v9, -1, v9, v16, v9, v9);
          return 0;
        }
        if ( v151 == 6 || v151 == 5 || (v66 = 0, !v151) )
          return 1;
        v67 = pSpriteObjects[uLayingItemID].uType + 1;
        v68 = pObjectList->uNumObjects;
        v69 = 0;
        v44 = pObjectList->uNumObjects == 0;
        v20 = (pObjectList->uNumObjects & 0x80000000u) != 0;
        pSpriteObjects[uLayingItemID].uType = v67;
        if ( v20 | v44 )
        {
LABEL_128:
          v71 = 0;
        }
        else
        {
          v70 = (char *)&pObjectList->pObjects->uObjectID;
          while ( v67 != *(short *)v70 )
          {
            ++v69;
            v70 += 56;
            if ( v69 >= (signed int)v68 )
              goto LABEL_128;
          }
          v71 = v69;
        }
        pSpriteObjects[uLayingItemID].uObjectDescID = v71;
        if ( !v71 )
          SpriteObject::OnInteraction(v153);
        v134 = 0;
        v72 = v153;
        v132 = 0;
        pSpriteObjects[uLayingItemID].vVelocity.z = v66;
        pSpriteObjects[uLayingItemID].vVelocity.y = v66;
        pSpriteObjects[uLayingItemID].vVelocity.x = v66;
        pSpriteObjects[uLayingItemID].uSpriteFrameID = v66;
        stru_50FE08.Add(PID(OBJECT_Item,v72), 512, pSpriteObjects[uLayingItemID].vPosition.x, pSpriteObjects[uLayingItemID].vPosition.y, pSpriteObjects[uLayingItemID].vPosition.z, v132, v134);
        if ( pSpriteObjects[uLayingItemID].uSoundID == (short)v66 )
          v78 = 0;
        else
          v78 = (signed __int16)pSpriteObjects[uLayingItemID].uSoundID + 4;
        v125 = word_4EE088_sound_ids[pSpriteObjects[uLayingItemID].spell_id - 1] + 1;
        pAudioPlayer->PlaySound((SoundID)v125, pSpriteObjects[uLayingItemID].vPosition.x, v66, -1, v66, v78, v66, v66);
        return 0;
      }
      if ( pSpriteObjects[uLayingItemID].uType == 4090 )
      {
        v84 = pSpriteObjects[uLayingItemID].uType + 2;
        v85 = pObjectList->uNumObjects;
        v9 = 0;
        v86 = 0;
        v44 = pObjectList->uNumObjects == 0;
        v20 = (pObjectList->uNumObjects & 0x80000000u) != 0;
        pSpriteObjects[uLayingItemID].uType = v84;
        if ( v20 | v44 )
        {
LABEL_155:
          v88 = 0;
        }
        else
        {
          v87 = (char *)&pObjectList->pObjects->uObjectID;
          while ( v84 != *(short *)v87 )
          {
            ++v86;
            v87 += 56;
            if ( v86 >= (signed int)v85 )
              goto LABEL_155;
          }
          v88 = v86;
        }
        pSpriteObjects[uLayingItemID].uObjectDescID = v88;
        if ( !v88 )
          SpriteObject::OnInteraction(v153);
        v89 = pSpriteObjects[uLayingItemID].uFacing - stru_5C6E00->uIntegerDoublePi;
        pSpriteObjects[uLayingItemID].vVelocity.z = 0;
        pSpriteObjects[uLayingItemID].vVelocity.y = 0;
        pSpriteObjects[uLayingItemID].vVelocity.x = 0;
        v142 = v89;
        v148 = 7;
        do
        {
          pRnd->SetRange(-128, 128);
          v90 = pRnd->GetInRange();
          pRnd->SetRange(5, 500);
          v91 = pRnd->GetInRange();
          v142 += (signed int)stru_5C6E00->uIntegerHalfPi >> 1;
          pSpriteObjects[uLayingItemID].Create(v90 + v142, 0, v91, 0);
          --v148;
        }
        while ( v148 );
        SpriteObject::OnInteraction(v153);
        if ( pSpriteObjects[uLayingItemID].uSoundID == (short)v9 )
          v16 = 0;
        else
          v16 = (signed __int16)pSpriteObjects[uLayingItemID].uSoundID + 4;
        v124 = 8 * v153;
        LOBYTE(v124) = v124 | 2;
        v125 = word_4EE088_sound_ids[pSpriteObjects[uLayingItemID].spell_id-1] + 1;
        pAudioPlayer->PlaySound((SoundID)v125, v124, v9, -1, v9, v16, v9, v9);
        return 0;
      }
      if ( pSpriteObjects[uLayingItemID].uType == 4092 )
      {
        v80 = pObjectList->uNumObjects;
        v66 = 0;
        v81 = 0;
        v44 = pObjectList->uNumObjects == 0;
        v20 = (pObjectList->uNumObjects & 0x80000000u) != 0;
        pSpriteObjects[uLayingItemID].uType = 4091;
        if ( v20 | v44 )
        {
LABEL_144:
          v83 = 0;
        }
        else
        {
          v82 = (char *)&pObjectList->pObjects->uObjectID;
          while ( *(short *)v82 != 4091 )
          {
            ++v81;
            v82 += 56;
            if ( v81 >= (signed int)v80 )
              goto LABEL_144;
          }
          v83 = v81;
        }
        pSpriteObjects[uLayingItemID].uObjectDescID = v83;
        if ( !v83 )
          SpriteObject::OnInteraction(v153);
        v134 = 0;
        v72 = v153;
        v132 = pSpriteObjects[uLayingItemID].field_61;
        pSpriteObjects[uLayingItemID].vVelocity.z = v66;
        pSpriteObjects[uLayingItemID].vVelocity.y = v66;
        pSpriteObjects[uLayingItemID].vVelocity.x = v66;
        pSpriteObjects[uLayingItemID].uSpriteFrameID = v66;
        stru_50FE08.Add(PID(OBJECT_Item,v72), 512, pSpriteObjects[uLayingItemID].vPosition.x, pSpriteObjects[uLayingItemID].vPosition.y, pSpriteObjects[uLayingItemID].vPosition.z, v132, v134);
        if ( pSpriteObjects[uLayingItemID].uSoundID == (short)v66 )
          v78 = 0;
        else
          v78 = (signed __int16)pSpriteObjects[uLayingItemID].uSoundID + 4;
        v125 = word_4EE088_sound_ids[pSpriteObjects[uLayingItemID].spell_id - 1] + 1;
        pAudioPlayer->PlaySound((SoundID)v125, pSpriteObjects[uLayingItemID].vPosition.x, v66, -1, v66, v78, v66, v66);
        return 0;
      }
      if ( pSpriteObjects[uLayingItemID].uType == 4100 || pSpriteObjects[uLayingItemID].uType == 6010 )
        goto LABEL_93;
      v79 = pSpriteObjects[uLayingItemID].uType == 6040;
    }
    if ( !v79 )
      return 0;
    goto LABEL_172;
  }
  if ( pSpriteObjects[uLayingItemID].uType == 3060 )
    goto LABEL_93;
  if ( pSpriteObjects[uLayingItemID].uType <= 555 )
  {
    if ( pSpriteObjects[uLayingItemID].uType == 555 )
    {
      sub_43A97E(v153, v145);
      v23 = pObjectList->uNumObjects;
      ++pSpriteObjects[uLayingItemID].uType;
      v18 = 0;
      v24 = 0;
      if ( (signed int)v23 <= 0 )
      {
LABEL_41:
        v22 = 0;
      }
      else
      {
        v25 = (char *)&pObjectList->pObjects->uObjectID;
        while ( pSpriteObjects[uLayingItemID].uType != *(short *)v25 )
        {
          ++v24;
          v25 += 56;
          if ( v24 >= (signed int)v23 )
            goto LABEL_41;
        }
        v22 = v24;
      }
LABEL_42:
      pSpriteObjects[uLayingItemID].uObjectDescID = v22;
      if ( v22 == v18 )
        SpriteObject::OnInteraction(v153);
      pSpriteObjects[uLayingItemID].vVelocity.z = v18;
      pSpriteObjects[uLayingItemID].vVelocity.y = v18;
      pSpriteObjects[uLayingItemID].vVelocity.x = v18;
      pSpriteObjects[uLayingItemID].uSpriteFrameID = v18;
    }
    else
    {
      switch ( pSpriteObjects[uLayingItemID].uType )
      {
        case 500:
        case 505:
        case 510:
        case 515:
        case 520:
        case 525:
        case 530:
        case 535:
        case 540:
          sub_43A97E(v153, v145);
          v8 = pObjectList->uNumObjects;
          ++pSpriteObjects[uLayingItemID].uType;
          v9 = 0;
          v10 = 0;
          if ( (signed int)v8 <= 0 )
          {
            v12 = 0;
            goto LABEL_24;
          }
          v11 = (char *)&pObjectList->pObjects->uObjectID;
          break;
        case 545:
        case 550:
          if ( pSpriteObjects[uLayingItemID].stru_24.uItemID != 405 && pSpriteObjects[uLayingItemID].stru_24.uSpecEnchantmentType != 3 )
          {
            pSpriteObjects[uLayingItemID].vVelocity.z = 0;
            pSpriteObjects[uLayingItemID].vVelocity.y = 0;
            pSpriteObjects[uLayingItemID].vVelocity.x = 0;
            pSpriteObjects[uLayingItemID].uSpriteFrameID = 0;
            sub_43A97E(v153, v145);
            SpriteObject::OnInteraction(v153);
            if ( pSpriteObjects[uLayingItemID].uSoundID == 0 )
              v16 = 0;
            else
              v16 = (signed __int16)pSpriteObjects[uLayingItemID].uSoundID + 4;
            v124 = 8 * v153;
            LOBYTE(v124) = v124 | 2;
            v125 = word_4EE088_sound_ids[pSpriteObjects[uLayingItemID].spell_id - 1] + 1;
            pAudioPlayer->PlaySound((SoundID)v125, v124, 0, -1, 0, v16, 0, 0);
            return 0;
          }
          v17 = pObjectList->uNumObjects;
          v18 = 0;
          v19 = 0;
          v44 = pObjectList->uNumObjects == 0;
          v20 = (pObjectList->uNumObjects & 0x80000000u) != 0;
          pSpriteObjects[uLayingItemID].uType = 600;
          if ( v20 | v44 )
            goto LABEL_41;
          v21 = (char *)&pObjectList->pObjects->uObjectID;
          while ( *(short *)v21 != 600 )
          {
            ++v19;
            v21 += 56;
            if ( v19 >= (signed int)v17 )
              goto LABEL_41;
          }
          v22 = v19;
          goto LABEL_42;
        default:
          return 0;
      }
      while ( pSpriteObjects[uLayingItemID].uType != *(short *)v11 )
      {
        ++v10;
        v11 += 56;
        if ( v10 >= (signed int)v8 )
        {
          v12 = 0;
          goto LABEL_24;
        }
      }
      v12 = v10;
LABEL_24:
      pSpriteObjects[uLayingItemID].uObjectDescID = v12;
      if ( !v12 )
        SpriteObject::OnInteraction(v153);
      v44 = pSpriteObjects[uLayingItemID].uType == 555;
      pSpriteObjects[uLayingItemID].vVelocity.z = 0;
      pSpriteObjects[uLayingItemID].vVelocity.y = 0;
      pSpriteObjects[uLayingItemID].vVelocity.x = 0;
      pSpriteObjects[uLayingItemID].uSpriteFrameID = 0;
      if ( !v44 )
      {
        v13 = 8 * v153;
        LOBYTE(v13) = PID(OBJECT_Item,v153);
        pAudioPlayer->PlaySound(SOUND_8, v13, v9, -1, v9, v9, v9, v9);
        return 0;
      }
    }
    return 0;
  }
  if ( pSpriteObjects[uLayingItemID].uType > 1100 )
  {
    v41 = pSpriteObjects[uLayingItemID].uType - 2030;
    if ( !v41 )
      goto LABEL_247;
    v42 = v41 - 30;
    if ( v42 )
    {
      v43 = v42 - 40;
      if ( !v43 )
        goto LABEL_59;
      v45 = v43 - 910;
      v44 = v45 == 0;
LABEL_91:
      if ( !v44 && v45 != 20 )
        return 0;
    }
LABEL_93:
    sub_43A97E(v153, v145);
    v51 = pObjectList->uNumObjects;
    ++pSpriteObjects[uLayingItemID].uType;
    v9 = 0;
    v52 = 0;
    if ( (signed int)v51 > 0 )
    {
      v53 = (char *)&pObjectList->pObjects->uObjectID;
      while ( pSpriteObjects[uLayingItemID].uType != *(short *)v53 )
      {
        ++v52;
        v53 += 56;
        if ( v52 >= (signed int)v51 )
          goto LABEL_181;
      }
      v95 = v52;
    }
	else
	{
LABEL_181:
    v95 = 0;
	}
    pSpriteObjects[uLayingItemID].uObjectDescID = v95;
    if ( v95 == (short)v9 )
      SpriteObject::OnInteraction(v153);
    v96 = pSpriteObjects[uLayingItemID].uSoundID;
    pSpriteObjects[uLayingItemID].vVelocity.z = v9;
    pSpriteObjects[uLayingItemID].vVelocity.y = v9;
    pSpriteObjects[uLayingItemID].vVelocity.x = v9;
    pSpriteObjects[uLayingItemID].uSpriteFrameID = v9;
    if ( v96 == (short)v9 )
      v97 = 0;
    else
      v97 = (signed __int16)v96 + 4;
    v92 = v153;
    v124 = 8 * v92;
    LOBYTE(v124) = v124 | 2;
    v125 = word_4EE088_sound_ids[pSpriteObjects[uLayingItemID].spell_id - 1] + 1;
    pAudioPlayer->PlaySound((SoundID)v125, v124, v9, -1, v9, v97, v9, v9);
    return 0;
  }
  if ( pSpriteObjects[uLayingItemID].uType == 1100 )
    goto LABEL_93;
  v26 = pSpriteObjects[uLayingItemID].uType - 600;
  if ( !v26 )
  {
    v33 = pObjectList->uNumObjects;
    v34 = 0;
    v44 = pObjectList->uNumObjects == 0;
    v20 = (pObjectList->uNumObjects & 0x80000000u) != 0;
    pSpriteObjects[uLayingItemID].uType = 601;
    if ( v20 | v44 )
    {
LABEL_69:
      v36 = 0;
    }
    else
    {
      v35 = (char *)&pObjectList->pObjects->uObjectID;
      while ( *(short *)v35 != 601 )
      {
        ++v34;
        v35 += 56;
        if ( v34 >= (signed int)v33 )
          goto LABEL_69;
      }
      v36 = v34;
    }
    pSpriteObjects[uLayingItemID].uObjectDescID = v36;
    if ( !v36 )
      SpriteObject::OnInteraction(v153);
    v37 = pSpriteObjects[uLayingItemID].vPosition.z;
    pSpriteObjects[uLayingItemID].vVelocity.z = 0;
    v38 = 8 * v153;
    v39 = pSpriteObjects[uLayingItemID].vPosition.y;
    LOBYTE(v38) = PID(OBJECT_Item,v153);
    pSpriteObjects[uLayingItemID].vVelocity.y = 0;
    pSpriteObjects[uLayingItemID].vVelocity.x = 0;
    v40 = pSpriteObjects[uLayingItemID].vPosition.x;
    pSpriteObjects[uLayingItemID].uSpriteFrameID = 0;
    v147 = v38;
    stru_50FE08.Add(v38, 512, v40, v39, v37, 0, 0);
    if (v3->uFlags & OBJECT_DESC_TRIAL_PARTICLE)
      trail_particle_generator.GenerateTrailParticles(pSpriteObjects[uLayingItemID].vPosition.x, pSpriteObjects[uLayingItemID].vPosition.y, pSpriteObjects[uLayingItemID].vPosition.z, v3->uParticleTrailColor);
    pAudioPlayer->PlaySound(SOUND_8, v147, 0, -1, 0, 0, 0, 0);
    return 0;
  }
  v27 = v26 - 410;
  if ( !v27 )
    goto LABEL_93;
  v28 = v27 - 40;
  if ( !v28 )
    goto LABEL_191;
  v29 = v28 - 10;
  if ( v29 )
  {
    if ( v29 != 20 )
      return 0;
LABEL_59:
    if ( v151 != 3 )
    {
      v30 = pSpriteObjects[uLayingItemID].uType + 1;
      v31 = pObjectList->uNumObjects;
      v32 = 0;
      pSpriteObjects[uLayingItemID].uType = v30;
      v146 = 0;
      if ( (signed int)v31 > 0 )
      {
        v140 = (char *)&pObjectList->pObjects->uObjectID;
        while ( v30 != *(short *)v140 )
        {
          ++v146;
          v140 += 56;
          if ( v146 >= (signed int)v31 )
          {
            v46 = 0;
            pSpriteObjects[uLayingItemID].uObjectDescID = v46;
            if ( v46 == (short)v32 )
              SpriteObject::OnInteraction(v153);
            v100 = pSpriteObjects[uLayingItemID].field_61;
            pSpriteObjects[uLayingItemID].uSpriteFrameID = v32;
            v102 = 8 * v153;
            LOBYTE(v102) = PID(OBJECT_Item,v153);
            pSpriteObjects[uLayingItemID].vVelocity.x = v32;
            pSpriteObjects[uLayingItemID].vVelocity.y = v32;
            pSpriteObjects[uLayingItemID].vVelocity.z = v32;
            stru_50FE08.Add(v102, 512, pSpriteObjects[uLayingItemID].vPosition.x, pSpriteObjects[uLayingItemID].vPosition.y, pSpriteObjects[uLayingItemID].vPosition.z, v100, v32);
            if (v3->uFlags & OBJECT_DESC_TRIAL_PARTICLE)
              trail_particle_generator.GenerateTrailParticles(pSpriteObjects[uLayingItemID].vPosition.x, pSpriteObjects[uLayingItemID].vPosition.y, pSpriteObjects[uLayingItemID].vPosition.z, v3->uParticleTrailColor);
            if ( pSpriteObjects[uLayingItemID].uSoundID == (short)v32 )
              v47 = 0;
            else
              v47 = (signed __int16)pSpriteObjects[uLayingItemID].uSoundID + 4;
            v125 = word_4EE088_sound_ids[pSpriteObjects[uLayingItemID].spell_id - 1] + 1;
            pAudioPlayer->PlaySound((SoundID)v125, v102, v32, -1, v32, v47, v32, v32);
            return 0;
          }
        }
        v46 = v146;
        pSpriteObjects[uLayingItemID].uObjectDescID = v46;
        if ( v46 == (short)v32 )
          SpriteObject::OnInteraction(v153);
        v100 = pSpriteObjects[uLayingItemID].field_61;
        pSpriteObjects[uLayingItemID].uSpriteFrameID = v32;
        v102 = 8 * v153;
        LOBYTE(v102) = PID(OBJECT_Item,v153);
        pSpriteObjects[uLayingItemID].vVelocity.x = v32;
        pSpriteObjects[uLayingItemID].vVelocity.y = v32;
        pSpriteObjects[uLayingItemID].vVelocity.z = v32;
        stru_50FE08.Add(v102, 512, pSpriteObjects[uLayingItemID].vPosition.x, pSpriteObjects[uLayingItemID].vPosition.y, pSpriteObjects[uLayingItemID].vPosition.z, v100, v32);
        if (v3->uFlags & OBJECT_DESC_TRIAL_PARTICLE)
          trail_particle_generator.GenerateTrailParticles(pSpriteObjects[uLayingItemID].vPosition.x, pSpriteObjects[uLayingItemID].vPosition.y, pSpriteObjects[uLayingItemID].vPosition.z, v3->uParticleTrailColor);
        if ( pSpriteObjects[uLayingItemID].uSoundID == (short)v32 )
          v47 = 0;
        else
          v47 = (signed __int16)pSpriteObjects[uLayingItemID].uSoundID + 4;
        v125 = word_4EE088_sound_ids[pSpriteObjects[uLayingItemID].spell_id - 1] + 1;
        pAudioPlayer->PlaySound((SoundID)v125, v102, v32, -1, v32, v47, v32, v32);
        return 0;
      }
      v46 = 0;
      pSpriteObjects[uLayingItemID].uObjectDescID = v46;
      if ( v46 == (short)v32 )
        SpriteObject::OnInteraction(v153);
      v100 = pSpriteObjects[uLayingItemID].field_61;
      pSpriteObjects[uLayingItemID].uSpriteFrameID = v32;
      v102 = 8 * v153;
      LOBYTE(v102) = PID(OBJECT_Item,v153);
      pSpriteObjects[uLayingItemID].vVelocity.x = v32;
      pSpriteObjects[uLayingItemID].vVelocity.y = v32;
      pSpriteObjects[uLayingItemID].vVelocity.z = v32;
      stru_50FE08.Add(v102, 512, pSpriteObjects[uLayingItemID].vPosition.x, pSpriteObjects[uLayingItemID].vPosition.y, pSpriteObjects[uLayingItemID].vPosition.z, v100, v32);
      if (v3->uFlags & OBJECT_DESC_TRIAL_PARTICLE)
        trail_particle_generator.GenerateTrailParticles(pSpriteObjects[uLayingItemID].vPosition.x, pSpriteObjects[uLayingItemID].vPosition.y, pSpriteObjects[uLayingItemID].vPosition.z, v3->uParticleTrailColor);
      if ( pSpriteObjects[uLayingItemID].uSoundID == (short)v32 )
        v47 = 0;
      else
        v47 = (signed __int16)pSpriteObjects[uLayingItemID].uSoundID + 4;
      v125 = word_4EE088_sound_ids[pSpriteObjects[uLayingItemID].spell_id - 1] + 1;
      pAudioPlayer->PlaySound((SoundID)v125, v102, v32, -1, v32, v47, v32, v32);
      return 0;
    }
    return 1;
  }
LABEL_247:
  if ( v151 == 6 || v151 == 5 || (v9 = 0, !v151) )
    return 1;
  if ( v151 != 2 )
  {
    sub_43A97E(v153, v145);
    v122 = pObjectList->uNumObjects;
    ++pSpriteObjects[uLayingItemID].uType;
    v52 = 0;
    if ( (signed int)v122 > 0 )
    {
      v123 = (char *)&pObjectList->pObjects->uObjectID;
      while ( pSpriteObjects[uLayingItemID].uType != *(short *)v123 )
      {
        ++v52;
        v123 += 56;
        if ( v52 >= (signed int)v122 )
          goto LABEL_181;
      }
      v95 = v52;
      pSpriteObjects[uLayingItemID].uObjectDescID = v95;
      if ( v95 == (short)v9 )
        SpriteObject::OnInteraction(v153);
      v96 = pSpriteObjects[uLayingItemID].uSoundID;
      pSpriteObjects[uLayingItemID].vVelocity.z = v9;
      pSpriteObjects[uLayingItemID].vVelocity.y = v9;
      pSpriteObjects[uLayingItemID].vVelocity.x = v9;
      pSpriteObjects[uLayingItemID].uSpriteFrameID = v9;
      if ( v96 == (short)v9 )
        v97 = 0;
      else
        v97 = (signed __int16)v96 + 4;
      v92 = v153;
      v124 = 8 * v92;
      LOBYTE(v124) = v124 | 2;
      v125 = word_4EE088_sound_ids[pSpriteObjects[uLayingItemID].spell_id - 1] + 1;
      pAudioPlayer->PlaySound((SoundID)v125, v124, v9, -1, v9, v97, v9, v9);
      return 0;
    }
    goto LABEL_181;
  }
  v117 = pSpriteObjects[uLayingItemID].uType + 1;
  v118 = pObjectList->uNumObjects;
  v119 = 0;
  v44 = pObjectList->uNumObjects == 0;
  v20 = (pObjectList->uNumObjects & 0x80000000u) != 0;
  pSpriteObjects[uLayingItemID].uType = v117;
  if ( v20 | v44 )
  {
    v121 = 0;
  }
  else
  {
    v120 = (char *)&pObjectList->pObjects->uObjectID;
    while ( v117 != *(short *)v120 )
    {
      ++v119;
      v120 += 56;
      if ( v119 >= (signed int)v118 )
      {
        v121 = 0;
        pSpriteObjects[uLayingItemID].uObjectDescID = v121;
        if ( !v121 )
          SpriteObject::OnInteraction(v153);
        pSpriteObjects[uLayingItemID].vVelocity.z = 0;
        pSpriteObjects[uLayingItemID].vVelocity.y = 0;
        pSpriteObjects[uLayingItemID].vVelocity.x = 0;
        pSpriteObjects[uLayingItemID].uSpriteFrameID = 0;
        v13 = 8 * v153;
        LOBYTE(v13) = PID(OBJECT_Item,v153);
        pAudioPlayer->PlaySound(SOUND_8, v13, v9, -1, v9, v9, v9, v9);
        return 0;
      }
    }
    v121 = v119;
  }
  pSpriteObjects[uLayingItemID].uObjectDescID = v121;
  if ( !v121 )
    SpriteObject::OnInteraction(v153);
  pSpriteObjects[uLayingItemID].vVelocity.z = 0;
  pSpriteObjects[uLayingItemID].vVelocity.y = 0;
  pSpriteObjects[uLayingItemID].vVelocity.x = 0;
  pSpriteObjects[uLayingItemID].uSpriteFrameID = 0;
  v13 = 8 * v153;
  LOBYTE(v13) = PID(OBJECT_Item,v153);
  pAudioPlayer->PlaySound(SOUND_8, v13, v9, -1, v9, v9, v9, v9);
  return 0;
}
