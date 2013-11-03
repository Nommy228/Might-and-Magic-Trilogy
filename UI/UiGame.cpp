#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "..\Texture.h"
#include "..\MM7.h"

#include "..\Mouse.h"
#include "..\Keyboard.h"
#include "..\mm7_data.h"

#include "..\Vis.h"
#include "..\MapInfo.h"
#include "..\Game.h"
#include "..\GUIWindow.h"
#include "..\GUIFont.h"
#include "..\Party.h"
#include "..\AudioPlayer.h"
#include "..\Outdoor.h"
#include "..\LOD.h"
#include "..\Actor.h"
#include "..\Viewport.h"
#include "..\SpriteObject.h"
#include "..\ObjectList.h"
#include "..\DecorationList.h"
#include "..\PlayerFrameTable.h"
#include "..\stru123.h"
#include "..\Time.h"
#include "..\IconFrameTable.h"
#include "..\TurnEngine.h"
#include "..\texts.h"
#include "UIHouses.h"
#include "..\BSPModel.h"
#include "..\Math.h"
#include "..\Level/Decoration.h"


int uTextureID_GameUI_CharSelectionFrame; // 50C98C

//----- (00421D00) --------------------------------------------------------
void __fastcall GameUI_OnPlayerPortraitLeftClick(unsigned int uPlayerID)
{
  Player* player = &pParty->pPlayers[uPlayerID - 1];
  if (pParty->pPickedItem.uItemID)
  {
    if (int slot = player->AddItem(-1, pParty->pPickedItem.uItemID))
    {
      memcpy(&player->pInventoryItemList[slot-1], &pParty->pPickedItem, 0x24u);
      viewparams->bRedrawGameUI = true;
      pMouse->RemoveHoldingItem();
      return;
    }

    if (!player->CanAct())
    {
      player = pPlayers[uActiveCharacter];
    }
    if( player->CanAct() || !pPlayers[uActiveCharacter]->CanAct() )
      player->PlaySound(SPEECH_NoRoom, 0);
  }

  if (pCurrentScreen == SCREEN_GAME)
  {
    viewparams->bRedrawGameUI = true;
    if ( uActiveCharacter != uPlayerID )
    {
      if ( pPlayers[uPlayerID]->uTimeToRecovery )
        return;

      uActiveCharacter = uPlayerID;
      return;
    }
    pGUIWindow_CurrentMenu = CharacterUI_Initialize(7);
    return;
  }
  if ( pCurrentScreen == SCREEN_SPELL_BOOK )
    return;
  if ( pCurrentScreen == SCREEN_CHEST )
  {
    viewparams->bRedrawGameUI = true;
    if ( uActiveCharacter == uPlayerID )
    {
      pWindowList_at_506F50_minus1_indexing_buttons____and_an_int_[0] = 103;
      pCurrentScreen = SCREEN_CHEST_INVENTORY;
      uActiveCharacter = uPlayerID;
      return;
    }
    if ( pPlayers[uPlayerID]->uTimeToRecovery )
      return;
    uActiveCharacter = uPlayerID;
    return;
  }
  if ( pCurrentScreen != SCREEN_HOUSE )
  {
    if ( pCurrentScreen == SCREEN_E )
    {
      uActiveCharacter = uPlayerID;
      return;
    }
    if ( pCurrentScreen != SCREEN_CHEST_INVENTORY )
    {
      viewparams->bRedrawGameUI = true;
      uActiveCharacter = uPlayerID;
      if ( pWindowList_at_506F50_minus1_indexing_buttons____and_an_int_[0] == 102 )
        FillAwardsData();
      return;
    }
    viewparams->bRedrawGameUI = true;
    if ( uActiveCharacter == uPlayerID )
    {
      pWindowList_at_506F50_minus1_indexing_buttons____and_an_int_[0] = 103;
      pCurrentScreen = SCREEN_CHEST_INVENTORY;
      uActiveCharacter = uPlayerID;
      return;
    }
    if ( pPlayers[uPlayerID]->uTimeToRecovery )
      return;
    uActiveCharacter = uPlayerID;
    return;
  }
  if ( window_SpeakInHouse->receives_keyboard_input_2 == WINDOW_INPUT_IN_PROGRESS)
    return;
  viewparams->bRedrawGameUI = true;
  if ( uActiveCharacter != uPlayerID )
    uActiveCharacter = uPlayerID;
    return;
  if (dialog_menu_id == HOUSE_DIALOGUE_SHOP_BUY_STANDARD || dialog_menu_id == HOUSE_DIALOGUE_SHOP_6)
  {
    __debugbreak(); // fix indexing
    pWindowList_at_506F50_minus1_indexing_buttons____and_an_int_[0] = 103;
    pGUIWindow_CurrentMenu = CharacterUI_Initialize(14);
    return;
  }
}
// 4E28F8: using guessed type int pCurrentScreen;
// F8B19C: using guessed type int dword_F8B19C;

//----- (00416B01) --------------------------------------------------------
void GameUI_DrawNPCPopup(void *_this)//PopupWindowForBenefitAndJoinText
{
  int v1; // edi@2
  NPCData *pNPC; // eax@16
  const CHAR *pText; // eax@18
  char *v11; // esi@26
  GUIWindow popup_window; // [sp+Ch] [bp-60h]@23
  int a2; // [sp+60h] [bp-Ch]@16
  LPCSTR lpsz; // [sp+68h] [bp-4h]@6

  if ( bNoNPCHiring != 1 )
  {
    v1 = 0;
    /*do
    {
      if ( v3->pName )
        pTmpBuf[v1++] = v2;
      ++v3;
      ++v2;
    }
    while ( (signed int)v3 < (signed int)&pParty->pPickedItem );*/
    for (int i = 0; i < 2; ++i)
    {
     if (pParty->pHirelings[i].pName)
        pTmpBuf[v1++] = i;
    }
    lpsz = 0;
    if ( (signed int)pNPCStats->uNumNewNPCs > 0 )
    {
      /*v4 = pNPCStats->pNewNPCData;
      do
      {
        if ( v4->uFlags & 0x80
          && (!pParty->pHirelings[0].pName || strcmp(v4->pName, pParty->pHirelings[0].pName))
          && (!pParty->pHirelings[1].pName || strcmp(v4->pName, pParty->pHirelings[1].pName)) )
          pTmpBuf[v1++] = (char)lpsz + 2;
        ++lpsz;
        ++v4;
      }
      while ( (signed int)lpsz < (signed int)pNPCStats->uNumNewNPCs );*/
      for ( uint i = 0; i < pNPCStats->uNumNewNPCs; ++i )
      {
        if (pNPCStats->pNewNPCData[i].Hired())
        {
          if (!pParty->pHirelings[0].pName || strcmp((char *)pNPCStats->pNewNPCData[i].pName, (char *)pParty->pHirelings[0].pName))
          {
            if (!pParty->pHirelings[1].pName || strcmp((char *)pNPCStats->pNewNPCData[i].pName, (char *)pParty->pHirelings[1].pName))
              pTmpBuf[v1++] = i + 2;
          }
        }
      }
    }
    if ( (signed int)((char *)_this + pParty->hirelingScrollPosition) < v1 )
    {
      sDialogue_SpeakingActorNPC_ID = -1 - pParty->hirelingScrollPosition - (int)_this;
      pNPC = GetNewNPCData(sDialogue_SpeakingActorNPC_ID, &a2);
      if ( pNPC )
      {
        if ( a2 == 57 )
          pText = pNPCTopics[512].pText; // Baby dragon
        else
          pText = (const CHAR *)pNPCStats->pProfessions[pNPC->uProfession].pBenefits;
        lpsz = pText;
        if ( !pText )
        {
          lpsz = (LPCSTR)pNPCStats->pProfessions[pNPC->uProfession].pJoinText;
          if ( !lpsz )
            lpsz = "";
        }
        popup_window.Hint = 0;
        popup_window.uFrameX = 38;
        popup_window.uFrameY = 60;
        popup_window.uFrameWidth = 276;
        popup_window.uFrameZ = 313;
        popup_window.uFrameHeight = pFontArrus->CalcTextHeight(lpsz, &popup_window, 0, 0) + 2 * LOBYTE(pFontArrus->uFontHeight) + 24;
        if ( (signed int)popup_window.uFrameHeight < 130 )
          popup_window.uFrameHeight = 130;
        popup_window.uFrameWidth = 400;
        popup_window.uFrameZ = popup_window.uFrameX + 399;
        popup_window.DrawMessageBox(0);
        sprintfex(pTmpBuf2.data(), "NPC%03d", pNPC->uPortraitID);
        pRenderer->DrawTextureIndexed(popup_window.uFrameX + 22, popup_window.uFrameY + 36,
            (Texture *)(pIcons_LOD->LoadTexture(pTmpBuf2.data(), TEXTURE_16BIT_PALETTE) != -1
            ? &pIcons_LOD->pTextures[pIcons_LOD->LoadTexture(pTmpBuf2.data(), TEXTURE_16BIT_PALETTE)] : 0));
        if ( pNPC->uProfession )
        {
          v11 = pTmpBuf.data();
          sprintfex(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[429], pNPC->pName, aNPCProfessionNames[pNPC->uProfession]);
        }
        else
        {
          v11 = pTmpBuf.data();
          strcpy(pTmpBuf.data(), pNPC->pName);
        }
        popup_window.DrawTitleText(pFontArrus, 0, 12, TargetColor(0xFFu, 0xFFu, 0x9Bu), v11, 3);
        popup_window.uFrameWidth -= 24;
        popup_window.uFrameZ = popup_window.uFrameX + popup_window.uFrameWidth - 1;
        popup_window.DrawText(pFontArrus, 100, 36, 0, BuildDialogueString((char *)lpsz, uActiveCharacter - 1, 0, 0, 0, 0), 0, 0, 0);
      }
    }
  }
}

//----- (00445D4A) --------------------------------------------------------
void GameUI_InitializeDialogue(Actor *actor, int bPlayerSaysHello)
{
  NPCData *pNPCInfo; // ebp@1
  int v9; // esi@8
  int pNumberContacts; // eax@11
  char pContainer[32]; // [sp+14h] [bp-28h]@3

  dword_A74CDC = -1;
  dword_AE336C = -1;
  pEventTimer->Pause();
  pMiscTimer->Pause();
  pAudioPlayer->StopChannels(-1, -1);
  uDialogueType = 0;
  sDialogue_SpeakingActorNPC_ID = actor->sNPC_ID;
  pDialogue_SpeakingActor = actor;
  pNPCInfo = GetNPCData(actor->sNPC_ID);
  if ( (pNPCInfo->uFlags & 3) != 2 )
    pNPCInfo->uFlags = pNPCInfo->uFlags + 1;

  switch (pParty->alignment)
  {
    case PartyAlignment_Good:    sprintfex(pContainer, "evt%02d-b", const_2()); break;
    case PartyAlignment_Neutral: sprintfex(pContainer, "evt%02d", const_2());   break;
    case PartyAlignment_Evil:    sprintfex(pContainer, "evt%02d-c", const_2()); break;
  }

  pDialogueNPCCount = 0;
  uNumDialogueNPCPortraits = 1;
  pTexture_Dialogue_Background = pIcons_LOD->LoadTexturePtr(pContainer, TEXTURE_16BIT_PALETTE);
  sprintfex(pContainer, "npc%03u", pNPCInfo->uPortraitID);
  v9 = 0;
  pDialogueNPCPortraits[0] = pIcons_LOD->LoadTexturePtr(pContainer, TEXTURE_16BIT_PALETTE);
  dword_591084 = areWeLoadingTexture;
  uTextureID_right_panel_loop = uTextureID_right_panel;
  if ( !pNPCInfo->Hired() && pNPCInfo->Location2D >= 0 )
  {
    if ( (signed int)pParty->GetPartyFame() <= pNPCInfo->fame
      || (pNumberContacts = pNPCInfo->uFlags & 0xFFFFFF7F, (pNumberContacts & 0x80000000u) != 0) )
    {
      v9 = 1;
    }
	else
	{
		if ( pNumberContacts > 1 )
		{
		  if ( pNumberContacts == 2 )
		  {
			v9 = 3;
		  }
		  else
		  {
			  if ( pNumberContacts != 3 )
			  {
				if ( pNumberContacts != 4 )
				  v9 = 1;
			  }
			  else
			  {
				v9 = 2;
			  }
		  }
		}
		else if ( pNPCInfo->rep )
		{
		  v9 = 2;
		}
	}
  }
  if (sDialogue_SpeakingActorNPC_ID < 0)
    v9 = 4;
  pDialogueWindow = GUIWindow::Create(0, 0, 640, 480, WINDOW_Dialogue, v9, 0);//pNumberContacts = 1, v9 = 0; pNumberContacts = 2, v9 = 3;
  if (pNPCInfo->Hired())
  {
    if ( !pNPCInfo->bHasUsedTheAbility )
    {
      if ( pNPCInfo->uProfession >= 10 )
      {
        if ( pNPCInfo->uProfession <= 12 || pNPCInfo->uProfession > 32 && (pNPCInfo->uProfession <= 34 
             || pNPCInfo->uProfession > 38 && (pNPCInfo->uProfession <= 43 || pNPCInfo->uProfession == 52)) )
        {
          pDialogueWindow->CreateButton(480, 250, 140, LOBYTE(pFontArrus->uFontHeight) - 3, 1, 0, UIMSG_SelectNPCDialogueOption, 9, 0, "", 0);
          pDialogueWindow->_41D08F_set_keyboard_control_group(4, 1, 0, 1);
        }
      }
    }
  }

  pDialogueWindow->CreateButton( 61, 424, 31, 40, 2, 94, UIMSG_SelectCharacter, 1, '1', "", 0);
  pDialogueWindow->CreateButton(177, 424, 31, 40, 2, 94, UIMSG_SelectCharacter, 2, '2', "", 0);
  pDialogueWindow->CreateButton(292, 424, 31, 40, 2, 94, UIMSG_SelectCharacter, 3, '3', "", 0);
  pDialogueWindow->CreateButton(407, 424, 31, 40, 2, 94, UIMSG_SelectCharacter, 4, '4', "", 0);

  if (bPlayerSaysHello && uActiveCharacter && !pNPCInfo->Hired())
  {
    if (pParty->uCurrentHour < 5 || pParty->uCurrentHour > 21)
      pPlayers[uActiveCharacter]->PlaySound(SPEECH_GoodEvening, 0);
    else
      pPlayers[uActiveCharacter]->PlaySound(SPEECH_GoodDay, 0);
  }
}

//----- (00445350) --------------------------------------------------------
void GameUI_DrawDialogue()
{
  NPCData *pNPC; // ebx@2
  int pGreetType; // eax@2
  int pTextHeight; // esi@39
  GUIButton *pButton; // eax@43
  int all_text_height; // ebx@93
  signed int index; // esi@99
  int v42; // edi@102
  int v45;
  unsigned __int16 pTextColor; // ax@104
  GUIWindow window; // [sp+ACh] [bp-68h]@42
  GUIFont *pOutString; // [sp+10Ch] [bp-8h]@39
  const char *pInString=NULL; // [sp+110h] [bp-4h]@32

  if ( !pDialogueWindow )
    return;

  // Window title(Заголовок окна)----
  memcpy(&window, pDialogueWindow, sizeof(window));
  pNPC = GetNPCData(sDialogue_SpeakingActorNPC_ID);
  pGreetType = GetGreetType(sDialogue_SpeakingActorNPC_ID);
  window.uFrameWidth -= 10;
  window.uFrameZ -= 10;
  pRenderer->DrawTextureIndexed(477, 0, pTexture_Dialogue_Background);
  pRenderer->DrawTextureTransparent(468, 0, (Texture *)(uTextureID_right_panel_loop != -1 ? &pIcons_LOD->pTextures[uTextureID_right_panel_loop] : 0));
  pRenderer->DrawTextureIndexed(pNPCPortraits_x[0][0] - 4, pNPCPortraits_y[0][0] - 4, (Texture *)(uTextureID_50795C != -1 ? &pIcons_LOD->pTextures[uTextureID_50795C] : 0));
  pRenderer->DrawTextureIndexed(pNPCPortraits_x[0][0], pNPCPortraits_y[0][0], pDialogueNPCPortraits[0]);

  if (pNPC->uProfession)
  {
    assert(pNPC->uProfession < sizeof(aNPCProfessionNames) / sizeof(*aNPCProfessionNames.data())); // sometimes buffer overflows; errors emerge both here and in dialogue text
    sprintfex(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[429], pNPC->pName, aNPCProfessionNames[pNPC->uProfession]);//^Pi[%s] %s
  }
  else if (pNPC->pName)
    strcpy(pTmpBuf.data(), pNPC->pName);

  window.DrawTitleText(pFontArrus, 483, 112, ui_game_dialogue_npc_name_color, pTmpBuf.data(), 3);
  pParty->GetPartyFame();

  pInString = nullptr;
  switch (uDialogueType)
  {
    case DIALOGUE_13:
      pInString = BuildDialogueString(pNPCStats->pProfessions[pNPC->uProfession].pJoinText, uActiveCharacter - 1, 0, 0, 0, 0);
    break;

    case DIALOGUE_PROFESSION_DETAILS:
    {
      //auto prof = pNPCStats->pProfessions[pNPC->uProfession];

      if (dialogue_show_profession_details)
        pInString = BuildDialogueString(pNPCStats->pProfessions[pNPC->uProfession].pBenefits, uActiveCharacter - 1, 0, 0, 0, 0);
      else if (pNPC->Hired())
        pInString = BuildDialogueString(pNPCStats->pProfessions[pNPC->uProfession].pDismissText, uActiveCharacter - 1, 0, 0, 0, 0);
      else
        pInString = BuildDialogueString(pNPCStats->pProfessions[pNPC->uProfession].pJoinText, uActiveCharacter - 1, 0, 0, 0, 0);
    }
    break;


    case DIALOGUE_ARENA_WELCOME:
      pInString = pGlobalTXT_LocalizationStrings[574]; // "Welcome to the Arena of Life and Death.  Remember, you are only allowed one arena combat per visit.  To fight an arena battle, select the option that best describes your abilities and return to me- if you survive:"
    break;

    case DIALOGUE_ARENA_FIGHT_NOT_OVER_YET:
      pInString = pGlobalTXT_LocalizationStrings[577]; //"Get back in there you wimps:"
    break;

    case DIALOGUE_ARENA_REWARD:
      sprintfex(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[576], gold_transaction_amount);// "Congratulations on your win: here's your stuff: %u gold."
      pInString = pTmpBuf.data();
    break;

    case DIALOGUE_ARENA_ALREADY_WON:
      pInString = pGlobalTXT_LocalizationStrings[582]; // "You already won this trip to the Arena:"
    break;

    default:
      if (uDialogueType > DIALOGUE_18 && uDialogueType < DIALOGUE_23 && !byte_5B0938[0])
      {
        pInString = (char *)current_npc_text;
      }
      else if (pGreetType == 1)//QuestNPC_greet
      {
        if (pNPC->greet)
        {
          if ((pNPC->uFlags & 3) == 2)
            pInString = pNPCStats->pNPCGreetings[pNPC->greet].pGreeting2;
          else
            pInString = pNPCStats->pNPCGreetings[pNPC->greet].pGreeting1;
        }
      }
      else if (pGreetType == 2)//HiredNPC_greet
      {
        NPCProfession* prof = &pNPCStats->pProfessions[pNPC->uProfession];

        if (pNPC->Hired())
          pInString = BuildDialogueString(prof->pDismissText, uActiveCharacter - 1, 0, 0, 0, 0);
        else
          pInString = BuildDialogueString(prof->pJoinText, uActiveCharacter - 1, 0, 0, 0, 0);
      }
    break;
  }

  // Message window(Окно сообщения)---- 
  if (pInString)
  {
    window.uFrameWidth = game_viewport_width;
    window.uFrameZ = 452;
    GUIFont* font = pFontArrus;
    pTextHeight = pFontArrus->CalcTextHeight(pInString, &window, 13, 0) + 7;
    if ( 352 - pTextHeight < 8 )
    {
      font = pFontCreate;
       pTextHeight = pFontCreate->CalcTextHeight(pInString, &window, 13, 0) + 7;
    }
    if (uTextureID_Leather != -1)
      pRenderer->GetLeather(8, 352 - pTextHeight, &pIcons_LOD->pTextures[uTextureID_Leather], pIcons_LOD->pTextures[uTextureID_Leather].uTextureHeight - pTextHeight);
    pRenderer->DrawTextureIndexed(8, 347 - pTextHeight, pTexture_591428);
    pDialogueWindow->DrawText(font, 13, 354 - pTextHeight, 0, FitTextInAWindow(pInString, font,  &window, 13, 0), 0, 0, 0);
  }
  // Right panel(Правая панель)------- 
  memcpy(&window, pDialogueWindow, sizeof(window));
  window.uFrameX = 483;
  window.uFrameWidth = 148;
  window.uFrameZ = 334;
  for (int i = window.pStartingPosActiveItem; i < window.pStartingPosActiveItem + window.pNumPresenceButton; ++i)
  {
    pButton = window.GetControl(i);
    if ( !pButton )
      break;

    if ( pButton->msg_param > 88 )
      pButton->pButtonName[0] = 0;
    else if (pButton->msg_param == 88)
      strcpy(pButton->pButtonName, pGlobalTXT_LocalizationStrings[581]); // Lord
    else if (pButton->msg_param == 87)
      strcpy(pButton->pButtonName, pGlobalTXT_LocalizationStrings[580]); // Knight
    else if (pButton->msg_param == 86)
      strcpy(pButton->pButtonName, pGlobalTXT_LocalizationStrings[579]); // Squire
    else if (pButton->msg_param == 85)
      strcpy(pButton->pButtonName, pGlobalTXT_LocalizationStrings[578]); // Page
    else if (pButton->msg_param == 77)
      strcpy(pButton->pButtonName, pGlobalTXT_LocalizationStrings[407]); // Details
    else if (pButton->msg_param == 76)
    {
      if (pNPC->Hired())
        sprintf(pButton->pButtonName, (const char*)pGlobalTXT_LocalizationStrings[408], pNPC->pName); // Release %s
      else
        strcpy(pButton->pButtonName, pGlobalTXT_LocalizationStrings[406]); // Hire
    }
    else if (pButton->msg_param == 24)
    {
      __debugbreak(); // learn conditions of this event
      if (!pNPC->evt_F)
      {
        pButton->pButtonName[0] = 0;
        pButton->msg_param = 0;
      }
      else
        strcpy(pButton->pButtonName, pNPCTopics[pNPC->evt_F].pTopic);
    }
    else if (pButton->msg_param == 9)
      strcpy(pButton->pButtonName, GetProfessionActionText(pNPC->uProfession));
    else if (pButton->msg_param == 19) // Scavenger Hunt
    {
      if (!pNPC->evt_A)
      {
        pButton->pButtonName[0] = 0;
        pButton->msg_param = 0;
      }
      else
        strcpy(pButton->pButtonName, pNPCTopics[pNPC->evt_A].pTopic);
    }
    else if (pButton->msg_param == 20) // Scavenger Hunt
    {
      if (!pNPC->evt_B)
      {
        pButton->pButtonName[0] = 0;
        pButton->msg_param = 0;
      }
      else strcpy(pButton->pButtonName, pNPCTopics[pNPC->evt_B].pTopic);
    }
    else if (pButton->msg_param == 21)
    {
      //__debugbreak(); // learn conditions of this event
      if (!pNPC->evt_C)
      {
        pButton->pButtonName[0] = 0;
        pButton->msg_param = 0;
      }
      else strcpy(pButton->pButtonName, pNPCTopics[pNPC->evt_C].pTopic);
    }
    else if (pButton->msg_param == 22)
    {
      //__debugbreak(); // learn conditions of this event
      if (!pNPC->evt_D)
      {
        pButton->pButtonName[0] = 0;
        pButton->msg_param = 0;
      }
      else strcpy(pButton->pButtonName, pNPCTopics[pNPC->evt_D].pTopic);
    }
    else if (pButton->msg_param == 23)
    {
      //__debugbreak(); // learn conditions of this event
      if (!pNPC->evt_E)
      {
        pButton->pButtonName[0] = 0;
        pButton->msg_param = 0;
      }
      else strcpy(pButton->pButtonName, pNPCTopics[pNPC->evt_E].pTopic);
    }
    else if (pButton->msg_param == 13)
    {
      if (pNPC->Hired())
        sprintf(pButton->pButtonName, pGlobalTXT_LocalizationStrings[408], pNPC->pName); // Release %s
      else
        strcpy(pButton->pButtonName, pGlobalTXT_LocalizationStrings[122]); // Join
    }
    else
      pButton->pButtonName[0] = 0;

    if (pParty->field_7B5_in_arena_quest && pParty->field_7B5_in_arena_quest != -1)
    {
      int num_dead_actors = 0;
      pInString = 0;
      for ( uint i = 0; i < uNumActors; ++i )
      {
        if (pActors[i].uAIState == Dead || pActors[i].uAIState == Removed || pActors[i].uAIState  == Disabled)
          ++num_dead_actors;
        else
        {
          int sumonner_type = PID_TYPE(pActors[i].uSummonerID);
          if (sumonner_type == OBJECT_Player)
            ++num_dead_actors;
        }
      }
      if (num_dead_actors == uNumActors)
        strcpy(pButton->pButtonName, pGlobalTXT_LocalizationStrings[658]); // Collect Prize
    }
  }

  // Install Buttons(Установка кнопок)-------- 
  index = 0;
  all_text_height = 0;
  for ( int i = pDialogueWindow->pStartingPosActiveItem;
        i < pDialogueWindow->pStartingPosActiveItem + pDialogueWindow->pNumPresenceButton; ++i )
  {
    pButton = pDialogueWindow->GetControl(i);
    if ( !pButton )
      break;
    all_text_height += pFontArrus->CalcTextHeight(pButton->pButtonName, &window, 0, 0);
    index++;
  }
  if ( index )
  {
    v45 = (174 - all_text_height) / index;
    if ( v45 > 32 )
      v45 = 32;
    v42 = (174 - v45 * index - all_text_height)/ 2 - v45 / 2 + 138;
    for ( int i = pDialogueWindow->pStartingPosActiveItem;
          i < pDialogueWindow->pNumPresenceButton + pDialogueWindow->pStartingPosActiveItem; ++i )
    {
      pButton = pDialogueWindow->GetControl(i);
      if ( !pButton )
        break;
      pButton->uY = (unsigned int)(v45 + v42);
      pTextHeight = pFontArrus->CalcTextHeight(pButton->pButtonName, &window, 0, 0);
      pButton->uHeight = pTextHeight;
      v42 = pButton->uY + pTextHeight - 1;
      pButton->uW = v42;
      pTextColor = ui_game_dialogue_option_normal_color;
      if ( pDialogueWindow->pCurrentPosActiveItem == i )
		pTextColor = ui_game_dialogue_option_highlight_color;
      window.DrawTitleText(pFontArrus, 0, pButton->uY, pTextColor, pButton->pButtonName, 3);
    }
  }
  pRenderer->DrawTextureIndexed(471, 445, pIcons_LOD->GetTexture(uExitCancelTextureId));
}

//----- (00444FBE) --------------------------------------------------------
void GameUI_DrawBranchlessDialogue()
{
  int pTextHeight; // esi@4
  char Str[200]; // [sp+Ch] [bp-120h]@12
  GUIWindow BranchlessDlg_window; // [sp+D4h] [bp-58h]@4
  GUIFont *pFont; // [sp+128h] [bp-4h]@1

  pFont = pFontArrus;
  if ( current_npc_text && !byte_5B0938[0] )
    strcpy(byte_5B0938.data(), current_npc_text);
  BranchlessDlg_window.uFrameWidth = game_viewport_width;
  BranchlessDlg_window.uFrameZ = 452;
  pTextHeight = pFontArrus->CalcTextHeight(byte_5B0938.data(), &BranchlessDlg_window, 12, 0) + 7;
  if ( 352 - pTextHeight < 8 )
  {
    pFont = pFontCreate;
    pTextHeight = pFontCreate->CalcTextHeight(byte_5B0938.data(), &BranchlessDlg_window, 12, 0) + 7;
  }
  pRenderer->GetLeather(8, 352 - pTextHeight, pIcons_LOD->GetTexture(uTextureID_Leather), pIcons_LOD->GetTexture(uTextureID_Leather)->uTextureHeight - pTextHeight);
  pRenderer->DrawTextureIndexed(8, 347 - pTextHeight, pTexture_591428);
  pGUIWindow2->DrawText(pFont, 12, 354 - pTextHeight, 0, FitTextInAWindow(byte_5B0938.data(), pFont, &BranchlessDlg_window, 12, 0), 0, 0, 0);
  pRenderer->DrawTextureRGB(0, 0x160u, pTexture_StatusBar);
  if ( pGUIWindow2->receives_keyboard_input_2 != WINDOW_INPUT_IN_PROGRESS)
  {
    if ( pGUIWindow2->receives_keyboard_input_2 == WINDOW_INPUT_CONFIRMED)
    {
      pGUIWindow2->receives_keyboard_input_2 = WINDOW_INPUT_NONE;
      strcpy(GameUI_Footer_TimedString.data(), (const char *)pKeyActionMap->pPressedKeysBuffer);
      sub_4452BB();
      return;
    }
    if ( pGUIWindow2->receives_keyboard_input_2 != WINDOW_INPUT_CANCELLED)
      return;
    pGUIWindow2->receives_keyboard_input_2 = WINDOW_INPUT_NONE;
    memset(GameUI_Footer_TimedString.data(), 0, 0xC8u);
    sub_4452BB();
    return;
  }
  if ( pGUIWindow2->ptr_1C == (void *)26 )
  {
    sprintf(Str, "%s %s", GameUI_Footer_TimedString, pKeyActionMap->pPressedKeysBuffer);
    pGUIWindow2->DrawText(pFontLucida, 13, 357, 0, Str, 0, 0, 0);
    pGUIWindow2->DrawFlashingInputCursor(pFontLucida->GetLineWidth(Str) + 13, 357, pFontLucida);
    return;
  }
  if ( pKeyActionMap->pPressedKeysBuffer[0] )
  {
    pKeyActionMap->SetWindowInputStatus(WINDOW_INPUT_NONE);
    memset(GameUI_Footer_TimedString.data(), 0, 0xC8u);
    sub_4452BB();
    return;
  }
}

//----- (004443D5) --------------------------------------------------------
const char *GameUI_GetMinimapHintText()
{
  double v3; // st7@1
  int v7; // eax@4
  ODMFace *pFace; // eax@6
  const char *v14; // eax@8
  char *result; // eax@12
  unsigned int pMapID; // eax@14
  int global_coord_X; // [sp+10h] [bp-1Ch]@1
  int global_coord_Y; // [sp+14h] [bp-18h]@1
  unsigned int pY; // [sp+1Ch] [bp-10h]@1
  unsigned int pX; // [sp+28h] [bp-4h]@1

  result = 0;
  pMouse->GetClickPos(&pX, &pY);
  v3 = 1.0 / (float)((signed int)viewparams->uMinimapZoom * 0.000015258789);
  global_coord_X = (signed __int64)((double)(pX - 557) * v3 + (double)pParty->vPosition.x);
  global_coord_Y = (signed __int64)((double)pParty->vPosition.y - (double)(pY - 74) * v3);
  if ( uCurrentlyLoadedLevelType != LEVEL_Outdoor || pOutdoor->uNumBModels <= 0 )
  {
    pMapID = pMapStats->GetMapInfo(pCurrentMapName);
    if ( pMapID == 0 )
      result = "No Maze Info for this maze on file!";
    else
      result = pMapStats->pInfos[pMapID].pName;
  }
  else
  {
    for ( uint j = 0; j < (uint)pOutdoor->uNumBModels; ++j )
    {
      v7 = int_get_vector_length(abs((signed)pOutdoor->pBModels[j].vBoundingCenter.x - global_coord_X),
                                 abs((signed)pOutdoor->pBModels[j].vBoundingCenter.y - global_coord_Y), 0);
      if ( v7 < 2 * pOutdoor->pBModels[j].sBoundingRadius )
      {
        if ( pOutdoor->pBModels[j].uNumFaces )
        {
          for ( uint i = 0; i < (uint)pOutdoor->pBModels[j].uNumFaces; ++i )
          {
            pFace = &pOutdoor->pBModels[j].pFaces[i];
            if ( pFace->sCogTriggeredID )
            {
              if ( !(BYTE2(pFace->uAttributes) & 0x10) )
              {
                v14 = GetEventHintString(pFace->sCogTriggeredID);
                if ( v14 )
                {
                  if ( _stricmp(v14, "") )
                    result = (char *)v14;
                }
              }
            }
          }
        }
        if ( result )
          return result;
      }
    }
    pMapID = pMapStats->GetMapInfo(pCurrentMapName);
    if ( pMapID == 0 )
      result = "No Maze Info for this maze on file!";
    else
      result = pMapStats->pInfos[pMapID].pName;
    return result;
  }
  return result;
}

//----- (0041D3B7) --------------------------------------------------------
void GameUI_CharacterQuickRecord_Draw(GUIWindow *window, Player *player)
{
  Texture *v13; // eax@6
  PlayerFrame *v15; // eax@12
  unsigned int pTextColor; // eax@15
  const char *v29; // eax@16
  int v36; // esi@22
  const char *v39; // eax@24
  signed int uFramesetID; // [sp+20h] [bp-8h]@9
  int uFramesetIDa; // [sp+20h] [bp-8h]@18

  uint numActivePlayerBuffs = 0;
  for (uint i = 0; i < 24; ++i)
    if (player->pPlayerBuffs[i].uExpireTime > 0)
      ++numActivePlayerBuffs;

  window->uFrameHeight = ((pFontArrus->uFontHeight + 162) + ((numActivePlayerBuffs - 1) * pFontArrus->uFontHeight));
  window->uFrameZ = window->uFrameWidth + window->uFrameX - 1;
  window->uFrameW = ((pFontArrus->uFontHeight + 162) + ((numActivePlayerBuffs - 1) * pFontArrus->uFontHeight)) + window->uFrameY - 1;
  window->DrawMessageBox(0);

  if (player->IsEradicated())
    v13 = pTexture_PlayerFaceEradicated;
  else if (player->IsDead())
    v13 = pTexture_PlayerFaceDead;
  else
  {
      uFramesetID = pPlayerFrameTable->GetFrameIdByExpression(player->expression);
      if ( !uFramesetID )
        uFramesetID = 1;
      if ( player->expression == CHARACTER_EXPRESSION_21)
        v15 = pPlayerFrameTable->GetFrameBy_y(&player->_expression21_frameset, &player->_expression21_animtime, pMiscTimer->uTimeElapsed);
      else
        v15 = pPlayerFrameTable->GetFrameBy_x(uFramesetID, pMiscTimer->Time());
      player->field_1AA2 = v15->uTextureID - 1;
      v13 = pTextures_PlayerFaces[(unsigned int)window->ptr_1C][v15->uTextureID - 1];
  }

  pRenderer->DrawTextureTransparent(window->uFrameX + 24, window->uFrameY + 24, v13);

  sprintfex(pTmpBuf.data(), "\f%05d", ui_character_header_text_color);
  sprintfex(pTmpBuf2.data(), pGlobalTXT_LocalizationStrings[429], player->pName, pClassNames[player->classType]); // "%s the %s"
  strcat(pTmpBuf.data(), pTmpBuf2.data());
  strcat(pTmpBuf.data(), "\f00000\n");

  pTextColor = UI_GetHealthManaAndOtherQualitiesStringColor(player->sHealth, player->GetMaxHealth());
  sprintfex(pTmpBuf2.data(), "%s : \f%05u%d\f00000 / %d\n", pGlobalTXT_LocalizationStrings[108], // "Hit Points"
            pTextColor, player->sHealth, player->GetMaxHealth());
  strcat(pTmpBuf.data(), pTmpBuf2.data());

  pTextColor = UI_GetHealthManaAndOtherQualitiesStringColor(player->sMana, player->GetMaxMana());
  sprintfex(pTmpBuf2.data(), "%s : \f%05u%d\f00000 / %d\n", pGlobalTXT_LocalizationStrings[212], // "Spell Points"
            pTextColor, player->sMana, player->GetMaxMana());
  strcat(pTmpBuf.data(), pTmpBuf2.data());

  pTextColor = player->GetMajorConditionIdx();
  sprintfex(pTmpBuf2.data(), "%s: \f%05d%s\f00000\n", pGlobalTXT_LocalizationStrings[47], // "Condition
            GetConditionDrawColor(pTextColor), aCharacterConditionNames[pTextColor]);
  strcat(pTmpBuf.data(), pTmpBuf2.data());

  if ( player->uQuickSpell )
    v29 = pSpellStats->pInfos[player->uQuickSpell].pShortName;
  else
    v29 = pGlobalTXT_LocalizationStrings[153];
  sprintfex(pTmpBuf2.data(), "%s: %s", pGlobalTXT_LocalizationStrings[172], v29); // "Quick Spell"
  strcat(pTmpBuf.data(), pTmpBuf2.data());

  window->DrawText(pFontArrus, 120, 22, 0, pTmpBuf.data(), 0, 0, 0);

  uFramesetIDa = 0;
  for (uint i = 0; i < 24; ++i)
  {
    SpellBuff* buff = &player->pPlayerBuffs[i];
    if (buff->uExpireTime > 0)
    {
      v36 = uFramesetIDa++ * pFontComic->uFontHeight + 134;
      window->DrawText(pFontComic, 52, v36, ui_game_character_record_playerbuff_colors[i], aSpellNames[20 + i], 0, 0, 0);
      DrawBuff_remaining_time_string(v36, window, buff->uExpireTime - pParty->uTimePlayed, pFontComic);
    }
  }

  v39 = "";
  if ( uFramesetIDa == 0 )
    v39 = pGlobalTXT_LocalizationStrings[153]; // "None"
  sprintfex(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[450], v39); // "Active Spells: %s"
  window->DrawText(pFontArrus, 14, 114, 0, pTmpBuf.data(), 0, 0, 0);
}

//----- (0041A57E) --------------------------------------------------------
void GameUI_QuickRef_Draw()
{
  unsigned int pTextColor; // eax@7
  unsigned int pX; // [sp+14h] [bp-18h]@2
  unsigned int pY; // edi@9
  int pSkillsCount; // ecx@27
  const char *pText; // eax@38
  int pFontHeight; // [sp+18h] [bp-14h]@1

  pRenderer->DrawTextureIndexed(8, 8, pIcons_LOD->LoadTexturePtr("quikref", TEXTURE_16BIT_PALETTE));
  pFontHeight = LOBYTE(pFontArrus->uFontHeight) + 1;
  for ( uint i = 0; i < 4; ++i )
  {
    Player* player = &pParty->pPlayers[i];
    pX = 94 * i + 89;
    if ( i == 0 )
      pGUIWindow_CurrentMenu->DrawTextInRect(pFontArrus, 22, 18, 0, pGlobalTXT_LocalizationStrings[149], 60, 0);//Name
    pGUIWindow_CurrentMenu->DrawTextInRect(pFontArrus, 94 * i + 89, 18, ui_character_header_text_color, player->pName, 84, 0);
    if ( i == 0 )
      pGUIWindow_CurrentMenu->DrawTextInRect(pFontArrus, 22, 47, 0, pGlobalTXT_LocalizationStrings[131], 60, 0); //Уров.
    sprintf(pTmpBuf.data(), "%lu", player->GetActualLevel());
    if ( player->GetActualLevel() <= player->GetBaseLevel())
      pTextColor = player->GetExperienceDisplayColor();
    else
      pTextColor = ui_character_bonus_text_color;
    pGUIWindow_CurrentMenu->DrawTextInRect(pFontArrus, pX, 47, pTextColor, pTmpBuf.data(), 84, 0);
    pY = pFontHeight + 47;
    if ( i == 0 )
      pGUIWindow_CurrentMenu->DrawTextInRect(pFontArrus, 22, pFontHeight + 47, 0, pGlobalTXT_LocalizationStrings[41], 60, 0);//Класс
    pGUIWindow_CurrentMenu->DrawTextInRect(pFontArrus, pX, pY, 0, pClassNames[player->classType], 84, 0);
    pY = pFontHeight + pY;
    if ( i == 0 )
      pGUIWindow_CurrentMenu->DrawTextInRect(pFontArrus, 22, pY, 0, pGlobalTXT_LocalizationStrings[107], 60, 0);//Здор.
    sprintf(pTmpBuf.data(), "%d", player->sHealth);
    pTextColor = UI_GetHealthManaAndOtherQualitiesStringColor(player->sHealth, player->GetMaxHealth());
    pGUIWindow_CurrentMenu->DrawTextInRect(pFontArrus, pX, pY, pTextColor, pTmpBuf.data(), 84, 0);
    pY = pFontHeight + pY;
    if ( i == 0 )
      pGUIWindow_CurrentMenu->DrawTextInRect(pFontArrus, 22, pY, 0, pGlobalTXT_LocalizationStrings[209], 60, 0);//Мана
    sprintf(pTmpBuf.data(), "%d", player->sMana);
    pTextColor = UI_GetHealthManaAndOtherQualitiesStringColor(player->sMana, player->GetMaxMana());
    pGUIWindow_CurrentMenu->DrawTextInRect(pFontArrus, pX, pY, pTextColor, pTmpBuf.data(), 84, 0);
    pY = pFontHeight + pY;
    if ( i == 0 )
      pGUIWindow_CurrentMenu->DrawTextInRect(pFontArrus, 22, pY, 0, pGlobalTXT_LocalizationStrings[0], 60, 0);//Класс брони
    sprintf(pTmpBuf.data(), "%d", player->GetActualAC());
    pTextColor = UI_GetHealthManaAndOtherQualitiesStringColor(player->GetActualAC(), player->GetBaseAC());
    pGUIWindow_CurrentMenu->DrawTextInRect(pFontArrus, pX, pY, pTextColor, pTmpBuf.data(), 84, 0);
    pY = pFontHeight + pY;
    if ( !i )
      pGUIWindow_CurrentMenu->DrawTextInRect(pFontArrus, 22, pY, 0, pGlobalTXT_LocalizationStrings[18], 60, 0);//Атака
    sprintf(pTmpBuf.data(), "%+d", player->GetActualAttack(false));
    pGUIWindow_CurrentMenu->DrawTextInRect(pFontArrus, pX, pY, 0, pTmpBuf.data(), 84, 0);
    pY = pFontHeight + pY;
    if ( !i )
      pGUIWindow_CurrentMenu->DrawTextInRect(pFontArrus, 22, pY, 0, pGlobalTXT_LocalizationStrings[66], 60, 0);//Повр.
    pGUIWindow_CurrentMenu->DrawTextInRect(pFontArrus, pX, pY, 0, player->GetMeleeDamageString(), 84, 0);
    pY = pFontHeight + pY;
    if ( !i )
      pGUIWindow_CurrentMenu->DrawTextInRect(pFontArrus, 22, pY, 0, pGlobalTXT_LocalizationStrings[203], 60, 0);// Стрелять
    sprintf(pTmpBuf.data(), "%+d", player->GetRangedAttack());
    pGUIWindow_CurrentMenu->DrawTextInRect(pFontArrus, pX, pY, 0, pTmpBuf.data(), 84, 0);
    pY = pFontHeight + pY;
    if ( !i )
      pGUIWindow_CurrentMenu->DrawTextInRect(pFontArrus, 22, pY, 0, pGlobalTXT_LocalizationStrings[66], 60, 0);//Повр.
    pGUIWindow_CurrentMenu->DrawTextInRect(pFontArrus, pX, pY, 0, player->GetRangedDamageString(), 84, 0);
    pY = pFontHeight + pY;
    if ( !i )
      pGUIWindow_CurrentMenu->DrawTextInRect(pFontArrus, 22, pY, 0, pGlobalTXT_LocalizationStrings[205], 60, 0);//Навыки
    pSkillsCount = 0;
    for ( uint j = 0; j <= 36; ++j )
    {
      if ( player->pActiveSkills[j] )
      ++pSkillsCount;
    }
    sprintf(pTmpBuf.data(), "%lu", pSkillsCount);
    pGUIWindow_CurrentMenu->DrawTextInRect(pFontArrus, pX, pY, 0, pTmpBuf.data(), 84, 0);
    pY = pFontHeight + pY;
    if ( !i )
      pGUIWindow_CurrentMenu->DrawTextInRect(pFontArrus, 22, pY, 0, pGlobalTXT_LocalizationStrings[168], 60, 0);//Очки
    sprintf(pTmpBuf.data(), "%lu", player->uSkillPoints);
    pGUIWindow_CurrentMenu->DrawTextInRect(pFontArrus, pX, pY, player->uSkillPoints ? ui_character_bonus_text_color : ui_character_default_text_color, pTmpBuf.data(), 84, 0);
    pY = pFontHeight + pY;
    if ( !i )
      pGUIWindow_CurrentMenu->DrawTextInRect(pFontArrus, 22, pY, 0, pGlobalTXT_LocalizationStrings[45], 60, 0);//Сост.
    pGUIWindow_CurrentMenu->DrawTextInRect(pFontArrus, pX, pY, pTextColor, aCharacterConditionNames[player->GetMajorConditionIdx()], 84, 0);
    pY = pFontHeight + pY;
    if ( !i )
      pGUIWindow_CurrentMenu->DrawTextInRect(pFontArrus, 22, pY, 0, pGlobalTXT_LocalizationStrings[170], 60, 0);//Б.Прим.
    if (player->uQuickSpell)
      pText = pSpellStats->pInfos[player->uQuickSpell].pShortName;
    else
      pText = pGlobalTXT_LocalizationStrings[153];//Нет
    pGUIWindow_CurrentMenu->DrawTextInRect(pFontArrus, pX, pY, 0, pText, 84, 0);
  }

  if ( pParty->GetPartyReputation() >= 0 )
  {
    if ( pParty->GetPartyReputation() <= 5 )
      pTextColor = ui_character_default_text_color;
    else
      pTextColor = ui_character_bonus_text_color_neg;
  }
  else
    pTextColor = ui_character_bonus_text_color;

  sprintf(pTmpBuf.data(), "%s: \f%05d%s\f00000", pGlobalTXT_LocalizationStrings[180], pTextColor, GetReputationString(pParty->GetPartyReputation()));//Reputation
  pGUIWindow_CurrentMenu->DrawText(pFontArrus, 22, 323, 0, pTmpBuf.data(), 0, 0, 0);
  sprintf(pTmpBuf.data(), "\r261%s: %d", pGlobalTXT_LocalizationStrings[84], pParty->GetPartyFame());// Fame Слава
  pGUIWindow_CurrentMenu->DrawText(pFontArrus, 0, 323, 0, pTmpBuf.data(), 0, 0, 0);
}

//----- (0041AD6E) --------------------------------------------------------
void GameUI_DrawRightPanelItems()
{
  if ( (unsigned long long)GameUI_RightPanel_BookFlashTimer > pParty->uTimePlayed )
    GameUI_RightPanel_BookFlashTimer = 0;

  if ( pParty->uTimePlayed - GameUI_RightPanel_BookFlashTimer > 128 )
  {
    GameUI_RightPanel_BookFlashTimer = pParty->uTimePlayed;
    
    static bool byte_50697C = false; // 50697C
    byte_50697C = !byte_50697C;
    if ( byte_50697C && pCurrentScreen != SCREEN_REST )
    {
      if (bFlashQuestBook)     pRenderer->DrawTextureTransparent(493, 355, pIcons_LOD->GetTexture(uTextureID_ib_td1_A));
      if (bFlashAutonotesBook) pRenderer->DrawTextureTransparent(527, 353, pIcons_LOD->GetTexture(uTextureID_ib_td2_A));
      if (bFlashHistoryBook)   pRenderer->DrawTextureTransparent(600, 361, pIcons_LOD->GetTexture(uTextureID_ib_td5_A));
    }
    else
    {
      pRenderer->DrawTextureRGB(468, 0, pTexture_RightFrame);
      GameUI_DrawHiredNPCs();
    }
  }
}

//----- (0041AEBB) --------------------------------------------------------
void GameUI_DrawFoodAndGold()
{
  int v2; // esi@2

  if ( uGameState != GAME_STATE_FINAL_WINDOW )
  {
    v2 = sub_44100D() != 0 ? 381 : 322;
    sprintf(pTmpBuf.data(), "\r087%lu", pParty->uNumFoodRations);
    pPrimaryWindow->DrawText(pFontSmallnum, 0, v2, uGameUIFontMain, pTmpBuf.data(), 0, 0, uGameUIFontShadow);
    sprintf(pTmpBuf.data(), "\r028%lu", pParty->uNumGold);
    pPrimaryWindow->DrawText(pFontSmallnum, 0, v2, uGameUIFontMain, pTmpBuf.data(), 0, 0, uGameUIFontShadow);
  }
}

//----- (0041B0C9) --------------------------------------------------------
void GameUI_DrawLifeManaBars()
{
  double v3; // st7@3
  double v7; // st7@25
  Texture *pTextureHealth; // [sp-4h] [bp-30h]@10
  Texture *pTextureMana; // [sp+Ch] [bp-20h]@1

  pTextureMana = pIcons_LOD->GetTexture(uTextureID_BarBlue);
  for (uint i = 0; i < 4; ++i)
  {
    if (pParty->pPlayers[i].sHealth > 0)
    {
      int v17 = 0;
      if (i == 2 || i == 3)
        v17 = 2;
      v3 = (double)pParty->pPlayers[i].sHealth / (double)pParty->pPlayers[i].GetMaxHealth();
      if( v3 > 0.5 )
      {
        if ( v3 > 1.0 )
          v3 = 1.0;
        pTextureHealth = pIcons_LOD->GetTexture(uTextureID_BarGreen);
      }
      else if ( v3 > 0.25 )
        pTextureHealth = pIcons_LOD->GetTexture(uTextureID_BarYellow);
      else if ( v3 > 0.0 )
        pTextureHealth = pIcons_LOD->GetTexture(uTextureID_BarRed);
      if( v3 > 0.0 )
      {
        pRenderer->SetTextureClipRect(v17 + pHealthBarPos[i], (signed __int64)((1.0 - v3) * pTextureHealth->uTextureHeight) + 402,
                                          v17 + pHealthBarPos[i] + pTextureHealth->uTextureWidth, pTextureHealth->uTextureHeight + 402);
        pRenderer->DrawTextureIndexed(v17 + pHealthBarPos[i], 402, pTextureHealth);
        pRenderer->ResetTextureClipRect();
      }
    }
    if (pParty->pPlayers[i].sMana > 0)
    {
      v7 = pParty->pPlayers[i].sMana / (double)pParty->pPlayers[i].GetMaxMana();
      if ( v7 > 1.0 )
        v7 = 1.0;
      int v17 = 0;
      if (i == 2)
        v17 = 1;
      pRenderer->SetTextureClipRect(v17 + pManaBarPos[i], (signed __int64)((1.0 - v7) * pTextureMana->uTextureHeight) + 402,
                                    v17 + pManaBarPos[i] + pTextureMana->uTextureWidth, pTextureMana->uTextureHeight + 402);
      pRenderer->DrawTextureIndexed(v17 + pManaBarPos[i], 402, pTextureMana);
      pRenderer->ResetTextureClipRect();
    }
  }
}

//----- (0041B3B6) --------------------------------------------------------
void GameUI_DrawRightPanel()
{
  pRenderer->DrawTextureTransparent(pViewport->uViewportBR_X, 0, pIcons_LOD->GetTexture(uTextureID_right_panel));
}

//----- (0041B3E2) --------------------------------------------------------
void GameUI_DrawRightPanelFrames()
{
  pRenderer->DrawTextureRGB(0,   0,   pTexture_TopFrame);
  pRenderer->DrawTextureRGB(0,   8,   pTexture_LeftFrame);
  pRenderer->DrawTextureRGB(468, 0,   pTexture_RightFrame);
  pRenderer->DrawTextureRGB(0,   352, pTexture_BottomFrame);
  GameUI_DrawFoodAndGold();
  GameUI_DrawRightPanelItems();
}

//----- (0041C047) --------------------------------------------------------
void GameUI_Footer_2()
{
  char *v1; // edx@2
  int v5; // eax@5

  pRenderer->DrawTextureRGB(0, 352, pTexture_StatusBar);
  if (GameUI_Footer_TimeLeft)
    v1 = GameUI_Footer_TimedString.data();
  else
  {
    if (!pFooterString[0])
      return;
    v1 = pFooterString.data();
  }

  v5 = pFontLucida->AlignText_Center(450, v1);
  pPrimaryWindow->DrawText(pFontLucida, v5 + 11, 357, uGameUIFontMain, v1, 0, 0, uGameUIFontShadow);
}

//----- (0041C0B8) --------------------------------------------------------
void GameUI_SetFooterString(const char *pStr)
{
  const char *v1; // esi@1

  v1 = pStr;
  if ( pStr && strcmp(pStr, "test") && !IsBadStringPtrA(pStr, 1) && (*v1 || GameUI_Footer_TimeLeft) )
  {
    if ( GameUI_Footer_TimeLeft )
    {
      for ( int i = pFontLucida->GetLineWidth(GameUI_Footer_TimedString.data()); i > 450; i = pFontLucida->GetLineWidth(GameUI_Footer_TimedString.data()) )
        GameUI_Footer_TimedString[strlen(GameUI_Footer_TimedString.data()) - 1] = 0;
    }
    else
    {
      strcpy(pFooterString.data(), v1);
      for ( int j = pFontLucida->GetLineWidth(pFooterString.data()); j > 450; j = pFontLucida->GetLineWidth(pFooterString.data()) )
        pFooterString[strlen(pFooterString.data()) - 1] = 0;
    }
  }
}

//----- (0041C179) --------------------------------------------------------
void GameUI_Footer()
{
  char *v1; // edi@5
  int v2; // eax@5
  int v6; // eax@9
  char v9; // zf@12

  if ( pFooterString[0] || GameUI_Footer_TimeLeft || bForceDrawFooter )
  {
    pRenderer->DrawTextureRGB(0, 352, pTexture_StatusBar);
    if ( GameUI_Footer_TimeLeft )
    {
      v1 = GameUI_Footer_TimedString.data();
      v2 = pFontLucida->GetLineWidth(GameUI_Footer_TimedString.data());
      while ( v2 > 450 )
      {
        GameUI_Footer_TimedString[strlen(GameUI_Footer_TimedString.data()) - 1] = 0;
        v2 = pFontLucida->GetLineWidth(GameUI_Footer_TimedString.data());
      }
    }
    else
    {
      v1 = pFooterString.data();
      v6 = pFontLucida->GetLineWidth(pFooterString.data());
      while ( v6 > 450 )
      {
        pFooterString[strlen(pFooterString.data()) - 1] = 0;
        v6 = pFontLucida->GetLineWidth(pFooterString.data());
      }
    }
    v9 = *v1 == 0;
    bForceDrawFooter = 0;
    if ( !v9 )
      pPrimaryWindow->DrawText(pFontLucida, pFontLucida->AlignText_Center(450, v1) + 11, 357, uGameUIFontMain, v1, 0, 0, uGameUIFontShadow);
  }
}
// 5C35BC: using guessed type int bForceDrawFooter;
//----- (00420EFF) --------------------------------------------------------
void  GameUI_WritePointedObjectStatusString()
{
  int v1; // ebx@6
  GUIWindow *pWindow; // edi@7
  GUIButton *pButton; // ecx@11
  int v7; // ecx@19
  enum UIMessageType pMessageType1; // esi@24
  int v14; // eax@41
  ItemGen *pItemGen; // ecx@44
  int v16; // ecx@46
  signed int v18; // eax@55
  signed int v18b;
  signed int v19; // ecx@63
  BLVFace *pFace; // eax@69
  const char *pText; // ecx@79
  char *v28; // esi@82
  enum UIMessageType pMessageType2; // esi@110
  enum UIMessageType pMessageType3; // edx@117
  char Str1[200]; // [sp+Ch] [bp-D4h]@129
  unsigned int pX; // [sp+D4h] [bp-Ch]@1
  unsigned int pY; // [sp+D8h] [bp-8h]@1
  unsigned int v45; // [sp+DCh] [bp-4h]@21

  int interaction_distance_limit = 512;

  pMouse->uPointingObjectID = 0;
  pMouse->GetClickPos(&pX, &pY);
  if ( pX < 0 || pX > 639 || pY < 0 || pY > 479 )
    return;
  if (pCurrentScreen == SCREEN_GAME)
  {
    if ( pX > 467 || pY > 351 )
      goto _click_on_game_ui;
    if ( pRenderer->pRenderD3D )  // inlined mm8::4C1E01
    {
      v18 = pGame->pVisInstance->get_picked_object_zbuf_val();
      if ( pX < (unsigned int)pViewport->uScreen_TL_X || pX > (unsigned int)pViewport->uScreen_BR_X
        || pY < (unsigned int)pViewport->uScreen_TL_Y || pY > (unsigned int)pViewport->uScreen_BR_Y )
        v18 = -1;
      if ( v18 == -1 )
      {
        pMouse->uPointingObjectID = 0;
        if ( pMouse->uPointingObjectID == 0 )
        {
          if ( uLastPointedObjectID != 0 )
          {
            pFooterString[0] = 0;
            bForceDrawFooter = 1;
          }
        }
        uLastPointedObjectID = pMouse->uPointingObjectID;
        return;
      }
    }
    else
      v18 = pRenderer->pActiveZBuffer[pX + pSRZBufferLineOffsets[pY]];
    pMouse->uPointingObjectID = (unsigned __int16)v18;
    v19 = (signed)PID_ID(v18);
  //For Items------------------------------------
    if (PID_TYPE(v18) == OBJECT_Item)
    {
      if ( pObjectList->pObjects[pSpriteObjects[v19].uObjectDescID].uFlags & 0x10 )
      {
        pMouse->uPointingObjectID = 0;
        pFooterString[0] = 0;
        bForceDrawFooter = 1;
        uLastPointedObjectID = 0;
        return;
      }
      if ( v18 >= (signed int)0x2000000u || pParty->pPickedItem.uItemID )
      {
        GameUI_SetFooterString(pSpriteObjects[v19].stru_24.GetDisplayName());
        if ( pMouse->uPointingObjectID == 0 )
        {
          if ( uLastPointedObjectID != 0 )
          {
            pFooterString[0] = 0;
            bForceDrawFooter = 1;
          }
        }
        uLastPointedObjectID = pMouse->uPointingObjectID;
        return;
      }
      v28 = pTmpBuf.data();
      sprintfex(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[470], pSpriteObjects[v19].stru_24.GetDisplayName());// "Get %s"
    }
//For Decorations----------------------------------
    if (PID_TYPE(v18) == OBJECT_Decoration)
    {
      if ( !pLevelDecorations[v19].uEventID )
      {
        if ( pLevelDecorations[v19].IsInteractive() )
          pText = pNPCTopics[stru_5E4C90_MapPersistVars._decor_events[pLevelDecorations[v19]._idx_in_stru123 - 75] + 379].pTopic;//неверно для костра
        else
          pText = pDecorationList->pDecorations[pLevelDecorations[v19].uDecorationDescID].field_20;
        GameUI_SetFooterString(pText);
        if ( pMouse->uPointingObjectID == 0 )
        {
          if ( uLastPointedObjectID != 0 )
          {
            pFooterString[0] = 0;
            bForceDrawFooter = 1;
          }
        }
        uLastPointedObjectID = pMouse->uPointingObjectID;
        return;
      }
      if ( !GetEventHintString(pLevelDecorations[v19].uEventID) )
      {
        if ( pMouse->uPointingObjectID == 0 )
        {
          if ( uLastPointedObjectID != 0 )
          {
            pFooterString[0] = 0;
            bForceDrawFooter = 1;
          }
        }
        uLastPointedObjectID = pMouse->uPointingObjectID;
        return;
      }
      GameUI_SetFooterString(GetEventHintString(pLevelDecorations[v19].uEventID));
      if ( pMouse->uPointingObjectID == 0 )
      {
        if ( uLastPointedObjectID != 0 )
        {
          pFooterString[0] = 0;
          bForceDrawFooter = 1;
        }
      }
      uLastPointedObjectID = pMouse->uPointingObjectID;
      return;
    }
  //For 3D Model-------------------------------------
    if (PID_TYPE(v18) == OBJECT_BModel)
    {
      if ( HIWORD(v18) < interaction_distance_limit )
      {
        if ( uCurrentlyLoadedLevelType != LEVEL_Indoor)
        {
          v18b = (signed int)(unsigned __int16)v18 >> 9;
          if ( !pOutdoor->pBModels[v18b].pFaces[v19 & 0x3F].sCogTriggeredID
            || !GetEventHintString(pOutdoor->pBModels[v18b].pFaces[v19 & 0x3F].sCogTriggeredID) )
          {
            pMouse->uPointingObjectID = 0;
            pFooterString[0] = 0;
            bForceDrawFooter = 1;
            uLastPointedObjectID = 0;
            return;
          }
          GameUI_SetFooterString(GetEventHintString(pOutdoor->pBModels[v18b].pFaces[v19 & 0x3F].sCogTriggeredID));
          if ( pMouse->uPointingObjectID == 0 )
          {
            if ( uLastPointedObjectID != 0 )
            {
              pFooterString[0] = 0;
              bForceDrawFooter = 1;
            }
          }
          uLastPointedObjectID = pMouse->uPointingObjectID;
          return;
        }
        pFace = &pIndoor->pFaces[v19];
        if ( BYTE3(pFace->uAttributes) & 6 )
        {
          if ( !pIndoor->pFaceExtras[pFace->uFaceExtraID].uEventID
            || !GetEventHintString(pIndoor->pFaceExtras[pFace->uFaceExtraID].uEventID) )
          {
            pMouse->uPointingObjectID = 0;
            pFooterString[0] = 0;
            bForceDrawFooter = 1;
            uLastPointedObjectID = 0;
            return;
          }
          GameUI_SetFooterString(GetEventHintString(pIndoor->pFaceExtras[pFace->uFaceExtraID].uEventID));
          if ( pMouse->uPointingObjectID == 0 )
          {
            if ( uLastPointedObjectID != 0 )
            {
              pFooterString[0] = 0;
              bForceDrawFooter = 1;
            }
          }
          uLastPointedObjectID = pMouse->uPointingObjectID;
          return;
        }
      }
      pMouse->uPointingObjectID = 0;
      pFooterString[0] = 0;
      bForceDrawFooter = 1;
      uLastPointedObjectID = 0;
      return;
    }
    else
    {
      if (PID_TYPE(v18) != OBJECT_Actor)
      {
        pMouse->uPointingObjectID = 0;
        if ( uLastPointedObjectID != 0 )
        {
          pFooterString[0] = 0;
          bForceDrawFooter = 1;
        }
        uLastPointedObjectID = pMouse->uPointingObjectID;
        return;
      }
      if ( v18 >= 335544320 )
      {
        pMouse->uPointingObjectID = 0;
        if ( pMouse->uPointingObjectID == 0 )
        {
          if ( uLastPointedObjectID != 0 )
          {
            pFooterString[0] = 0;
            bForceDrawFooter = 1;
          }
        }
        uLastPointedObjectID = pMouse->uPointingObjectID;
        return;
      }
      v28 = pTmpBuf.data();
      if ( pActors[v19].dword_000334_unique_name )
        pText = pMonsterStats->pPlaceStrings[pActors[v19].dword_000334_unique_name];
      else
        pText = pMonsterStats->pInfos[pActors[v19].pMonsterInfo.uID].pName;
      strncpy(pTmpBuf.data(), pText, 0x7D0u);
    }
    GameUI_SetFooterString(v28);
    if ( pMouse->uPointingObjectID == 0 )
    {
      if ( uLastPointedObjectID != 0 )
      {
        pFooterString[0] = 0;
        bForceDrawFooter = 1;
      }
    }
    uLastPointedObjectID = pMouse->uPointingObjectID;
    return;
  }
  v1 = uNumVisibleWindows;
  if ( uNumVisibleWindows > 0 )
  {
	  while ( 1 )                                   // some other fullscreen ui
	  {
		pWindow = &pWindowList[pVisibleWindowsIdxs[v1] - 1];
		if ( (signed int)pX >= (signed int)pWindow->uFrameX
		  && (signed int)pX <= (signed int)pWindow->uFrameZ
		  && (signed int)pY >= (signed int)pWindow->uFrameY
		  && (signed int)pY <= (signed int)pWindow->uFrameW )
		{
		  for ( pButton = pWindow->pControlsHead; ; pButton = pButton->pNext )
		  {
			if ( !pButton )
			  break;
			if ( pButton->uButtonType == 1 )
			{
			  if ( (signed int)pX >= (signed int)pButton->uX
				&& (signed int)pX <= (signed int)pButton->uZ
				&& (signed int)pY >= (signed int)pButton->uY
				&& (signed int)pY <= (signed int)pButton->uW )
			  {
				pMessageType1 = (UIMessageType)pButton->field_1C;
				if ( pMessageType1 )
				{
				  pMessageQueue_50CBD0->AddMessage(pMessageType1, pButton->msg_param, 0);
				}
				GameUI_SetFooterString(pButton->pButtonName);
				uLastPointedObjectID = 1;
				return;
			  }
			}
			else
			{
			  if ( pButton->uButtonType == 2 )
			  {
				v45 = pX - pButton->uX;
				v45 = pY - pButton->uY;
				if ( (double)(signed int)pButton->uWidth != 0.0 )
				{
				  if ( (double)(signed int)pButton->uHeight != 0.0 )
				  {
					  pMessageType1 = (UIMessageType)pButton->field_1C;
					  if ( pMessageType1 )
					  {
						pMessageQueue_50CBD0->AddMessage(pMessageType1, pButton->msg_param, 0);
					  }
					  GameUI_SetFooterString(pButton->pButtonName);
					  uLastPointedObjectID = 1;
					  return;
					//}
				  }
				}
			  }
			  else                                  // click on skill
			  {
              if ( pButton->uButtonType == 3 && pX >= pButton->uX && pX <= pButton->uZ && pY >= pButton->uY && pY <= pButton->uW )
              {
                v7 = (LOBYTE(pPlayers[uActiveCharacter]->pActiveSkills[pButton->msg_param]) & 0x3F) + 1;
                if ( pPlayers[uActiveCharacter]->uSkillPoints < v7 )
                  sprintf(Str1, pGlobalTXT_LocalizationStrings[469], v7 - pPlayers[uActiveCharacter]->uSkillPoints);// "You need %d more Skill Points to advance here"
                else
                  sprintf(Str1, pGlobalTXT_LocalizationStrings[468], v7);// "Clicking here will spend %d Skill Points"
                GameUI_SetFooterString(Str1);
                uLastPointedObjectID = 1;
                return;
				}
			  }
			}
		  }
		}
		if ( pWindow->uFrameHeight == 480 )
		  break;
		--v1;
		if ( v1 <= 0 )
		{
		  break;
		}
	  }
  }
  if ( uNumVisibleWindows <= 0 || (uNumVisibleWindows > 0 && pWindow->uFrameHeight != 480 && v1 <= 0))
  {
    if ( pCurrentScreen == SCREEN_CHEST )
    {
      ChestUI_WritePointedObjectStatusString();
      if ( pMouse->uPointingObjectID == 0 )
      {
        if ( uLastPointedObjectID != 0 )
        {
          pFooterString[0] = 0;
          bForceDrawFooter = 1;
        }
      }
      uLastPointedObjectID = pMouse->uPointingObjectID;
      return;
    }
    if ( pCurrentScreen == SCREEN_HOUSE )
    {
      if ( dialog_menu_id != HOUSE_DIALOGUE_SHOP_BUY_STANDARD
        || (v16 = pRenderer->pActiveZBuffer[pX + pSRZBufferLineOffsets[pY]], v16 == 0)
        || v16 == -65536 )
      {
        if ( pMouse->uPointingObjectID == 0 )
        {
          if ( uLastPointedObjectID != 0 )
          {
            pFooterString[0] = 0;
            bForceDrawFooter = 1;
          }
        }
        uLastPointedObjectID = pMouse->uPointingObjectID;
        return;
       }
      pItemGen = (ItemGen *)((char *)&pParty->pPickedItem + 36 * (v16 + 12 * (unsigned int)window_SpeakInHouse->ptr_1C) + 4);
      GameUI_SetFooterString(pItemGen->GetDisplayName());
      uLastPointedObjectID = 1;
      if ( pMouse->uPointingObjectID == 0 )
      {
        if ( uLastPointedObjectID != 0 )
        {
          pFooterString[0] = 0;
          bForceDrawFooter = 1;
        }
      }
      uLastPointedObjectID = pMouse->uPointingObjectID;
      return;
    }
    if ( (signed int)pY < 350 )
    {
      v14 = pRenderer->pActiveZBuffer[pX + pSRZBufferLineOffsets[pY]];
      if ( v14 == 0 || v14 == -65536 || (unsigned int)v14 >= 0x1388 )
      {
        if ( pMouse->uPointingObjectID == 0 )
        {
          if ( uLastPointedObjectID != 0 )
          {
            pFooterString[0] = 0;
            bForceDrawFooter = 1;
          }
        }
        uLastPointedObjectID = pMouse->uPointingObjectID;
        return;
      }
      pItemGen = (ItemGen *)&pPlayers[uActiveCharacter]->pInventoryItemList[v14-1];
      GameUI_SetFooterString(pItemGen->GetDisplayName());
      uLastPointedObjectID = 1;
      if ( pMouse->uPointingObjectID == 0 )
      {
        if ( uLastPointedObjectID != 0 )
        {
          pFooterString[0] = 0;
          bForceDrawFooter = 1;
        }
      }
      uLastPointedObjectID = pMouse->uPointingObjectID;
      return;
    }
_click_on_game_ui:
    if ( (signed int)pX >= (signed int)pWindowList[0].uFrameX
      && (signed int)pX <= (signed int)pWindowList[0].uFrameZ
      && (signed int)pY >= (signed int)pWindowList[0].uFrameY
      && (signed int)pY <= (signed int)pWindowList[0].uFrameW )
    {
      for ( pButton = pWindowList[0].pControlsHead; pButton != (GUIButton *)0; pButton = pButton->pNext )
      {
        if ( pButton->uButtonType == 1 )
        {
          if ( (signed int)pX >= (signed int)pButton->uX
            && (signed int)pX <= (signed int)pButton->uZ
            && (signed int)pY >= (signed int)pButton->uY
            && (signed int)pY <= (signed int)pButton->uW )
          {
            pMessageType3 = (UIMessageType)pButton->field_1C;
            if ( pMessageType3 == 0 ) // For books
            {
              GameUI_SetFooterString(pButton->pButtonName);
              uLastPointedObjectID = 1;
              return;
            }

            pMessageQueue_50CBD0->AddMessage(pMessageType3, pButton->msg_param, 0);
            uLastPointedObjectID = 1;
            return;
          }
        }
        else
        {
          if ( pButton->uButtonType == 2 )
          {
            v45 = pX - pButton->uX;
            v45 = pY - pButton->uY;

            if (pX >= pButton->uX && pX <= pButton->uZ &&
                pY >= pButton->uY && pY <= pButton->uW)
            if ( (double)(signed int)pButton->uWidth != 0.0 )
            {
              if ( (double)(signed int)pButton->uHeight != 0.0 )
              {
                 //UNDEF(v32);
                //if ( v33 | v34 )
                //{
                  pMessageType2 = (UIMessageType)pButton->field_1C;
                  if ( pMessageType2 != 0 )
                      pMessageQueue_50CBD0->AddMessage(pMessageType2, pButton->msg_param, 0);

                  GameUI_SetFooterString(pButton->pButtonName);
                  uLastPointedObjectID = 1;
                  return;
                //}
              }
            }
          }
          else
          {
            if ( pButton->uButtonType == 3
              && pX >= pButton->uX && pX <= pButton->uZ
              && pY >= pButton->uY && pY <= pButton->uW )
            {
              v7 = (LOBYTE(pPlayers[uActiveCharacter]->pActiveSkills[pButton->msg_param]) & 0x3F) + 1;
              if ( pPlayers[uActiveCharacter]->uSkillPoints < v7 )
                sprintf(Str1, pGlobalTXT_LocalizationStrings[469], v7 - pPlayers[uActiveCharacter]->uSkillPoints);// "You need %d more Skill Points to advance here"
              else
                sprintf(Str1, pGlobalTXT_LocalizationStrings[468], v7);// "Clicking here will spend %d Skill Points"
              GameUI_SetFooterString(Str1);
              uLastPointedObjectID = 1;
              return;
            }
          }
        }
      }
    }
    pMouse->uPointingObjectID = sub_46A99B();
    if ( pMouse->uPointingObjectID == 0 )
    {
      if ( uLastPointedObjectID != 0 )
      {
        pFooterString[0] = 0;
        bForceDrawFooter = 1;
      }
    }
    uLastPointedObjectID = pMouse->uPointingObjectID;
    return;
  }
}

//----- (0044158F) --------------------------------------------------------
void GameUI_DrawCharacterSelectionFrame()
{
  if ( uActiveCharacter )
    pRenderer->DrawTextureTransparent(pPlayerPortraitsXCoords_For_PlayerBuffAnimsDrawing[uActiveCharacter - 1] - 9, 380,
                                      pIcons_LOD->GetTexture(uTextureID_GameUI_CharSelectionFrame));
}

//----- (0044162D) --------------------------------------------------------
void GameUI_DrawPartySpells()
{
  unsigned int v0; // ebp@1
  Texture *v7; // [sp-4h] [bp-1Ch]@12
  Texture *v9; // [sp-4h] [bp-1Ch]@21

  v0 = (signed __int64)((double)GetTickCount() * 0.050000001);
  //v1 = 0;
  for (uint i = 0; i < 14; ++i)
  {
    //v2 =  byte_4E5DD8[v1];
    if (pParty->pPartyBuffs[byte_4E5DD8[i]].uExpireTime)
    {
      Texture* tex = pIcons_LOD->GetTexture(pTextureIDs_PartyBuffIcons[i]);
      //v3 = pTextureIDs_PartyBuffIcons[i];
      pRenderer->_4A65CC(pPartySpellbuffsUI_XYs[i][0],
      pPartySpellbuffsUI_XYs[i][1], tex, tex,
      v0 + 20 * pPartySpellbuffsUI_smthns[i], 0, 63);
    }
    //++v1;
  }
  //while ( v1 < 14 );
  if (pCurrentScreen == SCREEN_GAME || pCurrentScreen == SCREEN_NPC_DIALOGUE)
  {
    if (pParty->FlyActive())
    {
      if ( pParty->bFlying )
        v7 = pIcons_LOD->GetTexture(pIconsFrameTable->GetFrame(uIconIdx_FlySpell, v0)->uTextureID);
      else
        v7 = pIcons_LOD->GetTexture(pIconsFrameTable->GetFrame(uIconIdx_FlySpell, 0)->uTextureID);
      if ( pRenderer->pRenderD3D )
        pRenderer->DrawTextureIndexed(8, 8, v7);
      else
        pRenderer->DrawTextureTransparent(8, 8, v7);
    }
    if ( pParty->WaterWalkActive() )
    {
      if ( pParty->uFlags & PARTY_FLAGS_1_STANDING_ON_WATER )
        v9 = pIcons_LOD->GetTexture(pIconsFrameTable->GetFrame(uIconIdx_WaterWalk, v0)->uTextureID);
      else
        v9 = pIcons_LOD->GetTexture(pIconsFrameTable->GetFrame(uIconIdx_WaterWalk, 0)->uTextureID);
      if ( pRenderer->pRenderD3D )
        pRenderer->DrawTextureIndexed(396, 8, v9);
      else
        pRenderer->DrawTextureTransparent(396, 8, v9);
    }
  }
  for (uint i = 0; i < 4; ++i)
  {
    if ( pParty->pPlayers[i].pPlayerBuffs[PLAYER_BUFF_HAMMERHANDS].uExpireTime )
      pRenderer->DrawTextureIndexed(pPlayerPortraitsXCoords_For_PlayerBuffAnimsDrawing[i] + 72, 427, pIcons_LOD->GetTexture(uTextureID_PlayerBuff_Hammerhands));
    if ( pParty->pPlayers[i].pPlayerBuffs[PLAYER_BUFF_BLESS].uExpireTime )
      pRenderer->DrawTextureIndexed(pPlayerPortraitsXCoords_For_PlayerBuffAnimsDrawing[i] + 72, 393, pIcons_LOD->GetTexture(uTextureID_PlayerBuff_Bless));
    if ( pParty->pPlayers[i].pPlayerBuffs[PLAYER_BUFF_PRESERVATION].uExpireTime )
      pRenderer->DrawTextureIndexed(pPlayerPortraitsXCoords_For_PlayerBuffAnimsDrawing[i] + 72, 410, pIcons_LOD->GetTexture(uTextureID_PlayerBuff_Preservation));
    if ( pParty->pPlayers[i].pPlayerBuffs[PLAYER_BUFF_PAIN_REFLECTION].uExpireTime )
      pRenderer->DrawTextureIndexed(pPlayerPortraitsXCoords_For_PlayerBuffAnimsDrawing[i] + 72, 444, pIcons_LOD->GetTexture(uTextureID_PlayerBuff_PainReflection));
  }
}

//----- (004921C1) --------------------------------------------------------
void GameUI_DrawPortraits(unsigned int _this)
{
    unsigned int face_expression_ID; // eax@17
    PlayerFrame *pFrame; // eax@21
    int pTextureID; // eax@57
    Texture *pPortrait; // [sp-4h] [bp-1Ch]@27

  if ( qword_A750D8 )
  {
    qword_A750D8 -= (signed int)pMiscTimer->uTimeElapsed;
    if ( qword_A750D8 <= 0 )
    {
      if ( pPlayers[uSpeakingCharacter]->CanAct() )
        pPlayers[uSpeakingCharacter]->PlaySound(PlayerSpeechID, 0);
      qword_A750D8 = 0i64;
    }
  }

  for (uint i = 0; i < 4; ++i)
  {
    Player* pPlayer = &pParty->pPlayers[i];
    if ( pPlayer->IsEradicated() )
    {
      pPortrait = pTexture_PlayerFaceEradicated;
      if ( pParty->pPartyBuffs[PARTY_BUFF_INVISIBILITY].uExpireTime )
        pRenderer->_4A6E7E(pPlayerPortraitsXCoords_For_PlayerBuffAnimsDrawing[i], 388, pPortrait);
      else
        pRenderer->DrawTextureTransparent(pPlayerPortraitsXCoords_For_PlayerBuffAnimsDrawing[i] + 1, 388, pPortrait);
      if ( pPlayer->pPlayerBuffs[PLAYER_BUFF_BLESS].uExpireTime | pPlayer->pPlayerBuffs[PLAYER_BUFF_HASTE].uExpireTime
         | pPlayer->pPlayerBuffs[PLAYER_BUFF_HEROISM].uExpireTime | pPlayer->pPlayerBuffs[PLAYER_BUFF_SHIELD].uExpireTime
         | pPlayer->pPlayerBuffs[PLAYER_BUFF_STONESKIN].uExpireTime )
        sub_441A4E(i);
      continue;
    }
    if (pPlayer->IsDead())
    {
      pPortrait = pTexture_PlayerFaceDead;
      if ( pParty->pPartyBuffs[PARTY_BUFF_INVISIBILITY].uExpireTime )
        pRenderer->_4A6E7E(pPlayerPortraitsXCoords_For_PlayerBuffAnimsDrawing[i], 388, pPortrait);
      else
        pRenderer->DrawTextureTransparent(pPlayerPortraitsXCoords_For_PlayerBuffAnimsDrawing[i] + 1, 388, pPortrait);
      if ( pPlayer->pPlayerBuffs[PLAYER_BUFF_BLESS].uExpireTime | pPlayer->pPlayerBuffs[PLAYER_BUFF_HASTE].uExpireTime
         | pPlayer->pPlayerBuffs[PLAYER_BUFF_HEROISM].uExpireTime | pPlayer->pPlayerBuffs[PLAYER_BUFF_SHIELD].uExpireTime
         | pPlayer->pPlayerBuffs[PLAYER_BUFF_STONESKIN].uExpireTime )
        sub_441A4E(i);
      continue;
    }
    face_expression_ID = 0;
    for ( uint j = 0; j < pPlayerFrameTable->uNumFrames; ++j )
      if ( pPlayerFrameTable->pFrames[j].expression == pPlayer->expression )
      {
        face_expression_ID = j;
        break;
      }
    if ( face_expression_ID == 0 )
      face_expression_ID = 1;
    if (pPlayer->expression == CHARACTER_EXPRESSION_21)
      pFrame = pPlayerFrameTable->GetFrameBy_y(&pPlayer->_expression21_frameset, &pPlayer->_expression21_animtime, pMiscTimer->uTimeElapsed);
    else
      pFrame = pPlayerFrameTable->GetFrameBy_x(face_expression_ID, pPlayer->uExpressionTimePassed);
    if (pPlayer->field_1AA2 != pFrame->uTextureID - 1 || _this )
    {
      pPlayer->field_1AA2 = pFrame->uTextureID - 1;
      pPortrait = (Texture *)pTextures_PlayerFaces[i][pPlayer->field_1AA2];//pFace = (Texture *)pTextures_PlayerFaces[i][pFrame->uTextureID];
      if ( pParty->pPartyBuffs[PARTY_BUFF_INVISIBILITY].uExpireTime )
        pRenderer->_4A6E7E(pPlayerPortraitsXCoords_For_PlayerBuffAnimsDrawing[i], 388, pPortrait);
      else
        pRenderer->DrawTextureTransparent(pPlayerPortraitsXCoords_For_PlayerBuffAnimsDrawing[i] + 1, 388, pPortrait);
      if ( pPlayer->pPlayerBuffs[PLAYER_BUFF_BLESS].uExpireTime | pPlayer->pPlayerBuffs[PLAYER_BUFF_HASTE].uExpireTime
         | pPlayer->pPlayerBuffs[PLAYER_BUFF_HEROISM].uExpireTime | pPlayer->pPlayerBuffs[PLAYER_BUFF_SHIELD].uExpireTime
         | pPlayer->pPlayerBuffs[PLAYER_BUFF_STONESKIN].uExpireTime )
        sub_441A4E(i);
      continue;
    }
  }
  if ( pParty->bTurnBasedModeOn == 1 )
  {
    if ( pTurnEngine->turn_stage != 1 )
    {
      if (PID_TYPE(pTurnEngine->pQueue[0].uPackedID) == OBJECT_Player)
      {
        if ( pTurnEngine->uActorQueueSize > 0 )
        {
          for (uint i = 0; i < (uint)pTurnEngine->uActorQueueSize; ++i)
          {
            if (PID_TYPE(pTurnEngine->pQueue[i].uPackedID) != OBJECT_Player)
              break;
            pTextureID = dword_5079D0;
            if ( pParty->uFlags & 0x10 )
              pTextureID = dword_5079CC;
            else
            {
              if ( pParty->uFlags & 0x20 )
                pTextureID = dword_5079C8;
            }
            pRenderer->DrawTextureTransparent(pPlayerPortraitsXCoords_For_PlayerBuffAnimsDrawing[PID_ID(pTurnEngine->pQueue[i].uPackedID)] - 4, 385, pIcons_LOD->GetTexture(pTextureID));
          }
        }
      }
    }
  }
  else
  {
    for (uint i = 0; i < 4; ++i)
    {
      if (pParty->pPlayers[i].CanAct() && !pParty->pPlayers[i].uTimeToRecovery)
      {
        pTextureID = dword_5079D0;
        if ( pParty->uFlags & 0x10 )
          pTextureID = dword_5079CC;
        else
        {
          if ( pParty->uFlags & 0x20 )
            pTextureID = dword_5079C8;
        }
        pRenderer->DrawTextureTransparent(pPlayerPortraitsXCoords_For_PlayerBuffAnimsDrawing[i] - 4, 385, pIcons_LOD->GetTexture(pTextureID));
      }
    }
  }
}

//----- (00441D38) --------------------------------------------------------
void GameUI_DrawMinimap(unsigned int uX, unsigned int uY, unsigned int uZ, unsigned int uW, unsigned int uZoom, unsigned int bRedrawOdmMinimap)
{
  int uHeight; // ebx@6
  unsigned int pW; // ebx@23
  int v15; // eax@23
  double v20; // st7@30
  signed int v27; // eax@37
  unsigned __int16 *v28; // ecx@37
  signed int v29; // edi@40
  int pPoint_X; // edi@72
  int pPoint_Y; // ebx@72
  unsigned int lPitch; // [sp+20h] [bp-34h]@1
  unsigned int pY; // [sp+20h] [bp-34h]@23
  unsigned int pX; // [sp+24h] [bp-30h]@23
  signed int v70; // [sp+24h] [bp-30h]@37
  signed int uBluea; // [sp+28h] [bp-2Ch]@37
  int v73; // [sp+2Ch] [bp-28h]@30
  signed int uCenterY; // [sp+48h] [bp-Ch]@1
  signed int uCenterX; // [sp+4Ch] [bp-8h]@1
  signed int uWidth; // [sp+5Ch] [bp+8h]@30
  unsigned int pZ; // [sp+60h] [bp+Ch]@23
  float uWb; // [sp+60h] [bp+Ch]@30
  unsigned int pColor;

  uCenterX = (uX + uZ) / 2;
  uCenterY = (uY + uW) / 2;
  lPitch = pRenderer->uTargetSurfacePitch;
  bool bWizardEyeActive = pParty->WizardEyeActive();
  int uWizardEyeSkillLevel = pParty->WizardEyeSkillLevel();
  if ( CheckHiredNPCSpeciality(Cartographer) )
  {
    bWizardEyeActive = true;
    uWizardEyeSkillLevel = 2;
  }
  extern bool wizard_eye;
  if ( wizard_eye )
  {
    bWizardEyeActive = true;
    uWizardEyeSkillLevel = 3;
  }
  pRenderer->SetRasterClipRect(uX, uY, uZ - 1, uW - 1);
  uHeight = uW - uY;
  uWidth = uZ - uX;

  if ( uCurrentlyLoadedLevelType == LEVEL_Outdoor)
  {
    uchar* pMapLod0 = pIcons_LOD->pTextures[viewparams->uTextureID_LocationMap].pLevelOfDetail0_prolly_alpha_mask;
    ushort* pPal = pIcons_LOD->pTextures[viewparams->uTextureID_LocationMap].pPalette16;
    v73 = (1 << (pIcons_LOD->pTextures[viewparams->uTextureID_LocationMap].uWidthLn2 + 16)) / (signed int)uZoom;
    v20 = (double)(pParty->vPosition.x + 32768) / (double)(1 << (16 - pIcons_LOD->pTextures[viewparams->uTextureID_LocationMap].uWidthLn2));
    uWb = (double)(32768 - pParty->vPosition.y) / (double)(1 << (16 - pIcons_LOD->pTextures[viewparams->uTextureID_LocationMap].uWidthLn2));
    switch (uZoom)
    {
      case 512:
      {
        v20 = v20 - (double)(uWidth / 2);
        uWb = uWb - (double)(uHeight / 2);
      }
      break;
      case 1024:
      {
        v20 = v20 - (double)(uWidth / 4);
        uWb = uWb - (double)(uHeight / 4);
      }
      break;
      case 2048:
      {
        v20 = v20 - (double)(uWidth / 8);
        uWb = uWb - (double)(uHeight / 8);
      }
      break;
      default: assert(false);
    }
    assert(sizeof(pOdmMinimap) == 137 * 117 * sizeof(short));

    v70 = floorf(v20 * 65536.0 + 0.5f);//LODWORD(v24);
    uBluea = floorf(uWb * 65536.0 + 0.5f);//LODWORD(v25);
    v27 = uBluea >> 16;
    v28 = &pRenderer->pTargetSurface[uX + uY * lPitch];
    if (pMapLod0 && bRedrawOdmMinimap)
    {
      assert(uWidth == 137 && uHeight == 117);
      //auto pMinimap = (unsigned __int16 *)pOdmMinimap;

      ushort mapWidth = pIcons_LOD->pTextures[viewparams->uTextureID_LocationMap].uTextureWidth;

      v29 = v70 >> 16;
      for (int y = 0; y < uHeight; ++y)
      {
        uchar* pMapLod0Line = &pMapLod0[v27 * mapWidth];
        for (int x = 0; x < uWidth; ++x)
        {
          //*pMinimap++ = pPal[pMapLod0Line[v29]];
          pOdmMinimap[y][x] = pPal[pMapLod0Line[v29]];
          v29 = (v70 + x * v73) >> 16;
        }
        v29 = v70 >> 16;
        v28 += 137 - uWidth;
        uBluea += v73;
        v27 = uBluea >> 16;
      }
    }

    for (int y = 0; y < 117; ++y)
    {
      for (int x = 0; x < 137; ++x)
      {
        *v28++ = pOdmMinimap[y][x];
      }
      v28 += lPitch - 137;
    }
    uNumBlueFacesInBLVMinimap = 0;
  }
  else// uCurrentlyLoadedLevelType == LEVEL_Indoor
  {
    pRenderer->FillRectFast(uX, uY, uZ - uX, uHeight, 0xF);
    uNumBlueFacesInBLVMinimap = 0;

    for (uint i = 0; i < (uint)pIndoor->pMapOutlines->uNumOutlines; ++i)
    {
      BLVMapOutline* pOutline = &pIndoor->pMapOutlines->pOutlines[i];
      BLVFace* pFace1 = pIndoor->pFaces + pOutline->uFace1ID;
      BLVFace* pFace2 = pIndoor->pFaces + pOutline->uFace2ID;
      //v9 = pIndoor->pFaces[pMapVertex->uFace1ID].uAttributes;
        //v10 = pIndoor->pFaces[pMapVertex->uFace2ID].uAttributes;
      if (pFace1->Visible() && pFace2->Visible())
      {
        if ( pOutline->uFlags & 1 )
          goto LABEL_15;
        if (pFace1->uAttributes & 0x80 || pFace2->uAttributes & 0x80)
          goto LABEL_ABC;
      }
      continue;

LABEL_ABC:
      pOutline->uFlags = pOutline->uFlags | 1;
      pIndoor->_visible_outlines[i >> 3] |= 1 << (7 - i % 8);

LABEL_15:
      //v12 = &pIndoor->pFaces[pOutline->uFace1ID];
      if (bWizardEyeActive && uWizardEyeSkillLevel >= 3 &&
          (pFace1->uAttributes & 0x2000 || pFace2->uAttributes & 0x2000) &&
          (pIndoor->pFaceExtras[pFace1->uFaceExtraID].uEventID || pIndoor->pFaceExtras[pFace2->uFaceExtraID].uEventID))
      {
        if (uNumBlueFacesInBLVMinimap < 49)
          pBlueFacesInBLVMinimapIDs[uNumBlueFacesInBLVMinimap++] = i;
      }
      else
      {
        long long _a = (uZoom * (signed __int64)pIndoor->pVertices[pOutline->uVertex1ID].x);
        uint _b = ((unsigned int)((unsigned __int64)_a >> 16) << 16);
        int _c = ((signed int)(_b - uZoom * pParty->vPosition.x) >> 16);
        pX = uCenterX + ((signed int)(((unsigned int)((unsigned __int64)(uZoom * (signed __int64)pIndoor->pVertices[pOutline->uVertex1ID].x) >> 16) << 16) - uZoom * pParty->vPosition.x) >> 16);
        pY = uCenterY - ((signed int)(((unsigned int)((unsigned __int64)(uZoom * (signed __int64)pIndoor->pVertices[pOutline->uVertex1ID].y) >> 16) << 16) - uZoom * pParty->vPosition.y) >> 16);
        pZ = uCenterX + ((signed int)(((unsigned int)((unsigned __int64)(uZoom * (signed __int64)pIndoor->pVertices[pOutline->uVertex2ID].x) >> 16) << 16) - uZoom * pParty->vPosition.x) >> 16);
        pW = uCenterY - ((signed int)(((unsigned int)((unsigned __int64)(uZoom * (signed __int64)pIndoor->pVertices[pOutline->uVertex2ID].y) >> 16) << 16) - uZoom * pParty->vPosition.y) >> 16);
        v15 = abs(pOutline->sZ - pParty->vPosition.z) / 8;
        if ( v15 > 100 )
          v15 = 100;
        pRenderer->RasterLine2D(pX, pY, pZ, pW, viewparams->pPalette[-v15 + 200]);
      }
    }

    for (uint i = 0; i < uNumBlueFacesInBLVMinimap; ++i)
    {
      BLVMapOutline* pOutline = &pIndoor->pMapOutlines->pOutlines[pBlueFacesInBLVMinimapIDs[i]];
      pX = uCenterX + ((signed int)(((unsigned int)((unsigned __int64)((signed int)uZoom * (signed __int64)pIndoor->pVertices[pOutline->uVertex1ID].x) >> 16) << 16) - uZoom * pParty->vPosition.x) >> 16);
      pY = uCenterY - ((signed int)(((unsigned int)((unsigned __int64)((signed int)uZoom * (signed __int64)pIndoor->pVertices[pOutline->uVertex1ID].y) >> 16) << 16) - uZoom * pParty->vPosition.y) >> 16);
      pZ = uCenterX + ((signed int)(((unsigned int)((unsigned __int64)((signed int)uZoom * (signed __int64)pIndoor->pVertices[pOutline->uVertex2ID].x) >> 16) << 16) - uZoom * pParty->vPosition.x) >> 16);
      pW = uCenterY - ((signed int)(((unsigned int)((unsigned __int64)((signed int)uZoom * (signed __int64)pIndoor->pVertices[pOutline->uVertex2ID].y) >> 16) << 16) - uZoom * pParty->vPosition.y) >> 16);
      pRenderer->RasterLine2D(pX, pY, pZ, pW, ui_game_minimap_outline_color);
    }
  }

  //draw arrow on the minimap(include. Ritor1)
  uint arrow_idx;
  unsigned int rotate = pParty->sRotationY & stru_5C6E00->uDoublePiMask;
  if ( (signed int)rotate <= 1920 )
    arrow_idx = 6;
  if ( (signed int)rotate < 1664 )
    arrow_idx = 5;
  if ( (signed int)rotate <= 1408 )
    arrow_idx = 4;
  if ( (signed int)rotate < 1152 )
    arrow_idx = 3;
  if ( (signed int)rotate <= 896 )
    arrow_idx = 2;
  if ( (signed int)rotate < 640 )
    arrow_idx = 1;
  if ( (signed int)rotate <= 384 )
    arrow_idx = 0;
  if ( (signed int)rotate < 128 || (signed int)rotate > 1920 )
    arrow_idx = 7;
  pRenderer->DrawTextureTransparent(uCenterX - 3, uCenterY - 3, pIcons_LOD->GetTexture(pTextureIDs_pMapDirs[arrow_idx]));//стрелка

  //draw objects on the minimap
  if ( bWizardEyeActive )
  {
    if ( uWizardEyeSkillLevel >= 2 )
    {
      for ( uint i = 0; i < uNumSpriteObjects; ++i )
      {
        if ( !pSpriteObjects[i].uType || !pSpriteObjects[i].uObjectDescID )
          continue;
        //if (uWizardEyeSkillLevel == 1
        pPoint_X = uCenterX + ((unsigned __int64)((pSpriteObjects[i].vPosition.x - pParty->vPosition.x) * (signed __int64)uZoom) >> 16);
        pPoint_Y = uCenterY - ((signed __int64)((pSpriteObjects[i].vPosition.y - pParty->vPosition.y) * (signed __int64)uZoom) >> 16);
        if ( pPoint_X >= pRenderer->raster_clip_x && pPoint_X <= pRenderer->raster_clip_z &&
             pPoint_Y >= pRenderer->raster_clip_y && pPoint_Y <= pRenderer->raster_clip_w)
        {
          if (pObjectList->pObjects[pSpriteObjects[i].uObjectDescID].uFlags & OBJECT_DESC_UNPICKABLE)
          {
            pRenderer->RasterLine2D(pPoint_X, pPoint_Y, pPoint_X, pPoint_Y, ui_game_minimap_projectile_color);
          }
          else if ( uZoom > 512 )
          {
            pRenderer->RasterLine2D(pPoint_X - 2, pPoint_Y,     pPoint_X - 2, pPoint_Y + 1, ui_game_minimap_treasure_color);
            pRenderer->RasterLine2D(pPoint_X - 1, pPoint_Y - 1, pPoint_X - 1, pPoint_Y + 1, ui_game_minimap_treasure_color);
            pRenderer->RasterLine2D(pPoint_X,     pPoint_Y - 2, pPoint_X,     pPoint_Y + 1, ui_game_minimap_treasure_color);
            pRenderer->RasterLine2D(pPoint_X + 1, pPoint_Y - 1, pPoint_X + 1, pPoint_Y + 1, ui_game_minimap_treasure_color);
            pRenderer->RasterLine2D(pPoint_X + 2, pPoint_Y,     pPoint_X + 2, pPoint_Y + 1, ui_game_minimap_treasure_color);
          }
          else
          {
            pRenderer->RasterLine2D(pPoint_X - 1, pPoint_Y - 1, pPoint_X - 1, pPoint_Y, ui_game_minimap_treasure_color);
            pRenderer->RasterLine2D(pPoint_X,     pPoint_Y - 1, pPoint_X,     pPoint_Y, ui_game_minimap_treasure_color);
          }
        }
      }
    }
    for ( uint i = 0; i < uNumActors; ++i )//draw actors(отрисовка монстров и нпс)
    {
      if ( pActors[i].uAIState != Removed && pActors[i].uAIState != Disabled
       && (pActors[i].uAIState == Dead || BYTE1(pActors[i].uAttributes) & 0x80) )
      {
        pPoint_X = uCenterX + ((unsigned __int64)(( pActors[i].vPosition.x - pParty->vPosition.x) * (signed __int64)(signed int)uZoom) >> 16);
        pPoint_Y = uCenterY - ((unsigned __int64)(( pActors[i].vPosition.y - pParty->vPosition.y) * (signed __int64)(signed int)uZoom) >> 16);
        if ( pPoint_X >= pRenderer->raster_clip_x && pPoint_X <= pRenderer->raster_clip_z
          && pPoint_Y >= pRenderer->raster_clip_y && pPoint_Y <= pRenderer->raster_clip_w )
        {
          pColor = ui_game_minimap_actor_friendly_color;
          if ( BYTE3(pActors[i].uAttributes) & 1 )
            pColor = ui_game_minimap_actor_hostile_color;
          if ( pActors[i].uAIState == Dead)
            pColor = ui_game_minimap_actor_corpse_color;
          if ( uZoom > 1024 )
          {
            pRenderer->RasterLine2D(pPoint_X - 2, pPoint_Y - 1, pPoint_X - 2, pPoint_Y + 1, pColor);
            pRenderer->RasterLine2D(pPoint_X - 1, pPoint_Y - 2, pPoint_X - 1, pPoint_Y + 2, pColor);
            pRenderer->RasterLine2D(pPoint_X,     pPoint_Y - 2, pPoint_X,     pPoint_Y + 2, pColor);
            pRenderer->RasterLine2D(pPoint_X + 1, pPoint_Y - 2, pPoint_X + 1, pPoint_Y + 2, pColor);
            pRenderer->RasterLine2D(pPoint_X + 2, pPoint_Y - 1, pPoint_X + 2, pPoint_Y + 1, pColor);
          }
          else
          {
            pRenderer->RasterLine2D(pPoint_X - 1, pPoint_Y - 1, pPoint_X - 1, pPoint_Y, pColor);
            pRenderer->RasterLine2D(pPoint_X,     pPoint_Y - 1, pPoint_X,     pPoint_Y, pColor);
          }
        }
      }
    }
    for ( uint i = 0; i < (signed int)uNumLevelDecorations; ++i )//draw items(отрисовка предметов)
    {
      if ( pLevelDecorations[i].uFlags & 8 )
      {
        pPoint_X = uCenterX + ((unsigned __int64)((pLevelDecorations[i].vPosition.x - pParty->vPosition.x) * (signed __int64)(signed int)uZoom) >> 16);
        pPoint_Y = uCenterY - ((unsigned __int64)((pLevelDecorations[i].vPosition.y - pParty->vPosition.y) * (signed __int64)(signed int)uZoom) >> 16);
        if ( pPoint_X >= pRenderer->raster_clip_x && pPoint_X <= pRenderer->raster_clip_z
          && pPoint_Y >= pRenderer->raster_clip_y && pPoint_Y <= pRenderer->raster_clip_w )
        {
          if ( (signed int)uZoom > 512 )
          {
            pRenderer->RasterLine2D(pPoint_X - 1, pPoint_Y - 1, pPoint_X - 1, pPoint_Y + 1, ui_game_minimap_decoration_color_1);
            pRenderer->RasterLine2D(pPoint_X,     pPoint_Y - 1, pPoint_X,     pPoint_Y + 1, ui_game_minimap_decoration_color_1);
            pRenderer->RasterLine2D(pPoint_X + 1, pPoint_Y - 1, pPoint_X + 1, pPoint_Y + 1, ui_game_minimap_decoration_color_1);
          }
          else
            pRenderer->RasterLine2D(pPoint_X, pPoint_Y, pPoint_X, pPoint_Y, ui_game_minimap_decoration_color_1);
        }
      }
    }
  }
  pRenderer->DrawTextureTransparent(468, 0, pIcons_LOD->GetTexture(uTextureID_Minimap_Loop));//draw minimap_loop
  pRenderer->SetTextureClipRect(541, 0, 567, 480);
  pRenderer->DrawTextureIndexed(floorf(((double)pParty->sRotationY * 0.1171875) + 0.5f) + 285, 136, pIcons_LOD->GetTexture(uTextureID_Compas));//draw compas
  pRenderer->ResetTextureClipRect();
}

//----- (00441498) --------------------------------------------------------
void  GameUI_DrawTorchlightAndWizardEye()
{
  if (pCurrentScreen == SCREEN_GAME
      || pCurrentScreen == SCREEN_MENU
      || pCurrentScreen == SCREEN_OPTIONS
      || pCurrentScreen == SCREEN_REST
      || pCurrentScreen == SCREEN_SPELL_BOOK
      || pCurrentScreen == SCREEN_CHEST
      || pCurrentScreen == SCREEN_SAVEGAME
      || pCurrentScreen == SCREEN_LOADGAME
      || pCurrentScreen == SCREEN_CHEST_INVENTORY
      || pCurrentScreen == SCREEN_BOOKS
      || pCurrentScreen == SCREEN_BRANCHLESS_NPC_DIALOG )
  {
    if (pParty->TorchlightActive())
    {
      IconFrame* icon = pIconsFrameTable->GetFrame((signed __int16)pUIAnum_Torchlight->uIconID, pEventTimer->Time());
      pRenderer->DrawTextureTransparent(pUIAnum_Torchlight->x, pUIAnum_Torchlight->y, pIcons_LOD->GetTexture(icon->uTextureID));
    }
    if (pParty->WizardEyeActive())
    {
      IconFrame* icon = pIconsFrameTable->GetFrame((signed __int16)pUIAnim_WizardEye->uIconID, pEventTimer->Time());
      pRenderer->DrawTextureTransparent(pUIAnim_WizardEye->x, pUIAnim_WizardEye->y, pIcons_LOD->GetTexture(icon->uTextureID));
    }
  }
}
// 4E28F8: using guessed type int pCurrentScreen;


//----- (00491F87) --------------------------------------------------------
void GameUI_DrawHiredNPCs()
{
  unsigned int v13; // eax@23
  char pContainer[20]; // [sp+Ch] [bp-30h]@18
  signed int uFrameID; // [sp+24h] [bp-18h]@19
  int v22; // [sp+34h] [bp-8h]@2
  unsigned __int8 pNPC_limit_ID; // [sp+3Bh] [bp-1h]@2

  if ( bNoNPCHiring != 1 )
  {
    pNPC_limit_ID = 0;
    v22 = 0;
    if (pParty->pHirelings[0].pName)
      pTmpBuf[v22++] = 0;
    if (pParty->pHirelings[1].pName)
      pTmpBuf[v22++] = 1;

    for (uint i = 0; i < pNPCStats->uNumNewNPCs; ++i)
    {
      if (pNPCStats->pNewNPCData[i].uFlags & 128)
      {
        if (!pParty->pHirelings[0].pName || strcmp(pNPCStats->pNewNPCData[i].pName, pParty->pHirelings[0].pName))
        {
          if (!pParty->pHirelings[1].pName || strcmp(pNPCStats->pNewNPCData[i].pName, pParty->pHirelings[1].pName))
            pTmpBuf[v22++] = i + 2;
        }
      }
    }

    for ( int i = pParty->hirelingScrollPosition; i < v22 && pNPC_limit_ID < 2; i++ )
    {
      if ( (unsigned __int8)pTmpBuf[i] >= 2 )
      {
        sprintf(pContainer, "NPC%03d", pNPCStats->pNPCData[(unsigned __int8)pTmpBuf[i] + 499].uPortraitID);
        pRenderer->DrawTextureIndexed(pHiredNPCsIconsOffsetsX[pNPC_limit_ID], pHiredNPCsIconsOffsetsY[pNPC_limit_ID],
                      pIcons_LOD->GetTexture(pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE)));
      }
      else
      {
        sprintf(pContainer, "NPC%03d", pParty->pHirelings[(unsigned __int8)pTmpBuf[i]].uPortraitID);
        pRenderer->DrawTextureIndexed(pHiredNPCsIconsOffsetsX[pNPC_limit_ID], pHiredNPCsIconsOffsetsY[pNPC_limit_ID],
                   pIcons_LOD->GetTexture(pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE)));
        if ( pParty->pHirelings[(unsigned __int8)pTmpBuf[i]].evt_A == 1 )
        {
          uFrameID = pParty->pHirelings[(unsigned __int8)pTmpBuf[i]].evt_B;
          v13 = 0;
          if (pIconsFrameTable->uNumIcons)
          {
            for ( v13 = 0; v13 < pIconsFrameTable->uNumIcons; ++v13 )
            {
              if ( !_stricmp("spell96", pIconsFrameTable->pIcons[v13].pAnimationName) )
                break;
            }
          }
          pRenderer->DrawTextureTransparent(pHiredNPCsIconsOffsetsX[pNPC_limit_ID], pHiredNPCsIconsOffsetsY[pNPC_limit_ID],
                        &pIcons_LOD->pTextures[pIconsFrameTable->GetFrame(v13, uFrameID)->uTextureID]);
        }
      }
      ++pNPC_limit_ID;
    }
  }
}
// 6BE3C5: using guessed type char bNoNPCHiring;

//----- (004178FE) --------------------------------------------------------
unsigned int UI_GetHealthManaAndOtherQualitiesStringColor(signed int current_pos, signed int base_pos)
{
  unsigned __int16 R, G, B;

  if ( current_pos <= base_pos )
  {
    if ( current_pos == base_pos )//White
      return 0;
    if ( 100 * current_pos / base_pos >= 25 )//Yellow( current_pos > 1/4 )
      R = 255, G = 255, B = 100;
    else//Red( current_pos < 1/4 )
      R = 255, G = 0, B = 0;
  }
  else//Green
    R = 0, G = 255, B = 0;
  return TargetColor(R, G, B);
}

//----- (00417939) --------------------------------------------------------
int GetConditionDrawColor(unsigned int uConditionIdx)
{
  switch (uConditionIdx)
  {
    case Condition_Zombie:
    case Condition_Good:
      return ui_character_condition_normal_color;

    case Condition_Cursed:
    case Condition_Weak:
    case Condition_Fear:
    case Condition_Drunk:
    case Condition_Insane:
    case Condition_Poison1:
    case Condition_Disease1:
      return ui_character_condition_light_color;

    case Condition_Sleep:
    case Condition_Poison2:
    case Condition_Disease2:
    case Condition_Paralyzed:
    case Condition_Unconcious:
      return ui_character_condition_moderate_color;

    case Condition_Poison3:
    case Condition_Disease3:
    case Condition_Dead:
    case Condition_Pertified:
    case Condition_Eradicated:
      return ui_character_condition_severe_color;
  }
  Error("Invalid condition (%u)", uConditionIdx);
}
