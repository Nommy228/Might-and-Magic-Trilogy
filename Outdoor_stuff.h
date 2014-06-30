#pragma once
#include "VectorTypes.h"
#include <array>

struct ODMFace;


/*  141 */
// stru148
#pragma pack(push, 1)
struct Polygon 
{
  inline Polygon()
  {
    uNumVertices = 0;
  }

  int _479295();
  //void _48276F_sr();
  void _normalize_v_18();
  void Create_48607B(struct stru149 *a2);

  float field_0;
  float field_4;
  float field_8;
  float field_C;
  float field_10;
  unsigned int uNumVertices;
  Vec3_int_ v_18;
  int field_24;
  int sTextureDeltaU;
  int sTextureDeltaV;
  __int16 flags;
  __int16 field_32;
  int field_34;
  struct stru149 *ptr_38;
  struct Texture *pTexture;
  struct Span *_unused_prolly_head;
  struct Span *_unused_prolly_tail;
  int **ptr_48;
  unsigned __int16 uTileBitmapID;
  __int16 field_4E;
  __int16 field_50;
  __int16 field_52;
  ODMFace *pODMFace;
  char dimming_level;
  char field_59;
  char field_5A;
  char field_5B;
  char terrain_grid_z;
  char terrain_grid_x;
  unsigned __int8 uBModelID;
  unsigned __int8 uBModelFaceID;
  struct Edge *pEdgeList1[20];
  unsigned int uEdgeList1Size;
  struct Edge *pEdgeList2[20];
  unsigned int uEdgeList2Size;
  char field_108;
  char field_109;
  char field_10A;
  char field_10B;
};
#pragma pack(pop)
extern std::array<struct Polygon, 2000 + 18000> array_77EC08;

/*  142 */
#pragma pack(push, 1)
struct stru149
{
  void _48616B_frustum_odm(int a2, int a3, int a4, int a5, int a6, int a7);
  void _48653D_frustum_blv(int a2, int a3, int a4, int a5, int a6, int a7);
  void _48694B_frustum_sky();

  int field_0_party_dir_x;
  int field_4_party_dir_y;
  int field_8_party_dir_z;
  int angle_from_north;//field_C
  int angle_from_west;//field_10
  int viewing_angle_from_west_east;
  int angle_from_east;//field_18
  int angle_from_south;//field_1C
  int viewing_angle_from_north_south;//field_20
  int field_24;
  int field_28;
};
#pragma pack(pop)
extern stru149 stru_8019C8;


/*   88 */
#pragma pack(push, 1)
struct ODMRenderParams
{
  //----- (00462684) --------------------------------------------------------
  ODMRenderParams()
  {
    uPickDepth = 0;
    this->shading_dist_shade = 2048;
    shading_dist_shademist = 4096;
    shading_dist_mist = 8192;
    int_fov_rad = 0;
    this->bNoSky = 0;
    this->bDoNotRenderDecorations = 0;
    this->field_5C = 0;
    this->field_60 = 0;
    this->outdoor_no_wavy_water = 0;
    this->outdoor_no_mist = 0;
  }

  void Initialize();

  int uPickDepth;
  int shading_dist_shade;
  int shading_dist_shademist;
  int shading_dist_mist;
  unsigned int uCameraFovInDegrees;
  int int_fov_rad;                          // 157 struct IndoorCamera::fov_rad
  int int_fov_rad_inv;                      // 157 struct IndoorCamera::fov_rad_inv
  int _unused_camera_rotation_y_int_sine;   // merged with BLVRenderParams equivalents
  int _unused_camera_rotation_y_int_cosine; // into IndoorCameraD3D
  int _unused_camera_rotation_x_int_sine;   // --//--
  int _unused_camera_rotation_x_int_cosine; // --//--
  int uNumPolygons;
  unsigned int _unused_uNumEdges;
  unsigned int _unused_uNumSurfs;
  unsigned int _unused_uNumSpans;
  unsigned int uNumBillboards;
  float field_40;
  int field_44;
  int outdoor_grid_band_3;
  int field_4C;
  int field_50;
  unsigned int bNoSky;
  unsigned int bDoNotRenderDecorations;
  int field_5C;
  int field_60;
  int outdoor_no_wavy_water;
  int outdoor_no_mist;
  int building_gamme;
  int terrain_gamma;

  unsigned int uMapGridCellX; // moved from 157 struct IndoorCamera::0C
  unsigned int uMapGridCellZ; // moved from 157 struct IndoorCamera::10
};
#pragma pack(pop)
extern ODMRenderParams *pODMRenderParams;



void ODM_Project(unsigned int uNumVertices);