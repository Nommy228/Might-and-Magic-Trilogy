#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "Texture.h"
#include "mm7_data.h"
#include "VideoPlayer.h"
#include "Sprites.h"
#include "MapInfo.h"
#include "BSPModel.h"
#include "LightmapBuilder.h"
#include "DecalBuilder.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "GammaControl.h"
#include "mm7_data.h"
#include "FactionTable.h"
#include "Vis.h"
#include "mm7.h"
#include "Game.h"
#include "GUIWindow.h"
#include "GUIFont.h"
#include "GUIProgressBar.h"
#include "Party.h"
#include "AudioPlayer.h"
#include "Outdoor.h"
#include "Arcomage.h"
#include "LOD.h"
#include "Actor.h"
#include "Events.h"
#include "Viewport.h"
#include "FrameTableInc.h"
#include "Math.h"
#include "SpriteObject.h"
#include "ObjectList.h"
#include "Chest.h"
#include "PaletteManager.h"
#include "SaveLoad.h"
#include "Time.h"
#include "TurnEngine.h"
#include "CastSpellInfo.h"
#include "stru298.h"
#include "Events2D.h"
#include "texts.h"
#include "Log.h"
#include "UI\UIHouses.h"
#include "Lights.h"
#include "Lights.h"
#include "Level/Decoration.h"

//----- (004304E7) --------------------------------------------------------
void  GameUI_MsgProc()
{
  //signed int v0; // edi@6
  //char *v1; // esi@6
  unsigned int v2; // edx@7
  Actor *pActor; // ecx@13
  int v4; // ecx@18
  //NPCData *pNPCData0; // eax@18
  //int v6; // edx@20
  //int v7; // ecx@29
  //unsigned int v8; // edx@59
  //unsigned int v9; // ecx@60
  unsigned int v10; // ecx@73
  //unsigned int v11; // eax@75
  unsigned __int8 v12; // sf@75
  unsigned __int8 v13; // of@75
  int v14; // eax@98
  unsigned int v15; // eax@102
  unsigned __int8 v16; // al@104
  unsigned __int8 v17; // al@105
  int v18; // eax@106
  float v19; // ST64_4@121
  unsigned int v20; // ecx@121
  float v21; // ST64_4@126
  float v22; // ST64_4@127
  unsigned int v23; // ecx@135
  unsigned int v24; // ecx@149
  unsigned int v25; // ecx@165
  GUIWindow *pWindow; // eax@204
  unsigned int v27; // edx@204
  unsigned int v28; // ecx@204
  GUIWindow *pWindow2; // ecx@248
  //int v30; // edx@258
  //const char *v31; // ecx@262
  signed int v32; // eax@269
  unsigned int v33; // eax@277
  unsigned __int8 v34; // al@279
  unsigned __int8 v35; // al@280
  //GUIWindow *pWindow3; // ecx@332
  int v37; // eax@341
  int v38; // eax@358
  SHORT v39; // ax@365
  //signed int v40; // eax@365
  char *v41; // eax@380
  int v42; // eax@396
  POINT *pPoint; // eax@397
  signed int v44; // eax@398
  int v45; // edx@398
  signed int v46; // ecx@398
  char v47; // zf@399
  char v48; // zf@405
  BLVFace *pBLVFace; // ecx@410
  ODMFace *pODMFace; // ecx@412
  CastSpellInfo *pSpellInfo; // ecx@415
  void *v52; // eax@424
  __int16 v53; // ax@431
  int v54; // eax@432
  int v55; // ecx@432
  int v56; // edx@432
  int v57; // eax@432
  Player *pPlayer; // edx@442
  unsigned int pMapNum; // eax@445
  signed int v60; // ST64_4@459
  //NPCData *pNPCData2; // eax@467
  //unsigned __int64 v62; // kr00_8@467
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
  //int v79; // ecx@550
  //unsigned int v80; // edx@550
  signed int v81; // eax@552
  POINT *pPoint2; // eax@553
  signed int v83; // ecx@554
  signed int v84; // ecx@554
  GUIButton *pButton; // eax@578
  unsigned int v86; // eax@583
  const char *v87; // ecx@595
  const char *v88; // ecx@596
  //unsigned int v89; // eax@598
  unsigned int v90; // eax@602
  int v91; // edx@605
  int v92; // eax@605
  int v93; // edx@605
  int pPlayerNum; // edx@611
  int v95; // eax@611
  //const char *v96; // ecx@621
  unsigned int v97; // eax@624
  int v98; // eax@636
  unsigned __int8 v99; // al@643
  Player *pPlayer3; // eax@648
  int v101; // ecx@648
  int v102; // edx@652
  int v103; // eax@671
  Player *pPlayer4; // ecx@718
  int v105; // eax@718
  Player *pPlayer5; // ST78_4@758
  unsigned int v107; // eax@758
  unsigned int v108; // eax@758
  unsigned int v115; // eax@764
  int v116; // eax@776
  POINT *pPoint3; // eax@777
  unsigned int v118; // eax@785
  unsigned int v119; // ecx@786
  unsigned int v120; // [sp-28h] [bp-624h]@86
  unsigned int v121; // [sp-28h] [bp-624h]@711
  unsigned int v122; // [sp-24h] [bp-620h]@86
  unsigned int v123; // [sp-24h] [bp-620h]@711
  unsigned int v124; // [sp-20h] [bp-61Ch]@86
  unsigned int v125; // [sp-20h] [bp-61Ch]@711
  unsigned int v126; // [sp-1Ch] [bp-618h]@86
  int v127; // [sp-1Ch] [bp-618h]@107
  unsigned int v128; // [sp-1Ch] [bp-618h]@711
  int v129; // [sp-18h] [bp-614h]@86
  //signed int v130; // [sp-18h] [bp-614h]@107
  int v131; // [sp-14h] [bp-610h]@86
  //unsigned int v132; // [sp-14h] [bp-610h]@107
  //unsigned int v133; // [sp-10h] [bp-60Ch]@60
  unsigned int v134; // [sp-10h] [bp-60Ch]@86
  //signed int v135; // [sp-10h] [bp-60Ch]@107
  unsigned int v136; // [sp-10h] [bp-60Ch]@121
  //unsigned int v137; // [sp-Ch] [bp-608h]@60
  unsigned int v138; // [sp-Ch] [bp-608h]@86
  //signed int v139; // [sp-Ch] [bp-608h]@107
  unsigned int v140; // [sp-Ch] [bp-608h]@121
  enum WindowType pWindowType; // [sp-8h] [bp-604h]@56
  //enum WindowType pWindowType1; // [sp-8h] [bp-604h]@60
  unsigned __int8 v143; // [sp-8h] [bp-604h]@86
  //int v144; // [sp-8h] [bp-604h]@107
  enum WindowType pWindowType2; // [sp-8h] [bp-604h]@121
  //const char *v146; // [sp-8h] [bp-604h]@449
  //unsigned int v147; // [sp-8h] [bp-604h]@550
  //int v148; // [sp-4h] [bp-600h]@56
  GUIButton *pButton2; // [sp-4h] [bp-600h]@59
  const char *v150; // [sp-4h] [bp-600h]@86
  //unsigned int v151; // [sp-4h] [bp-600h]@107
  int v152; // [sp-4h] [bp-600h]@121
  int v153; // [sp-4h] [bp-600h]@135
  //int v154; // [sp-4h] [bp-600h]@149
  int v155; // [sp-4h] [bp-600h]@165
  int v156; // [sp-4h] [bp-600h]@204
  //const char *v157; // [sp-4h] [bp-600h]@444
  //unsigned int v158; // [sp-4h] [bp-600h]@449
  //__int16 v159; // [sp-4h] [bp-600h]@550
  int v160; // [sp-4h] [bp-600h]@599
  const char *v161; // [sp-4h] [bp-600h]@637
  //int v162; // [sp+0h] [bp-5FCh]@56
  //int v163; // [sp+0h] [bp-5FCh]@59
  Texture *pTexture; // [sp+0h] [bp-5FCh]@86
  //int v165; // [sp+0h] [bp-5FCh]@107
  int v166; // [sp+0h] [bp-5FCh]@121
  int v167; // [sp+0h] [bp-5FCh]@135
  //int v168; // [sp+0h] [bp-5FCh]@149
  int v169; // [sp+0h] [bp-5FCh]@165
  int v170; // [sp+0h] [bp-5FCh]@204
  //signed int v171; // [sp+0h] [bp-5FCh]@259
  KeyToggleType pKeyToggleType; // [sp+0h] [bp-5FCh]@287
  char *v173; // [sp+0h] [bp-5FCh]@444
  char *v174; // [sp+0h] [bp-5FCh]@449
  //int v175; // [sp+0h] [bp-5FCh]@550
  int v176; // [sp+0h] [bp-5FCh]@599
  const char *v177; // [sp+0h] [bp-5FCh]@629
  char *v178; // [sp+0h] [bp-5FCh]@637
  int v179; // [sp+4h] [bp-5F8h]@0
  //signed int _this; // [sp+14h] [bp-5E8h]@22
  signed int thisa; // [sp+14h] [bp-5E8h]@251
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
  int thisl; // [sp+14h] [bp-5E8h]@648
  int uMessageParam; // [sp+18h] [bp-5E4h]@7
  int uAction; // [sp+1Ch] [bp-5E0h]@18
  NPCData *pNPCData4; // [sp+20h] [bp-5DCh]@23
  unsigned int uNumSeconds; // [sp+24h] [bp-5D8h]@18
  char v197; // [sp+2Bh] [bp-5D1h]@101
  enum UIMessageType uMessage; // [sp+2Ch] [bp-5D0h]@7
  unsigned int v199; // [sp+30h] [bp-5CCh]@7
  char *v200; // [sp+34h] [bp-5C8h]@518
  POINT v201; // [sp+38h] [bp-5C4h]@553
  POINT v202; // [sp+40h] [bp-5BCh]@141
  POINT a2; // [sp+48h] [bp-5B4h]@127
  POINT v204; // [sp+50h] [bp-5ACh]@777
  POINT v205; // [sp+58h] [bp-5A4h]@171
  POINT v206; // [sp+60h] [bp-59Ch]@553
  POINT v207; // [sp+68h] [bp-594h]@155
  POINT v208; // [sp+70h] [bp-58Ch]@397
  POINT v209; // [sp+78h] [bp-584h]@777
  POINT v210; // [sp+80h] [bp-57Ch]@397
  POINT v211; // [sp+88h] [bp-574h]@704
  //__int64 v212; // [sp+90h] [bp-56Ch]@467
  int v213; // [sp+98h] [bp-564h]@385
  char pLevelName[32]; // [sp+9Ch] [bp-560h]@380
  char pOut[32]; // [sp+BCh] [bp-540h]@370
  FrameTableTxtLine v216; // [sp+DCh] [bp-520h]@524
  int v217[9]; // [sp+158h] [bp-4A4h]@652
  FrameTableTxtLine v218; // [sp+17Ch] [bp-480h]@524
  char a1[64]; // [sp+1F8h] [bp-404h]@467
  char Str2[128]; // [sp+238h] [bp-3C4h]@527
  Actor actor; // [sp+2B8h] [bp-344h]@4

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
          pVideoPlayer->_4BF5B2();
          pArcomageGame->bGameInProgress = 1;
          PrepareArcomage();
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
              auto npc = pNPCStats->pNewNPCData + i;
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
            ShowStatusBarString(pGlobalTXT_LocalizationStrings[201], 2u);// "Are you sure?  Click again to start a New Game"
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
          pGUIWindow_CurrentMenu = GUIWindow::Create(0, 0, 640, 480, WINDOW_8, 0, 0);
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
              pMessageQueue_50CBD0->AddMessage(UIMSG_SaveLoadBtn, 0, 0);
              pMessageQueue_50CBD0->AddMessage(UIMSG_LoadGame, 0, 0);
            }
            uLoadGameUI_SelectedSlot = v10;
            dword_6BE138 = v10;
          }
          else
          {
            pKeyActionMap->EnterText(0, 19, pGUIWindow_CurrentMenu);
            if ( strcmp((const char *)&pSavegameHeader[uLoadGameUI_SelectedSlot], pGlobalTXT_LocalizationStrings[72]) )// "Empty"
              strcpy((char *)pKeyActionMap->pPressedKeysBuffer, (const char *)&pSavegameHeader[uLoadGameUI_SelectedSlot]);
            pKeyActionMap->uNumKeysPressed = strlen((const char *)pKeyActionMap->pPressedKeysBuffer);
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
            strcpy((char *)&pSavegameHeader[uLoadGameUI_SelectedSlot], (const char *)pKeyActionMap->pPressedKeysBuffer);
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

          pGUIWindow_CurrentMenu = GUIWindow::Create(0, 0, 640, 480, WINDOW_Options, 0, 0);
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
          pGUIWindow_CurrentMenu = GUIWindow::Create(0, 0, 640, 480, WINDOW_KeyMappingOptions, 0, 0);
          pGUIWindow_CurrentMenu->CreateButton(0xF1u, 0x12Eu, 0xD6u, 0x28u, 1, 0, UIMSG_Escape, 0, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(19u, 0x12Eu, 0x6Cu, 0x14u, 1, 0, UIMSG_SelectKeyPage1, 0, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(127u, 0x12Eu, 0x6Cu, 0x14u, 1, 0, UIMSG_SelectKeyPage2, 0, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(127u, 0x144u, 0x6Cu, 0x14u, 1, 0, UIMSG_ResetKeyMapping, 0, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(19u, 0x144u, 0x6Cu, 0x14u, 1, 0, UIMSG_Game_OpenOptionsDialog, 0, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(129u, 0x94u, 0x46u, 0x13u, 1, 0, UIMSG_ChangeKeyButton, 0, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(129u, 0xA7u, 0x46u, 0x13u, 1, 0, UIMSG_ChangeKeyButton, 1u, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(129u, 0xBAu, 0x46u, 0x13u, 1, 0, UIMSG_ChangeKeyButton, 2u, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(129u, 0xCDu, 0x46u, 0x13u, 1, 0, UIMSG_ChangeKeyButton, 3u, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(129u, 224u, 70u, 19u, 1, 0, UIMSG_ChangeKeyButton, 4u, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(129u, 243u, 70u, 0x13u, 1, 0, UIMSG_ChangeKeyButton, 5u, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(129u, 262u, 70u, 0x13u, 1, 0, UIMSG_ChangeKeyButton, 6u, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(350u, 148u, 70u, 0x13u, 1, 0, UIMSG_ChangeKeyButton, 7u, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(350u, 167u, 70u, 0x13u, 1, 0, UIMSG_ChangeKeyButton, 8u, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(350u, 186u, 70u, 0x13u, 1, 0, UIMSG_ChangeKeyButton, 9u, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(350u, 205u, 70u, 0x13u, 1, 0, UIMSG_ChangeKeyButton, 0xAu, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(350u, 224u, 70u, 0x13u, 1, 0, UIMSG_ChangeKeyButton, 0xBu, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(350u, 243u, 70u, 0x13u, 1, 0, UIMSG_ChangeKeyButton, 0xCu, 0, "", 0);
          pGUIWindow_CurrentMenu->CreateButton(350u, 262u, 70u, 0x13u, 1, 0, UIMSG_ChangeKeyButton, 0xDu, 0, "", 0);
          uGameMenuUI_CurentlySelectedKeyIdx = -1;
          KeyboardPageNum = 1;
          memset(GameMenuUI_InvaligKeyBindingsFlags.data(), 0, sizeof(GameMenuUI_InvaligKeyBindingsFlags));
          //*(_WORD *)KeyButtonArray[28] = 0;
          memcpy(pPrevVirtualCidesMapping.data(), pKeyActionMap->pVirtualKeyCodesMapping, 0x78u);
          //v1 = "";
          //v0 = 1;
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
          uAction = 0;
          do
          {
            v15 = pKeyActionMap->GetActionVKey((enum InputAction)uAction);
            if ( v15 != pPrevVirtualCidesMapping[uAction] )
            {
              if ( v197 )
              {
                v16 = pKeyActionMap->GetActionVKey((enum InputAction)uAction);
                GUI_ReplaceHotkey(LOBYTE(pPrevVirtualCidesMapping[uAction]), v16, 1);
                v197 = 0;
              }
              else
              {
                v17 = pKeyActionMap->GetActionVKey((enum InputAction)uAction);
                GUI_ReplaceHotkey(LOBYTE(pPrevVirtualCidesMapping[uAction]), v17, 0);
              }
            }
            pPrevVirtualCidesMapping[uAction] = pKeyActionMap->GetActionVKey((enum InputAction)uAction);
            v18 = uAction++;
            v13 = uAction > 28;
            v12 = uAction - 28 < 0;
            GameMenuUI_InvaligKeyBindingsFlags[v18] = 0;
          }
          while ( v12 ^ v13 );
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
          pGUIWindow_CurrentMenu = GUIWindow::Create(0, 0, 640, 480, WINDOW_VideoOptions, 0, 0);
          pGUIWindow_CurrentMenu->CreateButton(0xF1u, 0x12Eu, 0xD6u, 0x28u, 1, 0, UIMSG_Escape, 0, 0, "", 0);
          if ( pRenderer->pRenderD3D )
          {
            pGUIWindow_CurrentMenu->CreateButton(0x13u, 0x118u, 0xD6u, 0x12u, 1, 0, UIMSG_ToggleBloodsplats, 0, 0, "", 0);
            pGUIWindow_CurrentMenu->CreateButton(0x13u, 0x12Eu, 0xD6u, 0x12u, 1, 0, UIMSG_ToggleColoredLights, 0, 0, "", 0);
            pGUIWindow_CurrentMenu->CreateButton(0x13u, 0x144u, 0xD6u, 0x12u, 1, 0, UIMSG_ToggleTint, 0, 0, "", 0);
          }
          if ( !pRenderer->bWindowMode )
          {
            //v0 = 1;
            if ( GammaController::IsGammaSupported() )
            {
              pBtn_SliderLeft = pGUIWindow_CurrentMenu->CreateButton(0x15u, 0xA1u, 0x10u, 0x10u, 1, 0, UIMSG_1A9, 4u, 0, "", pIcons_LOD->GetTexture(uTextureID_507C20), 0);
              pBtn_SliderRight = pGUIWindow_CurrentMenu->CreateButton(0xD5u, 0xA1u, 0x10u, 0x10u, 1, 0, UIMSG_1A9, 5u, 0, "", pIcons_LOD->GetTexture(uTextureID_507C24), 0);
              pGUIWindow_CurrentMenu->CreateButton(42, 162, 170, 18, 1, 0, UIMSG_1A9, 0, 0, "", 0);
            }
          }
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
          pRenderer->bUseColoredLights = pRenderer->bUseColoredLights == 0;
          continue;
        case UIMSG_ToggleTint:
          pRenderer->bTinting = pRenderer->bTinting == 0;
          continue;
        case UIMSG_ChangeMusicVolume:
          if ( uMessageParam == 4 )
          {
            --uMusicVolimeMultiplier;
            if ( (char)uMusicVolimeMultiplier < 1 )
              uMusicVolimeMultiplier = 0;
            GUIWindow::Create(243, 0xD8u, 0, 0, WINDOW_PressedButton2, (int)pBtn_SliderLeft, (char *)1);
            if ( uMusicVolimeMultiplier )
              pAudioPlayer->PlaySound(SOUND_Bell, -1, 0, -1, 0, 0, pSoundVolumeLevels[(char)uMusicVolimeMultiplier] * 64.0f, 0);
            pAudioPlayer->SetMusicVolume(pSoundVolumeLevels[(char)uMusicVolimeMultiplier] * 64.0f);
            continue;
          }
          if ( uMessageParam == 5 )
          {
            ++uMusicVolimeMultiplier;
            if ( (char)uMusicVolimeMultiplier > 9 )
              uMusicVolimeMultiplier = 9;
            GUIWindow::Create(435, 0xD8u, 0, 0, WINDOW_PressedButton2, (int)pBtn_SliderRight, (char *)1);
            if ( uMusicVolimeMultiplier )
              pAudioPlayer->PlaySound(SOUND_Bell, -1, 0, -1, 0, 0, pSoundVolumeLevels[(char)uMusicVolimeMultiplier] * 64.0f, 0);
            pAudioPlayer->SetMusicVolume(pSoundVolumeLevels[(char)uMusicVolimeMultiplier] * 64.0f);
            continue;
          }
          uMusicVolimeMultiplier = (pMouse->GetCursorPos(&v202)->x - 263) / 17;
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
          if ( !bUseLoResSprites )
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
          }
          ModalWindow(pNPCTopics[453].pText, UIMSG_0);
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
          pGUIWindow_CurrentMenu = GUIWindow::Create(0, 0, 640u, 480u, WINDOW_Book, uMessage, 0);
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
          pGUIWindow_CurrentMenu = GUIWindow::Create(0, 0, 640u, 480u, WINDOW_Book, uMessage, 0);
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
          pGUIWindow_CurrentMenu = GUIWindow::Create(0, 0, 640, 480, WINDOW_Book, uMessage, 0);
          pBooksWindow = GUIWindow::Create(546, 353, 0, 0, WINDOW_BooksWindow, (int)pBtn_Maps, 0);
          continue;
        case UIMSG_OpenCalendar:
          if ( pMessageQueue_50CBD0->uNumMessages )
            pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
          if ( pCurrentScreen )
            pGUIWindow_CurrentMenu->Release();
          pEventTimer->Pause();
          pAudioPlayer->StopChannels(-1, -1);
          pGUIWindow_CurrentMenu = GUIWindow::Create(0, 0, 640, 480, WINDOW_Book, uMessage, 0);
          pBooksWindow = GUIWindow::Create(570, 354, 0, 0, WINDOW_BooksWindow, (int)pBtn_Calendar, 0);
          continue;
        case UIMSG_OpenHistoryBook:
          if ( pMessageQueue_50CBD0->uNumMessages )
            pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
          if ( pCurrentScreen )
            pGUIWindow_CurrentMenu->Release();
          pEventTimer->Pause();
          pAudioPlayer->StopChannels(-1, -1);
          pGUIWindow_CurrentMenu = GUIWindow::Create(0, 0, 640, 480, WINDOW_Book, uMessage, 0);
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
              if ( !dword_50CDC8 && !dword_5C35C8 )
              {
                CloseWindowBackground();
                dword_5C35C8 = 0;
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
                    {
                      pMouse->SetCursorBitmap("MICON2");
                    }
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
                        ptr_50C9A4_ItemToEnchant = 0;
                      }
                      if ( pGUIWindow_Settings )
                      {
                       if ( pCurrentScreen == SCREEN_CHARACTERS )
                       {
                        pMouse->SetCursorBitmap("MICON2");
                       }
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
                        {
                         pMouse->SetCursorBitmap("MICON2");
                        }
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
                      WriteWindowsRegistryInt("graphicsmode", (unsigned __int8)byte_6BE388_graphicsmode);
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
                      if ( pRenderer->pRenderD3D )
                      {
                        WriteWindowsRegistryInt("Colored Lights", pRenderer->bUseColoredLights);
                        WriteWindowsRegistryInt("Tinting", pRenderer->bTinting);
                        WriteWindowsRegistryInt("Bloodsplats", (LOBYTE(pGame->uFlags2) >> 5) & 1);
                      }
                      if ( pRenderer->bWindowMode )
                        {
                          stru_506E40.Release();
                          break;
                        }
                      //v30 = uGammaPos;
                      //v31 = "GammaPos";
                      WriteWindowsRegistryInt("GammaPos", uGammaPos);
                      stru_506E40.Release();
                      break;

                    case SCREEN_KEYBOARD_OPTIONS://Return to game
                      v197 = 1;
                      v32 = 0;
                      while ( !GameMenuUI_InvaligKeyBindingsFlags[v32])
                      {
                        ++v32;
                        if ( v32 >= 28 )
                        {
                          thisb = (signed int)&uTextureID_Optkb;
                          assert(false && "Invalid condition values");
                          do
                          {
                            if ( *(int *)thisb )
                              pIcons_LOD->pTextures[*(int *)thisb].Release();
                            thisb += 4;
                          }
                          while ( thisb < (signed int)0x00507C08 );

                          memset(&uTextureID_Optkb, 0, 0x14u);
                          pIcons_LOD->SyncLoadedFilesCount();
                          uAction = 0;
                          do
                          {
                            v33 = pKeyActionMap->GetActionVKey((enum InputAction)uAction);
                            if ( v33 != pPrevVirtualCidesMapping[uAction] )
                            {
                              if ( v197 )
                              {
                                v34 = pKeyActionMap->GetActionVKey((enum InputAction)uAction);
                                GUI_ReplaceHotkey(v34, LOBYTE(pPrevVirtualCidesMapping[uAction]), 1);
                                v197 = 0;
                              }
                              else
                              {
                                v35 = pKeyActionMap->GetActionVKey((enum InputAction)uAction);
                                GUI_ReplaceHotkey(v35, LOBYTE(pPrevVirtualCidesMapping[uAction]), 0);
                              }
                            }
                            if ( uAction && uAction != 2 && uAction != 3 && uAction != 1 && uAction != 25 && uAction != 26 )
                              pKeyToggleType = (KeyToggleType)1;
                            else
                              pKeyToggleType = (KeyToggleType)0;
                            pKeyActionMap->SetKeyMapping(uAction, pPrevVirtualCidesMapping[uAction], pKeyToggleType);
                            ++uAction;
                          }
                          while ( uAction < 28 );
                          pKeyActionMap->StoreMappings();
                          stru_506E40.Release();
                          break;
                        }
                      }
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
                      dword_507B94 = 1;
                      if ( pGUIWindow_Settings )
                      {
                        if ( pCurrentScreen == SCREEN_CHARACTERS )
                        {
                          pMouse->SetCursorBitmap("MICON2");
                        }
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
                      if ( uGameState == GAME_STATE_2 )
                      {
                        while ( sub_4BD8B5() )
                          ;
                      }
                      else
                      {
                        if ( sub_4BD8B5() )
                          continue;
                      }
                      GetHouseGoodbyeSpeech();
                      pAudioPlayer->PlaySound(SOUND_7, 814, 0, -1, 0, 0, 0, 0);
                      pVideoPlayer->Unload();
                      pGUIWindow_CurrentMenu = window_SpeakInHouse;
                      if ( pGUIWindow_Settings )
                      {
                        if ( pCurrentScreen == SCREEN_CHARACTERS )
                        {
                          pMouse->SetCursorBitmap("MICON2");
                        }
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
                      pVideoPlayer->Unload();
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
                      pVideoPlayer->Unload();
                      continue;
                    case SCREEN_CHARACTERS:
                      CharacterUI_ReleaseButtons();
                      ReleaseAwardsScrollBar();
                      pIcons_LOD->RemoveTexturesPackFromTextureList();
                      if ( pGUIWindow_Settings )
                      {
                        if ( pCurrentScreen == SCREEN_CHARACTERS )
                        {
                          pMouse->SetCursorBitmap("MICON2");
                        }
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
                        {
                          pMouse->SetCursorBitmap("MICON2");
                        }
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
                    {
                      pMouse->SetCursorBitmap("MICON2");
                    }
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
                {
                  pMouse->SetCursorBitmap("MICON2");
                }
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

              pGUIWindow_CurrentMenu = GUIWindow::Create(0, 0, 640, 480, WINDOW_OptionsButtons, 0, 0);
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
              uNumSeconds = (unsigned int)MakeScreenshot(155, 117);
              pRenderer->SavePCXImage("gamma.pcx", (char *)uNumSeconds, 155, 117);
              free((void *)uNumSeconds);
              stru_506E40._40E55E("gamma.pcx", 0);
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
          pVideoPlayer->Unload();
          DialogueEnding();
          viewparams->bRedrawGameUI = 1;
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
            {
              v38 = dword_5B65BC;
            }
            if ( *dword_591164_teleport_map_name != 48 )
            {
              pGameLoadingUI_ProgressBar->uType = (GUIProgressBar::Type)2;
              dword_5B65C0 = _5B65A8_npcdata_uflags_or_other | _5B65AC_npcdata_fame_or_other | _5B65B0_npcdata_rep_or_other | _5B65B4_npcdata_loword_house_or_other | _5B65B8_npcdata_hiword_house_or_other | v38;
              OnMapLeave();
              sub_44987B(dword_591164_teleport_map_name, MapStartPoint_Party);
            }
          }
          else
          {
            EventProcessor(dword_5C3418, 0, 1, dword_5C341C);
          }
          if ( !_stricmp(byte_6BE3B0.data(), "d05.blv") )
            pParty->uTimePlayed += 1474560i64;
          continue;
        case UIMSG_TransitionWindowCloseBtn:
          CloseWindowBackground();
          pVideoPlayer->Unload();
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
          if ( pRenderer->pRenderD3D )
            LOWORD(v42) = pGame->pVisInstance->get_picked_object_zbuf_val();
          else
          {
            uNumSeconds = (unsigned int)pMouse->GetCursorPos(&v210);
            pPoint = pMouse->GetCursorPos(&v208);
            v42 = pRenderer->pActiveZBuffer[*(int *)uNumSeconds + pSRZBufferLineOffsets[pPoint->y]];
          }
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
              if ( !(pODMFace->uAttributes & 0x02000000) || !pODMFace->sCogTriggeredID )
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
            if ( !(BYTE3(pBLVFace->uAttributes) & 2) )
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
          dword_6BE364_game_settings_1 |= 1;
          uGameState = GAME_STATE_2;
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
          sub_4BD8B5();
          //goto LABEL_434;
          /*if ( (signed int)pMessageQueue_50CBD0->uNumMessages < 40 )
          {
            pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].eType = UIMSG_Escape;
            pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].param = v0;
            *(&pMessageQueue_50CBD0->uNumMessages + 3 * pMessageQueue_50CBD0->uNumMessages + 3) = 0;
            ++pMessageQueue_50CBD0->uNumMessages;
          }*/
          pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, 0);
          continue;

        case UIMSG_OnCastTownPortal:
          pAudioPlayer->StopChannels(-1, -1);
          pGUIWindow_CurrentMenu = GUIWindow::Create(0, 0, 640, 480, WINDOW_Book, WINDOW_TownPortal, (char *)uMessageParam);
        continue;

        case UIMSG_OnCastLloydsBeacon:
          pAudioPlayer->StopChannels(-1, -1);
          pGUIWindow_CurrentMenu = GUIWindow::Create(0, 0, 640, 480, WINDOW_Book, WINDOW_LloydsBeacon, 0);
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
            v173 = pMapStats->pInfos[sub_410D99_get_map_index(pPlayer->pInstalledBeacons[uMessageParam].SaveFileID)].pName;
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
          v174 = pMapStats->pInfos[sub_410D99_get_map_index(*(short *)(uNumSeconds + 26))].pName;
          sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[475], (unsigned int)pMapName, v174);// "Set %s over %s"
          GameUI_SetFooterString(pTmpBuf.data());
          continue;
        case UIMSG_CloseAfterInstallBeacon:
          dword_50CDC8 = 1;
          pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 0, 0);
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
          pAudioPlayer->PlaySound((SoundID)word_4EE088_sound_ids[dword_506338], 0, 0, -1, 0, dword_50633C, 0, 0);
          if ( bRecallingBeacon )
          {
            if ( _stricmp(pCurrentMapName, (const char *)&pGames_LOD->pSubIndices[pPlayer9->pInstalledBeacons[uMessageParam].SaveFileID]) )
            {
              SaveGame(1, 0);
              OnMapLeave();
              strcpy(pCurrentMapName, (const char *)&pGames_LOD->pSubIndices[pPlayer9->pInstalledBeacons[uMessageParam].SaveFileID]);
              dword_6BE364_game_settings_1 |= 1;
              uGameState = GAME_STATE_2;
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
            pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, 0);
            pBooksWindow->Release();
            pGUIWindow_CurrentMenu->Release();
            pBooksWindow = 0;
            pGUIWindow_CurrentMenu = 0;
          }
          else
          {
            sprintf(a1, "data\\lloyd%d%d.pcx", _506348_current_lloyd_playerid + 1, uMessageParam + 1);
            SaveScreenshot(a1);
            LoadThumbnailLloydTexture(uMessageParam, _506348_current_lloyd_playerid + 1);
            pPlayer9->pInstalledBeacons[uMessageParam].uBeaconTime = pParty->uTimePlayed + (signed __int64)((double)(qword_506350 << 7) * 0.033333335);
            pPlayer9->pInstalledBeacons[uMessageParam].PartyPos_X = pParty->vPosition.x;
            pPlayer9->pInstalledBeacons[uMessageParam].PartyPos_Y = pParty->vPosition.y;
            pPlayer9->pInstalledBeacons[uMessageParam].PartyPos_Z = pParty->vPosition.z;
            pPlayer9->pInstalledBeacons[uMessageParam].PartyRot_X = LOWORD(pParty->sRotationY);
            pPlayer9->pInstalledBeacons[uMessageParam].PartyRot_Y = LOWORD(pParty->sRotationX);
            if ( (signed int)pGames_LOD->uNumSubDirs / 2 <= 0 )
              continue;
            for ( thisg = 0; thisg < (signed int)pGames_LOD->uNumSubDirs / 2; ++thisg )
            {
              if ( !_stricmp((const char *)pGames_LOD->pSubIndices[thisg].pFilename, pCurrentMapName) )
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
                    dword_6BE364_game_settings_1 |= 1;
                    uGameState = GAME_STATE_2;
                    strcpy(pCurrentMapName, pMapStats->pInfos[TownPortalList[uMessageParam].uMapInfoID].pFilename);
                    dword_5B65C0 = 1;
                    _5B65A8_npcdata_uflags_or_other = TownPortalList[uMessageParam].pos.x;
                    _5B65AC_npcdata_fame_or_other = TownPortalList[uMessageParam].pos.y;
                    _5B65B0_npcdata_rep_or_other = TownPortalList[uMessageParam].pos.z;
                    v66 = TownPortalList[uMessageParam].rot_x;
                    _5B65B4_npcdata_loword_house_or_other = TownPortalList[uMessageParam].rot_y;
                    _5B65B8_npcdata_hiword_house_or_other = v66;
                    InitializeActors();
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
                  pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, 0);
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
          memcpy(&v216, txt_file_frametable_parser((const char *)pKeyActionMap->pPressedKeysBuffer, &v218), sizeof(v216));
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
                if ( !_stricmp((const char *)&pGames_LOD->pSubIndices[thish], Str2) )
                  break;
                ++thish;
                pNPCData3 = (NPCData *)((char *)pNPCData3 + 1);
              }
              while ( (signed int)pNPCData3 < (signed int)pNPCData4 );
              if ( (signed int)pNPCData3 < (signed int)pNPCData4 )
              {
                strcpy(pCurrentMapName, (const char *)&pGames_LOD->pSubIndices[(int)pNPCData3]);
                dword_6BE364_game_settings_1 |= 1u;
                uGameState = GAME_STATE_2;
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
          if ( pRenderer->pRenderD3D )
          {
            v81 = pGame->pVisInstance->get_picked_object_zbuf_val();
          }
          else
          {
            uNumSeconds = (unsigned int)pMouse->GetCursorPos(&v206);
            pPoint2 = pMouse->GetCursorPos(&v201);
            v81 = pRenderer->pActiveZBuffer[*(int *)uNumSeconds + pSRZBufferLineOffsets[pPoint2->y]];
          }
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
          ptr_507BC8 = GUIWindow::Create(0, 0, 640, 480, WINDOW_68, uMessageParam, 0);
          pCurrentScreen = SCREEN_19;
          pEventTimer->Pause();
          continue;
        case UIMSG_STEALFROMACTOR:
          if ( !uActiveCharacter )
            continue;
          if ( pParty->bTurnBasedModeOn != 1 )
          {
            if ( pActors[uMessageParam].uAIState == 5 )
              stru_50C198.LootActor(&pActors[uMessageParam]);
            else
              Actor::StealFrom(uMessageParam);
            continue;
          }
          if ( pTurnEngine->turn_stage == 1 || pTurnEngine->turn_stage == 3 )
            continue;
          if ( !(pTurnEngine->field_18 & TE_HAVE_PENDING_ACTIONS) )
          {
            if ( pActors[uMessageParam].uAIState == 5 )
              stru_50C198.LootActor(&pActors[uMessageParam]);
            else
              Actor::StealFrom(uMessageParam);
          }
          continue;

        case UIMSG_Attack:
          if ( !uActiveCharacter )
            continue;
          if ( pParty->bTurnBasedModeOn != 1 )
          {
            _42ECB5_PlayerAttacksActor();
            continue;
          }
          if ( pTurnEngine->turn_stage == 1 || pTurnEngine->turn_stage == 3 )
            continue;
          if ( !(pTurnEngine->field_18 & TE_HAVE_PENDING_ACTIONS) )
            _42ECB5_PlayerAttacksActor();
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
          if ( _42F4DA_check_actor_proximity() )
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
            pParty->pPlayers[3].pConditions[2] = pParty->uTimePlayed;
            pParty->pPlayers[2].pConditions[2] = pParty->uTimePlayed;
            pParty->pPlayers[1].pConditions[2] = pParty->uTimePlayed;
            pParty->pPlayers[0].pConditions[2] = pParty->uTimePlayed;
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
                LODWORD(pParty->pPlayers[pPlayerNum].pConditions[2]) = 0;
                HIDWORD(pParty->pPlayers[pPlayerNum].pConditions[2]) = 0;
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
                pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 0, 0);
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
          v99 = quick_spell_at_page + 11 * pPlayers[uActiveCharacter]->lastOpenedSpellbookPage;
          pPlayers[uActiveCharacter]->uQuickSpell = v99;
          stru_AA1058[uActiveCharacter - 1]._494836(v99, uActiveCharacter);
          if ( uActiveCharacter )
            pPlayer10->PlaySound(SPEECH_12, 0);
          byte_506550 = 0;
          continue;
        case UIMSG_SpellBook_PressTab:
			//__debugbreak();
          if ( !uActiveCharacter )
            continue;
          pPlayer3 = pPlayers[uActiveCharacter];
          pNPCData4 = 0;
          uAction = 0;
          v101 = 0;
          thisl = (int)&pPlayer3->pActiveSkills[12];
          do
          {
            if ( *(short *)thisl )
            {
              if ( pPlayer3->lastOpenedSpellbookPage == v101 )
                uAction = (int)pNPCData4;
              v102 = (int)pNPCData4;
              pNPCData4 = (NPCData *)((char *)pNPCData4 + 1);
              v217[v102] = v101;
            }
            thisl += 2;
            ++v101;
          }
          while ( v101 < 9 );
          if ( !pNPCData4 )
          {
            v127 = rand() % 2 + 204;
            pAudioPlayer->PlaySound((SoundID)v127, 0, 0, -1, 0, 0, 0, 0);
            continue;
          }
          if ( GetAsyncKeyState(16) )
          {
            --uAction;
            if ( uAction < 0 )
              uAction = (int)((char *)pNPCData4 - 1);
          }
          else
          {
            ++uAction;
            if ( uAction >= (signed int)pNPCData4 )
              uAction = 0;
          }
          OnCloseSpellBookPage();
          pPlayers[uActiveCharacter]->lastOpenedSpellbookPage = LOBYTE(v217[uAction]);
          pGUIWindow_CurrentMenu->OpenSpellBook();
          v127 = rand() % 2 + 204;
          pAudioPlayer->PlaySound((SoundID)v127, 0, 0, -1, 0, 0, 0, 0);
          continue;
        case UIMSG_OpenSpellbookPage:
          if ( pTurnEngine->turn_stage == 3 || !uActiveCharacter || uMessageParam == pPlayers[uActiveCharacter]->lastOpenedSpellbookPage )
            continue;
          OnCloseSpellBookPage();
          pPlayers[uActiveCharacter]->lastOpenedSpellbookPage = uMessageParam;
          pGUIWindow_CurrentMenu->OpenSpellBook();
          v127 = rand() % 2 + 204;
          pAudioPlayer->PlaySound((SoundID)v127, 0, 0, -1, 0, 0, 0, 0);
          continue;
        case UIMSG_SelectSpell:
        {
          if (pTurnEngine->turn_stage == 3)
            continue;
          if (!uActiveCharacter)
            continue;

            //  uNumSeconds = (unsigned int)pPlayers[uActiveCharacter];
          auto player = pPlayers[uActiveCharacter];
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
                  pMessageQueue_50C9E8->AddMessage(UIMSG_CastSpellFromBook, v103, uActiveCharacter - 1);
                //  pMessageQueue_50CBD0->AddMessage(UIMSG_CastSpellFromBook, v103, uActiveCharacter - 1);
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
          if ( pTurnEngine->turn_stage != 3 )
            _42777D_CastSpell_UseWand_ShootArrow(uMessageParam, v199, 0, 0, 0);
        continue;

        case UIMSG_SpellScrollUse:
			__debugbreak();
          if ( pTurnEngine->turn_stage != 3 )
            _42777D_CastSpell_UseWand_ShootArrow(uMessageParam, v199, 133, 1, 0);
          continue;
        case UIMSG_SpellBookWindow:
          if ( pTurnEngine->turn_stage == 3 )
            continue;
          if ( bUnderwater == 1 )
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
              if ( !pCurrentScreen )
              {
                GUIWindow::Create(0x1DCu, 0x1C2u, 0, 0, WINDOW_PressedButton2, (int)pBtn_CastSpell, 0);
                pCurrentScreen = SCREEN_SPELL_BOOK;
                pEventTimer->Pause();
                pGUIWindow_CurrentMenu = GUIWindow::Create(0, 0, 640, 480, WINDOW_SpellBook, 0, 0);
                pAudioPlayer->PlaySound((SoundID)48, 0, 0, -1, 0, 0, 0, 0);
                viewparams->field_48 = 1;
                continue;
              }
              if ( pCurrentScreen != SCREEN_REST
                && pCurrentScreen != SCREEN_CHARACTERS
                && (pCurrentScreen <= SCREEN_63
                 || pCurrentScreen > SCREEN_67) )
              {
                pGUIWindow_CurrentMenu->Release();
                GUIWindow::Create(0x1DCu, 0x1C2u, 0, 0, WINDOW_PressedButton2, (int)pBtn_CastSpell, 0);
                pCurrentScreen = SCREEN_SPELL_BOOK;
                pEventTimer->Pause();
                pGUIWindow_CurrentMenu = GUIWindow::Create(0, 0, 640, 480, WINDOW_SpellBook, 0, 0);
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
          pGUIWindow_CurrentMenu = GUIWindow::Create(0, 0, 640, 480, WINDOW_QuickReference, 5, 0);
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
          uNumSeconds = (unsigned int)MakeScreenshot(155, 117);
          pRenderer->SavePCXImage("gamma.pcx", (char *)uNumSeconds, 155, 117);
          free((void *)uNumSeconds);
          stru_506E40._40E55E("gamma.pcx", 0);
          GUIWindow::Create(0x25Au, 0x1C2u, 0, 0, WINDOW_PressedButton, (int)pBtn_GameSettings, 0);
//LABEL_453:
          /*if ( (signed int)pMessageQueue_50CBD0->uNumMessages >= 40 )
            continue;
          pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].eType = UIMSG_Escape;
          //goto LABEL_770;
          pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].param = 0;
          *(&pMessageQueue_50CBD0->uNumMessages + 3 * pMessageQueue_50CBD0->uNumMessages + 3) = 0;
          ++pMessageQueue_50CBD0->uNumMessages;*/
          pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 0, 0);
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
          pNPCData4 = (NPCData *)pParty->uCurrentHour;
          if ( (signed int)pParty->uCurrentHour <= 12 )
          {
            if ( !pNPCData4 )
              pNPCData4 = (NPCData *)12;
          }
          else
          {
            pNPCData4 = (NPCData *)((char *)pNPCData4 - 12);
          }
          if ( pParty->uCurrentHour < 0xC || (uNumSeconds = 1, pParty->uCurrentHour >= 0x18) )
            uNumSeconds = 0;
          sprintf(pTmpBuf.data(), "%d:%02d%s %s %d %s %d", pNPCData4, pParty->uCurrentMinute, aAMPMNames[uNumSeconds], aDayNames[pParty->uDaysPlayed % 7],
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
          OnChestLeftClick();
          continue;
        case UIMSG_InventoryLeftClick:
          pPlayers[uActiveCharacter]->OnInventoryLeftClick();
          continue;
        case UIMSG_MouseLeftClickInGame:
          if ( !pRenderer->pRenderD3D )
          {
            if ( pMessageQueue_50CBD0->uNumMessages )
            pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
            OnGameViewportClick();
            continue;
          }
          v115 = pMessageQueue_50CBD0->uNumMessages;
          if ( !pMessageQueue_50CBD0->uNumMessages )
          {
            pMessageQueue_50CBD0->AddMessage(UIMSG_MouseLeftClickInScreen, 0, 0);
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
            pMessageQueue_50CBD0->AddMessage(UIMSG_MouseLeftClickInScreen, 0, 0);
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
          pMessageQueue_50CBD0->AddMessage(UIMSG_MouseLeftClickInScreen, 0, 0);
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
          if ( pRenderer->pRenderD3D )
          {
            LOWORD(v116) = pGame->pVisInstance->get_picked_object_zbuf_val();
          }
          else
          {
            uNumSeconds = (unsigned int)pMouse->GetCursorPos(&v209);
            pPoint3 = pMouse->GetCursorPos(&v204);
            v116 = pRenderer->pActiveZBuffer[*(int *)uNumSeconds + pSRZBufferLineOffsets[pPoint3->y]];
          }
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
    pMessageQueue_50CBD0->AddMessage((UIMessageType)dword_50C9DC, (int)ptr_50C9E0, 0);
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
        pMessageQueue_50CBD0->AddMessage((UIMessageType)_50C9D0_AfterEnchClickEventId, _50C9D4_AfterEnchClickEventSecondParam, 0);
        _50C9D0_AfterEnchClickEventId = 0;
        _50C9D4_AfterEnchClickEventSecondParam = 0;
        _50C9D8_AfterEnchClickEventTimeout = 0;
      }
    }
  }
  CastSpellInfoHelpers::_427E01_cast_spell();
}

//----- (004356FF) --------------------------------------------------------
void back_to_game()
{
  dword_507BF0_is_there_popup_onscreen = 0;
  dword_4E455C = 1;

  extern int no_rightlick_in_inventory;
  no_rightlick_in_inventory = false;

  if ( pGUIWindow_ScrollWindow )
    free_book_subwindow();
  if ( !pCurrentScreen && !pGUIWindow_Settings )
    pEventTimer->Resume();
  viewparams->bRedrawGameUI = 1;
}

//----- (00435748) --------------------------------------------------------
void  GUI_MainMenuMessageProc()
{
  Player *pPlayer; // ebx@2
  void *v3; // edi@21
  signed int v4; // eax@29
  int v5; // ecx@29
  PLAYER_SKILL_TYPE v6; // edi@37
  GUIWindow *pWindow; // eax@56
  GUIButton *pButton; // eax@59
  int v15; // edi@70
  char v20; // dl@116
  unsigned int v21; // eax@116
  unsigned int v25; // eax@120
  unsigned int v26; // ecx@127
  SoundID pSoundID; // [sp-2Ch] [bp-3Ch]@36
  signed int v41; // [sp-10h] [bp-20h]@29
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
      auto player = &pParty->pPlayers[pParam];

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
          pSex = player->GetSexByVoice();
          do
          {
            if (player->uVoiceID == 0)
              player->uVoiceID = 19;
            else --player->uVoiceID;
          }
          while (player->GetSexByVoice() != pSex);
          pButton = pCreationUI_BtnPressLeft2[pParam];
          GUIWindow::Create(pButton->uX, pButton->uY, 0, 0, WINDOW_PressedButton, (int)pButton, (char *)1);
          pAudioPlayer->PlaySound(SOUND_Button, 0, 0, -1, 0, 0, 0, 0);
          player->PlaySound(SPEECH_PickMe, 0);
          break;
        case UIMSG_PlayerCreation_VoiceNext:
          pSex = player->GetSexByVoice();
          do
          {
            player->uVoiceID = (player->uVoiceID + 1) % 20;
          }
          while (player->GetSexByVoice() != pSex);
          pButton = pCreationUI_BtnPressRight2[pParam];
          GUIWindow::Create(pButton->uX, pButton->uY, 0, 0, WINDOW_PressedButton, (int)pButton, (char *)1);
          pAudioPlayer->PlaySound(SOUND_Button, 0, 0, -1, 0, 0, 0, 0);
          player->PlaySound(SPEECH_PickMe, 0);
          break;
        case UIMSG_PlayerCreation_FacePrev:
          pPlayer = &pParty->pPlayers[pParam];
          if (!pPlayer->uCurrentFace)
            pPlayer->uCurrentFace = 19;
          else
            pPlayer->uCurrentFace -= 1;
          pPlayer->uVoiceID = pPlayer->uCurrentFace;
          pPlayer->SetInitialStats();
          pPlayer->SetSexByVoice();
          pPlayer->RandomizeName();
          v25 = pParam;
          pGUIWindow_CurrentMenu->pCurrentPosActiveItem = (pGUIWindow_CurrentMenu->pCurrentPosActiveItem - pGUIWindow_CurrentMenu->pStartingPosActiveItem)
                                                    % 7 + pGUIWindow_CurrentMenu->pStartingPosActiveItem + 7 * pParam;
          uPlayerCreationUI_SelectedCharacter = v25;
          GUIWindow::Create(pCreationUI_BtnPressLeft[v25]->uX, pCreationUI_BtnPressLeft[v25]->uY, 0, 0, WINDOW_PressedButton, (int)pCreationUI_BtnPressLeft[v25], (char *)1);
          pAudioPlayer->PlaySound(SOUND_Button, 0, 0, -1, 0, 0, 0.0, 0);
          pPlayer->PlaySound(SPEECH_PickMe, 0);
          break;
        case UIMSG_PlayerCreation_FaceNext:
          pPlayer = &pParty->pPlayers[pParam];
          v20 = (char)((int)pPlayer->uCurrentFace + 1) % 20;
          pPlayer->uCurrentFace = v20;
          pPlayer->uVoiceID = v20;
          pPlayer->SetInitialStats();
          pPlayer->SetSexByVoice();
          pPlayer->RandomizeName();
          v21 = pParam;
          pGUIWindow_CurrentMenu->pCurrentPosActiveItem = (pGUIWindow_CurrentMenu->pCurrentPosActiveItem - pGUIWindow_CurrentMenu->pStartingPosActiveItem)
                                                    % 7 + pGUIWindow_CurrentMenu->pStartingPosActiveItem + 7 * pParam;
          uPlayerCreationUI_SelectedCharacter = v21;
          GUIWindow::Create(pCreationUI_BtnPressRight[v21]->uX, pCreationUI_BtnPressRight[v21]->uY, 0, 0, WINDOW_PressedButton, (int)pCreationUI_BtnPressRight[v21], (char *)1);
          pAudioPlayer->PlaySound(SOUND_Button, 0, 0, -1, 0, 0, 0, 0);
          pPlayer->PlaySound(SPEECH_PickMe, 0);
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
          pParty->pPlayers[pParam].pActiveSkills[pPlayer[pParam].GetSkillIdxByOrder(2)] = 0;
          break;
        case UIMSG_PlayerCreationRemoveDownSkill:
          v4 = pGUIWindow_CurrentMenu->pCurrentPosActiveItem - pGUIWindow_CurrentMenu->pStartingPosActiveItem;
          pGUIWindow_CurrentMenu->pCurrentPosActiveItem = v4 % 7 + pGUIWindow_CurrentMenu->pStartingPosActiveItem + 7 * pParam;
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
                pMessageQueue_50CBD0->AddMessage(UIMSG_SaveLoadBtn, 0, 0);
                pMessageQueue_50CBD0->AddMessage(UIMSG_LoadGame, 0, 0);
              }
              uLoadGameUI_SelectedSlot = v26;
              dword_6BE138 = v26;
          }
          else
          {
            //typing in the line
              pKeyActionMap->EnterText(0, 19, pGUIWindow_CurrentMenu);
              strcpy((char *)pKeyActionMap->pPressedKeysBuffer, pSavegameHeader[uLoadGameUI_SelectedSlot].pName);
              pKeyActionMap->uNumKeysPressed = strlen((const char *)pKeyActionMap->pPressedKeysBuffer);
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
          if ( !(BYTE1(dword_6BE364_game_settings_1) & 0x40) )
            break;
          v15 = 1;
          pVideoPlayer->bStopBeforeSchedule = 1;
          viewparams->bRedrawGameUI = 1;
          viewparams->field_48 = 1;
          if ( !GetCurrentMenuID() || GetCurrentMenuID() == MENU_CREATEPARTY || GetCurrentMenuID() == MENU_NAMEPANELESC )
          {
            if ( pCurrentScreen == SCREEN_VIDEO )
              pVideoPlayer->FastForwardToFrame(pVideoPlayer->pResetflag);
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
            pMessageQueue_50CBD0->AddMessage(UIMSG_ChangeGameState, 0, 0);
            break;
          }
          if ( GetCurrentMenuID() == MENU_CREDITSPROC && !pCurrentScreen )
          {
            if ( pCurrentScreen == SCREEN_VIDEO )
              pVideoPlayer->FastForwardToFrame(pVideoPlayer->pResetflag);
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
            pMessageQueue_50CBD0->AddMessage(UIMSG_ChangeGameState, 0, 0);
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
            pVideoPlayer->Unload();
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

//----- (0043648F) --------------------------------------------------------
void Vec3_short__to_RenderVertexSoft(RenderVertexSoft *_this, Vec3_short_ *a2)
{
  _this->flt_2C = 0.0;

  _this->vWorldPosition.x = a2->x;
  _this->vWorldPosition.y = a2->y;
  _this->vWorldPosition.z = a2->z;
}

//----- (004369DB) --------------------------------------------------------
void Vec3_float_::Normalize()
{
  this->x = (1.0 / sqrt(this->x * this->x + this->y * this->y + this->z * this->z)) * this->x;
  this->y = (1.0 / sqrt(this->x * this->x + this->y * this->y + this->z * this->z)) * this->y;
  this->z = (1.0 / sqrt(this->x * this->x + this->y * this->y + this->z * this->z)) * this->z;
}

//----- (00438F8F) --------------------------------------------------------
void area_of_effect__damage_evaluate()
{
  int v0; // edx@1
  char *v1; // esi@2
  int v2; // ecx@3
  signed int v3; // eax@3
  SpriteObject *v4; // eax@4
  signed int v5; // eax@4
  signed int v6; // eax@6
  unsigned int v7; // edi@6
  int v8; // eax@6
  __int16 v9; // bx@8
  int v10; // edi@8
  Vec3_int_ v11; // ST04_12@9
  int v12; // ST0C_4@10
  Actor *v13; // edi@11
  int v14; // edx@15
  int v15; // edx@15
  int v16; // ebx@15
  signed int v17; // eax@15
  int v18; // edi@15
  int v19; // edi@15
  Vec3_int_ v20; // ST04_12@16
  Vec3_int_ *v21; // eax@17
  unsigned int v22; // edx@17
  int v23; // edx@18
  int v24; // eax@18
  Vec3_int_ v25; // ST04_12@19
  unsigned int v26; // ebx@20
  Player *v27; // edi@20
  Actor *v28; // edi@27
  int v29; // ebx@29
  int v30; // eax@29
  int v31; // edx@29
  int v32; // eax@29
  int v33; // ST24_4@29
  Vec3_int_ v34; // ST04_12@30
  Vec3_int_ *v35; // eax@31
  SpriteObject *v36; // [sp+0h] [bp-28h]@0
  signed int v37; // [sp+4h] [bp-24h]@5
  int v38; // [sp+4h] [bp-24h]@15
  signed int v39; // [sp+8h] [bp-20h]@3
  int v40; // [sp+8h] [bp-20h]@15
  int v41; // [sp+Ch] [bp-1Ch]@5
  int v42; // [sp+Ch] [bp-1Ch]@15
  int v43; // [sp+10h] [bp-18h]@1
  int v44; // [sp+14h] [bp-14h]@15
  unsigned int uActorID; // [sp+18h] [bp-10h]@6
  Vec3_int_ *pVelocity; // [sp+1Ch] [bp-Ch]@2
  int a1; // [sp+20h] [bp-8h]@8
  int v48; // [sp+24h] [bp-4h]@8

  v0 = 0;
  v43 = 0;
  if ( stru_50FE08.count > 0 )
  {
    pVelocity = &stru_50FE08.vec_4B4;
    v1 = (char *)stru_50FE08.pZs;
    do
    {
      v2 = PID_TYPE(*((short *)v1 - 300));
      v3 = PID_ID((signed int)*((short *)v1 - 300));
      v39 = PID_ID((signed int)*((short *)v1 - 300));
      if ( v2 == 2 )
      {
        v4 = &pSpriteObjects[v3];
        v36 = v4;
        v5 = v4->spell_caster_pid;
        v2 = PID_TYPE(v5);
        v3 = PID_ID(v5);
      }
      v41 = v2;
      v37 = v3;
      if ( stru_50FE08.field_3EC[v0] & 1 )
      {
        v6 = ai_near_actors_targets_pid[v3];
        v7 = PID_ID(v6);
        v8 = PID_TYPE(v6) - 3;
        uActorID = v7;
        if ( v8 )
        {
          if ( v8 == 1 )
          {
            v9 = *(short *)v1;
            v10 = pParty->vPosition.y - *((short *)v1 - 100);
            a1 = pParty->vPosition.x - *((short *)v1 - 200);
            v48 = v10;
            if ( a1 * a1
               + v10 * v10
               + ((signed int)(pParty->vPosition.z + pParty->uPartyHeight) >> (1 - v9))
               * ((signed int)(pParty->vPosition.z + pParty->uPartyHeight) >> (1 - v9)) < (unsigned int)((*((short *)v1 + 100) + 32) * (*((short *)v1 + 100) + 32)) )
            {
              v11.z = *(short *)v1;
              v11.y = *((short *)v1 - 100);
              v11.x = *((short *)v1 - 200);
              if ( sub_407A1C(pParty->vPosition.x, pParty->vPosition.y, pParty->vPosition.z + pParty->sEyelevel, v11) )
              {
                v12 = stru_50C198.which_player_would_attack(&pActors[v39]);
                DamagePlayerFromMonster(*((short *)v1 - 300), stru_50FE08.field_450[v43], pVelocity, v12);
              }
            }
          }
        }
        else
        {
          v13 = &pActors[v7];
          if ( SHIDWORD(v13->pActorBuffs[6].uExpireTime) > 0
            || SHIDWORD(v13->pActorBuffs[6].uExpireTime) >= 0 && LODWORD(v13->pActorBuffs[6].uExpireTime)
            || v13->CanAct() )
          {
            v14 = v13->vPosition.y;
            v40 = v13->vPosition.x;
            v38 = v14;
            v15 = v14 - *((short *)v1 - 100);
            v16 = *(short *)v1;
            v42 = v13->vPosition.z;
            a1 = v40 - *((short *)v1 - 200);
            v17 = v13->uActorHeight;
            v18 = v13->uActorRadius;
            v44 = v42;
            v19 = *((short *)v1 + 100) + v18;
            v48 = v15;
            if ( a1 * a1 + v15 * v15 + (v42 + (v17 >> 1) - v16) * (v42 + (v17 >> 1) - v16) < (unsigned int)(v19 * v19) )
            {
              v20.z = *(short *)v1;
              v20.y = *((short *)v1 - 100);
              v20.x = *((short *)v1 - 200);
              if ( sub_407A1C(v40, v38, v42 + 50, v20) )
              {
                Vec3_int_::Normalize(&a1, &v48, &v44);
                v21 = pVelocity;
                v22 = uActorID;
                pVelocity->x = a1;
                v21->y = v48;
                v21->z = v44;
                sub_43B1B0(*((short *)v1 - 300), v22, v21, stru_50FE08.field_450[v43]);
              }
            }
          }
        }
      }
      else
      {
        v23 = pParty->vPosition.y - *((short *)v1 - 100);
        v24 = ((signed int)pParty->uPartyHeight >> 1) - *(short *)v1;
        a1 = pParty->vPosition.x - *((short *)v1 - 200);
        v48 = v23;
        if ( a1 * a1 + v23 * v23 + (pParty->vPosition.z + v24) * (pParty->vPosition.z + v24) < (unsigned int)((*((short *)v1 + 100) + 32) * (*((short *)v1 + 100) + 32)) )
        {
          v25.z = *(short *)v1;
          v25.y = *((short *)v1 - 100);
          v25.x = *((short *)v1 - 200);
          if ( sub_407A1C(pParty->vPosition.x, pParty->vPosition.y, pParty->vPosition.z + pParty->sEyelevel, v25) )
          {
            v26 = 0;
            v27 = pParty->pPlayers.data();//[0].pConditions[15];
            do
            {
				if ( !(HIDWORD(v27->pConditions[14]) | LODWORD(v27->pConditions[14])) && !v27->pConditions[15] && !v27->pConditions[16] )
                DamagePlayerFromMonster(*((short *)v1 - 300), stru_50FE08.field_450[v43], pVelocity, v26);
              ++v27;
              ++v26;
            }
			while ( v27 <= &pParty->pPlayers[3] );
          }
        }
        uActorID = 0;
        if ( (signed int)uNumActors > 0 )
        {
          v28 = pActors.data();//[0].vPosition.z;
          do
          {
            if ( v28->CanAct() )
            {
              v29 = *(short *)v1;
			  v30 = v28->vPosition.y - *((short *)v1 - 100);
			  a1 = v28->vPosition.x - *((short *)v1 - 200);
			  v31 = v28->vPosition.z;
              v48 = v30;
              v44 = v31;
			  v32 = (v28->uActorHeight >> 1) - v29;
			  v33 = v28->uActorRadius + *((short *)v1 + 100);
              if ( a1 * a1 + v48 * v48 + (v31 + v32) * (v31 + v32) < (unsigned int)(v33 * v33) )
              {
                v34.z = *(short *)v1;
                v34.y = *((short *)v1 - 100);
                v34.x = *((short *)v1 - 200);
				if ( sub_407A1C(v28->vPosition.x, v28->vPosition.y, v28->vPosition.z + 50, v34) )
                {
                  Vec3_int_::Normalize(&a1, &v48, &v44);
                  v35 = pVelocity;
                  pVelocity->x = a1;
                  v35->y = v48;
                  v35->z = v44;
                  switch ( v41 )
                  {
                    case OBJECT_Player:
                      DamageMonsterFromParty(*((short *)v1 - 300), uActorID, v35);
                      break;
                    case OBJECT_Actor:
                      if ( v36 && pActors[v37].GetActorsRelation(v28) )
                        sub_43B1B0(*((short *)v1 - 300), uActorID, pVelocity, v36->field_61);
                      break;
                    case OBJECT_Item:
                      sub_43B057(*((short *)v1 - 300), uActorID, v35);
                      break;
                  }
                }
              }
            }
            ++uActorID;
            ++v28;
          }
          while ( (signed int)uActorID < (signed int)uNumActors );
        }
      }
      ++pVelocity;
      v0 = v43 + 1;
      v1 += 2;
      ++v43;
    }
    while ( v43 < stru_50FE08.count );
  }
  stru_50FE08.count = 0;
}


//----- (0043A97E) --------------------------------------------------------
void __fastcall sub_43A97E(unsigned int uLayingItemID, signed int a2)
{
  if (PID_TYPE(a2) == OBJECT_BLVDoor)
  {
    layingitem_vel_50FDFC.x = pSpriteObjects[uLayingItemID].vVelocity.x;
    layingitem_vel_50FDFC.y = pSpriteObjects[uLayingItemID].vVelocity.y;
    layingitem_vel_50FDFC.z = pSpriteObjects[uLayingItemID].vVelocity.z;

    Vec3_int_::Normalize(&layingitem_vel_50FDFC.x, &layingitem_vel_50FDFC.y, &layingitem_vel_50FDFC.z);
    DamagePlayerFromMonster(PID(OBJECT_Item, uLayingItemID), pSpriteObjects[uLayingItemID].field_61, &layingitem_vel_50FDFC, 0xFFFFFFFFu);
  }
  else if (PID_TYPE(a2) == OBJECT_Actor)
  {
    layingitem_vel_50FDFC.x = pSpriteObjects[uLayingItemID].vVelocity.x;
    layingitem_vel_50FDFC.y = pSpriteObjects[uLayingItemID].vVelocity.y;
    layingitem_vel_50FDFC.z = pSpriteObjects[uLayingItemID].vVelocity.z;

    Vec3_int_::Normalize(&layingitem_vel_50FDFC.x, &layingitem_vel_50FDFC.y, &layingitem_vel_50FDFC.z);
    switch (PID_TYPE(pSpriteObjects[uLayingItemID].spell_caster_pid))
    {
      case OBJECT_Actor:
        sub_43B1B0(PID(OBJECT_Item, uLayingItemID), PID_ID(a2), &layingitem_vel_50FDFC, pSpriteObjects[uLayingItemID].field_61);
        break;
      case OBJECT_Player:
        DamageMonsterFromParty(PID(OBJECT_Item, uLayingItemID), PID_ID(a2), &layingitem_vel_50FDFC);
        break;
      case OBJECT_Item:
        sub_43B057(PID(OBJECT_Item, uLayingItemID), PID_ID(a2), &layingitem_vel_50FDFC);
        break;
    }
  }
}

//----- (0043AE12) --------------------------------------------------------
double __fastcall sub_43AE12(signed int a1)
{
  signed int v1; // ST00_4@1
  signed int v2; // ecx@1
  double v3; // st7@1
  double result; // st7@6

  v1 = a1;
  v2 = 0;
  v3 = (double)v1;
  do
  {
    if ( v3 < flt_4E4A80[v2 + 5] )
      break;
    ++v2;
  }
  while ( v2 < 5 );
  if ( v2 <= 0 || v2 >= 5 )
  {
    if ( v2 )
      result = flt_4E4A80[4];
    else
      result = flt_4E4A80[0];
  }
  else
  {
    result = (flt_4E4A80[v2] - flt_4E4A80[v2 - 1]) * (v3 - flt_4E4A80[v2 + 4]) / (flt_4E4A80[v2 + 5] - flt_4E4A80[v2 + 4]) + flt_4E4A80[v2];
  }
  return result;
}

//----- (0043B057) --------------------------------------------------------
void __fastcall sub_43B057(unsigned int uObjID, unsigned int uActorID, Vec3_int_ *pVelocity)
{
  Actor *pActor; // esi@1
  SpriteObject *v4; // eax@3
  int v5; // ecx@3
  int v6; // eax@4
  int v7; // edi@4
  unsigned int uActorIDa; // [sp+8h] [bp-8h]@1
  unsigned int a2; // [sp+Ch] [bp-4h]@1
  int a2a; // [sp+Ch] [bp-4h]@8

  uActorIDa = uActorID;
  pActor = &pActors[uActorID];
  a2 = uObjID;
  if ( !pActors[uActorID].IsNotAlive() )
  {
    if ( PID_TYPE(a2) == OBJECT_Item)
    {
      v4 = &pSpriteObjects[PID_ID(a2)];
      v5 = v4->spell_id;
      if ( v5 )
      {
        v6 = _43AFE3_calc_spell_damage(v5, v4->spell_level, v4->spell_skill, pActor->sCurrentHP);
        v7 = stru_50C198.CalcMagicalDamageToActor(pActor, 0, v6);
        pActor->sCurrentHP -= v7;
        if ( v7 )
        {
          if ( pActor->sCurrentHP > 0 )
            Actor::AI_Stun(uActorIDa, a2, 0);
          else
            Actor::Die(uActorIDa);
          a2a = 20 * v7 / (signed int)pActor->pMonsterInfo.uHP;
          if ( 20 * v7 / (signed int)pActor->pMonsterInfo.uHP > 10 )
            a2a = 10;
          if ( !MonsterStats::BelongsToSupertype(pActor->pMonsterInfo.uID, MONSTER_SUPERTYPE_TREANT) )
          {
            pVelocity->x = (unsigned __int64)(a2a * (signed __int64)pVelocity->x) >> 16;
            pVelocity->y = (unsigned __int64)(a2a * (signed __int64)pVelocity->y) >> 16;
            pVelocity->z = (unsigned __int64)(a2a * (signed __int64)pVelocity->z) >> 16;
            pActor->vVelocity.x = 50 * LOWORD(pVelocity->x);
            pActor->vVelocity.y = 50 * LOWORD(pVelocity->y);
            pActor->vVelocity.z = 50 * LOWORD(pVelocity->z);
          }
          Actor::AddBloodsplatOnDamageOverlay(uActorIDa, 1, v7);
        }
        else
        {
          Actor::AI_Stun(uActorIDa, a2, 0);
        }
      }
    }
  }
}

//----- (0043B1B0) --------------------------------------------------------
void sub_43B1B0(signed int a1, unsigned int a2, Vec3_int_ *pVelocity, signed int a4)
{
  int v4; // ebx@1
  SpriteObject *v5; // eax@2
  bool v6; // eax@3
  Actor *v7; // esi@4
  Actor *v8; // edi@4
  char v9; // zf@5
  __int64 v10; // qax@8
  signed __int16 v11; // cx@9
  signed int v12; // ecx@20
  int v13; // ecx@22
  int v14; // edi@30
  unsigned int uActorID; // [sp+Ch] [bp-8h]@1
  signed int v17; // [sp+10h] [bp-4h]@1
  int v18; // [sp+20h] [bp+Ch]@34

  v4 = 0;
  uActorID = a2;
  v17 = a1;
  if ( PID_TYPE(a1) == OBJECT_Item)
  {
    v5 = &pSpriteObjects[PID_ID(a1)];
    v4 = v5->field_60_distance_related_prolly_lod;
    v17 = v5->spell_caster_pid;
  }
  LOWORD(v6) = PID_TYPE(v17);
  if ( v6 == OBJECT_Actor)
  {
    v7 = &pActors[a2];
    v8 = &pActors[PID_ID(v17)];
    v6 = pActors[a2].IsNotAlive();
    if ( !v6 )
    {
      v9 = v7->uAIState == 7;
      v7->uLastCharacterIDToHit = v17;
      if ( v9 )
        BYTE2(v7->uAttributes) |= 2u;
      v6 = stru_50C198._4273BB(v8, v7, v4, 0);
      if ( v6 )
      {
        v10 = (unsigned int)v8->_43B3E0_CalcDamage(a4);
        if ( (signed __int64)v8->pActorBuffs[3].uExpireTime > 0 )
        {
          v11 = v8->pActorBuffs[3].uPower;
          if ( v11 )
            v10 = (signed int)v10 / (unsigned __int16)v11;
        }
        if ( SHIDWORD(v7->pActorBuffs[5].uExpireTime) >= SHIDWORD(v10)
          && (SHIDWORD(v7->pActorBuffs[5].uExpireTime) > SHIDWORD(v10)
           || LODWORD(v7->pActorBuffs[5].uExpireTime) > HIDWORD(v10)) )
          LODWORD(v10) = 0;
        if ( a4 != HIDWORD(v10) )
        {
          if ( a4 - HIDWORD(v10) == 1 )
          {
            v12 = v8->pMonsterInfo.uAttack2Type;
            if ( SHIDWORD(v7->pActorBuffs[15].uExpireTime) >= SHIDWORD(v10)
              && (SHIDWORD(v7->pActorBuffs[15].uExpireTime) > SHIDWORD(v10)
               || LODWORD(v7->pActorBuffs[15].uExpireTime) > HIDWORD(v10)) )
              LODWORD(v10) = (signed int)v10 >> 1;
            goto LABEL_30;
          }
          if ( a4 - HIDWORD(v10) == 2 )
          {
            v13 = v7->pMonsterInfo.uSpell1ID;
          }
          else
          {
            if ( a4 - HIDWORD(v10) != 3 )
            {
              if ( a4 - HIDWORD(v10) == 4 )
                v12 = v8->pMonsterInfo.field_3C_some_special_attack;
              else
                v12 = 4;
LABEL_30:
              v14 = stru_50C198.CalcMagicalDamageToActor(v7, v12, v10);
              v7->sCurrentHP -= v14;
              if ( v14 )
              {
                if ( v7->sCurrentHP > 0 )
                  Actor::AI_Stun(uActorID, v17, 0);
                else
                  Actor::Die(uActorID);
                Actor::AggroSurroundingPeasants(uActorID, 0);
                v18 = 20 * v14 / (signed int)v7->pMonsterInfo.uHP;
                if ( 20 * v14 / (signed int)v7->pMonsterInfo.uHP > 10 )
                  v18 = 10;
                if ( !MonsterStats::BelongsToSupertype(v7->pMonsterInfo.uID, MONSTER_SUPERTYPE_TREANT) )
                {
                  pVelocity->x = (unsigned __int64)(v18 * (signed __int64)pVelocity->x) >> 16;
                  pVelocity->y = (unsigned __int64)(v18 * (signed __int64)pVelocity->y) >> 16;
                  pVelocity->z = (unsigned __int64)(v18 * (signed __int64)pVelocity->z) >> 16;
                  v7->vVelocity.x = 50 * LOWORD(pVelocity->x);
                  v7->vVelocity.y = 50 * LOWORD(pVelocity->y);
                  v7->vVelocity.z = 50 * LOWORD(pVelocity->z);
                }
                Actor::AddBloodsplatOnDamageOverlay(uActorID, 1, v14);
              }
              else
              {
                Actor::AI_Stun(uActorID, v17, 0);
              }
              return;
            }
            v13 = v7->pMonsterInfo.uSpell2ID;
          }
          v12 = LOBYTE(pSpellStats->pInfos[v13].uSchool);
          goto LABEL_30;
        }
        v12 = v8->pMonsterInfo.uAttack1Type;
        goto LABEL_30;
      }
    }
  }
}


//----- (0043F515) --------------------------------------------------------
void FindBillboardsLightLevels_BLV()
{
  for (uint i = 0; i < uNumBillboardsToDraw; ++i)
  {
    auto p = &pBillboardRenderList[i];

    if (p->field_1E & 2 || uCurrentlyLoadedLevelType == LEVEL_Indoor && !p->uIndoorSectorID)
      p->dimming_level = 0;
    else
      p->dimming_level = _43F55F_get_billboard_light_level(p, -1);
  }
}

//----- (0043F55F) --------------------------------------------------------
int __fastcall _43F55F_get_billboard_light_level(RenderBillboard *a1, int uBaseLightLevel)
{
  signed int v3; // ecx@2

  if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
  {
    v3 = pIndoor->pSectors[a1->uIndoorSectorID].uMinAmbientLightLevel;
  }
  else
  {
    if ( uBaseLightLevel == -1 )
      v3 = a1->dimming_level;
    else
      v3 = uBaseLightLevel;
  }
  return _43F5C8_get_point_light_level_with_respect_to_lights(v3, a1->uIndoorSectorID, a1->world_x, a1->world_y, a1->world_z);
}

//----- (0043F5C8) --------------------------------------------------------
int __fastcall _43F5C8_get_point_light_level_with_respect_to_lights(unsigned int uBaseLightLevel, int uSectorID, float x, float y, float z)
{
  int v5; // esi@1
  signed int v6; // edi@1
  int v8; // eax@6
  int v9; // ebx@6
  unsigned int v10; // ecx@6
  unsigned int v11; // edx@9
  unsigned int v12; // edx@11
  signed int v13; // ecx@12
  BLVLightMM7 *v16; // esi@20
  int v17; // ebx@21
  int v18; // eax@24
  int v19; // ebx@24
  unsigned int v20; // ecx@24
  int v21; // edx@25
  unsigned int v22; // edx@27
  unsigned int v23; // edx@29
  signed int v24; // ecx@30
  int v26; // ebx@35
  int v27; // eax@38
  int v28; // ebx@38
  unsigned int v29; // ecx@38
  int v30; // edx@39
  unsigned int v31; // edx@41
  unsigned int v32; // edx@43
  signed int v33; // ecx@44
  int v37; // [sp+Ch] [bp-18h]@37
  int v38; // [sp+10h] [bp-14h]@5
  int v39; // [sp+10h] [bp-14h]@23
  int v40; // [sp+10h] [bp-14h]@36
  int v42; // [sp+14h] [bp-10h]@22
  unsigned int v43; // [sp+18h] [bp-Ch]@12
  unsigned int v44; // [sp+18h] [bp-Ch]@30
  unsigned int v45; // [sp+18h] [bp-Ch]@44

  v6 = uBaseLightLevel;
  for (uint i = 0; i < pMobileLightsStack->uNumLightsActive; ++i)
  {
    auto p = pMobileLightsStack->pLights + i;

      auto distX = abs(p->vPosition.x - x);
      if ( distX <= p->uRadius)
      {
        auto distY = abs(p->vPosition.y - y);
        if ( distY <= p->uRadius)
        {
          auto distZ = abs(p->vPosition.z - z);
          if ( distZ <= p->uRadius)
          {
            v8 = distX;
            v9 = distY;
            v10 = distZ;
            if (distX < distY)
            {
              v8 = distY;
              v9 = distX;
            }
            if ( v8 < distZ )
            {
              v11 = v8;
              v8 = distZ;
              v10 = v11;
            }
            if ( v9 < (signed int)v10 )
            {
              v12 = v10;
              v10 = v9;
              v9 = v12;
            }
            v43 = ((unsigned int)(11 * v9) / 32) + (v10 / 4) + v8;
            v13 = p->uRadius;
            if ( (signed int)v43 < v13 )
              v6 += ((unsigned __int64)(30i64 * (signed int)(v43 << 16) / v13) >> 16) - 30;
          }
        }
      }
  }

  if ( uCurrentlyLoadedLevelType == LEVEL_Indoor)
  {
    auto pSector = pIndoor->pSectors + uSectorID;

    for (uint i = 0; i < pSector->uNumLights; ++i)
    {
        v16 = pIndoor->pLights + pSector->pLights[i];
        if (~v16->uAtributes & 8)
        {
          v17 = abs(v16->vPosition.x - x);
          if ( v17 <= v16->uRadius )
          {
            v42 = abs(v16->vPosition.y - y);
            if ( v42 <= v16->uRadius )
            {
              v39 = abs(v16->vPosition.z - z);
              if ( v39 <= v16->uRadius )
              {
                v18 = v17;
                v19 = v42;
                v20 = v39;
                if ( v18 < v42 )
                {
                  v21 = v18;
                  v18 = v42;
                  v19 = v21;
                }
                if ( v18 < v39 )
                {
                  v22 = v18;
                  v18 = v39;
                  v20 = v22;
                }
                if ( v19 < (signed int)v20 )
                {
                  v23 = v20;
                  v20 = v19;
                  v19 = v23;
                }
                v44 = ((unsigned int)(11 * v19) >> 5) + (v20 >> 2) + v18;
                v24 = v16->uRadius;
                if ( (signed int)v44 < v24 )
                  v6 += ((unsigned __int64)(30i64 * (signed int)(v44 << 16) / v24) >> 16) - 30;
              }
            }
          }
        }
    }
  }

  for (uint i = 0; i < pStationaryLightsStack->uNumLightsActive; ++i)
  {
    auto p = pStationaryLightsStack->pLights + i;
      v26 = abs(p->vPosition.x - x);
      if ( v26 <= p->uRadius)
      {
        v40 = abs(p->vPosition.y - y);
        if ( v40 <= p->uRadius)
        {
          v37 = abs(p->vPosition.z - z);
          if ( v37 <= p->uRadius)
          {
            v27 = v26;
            v28 = v40;
            v29 = v37;
            if ( v27 < v40 )
            {
              v30 = v27;
              v27 = v40;
              v28 = v30;
            }
            if ( v27 < v37 )
            {
              v31 = v27;
              v27 = v37;
              v29 = v31;
            }
            if ( v28 < (signed int)v29 )
            {
              v32 = v29;
              v29 = v28;
              v28 = v32;
            }
            v45 = ((unsigned int)(11 * v28) >> 5) + (v29 >> 2) + v27;
            v33 = p->uRadius;
            if ( (signed int)v45 < v33 )
              v6 += ((unsigned __int64)(30i64 * (signed int)(v45 << 16) / v33) >> 16) - 30;
          }
        }
      }
  }

  if ( v6 <= 31 )
  {
    if ( v6 < 0 )
      v6 = 0;
  }
  else
  {
    v6 = 31;
  }
  return v6;
}


//----- (004070EF) --------------------------------------------------------
bool __fastcall sub_4070EF_prolly_detect_player(unsigned int uObjID, unsigned int uObj2ID)
{
  signed int v2; // eax@1
  int v5; // ecx@2
  int obj1_sector; // eax@4
  float v8; // ST24_4@5
  double v9; // ST18_8@5
  int v11; // ecx@6
  signed int v12; // eax@7
  int v13; // esi@7
  int v14; // esi@8
  int v15; // esi@9
  int obj2_z; // edi@11
  int obj2_x; // esi@11
  int obj2_sector; // eax@13
  float v20; // ST24_4@14
  double v21; // ST18_8@14
  int dist_x; // ebx@16
  signed int dist_3d; // ecx@16
  int v25; // eax@18
  BLVFace *v29; // ebx@32
  Vec3_short_ *v30; // esi@32
  int v31; // eax@32
  int v32; // ST50_4@44
  int v33; // ST54_4@44
  int v34; // eax@44
  signed int v38; // esi@45
  signed __int64 v40; // qtt@50
  __int16 next_sector; // bx@58
  int v43; // [sp-8h] [bp-70h]@11
  int v44; // [sp-4h] [bp-6Ch]@11
  int v47; // [sp+18h] [bp-50h]@20
  int v48; // [sp+1Ch] [bp-4Ch]@20
  int v49; // [sp+20h] [bp-48h]@20
  int dist_z; // [sp+24h] [bp-44h]@16
  signed int higher_z; // [sp+24h] [bp-44h]@27
  signed int lower_z; // [sp+28h] [bp-40h]@26
  signed int higher_y; // [sp+2Ch] [bp-3Ch]@23
  signed int lower_y; // [sp+30h] [bp-38h]@22
  signed int higher_x; // [sp+34h] [bp-34h]@21
  signed int lower_x; // [sp+38h] [bp-30h]@20
  signed int sectors_visited; // [sp+3Ch] [bp-2Ch]@28
  int v58; // [sp+44h] [bp-24h]@50
  int v59; // [sp+48h] [bp-20h]@44
  int obj2_y; // [sp+50h] [bp-18h]@11
  int obj1_x; // [sp+58h] [bp-10h]@4
  int obj1_y; // [sp+5Ch] [bp-Ch]@4
  int obj1_z; // [sp+60h] [bp-8h]@4
  int current_sector; // [sp+64h] [bp-4h]@7
  int dist_y;
  int v70;

  v2 = PID_ID(uObjID);
  switch( PID_TYPE(uObjID) )
  {
	case OBJECT_Decoration:
      obj1_x = pLevelDecorations[v2].vPosition.x;
      obj1_y = pLevelDecorations[v2].vPosition.y;
      obj1_z = pLevelDecorations[v2].vPosition.z;
      obj1_sector = pIndoor->GetSector(obj1_x, obj1_y, obj1_z);
	  break;
	case OBJECT_Actor:
      obj1_x = pActors[v2].vPosition.x;
      obj1_y = pActors[v2].vPosition.y;
      v8 = (double)pActors[v2].uActorHeight * 0.69999999;
      //v9 = v8 + 6.7553994e15;
      //obj1_z = LODWORD(v9) + pActors[v2].vPosition.z;
	  obj1_z = (int)v8 + pActors[v2].vPosition.z;
      obj1_sector = pActors[v2].uSectorID;
	  break;
	case OBJECT_Item:
      obj1_x = pSpriteObjects[v2].vPosition.x;
      obj1_y = pSpriteObjects[v2].vPosition.y;
      obj1_z = pSpriteObjects[v2].vPosition.z;
      obj1_sector = pSpriteObjects[v2].uSectorID;
	  break;
	default:
	  return 0;
  }
  v12 = PID_ID(uObj2ID);
  switch( PID_TYPE(uObj2ID) )
  {
    case OBJECT_Decoration:
      obj2_z = pLevelDecorations[v12].vPosition.z;
      obj2_x = pLevelDecorations[v12].vPosition.x;
      obj2_y = pLevelDecorations[v12].vPosition.y;
	  obj2_sector = pIndoor->GetSector(obj2_x, obj2_y, obj2_z);
	  break;
	case OBJECT_Player:
      obj2_x = pParty->vPosition.x;
      obj2_z = pParty->sEyelevel + pParty->vPosition.z;
      obj2_y = pParty->vPosition.y;
	  obj2_sector = pIndoor->GetSector(obj2_x, obj2_y, obj2_z);
      break;
	case OBJECT_Actor:
      obj2_y = pActors[v12].vPosition.y;
      obj2_x = pActors[v12].vPosition.x;
      v20 = (double)pActors[v12].uActorHeight * 0.69999999;
      //v21 = v20 + 6.7553994e15;
      //obj2_z = LODWORD(v21) + pActors[v12].vPosition.z;
	  obj2_z = (int)v20 + pActors[v12].vPosition.z;
      obj2_sector = pActors[v12].uSectorID;
	  break;
	case OBJECT_Item:
      obj2_x = pSpriteObjects[v12].vPosition.x;
      obj2_z = pSpriteObjects[v12].vPosition.z;
      obj2_y = pSpriteObjects[v12].vPosition.y;
      obj2_sector = pSpriteObjects[v12].uSectorID;
	  break;
	default:
	  return 0;
  }
  dist_x = obj2_x - obj1_x;
  dist_z = obj2_z - obj1_z;
  dist_y = obj2_y - obj1_y;
  dist_3d = integer_sqrt(dist_x * dist_x + dist_y * dist_y + dist_z * dist_z);
  //range check
  if ( dist_3d > 5120 )
    return 0;
  if ( uCurrentlyLoadedLevelType == LEVEL_Outdoor)
    return 1;
  v25 = 65536;
  if ( dist_3d )
    v25 = 65536 / dist_3d;
  v49 = dist_x * v25;
  v47 = dist_z * v25;
  v48 = dist_y * v25;
  if ( obj1_x < obj2_x )
  {
	lower_x = obj1_x;
    higher_x = obj2_x;
  }
  else
  {
    lower_x = obj2_x;
    higher_x = obj1_x;
  }
  if ( obj1_y < obj2_y )
  {
	lower_y = obj1_y;
    higher_y = obj2_y;
  }
  else
  {
    lower_y = obj2_y;
    higher_y = obj1_y;
  }
  if ( obj1_z < obj2_z )
  {
	lower_z = obj1_z;
    higher_z = obj2_z;
  }
  else
  {
    lower_z = obj2_z;
    higher_z = obj1_z;
  }
  sectors_visited = 0;
  //monster in same sector with player
  if ( obj1_sector == obj2_sector )
      return 1;
  //search starts from monster
  current_sector = obj1_sector;
  for( int current_portal = 0; current_portal < pIndoor->pSectors[current_sector].uNumPortals; current_portal++ )
  {
	v29 = &pIndoor->pFaces[pIndoor->pSectors[current_sector].pPortals[current_portal]];
	v30 = &pIndoor->pVertices[*v29->pVertexIDs];
	v31 = v29->pFacePlane_old.vNormal.z * (v30->z - obj1_z)
		+ v29->pFacePlane_old.vNormal.y * (v30->y - obj1_y)
		+ v29->pFacePlane_old.vNormal.x * (v30->x - obj1_x);

	if ( current_sector != v29->uSectorID )
		v31 = -v31;

	if ( v31 >= 0 && v30->x != obj1_x && v30->y != obj1_y && v30->z != obj1_z)
		continue;

	if(	lower_x > v29->pBounding.x2
		|| higher_x < v29->pBounding.x1
		|| lower_y > v29->pBounding.y2
		|| higher_y < v29->pBounding.y1
		|| lower_z > v29->pBounding.z2
		|| higher_z < v29->pBounding.z1 )
	{
		continue;
	}
	  
	v32 = fixpoint_mul(v29->pFacePlane_old.vNormal.x,v49);
	v33 = fixpoint_mul(v29->pFacePlane_old.vNormal.z,v47);
	v34 = fixpoint_mul(v29->pFacePlane_old.vNormal.y,v48);
	v59 = v32 + v33 + v34;
	if ( v59 )
	{
		v70 = v29->pFacePlane_old.dist 
			+ obj1_z * v29->pFacePlane_old.vNormal.z 
			+ obj1_x * v29->pFacePlane_old.vNormal.x 
			+ obj1_y * v29->pFacePlane_old.vNormal.y;
		v38 = -v70;

		// if ( v59 <= 0 ^ v70 <= 0 )
		
		/* TEMPORARY
		if ( v59 <= 0 && v70 <= 0 )
		{
			continue;
		}
		if ( !(v59 <= 0 && v70 <= 0) )
		{
			continue;
		}
		*/

		if( abs(v38) >> 14 > abs(v59) )
			continue;

		v58 = fixpoint_div(v38,v59);

		if( v58 < 0 )
		{
			continue;
		}

		if(!sub_4075DB(
				obj1_x + ((fixpoint_mul(v49,v58) + 32768) >> 16),
				obj1_y + ((fixpoint_mul(v48,v58) + 32768) >> 16),
				obj1_z + ((fixpoint_mul(v47,v58) + 32768) >> 16),
				v29) )
		{
			continue;
		}

		//if there is no next sector turn back
		if ( v29->uSectorID == current_sector )
			next_sector = v29->uBackSectorID;
		else
			next_sector = v29->uSectorID;

		//no more portals, quit
		if ( next_sector == current_sector )
		{
			break;
		}

		++sectors_visited;
		current_sector = next_sector;

		//found player, quit
		if ( next_sector == obj2_sector )
			return 1;

		current_sector = next_sector;

		//did we hit limit for portals?
		//does the next room have portals?
		if ( sectors_visited < 30 && pIndoor->pSectors[current_sector].uNumPortals > 0)
		{
				current_portal=-1;
				continue;
		}
		else
			break;
	}
  }
  //did we stop in the sector where player is?
  if ( current_sector != obj2_sector )
    return 0;
  return 1;
}

//----- (004075DB) --------------------------------------------------------
bool __fastcall sub_4075DB(int x, int y, int z, BLVFace *a4)
{
  unsigned int v5; // esi@1
  char v7; // zf@2
  int v8; // edi@2
  Vec3_short_ *v9; // ecx@3
  int v10; // edx@4
  Vec3_short_ *v12; // ecx@8
  int v13; // edx@9
  Vec3_short_ *v15; // ecx@12
  int v16; // edx@13
  bool v18; // esi@14
  int v19; // ecx@14
  int v20; // ecx@16
  int v21; // edx@16
  int v22; // eax@17
  signed int v23; // ebx@18
  int v24; // esi@20
  signed int v25; // eax@22
  signed __int64 v26; // qtt@22
  bool result; // eax@25
  int v29; // [sp+10h] [bp-Ch]@14
  signed int a3a; // [sp+24h] [bp+8h]@14
  int a4a; // [sp+28h] [bp+Ch]@2

  //__debugbreak();

  v5 = a4->uAttributes;
  v9 = pIndoor->pVertices;
  if ( v5 & FACE_XY_PLANE )
  {
    a4a = x;
    v8 = y;
    for(int i = 0; i < a4->uNumVertices; i++)
	{
        dword_4F5D98_xs[i] = v9[a4->pVertexIDs[i]].x;
        dword_4F5CC8_ys[i] = v9[a4->pVertexIDs[i]].y;
	}
  }
  else
  {
    v8 = z;
    if ( v5 & FACE_XZ_PLANE )
    {
      a4a = x;
      for(int i = 0; i < a4->uNumVertices; i++)
	  {
		dword_4F5D98_xs[i] = v9[a4->pVertexIDs[i]].x;
		dword_4F5CC8_ys[i] = v9[a4->pVertexIDs[i]].z;
	  }
    }
    else
    {
      a4a = y;
      for(int i = 0; i < a4->uNumVertices; i++)
	  {
		dword_4F5D98_xs[i] = v9[a4->pVertexIDs[i]].y;
		dword_4F5CC8_ys[i] = v9[a4->pVertexIDs[i]].z;
	  }
    }
  }
  a3a = 0;
  dword_4F5D98_xs[a4->uNumVertices] = dword_4F5D98_xs[0];
  dword_4F5CC8_ys[a4->uNumVertices] = dword_4F5CC8_ys[0];
  for(int i = 0; i < a4->uNumVertices && a3a < 2; i++)
  {
    if ( dword_4F5CC8_ys[i] >= v8 ^ (dword_4F5CC8_ys[i + 1] >= v8) )
    {
	  //if( dword_4F5D98_xs[i + 1] >= a4a || dword_4F5D98_xs[i] >= a4a)
	  if( !(dword_4F5D98_xs[i + 1] >= a4a && dword_4F5D98_xs[i] < a4a))
      {
		  if ( (dword_4F5D98_xs[i + 1] < a4a && dword_4F5D98_xs[i] >= a4a) )
			  ++a3a;
		  //|| (v25 = dword_4F5D98_xs[i + 1] - dword_4F5D98_xs[i],LODWORD(v26) = v25 << 16, HIDWORD(v26) = v25 >> 16, 
		  //dword_4F5D98_xs[i] + ((signed int)(((unsigned __int64)(v26 / (dword_4F5CC4_ys[i + 2] - dword_4F5CC4_ys[i + 1])* ((v8 - dword_4F5CC4_ys[i + 1]) << 16)) >> 16)
          //                + 32768) >> 16) >= a4a) )
		  else
		  {
			v25 = fixpoint_div(dword_4F5D98_xs[i + 1] - dword_4F5D98_xs[i], dword_4F5CC8_ys[i + 1] - dword_4F5CC8_ys[i]);
			if( dword_4F5D98_xs[i] + (fixpoint_mul(v25, (v8 - dword_4F5CC8_ys[i]) << 16) + 32768 >> 16) >= a4a)
				++a3a;
		  }
      }
    }
  }
  result = 1;
  if ( a3a != 1 )
    result = 0;
  return result;
}

//----- (004077F1) --------------------------------------------------------
bool __fastcall sub_4077F1(int a1, int a2, int a3, ODMFace *a4, BSPVertexBuffer *a5)
{
  //ODMFace *v5; // eax@1
  Vec3_int_ *v9;
  unsigned int v5; // esi@1
  signed int v7; // edi@1
  //char v8; // zf@2
  //unsigned __int16 *v9; // edx@3
  int v10; // ecx@4
  unsigned __int16 *v11; // edx@8
  int v12; // ecx@9
  int v13; // edx@12
  int v14; // ecx@13
  int v15; // esi@14
  bool v16; // edi@14
  int v17; // ecx@16
  int v18; // edx@16
  int v19; // eax@17
  signed int v20; // ebx@18
  int v21; // edi@20
  signed int v22; // eax@22
  signed __int64 v23; // qtt@22
  //bool result; // eax@25
  //int v25; // [sp+14h] [bp-8h]@14
  int a4a; // [sp+28h] [bp+Ch]@2
  signed int a5a; // [sp+2Ch] [bp+10h]@14

  __debugbreak();

  v5 = a4->uAttributes;
  v9 = a5->pVertices;
  if ( v5 & FACE_XY_PLANE )
  {
    a4a = a1;
    a3 = a2;
    for(int i = 0; i < a4->uNumVertices; i++)
	{
        dword_4F5BF4_xs[i+1] = v9[a4->pVertexIDs[i]].x;
        dword_4F5B24_ys[i+1] = v9[a4->pVertexIDs[i]].y;
	}
  }
  else
  {
    if ( v5 & FACE_XY_PLANE )
    {
      a4a = a1;
      for(int i = 0; i < a4->uNumVertices; i++)
	  {
		dword_4F5BF4_xs[i+1] = v9[a4->pVertexIDs[i]].x;
		dword_4F5B24_ys[i+1] = v9[a4->pVertexIDs[i]].z;
	  }
    }
    else
    {
      a4a = a2;
      for(int i = 0; i < a4->uNumVertices; i++)
	  {
		dword_4F5BF4_xs[i+1] = v9[a4->pVertexIDs[i]].y;
		dword_4F5B24_ys[i+1] = v9[a4->pVertexIDs[i]].z;
	  }
    }
  }
  a5a = 0;
  dword_4F5BF4_xs[a4->uNumVertices + 1] = dword_4F5BF4_xs[1];
  dword_4F5B24_ys[a4->uNumVertices + 1] = dword_4F5B24_ys[1];
  for(int i = 0; i < a4->uNumVertices; i++)
  {
    if ( a5a >= 2 )
      break;
    if ( dword_4F5B24_ys[i + 1] >= a3 ^ (dword_4F5B24_ys[i + 2] >= a3) )
    {
	  if( dword_4F5BF4_xs[i + 2] >= a4a || dword_4F5BF4_xs[i] >= a4a)
      {
		  if (dword_4F5BF4_xs[i + 2] >= a4a && dword_4F5BF4_xs[i + 1] >= a4a)
            ++a5a;
          else
          {
              v23 = (__int64)(dword_4F5BF4_xs[i + 2] - dword_4F5BF4_xs[i + 1]) << 16;
              __int64 _a = dword_4F5B24_ys[i + 2] - dword_4F5B24_ys[i + 1];
              __int64 _b = (__int64)(a3 - dword_4F5B24_ys[i + 1]) << 16;

              if (dword_4F5BF4_xs[i + 1] + ((((v23 / _a * _b) >> 16) + 32768) >> 16) >= a4a)
                ++a5a;
          }
	  }
    }
  }

  if ( a5a != 1 )
    return false;
  return true;

}

//----- (004088E9) --------------------------------------------------------
int __fastcall sub_4088E9(int x1, int y1, int x2, int y2, int x3, int y3)
{
  int v8; // eax@1
  signed int result; // eax@1
  int v11; // [sp+18h] [bp+8h]@1

  v11 = abs(x2 - x1);
  v8 = abs(y2 - y1);
  result = integer_sqrt(v11 * v11 + v8 * v8);
  if ( result )
    result = abs(((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / result);
  return result;
}

//----- (0040DEF3) --------------------------------------------------------
unsigned short TargetColor( unsigned __int16 r, unsigned __int16 g, unsigned __int16 b )
{
  return ((unsigned int)b >> (8 - LOBYTE(pRenderer->uTargetBBits))) | pRenderer->uTargetGMask & (g << (LOBYTE(pRenderer->uTargetGBits) + 
                           LOBYTE(pRenderer->uTargetBBits) - 8)) | pRenderer->uTargetRMask & (r << (LOBYTE(pRenderer->uTargetGBits) + 
                           LOBYTE(pRenderer->uTargetRBits) + LOBYTE(pRenderer->uTargetBBits) - 8));
}



//----- (0040F82D) --------------------------------------------------------
void __fastcall ZBuffer_Fill(int *pZBuffer, int uTextureId, int iZValue)
{
  assert(uTextureId != -1);
  ZBuffer_DoFill(pZBuffer, pIcons_LOD->GetTexture(uTextureId), iZValue);
}



//----- (0040F89C) --------------------------------------------------------
void __fastcall ZBuffer_DoFill(int *pZBuffer, Texture *pTex, int uZValue)
{
  void *v3; // eax@3
  void *v4; // esi@5
  int *v5; // edi@5
  int v6; // eax@5
  int v7; // ecx@6
  Texture *v8; // [sp+Ch] [bp-14h]@1
  int *v9; // [sp+10h] [bp-10h]@1
  int v10; // [sp+14h] [bp-Ch]@1
  int v11; // [sp+18h] [bp-8h]@1
  void *v12; // [sp+1Ch] [bp-4h]@5

  v10 = pTex->uTextureWidth;
  v8 = pTex;
  v9 = pZBuffer;
  v11 = pTex->uTextureHeight;
  if ( pIcons_LOD->dword_011BA4 && pTex->uDecompressedSize )
    v3 = pTex->UnzipPalette();
  else
    v3 = pTex->pLevelOfDetail0_prolly_alpha_mask;
  v12 = v3;
  v4 = v3;
  v5 = v9;
  v6 = 0;
  do
  {
    v7 = v10;
    do
    {
      LOBYTE(v6) = *(char *)v4;
      v4 = (char *)v4 + 1;
      if ( v6 )
        *v5 = uZValue;
      ++v5;
      --v7;
    }
    while ( v7 );
    v5 += 640 - v10;
    --v11;
  }
  while ( v11 );
  if ( pIcons_LOD->dword_011BA4 )
  {
    if ( v8->uDecompressedSize )
      free(v12);
  }
}

//----- (0040F92A) --------------------------------------------------------
void __fastcall ZBuffer_DoFill2(int *pZBuffer, Texture *a2, int a3)
{
  Texture *v3; // esi@1
  void *v4; // eax@3
  int *v5; // edi@5
  int v6; // ecx@6
  int *v7; // [sp+Ch] [bp-10h]@1
  int v8; // [sp+10h] [bp-Ch]@1
  int v9; // [sp+18h] [bp-4h]@1

  v3 = a2;
  v7 = pZBuffer;
  v8 = a2->uTextureWidth;
  v9 = a2->uTextureHeight;
  if ( pIcons_LOD->dword_011BA4 && a2->uDecompressedSize )
    v4 = a2->UnzipPalette();
  else
    v4 = a2->pLevelOfDetail0_prolly_alpha_mask;
  v5 = v7;
  do
  {
    v6 = v8;
    do
    {
      *v5 = a3;
      ++v5;
      --v6;
    }
    while ( v6 );
    v5 += 640 - v8;
    --v9;
  }
  while ( v9 );
  if ( pIcons_LOD->dword_011BA4 )
  {
    if ( v3->uDecompressedSize )
      free(v4);
  }
}

//----- (00410D99) --------------------------------------------------------
int __fastcall sub_410D99_get_map_index(int a1)
{
  int v1; // edi@1
  signed int v2; // ebp@1
  const char **v3; // esi@2
  char *v4; // ebx@2

  v1 = a1;
  v2 = 1;
  if ( _stricmp(pMapStats->pInfos[1].pFilename, (const char *)&pGames_LOD->pSubIndices[a1]) )
  {
    v3 = (const char **)&pMapStats->pInfos[1].pFilename;
    v4 = (char *)&pMapStats->pInfos[1].pFilename;
    do
    {
      if ( !*(int *)v4 )
        break;
      v3 += 17;
      ++v2;
      v4 = (char *)v3;
    }
    while ( _stricmp(*v3, (const char *)&pGames_LOD->pSubIndices[v1]) );
  }
  return v2;
}

// 4E28F8: using guessed type int pCurrentScreen;

void LoadFonts_and_DrawCopyrightWindow()
{
  MainMenuUI_LoadFontsAndSomeStuff();
  DrawCopyrightWindow();
}

//----- (00417AD4) --------------------------------------------------------
unsigned int GetSkillColor(unsigned int uPlayerClass, PLAYER_SKILL_TYPE uPlayerSkillType, signed int skill_level)
{  
  switch (uPlayerClass % 4)
  {
    case 0:
    {
      if (byte_4ED970_skill_learn_ability_by_class_table[uPlayerClass][uPlayerSkillType] >= skill_level)
        return ui_character_skillinfo_can_learn;
      if (byte_4ED970_skill_learn_ability_by_class_table[uPlayerClass + 1][uPlayerSkillType] < skill_level &&
          byte_4ED970_skill_learn_ability_by_class_table[uPlayerClass + 2][uPlayerSkillType] < skill_level)
      {
        if (byte_4ED970_skill_learn_ability_by_class_table[uPlayerClass + 3][uPlayerSkillType] < skill_level)
          return ui_character_skillinfo_cant_learn;
      }
      return ui_character_skillinfo_can_learn_gm;
    }
    break;

    case 1:
    {
      if (byte_4ED970_skill_learn_ability_by_class_table[uPlayerClass][uPlayerSkillType] >= skill_level)
        return ui_character_skillinfo_can_learn;
      if (byte_4ED970_skill_learn_ability_by_class_table[uPlayerClass + 1][uPlayerSkillType] < skill_level)
      {
        if (byte_4ED970_skill_learn_ability_by_class_table[uPlayerClass + 2][uPlayerSkillType] < skill_level)
          return ui_character_skillinfo_cant_learn;
      }
      return ui_character_skillinfo_can_learn_gm;
    }
    break;

    case 2:
    case 3:
    {
      if (byte_4ED970_skill_learn_ability_by_class_table[uPlayerClass][uPlayerSkillType] < skill_level)
        return ui_character_skillinfo_cant_learn;
      return ui_character_skillinfo_can_learn;
    }
    break;
  }
  Error("Invalid player class: %u", uPlayerClass);
}