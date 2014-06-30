#define _CRT_SECURE_NO_WARNINGS
#include "..\mm7_unsorted_subs.h"
#include "..\GUIWindow.h"
#include "..\GUIFont.h"
#include "..\Party.h"
#include "..\AudioPlayer.h"
#include "..\Outdoor.h"
#include "..\LOD.h"
#include "..\Viewport.h"
#include "..\Timer.h"
#include "..\texts.h"

#include "..\mm7_data.h"



//----- (0041F6C1) --------------------------------------------------------
void RestUI_Load()
{
  if ( !dword_506F14 )
    pAudioPlayer->StopChannels(-1, -1);
  if ( pCurrentScreen != SCREEN_GAME)
  {
    pGUIWindow_CurrentMenu->Release();
    pCurrentScreen = SCREEN_GAME;
    viewparams->bRedrawGameUI = true;
  }
  pEventTimer->Pause();
  if ( dword_506F14 != 2 )
    GUIWindow::Create(518, 450, 0, 0, WINDOW_PressedButton2, (int)pBtn_Rest, 0);
  _506F18_num_minutes_to_sleep = 0;
  dword_506F14 = 0;
  uRestUI_FoodRequiredToRest = 2;
  if ( uCurrentlyLoadedLevelType == LEVEL_Outdoor)
    uRestUI_FoodRequiredToRest = pOutdoor->GetNumFoodRequiredToRestInCurrentPos(pParty->vPosition.x, pParty->vPosition.y, pParty->vPosition.z);
  if ( PartyHasDragon() )
  {
    for ( uint i = 0; i < 4; ++i )
    {
      if (pParty->pPlayers[i].classType == PLAYER_CLASS_WARLOCK)
        ++uRestUI_FoodRequiredToRest;
    }
  }
  if ( CheckHiredNPCSpeciality(Porter) )
    --uRestUI_FoodRequiredToRest;
  if ( CheckHiredNPCSpeciality(QuarterMaster) )
    uRestUI_FoodRequiredToRest -= 2;
  if ( CheckHiredNPCSpeciality(Gypsy) )
    --uRestUI_FoodRequiredToRest;
  if ( uRestUI_FoodRequiredToRest < 1 )
    uRestUI_FoodRequiredToRest = 1;
  if ( !_stricmp(pCurrentMapName, "d29.blv") && _449B57_test_bit(pParty->_quest_bits, 98) )//Замок Хармондейл
    uRestUI_FoodRequiredToRest = 0;

  ++pIcons_LOD->uTexturePacksCount;
  if ( !pIcons_LOD->uNumPrevLoadedFiles )
    pIcons_LOD->uNumPrevLoadedFiles = pIcons_LOD->uNumLoadedFiles;

  pCurrentScreen = SCREEN_REST;
  _507CD4_RestUI_hourglass_anim_controller = 0;
  uTextureID_RestUI_restmain = pIcons_LOD->LoadTexture("restmain", TEXTURE_16BIT_PALETTE);
  uTextureID_RestUI_restb1 = pIcons_LOD->LoadTexture("restb1", TEXTURE_16BIT_PALETTE);
  uTextureID_RestUI_restb2 = pIcons_LOD->LoadTexture("restb2", TEXTURE_16BIT_PALETTE);
  uTextureID_RestUI_restb3 = pIcons_LOD->LoadTexture("restb3", TEXTURE_16BIT_PALETTE);
  uTextureID_RestUI_restb4 = pIcons_LOD->LoadTexture("restb4", TEXTURE_16BIT_PALETTE);
  uTextureID_RestUI_restexit = pIcons_LOD->LoadTexture("restexit", TEXTURE_16BIT_PALETTE);
  OutdoorLocation::LoadActualSkyFrame();
  pGUIWindow_CurrentMenu = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_Rest, 0, 0);
  pButton_RestUI_Exit          = pGUIWindow_CurrentMenu->CreateButton(280, 297, 154, 37, 1, 0, UIMSG_ExitRest,       0,   0, "", pIcons_LOD->GetTexture(uTextureID_RestUI_restexit), 0);
  pButton_RestUI_Main          = pGUIWindow_CurrentMenu->CreateButton( 24, 154, 225, 37, 1, 0, UIMSG_Rest8Hour,      0, 'R', "", pIcons_LOD->GetTexture(uTextureID_RestUI_restb4), 0);
  pButton_RestUI_WaitUntilDawn = pGUIWindow_CurrentMenu->CreateButton( 61, 232, 154, 33, 1, 0, UIMSG_AlreadyResting, 0, 'D', "", pIcons_LOD->GetTexture(uTextureID_RestUI_restb1), 0);
  pButton_RestUI_Wait1Hour     = pGUIWindow_CurrentMenu->CreateButton( 61, 264, 154, 33, 1, 0, UIMSG_Wait1Hour,      0, 'H', "", pIcons_LOD->GetTexture(uTextureID_RestUI_restb2), 0);
  pButton_RestUI_Wait5Minutes  = pGUIWindow_CurrentMenu->CreateButton( 61, 296, 154, 33, 1, 0, UIMSG_Wait5Minutes,   0, 'M', "", pIcons_LOD->GetTexture(uTextureID_RestUI_restb3), 0);
}

//----- (0041FA01) --------------------------------------------------------
void RestUI_Draw()
{
  int live_characters; // esi@1
  unsigned int v3; // eax@15
  GUIButton tmp_button; // [sp+8h] [bp-DCh]@19
  unsigned int am_pm_hours; // [sp+D8h] [bp-Ch]@9

  live_characters = 0;
  for( int i = 1; i < 5; ++i )
    if ( !pPlayers[i]->IsDead() && !pPlayers[i]->IsEradicated() && pPlayers[i]->sHealth > 0 )
      ++live_characters;

  if ( live_characters )
  {
    pRenderer->DrawTextureIndexed(8, 8, pIcons_LOD->GetTexture(uTextureID_RestUI_restmain));
    am_pm_hours = pParty->uCurrentHour;
    dword_506F1C = pGUIWindow_CurrentMenu->pCurrentPosActiveItem;
    if ( (signed int)pParty->uCurrentHour <= 12 )
    {
      if ( !am_pm_hours )
        am_pm_hours = 12;
    }
    else
      am_pm_hours -= 12;
    pRenderer->DrawTextureIndexed(16, 26, pTexture_RestUI_CurrentSkyFrame);
    if ( pTexture_RestUI_CurrentHourglassFrame )
    {
      pTexture_RestUI_CurrentHourglassFrame->Release();
      pIcons_LOD->SyncLoadedFilesCount();
    }
    v3 = pEventTimer->uTimeElapsed + _507CD4_RestUI_hourglass_anim_controller;
    _507CD4_RestUI_hourglass_anim_controller += pEventTimer->uTimeElapsed;
    if ( (unsigned int)_507CD4_RestUI_hourglass_anim_controller >= 512 )
    {
      v3 = 0;
      _507CD4_RestUI_hourglass_anim_controller = 0;
    }
    hourglass_icon_idx = (int)floorf(((double)v3 / 512.0 * 120.0) + 0.5f) % 256 + 1;
    if (hourglass_icon_idx >= 120 )
      hourglass_icon_idx = 1;

    sprintf(pTmpBuf.data(), "hglas%03d", hourglass_icon_idx);
    pTexture_RestUI_CurrentHourglassFrame = pIcons_LOD->LoadTexturePtr(pTmpBuf.data(), TEXTURE_16BIT_PALETTE);
    pRenderer->DrawTextureIndexed(267, 159, pTexture_RestUI_CurrentHourglassFrame);
    memset(&tmp_button, 0, sizeof(GUIButton));
    tmp_button.uX = 24;
    tmp_button.uY = 154;
    tmp_button.uZ = 194;
    tmp_button.uW = 190;
    tmp_button.uWidth = 171;
    tmp_button.uHeight = 37;
    tmp_button.pParent = pButton_RestUI_WaitUntilDawn->pParent;
    tmp_button.DrawLabel(pGlobalTXT_LocalizationStrings[183], pFontCreate, Color16(10, 0, 0), Color16(230, 214, 193));//Отдых и лечение 8 часов
    tmp_button.pParent = 0;

    sprintf(pTmpBuf.data(), "\r408%d", uRestUI_FoodRequiredToRest);
    pGUIWindow_CurrentMenu->DrawText(pFontCreate, 0, 164, Color16(10, 0, 0), pTmpBuf.data(), 0, 0, Color16(230, 214, 193));

    pButton_RestUI_WaitUntilDawn->DrawLabel(pGlobalTXT_LocalizationStrings[237], pFontCreate, Color16(10, 0, 0), Color16(230, 214, 193));//Ждать до рассвета
    pButton_RestUI_Wait1Hour->DrawLabel(pGlobalTXT_LocalizationStrings[239], pFontCreate, Color16(10, 0, 0), Color16(230, 214, 193));//Ждать 1 час
    pButton_RestUI_Wait5Minutes->DrawLabel(pGlobalTXT_LocalizationStrings[238], pFontCreate, Color16(10, 0, 0), Color16(230, 214, 193));//Ждать 5 минут
    pButton_RestUI_Exit->DrawLabel(pGlobalTXT_LocalizationStrings[81], pFontCreate, Color16(10, 0, 0), Color16(230, 214, 193));//Закончить отдыхать
    memset(&tmp_button, 0, sizeof(GUIButton));
    tmp_button.uX = 45;
    tmp_button.uY = 199;

    tmp_button.uZ = 229;
    tmp_button.uW = 228;

    tmp_button.uWidth = 185;
    tmp_button.uHeight = 30;

    tmp_button.pParent = pButton_RestUI_WaitUntilDawn->pParent;
    tmp_button.DrawLabel(pGlobalTXT_LocalizationStrings[236], pFontCreate, Color16(10, 0, 0), Color16(230, 214, 193));//Ждать без лечения
    tmp_button.pParent = 0;
    sprintf(pTmpBuf.data(), "%d:%02d %s", am_pm_hours, pParty->uCurrentMinute, aAMPMNames[(pParty->uCurrentHour >= 12 && pParty->uCurrentHour < 24)? 1:0]);
    pGUIWindow_CurrentMenu->DrawText(pFontCreate, 368, 168, Color16(10, 0, 0), pTmpBuf.data(), 0, 0, Color16(230, 214, 193));
    sprintf(pTmpBuf.data(), "%s\r190%d", pGlobalTXT_LocalizationStrings[56], pParty->uDaysPlayed + 1);//День
    pGUIWindow_CurrentMenu->DrawText(pFontCreate, 350, 190, Color16(10, 0, 0), pTmpBuf.data(), 0, 0, Color16(230, 214, 193));
    sprintf(pTmpBuf.data(), "%s\r190%d", pGlobalTXT_LocalizationStrings[146], pParty->uCurrentMonth + 1);//Месяц
    pGUIWindow_CurrentMenu->DrawText(pFontCreate, 350, 222, Color16(10, 0, 0), pTmpBuf.data(), 0, 0, Color16(230, 214, 193));
    sprintf(pTmpBuf.data(), "%s\r190%d", pGlobalTXT_LocalizationStrings[245], pParty->uCurrentYear);//Год
    pGUIWindow_CurrentMenu->DrawText(pFontCreate, 350, 254, Color16(10, 0, 0), pTmpBuf.data(), 0, 0, Color16(230, 214, 193));
    if ( dword_506F14 )
      Party::Sleep6Hours();
  }
  else
    GUIWindow::Create(pButton_RestUI_Exit->uX, pButton_RestUI_Exit->uY, 0, 0, WINDOW_CloseRestWindowBtn, 
      (int)pButton_RestUI_Exit, pGlobalTXT_LocalizationStrings[81]); // "Exit Rest"
}