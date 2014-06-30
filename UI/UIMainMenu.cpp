#define _CRT_SECURE_NO_WARNINGS
#include "..\Mouse.h"
#include "..\Keyboard.h"
#include "..\ErrorHandling.h"

#include "..\GUIWindow.h"
#include "..\GUIFont.h"
#include "..\AudioPlayer.h"
#include "..\Render.h"
#include "..\LOD.h"
#include "..\PaletteManager.h"
#include "..\IconFrameTable.h"
#include "..\texts.h"

#include "..\mm7_data.h"
#include "..\mm7_unsorted_subs.h"
#include "..\Game.h"


//----- (0041B578) --------------------------------------------------------
void MainMenuUI_LoadFontsAndSomeStuff()
{
  //pIcons_LOD->SetupPalettes(pRenderer->uTargetRBits, pRenderer->uTargetGBits, pRenderer->uTargetBBits);
  pIcons_LOD->SetupPalettes(5, 6, 5);
  //pPaletteManager->SetColorChannelInfo(pRenderer->uTargetRBits, pRenderer->uTargetGBits, pRenderer->uTargetBBits);
  pPaletteManager->SetColorChannelInfo(5, 6, 5);
  pPaletteManager->RecalculateAll();

  for (uint i = 0; i < window->GetHeight(); ++i)
    pSRZBufferLineOffsets[i] = window->GetWidth() * i;

  pRenderer->ResetTextureClipRect();

  uTextureID_FONTPAL = pIcons_LOD->LoadTexture("FONTPAL", TEXTURE_16BIT_PALETTE);

  pFontArrus = LoadFont("arrus.fnt", "FONTPAL", nullptr);
  pFontArrus->field_3 = 0;

  pFontLucida = LoadFont("lucida.fnt", "FONTPAL", nullptr);
  pFontLucida->field_3 = 0;

  pFontCreate = LoadFont("create.fnt", "FONTPAL", nullptr);
  pFontCreate->field_3 = 0;

  pFontSmallnum = LoadFont("smallnum.fnt", "FONTPAL", nullptr);
  pFontComic = LoadFont("comic.fnt", "FONTPAL", nullptr);

  for (uint i = 0; i < 20; ++i)
    pWindowList[i].eWindowType = WINDOW_null;

  uNumVisibleWindows = -1;
  memset(pVisibleWindowsIdxs.data(), 0, sizeof(pVisibleWindowsIdxs));
}

//----- (004415C5) --------------------------------------------------------
static void LoadPartyBuffIcons()
{
  for (uint i = 0; i < 14; ++i)
  {
    char filename[200];
    sprintf(filename, "isn-%02d", i + 1);
    pTextureIDs_PartyBuffIcons[i] = pIcons_LOD->LoadTexture(filename, TEXTURE_16BIT_PALETTE);
  }

  uIconIdx_FlySpell = pIconsFrameTable->FindIcon("spell21");
  uIconIdx_WaterWalk = pIconsFrameTable->FindIcon("spell27");
}

//----- (0041B690) --------------------------------------------------------
void MainMenuUI_Create()
{
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("wizeyeC"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("wizeyeB"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("wizeyeA"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("torchC"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("torchB"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("torchA"));

  pTextureIDs_pMapDirs[0] = pIcons_LOD->LoadTexture("MAPDIR1", TEXTURE_16BIT_PALETTE);
  pTextureIDs_pMapDirs[1] = pIcons_LOD->LoadTexture("MAPDIR2", TEXTURE_16BIT_PALETTE);
  pTextureIDs_pMapDirs[2] = pIcons_LOD->LoadTexture("MAPDIR3", TEXTURE_16BIT_PALETTE);
  pTextureIDs_pMapDirs[3] = pIcons_LOD->LoadTexture("MAPDIR4", TEXTURE_16BIT_PALETTE);
  pTextureIDs_pMapDirs[4] = pIcons_LOD->LoadTexture("MAPDIR5", TEXTURE_16BIT_PALETTE);
  pTextureIDs_pMapDirs[5] = pIcons_LOD->LoadTexture("MAPDIR6", TEXTURE_16BIT_PALETTE);
  pTextureIDs_pMapDirs[6] = pIcons_LOD->LoadTexture("MAPDIR7", TEXTURE_16BIT_PALETTE);
  pTextureIDs_pMapDirs[7] = pIcons_LOD->LoadTexture("MAPDIR8", TEXTURE_16BIT_PALETTE);

  uTextureID_BarBlue = pIcons_LOD->LoadTexture("ib-statB", TEXTURE_16BIT_PALETTE);
  uTextureID_BarGreen = pIcons_LOD->LoadTexture("ib-statG", TEXTURE_16BIT_PALETTE);
  uTextureID_BarYellow = pIcons_LOD->LoadTexture("ib-statY", TEXTURE_16BIT_PALETTE);
  uTextureID_BarRed = pIcons_LOD->LoadTexture("ib-statR", TEXTURE_16BIT_PALETTE);
  uTextureID_mhp_bd = pIcons_LOD->LoadTexture("mhp_bg", TEXTURE_16BIT_PALETTE);
  uTextureID_mhp_capl = pIcons_LOD->LoadTexture("mhp_capl", TEXTURE_16BIT_PALETTE);
  uTextureID_mhp_capr = pIcons_LOD->LoadTexture("mhp_capr", TEXTURE_16BIT_PALETTE);
  uTextureID_mhp_grn = pIcons_LOD->LoadTexture("mhp_grn", TEXTURE_16BIT_PALETTE);
  uTextureID_mhp_red = pIcons_LOD->LoadTexture("mhp_red", TEXTURE_16BIT_PALETTE);
  uTextureID_mhp_yel = pIcons_LOD->LoadTexture("mhp_yel", TEXTURE_16BIT_PALETTE);
  uTextureID_Leather = pIcons_LOD->LoadTexture("LEATHER", TEXTURE_16BIT_PALETTE);
  pTexture_Leather = pIcons_LOD->LoadTexturePtr("ibground", TEXTURE_16BIT_PALETTE);
  uTextureID_x_x_u = pIcons_LOD->LoadTexture("x_x_u", TEXTURE_16BIT_PALETTE);
  uTextureID_BUTTDESC2 = pIcons_LOD->LoadTexture("BUTTESC2", TEXTURE_16BIT_PALETTE);
  uTextureID_x_ok_u = pIcons_LOD->LoadTexture("x_ok_u", TEXTURE_16BIT_PALETTE);
  uTextureID_BUTTYES2 = pIcons_LOD->LoadTexture("BUTTYES2", TEXTURE_16BIT_PALETTE);
  uTextureID_BUTTMAKE = pIcons_LOD->LoadTexture("BUTTMAKE", TEXTURE_16BIT_PALETTE);
  uTextureID_BUTTMAKE2 = pIcons_LOD->LoadTexture("BUTTMAKE2", TEXTURE_16BIT_PALETTE);

  pPrimaryWindow = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_MainMenu, 0, 0);
  pPrimaryWindow->CreateButton(7, 8, 460, 343, 1, 0, UIMSG_MouseLeftClickInGame, 0, 0, "", 0);

  pPrimaryWindow->CreateButton(61, 424, 31, 80, 2, 94, UIMSG_SelectCharacter, 1, '1', "", 0);//buttons for portraits
  pPrimaryWindow->CreateButton(177, 424, 31, 80, 2, 94, UIMSG_SelectCharacter, 2, '2', "", 0);
  pPrimaryWindow->CreateButton(292, 424, 31, 40, 2, 94, UIMSG_SelectCharacter, 3, '3', "", 0);
  pPrimaryWindow->CreateButton(407, 424, 31, 40, 2, 94, UIMSG_SelectCharacter, 4, '4', "", 0);

  pPrimaryWindow->CreateButton(24, 404, 5, 49, 1, 93, UIMSG_0, 1, 0, "", 0);//buttons for HP
  pPrimaryWindow->CreateButton(139, 404, 5, 49, 1, 93, UIMSG_0, 2, 0, "", 0);
  pPrimaryWindow->CreateButton(255, 404, 5, 49, 1, 93, UIMSG_0, 3, 0, "", 0);
  pPrimaryWindow->CreateButton(370, 404, 5, 49, 1, 93, UIMSG_0, 4, 0, "", 0);

  pPrimaryWindow->CreateButton(97, 404, 5, 49, 1, 93, UIMSG_0, 1, 0, "", 0);//buttons for SP
  pPrimaryWindow->CreateButton(212, 404, 5, 49, 1, 93, UIMSG_0, 2, 0, "", 0);
  pPrimaryWindow->CreateButton(328, 404, 5, 49, 1, 93, UIMSG_0, 3, 0, "", 0);
  pPrimaryWindow->CreateButton(443, 404, 5, 49, 1, 93, UIMSG_0, 4, 0, "", 0);

  uTextureID_ib_td1_A = pIcons_LOD->LoadTexture("ib-td1-A", TEXTURE_16BIT_PALETTE);
  pBtn_Quests = pPrimaryWindow->CreateButton(491, 353, pIcons_LOD->GetTexture(uTextureID_ib_td1_A)->uTextureWidth,
      pIcons_LOD->GetTexture(uTextureID_ib_td1_A)->uTextureHeight, 1, 0, UIMSG_OpenQuestBook, 0, pKeyActionMap->GetActionVKey(INPUT_Quest),
      pGlobalTXT_LocalizationStrings[174], pIcons_LOD->GetTexture(uTextureID_ib_td1_A), 0); //Quests

  uTextureID_ib_td2_A = pIcons_LOD->LoadTexture("ib-td2-A", TEXTURE_16BIT_PALETTE);
  pBtn_Autonotes = pPrimaryWindow->CreateButton(527, 353, pIcons_LOD->GetTexture(uTextureID_ib_td2_A)->uTextureWidth,
     pIcons_LOD->GetTexture(uTextureID_ib_td2_A)->uTextureHeight, 1, 0, UIMSG_OpenAutonotes, 0, pKeyActionMap->GetActionVKey(INPUT_Autonotes),
     pGlobalTXT_LocalizationStrings[154], pIcons_LOD->GetTexture(uTextureID_ib_td2_A), 0);//Autonotes

  uTextureID_ib_td3_A = pIcons_LOD->LoadTexture("ib-td3-A", TEXTURE_16BIT_PALETTE);
  pBtn_Maps = pPrimaryWindow->CreateButton(546, 353, pIcons_LOD->GetTexture(uTextureID_ib_td3_A)->uTextureWidth,
     pIcons_LOD->GetTexture(uTextureID_ib_td3_A)->uTextureHeight, 1, 0, UIMSG_OpenMapBook, 0, pKeyActionMap->GetActionVKey(INPUT_Mapbook),
     pGlobalTXT_LocalizationStrings[139], pIcons_LOD->GetTexture(uTextureID_ib_td3_A), 0); //Maps

  uTextureID_ib_td4_A = pIcons_LOD->LoadTexture("ib-td4-A", TEXTURE_16BIT_PALETTE);
  pBtn_Calendar = pPrimaryWindow->CreateButton(570, 353, pIcons_LOD->GetTexture(uTextureID_ib_td4_A)->uTextureWidth,
     pIcons_LOD->GetTexture(uTextureID_ib_td4_A)->uTextureHeight, 1, 0, UIMSG_OpenCalendar, 0, pKeyActionMap->GetActionVKey(INPUT_TimeCal),
     pGlobalTXT_LocalizationStrings[78], pIcons_LOD->GetTexture(uTextureID_ib_td4_A), 0);//Calendar

  uTextureID_ib_td5_A = pIcons_LOD->LoadTexture("ib-td5-A", TEXTURE_16BIT_PALETTE);
  pBtn_History = pPrimaryWindow->CreateButton(600, 361, pIcons_LOD->GetTexture(uTextureID_ib_td5_A)->uTextureWidth,
      pIcons_LOD->GetTexture(uTextureID_ib_td5_A)->uTextureHeight, 1, 0, UIMSG_OpenHistoryBook, 0, 72,//ascii
      pGlobalTXT_LocalizationStrings[602], pIcons_LOD->GetTexture(uTextureID_ib_td5_A), 0);//History

  bFlashAutonotesBook = 0;
  bFlashQuestBook = 0;
  bFlashHistoryBook = 0;

  pBtn_ZoomIn = pPrimaryWindow->CreateButton(574, 136, pIcons_LOD->pTextures[uTextureID_Btn_ZoomIn].uTextureWidth,
     pIcons_LOD->pTextures[uTextureID_Btn_ZoomIn].uTextureHeight, 2, 0, UIMSG_ClickZoomInBtn, 0, pKeyActionMap->GetActionVKey(INPUT_ZoomIn),
     pGlobalTXT_LocalizationStrings[252], &pIcons_LOD->pTextures[uTextureID_Btn_ZoomIn], 0); // Zoom In

  pBtn_ZoomOut = pPrimaryWindow->CreateButton(519, 136, pIcons_LOD->pTextures[uTextureID_Btn_ZoomOut].uTextureWidth,
     pIcons_LOD->pTextures[uTextureID_Btn_ZoomOut].uTextureHeight, 2, 0, UIMSG_ClickZoomOutBtn, 0, pKeyActionMap->GetActionVKey(INPUT_ZoomOut),
     pGlobalTXT_LocalizationStrings[251], &pIcons_LOD->pTextures[uTextureID_Btn_ZoomOut], 0); // Zoom Out

  pPrimaryWindow->CreateButton(481, 0, 153, 67, 1, 92, UIMSG_0, 0, 0, "", 0);
  pPrimaryWindow->CreateButton(491, 149, 64, 74, 1, 0, UIMSG_StartHireling1Dialogue, 0, '5', "", 0);
  pPrimaryWindow->CreateButton(561, 149, 64, 74, 1, 0, UIMSG_StartHireling2Dialogue, 0, '6', "", 0);
  pPrimaryWindow->CreateButton(476, 322, 77, 17, 1, 100, UIMSG_0, 0, 0, "", 0);
  pPrimaryWindow->CreateButton(555, 322, 77, 17, 1, 101, UIMSG_0, 0, 0, "", 0);

  pBtn_CastSpell = pPrimaryWindow->CreateButton(476, 450,
      pIcons_LOD->GetTexture(uTextureID_Btn_CastSpell)->uTextureWidth,
      pIcons_LOD->GetTexture(uTextureID_Btn_CastSpell)->uTextureHeight,
      1, 0, UIMSG_SpellBookWindow, 0, 67, pGlobalTXT_LocalizationStrings[38], pIcons_LOD->GetTexture(uTextureID_Btn_CastSpell), 0);
  pBtn_Rest = pPrimaryWindow->CreateButton(518, 450,
      pIcons_LOD->GetTexture(uTextureID_Btn_Rest)->uTextureWidth,
      pIcons_LOD->GetTexture(uTextureID_Btn_Rest)->uTextureHeight,
      1, 0, UIMSG_RestWindow, 0, 82, pGlobalTXT_LocalizationStrings[182], pIcons_LOD->GetTexture(uTextureID_Btn_Rest), 0);
  pBtn_QuickReference = pPrimaryWindow->CreateButton(560, 450,
      pIcons_LOD->GetTexture(uTextureID_Btn_QuickReference)->uTextureWidth,
      pIcons_LOD->GetTexture(uTextureID_Btn_QuickReference)->uTextureHeight,
      1, 0, UIMSG_QuickReference, 0, 90, pGlobalTXT_LocalizationStrings[173], pIcons_LOD->GetTexture(uTextureID_Btn_QuickReference), 0);
  pBtn_GameSettings = pPrimaryWindow->CreateButton(602, 450,
      pIcons_LOD->GetTexture(uTextureID_Btn_GameSettings)->uTextureWidth,
      pIcons_LOD->GetTexture(uTextureID_Btn_GameSettings)->uTextureHeight,
      1, 0, UIMSG_GameMenuButton, 0, 0, pGlobalTXT_LocalizationStrings[93], pIcons_LOD->GetTexture(uTextureID_Btn_GameSettings), 0);

  pBtn_NPCLeft = pPrimaryWindow->CreateButton(469, 178,
      pIcons_LOD->GetTexture(uTextureID_Btn_NPCLeft)->uTextureWidth,
      pIcons_LOD->GetTexture(uTextureID_Btn_NPCLeft)->uTextureHeight,
      1, 0, UIMSG_ScrollNPCPanel, 0, 0, "", pIcons_LOD->GetTexture(uTextureID_Btn_NPCLeft), 0);
  pBtn_NPCRight = pPrimaryWindow->CreateButton(626, 178,
      pIcons_LOD->GetTexture(uTextureID_Btn_NPCRight)->uTextureWidth,
      pIcons_LOD->GetTexture(uTextureID_Btn_NPCRight)->uTextureHeight,
      1, 0, UIMSG_ScrollNPCPanel, 1, 0, "", pIcons_LOD->GetTexture(uTextureID_Btn_NPCRight), 0);
  LoadPartyBuffIcons();
}




//----- (00452AF3) --------------------------------------------------------
void __fastcall fill_pixels_fast(unsigned int a1, unsigned __int16 *pPixels, unsigned int uNumPixels)
{
  void *v3; // edi@1
  unsigned int v4; // eax@1
  unsigned __int16 *v5; // edi@3
  unsigned int i; // ecx@3

  __debugbreak(); // Nomad: sub operates on 16 bit pixels, we have 32 bits.

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

//----- (004979D2) --------------------------------------------------------
MENU_STATE MainMenuUI_Credits_Loop()
{
  char *cred_texturet; // edi@5
  FILE *pFile; // eax@5
  unsigned int pSize; // esi@7
  MSG Msg; // [sp+84h] [bp-B8h]@10
  GUIWindow credit_window;
  int move_Y; // [sp+128h] [bp-14h]@1
  char *pString; // [sp+12Ch] [bp-10h]@9
  GUIFont *pFontQuick; // [sp+134h] [bp-8h]@1
  GUIFont *pFontCChar; // [sp+138h] [bp-4h]@1
  RGBTexture mm6title_texture; // [sp+54h] [bp-E8h]@1
  RGBTexture cred_texture; // [sp+100h] [bp-3Ch]@1
  Texture pTemporaryTexture; // [sp+Ch] [bp-130h]@5

  pFontQuick = LoadFont("quick.fnt", "FONTPAL", NULL);
  pFontCChar = LoadFont("cchar.fnt", "FONTPAL", NULL);

  if ( pMessageQueue_50CBD0->uNumMessages )
    pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
  ++pIcons_LOD->uTexturePacksCount;
  if ( !pIcons_LOD->uNumPrevLoadedFiles )
    pIcons_LOD->uNumPrevLoadedFiles = pIcons_LOD->uNumLoadedFiles;
  //dword_A74C88 = 0;//??? часть дальнейшего кода отсутствует, там использовалась данная переменная

  pAudioPlayer->PlayMusicTrack(MUSIC_Credits);

  mm6title_texture.Load("mm6title.pcx", 0);
  cred_texturet = (char *)pEvents_LOD->LoadRaw("credits.txt", 0);
  pFile = pEvents_LOD->FindContainer("credits.txt", 0);
  if ( !pFile )
    Error(pGlobalTXT_LocalizationStrings[63]); // "Might and Magic VII is having trouble loading files. 

  // Please re-install to fix this problem. Note: Re-installing will not destroy your save games."

  //для получения размера-----------------------
  fread(&pTemporaryTexture, 1, 0x30, pFile);
  pSize = pTemporaryTexture.uDecompressedSize;
  if ( !pSize )
    pSize = pTemporaryTexture.uTextureSize;
  memset(&pTemporaryTexture, 0, 0x48);//обнуление
  cred_texturet[pSize] = 0;//конец текста

  credit_window.uFrameWidth = 250;
  credit_window.uFrameHeight = 440;
  credit_window.uFrameX = 389;
  credit_window.uFrameY = 19;

  cred_texture.uWidth = 250;
  cred_texture.uHeight = pFontQuick->GetStringHeight2(pFontCChar, cred_texturet, &credit_window, 0, 1) + 2 * credit_window.uFrameHeight;
  cred_texture.uNumPixels = cred_texture.uWidth * cred_texture.uHeight;
  cred_texture.pPixels = (unsigned __int16 *)malloc(2 * cred_texture.uNumPixels);
  fill_pixels_fast(Color16(0, 0xFFu, 0xFFu), cred_texture.pPixels, cred_texture.uNumPixels);
  cred_texture._allocation_flags = 0;

  //дать шрифт и цвета тексту
  pString = (char *)malloc(2 * pSize);
  strncpy(pString, cred_texturet, pSize);
  pString[pSize] = 0;
  pFontQuick->_44D2FD_prolly_draw_credits_entry(pFontCChar, 0, credit_window.uFrameHeight, cred_texture.uWidth,
    cred_texture.uHeight, Color16(0x70u, 0x8Fu, 0xFEu), Color16(0xECu, 0xE6u, 0x9Cu), pString, cred_texture.pPixels, cred_texture.uWidth);
  free(pString);

  pWindow_MainMenu = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_MainMenu, 0, cred_texturet);
  pWindow_MainMenu->CreateButton(0, 0, 0, 0, 1, 0, UIMSG_Escape, 0, 27, "", 0);
  pCurrentScreen = SCREEN_CREATORS;
  SetCurrentMenuID(MENU_CREDITSPROC);

  move_Y = 0;
  do
  {
    while ( PeekMessageA(&Msg, 0, 0, 0, 1) )
    {
      if ( Msg.message == 18 )
        Game_DeinitializeAndTerminate(0);
      TranslateMessage(&Msg);
      DispatchMessageA(&Msg);
    }
    if (dword_6BE364_game_settings_1 & GAME_SETTINGS_APP_INACTIVE)
    {
      WaitMessage();
    }
    else
    {
      pRenderer->BeginScene();
      pRenderer->DrawTextureRGB(0, 0, &mm6title_texture);
      pRenderer->SetTextureClipRect(credit_window.uFrameX, credit_window.uFrameY, credit_window.uFrameX + credit_window.uFrameWidth,
           credit_window.uFrameY + credit_window.uFrameHeight);
      pRenderer->CreditsTextureScroll(credit_window.uFrameX, credit_window.uFrameY, 0, move_Y, &cred_texture);
      pRenderer->ResetTextureClipRect();
      pRenderer->EndScene();
      ++move_Y;
      if ( move_Y >= cred_texture.uHeight )
        SetCurrentMenuID(MENU_MAIN);
      pRenderer->Present();
      pCurrentScreen = SCREEN_GAME;//Ritor1: temporarily, must be corrected GUI_MainMenuMessageProc()
      GUI_MainMenuMessageProc();
    }
  }
  while ( GetCurrentMenuID() == MENU_CREDITSPROC );
  pAudioPlayer->_4AA258(1);
  free(cred_texturet);
  free(pFontQuick);
  free(pFontCChar);
  pWindow_MainMenu->Release();
  pIcons_LOD->RemoveTexturesPackFromTextureList();
  mm6title_texture.Release();
  cred_texture.Release();
  return MENU_MAIN;     // return MENU_Main
}
