#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define _CRT_SECURE_NO_WARNINGS
#include <io.h>
#include <direct.h>
#include "mm7_unsorted_subs.h"
#include "UI\UIPartyCreation.h"
#include "GUIButton.h"


#include "mm7_data.h"
#include "Sprites.h"

#include "LightmapBuilder.h"
#include "DecalBuilder.h"
#include "ParticleEngine.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "MediaPlayer.h"
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
#include "Log.h"
#include "Actor.h"
#include "Events.h"
#include "Viewport.h"
#include "OurMath.h"
#include "SpriteObject.h"
#include "ObjectList.h"
#include "Chest.h"
#include "PaletteManager.h"
#include "DecorationList.h"
#include "SaveLoad.h"
#include "stru123.h"
#include "MapsLongTimer.h"
#include "Timer.h"
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
#include "UI\UIHouses.h"
#include "texts.h"
#include "MM7.h"
#include "Lights.h"
#include "NewUI/MainMenu.h"
#include "Level/Decoration.h"
#include "LuaVM.h"
#include "MMT.h"
#include "Registry.h"

int __stdcall aWinProc(HWND hWnd, UINT Msg, WPARAM wParam, unsigned int lParam);
int __stdcall InsertMM7CDDialogFunc(HWND hDlg, int a2, __int16 a3, int a4);
bool __fastcall FindMM7CD(HWND hWnd, char *pCDDrive);
bool __fastcall Initialize(HINSTANCE hInst, char *pCmdLine);


//----- (004BBA85) --------------------------------------------------------
void CheckBountyRespawnAndAward()
{
  int i; // eax@2
  int rand_monster_id; // edx@3

  uDialogueType = 83;
  pDialogueWindow->Release();
  pDialogueWindow = GUIWindow::Create(0, 0, window->GetWidth(), 350, WINDOW_MainMenu, 0, 0);
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

//----- (004BF91E) --------------------------------------------------------
unsigned int GameOverMenu(void *ecx0)
{
  const char *v1; // eax@2
  unsigned int result; // eax@3
  const char *v6; // eax@10
  const char *v7; // edx@10
  const char *v8; // ecx@12
  const char *v9; // eax@14
  unsigned int v10; // eax@25
  GUIWindow pWindow; // [sp+34h] [bp-9Ch]@1
  unsigned int v14; // [sp+A4h] [bp-2Ch]@5
  void *v15; // [sp+A8h] [bp-28h]@1
  const char *pInString; // [sp+ACh] [bp-24h]@5
  unsigned int v17; // [sp+B0h] [bp-20h]@5
  unsigned int v18; // [sp+B4h] [bp-1Ch]@5
  unsigned int v19; // [sp+B8h] [bp-18h]@5
  int v20; // [sp+BCh] [bp-14h]@7
  GUIFont *pFont; // [sp+C4h] [bp-Ch]@1
  unsigned __int64 v23; // [sp+C8h] [bp-8h]@5

  v15 = ecx0;

  RGBTexture _this; // [sp+Ch] [bp-C4h]@1
  //RGBTexture::RGBTexture(&this);

  dword_6BE364_game_settings_1 &= ~GAME_SETTINGS_4000;
  bGameoverLoop = 1;
  pMediaPlayer->bStopBeforeSchedule = 0;
  pAudioPlayer->StopChannels(-1, -1);
  pRenderer->BeginScene();
  pRenderer->ClearBlack();
  pRenderer->EndScene();
  pRenderer->Present();
  //pMediaPlayer->pResetflag = 0;
  _449B57_test_bit(pParty->_quest_bits, 99);
  _this.Load("winbg.pcx", 2);
  pRenderer->BeginScene();
  pRenderer->DrawTextureRGB(0, 0, &_this);
  pRenderer->EndScene();
  free(_this.pPixels);
  _this.pPixels = 0;
  window_SpeakInHouse = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_MainMenu, 0, 0);
  pWindow.uFrameX = 75;
  pWindow.uFrameY = 60;
  pWindow.uFrameWidth = 469;
  pWindow.uFrameHeight = 338;
  pWindow.uFrameZ = 543;
  pWindow.uFrameW = 397;
  pFont = LoadFont("endgame.fnt", "FONTPAL", NULL);
  if ( pParty->IsPartyGood() )
    v1 = pGlobalTXT_LocalizationStrings[675];//"Splendid job!  With the activation of the Gate, a thousand worlds lie at your feet.  Perhaps on one of them you will find the Ancients themselves, and return with the fruits their great civilization has to offer your world and your kingdom."
  else
  {
    result = pParty->IsPartyEvil();
    if ( !(short)result )
      return result;
    v1 = pGlobalTXT_LocalizationStrings[676];//"Brilliant!  The completion of the Heavenly Forge has provided enough Ancient weapons to crush all resistance to your plans.  Soon the world will bow to your every whim!  Still, you can't help but wonder what was beyond the Gate the other side was trying so hard to build."
  }
  pInString = v1;
  v23 = pParty->uTimePlayed - 138240;
  v19 = (unsigned int)((signed __int64)((double)(pParty->uTimePlayed - 138240) * 0.234375) / 60 / 60) / 24;
  v14 = (unsigned int)((signed __int64)((double)(pParty->uTimePlayed - 138240) * 0.234375) / 60 / 60) / 24 / 0x1C / 0xC;
  v18 = (unsigned int)((signed __int64)((double)(pParty->uTimePlayed - 138240) * 0.234375) / 60 / 60) / 24 / 0x1C % 0xC;
  v17 = v19 % 0x1C;
  if ( !v19 )
    v19 = 1;
  pRenderer->BeginScene();
  pWindow.DrawTitleText(pFont, 1, 0x23, 1, pGlobalTXT_LocalizationStrings[9], 3);//Congratulations!
  v23 = 0i64;
  v20 = 0;
  for ( uint i = 0; i < 4; i++ )
  {
    sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[129], pParty->pPlayers[i].pName, pParty->pPlayers[i].GetBaseLevel(), pClassNames[pParty->pPlayers[i].classType]);//%s the Level %u %s
    pWindow.DrawTitleText(pFont, 1, i * (LOBYTE(pFont->uFontHeight) - 2) + LOBYTE(pFont->uFontHeight) + 46, 1, pTmpBuf.data(), 3);
    v23 += pParty->pPlayers[i].uExperience;//__PAIR__(*(int *)(i - 4), *(int *)(i - 8));
  }
  v23 = (signed __int64)v23 / v19;
  v6 = FitTextInAWindow(pInString, pFont, &pWindow, 0xC, 0);
  pWindow.DrawTitleText(pFont, 1, 5 * (LOBYTE(pFont->uFontHeight) + 11), 1, v6, 0);
  strcpy(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[37]);//Total Time:

  v7 = pGlobalTXT_LocalizationStrings[56];
  if ( v17 != 1 )
    v7 = pGlobalTXT_LocalizationStrings[57];

  v8 = pGlobalTXT_LocalizationStrings[146];//Month
  if ( v18 != 1 )
    v8 = pGlobalTXT_LocalizationStrings[148];//Months

  v9 = pGlobalTXT_LocalizationStrings[245];
  if ( v14 != 1 )
    v9 = pGlobalTXT_LocalizationStrings[132];

  sprintf(pTmpBuf2.data(), " %lu %s, %lu %s, %lu %s ", v14, v9, v18, v8, v17, v7);
  strcat(pTmpBuf.data(), pTmpBuf2.data());
  pWindow.DrawTitleText(pFont, 1, pWindow.uFrameHeight - 2 * LOBYTE(pFont->uFontHeight) - 5, 1, pTmpBuf.data(), 3);
  sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[94], v23);
  pWindow.DrawTitleText(pFont, 1, pWindow.uFrameHeight, 1, pTmpBuf.data(), 3);
  dword_6BE364_game_settings_1 |= GAME_SETTINGS_4000;
  pRenderer->EndScene();
  pRenderer->Present();
  //if ( pRenderer->pRenderD3D )
    pRenderer->pBeforePresentFunction();
  pRenderer->SaveWinnersCertificate("MM7_Win.Pcx");
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
    pMessageQueue_50CBD0->AddGUIMessage(UIMSG_Quit, 1, 0);
  }
  else
  {
    pMessageQueue_50CBD0->AddGUIMessage(UIMSG_ShowFinalWindow, 1, 0);
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
//  int v160; // [sp+3Ch] [bp-4Ch]@13
  unsigned __int16 *v175; // [sp+4Ch] [bp-3Ch]@13
  unsigned __int16 *v193; // [sp+5Ch] [bp-2Ch]@7
  //signed int v231; // [sp+78h] [bp-10h]@7
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
  if ( dstHeight <= 0 )
    return result;
  
  //do
  for ( int height = 0; height < dstHeight; height++ )
  {
    for (int width = 0; width < dstWidth; width++)
    {
      a6s = (double)width / (double)dstWidth * (double)srcWidth;
      widthRatio = bankersRounding(a6s);
      a6t = (double)(width + 1) / (double)dstWidth * (double)srcWidth;
      widthRatioPlusOne = bankersRounding(a6t);

      v17 = (double)height / (double)dstHeight * (double)srcHeight;
      heightRatio = bankersRounding(v17);
      v18 = (double)(height + 1) / (double)dstHeight * (double)srcHeight;
      heightRatioPlusOne = bankersRounding(v18);

      v251 = 0;
      v19 = (heightRatioPlusOne - heightRatio) * (widthRatioPlusOne - widthRatio);
      v252 = 0;
      a6b = 0;
      v240 = 0i64;

      v175 = (unsigned short*)((char *)pSrc + field_0_bits * (widthRatio + srcPitch * heightRatio));
      for (int heightDiff = 0; heightDiff < heightRatioPlusOne - heightRatio; heightDiff++)
      {
        //int ratioDiff = widthRatioPlusOne - widthRatio;
        for(int ratioDiff = 0; ratioDiff < widthRatioPlusOne - widthRatio; ratioDiff++)
        {
          if(field0value == 32)
            v21 = _450FB1(((int*)v175)[ratioDiff]);
          else if(field0value == 16)
            v21 = _450FB1(((_WORD*)v175)[ratioDiff]);
          else if (field0value == 8)
            v21 = _450FB1(((unsigned __int8*)v175)[ratioDiff]);
          v240 += ((unsigned int)v21 >> 24);
          a6b += BYTE2(v21);
          v252 += BYTE1(v21);
          v251 += (unsigned __int8)v21;
        }
        if (field0value == 32)
          v175 += 2 * srcPitch;
        else if (field0value == 16)
          v175 += srcPitch;
        else if (field0value == 8)
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
      *(_DWORD *)v193 = v23; //Invalid memory access
      v193 = (unsigned __int16 *)((char *)v193 + field_20_bits);
    }
    v193 = (unsigned __int16 *)((char *)v193 + field_20_bits * (dstPitch - dstWidth));
    //++v231;
    result = height;
  }
  //while(v231 < dstHeight);
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
//  int item_counter;

  free(pHostileTXT_Raw);
  pHostileTXT_Raw = (char *)pEvents_LOD->LoadRaw("hostile.txt", 0);
  strtok(pHostileTXT_Raw, "\r");
  for ( i=0; i < 89; ++i )
  {
     test_string = strtok(NULL, "\r") + 1;
     break_loop = false;
     decode_step=0;
     do 
     {
       c = *(unsigned char*)test_string;
       temp_str_len = 0;
       while((c!='\t') && (c > 0))
       {
         ++temp_str_len;
         c = test_string[temp_str_len];
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
         break_loop = true;
       ++decode_step;
       test_string=tmp_pos+1;
     }
     while ((decode_step < 92) && !break_loop );
   }
  free(pHostileTXT_Raw);
  pHostileTXT_Raw = nullptr;
}

//----- (004610AA) --------------------------------------------------------
void __fastcall PrepareToLoadODM(unsigned int bLoading, ODMRenderParams *a2)
{
  pGameLoadingUI_ProgressBar->Reset(27);
  pSoundList->_4A9D79(0);
  uCurrentlyLoadedLevelType = LEVEL_Outdoor;
  ODM_LoadAndInitialize(pCurrentMapName, a2);
  if ( !bLoading )
    TeleportToStartingPoint(uLevel_StartingPointType);
  viewparams->_443365();
  PlayLevelMusic();
}
// 6BE35C: using guessed type int uLevel_StartingPointType;

//----- (00461103) --------------------------------------------------------
void _461103_load_level_sub()
{
  int v4; // edx@8
  signed int v6; // esi@14
  signed int v8; // ecx@16
  int v12; // esi@25
  int v13; // eax@26
  __int16 v14; // ax@41
  signed int v17; // [sp+14h] [bp-48h]@3
  signed int v18; // [sp+14h] [bp-48h]@23
  int v19; // [sp+18h] [bp-44h]@1
  signed int v20; // [sp+18h] [bp-44h]@14
  int v21[16]; // [sp+1Ch] [bp-40h]@17

  GenerateItemsInChest();
  pGameLoadingUI_ProgressBar->Progress();
  pParty->uFlags |= 2;
  pParty->field_7B5_in_arena_quest = 0;
  dword_5C6DF8 = 1;
  pNPCStats->uNewlNPCBufPos = 0;
  v19 = pMapStats->GetMapInfo(pCurrentMapName);

  //v15 = 0;
  for (uint i = 0; i < uNumActors; ++i)
  //if ( (signed int)uNumActors > 0 )
  {
    //Actor* pActor = &pActors[i];
    //v2 = (char *)&pActors[0].uNPC_ID;
    //do
    //{
      //v3 = pActors[i].pMonsterInfo.uID;
      v17 = 0;
      if ( pActors[i].pMonsterInfo.uID >= 115 && pActors[i].pMonsterInfo.uID <= 186
        || pActors[i].pMonsterInfo.uID >= 232 && pActors[i].pMonsterInfo.uID <= 249 )
        v17 = 1;
      //v1 = 0;
      v4 = (pActors[i].pMonsterInfo.uID - 1) % 3;
      if ( 2 == v4 )
      {
        if ( pActors[i].sNPC_ID && pActors[i].sNPC_ID < 5000 )
          continue;
      }
      else
      {
        if ( v4 != 1 )
        {
          if ( v4 == 0 && pActors[i].sNPC_ID == 0 )
            pActors[i].sNPC_ID = 0;
          continue;
        }
      }
      if ( pActors[i].sNPC_ID > 0 && pActors[i].sNPC_ID < 5000 )
        continue;
      if ( v17 )
      {
        pNPCStats->InitializeAdditionalNPCs(&pNPCStats->pAdditionalNPC[pNPCStats->uNewlNPCBufPos], pActors[i].pMonsterInfo.uID, 0, v19);
        v14 = LOWORD(pNPCStats->uNewlNPCBufPos) + 5000;
        ++pNPCStats->uNewlNPCBufPos;
        pActors[i].sNPC_ID = v14;
        continue;
      }
      pActors[i].sNPC_ID = 0;
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
    //v7 = (char *)&pActors[0].pMonsterInfo;
    //do
    //{
    for ( v8 = 0; v8 < v6; ++v8 )
    {
      if ( v21[v8] == pActors[i].pMonsterInfo.uID - 1 )
        break;
    }

    if ( v8 == v6 )
    {
      v21[v6++] = pActors[i].pMonsterInfo.uID - 1;
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

    for ( int _v0 = 0; _v0 < v6; ++_v0 )
    {
      for ( v18 = 4; v18; --v18 )
        pSoundList->LoadSound(pMonsterList->pMonsters[v21[_v0]].pSoundSampleIDs[4 - v18], 0);
      v12 = 0;
      do
        v13 = pSoundList->LoadSound(v12++ + word_4EE088_sound_ids[pMonsterStats->pInfos[v21[_v0] + 1].uSpell1ID], 1);
      while ( v13 );
    }
    //v0 = pGameLoadingUI_ProgressBar;
    //v1 = 0;

  pGameLoadingUI_ProgressBar->Progress();

  if (dword_6BE368_debug_settings_2 & DEBUG_SETTINGS_NO_ACTORS)
    uNumActors = 0;
  if ( dword_6BE368_debug_settings_2 & DEBUG_SETTINGS_NO_DECORATIONS)
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
  unsigned int pY; // [sp-18h] [bp-54h]@39
  Texture *pTexture; // [sp-14h] [bp-50h]@39
  GUIWindow *pWindow; // [sp+4h] [bp-38h]@11
  
  pCurrentScreen = SCREEN_GAME;

  pGUIWindow2 = 0;
  pAudioPlayer->StopChannels(-1, -1);
  pMouse->RemoveHoldingItem();

  pIcons_LOD->_inlined_sub2();

  pWindow_MainMenu = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_MainMenu, 0, 0);
  Texture* pNew = pIcons_LOD->LoadTexturePtr("title_new", TEXTURE_16BIT_PALETTE);
  Texture* pLoad = pIcons_LOD->LoadTexturePtr("title_load", TEXTURE_16BIT_PALETTE);
  Texture* pCredits = pIcons_LOD->LoadTexturePtr("title_cred", TEXTURE_16BIT_PALETTE);
  Texture* pExit = pIcons_LOD->LoadTexturePtr("title_exit", TEXTURE_16BIT_PALETTE);

  pMainMenu_BtnNew     = pWindow_MainMenu->CreateButton(495, 172, pNew->uTextureWidth,     pNew->uTextureHeight,     1, 0, UIMSG_MainMenu_ShowPartyCreationWnd, 0, 'N', "", pNew, 0);
  pMainMenu_BtnLoad    = pWindow_MainMenu->CreateButton(495, 227, pLoad->uTextureWidth,    pLoad->uTextureHeight,    1, 0, UIMSG_MainMenu_ShowLoadWindow,       1, 'L', "", pLoad, 0);
  pMainMenu_BtnCredits = pWindow_MainMenu->CreateButton(495, 282, pCredits->uTextureWidth, pCredits->uTextureHeight, 1, 0, UIMSG_ShowCredits,                   2, 'C', "", pCredits, 0);
  pMainMenu_BtnExit    = pWindow_MainMenu->CreateButton(495, 337, pExit->uTextureWidth,    pExit->uTextureHeight,    1, 0, UIMSG_ExitToWindows,                 3, 0, "", pExit, 0);

  pTexture_PCX.Release();
  pTexture_PCX.Load("title.pcx", 0);
  SetCurrentMenuID(MENU_MAIN);
  SetForegroundWindow(window->GetApiHandle());
  SendMessageW(window->GetApiHandle(), WM_ACTIVATEAPP, 1, 0);
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
        pGUIWindow2 = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_MainMenu, 0, 0);
        pCurrentScreen = SCREEN_LOADGAME;
        LoadUI_Load(0);
      }
      pWindow = pGUIWindow_CurrentMenu;
    }

    for (MSG msg; PeekMessageW(&msg, 0, 0, 0, PM_REMOVE);)
    {
      if (msg.message == WM_QUIT)
        Game_DeinitializeAndTerminate(0);
      TranslateMessage(&msg);
      DispatchMessageW(&msg);
    }

    if (dword_6BE364_game_settings_1 & GAME_SETTINGS_APP_INACTIVE)
    {
      WaitMessage();
      continue;
    }

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
            switch (pControlParam) // backlight for buttons
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
            pRenderer->DrawTextureIndexed(495, pY, pTexture);
           }
          }
        }
      }
      pRenderer->EndScene();
      pRenderer->Present();
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
  //if ( pRenderer->pRenderD3D )
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
  pRenderer->Release();
  delete window;
  //if ( !DestroyWindow(hWnd) )
  //  GetLastError();
  exit(exitCode);
}

//----- (004647AB) --------------------------------------------------------
void FinalInitialization()
{
  pViewport->SetScreen(viewparams->uSomeX, viewparams->uSomeY, viewparams->uSomeZ, viewparams->uSomeW);
  pViewport->SetFOV(flt_6BE3A0 * 65536.0f);

  //pIndoorCamera = new IndoorCamera;
  //pIndoorCamera->Initialize(65, viewparams->uScreen_BttmR_X - viewparams->uScreen_topL_X + 1,
  //                              viewparams->uScreen_BttmR_Y - viewparams->uScreen_topL_Y + 1);

  InitializeTurnBasedAnimations(&stru_50C198);
  pBitmaps_LOD->_inlined_sub1();
  pSprites_LOD->_inlined_sub1();
  pIcons_LOD->_inlined_sub1();
}
// 6BE3A0: using guessed type float flt_6BE3A0;


//----- (0042F3D6) --------------------------------------------------------
void InitializeTurnBasedAnimations(void *_this)
{
  for (unsigned int i = 0; i < pIconIDs_Turn.size(); ++i)
  {
    char icon_name[32];
    sprintf(icon_name, "turn%u", i);
    pIconIDs_Turn[i] = pIconsFrameTable->FindIcon(icon_name);
    pIconsFrameTable->InitializeAnimation(pIconIDs_Turn[i]);
  }

  uIconID_TurnStop = pIconsFrameTable->FindIcon("turnstop");
  uIconID_TurnHour = pIconsFrameTable->FindIcon("turnhour");
  uIconID_TurnStart = pIconsFrameTable->FindIcon("turnstart");
  uIconID_CharacterFrame = pIconsFrameTable->FindIcon("aframe1");
  uSpriteID_Spell11 = pSpriteFrameTable->FastFindSprite("spell11");

  pIconsFrameTable->InitializeAnimation(uIconID_TurnHour);
  pIconsFrameTable->InitializeAnimation(uIconID_TurnStop);
  pIconsFrameTable->InitializeAnimation(uIconID_TurnStart);
  pIconsFrameTable->InitializeAnimation(uIconID_CharacterFrame);
}

//----- (00464866) --------------------------------------------------------
void DoPrepareWorld(unsigned int bLoading, int _1_fullscreen_loading_2_box)
{
  char *v3; // eax@1
  unsigned int v5; // eax@3
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

//----- (00464E17) --------------------------------------------------------
bool __fastcall CheckMM7CD(char c)
{
  char DstBuf[256] = {0};
  char strCommand[256] = {0}; // [sp+10Ch] [bp-118h]@1
  char Filename[20] = {0}; // [sp+20Ch] [bp-18h]@1

  wchar_t pMagicPath[1024];
  swprintf(pMagicPath, wcslen(L"%C:\\anims\\magic7.vid"), L"%C:\\anims\\magic7.vid", c);
  if (GetFileAttributesW(pMagicPath) == -1)
    return false;

  wsprintfA(strCommand, "open %c: type cdaudio alias CD", c);
  if (!mciSendStringA(strCommand, DstBuf, 255, 0))//Uninitialized memory access

  {
    wsprintfA(strCommand, "info CD UPC wait");
    mciSendStringA(strCommand, DstBuf, 255, 0);
    wsprintfA(strCommand, "close CD");
    mciSendStringA(strCommand, DstBuf, 255, 0);
  }

  memcpy(Filename, "X:\\anims\\magic7.vid", sizeof(Filename));
  *Filename = c;

  FILE* f = fopen(Filename, "rb");
  if (!f)
    return false;

  if (!fseek(f, 0, SEEK_END))
  {
    if (!fseek(f, -100, SEEK_CUR))
      fread(DstBuf, 1, 0x64u, f);

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
    Log::Warning(L"Checking for CD...");
    if (!FindMM7CD(nullptr, &cMM7GameCDDriveLetter))
      return false;
    Log::Warning(L"...done.");
  }


  srand(GetTickCount());

  pEventTimer = Timer::Create();
  pEventTimer->Initialize();
  window = OSWindow::Create(L"Might and Magic® Trilogy", game_width, game_height);

  pRenderer = Render::Create();
  if (!pRenderer)
  {
    Log::Warning(L"Render creation failed");
    return false;
  }
  else
  {
    //bool bWindowMode = ReadWindowsRegistryInt("startinwindow", false);
    //uint uDefaultDevice = ReadWindowsRegistryInt("D3D Device", 1);
    bool bColoredLights = ReadWindowsRegistryInt("Colored Lights", false);
    uint uLevelOfDetail = ReadWindowsRegistryInt("Detail Level", 1);
    uint bTinting = ReadWindowsRegistryInt("Tinting", 1) != 0;

    if (!pRenderer->Initialize(window, bColoredLights, uLevelOfDetail, bTinting))
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

  MM6_Initialize(pMM6IniFile);

  pKeyActionMap = new KeyboardActionMapping;

  OnTimer(1);
  GameUI_StatusBar_UpdateTimedString(1);
  pGame = Game::Create();
  pMouse = pGame->pMouseInstance;


  pIcons_LOD = new LODFile_IconsBitmaps;
  if (!pIcons_LOD->Load("data\\icons.lod", "icons"))
  {
    MessageBoxW(nullptr, L"Some files are missing\n\nPlease Reinstall.",
                         L"Files Missing", MB_ICONEXCLAMATION);
    return false;
  }
  pIcons_LOD->dword_011BA4 = 0;
  
  pEvents_LOD = new LODFile_IconsBitmaps;
  if (!pEvents_LOD->Load("data\\events.lod", "icons"))
  {
    MessageBoxW(nullptr, L"Some files are missing\n\nPlease Reinstall.",
                         L"Files Missing", MB_ICONEXCLAMATION);
    return false;
  }

  InitializeGameText();
  
  pBitmaps_LOD = new LODFile_IconsBitmaps;
  if (!pBitmaps_LOD->Load("data\\bitmaps.lod", "bitmaps"))
  {
    MessageBoxA(nullptr, pGlobalTXT_LocalizationStrings[63],
                         pGlobalTXT_LocalizationStrings[184], MB_ICONEXCLAMATION);
    return false;
  }

  pSprites_LOD = new LODFile_Sprites;
  if (!pSprites_LOD->LoadSprites("data\\sprites.lod"))
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
    void *sft_mm7 = pEvents_LOD->LoadRaw("dsft.bin", 1);
    pSpriteFrameTable = new SpriteFrameTable;
    pSpriteFrameTable->FromFile(sft_mm6, sft_mm7, sft_mm8);
    free(sft_mm6);
    free(sft_mm7);
    free(sft_mm8);
    
    void *tft_mm6 = pIcons_LOD_mm6 ? pIcons_LOD_mm6->LoadRaw("dtft.bin", 1) : nullptr,
         *tft_mm8 = nullptr;
    void *tft_mm7 = pEvents_LOD->LoadRaw("dtft.bin", 1);
    pTextureFrameTable = new TextureFrameTable;
    pTextureFrameTable->FromFile(tft_mm6, tft_mm7, tft_mm8);
    free(tft_mm6);
    free(tft_mm7);
    free(tft_mm8);

    void *tiles_mm6 = pIcons_LOD_mm6 ? pIcons_LOD_mm6->LoadRaw("dtile.bin", 1) : nullptr,
         *tiles_mm8 = nullptr;
    void *tiles_mm7 = pEvents_LOD->LoadRaw("dtile.bin", 1);
    pTileTable = new TileTable;
    pTileTable->FromFile(tiles_mm6, tiles_mm7, tiles_mm8);
    free(tiles_mm6);
    free(tiles_mm7);
    free(tiles_mm8);
    
    void *pft_mm6 = pIcons_LOD_mm6 ? pIcons_LOD_mm6->LoadRaw("dpft.bin", 1) : nullptr,
         *pft_mm8 = nullptr;
    void *pft_mm7 = pEvents_LOD->LoadRaw("dpft.bin", 1);
    pPlayerFrameTable = new PlayerFrameTable;
    pPlayerFrameTable->FromFile(pft_mm6, pft_mm7, pft_mm8);
    free(pft_mm6);
    free(pft_mm7);
    free(pft_mm8);

    void *ift_mm6 = pIcons_LOD_mm6 ? pIcons_LOD_mm6->LoadRaw("dift.bin", 1) : nullptr,
         *ift_mm8 = nullptr;
    void *ift_mm7 = pEvents_LOD->LoadRaw("dift.bin", 1);
    pIconsFrameTable = new IconFrameTable;
    pIconsFrameTable->FromFile(ift_mm6, ift_mm7, ift_mm8);
    free(ift_mm6);
    free(ift_mm7);
    free(ift_mm8);

    void *decs_mm6 = pIcons_LOD_mm6 ? pIcons_LOD_mm6->LoadRaw("ddeclist.bin", 1) : nullptr,
         *decs_mm8 = nullptr;
    void *decs_mm7 = pEvents_LOD->LoadRaw("ddeclist.bin", 1);
    pDecorationList = new DecorationList;
    pDecorationList->FromFile(decs_mm6, decs_mm7, decs_mm8);
    free(decs_mm6);
    free(decs_mm7);
    free(decs_mm8);

    void *objs_mm6 = pIcons_LOD_mm6 ? pIcons_LOD_mm6->LoadRaw("dobjlist.bin", 1) : nullptr,
         *objs_mm8 = nullptr;
    void *objs_mm7 = pEvents_LOD->LoadRaw("dobjlist.bin", 1);
    pObjectList = new ObjectList;
    pObjectList->FromFile(objs_mm6, objs_mm7, objs_mm8);
    free(objs_mm6);
    free(objs_mm7);
    free(objs_mm8);

    void *mons_mm6 = pIcons_LOD_mm6 ? pIcons_LOD_mm6->LoadRaw("dmonlist.bin", 1) : nullptr,
         *mons_mm8 = nullptr;
    void *mons_mm7 = pEvents_LOD->LoadRaw("dmonlist.bin", 1);
    pMonsterList = new MonsterList;
    pMonsterList->FromFile(mons_mm6, mons_mm7, mons_mm8);
    free(mons_mm6);
    free(mons_mm7);
    free(mons_mm8);
    
    void *chests_mm6 = pIcons_LOD_mm6 ? pIcons_LOD_mm6->LoadRaw("dchest.bin", 1) : nullptr,
         *chests_mm8 = nullptr;
    void *chests_mm7 = pEvents_LOD->LoadRaw("dchest.bin", 1);
    pChestList = new ChestList;
    pChestList->FromFile(chests_mm6, chests_mm7, chests_mm8);
    free(chests_mm6);
    free(chests_mm7);
    free(chests_mm8);

    void *overlays_mm6 = pIcons_LOD_mm6 ? pIcons_LOD_mm6->LoadRaw("doverlay.bin", 1) : nullptr,
         *overlays_mm8 = nullptr;
    void *overlays_mm7 = pEvents_LOD->LoadRaw("doverlay.bin", 1);
    pOverlayList = new OverlayList;
    pOverlayList->FromFile(overlays_mm6, overlays_mm7, overlays_mm8);
    free(overlays_mm6);
    free(overlays_mm7);
    free(overlays_mm8);

    void *sounds_mm6 = pIcons_LOD_mm6 ? pIcons_LOD_mm6->LoadRaw("dsounds.bin", 1) : nullptr,
         *sounds_mm8 = nullptr;
    void *sounds_mm7 = pEvents_LOD->LoadRaw("dsounds.bin", 1);
    pSoundList = new SoundList;
    pSoundList->FromFile(sounds_mm6, sounds_mm7, sounds_mm8);
    free(sounds_mm6);
    free(sounds_mm7);
    free(sounds_mm8);
  }



  
  if (dword_6BE368_debug_settings_2 & DEBUG_SETTINGS_RUN_IN_WIDOW)
  {
    //window->SetWindowedMode(game_width, game_height);
    pRenderer->SwitchToWindow();
  }
  else
  {
    __debugbreak(); // Nomad
    window->SetFullscreenMode();
    pRenderer->InitializeFullscreen();
  }

  uSoundVolumeMultiplier = min(9, ReadWindowsRegistryInt("soundflag", 9));
  uMusicVolimeMultiplier = min(9, ReadWindowsRegistryInt("musicflag", 9));
  uVoicesVolumeMultiplier = min(9, ReadWindowsRegistryInt("CharVoices", 9));
  bShowDamage = ReadWindowsRegistryInt("ShowDamage", 1) != 0;

  uGammaPos = min(4, ReadWindowsRegistryInt("GammaPos", 4));
  pGame->pGammaController->Initialize(uGammaPos * 0.1 + 0.6);

  if ( ReadWindowsRegistryInt("Bloodsplats", 1) )
    pGame->uFlags2 |= GAME_FLAGS_2_DRAW_BLOODSPLATS;
  else
    pGame->uFlags2 &= ~GAME_FLAGS_2_DRAW_BLOODSPLATS;

  uTurnSpeed = ReadWindowsRegistryInt("TurnDelta", 3);

  if (!bNoSound)
    pAudioPlayer->Initialize();

  pMediaPlayer = new Media::MPlayer();
  pMediaPlayer->Initialize(window);

  dword_6BE364_game_settings_1 |= GAME_SETTINGS_4000;

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
  pMouse->Initialize(window);

  pItemsTable = new ItemsTable;
  pItemsTable->Initialize();

  //pBitmaps_LOD->can_load_hardware_sprites = 1;
  //pBitmaps_LOD->SetupPalettes(pRenderer->uTargetRBits, pRenderer->uTargetGBits, pRenderer->uTargetBBits);
  pBitmaps_LOD->SetupPalettes(5, 6, 5);
  //pIcons_LOD->SetupPalettes(pRenderer->uTargetRBits, pRenderer->uTargetGBits, pRenderer->uTargetBBits);
  pIcons_LOD->SetupPalettes(5, 6, 5);
  //pPaletteManager->SetColorChannelInfo(pRenderer->uTargetRBits, pRenderer->uTargetGBits, pRenderer->uTargetBBits);
  pPaletteManager->SetColorChannelInfo(5, 6, 5);

  pPaletteManager->SetMistColor(128, 128, 128);
  pPaletteManager->RecalculateAll();
  //pSprites_LOD->can_load_hardware_sprites = 1;
  pObjectList->InitializeSprites();
  pOverlayList->InitializeSprites();

  if (!bNoSound)
    pSoundList->Initialize();


  for (uint i = 0; i < 4; ++i)
  {
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

    pUIAnims[i]->uIconID = pIconsFrameTable->FindIcon(pUIAnimNames[i]);
    pIconsFrameTable->InitializeAnimation(pUIAnims[i]->uIconID);

    pUIAnims[i]->uAnimLength = 0;
    pUIAnims[i]->uAnimTime = 0;
    pUIAnims[i]->x = _4E98D0[i][0];
    pUIAnims[i]->y = _4E98D0[i][2];
  }

  for (unsigned int i = 0; i < pObjectList->uNumObjects; ++i)
  {
    pObjectList->pObjects[i].uParticleTrailColor = pObjectList->pObjects[i].uParticleTrailColorB |
                                                   ((unsigned int)pObjectList->pObjects[i].uParticleTrailColorG << 8) |
                                                   ((unsigned int)pObjectList->pObjects[i].uParticleTrailColorR << 16);
  }

  MainMenuUI_Create();
  pGame->pStru6Instance->LoadAnimations();

  for (uint i = 0; i < 7; ++i)
  {
    char container_name[64];
    sprintf(container_name, "HDWTR%03u", i);
    pRenderer->pHDWaterBitmapIDs[i] = pBitmaps_LOD->LoadTexture(container_name);
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
  static_assert(sizeof(MovieHeader) == 44, "Wrong type size");
  static_assert(sizeof(SoundDesc_mm6) == 112, "Wrong type size");
  static_assert(sizeof(SoundDesc) == 120, "Wrong type size");
  static_assert(sizeof(OverlayDesc) == 8, "Wrong type size");
  static_assert(sizeof(ChestDesc) == 36, "Wrong type size");
  static_assert(sizeof(ObjectDesc_mm6) == 52, "Wrong type size");
  static_assert(sizeof(ObjectDesc) == 56, "Wrong type size");
  static_assert(sizeof(DecorationDesc) == 84, "Wrong type size");
  static_assert(sizeof(IconFrame) == 32, "Wrong type size");
  static_assert(sizeof(PlayerFrame) == 10, "Wrong type size");
  static_assert(sizeof(TextureFrame) == 20, "Wrong type size");
  static_assert(sizeof(SpriteFrame) == 60, "Wrong type size");
  static_assert(sizeof(RenderVertexSoft) == 0x30, "Wrong type size");
  static_assert(sizeof(RenderBillboard) == 0x34, "Wrong type size");
  static_assert(sizeof(Texture) == 0x48, "Wrong type size");
  static_assert(sizeof(RGBTexture) == 0x28, "Wrong type size");
  //static_assert(sizeof(LODFile_IconsBitmaps) == 0x11BB8 + 4, "Wrong type size"); // + virtual dtor ptr
  static_assert(sizeof(AudioPlayer) == 0xC84, "Wrong type size");
  static_assert(sizeof(SoundDesc) == 0x78, "Wrong type size");
  static_assert(sizeof(stru339_spell_sound) == 0xAFD8, "Wrong type size");
  //static_assert(sizeof(VideoPlayer) == 0x108 + 4, "Wrong type size");
  static_assert(sizeof(MovieHeader) == 0x2C, "Wrong type size");
  static_assert(sizeof(DecorationDesc) == 0x54, "Wrong type size");
  static_assert(sizeof(ObjectDesc) == 0x38, "Wrong type size");
  static_assert(sizeof(OverlayDesc) == 0x8, "Wrong type size");
  static_assert(sizeof(ChestDesc) == 0x24, "Wrong type size");
  static_assert(sizeof(TileDesc) == 0x1A, "Wrong type size");
  static_assert(sizeof(MonsterDesc_mm6) == 148, "Wrong type size");
  static_assert(sizeof(MonsterDesc) == 152, "Wrong type size");
  static_assert(sizeof(Timer) == 0x28, "Wrong type size");
  static_assert(sizeof(OtherOverlay) == 0x14, "Wrong type size");
  static_assert(sizeof(ItemGen) == 0x24, "Wrong type size");
  static_assert(sizeof(SpriteObject) == 0x70, "Wrong type size");
  static_assert(sizeof(ItemDesc) == 0x30, "Wrong type size");
  static_assert(sizeof(ItemsTable) == 0x117A0, "Wrong type size");
  static_assert(sizeof(Chest) == 0x14CC, "Wrong type size");
  static_assert(sizeof(MapInfo) == 0x44, "Wrong type size");
  static_assert(sizeof(SpellInfo) == 0x24, "Wrong type size");
  static_assert(sizeof(SpellData) == 0x14, "Wrong type size");
  static_assert(sizeof(SpellBuff) == 0x10, "Wrong type size");
  static_assert(sizeof(AIDirection) == 0x1C, "Wrong type size");
  static_assert(sizeof(ActorJob) == 0xC, "Wrong type size");
  static_assert(sizeof(Actor) == 0x344, "Wrong type size");
  static_assert(sizeof(LevelDecoration) == 0x20, "Wrong type size");
  static_assert(sizeof(KeyboardActionMapping) == 0x20C, "Wrong type size");
  static_assert(sizeof(UIAnimation) == 0xD, "Wrong type size");
  static_assert(sizeof(SpawnPointMM7) == 0x18, "Wrong type size");
  static_assert(sizeof(ODMFace) == 0x134, "Wrong type size");
  static_assert(sizeof(BSPNode) == 0x8, "Wrong type size");
  static_assert(sizeof(BSPModel) == 0xBC, "Wrong type size");
  static_assert(sizeof(OutdoorLocation) == 0x1C28C, "Wrong type size");
  static_assert(sizeof(BLVFace) == 0x60, "Wrong type size");
  static_assert(sizeof(BLVFaceExtra) == 0x24, "Wrong type size");
  static_assert(sizeof(BLVSector) == 0x74, "Wrong type size");
  static_assert(sizeof(BLVLightMM7) == 0x10, "Wrong type size");
  static_assert(sizeof(BLVDoor) == 0x50, "Wrong type size");
  static_assert(sizeof(IndoorLocation) == 0x690, "Wrong type size");
  //static_assert(sizeof(ODMRenderParams) == 0x74, "Wrong type size");
  static_assert(sizeof(Mouse) == 0x114, "Wrong type size");
  static_assert(sizeof(Particle_sw) == 0x68, "Wrong type size");
  static_assert(sizeof(Particle) == 0x68, "Wrong type size");
  static_assert(sizeof(ParticleEngine) == 0xE430, "Wrong type size");
  static_assert(sizeof(Lightmap) == 0xC1C, "Wrong type size");
  static_assert(sizeof(LightmapBuilder) == 0x3CBC38, "Wrong type size");
  static_assert(sizeof(Vis_SelectionList) == 0x2008, "Wrong type size");
  static_assert(sizeof(Vis) == 0x20D0, "Wrong type size");
  static_assert(sizeof(PlayerBuffAnim) == 0x10, "Wrong type size");
  static_assert(sizeof(ProjectileAnim) == 0x1C, "Wrong type size");
  static_assert(sizeof(stru6) == 0x5F8, "Wrong type size");
  static_assert(sizeof(IndoorCameraD3D_Vec3) == 0x10, "Wrong type size");
  static_assert(sizeof(IndoorCameraD3D_Vec4) == 0x18, "Wrong type size"); //should be 14 (10 vec3 + 4 vdtor)  but 18 coz of his +4 from own vdtor, but it is odd since vdtor already present from vec3
  //static_assert(sizeof(IndoorCameraD3D) == 0x1A1384, "Wrong type size");
  static_assert(sizeof(StationaryLight) == 0xC, "Wrong type size");
  static_assert(sizeof(LightsStack_StationaryLight_) == 0x12C8, "Wrong type size");
  static_assert(sizeof(MobileLight) == 0x12, "Wrong type size");
  static_assert(sizeof(LightsStack_MobileLight_) == 0x1C28, "Wrong type size");
  static_assert(sizeof(Game) == 0xE78, "Wrong type size");
  static_assert(sizeof(stru141_actor_collision_object) == 0xA8, "Wrong type size");
  static_assert(sizeof(ActionQueue) == 0x7C, "Wrong type size");
  static_assert(sizeof(NPCData) == 0x4C, "Wrong type size");
  static_assert(sizeof(NPCStats) == 0x17FFC, "Wrong type size");
  static_assert(sizeof(BspRenderer) == 0x53740, "Wrong type size");
  static_assert(sizeof(PaletteManager) == 0x267AF0, "Wrong type size");
  static_assert(sizeof(ViewingParams) == 0x26C, "Wrong type size");
  //static_assert(sizeof(IndoorCamera) == 0x50, "Wrong type size");
  static_assert(sizeof(Bloodsplat) == 0x28, "Wrong type size");
  static_assert(sizeof(BloodsplatContainer) == 0xA0C, "Wrong type size");
  static_assert(sizeof(TrailParticle) == 0x18, "Wrong type size");
  static_assert(sizeof(EventIndex) == 0xC, "Wrong type size");
  static_assert(sizeof(_2devent) == 0x34, "Wrong type size");
  static_assert(sizeof(MapsLongTimer) == 0x20, "Wrong type size");
  static_assert(sizeof(SavegameHeader) == 0x64, "Wrong type size");
  static_assert(sizeof(SavegameList) == 0x3138, "Wrong type size");
  static_assert(sizeof(StorylineText) == 0x160, "Wrong type size");
  static_assert(sizeof(FactionTable) == 0x1EF1, "Wrong type size");
  static_assert(sizeof(Decal) == 0xC20, "Wrong type size");
  static_assert(sizeof(DecalBuilder) == 0x30C038, "Wrong type size");
  static_assert(sizeof(MonsterInfo) == 0x58, "Wrong type size");
  static_assert(sizeof(MonsterStats) == 0x5BA0, "Wrong type size");
  static_assert(sizeof(RenderD3D) == 0x148, "Wrong type size");
  //  static_assert(sizeof(Render) == 0x129844, "Wrong type size");
  static_assert(sizeof(Player) == 0x1B3C, "Wrong type size");
  static_assert(sizeof(PartyTimeStruct) == 0x678, "Wrong type size");
  static_assert(sizeof(Party) == 0x16238, "Wrong type size");
  static_assert(sizeof(GUIButton) == 0xBC, "Wrong type size");
  static_assert(sizeof(GUIWindow) == 0x54, "Wrong type size");
  static_assert(sizeof(GUIProgressBar) == 0x1B8, "Wrong type size");
  static_assert(sizeof(GUIFont) == 0x1020, "Wrong type size");
  // static_assert(sizeof(stru262_TurnBased) == 0x40, "Wrong type size");
  static_assert(sizeof(ArcomageGame) == 0xFB, "Wrong type size");
  static_assert(sizeof(CastSpellInfo) == 0x14, "Wrong type size");
  static_assert(sizeof(ArcomageCard) == 0x6C, "Wrong type size");
  static_assert(sizeof(stru320) == 0x3FC, "Wrong type size");
  static_assert(sizeof(TravelInfo) == 0x20, "Wrong type size");
  static_assert(sizeof(stru336) == 0x798, "Wrong type size");
  static_assert(sizeof(Vec3_short_) == 6, "Wrong type size");
  static_assert(sizeof(BLVFace) == 96, "Wrong type size");
  static_assert(sizeof(BLVFaceExtra) == 36, "Wrong type size");
  static_assert(sizeof(BLVSector) == 116, "Wrong type size");
  static_assert(sizeof(LevelDecoration) == 32, "Wrong type size");
  static_assert(sizeof(BLVLightMM7) == 16, "Wrong type size");
  static_assert(sizeof(BSPNode) == 8, "Wrong type size");
  static_assert(sizeof(SpawnPointMM7) == 24, "Wrong type size");
  static_assert(sizeof(DDM_DLV_Header) == 40, "Wrong type size");
  static_assert(sizeof(Actor) == 836, "Wrong type size");
  static_assert(sizeof(SpriteObject) == 112, "Wrong type size");
  static_assert(sizeof(Chest) == 5324, "Wrong type size");
  static_assert(sizeof(stru123) == 0xC8, "Wrong type size");
  static_assert(sizeof(BLVMapOutline) == 12, "Wrong type size");
  static_assert(sizeof(LODSprite) == 0x28, "Wrong type size");
}

bool new_sky = false;            //new sky(need texture)
int max_flight_height = 4000;    //maximum altitude




//----- (00462C94) --------------------------------------------------------
bool MM_Main(const wchar_t *pCmdLine)
{
  IntegrityTest();

  char test[1024];
  sprintfex(test, "^Pi[%s]: çíàõàð^R[ü;êà;]", "Çîëòàí");

  lua = new LuaVM;
  lua->Initialize();

  bool bNoMargareth = false;
  if (pCmdLine && *pCmdLine)
  {
    //if (wcsstr(pCmdLine, L"-usedefs"))
    //  bDebugResouces = 1;
    if (wcsstr(pCmdLine, L"-window"))
      dword_6BE368_debug_settings_2 |= DEBUG_SETTINGS_RUN_IN_WIDOW;

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
      dword_6BE364_game_settings_1 |= GAME_SETTINGS_NO_HOUSE_ANIM;
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

  //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF );//Ritor1: for memory test

  if (!MM7_Initialize(640, 480))
  {
    Log::Warning(L"MM init: failed");
    pGame->Deinitialize();
    return 1;
  }

  pEventTimer->Pause();

  SetUserInterface(PartyAlignment_Neutral, false);
  ShowLogoVideo();
  //ShowIntroVideo_and_LoadingScreen();
  WriteWindowsRegistryInt("Ran once", 1);
  dword_6BE364_game_settings_1 |= GAME_SETTINGS_4000;
  pGame->InitializeGammaController();
  SecondaryInitialization();
  pRenderer->SetRasterClipRect(0, 0, window->GetWidth() - 1, window->GetHeight() - 1);
  FinalInitialization();

  //Ritor1: include
  MMT_MainMenu_Loop();

  Log::Warning(L"MM: entering main loop");
  while ( 1 )
  {
    MainMenuWindow* main_menu_window = MainMenuWindow::Create();
    window->AddControl(main_menu_window);
    MainMenu_Loop();
    uGameState = GAME_STATE_PLAYING;
    while ( 1 )
    {
      if (uGameState == GAME_FINISHED || GetCurrentMenuID() == MENU_EXIT_GAME)
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
        pMediaPlayer->PlayFullscreenMovie(MOVIE_Emerald, true);
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
          pMouse->ChangeActivation(0);
          pParty->Reset();
          pParty->CreateDefaultParty(1);

          __debugbreak();
          /*extern void CreateDefaultBLVLevel();
          CreateDefaultBLVLevel();

          OPENFILENAMEA ofn;
          if ( !GetOpenFileNameA((LPOPENFILENAMEA)&ofn) )
          {
            pMouse->ChangeActivation(1);
            break;
          }
          _chdir("..\\");
          strcpy(pCurrentMapName, ofn.lpstrFileTitle);*/
          pMouse->ChangeActivation(1);
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
      AIL_redbook_track_info(pAudioPlayer->hAILRedbook, 14, &startms, &end_ms);
      AIL_redbook_play(pAudioPlayer->hAILRedbook, startms + 1, end_ms);
    }
  }
  //lua_close(L);
  pGame->Deinitialize();
  return 1;
}


//----- (00466082) --------------------------------------------------------
void MM6_Initialize(const wchar_t *pIniFilename)
{
  size_t v2; // eax@31
  size_t v3; // ebx@32
  size_t v4; // edi@36
  char pDefaultGroundTexture[16]; // [sp+FCh] [bp-8Ch]@32
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
    dword_6BE368_debug_settings_2 |= DEBUG_SETTINGS_NO_ACTORS;
  if (ReadWindowsRegistryInt("startinwindow", 0))
    dword_6BE368_debug_settings_2 |= DEBUG_SETTINGS_RUN_IN_WIDOW;
  if (GetPrivateProfileIntW(L"debug", L"showFR", 0, pIniFilename))
    dword_6BE368_debug_settings_2 |= DEBUG_SETTINGS_0002_SHOW_FR;
  if (GetPrivateProfileIntW(L"debug", L"nodamage", 0, pIniFilename))
    dword_6BE368_debug_settings_2 |= DEBUG_SETTINGS_NO_DAMAGE;
  if (GetPrivateProfileIntW(L"debug", L"nodecoration", 0, pIniFilename))
    dword_6BE368_debug_settings_2 |= DEBUG_SETTINGS_NO_DECORATIONS;

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
  pODMRenderParams->outdoor_no_mist = GetPrivateProfileIntW(L"debug", L"noMist", 0, pIniFilename);
  pODMRenderParams->bNoSky          = GetPrivateProfileIntW(L"outdoor", L"nosky", 0, pIniFilename);
  pODMRenderParams->bDoNotRenderDecorations = GetPrivateProfileIntW(L"render", L"nodecorations", 0, pIniFilename);
  pODMRenderParams->outdoor_no_wavy_water   = GetPrivateProfileIntW(L"outdoor", L"nowavywater", 0, pIniFilename);
  outdoor_grid_band_1 = GetPrivateProfileIntW(L"outdoor", L"gridband1", 10, pIniFilename);
  outdoor_grid_band_2 = GetPrivateProfileIntW(L"outdoor", L"gridband2", 15, pIniFilename);
  outdoor_grid_band_3 = GetPrivateProfileIntW(L"outdoor", L"gridband3", 25, pIniFilename);
  pODMRenderParams->terrain_gamma  = GetPrivateProfileIntW(L"outdoor", L"ter_gamma", 0, pIniFilename);
  pODMRenderParams->building_gamme = GetPrivateProfileIntW(L"outdoor", L"bld_gamma", 0, pIniFilename);
  pODMRenderParams->shading_dist_shade     = GetPrivateProfileIntW(L"shading", L"dist_shade", 2048, pIniFilename);
  pODMRenderParams->shading_dist_shademist = GetPrivateProfileIntW(L"shading", L"dist_shademist", 4096, pIniFilename);

  pODMRenderParams->shading_dist_mist = GetPrivateProfileIntW(L"shading", L"dist_mist", 0x2000, pIniFilename);//drawing dist 0x2000

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

  MM7Initialization();
}

//----- (004666D5) --------------------------------------------------------
void MM7Initialization()
{
  if ( uCurrentlyLoadedLevelType == LEVEL_Outdoor )
  {
    /*if (byte_6BE388_graphicsmode == 0)
    {
      outdoor_grid_band_1 = 10;
      outdoor_grid_band_2 = 15;
      outdoor_grid_band_3 = 20;
      pODMRenderParams->shading_dist_mist = 8192;
      pODMRenderParams->bNoSky = false;
      LOBYTE(viewparams->field_20) = 0;
    }*/
    pODMRenderParams->shading_dist_shade = 2048;
    pODMRenderParams->terrain_gamma = 0;
    pODMRenderParams->building_gamme = 0;
    pODMRenderParams->shading_dist_shademist = 4096;
    pODMRenderParams->outdoor_no_wavy_water = 0;
    //_47F4D3_initialize_terrain_bezier_stuff(outdoor_grid_band_1, outdoor_grid_band_2, outdoor_grid_band_3);
    {
      pODMRenderParams->outdoor_grid_band_3 = outdoor_grid_band_3;
      pODMRenderParams->uPickDepth = outdoor_grid_band_3 * 512;
    }
  }
  else
    LOBYTE(viewparams->field_20) = 0;
  pParty->uFlags |= 2;
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
    case MENU_MMT_MAIN_MENU: return L"MENU_MMT_MAIN_MENU";
    default:                return L"unk";
  };
};
void SetCurrentMenuID(MENU_STATE uMenu)
{
  sCurrentMenuID = uMenu;

  Log::Warning(L"CurrentMenu = %s \n", MENU_STATE_to_string(uMenu));
}

//----- (00466CA0) --------------------------------------------------------
MENU_STATE GetCurrentMenuID()
{
  return sCurrentMenuID;
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
  if (dword_6BE364_game_settings_1 & GAME_SETTINGS_0080_SKIP_USER_INPUT_THIS_FRAME)
  {
    dword_6BE364_game_settings_1 &= ~GAME_SETTINGS_0080_SKIP_USER_INPUT_THIS_FRAME;
    return;
  }

    if (uCurrentlyLoadedLevelType == LEVEL_Indoor)
      BLV_UpdateUserInputAndOther();
    else if (uCurrentlyLoadedLevelType == LEVEL_Outdoor)
        ODM_UpdateUserInputAndOther();

    area_of_effect__damage_evaluate();
}

//----- (0046BFFA) --------------------------------------------------------
bool __fastcall _46BFFA_check_object_intercept(unsigned int uLayingItemID, signed int a2)
{
  ObjectDesc *object; // ebx@1
  unsigned int v8; // eax@19
  signed int v10; // ebx@19
  char *v11; // edx@20
  unsigned __int16 v12; // ax@23
  int v13; // eax@27
  int v16; // eax@36
  __int16 v18; // di@37
  signed int v19; // edx@37
  unsigned __int16 v22; // ax@41
  signed int v24; // ebx@46
  char *v25; // edx@47
  signed int v34; // edx@65
  unsigned __int16 v36; // ax@69
  int v37; // ST14_4@72
  int v38; // eax@72
  int v39; // ST10_4@72
  int v40; // ST0C_4@72
  unsigned __int8 v44; // zf@79
  int v47; // eax@81
  signed int v52; // ebx@93
  signed int v56; // ebx@98
  unsigned __int16 v58; // ax@102
  unsigned __int16 v59; // ax@107
  signed int v61; // ebx@107
  unsigned __int16 v63; // ax@111
  int v64; // ebx@114
  signed int v65; // eax@114
  signed int v69; // ebx@124
  unsigned __int16 v71; // ax@128
  unsigned int v72; // ebx@131
  int v78; // eax@133
  signed int v81; // edx@140
  unsigned __int16 v83; // ax@144
  signed int v86; // ebx@151
  unsigned __int16 v88; // ax@155
  unsigned int v89; // eax@158
  int v90; // ST34_4@159
  int v91; // eax@159
  unsigned int v92; // eax@163
  unsigned __int16 v95; // ax@181
  unsigned __int16 v96; // ax@184
  int v97; // eax@185
  char v100; // ST18_1@198
  int v102; // eax@198
  signed int v106; // eax@208
  unsigned int v107; // edx@220
  signed int v108; // ebx@225
  signed int v110; // ebx@234
  unsigned __int16 v112; // ax@238
  unsigned __int16 v113; // si@241
  int v114; // eax@242
  int v115; // eax@245
  signed int v119; // ebx@251
  unsigned __int16 v121; // ax@255
  int v124; // eax@267
  int v125; // [sp-20h] [bp-4Ch]@28
  char v132; // [sp-8h] [bp-34h]@131
  char v134; // [sp-4h] [bp-30h]@131
  signed int v135; // [sp-4h] [bp-30h]@217
  int v136; // [sp+Ch] [bp-20h]@208
  int v137; // [sp+10h] [bp-1Ch]@208
  signed int v138; // [sp+14h] [bp-18h]@207
  signed int v139; // [sp+18h] [bp-14h]@208
  signed int v141; // [sp+1Ch] [bp-10h]@117
  unsigned int v142; // [sp+1Ch] [bp-10h]@158
  signed int v143; // [sp+1Ch] [bp-10h]@172
  signed int v146; // [sp+20h] [bp-Ch]@60
  int v147; // [sp+20h] [bp-Ch]@72
  signed int v148; // [sp+20h] [bp-Ch]@158
  unsigned __int16 v150; // [sp+20h] [bp-Ch]@208
  signed int v152; // [sp+24h] [bp-8h]@208

  object = &pObjectList->pObjects[pSpriteObjects[uLayingItemID].uObjectDescID];
  //v151 = PID_TYPE(a2);
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
    if ( pSpriteObjects[uLayingItemID].uAttributes & 4 )
    {
      --pTurnEngine->pending_actions;
      pSpriteObjects[uLayingItemID].uAttributes &= 0xFFFB;
    }
  }
  if ( PID_TYPE(a2) == OBJECT_BModel && PID_TYPE(pSpriteObjects[uLayingItemID].spell_caster_pid) != OBJECT_Player)
      {
      if (PID_ID(pSpriteObjects[uLayingItemID].spell_caster_pid) < 500)  //bugfix  PID_ID(v2->spell_caster_pid)==1000
         pActors[PID_ID(pSpriteObjects[uLayingItemID].spell_caster_pid)].uAttributes |= 0x40000;
      }

  //v6 = v2->uType;
  //v7 = v2->uType;

  switch (pSpriteObjects[uLayingItemID].uType)
  {

  case 1060:
  case 2030:
  case 9010:
	{
	  //v9 = 0;
	  if (PID_TYPE(a2) == 6 || PID_TYPE(a2) == 5 || !PID_TYPE(a2))
		  return 1;
	  if (PID_TYPE(a2) != 2)
	  {
		  sub_43A97E(uLayingItemID, a2);
		  ++pSpriteObjects[uLayingItemID].uType;
		  v95 = 0;
		  for (v52 = 0; v52 < (signed int)pObjectList->uNumObjects; ++v52)
		  {
			  if (pSpriteObjects[uLayingItemID].uType == pObjectList->pObjects[v52].uObjectID)
				  v95 = v52;
		  }
		  pSpriteObjects[uLayingItemID].uObjectDescID = v95;
		  if (!v95)
			  SpriteObject::OnInteraction(uLayingItemID);
		  pSpriteObjects[uLayingItemID].vVelocity.z = 0;
		  pSpriteObjects[uLayingItemID].vVelocity.y = 0;
		  pSpriteObjects[uLayingItemID].vVelocity.x = 0;
		  pSpriteObjects[uLayingItemID].uSpriteFrameID = 0;
		  if (!pSpriteObjects[uLayingItemID].uSoundID)
			  v97 = 0;
		  else
			  v97 = (signed __int16)pSpriteObjects[uLayingItemID].uSoundID + 4;
		  v124 = 8 * uLayingItemID;
		  LOBYTE(v124) = v124 | 2;
		  v125 = word_4EE088_sound_ids[pSpriteObjects[uLayingItemID].spell_id - 1] + 1;
		  pAudioPlayer->PlaySound((SoundID)v125, v124, 0, -1, 0, v97, 0, 0);
		  return 0;
	  }
	  pSpriteObjects[uLayingItemID].uType = pSpriteObjects[uLayingItemID].uType + 1;
	  v121 = 0;
	  for (v119 = 0; v119 < (signed int)pObjectList->uNumObjects; ++v119)
	  {
		  if (pSpriteObjects[uLayingItemID].uType == pObjectList->pObjects[v119].uObjectID)
			  v121 = v119;
	  }
	  pSpriteObjects[uLayingItemID].uObjectDescID = v121;
	  if (!v121)
		  SpriteObject::OnInteraction(uLayingItemID);
	  pSpriteObjects[uLayingItemID].vVelocity.z = 0;
	  pSpriteObjects[uLayingItemID].vVelocity.y = 0;
	  pSpriteObjects[uLayingItemID].vVelocity.x = 0;
	  pSpriteObjects[uLayingItemID].uSpriteFrameID = 0;
	  v13 = 8 * uLayingItemID;
	  LOBYTE(v13) = PID(OBJECT_Item, uLayingItemID);
	  pAudioPlayer->PlaySound(SOUND_8, v13, 0, -1, 0, 0, 0, 0);
	  return 0;
  }

  
  case 500:
  case 505:
  case 510:
  case 515:
  case 520:
  case 525:
  case 530:
  case 535:
  case 540:
  {
	  sub_43A97E(uLayingItemID, a2);
	  ++pSpriteObjects[uLayingItemID].uType;
	  v12 = 0;
	  for (v10 = 0; v10 < (signed int)pObjectList->uNumObjects; ++v10)
	  {
		  if (pSpriteObjects[uLayingItemID].uType == pObjectList->pObjects[v10].uObjectID)
			  v12 = v10;
	  }
	  pSpriteObjects[uLayingItemID].uObjectDescID = v12;
	  if (!v12)
		  SpriteObject::OnInteraction(uLayingItemID);
	  pSpriteObjects[uLayingItemID].vVelocity.z = 0;
	  pSpriteObjects[uLayingItemID].vVelocity.y = 0;
	  pSpriteObjects[uLayingItemID].vVelocity.x = 0;
	  pSpriteObjects[uLayingItemID].uSpriteFrameID = 0;
	  if (pSpriteObjects[uLayingItemID].uType == 555)
	  {
		  v13 = 8 * uLayingItemID;
		  LOBYTE(v13) = PID(OBJECT_Item, uLayingItemID);
		  pAudioPlayer->PlaySound(SOUND_8, v13, 0, -1, 0, 0, 0, 0);
	  }
	  return 0;
  }
  
  case 545:
  case 550:
  {
	  if (pSpriteObjects[uLayingItemID].stru_24.uItemID != 405 && pSpriteObjects[uLayingItemID].stru_24.uSpecEnchantmentType != 3)
	  {
		  pSpriteObjects[uLayingItemID].vVelocity.z = 0;
		  pSpriteObjects[uLayingItemID].vVelocity.y = 0;
		  pSpriteObjects[uLayingItemID].vVelocity.x = 0;
		  pSpriteObjects[uLayingItemID].uSpriteFrameID = 0;
		  sub_43A97E(uLayingItemID, a2);
		  SpriteObject::OnInteraction(uLayingItemID);
		  if (pSpriteObjects[uLayingItemID].uSoundID == 0)
			  v16 = 0;
		  else
			  v16 = (signed __int16)pSpriteObjects[uLayingItemID].uSoundID + 4;
		  v124 = 8 * uLayingItemID;
		  LOBYTE(v124) = v124 | 2;
		  v125 = word_4EE088_sound_ids[pSpriteObjects[uLayingItemID].spell_id] + 1;
		  pAudioPlayer->PlaySound((SoundID)v125, v124, 0, -1, 0, v16, 0, 0);
		  return 0;
	  }
	  v18 = 0;
	  pSpriteObjects[uLayingItemID].uType = 600;
	  v22 = 0;
	  for (v19 = 0; v19 < (signed int)pObjectList->uNumObjects; ++v19)
	  {
		  if (pSpriteObjects[uLayingItemID].uType == pObjectList->pObjects[v19].uObjectID)
			  v22 = v19;
	  }
	  pSpriteObjects[uLayingItemID].uObjectDescID = v22;
	  if (!v22)
		  SpriteObject::OnInteraction(uLayingItemID);
	  pSpriteObjects[uLayingItemID].vVelocity.z = v18;
	  pSpriteObjects[uLayingItemID].vVelocity.y = v18;
	  pSpriteObjects[uLayingItemID].vVelocity.x = v18;
	  pSpriteObjects[uLayingItemID].uSpriteFrameID = v18;
	  v12 = 0;
	  for (v10; v10 < (signed int)v8; ++v10)
	  {
		  v11 += 56;
		  if (pSpriteObjects[uLayingItemID].uType != *(short *)v11)
			  v12 = v10;
	  }
	  pSpriteObjects[uLayingItemID].uObjectDescID = v12;
	  if (!v12)
		  SpriteObject::OnInteraction(uLayingItemID);
	  v44 = pSpriteObjects[uLayingItemID].uType == 555;
	  pSpriteObjects[uLayingItemID].vVelocity.z = 0;
	  pSpriteObjects[uLayingItemID].vVelocity.y = 0;
	  pSpriteObjects[uLayingItemID].vVelocity.x = 0;
	  pSpriteObjects[uLayingItemID].uSpriteFrameID = 0;
	  if (!v44)
	  {
		  v13 = 8 * uLayingItemID;
		  LOBYTE(v13) = PID(OBJECT_Item, uLayingItemID);
		  pAudioPlayer->PlaySound(SOUND_8, v13, 0, -1, 0, 0, 0, 0);
		  return 0;
	  }
	  return 0;
  }
  
  case 600:
  {
			  pSpriteObjects[uLayingItemID].uType = 601;
			  v36 = 0;
			  for (v34 = 0; v34 < (signed int)pObjectList->uNumObjects; ++v34)
			  {
				  if (pSpriteObjects[uLayingItemID].uType == pObjectList->pObjects[v34].uObjectID)
					  v36 = v34;
			  }
			  pSpriteObjects[uLayingItemID].uObjectDescID = v36;
			  if (!v36)
				  SpriteObject::OnInteraction(uLayingItemID);
			  v37 = pSpriteObjects[uLayingItemID].vPosition.z;
			  pSpriteObjects[uLayingItemID].vVelocity.z = 0;
			  v38 = 8 * uLayingItemID;
			  v39 = pSpriteObjects[uLayingItemID].vPosition.y;
			  LOBYTE(v38) = PID(OBJECT_Item, uLayingItemID);
			  pSpriteObjects[uLayingItemID].vVelocity.y = 0;
			  pSpriteObjects[uLayingItemID].vVelocity.x = 0;
			  v40 = pSpriteObjects[uLayingItemID].vPosition.x;
			  pSpriteObjects[uLayingItemID].uSpriteFrameID = 0;
			  v147 = v38;
			  AttackerInfo.Add(v38, 512, v40, v39, v37, 0, 0);
			  if (object->uFlags & OBJECT_DESC_TRIAL_PARTICLE)
				  trail_particle_generator.GenerateTrailParticles(pSpriteObjects[uLayingItemID].vPosition.x, pSpriteObjects[uLayingItemID].vPosition.y, pSpriteObjects[uLayingItemID].vPosition.z, object->uParticleTrailColor);
			  pAudioPlayer->PlaySound(SOUND_8, v147, 0, -1, 0, 0, 0, 0);
			  return 0;
  }
  
  case 1010:
  case 1100:
  case 2060:
  case 3010:
  case 3030:
  case 3060:
  case 4000:
  case 4030:
  case 4050:
  case 4100:
  case 6010:
  case 6090:
  {
	sub_43A97E(uLayingItemID, a2);
	++pSpriteObjects[uLayingItemID].uType;
	v95 = 0;
	for (v52 = 0; v52 < (signed int)pObjectList->uNumObjects; ++v52)
	{
		if (pSpriteObjects[uLayingItemID].uType == pObjectList->pObjects[v52].uObjectID)
			v95 = v52;
	}
	pSpriteObjects[uLayingItemID].uObjectDescID = v95;
	if (!v95)
		SpriteObject::OnInteraction(uLayingItemID);
	v96 = pSpriteObjects[uLayingItemID].uSoundID;
	pSpriteObjects[uLayingItemID].vVelocity.z = 0;
	pSpriteObjects[uLayingItemID].vVelocity.y = 0;
	pSpriteObjects[uLayingItemID].vVelocity.x = 0;
	pSpriteObjects[uLayingItemID].uSpriteFrameID = 0;
	if (!v96)
		v97 = 0;
	else
		v97 = (signed __int16)v96 + 4;
	v124 = 8 * uLayingItemID;
	LOBYTE(v124) = v124 | 2;
	v125 = word_4EE088_sound_ids[pSpriteObjects[uLayingItemID].spell_id - 1] + 1;
	pAudioPlayer->PlaySound((SoundID)v125, v124, 0, -1, 0, v97, 0, 0);
	return 0;
  }

  
	  case 555:
  {
			  sub_43A97E(uLayingItemID, a2);
			  ++pSpriteObjects[uLayingItemID].uType;
			  v18 = 0;
			  v22 = 0;
			  v25 = (char *)&pObjectList->pObjects->uObjectID;
			  for (v24 = 0; v24 < (signed int)pObjectList->uNumObjects; ++v24)
			  {
				  if (pSpriteObjects[uLayingItemID].uType == pObjectList->pObjects[v24].uObjectID)
					  v22 = v24;
			  }
			  pSpriteObjects[uLayingItemID].uObjectDescID = v22;
			  if (v22 == v18)
				  SpriteObject::OnInteraction(uLayingItemID);
			  pSpriteObjects[uLayingItemID].vVelocity.z = v18;
			  pSpriteObjects[uLayingItemID].vVelocity.y = v18;
			  pSpriteObjects[uLayingItemID].vVelocity.x = v18;
			  pSpriteObjects[uLayingItemID].uSpriteFrameID = v18;
			  return 0;
  }
  
	  case 3090:
  {
			   //v9 = 0;
			   pSpriteObjects[uLayingItemID].uType = pSpriteObjects[uLayingItemID].uType + 2;
			   v63 = 0;
			   for (v61 = 0; v61 < (signed int)pObjectList->uNumObjects; ++v61)
			   {
				   if (v59 == pObjectList->pObjects[v61].uObjectID)
					   v63 = v61;
			   }
			   pSpriteObjects[uLayingItemID].uObjectDescID = v63;
			   if (!v63)
				   SpriteObject::OnInteraction(uLayingItemID);
			   v64 = pSpriteObjects[uLayingItemID].uFacing - stru_5C6E00->uIntegerDoublePi;
			   v44 = pSpriteObjects[uLayingItemID].spell_skill == 4;
			   pSpriteObjects[uLayingItemID].vVelocity.z = 0;
			   pSpriteObjects[uLayingItemID].vVelocity.y = 0;
			   pSpriteObjects[uLayingItemID].vVelocity.x = 0;
			   v65 = 7;
			   if (v44)
				   v65 = 9;
			   if (v65 > 0)
			   {
				   v141 = v65;
				   do
				   {
					   v64 += (signed int)stru_5C6E00->uIntegerHalfPi / 2;
					   pSpriteObjects[uLayingItemID].Create(v64, 0, 1000, 0);
					   --v141;
				   } while (v141);
			   }
			   SpriteObject::OnInteraction(uLayingItemID);
			   if (!pSpriteObjects[uLayingItemID].uSoundID)
				   v16 = 0;
			   else
				   v16 = (signed __int16)pSpriteObjects[uLayingItemID].uSoundID + 4;
			   v124 = 8 * uLayingItemID;
			   LOBYTE(v124) = v124 | 2;
			   v125 = word_4EE088_sound_ids[pSpriteObjects[uLayingItemID].spell_id - 1] + 1;
			   pAudioPlayer->PlaySound((SoundID)v125, v124, 0, -1, 0, v16, 0, 0);
			   return 0;
  }
  
	  case 3092:
  {
			   pSpriteObjects[uLayingItemID].uType = pSpriteObjects[uLayingItemID].uType - 1;
			   v58 = 0;
			   for (v56 = 0; v56 < (signed int)pObjectList->uNumObjects; ++v56)
			   {
				   if (pSpriteObjects[uLayingItemID].uType == pObjectList->pObjects[v56].uObjectID)
					   v58 = v56;
			   }
			   pSpriteObjects[uLayingItemID].uObjectDescID = v58;
			   if (!v58)
				   SpriteObject::OnInteraction(uLayingItemID);
			   pSpriteObjects[uLayingItemID].vVelocity.z = 0;
			   pSpriteObjects[uLayingItemID].vVelocity.y = 0;
			   pSpriteObjects[uLayingItemID].vVelocity.x = 0;
			   pSpriteObjects[uLayingItemID].uSpriteFrameID = 0;
			   sub_43A97E(uLayingItemID, a2);
			   if (!pSpriteObjects[uLayingItemID].uSoundID)
				   v16 = 0;
			   else
				   v16 = (signed __int16)pSpriteObjects[uLayingItemID].uSoundID + 4;
			   v124 = 8 * uLayingItemID;
			   LOBYTE(v124) = v124 | 2;
			   v125 = word_4EE088_sound_ids[pSpriteObjects[uLayingItemID].spell_id - 1] + 1;
			   pAudioPlayer->PlaySound((SoundID)v125, v124, 0, -1, 0, v16, 0, 0);
			   return 0;
  }
  
	  case 4070:
  {
			   if (PID_TYPE(a2) == 6 || PID_TYPE(a2) == 5 || !PID_TYPE(a2))
				   return 1;
			   pSpriteObjects[uLayingItemID].uType = pSpriteObjects[uLayingItemID].uType + 1;
			   v71 = 0;
			   for (v69 = 0; v69 < (signed int)pObjectList->uNumObjects; ++v69)
			   {
				   if (pSpriteObjects[uLayingItemID].uType == pObjectList->pObjects[v69].uObjectID)
					   v71 = v69;
			   }
			   pSpriteObjects[uLayingItemID].uObjectDescID = v71;
			   if (!v71)
				   SpriteObject::OnInteraction(uLayingItemID);
			   v134 = 0;
			   v72 = uLayingItemID;
			   v132 = 0;
			   pSpriteObjects[uLayingItemID].vVelocity.z = 0;
			   pSpriteObjects[uLayingItemID].vVelocity.y = 0;
			   pSpriteObjects[uLayingItemID].vVelocity.x = 0;
			   pSpriteObjects[uLayingItemID].uSpriteFrameID = 0;
			   AttackerInfo.Add(PID(OBJECT_Item, v72), 512, pSpriteObjects[uLayingItemID].vPosition.x, pSpriteObjects[uLayingItemID].vPosition.y, pSpriteObjects[uLayingItemID].vPosition.z, v132, v134);
			   if (!pSpriteObjects[uLayingItemID].uSoundID)
				   v78 = 0;
			   else
				   v78 = (signed __int16)pSpriteObjects[uLayingItemID].uSoundID + 4;
			   v125 = word_4EE088_sound_ids[pSpriteObjects[uLayingItemID].spell_id - 1] + 1;
			   pAudioPlayer->PlaySound((SoundID)v125, pSpriteObjects[uLayingItemID].vPosition.x, 0, -1, 0, v78, 0, 0);
			   return 0;
  }
  
	  case 4090:
  {
			   //v9 = 0;
			   pSpriteObjects[uLayingItemID].uType = pSpriteObjects[uLayingItemID].uType + 2;
			   v88 = 0;
			   for (v86 = 0; v86 < (signed int)pObjectList->uNumObjects; ++v86)
			   {
				   if (pSpriteObjects[uLayingItemID].uType == pObjectList->pObjects[v86].uObjectID)
					   v88 = v86;
			   }
			   pSpriteObjects[uLayingItemID].uObjectDescID = v88;
			   if (!v88)
				   SpriteObject::OnInteraction(uLayingItemID);
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
			   } while (v148);
			   SpriteObject::OnInteraction(uLayingItemID);
			   if (!pSpriteObjects[uLayingItemID].uSoundID)
				   v16 = 0;
			   else
				   v16 = (signed __int16)pSpriteObjects[uLayingItemID].uSoundID + 4;
			   v124 = 8 * uLayingItemID;
			   LOBYTE(v124) = v124 | 2;
			   v125 = word_4EE088_sound_ids[pSpriteObjects[uLayingItemID].spell_id - 1] + 1;
			   pAudioPlayer->PlaySound((SoundID)v125, v124, 0, -1, 0, v16, 0, 0);
			   return 0;
  }
  
	  case 4092:
  {
			   pSpriteObjects[uLayingItemID].uType = 4091;
			   v83 = 0;
			   for (v81 = 0; v81 < (signed int)pObjectList->uNumObjects; ++v81)
			   {
				   if (pSpriteObjects[uLayingItemID].uType == pObjectList->pObjects[v81].uObjectID)
					   v83 = v81;
			   }
			   pSpriteObjects[uLayingItemID].uObjectDescID = v83;
			   if (!v83)
				   SpriteObject::OnInteraction(uLayingItemID);
			   v134 = 0;
			   //v72 = uLayingItemID;
			   v132 = pSpriteObjects[uLayingItemID].field_61;
			   pSpriteObjects[uLayingItemID].vVelocity.z = 0;
			   pSpriteObjects[uLayingItemID].vVelocity.y = 0;
			   pSpriteObjects[uLayingItemID].vVelocity.x = 0;
			   pSpriteObjects[uLayingItemID].uSpriteFrameID = 0;
			   AttackerInfo.Add(PID(OBJECT_Item, uLayingItemID), 512, pSpriteObjects[uLayingItemID].vPosition.x, pSpriteObjects[uLayingItemID].vPosition.y, pSpriteObjects[uLayingItemID].vPosition.z, v132, v134);
			   if (!pSpriteObjects[uLayingItemID].uSoundID)
				   v78 = 0;
			   else
				   v78 = (signed __int16)pSpriteObjects[uLayingItemID].uSoundID + 4;
			   v125 = word_4EE088_sound_ids[pSpriteObjects[uLayingItemID].spell_id - 1] + 1;
			   pAudioPlayer->PlaySound((SoundID)v125, pSpriteObjects[uLayingItemID].vPosition.x, 0, -1, 0, v78, 0, 0);
			   return 0;
  }
    
	  case 8010:
	  {
		if (PID_TYPE(a2) == 3
			&& MonsterStats::BelongsToSupertype(pActors[PID_ID(a2)].pMonsterInfo.uID, MONSTER_SUPERTYPE_UNDEAD))
			sub_43A97E(uLayingItemID, a2);
		++pSpriteObjects[uLayingItemID].uType;
		//v9 = 0;
		v95 = 0;
		for (v52 = 0; v52 < (signed int)pObjectList->uNumObjects; ++v52)
		{
			if (pSpriteObjects[uLayingItemID].uType == pObjectList->pObjects[v52].uObjectID)
				v95 = v52;
		}
		pSpriteObjects[uLayingItemID].uObjectDescID = v95;
		if (!v95)
			SpriteObject::OnInteraction(uLayingItemID);
		v96 = pSpriteObjects[uLayingItemID].uSoundID;
		pSpriteObjects[uLayingItemID].vVelocity.z = 0;
		pSpriteObjects[uLayingItemID].vVelocity.y = 0;
		pSpriteObjects[uLayingItemID].vVelocity.x = 0;
		pSpriteObjects[uLayingItemID].uSpriteFrameID = 0;
		if (!v96)
			v97 = 0;
		else
			v97 = (signed __int16)v96 + 4;
		v92 = uLayingItemID;
		v124 = 8 * v92;
		LOBYTE(v124) = v124 | 2;
		v125 = word_4EE088_sound_ids[pSpriteObjects[uLayingItemID].spell_id - 1] + 1;
		pAudioPlayer->PlaySound((SoundID)v125, v124, 0, -1, 0, v97, 0, 0);
		return 0;
	  }
	  
	  case 7030:
	  case 7090:
	  case 8000:
	  case 8090:
	  {
		sub_43A97E(uLayingItemID, a2);
		++pSpriteObjects[uLayingItemID].uType;
		v95 = 0;
		for (v52 = 0; v52 < (signed int)pObjectList->uNumObjects; ++v52)
		{
			if (pSpriteObjects[uLayingItemID].uType == pObjectList->pObjects[v52].uObjectID)
				v95 = v52;
		}
		pSpriteObjects[uLayingItemID].uObjectDescID = v95;
		if (!v95)
			SpriteObject::OnInteraction(uLayingItemID);
		v96 = pSpriteObjects[uLayingItemID].uSoundID;
		pSpriteObjects[uLayingItemID].vVelocity.z = 0;
		pSpriteObjects[uLayingItemID].vVelocity.y = 0;
		pSpriteObjects[uLayingItemID].vVelocity.x = 0;
		pSpriteObjects[uLayingItemID].uSpriteFrameID = 0;
		if (!v96)
			v97 = 0;
		else
			v97 = (signed __int16)v96 + 4;
		v124 = 8 * uLayingItemID;
		LOBYTE(v124) = v124 | 2;
		v125 = word_4EE088_sound_ids[pSpriteObjects[uLayingItemID].spell_id - 1] + 1;
		pAudioPlayer->PlaySound((SoundID)v125, v124, 0, -1, 0, v97, 0, 0);
		return 0;
	  }
	  
	  case 6040:
	  case 8030:
	  case 9030:
	  {
		v143 = 17030;
		switch (pSpriteObjects[uLayingItemID].uType)
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
		if (PID_TYPE(a2) != OBJECT_Actor)
		{
			if (pSpriteObjects[uLayingItemID].uType != 9030 || pSpriteObjects[uLayingItemID].spell_skill != 4)
			{
				SpriteObject::OnInteraction(uLayingItemID);
				return 0;
			}
			pSpriteObjects[uLayingItemID]._46BEF1_apply_spells_aoe();
			if (!v138)
			{
				++pSpriteObjects[uLayingItemID].uType;
				v112 = 0;
				for (v110 = 0; v110 < (signed int)pObjectList->uNumObjects; ++v110)
				{
					if (pSpriteObjects[uLayingItemID].uType == pObjectList->pObjects[v110].uObjectID)
						v112 = v110;
				}
				pSpriteObjects[uLayingItemID].uObjectDescID = v112;
				if (!v112)
					SpriteObject::OnInteraction(uLayingItemID);
				pSpriteObjects[uLayingItemID].vVelocity.z = 0;
				pSpriteObjects[uLayingItemID].vVelocity.y = 0;
				pSpriteObjects[uLayingItemID].vVelocity.x = 0;
				pSpriteObjects[uLayingItemID].uSpriteFrameID = 0;
				v113 = pSpriteObjects[uLayingItemID].uSoundID;
				if (v113)
					v114 = (signed __int16)v113 + 4;
				else
					v114 = 0;
				v115 = 8 * uLayingItemID;
				LOBYTE(v115) = PID(OBJECT_Item, uLayingItemID);
				v125 = v143 + 1;
				pAudioPlayer->PlaySound((SoundID)v125, v115, 0, -1, 0, v114, 0, 0);
			}
			else
				SpriteObject::OnInteraction(uLayingItemID);
			return 0;
		}
		v106 = a2;
		v150 = 0;
		v139 = PID_ID(v106);
		v137 = pSpriteObjects[uLayingItemID].spell_level;
		v152 = pSpriteObjects[uLayingItemID].spell_skill;
		v136 = pSpriteObjects[uLayingItemID].spell_id;
		if (pSpriteObjects[uLayingItemID].uType == 9030)
		{
			v150 = 2;
			if (v152 == 2)
			{
				v150 = 3;
			}
			else
			{
				if (v152 >= 3)
					v150 = 4;
			}
			pActors[v139].uAttributes |= 0x80000;
			v107 = v135;
		}
		if (pSpriteObjects[uLayingItemID].uType == 6040)
		{
			v135 = 7;
			v107 = v135;
		}
		else
		{
			if (pSpriteObjects[uLayingItemID].uType == 8030)
			{
				v135 = 9;
				v107 = v135;
			}
			else
			{
				if (pSpriteObjects[uLayingItemID].uType != 9030)
				{
					v107 = v136;
				}
				if (pSpriteObjects[uLayingItemID].uType == 9030)
				{
					v135 = 10;
					v107 = v135;
				}
			}
		}
		if (pSpriteObjects[uLayingItemID].uType != 9030 || v152 != 4)
		{
			v108 = v139;
			if (pActors[v139].DoesDmgTypeDoDamage((DAMAGE_TYPE)v107))
			{
				v138 = 0;
				if (pSpriteObjects[uLayingItemID].uType == 8030)
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
		if (!v138)
		{
			++pSpriteObjects[uLayingItemID].uType;
			v112 = 0;
			for (v110 = 0; v110 < (signed int)pObjectList->uNumObjects; ++v110)
			{
				if (pSpriteObjects[uLayingItemID].uType == pObjectList->pObjects[v110].uObjectID)
					v112 = v110;
			}
			pSpriteObjects[uLayingItemID].uObjectDescID = v112;
			if (!v112)
				SpriteObject::OnInteraction(uLayingItemID);
			pSpriteObjects[uLayingItemID].vVelocity.z = 0;
			pSpriteObjects[uLayingItemID].vVelocity.y = 0;
			pSpriteObjects[uLayingItemID].vVelocity.x = 0;
			pSpriteObjects[uLayingItemID].uSpriteFrameID = 0;
			v113 = pSpriteObjects[uLayingItemID].uSoundID;
			if (v113)
				v114 = (signed __int16)v113 + 4;
			else
				v114 = 0;
			v115 = 8 * uLayingItemID;
			LOBYTE(v115) = PID(OBJECT_Item, uLayingItemID);
			v125 = v143 + 1;
			pAudioPlayer->PlaySound((SoundID)v125, v115, 0, -1, 0, v114, 0, 0);
		}
		else
			SpriteObject::OnInteraction(uLayingItemID);
		return 0;
	  }
	  
	  case 9040:
	  {
			sub_43A97E(uLayingItemID, a2);
			++pSpriteObjects[uLayingItemID].uType;
			v95 = 0;
			for (v52 = 0; v52 < (signed int)pObjectList->uNumObjects; ++v52)
			{
				if (pSpriteObjects[uLayingItemID].uType == pObjectList->pObjects[v52].uObjectID)
					v95 = v52;
			}
			pSpriteObjects[uLayingItemID].uObjectDescID = v95;
			if (!v95)
				SpriteObject::OnInteraction(uLayingItemID);
			v96 = pSpriteObjects[uLayingItemID].uSoundID;
			pSpriteObjects[uLayingItemID].vVelocity.z = 0;
			pSpriteObjects[uLayingItemID].vVelocity.y = 0;
			pSpriteObjects[uLayingItemID].vVelocity.x = 0;
			pSpriteObjects[uLayingItemID].uSpriteFrameID = 0;
			if (!v96)
				v97 = 0;
			else
				v97 = (signed __int16)v96 + 4;
			v124 = 8 * uLayingItemID;
			LOBYTE(v124) = v124 | 2;
			v125 = word_4EE088_sound_ids[pSpriteObjects[uLayingItemID].spell_id - 1] + 1;
			pAudioPlayer->PlaySound((SoundID)v125, v124, 0, -1, 0, v97, 0, 0);
			return 0;
	  }

/*
	  case 1080:
	  case 2100:
	  {
				   if (PID_TYPE(a2) != 3)
				   {
					   //v32 = 0;
					   pSpriteObjects[uLayingItemID].uType = pSpriteObjects[uLayingItemID].uType + 1;
					   v46 = 0;
					   for (v146 = 0; v146 < (signed int)pObjectList->uNumObjects; ++v146)
					   {
						   if (pSpriteObjects[uLayingItemID].uType == pObjectList->pObjects[v146].uObjectID)
							   v46 = v146;
					   }
					   pSpriteObjects[uLayingItemID].uObjectDescID = v46;
					   if (!v46)
						   SpriteObject::OnInteraction(uLayingItemID);
					   v100 = pSpriteObjects[uLayingItemID].field_61;
					   pSpriteObjects[uLayingItemID].uSpriteFrameID = 0;
					   v102 = 8 * uLayingItemID;
					   LOBYTE(v102) = PID(OBJECT_Item, uLayingItemID);
					   pSpriteObjects[uLayingItemID].vVelocity.x = 0;
					   pSpriteObjects[uLayingItemID].vVelocity.y = 0;
					   pSpriteObjects[uLayingItemID].vVelocity.z = 0;
					   AttackerInfo.Add(v102, 512, pSpriteObjects[uLayingItemID].vPosition.x, pSpriteObjects[uLayingItemID].vPosition.y, pSpriteObjects[uLayingItemID].vPosition.z, v100, 0);
					   if (object->uFlags & OBJECT_DESC_TRIAL_PARTICLE)
						   trail_particle_generator.GenerateTrailParticles(pSpriteObjects[uLayingItemID].vPosition.x, pSpriteObjects[uLayingItemID].vPosition.y, pSpriteObjects[uLayingItemID].vPosition.z, object->uParticleTrailColor);
					   if (!pSpriteObjects[uLayingItemID].uSoundID)
						   v47 = 0;
					   else
						   v47 = (signed __int16)pSpriteObjects[uLayingItemID].uSoundID + 4;
					   v125 = word_4EE088_sound_ids[pSpriteObjects[uLayingItemID].spell_id - 1] + 1;
					   pAudioPlayer->PlaySound((SoundID)v125, v102, 0, -1, 0, v47, 0, 0);
					   return 0;
				   }
				   return 1;
	  }*/

	  case 1080:
	  case 2100:
	  {
				   if (PID_TYPE(a2) == 3)
					   return 1;
					//else go to next case
	  }

	  case 1050:
	  case 9080:
	  {
		v95 = 0;
		pSpriteObjects[uLayingItemID].uType = pSpriteObjects[uLayingItemID].uType + 1;
		for (v146 = 0; v146 < (signed int)pObjectList->uNumObjects; ++v146)
		{
			if (pSpriteObjects[uLayingItemID].uType == pObjectList->pObjects[v146].uObjectID)
				v95 = v146;
		}
		pSpriteObjects[uLayingItemID].uObjectDescID = v95;
		if (!v95)
			SpriteObject::OnInteraction(uLayingItemID);
		v100 = pSpriteObjects[uLayingItemID].field_61;
		pSpriteObjects[uLayingItemID].uSpriteFrameID = 0;
		v102 = 8 * uLayingItemID;
		LOBYTE(v102) = PID(OBJECT_Item, uLayingItemID);
		pSpriteObjects[uLayingItemID].vVelocity.x = 0;
		pSpriteObjects[uLayingItemID].vVelocity.y = 0;
		pSpriteObjects[uLayingItemID].vVelocity.z = 0;
		AttackerInfo.Add(v102, 512, pSpriteObjects[uLayingItemID].vPosition.x, pSpriteObjects[uLayingItemID].vPosition.y, pSpriteObjects[uLayingItemID].vPosition.z, v100, 0);
		if (object->uFlags & OBJECT_DESC_TRIAL_PARTICLE)
			trail_particle_generator.GenerateTrailParticles(pSpriteObjects[uLayingItemID].vPosition.x, pSpriteObjects[uLayingItemID].vPosition.y, pSpriteObjects[uLayingItemID].vPosition.z, object->uParticleTrailColor);
		if (!pSpriteObjects[uLayingItemID].uSoundID)
			v47 = 0;
		else
			v47 = (signed __int16)pSpriteObjects[uLayingItemID].uSoundID + 4;
		v125 = word_4EE088_sound_ids[pSpriteObjects[uLayingItemID].spell_id - 1] + 1;
		pAudioPlayer->PlaySound((SoundID)v125, v102, 0, -1, 0, v47, 0, 0);
		return 0;
	  }

	  default:
		  return 0;
  }

}
