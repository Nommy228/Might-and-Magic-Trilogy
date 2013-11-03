#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif


#include "Bink_Smacker.h"

#include "mm7_data.h"
#include "CShow.h"
#include "Mouse.h"

#include "OSInfo.h"
#include "VideoPlayer.h"
#include "AudioPlayer.h"
#include "Game.h"
#include "Render.h"
#include "Party.h"
#include "GUIWindow.h"
#include "Time.h"
#include "Log.h"
#include "texts.h"
#include "UI\UIHouses.h"


VideoPlayer *pVideoPlayer = nullptr;







#pragma comment(lib, "Version.lib")
bool GetDllVersion(const wchar_t *pDllName, uint *pMajor, uint *pMinor)
{
  uint uVersionSize = GetFileVersionInfoSizeW(pDllName, nullptr);
  void *pVersionData = HeapAlloc(GetProcessHeap(), 0, uVersionSize);
  {
    GetFileVersionInfoW(pDllName, 0, uVersionSize, pVersionData);

    VS_FIXEDFILEINFO *pInfo = nullptr;
    UINT              uInfoSize = 0;
    VerQueryValueW(pVersionData, L"\\", (void **)&pInfo, &uInfoSize);

    if (!pMajor || !pMinor)
    {
      HeapFree(GetProcessHeap(), 0, pVersionData);
      return false;
    }
    *pMajor = pInfo->dwFileVersionMS;
    *pMinor = pInfo->dwFileVersionLS;
  }
  HeapFree(GetProcessHeap(), 0, pVersionData);
  return true;
}


// 3.15.1.0:
//      3  15   1   0
// 0x0003000F00010000
unsigned __int64 uBinkVersion;





bool BinkLockBuffer(struct _BINKBUF *_this);
void BinkUnlockBuffer(struct _BINKBUF *_this);

//----- (004BFE2D) --------------------------------------------------------
_BINKBUF *VideoPlayer::CreateBinkBuffer(unsigned int uWidth, unsigned int uHeight, char a4)
{
  __int32 v4; // edi@3
  _BINKBUF *v5; // esi@3
  HRESULT v6; // eax@5
  IDirectDrawSurface *v7; // eax@6
  HRESULT v8; // eax@9
  char v9; // al@11
  DDSURFACEDESC2 v11; // [sp+Ch] [bp-108h]@7
  DDSURFACEDESC Dst; // [sp+88h] [bp-8Ch]@3
  unsigned int v13; // [sp+F4h] [bp-20h]@1
  //struct tagRECT Rect; // [sp+F8h] [bp-1Ch]@11
  IDirectDrawSurface4 *v15; // [sp+108h] [bp-Ch]@7
  IDirectDrawSurface2 *a2; // [sp+10Ch] [bp-8h]@3

  v13 = uWidth;
  if (pVersion->pVersionInfo.dwPlatformId != VER_PLATFORM_WIN32_NT ||
      pVersion->pVersionInfo.dwMajorVersion >= 5u )
  {
    v4 = 0;
    v15 = 0;

    if (uBinkVersion == 0x0001000500150000)
    {
      v5 = new _BINKBUF_1_5_21_0;
      memset(v5, 0, sizeof(_BINKBUF_1_5_21_0));
    }
    else if (uBinkVersion == 0x0003000000000000)
    {
      v5 = new _BINKBUF_3_0_0_0;
      memset(v5, 0, sizeof(_BINKBUF_3_0_0_0));
    }
    else __debugbreak();

    memset(&v11, 0, 0x7Cu);
    v11.dwSize = 124;
    v11.dwWidth = v13;
    v11.dwFlags = 0x1007u;                      // DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT
    v11.ddsCaps.dwCaps = 0x4040u;               // DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY
    v11.dwHeight = uHeight;
    *(_QWORD *)&v11.ddpfPixelFormat.dwSize = 0x400000020ui64;// DDPF_FOURCC
    v11.ddpfPixelFormat.dwFourCC = 0x32595559u;
    if ( dword_6BE384_2dacceloff || pRenderer->pDirectDraw4->CreateSurface(&v11, &v15, 0) )
    {
      memset(&v11.ddpfPixelFormat, 0, sizeof(DDPIXELFORMAT));

      v11.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
      v11.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;

      long hr = pRenderer->pDirectDraw4->CreateSurface(&v11, &v15, 0);
      ErrD3D(hr);
    }
    v5->uWidth = v11.dwWidth;
    v5->uHeight = v11.dwHeight;
    v5->uBinkDDSurfaceType = BinkDDSurfaceType((IDirectDrawSurface *)v15);
    v5->field_38 = 0;
    v7 = (IDirectDrawSurface *)v15;
  }
  else
  {
    v4 = 0;
    a2 = 0;
    

    if (uBinkVersion == 0x0001000500150000)
    {
      v5 = new _BINKBUF_1_5_21_0;
      memset(v5, 0, sizeof(_BINKBUF_1_5_21_0));
    }
    else if (uBinkVersion == 0x0003000000000000)
    {
      v5 = new _BINKBUF_3_0_0_0;
      memset(v5, 0, sizeof(_BINKBUF_3_0_0_0));
    }
    else __debugbreak();

    memset(&Dst, 0, 0x6Cu);
    Dst.dwSize = 108;
    Dst.dwWidth = v13;
    Dst.dwFlags = 4103;
    Dst.ddsCaps.dwCaps = 16448;
    Dst.dwHeight = uHeight;
    Dst.ddpfPixelFormat.dwSize = 32;
    Dst.ddpfPixelFormat.dwFlags = 4;
    Dst.ddpfPixelFormat.dwFourCC = 844715353;
    if ( dword_6BE384_2dacceloff
      || pRenderer->pDirectDraw2->CreateSurface(&Dst, (LPDIRECTDRAWSURFACE *)&a2, 0) )
    {
      memset(&Dst.ddpfPixelFormat, 0, sizeof(DDPIXELFORMAT));

      Dst.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
      Dst.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;

      long hr = pRenderer->pDirectDraw2->CreateSurface(&Dst, (LPDIRECTDRAWSURFACE *)&a2, 0);
      ErrD3D(hr);
    }
    v5->uWidth = Dst.dwWidth;
    v5->uHeight = Dst.dwHeight;
    v5->uBinkDDSurfaceType = BinkDDSurfaceType((IDirectDrawSurface *)a2);
    v5->field_38 = 0;
    v7 = (IDirectDrawSurface *)a2;
  }
  v5->pTargetDDrawSurface = v7;
  v5->hWnd = window->GetApiHandle();
  v9 = a4;
  v5->field_4C = v4;
  v5->field_68 = v4;
  v5->field_5C = v4;
  v5->field_74 = v9 & 0x1F;
  v5->field_78 = 1;
  v5->target_width = GetSystemMetrics(SM_CXSCREEN);
  v5->target_height = GetSystemMetrics(SM_CYSCREEN);
  v5->field_2C = 16;

  RECT Rect;
  GetWindowRect(window->GetApiHandle(), &Rect);
  v5->field_8 = window->GetWidth();
  v5->field_C = window->GetHeight();
  v5->field_1C = Rect.left;
  v5->field_20 = Rect.top;
  Rect.left = v4;
  Rect.top = v4;
  ClientToScreen(window->GetApiHandle(), (POINT *)&Rect);
  v5->field_1C = Rect.left - v5->field_1C;
  v5->field_20 = Rect.top - v5->field_20;
  GetClientRect(window->GetApiHandle(), &Rect);
  v5->field_30 = v5->field_8 - Rect.right;
  v5->field_34 = v5->field_C - Rect.bottom;
  v5->field_8 = v5->uWidth + v5->field_30;
  v5->field_C = v5->uHeight + v5->field_34;
  BinkBufferSetOffset(v5, v4, v4);
  BinkBufferSetScale(v5, v5->uWidth, v5->uHeight);
  return v5;
}


//----- (004C0133) --------------------------------------------------------
bool BinkLockBuffer(_BINKBUF *_this)
{
  _BINKBUF *v1; // esi@1
  IDirectDrawSurface *v2; // edi@5
  DWORD v4; // eax@8
  DWORD v5; // eax@8
  IDirectDrawSurface4 *v6; // edi@11
  LPVOID v7; // eax@14

  v1 = _this;
  if ( pVersion->pVersionInfo.dwPlatformId != VER_PLATFORM_WIN32_NT || pVersion->pVersionInfo.dwMajorVersion >= 5u )
  {
  DDSURFACEDESC2 v8; // [sp+Ch] [bp-7Ch]@4
    if ( _this->pTargetDDrawSurface )
    {
      memset(&v8, 0, 0x7Cu);
      v8.dwSize = 124;
      while ( 1 )
      {
        v6 = (IDirectDrawSurface4 *)v1->pTargetDDrawSurface;
        if ( !v6->Lock(0, &v8, 1u, 0) )
          break;
        
        if (uBinkVersion < 0x0003000000000000)
          BYTE3(v1->uBinkDDSurfaceType) |= 0x80u;
        else
          BYTE3(v1->uBinkDDSurfaceType) |= 0x04;

        if ( v6->Restore() )
          return 0;
      }
      v7 = v8.lpSurface;
      v1->pDDrawSurfaceData_ = v8.lpSurface;
      v1->pDDrawSurfaceData = v7;
      v5 = v8.lPitch;
      goto LABEL_15;
    }
  }
  else
  {
  DDSURFACEDESC v8; // [sp+Ch] [bp-7Ch]@4
    if ( _this->pTargetDDrawSurface )
    {
      memset(&v8.lPitch, 0, 0x6Cu);
      v8.lPitch = 108;
      while ( 1 )
      {
        v2 = v1->pTargetDDrawSurface;
        if ( !v2->Lock(0, (LPDDSURFACEDESC)&v8.lPitch, 1u, 0) )
          break;
        
        if (uBinkVersion < 0x0003000000000000)
          __asm int 3;
        else
          BYTE3(v1->uBinkDDSurfaceType) |= 4u;

        if ( v2->Restore() )
          return 0;
      }
      v4 = (DWORD)v8.lpSurface;
      v1->pDDrawSurfaceData_ = (void *)v8.lpSurface;
      v1->pDDrawSurfaceData = (void *)v4;
      v5 = v8.dwReserved;
LABEL_15:
      v1->uDDrawSurfacePitch = v5;
      return 1;
    }
  }
  return 1;
}

//----- (004C01FB) --------------------------------------------------------
void BinkUnlockBuffer(_BINKBUF *_this)
{
  _BINKBUF *v1; // esi@1
  IDirectDrawSurface *v2; // eax@1

  v1 = _this;
  v2 = _this->pTargetDDrawSurface;
  if ( v2 )
  {
    v2->Unlock(0);
    v1->uDDrawSurfacePitch = 0;
    v1->pDDrawSurfaceData = 0;

    if (uBinkVersion < 0x0003000000000000)
      BYTE3(_this->uBinkDDSurfaceType) &= 0x7F;
    else
      BYTE3(_this->uBinkDDSurfaceType) &= 0xFB;
  }
}
//----- (004BF794) --------------------------------------------------------
void  ShowIntroVideo_and_LoadingScreen()
{
  RGBTexture tex; // [sp+Ch] [bp-30h]@1
  unsigned int uTrackStartMS; // [sp+34h] [bp-8h]@8
  unsigned int uTrackEndMS; // [sp+38h] [bp-4h]@8

  pVideoPlayer->bStopBeforeSchedule = false;
  pVideoPlayer->pResetflag = 0;
  bGameoverLoop = 1;
  if (!bNoVideo)
  {
    pRenderer->PresentBlackScreen();
    PlayFullscreenMovie(MOVIE_3DOLogo, true);
    //pGame->pCShow->PlayMovie(MOVIE_3DOLogo, 1);
    if ( !pVideoPlayer->bStopBeforeSchedule )
    {
      PlayFullscreenMovie(MOVIE_NWCLogo, true);
      //pGame->pCShow->PlayMovie(MOVIE_NWCLogo, 1);
      if ( !pVideoPlayer->bStopBeforeSchedule )
      {
        PlayFullscreenMovie(MOVIE_JVC, true);
        //pGame->pCShow->PlayMovie(MOVIE_JVC, 1);
        if ( !pVideoPlayer->bStopBeforeSchedule )
          PlayFullscreenMovie(MOVIE_Intro, true);
          //pGame->pCShow->PlayMovie(MOVIE_Intro, 1);
      }
    }
  }
  tex.Load("mm6title.pcx", 2);
  pRenderer->BeginScene();
  pRenderer->DrawTextureRGB(0, 0, &tex);
  free(tex.pPixels);
  tex.pPixels = 0;

  LoadFonts_and_DrawCopyrightWindow();

  pRenderer->EndScene();
  pRenderer->Present();

  #ifndef _DEBUG
    Sleep(1500);   // let the copyright window stay for a while
  #endif

  if (!bNoSound && pAudioPlayer->hAILRedbook )
  {
    pAudioPlayer->SetMusicVolume((signed __int64)(pSoundVolumeLevels[uMusicVolimeMultiplier] * 64.0));
    AIL_redbook_stop(pAudioPlayer->hAILRedbook);
    AIL_redbook_track_info(pAudioPlayer->hAILRedbook, 0xE, &uTrackStartMS, &uTrackEndMS);
    AIL_redbook_play(pAudioPlayer->hAILRedbook, uTrackStartMS + 1, uTrackEndMS);
  }
  bGameoverLoop = 0;
}




//----- (004BE70E) --------------------------------------------------------
void VideoPlayer::MovieLoop(const char *pMovieName, int a2, int ScreenSizeFlag, int a4)
{
  int v4; // ebp@1
  const char *pName; // edi@1
  MSG Msg; // [sp+Ch] [bp-1Ch]@12

  v4 = a2;
  pName = pMovieName;
  if ( !(dword_6BE364_game_settings_1 & 0x44) )
  {
    if ( a2 == 2 )
      v4 = 0;
    ShowCursor(0);
    pVideoPlayer->OpenGlobalMovie(pName, 0, ScreenSizeFlag);
    pVideoPlayer->bPlayingMovie = 1;
    pVideoPlayer->field_44 = v4;
    if ( pRenderer->pRenderD3D )
    {
      pRenderer->ClearTarget(0);
    }
    else
    {
      pRenderer->BeginScene();
      pRenderer->ClearTarget(0);
      pRenderer->EndScene();
    }
    pCurrentScreen = SCREEN_VIDEO;
    if ( pVideoPlayer->uMovieFormat == 2 )
    {
      if ( pVideoPlayer->pBinkMovie )
      {
        pVideoPlayer->BinkDrawFrame(v4, ScreenSizeFlag);
        while ( pVideoPlayer->pBinkMovie )
        {
          if ( pVideoPlayer->bStopBeforeSchedule )
            break;
          while ( PeekMessageA(&Msg, 0, 0, 0, 1) )
          {
            if ( Msg.message == 18 )
              Game_DeinitializeAndTerminate(0);
            if ( Msg.message == 15 )
              break;
            TranslateMessage(&Msg);
            DispatchMessageA(&Msg);
          }
          GUI_MainMenuMessageProc();
          if ( !pVideoPlayer->pBinkMovie )
            break;
          if ( !BinkWait(pVideoPlayer->pBinkMovie) && !pVideoPlayer->bStopBeforeSchedule )
            pVideoPlayer->BinkDrawFrame(v4, ScreenSizeFlag);
        }
      }
      if ( pVideoPlayer->bStopBeforeSchedule == 1 )
        Sleep(0x3E8u);
    }
    else
    {
      if ( pVideoPlayer->uMovieFormat == 1 )
      {
        if ( pVideoPlayer->pSmackerMovie )
        {
          pVideoPlayer->SmackDrawFrame(v4, ScreenSizeFlag);
          while ( pVideoPlayer->pSmackerMovie )
          {
            if ( pVideoPlayer->bStopBeforeSchedule )
              break;
            while ( PeekMessageW(&Msg, 0, 0, 0, 1) )
            {
              if (Msg.message == WM_QUIT)
                Game_DeinitializeAndTerminate(0);
              if (Msg.message == WM_PAINT)
                break;
              TranslateMessage(&Msg);
              DispatchMessageW(&Msg);
            }
            GUI_MainMenuMessageProc();
            if ( !pVideoPlayer->pSmackerMovie )
              break;
            if ( !SmackWait(pVideoPlayer->pSmackerMovie) && !pVideoPlayer->bStopBeforeSchedule )
              pVideoPlayer->SmackDrawFrame(v4, ScreenSizeFlag);
          }
        }
      }
    }
    if (a4 == 1)
      pCurrentScreen = SCREEN_GAME;
    pVideoPlayer->bPlayingMovie = 0;
    ShowCursor(1);
    if ( pCurrentScreen == SCREEN_VIDEO )
      pCurrentScreen = SCREEN_GAME;
  }
}




//----- (004BE95A) --------------------------------------------------------
unsigned int VideoPlayer::SmackCheckSurfaceFromat()
{
  DDPIXELFORMAT a2; // [sp+0h] [bp-20h]@1

  a2.dwSize = 32;
  a2.dwFlags = 64;
  pRenderer->GetTargetPixelFormat(&a2);
  if ( a2.dwRGBBitCount != 8 )
  {
    if ( a2.dwRBitMask == 0xF800 )
    {
      if ( a2.dwGBitMask == 0x7E0 && a2.dwBBitMask == 0x1F )
        return SMACKBUFFER565;
    }
    else
    {
      if ( a2.dwRBitMask == 0x7C00
        && a2.dwGBitMask == 0x3E0
        && a2.dwBBitMask == 0x1F )
        return SMACKBUFFER555;
    }
    MessageBoxA(0, "Unsupported pixel format.", "Smacker Error", 0);
  }
  return 0;
}

//----- (004BE9D8) --------------------------------------------------------
void VideoPlayer::Initialize(OSWindow *target_window)
{
  DWORD NumberOfBytesRead; // [sp+10h] [bp-4h]@9
    
  window = target_window;

  uint uBinkVersionMajor = -1,
       uBinkVersionMinor = -1;
  GetDllVersion(L"BINKW32.DLL", &uBinkVersionMajor, &uBinkVersionMinor);
  uBinkVersion = (unsigned __int64)uBinkVersionMajor << 32 | uBinkVersionMinor;

  strcpy(pTmpBuf.data(), "anims\\might7.vid");
  hMightVid = CreateFileW(L"anims\\might7.vid", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0x8000080u, 0);
  if ( hMightVid == INVALID_HANDLE_VALUE )
  {
    sprintf(pTmpBuf2.data(), "Can't open file - anims\\%s.smk", pTmpBuf.data());
    MessageBoxA(0, pTmpBuf2.data(), "Video File Error", 0);
    return;
  }
  strcpy(pTmpBuf.data(), "anims\\magic7.vid");
  hMagicVid = CreateFileW(L"anims\\magic7.vid", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0x8000080u, 0);
  if ( hMagicVid == INVALID_HANDLE_VALUE )
  {
    if ( !bCanLoadFromCD )
      {
       sprintf(pTmpBuf2.data(), "Can't open file - anims\\%s.smk", pTmpBuf.data());
       MessageBoxA(0, pTmpBuf2.data(), "Video File Error", 0);
       return;
	  }
    sprintf(pTmpBuf2.data(), "%c:\\%s", (unsigned __int8)cMM7GameCDDriveLetter, pTmpBuf.data());
    hMagicVid = CreateFileA(pTmpBuf2.data(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0x8000080u, 0);
    if ( hMagicVid == (HANDLE)INVALID_HANDLE_VALUE )
    {
	  sprintf(pTmpBuf2.data(), "Can't open file - %s", pTmpBuf.data());
      MessageBoxA(0, pTmpBuf2.data(), "Video File Error", 0);
      return;
    }
  }
  ReadFile(hMightVid, &uNumMightVideoHeaders, 4, &NumberOfBytesRead, 0);
  ReadFile(hMagicVid, &uNumMagicVideoHeaders, 4, &NumberOfBytesRead, 0);
  pMagicVideoHeaders = 0;
  pMightVideoHeaders = 0;
  pMightVideoHeaders = (MovieHeader *)malloc(sizeof(MovieHeader) * uNumMightVideoHeaders + 2);
  pMagicVideoHeaders = (MovieHeader *)malloc(sizeof(MovieHeader) * uNumMagicVideoHeaders + 2);
  ReadFile(hMightVid, pMightVideoHeaders, 44 * uNumMightVideoHeaders, &NumberOfBytesRead, 0);
  ReadFile(hMagicVid, pMagicVideoHeaders, 44 * uNumMagicVideoHeaders, &NumberOfBytesRead, 0);
}

//----- (004BEB41) --------------------------------------------------------
void VideoPlayer::Prepare()
{
  pEventTimer->Pause();
  if ( pAudioPlayer->hAILRedbook )
    AIL_redbook_pause(pAudioPlayer->hAILRedbook);

  bStopBeforeSchedule = false;
  field_54 = 1;
  pSmackerMovie = 0;
  pSmackerBuffer = 0;
  pBinkMovie = 0;
  pBinkBuffer = 0;
  bPlayingMovie = false;
  bFirstFrame = false;
  bUsingSmackerMMX = SmackUseMMX(1);
  BinkSetSoundSystem(BinkOpenMiles, pAudioPlayer->hDigDriver);
  if (pVersion->pVersionInfo.dwPlatformId != VER_PLATFORM_WIN32_NT || pVersion->pVersionInfo.dwMajorVersion >= 5)
    uBinkDirectDrawSurfaceType = BinkDDSurfaceType((IDirectDrawSurface *)pRenderer->pBackBuffer4);
  else
    uBinkDirectDrawSurfaceType = BinkDDSurfaceType((IDirectDrawSurface *)pRenderer->pBackBuffer2);
}


//----- (004BEBD7) --------------------------------------------------------
void VideoPlayer::Unload()
{
  //_BINK *pBinkMovie; // eax@1
  //_BINKBUF *pBinkBuffer; // eax@3
  //_SMACK *pSmackerMovie; // eax@5

  if ( pBinkMovie )
  {
    BinkPause(pBinkMovie, 1);
    Sleep(300);
    BinkClose(pBinkMovie);
    pBinkMovie = 0;
  }

  if ( pBinkBuffer )
  {
    pBinkBuffer->pTargetDDrawSurface->Release();
    pBinkBuffer->pTargetDDrawSurface = 0;
    free(pBinkBuffer);
    pBinkBuffer = 0;
  }

  if ( pSmackerMovie )
  {
    SmackSoundOnOff(pSmackerMovie, 0);
    SmackClose(pSmackerMovie);
    pSmackerMovie = 0;
  }
  if ( pSmackerBuffer )
  {
    SmackBufferClose(pSmackerBuffer);
    pSmackerBuffer = 0;
  }
  if ( pSmackMovieBlit )
  {
    SmackBlitClose(pSmackMovieBlit);
    pSmackMovieBlit = 0;
  }
  field_54 = 0;
  uMovieFormat = 0;
  dword_0000A0 = 0;
  memset(pCurrentMovieName, 0, 0x40);
  if ( pAudioPlayer->hAILRedbook && !bGameoverLoop )
    AIL_redbook_resume(pAudioPlayer->hAILRedbook);
  pEventTimer->Resume();
}

//----- (004BECD5) --------------------------------------------------------
void VideoPlayer::FastForwardToFrame(unsigned int uDstFrameNum)
{
  VideoPlayer *v2; // esi@1
  unsigned int v3; // eax@1

  v2 = this;
  v3 = this->uMovieFormat;
  if ( v3 == 2 )
  {
    if (uDstFrameNum)
    {
      int *pCurrentFrame = uBinkVersion == 0x0001000500150000 ? &((_BINK_1_5_21_0 *)pBinkMovie)->uCurrentFrame :
                           uBinkVersion == 0x0003000000000000 ? &((_BINK_3_0_0_0  *)pBinkMovie)->uCurrentFrame :
                           nullptr;
      while (*pCurrentFrame < uDstFrameNum)
      {
        BinkDoFrame(pBinkMovie);
        BinkNextFrame(pBinkMovie);
      }
    }
  }
  else
  {
    if ( v3 != 1 )
      return;
    SmackSoundOnOff(this->pSmackerMovie, 0);
    if (uDstFrameNum)
    {
      while ( v2->pSmackerMovie->FrameNum < uDstFrameNum)
      {
        SmackDoFrame(v2->pSmackerMovie);
        SmackNextFrame(v2->pSmackerMovie);
      }
    }
  }
  Unload();
}


//----- (004BED4F) --------------------------------------------------------
void VideoPlayer::BinkDrawFrame(int a3, int a4)
{
  //VideoPlayer *v4; // esi@1
  _BINKBUF *v5; // eax@5
  LONG v6; // edx@6
  int v7; // ecx@6
  LONG v8; // edx@6
  struct tagRECT a3a; // [sp+8h] [bp-20h]@10
  struct tagRECT a1; // [sp+18h] [bp-10h]@6

  //v4 = this;
  if ( this->pBinkMovie && this->pBinkBuffer )
  {
    BinkDoFrame(pBinkMovie);
    BinkGetRects(pBinkMovie, pBinkBuffer->uBinkDDSurfaceType);
    if ( BinkLockBuffer(pBinkBuffer) )
    {
      BinkCopyToBuffer(pBinkMovie, pBinkBuffer->pDDrawSurfaceData, pBinkBuffer->uDDrawSurfacePitch, pBinkBuffer->uHeight,
        0, 0, pBinkBuffer->uBinkDDSurfaceType);
      BinkUnlockBuffer(pBinkBuffer);
    }
    v5 = pBinkBuffer;
    if ( pRenderer->bWindowMode )
    {
      v6 = v5->uRectX;
      a1.left = v6;
      a1.top = v5->uRectY;
      v7 = v6 + v5->uWidth;
      a1.right = v6 + v5->uWidth;
      v8 = a1.top + v5->uHeight;
    }
    else
    {
      a1.left = 0;
      a1.top = 0;
      v7 = v5->uWidth;
      a1.right = v5->uWidth;
      v8 = v5->uHeight;
    }
    a1.bottom = v8;
    if ( a4 )
    {
      a1.right = v5->uWidth + v7;
      a1.bottom = v5->uHeight + v8;
    }
    a3a.left = 0;
    a3a.top = 0;
    a3a.right = v5->uWidth;
    a3a.bottom = v5->uHeight;
    pRenderer->BltToFront(&a1, v5->pTargetDDrawSurface, &a3a, 0x1000000u);
    
    
    int *pCurrentFrame = uBinkVersion == 0x0001000500150000 ? &((_BINK_1_5_21_0 *)pBinkMovie)->uCurrentFrame :
                         uBinkVersion == 0x0003000000000000 ? &((_BINK_3_0_0_0  *)pBinkMovie)->uCurrentFrame:
                         nullptr;
    int *pNumFrames = uBinkVersion == 0x0001000500150000 ? &((_BINK_1_5_21_0 *)pBinkMovie)->uNumFrames :
                      uBinkVersion == 0x0003000000000000 ? &((_BINK_3_0_0_0  *)pBinkMovie)->uNumFrames :
                      nullptr;

    if (*pCurrentFrame != *pNumFrames - 1 || bLoopPlaying)
      BinkNextFrame(pBinkMovie);
    else
      Unload();
  }
}



//----- (004BEE6B) --------------------------------------------------------
void VideoPlayer::SmackDrawFrame(int a3, int a4)
{
  VideoPlayer *v4; // esi@1
  _SMACK *v5; // eax@3
  unsigned int v6; // eax@5
  char v7; // zf@5
  int v8; // eax@11
  _SMACK *v9; // eax@13
  _SMACK *v10; // eax@16
  _SMACK *v11; // eax@20
  unsigned int v12; // [sp-1Ch] [bp-34h]@7
  LONG v13; // [sp-18h] [bp-30h]@7
  LONG v14; // [sp-14h] [bp-2Ch]@7
  signed int v15; // [sp-10h] [bp-28h]@7
  signed int v16; // [sp-Ch] [bp-24h]@7
  signed int v17; // [sp-8h] [bp-20h]@7
  struct tagPOINT Point; // [sp+8h] [bp-10h]@7
  unsigned short *pOutSurface; // [sp+10h] [bp-8h]@5
  unsigned int uPixelsPerRow; // [sp+14h] [bp-4h]@5

  v4 = this;
  if ( !pRenderer->bWindowMode || window->Focused())
  {
    v5 = v4->pSmackerMovie;
    if ( v5->NewPalette )
      SmackBlitSetPalette(v4->pSmackMovieBlit, v5->Palette, v5->PalType);
    SmackDoFrame(v4->pSmackerMovie);
    pRenderer->LockFrontBuffer((void **)&pOutSurface, &uPixelsPerRow);
    v6 = 2 * uPixelsPerRow;
    v7 = v4->bFirstFrame == 0;
    uPixelsPerRow *= 2;
    if ( !v7 )
    {
      if ( pRenderer->bWindowMode )
      {
        Point.y = 0;
        Point.x = 0;
        ClientToScreen(window->GetApiHandle(), &Point);
        v17 = -1;
        v16 = 480;
        v15 = 640;
        v14 = Point.y;
        v13 = Point.x;
        v12 = uPixelsPerRow;
      }
      else
      {
        v17 = -1;
        v16 = 480;
        v15 = 640;
        v14 = 0;
        v13 = 0;
        v12 = v6;
      }
      SmackBlitClear(v4->pSmackMovieBlit, pOutSurface, v12, v13, v14, v15, v16, v17);
      v4->bFirstFrame = 0;
    }
    pRenderer->RestoreFrontBuffer();
    if ( pRenderer->bWindowMode )
    {
      Point.y = 0;
      Point.x = 0;
      ClientToScreen(window->GetApiHandle(), &Point);
      v8 = SmackToBufferRect(v4->pSmackerMovie, 0);
      if ( a4 )
      {
        while ( v8 )
        {
          v9 = v4->pSmackerMovie;
          SmackBlit(
            v4->pSmackMovieBlit,
            pOutSurface,
            uPixelsPerRow,
            v9->LastRectx + Point.x / 2,
            a3 + v9->LastRecty + Point.y / 2,
            v4->pSomeSmackerBuffer,
            v9->Width,
            v9->LastRectx,
            v9->LastRecty,
            v9->LastRectw,
            v9->LastRecth);
          v8 = SmackToBufferRect(v4->pSmackerMovie, 0);
        }
      }
      else
      {
        while ( v8 )
        {
          v10 = v4->pSmackerMovie;
          SmackBlit(v4->pSmackMovieBlit, pOutSurface, uPixelsPerRow, Point.x + v10->LastRectx, a3 + Point.y + v10->LastRecty,
            v4->pSomeSmackerBuffer, v10->Width, v10->LastRectx, v10->LastRecty, v10->LastRectw, v10->LastRecth);
          v8 = SmackToBufferRect(v4->pSmackerMovie, 0);
        }
      }
    }
    else
    {
      while ( SmackToBufferRect(v4->pSmackerMovie, 0) )
      {
        v11 = v4->pSmackerMovie;
        SmackBlit(v4->pSmackMovieBlit, pOutSurface, uPixelsPerRow, v11->LastRectx, a3 + v11->LastRecty, v4->pSomeSmackerBuffer,
          v11->Width, v11->LastRectx, v11->LastRecty, v11->LastRectw, v11->LastRecth);
      }
    }
    pRenderer->UnlockFrontBuffer();
    if ( v4->pSmackerMovie->FrameNum != v4->pSmackerMovie->Frames - 1 || v4->bLoopPlaying )
      SmackNextFrame(v4->pSmackerMovie);
    else
      Unload();
  }
}

void VideoPlayer::_inlined_in_463149()
{
      if ( pSmackerMovie && !SmackWait(pSmackerMovie) )
      {
        pRenderer->BeginScene();
        pMouse->DrawCursorToTarget();
        SmackUpdatePalette();
        pMouse->_469EA4();
        pRenderer->EndScene();
      }
      else if ( pBinkMovie && !BinkWait(pBinkMovie) )
      {
        pRenderer->BeginScene();
        pMouse->DrawCursorToTarget();
        BinkUpdatePalette();
        pMouse->_469EA4();
        pRenderer->EndScene();
      }
}

//----- (004BF08B) --------------------------------------------------------
void VideoPlayer::SmackUpdatePalette()
{
  VideoPlayer *v2; // esi@1
  unsigned __int16 *v3; // ebx@1
  unsigned int v4; // edi@1
  unsigned int v5; // eax@1
  _SMACK *v6; // eax@1

  v2 = this;
  pRenderer->BeginScene();
  v3 = pRenderer->pTargetSurface;
  v4 = pRenderer->uTargetSurfacePitch;
  v5 = SmackCheckSurfaceFromat();
  SmackToBuffer(v2->pSmackerMovie, 8, 8, 2 * v4, pRenderer->field_14, v3, v5);
  v6 = v2->pSmackerMovie;
  if ( v6->NewPalette )
  {
    SmackBufferNewPalette((long)pSmackerBuffer, (long)v6->Palette, LOWORD(v6->PalType));
    SmackColorRemapWithTrans(
      (long)pSmackerMovie,
      (long)pSmackerBuffer->Palette,
      (long)pSmackerBuffer->MaxPalColors,
      (long)pSmackerBuffer->PalType,
      1000);
  }
  SmackDoFrame(v2->pSmackerMovie);
  if ( v2->pSmackerMovie->FrameNum != v2->pSmackerMovie->Frames - 1 || v2->bLoopPlaying )
    SmackNextFrame(v2->pSmackerMovie);
  else
    Unload();
  pRenderer->EndScene();
}





//----- (004BF141) --------------------------------------------------------
_BINK *VideoPlayer::OpenBink(const char *pName)
{
  //VideoPlayer *v2; // esi@1
  signed int v3; // edi@1
  int v4; // ebx@2
  signed int v5; // edi@5
  int v6; // ebx@6
  HANDLE v8; // [sp-8h] [bp-18h]@10
  unsigned int v9; // [sp-4h] [bp-14h]@10

  for (uint i = 0; i < uNumMightVideoHeaders; ++i)
    if (!_stricmp(pName, pMightVideoHeaders[i].pVideoName))
    {
      SetFilePointer(hMightVid, pMightVideoHeaders[i].uFileOffset, 0, FILE_BEGIN);

      if (uBinkVersion < 0x0003000000000000)
        return BinkOpen(hMightVid, 0x8800000);
      else
        return BinkOpen(hMightVid, 0x82000000);
    }

  for (uint i = 0; i < uNumMagicVideoHeaders; ++i)
    if (!_stricmp(pName, pMagicVideoHeaders[i].pVideoName))
    {
      SetFilePointer(hMagicVid, pMagicVideoHeaders[i].uFileOffset, 0, FILE_BEGIN);

      if (uBinkVersion < 0x0003000000000000)
        return BinkOpen(hMagicVid, 0x8800000);
      else
        return BinkOpen(hMagicVid, 0x82000000);
    }

  return nullptr;
}
//----- (004BF1E6) --------------------------------------------------------
_SMACK *VideoPlayer::OpenSmack(const char *pFilename)
{
  if ( this->uNumMightVideoHeaders )
  {
    for ( uint i = 0; i < (signed int)this->uNumMightVideoHeaders; ++i)
    {
      if ( !_stricmp(this->pMightVideoHeaders[i].pVideoName, pFilename) )
      {
        SetFilePointer(this->hMightVid, this->pMightVideoHeaders[i].uFileOffset, 0, 0);
        return SmackOpen(this->hMightVid, 0x7140, -1);
      }
    }
  }
  if ( this->uNumMagicVideoHeaders )
  {
    for ( uint i = 0; i < (signed int)this->uNumMagicVideoHeaders; ++i )
    {
      if ( !_stricmp(this->pMagicVideoHeaders[i].pVideoName, pFilename) )
      {
        SetFilePointer(this->hMagicVid, this->pMagicVideoHeaders[i].uFileOffset, 0, 0);
        return SmackOpen(this->hMagicVid, 0x7140, -1);
      }
    }
  }
  return 0;
}

//----- (004BF28F) --------------------------------------------------------
void VideoPlayer::OpenHouseMovie(const char *pMovieName, unsigned int a3_1)
{
  if ( !this->field_54 )
  {
    Prepare();
    this->bLoopPlaying = a3_1;
    if ( LOBYTE(this->field_104) == 1 )
    {
      MessageBoxA(nullptr, "Unsupported Bink playback!", "E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Video.cpp:925", 0);
      return;
    }
    sprintfex(Str2, "%s.smk", pMovieName);
    this->pSmackerMovie = OpenSmack(Str2);
    if ( !this->pSmackerMovie )
    {
      this->Unload();
      sprintf(pTmpBuf.data(), "Can't load %s", &Str2);
      MessageBoxA(nullptr, pTmpBuf.data(), "E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Video.cpp:937", 0);
      return;
    }
    this->uMovieFormat = 1;
    strcpy(this->pCurrentMovieName, (const char *)pMovieName);
    this->dword_0000A0 = 1;
    this->pSmackerBuffer = (_SMACKBUF *)SmackBufferOpen(window->GetApiHandle(), 4, LOWORD(this->pSmackerMovie->Width), LOWORD(this->pSmackerMovie->Height), LOWORD(this->pSmackerMovie->Width), LOWORD(this->pSmackerMovie->Height));
    if ( this->pSmackerBuffer )
    {
      pRenderer->BeginScene();
      SmackToBuffer(this->pSmackerMovie, 8, 8, 2 * pRenderer->uTargetSurfacePitch, pRenderer->field_14, pRenderer->pTargetSurface, SmackCheckSurfaceFromat());
      pRenderer->EndScene();
    }
    SmackVolumePan(this->pSmackerMovie, 1040384, (signed __int64)(pSoundVolumeLevels[(char)uSoundVolumeMultiplier] * 32767.0), 32767);
  }
}

//----- (004BF3F9) --------------------------------------------------------
bool VideoPlayer::AnyMovieLoaded()
{
  return pSmackerMovie || pBinkMovie;
}

//----- (004BF411) --------------------------------------------------------
void VideoPlayer::OpenGlobalMovie(const char *pFilename, unsigned int bLoop, int a4)
{
  unsigned int v10; // eax@11
  char pVideoName[120]; // [sp+Ch] [bp-28h]@2

  if ( !this->field_54 )
  {
    Prepare();
    this->bLoopPlaying = bLoop;
    sprintf(pVideoName, "%s.bik", pFilename);
    this->pBinkMovie = OpenBink(pVideoName);
    if ( this->pBinkMovie )
    {
      this->uMovieFormat = 2;
      strcpy(this->pCurrentMovieName, pFilename);
      pBinkMovie = this->pBinkMovie;
      this->dword_0000A0 = 1;
      if ( pBinkMovie )
        this->pBinkBuffer = CreateBinkBuffer(pBinkMovie->uWidth, pBinkMovie->uHeight, 0);
    }
    else
    {
      Unload();
      sprintfex(pVideoName, "%s.smk", pFilename);
      this->pSmackerMovie = OpenSmack(pVideoName);
      if ( !this->pSmackerMovie )
      {
        Unload();
        sprintf(pVideoName, "Can't load file - anims\\%s.smk", pFilename);
        MessageBoxA(0, pVideoName, "Smacker Error", 0);
        return;
      }
      this->uMovieFormat = 1;
      strcpy(this->pCurrentMovieName, pFilename);
      this->dword_0000A0 = 2;
      this->pSomeSmackerBuffer = (char *)malloc(this->pSmackerMovie->Width * this->pSmackerMovie->Height);
      if ( !this->pSomeSmackerBuffer )
      {
        Unload();
        sprintf(pVideoName, "Can't allocate memory for buffer");
        MessageBoxA(0, pVideoName, "Smacker Error", 0);
        return;
      }
      SmackToBuffer(this->pSmackerMovie, 0, 0, this->pSmackerMovie->Width, this->pSmackerMovie->Height, this->pSomeSmackerBuffer, 0);

      v10 = SmackCheckSurfaceFromat();
      if ( a4 )
      {
        v10 |= SMACKBLIT2X;

        if (uCPUSpeed >= 165 )
          v10 |= SMACKBLIT2XSMOOTHING;
        else
          Log::Warning(L"Smacker seems to use lower resolution because it thinks CPU is slow");
      }

      this->pSmackMovieBlit = SmackBlitOpen(v10);
      if ( !this->pSmackMovieBlit )
      {
        Unload();
        sprintf(pVideoName, "Failed to open Blit API");
        MessageBoxA(0, pVideoName, "Smacker Error", 0);
        return;
      }
    }
  }
}


//----- (004BF5B2) --------------------------------------------------------
void VideoPlayer::_4BF5B2()
{
  if ( this->uMovieFormat == 2 )
  {
    BinkGoto(pBinkMovie, 1, 0);
    BinkDoFrame(pBinkMovie);
    BinkNextFrame(pBinkMovie);
  }
  else
  {
    if ( this->uMovieFormat != 1 )
      return;
    SmackGoto(pSmackerMovie, 1);
    if ( pVersion->pVersionInfo.dwPlatformId != VER_PLATFORM_WIN32_NT || pVersion->pVersionInfo.dwMajorVersion != 4 )
    {
      SmackDoFrame(pSmackerMovie);
      SmackNextFrame(pSmackerMovie);
    }
  }
  pMouse->_469E24();
  if ( window_SpeakInHouse && window_SpeakInHouse->ptr_1C == (void *)165 && !this->pSmackerMovie )
  {
    bGameoverLoop = 1;
    HouseDialogPressCloseBtn();
    window_SpeakInHouse->Release();
    pParty->uFlags &= 0xFFFFFFFDu;
    if ( EnterHouse(HOUSE_BODY_GUILD_ERATHIA) )
    {
      pAudioPlayer->PlaySound(SOUND_Invalid, 0, 0, -1, 0, 0, 0, 0);
      window_SpeakInHouse = GUIWindow::Create(0, 0, 640, 480, WINDOW_HouseInterior, 165, 0);
      window_SpeakInHouse->CreateButton(0x3Du, 0x1A8u, 0x1Fu, 0, 2, 94, UIMSG_SelectCharacter, 1u, 0x31u, "", 0);
      window_SpeakInHouse->CreateButton(0xB1u, 0x1A8u, 0x1Fu, 0, 2, 94, UIMSG_SelectCharacter, 2u, 0x32u, "", 0);
      window_SpeakInHouse->CreateButton(0x124u, 0x1A8u, 0x1Fu, 0, 2, 94, UIMSG_SelectCharacter, 3u, 0x33u, "", 0);
      window_SpeakInHouse->CreateButton(0x197u, 0x1A8u, 0x1Fu, 0, 2, 94, UIMSG_SelectCharacter, 4u, 0x34u, "", 0);
    }
    bGameoverLoop = 0;
  }
}

//----- (004BF73A) --------------------------------------------------------
void VideoPlayer::SelectMovieType()
{
  char Source[32]; // [sp+Ch] [bp-40h]@1

  strcpy(Source, this->pCurrentMovieName);
  Unload();
  if ( this->dword_0000A0 == 1 )
    OpenHouseMovie(Source, LOBYTE(this->bLoopPlaying));
  else if ( this->dword_0000A0 == 2 )
    OpenGlobalMovie(Source, LOBYTE(this->bLoopPlaying), 1);
  else
    __debugbreak();
}

//----- (004BF8F6) --------------------------------------------------------
void VideoPlayer::PlayDeathMovie()
{
  bStopBeforeSchedule = 0;
  pResetflag = 0;
  PlayFullscreenMovie(MOVIE_Death, true);
  //pGame->pCShow->PlayMovie(MOVIE_Death, 1);
}

//----- (004BE6F5) --------------------------------------------------------
VideoPlayer::VideoPlayer() :
  field_54(0),
  pBinkMovie(nullptr), pBinkBuffer(nullptr),
  pSmackerMovie(nullptr), pSmackerBuffer(nullptr), pSmackMovieBlit(nullptr)
{
  //RGBTexture::RGBTexture(&pVideoPlayer->pVideoFrame);
  bStopBeforeSchedule = false;
  pResetflag = 0;
  byte_F8BC0C = 0;
  //pBinkMovie = nullptr;
}
