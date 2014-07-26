#pragma once
#include "Texture.h"


/*  278 */
#pragma pack(push, 1)
struct GUIProgressBar
{
  enum Type: unsigned __int32
  {
    TYPE_None = 0,
    TYPE_Fullscreen = 1,
    TYPE_Box = 2
  };

  bool Initialize(Type type);
  void Reset(unsigned __int8 uMaxProgress);
  void Progress();
  void Release();
  void Draw();

  __int16 uX;
  __int16 uY;
  __int16 uWidth;
  __int16 uHeight;
  char field_8;
  char field_9;
  char uProgressMax;
  char uProgressCurrent;
  Type uType;
  char field_10[8];
  //char field_11;
  //char field_12;
  //char field_13;
  //char field_14;
  //char field_15;
  //char field_16;
  //char field_17;
  RGBTexture pLoadingBg;
  RGBTexture field_40;
  RGBTexture field_68;
  RGBTexture field_90;
  RGBTexture field_B8;
  struct Texture field_E0;
  struct Texture pBardata;
  struct Texture pLoadingProgress;
};
#pragma pack(pop)



extern struct GUIProgressBar *pGameLoadingUI_ProgressBar;