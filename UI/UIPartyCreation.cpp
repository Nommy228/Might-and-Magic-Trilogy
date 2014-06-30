#define _CRT_SECURE_NO_WARNINGS
#include "UIPartyCreation.h"
#include "..\mm7_unsorted_subs.h"
#include "..\Mouse.h"
#include "..\Keyboard.h"
#include "..\ErrorHandling.h"

#include "..\Game.h"
#include "..\GUIWindow.h"
#include "..\GUIFont.h"
#include "..\Party.h"
#include "..\AudioPlayer.h"
#include "..\Render.h"
#include "..\LOD.h"
#include "..\Timer.h"
#include "..\IconFrameTable.h"
#include "..\texts.h"

#include "..\mm7_data.h"



//----- (004908DE) --------------------------------------------------------
bool PlayerCreation_Choose4Skills()
{
  signed int skills_count; // edx@2

  for ( uint j = 0; j < 4; ++j )
  {
    skills_count = 0;
    for ( uint i = 0; i < 37; ++i )
    {
      if ( pParty->pPlayers[j].pActiveSkills[i] )
        ++skills_count;
    }
    if ( skills_count < 4 )
      return false;
  }
  return true;
}

//----- (00491CB5) --------------------------------------------------------
void  LoadPlayerPortraintsAndVoices()
{
  pIcons_LOD->pFacesLock = pIcons_LOD->uNumLoadedFiles;

  for (uint i = 0; i < 4; ++i)
    for (uint j = 0; j < 56; ++j)
    {
      sprintf(pTmpBuf.data(), "%s%02d", pPlayerPortraitsNames[pParty->pPlayers[i].uCurrentFace], j + 1);
      pTextures_PlayerFaces[i][j] = pIcons_LOD->LoadTexturePtr(pTmpBuf.data(), TEXTURE_16BIT_PALETTE);
    }

  pTexture_PlayerFaceEradicated = pIcons_LOD->LoadTexturePtr("ERADCATE", TEXTURE_16BIT_PALETTE);
  pTexture_PlayerFaceDead = pIcons_LOD->LoadTexturePtr("DEAD", TEXTURE_16BIT_PALETTE);
  pTexture_PlayerFaceMask = pIcons_LOD->LoadTexturePtr("FACEMASK", TEXTURE_16BIT_PALETTE);

  if (SoundSetAction[24][0])
    for (uint i = 0; i < 4; ++i)
    {
      pSoundList->LoadSound(2 * (SoundSetAction[24][0] + 50 * pParty->pPlayers[i].uVoiceID) + 4998, 0);
      pSoundList->LoadSound(2 * (SoundSetAction[24][0] + 50 * pParty->pPlayers[i].uVoiceID) + 4999, 0);
    }
}

//----- (00491DE7) --------------------------------------------------------
void ReloadPlayerPortraits(int player_id, int face_id)//the transition from the zombies in the normal state
{
  for ( uint i = 0; i <= 55; ++i )
  {
    sprintf(pTmpBuf.data(), "%s%02d", pPlayerPortraitsNames[face_id], i + 1);
    pIcons_LOD->ReloadTexture(pTextures_PlayerFaces[player_id][i], pTmpBuf.data(), 2);
  }
}
//----- (00495B39) --------------------------------------------------------
void PlayerCreationUI_Draw()
{
  int pTextCenter; // eax@3
  IconFrame *pFrame; // eax@3
  int pX; // ecx@7
  GUIButton *uPosActiveItem; // edi@12
  int v17; // eax@33
  int uStatLevel; // eax@44
  unsigned int pStatColor; // eax@44
  PLAYER_SKILL_TYPE pSkillsType; // eax@44
  PLAYER_CLASS_TYPE uClassType; // edi@53
  int pColorText; // eax@53
  PLAYER_SKILL_TYPE pSkillId; // edi@72
  size_t pLenText; // eax@72
  signed int v104; // ecx@72
//  int pTextY; // ST08_4@81
  signed int pBonusNum; // edi@82
  const char *uRaceName; // [sp+0h] [bp-170h]@39
  char pText[200]; // [sp+10h] [bp-160h]@14
  GUIWindow message_window; // [sp+D8h] [bp-98h]@83
  int v126; // [sp+148h] [bp-28h]@25
  int pIntervalY; // [sp+150h] [bp-20h]@14
  int pX_Numbers; // [sp+154h] [bp-1Ch]@18
  int uX; // [sp+160h] [bp-10h]@18
  int pIntervalX;
  int pCorrective;

  //move sky
  pRenderer->BeginScene();
  pRenderer->DrawTextureRGB(0, 0, &pTexture_PCX);
  uPlayerCreationUI_SkySliderPos = (GetTickCount() % 12800) / 20;
  pRenderer->DrawTextureIndexed(uPlayerCreationUI_SkySliderPos, 2, pTexture_MAKESKY);
  pRenderer->DrawTextureIndexed(uPlayerCreationUI_SkySliderPos - window->GetWidth(), 2, pTexture_MAKESKY);
  pRenderer->DrawTextureTransparent(0, 0, pTexture_MAKETOP);

  uPlayerCreationUI_SelectedCharacter = (pGUIWindow_CurrentMenu->pCurrentPosActiveItem - pGUIWindow_CurrentMenu->pStartingPosActiveItem) / 7;
  switch (uPlayerCreationUI_SelectedCharacter)
  {
    case 0: pX = 12;  break;
    case 1: pX = 171; break;
    case 2: pX = 329; break;
    case 3: pX = 488; break;
    default:
      Error("Invalid selected character");
  }

  pTextCenter = pFontCChar->AlignText_Center(window->GetWidth(), pGlobalTXT_LocalizationStrings[51]);
  pGUIWindow_CurrentMenu->DrawText(pFontCChar, pTextCenter + 1, 0, 0, pGlobalTXT_LocalizationStrings[51], 0, 0, 0);//Ñ Î Ç Ä À Ò Ü  Î Ò Ð ß Ä
  pRenderer->DrawTextureTransparent(17, 35, pPlayerPortraits[pParty->pPlayers[0].uCurrentFace]);
  pRenderer->DrawTextureTransparent(176, 35, pPlayerPortraits[pParty->pPlayers[1].uCurrentFace]);
  pRenderer->DrawTextureTransparent(335, 35, pPlayerPortraits[pParty->pPlayers[2].uCurrentFace]);
  pRenderer->DrawTextureTransparent(494, 35, pPlayerPortraits[pParty->pPlayers[3].uCurrentFace]);
  pFrame = pIconsFrameTable->GetFrame(uIconID_CharacterFrame, pEventTimer->uStartTime);


  pRenderer->DrawTextureTransparent(pX, 29, &pIcons_LOD->pTextures[pFrame->uTextureID]);
  uPosActiveItem = pGUIWindow_CurrentMenu->GetControl(pGUIWindow_CurrentMenu->pCurrentPosActiveItem);
  uPlayerCreationUI_ArrowAnim = 19 - (GetTickCount() % 500) / 25;
  pRenderer->DrawTextureTransparent(uPosActiveItem->uZ - 4, uPosActiveItem->uY, pTextures_arrowl[uPlayerCreationUI_ArrowAnim]);
  pRenderer->DrawTextureTransparent(uPosActiveItem->uX - 12, uPosActiveItem->uY, pTextures_arrowr[uPlayerCreationUI_ArrowAnim]);

  memset(pText, 0, 200);
  strcpy(pText, pGlobalTXT_LocalizationStrings[205]);// "Skills"
  for ( int i = strlen(pText) - 1; i >= 0; i-- )//???
    pText[i] = toupper((unsigned __int8)pText[i]);

  pIntervalX = 18;
  pIntervalY = pFontCreate->uFontHeight - 2;
  uX = 32;
  pX_Numbers = 493;

  for (int i = 0; i < 4; ++i)
  {
    pGUIWindow_CurrentMenu->DrawText(pFontCreate, pIntervalX + 73, 100, 0, pClassNames[pParty->pPlayers[i].classType], 0, 0, 0);
    pRenderer->DrawTextureTransparent(pIntervalX + 77, 50, pTexture_IC_KNIGHT[pParty->pPlayers[i].classType / 4]);

    if ( pGUIWindow_CurrentMenu->receives_keyboard_input_2 != WINDOW_INPUT_NONE && pGUIWindow_CurrentMenu->ptr_1C == (void *)i )
    {
      switch ( pGUIWindow_CurrentMenu->receives_keyboard_input_2 )
      {
        case WINDOW_INPUT_IN_PROGRESS://press name panel
          v17 = pGUIWindow_CurrentMenu->DrawTextInRect(pFontCreate, 159 * (int)pGUIWindow_CurrentMenu->ptr_1C + 18, 124, 0, (const char *)pKeyActionMap->pPressedKeysBuffer, 120, 1);
          pGUIWindow_CurrentMenu->DrawFlashingInputCursor(159 * (unsigned int)pGUIWindow_CurrentMenu->ptr_1C + v17 + 20, 124, pFontCreate);
          break;
        case WINDOW_INPUT_CONFIRMED: // press enter
          pGUIWindow_CurrentMenu->receives_keyboard_input_2 = WINDOW_INPUT_NONE;
          v126 = 0;
          for ( int i = 0; i < strlen((const char *)pKeyActionMap->pPressedKeysBuffer); ++i )//edit name
          {
            if ( pKeyActionMap->pPressedKeysBuffer[i] == ' ' )
              ++v126;
          }
          if ( strlen((const char *)pKeyActionMap->pPressedKeysBuffer) && v126 != strlen((const char *)pKeyActionMap->pPressedKeysBuffer) )
            strcpy(pParty->pPlayers[i].pName, (const char *)pKeyActionMap->pPressedKeysBuffer);
          pGUIWindow_CurrentMenu->DrawTextInRect(pFontCreate, pIntervalX, 124, 0, pParty->pPlayers[i].pName, 130, 0);
          pParty->pPlayers[i].field_1988[27] = 1;
          break;
        case WINDOW_INPUT_CANCELLED: // press escape
          pGUIWindow_CurrentMenu->receives_keyboard_input_2 = WINDOW_INPUT_NONE;
          pGUIWindow_CurrentMenu->DrawTextInRect(pFontCreate, pIntervalX, 124, 0, pParty->pPlayers[i].pName, 130, 0);
          SetCurrentMenuID(MENU_NAMEPANELESC);
          break;
      }
    }
    else
    {
      pGUIWindow_CurrentMenu->DrawTextInRect(pFontCreate, pIntervalX, 124, 0, pParty->pPlayers[i].pName, 130, 0);
    }

    switch (pParty->pPlayers[i].GetRace())
    {
      case 0:  uRaceName = pGlobalTXT_LocalizationStrings[99]; break; // "Human"
      case 1:  uRaceName = pGlobalTXT_LocalizationStrings[103]; break; // "Dwarf"
      case 2:  uRaceName = pGlobalTXT_LocalizationStrings[106]; break; // "Goblin"
      case 3:  uRaceName = pGlobalTXT_LocalizationStrings[101]; break; // "Elf"
    }; 
    strcpy(pTmpBuf.data(), uRaceName);
    pGUIWindow_CurrentMenu->DrawTextInRect(pFontCreate, pIntervalX + 72, pIntervalY + 12, 0, pTmpBuf.data(), 130, 0);//Race Name

    pTextCenter = pFontCreate->AlignText_Center(150, pText);
    pGUIWindow_CurrentMenu->DrawText(pFontCreate, pTextCenter + uX - 24, 291, Color16(0xD1, 0xBB, 0x61), pText, 0, 0, 0); // Skills

    uStatLevel = pParty->pPlayers[i].GetActualMight();
    sprintf(pTmpBuf.data(), "%s\r%03d%d", pGlobalTXT_LocalizationStrings[144], pX_Numbers, uStatLevel);// "Might"
    pStatColor = pParty->pPlayers[i].GetStatColor(0);
    pGUIWindow_CurrentMenu->DrawText(pFontCreate, uX, 169, pStatColor, pTmpBuf.data(), 0, 0, 0);

    uStatLevel = pParty->pPlayers[i].GetActualIntelligence();
    sprintf(pTmpBuf.data(), "%s\r%03d%d", pGlobalTXT_LocalizationStrings[116], pX_Numbers, uStatLevel);// "Intellect"
    pStatColor = pParty->pPlayers[i].GetStatColor(1);
    pGUIWindow_CurrentMenu->DrawText(pFontCreate, uX, pIntervalY + 169, pStatColor, pTmpBuf.data(), 0, 0, 0);

    uStatLevel = pParty->pPlayers[i].GetActualWillpower();
    sprintf(pTmpBuf.data(), "%s\r%03d%d", pGlobalTXT_LocalizationStrings[163], pX_Numbers, uStatLevel);// "Personality"
    pStatColor = pParty->pPlayers[i].GetStatColor(2);
    pGUIWindow_CurrentMenu->DrawText(pFontCreate, uX, 2 * pIntervalY + 169, pStatColor, pTmpBuf.data(), 0, 0, 0);

    uStatLevel = pParty->pPlayers[i].GetActualEndurance();
    sprintf(pTmpBuf.data(), "%s\r%03d%d", pGlobalTXT_LocalizationStrings[75], pX_Numbers, uStatLevel);// "Endurance"
    pStatColor = pParty->pPlayers[i].GetStatColor(3);
    pGUIWindow_CurrentMenu->DrawText(pFontCreate, uX, 3 * pIntervalY + 169, pStatColor, pTmpBuf.data(), 0, 0, 0);

    uStatLevel = pParty->pPlayers[i].GetActualAccuracy();
    sprintf(pTmpBuf.data(), "%s\r%03d%d", pGlobalTXT_LocalizationStrings[1], pX_Numbers, uStatLevel);// "Accuracy"
    pStatColor = pParty->pPlayers[i].GetStatColor(4);
    pGUIWindow_CurrentMenu->DrawText(pFontCreate, uX, 4 * pIntervalY + 169, pStatColor, pTmpBuf.data(), 0, 0, 0);

    uStatLevel = pParty->pPlayers[i].GetActualSpeed();
    sprintf(pTmpBuf.data(), "%s\r%03d%d", pGlobalTXT_LocalizationStrings[211], pX_Numbers, uStatLevel);// "Speed"
    pStatColor = pParty->pPlayers[i].GetStatColor(5);
    pGUIWindow_CurrentMenu->DrawText(pFontCreate, uX, 5 * pIntervalY + 169, pStatColor, pTmpBuf.data(), 0, 0, 0);

    uStatLevel = pParty->pPlayers[i].GetActualLuck();
    sprintf(pTmpBuf.data(), "%s\r%03d%d", pGlobalTXT_LocalizationStrings[136], pX_Numbers, uStatLevel);// "Luck"
    pStatColor = pParty->pPlayers[i].GetStatColor(6);
    pGUIWindow_CurrentMenu->DrawText(pFontCreate, uX, 6 * pIntervalY + 169, pStatColor, pTmpBuf.data(), 0, 0, 0);


    pSkillsType = pParty->pPlayers[i].GetSkillIdxByOrder(0);
    pTextCenter = pFontCreate->AlignText_Center(150, pSkillNames[pSkillsType]);
    sprintf(pTmpBuf.data(), "\t%03u%s", pTextCenter, pSkillNames[pSkillsType]);
    pGUIWindow_CurrentMenu->DrawText(pFontCreate, uX - 24, 311, Color16(0xFF, 0xFF, 0xFF), pTmpBuf.data(), 0, 0, 0);

    pSkillsType = pParty->pPlayers[i].GetSkillIdxByOrder(1);
    pTextCenter = pFontCreate->AlignText_Center(150, pSkillNames[pSkillsType]);
    sprintf(pTmpBuf.data(), "\t%03u%s", pTextCenter, pSkillNames[pSkillsType]);
    pGUIWindow_CurrentMenu->DrawText(pFontCreate, uX - 24, pIntervalY + 311, Color16(0xFF, 0xFF, 0xFF), pTmpBuf.data(), 0, 0, 0);

    pSkillsType = pParty->pPlayers[i].GetSkillIdxByOrder(2);
    pTextCenter = pFontCreate->AlignText_Center(150, pSkillNames[pSkillsType]);
    sprintf(pTmpBuf.data(), "\t%03u%s", pTextCenter, pSkillNames[pSkillsType]);
    pColorText = Color16(0, 0xFF, 0);
    if ( (signed int)pSkillsType >= 37 )
      pColorText = Color16(0, 0xF7, 0xF7);
    pGUIWindow_CurrentMenu->DrawText(pFontCreate, uX - 24, 2 * pIntervalY + 311, pColorText, pTmpBuf.data(), 0, 0, 0);

    pSkillsType = pParty->pPlayers[i].GetSkillIdxByOrder(3);
    pTextCenter = pFontCreate->AlignText_Center(150, pSkillNames[pSkillsType]);
    sprintf(pTmpBuf.data(), "\t%03u%s", pTextCenter, pSkillNames[pSkillsType]);
    pColorText = Color16(0, 0xFF, 0);
    if ( (signed int)pSkillsType >= 37 )
      pColorText = Color16(0, 0xF7, 0xF7);
    pGUIWindow_CurrentMenu->DrawText(pFontCreate, uX - 24, 3 * pIntervalY + 311, pColorText, pTmpBuf.data(), 0, 0, 0);

    pIntervalX += 159;
    pX_Numbers -= 158;
    uX += 158;
  }

  strcpy(pText, pGlobalTXT_LocalizationStrings[41]);// "Class"
  for ( int i = strlen(pText) - 1; i >= 0; i-- )
    pText[i] = toupper((unsigned __int8)pText[i]);

  uClassType = pParty->pPlayers[uPlayerCreationUI_SelectedCharacter].classType;
  pTextCenter = pFontCreate->AlignText_Center(193, pText);
  pGUIWindow_CurrentMenu->DrawText(pFontCreate, pTextCenter + 324, 395, Color16(0xD1, 0xBB, 0x61), pText, 0, 0, 0);//Classes

  pColorText = Color16(0, 0xF7, 0xF7);
  if ( uClassType )
    pColorText = Color16(0xFF, 0xFF, 0xFF);
  pTextCenter = pFontCreate->AlignText_Center(65, pClassNames[0]);
  pGUIWindow_CurrentMenu->DrawText(pFontCreate, pTextCenter + 323, 417, pColorText, pClassNames[0], 0, 0, 0);

  pColorText = Color16(0, 0xF7, 0xF7);
  if ( uClassType != PLAYER_CLASS_PALADIN )
    pColorText = Color16(0xFF, 0xFF, 0xFF);
  pTextCenter = pFontCreate->AlignText_Center(65, pClassNames[12]);
  pGUIWindow_CurrentMenu->DrawText(pFontCreate, pTextCenter + 323, pIntervalY + 417, pColorText, pClassNames[12], 0, 0, 0);

  pColorText = Color16(0, 0xF7, 0xF7);
  if ( uClassType != PLAYER_CLASS_DRUID )
    pColorText = Color16(0xFF, 0xFF, 0xFF);
  pTextCenter = pFontCreate->AlignText_Center(65, pClassNames[20]);
  pGUIWindow_CurrentMenu->DrawText(pFontCreate, pTextCenter + 323, 2 * pIntervalY + 417, pColorText, pClassNames[20], 0, 0, 0);

  pColorText = Color16(0, 0xF7, 0xF7);
  if ( uClassType != PLAYER_CLASS_CLERIC )
    pColorText = Color16(0xFF, 0xFF, 0xFF);
  pTextCenter = pFontCreate->AlignText_Center(65, pClassNames[24]);
  pGUIWindow_CurrentMenu->DrawText(pFontCreate, pTextCenter + 388, 417, pColorText, pClassNames[24], 0, 0, 0);

  pColorText = Color16(0, 0xF7, 0xF7);
  if ( uClassType != PLAYER_CLASS_DRUID)
    pColorText = Color16(0xFF, 0xFF, 0xFF);
  pTextCenter = pFontCreate->AlignText_Center(65, pClassNames[28]);
  pGUIWindow_CurrentMenu->DrawText(pFontCreate, pTextCenter + 388, pIntervalY + 417, pColorText, pClassNames[28], 0, 0, 0);

  pColorText = Color16(0, 0xF7, 0xF7);
  if ( uClassType != PLAYER_CLASS_SORCERER )
    pColorText = Color16(0xFF, 0xFF, 0xFF);
  pTextCenter = pFontCreate->AlignText_Center(65, pClassNames[32]);
  pGUIWindow_CurrentMenu->DrawText(pFontCreate, pTextCenter + 388, 2 * pIntervalY + 417, pColorText, pClassNames[32], 0, 0, 0);

  pColorText = Color16(0, 0xF7, 0xF7);
  if ( uClassType != PLAYER_CLASS_ARCHER )
    pColorText = Color16(0xFF, 0xFF, 0xFF);
  pTextCenter = pFontCreate->AlignText_Center(65, pClassNames[16]);
  pGUIWindow_CurrentMenu->DrawText(pFontCreate, pTextCenter + 453, 417, pColorText, pClassNames[16], 0, 0, 0);

  pColorText = Color16(0, 0xF7, 0xF7);
  if ( uClassType != PLAYER_CLASS_MONK )
    pColorText = Color16(0xFF, 0xFF, 0xFF);
  pTextCenter = pFontCreate->AlignText_Center(65, pClassNames[8]);
  pGUIWindow_CurrentMenu->DrawText(pFontCreate, pTextCenter + 453, pIntervalY + 417, pColorText, pClassNames[8], 0, 0, 0);

  pColorText = Color16(0, 0xF7, 0xF7);
  if ( uClassType != PLAYER_CLASS_THEIF )
    pColorText = Color16(0xFF, 0xFF, 0xFF);
  pTextCenter = pFontCreate->AlignText_Center(65, pClassNames[4]);
  pGUIWindow_CurrentMenu->DrawText(pFontCreate, pTextCenter + 453, 2 * pIntervalY + 417, pColorText, pClassNames[4], 0, 0, 0);

  pTextCenter = pFontCreate->AlignText_Center(236, pGlobalTXT_LocalizationStrings[20]); // "Available Skills"
  pGUIWindow_CurrentMenu->DrawText(pFontCreate, pTextCenter + 37, 395, Color16(0xD1, 0xBB, 0x61), pGlobalTXT_LocalizationStrings[20], 0, 0, 0);
  for (uint i = 0; i < 9; ++i)
  {
    pSkillId = pParty->pPlayers[uPlayerCreationUI_SelectedCharacter].GetSkillIdxByOrder(i + 4);
    strcpy(pText, pSkillNames[pSkillId]);
    pLenText = strlen(pText);
    v104 = 0;
    if ( (signed int)pLenText > 0 )
    {
      if ( pText[v104] == 32 )
      {
        pText[v104] = 0;
      }
      else
      {
        while ( pText[v104] != 32 )
        {
          ++v104;
          if ( v104 >= (signed int)pLenText )
          break;
        }
      }
    }
    pCorrective = -10;//-5
    if ( (signed int)pLenText < 8 )//if ( (signed int)v124 > 2 )
      pCorrective = 0;
    pColorText = Color16(0, 0xF7, 0xF7);
    if ( !pParty->pPlayers[uPlayerCreationUI_SelectedCharacter].pActiveSkills[pSkillId] )
      pColorText = Color16(0xFF, 0xFF, 0xFF);
    pTextCenter = pFontCreate->AlignText_Center(100, pText);
    pGUIWindow_CurrentMenu->DrawText(pFontCreate, 100 * (i / 3) + pTextCenter + pCorrective + 17, pIntervalY * (i % 3) + 417, pColorText, pText, 0, 0, 0);
  }

  pTextCenter = pFontCreate->AlignText_Center(0x5C, pGlobalTXT_LocalizationStrings[30]);// "Bonus"
  pGUIWindow_CurrentMenu->DrawText(pFontCreate, pTextCenter + 533, 394, Color16(0xD1, 0xBB, 0x61), pGlobalTXT_LocalizationStrings[30], 0, 0, 0);
  pBonusNum = PlayerCreation_GetUnspentAttributePointCount();
  sprintf(pTmpBuf.data(), "%d", pBonusNum);
  pTextCenter = pFontCreate->AlignText_Center(84, pTmpBuf.data());
  pGUIWindow_CurrentMenu->DrawText(pFontCreate, pTextCenter + 530, 410, Color16(0xFF, 0xFF, 0xFF), pTmpBuf.data(), 0, 0, 0);
  if ( GameUI_Footer_TimeLeft > GetTickCount() )
  {
    message_window.Hint = pGlobalTXT_LocalizationStrings[412];// "Create Party cannot be completed unless you have assigned all characters 2 extra skills and have spent all of your bonus points."
    if ( pBonusNum < 0 )
      message_window.Hint = pGlobalTXT_LocalizationStrings[413];// "You can't spend more than 50 points."
    message_window.uFrameWidth = 300;
    message_window.uFrameHeight = 100;
    message_window.uFrameX = 170;
    message_window.uFrameY = 140;
    message_window.uFrameZ = 469;
    message_window.uFrameW = 239;
    message_window.DrawMessageBox(0);
  }
  pRenderer->EndScene();
}

//----- (0049695A) --------------------------------------------------------
void  PlayerCreationUI_Initialize()
{
  unsigned int v0; // ebx@5
  signed int uControlParam; // [sp+10h] [bp-Ch]@7
  signed int uX; // [sp+14h] [bp-8h]@5

  pMessageQueue_50CBD0->Flush();

  pAudioPlayer->SetMusicVolume(pSoundVolumeLevels[uMusicVolimeMultiplier] * 64.0f);
  ++pIcons_LOD->uTexturePacksCount;
  if ( !pIcons_LOD->uNumPrevLoadedFiles )
    pIcons_LOD->uNumPrevLoadedFiles = pIcons_LOD->uNumLoadedFiles;
  pCurrentScreen = SCREEN_PARTY_CREATION;
  uPlayerCreationUI_ArrowAnim = 0;
  uPlayerCreationUI_SkySliderPos = 0;
  uPlayerCreationUI_SelectedCharacter = 0;
  v0 = LOBYTE(pFontCreate->uFontHeight) - 2;
  pTexture_IC_KNIGHT[0] = pIcons_LOD->LoadTexturePtr("IC_KNIGHT", TEXTURE_16BIT_PALETTE);
  pTexture_IC_KNIGHT[1] = pIcons_LOD->LoadTexturePtr("IC_THIEF", TEXTURE_16BIT_PALETTE);
  pTexture_IC_KNIGHT[2] = pIcons_LOD->LoadTexturePtr("IC_MONK", TEXTURE_16BIT_PALETTE);
  pTexture_IC_KNIGHT[3] = pIcons_LOD->LoadTexturePtr("IC_PALAD", TEXTURE_16BIT_PALETTE);
  pTexture_IC_KNIGHT[4] = pIcons_LOD->LoadTexturePtr("IC_ARCH", TEXTURE_16BIT_PALETTE);
  pTexture_IC_KNIGHT[5] = pIcons_LOD->LoadTexturePtr("IC_RANGER", TEXTURE_16BIT_PALETTE);
  pTexture_IC_KNIGHT[6] = pIcons_LOD->LoadTexturePtr("IC_CLER", TEXTURE_16BIT_PALETTE);
  pTexture_IC_KNIGHT[7] = pIcons_LOD->LoadTexturePtr("IC_DRUID", TEXTURE_16BIT_PALETTE);
  pTexture_IC_KNIGHT[8] = pIcons_LOD->LoadTexturePtr("IC_SORC", TEXTURE_16BIT_PALETTE);
  pTexture_MAKETOP = pIcons_LOD->LoadTexturePtr("MAKETOP", TEXTURE_16BIT_PALETTE);
  pTexture_MAKESKY = pIcons_LOD->LoadTexturePtr("MAKESKY", TEXTURE_16BIT_PALETTE);
  for( uX = 0; uX < 22; ++uX ) // load PlayerPortraits texture
  {
    sprintf(pTmpBuf.data(), "%s01", pPlayerPortraitsNames[uX]);
    pPlayerPortraits[uX] = &pIcons_LOD->pTextures[pIcons_LOD->LoadTexture(pTmpBuf.data(), TEXTURE_16BIT_PALETTE)];

  }
  pTexture_PlayerFaceMask = pIcons_LOD->LoadTexturePtr("FACEMASK", TEXTURE_16BIT_PALETTE);
  pTexture_buttminu  = pIcons_LOD->LoadTexturePtr("buttminu", TEXTURE_16BIT_PALETTE);
  pTexture_buttplus  = pIcons_LOD->LoadTexturePtr("buttplus", TEXTURE_16BIT_PALETTE);
  pTexture_pressrigh = pIcons_LOD->LoadTexturePtr("presrigh", TEXTURE_16BIT_PALETTE);
  pTexture_presleft  = pIcons_LOD->LoadTexturePtr("presleft", TEXTURE_16BIT_PALETTE);
  uControlParam = 1;
  do
  {
    sprintf(pTmpBuf.data(), "arrowl%d", uControlParam);
    pTextures_arrowl[uControlParam] = pIcons_LOD->LoadTexturePtr(pTmpBuf.data(), TEXTURE_16BIT_PALETTE);

    sprintf(pTmpBuf.data(), "arrowr%d", uControlParam);
    pTextures_arrowr[uControlParam] = pIcons_LOD->LoadTexturePtr(pTmpBuf.data(), TEXTURE_16BIT_PALETTE);
  }
  while ( ++uControlParam < 20 );
  pGUIWindow_CurrentMenu = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_MainMenu, 0, 0);
  uControlParam = 0;
  uX = 8;
  do
  {
    pGUIWindow_CurrentMenu->CreateButton(uX, 120, 145, 25, 1, 0, UIMSG_PlayerCreationChangeName, uControlParam, 0, "", 0);
    uX += 158;
    ++uControlParam;
  }
  while ( (signed int)uX < window->GetWidth() );

  pCreationUI_BtnPressLeft[0]   = pGUIWindow_CurrentMenu->CreateButton( 10,  32, 11, 13, 1, 0, UIMSG_PlayerCreation_FacePrev,  0, 0, "", pTexture_presleft, 0);
  pCreationUI_BtnPressLeft[1]   = pGUIWindow_CurrentMenu->CreateButton(169,  32, 11, 13, 1, 0, UIMSG_PlayerCreation_FacePrev,  1, 0, "", pTexture_presleft, 0);
  pCreationUI_BtnPressLeft[2]   = pGUIWindow_CurrentMenu->CreateButton(327,  32, 11, 13, 1, 0, UIMSG_PlayerCreation_FacePrev,  2, 0, "", pTexture_presleft, 0);
  pCreationUI_BtnPressLeft[3]   = pGUIWindow_CurrentMenu->CreateButton(486,  32, 11, 13, 1, 0, UIMSG_PlayerCreation_FacePrev,  3, 0, "", pTexture_presleft, 0);
  pCreationUI_BtnPressRight[0]  = pGUIWindow_CurrentMenu->CreateButton( 74,  32, 11, 13, 1, 0, UIMSG_PlayerCreation_FaceNext,  0, 0, "", pTexture_pressrigh, 0);
  pCreationUI_BtnPressRight[1]  = pGUIWindow_CurrentMenu->CreateButton(233,  32, 11, 13, 1, 0, UIMSG_PlayerCreation_FaceNext,  1, 0, "", pTexture_pressrigh, 0);
  pCreationUI_BtnPressRight[2]  = pGUIWindow_CurrentMenu->CreateButton(391,  32, 11, 13, 1, 0, UIMSG_PlayerCreation_FaceNext,  2, 0, "", pTexture_pressrigh, 0);
  pCreationUI_BtnPressRight[3]  = pGUIWindow_CurrentMenu->CreateButton(549,  32, 11, 13, 1, 0, UIMSG_PlayerCreation_FaceNext,  3, 0, "", pTexture_pressrigh, 0);
  pCreationUI_BtnPressLeft2[0]  = pGUIWindow_CurrentMenu->CreateButton( 10, 103, 11, 13, 1, 0, UIMSG_PlayerCreation_VoicePrev, 0, 0, "", pTexture_presleft, 0);
  pCreationUI_BtnPressLeft2[1]  = pGUIWindow_CurrentMenu->CreateButton(169, 103, 11, 13, 1, 0, UIMSG_PlayerCreation_VoicePrev, 1, 0, "", pTexture_presleft, 0);
  pCreationUI_BtnPressLeft2[2]  = pGUIWindow_CurrentMenu->CreateButton(327, 103, 11, 13, 1, 0, UIMSG_PlayerCreation_VoicePrev, 2, 0, "", pTexture_presleft, 0);
  pCreationUI_BtnPressLeft2[3]  = pGUIWindow_CurrentMenu->CreateButton(486, 103, 11, 13, 1, 0, UIMSG_PlayerCreation_VoicePrev, 3, 0, "", pTexture_presleft, 0);
  pCreationUI_BtnPressRight2[0] = pGUIWindow_CurrentMenu->CreateButton( 74, 103, 11, 13, 1, 0, UIMSG_PlayerCreation_VoiceNext, 0, 0, "", pTexture_pressrigh, 0);
  pCreationUI_BtnPressRight2[1] = pGUIWindow_CurrentMenu->CreateButton(233, 103, 11, 13, 1, 0, UIMSG_PlayerCreation_VoiceNext, 1, 0, "", pTexture_pressrigh, 0);
  pCreationUI_BtnPressRight2[2] = pGUIWindow_CurrentMenu->CreateButton(391, 103, 11, 13, 1, 0, UIMSG_PlayerCreation_VoiceNext, 2, 0, "", pTexture_pressrigh, 0);
  pCreationUI_BtnPressRight2[3] = pGUIWindow_CurrentMenu->CreateButton(549, 103, 11, 13, 1, 0, UIMSG_PlayerCreation_VoiceNext, 3, 0, "", pTexture_pressrigh, 0);

  uControlParam = 0;
  uX = 8;
  do
  {
    pGUIWindow_CurrentMenu->CreateButton(uX, 308,          150, v0, 1, 0, UIMSG_48,                            uControlParam, 0, "", 0);
    pGUIWindow_CurrentMenu->CreateButton(uX, v0 + 308,     150, v0, 1, 0, UIMSG_49,                            uControlParam, 0, "", 0);
    pGUIWindow_CurrentMenu->CreateButton(uX, 2 * v0 + 308, 150, v0, 1, 0, UIMSG_PlayerCreationRemoveUpSkill,   uControlParam, 0, "", 0);
    pGUIWindow_CurrentMenu->CreateButton(uX, 3 * v0 + 308, 150, v0, 1, 0, UIMSG_PlayerCreationRemoveDownSkill, uControlParam, 0, "", 0);
    uX += 158;
    ++uControlParam;
  }
  while ( (signed int)uX < window->GetWidth() );

  pGUIWindow_CurrentMenu->CreateButton(  5, 21, 153, 365, 1, 0, UIMSG_PlayerCreation_SelectAttribute, 0, '1', "", 0);
  pGUIWindow_CurrentMenu->CreateButton(163, 21, 153, 365, 1, 0, UIMSG_PlayerCreation_SelectAttribute, 1, '2', "", 0);
  pGUIWindow_CurrentMenu->CreateButton(321, 21, 153, 365, 1, 0, UIMSG_PlayerCreation_SelectAttribute, 2, '3', "", 0);
  pGUIWindow_CurrentMenu->CreateButton(479, 21, 153, 365, 1, 0, UIMSG_PlayerCreation_SelectAttribute, 3, '4', "", 0);

  uX = 23;
  uControlParam = 2;
  do
  {
    pGUIWindow_CurrentMenu->CreateButton(uX, 169,          120, 20, 1, 0, UIMSG_0, uControlParam - 2, 0, "", 0);
    pGUIWindow_CurrentMenu->CreateButton(uX, v0 + 169,     120, 20, 1, 0, UIMSG_0, uControlParam - 1, 0, "", 0);
    pGUIWindow_CurrentMenu->CreateButton(uX, 2 * v0 + 169, 120, 20, 1, 0, UIMSG_0, uControlParam, 0, "", 0);
    pGUIWindow_CurrentMenu->CreateButton(uX, 3 * v0 + 169, 120, 20, 1, 0, UIMSG_0, uControlParam + 1, 0, "", 0);
    pGUIWindow_CurrentMenu->CreateButton(uX, 4 * v0 + 169, 120, 20, 1, 0, UIMSG_0, uControlParam + 2, 0, "", 0);
    pGUIWindow_CurrentMenu->CreateButton(uX, 5 * v0 + 169, 120, 20, 1, 0, UIMSG_0, uControlParam + 3, 0, "", 0);
    pGUIWindow_CurrentMenu->CreateButton(uX, 6 * v0 + 169, 120, 20, 1, 0, UIMSG_0, uControlParam + 4, 0, "", 0);
    uControlParam += 7;
    uX += 158;
  }
  while ( (signed int)uControlParam < 30 );
  pGUIWindow_CurrentMenu->_41D08F_set_keyboard_control_group(28, 0, 7, 40);

  pGUIWindow_CurrentMenu->CreateButton(323, 417,          65, v0, 1, 0, UIMSG_PlayerCreationSelectClass, 0,    0, "", 0);
  pGUIWindow_CurrentMenu->CreateButton(323, v0 + 417,     65, v0, 1, 0, UIMSG_PlayerCreationSelectClass, 0xC,  0, "", 0);
  pGUIWindow_CurrentMenu->CreateButton(323, 2 * v0 + 417, 65, v0, 1, 0, UIMSG_PlayerCreationSelectClass, 0x14, 0, "", 0);
  pGUIWindow_CurrentMenu->CreateButton(388, 417,          65, v0, 1, 0, UIMSG_PlayerCreationSelectClass, 0x18, 0, "", 0);
  pGUIWindow_CurrentMenu->CreateButton(388, v0 + 417,     65, v0, 1, 0, UIMSG_PlayerCreationSelectClass, 0x1C, 0, "", 0);
  pGUIWindow_CurrentMenu->CreateButton(388, 2 * v0 + 417, 65, v0, 1, 0, UIMSG_PlayerCreationSelectClass, 0x20, 0, "", 0);
  pGUIWindow_CurrentMenu->CreateButton(453, 417,          65, v0, 1, 0, UIMSG_PlayerCreationSelectClass, 0x10, 0, "", 0);
  pGUIWindow_CurrentMenu->CreateButton(453, v0 + 417,     65, v0, 1, 0, UIMSG_PlayerCreationSelectClass, 8,    0, "", 0);
  pGUIWindow_CurrentMenu->CreateButton(453, 2 * v0 + 417, 65, v0, 1, 0, UIMSG_PlayerCreationSelectClass, 4,    0, "", 0);

  uControlParam = 0;
  do
  {
    uX = -5;
    if ( uControlParam <= 3 )
      uX = 0;
    pGUIWindow_CurrentMenu->CreateButton(100 * (uControlParam / 3) + uX + 17, v0 * (uControlParam % 3) + 417, 100, v0, 1, 0, UIMSG_PlayerCreationSelectActiveSkill,
      uControlParam, 0, "", 0);
    ++uControlParam;
  }
  while ( uControlParam < 9 );

  pPlayerCreationUI_BtnOK    = pGUIWindow_CurrentMenu->CreateButton(580, 431, 51, 39, 1, 0, UIMSG_PlayerCreationClickOK, 0, '\r', "", pIcons_LOD->GetTexture(uTextureID_BUTTMAKE), 0);
  pPlayerCreationUI_BtnReset = pGUIWindow_CurrentMenu->CreateButton(527, 431, 51, 39, 1, 0, UIMSG_PlayerCreationClickReset, 0, 'C', "", pIcons_LOD->GetTexture(uTextureID_BUTTMAKE2), 0);
  pPlayerCreationUI_BtnMinus = pGUIWindow_CurrentMenu->CreateButton(523, 393, 20, 35, 1, 0, UIMSG_PlayerCreationClickMinus, 0, '-', "", pTexture_buttminu, 0);
  pPlayerCreationUI_BtnPlus  = pGUIWindow_CurrentMenu->CreateButton(613, 393, 20, 35, 1, 0, UIMSG_PlayerCreationClickPlus, 1, '+', "", pTexture_buttplus, 0);

  pFontCChar = LoadFont("cchar.fnt", "FONTPAL", NULL);
}
// 4E28F8: using guessed type int pCurrentScreen;

//----- (0049750E) --------------------------------------------------------
void DeleteCCharFont()
{
  free(pFontCChar);
  pFontCChar = 0;
}
//----- (00497526) --------------------------------------------------------
bool PlayerCreationUI_Loop()
{
  LONG uMouseX; // edi@6
  LONG uMouseY; // eax@6
  //GUIButton *pControlsHead; // edx@6
  //int pControlParam; // esi@12
  signed int v8; // edi@30
  int v9; // edx@31
//  char *v10; // ebx@37
  ItemGen item; // [sp+Ch] [bp-74h]@37
  char v20[32]; // [sp+30h] [bp-50h]@29
  MSG Msg; // [sp+50h] [bp-30h]@17
  POINT v25; // [sp+6Ch] [bp-14h]@6
  bool party_not_creation_flag; // [sp+74h] [bp-Ch]@1

  party_not_creation_flag = false;
  pTexture_PCX.Release();
  pTexture_PCX.Load("makeme.pcx", 0);

  pGUIWindow_CurrentMenu->receives_keyboard_input_2 = WINDOW_INPUT_NONE;
  SetCurrentMenuID(MENU_CREATEPARTY);
  while ( GetCurrentMenuID() == MENU_CREATEPARTY )
  {
    uMouseX = pMouse->GetCursorPos(&v25)->x;
    uMouseY = pMouse->GetCursorPos(&v25)->y;
    //pControlsHead = pGUIWindow_CurrentMenu->pControlsHead;

    //does nothing actually
    /*if ( pControlsHead != (GUIButton *)v1 )
    {
      pNumMessage = pMessageQueue_50CBD0->uNumMessages;
      do
      {
        if ( uMouseX >= (signed int)pControlsHead->uX && uMouseX <= (signed int)pControlsHead->uZ 
            && uMouseY >= (signed int)pControlsHead->uY && uMouseY <= (signed int)pControlsHead->uW )//mouse movement
        {
          pControlParam = pControlsHead->uControlParam;
          pMessageQueue_50CBD0->AddMessage((UIMessageType)pControlsHead->field_1C, pControlParam, 0);
          v1 = 0;
        }
        pControlsHead = pControlsHead->pNext;
      }
      while ( pControlsHead != (GUIButton *)v1 );
    }*/

    while ( PeekMessageA(&Msg, 0, 0, 0, PM_REMOVE) )
    {
      if ( Msg.message == WM_QUIT )
        Game_DeinitializeAndTerminate(0);
      TranslateMessage(&Msg);
      DispatchMessageA(&Msg);
    }
    if (dword_6BE364_game_settings_1 & GAME_SETTINGS_APP_INACTIVE)
      WaitMessage();
    else
    {
      PlayerCreationUI_Draw();
      GUI_MainMenuMessageProc();
      pRenderer->BeginScene();
      GUI_UpdateWindows();
      pRenderer->EndScene();
      pRenderer->Present();
      if ( uGameState == GAME_FINISHED )//if click Esc in PlayerCreation Window
      {
        party_not_creation_flag = true;
        SetCurrentMenuID(MENU_MAIN);
        continue;
      }
      if ( uGameState == GAME_STATE_STARTING_NEW_GAME )//if click OK in PlayerCreation Window
      {
        uGameState = GAME_STATE_PLAYING;
        SetCurrentMenuID(MENU_NEWGAME);
        continue;
      }
    }
  }
  pTexture_PCX.Release();
  pGUIWindow_CurrentMenu->Release();
  pIcons_LOD->RemoveTexturesPackFromTextureList();

  memset(v20, 0, 32);
  for ( int i = 0; i < 32; i++ )
  {
    for ( v8 = 0; v8 < 10; ++v8 )
    {
      v9 = rand() % 32;
      if ( !v20[v9] )
        break;
    }
    if ( v8 == 10 )
    {
      v9 = 0;
      if ( v20[0] )
      {
        do
          ++v9;
        while ( v20[v9] );
      }
    }
    pParty->field_854[i] = v9;
    v20[v9] = 1;
  }

  item.Reset();
  for (uint i = 0; i < 4; ++i)
  {
    if (pParty->pPlayers[i].classType == PLAYER_CLASS_KNIGHT)
      pParty->pPlayers[i].sResMagicBase = 10;
    pParty->pPlayers[i].pPlayerBuffs[22].uExpireTime = 0;
    for (uint j = 0; j < 9; j++)
    {
      if (pParty->pPlayers[i].pActiveSkills[PLAYER_SKILL_FIRE + j])
      {
        pParty->pPlayers[i].lastOpenedSpellbookPage = j;
        break;
      }
    }
    pItemsTable->GenerateItem(2, 40, &item);
    pParty->pPlayers[i].AddItem2(-1, &item);

    pParty->pPlayers[i].sHealth = pParty->pPlayers[i].GetMaxHealth();
    pParty->pPlayers[i].sMana = pParty->pPlayers[i].GetMaxMana();
    for (uint j = 0; j < 37; ++j)
    {
      if (!pParty->pPlayers[i].pActiveSkills[j])
        continue;

      switch (j)
      {
        case PLAYER_SKILL_STAFF:   pParty->pPlayers[i].AddItem(-1, 61); break;
        case PLAYER_SKILL_SWORD:   pParty->pPlayers[i].AddItem(-1, 1); break;
        case PLAYER_SKILL_DAGGER:  pParty->pPlayers[i].AddItem(-1, 15); break;
        case PLAYER_SKILL_AXE:     pParty->pPlayers[i].AddItem(-1, 23); break;
        case PLAYER_SKILL_SPEAR:   pParty->pPlayers[i].AddItem(-1, 31); break;
        case PLAYER_SKILL_BOW:     pParty->pPlayers[i].AddItem(-1, 47); break;
        case PLAYER_SKILL_MACE:    pParty->pPlayers[i].AddItem(-1, 50); break;
        case PLAYER_SKILL_BLASTER: Error("No blasters at startup :p");
        case PLAYER_SKILL_SHIELD:  pParty->pPlayers[i].AddItem(-1, 84); break;
        case PLAYER_SKILL_LEATHER: pParty->pPlayers[i].AddItem(-1, 66); break;
        case PLAYER_SKILL_CHAIN:   pParty->pPlayers[i].AddItem(-1, 71); break;
        case PLAYER_SKILL_PLATE:   pParty->pPlayers[i].AddItem(-1, 76); break;
        case PLAYER_SKILL_FIRE:
          pParty->pPlayers[i].AddItem(-1, 0x191);
          pParty->pPlayers[i].spellbook.pFireSpellbook.bIsSpellAvailable[0] = true;
        break;
        case PLAYER_SKILL_AIR:
          pParty->pPlayers[i].AddItem(-1, 0x19C);
          pParty->pPlayers[i].spellbook.pAirSpellbook.bIsSpellAvailable[0] = true;
        break;
        case PLAYER_SKILL_WATER:
          pParty->pPlayers[i].AddItem(-1, 0x1A7);
          pParty->pPlayers[i].spellbook.pWaterSpellbook.bIsSpellAvailable[0] = true;
        break;
        case PLAYER_SKILL_EARTH:
          pParty->pPlayers[i].AddItem(-1, 0x1B2);
          pParty->pPlayers[i].spellbook.pEarthSpellbook.bIsSpellAvailable[0] = true;
        break;
        case PLAYER_SKILL_SPIRIT:
          pParty->pPlayers[i].AddItem(-1, 0x1BD);
          pParty->pPlayers[i].spellbook.pSpiritSpellbook.bIsSpellAvailable[0] = true;
        break;
        case PLAYER_SKILL_MIND:
          pParty->pPlayers[i].AddItem(-1, 0x1C8);
          pParty->pPlayers[i].spellbook.pMindSpellbook.bIsSpellAvailable[0] = true;
        break;
        case PLAYER_SKILL_BODY:
          pParty->pPlayers[i].AddItem(-1, 0x1D3);
          pParty->pPlayers[i].spellbook.pBodySpellbook.bIsSpellAvailable[0] = true;
        break;
        case PLAYER_SKILL_LIGHT:
        case PLAYER_SKILL_DARK:
        case PLAYER_SKILL_DIPLOMACY:
          Error("No dimoplacy in mm7 (yet)");
        break;
        case PLAYER_SKILL_ITEM_ID:
        case PLAYER_SKILL_REPAIR:
        case PLAYER_SKILL_MEDITATION:
        case PLAYER_SKILL_PERCEPTION:
        case PLAYER_SKILL_TRAP_DISARM:
        case PLAYER_SKILL_LEARNING:
          pParty->pPlayers[i].AddItem(-1, 0xDC);
          pParty->pPlayers[i].AddItem(-1, 5 * (rand() % 3 + 40));
        break;
        case PLAYER_SKILL_DODGE:   pParty->pPlayers[i].AddItem(-1, 115); break;
        case PLAYER_SKILL_UNARMED: pParty->pPlayers[i].AddItem(-1, 110); break;
        default:
          break;
      }

      for (uint k = 0; k < 138; k++)
      {
        if (pParty->pPlayers[i].pOwnItems[k].uItemID)
          pParty->pPlayers[i].pOwnItems[k].SetIdentified();
      }
    }
  }

  pAudioPlayer->StopChannels(-1, -1);
  return party_not_creation_flag;
}