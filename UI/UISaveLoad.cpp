#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <io.h>
#include "..\MM7.h"

#include "..\Keyboard.h"

#include "..\MapInfo.h"
#include "..\GUIWindow.h"
#include "..\GUIFont.h"
#include "..\Render.h"
#include "..\LOD.h"
#include "..\SaveLoad.h"
#include "..\texts.h"

#include "..\mm7_data.h"


//----- (004601B7) --------------------------------------------------------
static void UI_DrawSaveLoad(bool save)
{
  const char *pSlotName; // edi@36
  GUIWindow save_load_window; // [sp+Ch] [bp-78h]@8
  unsigned int pSaveFiles; // [sp+70h] [bp-14h]@10
  unsigned __int64 full_hours;
  unsigned __int64 full_days;
  int full_weeks;
  int full_month;
  int current_year;
  int current_month;
  int current_day;
  int current_hour;
  int current_minutes;

  pRenderer->BeginScene();
  if ( GetCurrentMenuID() != MENU_SAVELOAD && GetCurrentMenuID() != MENU_LoadingProcInMainMenu )
  {
    pRenderer->DrawTextureIndexed(8, 8, pIcons_LOD->GetTexture(uTextureID_loadsave));
    if (save)
    {
      pRenderer->DrawTextureIndexed(241, 302, pIcons_LOD->GetTexture(uTextureID_LS_saveU));
      pRenderer->DrawTextureIndexed( 18, 139, pIcons_LOD->GetTexture(uTextureID_save_up));
    }
    else
    {
      pRenderer->DrawTextureIndexed(241, 302, pIcons_LOD->GetTexture(uTextureID_LS_loadU));
      pRenderer->DrawTextureIndexed( 18, 139, pIcons_LOD->GetTexture(uTextureID_load_up));
    }
    pRenderer->DrawTextureIndexed(351, 302, pIcons_LOD->GetTexture(uTextureID_x_u));
  }
  if ( pSavegameUsedSlots[uLoadGameUI_SelectedSlot] )
  {
    memset(&save_load_window, 0, 0x54);
    save_load_window.uFrameX = pGUIWindow_CurrentMenu->uFrameX + 240;
    save_load_window.uFrameWidth = 220;
    save_load_window.uFrameY = (pGUIWindow_CurrentMenu->uFrameY - pFontSmallnum->uFontHeight) + 157;
    save_load_window.uFrameZ = save_load_window.uFrameX + 219;
    save_load_window.uFrameHeight = pFontSmallnum->uFontHeight;
    save_load_window.uFrameW = pFontSmallnum->uFontHeight + save_load_window.uFrameY - 1;
    if ( pSavegameThumbnails[uLoadGameUI_SelectedSlot].pPixels )
      pRenderer->DrawTextureRGB(pGUIWindow_CurrentMenu->uFrameX + 276, pGUIWindow_CurrentMenu->uFrameY + 171, &pSavegameThumbnails[uLoadGameUI_SelectedSlot]);
//Draw map name
    save_load_window.DrawTitleText(pFontSmallnum, 0, 0, 0, pMapStats->pInfos[pMapStats->GetMapInfo(pSavegameHeader[uLoadGameUI_SelectedSlot].pLocationName)].pName, 3);
//Draw date
    full_hours = ((signed __int64)(pSavegameHeader[uLoadGameUI_SelectedSlot].uWordTime * 0.234375) / 60) / 60i64;
    full_days = (unsigned int)full_hours / 24;
    full_weeks = (unsigned int)(full_days / 7);
    full_month = (unsigned int)full_weeks / 4;
    current_year = (full_month / 12) + game_starting_year;
    current_month = full_month % 12;
    current_day = full_days % 28;
    current_hour = full_hours % 24;
    current_minutes = (((signed __int64)(pSavegameHeader[uLoadGameUI_SelectedSlot].uWordTime * 0.234375) / 60) % 60i64);

    save_load_window.uFrameY = pGUIWindow_CurrentMenu->uFrameY + 261;
    int am;
    if ( (signed int)current_hour >= 12 )
    {
      current_hour -= 12;
      if ( !current_hour )
        current_hour = 12;
      am = 1;
    }
    else
      am = 0;
    const char* day = aDayNames[full_days % 7];
    const char* ampm = aAMPMNames[am];
    const char* month = aMonthNames[current_month];

    sprintfex(pTmpBuf.data(), "%s %d:%02d %s\n%d %s %d", day, current_hour, current_minutes, aAMPMNames[am], current_day + 1, month, current_year);
    save_load_window.DrawTitleText(pFontSmallnum, 0, 0, 0, pTmpBuf.data(), 3);
  }
  if ( pGUIWindow_CurrentMenu->receives_keyboard_input_2 == WINDOW_INPUT_CONFIRMED)
  {
    pGUIWindow_CurrentMenu->receives_keyboard_input_2 = WINDOW_INPUT_NONE;
    strcpy((char *)&pSavegameHeader + 100 * uLoadGameUI_SelectedSlot, (const char *)pKeyActionMap->pPressedKeysBuffer);
    pMessageQueue_50CBD0->AddMessage(UIMSG_SaveGame, 0, 0);
  }
  else
  {
    if ( pGUIWindow_CurrentMenu->receives_keyboard_input_2 == WINDOW_INPUT_CANCELLED)
      pGUIWindow_CurrentMenu->receives_keyboard_input_2 = WINDOW_INPUT_NONE;
  }
  if (GetCurrentMenuID() == MENU_LoadingProcInMainMenu)
  {
    pGUIWindow_CurrentMenu->DrawText(pFontSmallnum, pFontSmallnum->AlignText_Center(186, pGlobalTXT_LocalizationStrings[135]) + 25,
        220, 0, pGlobalTXT_LocalizationStrings[135], 0, 0, 0);//Загрузка
    pGUIWindow_CurrentMenu->DrawTextInRect(pFontSmallnum, pFontSmallnum->AlignText_Center(186,
        (const char *)pSavegameHeader.data() + 100 * uLoadGameUI_SelectedSlot) + 25, 0x106, 0, pSavegameHeader[uLoadGameUI_SelectedSlot].pName, 185, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontSmallnum, pFontSmallnum->AlignText_Center(186, pGlobalTXT_LocalizationStrings[165]) + 25,
        304, 0, pGlobalTXT_LocalizationStrings[165], 0, 0, 0);//"Пожалуйста, пожождите"
  }
  else
  {
    if ( save )
      pSaveFiles = 40;
    else
      pSaveFiles = uNumSavegameFiles;

    int slot_Y = 199;
    for ( uint i = pSaveListPosition; i < pSaveFiles; ++i )
    {
      if ( slot_Y >= 346 )
        break;
      if ( pGUIWindow_CurrentMenu->receives_keyboard_input_2 != WINDOW_INPUT_IN_PROGRESS || i != uLoadGameUI_SelectedSlot )
        pGUIWindow_CurrentMenu->DrawTextInRect(pFontSmallnum, 27, slot_Y, i == uLoadGameUI_SelectedSlot ? TargetColor(0xFF, 0xFF, 0x64) : 0, pSavegameHeader[i].pName, 185, 0);
      else
        pGUIWindow_CurrentMenu->DrawFlashingInputCursor(pGUIWindow_CurrentMenu->DrawTextInRect(pFontSmallnum, 27, slot_Y, i == uLoadGameUI_SelectedSlot ? TargetColor(0xFF, 0xFF, 0x64) : 0, (const char *)pKeyActionMap->pPressedKeysBuffer, 175, 1) + 27,
           slot_Y, pFontSmallnum);
      slot_Y += 21;
    }
  }
  pRenderer->EndScene();
}
// 6A0C9C: using guessed type int dword_6A0C9C;

//----- (004606F7) --------------------------------------------------------
void LoadUI_Draw()
{
  UI_DrawSaveLoad(false);
}

//----- (004606FE) --------------------------------------------------------
void SaveUI_Draw()
{
  UI_DrawSaveLoad(true);
}

//----- (0045E361) --------------------------------------------------------
void LoadUI_Load(unsigned int uDialogueType)
{
  LODWriteableFile pLODFile; // [sp+1Ch] [bp-248h]@1

  dword_6BE138 = -1;
  pIcons_LOD->_inlined_sub2();

  memset(pSavegameUsedSlots.data(), 0, sizeof(pSavegameUsedSlots));
  memset(pSavegameThumbnails.data(), 0, 45 * sizeof(RGBTexture));
  uTextureID_loadsave = pIcons_LOD->LoadTexture("loadsave", TEXTURE_16BIT_PALETTE);
  uTextureID_load_up = pIcons_LOD->LoadTexture("load_up", TEXTURE_16BIT_PALETTE);
  uTextureID_save_up = pIcons_LOD->LoadTexture("save_up", TEXTURE_16BIT_PALETTE);
  uTextureID_LS_loadU = pIcons_LOD->LoadTexture("LS_loadU", TEXTURE_16BIT_PALETTE);
  uTextureID_LS_saveU = pIcons_LOD->LoadTexture("LS_saveU", TEXTURE_16BIT_PALETTE);
  uTextureID_x_u = pIcons_LOD->LoadTexture("x_u", TEXTURE_16BIT_PALETTE);
  if ( uDialogueType )
  {
    pRenderer->DrawTextureIndexed(8, 8, pIcons_LOD->GetTexture(uTextureID_loadsave));
    if ( pCurrentScreen == SCREEN_SAVEGAME )
    {
      pRenderer->DrawTextureIndexed(241, 302, pIcons_LOD->GetTexture(uTextureID_LS_saveU));
      pRenderer->DrawTextureIndexed( 18, 141, pIcons_LOD->GetTexture(uTextureID_save_up));
    }
    else
    {
      pRenderer->DrawTextureIndexed(241, 302, pIcons_LOD->GetTexture(uTextureID_LS_loadU));
      pRenderer->DrawTextureIndexed( 18, 141, pIcons_LOD->GetTexture(uTextureID_load_up));
    }
    pRenderer->DrawTextureIndexed(351, 302, pIcons_LOD->GetTexture(uTextureID_x_u));
  }
  else
    pRenderer->DrawTextureRGB(0, 0, &pTexture_PCX);
  pGUIWindow_CurrentMenu = GUIWindow::Create(saveload_dlg_xs[uDialogueType], saveload_dlg_ys[uDialogueType], saveload_dlg_zs[uDialogueType],
      saveload_dlg_ws[uDialogueType], WINDOW_MainMenu_Load, 0, 0);
  pGUIWindow_CurrentMenu->DrawText(pFontSmallnum, 25, 199, 0, pGlobalTXT_LocalizationStrings[505], 0, 0, 0);// "Reading..."
  pRenderer->Present();
  pSavegameList->Initialize(0);
  if ( pSaveListPosition > (signed int)uNumSavegameFiles )
  {
    pSaveListPosition = 0;
    uLoadGameUI_SelectedSlot = 0;
  }
  pLODFile.AllocSubIndicesAndIO(300, 0);
  Assert(sizeof(SavegameHeader) == 100);
  for (uint i = 0; i < uNumSavegameFiles; ++i)
  {
    sprintf(pTmpBuf.data(), "saves\\%s", pSavegameList->pFileList[i].pSaveFileName);
    if (_access(pTmpBuf.data(), 6))
    {
      pSavegameUsedSlots[i] = 0;
      strcpy(pSavegameHeader[i].pName, pGlobalTXT_LocalizationStrings[72]); // "Empty"
      continue;
    }
    pLODFile.LoadFile(pTmpBuf.data(), 1);
    if ( pLODFile.FindContainer("header.bin", true) )
      fread(&pSavegameHeader[i], 100, 1, pLODFile.FindContainer("header.bin", true));
    if ( !_stricmp(pSavegameList->pFileList[i].pSaveFileName, pGlobalTXT_LocalizationStrings[613]) )// "AutoSave.MM7"
      strcpy(pSavegameHeader[i].pName, pGlobalTXT_LocalizationStrings[16]);// "Autosave"
    if ( !pLODFile.FindContainer("image.pcx", true) )
    {
      pSavegameUsedSlots[i] = 0;
      strcpy(pSavegameList->pFileList[i].pSaveFileName, "");
    }
    else
    {
      pSavegameThumbnails[i].LoadFromFILE(pLODFile.FindContainer("image.pcx", true), 0, true);
      pLODFile.CloseWriteFile();
      pSavegameUsedSlots[i] = 1;
    }
  }

  pLODFile.FreeSubIndexAndIO();
  if ( pCurrentScreen == SCREEN_SAVEGAME )
  {
    uTextureID_x_d = pIcons_LOD->LoadTexture("x_d", TEXTURE_16BIT_PALETTE);
    uTextureID_LS_ = pIcons_LOD->LoadTexture("LS_saveD",TEXTURE_16BIT_PALETTE);
  }
  else
  {
    uTextureID_x_d = pIcons_LOD->LoadTexture("x_d", TEXTURE_16BIT_PALETTE);
    uTextureID_LS_ = pIcons_LOD->LoadTexture("LS_loadD",TEXTURE_16BIT_PALETTE);
  }
  uTextureID_AR_UP_DN = pIcons_LOD->LoadTexture("AR_UP_DN", TEXTURE_16BIT_PALETTE);
  uTextureID_AR_DN_DN = pIcons_LOD->LoadTexture("AR_DN_DN", TEXTURE_16BIT_PALETTE);
  pGUIWindow_CurrentMenu->CreateButton(21, 198, 191, 18, 1, 0, UIMSG_SelectLoadSlot, 0, 0, "", 0);
  pGUIWindow_CurrentMenu->CreateButton(21, 219, 191, 18, 1, 0, UIMSG_SelectLoadSlot, 1, 0, "", 0);
  pGUIWindow_CurrentMenu->CreateButton(21, 240, 191, 18, 1, 0, UIMSG_SelectLoadSlot, 2, 0, "", 0);
  pGUIWindow_CurrentMenu->CreateButton(21, 261, 191, 18, 1, 0, UIMSG_SelectLoadSlot, 3, 0, "", 0);
  pGUIWindow_CurrentMenu->CreateButton(21, 282, 191, 18, 1, 0, UIMSG_SelectLoadSlot, 4, 0, "", 0);
  pGUIWindow_CurrentMenu->CreateButton(21, 303, 191, 18, 1, 0, UIMSG_SelectLoadSlot, 5, 0, "", 0);
  pGUIWindow_CurrentMenu->CreateButton(21, 324, 191, 18, 1, 0, UIMSG_SelectLoadSlot, 6, 0, "", 0);
  pBtnLoadSlot  = pGUIWindow_CurrentMenu->CreateButton(241, 302, 105, 40, 1, 0, UIMSG_SaveLoadBtn, 0, 0, "", pIcons_LOD->GetTexture(uTextureID_LS_), 0);
  pBtnCancel    = pGUIWindow_CurrentMenu->CreateButton(350, 302, 105, 40, 1, 0, UIMSG_Cancel, 0, 0, "", pIcons_LOD->GetTexture(uTextureID_x_d), 0);
  pBtnArrowUp   = pGUIWindow_CurrentMenu->CreateButton(215, 199,  17, 17, 1, 0, UIMSG_ArrowUp, 0, 0, "", pIcons_LOD->GetTexture(uTextureID_AR_UP_DN), 0);
  pBtnDownArrow = pGUIWindow_CurrentMenu->CreateButton(215, 323,  17, 17, 1, 0, UIMSG_DownArrow, uNumSavegameFiles, 0, "", pIcons_LOD->GetTexture(uTextureID_AR_DN_DN), 0);
}

//----- (0045E93E) --------------------------------------------------------
void SaveUI_Load()
{
  char *v3; // eax@7
  LODWriteableFile pLODFile; // [sp+1Ch] [bp-248h]@1

  ++pIcons_LOD->uTexturePacksCount;
  if ( !pIcons_LOD->uNumPrevLoadedFiles )
    pIcons_LOD->uNumPrevLoadedFiles = pIcons_LOD->uNumLoadedFiles;
  memset(pSavegameUsedSlots.data(), 0, 0xB4u);
  memset(&pSavegameThumbnails, 0, 0x708u);
  uTextureID_loadsave = pIcons_LOD->LoadTexture("loadsave", TEXTURE_16BIT_PALETTE);
  uTextureID_load_up = pIcons_LOD->LoadTexture("load_up", TEXTURE_16BIT_PALETTE);
  uTextureID_save_up = pIcons_LOD->LoadTexture("save_up", TEXTURE_16BIT_PALETTE);
  uTextureID_LS_loadU = pIcons_LOD->LoadTexture("LS_loadU", TEXTURE_16BIT_PALETTE);
  uTextureID_LS_saveU = pIcons_LOD->LoadTexture("LS_saveU", TEXTURE_16BIT_PALETTE);
  uTextureID_x_u = pIcons_LOD->LoadTexture("x_u", TEXTURE_16BIT_PALETTE);
  pRenderer->DrawTextureIndexed(8, 8, pIcons_LOD->GetTexture(uTextureID_loadsave));
  if ( pCurrentScreen == SCREEN_SAVEGAME )
  {
    pRenderer->DrawTextureIndexed(241, 302, pIcons_LOD->GetTexture(uTextureID_LS_saveU));
    pRenderer->DrawTextureIndexed(351, 302, pIcons_LOD->GetTexture(uTextureID_x_u));
    pRenderer->DrawTextureIndexed(18, 141, pIcons_LOD->GetTexture(uTextureID_save_up));
  }
  else
  {
    pRenderer->DrawTextureIndexed(241, 302, pIcons_LOD->GetTexture(uTextureID_LS_loadU));
    pRenderer->DrawTextureIndexed(351, 302, pIcons_LOD->GetTexture(uTextureID_x_u));
    pRenderer->DrawTextureIndexed(18, 141, pIcons_LOD->GetTexture(uTextureID_load_up));
  }
  pGUIWindow_CurrentMenu->DrawText(pFontSmallnum, 25, 199, 0, pGlobalTXT_LocalizationStrings[505], 0, 0, 0);//Read...(Чтение...)
  pRenderer->Present();
  pSavegameList->Initialize(1);
  pLODFile.AllocSubIndicesAndIO(300, 0);
  for (uint i = 0; i < 40; ++i)
  {
    v3 = pSavegameList->pFileList[i].pSaveFileName;
    if ( !*pSavegameList->pFileList[i].pSaveFileName )
      v3 = "1.mm7";
    sprintf(pTmpBuf.data(), "saves\\%s", v3);
    if ( _access(pTmpBuf.data(), 0) || _access(pTmpBuf.data(), 6) )
    {
      pSavegameUsedSlots[i] = 0;
      strcpy(pSavegameHeader[i].pName, pGlobalTXT_LocalizationStrings[LOCSTR_EMPTY]);
    }
    else
    {
      pLODFile.LoadFile(pTmpBuf.data(), 1);
      fread(&pSavegameHeader[i], 100, 1, pLODFile.FindContainer("header.bin", 1));
      if ( pLODFile.FindContainer("image.pcx", 1) )
      {
        pSavegameThumbnails[i].LoadFromFILE(pLODFile.FindContainer("image.pcx", 1), 0, 1);
        pLODFile.CloseWriteFile();
        pSavegameUsedSlots[i] = 1;
      }
      else
        pSavegameUsedSlots[i] = 0;
    }
  }
  pLODFile.FreeSubIndexAndIO();
  uTextureID_x_d = pIcons_LOD->LoadTexture("x_d", TEXTURE_16BIT_PALETTE);
  uTextureID_LS_ = pIcons_LOD->LoadTexture("LS_saveD", TEXTURE_16BIT_PALETTE);
  uTextureID_AR_UP_DN = pIcons_LOD->LoadTexture("AR_UP_DN", TEXTURE_16BIT_PALETTE);
  uTextureID_AR_DN_DN = pIcons_LOD->LoadTexture("AR_DN_DN", TEXTURE_16BIT_PALETTE);
  pGUIWindow_CurrentMenu = GUIWindow::Create(0, 0, 640, 480, WINDOW_SaveLoadButtons, 0, 0);
  pGUIWindow_CurrentMenu->CreateButton(21, 198, 191, 18, 1, 0, UIMSG_SelectLoadSlot, 0, 0, "", 0);
  pGUIWindow_CurrentMenu->CreateButton(21, 218, 191, 18, 1, 0, UIMSG_SelectLoadSlot, 1, 0, "", 0);
  pGUIWindow_CurrentMenu->CreateButton(21, 238, 191, 18, 1, 0, UIMSG_SelectLoadSlot, 2, 0, "", 0);
  pGUIWindow_CurrentMenu->CreateButton(21, 258, 191, 18, 1, 0, UIMSG_SelectLoadSlot, 3, 0, "", 0);
  pGUIWindow_CurrentMenu->CreateButton(21, 278, 191, 18, 1, 0, UIMSG_SelectLoadSlot, 4, 0, "", 0);
  pGUIWindow_CurrentMenu->CreateButton(21, 298, 191, 18, 1, 0, UIMSG_SelectLoadSlot, 5, 0, "", 0);
  pGUIWindow_CurrentMenu->CreateButton(21, 318, 191, 18, 1, 0, UIMSG_SelectLoadSlot, 6, 0, "", 0);
  pBtnLoadSlot  = pGUIWindow_CurrentMenu->CreateButton(241, 302, 105, 40, 1, 0, UIMSG_SaveLoadBtn, 0, 0, "", pIcons_LOD->GetTexture(uTextureID_LS_), 0);
  pBtnCancel    = pGUIWindow_CurrentMenu->CreateButton(350, 302, 105, 40, 1, 0, UIMSG_Cancel, 0, 0, "", pIcons_LOD->GetTexture(uTextureID_x_d), 0);
  pBtnArrowUp   = pGUIWindow_CurrentMenu->CreateButton(215, 199,  17, 17, 1, 0, UIMSG_ArrowUp, 0, 0, "", pIcons_LOD->GetTexture(uTextureID_AR_UP_DN), 0);
  pBtnDownArrow = pGUIWindow_CurrentMenu->CreateButton(215, 323,  17, 17, 1, 0, UIMSG_DownArrow, 34, 0, "", pIcons_LOD->GetTexture(uTextureID_AR_DN_DN), 0);
}