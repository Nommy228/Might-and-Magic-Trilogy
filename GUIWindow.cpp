#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define _CRT_SECURE_NO_WARNINGS
#include "ErrorHandling.h"
#include "mm7_unsorted_subs.h"
#include "GUIWindow.h"
#include "GUIFont.h"
#include "Party.h"
#include "LOD.h"
#include "Keyboard.h"
#include "OurMath.h"
#include "MapInfo.h"
#include "Timer.h"
#include "AudioPlayer.h"
#include "Mouse.h"
#include "Viewport.h"
#include "Render.h"
#include "PlayerFrameTable.h"
#include "SaveLoad.h"
#include "StorylineTextTable.h"
#include "Events2D.h"
#include "UI\UIHouses.h"
#include "UI\UIBooks.h"
#include "texts.h"
#include "Autonotes.h"
#include "Awards.h"
#include "Chest.h"
#include "Outdoor.h"
#include "Game.h"
#include "IconFrameTable.h"
#include "Actor.h"


#include "mm7_data.h"

typedef struct _RGBColor
    {
    unsigned char R;
    unsigned char B;
    unsigned char G;
    }RGBColor;


std::array<RGBColor, 20> spell_tooltip_colors={{ 
    {0x96, 0xD4, 0xFF},
    {0xFF, 0x80, 0x00},
    {0xFF, 0xFF, 0x9B},
    {0xE1, 0xE1, 0xE1},
    {0x80, 0x80, 0x80},
    {0x96, 0xD4, 0xFF},
    {0xFF, 0x55, 0x00},
    {0x96, 0xD4, 0xFF},
    {0xFF, 0x55, 0x00},
    {0xE1, 0xE1, 0xE1},
    {0xFF, 0x55, 0x00},
    {0x96, 0xD4, 0xFF},
    {0xEB, 0x0F, 0xFF},
    {0xFF, 0x80, 0x00},
    {0x96, 0xD4, 0xFF},
    {0x80, 0x80, 0x80},
    {0xFF, 0x55, 0x00},
    {0x00, 0x80, 0xFF},
    {0x00, 0x80, 0xFF},
    {0x96, 0xD4, 0xFF}}};


int pWindowList_at_506F50_minus1_indexing_buttons____and_an_int_[1]; // idb
struct GUIWindow *pWindow_MMT_MainMenu;
struct GUIWindow *pWindow_MainMenu;
std::array<struct GUIWindow, 20> pWindowList;

struct GUIMessageQueue *pMessageQueue_50CBD0 = new GUIMessageQueue;
struct GUIMessageQueue *pMessageQueue_50C9E8 = new GUIMessageQueue;






// inlined
//----- (mm6c::00420520) --------------------------------------------------
void GUIMessageQueue::Flush()
{
  if (uNumMessages)
    uNumMessages = pMessages[0].field_8 != 0;
}

//----- (004356B9) --------------------------------------------------------
void GUIMessageQueue::PopMessage(enum UIMessageType *pType, int *pParam, int *a4)
{
  if ( this->uNumMessages )
  {
    *pType = this->pMessages[0].eType;
    *pParam = this->pMessages[0].param;
    *a4 = this->pMessages[0].field_8;
    if ( (signed int)(this->uNumMessages - 1) > 0 )
    {
      for ( uint i = 0; i < (signed int)(this->uNumMessages - 1); ++i )
      {
        this->pMessages[i].eType = this->pMessages[i + 1].eType;
        this->pMessages[i].param = this->pMessages[i + 1].param;
        this->pMessages[i].field_8 = this->pMessages[i + 1].field_8;
      }
    }
    --this->uNumMessages;
  }
}

//----- (0041B4E1) --------------------------------------------------------
int __fastcall GUI_ReplaceHotkey(unsigned __int8 uOldHotkey, unsigned __int8 uNewHotkey, char bFirstCall)
{
  int result; // eax@1
  int i; // edx@2
  GUIButton *j; // ecx@3
  int k; // edx@7
  GUIButton *l; // ecx@8
  unsigned __int8 v9; // [sp+4h] [bp-8h]@1
  char old_hot_key; // [sp+8h] [bp-4h]@1

  //v3 = uNewHotkey;
  old_hot_key = toupper(uOldHotkey);
  result = toupper(uNewHotkey);
  v9 = result;
  if ( bFirstCall )
  {
    for ( i = uNumVisibleWindows; i >= 0; --i )
    {
      result = 84 * pVisibleWindowsIdxs[i];
      for ( j = pWindowList[pVisibleWindowsIdxs[i] - 1].pControlsHead; j; j = j->pNext )
        j->field_28 = 0;
    }
  }
  for ( k = uNumVisibleWindows; k >= 0; --k )
  {
    result = 84 * pVisibleWindowsIdxs[k];
    for ( l = pWindowList[pVisibleWindowsIdxs[k] - 1].pControlsHead; l; l = l->pNext )
    {
      LOBYTE(result) = old_hot_key;
      if ( l->uHotkey == old_hot_key )
      {
        if ( !l->field_28 )
        {
          LOBYTE(result) = v9;
          l->field_28 = 1;
          l->uHotkey = v9;
        }
      }
    }
  }
  return result;
}

//----- (0041B438) --------------------------------------------------------
GUIButton *__fastcall GUI_HandleHotkey(unsigned __int8 uHotkey)
{
  char Hot_key_num; // al@1
  GUIWindow *current_window; // ecx@2
  GUIButton *result; // eax@2

  Hot_key_num = toupper(uHotkey);
  for( int i = uNumVisibleWindows; i >= 0 && pVisibleWindowsIdxs[i] > 0; i-- )
  {
	current_window = &pWindowList[pVisibleWindowsIdxs[i] - 1];
	for ( result = current_window->pControlsHead; result; result = result->pNext )
	{
	  if ( result->uHotkey == Hot_key_num )
	  {
		pMessageQueue_50CBD0->AddGUIMessage(result->msg, result->msg_param, 0);
		return result;
	  }
	}
	if ( !current_window->uFrameX && !current_window->uFrameY
		&& (current_window->uFrameWidth == window->GetWidth() && current_window->uFrameHeight == window->GetWidth()) )
	  break;
  }
  return 0;
}
// 5075E0: using guessed type int pVisibleWindowsIdxs[20];

//----- (0041D73D) --------------------------------------------------------
void GUIWindow::_41D73D_draw_buff_tooltip()
    {
 
  __int64 remaing_time; // ST28_8@11
  unsigned short text_color;
  int Y_pos; // esi@11
  int string_count; // [sp+20h] [bp-4h]@7


  string_count = 0;
  for (int i=0; i<20; ++i)
    if ( pParty->pPartyBuffs[i].uExpireTime > 0i64 )
      ++string_count;

  uFrameHeight = pFontArrus->uFontHeight + 72;
  uFrameHeight += (string_count - 1) * pFontArrus->uFontHeight;
  uFrameZ = uFrameWidth + uFrameX - 1;
  uFrameW = uFrameY + uFrameHeight - 1;
  DrawMessageBox(0);
  DrawTitleText(pFontArrus, 0, 12, 0, pGlobalTXT_LocalizationStrings[451], 3u);
  if ( !string_count )
     DrawTitleText(pFontComic, 0, 40, 0, pGlobalTXT_LocalizationStrings[153], 3u);

  GetTickCount();
  string_count = 0;
  for (int i=0; i<20; ++i)
  {
    if ( pParty->pPartyBuffs[i].uExpireTime>0i64 )//!!!
    {
      remaing_time = pParty->pPartyBuffs[i].uExpireTime- pParty->uTimePlayed;//!!!
      Y_pos = string_count * pFontComic->uFontHeight + 40; 
      text_color = Color16(spell_tooltip_colors[i].R, spell_tooltip_colors[i].G, spell_tooltip_colors[i].B);
      DrawText(pFontComic, 52, Y_pos, text_color, aSpellNames[i], 0, 0, 0);
      DrawBuff_remaining_time_string(Y_pos, this, remaing_time, pFontComic); 
      ++string_count;
        }
  }
}


//----- (0041D08F) --------------------------------------------------------
void GUIWindow::_41D08F_set_keyboard_control_group(int num_buttons, int a3, int a4, int a5)
{
  if (num_buttons)
  {
    this->pNumPresenceButton = num_buttons;
    this->field_30 = a3;
    this->field_34 = a4;
    this->pCurrentPosActiveItem = a5;
    this->pStartingPosActiveItem = a5;
    this->receives_keyboard_input = true;
  }
  else
  {
    this->pNumPresenceButton = 0;
    this->field_30 = a3;
    this->field_34 = a4;
    this->pCurrentPosActiveItem = 0;
    this->pStartingPosActiveItem = 0;
    this->receives_keyboard_input = false;
  }
}


//----- (0041C26A) --------------------------------------------------------
void GUIWindow::Release()
{
  //GUIWindow *v1; // esi@1
  int i; // edi@20
  //GUIButton *v8; // eax@26
  GUIButton *pNextBtn; // edi@27
  //int v10; // esi@28
  //int v11; // ecx@28
  int v12; // edx@29

  //v1 = this;
  if ( !this )
    return;
  
  switch( this->eWindowType )
  {
	case WINDOW_GreetingNPC:
		{
		pIcons_LOD->SyncLoadedFilesCount();
		pCurrentScreen = pMainScreenNum;
		pKeyActionMap->SetWindowInputStatus(WINDOW_INPUT_CANCELLED);
		break;
		}
	case WINDOW_HouseInterior:
		{
		for ( i = 0; i < uNumDialogueNPCPortraits; ++i )
			pDialogueNPCPortraits[i]->Release();
		uNumDialogueNPCPortraits = 0;
		pTexture_Dialogue_Background->Release();

		pIcons_LOD->SyncLoadedFilesCount();
		pIcons_LOD->RemoveTexturesPackFromTextureList();
		dword_5C35D4 = 0;
		if ( bFlipOnExit )
		{
          pParty->sRotationY = (stru_5C6E00->uIntegerDoublePi - 1) & (stru_5C6E00->uIntegerPi + pParty->sRotationY);
          pGame->pIndoorCameraD3D->sRotationY = pParty->sRotationY;
		}
		pParty->uFlags |= 2u;
		break;
		}
	case WINDOW_Transition:
		{
		//pVideoPlayer->Unload();
		pTexture_outside->Release();
		pTexture_Dialogue_Background->Release();
		pIcons_LOD->SyncLoadedFilesCount();
		pCurrentScreen = pMainScreenNum;
		break;
		}
	case WINDOW_SpellBook:
		{
		OnCloseSpellBookPage();
		OnCloseSpellBook();
		break;
		}
	case WINDOW_Book:
		{
		OnCloseBook();
		break;
		}
      case WINDOW_ChangeLocation:
      {
        pTexture_outside->Release();
        pTexture_Dialogue_Background->Release();
        pIcons_LOD->SyncLoadedFilesCount();
        pCurrentScreen = pMainScreenNum;
        break;
		}
	case WINDOW_Dialogue:
		{
        if ( !dword_591084 )
	        pDialogueNPCPortraits[0]->Release();
        uNumDialogueNPCPortraits = 0;
        pTexture_Dialogue_Background->Release();

        pIcons_LOD->SyncLoadedFilesCount();
        pCurrentScreen = pMainScreenNum;
        break;
		}
  case WINDOW_null:
    return;
	default:
		break;
  }
  //v8 = this->pControlsHead;
  if ( this->pControlsHead )
  {
    do
    {
      pNextBtn = this->pControlsHead->pNext;
      free(this->pControlsHead);
      this->pControlsHead = pNextBtn;
    }
    while ( pNextBtn );
  }
  this->pControlsHead = 0;
  this->pControlsTail = 0;
  this->uNumControls = 0;
  this->eWindowType = WINDOW_null;
  while ( this->numVisibleWindows < uNumVisibleWindows )
  {
    v12 = pVisibleWindowsIdxs[this->numVisibleWindows + 1];
    pVisibleWindowsIdxs[this->numVisibleWindows] = v12;
    --pWindowList[v12 - 1].numVisibleWindows;
    ++this->numVisibleWindows;
  }
  pVisibleWindowsIdxs[uNumVisibleWindows] = 0;
  uNumVisibleWindows = uNumVisibleWindows - 1;
}

//----- (0041CD3B) --------------------------------------------------------
GUIButton *GUIWindow::GetControl(unsigned int uID)
{
  GUIButton *result; // eax@1

  result = this->pControlsHead;
  for ( uID; uID; --uID )
    result = result->pNext;
  return result;
}

//----- (00411BFC) --------------------------------------------------------
void GUIWindow::InitializeBookView()
{
  char *pString; // eax@12
  int pTextHeight; // eax@12
  //__int64 page_count; // qax@12
  unsigned int page_count; // esi@12
  unsigned __int16 v18; // ax@38
  signed int max_beacons; // [sp+10h] [bp-5Ch]@38
  GUIWindow journal_window; // [sp+18h] [bp-54h]@8

  pAudioPlayer->StopChannels(-1, -1);
  InitializeBookFonts();
  this->CreateButton(475, 445, 158, 34, 1, 0, UIMSG_Escape, 0, 0, pGlobalTXT_LocalizationStrings[79], 0); // Close
  pCurrentScreen = SCREEN_BOOKS;
  full_num_items_in_book = 0;
  books_primary_item_per_page = 0;
  books_page_number = 0;
  num_achieved_awards = 0; 
  switch (this->par1C)
  {
    case WINDOW_LloydsBeacon:
    {
      byte_506360 = 0;
      pTexture_CurrentBook = pIcons_LOD->LoadTexturePtr("lb_bordr", TEXTURE_16BIT_PALETTE);
      pTexture_LloydBeacons[0] = pIcons_LOD->LoadTexturePtr("sbmap", TEXTURE_16BIT_PALETTE);
      pTexture_LloydBeacons[1] = pIcons_LOD->LoadTexturePtr("sbmap", TEXTURE_16BIT_PALETTE);
      pTex_book_button1_on  = pIcons_LOD->LoadTexturePtr("tab-an-6b", TEXTURE_16BIT_PALETTE);
      pTex_book_button1_off = pIcons_LOD->LoadTexturePtr("tab-an-6a", TEXTURE_16BIT_PALETTE);

      pBtn_Book_1 = this->CreateButton(415, 13, 39, 36, 1, 0, UIMSG_LloydsBeacon_FlippingBtn, 0, 0, pGlobalTXT_LocalizationStrings[375], 0); // Set Beacon
      pBtn_Book_2 = this->CreateButton(415, 48, 39, 36, 1, 0, UIMSG_LloydsBeacon_FlippingBtn, 1, 0, pGlobalTXT_LocalizationStrings[523], 0); // Recall Beacon

      max_beacons = 1;
      v18 = pParty->pPlayers[_506348_current_lloyd_playerid].pActiveSkills[PLAYER_SKILL_WATER];
      if ( v18 & 0x100 || (v18 & 0x80) )
          max_beacons = 5;
      else if ( v18 & 0x40 )
          max_beacons = 3;

      for ( int i = 0; i < max_beacons; ++i )
            CreateButton(pLloydsBeaconsPreviewXs[i], pLloydsBeaconsPreviewYs[i],
                           92, 68, 1, 180, UIMSG_InstallBeacon, i, 0, "", 0);

      for ( int i = 0; i < 5; ++i )
      {
        if (pParty->pPlayers[_506348_current_lloyd_playerid].pInstalledBeacons[i].uBeaconTime  >= (signed __int64)pParty->uTimePlayed)
            LoadThumbnailLloydTexture(i, _506348_current_lloyd_playerid + 1);
        else 
          memset(&pParty->pPlayers[_506348_current_lloyd_playerid].pInstalledBeacons[i], 0, sizeof(LloydBeacon));
      }
    }
    break;

    case WINDOW_TownPortal:
    {
      pTexture_CurrentBook        = pIcons_LOD->LoadTexturePtr("townport", TEXTURE_16BIT_PALETTE);
      pTexture_TownPortalIcons[0] = pIcons_LOD->LoadTexturePtr("tpharmndy", TEXTURE_16BIT_PALETTE);
      pTexture_TownPortalIcons[1] = pIcons_LOD->LoadTexturePtr("tpelf", TEXTURE_16BIT_PALETTE);
      pTexture_TownPortalIcons[2] = pIcons_LOD->LoadTexturePtr("tpwarlock", TEXTURE_16BIT_PALETTE);
      pTexture_TownPortalIcons[3] = pIcons_LOD->LoadTexturePtr("tpisland", TEXTURE_16BIT_PALETTE);
      pTexture_TownPortalIcons[4] = pIcons_LOD->LoadTexturePtr("tpheaven", TEXTURE_16BIT_PALETTE);
      pTexture_TownPortalIcons[5] = pIcons_LOD->LoadTexturePtr("tphell", TEXTURE_16BIT_PALETTE);
      
      static int pTownPortalBook_ws[6] = { 80,  66,  68,  72,  67,  74};
      static int pTownPortalBook_hs[6] = { 55,  56,  65,  67,  67,  59};
      for ( uint i = 0; i < 6; ++i )
        this->CreateButton(pTownPortalBook_xs[i], pTownPortalBook_ys[i], pTownPortalBook_ws[i], pTownPortalBook_hs[i], 1, 182, UIMSG_ClickTownInTP, i, 0, "", nullptr);
      
    }
    break;

  case WINDOW_QuestBook:
  {
      pTexture_CurrentBook    = pIcons_LOD->LoadTexturePtr("sbquiknot", TEXTURE_16BIT_PALETTE);
      pSpellBookPagesTextr_10 = pIcons_LOD->LoadTexturePtr( "divbar", TEXTURE_16BIT_PALETTE);
      pTex_book_button1_on = pIcons_LOD->LoadTexturePtr("tab-an-6b", TEXTURE_16BIT_PALETTE);
      pTex_book_button2_on = pIcons_LOD->LoadTexturePtr("tab-an-7b", TEXTURE_16BIT_PALETTE);
      pTex_book_button1_off = pIcons_LOD->LoadTexturePtr("tab-an-6a", TEXTURE_16BIT_PALETTE);
      pTex_book_button2_off = pIcons_LOD->LoadTexturePtr("tab-an-7a", TEXTURE_16BIT_PALETTE);
      pBtn_Book_1 = this->CreateButton(pViewport->uViewportTL_X + 398,         pViewport->uViewportTL_Y + 1,
                                     pTex_book_button1_on->uTextureWidth, pTex_book_button1_on->uTextureHeight,
                                     1, 0, UIMSG_ClickBooksBtn, 0xBu, 0, pGlobalTXT_LocalizationStrings[192],// "Scroll Up"
                                     pTex_book_button1_on, 0);
      pBtn_Book_2 = this->CreateButton(pViewport->uViewportTL_X + 398,          pViewport->uViewportTL_Y + 38,
                                     pTex_book_button2_on->uTextureHeight, pTex_book_button2_on->uTextureHeight,
                                     1, 0, UIMSG_ClickBooksBtn, 0xAu, 0, pGlobalTXT_LocalizationStrings[193],// "Scroll Down"
                                     pTex_book_button2_on, 0);
      num_achieved_awards = 0;
      memset(achieved_awards.data(), 0, 4000);
      for ( uint i = books_primary_item_per_page; i < 512; ++i )
      {
        if ( _449B57_test_bit(pParty->_quest_bits, i) && pQuestTable[i] )
        {
          achieved_awards[num_achieved_awards] = (AwardType)i;
          ++num_achieved_awards;
        }
      }
      full_num_items_in_book = num_achieved_awards;
      num_achieved_awards = 0;
    }
    break;

    case WINDOW_AutonotesBook:
    {
      pTexture_AutonotesBook   = pIcons_LOD->LoadTexturePtr("sbautnot", TEXTURE_16BIT_PALETTE);
      pSpellBookPagesTextr_10  = pIcons_LOD->LoadTexturePtr("divbar", TEXTURE_16BIT_PALETTE);
      pTex_book_button1_on  = pIcons_LOD->LoadTexturePtr("tab-an-6b", TEXTURE_16BIT_PALETTE);
      pTex_book_button2_on  = pIcons_LOD->LoadTexturePtr("tab-an-7b", TEXTURE_16BIT_PALETTE);
      pTex_book_button1_off = pIcons_LOD->LoadTexturePtr("tab-an-6a", TEXTURE_16BIT_PALETTE);
      pTex_book_button2_off = pIcons_LOD->LoadTexturePtr("tab-an-7a", TEXTURE_16BIT_PALETTE);
      pTex_book_button3_on = pIcons_LOD->LoadTexturePtr("tab-an-1b", TEXTURE_16BIT_PALETTE);
      pTex_book_button3_off = pIcons_LOD->LoadTexturePtr("tab-an-1a", TEXTURE_16BIT_PALETTE);
      pTex_book_button4_on = pIcons_LOD->LoadTexturePtr("tab-an-2b", TEXTURE_16BIT_PALETTE);
      pTex_book_button4_off = pIcons_LOD->LoadTexturePtr("tab-an-2a", TEXTURE_16BIT_PALETTE);
      pTex_book_button5_on = pIcons_LOD->LoadTexturePtr("tab-an-3b", TEXTURE_16BIT_PALETTE);
      pTex_book_button5_off = pIcons_LOD->LoadTexturePtr("tab-an-3a", TEXTURE_16BIT_PALETTE);
      pTex_book_button6_on = pIcons_LOD->LoadTexturePtr("tab-an-5b", TEXTURE_16BIT_PALETTE);
      pTex_book_button6_off = pIcons_LOD->LoadTexturePtr("tab-an-5a", TEXTURE_16BIT_PALETTE);
      pTex_book_button7_on = pIcons_LOD->LoadTexturePtr("tab-an-4b", TEXTURE_16BIT_PALETTE);
      pTex_book_button7_off = pIcons_LOD->LoadTexturePtr("tab-an-4a", TEXTURE_16BIT_PALETTE);
      pTex_book_button8_on = pIcons_LOD->LoadTexturePtr("tab-an-8b", TEXTURE_16BIT_PALETTE);
      pTex_book_button8_off = pIcons_LOD->LoadTexturePtr("tab-an-8a", TEXTURE_16BIT_PALETTE);

      pBtn_Book_1                = this->CreateButton(pViewport->uViewportTL_X + 398, pViewport->uViewportTL_Y + 1,   50, 34, 1, 0, 
                                                    UIMSG_ClickBooksBtn, 11, 0, pGlobalTXT_LocalizationStrings[193], pTex_book_button1_on, 0);
      pBtn_Book_2                = this->CreateButton(pViewport->uViewportTL_X + 398, pViewport->uViewportTL_Y + 38,  50, 34, 1, 0, 
                                                    UIMSG_ClickBooksBtn, 10, 0, pGlobalTXT_LocalizationStrings[192], pTex_book_button2_on, 0);
      pBtn_Book_3                = this->CreateButton(pViewport->uViewportTL_X + 398, pViewport->uViewportTL_Y + 113, 50, 34, 1, 0, 
                                                    UIMSG_ClickBooksBtn,  2, 0, pGlobalTXT_LocalizationStrings[85], pTex_book_button3_on, 0); // "Potion Notes"
      pBtn_Book_4                = this->CreateButton(pViewport->uViewportTL_X + 399, pViewport->uViewportTL_Y + 150, 50, 34, 1, 0, 
                                                    UIMSG_ClickBooksBtn,  3, 0, pGlobalTXT_LocalizationStrings[137], pTex_book_button4_on, 0); // "Fountain Notes"
      pBtn_Book_5                = this->CreateButton(pViewport->uViewportTL_X + 397, pViewport->uViewportTL_Y + 188, 50, 34, 1, 0, 
                                                    UIMSG_ClickBooksBtn,  4, 0, pGlobalTXT_LocalizationStrings[8], pTex_book_button5_on, 0); // "Obelisk Notes"
      pBtn_Book_6                = this->CreateButton(pViewport->uViewportTL_X + 397, pViewport->uViewportTL_Y + 226, 50, 34, 1, 0, 
                                                    UIMSG_ClickBooksBtn,  5, 0, pGlobalTXT_LocalizationStrings[141], pTex_book_button6_on, 0); // "Seer Notes"
      pBtn_Autonotes_Misc        = this->CreateButton(pViewport->uViewportTL_X + 397, pViewport->uViewportTL_Y + 264, 50, 34, 1, 0, 
                                                    UIMSG_ClickBooksBtn,  6, 0, pGlobalTXT_LocalizationStrings[123], pTex_book_button7_on, 0); // "Miscellaneous Notes"
      pBtn_Autonotes_Instructors = this->CreateButton(pViewport->uViewportTL_X + 397, pViewport->uViewportTL_Y + 302, 50, 34, 1, 0, 
                                                    UIMSG_ClickBooksBtn,  7, 0, pGlobalTXT_LocalizationStrings[662], pTex_book_button8_on, 0); // "Instructors"

      num_achieved_awards = 0;
      for ( uint i = books_primary_item_per_page; i < 196; ++i )
      {
        if ( _506568_autonote_type == pAutonoteTxt[i].eType)//dword_72371C[2 * v10] )
        {
          if ( i )
          {
            if ( _449B57_test_bit(pParty->_autonote_bits, i) && pAutonoteTxt[i].pText )
            {
              achieved_awards[num_achieved_awards] = (AwardType)i;
              ++num_achieved_awards;
            }
          }
        }
      }
      full_num_items_in_book = num_achieved_awards;
      num_achieved_awards = 0;
    }
    break;

    case WINDOW_MapsBook:
    {
      dword_506364 = 1;
      pSpellBookPagesTextr_12  = pIcons_LOD->LoadTexturePtr("sbmap", TEXTURE_16BIT_PALETTE);
      pTex_book_button1_on  = pIcons_LOD->LoadTexturePtr("zoom-on", TEXTURE_16BIT_PALETTE);
      pTex_book_button2_on  = pIcons_LOD->LoadTexturePtr("zoot-on", TEXTURE_16BIT_PALETTE);
      pTex_book_button1_off = pIcons_LOD->LoadTexturePtr("zoom-off", TEXTURE_16BIT_PALETTE);
      pTex_book_button2_off = pIcons_LOD->LoadTexturePtr("zoot-off", TEXTURE_16BIT_PALETTE);
      pTex_book_button3_on = pIcons_LOD->LoadTexturePtr("tabNon", TEXTURE_16BIT_PALETTE);
      pTex_book_button3_off = pIcons_LOD->LoadTexturePtr("tabNoff", TEXTURE_16BIT_PALETTE);
      pTex_book_button4_on = pIcons_LOD->LoadTexturePtr("tabSon", TEXTURE_16BIT_PALETTE);
      pTex_book_button4_off = pIcons_LOD->LoadTexturePtr("tabSoff", TEXTURE_16BIT_PALETTE);
      pTex_book_button5_on = pIcons_LOD->LoadTexturePtr("tabEon", TEXTURE_16BIT_PALETTE);
      pTex_book_button5_off = pIcons_LOD->LoadTexturePtr("tabEoff", TEXTURE_16BIT_PALETTE);
      pTex_book_button6_on = pIcons_LOD->LoadTexturePtr("tabWon", TEXTURE_16BIT_PALETTE);
      pTex_book_button6_off = pIcons_LOD->LoadTexturePtr("tabWoff", TEXTURE_16BIT_PALETTE);

      pBtn_Book_1 = this->CreateButton(pViewport->uViewportTL_X + 398, pViewport->uViewportTL_Y + 1,   50, 34, 1, 0, 
                                     UIMSG_ClickBooksBtn, 0, 0, pGlobalTXT_LocalizationStrings[251], pTex_book_button1_on, 0);// "Zoom In"
      pBtn_Book_2 = this->CreateButton(pViewport->uViewportTL_X + 398, pViewport->uViewportTL_Y + 38,  50, 34, 1, 0, 
                                     UIMSG_ClickBooksBtn, 1, 0, pGlobalTXT_LocalizationStrings[252], pTex_book_button2_on, 0);// "Zoom Out"
      pBtn_Book_3 = this->CreateButton(pViewport->uViewportTL_X + 397, pViewport->uViewportTL_Y + 113, 50, 34, 1, 0, 
                                     UIMSG_ClickBooksBtn, 2, 0, pGlobalTXT_LocalizationStrings[192], (Texture *)"", 0);// Scroll Up
      pBtn_Book_4 = this->CreateButton(pViewport->uViewportTL_X + 397, pViewport->uViewportTL_Y + 150, 50, 34, 1, 0, 
                                     UIMSG_ClickBooksBtn, 3, 0, pGlobalTXT_LocalizationStrings[193], (Texture *)"", 0);// Scroll Down
      pBtn_Book_5 = this->CreateButton(pViewport->uViewportTL_X + 397, pViewport->uViewportTL_Y + 188, 50, 34, 1, 0, 
                                     UIMSG_ClickBooksBtn, 4, 0, pGlobalTXT_LocalizationStrings[573], (Texture *)"", 0);// "Scroll Right"
      pBtn_Book_6 = this->CreateButton(pViewport->uViewportTL_X + 397, pViewport->uViewportTL_Y + 226, 50, 34, 1, 0, 
                                      UIMSG_ClickBooksBtn, 5, 0, pGlobalTXT_LocalizationStrings[572], (Texture *)"", 0);// "Scroll Left"
    }
    break;

    case WINDOW_CalendarBook:
    {
      pSpellBookPagesTextr_13 = pIcons_LOD->LoadTexturePtr("sbdate-time", TEXTURE_16BIT_PALETTE);
      pTex_moon_new = pIcons_LOD->LoadTexturePtr("moon_new", TEXTURE_16BIT_PALETTE);
      pTex_moon_4   = pIcons_LOD->LoadTexturePtr("moon_4", TEXTURE_16BIT_PALETTE);
      pTex_moon_2   = pIcons_LOD->LoadTexturePtr("moon_2", TEXTURE_16BIT_PALETTE);
      pTex_moon_2_2 = pIcons_LOD->LoadTexturePtr("moon_2", TEXTURE_16BIT_PALETTE);
      pTex_moon_ful = pIcons_LOD->LoadTexturePtr("moon_ful", TEXTURE_16BIT_PALETTE);
    }
    break;

    case WINDOW_JournalBook:
    {
      pSpellBookPagesTextr_11  = pIcons_LOD->LoadTexturePtr("sbplayrnot", TEXTURE_16BIT_PALETTE);
      pTex_book_button1_on  = pIcons_LOD->LoadTexturePtr("tab-an-6b", TEXTURE_16BIT_PALETTE);
      pTex_book_button2_on  = pIcons_LOD->LoadTexturePtr("tab-an-7b", TEXTURE_16BIT_PALETTE);
      pTex_book_button1_off = pIcons_LOD->LoadTexturePtr("tab-an-6a", TEXTURE_16BIT_PALETTE);
      pTex_book_button2_off = pIcons_LOD->LoadTexturePtr("tab-an-7a", TEXTURE_16BIT_PALETTE);

      pBtn_Book_1 = this->CreateButton(pViewport->uViewportTL_X + 398, pViewport->uViewportTL_Y + 1,  
                      pTex_book_button1_on->uTextureWidth,  pTex_book_button1_on->uTextureHeight, 1, 0, 
                      UIMSG_ClickBooksBtn, 11, 0, pGlobalTXT_LocalizationStrings[192], pTex_book_button1_on, 0);
      pBtn_Book_2 = this->CreateButton(pViewport->uViewportTL_X + 398, pViewport->uViewportTL_Y + 38, pTex_book_button2_on->uTextureHeight,
                                      pTex_book_button2_on->uTextureHeight, 1, 0, UIMSG_ClickBooksBtn, 10, 0, 
                                      pGlobalTXT_LocalizationStrings[193], pTex_book_button2_on, 0);

      num_achieved_awards = 0;
      journal_window.uFrameX = 48;
      journal_window.uFrameY = 70;
      journal_window.uFrameWidth = 360;
      journal_window.uFrameHeight = 264;
      journal_window.uFrameZ = 407;
      journal_window.uFrameHeight = (LOBYTE(pAutonoteFont->uFontHeight) - 3) * 264 / LOBYTE(pAutonoteFont->uFontHeight) - 3;
      journal_window.uFrameW = journal_window.uFrameHeight + 69;
      memset(&achieved_awards, 0, 4000);
      memset(Journal_limitation_factor.data(), 0, 100);
      if ( books_primary_item_per_page < 29 )
      {
        for ( int i = books_primary_item_per_page; i < books_primary_item_per_page + 29; i++ )
        {
          if ( pParty->PartyTimes.HistoryEventTimes[i] > 0 )
          {
            if ( pStorylineText->StoreLine[i + 1].pText )
            {
              pString = BuildDialogueString(pStorylineText->StoreLine[i + 1].pText, uActiveCharacter - 1, 0, 0, 0, &pParty->PartyTimes.HistoryEventTimes[i]);
              pTextHeight = pAutonoteFont->CalcTextHeight(pString, &journal_window, 1, 0);
              page_count = ((pTextHeight - (pAutonoteFont->uFontHeight - 3)) / (signed int)journal_window.uFrameHeight) + 1;
              memset32((char *)&achieved_awards[num_achieved_awards] , i + 1, page_count);
              for ( uint j = 0; j <= page_count - 1; ++j )
                Journal_limitation_factor[num_achieved_awards++] = j;
            }
          }
        }
      }
      full_num_items_in_book = num_achieved_awards;
      num_achieved_awards = 0;
    }
    break;
  }
}

//----- (00415551) --------------------------------------------------------
void GUIWindow::DrawMessageBox(int arg0)
{
  unsigned int v2; // edi@1
  signed int v4; // esi@2
  unsigned int v5; // eax@2
  unsigned int v16; // esi@19
  GUIWindow current_window; // [sp+Ch] [bp-60h]@18
  POINT cursor; // [sp+60h] [bp-Ch]@8
  unsigned int v22; // [sp+74h] [bp+8h]@2

  v2 = 0;
  if ( arg0 )
  {
    v4 = pViewport->uViewportTL_X;
    v5 = pViewport->uViewportBR_X;
    v2 = pViewport->uViewportTL_Y;
    v22 = pViewport->uViewportBR_Y;
  }
  else
  {
    v4 = 0;
    v5 = window->GetWidth();
    v22 = window->GetHeight();
  }
  pMouse->GetCursorPos(&cursor);
  if ( (signed int)this->uFrameX >= v4 )
  {
    if ( (signed int)(this->uFrameWidth + this->uFrameX) > (signed int)v5 )
    {
      this->uFrameX = v5 - this->uFrameWidth;
      this->uFrameY = cursor.y + 30;
    }
  }
  else
  {
    this->uFrameX = v4;
    this->uFrameY = cursor.y + 30;
  }

  if ( (signed int)this->uFrameY >= (signed int)v2 )
  {
    if ( (signed int)(this->uFrameY + this->uFrameHeight) > (signed int)v22 )
      this->uFrameY = cursor.y - this->uFrameHeight - 30;
  }
  else
    this->uFrameY = cursor.y + 30;
  if ( (signed int)this->uFrameY < (signed int)v2 )
    this->uFrameY = v2;
  if ( (signed int)this->uFrameX < v4 )
    this->uFrameX = v4;
  this->uFrameZ = this->uFrameWidth + this->uFrameX - 1;
  this->uFrameW = this->uFrameHeight + this->uFrameY - 1;
  memcpy(&current_window, this, sizeof(current_window));
  current_window.uFrameX += 12;
  current_window.uFrameWidth -= 24;
  current_window.uFrameY += 12;
  current_window.uFrameHeight -= 12;
  current_window.uFrameZ = current_window.uFrameWidth + current_window.uFrameX - 1;
  current_window.uFrameW = current_window.uFrameHeight + current_window.uFrameY - 1;
  if ( this->Hint )
    v16 = pFontLucida->CalcTextHeight(this->Hint, &current_window, 0, 0) + 24;
  else
    v16 = this->uFrameHeight;
  if ( (signed int)v16 < 64 )
    v16 = 64;
  if ( (signed int)(v16 + this->uFrameY) > 479 )
    v16 = 479 - this->uFrameY;
  DrawPopupWindow(this->uFrameX, this->uFrameY, this->uFrameWidth, v16);
  if ( this->Hint )
    current_window.DrawTitleText(pFontLucida, 0, (signed int)(v16 - pFontLucida->CalcTextHeight(this->Hint, &current_window, 0, 0)) / 2 - 14, 0, this->Hint, 3);
}

//----- (00411B59) --------------------------------------------------------
void __fastcall LoadThumbnailLloydTexture(unsigned int uSlot, unsigned int uPlayer)
{
  //unsigned int v2; // esi@1
  //unsigned int v3; // edi@1
  FILE *v4; // ebx@1
  FILE *v5; // eax@2
  char pContainerName[64]; // [sp+Ch] [bp-44h]@1
  //unsigned int v7; // [sp+4Ch] [bp-4h]@1

  //v2 = uSlot;
  //v7 = uPlayer;
  //v3 = uSlot + 1;
  sprintf(pContainerName, "data\\lloyd%d%d.pcx", uPlayer, uSlot + 1);
  v4 = fopen(pContainerName, "rb");
  if ( v4 )
  {
    pSavegameThumbnails[uSlot].LoadFromFILE(v4, 0, 1);
    fclose(v4);
  }
  else
  {
    sprintf(pContainerName, "lloyd%d%d.pcx", uPlayer, uSlot + 1);
    v5 = pNew_LOD->FindContainer(pContainerName, 1);
    if ( v5 )
      pSavegameThumbnails[uSlot].LoadFromFILE(v5, 0, 0);
    else
      *((int *)&pSavegameThumbnails.data()->pPixels + 10 * uSlot) = 0;
  }
}


//----- (00411621) --------------------------------------------------------
void GUIWindow::OpenSpellBook()
{
  Player *pPlayer; // edi@1
  //GUIWindow *pWindow; // esi@1
  //unsigned int v3; // ebp@1
  int v4; // eax@3
  ///GUIButton *result; // eax@25
  int a2; // [sp+10h] [bp-8h]@1
  //int v7; // [sp+14h] [bp-4h]@1

  pPlayer = pPlayers[uActiveCharacter];
  //pWindow = this;
  LoadSpellbook(pPlayer->lastOpenedSpellbookPage);
  //v3 = 0;
  a2 = 0;

  PlayerSpellbookChapter* chapter = &pPlayer->spellbook.pChapters[pPlayer->lastOpenedSpellbookPage];
  for (uint i = 0; i < 11; ++i)
  {
    if (!chapter->bIsSpellAvailable[i])
      continue;
		v4= pPlayer->lastOpenedSpellbookPage;
      //v4 = (12 * pPlayer->lastOpenedSpellbookPage + pSpellbookSpellIndices[pPlayer->lastOpenedSpellbookPage][i + 1]);
      CreateButton(pViewport->uViewportTL_X +  pIconPos[v4][pSpellbookSpellIndices[v4][i+1]].Xpos,
                   pViewport->uViewportTL_Y +  pIconPos[v4][pSpellbookSpellIndices[v4][i+1]].Ypos,  //dword_4E20D0
                   SBPageSSpellsTextureList[i + 1]->uTextureWidth,
                   SBPageSSpellsTextureList[i + 1]->uTextureHeight,
                   1, 79, UIMSG_SelectSpell, i, 0, "", 0);
      ++a2;
    //++v3;
  }
  //while ( (signed int)v3 < 11 );

  CreateButton(0, 0, 0, 0, 1, 0, UIMSG_SpellBook_PressTab, 0, '\t', "", 0);
  if ( a2 )
    _41D08F_set_keyboard_control_group(a2, 0, 0, 0);

  if (pPlayer->pActiveSkills[PLAYER_SKILL_FIRE])   CreateButton(399,  10, 50, 36, 1, 0, UIMSG_OpenSpellbookPage, 0, 0, aSpellSchoolNames[0], 0);
  if (pPlayer->pActiveSkills[PLAYER_SKILL_AIR])    CreateButton(399,  46, 50, 36, 1, 0, UIMSG_OpenSpellbookPage, 1, 0, aSpellSchoolNames[1], 0);
  if (pPlayer->pActiveSkills[PLAYER_SKILL_WATER])  CreateButton(399,  83, 50, 36, 1, 0, UIMSG_OpenSpellbookPage, 2, 0, aSpellSchoolNames[2], 0);
  if (pPlayer->pActiveSkills[PLAYER_SKILL_EARTH])  CreateButton(399, 121, 50, 36, 1, 0, UIMSG_OpenSpellbookPage, 3, 0, aSpellSchoolNames[3], 0);
  if (pPlayer->pActiveSkills[PLAYER_SKILL_SPIRIT]) CreateButton(399, 158, 50, 36, 1, 0, UIMSG_OpenSpellbookPage, 4, 0, aSpellSchoolNames[4], 0);
  if (pPlayer->pActiveSkills[PLAYER_SKILL_MIND])   CreateButton(400, 196, 50, 36, 1, 0, UIMSG_OpenSpellbookPage, 5, 0, aSpellSchoolNames[5], 0);
  if (pPlayer->pActiveSkills[PLAYER_SKILL_BODY])   CreateButton(400, 234, 50, 36, 1, 0, UIMSG_OpenSpellbookPage, 6, 0, aSpellSchoolNames[6], 0);
  if (pPlayer->pActiveSkills[PLAYER_SKILL_LIGHT])  CreateButton(400, 271, 50, 36, 1, 0, UIMSG_OpenSpellbookPage, 7, 0, aSpellSchoolNames[7], 0);
  if (pPlayer->pActiveSkills[PLAYER_SKILL_DARK])   CreateButton(400, 307, 50, 36, 1, 0, UIMSG_OpenSpellbookPage, 8, 0, aSpellSchoolNames[8], 0);

                            CreateButton(476, 450, pSBClickQuickSpellBtnTextr->uTextureWidth, pSBClickQuickSpellBtnTextr->uTextureHeight, 1, 78, UIMSG_ClickInstallRemoveQuickSpellBtn, 0, 0, "", 0);
  pBtn_InstallRemoveSpell = CreateButton(476, 450, 48, 32, 1, 78, UIMSG_ClickInstallRemoveQuickSpellBtn, 0, 0, "", pSBClickQuickSpellBtnTextr, 0);
                            CreateButton(561, 450, pSpellBookClickCloseBtnTextr->uTextureWidth, pSpellBookClickCloseBtnTextr->uTextureHeight, 1,  0, UIMSG_Escape, 0, 0, pGlobalTXT_LocalizationStrings[79], 0);
  pBtn_CloseBook          = CreateButton(561, 450, 48, 32, 1, 0, UIMSG_Escape, 0, 0, pGlobalTXT_LocalizationStrings[79], pSpellBookClickCloseBtnTextr, 0);
}
// 50640C: using guessed type int dword_50640C[];

//----- (004B3157) --------------------------------------------------------
void GUIWindow::HouseDialogManager()
{
  unsigned __int16 pWhiteColor; // di@2
  const char *pHouseName; // edx@4
  signed int v3; // edx@5
  char *v4; // edi@9
  int pTextHeight; // eax@45
  int v6; // edi@45
  char *v7; // eax@45
  int v8; // edi@46
  int v9; // eax@50
  unsigned int v10; // [sp-10h] [bp-C8h]@53
  char *pTitleText; // [sp-8h] [bp-C0h]@50
  GUIWindow pDialogWindow; // [sp+Ch] [bp-ACh]@4
  GUIWindow pWindow; // [sp+60h] [bp-58h]@2
  int pColor2; // [sp+B4h] [bp-4h]@2

  if ( !window_SpeakInHouse )
    return;
  memcpy(&pWindow, this, sizeof(pWindow));
  pWindow.uFrameWidth -= 18;
  pWindow.uFrameZ -= 18;
  pWhiteColor = Color16(0xFFu, 0xFFu, 0xFFu);
  pColor2 = Color16(0x15u, 0x99u, 0xE9u);
  pRenderer->DrawTextureIndexed(0x1DDu, 0, pTexture_Dialogue_Background);
  pRenderer->DrawTextureTransparent(0x1D4u, 0, &pIcons_LOD->pTextures[uTextureID_right_panel_loop]);
  if ( pDialogueNPCCount != uNumDialogueNPCPortraits || !uHouse_ExitPic )
  {
    pDialogWindow.uFrameWidth = 130;
    pDialogWindow.uFrameHeight = 2 * LOBYTE(pFontCreate->uFontHeight);
    pHouseName = p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].pName;
    if ( pHouseName )
    {
      v3 = 2 * LOBYTE(pFontCreate->uFontHeight) - 6 - pFontCreate->CalcTextHeight(pHouseName, &pDialogWindow, 0, 0);
      if ( v3 < 0 )
        v3 = 0;
      pWindow.DrawTitleText(pFontCreate, 0x1EAu, v3 / 2 + 4, pWhiteColor,
        //(const char *)p2DEvents_minus1_::04[13 * (unsigned int)ptr_507BC0->ptr_1C],
        p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].pName, 3);
    }
  }
  pWindow.uFrameWidth += 8;
  pWindow.uFrameZ += 8;
  if ( !pDialogueNPCCount )
  {
    if ( in_current_building_type == BuildingType_Jail )
    {
      JailDialog();
      if ( pDialogueNPCCount == uNumDialogueNPCPortraits && uHouse_ExitPic )
      {
        pRenderer->DrawTextureIndexed(556, 451, &pIcons_LOD->pTextures[uTextureID_x_x_u]);
        pRenderer->DrawTextureIndexed(476, 451, &pIcons_LOD->pTextures[uTextureID_x_ok_u]);
      }
      else
        pRenderer->DrawTextureIndexed(471, 445, &pIcons_LOD->pTextures[uExitCancelTextureId]);
      return;
    }
    if ( current_npc_text )
    {
      pDialogWindow.uFrameWidth = 458;
      pDialogWindow.uFrameZ = 457;
      pTextHeight = pFontArrus->CalcTextHeight(current_npc_text, &pDialogWindow, 13, 0);
      v6 = pTextHeight + 7;
      pRenderer->GetLeather(8, 352 - (pTextHeight + 7), &pIcons_LOD->pTextures[uTextureID_Leather], 
          pIcons_LOD->pTextures[uTextureID_Leather].uTextureHeight - (pTextHeight + 7));
      pRenderer->DrawTextureIndexed(8, 347 - v6, pTexture_591428);
      v7 = FitTextInAWindow(current_npc_text, pFontArrus, &pDialogWindow, 0xDu, 0);
      window_SpeakInHouse->DrawText(pFontArrus, 13, 354 - v6, 0, v7, 0, 0, 0);
    }
    if ( uNumDialogueNPCPortraits <= 0 )
    {
      if ( pDialogueNPCCount == uNumDialogueNPCPortraits && uHouse_ExitPic )
      {
        pRenderer->DrawTextureIndexed(556, 451, &pIcons_LOD->pTextures[uTextureID_x_x_u]);
        pRenderer->DrawTextureIndexed(476, 451, &pIcons_LOD->pTextures[uTextureID_x_ok_u]);
      }
      else
        pRenderer->DrawTextureIndexed(471, 445, &pIcons_LOD->pTextures[uExitCancelTextureId]);
      return;
    }
    for ( v8 = 0; v8 < uNumDialogueNPCPortraits; ++v8 )
    {
      pRenderer->DrawTextureIndexed(pNPCPortraits_x[uNumDialogueNPCPortraits - 1][v8] - 4,
                                    pNPCPortraits_y[uNumDialogueNPCPortraits - 1][v8] - 4, &pIcons_LOD->pTextures[uTextureID_50795C]);
      pRenderer->DrawTextureIndexed(pNPCPortraits_x[uNumDialogueNPCPortraits - 1][v8],
                                    pNPCPortraits_y[uNumDialogueNPCPortraits - 1][v8], pDialogueNPCPortraits[v8]);
      if ( uNumDialogueNPCPortraits < 4 )
      {
        if ( v8 + 1 == uNumDialogueNPCPortraits && uHouse_ExitPic )
        {
          pTitleText = pMapStats->pInfos[uHouse_ExitPic].pName;
          v9 = 94 * v8 + 113;
        }
        else
        {
          if ( !v8 && dword_591080 )
          {
            pTitleText = (char *)p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].pProprieterTitle;
            pWindow.DrawTitleText(pFontCreate, 0x1E3u, 113, pColor2, pTitleText, 3);
            continue;
          }
          pTitleText = HouseNPCData[v8 +1 - (dword_591080 != 0)]->pName;
          v9 = pNPCPortraits_y[uNumDialogueNPCPortraits - 1][v8] + pDialogueNPCPortraits[v8]->uTextureHeight + 2;
        }
        v10 = v9;
        pWindow.DrawTitleText(pFontCreate, 483, v10, pColor2, pTitleText, 3);
      }
    }
      if ( pDialogueNPCCount == uNumDialogueNPCPortraits && uHouse_ExitPic )
      {
        pRenderer->DrawTextureIndexed(556, 451, &pIcons_LOD->pTextures[uTextureID_x_x_u]);
        pRenderer->DrawTextureIndexed(476, 451, &pIcons_LOD->pTextures[uTextureID_x_ok_u]);
      }
      else
        pRenderer->DrawTextureIndexed(471, 445, &pIcons_LOD->pTextures[uExitCancelTextureId]);
      return;
  }
  v4 = (char *)pDialogueNPCCount - 1;
  pRenderer->DrawTextureIndexed(pNPCPortraits_x[0][0] - 4, pNPCPortraits_y[0][0] - 4, &pIcons_LOD->pTextures[uTextureID_50795C]);
  pRenderer->DrawTextureIndexed(pNPCPortraits_x[0][0], pNPCPortraits_y[0][0], pDialogueNPCPortraits[(signed int)v4]);
  if ( pCurrentScreen == SCREEN_E )
  {
    CharacterUI_InventoryTab_Draw(pPlayers[uActiveCharacter], true);
    if ( pDialogueNPCCount == uNumDialogueNPCPortraits && uHouse_ExitPic )
    {
      pRenderer->DrawTextureIndexed(556, 451, &pIcons_LOD->pTextures[uTextureID_x_x_u]);
      pRenderer->DrawTextureIndexed(476, 451, &pIcons_LOD->pTextures[uTextureID_x_ok_u]);
    }
    else
      pRenderer->DrawTextureIndexed(471, 445, &pIcons_LOD->pTextures[uExitCancelTextureId]);
    return;
  }
  if ( v4 || !dword_591080 )//на изумрудном острове заходит на корабле пока не выполнены квесты
    SimpleHouseDialog();
  else
  {
    sprintfex( pTmpBuf.data(), pGlobalTXT_LocalizationStrings[429],
      p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].pProprieterName,
      p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].pProprieterTitle);
    pWindow.DrawTitleText(pFontCreate, 0x1E3u, 0x71u, pColor2, pTmpBuf.data(), 3);
      switch ( in_current_building_type )
      {
        case BuildingType_WeaponShop:
          WeaponShopDialog();
          break;
        case BuildingType_ArmorShop:
          ArmorShopDialog();
          break;
        case BuildingType_MagicShop:
          MagicShopDialog();
          break;
        case BuildingType_AlchemistShop:
          AlchemistDialog();
          break;
        case BuildingType_FireGuild:
        case BuildingType_AirGuild:
        case BuildingType_WaterGuild:
        case BuildingType_EarthGuild:
        case BuildingType_SpiritGuild:
        case BuildingType_MindGuild:
        case BuildingType_BodyGuild:
        case BuildingType_LightGuild:
        case BuildingType_DarkGuild:
          GuildDialog();
          break;
        case BuildingType_18:
          __debugbreak(); //What over the dialog?
          sub_4B6478();
          break;
        case BuildingType_TownHall:
          TownHallDialog();
          break;
        case BuildingType_Tavern:
          TavernDialog();
          break;
        case BuildingType_Bank:
          BankDialog();
          break;
        case BuildingType_Temple:
          TempleDialog();
          break;
        case BuildingType_Stables:
        case BuildingType_Boats:
          TravelByTransport();
          break;
        case BuildingType_Training:
          TrainingDialog();
          break;
        case BuildingType_Jail:
          JailDialog();
          break;
        default:
          //__debugbreak();//New BuildingType (if enter Boat)
          break;
      }
  }
  if ( pDialogueNPCCount == uNumDialogueNPCPortraits && uHouse_ExitPic )
  {
    pRenderer->DrawTextureIndexed(556, 451, &pIcons_LOD->pTextures[uTextureID_x_x_u]);
    pRenderer->DrawTextureIndexed(476, 451, &pIcons_LOD->pTextures[uTextureID_x_ok_u]);
  }
  else
    pRenderer->DrawTextureIndexed(471, 445, &pIcons_LOD->pTextures[uExitCancelTextureId]);
}

//----- (004B1854) --------------------------------------------------------
void GUIWindow::DrawShops_next_generation_time_string( __int64 next_generation_time )
{
  unsigned int full_time; // esi@1
  signed __int64 hours; // kr00_8@1
  const char *text; // eax@2
  signed __int64 minutes; // [sp+Ch] [bp-10h]@1
  signed __int64 seconds; // [sp+14h] [bp-8h]@1
  unsigned int days; // [sp+20h] [bp+4h]@1

  full_time = (signed __int64)((double)next_generation_time * 0.234375);
  seconds = (signed __int64)full_time % 60;
  minutes = (signed __int64)(full_time / 60) % 60;
  hours = ((full_time / 60) / 60) % 24;
  days = (unsigned int)((full_time / 60) / 60) / 24;
  strcpy(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[532]);
  if ( days )
  {
    text = pGlobalTXT_LocalizationStrings[57];//Days
    if ( days <= 1 )
      text = pGlobalTXT_LocalizationStrings[56];//Day
    sprintfex(pTmpBuf2.data(), "%d %s ", days, text);
    strcat(pTmpBuf.data(), pTmpBuf2.data());
  }
  if ( hours )
  {
    if ( hours <= 1 )
      text = pGlobalTXT_LocalizationStrings[109];//Hour
    else
      text = pGlobalTXT_LocalizationStrings[110];//Hours
    sprintfex(pTmpBuf2.data(), "%d %s ", (int)hours, text);
    strcat(pTmpBuf.data(), pTmpBuf2.data());
  }
  if ( minutes && !days )
  {
    if ( minutes <= 1 )
      text = pGlobalTXT_LocalizationStrings[437];//"Minute"
    else
      text = pGlobalTXT_LocalizationStrings[436]; //"Minutes"
    sprintfex(pTmpBuf2.data(), "%d %s ", (int)minutes, text);
    strcat(pTmpBuf.data(), pTmpBuf2.data());
  }
  if ( seconds && !hours )
  {
    if ( seconds <= 1 )
      text = pGlobalTXT_LocalizationStrings[439]; //"Second"	
    else
      text = pGlobalTXT_LocalizationStrings[438]; //"Seconds"
    sprintfex(pTmpBuf2.data(), "%d %s ", (int)seconds, text);
    strcat(pTmpBuf.data(), pTmpBuf2.data());
  }
  this->DrawTitleText(pFontArrus, 0, (212 - pFontArrus->CalcTextHeight(pTmpBuf.data(), this, 0, 0)) / 2 + 101, Color16(0xFFu, 0xFFu, 0x9Bu), pTmpBuf.data(), 3);
}

//----- (0044D406) --------------------------------------------------------
void GUIWindow::DrawTitleText( GUIFont *a2, signed int uHorizontalMargin, unsigned int uVerticalMargin, unsigned __int16 uDefaultColor, 
                               const char *pInString, unsigned int uLineSpacing )
{
  //GUIWindow *pWindow; // esi@1
  unsigned int v8; // ebx@1
  char *v9; // eax@1
  unsigned int v11; // edi@1
  signed int v12; // esi@1
  int v13; // eax@2
  GUIFont *pFont; // [sp+Ch] [bp-4h]@1
  const char *Stra; // [sp+24h] [bp+14h]@5

  //pWindow = this;
  pFont = a2;
  v8 = this->uFrameWidth - uHorizontalMargin;
  ui_current_text_color = uDefaultColor;
  v9 = FitTextInAWindow(pInString, a2, this, uHorizontalMargin, 0);
  Stra = strtok(v9, "\n");
  v11 = uHorizontalMargin + this->uFrameX;
  v12 = uVerticalMargin + this->uFrameY;
  while ( 1 )
  {
    if ( !Stra )
      break;
    v13 = (signed int)(v8 - pFont->GetLineWidth(Stra)) >> 1;
    if ( v13 < 0 )
      v13 = 0;
    pFont->DrawTextLine(uDefaultColor, v11 + v13, v12, Stra, window->GetWidth());
    v12 += pFont->uFontHeight - uLineSpacing;
    Stra = strtok(0, "\n");
  }
}
// 5C6DB4: using guessed type int ui_current_text_color;

//----- (0044CE08) --------------------------------------------------------
void GUIWindow::DrawText( GUIFont *a2, signed int uX, int uY, unsigned int uFontColor, const char *Str, int a7, int a8, signed int uFontShadowColor )
    {
  GUIWindow *v9; // edi@1
  GUIFont *v10; // ebx@1
  int v11; // eax@2
  signed int v12; // esi@9
  signed int v13; // edi@9
  int v14; // edx@9
  int v15; // eax@25
  unsigned int v16; // ecx@25
  int v17; // eax@27
  int v18; // edi@32
  int v19; // esi@38
  std::string v21; // [sp-18h] [bp-50h]@2
//  const char *v22; // [sp-8h] [bp-40h]@2
//  int v23; // [sp-4h] [bp-3Ch]@2
  char Dest[6]; // [sp+Ch] [bp-2Ch]@32
  //char v25; // [sp+Fh] [bp-29h]@32
  //char v26; // [sp+11h] [bp-27h]@34
  const char *v27; // [sp+20h] [bp-18h]@25
  int v28; // [sp+24h] [bp-14h]@25
  int v29; // [sp+28h] [bp-10h]@1
  size_t v30; // [sp+2Ch] [bp-Ch]@4
  GUIWindow *v31; // [sp+30h] [bp-8h]@1
  const char *v32; // [sp+34h] [bp-4h]@7
  size_t pInString; // [sp+4Ch] [bp+14h]@11
  
  GUIWindow* a1 = this;
  v29 = 0;
  v9 = a1;
  v10 = a2;
  v31 = a1;
  if ( !Str )
  {
    MessageBoxW(nullptr, L"Invalid string passed!", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Font.cpp:859", 0);
    return;
  }
  v11 = strcmp(Str, "null");
  if ( v11 )
  {
    v30 = strlen(Str);
    LOBYTE(v11) = 0;
    if ( !uX )
      uX = 12;
    if ( a8 )
      v32 = Str;
    else
    {
      v11 = (int)FitTextInAWindow(Str, v10, v9, uX, 0);
      v32 = (const char *)v11;
    }
    v12 = uX + v9->uFrameX;
    v13 = uY + v9->uFrameY;
    v14 = 0;
    if ( !a8 || (v11 = v13 + LOBYTE(v10->uFontHeight), v11 <= a8) )
    {
      pInString = 0;
      if ( (signed int)v30 > 0 )
      {
        do
        {
          LOBYTE(v11) = v32[v14];
          if ( (unsigned __int8)v11 >= v10->cFirstChar && (unsigned __int8)v11 <= v10->cLastChar
            || (char)v11 == 12
            || (char)v11 == 13
            || (char)v11 == 9
            || (char)v11 == 10 )
          {
            switch ( (unsigned __int8)v11 )
            {
              case 9u:
                strncpy(Dest, &v32[v14 + 1], 3);
                Dest[3] = 0;
                pInString += 3;
                v29 = atoi(Dest);
                v19 = atoi(Dest);
                LOBYTE(v11) = (char)v31;
                v12 = uX + v31->uFrameX + v19;
                break;
              case 0xAu:
                v11 = LOBYTE(v10->uFontHeight);
                uY = uY + v11 - 3;
                v13 = uY + v31->uFrameY;
                v12 = uX + v29 + v31->uFrameX;
                if ( a8 )
                {
                  v11 = v11 + v13 - 3;
                  if ( v11 > a8 )
                    return;
                }
                break;
              case 0xCu:
                strncpy(Dest, &v32[v14 + 1], 5);
                Dest[5] = 0;
                v11 = atoi(Dest);
                pInString += 5;
                uFontColor = v11;
                break;
              case 0xDu:
                strncpy(Dest, &v32[v14 + 1], 3);
                Dest[3] = 0;
                pInString += 3;
                v18 = atoi(Dest);
                v11 = v10->GetLineWidth(&v32[pInString]);
                v12 = v31->uFrameZ - v11 - v18;
                v13 = uY + v31->uFrameY;
                if ( a8 )
                {
                  v11 = LOBYTE(v10->uFontHeight);
                  v11 = v11 + v13 - 3;
                  if ( v11 > a8 )
                    return;
                  break;
                }
                break;
              default:
                if ( (char)v11 == 34 && v32[v14 + 1] == 34 )
                {
                  ++v14;
                  pInString = v14;
                }
                v27 = &v32[v14];
                v15 = (unsigned __int8)v32[v14];
                v16 = *((int *)&v10->cFirstChar + 3 * v15 + 9);
                v28 = *((int *)&v10->cFirstChar + 3 * v15 + 9);
                if ( v14 > 0 )
                  v12 += v10->pMetrics[v15].uLeftSpacing;
                v17 = (int)((char *)&v10[1] + v10->font_pixels_offset[v15]);
                if ( (short)uFontColor )
                  pRenderer->DrawText(v12, v13, (unsigned __int8 *)v17, v16, LOBYTE(v10->uFontHeight),
                    v10->pFontPalettes[0], uFontColor, uFontShadowColor);
                else
                  pRenderer->DrawTextPalette(v12, v13, (unsigned char*)v17, v16, LOBYTE(v10->uFontHeight),
                    v10->pFontPalettes[0], a7);
                LOBYTE(v11) = v30;
                v12 += v28;
                if ( (signed int)pInString < (signed int)v30 )
                {
                  LOBYTE(v11) = 3 * *v27;
                  v12 += v10->pMetrics[(unsigned __int8)*v27].uRightSpacing;
                }
                break;
            }
          }
          v14 = pInString++ + 1;
        }
        while ( (signed int)pInString < (signed int)v30 );
      }
    }
  }
  return;
}


//----- (0044CB4F) --------------------------------------------------------
int GUIWindow::DrawTextInRect( GUIFont *pFont, unsigned int uX, unsigned int uY, unsigned int uColor, const char *text, int rect_width, int reverse_text )
{
  int pLineWidth; // ebx@1
  int text_width; // esi@3
  unsigned __int8 v12; // cl@7
  signed int v13; // esi@19
  signed int v14; // ebx@19
  unsigned __int8 v15; // cl@21
//  int v16; // eax@22
//  int v17; // ecx@22
//  int v18; // ecx@23
//  int v19; // ecx@24
  unsigned int v20; // ecx@26
  unsigned char* v21; // eax@28
//  int v22; // ebx@34
  int v23; // eax@34
  int v24; // ebx@36
  char Str[6]; // [sp+Ch] [bp-20h]@34
//  char v26; // [sp+Fh] [bp-1Dh]@34
//  char v27; // [sp+11h] [bp-1Bh]@35
  int v28; // [sp+20h] [bp-Ch]@17
  GUIWindow *pWindow; // [sp+24h] [bp-8h]@1
  size_t pNumLen; // [sp+28h] [bp-4h]@1
  size_t Str1a; // [sp+40h] [bp+14h]@5
//  size_t Str1b; // [sp+40h] [bp+14h]@19
//  const char *Sourcea; // [sp+44h] [bp+18h]@20
//  int v34; // [sp+48h] [bp+1Ch]@26
  int i;


  pWindow = this;
  pNumLen = strlen(text);
  pLineWidth = pFont->GetLineWidth(text);
  if ( pLineWidth < rect_width )
  {
    pWindow->DrawText(pFont, uX, uY, uColor, text, 0, 0, 0);
    return pLineWidth;
  }
  strcpy(pTmpBuf2.data(), text);
  text_width = 0;
  if ( reverse_text )
    _strrev(pTmpBuf2.data());
  Str1a = 0;
  for ( i = 0; i < pNumLen; ++i )
    {
      if ( text_width >= rect_width )
        break;
      v12 = pTmpBuf2[i];
      if ( pFont->IsCharValid(v12) )
      {
      switch (v12)
          {
      case '\t':// Horizontal tab 09
      case '\n': //Line Feed 0A 10
      case '\r': //Form Feed, page eject  0C 12
          break;
      case '\f': //Carriage Return 0D 13
          i += 5;	  
          break;
      default:
          if ( i > 0 )
            text_width += pFont->pMetrics[v12].uLeftSpacing;
          text_width += pFont->pMetrics[v12].uWidth;
          if ( i < pNumLen )
              text_width += pFont->pMetrics[v12].uRightSpacing;
          }
      }
    }
  pTmpBuf2[i - 1] = 0;


  pNumLen = strlen(pTmpBuf2.data());
  v28 = pFont->GetLineWidth(pTmpBuf2.data());
  if ( reverse_text )
    _strrev(pTmpBuf2.data());

  v13 = uX + pWindow->uFrameX;
  v14 = uY + pWindow->uFrameY;
  for (i=0; i<pNumLen; ++i)
  {
      v15 = pTmpBuf2[i];
      if ( pFont->IsCharValid(v15) )
      {
      switch (v12)
          {
      case '\t':// Horizontal tab 09
          {
          strncpy(Str,  &pTmpBuf2[i+1], 3);
          Str[3] = 0;
       //   atoi(Str);
          i += 3;
          break;
          }
      case '\n': //Line Feed 0A 10
          {
          v24 = pFont->uFontHeight;
          v13 = uX;
          uY = uY + pFont->uFontHeight - 3;
          v14 = uY+pFont->uFontHeight - 3;
          break;
          }
      case '\r': //Form Feed, page eject  0C 12
          {
          strncpy(Str, &pTmpBuf2[i+1], 5);
          Str[5] = 0;
          i += 5;
          uColor = atoi(Str);
          break;
          }
      case '\f': //Carriage Return 0D 13
          {
          strncpy(Str, &pTmpBuf2[i+1], 3);
          Str[3] = 0;
          i += 3;
          v23 = pFont->GetLineWidth(&pTmpBuf2[i]);
          v13 = pWindow->uFrameZ - v23 - atoi(Str);
          v14 = uY;
          break;
          }
      default:
          v20 = pFont->pMetrics[v15].uWidth;
          if ( i > 0 )
              v13 += pFont->pMetrics[v15].uLeftSpacing;
          v21 = &pFont->pFontData[pFont->font_pixels_offset[v15]];
          if ( uColor )
              pRenderer->DrawText(v13, v14,  v21, v20, pFont->uFontHeight, pFont->pFontPalettes[0], uColor, 0);
          else
              pRenderer->DrawTextPalette(v13, v14, v21, v20, pFont->uFontHeight, pFont->pFontPalettes[0], 0);
          v13 += v20;
          if ( i < (signed int)pNumLen )
              v13 += pFont->pMetrics[v15].uRightSpacing;
          }
      }
  }
  return v28;
}

//----- (0041D12F) --------------------------------------------------------
GUIButton *GUIWindow::CreateButton(unsigned int uX, unsigned int uY, unsigned int uWidth, unsigned int uHeight, 
	int a6, int a7, UIMessageType msg, unsigned int msg_param, unsigned __int8 uHotkey, const char *pName, Texture *pTextures, ...)
{
  GUIButton *pButton; // esi@1
//  unsigned int v13; // eax@1
//  unsigned int v14; // ebx@4
//  unsigned int v15; // eax@4
  unsigned int TextureNum=0; // ebx@4
//  unsigned int v17; // eax@4
//  Texture *v18; // eax@4
//  Texture **v19; // ecx@5
//  Texture **v20; // edx@5
//  GUIButton *v21; // eax@7
  va_list texturs_ptr;

  pButton = (GUIButton *)malloc(0xBC);
  pButton->pParent = this;
  pButton->uWidth = uWidth;
  pButton->uHeight = uHeight;
  
  if ( a6 == 2 && !uHeight )
    pButton->uHeight = uWidth;

  pButton->uButtonType = a6;
  pButton->uX = uX + this->uFrameX;
  pButton->uY = uY + this->uFrameY;
  pButton->uZ = pButton->uX + uWidth - 1;
  pButton->uW = pButton->uY + uHeight - 1;
  pButton->field_2C_is_pushed = 0;
  pButton->field_1C = a7;
  pButton->msg = msg;
  pButton->msg_param = msg_param;
  pButton->uHotkey = uHotkey;
  //strlen(pName);
  strcpy(pButton->pButtonName, pName);
  va_start(texturs_ptr, pName);
  while  (NULL!=(pTextures=va_arg(texturs_ptr, Texture *)))
  {
	pButton->pTextures[TextureNum]=pTextures;
	++TextureNum;	
  }
  va_end(texturs_ptr);
  pButton->uNumTextures = TextureNum;
  if ( this->pControlsTail )
    this->pControlsTail->pNext = pButton;
  else
    this->pControlsHead = pButton;
  pButton->pPrev = this->pControlsTail;
  this->pControlsTail = pButton;
  pButton->pNext = 0;
  ++this->uNumControls;
  return pButton;
}

//----- (00459C2B) --------------------------------------------------------
void GUIWindow::DrawFlashingInputCursor( signed int uX, int uY, struct GUIFont *a2 )
{
  if ( GetTickCount() % 1000 > 500 )
    DrawText(a2, uX, uY, 0, "_", 0, 0, 0);
}

//----- (0041C432) --------------------------------------------------------
GUIWindow * GUIWindow::Create( unsigned int uX, unsigned int uY, unsigned int uWidth, unsigned int uHeight, enum WindowType eWindowType, int pButton, const char* hint )
{
  unsigned int uNextFreeWindowID; // ebp@1
  //int *v8; // eax@1
  //GUIWindow *pWindow; // esi@4
  //int v10; // eax@4
  unsigned int v11; // ebx@15
  NPCData *speakingNPC; // ebp@15
  int v14; // eax@20
  int v16; // eax@25
  int v18; // eax@30
  int v20; // eax@35
  int v22; // eax@40
  int v24; // eax@45
//  int v25; // eax@65
  unsigned int v26; // ebx@65
  char *v27; // eax@71
  const char *v29; // [sp-8h] [bp-18h]@68
  char *v30; // [sp-4h] [bp-14h]@68
//  int uWidtha; // [sp+14h] [bp+4h]@66
  int num_menu_buttons; // [sp+20h] [bp+10h]@15

  for (uNextFreeWindowID = 0; uNextFreeWindowID < 20; ++uNextFreeWindowID)
  {
    if (pWindowList[uNextFreeWindowID].eWindowType == WINDOW_null)
      break;
  }

  GUIWindow* pWindow = &pWindowList[uNextFreeWindowID];
  pWindow->uFrameWidth = uWidth;
  pWindow->uFrameHeight = uHeight;

  pWindow->uFrameX = uX;
  pWindow->uFrameY = uY;
  pWindow->uFrameZ = uX + uWidth - 1;
  pWindow->uFrameW = uY + uHeight - 1;

  pWindow->ptr_1C = (void *)pButton;
  pWindow->Hint = hint;
  
  pWindow->eWindowType = eWindowType;
  pWindow->receives_keyboard_input = false;
  ++uNumVisibleWindows;
  pWindow->numVisibleWindows = uNumVisibleWindows;
  pVisibleWindowsIdxs[uNumVisibleWindows] = uNextFreeWindowID + 1;
  if ( (signed int)eWindowType <= 20 )
  {
    if (eWindowType != WINDOW_Chest)
    {
      switch (eWindowType)
      {
        case WINDOW_Book: {
          pWindow->InitializeBookView();
          break;
          }
        case WINDOW_Dialogue: {
          pMainScreenNum = pCurrentScreen;
          pCurrentScreen = SCREEN_NPC_DIALOGUE;
          pBtn_ExitCancel = pWindow->CreateButton(0x1D7u, 0x1BDu, 0xA9u, 0x23u, 1, 0, UIMSG_Escape, 0, 0, pGlobalTXT_LocalizationStrings[79], //"Exit"
                         pIcons_LOD->GetTexture(uExitCancelTextureId), 0);
          if ( pWindow->par1C != 1 )
          {
            num_menu_buttons = 0;
            v11 = LOBYTE(pFontArrus->uFontHeight) - 3;
            speakingNPC = GetNPCData(sDialogue_SpeakingActorNPC_ID);
            if ( GetGreetType(sDialogue_SpeakingActorNPC_ID) == 1 )//QuestsNPC_greet
            {
              if ( speakingNPC->joins )
              {
                pWindow->CreateButton(480, 130, 140, v11, 1, 0, UIMSG_SelectNPCDialogueOption, 0xDu, 0, "", 0);
                num_menu_buttons = 1;
              }
              if ( speakingNPC->evt_A )
              {
                if ( num_menu_buttons < 4 )
                {
                  v14 = NPC_EventProcessor(speakingNPC->evt_A);
                  if ( v14 == 1 || v14 == 2 )
                    pWindow->CreateButton(0x1E0u, num_menu_buttons++ * v11 + 130, 0x8Cu, v11, 1, 0, UIMSG_SelectNPCDialogueOption, 0x13u, 0, "", 0);
                }
              }
              if ( speakingNPC->evt_B )
              {
                if ( num_menu_buttons < 4 )
                {
                  v16 = NPC_EventProcessor(speakingNPC->evt_B);
                  if ( v16 == 1 || v16 == 2 )
                    pWindow->CreateButton(0x1E0u, num_menu_buttons++ * v11 + 130, 0x8Cu, v11, 1, 0, UIMSG_SelectNPCDialogueOption, 0x14u, 0, "", 0);
                }
              }
              if ( speakingNPC->evt_C )
              {
                if ( num_menu_buttons < 4 )
                {
                  v18 = NPC_EventProcessor(speakingNPC->evt_C);
                  if ( v18 == 1 || v18 == 2 )
                    pWindow->CreateButton( 0x1E0u, num_menu_buttons++ * v11 + 130, 0x8Cu, v11, 1, 0, UIMSG_SelectNPCDialogueOption, 0x15u, 0, "", 0);
                }
              }
              if ( speakingNPC->evt_D )
              {
                if ( num_menu_buttons < 4 )
                {
                  v20 = NPC_EventProcessor(speakingNPC->evt_D);
                  if ( v20 == 1 || v20 == 2 )
                    pWindow->CreateButton(0x1E0u, num_menu_buttons++ * v11 + 130, 0x8Cu, v11, 1, 0, UIMSG_SelectNPCDialogueOption, 0x16u, 0, "", 0);
                }
              }
              if ( speakingNPC->evt_E )
              {
                if ( num_menu_buttons < 4 )
                {
                  v22 = NPC_EventProcessor(speakingNPC->evt_E);
                  if ( v22 == 1 || v22 == 2 )
                    pWindow->CreateButton(0x1E0u, num_menu_buttons++ * v11 + 130, 0x8Cu, v11, 1, 0, UIMSG_SelectNPCDialogueOption, 0x17u, 0, "", 0);
                }
              }
              if (speakingNPC->evt_F )
              {
                if ( num_menu_buttons < 4 )
                {
                  v24 = NPC_EventProcessor(speakingNPC->evt_F);
                  if ( v24 == 1 || v24 == 2 )
                    pWindow->CreateButton(0x1E0u, num_menu_buttons++ * v11 + 130, 0x8Cu, v11, 1, 0, UIMSG_SelectNPCDialogueOption, 0x18u, 0, "", 0);
                }
              }
            }
            else
            {
              if ( speakingNPC->joins )
              {
                pWindow->CreateButton(0x1E0u, 0x82u, 0x8Cu, v11, 1, 0, UIMSG_SelectNPCDialogueOption, 0x4Du, 0, pGlobalTXT_LocalizationStrings[407], 0);//Подробнее
                if (speakingNPC->Hired())
                {
                  sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[408], speakingNPC->pName); //Отпустить
                  pWindow->CreateButton(0x1E0u, v11 + 130, 0x8Cu, v11, 1, 0, UIMSG_SelectNPCDialogueOption, 0x4Cu, 0, pTmpBuf.data(), 0);
                }
                else
                  pWindow->CreateButton(0x1E0u, v11 + 130, 0x8Cu, v11, 1, 0, UIMSG_SelectNPCDialogueOption, 0x4Cu, 0, pGlobalTXT_LocalizationStrings[406], 0);//Нанять
                num_menu_buttons = 2;
              }
            }
            pWindow->_41D08F_set_keyboard_control_group(num_menu_buttons, 1, 0, 1);
          }
          break;
            }
        case WINDOW_ChangeLocation:
        {
          pMainScreenNum = pCurrentScreen;
          pCurrentScreen = SCREEN_CHANGE_LOCATION;
          pBtn_ExitCancel = pWindow->CreateButton(                  566,                   445,  75,  33, 1, 0, UIMSG_CHANGE_LOCATION_ClickCencelBtn, 0, 'N', pGlobalTXT_LocalizationStrings[156], pIcons_LOD->GetTexture(uTextureID_BUTTDESC2), 0);//Остаться в этой области
          pBtn_YES        = pWindow->CreateButton(                  486,                   445,  75,  33, 1, 0, UIMSG_OnTravelByFoot, 0, 'Y', pWindow->Hint, pIcons_LOD->GetTexture(uTextureID_BUTTYES2), 0);
                            pWindow->CreateButton(pNPCPortraits_x[0][0], pNPCPortraits_y[0][0],  63,  73, 1, 0, UIMSG_OnTravelByFoot, 1, ' ', pWindow->Hint, 0, 0, 0);
                            pWindow->CreateButton(                    8,                     8, 460, 344, 1, 0, UIMSG_OnTravelByFoot, 1,   0, pWindow->Hint, 0);
          break;
        }
        case WINDOW_SpellBook: {// окно книги заклов
          InitializeBookTextures();
          pWindow->OpenSpellBook();
          break;
            }
        case WINDOW_GreetingNPC: {// окно приветствия НПС
          pMainScreenNum = pCurrentScreen;
          pKeyActionMap->EnterText(0, 15, pWindow);
          pCurrentScreen = SCREEN_BRANCHLESS_NPC_DIALOG;
          break;
            }

      }
      return pWindow;
    }
//LABEL_62:
    pWindow->CreateButton(61, 424, 31, 0, 2, 94, UIMSG_SelectCharacter, 1, '1', "", 0);
    pWindow->CreateButton(177, 424, 31, 0, 2, 94, UIMSG_SelectCharacter, 2, '2', "", 0);
    pWindow->CreateButton(292, 424, 31, 0, 2, 94, UIMSG_SelectCharacter, 3, '3', "", 0);
    pWindow->CreateButton(407, 424, 31, 0, 2, 94, UIMSG_SelectCharacter, 4, '4', "", 0);
    pWindow->CreateButton(0, 0, 0, 0, 1, 0, UIMSG_CycleCharacters, 0, 9, "", 0);
    return pWindow;
  }
  if (eWindowType == WINDOW_HouseInterior)
  {
    pCurrentScreen = SCREEN_HOUSE;
    pBtn_ExitCancel = pWindow->CreateButton(471, 445, 169, 35, 1, 0, UIMSG_Escape, 0, 0, pGlobalTXT_LocalizationStrings[80],//Выйти из здания
                   pIcons_LOD->GetTexture(uExitCancelTextureId), 0);
    for ( v26 = 0; v26 < uNumDialogueNPCPortraits; ++v26 )
    {
      if ( v26 + 1 == uNumDialogueNPCPortraits && uHouse_ExitPic )
      {
        v30 = pMapStats->pInfos[uHouse_ExitPic].pName;
        v29 = (char*)pGlobalTXT_LocalizationStrings[LOCSTR_ENTER_S];
      }
      else
      {
        if ( v26 || !dword_591080 )
          v27 = HouseNPCData[v26 +1 - ((dword_591080 != 0)? 1:0)]->pName;
        else
          v27 = (char*)p2DEvents[pButton - 1].pProprieterName;
        v30 = v27;
        v29 = (char*)pGlobalTXT_LocalizationStrings[435];
      }
      sprintfex(byte_591180[v26].data(), v29, v30);
      HouseNPCPortraitsButtonsList[v26] = pWindow->CreateButton(pNPCPortraits_x[uNumDialogueNPCPortraits - 1][v26],
                                                                pNPCPortraits_y[uNumDialogueNPCPortraits - 1][v26],
                                           63, 73, 1, 0, UIMSG_ClickHouseNPCPortrait, v26, 0, byte_591180[v26].data(), 0, 0, 0);
    }
    if ( uNumDialogueNPCPortraits == 1 )
    {
      window_SpeakInHouse = &pWindowList[uNextFreeWindowID];
      _4B4224_UpdateNPCTopics(0);
    }
  }
  else
  {
    if (eWindowType == WINDOW_Transition)
    {
      pMainScreenNum = pCurrentScreen;
      pCurrentScreen = SCREEN_INPUT_BLV;
      pBtn_ExitCancel = pWindow->CreateButton(0x236u, 0x1BDu, 0x4Bu, 0x21u, 1, 0, UIMSG_TransitionWindowCloseBtn, 0, 'N', pGlobalTXT_LocalizationStrings[34], pIcons_LOD->GetTexture(uTextureID_BUTTDESC2), 0);//Отмена
      pBtn_YES        = pWindow->CreateButton(0x1E6u, 0x1BDu, 0x4Bu, 0x21u, 1, 0, UIMSG_TransitionUI_Confirm, 0, 'Y', pWindow->Hint, pIcons_LOD->GetTexture(uTextureID_BUTTYES2), 0);
                        pWindow->CreateButton(pNPCPortraits_x[0][0], pNPCPortraits_y[0][0], 0x3Fu, 0x49u, 1, 0, UIMSG_TransitionUI_Confirm, 1, 0x20u, pWindow->Hint, 0);
                        pWindow->CreateButton(8, 8, 0x1CCu, 0x158u, 1, 0, UIMSG_TransitionUI_Confirm, 1u, 0, pWindow->Hint, 0);
      return pWindow;
    }
    if (eWindowType == WINDOW_CastSpell)
    {
      pEventTimer->Pause();
      pAudioPlayer->StopChannels(-1, -1);
      pMouse->SetCursorBitmap("MICON2");
      ShowStatusBarString(pGlobalTXT_LocalizationStrings[39], 2u);//Выберите цель
      return pWindow;
    }
    if (eWindowType == WINDOW_Scroll)
    {
      pWindow->CreateButton(61, 424, 31, 0, 2, 94, UIMSG_SelectCharacter, 1, '1', "", 0);
      pWindow->CreateButton(177, 424, 31, 0, 2, 94, UIMSG_SelectCharacter, 2, '2', "", 0);
      pWindow->CreateButton(292, 424, 31, 0, 2, 94, UIMSG_SelectCharacter, 3, '3', "", 0);
      pWindow->CreateButton(407, 424, 31, 0, 2, 94, UIMSG_SelectCharacter, 4, '4', "", 0);
      pWindow->CreateButton(0, 0, 0, 0, 1, 0, UIMSG_CycleCharacters, 0, '\t', "", 0);
      return pWindow;
    }
    if (eWindowType == WINDOW_CastSpell_InInventory)
    {
      pMouse->SetCursorBitmap("MICON2");
      pBtn_ExitCancel = pWindow->CreateButton(392, 318, 75, 33, 1, 0, UIMSG_Escape, 0, 0, pGlobalTXT_LocalizationStrings[34],//Отмена
                     pIcons_LOD->GetTexture(uTextureID_BUTTDESC2), 0);
      ShowStatusBarString(pGlobalTXT_LocalizationStrings[39], 2);//Выбрать цель
      ++pIcons_LOD->uTexturePacksCount;
      pWindowList_at_506F50_minus1_indexing_buttons____and_an_int_[0] = 103;
      pCurrentScreen = SCREEN_CASTING;
      if ( !pIcons_LOD->uNumPrevLoadedFiles )
        pIcons_LOD->uNumPrevLoadedFiles = pIcons_LOD->uNumLoadedFiles;
    }
  }
  return pWindow;
}
//----- (004B3EF0) --------------------------------------------------------
void DrawJoinGuildWindow( int pEventCode )
{
  uDialogueType = 81;//enum JoinGuildDialog
  current_npc_text = (char *)pNPCTopics[pEventCode + 99].pText;
  ContractSelectText(pEventCode);
  pDialogueWindow->Release();
  pDialogueWindow = GUIWindow::Create(0, 0, window->GetWidth(), 350, WINDOW_MainMenu, pEventCode, 0);
  pBtn_ExitCancel = pDialogueWindow->CreateButton(471, 445, 169, 35, 1, 0, UIMSG_Escape,                    0, 0, pGlobalTXT_LocalizationStrings[34], pIcons_LOD->GetTexture(uExitCancelTextureId), 0); // Cancel
                    pDialogueWindow->CreateButton(  0,   0,   0,  0, 1, 0, UIMSG_BuyInShop_Identify_Repair, 0, 0, "", 0);
                    pDialogueWindow->CreateButton(480, 160, 140, 30, 1, 0, UIMSG_ClickNPCTopic,             0x52u, 0, pGlobalTXT_LocalizationStrings[122], 0);
  pDialogueWindow->_41D08F_set_keyboard_control_group(1, 1, 0, 2);
  dialog_menu_id = HOUSE_DIALOGUE_OTHER;
}
//----- (0044603D) --------------------------------------------------------
void  DialogueEnding()
{
  sDialogue_SpeakingActorNPC_ID = 0;
  pDialogueWindow->Release();
  pDialogueWindow = 0;
  pMiscTimer->Resume();
  pEventTimer->Resume();
}
//----- (004156F0) --------------------------------------------------------
void GUI_UpdateWindows() 
{
  GUIWindow *pWindow; // esi@4
  //unsigned int pWindowType; // eax@4
  const char *pHint; // edx@66
//  GUIButton *pButtonPtr_1C; // ebp@79
//  char *pHint1; // edx@80
  int v26; // eax@98
  unsigned int v27; // ebp@106
  GUIWindow *pGUIWindow2; // ecx@109
//  GUIFont *pGUIFont; // ST1C_4@115
  int v31; // eax@115
  GUIButton *pButton; // ebp@118
  int v39; // eax@129
  unsigned int pNumMessages; // eax@142
  GUIButton *pGUIButton; // ebp@146
  //unsigned int pX; // [sp-1Ch] [bp-124h]@17
  //unsigned int pY; // [sp-18h] [bp-120h]@17
  //Texture *pTexture; // [sp-14h] [bp-11Ch]@17
  //Texture *pTexture2; // [sp-14h] [bp-11Ch]@86
  int i; // [sp+0h] [bp-108h]@3
//  ItemGen pItemGen; // [sp+4h] [bp-104h]@98
  GUIButton GUIButton2; // [sp+28h] [bp-E0h]@133
  ItemGen ItemGen2; // [sp+E4h] [bp-24h]@129

  if (GetCurrentMenuID() != MENU_CREATEPARTY)
    Mouse::UI_OnKeyDown(VK_NEXT);

  for ( i = 1; i <= uNumVisibleWindows; ++i )
  {
    pWindow = &pWindowList[pVisibleWindowsIdxs[i] - 1];
    switch (pWindow->eWindowType)
    {
      case WINDOW_OptionsButtons:
      {
        pRenderer->DrawTextureIndexed(pViewport->uViewportTL_Y,
                                      pViewport->uViewportTL_X, pIcons_LOD->GetTexture(uTextureID_Options));
        viewparams->bRedrawGameUI = 1;
        continue;
      }
      case WINDOW_CharacterRecord:
      {
        CharacterUI_CharacterScreen_Draw(pPlayers[uActiveCharacter]);
        continue;
      }
      case WINDOW_Options:
      {
        GameMenuUI_Options_Draw();
        continue;
      }
      case WINDOW_Book:
      {
        BookUI_Draw((WindowType)(int)pWindow->ptr_1C);
        continue;
      }
      case WINDOW_Dialogue:
      {
        GameUI_DrawDialogue();
        continue;
      }
      case WINDOW_QuickReference:
      {
        GameUI_QuickRef_Draw();
        continue;
      }
      case WINDOW_Rest:
      {
        RestUI_Draw();
        continue;
      }
      case WINDOW_ChangeLocation:
      {
        TravelUI_Draw();
        continue;
      }
      case WINDOW_SpellBook:
      {
        DrawSpellBookContent(pPlayers[uActiveCharacter]);
        continue;
      }
      case WINDOW_GreetingNPC:
      {
        GameUI_DrawBranchlessDialogue();
        continue;
      }
      case WINDOW_Chest:
      {
        if ( pCurrentScreen == SCREEN_CHEST )
        {
          Chest::DrawChestUI(pWindow->par1C);
        }
        else if ( pCurrentScreen == SCREEN_CHEST_INVENTORY )
        {
          pRenderer->ClearZBuffer(0, 479);
          draw_leather();
          CharacterUI_InventoryTab_Draw(pPlayers[uActiveCharacter], true);
          pRenderer->DrawTextureIndexed(pBtn_ExitCancel->uX, pBtn_ExitCancel->uY, pIcons_LOD->GetTexture(uExitCancelTextureId));
        }
        continue;
      }
      case WINDOW_SaveLoadButtons:
      {
        SaveUI_Draw();
        continue;
      }
      case WINDOW_MainMenu_Load:
      {
        LoadUI_Draw();
        continue;
      }
      case WINDOW_HouseInterior:
      {
        pWindowList[pVisibleWindowsIdxs[i] - 1].HouseDialogManager();
        if ( !window_SpeakInHouse )
          continue;
        if ( window_SpeakInHouse->par1C >= 53 )
          continue;
        if ( pParty->PartyTimes._shop_ban_times[window_SpeakInHouse->par1C] <=pParty->uTimePlayed )
        {
          if ( window_SpeakInHouse->par1C < 53 )
            pParty->PartyTimes._shop_ban_times[window_SpeakInHouse->par1C] = 0;
          continue;
        }
        pNumMessages = pMessageQueue_50CBD0->uNumMessages;
        pMessageQueue_50CBD0->AddGUIMessage(UIMSG_Escape, 0, 0);
        continue;
      }
      case WINDOW_Transition:
      {
        TransitionUI_Draw();
        continue;
      }
      case WINDOW_Scroll:
      {
        CreateScrollWindow();
        continue;
      }
      case WINDOW_CastSpell_InInventory:
      {
        pRenderer->ClearZBuffer(0, 479);
        draw_leather();
        CharacterUI_InventoryTab_Draw(pPlayers[uActiveCharacter], true);
        CharacterUI_DrawPaperdoll(pPlayers[uActiveCharacter]);
        pRenderer->DrawTextureTransparent(pBtn_ExitCancel->uX, pBtn_ExitCancel->uY, pIcons_LOD->GetTexture(uTextureID_x_x_u));
        continue;
      }
      case WINDOW_ModalWindow:
      {
        ModalWindow_ShowHint();
        continue;
      }
      case WINDOW_50:
      {
        v27 = Color16(255, 255, 255);
        if ( ptr_507BD0->receives_keyboard_input_2 == WINDOW_INPUT_IN_PROGRESS)
        {
          ptr_507BD0->DrawMessageBox(0);
          ptr_507BD0->DrawText(pFontCreate, 30, 40, v27, pKeyActionMap->pPressedKeysBuffer, 0, 0, 0);
          v31 = pFontCreate->GetLineWidth(pKeyActionMap->pPressedKeysBuffer);
          ptr_507BD0->DrawFlashingInputCursor(v31 + 30, 40, pFontCreate);
          continue;
        }
        if ( ptr_507BD0->receives_keyboard_input_2 == WINDOW_INPUT_CONFIRMED)
        {
          pWindow->receives_keyboard_input_2 = WINDOW_INPUT_NONE;
          pMessageQueue_50CBD0->AddGUIMessage((UIMessageType)(int)ptr_507BD0->ptr_1C, 0, 0);
          pEventTimer->Resume();
          ptr_507BD0->Release();
          pCurrentScreen = SCREEN_GAME;
          viewparams->bRedrawGameUI = true;
          continue;
        }
        if ( ptr_507BD0->receives_keyboard_input_2 == WINDOW_INPUT_CANCELLED)
        {
          pWindow->receives_keyboard_input_2 = WINDOW_INPUT_NONE;
          pEventTimer->Resume();
          ptr_507BD0->Release();
          continue;
        }
      }
      case WINDOW_59:
      {
        pWindow->DrawMessageBox(0);
        pWindow->DrawText(pFontLucida, 10, 20, 0, "Making item number", 0, 0, 0);
        pWindow->DrawText(pFontLucida, 10, 40, 0, pKeyActionMap->pPressedKeysBuffer, 0, 0, 0);
        if ( !pKeyActionMap->field_204 )
        {
          ItemGen2.Reset();
          pWindow->Release();
          pEventTimer->Resume();
          pCurrentScreen = 0;
          viewparams->bRedrawGameUI = true;
          v26 = atoi(pKeyActionMap->pPressedKeysBuffer);
          if ( v26 > 0 )
          {
            if ( v26 < 800 )
            {
              ItemGen2.uAttributes |= 1u;
              ItemGen2.uItemID = v26;
              if ( pItemsTable->pItems[v26].uEquipType == 12 )
              {
                ItemGen2.uNumCharges = rand() % 6 + ItemGen2.GetDamageMod() + 1;
                ItemGen2.uMaxCharges = LOBYTE(ItemGen2.uNumCharges);
              }
              else
              {
                if ( v26 >= 221 && v26 < 271 )
                  ItemGen2.uEnchantmentType = rand() % 10 + 1;
              }
              pItemsTable->SetSpecialBonus(&ItemGen2);
              pParty->SetHoldingItem(&ItemGen2);
            }
          }
        }
        continue;
      }
      case WINDOW_PressedButton2:
      {
        if ( pWindow->Hint != (char *)1 )
          pAudioPlayer->PlaySound((SoundID)75, 0, 0, -1, 0, 0, 0, 0);
        pButton = (GUIButton *)pWindow->ptr_1C;
        if ( pButton->uX >= 0 && pButton->uX <= window->GetWidth() )
        {
          if ( pButton->uY >= 0 && pButton->uY <= window->GetHeight() )
          {
            pRenderer->DrawTextureIndexed(pWindow->uFrameX, pWindow->uFrameY, pButton->pTextures[0]);
            viewparams->bRedrawGameUI = 1;
            if ( pWindow->Hint )
            {
              if ( pWindow->Hint != (char *)1 )
                pButton->DrawLabel(pWindow->Hint, pFontCreate, 0, 0);
            }
            pWindow->Release();
            continue;
          }
        }
        viewparams->bRedrawGameUI = 1;
        if ( pWindow->Hint )
        {
          if ( pWindow->Hint != (char *)1 )
            pButton->DrawLabel(pWindow->Hint, pFontCreate, 0, 0);
        }
        pWindow->Release();
        continue;
      }
      case WINDOW_CharactersPressedButton:
      {
        if ( pWindow->Hint != (char *)1 )
          pAudioPlayer->PlaySound((SoundID)75, 0, 0, -1, 0, 0, 0, 0);
        pButton = (GUIButton *)pWindow->ptr_1C;
        pRenderer->DrawTextureIndexed(pWindow->uFrameX, pWindow->uFrameY, pButton->pTextures[1]);
        viewparams->bRedrawGameUI = 1;
        if ( pWindow->Hint )
        {
          if ( pWindow->Hint != (char *)1 )
            pButton->DrawLabel(pWindow->Hint, pFontCreate, 0, 0);
        }
        pWindow->Release();
        continue;
      }
      case WINDOW_PressedButton:
      {
        if ( pWindow->Hint != (char *)1 )
          pAudioPlayer->PlaySound((SoundID)75, 0, 0, -1, 0, 0, 0, 0);
        pButton = (GUIButton *)pWindow->ptr_1C;
        pRenderer->DrawTextureTransparent(pWindow->uFrameX, pWindow->uFrameY, pButton->pTextures[0]);
        viewparams->bRedrawGameUI = 1;
        if ( pWindow->Hint )
        {
          if ( pWindow->Hint != (char *)1 )
            pButton->DrawLabel(pWindow->Hint, pFontCreate, 0, 0);
        }
        pWindow->Release();
        continue;
      }
      case WINDOW_5D:
      {
        if ( pWindow->Hint != (char *)1 )
          pAudioPlayer->PlaySound((SoundID)75, 0, 0, -1, 0, 0, 0, 0);
        pButton = (GUIButton *)pWindow->ptr_1C;
        pRenderer->DrawTextureTransparent(pWindow->uFrameX, pWindow->uFrameY, pButton->pTextures[1]);
        viewparams->bRedrawGameUI = 1;
        pWindow->Release();
        continue;
      }
      case WINDOW_SaveLoadBtn:
      {
        if (pWindow->Hint != (char *)1)
          pAudioPlayer->PlaySound(SOUND_Button2, 0, 0, -1, 0, 0, 0, 0);
        pButton = (GUIButton *)pWindow->ptr_1C;
        pRenderer->DrawTextureIndexed(pWindow->uFrameX, pWindow->uFrameY, pButton->pTextures[0]);
        pHint = pWindow->Hint;
        viewparams->bRedrawGameUI = 1;
        if ( pHint && pHint != (char *)1 )
          pButton->DrawLabel(pHint, pFontCreate, 0, 0);
        pWindow->Release();
        if (pCurrentScreen == SCREEN_SAVEGAME)
          pMessageQueue_50CBD0->AddGUIMessage(UIMSG_SaveGame, 0, 0);
        else
          pMessageQueue_50CBD0->AddGUIMessage(UIMSG_LoadGame, 0, 0);
        continue;
      }
      case WINDOW_LoadGame_CancelBtn:
      {
        if ( pWindow->Hint != (char *)1 )
          pAudioPlayer->PlaySound((SoundID)75, 0, 0, -1, 0, 0, 0, 0);
        pButton = (GUIButton *)pWindow->ptr_1C;
        pRenderer->DrawTextureTransparent(pWindow->uFrameX, pWindow->uFrameY, pButton->pTextures[0]);
        viewparams->bRedrawGameUI = 1;
        if ( pWindow->Hint && pWindow->Hint != (char *)1 )
          pButton->DrawLabel(pWindow->Hint, pFontCreate, 0, 0);
        pWindow->Release();
        pMessageQueue_50CBD0->AddGUIMessage(UIMSG_Escape, 0, 0);
        continue;
      }
      case WINDOW_CloseRestWindowBtn:
      {
        if ( pWindow->Hint != (char *)1 )
          pAudioPlayer->PlaySound(SOUND_Button2, 0, 0, -1, 0, 0, 0, 0);
        pGUIButton = (GUIButton *)pWindow->ptr_1C;
        pRenderer->DrawTextureIndexed(pWindow->uFrameX, pWindow->uFrameY, pGUIButton->pTextures[0]);
        pHint = pWindow->Hint;
        viewparams->bRedrawGameUI = 1;
        if ( pHint && pHint != (char *)1 )
          pGUIButton->DrawLabel(pHint, pFontCreate, 0, 0);
        pWindow->Release();
        pMessageQueue_50CBD0->AddGUIMessage(UIMSG_Escape, 0, 0);
        continue;
      }
      case WINDOW_ExitCharacterWindow:
      {
        if ( pWindow->Hint != (char *)1 )
          pAudioPlayer->PlaySound(SOUND_Button2, 0, 0, -1, 0, 0, 0, 0);
        pButton = (GUIButton *)pWindow->ptr_1C;
        pRenderer->DrawTextureIndexed(pWindow->uFrameX, pWindow->uFrameY, pButton->pTextures[1]);
        pHint = pWindow->Hint;
        viewparams->bRedrawGameUI = 1;
        if ( pHint && pHint != (char *)1 )
          pButton->DrawLabel(pHint, pFontCreate, 0, 0);
        pWindow->Release();
        pNumMessages = pMessageQueue_50CBD0->uNumMessages;
        pMessageQueue_50CBD0->AddGUIMessage(UIMSG_Escape, 0, 0);
        continue;
      }
      case WINDOW_RestWindow:
      {
        memset(&GUIButton2, 0, 0xBCu);
        GUIButton2.uZ = 197;
        GUIButton2.uW = 197;
        GUIButton2.uX = 27;
        GUIButton2.uY = 161;
        GUIButton2.uWidth = 171;
        GUIButton2.uHeight = 37;
        GUIButton2.pParent = pButton_RestUI_WaitUntilDawn->pParent;
        pAudioPlayer->PlaySound(SOUND_Button2, 0, 0, -1, 0, 0, 0, 0);
        pRenderer->DrawTextureIndexed(pWindow->uFrameX, pWindow->uFrameY, *((Texture **)pWindow->ptr_1C + 15));
        viewparams->bRedrawGameUI = 1;
        GUIButton2.DrawLabel(pGlobalTXT_LocalizationStrings[183], pFontCreate, 0, 0);//Отдых и лечение 8 часов
        GUIButton2.pParent = 0;
        pGUIWindow2 = pWindow;
        pGUIWindow2->Release();
        continue;
      }
      case WINDOW_BooksWindow:
      {
        pButton = (GUIButton *)pWindow->ptr_1C;
        pRenderer->DrawTextureIndexed(pWindow->uFrameY,
                                      pWindow->uFrameX, pButton->pTextures[0]);
        viewparams->bRedrawGameUI = true;
        continue;
      }
      case WINDOW_CharacterWindow_Inventory:
      {
        pWindow->DrawMessageBox(0);
        pWindow->DrawText(pFontLucida, 10, 20, 0, "Making item number", 0, 0, 0);
        pWindow->DrawText(pFontLucida, 10, 40, 0, pKeyActionMap->pPressedKeysBuffer, 0, 0, 0);
        if ( !pKeyActionMap->field_204 )
        {
          ItemGen2.Reset();
          pWindow->Release();
          pEventTimer->Resume();
          pCurrentScreen = SCREEN_GAME;
          viewparams->bRedrawGameUI = 1;
          v39 = atoi(pKeyActionMap->pPressedKeysBuffer);
          if ( v39 > 0 )
          {
            if ( v39 < 800 )
              SpawnActor(v39);
          }
        }
        continue;
      }
      case WINDOW_KeyMappingOptions:
      {
        GameMenuUI_DrawKeyBindings();
        continue;
      }
      case WINDOW_VideoOptions:
      {
        GameMenuUI_DrawVideoOptions();
        continue;
      }
      default:
        continue;
    }
  }
  if ( GetCurrentMenuID() == -1 )
    GameUI_DrawFoodAndGold();
  if ( sub_4637E0_is_there_popup_onscreen() )
    UI_OnMouseRightClick(0);
}
//void LoadFonts_and_DrawCopyrightWindow()
//{
  //MainMenuUI_LoadFontsAndSomeStuff();
 // DrawCopyrightWindow();
//}
//----- (00415485) --------------------------------------------------------
void DrawMM7CopyrightWindow()
{
  GUIWindow Dst; // [sp+8h] [bp-54h]@1

  memset(&Dst, 0, 0x54u);
  Dst.uFrameWidth = 624;
  Dst.uFrameHeight = 256;
  Dst.uFrameX = 8;
  Dst.uFrameY = 30;                             // c 1999 The 3DO Company.
  Dst.uFrameHeight = pFontSmallnum->CalcTextHeight(pGlobalTXT_LocalizationStrings[157], &Dst, 24, 0)
                   + 2 * LOBYTE(pFontSmallnum->uFontHeight)
                   + 24;
  Dst.uFrameY = 470 - Dst.uFrameHeight;
  Dst.uFrameZ = Dst.uFrameX + Dst.uFrameWidth - 1;
  Dst.uFrameW = 469;
  //Dst.Hint = "abcagfdsgsg ljsrengvlkjesnfkjwnef";
  Dst.DrawMessageBox(0);

  Dst.uFrameWidth -= 24;
  Dst.uFrameX += 12;
  Dst.uFrameY += 12;
  Dst.uFrameHeight -= 12;
  Dst.uFrameZ = Dst.uFrameX + Dst.uFrameWidth - 1;
  Dst.uFrameW = Dst.uFrameY + Dst.uFrameHeight - 1;
  Dst.DrawTitleText(pFontSmallnum, 0, 0xCu, ui_mainmenu_copyright_color, pGlobalTXT_LocalizationStrings[157], 3);
}

int modal_window_prev_screen;

//----- (004141CA) --------------------------------------------------------
void ModalWindow(const char *pStrHint, UIMessageType OnRelease_message)
{
  pEventTimer->Pause();
  modal_window_prev_screen = pCurrentScreen;
  pModalWindow = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_ModalWindow, OnRelease_message, pStrHint);
  pCurrentScreen = SCREEN_MODAL_WINDOW;
}

//----- (0041420D) --------------------------------------------------------
void  ModalWindow_ShowHint()
{
  GUIWindow pWindow; // [sp+4h] [bp-54h]@1

  sprintf(pTmpBuf2.data(), "%s\n \n%s", pModalWindow->Hint, pGlobalTXT_LocalizationStrings[61]);// Press Escape
  pWindow.Hint = pTmpBuf2.data();
  pWindow.uFrameWidth = 400;
  pWindow.uFrameHeight = 100;
  pWindow.uFrameX = 120;
  pWindow.uFrameY = 140;
  pWindow.uFrameZ = 519;
  pWindow.uFrameW = 239;
  pWindow.DrawMessageBox(0);
}

//----- (0041426F) --------------------------------------------------------
void ModalWindow_Release()
{
  pMessageQueue_50CBD0->AddGUIMessage((UIMessageType)pModalWindow->par1C, 0, 0);

  pModalWindow->Release();
  pModalWindow = nullptr;

  pCurrentScreen = modal_window_prev_screen;
  pEventTimer->Resume();
}

//----- (00467FB6) --------------------------------------------------------
void CreateScrollWindow()
{
  unsigned int v0; // eax@1
  char *v1; // ST18_4@3
  GUIWindow a1; // [sp+Ch] [bp-54h]@1

  memcpy(&a1, pGUIWindow_ScrollWindow, sizeof(a1));
  a1.Hint = 0;
  a1.uFrameX = 1;
  a1.uFrameY = 1;
  a1.uFrameWidth = 468;
  v0 = pFontSmallnum->CalcTextHeight(pScrolls[pGUIWindow_ScrollWindow->par1C], &a1, 0, 0)
     + 2 * LOBYTE(pFontCreate->uFontHeight) + 24;
  a1.uFrameHeight = v0;
  if ( (signed int)(v0 + a1.uFrameY) > 479 )
  {
    v0 = 479 - a1.uFrameY;
    a1.uFrameHeight = 479 - a1.uFrameY;
  }
  a1.uFrameZ = a1.uFrameWidth + a1.uFrameX - 1;
  a1.uFrameW = v0 + a1.uFrameY - 1;
  a1.DrawMessageBox(0);
  a1.uFrameX += 12;
  a1.uFrameWidth -= 24;
  a1.uFrameY += 12;
  a1.uFrameHeight -= 12;
  a1.uFrameZ = a1.uFrameWidth + a1.uFrameX - 1;
  a1.uFrameW = a1.uFrameHeight + a1.uFrameY - 1;
  v1 = pItemsTable->pItems[(unsigned int)pGUIWindow_ScrollWindow->ptr_1C + 700].pName;
  sprintf(pTmpBuf.data(), format_4E2D80, Color16(0xFFu, 0xFFu, 0x9Bu), v1);
  a1.DrawTitleText(pFontCreate, 0, 0, 0, pTmpBuf.data(), 3);
  a1.DrawText(pFontSmallnum, 1, LOBYTE(pFontCreate->uFontHeight) - 3, 0,
              pScrolls[(unsigned int)pGUIWindow_ScrollWindow->ptr_1C], 0, 0, 0);
}
//----- (00467F48) --------------------------------------------------------
void CreateMsgScrollWindow( signed int mscroll_id )
{
  if ( !pGUIWindow_ScrollWindow && mscroll_id >= 700 )
  {
    if ( mscroll_id <= 782 )
    {
      uTextureID_720980 = pIcons_LOD->LoadTexture("leather", TEXTURE_16BIT_PALETTE);
      pGUIWindow_ScrollWindow = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_Scroll, mscroll_id - 700, 0);
    }
  }
}
//----- (00467F9F) --------------------------------------------------------
void  free_book_subwindow()
{
  if ( pGUIWindow_ScrollWindow )
  {
    pGUIWindow_ScrollWindow->Release();
    pGUIWindow_ScrollWindow = 0;
  }
}
//----- (004226EF) --------------------------------------------------------
void SetUserInterface(PartyAlignment align, bool bReplace)
{
  extern void set_default_ui_skin();
  set_default_ui_skin();

  if (align == PartyAlignment_Evil)
  {
    if ( bReplace )
    {
      pTexture_RightFrame->Reload("ib-r-C.pcx");
      pTexture_BottomFrame->Reload("ib-b-C.pcx");
      pTexture_TopFrame->Reload("ib-t-C.pcx");
      pTexture_LeftFrame->Reload("ib-l-C.pcx");
      pTexture_StatusBar->Reload("IB-Foot-c.pcx");
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_right_panel], "ib-mb-C", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_Minimap_Loop], "ib-autmask-c", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_Compas], "IB-COMP-C", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[dword_5079D0], "IB-InitG-c", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[dword_5079C8], "IB-InitY-c", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[dword_5079CC], "IB-InitR-c", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_Btn_NPCLeft], "IB-NPCLD-C", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_Btn_NPCRight], "IB-NPCRD-C", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_Btn_ZoomIn], "ib-autout-C", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_Btn_ZoomOut], "ib-autin-C", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_GameUI_CharSelectionFrame], "IB-selec-C", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_Btn_CastSpell], "ib-m1d-c", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_Btn_Rest], "ib-m2d-c", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_Btn_QuickReference], "ib-m3d-c", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_Btn_GameSettings], "ib-m4d-c", 2);

      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_PlayerBuff_Bless], "isg-01-c", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_PlayerBuff_Preservation], "isg-02-c", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_PlayerBuff_Hammerhands], "isg-03-c", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_PlayerBuff_PainReflection], "isg-04-c", 2);

      pUIAnim_WizardEye->uIconID = pIconsFrameTable->FindIcon("wizeyeC");
      pIconsFrameTable->InitializeAnimation(pUIAnim_WizardEye->uIconID);
      pUIAnum_Torchlight->uIconID = pIconsFrameTable->FindIcon("torchC");
      pIconsFrameTable->InitializeAnimation(pUIAnum_Torchlight->uIconID);

      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uExitCancelTextureId], "ib-bcu-c", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_50795C], "evtnpc-c", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_CharacterUI_InventoryBackground], "fr_inven-c", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_Parchment], "parchment", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_5076B4], "cornr_ll-c", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_5076B0], "cornr_lr-c", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_5076AC], "cornr_ul-c", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_5076A8], "cornr_ur-c", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_5076A4], "edge_btm-c", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_5076A0], "edge_lf-c", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_50769C], "edge_rt-c", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_507698], "edge_top-c", 2);
      pIcons_LOD->ReloadTexture(pTexture_591428, "endcap-c", 2);
    }
    else
    {
      pTexture_RightFrame->Load("ib-r-C.pcx", 0);
      pTexture_BottomFrame->Load("ib-b-c.pcx", 0);
      pTexture_TopFrame->Load("ib-t-C.pcx", 0);
      pTexture_LeftFrame->Load("ib-l-C.pcx", 0);
      pTexture_StatusBar->Load("IB-Foot-c.pcx", 0);
      uTextureID_right_panel = pIcons_LOD->LoadTexture("ib-mb-C", TEXTURE_16BIT_PALETTE);
      uTextureID_Minimap_Loop = pIcons_LOD->LoadTexture("ib-autmask-c", TEXTURE_16BIT_PALETTE);
      uTextureID_Compas = pIcons_LOD->LoadTexture("IB-COMP-C", TEXTURE_16BIT_PALETTE);
      dword_5079D0 = pIcons_LOD->LoadTexture("IB-InitG-c", TEXTURE_16BIT_PALETTE);
      dword_5079C8 = pIcons_LOD->LoadTexture("IB-InitY-c", TEXTURE_16BIT_PALETTE);
      dword_5079CC = pIcons_LOD->LoadTexture("IB-InitR-c", TEXTURE_16BIT_PALETTE);
      uTextureID_Btn_NPCLeft = pIcons_LOD->LoadTexture("IB-NPCLD-C", TEXTURE_16BIT_PALETTE);
      uTextureID_Btn_NPCRight = pIcons_LOD->LoadTexture("IB-NPCRD-C", TEXTURE_16BIT_PALETTE);
      uTextureID_Btn_ZoomIn = pIcons_LOD->LoadTexture("ib-autout-C", TEXTURE_16BIT_PALETTE);
      uTextureID_Btn_ZoomOut = pIcons_LOD->LoadTexture("ib-autin-C", TEXTURE_16BIT_PALETTE);
      uTextureID_GameUI_CharSelectionFrame = pIcons_LOD->LoadTexture("IB-selec-C", TEXTURE_16BIT_PALETTE);
      uTextureID_Btn_CastSpell = pIcons_LOD->LoadTexture("ib-m1d-c", TEXTURE_16BIT_PALETTE);
      uTextureID_Btn_Rest = pIcons_LOD->LoadTexture("ib-m2d-c", TEXTURE_16BIT_PALETTE);
      uTextureID_Btn_QuickReference = pIcons_LOD->LoadTexture("ib-m3d-c", TEXTURE_16BIT_PALETTE);
      uTextureID_Btn_GameSettings = pIcons_LOD->LoadTexture("ib-m4d-c", TEXTURE_16BIT_PALETTE);
      uExitCancelTextureId = pIcons_LOD->LoadTexture("ib-bcu-c", TEXTURE_16BIT_PALETTE);
      uTextureID_PlayerBuff_Bless = pIcons_LOD->LoadTexture("isg-01-c", TEXTURE_16BIT_PALETTE);
      uTextureID_PlayerBuff_Preservation = pIcons_LOD->LoadTexture("isg-02-c", TEXTURE_16BIT_PALETTE);
      uTextureID_PlayerBuff_Hammerhands = pIcons_LOD->LoadTexture("isg-03-c", TEXTURE_16BIT_PALETTE);
      uTextureID_PlayerBuff_PainReflection = pIcons_LOD->LoadTexture("isg-04-c", TEXTURE_16BIT_PALETTE);
      uTextureID_50795C = pIcons_LOD->LoadTexture("evtnpc-c", TEXTURE_16BIT_PALETTE);
      uTextureID_CharacterUI_InventoryBackground = pIcons_LOD->LoadTexture("fr_inven", TEXTURE_16BIT_PALETTE);
      pUIAnim_WizardEye->uIconID = pIconsFrameTable->FindIcon("wizeyeC");
      pIconsFrameTable->InitializeAnimation((signed __int16)pUIAnim_WizardEye->uIconID);
      pUIAnum_Torchlight->uIconID = pIconsFrameTable->FindIcon("torchC");
      pIconsFrameTable->InitializeAnimation((signed __int16)pUIAnum_Torchlight->uIconID);
    }
    uGameUIFontMain = Color16(0xC8u, 0, 0);
    uGameUIFontShadow = Color16(10, 0, 0);
  }
  else if (align == PartyAlignment_Neutral)
  {
    if ( bReplace )
    {
      pTexture_RightFrame->Reload("ib-r-a.pcx");
      pTexture_BottomFrame->Reload("ib-b-a.pcx");
      pTexture_TopFrame->Reload("ib-t-a.pcx");
      pTexture_LeftFrame->Reload("ib-l-a.pcx");
      pTexture_StatusBar->Reload("IB-Foot-a.pcx");
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_right_panel], "ib-mb-a", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_Minimap_Loop], "ib-autmask-a", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_Compas], "IB-COMP-a", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[dword_5079D0], "IB-InitG-a", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[dword_5079C8], "IB-InitY-a", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[dword_5079CC], "IB-InitR-a", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_Btn_NPCLeft], "IB-NPCLD-a", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_Btn_NPCRight], "IB-NPCRD-a", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_Btn_ZoomIn], "ib-autout-a", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_Btn_ZoomOut], "ib-autin-a", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_GameUI_CharSelectionFrame], "IB-selec-a", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_Btn_CastSpell], "ib-m1d-a", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_Btn_Rest], "ib-m2d-a", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_Btn_QuickReference], "ib-m3d-a", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_Btn_GameSettings], "ib-m4d-a", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_PlayerBuff_Bless], "isg-01-a", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_PlayerBuff_Preservation], "isg-02-a", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_PlayerBuff_Hammerhands], "isg-03-a", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_PlayerBuff_PainReflection], "isg-04-a", 2);
      pUIAnim_WizardEye->uIconID = pIconsFrameTable->FindIcon("wizeyeA");
      pIconsFrameTable->InitializeAnimation((signed __int16)pUIAnim_WizardEye->uIconID);
      pUIAnum_Torchlight->uIconID = pIconsFrameTable->FindIcon("torchA");
      pIconsFrameTable->InitializeAnimation((signed __int16)pUIAnum_Torchlight->uIconID);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uExitCancelTextureId], "ib-bcu-a", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_50795C], "evtnpc", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_CharacterUI_InventoryBackground], "fr_inven", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_Parchment], "parchment", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_5076B4], "cornr_ll", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_5076B0], "cornr_lr", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_5076AC], "cornr_ul", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_5076A8], "cornr_ur", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_5076A4], "edge_btm", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_5076A0], "edge_lf", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_50769C], "edge_rt", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_507698], "edge_top", 2);
      pIcons_LOD->ReloadTexture(pTexture_591428, "endcap", 2);
    }
    else
    {
      pTexture_RightFrame->Load("ib-r-A.pcx", 0);
      pTexture_BottomFrame->Load("ib-b-A.pcx", 0);
      pTexture_TopFrame->Load("ib-t-A.pcx", 0);
      pTexture_LeftFrame->Load("ib-l-A.pcx", 0);
      pTexture_StatusBar->Load("IB-Foot-a.pcx", 0);
      uTextureID_right_panel = pIcons_LOD->LoadTexture("ib-mb-A", TEXTURE_16BIT_PALETTE);
      uTextureID_Minimap_Loop = pIcons_LOD->LoadTexture("ib-autmask-a", TEXTURE_16BIT_PALETTE);
      uTextureID_Compas = pIcons_LOD->LoadTexture("IB-COMP-A", TEXTURE_16BIT_PALETTE);
      dword_5079D0 = pIcons_LOD->LoadTexture("IB-InitG-a", TEXTURE_16BIT_PALETTE);
      dword_5079C8 = pIcons_LOD->LoadTexture("IB-InitY-a", TEXTURE_16BIT_PALETTE);
      dword_5079CC = pIcons_LOD->LoadTexture("IB-InitR-a", TEXTURE_16BIT_PALETTE);
      uTextureID_Btn_NPCLeft = pIcons_LOD->LoadTexture("IB-NPCLD-A", TEXTURE_16BIT_PALETTE);
      uTextureID_Btn_NPCRight = pIcons_LOD->LoadTexture("IB-NPCRD-A", TEXTURE_16BIT_PALETTE);
      uTextureID_GameUI_CharSelectionFrame = pIcons_LOD->LoadTexture("IB-selec-A", TEXTURE_16BIT_PALETTE);
      uTextureID_Btn_CastSpell = pIcons_LOD->LoadTexture("ib-m1d-a", TEXTURE_16BIT_PALETTE);
      uTextureID_Btn_Rest = pIcons_LOD->LoadTexture("ib-m2d-a", TEXTURE_16BIT_PALETTE);
      uTextureID_Btn_QuickReference = pIcons_LOD->LoadTexture("ib-m3d-a", TEXTURE_16BIT_PALETTE);
      uTextureID_Btn_GameSettings = pIcons_LOD->LoadTexture("ib-m4d-a", TEXTURE_16BIT_PALETTE);
      uTextureID_Btn_ZoomIn = pIcons_LOD->LoadTexture("ib-autout-a", TEXTURE_16BIT_PALETTE);
      uTextureID_Btn_ZoomOut = pIcons_LOD->LoadTexture("ib-autin-a", TEXTURE_16BIT_PALETTE);
      uExitCancelTextureId = pIcons_LOD->LoadTexture("ib-bcu-a", TEXTURE_16BIT_PALETTE);
      uTextureID_PlayerBuff_Bless = pIcons_LOD->LoadTexture("isg-01-a", TEXTURE_16BIT_PALETTE);
      uTextureID_PlayerBuff_Preservation = pIcons_LOD->LoadTexture("isg-02-a", TEXTURE_16BIT_PALETTE);
      uTextureID_PlayerBuff_Hammerhands = pIcons_LOD->LoadTexture("isg-03-a", TEXTURE_16BIT_PALETTE);
      uTextureID_PlayerBuff_PainReflection = pIcons_LOD->LoadTexture("isg-04-a", TEXTURE_16BIT_PALETTE);
      uTextureID_50795C = pIcons_LOD->LoadTexture("evtnpc", TEXTURE_16BIT_PALETTE);
      uTextureID_CharacterUI_InventoryBackground = pIcons_LOD->LoadTexture("fr_inven", TEXTURE_16BIT_PALETTE);
      pUIAnim_WizardEye->uIconID = pIconsFrameTable->FindIcon("wizeyeA");
      pIconsFrameTable->InitializeAnimation((signed __int16)pUIAnim_WizardEye->uIconID);
      pUIAnum_Torchlight->uIconID = pIconsFrameTable->FindIcon("torchA");
      pIconsFrameTable->InitializeAnimation((signed __int16)pUIAnum_Torchlight->uIconID);
      uTextureID_Parchment = pIcons_LOD->LoadTexture("parchment", TEXTURE_16BIT_PALETTE);
      uTextureID_5076B4 = pIcons_LOD->LoadTexture("cornr_ll", TEXTURE_16BIT_PALETTE);
      uTextureID_5076B0 = pIcons_LOD->LoadTexture("cornr_lr", TEXTURE_16BIT_PALETTE);
      uTextureID_5076AC = pIcons_LOD->LoadTexture("cornr_ul", TEXTURE_16BIT_PALETTE);
      uTextureID_5076A8 = pIcons_LOD->LoadTexture("cornr_ur", TEXTURE_16BIT_PALETTE);
      uTextureID_5076A4 = pIcons_LOD->LoadTexture("edge_btm", TEXTURE_16BIT_PALETTE);
      uTextureID_5076A0 = pIcons_LOD->LoadTexture("edge_lf", TEXTURE_16BIT_PALETTE);
      uTextureID_50769C = pIcons_LOD->LoadTexture("edge_rt", TEXTURE_16BIT_PALETTE);
      uTextureID_507698 = pIcons_LOD->LoadTexture("edge_top", TEXTURE_16BIT_PALETTE);
      pTexture_591428 = pIcons_LOD->LoadTexturePtr("endcap", TEXTURE_16BIT_PALETTE);
    }
    uGameUIFontMain = Color16(0xAu, 0, 0);
    uGameUIFontShadow = Color16(230, 214, 193);
  }
  else if (align == PartyAlignment_Good)
  {
    if ( bReplace )
    {
      pTexture_RightFrame->Reload("ib-r-B.pcx");
      pTexture_BottomFrame->Reload("ib-b-B.pcx");
      pTexture_TopFrame->Reload("ib-t-B.pcx");
      pTexture_LeftFrame->Reload("ib-l-B.pcx");
      pTexture_StatusBar->Reload("IB-Foot-b.pcx");
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_right_panel], "ib-mb-B", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_Minimap_Loop], "ib-autmask-b", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_Compas], "IB-COMP-B", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[dword_5079D0], "IB-InitG-b", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[dword_5079C8], "IB-InitY-b", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[dword_5079CC], "IB-InitR-b", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_Btn_NPCLeft], "IB-NPCLD-B", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_Btn_NPCRight], "IB-NPCRD-B", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_Btn_ZoomIn], "ib-autout-B", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_Btn_ZoomOut], "ib-autin-B", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_GameUI_CharSelectionFrame], "IB-selec-B", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_Btn_CastSpell], "ib-m1d-b", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_Btn_Rest], "ib-m2d-b", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_Btn_QuickReference], "ib-m3d-b", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_Btn_GameSettings], "ib-m4d-b", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_PlayerBuff_Bless], "isg-01-b", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_PlayerBuff_Preservation], "isg-02-b", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_PlayerBuff_Hammerhands], "isg-03-b", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_PlayerBuff_PainReflection], "isg-04-b", 2);
      pUIAnim_WizardEye->uIconID = pIconsFrameTable->FindIcon("wizeyeB");
      pIconsFrameTable->InitializeAnimation((signed __int16)pUIAnim_WizardEye->uIconID);
      pUIAnum_Torchlight->uIconID = pIconsFrameTable->FindIcon("torchB");
      pIconsFrameTable->InitializeAnimation((signed __int16)pUIAnum_Torchlight->uIconID);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uExitCancelTextureId], "ib-bcu-b", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_50795C], "evtnpc-b", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_CharacterUI_InventoryBackground], "fr_inven-b", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_Parchment], "parchment", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_5076B4], "cornr_ll-b", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_5076B0], "cornr_lr-b", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_5076AC], "cornr_ul-b", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_5076A8], "cornr_ur-b", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_5076A4], "edge_btm-b", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_5076A0], "edge_lf-b", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_50769C], "edge_rt-b", 2);
      pIcons_LOD->ReloadTexture(&pIcons_LOD->pTextures[uTextureID_507698], "edge_top-b", 2);
      pIcons_LOD->ReloadTexture(pTexture_591428, "endcap-b", 2);
    }
    uGameUIFontMain = Color16(0, 0, 0xC8u);
    uGameUIFontShadow = Color16(255, 255, 255);
  }
  else Error("Invalid alignment type: %u", align);
}
//----- (0041D20D) --------------------------------------------------------
void DrawBuff_remaining_time_string( int uY, struct GUIWindow *window, __int64 remaining_time, struct GUIFont *Font )
{
  unsigned int full_time; // esi@1
  signed __int64 hours; // kr00_8@1
  const char *text; // eax@2
  signed __int64 minutes; // [sp+10h] [bp-10h]@1
  signed __int64 seconds; // [sp+18h] [bp-8h]@1
  unsigned int day; // [sp+24h] [bp+4h]@1

  full_time = (signed __int64)((double)remaining_time * 0.234375);
  day = (unsigned int)((full_time / 60) / 60) / 24;
  hours = ((full_time / 60) / 60) % 24;
  minutes = (signed __int64)(full_time / 60) % 60;
  seconds = (signed __int64)full_time % 60;
  strcpy(pTmpBuf.data(), "\r020");
  if ( day )
  {
    text = pGlobalTXT_LocalizationStrings[57];   // Days
    if ( day <= 1 )
      text = pGlobalTXT_LocalizationStrings[56]; // Day
    sprintfex(pTmpBuf2.data(), "%d %s ", (int)day, text);
    strcat(pTmpBuf.data(), pTmpBuf2.data());
  }
  if ( hours )
  {
    if ( hours <= 1 )
      text = pGlobalTXT_LocalizationStrings[109];// Hour
    else
      text = pGlobalTXT_LocalizationStrings[110];// Hours
    sprintfex(pTmpBuf2.data(), "%d %s ", (int)hours, text);
    strcat(pTmpBuf.data(), pTmpBuf2.data());
  }
  if ( minutes && !day )
  {
    if ( minutes <= 1 )
      text = pGlobalTXT_LocalizationStrings[437];// Minute
    else
      text = pGlobalTXT_LocalizationStrings[436];// Minutes
    sprintfex(pTmpBuf2.data(), "%d %s ", (int)minutes, text);
    strcat(pTmpBuf.data(), pTmpBuf2.data());
  }
  if ( seconds && !hours )
  {
    if ( seconds <= 1 )
      text = pGlobalTXT_LocalizationStrings[439];// Second
    else
      text = pGlobalTXT_LocalizationStrings[438];// Seconds
    sprintfex(pTmpBuf2.data(), "%d %s ", (int)seconds, text);
    strcat(pTmpBuf.data(), pTmpBuf2.data());
  }
  window->DrawText(Font, 32, uY, 0, pTmpBuf.data(), 0, 0, 0);
}


//----- (0042EB8D) --------------------------------------------------------
void GUIMessageQueue::AddMessageImpl(UIMessageType msg, int param, unsigned int a4, const char *file, int line)
{
  //Log::Warning(L"%s @ (%S %u)", UIMessage2String(msg), file, line);
  if (uNumMessages < 40)
  {
    files[uNumMessages] = file;
    lines[uNumMessages] = line;

    pMessages[uNumMessages].eType = msg;
    pMessages[uNumMessages].param = param;
    pMessages[uNumMessages++].field_8 = a4;
  }
}

//----- (004637E0) --------------------------------------------------------
char  sub_4637E0_is_there_popup_onscreen()
{
  return dword_507BF0_is_there_popup_onscreen == 1;
}
// 507BF0: using guessed type int dword_507BF0_is_there_popup_onscreen;