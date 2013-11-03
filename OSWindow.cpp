#include "OSWindow.h"
#include "mm7_data.h"
#include "Arcomage.h"
#include "AudioPlayer.h"
#include "VideoPlayer.h"
#include "Mouse.h"
#include "Time.h"
#include "GUIWindow.h"
#include "Party.h"
#include "Game.h"
#include "IndoorCameraD3D.h"
#include "Keyboard.h"
#include "Render.h"
#include "Viewport.h"
#include "Vis.h"
#include "Texts.h"
#include "AIL.h"
#include "Bink_Smacker.h"
#include "ErrorHandling.h"


bool OSWindow::OnMouseLeftClick(int x, int y)
{
  if (UIControl::OnMouseLeftClick(x, y))
    return true;

  if (pVideoPlayer->pVideoFrame.pPixels)
    pVideoPlayer->bStopBeforeSchedule = true;

  pMouse->SetMouseClick(x, y);

  if (GetCurrentMenuID() == MENU_CREATEPARTY)
    UI_OnKeyDown(VK_SELECT);

  if (pGame)
    pGame->PickMouse(512.0, x, y, false, &vis_sprite_filter_3, &vis_door_filter);

  UI_OnMouseLeftClick(0);
  return true;
}

bool OSWindow::OnMouseRightClick(int x, int y)
{
  if (UIControl::OnMouseRightClick(x, y))
    return true;

  if (pVideoPlayer->pVideoFrame.pPixels)
    pVideoPlayer->bStopBeforeSchedule = true;

  pMouse->SetMouseClick(x, y);

  if (pGame)
    pGame->PickMouse(pGame->pIndoorCameraD3D->GetPickDepth(), x, y, 0, &vis_sprite_filter_2, &vis_door_filter);

  UI_OnMouseRightClick(0);
  return true;
}


bool _507B98_ctrl_pressed = false;
bool OSWindow::WinApiMessageProc(UINT msg, WPARAM wparam, LPARAM lparam, LRESULT *result)
{
  switch (msg)
  {
    case WM_KEYUP:
    {
      if (wparam == VK_CONTROL)
        _507B98_ctrl_pressed = false;

      OnKey(wparam);
      return *result = 0, true;
    }

    case WM_SIZING: return *result = 1, true;
    case WM_WINDOWPOSCHANGED:
      if (pVideoPlayer && pVideoPlayer->AnyMovieLoaded() && pVideoPlayer->pBinkBuffer)
        BinkBufferSetOffset(pVideoPlayer->pBinkBuffer, 0, 0);
      return false;

    case WM_CHAR:
      if (!pKeyActionMap->_459F10(wparam) && !viewparams->field_4C)
        GUI_HandleHotkey(wparam);
      return false;

    case WM_DESTROY:
      ExitProcess(GetLastError());
    //  SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
    //  PostQuitMessage(0);
    //  return 0;

    //case WM_DEVICECHANGE:
    //{
    //  if (wParam == 0x8000)          // CD or some device has been inserted - notify InsertCD dialog
    //    PostMessageW(hInsertCDWindow, WM_USER + 1, 0, 0);
    //  return 0;
    //}

    case WM_COMMAND:
      if (OnOSMenu(wparam))
        return *result = 0, true;
    return false;

    case WM_LBUTTONDOWN:
    {
      if (pArcomageGame->bGameInProgress)
      {
        pArcomageGame->stru1.field_0 = 7;
        ArcomageGame::OnMouseClick(0, true);
        return false;
      }

      OnMouseLeftClick(LOWORD(lparam), HIWORD(lparam));
    }
    return false;


    case WM_RBUTTONDOWN:
    {
      if (pArcomageGame->bGameInProgress)
      {
        pArcomageGame->stru1.field_0 = 8;
        ArcomageGame::OnMouseClick(1, true);
        return false;
      }

      OnMouseRightClick(LOWORD(lparam), HIWORD(lparam));
    }
    return false;


    case WM_LBUTTONUP:
      if ( !pArcomageGame->bGameInProgress )
      {
        back_to_game();
        return false;
      }
      pArcomageGame->stru1.field_0 = 3;
      ArcomageGame::OnMouseClick(0, 0);
    return false;

    case WM_RBUTTONUP:
      if ( !pArcomageGame->bGameInProgress )
      {
        back_to_game();
        return false;
      }
      pArcomageGame->stru1.field_0 = 4;
      ArcomageGame::OnMouseClick(1, false);
    return false;

    case WM_LBUTTONDBLCLK:
    {
      if ( pArcomageGame->bGameInProgress )
      {
        pArcomageGame->stru1.field_0 = 7;
        return false;
      }

      OnMouseLeftClick(LOWORD(lparam), HIWORD(lparam));
    }
    return false;

    case WM_RBUTTONDBLCLK:
    {
      if (pArcomageGame->bGameInProgress)
      {
        pArcomageGame->stru1.field_0 = 8;
        return false;
      }

      OnMouseRightClick(LOWORD(lparam), HIWORD(lparam));
    }
    return false;

    /*case WM_MBUTTONDOWN:
      if (pRenderer->pRenderD3D && pGame)
      {
        pGame->PickMouse(pGame->pIndoorCameraD3D->GetPickDepth(), LOWORD(lParam), HIWORD(lParam), 1, &vis_sprite_filter_3, &vis_face_filter);
      }
      return false;*/

    case WM_MOUSEMOVE:
      if ( pArcomageGame->bGameInProgress )
      {
        ArcomageGame::OnMouseMove(LOWORD(lparam), HIWORD(lparam));
        ArcomageGame::OnMouseClick(0, wparam == MK_LBUTTON);
        ArcomageGame::OnMouseClick(1, wparam == MK_RBUTTON);
      }
      else if (pMouse)
        pMouse->SetMouseClick(LOWORD(lparam), HIWORD(lparam));
    return false;

    case WM_SYSCOMMAND:
      if ( wparam == SC_SCREENSAVE || wparam == SC_MONITORPOWER )
        return *result = 0, true;
      return false;

    case WM_KEYDOWN:
      if ( uGameMenuUI_CurentlySelectedKeyIdx != -1 )
      {
        pKeyActionMap->_459F10(wparam);
        return false;
      }
      if ( !pArcomageGame->bGameInProgress )
      {
        if ( pVideoPlayer->pVideoFrame.pPixels )
          pVideoPlayer->bStopBeforeSchedule = 1;
        if ( wparam == VK_RETURN )
        {
          if ( !viewparams->field_4C )
            UI_OnKeyDown(wparam);
          return 0;
        }
        if (wparam == VK_CONTROL)
        {
          _507B98_ctrl_pressed = true;
          return false;
        }
        if ( wparam == VK_ESCAPE )
        {
          pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, window_SpeakInHouse != 0, 0);
          return 0;
        }
        if ( wparam <= VK_HOME )
          return 0;
        if ( wparam > VK_DOWN )
        {
          if ( wparam != VK_F4 || pVideoPlayer->AnyMovieLoaded() )
            return 0;

          // F4 - toggle fullscreen
          SendMessageW(api_handle, WM_COMMAND, 104, 0);
          return 0;
        }
        if ( wparam >= VK_LEFT && wparam <= VK_DOWN )
        {
          if ( pCurrentScreen != SCREEN_GAME && pCurrentScreen != SCREEN_MODAL_WINDOW )
          {
            if ( !viewparams->field_4C )
              UI_OnKeyDown(wparam);
            return 0;
          }
        }
        if ( pCurrentScreen != SCREEN_GAME && pCurrentScreen != SCREEN_MODAL_WINDOW )
          return 0;
      }

      pArcomageGame->stru1.field_0 = 1;

      set_stru1_field_8_InArcomage(MapVirtualKey(wparam, MAPVK_VK_TO_CHAR));
      if ( wparam == 27 )
      {
        pArcomageGame->GameOver = 1;
        pArcomageGame->field_F4 = 1;
        pArcomageGame->uGameResult = 2;
        pArcomageGame->field_B0 = -2;
        return false;
      }
      if ( wparam != 114 )
      {
        if ( wparam == 115 && !pVideoPlayer->AnyMovieLoaded() )
          SendMessage(api_handle, WM_COMMAND, 104, 0);
        return false;
      }
      SendMessageW(api_handle, WM_COMMAND, 103, 0);
      return *result = 0, true;

    case WM_ACTIVATEAPP:
      if ( wparam && (GetForegroundWindow() == api_handle || GetForegroundWindow() == hInsertCDWindow) )
      {
        if ( BYTE1(dword_6BE364_game_settings_1) & 1 )
        {
          dword_4E98BC_bApplicationActive = 1;
          if ( pRenderer->bWindowMode )
          {
            HDC hDC = GetDC(api_handle);
            int bitsPerPixel = GetDeviceCaps(hDC, BITSPIXEL);
            int planes = GetDeviceCaps(hDC, PLANES);
            ReleaseDC(api_handle, hDC);
            if (bitsPerPixel != 16 || planes != 1)
              Error(pGlobalTXT_LocalizationStrings[62]);
          }
          BYTE1(dword_6BE364_game_settings_1) &= 0xFEu;

          if ( pArcomageGame->bGameInProgress )
          {
            pArcomageGame->field_F9 = 1;
          }
          else
          {
            if ( BYTE1(dword_6BE364_game_settings_1) & 2 )
              BYTE1(dword_6BE364_game_settings_1) &= 0xFDu;
            else
              pEventTimer->Resume();
            if ( BYTE1(dword_6BE364_game_settings_1) & 4 )
              BYTE1(dword_6BE364_game_settings_1) &= 0xFBu;
            else
              pMiscTimer->Resume();

            viewparams->bRedrawGameUI = true;
            if ( pVideoPlayer->pSmackerMovie )
            {
              pRenderer->RestoreFrontBuffer();
              pRenderer->_4A184C();
              pVideoPlayer->_4BF5B2();
            }
          }
          if ( pAudioPlayer->hAILRedbook && !bGameoverLoop && !pVideoPlayer->pSmackerMovie )
            AIL_redbook_resume(pAudioPlayer->hAILRedbook);
        }
      }
      else
      {
        if (!(dword_6BE364_game_settings_1 & 0x100))
        {
          dword_4E98BC_bApplicationActive = 0;
          if ( (pVideoPlayer->pSmackerMovie || pVideoPlayer->pBinkMovie) && pVideoPlayer->bPlayingMovie )
            pVideoPlayer->bStopBeforeSchedule = 1;

          if (/*pRenderer->bUserDirect3D && */pRenderer->uAcquiredDirect3DDevice == 1)
            SetWindowPos(api_handle, (HWND)0xFFFFFFFE, 0, 0, 0, 0, 0x18u);
          ClipCursor(0);
          dword_6BE364_game_settings_1 |= 0x100u;
          if ( pEventTimer->bPaused )
            BYTE1(dword_6BE364_game_settings_1) |= 2u;
          else
            pEventTimer->Pause();
          if ( pMiscTimer->bPaused )
            BYTE1(dword_6BE364_game_settings_1) |= 4u;
          else
            pMiscTimer->Pause();

          pAudioPlayer->StopChannels(-1, -1);
          if ( pAudioPlayer->hAILRedbook )
            AIL_redbook_pause(pAudioPlayer->hAILRedbook);
        }
      }
      return *result = 0, true;

    case WM_SETFOCUS:
      dword_4E98BC_bApplicationActive = 0;
      if (pRenderer)
      {
        if (/*pRenderer->bUserDirect3D && */pRenderer->uAcquiredDirect3DDevice == 1)
          SetWindowPos(api_handle, (HWND)0xFFFFFFFE, 0, 0, 0, 0, 0x18u);
      }
      ClipCursor(0);
      return false;

    case WM_KILLFOCUS:
      dword_4E98BC_bApplicationActive = 1;
      return false;

    case WM_PAINT:
      if (pRenderer)
      {
        if ( !GetUpdateRect(api_handle, 0, 0) || !dword_4E98BC_bApplicationActive && !pRenderer->bWindowMode )
          return *result = 0, true;
      }
      if (!api_handle || !pRenderer)
        return false;
      PAINTSTRUCT Paint;
      BeginPaint(api_handle, &Paint);
      if ( pArcomageGame->bGameInProgress )
      {
        pArcomageGame->field_F9 = 1;
      }
      else
      {
        if (/*!pRenderer->pRenderD3D && !pRenderer->UsingDirect3D() ||*/ !AreRenderSurfacesOk())
        {
          EndPaint(api_handle, &Paint);
          return *result = 0, true;
        }
      }
      pRenderer->Present();
      EndPaint(api_handle, &Paint);
      return *result = 0, true;

  }
  return false;
}



bool OSWindow::Initialize(const wchar_t *title, int window_width, int window_height)
{
  WNDCLASSEXW wcxw;
  wcxw.cbClsExtra = 0;
  wcxw.cbSize = sizeof(wcxw);
  wcxw.cbWndExtra = 0;
  wcxw.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);
  wcxw.hCursor = nullptr;
  wcxw.hIcon = wcxw.hIconSm = nullptr;
  wcxw.hInstance = GetModuleHandleW(nullptr);
  wcxw.lpfnWndProc = WinApiMsgRouter;
  wcxw.lpszClassName = L"M&MTrilogy";
  wcxw.lpszMenuName = nullptr;
  wcxw.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
  if (!RegisterClassExW(&wcxw))
    return false;

  api_handle = CreateWindowExW(0, wcxw.lpszClassName, title,
                               0,
                               0, 0, window_width, window_height, nullptr,
                               nullptr, wcxw.hInstance, this);
  if (!api_handle)
  {
    UnregisterClassW(wcxw.lpszClassName, wcxw.hInstance);
    return false;
  }

  HDC hDC = GetDC(NULL);
  int bitsPerPixel = GetDeviceCaps(hDC, BITSPIXEL);
  int planes = GetDeviceCaps(hDC, PLANES);
  ReleaseDC(NULL, hDC);
  if (bitsPerPixel != 16 || planes != 1)
  {
	SetColorDepth(16);
  }

  SetWindowedMode(window_width, window_height);
  Show();

  SetActiveWindow(api_handle);

  MSG msg;
  while (PeekMessageW(&msg, api_handle, 0, 0, PM_REMOVE))
  {
    TranslateMessage(&msg);
    DispatchMessageW(&msg);
  }

  return true;
}




OSWindow *OSWindow::Create(const wchar_t *title, int window_width, int window_height)
{
  OSWindow* window = new OSWindow;
  if (window)
    if (!window->Initialize(title, window_width, window_height))
    {
      delete window;
      window = nullptr;
    }

  return window;
}



LRESULT __stdcall OSWindow::WinApiMsgRouter(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
  if (msg == WM_NCCREATE)
  {
    CREATESTRUCTA* cs = (CREATESTRUCTA *)(lparam);
    OSWindow* window = (OSWindow *)cs->lpCreateParams;

    SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)window);
    return DefWindowProcW(hwnd, msg, wparam, lparam);
  }

  OSWindow* window = (OSWindow *)GetWindowLongPtrW(hwnd, GWLP_USERDATA);
  if (window && window->api_handle == hwnd)
  {
    LPARAM result;
    if (window->WinApiMessageProc(msg, wparam, lparam, &result))
      return result;
  }
  return DefWindowProcW(hwnd, msg, (WPARAM)wparam, (LPARAM)lparam);
}


void OSWindow::Show()
{
  ShowWindow(api_handle, SW_SHOWNORMAL);
  UpdateWindow(api_handle);
}


void OSWindow::SetCursor(const char *cursor_name)
{
  POINT cursor_pos;
  GetCursorPos(&cursor_pos);

  if (!strcmp(cursor_name, "MICON1") )
    SetClassLongPtrW(api_handle, GCLP_HCURSOR, (LONG)LoadCursorW(GetModuleHandleW(nullptr), IDC_ARROW));
  else if (!strcmp(cursor_name, "MICON2") )
    SetClassLongPtrW(api_handle, GCLP_HCURSOR, (LONG)LoadCursorW(GetModuleHandleW(nullptr), L"Target"));
  else if (!strcmp(cursor_name, "MICON3") )
    SetClassLongPtrW(api_handle, GCLP_HCURSOR, (LONG)LoadCursorW(GetModuleHandleW(nullptr), IDC_WAIT));

  //ClientToScreen(api_handle, &cursor_pos);
  SetCursorPos(cursor_pos.x, cursor_pos.y);
}

void OSWindow::SetFullscreenMode()
{
  SetMenu(api_handle, nullptr);
  SetWindowLongW(api_handle, GWL_EXSTYLE, WS_EX_TOPMOST);
  SetWindowLongW(api_handle, GWL_STYLE,   WS_VISIBLE | WS_POPUP);

  SetWindowPos(api_handle, HWND_TOP, 0, 0, -1, -1, SWP_NOSIZE | SWP_SHOWWINDOW);
}

void OSWindow::SetWindowedMode(int new_window_width, int new_window_height)
{
  SetWindowLongW(api_handle, GWL_EXSTYLE, 0);
  SetWindowLongW(api_handle, GWL_STYLE,   WS_VISIBLE | WS_OVERLAPPEDWINDOW);

  RECT rcWindow;
  GetWindowRect(api_handle, &rcWindow);

  RECT rcClient;
  GetClientRect(api_handle, &rcClient);

  int window_borders_width = (rcWindow.right - rcWindow.left) - (rcClient.right - rcClient.left),
      window_borders_height = (rcWindow.bottom - rcWindow.top) - (rcClient.bottom - rcClient.top);
  int window_total_width = new_window_width + window_borders_width,
      window_total_height = new_window_height + window_borders_height;

  #ifdef _DEBUG
    if (!GetMenu(api_handle))
      window_total_height += GetSystemMetrics(SM_CYMENU);
  #endif
  MoveWindow(api_handle, ReadWindowsRegistryInt("window X", (GetSystemMetrics(SM_CXSCREEN) - window_total_width) / 2),
                         ReadWindowsRegistryInt("window Y", (GetSystemMetrics(SM_CYSCREEN) - window_total_height) / 2),
                         window_total_width,
                         window_total_height, 0);
  #ifdef _DEBUG
    static HMENU debug_menu = CreateDebugMenuPanel();
    SetMenu(api_handle, debug_menu);
  #endif
}




HMENU OSWindow::CreateDebugMenuPanel()
{
  HMENU menu = CreateMenu();
  {
    HMENU file = CreatePopupMenu();
    AppendMenuW(menu, MF_ENABLED | MF_STRING | MF_POPUP, (UINT_PTR)file, L"&File");
    {
      AppendMenuW(file, MF_ENABLED | MF_STRING, 40001, L"Exit");
    }

    HMENU debug = CreatePopupMenu();
    AppendMenuW(menu, MF_ENABLED | MF_STRING | MF_POPUP, (UINT_PTR)debug, L"&Debug");
    {
      HMENU debug_party = CreatePopupMenu();
      AppendMenuW(debug, MF_ENABLED | MF_STRING | MF_POPUP, (UINT_PTR)debug_party, L"&Party");
      {
        AppendMenuW(debug_party, MF_ENABLED | MF_STRING, 40007, L"Give Gold (10 000)");
        AppendMenuW(debug_party, MF_ENABLED | MF_STRING, 40008, L"Give Exp (20 000)");
        AppendMenuW(debug_party, MF_ENABLED | MF_STRING, 40059, L"Give Skills (50 each)");
        AppendMenuW(debug_party, MF_ENABLED | MF_STRING, 40013, L"Remove Gold");

        HMENU debug_party_setconditions = CreatePopupMenu();
        AppendMenuW(debug_party, MF_ENABLED | MF_STRING | MF_POPUP, (UINT_PTR)debug_party_setconditions, L"Set Condition");
        {
          AppendMenuW(debug_party_setconditions, MF_ENABLED | MF_STRING, 40044, L"Afraid");
          AppendMenuW(debug_party_setconditions, MF_ENABLED | MF_STRING, 40043, L"Asleep");
          AppendMenuW(debug_party_setconditions, MF_ENABLED | MF_STRING, 40037, L"Curse");
          AppendMenuW(debug_party_setconditions, MF_ENABLED | MF_STRING, 40036, L"Disease1");
          AppendMenuW(debug_party_setconditions, MF_ENABLED | MF_STRING, 40035, L"Disease2");
          AppendMenuW(debug_party_setconditions, MF_ENABLED | MF_STRING, 40034, L"Disease3");
          AppendMenuW(debug_party_setconditions, MF_ENABLED | MF_STRING, 40041, L"Dead");
          AppendMenuW(debug_party_setconditions, MF_ENABLED | MF_STRING, 40039, L"Drunk");
          AppendMenuW(debug_party_setconditions, MF_ENABLED | MF_STRING, 40042, L"Eradicated");
          AppendMenuW(debug_party_setconditions, MF_ENABLED | MF_STRING, 40038, L"Insane");
          AppendMenuW(debug_party_setconditions, MF_ENABLED | MF_STRING, 40045, L"Paralyzed");
          AppendMenuW(debug_party_setconditions, MF_ENABLED | MF_STRING, 40033, L"Poison1");
          AppendMenuW(debug_party_setconditions, MF_ENABLED | MF_STRING, 40032, L"Poison2");
          AppendMenuW(debug_party_setconditions, MF_ENABLED | MF_STRING, 40031, L"Poison3");
          AppendMenuW(debug_party_setconditions, MF_ENABLED | MF_STRING, 40029, L"&Stone");
          AppendMenuW(debug_party_setconditions, MF_ENABLED | MF_STRING, 40040, L"Unconscious");
          AppendMenuW(debug_party_setconditions, MF_ENABLED | MF_STRING, 40030, L"Weak");
          AppendMenuW(debug_party_setconditions, MF_ENABLED | MF_STRING, 40073, L"Zombie");
        }

        AppendMenuW(debug_party, MF_ENABLED | MF_STRING, 40006, L"Set Food (20)");

        HMENU debug_party_alignment = CreatePopupMenu();
        AppendMenuW(debug_party, MF_ENABLED | MF_STRING | MF_POPUP, (UINT_PTR)debug_party_alignment, L"Alignment");
        {
          AppendMenuW(debug_party_alignment, MF_ENABLED | MF_STRING, 40062, L"Good");
          AppendMenuW(debug_party_alignment, MF_ENABLED | MF_STRING | MF_CHECKED, 40063, L"Neutral");
          AppendMenuW(debug_party_alignment, MF_ENABLED | MF_STRING, 40064, L"Evil");
        }
      }

      HMENU debug_time = CreatePopupMenu();
      AppendMenuW(debug, MF_ENABLED | MF_STRING | MF_POPUP, (UINT_PTR)debug_time, L"&Time");
      {
        AppendMenuW(debug_time, MF_ENABLED | MF_STRING, 40009, L"Add 1 Day");
        AppendMenuW(debug_time, MF_ENABLED | MF_STRING, 40010, L"Add 1 Week");
        AppendMenuW(debug_time, MF_ENABLED | MF_STRING, 40011, L"Add 1 Month");
        AppendMenuW(debug_time, MF_ENABLED | MF_STRING, 40012, L"Add 1 Year");
      }

      HMENU debug_items = CreatePopupMenu();
      AppendMenuW(debug, MF_ENABLED | MF_STRING | MF_POPUP, (UINT_PTR)debug_items, L"&Items");
      {
        AppendMenuW(debug_items, MF_ENABLED | MF_STRING, 40015, L"Generate level &1 item");
        AppendMenuW(debug_items, MF_ENABLED | MF_STRING, 40016, L"Generate level &2 item");
        AppendMenuW(debug_items, MF_ENABLED | MF_STRING, 40017, L"Generate level &3 item");
        AppendMenuW(debug_items, MF_ENABLED | MF_STRING, 40018, L"Generate level &4 item");
        AppendMenuW(debug_items, MF_ENABLED | MF_STRING, 40019, L"Generate level &5 item");
        AppendMenuW(debug_items, MF_ENABLED | MF_STRING, 40020, L"Generate level &6 item");
        AppendMenuW(debug_items, MF_ENABLED | MF_STRING, 40061, L"Generate special item");
      }

      HMENU debug_graphics = CreatePopupMenu();
      AppendMenuW(debug, MF_ENABLED | MF_STRING | MF_POPUP, (UINT_PTR)debug_graphics, L"&Graphics");
      {
        AppendMenuW(debug_graphics, MF_ENABLED | MF_STRING, 40023, L"Lighting Mode");
        AppendMenuW(debug_graphics, MF_ENABLED | MF_STRING, 40024, L"Lighting Geometry");
        AppendMenuW(debug_graphics, MF_ENABLED | MF_STRING | MF_GRAYED, 40104, L"Lights Off");
        AppendMenuW(debug_graphics, MF_ENABLED | MF_STRING, 40105, L"Colored Lights");
        AppendMenuW(debug_graphics, MF_ENABLED | MF_STRING, 40025, L"Debug Lights");
        AppendMenuW(debug_graphics, MF_ENABLED | MF_STRING, 40101, L"Debug Decals");
        AppendMenuW(debug_graphics, MF_ENABLED | MF_STRING, 40027, L"HWID Portals");
        AppendMenuW(debug_graphics, MF_ENABLED | MF_STRING, 40047, L"SWID Portals");
        AppendMenuW(debug_graphics, MF_ENABLED | MF_STRING, 40051, L"OD Frustum");
        AppendMenuW(debug_graphics, MF_ENABLED | MF_STRING, 40054, L"SWOD Constant Redraw");
        AppendMenuW(debug_graphics, MF_ENABLED | MF_STRING, 40055, L"SWOD Lit Rasterizer");
        AppendMenuW(debug_graphics, MF_ENABLED | MF_STRING, 40056, L"Party Light off");
        AppendMenuW(debug_graphics, MF_ENABLED | MF_STRING, 40060, L"SWOD Nice Lighting off");
        AppendMenuW(debug_graphics, MF_ENABLED | MF_STRING, 40067, L"HWOD Additive Fog Lights");
        AppendMenuW(debug_graphics, MF_ENABLED | MF_STRING, 40072, L"HWID Nice Lighting");
        AppendMenuW(debug_graphics, MF_ENABLED | MF_STRING, 40048, L"Wireframe");
        AppendMenuW(debug_graphics, MF_ENABLED | MF_STRING, 40049, L"Fog");
      }

      HMENU debug_misc = CreatePopupMenu();
      AppendMenuW(debug, MF_ENABLED | MF_STRING | MF_POPUP, (UINT_PTR)debug_misc, L"&Misc");
      {
        AppendMenuW(debug_misc, MF_ENABLED | MF_STRING, 40066, L"Object Viewcone Culling");
        AppendMenuW(debug_misc, MF_ENABLED | MF_STRING, 40068, L"Red Tint");
        AppendMenuW(debug_misc, MF_ENABLED | MF_STRING, 40071, L"Display Secrets");
        AppendMenuW(debug_misc, MF_ENABLED | MF_STRING, 40102, L"Massive Bloodsplat");
        AppendMenuW(debug_misc, MF_ENABLED | MF_STRING, 40103, L"Underwater Gravity");
      }
      
      HMENU debug_eax = CreatePopupMenu();
      AppendMenuW(debug, MF_ENABLED | MF_STRING | MF_POPUP, (UINT_PTR)debug_eax, L"EAX Environs");
      {
        AppendMenuW(debug_eax, MF_ENABLED | MF_STRING, 40074, L"NONE");
        AppendMenuW(debug_eax, MF_ENABLED | MF_STRING, 40075, L"GENERIC");
        AppendMenuW(debug_eax, MF_ENABLED | MF_STRING, 40076, L"PADDEDCELL");
        AppendMenuW(debug_eax, MF_ENABLED | MF_STRING, 40077, L"ROOM");
        AppendMenuW(debug_eax, MF_ENABLED | MF_STRING, 40078, L"BATHROOM");
        AppendMenuW(debug_eax, MF_ENABLED | MF_STRING, 40079, L"LIVINGROOM");
        AppendMenuW(debug_eax, MF_ENABLED | MF_STRING, 40080, L"STONEROOM");
        AppendMenuW(debug_eax, MF_ENABLED | MF_STRING, 40081, L"AUDITORIUM");
        AppendMenuW(debug_eax, MF_ENABLED | MF_STRING, 40082, L"CONCERTHALL");
        AppendMenuW(debug_eax, MF_ENABLED | MF_STRING, 40083, L"CAVE");
        AppendMenuW(debug_eax, MF_ENABLED | MF_STRING, 40084, L"ARENA");
        AppendMenuW(debug_eax, MF_ENABLED | MF_STRING, 40085, L"HANGAR");
        AppendMenuW(debug_eax, MF_ENABLED | MF_STRING, 40086, L"CARPETEDHALLWAY");
        AppendMenuW(debug_eax, MF_ENABLED | MF_STRING, 40087, L"HALLWAY");
        AppendMenuW(debug_eax, MF_ENABLED | MF_STRING, 40088, L"STONECORRIDOR");
        AppendMenuW(debug_eax, MF_ENABLED | MF_STRING, 40089, L"ALLEY");
        AppendMenuW(debug_eax, MF_ENABLED | MF_STRING, 40090, L"FOREST");
        AppendMenuW(debug_eax, MF_ENABLED | MF_STRING, 40091, L"CITY");
        AppendMenuW(debug_eax, MF_ENABLED | MF_STRING, 40092, L"MOUNTAINS");
        AppendMenuW(debug_eax, MF_ENABLED | MF_STRING, 40093, L"QUARRY");
        AppendMenuW(debug_eax, MF_ENABLED | MF_STRING, 40094, L"PLAIN");
        AppendMenuW(debug_eax, MF_ENABLED | MF_STRING, 40095, L"PARKINGLOT");
        AppendMenuW(debug_eax, MF_ENABLED | MF_STRING, 40096, L"SEWERPIPE");
        AppendMenuW(debug_eax, MF_ENABLED | MF_STRING, 40097, L"UNDERWATER");
        AppendMenuW(debug_eax, MF_ENABLED | MF_STRING, 40098, L"DRUGGED");
        AppendMenuW(debug_eax, MF_ENABLED | MF_STRING, 40099, L"DIZZY");
        AppendMenuW(debug_eax, MF_ENABLED | MF_STRING, 40100, L"PSICHOTIC");
      }
    }
  }
  return menu;
}




bool OSWindow::OnOSMenu(int item_id)
{
  switch (item_id)
  {
    default: return false;


    case 103:  pRenderer->SavePCXScreenshot(); break;
    case 101:  // Quit game
    case 40001:
		pGame->Deinitialize();
      SendMessageW(api_handle, WM_DESTROY, 0, 0);
    break;

    case 104:
      pRenderer->ChangeBetweenWinFullscreenModes();
      if (pArcomageGame->bGameInProgress)
        pArcomageGame->field_F6 = 1;
    break;

        //SubMenu "Party"
    case 40006:  pParty->SetFood(pParty->uNumFoodRations + 20); break;
    case 40007:  pParty->SetGold(pParty->uNumGold + 10000);     break;
    case 40008:  pParty->GivePartyExp(20000);  break;
    case 40013:  pParty->SetGold(0);   break;

    case 40059:
      for (uint i = 0; i < 4; ++i)
        pParty->pPlayers[i].uSkillPoints = 50;
    break;
        
    case 40029:  pPlayers[uActiveCharacter]->SetPertified(true);  break;
    case 40030:  pPlayers[uActiveCharacter]->SetWeak(true);       break;
    case 40031:  pPlayers[uActiveCharacter]->SetPoison3(true);    break;
    case 40032:  pPlayers[uActiveCharacter]->SetPoison2(true);    break;
    case 40033:  pPlayers[uActiveCharacter]->SetPoison1(true);    break;
    case 40034:  pPlayers[uActiveCharacter]->SetDisease3(true);   break;
    case 40035:  pPlayers[uActiveCharacter]->SetDisease2(true);   break;
    case 40036:  pPlayers[uActiveCharacter]->SetDisease1(true);   break;
    case 40037:  pPlayers[uActiveCharacter]->SetCursed(true);     break;
    case 40038:  pPlayers[uActiveCharacter]->SetInsane(true);     break;
    case 40039:  pPlayers[uActiveCharacter]->SetDrunk(true);      break;
    case 40040:  pPlayers[uActiveCharacter]->SetUnconcious(true); break;
    case 40041:  pPlayers[uActiveCharacter]->SetDead(true);       break;
    case 40042:  pPlayers[uActiveCharacter]->SetEradicated(true); break;
    case 40043:  pPlayers[uActiveCharacter]->SetAsleep(true);     break;
    case 40044:  pPlayers[uActiveCharacter]->SetAfraid(true);     break;
    case 40045:  pPlayers[uActiveCharacter]->SetParalyzed(true);  break;
    case 40073:  pPlayers[uActiveCharacter]->SetZombie(true);     break;

    case 40062:
      pParty->alignment = PartyAlignment_Good;
      SetUserInterface(pParty->alignment, true);
    break;
    case 40063:
      pParty->alignment = PartyAlignment_Neutral;
      SetUserInterface(pParty->alignment, true);
    break;
    case 40064:
      pParty->alignment = PartyAlignment_Evil;
      SetUserInterface(pParty->alignment, true);
    break;



        //SubMenu "Time"
    case 40009:  pParty->uTimePlayed += Timer::Day;   break;
    case 40010:  pParty->uTimePlayed += Timer::Week;  break;
    case 40011:  pParty->uTimePlayed += Timer::Month; break;
    case 40012:  pParty->uTimePlayed += Timer::Year;  break;

        //SubMenu "Items"
    case 40015: // uItemID_Rep_St == 1
    case 40016: // uItemID_Rep_St == 2
    case 40017: // uItemID_Rep_St == 3
    case 40018: // uItemID_Rep_St == 4
    case 40019: // uItemID_Rep_St == 5
    case 40020: // uItemID_Rep_St == 6
    {
      int pItemID = rand() % 500;
      for (uint i = 0; i < 500; ++i)
      {
        if (pItemID + i > 499)
          pItemID = 0;
        if (pItemsTable->pItems[pItemID + i].uItemID_Rep_St == (item_id - 40015 + 1))
        {
          pPlayers[uActiveCharacter]->AddItem(-1, pItemID + i);
          break;
        }
      }
    }
    break;

    case 40061:
    {
      int pItemID = rand() % 500;
      for (uint i = 0; i < 500; ++i)
      {
        if (pItemID + i > 499)
          pItemID = 0;
        if (pItemsTable->pItems[pItemID + i].uItemID_Rep_St > 6)
        {
          pPlayers[uActiveCharacter]->AddItem(-1, pItemID + i);
          break;
        }
      }
    }
    break;
  }

  return true;
}

bool OSWindow::SetColorDepth(int bit)
{
		dm.dmSize = sizeof(DEVMODE);
		if (!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm))
		{
			printf("EnumDisplaySettings failed:%d\n", GetLastError());
			return false;
		}
 		dm.dmBitsPerPel = bit;
		dm.dmFields = DM_BITSPERPEL;
		if (ChangeDisplaySettings(&dm, CDS_TEST) !=DISP_CHANGE_SUCCESSFUL)
		{
			printf("\nIllegal graphics mode: %d\n", GetLastError());
			return false;
		}
		if (ChangeDisplaySettings(&dm, 0) == DISP_CHANGE_SUCCESSFUL)
		{
			ChangedColorDepth = true;
		}
}

void OSWindow::Delete()
{
	Deinitialize();
}

void OSWindow::Deinitialize()
{
	if( ChangedColorDepth )
	{
		SetColorDepth(32);
	}
}