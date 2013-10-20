#include "Vis.h"
#include "Outdoor.h"
#include "BSPModel.h"
#include "Game.h"
#include "Actor.h"
#include "Outdoor_stuff.h"
#include "Viewport.h"
#include "Math.h"
#include "Log.h"
#include "ErrorHandling.h"

#include "mm7_data.h"
#include "MM7.h"
#include "Level/Decoration.h"


static Vis_SelectionList Vis_static_sub_4C1944_stru_F8BDE8;

Vis_SelectionFilter vis_sprite_filter_1 = {VisObjectType_Sprite, OBJECT_Decoration,  0, 0,         2}; // 00F93E1C
Vis_SelectionFilter vis_sprite_filter_2 = {VisObjectType_Sprite, OBJECT_Decoration,  0, 0,         2}; // 00F93E30
Vis_SelectionFilter vis_face_filter     = {VisObjectType_Face,   OBJECT_Any,        -1, 0,         0}; // 00F93E44
Vis_SelectionFilter vis_door_filter     = {VisObjectType_Face,   OBJECT_BLVDoor,    -1, 0x100000,  0}; // 00F93E58
Vis_SelectionFilter vis_sprite_filter_3 = {VisObjectType_Sprite, OBJECT_Decoration, -1, 0,         4}; // 00F93E6C
Vis_SelectionFilter vis_sprite_filter_4 = {VisObjectType_Any,    OBJECT_Item,       -1, 0,         0}; // static to sub_44EEA7



//----- (004C1026) --------------------------------------------------------
Vis_ObjectInfo *Vis::DetermineFacetIntersection(BLVFace *face, unsigned int pid, float pick_depth)
{
  char *v4; // eax@4
  signed int v5; // ecx@4
  RenderVertexSoft pRay[2]; // [sp+20h] [bp-70h]@17
  int v20; // [sp+84h] [bp-Ch]@10

  static Vis_SelectionList SelectedPointersList;//stru_F8FE00
  SelectedPointersList.uNumPointers = 0;

  static bool _init_flag = false;
  static RenderVertexSoft static_DetermineFacetIntersection_array_F8F200[64];
  if (!_init_flag)
  {
    _init_flag = true;
    for (uint i = 0; i < 64; ++i)
      static_DetermineFacetIntersection_array_F8F200[i].flt_2C = 0.0f;
  }

  if (uCurrentlyLoadedLevelType == LEVEL_Indoor)
  {
    if ( (signed int)face->uNumVertices > 0 )
    {
      for ( int i = 0; i < face->uNumVertices; i++)
      {
        static_DetermineFacetIntersection_array_F8F200[i].vWorldPosition.x = (double)pIndoor->pVertices[face->pVertexIDs[i]].x;
        static_DetermineFacetIntersection_array_F8F200[i].vWorldPosition.y = (double)pIndoor->pVertices[face->pVertexIDs[i]].y;
        static_DetermineFacetIntersection_array_F8F200[i].vWorldPosition.z = (double)pIndoor->pVertices[face->pVertexIDs[i]].z;
      }
    }
  }
  else if (uCurrentlyLoadedLevelType == LEVEL_Outdoor)
  {
    uint bmodel_id = pid >> 9;
    auto v = (Vec3_int_ *)pOutdoor->pBModels[bmodel_id].pVertices.pVertices;
    for (uint i = 0; i < face->uNumVertices; ++i)
    {
      static_DetermineFacetIntersection_array_F8F200[i].vWorldPosition.x = v[face->pVertexIDs[i]].x;
      static_DetermineFacetIntersection_array_F8F200[i].vWorldPosition.y = v[face->pVertexIDs[i]].y;
      static_DetermineFacetIntersection_array_F8F200[i].vWorldPosition.z = v[face->pVertexIDs[i]].z;
    }
  }
  else assert(false);

  pGame->pIndoorCameraD3D->ViewTransform(static_DetermineFacetIntersection_array_F8F200, face->uNumVertices);
  pGame->pIndoorCameraD3D->Project(static_DetermineFacetIntersection_array_F8F200, face->uNumVertices, 1);

  SortVectors_x(static_DetermineFacetIntersection_array_F8F200, 0, face->uNumVertices - 1);
  if (static_DetermineFacetIntersection_array_F8F200[0].vWorldViewPosition.x > pick_depth)
    return nullptr;

  float screenspace_center_x,
        screenspace_center_y;
  GetPolygonScreenSpaceCenter(static_DetermineFacetIntersection_array_F8F200, face->uNumVertices, &screenspace_center_x, &screenspace_center_y);
  if (IsPolygonOccludedByBillboard(static_DetermineFacetIntersection_array_F8F200, face->uNumVertices, screenspace_center_x, screenspace_center_y))
    return nullptr;

  CastPickRay(pRay, screenspace_center_x, screenspace_center_y, pick_depth);

  if (uCurrentlyLoadedLevelType == LEVEL_Outdoor)
    PickOutdoorFaces_Mouse(pick_depth, pRay, &SelectedPointersList, &vis_face_filter, true);
  else if (uCurrentlyLoadedLevelType == LEVEL_Indoor)
    PickIndoorFaces_Mouse(pick_depth, pRay, &SelectedPointersList, &vis_face_filter);
  else assert(false);

  SelectedPointersList.create_object_pointers();
  sort_object_pointers(SelectedPointersList.object_pointers, 0, SelectedPointersList.uNumPointers - 1);
  if (!SelectedPointersList.uNumPointers)
    return nullptr;

  if (!SelectedPointersList.SelectionPointers(VisObjectType_Face, pid))
    return nullptr;

  if (SelectedPointersList.uNumPointers)
    return SelectedPointersList.object_pointers[0];
  else return nullptr;
}
// F91E08: using guessed type char static_DetermineFacetIntersection_byte_F91E08__init_flags;

//----- (004C12C3) --------------------------------------------------------
bool Vis::IsPolygonOccludedByBillboard(RenderVertexSoft *vertices, int num_vertices, float x, float y)
{
  //signed int v5; // esi@1
  //RenderBillboardD3D *v6; // edi@2
  //double v7; // st7@9
  //int v8; // edx@9
  //RenderVertexSoft *v9; // ecx@10
  //char result; // al@24
  //Vis *thisa; // [sp+10h] [bp-8h]@1
  //float thisb; // [sp+10h] [bp-8h]@9
  //signed int v13; // [sp+14h] [bp-4h]@1
  //float a3a; // [sp+28h] [bp+10h]@9
  //float a4a; // [sp+2Ch] [bp+14h]@9

  int v13 = -1;
  //v5 = 0;
  //thisa = this;

  //v6 = pRenderer->pBillboardRenderListD3D;
  for (uint i = 0; i < pRenderer->uNumBillboardsToDraw; ++i)
  {
    auto billboard = pRenderer->pBillboardRenderListD3D + i;
    if (IsPointInsideD3DBillboard(billboard, x, y))
    {
      if (v13 == -1)
        v13 = i;
      else  if ((unsigned int)pBillboardRenderList[billboard->uParentBillboardID].sZValue < 
                  pBillboardRenderList[pRenderer->pBillboardRenderListD3D[v13].uParentBillboardID].sZValue)
        v13 = i;
    }
  }

  if ( v13 == -1 )
    return false;
  
  //v8 = num_vertices;
  //v7 = 3.4028235e38;
  float min_x = FLT_MAX;
  //a4a = 3.4028235e38;
  float min_y = FLT_MAX;
  //a3a = -3.4028235e38;
  float max_x = -FLT_MAX;
  //thisb = -3.4028235e38;
  float max_y = -FLT_MAX;
  for (uint i = 0; i < num_vertices; ++i)
  {
    //v9 = a1;
    //do
    //{
    auto v = vertices + i;

    if (v->vWorldViewProjX < min_x)
      min_x = v->vWorldViewProjX;
    if (v->vWorldViewProjX > max_x)
      max_x = v->vWorldViewProjX;

    if (v->vWorldViewProjY < min_y)
      min_y = v->vWorldViewProjY;
    if (v->vWorldViewProjY > max_y)
      max_y = v->vWorldViewProjY;
      //++v9;
      //--v8;
    //}
    //while ( v8 );
  }

  if (min_x < pRenderer->pBillboardRenderListD3D[v13].pQuads[0].pos.x || pRenderer->pBillboardRenderListD3D[v13].pQuads[0].pos.y > min_y ||
      pRenderer->pBillboardRenderListD3D[v13].pQuads[3].pos.x < max_x || pRenderer->pBillboardRenderListD3D[v13].pQuads[1].pos.y < max_y)
    return false;

  return true;
}

//----- (004C1417) --------------------------------------------------------
void Vis::GetPolygonCenter(RenderVertexD3D3 *pVertices, unsigned int uNumVertices, float *pCenterX, float *pCenterY)
{
  static RenderVertexD3D3 unk_F8EA00[64];

  memcpy(unk_F8EA00, pVertices, 32 * uNumVertices);

  SortVerticesByX(unk_F8EA00, 0, uNumVertices - 1);
  *pCenterX = (unk_F8EA00[uNumVertices - 1].pos.x - unk_F8EA00[0].pos.x) * 0.5 + unk_F8EA00[0].pos.x;

  SortVerticesByY(unk_F8EA00, 0, uNumVertices - 1);
  *pCenterY = (unk_F8EA00[uNumVertices - 1].pos.y - unk_F8EA00[0].pos.y) * 0.5 + unk_F8EA00[0].pos.y;
}

//----- (004C1495) --------------------------------------------------------
void Vis::GetPolygonScreenSpaceCenter(RenderVertexSoft *vertices, int num_vertices, float *out_center_x, float *out_center_y)
{
  char *v5; // eax@2
  signed int v6; // ecx@2
  float *result; // eax@5

  static RenderVertexSoft static_sub_4C1495_array_F8DDF8[64];

  memcpy(static_sub_4C1495_array_F8DDF8, vertices, 48 * num_vertices);

  SortByScreenSpaceX(static_sub_4C1495_array_F8DDF8, 0, num_vertices - 1);
  *out_center_x = (static_sub_4C1495_array_F8DDF8[num_vertices - 1].vWorldViewProjX - static_sub_4C1495_array_F8DDF8[0].vWorldViewProjX) * 0.5 + static_sub_4C1495_array_F8DDF8[0].vWorldViewProjX;

  SortByScreenSpaceY(static_sub_4C1495_array_F8DDF8, 0, num_vertices - 1);
  *out_center_y = (static_sub_4C1495_array_F8DDF8[num_vertices - 1].vWorldViewProjY - static_sub_4C1495_array_F8DDF8[0].vWorldViewProjY) * 0.5 + static_sub_4C1495_array_F8DDF8[0].vWorldViewProjY;
}

//----- (004C1542) --------------------------------------------------------
void Vis::PickBillboards_Mouse(float fPickDepth, float fX, float fY, Vis_SelectionList *list, Vis_SelectionFilter *filter)
{
  for (uint i = 0; i < pRenderer->uNumBillboardsToDraw; ++i)
  {
    auto d3d_billboard = &pRenderer->pBillboardRenderListD3D[i];
    if (is_part_of_selection((void *)i, filter) && IsPointInsideD3DBillboard(d3d_billboard, fX, fY))
    {
      if (DoesRayIntersectBillboard(fPickDepth, i))
      {
        auto billboard = &pBillboardRenderList[d3d_billboard->uParentBillboardID];

        list->AddObject((void *)d3d_billboard->uParentBillboardID, VisObjectType_Sprite, billboard->sZValue);
      }
    }
  }
}

//----- (004C1607) --------------------------------------------------------
bool Vis::IsPointInsideD3DBillboard(RenderBillboardD3D *a1, float x, float y)
{
  //RenderBillboardD3D *result; // eax@1
  double v5; // st7@2
  float v6; // ecx@2
  float v7; // ST00_4@3
  __int16 v8; // fps@6
  double v9; // st6@6
  char v10; // c0@6
  char v11; // c2@6
  char v12; // c3@6
  __int16 v13; // fps@7
  double v14; // st6@7
  unsigned __int8 v15; // c0@7
  char v16; // c2@7
  unsigned __int8 v17; // c3@7
  __int16 v18; // fps@8
  double v19; // st6@8
  char v20; // c0@8
  char v21; // c2@8
  char v22; // c3@8
  __int16 v23; // fps@9
  double v24; // st6@9
  unsigned __int8 v25; // c0@9
  char v26; // c2@9
  unsigned __int8 v27; // c3@9
  float v28; // [sp+4h] [bp-8h]@2
  float v29; // [sp+8h] [bp-4h]@2
  float a1a; // [sp+14h] [bp+8h]@2

  if ( a1->uParentBillboardID == -1 )
    return false;

  //result = a1;
  v5 = a1->pQuads[0].pos.x;
  a1a = a1->pQuads[3].pos.x;
  v6 = a1->pQuads[0].pos.y;
  //result = (RenderBillboardD3D *)LODWORD(result->pQuads[1].pos.y);
  v29 = v6;
  v28 = a1->pQuads[1].pos.y;
  if ( v5 > a1a )
  {
    v7 = v5;
    //HIWORD(result) = HIWORD(v7);
    v5 = a1a;
    a1a = v7;
  }
  if ( v6 > (double)v28 )
  {
    //result = (RenderBillboardD3D *)LODWORD(v28);
    v28 = v6;
    v29 = v28;
  }
  v9 = x + 1.0;
  //UNDEF(v8);
  //v10 = v9 < v5;
  //v11 = 0;
  //v12 = v9 == v5;
  //BYTE1(result) = HIBYTE(v8);//crash
  if (v9 >= v5 &&
      (v14 = x - 1.0, v14<=a1a) &&///*UNDEF(v13),*/ v15 = v14 < a1a, v16 = 0, v17 = v14 == a1a, BYTE1(result) = HIBYTE(v13), v15 | v17)
      (v19 = y + 1.0, v19>=v29) &&///*UNDEF(v18),*/ v20 = v19 < v29, v21 = 0, v22 = v19 == v29, BYTE1(result) = HIBYTE(v18), v19 >= v29)
      (v24 = y - 1.0, v24<=v28))///*UNDEF(v23),*/ v25 = v24 < v28, v26 = 0, v27 = v24 == v28, BYTE1(result) = HIBYTE(v23), v25 | v27) )
    return true;
  else
    return false;
}

//----- (004C16B4) --------------------------------------------------------
void Vis::PickIndoorFaces_Mouse(float fDepth, RenderVertexSoft *pRay, Vis_SelectionList *list, Vis_SelectionFilter *filter)
{
  int v5; // eax@1
  signed int pFaceID; // edi@2
  int v8; // ecx@7
  int v9; // eax@7
  unsigned int *pNumPointers; // eax@7
  unsigned int v11; // ecx@7
  Vis_ObjectInfo *v12; // edi@7
  RenderVertexSoft a1; // [sp+Ch] [bp-44h]@1
  BLVFace *pFace; // [sp+3Ch] [bp-14h]@7
  void *v15; // [sp+40h] [bp-10h]@7
  int v17; // [sp+48h] [bp-8h]@1

  v5 = 0;
  v17 = 0;
  for ( a1.flt_2C = 0.0; v17 < (signed int)pBspRenderer->num_faces; ++v17 )
  {
    pFaceID = pBspRenderer->faces[v5].uFaceID;
    if ( pFaceID >= 0 )
    {
      if ( pFaceID < (signed int)pIndoor->uNumFaces )
      {
        auto face = pIndoor->pFaces + pFaceID;
        if ( is_part_of_selection(face, filter) )
        {
          if ( !pGame->pIndoorCameraD3D->IsCulled(face) )
          {
            if ( Intersect_Ray_Face(pRay, pRay + 1, &fDepth, &a1, face, 0xFFFFFFFFu) )
            {
              pGame->pIndoorCameraD3D->ViewTransform(&a1, 1);
              v9 = fixpoint_from_float(/*v8, */a1.vWorldViewPosition.x);
              LOWORD(v9) = 0;
              v15 = (void *)((PID(OBJECT_BModel,pFaceID)) + v9);
              pNumPointers = &list->uNumPointers;
              //v16 = 2;
              //v11 = list->uNumPointers;
              pFace = &pIndoor->pFaces[pFaceID];
              v12 = &list->object_pool[list->uNumPointers];
              v12->object = &pIndoor->pFaces[pFaceID];
              v12 = (Vis_ObjectInfo *)((char *)v12 + 4);
              v12->object = v15;
              v12->sZValue = 2;
              ++*pNumPointers;
            }
          }
        }
        
        if (face->uAttributes & FACE_PICKED)
          face->uAttributes |= FACE_OUTLINED;
        else
          face->uAttributes &= ~FACE_OUTLINED;
        face->uAttributes &= ~FACE_PICKED;
      }
    }
    v5 = v17 + 1;
  }
}

//----- (004C17CF) --------------------------------------------------------
void Vis::PickOutdoorFaces_Mouse(float fDepth, RenderVertexSoft *pRay, Vis_SelectionList *list, Vis_SelectionFilter *filter, bool only_reachable)
{  
  if (!pOutdoor)
    return;

  for (uint i = 0; i < pOutdoor->uNumBModels; ++i)
  {
    int reachable;
    if (!IsBModelVisible(i, &reachable))
      continue;
    if (!reachable && only_reachable)
      continue;

    auto bmodel = &pOutdoor->pBModels[i];
    for (uint j = 0; j < bmodel->uNumFaces; ++j)
    {
      auto face = &bmodel->pFaces[j];
      if (is_part_of_selection(face, filter))
      {
        BLVFace blv_face;
        blv_face.FromODM(face);
        
        RenderVertexSoft intersection;
        if (Intersect_Ray_Face(pRay, pRay + 1, &fDepth, &intersection, &blv_face, i))
        {
          pGame->pIndoorCameraD3D->ViewTransform(&intersection, 1);
          int v13 = fixpoint_from_float(/*v12, */intersection.vWorldViewPosition.x);
          v13 &= 0xFFFF0000;
          v13 += PID(OBJECT_BModel, j | (i << 6));

          list->AddObject(face, VisObjectType_Face, v13);
        }

        if (blv_face.uAttributes & FACE_PICKED)
          face->uAttributes |= FACE_OUTLINED;
        else
          face->uAttributes &= ~FACE_OUTLINED;
        blv_face.uAttributes &= ~FACE_PICKED;
      }
    }
  }
}

//----- (004C1930) --------------------------------------------------------
//bool Vis::j_DoesRayIntersectBillboard(float fDepth, unsigned int uD3DBillboardIdx)
//{return DoesRayIntersectBillboard(fDepth, uD3DBillboardIdx);}

//----- (004C1944) --------------------------------------------------------
int Vis::PickClosestActor(int object_id, unsigned int pick_depth, int a4, int a5, int a6)
{
  //float v6; // ST00_4@3
  //int result; // eax@4
  Vis_SelectionFilter v8; // [sp+18h] [bp-20h]@3
  //__int64 v9; // [sp+2Ch] [bp-Ch]@3
  //Vis *v14; // [sp+34h] [bp-4h]@1

  //v14 = this;

  static Vis_SelectionList Vis_static_sub_4C1944_stru_F8BDE8;
  
  v8.object_type = VisObjectType_Sprite;
  v8.object_id = object_id;
  v8.at_ai_state = a6;
  v8.no_at_ai_state = a5;
  v8.select_flags = a4;
  Vis_static_sub_4C1944_stru_F8BDE8.uNumPointers = 0;
  PickBillboards_Keyboard(pick_depth, &Vis_static_sub_4C1944_stru_F8BDE8, &v8);
  Vis_static_sub_4C1944_stru_F8BDE8.create_object_pointers(Vis_SelectionList::Unique);
  sort_object_pointers(
    Vis_static_sub_4C1944_stru_F8BDE8.object_pointers,
    0,
    Vis_static_sub_4C1944_stru_F8BDE8.uNumPointers - 1);

  if (!Vis_static_sub_4C1944_stru_F8BDE8.uNumPointers)
    return -1;
  return Vis_static_sub_4C1944_stru_F8BDE8.object_pointers[0]->sZValue;
}

//----- (004C1A02) --------------------------------------------------------
void Vis::_4C1A02()
{
  RenderVertexSoft v1; // [sp+8h] [bp-C0h]@1
  RenderVertexSoft v2; // [sp+38h] [bp-90h]@1
  RenderVertexSoft v3; // [sp+68h] [bp-60h]@1
  RenderVertexSoft v4; // [sp+98h] [bp-30h]@1

  v4.flt_2C = 0.0;
  v4.vWorldPosition.x = 0.0;
  v4.vWorldPosition.y = 65536.0;
  v4.vWorldPosition.z = 0.0;
  v3.flt_2C = 0.0;
  v3.vWorldPosition.x = 65536.0;
  v3.vWorldPosition.y = 0.0;
  v3.vWorldPosition.z = 0.0;
  memcpy(&v1, &v3, sizeof(v1));
  v3.flt_2C = 0.0;
  v3.vWorldPosition.x = 0.0;
  v3.vWorldPosition.y = 65536.0;
  v3.vWorldPosition.z = 0.0;
  memcpy(&v2, &v4, sizeof(v2));
  v4.flt_2C = 0.0;
  v4.vWorldPosition.x = 65536.0;
  v4.vWorldPosition.y = 0.0;
  v4.vWorldPosition.z = 0.0;
  memcpy(&this->stru_200C, &v1, 0x60u);
  memcpy(&v1, &v4, sizeof(v1));
  memcpy(&v2, &v3, sizeof(v2));
  memcpy(&this->stru_206C, &v1, 0x60u);
}

//----- (004C1ABA) --------------------------------------------------------
bool Vis::SortVectors_x(RenderVertexSoft *a2, int a3, int a4)
{
  bool result; // eax@1
  RenderVertexSoft *v5; // edx@2
  int v6; // ebx@2
  int i; // ecx@2
  int v8; // esi@3
  int v9; // esi@5
  RenderVertexSoft *v10; // eax@8
  RenderVertexSoft v11; // [sp+4h] [bp-6Ch]@8
  RenderVertexSoft v12; // [sp+34h] [bp-3Ch]@2
  int v13; // [sp+64h] [bp-Ch]@7
  Vis *thisa; // [sp+68h] [bp-8h]@1
  RenderVertexSoft *v15; // [sp+6Ch] [bp-4h]@2

  result = a4 != 0;
  thisa = this;
  if ( a4 > a3 )
  {
    v5 = a2;
    v15 = &a2[a4];
    v6 = a3 - 1;
    memcpy(&v12, &a2[a4], sizeof(v12));
    for ( i = a4; ; i = v13 )
    {
      v8 = (int)&v5[v6].vWorldViewPosition;
      do
      {
        v8 += 48;
        ++v6;
      }
      while ( *(float *)v8 < (double)v12.vWorldViewPosition.x );
      v9 = (int)&v5[i].vWorldViewPosition;
      do
      {
        v9 -= 48;
        --i;
      }
      while ( *(float *)v9 > (double)v12.vWorldViewPosition.x );
      v13 = i;
      if ( v6 >= i )
        break;
      v10 = &v5[i];
      memcpy(&v11, &a2[v6], sizeof(v11));
      v5 = a2;
      memcpy(&a2[v6], v10, sizeof(a2[v6]));
      memcpy(v10, &v11, 0x30u);
    }
    memcpy(&v11, &v5[v6], sizeof(v11));
    memcpy(&v5[v6], v15, sizeof(v5[v6]));
    memcpy(v15, &v11, 0x30u);
    SortVectors_x(v5, a3, v6 - 1);
    SortVectors_x(a2, v6 + 1, a4);
  }
  return true;
}

//----- (004C1BAA) --------------------------------------------------------
int Vis::get_object_zbuf_val(Vis_ObjectInfo *info)
{
  switch (info->object_type)
  {
    case VisObjectType_Sprite:
    case VisObjectType_Face:
      return info->sZValue;

    default:
      MessageBoxW(nullptr, L"Undefined type requested for: CVis::get_object_zbuf_val()", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Vis.cpp:1037", 0);
      return -1;
  }
}

//----- (004C1BF1) --------------------------------------------------------
int Vis::get_picked_object_zbuf_val()
{
  if (!default_list.uNumPointers)
    return -1;

  return get_object_zbuf_val(default_list.object_pointers[0]);
}

//----- (004C1C0C) --------------------------------------------------------
bool Vis::Intersect_Ray_Face(RenderVertexSoft *pRayStart, RenderVertexSoft *pRayEnd, float *pDepth, RenderVertexSoft *Intersection, BLVFace *pFace, unsigned int pBModelID)
{
  float c1; // st5@6
  float c2; // st7@11
  Vec3_short_ IntersectPoint; // ST04_6@11


  if (pFace->Portal() || pFace->Invisible())
    return false;

  int ray_dir_x = pRayEnd->vWorldPosition.x - pRayStart->vWorldPosition.x,//calculate the direction vector of the line(�������� ������ ����������� �����)
      ray_dir_y = pRayEnd->vWorldPosition.y - pRayStart->vWorldPosition.y,
      ray_dir_z = pRayEnd->vWorldPosition.z - pRayStart->vWorldPosition.z;

//c1 = -d-(n*p0)
  c1 = -pFace->pFacePlane.dist -(pFace->pFacePlane.vNormal.x * pRayStart->vWorldPosition.x
        + pFace->pFacePlane.vNormal.y * pRayStart->vWorldPosition.y
        + pFace->pFacePlane.vNormal.z * pRayStart->vWorldPosition.z);
  if (c1 > 0)
    return false;
#define EPSILON 1e-6
//c2 = n*u
  c2 = pFace->pFacePlane.vNormal.x * ray_dir_y// get length of the line(��� ���� ��� ����� �����)
       + pFace->pFacePlane.vNormal.y * ray_dir_x 
       + pFace->pFacePlane.vNormal.z * ray_dir_z;
  if (c2 > -EPSILON && c2 < EPSILON)   // ray faces face's normal ( > 0) or parallel ( == 0)
    return false;

//t = -d-(n*p0)/n*u
  float t = c1 / c2;//How far is crossing the line in percent for 0 to 1(��� ������ ����������� ����� � ��������� �� 0 �� 1 )

  if (t < 0 || t > 1)
    return false;

// p(t) = p0 + tu;
  Intersection->vWorldPosition.x = pRayStart->vWorldPosition.x + t * ray_dir_y;// add the interest to the start line(���������� ������� ����� � ����� ������)
  Intersection->vWorldPosition.y = pRayStart->vWorldPosition.y + t * ray_dir_x;
  Intersection->vWorldPosition.z = pRayStart->vWorldPosition.z + t * ray_dir_z;

  IntersectPoint.x = Intersection->vWorldPosition.x;
  IntersectPoint.y = Intersection->vWorldPosition.y;
  IntersectPoint.z = Intersection->vWorldPosition.z;

  if ( !CheckIntersectBModel(pFace, IntersectPoint, pBModelID) )
    return false;

  *pDepth = t;//Record the distance from the origin of the ray (���������� ��������� �� ������ ����)
  return true;
}

//----- (004C1D2B) --------------------------------------------------------
bool Vis::CheckIntersectBModel(BLVFace *pFace, Vec3_short_ IntersectPoint, unsigned int uModelID)
{
  int v5; // esi@10
  bool v6; // edi@10
  signed int v10; // ebx@14
  int v15; // [sp+10h] [bp-Ch]@10
  signed int v16; // [sp+18h] [bp-4h]@10

  int a = 0, b = 0;

  if (IntersectPoint.x < pFace->pBounding.x1 || IntersectPoint.x > pFace->pBounding.x2 ||
      IntersectPoint.y < pFace->pBounding.y1 || IntersectPoint.y > pFace->pBounding.y2 ||
      IntersectPoint.z < pFace->pBounding.z1 || IntersectPoint.z > pFace->pBounding.z2 )
    return false;

  if (uModelID != -1)
    ODM_CreateIntersectFacesVertexCoordList(&a, &b, intersect_face_vertex_coords_list_a.data(), intersect_face_vertex_coords_list_b.data(),
                                &IntersectPoint, pFace, uModelID);
  else
    BLV_CreateIntersectFacesVertexCoordList(&a, &b, intersect_face_vertex_coords_list_a.data(), intersect_face_vertex_coords_list_b.data(),
                                  &IntersectPoint, pFace);
  v5 = 2 * pFace->uNumVertices;
  v16 = 0;
  intersect_face_vertex_coords_list_a[v5] = intersect_face_vertex_coords_list_a[0];
  intersect_face_vertex_coords_list_b[v5] = intersect_face_vertex_coords_list_b[0];
  v6 = intersect_face_vertex_coords_list_b[0] >= b;
  if (v5 <= 0)
    return false;
  for ( uint i = 0; i < v5; ++i )
  {
    if ( v16 >= 2 )
      break;
    if ( v6 ^ (intersect_face_vertex_coords_list_b[i + 1] >= b) )
    {
      if ( intersect_face_vertex_coords_list_a[i + 1] >= a )
        v10 = 0;
      else
        v10 = 2;
      v10 |= intersect_face_vertex_coords_list_a[i] < a ? 1 : 0;
      if ( v10 != 3 )
      {
        if ( !v10)
          ++v16;
        else
        {
          int _v1 = fixpoint_div(intersect_face_vertex_coords_list_a[i + 1] - intersect_face_vertex_coords_list_a[i],
                                 intersect_face_vertex_coords_list_b[i + 1] - intersect_face_vertex_coords_list_b[i]);
          int _v2 = fixpoint_mul(b - intersect_face_vertex_coords_list_b[i], _v1) + 32768;

          if (intersect_face_vertex_coords_list_a[i] + (_v2 >> 16) >= a)
            ++v16;
        }
      }
    }
    v6 = intersect_face_vertex_coords_list_b[i + 1] >= b;
  }

  if ( v16 != 1 )
    return false;

  extern bool show_picked_face;
  if ( show_picked_face )
    pFace->uAttributes |= FACE_PICKED;
  return true;
/*
  int v5; // esi@10
  bool v6; // edi@10
  signed int v10; // ebx@14
  int v11; // edi@16
  signed int v12; // ST28_4@18
  signed __int64 v13; // qtt@18
  signed int result; // eax@21
  int v15; // [sp+10h] [bp-Ch]@10
  signed int v16; // [sp+18h] [bp-4h]@10

  int a = 0, b = 0;

  if (IntersectPoint.x < pFace->pBounding.x1 || IntersectPoint.x > pFace->pBounding.x2 ||
      IntersectPoint.y < pFace->pBounding.y1 || IntersectPoint.y > pFace->pBounding.y2 ||
      IntersectPoint.z < pFace->pBounding.z1 || IntersectPoint.z > pFace->pBounding.z2 )
    return false;

  pFace->uAttributes |= 0x80000000;

  if (uModelID != -1)
    ODM_CreateIntersectFacesVertexCoordList(&a, &b, intersect_face_vertex_coords_list_a, intersect_face_vertex_coords_list_b,
                                &IntersectPoint, pFace, uModelID);
  else
    BLV_CreateIntersectFacesVertexCoordList(&a, &b, intersect_face_vertex_coords_list_a, intersect_face_vertex_coords_list_b,
                                  &IntersectPoint, pFace);
  v5 = 2 * pFace->uNumVertices;
  v16 = 0;
  intersect_face_vertex_coords_list_a[v5] = intersect_face_vertex_coords_list_a[0];
  intersect_face_vertex_coords_list_b[v5] = intersect_face_vertex_coords_list_b[0];
  v6 = intersect_face_vertex_coords_list_b[0] >= b;
  if (v5 <= 0)
    return false;
  for ( uint i = 0; i < v5; ++i )
  {
    if ( v16 >= 2 )
      break;
    if ( v6 ^ intersect_face_vertex_coords_list_b[i + 1] >= b )
    {
      if ( intersect_face_vertex_coords_list_a[i + 1] >= a )
        v10 = 0;
      else
        v10 = 2;
      v11 = v10 | intersect_face_vertex_coords_list_a[i] < a;
      if ( v11 != 3 )
      {
        if ( !v11
          || (v12 = intersect_face_vertex_coords_list_a[i + 1] - intersect_face_vertex_coords_list_a[i],
              LODWORD(v13) = v12 << 16,
              HIDWORD(v13) = v12 >> 16,
              intersect_face_vertex_coords_list_a[i]
            + ((signed int)(((unsigned __int64)(v13 / (intersect_face_vertex_coords_list_b[i + 1] - intersect_face_vertex_coords_list_b[i])
                                              * (signed int)((b - intersect_face_vertex_coords_list_b[i]) << 16)) >> 16) + 32768) >> 16) >= a) )
          ++v16;
      }
    }
    v6 = intersect_face_vertex_coords_list_b[i + 1] >= b;
  }
  result = true;
  if ( v16 != 1 )
    result = false;
  return result;
}*/
}

//----- (004C1EE5) --------------------------------------------------------
void Vis::BLV_CreateIntersectFacesVertexCoordList(int *a, int *b, __int16 *intersect_face_vertex_coords_list_a,
                                                                  __int16 *intersect_face_vertex_coords_list_b, 
                                                                  Vec3_short_ *IntersectPoint, BLVFace *pFace)
{
  if (pFace->uAttributes & FACE_XY_PLANE)
  {
    *a = IntersectPoint->x;
    *b = IntersectPoint->y;

    for (uint i = 0; i < pFace->uNumVertices; ++i)
    {
      intersect_face_vertex_coords_list_a[2 * i]     = pFace->pXInterceptDisplacements[i]     + pIndoor->pVertices[pFace->pVertexIDs[i]].x;
      intersect_face_vertex_coords_list_a[2 * i + 1] = pFace->pXInterceptDisplacements[i + 1] + pIndoor->pVertices[pFace->pVertexIDs[i + 1]].x;

      intersect_face_vertex_coords_list_b[2 * i]     = pFace->pYInterceptDisplacements[i]     + pIndoor->pVertices[pFace->pVertexIDs[i]].y;
      intersect_face_vertex_coords_list_b[2 * i + 1] = pFace->pYInterceptDisplacements[i + 1] + pIndoor->pVertices[pFace->pVertexIDs[i + 1]].y;
    }
  }
  else if (pFace->uAttributes & FACE_XZ_PLANE)
  {
    *a = IntersectPoint->x;
    *b = IntersectPoint->z;

    for (uint i = 0; i < pFace->uNumVertices; ++i)
    {
      intersect_face_vertex_coords_list_a[2 * i]     = pFace->pXInterceptDisplacements[i]     + pIndoor->pVertices[pFace->pVertexIDs[i]].x;
      intersect_face_vertex_coords_list_a[2 * i + 1] = pFace->pXInterceptDisplacements[i + 1] + pIndoor->pVertices[pFace->pVertexIDs[i + 1]].x;

      intersect_face_vertex_coords_list_b[2 * i]     = pFace->pZInterceptDisplacements[i]     + pIndoor->pVertices[pFace->pVertexIDs[i]].z;
      intersect_face_vertex_coords_list_b[2 * i + 1] = pFace->pZInterceptDisplacements[i + 1] + pIndoor->pVertices[pFace->pVertexIDs[i + 1]].z;
    }
  }
  else if (pFace->uAttributes & FACE_YZ_PLANE)
  {
    *a = IntersectPoint->y;
    *b = IntersectPoint->z;

    for (uint i = 0; i < pFace->uNumVertices; ++i)
    {
      intersect_face_vertex_coords_list_a[2 * i]     = pFace->pYInterceptDisplacements[i]     + pIndoor->pVertices[pFace->pVertexIDs[i]].y;
      intersect_face_vertex_coords_list_a[2 * i + 1] = pFace->pYInterceptDisplacements[i + 1] + pIndoor->pVertices[pFace->pVertexIDs[i + 1]].y;

      intersect_face_vertex_coords_list_b[2 * i]     = pFace->pZInterceptDisplacements[i]     + pIndoor->pVertices[pFace->pVertexIDs[i]].z;
      intersect_face_vertex_coords_list_b[2 * i + 1] = pFace->pZInterceptDisplacements[i + 1] + pIndoor->pVertices[pFace->pVertexIDs[i + 1]].z;
    }
  }
  else assert(false);
}

//----- (004C2186) --------------------------------------------------------
void Vis::ODM_CreateIntersectFacesVertexCoordList(int *a, int *b, __int16 *intersect_face_vertex_coords_list_a, 
                                                                  __int16 *intersect_face_vertex_coords_list_b, 
                                                                    Vec3_short_ *IntersectPoint, BLVFace *pFace, unsigned int uModelID)
{
  if (pFace->uAttributes & FACE_XY_PLANE)
  {
    *a = IntersectPoint->x;
    *b = IntersectPoint->y;

    for (int i = 0; i < pFace->uNumVertices; ++i)
    {
      intersect_face_vertex_coords_list_a[2 * i]     = pFace->pXInterceptDisplacements[i]     + pOutdoor->pBModels[uModelID].pVertices.pVertices[pFace->pVertexIDs[i]].x;
      intersect_face_vertex_coords_list_a[i * 2 + 1] = pFace->pXInterceptDisplacements[i + 1] + pOutdoor->pBModels[uModelID].pVertices.pVertices[pFace->pVertexIDs[i + 1]].x;

      intersect_face_vertex_coords_list_b[2 * i]     = pFace->pYInterceptDisplacements[i]     + pOutdoor->pBModels[uModelID].pVertices.pVertices[pFace->pVertexIDs[i]].y;
      intersect_face_vertex_coords_list_b[i * 2 + 1] = pFace->pYInterceptDisplacements[i + 1] + pOutdoor->pBModels[uModelID].pVertices.pVertices[pFace->pVertexIDs[i + 1]].y;
    }
  }
  else if (pFace->uAttributes & FACE_XZ_PLANE)
  {
    *a = IntersectPoint->x;
    *b = IntersectPoint->z;

    for (int i = 0; i < pFace->uNumVertices; ++i)
    {
      intersect_face_vertex_coords_list_a[2 * i]     = pFace->pXInterceptDisplacements[i]     + pOutdoor->pBModels[uModelID].pVertices.pVertices[pFace->pVertexIDs[i]].x;
      intersect_face_vertex_coords_list_a[i * 2 + 1] = pFace->pXInterceptDisplacements[i + 1] + pOutdoor->pBModels[uModelID].pVertices.pVertices[pFace->pVertexIDs[i + 1]].x;

      intersect_face_vertex_coords_list_b[2 * i]     = pFace->pZInterceptDisplacements[i]     + pOutdoor->pBModels[uModelID].pVertices.pVertices[pFace->pVertexIDs[i]].z;
      intersect_face_vertex_coords_list_b[i * 2 + 1] = pFace->pZInterceptDisplacements[i + 1] + pOutdoor->pBModels[uModelID].pVertices.pVertices[pFace->pVertexIDs[i + 1]].z;
    }
  }
  else if (pFace->uAttributes & FACE_YZ_PLANE)
  {
      *a = IntersectPoint->y;
      *b = IntersectPoint->z;

    for (int i = 0; i < pFace->uNumVertices; ++i)
    {
      intersect_face_vertex_coords_list_a[2 * i]     = pFace->pYInterceptDisplacements[i]     + pOutdoor->pBModels[uModelID].pVertices.pVertices[pFace->pVertexIDs[i]].y;
      intersect_face_vertex_coords_list_a[i * 2 + 1] = pFace->pYInterceptDisplacements[i + 1] + pOutdoor->pBModels[uModelID].pVertices.pVertices[pFace->pVertexIDs[i + 1]].y;

      intersect_face_vertex_coords_list_b[2 * i]     = pFace->pZInterceptDisplacements[i]     + pOutdoor->pBModels[uModelID].pVertices.pVertices[pFace->pVertexIDs[i]].z;
      intersect_face_vertex_coords_list_b[i * 2 + 1] = pFace->pZInterceptDisplacements[i + 1] + pOutdoor->pBModels[uModelID].pVertices.pVertices[pFace->pVertexIDs[i + 1]].z;
    }
  }
  else assert(false);
}

//----- (0046A0A1) --------------------------------------------------------
int UnprojectX(int x)
{
  int v3; // [sp-4h] [bp-8h]@5

  if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
  {
    //if ( pRenderer->pRenderD3D )
      v3 = pGame->pIndoorCameraD3D->fov;
    //else
    //  v3 = pIndoorCamera->fov_rad;
  }
  else
  {
    v3 = pODMRenderParams->int_fov_rad;
  }
  return stru_5C6E00->Atan2(x - pViewport->uScreenCenterX, v3) - stru_5C6E00->uIntegerHalfPi;
}

//----- (0046A0F6) --------------------------------------------------------
int UnprojectY(int y)
{
  int v3; // [sp-4h] [bp-8h]@5

  if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
  {
    //if ( pRenderer->pRenderD3D )
      v3 = pGame->pIndoorCameraD3D->fov;
    //else
    //  v3 = pIndoorCamera->fov_rad;
  }
  else
  {
    v3 = pODMRenderParams->int_fov_rad;
  }
  return stru_5C6E00->Atan2(y - pViewport->uScreenCenterY, v3) - stru_5C6E00->uIntegerHalfPi;
}

//----- (004C248E) --------------------------------------------------------
void Vis::CastPickRay(RenderVertexSoft *pRay, float fMouseX, float fMouseY, float fPickDepth)
{
  int pRotY; // esi@1
  Vec3_int_ pStartR; // ST08_12@1
  int pRotX; // ST04_4@1
  int pDepth; // eax@1
  RenderVertexSoft v11[2]; // [sp+2Ch] [bp-74h]@1
  int outx;
  int outz; // [sp+94h] [bp-Ch]@1
  int outy; // [sp+98h] [bp-8h]@1
  
  pRotY = pGame->pIndoorCameraD3D->sRotationY + UnprojectX(fMouseX);
  pRotX = pGame->pIndoorCameraD3D->sRotationX + UnprojectY(fMouseY);

  pStartR.z = pGame->pIndoorCameraD3D->vPartyPos.z;
  pStartR.x = pGame->pIndoorCameraD3D->vPartyPos.x;
  pStartR.y = pGame->pIndoorCameraD3D->vPartyPos.y;

  v11[1].vWorldPosition.x = (double)pGame->pIndoorCameraD3D->vPartyPos.x;
  v11[1].vWorldPosition.y = (double)pGame->pIndoorCameraD3D->vPartyPos.y;
  v11[1].vWorldPosition.z = (double)pGame->pIndoorCameraD3D->vPartyPos.z;

  pDepth = fixpoint_from_float(fPickDepth);
  Vec3_int_::Rotate(pDepth, pRotY, pRotX, pStartR, &outx, &outy, &outz);

  v11[0].vWorldPosition.x = (double)outx;
  v11[0].vWorldPosition.y = (double)outy;
  v11[0].vWorldPosition.z = (double)outz;

  memcpy(pRay + 0, &v11[1], sizeof(RenderVertexSoft));
  memcpy(pRay + 1, &v11[0], sizeof(RenderVertexSoft));
}

//----- (004C2551) --------------------------------------------------------
Vis_ObjectInfo *Vis_SelectionList::SelectionPointers(int pVisObjectType, int pid)
{
  //unsigned int v3; // esi@1
  //signed int v4; // edx@1
  //char *v5; // eax@2
  //Vis_ObjectInfo *result; // eax@6

  //v3 = this->uNumPointers;
  if ( this->uNumPointers > 0 )
  {
    for ( uint i = 0; i < this->uNumPointers; ++i )
    {
      if ( this->object_pool[i].object_type == pVisObjectType && (this->object_pool[i].sZValue & 0xFFFF) == pid )
        return &this->object_pool[i];
    }
  }
  return nullptr;
  /*v4 = 0;
  if ( this->uNumPointers <= 0 )
    return false;
  else
  {
    //v5 = (char *)&this->object_pool[0].sZValue;
    while ( this->object_pool[v4].object_type != a2 || (this->object_pool[v4].sZValue & 0xFFFF) != a3 )
    {
      ++v4;
      //v5 += 12;
      if ( v4 >= this->uNumPointers )
       return false;
    }
    result = &this->object_pool[v4];
  }
  return result;*/
}

//----- (004C2591) --------------------------------------------------------
void Vis_SelectionList::create_object_pointers(PointerCreationType type)
{
  switch (type)
  {
    case All:
    {
      for (uint i = 0; i < uNumPointers; ++i)
        object_pointers[i] = &object_pool[i];
    }
    break;

    case Unique: // seems quite retarted; the inner if condition will never trigger, since we compare pointers, not values. pointers will always be unique
    {            // but it may be decompilation error thou
      bool create = true;
 
      for (uint i = 0; i < uNumPointers; ++i)
      {
        for (uint j = 0; j < i; ++j)
        {
          if (object_pointers[j] == &object_pool[i])
          {
            create = false;
            break;
          }
        }

        if (create)
          object_pointers[i] = &object_pool[i];
      }
    }
    break;

    default:
      MessageBoxW(nullptr, L"Unknown pointer creation flag passed to ::create_object_pointers()", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Vis.cpp:1358", 0);
  }
}

//----- (004C264A) --------------------------------------------------------
void Vis::sort_object_pointers( Vis_ObjectInfo **pPointers, int start, int end )
    {
  int sort_start; // edx@1
  int forward_sort_index; // esi@2
  signed int backward_sort_index; // ecx@2
  unsigned int last_z_val; // eax@3
  unsigned int more_lz_val; // ebx@4
  unsigned int less_lz_val; // ebx@6
  Vis_ObjectInfo *temp_pointer; // eax@7
  Vis_ObjectInfo *a3a; // [sp+14h] [bp+Ch]@2

  sort_start = start;

  if ( end > start )
  {
    do
    {
      forward_sort_index = sort_start - 1;
      backward_sort_index = end;
      do
      {
        last_z_val = pPointers[end]->sZValue&0xFFFF0000;
        do
        {
          ++forward_sort_index;
          more_lz_val = pPointers[forward_sort_index]->sZValue&0xFFFF0000;
        }
        while ( more_lz_val < last_z_val );

        do
        {
          if ( backward_sort_index < 1 )
            break;
          --backward_sort_index;
          less_lz_val = pPointers[backward_sort_index]->sZValue&0xFFFF0000;
        }
        while ( less_lz_val > last_z_val );

        temp_pointer = pPointers[forward_sort_index];
        if ( forward_sort_index >= backward_sort_index )
            {
            pPointers[forward_sort_index] = pPointers[end];
            pPointers[end] = temp_pointer;
            }
        else
            {
            pPointers[forward_sort_index] = pPointers[backward_sort_index];
            pPointers[backward_sort_index] = temp_pointer;
            }
       
      } while ( forward_sort_index < backward_sort_index );

      sort_object_pointers(pPointers, sort_start, forward_sort_index - 1);
      sort_start = forward_sort_index + 1;
    }
    while ( end > forward_sort_index + 1 );
  }
}

//----- (004C26D0) --------------------------------------------------------
bool Vis::SortVerticesByX(RenderVertexD3D3 *a2, unsigned int uStart, unsigned int uEnd)
{
  bool result; // eax@1
  RenderVertexD3D3 *v5; // edx@2
  RenderVertexD3D3 *v6; // esi@2
  void *v7; // edi@2
  unsigned int v8; // ebx@2
  RenderVertexD3D3 *v9; // ecx@3
  RenderVertexD3D3 *v10; // ecx@5
  RenderVertexD3D3 *v11; // eax@8
  RenderVertexD3D3 v12; // [sp+4h] [bp-4Ch]@8
  RenderVertexD3D3 v13; // [sp+24h] [bp-2Ch]@2
  Vis *thisa; // [sp+44h] [bp-Ch]@1
  RenderVertexD3D3 *v15; // [sp+48h] [bp-8h]@2
  unsigned int v16; // [sp+4Ch] [bp-4h]@2

  result = uEnd;
  thisa = this;
  if ( (signed int)uEnd > (signed int)uStart )
  {
    v5 = a2;
    v6 = &a2[uEnd];
    v7 = &v13;
    v15 = &a2[uEnd];
    v8 = uStart - 1;
    v16 = uEnd;
    while ( 1 )
    {
      memcpy(v7, v6, 0x20u);
      v9 = &v5[v8];
      do
      {
        ++v9;
        ++v8;
      }
      while ( v9->pos.x < (double)v13.pos.x );
      v10 = &v5[v16];
      do
      {
        --v10;
        --v16;
      }
      while ( v10->pos.x > (double)v13.pos.x );
      if ( (signed int)v8 >= (signed int)v16 )
        break;
      v11 = &v5[v16];
      memcpy(&v12, &a2[v8], sizeof(v12));
      v5 = a2;
      memcpy(&a2[v8], v11, sizeof(a2[v8]));
      v6 = &v12;
      v7 = v11;
    }
    memcpy(&v12, &v5[v8], sizeof(v12));
    memcpy(&v5[v8], v15, sizeof(v5[v8]));
    memcpy(v15, &v12, 0x20u);
    SortVerticesByX(v5, uStart, v8 - 1);
    SortVerticesByX(a2, v8 + 1, uEnd);
  }
  return true;
}

//----- (004C27AD) --------------------------------------------------------
bool Vis::SortVerticesByY(RenderVertexD3D3 *a2, unsigned int uStart, unsigned int uEnd)
{
  bool result; // eax@1
  RenderVertexD3D3 *v5; // edx@2
  RenderVertexD3D3 *v6; // esi@2
  void *v7; // edi@2
  unsigned int v8; // ebx@2
  float *v9; // ecx@3
  float *v10; // ecx@5
  RenderVertexD3D3 *v11; // eax@8
  RenderVertexD3D3 v12; // [sp+4h] [bp-4Ch]@8
  RenderVertexD3D3 v13; // [sp+24h] [bp-2Ch]@2
  Vis *thisa; // [sp+44h] [bp-Ch]@1
  RenderVertexD3D3 *v15; // [sp+48h] [bp-8h]@2
  unsigned int v16; // [sp+4Ch] [bp-4h]@2

  result = uEnd;
  thisa = this;
  if ( (signed int)uEnd > (signed int)uStart )
  {
    v5 = a2;
    v6 = &a2[uEnd];
    v7 = &v13;
    v15 = &a2[uEnd];
    v8 = uStart - 1;
    v16 = uEnd;
    while ( 1 )
    {
      memcpy(v7, v6, 0x20u);
	  v9 = &v5[v8].pos.y;
      do
      {
        v9+=8;
        ++v8;
      }
      while ( *v9 < (double)v13.pos.y );
      v10 = &v5[v16].pos.y;
      do
      {
        v10-=8;
        --v16;
      }
      while ( *v10 > (double)v13.pos.y );
      if ( (signed int)v8 >= (signed int)v16 )
        break;
      v11 = &v5[v16];
      memcpy(&v12, &a2[v8], sizeof(v12));
      v5 = a2;
      memcpy(&a2[v8], v11, sizeof(a2[v8]));
      v6 = &v12;
      v7 = v11;
    }
    memcpy(&v12, &v5[v8], sizeof(v12));
    memcpy(&v5[v8], v15, sizeof(v5[v8]));
    memcpy(v15, &v12, 0x20u);
    SortVerticesByY(v5, uStart, v8 - 1);
    SortVerticesByY(a2, v8 + 1, uEnd);
  }
  return true;
}

//----- (004C288E) --------------------------------------------------------
bool Vis::SortByScreenSpaceX(RenderVertexSoft *pArray, int sLeft, int sRight)
{
  bool result; // eax@1
  RenderVertexSoft *v5; // edx@2
  int v6; // ebx@2
  int i; // ecx@2
  int v8; // esi@3
  int v9; // esi@5
  //RenderVertexSoft *v10; // eax@8
  const void *v10;
  RenderVertexSoft v11; // [sp+4h] [bp-6Ch]@8
  RenderVertexSoft v12; // [sp+34h] [bp-3Ch]@2
  //float v13; // [sp+4Ch] [bp-24h]@4
  int v14; // [sp+64h] [bp-Ch]@7
  //Vis *thisa; // [sp+68h] [bp-8h]@1
  //void *thisa;
  //RenderVertexSoft *v16; // [sp+6Ch] [bp-4h]@2
  const void *v16;

  //thisa = this;
  if (sRight <= sLeft)
    return true;
  v5 = pArray;
  v16 = &pArray[sRight];
  v6 = sLeft - 1;
  memcpy(&v12, &pArray[sRight], sizeof(v12));
  for ( i = sRight; ; i = v14 )
  {
    v8 = (int)&v5[v6].vWorldViewProjX;
    do
    {
      v8 += 48;
      ++v6;
    }
    while ( *(float *)v8 < v12.vWorldViewProjX);
    v9 = (int)&v5[i].vWorldViewProjX;
    do
    {
      v9 -= 48;
      --i;
    }
    while ( *(float *)v9 > v12.vWorldViewProjX);
    v14 = i;
    if ( v6 >= i )
      break;
    v10 = &v5[i];
    memcpy(&v11, &pArray[v6], sizeof(v11));
    v5 = pArray;
    memcpy(&pArray[v6], v10, sizeof(pArray[v6]));
    memcpy((void *)v10, &v11, sizeof(0x30u));
  }
  memcpy(&v11, &v5[v6], sizeof(v11));
  memcpy(&v5[v6], v16, sizeof(v5[v6]));
  memcpy((void *)v16, &v11, sizeof(0x30u));
  SortByScreenSpaceX(v5, sLeft, v6 - 1);
  SortByScreenSpaceX(pArray, v6 + 1, sRight);
  return true;
}

//----- (004C297E) --------------------------------------------------------
bool Vis::SortByScreenSpaceY(RenderVertexSoft *pArray, int sLeft, int sRight)
{
  //bool result; // eax@1
  RenderVertexSoft *v5; // edx@2
  int v6; // ebx@2
  int i; // ecx@2
  int v8; // esi@3
  int v9; // esi@5
  //RenderVertexSoft *v10; // eax@8
  const void *v10;
  //char v11; // [sp+4h] [bp-6Ch]@8
  RenderVertexSoft v11;
  RenderVertexSoft v12; // [sp+34h] [bp-3Ch]@2
  //float v13; // [sp+50h] [bp-20h]@4
  int v14; // [sp+64h] [bp-Ch]@7
  //Vis *thisa; // [sp+68h] [bp-8h]@1
  RenderVertexSoft *v16; // [sp+6Ch] [bp-4h]@2

  if (sRight <= sLeft)
   return true;
  v5 = pArray;
  v16 = &pArray[sRight];
  v6 = sLeft - 1;
  memcpy(&v12, &pArray[sRight], sizeof(v12));
  for ( i = sRight; ; i = v14 )
  {
    v8 = (int)&v5[v6].vWorldViewProjY;
    do
    {
      v8 += 48;
      ++v6;
    }
    while ( *(float *)v8 < v12.vWorldViewProjY);
    v9 = (int)&v5[v6].vWorldViewProjY;
    do
    {
      v9 -= 48;
      --i;
    }
    while ( *(float *)v9 > v12.vWorldViewProjY);
    v14 = i;
    if ( v6 >= i )
      break;
    v10 = &v5[i];
    memcpy(&v11, &pArray[v6], sizeof(0x30));
    v5 = pArray;
    memcpy(&pArray[v6], v10, sizeof(pArray[v6]));
    memcpy((void *)v10, &v11, sizeof(0x30));
  }
  memcpy(&v11, &v5[v6], sizeof(0x30));
  memcpy(&v5[v6], v16, sizeof(v5[v6]));
  memcpy((void *)v16, &v11, sizeof(0x30));
  SortByScreenSpaceY(v5, sLeft, v6 - 1);
  SortByScreenSpaceY(pArray, v6 + 1, sRight);
  return true;
}

//----- (004C04AF) --------------------------------------------------------
Vis::Vis()
{
  RenderVertexSoft v3; // [sp+Ch] [bp-60h]@1
  RenderVertexSoft v4; // [sp+3Ch] [bp-30h]@1

  v3.flt_2C = 0.0;
  v3.vWorldPosition.x = 0.0;
  v3.vWorldPosition.y = 65536.0;
  v3.vWorldPosition.z = 0.0;
  v4.flt_2C = 0.0;
  v4.vWorldPosition.x = 65536.0;
  v4.vWorldPosition.y = 0.0;
  v4.vWorldPosition.z = 0.0;
  memcpy(&stru_200C, &v4, sizeof(stru_200C));

  v4.flt_2C = 0.0;
  v4.vWorldPosition.x = 0.0;
  v4.vWorldPosition.y = 65536.0;
  v4.vWorldPosition.z = 0.0;
  memcpy(&stru_203C, &v3, sizeof(stru_203C));

  v3.flt_2C = 0.0;
  v3.vWorldPosition.x = 65536.0;
  v3.vWorldPosition.y = 0.0;
  v3.vWorldPosition.z = 0.0;
  memcpy(&stru_206C, &v3, sizeof(stru_206C));
  memcpy(&stru_209C, &v4, sizeof(stru_209C));

  keyboard_pick_depth = 512;
}

//----- (004C055C) --------------------------------------------------------
Vis_SelectionList::Vis_SelectionList()
{
  for (uint i = 0; i < 512; ++i)
  {
    object_pool[i].object = nullptr;
    object_pool[i].sZValue = -1;
    object_pool[i].object_type = VisObjectType_Any;
  }
  uNumPointers = 0;
}

//----- (004C05CC) --------------------------------------------------------
bool Vis::PickKeyboard(Vis_SelectionList *list, Vis_SelectionFilter *sprite_filter, Vis_SelectionFilter *face_filter)
{
  if (!list)
    list = &default_list;
  list->uNumPointers = 0;

  PickBillboards_Keyboard(keyboard_pick_depth, list, sprite_filter);
  if (uCurrentlyLoadedLevelType == LEVEL_Indoor)
    PickIndoorFaces_Keyboard(keyboard_pick_depth, list, face_filter);
  else if (uCurrentlyLoadedLevelType == LEVEL_Outdoor)
    PickOutdoorFaces_Keyboard(keyboard_pick_depth, list, face_filter);
  else
    assert(false);

  list->create_object_pointers(Vis_SelectionList::Unique);
  sort_object_pointers(list->object_pointers, 0, list->uNumPointers - 1);

  return true;
}

//----- (004C0646) --------------------------------------------------------
bool Vis::PickMouse(float fDepth, float fMouseX, float fMouseY, Vis_SelectionFilter *sprite_filter, Vis_SelectionFilter *face_filter)
{
  RenderVertexSoft pMouseRay[2]; // [sp+1Ch] [bp-60h]@1

  default_list.uNumPointers = 0;
  CastPickRay(pMouseRay, fMouseX, fMouseY, fDepth);
  PickBillboards_Mouse(fDepth, fMouseX, fMouseY, &default_list, sprite_filter);
  if (uCurrentlyLoadedLevelType == LEVEL_Indoor)
    PickIndoorFaces_Mouse(fDepth, pMouseRay, &default_list, face_filter);
  else if (uCurrentlyLoadedLevelType == LEVEL_Outdoor)
    PickOutdoorFaces_Mouse(fDepth, pMouseRay, &default_list, face_filter, false);
  else
  {
    Log::Warning(L"Picking mouse in undefined level"); // picking in main menu is default (buggy) game behaviour. should've returned false in Game::PickMouse
    return false;
  }
  default_list.create_object_pointers(Vis_SelectionList::All);
  sort_object_pointers(default_list.object_pointers, 0, default_list.uNumPointers - 1);

  return true;
}

//----- (004C06F8) --------------------------------------------------------
void Vis::PickBillboards_Keyboard(float pick_depth, Vis_SelectionList *list, Vis_SelectionFilter *filter)
{
  for (int i = 0; i < pRenderer->uNumBillboardsToDraw; ++i)
  {
    auto d3d_billboard = &pRenderer->pBillboardRenderListD3D[i];

    if (is_part_of_selection((void *)i, filter))
    {
      if (DoesRayIntersectBillboard(pick_depth, i))
      {
        auto billboard = &pBillboardRenderList[d3d_billboard->uParentBillboardID];

        list->AddObject((void *)d3d_billboard->uParentBillboardID, VisObjectType_Sprite, billboard->sZValue);
      }
    }
  }
}


// tests the object against selection filter to determine whether it can be picked or not
//----- (004C0791) --------------------------------------------------------
bool Vis::is_part_of_selection(void *uD3DBillboardIdx_or_pBLVFace_or_pODMFace, Vis_SelectionFilter *filter)
{
  //stru157 *v3; // esi@1
  //int result; // eax@1
  int v5; // edx@2
  //int v6; // ecx@2
  //char v7; // zf@3
  int v8; // esi@5
  std::string *v9; // ecx@7
  Actor *v10; // edi@18
  //const char *v12; // [sp-20h] [bp-2Ch]@7
  int v13; // [sp-1Ch] [bp-28h]@7
  //std::string v14; // [sp-18h] [bp-24h]@7
  //const char *v15; // [sp-8h] [bp-14h]@7
  int v16; // [sp-4h] [bp-10h]@7

  switch (filter->object_type)
  {
    case VisObjectType_Any:
      return true;

    case VisObjectType_Sprite:
    {
      v5 = filter->select_flags;
      int object_idx = PID_ID(pBillboardRenderList[pRenderer->pBillboardRenderListD3D[(int)uD3DBillboardIdx_or_pBLVFace_or_pODMFace].uParentBillboardID].object_pid);
      int object_type = PID_TYPE(pBillboardRenderList[pRenderer->pBillboardRenderListD3D[(int)uD3DBillboardIdx_or_pBLVFace_or_pODMFace].uParentBillboardID].object_pid);
      if ( v5 & 2 )
      {
        if (object_type == filter->object_id)
          return false;
        return true;
      }
      if ( v5 & 4 )
      {
        v8 = filter->object_id;
        if ( object_type != filter->object_id)
          return true;
        if (v8 != OBJECT_Decoration)
        {
          MessageBoxA(nullptr, "Unsupported \"exclusion if no event\" type in CVis::is_part_of_selection", "E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Vis.cpp:207", 0);
          return true;
        }
        if (pLevelDecorations[object_idx].uCog || pLevelDecorations[object_idx].uEventID)
          return true;
        return pLevelDecorations[object_idx].IsInteractive();
      }
      if (object_type == filter->object_id)
      {
        if (object_type != OBJECT_Actor)
        {
          MessageBoxA(nullptr, "Default case reached in VIS", "E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Vis.cpp:245", 0);
          return true;
        }

        v10 = &pActors[object_idx];
        int result = 1 << LOBYTE(v10->uAIState);
        if ( result & filter->no_at_ai_state
            || !(result & filter->at_ai_state)
            || v5 & 8 && (result = MonsterStats::BelongsToSupertype(v10->pMonsterInfo.uID, MONSTER_SUPERTYPE_UNDEAD)) == 0 )
          return false;
        if ( !(filter->select_flags & 1) )
          return true;

        result = v10->GetActorsRelation(nullptr);
        if (result == 0)
          return false;
        return true;
      }
      return false;
    }

    case VisObjectType_Face:
    {
      uint face_attrib = 0;
      bool no_event = true;
      if (uCurrentlyLoadedLevelType == LEVEL_Outdoor)
      {
        auto face = (ODMFace *)uD3DBillboardIdx_or_pBLVFace_or_pODMFace;
        no_event = face->sCogTriggeredID == 0;
        face_attrib = face->uAttributes;
      }
      else if (uCurrentlyLoadedLevelType == LEVEL_Indoor) 
      {
        auto face = (BLVFace *)uD3DBillboardIdx_or_pBLVFace_or_pODMFace;
        no_event = pIndoor->pFaceExtras[face->uFaceExtraID].uEventID == 0;
        face_attrib = face->uAttributes;
      }
      else
        assert(false);

      if (filter->object_id != OBJECT_BLVDoor)
        return true;
      if (no_event || face_attrib & filter->no_at_ai_state)
        return false;
      return (face_attrib & filter->at_ai_state) != 0;
    }

    default:
      assert(false);
  }
}

//----- (004C091D) --------------------------------------------------------
bool Vis::DoesRayIntersectBillboard(float fDepth, unsigned int uD3DBillboardIdx)
{
  int v3; // eax@3
  //signed int v5; // ecx@4
  //float v6; // ST04_4@6
  //float v7; // ST00_4@7
  //int v8; // eax@10
  //unsigned int v9; // eax@12
  int v10; // eax@17
  double v11; // st6@18
  double v12; // st7@18
  double v13; // st4@18
  float v14; // ST0C_4@22
  float v15; // ST08_4@22
  //float v16; // ST04_4@23
  //float v17; // ST00_4@24
  //signed int v18; // eax@27
  //unsigned int v19; // eax@29
  double v20; // st6@32
  double v21; // st7@32
  int v22; // eax@32
  double v23; // st7@36
  //void *v24; // esi@40
  float v25; // ST08_4@40
  //float v26; // ST04_4@41
  //float v27; // ST00_4@42
  int v28; // eax@45
  unsigned int v29; // eax@47
  char result; // al@48
  struct RenderVertexSoft pPickingRay[2];
  //int v31; // [sp+20h] [bp-DCh]@5
  struct RenderVertexSoft local_80[2];

  float  test_x;
  float  test_y;

  float  t1_x;
  float  t1_y;
  float  t2_x;
  float  t2_y;
  float  swap_temp;
  int v37; // [sp+F0h] [bp-Ch]@5

  signed int v40; // [sp+108h] [bp+Ch]@17


  static Vis_SelectionList Vis_static_stru_F91E10;
  Vis_static_stru_F91E10.uNumPointers = 0;
  v3 = pRenderer->pBillboardRenderListD3D[uD3DBillboardIdx].uParentBillboardID;
  if (v3 == -1)
    return false;

  if (pBillboardRenderList[v3].GetFloatZ() > fDepth)
    return false;


    GetPolygonCenter(pRenderer->pBillboardRenderListD3D[v3].pQuads, 4, &test_x, &test_y);
    CastPickRay(pPickingRay, test_x, test_y, fDepth);
    if (uCurrentlyLoadedLevelType == LEVEL_Indoor)
      PickIndoorFaces_Mouse(fDepth, pPickingRay, &Vis_static_stru_F91E10, &vis_face_filter);
    else
      PickOutdoorFaces_Mouse(fDepth, pPickingRay, &Vis_static_stru_F91E10, &vis_face_filter, false);
    Vis_static_stru_F91E10.create_object_pointers();
    sort_object_pointers(Vis_static_stru_F91E10.object_pointers, 0, Vis_static_stru_F91E10.uNumPointers - 1);
    if (Vis_static_stru_F91E10.uNumPointers)
    {
       if (Vis_static_stru_F91E10.object_pointers[0]->actual_z > pBillboardRenderList[v3].actual_z)
         return true;
    }
    else if ((double)(pViewport->uScreen_TL_X) <= test_x &&
             (double)pViewport->uScreen_BR_X >= test_x &&
             (double)pViewport->uScreen_TL_Y <= test_y &&
             (double)pViewport->uScreen_BR_Y >= test_y)
      return true;

    for (v40 = 0; v40 < 4; ++v40)
        {
        test_x=pRenderer->pBillboardRenderListD3D[uD3DBillboardIdx].pQuads[v40].pos.x;
        test_y=  pRenderer->pBillboardRenderListD3D[uD3DBillboardIdx].pQuads[v40].pos.y;
        if ((double)(pViewport->uScreen_TL_X) <= test_x &&
            (double)pViewport->uScreen_BR_X >= test_x &&
            (double)pViewport->uScreen_TL_Y <= test_y &&
            (double)pViewport->uScreen_BR_Y >= test_y)
            {
            CastPickRay(local_80, test_x, test_y, fDepth);
            if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
                PickIndoorFaces_Mouse(fDepth, local_80, &Vis_static_stru_F91E10, &vis_face_filter);
            else
                PickOutdoorFaces_Mouse(fDepth, local_80, &Vis_static_stru_F91E10, &vis_face_filter, false);
            Vis_static_stru_F91E10.create_object_pointers();
            sort_object_pointers(Vis_static_stru_F91E10.object_pointers, 0, Vis_static_stru_F91E10.uNumPointers - 1);
            if ( !Vis_static_stru_F91E10.uNumPointers )
                return true;
            if (Vis_static_stru_F91E10.object_pointers[0]->actual_z > pBillboardRenderList[v3].actual_z)
                return true;
            }

        }
  
        if ( v40 >= 4 )
        {
          if ( uCurrentlyLoadedLevelType != LEVEL_Outdoor )
            return false;
          t1_x = pRenderer->pBillboardRenderListD3D[uD3DBillboardIdx].pQuads[0].pos.x;
          t2_x = pRenderer->pBillboardRenderListD3D[uD3DBillboardIdx].pQuads[3].pos.x;

          t1_y = pRenderer->pBillboardRenderListD3D[uD3DBillboardIdx].pQuads[0].pos.y;
          t2_y = pRenderer->pBillboardRenderListD3D[uD3DBillboardIdx].pQuads[1].pos.y;
          if ( t1_x > t2_x )
          {
            swap_temp = t1_x;
            t1_x = t2_x;
            t2_x = swap_temp;
          }
          if ( t1_y > t2_y )
            test_y = t1_y;
          else
            test_y = t2_y;

          Vis_static_stru_F91E10.uNumPointers = 0;

          test_x = (t2_x - t1_x) * 0.5;
          if ((double)(pViewport->uScreen_TL_X) <= test_x &&
              (double)pViewport->uScreen_BR_X >= test_x &&
              (double)pViewport->uScreen_TL_Y <= test_y &&
              (double)pViewport->uScreen_BR_Y >= test_y)
              {
              CastPickRay(local_80, test_x, test_y, fDepth);
              if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
                  PickIndoorFaces_Mouse(fDepth, local_80, &Vis_static_stru_F91E10, &vis_face_filter);
              else
                  PickOutdoorFaces_Mouse(fDepth, local_80, &Vis_static_stru_F91E10, &vis_face_filter, false);
              Vis_static_stru_F91E10.create_object_pointers();
              sort_object_pointers(Vis_static_stru_F91E10.object_pointers, 0, Vis_static_stru_F91E10.uNumPointers - 1);
              if ( !Vis_static_stru_F91E10.uNumPointers )
                  return true;
              if (Vis_static_stru_F91E10.object_pointers[0]->actual_z > pBillboardRenderList[v3].actual_z)
                  return true;

              }
        
      }
  return false;
}
// F93E18: using guessed type char static_byte_F93E18_init;

//----- (004C0D32) --------------------------------------------------------
void Vis::PickIndoorFaces_Keyboard(float pick_depth, Vis_SelectionList *list, Vis_SelectionFilter *filter)
{
  int result; // eax@1
  signed int pFaceID; // esi@2
  BLVFace *pFace; // edi@4
  //unsigned int v7; // eax@6
  Vis_ObjectInfo *v8; // eax@6
  signed int i; // [sp+18h] [bp-8h]@1

  result = 0;
  for ( i = 0; i < (signed int)pBspRenderer->num_faces; ++i )
  {
    pFaceID = pBspRenderer->faces[result].uFaceID;
    if ( pFaceID >= 0 )
    {
      if ( pFaceID < (signed int)pIndoor->uNumFaces )
      {
        pFace = &pIndoor->pFaces[pFaceID];
        if ( !pGame->pIndoorCameraD3D->IsCulled(&pIndoor->pFaces[pFaceID]) )
        {
          if ( is_part_of_selection(pFace, filter) )
          {
            v8 = DetermineFacetIntersection(pFace, PID(OBJECT_BModel, pFaceID), pick_depth);
            if ( v8 )
              list->AddObject(v8->object, v8->object_type, v8->sZValue);
          }
        }
      }
    }
    result = i + 1;
  }
}

//----- (004C0DEA) --------------------------------------------------------
void Vis::PickOutdoorFaces_Keyboard(float pick_depth, Vis_SelectionList *list, Vis_SelectionFilter *filter)
{
  for (uint i = 0; i < pOutdoor->uNumBModels; ++i)
  {
    int v17;
    if (!IsBModelVisible(i, &v17))
      continue;
    if (!v17)
      continue;

    auto bmodel = pOutdoor->pBModels + i;
    for (uint j = 0; j < bmodel->uNumFaces; ++j)
    {
      auto face = &bmodel->pFaces[j];

      if (is_part_of_selection(face, filter) )
      {
        BLVFace blv_face;
        blv_face.FromODM(face);

        int pid = PID(OBJECT_BModel, j | (i << 6));
        if (auto object_info = DetermineFacetIntersection(&blv_face, pid, pick_depth))
          list->AddObject(object_info->object, object_info->object_type, object_info->sZValue);
      }
    }
  }
}