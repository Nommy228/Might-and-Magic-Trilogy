#pragma once

#include "Indoor_stuff.h"


#pragma pack(push, 1)
struct LocationTime_stru1
{
  unsigned __int64 uLastVisitDay;
  char sky_texture_name[12];
  int day_attrib;
  int day_fogrange_1;
  int day_fogrange_2;
  char field_2F4[24];
};
#pragma pack(pop)

/*  319 */
enum LEVEL_TYPE
{
  LEVEL_null = 0,
  LEVEL_Indoor = 0x1,
  LEVEL_Outdoor = 0x2,
};
extern LEVEL_TYPE uCurrentlyLoadedLevelType;













/*   86 */
#pragma pack(push, 1)
struct SpawnPointMM6
{
  Vec3_int_ vPosition;
  unsigned __int16 uRadius;
  unsigned __int16 uKind;
  unsigned __int16 uIndex;
  unsigned __int16 uAttributes;
};
#pragma pack(pop)



/*  102 */
#pragma pack(push, 1)
struct SpawnPointMM7
{
  //----- (00448DD6) --------------------------------------------------------
  SpawnPointMM7()
  {
    uRadius = 32;
    uAttributes = 0;
    uIndex = 0;
    uKind = 0;
    uGroup = 0;
  }


  Vec3_int_ vPosition;
  unsigned __int16 uRadius;
  unsigned __int16 uKind;
  unsigned __int16 uIndex;
  unsigned __int16 uAttributes;
  unsigned int uGroup;
};
#pragma pack(pop)










#pragma pack(push, 1)
struct DDM_DLV_Header
{
  //----- (00462607) --------------------------------------------------------
  inline DDM_DLV_Header()
  {
    this->uLastRepawnDay = 0;
    this->uNumRespawns = 0;
    this->uReputation = 0;
    this->field_C_alert = 0;
    this->uNumFacesInBModels = 0;
    this->uNumDecorations = 0;
    this->uNumBModels = 0;
  }


  int uNumRespawns;
  int uLastRepawnDay;
  int uReputation;
  int field_C_alert;
  unsigned int uNumFacesInBModels;
  unsigned int uNumDecorations;
  unsigned int uNumBModels;
  int field_1C;
  int field_20;
  int field_24;
};
#pragma pack(pop)




/*   90 */
#pragma pack(push, 1)
struct BLVHeader
{
  char field_0[104];
  unsigned int uFaces_fdata_Size;
  unsigned int uSector_rdata_Size;
  unsigned int uSector_lrdata_Size;
  unsigned int uDoors_ddata_Size;
  char field_78[16];
};
#pragma pack(pop)



/*   96 */
#pragma pack(push, 1)
struct BLVSectorMM8
{
  int dword_000074;
};
#pragma pack(pop)

/*   97 */
#pragma pack(push, 1)
struct BLVLightMM6
{
  Vec3_short_ vPosition;
  __int16 uRadius;
  __int16 uAttributes;
  unsigned __int16 uBrightness;
};
#pragma pack(pop)

/*   98 */
#pragma pack(push, 1)
struct BLVLightMM7 //10h
{
  struct Vec3_short_ vPosition;
  __int16 uRadius;
  char uRed;
  char uGreen;
  char uBlue;
  char uType;
  __int16 uAtributes;           // & 0x08    doesn't light faces
  __int16 uBrightness;
};
#pragma pack(pop)

/*   99 */
#pragma pack(push, 1)
struct BLVLightMM8
{
  int uID;
};
#pragma pack(pop)



/*  100 */
#pragma pack(push, 1)
struct BLVDoor //50h
{
  enum State: unsigned __int16
  {
    Closed = 0,
    Opening = 1,
    Open = 2,
    Closing = 3
  };

  unsigned int uAttributes;
  unsigned int uDoorID;
  unsigned int uTimeSinceTriggered;
  Vec3_int_ vDirection;
  int uMoveLength;
  int uOpenSpeed;
  int uCloseSpeed;
  unsigned __int16 *pVertexIDs;
  unsigned __int16 *pFaceIDs;
  unsigned __int16 *pSectorIDs;
  __int16 *pDeltaUs;
  __int16 *pDeltaVs;
  unsigned __int16 *pXOffsets;
  unsigned __int16 *pYOffsets;
  unsigned __int16 *pZOffsets;
  unsigned __int16 uNumVertices;
  unsigned __int16 uNumFaces;
  __int16 field_48;
  unsigned __int16 uNumOffsets;
  State uState;
  __int16 field_4E;
};
#pragma pack(pop)

/*  101 */
#pragma pack(push, 1)
struct BLVMapOutline //0C
{
  unsigned __int16 uVertex1ID;
  unsigned __int16 uVertex2ID;
  unsigned __int16 uFace1ID;
  unsigned __int16 uFace2ID;
  __int16 sZ;
  unsigned __int16 uFlags;
};
#pragma pack(pop)


#define FACE_PORTAL             0x00000001 // portal/two-sided
#define FACE_CAN_SATURATE_COLOR 0x00000002
#define FACE_UNKNOW8            0x00000004
#define FACE_FLUID              0x00000010 // wavy animated water or lava
#define FACE_UNKNOW9            0x00000020
#define FACE_DONT_CACHE_TEXTURE 0x00000040 // do not load face texture if it isn't loaded already
#define FACE_UNKNOW4            0x00000080
#define FACE_XY_PLANE           0x00000100
#define FACE_XZ_PLANE           0x00000200
#define FACE_YZ_PLANE           0x00000400
#define FACE_UNKNOW10           0x00000800
#define FACE_UNKNOW3            0x00001000
#define FACE_INVISIBLE          0x00002000
#define FACE_TEXTURE_FRAME      0x00004000 // Texture ID is a frameset from TextureFrameTable, otherwise BitmapID
#define FACE_OUTLINED           0x00010000 // outline face edges
#define FACE_UNKNOW7            0x00020000
#define FACE_TEXTURE_FLOW       0x00040000 // The texture moves slowly. For horizontal facets only.
#define FACE_UNKNOW             0x00100000
#define FACE_UNKNOW6            0x00200000
#define FACE_INDOOR_SKY         0x00400000
#define FACE_CLICKABLE          0x02000000 // Event can be triggered by clicking on the facet.
#define FACE_PRESSURE_PLATE     0x04000000 // Event can be triggered by stepping on the facet.
#define FACE_INDICATE           0x06000000 // Event can be triggered by indicating on the facet.
#define FACE_UNKNOW2            0x10000000
#define FACE_ETHEREAL           0x20000000 // Untouchable. You can pass through it.
#define FACE_UNKNOW5            0x40000000
#define FACE_PICKED             0x80000000

/*   93 */
#pragma pack(push, 1)
struct BLVFace  //60h
{
  //----- (0046ED02) --------------------------------------------------------
  inline BLVFace()
  {
    this->uNumVertices = 0;
    this->uAttributes = 0;
    this->uFaceExtraID = 0;
    this->pVertexIDs = nullptr;
    this->pZInterceptDisplacements = nullptr;
    this->pYInterceptDisplacements = nullptr;
    this->pXInterceptDisplacements = nullptr;
  }

  void _get_normals(Vec3_int_ *a2, Vec3_int_ *a3);
  struct Texture *GetTexture();
  void FromODM(struct ODMFace *face);

  inline bool Invisible() const {return (uAttributes & FACE_INVISIBLE) != 0;}
  inline bool Visible() const   {return !Invisible();}
  inline bool Portal() const    {return (uAttributes & FACE_PORTAL) != 0;}
  inline bool Fluid() const     {return (uAttributes & FACE_FLUID) != 0;}
  inline bool Indoor_sky() const     {return (uAttributes & FACE_INDOOR_SKY) != 0;}
  inline bool Clickable() const {return (uAttributes & FACE_CLICKABLE) != 0;}
  inline bool Pressure_Plate() const {return (uAttributes & FACE_PRESSURE_PLATE) != 0;}
  inline bool Ethereal() const {return (uAttributes & FACE_ETHEREAL) != 0;}

  struct Plane_float_ pFacePlane;
  struct Plane_int_ pFacePlane_old;
  int zCalc1;
  int zCalc2;
  int zCalc3;
  unsigned int uAttributes;
  unsigned __int16 *pVertexIDs;
  signed __int16 *pXInterceptDisplacements;
  signed __int16 *pYInterceptDisplacements;
  signed __int16 *pZInterceptDisplacements;
  signed __int16   *pVertexUIDs;
  signed __int16   *pVertexVIDs;
  unsigned __int16  uFaceExtraID;
  unsigned __int16  uBitmapID;
  unsigned __int16  uSectorID;
  __int16 uBackSectorID;
  struct BBox_short_ pBounding;
  PolygonType uPolygonType;
  unsigned __int8 uNumVertices;
  char field_5E;
  char field_5F;
};
#pragma pack(pop)

/*   94 */
#pragma pack(push, 1)
struct BLVFaceExtra  //24h
{
bool HasEventint();

  __int16 field_0;
  __int16 field_2;
  __int16 field_4;
  __int16 field_6;
  __int16 field_8;
  __int16 field_A;
  __int16 field_C;
  unsigned __int16 uAdditionalBitmapID;
  __int16 field_10;
  __int16 field_12;
  __int16 sTextureDeltaU;
  __int16 sTextureDeltaV;
  __int16 sCogNumber;
  unsigned __int16 uEventID;
  __int16 field_1C;
  __int16 field_1E;
  __int16 field_20;
  __int16 field_22;
};
#pragma pack(pop)

/*   95 */
#pragma pack(push, 1)
struct BLVSector //0x74
{
  int field_0;
  unsigned __int16 uNumFloors;
  __int16 field_6;
  unsigned __int16 *pFloors;
  unsigned __int16 uNumWalls;
  __int16 field_E;
  unsigned __int16 *pWalls;
  unsigned __int16 uNumCeilings;
  __int16 field_16;
  unsigned __int16 *pCeilings;
  unsigned __int16 uNumFluids;
  __int16 field_1E;
  unsigned __int16 *pFluids;
  __int16 uNumPortals;
  __int16 field_26;
  unsigned __int16 *pPortals;
  unsigned __int16 uNumFaces;
  unsigned __int16 uNumNonBSPFaces;
  unsigned __int16 *pFaceIDs;
  unsigned __int16 uNumCylinderFaces;
  __int16 field_36;
  int pCylinderFaces;
  unsigned __int16 uNumCogs;
  __int16 field_3E;
  unsigned __int16 *pCogs;
  unsigned __int16 uNumDecorations;
  __int16 field_46;
  unsigned __int16 *pDecorationIDs;
  unsigned __int16 uNumMarkers;
  __int16 field_4E;
  unsigned __int16 *pMarkers;
  unsigned __int16 uNumLights;
  __int16 field_56;
  unsigned __int16 *pLights;
  __int16 uWaterLevel;
  __int16 uMistLevel;
  __int16 uLightDistanceMultiplier;
  __int16 uMinAmbientLightLevel;
  __int16 uFirstBSPNode;
  __int16 exit_tag;
  BBox_short_ pBounding;
};
#pragma pack(pop)


#pragma pack(push, 1)
struct BLVMapOutlines
{
  int          uNumOutlines;
  BLVMapOutline pOutlines[7000];
};
#pragma pack(pop)







/*   89 */
#pragma pack(push, 1)
struct IndoorLocation
{
  //----- (00462592) --------------------------------------------------------
  inline IndoorLocation()
  {
    bLoaded = 0;
    ptr_0002B8_sector_lrdata = 0;
    ptr_0002B4_doors_ddata = 0;
    ptr_0002B0_sector_rdata = 0;
    pLFaces = 0;
    pVertices = 0;
    pFaces = 0;
    pFaceExtras = 0;
    pSectors = 0;
    pLights = 0;
    pDoors = 0;
    pNodes = 0;
    pMapOutlines = 0;
    uNumSpawnPoints = 0;
    pSpawnPoints = 0;
  }

  int GetSector(int sX, int sY, int sZ);
  void Release();
  bool Alloc();
  bool Load(char *pFilename, int a3, size_t i, char *pDest);
  void Draw();
  void ToggleLight(signed int uLightID, unsigned int bToggle);

  static unsigned int GetLocationIndex(const char *Str1);
  static void ExecDraw(bool bD3D);
  //static void ExecDraw_sw(unsigned int uFaceID);
  static void ExecDraw_d3d(unsigned int uFaceID, struct IndoorCameraD3D_Vec4 *pVertices, unsigned int uNumVertices, struct RenderVertexSoft *pPortalBounding);

  char pFilename[32];
  char field_20[48];
  unsigned int bLoaded;
  char field_54[404];
  struct BLVHeader blv;
  unsigned int uNumVertices;
  struct Vec3_short_ *pVertices;
  unsigned int uNumFaces;
  struct BLVFace *pFaces;
  unsigned int uNumFaceExtras;
  struct BLVFaceExtra *pFaceExtras;
  int uNumSectors;
  struct BLVSector *pSectors;
  int uNumLights;
  struct BLVLightMM7 *pLights;
  int uNumDoors;
  struct BLVDoor *pDoors;
  unsigned int uNumNodes;
  struct BSPNode *pNodes;
  BLVMapOutlines *pMapOutlines;
  unsigned __int16 *pLFaces;
  unsigned __int16 *ptr_0002B0_sector_rdata;
  unsigned __int16 *ptr_0002B4_doors_ddata;
  unsigned __int16 *ptr_0002B8_sector_lrdata;
  unsigned int uNumSpawnPoints;
  struct SpawnPointMM7 *pSpawnPoints;
  struct DDM_DLV_Header dlv;
  LocationTime_stru1 stru1;
  char _visible_outlines[875];
  char padding;
};
#pragma pack(pop)

extern IndoorLocation *pIndoor;













/*  162 */
#pragma pack(push, 1)
struct BLVRenderParams
{
  inline BLVRenderParams()
  {
    uViewportX = 0;
    uViewportY = 0;
    uViewportZ = 0;
    uViewportW = 0;
  }

  void Reset();

  int field_0_timer_;
  int _unused_uFlags;              // & INDOOR_CAMERA_DRAW_D3D_OUTLINES:  render d3d outlines
  Vec3_int_ _unused_vPartyPos;
  int _unused_sPartyRotY;
  int _unused_sPartyRotX;
  int uPartySectorID;
  int _unused_sCosineY;      // matches ODMRenderParams::int sines and cosines
  int _unused_sSineY;        // computed in 0048600E
  int _unused_sCosineNegX;   // merged into IndoorCameraD3D
  int _unused_sSineNegX;     // --//--
  float _unused_fCosineY;    // matches old IndoorCamera::fRotationCosineY (new IndoorCameraD3D::fRotationCosineY)
  float _unused_fSineY;      // matches old IndoorCamera::fRotationSineY   (new IndoorCameraD3D::fRotationSineY)
  float _unused_fCosineNegX; // the same
  float _unused_fSineNegX;   // the same
  int fov_rad_fixpoint;
  int fov_rad_inv_fixpoint;//float
  void *pRenderTarget;
  unsigned int uTargetWidth;
  unsigned int uTargetHeight;
  unsigned int uViewportX;
  unsigned int uViewportY;
  unsigned int uViewportZ;
  unsigned int uViewportW;
  int field_64;
  int *pTargetZBuffer;
  int uViewportHeight;
  int uViewportWidth;
  int uViewportCenterX;
  int uViewportCenterY;
  struct BspRenderer_PortalViewportData *field_7C;
  unsigned int uNumFacesRenderedThisFrame;
  int field_84;
  int field_88;
  int field_8C;
  int field_90;
  int field_94;
};
#pragma pack(pop)
extern BLVRenderParams *pBLVRenderParams;





int __fastcall GetPortalScreenCoord(unsigned int uFaceID);
bool PortalFrustrum(int pNumVertices, struct BspRenderer_PortalViewportData *a2, struct BspRenderer_PortalViewportData *near_portal, int uFaceID);
void PrepareBspRenderList_BLV();
void PrepareDecorationsRenderList_BLV(unsigned int uDecorationID, unsigned int uSectorID);
void PrepareActorRenderList_BLV();
void PrepareItemsRenderList_BLV();
void AddBspNodeToRenderList(unsigned int node_id);
void __fastcall sub_4406BC(unsigned int node_id, unsigned int uFirstNode); // idb
char __fastcall DoInteractionWithTopmostZObject(int a1, int a2);
int __fastcall sub_4AAEA6_transform(struct RenderVertexSoft *a1);
unsigned int __fastcall sub_4B0E07(unsigned int uFaceID); // idb
void BLV_UpdateUserInputAndOther();
int BLV_GetFloorLevel(int x, int y, int z, unsigned int uSectorID, unsigned int *pFaceID);
void BLV_UpdateDoors();
void UpdateActors_BLV();
void BLV_ProcessPartyActions();
void Door_switch_animation(unsigned int uDoorID, int a2); // idb: sub_449A49
int __fastcall sub_4088E9(int a1, int a2, int a3, int a4, int a5, int a6);
void __fastcall PrepareDrawLists_BLV();
void PrepareToLoadBLV(unsigned int bLoading);
int GetAlertStatus();
int __fastcall _45063B_spawn_some_monster(struct MapInfo *a1, int a2);
int __fastcall sub_450521_ProllyDropItemAt(int ecx0, signed int a2, int a3, int a4, int a5, unsigned __int16 a6);
