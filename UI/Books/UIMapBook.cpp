#define _CRT_SECURE_NO_WARNINGS
#include "UIMapBook.h"
#include "..\..\Events.h"
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
#include "..\..\Outdoor.h"
#include "..\..\LOD.h"
#include "..\..\Viewport.h"
#include "..\..\OurMath.h"
#include "..\..\texts.h"
#include "..\..\Level/Decoration.h"
//
#include "..\..\mm7_data.h"

std::array<__int16, 6> pTownPortalBook_xs = {{260, 324, 147, 385, 390,  19}};
std::array<__int16, 6> pTownPortalBook_ys = {{206,  84, 182, 239,  17, 283}};

std::array<unsigned int, 5> pLloydsBeaconsPreviewXs = {{61, 281,  61, 281, 171}}; // 004E249C
std::array<unsigned int, 5> pLloydsBeaconsPreviewYs = {{84,  84, 228, 228, 155}};
std::array<unsigned int, 5> pLloydsBeacons_SomeXs = {{59, 279, 59, 279, 169}};
std::array<unsigned int, 5> pLloydsBeacons_SomeYs = {{82, 82, 226, 226, 153}};




//----- (00411150) --------------------------------------------------------
void BookUI_DrawTownPortalMap()
{
  int v3; // edi@17
  GUIWindow TownPortalWindow; // [sp+Ch] [bp-64h]@1
  POINT a2; // [sp+68h] [bp-8h]@17

  pRenderer->ClearZBuffer(0, 479);
  pRenderer->DrawTextureTransparent(8, 8, pTexture_CurrentBook);
  pRenderer->DrawTextureTransparent(471, 445, pIcons_LOD->GetTexture(uExitCancelTextureId));

  TownPortalWindow.uFrameX = game_viewport_x;
  TownPortalWindow.uFrameY = game_viewport_y;
  TownPortalWindow.uFrameWidth = game_viewport_width;
  TownPortalWindow.uFrameHeight = game_viewport_height;
  TownPortalWindow.uFrameZ = game_viewport_z;
  TownPortalWindow.uFrameW = game_viewport_w;

  const uint fountain_bits_lut[] = {PARTY_QUEST_FOUNTAIN_HARMONDALE,
                                    PARTY_QUEST_FOUNTAIN_PIERPONT,
                                    PARTY_QUEST_FOUNTAIN_NIGHON,
                                    PARTY_QUEST_FOUNTAIN_EVENMORN_ISLE,
                                    PARTY_QUEST_FOUNTAIN_CELESTIA,
                                    PARTY_QUEST_FOUNTAIN_THE_PIT};
  for (uint i = 0; i < 6; ++i)
  {

    if (_449B57_test_bit(pParty->_quest_bits, fountain_bits_lut[i]))
      pRenderer->DrawMaskToZBuffer(pTownPortalBook_xs[i],
                                   pTownPortalBook_ys[i],
                                   pTexture_TownPortalIcons[i], i + 1);
  }

  pMouse->GetCursorPos(&a2);
  v3 = pRenderer->pActiveZBuffer[a2.x + pSRZBufferLineOffsets[a2.y]] & 0xFFFF;

  if (v3)
  {
    if (_449B57_test_bit(pParty->_quest_bits, fountain_bits_lut[v3 - 1]))
      pRenderer->DrawTextureIndexed(pTownPortalBook_xs[v3 - 1], pTownPortalBook_ys[v3 - 1], pTexture_TownPortalIcons[v3 - 1]);
  }
  TownPortalWindow.DrawTitleText(pBook2Font, 0, 22, 0, pGlobalTXT_LocalizationStrings[10], 3);
}

//----- (00410DEC) --------------------------------------------------------
unsigned int  DrawLloydBeaconsScreen()
{
  Player *pPlayer; // esi@1
  const char *pText; // eax@1
  int pTextHeight; // eax@14
  int RemainingTime; // kr08_8@14
  unsigned int pHours; // esi@14
  unsigned int pDays; // eax@14
  const char *pSelectionText; // eax@19
  Texture *v19; // [sp-4h] [bp-8Ch]@4
  GUIWindow pWindow; // [sp+Ch] [bp-7Ch]@1
  char *Str; // [sp+74h] [bp-14h]@14
  int BeaconID; // [sp+78h] [bp-10h]@11
  int uNumMaxBeacons; // [sp+84h] [bp-4h]@6

  pPlayer = &pParty->pPlayers[_506348_current_lloyd_playerid];
  pRenderer->DrawTextureIndexed(8, 8, pTexture_LloydBeacons[(unsigned __int8)bRecallingBeacon]);
  pText = pGlobalTXT_LocalizationStrings[523];     // Recall Beacon
  pWindow.uFrameX = game_viewport_x;
  pWindow.uFrameY = game_viewport_y;
  pWindow.uFrameWidth = 428;
  pWindow.uFrameHeight = game_viewport_height;
  pWindow.uFrameZ = 435;
  pWindow.uFrameW = game_viewport_w;
  if ( !bRecallingBeacon )
    pText = pGlobalTXT_LocalizationStrings[375];   // Set Beacon
  sprintf(pTmpBuf.data(), "%s", pText);
  pWindow.DrawTitleText(pBook2Font, 0, 22, 0, pTmpBuf.data(), 3);
  if ( bRecallingBeacon )
  {
    pRenderer->DrawTextureTransparent(pBtn_Book_1->uX, pBtn_Book_1->uY, pTex_book_button1_on);
    v19 = pTex_book_button1_off;
  }
  else
  {
    pRenderer->DrawTextureTransparent(pBtn_Book_1->uX, pBtn_Book_1->uY, pTex_book_button1_off);
    v19 = pTex_book_button1_on;
  }
  pRenderer->DrawTextureTransparent(pBtn_Book_2->uX, pBtn_Book_2->uY, v19);
  uNumMaxBeacons = 1;
  if ( HIBYTE(pPlayer->pActiveSkills[PLAYER_SKILL_WATER]) & 1 || (pPlayer->pActiveSkills[PLAYER_SKILL_WATER] & 0x80u) != 0 )
  {
    uNumMaxBeacons = 5;
  }
  else
  {
    if ( pPlayer->pActiveSkills[PLAYER_SKILL_WATER] & 0x40 )
      uNumMaxBeacons = 3;
  }
  if ( uNumMaxBeacons > 0 )
  {
    for ( BeaconID = 0; BeaconID < uNumMaxBeacons; BeaconID++ )
    {
      pWindow.uFrameWidth = 92;
      pWindow.uFrameHeight = 68;
      pWindow.uFrameY = pLloydsBeaconsPreviewYs[BeaconID];
      pWindow.uFrameX = pLloydsBeaconsPreviewXs[BeaconID];
      pWindow.uFrameW = pWindow.uFrameY + 67;
      pWindow.uFrameZ = pLloydsBeaconsPreviewXs[BeaconID] + 91;
      //if ( pSavegameThumbnails[BeaconID].pPixels != 0 )
      if ( pPlayer->pInstalledBeacons[BeaconID].SaveFileID != 0 )
      {
        pRenderer->DrawTextureTransparent(pLloydsBeacons_SomeXs[BeaconID], pLloydsBeacons_SomeYs[BeaconID], pTexture_CurrentBook);
        pRenderer->DrawTextureRGB(pLloydsBeaconsPreviewXs[BeaconID], pLloydsBeaconsPreviewYs[BeaconID], &pSavegameThumbnails[BeaconID]);
        Str = pMapStats->pInfos[pMapStats->sub_410D99_get_map_index(pPlayer->pInstalledBeacons[BeaconID].SaveFileID)].pName;
        pTextHeight = pSpellFont->CalcTextHeight(Str, &pWindow, 0, 0);
        pWindow.uFrameY += -6 - pTextHeight;
        pWindow.DrawTitleText(pSpellFont, 0, 0, 1, Str, 3);
        RemainingTime = pPlayer->pInstalledBeacons[BeaconID].uBeaconTime - pParty->uTimePlayed;
        pHours = (signed __int64)((double)RemainingTime * 0.234375) / 60 / 60;
        pDays = pHours / 24;
        if ( pDays )
        {
          sprintf(pTmpBuf.data(), "%lu %s", pDays + 1, pGlobalTXT_LocalizationStrings[57]);//days
          pWindow.uFrameY = pWindow.uFrameY + pWindow.uFrameHeight + 4;
          pWindow.DrawTitleText(pSpellFont, 0, 0, 1, pTmpBuf.data(), 3);
          continue;
        }
        else
        {
          if ( pHours + 1 <= 23 )
          {
            if ( pHours < 1 )
              pSelectionText = pGlobalTXT_LocalizationStrings[109];// Hour
            else
              pSelectionText = pGlobalTXT_LocalizationStrings[110];// Hours
            sprintf(pTmpBuf.data(), "%lu %s", pHours + 1, pSelectionText);
            pWindow.uFrameY = pWindow.uFrameY + pWindow.uFrameHeight + 4;
            pWindow.DrawTitleText(pSpellFont, 0, 0, 1, pTmpBuf.data(), 3);
            continue;
          }
        }
        sprintf(pTmpBuf.data(), "%lu %s", pDays + 1, pGlobalTXT_LocalizationStrings[56]);//Day
        pWindow.uFrameY = pWindow.uFrameY + pWindow.uFrameHeight + 4;
        pWindow.DrawTitleText(pSpellFont, 0, 0, 1, pTmpBuf.data(), 3);
        continue;
      }
      if ( !bRecallingBeacon )
      {
        pRenderer->DrawTextureTransparent(pLloydsBeacons_SomeXs[BeaconID], pLloydsBeacons_SomeYs[BeaconID], pTexture_CurrentBook);
        pTextHeight = pSpellFont->CalcTextHeight(pGlobalTXT_LocalizationStrings[19], &pWindow, 0, 0);
        pWindow.DrawTitleText(pSpellFont, 0, (signed int)pWindow.uFrameHeight / 2 - pTextHeight / 2, 1, pGlobalTXT_LocalizationStrings[19], 3);//Доступно
      }
    }
  }
  if ( byte_506360 )
    pMessageQueue_50CBD0->AddGUIMessage(UIMSG_CloseAfterInstallBeacon, 0, 0);
  return BeaconID;
}

//----- (00413980) --------------------------------------------------------
void BookUI_Map_Draw()
{
  unsigned int map_id; // eax@35
  char party_coord[120]; // [sp+Ch] [bp-CCh]@37
  GUIWindow map_window; // [sp+84h] [bp-54h]@35

  pRenderer->DrawTextureIndexed(pViewport->uViewportTL_X, pViewport->uViewportTL_Y, pSpellBookPagesTextr_12);
  if ( BtnUp_flag || viewparams->uMapBookMapZoom / 128 >= 12 )//Button 1
    pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 408, pViewport->uViewportTL_Y + 2, pTex_book_button1_off);
  else
    pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 398, pViewport->uViewportTL_Y + 1, pTex_book_button1_on);

  if ( BtnDown_flag || viewparams->uMapBookMapZoom / 128 <= 3 )//Button 2
    pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 408, pViewport->uViewportTL_Y + 38, pTex_book_button2_off);
  else
    pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 398, pViewport->uViewportTL_Y + 38, pTex_book_button2_on);

  if ( Book_PageBtn3_flag )//Button 3
    pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 408, pViewport->uViewportTL_Y + 113, pTex_book_button3_off);
  else
    pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 398, pViewport->uViewportTL_Y + 113, pTex_book_button3_on);

  if ( Book_PageBtn4_flag )//Button 4
    pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 408, pViewport->uViewportTL_Y + 150, pTex_book_button4_off);
  else
    pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 399, pViewport->uViewportTL_Y + 150, pTex_book_button4_on);

  if ( Book_PageBtn5_flag )//Button 5
    pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 408, pViewport->uViewportTL_Y + 188, pTex_book_button5_off);
  else
    pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 397, pViewport->uViewportTL_Y + 188, pTex_book_button5_on);

  if ( Book_PageBtn6_flag )//Button 6
    pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 408, pViewport->uViewportTL_Y + 226, pTex_book_button6_off);
  else
    pRenderer->DrawTextureTransparent(pViewport->uViewportTL_X + 397, pViewport->uViewportTL_Y + 226, pTex_book_button6_on);

  if ( BtnDown_flag )
    viewparams->CenterOnParty2();
  if ( BtnUp_flag )
    viewparams->CenterOnParty();
  if ( Book_PageBtn3_flag )
    viewparams->_443219();
  if ( Book_PageBtn4_flag )
    viewparams->_443231();
  if ( Book_PageBtn5_flag )
    viewparams->_44323D();
  if ( Book_PageBtn6_flag )
    viewparams->_443225();

  if ( BtnUp_flag | BtnDown_flag | Book_PageBtn3_flag | Book_PageBtn4_flag | Book_PageBtn5_flag | Book_PageBtn6_flag )
    pAudioPlayer->PlaySound(SOUND_Button2, 0, 0, -1, 0, 0, 0, 0);
  BtnUp_flag = 0;
  BtnDown_flag = 0;
  Book_PageBtn6_flag = 0;
  Book_PageBtn5_flag = 0;
  Book_PageBtn4_flag = 0;
  Book_PageBtn3_flag = 0;
  DrawBook_Map_sub(97, 49, 361, 313, 0);
  pRenderer->DrawTextureTransparent(75, 22, pTexture_mapbordr);
  map_window.uFrameWidth = game_viewport_width;
  map_window.uFrameHeight = game_viewport_height;
  map_window.uFrameX = game_viewport_x;
  map_window.uFrameY = game_viewport_y;
  map_window.uFrameZ = game_viewport_z;
  map_window.uFrameW = game_viewport_w;
  map_id = pMapStats->GetMapInfo(pCurrentMapName);
  if ( map_id )
    map_window.DrawTitleText(pBook2Font, -14, 12, ui_book_map_title_color, pMapStats->pInfos[map_id].pName, 3);

  map_window.uFrameX = 0;
  sprintf(party_coord, pGlobalTXT_LocalizationStrings[659], pParty->vPosition.x, pParty->vPosition.y); //"x: %d  y: %d"
  map_window.DrawTitleText(pFontComic, 0, 320, ui_book_map_coordinates_color, party_coord, 0);
}

//----- (00442955) --------------------------------------------------------
void DrawBook_Map_sub( unsigned int tl_x, unsigned int tl_y, unsigned int br_x, int br_y, int _48074 )
{
  int v20; // eax@16
  int v26; // ecx@21
  unsigned __int16 *v27; // edi@21
  int v28; // edx@21
  int v29; // eax@21
  double v30; // st7@23
//  signed __int64 v31; // qax@23
//  unsigned short *v32; // edx@23
  int textr_width; // esi@23
//  signed int v34; // eax@23
//  signed int v35; // ecx@23
//  int v36; // esi@27
//  int v37; // ecx@27
//  int v38; // edx@31
//  unsigned int v39; // eax@33
//  short *v40; // esi@33
//  short *v41; // edi@33
//  unsigned __int8 v42; // cf@33
//  unsigned int v43; // ecx@33
//  short *v44; // edi@33
//  short *v45; // esi@33
//  int v46; // ecx@33
  signed int v47; // esi@38
  signed int v48; // ecx@38
  int v49; // eax@38
  signed int v50; // edx@55
  unsigned int v51; // ecx@55
//  int result; // eax@72
  int v54; // esi@75
  int v55; // eax@75
//  __int16 v56; // si@85
  double v57; // st7@85
//  int v58; // ebx@85
//  signed __int64 v59; // qax@85
  signed int v60; // edi@85
//  signed __int64 v61; // qax@85
  signed int v62; // ebx@85
  signed int v63; // esi@85
  int v64; // eax@87
//  unsigned int v65; // ebx@95
//  unsigned short *v66; // edx@95
//  unsigned __int16 *v67; // esi@96
//  int v68; // edi@98
//  unsigned __int16 v69; // cx@99
//  signed int v73; // [sp-4h] [bp-48068h]@59
//  unsigned __int16 v74; // [sp-4h] [bp-48068h]@79
  //unsigned short map_texture_16[320 * 460 + 256]; // [sp+Ch] [bp-48058h]@23
//  int v76; // [sp+4800Ch] [bp-58h]@23
  //unsigned __int16 *v77; // [sp+48010h] [bp-54h]@27
  unsigned __int16 *pPalette_16; // [sp+48014h] [bp-50h]@23
  int map_tile_X; // [sp+48020h] [bp-44h]@23
  //unsigned __int16* render16_data;
  unsigned char* texture8_data;
  unsigned char* curr_line;
  int scale_increment;
  int scaled_posX;
  int scaled_posY;
  int stepX_r;
  int stepY_r;
  unsigned int teal; // [sp+48028h] [bp-3Ch]@8
  int pCenterY; // [sp+4802Ch] [bp-38h]@1
  int screenCenter_X; // [sp+48030h] [bp-34h]@1
  int pCenterX; // [sp+48034h] [bp-30h]@1
  int v87; // [sp+48038h] [bp-2Ch]@16
  unsigned int v88; // [sp+4803Ch] [bp-28h]@16
  int black; // [sp+48040h] [bp-24h]@8
  int screenCenterY; // [sp+48044h] [bp-20h]@1
  unsigned int screenHeight; // [sp+4804Ch] [bp-18h]@16
  unsigned __int16 *v93; // [sp+48050h] [bp-14h]@16
  signed int screenWidth; // [sp+48054h] [bp-10h]@8
  unsigned int v95; // [sp+48058h] [bp-Ch]@16
  int map_tile_Y; // [sp+4805Ch] [bp-8h]@10
  const void *v97; // [sp+48060h] [bp-4h]@16
//  unsigned short *a4a; // [sp+4806Ch] [bp+8h]@85
  //int a5a; // [sp+48070h] [bp+Ch]@86

  screenCenter_X = (signed int)(tl_x + br_x) / 2;
  screenCenterY = (signed int)(tl_y + br_y) / 2;
  pRenderer->SetRasterClipRect(tl_x, tl_y, br_x, br_y);
  pCenterX = viewparams->sViewCenterX;
  pCenterY = viewparams->sViewCenterY;
  if ( viewparams->uMapBookMapZoom != 384 )
  {
    if ( viewparams->uMapBookMapZoom == 768 )
    {
      if ( uCurrentlyLoadedLevelType == LEVEL_Indoor)
        viewparams->uMapBookMapZoom = 680;
    }
  }
  else
  {
    viewparams->sViewCenterX = viewparams->indoor_center_x;
    pCenterX = viewparams->indoor_center_x;
    pCenterY = viewparams->indoor_center_y;
    if ( uCurrentlyLoadedLevelType == LEVEL_Indoor)
      viewparams->uMapBookMapZoom = viewparams->uMapBookMapZoom - 34;
  }
  if ( uCurrentlyLoadedLevelType != LEVEL_Indoor)
  {
    screenWidth = br_x - tl_x + 1;
    screenHeight = br_y - tl_y + 1;
    //render16_data = &pRenderer->pTargetSurface[tl_x + tl_y * pRenderer->uTargetSurfacePitch];
    texture8_data = pIcons_LOD->pTextures[viewparams->uTextureID_LocationMap].pLevelOfDetail0_prolly_alpha_mask;
    pPalette_16 = pIcons_LOD->pTextures[viewparams->uTextureID_LocationMap].pPalette16;
    scale_increment = (1 << (pIcons_LOD->pTextures[viewparams->uTextureID_LocationMap].uWidthLn2 + 16)) / viewparams->uMapBookMapZoom;

    v30 = (double)(1 << (16 - pIcons_LOD->pTextures[viewparams->uTextureID_LocationMap].uWidthLn2));

    teal = (unsigned int)(signed __int64)((double)(viewparams->sViewCenterX - 22528 / (viewparams->uMapBookMapZoom / 384) + 32768) / v30) << 16;

    textr_width = pIcons_LOD->pTextures[viewparams->uTextureID_LocationMap].uTextureWidth;
    stepY_r =            (int)(signed __int64)((double)(- pCenterY - 22528 / (viewparams->uMapBookMapZoom / 384)+ 32768) / v30) << 16;
    black = (signed __int16)(signed __int64)((double)(viewparams->sViewCenterX - 22528 / (viewparams->uMapBookMapZoom / 384) + 32768) / v30);

    scaled_posY = stepY_r >> 16;
    //nearest neiborhood scaling
    if ( texture8_data)  
    {
      for( uint i = 0; i < screenHeight; ++i )
      {
        curr_line = &texture8_data[scaled_posY*textr_width];
        stepX_r = teal;
        for( uint j = 0; j < screenWidth; ++j )
        {
          scaled_posX = stepX_r>>16;
          //map_texture_16[i*screenWidth+j]=pPalette_16[*(curr_line+scaled_posX)];
          pRenderer->WritePixel16(tl_x + j, tl_y + i, pPalette_16[*(curr_line+scaled_posX)]);
          stepX_r += scale_increment;
        }
        stepY_r+=scale_increment;
        scaled_posY=stepY_r>>16;
      }
    }

    /*//move visible square to render
    for( uint i = 0; i < screenHeight; ++i )
    {
      if ( screenWidth > 0 )
        memcpy((void*)&render16_data[pRenderer->uTargetSurfacePitch * i],(void*)&map_texture_16[i*screenWidth], screenWidth*2);
    }*/
  }
  else
  {
    black = Color16(0, 0, 0);
    teal = Color16(0, 0xFFu, 0xFFu);
    uNumBlueFacesInBLVMinimap = 0;
    if ( pIndoor->pMapOutlines->uNumOutlines )
    {
     for ( uint i = 0; i < pIndoor->pMapOutlines->uNumOutlines; ++i )
      {
        if ( !(pIndoor->pFaces[pIndoor->pMapOutlines->pOutlines[i].uFace1ID].Invisible() 
           || (pIndoor->pFaces[pIndoor->pMapOutlines->pOutlines[i].uFace2ID].Invisible()) ))
        {
          if ( !(pIndoor->pMapOutlines->pOutlines[i].uFlags & 1) )
          {
            if ( !(!(pIndoor->pFaces[pIndoor->pMapOutlines->pOutlines[i].uFace1ID].uAttributes & FACE_UNKNOW4)
                && !(pIndoor->pFaces[pIndoor->pMapOutlines->pOutlines[i].uFace2ID].uAttributes & FACE_UNKNOW4) ))
            {
              pIndoor->pMapOutlines->pOutlines[i].uFlags = pIndoor->pMapOutlines->pOutlines[i].uFlags | 1;
              pIndoor->_visible_outlines[i >> 3] |= 1 << (7 - i % 8);
            }
          }
          if ( (!(pIndoor->pMapOutlines->pOutlines[i].uFlags & 1) 
            && !(!(pIndoor->pFaces[pIndoor->pMapOutlines->pOutlines[i].uFace1ID].uAttributes & FACE_UNKNOW4)
            && !(pIndoor->pFaces[pIndoor->pMapOutlines->pOutlines[i].uFace2ID].uAttributes & FACE_UNKNOW4) ))
            || pIndoor->pMapOutlines->pOutlines[i].uFlags & 1)
          {
            v93 = (unsigned __int16 *)(pIndoor->pVertices[pIndoor->pMapOutlines->pOutlines[i].uVertex1ID].x - viewparams->sViewCenterX);
            screenHeight = pIndoor->pVertices[pIndoor->pMapOutlines->pOutlines[i].uVertex1ID].y - pCenterY;
            v20 = pIndoor->pVertices[pIndoor->pMapOutlines->pOutlines[i].uVertex2ID].y - pCenterY;
            v95 = pIndoor->pVertices[pIndoor->pMapOutlines->pOutlines[i].uVertex2ID].x - viewparams->sViewCenterX;

            v88 = fixpoint_mul((pIndoor->pVertices[pIndoor->pMapOutlines->pOutlines[i].uVertex1ID].x - viewparams->sViewCenterX), viewparams->uMapBookMapZoom);
            v87 = fixpoint_mul(screenHeight, viewparams->uMapBookMapZoom);
            v93 = (unsigned __int16 *)(fixpoint_mul((pIndoor->pVertices[pIndoor->pMapOutlines->pOutlines[i].uVertex2ID].x - viewparams->sViewCenterX), viewparams->uMapBookMapZoom));
            screenHeight = fixpoint_mul(v20, viewparams->uMapBookMapZoom);
            pRenderer->RasterLine2D(screenCenter_X + v88, screenCenterY - v87,
              screenCenter_X + (fixpoint_mul((pIndoor->pVertices[pIndoor->pMapOutlines->pOutlines[i].uVertex2ID].x - viewparams->sViewCenterX), viewparams->uMapBookMapZoom)), screenCenterY - screenHeight, black);
          }
        }
      }
    }
    if ( (signed int)uNumBlueFacesInBLVMinimap > 0 )
    {
      for ( uint j = 0; j < (signed int)uNumBlueFacesInBLVMinimap; ++j )
      {
        v26 = pIndoor->pVertices[pIndoor->pMapOutlines->pOutlines[pBlueFacesInBLVMinimapIDs[j]].uVertex2ID].x;
        v27 = (unsigned __int16 *)(pIndoor->pVertices[pIndoor->pMapOutlines->pOutlines[pBlueFacesInBLVMinimapIDs[j]].uVertex1ID].x - pCenterX);
        v28 = pIndoor->pVertices[pIndoor->pMapOutlines->pOutlines[pBlueFacesInBLVMinimapIDs[j]].uVertex1ID].y - pCenterY;
        v29 = pIndoor->pVertices[pIndoor->pMapOutlines->pOutlines[pBlueFacesInBLVMinimapIDs[j]].uVertex2ID].y - pCenterY;

        v87 = fixpoint_mul((signed int)v27, viewparams->uMapBookMapZoom);
        v88 = fixpoint_mul(v28, viewparams->uMapBookMapZoom);
        uint i = fixpoint_mul((v26 - pCenterX), viewparams->uMapBookMapZoom);
        v95 = fixpoint_mul(v29, viewparams->uMapBookMapZoom);
        pRenderer->RasterLine2D(screenCenter_X + (fixpoint_mul((signed int)v27, viewparams->uMapBookMapZoom)),
          screenCenterY - v88, screenCenter_X + (fixpoint_mul((v26 - pCenterX), viewparams->uMapBookMapZoom)), screenCenterY - v95, teal);
      }
      viewparams->sViewCenterX = pCenterX;
    }
  }
  v47 = (fixpoint_mul((pParty->vPosition.x - viewparams->sViewCenterX), viewparams->uMapBookMapZoom)) + screenCenter_X - 3;
  v97 = (const void *)(fixpoint_mul((pParty->vPosition.y - pCenterY), viewparams->uMapBookMapZoom));
  v48 = 1;
  v49 = screenCenterY - (int)v97 - 3;
  if ( v47 >= (signed int)tl_x )
  {
    if ( v47 > (signed int)br_x )
    {
      if ( (signed int)((fixpoint_mul((pParty->vPosition.x - viewparams->sViewCenterX), viewparams->uMapBookMapZoom)) + screenCenter_X - 6) > (signed int)br_x )
        v48 = 0;
      v47 = br_x;
    }
  }
  else
  {
    if ( (signed int)((fixpoint_mul((pParty->vPosition.x - viewparams->sViewCenterX), viewparams->uMapBookMapZoom)) + screenCenter_X) < (signed int)tl_x )
      v48 = 0;
    v47 = tl_x;
  }
  if ( v49 >= (signed int)tl_y )
  {
    if ( v49 > br_y )
    {
      if ( screenCenterY - (signed int)v97 - 6 > br_y )
        v48 = 0;
      v49 = br_y;
    }
  }
  else
  {
    if ( screenCenterY - (signed int)v97 < (signed int)tl_y )
      v48 = 0;
    v49 = tl_y;
  }
  if ( v48 == 1 )
  {
    v51 = pParty->sRotationY & stru_5C6E00->uDoublePiMask;
    if ( (signed int)v51 <= 1920 )
      v50 = 6;
    if ( (signed int)v51 < 1664 )
      v50 = 5;
    if ( (signed int)v51 <= 1408 )
      v50 = 4;
    if ( (signed int)v51 < 1152 )
      v50 = 3;
    if ( (signed int)v51 <= 896 )
      v50 = 2;
    if ( (signed int)v51 < 640 )
      v50 = 1;
    if ( (signed int)v51 <= 384 )
      v50 = 0;
    if ( (signed int)v51 < 128 || (signed int)v51 > 1920 )
      v50 = 7;
    pRenderer->DrawTransparentRedShade(v47, v49, pIcons_LOD->GetTexture(pTextureIDs_pMapDirs[v50]));
  }
  if ( (signed int)uNumLevelDecorations > 0 )
  {
    for ( uint i = 0; i < (signed int)uNumLevelDecorations; ++i )
    {
      if (pLevelDecorations[i].uFlags & LEVEL_DECORATION_VISIBLE_ON_MAP)
      {
        screenHeight = pLevelDecorations[i].vPosition.y - pCenterY;
        v93 = (unsigned __int16 *)(pLevelDecorations[i].vPosition.x - viewparams->sViewCenterX);
        v54 = (fixpoint_mul((signed int)v93, viewparams->uMapBookMapZoom)) + screenCenter_X;
        v97 = (const void *)(fixpoint_mul(screenHeight, viewparams->uMapBookMapZoom));
        v55 = screenCenterY - (int)v97;
        //if ( v54 >= pRenderer->raster_clip_x && v54 <= pRenderer->raster_clip_z
        //  && v55 >= pRenderer->raster_clip_y && v55 <= pRenderer->raster_clip_w )
        {
          if ( viewparams->uMapBookMapZoom > 512 )
          {
            pRenderer->RasterLine2D(v54 - 1, v55 - 1, v54 - 1, v55 + 1, Color16(0xFFu, 0xFFu, 0xFFu));
            pRenderer->RasterLine2D(v54,     v55 - 1, v54,     v55 + 1, Color16(0xFFu, 0xFFu, 0xFFu));
            pRenderer->RasterLine2D(v54 + 1, v55 - 1, v54 + 1, v55 + 1, Color16(0xFFu, 0xFFu, 0xFFu));
          }
          else
            pRenderer->RasterLine2D(v54, screenCenterY - (int)v97,
                   (fixpoint_mul((signed int)v93, viewparams->uMapBookMapZoom)) + screenCenter_X,
                   screenCenterY - (int)v97, Color16(0xFFu, 0xFFu, 0xFFu));
        }
      }
    }
  }
  if ( uCurrentlyLoadedLevelType == LEVEL_Outdoor)
  {
    screenCenterY = br_x - tl_x + 1;
    v95 = br_y - tl_y + 1;
    //v77 = &pRenderer->pTargetSurface[tl_x + tl_y * pRenderer->uTargetSurfacePitch];
    black = (1 << (pIcons_LOD->pTextures[viewparams->uTextureID_LocationMap].uWidthLn2 + 16)) / viewparams->uMapBookMapZoom;
    v57 = (double)(1 << (16 - pIcons_LOD->pTextures[viewparams->uTextureID_LocationMap].uWidthLn2));
    v60 = (int)((signed __int64)((double)(viewparams->sViewCenterX - (22528 / (viewparams->uMapBookMapZoom / 384)) + 32768) / v57)) << 16;
    teal = v60 >> 16;
    pPalette_16 = (unsigned __int16 *)(v60 >> 16);
    v97 = (const void *)((int)((signed __int64)((double)(viewparams->sViewCenterX - (22528 / (viewparams->uMapBookMapZoom / 384)) + 32768) / v57)) << 16);
    v62 = (int)((signed __int64)((double)(32768 - (22528 / (viewparams->uMapBookMapZoom / 384)) - pCenterY) / v57)) << 16;
    v63 = (signed __int16)((signed __int64)((double)(32768 - (22528 / (viewparams->uMapBookMapZoom / 384)) - pCenterY) / v57));


    for (int y = 0; y < (signed int)v95; ++y)
    {
        map_tile_Y = (v63 - 80) / 4;
        v64 = teal;
        for (int x = 0; x < screenCenterY; ++x)
        {
          map_tile_X = (v64 - 80) / 4;
          if ( !pOutdoor->IsMapCellFullyRevealed(map_tile_X, map_tile_Y) )
          {
            if ( pOutdoor->IsMapCellPartiallyRevealed(map_tile_X, map_tile_Y) )
            {
              if ( !((x + screenCenter_X) % 2) )
                //*a4a = Color16(12, 12, 12);
                  pRenderer->WritePixel16(tl_x + x, tl_y + y, Color16(12, 12, 12));
            }
            else
              //*a4a = 0;
                  pRenderer->WritePixel16(tl_x + x, tl_y + y, Color16(0, 0, 0));
          }
          v97 = (char *)v97 + black;
          v64 = (signed int)v97 >> 16;
        }

      v62 += black;
      v97 = (const void *)v60;
      v63 = v62 >> 16;
      teal = (unsigned int)pPalette_16;
    }
    /*a4a = map_texture_16;
    for ( screenCenter_X = 0; screenCenter_X < (signed int)v95; ++screenCenter_X )
    {
      if ( screenCenterY > 0 )
      {
        map_tile_Y = (v63 - 80) / 4;
        v64 = teal;
        for ( a5a = 0; a5a < screenCenterY; ++a5a )
        {
          map_tile_X = (v64 - 80) / 4;
          if ( !pOutdoor->_47F04C(map_tile_X, map_tile_Y) )
          {
            if ( pOutdoor->_47F097(map_tile_X, map_tile_Y) )
            {
              if ( !((a5a + screenCenter_X) % 2) )
                *a4a = Color16(0xCu, 0xCu, 0xCu);
            }
            else
              *a4a = 0;
          }
          ++a4a;
          v97 = (char *)v97 + black;
          v64 = (signed int)v97 >> 16;
        }
      }
      v62 += black;
      v97 = (const void *)v60;
      a4a += screenCenterY - a5a;
      v63 = v62 >> 16;
      teal = (unsigned int)pPalette_16;
    }

    v66 = map_texture_16;
    if ( (signed int)v95 > 0 )
    {
      v67 = v77;
      result = 2 * (pRenderer->uTargetSurfacePitch - screenCenterY);
      for ( v65 = v95; v65; --v65 )
      {
        if ( screenCenterY > 0 )
        {
          for ( v68 = screenCenterY; v68; --v68 )
          {
            v69 = *(short *)v66;
            if ( !*(short *)v66 || v69 == (short)result )
              *v67 = v69;
            ++v66;
            ++v67;
          }
        }
        v67 = (unsigned __int16 *)((char *)v67 + result);
      }
    }*/
  }
}

//----- (00444564) --------------------------------------------------------
const char * GetMapBookHintText()
{
  int v20;
  int v21; // [sp+14h] [bp-Ch]@1
  double v0; // st7@3
  unsigned int pX; // [sp+1Ch] [bp-4h]@3
  unsigned int pY; // [sp+8h] [bp-18h]@3
  int global_coord_X; // ebx@3
  int global_coord_Y;
  int map_tile_X; // edi@3
  int map_tile_Y; // eax@3
  const char *result; // eax@15

  v20 = viewparams->sViewCenterX;
  v21 = viewparams->sViewCenterY;
  if ( viewparams->uMapBookMapZoom == 384 )
  {
    v20 = viewparams->indoor_center_x;
    v21 = viewparams->indoor_center_y;
  }
  pMouse->GetClickPos(&pX, &pY);
  v0 = 1.0 / (float)((signed int)viewparams->uMapBookMapZoom * 0.000015258789);

  global_coord_X = (signed __int64)((double)(pX - 229) * v0 + (double)v20);
  global_coord_Y = (signed __int64)((double)v21 - (double)(pY - 181) * v0);

  result = 0;
  map_tile_X = abs(global_coord_X + 22528) / 512;//In the mapbook only lady Margaret dispays for defoult zoom(В книге карты только Леди Маргарита всплывает при дефолтном зуме)
  map_tile_Y = abs(global_coord_Y - 22528) / 512;
  if ( pOutdoor->IsMapCellFullyRevealed(map_tile_X, map_tile_Y) && uCurrentlyLoadedLevelType == LEVEL_Outdoor && (signed int)pOutdoor->uNumBModels > 0 )
  {
    for(int i = 0; i < pOutdoor->uNumBModels && !result; i++)
    {
      if ( int_get_vector_length(abs((signed)pOutdoor->pBModels[i].vBoundingCenter.x - global_coord_X),
                                 abs((signed)pOutdoor->pBModels[i].vBoundingCenter.y - global_coord_Y), 0) < pOutdoor->pBModels[i].sBoundingRadius )
      {
        if ( pOutdoor->pBModels[i].uNumFaces > 0 )
        {
          for( int j = 0; j < pOutdoor->pBModels[i].uNumFaces; j++ )
          {
            if ( pOutdoor->pBModels[i].pFaces[j].sCogTriggeredID )
            {
              if ( !(pOutdoor->pBModels[i].pFaces[j].uAttributes & FACE_UNKNOW) )
              {
                if ( GetEventHintString(pOutdoor->pBModels[i].pFaces[j].sCogTriggeredID) )
                {
                  if ( _stricmp(GetEventHintString(pOutdoor->pBModels[i].pFaces[j].sCogTriggeredID), "") )
                    result = GetEventHintString(pOutdoor->pBModels[i].pFaces[j].sCogTriggeredID);
                }
              }
            }
          }
        }
      }
    }
  }
  return result;
}