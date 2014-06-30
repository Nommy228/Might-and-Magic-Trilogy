#pragma once

#include <cstdint>
#include <cstdio>

#include "lib\legacy_dx\d3d.h"
#include "OSWindow.h"

#include "VectorTypes.h"


#define ErrD3D(hr) \
  do \
  {  \
    extern void ErrHR(HRESULT, const char *, const char *, const char *, int); \
    ErrHR(hr, "Direct3D", __FUNCTION__, __FILE__, __LINE__); \
  } while(0)

struct Polygon;
struct Texture;
struct RGBTexture;
struct RenderBillboardTransform_local0;
struct ODMFace;


unsigned __int16 Color16(unsigned __int32 r, unsigned __int32 g, unsigned __int32 b);
unsigned __int32 Color32(unsigned __int16 color16);

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
  signed int diffuse;
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
  signed __int16 HwSpriteID;
  __int16 uPalette;
  __int16 uIndoorSectorID;
  __int16 field_1E;
  __int16 world_x;
  __int16 world_y;
  __int16 world_z;
  __int16 uScreenSpaceX;
  __int16 uScreenSpaceY;
  unsigned __int16 dimming_level;
  signed int sTintColor;
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
  inline RenderBillboardD3D():
    opacity(Transparent),
    field_90(-1),
    sParentBillboardID(-1),
    uNumVertices(4)
  {}

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
  OpacityType opacity;
  int field_90;
  int sZValue;
  signed int sParentBillboardID;
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
class RenderD3D
{
public:
  RenderD3D();
  
  void GetAvailableDevices(RenderD3D__DevInfo **pOutDevices);
  void Release();
  bool CreateDevice(unsigned int uDeviceID, int bWindowed, OSWindow *window);
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

  bool Initialize(OSWindow *window, bool bColoredLights, uint32_t uDetailLevel, bool bTinting);


  void ClearBlack();
  void PresentBlackScreen();
  void SavePCXScreenshot();
  void SaveWinnersCertificate(const char *a1);
  void ClearTarget(unsigned int uColor);
  void Present();
  void _49FD3A_fullscreen();
  void CreateZBuffer();
  void Release();
  bool InitializeFullscreen();
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
  void DrawBillboard_Indoor(RenderBillboardTransform_local0 *pSoftBillboard, Sprite *pSprite, int dimming_level);
  void DrawProjectile(float srcX, float srcY, float a3, float a4, float dstX, float dstY, float a7, float a8, IDirect3DTexture2 *a9);
  void _4A4CC9_AddSomeBillboard(struct stru6_stru1_indoor_sw_billboard *a1, int diffuse);
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
  void DrawOutdoorSkyD3D();
  //int DrawSkySW(struct Span *a1, Polygon *a2, int a3);
  void PrepareDecorationsRenderList_ODM();
  void DrawSpriteObjects_ODM();
  void TransformBillboardsAndSetPalettesODM();
  float DrawBezierTerrain();
  void RenderTerrainD3D();
  void DrawTerrainD3D(int a1, int edx0, int a3, int unk4);
  //void DrawTerrainSW(int a1, int a2, int a3, int a4);
  //void ExecOutdoorDrawSW();
  void ChangeBetweenWinFullscreenModes();
  void DrawBillboardList_BLV();
  bool AreRenderSurfacesOk();
  bool IsGammaSupported();
  void SaveScreenshot(const char *pFilename, unsigned int width, unsigned int height);
  void PackScreenshot(unsigned int width, unsigned int height, void *out_data, unsigned int data_size, unsigned int *screenshot_size);
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
  void DrawFansTransparent(const RenderVertexD3D3 *vertices, unsigned int num_vertices);
  void DrawSpecialEffectsQuad(const RenderVertexD3D3 *vertices, IDirect3DTexture2 *texture);

  void am_Blt_Copy(RECT *pSrcRect, POINT *pTargetXY, int a3);
  void am_Blt_Chroma(RECT *pSrcRect, POINT *pTargetPoint, int a3, int blend_mode);


  protected: unsigned int uDesiredDirect3DDevice;
  protected: int raster_clip_x;
  protected: int raster_clip_y;    // clipping rect for raster ops
  protected: int raster_clip_z;    //    like RasterLine2D for (mini)map
  protected: int raster_clip_w;
  public: int *pActiveZBuffer;
  protected: int *pDefaultZBuffer;
  protected: OSWindow *window;
  protected: unsigned int bWindowMode;
  protected: RenderD3D *pRenderD3D;
  public: IDirectDraw4 *pDirectDraw4;
  public: IDirectDrawSurface4 *pFrontBuffer4;
  public: IDirectDrawSurface4 *pBackBuffer4;
  protected: DDPIXELFORMAT ddpfPrimarySuface;
  protected: unsigned int uTargetRBits;
  protected: unsigned int uTargetGBits;
  protected: unsigned int uTargetBBits;
  protected: unsigned int uTargetRMask;
  protected: unsigned int uTargetGMask;
  protected: unsigned int uTargetBMask;
  protected: unsigned int uNumSceneBegins;
  protected: unsigned __int32 *pTargetSurface_unaligned;
  public: void        *pTargetSurface;
  public: unsigned int uTargetSurfacePitch;
  protected: unsigned int uClipY;
  protected: unsigned int uClipX;
  protected: unsigned int uClipW;
  protected: unsigned int uClipZ;
  protected: unsigned int bClip;
  protected: unsigned int uNumD3DSceneBegins;
  protected: int using_software_screen_buffer;
  protected: RenderHWLContainer pD3DBitmaps;
  protected: RenderHWLContainer pD3DSprites;
  public: unsigned int bUseColoredLights;
  protected: unsigned int bRequiredTextureStagesAvailable;
  public: unsigned int bTinting;
  protected: unsigned int uLevelOfDetail;
  protected: unsigned int uMaxDeviceTextureDim;
  protected: unsigned int uMinDeviceTextureDim;
  public: unsigned int bUsingSpecular;
  public: uint32_t uFogColor;
  public: unsigned int pHDWaterBitmapIDs[7];
  public: int hd_water_current_frame;
  public: int hd_water_tile_id;
  public: void (*pBeforePresentFunction)();
  public: uint32_t bFogEnabled;
  public: RenderBillboardD3D pBillboardRenderListD3D[1000];
  public: unsigned int uNumBillboardsToDraw;


  protected:
    void DoRenderBillboards_D3D();
    void SetBillboardBlendOptions(RenderBillboardD3D::OpacityType a1);
    void DrawBorderTiles(struct Polygon *poly);
    unsigned short *MakeScreenshot(signed int width, signed int height);
    bool CheckTextureStages();
    void ParseTargetPixelFormat();
    void TransformBillboard(RenderBillboardTransform_local0 *a2, Sprite *pSprite, int dimming_level, RenderBillboard *pBillboard);
    void CreateDirectDraw();
    void SetDirectDrawCooperationMode(HWND hWnd, bool bFullscreen);
    void SetDirectDrawDisplayMode(unsigned int uWidth, unsigned int uHeight, unsigned int uBPP);
    void CreateFrontBuffer();
    void CreateBackBuffer();
    void CreateDirectDrawPrimarySurface();
    void CreateClipper(HWND a2);
    void PackPCXpicture(unsigned short* picture_data, int wight, int heidth, void *data_buff, int max_buff_size,unsigned int* packed_size);
    void SavePCXImage(const char *Filename, unsigned short* picture_data, int width, int height);
    unsigned int Billboard_ProbablyAddToListAndSortByZOrder(float z);
    unsigned int GetParentBillboardID(unsigned int uBillboardID);
    unsigned int GetBillboardDrawListSize();

  public:
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

  //int windowed_mode_width;
  //int windowed_mode_height;
};
#pragma pack(pop)

bool PauseGameDrawing();

extern struct IDirectDrawClipper *pDDrawClipper;
extern struct Render *pRenderer; // idb
extern struct pUnkTerrain *Unks;



/*  248 */
#pragma pack(push, 1)
struct RenderBillboardTransform_local0
{
  void *pTarget;
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
  int sParentBillboardID;
  int sTintColor;
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


int ODM_NearClip(unsigned int uVertexID); // idb
int ODM_FarClip(unsigned int uNumVertices);