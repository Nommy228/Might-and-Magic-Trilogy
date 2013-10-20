#pragma once
#include <vector>
#include "Render.h"

struct LightsStack_StationaryLight_;
struct LightsStack_MobileLight_;

/*  115 */
#pragma pack(push, 1)
struct Lightmap
{
  Lightmap();
  virtual ~Lightmap() {}

  //void ( ***vdestructor_ptr)(Lightmap *, bool);
  unsigned int uNumVertices;
  RenderVertexSoft pVertices[64];
  __int16 field_C08;
  __int16 field_C0A;
  __int16 field_C0C;
  __int16 field_C0E;
  unsigned int uColorMask;
  float fBrightness;
  int field_C18;
};
#pragma pack(pop)


/*  114 */
#pragma pack(push, 1)
class LightmapBuilder
{
public:
  LightmapBuilder();
  virtual ~LightmapBuilder() //----- (0045BBAA)
  {}

  void Draw_183808_Lightmaps();
  bool DrawLightmap(Lightmap *a1, Vec3_float_ *pColorMult, float z_bias);
  bool DoDraw_183808_Lightmaps(float a2);
  bool DrawLightmaps(int *indices = nullptr);
  void DrawLightmapsType(int type);
  bool DrawDebugOutlines(char bit_one_for_list1__bit_two_for_list2);
  double _45D643_sw(struct Edge *a1, float a2);
  //int _45D426_sw(struct Span *a1, struct Edge **a2, unsigned int a3, struct Edge *a4, int a5);
  //bool _45D3C7_sw(struct Polygon *a1);
  bool StackLight_TerrainFace(struct StationaryLight *pLight, struct Vec3_float_ *pNormal, float *a3, struct RenderVertexSoft *a1, unsigned int uStripType, int X, unsigned int *pSlot);
  bool StackLights_TerrainFace(struct Vec3_float_ *pNormal, float *a3, struct RenderVertexSoft *a1, unsigned int uStripType, bool bLightBackfaces);
  bool ApplyLight_ODM(struct StationaryLight *pLight, struct ODMFace *pFace, unsigned int *pSlot, char a4);
  bool ApplyLights_OutdoorFace(struct ODMFace *pFace);
  double _45CC0C_light(struct Vec3_float_ a1, float a2, float a3, struct Vec3_float_ *pNormal, float a5, int uLightType);
  int _45CBD4(struct RenderVertexSoft *a2, int a3, int *a4, int *a5);
  int _45CB89(struct RenderVertexSoft *a1, int a2);
  int _45CA88(struct stru320 *a2, struct RenderVertexSoft *a3, int a4, struct Vec3_float_ *pNormal);
  bool ApplyLight_BLV(struct StationaryLight *pLight, struct BLVFace *a2, unsigned int *pSlot, bool bLightBackfaces, char *a5);
  bool ApplyLights_IndoorFace(unsigned int uFaceID);
  int _45C6D6(int a2, struct RenderVertexSoft *a3, Lightmap *pLightmap);
  int _45C4B9(int a2, struct RenderVertexSoft *a3, Lightmap *pLightmap);
  bool _45BE86_build_light_polygon(Vec3_int_ *pos, float radius, unsigned int uColorMask, float dot_dist, int uLightType, struct stru314 *a7, unsigned int uNumVertices, RenderVertexSoft *a9, char uClipFlag);
  bool ApplyLights(struct stru320 *a2, struct stru154 *a3, unsigned int uNumVertices, struct RenderVertexSoft *a5, struct IndoorCameraD3D_Vec4 *, char uClipFlag);


  //void ( ***vdestructor_ptr)(LightmapBuilder *, bool);
  //std::vector<Lightmap> std__vector_000004;
  //std::vector<Lightmap> std__vector_183808;
  Lightmap std__vector_000004[512];
  unsigned int std__vector_000004_size;
  Lightmap std__vector_183808[768];
  unsigned int std__vector_183808_size;
  float flt_3C8C0C;
  float flt_3C8C10;
  float flt_3C8C14;
  float flt_3C8C18;
  float flt_3C8C1C;
  float flt_3C8C20;
  float flt_3C8C24;
  float flt_3C8C28;
  float flt_3C8C2C_lightmaps_brightness;
  float flt_3C8C30;
  RenderVertexSoft field_3C8C34[256];
  int uFlags;
};
#pragma pack(pop)




extern LightsStack_StationaryLight_ *pStationaryLightsStack;
//extern StationaryLight pStationaryLights[400];
//extern int uNumStationaryLightsApplied; // weak
extern LightsStack_MobileLight_ *pMobileLightsStack;
//extern MobileLight pMobileLights[400];
//extern int uNumMobileLightsApplied;