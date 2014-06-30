#pragma once

#include <array>

#include "Render.h"
#include "IndoorCameraD3D.h"



/*  165 */
#pragma pack(push, 1)
struct BspRenderer_PortalViewportData
{
  void GetViewportData(__int16 x, int y, __int16 z, int w);

  int _viewport_space_y;
  int _viewport_space_w;
  int _viewport_space_x;
  int _viewport_space_z;
  int _viewport_x_minID;
  int _viewport_z_maxID;
  __int16 viewport_left_side[480];
  __int16 viewport_right_side[480];
};
#pragma pack(pop)
extern BspRenderer_PortalViewportData stru_F8A590;




/*  164 */
#pragma pack(push, 1)
struct BspRenderer_stru0
{
  //----- (0043F2BF) --------------------------------------------------------
  inline BspRenderer_stru0()
  {
    //_eh_vector_constructor_iterator_(std__vector_0007AC, 24, 4,
    //    (void ( *)(void *))IndoorCameraD3D_Vec4::IndoorCameraD3D_Vec4,
    //    (void ( *)(void *))IndoorCameraD3D_Vec4::dtor);
    //for (int i = 0; i < 4; ++i)
    //  pVertices[i].flt_2C = 0.0f;
  }

  //----- (0043F2A9) --------------------------------------------------------
  ~BspRenderer_stru0()
  {
    //_eh_vector_destructor_iterator_(this->std__vector_0007AC, 24, 4, IndoorCameraD3D_Vec4::dtor);
  }

  unsigned __int16 uSectorID;
  unsigned __int16 uViewportX;
  unsigned __int16 uViewportY;
  unsigned __int16 uViewportZ;
  unsigned __int16 uViewportW;
  __int16 field_A;
  BspRenderer_PortalViewportData PortalScreenData;
  unsigned __int16 uFaceID;
  __int16 field_7A6;
  unsigned int         viewing_portal_id;             // portal through which we're seeing this node
  IndoorCameraD3D_Vec4 std__vector_0007AC[4];
  RenderVertexSoft     pPortalBounding[4];
};
#pragma pack(pop)




#pragma pack(push, 1)
struct BspFace
{
  unsigned __int16 uFaceID;
  unsigned __int16 uNodeID;
};
#pragma pack(pop)


/*  163 */
#pragma pack(push, 1)
struct BspRenderer // stru170
{
  //----- (0043F282) --------------------------------------------------------
  inline BspRenderer()
  {
   // _eh_vector_constructor_iterator_(field_FA8, 2252, 150,
   //    (void ( *)(void *))stru170_stru0::stru170_stru0,
   //    (void ( *)(void *))stru170_stru0::dtor);
    num_faces = 0;
    num_nodes = 0;
    uNumVisibleNotEmptySectors = 0;
  }

  //void AddFaceToRenderList_sw(unsigned int node_id, unsigned int uFaceID);
  void AddFaceToRenderList_d3d(unsigned int node_id, unsigned int uFaceID);
  void MakeVisibleSectorList();
  //void DrawFaceOutlines();

  unsigned int num_faces;
  //__int16 pFaceIDs[2000];
  BspFace faces[1000];
  //char field_130[3700];
  unsigned int num_nodes;
  BspRenderer_stru0 nodes[150];
  unsigned int uNumVisibleNotEmptySectors;
  unsigned __int16 pVisibleSectorIDs_toDrawDecorsActorsEtcFrom[6];
};
#pragma pack(pop)


extern struct BspRenderer *pBspRenderer; // idb












/*  358 */
#pragma pack(push, 1)
struct stru320
{
  int field_0;
  Plane_int_ plane_4;
  Vec3_int_ vec_14;
  Vec3_int_ vec_20;
  unsigned int uCurrentAmbientLightLevel;
  int field_30;
  int field_34;
  int field_38;
  int pDeltaUV[2];
  int field_44;
  int field_48;
  char field_4C[8];
  Vec3_int_ rotated_normal;
  Vec3_int_ vec_60;
  int field_6C;
  Vec3_int_ vec_70;
  int field_7C;
  Vec3_int_ vec_80;
  Vec3_int_ vec_8C;
  int field_98;
  Vec3_int_ vec_9C;
  int field_A8;
  unsigned int uNumLightsApplied;
  int _blv_lights_radii[20];
  int _blv_lights_inv_radii[20];
  int _blv_lights_xs[20];
  int _blv_lights_ys[20];
  int _blv_lights_light_dot_faces[20];
  int field_240;
  int field_244;
  int field_248;
  int field_24C;
  int field_250;
  int field_254;
  int field_258;
  int field_25C;
  int field_260;
  int field_264;
  int field_268;
  int field_26C;
  int field_270;
  int field_274;
  int field_278;
  int field_27C;
  int field_280;
  int field_284;
  int field_288;
  int field_28C;
  int _blv_lights_zs[20];
  float _blv_lights_rs[20];
  float _blv_lights_gs[20];
  float _blv_lights_bs[20];
  char _blv_lights_types[20];
  int field_3E4;
  int field_3E8;
  int field_3EC;
  int field_3F0;
  int field_3F4;
  unsigned int uDefaultAmbientLightLevel;
};
#pragma pack(pop)

extern stru320 stru_F8AD28; // idb








/*  345 */
#pragma pack(push, 1)
struct stru315
{
  int field_0;
  int field_4;
  int field_8;
  int field_C;
  int field_10;
  int field_14;
  int field_18;
  int field_1C;
  int field_20;
  int field_24;
  int field_28;
  int field_2C;
  int field_30;
  unsigned __int16 *field_34_palette;
  unsigned __int16 *pTextureLOD;
  unsigned int *pDepthBuffer;
  unsigned __int16 *pColorBuffer;
};
#pragma pack(pop)





/*  346 */
#pragma pack(push, 1)
struct stru316
{
  int field_0;
  int field_4;
  int field_8;
  int field_C;
  int field_10;
  int field_14;
  int field_18;
  int field_1C;
  int field_20;
  unsigned __int16 *field_24_palette;
};
#pragma pack(pop)






/*  134 */
#pragma pack(push, 1)
struct stru141_actor_collision_object
{
  int _47050A(int a2);

  int field_0;
  int prolly_normal_d;
  int field_8_radius;
  int height;
  int field_10;
  int field_14;
  int field_18;
  Vec3_int_ velocity;
  Vec3_int_ normal;
  Vec3_int_ position;
  Vec3_int_ normal2;
  int field_4C;
  int field_50;
  int field_54;
  Vec3_int_ direction; // velocity's unit vector
  int speed;
  int inv_speed;
  int field_6C;
  int field_70;
  unsigned int uSectorID;
  unsigned int uFaceID;
  int field_7C;
  int field_80;
  int field_84;
  int field_88;
  int sMaxX;
  int sMinX;
  int sMaxY;
  int sMinY;
  int sMaxZ;
  int sMinZ;
  int field_A4;
};
#pragma pack(pop)
extern stru141_actor_collision_object stru_721530;






/*  378 */
#pragma pack(push, 1)
struct stru337_stru0
{
  int field_0;
  int field_4;
  int field_8;
  int field_C;
  int field_10;
};
#pragma pack(pop)



/*  377 */
#pragma pack(push, 1)
struct stru337
{
  stru337_stru0 field_0;
  int field_14;
  int field_18;
  int field_1C;
  int field_20;
  int field_24;
  int field_28;
  int field_2C;
  int field_30;
  stru337_stru0 field_34;
};
#pragma pack(pop)
extern stru337 stru_F81018;


/*  291 */
enum PolygonType : __int8
{
  POLYGON_Invalid = 0x0,
  POLYGON_VerticalWall = 0x1,
  POLYGON_unk = 0x2,
  POLYGON_Floor = 0x3,
  POLYGON_InBetweenFloorAndWall = 0x4,
  POLYGON_Ceiling = 0x5,
  POLYGON_InBetweenCeilingAndWall = 0x6,
};


/*  147 */
#pragma pack(push, 1)

struct stru154
{
  //----- (0049B001) --------------------------------------------------------
  inline stru154()
  {}
  
  //----- (0049B027) --------------------------------------------------------
  inline ~stru154()
  {}

  void GetFacePlaneAndClassify(struct ODMFace *a2, struct BSPVertexBuffer *a3);
  void ClassifyPolygon(struct Vec3_float_ *pNormal, float dist);
  void GetFacePlane(struct ODMFace *pFace, struct BSPVertexBuffer *pVertices, struct Vec3_float_ *pOutNormal, float *pOutDist);


  void ( ***vdestructor_ptr)(stru154 *, bool);
  Plane_float_ face_plane;
  PolygonType polygonType;
  char field_15;
  char field_16;
  char field_17;
};
#pragma pack(pop)





/*  392 */
#pragma pack(push, 1)
struct stru352
{
  int field_0;
  int field_4;
  int field_8;
  int field_C;
  int field_10;
  int field_14;
  int field_18;
  int field_1C;
  int field_20;
  int field_24;
  int field_28;
  int field_2C;
  int field_30;
  int field_34;
};
#pragma pack(pop)
extern std::array<stru352, 480> stru_F83B80;