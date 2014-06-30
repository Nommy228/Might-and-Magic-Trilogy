#pragma once
#include "VectorTypes.h"



/*  124 */
#pragma pack(push, 1)
struct IndoorCameraD3D_Vec3
{
  //----- (004C0376) --------------------------------------------------------
  inline IndoorCameraD3D_Vec3() {}
  //----- (004C037F) --------------------------------------------------------
  virtual ~IndoorCameraD3D_Vec3() {}
  //----- (004C039C) --------------------------------------------------------
  //void ~IndoorCameraD3D_Vec3() {}

  //void ( ***vdestructor_ptr)(IndoorCameraD3D_Vec3 *, bool);

  union
  {
    struct
    {
      float x;
      float y;
      float z;
    };
    float v[3];
  };
};
#pragma pack(pop)

/*  125 */
#pragma pack(push, 1)
struct IndoorCameraD3D_Vec4: public IndoorCameraD3D_Vec3
{
  //----- (00498038) --------------------------------------------------------
  inline IndoorCameraD3D_Vec4():
    IndoorCameraD3D_Vec3()
  {}

  //----- (00498069) --------------------------------------------------------
  virtual ~IndoorCameraD3D_Vec4()
  {}

  float dot;
  int _wtf; // sizeof vec4 is 18 and first member is vdtor, but vdtor is already included in vec3 so very weird
};
#pragma pack(pop)



/*  199 */
#pragma pack(push, 1)
struct IndoorCameraD3D_stru3
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
  float flt_30;
  int field_34;
};
#pragma pack(pop)



/*  197 */
#pragma pack(push, 1)
struct IndoorCameraD3D_stru1
{
  //----- (004363A2) --------------------------------------------------------
  IndoorCameraD3D_stru1()
  {
    this->flt_2C = 0.0;
  }


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
  float flt_2C;
};
#pragma pack(pop)

/*  198 */
#pragma pack(push, 1)
struct IndoorCameraD3D_stru2
{
  unsigned int mm7__vector_000004_size;
  IndoorCameraD3D_stru1 mm7__vector_000004[64];
  int field_C04;
  int field_C08;
  int field_C0C;
};
#pragma pack(pop)



#define BLV_RENDER_DRAW_SW_OUTLINES      (1 << 0) // 1
#define BLV_RENDER_DRAW_D3D_OUTLINES     (1 << 1) // 2
#define ODM_RENDER_DRAW_D3D_OUTLINES     (1 << 2) // 4
#define ODM_RENDER_DRAW_TERRAIN_OUTLINES (1 << 3) // 8

/*  123 */
#pragma pack(push, 1)
struct IndoorCameraD3D
{
  IndoorCameraD3D();

  //----- (004363C6) --------------------------------------------------------
  virtual ~IndoorCameraD3D()
  {
    //this->vdestructor_ptr = &stru8_pvdtor;
    //_eh_vector_destructor_iterator_(this->std__vector_000034_prolly_frustrum, 24, 6, IndoorCameraD3D_Vec4::dtor);
    //IndoorCameraD3D_Vec3::dtor(&v1->field_24);
    //IndoorCameraD3D_Vec3::dtor(&v1->field_14);
    //IndoorCameraD3D_Vec3::dtor(&v1->field_4);
  }

  double GetPolygonMaxZ(struct RenderVertexSoft *pVertex, unsigned int uStripType);
  double GetPolygonMinZ(struct RenderVertexSoft *pVertices, unsigned int uStripType);
  struct IDirect3DTexture2 *LoadTextureAndGetHardwarePtr(char *Str1);
  void Project(signed int x, signed int y, signed int z, int *a5, int *a6);
  void Project(struct RenderVertexSoft *pVertices, unsigned int uNumVertices, char a4);
  void _436CDC_mess_with_lightmap__clipflag_2(struct RenderVertexSoft *pInVertices, int uNumInVertices, struct RenderVertexSoft *pOutVertices, unsigned int *pOutNumVertices);
  void _436F09_mess_with_lightmap__clipflag_4(struct RenderVertexSoft *pInVertices, int uNumInVertices, struct RenderVertexSoft *pOutVertices, unsigned int *pOutNumVertices);
  void _437143(unsigned int uNumInVertices, struct RenderVertexSoft *pOutVertices, struct RenderVertexSoft *pInVertices, unsigned int *pOutNumVertices);
  bool _4371C3(struct RenderVertexSoft *pVertices, unsigned int *pOutNumVertices, int _unused);
  bool CalcPortalShape(struct RenderVertexSoft *a1, unsigned int *pOutNumVertices, struct RenderVertexSoft *pVertices, IndoorCameraD3D_Vec4 *a4, signed int uNumVertices, char a6, int _unused);
  char _437376(struct stru154 *thisa, struct RenderVertexSoft *a2, unsigned int *pOutNumVertices);
  void _4374E8_ProllyBuildFrustrum();
  void _437607(IndoorCameraD3D_Vec3 *a1, IndoorCameraD3D_Vec4 *a2);
  void Vec3Transform(const IndoorCameraD3D_Vec3 *pVector, IndoorCameraD3D_Vec3 *pOut);
  void CreateWorldMatrixAndSomeStuff();
  void MatrixMultiply(struct Matrix3x3_float_ *a1, struct Matrix3x3_float_ *a2, struct Matrix3x3_float_ *a3);
  void PrepareAndDrawDebugOutline(struct BLVFace *pFace, unsigned int uDiffuse);
  void debug_outline_sw(struct RenderVertexSoft *a2, unsigned int uNumVertices, unsigned int uDiffuse, float a5);
  void debug_outline_d3d(const struct RenderVertexD3D3 *pLineVertices, unsigned int uNumLines, int uDiffuse, float z_stuff);
  void do_draw_debug_line_sw(struct RenderVertexSoft *pLineBegin, signed int sStartDiffuse, struct RenderVertexSoft *pLineEnd, signed int sEndDiffuse, unsigned int uOutNumVertices, float z_stuff);
  //void sr_437D4A_draw_some_vertices(float x, float y, float z, unsigned int a5, char a6, float a7);
  //void sr_438141_draw_list_0037C();
  //void sr_438240_draw_lits();
  //void sr_Reset_list_0037C();
  bool is_face_faced_to_camera(struct BLVFace *pFace, struct RenderVertexSoft *a2);
  bool GetFacetOrientation(char polyType, struct Vec3_float_ *a2, struct Vec3_float_ *a3, struct Vec3_float_ *a4);
  void ViewTransform(struct RenderVertexSoft *a1a, unsigned int uNumVertices);
  bool IsCulled(struct BLVFace *pFace);
  void ViewTransfrom_OffsetUV(struct RenderVertexSoft *pVertices, unsigned int uNumVertices, struct RenderVertexSoft *pOutVertices, struct stru320 *a5);
  bool ApplyViewTransform_TrueIfStillVisible_BLV(int x, int y, int z, signed int *pOutX, int *pOutZ, int *pOutY, char bDoNotShow);
  float GetPickDepth();
  float GetShadingDistMist();

  void DebugDrawPortal(struct BLVFace *pFace);

  //void ( ***vdestructor_ptr)(IndoorCameraD3D *, bool);
  IndoorCameraD3D_Vec3 field_4[3];
  //IndoorCameraD3D_Vec3 field_14;
  //IndoorCameraD3D_Vec3 field_24;
  IndoorCameraD3D_Vec4 std__vector_000034_prolly_frustrum[6];
  float fov;
  float screenCenterX;
  float screenCenterY;
  float fov_x;
  float fov_y;
  float inv_fov;
  float _unused_blv_party_x;
  float _unused_blv_party_y;
  float _unused_blv_party_z;
  char field_E8[32];
  float field_108;
  float _unused_blv_party_x_2;
  float _unused_blv_party_y_2;
  float _unused_blv_party_z_2;
  char field_118[32];
  float field_138;
  char field_13C[44];
  float field_168;
  char field_16C[44];
  float field_198;
  char field_19C[44];
  float field_1C8;
  char field_1CC[44];
  float field_1F8;
  char field_1FC[44];
  float field_228;
  char field_22C[44];
  float field_258;
  char field_25C[44];
  float field_288;
  char field_28C[44];
  float field_2B8;
  float field_2BC;
  float field_2C0;
  float field_2C4;
  char field_2C8[32];
  float field_2E8;
  float field_2EC;
  float field_2F0;
  float field_2F4;
  char field_2F8[32];
  float field_318;
  float field_31C;
  float field_320;
  float field_324;
  char field_328[32];
  float field_348;
  float field_34C;
  float field_350;
  float field_354;
  char field_358[32];
  float field_378;
  IndoorCameraD3D_stru3 list_0037C[16384];
  unsigned int list_0037C_size;
  IndoorCameraD3D_stru2 list_E0380[256];
  int list_E0380_size;


  void CalculateRotations(int camera_rot_x, int camera_rot_y);
  int       sRotationY;        // moved  from 157 struct IndoorCamera::18
  int       sRotationX;        // moved  from 157 struct IndoorCamera::14
  float     fRotationYSine;    // moved  from 157 struct IndoorCamera::2C
  float     fRotationYCosine;  // moved  from 157 struct IndoorCamera::30
  float     fRotationXSine;    // moved  from 157 struct IndoorCamera::34
  float     fRotationXCosine;  // moved  from 157 struct IndoorCamera::38
  Vec3<int> vPartyPos;         // moved  from 157 struct IndoorCamera::00
                               // merged from 162 struct BLVRenderParams::08
  int        debug_flags;      // moved  from 157 struct IndoorCamera::4C
                               // merged from 162 struct BLVRenderParams::04
  int        int_sine_y;       // moved  from 157 struct ODMRenderParams::1C
                               // merged from 162 struct BLVRenderParams::24
  int        int_cosine_y;     // moved  from 157 struct ODMRenderParams::20
                               // merged from 162 struct BLVRenderParams::20
  int        int_sine_x;       // moved  from 157 struct ODMRenderParams::24
                               // merged from 162 struct BLVRenderParams::2C
  int        int_cosine_x;     // moved  from 157 struct ODMRenderParams::28
                               // merged from 162 struct BLVRenderParams::28
};
#pragma pack(pop)