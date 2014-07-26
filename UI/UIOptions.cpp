#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define _CRT_SECURE_NO_WARNINGS
#include "..\MM7.h"

#include "..\Keyboard.h"
#include "..\IndoorCameraD3D.h"
#include "..\GammaControl.h"
#include "..\Render.h"

#include "..\Game.h"
#include "..\GUIWindow.h"
#include "..\GUIFont.h"
#include "..\AudioPlayer.h"
#include "..\LOD.h"
#include "..\texts.h"

#include "..\mm7_data.h"




OptionsMenuSkin options_menu_skin; // 507C60


std::array<bool, 28> GameMenuUI_InvaligKeyBindingsFlags; // 506E6C
//----- (00414D24) --------------------------------------------------------
static unsigned int GameMenuUI_GetKeyBindingColor(int key_index)
{
  if (uGameMenuUI_CurentlySelectedKeyIdx == key_index)
  {
    if (GetTickCount() % 1000 < 500)
      return ui_gamemenu_keys_key_selection_blink_color_1;
    else
      return ui_gamemenu_keys_key_selection_blink_color_2;
  }
  else if (GameMenuUI_InvaligKeyBindingsFlags[key_index])
  {
    int intensity;

    int time = GetTickCount() % 800;
    if (time < 400)
      intensity = - 70 + 70 * time / 400;
    else
      intensity = + 70 - 70 * time / 800;

    return Color16(185 + intensity, 40 + intensity / 4, 40 + intensity / 4);
  }

  return ui_gamemenu_keys_key_default_color;
}

//----- (004142D3) --------------------------------------------------------
void GameMenuUI_DrawKeyBindings()
{
  signed int v4; // ecx@7
  signed int v5; // eax@8
  
  if ( pGUIWindow_CurrentMenu->receives_keyboard_input_2 == WINDOW_INPUT_CONFIRMED)
  {
    pPrevVirtualCidesMapping[uGameMenuUI_CurentlySelectedKeyIdx] = pKeyActionMap->pPressedKeysBuffer[0];
    memset(GameMenuUI_InvaligKeyBindingsFlags.data(), 0, sizeof(GameMenuUI_InvaligKeyBindingsFlags));
    v4 = 0;
    do
    {
      v5 = 0;
      do
      {
        if ( v4 != v5 && pPrevVirtualCidesMapping[v4] == pPrevVirtualCidesMapping[v5] )
        {
          GameMenuUI_InvaligKeyBindingsFlags[v4] = true;
          GameMenuUI_InvaligKeyBindingsFlags[v5] = true;
        }
        ++v5;
      }
      while ( v5 < 28 );
      ++v4;
    }
    while ( v4 < 28 );
    uGameMenuUI_CurentlySelectedKeyIdx = -1;
    pGUIWindow_CurrentMenu->receives_keyboard_input_2 = WINDOW_INPUT_NONE;
  }
  pRenderer->DrawTextureIndexed(8, 8, pIcons_LOD->GetTexture(uTextureID_Optkb[0]));//draw base texture
  if ( KeyboardPageNum == 1 )
  {
    pRenderer->DrawTextureIndexed(19, 302, pIcons_LOD->GetTexture(uTextureID_Optkb[3]));

    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 23, 142, ui_gamemenu_keys_action_name_color, "ÂÏÅÐ¨Ä", 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 127, 142, GameMenuUI_GetKeyBindingColor(0), pKeyActionMap->GetVKeyDisplayName(pPrevVirtualCidesMapping[0]), 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 23, 163, ui_gamemenu_keys_action_name_color, "ÍÀÇÀÄ", 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 127, 163, GameMenuUI_GetKeyBindingColor(1), pKeyActionMap->GetVKeyDisplayName(pPrevVirtualCidesMapping[1]), 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 23, 184, ui_gamemenu_keys_action_name_color, "ÂËÅÂÎ", 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 127, 184, GameMenuUI_GetKeyBindingColor(2), pKeyActionMap->GetVKeyDisplayName(pPrevVirtualCidesMapping[2]), 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 23, 205, ui_gamemenu_keys_action_name_color, "ÂÏÐÀÂÎ", 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 127, 205, GameMenuUI_GetKeyBindingColor(3), pKeyActionMap->GetVKeyDisplayName(pPrevVirtualCidesMapping[3]), 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 23, 226, ui_gamemenu_keys_action_name_color, "ÊÐÈÊ", 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 127, 226, GameMenuUI_GetKeyBindingColor(4), pKeyActionMap->GetVKeyDisplayName(pPrevVirtualCidesMapping[4]), 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 23, 247, ui_gamemenu_keys_action_name_color, "ÏÐÛÆÎÊ", 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 127, 247, GameMenuUI_GetKeyBindingColor(5), pKeyActionMap->GetVKeyDisplayName(pPrevVirtualCidesMapping[5]), 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 23, 268, ui_gamemenu_keys_action_name_color, "Ï.ÐÅÆÈÌ", 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 127, 268, GameMenuUI_GetKeyBindingColor(6), pKeyActionMap->GetVKeyDisplayName(pPrevVirtualCidesMapping[6]), 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 247, 142, ui_gamemenu_keys_action_name_color, "ÏÐÈÌ. ÇÀÊË.", 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 350, 142, GameMenuUI_GetKeyBindingColor(7), pKeyActionMap->GetVKeyDisplayName(pPrevVirtualCidesMapping[7]), 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 247, 163, ui_gamemenu_keys_action_name_color, "ÀÒÀÊÀ", 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 350, 163, GameMenuUI_GetKeyBindingColor(8), pKeyActionMap->GetVKeyDisplayName(pPrevVirtualCidesMapping[8]), 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 247, 184, ui_gamemenu_keys_action_name_color, "ÄÅÉÑÒÂ.", 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 350, 184, GameMenuUI_GetKeyBindingColor(9), pKeyActionMap->GetVKeyDisplayName(pPrevVirtualCidesMapping[9]), 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 247, 205, ui_gamemenu_keys_action_name_color, "ÇÀÊËÈÍ.", 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 350, 205, GameMenuUI_GetKeyBindingColor(10), pKeyActionMap->GetVKeyDisplayName(pPrevVirtualCidesMapping[10]), 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 247, 226, ui_gamemenu_keys_action_name_color, "ÈÃÐÎÊ", 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 350, 226, GameMenuUI_GetKeyBindingColor(11), pKeyActionMap->GetVKeyDisplayName(pPrevVirtualCidesMapping[11]), 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 247, 247, ui_gamemenu_keys_action_name_color, "ÑËÅÄ. ÈÃÐÎÊ", 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 350, 247, GameMenuUI_GetKeyBindingColor(12), pKeyActionMap->GetVKeyDisplayName(pPrevVirtualCidesMapping[12]), 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 247, 268, ui_gamemenu_keys_action_name_color, "ÇÀÄÀÍÈß", 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 350, 268, GameMenuUI_GetKeyBindingColor(13), pKeyActionMap->GetVKeyDisplayName(pPrevVirtualCidesMapping[13]), 0, 0, 0);
  }
  else
  {
    pRenderer->DrawTextureIndexed(127, 302, pIcons_LOD->GetTexture(uTextureID_Optkb[4]));

    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 23, 142, ui_gamemenu_keys_action_name_color, "Á. ÑÏÐÀÂÊÀ", 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 127, 142, GameMenuUI_GetKeyBindingColor(14), pKeyActionMap->GetVKeyDisplayName(pPrevVirtualCidesMapping[14]), 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 23, 163, ui_gamemenu_keys_action_name_color, "ÎÒÄÛÕ", 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 127, 163, GameMenuUI_GetKeyBindingColor(15), pKeyActionMap->GetVKeyDisplayName(pPrevVirtualCidesMapping[15]), 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 23, 184, ui_gamemenu_keys_action_name_color, "ÒÅÊ. ÂÐÅÌß", 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 127, 184, GameMenuUI_GetKeyBindingColor(16), pKeyActionMap->GetVKeyDisplayName(pPrevVirtualCidesMapping[16]), 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 23, 205, ui_gamemenu_keys_action_name_color, "ÀÂÒÎÇÀÌÅÒÊÈ", 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 127, 205, GameMenuUI_GetKeyBindingColor(17), pKeyActionMap->GetVKeyDisplayName(pPrevVirtualCidesMapping[17]), 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 23, 226, ui_gamemenu_keys_action_name_color, "ÊÀÐÒÀ", 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 127, 226, GameMenuUI_GetKeyBindingColor(18), pKeyActionMap->GetVKeyDisplayName(pPrevVirtualCidesMapping[18]), 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 23, 247, ui_gamemenu_keys_action_name_color, "ÁÅÆÀÒÜ", 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 127, 247, GameMenuUI_GetKeyBindingColor(19), pKeyActionMap->GetVKeyDisplayName(pPrevVirtualCidesMapping[19]), 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 23, 268, ui_gamemenu_keys_action_name_color, "ÑÌ. ÂÂÅÐÕ", 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 127, 268, GameMenuUI_GetKeyBindingColor(20), pKeyActionMap->GetVKeyDisplayName(pPrevVirtualCidesMapping[20]), 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 247, 142, ui_gamemenu_keys_action_name_color, "ÑÌ. ÂÍÈÇ", 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 350, 142, GameMenuUI_GetKeyBindingColor(21), pKeyActionMap->GetVKeyDisplayName(pPrevVirtualCidesMapping[21]), 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 247, 163, ui_gamemenu_keys_action_name_color, "ÑÌ. ÂÏÅÐ¨Ä", 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 350, 163, GameMenuUI_GetKeyBindingColor(22), pKeyActionMap->GetVKeyDisplayName(pPrevVirtualCidesMapping[22]), 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 247, 184, ui_gamemenu_keys_action_name_color, "ÏÐÈÁËÈÇ", 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 350, 184, GameMenuUI_GetKeyBindingColor(23), pKeyActionMap->GetVKeyDisplayName(pPrevVirtualCidesMapping[23]), 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 247, 205, ui_gamemenu_keys_action_name_color, "ÎÒÄÀËÈÒÜ", 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 350, 205, GameMenuUI_GetKeyBindingColor(24), pKeyActionMap->GetVKeyDisplayName(pPrevVirtualCidesMapping[24]), 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 247, 226, ui_gamemenu_keys_action_name_color, "Ï. ÂÂÅÐÕ", 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 350, 226, GameMenuUI_GetKeyBindingColor(25), pKeyActionMap->GetVKeyDisplayName(pPrevVirtualCidesMapping[25]), 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 247, 247, ui_gamemenu_keys_action_name_color, "Ï. ÂÍÈÇ", 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 350, 247, GameMenuUI_GetKeyBindingColor(26), pKeyActionMap->GetVKeyDisplayName(pPrevVirtualCidesMapping[26]), 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 247, 268, ui_gamemenu_keys_action_name_color, "ÏÐÈÇÅÌË", 0, 0, 0);
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, 350, 268, GameMenuUI_GetKeyBindingColor(27), pKeyActionMap->GetVKeyDisplayName(pPrevVirtualCidesMapping[27]), 0, 0, 0);
  }
}

//----- (00414D9A) --------------------------------------------------------
void GameMenuUI_DrawVideoOptions()
{
  GUIWindow msg_window; // [sp+8h] [bp-54h]@3

  pRenderer->DrawTextureIndexed(8, 8, pIcons_LOD->GetTexture(optvid_base_texture_id));//draw base texture
  //if ( !pRenderer->bWindowMode && pRenderer->IsGammaSupported() )
  {
    pRenderer->DrawTextureIndexed(17 * uGammaPos + 42, 162, pIcons_LOD->GetTexture(pTextureIDs_GammaPositions[uGammaPos]));
    pRenderer->DrawTextureRGB(274, 169, &stru_506E40);//review_window
    msg_window.uFrameX = 22;
    msg_window.uFrameY = 190;
    msg_window.uFrameWidth = 211;
    msg_window.uFrameHeight = 79;
    msg_window.uFrameZ = 232;
    msg_window.uFrameW = 268;
    msg_window.DrawTitleText(pFontSmallnum, 0, 0, ui_gamemenu_video_gamma_title_color, pGlobalTXT_LocalizationStrings[226], 3); // "Gamma controls the relative ""brightness"" of the game.  May vary depending on your monitor."
  }

  /*if (!pRenderer->pRenderD3D)
  {
    pRenderer->DrawTextureIndexed(20, 281, pIcons_LOD->GetTexture(not_available_bloodsplats_texture_id));
    pRenderer->DrawTextureIndexed(20, 303, pIcons_LOD->GetTexture(not_available_us_colored_lights_texture_id));
    pRenderer->DrawTextureIndexed(20, 325, pIcons_LOD->GetTexture(not_available_tinting_texture_id));
  }
  else*/
  {
    if (pGame->uFlags2 & GAME_FLAGS_2_DRAW_BLOODSPLATS)
      pRenderer->DrawTextureIndexed(20, 281, pIcons_LOD->GetTexture(bloodsplats_texture_id));
    if (pRenderer->bUseColoredLights)
      pRenderer->DrawTextureIndexed(20, 303, pIcons_LOD->GetTexture(us_colored_lights_texture_id));
    if (pRenderer->bTinting)
      pRenderer->DrawTextureIndexed(20, 325, pIcons_LOD->GetTexture(tinting_texture_id));
  }
}

//----- (00414F82) --------------------------------------------------------
void GameMenuUI_Options_Draw()
{
  pRenderer->DrawTextureIndexed(8,   8, pIcons_LOD->GetTexture(uTextureID_Options));
  pRenderer->DrawTextureIndexed(8, 132, pIcons_LOD->GetTexture(options_menu_skin.uTextureID_Background));

  switch (uTurnSpeed)
  {
    case 64:   pRenderer->DrawTextureIndexed(BtnTurnCoord[1], 270, pIcons_LOD->GetTexture(options_menu_skin.uTextureID_TurnSpeed[1])); break;
    case 128:  pRenderer->DrawTextureIndexed(BtnTurnCoord[2], 270, pIcons_LOD->GetTexture(options_menu_skin.uTextureID_TurnSpeed[2])); break;
    default:   pRenderer->DrawTextureIndexed(BtnTurnCoord[0], 270, pIcons_LOD->GetTexture(options_menu_skin.uTextureID_TurnSpeed[0])); break;
  }

  if (bWalkSound)  pRenderer->DrawTextureIndexed( 20, 303, pIcons_LOD->GetTexture(options_menu_skin.uTextureID_WalkSound));
  if (bShowDamage) pRenderer->DrawTextureIndexed(128, 303, pIcons_LOD->GetTexture(options_menu_skin.uTextureID_ShowDamage));
  if (bFlipOnExit) pRenderer->DrawTextureIndexed(128, 325, pIcons_LOD->GetTexture(options_menu_skin.uTextureID_FlipOnExit));
  if (bAlwaysRun)  pRenderer->DrawTextureIndexed( 20, 325, pIcons_LOD->GetTexture(options_menu_skin.uTextureID_AlwaysRun));

  pRenderer->DrawTextureIndexed(265 + 17 * uSoundVolumeMultiplier,  162, pIcons_LOD->GetTexture(options_menu_skin.uTextureID_SoundLevels[uSoundVolumeMultiplier]));
  pRenderer->DrawTextureIndexed(265 + 17 * uMusicVolimeMultiplier,  216, pIcons_LOD->GetTexture(options_menu_skin.uTextureID_SoundLevels[uMusicVolimeMultiplier]));
  pRenderer->DrawTextureIndexed(265 + 17 * uVoicesVolumeMultiplier, 270, pIcons_LOD->GetTexture(options_menu_skin.uTextureID_SoundLevels[uVoicesVolumeMultiplier]));
}

OptionsMenuSkin::OptionsMenuSkin():
  uTextureID_Background(0),
  uTextureID_ArrowLeft(0),
  uTextureID_ArrowRight(0),
  uTextureID_unused_0(0), uTextureID_unused_1(0), uTextureID_unused_2(0),
  uTextureID_FlipOnExit(0),
  uTextureID_AlwaysRun(0),
  uTextureID_WalkSound(0),
  uTextureID_ShowDamage(0)
{
    for (uint i = 0; i < 3; ++i)  uTextureID_TurnSpeed[i] = 0;
    for (uint i = 0; i < 10; ++i) uTextureID_SoundLevels[i] = 0;
} 

void OptionsMenuSkin::Relaease()
{
  #define RELEASE(id) \
  {\
    if (id)\
      pIcons_LOD->GetTexture(id)->Release();\
    id = 0;\
  }

  RELEASE(uTextureID_Background);
  for (uint i = 0; i < 3; ++i)
    RELEASE(uTextureID_TurnSpeed[i]);
  RELEASE(uTextureID_ArrowLeft);
  RELEASE(uTextureID_ArrowRight);
  RELEASE(uTextureID_FlipOnExit);
  for (uint i = 0; i < 10; ++i)
    RELEASE(uTextureID_SoundLevels[i]);
  RELEASE(uTextureID_AlwaysRun);
  RELEASE(uTextureID_WalkSound);
  RELEASE(uTextureID_ShowDamage);

  #undef RELEASE
}