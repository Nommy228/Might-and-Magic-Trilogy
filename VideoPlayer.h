#pragma once
#include "OSWindow.h"
#include "Texture.h"




#pragma pack(push, 1)



struct _BINK
{
  int uWidth;
  int uHeight;
};

struct _BINK_1_5_21_0: public _BINK
{
  int uNumFrames;
  int uCurrentFrame;
  int Data1;
  int uFrameRate;
  int field_18[3];
  int uFlags;
};

struct _BINK_3_0_0_0: public _BINK
{
  int unk_0;
  int unk_1;
  int uNumFrames;
  int uCurrentFrame;
  int _unk2;
  int _unk_[10];
};


struct _BINKBUF
{
  int uWidth;
  int uHeight;
  int field_8;
  int field_C;
  int uBinkDDSurfaceType;
  void *pDDrawSurfaceData;
  int uDDrawSurfacePitch;
  int field_1C;
  int field_20;
  int target_width;
  int target_height;
  int field_2C;
  int field_30;
  int field_34;
  int field_38;
  int field_3C;
  int field_40;
  int field_44;
  struct IDirectDrawSurface *pTargetDDrawSurface;
  int field_4C;
  int uRectX;
  int uRectY;
  HWND hWnd;
  int field_5C;
  float field_60;
  float field_64;
  int field_68;
  int field_6C;
  void *pDDrawSurfaceData_;
  int field_74;
  int field_78;
};
struct _BINKBUF_1_5_21_0: public _BINKBUF
{
  int field_7C;
  int field_80;
  int field_84;
  int field_88;
  int field_8C;
  int field_90;
  int field_94;
  int field_98;
  int field_9C;
  int field_A0;
};

struct _BINKBUF_3_0_0_0: public _BINKBUF
{
};
#pragma pack(pop)




#pragma pack(push, 1)
struct MovieHeader
{
  char pVideoName[40];
  unsigned int uFileOffset;
};
#pragma pack(pop)





void  ShowIntroVideo_and_LoadingScreen();




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

  void PlayDeathMovie();
  unsigned int SmackCheckSurfaceFromat();
  void Initialize(OSWindow *window);
  void Prepare();
  void Unload();
  void FastForwardToFrame(unsigned int uFrameNum);
  void BinkDrawFrame(int a3, int a4);
  void BinkUpdatePalette() {}
  void SmackDrawFrame(int a3, int a4);
  void SmackUpdatePalette();
  _BINK *OpenBink(const char *pName);
  struct _SMACK *OpenSmack(const char *pFilename);
  void OpenHouseMovie(const char *pMovieName, unsigned int a3_1);//0x4BF28F
  bool AnyMovieLoaded();
  void OpenGlobalMovie(const char *pFilename, unsigned int bLoop, int a4);
  void _4BF5B2();
  void SelectMovieType();//0x4BF73A
  _BINKBUF *CreateBinkBuffer(unsigned int uWidth, unsigned int uHeight, char a4);
  void _inlined_in_463149();

  static void MovieLoop(const char *pMovieName, int a2, int a3, int a4);


  RGBTexture pVideoFrame;
  struct _SMACK *pSmackerMovie;
  struct _SMACKBUF *pSmackerBuffer;
  char *pSomeSmackerBuffer;
  int field_34;
  MovieHeader *pMightVideoHeaders;
  MovieHeader *pMagicVideoHeaders;
  int pResetflag;
  int field_44;
  unsigned int uNumMightVideoHeaders;
  unsigned int uNumMagicVideoHeaders;
  int uBinkDirectDrawSurfaceType;
  int field_54;
  unsigned int bPlayingMovie;
  unsigned int bFirstFrame;
  unsigned int bUsingSmackerMMX;
  unsigned int bLoopPlaying;
  int field_68;
  unsigned int bStopBeforeSchedule;
  //HWND hWindow;
  OSWindow *window;
  struct _SMACKBLIT *pSmackMovieBlit;
  HANDLE hMightVid;
  HANDLE hMagicVid;
  _BINK *pBinkMovie;
  _BINKBUF *pBinkBuffer;
  char field_88[20];
  unsigned int uMovieFormat;
  int dword_0000A0;
  char pCurrentMovieName[64];
  char pVideoFrameTextureFilename[32];
  int field_104;
};
#pragma pack(pop)





extern VideoPlayer *pVideoPlayer;
