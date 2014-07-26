#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define _CRT_SECURE_NO_WARNINGS
#include "OSAPI.h"
#include "OSInfo.h"
#include "Log.h"


bool           OSInfo::initialized = false;
OSVERSIONINFOA OSInfo::info;




//----- (00462C94) --------------------------------------------------------
int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE, wchar_t *lpCmdLine, int nShowCmd)
{
  Log::Initialize();
  
  //if (HWND hMM7Window = FindWindowW(L"MM7", 0))
  if (HWND hMM7Window = FindWindowW(L"M&MTrilogy", 0))//check whether the window is open(проверка открыто ли окно такое же)
  {
    if (IsIconic(hMM7Window))
      ShowWindow(hMM7Window, SW_RESTORE);
    SetForegroundWindow(GetLastActivePopup(hMM7Window));
    return 0;
  }
  //GetActiveWindow() возвращает дескриптор активного окна текущего приложения(котоpое имеет текущий фокус ввода).
  //В случае ошибки,или в случае,если в программе нет активных окон,функция возвращает 0. 
  HWND hPrevWindow = GetActiveWindow();
  if (!hPrevWindow)
  {
    Log::Warning(L"OS init: ok");
    extern bool MM_Main(const wchar_t *pCmdLine);
    MM_Main(lpCmdLine);
  }
  if (hPrevWindow)
    SetActiveWindow(hPrevWindow);

  return GetLastError();
}




void MsgBox(const wchar_t *msg, const wchar_t *title)
{
  MessageBoxW(nullptr, msg, title, 0);
}