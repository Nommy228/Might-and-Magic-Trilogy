#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif


#include "Keyboard.h"
#include "GUIWindow.h"
#include "Game.h"
#include "OSInfo.h"

#include "mm7_data.h"
#include "Vis.h"
#include "MM7.h"
#include "Actor.h"
#include "DecorationList.h"
#include "Indoor.h"
#include "viewport.h"
#include "AudioPlayer.h"
#include "Level/Decoration.h"


struct KeyboardActionMapping *pKeyActionMap;
struct AsyncKeyboard *pAsyncKeyboard;





//----- (00459C68) --------------------------------------------------------
void KeyboardActionMapping::SetKeyMapping(int uAction, int vKey, KeyToggleType type)
{
  pVirtualKeyCodesMapping[uAction] = vKey;
  pToggleTypes[uAction] = type;
}

//----- (00459C82) --------------------------------------------------------
unsigned int KeyboardActionMapping::GetActionVKey(enum InputAction eAction)
{
  return this->pVirtualKeyCodesMapping[eAction];
}

//----- (00459C8D) --------------------------------------------------------
KeyboardActionMapping::KeyboardActionMapping()
{
  uLastKeyPressed = 0;
  field_204 = 0;
  pWindow = nullptr;

  SetDefaultMapping();
  ReadMappings();

  ResetKeys();

  uGameMenuUI_CurentlySelectedKeyIdx = -1;
}
// 506E68: using guessed type int uGameMenuUI_CurentlySelectedKeyIdx;

//----- (00459CC4) --------------------------------------------------------
void KeyboardActionMapping::SetDefaultMapping()
{
  pVirtualKeyCodesMapping[0] = VK_UP;
  pToggleTypes[0] = TOGGLE_Continuously;
  pVirtualKeyCodesMapping[1] = VK_DOWN;
  pToggleTypes[1] = TOGGLE_Continuously;
  pVirtualKeyCodesMapping[2] = VK_LEFT;
  pToggleTypes[2] = TOGGLE_Continuously;
  pVirtualKeyCodesMapping[3] = VK_RIGHT;
  pToggleTypes[3] = TOGGLE_Continuously;
  pVirtualKeyCodesMapping[8] = 'A';
  pToggleTypes[8] = TOGGLE_OneTimePress;
  pVirtualKeyCodesMapping[7] = 'S';
  pToggleTypes[7] = TOGGLE_OneTimePress;
  pVirtualKeyCodesMapping[4] = 'Y';
  pToggleTypes[4] = TOGGLE_OneTimePress;
  pVirtualKeyCodesMapping[5] = 'X';
  pToggleTypes[5] = TOGGLE_OneTimePress;
  pVirtualKeyCodesMapping[6] = VK_RETURN;
  pToggleTypes[6] = TOGGLE_OneTimePress;
  pVirtualKeyCodesMapping[9] = VK_SPACE;
  pToggleTypes[9] = TOGGLE_OneTimePress;
  pVirtualKeyCodesMapping[10] = 'C';
  pToggleTypes[10] = TOGGLE_OneTimePress;
  pVirtualKeyCodesMapping[11] = 'B';
  pToggleTypes[11] = TOGGLE_OneTimePress;
  pVirtualKeyCodesMapping[12] = VK_TAB;
  pToggleTypes[12] = TOGGLE_OneTimePress;
  pVirtualKeyCodesMapping[13] = 'Q';
  pToggleTypes[13] = TOGGLE_OneTimePress;
  pVirtualKeyCodesMapping[14] = 'Z';
  pToggleTypes[14] = TOGGLE_OneTimePress;
  pVirtualKeyCodesMapping[15] = 'R';
  pToggleTypes[15] = TOGGLE_OneTimePress;
  pVirtualKeyCodesMapping[16] = 'T';
  pToggleTypes[16] = TOGGLE_OneTimePress;
  pVirtualKeyCodesMapping[17] = 'N';
  pToggleTypes[17] = TOGGLE_OneTimePress;
  pVirtualKeyCodesMapping[18] = 'M';
  pToggleTypes[18] = TOGGLE_OneTimePress;
  pVirtualKeyCodesMapping[20] = VK_NEXT;
  pToggleTypes[20] = TOGGLE_Continuously;
  pVirtualKeyCodesMapping[21] = VK_DELETE;
  pToggleTypes[21] = TOGGLE_Continuously;

  SetKeyMapping(INPUT_CenterView, VK_END, TOGGLE_Continuously);
  SetKeyMapping(INPUT_ZoomIn, VK_ADD, TOGGLE_OneTimePress);
  SetKeyMapping(INPUT_ZoomOut, VK_SUBTRACT, TOGGLE_OneTimePress);
  SetKeyMapping(INPUT_FlyUp, VK_PRIOR, TOGGLE_Continuously);
  SetKeyMapping(INPUT_FlyDown, VK_INSERT, TOGGLE_Continuously);
  SetKeyMapping(INPUT_Land, VK_HOME, TOGGLE_OneTimePress);
  SetKeyMapping(INPUT_AlwaysRun, 'U', TOGGLE_OneTimePress);
  SetKeyMapping(INPUT_StrafeLeft, VK_OEM_4, TOGGLE_Continuously);
  SetKeyMapping(INPUT_StrafeRight, VK_OEM_6, TOGGLE_Continuously);
}

//----- (00459E3F) --------------------------------------------------------
void KeyboardActionMapping::ResetKeys()
{
  for (uint i = 0; i < 30; ++i)
    GetAsyncKeyState(pVirtualKeyCodesMapping[i]);
}

//----- (00459E5A) --------------------------------------------------------
void KeyboardActionMapping::EnterText(int a2, int max_string_len, GUIWindow *pWindow)
{
  KeyboardActionMapping *v4; // esi@1

  v4 = this;
  memset(this->pPressedKeysBuffer, 0, 0x101u);
  v4->uNumKeysPressed = 0;
  if ( a2 )
    v4->field_204 = 2;
  else
    v4->field_204 = 1;
  v4->max_input_string_len = max_string_len;
  v4->pWindow = pWindow;
  pWindow->receives_keyboard_input_2 = WINDOW_INPUT_IN_PROGRESS;
}

//----- (00459ED1) --------------------------------------------------------
void KeyboardActionMapping::SetWindowInputStatus(int a2)
{
  field_204 = 0;
  if ( pWindow )
    pWindow->receives_keyboard_input_2 = a2;
}

//----- (00459F10) --------------------------------------------------------
bool KeyboardActionMapping::_459F10(unsigned int a2)
{
  int v3; // [sp-4h] [bp-4h]@3

  pKeyActionMap->uLastKeyPressed = a2;
  if ( uGameMenuUI_CurentlySelectedKeyIdx == -1 )
  {
    if ( pKeyActionMap->field_204 == 1 )
    {
      if ( a2 != 8 )
      {
        if ( a2 == 9 )
          return 1;
        if ( a2 == 13 )
          goto LABEL_3;
        if ( a2 == 27 )
          goto LABEL_15;
        if ( this->uNumKeysPressed >= this->max_input_string_len )
          return 1;
        pKeyActionMap->pPressedKeysBuffer[pKeyActionMap->uNumKeysPressed] = a2;
        ++pKeyActionMap->uNumKeysPressed;
LABEL_24:
        pKeyActionMap->pPressedKeysBuffer[pKeyActionMap->uNumKeysPressed] = 0;
        return 1;
      }
    }
    else
    {
      if ( pKeyActionMap->field_204 != 2 )
        return 0;
      if ( a2 != 8 )
      {
        if ( a2 == 13 )
          goto LABEL_3;
        if ( a2 != 27 )
        {
          if ( (signed int)a2 >= 48 && (signed int)a2 <= 57 )
          {
            if ( pKeyActionMap->uNumKeysPressed < this->max_input_string_len )
            {
              pKeyActionMap->pPressedKeysBuffer[pKeyActionMap->uNumKeysPressed] = a2;
              ++pKeyActionMap->uNumKeysPressed;
            }
          }
          return 1;
        }
LABEL_15:
        pKeyActionMap->SetWindowInputStatus(WINDOW_INPUT_CANCELLED);
        return 1;
      }
    }
    if ( !pKeyActionMap->uNumKeysPressed )
      return 1;
    --pKeyActionMap->uNumKeysPressed;
    goto LABEL_24;
  }
  pKeyActionMap->pPressedKeysBuffer[pKeyActionMap->uNumKeysPressed] = a2;
  ++pKeyActionMap->uNumKeysPressed;
  pKeyActionMap->pPressedKeysBuffer[pKeyActionMap->uNumKeysPressed] = 0;

LABEL_3:
  pKeyActionMap->SetWindowInputStatus(WINDOW_INPUT_CONFIRMED);
  return 1;
}
// 506E68: using guessed type int uGameMenuUI_CurentlySelectedKeyIdx;

//----- (00459FFC) --------------------------------------------------------
void KeyboardActionMapping::ReadMappings()
{
  //KeyboardActionMapping *v1; // esi@1
  int v2; // eax@2
  unsigned int v3; // eax@3
  int v4; // eax@6
  unsigned int v5; // eax@7
  int v6; // eax@10
  unsigned int v7; // eax@11
  int v8; // eax@14
  unsigned int v9; // eax@15
  int v10; // eax@18
  unsigned int v11; // eax@19
  int v12; // eax@22
  //unsigned int v13; // eax@23
  int v14; // eax@26
  //unsigned int v15; // eax@27
  int v16; // eax@30
  //unsigned int v17; // eax@31
  int v18; // eax@34
  //unsigned int v19; // eax@35
  int v20; // eax@38
  //unsigned int v21; // eax@39
  int v22; // eax@42
  //unsigned int v23; // eax@43
  int v24; // eax@46
  //unsigned int v25; // eax@47
  int v26; // eax@50
  //unsigned int v27; // eax@51
  int v28; // eax@54
  //unsigned int v29; // eax@55
  int v30; // eax@58
  //unsigned int v31; // eax@59
  int v32; // eax@62
  //unsigned int v33; // eax@63
  int v34; // eax@66
  //unsigned int v35; // eax@67
  int v36; // eax@70
  //unsigned int v37; // eax@71
  int v38; // eax@74
  //unsigned int v39; // eax@75
  int v40; // eax@78
  //unsigned int v41; // eax@79
  int v42; // eax@82
  //unsigned int v43; // eax@83
  int v44; // eax@86
  //unsigned int v45; // eax@87
  int v46; // eax@90
  //unsigned int v47; // eax@91
  int v48; // eax@94
  //unsigned int v49; // eax@95
  int v50; // eax@98
  //unsigned int v51; // eax@99
  int v52; // eax@102
  //unsigned int v53; // eax@103
  int v54; // eax@106
  //unsigned int v55; // eax@107
  int v56; // eax@110
  unsigned int v57; // eax@111
  int v58; // eax@114
  unsigned int v59; // eax@115
  int v60; // eax@118
  unsigned int v61; // eax@119
  //char str[32]; // [sp+Ch] [bp-20h]@1

  char str[32];

  ReadWindowsRegistryString("KEY_FORWARD", str, 32, "DEFAULT");
  if ( strcmp(str, "DEFAULT") && (LOBYTE(v2) = GetActionDefaultVKey(str), v2 != -1) )
  {
    pVirtualKeyCodesMapping[0] = GetActionDefaultVKey(str);
  }
  else
  {
    pVirtualKeyCodesMapping[0] = VK_UP;
  }
  pToggleTypes[0] = TOGGLE_Continuously;
  ReadWindowsRegistryString("KEY_BACKWARD", str, 0x20u, "DEFAULT");
  if ( strcmp(str, "DEFAULT") && (LOBYTE(v4) = GetActionDefaultVKey(str), v4 != -1) )
  {
    pVirtualKeyCodesMapping[1] = GetActionDefaultVKey(str);
  }
  else
  {
    pVirtualKeyCodesMapping[1] = VK_DOWN;
  }
  pToggleTypes[1] = TOGGLE_Continuously;

  ReadWindowsRegistryString("KEY_LEFT", str, 0x20u, "DEFAULT");
  if ( strcmp(str, "DEFAULT") && (LOBYTE(v6) = GetActionDefaultVKey(str), v6 != -1) )
  {
    pVirtualKeyCodesMapping[2] = GetActionDefaultVKey(str);
  }
  else
  {
    pVirtualKeyCodesMapping[2] = VK_LEFT;
  }
  pToggleTypes[2] = TOGGLE_Continuously;

  ReadWindowsRegistryString("KEY_RIGHT", str, 0x20u, "DEFAULT");
  if ( strcmp(str, "DEFAULT") && (LOBYTE(v8) = GetActionDefaultVKey(str), v8 != -1) )
  {
    pVirtualKeyCodesMapping[3] = GetActionDefaultVKey(str);
  }
  else
  {
    pVirtualKeyCodesMapping[3] = VK_RIGHT;
  }
  pToggleTypes[3] = TOGGLE_Continuously;

  ReadWindowsRegistryString("KEY_ATTACK", str, 0x20u, "DEFAULT");
  if ( strcmp(str, "DEFAULT") && (LOBYTE(v10) = GetActionDefaultVKey(str), v10 != -1) )
  {
    pVirtualKeyCodesMapping[8] = GetActionDefaultVKey(str);
  }
  else
  {
    pVirtualKeyCodesMapping[8] = 'A';
  }
  pToggleTypes[8] = TOGGLE_OneTimePress;

  ReadWindowsRegistryString("KEY_CASTREADY", str, 0x20u, "DEFAULT");
  if ( strcmp(str, "DEFAULT") && (LOBYTE(v12) = GetActionDefaultVKey(str), v12 != -1) )
  {
    pVirtualKeyCodesMapping[7] = GetActionDefaultVKey(str);
  }
  else
  {
    pVirtualKeyCodesMapping[7] = 'S';
  }
  pToggleTypes[7] = TOGGLE_OneTimePress;

  ReadWindowsRegistryString("KEY_YELL", str, 0x20u, "DEFAULT");
  if ( strcmp(str, "DEFAULT") && (LOBYTE(v14) = GetActionDefaultVKey(str), v14 != -1) )
  {
    pVirtualKeyCodesMapping[4] = GetActionDefaultVKey(str);
  }
  else
  {
    pVirtualKeyCodesMapping[4] = 'Y';
  }
  pToggleTypes[4] = TOGGLE_OneTimePress;

  ReadWindowsRegistryString("KEY_JUMP", str, 0x20u, "DEFAULT");
  if ( strcmp(str, "DEFAULT") && (LOBYTE(v16) = GetActionDefaultVKey(str), v16 != -1) )
  {
    pVirtualKeyCodesMapping[5] = GetActionDefaultVKey(str);
  }
  else
  {
    pVirtualKeyCodesMapping[5] = 'X';
  }
  pToggleTypes[5] = TOGGLE_OneTimePress;

  ReadWindowsRegistryString("KEY_COMBAT", str, 0x20u, "DEFAULT");
  if ( strcmp(str, "DEFAULT") && (LOBYTE(v18) = GetActionDefaultVKey(str), v18 != -1) )
  {
    pVirtualKeyCodesMapping[6] = GetActionDefaultVKey(str);
  }
  else
  {
    pVirtualKeyCodesMapping[6] = VK_RETURN;
  }
  pToggleTypes[6] = TOGGLE_OneTimePress;

  ReadWindowsRegistryString("KEY_EVENTTRIGGER", str, 0x20u, "DEFAULT");
  if ( strcmp(str, "DEFAULT") && (LOBYTE(v20) = GetActionDefaultVKey(str), v20 != -1) )
  {
    pVirtualKeyCodesMapping[9] = GetActionDefaultVKey(str);
  }
  else
  {
    pVirtualKeyCodesMapping[9] = VK_SPACE;
  }
  pToggleTypes[9] = TOGGLE_OneTimePress;

  ReadWindowsRegistryString("KEY_CAST", str, 0x20u, "DEFAULT");
  if ( strcmp(str, "DEFAULT") && (LOBYTE(v22) = GetActionDefaultVKey(str), v22 != -1) )
  {
    pVirtualKeyCodesMapping[10] = GetActionDefaultVKey(str);
  }
  else
  {
    pVirtualKeyCodesMapping[10] = 'C';
  }
  pToggleTypes[10] = TOGGLE_OneTimePress;

  ReadWindowsRegistryString("KEY_PASS", str, 0x20u, "DEFAULT");
  if ( strcmp(str, "DEFAULT") && (LOBYTE(v24) = GetActionDefaultVKey(str), v24 != -1) )
  {
    pVirtualKeyCodesMapping[11] = GetActionDefaultVKey(str);
  }
  else
  {
    pVirtualKeyCodesMapping[11] = 'B';
  }
  pToggleTypes[11] = TOGGLE_OneTimePress;

  ReadWindowsRegistryString("KEY_CHARCYCLE", str, 0x20u, "DEFAULT");
  if ( strcmp(str, "DEFAULT") && (LOBYTE(v26) = GetActionDefaultVKey(str), v26 != -1) )
  {
    pVirtualKeyCodesMapping[12] = GetActionDefaultVKey(str);
  }
  else
  {
    pVirtualKeyCodesMapping[12] = VK_TAB;
  }
  pToggleTypes[12] = TOGGLE_OneTimePress;

  ReadWindowsRegistryString("KEY_QUEST", str, 0x20u, "DEFAULT");
  if ( strcmp(str, "DEFAULT") && (LOBYTE(v28) = GetActionDefaultVKey(str), v28 != -1) )
  {
    pVirtualKeyCodesMapping[13] = GetActionDefaultVKey(str);
  }
  else
  {
    pVirtualKeyCodesMapping[13] = 'Q';
  }
  pToggleTypes[13] = TOGGLE_OneTimePress;

  ReadWindowsRegistryString("KEY_QUICKREF", str, 0x20u, "DEFAULT");
  if ( strcmp(str, "DEFAULT") && (LOBYTE(v30) = GetActionDefaultVKey(str), v30 != -1) )
  {
    pVirtualKeyCodesMapping[14] = GetActionDefaultVKey(str);
  }
  else
  {
    pVirtualKeyCodesMapping[14] = 'Z';
  }
  pToggleTypes[14] = TOGGLE_OneTimePress;

  ReadWindowsRegistryString("KEY_REST", str, 0x20u, "DEFAULT");
  if ( strcmp(str, "DEFAULT") && (LOBYTE(v32) = GetActionDefaultVKey(str), v32 != -1) )
  {
    pVirtualKeyCodesMapping[15] = GetActionDefaultVKey(str);
  }
  else
  {
    pVirtualKeyCodesMapping[15] = 'R';
  }
  pToggleTypes[15] = TOGGLE_OneTimePress;

  ReadWindowsRegistryString("KEY_TIMECAL", str, 0x20u, "DEFAULT");
  if ( strcmp(str, "DEFAULT") && (LOBYTE(v34) = GetActionDefaultVKey(str), v34 != -1) )
  {
    pVirtualKeyCodesMapping[16] = GetActionDefaultVKey(str);
  }
  else
  {
    pVirtualKeyCodesMapping[16] = 'T';
  }
  pToggleTypes[16] = TOGGLE_OneTimePress;

  ReadWindowsRegistryString("KEY_AUTONOTES", str, 0x20u, "DEFAULT");
  if ( strcmp(str, "DEFAULT") && (LOBYTE(v36) = GetActionDefaultVKey(str), v36 != -1) )
  {
    pVirtualKeyCodesMapping[17] = GetActionDefaultVKey(str);
  }
  else
  {
    pVirtualKeyCodesMapping[17] = 'N';
  }
  pToggleTypes[17] = TOGGLE_OneTimePress;

  ReadWindowsRegistryString("KEY_MAPBOOK", str, 0x20u, "DEFAULT");
  if ( strcmp(str, "DEFAULT") && (LOBYTE(v38) = GetActionDefaultVKey(str), v38 != -1) )
  {
    pVirtualKeyCodesMapping[18] = GetActionDefaultVKey(str);
  }
  else
  {
    pVirtualKeyCodesMapping[18] = 'M';
  }
  pToggleTypes[18] = TOGGLE_OneTimePress;

  ReadWindowsRegistryString("KEY_LOOKUP", str, 0x20u, "DEFAULT");
  if ( strcmp(str, "DEFAULT") && (LOBYTE(v40) = GetActionDefaultVKey(str), v40 != -1) )
  {
    pVirtualKeyCodesMapping[20] = GetActionDefaultVKey(str);
  }
  else
  {
    pVirtualKeyCodesMapping[20] = VK_NEXT;
  }
  pToggleTypes[20] = TOGGLE_OneTimePress;

  ReadWindowsRegistryString("KEY_LOOKDOWN", str, 0x20u, "DEFAULT");
  if ( strcmp(str, "DEFAULT") && (LOBYTE(v42) = GetActionDefaultVKey(str), v42 != -1) )
  {
    pVirtualKeyCodesMapping[21] = GetActionDefaultVKey(str);
  }
  else
  {
    pVirtualKeyCodesMapping[21] = VK_DELETE;
  }
  pToggleTypes[21] = TOGGLE_OneTimePress;

  ReadWindowsRegistryString("KEY_CENTERVIEWPT", str, 0x20u, "DEFAULT");
  if ( strcmp(str, "DEFAULT") && (LOBYTE(v44) = GetActionDefaultVKey(str), v44 != -1) )
  {
    pVirtualKeyCodesMapping[22] = GetActionDefaultVKey(str);
  }
  else
  {
    pVirtualKeyCodesMapping[22] = VK_END;
  }
  pToggleTypes[22] = TOGGLE_OneTimePress;

  ReadWindowsRegistryString("KEY_ZOOMIN", str, 0x20u, "DEFAULT");
  if ( strcmp(str, "DEFAULT") && (LOBYTE(v46) = GetActionDefaultVKey(str), v46 != -1) )
  {
    pVirtualKeyCodesMapping[23] = GetActionDefaultVKey(str);
  }
  else
  {
    pVirtualKeyCodesMapping[23] = VK_ADD;
  }
  pToggleTypes[23] = TOGGLE_OneTimePress;

  ReadWindowsRegistryString("KEY_ZOOMOUT", str, 0x20u, "DEFAULT");
  if ( strcmp(str, "DEFAULT") && (LOBYTE(v48) = GetActionDefaultVKey(str), v48 != -1) )
  {
    pVirtualKeyCodesMapping[24] = GetActionDefaultVKey(str);
  }
  else
  {
    pVirtualKeyCodesMapping[24] = VK_SUBTRACT;
  }
  pToggleTypes[24] = TOGGLE_OneTimePress;

  ReadWindowsRegistryString("KEY_FLYUP", str, 0x20u, "DEFAULT");
  if ( strcmp(str, "DEFAULT") && (LOBYTE(v50) = GetActionDefaultVKey(str), v50 != -1) )
  {
    pVirtualKeyCodesMapping[25] = GetActionDefaultVKey(str);
  }
  else
  {
    pVirtualKeyCodesMapping[25] = VK_PRIOR;
  }
  pToggleTypes[25] = TOGGLE_Continuously;

  ReadWindowsRegistryString("KEY_FLYDOWN", str, 0x20u, "DEFAULT");
  if ( strcmp(str, "DEFAULT") && (LOBYTE(v52) = GetActionDefaultVKey(str), v52 != -1) )
  {
    pVirtualKeyCodesMapping[26] = GetActionDefaultVKey(str);
  }
  else
  {
    pVirtualKeyCodesMapping[26] = VK_INSERT;
  }
  pToggleTypes[26] = TOGGLE_Continuously;

  ReadWindowsRegistryString("KEY_LAND", str, 0x20u, "DEFAULT");
  if ( strcmp(str, "DEFAULT") && (LOBYTE(v54) = GetActionDefaultVKey(str), v54 != -1) )
  {
    pVirtualKeyCodesMapping[27] = GetActionDefaultVKey(str);
  }
  else
  {
    pVirtualKeyCodesMapping[27] = VK_HOME;
  }
  pToggleTypes[27] = TOGGLE_OneTimePress;

  ReadWindowsRegistryString("KEY_ALWAYSRUN", str, 0x20u, "DEFAULT");
  if ( strcmp(str, "DEFAULT") && (LOBYTE(v56) = GetActionDefaultVKey(str), v56 != -1) )
  {
    pVirtualKeyCodesMapping[19] = GetActionDefaultVKey(str);
  }
  else
  {
    pVirtualKeyCodesMapping[19] = 'U';
  }
  pToggleTypes[19] = TOGGLE_OneTimePress;

  bAlwaysRun = ReadWindowsRegistryInt("valAlwaysRun", 0) != 0;
  bFlipOnExit = ReadWindowsRegistryInt("FlipOnExit", 0) != 0;
  ReadWindowsRegistryString("KEY_STEPLEFT", str, 0x20u, "DEFAULT");
  if ( strcmp(str, "DEFAULT") && (LOBYTE(v58) = GetActionDefaultVKey(str), v58 != -1) )
  {
    pVirtualKeyCodesMapping[28] = GetActionDefaultVKey(str);
    pToggleTypes[28] = TOGGLE_OneTimePress;
  }
  else
  {
    pToggleTypes[28] = TOGGLE_Continuously;
    pVirtualKeyCodesMapping[28] = VK_OEM_4;
  }

  ReadWindowsRegistryString("KEY_STEPRIGHT", str, 0x20u, "DEFAULT");
  if ( strcmp(str, "DEFAULT") && (LOBYTE(v60) = GetActionDefaultVKey(str), v60 != -1) )
  {
    pVirtualKeyCodesMapping[29] = GetActionDefaultVKey(str);
    pToggleTypes[29] = TOGGLE_OneTimePress;
  }
  else
  {
    pToggleTypes[29] = TOGGLE_Continuously;
    pVirtualKeyCodesMapping[29] = VK_OEM_6;
  }
}

//----- (0045A960) --------------------------------------------------------
void KeyboardActionMapping::StoreMappings()
{
  const char *v2; // eax@1
  const char *v3; // eax@1
  const char *v4; // eax@1
  const char *v5; // eax@1
  const char *v6; // eax@1
  const char *v7; // eax@1
  const char *v8; // eax@1
  const char *v9; // eax@1
  const char *v10; // eax@1
  const char *v11; // eax@1
  const char *v12; // eax@1
  const char *v13; // eax@1
  const char *v14; // eax@1
  const char *v15; // eax@1
  const char *v16; // eax@1
  const char *v17; // eax@1
  const char *v18; // eax@1
  const char *v19; // eax@1
  const char *v20; // eax@1
  const char *v21; // eax@1
  const char *v22; // eax@1
  const char *v23; // eax@1
  const char *v24; // eax@1
  const char *v25; // eax@1
  const char *v26; // eax@1
  const char *v27; // eax@1
  const char *v28; // eax@1
  const char *v29; // eax@1

  v2 = GetVKeyDisplayName(pVirtualKeyCodesMapping[0]);
  WriteWindowsRegistryString("KEY_FORWARD", v2);
  v3 = GetVKeyDisplayName(pVirtualKeyCodesMapping[1]);
  WriteWindowsRegistryString("KEY_BACKWARD", v3);
  v4 = GetVKeyDisplayName(pVirtualKeyCodesMapping[2]);
  WriteWindowsRegistryString("KEY_LEFT", v4);
  v5 = GetVKeyDisplayName(pVirtualKeyCodesMapping[3]);
  WriteWindowsRegistryString("KEY_RIGHT", v5);
  v6 = GetVKeyDisplayName(pVirtualKeyCodesMapping[8]);
  WriteWindowsRegistryString("KEY_ATTACK", v6);
  v7 = GetVKeyDisplayName(pVirtualKeyCodesMapping[7]);
  WriteWindowsRegistryString("KEY_CASTREADY", v7);
  v8 = GetVKeyDisplayName(pVirtualKeyCodesMapping[4]);
  WriteWindowsRegistryString("KEY_YELL", v8);
  v9 = GetVKeyDisplayName(pVirtualKeyCodesMapping[5]);
  WriteWindowsRegistryString("KEY_JUMP", v9);
  v10 = GetVKeyDisplayName(pVirtualKeyCodesMapping[6]);
  WriteWindowsRegistryString("KEY_COMBAT", v10);
  v11 = GetVKeyDisplayName(pVirtualKeyCodesMapping[9]);
  WriteWindowsRegistryString("KEY_EVENTTRIGGER", v11);
  v12 = GetVKeyDisplayName(pVirtualKeyCodesMapping[10]);
  WriteWindowsRegistryString("KEY_CAST", v12);
  v13 = GetVKeyDisplayName(pVirtualKeyCodesMapping[11]);
  WriteWindowsRegistryString("KEY_PASS", v13);
  v14 = GetVKeyDisplayName(pVirtualKeyCodesMapping[12]);
  WriteWindowsRegistryString("KEY_CHARCYCLE", v14);
  v15 = GetVKeyDisplayName(pVirtualKeyCodesMapping[13]);
  WriteWindowsRegistryString("KEY_QUEST", v15);
  v16 = GetVKeyDisplayName(pVirtualKeyCodesMapping[14]);
  WriteWindowsRegistryString("KEY_QUICKREF", v16);
  v17 = GetVKeyDisplayName(pVirtualKeyCodesMapping[15]);
  WriteWindowsRegistryString("KEY_REST", v17);
  v18 = GetVKeyDisplayName(pVirtualKeyCodesMapping[16]);
  WriteWindowsRegistryString("KEY_TIMECAL", v18);
  v19 = GetVKeyDisplayName(pVirtualKeyCodesMapping[17]);
  WriteWindowsRegistryString("KEY_AUTONOTES", v19);
  v20 = GetVKeyDisplayName(pVirtualKeyCodesMapping[18]);
  WriteWindowsRegistryString("KEY_MAPBOOK", v20);
  v21 = GetVKeyDisplayName(pVirtualKeyCodesMapping[20]);
  WriteWindowsRegistryString("KEY_LOOKUP", v21);
  v22 = GetVKeyDisplayName(pVirtualKeyCodesMapping[21]);
  WriteWindowsRegistryString("KEY_LOOKDOWN", v22);
  v23 = GetVKeyDisplayName(pVirtualKeyCodesMapping[22]);
  WriteWindowsRegistryString("KEY_CENTERVIEWPT", v23);
  v24 = GetVKeyDisplayName(pVirtualKeyCodesMapping[23]);
  WriteWindowsRegistryString("KEY_ZOOMIN", v24);
  v25 = GetVKeyDisplayName(pVirtualKeyCodesMapping[24]);
  WriteWindowsRegistryString("KEY_ZOOMOUT", v25);
  v26 = GetVKeyDisplayName(pVirtualKeyCodesMapping[25]);
  WriteWindowsRegistryString("KEY_FLYUP", v26);
  v27 = GetVKeyDisplayName(pVirtualKeyCodesMapping[26]);
  WriteWindowsRegistryString("KEY_FLYDOWN", v27);
  v28 = GetVKeyDisplayName(pVirtualKeyCodesMapping[27]);
  WriteWindowsRegistryString("KEY_LAND", v28);
  v29 = GetVKeyDisplayName(pVirtualKeyCodesMapping[19]);
  WriteWindowsRegistryString("KEY_ALWAYSRUN", v29);
}

//----- (0045ABCA) --------------------------------------------------------
const unsigned __int8 KeyboardActionMapping::GetActionDefaultVKey(const char *Str)
{
  unsigned __int8 result; // al@3

  if ( !strcmp(Str, "UP") )
    return VK_UP;
  if ( !strcmp(Str, "DOWN") )
    return VK_DOWN;
  if (!strcmp(Str, "бкебн") || !strcmp(Str, "LEFT"))
    return VK_LEFT;
  if (!strcmp(Str, "бопюбн") || !strcmp(Str, "RIGHT"))
    return VK_RIGHT;
  if ( !strcmp(Str, "RETURN") )
    return VK_RETURN;
  if ( !strcmp(Str, "SPACE") )
    return VK_SPACE;
  if ( !strcmp(Str, "PAGE_DOWN") )
    return VK_NEXT;
  if ( !strcmp(Str, "PAGE_UP") )
    return VK_PRIOR;
  if ( !strcmp(Str, "TAB") )
    return VK_TAB;
  if ( !strcmp(Str, "SUBTRACT") )
    return VK_SUBTRACT;
  if ( !strcmp(Str, "ADD") )
    return VK_ADD;
  if ( !strcmp(Str, "END") )
    return VK_END;
  if ( !strcmp(Str, "DELETE") )
    return VK_DELETE;
  if ( !strcmp(Str, "HOME") )
    return VK_HOME;
  if ( !strcmp(Str, "INSERT") )
    return VK_INSERT;
  if ( strcmp(Str, "COMMA") )
  {
    if ( !strcmp(Str, "DECIMAL") )
      return VK_DECIMAL;
    if ( strcmp(Str, "SEMICOLON") )
    {
      if ( strcmp(Str, "PERIOD") )
      {
        if ( strcmp(Str, "SLASH") )
        {
          if ( strcmp(Str, "SQUOTE") )
          {
            if ( strcmp(Str, "BACKSLASH") )
            {
              if ( !strcmp(Str, "BACKSPACE") )
                return VK_BACK;
              if ( !strcmp(Str, "CONTROL") )
                return VK_CONTROL;
              if ( strlen(Str) != 1 || (result = *Str, (unsigned __int8)*Str < 0x41u) || result > 0x5Au )
                result = -1;
            }
            else
            {
              result = -36;
            }
          }
          else
          {
            result = -34;
          }
        }
        else
        {
          result = -65;
        }
      }
      else
      {
        result = -66;
      }
    }
    else
    {
      result = -70;
    }
  }
  else
  {
    result = -68;
  }
  return result;
}

//----- (0045AE2C) --------------------------------------------------------
const char *KeyboardActionMapping::GetVKeyDisplayName(signed int a1)
{
  char *v3; // [sp-4h] [bp-8h]@2

  static char static_sub_45AE2C_string_69ADE0[32];

  if ( a1 == VK_UP )
  {
    v3 = "UP";
LABEL_53:
    strcpy(static_sub_45AE2C_string_69ADE0, v3);
    return static_sub_45AE2C_string_69ADE0;
  }
  if ( a1 == VK_DOWN )
  {
    v3 = "DOWN";
    goto LABEL_53;
  }
  if ( a1 == VK_LEFT )
  {
    v3 = "LEFT";
    goto LABEL_53;
  }
  if ( a1 == VK_RIGHT )
  {
    v3 = "RIGHT";
    goto LABEL_53;
  }
  if ( a1 == VK_RETURN )
  {
    v3 = "RETURN";
    goto LABEL_53;
  }
  if ( a1 == VK_SPACE )
  {
    v3 = "SPACE";
    goto LABEL_53;
  }
  if ( a1 == VK_NEXT )
  {
    v3 = "PAGE_DOWN";
    goto LABEL_53;
  }
  if ( a1 == VK_PRIOR )
  {
    v3 = "PAGE_UP";
    goto LABEL_53;
  }
  if ( a1 == VK_TAB )
  {
    v3 = "TAB";
    goto LABEL_53;
  }
  if ( a1 == 'm' )
  {
    v3 = "SUBTRACT";
    goto LABEL_53;
  }
  if ( a1 == 'k' )
  {
    v3 = "ADD";
    goto LABEL_53;
  }
  if ( a1 == VK_END )
  {
    v3 = "END";
    goto LABEL_53;
  }
  if ( a1 == VK_DELETE )
  {
    v3 = "DELETE";
    goto LABEL_53;
  }
  if ( a1 == VK_HOME )
  {
    v3 = "HOME";
    goto LABEL_53;
  }
  if ( a1 == VK_INSERT )
  {
    v3 = "INSERT";
    goto LABEL_53;
  }
  if ( a1 == VK_DECIMAL )
  {
    v3 = "DECIMAL";
    goto LABEL_53;
  }
  if ( a1 == VK_OEM_COMMA )
  {
    v3 = "COMMA";
    goto LABEL_53;
  }
  if ( a1 == VK_OEM_1 )
  {
    v3 = "SEMICOLON";
    goto LABEL_53;
  }
  if ( a1 == VK_OEM_PERIOD )
  {
    v3 = "PERIOD";
    goto LABEL_53;
  }
  if ( a1 == VK_OEM_2 )
  {
    v3 = "SLASH";
    goto LABEL_53;
  }
  if ( a1 == VK_OEM_7 )
  {
    v3 = "SQUOTE";
    goto LABEL_53;
  }
  if ( a1 == VK_OEM_5 )
  {
    v3 = "BACKSLASH";
    goto LABEL_53;
  }
  if ( a1 == VK_CONTROL )
  {
    v3 = "CONTROL";
    goto LABEL_53;
  }
  if ( a1 == VK_BACK )
  {
    v3 = "BACKSPACE";
    goto LABEL_53;
  }
  if ( a1 < 65 || a1 > 90 )
  {
    v3 = "-мер -";
    goto LABEL_53;
  }
  *(unsigned short *)static_sub_45AE2C_string_69ADE0 = (unsigned __int8)a1;
  return static_sub_45AE2C_string_69ADE0;
}

//----- (0045AFD9) --------------------------------------------------------
Keyboard::Keyboard()
{
  bUsingAsynKeyboard = false;

  if (pVersion->pVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT &&
      pVersion->pVersionInfo.dwMajorVersion == 4)
    bUsingAsynKeyboard = false;
}

//----- (0045B019) --------------------------------------------------------
void Keyboard::EnterCriticalSection()
{
}

//----- (0045B06E) --------------------------------------------------------
bool Keyboard::IsShiftHeld()
{
  return GetAsyncKeyState(VK_SHIFT);
}

//----- (0045B0A9) --------------------------------------------------------
bool Keyboard::IsKeyBeingHeld(int vKey)
{
  return GetAsyncKeyState(vKey) & 0x8001;
}

//----- (0045B0CE) --------------------------------------------------------
bool Keyboard::WasKeyPressed(int vKey)
{
  return GetAsyncKeyState(vKey) & 1;
}
//----- (0046A14B) --------------------------------------------------------
void OnPressSpace()
{
  //SHORT v0; // ax@2
  int *v1; // eax@2
  char *v2; // ebx@5
  unsigned int v3; // esi@5
  signed int v4; // edi@7
  unsigned int v5; // edx@7
  int v6; // ecx@8
  int v7; // eax@8
  int v8; // ecx@17
  int *v9; // esi@22
  signed int v10; // ebx@22
  int i; // edi@23
  int v12; // edx@24
  int v13; // ecx@24
  int j; // esi@28
  int v16; // [sp+4h] [bp-1Ch]@0
  char *v17; // [sp+8h] [bp-18h]@5
  unsigned int v18; // [sp+Ch] [bp-14h]@5
  int v19; // [sp+10h] [bp-10h]@8
  int *v20; // [sp+14h] [bp-Ch]@5
  int *v21; // [sp+18h] [bp-8h]@7
  int v22; // [sp+1Ch] [bp-4h]@4

  if ( pRenderer->pRenderD3D )
  {
    pGame->PickKeyboard(GetAsyncKeyState(VK_CONTROL) & 0x8001, &vis_sprite_filter_3, &vis_door_filter);
    auto pid = pGame->pVisInstance->get_picked_object_zbuf_val();
    if ( pid != -1 )
      DoInteractionWithTopmostZObject(pid & 0xFFFF, PID_ID(pid));
    return;
  }

  
  // software render stuff following

  static int dword_720660[100]; // 720660
  static int dword_7207F0[100]; // 7207F0

  v22 = 0;
  v1 = (int *)((signed int)(viewparams->uScreen_BttmR_X + viewparams->uScreen_topL_X) >> 1);//wrong pointer
  if ( (signed int)viewparams->uScreen_topL_Y < (signed int)viewparams->uScreen_BttmR_Y )
  {
	  v2 = (char *)v1 - 50;
	  v1 = (int *)((char *)v1 + 50);
	  v3 = 640 * viewparams->uScreen_topL_Y;
	  v17 = v2;
	  v20 = v1;
	  v18 = ((viewparams->uScreen_BttmR_Y - viewparams->uScreen_topL_Y - 1) >> 1) + 1;
	  do
	  {
		if ( (signed int)v2 < (signed int)v20 )
		{
			v1 = &pRenderer->pActiveZBuffer[(int)&v2[v3]];
			v21 = &pRenderer->pActiveZBuffer[(int)&v2[v3]];
			v4 = v22;
			v5 = (((char *)v20 - v2 - 1) >> 1) + 1;
			do
			{
			  v6 = 0;
			  v7 = *v1 & 0xFFFF;
			  v19 = 0;
			  if ( v4 > 0 )
			  {
				do
				{
				  if ( dword_7207F0[v6] == v7 )
					break;
				  ++v6;
				  v19 = v6;
				}
				while ( v6 < v22 );
			  }
			  if ( PID_TYPE(v7) == OBJECT_Decoration)
			  {
				v16 = (unsigned int)PID_ID(v7);
				if ( (signed int)(((unsigned int)*v21 >> 16)
								- pDecorationList->pDecorations[pLevelDecorations[(unsigned int)PID_ID(v7)].uDecorationDescID].uRadius) <= 512 )
				  if ( v19 == v22 && v4 < 100 )
				  {
					++v22;
					++v4;
					v8 = *v21;
					dword_7207F0[v4 - 1] = v7;
					dword_720660[v4 - 1] = v8;
				  }
			  }
			  else if ( (unsigned int)*v21 <= 0x2000000 )
			  {
				  if ( v19 == v22 && v4 < 100 )
				  {
					++v22;
					++v4;
					v8 = *v21;
					dword_7207F0[v4 - 1] = v7;
					dword_720660[v4 - 1] = v8;
				  }
			  }
			  v1 = v21 + 2;
			  --v5;
			  v21 += 2;
			}
			while ( v5 );
			v2 = v17;
		}
		v3 += 1280;
		--v18;
	  }
	  while ( v18 );
  }
  if ( v22 > 0 )
  {
    v9 = dword_720660;
    v10 = 1;
    do
    {
      for ( i = v10; i < v22; ++i )
      {
        v12 = *v9;
        v13 = dword_720660[i];
        if ( v13 < *v9 )
        {
          *v9 = v13;
          dword_720660[i] = v12;
        }
      }
      ++v10;
      ++v9;
      LOBYTE(v1) = v10 - 1;
    }
    while ( v10 - 1 < v22 );
  }
  for ( j = 0; j < v22; ++j )
  {
    LOBYTE(v1) = DoInteractionWithTopmostZObject(dword_720660[j] & 0xFFFF, v16);
    if ( !(char)v1 )
      break;
  }
}
