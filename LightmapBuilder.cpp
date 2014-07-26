#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define _CRT_SECURE_NO_WARNINGS
#include "LightmapBuilder.h"
#include "Game.h"
#include "stru314.h"
#include "Outdoor.h"
#include "Outdoor_stuff.h"
#include "Log.h"
#include "ErrorHandling.h"

#include "Lights.h"

#include "mm7_data.h"



#include "stru9.h"




LightsStack_StationaryLight_ *pStationaryLightsStack = new LightsStack_StationaryLight_;
//StationaryLight pStationaryLights[400];
//int uNumStationaryLightsApplied; // weak
LightsStack_MobileLight_ *pMobileLightsStack = new LightsStack_MobileLight_;
//MobileLight pMobileLights[400];
//int uNumMobileLightsApplied;





//----- (0045DF13) --------------------------------------------------------
Lightmap::Lightmap()
{
  field_C18 = 0;
}

//----- (0045BB06) --------------------------------------------------------
LightmapBuilder::LightmapBuilder()
{
}

//----- (0045BC07) --------------------------------------------------------
bool LightmapBuilder::ApplyLights(stru320 *a2, stru154 *a3, unsigned int uNumVertices, RenderVertexSoft *a5, IndoorCameraD3D_Vec4 *a6, char uClipFlag)
{
  Vec3_int_ pos; // [sp+2Ch] [bp-40h]@21
  RenderVertexSoft *a9; // [sp+68h] [bp-4h]@8

  if (!uNumVertices)
    return false;

  static RenderVertexSoft static_69B140[64];

  a9 = a5;
  if (a6)
  {
    for (uint i = 0; i < uNumVertices; ++i)
      memcpy(static_69B140 + i, a5 + i, sizeof(RenderVertexSoft));

    //__debugbreak();
    if (pGame->pIndoorCameraD3D->_437376(a3, static_69B140, &uNumVertices) == 1)
    {
      if ( !uNumVertices )
        return false;
      a9 = static_69B140;
    }
  }

  static stru314 static_69B110;
  static_69B110.field_4.x = a3->face_plane.vNormal.x;
  static_69B110.field_4.y = a3->face_plane.vNormal.y;
  static_69B110.field_4.z = a3->face_plane.vNormal.z;
  static_69B110.dist = a3->face_plane.dist;
  if (!pGame->pIndoorCameraD3D->GetFacetOrientation(a3->polygonType, &static_69B110.field_4,
          &static_69B110.field_10, &static_69B110.field_1C))
  {
    MessageBoxW(nullptr, L"Error: Failed to get the facet orientation", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Light.cpp:119", 0);
    ExitProcess(0);
  }

  for (uint i = 0; i < a2->uNumLightsApplied; ++i)
  {
    pos.x = a2->_blv_lights_xs[i];
    pos.y = a2->_blv_lights_ys[i];
    pos.z = a2->_blv_lights_zs[i];

    uint uColorR = (uint)floorf(a2->_blv_lights_rs[i] * 255.0 + 0.5f) & 0xFF,
         uColorG = (uint)floorf(a2->_blv_lights_gs[i] * 255.0 + 0.5f) & 0xFF,
         uColorB = (uint)floorf(a2->_blv_lights_bs[i] * 255.0 + 0.5f) & 0xFF;
    uint uColor = (uColorR << 16) | (uColorG << 8) | uColorB;
    if (!uColor)
      uColor = 0x00FFFFF;

    if (!_45BE86_build_light_polygon(&pos, a2->_blv_lights_radii[i], uColor, a2->_blv_lights_light_dot_faces[i],
              a2->_blv_lights_types[i], &static_69B110, uNumVertices, a9, uClipFlag) )
    {
      MessageBoxW(nullptr, L"Error: Failed to build light polygon", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Light.cpp:138", 0);
    }
  }
  return true;
}

//----- (0045BE86) --------------------------------------------------------
bool LightmapBuilder::_45BE86_build_light_polygon(Vec3_int_ *pos, float radius, unsigned int uColorMask, float dot_dist, int uLightType, stru314 *a7, unsigned int uNumVertices, RenderVertexSoft *a9, char uClipFlag)
{
  Lightmap *v11; // edi@3
  double v17; // st7@5
  double v24; // st7@6
  double v38; // st7@14
  double v39; // st7@16
  double v40; // st7@16
  int v45; // eax@24

  if (fabsf(radius) < 1e-6f)
    return true;

  v11 = uLightType & 1 ? &std__vector_000004[std__vector_000004_size] :
                         &std__vector_183808[std__vector_183808_size];
  flt_3C8C24 = radius - dot_dist;
  flt_3C8C28 = sqrt((radius + dot_dist) * (radius - dot_dist));
  flt_3C8C2C_lightmaps_brightness = 1.0 - (radius - flt_3C8C28) / radius;
  v11->field_C08 = (double)pos->x - dot_dist * a7->field_4.x;
  v11->field_C0A = (double)pos->y - dot_dist * a7->field_4.y;
  v11->field_C0C = (double)pos->z - dot_dist * a7->field_4.z;

  v17 = radius * flt_3C8C2C_lightmaps_brightness;
  flt_3C8C30 = v17;
  flt_3C8C0C = v17 * a7->field_10.x;
  flt_3C8C10 = v17 * a7->field_10.y;
  flt_3C8C14 = v17 * a7->field_10.z;
  flt_3C8C18 = v17 * a7->field_1C.x;
  flt_3C8C1C = v17 * a7->field_1C.y;
  flt_3C8C20 = v17 * a7->field_1C.z;

  /*
  v11->pVertices[0].vWorldPosition.x = v11->field_C08 - flt_3C8C18 + flt_3C8C0C;
  v11->pVertices[0].vWorldPosition.y = v11->field_C0A - flt_3C8C1C + flt_3C8C10;
  v11->pVertices[0].vWorldPosition.z = v11->field_C0C - flt_3C8C20 + flt_3C8C14;
  v11->pVertices[0].u = 0.0;
  v11->pVertices[0].v = 0.0;

  v11->pVertices[1].vWorldPosition.x = v11->field_C08 - flt_3C8C18 - flt_3C8C0C;
  v11->pVertices[1].vWorldPosition.y = v11->field_C0A - flt_3C8C1C - flt_3C8C10;
  v11->pVertices[1].vWorldPosition.z = v11->field_C0C - flt_3C8C20 - flt_3C8C14;
  v11->pVertices[1].u = 0.0;
  v11->pVertices[1].v = 1.0;

  v11->pVertices[2].vWorldPosition.x = v11->field_C08 + flt_3C8C18 - flt_3C8C0C;
  v11->pVertices[2].vWorldPosition.y = v11->field_C0A + flt_3C8C1C - flt_3C8C10;
  v11->pVertices[2].vWorldPosition.z = v11->field_C0C + flt_3C8C20 - flt_3C8C14;
  v11->pVertices[2].u = 1.0;
  v11->pVertices[2].v = 1.0;

  v11->pVertices[3].vWorldPosition.x = v11->field_C08 + flt_3C8C18 + flt_3C8C0C;
  v11->pVertices[3].vWorldPosition.y = v11->field_C0A + flt_3C8C1C + flt_3C8C10;
  v11->pVertices[3].vWorldPosition.z = v11->field_C0C + flt_3C8C20 + flt_3C8C14;
  v11->pVertices[3].u = 1.0;
  v11->pVertices[3].v = 0.0;
  */

  for (uint i = 0; i < 4; ++i)
  {
    v11->pVertices[i].vWorldPosition.x = v11->field_C08 - flt_3C8C18 + flt_3C8C0C;
    v11->pVertices[i].vWorldPosition.y = v11->field_C0A - flt_3C8C1C + flt_3C8C10;
    v11->pVertices[i].vWorldPosition.z = v11->field_C0C - flt_3C8C20 + flt_3C8C14;
    v11->pVertices[i].u = 0.0;
    v11->pVertices[i].v = 0.0;

    v24 = a7->field_4.y * v11->pVertices[i].vWorldPosition.y
        + a7->field_4.z * v11->pVertices[i].vWorldPosition.z
        + a7->field_4.x * v11->pVertices[i].vWorldPosition.x
        + a7->dist;

    v11->pVertices[i].vWorldPosition.x -= v24 * a7->field_4.x;
    v11->pVertices[i].vWorldPosition.y -= v24 * a7->field_4.y;
    v11->pVertices[i].vWorldPosition.z -= v24 * a7->field_4.z;
  }

  v11->uColorMask = uColorMask;
  v11->uNumVertices = 4;

  if (~pGame->uFlags2 & 4)
    v11->fBrightness = flt_3C8C2C_lightmaps_brightness;
  else
  {
    Vec3_float_ a1; // [sp+2Ch] [bp-20h]@8
    a1.x = (double)pos->x - v11->field_C08;
    a1.y = (double)pos->y - v11->field_C0A;
    a1.z = (double)pos->z - v11->field_C0C;
    a1.Normalize();

    auto dist_x = abs(pos->x - v11->field_C08), //v31
         dist_y = abs(pos->y - v11->field_C0A), //v32  arg0a
         dist_z = abs(pos->z - v11->field_C0C); //v33  _v64
    v38 = int_get_vector_length(dist_x, dist_y, dist_z);
    if (v38 > radius)
      return true;
    //radius = (1 / radius) * v38;
    if ( uLightType & 4 )
    {
      //v59 = (void *)v37;
      //uLightType = flt_4D86CC;
      //v58 = v37;
      v39 = fabs(a1.x * a7->field_4.x + a1.z * a7->field_4.z + a1.y * a7->field_4.y);
      v40 = v39 * 1.0 * flt_4D86CC;

      v11->fBrightness = v40 - (1 / radius) * v38 * v40;
    }
    else if ( uLightType & 8 )
    {
        v40 = 1.0 * 1.0;
        v11->fBrightness = v40 - (1 / radius) * v38;
    }
    else
        MessageBoxW(nullptr, L"Invalid light type!", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Light.cpp:277", 0);
  }

  if (!pGame->pStru9Instance->_4980B9(a9, uNumVertices,
                                      a7->field_4.x, a7->field_4.y, a7->field_4.z,
                                      v11->pVertices, &v11->uNumVertices))
    return false;

  //v44 = &v11->uNumVertices;
  if (!v11->uNumVertices)
    return true;

  v45 = _45C6D6(uNumVertices, a9, v11);
  if ( v45 != uNumVertices && v45 > 0 )
    _45C4B9(uNumVertices, a9, v11);
  //v59 = v11->uNumVertices;
  //v46 = (RenderVertexSoft *)pLightmapVertices_;
  pGame->pIndoorCameraD3D->ViewTransform(v11->pVertices, v11->uNumVertices);
  //v59 = 0;
  //v58 = v11->uNumVertices;
  pGame->pIndoorCameraD3D->Project(v11->pVertices, v11->uNumVertices, 0);

  unsigned int _a4 = 0;
  if ( !(uClipFlag & 1) )
    _a4 = 1;
  else if ( uCurrentlyLoadedLevelType == LEVEL_Outdoor)
  {
    if ( uClipFlag & 2 )
    {
      //v59 = &a4;
      //v58 = (unsigned int)field_3C8C34;
      //v57 = *v44;
      pGame->pIndoorCameraD3D->_436CDC_mess_with_lightmap__clipflag_2(v11->pVertices, v11->uNumVertices, field_3C8C34, &_a4);

      //v59 = v44;
      //v58 = (unsigned int)field_3C8C34;
      //v57 = (int)v46;
      //v56 = a4;
      pGame->pIndoorCameraD3D->_437143(_a4, v11->pVertices, field_3C8C34, &v11->uNumVertices);
    }
    else if ( uClipFlag & 4 )
    {
      //v59 = &a4;
      //v58 = (unsigned int)field_3C8C34;
      //v57 = *v44;
      pGame->pIndoorCameraD3D->_436F09_mess_with_lightmap__clipflag_4(v11->pVertices, v11->uNumVertices, field_3C8C34, &_a4);

      //v59 = v44;
      //v58 = (unsigned int)field_3C8C34;
      //v57 = (int)v46;
      //v56 = a4;
      pGame->pIndoorCameraD3D->_437143(_a4, v11->pVertices, field_3C8C34, &v11->uNumVertices);
    }
    else
      MessageBoxW(nullptr, L"Undefined clip flag specified", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Light.cpp:330", 0);
  }
  else
    MessageBoxW(nullptr, L"Lightpoly builder native indoor clipping not implemented", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Light.cpp:335", 0);

  if (_a4)
  {
    if (uLightType & 1)
    {
      //v48 = (char *)&std__vector_000004_size;
      //v49 = std__vector_000004_size;
      //v51 = __OFSUB__(std__vector_000004_size, 511);
      //v50 = std__vector_000004_size - 511 < 0;
      if (std__vector_000004_size < 512 - 1)
        ++std__vector_000004_size;
    }
    else
    {
      //v48 = (char *)&std__vector_183808_size;
      //v49 = std__vector_183808_size;
      //v51 = __OFSUB__(std__vector_183808_size, 767);
      //v50 = std__vector_183808_size - 767 < 0;
      if (std__vector_183808_size < 768 - 1)
        ++std__vector_183808_size;
    }
    //if ( v50 ^ v51 )
    //  *(unsigned int *)v48 = v49 + 1;
  }
  return true;
}

//----- (0045C4B9) --------------------------------------------------------
int LightmapBuilder::_45C4B9(int a2, RenderVertexSoft *a3, Lightmap *pLightmap)
{
  Lightmap *v4; // edi@1
  int v5; // eax@1
  signed int v6; // ecx@1
  RenderVertexSoft *v7; // ebx@2
  int v8; // edx@3
  RenderVertexSoft *v9; // esi@3
  int v10; // eax@6
//  char *v11; // edi@7
  RenderVertexSoft *v12; // ecx@8
  char v13; // bl@17
  signed int v14; // edx@17
  double v15; // st6@18
  double v16; // st6@21
  double v17; // st6@24
  signed int v18; // edx@33
  int v20; // [sp+4h] [bp-1Ch]@3
  //int v21; // [sp+8h] [bp-18h]@8
  float v22; // [sp+Ch] [bp-14h]@23
  float v23; // [sp+10h] [bp-10h]@20
  int v24; // [sp+14h] [bp-Ch]@1
  RenderVertexSoft *v25; // [sp+18h] [bp-8h]@2
  char v26; // [sp+1Eh] [bp-2h]@17
  char v27; // [sp+1Fh] [bp-1h]@17

  //__debugbreak();//Ritor1: needed cleaning

  v4 = pLightmap;
  v5 = 0;
  v6 = pLightmap->uNumVertices;
  v24 = 0;
  if ( v6 > 0 )
  {
    v7 = pLightmap->pVertices;
    v25 = pLightmap->pVertices;
    do
    {
      v20 = v5 + 1;
      v8 = (v5 + 1) % v6;
      v9 = &v4->pVertices[v8];
      if ( v7->vWorldPosition.x != v9->vWorldPosition.x
        || v7->vWorldPosition.y != v4->pVertices[v8].vWorldPosition.y
        || v7->vWorldPosition.z != v4->pVertices[v8].vWorldPosition.z )
      {
        //v10 = 0;
        if ( a2 > 0 )
        {
          //v11 = (char *)&a3->vWorldPosition.z;
          for ( v10 = 1; v10 <= a2; ++v10 )
          {
            //v21 = v10 + 1;
            v12 = &a3[v10 % a2];
            if ((a3[v10].vWorldPosition.x != v12->vWorldPosition.x
              || a3[v10].vWorldPosition.y != v12->vWorldPosition.y || a3[v10].vWorldPosition.z != v12->vWorldPosition.z)
              && v7->vWorldPosition.x == a3[v10].vWorldPosition.x
              && v7->vWorldPosition.y == a3[v10].vWorldPosition.y && v7->vWorldPosition.z == a3[v10].vWorldPosition.z
              && (v9->vWorldPosition.x != v12->vWorldPosition.x
               || v9->vWorldPosition.y != v12->vWorldPosition.y || v9->vWorldPosition.z != v12->vWorldPosition.z) )
            {
              v13 = 0;
              v14 = 0;
              v27 = 0;
              v26 = 0;
              if ( v9->vWorldPosition.x <= (double)v12->vWorldPosition.x )
                v15 = v12->vWorldPosition.x - v9->vWorldPosition.x;
              else
                v15 = v9->vWorldPosition.x - v12->vWorldPosition.x;
              v23 = v15;

              if ( v9->vWorldPosition.y <= (double)v12->vWorldPosition.y )
                v16 = v12->vWorldPosition.y - v9->vWorldPosition.y;
              else
                v16 = v9->vWorldPosition.y - v12->vWorldPosition.y;
              v22 = v16;

              if ( v9->vWorldPosition.z <= (double)v12->vWorldPosition.z )
                v17 = v12->vWorldPosition.z - v9->vWorldPosition.z;
              else
                v17 = v9->vWorldPosition.z - v12->vWorldPosition.z;
              if ( v23 < 1.0 )
              {
                v13 = 1;
                v14 = 1;
              }
              if ( v22 < 1.0 )
              {
                v27 = 1;
                ++v14;
              }
              if ( v17 < 1.0 )
              {
                v26 = 1;
                ++v14;
              }
              if ( v14 > 1 )
              {
                v18 = 0;
                if ( v13 && v9->vWorldPosition.x != v12->vWorldPosition.x )
                {
                  v18 = 1;
                  v9->vWorldPosition.x = v12->vWorldPosition.x;
                }
                if ( v27 && v9->vWorldPosition.y != v12->vWorldPosition.y )
                {
                  ++v18;
                  v9->vWorldPosition.y = v12->vWorldPosition.y;
                }
                if ( v26 && v9->vWorldPosition.z != v12->vWorldPosition.z )
                {
                  ++v18;
                  v9->vWorldPosition.z = v12->vWorldPosition.z;
                }
                if ( v18 > 0 )
                  ++v24;
              }
              v7 = v25;
            }
            //++v10;
            //v11 += 48;
          }
          //while ( v21 < a2 );
          v4 = pLightmap;
        }
      }
      v5 = v20;
      v6 = v4->uNumVertices;
      ++v7;
      v25 = v7;
    }
    while ( v20 < v6 );
  }
  return v24;
}

//----- (0045C6D6) --------------------------------------------------------
int LightmapBuilder::_45C6D6(int a2, RenderVertexSoft *a3, Lightmap *pLightmap)
{
  signed int v6; // esi@1
  double v9; // st7@6
  double v10; // st6@10
  double v11; // st5@14
  double v12; // st7@17
  int v15; // [sp+Ch] [bp-8h]@1
  float v16; // [sp+10h] [bp-4h]@1

  v6 = -1;
  v16 = 3.4028235e38;
  v15 = 0;
  if ( (signed int)pLightmap->uNumVertices > 0 )
  {
    for ( uint i = 0; i < (signed int)pLightmap->uNumVertices; ++i )
    {
      if ( a2 > 0 )
      {
        for ( uint j = 0; j < a2; ++j )
        {
          if ( pLightmap->pVertices[i].vWorldPosition.x <= (double)a3[j].vWorldPosition.x )
            v9 = a3[j].vWorldPosition.x - pLightmap->pVertices[i].vWorldPosition.x;
          else
            v9 = pLightmap->pVertices[i].vWorldPosition.x - a3[j].vWorldPosition.x;
          if ( v9 < 2.0 )
          {
            v10 = pLightmap->pVertices[i].vWorldPosition.y <= (double)a3[j].vWorldPosition.y
                ? a3[j].vWorldPosition.y - pLightmap->pVertices[i].vWorldPosition.y
                : pLightmap->pVertices[i].vWorldPosition.y - a3[j].vWorldPosition.y;
            if ( v10 < 2.0 )
            {
              v11 = pLightmap->pVertices[i].vWorldPosition.z <= (double)a3[j].vWorldPosition.z
                  ? a3[j].vWorldPosition.z - pLightmap->pVertices[i].vWorldPosition.z
                  : pLightmap->pVertices[i].vWorldPosition.z - a3[j].vWorldPosition.z;
              if ( v11 < 2.0 )
              {
                v12 = v9 + v11 + v10;
                if ( v12 < v16 )
                {
                  v16 = v12;
                  v6 = j;
                }
              }
            }
          }
        }
        if ( v6 != -1 )
        {
          ++v15;
          pLightmap->pVertices[i].vWorldPosition.x = a3[v6].vWorldPosition.x;
          pLightmap->pVertices[i].vWorldPosition.y = a3[v6].vWorldPosition.y;
          pLightmap->pVertices[i].vWorldPosition.z = a3[v6].vWorldPosition.z;
        }
      }
      v6 = -1;
      v16 = 3.4028235e38;
    }
  }
  return v15;
}

//----- (0045C7F6) --------------------------------------------------------
bool LightmapBuilder::ApplyLights_IndoorFace(unsigned int uFaceID)
{
  BLVFace* pFace = &pIndoor->pFaces[uFaceID];
  BLVSector* pSector = &pIndoor->pSectors[pFace->uSectorID];

  stru_F8AD28.uCurrentAmbientLightLevel = (stru_F8AD28.uDefaultAmbientLightLevel + pSector->uMinAmbientLightLevel) << 16;

  uint uNumLightsApplied = 0;
  for (uint i = 0; i < pMobileLightsStack->uNumLightsActive; ++i)
  {
    if (uNumLightsApplied >= 20)
      break;

    ApplyLight_BLV((StationaryLight *)(pMobileLightsStack->pLights + i), pFace, &uNumLightsApplied, true, 0);
  }

  for (uint i = 0; i < pSector->uNumLights; ++i)
  {
    if (uNumLightsApplied >= 20 )
      break;

    BLVLightMM7* pLight = &pIndoor->pLights[pSector->pLights[i]];
    if (~pLight->uAtributes & 0x08)
      ApplyLight_BLV((StationaryLight *)pLight, pFace, &uFaceID, false, &_4E94D0_light_type);
  }

  for (uint i = 0; i < pStationaryLightsStack->uNumLightsActive; ++i)
  {
    if (uNumLightsApplied >= 20)
      break;

    ApplyLight_BLV(pStationaryLightsStack->pLights + i, pFace, &uNumLightsApplied, false, &_4E94D0_light_type);
  }

  stru_F8AD28.uNumLightsApplied = uNumLightsApplied;
  return true;
}

//----- (0045C911) --------------------------------------------------------
bool LightmapBuilder::ApplyLight_BLV(StationaryLight *pLight, BLVFace *a2, unsigned int *pSlot, bool bLightBackfaces, char *a5)
{
  double v13; // st7@8

  if (!pLight->uRadius)
    return false;

  if (pLight->vPosition.x > a2->pBounding.x1 - pLight->uRadius &&
      pLight->vPosition.x < a2->pBounding.x2 + pLight->uRadius &&
      pLight->vPosition.y > a2->pBounding.y1 - pLight->uRadius &&
      pLight->vPosition.y < a2->pBounding.y2 + pLight->uRadius &&
      pLight->vPosition.z > a2->pBounding.z1 - pLight->uRadius &&
      pLight->vPosition.z < a2->pBounding.z2 + pLight->uRadius)
  {
    v13 = (double)pLight->vPosition.z * a2->pFacePlane.vNormal.z +
          (double)pLight->vPosition.y * a2->pFacePlane.vNormal.y +
          (double)pLight->vPosition.x * a2->pFacePlane.vNormal.x +
          a2->pFacePlane.dist;
    if ((bLightBackfaces || v13 >= 0.0f) && fabsf(v13) <= pLight->uRadius)
    {
      unsigned int slot = *pSlot;

      stru_F8AD28._blv_lights_radii[slot] = pLight->uRadius;
      stru_F8AD28._blv_lights_inv_radii[slot] = 65536 / pLight->uRadius;
      stru_F8AD28._blv_lights_xs[slot] = pLight->vPosition.x;
      stru_F8AD28._blv_lights_ys[slot] = pLight->vPosition.y;
      stru_F8AD28._blv_lights_zs[slot] = pLight->vPosition.z;
      stru_F8AD28._blv_lights_rs[slot] = (double)pLight->uLightColorR / 255.0f;
      stru_F8AD28._blv_lights_gs[slot] = (double)pLight->uLightColorG / 255.0f;
      stru_F8AD28._blv_lights_bs[slot] = (double)pLight->uLightColorB / 255.0f;
      stru_F8AD28._blv_lights_light_dot_faces[slot] = abs((int)floorf(v13 + 0.5f));
      stru_F8AD28._blv_lights_types[slot] = pLight->uLightType;

      *pSlot += 1;
      return true;
    }
  }

  return false;
}

//----- (0045CA88) --------------------------------------------------------
int LightmapBuilder::_45CA88(stru320 *a2, RenderVertexSoft *a3, int a4, Vec3_float_ *pNormal)
{
  int result; // eax@1
  stru320 *v6; // ecx@2
  RenderVertexSoft *v7; // ebx@2
  double v8; // st7@2
  double v9; // st6@2
  char *v10; // eax@3
  double v11; // st7@5
  __int64 v12; // ST2C_8@5
  float v13; // edx@5
  int v14; // eax@5
  float v15; // ST10_4@5
  Vec3_float_ v16; // ST00_12@5
  double v17; // st7@5
  int a5; // [sp+2Ch] [bp-1Ch]@1
  float v19; // [sp+30h] [bp-18h]@1
  float v20; // [sp+34h] [bp-14h]@1
  //LightmapBuilder *thisa; // [sp+38h] [bp-10h]@1
  int v22; // [sp+3Ch] [bp-Ch]@1
  int *j; // [sp+40h] [bp-8h]@3
  int i; // [sp+44h] [bp-4h]@1
  int a3a; // [sp+58h] [bp+10h]@2

  __debugbreak();//Ritor1: needed cleaning

  *(float *)&a5 = 0.0;
  v19 = 0.0;
  //thisa = this;
  v20 = 0.0;
  result = _45CBD4(a3, a4, dword_69B010.data(), &v22);
  for ( i = 0; i < v22; result = i )
  {
    v6 = a2;
    a3a = 0;
    v7 = &a3[dword_69B010[i]];
    v8 = v7->vWorldPosition.z;
    v9 = v7->vWorldPosition.y;
    *(float *)&a5 = v7->vWorldPosition.x;
    v19 = v9;
    v20 = v8;
    v7->flt_2C = 0.0;
    if ( a2->uNumLightsApplied > 0 )
    {
      v10 = (char *)a2->_blv_lights_ys;
      for ( j = a2->_blv_lights_ys; ; v10 = (char *)j )
      {
        v11 = (double)*((signed int *)v10 - 60);
        LODWORD(v12) = *((unsigned int *)v10 - 20);
        HIDWORD(v12) = *(unsigned int *)v10;
        LODWORD(v13) = *((unsigned int *)v10 + 60);
        v14 = a3a;
        LOBYTE(v14) = v6->_blv_lights_types[a3a];
        v15 = v11;
        *(_QWORD *)&v16.x = v12;
        v16.z = v13;
        v17 = _45CC0C_light(v16/*COERCE_VEC3_FLOAT_(v16.x)*/, 1.0, v15, pNormal, *(float *)&a5/*COERCE_FLOAT(&a5)*/, v14)
            + v7->flt_2C;
        ++a3a;
        ++j;
        v7->flt_2C = v17;
        if ( a3a >= a2->uNumLightsApplied )
          break;
        v6 = a2;
      }
    }
    ++i;
  }
  return result;
}

//----- (0045CB89) --------------------------------------------------------
int LightmapBuilder::_45CB89(RenderVertexSoft *a1, int a2)
{
  int v3; // edx@1
  int result; // eax@2
  char *v5; // ecx@2
  double v6; // st7@4
  __int16 v7; // fps@4
  char v8; // c0@4
  char v9; // c2@4
  char v10; // c3@4
  double v11; // st7@5
  double v12; // st7@6
  __int16 v13; // fps@6
  char v14; // c0@6
  char v15; // c2@6
  char v16; // c3@6

  v3 = a2;
  if ( a2 > 0 )
  {
    HIWORD(result) = HIWORD(a1);
    v5 = (char *)&a1->flt_2C;
    do
    {
      __debugbreak(); // warning C4700: uninitialized local variable 'v7' used
      if ( *(float *)v5 < 0.0
        || (v6 = *(float *)v5, /*UNDEF(v7),*/ v8 = 1.0 < v6, v9 = 0, v10 = 1.0 == v6, LOWORD(result) = v7, v6 <= 1.0) )
      {
        v12 = *(float *)v5;
        //UNDEF(v13);
        v14 = 0.0 < v12;
        v15 = 0;
        v16 = 0.0 == v12;
      __debugbreak(); // warning C4700: uninitialized local variable 'v13' used
        LOWORD(result) = v13;
        if ( v12 >= 0.0 )
          v11 = *(float *)v5;
        else
          v11 = 0.0;
      }
      else
      {
        v11 = 1.0;
      }
      *(float *)v5 = v11;
      v5 += 48;
      --v3;
    }
    while ( v3 );
  }
  return result;
}

//----- (0045CBD4) --------------------------------------------------------
int LightmapBuilder::_45CBD4(RenderVertexSoft *a2, int a3, int *a4, int *a5)
{
  int result; // eax@1
  int v6; // edx@1
  int v7; // ecx@2
  int v8; // esi@2

  result = (int)a5;
  v6 = 0;
  for ( *a5 = 0; v6 < a3; ++v6 )
  {
    v7 = *a5;
    v8 = 0;
    if ( *a5 <= 0 )
    {
LABEL_5:
      a4[v7] = v6;
      ++*a5;
    }
    else
    {
      while ( a4[v8] != v6 )
      {
        ++v8;
        if ( v8 >= v7 )
          goto LABEL_5;
      }
    }
  }
  return result;
}

//----- (0045CC0C) --------------------------------------------------------
double LightmapBuilder::_45CC0C_light(Vec3_float_ a1, float a2, float a3, Vec3_float_ *pNormal, float a5, int uLightType)
{
  float v7; // esi@1
  int v8; // eax@1
//  int v9; // ebx@1
//  unsigned int v10; // ecx@1
//  int v11; // edx@2
//  int v12; // edx@4
//  unsigned int v13; // edx@6
  double v14; // st7@7
  double result; // st7@8
  double v16; // st7@9
  int v17; // esi@9
  const char *v18; // ecx@9
  double v19; // st7@10
  double v20; // st7@10
  std::string v21; // [sp-10h] [bp-40h]@13
  const char *v22[6]; // [sp+0h] [bp-30h]@10
  double v23; // [sp+18h] [bp-18h]@1
//  double v24; // [sp+20h] [bp-10h]@1
  int v25; // [sp+28h] [bp-8h]@1
  int v26; // [sp+2Ch] [bp-4h]@1

  v7 = a5;
  LODWORD(a5) = *(unsigned int *)(LODWORD(a5) + 8);
  //v24 = a5 + 6.7553994e15;
  v26 = floorf(a5 + 0.5f);//LODWORD(v24);
  LODWORD(a5) = *(unsigned int *)(LODWORD(v7) + 4);
  //v24 = a5 + 6.7553994e15;
  auto _v24 = floorf(a5 + 0.5f);
  LODWORD(a5) = *(unsigned int *)LODWORD(v7);
  //v23 = a5 + 6.7553994e15;
  auto _v23 = floorf(a5 + 0.5f);
  //*(_QWORD *)((char *)&v24 + 4) = __PAIR__(LODWORD(v24), LODWORD(v23));
  v26 = abs((signed)LODWORD(a1.z) - v26);
  //v25 = abs((signed)LODWORD(a1.y) - (signed)LODWORD(v24));
  //v8 = abs((int)a1.x - (signed)LODWORD(v23));
  v25 = abs((signed)LODWORD(a1.y) - (signed)_v24);
  v8 = abs((int)a1.x - (signed)_v23);
  v14 = int_get_vector_length(v26, v25, v8);
  if ( v14 <= a3 )
  {
    a5 = v14 / a3;
    v16 = (double)(signed int)a1.x;
    *(float *)&v23 = (double)SLODWORD(a1.y);
    LODWORD(a1.x) = *(unsigned int *)LODWORD(v7);
    v17 = LODWORD(v7) + 4;
    *((float *)&v23 + 1) = (double)SLODWORD(a1.z);
    LODWORD(a1.y) = *(unsigned int *)v17;
    LODWORD(a1.z) = *(unsigned int *)(v17 + 4);
    a3 = *((float *)&v23 + 1) - a1.z;
    a1.z = a3;
    a1.x = v16 - a1.x;
    a1.y = (float)v23 - a1.y;
    a1.Normalize();
    if ( uLightType & 4 )
    {
      __debugbreak(); // warning C4700: uninitialized local variable 'v18' used
      v22[1] = v18;
      uLightType = dword_4D86D8;
      v22[0] = v18;
      v19 = fabs(a1.z * pNormal->z + a1.y * pNormal->y + a1.x * pNormal->x);
      v20 = v19 * *(float *)&uLightType * a2;
    }
    else
    {
      if ( uLightType & 8 )
      {
        v20 = 1.3 * a2;
      }
      else
      {
        MessageBoxW(nullptr, L"Invalid light type detected!", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Light.cpp:783", 0);
        v20 = *(float *)&uLightType;
      }
    }
    result = v20 - a5 * v20;
  }
  else
  {
    result = 0.0;
  }
  return result;
}
// 4D86D8: using guessed type int dword_4D86D8;

//----- (0045CDB7) --------------------------------------------------------
bool LightmapBuilder::ApplyLights_OutdoorFace(ODMFace *pFace)
{
  int v3; // eax@1
  bool result; // eax@9
  int pSlot; // [sp+10h] [bp-4h]@1

  v3 = stru_F8AD28.uDefaultAmbientLightLevel + pFace->uShadeType;
  pSlot = 0;
  stru_F8AD28.uCurrentAmbientLightLevel = v3 << 16;
  for ( uint i = 0; i < pMobileLightsStack->uNumLightsActive; ++i )
  {
    if ( pSlot >= 20 )
      break;
    ApplyLight_ODM((StationaryLight *)pMobileLightsStack[i].pLights, pFace, (unsigned int *)&pSlot, 1);
  }
  for ( uint i = 0; i < pStationaryLightsStack->uNumLightsActive; ++i )
  {
    if ( pSlot >= 20 )
      break;
    ApplyLight_ODM(&pStationaryLightsStack->pLights[i], pFace, (unsigned int *)&pSlot, 0);
  }
  result = pSlot;
  stru_F8AD28.uNumLightsApplied = pSlot;
  return true;
}

//----- (0045CE50) --------------------------------------------------------
bool LightmapBuilder::ApplyLight_ODM(StationaryLight *pLight, ODMFace *pFace, unsigned int *pSlot, char a4)
{
  int result; // eax@0
  int v10; // ecx@8
  //unsigned int v12; // ebx@11
  //RenderD3D *v13; // ecx@11
  char v14; // dl@11

  //__debugbreak();

  result = (int)pFace;
  v10 = (pFace->pFacePlane.dist
              + pLight->vPosition.x * pFace->pFacePlane.vNormal.x
              + pLight->vPosition.y * pFace->pFacePlane.vNormal.y
              + pLight->vPosition.z * pFace->pFacePlane.vNormal.z) >> 16;
  if ( pLight->uRadius > 0
    && (pLight->vPosition.x > pFace->pBoundingBox.x1 - pLight->uRadius) && pLight->vPosition.x < pLight->uRadius + pFace->pBoundingBox.x2
    && (pLight->vPosition.y > pFace->pBoundingBox.y1 - pLight->uRadius) && pLight->vPosition.y < pLight->uRadius + pFace->pBoundingBox.y2
    && (pLight->vPosition.z > pFace->pBoundingBox.z1 - pLight->uRadius) && pLight->vPosition.z < pLight->uRadius + pFace->pBoundingBox.z2
    && ((a4) || v10 >= 0) && v10 <= pLight->uRadius )
  {
    stru_F8AD28._blv_lights_radii[*pSlot] = pLight->uRadius;
    stru_F8AD28._blv_lights_inv_radii[*pSlot] = 65536 / pLight->uRadius;
    stru_F8AD28._blv_lights_xs[*pSlot] = pLight->vPosition.x;
    stru_F8AD28._blv_lights_ys[*pSlot] = pLight->vPosition.y;
    stru_F8AD28._blv_lights_zs[*pSlot] = pLight->vPosition.z;
    stru_F8AD28._blv_lights_rs[*pSlot] = (double)pLight->uLightColorR * 0.0039215689;
    stru_F8AD28._blv_lights_gs[*pSlot] = (double)pLight->uLightColorG * 0.0039215689;
    stru_F8AD28._blv_lights_bs[*pSlot] = (double)pLight->uLightColorB * 0.0039215689;
    //v11 = abs(v10);
    //v12 = pRenderer->bUsingSpecular;
    stru_F8AD28._blv_lights_light_dot_faces[*pSlot] = abs(v10);
    stru_F8AD28._blv_lights_types[*pSlot] = pLight->uLightType;
    //v13 = pRenderer->pRenderD3D;
    v14 = stru_F8AD28._blv_lights_types[*pSlot];
    if ( /*pRenderer->pRenderD3D &&*/ pRenderer->bUsingSpecular && v14 & 4 )
      v14 = _4E94D2_light_type;
    stru_F8AD28._blv_lights_types[*pSlot] = v14;
    result = 4 * *pSlot;
    if ( /*v13*/true && pRenderer->bUsingSpecular )
    {
      if ( stru_F8AD28._blv_lights_types[*pSlot] & 4 )
      {
        *(float *)((char *)stru_F8AD28._blv_lights_rs + result) = *(float *)((char *)stru_F8AD28._blv_lights_rs + result)
                                                                * 0.33000001;
        *(float *)((char *)stru_F8AD28._blv_lights_gs + result) = *(float *)((char *)stru_F8AD28._blv_lights_gs + result)
                                                                * 0.33000001;
        *(float *)((char *)stru_F8AD28._blv_lights_bs + result) = *(float *)((char *)stru_F8AD28._blv_lights_bs + result)
                                                                * 0.33000001;
      }
    }
    ++*pSlot;
    LOBYTE(result) = 1;
  }
  else
    LOBYTE(result) = 0;
  return result;
}
// 4E94D2: using guessed type char _4E94D2_light_type;

//----- (0045D036) --------------------------------------------------------
bool LightmapBuilder::StackLights_TerrainFace(Vec3_float_ *pNormal, float *a2, RenderVertexSoft *a3, unsigned int uStripType, bool bLightBackfaces)
{
  /*int v6; // esi@1
  //LightmapBuilder *v7; // edi@1
  MobileLight *v8; // ebx@2
  int v9; // esi@5
  StationaryLight *v10; // ebx@6
  //bool result; // eax@9
  unsigned int a7; // [sp+Ch] [bp-4h]@1

  v6 = 0;
  //v7 = this;
  a7 = 0;
  stru_F8AD28.uCurrentAmbientLightLevel = pOutdoor->field_CBC_terrain_triangles_shade_type;
  if ( pMobileLightsStack->uNumLightsActive > 0 )
  {
    v8 = pMobileLightsStack->pLights;
    do
    {
      if ( (signed int)a7 >= 20 )
        break;
      StackLight_TerrainFace((StationaryLight *)v8, pNormal, a3, a1, uStripType, bLightBackfaces, &a7);
      ++v6;
      ++v8;
    }
    while ( v6 < pMobileLightsStack->uNumLightsActive );
  }
  v9 = 0;
  if ( pStationaryLightsStack->uNumLightsActive > 0 )
  {
    v10 = pStationaryLightsStack->pLights;
    do
    {
      if ( (signed int)a7 >= 20 )
        break;
      StackLight_TerrainFace(v10, pNormal, a3, a1, uStripType, bLightBackfaces, &a7);
      ++v9;
      ++v10;
    }
    while ( v9 < pStationaryLightsStack->uNumLightsActive );
  }

  stru_F8AD28.uNumLightsApplied = a7;
  return true;*/
//	bool __stdcall sub_45D036(struct Vec3<float> *pNormal, int a2, struct RenderVertex *a3, int a4, signed int X)
//{
//  float v6; // ebx@2
  unsigned int v10; // [sp+Ch] [bp-4h]@1
 int i;

  v10 = 0;
  stru_F8AD28.uCurrentAmbientLightLevel = pOutdoor->max_terrain_dimming_level;
  for (i = 0; i < pMobileLightsStack->uNumLightsActive; ++i)
  {
      if ( v10 >= 20 )
        break;
      StackLight_TerrainFace((StationaryLight *)&pMobileLightsStack->pLights[i], pNormal, a2, a3, uStripType, bLightBackfaces, &v10);
  }

  for (i = 0; i < pStationaryLightsStack->uNumLightsActive; ++i)
  {
      if ( v10 >= 20 )
        break;
      StackLight_TerrainFace(&pStationaryLightsStack->pLights[i], pNormal, a2, a3, uStripType, bLightBackfaces, &v10);
  }

  stru_F8AD28.uNumLightsApplied = v10;
  return true;

}
// 519AB4: using guessed type int uNumStationaryLightsApplied;

//----- (0045D0D5) --------------------------------------------------------
bool LightmapBuilder::StackLight_TerrainFace(StationaryLight *pLight, Vec3_float_ *pNormal, float *a3, RenderVertexSoft *a1, unsigned int uStripType, int X, unsigned int *pSlot)
{
  StationaryLight *v8; // edi@1
  bool result; // eax@1
//  unsigned int v10; // esi@2
  RenderVertexSoft *v11; // ebx@2
  float v12; // eax@3
  float v13; // eax@4
//  double v14; // st7@11
  double maxz; // st7@11
  double v16; // st7@11
  double v17; // st6@11
//  __int16 v18; // fps@11
//  char v19; // c0@11
  char v20; // c2@11
//  char v21; // c3@11
//  double v22; // st6@12
//  __int16 v23; // fps@12
//  char v24; // c0@12
//  char v25; // c2@12
//  char v26; // c3@12
//  double v27; // st7@13
//  double v28; // st6@13
//  __int16 v29; // fps@13
//  char v30; // c0@13
//  char v31; // c2@13
//  char v32; // c3@13
//  double v33; // st6@14
//  __int16 v34; // fps@14
//  char v35; // c0@14
//  char v36; // c2@14
//  char v37; // c3@14
//  double v38; // st7@15
//  double v39; // st6@15
//  __int16 v40; // fps@15
//  char v41; // c0@15
//  char v42; // c2@15
//  char v43; // c3@15
//  double v44; // st6@16
//  __int16 v45; // fps@16
//  char v46; // c0@16
//  char v47; // c2@16
//  char v48; // c3@16
  Vec3_float_ *v49; // esi@17
//  double v50; // st7@17
//  double v51; // st6@17
  signed int v52; // ecx@17
  unsigned int *v53; // esi@18
  int v54; // eax@18
  unsigned int v55; // ebx@18
  //RenderD3D *v56; // ecx@18
  char v57; // dl@18
  std::string v58; // [sp-18h] [bp-38h]@10
  const char *v59; // [sp-8h] [bp-28h]@10
  signed int v60; // [sp-4h] [bp-24h]@10
  double v61; // [sp+Ch] [bp-14h]@11
  float minz; // [sp+14h] [bp-Ch]@11
  int v63; // [sp+18h] [bp-8h]@1
  float v64; // [sp+1Ch] [bp-4h]@5

  v8 = pLight;
  result = pLight->uRadius;
  v63 = pLight->uRadius;
  if ( result <= 0 )
    return 0;
  //v10 = uStripType;
  v11 = a1;
  if ( uStripType == 4 )
  {
    pLight = (StationaryLight *)LODWORD(a1->vWorldPosition.x);
    v12 = a1[3].vWorldPosition.x;
  }
  else
  {
    if ( uStripType != 3 )
    {
      MessageBoxW(nullptr, L"Uknown strip type detected!", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Light.cpp:981", 0);
      goto LABEL_11;
    }
    if ( (unsigned char)X )
    {
      pLight = (StationaryLight *)LODWORD(a1->vWorldPosition.x);
      uStripType = LODWORD(a1[2].vWorldPosition.x);
      a1 = (RenderVertexSoft *)LODWORD(a1[1].vWorldPosition.y);
      v13 = v11[2].vWorldPosition.y;
      goto LABEL_5;
    }
    pLight = (StationaryLight *)LODWORD(a1[1].vWorldPosition.x);
    v12 = a1[2].vWorldPosition.x;
  }
  //uStripType = LODWORD(v12);
  a1 = (RenderVertexSoft *)LODWORD(a1[1].vWorldPosition.y);
  v13 = v11->vWorldPosition.y;
LABEL_5:
  v64 = v13;
LABEL_11:
  //v60 = v10;
  v59 = (const char *)v11;
  v60 = v12;//uStripType;
  minz = pGame->pIndoorCameraD3D->GetPolygonMinZ(v11, uStripType);
  maxz = pGame->pIndoorCameraD3D->GetPolygonMaxZ(v11, uStripType);
  result = v8->vPosition.x;
  *((float *)&v61 + 1) = maxz;
  v16 = (double)result;
  *(float *)&X = (double)v63;
  v17 = *(float *)&pLight - *(float *)&X;
  //UNDEF(v18);
  //v19 = v16 < v17;
  v20 = 0;
  //v21 = v16 == v17;
/*  BYTE1(result) = HIBYTE(v18);
  if ( v16 <= v17
    || (v22 = *(float *)&X + *(float *)&uStripType,
        //UNDEF(v23),
        v24 = v16 < v22,
        v25 = 0,
        v26 = v16 == v22,
        BYTE1(result) = HIBYTE(v23),
        v16 >= v22)
    || (result = v8->vPosition.y,
        pLight = (StationaryLight *)v8->vPosition.y,
        v27 = (double)(signed int)pLight,
        v28 = *(float *)&a1 - *(float *)&X,
        //UNDEF(v29),
        v30 = v27 < v28,
        v31 = 0,
        v32 = v27 == v28,
        BYTE1(result) = HIBYTE(v29),
        v27 <= v28)
    || (v33 = *(float *)&X + v64,
        //UNDEF(v34),
        v35 = v27 < v33,
        v36 = 0,
        v37 = v27 == v33,
        BYTE1(result) = HIBYTE(v34),
        v27 >= v33)
    || (result = v8->vPosition.z,
        pLight = (StationaryLight *)v8->vPosition.z,
        v38 = (double)(signed int)pLight,
        v39 = minz - *(float *)&X,
        //UNDEF(v40),
        v41 = v38 < v39,
        v42 = 0,
        v43 = v38 == v39,
        BYTE1(result) = HIBYTE(v40),
        v38 <= v39)
    || (v44 = *(float *)&X + *((float *)&v61 + 1),
        //UNDEF(v45),
        v46 = v38 < v44,
        v47 = 0,
        v48 = v38 == v44,
        BYTE1(result) = HIBYTE(v45),
        v38 >= v44)
    || (v49 = pNormal,
        Vec3_float_::NegDot(&v11->vWorldPosition, pNormal, a3),
        X = v8->vPosition.y,
        v50 = (double)X * v49->y,
        X = v8->vPosition.z,
        v51 = (double)X * v49->z,
        X = v8->vPosition.x,
        *(float *)&a3 = v50 + v51 + (double)X * v49->x + *a3,
        v61 = *(float *)&a3 + 6.7553994e15,
        result = LODWORD(v61),
        X = LODWORD(v61),
        v52 = v63,
        SLODWORD(v61) > v63) )*/
        v49 = pNormal;
        Vec3_float_::NegDot(&v11->vWorldPosition, pNormal, a3);
        *(float *)a3 = (double)v8->vPosition.x * v49->x
                      + (double)v8->vPosition.y * v49->y
                      + (double)v8->vPosition.z * v49->z + *a3;
        v61 = *(float *)a3 + 6.7553994e15;
        result = LODWORD(v61);
        X = LODWORD(v61);
        v52 = v63;
  if ( SLODWORD(v61) > v63)
    return 0;
  v53 = pSlot;
  v60 = X;
  stru_F8AD28._blv_lights_radii[*pSlot] = v63;
  stru_F8AD28._blv_lights_inv_radii[*v53] = 65536 / v63;
  stru_F8AD28._blv_lights_xs[*v53] = v8->vPosition.x;
  stru_F8AD28._blv_lights_ys[*v53] = v8->vPosition.y;
  stru_F8AD28._blv_lights_zs[*v53] = v8->vPosition.z;
  //a3 = (float *)v8->uLightColorR;
  stru_F8AD28._blv_lights_rs[*v53] = v8->uLightColorR / 255.0f;
  //a3 = (float *)v8->uLightColorG;
  stru_F8AD28._blv_lights_gs[*v53] = v8->uLightColorG / 255.0f;
  //a3 = (float *)v8->uLightColorB;
  stru_F8AD28._blv_lights_bs[*v53] = v8->uLightColorB / 255.0f;
  v54 = abs(v63);
  v55 = pRenderer->bUsingSpecular;
  stru_F8AD28._blv_lights_light_dot_faces[*v53] = v54;
  stru_F8AD28._blv_lights_types[*v53] = v8->uLightType;
  //v56 = pRenderer->pRenderD3D;
  v57 = stru_F8AD28._blv_lights_types[*v53];
  if ( /*pRenderer->pRenderD3D &&*/ v55 && v57 & 4 )
    v57 = _4E94D2_light_type;
  stru_F8AD28._blv_lights_types[*v53] = v57;
  result = 4 * *v53;
  if ( /*v56 &&*/ v55 )
  {
    if ( stru_F8AD28._blv_lights_types[*v53] & 4 )
    {
      stru_F8AD28._blv_lights_rs[result] = stru_F8AD28._blv_lights_rs[result] * 0.3300000131130219;
      stru_F8AD28._blv_lights_gs[result] = stru_F8AD28._blv_lights_gs[result] * 0.3300000131130219;
      stru_F8AD28._blv_lights_bs[result] = stru_F8AD28._blv_lights_bs[result] * 0.3300000131130219;
    }
  }
  ++*v53;
  return 1;
}

//----- (0045D698) --------------------------------------------------------
bool LightmapBuilder::DrawDebugOutlines(char bit_one_for_list1__bit_two_for_list2)
{
  bool result; // eax@1
  LightmapBuilder *v3; // esi@1
  RenderVertexSoft *v4; // edi@3
  RenderVertexSoft *v5; // edi@7
  IndoorCameraD3D *thisa; // [sp+10h] [bp-8h]@1
  bool v7; // [sp+14h] [bp-4h]@2
  bool a2a; // [sp+20h] [bp+8h]@6

  result = (bool)pGame->pIndoorCameraD3D;
  v3 = this;
  thisa = pGame->pIndoorCameraD3D;
  if ( bit_one_for_list1__bit_two_for_list2 & 1 )
  {
    v7 = 0;
    if ( (signed int)this->std__vector_000004_size > 0 )
    {
      v4 = this->std__vector_000004[0].pVertices;
      do
      {
        pGame->pIndoorCameraD3D->debug_outline_sw(v4, *((unsigned int *)v4 - 1), 0xFF00u, 0.0);
        ++v7;
        v4 = (RenderVertexSoft *)((char *)v4 + 3100);
        result = v7;
      }
      while ( v7 < (signed int)v3->std__vector_000004_size );
    }
  }
  if ( bit_one_for_list1__bit_two_for_list2 & 2 )
  {
    a2a = 0;
    if ( (signed int)v3->std__vector_183808_size > 0 )
    {
      v5 = v3->std__vector_183808[0].pVertices;
      do
      {
        pGame->pIndoorCameraD3D->debug_outline_sw(v5, *((unsigned int *)v5 - 1), 0xC04000u, 0.00019999999);
        ++a2a;
        v5 = (RenderVertexSoft *)((char *)v5 + 3100);
        result = a2a;
      }
      while ( a2a < (signed int)v3->std__vector_183808_size );
    }
  }
  LOBYTE(result) = 1;
  return result;
}

//----- (0045D73F) --------------------------------------------------------
void LightmapBuilder::DrawLightmapsType(int type)
{
  if (type == 2)
    Draw_183808_Lightmaps();
}

//----- (0045D74F) --------------------------------------------------------
bool LightmapBuilder::DrawLightmaps(int *indices)
{
  //char v3; // zf@1
//  IDirect3DDevice3 *v4; // eax@2
//  HRESULT v5; // eax@2
  //char *v6; // eax@2
  //struct IDirect3DTexture2 *v7; // edi@4
//  HRESULT v8; // eax@8
//  HRESULT v9; // eax@8
//  HRESULT v10; // eax@8
//  HRESULT v11; // eax@8
//  HRESULT v12; // eax@8
  //int *v13; // eax@8
  //float v14; // ecx@15
  //IDirect3DDevice3 *v15; // eax@21
  //HRESULT v16; // eax@21
  //IDirect3DDevice3 *v17; // eax@21
  //HRESULT v18; // eax@21
  //IDirect3DDevice3 *v19; // eax@21
  //HRESULT v20; // eax@21
  //IDirect3DDevice3 *v21; // eax@21
  //HRESULT v22; // eax@21
  //IDirect3DDevice3 *v23; // eax@23
  std::string v25; // [sp+44h] [bp-44h]@12
//  signed int v26; // [sp+48h] [bp-40h]@21
//  signed int v27; // [sp+4Ch] [bp-3Ch]@21
  Lightmap *v28; // [sp+50h] [bp-38h]@2
//  int v29; // [sp+54h] [bp-34h]@2
  //float v30; // [sp+58h] [bp-30h]@2
  Vec3_float_ arg4;
  //int arg4; // [sp+68h] [bp-20h]@8
  //float v32; // [sp+6Ch] [bp-1Ch]@8
  //float v33; // [sp+70h] [bp-18h]@8
//  int v34; // [sp+74h] [bp-14h]@19
//  int v35; // [sp+78h] [bp-10h]@2
  //int *v36; // [sp+7Ch] [bp-Ch]@10
  //int a1; // [sp+80h] [bp-8h]@12
  //float v38; // [sp+84h] [bp-4h]@1

  if (std__vector_000004_size == 0)
    return true;
  
  if (byte_4D864C && pGame->uFlags & GAME_FLAGS_1_01_lightmap_related)
    return true;

  pRenderer->BeginLightmaps();

      arg4.x = 1.0f;
      arg4.y = 1.0f;
      arg4.z = 1.0f;
      if (indices)
      {
        for (int* i = indices; *i != -1; ++i)
        {
          v28 = &std__vector_000004[*i];
          if ( !pRenderer->DrawLightmap(v28, &arg4, 0.0) )
            Error("Invalid lightmap detected! (%u)", *i);
        }
      }
      else
      {
        for (unsigned int i = 0; i < std__vector_000004_size; ++i)
        {
            Lightmap* _a1 = &std__vector_000004[(int)i];
            if ( !pRenderer->DrawLightmap(_a1, &arg4, 0.0) )
              Error("Invalid lightmap detected! (%u)", i);
        }
      }

  pRenderer->EndLightmaps();
  
  return true;
}


//----- (0045DA56) --------------------------------------------------------
bool LightmapBuilder::DoDraw_183808_Lightmaps(float z_bias)
{
  Vec3_float_ v; // [sp+Ch] [bp-1Ch]@2
  v.z = 1.0;
  v.y = 1.0;
  v.x = 1.0;
  
  for (uint i = 0; i < std__vector_183808_size; ++i)
    if (!pRenderer->DrawLightmap(std__vector_183808 + i, &v, z_bias))
      Error("Invalid lightmap detected! (%u)", i);

  return true;
}

//----- (0045DAE8) --------------------------------------------------------
bool Render::DrawLightmap(Lightmap *pLightmap, Vec3_float_ *pColorMult, float z_bias)
{
  double v10; // st7@4
  double v14; // st7@7
//  __int16 v15; // fps@8
  double v18; // st3@8
  signed int v24; // [sp-1Ch] [bp-670h]@13
//  const char *v25; // [sp-18h] [bp-66Ch]@13
//  int v26; // [sp-14h] [bp-668h]@13
  RenderVertexD3D3 a2[32]; // [sp+0h] [bp-654h]@7


  if (pLightmap->uNumVertices < 3)
  {
    Log::Warning(L"Lightmap uNumVertices < 3");
    return false;
  }

  uint uLightmapColorMaskR = (pLightmap->uColorMask >> 16) & 0xFF;
  uint uLightmapColorR = floorf(uLightmapColorMaskR * pLightmap->fBrightness * pColorMult->x + 0.5f);
  
  uint uLightmapColorMaskG = (pLightmap->uColorMask >> 8) & 0xFF;
  uint uLightmapColorG = floorf(uLightmapColorMaskG * pLightmap->fBrightness * pColorMult->y + 0.5f);
 
  uint uLightmapColorMaskB = pLightmap->uColorMask & 0xFF;
  uint uLightmapColorB = floorf(uLightmapColorMaskB * pLightmap->fBrightness * pColorMult->z + 0.5f);
 
  uint uLightmapColor = uLightmapColorB | (uLightmapColorMaskG << 8) | (uLightmapColorMaskR << 16);

  if (uCurrentlyLoadedLevelType == LEVEL_Outdoor)
    v10 = (double)pODMRenderParams->shading_dist_mist;
  else
    v10 = 16192.0;
  v14 = 1.0 / v10;

  for (uint i = 0; i < pLightmap->uNumVertices; ++i)
  {
    v18 = 1.0 - 1.0 / (v14 * pLightmap->pVertices[i].vWorldViewPosition.x * 1000.0);
    if (fabsf(z_bias) < 1e-5f)
    {
      v18 = v18 - z_bias;
      if (v18 < 0.000099999997)
        v18 = 0.000099999997;
    }

    a2[i].pos.x = pLightmap->pVertices[i].vWorldViewProjX;
    a2[i].pos.z = v18;
    a2[i].pos.y = pLightmap->pVertices[i].vWorldViewProjY;
    a2[i].rhw = 1.0 / pLightmap->pVertices[i].vWorldViewPosition.x;
    a2[i].diffuse = uLightmapColor;
    a2[i].specular = 0;
    a2[i].texcoord.x = pLightmap->pVertices[i].u;
    a2[i].texcoord.y = pLightmap->pVertices[i].v;
  }

  if (uCurrentlyLoadedLevelType == LEVEL_Indoor)
    v24 = D3DDP_DONOTLIGHT | D3DDP_DONOTCLIP | D3DDP_DONOTUPDATEEXTENTS;
   else
    v24 = D3DDP_DONOTLIGHT;

  ErrD3D(pRenderer->pRenderD3D->pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,
            D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1,
            a2,
            pLightmap->uNumVertices,
            v24));

  return true;
}

//----- (0045DCA9) --------------------------------------------------------
void LightmapBuilder::Draw_183808_Lightmaps()
{
  if (!std__vector_183808_size)
    return;

  pRenderer->BeginLightmaps2();

  DoDraw_183808_Lightmaps(0.00050000002);

  pRenderer->EndLightmaps2();
}