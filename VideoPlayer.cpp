#define _CRT_SECURE_NO_WARNINGS

#include "mm7_unsorted_subs.h"
#include "Bink_Smacker.h"

#include "mm7_data.h"
#include "CShow.h"
#include "Mouse.h"

#include "VideoPlayer.h"
#include "MediaPlayer.h"
#include "AudioPlayer.h"
#include "Game.h"
#include "Render.h"
#include "Party.h"
#include "GUIWindow.h"
#include "Timer.h"
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

//----- (004BF794) --------------------------------------------------------
void ShowMM7IntroVideo_and_LoadingScreen()
{
  RGBTexture tex; // [sp+Ch] [bp-30h]@1
  unsigned int uTrackStartMS; // [sp+34h] [bp-8h]@8
  unsigned int uTrackEndMS; // [sp+38h] [bp-4h]@8

  pVideoPlayer->bStopBeforeSchedule = false;
  pVideoPlayer->pResetflag = 0;
  bGameoverLoop = true;
  if (!bNoVideo)
  {
    pRenderer->PresentBlackScreen();
    if ( !pVideoPlayer->bStopBeforeSchedule )
      PlayFullscreenMovie(MOVIE_Intro, true);
  }
//  char pContainerName[64];

  tex.Load("mm6title.pcx", 2);
  pRenderer->BeginScene();
  pRenderer->DrawTextureRGB(0, 0, &tex);
  free(tex.pPixels);
  tex.pPixels = 0;

  //LoadFonts_and_DrawCopyrightWindow();
  DrawMM7CopyrightWindow();

  pRenderer->EndScene();
  pRenderer->Present();

  #ifndef _DEBUG
    Sleep(1500);   // let the copyright window stay for a while
  #endif

  if (!bNoSound && pAudioPlayer->hAILRedbook )
  {
    pAudioPlayer->SetMusicVolume((signed __int64)(pSoundVolumeLevels[uMusicVolimeMultiplier] * 64.0));
    AIL_redbook_stop(pAudioPlayer->hAILRedbook);
    AIL_redbook_track_info(pAudioPlayer->hAILRedbook, 14, &uTrackStartMS, &uTrackEndMS);
    AIL_redbook_play(pAudioPlayer->hAILRedbook, uTrackStartMS + 1, uTrackEndMS);
  }
  bGameoverLoop = false;
}

//----- (004BE70E) --------------------------------------------------------
void VideoPlayer::MovieLoop(const char *pMovieName, int a2, int ScreenSizeFlag, int a4)
{
  int v4; // ebp@1
  MSG Msg; // [sp+Ch] [bp-1Ch]@12

  v4 = a2;
  if ( !(dword_6BE364_game_settings_1 & (GAME_SETTINGS_NO_HOUSE_ANIM | GAME_SETTINGS_NO_INTRO)) )
  {
    if ( a2 == 2 )
      v4 = 0;
    ShowCursor(0);//Убрать курсор
    OpenGlobalMovie(pMovieName, 0, ScreenSizeFlag);
    bPlayingMovie = 1;
    field_44 = v4;
    pRenderer->ClearTarget(0);
    pCurrentScreen = SCREEN_VIDEO;

    auto hwnd = pVideoPlayer->window->GetApiHandle();
    RECT rc_client;
    GetClientRect(hwnd, &rc_client);
    int client_width = rc_client.right - rc_client.left,
        client_height = rc_client.bottom - rc_client.top;

    HDC     dc = GetDC(hwnd);
    HDC     back_dc = CreateCompatibleDC(dc);
    HBITMAP back_bmp;
    SelectObject(back_dc, back_bmp = CreateCompatibleBitmap(dc, client_width, client_height));

	DWORD t = GetTickCount();//текущее время от запуска ОС

	end_current_file = false;//установить флаг конца видеоролика

    while (true) //Цикл воспроизведения видеоролика
    {
      if ( pVideoPlayer->bStopBeforeSchedule )
        break;
      while (PeekMessageA(&Msg, hwnd, 0, 0, PM_REMOVE))
      {
        if (Msg.message == 18)
          Game_DeinitializeAndTerminate(0);
        if (Msg.message == 15)
          break;
        TranslateMessage(&Msg);
        DispatchMessageA(&Msg);
      }

      double dt = (GetTickCount() - t) / 1000.0; //изменение времени
      //dt = 1.0/15.0;
      t = GetTickCount();

      //log("dt=%.5f\n", dt);

      auto image = new char[client_width * client_height * 4];

      pMovie->GetNextFrame(dt, image);	//получить следующий кадр

      if (end_current_file)//достигнут ли конец видео?
        break;

      if (image)
      {
        // draw to hwnd
        BITMAPINFO bmi;
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = client_width;
        bmi.bmiHeader.biHeight = -client_height;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;
        bmi.bmiHeader.biSizeImage = 0;
        bmi.bmiHeader.biXPelsPerMeter = 0;
        bmi.bmiHeader.biYPelsPerMeter = 0;
        bmi.bmiHeader.biClrUsed = 0;
        bmi.bmiHeader.biClrImportant = 0;
        GetDIBits(back_dc, back_bmp, 0, client_height, 0, &bmi, DIB_RGB_COLORS);
        SetDIBits(back_dc, back_bmp, 0, client_height, image, &bmi, DIB_RGB_COLORS);
        BitBlt(dc, 0, 0, client_width, client_height, back_dc, 0, 0, SRCCOPY);

        delete[] image;//удалить кадр
      }

      GUI_MainMenuMessageProc();  //после отрисовки, т.к. иначе вылетает dt = 0.03, image - ???

      if (pVideoPlayer->bStopBeforeSchedule == 1)//остановка видео
        Sleep(1000); //пауза после видеоролика
    }

    pVideoPlayer->Unload();

    if (a4 == 1)
      pCurrentScreen = SCREEN_GAME;

    pVideoPlayer->bPlayingMovie = 0;

    ShowCursor(1);//вернуть курсор

    if ( pCurrentScreen == SCREEN_VIDEO )
      pCurrentScreen = SCREEN_GAME;
  }
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
  hMightVid = CreateFileW(L"anims\\might7.vid", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0x8000080, 0);
  if ( hMightVid == INVALID_HANDLE_VALUE )
  {
    sprintf(pTmpBuf2.data(), "Can't open file - anims\\%s.smk", pTmpBuf.data());
    MessageBoxA(0, pTmpBuf2.data(), "Video File Error", 0);
    return;
  }
  strcpy(pTmpBuf.data(), "anims\\magic7.vid");
  hMagicVid = CreateFileW(L"anims\\magic7.vid", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0x8000080, 0);
  if ( hMagicVid == INVALID_HANDLE_VALUE )
  {
    if ( !bCanLoadFromCD )
    {
       sprintf(pTmpBuf2.data(), "Can't open file - anims\\%s.smk", pTmpBuf.data());
       MessageBoxA(0, pTmpBuf2.data(), "Video File Error", 0);
       return;
    }
    sprintf(pTmpBuf2.data(), "%c:\\%s", (unsigned __int8)cMM7GameCDDriveLetter, pTmpBuf.data());
    hMagicVid = CreateFileA(pTmpBuf2.data(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0x8000080, 0);
    if ( hMagicVid == (HANDLE)INVALID_HANDLE_VALUE )
    {
      sprintf(pTmpBuf2.data(), "Can't open file - %s", pTmpBuf.data());
      MessageBoxA(0, pTmpBuf2.data(), "Video File Error", 0);
      return;
    }
  }
  ReadFile(hMightVid, &uNumMightVideoHeaders, 4, &NumberOfBytesRead, 0);
  ReadFile(hMagicVid, &uNumMagicVideoHeaders, 4, &NumberOfBytesRead, 0);
  pMightVideoHeaders = (MovieHeader *)malloc(sizeof(MovieHeader) * uNumMightVideoHeaders + 2);
  pMagicVideoHeaders = (MovieHeader *)malloc(sizeof(MovieHeader) * uNumMagicVideoHeaders + 2);
  ReadFile(hMightVid, pMightVideoHeaders, 44 * uNumMightVideoHeaders, &NumberOfBytesRead, 0);
  ReadFile(hMagicVid, pMagicVideoHeaders, 44 * uNumMagicVideoHeaders, &NumberOfBytesRead, 0);
}

//----- (004BEBD7) --------------------------------------------------------
void VideoPlayer::Unload()
{
  if ( pSmackMovieBlit )
  {
    SmackBlitClose(pSmackMovieBlit);
    pSmackMovieBlit = 0;
  }
  bBufferLoaded = 0;
  uMovieFormat = 0;
  uMovieFormatSwapped = 0;
  memset(pCurrentMovieName, 0, 0x40);
  if ( pAudioPlayer->hAILRedbook && !bGameoverLoop )
    AIL_redbook_resume(pAudioPlayer->hAILRedbook);
  pEventTimer->Resume();

	delete pMovie;
	pMovie = nullptr;
}

void VideoPlayer::_inlined_in_463149()
{
	if (pMovie)
	{
		pRenderer->BeginScene();
		pMouse->DrawCursorToTarget();
		UpdatePalette();
		pMouse->ReadCursorWithItem();
		pRenderer->EndScene();
	}
}

void VideoPlayer::UpdatePalette() //UpdateVideo
{
  Log::Warning(L"smacker");
  loop_current_file = true;//зацикленный ролик
  pRenderer->BeginScene();
  if (end_current_file)//видео завершено/перезагрузка
  {
    int width = game_viewport_width;
    int height = game_viewport_height;
	MovieRelease();

    SetFilePointer(hVidFile, uOffset, nullptr, FILE_BEGIN);
    pMovie = nullptr;
    pMovie = pMediaPlayer->LoadMovieFromLOD(hVidFile, &readFunction, &seekFunction, width, height);
	end_current_file = false;
  }
  //else 
  //{
      double dt = (GetTickCount() - time_video_begin) / 1000.0;
      //dt = 1.0/15.0;
      time_video_begin = GetTickCount();

      //log("dt=%.5f\n", dt);

      auto image = new char[current_movie_width * current_movie_height * 4];

      pMovie->GetNextFrame(dt, image);

    int image_array[460 * 344];//game_viewport_width * game_viewport_height
    if (image)
    {
      memcpy(image_array, image, sizeof (image_array));
      for (unsigned int y = 8; y < 8 + game_viewport_height; ++y)//координаты местоположения видеоролика
      {
        for (unsigned int x = 8; x < 8 + game_viewport_width; ++x)
        {
          auto p = (unsigned __int32 *)pRenderer->pTargetSurface + x + y * pRenderer->uTargetSurfacePitch;
          *p = image_array[((x - 8) + ((y - 8)*game_viewport_width))];
        }
      }
      delete[] image;
    }
  //}
  pRenderer->EndScene();
}

//----- (004BF28F) --------------------------------------------------------
void VideoPlayer::OpenHouseMovie(const char *pMovieName, unsigned int a3_1)
{
  if (!this->bBufferLoaded)
  {
    //Prepare();
    pEventTimer->Pause();
    if (pAudioPlayer->hAILRedbook)
      AIL_redbook_pause(pAudioPlayer->hAILRedbook);

    bStopBeforeSchedule = false;
    bBufferLoaded = 1;
    bPlayingMovie = false;
    bFirstFrame = false;
    bUsingSmackerMMX = SmackUseMMX(1);

    this->bLoopPlaying = a3_1;
    if ( LOBYTE(this->field_104) == 1 )
    {
      MessageBoxA(nullptr, "Unsupported Bink playback!", "E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Video.cpp:925", 0);
      return;
    }

    LoadMovie(pMovieName);
	time_video_begin = GetTickCount();
  }
}

//----- (004BF3F9) --------------------------------------------------------
bool VideoPlayer::AnyMovieLoaded()
{
	return pMovie;// pSmackerMovie || pBinkMovie;
}

//----- (004BF411) --------------------------------------------------------
void VideoPlayer::OpenGlobalMovie(const char *pFilename, unsigned int bLoop, int a4)
{
//  unsigned int v10; // eax@11

  if (!this->bBufferLoaded)
  {
    pEventTimer->Pause();
	if (pAudioPlayer->hAILRedbook)
		AIL_redbook_pause(pAudioPlayer->hAILRedbook);

	bStopBeforeSchedule = false;
	bBufferLoaded = 1;
	bPlayingMovie = false;
	bFirstFrame = false;
	bUsingSmackerMMX = SmackUseMMX(1);
	this->bLoopPlaying = bLoop;
	LoadMovie(pFilename);
	return;
  }
}

 //----- (004BF5B2) --------------------------------------------------------
void VideoPlayer::_4BF5B2()
{
	//__debugbreak();
	
  /*if ( this->uMovieFormat == 2 )
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
  }*/
  pMouse->_469E24();
  if ( window_SpeakInHouse && window_SpeakInHouse->ptr_1C == (void *)165 && !pMovie)//!this->pSmackerMovie )
  {
    bGameoverLoop = true;
    HouseDialogPressCloseBtn();
    window_SpeakInHouse->Release();
    pParty->uFlags &= 0xFFFFFFFD;
    if ( EnterHouse(HOUSE_BODY_GUILD_ERATHIA) )
    {
      pAudioPlayer->PlaySound(SOUND_Invalid, 0, 0, -1, 0, 0, 0, 0);
      window_SpeakInHouse = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_HouseInterior, 165, 0);
      window_SpeakInHouse->CreateButton(0x3Du, 0x1A8u, 0x1Fu, 0, 2, 94, UIMSG_SelectCharacter, 1, 0x31, "", 0);
      window_SpeakInHouse->CreateButton(0xB1u, 0x1A8u, 0x1Fu, 0, 2, 94, UIMSG_SelectCharacter, 2, 0x32, "", 0);
      window_SpeakInHouse->CreateButton(0x124u, 0x1A8u, 0x1Fu, 0, 2, 94, UIMSG_SelectCharacter, 3, 0x33, "", 0);
      window_SpeakInHouse->CreateButton(0x197u, 0x1A8u, 0x1Fu, 0, 2, 94, UIMSG_SelectCharacter, 4, 0x34, "", 0);
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
  if ( this->uMovieFormatSwapped == 1 )
    OpenHouseMovie(Source, LOBYTE(this->bLoopPlaying));
  else if ( this->uMovieFormatSwapped == 2 )
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
bBufferLoaded(0),
  //pBinkMovie(nullptr), pBinkBuffer(nullptr),
  //pSmackerMovie(nullptr), pSmackerBuffer(nullptr), 
  pSmackMovieBlit(nullptr)
{
  //RGBTexture::RGBTexture(&pVideoPlayer->pVideoFrame);
  bStopBeforeSchedule = false;
  pResetflag = 0;
  pMediaPlayer = new Media::MPlayer;//создаётся плеер
  pMovie = nullptr;
  //pBinkMovie = nullptr;
}

int VideoPlayer::readFunction(void* opaque, uint8_t* buf, int buf_size)
{
  HANDLE stream = (HANDLE)opaque;
  //int numBytes = stream->read((char*)buf, buf_size);
  int numBytes;
  ReadFile(stream, (char *)buf, buf_size, (LPDWORD)&numBytes, NULL);
  return numBytes;
}

int64_t VideoPlayer::seekFunction(void* opaque, int64_t offset, int whence)
{
  if (whence == AVSEEK_SIZE)
    return pVideoPlayer->uSize;
  HANDLE h = (HANDLE)opaque;
  LARGE_INTEGER li;
  li.QuadPart = offset;

  if (!SetFilePointerEx(h, li, (PLARGE_INTEGER)&li, FILE_BEGIN))
    return -1;
  return li.QuadPart;
}

void VideoPlayer::LoadMovie(const char * pFilename)
{
  char pVideoNameBik[120]; // [sp+Ch] [bp-28h]@2
  char pVideoNameSmk[120]; // [sp+Ch] [bp-28h]@2

  sprintf(pVideoNameBik, "%s.bik", pFilename);
  sprintf(pVideoNameSmk, "%s.smk", pFilename);
  for (uint i = 0; i < uNumMightVideoHeaders; ++i)
  {
    if (!_stricmp(pVideoNameSmk, pMightVideoHeaders[i].pVideoName))
    {
      hVidFile = hMightVid;
      uOffset = pMightVideoHeaders[i].uFileOffset;
      uSize = pMightVideoHeaders[i + 1].uFileOffset - uOffset;
      this->uMovieFormat = 1;
      this->uMovieFormatSwapped = 2;
    }
  }
  for (uint i = 0; i < uNumMagicVideoHeaders; ++i)
  {
    if (!_stricmp(pVideoNameBik, pMagicVideoHeaders[i].pVideoName))
    {
      hVidFile = hMagicVid;
      uOffset = pMagicVideoHeaders[i].uFileOffset;
      uSize = pMagicVideoHeaders[i + 1].uFileOffset - uOffset;
      this->uMovieFormat = 2;
      this->uMovieFormatSwapped = 1;
    }
    if (!_stricmp(pVideoNameSmk, pMagicVideoHeaders[i].pVideoName))
    {
      hVidFile = hMagicVid;
      uOffset = pMagicVideoHeaders[i].uFileOffset;
      uSize = pMagicVideoHeaders[i + 1].uFileOffset - uOffset;
      this->uMovieFormat = 1;
      this->uMovieFormatSwapped = 2;
    }
  }
  if (!hVidFile)
  {
    Unload();
    MessageBoxA(0, "VideoPlayer error", "VideoPlayer Error", 0);
    return;
  }

  SetFilePointer(hVidFile, uOffset, 0, FILE_BEGIN);
  strcpy(this->pCurrentMovieName, pFilename);

  auto hwnd = pVideoPlayer->window->GetApiHandle();
  RECT rc_client;
  GetClientRect(hwnd, &rc_client);
  int client_width = rc_client.right - rc_client.left,
      client_height = rc_client.bottom - rc_client.top;


  pMovie = pMediaPlayer->LoadMovieFromLOD(hVidFile, &readFunction, &seekFunction, client_width, client_height);

}
