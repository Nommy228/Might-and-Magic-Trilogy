#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define _CRT_SECURE_NO_WARNINGS
#include "..\MM7.h"
#include "UIBooks.h"
#include "..\Render.h"
#include "..\GUIWindow.h"
#include "..\GUIFont.h"
#include "..\AudioPlayer.h"
#include "..\LOD.h"

#include "..\mm7_data.h"

//----- (00413CC6) --------------------------------------------------------
void BookUI_Draw(WindowType book)
{
  pRenderer->DrawTextureIndexed(471, 445, pIcons_LOD->GetTexture(uExitCancelTextureId));
  switch (book)
  {
    case WINDOW_QuestBook:     BookUI_Questbook_Draw();  break;
    case WINDOW_AutonotesBook: BookUI_Autonotes_Draw();  break;
    case WINDOW_MapsBook:      BookUI_Map_Draw();        break;
    case WINDOW_CalendarBook:  BookUI_Calendar_Draw();   break;
    case WINDOW_JournalBook:   BookUI_Journal_Draw();    break;

    case WINDOW_LloydsBeacon:  DrawLloydBeaconsScreen(); break;
    case WINDOW_TownPortal:    BookUI_DrawTownPortalMap();   break;
  }
}

//----- (0041192C) --------------------------------------------------------
void InitializeBookTextures()
{
  pAudioPlayer->StopChannels(-1, -1);
  ++pIcons_LOD->uTexturePacksCount;
  if ( !pIcons_LOD->uNumPrevLoadedFiles )
    pIcons_LOD->uNumPrevLoadedFiles = pIcons_LOD->uNumLoadedFiles;
  pAudioPlayer->PlaySound(SOUND_OpenBook, 0, 0, -1, 0, 0, 0, 0);
  pSpellBookPagesTextr_9 = pIcons_LOD->LoadTexturePtr("book", TEXTURE_16BIT_PALETTE);
  pTexture_pagemask = pIcons_LOD->LoadTexturePtr("pagemask", TEXTURE_16BIT_PALETTE);
  pSpellBookCloseBtnTextr = pIcons_LOD->LoadTexturePtr("ib-m5-u", TEXTURE_16BIT_PALETTE);
  pSpellBookClickCloseBtnTextr = pIcons_LOD->LoadTexturePtr("ib-m5-d", TEXTURE_16BIT_PALETTE);
  pSBQuickSpellBtnTextr   = pIcons_LOD->LoadTexturePtr("ib-m6-u",TEXTURE_16BIT_PALETTE);

  static const char *texNames[9] = // 004E24EC
  {
    "SBFB00", "SBAB00", "SBWB00", "SBEB00",
    "SBSB00", "SBMB00", "SBBB00", "SBLB00", "SBDB00"
  };

  pSBClickQuickSpellBtnTextr = pIcons_LOD->LoadTexturePtr("ib-m6-d",TEXTURE_16BIT_PALETTE);
  for ( uint i = 0; i < 9; ++i )
  {
    pSpellBookPagesTextr[i] = pIcons_LOD->LoadTexturePtr(texNames[i], TEXTURE_16BIT_PALETTE);
    sprintf(pTmpBuf.data(), "tab%da", i + 1);
    pTextures_tabs[i][0] = pIcons_LOD->LoadTexturePtr(pTmpBuf.data(), TEXTURE_16BIT_PALETTE);
    sprintf(pTmpBuf.data(), "tab%db", i + 1);
    pTextures_tabs[i][1] = pIcons_LOD->LoadTexturePtr(pTmpBuf.data(), TEXTURE_16BIT_PALETTE);
  }
}

//----- (00411AAA) --------------------------------------------------------
void InitializeBookFonts()
{
  pAudioPlayer->StopChannels(-1, -1);
  ++pIcons_LOD->uTexturePacksCount;
  if ( !pIcons_LOD->uNumPrevLoadedFiles )
    pIcons_LOD->uNumPrevLoadedFiles = pIcons_LOD->uNumLoadedFiles;
  pAudioPlayer->PlaySound(SOUND_OpenBook, 0, 0, -1, 0, 0, 0, 0);
  pTexture_mapbordr = pIcons_LOD->LoadTexturePtr("mapbordr", TEXTURE_16BIT_PALETTE);
  pBookFont = LoadFont("book.fnt", "FONTPAL", NULL);
  pBook2Font = LoadFont("book2.fnt", "FONTPAL", NULL);
  pAutonoteFont = LoadFont("autonote.fnt", "FONTPAL", NULL);
  pSpellFont = LoadFont("spell.fnt", "FONTPAL", NULL);
}

//----- (0041140B) --------------------------------------------------------
void OnCloseSpellBookPage()
{
  GUIButton *pNextButton; // esi@4
  for ( uint i = 1; i <= 11; i++ )
  {
    SBPageCSpellsTextureList[i]->Release();
    SBPageSSpellsTextureList[i]->Release();
  }
  pIcons_LOD->SyncLoadedFilesCount();
  if ( pGUIWindow_CurrentMenu->pControlsHead )
  {
    do
    {
      pNextButton = pGUIWindow_CurrentMenu->pControlsHead->pNext;
      free(pGUIWindow_CurrentMenu->pControlsHead);
      pGUIWindow_CurrentMenu->pControlsHead = pNextButton;
    }
    while ( pNextButton );
  }
  pGUIWindow_CurrentMenu->pControlsHead = 0;
  pGUIWindow_CurrentMenu->pControlsTail = 0;
  pGUIWindow_CurrentMenu->uNumControls = 0;
}

//----- (00411473) --------------------------------------------------------
void OnCloseSpellBook()
{
  pTexture_pagemask->Release();
  pSpellBookCloseBtnTextr->Release();
  pSBQuickSpellBtnTextr->Release();
  for ( uint i = 0; i < 9; ++i )
  {
    pSpellBookPagesTextr[i]->Release();
    pTextures_tabs[i][0]->Release();
    pTextures_tabs[i][1]->Release();
  }
  pAudioPlayer->PlaySound(SOUND_CloseBook, 0, 0, -1, 0, 0, 0, 0);
  pIcons_LOD->RemoveTexturesPackFromTextureList();
}
