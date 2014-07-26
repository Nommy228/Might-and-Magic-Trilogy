#pragma once

#include <cstdint>
#include <cstdio>

#include "lib\legacy_dx\d3d.h"
#include "OSWindow.h"
#include "RenderStruct.h"

#include "VectorTypes.h"

struct Render
{
  Render();
  ~Render();

  static Render *Create() {return new Render;}

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
  void LockFrontBuffer(void **pOutSurface, unsigned int *pOutPixelsPerRow);
  void UnlockFrontBuffer();
  void RestoreFrontBuffer();
  void RestoreBackBuffer();
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

  int _46ภ6ภั_GetActorsInViewport(int pDepth);

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

  public:
    int *pActiveZBuffer;
    IDirectDraw4 *pDirectDraw4;
    IDirectDrawSurface4 *pFrontBuffer4;
    IDirectDrawSurface4 *pBackBuffer4;
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

    inline void WritePixel16(int x, int y, unsigned __int16 color)
    {
      if (ddpfPrimarySuface.dwRGBBitCount == 32)
      {
        auto p = (unsigned __int32 *)pTargetSurface + x + y * uTargetSurfacePitch;
        *p = Color32(color);
      }
      else if (ddpfPrimarySuface.dwRGBBitCount == 16)
      {
        auto p = (unsigned __int16 *)pTargetSurface + x + y * uTargetSurfacePitch;
        *p = color;
      }
      else __debugbreak();
    }

    inline unsigned __int16 ReadPixel16(int x, int y)
    {
      if (ddpfPrimarySuface.dwRGBBitCount == 32)
      {
        auto p = (unsigned __int32 *)pTargetSurface + x + y * uTargetSurfacePitch;
        return Color16((*p >> 16) & 255, (*p >> 8) & 255, *p & 255);
      }
      else if (ddpfPrimarySuface.dwRGBBitCount == 16)
      {
        auto p = (unsigned __int16 *)pTargetSurface + x + y * uTargetSurfacePitch;
        return *p;
      }
      else __debugbreak();
    }

    inline void ToggleTint()          {bTinting = !bTinting;}
    inline void ToggleColoredLights() {bUseColoredLights = !bUseColoredLights;}

    inline unsigned int GetRenderWidth() {return window->GetWidth();}
    inline unsigned int GetRenderHeight() {return window->GetHeight();}

    inline void Sub01()
    {
      if (pRenderD3D && !bWindowMode)
        _49FD3A_fullscreen();
    }

    friend void Present_NoColorKey();

  protected:
    unsigned int uDesiredDirect3DDevice;
    int raster_clip_x;
    int raster_clip_y;    // clipping rect for raster ops
    int raster_clip_z;    //    like RasterLine2D for (mini)map
    int raster_clip_w;
    int *pDefaultZBuffer;
    OSWindow *window;
    unsigned int bWindowMode;
    RenderD3D *pRenderD3D;
    DDPIXELFORMAT ddpfPrimarySuface;
    unsigned int uTargetRBits;
    unsigned int uTargetGBits;
    unsigned int uTargetBBits;
    unsigned int uTargetRMask;
    unsigned int uTargetGMask;
    unsigned int uTargetBMask;
    unsigned int uNumSceneBegins;
    unsigned __int32 *pTargetSurface_unaligned;
    unsigned int uClipY;
    unsigned int uClipX;
    unsigned int uClipW;
    unsigned int uClipZ;
    unsigned int bClip;
    unsigned int uNumD3DSceneBegins;
    int using_software_screen_buffer;
    RenderHWLContainer pD3DBitmaps;
    RenderHWLContainer pD3DSprites;
    unsigned int bRequiredTextureStagesAvailable;
    unsigned int uLevelOfDetail;
    unsigned int uMaxDeviceTextureDim;
    unsigned int uMinDeviceTextureDim;

    void DoRenderBillboards_D3D();
    void SetBillboardBlendOptions(RenderBillboardD3D::OpacityType a1);
    void TransformBillboard(RenderBillboardTransform_local0 *a2, Sprite *pSprite, int dimming_level, RenderBillboard *pBillboard);
    unsigned int Billboard_ProbablyAddToListAndSortByZOrder(float z);
    unsigned int GetParentBillboardID(unsigned int uBillboardID);
    unsigned int GetBillboardDrawListSize();

    void DrawBorderTiles(struct Polygon *poly);

    unsigned short *MakeScreenshot(signed int width, signed int height);
    bool CheckTextureStages();
    void ParseTargetPixelFormat();

    void CreateDirectDraw();
    void SetDirectDrawCooperationMode(HWND hWnd, bool bFullscreen);
    void SetDirectDrawDisplayMode(unsigned int uWidth, unsigned int uHeight, unsigned int uBPP);
    void CreateFrontBuffer();
    void CreateBackBuffer();
    void CreateDirectDrawPrimarySurface();
    void CreateClipper(HWND a2);

    void PackPCXpicture(unsigned short* picture_data, int wight, int heidth, void *data_buff, int max_buff_size,unsigned int* packed_size);
    void SavePCXImage(const char *Filename, unsigned short* picture_data, int width, int height);

  //int windowed_mode_width;
  //int windowed_mode_height;
};
