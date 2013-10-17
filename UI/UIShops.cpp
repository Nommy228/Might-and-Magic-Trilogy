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
#include "..\MapInfo.h"
#include "..\Viewport.h"

//----- (004B910F) --------------------------------------------------------
void  WeaponShopDialog()
{
  int phrases_id; // eax@16
  int v19; // edi@25
  GUIButton *pButton; // esi@27
  unsigned int pColorText; // ax@27
  signed int v32; // esi@41
  const char *pText; // ecx@84
  int all_text_height; // esi@96
  ItemGen *item; 
  int pItemID;
  POINT mouse; // [sp+64h] [bp-94h]@8
  GUIWindow dialog_window; // [sp+7Ch] [bp-7Ch]@1
  __int32 v103; // [sp+D4h] [bp-24h]@25
  int pPrice; // [sp+ECh] [bp-Ch]@26
  unsigned int v109;
  int pNumString;
  int item_X;

  memcpy(&dialog_window, window_SpeakInHouse, sizeof(dialog_window));
  dialog_window.uFrameX = 483;
  dialog_window.uFrameWidth = 148;
  dialog_window.uFrameZ = 334;

  switch(dialog_menu_id)
  {
    case HOUSE_DIALOGUE_MAIN:
    {
      if ( HouseUI_CheckIfPlayerCanInteract() )
      {
        pShopOptions[0] = const_cast<char *>(pGlobalTXT_LocalizationStrings[134]);
        pShopOptions[1] = const_cast<char *>(pGlobalTXT_LocalizationStrings[152]);
        pShopOptions[2] = const_cast<char *>(pGlobalTXT_LocalizationStrings[159]);
        pShopOptions[3] = const_cast<char *>(pGlobalTXT_LocalizationStrings[160]);
        all_text_height = 0;
        for ( int i = 0; i < 4; ++i )
          all_text_height += pFontArrus->CalcTextHeight(pShopOptions[i], &dialog_window, 0, 0);
        v103 = (174 - all_text_height) / 4;
        v19 = (174 - 4 * (174 - all_text_height) / 4 - all_text_height) / 2 - (174 - all_text_height) / 4 / 2 + 138;
        pNumString = 0;
        for ( int i = pDialogueWindow->pStartingPosActiveItem; i < pDialogueWindow->pNumPresenceButton + pDialogueWindow->pStartingPosActiveItem; ++i )
        {
          pButton = pDialogueWindow->GetControl(i);
          pButton->uY = v103 + v19;
          pButton->uHeight = pFontArrus->CalcTextHeight((const char *)pShopOptions[pNumString], &dialog_window, 0, 0);
          v19 = pButton->uY + pFontArrus->CalcTextHeight((const char *)pShopOptions[pNumString], &dialog_window, 0, 0) - 1;
          pButton->uW = v19;
          pColorText = TargetColor(0xE1u, 0xCDu, 0x23u);
          if ( pDialogueWindow->pCurrentPosActiveItem != i )
            pColorText = TargetColor(0xFFu, 0xFFu, 0xFFu);
          dialog_window.DrawTitleText(pFontArrus, 0, pButton->uY, pColorText, (const char *)pShopOptions[pNumString], 3);
          ++pNumString;
        }
      }
      break;
    }
    case HOUSE_DIALOGUE_SHOP_BUY_STANDARD:
    {
      pRenderer->DrawTextureIndexed(8, 8, ShopTexture);
      item_X = 0;
      for ( uint i = 0; i < 6; ++i )
      {
        if ( pParty->StandartItemsInShops[(unsigned int)window_SpeakInHouse->ptr_1C][i].uItemID)
        {
          pRenderer->DrawTextureTransparent((60 - ((signed int)ItemsInShopTexture[i]->uTextureWidth / 2)) + item_X, weapons_Ypos[i] + 30, ItemsInShopTexture[i]);
          ZBuffer_DoFill2(&pRenderer->pActiveZBuffer[(60 - ((signed int)ItemsInShopTexture[i]->uTextureWidth / 2)) + item_X + 640 * (weapons_Ypos[i] + 30)], ItemsInShopTexture[i], i + 1);
        }
        item_X += 70;
      }
      if ( HouseUI_CheckIfPlayerCanInteract() )
      {
        uint item_num = 0;
        for ( uint i = 0; i < 6; ++i )
        {
          if ( pParty->StandartItemsInShops[(unsigned int)window_SpeakInHouse->ptr_1C][i].uItemID)
            ++item_num;
        }
        if ( GetAsyncKeyState(17) && pPlayers[uActiveCharacter]->CanSteal() )
          pText = pGlobalTXT_LocalizationStrings[185];//Украсть предмет
        else
          pText = pGlobalTXT_LocalizationStrings[195];//Выберите предмет для покупки
        DrawTextAtStatusBar(pText, 0);
        if ( item_num )
        {
          pMouse->GetCursorPos(&mouse);
          if ( pRenderer->pActiveZBuffer[mouse.x + pSRZBufferLineOffsets[mouse.y]] & 0xFFFF )
          {
            item = &pParty->StandartItemsInShops[(int)window_SpeakInHouse->ptr_1C][(pRenderer->pActiveZBuffer[mouse.x + pSRZBufferLineOffsets[mouse.y]] & 0xFFFF) - 1];
            if ( !GetAsyncKeyState(17) || !pPlayers[uActiveCharacter]->CanSteal() )
            {
              pText = BuildDialogueString(pMerchantsBuyPhrases[pPlayers[uActiveCharacter]->SelectPhrasesTransaction(item, BuildingType_WeaponShop, (int)window_SpeakInHouse->ptr_1C, 2)],
                                       uActiveCharacter - 1, item, (char *)window_SpeakInHouse->ptr_1C, 2, 0);
              dialog_window.DrawTitleText(pFontArrus, 0, (174 - pFontArrus->CalcTextHeight(pText, &dialog_window, 0, 0)) / 2 + 138, TargetColor(0xFFu, 0xFFu, 0xFFu), pText, 3);
            }
            else
            {
              pText = BuildDialogueString(pGlobalTXT_LocalizationStrings[181], uActiveCharacter - 1, item, (char *)window_SpeakInHouse->ptr_1C, 2, 0);
              dialog_window.DrawTitleText(pFontArrus, 0, (174 - pFontArrus->CalcTextHeight(pText, &dialog_window, 0, 0)) / 2 + 138, TargetColor(0xFFu, 0xFFu, 0xFFu), pText, 3);
            }
          }
        }
        else
          dialog_window.DrawShops_next_generation_time_string(pParty->PartyTimes.Shops_next_generation_time[(unsigned int)window_SpeakInHouse->ptr_1C] - pParty->uTimePlayed);//Приходите через 7 дней
      }
      break;
    }

    case HOUSE_DIALOGUE_SHOP_BUY_SPECIAL:
    {
      pRenderer->DrawTextureIndexed(8, 8, ShopTexture);
      item_X = 0;
      for ( uint i = 0; i < 6; ++i )
      {
        if (pParty->SpecialItemsInShops[(unsigned int)window_SpeakInHouse->ptr_1C][i].uItemID)
        {
          pRenderer->DrawTextureTransparent((60 - ((signed int)ItemsInShopTexture[i]->uTextureWidth / 2)) + item_X, weapons_Ypos[i] + 30, ItemsInShopTexture[i]);
          ZBuffer_DoFill2(&pRenderer->pActiveZBuffer[(60 - ((signed int)ItemsInShopTexture[i]->uTextureWidth / 2)) + item_X + 640 * (weapons_Ypos[i] + 30)], ItemsInShopTexture[i], i + 1);
        }
        item_X += 70;
      }
      if ( HouseUI_CheckIfPlayerCanInteract() )
      {
        uint item_num = 0;
        for ( uint i = 0; i < 6; ++i )
        {
          if ( pParty->SpecialItemsInShops[(unsigned int)window_SpeakInHouse->ptr_1C][i].uItemID )
            ++item_num;
        }
        if ( GetAsyncKeyState(17) && pPlayers[uActiveCharacter]->CanSteal() )
          pText = pGlobalTXT_LocalizationStrings[185];
        else
            pText = pGlobalTXT_LocalizationStrings[196];
        DrawTextAtStatusBar(pText, 0);
        if ( item_num )
        {
          pMouse->GetCursorPos(&mouse);
          if ( pRenderer->pActiveZBuffer[mouse.x + pSRZBufferLineOffsets[mouse.y]] & 0xFFFF )
          {
            item = &pParty->SpecialItemsInShops[(int)window_SpeakInHouse->ptr_1C][(pRenderer->pActiveZBuffer[mouse.x + pSRZBufferLineOffsets[mouse.y]] & 0xFFFF) - 1];
            if ( !GetAsyncKeyState(17) || !pPlayers[uActiveCharacter]->CanSteal() )
            {
              pText = BuildDialogueString(pMerchantsBuyPhrases[pPlayers[uActiveCharacter]->SelectPhrasesTransaction(item, BuildingType_WeaponShop, (int)window_SpeakInHouse->ptr_1C, 2)],
                                       uActiveCharacter - 1, item, (char *)window_SpeakInHouse->ptr_1C, 2, 0);
              dialog_window.DrawTitleText(pFontArrus, 0, (174 - pFontArrus->CalcTextHeight(pText, &dialog_window, 0, 0)) / 2 + 138, TargetColor(0xFFu, 0xFFu, 0xFFu), pText, 3);
            }
            else
            {
              pText = BuildDialogueString(pGlobalTXT_LocalizationStrings[181], uActiveCharacter - 1, item, (char *)window_SpeakInHouse->ptr_1C, 2, 0);
              dialog_window.DrawTitleText(pFontArrus, 0, (174 - pFontArrus->CalcTextHeight(pText, &dialog_window, 0, 0)) / 2 + 138, TargetColor(0xFFu, 0xFFu, 0xFFu), pText, 3);
            }
          }
        }
        else
          dialog_window.DrawShops_next_generation_time_string(pParty->PartyTimes.Shops_next_generation_time[(unsigned int)window_SpeakInHouse->ptr_1C]  - pParty->uTimePlayed);
      }
      break;
    }

    case HOUSE_DIALOGUE_SHOP_SELL:
    {
      draw_leather();
      CharacterUI_InventoryTab_Draw(pPlayers[uActiveCharacter], true);
      DrawTextAtStatusBar(pGlobalTXT_LocalizationStrings[199], 0);
      if ( !HouseUI_CheckIfPlayerCanInteract() )
        return;
      pMouse->GetCursorPos(&mouse);
      v109 = ((mouse.x + 14) >> 5) + 14 * ((mouse.y - 17) >> 5);
      if( mouse.x <= 13 || mouse.x >= 462 )
        return;
      pItemID = pPlayers[uActiveCharacter]->GetItemIDAtInventoryIndex((int *)&v109);
      if ( !pItemID )
        return;
      item = &pPlayers[uActiveCharacter]->pInventoryItemList[pItemID - 1];
      phrases_id = pPlayers[uActiveCharacter]->SelectPhrasesTransaction(item, BuildingType_WeaponShop, (int)window_SpeakInHouse->ptr_1C, 3);
      pText = BuildDialogueString(pMerchantsSellPhrases[phrases_id], uActiveCharacter - 1, item, (char *)window_SpeakInHouse->ptr_1C, 3, 0);
      dialog_window.DrawTitleText(pFontArrus, 0, (174 - pFontArrus->CalcTextHeight(pText, &dialog_window, 0, 0)) / 2 + 138, TargetColor(0xFFu, 0xFFu, 0xFFu), pText, 3);
      break;
    }
    case HOUSE_DIALOGUE_SHOP_IDENTIFY:
    {
      draw_leather();
      CharacterUI_InventoryTab_Draw(pPlayers[uActiveCharacter], true);
      DrawTextAtStatusBar(pGlobalTXT_LocalizationStrings[197], 0);
      if ( !HouseUI_CheckIfPlayerCanInteract() )
        return;
      pMouse->GetCursorPos(&mouse);
      v109 = ((mouse.x - 14) >> 5) + 14 * ((mouse.y - 17) >> 5);
      if( mouse.x <= 13 || mouse.x >= 462 )
        return;
      pItemID = pPlayers[uActiveCharacter]->GetItemIDAtInventoryIndex((int *)&v109);
      if ( !pItemID )
        return;
      item = &pPlayers[uActiveCharacter]->pInventoryItemList[pItemID - 1];
      if (!item->IsIdentified())
      {
        phrases_id = pPlayers[uActiveCharacter]->SelectPhrasesTransaction(item, BuildingType_WeaponShop, (int)window_SpeakInHouse->ptr_1C, 4);
        pText = BuildDialogueString(pMerchantsIdentifyPhrases[phrases_id], uActiveCharacter - 1, item, (char *)window_SpeakInHouse->ptr_1C, 4, 0);
        dialog_window.DrawTitleText(pFontArrus, 0, (174 - pFontArrus->CalcTextHeight(pText, &dialog_window, 0, 0)) / 2 + 138, TargetColor(0xFFu, 0xFFu, 0xFFu), pText, 3);
      }
      else
      {
        pText = BuildDialogueString("%24", uActiveCharacter - 1, item, (char *)window_SpeakInHouse->ptr_1C, 4, 0);
        dialog_window.DrawTitleText(pFontArrus, 0, (174 - pFontArrus->CalcTextHeight(pText, &dialog_window, 0, 0)) / 2 + 138, TargetColor(0xFFu, 0xFFu, 0xFFu), pText, 3);
      }
      break;
    }

    case HOUSE_DIALOGUE_SHOP_REPAIR:
    {
      draw_leather();
      CharacterUI_InventoryTab_Draw(pPlayers[uActiveCharacter], true);
      DrawTextAtStatusBar(pGlobalTXT_LocalizationStrings[198], 0);
      if ( !HouseUI_CheckIfPlayerCanInteract())
        return;
      pMouse->GetCursorPos(&mouse);
      v109 = ((mouse.x - 14) >> 5) + 14 * (mouse.y - 17) >> 5;
      if( ( mouse.x <= 13) || mouse.x >= 462 )
        return;
      pItemID = pPlayers[uActiveCharacter]->GetItemIDAtInventoryIndex((int *)&v109);
      if ( !pItemID || (!(pPlayers[uActiveCharacter]->pOwnItems[pItemID - 1].uAttributes & 2)) )
        return;
      item = &pPlayers[uActiveCharacter]->pInventoryItemList[pItemID - 1];
      phrases_id = pPlayers[uActiveCharacter]->SelectPhrasesTransaction(item, BuildingType_ArmorShop, (int)window_SpeakInHouse->ptr_1C, 5);
      pText = BuildDialogueString(pMerchantsRepairPhrases[phrases_id], uActiveCharacter - 1, item, (char *)window_SpeakInHouse->ptr_1C, 5, 0);
      dialog_window.DrawTitleText(pFontArrus, 0, (174 - pFontArrus->CalcTextHeight(pText, &dialog_window, 0, 0)) / 2 + 138, TargetColor(0xFFu, 0xFFu, 0xFFu), pText, 3);
      return;
    }
    break;

    case HOUSE_DIALOGUE_SHOP_DISPLAY_EQUIPMENT:
    {
      draw_leather();
      CharacterUI_InventoryTab_Draw(pPlayers[uActiveCharacter], true);
      pShopOptions[0] = const_cast<char *>(pGlobalTXT_LocalizationStrings[200]);//sell
      pShopOptions[1] = const_cast<char *>(pGlobalTXT_LocalizationStrings[113]);//identify
      pShopOptions[2] = const_cast<char *>(pGlobalTXT_LocalizationStrings[179]);//repair
      all_text_height = 0;
      for ( int i = 0; i < 3; ++i )
        all_text_height += pFontArrus->CalcTextHeight(pShopOptions[i], &dialog_window, 0, 0);
      v103 = (174 - all_text_height) / 3;
      v19 = (3 * (58 - (signed int)v103) - all_text_height) / 2 - ((174 - all_text_height) / 3) / 2 + 138;
      pNumString = 0;
      for ( int i = pDialogueWindow->pStartingPosActiveItem; i < pDialogueWindow->pNumPresenceButton + pDialogueWindow->pStartingPosActiveItem; ++i )
      {
        pButton = pDialogueWindow->GetControl(i);
        pButton->uY = v103 + v19;
        pButton->uHeight = pFontArrus->CalcTextHeight((const char *)pShopOptions[pNumString], &dialog_window, 0, 0);
        v19 = pButton->uY + pFontArrus->CalcTextHeight((const char *)pShopOptions[pNumString], &dialog_window, 0, 0) - 1;
        pButton->uW = v19;
        pColorText = TargetColor(0xE1u, 0xCDu, 0x23u);
        if ( pDialogueWindow->pCurrentPosActiveItem != i )
          pColorText = TargetColor(0xFFu, 0xFFu, 0xFFu);
        dialog_window.DrawTitleText(pFontArrus, 0, pButton->uY, pColorText, (const char *)pShopOptions[pNumString], 3);
        ++pNumString;
        }
      break;
    }

    case HOUSE_DIALOGUE_LEARN_SKILLS:
    {
      if (!HouseUI_CheckIfPlayerCanInteract())
        return;
      uint item_num = 0;
      all_text_height = 0;
      v32 = (signed __int64)(p2DEvents[(signed int)window_SpeakInHouse->ptr_1C - 1].flt_24 * 500.0);
      pPrice = v32 * (100 - pPlayers[uActiveCharacter]->GetMerchant()) / 100;
      if ( pPrice < v32 / 3 )
        pPrice = v32 / 3;
      for ( int i = pDialogueWindow->pStartingPosActiveItem;
            i < pDialogueWindow->pNumPresenceButton + pDialogueWindow->pStartingPosActiveItem; ++i )
      {
        if ( byte_4ED970_skill_learn_ability_by_class_table[pPlayers[uActiveCharacter]->classType][pDialogueWindow->GetControl(i)->msg_param - 36]
          && !pPlayers[uActiveCharacter]->pActiveSkills[pDialogueWindow->GetControl(i)->msg_param - 36] )
        {
          all_text_height += pFontArrus->CalcTextHeight(pSkillNames[pDialogueWindow->GetControl(i)->msg_param - 36], &dialog_window, 0, 0);
          item_num++;
        }
      }
      if ( item_num )
      {
        sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[401], pPrice);//Стоимость навыка: %lu
        dialog_window.DrawTitleText(pFontArrus, 0, 0x92u, 0, pTmpBuf.data(), 3);
        v103 = (149 - all_text_height) / item_num;
        if ( (149 - all_text_height) / item_num > 32 )
          v103 = 32;
        v19 = (149 - item_num * v103 - all_text_height) / 2 - v103 / 2 + 162;
        for ( uint i = pDialogueWindow->pStartingPosActiveItem;
                   i < pDialogueWindow->pNumPresenceButton + pDialogueWindow->pStartingPosActiveItem; ++i )
        {
          pButton = pDialogueWindow->GetControl(i);
          if ( !byte_4ED970_skill_learn_ability_by_class_table[pPlayers[uActiveCharacter]->classType][pButton->msg_param - 36] || pPlayers[uActiveCharacter]->pActiveSkills[pButton->msg_param - 36] )
          {
            pButton->uW = 0;
            pButton->uHeight = 0;
            pButton->uY = 0;
          }
          else
          {
            pButton->uY = v103 + v19;
            pButton->uHeight = pFontArrus->CalcTextHeight(pSkillNames[pButton->msg_param - 36], &dialog_window, 0, 0);
            v19 = pButton->uY + pFontArrus->CalcTextHeight(pSkillNames[pButton->msg_param - 36], &dialog_window, 0, 0) - 1;
            pButton->uW = v19;
            pColorText = TargetColor(0xE1u, 0xCDu, 0x23u);
            if ( pDialogueWindow->pCurrentPosActiveItem != i )
              pColorText = TargetColor(0xFFu, 0xFFu, 0xFFu);
            dialog_window.DrawTitleText(pFontArrus, 0, pButton->uY, pColorText, pSkillNames[pButton->msg_param - 36], 3);
          }
        }
        break;
      }
      sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[544], pPlayers[uActiveCharacter]->pName, pClassNames[pPlayers[uActiveCharacter]->classType]);
      strcat(pTmpBuf.data(), "\n \n");
      strcat(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[528]);//Больше ничего не могу предложить.
      dialog_window.DrawTitleText(pFontArrus, 0, (174 - pFontArrus->CalcTextHeight(pTmpBuf.data(), &dialog_window, 0, 0)) / 2 + 138, TargetColor(0xE1u, 0xCDu, 0x23u), pTmpBuf.data(), 3);
      return;
    }
    default:
    {
      __debugbreak();
      break;
    }
  }
}

//----- (004BA928) --------------------------------------------------------
void  ArmorShopDialog()
{
  int phrases_id; // eax@20
  int all_text_height; // ebx@22
  int v146; // [sp+E4h] [bp-20h]@24
  int v23; // ebx@24
  GUIButton *pButton; // eax@26
  signed int v38; // esi@42
  int v59; // eax@68
  const char *pText; // ecx@91
  ItemGen *selected_item; // ecx@99
  int pNumString; // edx@109
  unsigned __int16 pTextColor; // ax@109
  POINT mouse; // [sp+4Ch] [bp-B8h]@97
  GUIWindow dialog_window; // [sp+8Ch] [bp-78h]@1
  __int32 pItemCount; // [sp+F0h] [bp-14h]@8
  int item_x; // [sp+100h] [bp-4h]@44
  int pPrice;
  unsigned int v153;

  memcpy(&dialog_window, window_SpeakInHouse, sizeof(dialog_window));
  dialog_window.uFrameX = 483;
  dialog_window.uFrameWidth = 148;
  dialog_window.uFrameZ = 334;
  switch (dialog_menu_id)
  {
    case HOUSE_DIALOGUE_MAIN:
    {
      if ( !HouseUI_CheckIfPlayerCanInteract() )
        return;
      pShopOptions[0] = const_cast<char *>(pGlobalTXT_LocalizationStrings[134]); //"Buy Standard"
      pShopOptions[1] = const_cast<char *>(pGlobalTXT_LocalizationStrings[152]); //"Buy Special"
      pShopOptions[2] = const_cast<char *>(pGlobalTXT_LocalizationStrings[159]); //"Display Inventory"
      pShopOptions[3] = const_cast<char *>(pGlobalTXT_LocalizationStrings[160]);
      all_text_height = 0;
      for( int i = 0; i < 4; ++i )
        all_text_height += pFontArrus->CalcTextHeight(pShopOptions[i], &dialog_window, 0, 0);
      v146 = ( 174 - all_text_height ) / 4;
      v23 = ( 174 - 4 * ( 174 - all_text_height ) / 4 - all_text_height ) / 2 - ( 174 - all_text_height ) / 4 / 2 + 138;
      pNumString = 0;
      for (int i = pDialogueWindow->pStartingPosActiveItem;
               i < pDialogueWindow->pNumPresenceButton + pDialogueWindow->pStartingPosActiveItem; ++i)
      {
        pButton = pDialogueWindow->GetControl(i);
        pButton->uY = v146 + v23;
        pButton->uHeight = pFontArrus->CalcTextHeight(pShopOptions[pNumString], &dialog_window, 0, 0);
        v23 = pButton->uY + pFontArrus->CalcTextHeight(pShopOptions[pNumString], &dialog_window, 0, 0) - 1;
        pButton->uW = v23;
        pTextColor = TargetColor(0xE1u, 0xCDu, 0x23u);
        if ( pDialogueWindow->pCurrentPosActiveItem != i )
          pTextColor = TargetColor(0xFFu, 0xFFu, 0xFFu);
        dialog_window.DrawTitleText(pFontArrus, 0, pButton->uY, pTextColor, pShopOptions[pNumString], 3);
        ++pNumString;
      }
    }
    break;

    case HOUSE_DIALOGUE_SHOP_BUY_STANDARD:
    {
      pRenderer->DrawTextureIndexed(8, 8, ShopTexture);// подложка
      item_x = 0;
      for ( int i = 0; i < 8; ++i )// разместить вещи
      {
        if ( pParty->StandartItemsInShops[window_SpeakInHouse->par1C][i].uItemID)
        {
          if ( i >= 4 )  //low row
          {
            pRenderer->DrawTextureTransparent((90 - (ItemsInShopTexture[i]->uTextureWidth / 2)) + item_x - 420, 126, ItemsInShopTexture[i]);
            v59 = (90 - (ItemsInShopTexture[i]->uTextureWidth / 2)) + item_x + 80220;
          }
          else
          {
            pRenderer->DrawTextureTransparent(( 86 - (ItemsInShopTexture[i]->uTextureWidth / 2)) + item_x, 98 -  ItemsInShopTexture[i]->uTextureHeight, ItemsInShopTexture[i]);
            v59 = item_x + (86 - (ItemsInShopTexture[i]->uTextureWidth / 2)) + 640 * (98 -  ItemsInShopTexture[i]->uTextureHeight);
          }
          ZBuffer_DoFill(&pRenderer->pActiveZBuffer[v59], ItemsInShopTexture[i], i + 1);
        }
        item_x += 105;
      }
      if ( !HouseUI_CheckIfPlayerCanInteract() )
        return;
      pItemCount = 0;
      for ( int i = 0; i < 8; ++i )
      {
        if ( pParty->StandartItemsInShops[window_SpeakInHouse->par1C][i].uItemID )
          ++pItemCount;
      }
      if ( GetAsyncKeyState(17) == 0 || pPlayers[uActiveCharacter]->CanSteal() == 0 )
        pText = pGlobalTXT_LocalizationStrings[195]; //"Select the Item to Buy"
      else
        pText = pGlobalTXT_LocalizationStrings[185];//"Steal item"
      DrawTextAtStatusBar(pText, 0);
      if ( pItemCount )
      {
        pMouse->GetCursorPos(&mouse);
        if ( !(pRenderer->pActiveZBuffer[mouse.x + pSRZBufferLineOffsets[mouse.y]] & 0xFFFF) )
          return;
        selected_item = &pParty->StandartItemsInShops[window_SpeakInHouse->par1C][(pRenderer->pActiveZBuffer[mouse.x + pSRZBufferLineOffsets[mouse.y]] & 0xFFFF) - 1];
        if ( !GetAsyncKeyState(17) || !pPlayers[uActiveCharacter]->CanSteal())
        {
          pText = BuildDialogueString(pMerchantsBuyPhrases[pPlayers[uActiveCharacter]->SelectPhrasesTransaction(selected_item, BuildingType_ArmorShop, window_SpeakInHouse->par1C, 2)],
                                 uActiveCharacter - 1, selected_item, (char *)window_SpeakInHouse->ptr_1C, 2, 0);
          dialog_window.DrawTitleText(pFontArrus, 0, (174 - pFontArrus->CalcTextHeight(pText, &dialog_window, 0, 0)) / 2 + 138, TargetColor(0xFFu, 0xFFu, 0xFFu), pText, 3);
        }
        else
        {
          pText = BuildDialogueString(pGlobalTXT_LocalizationStrings[181], uActiveCharacter - 1, selected_item, (char *)window_SpeakInHouse->ptr_1C, 2, 0); //"Steal %24"
          dialog_window.DrawTitleText(pFontArrus, 0, (174 - pFontArrus->CalcTextHeight(pText, &dialog_window, 0, 0)) / 2 + 138, TargetColor(0xFFu, 0xFFu, 0xFFu), pText, 3);
        }
        return;
      }
      dialog_window.DrawShops_next_generation_time_string(pParty->PartyTimes.Shops_next_generation_time[window_SpeakInHouse->par1C]- pParty->uTimePlayed);
      return;
    }
    break;

    case HOUSE_DIALOGUE_SHOP_BUY_SPECIAL:
    {
      pRenderer->DrawTextureIndexed(8, 8, ShopTexture);
      item_x = 0;
      for ( int i = 0; i < 8; ++i )
      {
        if ( pParty->SpecialItemsInShops[window_SpeakInHouse->par1C][i].uItemID )
        {
          if ( i >= 4 )
          {
            pRenderer->DrawTextureTransparent(item_x + (90 - ((signed int)ItemsInShopTexture[i]->uTextureWidth / 2)) - 420, 126, ItemsInShopTexture[i]);
            v59 = item_x + (90 - ((signed int)ItemsInShopTexture[i]->uTextureWidth / 2)) + 80220;
          }
          else
          {
            pRenderer->DrawTextureTransparent(86 - ((signed int)ItemsInShopTexture[i]->uTextureWidth / 2) + item_x, 98 - ItemsInShopTexture[i]->uTextureHeight, ItemsInShopTexture[i]);
            v59 = (86 - ((signed int)ItemsInShopTexture[i]->uTextureWidth / 2)) + item_x + 640 * (98 - ItemsInShopTexture[i]->uTextureHeight);
          }
          ZBuffer_DoFill(&pRenderer->pActiveZBuffer[v59], ItemsInShopTexture[i], i + 1);
        }
        item_x += 105;
      }
      if ( !HouseUI_CheckIfPlayerCanInteract() )
        return;
      pItemCount = 0;
      for ( uint i = 0; i < 6; ++i )
      {
        if (pParty->SpecialItemsInShops[window_SpeakInHouse->par1C][i].uItemID)
          ++pItemCount;
      }
      if ( !GetAsyncKeyState(17) || !pPlayers[uActiveCharacter]->CanSteal() )
        pText = pGlobalTXT_LocalizationStrings[196]; //Select the Special Item to Buy"
      else
        pText = pGlobalTXT_LocalizationStrings[185];
      DrawTextAtStatusBar(pText, 0);
      if ( pItemCount )
      {
        pMouse->GetCursorPos(&mouse);
        if ( !(pRenderer->pActiveZBuffer[mouse.x + pSRZBufferLineOffsets[mouse.y]] & 0xFFFF) )
          return;
        pItemCount = (pRenderer->pActiveZBuffer[mouse.x + pSRZBufferLineOffsets[mouse.y]] & 0xFFFF) - 1;
        if ( dialog_menu_id == 2 )
          selected_item = &pParty->StandartItemsInShops[window_SpeakInHouse->par1C][(pRenderer->pActiveZBuffer[mouse.x + pSRZBufferLineOffsets[mouse.y]] & 0xFFFF) - 1];
        else
          selected_item = &pParty->SpecialItemsInShops[window_SpeakInHouse->par1C][(pRenderer->pActiveZBuffer[mouse.x + pSRZBufferLineOffsets[mouse.y]] & 0xFFFF) - 1];
        if ( !GetAsyncKeyState(17) || !pPlayers[uActiveCharacter]->CanSteal() )
        {
          pText = BuildDialogueString(pMerchantsBuyPhrases[pPlayers[uActiveCharacter]->SelectPhrasesTransaction(selected_item, BuildingType_ArmorShop, window_SpeakInHouse->par1C, 2)],
                 uActiveCharacter - 1, selected_item, (char *)window_SpeakInHouse->ptr_1C, 2, 0);
          dialog_window.DrawTitleText(pFontArrus, 0, (174 - pFontArrus->CalcTextHeight(pText, &dialog_window, 0, 0)) / 2 + 138, TargetColor(0xFFu, 0xFFu, 0xFFu), pText, 3);
        }
        else
        {
          pText = BuildDialogueString(pGlobalTXT_LocalizationStrings[181], uActiveCharacter - 1, selected_item, (char *)window_SpeakInHouse->ptr_1C, 2, 0);//"Steal %24"
          dialog_window.DrawTitleText(pFontArrus, 0, (174 - pFontArrus->CalcTextHeight(pText, &dialog_window, 0, 0)) / 2 + 138, TargetColor(0xFFu, 0xFFu, 0xFFu), pText, 3);
        }
        return;
      }
      dialog_window.DrawShops_next_generation_time_string(pParty->PartyTimes.Shops_next_generation_time[window_SpeakInHouse->par1C]- pParty->uTimePlayed);
      return;
    }
    break;

    case HOUSE_DIALOGUE_SHOP_DISPLAY_EQUIPMENT:
    {
      draw_leather();
      CharacterUI_InventoryTab_Draw(pPlayers[uActiveCharacter], true);
      pShopOptions[0] = const_cast<char *>(pGlobalTXT_LocalizationStrings[200]); //"Sell"
      pShopOptions[1] = const_cast<char *>(pGlobalTXT_LocalizationStrings[113]); //"Identify"
      pShopOptions[2] = const_cast<char *>(pGlobalTXT_LocalizationStrings[179]); //"Repair"
      all_text_height = 0;
      for ( int i = 0; i < 3; ++i )
        all_text_height += pFontArrus->CalcTextHeight(pShopOptions[i], &dialog_window, 0, 0);
      v146 = (174 - all_text_height) / 3;
      v23 = (3 * (58 - v146) - all_text_height) / 2 - v146 / 2 + 138;
      pNumString = 0;
      for ( int i = pDialogueWindow->pStartingPosActiveItem;
                i  < pDialogueWindow->pNumPresenceButton + pDialogueWindow->pStartingPosActiveItem; ++i )
      {
        pButton = pDialogueWindow->GetControl(i);
        pButton->uY = v146 + v23;
        pButton->uHeight = pFontArrus->CalcTextHeight(pShopOptions[pNumString], &dialog_window, 0, 0);
        v23 = pButton->uY + pFontArrus->CalcTextHeight(pShopOptions[pNumString], &dialog_window, 0, 0) - 1;
        pButton->uW = v23;
        pTextColor = TargetColor(0xE1u, 0xCDu, 0x23u);
        if ( pDialogueWindow->pCurrentPosActiveItem != i )
          pTextColor = TargetColor(0xFFu, 0xFFu, 0xFFu);
        dialog_window.DrawTitleText(pFontArrus, 0, pButton->uY, pTextColor, pShopOptions[pNumString], 3);
        ++pNumString;
      }
      return;
    }
    break;

    case HOUSE_DIALOGUE_SHOP_SELL:
    {
      draw_leather();
      CharacterUI_InventoryTab_Draw(pPlayers[uActiveCharacter], true);
      DrawTextAtStatusBar(pGlobalTXT_LocalizationStrings[199], 0);//"Select the Item to Sell"
      if ( !HouseUI_CheckIfPlayerCanInteract())
        return;
      pMouse->GetCursorPos(&mouse);
      v153 = ((mouse.x - 14) >> 5) + 14 * ((mouse.y - 17) >> 5);
      if ( mouse.x <= 13 || mouse.x >= 462 )
        return;
      if ( !pPlayers[uActiveCharacter]->GetItemIDAtInventoryIndex((int *)&v153) )
           return;
      selected_item = &pPlayers[uActiveCharacter]->pInventoryItemList[pPlayers[uActiveCharacter]->GetItemIDAtInventoryIndex((int *)&v153) - 1];
      pText = BuildDialogueString(pMerchantsSellPhrases[pPlayers[uActiveCharacter]->SelectPhrasesTransaction(selected_item, BuildingType_ArmorShop, window_SpeakInHouse->par1C, 3)],
              uActiveCharacter - 1, selected_item, (char *) window_SpeakInHouse->par1C, 3, 0);
      dialog_window.DrawTitleText(pFontArrus, 0, (174 - pFontArrus->CalcTextHeight(pText, &dialog_window, 0, 0)) / 2 + 138, TargetColor(0xFFu, 0xFFu, 0xFFu), pText, 3);
      return;
    }
    break;
    case HOUSE_DIALOGUE_SHOP_IDENTIFY:
    {
      draw_leather();
      CharacterUI_InventoryTab_Draw(pPlayers[uActiveCharacter], true);
      DrawTextAtStatusBar(pGlobalTXT_LocalizationStrings[197], 0); //"Select the Item to Identify"	
      if ( HouseUI_CheckIfPlayerCanInteract() )
      {
        pMouse->GetCursorPos(&mouse);
        v153 = ((mouse.x - 14) >> 5) + 14 * ((mouse.y - 17) >> 5);
        if ( mouse.x > 13 && mouse.x < 462 )
        {
          if ( pPlayers[uActiveCharacter]->GetItemIDAtInventoryIndex((int *)&v153) )
          {
            selected_item = &pPlayers[uActiveCharacter]->pInventoryItemList[pPlayers[uActiveCharacter]->GetItemIDAtInventoryIndex((int *)&v153)-1];
            if (selected_item->IsIdentified())
              pText = BuildDialogueString("%24", uActiveCharacter - 1, selected_item, (char *)window_SpeakInHouse->ptr_1C, 4, 0);
            else
              pText = BuildDialogueString(pMerchantsIdentifyPhrases[pPlayers[uActiveCharacter]->SelectPhrasesTransaction(selected_item, BuildingType_ArmorShop,
                    (int)window_SpeakInHouse->ptr_1C, 4)], uActiveCharacter - 1, selected_item, (char *)window_SpeakInHouse->ptr_1C, 4, 0);
            dialog_window.DrawTitleText(pFontArrus, 0, (174 - pFontArrus->CalcTextHeight(pText, &dialog_window, 0, 0)) / 2 + 138, TargetColor(0xFFu, 0xFFu, 0xFFu), pText, 3);
            return;
          }
        }
      }
    }
    break;
    case HOUSE_DIALOGUE_SHOP_REPAIR:
    {
      draw_leather();
      CharacterUI_InventoryTab_Draw(pPlayers[uActiveCharacter], true);
      DrawTextAtStatusBar(pGlobalTXT_LocalizationStrings[198], 0);
      if ( !HouseUI_CheckIfPlayerCanInteract())
        return;
      pMouse->GetCursorPos(&mouse);
      v153 = ((mouse.x -14) >> 5) + 14 * ((mouse.y - 17) >> 5);
      if( mouse.x <= 13 || mouse.x >= 462 )
        return;
      if ( !pPlayers[uActiveCharacter]->GetItemIDAtInventoryIndex((int *)&v153)
       || (!(pPlayers[uActiveCharacter]->pOwnItems[pPlayers[uActiveCharacter]->GetItemIDAtInventoryIndex((int *)&v153) - 1].uAttributes& 2)) )
        return;
      selected_item = &pPlayers[uActiveCharacter]->pInventoryItemList[pPlayers[uActiveCharacter]->GetItemIDAtInventoryIndex((int *)&v153) - 1];
      phrases_id = pPlayers[uActiveCharacter]->SelectPhrasesTransaction(&pPlayers[uActiveCharacter]->pInventoryItemList[pPlayers[uActiveCharacter]->GetItemIDAtInventoryIndex((int *)&v153) - 1],
             BuildingType_ArmorShop, window_SpeakInHouse->par1C, 5);
      pText = BuildDialogueString(pMerchantsRepairPhrases[phrases_id], uActiveCharacter - 1, selected_item, (char *)window_SpeakInHouse->ptr_1C, 5, 0);
      dialog_window.DrawTitleText(pFontArrus, 0, (174 - pFontArrus->CalcTextHeight(pText, &dialog_window, 0, 0)) / 2 + 138, TargetColor(0xFFu, 0xFFu, 0xFFu), pText, 3);
      return;
    }
    break;

    case HOUSE_DIALOGUE_LEARN_SKILLS:
    {
      if (!HouseUI_CheckIfPlayerCanInteract() )
        return;
      uint item_num = 0;
      v38 = (signed __int64)(p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].flt_24 * 500.0);
      pPrice = v38 * (100 - pPlayers[uActiveCharacter]->GetMerchant()) / 100;
      if ( (signed int)pPrice < v38 / 3 )
        pPrice = v38 / 3;
      all_text_height = 0;
      for ( int i = pDialogueWindow->pStartingPosActiveItem;
                  i < pDialogueWindow->pNumPresenceButton + pDialogueWindow->pStartingPosActiveItem; ++i )
      {
        if ( byte_4ED970_skill_learn_ability_by_class_table[pPlayers[uActiveCharacter]->classType][pDialogueWindow->GetControl(i)->msg_param - 36]
             && !pPlayers[uActiveCharacter]->pActiveSkills[pDialogueWindow->GetControl(i)->msg_param - 36] )
        {
          all_text_height += pFontArrus->CalcTextHeight(pSkillNames[pDialogueWindow->GetControl(i)->msg_param - 36], &dialog_window, 0, 0);
          item_num ++;
        }
      }
      if ( item_num )
      {
        sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[401], pPrice);
        dialog_window.DrawTitleText(pFontArrus, 0, 0x92u, 0, pTmpBuf.data(), 3);
        v146 = (149 - all_text_height) / item_num;
        if ( (149 - all_text_height) / item_num > 32 )
          v146 = 32;
        v23 = (149 - v146 * item_num - all_text_height) / 2 - v146 / 2 + 162;
        for ( int i = pDialogueWindow->pStartingPosActiveItem;
                  i < pDialogueWindow->pNumPresenceButton + pDialogueWindow->pStartingPosActiveItem; ++i )
        {
          pButton = pDialogueWindow->GetControl(i);
          if ( !byte_4ED970_skill_learn_ability_by_class_table[pPlayers[uActiveCharacter]->classType][pButton->msg_param - 36]
            || pPlayers[uActiveCharacter]->pActiveSkills[pButton->msg_param - 36] )
          {
            pButton->uW = 0;
            pButton->uHeight = 0;
            pButton->uY = 0;
          }
          else
          {
            pButton->uY = v146 + v23;
            pButton->uHeight = pFontArrus->CalcTextHeight(pSkillNames[pButton->msg_param - 36], &dialog_window, 0, 0);
            v23 = pButton->uY + pFontArrus->CalcTextHeight(pSkillNames[pButton->msg_param - 36], &dialog_window, 0, 0) - 1;
            pButton->uW = v23;
            pTextColor = TargetColor(0xE1u, 0xCDu, 0x23u);
            if ( pDialogueWindow->pCurrentPosActiveItem != i )
              pTextColor = TargetColor(0xFFu, 0xFFu, 0xFFu);
            dialog_window.DrawTitleText(pFontArrus, 0, pButton->uY, pTextColor, pSkillNames[pButton->msg_param - 36], 3);
          }
        }
        return;
      }
      sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[544], pPlayers[uActiveCharacter]->pName, pClassNames[pPlayers[uActiveCharacter]->classType]); //"Seek knowledge elsewhere %s the %s"
      strcat(pTmpBuf.data(), "\n \n");
      strcat(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[528]); //"I can offer you nothing further."
      dialog_window.DrawTitleText(pFontArrus, 0, (174 - pFontArrus->CalcTextHeight(pTmpBuf.data(), &dialog_window, 0, 0)) / 2 + 138, TargetColor(0xE1u, 0xCDu, 0x23u), pTmpBuf.data(), 3);
      return;
    }
    break;
    default:
      __debugbreak();
  }
}
//----- (004B9CC6) --------------------------------------------------------
void  AlchemistDialog()
{
  int index; // eax@7
  signed int v5; // esi@9
  int v18; // eax@23
  const char *pText; // edx@29
  unsigned int product_height_1row; // edi@55
  unsigned int product_width_1row; // esi@57
  unsigned int product_height_2row; // edi@67
  unsigned int product_width_2row; // esi@69
  ItemGen *item; // esi@118
  int v71; // eax@123
  int all_text_height; // edi@125
  GUIButton *pButton; // esi@129
  unsigned int pColorText; // ax@129
  POINT mouse; // [sp+2Ch] [bp-ACh]@117
  GUIWindow dialog_window; // [sp+5Ch] [bp-7Ch]@1
  int v105; // [sp+B0h] [bp-28h]@19
  int pNumString; // [sp+C8h] [bp-10h]@9
  int item_num; // [sp+D4h] [bp-4h]@11
  int pPrice;

  memcpy(&dialog_window, window_SpeakInHouse, sizeof(dialog_window));
  dialog_window.uFrameX = 483;
  dialog_window.uFrameWidth = 148;
  dialog_window.uFrameZ = 334;
  switch(dialog_menu_id)
  {
    case HOUSE_DIALOGUE_MAIN:
    {
      if ( HouseUI_CheckIfPlayerCanInteract() )
      {
        pShopOptions[0] = const_cast<char *>(pGlobalTXT_LocalizationStrings[134]);
        pShopOptions[1] = const_cast<char *>(pGlobalTXT_LocalizationStrings[152]);
        pShopOptions[2] = const_cast<char *>(pGlobalTXT_LocalizationStrings[159]);
        pShopOptions[3] = const_cast<char *>(pGlobalTXT_LocalizationStrings[160]);
        all_text_height = 0;
        for ( int i = 0; i < 4; ++i )
          all_text_height += pFontArrus->CalcTextHeight(pShopOptions[i], &dialog_window, 0, 0);
        v18 = (174 - all_text_height) / 4;
        v105 = (174 - 4 * (174 - all_text_height) / 4 - all_text_height) / 2 - (174 - all_text_height) / 4 / 2 + 138;
        pNumString = 0;
        for ( int i = pDialogueWindow->pStartingPosActiveItem;
                  i < pDialogueWindow->pNumPresenceButton + pDialogueWindow->pStartingPosActiveItem; ++i )
        {
          pButton = pDialogueWindow->GetControl(i);
          pButton->uY = v18 + v105;
          pButton->uHeight = pFontArrus->CalcTextHeight((const char *)pShopOptions[pNumString], &dialog_window, 0, 0);
          v105 = pButton->uY + pFontArrus->CalcTextHeight((const char *)pShopOptions[pNumString], &dialog_window, 0, 0) - 1;
          pButton->uW = v105;
          pColorText = TargetColor(0xE1u, 0xCDu, 0x23u);
          if ( pDialogueWindow->pCurrentPosActiveItem != i )
            pColorText = TargetColor(0xFFu, 0xFFu, 0xFFu);
          dialog_window.DrawTitleText(pFontArrus, 0, pButton->uY, pColorText, (const char *)pShopOptions[pNumString], 3);
          ++pNumString;
        }
      }
      return;
    }

    case HOUSE_DIALOGUE_SHOP_BUY_STANDARD:
    {
      pRenderer->DrawTextureIndexed(8, 8, ShopTexture);
      for ( uint i = 0; i < 6; ++i )
      {
        if ( pParty->StandartItemsInShops[(unsigned int)window_SpeakInHouse->ptr_1C][i].uItemID )
        {
          product_height_1row = 152 - ItemsInShopTexture[i]->uTextureHeight;
          if ( (signed int)product_height_1row < 1 )
            product_height_1row = 0;
          product_width_1row = 75 * i - ItemsInShopTexture[i]->uTextureWidth / 2 + 40;
          if ( i )
          {
            if ( i == 5 )
            {
              if ( (signed int)product_width_1row > 457 - ItemsInShopTexture[5]->uTextureWidth )
                product_width_1row = 457 - ItemsInShopTexture[5]->uTextureWidth;
            }
          }
          else if ( (signed int)product_width_1row < 18 )
            product_width_1row = 18;
          pRenderer->DrawTextureTransparent(product_width_1row, product_height_1row, ItemsInShopTexture[i]);
          ZBuffer_DoFill2(&pRenderer->pActiveZBuffer[product_width_1row + 640 * product_height_1row], ItemsInShopTexture[i], i + 1);
        }
      }
      for ( uint i = 0; i < 6; ++i )
      {
        if ( pParty->StandartItemsInShops[(unsigned int)window_SpeakInHouse->ptr_1C][i + 6].uItemID )
        {
          product_height_2row = 308 - ItemsInShopTexture[i + 6]->uTextureHeight;
          if ( (signed int)product_height_2row < 1 )
            product_height_2row = 0;
          product_width_2row = 75 * i - ItemsInShopTexture[i + 6]->uTextureWidth / 2 + 40;
          if ( i )
          {
            if ( i == 5 )
            {
              if ( (signed int)product_width_2row > 457 - ItemsInShopTexture[11]->uTextureWidth )
                product_width_2row = 457 - ItemsInShopTexture[11]->uTextureWidth;
            }
          }
          else
          {
            if ( (signed int)product_width_2row < 18 )
              product_width_2row = 18;
          }
          pRenderer->DrawTextureTransparent(product_width_2row, product_height_2row, ItemsInShopTexture[i + 6]);
          ZBuffer_DoFill2(&pRenderer->pActiveZBuffer[product_width_2row + 640 * product_height_2row], ItemsInShopTexture[i + 6], i + 7);
        }
      }
      if ( HouseUI_CheckIfPlayerCanInteract() )
      {
        item_num = 0;
        for ( uint i = 0; i < 12; ++i )
        {
          if ( pParty->StandartItemsInShops[(unsigned int)window_SpeakInHouse->ptr_1C][i].uItemID)
            ++item_num;
        }
        if ( GetAsyncKeyState(17) && pPlayers[uActiveCharacter]->CanSteal() )
          DrawTextAtStatusBar(pGlobalTXT_LocalizationStrings[185], 0);
        else
          DrawTextAtStatusBar(pGlobalTXT_LocalizationStrings[195], 0);
        if ( !item_num )
        {
          dialog_window.DrawShops_next_generation_time_string(pParty->PartyTimes.Shops_next_generation_time[ (unsigned int)window_SpeakInHouse->ptr_1C] - pParty->uTimePlayed);
          return;
        }
        pMouse->GetCursorPos(&mouse);
        if ( pRenderer->pActiveZBuffer[mouse.x + pSRZBufferLineOffsets[mouse.y]] & 0xFFFF )
        {
          item = &pParty->StandartItemsInShops[(int)window_SpeakInHouse->ptr_1C][(pRenderer->pActiveZBuffer[mouse.x + pSRZBufferLineOffsets[mouse.y]] & 0xFFFF) - 1];
          if ( !GetAsyncKeyState(17) || !pPlayers[uActiveCharacter]->CanSteal() )
          {
            v71 = pPlayers[uActiveCharacter]->SelectPhrasesTransaction(item, BuildingType_AlchemistShop, (int)window_SpeakInHouse->ptr_1C, 2);
            pText = BuildDialogueString(pMerchantsBuyPhrases[v71], uActiveCharacter - 1, item, (char *)window_SpeakInHouse->ptr_1C, 2, 0);
          }
          else
            pText = BuildDialogueString(pGlobalTXT_LocalizationStrings[181], uActiveCharacter - 1, item, (char *)window_SpeakInHouse->ptr_1C, 2, 0);
          dialog_window.DrawTitleText(pFontArrus, 0, (174 - pFontArrus->CalcTextHeight(pText, &dialog_window, 0, 0)) / 2 + 138, TargetColor(0xFFu, 0xFFu, 0xFFu), pText, 3);
          return ;
        }
      }
      return;
    }

    case HOUSE_DIALOGUE_SHOP_BUY_SPECIAL:
    {
      pRenderer->DrawTextureIndexed(8, 8, ShopTexture);
      for ( uint i = 0; i < 6; ++i )
      {
        if (pParty->SpecialItemsInShops[(unsigned int)window_SpeakInHouse->ptr_1C][i].uItemID)
        {
          product_height_1row = 152 - ItemsInShopTexture[i]->uTextureHeight;
          if ( (signed int)product_height_1row < 1 )
            product_height_1row = 0;
          product_width_1row = 75 * i - ItemsInShopTexture[i]->uTextureWidth / 2 + 40;
          if ( i )
          {
            if ( i == 5 )
            {
              if ( (signed int)product_width_1row > 457 - ItemsInShopTexture[5]->uTextureWidth )
                product_width_1row = 457 - ItemsInShopTexture[5]->uTextureWidth;
            }
          }
          else
          {
            if ( (signed int)product_width_1row < 18 )
              product_width_1row = 18;
          }
          pRenderer->DrawTextureTransparent(product_width_1row, product_height_1row, ItemsInShopTexture[i]);
          ZBuffer_DoFill2(&pRenderer->pActiveZBuffer[product_width_1row + 640 * product_height_1row], ItemsInShopTexture[i], i + 1);
        }
      }
      for ( uint i = 0; i < 6; ++i )
      {
        if (pParty->SpecialItemsInShops[(unsigned int)window_SpeakInHouse->ptr_1C][(signed int)i + 6].uItemID) //not itemid
        {
          product_height_2row = 308 - ItemsInShopTexture[i + 6]->uTextureHeight;
          if ( (signed int)product_height_2row < 1 )
            product_height_2row = 0;
          product_width_2row = 75 * i - ItemsInShopTexture[i + 6]->uTextureWidth / 2 + 40;
          if ( i )
          {
            if ( i == 5 )
            {
              if ( (signed int)product_width_2row > 457 - ItemsInShopTexture[11]->uTextureWidth )
                product_width_2row = 457 - ItemsInShopTexture[11]->uTextureWidth;
            }
          }
          else
          {
            if ( (signed int)product_width_2row < 18 )
              product_width_2row = 18;
          }
          pRenderer->DrawTextureTransparent(product_width_2row, product_height_2row, ItemsInShopTexture[i + 6]);
          ZBuffer_DoFill2(&pRenderer->pActiveZBuffer[product_width_2row + 640 * product_height_2row], ItemsInShopTexture[i + 6], i + 7);
        }
      }
      if ( HouseUI_CheckIfPlayerCanInteract() )
      {
        item_num = 0;
        for ( uint i = 0; i < 12; ++i )
        {
          if (pParty->SpecialItemsInShops[(unsigned int)window_SpeakInHouse->ptr_1C][i].uItemID)
            ++item_num;
        }
        if ( GetAsyncKeyState(17) && pPlayers[uActiveCharacter]->CanSteal() )
          DrawTextAtStatusBar(pGlobalTXT_LocalizationStrings[185], 0);
        else
          DrawTextAtStatusBar(pGlobalTXT_LocalizationStrings[196], 0);
        if ( !item_num )
        {
          dialog_window.DrawShops_next_generation_time_string(pParty->PartyTimes.Shops_next_generation_time[ (unsigned int)window_SpeakInHouse->ptr_1C] - pParty->uTimePlayed);
          return;
        }
        pMouse->GetCursorPos(&mouse);
        if ( pRenderer->pActiveZBuffer[mouse.x + pSRZBufferLineOffsets[mouse.y]] & 0xFFFF )
        {
          item = &pParty->SpecialItemsInShops[(int)window_SpeakInHouse->ptr_1C][(pRenderer->pActiveZBuffer[mouse.x + pSRZBufferLineOffsets[mouse.y]] & 0xFFFF) - 1];
          if ( !GetAsyncKeyState(17) || !pPlayers[uActiveCharacter]->CanSteal() )
          {
            v71 = pPlayers[uActiveCharacter]->SelectPhrasesTransaction(item, BuildingType_AlchemistShop, (int)window_SpeakInHouse->ptr_1C, 2);
            pText = BuildDialogueString(pMerchantsBuyPhrases[v71], uActiveCharacter - 1, item, (char *)window_SpeakInHouse->ptr_1C, 2, 0);
          }
          else
            pText = BuildDialogueString(pGlobalTXT_LocalizationStrings[181], uActiveCharacter - 1, item, (char *)window_SpeakInHouse->ptr_1C, 2, 0);
          dialog_window.DrawTitleText(pFontArrus, 0, (174 - pFontArrus->CalcTextHeight(pText, &dialog_window, 0, 0)) / 2 + 138, TargetColor(0xFFu, 0xFFu, 0xFFu), pText, 3);
          return;
        }
      }
      return;
    }

    case HOUSE_DIALOGUE_SHOP_SELL:
    {
      draw_leather();
      CharacterUI_InventoryTab_Draw(pPlayers[uActiveCharacter], true);
      DrawTextAtStatusBar(pGlobalTXT_LocalizationStrings[199], 0);
      if ( !HouseUI_CheckIfPlayerCanInteract() )
        return;
      pMouse->GetCursorPos(&mouse);
      index = ((mouse.x - 14) >> 5) + 14 * ((mouse.y - 17) >> 5);
      if ( mouse.x <= 13 || mouse.x >= 462 )
        return;
      if ( !pPlayers[uActiveCharacter]->GetItemIDAtInventoryIndex((int *)&index) )
          return;
      v71 = pPlayers[uActiveCharacter]->SelectPhrasesTransaction(&pPlayers[uActiveCharacter]->pInventoryItemList[pPlayers[uActiveCharacter]->GetItemIDAtInventoryIndex((int *)&index) - 1],
             BuildingType_AlchemistShop, (int)window_SpeakInHouse->ptr_1C, 3);
      pText = BuildDialogueString(pMerchantsSellPhrases[v71], uActiveCharacter - 1, &pPlayers[uActiveCharacter]->pInventoryItemList[pPlayers[uActiveCharacter]->GetItemIDAtInventoryIndex((int *)&index) - 1],
             (char *)window_SpeakInHouse->ptr_1C, 3, 0);
      dialog_window.DrawTitleText(pFontArrus, 0, (174 - pFontArrus->CalcTextHeight(pText, &dialog_window, 0, 0)) / 2 + 138, TargetColor(0xFFu, 0xFFu, 0xFFu), pText, 3);
      return; 
    }

    case HOUSE_DIALOGUE_SHOP_IDENTIFY:
    {
      draw_leather();
      CharacterUI_InventoryTab_Draw(pPlayers[uActiveCharacter], true);
      DrawTextAtStatusBar(pGlobalTXT_LocalizationStrings[197], 0);
      if ( HouseUI_CheckIfPlayerCanInteract() )
        return;
      pMouse->GetCursorPos(&mouse);
      index = ((mouse.x - 14) >> 5) + 14 * ((mouse.y - 17) >> 5);
      if ( mouse.x <= 13 || mouse.x >= 462 )
        return;
      if ( !pPlayers[uActiveCharacter]->GetItemIDAtInventoryIndex((int *)&index) )
        return;
      item = &pPlayers[uActiveCharacter]->pInventoryItemList[pPlayers[uActiveCharacter]->GetItemIDAtInventoryIndex((int *)&index) - 1];
      if (!item->IsIdentified())
      {
        v71 = pPlayers[uActiveCharacter]->SelectPhrasesTransaction(item, BuildingType_AlchemistShop, (int)window_SpeakInHouse->ptr_1C, 4);
        pText = BuildDialogueString(pMerchantsIdentifyPhrases[v71], uActiveCharacter - 1, item, (char *)window_SpeakInHouse->ptr_1C, 4, 0);
      }
      else
        pText = BuildDialogueString("%24", uActiveCharacter - 1, item, (char *)window_SpeakInHouse->ptr_1C, 4, 0);
      dialog_window.DrawTitleText(pFontArrus, 0, (174 - pFontArrus->CalcTextHeight(pText, &dialog_window, 0, 0)) / 2 + 138, TargetColor(0xFFu, 0xFFu, 0xFFu), pText, 3);
      return;
    }

    case HOUSE_DIALOGUE_SHOP_DISPLAY_EQUIPMENT:
    {
      draw_leather();
      CharacterUI_InventoryTab_Draw(pPlayers[uActiveCharacter], true);
      pShopOptions[0] = const_cast<char *>(pGlobalTXT_LocalizationStrings[200]);
      pShopOptions[1] = const_cast<char *>(pGlobalTXT_LocalizationStrings[113]);
      all_text_height = 0;
      for ( int i = 0; i < 2; ++i )
        all_text_height += pFontArrus->CalcTextHeight(pShopOptions[i], &dialog_window, 0, 0);
      v18 = (174 - all_text_height) / 2;
      v105 = (2 * (87 - (174 - all_text_height) / 2) - all_text_height) / 2 - (174 - all_text_height) / 2 / 2 + 138;
      pNumString = 0;
      for ( int i = pDialogueWindow->pStartingPosActiveItem;
                i < pDialogueWindow->pNumPresenceButton + pDialogueWindow->pStartingPosActiveItem; ++i )
      {
        pButton = pDialogueWindow->GetControl(i);
        pButton->uY = v18 + v105;
        pButton->uHeight = pFontArrus->CalcTextHeight((const char *)pShopOptions[pNumString], &dialog_window, 0, 0);
        v105 = pButton->uY + pFontArrus->CalcTextHeight((const char *)pShopOptions[pNumString], &dialog_window, 0, 0) - 1;
        pButton->uW = v105;
        pColorText = TargetColor(0xE1u, 0xCDu, 0x23u);
        if ( pDialogueWindow->pCurrentPosActiveItem != i )
          pColorText = TargetColor(0xFFu, 0xFFu, 0xFFu);
        dialog_window.DrawTitleText(pFontArrus, 0, pButton->uY, pColorText, (const char *)pShopOptions[pNumString], 3);
        ++pNumString;
      }
      return;
    }

    case HOUSE_DIALOGUE_LEARN_SKILLS:
    {
      if (!HouseUI_CheckIfPlayerCanInteract())
        return;
      all_text_height = 0;
      v5 = (signed __int64)(p2DEvents[(signed int)window_SpeakInHouse->ptr_1C - 1].flt_24 * 500.0);
      pPrice = v5 * (100 - pPlayers[uActiveCharacter]->GetMerchant()) / 100;
      if ( pPrice < v5 / 3 )
        pPrice = v5 / 3;
      item_num = 0;
      for ( int i = pDialogueWindow->pStartingPosActiveItem;
                i < pDialogueWindow->pNumPresenceButton + pDialogueWindow->pStartingPosActiveItem; ++i )
      {
        pButton = pDialogueWindow->GetControl(i);
        if ( byte_4ED970_skill_learn_ability_by_class_table[pPlayers[uActiveCharacter]->classType][pButton->msg_param - 36]
         && !pPlayers[uActiveCharacter]->pActiveSkills[pButton->msg_param - 36] )
        {
          all_text_height = pFontArrus->CalcTextHeight(pSkillNames[pButton->msg_param - 36], &dialog_window, 0, 0);
          ++item_num;
        }
      }
      if ( !item_num )
      {
        sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[544], pPlayers[uActiveCharacter]->pName, pClassNames[pPlayers[uActiveCharacter]->classType]);
        strcat(pTmpBuf.data(), "\n \n");
        strcat(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[528]);
        dialog_window.DrawTitleText(pFontArrus, 0, (174 - pFontArrus->CalcTextHeight(pTmpBuf.data(), &dialog_window, 0, 0)) / 2 + 138, TargetColor(0xE1u, 0xCDu, 0x23u), pTmpBuf.data(), 3);
        return;
      }
      if ( item_num )
      {
        sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[401], pPrice);
        dialog_window.DrawTitleText(pFontArrus, 0, 0x92u, 0, pTmpBuf.data(), 3);
        v18 = (149 - all_text_height) / item_num;
        if ( (149 - all_text_height) / item_num > 32 )
          v18 = 32;
        v105 = (149 - item_num * v18 - all_text_height) / 2 - v18 / 2 + 162;
        for ( int i = pDialogueWindow->pStartingPosActiveItem;
                  i < pDialogueWindow->pNumPresenceButton + pDialogueWindow->pStartingPosActiveItem; i++ )
        {
          pButton = pDialogueWindow->GetControl(i);
          if ( !byte_4ED970_skill_learn_ability_by_class_table[pPlayers[uActiveCharacter]->classType][pButton->msg_param - 36]
             || pPlayers[uActiveCharacter]->pActiveSkills[pButton->msg_param - 36] )
          {
            pButton->uW = 0;
            pButton->uHeight = 0;
            pButton->uY = 0;
          }
          else
          {
            pButton->uY = v18 + v105;
            pButton->uHeight = pFontArrus->CalcTextHeight(pSkillNames[pButton->msg_param - 36], &dialog_window, 0, 0);
            v105 = pButton->uY + pFontArrus->CalcTextHeight(pSkillNames[pButton->msg_param - 36], &dialog_window, 0, 0) - 1;
            pButton->uW = v105;
            pColorText = TargetColor(0xE1u, 0xCDu, 0x23u);
            if ( pDialogueWindow->pCurrentPosActiveItem != i )
              pColorText = TargetColor(0xFFu, 0xFFu, 0xFFu);
            dialog_window.DrawTitleText(pFontArrus, 0, pButton->uY, pColorText, pSkillNames[pButton->msg_param - 36], 3);
          }
        }
        return;
      }
    }
    default:
      __debugbreak();
  }
}

//----- (004B4FCF) --------------------------------------------------------
void MagicShopDialog()
{
  int all_text_height; // edi@21
  GUIButton *control_button; // esi@25
  int v23; // eax@25
  unsigned __int16 text_color; // ax@25
  char *v30; // edx@35
  signed int v33; // esi@40
  unsigned int product_height_1row; // edi@64
  unsigned int product_width_1row; // esi@66
  unsigned int product_height_2row; // edi@76
  unsigned int product_width_2row; // esi@76
  int v75; // eax@130
  POINT mouse; // [sp+6Ch] [bp-8Ch]@30
  GUIWindow dialog_window; // [sp+7Ch] [bp-7Ch]@1
  int item_num; // [sp+E0h] [bp-18h]@8
  int v117; // [sp+E4h] [bp-14h]@40
  int pPrice; // [sp+E8h] [bp-10h]@24
  int one_string; // [sp+ECh] [bp-Ch]@1
  int pSrtingNum;
  ItemGen *item;

  memcpy(&dialog_window, window_SpeakInHouse, sizeof(dialog_window));
  dialog_window.uFrameX = 483;
  dialog_window.uFrameWidth = 148;
  dialog_window.uFrameZ = 334;
  if ( dialog_menu_id == HOUSE_DIALOGUE_MAIN)
  {
    if ( !HouseUI_CheckIfPlayerCanInteract() )
        return;
    pShopOptions[0] = const_cast<char *>(pGlobalTXT_LocalizationStrings[134]); //"Buy Standard"
    pShopOptions[1] = const_cast<char *>(pGlobalTXT_LocalizationStrings[152]); //"Buy Special"
    pShopOptions[2] = const_cast<char *>(pGlobalTXT_LocalizationStrings[159]); //"Display Inventory"
    pShopOptions[3] = const_cast<char *>(pGlobalTXT_LocalizationStrings[160]);
    all_text_height = 0;
    for ( int i = 0; i < 4; ++i )
      all_text_height += pFontArrus->CalcTextHeight(pShopOptions[i], &dialog_window, 0, 0);
    one_string = (174 - all_text_height) / 4;
    v23 = (174 - 4 * one_string - all_text_height) / 2 - one_string / 2 + 138;
    int pNumString = 0;
    for (int i = pDialogueWindow->pStartingPosActiveItem;
             i < pDialogueWindow->pNumPresenceButton + pDialogueWindow->pStartingPosActiveItem;  ++i)
    {
      control_button = pDialogueWindow->GetControl(i);
      control_button->uY = one_string + v23;
      control_button->uHeight = pFontArrus->CalcTextHeight(pShopOptions[pNumString], &dialog_window, 0, 0);
      v23 = control_button->uY + control_button->uHeight - 1;
      control_button->uW = v23;
      text_color = TargetColor(225, 205, 35);
      if ( pDialogueWindow->pCurrentPosActiveItem != i )
        text_color = TargetColor(255, 255, 255);
      dialog_window.DrawTitleText(pFontArrus, 0, control_button->uY, text_color, pShopOptions[pNumString], 3);
      ++pNumString;
    }
    return;
  }

  if ( dialog_menu_id == HOUSE_DIALOGUE_SHOP_BUY_STANDARD)
  {
    pRenderer->DrawTextureIndexed(8, 8, ShopTexture);
    for ( uint i = 0; i < 6; ++i )
    {
      if ( pParty->StandartItemsInShops[(unsigned int)window_SpeakInHouse->ptr_1C][i].uItemID)
      {
        product_height_1row = 152 - ItemsInShopTexture[i]->uTextureHeight;
        if ( (signed int)product_height_1row < 1 )
          product_height_1row = 0;
        product_width_1row = 75 * i - ItemsInShopTexture[i]->uTextureWidth / 2 + 40;
        if ( i )
        {
          if ( i == 5 )
          {
            if ( (signed int)product_width_1row > 457 - ItemsInShopTexture[5]->uTextureWidth )
              product_width_1row = 457 - ItemsInShopTexture[5]->uTextureWidth;
          }
        }
        else
        {
          if ( (signed int)product_width_1row < 18 )
            product_width_1row = 18;
        }
        pRenderer->DrawTextureTransparent(product_width_1row, product_height_1row, ItemsInShopTexture[i]);
        ZBuffer_DoFill2(&pRenderer->pActiveZBuffer[product_width_1row + 640 * product_height_1row], ItemsInShopTexture[i], i + 1);
      }
    }
    for ( uint i = 0; i < 6; ++i )
    {
      if ( pParty->StandartItemsInShops[(unsigned int)window_SpeakInHouse->ptr_1C][i + 6].uItemID)
      {
        product_height_2row = 306 - ItemsInShopTexture[i + 6]->uTextureHeight;
        product_width_2row = 75 * i - ItemsInShopTexture[i + 6]->uTextureWidth / 2 + 40;
        if ( i )
        {
          if ( i == 5 )
          {
            if ( (signed int)product_width_2row > 457 - ItemsInShopTexture[11]->uTextureWidth )
              product_width_2row = 457 - ItemsInShopTexture[11]->uTextureWidth;
          }
        }
        else
        {
          if ( (signed int)product_width_2row < 18 )
            product_width_2row = 18;
        }
        pRenderer->DrawTextureTransparent(product_width_2row, product_height_2row, ItemsInShopTexture[i + 6]);
        ZBuffer_DoFill2(&pRenderer->pActiveZBuffer[product_width_2row + 640 * product_height_2row], ItemsInShopTexture[i + 6], i + 7);
      }
    }
    if ( HouseUI_CheckIfPlayerCanInteract() )
    {
      item_num = 0;
      for ( uint i = 0; i < 12; ++i )
      {
        if ( pParty->StandartItemsInShops[(unsigned int)window_SpeakInHouse->ptr_1C][i].uItemID )
          ++item_num;
      }
      if ( GetAsyncKeyState(VK_CONTROL) && pPlayers[uActiveCharacter]->CanSteal() )
        DrawTextAtStatusBar(pGlobalTXT_LocalizationStrings[185], 0);// "Steal item"
      else
        DrawTextAtStatusBar(pGlobalTXT_LocalizationStrings[195], 0);// "Select the Item to Buy"
      if ( !item_num )
      {
        dialog_window.DrawShops_next_generation_time_string(pParty->PartyTimes.Shops_next_generation_time[(unsigned int)window_SpeakInHouse->ptr_1C] - pParty->uTimePlayed);
        return;
      }
      pMouse->GetCursorPos(&mouse);
      if ( pRenderer->pActiveZBuffer[mouse.x + pSRZBufferLineOffsets[mouse.y]] & 0xFFFF )
      {
        item = &pParty->StandartItemsInShops[(int)window_SpeakInHouse->ptr_1C][(pRenderer->pActiveZBuffer[mouse.x + pSRZBufferLineOffsets[mouse.y]] & 0xFFFF) - 1];
        if ( GetAsyncKeyState(VK_CONTROL) && pPlayers[uActiveCharacter]->CanSteal() )
          v30 = BuildDialogueString(pGlobalTXT_LocalizationStrings[181], uActiveCharacter - 1, item, (char *)window_SpeakInHouse->ptr_1C, 2, 0);// "Steal %24"
        else
        {
          v75 = pPlayers[uActiveCharacter]->SelectPhrasesTransaction(item, BuildingType_MagicShop, (int)window_SpeakInHouse->ptr_1C, 2);
          v30 = BuildDialogueString(pMerchantsBuyPhrases[v75], uActiveCharacter - 1, item, (char *)window_SpeakInHouse->ptr_1C, 2, 0);
        }
        dialog_window.DrawTitleText(pFontArrus, 0, (174 - pFontArrus->CalcTextHeight(v30, &dialog_window, 0, 0)) / 2 + 138, TargetColor(255, 255, 255), v30, 3);
        return;
      }
    }
    return;
  }

  if ( dialog_menu_id == HOUSE_DIALOGUE_SHOP_BUY_SPECIAL)
  {
    pRenderer->DrawTextureIndexed(8, 8, ShopTexture);
    for ( uint i = 0; i < 6; ++i )
    {
      if ( pParty->SpecialItemsInShops[(unsigned int)window_SpeakInHouse->ptr_1C][i].uItemID )
      {
        product_height_1row = 152 - ItemsInShopTexture[i]->uTextureHeight;
        if ( (signed int)product_height_1row < 1 )
          product_height_1row = 0;
        product_width_1row = 75 * i - ItemsInShopTexture[i]->uTextureWidth / 2 + 40;
        if ( i )
        {
          if ( i == 5 )
          {
            if ( (signed int)product_width_1row > 457 - ItemsInShopTexture[5]->uTextureWidth )
              product_width_1row = 457 - ItemsInShopTexture[5]->uTextureWidth;
          }
        }
        else
        {
          if ( (signed int)product_width_1row < 18 )
            product_width_1row = 18;
        }
        pRenderer->DrawTextureTransparent(product_width_1row, product_height_1row, ItemsInShopTexture[i]);
        ZBuffer_DoFill2(&pRenderer->pActiveZBuffer[product_width_1row + 640 * product_height_1row], ItemsInShopTexture[i], i + 1);
      }
    }
    for ( uint i = 0; i < 6; ++i )
    {
      if (pParty->SpecialItemsInShops[(unsigned int)window_SpeakInHouse->ptr_1C][i + 6].uItemID)
      {
        product_height_2row = 306 - ItemsInShopTexture[i + 6]->uTextureHeight;
        if ( (signed int)product_height_2row < 1 )
          product_height_2row = 0;
        product_width_2row = 75 * i - ItemsInShopTexture[i + 6]->uTextureWidth / 2 + 40;
        if ( i )
        {
          if ( i == 5 )
          {
            if ( (signed int)product_width_2row > 457 - ItemsInShopTexture[11]->uTextureWidth )
              product_width_2row = 457 - ItemsInShopTexture[11]->uTextureWidth;
          }
        }
        else
        {
          if ( (signed int)product_width_2row < 18 )
            product_width_2row = 18;
        }
        pRenderer->DrawTextureTransparent(product_width_2row, product_height_2row, ItemsInShopTexture[i + 6]);
        ZBuffer_DoFill2(&pRenderer->pActiveZBuffer[product_width_2row + 640 * product_height_2row], ItemsInShopTexture[i + 6], i + 7);
      }
    }
    if ( HouseUI_CheckIfPlayerCanInteract() )
    {
      item_num = 0;
      for ( uint i = 0; i < 12; ++i )
      {
        if (pParty->SpecialItemsInShops[(unsigned int)window_SpeakInHouse->ptr_1C][i].uItemID)
          ++item_num;
      }
      if ( GetAsyncKeyState(VK_CONTROL) && pPlayers[uActiveCharacter]->CanSteal() )
        DrawTextAtStatusBar(pGlobalTXT_LocalizationStrings[185], 0);// "Steal item"
      else
        DrawTextAtStatusBar(pGlobalTXT_LocalizationStrings[196], 0);// "Select the Special Item to Buy"
      if ( !item_num )
      {
        dialog_window.DrawShops_next_generation_time_string( pParty->PartyTimes.Shops_next_generation_time[(unsigned int)window_SpeakInHouse->ptr_1C] - pParty->uTimePlayed);
        return;
      }
      pMouse->GetCursorPos(&mouse);
      if ( pRenderer->pActiveZBuffer[mouse.x + pSRZBufferLineOffsets[mouse.y]] & 0xFFFF )
      {
        item = &pParty->SpecialItemsInShops[(int)window_SpeakInHouse->ptr_1C][(pRenderer->pActiveZBuffer[mouse.x + pSRZBufferLineOffsets[mouse.y]] & 0xFFFF) - 1];
        if ( GetAsyncKeyState(VK_CONTROL) && pPlayers[uActiveCharacter]->CanSteal() )
          v30 = BuildDialogueString(pGlobalTXT_LocalizationStrings[181], uActiveCharacter - 1, item, (char *)window_SpeakInHouse->ptr_1C, 2, 0);// "Steal %24"
        else
        {
          v75 = pPlayers[uActiveCharacter]->SelectPhrasesTransaction(item, BuildingType_MagicShop, (int)window_SpeakInHouse->ptr_1C, 2);
          v30 = BuildDialogueString(pMerchantsBuyPhrases[v75], uActiveCharacter - 1, item, (char *)window_SpeakInHouse->ptr_1C, 2, 0);
        }
        dialog_window.DrawTitleText(pFontArrus, 0, (174 - pFontArrus->CalcTextHeight(v30, &dialog_window, 0, 0)) / 2 + 138, TargetColor(255, 255, 255), v30, 3);
        return;
      }
    }
    return;
  }

  if ( dialog_menu_id == HOUSE_DIALOGUE_SHOP_DISPLAY_EQUIPMENT )
  {
    draw_leather();
    CharacterUI_InventoryTab_Draw(pPlayers[uActiveCharacter], true);
    pShopOptions[0] = const_cast<char *>(pGlobalTXT_LocalizationStrings[200]);// "Sell"
    pShopOptions[1] = const_cast<char *>(pGlobalTXT_LocalizationStrings[113]);// "Identify"
    pShopOptions[2] = const_cast<char *>(pGlobalTXT_LocalizationStrings[179]);// "Repair"
    all_text_height = 0;
    for ( uint i = 0; i < 3; ++i )
      all_text_height += pFontArrus->CalcTextHeight(pShopOptions[i], &dialog_window, 0, 0);
    one_string = ((174 - all_text_height) / 3);
    v23 = (3 * (58 - (signed int)one_string) - all_text_height) / 2 - (174 - all_text_height) / 3 / 2 + 138;
    pSrtingNum = 0;
    for ( int i = pDialogueWindow->pStartingPosActiveItem;
              i < pDialogueWindow->pNumPresenceButton + pDialogueWindow->pStartingPosActiveItem; ++i )
    {
      control_button = pDialogueWindow->GetControl(i);
      control_button->uY = one_string + v23;
      control_button->uHeight = pFontArrus->CalcTextHeight(pShopOptions[pSrtingNum], &dialog_window, 0, 0);
      v23 = control_button->uHeight + control_button->uY - 1;
      control_button->uW = v23;
      text_color = TargetColor(225, 205, 35);
      if ( pDialogueWindow->pCurrentPosActiveItem != i )
        text_color = TargetColor(255, 255, 255);
      dialog_window.DrawTitleText(pFontArrus, 0, control_button->uY, text_color, pShopOptions[pSrtingNum], 3);
      ++pSrtingNum;
    }
    return;
  }

  if ( dialog_menu_id == HOUSE_DIALOGUE_SHOP_SELL)
  {
    draw_leather();
    CharacterUI_InventoryTab_Draw(pPlayers[uActiveCharacter], true);
    DrawTextAtStatusBar(pGlobalTXT_LocalizationStrings[199], 0);// "Select the Item to Sell"
    if ( !HouseUI_CheckIfPlayerCanInteract() )
      return;
    pMouse->GetCursorPos(&mouse);
    v117 = ((mouse.x - 14) >> 5) + 14 * ((mouse.y - 17) >> 5);
    if ( mouse.x <= 13 || mouse.x >= 462 )
      return;
    if ( !pPlayers[uActiveCharacter]->GetItemIDAtInventoryIndex((int *)&v117) )
      return;
    item = &pPlayers[uActiveCharacter]->pInventoryItemList[pPlayers[uActiveCharacter]->GetItemIDAtInventoryIndex((int *)&v117) - 1];
    v75 = pPlayers[uActiveCharacter]->SelectPhrasesTransaction(item, BuildingType_MagicShop, (int)window_SpeakInHouse->ptr_1C, 3);
    v30 = BuildDialogueString(pMerchantsSellPhrases[v75], uActiveCharacter - 1, item, (char *)window_SpeakInHouse->ptr_1C, 3, 0);
    dialog_window.DrawTitleText(pFontArrus, 0, (174 - pFontArrus->CalcTextHeight(v30, &dialog_window, 0, 0)) / 2 + 138, TargetColor(255, 255, 255), v30, 3);
    return;
  }

  if ( dialog_menu_id == HOUSE_DIALOGUE_SHOP_IDENTIFY)
  {
    draw_leather();
    CharacterUI_InventoryTab_Draw(pPlayers[uActiveCharacter], true);
    DrawTextAtStatusBar(pGlobalTXT_LocalizationStrings[197], 0);// "Select the Item to Identify"
    if ( HouseUI_CheckIfPlayerCanInteract() )
    {
      pMouse->GetCursorPos(&mouse);
      v117 = ((mouse.x -14) >> 5) + 14 * ((mouse.y - 17) >> 5);
      if ( mouse.x > 13 && mouse.x < 462 )
      {
        if ( pPlayers[uActiveCharacter]->GetItemIDAtInventoryIndex((int *)&v117) )
        {
          item = &pPlayers[uActiveCharacter]->pInventoryItemList[pPlayers[uActiveCharacter]->GetItemIDAtInventoryIndex((int *)&v117) - 1];
          if ( item->uAttributes & 1 )
          {
            dialog_window.DrawTitleText(pFontArrus, 0, (212 - pFontArrus->CalcTextHeight(BuildDialogueString("%24", uActiveCharacter - 1, item, (char *)window_SpeakInHouse->ptr_1C, 4, 0), &dialog_window, 0, 0)) / 2 + 101,
               TargetColor(255, 255, 255), BuildDialogueString("%24", uActiveCharacter - 1, item, (char *)window_SpeakInHouse->ptr_1C, 4, 0), 3); 
            return;
          }
          v75 = pPlayers[uActiveCharacter]->SelectPhrasesTransaction(item, BuildingType_MagicShop, (int)window_SpeakInHouse->ptr_1C, 4);
          v30 = BuildDialogueString(pMerchantsIdentifyPhrases[v75], uActiveCharacter - 1, item, (char *)window_SpeakInHouse->ptr_1C, 4, 0);
          dialog_window.DrawTitleText(pFontArrus, 0, (174 - pFontArrus->CalcTextHeight(v30, &dialog_window, 0, 0)) / 2 + 138, TargetColor(255, 255, 255), v30, 3);
          return;
        }
      }
    }
    return;
  }

  if ( dialog_menu_id == HOUSE_DIALOGUE_SHOP_REPAIR)
  {
    draw_leather();
    CharacterUI_InventoryTab_Draw(pPlayers[uActiveCharacter], true);
    DrawTextAtStatusBar(pGlobalTXT_LocalizationStrings[198], 0);// "Select the Item to Repair"
    if ( !HouseUI_CheckIfPlayerCanInteract() )
      return;
    pMouse->GetCursorPos(&mouse);
    v117 = ((mouse.x - 14) >> 5) + 14 * ((mouse.y - 17) >> 5);
    if ( mouse.x <= 13 || mouse.x >= 462 )
      return;
    if ( !pPlayers[uActiveCharacter]->GetItemIDAtInventoryIndex((int *)&v117)
      || !(pPlayers[uActiveCharacter]->pOwnItems[pPlayers[uActiveCharacter]->GetItemIDAtInventoryIndex((int *)&v117)-1].uAttributes& 2) )
      return;
    item = &pPlayers[uActiveCharacter]->pInventoryItemList[pPlayers[uActiveCharacter]->GetItemIDAtInventoryIndex((int *)&v117) - 1];
    v75 = pPlayers[uActiveCharacter]->SelectPhrasesTransaction(&pPlayers[uActiveCharacter]->pInventoryItemList[pPlayers[uActiveCharacter]->GetItemIDAtInventoryIndex((int *)&v117) - 1],
             BuildingType_MagicShop, (int)window_SpeakInHouse->ptr_1C, 5);
    v30 = BuildDialogueString(pMerchantsRepairPhrases[v75], uActiveCharacter - 1, item, (char *)window_SpeakInHouse->ptr_1C, 5, 0);
    dialog_window.DrawTitleText(pFontArrus, 0, (174 - pFontArrus->CalcTextHeight(v30, &dialog_window, 0, 0)) / 2 + 138, TargetColor(255, 255, 255), v30, 3);
    return;
  }

  if ( dialog_menu_id == HOUSE_DIALOGUE_LEARN_SKILLS )
  {
    if ( HouseUI_CheckIfPlayerCanInteract() )
    {
      all_text_height = 0;
      v33 = (signed __int64)(p2DEvents[(signed int)window_SpeakInHouse->ptr_1C - 1].flt_24 * 500.0);
      pPrice = v33 * (100 - pPlayers[uActiveCharacter]->GetMerchant()) / 100;
      if ( pPrice < v33 / 3 )
        pPrice = v33 / 3;
      item_num = 0;
      for ( int i = pDialogueWindow->pStartingPosActiveItem;
                i < pDialogueWindow->pNumPresenceButton + pDialogueWindow->pStartingPosActiveItem; ++i )
      {
        if ( byte_4ED970_skill_learn_ability_by_class_table[pPlayers[uActiveCharacter]->classType][pDialogueWindow->GetControl(i)->msg_param - 36]
          && !pPlayers[uActiveCharacter]->pActiveSkills[pDialogueWindow->GetControl(i)->msg_param - 36] )
        {
          all_text_height += pFontArrus->CalcTextHeight(pSkillNames[pDialogueWindow->GetControl(i)->msg_param - 36], &dialog_window, 0, 0);
          ++item_num;
        }
      }
      if ( !item_num )
      {
        sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[544], pPlayers[uActiveCharacter]->pName, pClassNames[pPlayers[uActiveCharacter]->classType]);// 
                                            // "Seek knowledge elsewhere %s the %s"
        strcat(pTmpBuf.data(), "\n \n");
        strcat(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[528]);// "I can offer you nothing further."
        dialog_window.DrawTitleText(pFontArrus, 0, (174 - pFontArrus->CalcTextHeight(pTmpBuf.data(), &dialog_window, 0, 0)) / 2 + 138, TargetColor(225, 205, 35), pTmpBuf.data(), 3);
        return;
      }
      sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[401], pPrice);// "Skill Cost: %lu"
      dialog_window.DrawTitleText(pFontArrus, 0, 0x92u, 0, pTmpBuf.data(), 3);
      one_string = (149 - all_text_height) / item_num;
      if ( one_string > 32 )
        one_string = 32;
      v23 = (149 - item_num * one_string - all_text_height) / 2 - one_string / 2 + 162;
      for ( int i = pDialogueWindow->pStartingPosActiveItem;
                i < pDialogueWindow->pStartingPosActiveItem + pDialogueWindow->pNumPresenceButton; ++i)
      {
        control_button = pDialogueWindow->GetControl(i);
        if ( !byte_4ED970_skill_learn_ability_by_class_table[pPlayers[uActiveCharacter]->classType][control_button->msg_param - 36]
           || pPlayers[uActiveCharacter]->pActiveSkills[control_button->msg_param - 36] )
        {
          control_button->uW = 0;
          control_button->uHeight = 0;
          control_button->uY = 0;
        }
        else
        {
          control_button->uY = one_string + v23;
          control_button->uHeight = pFontArrus->CalcTextHeight(pSkillNames[control_button->msg_param - 36], &dialog_window, 0, 0);
          v23 = control_button->uY + control_button->uHeight - 1;
          control_button->uW = v23;
          text_color = TargetColor(225, 205, 35);
          if ( pDialogueWindow->pCurrentPosActiveItem != i )
            text_color = TargetColor(255, 255, 255);
          dialog_window.DrawTitleText(pFontArrus, 0, control_button->uY, text_color, pSkillNames[control_button->msg_param - 36], 3);
        }
      }
    }
    return;
  }
  return;
}
//----- (004BDB56) --------------------------------------------------------
void  UIShop_Buy_Identify_Repair()
{
  int v8; // eax@15
  unsigned int pItemID; // esi@20
  ItemGen *item; // esi@21
  unsigned int v15; // eax@33
  POINT *pCursorPos; // esi@37
  int v18; // ecx@37
  float pPriceMultiplier; // ST1C_4@38
  int taken_item; // eax@40
  ItemGen *bought_item; // esi@51
  int party_reputation; // eax@55
  int v39; // eax@63
  int v42; // esi@74
  signed int v43; // ebx@74
  unsigned __int16 *pSkill; // esi@77
  int v55; // [sp+0h] [bp-B4h]@26
  POINT cursor; // [sp+40h] [bp-74h]@37
  int a6; // [sp+98h] [bp-1Ch]@57
  int a3; // [sp+9Ch] [bp-18h]@53
  unsigned int uNumSeconds; // [sp+A4h] [bp-10h]@53
  unsigned int v79; // [sp+A8h] [bp-Ch]@9
  int uPriceItemService; // [sp+ACh] [bp-8h]@12

  if ( pCurrentScreen == SCREEN_E )
  {
    pPlayers[uActiveCharacter]->OnInventoryLeftClick();
    return;
  }
  if ( !HouseUI_CheckIfPlayerCanInteract() )
  {
    pAudioPlayer->PlaySound(SOUND_error, 0, 0, -1, 0, 0, 0, 0);
    return;
  }

  switch(dialog_menu_id)
  {
    case HOUSE_DIALOGUE_SHOP_DISPLAY_EQUIPMENT:
    {
      pWindowList_at_506F50_minus1_indexing_buttons____and_an_int_[0] = 103;
      pPlayers[uActiveCharacter]->OnInventoryLeftClick();
      break;
    }
    case HOUSE_DIALOGUE_GUILD_BUY_BOOKS:
    {
      pMouse->GetCursorPos(&cursor);
      v18 = pRenderer->pActiveZBuffer[cursor.x + pSRZBufferLineOffsets[cursor.y]] & 0xFFFF;
      if ( !v18 )
        return;
      bought_item = (ItemGen *)(&pParty->pPlayers[1].uExpressionTimeLength + 18 * (v18 + 12 * (int)window_SpeakInHouse->ptr_1C));
      pPriceMultiplier = p2DEvents[(signed int)window_SpeakInHouse->ptr_1C - 1].fPriceMultiplier;
      uPriceItemService = pPlayers[uActiveCharacter]->GetBuyingPrice(bought_item->GetValue(), pPriceMultiplier);
      GetAsyncKeyState(VK_CONTROL);
      if ( pParty->uNumGold < uPriceItemService )
      {
        PlayHouseSound((unsigned int)window_SpeakInHouse->ptr_1C, (HouseSoundID)2);
        ShowStatusBarString(pGlobalTXT_LocalizationStrings[155], 2);
        return;
      }
      taken_item = pPlayers[uActiveCharacter]->AddItem(-1, bought_item->uItemID);
      if ( taken_item )
      {
        bought_item->SetIdentified();
        memcpy(&pPlayers[uActiveCharacter]->pInventoryItemList[taken_item - 1], bought_item, 0x24u);
        dword_F8B1E4 = 1;
        Party::TakeGold(uPriceItemService);
        viewparams->bRedrawGameUI = 1;
        bought_item->Reset();
        pRenderer->ClearZBuffer(0, 479);
        pPlayers[uActiveCharacter]->PlaySound((PlayerSpeech)SPEECH_75, 0);
        return;
      }
      pPlayers[uActiveCharacter]->PlaySound(SPEECH_NoRoom, 0);
      ShowStatusBarString(pGlobalTXT_LocalizationStrings[563], 5);  // "Pack is Full!"
      break;
    }
    case HOUSE_DIALOGUE_SHOP_SELL:
    {
      v79 = ((pMouse->GetCursorPos(&cursor)->x - 14) >> 5) + 14 * ((pMouse->GetCursorPos(&cursor)->y - 17) >> 5);
      if ( pMouse->GetCursorPos(&cursor)->x <= 13
        || pMouse->GetCursorPos(&cursor)->x >= 462
        || (v15 = pPlayers[uActiveCharacter]->GetItemIDAtInventoryIndex((int *)&v79), !v15) )
          return;
      if ( MerchandiseTest(&pPlayers[uActiveCharacter]->pInventoryItemList[v15 - 1], (int)window_SpeakInHouse->ptr_1C) )
      {
        dword_F8B1E4 = 1;
        pPlayers[uActiveCharacter]->SalesProcess(v79, v15 - 1, (int)window_SpeakInHouse->ptr_1C);
        viewparams->bRedrawGameUI = 1;
        pRenderer->ClearZBuffer(0, 479);
        pPlayers[uActiveCharacter]->PlaySound((PlayerSpeech)77, 0);
        return;
      }
      pPlayers[uActiveCharacter]->PlaySound(SPEECH_79, 0);
      pAudioPlayer->PlaySound(SOUND_error, 0, 0, -1, 0, 0, 0, 0);
      break;
    }
    case HOUSE_DIALOGUE_SHOP_IDENTIFY:
    {
      pMouse->GetCursorPos(&cursor);
      v79 = ((cursor.x - 14) >> 5) + 14 * ((cursor.y - 17) >> 5);
      if (cursor.x > 13  && cursor.x < 462)
      {
        pItemID = pPlayers[uActiveCharacter]->GetItemIDAtInventoryIndex((int *)&v79);
        if ( pItemID )
        {
          uPriceItemService = pPlayers[uActiveCharacter]->GetPriceIdentification(p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].fPriceMultiplier);
          item = &pPlayers[uActiveCharacter]->pInventoryItemList[pItemID - 1];
          if ( !(item->uAttributes & 1) )
          {
            if ( MerchandiseTest(item, (int)window_SpeakInHouse->ptr_1C) )
            {
              if ( pParty->uNumGold >= uPriceItemService )
              {
                dword_F8B1E4 = 1;
                Party::TakeGold(uPriceItemService);
                item->uAttributes |= 1;
                pPlayers[uActiveCharacter]->PlaySound(SPEECH_73, 0);
                ShowStatusBarString(pGlobalTXT_LocalizationStrings[569], 2);
                return;
              }
              PlayHouseSound((unsigned int)window_SpeakInHouse->ptr_1C, (HouseSoundID)2);
              return;
            }
            pAudioPlayer->PlaySound(SOUND_error, 0, 0, -1, 0, 0, 0, 0);
            pPlayers[uActiveCharacter]->PlaySound((PlayerSpeech)79, 0);
            return;
          }
          pPlayers[uActiveCharacter]->PlaySound((PlayerSpeech)76, 0);
          return;
        }
      }
      break;
    }
    case HOUSE_DIALOGUE_SHOP_REPAIR:
    {
      v79 = ((pMouse->GetCursorPos(&cursor)->x - 14) >> 5) + 14 * ((pMouse->GetCursorPos(&cursor)->y - 17) >> 5);
      if ( pMouse->GetCursorPos(&cursor)->x > 13 )
      {
        if ( pMouse->GetCursorPos(&cursor)->x < 462 )
        {
          pItemID = pPlayers[uActiveCharacter]->GetItemIDAtInventoryIndex((int *)&v79);
          if ( pItemID )
          {
            item = &pPlayers[uActiveCharacter]->pInventoryItemList[pItemID - 1];
            pPriceMultiplier = p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].fPriceMultiplier;
            auto _v = (ItemGen *)&pPlayers[uActiveCharacter]->pInventoryItemList[pItemID - 1];
            uPriceItemService = pPlayers[uActiveCharacter]->GetPriceRepair(_v->GetValue(), pPriceMultiplier);
            if ( item->uAttributes & 2 )
            {
              if ( MerchandiseTest(item, (int)window_SpeakInHouse->ptr_1C) )
              {
                if ( pParty->uNumGold >= uPriceItemService )
                {
                  dword_F8B1E4 = 1;
                  Party::TakeGold(uPriceItemService);
                  v8 = item->uAttributes;
                  LOBYTE(v8) = v8 & 0xFD;
                  item->uAttributes = v8 | 1;
                  pPlayers[uActiveCharacter]->PlaySound(SPEECH_74, 0);
                  ShowStatusBarString(pGlobalTXT_LocalizationStrings[570], 2);
                  return;
                }
                PlayHouseSound((unsigned int)window_SpeakInHouse->ptr_1C, (HouseSoundID)2);
                return;
              }
              pAudioPlayer->PlaySound(SOUND_error, 0, 0, -1, 0, 0, 0, 0);
              pPlayers[uActiveCharacter]->PlaySound((PlayerSpeech)79, 0);
              return;
            }
            pPlayers[uActiveCharacter]->PlaySound((PlayerSpeech)76, 0);
            return;
          }
        }
      }
      break;
    }
    case HOUSE_DIALOGUE_SHOP_BUY_STANDARD:
    case HOUSE_DIALOGUE_SHOP_BUY_SPECIAL:
    {
      pCursorPos = pMouse->GetCursorPos(&cursor);
      v18 = pRenderer->pActiveZBuffer[pCursorPos->x + pSRZBufferLineOffsets[pCursorPos->y]] & 0xFFFF;
      if ( !v18 )
        return;
      if ( dialog_menu_id == HOUSE_DIALOGUE_SHOP_BUY_STANDARD)
        bought_item = (ItemGen *)&pParty->StandartItemsInShops[(int)window_SpeakInHouse->ptr_1C][v18 - 1];
      else
        bought_item = &pParty->SpecialItemsInShops[(int)window_SpeakInHouse->ptr_1C][v18 - 1];
      uPriceItemService = pPlayers[uActiveCharacter]->GetBuyingPrice(bought_item->GetValue(), p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].fPriceMultiplier);
      uNumSeconds = 0;
      a3 = 0;
      if ( pMapStats->GetMapInfo(pCurrentMapName) )
        a3 = pMapStats->pInfos[pMapStats->GetMapInfo(pCurrentMapName)]._steal_perm;
      party_reputation = pParty->GetPartyReputation();
      if (pPlayers[uActiveCharacter]->CanSteal())
      {
        if ( GetAsyncKeyState(VK_CONTROL) )
        {
          uNumSeconds = pPlayers[uActiveCharacter]->StealFromShop(bought_item, a3, party_reputation, 0, &a6);
          if ( !uNumSeconds )
          {
            sub_4B1447_party_fine((int)window_SpeakInHouse->ptr_1C, 0, a6);
            return;
          }
        }
      }
      if ( pParty->uNumGold < uPriceItemService )
      {
        if ( uNumSeconds != 2 )
        {
          if ( uNumSeconds != 1 )
          {
            PlayHouseSound((unsigned int)window_SpeakInHouse->ptr_1C, (HouseSoundID)2);
            ShowStatusBarString(pGlobalTXT_LocalizationStrings[155], 2);// "You don't have enough gold"
            return;
          }
        }
      }
      v39 = pPlayers[uActiveCharacter]->AddItem(-1, bought_item->uItemID);
      if ( v39 )
      {
        bought_item->SetIdentified();
        memcpy(&pPlayers[uActiveCharacter]->pInventoryItemList[v39 - 1], bought_item, sizeof(ItemGen));
        if ( pPlayers[uActiveCharacter]->CanSteal() )
        {
          if ( GetAsyncKeyState(VK_CONTROL) )
          {
            if ( uNumSeconds == 1 || uNumSeconds == 2 )
            {
              pPlayers[uActiveCharacter]->pInventoryItemList[v39 - 1].SetStolen();
              sub_4B1447_party_fine((int)window_SpeakInHouse->ptr_1C, uNumSeconds, a6);
              viewparams->bRedrawGameUI = 1;
              bought_item->Reset();
              pRenderer->ClearZBuffer(0, 479);
              pPlayers[uActiveCharacter]->PlaySound((PlayerSpeech)SPEECH_75, 0);
              return;
            }
          }
        }
        dword_F8B1E4 = 1;
        Party::TakeGold(uPriceItemService);
        viewparams->bRedrawGameUI = 1;
        bought_item->Reset();
        pRenderer->ClearZBuffer(0, 479);
        pPlayers[uActiveCharacter]->PlaySound((PlayerSpeech)SPEECH_75, 0);
        return;
      }
      pPlayers[uActiveCharacter]->PlaySound(SPEECH_NoRoom, 0);
      ShowStatusBarString(pGlobalTXT_LocalizationStrings[563], 2); // "Pack is Full!"
      break;
    }
    default:// if click video screen in shop
    {
      __debugbreak(); // please do record these dialogue ids to the HOUSE_DIALOGUE_MENU  enum
      if( dialog_menu_id >= 36 && dialog_menu_id <= 72 )
      {
        v42 = dialog_menu_id - 36;
        //v43 = (signed __int64)(*(float *)&p2DEvents_minus1__24[13 * (unsigned int)ptr_507BC0->ptr_1C] * 500.0);
        v43 = (signed __int64)(p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].flt_24 * 500.0);
        uPriceItemService = v43 * (100 - pPlayers[uActiveCharacter]->GetMerchant()) / 100;
        if ( uPriceItemService < v43 / 3 )
          uPriceItemService = v43 / 3;
        if ( byte_4ED970_skill_learn_ability_by_class_table[pPlayers[uActiveCharacter]->classType][v42] )
        {
          pSkill = &pPlayers[uActiveCharacter]->pActiveSkills[v42];
          if ( !*pSkill )
          {
            if ( pParty->uNumGold < uPriceItemService )
            {
              ShowStatusBarString(pGlobalTXT_LocalizationStrings[155], 2);// "You don't have enough gold"
              if ( in_current_building_type == BuildingType_Training )
                v55 = 4;
              else
                v55 = 2;
              PlayHouseSound((unsigned int)window_SpeakInHouse->ptr_1C, (HouseSoundID)v55);
              return;
            }
            Party::TakeGold(uPriceItemService);
            dword_F8B1E4 = 1;
           *pSkill = 1;
            pPlayers[uActiveCharacter]->PlaySound((PlayerSpeech)78, 0);
            return;
          }
        }
      }
      break;
    }
  }
}
//----- (004B1A2D) --------------------------------------------------------
void  ShowPopupShopItem()
{
  int v3; // ecx@5
  unsigned int v6; // eax@13
  ItemGen *v7; // ecx@13
  POINT  cursor; // [sp+30h] [bp-1Ch]@17

  if ( in_current_building_type <= 0 )
    return;
  if ( dialog_menu_id < HOUSE_DIALOGUE_SHOP_BUY_STANDARD)
    return;
  if ( in_current_building_type <= BuildingType_AlchemistShop )
  {
    if ( dialog_menu_id == HOUSE_DIALOGUE_SHOP_BUY_STANDARD || dialog_menu_id == HOUSE_DIALOGUE_SHOP_BUY_SPECIAL )
    {
      pMouse->GetCursorPos(&cursor);
      v3 = pRenderer->pActiveZBuffer[cursor.x + pSRZBufferLineOffsets[cursor.y]] & 0xFFFF;
      if ( !v3 )
        return;
      v7 = &pParty->StandartItemsInShops[(unsigned int)window_SpeakInHouse->ptr_1C][v3 - 1];
      if ( dialog_menu_id == HOUSE_DIALOGUE_SHOP_BUY_SPECIAL)
        v7 = &pParty->SpecialItemsInShops[(unsigned int)window_SpeakInHouse->ptr_1C][v3 - 1];
      GameUI_DrawItemInfo(v7);
      return;
    }

    if ( dialog_menu_id >= HOUSE_DIALOGUE_SHOP_SELL && dialog_menu_id <= HOUSE_DIALOGUE_SHOP_REPAIR
      || dialog_menu_id == HOUSE_DIALOGUE_SHOP_DISPLAY_EQUIPMENT )
    {
      pMouse->GetCursorPos(&cursor);
      v3 = ((cursor.x - 14) >> 5) + 14 * ((cursor.y - 17) >> 5);
      if ( cursor.x <= 13 || cursor.x >= 462
        || !pPlayers[uActiveCharacter]->GetItemIDAtInventoryIndex(&v3) )
        return;
      GameUI_DrawItemInfo(&pPlayers[uActiveCharacter]->pInventoryItemList[pPlayers[uActiveCharacter]->GetItemIDAtInventoryIndex(&v3) - 1]);
      return;
    }
  }
  if ( in_current_building_type <= BuildingType_16 && dialog_menu_id == HOUSE_DIALOGUE_GUILD_BUY_BOOKS )
  {
    pMouse->GetCursorPos(&cursor);
    v3 = pRenderer->pActiveZBuffer[cursor.x + pSRZBufferLineOffsets[cursor.y]] & 0xFFFF;
    if ( v3 )
      sub_4B1523((int *)&pParty->pPlayers[1].uExpressionTimeLength + 9 * (v3 + 12 * (unsigned int)window_SpeakInHouse->ptr_1C));
  }
}
//----- (004B1D27) --------------------------------------------------------
void  GetHouseGoodbyeSpeech()
{
  signed int v2; // edi@10
  signed int v5; // edi@20
  int v7[4]; // [sp+Ch] [bp-10h]@12

  if ( in_current_building_type > 0 )
  {
    if ( in_current_building_type > BuildingType_MagicShop )
    {
      if ( in_current_building_type == BuildingType_Bank )
      {
        if ( !dword_F8B1E4 )
          return;
      }
      else
      {
        if ( in_current_building_type != BuildingType_Temple )
          return;
      }
      PlayHouseSound((unsigned int)window_SpeakInHouse->ptr_1C, HouseSound_Greeting_2);
      return;
    }
    if ( (signed __int64)pParty->PartyTimes._shop_ban_times[(unsigned int)window_SpeakInHouse->ptr_1C]<= (signed __int64)pParty->uTimePlayed )
    {
      if ( pParty->uNumGold <= 10000 )
      {
        if ( !dword_F8B1E4 )
          return;
        PlayHouseSound((unsigned int)window_SpeakInHouse->ptr_1C, HouseSound_Goodbye);
        return;
      }
      PlayHouseSound((unsigned int)window_SpeakInHouse->ptr_1C, (HouseSoundID)(dword_F8B1E4 + 3));
      if ( !dword_F8B1E4 && !qword_A750D8 )
      {
        v5 = 0;
        for ( uint i = 1; i <= 4; ++i )
        {
          if ( pPlayers[i]->CanAct() )
            v7[v5++] = i;
        }
        if ( v5 )
        {
          qword_A750D8 = 256i64;
          PlayerSpeechID = SPEECH_80;
          uSpeakingCharacter = v7[rand() % v5];
          return;
        }
      }
    }
    else
    {
      if ( !qword_A750D8 )
      {
        v2 = 0;
        for ( uint i = 1; i <= 4; ++i )
        {
          if ( pPlayers[i]->CanAct() )
            v7[v2++] = i;
        }
        if ( v2 )
        {
          qword_A750D8 = 256i64;
          PlayerSpeechID = SPEECH_80;
          uSpeakingCharacter = v7[rand() % v2];
          return;
        }
      }
    }
  }
}