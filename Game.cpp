#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define _CRT_SECURE_NO_WARNINGS
#include "Arcomage.h"
#include "mm7_unsorted_subs.h"
#include "Vis.h"
#include "Weather.h"
#include "LightmapBuilder.h"
#include "DecalBuilder.h"
#include "ParticleEngine.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "GammaControl.h"
#include "stru6.h"
#include "stru9.h"
#include "stru10.h"
#include "ErrorHandling.h"
#include "MediaPlayer.h"

#include "Game.h"
#include "Party.h"
#include "Viewport.h"
#include "Timer.h"
#include "Outdoor.h"
#include "Outdoor_stuff.h"
#include "Overlays.h"
#include "AudioPlayer.h"
#include "LOD.h"
#include "GUIWindow.h"
#include "TurnEngine.h"
#include "Bink_Smacker.h"
#include "Events.h"
#include "Arcomage.h"
#include "texts.h"
#include "GUIFont.h"
#include "Log.h"
#include "Lights.h"
#include "mm7_data.h"
#include "MapInfo.h"
#include "CastSpellInfo.h"
#include "FrameTableInc.h"
#include "Actor.h"
#include "Events2D.h"
#include "GUIProgressBar.h"
#include "ObjectList.h"
#include "Level/Decoration.h"
#include "PaletteManager.h"
#include "UI\UIHouses.h"
#include "UI\UIShops.h"
#include "UI\UIPartyCreation.h"
#include "SaveLoad.h"
#include "SpriteObject.h"
#include "mm7.h"
#include "Sprites.h"
#include "Registry.h"
#include "Chest.h"

#include "UI\UIGame.h"




Game *pGame;





//----- (00435694) --------------------------------------------------------
void Game::ToggleFlags2(unsigned int uFlag)
{
  unsigned int v2; // eax@1

  v2 = this->uFlags2;
  if ( v2 & uFlag )
    this->uFlags2 = v2 & ~uFlag;
  else
    this->uFlags2 = uFlag | v2;
}

//----- (0044103C) --------------------------------------------------------
void Game::Draw()
{
  int v4; // edi@26

  uFlags2 &= ~0x02;
  if ( pParty->_497FC5_check_party_perception_against_level() )
    uFlags2 |= 2;

  pGame->pIndoorCameraD3D->sRotationX = pParty->sRotationX;
  pGame->pIndoorCameraD3D->sRotationY = pParty->sRotationY;
  pGame->pIndoorCameraD3D->vPartyPos.x = pParty->vPosition.x - pParty->y_rotation_granularity * cosf(2 * 3.141592653589 * pParty->sRotationY / 2048.0);
  pGame->pIndoorCameraD3D->vPartyPos.y = pParty->vPosition.y - pParty->y_rotation_granularity * sinf(2 * 3.141592653589 * pParty->sRotationY / 2048.0);
  pGame->pIndoorCameraD3D->vPartyPos.z = pParty->vPosition.z + pParty->sEyelevel;//193, but real 353

  //pIndoorCamera->Initialize2();
  pIndoorCameraD3D->CalculateRotations(pParty->sRotationX, pParty->sRotationY);
  pIndoorCameraD3D->CreateWorldMatrixAndSomeStuff();
  pIndoorCameraD3D->_4374E8_ProllyBuildFrustrum();

  if ( pMovie_Track )
  {
    /*if ( !pRenderer->pRenderD3D )
    {
		pRenderer->BeginSceneD3D();
		pMouse->DrawCursorToTarget();
		pRenderer->DrawBillboards_And_MaybeRenderSpecialEffects_And_EndScene();
	}*/
  }
  else
  {
    if ( pParty->vPosition.x != pParty->vPrevPosition.x || pParty->sRotationY != pParty->sPrevRotationY || pParty->vPosition.y != pParty->vPrevPosition.y 
       || pParty->sRotationX != pParty->sPrevRotationX || pParty->vPosition.z != pParty->vPrevPosition.z || pParty->sEyelevel != pParty->sPrevEyelevel )
      pParty->uFlags |= 2u;
    pParty->vPrevPosition.x = pParty->vPosition.x;
    pParty->vPrevPosition.y = pParty->vPosition.y;
    pParty->vPrevPosition.z = pParty->vPosition.z;
    //v0 = &pRenderer;
    pParty->sPrevRotationY = pParty->sRotationY;
    pParty->sPrevRotationX = pParty->sRotationX;

    pParty->sPrevEyelevel = pParty->sEyelevel;
    pRenderer->BeginSceneD3D();

    //if ( !pRenderer->pRenderD3D )
      //pMouse->DrawCursorToTarget();
    if ( !PauseGameDrawing() || viewparams->field_48 == 1 )
    {
      //if ( pRenderer->pRenderD3D )
      {
        float v2 = (double)(((signed int)pMiscTimer->uTotalGameTimeElapsed >> 2) & 0x1F) * 0.032258064 * 6.0;
        //v3 = v2 + 6.7553994e15;
        //pRenderer->field_1036A8_bitmapid = LODWORD(v3);
        pRenderer->hd_water_current_frame = floorf(v2 + 0.5f);
      }

      if (uCurrentlyLoadedLevelType == LEVEL_Indoor)
        pIndoor->Draw();
      else if (uCurrentlyLoadedLevelType == LEVEL_Outdoor)
        pOutdoor->Draw();
      else 
        Error("Invalid level type: %u", uCurrentlyLoadedLevelType);

      //if (pRenderer->pRenderD3D)
      {
        pDecalBuilder->DrawBloodsplats();
        pGame->pLightmapBuilder->DrawLightmapsType(2);
      }
    }
    pRenderer->DrawBillboards_And_MaybeRenderSpecialEffects_And_EndScene();
  }

  //DEBUG: force redraw gui
  viewparams->bRedrawGameUI = true;


  pRenderer->BeginScene();
  //if (pRenderer->pRenderD3D)
    pMouse->DrawCursorToTarget();
  if (pOtherOverlayList->bRedraw)
    viewparams->bRedrawGameUI = true;
  v4 = viewparams->bRedrawGameUI;
  GameUI_Footer();
  if (!viewparams->bRedrawGameUI)
    GameUI_DrawRightPanelItems();
  else
  {
    GameUI_DrawRightPanelFrames();
    GameUI_Footer_2();
    viewparams->bRedrawGameUI = false;
  }
  if (!pMovie_Track)//!pVideoPlayer->pSmackerMovie)
  {
    GameUI_DrawMinimap(488, 16, 625, 133, viewparams->uMinimapZoom, true);//redraw = pParty->uFlags & 2);
    if (v4)
    {
      if ( !PauseGameDrawing() /*&& pRenderer->pRenderD3D*/) // clear game viewport with transparent color
        pRenderer->FillRectFast(pViewport->uViewportTL_X, pViewport->uViewportTL_Y, pViewport->uViewportBR_X - pViewport->uViewportTL_X,
                                pViewport->uViewportBR_Y - pViewport->uViewportTL_Y + 1,
                                0x7FF);
      viewparams->field_48 = 0;
    }
  }

  viewparams->bRedrawGameUI = pOtherOverlayList->bRedraw;
  pOtherOverlayList->bRedraw = 0;

  GameUI_DrawPartySpells();
  if (v4 || pParty->pHirelings[0].evt_C || pParty->pHirelings[1].evt_C )
    GameUI_DrawHiredNPCs();
  GameUI_DrawPortraits(v4);
  GameUI_DrawLifeManaBars();
  GameUI_DrawCharacterSelectionFrame();
  if ( _44100D_should_alter_right_panel() )
    GameUI_DrawRightPanel();
  if ( !pMovie_Track )
  {
    pStru6Instance->DrawPlayerBuffAnims();
    pOtherOverlayList->DrawTurnBasedIcon(v4);
    GameUI_DrawTorchlightAndWizardEye();
  }


  static bool render_framerate = false;
  static float framerate = 0.0f;
  static uint frames_this_second = 0;
  static uint last_frame_time = GetTickCount();
  static uint framerate_time_elapsed = 0;
  if ( pCurrentScreen == SCREEN_GAME && uCurrentlyLoadedLevelType == LEVEL_Outdoor)
    pWeather->Draw();//Ritor1: my include

  uint frame_dt = GetTickCount() - last_frame_time;
  last_frame_time = GetTickCount();

  framerate_time_elapsed += frame_dt;
  if (framerate_time_elapsed >= 1000)
  {
    framerate = frames_this_second *  (1000.0f / framerate_time_elapsed);

    framerate_time_elapsed = 0;
    frames_this_second = 0;
    render_framerate = true;
  }

  ++frames_this_second;

  if ( debug_information )
  {
    if (render_framerate)
    {
      sprintf(pTmpBuf.data(), "FPS: % .4f", framerate);
      pPrimaryWindow->DrawText(pFontArrus, 494, 0, Color16(0, 0, 0), pTmpBuf.data(), 0, 0, 0);
    }

    if (uCurrentlyLoadedLevelType == LEVEL_Indoor)
    {
      int sector_id = pIndoor->GetSector(pParty->vPosition.x, pParty->vPosition.y, pParty->vPosition.z);
      sprintf(pTmpBuf.data(), "Party Sector ID:        %u/%u\n", sector_id, pIndoor->uNumSectors);
      pPrimaryWindow->DrawText(pFontArrus, 16, 16, Color16(255, 255, 255), pTmpBuf.data(), 0, 0, -1);
    }
    sprintf(pTmpBuf.data(), "Party Position:         % d % d % d", pParty->vPosition.x, pParty->vPosition.y, pParty->vPosition.z);
    pPrimaryWindow->DrawText(pFontArrus, 16, 16 + 16, Color16(255, 255, 255), pTmpBuf.data(), 0, 0, -1);
  
    if (uCurrentlyLoadedLevelType == LEVEL_Indoor)
    {
      uint uFaceID;
      int sector_id = pIndoor->GetSector(pParty->vPosition.x, pParty->vPosition.y, pParty->vPosition.z);
      int floor_level = BLV_GetFloorLevel(pParty->vPosition.x, pParty->vPosition.y, pParty->vPosition.z + 40, sector_id, &uFaceID);
      sprintf(pTmpBuf.data(), "BLV_GetFloorLevel: %d   face_id %d\n", floor_level, uFaceID);
    }
    else
    {
      int on_water, _a6;
      int floor_level = ODM_GetFloorLevel(pParty->vPosition.x, pParty->vPosition.y, pParty->vPosition.z, 0, &on_water, &_a6, false);
      sprintf(pTmpBuf.data(), "ODM_GetFloorLevel: %d   on_water: %s    a6 = %d\n", floor_level, on_water ? "true" : "false", _a6);
    }
    pPrimaryWindow->DrawText(pFontArrus, 16, 16 + 16 + 16, Color16(255, 255, 255), pTmpBuf.data(), 0, 0, -1);
  }

  GUI_UpdateWindows();
  pParty->UpdatePlayersAndHirelingsEmotions();

  _unused_5B5924_is_travel_ui_drawn = false;
  if (v4)
    pMouse->bRedraw = true;
  pMouse->ReadCursorWithItem();
  pMouse->DrawCursor();
  pMouse->Activate();
  pRenderer->EndScene();
  pRenderer->Present();
  pParty->uFlags &= ~2;
}

//----- (0047A815) --------------------------------------------------------
void Game::DrawParticles()
{
  pParticleEngine->Draw();
}

//----- (00463149) --------------------------------------------------------
void Game::Loop()
{
  const char *pLocationName; // [sp-4h] [bp-68h]@74
  bool bLoading; // [sp+10h] [bp-54h]@1
  signed int v16; // [sp+14h] [bp-50h]@8
  MSG Msg; // [sp+28h] [bp-3Ch]@20
  char Source[64]; // [sp+44h] [bp-20h]@76

  bLoading = sCurrentMenuID == MENU_LoadingProcInMainMenu;
  SetCurrentMenuID((MENU_STATE)-1);
  if (bLoading)
  {
    pParty->Reset();
    uGameState = GAME_STATE_PLAYING;
    LoadGame(uLoadGameUI_SelectedSlot);
  }

  for (uint i = 1; i < 5; ++i)
    for (uint j = 1; j < 6; ++j)
    {
      sprintf(pTmpBuf.data(), "data\\lloyd%d%d.pcx", i, j);
      remove(pTmpBuf.data());
    }

  LoadPlayerPortraintsAndVoices();
  pIcons_LOD->_inlined_sub1();
  pAudioPlayer->SetMusicVolume(pSoundVolumeLevels[uMusicVolimeMultiplier] * 64.0f);

  while ( 2 )
  {
    v16 = 1;
    pMessageQueue_50CBD0->Flush();

    pPartyActionQueue->uNumActions = 0;
    if (pParty->bTurnBasedModeOn)
    {
      pTurnEngine->End(false);
      pParty->bTurnBasedModeOn = false;
    }
    DoPrepareWorld(bLoading, 1);
    pEventTimer->Resume();
    dword_6BE364_game_settings_1 |= GAME_SETTINGS_0080_SKIP_USER_INPUT_THIS_FRAME;
    // uGame_if_0_else_ui_id__11_save__else_load__8_drawSpellInfoPopup__22_final_window__26_keymapOptions__2_options__28_videoOptions = 0;
    pCurrentScreen = SCREEN_GAME;

    //if ( pRenderer->pRenderD3D )
      pGame->pVisInstance->_4C1A02();

    bool game_finished = false;
    do
    {
      while ( PeekMessageA(&Msg, 0, 0, 0, PM_REMOVE) )
      {
        if ( Msg.message == WM_QUIT )
          Game_DeinitializeAndTerminate(0);
        TranslateMessage(&Msg);
        DispatchMessageA(&Msg);
      }
      if (dword_6BE364_game_settings_1 & GAME_SETTINGS_APP_INACTIVE)
      {
        WaitMessage();
        continue;
      }
      pGame->_44EEA7();
      GameUI_WritePointedObjectStatusString();
      Keyboard::ProcessInputActions();
      GameUI_MsgProc();
      if ( pArcomageGame->bGameInProgress )
      {
        ArcomageGame::Loop();
        pRenderer->Present();
        continue;
      }

      pMediaPlayer->HouseMovieLoop();
   
      pEventTimer->Update();
      pMiscTimer->Update();

      OnTimer(0);
      GameUI_StatusBar_UpdateTimedString(0);
      if ( pMiscTimer->bPaused && !pEventTimer->bPaused )
        pMiscTimer->Resume();
      if ( pEventTimer->bTackGameTime && !pParty->bTurnBasedModeOn )
        pEventTimer->bTackGameTime = 0;
      if ( !pEventTimer->bPaused && uGameState == GAME_STATE_PLAYING)
      {
        if ( !pEventTimer->bTackGameTime )
          _494035_timed_effects__water_walking_damage__etc();

        if (dword_6BE364_game_settings_1 & GAME_SETTINGS_0001)
          dword_6BE364_game_settings_1 &= ~GAME_SETTINGS_0001;
        else
        {
          Actor::UpdateActorAI();
          UpdateUserInput_and_MapSpecificStuff();
        }
      }
      if ( v16 )
      {
        v16 = 0;
        viewparams->bRedrawGameUI = true;
      }
      pAudioPlayer->UpdateSounds();
      if ( (signed int)uGameState == GAME_STATE_PLAYING )
      {
        pGame->Draw();
        continue;
      }
      if (uGameState == GAME_FINISHED)
      {
        game_finished = true;
        continue;
      }
      if (uGameState == GAME_STATE_CHANGE_LOCATION)// смена локации
      {
        pAudioPlayer->StopChannels(-1, -1);
        PrepareWorld(0);
        uGameState = GAME_STATE_PLAYING;
        continue;
      }
      if ( (signed int)uGameState <= GAME_STATE_5 || uGameState == GAME_STATE_GAME_QUITTING_TO_MAIN_MENU )//GAME_STATE_NEWGAME_OUT_GAMEMENU, GAME_STATE_LOADING_GAME
      {
        game_finished = true;
        continue;
      }
      if ( uGameState == GAME_STATE_FINAL_WINDOW )
      {
        pRenderer->BeginScene();
        GUI_UpdateWindows();
        pRenderer->EndScene();
        pRenderer->Present();
        continue;
      }
      if ( uGameState != GAME_STATE_PARTY_DIED )
      {
        pGame->Draw();
        continue;
      }
      if ( uGameState == GAME_STATE_PARTY_DIED )
      {
        pAudioPlayer->StopChannels(-1, -1);
        memset(&pParty->pHirelings[0], 0, 0x4Cu);
        memset(&pParty->pHirelings[1], 0, 0x4Cu);
        for ( int i = 0; i < (signed int)pNPCStats->uNumNewNPCs; ++i )
        {
          if ( pNPCStats->pNewNPCData[i].field_24 )
            pNPCStats->pNewNPCData[i].uFlags &= 0xFFFFFF7Fu;
        }
        pMediaPlayer->bStopBeforeSchedule = 0;
        pMediaPlayer->PlayFullscreenMovie(MOVIE_Death, true);
        if ( pMovie_Track )
          pMediaPlayer->Unload();
        SaveGame(0, 0);
        ++pParty->uNumDeaths;
        for ( uint i = 0; i < 4; ++i )
          pParty->pPlayers[i].SetVariable(VAR_Award, 85);
        pParty->days_played_without_rest = 0;
        pParty->uTimePlayed += 0x276000ui64;
        LOWORD(pParty->uFlags) &= ~0x204;
        pParty->SetGold(0);
        pOtherOverlayList->Reset();
        memset(pParty->pPartyBuffs.data(), 0, 0x140u);

        if ( pParty->bTurnBasedModeOn == 1 )
        {
          pTurnEngine->End(true);
          pParty->bTurnBasedModeOn = 0;
        }
        for( int i = 0; i < 4; ++i)
        {
          memset(pParty->pPlayers[i].pConditions.data(), 0, 0xA0u);//(pConditions, 0, 160)
          memset(pParty->pPlayers[i].pPlayerBuffs.data(), 0, 0x180u);//(pPlayerBuffs[0], 0, 384)
          pParty->pPlayers[i].sHealth = 1;
          uActiveCharacter = 1;
        }
        if (_449B57_test_bit(pParty->_quest_bits, PARTY_QUEST_FINISHED_EMERALD_ISLE))
        {
          pParty->vPosition.x = -17331;            // respawn in harmondale
          pParty->vPosition.y = 12547;
          pParty->vPosition.z = 465;
          pParty->sRotationY = 0;
          pLocationName = "out02.odm";
        }
        else
        {
          pParty->vPosition.x = 12552;             // respawn on emerald isle
          pParty->vPosition.y = 1816;
          pParty->vPosition.z = 0;
          pParty->sRotationY = 512;
          pLocationName = "out01.odm";
        }
        strcpy(Source, pLocationName);
        pParty->uFallStartY = pParty->vPosition.z;
        pParty->sRotationX = 0;
        pParty->uFallSpeed = 0;
        pParty->field_6E4 = 0;
        pParty->field_6E0 = 0;
        if ( _stricmp(Source, pCurrentMapName) )
        {
          strcpy(pCurrentMapName, Source);
          _5B65A8_npcdata_uflags_or_other = pParty->vPosition.x;
          _5B65AC_npcdata_fame_or_other = pParty->vPosition.y;
          _5B65B0_npcdata_rep_or_other = pParty->vPosition.z;
          _5B65B4_npcdata_loword_house_or_other = pParty->sRotationY;
          _5B65B8_npcdata_hiword_house_or_other = pParty->sRotationX;
          dword_5B65C0 = 1;
          PrepareWorld(1);
        }
        Actor::InitializeActors();

        int num_conscious_players = 0;
        int conscious_players_ids[4] = {-1, -1, -1, -1};
        for (int i = 0; i < 4; ++i)
        {
          if (pParty->pPlayers[i].CanAct())
            conscious_players_ids[num_conscious_players++] = i;
        }
        if (num_conscious_players)
        {
          int idx = conscious_players_ids[rand() % num_conscious_players];
          Assert(idx >= 0);
          pParty->pPlayers[idx].PlaySound(SPEECH_99, 0);
        }

        ShowStatusBarString(pGlobalTXT_LocalizationStrings[524], 2);// "Once again you've cheated death!.." "Вы снова обхитрили смерть! …"
        uGameState = GAME_STATE_PLAYING;
      }
    }
    while (!game_finished);

    pEventTimer->Pause();
    ResetCursor_Palettes_LODs_Level_Audio_SFT_Windows();
    if ( uGameState == GAME_STATE_LOADING_GAME )
    {
      sub_491E3A();
      LoadPlayerPortraintsAndVoices();
      uGameState = GAME_STATE_PLAYING;
      pIcons_LOD->dword_11B84 = pIcons_LOD->uNumLoadedFiles;
      bLoading = true;
      continue;
    }
    break;
  }
  pCurrentScreen = SCREEN_VIDEO;
  sub_491E3A();
}


//----- (0044F192) --------------------------------------------------------
void Game::PrepareBloodsplats()
{
  for (uint i = 0; i < uNumBloodsplats; ++i)
  {
    pBloodsplatContainer->AddBloodsplat(pBloodsplats[i].x, pBloodsplats[i].y, pBloodsplats[i].z,
        pBloodsplats[i].radius, pBloodsplats[i].r, pBloodsplats[i].g, pBloodsplats[i].b);
   }
}


//----- (0044F120) --------------------------------------------------------
void Game::PushStationaryLights(int a2)
{
  Game__StationaryLight* pLight;
 
  for( int i=0; i<uNumStationaryLights; ++i ) 
      {
       pLight=&pStationaryLights[i];
       pStationaryLightsStack->AddLight(pLight->vPosition.x, pLight->vPosition.y, pLight->vPosition.z,
           pLight->flt_18, pLight->vRGBColor.x, pLight->vRGBColor.y, pLight->vRGBColor.z, _4E94D0_light_type);
      }
}
// 4E94D0: using guessed type char _4E94D0_light_type;

//----- (0044F0FD) --------------------------------------------------------
void Game::_44F0FD()
{
  ToggleFlags(0x40u);

  if ( !(uFlags & 0x40) )
  {
    uNumBloodsplats = 0;
    field_E0C = 0;
  }
}

//----- (0044F0D8) --------------------------------------------------------
void Game::ToggleFlags(uint uMask)
{
  if (uFlags & uMask)
    uFlags &= ~uMask;
  else
    uFlags |= uMask;
}


//----- (0044F07B) --------------------------------------------------------
bool Game::_44F07B()
{
  if (!pKeyboardInstance->IsKeyBeingHeld(VK_SHIFT) && !pKeyboardInstance->IsKeyBeingHeld(VK_LSHIFT) &&
      !pKeyboardInstance->IsKeyBeingHeld(VK_LSHIFT) || (pKeyboardInstance->WasKeyPressed(VK_F11) == 0 &&
       pKeyboardInstance->WasKeyPressed(VK_F11)))
    return true;
  return false;
}

//----- (0044EEA7) --------------------------------------------------------
bool Game::_44EEA7()
{
  //Game *v1; // esi@1
  //double v2; // st7@2
  float depth; // ST00_4@9
  //bool result; // eax@9
  //unsigned int v5; // eax@14
  __int64 v6; // kr00_8@21
  //unsigned int y; // [sp+4h] [bp-24h]@2
  //unsigned int x; // [sp+8h] [bp-20h]@2
  Vis_SelectionFilter *v10; // [sp+10h] [bp-18h]@2
  Vis_SelectionFilter *v11; // [sp+14h] [bp-14h]@2
  POINT cursor; // [sp+20h] [bp-8h]@1

  //v1 = this;
  ++qword_5C6DF0;
  pParticleEngine->UpdateParticles();
  pMouseInstance->GetCursorPos(&cursor);

  //x = cursor.y;
  //y = cursor.x;
  if ( sub_4637E0_is_there_popup_onscreen() )
  {
    v11 = &vis_face_filter;
    v10 = &vis_sprite_filter_2;
    depth = pGame->pIndoorCameraD3D->GetPickDepth();
  }
  else
  {
    if ( uFlags2 & GAME_FLAGS_2_TARGETING_MODE )
    {
      v11 = &vis_face_filter;
      v10 = &vis_sprite_filter_1;
    }
    else
    {
      v11 = &vis_face_filter;
      v10 = &vis_sprite_filter_4;
    }
    depth = 5120.0;
  }
  //depth = v2;

  PickMouse(depth, cursor.x, cursor.y, false, v10, v11);
  pLightmapBuilder->std__vector_000004_size = 0;
  pLightmapBuilder->std__vector_183808_size = 0;
  pDecalBuilder->std__vector_pDecals_size = 0;
  pDecalBuilder->field_308008 = 0;
  if (!_44F07B())
    return false;

    if ( uFlags & GAME_FLAGS_1_DRAW_BLV_DEBUGS)
      pStru10Instance->bDoNotDrawPortalFrustum = false;
    if ( /*pRenderer->pRenderD3D &&*/ uCurrentlyLoadedLevelType == LEVEL_Outdoor)
      pRenderer->uFogColor = GetLevelFogColor() & 0xFFFFFF;
    if (uFlags & 0x0400)
      uFlags2 |= 0x01;
    /*if ( !pRenderer->pRenderD3D && uCurrentlyLoadedLevelType == LEVEL_Outdoor && pMobileLightsStack->uNumLightsActive )
    {
      uFlags2 |= 0x01;
      field_E10 = qword_5C6DF0;
    }*/
    v6 = qword_5C6DF0 - field_E10;
    if ( qword_5C6DF0 - field_E10 == 1 )
      uFlags2 |= v6;
    if (uNumStationaryLights_in_pStationaryLightsStack != pStationaryLightsStack->uNumLightsActive )
    {
      uFlags2 |= 1;
      uNumStationaryLights_in_pStationaryLightsStack = pStationaryLightsStack->uNumLightsActive;
    }
    _44E904();
    return true;
}


//----- (0044EDE4) --------------------------------------------------------
bool Game::AlterGamma_BLV(BLVFace *pFace, signed int *pColor)
{
  if (uFlags2 & GAME_FLAGS_2_SATURATE_LIGHTMAPS &&
      pFace->uAttributes & FACE_CAN_SATURATE_COLOR)
  {
    *pColor = ReplaceHSV(*pColor, 1.0, fSaturation, -1.0);
    return true;
  }
  else
    return false;
}

//----- (0044EE30) --------------------------------------------------------
bool Game::AlterGamma_ODM(ODMFace *pFace, signed int *pColor)
{
  if (uFlags2 & GAME_FLAGS_2_SATURATE_LIGHTMAPS &&
      pFace->uAttributes & FACE_CAN_SATURATE_COLOR)
  {
    *pColor = ReplaceHSV(*pColor, 1.0, fSaturation, -1.0);
    return true;
  }
  else
    return false;
}


//----- (004645FA) --------------------------------------------------------
void Game::Deinitialize()
{
  WriteWindowsRegistryInt("startinwindow", 1);//pRenderer->bWindowMode);
  //if (pRenderer->bWindowMode)
  {
    WriteWindowsRegistryInt("window X", window->GetX());
    WriteWindowsRegistryInt("window Y", window->GetY());
  }
  WriteWindowsRegistryInt("valAlwaysRun", bAlwaysRun);
  pItemsTable->Release();
  pNPCStats->Release();

  if (pMouse)
    pMouse->Deactivate();

  delete pRenderer;
  pAudioPlayer->Release();//error
  pNew_LOD->FreeSubIndexAndIO();
  pGames_LOD->FreeSubIndexAndIO();
  ClipCursor(0);
  Game::Destroy();
  delete pEventTimer;
}

//----- (0044EE7C) --------------------------------------------------------
bool Game::draw_debug_outlines()
{
  if (uFlags & 0x04)
  {
    pLightmapBuilder->DrawDebugOutlines(-1);
    pDecalBuilder->DrawDecalDebugOutlines();
  }
  return true;
}

//----- (0044EC23) --------------------------------------------------------
int Game::_44EC23(struct Polygon *a2, int *a3, signed int a4)
{
  double v4; // st7@4
  //double v5; // ST00_8@4
  signed int v6; // eax@5
  //double v7; // ST00_8@6
  signed int result; // eax@8
  //double v9; // ST00_8@9
  //double v10; // ST00_8@11
  float a2a; // [sp+14h] [bp+8h]@4
  float a3a; // [sp+18h] [bp+Ch]@4
  float a3b; // [sp+18h] [bp+Ch]@6
  float a4a; // [sp+1Ch] [bp+10h]@9
  float a4b; // [sp+1Ch] [bp+10h]@11

  if ( this->uFlags2 & 2 && a2->field_59 == 5 && a2->pODMFace->uAttributes & 2 )
  {
    v4 = (double)a4;
    a2a = v4;
    *a3 |= 2u;
    a3a = (1.0 - this->fSaturation) * v4;
    //v5 = a3a + 6.7553994e15;
    //if ( SLODWORD(v5) >= 0 )
    if (floorf(a3a + 0.5f) >= 0 )
    {
      a3b = (1.0 - this->fSaturation) * a2a;
      //v7 = a3b + 6.7553994e15;
      //v6 = LODWORD(v7);
      v6 = floorf(a3b + 0.5f);
    }
    else
      v6 = 0;
    if ( a4 >= v6 )
    {
      a4a = (1.0 - fSaturation) * a2a;
      //v9 = a4a + 6.7553994e15;
      //if ( SLODWORD(v9) >= 0 )
      if (floorf(a4a + 0.5f) >= 0)
      {
        a4b = (1.0 - fSaturation) * a2a;
        //v10 = a4b + 6.7553994e15;
        //result = LODWORD(v10);
        result = floorf(a4b + 0.5f);
      }
      else
        result = 0;
    }
    else
      result = a4;
  }
  else
    result = -1;
  return result;
}



//----- (00465C8B) --------------------------------------------------------
Game *Game::Create()
{
  return new Game;
}

//----- (00465CF3) --------------------------------------------------------
void Game::Destroy()
{
  delete pGame;
  pGame = nullptr;
}

//----- (0044ED0A) --------------------------------------------------------
signed int Game::_44ED0A(BLVFace *a2, int *a3, signed int a4)
{
  double v4; // st7@3
  //double v5; // ST00_8@3
  signed int v6; // eax@4
  //double v7; // ST00_8@5
  signed int result; // eax@7
  //double v9; // ST00_8@8
  //double v10; // ST00_8@10
  float v11; // [sp+14h] [bp+8h]@3
  float v12; // [sp+18h] [bp+Ch]@3
  float v13; // [sp+18h] [bp+Ch]@5
  float v14; // [sp+1Ch] [bp+10h]@8
  float v15; // [sp+1Ch] [bp+10h]@10

  if ( this->uFlags2 & 2 && a2->uAttributes & 2 )
  {
    v4 = (double)a4;
    v11 = v4;
    *a3 |= 2u;
    v12 = (1.0 - this->fSaturation) * v4;
    //v5 = v12 + 6.7553994e15;
    if (floorf(v12 + 0.5f)/* SLODWORD(v5)*/ >= 0 )
    {
      v13 = (1.0 - this->fSaturation) * v11;
      //v7 = v13 + 6.7553994e15;
      //v6 = LODWORD(v7);
      v6 = floorf(v13 + 0.5f);
    }
    else
      v6 = 0;
    if ( a4 >= v6 )
    {
      v14 = (1.0 - fSaturation) * v11;
      //v9 = v14 + 6.7553994e15;
      if (floorf(v14 + 0.5f)/* SLODWORD(v9)*/ >= 0 )
      {
        v15 = (1.0 - fSaturation) * v11;
        //v10 = v15 + 6.7553994e15;
        //result = LODWORD(v10);
        result = floorf(v15 + 0.5f);
      }
      else
        result = 0;
    }
    else
      result = a4;
  }
  else
    result = -1;
  return result;
}


//----- (0044E4B7) --------------------------------------------------------
Game::Game()
{
  uNumStationaryLights = 0;
  uNumBloodsplats = 0;
  field_E0C = 0;
  field_E10 = 0;
  uNumStationaryLights_in_pStationaryLightsStack = 0;
  bGammaControlInitialized = false;
  uFlags = 0;
  uFlags2 = 0;
  uSomeGammaStartTime = 0;
  uSomeGammaDeltaTime = 0;

  //pThreadWardInstance = new ThreadWard;
  pThreadWardInstance = nullptr;
  pParticleEngine = new ParticleEngine;
  pMouse = pMouseInstance = new Mouse;
  pLightmapBuilder = new LightmapBuilder;
  pVisInstance = new Vis;
  pStru6Instance = new stru6;
  pIndoorCameraD3D = new IndoorCameraD3D;
  pStru9Instance = new stru9;
  pStru10Instance = new stru10;
  //pStru11Instance = new stru11;
  pStru11Instance = nullptr;
  //pStru12Instance = new stru12(pStru11Instance);
  pStru12Instance = nullptr;
  //pCShow = new CShow;
  pCShow = nullptr;
  pKeyboardInstance = new Keyboard;
  pGammaController = new GammaController;

  uFlags |= 0x0800;
  uFlags2 |= 0x24;

  _44F0FD();
}

//----- (0044E7F3) --------------------------------------------------------
Game::~Game()
{
  delete pGammaController;
  delete pKeyboardInstance;
/*delete pCShow;
  delete pStru12Instance;
  delete pStru11Instance;*/
  delete pStru10Instance;
  delete pStru9Instance;
  delete pIndoorCameraD3D;
  delete pStru6Instance;
  delete pVisInstance;
  delete pLightmapBuilder;
  delete pMouseInstance;
  delete pParticleEngine;
//delete pThreadWardInstance;
}

//----- (0044E904) --------------------------------------------------------
void Game::_44E904()
{
  //Game *v1; // esi@1
  unsigned __int64 v2; // qax@1
  unsigned int v3; // ecx@1
  int v4; // edi@1
  unsigned __int8 v5; // cf@7
  double v6; // st7@13
  double v7; // st7@15
  signed __int64 v8; // [sp+Ch] [bp-8h]@1

  //v1 = this;
  v2 = pEventTimer->Time();
  v4 = (v2 - uSomeGammaStartTime) >> 32;
  v3 = v2 - LODWORD(uSomeGammaStartTime);
  v8 = v2 - uSomeGammaStartTime;
  if ( v4 < 0
    || SHIDWORD(v2) < ((unsigned int)v2 < LODWORD(uSomeGammaStartTime)) + HIDWORD(uSomeGammaStartTime) | v4 == 0
    && v3 <= 0x80 )
  {
    if ( v4 < 0 )
	{
		v3 = 0;
		v4 = 0;
		uSomeGammaStartTime = v2;
		v8 = __PAIR__(v4, v3);
	}
  }
  else
  {
    if ( uSomeGammaDeltaTime )
    {
      LODWORD(uSomeGammaDeltaTime) = 0;
      HIDWORD(uSomeGammaDeltaTime) = 0;
    }
    else
    {
      LODWORD(uSomeGammaDeltaTime) = v3;
      HIDWORD(uSomeGammaDeltaTime) = v4;
    }
    v5 = __CFADD__(v3, -128);
    v3 -= 128;
    v4 = v5 + v4 - 1;
    uSomeGammaStartTime = v2;
    v8 = __PAIR__(v4, v3);
  }
  if ( uSomeGammaDeltaTime )
    v6 = (double)(signed __int64)(uSomeGammaDeltaTime - __PAIR__(v4, v3));
  else
    v6 = (double)v8;
  v7 = v6 * 0.0078125;
  if ( v7 < 0.0 || v7 <= 1.0 )
  {
    if ( v7 < 0.0 )
      v7 = 0.0;
  }
  else
    v7 = 1.0;
  //if ( pRenderer->pRenderD3D )
    fSaturation = v7;
  //else
  //  fSaturation = (1.0 - 0.5) * v7 + 0.5;
}

//----- (0044EA17) --------------------------------------------------------
bool Game::InitializeGammaController()
{
  //if (pVersion->pVersionInfo.dwPlatformId != VER_PLATFORM_WIN32_NT ||
      //pVersion->pVersionInfo.dwMajorVersion != 4 )
    pGammaController->InitializeFromSurface(pRenderer->pFrontBuffer4);

  bGammaControlInitialized = true;
  uSomeGammaStartTime = pEventTimer->Time();
  return true;
}

//----- (0044EA5E) --------------------------------------------------------
bool Game::PickMouse(float fPickDepth, unsigned int uMouseX, unsigned int uMouseY, bool bOutline, Vis_SelectionFilter *sprite_filter, Vis_SelectionFilter *face_filter)
{
  /*if (pCurrentScreen != SCREEN_GAME|| !pRenderer->pRenderD3D)
    return false;*/

  if (!pVisInstance)
  {
    MessageBoxW(nullptr, L"The 'Vis' object pointer has not been instatiated, but CGame::Pick() is trying to call through it.", nullptr, 0);
    return false;
  }

  if (uMouseX >= (signed int)pViewport->uScreen_TL_X &&
      uMouseX <= (signed int)pViewport->uScreen_BR_X &&
      uMouseY >= (signed int)pViewport->uScreen_TL_Y &&
      uMouseY <= (signed int)pViewport->uScreen_BR_Y)
  {
    pVisInstance->PickMouse(fPickDepth, uMouseX, uMouseY, sprite_filter, face_filter);

    if (bOutline)
      OutlineSelection();
  }

  return true;
}
// 4E28F8: using guessed type int pCurrentScreen;

//----- (0044EB12) --------------------------------------------------------
bool Game::PickKeyboard(bool bOutline, Vis_SelectionFilter *sprite_filter, Vis_SelectionFilter *face_filter)
{
  if (pCurrentScreen == SCREEN_GAME && pVisInstance /*&& pRenderer->pRenderD3D*/)
  {
    bool r = pVisInstance->PickKeyboard(&pVisInstance->default_list, sprite_filter, face_filter);

    if (bOutline)
      OutlineSelection();
    return r;
  }
  return false;
}
/*
Result::Code Game::PickKeyboard(bool bOutline, struct unnamed_F93E6C *a3, struct unnamed_F93E6C *a4)
{
 if (dword_4E28F8_PartyCantJumpIfTrue)
   return Result::Generic;

 pVis->PickKeyboard(a3, a4);
 if (bOutline)
   Game_outline_selection((int)this);
 return Result::Success;
}
*/
// 4E28F8: using guessed type int pCurrentScreen;

//----- (0044EB5A) --------------------------------------------------------
void Game::OutlineSelection()
{
  if (!pVisInstance)
    return;

  if (!pVisInstance->default_list.uNumPointers)
    return;

  Vis_ObjectInfo* object_info = pVisInstance->default_list.object_pointers[0];
  if (object_info)
    switch (object_info->object_type)
    {
      case VisObjectType_Sprite:
      {
        Log::Warning(L"Sprite outline currently unsupported");
        return;
      }

      case VisObjectType_Face:
      {
        if (uCurrentlyLoadedLevelType == LEVEL_Outdoor)
        {
          ODMFace* face = (ODMFace *)object_info->object;
          if (face->uAttributes & FACE_OUTLINED)
            face->uAttributes &= ~FACE_OUTLINED;
          else
            face->uAttributes |= FACE_OUTLINED;
        }
        else if (uCurrentlyLoadedLevelType == LEVEL_Indoor)
        {
          BLVFace* face = (BLVFace *)object_info->object;
          if (face->uAttributes & FACE_OUTLINED)
            face->uAttributes &= ~FACE_OUTLINED;
          else
            face->uAttributes |= FACE_OUTLINED;
        }
        else 
          Error("Invalid level type", uCurrentlyLoadedLevelType);
      }
      break;

      default:
      {
        MessageBoxW(nullptr, L"Undefined CObjectInfo type requested in CGame::outline_selection()", nullptr, 0);
        ExitProcess(0);
      }
    }
}
//----- (004304E7) --------------------------------------------------------
void  GameUI_MsgProc()
{
  unsigned int v2; // edx@7
  Actor *pActor; // ecx@13
  int v4; // ecx@18
  unsigned int v10; // ecx@73
  int v14; // eax@98
  int v18; // eax@106
  float v19; // ST64_4@121
  float v21; // ST64_4@126
  float v22; // ST64_4@127
  unsigned int v24; // ecx@149
  GUIWindow *pWindow2; // ecx@248
  bool pKeyBindingFlag; // eax@269
  unsigned int v33; // eax@277
  int v37; // eax@341
  int v38; // eax@358
  SHORT v39; // ax@365
  char *v41; // eax@380
  int v42; // eax@396
  signed int v44; // eax@398
  int v45; // edx@398
  signed int v46; // ecx@398
  char v47; // zf@399
  char v48; // zf@405
  BLVFace *pBLVFace; // ecx@410
  ODMFace *pODMFace; // ecx@412
  CastSpellInfo *pSpellInfo; // ecx@415
  __int16 v53; // ax@431
  int v54; // eax@432
  int v55; // ecx@432
  int v56; // edx@432
  int v57; // eax@432
  Player *pPlayer; // edx@442
  unsigned int pMapNum; // eax@445
  signed int v60; // ST64_4@459
  __int16 v63; // dx@479
  unsigned int v64; // eax@486
  int v65; // ecx@486
  int v66; // eax@488
  char *v67; // eax@489
  __int16 v68; // dx@498
  char *v69; // eax@512
  int v70; // eax@525
  int v71; // edi@527
  NPCData *pNPCData3; // esi@527
  char *v73; // ecx@533
  signed int v74; // edi@535
  int v75; // eax@535
  int v76; // esi@535
  int v77; // eax@537
  Player *pPlayer2; // ecx@549
  signed int v81; // eax@552
  signed int v83; // ecx@554
  signed int v84; // ecx@554
  GUIButton *pButton; // eax@578
  unsigned int v86; // eax@583
  const char *v87; // ecx@595
  const char *v88; // ecx@596
  unsigned int v90; // eax@602
  int v91; // edx@605
  int v92; // eax@605
  int v93; // edx@605
  int pPlayerNum; // edx@611
  int v95; // eax@611
  unsigned int v97; // eax@624
  int v98; // eax@636
  int v103; // eax@671
  Player *pPlayer4; // ecx@718
  int v105; // eax@718
  Player *pPlayer5; // ST78_4@758
  unsigned int v107; // eax@758
  unsigned int v108; // eax@758
  unsigned int v115; // eax@764
  int v116; // eax@776
  unsigned int v118; // eax@785
  unsigned int v119; // ecx@786
  unsigned int v121; // [sp-28h] [bp-624h]@711
  unsigned int v123; // [sp-24h] [bp-620h]@711
  unsigned int v125; // [sp-20h] [bp-61Ch]@711
  int v127; // [sp-1Ch] [bp-618h]@107
  unsigned int v128; // [sp-1Ch] [bp-618h]@711
  GUIButton *pButton2; // [sp-4h] [bp-600h]@59
  const char *v161; // [sp-4h] [bp-600h]@637
  KeyToggleType pKeyToggleType; // [sp+0h] [bp-5FCh]@287
  char *v173; // [sp+0h] [bp-5FCh]@444
  char *v174; // [sp+0h] [bp-5FCh]@449
  const char *v177; // [sp+0h] [bp-5FCh]@629
  char *v178; // [sp+0h] [bp-5FCh]@637
  signed int thisb; // [sp+14h] [bp-5E8h]@272
  Player *pPlayer7; // [sp+14h] [bp-5E8h]@373
  Player *pPlayer8; // [sp+14h] [bp-5E8h]@377
  char *pMapName; // [sp+14h] [bp-5E8h]@445
  Player *pPlayer9; // [sp+14h] [bp-5E8h]@455
  int thisg; // [sp+14h] [bp-5E8h]@467
  int thish; // [sp+14h] [bp-5E8h]@528
  signed int thisi; // [sp+14h] [bp-5E8h]@535
  MapInfo *pMapInfo; // [sp+14h] [bp-5E8h]@604
  Player *pPlayer10; // [sp+14h] [bp-5E8h]@641
  int uMessageParam; // [sp+18h] [bp-5E4h]@7
  int uAction; // [sp+1Ch] [bp-5E0h]@18
  NPCData *pNPCData4; // [sp+20h] [bp-5DCh]@23
  unsigned int uNumSeconds; // [sp+24h] [bp-5D8h]@18
  char v197; // [sp+2Bh] [bp-5D1h]@101
  enum UIMessageType uMessage; // [sp+2Ch] [bp-5D0h]@7
  unsigned int v199; // [sp+30h] [bp-5CCh]@7
  char *v200; // [sp+34h] [bp-5C8h]@518
  POINT v202; // [sp+40h] [bp-5BCh]@141
  POINT a2; // [sp+48h] [bp-5B4h]@127
  POINT v205; // [sp+58h] [bp-5A4h]@171
  POINT v207; // [sp+68h] [bp-594h]@155
  POINT v211; // [sp+88h] [bp-574h]@704
  int v213; // [sp+98h] [bp-564h]@385
  char pLevelName[32]; // [sp+9Ch] [bp-560h]@380
  char pOut[32]; // [sp+BCh] [bp-540h]@370
  FrameTableTxtLine v216; // [sp+DCh] [bp-520h]@524
  int v217[9]; // [sp+158h] [bp-4A4h]@652
  FrameTableTxtLine v218; // [sp+17Ch] [bp-480h]@524
  char a1[64]; // [sp+1F8h] [bp-404h]@467
  char Str2[128]; // [sp+238h] [bp-3C4h]@527
  Actor actor; // [sp+2B8h] [bp-344h]@4
  int currHour;

  dword_50CDC8 = 0;
  if ( !pEventTimer->bPaused )
  {
    pParty->sEyelevel = pParty->uDefaultEyelevel;
    pParty->uPartyHeight = pParty->uDefaultPartyHeight;
  }
  if ( bDialogueUI_InitializeActor_NPC_ID )
  {
    //Actor::Actor(&actor);
    memset(&actor, 0, 0x344u);
    dword_5B65D0_dialogue_actor_npc_id = bDialogueUI_InitializeActor_NPC_ID;
    actor.sNPC_ID = bDialogueUI_InitializeActor_NPC_ID;
    GameUI_InitializeDialogue(&actor, false);
    bDialogueUI_InitializeActor_NPC_ID = 0;
  }
  if ( pMessageQueue_50CBD0->uNumMessages )
  {
    //v1 = "";
    while ( 2 )
    {
      if ( !pMessageQueue_50CBD0->uNumMessages )
        break;

      pMessageQueue_50CBD0->PopMessage(&uMessage, &uMessageParam, (int *)&v199);
      switch ( uMessage )
      {
        case UIMSG_ChangeGameState:
          uGameState = GAME_FINISHED;
          continue;
        case UIMSG_PlayArcomage:
          BackToHouseMenu();
          pArcomageGame->bGameInProgress = 1;
          ArcomageGame::PrepareArcomage();
          continue;
        case UIMSG_StartNPCDialogue:
          if ( !uActiveCharacter )
            continue;
          viewparams->field_48 = 1;
          if ( pMessageQueue_50CBD0->uNumMessages )
            pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
          dword_5B65D0_dialogue_actor_npc_id = pActors[uMessageParam].sNPC_ID;
          pActor = &pActors[uMessageParam];
          //goto _actor_init_dlg;
          GameUI_InitializeDialogue(pActor, true);
          continue;
        case UIMSG_StartHireling1Dialogue:
        case UIMSG_StartHireling2Dialogue:
        {
          if (bNoNPCHiring || pCurrentScreen)
            continue;

            if ( pMessageQueue_50CBD0->uNumMessages )
              pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
            uAction = 0;
            int hireling_idx = uMessage - UIMSG_StartHireling1Dialogue;
            v4 = 0;

            for (uint i = 0; i < 2; ++i)
            //pNPCData0 = pParty->pHirelings;
            //do
            {
              if (pParty->pHirelings[i].pName)
              //{
                //v6 = uAction++;
                pTmpBuf[uAction++] = i;
              //}
              //++pNPCData0;
              //++v4;
            }
            //while ( (signed int)pNPCData0 < (signed int)&pParty->pPickedItem );

            //_this = 0;
            for (uint i = 0; i < pNPCStats->uNumNewNPCs; ++i)
            {
              NPCData* npc = &pNPCStats->pNewNPCData[i];
              //do
              //{
                if (npc->Hired() &&
                    (!pParty->pHirelings[0].pName || strcmp(npc->pName, pParty->pHirelings[0].pName)) &&
                    (!pParty->pHirelings[1].pName || strcmp(npc->pName, pParty->pHirelings[1].pName)) )
                {
                  //v7 =;
                  pTmpBuf[uAction++] = i + 2;
                }
                //++_this;
                //++pNPCData4;
              //}
              //while ( _this < (signed int)pNPCStats->uNumNewNPCs );
            }

            if ( (signed int)(hireling_idx + pParty->hirelingScrollPosition) < uAction )
            {
              //Actor::Actor(&actor);
              memset(&actor, 0, 0x344u);
              actor.sNPC_ID += -1 - pParty->hirelingScrollPosition - hireling_idx;
              pActor = &actor;
              GameUI_InitializeDialogue(&actor, true);
            }
          }
          continue;

        case UIMSG_BuyInShop_Identify_Repair:
          UIShop_Buy_Identify_Repair();
          continue;
        case UIMSG_ClickNPCTopic:
          ClickNPCTopic(uMessageParam);
          continue;
        case UIMSG_SelectShopDialogueOption:
          OnSelectShopDialogueOption(uMessageParam);
          continue;
        case UIMSG_SelectNPCDialogueOption:
          OnSelectNPCDialogueOption((DIALOGUE_TYPE)uMessageParam);
          continue;
        case UIMSG_ClickHouseNPCPortrait:
          _4B4224_UpdateNPCTopics(uMessageParam);
          continue;
        case UIMSG_StartNewGame:
          if ( dword_6BE138 == 124 || uMessageParam )
          {
            pIcons_LOD->SyncLoadedFilesCount();
            pIcons_LOD->RemoveTexturesPackFromTextureList();
            if ( pMessageQueue_50CBD0->uNumMessages )
              pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
            pGUIWindow_CurrentMenu->Release();
            uGameState = GAME_STATE_NEWGAME_OUT_GAMEMENU;
            pCurrentScreen = SCREEN_GAME;
            viewparams->bRedrawGameUI = 1;
          }
          else
          {
            ShowStatusBarString(pGlobalTXT_LocalizationStrings[201], 2);// "Are you sure?  Click again to start a New Game"
            pAudioPlayer->PlaySound(SOUND_20001, 0, 0, -1, 0, 0, 0, 0);
            dword_6BE138 = 124;
          }
          stru_506E40.Release();
          continue;
        case UIMSG_Game_OpenLoadGameDialog:
          pIcons_LOD->SyncLoadedFilesCount();
          pIcons_LOD->RemoveTexturesPackFromTextureList();
          if ( pMessageQueue_50CBD0->uNumMessages )
            pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
          pGUIWindow_CurrentMenu->Release();
          pCurrentScreen = SCREEN_LOADGAME;
          LoadUI_Load(1);
          continue;
        case UIMSG_Quit:
          if ( dword_6BE138 == 132 || uMessageParam )
          {
            pIcons_LOD->SyncLoadedFilesCount();
            pIcons_LOD->RemoveTexturesPackFromTextureList();
            if ( pMessageQueue_50CBD0->uNumMessages )
              pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
            pGUIWindow_CurrentMenu->Release();
            pCurrentScreen = SCREEN_GAME;
            viewparams->bRedrawGameUI = 1;
            if ( !uMessageParam )
              pAudioPlayer->PlaySound((SoundID)(SOUND_EnteringAHouse|0x1), 0, 0, -1, 0, 0, 0, 0);
            uGameState = GAME_STATE_GAME_QUITTING_TO_MAIN_MENU;
          }
          else
          {
            ShowStatusBarString(pGlobalTXT_LocalizationStrings[82], 2);// "Are you sure?  Click again to quit"
            pAudioPlayer->PlaySound(SOUND_20001, 0, 0, -1, 0, 0, 0, 0);
            dword_6BE138 = 132;
          }
          stru_506E40.Release();
          continue;
        case UIMSG_80:
			__debugbreak();
          pIcons_LOD->SyncLoadedFilesCount();
          pIcons_LOD->RemoveTexturesPackFromTextureList();
          pGUIWindow_CurrentMenu->Release();
          pCurrentScreen = SCREEN_OPTIONS;
          pGUIWindow_CurrentMenu = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_8, 0, 0);
          continue;
        case UIMSG_ArrowUp:
          --pSaveListPosition;
          if ( pSaveListPosition < 0 )
            pSaveListPosition = 0;
          GUIWindow::Create(215, 199, 17, 17, WINDOW_PressedButton2, (int)pBtnArrowUp, 0);
          continue;
        case UIMSG_DownArrow:
          ++pSaveListPosition;
          if ( pSaveListPosition >= uMessageParam )
            pSaveListPosition = uMessageParam - 1;
          GUIWindow::Create(215, 323, 17, 17, WINDOW_PressedButton2, (int)pBtnDownArrow, 0);
          continue;
        case UIMSG_Cancel:
          GUIWindow::Create(350, 302, 106, 42, WINDOW_CloseRestWindowBtn, (int)pBtnCancel, 0);
          continue;
        case UIMSG_SaveLoadBtn:
          GUIWindow::Create(241, 302, 106, 42, WINDOW_SaveLoadBtn, (int)pBtnLoadSlot, 0);
          continue;
        case UIMSG_SelectLoadSlot:
          if ( pGUIWindow_CurrentMenu->receives_keyboard_input_2 == WINDOW_INPUT_IN_PROGRESS)
            pKeyActionMap->SetWindowInputStatus(WINDOW_INPUT_NONE);
          if ( pCurrentScreen != SCREEN_SAVEGAME || uLoadGameUI_SelectedSlot != pSaveListPosition + uMessageParam )
          {
            v10 = pSaveListPosition + uMessageParam;
            if ( dword_6BE138 == pSaveListPosition + uMessageParam )
            {
              pMessageQueue_50CBD0->AddGUIMessage(UIMSG_SaveLoadBtn, 0, 0);
              pMessageQueue_50CBD0->AddGUIMessage(UIMSG_LoadGame, 0, 0);
            }
            uLoadGameUI_SelectedSlot = v10;
            dword_6BE138 = v10;
          }
          else
          {
            pKeyActionMap->EnterText(0, 19, pGUIWindow_CurrentMenu);
            if ( strcmp(pSavegameHeader[uLoadGameUI_SelectedSlot].pName, pGlobalTXT_LocalizationStrings[72]) )// "Empty"
              strcpy(pKeyActionMap->pPressedKeysBuffer, pSavegameHeader[uLoadGameUI_SelectedSlot].pName);
            pKeyActionMap->uNumKeysPressed = strlen(pKeyActionMap->pPressedKeysBuffer);
          }
          continue;
        case UIMSG_LoadGame:
          if ( pSavegameUsedSlots[uLoadGameUI_SelectedSlot] )
          {
            LoadGame(uLoadGameUI_SelectedSlot);
            uGameState = GAME_STATE_LOADING_GAME;
          }
          stru_506E40.Release();
          continue;
        case UIMSG_SaveGame:
          if ( pGUIWindow_CurrentMenu->receives_keyboard_input_2 == WINDOW_INPUT_IN_PROGRESS)
          {
            pKeyActionMap->SetWindowInputStatus(WINDOW_INPUT_NONE);
            strcpy((char *)&pSavegameHeader[uLoadGameUI_SelectedSlot], pKeyActionMap->pPressedKeysBuffer);
          }
          DoSavegame(uLoadGameUI_SelectedSlot);
          stru_506E40.Release();
          continue;
        case UIMSG_Game_OpenSaveGameDialog:
          pGUIWindow_CurrentMenu->Release();
          pCurrentScreen = SCREEN_SAVEGAME;
          SaveUI_Load();
          continue;
        case UIMSG_Game_OpenOptionsDialog://Open
          if ( pMessageQueue_50CBD0->uNumMessages )
            pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
          pGUIWindow_CurrentMenu->Release();
          viewparams->field_48 = 1;
          pCurrentScreen = SCREEN_OPTIONS;

          options_menu_skin.uTextureID_Background     = pIcons_LOD->LoadTexture("ControlBG", TEXTURE_16BIT_PALETTE);
          options_menu_skin.uTextureID_TurnSpeed[2]   = pIcons_LOD->LoadTexture("con_16x", TEXTURE_16BIT_PALETTE);
          options_menu_skin.uTextureID_TurnSpeed[1]   = pIcons_LOD->LoadTexture("con_32x", TEXTURE_16BIT_PALETTE);
          options_menu_skin.uTextureID_TurnSpeed[0]   = pIcons_LOD->LoadTexture("con_Smoo", TEXTURE_16BIT_PALETTE);
          options_menu_skin.uTextureID_ArrowLeft      = pIcons_LOD->LoadTexture("con_ArrL", TEXTURE_16BIT_PALETTE);
          options_menu_skin.uTextureID_ArrowRight     = pIcons_LOD->LoadTexture("con_ArrR", TEXTURE_16BIT_PALETTE);
          options_menu_skin.uTextureID_SoundLevels[0] = pIcons_LOD->LoadTexture("convol10", TEXTURE_16BIT_PALETTE);
          options_menu_skin.uTextureID_SoundLevels[1] = pIcons_LOD->LoadTexture("convol20", TEXTURE_16BIT_PALETTE);
          options_menu_skin.uTextureID_SoundLevels[2] = pIcons_LOD->LoadTexture("convol30", TEXTURE_16BIT_PALETTE);
          options_menu_skin.uTextureID_SoundLevels[3] = pIcons_LOD->LoadTexture("convol40", TEXTURE_16BIT_PALETTE);
          options_menu_skin.uTextureID_SoundLevels[4] = pIcons_LOD->LoadTexture("convol50", TEXTURE_16BIT_PALETTE);
          options_menu_skin.uTextureID_SoundLevels[5] = pIcons_LOD->LoadTexture("convol60", TEXTURE_16BIT_PALETTE);
          options_menu_skin.uTextureID_SoundLevels[6] = pIcons_LOD->LoadTexture("convol70", TEXTURE_16BIT_PALETTE);
          options_menu_skin.uTextureID_SoundLevels[7] = pIcons_LOD->LoadTexture("convol80", TEXTURE_16BIT_PALETTE);
          options_menu_skin.uTextureID_SoundLevels[8] = pIcons_LOD->LoadTexture("convol90", TEXTURE_16BIT_PALETTE);
          options_menu_skin.uTextureID_SoundLevels[9] = pIcons_LOD->LoadTexture("convol00", TEXTURE_16BIT_PALETTE);
          options_menu_skin.uTextureID_FlipOnExit     = pIcons_LOD->LoadTexture("option04", TEXTURE_16BIT_PALETTE);
          options_menu_skin.uTextureID_AlwaysRun      = pIcons_LOD->LoadTexture("option03", TEXTURE_16BIT_PALETTE);
          options_menu_skin.uTextureID_ShowDamage     = pIcons_LOD->LoadTexture("option02", TEXTURE_16BIT_PALETTE);
          options_menu_skin.uTextureID_WalkSound      = pIcons_LOD->LoadTexture("option01", TEXTURE_16BIT_PALETTE);

          pGUIWindow_CurrentMenu = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_Options, 0, 0);
          pGUIWindow_CurrentMenu->CreateButton(22, 270,
                                               pIcons_LOD->GetTexture(options_menu_skin.uTextureID_TurnSpeed[2])->uTextureWidth,
                                               pIcons_LOD->GetTexture(options_menu_skin.uTextureID_TurnSpeed[2])->uTextureHeight,
                                               1, 0, UIMSG_SetTurnSpeed, 0x80, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(93, 270,
                                               pIcons_LOD->GetTexture(options_menu_skin.uTextureID_TurnSpeed[1])->uTextureWidth,
                                               pIcons_LOD->GetTexture(options_menu_skin.uTextureID_TurnSpeed[1])->uTextureHeight,
                                               1, 0, UIMSG_SetTurnSpeed, 0x40u, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(164, 270,
                                               pIcons_LOD->GetTexture(options_menu_skin.uTextureID_TurnSpeed[0])->uTextureWidth,
                                               pIcons_LOD->GetTexture(options_menu_skin.uTextureID_TurnSpeed[0])->uTextureHeight,
                                               1, 0, UIMSG_SetTurnSpeed, 0, 0, "", 0);

          pGUIWindow_CurrentMenu->CreateButton(20, 303,
                                               pIcons_LOD->GetTexture(options_menu_skin.uTextureID_WalkSound)->uTextureWidth,
                                               pIcons_LOD->GetTexture(options_menu_skin.uTextureID_WalkSound)->uTextureHeight,
                                               1, 0, UIMSG_ToggleWalkSound, 0, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(128, 303,
                                               pIcons_LOD->GetTexture(options_menu_skin.uTextureID_ShowDamage)->uTextureWidth,
                                               pIcons_LOD->GetTexture(options_menu_skin.uTextureID_ShowDamage)->uTextureHeight,
                                               1, 0, UIMSG_ToggleShowDamage, 0, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(20, 325,
                                               pIcons_LOD->GetTexture(options_menu_skin.uTextureID_AlwaysRun)->uTextureWidth,
                                               pIcons_LOD->GetTexture(options_menu_skin.uTextureID_AlwaysRun)->uTextureHeight,
                                               1, 0, UIMSG_ToggleAlwaysRun, 0, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(128, 325,
                                               pIcons_LOD->GetTexture(options_menu_skin.uTextureID_FlipOnExit)->uTextureWidth,
                                               pIcons_LOD->GetTexture(options_menu_skin.uTextureID_FlipOnExit)->uTextureHeight,
                                               1, 0, UIMSG_ToggleFlipOnExit, 0, 0, "", 0);

          pBtn_SliderLeft  = pGUIWindow_CurrentMenu->CreateButton(243, 162, 16, 16, 1, 0, UIMSG_ChangeSoundVolume, 4, 0, "", pIcons_LOD->GetTexture(options_menu_skin.uTextureID_ArrowLeft), 0);
          pBtn_SliderRight = pGUIWindow_CurrentMenu->CreateButton(435, 162, 16, 16, 1, 0, UIMSG_ChangeSoundVolume, 5, 0, "", pIcons_LOD->GetTexture(options_menu_skin.uTextureID_ArrowRight), 0);
          pGUIWindow_CurrentMenu->CreateButton(263, 162, 172, 17, 1, 0, UIMSG_ChangeSoundVolume, 0, 0, "", 0);

          pBtn_SliderLeft  = pGUIWindow_CurrentMenu->CreateButton(243, 216, 16, 16, 1, 0, UIMSG_ChangeMusicVolume, 4, 0, "", pIcons_LOD->GetTexture(options_menu_skin.uTextureID_ArrowLeft), 0);
          pBtn_SliderRight = pGUIWindow_CurrentMenu->CreateButton(435, 216, 16, 16, 1, 0, UIMSG_ChangeMusicVolume, 5, 0, "", pIcons_LOD->GetTexture(options_menu_skin.uTextureID_ArrowRight), 0);
          pGUIWindow_CurrentMenu->CreateButton(263, 216, 172, 17, 1, 0, UIMSG_ChangeMusicVolume, 0, 0, "", 0);

          pBtn_SliderLeft  = pGUIWindow_CurrentMenu->CreateButton(243, 270, 16, 16, 1, 0, UIMSG_ChangeVoiceVolume, 4, 0, "", pIcons_LOD->GetTexture(options_menu_skin.uTextureID_ArrowLeft), 0);
          pBtn_SliderRight = pGUIWindow_CurrentMenu->CreateButton(435, 270, 16, 16, 1, 0, UIMSG_ChangeVoiceVolume, 5, 0, "", pIcons_LOD->GetTexture(options_menu_skin.uTextureID_ArrowRight), 0);
          pGUIWindow_CurrentMenu->CreateButton(263, 270, 172, 17, 1, 0, UIMSG_ChangeVoiceVolume, 0, 0, "", 0);

          pGUIWindow_CurrentMenu->CreateButton(241, 302, 214, 40, 1, 0, UIMSG_Escape, 0, 0, pGlobalTXT_LocalizationStrings[619], 0); // "Return to Game"
          pGUIWindow_CurrentMenu->CreateButton( 19, 140, 214, 40, 1, 0, UIMSG_OpenKeyMappingOptions, 0, 0x4Bu, "", 0);
          pGUIWindow_CurrentMenu->CreateButton( 19, 194, 214, 40, 1, 0, UIMSG_OpenVideoOptions, 0, 86, "", 0);
          continue;

        case UIMSG_OpenKeyMappingOptions://Open
          if ( pMessageQueue_50CBD0->uNumMessages )
            pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
          pGUIWindow_CurrentMenu->Release();
          viewparams->field_48 = 1;
          pCurrentScreen = SCREEN_KEYBOARD_OPTIONS;
          uTextureID_Optkb[0] = pIcons_LOD->LoadTexture("optkb", TEXTURE_16BIT_PALETTE);
          uTextureID_Optkb[1] = pIcons_LOD->LoadTexture("optkb_h", TEXTURE_16BIT_PALETTE);
          uTextureID_Optkb[2] = pIcons_LOD->LoadTexture("resume1", TEXTURE_16BIT_PALETTE);
          uTextureID_Optkb[3] = pIcons_LOD->LoadTexture("optkb_1", TEXTURE_16BIT_PALETTE);
          uTextureID_Optkb[4] = pIcons_LOD->LoadTexture("optkb_2", TEXTURE_16BIT_PALETTE);
          pGUIWindow_CurrentMenu = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_KeyMappingOptions, 0, 0);

          pGUIWindow_CurrentMenu->CreateButton(241, 302, 214, 40, 1, 0, UIMSG_Escape, 0, 0, "", 0);

          pGUIWindow_CurrentMenu->CreateButton(19, 302, 108, 20, 1, 0, UIMSG_SelectKeyPage1, 0, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(127, 302, 108, 20, 1, 0, UIMSG_SelectKeyPage2, 0, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(127, 324, 108, 20, 1, 0, UIMSG_ResetKeyMapping, 0, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(19, 324, 108, 20, 1, 0, UIMSG_Game_OpenOptionsDialog, 0, 0, "", 0);

          pGUIWindow_CurrentMenu->CreateButton(129, 148, 70, 19, 1, 0, UIMSG_ChangeKeyButton, 0, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(129, 167, 70, 19, 1, 0, UIMSG_ChangeKeyButton, 1, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(129, 186, 70, 19, 1, 0, UIMSG_ChangeKeyButton, 2, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(129, 205, 70, 19, 1, 0, UIMSG_ChangeKeyButton, 3, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(129, 224, 70, 19, 1, 0, UIMSG_ChangeKeyButton, 4, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(129, 243, 70, 19, 1, 0, UIMSG_ChangeKeyButton, 5, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(129, 262, 70, 19, 1, 0, UIMSG_ChangeKeyButton, 6, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(350, 148, 70, 19, 1, 0, UIMSG_ChangeKeyButton, 7, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(350, 167, 70, 19, 1, 0, UIMSG_ChangeKeyButton, 8, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(350, 186, 70, 19, 1, 0, UIMSG_ChangeKeyButton, 9, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(350, 205, 70, 19, 1, 0, UIMSG_ChangeKeyButton, 10, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(350, 224, 70, 19, 1, 0, UIMSG_ChangeKeyButton, 11, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(350, 243, 70, 19, 1, 0, UIMSG_ChangeKeyButton, 12, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(350, 262, 70, 19, 1, 0, UIMSG_ChangeKeyButton, 13, 0, "", 0);

          uGameMenuUI_CurentlySelectedKeyIdx = -1;
          KeyboardPageNum = 1;
          memset(GameMenuUI_InvaligKeyBindingsFlags.data(), 0, sizeof(GameMenuUI_InvaligKeyBindingsFlags));
          //*(_WORD *)KeyButtonArray[28] = 0;
          memcpy(pPrevVirtualCidesMapping.data(), pKeyActionMap->pVirtualKeyCodesMapping, 0x78u);
          continue;
        case UIMSG_ChangeKeyButton:
          if ( uGameMenuUI_CurentlySelectedKeyIdx != -1 )
          {
            pAudioPlayer->PlaySound((SoundID)27, 0, 0, -1, 0, 0, 0, 0);
            continue;
          }
          v14 = uMessageParam;
          if ( KeyboardPageNum != 1 )
            v14 = uMessageParam + 14;
          uGameMenuUI_CurentlySelectedKeyIdx = v14;
          pKeyActionMap->EnterText(0, 1, pGUIWindow_CurrentMenu);
          continue;
        case UIMSG_ResetKeyMapping:
          v197 = 1;
          pKeyActionMap->SetDefaultMapping();
          for ( uint i = 0; i < 28; i++ )
          {
            if ( pKeyActionMap->GetActionVKey((enum InputAction)i) != pPrevVirtualCidesMapping[i] )
            {
              if ( v197 )
              {
                GUI_ReplaceHotkey(LOBYTE(pPrevVirtualCidesMapping[i]), pKeyActionMap->GetActionVKey((enum InputAction)i), 1);
                v197 = 0;
              }
              else
                GUI_ReplaceHotkey(LOBYTE(pPrevVirtualCidesMapping[i]), pKeyActionMap->GetActionVKey((enum InputAction)i), 0);
            }
            pPrevVirtualCidesMapping[i] = pKeyActionMap->GetActionVKey((enum InputAction)i);
            GameMenuUI_InvaligKeyBindingsFlags[i] = false;
          }
          pAudioPlayer->PlaySound((SoundID)219, 0, 0, -1, 0, 0, 0, 0);
          continue;
        case UIMSG_SelectKeyPage1:
          KeyboardPageNum = 1;
          continue;
        case UIMSG_SelectKeyPage2:
          KeyboardPageNum = 2;
          continue;
        case UIMSG_OpenVideoOptions:
          if ( pMessageQueue_50CBD0->uNumMessages )
            pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
          pGUIWindow_CurrentMenu->Release();
          viewparams->field_48 = 1;
          pCurrentScreen = SCREEN_VIDEO_OPTIONS;
          optvid_base_texture_id = pIcons_LOD->LoadTexture("optvid", TEXTURE_16BIT_PALETTE);
          bloodsplats_texture_id = pIcons_LOD->LoadTexture("opvdH-bs", TEXTURE_16BIT_PALETTE);
          us_colored_lights_texture_id = pIcons_LOD->LoadTexture("opvdH-cl", TEXTURE_16BIT_PALETTE);
          tinting_texture_id = pIcons_LOD->LoadTexture("opvdH-tn", TEXTURE_16BIT_PALETTE);
          uTextureID_507C20 = pIcons_LOD->LoadTexture("con_ArrL", TEXTURE_16BIT_PALETTE);
          uTextureID_507C24 = pIcons_LOD->LoadTexture("con_ArrR", TEXTURE_16BIT_PALETTE);
          pTextureIDs_GammaPositions[0] = pIcons_LOD->LoadTexture("convol10", TEXTURE_16BIT_PALETTE);
          pTextureIDs_GammaPositions[1] = pIcons_LOD->LoadTexture("convol20", TEXTURE_16BIT_PALETTE);
          pTextureIDs_GammaPositions[2] = pIcons_LOD->LoadTexture("convol30", TEXTURE_16BIT_PALETTE);
          pTextureIDs_GammaPositions[3] = pIcons_LOD->LoadTexture("convol40", TEXTURE_16BIT_PALETTE);
          pTextureIDs_GammaPositions[4] = pIcons_LOD->LoadTexture("convol50", TEXTURE_16BIT_PALETTE);
          pTextureIDs_GammaPositions[5] = pIcons_LOD->LoadTexture("convol60", TEXTURE_16BIT_PALETTE);
          pTextureIDs_GammaPositions[6] = pIcons_LOD->LoadTexture("convol70", TEXTURE_16BIT_PALETTE);
          pTextureIDs_GammaPositions[7] = pIcons_LOD->LoadTexture("convol80", TEXTURE_16BIT_PALETTE);
          pTextureIDs_GammaPositions[8] = pIcons_LOD->LoadTexture("convol90", TEXTURE_16BIT_PALETTE);
          pTextureIDs_GammaPositions[9] = pIcons_LOD->LoadTexture("convol00", TEXTURE_16BIT_PALETTE);
          not_available_bloodsplats_texture_id = pIcons_LOD->LoadTexture("opvdG-bs", TEXTURE_16BIT_PALETTE);
          not_available_us_colored_lights_texture_id = pIcons_LOD->LoadTexture("opvdG-cl", TEXTURE_16BIT_PALETTE);
          not_available_tinting_texture_id = pIcons_LOD->LoadTexture("opvdG-tn", TEXTURE_16BIT_PALETTE);
          pGUIWindow_CurrentMenu = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_VideoOptions, 0, 0);
          pGUIWindow_CurrentMenu->CreateButton(0xF1u, 0x12Eu, 0xD6u, 0x28u, 1, 0, UIMSG_Escape, 0, 0, "", 0);
          //if ( pRenderer->pRenderD3D )
          {
            pGUIWindow_CurrentMenu->CreateButton(0x13u, 0x118u, 0xD6u, 0x12u, 1, 0, UIMSG_ToggleBloodsplats, 0, 0, "", 0);
            pGUIWindow_CurrentMenu->CreateButton(0x13u, 0x12Eu, 0xD6u, 0x12u, 1, 0, UIMSG_ToggleColoredLights, 0, 0, "", 0);
            pGUIWindow_CurrentMenu->CreateButton(0x13u, 0x144u, 0xD6u, 0x12u, 1, 0, UIMSG_ToggleTint, 0, 0, "", 0);
          }
          /*if ( !pRenderer->bWindowMode )
          {
            //v0 = 1;
            if ( pRenderer->IsGammaSupported() )
            {
              pBtn_SliderLeft = pGUIWindow_CurrentMenu->CreateButton(0x15u, 0xA1u, 0x10u, 0x10u, 1, 0, UIMSG_1A9, 4u, 0, "", pIcons_LOD->GetTexture(uTextureID_507C20), 0);
              pBtn_SliderRight = pGUIWindow_CurrentMenu->CreateButton(0xD5u, 0xA1u, 0x10u, 0x10u, 1, 0, UIMSG_1A9, 5u, 0, "", pIcons_LOD->GetTexture(uTextureID_507C24), 0);
              pGUIWindow_CurrentMenu->CreateButton(42, 162, 170, 18, 1, 0, UIMSG_1A9, 0, 0, "", 0);
            }
          }*/
          continue;
        case UIMSG_1A9:
			__debugbreak();
          if ( uMessageParam == 4 )
          {
            //--uGammaPos;
            if ( (uGammaPos -- -1) < 0 )
            {
              uGammaPos = 0;
              pAudioPlayer->PlaySound((SoundID)21, 0, 0, -1, 0, 0, 0, 0);
              continue;
            }
            v19 = (double)(signed int)uGammaPos * 0.1 + 0.6;
            pGame->pGammaController->Initialize(v19);
            GUIWindow::Create(21, 161, 0, 0, WINDOW_PressedButton2, (int)pBtn_SliderLeft, (char *)1);
            pAudioPlayer->PlaySound((SoundID)21, 0, 0, -1, 0, 0, 0, 0);
            continue;
          }
          if ( uMessageParam == 5 )
          {
            ++uGammaPos;
            if ( (signed int)uGammaPos <= 9 )
            {
              v21 = (double)(signed int)uGammaPos * 0.1 + 0.6;
              pGame->pGammaController->Initialize(v21);
              GUIWindow::Create(213, 161, 0, 0, WINDOW_PressedButton2, (int)pBtn_SliderRight, (char *)1);
              pAudioPlayer->PlaySound((SoundID)21, 0, 0, -1, 0, 0, 0, 0);
              continue;
            }
            uGammaPos = 9;
          }
          else
          {
            uGammaPos = (pMouse->GetCursorPos(&a2)->x - 42) / 17;
            v22 = (double)(signed int)uGammaPos * 0.1 + 0.6;
            pGame->pGammaController->Initialize(v22);
          }
          pAudioPlayer->PlaySound((SoundID)21, 0, 0, -1, 0, 0, 0, 0);
          continue;
        case UIMSG_ToggleBloodsplats:
          pGame->ToggleFlags2(0x20u);
          continue;
        case UIMSG_ToggleColoredLights:
          pRenderer->ToggleColoredLights();
          continue;
        case UIMSG_ToggleTint:
          pRenderer->ToggleTint();
          continue;
        case UIMSG_ChangeMusicVolume:
          if ( uMessageParam == 4 )//-
          {
            --uMusicVolimeMultiplier;
            if ( (char)uMusicVolimeMultiplier < 1 )
              uMusicVolimeMultiplier = 0;
            GUIWindow::Create(243, 216, 0, 0, WINDOW_PressedButton2, (int)pBtn_SliderLeft, (char *)1);
            if ( uMusicVolimeMultiplier )
              pAudioPlayer->PlaySound(SOUND_Bell, -1, 0, -1, 0, 0, pSoundVolumeLevels[uMusicVolimeMultiplier] * 64.0f, 0);
            pAudioPlayer->SetMusicVolume(pSoundVolumeLevels[uMusicVolimeMultiplier] * 64.0f);
            continue;
          }
          if ( uMessageParam == 5 )//+
          {
            ++uMusicVolimeMultiplier;
            if ( (char)uMusicVolimeMultiplier > 9 )
              uMusicVolimeMultiplier = 9;
            GUIWindow::Create(435, 216, 0, 0, WINDOW_PressedButton2, (int)pBtn_SliderRight, (char *)1);
            if ( uMusicVolimeMultiplier )
              pAudioPlayer->PlaySound(SOUND_Bell, -1, 0, -1, 0, 0, pSoundVolumeLevels[uMusicVolimeMultiplier] * 64.0f, 0);
            pAudioPlayer->SetMusicVolume(pSoundVolumeLevels[uMusicVolimeMultiplier] * 64.0f);
            continue;
          }
          uMusicVolimeMultiplier = (pMouse->GetCursorPos(&v202)->x - 263) / 17;//for mouse
          if ( (char)uMusicVolimeMultiplier > 9 )
            uMusicVolimeMultiplier = 9;
          if ( uMusicVolimeMultiplier )
            pAudioPlayer->PlaySound(SOUND_Bell, -1, 0, -1, 0, 0, pSoundVolumeLevels[uMusicVolimeMultiplier] * 64.0f, 0);
          pAudioPlayer->SetMusicVolume(pSoundVolumeLevels[uMusicVolimeMultiplier] * 64.0f);
          continue;
        case UIMSG_ChangeSoundVolume:
          if ( uMessageParam == 4 )//reduce sound level button left
          {
            --uSoundVolumeMultiplier;
            if ( (char)uSoundVolumeMultiplier < 1 )
              uSoundVolumeMultiplier = 0;
            GUIWindow::Create(243, 162, 0, 0, WINDOW_PressedButton2, (int)pBtn_SliderLeft, (char *)1);
            pAudioPlayer->SetMasterVolume(pSoundVolumeLevels[uSoundVolumeMultiplier] * 128.0f);
            pAudioPlayer->PlaySound((SoundID)218, -1, 0, -1, 0, 0, 0, 0);
            continue;
          }
          if ( uMessageParam == 5 )//Increase sound level button right
          {
            ++uSoundVolumeMultiplier;
            if ( (char)uSoundVolumeMultiplier > 8 )
              uSoundVolumeMultiplier = 9;
            //v168 = 1;
            v24 = 435;
            //v154 = (int)pBtn_SliderRight;
            GUIWindow::Create(v24, 0xA2u, 0, 0, WINDOW_PressedButton2, (int)pBtn_SliderRight, (char *)1);
            pAudioPlayer->SetMasterVolume(pSoundVolumeLevels[uSoundVolumeMultiplier] * 128.0f);
            pAudioPlayer->PlaySound((SoundID)218, -1, 0, -1, 0, 0, 0, 0);
            continue;
          }
          uSoundVolumeMultiplier = (pMouse->GetCursorPos(&v207)->x - 263) / 17;
          if ( (char)uSoundVolumeMultiplier > 8 )
            uSoundVolumeMultiplier = 9;
          pAudioPlayer->SetMasterVolume(pSoundVolumeLevels[uSoundVolumeMultiplier] * 128.0f);
          pAudioPlayer->PlaySound((SoundID)218, -1, 0, -1, 0, 0, 0, 0);
          continue;
        case UIMSG_ToggleFlipOnExit:
          bFlipOnExit = bFlipOnExit == 0;
          continue;
        case UIMSG_ToggleAlwaysRun:
          bAlwaysRun = bAlwaysRun == 0;
          continue;
        case UIMSG_ToggleWalkSound:
          bWalkSound = bWalkSound == 0;
          continue;
        case UIMSG_ToggleShowDamage:
          bShowDamage = bShowDamage == 0;
          continue;
        case UIMSG_ChangeVoiceVolume:
          if ( uMessageParam == 4 )
          {
            --uVoicesVolumeMultiplier;
            if ( (char)uVoicesVolumeMultiplier < 1 )
              uVoicesVolumeMultiplier = 0;
            GUIWindow::Create(243, 270, 0, 0, WINDOW_PressedButton2, (int)pBtn_SliderLeft, (char *)1);
            if ( !uVoicesVolumeMultiplier )
              continue;
            pAudioPlayer->PlaySound((SoundID)5788, -1, 0, -1, 0, 0, pSoundVolumeLevels[uVoicesVolumeMultiplier] * 128.0f, 0);
            continue;
          }
          if ( uMessageParam == 5 )
          {
            ++uVoicesVolumeMultiplier;
            if ( (char)uVoicesVolumeMultiplier > 8 )
              uVoicesVolumeMultiplier = 9;
            GUIWindow::Create(435, 270, 0, 0, WINDOW_PressedButton2, (int)pBtn_SliderRight, (char *)1);
            if ( !uVoicesVolumeMultiplier )
              continue;
            pAudioPlayer->PlaySound((SoundID)5788, -1, 0, -1, 0, 0, pSoundVolumeLevels[uVoicesVolumeMultiplier] * 128.0f, 0);
            continue;
          }
          uVoicesVolumeMultiplier = (pMouse->GetCursorPos(&v205)->x - 263) / 17;
          if ( (char)uVoicesVolumeMultiplier > 8 )
            uVoicesVolumeMultiplier = 9;
          if ( !uVoicesVolumeMultiplier )
            continue;
          pAudioPlayer->PlaySound((SoundID)5788, -1, 0, -1, 0, 0, pSoundVolumeLevels[uVoicesVolumeMultiplier] * 128.0f, 0);
          continue;
        case UIMSG_SetTurnSpeed:
          if ( uMessageParam )
            pParty->sRotationY = uMessageParam * pParty->sRotationY / uMessageParam;
          uTurnSpeed = uMessageParam;
          continue;

        case UIMSG_SetGraphicsMode:
          /*if ( !bUseLoResSprites )
          {
            byte_6BE388_graphicsmode = uMessageParam;
            MM7Initialization();
            continue;
          }
          if ( uMessageParam )
          {
            if ( uMessageParam == 1 )
            {
              byte_6BE388_graphicsmode = 0;
            }
            else
            {
              if ( uMessageParam != 2 )
                continue;
              byte_6BE388_graphicsmode = 1;
            }
            MM7Initialization();
            continue;
          }*/
          ModalWindow(pNPCTopics[453].pText, UIMSG_0);
          __debugbreak(); // Nomad: graphicsmode as it was now removed
          continue;

        case UIMSG_GameMenu_ReturnToGame:
          pGUIWindow_CurrentMenu->Release();
          pEventTimer->Resume();
          pCurrentScreen = SCREEN_GAME;
          viewparams->bRedrawGameUI = 1;
          stru_506E40.Release();
          continue;
        case UIMSG_OpenQuestBook:
          if ( pMessageQueue_50CBD0->uNumMessages )
            pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
          if ( pCurrentScreen )
            pGUIWindow_CurrentMenu->Release();
          pEventTimer->Pause();
          pAudioPlayer->StopChannels(-1, -1);
          pGUIWindow_CurrentMenu = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_Book, uMessage, 0);
          pBooksWindow = GUIWindow::Create(493u, 355u, 0, 0, WINDOW_BooksWindow, (int)pBtn_Quests, 0);
          bFlashQuestBook = 0;
          continue;
        case UIMSG_OpenAutonotes:
          if ( pMessageQueue_50CBD0->uNumMessages )
            pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
          if ( pCurrentScreen )
            pGUIWindow_CurrentMenu->Release();
          pEventTimer->Pause();
          pAudioPlayer->StopChannels(-1, -1);
          pGUIWindow_CurrentMenu = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_Book, uMessage, 0);
          pBooksWindow = GUIWindow::Create(527u, 353u, 0, 0, WINDOW_BooksWindow, (int)pBtn_Autonotes, 0);
          bFlashAutonotesBook = 0;
          continue;
        case UIMSG_OpenMapBook:
          if ( pMessageQueue_50CBD0->uNumMessages )
            pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
          if ( pCurrentScreen )
            pGUIWindow_CurrentMenu->Release();
          pEventTimer->Pause();
          viewparams->sViewCenterX = pParty->vPosition.x;
          viewparams->sViewCenterY = pParty->vPosition.y;
          pAudioPlayer->StopChannels(-1, -1);
          pGUIWindow_CurrentMenu = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_Book, uMessage, 0);
          pBooksWindow = GUIWindow::Create(546, 353, 0, 0, WINDOW_BooksWindow, (int)pBtn_Maps, 0);
          continue;
        case UIMSG_OpenCalendar:
          if ( pMessageQueue_50CBD0->uNumMessages )
            pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
          if ( pCurrentScreen )
            pGUIWindow_CurrentMenu->Release();
          pEventTimer->Pause();
          pAudioPlayer->StopChannels(-1, -1);
          pGUIWindow_CurrentMenu = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_Book, uMessage, 0);
          pBooksWindow = GUIWindow::Create(570, 354, 0, 0, WINDOW_BooksWindow, (int)pBtn_Calendar, 0);
          continue;
        case UIMSG_OpenHistoryBook:
          if ( pMessageQueue_50CBD0->uNumMessages )
            pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
          if ( pCurrentScreen )
            pGUIWindow_CurrentMenu->Release();
          pEventTimer->Pause();
          pAudioPlayer->StopChannels(-1, -1);
          pGUIWindow_CurrentMenu = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_Book, uMessage, 0);
          pBooksWindow = GUIWindow::Create(0x258u, 0x169u, 0, 0, WINDOW_BooksWindow, (int)pBtn_History, 0);
          bFlashHistoryBook = 0;
          continue;
        case UIMSG_Escape:// нажатие Escape and return to game
          back_to_game();
          if ( pMessageQueue_50CBD0->uNumMessages )
            pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
          switch ( pCurrentScreen )
          {
            case SCREEN_E:
				__debugbreak();
            case SCREEN_NPC_DIALOGUE:
            case SCREEN_CHEST:
            case SCREEN_CHEST_INVENTORY:
            case SCREEN_CHANGE_LOCATION:
            case SCREEN_INPUT_BLV:
            case SCREEN_QUICK_REFERENCE:
              if ( dword_50CDC8 )
                break;
              CloseWindowBackground();
              uMessageParam = 1;
              break;
            case SCREEN_HOUSE:
              if ( !dword_50CDC8 )
              {
                CloseWindowBackground();
                uMessageParam = 1;
                break;
              }
              break;
          }
          if ( !pModalWindow )
          {
            pRenderer->ClearZBuffer(0, 479);
            viewparams->bRedrawGameUI = 1;
            viewparams->field_48 = 1;
            if ( pCurrentScreen )
            {
              if ( pCurrentScreen > SCREEN_67 )
              {
                if ( pCurrentScreen == SCREEN_QUICK_REFERENCE )
                {
                  pIcons_LOD->RemoveTexturesPackFromTextureList();
                  if ( pGUIWindow_Settings )
                  {
                    if ( pCurrentScreen == SCREEN_CHARACTERS )
                      pMouse->SetCursorBitmap("MICON2");
                    else
                    {
                      pGUIWindow_Settings->Release();
                      pGUIWindow_Settings = 0;
                      pMouse->SetCursorBitmap("MICON1");
                      GameUI_Footer_TimeLeft = 0;
                      _50C9A0_IsEnchantingInProgress = 0;
                      back_to_game();
                    }
                  }
                  if ( (signed int)uActiveCharacter < 1 || (signed int)uActiveCharacter > 4 )
                    uActiveCharacter = pParty->GetNextActiveCharacter();
                  pGUIWindow_CurrentMenu->Release();
                  if ( pGUIWindow_CurrentMenu == window_SpeakInHouse )
                    window_SpeakInHouse = 0;
                  pGUIWindow_CurrentMenu = 0;
                  pEventTimer->Resume();
                  pCurrentScreen = SCREEN_GAME;
                  viewparams->bRedrawGameUI = 1;
                  pIcons_LOD->RemoveTexturesFromTextureList();
                  continue;
                }
              }
              else
              {
                if ( pCurrentScreen < SCREEN_64 )
                {
                  switch ( pCurrentScreen )
                  {
                    case SCREEN_CASTING:
                      pIcons_LOD->RemoveTexturesPackFromTextureList();
                      if ( some_active_character )
                      {
                        uActiveCharacter = some_active_character;
                        uActiveCharacter = pParty->GetNextActiveCharacter();
                        some_active_character = 0;
                        if ( pParty->bTurnBasedModeOn )
                          pTurnEngine->ApplyPlayerAction();
                        _50C9D0_AfterEnchClickEventId = 0;
                        _50C9D4_AfterEnchClickEventSecondParam = 0;
                        _50C9D8_AfterEnchClickEventTimeout = 0;
                      }
                      if ( ptr_50C9A4_ItemToEnchant && ptr_50C9A4_ItemToEnchant->uItemID )
                      {
                        LOBYTE(ptr_50C9A4_ItemToEnchant->uAttributes) &= 0xFu;
                        _50C9A8_item_enchantment_timer = 0;
                        ptr_50C9A4_ItemToEnchant = nullptr;
                      }
                      if ( pGUIWindow_Settings )
                      {
                        if ( pCurrentScreen == SCREEN_CHARACTERS )
                          pMouse->SetCursorBitmap("MICON2");
                        else
                        {
                          pGUIWindow_Settings->Release();
                          pGUIWindow_Settings = 0;
                          pMouse->SetCursorBitmap("MICON1");
                          GameUI_Footer_TimeLeft = 0;
                          _50C9A0_IsEnchantingInProgress = 0;
                          back_to_game();
                        }
                       }
                       if ( (signed int)uActiveCharacter < 1 || (signed int)uActiveCharacter > 4 )
                       uActiveCharacter = pParty->GetNextActiveCharacter();
                       pGUIWindow_CurrentMenu->Release();
                       if ( pGUIWindow_CurrentMenu == window_SpeakInHouse )
                        window_SpeakInHouse = 0;
                       pGUIWindow_CurrentMenu = 0;
                       pEventTimer->Resume();
                       pCurrentScreen = SCREEN_GAME;
                       viewparams->bRedrawGameUI = 1;
                       pIcons_LOD->RemoveTexturesFromTextureList();
                       continue;
                    case SCREEN_BOOKS:
                      pBooksWindow->Release();
                      //crt_deconstruct_ptr_6A0118();
                      pBooksWindow = 0;
                      pEventTimer->Resume();
                      if ( pGUIWindow_Settings )
                      {
                        if ( pCurrentScreen == SCREEN_CHARACTERS )
                          pMouse->SetCursorBitmap("MICON2");
                        else
                        {
                          pGUIWindow_Settings->Release();
                          pGUIWindow_Settings = 0;
                          pMouse->SetCursorBitmap("MICON1");
                          GameUI_Footer_TimeLeft = 0;
                          _50C9A0_IsEnchantingInProgress = 0;
                          back_to_game();
                        }
                      }
                      if ( (signed int)uActiveCharacter < 1 || (signed int)uActiveCharacter > 4 )
                        uActiveCharacter = pParty->GetNextActiveCharacter();
                      pGUIWindow_CurrentMenu->Release();
                      if ( pGUIWindow_CurrentMenu == window_SpeakInHouse )
                        window_SpeakInHouse = 0;
                      pGUIWindow_CurrentMenu = 0;
                      pEventTimer->Resume();
                      pCurrentScreen = SCREEN_GAME;
                      viewparams->bRedrawGameUI = 1;
                      pIcons_LOD->RemoveTexturesFromTextureList();
                      continue;
                    case SCREEN_SAVEGAME:
                    case SCREEN_LOADGAME:
                      pIcons_LOD->RemoveTexturesPackFromTextureList();
                      //crt_deconstruct_ptr_6A0118();
                      stru_506E40.Release();
                      break;
                    case SCREEN_CHEST_INVENTORY:
                      pCurrentScreen = SCREEN_CHEST;
                      continue;
                    case SCREEN_CHEST:
                      pWindow2 = pChestWindow;
                      pWindow2->Release();
                      pIcons_LOD->RemoveTexturesPackFromTextureList();
                      pCurrentScreen = SCREEN_GAME;
                      viewparams->bRedrawGameUI = 1;
                      pEventTimer->Resume();
                      continue;
                    case SCREEN_19:
						__debugbreak();
                      pWindow2 = ptr_507BC8;
                      pWindow2->Release();
                      pIcons_LOD->RemoveTexturesPackFromTextureList();
                      pCurrentScreen = SCREEN_GAME;
                      viewparams->bRedrawGameUI = 1;
                      pEventTimer->Resume();
                      continue;
                    case SCREEN_OPTIONS://Close
                      options_menu_skin.Relaease();
                      pIcons_LOD->SyncLoadedFilesCount();
                      WriteWindowsRegistryInt("soundflag", (char)uSoundVolumeMultiplier);
                      WriteWindowsRegistryInt("musicflag", (char)uMusicVolimeMultiplier);
                      WriteWindowsRegistryInt("CharVoices", (char)uVoicesVolumeMultiplier);
                      WriteWindowsRegistryInt("WalkSound", bWalkSound);
                      WriteWindowsRegistryInt("ShowDamage", bShowDamage);
                      //WriteWindowsRegistryInt("graphicsmode", (unsigned __int8)byte_6BE388_graphicsmode);
                      WriteWindowsRegistryInt("valAlwaysRun", bAlwaysRun);
                      WriteWindowsRegistryInt("FlipOnExit", bFlipOnExit);
                      if ( !uTurnSpeed )
                      {
                        WriteWindowsRegistryInt("TurnDelta", 3);
                        stru_506E40.Release();
                        break;
                      }
                      if ( uTurnSpeed == 64 )
                      {
                        WriteWindowsRegistryInt("TurnDelta", 2);
                        stru_506E40.Release();
                        break;
                      }
                      if ( uTurnSpeed != 128 )
                        {
                          stru_506E40.Release();
                          break;
                        }
                      WriteWindowsRegistryInt("TurnDelta", 1);
                      stru_506E40.Release();
                      break;
                    case SCREEN_MENU:
                      pIcons_LOD->SyncLoadedFilesCount();
                      pIcons_LOD->RemoveTexturesPackFromTextureList();
                      pIcons_LOD->SyncLoadedFilesCount();
                      pIcons_LOD->RemoveTexturesPackFromTextureList();
                      stru_506E40.Release();
                      break;
                    case SCREEN_VIDEO_OPTIONS:
                      //if ( pRenderer->pRenderD3D )
                      {
                        WriteWindowsRegistryInt("Colored Lights", pRenderer->bUseColoredLights);
                        WriteWindowsRegistryInt("Tinting", pRenderer->bTinting);
                        WriteWindowsRegistryInt("Bloodsplats", (LOBYTE(pGame->uFlags2) >> 5) & 1);
                      }
                      /*if ( !pRenderer->bWindowMode )
                        WriteWindowsRegistryInt("GammaPos", uGammaPos);*/

                      stru_506E40.Release();
                      break;

                    case SCREEN_KEYBOARD_OPTIONS://Return to game
                      v197 = 1;
					  pKeyBindingFlag = false;
                      for ( uint i = 0; i < 28; ++i )
                      {
                        if ( GameMenuUI_InvaligKeyBindingsFlags[i] )
                          pKeyBindingFlag = true;
                      }
                      if ( !pKeyBindingFlag )
                      {
                        for ( uint i = 0; i < 5; i++ )
                        {
                          if ( uTextureID_Optkb[i] )
                            pIcons_LOD->pTextures[uTextureID_Optkb[i]].Release();
                        }
                        memset(&uTextureID_Optkb, 0, 20);
                        pIcons_LOD->SyncLoadedFilesCount();
                        for ( uint i = 0; i < 28; ++i )
                        {
                          if ( pKeyActionMap->GetActionVKey((enum InputAction)i) != pPrevVirtualCidesMapping[i] )
                          {
                            if ( v197 )
                            {
                              GUI_ReplaceHotkey(pKeyActionMap->GetActionVKey((enum InputAction)i), LOBYTE(pPrevVirtualCidesMapping[i]), 1);
                              v197 = 0;
                            }
                            else
                              GUI_ReplaceHotkey(pKeyActionMap->GetActionVKey((enum InputAction)i), LOBYTE(pPrevVirtualCidesMapping[i]), 0);
                          }
                          if ( i > 3 && i != 25 && i != 26 )
                            pKeyToggleType = TOGGLE_OneTimePress;
                          else
                            pKeyToggleType = TOGGLE_Continuously;
                          pKeyActionMap->SetKeyMapping(i, pPrevVirtualCidesMapping[i], pKeyToggleType);
                        }
                        pKeyActionMap->StoreMappings();
                        stru_506E40.Release();
                        break;
                      }
                      pAudioPlayer->PlaySound((SoundID)27, 0, 0, -1, 0, 0, 0, 0);
                      break;
                    case SCREEN_REST://close rest screen
                      if ( dword_506F14 )
                      {
                        Rest(_506F18_num_minutes_to_sleep);
                        pParty->pPlayers[3].SetAsleep(false);
                        pParty->pPlayers[2].SetAsleep(false);
                        pParty->pPlayers[1].SetAsleep(false);
                        pParty->pPlayers[0].SetAsleep(false);
                      }
                      pTexture_RestUI_CurrentSkyFrame->Release();
                      pTexture_RestUI_CurrentHourglassFrame->Release();
                      pTexture_RestUI_CurrentHourglassFrame = 0;
                      pTexture_RestUI_CurrentSkyFrame = 0;
                      pIcons_LOD->SyncLoadedFilesCount();
                      pIcons_LOD->RemoveTexturesPackFromTextureList();
                      if ( uCurrentlyLoadedLevelType == LEVEL_Outdoor)
                      {
                        pOutdoor->UpdateSunlightVectors();
                        pOutdoor->UpdateFog();
                      }
                      _506F18_num_minutes_to_sleep = 0;
                      dword_506F14 = 0;
                      if ( pGUIWindow_Settings )
                      {
                        if ( pCurrentScreen == SCREEN_CHARACTERS )
                          pMouse->SetCursorBitmap("MICON2");
                        else
                        {
                          pGUIWindow_Settings->Release();
                          pGUIWindow_Settings = 0;
                          pMouse->SetCursorBitmap("MICON1");
                          GameUI_Footer_TimeLeft = 0;
                          _50C9A0_IsEnchantingInProgress = 0;
                          back_to_game();
                        }
                      }
                      if ( (signed int)uActiveCharacter < 1 || (signed int)uActiveCharacter > 4 )
                        uActiveCharacter = pParty->GetNextActiveCharacter();
                      pGUIWindow_CurrentMenu->Release();
                      if ( pGUIWindow_CurrentMenu == window_SpeakInHouse )
                        window_SpeakInHouse = 0;
                      pGUIWindow_CurrentMenu = 0;
                      pEventTimer->Resume();
                      pCurrentScreen = SCREEN_GAME;
                      viewparams->bRedrawGameUI = 1;
                      pIcons_LOD->RemoveTexturesFromTextureList();
                      continue;
                    case SCREEN_E:
						__debugbreak();
                      pGUIWindow_CurrentMenu->Release();
                      pCurrentScreen = SCREEN_HOUSE;
                      pIcons_LOD->RemoveTexturesPackFromTextureList();
                      continue;
                    case SCREEN_HOUSE:
                      if ( uDialogueType )
                        uDialogueType = 0;
                      if ( uGameState == GAME_STATE_CHANGE_LOCATION )
                      {
                        while ( HouseDialogPressCloseBtn() )
                          ;
                      }
                      else
                      {
                        if ( HouseDialogPressCloseBtn() )
                          continue;
                      }
                      GetHouseGoodbyeSpeech();
                      pAudioPlayer->PlaySound(SOUND_7, 814, 0, -1, 0, 0, 0, 0);
                      pMediaPlayer->Unload();
                      pGUIWindow_CurrentMenu = window_SpeakInHouse;
                      if ( pGUIWindow_Settings )
                      {
                        if ( pCurrentScreen == SCREEN_CHARACTERS )
                          pMouse->SetCursorBitmap("MICON2");
                        else
                        {
                          pGUIWindow_Settings->Release();
                          pGUIWindow_Settings = 0;
                          pMouse->SetCursorBitmap("MICON1");
                          GameUI_Footer_TimeLeft = 0;
                          _50C9A0_IsEnchantingInProgress = 0;
                          back_to_game();
                        }
                      }
                      if ( (signed int)uActiveCharacter < 1 || (signed int)uActiveCharacter > 4 )
                        uActiveCharacter = pParty->GetNextActiveCharacter();
                      pGUIWindow_CurrentMenu->Release();
                      if ( pGUIWindow_CurrentMenu == window_SpeakInHouse )
                        window_SpeakInHouse = 0;
                      pGUIWindow_CurrentMenu = 0;
                      pEventTimer->Resume();
                      pCurrentScreen = SCREEN_GAME;
                      viewparams->bRedrawGameUI = true;
                      pIcons_LOD->RemoveTexturesFromTextureList();
                      continue;
                    case SCREEN_INPUT_BLV://click escape
                      if ( uCurrentHouse_Animation == 153 )
                        PlayHouseSound(0x99u, HouseSound_Greeting_2);
                      pMediaPlayer->Unload();
                      if ( npcIdToDismissAfterDialogue )
                      {
                        pParty->hirelingScrollPosition = 0;
                        LOBYTE(pNPCStats->pNewNPCData[npcIdToDismissAfterDialogue].uFlags) &= 0x7Fu;
                        pParty->CountHirelings();
                        viewparams->bRedrawGameUI = true;
                        npcIdToDismissAfterDialogue = 0;
                      }
                      DialogueEnding();
                      pCurrentScreen = SCREEN_GAME;
                      viewparams->bRedrawGameUI = true;
                      continue;
                    case SCREEN_NPC_DIALOGUE://click escape
                      if ( npcIdToDismissAfterDialogue )
                      {
                        pParty->hirelingScrollPosition = 0;
                        LOBYTE(pNPCStats->pNewNPCData[npcIdToDismissAfterDialogue].uFlags) &= 0x7Fu;
                        pParty->CountHirelings();
                        viewparams->bRedrawGameUI = true;
                        npcIdToDismissAfterDialogue = 0;
                      }
                      //goto LABEL_317;
                      DialogueEnding();
                      pCurrentScreen = SCREEN_GAME;
                      viewparams->bRedrawGameUI = true;
                      continue;
                    case SCREEN_BRANCHLESS_NPC_DIALOG://click escape
                      memset(GameUI_Footer_TimedString.data(), 0, 0xC8u);
                      sub_4452BB();
                      DialogueEnding();
                      pCurrentScreen = SCREEN_GAME;
                      viewparams->bRedrawGameUI = true;
                      continue;
                    case SCREEN_CHANGE_LOCATION://click escape
                      if ( pParty->vPosition.x < -22528 )
                        pParty->vPosition.x = -22528;
                      if ( pParty->vPosition.x > 22528 )
                        pParty->vPosition.x = 22528;
                      if ( pParty->vPosition.y < -22528 )
                        pParty->vPosition.y = -22528;
                      if ( pParty->vPosition.y > 22528 )
                        pParty->vPosition.y = 22528;
                      DialogueEnding();
                      pCurrentScreen = SCREEN_GAME;
                      viewparams->bRedrawGameUI = true;
                      continue;
                    case SCREEN_VIDEO:
                      pMediaPlayer->Unload();
                      continue;
                    case SCREEN_CHARACTERS:
                      CharacterUI_ReleaseButtons();
                      ReleaseAwardsScrollBar();
                      pIcons_LOD->RemoveTexturesPackFromTextureList();
                      if ( pGUIWindow_Settings )
                      {
                        if ( pCurrentScreen == SCREEN_CHARACTERS )
                          pMouse->SetCursorBitmap("MICON2");
                        else
                        {
                          pGUIWindow_Settings->Release();
                          pGUIWindow_Settings = 0;
                          pMouse->SetCursorBitmap("MICON1");
                          GameUI_Footer_TimeLeft = 0;
                          _50C9A0_IsEnchantingInProgress = 0;
                          back_to_game();
                        }
                      }
                      if ( (signed int)uActiveCharacter < 1 || (signed int)uActiveCharacter > 4 )
                        uActiveCharacter = pParty->GetNextActiveCharacter();
                      pGUIWindow_CurrentMenu->Release();
                      if ( pGUIWindow_CurrentMenu == window_SpeakInHouse )
                        window_SpeakInHouse = 0;
                      pGUIWindow_CurrentMenu = 0;
                      pEventTimer->Resume();
                      pCurrentScreen = SCREEN_GAME;
                      viewparams->bRedrawGameUI = true;
                      pIcons_LOD->RemoveTexturesFromTextureList();
                      continue;
                    default:
                      if ( pGUIWindow_Settings )
                      {
                        if ( pCurrentScreen == SCREEN_CHARACTERS )
                          pMouse->SetCursorBitmap("MICON2");
                        else
                        {
                          pGUIWindow_Settings->Release();
                          pGUIWindow_Settings = 0;
                          pMouse->SetCursorBitmap("MICON1");
                          GameUI_Footer_TimeLeft = 0;
                          _50C9A0_IsEnchantingInProgress = 0;
                          back_to_game();
                        }
                      }
                      if ( (signed int)uActiveCharacter < 1 || (signed int)uActiveCharacter > 4 )
                        uActiveCharacter = pParty->GetNextActiveCharacter();
                      pGUIWindow_CurrentMenu->Release();
                      if ( pGUIWindow_CurrentMenu == window_SpeakInHouse )
                        window_SpeakInHouse = 0;
                      pGUIWindow_CurrentMenu = 0;
                      pEventTimer->Resume();
                      pCurrentScreen = SCREEN_GAME;
                      viewparams->bRedrawGameUI = 1;
                      pIcons_LOD->RemoveTexturesFromTextureList();
                      continue;
                  }
                  if ( pGUIWindow_Settings )
                  {
                    if ( pCurrentScreen == SCREEN_CHARACTERS )
                      pMouse->SetCursorBitmap("MICON2");
                    else
                    {
                      pGUIWindow_Settings->Release();
                      pGUIWindow_Settings = 0;
                      pMouse->SetCursorBitmap("MICON1");
                      GameUI_Footer_TimeLeft = 0;
                      _50C9A0_IsEnchantingInProgress = 0;
                      back_to_game();
                    }
                  }
                  if ( (signed int)uActiveCharacter < 1 || (signed int)uActiveCharacter > 4 )
                    uActiveCharacter = pParty->GetNextActiveCharacter();
                  pGUIWindow_CurrentMenu->Release();
                  if ( pGUIWindow_CurrentMenu == window_SpeakInHouse )
                    window_SpeakInHouse = 0;
                  pGUIWindow_CurrentMenu = 0;
                  pEventTimer->Resume();
                  pCurrentScreen = SCREEN_GAME;
                  viewparams->bRedrawGameUI = true;
                  pIcons_LOD->RemoveTexturesFromTextureList();
                  continue;
                }
                CharacterUI_ReleaseButtons();
                ReleaseAwardsScrollBar();
                pIcons_LOD->RemoveTexturesPackFromTextureList();
              }
              if ( pGUIWindow_Settings )
              {
                if ( pCurrentScreen == SCREEN_CHARACTERS )
                  pMouse->SetCursorBitmap("MICON2");
                else
                {
                  pGUIWindow_Settings->Release();
                  pGUIWindow_Settings = 0;
                  pMouse->SetCursorBitmap("MICON1");
                  GameUI_Footer_TimeLeft = 0;
                  _50C9A0_IsEnchantingInProgress = 0;
                  back_to_game();
                }
              }
              if ( (signed int)uActiveCharacter < 1 || (signed int)uActiveCharacter > 4 )
                uActiveCharacter = pParty->GetNextActiveCharacter();
              pGUIWindow_CurrentMenu->Release();
              if ( pGUIWindow_CurrentMenu == window_SpeakInHouse )
                window_SpeakInHouse = 0;
              pGUIWindow_CurrentMenu = 0;
              pEventTimer->Resume();
              pCurrentScreen = SCREEN_GAME;
              viewparams->bRedrawGameUI = true;
              pIcons_LOD->RemoveTexturesFromTextureList();
              continue;
            }
            if ( !pGUIWindow_Settings )//Draw Menu
            {
              dword_6BE138 = -1;
              GUIWindow::Create(0x25Au, 0x1C2u, 0, 0, WINDOW_PressedButton2, (int)pBtn_GameSettings, (char *)1);
              pEventTimer->Pause();
              pAudioPlayer->StopChannels(-1, -1);
              pCurrentScreen = SCREEN_MENU;
              
              ++pIcons_LOD->uTexturePacksCount;
              if ( !pIcons_LOD->uNumPrevLoadedFiles )
                pIcons_LOD->uNumPrevLoadedFiles = pIcons_LOD->uNumLoadedFiles;

              pGUIWindow_CurrentMenu = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_OptionsButtons, 0, 0);
              uTextureID_Options = pIcons_LOD->LoadTexture("options", TEXTURE_16BIT_PALETTE);
              uTextureID_New1 = pIcons_LOD->LoadTexture("new1", TEXTURE_16BIT_PALETTE);
              uTextureID_Load1 = pIcons_LOD->LoadTexture("load1", TEXTURE_16BIT_PALETTE);
              uTextureID_Save1 = pIcons_LOD->LoadTexture("save1", TEXTURE_16BIT_PALETTE);
              uTextureID_Controls1 = pIcons_LOD->LoadTexture("controls1", TEXTURE_16BIT_PALETTE);
              uTextureID_Resume1 = pIcons_LOD->LoadTexture("resume1", TEXTURE_16BIT_PALETTE);
              uTextureID_Quit1 = pIcons_LOD->LoadTexture("quit1", TEXTURE_16BIT_PALETTE);
              pBtn_NewGame = pGUIWindow_CurrentMenu->CreateButton(0x13u, 0x9Bu, 0xD6u, 0x28u, 1, 0, UIMSG_StartNewGame, 0, 0x4Eu,
                             pGlobalTXT_LocalizationStrings[614],// "New Game"
                             pIcons_LOD->GetTexture(uTextureID_New1), 0);
              pBtn_SaveGame = pGUIWindow_CurrentMenu->CreateButton(0x13u, 0xD1u, 0xD6u, 0x28u, 1, 0, UIMSG_Game_OpenSaveGameDialog, 0, 0x53u,
                             pGlobalTXT_LocalizationStrings[615],// "Save Game"
                             pIcons_LOD->GetTexture(uTextureID_Save1), 0);
              pBtn_LoadGame = pGUIWindow_CurrentMenu->CreateButton(19, 263, 0xD6u, 0x28u, 1, 0, UIMSG_Game_OpenLoadGameDialog, 0, 0x4Cu,
                             pGlobalTXT_LocalizationStrings[616],// "Load Game"
                             pIcons_LOD->GetTexture(uTextureID_Load1), 0);
              pBtn_GameControls = pGUIWindow_CurrentMenu->CreateButton(241, 155, 214, 40, 1, 0, UIMSG_Game_OpenOptionsDialog, 0, 0x43u,
                             pGlobalTXT_LocalizationStrings[617],// ""Sound, Keyboard, Game Options:""
                             pIcons_LOD->GetTexture(uTextureID_Controls1), 0);
              pBtn_QuitGame = pGUIWindow_CurrentMenu->CreateButton(241, 209, 214, 40, 1, 0, UIMSG_Quit, 0, 0x51u,
                             pGlobalTXT_LocalizationStrings[618],// "Quit"
                             pIcons_LOD->GetTexture(uTextureID_Quit1), 0);
              pBtn_Resume = pGUIWindow_CurrentMenu->CreateButton(241, 263, 214, 40, 1, 0, UIMSG_GameMenu_ReturnToGame, 0, 0x52u,
                             pGlobalTXT_LocalizationStrings[619],// "Return to Game"
                             pIcons_LOD->GetTexture(uTextureID_Resume1), 0);
              pGUIWindow_CurrentMenu->_41D08F_set_keyboard_control_group(6, 1, 0, 0);
              viewparams->field_48 = 1;

              stru_506E40.Release();
              pRenderer->SaveScreenshot("gamma.pcx", 155, 117);
              stru_506E40.LoadPCXFile("gamma.pcx", 0);

              continue;
            }
            pGUIWindow_Settings->Release();
            pGUIWindow_Settings = 0;
            pMouse->SetCursorBitmap("MICON1");
            GameUI_Footer_TimeLeft = 0;
            _50C9A0_IsEnchantingInProgress = 0;
            back_to_game();
            continue;
          }
          ModalWindow_Release();
          continue;
        case UIMSG_ScrollNPCPanel://Right and Left button for NPCPanel
          if ( uMessageParam )
          {
            GUIWindow::Create(626, 179, 0, 0, WINDOW_PressedButton2, (int)pBtn_NPCRight, 0);
            v37 = (pParty->pHirelings[0].pName != 0) + (pParty->pHirelings[1].pName != 0) + (unsigned __int8)pParty->field_70A - 2;
            if ( pParty->hirelingScrollPosition < v37 )
            {
              ++pParty->hirelingScrollPosition;//??? maybe number of the first cell???
              if ( pParty->hirelingScrollPosition >= v37 )
                pParty->hirelingScrollPosition = (pParty->pHirelings[0].pName != 0) + (pParty->pHirelings[1].pName != 0) + pParty->field_70A - 2;
            }
          }
          else
          {
            GUIWindow::Create(469, 179, 0, 0, WINDOW_PressedButton2, (int)pBtn_NPCLeft, 0);
            /*if ( pParty->field_709 )
            {
              --pParty->field_709;
              if ( pParty->field_709 < 1 )
                pParty->field_709 = 0;
            }*/
          }
          GameUI_DrawHiredNPCs();
          continue;
        case UIMSG_TransitionUI_Confirm:
          if ( pMessageQueue_50CBD0->uNumMessages )
            pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
          dword_50CDC8 = 1;
          sub_42FBDD();
          PlayHouseSound(uCurrentHouse_Animation, HouseSound_NotEnoughMoney_TrainingSuccessful);

          if (pMovie_Track)
            pMediaPlayer->Unload();
          DialogueEnding();
          viewparams->bRedrawGameUI = true;
          if ( dword_59117C_teleportx | dword_591178_teleporty | dword_591174_teleportz | dword_591170_teleport_directiony | dword_59116C_teleport_directionx | dword_591168_teleport_speedz )
          {
            if ( dword_59117C_teleportx )
            {
              pParty->vPosition.x = dword_59117C_teleportx;
              _5B65A8_npcdata_uflags_or_other = dword_59117C_teleportx;
            }
            if ( dword_591178_teleporty )
            {
              pParty->vPosition.y = dword_591178_teleporty;
              _5B65AC_npcdata_fame_or_other = dword_591178_teleporty;
            }
            if ( dword_591174_teleportz )
            {
              pParty->vPosition.z = dword_591174_teleportz;
              _5B65B0_npcdata_rep_or_other = dword_591174_teleportz;
              pParty->uFallStartY = dword_591174_teleportz;
            }
            if ( dword_591170_teleport_directiony )
            {
              pParty->sRotationY = dword_591170_teleport_directiony;
              _5B65B4_npcdata_loword_house_or_other = dword_591170_teleport_directiony;
            }
            if ( dword_59116C_teleport_directionx )
            {
              pParty->sRotationX = dword_59116C_teleport_directionx;
              _5B65B8_npcdata_hiword_house_or_other = dword_59116C_teleport_directionx;
              v38 = dword_591168_teleport_speedz;
              pParty->uFallSpeed = dword_591168_teleport_speedz;
              dword_5B65BC = dword_591168_teleport_speedz;
            }
            else
              v38 = dword_5B65BC;
            if ( *dword_591164_teleport_map_name != 48 )
            {
              pGameLoadingUI_ProgressBar->uType = (GUIProgressBar::Type)2;
              dword_5B65C0 = _5B65A8_npcdata_uflags_or_other | _5B65AC_npcdata_fame_or_other | _5B65B0_npcdata_rep_or_other | _5B65B4_npcdata_loword_house_or_other | _5B65B8_npcdata_hiword_house_or_other | v38;
              OnMapLeave();
              Transition_StopSound_Autosave(dword_591164_teleport_map_name, MapStartPoint_Party);
            }
          }
          else
            EventProcessor(dword_5C3418, 0, 1, dword_5C341C);
          if ( !_stricmp(byte_6BE3B0.data(), "d05.blv") )
            pParty->uTimePlayed += 1474560i64;
          continue;
        case UIMSG_TransitionWindowCloseBtn:
          CloseWindowBackground();
          pMediaPlayer->Unload();
          DialogueEnding();
          viewparams->bRedrawGameUI = true;
          continue;
        case UIMSG_CycleCharacters:
          v39 = GetAsyncKeyState(VK_SHIFT);
          uActiveCharacter = CycleCharacter(v39);
          viewparams->bRedrawGameUI = true;
          continue;
        case UIMSG_OnTravelByFoot:
          if ( pMessageQueue_50CBD0->uNumMessages )
            pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
          dword_50CDC8 = 1;
          sub_42FBDD();
          //pNPCData4 = (NPCData *)GetTravelTime();
          strcpy(pOutdoor->pLevelFilename, pCurrentMapName);
          if ( bUnderwater != 1 && pParty->bFlying
            || pOutdoor->GetTravelDestination(pParty->vPosition.x, pParty->vPosition.y, pOut, 20) != 1 )
          {
            viewparams->bRedrawGameUI = 1;
            CloseWindowBackground();
            if ( pParty->vPosition.x < -22528 )
              pParty->vPosition.x = -22528;
            if ( pParty->vPosition.x > 22528 )
              pParty->vPosition.x = 22528;
            if ( pParty->vPosition.y < -22528 )
              pParty->vPosition.y = -22528;
            if ( pParty->vPosition.y > 22528 )
              pParty->vPosition.y = 22528;
            DialogueEnding();
            pCurrentScreen = SCREEN_GAME;
          }
          else
          {
            pParty->field_6E4 = 0;
            pParty->field_6E0 = 0;
            CastSpellInfoHelpers::_427D48();
            DialogueEnding();
            pEventTimer->Pause();
            pGameLoadingUI_ProgressBar->Initialize(GUIProgressBar::TYPE_Box);
            ++pGameLoadingUI_ProgressBar->uProgressMax;
            SaveGame(1, 0);
            pGameLoadingUI_ProgressBar->Progress();
            RestAndHeal(1440 * (signed int)GetTravelTime());
            if ( pParty->uNumFoodRations )
            {
              pParty->RestAndHeal();
              if ( ((pParty->uNumFoodRations - (signed int)GetTravelTime()) & 0x80000000u) != 0 )
              {
                pPlayer7 = pParty->pPlayers.data();
                do
                {
                  pPlayer7->SetCondition(1, 0);
                  ++pPlayer7;
                }
                while ( (signed int)pPlayer7 < (signed int)pParty->pHirelings.data() );
                ++pParty->days_played_without_rest;
              }
              Party::TakeFood((unsigned int)GetTravelTime());
            }
            else
            {
              pPlayer8 = pParty->pPlayers.data();
              do
              {
                pPlayer8->SetCondition(1, 0);
                ++pPlayer8;
              }
              while ( (signed int)pPlayer8 < (signed int)pParty->pHirelings.data() );
              ++pParty->days_played_without_rest;
            }
            pPaletteManager->ResetNonLocked();
            pSpriteFrameTable->ResetSomeSpriteFlags();
            strcpy(pCurrentMapName, pOut);
            strcpy(pLevelName, pCurrentMapName);
            v41 = strtok(pLevelName, ".");
            strcpy(pLevelName, v41);
            Level_LoadEvtAndStr(pLevelName);
            pDecalBuilder->Reset(0);
            LoadLevel_InitializeLevelEvt();
            uLevelMapStatsID = pMapStats->GetMapInfo(pCurrentMapName);
            bUnderwater = 0;
            bNoNPCHiring = 0;
            pGame->uFlags2 &= 0xFFFFFFF7u;
            if ( Is_out15odm_underwater() )
            {
              bUnderwater = 1;
              pGame->uFlags2 |= 8u;
            }
            if ( !_stricmp(pCurrentMapName, "out15.odm") || !_stricmp(pCurrentMapName, "d47.blv") )
              bNoNPCHiring = 1;
            PrepareToLoadODM(1u, (ODMRenderParams *)1);
            pAudioPlayer->SetMapEAX();
            bDialogueUI_InitializeActor_NPC_ID = 0;
            OnMapLoad();
            pOutdoor->SetFog();
            TeleportToStartingPoint(uLevel_StartingPointType);
            pParty->vPosition.z = GetTerrainHeightsAroundParty2(pParty->vPosition.x, pParty->vPosition.y, &v213, 0);
            pParty->uFallStartY = pParty->vPosition.z;
            _461103_load_level_sub();
            pEventTimer->Resume();
            viewparams->bRedrawGameUI = 1;
            pCurrentScreen = SCREEN_GAME;
            pGameLoadingUI_ProgressBar->Release();
          }
          viewparams->bRedrawGameUI = 1;
          continue;
        case UIMSG_CHANGE_LOCATION_ClickCencelBtn:
          CloseWindowBackground();
          if ( pParty->vPosition.x < -22528 )
            pParty->vPosition.x = -22528;
          if ( pParty->vPosition.x > 22528 )
            pParty->vPosition.x = 22528;
          if ( pParty->vPosition.y < -22528 )
            pParty->vPosition.y = -22528;
          if ( pParty->vPosition.y > 22528 )
            pParty->vPosition.y = 22528;
          DialogueEnding();
          pCurrentScreen = SCREEN_GAME;
          viewparams->bRedrawGameUI = 1;
          continue;
        case UIMSG_CastSpell_Telekinesis:
          //if ( pRenderer->pRenderD3D )
            LOWORD(v42) = pGame->pVisInstance->get_picked_object_zbuf_val();
          /*else
          {
            uNumSeconds = (unsigned int)pMouse->GetCursorPos(&v210);
            pPoint = pMouse->GetCursorPos(&v208);
            v42 = pRenderer->pActiveZBuffer[*(int *)uNumSeconds + pSRZBufferLineOffsets[pPoint->y]];
          }*/
          v44 = (unsigned __int16)v42;
          v45 = PID_TYPE(v44);
          uNumSeconds = v44;
          v46 = PID_ID(v44);
          if ( v45 == 3 )
          {
            v47 = pActors[v46].uAIState == Dead;
            if ( !v47 )
              continue;
            pSpellInfo = (CastSpellInfo *)pGUIWindow_Settings->ptr_1C;
            pSpellInfo->uFlags &= ~0x40u;
            pSpellInfo->uPlayerID_2 = uMessageParam;
            pSpellInfo->spell_target_pid = v44;
            pParty->pPlayers[pSpellInfo->uPlayerID].SetRecoveryTime(300);
            pGUIWindow_Settings->Release();
            pGUIWindow_Settings = 0;
            pMouse->SetCursorBitmap("MICON1");
            GameUI_Footer_TimeLeft = 0;
            _50C9A0_IsEnchantingInProgress = 0;
            back_to_game();
            continue;
          }
          if ( v45 == 2 )
          {
            v47 = (pObjectList->pObjects[pSpriteObjects[v46].uObjectDescID].uFlags & 0x10) == 0;
            if ( !v47 )
              continue;
            pSpellInfo = (CastSpellInfo *)pGUIWindow_Settings->ptr_1C;
            pSpellInfo->uFlags &= ~0x40u;
            pSpellInfo->uPlayerID_2 = uMessageParam;
            pSpellInfo->spell_target_pid = v44;
            pParty->pPlayers[pSpellInfo->uPlayerID].SetRecoveryTime(300);
            pGUIWindow_Settings->Release();
            pGUIWindow_Settings = 0;
            pMouse->SetCursorBitmap("MICON1");
            GameUI_Footer_TimeLeft = 0;
            _50C9A0_IsEnchantingInProgress = 0;
            back_to_game();
            continue;
          }
          if ( v45 == 5 )
          {
            v48 = pLevelDecorations[v46].uEventID == 0;
          }
          else
          {
            if ( v45 != 6 )
              continue;
            if ( uCurrentlyLoadedLevelType != 1 )
            {
              pODMFace = &pOutdoor->pBModels[v44 >> 9].pFaces[v46 & 0x3F];
              if ( !pODMFace->Clickable() || !pODMFace->sCogTriggeredID )
                continue;
              v44 = uNumSeconds;
              pSpellInfo = (CastSpellInfo *)pGUIWindow_Settings->ptr_1C;
              pSpellInfo->uFlags &= ~0x40u;
              pSpellInfo->uPlayerID_2 = uMessageParam;
              pSpellInfo->spell_target_pid = v44;
              pParty->pPlayers[pSpellInfo->uPlayerID].SetRecoveryTime(300);
              pGUIWindow_Settings->Release();
              pGUIWindow_Settings = 0;
              pMouse->SetCursorBitmap("MICON1");
              GameUI_Footer_TimeLeft = 0;
              _50C9A0_IsEnchantingInProgress = 0;
              back_to_game();
              continue;
            }
            pBLVFace = &pIndoor->pFaces[v46];
            if ( !pBLVFace->Clickable() )
              continue;
            v48 = pIndoor->pFaceExtras[pBLVFace->uFaceExtraID].uEventID == 0;
          }
          if ( v48 )
            continue;
          pSpellInfo = (CastSpellInfo *)pGUIWindow_Settings->ptr_1C;
          pSpellInfo->uFlags &= ~0x40u;
          pSpellInfo->uPlayerID_2 = uMessageParam;
          pSpellInfo->spell_target_pid = v44;
          pParty->pPlayers[pSpellInfo->uPlayerID].SetRecoveryTime(300);
          pGUIWindow_Settings->Release();
          pGUIWindow_Settings = 0;
          pMouse->SetCursorBitmap("MICON1");
          GameUI_Footer_TimeLeft = 0;
          _50C9A0_IsEnchantingInProgress = 0;
          back_to_game();
          continue;
        case UIMSG_CastSpell_Character_Big_Improvement://Preservation and blessing, treatment paralysis, hand hammers(individual upgrade)
        case UIMSG_CastSpell_Character_Small_Improvement://Fate, cure
        case UIMSG_HiredNPC_CastSpell:
          if ( pMessageQueue_50CBD0->uNumMessages )
            pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
          if ( _50C9A0_IsEnchantingInProgress )
          {
            uActiveCharacter = uMessageParam;
            viewparams->bRedrawGameUI = 1;
          }
          else
          {
            if ( pGUIWindow_Settings )
            {
              pSpellInfo = (CastSpellInfo *)pGUIWindow_Settings->ptr_1C;
              switch ( uMessage )
              {
                case UIMSG_CastSpell_Character_Big_Improvement:
                  pSpellInfo->uFlags &= ~0x02u;
                  break;
                case UIMSG_CastSpell_Character_Small_Improvement:
                  pSpellInfo->uFlags &= ~0x0100u;
                  break;
                case UIMSG_HiredNPC_CastSpell:
                  pSpellInfo->uFlags &= ~0x0200u;
                  break;
              }
              pSpellInfo->uPlayerID_2 = uMessageParam;
              pParty->pPlayers[pSpellInfo->uPlayerID].SetRecoveryTime(300);
              pGUIWindow_Settings->Release();
              pGUIWindow_Settings = 0;
              pEventTimer->Resume();
              pMouse->SetCursorBitmap("MICON1");
              GameUI_Footer_TimeLeft = 0;
              _50C9A0_IsEnchantingInProgress = 0;
            }
          }
          continue;
        case UIMSG_BF:
			__debugbreak();
          dword_50CDC8 = 1;
          sub_42FBDD();
          SaveGame(1, 0);
          strcpy(pCurrentMapName, pMapStats->pInfos[uHouse_ExitPic].pFilename);
          dword_6BE364_game_settings_1 |= GAME_SETTINGS_0001;
          uGameState = GAME_STATE_CHANGE_LOCATION;
          //v53 = p2DEvents_minus1_::30[26 * (unsigned int)ptr_507BC0->ptr_1C];
          v53 = p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1]._quest_related;
          if ( v53 < 0 )
          {
            v54 = abs(v53) - 1;
            _5B65B8_npcdata_hiword_house_or_other = 0;
            dword_5B65BC = 0;
            v55 = dword_4E4560[v54];
            _5B65AC_npcdata_fame_or_other = dword_4E4578[v54];
            v56 = dword_4E4590[v54];
            v57 = dword_4E45A8[v54];
            _5B65A8_npcdata_uflags_or_other = v55;
            _5B65B4_npcdata_loword_house_or_other = v57;
            _5B65B0_npcdata_rep_or_other = v56;
            dword_5B65C0 = v55 | _5B65AC_npcdata_fame_or_other | v56 | v57;
          }
          HouseDialogPressCloseBtn();
          //goto LABEL_434;
          /*if ( (signed int)pMessageQueue_50CBD0->uNumMessages < 40 )
          {
            pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].eType = UIMSG_Escape;
            pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].param = v0;
            *(&pMessageQueue_50CBD0->uNumMessages + 3 * pMessageQueue_50CBD0->uNumMessages + 3) = 0;
            ++pMessageQueue_50CBD0->uNumMessages;
          }*/
          pMessageQueue_50CBD0->AddGUIMessage(UIMSG_Escape, 1, 0);
          continue;

        case UIMSG_OnCastTownPortal:
          pAudioPlayer->StopChannels(-1, -1);
          pGUIWindow_CurrentMenu = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_Book, WINDOW_TownPortal, (char *)uMessageParam);
        continue;

        case UIMSG_OnCastLloydsBeacon:
          pAudioPlayer->StopChannels(-1, -1);
          pGUIWindow_CurrentMenu = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_Book, WINDOW_LloydsBeacon, 0);
        continue;

        case UIMSG_LloydsBeacon_FlippingBtn:
          bRecallingBeacon = uMessageParam;
          v127 = uMessageParam + 204;
          pAudioPlayer->PlaySound((SoundID)v127, 0, 0, -1, 0, 0, 0, 0);
          continue;
        case UIMSG_HintBeaconSlot:
          if ( !pGUIWindow_CurrentMenu )
            continue;
          pPlayer = pPlayers[_506348_current_lloyd_playerid + 1];
          uNumSeconds = (unsigned int)&pPlayer->pInstalledBeacons[uMessageParam];
          if ( bRecallingBeacon )
          {
            if ( !*((int *)&pSavegameThumbnails[10 * uMessageParam].pPixels ) )
              continue;
            v173 = pMapStats->pInfos[pMapStats->sub_410D99_get_map_index(pPlayer->pInstalledBeacons[uMessageParam].SaveFileID)].pName;
            sprintfex(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[474], v173);// "Recall to %s"
            GameUI_SetFooterString(pTmpBuf.data());
            continue;
          }
          pMapNum = pMapStats->GetMapInfo(pCurrentMapName);
          pMapName = "Not in Map Stats";
          if ( pMapNum )
            pMapName = pMapStats->pInfos[pMapNum].pName;
          if ( !*((int *)&pSavegameThumbnails[10 * uMessageParam].pPixels ) || !pMapNum )
          {
            sprintfex(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[476], pMapName);// "Set to %s"
            GameUI_SetFooterString(pTmpBuf.data());
            continue;
          }
          v174 = pMapStats->pInfos[pMapStats->sub_410D99_get_map_index(*(short *)(uNumSeconds + 26))].pName;
          sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[475], (unsigned int)pMapName, v174);// "Set %s over %s"
          GameUI_SetFooterString(pTmpBuf.data());
          continue;
        case UIMSG_CloseAfterInstallBeacon:
          dword_50CDC8 = 1;
          pMessageQueue_50CBD0->AddGUIMessage(UIMSG_Escape, 0, 0);
          /*if ( (signed int)pMessageQueue_50CBD0->uNumMessages >= 40 )
            continue;
          pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].eType = UIMSG_Escape;
          pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].param = 0;
          *(&pMessageQueue_50CBD0->uNumMessages + 3 * pMessageQueue_50CBD0->uNumMessages + 3) = 0;
          ++pMessageQueue_50CBD0->uNumMessages;*/
          continue;
        case UIMSG_InstallBeacon:
          pPlayer9 = pPlayers[_506348_current_lloyd_playerid + 1];
          if ( !pPlayer9->pInstalledBeacons[uMessageParam].uBeaconTime && bRecallingBeacon )
            continue;
          byte_506360 = 1;
          pPlayer9->CanCastSpell(uRequiredMana);
          if ( pParty->bTurnBasedModeOn )
          {
            v60 = sRecoveryTime;
            pParty->pTurnBasedPlayerRecoveryTimes[_506348_current_lloyd_playerid] = sRecoveryTime;
            pPlayer9->SetRecoveryTime(v60);
            pTurnEngine->ApplyPlayerAction();
          }
          else
          {
            pPlayer9->SetRecoveryTime((signed __int64)(flt_6BE3A4_debug_recmod1 * (double)sRecoveryTime * 2.133333333333333));
          }
          pAudioPlayer->PlaySound((SoundID)word_4EE088_sound_ids[lloyds_beacon_spell_id], 0, 0, -1, 0, lloyds_beacon_sound_id, 0, 0);
          if ( bRecallingBeacon )
          {
            if ( _stricmp(pCurrentMapName, (const char *)&pGames_LOD->pSubIndices[pPlayer9->pInstalledBeacons[uMessageParam].SaveFileID]) )
            {
              SaveGame(1, 0);
              OnMapLeave();
              strcpy(pCurrentMapName, (const char *)&pGames_LOD->pSubIndices[pPlayer9->pInstalledBeacons[uMessageParam].SaveFileID]);
              dword_6BE364_game_settings_1 |= GAME_SETTINGS_0001;
              uGameState = GAME_STATE_CHANGE_LOCATION;
              _5B65A8_npcdata_uflags_or_other = pPlayer9->pInstalledBeacons[uMessageParam].PartyPos_X;
              _5B65AC_npcdata_fame_or_other = pPlayer9->pInstalledBeacons[uMessageParam].PartyPos_Y;
              _5B65B0_npcdata_rep_or_other = pPlayer9->pInstalledBeacons[uMessageParam].PartyPos_Z;
              _5B65B4_npcdata_loword_house_or_other = pPlayer9->pInstalledBeacons[uMessageParam].PartyRot_X;
              _5B65B8_npcdata_hiword_house_or_other = pPlayer9->pInstalledBeacons[uMessageParam].PartyRot_Y;
              dword_5B65C0 = 1;
            }
            else
            {
              pParty->vPosition.x = pPlayer9->pInstalledBeacons[uMessageParam].PartyPos_X;
              pParty->vPosition.y = pPlayer9->pInstalledBeacons[uMessageParam].PartyPos_Y;
              pParty->vPosition.z = pPlayer9->pInstalledBeacons[uMessageParam].PartyPos_Z;
              pParty->uFallStartY = pParty->vPosition.z;
              pParty->sRotationY = pPlayer9->pInstalledBeacons[uMessageParam].PartyRot_X;
              pParty->sRotationX = pPlayer9->pInstalledBeacons[uMessageParam].PartyRot_Y;
            }
            pMessageQueue_50CBD0->AddGUIMessage(UIMSG_Escape, 1, 0);
            pBooksWindow->Release();
            pGUIWindow_CurrentMenu->Release();
            pBooksWindow = 0;
            pGUIWindow_CurrentMenu = 0;
          }
          else
          {
            sprintf(a1, "data\\lloyd%d%d.pcx", _506348_current_lloyd_playerid + 1, uMessageParam + 1);
            pRenderer->SaveScreenshot(a1, 92, 68);
            LoadThumbnailLloydTexture(uMessageParam, _506348_current_lloyd_playerid + 1);
            pPlayer9->pInstalledBeacons[uMessageParam].uBeaconTime = pParty->uTimePlayed + (signed __int64)((double)(lloyds_beacon_spell_level << 7) * 0.033333335);
            pPlayer9->pInstalledBeacons[uMessageParam].PartyPos_X = pParty->vPosition.x;
            pPlayer9->pInstalledBeacons[uMessageParam].PartyPos_Y = pParty->vPosition.y;
            pPlayer9->pInstalledBeacons[uMessageParam].PartyPos_Z = pParty->vPosition.z;
            pPlayer9->pInstalledBeacons[uMessageParam].PartyRot_X = LOWORD(pParty->sRotationY);
            pPlayer9->pInstalledBeacons[uMessageParam].PartyRot_Y = LOWORD(pParty->sRotationX);
            if ( (signed int)pGames_LOD->uNumSubDirs / 2 <= 0 )
              continue;
            for ( thisg = 0; thisg < (signed int)pGames_LOD->uNumSubDirs / 2; ++thisg )
            {
              if ( !_stricmp(pGames_LOD->pSubIndices[thisg].pFilename, pCurrentMapName) )
                pPlayer9->pInstalledBeacons[uMessageParam].SaveFileID = thisg;
            }
          }
          continue;
        case UIMSG_ClickTownInTP:
          if ( uMessageParam )
          {
            switch ( uMessageParam )
            {
              case 1:
                v63 = 208;
                break;
              case 2:
                v63 = 207;
                break;
              case 3:
                v63 = 211;
                break;
              case 4:
                v63 = 209;
                break;
              default:
                if ( uMessageParam != 5 )
                {
LABEL_486:
                  SaveGame(1, 0);
                  v64 = pMapStats->GetMapInfo(pCurrentMapName);
                  v65 = uMessageParam;
                  if ( v64 == TownPortalList[uMessageParam].uMapInfoID )
                  {
                    pParty->vPosition.x = TownPortalList[v65].pos.x;
                    pParty->vPosition.y = TownPortalList[v65].pos.y;
                    pParty->vPosition.z = TownPortalList[v65].pos.z;
                    pParty->uFallStartY = pParty->vPosition.z;
                    pParty->sRotationY = TownPortalList[v65].rot_y;
                    pParty->sRotationX = TownPortalList[v65].rot_x;
                  }
                  else
                  {
                    SaveGame(1, 0);
                    OnMapLeave();
                    dword_6BE364_game_settings_1 |= GAME_SETTINGS_0001;
                    uGameState = GAME_STATE_CHANGE_LOCATION;
                    strcpy(pCurrentMapName, pMapStats->pInfos[TownPortalList[uMessageParam].uMapInfoID].pFilename);
                    dword_5B65C0 = 1;
                    _5B65A8_npcdata_uflags_or_other = TownPortalList[uMessageParam].pos.x;
                    _5B65AC_npcdata_fame_or_other = TownPortalList[uMessageParam].pos.y;
                    _5B65B0_npcdata_rep_or_other = TownPortalList[uMessageParam].pos.z;
                    v66 = TownPortalList[uMessageParam].rot_x;
                    _5B65B4_npcdata_loword_house_or_other = TownPortalList[uMessageParam].rot_y;
                    _5B65B8_npcdata_hiword_house_or_other = v66;
                    Actor::InitializeActors();
                  }
                  v67 = (char*)pGUIWindow_CurrentMenu->Hint;
                  if ( v67 )
                    *((int *)v67 + 17) = 1;
                  else
                    pParty->pPlayers[(unsigned __int8)town_portal_caster_id].CanCastSpell(0x14u);
                  /*if ( (signed int)pMessageQueue_50CBD0->uNumMessages < 40 )
                  {
                    pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].eType = UIMSG_Escape;
                    pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].param = v0;
                    *(&pMessageQueue_50CBD0->uNumMessages + 3 * pMessageQueue_50CBD0->uNumMessages + 3) = 0;
                    ++pMessageQueue_50CBD0->uNumMessages;
                  }*/
                  pMessageQueue_50CBD0->AddGUIMessage(UIMSG_Escape, 1, 0);
                  continue;
                }
                v63 = 210;
                break;
            }
          }
          else
          {
            v63 = 206;
          }
          if ( !(unsigned __int16)_449B57_test_bit(pParty->_quest_bits, v63) )
            return;
          goto LABEL_486;
        case UIMSG_HintTownPortal:
          if ( uMessageParam )
          {
            switch ( uMessageParam )
            {
              case 1:
                v68 = 208;
                break;
              case 2:
                v68 = 207;
                break;
              case 3:
                v68 = 211;
                break;
              case 4:
                v68 = 209;
                break;
              default:
                if ( uMessageParam != 5 )
                  //goto LABEL_506;
                {
                  if ( uMessageParam )
                  {
                    switch ( uMessageParam )
                    {
                    case 1:
                      v69 = pMapStats->pInfos[4].pName;
                      break;
                    case 2:
                      v69 = pMapStats->pInfos[3].pName;
                      break;
                    case 3:
                      v69 = pMapStats->pInfos[10].pName;
                      break;
                    case 4:
                      v69 = pMapStats->pInfos[7].pName;
                      break;
                    default:
                    if ( uMessageParam != 5 )
                    {
                      __debugbreak(); // warning C4700: uninitialized local variable 'v200' used
                      sprintfex(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[35], v200);
                      GameUI_SetFooterString(pTmpBuf.data());
                      continue;
                    }
                    v69 = pMapStats->pInfos[8].pName;
                    break;
                  }
                }
                else
                {
                  v69 = pMapStats->pInfos[21].pName;
                }
                sprintfex(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[35], v69);
                GameUI_SetFooterString(pTmpBuf.data());
                continue;
              }
              v68 = 210;
              break;
            }
          }
          else
          {
            v68 = 206;
          }
          if ( !(unsigned __int16)_449B57_test_bit(pParty->_quest_bits, v68) )
          {
            pRenderer->DrawTextureRGB(0, 0x160u, pTexture_StatusBar);
            continue;
          }
//LABEL_506:
          if ( uMessageParam )
          {
            switch ( uMessageParam )
            {
              case 1:
                v69 = pMapStats->pInfos[4].pName;
                break;
              case 2:
                v69 = pMapStats->pInfos[3].pName;
                break;
              case 3:
                v69 = pMapStats->pInfos[10].pName;
                break;
              case 4:
                v69 = pMapStats->pInfos[7].pName;
                break;
              default:
                if ( uMessageParam != 5 )
                  //goto LABEL_519;
                  {
                    sprintfex(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[35], v200);
                    GameUI_SetFooterString(pTmpBuf.data());
                    continue;
                  }
                v69 = pMapStats->pInfos[8].pName;
                break;
            }
          }
          else
          {
            v69 = pMapStats->pInfos[21].pName;
          }
          sprintfex(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[35], v69);
          GameUI_SetFooterString(pTmpBuf.data());
          continue;
        case UIMSG_ShowFinalWindow:
          sprintfex(pFinalMessage.data(), "%s\n \n%s\n \n%s", pGlobalTXT_LocalizationStrings[151],// "Congratulations Adventurer."
            pGlobalTXT_LocalizationStrings[118],// "We hope that you've enjoyed playing Might and Magic VII as much as we did making it. We have saved this screen as MM7_WIN.PCX in your MM7 directory. You can print it out as proof of your accomplishment."
            pGlobalTXT_LocalizationStrings[167]);// "- The Might and Magic VII Development Team."   
          ModalWindow(pFinalMessage.data(), UIMSG_OnFinalWindowClose);
          uGameState = GAME_STATE_FINAL_WINDOW;
          continue;
        case UIMSG_OnFinalWindowClose:
			__debugbreak();
          uGameState = GAME_STATE_PLAYING;
          strcpy((char *)pKeyActionMap->pPressedKeysBuffer, "2");
          __debugbreak(); // missed break/continue?
        case UIMSG_DD:
			__debugbreak();
          sprintf(pTmpBuf.data(), "%s", pKeyActionMap->pPressedKeysBuffer);
          memcpy(&v216, txt_file_frametable_parser(pKeyActionMap->pPressedKeysBuffer, &v218), sizeof(v216));
          if ( v216.uPropCount == 1 )
          {
            pNPCData4 = (NPCData *)((signed int)pGames_LOD->uNumSubDirs / 2);
            v70 = atoi(v216.pProperties[0]);
            if ( v70 <= 0 || v70 >= 77 )
              continue;
            v71 = v70;
            strcpy(Str2, pMapStats->pInfos[v70].pFilename);
            pNPCData3 = 0;
            if ( (signed int)pNPCData4 > 0 )
            {
              thish = 0;
              do
              {
                if ( !_stricmp(pGames_LOD->pSubIndices[thish].pFilename, Str2) )
                  break;
                ++thish;
                pNPCData3 = (NPCData *)((char *)pNPCData3 + 1);
              }
              while ( (signed int)pNPCData3 < (signed int)pNPCData4 );
              if ( (signed int)pNPCData3 < (signed int)pNPCData4 )
              {
                strcpy(pCurrentMapName, pGames_LOD->pSubIndices[(int)pNPCData3].pFilename);
                dword_6BE364_game_settings_1 |= GAME_SETTINGS_0001;
                uGameState = GAME_STATE_CHANGE_LOCATION;
                OnMapLeave();
                continue;
              }
            }
            sprintf(Str2, "No map found for %s", pMapStats->pInfos[v71].pName);
            v73 = Str2;
          }
          else
          {
            if ( v216.uPropCount != 3 )
              continue;
            v74 = atoi(v216.pProperties[0]);
            thisi = atoi(v216.pProperties[1]);
            v75 = atoi(v216.pProperties[2]);
            v76 = v75;
            if ( uCurrentlyLoadedLevelType == LEVEL_Indoor)
            {
              if ( pIndoor->GetSector(v74, thisi, v75) )
              {
                v77 = thisi;
                pParty->vPosition.x = v74;
                pParty->vPosition.y = v77;
                pParty->vPosition.z = v76;
                pParty->uFallStartY = v76;
                continue;
              }
            }
            else
            {
              if ( v74 > -32768 )
              {
                if ( v74 < 32768 )
                {
                  v77 = thisi;
                  if ( thisi > -32768 )
                  {
                    if ( thisi < 32768 && v76 >= 0 && v76 < 10000 )
                    {
                      pParty->vPosition.x = v74;
                      pParty->vPosition.y = v77;
                      pParty->vPosition.z = v76;
                      pParty->uFallStartY = v76;
                      continue;
                    }
                  }
                }
              }
            }
            pAudioPlayer->PlaySound(SOUND_error, 0, 0, -1, 0, 0, 0, 0);
            v73 = "Can't jump to that location!";
          }
          ShowStatusBarString(v73, 6u);
          continue;
        case UIMSG_CastQuickSpell:
          if ( bUnderwater == 1 )
          {
            ShowStatusBarString(pGlobalTXT_LocalizationStrings[652], 2);// "You can not do that while you are underwater!"
            pAudioPlayer->PlaySound((SoundID)27, 0, 0, -1, 0, 0, 0, 0);
            continue;
          }
          if ( !uActiveCharacter || (pPlayer2 = pPlayers[uActiveCharacter], pPlayer2->uTimeToRecovery) )
            continue;
          _42777D_CastSpell_UseWand_ShootArrow(pPlayer2->uQuickSpell, uActiveCharacter - 1, 0, 0, uActiveCharacter);
          continue;
        case UIMSG_CastSpell_Monster_Improvement:
        case UIMSG_CastSpell_Shoot_Monster://FireBlow, Lightning, Ice Lightning, Swarm, 
          //if ( pRenderer->pRenderD3D )
          {
            v81 = pGame->pVisInstance->get_picked_object_zbuf_val();
          }
          /*else
          {
            uNumSeconds = (unsigned int)pMouse->GetCursorPos(&v206);
            pPoint2 = pMouse->GetCursorPos(&v201);
            v81 = pRenderer->pActiveZBuffer[*(int *)uNumSeconds + pSRZBufferLineOffsets[pPoint2->y]];
          }*/
          v83 = v81;
          v44 = (unsigned __int16)v81;
          v84 = v83 >> 16;
          if ( PID_TYPE(v44) != 3 || v84 >= 5120 )
            continue;
          pSpellInfo = (CastSpellInfo *)pGUIWindow_Settings->ptr_1C;
          if ( uMessage == UIMSG_CastSpell_Shoot_Monster )
          {
            pSpellInfo->uFlags &= ~0x08;
          }
          else
          {
            if ( uMessage == UIMSG_CastSpell_Monster_Improvement )
              pSpellInfo->uFlags &= ~0x0100u;
            else
              pSpellInfo->uFlags &= ~0x0200u;
          }
          pSpellInfo->uPlayerID_2 = uMessageParam;
          pSpellInfo->spell_target_pid = v44;
          pParty->pPlayers[pSpellInfo->uPlayerID].SetRecoveryTime(300);
          pGUIWindow_Settings->Release();
          pGUIWindow_Settings = 0;
          pMouse->SetCursorBitmap("MICON1");
          GameUI_Footer_TimeLeft = 0;
          _50C9A0_IsEnchantingInProgress = 0;
          back_to_game();
          continue;
        case UIMSG_1C:
			__debugbreak();
          if ( !uActiveCharacter || pCurrentScreen )
            continue;
          ptr_507BC8 = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_68, uMessageParam, 0);
          pCurrentScreen = SCREEN_19;
          pEventTimer->Pause();
          continue;
        case UIMSG_STEALFROMACTOR:
          if ( !uActiveCharacter )
            continue;
          if ( pParty->bTurnBasedModeOn != 1 )
          {
            if ( pActors[uMessageParam].uAIState == 5 )
              pActors[uMessageParam].LootActor();
            else
              Actor::StealFrom(uMessageParam);
            continue;
          }
          if ( pTurnEngine->turn_stage == TE_WAIT || pTurnEngine->turn_stage == TE_MOVEMENT )
            continue;
          if ( !(pTurnEngine->field_18 & TE_HAVE_PENDING_ACTIONS) )
          {
            if ( pActors[uMessageParam].uAIState == 5 )
              pActors[uMessageParam].LootActor();
            else
              Actor::StealFrom(uMessageParam);
          }
          continue;

        case UIMSG_Attack:
          if ( !uActiveCharacter )
            continue;
          if ( pParty->bTurnBasedModeOn != 1 )
          {
            Player::_42ECB5_PlayerAttacksActor();
            continue;
          }
          if ( pTurnEngine->turn_stage == TE_WAIT || pTurnEngine->turn_stage == TE_MOVEMENT )
            continue;
          if ( !(pTurnEngine->field_18 & TE_HAVE_PENDING_ACTIONS) )
            Player::_42ECB5_PlayerAttacksActor();
          continue;
        case UIMSG_ExitRest:
          GUIWindow::Create(pButton_RestUI_Exit->uX, pButton_RestUI_Exit->uY, 0, 0, WINDOW_CloseRestWindowBtn, (int)pButton_RestUI_Exit, pGlobalTXT_LocalizationStrings[81]);// "Exit Rest"
          continue;
        case UIMSG_Wait5Minutes:
          if ( dword_506F14 == 2 )
          {
            ShowStatusBarString(pGlobalTXT_LocalizationStrings[477], 2);// "You are already resting!"
            pAudioPlayer->PlaySound((SoundID)27, 0, 0, -1, 0, 0, 0, 0);
            continue;
          }
          GUIWindow::Create(pButton_RestUI_Wait5Minutes->uX, pButton_RestUI_Wait5Minutes->uY, 0, 0, WINDOW_PressedButton2,
            (int)pButton_RestUI_Wait5Minutes, pGlobalTXT_LocalizationStrings[238]);// "Wait 5 Minutes"
          dword_506F14 = 1;
          _506F18_num_minutes_to_sleep = 5;
          continue;
        case UIMSG_Wait1Hour:
          if ( dword_506F14 == 2 )
          {
            ShowStatusBarString(pGlobalTXT_LocalizationStrings[477], 2);// "You are already resting!"
            pAudioPlayer->PlaySound((SoundID)27, 0, 0, -1, 0, 0, 0, 0);
            continue;
          }
          GUIWindow::Create(pButton_RestUI_Wait1Hour->uX, pButton_RestUI_Wait1Hour->uY, 0, 0, WINDOW_PressedButton2,
            (int)pButton_RestUI_Wait1Hour, pGlobalTXT_LocalizationStrings[239]);// "Wait 1 Hour"
          dword_506F14 = 1;
          _506F18_num_minutes_to_sleep = 60;
          continue;
        case UIMSG_RentRoom:
          dword_506F14 = 2;
          RestUI_Load();
          v86 = 60 * (_494820_training_time(pParty->uCurrentHour) + 1) - pParty->uCurrentMinute;
          _506F18_num_minutes_to_sleep = v86;
          if ( uMessageParam == 111 || uMessageParam == 114 || uMessageParam == 116 ) // 107 = Emerald Isle tavern
            _506F18_num_minutes_to_sleep = v86 + 12 * 60;
          dword_506F14 = 2;
          pParty->RestAndHeal();
          pParty->days_played_without_rest = 0;
          pParty->pPlayers[3].SetAsleep(1);
          pParty->pPlayers[2].SetAsleep(1);
          pParty->pPlayers[1].SetAsleep(1);
          pParty->pPlayers[0].SetAsleep(1);
          continue;
        case UIMSG_RestWindow:
          if ( pMessageQueue_50CBD0->uNumMessages )
            pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
          if ( pCurrentScreen )
            continue;
          if ( CheckActors_proximity() )
          {
            if ( pParty->bTurnBasedModeOn == 1 )
            {
              ShowStatusBarString(pGlobalTXT_LocalizationStrings[478], 2);// "You can't rest in turn-based mode!"
              continue;
            }
            v88 = pGlobalTXT_LocalizationStrings[480];// "There are hostile enemies near!"
            if ( pParty->uFlags & 0x88 )
              v88 = pGlobalTXT_LocalizationStrings[479];// "You can't rest here!"
            ShowStatusBarString(v88, 2);
            if ( !uActiveCharacter )
              continue;
            pPlayers[uActiveCharacter]->PlaySound((PlayerSpeech)13, 0);
            continue;
          }
          if ( pParty->bTurnBasedModeOn == 1 )
          {
            ShowStatusBarString(pGlobalTXT_LocalizationStrings[478], 2);// "You can't rest in turn-based mode!"
            continue;
          }
          if ( !(pParty->uFlags & 0x88) )
          {
            RestUI_Load();
            continue;
          }
          if ( pParty->bTurnBasedModeOn == 1 )
          {
            ShowStatusBarString(pGlobalTXT_LocalizationStrings[478], 2);// "You can't rest in turn-based mode!"
            continue;
          }
          v88 = pGlobalTXT_LocalizationStrings[480];// "There are hostile enemies near!"
          if ( pParty->uFlags & 0x88 )
            v88 = pGlobalTXT_LocalizationStrings[479];// "You can't rest here!"
          ShowStatusBarString(v88, 2u);
          if ( !uActiveCharacter )
            continue;
          pPlayers[uActiveCharacter]->PlaySound((PlayerSpeech)13, 0);
          continue;
        case UIMSG_Rest8Hour:
          if ( dword_506F14 )
          {
            ShowStatusBarString(pGlobalTXT_LocalizationStrings[477], 2);// "You are already resting!"
            pAudioPlayer->PlaySound((SoundID)27, 0, 0, -1, 0, 0, 0, 0);
            continue;
          }
          if ( pParty->uNumFoodRations < uRestUI_FoodRequiredToRest )
          {
            ShowStatusBarString(pGlobalTXT_LocalizationStrings[482], 2u);// "You don't have enough food to rest"
            if ( uActiveCharacter && pPlayers[uActiveCharacter]->CanAct() )
              pPlayers[uActiveCharacter]->PlaySound((PlayerSpeech)SPEECH_108, 0);
          }
          else
          {
            pParty->pPlayers[3].pConditions[Condition_Sleep] = pParty->uTimePlayed;
            pParty->pPlayers[2].pConditions[Condition_Sleep] = pParty->uTimePlayed;
            pParty->pPlayers[1].pConditions[Condition_Sleep] = pParty->uTimePlayed;
            pParty->pPlayers[0].pConditions[Condition_Sleep] = pParty->uTimePlayed;
            v90 = pMapStats->GetMapInfo(pCurrentMapName);
            if ( !v90 )
              v90 = rand() % (signed int)pMapStats->uNumMaps + 1;
            pMapInfo = &pMapStats->pInfos[v90];
            if ( rand() % 100 + 1 <= pMapInfo->Encounter_percent )
            {
              v91 = rand() % 100;
              v92 = pMapInfo->EncM1percent;
              v93 = v91 + 1;
              if ( v93 > v92 )
                pNPCData4 = (NPCData *)((v93 > v92 + pMapInfo->EncM2percent) + 2);
              else
                pNPCData4 = (NPCData *)1;
              if ( !_45063B_spawn_some_monster(pMapInfo, (int)pNPCData4) )
                pNPCData4 = 0;
              if ( pNPCData4 )
              {
                pPlayerNum = rand() % 4;
                pParty->pPlayers[pPlayerNum].pConditions[Condition_Sleep] = 0;
                v95 = rand();
                Rest(v95 % 6 + 60);
                _506F18_num_minutes_to_sleep = 0;
                dword_506F14 = 0;
                /*if ( (signed int)pMessageQueue_50CBD0->uNumMessages < 40 )
                {
                  pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].eType = UIMSG_Escape;
                  pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].param = 0;
                  *(&pMessageQueue_50CBD0->uNumMessages + 3 * pMessageQueue_50CBD0->uNumMessages + 3) = 0;
                  ++pMessageQueue_50CBD0->uNumMessages;
                }*/
                pMessageQueue_50CBD0->AddGUIMessage(UIMSG_Escape, 0, 0);
                ShowStatusBarString(pGlobalTXT_LocalizationStrings[481], 2);// "Encounter!"
                pAudioPlayer->PlaySound((SoundID)227, 0, 0, -1, 0, 0, 0, 0);
                continue;
              }
            }
            Party::TakeFood(uRestUI_FoodRequiredToRest);
            _506F18_num_minutes_to_sleep = 480;
            dword_506F14 = 2;
            pParty->RestAndHeal();
            pParty->days_played_without_rest = 0;
            pParty->pPlayers[3].SetAsleep(1);
            pParty->pPlayers[2].SetAsleep(1);
            pParty->pPlayers[1].SetAsleep(1);
            pParty->pPlayers[0].SetAsleep(1);
          }
          continue;
        case UIMSG_AlreadyResting:
          if ( dword_506F14 == 2 )
          {
            ShowStatusBarString(pGlobalTXT_LocalizationStrings[477], 2);// "You are already resting!"
            pAudioPlayer->PlaySound((SoundID)27, 0, 0, -1, 0, 0, 0, 0);
            continue;
          }
          GUIWindow::Create(pButton_RestUI_WaitUntilDawn->uX, pButton_RestUI_WaitUntilDawn->uY, 0, 0, WINDOW_PressedButton2,
            (int)pButton_RestUI_WaitUntilDawn, pGlobalTXT_LocalizationStrings[237]);// "Wait until Dawn"
          v97 = _494820_training_time(pParty->uCurrentHour);
          dword_506F14 = 1;
          _506F18_num_minutes_to_sleep = 60 * v97 - pParty->uCurrentMinute;
          continue;
        case UIMSG_HintSelectRemoveQuickSpellBtn:
          if ( quick_spell_at_page && byte_506550 )
          {
            v173 = pSpellStats->pInfos[quick_spell_at_page + 11 * pPlayers[uActiveCharacter]->lastOpenedSpellbookPage].pName;
            sprintfex(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[483], v173);
          }
          else
          {
            if ( pPlayers[uActiveCharacter]->uQuickSpell )
              v177 = pGlobalTXT_LocalizationStrings[584];// "Click here to remove your Quick Spell"
            else
              v177 = pGlobalTXT_LocalizationStrings[484];// "Select a spell then click here to set a QuickSpell"
            strcpy(pTmpBuf.data(), v177);
          }
          GameUI_SetFooterString(pTmpBuf.data());
          continue;
        case UIMSG_SPellbook_ShowHightlightedSpellInfo:
          if ( !uActiveCharacter || (uNumSeconds = (unsigned int)pPlayers[uActiveCharacter],
                !*(char *)(uNumSeconds + 11 * *(char *)(uNumSeconds + 6734) + uMessageParam + 402)) )
            continue;
          if ( sub_4637E0_is_there_popup_onscreen() )
            dword_507B00_spell_info_to_draw_in_popup = uMessageParam + 1;
          v98 = *(char *)(uNumSeconds + 6734);
          if ( quick_spell_at_page - 1 == uMessageParam )
          {
            v178 = pSpellStats->pInfos[uMessageParam + 11 * v98 + 1].pName;
            v161 = pGlobalTXT_LocalizationStrings[485];
          }
          else
          {
            v178 = pSpellStats->pInfos[uMessageParam + 11 * v98 + 1].pName;
            v161 = pGlobalTXT_LocalizationStrings[486];
          }
          sprintfex(pTmpBuf.data(), v161, v178);
          GameUI_SetFooterString(pTmpBuf.data());
          continue;
        case UIMSG_ClickInstallRemoveQuickSpellBtn:
          GUIWindow::Create(pBtn_InstallRemoveSpell->uX, pBtn_InstallRemoveSpell->uY, 0, 0, WINDOW_PressedButton2, (int)pBtn_InstallRemoveSpell, 0);
          if ( !uActiveCharacter )
            continue;
          pPlayer10 = pPlayers[uActiveCharacter];
          if ( !byte_506550 || !quick_spell_at_page )
          {
            pPlayer10->uQuickSpell = 0;
            quick_spell_at_page = 0;
            pAudioPlayer->PlaySound((SoundID)203, 0, 0, -1, 0, 0, 0, 0);
            continue;
          }
          pPlayers[uActiveCharacter]->uQuickSpell = quick_spell_at_page + 11 * pPlayers[uActiveCharacter]->lastOpenedSpellbookPage;
          AA1058_PartyQuickSpellSound[uActiveCharacter - 1].AddPartySpellSound(pPlayers[uActiveCharacter]->uQuickSpell, uActiveCharacter);
          if ( uActiveCharacter )
            pPlayer10->PlaySound(SPEECH_12, 0);
          byte_506550 = 0;
          continue;
        case UIMSG_SpellBook_PressTab://перелистывание страниц клавишей Tab
        {
          if ( !uActiveCharacter )
            continue;
          int skill_count = 0;
          uAction = 0;
          for ( uint i = 0; i < 9; i++ )
          {
            if ( pPlayers[uActiveCharacter]->pActiveSkills[PLAYER_SKILL_FIRE + i] )
            {
              if ( pPlayers[uActiveCharacter]->lastOpenedSpellbookPage == i )
                uAction = skill_count;
              v217[skill_count++] = i;
            }
          }
          if ( !skill_count )//нет скиллов
            pAudioPlayer->PlaySound((SoundID)(rand() % 2 + 204), 0, 0, -1, 0, 0, 0, 0);
          else
          {
            if ( GetAsyncKeyState(16) )
            {
              --uAction;
              if ( uAction < 0 )
                uAction = skill_count - 1;
            }
            else
            {
              ++uAction;
              if ( uAction >= skill_count )
                uAction = 0;
            }
            OnCloseSpellBookPage();
            pPlayers[uActiveCharacter]->lastOpenedSpellbookPage = LOBYTE(v217[uAction]);
            pGUIWindow_CurrentMenu->OpenSpellBook();
            pAudioPlayer->PlaySound((SoundID)(rand() % 2 + 204), 0, 0, -1, 0, 0, 0, 0);
          }
          continue;
        }
        case UIMSG_OpenSpellbookPage:
          if ( pTurnEngine->turn_stage == TE_MOVEMENT || !uActiveCharacter || uMessageParam == pPlayers[uActiveCharacter]->lastOpenedSpellbookPage )
            continue;
          OnCloseSpellBookPage();
          pPlayers[uActiveCharacter]->lastOpenedSpellbookPage = uMessageParam;
          pGUIWindow_CurrentMenu->OpenSpellBook();
          pAudioPlayer->PlaySound((SoundID)(rand() % 2 + 204), 0, 0, -1, 0, 0, 0, 0);
          continue;
        case UIMSG_SelectSpell:
        {
          if (pTurnEngine->turn_stage == TE_MOVEMENT)
            continue;
          if (!uActiveCharacter)
            continue;

            //  uNumSeconds = (unsigned int)pPlayers[uActiveCharacter];
          Player* player = pPlayers[uActiveCharacter];
          if (player->spellbook.pChapters[player->lastOpenedSpellbookPage].bIsSpellAvailable[uMessageParam])
              //if ( *(char *)(uNumSeconds + 11 * *(char *)(uNumSeconds + &lastOpenedSpellbookPage) + uMessageParam + 402) )
          {
                if ( quick_spell_at_page - 1 == uMessageParam )
                {
                  pGUIWindow_CurrentMenu->Release();
                  pEventTimer->Resume();
                  viewparams->bRedrawGameUI = 1;
                  pCurrentScreen = SCREEN_GAME;
                  pIcons_LOD->RemoveTexturesPackFromTextureList();
                  v103 = quick_spell_at_page + 11 * player->lastOpenedSpellbookPage;
                  /*if ( dword_50C9E8 < 40 )
                  {
                    dword_50C9EC[3 * dword_50C9E8] = UIMSG_CastSpellFromBook;
                    dword_50C9EC[3 * dword_50C9E8 + 1] = v103;
                    dword_50C9EC[3 * dword_50C9E8 + 2] = uActiveCharacter - 1;
                    ++dword_50C9E8;
                  }*/
                  pMessageQueue_50C9E8->AddGUIMessage(UIMSG_CastSpellFromBook, v103, uActiveCharacter - 1);
                //  pMessageQueue_50CBD0->AddGUIMessage(UIMSG_CastSpellFromBook, v103, uActiveCharacter - 1);
                }
                else
                {
                  byte_506550 = 1;
                  quick_spell_at_page = uMessageParam + 1;
                }
          }
        }
        continue;

        case UIMSG_CastSpellFromBook:
          if ( pTurnEngine->turn_stage != TE_MOVEMENT )
            _42777D_CastSpell_UseWand_ShootArrow(uMessageParam, v199, 0, 0, 0);
        continue;

        case UIMSG_SpellScrollUse:
			__debugbreak();
          if ( pTurnEngine->turn_stage != TE_MOVEMENT )
            _42777D_CastSpell_UseWand_ShootArrow(uMessageParam, v199, 133, 1, 0);
          continue;
        case UIMSG_SpellBookWindow:
          if ( pTurnEngine->turn_stage == TE_MOVEMENT )
            continue;
          if ( bUnderwater == true )
          {
            ShowStatusBarString(pGlobalTXT_LocalizationStrings[652], 2);// "You can not do that while you are underwater!"
            pAudioPlayer->PlaySound((SoundID)27, 0, 0, -1, 0, 0, 0, 0);
          }
          else
          {
            if ( pMessageQueue_50CBD0->uNumMessages )
              pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
            if ( uActiveCharacter && !pPlayers[uActiveCharacter]->uTimeToRecovery )
            {
              if ( pCurrentScreen == SCREEN_GAME )
              {
                GUIWindow::Create(476, 450, 0, 0, WINDOW_PressedButton2, (int)pBtn_CastSpell, 0);
                pCurrentScreen = SCREEN_SPELL_BOOK;
                pEventTimer->Pause();
                pGUIWindow_CurrentMenu = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_SpellBook, 0, 0);
                pAudioPlayer->PlaySound((SoundID)48, 0, 0, -1, 0, 0, 0, 0);
                viewparams->field_48 = 1;
                continue;
              }
              if ( pCurrentScreen != SCREEN_REST && pCurrentScreen != SCREEN_CHARACTERS
                && (pCurrentScreen <= SCREEN_63 || pCurrentScreen > SCREEN_67) )
              {
                pGUIWindow_CurrentMenu->Release();
                GUIWindow::Create(476, 450, 0, 0, WINDOW_PressedButton2, (int)pBtn_CastSpell, 0);
                pCurrentScreen = SCREEN_SPELL_BOOK;
                pEventTimer->Pause();
                pGUIWindow_CurrentMenu = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_SpellBook, 0, 0);
                pAudioPlayer->PlaySound((SoundID)48, 0, 0, -1, 0, 0, 0, 0);
                viewparams->field_48 = 1;
                continue;
              }
            }
          }
          continue;
        case UIMSG_QuickReference:
          if ( pMessageQueue_50CBD0->uNumMessages )
            pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
          if ( pCurrentScreen )
            pGUIWindow_CurrentMenu->Release();
          ++pIcons_LOD->uTexturePacksCount;
          if ( !pIcons_LOD->uNumPrevLoadedFiles )
            pIcons_LOD->uNumPrevLoadedFiles = pIcons_LOD->uNumLoadedFiles;
          GUIWindow::Create(0x230u, 0x1C2u, 0, 0, WINDOW_PressedButton2, (int)pBtn_QuickReference, 0);
          viewparams->bRedrawGameUI = 1;
          pEventTimer->Pause();
          pAudioPlayer->StopChannels(-1, -1);
          pCurrentScreen = SCREEN_QUICK_REFERENCE;
          pGUIWindow_CurrentMenu = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_QuickReference, 5, 0);
          papredoll_dbrds[2] = pIcons_LOD->LoadTexture("BUTTEXI1", TEXTURE_16BIT_PALETTE);
          pBtn_ExitCancel = pGUIWindow_CurrentMenu->CreateButton(0x187u, 0x13Cu, 0x4Bu, 0x21u, 1, 0, UIMSG_Escape, 0, 0,
                         pGlobalTXT_LocalizationStrings[79],// "Exit"
                         pIcons_LOD->GetTexture(uTextureID_BUTTDESC2), 0); //, v179);
          continue;
        case UIMSG_GameMenuButton:
          if ( pCurrentScreen )
          {
            pGUIWindow_CurrentMenu->Release();
            pEventTimer->Resume();
            pCurrentScreen = SCREEN_GAME;
            viewparams->bRedrawGameUI = 1;
          }

          stru_506E40.Release();
          pRenderer->SaveScreenshot("gamma.pcx", 155, 117);
          stru_506E40.LoadPCXFile("gamma.pcx", 0);

          GUIWindow::Create(0x25Au, 0x1C2u, 0, 0, WINDOW_PressedButton, (int)pBtn_GameSettings, 0);
//LABEL_453:
          /*if ( (signed int)pMessageQueue_50CBD0->uNumMessages >= 40 )
            continue;
          pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].eType = UIMSG_Escape;
          //goto LABEL_770;
          pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].param = 0;
          *(&pMessageQueue_50CBD0->uNumMessages + 3 * pMessageQueue_50CBD0->uNumMessages + 3) = 0;
          ++pMessageQueue_50CBD0->uNumMessages;*/
          pMessageQueue_50CBD0->AddGUIMessage(UIMSG_Escape, 0, 0);
          continue;
        case UIMSG_ClickAwardScrollBar:
          books_page_number = 1;
          if ( pMouse->GetCursorPos(&v211)->y > 178 )
            books_page_number = -1;
          continue;
        case UIMSG_ClickAwardsUpBtn:
          GUIWindow::Create(pBtn_Up->uX, pBtn_Up->uY, 0, 0, WINDOW_CharactersPressedButton, (int)pBtn_Up, 0);
          BtnUp_flag = 1;
          continue;
        case UIMSG_ClickAwardsDownBtn:
          GUIWindow::Create(pBtn_Down->uX, pBtn_Down->uY, 0, 0, WINDOW_CharactersPressedButton, (int)pBtn_Down, 0);
          BtnDown_flag = 1;
          continue;
        case UIMSG_ChangeDetaliz:
          bRingsShownInCharScreen ^= 1;
          pCharacterScreen_DetalizBtn->Release();
          pCharacterScreen_DollBtn->Release();
          if ( bRingsShownInCharScreen )
          {
            v128 = pIcons_LOD->GetTexture(uTextureID_detaliz_close_button)->uTextureHeight;
            v125 = pIcons_LOD->GetTexture(uTextureID_detaliz_close_button)->uTextureWidth;
            v123 = 445;
            v121 = 470;
          }
          else
          {
            v128 = 30;
            v125 = 30;
            v123 = 300;
            v121 = 600;
          }
          pCharacterScreen_DetalizBtn = pGUIWindow_CurrentMenu->CreateButton(v121, v123, v125, v128, 1, 0, UIMSG_ChangeDetaliz, 0, 0,
                         pGlobalTXT_LocalizationStrings[64],// "Detail Toggle"
                         0);
          pCharacterScreen_DollBtn = pGUIWindow_CurrentMenu->CreateButton(0x1DCu, 0, 0xA4u, 0x159u, 1, 0, UIMSG_ClickPaperdoll, 0, 0, "", 0);
          viewparams->bRedrawGameUI = 1;
          continue;
        case UIMSG_ClickPaperdoll:
          OnPaperdollLeftClick();
          continue;
        case UIMSG_ClickStatsBtn:
          pWindowList_at_506F50_minus1_indexing_buttons____and_an_int_[0] = 100;
          CharacterUI_ReleaseButtons();
          ReleaseAwardsScrollBar();
          GUIWindow::Create(pCharacterScreen_StatsBtn->uX, pCharacterScreen_StatsBtn->uY, 0, 0, WINDOW_CharactersPressedButton, (int)pCharacterScreen_StatsBtn, 0);
          continue;
        case UIMSG_ClickSkillsBtn:
          pWindowList_at_506F50_minus1_indexing_buttons____and_an_int_[0] = 101;
          CharacterUI_ReleaseButtons();
          ReleaseAwardsScrollBar();
          CharacterUI_SkillsTab_CreateButtons();
          GUIWindow::Create(pCharacterScreen_SkillsBtn->uX, pCharacterScreen_SkillsBtn->uY, 0, 0, WINDOW_CharactersPressedButton, (int)pCharacterScreen_SkillsBtn, 0);
          continue;
        case UIMSG_SkillUp:
          pPlayer4 = pPlayers[uActiveCharacter];
          v105 = (int)&pPlayer4->pActiveSkills[uMessageParam];
          LOWORD(v2) = *(short *)v105;
          uNumSeconds = v2;
          if ( pPlayer4->uSkillPoints < (v2 & 0x3F) + 1 )
          {
            v87 = pGlobalTXT_LocalizationStrings[488];// "You don't have enough skill points!"
          }
          else
          {
            if ( (uNumSeconds & 0x3F) < 0x3C )
            {
              *(short *)v105 = uNumSeconds + 1;
              pPlayer4->uSkillPoints -= pPlayer4->pActiveSkills[uMessageParam] & 0x3F;
              pPlayer4->PlaySound(SPEECH_14, 0);
              pAudioPlayer->PlaySound((SoundID)20001, 0, 0, -1, 0, 0, 0, 0);
              continue;
            }
            v87 = pGlobalTXT_LocalizationStrings[487];// "You have already mastered this skill!"
          }
          ShowStatusBarString(v87, 2);
          continue;
        case UIMSG_ClickInventoryBtn:
          pWindowList_at_506F50_minus1_indexing_buttons____and_an_int_[0] = 103;
          ReleaseAwardsScrollBar();
          CharacterUI_ReleaseButtons();
          GUIWindow::Create(pCharacterScreen_InventoryBtn->uX, pCharacterScreen_InventoryBtn->uY, 0, 0, WINDOW_CharactersPressedButton, (int)pCharacterScreen_InventoryBtn, 0);
          continue;
        case UIMSG_ClickExitCharacterWindowBtn:
          GUIWindow::Create(pCharacterScreen_ExitBtn->uX, pCharacterScreen_ExitBtn->uY, 0, 0, WINDOW_ExitCharacterWindow, (int)pCharacterScreen_ExitBtn, 0);
          continue;
        case UIMSG_ClickAwardsBtn:
          ReleaseAwardsScrollBar();
          CharacterUI_ReleaseButtons();
          CreateAwardsScrollBar();
          pWindowList_at_506F50_minus1_indexing_buttons____and_an_int_[0] = 102;
          GUIWindow::Create(pCharacterScreen_AwardsBtn->uX, pCharacterScreen_AwardsBtn->uY, 0, 0, WINDOW_CharactersPressedButton, (int)pCharacterScreen_AwardsBtn, 0);
          FillAwardsData();
          continue;
        case UIMSG_ClickBooksBtn:
          switch ( uMessageParam )
          {
            case 11://Page UP
              BtnUp_flag = 1;
              pButton = pBtn_Book_2;
              break;
            case 10://Page DOWN
              BtnDown_flag = 1;
              pButton = pBtn_Book_1;
              break;
            case 0://Zoom plus
              pButton = pBtn_Book_1;
              BtnDown_flag = 1;
              break;
            case 1://Zoom minus
              pButton = pBtn_Book_2;
              BtnUp_flag = 1;
              break;
            case 2://Potions
              Book_PageBtn3_flag = 1;
              if ( dword_506364 )
                continue;
              pButton = pBtn_Book_3;
              break;
            case 3://fountains
              Book_PageBtn4_flag = 1;
              if ( dword_506364 )
                continue;
              pButton = pBtn_Book_4;
              break;
            case 4://obelisks
              Book_PageBtn5_flag = 1;//Autonotes_Obelisks_page_flag
              if ( dword_506364 )
                continue;
              pButton = pBtn_Book_5;
              break;
            case 5://seer
              Book_PageBtn6_flag = 1;//Autonotes_Seer_page_flag
              if ( dword_506364 )
                continue;
              pButton = pBtn_Book_6;
              break;
            case 6://misc
              pButton = pBtn_Autonotes_Misc;
              Autonotes_Misc_page_flag = 1;
              break;
            case 7://Instructors
              pButton = pBtn_Autonotes_Instructors;
              Autonotes_Instructors_page_flag = 1;
              break;
            default:
              continue;
          }
          GUIWindow::Create(pButton->uX, pButton->uY, 0, 0, WINDOW_PressedButton, (int)pButton, (char *)1);
          continue;
        case UIMSG_SelectCharacter:
          if ( pMessageQueue_50CBD0->uNumMessages )
            pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
          GameUI_OnPlayerPortraitLeftClick(uMessageParam);
          continue;
        case UIMSG_ShowStatus_Funds:
          v174 = (char *)pParty->uNumGoldInBank;
          //v158 = pParty->uNumGold + pParty->uNumGoldInBank;
          sprintfex(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[489], pParty->uNumGold + pParty->uNumGoldInBank, v174);// "You have %d total gold, %d in the Bank"
          GameUI_SetFooterString(pTmpBuf.data());
          continue;
        case UIMSG_ShowStatus_DateTime:
          currHour = pParty->uCurrentHour;
          uNumSeconds = 1;
          if (pParty->uCurrentHour > 12 )
          {
            if (pParty->uCurrentHour >= 24)
              uNumSeconds = 0;
            currHour = (currHour - 12);
          }
          else 
          {
		  	if (pParty->uCurrentHour < 12) // 12:00 is PM
	            uNumSeconds = 0;
            if (pParty->uCurrentHour == 0)
              currHour = 12;
          }
          sprintf(pTmpBuf.data(), "%d:%02d%s %s %d %s %d", currHour, pParty->uCurrentMinute, aAMPMNames[uNumSeconds], aDayNames[pParty->uDaysPlayed % 7],
            7 * pParty->uCurrentMonthWeek + pParty->uDaysPlayed % 7 + 1, aMonthNames[pParty->uCurrentMonth], pParty->uCurrentYear);
          GameUI_SetFooterString(pTmpBuf.data());
          continue;
        case UIMSG_ShowStatus_Food:
          sprintfex(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[501], pParty->uNumFoodRations); // "You have %lu food"
          GameUI_SetFooterString(pTmpBuf.data());
          continue;
        case UIMSG_ShowStatus_Player:
          pPlayer5 = pPlayers[uMessageParam];
          sprintfex(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[429], pPlayer5->pName, pClassNames[pPlayer5->classType]);// "%s the %s"
          strcat(pTmpBuf.data(), ": ");
          v107 = pPlayer5->GetMajorConditionIdx();
          strcat(pTmpBuf.data(), aCharacterConditionNames[v107]);
          GameUI_SetFooterString(pTmpBuf.data());
          v108 = 8 * uMessageParam - 8;
          LOBYTE(v108) = v108 | 4;
          pMouse->uPointingObjectID = PID(OBJECT_Player,v108);
          continue;
        case UIMSG_ShowStatus_ManaHP:
          sprintf(pTmpBuf.data(), "%d / %d %s    %d / %d %s", pPlayers[uMessageParam]->sHealth, pPlayers[uMessageParam]->GetMaxHealth(), 
                  pGlobalTXT_LocalizationStrings[108], pPlayers[uMessageParam]->sMana, pPlayers[uMessageParam]->GetMaxMana(), 
                  pGlobalTXT_LocalizationStrings[212]);
          GameUI_SetFooterString(pTmpBuf.data());
          continue;
        case UIMSG_CHEST_ClickItem:
          if ( pCurrentScreen == SCREEN_CHEST_INVENTORY )
          {
            pPlayers[uActiveCharacter]->OnInventoryLeftClick();
            continue;
          }
          Chest::OnChestLeftClick();
          continue;
        case UIMSG_InventoryLeftClick:
          pPlayers[uActiveCharacter]->OnInventoryLeftClick();
          continue;
        case UIMSG_MouseLeftClickInGame:
          /*if ( !pRenderer->pRenderD3D )
          {
            if ( pMessageQueue_50CBD0->uNumMessages )
            pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
            OnGameViewportClick();
            continue;
          }*/
          v115 = pMessageQueue_50CBD0->uNumMessages;
          if ( !pMessageQueue_50CBD0->uNumMessages )
          {
            pMessageQueue_50CBD0->AddGUIMessage(UIMSG_MouseLeftClickInScreen, 0, 0);
            /*if ( (signed int)v115 < 40 )
            //goto LABEL_769;
            {
              pMessageQueue_50CBD0->pMessages[v115].eType = UIMSG_MouseLeftClickInScreen;
              pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].param = 0;
              *(&pMessageQueue_50CBD0->uNumMessages + 3 * pMessageQueue_50CBD0->uNumMessages + 3) = 0;
              ++pMessageQueue_50CBD0->uNumMessages;
              continue;
            }*/
            continue;
          }
          if ( pMessageQueue_50CBD0->pMessages[0].field_8 )
          {
            pMessageQueue_50CBD0->uNumMessages = 1;
            pMessageQueue_50CBD0->AddGUIMessage(UIMSG_MouseLeftClickInScreen, 0, 0);
            /*v115 = v0;
            pMessageQueue_50CBD0->uNumMessages = v0;
            pMessageQueue_50CBD0->pMessages[v115].eType = UIMSG_MouseLeftClickInScreen;
            pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].param = 0;
            //goto LABEL_771;
            *(&pMessageQueue_50CBD0->uNumMessages + 3 * pMessageQueue_50CBD0->uNumMessages + 3) = 0;
            ++pMessageQueue_50CBD0->uNumMessages;*/
            continue;
          }
          v115 = 0;
          pMessageQueue_50CBD0->uNumMessages = 0;
          pMessageQueue_50CBD0->AddGUIMessage(UIMSG_MouseLeftClickInScreen, 0, 0);
          /*if ( (signed int)v115 < 40 )
            //goto LABEL_769;
          {
            pMessageQueue_50CBD0->pMessages[v115].eType = UIMSG_MouseLeftClickInScreen;
            pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].param = 0;
            *(&pMessageQueue_50CBD0->uNumMessages + 3 * pMessageQueue_50CBD0->uNumMessages + 3) = 0;
            ++pMessageQueue_50CBD0->uNumMessages;
            continue;
          }*/
          continue;
        case UIMSG_MouseLeftClickInScreen://срабатывает при нажатии на правую кнопку мыши после UIMSG_MouseLeftClickInGame
          if ( pMessageQueue_50CBD0->uNumMessages )
            pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
          OnGameViewportClick();
          continue;
        case UIMSG_F:
			__debugbreak();
          //if ( pRenderer->pRenderD3D )
          {
            LOWORD(v116) = pGame->pVisInstance->get_picked_object_zbuf_val();
          }
          /*else
          {
            uNumSeconds = (unsigned int)pMouse->GetCursorPos(&v209);
            pPoint3 = pMouse->GetCursorPos(&v204);
            v116 = pRenderer->pActiveZBuffer[*(int *)uNumSeconds + pSRZBufferLineOffsets[pPoint3->y]];
          }*/
          pButton2 = (GUIButton *)(unsigned __int16)v116;
          GUIWindow::Create(0, 0, 0, 0, WINDOW_F, (int)pButton2, 0);
          continue;
        case UIMSG_54:
			__debugbreak();
          pButton2 = (GUIButton *)uMessageParam;
          GUIWindow::Create(0, 0, 0, 0, WINDOW_22, (int)pButton2, 0);
          continue;
        case UIMSG_Game_Action:
          if ( pMessageQueue_50CBD0->uNumMessages )
            pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
          OnPressSpace();
          continue;
        case UIMSG_ClickZoomOutBtn:
          if ( pCurrentScreen )
            continue;
          pParty->uFlags |= 2u;
          GUIWindow::Create(519, 136, 0, 0, WINDOW_PressedButton2, (int)pBtn_ZoomOut, 0);
          uNumSeconds = 131072;
          v118 = 2 * viewparams->uMinimapZoom;
          ++viewparams->field_28;
          viewparams->uMinimapZoom *= 2;
          if ( uCurrentlyLoadedLevelType != LEVEL_Outdoor)
          {
            if ( (signed int)v118 > 4096 )
            {
              viewparams->uMinimapZoom = 4096;
              viewparams->field_28 = 12;
            }
            continue;
          }
          v119 = 2048;
          if ( (signed int)v118 <= 2048 )
          {
            _576E2C_current_minimap_zoom = v118;
            dword_576E28 = viewparams->field_28;
            break;
          }
          viewparams->field_28 = 11;
          viewparams->uMinimapZoom = v119;
          _576E2C_current_minimap_zoom = v119;
          dword_576E28 = viewparams->field_28;
          break;
        case UIMSG_ClickZoomInBtn:
          if ( pCurrentScreen )
            continue;
          pParty->uFlags |= 2u;
          GUIWindow::Create(574, 136, 0, 0, WINDOW_PressedButton2, (int)pBtn_ZoomIn, 0);
          uNumSeconds = 32768;
          v118 = (unsigned __int64)((signed __int64)(signed int)viewparams->uMinimapZoom << 15) >> 16;
          --viewparams->field_28;
          viewparams->uMinimapZoom = (unsigned __int64)((signed __int64)(signed int)viewparams->uMinimapZoom << 15) >> 16;
          if ( uCurrentlyLoadedLevelType == LEVEL_Outdoor)
          {
            v119 = 512;
            if ( (signed int)v118 < 512 )
            {
              viewparams->field_28 = 9;
              v118 = v119;
              viewparams->uMinimapZoom = v119;
            }
            _576E2C_current_minimap_zoom = v118;
            dword_576E28 = viewparams->field_28;
          }
          else
          {
            if ( (signed int)v118 < 256 )
            {
              viewparams->uMinimapZoom = 256;
              viewparams->field_28 = 8;
            }
          }
        default:
          continue;
      }
    }
  }
  pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50C9E8->uNumMessages;//dword_50C9E8;
  memcpy(pMessageQueue_50CBD0->pMessages, pMessageQueue_50C9E8->pMessages, sizeof(GUIMessage) * pMessageQueue_50C9E8->uNumMessages);
  //memcpy(pMessageQueue_50CBD0->pMessages, dword_50C9EC, 12 * dword_50C9E8);
  //dword_50C9E8 = 0;


   pMessageQueue_50C9E8->uNumMessages=0;
  if ( dword_50C9DC )
  {
    /*if ( (signed int)pMessageQueue_50CBD0->uNumMessages < 40 )
    {
      pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].eType = (UIMessageType)dword_50C9DC;
      pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].param = (int)ptr_50C9E0;
      *(&pMessageQueue_50CBD0->uNumMessages + 3 * pMessageQueue_50CBD0->uNumMessages + 3) = 0;
      ++pMessageQueue_50CBD0->uNumMessages;
    }*/
    pMessageQueue_50CBD0->AddGUIMessage((UIMessageType)dword_50C9DC, (int)ptr_50C9E0, 0);
    dword_50C9DC = 0;
  }
  else
  {
    if ( _50C9D0_AfterEnchClickEventId > 0 )
    {
      _50C9D8_AfterEnchClickEventTimeout -= pEventTimer->uTimeElapsed;
      if ( _50C9D8_AfterEnchClickEventTimeout <= 0 )
      {
        /*if ( (signed int)pMessageQueue_50CBD0->uNumMessages < 40 )
        {
          pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].eType = (UIMessageType)dword_50C9D0;
          pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].param = dword_50C9D4;
          *(&pMessageQueue_50CBD0->uNumMessages + 3 * pMessageQueue_50CBD0->uNumMessages + 3) = 0;
          ++pMessageQueue_50CBD0->uNumMessages;
        }*/
        pMessageQueue_50CBD0->AddGUIMessage((UIMessageType)_50C9D0_AfterEnchClickEventId, _50C9D4_AfterEnchClickEventSecondParam, 0);
        _50C9D0_AfterEnchClickEventId = 0;
        _50C9D4_AfterEnchClickEventSecondParam = 0;
        _50C9D8_AfterEnchClickEventTimeout = 0;
      }
    }
  }
  CastSpellInfoHelpers::_427E01_cast_spell();
}
//----- (00435748) --------------------------------------------------------
void  GUI_MainMenuMessageProc()
{
  Player *pPlayer; // ebx@2
  void *v3; // edi@21
  signed int v4; // eax@29
//  int v5; // ecx@29
//  PLAYER_SKILL_TYPE v6; // edi@37
  GUIWindow *pWindow; // eax@56
  GUIButton *pButton; // eax@59
  int v15; // edi@70
  char v20; // dl@116
  unsigned int v21; // eax@116
  unsigned int v25; // eax@120
  unsigned int v26; // ecx@127
//  SoundID pSoundID; // [sp-2Ch] [bp-3Ch]@36
//  signed int v41; // [sp-10h] [bp-20h]@29
  int pParam; // [sp+4h] [bp-Ch]@3
  UIMessageType pUIMessageType; // [sp+8h] [bp-8h]@3
  int pSex; // [sp+Ch] [bp-4h]@3

  if ( pMessageQueue_50CBD0->uNumMessages )
  {
    pPlayer = pParty->pPlayers.data();
    do
    {
      int param2;
      pMessageQueue_50CBD0->PopMessage(&pUIMessageType, &pParam, &param2);
      //auto player = &pParty->pPlayers[pParam];

      switch (pUIMessageType) // For buttons of window MainMenu
      {
        case UIMSG_MainMenu_ShowPartyCreationWnd:
          GUIWindow::Create(495, 172, 0, 0, WINDOW_PressedButton2, (int)pMainMenu_BtnNew, 0);
          SetCurrentMenuID(MENU_NEWGAME);
          break;
        case UIMSG_MainMenu_ShowLoadWindow:
          GUIWindow::Create(495, 227, 0, 0, WINDOW_PressedButton2, (int)pMainMenu_BtnLoad, 0);
          SetCurrentMenuID(MENU_SAVELOAD);
          break;
        case UIMSG_ShowCredits:
          GUIWindow::Create(495, 282, 0, 0, WINDOW_PressedButton2, (int)pMainMenu_BtnCredits, 0);
          SetCurrentMenuID(MENU_CREDITS);
          break;
        case UIMSG_ExitToWindows:
          GUIWindow::Create(495, 337, 0, 0, WINDOW_PressedButton2, (int)pMainMenu_BtnExit, 0);
          SetCurrentMenuID(MENU_EXIT_GAME);
          break;
        case UIMSG_PlayerCreation_SelectAttribute:
          pGUIWindow_CurrentMenu->pCurrentPosActiveItem = (pGUIWindow_CurrentMenu->pCurrentPosActiveItem - pGUIWindow_CurrentMenu->pStartingPosActiveItem)
                                                    % 7 + pGUIWindow_CurrentMenu->pStartingPosActiveItem + 7 * pParam;
          uPlayerCreationUI_SelectedCharacter = pParam;
          pAudioPlayer->PlaySound(SOUND_Button, 0, 0, -1, 0, 0, 0, 0);
          break;
        case UIMSG_PlayerCreation_VoicePrev:
          pSex = pParty->pPlayers[pParam].GetSexByVoice();
          do
          {
            if (pParty->pPlayers[pParam].uVoiceID == 0)
              pParty->pPlayers[pParam].uVoiceID = 19;
            else --pParty->pPlayers[pParam].uVoiceID;
          }
          while (pParty->pPlayers[pParam].GetSexByVoice() != pSex);
          pButton = pCreationUI_BtnPressLeft2[pParam];
          GUIWindow::Create(pButton->uX, pButton->uY, 0, 0, WINDOW_PressedButton, (int)pButton, (char *)1);
          pAudioPlayer->PlaySound(SOUND_Button, 0, 0, -1, 0, 0, 0, 0);
          pParty->pPlayers[pParam].PlaySound(SPEECH_PickMe, 0);
          break;
        case UIMSG_PlayerCreation_VoiceNext:
          pSex = pParty->pPlayers[pParam].GetSexByVoice();
          do
          {
            pParty->pPlayers[pParam].uVoiceID = (pParty->pPlayers[pParam].uVoiceID + 1) % 20;
          }
          while (pParty->pPlayers[pParam].GetSexByVoice() != pSex);
          pButton = pCreationUI_BtnPressRight2[pParam];
          GUIWindow::Create(pButton->uX, pButton->uY, 0, 0, WINDOW_PressedButton, (int)pButton, (char *)1);
          pAudioPlayer->PlaySound(SOUND_Button, 0, 0, -1, 0, 0, 0, 0);
          pParty->pPlayers[pParam].PlaySound(SPEECH_PickMe, 0);
          break;
        case UIMSG_PlayerCreation_FacePrev:
          //pPlayer = &pParty->pPlayers[pParam];
          if (!pParty->pPlayers[pParam].uCurrentFace)
            pParty->pPlayers[pParam].uCurrentFace = 19;
          else
            pParty->pPlayers[pParam].uCurrentFace -= 1;
          pParty->pPlayers[pParam].uVoiceID = pParty->pPlayers[pParam].uCurrentFace;
          pParty->pPlayers[pParam].SetInitialStats();
          pParty->pPlayers[pParam].SetSexByVoice();
          pParty->pPlayers[pParam].RandomizeName();
          v25 = pParam;
          pGUIWindow_CurrentMenu->pCurrentPosActiveItem = (pGUIWindow_CurrentMenu->pCurrentPosActiveItem - pGUIWindow_CurrentMenu->pStartingPosActiveItem)
                                                    % 7 + pGUIWindow_CurrentMenu->pStartingPosActiveItem + 7 * pParam;
          uPlayerCreationUI_SelectedCharacter = v25;
          GUIWindow::Create(pCreationUI_BtnPressLeft[v25]->uX, pCreationUI_BtnPressLeft[v25]->uY, 0, 0, WINDOW_PressedButton, (int)pCreationUI_BtnPressLeft[v25], (char *)1);
          pAudioPlayer->PlaySound(SOUND_Button, 0, 0, -1, 0, 0, 0.0, 0);
          pParty->pPlayers[pParam].PlaySound(SPEECH_PickMe, 0);
          break;
        case UIMSG_PlayerCreation_FaceNext:
          //pPlayer = &pParty->pPlayers[pParam];
          v20 = (char)((int)pParty->pPlayers[pParam].uCurrentFace + 1) % 20;
          pParty->pPlayers[pParam].uCurrentFace = v20;
          pParty->pPlayers[pParam].uVoiceID = v20;
          pParty->pPlayers[pParam].SetInitialStats();
          pParty->pPlayers[pParam].SetSexByVoice();
          pParty->pPlayers[pParam].RandomizeName();
          v21 = pParam;
          pGUIWindow_CurrentMenu->pCurrentPosActiveItem = (pGUIWindow_CurrentMenu->pCurrentPosActiveItem - pGUIWindow_CurrentMenu->pStartingPosActiveItem)
                                                    % 7 + pGUIWindow_CurrentMenu->pStartingPosActiveItem + 7 * pParam;
          uPlayerCreationUI_SelectedCharacter = v21;
          GUIWindow::Create(pCreationUI_BtnPressRight[v21]->uX, pCreationUI_BtnPressRight[v21]->uY, 0, 0, WINDOW_PressedButton, (int)pCreationUI_BtnPressRight[v21], (char *)1);
          pAudioPlayer->PlaySound(SOUND_Button, 0, 0, -1, 0, 0, 0, 0);
          pParty->pPlayers[pParam].PlaySound(SPEECH_PickMe, 0);
          break;
        case UIMSG_PlayerCreationClickPlus:
          GUIWindow::Create(613, 393, 0, 0, WINDOW_PressedButton2, (int)pPlayerCreationUI_BtnPlus, (char *)1);
          pPlayer[uPlayerCreationUI_SelectedCharacter].IncreaseAttribute((pGUIWindow_CurrentMenu->pCurrentPosActiveItem - pGUIWindow_CurrentMenu->pStartingPosActiveItem) % 7);
          pAudioPlayer->PlaySound((SoundID)20, 0, 0, -1, 0, 0, 0, 0);
          break;
        case UIMSG_PlayerCreationClickMinus:
          GUIWindow::Create(523, 393, 0, 0, WINDOW_PressedButton2, (int)pPlayerCreationUI_BtnMinus, (char *)1);
          pPlayer[uPlayerCreationUI_SelectedCharacter].DecreaseAttribute((pGUIWindow_CurrentMenu->pCurrentPosActiveItem - pGUIWindow_CurrentMenu->pStartingPosActiveItem) % 7);
          pAudioPlayer->PlaySound((SoundID)23, 0, 0, -1, 0, 0, 0, 0);
          break;
        case UIMSG_PlayerCreationSelectActiveSkill:
          if ( pPlayer[uPlayerCreationUI_SelectedCharacter].GetSkillIdxByOrder(3) == 37 )
            pParty->pPlayers[uPlayerCreationUI_SelectedCharacter].pActiveSkills[pPlayer[uPlayerCreationUI_SelectedCharacter].GetSkillIdxByOrder(pParam + 4)] = 1;
          pAudioPlayer->PlaySound(SOUND_24, 0, 0, -1, 0, 0, 0, 0);
          break;
        case UIMSG_PlayerCreationSelectClass:
          pPlayer[uPlayerCreationUI_SelectedCharacter].Reset((PLAYER_CLASS_TYPE)pParam);
          pAudioPlayer->PlaySound(SOUND_Button, 0, 0, -1, 0, 0, 0, 0);
          break;
        case UIMSG_PlayerCreationClickOK:
          GUIWindow::Create(580, 431, 0, 0, WINDOW_PressedButton2, (int)pPlayerCreationUI_BtnOK, 0);
          if ( PlayerCreation_GetUnspentAttributePointCount() || !PlayerCreation_Choose4Skills() )
            GameUI_Footer_TimeLeft = GetTickCount() + 4000;
          else
            uGameState = GAME_STATE_STARTING_NEW_GAME;
          break;
        case UIMSG_PlayerCreationClickReset:
          GUIWindow::Create(527, 431, 0, 0, WINDOW_PressedButton2, (int)pPlayerCreationUI_BtnReset, 0);
          pParty->Reset();
          break;
        case UIMSG_PlayerCreationRemoveUpSkill:
          v4 = pGUIWindow_CurrentMenu->pCurrentPosActiveItem - pGUIWindow_CurrentMenu->pStartingPosActiveItem;
          pGUIWindow_CurrentMenu->pCurrentPosActiveItem = v4 % 7 + pGUIWindow_CurrentMenu->pStartingPosActiveItem + 7 * pParam;
          if ( pPlayer[pParam].GetSkillIdxByOrder(2) != 37 )//37 - None(Нет)
            pParty->pPlayers[pParam].pActiveSkills[pPlayer[pParam].GetSkillIdxByOrder(2)] = 0;
          break;
        case UIMSG_PlayerCreationRemoveDownSkill:
          v4 = pGUIWindow_CurrentMenu->pCurrentPosActiveItem - pGUIWindow_CurrentMenu->pStartingPosActiveItem;
          pGUIWindow_CurrentMenu->pCurrentPosActiveItem = v4 % 7 + pGUIWindow_CurrentMenu->pStartingPosActiveItem + 7 * pParam;
          if ( pPlayer[pParam].GetSkillIdxByOrder(3) != 37 )//37 - None(Нет)
            pParty->pPlayers[pParam].pActiveSkills[pPlayer[pParam].GetSkillIdxByOrder(3)] = 0;
          break;
        case UIMSG_PlayerCreationChangeName:
          pAudioPlayer->PlaySound((SoundID)24, 0, 0, -1, 0, 0, 0, 0);
          v3 = (void *)pParam;
          uPlayerCreationUI_SelectedCharacter = pParam;
          pKeyActionMap->EnterText(0, 15, pGUIWindow_CurrentMenu);
          pGUIWindow_CurrentMenu->ptr_1C = v3;
          break;
        case UIMSG_ChangeGameState:
          uGameState = GAME_FINISHED;
          break;
        case UIMSG_ChangeCursor:
          pMouse->SetCursorBitmap("MICON2");
          break;
        case UIMSG_3A:
          SetCurrentMenuID(MENU_DebugBLVLevel);
          break;
        case UIMSG_LoadGame:
          if (!pSavegameUsedSlots[uLoadGameUI_SelectedSlot])
            break;
          SetCurrentMenuID(MENU_LoadingProcInMainMenu);
          break;
        case UIMSG_SelectLoadSlot:
            //main menu save/load wnd   clicking on savegame lines
          if (pGUIWindow_CurrentMenu->receives_keyboard_input_2 == WINDOW_INPUT_IN_PROGRESS)
            pKeyActionMap->SetWindowInputStatus(WINDOW_INPUT_NONE);
          if ( pCurrentScreen != SCREEN_SAVEGAME || uLoadGameUI_SelectedSlot != pParam + pSaveListPosition )
          {
            //load clicked line
              v26 = pParam + pSaveListPosition;
              if ( dword_6BE138 == pParam + pSaveListPosition )
              {
                pMessageQueue_50CBD0->AddGUIMessage(UIMSG_SaveLoadBtn, 0, 0);
                pMessageQueue_50CBD0->AddGUIMessage(UIMSG_LoadGame, 0, 0);
              }
              uLoadGameUI_SelectedSlot = v26;
              dword_6BE138 = v26;
          }
          else
          {
            //typing in the line
              pKeyActionMap->EnterText(0, 19, pGUIWindow_CurrentMenu);
              strcpy(pKeyActionMap->pPressedKeysBuffer, pSavegameHeader[uLoadGameUI_SelectedSlot].pName);
              pKeyActionMap->uNumKeysPressed = strlen(pKeyActionMap->pPressedKeysBuffer);
          }
          break;
        case UIMSG_SaveLoadBtn:
          GUIWindow::Create(pGUIWindow_CurrentMenu->uFrameX + 241, pGUIWindow_CurrentMenu->uFrameY + 302, 61, 28, WINDOW_SaveLoadBtn, (int)pBtnLoadSlot, 0);
          break;
        case UIMSG_DownArrow:
          ++pSaveListPosition;
          if ( pSaveListPosition >= pParam )
            pSaveListPosition = pParam - 1;
          if ( pSaveListPosition < 1 )
            pSaveListPosition = 0;
          pWindow = pGUIWindow_CurrentMenu;
          GUIWindow::Create(pWindow->uFrameX + 215, pGUIWindow_CurrentMenu->uFrameY + 323, 0, 0, WINDOW_PressedButton2, (int)pBtnDownArrow, 0);
          break;
        case UIMSG_Cancel:
          GUIWindow::Create(pGUIWindow_CurrentMenu->uFrameX + 350, pGUIWindow_CurrentMenu->uFrameY + 302, 61, 28, WINDOW_LoadGame_CancelBtn, (int)pBtnCancel, 0);
          break;
        case UIMSG_ArrowUp:
          --pSaveListPosition;
          if ( pSaveListPosition < 0 )
            pSaveListPosition = 0;
          pWindow = pGUIWindow_CurrentMenu;
          GUIWindow::Create(pWindow->uFrameX + 215, pGUIWindow_CurrentMenu->uFrameY + 197, 0, 0, WINDOW_PressedButton2, (int)pBtnArrowUp, 0);
          break;
        case UIMSG_AD:
          GUIWindow::Create(pMainMenu_BtnNew->uX, pMainMenu_BtnNew->uY, 0, 0, WINDOW_PressedButton2, (int)pMainMenu_BtnNew, 0);
          SetCurrentMenuID(MENU_LoadingProcInMainMenu);
          break;
        case UIMSG_AE:
          GUIWindow::Create(pMainMenu_BtnExit->uX, pMainMenu_BtnExit->uY, 0, 0, WINDOW_PressedButton2, (int)pMainMenu_BtnExit, 0);
          SetCurrentMenuID(MENU_DebugBLVLevel);
          break;
        case UIMSG_Escape:
          if ( pModalWindow )
          {
            ModalWindow_Release();
            break;
          }
          if ( !(dword_6BE364_game_settings_1 & GAME_SETTINGS_4000))
            break;
          v15 = 1;
          pMediaPlayer->bStopBeforeSchedule = 1;
          viewparams->bRedrawGameUI = 1;
          viewparams->field_48 = 1;
          if ( GetCurrentMenuID() == MENU_MAIN || GetCurrentMenuID() == MENU_MMT_MAIN_MENU
			 || GetCurrentMenuID() == MENU_CREATEPARTY || GetCurrentMenuID() == MENU_NAMEPANELESC )
          {
            //if ( pCurrentScreen == SCREEN_VIDEO )
              //pVideoPlayer->FastForwardToFrame(pVideoPlayer->pResetflag);			  
            if (GetCurrentMenuID() == MENU_NAMEPANELESC)  //из панели изменения имени
            {
              SetCurrentMenuID(MENU_CREATEPARTY);//в окно создания группы
              break;
            }
            if (GetCurrentMenuID() == MENU_CREDITSPROC)	//из окна Создатели
            {
              SetCurrentMenuID(MENU_CREDITSCLOSE);//в закрытие Создатели
              break;
            }
            pMessageQueue_50CBD0->AddGUIMessage(UIMSG_ChangeGameState, 0, 0);
            break;
          }
          if ( GetCurrentMenuID() == MENU_CREDITSPROC && !pCurrentScreen )
          {
            //if ( pCurrentScreen == SCREEN_VIDEO )
              //pVideoPlayer->FastForwardToFrame(pVideoPlayer->pResetflag);
            if (GetCurrentMenuID() == MENU_NAMEPANELESC)
            {
              SetCurrentMenuID(MENU_CREATEPARTY);
              break;
            }
            if (GetCurrentMenuID() == MENU_CREDITSPROC)
            {
              SetCurrentMenuID(MENU_CREDITSCLOSE);
              break;
            }
            pMessageQueue_50CBD0->AddGUIMessage(UIMSG_ChangeGameState, 0, 0);
            break;
          }
          if ( pCurrentScreen == SCREEN_LOADGAME )
          {
            pIcons_LOD->RemoveTexturesPackFromTextureList();
            //crt_deconstruct_ptr_6A0118();
            pTexture_PCX.Release();
            pTexture_PCX.Load("title.pcx", 0);
            SetCurrentMenuID(MENU_MAIN);
            v15 = 1;
            pGUIWindow_CurrentMenu->Release();
            pGUIWindow2->Release();
            pGUIWindow2 = 0;
            pEventTimer->Resume();
            pCurrentScreen = SCREEN_GAME;
            viewparams->bRedrawGameUI = v15;
            break;
          }
          if ( pCurrentScreen == SCREEN_VIDEO )
          {
            //pVideoPlayer->Unload();
          }
          else
          {
            if ( pCurrentScreen != SCREEN_1B )
            {
              pGUIWindow_CurrentMenu->Release();
              pGUIWindow2->Release();
              pGUIWindow2 = 0;
              pEventTimer->Resume();
              pCurrentScreen = SCREEN_GAME;
              viewparams->bRedrawGameUI = v15;
              break;
            }
            //VideoPlayer::dtor();
          }
          break;
        default:
          break;
      }
    }
    while ( pMessageQueue_50CBD0->uNumMessages );
  }
}



//----- (0042FBDD) --------------------------------------------------------
void  sub_42FBDD()
{
  pAudioPlayer->PlaySound(SOUND_Button2, 0, 0, -1, 0, 0, 0, 0);
  pRenderer->DrawTextureTransparent(pBtn_YES->uX, pBtn_YES->uY, pBtn_YES->pTextures[0]);
  pRenderer->Present();
}

//----- (0042FC15) --------------------------------------------------------
void  CloseWindowBackground()
{
  pAudioPlayer->PlaySound(SOUND_Button2, -2, 0, -1, 0, 0, 0, 0);
  pRenderer->DrawTextureTransparent(pBtn_ExitCancel->uX, pBtn_ExitCancel->uY, pBtn_ExitCancel->pTextures[0]);
  pRenderer->Present();
}