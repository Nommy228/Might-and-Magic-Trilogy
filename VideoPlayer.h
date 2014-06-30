#pragma once
#include "OSWindow.h"
#include "Texture.h"
#include "MediaPlayer.h"

#pragma pack(push, 1)

#pragma pack(pop)

#pragma pack(push, 1)
#pragma pack(pop)

void ShowMM7IntroVideo_and_LoadingScreen();

extern "C"
{
#include "lib/libavcodec/avcodec.h"
#include "lib/libavformat/avformat.h"
#include "lib/libavutil/avutil.h"
#include "lib/libavutil/imgutils.h"
#include "lib/libswscale/swscale.h"
#include "lib/libswresample/swresample.h"
#include "lib/libavutil/opt.h"
}
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "swscale.lib")
#pragma comment(lib, "swresample.lib")

#include "lib/OpenAL/al.h"
#include "lib/OpenAL/alc.h"
#pragma comment(lib, "OpenAL32.lib")

#pragma pack(push, 1)
struct VideoPlayer
{
  VideoPlayer();
  //----- (004BECBD) --------------------------------------------------------
  virtual ~VideoPlayer()
  {
    bStopBeforeSchedule = false;
    pResetflag = 0;
    pVideoFrame.Release();
  }

  void Initialize(OSWindow *window);
  void Unload();

  void OpenHouseMovie(const char *pMovieName, unsigned int a3_1);//0x4BF28F
  void PlayDeathMovie();
  bool AnyMovieLoaded();
  void OpenGlobalMovie(const char *pFilename, unsigned int bLoop, int a4);
  void _4BF5B2();
  void SelectMovieType();//0x4BF73A
  void _inlined_in_463149();

  void MovieLoop(const char *pMovieName, int a2, int a3, int a4);
  void UpdatePalette();
  static int readFunction(void *, uint8_t *, int);
  static int64_t seekFunction(void *, int64_t, int);
  void LoadMovie(const char *);

  RGBTexture pVideoFrame;
  int field_34;

  int pResetflag;
  int field_44;

  int uBinkDirectDrawSurfaceType;
  int bBufferLoaded;
  unsigned int bPlayingMovie;
  unsigned int bFirstFrame;
  unsigned int bUsingSmackerMMX;
  unsigned int bLoopPlaying;
  int field_68;
  unsigned int bStopBeforeSchedule;
  OSWindow *window;
  struct _SMACKBLIT *pSmackMovieBlit;

  char field_88[20];
  unsigned int uMovieFormat;
  int uMovieFormatSwapped;
  char pCurrentMovieName[64];
  char pVideoFrameTextureFilename[32];
  int field_104;

  HANDLE hVidFile;
  int uSize;
  int uOffset;
};
#pragma pack(pop)

extern VideoPlayer *pVideoPlayer;
extern LRESULT __stdcall wWinProc(HWND hwnd, unsigned int msg, WPARAM wparam, LPARAM lparam);