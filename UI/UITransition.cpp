#define _CRT_SECURE_NO_WARNINGS
#include <io.h>

#include "..\ErrorHandling.h"
#include "..\mm7_unsorted_subs.h"
#include "..\mm7_data.h"
#include "..\VideoPlayer.h"
#include "..\MapInfo.h"
#include "..\GUIWindow.h"
#include "..\GUIFont.h"
#include "..\GUIProgressBar.h"
#include "..\Party.h"
#include "..\AudioPlayer.h"
#include "..\Outdoor.h"
#include "..\LOD.h"
#include "..\Timer.h"
#include "..\stru159.h"
#include "..\Events2D.h"
#include "..\texts.h"
#include "UIHouses.h"




//----- (00444839) --------------------------------------------------------
void TransitionUI_Load(uint anim_id, uint exit_pic_id, int x, int y, int z, int directiony, int directionx, int a8, const char *pLocationName)
{
  const char *v15; // eax@14
  char pContainer[40]; // [sp+Ch] [bp-2Ch]@1

  dword_59117C_teleportx = x;
  dword_591178_teleporty = y;
  dword_591174_teleportz = z;
  dword_591170_teleport_directiony = directiony;
  dword_59116C_teleport_directionx = directionx;
  dword_591168_teleport_speedz = a8;
  dword_591164_teleport_map_name = (char *)pLocationName;
  uCurrentHouse_Animation = anim_id;
  pEventTimer->Pause();
  pAudioPlayer->StopChannels(-1, -1);

  switch (pParty->alignment)
  {
    case PartyAlignment_Good:    sprintfex(pContainer, "evt%02d-b", const_2()); break;
    case PartyAlignment_Neutral: sprintfex(pContainer, "evt%02d", const_2());   break;
    case PartyAlignment_Evil:    sprintfex(pContainer, "evt%02d-c", const_2()); break;
    default: Error("Invalid alignment: %u", pParty->alignment);
  }

  pTexture_Dialogue_Background = &pIcons_LOD->pTextures[pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE)];
  pTexture_outside = pIcons_LOD->LoadTexturePtr(pHouse_ExitPictures[exit_pic_id], TEXTURE_16BIT_PALETTE);
  if (anim_id)
  {
    if ( !IndoorLocation::GetLocationIndex(pLocationName) )
      pVideoPlayer->OpenHouseMovie(pAnimatedRooms[p2DEvents[anim_id - 1].uAnimationID].video_name, 1);
  }
  else if ( !IndoorLocation::GetLocationIndex(pLocationName) )
  {
    if ( pMapStats->GetMapInfo(pCurrentMapName) )
    {
      sprintfex(sHouseName.data(), pGlobalTXT_LocalizationStrings[410], pMapStats->pInfos[pMapStats->GetMapInfo(pCurrentMapName)].pName); // "Leave %s"
      pDialogueWindow = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_Transition, 0, sHouseName.data());
      //if ( pAnimatedRooms[p2DEvents[anim_id].uAnimationID].uRoomSoundId )
        //PlayHouseSound(anim_id, HouseSound_Greeting);
      if ( uCurrentlyLoadedLevelType == LEVEL_Indoor && uActiveCharacter && pParty->uFlags & 0x30 )
        pPlayers[uActiveCharacter]->PlaySound(SPEECH_47, 0);
      if ( IndoorLocation::GetLocationIndex(pLocationName) )
        uCurrentHouse_Animation = IndoorLocation::GetLocationIndex(pLocationName);
      return;
    }
    strcpy(sHouseName.data(), pGlobalTXT_LocalizationStrings[79]);
    pDialogueWindow = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_Transition, 0, sHouseName.data());
    //if ( pAnimatedRooms[p2DEvents[anim_id].uAnimationID].uRoomSoundId )
      //PlayHouseSound(anim_id, HouseSound_Greeting);
    if ( uCurrentlyLoadedLevelType == LEVEL_Indoor && uActiveCharacter && pParty->uFlags & 0x30 )
      pPlayers[uActiveCharacter]->PlaySound(SPEECH_47, 0);
    if ( IndoorLocation::GetLocationIndex(pLocationName) )
      uCurrentHouse_Animation = IndoorLocation::GetLocationIndex(pLocationName);
    return;
  }
  v15 = pLocationName;
  if ( *pLocationName == 48 )
    v15 = pCurrentMapName;
  if ( pMapStats->GetMapInfo(v15) )
  {
    sprintfex(sHouseName.data(), pGlobalTXT_LocalizationStrings[411], pMapStats->pInfos[pMapStats->GetMapInfo(v15)].pName);//Войти в ^Pv[%s]
    pDialogueWindow = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_Transition, 0, sHouseName.data());
    if ( uCurrentlyLoadedLevelType == LEVEL_Indoor && uActiveCharacter && pParty->uFlags & 0x30 )
      pPlayers[uActiveCharacter]->PlaySound(SPEECH_47, 0);
    if ( IndoorLocation::GetLocationIndex(pLocationName) )
      uCurrentHouse_Animation = IndoorLocation::GetLocationIndex(pLocationName);
    return;
  }
  strcpy(sHouseName.data(), pGlobalTXT_LocalizationStrings[73]);//Войти
  pDialogueWindow = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_Transition, 0, sHouseName.data());
  //if ( pAnimatedRooms[p2DEvents[anim_id].uAnimationID].uRoomSoundId )
    //PlayHouseSound(anim_id, HouseSound_Greeting);
  if ( uCurrentlyLoadedLevelType == LEVEL_Indoor && uActiveCharacter && pParty->uFlags & 0x30 )
    pPlayers[uActiveCharacter]->PlaySound(SPEECH_47, 0);
  if ( IndoorLocation::GetLocationIndex(pLocationName) )
    uCurrentHouse_Animation = IndoorLocation::GetLocationIndex(pLocationName);
}

//----- (00444C8F) --------------------------------------------------------
void TravelUI_Load()
{
  char pContainer[32]; // [sp+0h] [bp-28h]@1

  pEventTimer->Pause();

  switch (pParty->alignment)
  {
    case PartyAlignment_Good:    sprintfex(pContainer, "evt%02d-b", const_2()); break;
    case PartyAlignment_Neutral: sprintfex(pContainer, "evt%02d", const_2());   break;
    case PartyAlignment_Evil:    sprintfex(pContainer, "evt%02d-c", const_2()); break;
    default: Error("Invalid alignment: %u", pParty->alignment);
  }

  pTexture_Dialogue_Background = pIcons_LOD->LoadTexturePtr(pContainer, TEXTURE_16BIT_PALETTE);
  pTexture_outside = pIcons_LOD->LoadTexturePtr("outside", TEXTURE_16BIT_PALETTE);
  if ( pMapStats->GetMapInfo(pCurrentMapName) )
    sprintfex(sHouseName.data(), pGlobalTXT_LocalizationStrings[410], pMapStats->pInfos[pMapStats->GetMapInfo(pCurrentMapName)].pName);// "Leave %s"
  else
    strcpy(sHouseName.data(), pGlobalTXT_LocalizationStrings[79]);// "Exit"
  pDialogueWindow = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_ChangeLocation, 0, sHouseName.data());
}

//----- (00444DCA) --------------------------------------------------------
void TravelUI_Draw()
{
  GUIWindow travel_window; // [sp+Ch] [bp-78h]@1
  char pDestinationMapName[32]; // [sp+60h] [bp-24h]@1

  memcpy(&travel_window, pPrimaryWindow, sizeof(travel_window));
  pOutdoor->GetTravelDestination(pParty->vPosition.x, pParty->vPosition.y, pDestinationMapName, 20);
  pRenderer->DrawTextureIndexed(477, 0, pTexture_Dialogue_Background);
  pRenderer->DrawTextureTransparent(468, 0, &pIcons_LOD->pTextures[uTextureID_right_panel_loop]);
  pRenderer->DrawTextureIndexed(pNPCPortraits_x[0][0], pNPCPortraits_y[0][0], pTexture_outside);
  pRenderer->DrawTextureIndexed(556, 451, pIcons_LOD->GetTexture(uTextureID_x_x_u));
  pRenderer->DrawTextureIndexed(476, 451, pIcons_LOD->GetTexture(uTextureID_x_ok_u));
  if ( pMapStats->GetMapInfo(pDestinationMapName) )
  {
    travel_window.uFrameX = 493;
    travel_window.uFrameWidth = 126;
    travel_window.uFrameZ = 366;
    travel_window.DrawTitleText(pFontCreate, 0, 4, 0, pMapStats->pInfos[pMapStats->GetMapInfo(pDestinationMapName)].pName, 3);
    travel_window.uFrameX = 483;
    travel_window.uFrameWidth = 148;
    travel_window.uFrameZ = 334;
    if ( GetTravelTime() == 1 )
      sprintfex(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[663], 1, pMapStats->pInfos[pMapStats->GetMapInfo(pDestinationMapName)].pName);// "It will take %d day to cross to %s."
    else
      sprintfex(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[128], GetTravelTime(), pMapStats->pInfos[pMapStats->GetMapInfo(pDestinationMapName)].pName);// "It will take %d days to travel to %s."

    strcat(pTmpBuf.data(), "\n \n");
    sprintfex(pTmpBuf2.data(), pGlobalTXT_LocalizationStrings[126], pMapStats->pInfos[pMapStats->GetMapInfo(pCurrentMapName)].pName);
    strcat(pTmpBuf.data(), pTmpBuf2.data());
    travel_window.DrawTitleText(pFontCreate, 0, (212 - pFontCreate->CalcTextHeight(pTmpBuf.data(), &travel_window, 0, 0)) / 2 + 101, 0, pTmpBuf.data(), 3);
    _unused_5B5924_is_travel_ui_drawn = 1;
  }
}

//----- (00444A51) --------------------------------------------------------
void TransitionUI_Draw()
{
  unsigned int v4; // [sp-10h] [bp-7Ch]@12
  GUIWindow transition_window; // [sp+Ch] [bp-60h]@1
  unsigned int v9; // [sp+60h] [bp-Ch]@1
  unsigned int map_id; // [sp+64h] [bp-8h]@1

  memcpy(&transition_window, pPrimaryWindow, sizeof(transition_window));
  v9 = IndoorLocation::GetLocationIndex(dword_591164_teleport_map_name);
  pRenderer->DrawTextureIndexed(0x1DDu, 0, pTexture_Dialogue_Background);
  pRenderer->DrawTextureIndexed(pNPCPortraits_x[0][0] - 4, pNPCPortraits_y[0][0] - 4, pIcons_LOD->GetTexture(uTextureID_50795C));
  pRenderer->DrawTextureIndexed(pNPCPortraits_x[0][0], pNPCPortraits_y[0][0], pTexture_outside);
  uTextureID_right_panel_loop = uTextureID_right_panel;
  pRenderer->DrawTextureTransparent(468, 0, pIcons_LOD->GetTexture(uTextureID_right_panel));
  pRenderer->DrawTextureIndexed(556, 451, pIcons_LOD->GetTexture(uTextureID_x_x_u));
  pRenderer->DrawTextureIndexed(476, 451, pIcons_LOD->GetTexture(uTextureID_x_ok_u));
  map_id = pMapStats->GetMapInfo(pCurrentMapName);
  if ( (pMovie || v9) && *dword_591164_teleport_map_name != ' ' )
    map_id = pMapStats->GetMapInfo(dword_591164_teleport_map_name);
  transition_window.uFrameX = 493;
  transition_window.uFrameWidth = 126;
  transition_window.uFrameZ = 366;
  transition_window.DrawTitleText(pFontCreate, 0, 5, 0, pMapStats->pInfos[map_id].pName, 3);
  transition_window.uFrameX = 483;
  transition_window.uFrameWidth = 148;
  transition_window.uFrameZ = 334;

  if ( uCurrentHouse_Animation )
  {
    v4 = (212 - pFontCreate->CalcTextHeight(pTransitionStrings[uCurrentHouse_Animation], &transition_window, 0, 0)) / 2 + 101;
    transition_window.DrawTitleText(pFontCreate, 0, v4, 0, pTransitionStrings[uCurrentHouse_Animation], 3);
  }
  else if ( map_id )
  {
    sprintfex(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[409], pMapStats->pInfos[map_id].pName);// "Do you wish to leave %s?"
    v4 = (212 - pFontCreate->CalcTextHeight(pTmpBuf.data(), &transition_window, 0, 0)) / 2 + 101;
    transition_window.DrawTitleText(pFontCreate, 0, v4, 0, pTmpBuf.data(), 3);
  }
  else Error("Troubles in da house");

  _unused_5B5924_is_travel_ui_drawn = true;
}
