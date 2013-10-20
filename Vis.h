#pragma once
#include "Render.h"



enum VisObjectType: unsigned __int32
{
  VisObjectType_Any = 0,
  VisObjectType_Sprite = 1,
  VisObjectType_Face = 2
};

/*  150 */
#pragma pack(push, 1)
struct Vis_SelectionFilter //stru157
{
  VisObjectType object_type;
  int object_id;  // OBJECT_Actor, OBJECT_Player etc
  int at_ai_state;
  int no_at_ai_state;
  int select_flags;
};
#pragma pack(pop)
extern Vis_SelectionFilter vis_sprite_filter_1; // 00F93E1C
extern Vis_SelectionFilter vis_sprite_filter_2; // 00F93E30
extern Vis_SelectionFilter vis_face_filter;     // 00F93E44
extern Vis_SelectionFilter vis_door_filter;     // 00F93E58
extern Vis_SelectionFilter vis_sprite_filter_3; // 00F93E6C
extern Vis_SelectionFilter vis_sprite_filter_4; // static to sub_44EEA7



#pragma pack(push, 1)
struct Vis_ObjectInfo
{
  void *object;
  union
  {
    int sZValue;
    struct
    {
      unsigned __int16 object_pid;
      signed __int16 actual_z;
    };
  };
  VisObjectType object_type;
};
#pragma pack(pop)


#pragma pack(push, 1)
struct Vis_SelectionList
{
  enum PointerCreationType
  {
    All = 0,
    Unique = 1
  };

  Vis_SelectionList();
  //----- (004C0585) --------------------------------------------------------
  ~Vis_SelectionList() {}
  Vis_ObjectInfo *SelectionPointers(int a2, int a3);
  void            create_object_pointers(PointerCreationType type = All);

  inline void AddObject(void *object, VisObjectType type, int packed_zval)
  {
    object_pool[uNumPointers].object = object;
    object_pool[uNumPointers].object_type = type;
    object_pool[uNumPointers++].sZValue = packed_zval;
  }

  void ( ***vdestructor_ptr)(Vis_SelectionList *, bool);
  Vis_ObjectInfo  object_pool[512];
  Vis_ObjectInfo *object_pointers[512];
  unsigned int    uNumPointers;
};
#pragma pack(pop)



/*  116 */
#pragma pack(push, 1)
class Vis
{
public:
  Vis();
  //----- (004C05A2) --------------------------------------------------------
  //virtual ~Vis() {}
  //----- (004C05BE) --------------------------------------------------------
  virtual ~Vis() {}
  bool PickKeyboard(Vis_SelectionList *list, Vis_SelectionFilter *sprite_filter, Vis_SelectionFilter *face_filter);
  void PickBillboards_Keyboard(float pick_depth, Vis_SelectionList *list, Vis_SelectionFilter *filter);
  void PickIndoorFaces_Keyboard(float pick_depth, Vis_SelectionList *list, Vis_SelectionFilter *filter);
  void PickOutdoorFaces_Keyboard(float pick_depth, Vis_SelectionList *list, Vis_SelectionFilter *filter);

  bool PickMouse(float fDepth, float fMouseX, float fMouseY, Vis_SelectionFilter *sprite_filter, Vis_SelectionFilter *face_filter);
  void PickBillboards_Mouse(float fPickDepth, float fX, float fY, Vis_SelectionList *list, Vis_SelectionFilter *filter);
  void PickIndoorFaces_Mouse(float fDepth, struct RenderVertexSoft *pRay, Vis_SelectionList *list, Vis_SelectionFilter *filter);
  void PickOutdoorFaces_Mouse(float fDepth, struct RenderVertexSoft *pRay, Vis_SelectionList *list, Vis_SelectionFilter *filter, bool only_reachable);

  bool is_part_of_selection(void *uD3DBillboardIdx_or_pBLVFace_or_pODMFace, Vis_SelectionFilter *filter);
  bool DoesRayIntersectBillboard(float fDepth, unsigned int uD3DBillboardIdx);
  Vis_ObjectInfo *DetermineFacetIntersection(struct BLVFace *face, unsigned int a3, float pick_depth);
  bool IsPolygonOccludedByBillboard(struct RenderVertexSoft *vertices, int num_vertices, float x, float y);
  void GetPolygonCenter(struct RenderVertexD3D3 *pVertices, unsigned int uNumVertices, float *pCenterX, float *pCenterY);
  void GetPolygonScreenSpaceCenter(struct RenderVertexSoft *vertices, int num_vertices, float *out_center_x, float *out_center_y);
  bool IsPointInsideD3DBillboard(struct RenderBillboardD3D *a1, float x, float y);
  int PickClosestActor(int object_id, unsigned int pick_depth, int a4, int a5, int a6);
  void _4C1A02();
  bool SortVectors_x(RenderVertexSoft *a2, int a3, int a4);
  int get_object_zbuf_val(Vis_ObjectInfo *info);
  int get_picked_object_zbuf_val();
  bool Intersect_Ray_Face(struct RenderVertexSoft *pRayStart, struct RenderVertexSoft *pRayEnd, float *pDepth, RenderVertexSoft *Intersection, BLVFace *pFace, unsigned int pBModelID);
  bool CheckIntersectBModel(BLVFace *pFace, Vec3_short_ IntersectPoint, unsigned int uModelID);
  void BLV_CreateIntersectFacesVertexCoordList(int *a, int *b, __int16 *intersect_face_vertex_coords_list_a, __int16 *intersect_face_vertex_coords_list_b, Vec3_short_ *IntersectPoint, BLVFace *pFace);
  void ODM_CreateIntersectFacesVertexCoordList(int *a, int *b, __int16 *intersect_face_vertex_coords_list_a, __int16 *intersect_face_vertex_coords_list_b, Vec3_short_ *IntersectPoint, BLVFace *pFace, unsigned int uModelID);
  void CastPickRay(RenderVertexSoft *pRay, float fMouseX, float fMouseY, float fPickDepth);
  void sort_object_pointers(Vis_ObjectInfo **pPointers, int start, int end);
  bool SortVerticesByX(struct RenderVertexD3D3 *a2, unsigned int uStart, unsigned int uEnd);
  bool SortVerticesByY(struct RenderVertexD3D3 *a2, unsigned int uStart, unsigned int uEnd);
  bool SortByScreenSpaceX(struct RenderVertexSoft *pArray, int sLeft, int sRight);
  bool SortByScreenSpaceY(struct RenderVertexSoft *pArray, int sLeft, int sRight);

  //void ( ***vdestructor_ptr)(Vis *, bool);
  Vis_SelectionList default_list;
  RenderVertexSoft  stru_200C;
  RenderVertexSoft  stru_203C;
  RenderVertexSoft  stru_206C;
  RenderVertexSoft  stru_209C;
  int keyboard_pick_depth;
};
#pragma pack(pop)
