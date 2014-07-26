#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define _CRT_SECURE_NO_WARNINGS
#include "..\mm7_unsorted_subs.h"
#include "..\Items.h"
#include "..\GUIWindow.h"
#include "..\mm7_data.h"
#include "..\texts.h"
#include "UIHouses.h"
#include "..\GUIFont.h"
#include "..\Render.h"
#include "..\Party.h"
#include "..\Texture.h"
#include "..\Mouse.h"
#include "..\Events2D.h"
#include "..\AudioPlayer.h"
#include "..\LOD.h"

//----- (004B5D7C) --------------------------------------------------------
void GuildDialog()
{
  signed int base_teach_price; // ebx@1
  int v6; // esi@13
  signed int v7; // esi@17
  int v8; // esi@22
//  const char *statusbar_string; // ecx@26
  int v13; // ecx@30
  ItemGen *v15; // ST18_4@31
  int v17; // eax@31
  char *v18; // edx@31
  int v26; // ecx@47
  GUIButton *pButton; // eax@49
  int pTextHeight; // eax@55
  unsigned __int16 pTextColor; // ax@55
  POINT cursor; // [sp+26Ch] [bp-80h]@30
  GUIWindow working_window; // [sp+274h] [bp-78h]@1
  int v58; // [sp+2D4h] [bp-18h]@1
  bool pSkillFlag; // [sp+2DCh] [bp-10h]@35
  int v61; // [sp+2E0h] [bp-Ch]@35
  unsigned int v62; // [sp+2E4h] [bp-8h]@13
  int pPrice; // [sp+2E8h] [bp-4h]@1
  int all_text_height;
  int pX;

  memcpy(&working_window, window_SpeakInHouse, sizeof(GUIWindow));
  working_window.uFrameX = 483;
  working_window.uFrameWidth = 148;
  working_window.uFrameZ = 334;
  base_teach_price = (signed __int64)(p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].fPriceMultiplier * 500.0);
  pPrice = base_teach_price * (100 - pPlayers[uActiveCharacter]->GetMerchant()) / 100;
  if ( pPrice < base_teach_price / 3 )
    pPrice = base_teach_price / 3;
  if ( dialog_menu_id == HOUSE_DIALOGUE_MAIN )
  {
    if ( !(unsigned __int16)_449B57_test_bit((unsigned __int8 *)pPlayers[uActiveCharacter]->_achieved_awards_bits,
            guild_mambership_flags[(unsigned int)window_SpeakInHouse->ptr_1C - 139]) )
    { //you must me member
      pTextHeight = pFontArrus->CalcTextHeight(pNPCTopics[121].pText, &working_window, 0, 0);
      working_window.DrawTitleText(pFontArrus, 0, (212 - pTextHeight) / 2 + 101, Color16(0xFFu, 0xFFu, 0x9Bu), pNPCTopics[121].pText, 3);
      pDialogueWindow->pNumPresenceButton = 0;
      return;
    }
    if ( !HouseUI_CheckIfPlayerCanInteract() )
      return;
    if ( pDialogueWindow->pStartingPosActiveItem >= pDialogueWindow->pStartingPosActiveItem + pDialogueWindow->pNumPresenceButton )
    {
      sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[544], pPlayers[uActiveCharacter]->pName, pClassNames[pPlayers[uActiveCharacter]->classType]); // "Seek knowledge elsewhere %s the %s"	
      strcat(pTmpBuf.data(), "\n \n");
      strcat(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[528]); //"I can offer you nothing further."
      pTextHeight = pFontArrus->CalcTextHeight(pTmpBuf.data(), &working_window, 0, 0);
      working_window.DrawTitleText(pFontArrus, 0, (174 - pTextHeight) / 2 + 138, Color16(0xFFu, 0xFFu, 0x9Bu), pTmpBuf.data(), 3);
      return;
    }
    v61 = 0;
    pSkillFlag = false;
    all_text_height = 0;
    for ( int i = pDialogueWindow->pStartingPosActiveItem;
          i < pDialogueWindow->pNumPresenceButton + pDialogueWindow->pStartingPosActiveItem; ++i )
    {
      if ( pDialogueWindow->GetControl(i)->msg_param == 18 )
      {
        all_text_height += pFontArrus->CalcTextHeight(pGlobalTXT_LocalizationStrings[400], &working_window, 0, 0); //"Buy Spells"
        v61++;
      }
      else
      {
        if( byte_4ED970_skill_learn_ability_by_class_table[pPlayers[uActiveCharacter]->classType][pDialogueWindow->GetControl(i)->msg_param - 36] 
            && !pPlayers[uActiveCharacter]->pActiveSkills[pDialogueWindow->GetControl(i)->msg_param - 36] )
        {
          all_text_height += pFontArrus->CalcTextHeight(pSkillNames[pDialogueWindow->GetControl(i)->msg_param - 36], &working_window, 0, 0);
          v61++;
          pSkillFlag = true;
        }
      }
    }
    if ( !v61 )
    {
      sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[544], pPlayers[uActiveCharacter]->pName, pClassNames[pPlayers[uActiveCharacter]->classType]); // "Seek knowledge elsewhere %s the %s"	
      strcat(pTmpBuf.data(), "\n \n");
      strcat(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[528]); //"I can offer you nothing further."
      pTextHeight = pFontArrus->CalcTextHeight(pTmpBuf.data(), &working_window, 0, 0);
      working_window.DrawTitleText(pFontArrus, 0, (174 - pTextHeight) / 2 + 138, Color16(0xFFu, 0xFFu, 0x9Bu), pTmpBuf.data(), 3);
      return;
    }
    if ( pSkillFlag )
    {
      sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[401], pPrice); //"Skill Cost: %lu"
      working_window.DrawTitleText(pFontArrus, 0, 0x92u, 0, pTmpBuf.data(), 3);
    }
    v58 = (149 - all_text_height) / v61;
    if ( v58 > 32 )
      v58 = 32;
    v26 = (149 - v61 * v58 - all_text_height) / 2 - v58 / 2 + 162;
    for ( int i = pDialogueWindow->pStartingPosActiveItem;
          i < pDialogueWindow->pNumPresenceButton + pDialogueWindow->pStartingPosActiveItem; ++i )
    {
      pButton = pDialogueWindow->GetControl(i);
      if ( pButton->msg_param == 18 )
      {
        pButton->uY = v58 + v26;
        pTextHeight = pFontArrus->CalcTextHeight(pGlobalTXT_LocalizationStrings[400], &working_window, 0, 0);
        pButton->uHeight = pTextHeight;
        v26 = pButton->uY + pTextHeight - 1;
        pButton->uW = v26;
        pTextColor = Color16(0xFFu, 0xFFu, 0x9Bu);
        if ( pDialogueWindow->pCurrentPosActiveItem != i )
          pTextColor = Color16(0xFFu, 0xFFu, 0xFFu);
        working_window.DrawTitleText(pFontArrus, 0, pButton->uY, pTextColor, pGlobalTXT_LocalizationStrings[400], 3);//"Buy Spells"
      }
      else
      {
        if ( byte_4ED970_skill_learn_ability_by_class_table[pPlayers[uActiveCharacter]->classType][pButton->msg_param - 36] 
             && !pPlayers[uActiveCharacter]->pActiveSkills[pButton->msg_param - 36] )
        {
          pButton->uY = v58 + v26;
          pTextHeight = pFontArrus->CalcTextHeight(pSkillNames[pButton->msg_param - 36], &working_window, 0, 0);
          pButton->uHeight = pTextHeight;
          v26 = pButton->uY + pTextHeight - 1;
          pButton->uW = v26;
          pTextColor = Color16(0xFFu, 0xFFu, 0x9Bu);
          if ( pDialogueWindow->pCurrentPosActiveItem != i )
            pTextColor = Color16(0xFFu, 0xFFu, 0xFFu);
          working_window.DrawTitleText(pFontArrus, 0, pButton->uY, pTextColor, pSkillNames[pButton->msg_param - 36], 3);
        }
        else
        {
          pButton->uW = 0;
          pButton->uHeight = 0;
          pButton->uY = 0;
        }
      }
    }
    return;
  }
  if ( dialog_menu_id == HOUSE_DIALOGUE_GUILD_BUY_BOOKS ) //buy skill
  {
    pRenderer->DrawTextureIndexed(8, 8, ShopTexture);
    v6 = 0;
    v62 = 0;
    for ( pX = 32; pX < 452; pX += 70 )//������������ � ������� ����
    {
      if ( pParty->SpellBooksInGuilds[window_SpeakInHouse->par1C-139][v6].uItemID )
      {
        pRenderer->DrawTextureTransparent(pX, 90, ItemsInShopTexture[v6]);
        ZBuffer_DoFill((int *)((char *)pRenderer->pActiveZBuffer + v62 + 230528), ItemsInShopTexture[v6], v6 + 1);
      }
      v62 += 280;
      ++v6;
    }
    v62 = 1680;
    v7 = 6;
    for ( pX = 32; pX < 452; pX += 70 )//������������ � ������ ����
    {
      if (pParty->SpellBooksInGuilds[window_SpeakInHouse->par1C-139][v7].uItemID)
      {
        pRenderer->DrawTextureTransparent(pX, 250, ItemsInShopTexture[v7]);
        ZBuffer_DoFill((int *)((char *)pRenderer->pActiveZBuffer + v62 + 638448), ItemsInShopTexture[v7], v7 + 1);
      }
      v62 += 280;
      ++v7;
    }
    if ( HouseUI_CheckIfPlayerCanInteract() )
    {
      v8 = 0;
      for ( uint i = 0; i < 12; ++i )
      {
        if ( pParty->SpellBooksInGuilds[window_SpeakInHouse->par1C - 139][i].uItemID > 0 )
          ++v8;
      }
      GetAsyncKeyState(17);
      DrawTextAtStatusBar(pGlobalTXT_LocalizationStrings[195], 0);//"Select the Item to Buy"
      if ( !v8 )
      {
        working_window.DrawShops_next_generation_time_string(pParty->PartyTimes.Shops_next_generation_time[window_SpeakInHouse->par1C - 139] - pParty->uTimePlayed);//"��������� ����� 14 ����"
        return;
      }
      pMouse->GetCursorPos(&cursor);
      v13 = pRenderer->pActiveZBuffer[cursor.x + pSRZBufferLineOffsets[cursor.y]] & 0xFFFF;
      if ( v13 )
      {
        v15 = (ItemGen *)(&pParty->pPlayers[1].uExpressionTimeLength + 18 * (v13 + 12 * (int)window_SpeakInHouse->ptr_1C));
        v17 = pPlayers[uActiveCharacter]->SelectPhrasesTransaction( (ItemGen *)&pParty->pPlayers[1].uExpressionTimeLength + v13 + 12 * (int)window_SpeakInHouse->ptr_1C, BuildingType_MagicShop, (int)window_SpeakInHouse->ptr_1C,  2);
        v18 = BuildDialogueString(pMerchantsBuyPhrases[v17], uActiveCharacter - 1, v15, (char *)window_SpeakInHouse->ptr_1C, 2, 0);
        pTextHeight = pFontArrus->CalcTextHeight(v18, &working_window, 0, 0);
        working_window.DrawTitleText(pFontArrus, 0, (174 - pTextHeight) / 2 + 138,  Color16(0xFFu, 0xFFu, 0xFFu), v18, 3);
        return;
      }
    }
    return;
  }
  if ( HouseUI_CheckIfPlayerCanInteract() )
  {
    if ( pPlayers[uActiveCharacter]->pActiveSkills[dialog_menu_id-36] )
    {
      sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[403], pSkillNames[dialog_menu_id-36]); //"You already know the %s skill"
      ShowStatusBarString(pTmpBuf.data(), 2);
      pAudioPlayer->PlaySound(SOUND_error, 0, 0, -1, 0, 0, 0, 0);
    }
    else
    {
      if ( pParty->uNumGold < pPrice )
      {
        ShowStatusBarString(pGlobalTXT_LocalizationStrings[155], 2); //"You don't have enough gold"
        PlayHouseSound((unsigned int)window_SpeakInHouse->ptr_1C, HouseSound_NotEnoughMoney_TrainingSuccessful);
      }
      else
      {
        Party::TakeGold(pPrice);
        pPlayers[uActiveCharacter]->pActiveSkills[dialog_menu_id-36] = 1;
      }
    }
  }
  pMessageQueue_50CBD0->AddGUIMessage(UIMSG_Escape, 1, 0);
  return;
}
//----- (004BC8D5) --------------------------------------------------------
void SpellBookGenerator()//for GuildDialogs
{
  int pItemNum; // esi@1
  int v4; // esi@7

  for( int i = 0; i < 12; ++i )
  {
    if ( p2DEvents[window_SpeakInHouse->par1C - 1].uType >= 5 )
    {
      if ( p2DEvents[window_SpeakInHouse->par1C - 1].uType <= 13 )
        pItemNum = rand() % word_4F0F30[(signed int)window_SpeakInHouse->par1C - 139] + 11 * p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].uType + 345;
      else
      {
        if ( p2DEvents[window_SpeakInHouse->par1C - 1].uType == 14 )
          v4 = rand() % 4;
        else if ( p2DEvents[window_SpeakInHouse->par1C - 1].uType == 15 )
          v4 = rand() % 3 + 4;
        else if ( p2DEvents[window_SpeakInHouse->par1C - 1].uType == 16 )
          v4 = rand() % 2 + 7;
        if( p2DEvents[window_SpeakInHouse->par1C - 1].uType <= 16 )
          pItemNum = rand() % word_4F0F30[(signed int)window_SpeakInHouse->par1C - 139] + 11 * v4 + 400;
      }
    }
    if ( pItemNum == 487 )
    {
      if ( !(unsigned __int16)_449B57_test_bit(pParty->_quest_bits, 239) )
        pItemNum = 486;
    }
    ItemGen * item_spellbook = &pParty->SpellBooksInGuilds[window_SpeakInHouse->par1C-139][i];
    item_spellbook->Reset();
    pParty->SpellBooksInGuilds[window_SpeakInHouse->par1C-139][i].uItemID = pItemNum;
    pParty->SpellBooksInGuilds[window_SpeakInHouse->par1C-139][i].IsIdentified();
    ItemsInShopTexture[i] = pIcons_LOD->LoadTexturePtr(pItemsTable->pItems[pItemNum].pIconName, TEXTURE_16BIT_PALETTE);
  }
  return;
}