#include "IndoorCameraD3D.h"
#include "VectorTypes.h"
#include "Game.h"
#include "Indoor.h"
#include "Viewport.h"
#include "Outdoor_stuff.h"
#include "Render.h"
#include "LOD.h"
#include "Math.h"
#include "mm7_data.h"

#include "stru9.h"


// calculates FOV (Field of View) angle in radians for IndoorCamera::Initialize and BLVRenderParams::Reset
float _calc_fov(int viewport_width, int angle_degree)
{
  return viewport_width * 0.5 / tan(angle_degree / 2 * 0.01745329) + 0.5;
}



//----- (004361EF) --------------------------------------------------------
IndoorCameraD3D::IndoorCameraD3D()
{
  IndoorCameraD3D *v1; // esi@1
  //double v2; // st7@1
  //double v3; // st6@1
  //double v4; // st5@1
  //double v5; // st7@1
  //double v6; // st6@1
  //double v7; // st5@1

  v1 = this;
  //IndoorCameraD3D_Vec3::IndoorCameraD3D_Vec3(&this->field_4);
  //IndoorCameraD3D_Vec3::IndoorCameraD3D_Vec3(&v1->field_14);
  //IndoorCameraD3D_Vec3::IndoorCameraD3D_Vec3(&v1->field_24);
  /*_eh_vector_constructor_iterator_(v1->std__vector_000034_prolly_frustrum,
    24,
    6,
    (void ( *)(void *))IndoorCameraD3D_Vec4::IndoorCameraD3D_Vec4,
    (void ( *)(void *))IndoorCameraD3D_Vec4::dtor);*/
  //v2 = 0;//(double)pBLVRenderParams->vPartyPos.z;
  //v3 = 0;//(double)pBLVRenderParams->vPartyPos.y;
  //v4 = 0;//(double)pBLVRenderParams->vPartyPos.x;
  v1->field_108 = 0.0;
  //v1->blv_party_x = 0;
  //v1->blv_party_y = 0;
  //v1->blv_party_z = 0;
  //v5 = 0;//(double)pBLVRenderParams->vPartyPos.z;
  //v6 = 0;//(double)pBLVRenderParams->vPartyPos.y;
  //v7 = 300;//(double)(pBLVRenderParams->vPartyPos.x + 300);
  v1->field_138 = 0.0;
  //v1->blv_party_x_2 = 300;
  //v1->blv_party_y_2 = 0;
  //v1->blv_party_z_2 = 0;
  v1->field_168 = 0.0;
  v1->field_198 = 0.0;
  v1->field_1C8 = 0.0;
  v1->field_1F8 = 0.0;
  v1->field_228 = 0.0;
  v1->field_258 = 0.0;
  v1->field_288 = 0.0;
  v1->field_2B8 = 0.0;
  v1->field_2E8 = 0.0;
  v1->field_2BC = 0.0;
  v1->field_2C0 = 0.0;
  v1->field_2C4 = 0.0;
  v1->field_318 = 0.0;
  v1->field_2EC = 0.0;
  v1->field_2F0 = 0.0;
  v1->field_2F4 = 0.0;
  v1->field_348 = 0.0;
  v1->field_31C = 0.0;
  v1->field_320 = 0.0;
  v1->field_324 = 0.0;
  v1->field_378 = 0.0;
  v1->field_34C = 0.0;
  v1->field_350 = 0.0;
  v1->field_354 = 0.0;
  for (uint i = 0; i < 16384; ++i)
  {
    list_0037C[i].field_0 = 0;
    list_0037C[i].flt_30 = 0.0f;
  }

  list_0037C_size = 0;
  for (uint i = 0; i < 256; ++i)
    list_E0380[i].mm7__vector_000004_size = 0;
  /*v10 = v1->list_E0380;
  v12 = 256;
  do
  {
    v10->mm7__vector_000004_size = 0;
    //mm7__vector_constructor(
    //  v10->mm7__vector_000004,
    //  48,
    //  64,
    //  (int ( *)(int))IndoorCameraD3D_stru1::IndoorCameraD3D_stru1);
    //++v10;
    --v12;
  }
  while ( v12 );*/
  list_E0380_size = 0;
  //v1->vdestructor_ptr = &stru8_pvdtor;
}


//----- (0043643E) --------------------------------------------------------
float IndoorCameraD3D::GetPickDepth()
{
  if (uCurrentlyLoadedLevelType == LEVEL_Outdoor)
    return pODMRenderParams->uPickDepth;
  else
    return 16192.0;
}

//----- (00436427) --------------------------------------------------------
float IndoorCameraD3D::GetShadingDistMist()
{
  if (uCurrentlyLoadedLevelType == LEVEL_Outdoor)
    return (double)pODMRenderParams->shading_dist_mist;
  else
    return 16192.0;
}

//----- (004364C5) --------------------------------------------------------
void IndoorCameraD3D::ViewTransfrom_OffsetUV(RenderVertexSoft *pVertices, unsigned int uNumVertices, RenderVertexSoft *pOutVertices, stru320 *a5)
{
  for (uint i = 0; i < uNumVertices; ++i)
  {
    pOutVertices[i].vWorldPosition.x = pVertices[i].vWorldPosition.x;
    pOutVertices[i].vWorldPosition.y = pVertices[i].vWorldPosition.y;
    pOutVertices[i].vWorldPosition.z = pVertices[i].vWorldPosition.z;

    pOutVertices[i].u = pVertices[i].u + a5->pDeltaUV[0];
    pOutVertices[i].v = pVertices[i].v + a5->pDeltaUV[1];
  }
  ViewTransform(pOutVertices, uNumVertices);
}

//----- (0043669D) --------------------------------------------------------
bool IndoorCameraD3D::ApplyViewTransform_TrueIfStillVisible_BLV(int x, int y, int z, signed int *pOutX, int *pOutZ, int *pOutY, char bDoNotShow)
{
  int to_z; // esi@2
  int v9; // ecx@3
  //signed int *v10; // esi@5
  //int pOutY_; // ecx@5
  //signed int v12; // esi@7
  int v14; // [sp+8h] [bp-4h]@3
  int to_x; // [sp+14h] [bp+8h]@1
  int to_y; // [sp+18h] [bp+Ch]@1
  int a2b; // [sp+18h] [bp+Ch]@5
  int a3a; // [sp+1Ch] [bp+10h]@5

  to_x = x - pGame->pIndoorCameraD3D->vPartyPos.x;
  to_y = y - pGame->pIndoorCameraD3D->vPartyPos.y;
  if (pGame->pIndoorCameraD3D->sRotationX)
  {
    to_z = (z - pGame->pIndoorCameraD3D->vPartyPos.z) << 16;
    //if ( pRenderer->pRenderD3D )
    //{
      v14 = (unsigned __int64)(to_x * (signed __int64)pGame->pIndoorCameraD3D->int_cosine_y)
          + (unsigned __int64)(to_y * (signed __int64)pGame->pIndoorCameraD3D->int_sine_y);
      v9 = (unsigned __int64)(to_x * (signed __int64)pGame->pIndoorCameraD3D->int_sine_y)
         - (unsigned __int64)(to_y * (signed __int64)pGame->pIndoorCameraD3D->int_cosine_y);
    //}
    a3a = (z - pGame->pIndoorCameraD3D->vPartyPos.z) << 16;
    a2b = (unsigned __int64)(to_z * (signed __int64)pGame->pIndoorCameraD3D->int_sine_x) >> 16;
    //v10 = pOutX;
    *pOutX = ((unsigned __int64)(v14 * (signed __int64)pGame->pIndoorCameraD3D->int_cosine_x) >> 16) - a2b;
    *pOutZ = v9;
    *pOutY = ((unsigned __int64)(v14 * (signed __int64)pGame->pIndoorCameraD3D->int_sine_x) >> 16)
           + ((unsigned __int64)(a3a * (signed __int64)pGame->pIndoorCameraD3D->int_cosine_x) >> 16);
  }
  else
  {
    *pOutY = (z - pGame->pIndoorCameraD3D->vPartyPos.z) << 16;
    //if ( pRenderer->pRenderD3D )
    //{
      //v10 = pOutX;
      *pOutX = (unsigned __int64)(to_x * (signed __int64)pGame->pIndoorCameraD3D->int_cosine_y)
             + (unsigned __int64)(to_y * (signed __int64)pGame->pIndoorCameraD3D->int_sine_y);
      *pOutZ = (unsigned __int64)(to_x * (signed __int64)pGame->pIndoorCameraD3D->int_sine_y)
             - (unsigned __int64)(to_y * (signed __int64)pGame->pIndoorCameraD3D->int_cosine_y);
    //}
  }
  if (!bDoNotShow)
    return false;

  return *pOutX >= fixpoint_from_int(4, 0) &&
         *pOutX <= fixpoint_from_int(8000, 0);
}

//----- (00436455) --------------------------------------------------------
bool IndoorCameraD3D::IsCulled(BLVFace *pFace)
{
  RenderVertexSoft v; // [sp+8h] [bp-30h]@1

  Vec3_short__to_RenderVertexSoft(&v, &pIndoor->pVertices[*pFace->pVertexIDs]);
  return is_face_faced_to_camera(pFace, &v);
}


//----- (00436523) --------------------------------------------------------
void IndoorCameraD3D::ViewTransform(RenderVertexSoft *a1a, unsigned int uNumVertices)
{
  if (byte_4D864C && pGame->uFlags & 0x80 ||
      uCurrentlyLoadedLevelType == LEVEL_Indoor)
  {
    float sin_x = fRotationXSine,
          cos_x = fRotationXCosine;
    float sin_y = fRotationYSine,
          cos_y = fRotationYCosine;

    //v4 = uNumVertices;
    //v7 = pIndoorCamera->fRotationXSine;
    if (pGame->pIndoorCameraD3D->sRotationX)
    {

        //_EAX = a1a;
    for (uint i = 0; i < uNumVertices; ++i)
    {
          float st0, st1, st2;
          if ( pRenderer->pRenderD3D )
          {
            /*__asm
            {
              fld     [ebp+uNumVertices] // [(a1a[i].y - pIndoorCamera->pos.y)]   [(a1a[i].z - pIndoorCamera->pos.z)]   [pIndoorCamera->fRotationXCosine]   [pIndoorCamera->fRotationYSine]   [pIndoorCamera->fRotationYCosine]
              fmul    st, st(3)          // [pIndoorCamera->fRotationYSine * (a1a[i].y - pIndoorCamera->pos.y)]   [(a1a[i].z - pIndoorCamera->pos.z)]   [pIndoorCamera->fRotationXCosine]   [pIndoorCamera->fRotationYSine]   [pIndoorCamera->fRotationYCosine]
              fld     [ebp+a1]           // [(a1a[i].x - pIndoorCamera->pos.x)]   [pIndoorCamera->fRotationYSine * (a1a[i].y - pIndoorCamera->pos.y)]   [(a1a[i].z - pIndoorCamera->pos.z)]   [pIndoorCamera->fRotationXCosine]   [pIndoorCamera->fRotationYSine]   [pIndoorCamera->fRotationYCosine]
              fmul    st, st(5)          // 0[pIndoorCamera->fRotationYCosine * (a1a[i].x - pIndoorCamera->pos.x)]
                                         // 1[pIndoorCamera->fRotationYSine * (a1a[i].y - pIndoorCamera->pos.y)]
                                         // 2[(a1a[i].z - pIndoorCamera->pos.z)]
                                         // 3[pIndoorCamera->fRotationXCosine]
                                         // 4[pIndoorCamera->fRotationYSine]
                                         // 5[pIndoorCamera->fRotationYCosine]
              faddp   st(1), st          // [pIndoorCamera->fRotationYCosine * (a1a[i].x - pIndoorCamera->pos.x) + pIndoorCamera->fRotationYSine * (a1a[i].y - pIndoorCamera->pos.y)]   [(a1a[i].z - pIndoorCamera->pos.z)]   [pIndoorCamera->fRotationXCosine]   [pIndoorCamera->fRotationYSine]   [pIndoorCamera->fRotationYCosine]
              fld     [ebp+a1]           // [(a1a[i].x - pIndoorCamera->pos.x)]   [pIndoorCamera->fRotationYCosine * (a1a[i].x - pIndoorCamera->pos.x) + pIndoorCamera->fRotationYSine * (a1a[i].y - pIndoorCamera->pos.y)]   [(a1a[i].z - pIndoorCamera->pos.z)]   [pIndoorCamera->fRotationXCosine]   [pIndoorCamera->fRotationYSine]   [pIndoorCamera->fRotationYCosine]
              fmul    st, st(4)          // [pIndoorCamera->fRotationYSine * (a1a[i].x - pIndoorCamera->pos.x)]   [pIndoorCamera->fRotationYCosine * (a1a[i].x - pIndoorCamera->pos.x) + pIndoorCamera->fRotationYSine * (a1a[i].y - pIndoorCamera->pos.y)]   [(a1a[i].z - pIndoorCamera->pos.z)]   [pIndoorCamera->fRotationXCosine]   [pIndoorCamera->fRotationYSine]   [pIndoorCamera->fRotationYCosine]

              fld     [ebp+uNumVertices] // 0[a1a[i].y - pIndoorCamera->pos.y]
                                         // 1[pIndoorCamera->fRotationYSine * (a1a[i].x - pIndoorCamera->pos.x)]
                                         // 2[pIndoorCamera->fRotationYCosine * (a1a[i].x - pIndoorCamera->pos.x) + pIndoorCamera->fRotationYSine * (a1a[i].y - pIndoorCamera->pos.y)]
                                         // 3[(a1a[i].z - pIndoorCamera->pos.z)]
                                         // 4[pIndoorCamera->fRotationXCosine]
                                         // 5[pIndoorCamera->fRotationYSine]
                                         // 6[pIndoorCamera->fRotationYCosine]

              fmul    st, st(6)          // 0[pIndoorCamera->fRotationYCosine * (a1a[i].y - pIndoorCamera->pos.y)]
                                         // 1[pIndoorCamera->fRotationYSine * (a1a[i].x - pIndoorCamera->pos.x)]
                                         // 2[pIndoorCamera->fRotationYCosine * (a1a[i].x - pIndoorCamera->pos.x) + pIndoorCamera->fRotationYSine * (a1a[i].y - pIndoorCamera->pos.y)]
                                         // 3[(a1a[i].z - pIndoorCamera->pos.z)]
                                         // 4[pIndoorCamera->fRotationXCosine]
                                         // 5[pIndoorCamera->fRotationYSine]
                                         // 6[pIndoorCamera->fRotationYCosine]

              fsubp   st(1), st          // 0[pIndoorCamera->fRotationYSine * (a1a[i].x - pIndoorCamera->pos.x) - pIndoorCamera->fRotationYCosine * (a1a[i].y - pIndoorCamera->pos.y)]
                                         // 1[pIndoorCamera->fRotationYCosine * (a1a[i].x - pIndoorCamera->pos.x) + pIndoorCamera->fRotationYSine * (a1a[i].y - pIndoorCamera->pos.y)]
                                         // 2[(a1a[i].z - pIndoorCamera->pos.z)]
                                         // 3[pIndoorCamera->fRotationXCosine]
                                         // 4[pIndoorCamera->fRotationYSine]
                                         // 5[pIndoorCamera->fRotationYCosine]
            }*/
            st0 = sin_y * (a1a[i].vWorldPosition.x - pGame->pIndoorCameraD3D->vPartyPos.x) - cos_y * (a1a[i].vWorldPosition.y - pGame->pIndoorCameraD3D->vPartyPos.y);
            st1 = cos_y * (a1a[i].vWorldPosition.x - pGame->pIndoorCameraD3D->vPartyPos.x) + sin_y * (a1a[i].vWorldPosition.y - pGame->pIndoorCameraD3D->vPartyPos.y);
            st2 = (a1a[i].vWorldPosition.z - pGame->pIndoorCameraD3D->vPartyPos.z);
          }
          else
          {
            /*__asm
            {
              fld     [ebp+a1]           // 0[(a1a[i].x - pIndoorCamera->pos.x)]
                                         // 1[(a1a[i].z - pIndoorCamera->pos.z)]
                                         // 2[pIndoorCamera->fRotationXCosine]
                                         // 3[pIndoorCamera->fRotationYSine]
                                         // 4[pIndoorCamera->fRotationYCosine]
              fmul    st, st(4)          // 0[pIndoorCamera->fRotationYCosine * (a1a[i].x - pIndoorCamera->pos.x)]
                                         // 1[(a1a[i].z - pIndoorCamera->pos.z)]
                                         // 2[pIndoorCamera->fRotationXCosine]
                                         // 3[pIndoorCamera->fRotationYSine]
                                         // 4[pIndoorCamera->fRotationYCosine]
              fld     [ebp+uNumVertices] // 0[(a1a[i].y - pIndoorCamera->pos.y)]
                                         // 1[pIndoorCamera->fRotationYCosine * (a1a[i].x - pIndoorCamera->pos.x)]
                                         // 2[(a1a[i].z - pIndoorCamera->pos.z)]
                                         // 3[pIndoorCamera->fRotationXCosine]
                                         // 4[pIndoorCamera->fRotationYSine]
                                         // 5[pIndoorCamera->fRotationYCosine]
              fmul    st, st(4)          // 0[pIndoorCamera->fRotationYSine * (a1a[i].y - pIndoorCamera->pos.y)]
                                         // 1[pIndoorCamera->fRotationYCosine * (a1a[i].x - pIndoorCamera->pos.x)]
                                         // 2[(a1a[i].z - pIndoorCamera->pos.z)]
                                         // 3[pIndoorCamera->fRotationXCosine]
                                         // 4[pIndoorCamera->fRotationYSine]
                                         // 5[pIndoorCamera->fRotationYCosine]
              fsubp   st(1), st          // 0[pIndoorCamera->fRotationYCosine * (a1a[i].x - pIndoorCamera->pos.x) - pIndoorCamera->fRotationYSine * (a1a[i].y - pIndoorCamera->pos.y)]
                                         // 1[(a1a[i].z - pIndoorCamera->pos.z)]
                                         // 2[pIndoorCamera->fRotationXCosine]
                                         // 3[pIndoorCamera->fRotationYSine]
                                         // 4[pIndoorCamera->fRotationYCosine]
              fld     [ebp+a1]           // 0[(a1a[i].x - pIndoorCamera->pos.x)]
                                         // 1[pIndoorCamera->fRotationYCosine * (a1a[i].x - pIndoorCamera->pos.x) - pIndoorCamera->fRotationYSine * (a1a[i].y - pIndoorCamera->pos.y)]
                                         // 2[(a1a[i].z - pIndoorCamera->pos.z)]
                                         // 3[pIndoorCamera->fRotationXCosine]
                                         // 4[pIndoorCamera->fRotationYSine]
                                         // 5[pIndoorCamera->fRotationYCosine]
              fmul    st, st(4)          // 0[pIndoorCamera->fRotationYSine * (a1a[i].x - pIndoorCamera->pos.x)]
                                         // 1[pIndoorCamera->fRotationYCosine * (a1a[i].x - pIndoorCamera->pos.x) - pIndoorCamera->fRotationYSine * (a1a[i].y - pIndoorCamera->pos.y)]
                                         // 2[(a1a[i].z - pIndoorCamera->pos.z)]
                                         // 3[pIndoorCamera->fRotationXCosine]
                                         // 4[pIndoorCamera->fRotationYSine]
                                         // 5[pIndoorCamera->fRotationYCosine]
              fld     [ebp+uNumVertices] // 0[(a1a[i].y - pIndoorCamera->pos.y)]
                                         // 1[pIndoorCamera->fRotationYSine * (a1a[i].x - pIndoorCamera->pos.x)]
                                         // 2[pIndoorCamera->fRotationYCosine * (a1a[i].x - pIndoorCamera->pos.x) - pIndoorCamera->fRotationYSine * (a1a[i].y - pIndoorCamera->pos.y)]
                                         // 3[(a1a[i].z - pIndoorCamera->pos.z)]
                                         // 4[pIndoorCamera->fRotationXCosine]
                                         // 5[pIndoorCamera->fRotationYSine]
                                         // 6[pIndoorCamera->fRotationYCosine]
              fmul    st, st(6)          // 0[pIndoorCamera->fRotationYCosine * (a1a[i].y - pIndoorCamera->pos.y)]
                                         // 1[pIndoorCamera->fRotationYSine * (a1a[i].x - pIndoorCamera->pos.x)]
                                         // 2[pIndoorCamera->fRotationYCosine * (a1a[i].x - pIndoorCamera->pos.x) - pIndoorCamera->fRotationYSine * (a1a[i].y - pIndoorCamera->pos.y)]
                                         // 3[(a1a[i].z - pIndoorCamera->pos.z)]
                                         // 4[pIndoorCamera->fRotationXCosine]
                                         // 5[pIndoorCamera->fRotationYSine]
                                         // 6[pIndoorCamera->fRotationYCosine]
              faddp   st(1), st          // 0[pIndoorCamera->fRotationYCosine * (a1a[i].y - pIndoorCamera->pos.y) + pIndoorCamera->fRotationYSine * (a1a[i].x - pIndoorCamera->pos.x)]
                                         // 1[pIndoorCamera->fRotationYCosine * (a1a[i].x - pIndoorCamera->pos.x) - pIndoorCamera->fRotationYSine * (a1a[i].y - pIndoorCamera->pos.y)]
                                         // 2[(a1a[i].z - pIndoorCamera->pos.z)]
                                         // 3[pIndoorCamera->fRotationXCosine]
                                         // 4[pIndoorCamera->fRotationYSine]
                                         // 5[pIndoorCamera->fRotationYCosine]
            }*/
            st0 = cos_y * (a1a[i].vWorldPosition.y - pGame->pIndoorCameraD3D->vPartyPos.y) + sin_y * (a1a[i].vWorldPosition.x - pGame->pIndoorCameraD3D->vPartyPos.x);
            st1 = cos_y * (a1a[i].vWorldPosition.x - pGame->pIndoorCameraD3D->vPartyPos.x) - sin_y * (a1a[i].vWorldPosition.y - pGame->pIndoorCameraD3D->vPartyPos.y);
            st2 = (a1a[i].vWorldPosition.z - pGame->pIndoorCameraD3D->vPartyPos.z);
          }

          a1a[i].vWorldViewPosition.x = st1*cos_x - st2*sin_x;
          a1a[i].vWorldViewPosition.y = st0;
          a1a[i].vWorldViewPosition.z = st2*cos_x + st1*sin_x;
      }
    }
    else
    {
      for (uint i = 0; i < uNumVertices; ++i)
      {
          if ( pRenderer->pRenderD3D )
          {
            /*__asm
            {
              fld     [ebp+uNumVertices]   // 0[(a1a[i].y - pIndoorCamera->pos.y)]
                                           // 1[(a1a[i].z - pIndoorCamera->pos.z)]
                                           // 2[sin_y]
                                           // 3[cos_y]
              fmul    st, st(2)            // 0[sin_y * (a1a[i].y - pIndoorCamera->pos.y)]
                                           // 1[(a1a[i].z - pIndoorCamera->pos.z)]
                                           // 2[sin_y]
                                           // 3[cos_y]
              fld     [ebp+a1]             // 0[(a1a[i].x - pIndoorCamera->pos.x)]
                                           // 1[sin_y * (a1a[i].y - pIndoorCamera->pos.y)]
                                           // 2[(a1a[i].z - pIndoorCamera->pos.z)]
                                           // 3[sin_y]
                                           // 4[cos_y]
              fmul    st, st(4)            // 0[cos_y * (a1a[i].x - pIndoorCamera->pos.x)]
                                           // 1[sin_y * (a1a[i].y - pIndoorCamera->pos.y)]
                                           // 2[(a1a[i].z - pIndoorCamera->pos.z)]
                                           // 3[sin_y]
                                           // 4[cos_y]
              faddp   st(1), st            // 0[cos_y * (a1a[i].x - pIndoorCamera->pos.x) + sin_y * (a1a[i].y - pIndoorCamera->pos.y)]
                                           // 1[(a1a[i].z - pIndoorCamera->pos.z)]
                                           // 2[sin_y]
                                           // 3[cos_y]
              fstp    dword ptr [eax+0Ch]
              fld     [ebp+a1]             // 0[(a1a[i].x - pIndoorCamera->pos.x)]
                                           // 1[(a1a[i].z - pIndoorCamera->pos.z)]
                                           // 2[sin_y]
                                           // 3[cos_y]
              fmul    st, st(2)            // 0[sin_y * (a1a[i].x - pIndoorCamera->pos.x)]
                                           // 1[(a1a[i].z - pIndoorCamera->pos.z)]
                                           // 2[sin_y]
                                           // 3[cos_y]
              fld     [ebp+uNumVertices]   // 0[(a1a[i].y - pIndoorCamera->pos.y)]
                                           // 1[sin_y * (a1a[i].x - pIndoorCamera->pos.x)]
                                           // 2[(a1a[i].z - pIndoorCamera->pos.z)]
                                           // 3[sin_y]
                                           // 4[cos_y]
              fmul    st, st(4)            // 0[cos_y * (a1a[i].y - pIndoorCamera->pos.y)]
                                           // 1[sin_y * (a1a[i].x - pIndoorCamera->pos.x)]
                                           // 2[(a1a[i].z - pIndoorCamera->pos.z)]
                                           // 3[sin_y]
                                           // 4[cos_y]
              fsubp   st(1), st            // 0[sin_y * (a1a[i].x - pIndoorCamera->pos.x) - cos_y * (a1a[i].y - pIndoorCamera->pos.y)]
                                           // 1[(a1a[i].z - pIndoorCamera->pos.z)]
                                           // 2[sin_y]
                                           // 3[cos_y]
              fstp    dword ptr [eax+10h]
              fstp    dword ptr [eax+14h]
            }*/
            a1a[i].vWorldViewPosition.x = cos_y * (a1a[i].vWorldPosition.x - pGame->pIndoorCameraD3D->vPartyPos.x) + sin_y * (a1a[i].vWorldPosition.y - pGame->pIndoorCameraD3D->vPartyPos.y);
            a1a[i].vWorldViewPosition.y = sin_y * (a1a[i].vWorldPosition.x - pGame->pIndoorCameraD3D->vPartyPos.x) - cos_y * (a1a[i].vWorldPosition.y - pGame->pIndoorCameraD3D->vPartyPos.y);
            a1a[i].vWorldViewPosition.z = (a1a[i].vWorldPosition.z - pGame->pIndoorCameraD3D->vPartyPos.z);
          }
          else
          {
            __debugbreak();
            /*__asm
            {
              fld     [ebp+a1]
              fmul    st, st(3)
              fld     [ebp+uNumVertices]
              fmul    st, st(3)
              fsubp   st(1), st
              fstp    dword ptr [eax+0Ch]
              fld     [ebp+a1]
              fmul    st, st(2)
              fld     [ebp+uNumVertices]
              fmul    st, st(4)
              faddp   st(1), st

              fstp    dword ptr [eax+10h]
              fstp    dword ptr [eax+14h]
            }*/
          }
      }
    }
  }
  else for (uint i = 0; i < uNumVertices; ++i)
    //pIndoorCamera->ViewTransform_ODM(a1a + i);
  {
    //ViewTransform_ODM(a1a + i);

    RenderVertexSoft* a1 = &a1a[i];
    //----- (00481CCE) --------------------------------------------------------
    //void ViewTransform_ODM(RenderVertexSoft *a1)
    {
  float result; // eax@1
  double vCamToVertexZ; // st7@1
  double v3; // st6@1
  double v4; // st5@1
  double v5; // st4@1
  float v6; // ST04_4@3
  float v7; // [sp+0h] [bp-14h]@1
  float v8; // [sp+8h] [bp-Ch]@1
  float vCamToVertexX; // [sp+Ch] [bp-8h]@1
  float vCamToVertexY; // [sp+10h] [bp-4h]@1

  v8 = fRotationXCosine;
  result = fRotationXSine;
  v7 = fRotationXSine;
  vCamToVertexX = a1->vWorldPosition.x - (double)pGame->pIndoorCameraD3D->vPartyPos.x;
  vCamToVertexY = a1->vWorldPosition.y - (double)pGame->pIndoorCameraD3D->vPartyPos.y;
  vCamToVertexZ = a1->vWorldPosition.z - (double)pGame->pIndoorCameraD3D->vPartyPos.z;
  v3 = fRotationYCosine;
  v4 = fRotationYSine;
  v5 = vCamToVertexY * fRotationYSine + fRotationYCosine * vCamToVertexX;
  if (pGame->pIndoorCameraD3D->sRotationX)
  {
    v6 = vCamToVertexY * fRotationYSine + fRotationYCosine * vCamToVertexX;
    a1->vWorldViewPosition.x = v5 * fRotationXCosine + fRotationXSine * vCamToVertexZ;
    a1->vWorldViewPosition.y = v3 * vCamToVertexY - v4 * vCamToVertexX;
    a1->vWorldViewPosition.z = v8 * vCamToVertexZ - v6 * v7;
  }
  else
  {
    a1->vWorldViewPosition.x = vCamToVertexY * fRotationYSine + fRotationYCosine * vCamToVertexX;
    a1->vWorldViewPosition.y = v3 * vCamToVertexY - v4 * vCamToVertexX;
    a1->vWorldViewPosition.z = vCamToVertexZ;
  }
    }
  }
}


//----- (00436932) --------------------------------------------------------
bool IndoorCameraD3D::GetFacetOrientation(char polyType, Vec3_float_ *a2, Vec3_float_ *a3, Vec3_float_ *a4)
{
  switch ((PolygonType)polyType)
  {
    case POLYGON_VerticalWall:
    {
      a4->x = -a2->y;
      a4->y = a2->x;
      a4->z = 0.0;

      a3->x = 0.0;
      a3->y = 0.0;
      a3->z = 1.0f;
    }
    return true;

    case POLYGON_Floor:
    case POLYGON_Ceiling:
    {
      a4->x = 1.0;
      a4->y = 0.0;
      a4->z = 0.0;

      a3->x = 0.0;
      a3->y = 1.0;
      a3->z = 0.0;
    }
    return true;

    case POLYGON_InBetweenFloorAndWall:
    case POLYGON_InBetweenCeilingAndWall:
    {
      if (fabs(a2->z) < 0.70811361)
      {
        a4->x = -a2->y;
        a4->y = a2->x;
        a4->z = 0.0;
        a4->Normalize();

        a3->x = 0.0;
        a3->y = 0.0;
        a3->z = 1.0;
      }
      else
      {
        a4->x = 1.0;
        a4->y = 0.0;
        a4->z = 0.0;

        a3->x = 0.0;
        a3->y = 1.0;
        a3->z = 0.0;
      }
    }
    return true;

    default:
      return false;
  }
}

//----- (00438258) --------------------------------------------------------
bool IndoorCameraD3D::is_face_faced_to_camera(BLVFace *pFace, RenderVertexSoft *a2)
{
  if (pFace->Portal())
    return false;

  //really strange cull; dot(to_cam, normal) < 0 means we see the BACK face, not font %_%
  if ( (a2->vWorldPosition.z - (double)pGame->pIndoorCameraD3D->vPartyPos.z) * (double)pFace->pFacePlane_old.vNormal.z
     + (a2->vWorldPosition.y - (double)pGame->pIndoorCameraD3D->vPartyPos.y) * (double)pFace->pFacePlane_old.vNormal.y
     + (a2->vWorldPosition.x - (double)pGame->pIndoorCameraD3D->vPartyPos.x) * (double)pFace->pFacePlane_old.vNormal.x < 0.0)
    return false;

  return true;
}

//----- (00437C96) --------------------------------------------------------
void IndoorCameraD3D::do_draw_debug_line_d3d(const RenderVertexD3D3 *pLineBegin, unsigned int uDiffuseBegin, const RenderVertexD3D3 *pLineEnd, unsigned int uDiffuseEnd, float z_stuff)
{
  double v6; // st7@2
  IDirect3DDevice3 *v7; // eax@2
  HRESULT v8; // eax@2
  std::string v9; // [sp-18h] [bp-60h]@3
  const char *v10; // [sp-Ch] [bp-54h]@2
  const char *v11; // [sp-8h] [bp-50h]@2
  int v12; // [sp-4h] [bp-4Ch]@2
  RenderVertexD3D3 v13[2]; // [sp+8h] [bp-40h]@2

  if ( pRenderer->pRenderD3D )
  {
    v6 = 0.001 - z_stuff;
    memcpy(v13, pLineBegin, 0x20u);
    memcpy(&v13[1], pLineEnd, sizeof(v13[1]));
    v13[0].pos.z = v6;
    v13[1].pos.z = v6;
    v12 = 0;
    v13[0].diffuse = uDiffuseBegin;
    v13[1].diffuse = uDiffuseEnd;
    v7 = pRenderer->pRenderD3D->pDevice;
    v11 = 0;
    //v10 = (const char *)v7;
    ErrD3D(pRenderer->pRenderD3D->pDevice->SetTexture(0, nullptr));
    ErrD3D(pRenderer->pRenderD3D->pDevice->DrawPrimitive(
           D3DPT_LINELIST,
           452,
           v13,
           2,
           16));
  }
  else
  {
    MessageBoxW(nullptr, L"draw_debug_line() not implemented for SW rendering", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Core3D.cpp:1415", 0);
  }
}

//----- (00437AB5) --------------------------------------------------------
void IndoorCameraD3D::do_draw_debug_line_sw(RenderVertexSoft *pLineBegin, unsigned int uStartDiffuse, RenderVertexSoft *pLineEnd, unsigned int uEndDiffuse, unsigned int uOutNumVertices, float z_stuff)
{
  char *v7; // eax@2
  signed int v8; // edx@2
  float v9; // ecx@5
  float v10; // eax@5
  char *v11; // eax@6
  signed int v12; // edx@6
  float v13; // edx@8
  float v14; // eax@8
  float v15; // eax@8
  double v16; // st7@11
  HRESULT v17; // eax@11
  std::string v18; // [sp-18h] [bp-7E4h]@12
  IDirect3DDevice3 *v19; // [sp-Ch] [bp-7D8h]@8
  const char *v20; // [sp-8h] [bp-7D4h]@8
  int v21; // [sp-4h] [bp-7D0h]@8
  RenderVertexSoft a1[20]; // [sp+8h] [bp-7C4h]@6
  RenderVertexSoft pVertices[20]; // [sp+3C8h] [bp-404h]@2
  RenderVertexD3D3 v24[2]; // [sp+788h] [bp-44h]@11
  IndoorCameraD3D *v25; // [sp+7C8h] [bp-4h]@1

  v25 = this;
  if ( pRenderer->pRenderD3D )
  {
    v7 = (char *)&pVertices[0].flt_2C;
    v8 = 20;
    do
    {
      *(float *)v7 = 0.0;
      v7 += 48;
      --v8;
    }
    while ( v8 );
    if ( (char)uOutNumVertices )
    {
      v9 = pLineBegin->vWorldViewProjX;
      pVertices[0].vWorldViewProjY = pLineBegin->vWorldViewProjY;
      pVertices[0].vWorldViewProjX = v9;
      v10 = pLineEnd->vWorldViewProjY;
      pVertices[1].vWorldViewProjX = pLineEnd->vWorldViewProjX;
      pVertices[1].vWorldViewProjY = v10;
LABEL_11:
      v16 = 0.001 - z_stuff;
      v24[0].specular = 0;
      v24[0].pos.x = pVertices[0].vWorldViewProjX;
      v24[0].pos.y = pVertices[0].vWorldViewProjY;
      v24[0].pos.z = v16;
      v24[0].diffuse = uStartDiffuse;
      v24[0].rhw = 0.001;
      v24[1].pos.x = pVertices[1].vWorldViewProjX;
      v24[0].texcoord.x = 0.0;
      v24[1].pos.y = pVertices[1].vWorldViewProjY;
      v24[0].texcoord.y = 0.0;
      v24[1].diffuse = uEndDiffuse;
      v24[1].pos.z = v16;
      v24[1].specular = 0;
      v21 = 0;
      v24[1].rhw = 0.001;
      v20 = 0;
      v24[1].texcoord.x = 0.0;
      v24[1].texcoord.y = 0.0;
      //v19 = pRenderer->pRenderD3D->pDevice;
      ErrD3D(pRenderer->pRenderD3D->pDevice->SetTexture(0, nullptr));
      ErrD3D(pRenderer->pRenderD3D->pDevice->DrawPrimitive(
              D3DPT_LINELIST,
              D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1,
              v24,
              2,
              16));
      return;
    }
    v11 = (char *)&a1[0].flt_2C;
    v12 = 20;
    do
    {
      *(float *)v11 = 0.0;
      v11 += 48;
      --v12;
    }
    while ( v12 );
    v21 = 0;
    v20 = (const char *)1;
    v19 = (IDirect3DDevice3 *)4;
    v13 = pLineBegin->vWorldPosition.x;
    uOutNumVertices = 2;
    a1[0].vWorldPosition.x = v13;
    v14 = pLineBegin->vWorldPosition.z;
    a1[0].vWorldPosition.y = pLineBegin->vWorldPosition.y;
    a1[0].vWorldPosition.z = v14;
    a1[1].vWorldPosition.x = pLineEnd->vWorldPosition.x;
    v15 = pLineEnd->vWorldPosition.z;
    a1[1].vWorldPosition.y = pLineEnd->vWorldPosition.y;
    a1[1].vWorldPosition.z = v15;
    if ( CalcPortalShape(
           a1,
           &uOutNumVertices,
           pVertices,
           this->std__vector_000034_prolly_frustrum,
           4,
           1,
           0) != 1
      || (signed int)uOutNumVertices >= 2 )
    {
      ViewTransform(pVertices, 2u);
      Project(pVertices, 2u, 0);
      goto LABEL_11;
    }
  }
  else
    MessageBoxW(nullptr, L"draw_debug_line() not implemented for SW rendering", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Core3D.cpp:1383", 0);
}

//----- (00437A55) --------------------------------------------------------
void IndoorCameraD3D::debug_outline_d3d(const RenderVertexD3D3 *pLineVertices, unsigned int uNumLines, int uDiffuse, float z_stuff)
{
  unsigned int v5; // esi@1
  const RenderVertexD3D3 *v6; // ecx@2
  unsigned int v7; // ebx@2
  const RenderVertexD3D3 *v8; // edi@3
  IndoorCameraD3D *thisa; // [sp+10h] [bp-4h]@1

  v5 = 0;
  thisa = this;
  if ( (signed int)(uNumLines - 1) > 0 )
  {
    v6 = pLineVertices;
    v7 = uNumLines - 1;
    v5 = uNumLines - 1;
    do
    {
      v8 = v6 + 1;
      do_draw_debug_line_d3d(v6, uDiffuse, v6 + 1, uDiffuse, z_stuff);
      --v7;
      v6 = v8;
    }
    while ( v7 );
  }
  do_draw_debug_line_d3d(&pLineVertices[v5], uDiffuse, pLineVertices, uDiffuse, z_stuff);
}

//----- (004379EE) --------------------------------------------------------
void IndoorCameraD3D::debug_outline_sw(RenderVertexSoft *a2, unsigned int uNumVertices, unsigned int uDiffuse, float a5)
{
  unsigned int v5; // esi@1
  RenderVertexSoft *v6; // eax@2
  RenderVertexSoft *v7; // edi@3
  char v8; // zf@3
  unsigned int uNumVerticesa; // [sp+20h] [bp+Ch]@2

  v5 = 0;
  if ( (signed int)(uNumVertices - 1) > 0 )
  {
    v6 = a2;
    v5 = uNumVertices - 1;
    uNumVerticesa = uNumVertices - 1;
    do
    {
      v7 = v6 + 1;
      do_draw_debug_line_sw(v6, uDiffuse, v6 + 1, uDiffuse, 0, a5);
      v8 = uNumVerticesa-- == 1;
      v6 = v7;
    }
    while ( !v8 );
  }
  do_draw_debug_line_sw(&a2[v5], uDiffuse, a2, uDiffuse, 0, a5);
}


void IndoorCameraD3D::DebugDrawPortal(BLVFace *pFace)
{
  Assert(pFace->uNumVertices <= 32);

  RenderVertexSoft sw[32];
  for (uint i = 0; i < pFace->uNumVertices; ++i)
  {
    sw[i].vWorldPosition.x = pIndoor->pVertices[pFace->pVertexIDs[i]].x;
    sw[i].vWorldPosition.y = pIndoor->pVertices[pFace->pVertexIDs[i]].y;
    sw[i].vWorldPosition.z = pIndoor->pVertices[pFace->pVertexIDs[i]].z;
  }
  ViewTransform(sw, pFace->uNumVertices);
  Project(sw, pFace->uNumVertices, 0);



  RenderVertexD3D3 v[32];
  for (uint i = 0; i < pFace->uNumVertices; ++i)
  {
    v[i].pos.x = sw[i].vWorldViewProjX;
    v[i].pos.y = sw[i].vWorldViewProjY;
    v[i].pos.z = 1.0 - 1.0 / (sw[i].vWorldViewPosition.x * 0.061758894);
    v[i].rhw = 1.0 / sw[i].vWorldViewPosition.x;
    v[i].diffuse = 0x80F020F0;
    v[i].specular = 0;
    //v[i].texcoord.x = pFace->pVertexUIDs[i] / (double)pTex->uTextureWidth;
    //v[i].texcoord.y = pFace->pVertexUIDs[i] / (double)pTex->uTextureHeight;
    v[i].texcoord.x = 0;
    v[i].texcoord.y = 0;
  }


  //ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, false));
  //ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZENABLE, false));
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE));
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA));
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA));
  
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetTexture(0, nullptr));
  ErrD3D(pRenderer->pRenderD3D->pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,
                D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1,
                v,
                pFace->uNumVertices,
                28));

  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE));
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ZERO));
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE));
  //ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZENABLE, TRUE));
  //ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE));
}

//----- (00437906) --------------------------------------------------------
void IndoorCameraD3D::PrepareAndDrawDebugOutline(BLVFace *pFace, unsigned int uDiffuse)
{
  static RenderVertexSoft static_sub_437906_array_50CDD0[64];
  static bool __init_flag1 = false;
  if (!__init_flag1)
  {
    __init_flag1 = true;

    for (uint i = 0; i < 64; ++i)
      static_sub_437906_array_50CDD0[i].flt_2C = 0.0f;
  }
  if ( pFace->uNumVertices )
  {
    for ( uint i = 0; i < pFace->uNumVertices; i++ )
    {
      static_sub_437906_array_50CDD0[i].vWorldPosition.x = (double)pIndoor->pVertices[pFace->pVertexIDs[i]].x;
      static_sub_437906_array_50CDD0[i].vWorldPosition.y = (double)pIndoor->pVertices[pFace->pVertexIDs[i]].y;
      static_sub_437906_array_50CDD0[i].vWorldPosition.z = (double)pIndoor->pVertices[pFace->pVertexIDs[i]].z;
      static_sub_437906_array_50CDD0[i].u = (double)pFace->pVertexUIDs[i];
      static_sub_437906_array_50CDD0[i].v = (double)pFace->pVertexVIDs[i];
    }
  }
  extern bool draw_debug_line;
  if ( draw_debug_line )
    debug_outline_sw(static_sub_437906_array_50CDD0, pFace->uNumVertices, uDiffuse, 0.0);
}
// 50D9D0: using guessed type char static_sub_437906_byte_50D9D0_init_flag;

//----- (004378BA) --------------------------------------------------------
void IndoorCameraD3D::MatrixMultiply(Matrix3x3_float_ *a1, Matrix3x3_float_ *a2, Matrix3x3_float_ *out)
{
  float *v4; // ecx@1
  float *v5; // eax@1
  signed int v6; // ebx@1
  float *v7; // edx@2
  float *v8; // ecx@2
  signed int v9; // edi@2
  double v10; // st7@3
  double v11; // st6@3

  v4 = (float *)out;
  v5 = &a1->_12;
  v6 = 3;
  do
  {
    v7 = v4;
    v8 = &a2->_21;
    v9 = 3;
    do
    {
      v10 = v8[3] * v5[1] + *(v8 - 3) * *(v5 - 1);
      v11 = *v8 * *v5;
      ++v8;
      *v7 = v10 + v11;
      ++v7;
      --v9;
    }
    while ( v9 );
    v5 += 3;
    --v6;
    v4 = v7;
  }
  while ( v6 );
}

//----- (004376E7) --------------------------------------------------------
void IndoorCameraD3D::CreateWorldMatrixAndSomeStuff()
{
  //IndoorCameraD3D *v1; // esi@1
  double v2; // st7@1
  //signed int v3; // edi@1
  //int v4; // eax@1
  //double v5; // st7@2
  //double v6; // st7@3
  Matrix3x3_float_ m1; // [sp+10h] [bp-B8h]@1
  Matrix3x3_float_ m2; // [sp+34h] [bp-94h]@1
  Matrix3x3_float_ m3; // [sp+58h] [bp-70h]@1
  Matrix3x3_float_ m4; // [sp+7Ch] [bp-4Ch]@1
  Matrix3x3_float_ m5; // [sp+A0h] [bp-28h]@1
  float v12; // [sp+C4h] [bp-4h]@1

 //RotationZ(0)
  m5._11 = cosf(0);         m5._12 = sinf(0);        m5._13 = 0;
  m5._21 = -sinf(0);        m5._22 = cosf(0);        m5._23 = 0;
  m5._31 = 0;               m5._32 = 0;              m5._33 = 1;

  float cos_x1 = fRotationXCosine,
        sin_x1 = fRotationXSine;
 //RotationX(x)
  m4._11 = 1;               m4._12 = 0;              m4._13 = 0;
  m4._21 = 0;               m4._22 = cos_x1;         m4._23 = sin_x1;
  m4._31 = 0;               m4._32 = -sin_x1;        m4._33 = cos_x1;
  
  float cos_y1 = fRotationYCosine,
        sin_y1 = fRotationYSine;
 //RotationY(some_angle)
  m3._11 = cos_y1;          m3._12 = 0;              m3._13 = -sin_y1;
  m3._21 = 0;               m3._22 = 1;              m3._23 = 0;
  m3._31 = sin_y1;          m3._32 = 0;              m3._33 = cos_y1;

  MatrixMultiply(&m5, &m3, &m1);
  MatrixMultiply(&m4, &m1, &m2);

  for (uint i = 0; i < 3; ++i)
  {
    field_4[0].v[i] = m2.v[1][i];
    field_4[1].v[i] = m2.v[0][i];
    field_4[2].v[i] = m2.v[2][i];
  }

  inv_fov = 1.1344639;
  fov_x = (double)pViewport->uScreenWidth * 0.8814736;

  fov_y = 0.8814736 * (double)pViewport->uScreenHeight;
  fov = fov_y;
  if ( fov_x > fov )
    fov = fov_x;
  screenCenterX = (double)pViewport->uScreenCenterX;
  screenCenterY = (double)(pViewport->uScreenCenterY - pViewport->uScreen_TL_Y);
}

//----- (00437691) --------------------------------------------------------
void IndoorCameraD3D::Vec3Transform(const IndoorCameraD3D_Vec3 *pVector, IndoorCameraD3D_Vec3 *pOut)
{
  pOut->y = field_4[1].x * pVector->x + field_4[0].x * pVector->y + field_4[2].x * pVector->z;
  pOut->z = field_4[1].y * pVector->x + field_4[0].y * pVector->y + field_4[2].y * pVector->z;
  pOut->x = field_4[1].z * pVector->x + field_4[0].z * pVector->y + field_4[2].z * pVector->z;
}

//----- (00437607) --------------------------------------------------------
void IndoorCameraD3D::_437607(IndoorCameraD3D_Vec3 *a1, IndoorCameraD3D_Vec4 *a2)
{
  double v4; // st7@1
  IndoorCameraD3D_Vec3 v8; // [sp+8h] [bp-1Ch]@1

  v8.x = (double)pGame->pIndoorCameraD3D->vPartyPos.x;
  v8.y = (double)pGame->pIndoorCameraD3D->vPartyPos.y;
  v8.z = (double)pGame->pIndoorCameraD3D->vPartyPos.z;
  Vec3Transform(a1, a2);

  v4 = v8.x * a2->x + v8.y * a2->y + v8.z * a2->z;
  a2->dot = v4 + 0.000099999997;
}

//----- (004374E8) --------------------------------------------------------
void IndoorCameraD3D::_4374E8_ProllyBuildFrustrum()
{
  //IndoorCameraD3D *v1; // esi@1
  //double v2; // st7@1
  double v3; // st7@1
  //double v4; // st7@1
  double v5; // st7@1
  //double v6; // st7@1
  IndoorCameraD3D_Vec3 v7; // [sp+Ch] [bp-24h]@1
  //float v8; // [sp+10h] [bp-20h]@1
  //float v9; // [sp+14h] [bp-1Ch]@1
  //float v10; // [sp+18h] [bp-18h]@1
  //float v11; // [sp+1Ch] [bp-14h]@1
  //float v12; // [sp+20h] [bp-10h]@1
  //int v13; // [sp+2Ch] [bp-4h]@1

  //v1 = this;
  //IndoorCameraD3D_Vec3::IndoorCameraD3D_Vec3(&v7);
  //v2 = 2.0 / inv_fov;
  //v13 = 0;
  v3 = atan(2.0 / inv_fov * fov / fov_x);
  //v12 = v3;
  //v11 = sin(v3);
  //v4 = cos(v3);
  v7.x = -sin(v3);
  v7.y = 0.0;
  v7.z = cos(v3);
  _437607(&v7, std__vector_000034_prolly_frustrum + 0);
  v7.x = sin(v3);
  _437607(&v7, std__vector_000034_prolly_frustrum + 1);
  v5 = atan(2.0 / inv_fov * fov / (fov_y + 0.5));
  //v12 = v5;
  //v11 = sin(v5);
  //v6 = cos(v5);
  v7.y = sin(v5);
  v7.x = 0.0;
  v7.z = cos(v5);
  _437607(&v7, &std__vector_000034_prolly_frustrum[2]);
  v7.y = -sin(v5);
  _437607(&v7, &std__vector_000034_prolly_frustrum[3]);
  //v13 = -1;
  //IndoorCameraD3D_Vec3::dtor(&v7);
}

//----- (00437376) --------------------------------------------------------
char IndoorCameraD3D::_437376(stru154 *thisa, RenderVertexSoft *a2, unsigned int *pOutNumVertices)
{
  unsigned int v4; // ebx@1
  RenderVertexSoft *v5; // edx@2
  double v6; // st7@3
  unsigned int v7; // edi@5
  signed int v8; // esi@6
  int v9; // ebx@8
  int v10; // eax@8
  int v11; // ecx@14
  int v12; // eax@14
  int v13; // eax@15
  signed int v14; // ebx@17
  RenderVertexSoft *v15; // eax@18
  unsigned int *v16; // eax@20
  char result; // al@24
  RenderVertexSoft v18; // [sp+Ch] [bp-34h]@2
  int v19; // [sp+3Ch] [bp-4h]@8
  signed int thisb; // [sp+48h] [bp+8h]@6
  char a2_3; // [sp+4Fh] [bp+Fh]@5

  v4 = *pOutNumVertices;
  if ( (signed int)*pOutNumVertices <= 3
    || ((v5 = a2,
         memcpy(&v18, a2, sizeof(v18)),
         (v18.vWorldPosition.z - (double)pGame->pIndoorCameraD3D->vPartyPos.z) * thisa->face_plane.vNormal.z
       + (v18.vWorldPosition.y - (double)pGame->pIndoorCameraD3D->vPartyPos.y) * thisa->face_plane.vNormal.y
       + (v18.vWorldPosition.x - (double)pGame->pIndoorCameraD3D->vPartyPos.x) * thisa->face_plane.vNormal.x < 0.0) ? (v6 = 1.0) : (v6 = -1.0),
        a2_3 = 0,
        memcpy(&v5[v4], v5, sizeof(v5[v4])),
        memcpy(&v5[*pOutNumVertices + 1], &v5[1], sizeof(v5[*pOutNumVertices + 1])),
        v7 = *pOutNumVertices,
        (signed int)*pOutNumVertices <= 0) )
    goto LABEL_28;
  v8 = 1;
  for ( thisb = 1; ; v8 = thisb )
  {
    v9 = v8 - 1;
    v10 = v8 + 1;
    v19 = v8 + 1;
    if ( v8 - 1 >= (signed int)v7 )
      v9 -= v7;
    if ( v8 >= (signed int)v7 )
      v8 -= v7;
    if ( v19 >= (signed int)v7 )
      v10 = v19 - v7;
    v11 = (int)&v5[v10];
    v12 = (int)&v5[v9];
    if ( -0.009999999776482582 > ((v5[v8].vWorldViewProjX - *(float *)(v12 + 24))
                                * (*(float *)(v11 + 28) - *(float *)(v12 + 28))
                                - (v5[v8].vWorldViewProjY - *(float *)(v12 + 28))
                                * (*(float *)(v11 + 24) - *(float *)(v12 + 24)))
                               * v6 )
    {
      thisb = v19;
      v16 = pOutNumVertices;
    }
    else
    {
      v13 = thisb;
      if ( thisb >= (signed int)v7 )
        v13 = thisb - v7;
      v14 = v13;
      if ( v13 < (signed int)v7 )
      {
        v15 = &v5[v13];
        do
        {
          memcpy(v15, &v15[1], 0x30u);
          ++v14;
          ++v15;
        }
        while ( v14 < (signed int)*pOutNumVertices );
      }
      v16 = pOutNumVertices;
      a2_3 = 1;
      --*v16;
    }
    v7 = *v16;
    if ( thisb - 1 >= (signed int)*v16 )
      break;
  }
  if ( a2_3 )
    result = 1;
  else
LABEL_28:
    result = 0;
  return result;
}

//----- (00437285) --------------------------------------------------------
bool IndoorCameraD3D::CalcPortalShape(RenderVertexSoft *a1, unsigned int *pOutNumVertices, RenderVertexSoft *pVertices, IndoorCameraD3D_Vec4 *a4, signed int uNumVertices, char a6, int _unused)
{
  char *v8; // eax@2
  signed int v9; // ecx@2
  //bool result; // eax@5
  int v11; // ecx@5
  //signed int v12; // ecx@6
  //char *v13; // esi@6
  RenderVertexSoft *v14; // eax@8
  RenderVertexSoft *v15; // edx@8
  Vec3_float_ a5; // [sp+18h] [bp-3Ch]@12
  //float v17; // [sp+44h] [bp-10h]@1
  //int v18; // [sp+48h] [bp-Ch]@5
  //stru9 *thisa; // [sp+4Ch] [bp-8h]@1
  int a7a; // [sp+53h] [bp-1h]@5
  //bool a6a; // [sp+70h] [bp+1Ch]@5

  //v17 = 0.0;
  //thisa = pGame->pStru9Instance;
  
  static RenderVertexSoft sr_vertices_50D9D8[64];

  //result = 0;
  a7a = 0;
  v11 = 2 * (a6 == 0) + 1;
  //a6a = 0;
  //v18 = v11;
  if (uNumVertices <= 0)
    return false;

    //v12 = *pOutNumVertices;
    //v13 = (char *)&a4->y;
  
  //while ( 1 )
  for ( uint i = 0; i < uNumVertices; ++i )
  {
    if (i % 2)
    {
      v14 = a1;
      v15 = sr_vertices_50D9D8;
    }
    else
    {
      v15 = a1;
      v14 = sr_vertices_50D9D8;
    }
    if (i == uNumVertices - 1)
      v14 = pVertices;
    a5.x = a4[i].x;
    a5.y = a4[i].y;
    a5.z = a4[i].z;
    pGame->pStru9Instance->CalcPortalShape(v15, *pOutNumVertices, v14, pOutNumVertices, &a5, a4[i].dot, (char *)&a7a, _unused);
    //v12 = *pOutNumVertices;
    if (*pOutNumVertices < v11)
    {
      *pOutNumVertices = 0;
      return true;
    }
    //result = a6a;
    //v13 += 24;
    //if (++i >= uNumVertices)
      //
  }
  return a7a;
}

//----- (004371C3) --------------------------------------------------------
bool IndoorCameraD3D::_4371C3(RenderVertexSoft *pVertices, unsigned int *pOutNumVertices, int _unused)
{
  char *v4; // eax@2
  signed int v5; // ecx@2
  RenderVertexSoft *v6; // esi@5
  unsigned int *v7; // edi@5
  char *v8; // ecx@6
  int v9; // eax@6
  IndoorCameraD3D *thisa; // [sp+0h] [bp-Ch]@1
  signed int v12; // [sp+4h] [bp-8h]@5
  unsigned int pVerticesa; // [sp+14h] [bp+8h]@6
  unsigned int pOutNumVerticesa; // [sp+18h] [bp+Ch]@6

  thisa = this;

  
  static RenderVertexSoft static_4371C3_array_50E5E0[64];
  static bool __init_flag1 = false;
  if (!__init_flag1)
  {
    __init_flag1 = true;

    for (uint i = 0; i < 64; ++i)
      static_4371C3_array_50E5E0[i].flt_2C = 0.0f;
  }

  v12 = 0;
  v6 = pVertices;
  v7 = pOutNumVertices;
  if ( (signed int)*pOutNumVertices > 0 )
  {
    pOutNumVerticesa = (char *)static_4371C3_array_50E5E0 - (char *)pVertices;
    pVerticesa = (char *)&static_4371C3_array_50E5E0[0].vWorldViewProjY - (char *)pVertices;
    v8 = (char *)&static_4371C3_array_50E5E0[0].vWorldPosition.y;
    v9 = (int)&v6->vWorldPosition.z;
    do
    {
      ++v12;
      *((int *)v8 - 1) = *(int *)(v9 - 8);
      *(int *)v8 = *(int *)(v9 - 4);
      v8 += 48;
      *(float *)(pOutNumVerticesa + v9) = *(float *)v9;
      *(float *)(pVerticesa + v9) = *(float *)(v9 + 28);
      *(float *)((char *)&static_4371C3_array_50E5E0[0]._rhw - (char *)v6 + v9) = *(float *)(v9 + 32);
      v9 += 48;
    }
    while ( v12 < (signed int)*v7 );
  }
  return CalcPortalShape(
           static_4371C3_array_50E5E0,
           v7,
           v6,
           thisa->std__vector_000034_prolly_frustrum,
           4,
           0,
           _unused);
}
// 50F1E0: using guessed type char static_sub_4371C3_byte_50F1E0_init_flags;

//----- (00437143) --------------------------------------------------------
int IndoorCameraD3D::_437143(unsigned int uNumInVertices, RenderVertexSoft *pOutVertices, RenderVertexSoft *pInVertices, unsigned int *pOutNumVertices)
{
  unsigned int v5; // edi@1
  char *pOutVertices_; // edx@2
  char *v7; // eax@2
  unsigned int v8; // ebx@2
  double v9; // st7@3
  double v10; // st6@3
  int result; // eax@5
  unsigned int a2a; // [sp+10h] [bp+Ch]@2

  v5 = uNumInVertices;
  if ( (signed int)uNumInVertices > 0 )
  {
    pOutVertices_ = (char *)&pOutVertices->vWorldViewProjY;
    v7 = (char *)&pInVertices->_rhw;
    v8 = (char *)pOutVertices - (char *)pInVertices;
    a2a = uNumInVertices;
    do
    {
      *(float *)v7 = 1.0 / (*((float *)v7 - 5) + 0.0000001);
      memcpy(pOutVertices_ - 28, v7 - 32, 0x30u);
      v9 = (double)pODMRenderParams->int_fov_rad * *(float *)&v7[v8];
      v10 = (double)pViewport->uScreenCenterX - v9 * *((float *)v7 - 4);
      v7 += 48;
      *((float *)pOutVertices_ - 1) = v10;
      *(float *)pOutVertices_ = (double)pViewport->uScreenCenterY - v9 * *((float *)v7 - 15);
      pOutVertices_ += 48;
      --a2a;
    }
    while ( a2a );
    v5 = uNumInVertices;
  }
  result = (int)pOutNumVertices;
  *pOutNumVertices = v5;
  return result;
}

//----- (00436F09) --------------------------------------------------------
void IndoorCameraD3D::_436F09_mess_with_lightmap__clipflag_4(RenderVertexSoft *pInVertices, int uNumInVertices, RenderVertexSoft *pOutVertices, unsigned int *pOutNumVertices)
{
  unsigned int *pOutNumVertices_; // ebx@1
  double v6; // st7@2
  signed int v7; // esi@2
  char *v8; // edx@5
  unsigned int v9; // eax@10
  RenderVertexSoft *v10; // ecx@11
  double v11; // st6@11
  double v12; // st6@12
  unsigned int v13; // edi@14
  unsigned __int8 v14; // c2@16
  unsigned __int8 v15; // c3@16
  unsigned int v16; // edi@17
  bool a1a; // [sp+Ch] [bp+8h]@7
  int a4a; // [sp+18h] [bp+14h]@5

  pOutNumVertices_ = pOutNumVertices;
  *pOutNumVertices = 0;
  if ( uNumInVertices )
  {
    memcpy(&pInVertices[uNumInVertices], pInVertices, sizeof(pInVertices[uNumInVertices]));
    v6 = (double)pODMRenderParams->shading_dist_mist;
    v7 = 0;
    if ( v6 >= pInVertices->vWorldViewPosition.x )
      v7 = 1;
    if ( uNumInVertices + 1 > 1 )
    {
      a4a = uNumInVertices;
      v8 = (char *)&pInVertices[1].u;
      do
      {
        a1a = v6 >= *((float *)v8 - 6);
        if ( v7 == a1a )
        {
          v10 = pOutVertices;
          goto LABEL_23;
        }
        v9 = *pOutNumVertices_;
        if ( a1a )
        {
          v10 = pOutVertices;
          v11 = (v6 - *((float *)v8 - 18)) / (*((float *)v8 - 6) - *((float *)v8 - 18));
          pOutVertices[v9].vWorldViewPosition.y = (*((float *)v8 - 5) - *((float *)v8 - 17)) * v11 + *((float *)v8 - 17);
          pOutVertices[*pOutNumVertices_].vWorldViewPosition.z = (*((float *)v8 - 4) - *((float *)v8 - 16)) * v11
                                                               + *((float *)v8 - 16);
          pOutVertices[*pOutNumVertices_].u = (*(float *)v8 - *((float *)v8 - 12)) * v11 + *((float *)v8 - 12);
          pOutVertices[*pOutNumVertices_].v = (*((float *)v8 + 1) - *((float *)v8 - 11)) * v11 + *((float *)v8 - 11);
        }
        else
        {
          v12 = (v6 - *((float *)v8 - 6)) / (*((float *)v8 - 18) - *((float *)v8 - 6));
          pOutVertices[v9].vWorldViewPosition.y = (*((float *)v8 - 17) - *((float *)v8 - 5)) * v12 + *((float *)v8 - 5);
          pOutVertices[*pOutNumVertices_].vWorldViewPosition.z = (*((float *)v8 - 16) - *((float *)v8 - 4)) * v12
                                                               + *((float *)v8 - 4);
          pOutVertices[*pOutNumVertices_].u = (*((float *)v8 - 12) - *(float *)v8) * v12 + *(float *)v8;
          pOutVertices[*pOutNumVertices_].v = (*((float *)v8 - 11) - *((float *)v8 + 1)) * v12 + *((float *)v8 + 1);
          v10 = pOutVertices;
        }
        v10[*pOutNumVertices_].vWorldViewPosition.x = v6;
        v10[*pOutNumVertices_]._rhw = 1.0 / v6;
        if ( v7 )
        {
          v13 = (unsigned int)&v10[*pOutNumVertices_];
          if ( *(float *)(v13 + 12) != *((float *)v8 - 18) || *(float *)(v13 + 16) != *((float *)v8 - 17) )
            goto LABEL_21;
          v14 = 0;
          v15 = *(float *)(v13 + 20) == *((float *)v8 - 16);
        }
        else
        {
          v16 = (unsigned int)&v10[*pOutNumVertices_];
          if ( *(float *)(v16 + 12) != *((float *)v8 - 6) || *(float *)(v16 + 16) != *((float *)v8 - 5) )
          {
LABEL_21:
            ++*pOutNumVertices_;
            goto LABEL_23;
          }
          v14 = 0;
          v15 = *(float *)(v16 + 20) == *((float *)v8 - 4);
        }
        if ( !(v15 | v14) )
          goto LABEL_21;
LABEL_23:
        if ( a1a )
        {
          memcpy(&v10[*pOutNumVertices_], v8 - 36, sizeof(v10[*pOutNumVertices_]));
          pOutVertices[*pOutNumVertices_]._rhw = 1.0 / (*((float *)v8 - 6) + 0.0000001);
          pOutVertices[(*pOutNumVertices_)++].flt_2C = *((float *)v8 + 2);
        }
        v7 = a1a;
        v8 += 48;
        --a4a;
      }
      while ( a4a );
    }
    if ( (signed int)*pOutNumVertices_ < 3 )
      *pOutNumVertices_ = 0;
  }
}

//----- (00436CDC) --------------------------------------------------------
void IndoorCameraD3D::_436CDC_mess_with_lightmap__clipflag_2(RenderVertexSoft *pInVertices, int uNumInVertices, RenderVertexSoft *pOutVertices, unsigned int *pOutNumVertices)
{
  signed int v5; // esi@2
  char *v6; // edx@5
  unsigned int v7; // eax@10
  RenderVertexSoft *v8; // ecx@11
  double v9; // st6@11
  double v10; // st6@12
  unsigned int v11; // edi@14
  unsigned __int8 v12; // c2@16
  unsigned __int8 v13; // c3@16
  unsigned int v14; // edi@17
  bool a1a; // [sp+Ch] [bp+8h]@7

  if ( uNumInVertices )
  {
    *pOutNumVertices = 0;
    memcpy(&pInVertices[uNumInVertices], pInVertices, sizeof(pInVertices[uNumInVertices]));
    v5 = 0;
    if ( pInVertices->vWorldViewPosition.x >= 8.0 )
      v5 = 1;
    if ( uNumInVertices + 1 > 1 )
    {
      v6 = (char *)&pInVertices[1].u;
      do
      {
        a1a = *((float *)v6 - 6) >= 8.0;
        if ( v5 == a1a )
        {
          v8 = pOutVertices;
          goto LABEL_23;
        }
        v7 = *pOutNumVertices;
        if ( a1a )
        {
          v8 = pOutVertices;
          v9 = (8.0 - *((float *)v6 - 18)) / (*((float *)v6 - 6) - *((float *)v6 - 18));
          pOutVertices[v7].vWorldViewPosition.y = (*((float *)v6 - 5) - *((float *)v6 - 17)) * v9 + *((float *)v6 - 17);
          pOutVertices[*pOutNumVertices].vWorldViewPosition.z = (*((float *)v6 - 4) - *((float *)v6 - 16)) * v9
                                                              + *((float *)v6 - 16);
          pOutVertices[*pOutNumVertices].u = (*(float *)v6 - *((float *)v6 - 12)) * v9 + *((float *)v6 - 12);
          pOutVertices[*pOutNumVertices].v = (*((float *)v6 + 1) - *((float *)v6 - 11)) * v9 + *((float *)v6 - 11);
        }
        else
        {
          v10 = (8.0 - *((float *)v6 - 6)) / (*((float *)v6 - 18) - *((float *)v6 - 6));
          pOutVertices[v7].vWorldViewPosition.y = (*((float *)v6 - 17) - *((float *)v6 - 5)) * v10 + *((float *)v6 - 5);
          pOutVertices[*pOutNumVertices].vWorldViewPosition.z = (*((float *)v6 - 16) - *((float *)v6 - 4)) * v10
                                                              + *((float *)v6 - 4);
          pOutVertices[*pOutNumVertices].u = (*((float *)v6 - 12) - *(float *)v6) * v10 + *(float *)v6;
          pOutVertices[*pOutNumVertices].v = (*((float *)v6 - 11) - *((float *)v6 + 1)) * v10 + *((float *)v6 + 1);
          v8 = pOutVertices;
        }
        v8[*pOutNumVertices].vWorldViewPosition.x = 8.0;
        v8[*pOutNumVertices]._rhw = 0.125;
        if ( v5 )
        {
          v11 = (unsigned int)&v8[*pOutNumVertices];
          if ( *(float *)(v11 + 12) != *((float *)v6 - 18) || *(float *)(v11 + 16) != *((float *)v6 - 17) )
            goto LABEL_21;
          v12 = 0;
          v13 = *(float *)(v11 + 20) == *((float *)v6 - 16);
        }
        else
        {
          v14 = (unsigned int)&v8[*pOutNumVertices];
          if ( *(float *)(v14 + 12) != *((float *)v6 - 6) || *(float *)(v14 + 16) != *((float *)v6 - 5) )
          {
LABEL_21:
            ++*pOutNumVertices;
            goto LABEL_23;
          }
          v12 = 0;
          v13 = *(float *)(v14 + 20) == *((float *)v6 - 4);
        }
        if ( !(v13 | v12) )
          goto LABEL_21;
LABEL_23:
        if ( a1a )
        {
          memcpy(&v8[*pOutNumVertices], v6 - 36, sizeof(v8[*pOutNumVertices]));
          pOutVertices[(*pOutNumVertices)++]._rhw = 1.0 / (*((float *)v6 - 6) + 0.0000001);
        }
        v5 = a1a;
        v6 += 48;
        --uNumInVertices;
      }
      while ( uNumInVertices );
    }
    if ( (signed int)*pOutNumVertices < 3 )
      *pOutNumVertices = 0;
  }
}

//----- (00436BB7) --------------------------------------------------------
void IndoorCameraD3D::Project(RenderVertexSoft *pVertices, unsigned int uNumVertices, char a4)
{
  double v7; // st7@7
  double v8; // st7@9
  double v9; // st6@10
  double v10; // st5@12
  double v11; // st7@16
  double v12; // st6@17
  double v13; // st5@19
  float uNumVerticesa; // [sp+14h] [bp+Ch]@13
  float uNumVerticesb; // [sp+14h] [bp+Ch]@20

  for (uint i = 0; i < uNumVertices; ++i)
  {
    if (byte_4D864C && pGame->uFlags & 0x80 || uCurrentlyLoadedLevelType == LEVEL_Indoor)
    {
      v7 = 1.0 / pVertices[i].vWorldViewPosition.x;

      pVertices[i].vWorldViewProjX = pVertices[i].vWorldViewPosition.y * fov * v7 + screenCenterX;
      pVertices[i].vWorldViewProjY = (signed int)pViewport->uViewportBR_Y - (pVertices[i].vWorldViewPosition.z * fov * v7 + screenCenterY);
    }
    else
    {
      extern void _outdoor_project(RenderVertexSoft *v);
      _outdoor_project(pVertices + i);
    }

      if ( a4 )
      {
//        __debugbreak();
        v8 = (double)(signed int)pViewport->uViewportBR_X;
        if ( v8 >= pVertices[i].vWorldViewProjX )
          v9 = pVertices[i].vWorldViewProjX;
        else
          v9 = v8;
        v10 = (double)(signed int)pViewport->uViewportTL_X;
        if ( v10 <= v9 )
        {
          if ( v8 >= pVertices[i].vWorldViewProjX)
            v8 = pVertices[i].vWorldViewProjX;
        }
        else
        {
          uNumVerticesa = v10;
          v8 = uNumVerticesa;
        }
        pVertices[i].vWorldViewProjX = v8;
        v11 = (double)(signed int)pViewport->uViewportBR_Y;
        if ( v11 >= pVertices[i].vWorldViewProjY)
          v12 = pVertices[i].vWorldViewProjY;
        else
          v12 = v11;
        v13 = (double)(signed int)pViewport->uViewportTL_Y;
        if ( v13 <= v12 )
        {
          if ( v11 >= pVertices[i].vWorldViewProjY)
            v11 = pVertices[i].vWorldViewProjY;
        }
        else
        {
          uNumVerticesb = v13;
          v11 = uNumVerticesb;
        }
        pVertices[i].vWorldViewProjY = v11;
      }
  }
}

//----- (00436A9A) --------------------------------------------------------
void IndoorCameraD3D::Project(signed int x, signed int y, signed int z, int *a5, int *a6)
{
  double v6; // ST00_8@2
  //double v7; // ST08_8@2
  //double v8; // ST00_8@2
  signed __int64 v9; // qtt@3
  int v10; // ST04_4@3
  float a2a; // [sp+18h] [bp+8h]@2
  float a2b; // [sp+18h] [bp+8h]@2

  if ( pRenderer->pRenderD3D )
  {
    v6 = 1.0 / (double)x;
    a2a = (double)y * fov * v6 + screenCenterX;
    //v7 = a2a + 6.7553994e15;
    *a5 = floorf(a2a + 0.5f);
    a2b = (double)z * fov * v6 + screenCenterY;
    //v8 = a2b + 6.7553994e15;
    *a6 = pViewport->uViewportBR_Y - floorf(a2b + 0.5f);
  }
  /*else
  {
    LODWORD(v9) = pBLVRenderParams->fov_rad_fixpoint << 16;
    HIDWORD(v9) = pBLVRenderParams->fov_rad_fixpoint >> 16;
    v10 = v9 / x;
    LODWORD(v9) = pBLVRenderParams->fov_rad_fixpoint << 16;
    HIDWORD(v9) = pBLVRenderParams->fov_rad_fixpoint >> 16;
    *a5 = pBLVRenderParams->uViewportCenterX
        - ((signed int)(((unsigned __int64)(v10 * (signed __int64)y) >> 16) + 32768) >> 16);
    *a6 = pBLVRenderParams->uViewportCenterY - ((signed int)(((unsigned __int64)(v9 / x * z) >> 16) + 32768) >> 16);
  }*/
}

//----- (00436A6D) --------------------------------------------------------
double IndoorCameraD3D::GetPolygonMinZ(RenderVertexSoft *pVertices, unsigned int uStripType)
{
  unsigned int v3; // edx@1
  double result; // st7@1
  float *v5; // ecx@2

  v3 = uStripType;
  result = 3.402823466385289e38;
  if ( (signed int)uStripType > 0 )
  {
    v5 = &pVertices->vWorldPosition.z;
    do
    {
      if ( *v5 < result )
        result = *v5;
      v5 += 12;
      --v3;
    }
    while ( v3 );
  }
  return result;
}

//----- (00436A24) --------------------------------------------------------
struct IDirect3DTexture2 *IndoorCameraD3D::LoadTextureAndGetHardwarePtr(char *Str1)
{
  return pBitmaps_LOD->pHardwareTextures[pBitmaps_LOD->LoadTexture(Str1)];
}

//----- (00436A40) --------------------------------------------------------
double IndoorCameraD3D::GetPolygonMaxZ(RenderVertexSoft *pVertex, unsigned int uStripType)
{
  unsigned int v3; // edx@1
  double result; // st7@1
  float *v5; // ecx@2

  v3 = uStripType;
  result = 1.1754944e-38;
  if ( (signed int)uStripType > 0 )
  {
    v5 = &pVertex->vWorldPosition.z;
    do
    {
      if ( *v5 > result )
        result = *v5;
      v5 += 12;
      --v3;
    }
    while ( v3 );
  }
  return result;
}



// -- new
// merged from IndoorCamera::Initialize2
//         and ODMRenderParams::RotationToInts
//         and BLVRenderParams::Reset
void IndoorCameraD3D::CalculateRotations(int camera_rot_x, int camera_rot_y)
{
  sRotationX = camera_rot_x;
  sRotationY = camera_rot_y;

  fRotationYSine = sin((3.141592653589793 + 3.141592653589793) * (double)sRotationY / 2048.0);
  fRotationYCosine = cos((3.141592653589793 + 3.141592653589793) * (double)sRotationY / 2048.0);
  if (byte_4D864C && pGame->uFlags & 0x80 || uCurrentlyLoadedLevelType == LEVEL_Indoor)
  {
    fRotationXSine = sin((3.141592653589793 + 3.141592653589793) * (double)-sRotationX / 2048.0);
    fRotationXCosine = cos((3.141592653589793 + 3.141592653589793) * (double)-sRotationX / 2048.0);

    int_sine_y   = stru_5C6E00->Sin( pGame->pIndoorCameraD3D->sRotationY);
    int_cosine_y = stru_5C6E00->Cos( pGame->pIndoorCameraD3D->sRotationY);
    int_sine_x   = stru_5C6E00->Sin(-pGame->pIndoorCameraD3D->sRotationX);
    int_cosine_x = stru_5C6E00->Cos(-pGame->pIndoorCameraD3D->sRotationX);
  }
  else
  {
    fRotationXSine = sin((3.141592653589793 + 3.141592653589793) * (double)sRotationX / 2048.0);
    fRotationXCosine = cos((3.141592653589793 + 3.141592653589793) * (double)sRotationX / 2048.0);

    int_sine_y   = stru_5C6E00->Sin(pGame->pIndoorCameraD3D->sRotationY);
    int_cosine_y = stru_5C6E00->Cos(pGame->pIndoorCameraD3D->sRotationY);
    int_sine_x   = stru_5C6E00->Sin(pGame->pIndoorCameraD3D->sRotationX);
    int_cosine_x = stru_5C6E00->Cos(pGame->pIndoorCameraD3D->sRotationX);
  }
}