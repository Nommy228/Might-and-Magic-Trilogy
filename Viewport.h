#pragma once

/*  149 */
#pragma pack(push, 1)
struct Viewport
{
  inline Viewport()
  {
    field_of_view = 65536 / 2;
    SetScreen(0, 0, 639, 479);
  }

  void SetScreen(signed int uX, signed int uY, signed int uZ, signed int uW);
  void SetFOV(int a2);
  void SetViewport(signed int uX, signed int uY, signed int uZ, signed int uW);


  signed int uScreen_TL_X;
  signed int uScreen_TL_Y;
  signed int uScreen_BR_X;
  signed int uScreen_BR_Y;
  signed int uViewportTL_X;
  signed int uViewportTL_Y;
  signed int uViewportBR_X;
  signed int uViewportBR_Y;
  int uScreenWidth;
  int uScreenHeight;
  int uScreenCenterX;
  int uScreenCenterY;
  int field_of_view;
};
#pragma pack(pop)

void OnGameViewportClick();


extern struct Viewport *pViewport;



/*  201 */
#pragma pack(push, 1)
struct ViewingParams
{
  inline ViewingParams()
  {
    draw_sw_outlines = false;
    draw_d3d_outlines = false;
    field_4C = 0;
  }

  void CenterOnParty2();
  void  InitGrayPalette();
  void _443219();
  void  _443225();
  void  _443231();
  void _44323D();
  void CenterOnParty();
  void  AdjustPosition();
  void _443365();



  int uSomeX;
  int uSomeY;
  int uSomeZ;
  int uSomeW;
  unsigned int uScreen_topL_X;
  unsigned int uScreen_topL_Y;
  unsigned int uScreen_BttmR_X;
  unsigned int uScreen_BttmR_Y;
  int field_20;
  unsigned int uMinimapZoom;
  int field_28;
  unsigned int uMapBookMapZoom;
  int sViewCenterX;
  int sViewCenterY;
  __int16 indoor_center_x;
  __int16 indoor_center_y;
  int field_3C;
  unsigned int uTextureID_LocationMap;
  int bRedrawGameUI;
  int field_48;
  int field_4C;
  int draw_sw_outlines;
  int draw_d3d_outlines;
  int field_58;
  int field_5C;
  int field_60;
  int field_64;
  int field_68;
  unsigned __int16 pPalette[256];
};
#pragma pack(pop)



extern struct ViewingParams *viewparams;