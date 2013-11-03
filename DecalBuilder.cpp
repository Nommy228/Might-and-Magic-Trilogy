#include "DecalBuilder.h"
#include "Game.h"
#include "Time.h"
#include "stru314.h"
#include "Outdoor.h"
#include "Log.h"

#include "mm7_data.h"

#include "stru9.h"

#include "Outdoor_stuff.h"


struct DecalBuilder *pDecalBuilder = new DecalBuilder;
struct BloodsplatContainer *pBloodsplatContainer = new BloodsplatContainer;


//----- (0043B570) --------------------------------------------------------
double DecalBuilder_stru0::_43B570_get_color_mult_by_time()
{
  double result; // st7@3
  
  if (field_1C_flags & 1)
  {
    if ((field_20_time - pEventTimer->Time() + 384) / 384.0 >= 0.0)
      result = (field_20_time - pEventTimer->Time() + 384) / 384.0;
    else
      result = 0.0;
  }
  else
  {
    result = 1.0;
  }
  return result;
}





//----- (0043B6EF) --------------------------------------------------------
bool BloodsplatContainer::AddBloodsplat(float x, float y, float z, float radius, char r, char g, char b)
{
  int *v8; // esi@1
  int v9; // eax@3
  Bloodsplat *v10; // eax@3
  int *result; // eax@3
  int v12; // ecx@3

  v8 = &this->uNumBloodsplats;
  if ( this->uNumBloodsplats == 64 )
    *v8 = 0;
  v9 = 5 * (*v8)++;
  v10 = &this->std__vector_pBloodsplats[8 * v9 / 0x28u];
  v10->x = x;
  v10->y = y;
  v10->z = z;
  v10->radius = radius;
  v10->r = r;
  v10->g = g;
  v10->b = b;
  result = (int *)&this->std__vector_pBloodsplats_size;
  v12 = this->std__vector_pBloodsplats_size + 1;
  if ( v12 > 64 )
    v12 = 64;
  *result = v12;
  LOBYTE(result) = 1;
  return (bool)result;
}

//----- (0049B490) --------------------------------------------------------
bool DecalBuilder::AddBloodsplat(float x, float y, float z, float r, float g, float b, float radius, int a8, int a9)
{
  //double v10; // ST1C_8@1
  char v11; // ST24_1@1
  //double v12; // ST1C_8@1
  //double v13; // ST1C_8@1
  int a7a; // [sp+40h] [bp+18h]@1
  //float arg14a; // [sp+44h] [bp+1Ch]@1
  //float arg14b; // [sp+44h] [bp+1Ch]@1
  //float arg14c; // [sp+44h] [bp+1Ch]@1

  /*arg14a = b * 255.0;
  v10 = arg14a + 6.7553994e15;
  v11 = LOBYTE(v10);
  arg14b = g * 255.0;
  v12 = arg14b + 6.7553994e15;
  a7a = LODWORD(v12);
  arg14c = r * 255.0;
  v13 = arg14c + 6.7553994e15;*/
  return pBloodsplatContainer->AddBloodsplat(x, y, z, radius,
                                             //SLOBYTE(v13), a7a, v11);
                                             r * 255.0f,
                                             g * 255.0f,
                                             b * 255.0f);
}

//----- (0049B525) --------------------------------------------------------
void DecalBuilder::Reset(unsigned int bPreserveBloodsplats)
{
  if ( !bPreserveBloodsplats )
  {
    pBloodsplatContainer->std__vector_pBloodsplats_size = 0;
    pBloodsplatContainer->uNumBloodsplats = 0;
  }
  std__vector_pDecals_size = 0;
}

//----- (0049B540) --------------------------------------------------------
char DecalBuilder::ApplyDecals(int light_level, char a3, stru154 *a4, int a5, RenderVertexSoft *a6, IndoorCameraD3D_Vec4 *a7, char a8, unsigned int uSectorID)
{
  char *v9; // eax@3
  signed int v10; // ecx@3
  RenderVertexSoft *v11; // eax@10
  unsigned int v12; // edx@10
  RenderVertexSoft *v13; // esi@11
  RenderVertexSoft *v14; // edi@11
  char v15; // zf@11
  stru154 *v16; // esi@12
  double v18; // st7@17
  double v19; // st6@17
  float v20; // eax@17
  Bloodsplat *v21; // esi@21
  int v22; // eax@21
  int v23; // ecx@21
  double v24; // st7@21
  int v25; // ebx@21
  double v26; // st7@21
  int v27; // edi@21
  double v28; // st7@21
  float v29; // ST10_4@21
  int v30; // ST08_4@21
  //DecalBuilder *v31; // esi@21
  int v32; // [sp+4h] [bp-44h]@18
  float v33; // [sp+8h] [bp-40h]@21
  stru314 *v34; // [sp+Ch] [bp-3Ch]@21
  float v35; // [sp+10h] [bp-38h]@21
  float v36; // [sp+14h] [bp-34h]@17
  int v37; // [sp+18h] [bp-30h]@17
  int a5a; // [sp+28h] [bp-20h]@21
  int v39; // [sp+2Ch] [bp-1Ch]@21
  int v40; // [sp+30h] [bp-18h]@21
  int v41; // [sp+34h] [bp-14h]@22
  int v42; // [sp+38h] [bp-10h]@21
  int v43; // [sp+3Ch] [bp-Ch]@21
  //DecalBuilder *thisa; // [sp+40h] [bp-8h]@1
  //RenderVertexSoft *a11; // [sp+44h] [bp-4h]@8
  int a6a;
  int *a6b;


//  __debugbreak();

  //auto a2 = light_level;
  //auto a9 = uSectorID;

  //thisa = this;
  if ( !a5 )
    return 0;

  static RenderVertexSoft static_AE4F90[64];
  static bool __init_flag1 = false;
  if (!__init_flag1)
  {
    __init_flag1 = true;

    for (uint i = 0; i < 64; ++i)
      static_AE4F90[i].flt_2C = 0.0f;
  }
  
  static stru314 static_AE4F60; // idb
  /*static bool __init_flag2 = false;
  if (!__init_flag2)
  {
    __init_flag2 = true;

    stru314::stru314(&static_AE4F60);
  }*/

  //a11 = a6;
  if ( a7 )
  {
	  for(int i=0; i < a5;i++)
      {
        memcpy(&static_AE4F90[i], a6, 0x30u);
		++a6;
      }
    v16 = a4;
    if ( pGame->pIndoorCameraD3D->_437376(a4, static_AE4F90, (unsigned int *)&a5) == 1 )
    {
      if ( !a5 )
        return 0;
      a6 = static_AE4F90;
    }
  }
  else
  {
    v16 = a4;
  }
  v18 = v16->face_plane.vNormal.z;
  v19 = v16->face_plane.vNormal.y;
  v20 = v16->face_plane.vNormal.x;
  v37 = (int)&static_AE4F60.field_1C;
  static_AE4F60.field_4.y = v19;
  static_AE4F60.field_4.x = v20;
  LODWORD(v36) = (DWORD)&static_AE4F60.field_10;
  static_AE4F60.field_4.z = v18;
  static_AE4F60.dist = v16->face_plane.dist;
  if ( !pGame->pIndoorCameraD3D->GetFacetOrientation(v16->polygonType, &static_AE4F60.field_4, &static_AE4F60.field_10, &static_AE4F60.field_1C) )
  {
      MessageBoxW(nullptr, L"Error: Failed to get the facet orientation", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\PolyProjector.cpp:101", 0);
  }
  int _a7 = 0;
  if ( this->uNumDecals > 0 )
  {
    a6b = this->std__vector_30B00C;
    do
    {
      v21 = &pBloodsplatContainer->std__vector_pBloodsplats[*a6b];
      v22 = _43F5C8_get_point_light_level_with_respect_to_lights(light_level, uSectorID, v21->x, v21->y, v21->z);
      v23 = v21->b;
      v24 = v21->x;
      v42 = v22;
      BYTE3(v22) = 0;
      *(short *)((char *)&v22 + 1) = v21->r;
      LOBYTE(v22) = v21->g;
      v43 = v23 | (v22 << 8);
      v25 = (signed __int64)v24;
      v26 = v21->z;
      v27 = (signed __int64)v21->y;
      v37 = a8;
      v40 = (signed __int64)v26;
      v28 = v21->dot_dist;
      //LODWORD(v36) = (uint32)a6;
      a5a = v25;
      v39 = v27;
      LODWORD(v35) = a5;
      v34 = &static_AE4F60;
      v33 = v28;
      v32 = v23 | (v22 << 8);
      v29 = v21->radius;
      //v30 = (int)v21;
      //v31 = thisa;
      if ( !this->_49B790_build_decal_geometry(v42, a3, (DecalBuilder_stru0 *)v21, (int)&a5a, SLODWORD(v29), v43, v33, &static_AE4F60, a5, a6, a8) )
      {
      MessageBoxW(nullptr, L"Error: Failed to build decal geometry", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\PolyProjector.cpp:114", 0);
      }
      ++_a7;
      ++a6b;
    }
    while ( _a7 < this->uNumDecals );
  }
  return 1;
}

//----- (0049B790) --------------------------------------------------------
char DecalBuilder::_49B790_build_decal_geometry(int a2, char a3, DecalBuilder_stru0 *a4, int a5, float a6, unsigned int uColorMultiplier, float a8, stru314 *a9, signed int a10, RenderVertexSoft *a11, char uClipFlags)
{
  DecalBuilder *v12; // esi@1
  Decal *v13; // edi@2
  int *v14; // eax@2
  double v15; // st7@4
  double v16; // st7@4
  int v17; // eax@4
  stru314 *v18; // ebx@4
  double v19; // st7@4
  double v20; // st7@4
  double v21; // st7@4
  double v22; // st6@4
  double v23; // st6@4
  double v24; // st5@4
  char *v25; // eax@4
  signed int v26; // ecx@4
  double v27; // st5@4
  double v28; // st7@5
  double v29; // st7@6
  char result; // al@6
  unsigned int *v31; // edi@7
  RenderVertexSoft *v32; // ebx@8
  std::string *v33; // ecx@15
  int v34; // eax@19
  const char *v35; // [sp-Ch] [bp-2Ch]@15
  int v36; // [sp-8h] [bp-28h]@15
  std::string v37; // [sp-4h] [bp-24h]@15
  float v38; // [sp+8h] [bp-18h]@6
  RenderVertexSoft *v39; // [sp+Ch] [bp-14h]@6
  unsigned int v40; // [sp+10h] [bp-10h]@6

  int a6a;
  RenderVertexSoft *a8a;
  unsigned int a8b = 0;

  v12 = this;
  if ( a6 == 0.0 )
    return 1;
  v13 = &this->std__vector_pDecals[this->field_308008];
  v14 = &this->std__vector_pDecals[this->field_308008].field_C1C;
  this->std__vector_pDecals[this->field_308008].field_C18 = a4;
  *v14 = 0;
  if ( a3 & 2 )
    *v14 = 1;
  v15 = a6 - a8;
  this->field_30C028 = v15;
  v16 = sqrt((a6 + a6 - v15) * v15);
  v12->field_30C02C = v16;
  v17 = a5;
  v18 = a9;
  v12->flt_30C030 = 1.0 - (a6 - v16) / a6;
  v13->field_C08 = (signed __int64)((double)*(signed int *)v17 - a8 * v18->field_4.x);
  v13->field_C0A = (signed __int64)((double)*(signed int *)(a5 + 4) - a8 * v18->field_4.y);
  v19 = a6;
  v13->field_C0C = (signed __int64)((double)*(signed int *)(a5 + 8) - a8 * v18->field_4.z);
  v20 = v19 * v12->flt_30C030;
  a8a = v13->pVertices;
  v12->field_30C034 = v20;
  v12->field_30C010 = v20 * v18->field_10.x;
  v12->field_30C014 = v20 * v18->field_10.y;
  v12->field_30C018 = v20 * v18->field_10.z;
  v12->field_30C01C = v20 * v18->field_1C.x;
  v12->field_30C020 = v20 * v18->field_1C.y;
  v12->field_30C024 = v20 * v18->field_1C.z;
  a6a = v13->field_C08;
  v21 = (double)a6a;
  v22 = v21 - v12->field_30C01C;
  a6a = v13->field_C0A;
  v13->pVertices[0].vWorldPosition.x = v22 + v12->field_30C010;
  v23 = (double)a6a;
  v24 = v23 - v12->field_30C020 + v12->field_30C014;
  v25 = (char *)&v13->pVertices[0].vWorldPosition.y;
  a6 = v13->field_C0C;
  v26 = 4;
  v13->pVertices[0].vWorldPosition.y = v24;
  v27 = (double)a6a;
  a6 = v27;
  v13->pVertices[0].vWorldPosition.z = v27 - v12->field_30C024 + v12->field_30C018;
  v13->pVertices[0].u = 0.0;
  v13->pVertices[0].v = 0.0;
  v13->pVertices[1].vWorldPosition.x = v21 - v12->field_30C01C - v12->field_30C010;
  v13->pVertices[1].vWorldPosition.y = v23 - v12->field_30C020 - v12->field_30C014;
  v13->pVertices[1].vWorldPosition.z = v27 - v12->field_30C024 - v12->field_30C018;
  v13->pVertices[1].u = 0.0;
  v13->pVertices[1].v = 1.0;
  v13->pVertices[2].vWorldPosition.x = v21 + v12->field_30C01C - v12->field_30C010;
  v13->pVertices[2].vWorldPosition.y = v23 + v12->field_30C020 - v12->field_30C014;
  v13->pVertices[2].vWorldPosition.z = v27 + v12->field_30C024 - v12->field_30C018;
  v13->pVertices[2].u = 1.0;
  v13->pVertices[2].v = 1.0;
  v13->pVertices[3].vWorldPosition.x = v21 + v12->field_30C01C + v12->field_30C010;
  v13->pVertices[3].vWorldPosition.y = v23 + v12->field_30C020 + v12->field_30C014;
  v13->pVertices[3].vWorldPosition.z = a6 + v12->field_30C024 + v12->field_30C018;
  v13->pVertices[3].u = 1.0;
  v13->pVertices[3].v = 0.0;
  do
  {
    v28 = v18->field_4.y * *(float *)v25
        + *((float *)v25 - 1) * v18->field_4.x
        + *((float *)v25 + 1) * v18->field_4.z
        + v18->dist;
    *((float *)v25 - 1) = *((float *)v25 - 1) - v28 * v18->field_4.x;
    *(float *)v25 = *(float *)v25 - v28 * v18->field_4.y;
    v25 += 48;
    --v26;
    *((float *)v25 - 11) = *((float *)v25 - 11) - v28 * v18->field_4.z;
  }
  while ( v26 );
  v13->uColorMultiplier = uColorMultiplier;
  //v40 = (unsigned int *)&v13->uNumVertices;
  v39 = v13->pVertices;
  v13->uNumVertices = 4;
  v13->field_C14 = a2;
  v29 = v18->field_4.z;
  //a6a = (unsigned int *)&v13->uNumVertices;
  v38 = v29;
  result = pGame->pStru9Instance->_4980B9(a11, a10, v18->field_4.x, v18->field_4.y, v38, v39, (unsigned int*)&v13->uNumVertices);
  if ( result )
  {
    //v31 = a6a;
    if ( !v13->uNumVertices )
      return 1;
    v32 = a8a;
    //v40 = *a6a;
    v39 = a8a;
    pGame->pIndoorCameraD3D->ViewTransform(a8a, (unsigned int)v13->uNumVertices);
    v40 = 0;
    pGame->pIndoorCameraD3D->Project(v32, v13->uNumVertices, 0);
    if ( !(uClipFlags & 1) )
    {
      ++v12->field_308008;
      v34 = 1024;
      if ( v12->field_308008 == 1024 )
        v12->field_308008 = 0;
      if ( (signed int)(v12->std__vector_pDecals_size + 1) <= 1024 )
        v34 = v12->std__vector_pDecals_size + 1;
      v12->std__vector_pDecals_size = v34;
      return 1;
    }
    if ( uCurrentlyLoadedLevelType == LEVEL_Outdoor )
    {
      if ( uClipFlags & 2 )
      {
        v40 = (int)&a8;
        v39 = v12->pVertices;
      __debugbreak(); // warning C4700: uninitialized local variable 'v31' used
        pGame->pIndoorCameraD3D->_436CDC_mess_with_lightmap__clipflag_2(v32, *v31, v12->pVertices, &a8b);
        v40 = (int)v31;
        v39 = v12->pVertices;
        pGame->pIndoorCameraD3D->_437143(a8b, v32, v12->pVertices, v31);
      }
      else if ( uClipFlags & 4 )
      {
        v40 = (int)&a8;
        v39 = v12->pVertices;
        pGame->pIndoorCameraD3D->_436F09_mess_with_lightmap__clipflag_4(v32, *v31, v12->pVertices, &a8b);
        v40 = (int)v31;
        v39 = v12->pVertices;
        pGame->pIndoorCameraD3D->_437143(a8b, v32, v12->pVertices, v31);
      }
	  else
	  {
        v39 = (RenderVertexSoft *)"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\PolyProjector.cpp:258";
        v35 = "Undefined clip flag specified";
	    MessageBoxA(nullptr, v35, (const char *)v39, 0);
	  }
    }
    else
    {
      v39 = (RenderVertexSoft *)"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\PolyProjector.cpp:263";
      v35 = "Lightpoly builder native indoor clipping not implemented";
	  MessageBoxA(nullptr, v35, (const char *)v39, 0);
    }
    if ( a8b != 0 )
	{
      ++v12->field_308008;
      v34 = 1024;
      if ( v12->field_308008 == 1024 )
        v12->field_308008 = 0;
      if ( (signed int)(v12->std__vector_pDecals_size + 1) <= 1024 )
        v34 = v12->std__vector_pDecals_size + 1;
      v12->std__vector_pDecals_size = v34;
      return 1;
	}
    result = 1;
  }
  return result;
}

//----- (0049BBBD) --------------------------------------------------------
bool DecalBuilder::ApplyBloodsplatDecals_IndoorFace(unsigned int uFaceID)
{
  double v7; // st7@12

  uNumDecals = 0;  
  if (!pBloodsplatContainer->std__vector_pBloodsplats_size)
    return true;
  
  BLVFace* pFace = &pIndoor->pFaces[uFaceID];

  if (pFace->uAttributes & 0x400000)
    return true;
  if (pFace->Fluid())
    return true;
  for (uint i = 0; i < pBloodsplatContainer->std__vector_pBloodsplats_size; ++i)
  {
    Bloodsplat* pBloodsplat = &pBloodsplatContainer->std__vector_pBloodsplats[i];
    if (pFace->pBounding.x1 - pBloodsplat->radius < pBloodsplat->x &&
        pFace->pBounding.x2 + pBloodsplat->radius > pBloodsplat->x &&
        pFace->pBounding.y1 - pBloodsplat->radius < pBloodsplat->y &&
        pFace->pBounding.y2 + pBloodsplat->radius > pBloodsplat->y &&
        pFace->pBounding.z1 - pBloodsplat->radius < pBloodsplat->z &&
        pFace->pBounding.z2 + pBloodsplat->radius > pBloodsplat->z)
    {
      v7 = pFace->pFacePlane.vNormal.z * pBloodsplat->z +
           pFace->pFacePlane.vNormal.y * pBloodsplat->y +
           pFace->pFacePlane.vNormal.x * pBloodsplat->x +
           pFace->pFacePlane.dist;
      if (v7 <= pBloodsplat->radius)
      {
        pBloodsplat->dot_dist = v7;
        std__vector_30B00C[uNumDecals++] = i;
      }
    }
  }

  return true;
}

//----- (0049BCEB) --------------------------------------------------------
char DecalBuilder::ApplyDecals_OutdoorFace(ODMFace *pFace)
{
	double v8; // st7@12
	unsigned int v10; // [sp+20h] [bp-1Ch]@1

	Bloodsplat *pBloodsplat;

	this->uNumDecals = 0;
	v10 = pBloodsplatContainer->std__vector_pBloodsplats_size;
	if ( !(pFace->uAttributes & 0x400000) && !(pFace->uAttributes & 0x10) )
	{
		for(int i = 0; i < pBloodsplatContainer->std__vector_pBloodsplats_size; i++ )
		{
			pBloodsplat = &pBloodsplatContainer->std__vector_pBloodsplats[i];
			if ( (double)pFace->pBoundingBox.x1 - pBloodsplat->radius < pBloodsplat->x &&
				(double)pFace->pBoundingBox.x2 + pBloodsplat->radius > pBloodsplat->x &&
				(double)pFace->pBoundingBox.y1 - pBloodsplat->radius < pBloodsplat->y &&
				(double)pFace->pBoundingBox.y2 + pBloodsplat->radius > pBloodsplat->y &&
				(double)pFace->pBoundingBox.z1 - pBloodsplat->radius < pBloodsplat->z &&
				(double)pFace->pBoundingBox.z2 + pBloodsplat->radius > pBloodsplat->z )
			{
				v8 = (double)((pFace->pFacePlane.dist
							+ round(pBloodsplat->x) * pFace->pFacePlane.vNormal.x
							+ round(pBloodsplat->y) * pFace->pFacePlane.vNormal.y
							+ round(pBloodsplat->z) * pFace->pFacePlane.vNormal.z) >> 16);
				if ( v8 <= pBloodsplat->radius )
				{
					pBloodsplat->dot_dist = v8;
					this->std__vector_30B00C[this->uNumDecals++] = i;
				}
			}
		}
	}
	return 1;
}

//----- (0049BE8A) --------------------------------------------------------
bool DecalBuilder::_49BE8A(struct Polygon *a2, Vec3_float_ *_a3, float *a4, RenderVertexSoft *a5, unsigned int uStripType, char a7)
{
  bool result; // eax@1
  RenderVertexSoft *v8; // edi@3
  Vec3_float_ *v9; // ebx@3
  Bloodsplat *v10; // esi@3
  float v11; // eax@5
  float v12; // eax@6
  double v13; // st7@13
  double v14; // st7@19
  short v15; // eax@20
  int v16; // eax@22
  int v17; // edx@24
  DecalBuilder *v18; // eax@24
  std::string v19; // [sp-18h] [bp-54h]@12
  const char *v20; // [sp-8h] [bp-44h]@12
  int v21; // [sp-4h] [bp-40h]@12
  double v22; // [sp+Ch] [bp-30h]@19
  unsigned int v23; // [sp+14h] [bp-28h]@1
  DecalBuilder *v24; // [sp+18h] [bp-24h]@1
  int v25; // [sp+1Ch] [bp-20h]@19
  float v26; // [sp+20h] [bp-1Ch]@12
  int v27; // [sp+24h] [bp-18h]@12
  float v28; // [sp+28h] [bp-14h]@13
  float v29; // [sp+2Ch] [bp-10h]@7
  float v30; // [sp+30h] [bp-Ch]@6
  float v31; // [sp+34h] [bp-8h]@6
  bool v32; // [sp+38h] [bp-4h]@2
  float a3;

  this->uNumDecals = 0;
  result = pBloodsplatContainer->std__vector_pBloodsplats_size != 0;
  v24 = this;
  v23 = pBloodsplatContainer->std__vector_pBloodsplats_size;
  if ( pBloodsplatContainer->std__vector_pBloodsplats_size )
  {
    v32 = 0;
    if ( (signed int)pBloodsplatContainer->std__vector_pBloodsplats_size > 0 )
    {
      v8 = a5;
      v9 = _a3;
	  v10 = pBloodsplatContainer->std__vector_pBloodsplats;
      while ( v32 < (signed int)v23)
      {
		if ( uStripType == 4 )
		{
		  a3 = v8->vWorldPosition.x;
		  v11 = v8[3].vWorldPosition.x;
		  v31 = v11;
		  v30 = v8[1].vWorldPosition.y;
		  v12 = v8->vWorldPosition.y;
		  v29 = v12;
		}
		else if ( uStripType == 3 )
        {
          if ( a7 )
          {
            a3 = v8->vWorldPosition.x;
            v31 = v8[2].vWorldPosition.x;
            v30 = v8[1].vWorldPosition.y;
            v12 = v8[2].vWorldPosition.y;
            v29 = v12;
          }
		  else
		  {
			  a3 = v8[1].vWorldPosition.x;
			  v11 = v8[2].vWorldPosition.x;
			  v31 = v11;
			  v30 = v8[1].vWorldPosition.y;
			  v12 = v8->vWorldPosition.y;
			  v29 = v12;
		  }
        }
		else
			MessageBoxW(nullptr, L"Uknown strip type detected!", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\PolyProjector.cpp:434", 0);
        v21 = uStripType;
        v13 = pGame->pIndoorCameraD3D->GetPolygonMinZ(v8, uStripType);
        v21 = uStripType;
        v28 = v13;
        v26 = pGame->pIndoorCameraD3D->GetPolygonMaxZ(v8, uStripType);
		if ( a3 - v10->radius < v10->x &&
			v31 + v10->radius > v10->x &&
			v30 - v10->radius < v10->y &&
            v29 + v10->radius > v10->y &&
            v28 - v10->radius < v10->z &&
            v26 + v10->radius > v10->z )
        {
			Vec3_float_::NegDot(&v8->vWorldPosition, v9, a4);
			v26 = v9->y * v10->y
				+ v9->z * v10->z
				+ v9->x * v10->x
				+ *a4;
			v22 = v26 + 6.7553994e15;
			v25 = LODWORD(v22);
			v14 = (double)SLODWORD(v22);
			v28 = v14;
			if ( v14 <= v10->radius )
			{
				v15 = a2->flags;
				if ( v15 & 2 || BYTE1(v15) & 1 )
				{
					v16 = v10->field_1C;
					if ( !(v16 & 1) )
					{
						LOBYTE(v16) = v16 | 1;
						v10->field_1C = v16;
						v10->field_20 = pEventTimer->Time();
					}
				}
				v17 = v32;
				v10->dot_dist = LODWORD(v28);
				v18 = v24;
				v24->std__vector_30B00C[v24->uNumDecals] = v17;
				++v18->uNumDecals;
			}
        }
        ++v32;
        ++v10;
        result = v32;
      }
    }
  }
  LOBYTE(result) = 1;
  return result;
}

//----- (0049C095) --------------------------------------------------------
void DecalBuilder::DrawDecal(Decal *pDecal, float z_bias)
{
  signed int v21; // [sp+Ch] [bp-864h]@15
  RenderVertexD3D3 pVerticesD3D[64]; // [sp+20h] [bp-850h]@6

  if (pDecal->uNumVertices < 3)
  {
    Log::Warning(L"Decal has < 3 vertices");
    return;
  }

  float color_mult;
  if ( pDecal->field_C1C & 1 )
    color_mult = 1.0;
  else
    color_mult = pDecal->field_C18->_43B570_get_color_mult_by_time();

  for (uint i = 0; i < pDecal->uNumVertices; ++i)
  {
    uint uTint = pRenderer->GetActorTintColor(pDecal->pVertices[i].vWorldViewPosition.x, pDecal->field_C14, 0, 0, nullptr);

    uint uTintR = (uTint >> 16) & 0xFF,
         uTintG = (uTint >> 8) & 0xFF,
         uTintB = uTint & 0xFF;

    uint uDecalColorMultR = (pDecal->uColorMultiplier >> 16) & 0xFF,
         uDecalColorMultG = (pDecal->uColorMultiplier >> 8) & 0xFF,
         uDecalColorMultB = pDecal->uColorMultiplier & 0xFF;

    uint uFinalR = floorf(uTintR / 255.0 * color_mult * uDecalColorMultR + 0.0f),
         uFinalG = floorf(uTintG / 255.0 * color_mult * uDecalColorMultG + 0.0f),
         uFinalB = floorf(uTintB / 255.0 * color_mult * uDecalColorMultB + 0.0f);


    float v15;
    if (fabs(z_bias) < 1e-5)
      v15 = 1.0 - 1.0 / ((1.0f / pGame->pIndoorCameraD3D->GetShadingDistMist()) * pDecal->pVertices[i].vWorldViewPosition.x * 1000.0);
     else
     {
      v15 = 1.0 - 1.0 / ((1.0f / pGame->pIndoorCameraD3D->GetShadingDistMist()) * pDecal->pVertices[i].vWorldViewPosition.x * 1000.0) - z_bias;
      if (v15 < 0.000099999997)
        v15 = 0.000099999997;
     }

    pVerticesD3D[i].pos.z = v15;

    pVerticesD3D[i].pos.x = pDecal->pVertices[i].vWorldViewProjX;
    pVerticesD3D[i].pos.y = pDecal->pVertices[i].vWorldViewProjY;
    pVerticesD3D[i].texcoord.x = pDecal->pVertices[i].u;
    pVerticesD3D[i].texcoord.y = pDecal->pVertices[i].v;
    pVerticesD3D[i].diffuse = (uFinalR << 16) | (uFinalG << 8) | uFinalB;
    pVerticesD3D[i].specular = 0;
    pVerticesD3D[i].rhw = 1.0 / pDecal->pVertices[i].vWorldViewPosition.x;
  }

  if (uCurrentlyLoadedLevelType == LEVEL_Indoor)
    v21 = D3DDP_DONOTLIGHT | D3DDP_DONOTCLIP | D3DDP_DONOTUPDATEEXTENTS;
  else
    v21 = D3DDP_DONOTLIGHT;

  ErrD3D(pRenderer->pRenderD3D->pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,
            D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1,
            pVerticesD3D,
            pDecal->uNumVertices,
            v21));
}

//----- (0049C2CD) --------------------------------------------------------
void DecalBuilder::DrawDecals(float z_bias)
{
  for (uint i = 0; i < std__vector_pDecals_size; ++i)
    DrawDecal(std__vector_pDecals + i, z_bias);
}

//----- (0049C304) --------------------------------------------------------
void DecalBuilder::DrawBloodsplats()
{
  if (!std__vector_pDecals_size)
    return;

  if (pRenderer->bUsingSpecular)
    ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE));
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_CLAMP));

  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE));
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE));
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE));
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE));
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE));
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DITHERENABLE, FALSE));

  IDirect3DTexture2* pTex = pGame->pIndoorCameraD3D->LoadTextureAndGetHardwarePtr("hwsplat04");
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetTexture(0, pTex));
 
  DrawDecals(0.00039999999);
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CW));
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE));
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE));
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE));
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ZERO));

  if (pRenderer->bUsingSpecular)
    ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, TRUE));
}

//----- (0049C550) --------------------------------------------------------
void DecalBuilder::DrawDecalDebugOutlines()
{
  for(int i = 0; i < std__vector_pDecals_size; i++)
	pGame->pIndoorCameraD3D->debug_outline_sw(std__vector_pDecals[i].pVertices, std__vector_pDecals[i].uNumVertices, 0xC86400u, 0.0);
}

//----- (0040E4C2) --------------------------------------------------------
void Decal::Decal_base_ctor()
{
  uNumVertices = -1;
  for (uint i = 0; i < 64; ++i)
    pVertices[i].flt_2C = 0.0f;
}
