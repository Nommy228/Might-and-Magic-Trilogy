#pragma once

#include <cstdint>
#include <cstdio>

#include "lib\legacy_dx\d3d.h"
#include "OSWindow.h"

#include "VectorTypes.h"

struct Polygon;
struct Texture;
struct RGBTexture;
struct RenderBillboardTransform_local0;
struct ODMFace;


/*  119 */
#pragma pack(push, 1)
struct RenderVertexSoft
{
  inline RenderVertexSoft():
    flt_2C(0.0f)
  {}

  Vec3_float_ vWorldPosition;
  Vec3_float_ vWorldViewPosition;
  float vWorldViewProjX;
  float vWorldViewProjY;
  float _rhw;
  float u;
  float v;
  float flt_2C;
};
#pragma pack(pop)



/*  112 */
#pragma pack(push, 1)
struct RenderVertexD3D3
{
  Vec3_float_ pos;
  float rhw;
  unsigned int diffuse;
  unsigned int specular;
  Vec2_float_ texcoord;
};
#pragma pack(pop)

class Sprite;
class SpriteFrame;

/*  161 */
#pragma pack(push, 1)
struct RenderBillboard
{
  int _screenspace_x_scaler_packedfloat;
  int _screenspace_y_scaler_packedfloat;
  float fov_x;
  float fov_y;
  union
  {
    int sZValue;
    struct
    {
      unsigned __int16 object_pid;
      signed __int16 actual_z;
    };
  };
  int field_14_actor_id;
  unsigned __int16 uHwSpriteID;
  __int16 uPalette;
  __int16 uIndoorSectorID;
  __int16 field_1E;
  __int16 world_x;
  __int16 world_y;
  __int16 world_z;
  __int16 uScreenSpaceX;
  __int16 uScreenSpaceY;
  unsigned __int16 dimming_level;
  unsigned int uTintColor;
  SpriteFrame *pSpriteFrame;

   inline float GetFloatZ() const
 {
  return (float)object_pid / 65535.0f + (float)actual_z;
 }
};
#pragma pack(pop)


#pragma pack(push, 1)
struct RenderD3D__DevInfo
{
  unsigned int bIsDeviceCompatible;
  char *pName;
  char *pDescription;
  GUID *pGUID;
  unsigned int uCaps;
  char *pDriverName;
  char *pDeviceDesc;
  char *pDDraw4DevDesc;
  GUID *pDirectDrawGUID;
  int uVideoMem;
};
#pragma pack(pop)



#pragma pack(push, 1)
struct RenderD3D_D3DDevDesc
{
  int field_0;
  int field_4;
  int field_8;
  int field_C;
  int field_10;
  char *pDriverName;
  char *pDeviceDesc;
  char *pDDraw4DevDesc;
  GUID *pGUID;
  unsigned int uVideoMem;
};
#pragma pack(pop)









/*  280 */
#pragma pack(push, 1)
struct HWLTexture
{
  inline HWLTexture():
    field_0(0), field_4(0), field_8(0),
    field_C(0), field_10(0), field_14(0)
  {}

  int field_0;
  int field_4;
  int field_8;
  int field_C;
  int field_10;
  int field_14;
  int uBufferWidth;
  int uBufferHeight;
  int uAreaWidth;
  int uAreaHeigth;
  unsigned int uWidth;
  unsigned int uHeight;
  int uAreaX;
  int uAreaY;
  unsigned __int16 *pPixels;
};
#pragma pack(pop)


/*  185 */
#pragma pack(push, 1)
struct RenderHWLContainer
{
       RenderHWLContainer();
  bool Load(const wchar_t *pFilename);
  bool Release();

  HWLTexture *LoadTexture(const char *pName, int bMipMaps);

  FILE *pFile;
  uint32_t uSignature;
  unsigned int uDataOffset;
  unsigned int uNumItems;
  char *pSpriteNames[50000];
  int pSpriteOffsets[50000];
  int bDumpDebug;
  int scale_hwls_to_half;
};
#pragma pack(pop)



/*  242 */
#pragma pack(push, 1)
struct RenderBillboardD3D
{
  enum OpacityType: unsigned __int32
  {
    Transparent = 0,
    Opaque_1 = 1,
    Opaque_2 = 2,
    Opaque_3 = 3,
    NoBlend = 0xFFFFFFFF
  };

  IDirect3DTexture2 *pTexture;
  unsigned int uNumVertices;
  RenderVertexD3D3 pQuads[4];
  float z_order;
  OpacityType uOpacity;
  int field_90;
  int sZValue;
  unsigned int uParentBillboardID;
};
#pragma pack(pop)




#pragma pack(push, 1)
struct RenderD3D_aux
{
  RenderD3D__DevInfo *pInfo;
  RenderD3D_D3DDevDesc *ptr_4;
};
#pragma pack(pop)



/*  183 */
#pragma pack(push, 1)
struct RenderD3D
{
  RenderD3D();
  
  void GetAvailableDevices(RenderD3D__DevInfo **pOutDevices);
  void Release();
  bool CreateDevice(unsigned int uDeviceID, int bWindowed, HWND hWnd);
  unsigned int GetDeviceCaps();
  void ClearTarget(unsigned int bClearColor, unsigned int uClearColor, unsigned int bClearDepth, float z_clear);
  void Present(bool bForceBlit);
  bool CreateTexture(unsigned int uTextureWidth, unsigned int uTextureHeight, IDirectDrawSurface4 **pOutSurface, IDirect3DTexture2 **pOutTexture, bool bAlphaChannel, bool bMipmaps, unsigned int uMinDeviceTexDim);
  void HandleLostResources();


  unsigned int bWindowed;
  int field_4;
  int field_8;
  HWND hWindow;
  int field_10;
  int field_14;
  int field_18;
  RenderD3D__DevInfo *pAvailableDevices;
  IDirectDraw4 *pHost;
  IDirect3D3 *pDirect3D;
  IUnknown *pUnk;
  IDirectDrawSurface4 *pBackBuffer;
  IDirectDrawSurface4 *pFrontBuffer;
  IDirectDrawSurface4 *pZBuffer;
  IDirect3DDevice3 *pDevice;
  IDirect3DViewport3 *pViewport;
  int field_40;
  int field_44;
  char pErrorMessage[48];
  char field_78[208];
};
#pragma pack(pop)
extern struct RenderVertexD3D3  pVertices[50];

/*  182 */
#pragma pack(push, 1)
struct Render
{
  Render();
  ~Render();

  static Render *Create() {return new Render;}

  bool Initialize(bool bWindowed, OSWindow *window, bool bColoredLights, uint32_t uDetailLevel, bool bTinting);


  bool IsColorKeySupported(IDirectDraw4 *);
  void _49EBF1();
  void ClearBlack();
  void PresentBlackScreen();
  void SavePCXScreenshot();
  void _49F1BC(const char *a1);
  void PackPCXpicture(unsigned short* picture_data, int wight, int heidth, void *data_buff, int max_buff_size,unsigned int* packed_size);
  FILE *SavePCXImage(const char *Filename, char *a3, int a4, int a5);
  void ClearTarget(unsigned int uColor);
  void Release2();
  void Present();
  void _49FD3A();
  void CreateZBuffer();
  void Release();
  void CreateSomeTexture();
  bool InitializeFullscreen();
  bool SwitchToWindow();
  char RasterLine2D(unsigned int uX, unsigned int uY, unsigned int uZ, unsigned int uW, unsigned __int16 uColor);
  void ClearZBuffer(int a2, int a3);
  void SetRasterClipRect(unsigned int uX, unsigned int uY, unsigned int uZ, unsigned int uW);
  void ParseTargetPixelFormat();
  bool LockSurface_DDraw4(IDirectDrawSurface4 *pSurface, DDSURFACEDESC2 *pDesc, unsigned int uLockFlags);
  bool LockSurface_DDraw2(IDirectDrawSurface2 *pSurface, DDSURFACEDESC *pDesc, unsigned int uLockFlags);
  void CreateDirectDraw();
  void SetDirectDrawCooperationMode(HWND hWnd, bool bFullscreen);
  void SetDirectDrawDisplayMode(unsigned int uWidth, unsigned int uHeight, unsigned int uBPP);
  void CreateFrontBuffer();
  void CreateBackBuffer();
  void CreateDirectDrawPrimarySurface();
  void CreateClipper(HWND a2);
  void GetTargetPixelFormat(DDPIXELFORMAT *pOut);
  void LockRenderSurface(void **pOutSurfacePtr, unsigned int *pOutPixelsPerRow);
  void UnlockBackBuffer();
  void LockFrontBuffer(void **pOutSurface, unsigned int *pOutPixelsPerRow);
  void UnlockFrontBuffer();
  void RestoreFrontBuffer();
  HRESULT _4A184C();
  void PresentRect(RECT *a2, RECT *a3);
  void BltToFront(RECT *pDstRect, IDirectDrawSurface *pSrcSurface, RECT *pSrcRect, unsigned int uBltFlags);
  void BltBackToFontFast(int a2, int a3, RECT *a4);
  unsigned int Billboard_ProbablyAddToListAndSortByZOrder(float z);
  unsigned int GetBillboardDrawListSize();
  unsigned int GetParentBillboardID(unsigned int uBillboardID);
  void BeginSceneD3D();
  void DrawBillboards_And_MaybeRenderSpecialEffects_And_EndScene();
  unsigned int GetActorTintColor(float a2, int tint, int a4, int a5, RenderBillboard *a6);
  void DrawPolygon(unsigned int uNumVertices, struct Polygon *a3, ODMFace *a4, IDirect3DTexture2 *pTexture);
  void DrawTerrainPolygon(unsigned int uNumVertices, struct Polygon *a4, IDirect3DTexture2 *a5, bool transparent, bool clampAtTextureBorders);
  void DrawOutdoorSkyPolygon(unsigned int uNumVertices, struct Polygon *pSkyPolygon, IDirect3DTexture2 *pTexture);
  void DrawIndoorSkyPolygon(signed int uNumVertices, struct Polygon *pSkyPolygon, IDirect3DTexture2 *pTexture);
  void DrawIndoorPolygon(unsigned int uNumVertices, struct BLVFace *a3, IDirect3DTexture2 *pHwTex, struct Texture *pTex, int uPackedID, unsigned int uColor, int a8);
  void DrawIndoorSky(unsigned int uNumVertices, unsigned int uFaceID);
  void MakeParticleBillboardAndPush_BLV(RenderBillboardTransform_local0 *a2, IDirect3DTexture2 *a3, unsigned int uDiffuse, int angle);
  void MakeParticleBillboardAndPush_ODM(RenderBillboardTransform_local0 *a2, IDirect3DTexture2 *a3, unsigned int uDiffuse, int angle);
  void TransformBillboard(RenderBillboardTransform_local0 *a2, Sprite *pSprite, int dimming_level, RenderBillboard *pBillboard);
  void DrawBillboard_Indoor(RenderBillboardTransform_local0 *pSoftBillboard, Sprite *pSprite, int dimming_level);
  int MakeParticleBillboardAndPush_BLV_Software(int screenSpaceX, int screenSpaceY, int z, int lightColor, int a6);
  void DrawProjectile(float srcX, float srcY, float a3, float a4, float dstX, float dstY, float a7, float a8, IDirect3DTexture2 *a9);
  void _4A4CC9_AddSomeBillboard(struct stru6_stru1_indoor_sw_billboard *a1, int diffuse);
  bool LoadTexture(const char *pName, unsigned int bMipMaps, IDirectDrawSurface4 **pOutSurface, IDirect3DTexture2 **pOutTexture);
  bool MoveSpriteToDevice(Sprite *pSprite);
  void BeginScene();
  void EndScene();
  unsigned int _4A52F1(unsigned int this_, float a3);
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
  void GetLeather(unsigned int a2, unsigned int a3, struct Texture *a4, __int16 height);
  void DrawTextPalette(int x, int y, unsigned char* font_pixels, int a5, unsigned int uFontHeight, unsigned __int16 *pPalette, int a8);
  void DrawText(signed int uOutX, signed int uOutY, unsigned __int8 *pFontPixels, unsigned int uCharWidth, unsigned int uCharHeight, unsigned __int16 *pFontPalette, unsigned __int16 uFaceColor, unsigned __int16 uShadowColor);
  void FillRectFast(unsigned int uX, unsigned int uY, unsigned int uWidth, unsigned int uHeight, unsigned int uColor16);
  int _4A6DF5(unsigned __int16 *pBitmap, unsigned int uBitmapPitch, struct Vec2_int_ *pBitmapXY, unsigned __int16 *pTarget, unsigned int uTargetPitch, Vec4_int_ *a7);
  void _4A6E7E(unsigned int a2, unsigned int a3, struct Texture *a4);
  void DrawBuildingsD3D();
  //struct BSPModel *DrawBuildingsSW();
  //int OnOutdoorRedrawSW();
  void DrawOutdoorSkyD3D();
  //int DrawSkySW(struct Span *a1, Polygon *a2, int a3);
  void PrepareDecorationsRenderList_ODM();
  void DrawSpriteObjects_ODM();
  void TransformBillboardsAndSetPalettesODM();
  //float DrawBezierTerrain();
  void RenderTerrainD3D();
  void DrawTerrainD3D(int a1, int edx0, int a3, int unk4);
  //void DrawTerrainSW(int a1, int a2, int a3, int a4);
  //void ExecOutdoorDrawSW();
  void ChangeBetweenWinFullscreenModes();
  void DrawBillboardList_BLV();


  //unsigned int bUserDirect3D;
  unsigned int bStartInWindow;
  unsigned int uDesiredDirect3DDevice;
  unsigned int uAcquiredDirect3DDevice;
  int field_10;
  int field_14;
  int field_18_locked_pitch;
  int raster_clip_x;
  int raster_clip_y;    // clipping rect for raster ops
  int raster_clip_z;    //    like RasterLine2D for (mini)map
  int raster_clip_w;
  __int16 field_2C[65536];
  __int16 field_2002C[65536];
  int field_4002C;
  int field_40030;
  int *pActiveZBuffer;
  int *pDefaultZBuffer;
  int field_4003C;
  int field_40040;
  int field_40044;
  int field_40048;
  int field_4004C;
  //HWND hWnd;
  OSWindow *window;
  int field_40054;
  unsigned int bWindowMode;
  int field_4005C[11];
  RenderD3D *pRenderD3D;
  IDirectDraw4 *pDirectDraw4;
  IDirectDrawSurface4 *pFrontBuffer4;
  IDirectDrawSurface4 *pBackBuffer4;
  IDirectDrawSurface4 *pColorKeySurface4;
  IDirectDraw2 *pDirectDraw2;
  IDirectDrawSurface2 *pFrontBuffer2;
  IDirectDrawSurface2 *pBackBuffer2;
  IDirectDrawSurface2 *pSomeSurface2;
  DDPIXELFORMAT ddpfPrimareSuface;
  unsigned int uTargetRBits;
  unsigned int uTargetGBits;
  unsigned int uTargetBBits;
  unsigned int uTargetRMask;
  unsigned int uTargetGMask;
  unsigned int uTargetBMask;
  unsigned int uNumSceneBegins;
  int *ptr_400E8;
  unsigned __int16 *pTargetSurface;
  unsigned int uTargetSurfacePitch;
  unsigned int uClipY;
  unsigned int uClipX;
  unsigned int uClipW;
  unsigned int uClipZ;
  unsigned int bClip;
  unsigned int bColorKeySupported;
  unsigned int uNumD3DSceneBegins;
  int field_40110;
  RenderHWLContainer pD3DBitmaps;
  RenderHWLContainer pD3DSprites;
  unsigned int bUseColoredLights;
  unsigned int bRequiredTextureStagesAvailable;
  unsigned int bTinting;
  unsigned int uLevelOfDetail;
  unsigned int uMaxDeviceTextureDim;
  unsigned int uMinDeviceTextureDim;
  int field_10365C;
  unsigned int bUsingSpecular;
  uint32_t uFogColor;
  int field_103668;
  unsigned int pHDWaterBitmapIDs[7];
  char field_103688[32];
  int hd_water_current_frame;
  int hd_water_tile_id;
  IDirectDrawSurface4 *pSurface;
  IDirect3DTexture2 *pTexture;
  int field_1036B8;
  int _gpu_memory_used;
  void ( *pBeforePresentFunction)();
  int field_1036C4;
  uint32_t bFogEnabled;
  int field_1036CC;
  RenderBillboardD3D pBillboardRenderListD3D[1000];
  unsigned int uNumBillboardsToDraw;
  int field_129834;
  unsigned int uCurrentlyLockedSurfacePitch;
  unsigned __int16 *pCurrentlyLockedSurfaceDataPtr;
  unsigned __int16 *pCurrentlyLockedSoftSurface;


  //int windowed_mode_width;
  //int windowed_mode_height;
};
#pragma pack(pop)

extern struct IDirectDrawClipper *pDDrawClipper;
extern struct Render *pRenderer; // idb
extern struct pUnkTerrain *Unks;



/*  248 */
#pragma pack(push, 1)
struct RenderBillboardTransform_local0
{
  unsigned __int16 *pTarget;
  int *pTargetZ;
  int uScreenSpaceX;
  int uScreenSpaceY;
  int _screenspace_x_scaler_packedfloat;
  int _screenspace_y_scaler_packedfloat;
  char field_18[8];
  unsigned __int16 *pPalette;
  unsigned __int16 *pPalette2;
  union
  {
    int sZValue;
    struct
    {
      unsigned short object_pid;
      short          zbuffer_depth;
    };
  };
  unsigned int uFlags;        // & 4   - mirror horizontally
  unsigned int uTargetPitch;
  unsigned int uViewportX;
  unsigned int uViewportY;
  unsigned int uViewportZ;
  unsigned int uViewportW;
  int field_44;
  int uParentBillboardID;
  int uTintColor;
};
#pragma pack(pop)




extern int uNumDecorationsDrawnThisFrame; // weak
extern RenderBillboard pBillboardRenderList[500];
extern unsigned int uNumBillboardsToDraw;
extern int uNumSpritesDrawnThisFrame; // weak




extern RenderVertexSoft array_507D30[50];
extern RenderVertexSoft array_50AC10[50];
extern RenderVertexSoft array_73D150[20];

extern RenderVertexD3D3 d3d_vertex_buffer[50];