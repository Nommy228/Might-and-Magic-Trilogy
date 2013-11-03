#include "OSAPI.h"
#include "OSInfo.h"
#include "Log.h"


OSVersion *pVersion = nullptr;




//----- (00462C94) --------------------------------------------------------
int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE, wchar_t *lpCmdLine, int nShowCmd)
{
  Log::Initialize();
  
  //if (HWND hMM7Window = FindWindowW(L"MM7", 0))
  if (HWND hMM7Window = FindWindowW(L"M&MTrilogy", 0))
  {
    if (IsIconic(hMM7Window))
      ShowWindow(hMM7Window, SW_RESTORE);
    SetForegroundWindow(GetLastActivePopup(hMM7Window));
    return 0;
  }
  HWND hPrevWindow = GetActiveWindow();

  pVersion = new OSVersion;
  {
    Log::Warning(L"OS init: ok");

    extern bool MM_Main(const wchar_t *pCmdLine);
    MM_Main(lpCmdLine);
  }
  delete pVersion;
  pVersion = nullptr;

  if (hPrevWindow)
    SetActiveWindow(hPrevWindow);

  //__debugbreak();
  return 0;
}




void MsgBox(const wchar_t *msg, const wchar_t *title)
{
  MessageBoxW(nullptr, msg, title, 0);
}