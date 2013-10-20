#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "Vis.h"

#include "LightmapBuilder.h"
#include "DecalBuilder.h"
#include "ParticleEngine.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "CShow.h"
#include "GammaControl.h"
#include "stru6.h"
#include "stru9.h"
#include "stru10.h"

#include "Game.h"
#include "Party.h"
#include "Viewport.h"
#include "Time.h"
#include "Outdoor.h"
#include "Outdoor_stuff.h"
#include "Overlays.h"
#include "AudioPlayer.h"
#include "LOD.h"
#include "OSInfo.h"
#include "GUIWindow.h"
#include "TurnEngine.h"
#include "VideoPlayer.h"
#include "Bink_Smacker.h"
#include "Events.h"
#include "Arcomage.h"
#include "texts.h"
#include "GUIFont.h"
#include "Log.h"
#include "Lights.h"
#include "mm7_data.h"

//#include "MM7.h"






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
  //float v2; // ST24_4@11
  //double v3; // ST28_8@11
  int v4; // edi@26
  //int v5; // eax@35

  uFlags2 &= ~0x02;
  if ( pParty->_497FC5_check_party_perception_against_level() )
    uFlags2 |= 2u;

  pGame->pIndoorCameraD3D->sRotationX = pParty->sRotationX;
  pGame->pIndoorCameraD3D->sRotationY = pParty->sRotationY;
  pGame->pIndoorCameraD3D->vPartyPos.x = pParty->vPosition.x - pParty->y_rotation_granularity * cosf(2 * 3.141592653589 * pParty->sRotationY / 2048.0);
  pGame->pIndoorCameraD3D->vPartyPos.y = pParty->vPosition.y - pParty->y_rotation_granularity * sinf(2 * 3.141592653589 * pParty->sRotationY / 2048.0);
  pGame->pIndoorCameraD3D->vPartyPos.z = pParty->vPosition.z + pParty->sEyelevel;//193, but real 353

  //pIndoorCamera->Initialize2();
  pIndoorCameraD3D->CalculateRotations(pParty->sRotationX, pParty->sRotationY);
  pIndoorCameraD3D->CreateWorldMatrixAndSomeStuff();
  pIndoorCameraD3D->_4374E8_ProllyBuildFrustrum();

  if ( pVideoPlayer->AnyMovieLoaded() )
  {
    if ( !pRenderer->pRenderD3D )
    {
		pRenderer->BeginSceneD3D();
		pMouse->DrawCursorToTarget();
		pRenderer->DrawBillboards_And_MaybeRenderSpecialEffects_And_EndScene();
	}
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

    if ( !pRenderer->pRenderD3D )
      pMouse->DrawCursorToTarget();
    if ( !PauseGameDrawing() || viewparams->field_48 == 1 )
    {
      if ( pRenderer->pRenderD3D )
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
      else Error("Invalid level type: %u", uCurrentlyLoadedLevelType);

      if (pRenderer->pRenderD3D)
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
  if (pRenderer->pRenderD3D)
    pMouse->DrawCursorToTarget();
  if (pOtherOverlayList->bRedraw)
    viewparams->bRedrawGameUI = true;
  v4 = viewparams->bRedrawGameUI;
  GameUI_Footer();
  if (!viewparams->bRedrawGameUI)
  {
    GameUI_DrawRightPanelItems();
  }
  else
  {
    GameUI_DrawRightPanelFrames();
    GameUI_Footer_2();
    viewparams->bRedrawGameUI = false;
  }
  if (!pVideoPlayer->pSmackerMovie)
  {
    GameUI_DrawMinimap(488, 16, 625, 133, viewparams->uMinimapZoom, pParty->uFlags & 2);
    if (v4)
    {
      if ( !PauseGameDrawing() && pRenderer->pRenderD3D) // clear game viewport with transparent color
        pRenderer->FillRectFast(pViewport->uViewportTL_X, pViewport->uViewportTL_Y, pViewport->uViewportBR_X - pViewport->uViewportTL_X,
                                pViewport->uViewportBR_Y - pViewport->uViewportTL_Y + 1,
                                pRenderer->uTargetGMask | pRenderer->uTargetBMask);
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
  if ( sub_44100D() )
    GameUI_DrawRightPanel();
  if ( !pVideoPlayer->AnyMovieLoaded() )
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

  auto frame_dt = GetTickCount() - last_frame_time;
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

  extern bool debug_information;
  if ( debug_information )
  {
    if (render_framerate)
    {
      sprintf(pTmpBuf.data(), "FPS: % .4f", framerate);
      pPrimaryWindow->DrawText(pFontArrus, 494, 0, TargetColor(0, 0, 0), pTmpBuf.data(), 0, 0, 0);
    }

    if (uCurrentlyLoadedLevelType == LEVEL_Indoor)
    {
      auto sector_id = pIndoor->GetSector(pParty->vPosition.x, pParty->vPosition.y, pParty->vPosition.z);
      sprintf(pTmpBuf.data(), "Party Sector ID:        %u/%u\n", sector_id, pIndoor->uNumSectors);
      pPrimaryWindow->DrawText(pFontArrus, 16, 16, TargetColor(255, 255, 255), pTmpBuf.data(), 0, 0, 0xFFFFFFFF);
    }
    sprintf(pTmpBuf.data(), "Party Position:         % d % d % d", pParty->vPosition.x, pParty->vPosition.y, pParty->vPosition.z);
    pPrimaryWindow->DrawText(pFontArrus, 16, 16 + 16, TargetColor(255, 255, 255), pTmpBuf.data(), 0, 0, 0xFFFFFFFF);
  
    if (uCurrentlyLoadedLevelType == LEVEL_Indoor)
    {
      uint uFaceID;
      auto sector_id = pIndoor->GetSector(pParty->vPosition.x, pParty->vPosition.y, pParty->vPosition.z);
      auto floor_level = BLV_GetFloorLevel(pParty->vPosition.x, pParty->vPosition.y, pParty->vPosition.z + 40, sector_id, &uFaceID);
      sprintf(pTmpBuf.data(), "BLV_GetFloorLevel: %d   face_id %d\n", floor_level, uFaceID);
    }
    else
    {
      int on_water, _a6;
      auto floor_level = ODM_GetFloorLevel(pParty->vPosition.x, pParty->vPosition.y, pParty->vPosition.z, 0, &on_water, &_a6, false);
      sprintf(pTmpBuf.data(), "ODM_GetFloorLevel: %d   on_water: %s    a6 = %d\n", floor_level, on_water ? "true" : "false", _a6);
    }
    pPrimaryWindow->DrawText(pFontArrus, 16, 16 + 16 + 16, TargetColor(255, 255, 255), pTmpBuf.data(), 0, 0, 0xFFFFFFFF);
  }

  GUI_UpdateWindows();
  pParty->UpdatePlayersAndHirelingsEmotions();

  _unused_5B5924_is_travel_ui_drawn = false;
  if (v4)
    pMouse->bRedraw = true;
  pMouse->_469EA4();
  pMouse->DrawCursor();
  pMouse->_469E1C();
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
  //signed int v0; // ebp@3
  //signed int v1; // esi@4
  //Render *v2; // edi@7
  //signed int v3; // esi@7
  signed int pNewNPCsCount; // ecx@58
  NPCData *pNPC; // eax@59
  Player *pPlayer; // esi@65
  //OtherOverlay *pOtherOverlay; // esi@67
  //signed int v8; // edi@67
  //int pPlayerNum; // edi@69
  int *pHealth; // esi@71
  //signed int v11; // esi@78
  //int v12; // eax@83
  const char *pLocationName; // [sp-4h] [bp-68h]@74
  bool bLoading; // [sp+10h] [bp-54h]@1
  //signed int bLoadinga; // [sp+10h] [bp-54h]@19
  signed int v16; // [sp+14h] [bp-50h]@8
  //int v17[4]; // [sp+18h] [bp-4Ch]@80
  MSG Msg; // [sp+28h] [bp-3Ch]@20
  char Source[64]; // [sp+44h] [bp-20h]@76

  bLoading = uCurrentMenuID == MENU_LoadingProcInMainMenu;
  SetCurrentMenuID((MENU_STATE)-1);
  if (bLoading)
  {
    pParty->Reset();
    dword_6BE340 = 0;
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
    dword_6BE364_game_settings_1 |= 0x80;
    dword_6BE340 = 2;
    // uGame_if_0_else_ui_id__11_save__else_load__8_drawSpellInfoPopup__22_final_window__26_keymapOptions__2_options__28_videoOptions = 0;
    pCurrentScreen = SCREEN_GAME;

    if ( pRenderer->pRenderD3D )
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
      if (dword_6BE364_game_settings_1 & 0x0100 )
      {
        WaitMessage();
        continue;
      }
      pGame->_44EEA7();
      GameUI_WritePointedObjectStatusString();
      ProcessInputActions();
      GameUI_MsgProc();
      if ( pArcomageGame->bGameInProgress )
      {
        ArcomageGame::Loop();
        pRenderer->Present();
        continue;
      }

      pVideoPlayer->_inlined_in_463149();
      
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
        if ( dword_6BE364_game_settings_1 & 1 )
        {
          dword_6BE364_game_settings_1 &= 0xFFFFFFFEu;
        }
        else
        {
          UpdateActorAI();
          UpdateUserInput_and_MapSpecificStuff();
        }
      }
      if ( v16 )
      {
        v16 = 0;
        viewparams->bRedrawGameUI = true;
      }
      pAudioPlayer->UpdateSounds();
      if (uGameState == GAME_FINISHED)
        //goto LABEL_96;
      {
        game_finished = true;
        continue;
      }
      if (uGameState == GAME_STATE_2)
      {
        pAudioPlayer->StopChannels(-1, -1);
        PrepareWorld(0);
        uGameState = GAME_STATE_PLAYING;
        continue;
      }
      if ( (signed int)uGameState <= GAME_STATE_2 )
        //goto LABEL_85;
      {
        pGame->Draw();
        continue;
      }
      if ( (signed int)uGameState <= GAME_STATE_5 || uGameState == GAME_STATE_GAME_QUITTING_TO_MAIN_MENU )
      {
//LABEL_96:
        game_finished = true;
        continue;
      }
      if ( uGameState != GAME_STATE_PARTY_DIED )
      {
        if ( uGameState != GAME_STATE_FINAL_WINDOW )
        {
//LABEL_85:
          pGame->Draw();
        }
        else
        {
          pRenderer->BeginScene();
          GUI_UpdateWindows();
          pRenderer->EndScene();
          //goto LABEL_89;
          pRenderer->Present();
        }
        continue;
      }
      pAudioPlayer->StopChannels(-1, -1);//далее в случае смерти группы
      memset(&pParty->pHirelings[0], 0, 0x4Cu);
      memset(&pParty->pHirelings[1], 0, 0x4Cu);
      pNewNPCsCount = 0;
      if ( (signed int)pNPCStats->uNumNewNPCs > 0 )
      {
        pNPC = pNPCStats->pNewNPCData;
        do
        {
		  if ( pNPC->field_24 )
			  pNPC->uFlags &= 0xFFFFFF7Fu;
          ++pNewNPCsCount;
          ++pNPC;
        }
        while ( pNewNPCsCount < (signed int)pNPCStats->uNumNewNPCs );
      }
      pVideoPlayer->PlayDeathMovie();
      if ( pVideoPlayer->AnyMovieLoaded() )
        pVideoPlayer->Unload();
      SaveGame(0, 0);
      ++pParty->uNumDeaths;
      pPlayer = pParty->pPlayers.data();
      do
      {
        pPlayer->SetVariable(VAR_Award, 85);
        ++pPlayer;
      }
      while ( (signed int)pPlayer < (signed int)pParty->pHirelings.data() );
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
      //pHealth = &pParty->pPlayers[0].sHealth;//193C
      //do
	  for(int i=0; i<4; ++i)
      {
        memset(pParty->pPlayers[i].pConditions.data(), 0, 0xA0u);//(pConditions, 0, 160)
        memset(pParty->pPlayers[i].pPlayerBuffs.data(), 0, 0x180u);//(pPlayerBuffs[0], 0, 384)
        //*pHealth = 1;
		pParty->pPlayers[i].sHealth=1;
        //pHealth += 1743; //6CF
        uActiveCharacter = 1;
      }
    //  while ( (signed int)pHealth < (signed int)&pParty->field_871C[567] );
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
      InitializeActors();


      int num_conscious_players = 0;
      int conscious_players_ids[4] = {-1, -1, -1, -1};
      for (int v11 = 0; v11 < 4; ++v11)
      {
        if (pPlayers[v11 + 1]->CanAct())
          conscious_players_ids[num_conscious_players++] = v11;
      }
      if (num_conscious_players)
      {
        int idx = conscious_players_ids[rand() % num_conscious_players];
        Assert(idx >= 0);
        pPlayers[idx + 1]->PlaySound(SPEECH_99, 0);
      }

      ShowStatusBarString(pGlobalTXT_LocalizationStrings[524], 2u);// "Once again you've cheated death!.." "Вы снова обхитрили смерть! …"
      uGameState = GAME_STATE_PLAYING;
    }
    while (!game_finished);

    dword_6BE340 = 0;
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
    pBloodsplatContainer->AddBloodsplat(
        pBloodsplats[i].x,
        pBloodsplats[i].y,
        pBloodsplats[i].z,
        pBloodsplats[i].radius,
        pBloodsplats[i].r,
        pBloodsplats[i].g,
        pBloodsplats[i].b);
   }
}


//----- (0044F120) --------------------------------------------------------
void Game::PushStationaryLights(int a2)
{
  Game__StationaryLight* pLight;
 
  for( int i=0; i<uNumStationaryLights; ++i ) 
      {
       pLight=&pStationaryLights[i];
       pStationaryLightsStack->AddLight(
           pLight->vPosition.x, 
           pLight->vPosition.y,
           pLight->vPosition.z,
           pLight->flt_18,
           pLight->vRGBColor.x,
           pLight->vRGBColor.y,
           pLight->vRGBColor.z,
           byte_4E94D0);
      }
}
// 4E94D0: using guessed type char byte_4E94D0;

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
  if (!pKeyboardInstance->IsKeyBeingHeld(VK_SHIFT) &&
      !pKeyboardInstance->IsKeyBeingHeld(VK_LSHIFT) &&
      !pKeyboardInstance->IsKeyBeingHeld(VK_LSHIFT) ||

      (pKeyboardInstance->WasKeyPressed(VK_F11) == 0 &&
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
  unsigned int v5; // eax@14
  __int64 v6; // kr00_8@21
  unsigned int y; // [sp+4h] [bp-24h]@2
  unsigned int x; // [sp+8h] [bp-20h]@2
  Vis_SelectionFilter *v10; // [sp+10h] [bp-18h]@2
  Vis_SelectionFilter *v11; // [sp+14h] [bp-14h]@2
  POINT a2; // [sp+20h] [bp-8h]@1

  //v1 = this;
  ++qword_5C6DF0;
  pParticleEngine->UpdateParticles();
  pMouseInstance->GetCursorPos(&a2);

  x = a2.y;
  y = a2.x;
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

  PickMouse(depth, y, x, false, v10, v11);
  pLightmapBuilder->std__vector_000004_size = 0;
  pLightmapBuilder->std__vector_183808_size = 0;
  pDecalBuilder->std__vector_pDecals_size = 0;
  pDecalBuilder->field_308008 = 0;
  if (!_44F07B())
    return false;

    if ( uFlags & GAME_FLAGS_1_DRAW_BLV_DEBUGS)
      pStru10Instance->bDoNotDrawPortalFrustum = false;
    if ( pRenderer->pRenderD3D && uCurrentlyLoadedLevelType == LEVEL_Outdoor)
    {
      v5 = GetLevelFogColor();
      pRenderer->uFogColor = v5 & 0xFFFFFF;
    }
    if (uFlags & 0x0400)
      uFlags2 |= 0x01;
    if ( !pRenderer->pRenderD3D && uCurrentlyLoadedLevelType == LEVEL_Outdoor && pMobileLightsStack->uNumLightsActive )
    {
      uFlags2 |= 0x01;
      field_E10 = qword_5C6DF0;
    }
    v6 = qword_5C6DF0 - field_E10;
    if ( qword_5C6DF0 - field_E10 == 1 )
      uFlags2 |= v6;
    if (uNumStationaryLights_in_pStationaryLightsStack != pStationaryLightsStack->uNumLightsActive )
    {
      uFlags2 |= 1u;
      uNumStationaryLights_in_pStationaryLightsStack = pStationaryLightsStack->uNumLightsActive;
    }
    _44E904();
    return true;
}


//----- (0044EDE4) --------------------------------------------------------
bool Game::AlterGamma_BLV(BLVFace *pFace, unsigned int *pColor)
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
bool Game::AlterGamma_ODM(ODMFace *pFace, unsigned int *pColor)
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
  struct tagRECT Rect; // [sp+0h] [bp-10h]@6


  WriteWindowsRegistryInt("startinwindow", pRenderer->bWindowMode);
  /*if ( GetWindowRect(hWnd, &Rect) && pRenderer->bWindowMode )
  {
    WriteWindowsRegistryInt("window X", Rect.left);
    WriteWindowsRegistryInt("window Y", Rect.top);
  }*/
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
    {
      v6 = 0;
    }
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
      {
        result = 0;
      }
    }
    else
    {
      result = a4;
    }
  }
  else
  {
    result = -1;
  }
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
  if (pGame)
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
    {
      v6 = 0;
    }
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
      {
        result = 0;
      }
    }
    else
    {
      result = a4;
    }
  }
  else
  {
    result = -1;
  }
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

  bWinNT4_0 = false;
  if (pVersion->pVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT &&
      pVersion->pVersionInfo.dwMajorVersion == 4)
    bWinNT4_0 = true;
}

//----- (0044E7F3) --------------------------------------------------------
Game::~Game()
{
  if (pGammaController)
    delete pGammaController;
  if (pKeyboardInstance)
    delete pKeyboardInstance;
  /*if (pCShow)
    delete pCShow;
  if (pStru12Instance)
    delete pStru12Instance;
  if (pStru11Instance)
    delete pStru11Instance;*/
  if (pStru10Instance)
    delete pStru10Instance;
  if (pStru9Instance)
    delete pStru9Instance;
  if (pIndoorCameraD3D)
    delete pIndoorCameraD3D;
  if (pStru6Instance)
    delete pStru6Instance;
  if (pVisInstance)
    delete pVisInstance;
  if (pLightmapBuilder)
    delete pLightmapBuilder;
  if (pMouseInstance)
    delete pMouseInstance;
  if (pParticleEngine)
    delete pParticleEngine;
  //if (pThreadWardInstance)
  //  delete pThreadWardInstance;
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
  {
    v7 = 1.0;
  }
  if ( pRenderer->pRenderD3D )
    fSaturation = v7;
  else
    fSaturation = (1.0 - 0.5) * v7 + 0.5;
}

//----- (0044EA17) --------------------------------------------------------
bool Game::InitializeGammaController()
{
  if (pVersion->pVersionInfo.dwPlatformId != VER_PLATFORM_WIN32_NT ||
      pVersion->pVersionInfo.dwMajorVersion != 4 )
    pGammaController->InitializeFromSurface(pRenderer->pFrontBuffer4);

  bGammaControlInitialized = true;
  uSomeGammaStartTime = pEventTimer->Time();
  return true;
}

//----- (0044EA5E) --------------------------------------------------------
bool Game::PickMouse(float fPickDepth, unsigned int uMouseX, unsigned int uMouseY, bool bOutline, Vis_SelectionFilter *sprite_filter, Vis_SelectionFilter *face_filter)
{
  if (pCurrentScreen != SCREEN_GAME|| !pRenderer->pRenderD3D)
    return false;

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
  if (pCurrentScreen == SCREEN_GAME && pVisInstance && pRenderer->pRenderD3D)
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

  auto object_info = pVisInstance->default_list.object_pointers[0];
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
          auto face = (ODMFace *)object_info->object;
          if (face->uAttributes & FACE_OUTLINED)
            face->uAttributes &= ~FACE_OUTLINED;
          else
            face->uAttributes |= FACE_OUTLINED;
        }
        else if (uCurrentlyLoadedLevelType == LEVEL_Indoor)
        {
          auto face = (BLVFace *)object_info->object;
          if (face->uAttributes & FACE_OUTLINED)
            face->uAttributes &= ~FACE_OUTLINED;
          else
            face->uAttributes |= FACE_OUTLINED;
        }
        else Error("Invalid level type", uCurrentlyLoadedLevelType);
      }
      break;

      default:
      {
        MessageBoxW(nullptr, L"Undefined CObjectInfo type requested in CGame::outline_selection()", nullptr, 0);
        ExitProcess(0);
      }
    }
}