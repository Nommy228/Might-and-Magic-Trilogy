/*#pragma once
#define WIN32_LEAN_AND_MEAN

#include "OSWindow.h"
#include <cstdint>
#include <cstdio>

//#include "lib\legacy_dx\d3d.h"

#include "VectorTypes.h"

#include <d3d11.h>
#include "RenderStruct.h"
#pragma comment(lib, "d3d11.lib")

class D3D11
{
  HINSTANCE               g_hInst;
  HWND                    g_hWnd;
  D3D_DRIVER_TYPE         g_driverType;       //Этот параметр указывает, производить вычисления в видеокарте или в центральном процессоре. 
  D3D_FEATURE_LEVEL       g_featureLevel;     //параметр, указывающий, какую версию DirectX поддерживает наша видеокарта.
  ID3D11Device*           g_pd3dDevice;       //устройство d3d11
  ID3D11DeviceContext*    g_pImmediateContext;// контекст устройства
  IDXGISwapChain*         g_pSwapChain;       // цепочка обмена
  ID3D11RenderTargetView* g_pRenderTargetView;


public:

  int *pActiveZBuffer;
  //IDirectDraw4 *pDirectDraw4;
  //IDirectDrawSurface4 *pFrontBuffer4;
  //IDirectDrawSurface4 *pBackBuffer4;
  ID3D11Texture2D* pBackBuffer;
  void        *pTargetSurface;
  unsigned int uTargetSurfacePitch;
  unsigned int bUseColoredLights;
  unsigned int bTinting;
  unsigned int bUsingSpecular;
  uint32_t uFogColor;
  unsigned int pHDWaterBitmapIDs[7];
  int hd_water_current_frame;
  int hd_water_tile_id;
  void (*pBeforePresentFunction)();
  uint32_t bFogEnabled;
  RenderBillboardD3D pBillboardRenderListD3D[1000];
  unsigned int uNumBillboardsToDraw;

  D3D11():
  g_hInst(NULL), g_hWnd(NULL), g_driverType(D3D_DRIVER_TYPE_NULL), g_featureLevel(D3D_FEATURE_LEVEL_11_0),
  g_pd3dDevice(NULL), g_pImmediateContext(NULL), g_pSwapChain(NULL), g_pRenderTargetView(NULL){}

  HRESULT InitDevice();      // Инициализация устройств DirectX
  void CleanupDevice();      // Удаление созданнных устройств DirectX
  //void Render();             // Функция рисования

  bool Initialize(OSWindow *window, bool bColoredLights, uint32_t uDetailLevel, bool bTinting);


  void ClearBlack();
  void PresentBlackScreen();

  void SaveWinnersCertificate(const char *a1);
  void ClearTarget(unsigned int uColor);
  void Present();

  void _49FD3A_fullscreen();
  bool InitializeFullscreen();

  void CreateZBuffer();
  void Release();

  bool SwitchToWindow();
  void RasterLine2D(signed int uX, signed int uY, signed int uZ, signed int uW, unsigned __int16 uColor);
  void ClearZBuffer(int a2, int a3);
  void SetRasterClipRect(unsigned int uX, unsigned int uY, unsigned int uZ, unsigned int uW);
  bool LockSurface_DDraw4(IDirectDrawSurface4 *pSurface, DDSURFACEDESC2 *pDesc, unsigned int uLockFlags);
  void GetTargetPixelFormat(DDPIXELFORMAT *pOut);
  void LockRenderSurface(void **pOutSurfacePtr, unsigned int *pOutPixelsPerRow);

  void UnlockBackBuffer();
  void RestoreBackBuffer();

  void LockFrontBuffer(void **pOutSurface, unsigned int *pOutPixelsPerRow);
  void UnlockFrontBuffer();
  void RestoreFrontBuffer();

  void BltToFront(RECT *pDstRect, IDirectDrawSurface *pSrcSurface, RECT *pSrcRect, unsigned int uBltFlags);
  void BltBackToFontFast(int a2, int a3, RECT *a4);
  void BeginSceneD3D();

  unsigned int GetActorTintColor(float a2, int tint, int a4, int a5, RenderBillboard *a6);

  void DrawPolygon(unsigned int uNumVertices, struct Polygon *a3, ODMFace *a4, IDirect3DTexture2 *pTexture);
  void DrawTerrainPolygon(unsigned int uNumVertices, struct Polygon *a4, IDirect3DTexture2 *a5, bool transparent, bool clampAtTextureBorders);
  void DrawIndoorPolygon(unsigned int uNumVertices, struct BLVFace *a3, IDirect3DTexture2 *pHwTex, struct Texture *pTex, int uPackedID, unsigned int uColor, int a8);

  void MakeParticleBillboardAndPush_BLV(RenderBillboardTransform_local0 *a2, IDirect3DTexture2 *a3, unsigned int uDiffuse, int angle);
  void MakeParticleBillboardAndPush_ODM(RenderBillboardTransform_local0 *a2, IDirect3DTexture2 *a3, unsigned int uDiffuse, int angle);

  void DrawBillboards_And_MaybeRenderSpecialEffects_And_EndScene();
  void DrawBillboard_Indoor(RenderBillboardTransform_local0 *pSoftBillboard, Sprite *pSprite, int dimming_level);
  void _4A4CC9_AddSomeBillboard(struct stru6_stru1_indoor_sw_billboard *a1, int diffuse);
  void TransformBillboardsAndSetPalettesODM();
  void DrawBillboardList_BLV();

  void DrawProjectile(float srcX, float srcY, float a3, float a4, float dstX, float dstY, float a7, float a8, IDirect3DTexture2 *a9);
  bool LoadTexture(const char *pName, unsigned int bMipMaps, IDirectDrawSurface4 **pOutSurface, IDirect3DTexture2 **pOutTexture);
  bool MoveSpriteToDevice(Sprite *pSprite);

  void BeginScene();
  void EndScene();
  void ScreenFade(unsigned int color, float t);

  void SetTextureClipRect(unsigned int uX, unsigned int uY, unsigned int uZ, unsigned int uW);
  void ResetTextureClipRect();
  void DrawTextureRGB(unsigned int uOutX, unsigned int uOutY, RGBTexture *a4);
  void CreditsTextureScroll(unsigned int pX, unsigned int pY, int move_X, int move_Y, RGBTexture *pTexture);
  void DrawTextureIndexed(unsigned int uX, unsigned int uY, struct Texture *a4);

  void ZBuffer_Fill_2(signed int a2, signed int a3, struct Texture *pTexture, int a5);
  void DrawMaskToZBuffer(signed int uOutX, unsigned int uOutY, struct Texture *pTexture, int zVal);
  void DrawTextureTransparent(unsigned int uX, unsigned int uY, struct Texture *pTexture);
  void DrawAura(unsigned int a2, unsigned int a3, struct Texture *a4, struct Texture *a5, int a6, int a7, int a8);
  void _4A65CC(unsigned int x, unsigned int y, struct Texture *a4, struct Texture *a5, int a6, int a7, int a8);

  void DrawTransparentRedShade(unsigned int a2, unsigned int a3, struct Texture *a4);
  void DrawTransparentGreenShade(signed int a2, signed int a3, struct Texture *pTexture);
  void DrawFansTransparent(const RenderVertexD3D3 *vertices, unsigned int num_vertices);

  void DrawMasked(signed int a2, signed int a3, struct Texture *pTexture, unsigned __int16 mask);
  void GetLeather(unsigned int a2, unsigned int a3, struct Texture *a4, __int16 height);

  void DrawTextPalette(int x, int y, unsigned char* font_pixels, int a5, unsigned int uFontHeight, unsigned __int16 *pPalette, int a8);
  void DrawText(signed int uOutX, signed int uOutY, unsigned __int8 *pFontPixels, unsigned int uCharWidth, unsigned int uCharHeight, unsigned __int16 *pFontPalette, unsigned __int16 uFaceColor, unsigned __int16 uShadowColor);

  void FillRectFast(unsigned int uX, unsigned int uY, unsigned int uWidth, unsigned int uHeight, unsigned int uColor16);
  void _4A6DF5(unsigned __int16 *pBitmap, unsigned int uBitmapPitch, struct Vec2_int_ *pBitmapXY, void *pTarget, unsigned int uTargetPitch, Vec4_int_ *a7);
  void DrawTranslucent(unsigned int a2, unsigned int a3, struct Texture *a4);

  void DrawBuildingsD3D();
  //struct BSPModel *DrawBuildingsSW();
  //int OnOutdoorRedrawSW();

  void DrawIndoorSky(unsigned int uNumVertices, unsigned int uFaceID);
  void DrawOutdoorSkyD3D();
  //int DrawSkySW(struct Span *a1, Polygon *a2, int a3);
  void DrawOutdoorSkyPolygon(unsigned int uNumVertices, struct Polygon *pSkyPolygon, IDirect3DTexture2 *pTexture);
  void DrawIndoorSkyPolygon(signed int uNumVertices, struct Polygon *pSkyPolygon, IDirect3DTexture2 *pTexture);

  void PrepareDecorationsRenderList_ODM();
  void DrawSpriteObjects_ODM();

  //float DrawBezierTerrain();
  void RenderTerrainD3D();
  void DrawTerrainD3D(int a1, int edx0, int a3, int unk4);
  //void DrawTerrainSW(int a1, int a2, int a3, int a4);

  //void ExecOutdoorDrawSW();
  void ChangeBetweenWinFullscreenModes();
  bool AreRenderSurfacesOk();
  bool IsGammaSupported();

  void SaveScreenshot(const char *pFilename, unsigned int width, unsigned int height);
  void PackScreenshot(unsigned int width, unsigned int height, void *out_data, unsigned int data_size, unsigned int *screenshot_size);
  void SavePCXScreenshot();

  int _46А6АС_GetActorsInViewport(int pDepth);

  void BeginLightmaps();
  void EndLightmaps();  
  void BeginLightmaps2();
  void EndLightmaps2();
  bool DrawLightmap(struct Lightmap *pLightmap, struct Vec3_float_ *pColorMult, float z_bias);

  void BeginDecals();
  void EndDecals();
  void DrawDecal(struct Decal *pDecal, float z_bias);
  
  void do_draw_debug_line_d3d(const RenderVertexD3D3 *pLineBegin, signed int sDiffuseBegin, const RenderVertexD3D3 *pLineEnd, signed int sDiffuseEnd, float z_stuff);
  void DrawLines(const RenderVertexD3D3 *vertices, unsigned int num_vertices);

  void DrawSpecialEffectsQuad(const RenderVertexD3D3 *vertices, IDirect3DTexture2 *texture);

  void am_Blt_Copy(RECT *pSrcRect, POINT *pTargetXY, int a3);
  void am_Blt_Chroma(RECT *pSrcRect, POINT *pTargetPoint, int a3, int blend_mode);

  inline void ToggleTint()          {bTinting = !bTinting;}
  inline void ToggleColoredLights() {bUseColoredLights = !bUseColoredLights;}

  inline unsigned int GetRenderWidth() {return window->GetWidth();}
  inline unsigned int GetRenderHeight() {return window->GetHeight();}


  friend void Present_NoColorKey();

};
*/