#define _CRT_SECURE_NO_WARNINGS
#include "..\..\mm7_unsorted_subs.h"
#include "..\..\MM7.h"
#include "..\..\Render.h"
#include "..\..\Mouse.h"
#include "..\UIBooks.h"
#include "..\..\MapInfo.h"
#include "..\..\GUIWindow.h"
#include "..\..\GUIFont.h"
#include "..\..\Party.h"
#include "..\..\AudioPlayer.h"
#include "..\..\LOD.h"
#include "..\..\Viewport.h"
#include "..\..\Awards.h"
#include "..\..\Autonotes.h"
#include "..\..\StorylineTextTable.h"
#include "..\..\texts.h"
#include "..\..\mm7_data.h"

//----- (00413D3C) --------------------------------------------------------
static const char *GetDayPart()
{
  if ( pParty->uCurrentHour > 5 && pParty->uCurrentHour < 20 )
    return pGlobalTXT_LocalizationStrings[56]; // "Day"
  else if ( pParty->uCurrentHour == 5 )
    return pGlobalTXT_LocalizationStrings[55]; // "Dawn"
  else if ( pParty->uCurrentHour == 20 )
    return pGlobalTXT_LocalizationStrings[566]; // "Dusk"
  else
    return pGlobalTXT_LocalizationStrings[567]; // "Night"
}

//----- (00413D6F) --------------------------------------------------------
void BookUI_Calendar_Draw()
{
  int am; // ecx@5
  char *pMapName; // eax@6
  GUIWindow calendar_window; // [sp+Ch] [bp-60h]@5
  unsigned int pMapID; // [sp+60h] [bp-Ch]@1
  unsigned int pHour;

  static unsigned int pDayMoonPhase[28] = // 4E1B18
  {
    0, 0, 0,
    1, 1, 1, 1,
    2, 2, 2,
    3, 3, 3, 3,
    4, 4, 4,
    3, 3, 3, 3,
    2, 2, 2,
    1, 1, 1, 1
  };

  pRenderer->DrawTextureIndexed(pViewport->uViewportTL_X, pViewport->uViewportTL_Y, pSpellBookPagesTextr_13);
  pHour = pParty->uCurrentHour;
  if ( (signed int)pHour >= 12 )
  {
    pHour -= 12;
    if ( !pHour )
      pHour = 12;
    am = 1;
  }
  else
    am = 0;

  calendar_window.uFrameX = game_viewport_x;
  calendar_window.uFrameY = game_viewport_y;
  calendar_window.uFrameWidth = game_viewport_width;
  calendar_window.uFrameHeight = game_viewport_height;
  calendar_window.uFrameZ = game_viewport_z;
  calendar_window.uFrameW = game_viewport_w;
  calendar_window.DrawTitleText(pBook2Font, 0, 22, ui_book_calendar_title_color, pGlobalTXT_LocalizationStrings[186], 3); // "Time in Erathia"

  sprintf(pTmpBuf.data(), "%s\t100:\t110%d:%02d %s - %s", pGlobalTXT_LocalizationStrings[526], // "Time"
     pHour, pParty->uCurrentMinute, aAMPMNames[am], GetDayPart());
  calendar_window.DrawText(pBookFont, 70, 55, ui_book_calendar_time_color, pTmpBuf.data(), 0, 0, 0);

  sprintf(pTmpBuf.data(), "%s\t100:\t110%d - %s", pGlobalTXT_LocalizationStrings[56], // "Day"
     pParty->uDaysPlayed + 1, aDayNames[pParty->uDaysPlayed % 7]);
  calendar_window.DrawText(pBookFont, 70, 2 * LOBYTE(pBookFont->uFontHeight) + 49, ui_book_calendar_day_color, pTmpBuf.data(), 0, 0, 0);

  sprintf(pTmpBuf.data(), "%s\t100:\t110%d - %s", pGlobalTXT_LocalizationStrings[146], // "Month"
        pParty->uCurrentMonth + 1, aMonthNames[pParty->uCurrentMonth]);
  calendar_window.DrawText(pBookFont, 70, 4 * LOBYTE(pBookFont->uFontHeight) + 43, ui_book_calendar_month_color, pTmpBuf.data(), 0, 0, 0);

  sprintf(pTmpBuf.data(), "%s\t100:\t110%d", pGlobalTXT_LocalizationStrings[245], pParty->uCurrentYear); // "Year"
  calendar_window.DrawText(pBookFont, 70, 6 * LOBYTE(pBookFont->uFontHeight) + 37, ui_book_calendar_year_color, pTmpBuf.data(), 0, 0, 0);

  sprintf(pTmpBuf.data(), "%s\t100:\t110%s", pGlobalTXT_LocalizationStrings[530], aMoonPhaseNames[pDayMoonPhase[pParty->uDaysPlayed]]); // "Moon"
  calendar_window.DrawText(pBookFont, 70, 8 * LOBYTE(pBookFont->uFontHeight) + 31, ui_book_calendar_moon_color, pTmpBuf.data(), 0, 0, 0);

  pMapID = pMapStats->GetMapInfo(pCurrentMapName);
  if ( pMapID )
    pMapName = pMapStats->pInfos[pMapID].pName;
  else
    pMapName = "Unknown";
  sprintf(pTmpBuf.data(), "%s\t100:\t110%s", pGlobalTXT_LocalizationStrings[531], pMapName); // "Location"
  calendar_window.DrawText(pBookFont, 70, 10 * LOBYTE(pBookFont->uFontHeight) + 25, ui_book_calendar_location_color, pTmpBuf.data(), 0, 0, 0);
}

//----- (00413126) --------------------------------------------------------
void BookUI_Questbook_Draw()
{
  int pTextHeight; // eax@19
  GUIWindow questbook_window; // [sp+Ch] [bp-54h]@9

  pRenderer->DrawTextureIndexed(pViewport->uViewportTL_X, pViewport->uViewportTL_Y, pTexture_CurrentBook);
  if ( BtnUp_flag || !books_primary_item_per_page )//Bookmark Up(Закладка вверх)
    pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 407, pViewport->uViewportTL_Y + 2, pTex_book_button1_off);
  else
    pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 398, pViewport->uViewportTL_Y + 1, pTex_book_button1_on);

  if ( BtnDown_flag || books_primary_item_per_page + num_achieved_awards >= full_num_items_in_book )//Bookmark Down(Закладка вниз)
    pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 407, pViewport->uViewportTL_Y + 38, pTex_book_button2_off);
  else
    pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 398, pViewport->uViewportTL_Y + 38, pTex_book_button2_on);

  //for title
  questbook_window.uFrameWidth = game_viewport_width;
  questbook_window.uFrameHeight = game_viewport_height;
  questbook_window.uFrameX = game_viewport_x;
  questbook_window.uFrameY = game_viewport_y;
  questbook_window.uFrameZ = game_viewport_z;
  questbook_window.uFrameW = game_viewport_w;
  questbook_window.DrawTitleText(pBook2Font, 0, 22, ui_book_quests_title_color, pGlobalTXT_LocalizationStrings[174], 3); //"Current Quests"

  //for other text
  questbook_window.uFrameX = 48;
  questbook_window.uFrameY = 70;
  questbook_window.uFrameWidth = 360;
  questbook_window.uFrameHeight = 264;
  questbook_window.uFrameZ = 407;
  questbook_window.uFrameW = 333;
  if ( BtnDown_flag && books_primary_item_per_page + num_achieved_awards < full_num_items_in_book )//Click Bookmark Down(нажатие закладки вниз)
  {
    pAudioPlayer->PlaySound(SOUND_OpenBook, 0, 0, -1, 0, 0, 0, 0);
    books_primary_item_per_page += num_achieved_awards;
    books_num_items_per_page[books_page_number++] = num_achieved_awards;
  }
  if ( BtnUp_flag && books_page_number )//Click Bookmark Up(Нажатие закладки вверх)
  {
    pAudioPlayer->PlaySound(SOUND_OpenBook, 0, 0, -1, 0, 0, 0, 0);
    --books_page_number;
    books_primary_item_per_page -= (unsigned __int8)books_num_items_per_page[books_page_number];
  }
  if ( !num_achieved_awards || !books_primary_item_per_page )
  {
    books_page_number = 0;
    books_primary_item_per_page = 0;
  }
  BtnDown_flag = 0;
  BtnUp_flag = 0;
  num_achieved_awards = 0;
  for ( uint i = books_primary_item_per_page; i < full_num_items_in_book; ++i )
  {
    ++num_achieved_awards;
    questbook_window.DrawText(pAutonoteFont, 1, 0, ui_book_quests_text_color, pQuestTable[achieved_awards[i]], 0, 0, 0);
    pTextHeight = pAutonoteFont->CalcTextHeight(pQuestTable[achieved_awards[i]], &questbook_window, 1, 0);
    if ( (signed int)(questbook_window.uFrameY + pTextHeight) > (signed int)questbook_window.uFrameHeight )
      break;
    pRenderer->DrawTextureTransparent(100, (questbook_window.uFrameY + pTextHeight) + 12, pSpellBookPagesTextr_10);
    questbook_window.uFrameY = (questbook_window.uFrameY + pTextHeight) + 24;
  }
}

//----- (0041338E) --------------------------------------------------------
void BookUI_Autonotes_Draw()
{
  int pTextHeight; // eax@65
  bool change_flag; // [sp+10h] [bp-58h]@1
  GUIWindow autonotes_window; // [sp+14h] [bp-54h]@46

  change_flag = false;
  pRenderer->DrawTextureIndexed(pViewport->uViewportTL_X, pViewport->uViewportTL_Y, pTexture_AutonotesBook);
  if ( BtnUp_flag || !books_primary_item_per_page )
    pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 407, pViewport->uViewportTL_Y + 2, pTex_book_button1_off);
  else
    pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 398, pViewport->uViewportTL_Y + 1, pTex_book_button1_on);

  if ( BtnDown_flag || books_primary_item_per_page + num_achieved_awards >= full_num_items_in_book )
    pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 407, pViewport->uViewportTL_Y + 38, pTex_book_button2_off);
  else
    pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 398, pViewport->uViewportTL_Y + 38, pTex_book_button2_on);

  if ( Book_PageBtn3_flag )//Potions_page_flag
  {
    if (_506568_autonote_type == AUTONOTE_POTION_RECEPIE)//press again(повторное нажатие)
      pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 398, pViewport->uViewportTL_Y + 113, pTex_book_button3_on);
    else//press(нажатие)
    {
      change_flag = true;
      pAudioPlayer->PlaySound(SOUND_Button2, 0, 0, -1, 0, 0, 0, 0);
      _506568_autonote_type = AUTONOTE_POTION_RECEPIE;
      pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 398, pViewport->uViewportTL_Y + 113, pTex_book_button3_on);
    }
  }
  else 
  {
    if (_506568_autonote_type == AUTONOTE_POTION_RECEPIE)// default(по умолчанию при запуске окна)
      pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 398, pViewport->uViewportTL_Y + 113, pTex_book_button3_on);
    else//Potions_page not active(вкладка снадобья не активна)
      pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 408, pViewport->uViewportTL_Y + 113, pTex_book_button3_off);
  }

  if ( Book_PageBtn4_flag )//Fontains_page_flag
  {
    if ( _506568_autonote_type == AUTONOTE_STAT_HINT )
      pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 399, pViewport->uViewportTL_Y + 150, pTex_book_button4_on);
    else
    {
      change_flag = true;
      pAudioPlayer->PlaySound(SOUND_Button2, 0, 0, -1, 0, 0, 0, 0);
      _506568_autonote_type = AUTONOTE_STAT_HINT;
      pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 399, pViewport->uViewportTL_Y + 150, pTex_book_button4_on);
    }
  }
  else
  {
    if ( _506568_autonote_type == AUTONOTE_STAT_HINT )
      pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 399, pViewport->uViewportTL_Y + 150, pTex_book_button4_on);
    else
      pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 408, pViewport->uViewportTL_Y + 150, pTex_book_button4_off);
  }

  if ( Book_PageBtn5_flag )//Autonotes_Obelisks_page_flag
  {
    if ( _506568_autonote_type == AUTONOTE_OBELISK)
      pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 397, pViewport->uViewportTL_Y + 188, pTex_book_button5_on);
    else
    {
      change_flag = true;
      pAudioPlayer->PlaySound(SOUND_Button2, 0, 0, -1, 0, 0, 0, 0);
      _506568_autonote_type = AUTONOTE_OBELISK;
      pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 397, pViewport->uViewportTL_Y + 188, pTex_book_button5_on);
    }
  }
  else
  {
    if ( _506568_autonote_type == AUTONOTE_OBELISK)
      pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 397, pViewport->uViewportTL_Y + 188, pTex_book_button5_on);
    else
      pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 408, pViewport->uViewportTL_Y + 188, pTex_book_button5_off);
  }

  if ( Book_PageBtn6_flag )//Autonotes_Seer_page_flag
  {
    if ( _506568_autonote_type == AUTONOTE_SEER)
      pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 397, pViewport->uViewportTL_Y + 226, pTex_book_button6_on);
    else
    {
      change_flag = true;
      pAudioPlayer->PlaySound(SOUND_Button2, 0, 0, -1, 0, 0, 0, 0);
      _506568_autonote_type = AUTONOTE_SEER;
      pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 397, pViewport->uViewportTL_Y + 226, pTex_book_button6_on);
    }
  }
  else
  {
    if ( _506568_autonote_type == AUTONOTE_SEER)
      pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 397, pViewport->uViewportTL_Y + 226, pTex_book_button6_on);
    else
      pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 408, pViewport->uViewportTL_Y + 226, pTex_book_button6_off);
  }

  if ( Autonotes_Misc_page_flag )
  {
    if ( _506568_autonote_type == AUTONOTE_MISC)
      pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 397, pViewport->uViewportTL_Y + 264, pTex_book_button7_on);
    else
    {
      change_flag = true;
      pAudioPlayer->PlaySound(SOUND_Button2, 0, 0, -1, 0, 0, 0, 0);
      _506568_autonote_type = AUTONOTE_MISC;
      pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 397, pViewport->uViewportTL_Y + 264, pTex_book_button7_on);
    }
  }
  else
  {
    if ( _506568_autonote_type == AUTONOTE_MISC)
      pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 397, pViewport->uViewportTL_Y + 264, pTex_book_button7_on);
    else
      pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 408, pViewport->uViewportTL_Y + 263, pTex_book_button7_off);
  }

  if ( Autonotes_Instructors_page_flag )
  {
    if ( _506568_autonote_type == AUTONOTE_TEACHER)
      pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 397, pViewport->uViewportTL_Y + 302, pTex_book_button8_on);
    else
    {
      change_flag = true;
      pAudioPlayer->PlaySound(SOUND_Button2, 0, 0, -1, 0, 0, 0, 0);
      _506568_autonote_type = AUTONOTE_TEACHER;
      pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 397, pViewport->uViewportTL_Y + 302, pTex_book_button8_on);
    }
  }
  else
  {
    if ( _506568_autonote_type == AUTONOTE_TEACHER)
      pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 397, pViewport->uViewportTL_Y + 302, pTex_book_button8_on);
    else
     pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 408, pViewport->uViewportTL_Y + 302, pTex_book_button8_off);
  }

  //for title
  autonotes_window.uFrameWidth = game_viewport_width;
  autonotes_window.uFrameHeight = game_viewport_height;
  autonotes_window.uFrameX = game_viewport_x;
  autonotes_window.uFrameY = game_viewport_y;
  autonotes_window.uFrameZ = game_viewport_z;
  autonotes_window.uFrameW = game_viewport_w;
  autonotes_window.DrawTitleText(pBook2Font, 0, 22, ui_book_autonotes_title_color, pGlobalTXT_LocalizationStrings[154], 3); // "Auto notes"

  //for other text
  autonotes_window.uFrameX = 48;
  autonotes_window.uFrameY = 70;
  autonotes_window.uFrameWidth = 360;
  autonotes_window.uFrameHeight = 264;
  autonotes_window.uFrameZ = 407;
  autonotes_window.uFrameW = 333;
  if ( change_flag )//change bookmark(смена закладки)
  {
    full_num_items_in_book = 0;
    books_primary_item_per_page = 0;
    books_page_number = 0;
    num_achieved_awards = 0;
    for ( uint i = 1; i <= 195; ++i )
    {
      if ( pAutonoteTxt[i].eType == _506568_autonote_type )
      {
        if ( (unsigned __int16)_449B57_test_bit(pParty->_autonote_bits, i) && (char *)pAutonoteTxt[i].pText )
          achieved_awards[num_achieved_awards++] = (AwardType)i;
      }
    }
    full_num_items_in_book = num_achieved_awards;
  }
  else//not change bookmark(не меняется закладка)
  {
    if ( BtnDown_flag )// press Down bookmark(нажатие закладки пролистать дальше)
    {
      if ( num_achieved_awards + books_primary_item_per_page < full_num_items_in_book )
      {
        books_num_items_per_page[books_page_number++] = num_achieved_awards;
        books_primary_item_per_page = num_achieved_awards + books_primary_item_per_page;
        pAudioPlayer->PlaySound(SOUND_OpenBook, 0, 0, -1, 0, 0, 0, 0);
      }
    }
    if ( BtnUp_flag && books_page_number )// press Up bookmark(нажатие закладки пролистать назад)
    {
      --books_page_number;
      books_primary_item_per_page -= (unsigned __int8)books_num_items_per_page[books_page_number];
      pAudioPlayer->PlaySound(SOUND_OpenBook, 0, 0, -1, 0, 0, 0, 0);
    }
    if ( !num_achieved_awards || !books_primary_item_per_page )//количество записей 0 или номер первой страницы 0
    {
      books_primary_item_per_page = 0;
      books_page_number = 0;
    }
  }
  BtnUp_flag = 0;
  BtnDown_flag = 0;
  Book_PageBtn3_flag = 0;//Potions_page_flag
  Book_PageBtn4_flag = 0;//Fontains_page_flag
  Book_PageBtn5_flag = 0;//Autonotes_Obelisks_page_flag
  Book_PageBtn6_flag = 0;//Autonotes_Seer_page_flag
  Autonotes_Misc_page_flag = 0;
  Autonotes_Instructors_page_flag = 0;
  num_achieved_awards = 0;
  for ( uint i = books_primary_item_per_page; i < full_num_items_in_book; ++i )
  {
    ++num_achieved_awards;
    autonotes_window.DrawText(pAutonoteFont, 1, 0, ui_book_autonotes_text_color, pAutonoteTxt[achieved_awards[i]].pText, 0, 0, 0);
    pTextHeight = pAutonoteFont->CalcTextHeight(pAutonoteTxt[achieved_awards[i]].pText, &autonotes_window, 1, 0);
    if ( (signed int)(autonotes_window.uFrameY + pTextHeight) > (signed int)autonotes_window.uFrameHeight )
      break;
    pRenderer->DrawTextureTransparent(100, (autonotes_window.uFrameY + pTextHeight) + 12, pSpellBookPagesTextr_10);
    autonotes_window.uFrameY = (autonotes_window.uFrameY + pTextHeight) + 24;
  }
}

    //----- (00412E85) --------------------------------------------------------
void BookUI_Journal_Draw()
{
  char *pDialogueString; // eax@21
  char* pStringOnPage; // eax@22
  GUIWindow journal_window; // [sp+8h] [bp-54h]@10

  pRenderer->DrawTextureIndexed(pViewport->uViewportTL_X, pViewport->uViewportTL_Y, pSpellBookPagesTextr_11);
  if ( BtnUp_flag || !books_primary_item_per_page )
    pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 407, pViewport->uViewportTL_Y + 2, pTex_book_button1_off);
  else
    pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 398, pViewport->uViewportTL_Y + 1, pTex_book_button1_on);

  if ( BtnDown_flag || books_primary_item_per_page + num_achieved_awards >= full_num_items_in_book )
    pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 407, pViewport->uViewportTL_Y + 38, pTex_book_button2_off);
  else
    pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 398, pViewport->uViewportTL_Y + 38, pTex_book_button2_on);

  if ( !Journal_limitation_factor[books_primary_item_per_page] )//for title
  {
    journal_window.uFrameWidth = game_viewport_width;
    journal_window.uFrameX = game_viewport_x;
    journal_window.uFrameY = game_viewport_y;
    journal_window.uFrameHeight = game_viewport_height;
    journal_window.uFrameZ = game_viewport_z;
    journal_window.uFrameW = game_viewport_w;
    if ( pStorylineText->StoreLine[achieved_awards[books_primary_item_per_page]].pPageTitle )
      journal_window.DrawTitleText(pBook2Font, 0, 22, ui_book_journal_title_color, pStorylineText->StoreLine[achieved_awards[books_primary_item_per_page]].pPageTitle, 3);
  }

  //for other text
  journal_window.uFrameX = 48;
  journal_window.uFrameY = 70;
  journal_window.uFrameWidth = 360;
  journal_window.uFrameHeight = 264;
  journal_window.uFrameZ = 407;
  journal_window.uFrameW = journal_window.uFrameHeight + 69;
  if ( BtnDown_flag && books_primary_item_per_page + num_achieved_awards < full_num_items_in_book )//Press bookmark next page
  {
    pAudioPlayer->PlaySound(SOUND_OpenBook, 0, 0, -1, 0, 0, 0, 0);
    books_primary_item_per_page += num_achieved_awards;
    books_num_items_per_page[books_page_number++] = num_achieved_awards;
  }
  if ( BtnUp_flag && books_page_number )//Press bookmark previous page
  {
    pAudioPlayer->PlaySound(SOUND_OpenBook, 0, 0, -1, 0, 0, 0, 0);
    --books_page_number;
    books_primary_item_per_page -= (unsigned __int8)books_num_items_per_page[books_page_number];
  }
  if ( !num_achieved_awards || books_primary_item_per_page < 1 )
  {
    books_primary_item_per_page = 0;
    books_page_number = 0;
  }
  BtnDown_flag = 0;
  BtnUp_flag = 0;
  num_achieved_awards = 0;
  if ( achieved_awards[books_primary_item_per_page] )
  {
    int index = ((int)achieved_awards[books_primary_item_per_page] - 1);
    pDialogueString = BuildDialogueString(pStorylineText->StoreLine[ achieved_awards[books_primary_item_per_page]].pText, uActiveCharacter - 1,
                                          0, 0, 0, &pParty->PartyTimes.HistoryEventTimes[index]);
    pStringOnPage = pAutonoteFont->GetPageTop(pDialogueString, &journal_window, 1, (unsigned __int8)Journal_limitation_factor[books_primary_item_per_page]);
    journal_window.DrawText(pAutonoteFont, 1, 0, ui_book_journal_text_color, pStringOnPage, 0,
                            journal_window.uFrameY + journal_window.uFrameHeight, ui_book_journal_text_shadow);
    ++num_achieved_awards;
  }
}
