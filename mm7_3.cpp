#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "Weather.h"
#include "Texture.h"
#include "mm7_data.h"
#include "Sprites.h"
#include "BSPModel.h"
#include "LightmapBuilder.h"
#include "DecalBuilder.h"
#include "ParticleEngine.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "stru6.h"
#include "FactionTable.h"
#include "MapInfo.h"
#include "Game.h"
#include "GUIWindow.h"
#include "GUIFont.h"
#include "GUIProgressBar.h"
#include "Party.h"
#include "AudioPlayer.h"
#include "Outdoor.h"
#include "Outdoor_stuff.h"
#include "Overlays.h"
#include "LOD.h"
#include "Actor.h"
#include "Events.h"
#include "Viewport.h"
#include "Math.h"
#include "SpriteObject.h"
#include "ObjectList.h"
#include "PaletteManager.h"
#include "DecorationList.h"
#include "stru123.h"
#include "Time.h"
#include "IconFrameTable.h"
#include "TurnEngine.h"
#include "Events2D.h"
#include "stru176.h"
#include "stru298.h"
#include "texts.h"
#include "Log.h"
#include "Lights.h"
#include "Level/Decoration.h"

#include "MM7.h"

//----- (0046E44E) --------------------------------------------------------
int  _46E44E_collide_against_faces_and_portals(unsigned int b1)
{
  BLVSector *pSector; // edi@1
  signed int v2; // ebx@1
  BLVFace *pFace; // esi@2
  __int16 pNextSector; // si@10
  int pArrayNum; // ecx@12
  unsigned __int8 v6; // sf@12
  unsigned __int8 v7; // of@12
  int result; // eax@14
  //BLVSector *v9; // ebx@15
  int v10; // ecx@15
  int pFloor; // eax@16
  //BLVFace *v12; // ebp@16
  int v13; // eax@24
  int v14; // esi@24
  int v15; // eax@24
  int v16; // edx@25
  int v17; // eax@29
  unsigned int v18; // eax@33
  int v19; // eax@35
  int v20; // edx@35
  int v21; // eax@35
  int v22; // ecx@36
  int v23; // eax@40
  unsigned int v24; // eax@44
  int a3; // [sp+10h] [bp-48h]@28
  int v26; // [sp+14h] [bp-44h]@15
  int i; // [sp+18h] [bp-40h]@1
  int a10; // [sp+1Ch] [bp-3Ch]@1
  int v29; // [sp+20h] [bp-38h]@14
  int v30; // [sp+24h] [bp-34h]@35
  int v31; // [sp+28h] [bp-30h]@24
  int v32; // [sp+2Ch] [bp-2Ch]@15
  int pSectorsArray[10]; // [sp+30h] [bp-28h]@1

  pSector = &pIndoor->pSectors[stru_721530.uSectorID];
  v2 = 0;
  a10 = b1;
  pSectorsArray[0] = stru_721530.uSectorID;
  for ( i = 1; v2 < pSector->uNumPortals; ++v2 )
  {
    pFace = &pIndoor->pFaces[pSector->pPortals[v2]];
    if ( stru_721530.sMaxX <= pFace->pBounding.x2
      && stru_721530.sMinX >= pFace->pBounding.x1
      && stru_721530.sMaxY <= pFace->pBounding.y2
      && stru_721530.sMinY >= pFace->pBounding.y1
      && stru_721530.sMaxZ <= pFace->pBounding.z2
      && stru_721530.sMinZ >= pFace->pBounding.z1
      && abs((pFace->pFacePlane_old.dist
            + stru_721530.normal.x * pFace->pFacePlane_old.vNormal.x
            + stru_721530.normal.y * pFace->pFacePlane_old.vNormal.y
            + stru_721530.normal.z * pFace->pFacePlane_old.vNormal.z) >> 16) <= stru_721530.field_6C + 16 )
    {
      pNextSector = pFace->uSectorID == stru_721530.uSectorID ? pFace->uBackSectorID : pFace->uSectorID;//FrontSectorID
      pArrayNum = i++;
      v7 = i < 10;
      v6 = i - 10 < 0;
      pSectorsArray[pArrayNum] = pNextSector;
      if ( !(v6 ^ v7) )
        break;
    }
  }
  result = 0;
  v29 = 0;
  if ( i > 0 )
  {
    while ( 1 )
    {
      pSector = &pIndoor->pSectors[pSectorsArray[result]];
      v10 = 0;
      v32 = pSector->uNumFloors + pSector->uNumWalls + pSector->uNumCeilings;
      v26 = 0;
      if ( v32 > 0 )
        break;
LABEL_46:
      result = v29++ + 1;
      if ( v29 >= i )
        return result;
    }
    while ( 1 )
    {
      pFloor = pSector->pFloors[v10];
      pFace = &pIndoor->pFaces[pFloor];
      if ( pFace->Portal()
        || stru_721530.sMaxX > pFace->pBounding.x2
        || stru_721530.sMinX < pFace->pBounding.x1
        || stru_721530.sMaxY > pFace->pBounding.y2
        || stru_721530.sMinY < pFace->pBounding.y1
        || stru_721530.sMaxZ > pFace->pBounding.z2
        || stru_721530.sMinZ < pFace->pBounding.z1
        || pFloor == stru_721530.field_84 )
        goto LABEL_45;
      v13 = pFace->pFacePlane_old.vNormal.y;
      v14 = pFace->pFacePlane_old.dist;
      v31 = v13;
      v15 = (stru_721530.normal.x * pFace->pFacePlane_old.vNormal.x + v14 + stru_721530.normal.y * v13
           + stru_721530.normal.z * pFace->pFacePlane_old.vNormal.z) >> 16;
      if ( v15 > 0 )
      {
        v16 = (stru_721530.normal2.y * v31 + pFace->pFacePlane_old.dist + stru_721530.normal2.z * pFace->pFacePlane_old.vNormal.z
             + stru_721530.normal2.x * pFace->pFacePlane_old.vNormal.x) >> 16;
        if ( v15 <= stru_721530.prolly_normal_d || v16 <= stru_721530.prolly_normal_d )
        {
          if ( v16 <= v15 )
          {
            a3 = stru_721530.field_6C;
            if ( sub_47531C(stru_721530.prolly_normal_d, &a3, stru_721530.normal.x, stru_721530.normal.y, stru_721530.normal.z,
                   stru_721530.direction.x, stru_721530.direction.y, stru_721530.direction.z, pFace, a10) )
            {
              v17 = a3;
            }
            else
            {
              a3 = stru_721530.field_6C + stru_721530.prolly_normal_d;
              if ( !sub_475D85(&stru_721530.normal, &stru_721530.direction, &a3, pFace) )
                goto LABEL_34;
              v17 = a3 - stru_721530.prolly_normal_d;
              a3 -= stru_721530.prolly_normal_d;
            }
            if ( v17 < stru_721530.field_7C )
            {
              stru_721530.field_7C = v17;
              v18 = 8 * pSector->pFloors[v26];
              LOBYTE(v18) = v18 | 6;
              stru_721530.uFaceID = v18;
            }
          }
        }
      }
LABEL_34:
      if ( !(stru_721530.field_0 & 1)
        || (v19 = pFace->pFacePlane_old.vNormal.x,
            v20 = pFace->pFacePlane_old.vNormal.y,
            v30 = v19,
            v21 = (stru_721530.position.x * v19 + pFace->pFacePlane_old.dist + stru_721530.position.y * v20
                 + stru_721530.position.z * pFace->pFacePlane_old.vNormal.z) >> 16,
            v21 <= 0)
        || (v22 = (stru_721530.field_4C * v30 + pFace->pFacePlane_old.dist + stru_721530.field_50 * v20
                 + stru_721530.field_54 * pFace->pFacePlane_old.vNormal.z) >> 16,
            v21 > stru_721530.prolly_normal_d)
        && v22 > stru_721530.prolly_normal_d
        || v22 > v21 )
        goto LABEL_45;
      a3 = stru_721530.field_6C;
      if ( sub_47531C(stru_721530.field_8_radius, &a3, stru_721530.position.x, stru_721530.position.y, stru_721530.position.z,
             stru_721530.direction.x, stru_721530.direction.y, stru_721530.direction.z, pFace, a10) )
      {
        v23 = a3;
        goto LABEL_43;
      }
      a3 = stru_721530.field_6C + stru_721530.field_8_radius;
      if ( sub_475D85(&stru_721530.position, &stru_721530.direction, &a3, pFace) )
      {
        v23 = a3 - stru_721530.prolly_normal_d;
        a3 -= stru_721530.prolly_normal_d;
LABEL_43:
        if ( v23 < stru_721530.field_7C )
        {
          stru_721530.field_7C = v23;
          v24 = 8 * pSector->pFloors[v26];
          LOBYTE(v24) = v24 | 6;
          stru_721530.uFaceID = v24;
        }
      }
LABEL_45:
      v10 = v26++ + 1;
      if ( v26 >= v32 )
        goto LABEL_46;
    }
  }
  return result;
}
// 46E44E: using guessed type int var_28[10];

//----- (0046E889) --------------------------------------------------------
int __fastcall _46E889_collide_against_bmodels(unsigned int ecx0)
{
  int result; // eax@1
  BSPModel *v2; // ecx@3
  int v3; // ebx@9
  ODMFace *v4; // eax@11
  int v5; // edi@17
  int v6; // esi@17
  unsigned int v7; // ecx@17
  int v8; // eax@19
  int v9; // ecx@20
  int v10; // eax@24
  int v11; // ST18_4@25
  int v12; // ST1C_4@25
  int v13; // ST0C_4@25
  unsigned int v14; // eax@28
  int v15; // eax@30
  int v16; // ecx@31
  unsigned int v17; // eax@36
  int v18; // ST18_4@41
  int v19; // ST1C_4@41
  int v20; // ST0C_4@41
  int v21; // eax@42
  unsigned int v22; // eax@43
  //BLVFace f; // [sp+Ch] [bp-7Ch]@1
  int v24; // [sp+6Ch] [bp-1Ch]@9
  int a11; // [sp+70h] [bp-18h]@1
  unsigned int v26; // [sp+74h] [bp-14h]@2
  unsigned int v27; // [sp+78h] [bp-10h]@10
  BSPModel *v28; // [sp+7Ch] [bp-Ch]@3
  int a10; // [sp+80h] [bp-8h]@1
  int a2; // [sp+84h] [bp-4h]@23

  a11 = ecx0;

  BLVFace f; // [sp+Ch] [bp-7Ch]@1
  //BLVFace::BLVFace(&f);

  result = 0;
  a10 = 0;
  if ( (signed int)pOutdoor->uNumBModels > 0 )
  {
    v26 = 0;
    while ( 1 )
    {
      v2 = &pOutdoor->pBModels[v26 / 0xBC];
      v28 = &pOutdoor->pBModels[v26 / 0xBC];
      if ( stru_721530.sMaxX <= pOutdoor->pBModels[v26 / 0xBC].sMaxX )
      {
        if ( stru_721530.sMinX >= v2->sMinX )
        {
          if ( stru_721530.sMaxY <= v2->sMaxY )
          {
            if ( stru_721530.sMinY >= v2->sMinY )
            {
              if ( stru_721530.sMaxZ <= v2->sMaxZ )
              {
                if ( stru_721530.sMinZ >= v2->sMinZ )
                {
                  v3 = 0;
                  v24 = v2->uNumFaces;
                  if ( v24 > 0 )
                    break;
                }
              }
            }
          }
        }
      }
LABEL_39:
      ++a10;
      v26 += 188;
      result = a10;
      if ( a10 >= (signed int)pOutdoor->uNumBModels )
        return result;
    }
    v27 = 0;
    while ( 1 )
    {
      v4 = &v2->pFaces[v27 / 0x134];
      if ( stru_721530.sMaxX > v4->pBoundingBox.x2
        || stru_721530.sMinX < v4->pBoundingBox.x1
        || stru_721530.sMaxY > v4->pBoundingBox.y2
        || stru_721530.sMinY < v4->pBoundingBox.y1
        || stru_721530.sMaxZ > v4->pBoundingBox.z2
        || stru_721530.sMinZ < v4->pBoundingBox.z1 )
        goto LABEL_38;
      f.pFacePlane_old.vNormal.x = v4->pFacePlane.vNormal.x;
      f.pFacePlane_old.vNormal.y = v4->pFacePlane.vNormal.y;
      v5 = v4->pFacePlane.vNormal.z;
      f.pFacePlane_old.vNormal.z = v4->pFacePlane.vNormal.z;
      v6 = v4->pFacePlane.dist;
      f.pFacePlane_old.dist = v4->pFacePlane.dist;
      v7 = v4->uAttributes;
      f.uAttributes = v7;
      f.pBounding.x1 = v4->pBoundingBox.x1;
      f.pBounding.y1 = v4->pBoundingBox.y1;
      f.pBounding.z1 = v4->pBoundingBox.z1;
      f.pBounding.x2 = v4->pBoundingBox.x2;
      f.pBounding.y2 = v4->pBoundingBox.y2;
      f.pBounding.z2 = v4->pBoundingBox.z2;
      f.zCalc1 = v4->zCalc1;
      f.zCalc2 = v4->zCalc2;
      f.zCalc3 = v4->zCalc3;
      f.pXInterceptDisplacements = v4->pXInterceptDisplacements;
      f.pYInterceptDisplacements = v4->pYInterceptDisplacements;
      f.pZInterceptDisplacements = v4->pZInterceptDisplacements;
      f.uPolygonType = (PolygonType)v4->uPolygonType;
      f.uNumVertices = v4->uNumVertices;
      f.uBitmapID = v4->uTextureID;
      f.pVertexIDs = v4->pVertexIDs;
      if ( !(v7 & 0x20000000) )
      {
        if ( !(v7 & 1) )
          break;
      }
LABEL_37:
      v2 = v28;
LABEL_38:
      v27 += 308;
      ++v3;
      if ( v3 >= v24 )
        goto LABEL_39;
    }
    v8 = (v5 * stru_721530.normal.z
        + v6
        + f.pFacePlane_old.vNormal.y * stru_721530.normal.y
        + f.pFacePlane_old.vNormal.x * stru_721530.normal.x) >> 16;
    if ( v8 > 0 )
    {
      v9 = (v5 * stru_721530.normal2.z
          + v6
          + f.pFacePlane_old.vNormal.y * stru_721530.normal2.y
          + f.pFacePlane_old.vNormal.x * stru_721530.normal2.x) >> 16;
      if ( v8 <= stru_721530.prolly_normal_d || v9 <= stru_721530.prolly_normal_d )
      {
        if ( v9 <= v8 )
        {
          a2 = stru_721530.field_6C;
          if ( sub_4754BF(
                 stru_721530.prolly_normal_d,
                 &a2,
                 stru_721530.normal.x,
                 stru_721530.normal.y,
                 stru_721530.normal.z,
                 stru_721530.direction.x,
                 stru_721530.direction.y,
                 stru_721530.direction.z,
                 &f,
                 a10,
                 a11) )
          {
            v10 = a2;
          }
          else
          {
            v11 = stru_721530.direction.y;
            v12 = stru_721530.direction.z;
            v13 = stru_721530.normal.y;
            a2 = stru_721530.prolly_normal_d + stru_721530.field_6C;
            if ( !sub_475F30(
                    &a2,
                    &f,
                    stru_721530.normal.x,
                    v13,
                    stru_721530.normal.z,
                    stru_721530.direction.x,
                    v11,
                    v12,
                    a10) )
              goto LABEL_29;
            v10 = a2 - stru_721530.prolly_normal_d;
            a2 -= stru_721530.prolly_normal_d;
          }
          if ( v10 < stru_721530.field_7C )
          {
            stru_721530.field_7C = v10;
            v14 = 8 * (v3 | (a10 << 6));
            LOBYTE(v14) = v14 | 6;
            stru_721530.uFaceID = v14;
          }
        }
      }
    }
LABEL_29:
    if ( stru_721530.field_0 & 1 )
    {
      v15 = (f.pFacePlane_old.vNormal.z * stru_721530.position.z
           + f.pFacePlane_old.dist
           + f.pFacePlane_old.vNormal.y * stru_721530.position.y
           + f.pFacePlane_old.vNormal.x * stru_721530.position.x) >> 16;
      if ( v15 > 0 )
      {
        v16 = (f.pFacePlane_old.vNormal.z * stru_721530.field_54
             + f.pFacePlane_old.dist
             + f.pFacePlane_old.vNormal.y * stru_721530.field_50
             + f.pFacePlane_old.vNormal.x * stru_721530.field_4C) >> 16;
        if ( v15 <= stru_721530.prolly_normal_d || v16 <= stru_721530.prolly_normal_d )
        {
          if ( v16 <= v15 )
          {
            a2 = stru_721530.field_6C;
            if ( sub_4754BF(stru_721530.field_8_radius,
                   &a2,
                   stru_721530.position.x,
                   stru_721530.position.y,
                   stru_721530.position.z,
                   stru_721530.direction.x,
                   stru_721530.direction.y,
                   stru_721530.direction.z,
                   &f,
                   a10,
                   a11) )
            {
              if ( a2 < stru_721530.field_7C )
              {
                stru_721530.field_7C = a2;
                v17 = 8 * (v3 | (a10 << 6));
                LOBYTE(v17) = v17 | 6;
                stru_721530.uFaceID = v17;
              }
            }
            else
            {
              v18 = stru_721530.direction.y;
              v19 = stru_721530.direction.z;
              v20 = stru_721530.position.y;
              a2 = stru_721530.field_6C + stru_721530.field_8_radius;
              if ( sub_475F30(&a2,
                     &f,
                     stru_721530.position.x,
                     v20,
                     stru_721530.position.z,
                     stru_721530.direction.x,
                     v18,
                     v19,
                     a10) )
              {
                v2 = v28;
                v21 = a2 - stru_721530.prolly_normal_d;
                a2 -= stru_721530.prolly_normal_d;
                if ( a2 < stru_721530.field_7C )
                {
                  stru_721530.field_7C = v21;
                  v22 = 8 * (v3 | (a10 << 6));
                  LOBYTE(v22) = v22 | 6;
                  stru_721530.uFaceID = v22;
                }
                goto LABEL_38;
              }
            }
          }
        }
      }
    }
    goto LABEL_37;
  }
  return result;
}

//----- (0046ED1B) --------------------------------------------------------
int collide_against_floor(int x, int y, int z, unsigned int *pSectorID, unsigned int *pFaceID)
{
  uint uFaceID = -1;
  int floor_level = BLV_GetFloorLevel(x, y, z, *pSectorID, &uFaceID);

  if (floor_level != -30000 && floor_level <= z + 50)
  {
    *pFaceID = uFaceID;
    return floor_level;
  }

  uint uSectorID = pIndoor->GetSector(x, y, z);
  *pSectorID = uSectorID;

  floor_level = BLV_GetFloorLevel(x, y, z, uSectorID, &uFaceID);
  if (uSectorID && floor_level != -30000)
    *pFaceID = uFaceID;
  else return -30000;
  return floor_level;
}

//----- (0046ED8A) --------------------------------------------------------
void __fastcall _46ED8A_collide_against_sprite_objects(unsigned int _this)
{
  int v1; // ebx@2
  ObjectDesc *v2; // edx@4
  int v3; // esi@5
  int v4; // ecx@5
  int v5; // eax@7
  int v6; // edi@9
  int v7; // edx@9
  int v8; // edi@11
  int v9; // ebx@11
  int v10; // ecx@12
  int v11; // esi@13
  signed int v12; // [sp+0h] [bp-14h]@1
  int v13; // [sp+4h] [bp-10h]@9
  char *v14; // [sp+8h] [bp-Ch]@2
  unsigned int v15; // [sp+10h] [bp-4h]@1

  v15 = 0;
  v12 = _this;
  if ( (signed int)uNumSpriteObjects > 0 )
  {
    v1 = (int)&pSpriteObjects[0].uObjectDescID;
    v14 = (char *)&pSpriteObjects[0].uObjectDescID;
    do
    {
      if ( *(short *)v1 )
      {
        v2 = &pObjectList->pObjects[*(short *)v1];
        if ( !(v2->uFlags & 2) )
        {
          v3 = v2->uRadius;
          v4 = *(int *)(v1 + 2);
          if ( stru_721530.sMaxX <= v4 + v3 )
          {
            if ( stru_721530.sMinX >= v4 - v3 )
            {
              v5 = *(int *)(v1 + 6);
              if ( stru_721530.sMaxY <= v5 + v3 )
              {
                if ( stru_721530.sMinY >= v5 - v3 )
                {
                  v6 = v2->uHeight;
                  v7 = *(int *)(v1 + 10);
                  v13 = v6;
                  if ( stru_721530.sMaxZ <= v7 + v6 )
                  {
                    if ( stru_721530.sMinZ >= v7 )
                    {
                      v8 = v4 - stru_721530.normal.x;
                      v9 = v5 - stru_721530.normal.y;
                      if ( abs(((v4 - stru_721530.normal.x) * stru_721530.direction.y
                              - (v5 - stru_721530.normal.y) * stru_721530.direction.x) >> 16) <= v3
                                                                                              + stru_721530.prolly_normal_d )
                      {
                        v10 = (v8 * stru_721530.direction.x + v9 * stru_721530.direction.y) >> 16;
                        if ( v10 > 0 )
                        {
                          v11 = stru_721530.normal.z
                              + ((unsigned __int64)(stru_721530.direction.z * (signed __int64)v10) >> 16);
                          if ( v11 >= *(int *)(v14 + 10) - stru_721530.prolly_normal_d )
                          {
                            if ( v11 <= v13 + stru_721530.prolly_normal_d + *(int *)(v14 + 10) )
                            {
                              if ( v10 < stru_721530.field_7C )
                                sub_46DEF2(v12, v15);
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
      ++v15;
      v1 = (int)(v14 + 112);
      v14 += 112;
    }
    while ( (signed int)v15 < (signed int)uNumSpriteObjects );
  }
}

//----- (0046EF01) --------------------------------------------------------
int _46EF01_collision_chech_player(int a1)
{
  int v1; // edx@1
  int result; // eax@1
  int v3; // ebx@7
  int v4; // esi@7
  int v5; // edi@8
  int v6; // ecx@9
  int v7; // edi@12
  int v8; // [sp+Ch] [bp-10h]@1
  unsigned int v9; // [sp+10h] [bp-Ch]@1
  int v10; // [sp+14h] [bp-8h]@7
  int v11; // [sp+18h] [bp-4h]@7

  v8 = a1;
  v1 = 2 * pParty->field_14_radius;
  result = pParty->vPosition.x;
  v9 = pParty->uPartyHeight;
  if ( stru_721530.sMaxX <= pParty->vPosition.x + 2 * pParty->field_14_radius )
  {
    if ( stru_721530.sMinX >= pParty->vPosition.x - v1 )
    {
      if ( stru_721530.sMaxY <= pParty->vPosition.y + v1 )
      {
        if ( stru_721530.sMinY >= pParty->vPosition.y - v1 )
        {
          if ( stru_721530.sMaxZ <= (signed int)(pParty->vPosition.z + pParty->uPartyHeight) )
          {
            if ( stru_721530.sMinZ >= pParty->vPosition.z )
            {
              v3 = stru_721530.prolly_normal_d + v1;
              v11 = pParty->vPosition.x - stru_721530.normal.x;
              v4 = ((pParty->vPosition.x - stru_721530.normal.x) * stru_721530.direction.y
                  - (pParty->vPosition.y - stru_721530.normal.y) * stru_721530.direction.x) >> 16;
              v10 = pParty->vPosition.y - stru_721530.normal.y;
              result = abs(((pParty->vPosition.x - stru_721530.normal.x) * stru_721530.direction.y
                          - (pParty->vPosition.y - stru_721530.normal.y) * stru_721530.direction.x) >> 16);
              if ( result <= v3 )
              {
                result = v10 * stru_721530.direction.y;
                v5 = (v10 * stru_721530.direction.y + v11 * stru_721530.direction.x) >> 16;
                if ( v5 > 0 )
                {
                  v6 = ((unsigned __int64)(stru_721530.direction.z * (signed __int64)v5) >> 16) + stru_721530.normal.z;
                  result = pParty->vPosition.z;
                  if ( v6 >= pParty->vPosition.z )
                  {
                    result = v9 + pParty->vPosition.z;
                    if ( v6 <= (signed int)(v9 + pParty->vPosition.z) || v8 )
                    {
                      result = integer_sqrt(v3 * v3 - v4 * v4);
                      v7 = v5 - result;
                      if ( v7 < 0 )
                        v7 = 0;
                      if ( v7 < stru_721530.field_7C )
                      {
                        stru_721530.field_7C = v7;
                        stru_721530.uFaceID = 4;
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return result;
}

//----- (0046F04E) --------------------------------------------------------
int _46F04E_collide_against_portals()
{
  BLVSector *v0; // ecx@1
  unsigned int v1; // eax@1
  int v2; // eax@2
  BLVFace *v3; // eax@3
  int v4; // ecx@9
  int v5; // edx@9
  signed int result; // eax@21
  BLVFace *v7; // eax@22
  signed int v8; // [sp+0h] [bp-14h]@1
  BLVSector *v9; // [sp+4h] [bp-10h]@1
  unsigned int v10; // [sp+8h] [bp-Ch]@1
  int a3; // [sp+Ch] [bp-8h]@13
  int v12; // [sp+10h] [bp-4h]@15

  v8 = 0;
  v0 = &pIndoor->pSectors[stru_721530.uSectorID];
  v1 = 0xFFFFFFu;
  v10 = 0xFFFFFFu;
  v9 = v0;
  if ( v0->uNumPortals > 0 )
  {
    do
    {
      v2 = v0->pPortals[v8];
      if ( v2 != stru_721530.field_80 )
      {
        v3 = &pIndoor->pFaces[v2];
        if ( stru_721530.sMaxX <= v3->pBounding.x2 )
        {
          if ( stru_721530.sMinX >= v3->pBounding.x1
            && stru_721530.sMaxY <= v3->pBounding.y2
            && stru_721530.sMinY >= v3->pBounding.y1
            && stru_721530.sMaxZ <= v3->pBounding.z2
            && stru_721530.sMinZ >= v3->pBounding.z1 )
          {
            v4 = (stru_721530.normal.x * v3->pFacePlane_old.vNormal.x + v3->pFacePlane_old.dist
                + stru_721530.normal.y * v3->pFacePlane_old.vNormal.y
                + stru_721530.normal.z * v3->pFacePlane_old.vNormal.z) >> 16;
            v5 = (stru_721530.normal2.z * v3->pFacePlane_old.vNormal.z + v3->pFacePlane_old.dist
                + stru_721530.normal2.x * v3->pFacePlane_old.vNormal.x
                + stru_721530.normal2.y * v3->pFacePlane_old.vNormal.y) >> 16;
            if ( (v4 < stru_721530.prolly_normal_d || v5 < stru_721530.prolly_normal_d)
              && (v4 > -stru_721530.prolly_normal_d || v5 > -stru_721530.prolly_normal_d)
              && (a3 = stru_721530.field_6C, sub_475D85(&stru_721530.normal, &stru_721530.direction, &a3, v3))
              && a3 < (signed int)v10 )
            {
              v0 = v9;
              v10 = a3;
              v12 = v9->pPortals[v8];
            }
            else
            {
              v0 = v9;
            }
          }
        }
      }
      ++v8;
    }
    while ( v8 < v0->uNumPortals );
    v1 = v10;
  }
  if ( stru_721530.field_7C >= (signed int)v1 && (signed int)v1 <= stru_721530.field_6C )
  {
    stru_721530.field_80 = v12;
    v7 = &pIndoor->pFaces[v12];
    if ( v7->uSectorID == stru_721530.uSectorID )
      stru_721530.uSectorID = v7->uBackSectorID;
    else
      stru_721530.uSectorID = v7->uSectorID;
    stru_721530.field_7C = 268435455;
    result = 0;
  }
  else
  {
    result = 1;
  }
  return result;
}

//----- (0047050A) --------------------------------------------------------
int stru141_actor_collision_object::_47050A(int dt)
{
  //stru141_actor_collision_object *v2; // esi@1
  //signed int v3; // eax@1
  //int v4; // ecx@1
  //int v5; // edx@1
  //int v6; // edx@1
  int v7; // eax@1
  //int v8; // eax@3
  signed int result; // eax@4
  //int v10; // eax@5
  //int v11; // eax@5
  //int v12; // ebx@5
  //int v13; // edx@5
  //int v14; // edi@5
  //int v15; // ecx@5
  //int v16; // eax@5
  int v17; // eax@5
  int v18; // eax@7
  //int v19; // edx@9
  //int v20; // edi@9
  int v21; // eax@9
  int v22; // eax@11
  //int v23; // edx@13
  //int v24; // eax@13
  //int v25; // eax@14
  //int v26; // eax@16
  //int v27; // eax@17
  //int v28; // [sp+14h] [bp+8h]@5

  //v2 = this;
  int speed = 1 | integer_sqrt(this->velocity.z * this->velocity.z + this->velocity.y * this->velocity.y + this->velocity.x * this->velocity.x);

  this->direction.x = 65536 / speed * this->velocity.x;
  this->direction.y = 65536 / speed * this->velocity.y;
  this->direction.z = 65536 / speed * this->velocity.z;

  this->speed = speed;
  this->inv_speed = 65536 / speed;

  if (dt)
    v7 = dt;
  else
    v7 = pEventTimer->dt_in_some_format;

  //v8 = fixpoint_mul(v7, speed) - this->field_70; // speed * dt - something
  this->field_6C = fixpoint_mul(v7, speed) - this->field_70;
  if ( this->field_6C > 0 )
  {
    //v10 = fixpoint_mul(v8, this->direction.x) + this->normal.x;
    this->field_4C = fixpoint_mul(this->field_6C, this->direction.x) + this->normal.x;
    this->normal2.x = fixpoint_mul(this->field_6C, this->direction.x) + this->normal.x;
    //v11 = fixpoint_mul(this->field_6C, this->direction.y) + this->normal.y;
    this->field_50 = fixpoint_mul(this->field_6C, this->direction.y) + this->normal.y;
    this->normal2.y = fixpoint_mul(this->field_6C, this->direction.y) + this->normal.y;
    this->normal2.z = fixpoint_mul(this->field_6C, this->direction.z) + this->normal.z;
    //v12 = this->position.z;
    //v13 = this->normal.x;
    //v14 = this->normal2.x;
    //v15 = this->prolly_normal_d;
    //v16 = this->position.z + fixpoint_mul(this->field_6C, this->direction.z);
    //v28 = this->position.z + fixpoint_mul(this->field_6C, this->direction.z);
    this->field_54 = this->position.z + fixpoint_mul(this->field_6C, this->direction.z);
    v17 = this->normal.x;
    if ( v17 >= this->normal2.x )
      v17 = this->normal2.x;
    this->sMaxX = v17 - this->prolly_normal_d;
    v18 = this->prolly_normal_d + this->normal.x;
    if ( this->normal.x <= this->normal2.x )
      v18 = this->prolly_normal_d + this->normal2.x;
    //v19 = this->normal.y;
    //v20 = this->normal2.y;
    this->sMinX = v18;
    v21 = this->normal.y;
    if ( v21 >= this->normal2.y )
      v21 = this->normal2.y;
    this->sMaxY = v21 - this->prolly_normal_d;
    v22 = this->prolly_normal_d + this->normal.y;
    if ( this->normal.y <= this->normal2.y )
      v22 = this->normal2.y + this->prolly_normal_d;
    //v23 = this->normal2.z;
    this->sMinY = v22;
    //v24 = this->normal.z;
    if ( this->normal.z >= this->normal2.z )
      this->sMaxZ = this->normal2.z - this->prolly_normal_d;
    else
      this->sMaxZ = this->normal.z - this->prolly_normal_d;
    //this->sMaxZ = v25;
    //v26 = this->field_8_radius;
    if ( this->position.z <= this->position.z + fixpoint_mul(this->field_6C, this->direction.z) )
      this->sMinZ = (this->position.z + fixpoint_mul(this->field_6C, this->direction.z)) + this->field_8_radius;
    else
      this->sMinZ = this->position.z + this->field_8_radius;
    this->uFaceID = 0;
    this->field_80 = -1;
    this->field_88 = -1;
    //this->sMinZ = v27;
    this->field_7C = 0xFFFFFFu;
    result = 0;
  }
  else
  {
    result = 1;
  }
  return result;
}

//----- (004706C6) --------------------------------------------------------
void UpdateActors_ODM()
{
  Actor *v0; // esi@2
  AIState uAIState; // ax@2
  //unsigned int v2; // ecx@6
  int v3; // ebx@6
  //int v4; // eax@8
  int v5; // eax@10
  int v6; // ecx@10
  //signed int v7; // ebx@10
  signed int v8; // ebx@17
  //unsigned __int8 v9; // zf@17
  unsigned __int8 v10; // sf@17
  unsigned __int16 v11; // ax@21
  //int v12; // eax@29
  //unsigned __int64 v13; // qax@29
  //int v14; // eax@30
  //unsigned __int64 v15; // qax@30
  int v16; // eax@33
  //int v17; // edi@34
  //int v18; // edx@42
  //int v19; // ecx@42
  __int16 v20; // ax@42
  //int v21; // ebx@42
  //int v22; // edi@42
  //int v23; // ecx@42
  //__int16 v24; // ax@42
  int v25; // eax@45
  signed int v26; // ecx@50
  //int v27; // eax@52
  int v28; // eax@54
  signed int v29; // ebx@57
  signed int v30; // eax@57
  int v31; // edi@57
  signed int i; // ebx@57
  unsigned int v33; // ecx@58
  //int v34; // ebx@64
  int v35; // edi@64
  int v36; // eax@64
  //unsigned __int16 v37; // cx@66
  signed int v38; // edx@71
  unsigned int v39; // edi@71
  BSPModel *v40; // eax@75
  ODMFace *v41; // edi@75
  int v42; // ebx@76
  int v43; // ecx@77
  int v44; // edx@77
  __int16 v45; // dx@82
  int v46; // ecx@82
  signed int v47; // ebx@85
  int v48; // edi@85
  int v49; // edi@85
  //int v50; // eax@85
  //unsigned __int64 v51; // qax@85
  //unsigned __int8 v52; // zf@87
  //unsigned __int8 v53; // sf@87
 // unsigned __int8 v54; // of@104
  int v55; // eax@107
  unsigned int v56; // edi@107
  int v57; // ST10_4@107
  unsigned int v58; // edi@107
  unsigned int v59; // ebx@107
  signed int v60; // eax@107
  int v61; // eax@124
  Vec3_int_ v62; // [sp+Ch] [bp-44h]@42
  int v63; // [sp+18h] [bp-38h]@64
  int v64; // [sp+1Ch] [bp-34h]@64
  int v65; // [sp+20h] [bp-30h]@2
  int v66; // [sp+24h] [bp-2Ch]@2
  bool v67; // [sp+28h] [bp-28h]@10
  //unsigned int v68; // [sp+2Ch] [bp-24h]@10
  unsigned int v69; // [sp+30h] [bp-20h]@6
  unsigned int v70; // [sp+34h] [bp-1Ch]@10
  int v71; // [sp+38h] [bp-18h]@62
  int uIsAboveFloor; // [sp+3Ch] [bp-14h]@10
  int v72b;
  //int v73; // [sp+40h] [bp-10h]@17
  int uIsFlying; // [sp+44h] [bp-Ch]@8
  unsigned int v75; // [sp+48h] [bp-8h]@1
  int uIsOnWater; // [sp+4Ch] [bp-4h]@10

  for (v75 = 0; v75 < uNumActors; ++v75)
  {
    v0 = &pActors[v75];
    v66 = v0->vPosition.x;
    v65 = v0->vPosition.y;
    uAIState = v0->uAIState;
    if ( uAIState == Removed || uAIState == Disabled || uAIState == Summoned || !v0->uMovementSpeed )
	{
		continue;
	}
    v3 = 0;
    v69 = 0;
    if ( MonsterStats::BelongsToSupertype(v0->pMonsterInfo.uID, MONSTER_SUPERTYPE_WATER_ELEMENTAL) )
      v3 = 1;
    v0->uSectorID = 0;
    uIsFlying = v0->pMonsterInfo.uFlying;
    if ( !v0->CanAct() )
      uIsFlying = 0;
    v70 = IsTerrainSlopeTooHigh(v0->vPosition.x, v0->vPosition.y);
    v5 = ODM_GetFloorLevel(
           v0->vPosition.x,
           v0->vPosition.y,
           v0->vPosition.z,
           v0->uActorHeight,
           &uIsOnWater,
           (int *)&v69,
           v3);
    v6 = v0->vPosition.z;
    uIsAboveFloor = 0;
    v67 = v69 == 0;
    if ( v6 > v5 + 1 )
      uIsAboveFloor = 1;
    if ( uAIState == Dead && uIsOnWater && !uIsAboveFloor )
    {
      v0->uAIState = Removed;
	  continue;
    }
    if ( v0->uCurrentActionAnimation == ANIM_Walking )
    {
      v8 = v0->uMovementSpeed;
      if ( (signed __int64)v0->pActorBuffs[7].uExpireTime > 0 )
      {
        v8 = (signed __int64)((double)v8 * 0.5);
      }
      if ( uAIState == Fleeing || uAIState == Pursuing )
      {
        v8 *= 2;
      }
      if ( pParty->bTurnBasedModeOn == 1 && pTurnEngine->turn_stage == 1 )
        v8 *= flt_6BE3AC_debug_recmod1_x_1_6;
      if ( v8 > 1000 )
        v8 = 1000;

      v0->vVelocity.x = fixpoint_mul(stru_5C6E00->Cos(v0->uYawAngle), v8);
      v0->vVelocity.y = fixpoint_mul(stru_5C6E00->Sin(v0->uYawAngle), v8);
      if ( uIsFlying )
      {
        v0->vVelocity.z = fixpoint_mul(stru_5C6E00->Sin(v0->uPitchAngle), v8);
      }
      //v7 = v68;
    }
    else
    {
      v0->vVelocity.x = fixpoint_mul(55000, v0->vVelocity.x);
      v0->vVelocity.y = fixpoint_mul(55000, v0->vVelocity.y);
      if ( uIsFlying )
      {
        v0->vVelocity.z = fixpoint_mul(55000, v0->vVelocity.z);
      }
    }
    if ( v0->vPosition.z < v5 )
    {
      v16 = uIsFlying;
      v0->vPosition.z = v5;
      v0->vVelocity.z = v16 != 0 ? 0x14 : 0;
    }
    //v17 = 0;
    if ( !uIsAboveFloor || uIsFlying )
    {
      if ( v70 && !uIsAboveFloor && v67 )
      {
        v0->vPosition.z = v5;
        ODM_GetTerrainNormalAt(v0->vPosition.x, v0->vPosition.y, &v62);
        v20 = GetGravityStrength();
        //v21 = v62.y;
        //v22 = v62.z;
        //v23 = v62.y * v0->vVelocity.y;
        v0->vVelocity.z += -8 * LOWORD(pEventTimer->uTimeElapsed) * v20;
        int v73 = abs(v62.x * v0->vVelocity.x + v62.z * v0->vVelocity.z + v62.y * v0->vVelocity.y) >> 16;
        //v72b = v21;
        v0->vVelocity.x += fixpoint_mul(v73, v62.x);
        v0->vVelocity.y += fixpoint_mul(v73, v62.y);
        v0->vVelocity.z += fixpoint_mul(v73, v62.z);
        //v17 = 0;
      }
    }
    else
    {
      v0->vVelocity.z -= LOWORD(pEventTimer->uTimeElapsed) * GetGravityStrength();
    }
    if ( pParty->armageddon_timer != 0 && v0->CanAct() )
    {
      v0->vVelocity.x += rand() % 100 - 50;
      v0->vVelocity.y += rand() % 100 - 50;
      v0->vVelocity.z += rand() % 100 - 20;
      v25 = rand();
      v0->uAIState = Stunned;
      v0->uYawAngle += v25 % 32 - 16;
      v0->UpdateAnimation();
    }
    if ( v0->vVelocity.x * v0->vVelocity.x + v0->vVelocity.y * v0->vVelocity.y < 400 && v70 == 0 )
    {
      v0->vVelocity.y = 0;
      v0->vVelocity.x = 0;
    }
    stru_721530.field_0 = 1;
    if ( !uIsFlying )
      v26 = 40;
    else
      v26 = v0->uActorRadius;

    stru_721530.field_84 = -1;
    stru_721530.field_8_radius = v26;
    stru_721530.prolly_normal_d = v26;
    stru_721530.height = v0->uActorHeight;
    stru_721530.field_70 = 0;
    v69 = 0;
    while ( 1 )
    {
      stru_721530.position.x = v0->vPosition.x;
      stru_721530.normal.x = stru_721530.position.x;
      stru_721530.position.y = v0->vPosition.y;
      stru_721530.normal.y = stru_721530.position.y;
      v28 = v0->vPosition.z;
      stru_721530.normal.z = v28 + v26 + 1;
      stru_721530.position.z = v28 - v26 + stru_721530.height - 1;
      if ( stru_721530.position.z < stru_721530.normal.z )
        stru_721530.position.z = v28 + v26 + 1;
      stru_721530.velocity.x = v0->vVelocity.x;
      stru_721530.uSectorID = 0;
      stru_721530.velocity.y = v0->vVelocity.y;
      stru_721530.velocity.z = v0->vVelocity.z;
      if ( stru_721530._47050A(0) )
        break;
      _46E889_collide_against_bmodels(1u);
      v29 = WorldPosToGridCellZ(v0->vPosition.y);
      v30 = WorldPosToGridCellX(v0->vPosition.x);
      _46E26D_collide_against_sprites(v30, v29);
      _46EF01_collision_chech_player(0);
      _46ED8A_collide_against_sprite_objects(PID(OBJECT_Actor,v75));
      v31 = 0;
      for ( i = 0; v31 < ai_arrays_size; ++v31 )
      {
        v33 = ai_near_actors_ids[v31];
        if ( v33 != v75 && _46DF1A_collide_against_actor(v33, 40) )
          ++i;
      }
      v71 = i > 1;
      if ( stru_721530.field_7C < stru_721530.field_6C )
        v70 = fixpoint_mul(stru_721530.field_7C, stru_721530.direction.z);
      //v34 = 0;
      v35 = stru_721530.normal2.z - stru_721530.prolly_normal_d - 1;
      v36 = ODM_GetFloorLevel(stru_721530.normal2.x,
              stru_721530.normal2.y,
              stru_721530.normal2.z - stru_721530.prolly_normal_d - 1,
              v0->uActorHeight,
              (int *)&v63,
              &v64,
              0);
      if ( uIsOnWater )
      {
        if ( v35 < v36 + 60 )
        {
          if ( uAIState == Dead || uAIState == Dying || uAIState == Removed || uAIState == Disabled )
          {
            if ( v64 )
              v61 = v36 + 30;
            else
              v61 = v5 + 60;
            sub_42F960_create_object(v0->vPosition.x, v0->vPosition.y, v61);
            v0->uAIState = Removed;
            return;
          }
        }
      }
      if ( stru_721530.field_7C >= stru_721530.field_6C )
      {
        v0->vPosition.x = LOWORD(stru_721530.normal2.x);
        v0->vPosition.y = LOWORD(stru_721530.normal2.y);
        v0->vPosition.z = LOWORD(stru_721530.normal2.z) - LOWORD(stru_721530.prolly_normal_d) - 1;
        break;
      }
      //v72b = fixpoint_mul(stru_721530.field_7C, stru_721530.field_58.x);
      v0->vPosition.x += fixpoint_mul(stru_721530.field_7C, stru_721530.direction.x);
      //v72b = (unsigned __int64)(stru_721530.field_7C * (signed __int64)stru_721530.field_58.y) >> 16;
      v0->vPosition.y += fixpoint_mul(stru_721530.field_7C, stru_721530.direction.y);
      //v72b = (unsigned __int64)(stru_721530.field_7C * (signed __int64)stru_721530.field_58.z) >> 16;
      v0->vPosition.z += fixpoint_mul(stru_721530.field_7C, stru_721530.direction.z);
      v38 = stru_721530.uFaceID;
      stru_721530.field_70 += stru_721530.field_7C;
	  v39 = PID_ID(v38);
      switch ( PID_TYPE(v38) )
      {
        case OBJECT_Actor:
          if ( pTurnEngine->turn_stage != 2 && pTurnEngine->turn_stage != 3 || pParty->bTurnBasedModeOn != 1 )
          {
			//if(pParty->bTurnBasedModeOn == 1)
				//v34 = 0;
            if ( v0->pMonsterInfo.uHostilityType )
            {
              if ( v71 == 0 )
			  {
				  Actor::AI_Flee(v75, v38, 0, (AIDirection *)0);
			  }
			  else
			  {
				  Actor::AI_StandOrBored(v75, 4, 0, (AIDirection *)0);
			  }
            }
            else if ( v71 )
			{
              Actor::AI_StandOrBored(v75, 4, 0, (AIDirection *)0);
			}
			else if ( pActors[v39].pMonsterInfo.uHostilityType == MonsterInfo::Hostility_Friendly )
            {
              Actor::AI_Flee(v75, v38, 0, (AIDirection *)0);
            }
			else
			{
				Actor::AI_FaceObject(v75, v38, 0, (AIDirection *)0);
			}
          }
          break;
        case OBJECT_Player:
          if ( !v0->GetActorsRelation(0) )
          {
            v38 = stru_721530.uFaceID;
            Actor::AI_FaceObject(v75, v38, 0, (AIDirection *)0);
            break;
          }
          //v52 = HIDWORD(pParty->pPartyBuffs[PARTY_BUFF_INVISIBILITY].uExpireTime) == 0;
          //v53 = SHIDWORD(pParty->pPartyBuffs[PARTY_BUFF_INVISIBILITY].uExpireTime) < 0;
          v0->vVelocity.y = 0;
          v0->vVelocity.x = 0;
          //if ( !v53 && (!(v53 | v52) || LODWORD(pParty->pPartyBuffs[PARTY_BUFF_INVISIBILITY].uExpireTime) > 0) )
		  if ( (signed __int64)pParty->pPartyBuffs[PARTY_BUFF_INVISIBILITY].uExpireTime < 0)
            pParty->pPartyBuffs[PARTY_BUFF_INVISIBILITY].Reset();
          viewparams->bRedrawGameUI = 1;
          break;
        case OBJECT_Decoration:
          v47 = integer_sqrt(v0->vVelocity.x * v0->vVelocity.x + v0->vVelocity.y * v0->vVelocity.y);
          v48 = stru_5C6E00->Atan2(
                  v0->vPosition.x - pLevelDecorations[v39].vPosition.x,
                  v0->vPosition.y - pLevelDecorations[v39].vPosition.y);
          v49 = v48;
          v0->vVelocity.x = fixpoint_mul(stru_5C6E00->Cos(v48), v47);
          v0->vVelocity.y = fixpoint_mul(stru_5C6E00->Sin(v48), v47);
          break;
        case OBJECT_BModel:
          v40 = &pOutdoor->pBModels[v38 >> 9];
          v41 = &v40->pFaces[v39 & 0x3F];
          if ( !(v41->uAttributes & 0x20000000) )
          {
            v42 = v41->uPolygonType;
            if ( v42 == 3 )
            {
              v43 = v0->vVelocity.y;
              v44 = v0->vVelocity.x;
              v0->vVelocity.z = 0;
              v0->vPosition.z = LOWORD(v40->pVertices.pVertices[v41->pVertexIDs[0]].z) + 1;
              if ( v44 * v44 + v43 * v43 < 400 )
              {
                v0->vVelocity.y = 0;
                v0->vVelocity.x = 0;
              }
            }
            else
            {
              v72b = abs(v41->pFacePlane.vNormal.y * v0->vVelocity.y + v41->pFacePlane.vNormal.z * v0->vVelocity.z
                                                                    + v41->pFacePlane.vNormal.x * v0->vVelocity.x) >> 16;
              if ( (stru_721530.speed >> 3) > v72b )
                v72b = stru_721530.speed >> 3;

              v0->vVelocity.x += fixpoint_mul(v72b, v41->pFacePlane.vNormal.x);
              v0->vVelocity.y += fixpoint_mul(v72b, v41->pFacePlane.vNormal.y);
              v0->vVelocity.z += fixpoint_mul(v72b, v41->pFacePlane.vNormal.z);
              if ( v42 != 4 )
              {
                v45 = v0->vPosition.z;
                v46 = stru_721530.prolly_normal_d
                    - ((v41->pFacePlane.dist
                      + v41->pFacePlane.vNormal.x * v0->vPosition.x
                      + v41->pFacePlane.vNormal.y * v0->vPosition.y
                      + v41->pFacePlane.vNormal.z * v0->vPosition.z) >> 16);
                if ( v46 > 0 )
                {
                  v0->vPosition.x += (unsigned int)(v46 * v41->pFacePlane.vNormal.x) >> 16;
                  v0->vPosition.y += (unsigned int)(v46 * v41->pFacePlane.vNormal.y) >> 16;
                  v0->vPosition.z = v45 + ((unsigned int)(v46 * v41->pFacePlane.vNormal.z) >> 16);
                }
                v0->uYawAngle = stru_5C6E00->Atan2(v0->vVelocity.x, v0->vVelocity.y);
              }
            }
          }
          break;
      }

      v0->vVelocity.x = fixpoint_mul(58500, v0->vVelocity.x);
      v0->vVelocity.y = fixpoint_mul(58500, v0->vVelocity.y);
      v0->vVelocity.z = fixpoint_mul(58500, v0->vVelocity.z);

      ++v69;
      if ( v69 >= 100 )
        break;
      v26 = stru_721530.prolly_normal_d;
    }
    v58 = ((unsigned int)~pOutdoor->ActuallyGetSomeOtherTileInfo(WorldPosToGridCellX(v66), WorldPosToGridCellZ(v65) - 1) >> 1) & 1;
    v59 = ((unsigned int)~pOutdoor->ActuallyGetSomeOtherTileInfo(WorldPosToGridCellX(v0->vPosition.x), WorldPosToGridCellZ(v0->vPosition.y) - 1) >> 1) & 1;
    if ( WorldPosToGridCellX(v66) == WorldPosToGridCellX(v0->vPosition.x) 
		&& WorldPosToGridCellZ(v65) == WorldPosToGridCellZ(v0->vPosition.y)
		&& v58 
		|| v67 != 0 )
    {
      if ( MonsterStats::BelongsToSupertype(v0->pMonsterInfo.uID, MONSTER_SUPERTYPE_WATER_ELEMENTAL) )
      {
        v58 = v58 == 0;
        v59 = v59 == 0;
      }
      if ( !uIsFlying && v58 && !v59 )
      {
        v0->vPosition.x = v66;
        v0->vPosition.y = v65;
        if ( v0->CanAct() )
        {
          v0->uYawAngle -= 32;
          v0->uCurrentActionTime = 0;
          v0->uCurrentActionLength = 128;
          v0->uAIState = Fleeing;
        }
      }
    }
  }
}

//----- (0047253E) --------------------------------------------------------
void UpdateObjects()
{
  unsigned int v0; // ebx@0
  //unsigned int v1; // edi@1
  //char *v2; // esi@2
  //__int16 v3; // dx@5
  ObjectDesc *v4; // eax@5
  int v5; // ecx@6
  __int16 v6; // cx@7
  signed int v7; // eax@9
  __int16 v8; // cx@14
  __int16 v9; // dx@14
  unsigned int v10; // ecx@16
  signed int v11; // eax@17
  int v12; // edi@27
  int v13; // eax@27
  int v14; // ebx@27
  unsigned int v15; // ecx@27
  unsigned int v16; // edx@30
  unsigned int v17; // edx@32
  int v18; // [sp+4h] [bp-10h]@27
  int v19; // [sp+8h] [bp-Ch]@27
  //signed int v20; // [sp+10h] [bp-4h]@1

  //v1 = 0;
  //v20 = 0;
  for (uint i = 0; i < uNumSpriteObjects; ++i)
  {
    auto item = &pSpriteObjects[i];
    //v2 = (char *)&item->uSpriteFrameID;
    //do
    //{
      //LOWORD(v0) = item->uAttributes;
    if (item->uAttributes & OBJECT_40)
      item->uAttributes &= ~OBJECT_40;
    else
    {
              //v3 = item->uObjectDescID;
      v4 = &pObjectList->pObjects[item->uObjectDescID];
      if (item->AttachedToActor())
      {
          v5 = PID_ID(item->spell_target_pid);
		  item->vPosition.x = pActors[v5].vPosition.x;
          item->vPosition.y = pActors[v5].vPosition.y;
          item->vPosition.z = pActors[v5].vPosition.z + pActors[v5].uActorHeight;
		  if ( !item->uObjectDescID || (v6 = LOWORD(pEventTimer->uTimeElapsed) + item->uSpriteFrameID, item->uSpriteFrameID = v6, !(v4->uFlags & 4)) )
            continue;
          if ( v6 >= 0 )
          {
            v7 = v4->uLifetime;
            if (item->uAttributes & 2)
              v7 = item->field_20;
            if ( v6 < v7 )
              continue;
          }
            v10 = i;
            SpriteObject::OnInteraction(v10);
            continue;
      }

        if ( item->uObjectDescID )
        {
          v8 = LOWORD(pEventTimer->uTimeElapsed) + item->uSpriteFrameID;
          item->uSpriteFrameID = v8;
          v9 = v4->uFlags;
          if ( v9 & 4 )
		  {    
			  if ( v8 < 0 )
			  {
				v10 = i;
				SpriteObject::OnInteraction(v10);
				continue;
			  }
			  v11 = v4->uLifetime;
			  if (item->uAttributes & 2)
				v11 = item->field_20;
		  }
		  if ( !(v9 & 4) || v8 < v11 )
          {
            if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
              SpriteObject::UpdateObject_fn0_BLV(i);
            else
              SpriteObject::UpdateObject_fn0_ODM(i);
			if ( pParty->bTurnBasedModeOn != 1 || !(item->uSectorID & 4) )
              continue;
			v12 = abs(pParty->vPosition.x - item->vPosition.x);
			v18 = abs(pParty->vPosition.y - item->vPosition.y);
			v19 = abs(pParty->vPosition.z - item->vPosition.z);
            v13 = v12;
            v14 = v18;
            v15 = v19;
            if ( v12 < v18 )
            {
              v13 = v18;
              v14 = v12;
            }
            if ( v13 < v19 )
            {
              v16 = v13;
              v13 = v19;
              v15 = v16;
            }
            if ( v14 < (signed int)v15 )
            {
              v17 = v15;
              v15 = v14;
              v14 = v17;
            }
            v0 = (unsigned int)(11 * v14) >> 5;
            if ( (signed int)(v0 + (v15 >> 2) + v13) <= 5120 )
              continue;
            v10 = i;
            SpriteObject::OnInteraction(v10);
            continue;
          }
          v10 = i;
          if ( !(v9 & 0x40) )
		  {
            SpriteObject::OnInteraction(v10);
            continue;
		  }
          _46BFFA_check_object_intercept(i, PID(OBJECT_Item,i));
        }
    }
      //v2 += 112;
      //v1 = v20++ + 1;
    //}
    //while ( v20 < (signed int)uNumSpriteObjects );
  }
}

//----- (0047272C) --------------------------------------------------------
int collide_against_floor_approximate(int x, int y, int z, unsigned int *pSectorID, unsigned int *pFaceID)
{
  signed int v5; // ebx@1
  int result; // eax@1
  int v7; // [sp+10h] [bp-8h]@1
  signed int v8; // [sp+14h] [bp-4h]@1

  v5 = x - 2;
  v7 = x;
  v8 = y;
  *pSectorID = pIndoor->GetSector(x - 2, y, z + 40);
  result = collide_against_floor(v5, v8, z + 40, pSectorID, pFaceID);
  if ( result == -30000 || !*pSectorID )
  {
    *pSectorID = pIndoor->GetSector(v7 + 2, v8, z + 40);
    result = collide_against_floor(v7 + 2, v8, z + 40, pSectorID, pFaceID);
    if ( result == -30000 || !*pSectorID )
    {
      *pSectorID = pIndoor->GetSector(v7, v8 - 2, z + 40);
      result = collide_against_floor(v7, v8 - 2, z + 40, pSectorID, pFaceID);
      if ( result == -30000 || !*pSectorID )
      {
        *pSectorID = pIndoor->GetSector(v7, v8 + 2, z + 40);
        result = collide_against_floor(v7, v8 + 2, z + 40, pSectorID, pFaceID);
        if ( result == -30000 || !*pSectorID )
        {
          *pSectorID = pIndoor->GetSector(v7, v8, z + 140);
          result = collide_against_floor(v7, v8, z + 140, pSectorID, pFaceID);
        }
      }
    }
  }
  return result;
}

//----- (00472866) --------------------------------------------------------
void BLV_ProcessPartyActions()
{
  int v1; // ebx@1
  int v2; // edi@1
  //int v6; // eax@18
  //signed __int64 v8; // qax@27
  //int v9; // eax@27
  double v10; // st7@27
  unsigned int v12; // eax@49
  double v13; // st7@50
  //int v14; // eax@51
  //signed __int64 v15; // qax@53
  //double v16; // st7@54
  int v17; // eax@62
  double v18; // st7@62
  int v19; // ST40_4@62
  int v20; // eax@65
  double v21; // st7@65
  int v22; // ST40_4@65
  int v23; // eax@66
  double v24; // st7@66
  int v25; // ST40_4@66
  int v26; // eax@67
  double v27; // st7@67
  int v28; // ST40_4@67
  unsigned int v35; // eax@74
  int v36; // ecx@88
  int new_party_z; // esi@96
  int v38; // eax@96
  int v39; // ecx@106
  int v40; // eax@106
  Player **v41; // esi@113
  int v42; // eax@120
  signed int v43; // ecx@123
  BLVFace *v44; // esi@126
  PolygonType v45; // al@126
  int v46; // ecx@133
  char v47; // zf@133
  int v48; // eax@133
  int v49; // ST40_4@136
  int v50; // ST40_4@140
  int v51; // eax@140
  int v52; // eax@140
  signed int v53; // edi@146
  int v54; // ebx@146
  int v55; // ST30_4@146
  AudioPlayer *v56; // ecx@162
  SoundID v57; // [sp-20h] [bp-80h]@163
  signed int v58; // [sp-1Ch] [bp-7Ch]@161
  unsigned int v59; // [sp-18h] [bp-78h]@161
  signed int v60; // [sp-14h] [bp-74h]@161
  signed int v61; // [sp-10h] [bp-70h]@161
  int v62; // [sp-Ch] [bp-6Ch]@161
  int v63; // [sp-8h] [bp-68h]@75
  unsigned int v64; // [sp-8h] [bp-68h]@161
  int v65; // [sp-4h] [bp-64h]@75
  int v66; // [sp-4h] [bp-64h]@161
  //int v68; // [sp+10h] [bp-50h]@45
  int v69; // [sp+10h] [bp-50h]@140
  unsigned int uFaceEvent; // [sp+14h] [bp-4Ch]@1
  //signed int v71; // [sp+18h] [bp-48h]@1
  signed int v72; // [sp+1Ch] [bp-44h]@1
  signed int v73; // [sp+20h] [bp-40h]@100
  int v74; // [sp+20h] [bp-40h]@140
  bool bFeatherFall; // [sp+24h] [bp-3Ch]@15
  unsigned int uSectorID; // [sp+28h] [bp-38h]@1
  int v78; // [sp+2Ch] [bp-34h]@1
  unsigned int uFaceID; // [sp+30h] [bp-30h]@1
  int v80; // [sp+34h] [bp-2Ch]@1
  int v81; // [sp+38h] [bp-28h]@47
  int v82; // [sp+3Ch] [bp-24h]@47
  int _view_angle; // [sp+40h] [bp-20h]@47
  int bJumping; // [sp+44h] [bp-1Ch]@1
  int new_party_y; // [sp+48h] [bp-18h]@1
  int new_party_x; // [sp+4Ch] [bp-14h]@1
  int party_z; // [sp+50h] [bp-10h]@1
  //int v89; // [sp+58h] [bp-8h]@1
  int angle; // [sp+5Ch] [bp-4h]@47

  uFaceEvent = 0;
  //v89 = pParty->uFallSpeed;
  v1 = 0;
  v2 = 0;
  new_party_x = pParty->vPosition.x;
  new_party_y = pParty->vPosition.y;
  party_z = pParty->vPosition.z;
  uSectorID = pIndoor->GetSector(pParty->vPosition.x, pParty->vPosition.y, pParty->vPosition.z);
  v72 = 0;
  v78 = 0;
  bJumping = 0;

  uFaceID = -1;
  auto floor_level = collide_against_floor(new_party_x, new_party_y, party_z + 40, &uSectorID, &uFaceID);

  if ( pParty->bFlying )
  {
    pParty->bFlying = false;
    if (pParty->FlyActive())
      pOtherOverlayList->pOverlays[pParty->pPartyBuffs[PARTY_BUFF_FLY].uOverlayID - 1].field_E |= 1;
  }

  if ( floor_level == -30000  || uFaceID == -1)
  {
    floor_level = collide_against_floor_approximate(new_party_x, new_party_y, party_z + 40, &uSectorID, &uFaceID);
    if ( floor_level == -30000 || uFaceID == -1)
    {
      __debugbreak(); // level built with errors 
      pParty->vPosition.x = blv_prev_party_x;
      pParty->vPosition.y = blv_prev_party_z;
      pParty->vPosition.z = blv_prev_party_y;
      pParty->uFallStartY = blv_prev_party_y;
      return;
    }
  }

  blv_prev_party_x = pParty->vPosition.x;
  blv_prev_party_z = pParty->vPosition.y;
  blv_prev_party_y = pParty->vPosition.z;
  if (!pParty->bTurnBasedModeOn)
  {
    auto v67 = GetTickCount() / 500;
    if (dword_720CDC != v67 )
    {
      dword_4F8580[3 * dword_4F8580[1]] = pParty->vPosition.x;
      dword_4F8580[3 * dword_4F8580[2]] = pParty->vPosition.y;
      dword_4F8580[3 * dword_4F8580[3]] = pParty->vPosition.z;
      if ( dword_4F8580[0] > 60 )
        dword_4F8580[0] = 1;
    }
    dword_720CDC = v67;
  }

  uint fall_start;
  if (!pParty->FeatherFallActive())
  {
    bFeatherFall = false;
    if (!pParty->pPlayers[0].WearsItemAnyWhere(ITEM_ARTIFACT_LADYS_ESCORT) &&  // grants feather fall
        !pParty->pPlayers[1].WearsItemAnyWhere(ITEM_ARTIFACT_LADYS_ESCORT) &&
        !pParty->pPlayers[2].WearsItemAnyWhere(ITEM_ARTIFACT_LADYS_ESCORT) &&
        !pParty->pPlayers[3].WearsItemAnyWhere(ITEM_ARTIFACT_LADYS_ESCORT))
    {
      fall_start = pParty->uFallStartY;
    }
  }
  else
  {
    fall_start = floor_level;
    bFeatherFall = true;
    pParty->uFallStartY = floor_level;
  }


  if (fall_start - party_z > 512 && !bFeatherFall && party_z <= floor_level + 1)
  {
    assert(~pParty->uFlags & PARTY_FLAGS_1_LANDING); // why land in indoor?
    if (pParty->uFlags & PARTY_FLAGS_1_LANDING)
      pParty->uFlags &= ~PARTY_FLAGS_1_LANDING;
    else for (uint i = 0; i < 4; ++i)
    {                                      // receive falling damage
      auto player = &pParty->pPlayers[i];
      if (!player->HasEnchantedItemEquipped(72) && !player->WearsItem(ITEM_ARTIFACT_HERMES_SANDALS, EQUIP_BOOTS))
      {
        player->ReceiveDamage((pParty->uFallStartY - party_z) * (0.1f * player->GetMaxHealth()) / 256, DMGT_PHISYCAL);
        v10 = (double)(20 - player->GetParameterBonus(player->GetActualEndurance())) * flt_6BE3A4_debug_recmod1 * 2.133333333333333;
        player->SetRecoveryTime((signed __int64)v10);
      }
    }
  }

  if ( party_z > floor_level + 1 )
    bJumping = 1;

  bool jumping_up = false;

  if ( party_z - floor_level <= 32 )
  {
    pParty->uFallStartY = party_z;
    jumping_up = true;
  }

  if (bWalkSound && pParty->walk_sound_timer)
  {
    if (pParty->walk_sound_timer > pEventTimer->uTimeElapsed)
      pParty->walk_sound_timer -= pEventTimer->uTimeElapsed;
    else pParty->walk_sound_timer = 0;
  }


  if (party_z <= floor_level + 1)
  {
    party_z = floor_level + 1;
    pParty->uFallStartY = floor_level + 1;

    if (!bJumping && pParty->floor_face_pid != uFaceID)
    {
      auto pFace = &pIndoor->pFaces[uFaceID];
      if (pFace->uAttributes & FACE_PRESSURE_PLATE)
        uFaceEvent = pIndoor->pFaceExtras[pFace->uFaceExtraID].uEventID;
    }
  }
  if (!bJumping)
    pParty->floor_face_pid = uFaceID;

  bool on_water = false;
  if ( pIndoor->pFaces[uFaceID].Fluid())
    on_water = true;

  v81 = pParty->uWalkSpeed;
  angle = pParty->sRotationY;
  _view_angle = pParty->sRotationX;
  v82 = (unsigned __int64)(pEventTimer->dt_in_some_format * (signed __int64)((signed int)(pParty->y_rotation_speed * stru_5C6E00->uIntegerPi)
                                          / 180)) >> 16;
//LABEL_87:
  while ( pPartyActionQueue->uNumActions )
  {
    switch ( pPartyActionQueue->Next() )
    {
      case PARTY_TurnLeft:
        if (uTurnSpeed)
          angle = stru_5C6E00->uDoublePiMask & (angle + uTurnSpeed);
        else
          angle = stru_5C6E00->uDoublePiMask & (angle + (int)(v82 * fTurnSpeedMultiplier));
        break;
      case PARTY_TurnRight:
        if (uTurnSpeed)
          angle = stru_5C6E00->uDoublePiMask & (angle - uTurnSpeed);
        else
          angle = stru_5C6E00->uDoublePiMask & (angle - (int)(v82 * fTurnSpeedMultiplier));
        break;

      case PARTY_FastTurnLeft:
        if (uTurnSpeed)
          angle = stru_5C6E00->uDoublePiMask & (angle + uTurnSpeed);
        else
          angle = stru_5C6E00->uDoublePiMask & (angle + (int)(2.0f * fTurnSpeedMultiplier * (double)v82));
        break;

      case PARTY_FastTurnRight:
        if (uTurnSpeed)
          angle = stru_5C6E00->uDoublePiMask & (angle - uTurnSpeed);
        else
          angle = stru_5C6E00->uDoublePiMask & (angle - (int)(2.0f * fTurnSpeedMultiplier * (double)v82));
        break;

      case PARTY_StrafeLeft:
        v2 -= fixpoint_mul(stru_5C6E00->Sin(angle), v81 * fWalkSpeedMultiplier / 2);
        v1 += fixpoint_mul(stru_5C6E00->Cos(angle), v81 * fWalkSpeedMultiplier / 2);
        v78 = 1;
        break;
      case PARTY_StrafeRight:
        v2 += fixpoint_mul(stru_5C6E00->Sin(angle), v81 * fWalkSpeedMultiplier / 2);
        v1 -= fixpoint_mul(stru_5C6E00->Cos(angle), v81 * fWalkSpeedMultiplier / 2);
        v78 = 1;
        break;
      case PARTY_WalkForward:
        v2 += fixpoint_mul(stru_5C6E00->Cos(angle), 5 * v81 * fWalkSpeedMultiplier);
        v1 += fixpoint_mul(stru_5C6E00->Sin(angle), 5 * v81 * fWalkSpeedMultiplier);
        v78 = 1;
        break;
      case PARTY_WalkBackward:
        v2 -= fixpoint_mul(stru_5C6E00->Cos(angle), v81 * fBackwardWalkSpeedMultiplier);
        v1 -= fixpoint_mul(stru_5C6E00->Sin(angle), v81 * fBackwardWalkSpeedMultiplier);
        v78 = 1;
        break;
      case PARTY_RunForward:
        v2 += fixpoint_mul(stru_5C6E00->Cos(angle), 2 * v81 * fWalkSpeedMultiplier);
        v1 += fixpoint_mul(stru_5C6E00->Sin(angle), 2 * v81 * fWalkSpeedMultiplier);
        v72 = 1;
        break;
      case PARTY_RunBackward:
        //v32 = stru_5C6E00->SinCos(angle);
        //v33 = (double)v81;
        //v88 = (double)v81;
        v2 -= fixpoint_mul(stru_5C6E00->Cos(angle), v81 * fBackwardWalkSpeedMultiplier);
        //v34 = stru_5C6E00->SinCos(angle - stru_5C6E00->uIntegerHalfPi);
        v1 -= fixpoint_mul(stru_5C6E00->Sin(angle), v81 * fBackwardWalkSpeedMultiplier);
        v72 = 1;
        break;
      case PARTY_LookUp:
        _view_angle += (signed __int64)(flt_6BE150_look_up_down_dangle * 25.0);
        if ( _view_angle > 128 )
          _view_angle = 128;
        v35 = uActiveCharacter;
        if ( uActiveCharacter )
          pPlayers[v35]->PlaySound((PlayerSpeech)SPEECH_63, 0);
        break;
      case PARTY_LookDown:
        _view_angle += (signed __int64)(flt_6BE150_look_up_down_dangle * -25.0);
        if ( _view_angle < -128 )
          _view_angle = -128;
        v35 = uActiveCharacter;
        if ( uActiveCharacter )
          pPlayers[v35]->PlaySound((PlayerSpeech)SPEECH_64, 0);
        break;
      case PARTY_CenterView:
        _view_angle = 0;
        break;
      case PARTY_Jump:
        if ( (!bJumping || party_z <= floor_level + 6 && pParty->uFallSpeed <= 0) && pParty->field_24 )
        {
          bJumping = 1;
          pParty->uFallSpeed = (signed __int64)((double)(pParty->field_24 << 6) * 1.5 + (double)pParty->uFallSpeed);
        }
        break;
      default:
        break;
    }
  }
  v36 = 0;
  pParty->sRotationY = angle;
  pParty->sRotationX = _view_angle;
  if ( bJumping )
  {
	pParty->uFallSpeed += -2 * pEventTimer->uTimeElapsed * GetGravityStrength();
	if ( bJumping != v36 && pParty->uFallSpeed <= v36 )
	{
		if ( pParty->uFallSpeed < -500 && !pParty->bFlying )
		{
			v41 = &pPlayers[1];
			do
			{
				if ( !(*v41)->HasEnchantedItemEquipped(72) && !(*v41)->WearsItem(ITEM_ARTIFACT_HERMES_SANDALS, EQUIP_BOOTS) ) //was 8 
					(*v41)->PlayEmotion(CHARACTER_EXPRESSION_SCARED, 0);
				++v41;
			}
			while ( (signed int)v41 <= (signed int)&pPlayers[4] );
		}
	}
	else
		pParty->uFallStartY = party_z;
  }
  else
  {
    if ( pIndoor->pFaces[uFaceID].pFacePlane_old.vNormal.z < 32768 )
    {
      pParty->uFallSpeed -= pEventTimer->uTimeElapsed * GetGravityStrength();
      pParty->uFallStartY = party_z;
    }
	else
	{
		if (! (pParty->uFlags & PARTY_FLAGS_1_LANDING) )
		  pParty->uFallSpeed = 0;
		pParty->uFallStartY = party_z;
	}
  }
  if ( v2 * v2 + v1 * v1 < 400 )
  {
    v1 = 0;
    v2 = 0;
  }
  stru_721530.field_84 = -1;
  stru_721530.field_70 = 0;
  stru_721530.prolly_normal_d = pParty->field_14_radius;
  stru_721530.field_8_radius = pParty->field_14_radius / 2;
  auto v83 = 0;
  stru_721530.field_0 = 1;
  stru_721530.height = pParty->uPartyHeight - 32;
  while ( 1 )
  {
    new_party_z = party_z;
    stru_721530.position.x = new_party_x;
    stru_721530.normal.x = new_party_x;
    stru_721530.velocity.x = v2;
    stru_721530.position.y = new_party_y;
    stru_721530.normal.y = new_party_y;
    stru_721530.velocity.y = v1;
    stru_721530.normal.z = stru_721530.prolly_normal_d + party_z + 1;
    stru_721530.position.z = stru_721530.height + party_z + 1;
    stru_721530.velocity.z = pParty->uFallSpeed;
    stru_721530.uSectorID = uSectorID;
    v38 = 0;
    if ( pParty->bTurnBasedModeOn == 1 && pTurnEngine->turn_stage == 3 )
      v38 = 13312;
    if ( stru_721530._47050A(v38) )
      break;
    v73 = 0;
    do
    {
      _46E44E_collide_against_faces_and_portals(1u);
      _46E0B2_collide_against_decorations();
      for ( v80 = 0; v80 < (signed int)uNumActors; ++v80 )
        _46DF1A_collide_against_actor(v80, 0);
      if ( _46F04E_collide_against_portals() )
        break;
      ++v73;
    }
    while ( v73 < 100 );
    if ( stru_721530.field_7C >= stru_721530.field_6C )
    {
      v39 = stru_721530.normal2.x;
      uSectorID = stru_721530.normal2.y;
      v40 = stru_721530.normal2.z - stru_721530.prolly_normal_d - 1;
    }
    else
    {
      v39 = ((unsigned __int64)(stru_721530.field_7C * (signed __int64)stru_721530.direction.x) >> 16) + new_party_x;
      uSectorID = new_party_y + ((unsigned __int64)(stru_721530.field_7C * (signed __int64)stru_721530.direction.y) >> 16);
      v40 = new_party_z + ((unsigned __int64)(stru_721530.field_7C * (signed __int64)stru_721530.direction.z) >> 16);
    }
    v42 = collide_against_floor(v39, uSectorID, v40 + 40, &stru_721530.uSectorID, &uFaceID);
    if ( v42 == -30000 || v42 - new_party_z > 128 )
      return;
    if ( stru_721530.field_7C >= stru_721530.field_6C )
    {
      new_party_x = stru_721530.normal2.x;
      new_party_y = stru_721530.normal2.y;
      new_party_z = stru_721530.normal2.z - stru_721530.prolly_normal_d - 1;
      break;
    }
    new_party_x += (unsigned __int64)(stru_721530.field_7C * (signed __int64)stru_721530.direction.x) >> 16;
    new_party_y += (unsigned __int64)(stru_721530.field_7C * (signed __int64)stru_721530.direction.y) >> 16;
    v43 = stru_721530.uFaceID;
    uSectorID = stru_721530.uSectorID;
    stru_721530.field_70 += stru_721530.field_7C;
    auto v87 = ((unsigned __int64)(stru_721530.field_7C * (signed __int64)stru_721530.direction.z) >> 16) + new_party_z;
    if ( PID_TYPE(stru_721530.uFaceID) == OBJECT_Actor)
    {
      if ( SHIDWORD(pParty->pPartyBuffs[PARTY_BUFF_INVISIBILITY].uExpireTime) >= 0
        && (SHIDWORD(pParty->pPartyBuffs[PARTY_BUFF_INVISIBILITY].uExpireTime) > 0 || LODWORD(pParty->pPartyBuffs[PARTY_BUFF_INVISIBILITY].uExpireTime)) )
        pParty->pPartyBuffs[PARTY_BUFF_INVISIBILITY].Reset();
      viewparams->bRedrawGameUI = 1;
    }
	else if ( PID_TYPE(stru_721530.uFaceID) == OBJECT_Decoration)
    {
      v53 = integer_sqrt(v2 * v2 + v1 * v1);
      v80 = v53;
      v54 = stru_5C6E00->Atan2(new_party_x - pLevelDecorations[stru_721530.uFaceID >> 3].vPosition.x,
              new_party_y - pLevelDecorations[stru_721530.uFaceID >> 3].vPosition.y);
      v2 = (unsigned __int64)(stru_5C6E00->Cos(v54) * (signed __int64)v53) >> 16;
      v55 = stru_5C6E00->Sin(v54);
      v1 = (unsigned __int64)(v55 * (signed __int64)v80) >> 16;
    }
    else
    {
      if ( PID_TYPE(stru_721530.uFaceID) == OBJECT_BModel)
      {
        v44 = &pIndoor->pFaces[(signed int)stru_721530.uFaceID >> 3];
        v45 = v44->uPolygonType;
        if ( v45 == 3 )
        {
          if ( pParty->uFallSpeed < 0 )
            pParty->uFallSpeed = 0;
          v87 = pIndoor->pVertices[*v44->pVertexIDs].z + 1;
          if ( pParty->uFallStartY - v87 < 512 )
            pParty->uFallStartY = v87;
          if ( v2 * v2 + v1 * v1 < 400 )
          {
            v1 = 0;
            v2 = 0;
          }
          if ( pParty->floor_face_pid != PID_ID(v43) && BYTE3(v44->uAttributes) & 4 )
            uFaceEvent = pIndoor->pFaceExtras[v44->uFaceExtraID].uEventID;
        }
		else
		{
			v46 = pParty->uFallSpeed * v44->pFacePlane_old.vNormal.z;
			v47 = v45 == 4;
			v48 = v44->pFacePlane_old.vNormal.x;
			if ( !v47 )
			{
			  v80 = abs(v1 * v44->pFacePlane_old.vNormal.y + v46 + v2 * v48) >> 16;
			  if ((stru_721530.speed >> 3) > v80 )
				v80 = stru_721530.speed >> 3;
			  v50 = (unsigned __int64)(v80 * (signed __int64)v44->pFacePlane_old.vNormal.x) >> 16;
			  v81 = v44->pFacePlane_old.vNormal.y;
			  v81 = (unsigned __int64)(v80 * (signed __int64)v81) >> 16;
			  v82 = v44->pFacePlane_old.vNormal.z;
			  v82 = (unsigned __int64)(v80 * (signed __int64)v82) >> 16;
			  v2 += v50;
			  pParty->uFallSpeed += v82;
			  v74 = v44->pFacePlane_old.vNormal.z;
			  v51 = v44->pFacePlane_old.vNormal.y;
			  v69 = v44->pFacePlane_old.vNormal.x;
			  v80 = v51;
			  v1 += v81;
			  v52 = stru_721530.prolly_normal_d - ((v44->pFacePlane_old.dist + v87 * v74 + new_party_y * v51 + new_party_x * v69) >> 16);
			  if ( v52 > 0 )
			  {
				new_party_x += v52 * v69 >> 16;
				new_party_y += v52 * v80 >> 16;
				v87 += v52 * v74 >> 16;
			  }
			  v43 = stru_721530.uFaceID;
			  if ( pParty->floor_face_pid != PID_ID(v43) && BYTE3(v44->uAttributes) & 4 )
				uFaceEvent = pIndoor->pFaceExtras[v44->uFaceExtraID].uEventID;
			}
			else
			{
				v80 = abs(v1 * v44->pFacePlane_old.vNormal.y + v46 + v2 * v48) >> 16;
				if ((stru_721530.speed >> 3) > v80 )
				  v80 = stru_721530.speed >> 3;
				v49 = (unsigned __int64)(v80 * (signed __int64)v44->pFacePlane_old.vNormal.x) >> 16;
				v81 = v44->pFacePlane_old.vNormal.y;
				v81 = (unsigned __int64)(v80 * (signed __int64)v81) >> 16;
				v82 = v44->pFacePlane_old.vNormal.z;
				v82 = (unsigned __int64)(v80 * (signed __int64)v82) >> 16;
				v2 += v49;
				v1 += v81;
				pParty->uFallSpeed += v82;
				if ( v2 * v2 + v1 * v1 >= 400 )
				{
				  v43 = stru_721530.uFaceID;
				  if ( pParty->floor_face_pid != PID_ID(v43) && BYTE3(v44->uAttributes) & 4 )
					uFaceEvent = pIndoor->pFaceExtras[v44->uFaceExtraID].uEventID;
				}
				else
				{
					v2 = 0;
					v1 = 0;
					pParty->uFallSpeed = 0;
				}
			}
		}
      }
    }
    v2 = (unsigned __int64)(58500i64 * v2) >> 16;
    v1 = (unsigned __int64)(58500i64 * v1) >> 16;
    ++v83;
    pParty->uFallSpeed = (unsigned __int64)(58500i64 * pParty->uFallSpeed) >> 16;
    if ( v83 >= 100 )
    {
      new_party_z = v87;
      break;
    }
  }
  if ( bWalkSound && !pParty->walk_sound_timer)
  {
    if ( integer_sqrt((pParty->vPosition.x - new_party_x) * (pParty->vPosition.x - new_party_x) + (pParty->vPosition.y - new_party_y)
         * (pParty->vPosition.y - new_party_y) + (pParty->vPosition.z - new_party_z)
         * (pParty->vPosition.z - new_party_z)) <= 16 )
	{
      pAudioPlayer->_4AA258(804);
      pParty->walk_sound_timer = 64;
	}
    else if ( v72 && (!bJumping || jumping_up) )
    {
      v66 = 0;
      v64 = 0;
      v62 = 0;
      v61 = 0;
      v60 = -1;
      v59 = 1;
      v58 = 804;
      if ( !on_water )
      {
        v56 = pAudioPlayer;
        if ( BYTE2(pIndoor->pFaces[uFaceID].uAttributes) & 0x20 )
          v57 = (SoundID)50;
        else
          v57 = (SoundID)64;
        pAudioPlayer->PlaySound(v57, v58, v59, v60, v61, v62, v64, v66);
      }
	  else
	  {
		  v57 = (SoundID)63;
		  v56 = pAudioPlayer;
		  pAudioPlayer->PlaySound(v57, v58, v59, v60, v61, v62, v64, v66);
	  }
    }
    else if ( v78 && (!bJumping || jumping_up) )
    {
      v66 = 0;
      v64 = 0;
      v62 = 0;
      v61 = 0;
      v60 = -1;
      v59 = 1;
      v58 = 804;
      if ( on_water )
      {
        v57 = (SoundID)102;
        v56 = pAudioPlayer;
      }
	  else
	  {
		  v56 = pAudioPlayer;
		  if ( BYTE2(pIndoor->pFaces[uFaceID].uAttributes) & 0x20 )
			v57 = (SoundID)89;
		  else
			v57 = (SoundID)103;
	  }
      pAudioPlayer->PlaySound(v57, v58, v59, v60, v61, v62, v64, v66);
    }
    else
    {
      pAudioPlayer->_4AA258(804);
      pParty->walk_sound_timer = 64;
    }
  }
  if ( !bJumping || jumping_up )
    pParty->uFlags &= ~PARTY_FLAGS_1_FALLING;
  else
    pParty->uFlags |= PARTY_FLAGS_1_FALLING;
  pParty->uFlags &= ~0x200;
  pParty->vPosition.x = new_party_x;
  pParty->vPosition.z = new_party_z;
  pParty->vPosition.y = new_party_y;
  //pParty->uFallSpeed = v89;
  if ( !bJumping && BYTE3(pIndoor->pFaces[uFaceID].uAttributes) & 0x40 )
    pParty->uFlags |= 0x200;
  if (uFaceEvent)
    EventProcessor(uFaceEvent, 0, 1);
}

//----- (00473893) --------------------------------------------------------
void ODM_ProcessPartyActions()
{
  int v1; // edi@1
  int v2; // ebx@1
  int v3; // eax@14
  int v6; // esi@45
  ODMFace *v7; // ecx@45
  signed int v33; // eax@143
  int v34; // esi@143
  int v35; // esi@147
  int v36; // eax@155
  signed int v37; // esi@159
  signed int v38; // eax@159
  signed int i; // esi@159
  int v40; // esi@162
  bool v42; // eax@180
  signed int v43; // ecx@184
  signed int v44; // edx@184
  int v45; // ecx@200
  BSPModel *pModel; // eax@203
  ODMFace *pODMFace; // esi@203
  int v48; // eax@203
  char v49; // zf@203
  char v50; // sf@203
  unsigned __int8 v51; // of@203
  int v52; // eax@203
  BSPModel *v54; // eax@215
  int v55; // eax@217
  signed int v56; // ebx@228
  int v57; // eax@228
  BSPModel *v58; // eax@228
  unsigned __int64 v59; // qax@228
  BSPModel *v60; // eax@228
  unsigned __int64 v61; // qax@228
  int v62; // eax@241
  unsigned int v65; // ebx@263
  unsigned int v66; // esi@263
  int v67; // eax@263
  signed int v68; // ecx@263
  int v69; // eax@263
  int v70; // ebx@271
  int v71; // esi@271
  int v72; // edi@271
  int v73; // eax@271
  unsigned int v76; // edi@293
  bool v77; // edx@297
  bool v78; // ecx@303
  int v79; // ecx@314
  __int16 v80; // dx@317
  int v81; // ebx@318
  int v82; // ecx@318
  int pTerrainHeight; // eax@321
  int v86; // [sp-20h] [bp-B4h]@246
  int v87; // [sp-20h] [bp-B4h]@248
  signed int v88; // [sp-1Ch] [bp-B0h]@246
  unsigned int v89; // [sp-18h] [bp-ACh]@246
  signed int v90; // [sp-14h] [bp-A8h]@246
  signed int v91; // [sp-10h] [bp-A4h]@246
  int v92; // [sp-Ch] [bp-A0h]@246
  unsigned int v94; // [sp-8h] [bp-9Ch]@246
  int v96; // [sp-4h] [bp-98h]@246
  int v97; // [sp+Ch] [bp-88h]@180
  Vec3_int_ v98;
  bool v101; // [sp+1Ch] [bp-78h]@33
  int v102; // [sp+20h] [bp-74h]@1
  int v103; // [sp+24h] [bp-70h]@1
  int bFeatherFall; // [sp+28h] [bp-6Ch]@4
  int v105; // [sp+2Ch] [bp-68h]@24
  int bWaterWalk; // [sp+30h] [bp-64h]@1
  int v109; // [sp+3Ch] [bp-58h]@28
  int v110; // [sp+40h] [bp-54h]@180
  int v111; // [sp+44h] [bp-50h]@14
  int bJumping; // [sp+48h] [bp-4Ch]@1
  int v113; // [sp+4Ch] [bp-48h]@1
  unsigned int v114; // [sp+50h] [bp-44h]@1
  int _walk_speed; // [sp+54h] [bp-40h]@48
  int pX; // [sp+58h] [bp-3Ch]@1
  int pY; // [sp+5Ch] [bp-38h]@1
  int v118; // [sp+60h] [bp-34h]@1
  int _angle_x; // [sp+68h] [bp-2Ch]@48
  unsigned int v122; // [sp+70h] [bp-24h]@180
  int pZ; // [sp+74h] [bp-20h]@1
  int v124; // [sp+78h] [bp-1Ch]@1
  int _angle_y; // [sp+7Ch] [bp-18h]@48
  int v126; // [sp+80h] [bp-14h]@48
  int v128; // [sp+88h] [bp-Ch]@1
  int v129; // [sp+8Ch] [bp-8h]@92

  pZ = pParty->vPosition.z;
  v1 = 0;
  v103 = 0;
  v2 = 0;
  //*(float *)&v128 = 0.0;
  v128 = 0;
  v129 = 0;
  pX = pParty->vPosition.x;
  pY = pParty->vPosition.y;
  v113 = pParty->field_6F0;
  bJumping = 0;
  auto partyAtHighSlope = IsTerrainSlopeTooHigh(pParty->vPosition.x, pParty->vPosition.y);
  v114 = 0;
  v124 = 0;
  v102 = 0;
  pModel = 0;
  bWaterWalk = 0;
  if (!pParty->FeatherFallActive())
  {
    bFeatherFall = 0;
    for (int i = 0; i < 4; ++i)
      if (pParty->pPlayers[i].WearsItemAnyWhere(ITEM_ARTIFACT_LADYS_ESCORT))  // seems like flying boots
      {
        bFeatherFall = 1;
        break;
      }
  }
  else
  {
	bFeatherFall = 1;
  }
//LABEL_9:
  pParty->uFlags &= ~PARTY_FLAGS_1_STANDING_ON_WATER;
  if (pParty->WaterWalkActive())
  {
    //LOBYTE(pParty->uFlags) &= 0x7Fu;
    bWaterWalk = 1;
    *(short *)&stru_5E4C90_MapPersistVars._decor_events[20 * pParty->pPartyBuffs[PARTY_BUFF_WATER_WALK].uOverlayID + 119] |= 1u;
    if (!(pParty->pPartyBuffs[PARTY_BUFF_WATER_WALK].uFlags & 1) &&
		pParty->pPlayers[pParty->pPartyBuffs[PARTY_BUFF_WATER_WALK].uCaster - 1].sMana <= 0 )
      bWaterWalk = 0;
  }
  int bmodel_standing_on_pid;
  int is_on_water = false;
  v3 = ODM_GetFloorLevel(pX, pY, pZ, pParty->uPartyHeight, &is_on_water, &bmodel_standing_on_pid, bWaterWalk);
  int is_not_on_bmodel = bmodel_standing_on_pid == 0;

  v111 = v3;
  if ( bFeatherFall )
    pParty->uFallStartY = v3;
  else
    v3 = pParty->uFallStartY;
  if ( v3 - pZ > 512 && !bFeatherFall && pZ <= v111 + 1 )
  {
    if (pParty->uFlags & PARTY_FLAGS_1_LANDING)
    {
      pParty->uFlags &= ~PARTY_FLAGS_1_LANDING;
    }
    else for (int _i = 0; _i < 4; ++_i)     // receive falling damage
    {
      auto player = &pParty->pPlayers[_i];

      if ( !player->HasEnchantedItemEquipped(72) && !player->WearsItem(ITEM_ARTIFACT_HERMES_SANDALS, EQUIP_BOOTS) )
      {
        player->ReceiveDamage(
            (signed int)((pParty->uFallStartY - pZ) * (unsigned __int64)(player->GetMaxHealth() / 10)) / 256,
            DMGT_PHISYCAL);
        v105 = 20 - player->GetParameterBonus(player->GetActualEndurance());
        player->SetRecoveryTime((signed __int64)((double)v105 * flt_6BE3A4_debug_recmod1 * 2.133333333333333));
      }
      //}
      //while ( (signed int)v4 <= (signed int)&pPlayers[4] );
    }
  }
  v109 = -1;
  if ( pParty->bFlying )
    v109 = sub_46D8E3(pX, pY, pZ + pParty->uPartyHeight, (int)&v102);
  //v107 = bmodel_standing_on_pid == 0;
  v105 = v111 + 1;
  if ( pZ <= v111 + 1 )
  {
    v109 = -1;
    pParty->bFlying = false;
  }
  else
  {
    bJumping = 1;
  }
  v101 = pZ - v111 <= 32;
  if ( bWalkSound && pParty->walk_sound_timer)
  {
    if (pParty->walk_sound_timer >= pEventTimer->uTimeElapsed)
      pParty->walk_sound_timer -= pEventTimer->uTimeElapsed;
    else pParty->walk_sound_timer = 0;
  }
  if (!bUnderwater && pParty->pPartyBuffs[PARTY_BUFF_FLY].uExpireTime <= 0)
    pParty->bFlying = false;
  if (!bJumping)
  {
    if ( pParty->floor_face_pid != PID(OBJECT_BModel, bmodel_standing_on_pid) )
    {
      if (bmodel_standing_on_pid)
      {
        if ( (bmodel_standing_on_pid >> 6) < pOutdoor->uNumBModels )
        {
          v7 = pOutdoor->pBModels[bmodel_standing_on_pid >> 6].pFaces;
          v6 = bmodel_standing_on_pid & 0x3F;
          /*if ( *(char *)(v7->pFacePlane.vNormal.x + 308 * v6 + 31) & 4 )
          {
            pParty->field_6F4_packedid = PID(OBJECT_BModel,v108);
            v103 = *(short *)(v7->pFacePlane.vNormal.x + 308 * v6 + 292);
          }*/
		  if ( BYTE3(v7[v6].uAttributes) & 4 )
          {
            pParty->floor_face_pid = PID(OBJECT_BModel, bmodel_standing_on_pid);
            v103 = v7[v6].sCogTriggeredID;
          }
        }
      }
    }
    pParty->floor_face_pid = PID(OBJECT_BModel, bmodel_standing_on_pid);
  }
  _walk_speed = pParty->uWalkSpeed;
  _angle_y = pParty->sRotationY;
  _angle_x = pParty->sRotationX;
  v126 = pEventTimer->dt_in_some_format;
  /*v119 = (Player **)((unsigned __int64)(pEventTimer->dt_in_some_format
                                      * (signed __int64)((signed int)(pParty->field_20_prolly_turn_speed
                                                                    * stru_5C6E00->uIntegerPi)
                                                       / 180)) >> 16);*/
  __int64 dturn = (unsigned __int64)(pEventTimer->dt_in_some_format * (signed __int64)((signed int)(pParty->y_rotation_speed * stru_5C6E00->uIntegerPi) / 180)) >> 16;
  while (pPartyActionQueue->uNumActions)
  {
    switch (pPartyActionQueue->Next())
    {
      case PARTY_FlyUp:
      {
        if (!pParty->FlyActive() && !bUnderwater)
          break;

        pParty->bFlying = false;
        if (bUnderwater ||
            pParty->pPartyBuffs[PARTY_BUFF_FLY].uFlags & 1 ||
            pParty->pPlayers[pParty->pPartyBuffs[PARTY_BUFF_FLY].uCaster - 1].sMana > 0 )
          {
            if ( pParty->vPosition.z < 4000 || bJumping )
            {
              pZ += 30;
              v113 += 30;
              pParty->bFlying = 1;
              if ( pZ > 4000 )
              {
                pZ = 4000;
                v113 = 4000;
              }
              v1 = 0;
              v2 = 0;
              pParty->uFallSpeed = 0;
              *(float *)&v128 = 0.0;
              if ( v102 && pZ < v109 && (signed int)(pParty->uPartyHeight + pZ) >= v109 )
              {
                pParty->field_6E0 = 0;
                pParty->field_6E4 = 0;
                pPartyActionQueue->uNumActions = 0;
                pParty->uFlags |= PARTY_FLAGS_1_LANDING;
                pParty->vPosition.z = v109 - pParty->uPartyHeight - 31;
                pParty->field_6F0 = pZ;
                pParty->bFlying = 0;
                pZ = v109 - pParty->uPartyHeight - 31;
                v113 = pParty->field_6F0;
              }
              pParty->uFallSpeed = 0;
              pModel = (BSPModel *)1;
            }
          }
      }
      break;

      case PARTY_FlyDown:
        if (pParty->FlyActive() || bUnderwater)
        {
          pParty->bFlying = 0;
          if ( bUnderwater
            || pParty->pPartyBuffs[PARTY_BUFF_FLY].uFlags & 1
            || pParty->pPlayers[pParty->pPartyBuffs[PARTY_BUFF_FLY].uCaster - 1].sMana > 0 )//*(int *)&pParty->pArtifactsFound[6972 * pParty->pPartyBuffs[PARTY_BUFF_FLY].uCaster + 10] > 0 )
          {
            pZ -= 30;
            v113 -= 30;
            pParty->uFallSpeed = 0;
            //pParty->uFallSpeed = 0;
            pParty->bFlying = 1;
            pModel = (BSPModel *)1;
            if ( pZ <= v111 )
            {
              pParty->bFlying = 0;
              pPartyActionQueue->uNumActions = 0;
            }
          }
        }
        break;

      case PARTY_TurnLeft:
        if (uTurnSpeed)
          _angle_y += uTurnSpeed;   //descrete turn
        else
          _angle_y += dturn * fTurnSpeedMultiplier;  // time-based smooth turn

        _angle_y &= stru_5C6E00->uDoublePiMask;
      break;

      case PARTY_TurnRight:
        if (uTurnSpeed)
          _angle_y -= uTurnSpeed;
        else
          _angle_y -= dturn * fTurnSpeedMultiplier;

        _angle_y &= stru_5C6E00->uDoublePiMask;
      break;

      case PARTY_FastTurnLeft:
        if (uTurnSpeed)
          _angle_y += uTurnSpeed;
        else
          _angle_y += 2.0f * fTurnSpeedMultiplier * (double)dturn;

        _angle_y &= stru_5C6E00->uDoublePiMask;
      break;

      case PARTY_FastTurnRight:
        if (!uTurnSpeed)
          _angle_y -= 2.0f * fTurnSpeedMultiplier * (double)dturn;
        else
          _angle_y -= uTurnSpeed;

        _angle_y &= stru_5C6E00->uDoublePiMask;
      break;

      case PARTY_StrafeLeft:
      {
        *(float *)&v128 = pParty->uWalkSpeed;

        float sin_y = sinf(2 * 3.141592653589 * _angle_y / 2048.0);
        int dx = sin_y * pParty->uWalkSpeed * fWalkSpeedMultiplier;
        v2 -= 3 * dx / 4;
        
        float cos_y = cosf(2 * 3.141592653589 * _angle_y / 2048.0);
        int dy = cos_y * pParty->uWalkSpeed * fWalkSpeedMultiplier;
        v1 += 3 * dy / 4;

        v128 = v1;
        v124 = 1;
      }
      break;

      case PARTY_StrafeRight:
      {
        *(float *)&v128 = pParty->uWalkSpeed;

        float sin_y = sinf(2 * 3.141592653589 * _angle_y / 2048.0);
        int dx = sin_y * pParty->uWalkSpeed * fWalkSpeedMultiplier;
        v2 += 3 * dx / 4;
        
        float cos_y = cosf(2 * 3.141592653589 * _angle_y / 2048.0);
        int dy = cos_y * pParty->uWalkSpeed * fWalkSpeedMultiplier;
        v1 -= 3 * dy / 4;

        v128 = v1;
        v124 = 1;
      }
      break;

      case PARTY_WalkForward:
      {
        *(float *)&v128 = _walk_speed;

        float sin_y = sinf(2 * 3.141592653589 * _angle_y / 2048.0),
              cos_y = cosf(2 * 3.141592653589 * _angle_y / 2048.0);

        int dx = cos_y * pParty->uWalkSpeed * fWalkSpeedMultiplier;
        v2 += dx * 12;
        
        int dy = sin_y * pParty->uWalkSpeed * fWalkSpeedMultiplier;
        v1 += dy * 12;

        v128 = v1;
        v124 = 1;
      }
      break;

      case PARTY_RunForward:
      {
        *(float *)&v128 = _walk_speed;

        float sin_y = sinf(2 * 3.141592653589 * _angle_y / 2048.0),
              cos_y = cosf(2 * 3.141592653589 * _angle_y / 2048.0);

        int dx = cos_y * pParty->uWalkSpeed * fWalkSpeedMultiplier;
        int dy = sin_y * pParty->uWalkSpeed * fWalkSpeedMultiplier;

        if (pParty->bFlying)
        {
          v2 += 4 * dx;
          v1 += 4 * dy;

          v128 = v1;
        }
        else if (partyAtHighSlope && !bmodel_standing_on_pid)
        {
          v2 += dx;
          v1 += dy;

          v128 = v1;
          v124 = 1;
        }
        else
        {
          v2 += 2 * dx;
          v1 += 2 * dy;
          
          v128 = v1;
          v114 = 1;
        }
      }
      break;


      case PARTY_WalkBackward:
      {
        *(float *)&v128 = _walk_speed;

        float sin_y = sinf(2 * 3.141592653589 * _angle_y / 2048.0),
              cos_y = cosf(2 * 3.141592653589 * _angle_y / 2048.0);

        int dx = cos_y * pParty->uWalkSpeed * fBackwardWalkSpeedMultiplier;
        v2 -= dx;
        
        int dy = sin_y * pParty->uWalkSpeed * fBackwardWalkSpeedMultiplier;
        v1 -= dy;

        v128 = v1;
        v124 = 1;
      }
      break;


      case PARTY_RunBackward:
      {
        float sin_y = sinf(2 * 3.141592653589 * _angle_y / 2048.0),
              cos_y = cosf(2 * 3.141592653589 * _angle_y / 2048.0);

        int dx = cos_y * pParty->uWalkSpeed * fBackwardWalkSpeedMultiplier;        
        int dy = sin_y * pParty->uWalkSpeed * fBackwardWalkSpeedMultiplier;

        if (pParty->bFlying)
        {
          v2 -= 4 * dx;
          v1 -= 4 * dy;
          v128 = v1;
        }
        else
        {
          v2 -= dx;
          v1 -= dy;

          v128 = v1;
          v124 = 1;
        }
      }
      break;

      case PARTY_CenterView:
        _angle_x = 0;
      break;

      case PARTY_LookUp:
        _angle_x += (signed __int64)(flt_6BE150_look_up_down_dangle * 25.0);
        if ( _angle_x > 128 )
          _angle_x = 128;
        if (uActiveCharacter)
          pPlayers[uActiveCharacter]->PlaySound(SPEECH_63, 0);
      break;

      case PARTY_LookDown:
        _angle_x += (signed __int64)(flt_6BE150_look_up_down_dangle * -25.0);
        if ( _angle_x < -128 )
          _angle_x = -128;
        if (uActiveCharacter)
          pPlayers[uActiveCharacter]->PlaySound(SPEECH_64, 0);
      break;

      case PARTY_Jump:
        if ( (!partyAtHighSlope || bmodel_standing_on_pid) && !bJumping && pParty->field_24 && !(pParty->uFlags & 4) && !(BYTE1(pParty->uFlags) & 2) )
        {
          v126 = pParty->field_24 << 6;
          bJumping = 1;
          pParty->uFallSpeed = (signed __int64)((double)(pParty->field_24 << 6) * 1.5 + (double)pParty->uFallSpeed);
        }
      break;

      default:
        assert(false);

      case PARTY_Land:
        if (pParty->bFlying)
        {
          pParty->uFlags |= PARTY_FLAGS_1_LANDING;
          pParty->uFallSpeed = 0;
        }
        pParty->bFlying = false;
        pPartyActionQueue->uNumActions = 0;
      break;
    }
  }

//LABEL_123:
  pParty->sRotationY = _angle_y;
  pParty->sRotationX = _angle_x;
  if ( pZ < v111 || pParty->bFlying)
  {
    if ( pParty->bFlying )
    {
      v126 = stru_5C6E00->Cos(GetTickCount());
      v129 = (unsigned __int64)(4i64 * v126) >> 16;
      pZ = v113 + v129;
      if ( pModel )
        pZ = v113;
      if (pParty->FlyActive())
        stru_5E4C90_MapPersistVars._decor_events[20 * pParty->pPartyBuffs[PARTY_BUFF_FLY].uOverlayID + 119] &= 0xFEu;
      pParty->uFallStartY = pZ;
    }
    else
    {
      if ( is_on_water && pParty->uFallSpeed )
        sub_42F960_create_object(pX, pY, v111);
      pParty->uFallSpeed = 0;
      pZ = v111;
      pParty->uFallStartY = v111;
      v113 = pZ;
      if (pParty->FlyActive())
        stru_5E4C90_MapPersistVars._decor_events[20 * pParty->pPartyBuffs[PARTY_BUFF_FLY].uOverlayID + 119] |= 1u;
    }
  }
  else
  {
	  v113 = pZ;
	  if (pParty->FlyActive())
		stru_5E4C90_MapPersistVars._decor_events[20 * pParty->pPartyBuffs[PARTY_BUFF_FLY].uOverlayID + 119] |= 1u;
  }
//LABEL_141:
  if (bJumping && !pParty->bFlying)
  {
    v33 = -(pEventTimer->uTimeElapsed * GetGravityStrength());
    v34 = pParty->uFallSpeed + 2 * v33;
    pParty->uFallSpeed += 2 * v33;
  }
  else if (!partyAtHighSlope)
  {
    v34 = pParty->uFallSpeed;
  }
  else if (!bJumping)
  {
    if ( !bmodel_standing_on_pid )
	{
		// rolling down the hill
		// how it's done: you get a little bit pushed in the air along terrain normal, getting in the air
		// and falling to the gravity, gradually sliding downwards. nice trick
		pZ = v111;
		ODM_GetTerrainNormalAt(pX, pY, &v98);
		v35 = pParty->uFallSpeed + -8 * pEventTimer->uTimeElapsed * GetGravityStrength();
		v129 = abs((signed __int64)v2 * v98.x + (signed __int64)v1 * v98.y + (signed __int64)v35 * v98.z) >> 16;
		v2 += (unsigned __int64)(v129 * (signed __int64)v98.x) >> 16;
		v1 += (unsigned __int64)(v129 * (signed __int64)v98.y) >> 16;
		v34 = v35 + ((unsigned __int64)(v129 * (signed __int64)v98.z) >> 16);
		v128 = v1;
		pParty->uFallSpeed = v34;
	}
  }
  else
	  v34 = pParty->uFallSpeed;

//LABEL_164:
  if(bJumping)
  {
	  if ( !bUnderwater && v34 <= 0)
	  {
		if ( v34 < -500
		  && !pParty->bFlying
		  && pParty->vPosition.z - v111 > 1000
		  && !pParty->FeatherFallActive())
		{ // falling scream
		  for (int i = 0; i < 4; ++i)
		  {
			auto player = &pParty->pPlayers[i];
			if (!player->HasEnchantedItemEquipped(72) && !player->WearsItem(ITEM_ARTIFACT_HERMES_SANDALS, EQUIP_BOOTS) && player->CanAct())
			  player->PlaySound(SPEECH_66, 0);
		  }
		}
	  }
  }
  else
  {
//LABEL_150:
  pParty->uFallStartY = pZ;
  }

  if ( v2 * v2 + v1 * v1 < 400 && !partyAtHighSlope )
  {
    *(float *)&v128 = 0.0;
    v2 = 0;
  }
  stru_721530.field_84 = -1;
  stru_721530.field_70 = 0;
  stru_721530.prolly_normal_d = pParty->field_14_radius;
  stru_721530.field_8_radius = pParty->field_14_radius >> 1;
  v126 = 0;
  stru_721530.field_0 = 1;
  stru_721530.height = pParty->uPartyHeight - 32;
  do
  {
    stru_721530.position.x = pX;
    stru_721530.normal.x = pX;
    stru_721530.velocity.x = v2;
    stru_721530.position.y = pY;
    stru_721530.normal.y = pY;
    stru_721530.normal.z = stru_721530.prolly_normal_d + pZ + 1;
    stru_721530.position.z = stru_721530.height + pZ + 1;
    stru_721530.velocity.y = v128;
    stru_721530.velocity.z = pParty->uFallSpeed;
    v36 = 0;
    stru_721530.uSectorID = 0;
    if ( pParty->bTurnBasedModeOn == 1 && pTurnEngine->turn_stage == 3 )
      v36 = 13312;
    if ( stru_721530._47050A(v36) )
      break;
    _46E889_collide_against_bmodels(1u);
    v37 = WorldPosToGridCellZ(pParty->vPosition.y);
    v38 = WorldPosToGridCellX(pParty->vPosition.x);
    _46E26D_collide_against_sprites(v38, v37);
    _46ED8A_collide_against_sprite_objects(4u);
    for ( i = 0; i < (signed int)uNumActors; ++i )
      _46DF1A_collide_against_actor(i, 0);
    if ( stru_721530.field_7C >= stru_721530.field_6C )
    {
      _angle_x = stru_721530.normal2.x;
      _angle_y = stru_721530.normal2.y;
      v40 = stru_721530.normal2.z - stru_721530.prolly_normal_d - 1;
    }
    else
    {
      _angle_x = pX + fixpoint_mul(stru_721530.field_7C, stru_721530.direction.x);
      _angle_y = pY + fixpoint_mul(stru_721530.field_7C, stru_721530.direction.y);
      pModel = (BSPModel *)fixpoint_mul(stru_721530.field_7C, stru_721530.direction.z);
      v40 = fixpoint_mul(stru_721530.field_7C, stru_721530.direction.z) + pZ;
    }
    v122 = v40;
    ODM_GetFloorLevel(_angle_x, _angle_y, v40, pParty->uPartyHeight, &is_on_water, &bmodel_standing_on_pid, 0);
    v129 = ODM_GetFloorLevel(_angle_x, pY, v40, pParty->uPartyHeight, &is_on_water, &v97, 0);
    auto v119 = ODM_GetFloorLevel(pX, _angle_y, v40, pParty->uPartyHeight, &is_on_water, &v110, 0);
    pModel = (BSPModel *)IsTerrainSlopeTooHigh(_angle_x, pY);
    v42 = IsTerrainSlopeTooHigh(pX, _angle_y);
    is_not_on_bmodel = false;
    v118 = v42;
    if ( !v97 && !v110 && !bmodel_standing_on_pid )
      is_not_on_bmodel = true;
    v43 = 1;
    v44 = 1;
    if ( bUnderwater || !is_not_on_bmodel )
	{
		  pX = _angle_x;
		  if ( v43 )
			  pY = _angle_y;
	}
	else
	{
		if ( pModel && v129 > pZ )
		  v44 = 0;
		if ( v118 && v119 > pZ )
		  v43 = 0;
		if ( v44 )
		{
			  pX = _angle_x;
			  if ( v43 )
				  pY = _angle_y;
		}
		else if ( v43 )
		  pY = _angle_y;
		else
		{
			pModel = (BSPModel *)ODM_GetFloorLevel(_angle_x, _angle_y, v40, pParty->uPartyHeight, &is_on_water, &bmodel_standing_on_pid, 0);
			if ( IsTerrainSlopeTooHigh(_angle_x, _angle_y) && (signed int)pModel <= pZ )
			{
			  v43 = 1;
			  pX = _angle_x;
			  if ( v43 )
				  pY = _angle_y;
			}
		}
	}
    if ( stru_721530.field_7C >= stru_721530.field_6C )
    {
      if ( !is_not_on_bmodel )
      {
        pX = stru_721530.normal2.x;
        pY = stru_721530.normal2.y;
      }
      pZ = stru_721530.normal2.z - stru_721530.prolly_normal_d - 1;
      break;
    }
    stru_721530.field_70 += stru_721530.field_7C;
    pX = _angle_x;
    pY = _angle_y;
    v45 = stru_721530.uFaceID;
    pZ = v40;
    if ( PID_TYPE(stru_721530.uFaceID) == OBJECT_Actor)
    {
      if (pParty->Invisible())
        pParty->pPartyBuffs[PARTY_BUFF_INVISIBILITY].Reset();

      viewparams->bRedrawGameUI = 1;
		v2 = (unsigned __int64)(58500i64 * v2) >> 16;
		pModel = (BSPModel *)58500;
		v128 = (unsigned __int64)(58500i64 * v128) >> 16;
		v122 = (unsigned __int64)(58500i64 * pParty->uFallSpeed) >> 16;
		++v126;
		pParty->uFallSpeed = (unsigned __int64)(58500i64 * pParty->uFallSpeed) >> 16;
		continue;
	}
    if ( PID_TYPE(stru_721530.uFaceID) == OBJECT_Decoration)
    {
      v56 = integer_sqrt(v2 * v2 + v128 * v128);
      v118 = v56;
      v57 = stru_5C6E00->Atan2(_angle_x - pLevelDecorations[(signed int)stru_721530.uFaceID >> 3].vPosition.x,
                               _angle_y - pLevelDecorations[(signed int)stru_721530.uFaceID >> 3].vPosition.y);
      v129 = v57;
      v58 = (BSPModel *)stru_5C6E00->Cos(v57);
      pModel = v58;
      v59 = (signed int)v58 * (signed __int64)v56;
      v122 = v59 >> 16;
      v2 = v59 >> 16;
      v60 = (BSPModel *)stru_5C6E00->Sin(v129);
      pModel = v60;
      v61 = (signed int)v60 * (signed __int64)v118;
      v122 = v61 >> 16;
      v128 = v61 >> 16;
    }
    else
    {
      if ( PID_TYPE(stru_721530.uFaceID) == OBJECT_BModel)
      {
        pParty->bFlying = 0;
        pModel = &pOutdoor->pBModels[(signed int)stru_721530.uFaceID >> 9];
        //v127 = v46;
        pODMFace = &pModel->pFaces[((signed int)stru_721530.uFaceID >> 3) & 0x3F];
        v48 = pODMFace->pBoundingBox.z2 - pODMFace->pBoundingBox.z1;
        v51 = __OFSUB__(v48, 32);
        v49 = v48 == 32;
        v50 = v48 - 32 < 0;
        v52 = pODMFace->pFacePlane.vNormal.z;
        v129 = (unsigned __int8)(v50 ^ v51 | v49);
        v119 = v52 < 46378;
        if ( bUnderwater == 1 )
          v119 = 0;
        if ( pODMFace->uPolygonType == POLYGON_Floor )
        {
          if ( pParty->uFallSpeed < 0 )
            pParty->uFallSpeed = 0;
          pZ = pModel->pVertices.pVertices[pODMFace->pVertexIDs[0]].z + 1;
          if ( v2 * v2 + v128 * v128 < 400 )
          {
            v2 = 0;
            *(float *)&v128 = 0.0;
          }
          if ( pParty->floor_face_pid != v45 && (pODMFace->uAttributes & FACE_PRESSURE_PLATE))
          {
            pParty->floor_face_pid = v45;
            v103 = pODMFace->sCogTriggeredID;
          }
          v2 = (unsigned __int64)(58500i64 * v2) >> 16;
          pModel = (BSPModel *)58500;
          v128 = (unsigned __int64)(58500i64 * v128) >> 16;
          v122 = (unsigned __int64)(58500i64 * pParty->uFallSpeed) >> 16;
          ++v126;
          pParty->uFallSpeed = (unsigned __int64)(58500i64 * pParty->uFallSpeed) >> 16;
          continue;
        }
        if ( !v129 && (pODMFace->uPolygonType != POLYGON_InBetweenFloorAndWall || v119) )// упЄрс€ в столб
        {
          v118 = abs(v128 * pODMFace->pFacePlane.vNormal.y + pParty->uFallSpeed * v52 + v2 * pODMFace->pFacePlane.vNormal.x) >> 16;
          if ((stru_721530.speed >> 3) > v118 )
            v118 = stru_721530.speed >> 3;
          v129 = (unsigned __int64)(v118 * (signed __int64)pODMFace->pFacePlane.vNormal.x) >> 16;
          _walk_speed = (unsigned __int64)(v118 * (signed __int64)pODMFace->pFacePlane.vNormal.y) >> 16;
          v54 = 0;
          if ( !v119 )
          {
            pModel = (BSPModel *)pODMFace->pFacePlane.vNormal.z;
            pModel = (BSPModel *)((unsigned __int64)(v118 * (signed __int64)(signed int)pModel) >> 16);
            v54 = pModel;
          }
          pParty->uFallSpeed += (int)v54;
          v128 += _walk_speed;
          v2 += v129;
          v55 = stru_721530.prolly_normal_d
              - ((signed int)(pODMFace->pFacePlane.dist
                            + v122 * pODMFace->pFacePlane.vNormal.z
                            + _angle_y * pODMFace->pFacePlane.vNormal.y
                            + _angle_x * pODMFace->pFacePlane.vNormal.x) >> 16);
          if ( v55 > 0 )
          {
            pX = _angle_x + (pODMFace->pFacePlane.vNormal.x * v55 >> 16);
            pY = _angle_y + (pODMFace->pFacePlane.vNormal.y * v55 >> 16);
            if ( !v119 )
              pZ = v122 + (pODMFace->pFacePlane.vNormal.z * v55 >> 16);
          }
//LABEL_220:
			v45 = stru_721530.uFaceID;
			if ( pParty->floor_face_pid != v45 && BYTE3(pODMFace->uAttributes) & 4 )
			{
				pParty->floor_face_pid = v45;
				v103 = pODMFace->sCogTriggeredID;
			}
			v2 = (unsigned __int64)(58500i64 * v2) >> 16;
			pModel = (BSPModel *)58500;
			v128 = (unsigned __int64)(58500i64 * v128) >> 16;
			v122 = (unsigned __int64)(58500i64 * pParty->uFallSpeed) >> 16;
			++v126;
			pParty->uFallSpeed = (unsigned __int64)(58500i64 * pParty->uFallSpeed) >> 16;
			continue;
        }
        v118 = abs(v128 * pODMFace->pFacePlane.vNormal.y + pParty->uFallSpeed * v52 + v2 * pODMFace->pFacePlane.vNormal.x) >> 16;
        if ((stru_721530.speed >> 3) > v118 )
          v118 = stru_721530.speed >> 3;
        v122 = pODMFace->pFacePlane.vNormal.x;
        v122 = (unsigned __int64)(v118 * (signed __int64)(signed int)v122) >> 16;
        pModel = (BSPModel *)pODMFace->pFacePlane.vNormal.y;
        pModel = (BSPModel *)((unsigned __int64)(v118 * (signed __int64)(signed int)pModel) >> 16);
        v129 = pODMFace->pFacePlane.vNormal.z;
        v129 = (unsigned __int64)(v118 * (signed __int64)v129) >> 16;
        pParty->uFallSpeed += v129;
        v2 += v122;
        v128 += (int)pModel;
        if ( v2 * v2 + v128 * v128 >= 400 )
		{
			v45 = stru_721530.uFaceID;
			if ( pParty->floor_face_pid != v45 && BYTE3(pODMFace->uAttributes) & 4 )
			{
				pParty->floor_face_pid = v45;
				v103 = pODMFace->sCogTriggeredID;
			}
			v2 = (unsigned __int64)(58500i64 * v2) >> 16;
			pModel = (BSPModel *)58500;
			v128 = (unsigned __int64)(58500i64 * v128) >> 16;
			v122 = (unsigned __int64)(58500i64 * pParty->uFallSpeed) >> 16;
			++v126;
			pParty->uFallSpeed = (unsigned __int64)(58500i64 * pParty->uFallSpeed) >> 16;
			continue;
		}
        v2 = 0;
        pParty->uFallSpeed = 0;
        *(float *)&v128 = 0.0;
      }
    }
//LABEL_234:
    v2 = (unsigned __int64)(58500i64 * v2) >> 16;
    pModel = (BSPModel *)58500;
    v128 = (unsigned __int64)(58500i64 * v128) >> 16;
    v122 = (unsigned __int64)(58500i64 * pParty->uFallSpeed) >> 16;
    ++v126;
    pParty->uFallSpeed = (unsigned __int64)(58500i64 * pParty->uFallSpeed) >> 16;
  }
  while ( v126 < 100 );
  if ( bWalkSound && pParty->walk_sound_timer <= 0 )
  {
    v122 = abs(pParty->vPosition.x - pX);
    v126 = abs(pParty->vPosition.y - pY);
    v62 = abs(pParty->vPosition.z - pZ);
    if ( integer_sqrt(v122 * v122 + v126 * v126 + v62 * v62) < 8 )
	{
		  pAudioPlayer->_4AA258(804);
		  pParty->walk_sound_timer = 64;
	}
	else
	{
		if ( v114 && (!bJumping || v101) )
		{
		  if ( !is_not_on_bmodel
			&& !(BYTE1(pOutdoor->pBModels[pParty->floor_face_pid >> 9].pFaces[(pParty->floor_face_pid >> 3) & 0x3F].uAttributes) & 0x20) )
		  {
            pAudioPlayer->PlaySound(SOUND_64, 804, 1, -1, 0, 0, 0, 0);
		  }
		  else
		  {
			//v63 = WorldPosToGridCellZ(pParty->vPosition.y) - 1;
			//v64 = WorldPosToGridCellX(pParty->vPosition.x);
			v87 = pOutdoor->GetSoundIdByPosition(WorldPosToGridCellX(pParty->vPosition.x), WorldPosToGridCellZ(pParty->vPosition.y) - 1, 1);
			pAudioPlayer->PlaySound((SoundID)v87, 804, 1, -1, 0, 0, 0, 0);
		  }
		}
		else
		{
			if ( v124 && (!bJumping || v101) )
			{
			  if ( is_not_on_bmodel
				|| BYTE1(pOutdoor->pBModels[pParty->floor_face_pid >> 9].pFaces[(pParty->floor_face_pid >> 3) & 0x3F].uAttributes) & 0x20 )
			  {
				//v63 = WorldPosToGridCellZ(pParty->vPosition.y) - 1;
				//v64 = WorldPosToGridCellX(pParty->vPosition.x);
				v87 = pOutdoor->GetSoundIdByPosition(WorldPosToGridCellX(pParty->vPosition.x), WorldPosToGridCellZ(pParty->vPosition.y) - 1, 0);
				pAudioPlayer->PlaySound((SoundID)v87, 804, 1, -1, 0, 0, 0, 0);
			  }
			  else
                pAudioPlayer->PlaySound(SOUND_103, 804, 1, -1, 0, 0, 0, 0);
			}
			else
			{
			  pAudioPlayer->_4AA258(804);
			  pParty->walk_sound_timer = 64;
			}
		}
    }
  }
  if ( !bJumping || v101 )
    pParty->uFlags &= ~PARTY_FLAGS_1_FALLING;
  else
    pParty->uFlags |= PARTY_FLAGS_1_FALLING;
  v126 = WorldPosToGridCellX(pParty->vPosition.x);
  v65 = WorldPosToGridCellZ(pParty->vPosition.y) - 1;
  v114 = WorldPosToGridCellX(pX);
  v66 = WorldPosToGridCellZ(pY) - 1;
  pModel = (BSPModel *)((~(unsigned int)pOutdoor->ActuallyGetSomeOtherTileInfo(v126, v65) >> 1) & 1);
  v122 = (~(unsigned int)pOutdoor->ActuallyGetSomeOtherTileInfo(v114, v65) >> 1) & 1;
  v67 = pOutdoor->ActuallyGetSomeOtherTileInfo(v126, v66);
  v68 = 0;
  v69 = (~(unsigned int)v67 >> 1) & 1;
  if ( v114 == v126 && v66 == v65 && v122 && v69 )
    v68 = 1;
  if ( !is_not_on_bmodel )
    v68 = 1;
  if ( v68 )
  {
    v70 = pZ;
    v71 = pX;
    v72 = pY;
    //pParty->uFallSpeed = v121;
    v73 = pZ;
    pParty->vPosition.x = pX;
    pParty->vPosition.y = pY;
    pParty->vPosition.z = pZ;
    pParty->field_6F0 = v113;
    if ( pZ > 8160 )
    {
      v73 = 8160;
      pParty->uFallStartY = 8160;
      pParty->vPosition.z = 8160;
    }
    if ( !v103
      || (EventProcessor(v103, 0, 1), pParty->vPosition.x == v71)
      && pParty->vPosition.y == v72
      && (v73 = pParty->vPosition.z, pParty->vPosition.z == v70) )
    {
      if ( v73 < v111 )
      {
        pParty->uFallSpeed = 0;
        v73 = v105;
        pParty->vPosition.z = v105;
        if ( pParty->uFallStartY - v70 > 512 && !bFeatherFall && v70 <= v105 && !bUnderwater )//Fall to the ground(падение на землю с высоты)
        {
          if ( pParty->uFlags & PARTY_FLAGS_1_LANDING )
          {
            pParty->uFlags &= ~PARTY_FLAGS_1_LANDING;
          }
          else
          {
            for ( uint i = 1; i <= 4; ++i )
            {
              v110 = pPlayers[i]->GetMaxHealth();
              pPlayers[i]->ReceiveDamage((signed int)((pParty->uFallStartY - v70) * (unsigned __int64)(signed __int64)((double)v110 * 0.1)) / 256,
                DMGT_PHISYCAL);
              v110 = 20 - pPlayers[i]->GetParameterBonus(pPlayers[i]->GetActualEndurance());
              pPlayers[i]->SetRecoveryTime((signed __int64)((double)v110 * flt_6BE3A4_debug_recmod1 * 2.133333333333333));
            }
            v73 = pParty->vPosition.z;
          }
        }
        pParty->uFallStartY = v70;
      }
      if ( v102 && v73 < v109 )
      {
        if ( (signed int)(pParty->uPartyHeight + v73) >= v109 )
        {
          pParty->vPosition.z = v109 - pParty->uPartyHeight - 1;
          pParty->field_6F0 = v109 - pParty->uPartyHeight - 1;
        }
      }
      pParty->uFlags &= ~0x204;
    }
    return;
  }
  v76 = pParty->bFlying;
  if ( pParty->bFlying || v101 == 0 || bWaterWalk || !pModel )
    v77 = 1;
  else
    v77 = v122 != 0;
  v114 = 0;
  if ( !pParty->bFlying && v101 != 0 && !bWaterWalk )
  {
    if ( pModel )
    {
      v78 = v69 != 0;
    }
	else
	{
		v114 = 1;
		v78 = 1;
	}
  }
  else
	v78 = 1;
//LABEL_306:
  if ( v77 )
  {
    pParty->vPosition.x = pX;
  }
  if ( v78 )
  {
    pParty->vPosition.y = pY;
  }
  if ( v78 || v77)
  {
    if ( bWaterWalk )
    {
      pParty->uFlags &= ~PARTY_FLAGS_1_STANDING_ON_WATER;
      //v79 = 20 * pParty->pPartyBuffs[PARTY_BUFF_WATER_WALK].uOverlayID + 6180178;
      //*(short *)&stru_5E4C90._decor_events[20 * pParty->pPartyBuffs[PARTY_BUFF_WATER_WALK].uOverlayID + 119] |= 1u;
      v79 = (int)&stru_5E4C90_MapPersistVars._decor_events[20 * pParty->pPartyBuffs[PARTY_BUFF_WATER_WALK].uOverlayID + 119];
      *(short *)v79 |= 1u;
      if ( !v122 || !v69 )
      {
        if ( !v76 )
        {
          v80 = *(short *)v79;
          pParty->uFlags |= PARTY_FLAGS_1_STANDING_ON_WATER;
          *(short *)v79 = v80 & 0xFFFE;
        }
      }
    }
  }
  else if ( bWalkSound && pParty->walk_sound_timer <= 0 )
  {
    pAudioPlayer->_4AA258(804);
    pParty->walk_sound_timer = 64;
  }
//LABEL_318:
  v81 = pZ;
  v82 = pZ;
  pParty->vPosition.z = pZ;
  if ( pZ > 8160 )
  {
    v82 = 8160;
    pParty->uFallStartY = 8160;
    pParty->vPosition.z = 8160;
  }
  LOWORD(pParty->uFlags) &= 0xFDFBu;
  //pParty->uFallSpeed = v121;
  pParty->field_6F0 = v113;
  if ( v114 )//party stand to the water(группа стоит на воде)
  {
    pTerrainHeight = GetTerrainHeightsAroundParty2(pParty->vPosition.x, pParty->vPosition.y, &v110, 1);
    if ( pParty->vPosition.z <= pTerrainHeight + 1 )//положение группы всегда +1
      pParty->uFlags |= PARTY_FLAGS_1_WATER_DAMAGE;
  }
  if ( !v103
    || (EventProcessor(v103, 0, 1), pParty->vPosition.x == pX)
    && pParty->vPosition.y == pY
    && (v82 = pParty->vPosition.z, pParty->vPosition.z == v81) )
  {
    if ( v82 < v111 )
    {
      v82 = v105;
      pParty->uFallSpeed = 0;
      pParty->vPosition.z = v105;
      if ( pParty->uFallStartY - v81 > 512 && !bFeatherFall && v81 <= v105 && !bUnderwater )//Fall to the water(падение на воду с высоты)
      {
        if ( pParty->uFlags & PARTY_FLAGS_1_LANDING )
        {
          pParty->uFlags &= ~PARTY_FLAGS_1_LANDING;
        }
        else
        {
          for ( uint i = 1; i <= 4; ++i )
          {
            v110 = pPlayers[i]->GetMaxHealth();
            pPlayers[i]->ReceiveDamage((signed int)((pParty->uFallStartY - v81) * (unsigned __int64)(signed __int64)((double)v110 * 0.1)) / 256,
              DMGT_PHISYCAL);
            v110 = 20 - pPlayers[i]->GetParameterBonus(pPlayers[i]->GetActualEndurance());
            pPlayers[i]->SetRecoveryTime((signed __int64)((double)v110 * flt_6BE3A4_debug_recmod1 * 2.133333333333333));
          }
          v82 = pParty->vPosition.z;
        }
      }
      pParty->uFallStartY = v81;
    }
    if ( v102 && v82 < v109 && (signed int)(pParty->uPartyHeight + v82) >= v109 )
    {
      pParty->vPosition.z = v82 + pParty->uPartyHeight - v109 + 1;
      pParty->field_6F0 = v82 + pParty->uPartyHeight - v109 + 1;
    }
  }
}

//----- (0047531C) --------------------------------------------------------
bool __fastcall sub_47531C(int a1, int *a2, int a3, int a4, int a5, int a6, int a7, int a8, BLVFace *a9, int a10)
{
  BLVFace *v10; // ebx@1
  int v11; // ST1C_4@3
  int v12; // edi@3
  int v13; // esi@3
  int v14; // edi@4
  signed __int64 v15; // qtt@6
  __int16 v16; // si@7
  int *v18; // [sp+Ch] [bp-Ch]@1
  int v19; // [sp+10h] [bp-8h]@1
  int a7a; // [sp+30h] [bp+18h]@7
  int a9b; // [sp+38h] [bp+20h]@3
  int a9a; // [sp+38h] [bp+20h]@3
  int a10b; // [sp+3Ch] [bp+24h]@3
  signed int a10a; // [sp+3Ch] [bp+24h]@4
  int a10c; // [sp+3Ch] [bp+24h]@5

  v10 = a9;
  v18 = a2;
  v19 = a1;
  if ( a10 && BYTE3(a9->uAttributes) & 0x20 )
    return 0;
  v11 = (unsigned __int64)(a6 * (signed __int64)a9->pFacePlane_old.vNormal.x) >> 16;
  a10b = (unsigned __int64)(a7 * (signed __int64)a9->pFacePlane_old.vNormal.y) >> 16;
  a9b = (unsigned __int64)(a8 * (signed __int64)a9->pFacePlane_old.vNormal.z) >> 16;
  v12 = v11 + a9b + a10b;
  a9a = v11 + a9b + a10b;
  v13 = (a1 << 16)
      - a3 * v10->pFacePlane_old.vNormal.x
      - a4 * v10->pFacePlane_old.vNormal.y
      - a5 * v10->pFacePlane_old.vNormal.z
      - v10->pFacePlane_old.dist;
  if ( abs((a1 << 16)
         - a3 * v10->pFacePlane_old.vNormal.x
         - a4 * v10->pFacePlane_old.vNormal.y
         - a5 * v10->pFacePlane_old.vNormal.z - v10->pFacePlane_old.dist) >= a1 << 16 )
  {
    a10c = abs(v13) >> 14;
    if ( a10c > abs(v12) )
      return 0;
    LODWORD(v15) = v13 << 16;
    HIDWORD(v15) = v13 >> 16;
    v14 = v19;
    a10a = v15 / a9a;
  }
  else
  {
    a10a = 0;
    v14 = abs(v13) >> 16;
  }
  v16 = a4 + ((unsigned int)((unsigned __int64)(a10a * (signed __int64)a7) >> 16) >> 16);
  LOWORD(a7a) = (short)a3
              + ((unsigned int)((unsigned __int64)(a10a * (signed __int64)a6) >> 16) >> 16)
              - ((unsigned int)(v14 * v10->pFacePlane_old.vNormal.x) >> 16);
  HIWORD(a7a) = v16 - ((unsigned int)(v14 * v10->pFacePlane_old.vNormal.y) >> 16);
  if ( !sub_475665(
          v10,
          a7a,
          (short)a5
        + ((unsigned int)((unsigned __int64)(a10a * (signed __int64)a8) >> 16) >> 16)
        - ((unsigned int)(v14 * v10->pFacePlane_old.vNormal.z) >> 16)) )
    return 0;
  *v18 = a10a >> 16;
  if ( a10a >> 16 < 0 )
    *v18 = 0;
  return 1;
}

//----- (004754BF) --------------------------------------------------------
bool __fastcall sub_4754BF(int a1, int *a2, int a3, int a4, int a5, int a6, int a7, int a8, BLVFace *a9, int a10, int a11)
{
  BLVFace *v11; // ebx@1
  int v12; // ST1C_4@3
  int v13; // edi@3
  int v14; // esi@3
  int v15; // edi@4
  signed __int64 v16; // qtt@6
  __int16 v17; // si@7
  int *v19; // [sp+Ch] [bp-Ch]@1
  int v20; // [sp+10h] [bp-8h]@1
  int a7a; // [sp+30h] [bp+18h]@7
  int a1b; // [sp+38h] [bp+20h]@3
  int a1a; // [sp+38h] [bp+20h]@3
  int a11b; // [sp+40h] [bp+28h]@3
  signed int a11a; // [sp+40h] [bp+28h]@4
  int a11c; // [sp+40h] [bp+28h]@5

  v11 = a9;
  v19 = a2;
  v20 = a1;
  if ( a11 && BYTE3(a9->uAttributes) & 0x20 )
    return 0;
  v12 = (unsigned __int64)(a6 * (signed __int64)a9->pFacePlane_old.vNormal.x) >> 16;
  a11b = (unsigned __int64)(a7 * (signed __int64)a9->pFacePlane_old.vNormal.y) >> 16;
  a1b = (unsigned __int64)(a8 * (signed __int64)a9->pFacePlane_old.vNormal.z) >> 16;
  v13 = v12 + a1b + a11b;
  a1a = v12 + a1b + a11b;
  v14 = (a1 << 16)
      - a3 * v11->pFacePlane_old.vNormal.x
      - a4 * v11->pFacePlane_old.vNormal.y
      - a5 * v11->pFacePlane_old.vNormal.z
      - v11->pFacePlane_old.dist;
  if ( abs((a1 << 16)
         - a3 * v11->pFacePlane_old.vNormal.x
         - a4 * v11->pFacePlane_old.vNormal.y
         - a5 * v11->pFacePlane_old.vNormal.z - v11->pFacePlane_old.dist) >= a1 << 16 )
  {
    a11c = abs(v14) >> 14;
    if ( a11c > abs(v13) )
      return 0;
    LODWORD(v16) = v14 << 16;
    HIDWORD(v16) = v14 >> 16;
    v15 = v20;
    a11a = v16 / a1a;
  }
  else
  {
    a11a = 0;
    v15 = abs(v14) >> 16;
  }
  v17 = a4 + ((unsigned int)((unsigned __int64)(a11a * (signed __int64)a7) >> 16) >> 16);
  LOWORD(a7a) = (short)a3
              + ((unsigned int)((unsigned __int64)(a11a * (signed __int64)a6) >> 16) >> 16)
              - ((unsigned int)(v15 * v11->pFacePlane_old.vNormal.x) >> 16);
  HIWORD(a7a) = v17 - ((unsigned int)(v15 * v11->pFacePlane_old.vNormal.y) >> 16);
  if ( !sub_4759C9(
          v11,
          a10,
          a7a,
          (short)a5
        + ((unsigned int)((unsigned __int64)(a11a * (signed __int64)a8) >> 16) >> 16)
        - ((unsigned int)(v15 * v11->pFacePlane_old.vNormal.z) >> 16)) )
    return 0;
  *v19 = a11a >> 16;
  if ( a11a >> 16 < 0 )
    *v19 = 0;
  return 1;
}

//----- (00475665) --------------------------------------------------------
int sub_475665(BLVFace *_this, int a2, __int16 a3)
{
  unsigned int v3; // eax@1
  Vec3_short_ *v4; // edx@3
  signed int v5; // eax@4
  int v6; // esi@4
  signed int v7; // eax@7
  Vec3_short_ *v8; // edx@8
  signed int v9; // eax@9
  int v10; // esi@9
  signed int v11; // eax@11
  Vec3_short_ *v12; // edx@12
  signed int v13; // eax@13
  int v14; // esi@13
  int v15; // esi@14
  bool v16; // edi@14
  int v17; // ecx@16
  signed int v18; // edx@16
  int v19; // eax@17
  signed int v20; // ebx@18
  int v21; // edi@20
  signed int v22; // ST14_4@22
  signed __int64 v23; // qtt@22
  signed int result; // eax@25
  int v25; // [sp+14h] [bp-10h]@14
  int v26; // [sp+1Ch] [bp-8h]@2
  signed int v27; // [sp+20h] [bp-4h]@2
  signed int v28; // [sp+30h] [bp+Ch]@2
  signed int v29; // [sp+30h] [bp+Ch]@7
  signed int v30; // [sp+30h] [bp+Ch]@11
  signed int v31; // [sp+30h] [bp+Ch]@14

  v3 = _this->uAttributes;
  if ( BYTE1(v3) & 1 )
  {
    v28 = 0;
    v26 = (signed __int16)a2;
    v27 = SHIWORD(a2);
    if ( _this->uNumVertices )
    {
      v4 = pIndoor->pVertices;
      do
      {
        v5 = v28;
        v6 = 2 * v28;
        word_720C10_intercepts_xs[2 * v28] = _this->pXInterceptDisplacements[v28] + v4[_this->pVertexIDs[v28]].x;
        word_720B40_intercepts_zs[2 * v28] = _this->pYInterceptDisplacements[v5] + v4[_this->pVertexIDs[v5]].y;
        word_720C10_intercepts_xs[2 * v28++ + 1] = _this->pXInterceptDisplacements[v5 + 1]
                                                 + v4[_this->pVertexIDs[v5 + 1]].x;
        word_720B40_intercepts_zs[v6 + 1] = _this->pYInterceptDisplacements[v5 + 1] + v4[_this->pVertexIDs[v5 + 1]].y;
      }
      while ( v28 < _this->uNumVertices );
    }
  }
  else
  {
    if ( BYTE1(v3) & 2 )
    {
      v26 = (signed __int16)a2;
      v7 = a3;
      v29 = 0;
      v27 = v7;
      if ( _this->uNumVertices )
      {
        v8 = pIndoor->pVertices;
        do
        {
          v9 = v29;
          v10 = 2 * v29;
          word_720C10_intercepts_xs[2 * v29] = _this->pXInterceptDisplacements[v29] + v8[_this->pVertexIDs[v29]].x;
          word_720B40_intercepts_zs[2 * v29] = _this->pZInterceptDisplacements[v9] + v8[_this->pVertexIDs[v9]].z;
          word_720C10_intercepts_xs[2 * v29++ + 1] = _this->pXInterceptDisplacements[v9 + 1] + v8[_this->pVertexIDs[v9 + 1]].x;
          word_720B40_intercepts_zs[v10 + 1] = _this->pZInterceptDisplacements[v9 + 1] + v8[_this->pVertexIDs[v9 + 1]].z;
        }
        while ( v29 < _this->uNumVertices );
      }
    }
    else
    {
      v26 = SHIWORD(a2);
      v11 = a3;
      v30 = 0;
      v27 = v11;
      if ( _this->uNumVertices )
      {
        v12 = pIndoor->pVertices;
        do
        {
          v13 = v30;
          v14 = 2 * v30;
          word_720C10_intercepts_xs[2 * v30] = _this->pYInterceptDisplacements[v30] + v12[_this->pVertexIDs[v30]].y;
          word_720B40_intercepts_zs[2 * v30] = _this->pZInterceptDisplacements[v13] + v12[_this->pVertexIDs[v13]].z;
          word_720C10_intercepts_xs[2 * v30++ + 1] = _this->pYInterceptDisplacements[v13 + 1] + v12[_this->pVertexIDs[v13 + 1]].y;
          word_720B40_intercepts_zs[v14 + 1] = _this->pZInterceptDisplacements[v13 + 1] + v12[_this->pVertexIDs[v13 + 1]].z;
        }
        while ( v30 < _this->uNumVertices );
      }
    }
  }
  v15 = 2 * _this->uNumVertices;
  v31 = 0;
  word_720C10_intercepts_xs[2 * _this->uNumVertices] = word_720C10_intercepts_xs[0];
  word_720B40_intercepts_zs[v15] = word_720B40_intercepts_zs[0];
  v25 = 0;
  v16 = word_720B40_intercepts_zs[0] >= v27;
  if ( v15 <= 0 )
    return 0;
  do
  {
    if ( v31 >= 2 )
      break;
    v17 = v25;
    v18 = word_720B40_intercepts_zs[v25 + 1];
    if ( v16 ^ (v18 >= v27) )
    {
      v19 = word_720C10_intercepts_xs[v17 + 1];
      if ( v19 >= v26 )
        v20 = 0;
      else
        v20 = 2;
      v21 = v20 | (word_720C10_intercepts_xs[v17] < v26);
      if ( v21 != 3 )
      {
        if ( !v21
          || (v22 = v19 - word_720C10_intercepts_xs[v17],
              LODWORD(v23) = v22 << 16,
              HIDWORD(v23) = v22 >> 16,
              word_720C10_intercepts_xs[v17]
            + ((signed int)(((unsigned __int64)(v23
                                              / (v18 - word_720B40_intercepts_zs[v17])
                                              * ((v27 - (signed int)word_720B40_intercepts_zs[v17]) << 16)) >> 16)
                          + 32768) >> 16) >= v26) )
          ++v31;
      }
    }
    ++v25;
    v16 = v18 >= v27;
  }
  while ( v25 < v15 );
  result = 1;
  if ( v31 != 1 )
    result = 0;
  return result;
}

//----- (004759C9) --------------------------------------------------------
bool __fastcall sub_4759C9(BLVFace *a1, int a2, int a3, __int16 a4)
{
  unsigned int v4; // eax@1
  int v5; // edx@4
  __int16 v6; // si@4
  int v7; // edx@9
  __int16 v8; // si@9
  int v9; // edx@13
  __int16 v10; // si@13
  int v11; // esi@14
  bool v12; // edi@14
  int v13; // ecx@16
  signed int v14; // edx@16
  int v15; // eax@17
  signed int v16; // ebx@18
  int v17; // edi@20
  signed int v18; // ST14_4@22
  signed __int64 v19; // qtt@22
  bool result; // eax@25
  int v21; // [sp+14h] [bp-10h]@14
  signed int v22; // [sp+18h] [bp-Ch]@1
  int v23; // [sp+1Ch] [bp-8h]@2
  signed int v24; // [sp+20h] [bp-4h]@2
  Vec3_int_ **a4a; // [sp+30h] [bp+Ch]@3
  Vec3_int_ **a4b; // [sp+30h] [bp+Ch]@8
  Vec3_int_ **a4c; // [sp+30h] [bp+Ch]@12
  signed int a4d; // [sp+30h] [bp+Ch]@14

  v4 = a1->uAttributes;
  v22 = 0;
  if ( BYTE1(v4) & 1 )
  {
    v23 = (signed __int16)a3;
    v24 = SHIWORD(a3);
    if ( a1->uNumVertices )
    {
      a4a = &pOutdoor->pBModels[a2].pVertices.pVertices;
      do
      {
        v5 = 2 * v22;
        word_720A70_intercepts_xs_plus_xs[2 * v22] = a1->pXInterceptDisplacements[v22] + LOWORD((*a4a)[a1->pVertexIDs[v22]].x);
        word_7209A0_intercepts_ys_plus_ys[2 * v22] = a1->pYInterceptDisplacements[v22] + LOWORD((*a4a)[a1->pVertexIDs[v22]].y);
        word_720A70_intercepts_xs_plus_xs[2 * v22 + 1] = a1->pXInterceptDisplacements[v22 + 1] + LOWORD((*a4a)[a1->pVertexIDs[v22 + 1]].x);
        word_7209A0_intercepts_ys_plus_ys[v5 + 1] = v6 = a1->pYInterceptDisplacements[v22 + 1] + LOWORD((*a4a)[a1->pVertexIDs[v22 + 1]].y);
        ++v22;
      }
      while ( v22 < a1->uNumVertices );
    }
  }
  else
  {
    if ( BYTE1(v4) & 2 )
    {
      v23 = (signed __int16)a3;
      v24 = a4;
      if ( a1->uNumVertices )
      {
        a4b = &pOutdoor->pBModels[a2].pVertices.pVertices;
        do
        {
          v7 = 2 * v22;
          word_720A70_intercepts_xs_plus_xs[2 * v22] = a1->pXInterceptDisplacements[v22] + LOWORD((*a4b)[a1->pVertexIDs[v22]].x);
          word_7209A0_intercepts_ys_plus_ys[2 * v22] = a1->pZInterceptDisplacements[v22] + LOWORD((*a4b)[a1->pVertexIDs[v22]].z);
          word_720A70_intercepts_xs_plus_xs[2 * v22 + 1] = a1->pXInterceptDisplacements[v22 + 1] + LOWORD((*a4b)[a1->pVertexIDs[v22 + 1]].x);
          word_7209A0_intercepts_ys_plus_ys[v7 + 1] = v8 = a1->pZInterceptDisplacements[v22 + 1] + LOWORD((*a4b)[a1->pVertexIDs[v22 + 1]].z);
          ++v22;
        }
        while ( v22 < a1->uNumVertices );
      }
    }
    else
    {
      v23 = SHIWORD(a3);
      v24 = a4;
      if ( a1->uNumVertices )
      {
        a4c = &pOutdoor->pBModels[a2].pVertices.pVertices;
        do
        {
          v9 = 2 * v22;
          word_720A70_intercepts_xs_plus_xs[2 * v22] = a1->pYInterceptDisplacements[v22] + LOWORD((*a4c)[a1->pVertexIDs[v22]].y);
          word_7209A0_intercepts_ys_plus_ys[2 * v22] = a1->pZInterceptDisplacements[v22] + LOWORD((*a4c)[a1->pVertexIDs[v22]].z);
          word_720A70_intercepts_xs_plus_xs[2 * v22 + 1] = a1->pYInterceptDisplacements[v22 + 1] + LOWORD((*a4c)[a1->pVertexIDs[v22 + 1]].y);
          word_7209A0_intercepts_ys_plus_ys[v9 + 1] = v10 = a1->pZInterceptDisplacements[v22 + 1] + LOWORD((*a4c)[a1->pVertexIDs[v22 + 1]].z);
          ++v22;
        }
        while ( v22 < a1->uNumVertices );
      }
    }
  }
  v11 = 2 * a1->uNumVertices;
  a4d = 0;
  word_720A70_intercepts_xs_plus_xs[2 * a1->uNumVertices] = word_720A70_intercepts_xs_plus_xs[0];
  word_7209A0_intercepts_ys_plus_ys[v11] = word_7209A0_intercepts_ys_plus_ys[0];
  v21 = 0;
  v12 = word_7209A0_intercepts_ys_plus_ys[0] >= v24;
  if ( v11 <= 0 )
    return 0;
  do
  {
    if ( a4d >= 2 )
      break;
    v13 = v21;
    v14 = word_7209A0_intercepts_ys_plus_ys[v21 + 1];
    if ( v12 ^ (v14 >= v24) )
    {
      v15 = word_720A70_intercepts_xs_plus_xs[v13 + 1];
      if ( v15 >= v23 )
        v16 = 0;
      else
        v16 = 2;
      v17 = v16 | (word_720A70_intercepts_xs_plus_xs[v13] < v23);
      if ( v17 != 3 )
      {
        if ( !v17
          || (v18 = v15 - word_720A70_intercepts_xs_plus_xs[v13],
              LODWORD(v19) = v18 << 16,
              HIDWORD(v19) = v18 >> 16,
              word_720A70_intercepts_xs_plus_xs[v13]
            + ((signed int)(((unsigned __int64)(v19
                                              / (v14 - word_7209A0_intercepts_ys_plus_ys[v13])
                                              * ((v24 - (signed int)word_7209A0_intercepts_ys_plus_ys[v13]) << 16)) >> 16)
                          + 32768) >> 16) >= v23) )
          ++a4d;
      }
    }
    ++v21;
    v12 = v14 >= v24;
  }
  while ( v21 < v11 );
  result = 1;
  if ( a4d != 1 )
    result = 0;
  return result;
}

//----- (00475D85) --------------------------------------------------------
bool __fastcall sub_475D85(Vec3_int_ *a1, Vec3_int_ *a2, int *a3, BLVFace *a4)
{
  BLVFace *v4; // ebx@1
  int v5; // ST24_4@2
  int v6; // ST28_4@2
  int v7; // edi@2
  int v8; // eax@5
  signed int v9; // esi@5
  signed __int64 v10; // qtt@10
  Vec3_int_ *v11; // esi@11
  int v12; // ST14_4@11
  Vec3_int_ *v14; // [sp+Ch] [bp-18h]@1
  Vec3_int_ *v15; // [sp+14h] [bp-10h]@1
  int v16; // [sp+18h] [bp-Ch]@2
  int v17; // [sp+20h] [bp-4h]@10
  int a4b; // [sp+30h] [bp+Ch]@2
  int a4c; // [sp+30h] [bp+Ch]@9
  signed int a4a; // [sp+30h] [bp+Ch]@10

  v4 = a4;
  v15 = a2;
  v14 = a1;
  if ( BYTE3(a4->uAttributes) & 0x20
    || (v5 = (unsigned __int64)(a2->x * (signed __int64)a4->pFacePlane_old.vNormal.x) >> 16,
        a4b = (unsigned __int64)(a2->y * (signed __int64)a4->pFacePlane_old.vNormal.y) >> 16,
        v6 = (unsigned __int64)(a2->z * (signed __int64)v4->pFacePlane_old.vNormal.z) >> 16,
        v7 = v5 + v6 + a4b,
        (v16 = v5 + v6 + a4b) == 0)
    || v7 > 0 && !v4->Portal())
    return 0;
  v8 = v4->pFacePlane_old.vNormal.z * a1->z;
  v9 = -(v4->pFacePlane_old.dist + v8 + a1->y * v4->pFacePlane_old.vNormal.y + a1->x * v4->pFacePlane_old.vNormal.x);
  if ( v7 <= 0 )
  {
    if ( v4->pFacePlane_old.dist + v8 + a1->y * v4->pFacePlane_old.vNormal.y + a1->x * v4->pFacePlane_old.vNormal.x < 0 )
      return 0;
  }
  else
  {
    if ( v9 < 0 )
      return 0;
  }
  a4c = abs(-(v4->pFacePlane_old.dist + v8 + a1->y * v4->pFacePlane_old.vNormal.y + a1->x * v4->pFacePlane_old.vNormal.x)) >> 14;
  if ( a4c > abs(v7)
    || (LODWORD(v10) = v9 << 16, HIDWORD(v10) = v9 >> 16, a4a = v10 / v16, v17 = v10 / v16, v17 > *a3 << 16)
    || (v11 = v14,
        LOWORD(v12) = LOWORD(v14->x)
                    + (((unsigned int)((unsigned __int64)(v17 * (signed __int64)v15->x) >> 16) + 32768) >> 16),
        HIWORD(v12) = LOWORD(v11->y)
                    + (((unsigned int)((unsigned __int64)(v17 * (signed __int64)v15->y) >> 16) + 32768) >> 16),
        !sub_475665(
           v4,
           v12,
           LOWORD(v11->z) + (((unsigned int)((unsigned __int64)(v17 * (signed __int64)v15->z) >> 16) + 32768) >> 16))) )
    return 0;
  *a3 = a4a >> 16;
  return 1;
}

//----- (00475F30) --------------------------------------------------------
bool __fastcall sub_475F30(int *a1, BLVFace *a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9)
{
  BLVFace *v9; // ebx@1
  int v10; // ST20_4@2
  int v11; // ST28_4@2
  int v12; // ST24_4@2
  char v13; // zf@2
  int v14; // edi@2
  int v15; // eax@5
  signed int v16; // esi@5
  int v17; // ST20_4@9
  signed __int64 v18; // qtt@10
  int v19; // ST14_4@11
  int *v21; // [sp+14h] [bp-10h]@1
  int v22; // [sp+1Ch] [bp-8h]@2
  int v23; // [sp+1Ch] [bp-8h]@10
  signed int v24; // [sp+20h] [bp-4h]@10

  v9 = a2;
  v21 = a1;
  if ( BYTE3(a2->uAttributes) & 0x20
    || (v10 = (unsigned __int64)(a6 * (signed __int64)a2->pFacePlane_old.vNormal.x) >> 16,
        v11 = (unsigned __int64)(a7 * (signed __int64)a2->pFacePlane_old.vNormal.y) >> 16,
        v12 = (unsigned __int64)(a8 * (signed __int64)a2->pFacePlane_old.vNormal.z) >> 16,
        v13 = v10 + v12 + v11 == 0,
        v14 = v10 + v12 + v11,
        v22 = v10 + v12 + v11,
        v13)
    || v14 > 0 && !a2->Portal())
    return 0;
  v15 = a4 * a2->pFacePlane_old.vNormal.y;
  v16 = -(a2->pFacePlane_old.dist + v15 + a3 * a2->pFacePlane_old.vNormal.x + a5 * a2->pFacePlane_old.vNormal.z);
  if ( v14 <= 0 )
  {
    if ( a2->pFacePlane_old.dist + v15 + a3 * a2->pFacePlane_old.vNormal.x + a5 * a2->pFacePlane_old.vNormal.z < 0 )
      return 0;
  }
  else
  {
    if ( v16 < 0 )
      return 0;
  }
  v17 = abs(-(a2->pFacePlane_old.dist + v15 + a3 * a2->pFacePlane_old.vNormal.x + a5 * a2->pFacePlane_old.vNormal.z)) >> 14;
  if ( v17 > abs(v14)
    || (LODWORD(v18) = v16 << 16, HIDWORD(v18) = v16 >> 16, v24 = v18 / v22, v23 = v18 / v22, v23 > *v21 << 16)
    || (LOWORD(v19) = a3 + (((unsigned int)((unsigned __int64)(v23 * (signed __int64)a6) >> 16) + 32768) >> 16),
        HIWORD(v19) = a4 + (((unsigned int)((unsigned __int64)(v23 * (signed __int64)a7) >> 16) + 32768) >> 16),
        !sub_4759C9(
           v9,
           a9,
           v19,
           a5 + (((unsigned int)((unsigned __int64)(v23 * (signed __int64)a8) >> 16) + 32768) >> 16))) )
    return 0;
  *v21 = v24 >> 16;
  return 1;
}

//----- (004760D5) --------------------------------------------------------
PartyAction ActionQueue::Next()
{
  if (!uNumActions)
    return PARTY_INVALID;

  auto result = pActions[0];
  for (unsigned int i = 0; i < uNumActions - 1; ++i)
    pActions[i] = pActions[i + 1];
  --uNumActions;

  return result;
}

//----- (004775ED) --------------------------------------------------------
int stru6_stru1_indoor_sw_billboard::_4775ED(float a2)
{
  char *v2; // edi@1
  int v3; // eax@1
  char *v4; // edx@2
  char *v5; // esi@3
  double v6; // st7@6
  signed __int64 v7; // ST84_8@6
  double v8; // ST0C_8@6
  int v9; // esi@6
  double v10; // ST44_8@6
  int v11; // ecx@6
  double v12; // ST34_8@6
  int v13; // ecx@6
  double v14; // ST14_8@6
  double v15; // st7@8
  unsigned int v16; // ecx@8
  signed __int64 v17; // ST64_8@8
  double v18; // ST24_8@8
  int v19; // edi@8
  double v20; // ST3C_8@8
  int v21; // ecx@8
  double v22; // ST2C_8@8
  int v23; // ST9C_4@8
  double v24; // ST1C_8@8
  int *v25; // edi@8
  int v26; // esi@8
  int *v27; // edi@10
  int v28; // esi@10
  int result; // eax@12
  __int64 v30; // [sp+A8h] [bp-30h]@8
  float v31; // [sp+B0h] [bp-28h]@6
  float v32; // [sp+B4h] [bp-24h]@6
  int v33; // [sp+B8h] [bp-20h]@6
  int v34; // [sp+BCh] [bp-1Ch]@2
  stru6_stru1_indoor_sw_billboard *v35; // [sp+C0h] [bp-18h]@1
  float v36; // [sp+C4h] [bp-14h]@6
  int v37; // [sp+C8h] [bp-10h]@6
  int v38; // [sp+CCh] [bp-Ch]@1
  float v39; // [sp+D0h] [bp-8h]@6
  int *v40; // [sp+D4h] [bp-4h]@2

  v2 = (char *)&this->field_64[4 * this->uNumVertices];
  v38 = 0;
  *(int *)v2 = this->field_64[0];
  v2 += 4;
  *(int *)v2 = this->field_64[1];
  v2 += 4;
  *(int *)v2 = this->field_64[2];
  *((int *)v2 + 1) = this->field_64[3];
  v3 = this->uNumVertices;
  v35 = this;
  if ( v3 > 0 )
  {
    v40 = &this->field_64[20];
    v4 = (char *)&this->field_64[3] + 3;
    v34 = v3;
    while ( 1 )
    {
      v5 = v4 - 15;
      if ( *(float *)(v4 - 15) <= (double)a2 && *(float *)(v4 + 1) <= (double)a2 )
	  {
        v4 += 16;
		--v34;
		if ( !v34 )
		  return this->uNumVertices = v38;
		continue;
	  }
      if ( *(float *)v5 <= (double)a2 )
	  {
		v6 = (a2 - *(float *)v5) / (*(float *)(v4 + 1) - *(float *)v5);
		v7 = (unsigned __int8)v4[16] - (unsigned int)(unsigned __int8)*v4;
		v36 = v6;
		v31 = (*(float *)(v4 + 5) - *(float *)(v4 - 11)) * v6 + *(float *)(v4 - 11);
		v32 = (*(float *)(v4 + 9) - *(float *)(v4 - 7)) * v6 + *(float *)(v4 - 7);
		*(float *)&v37 = (double)v7 * v6;
		v8 = *(float *)&v37 + 6.7553994e15;
		v9 = (unsigned __int8)*v4;
		*(float *)&v37 = (double)((unsigned __int8)v4[15] - (unsigned int)(unsigned __int8)*(v4 - 1)) * v36;
		v10 = *(float *)&v37 + 6.7553994e15;
		v11 = (unsigned __int8)*(v4 - 2);
		v37 = LODWORD(v10) + (unsigned __int8)*(v4 - 1);
		v39 = (double)((unsigned int)(unsigned __int8)v4[14] - v11) * v36;
		v12 = v39 + 6.7553994e15;
		v13 = LODWORD(v12) + (unsigned __int8)*(v4 - 2);
		v39 = (double)((*(int *)(v4 + 13) & 0xFF) - (*(int *)(v4 - 3) & 0xFFu)) * v36;
		v14 = v39 + 6.7553994e15;
		v33 = (LODWORD(v14) + (*(int *)(v4 - 3) & 0xFF)) | ((v13 | ((v37 | ((LODWORD(v8) + v9) << 8)) << 8)) << 8);
		//this = v35;
		v5 = (char *)&v30 + 4;
	  }
      else if ( *(float *)(v4 + 1) <= (double)a2 )
      {
        v15 = (a2 - *(float *)v5) / (*(float *)(v4 + 1) - *(float *)v5);
        v16 = (unsigned __int8)*v4;
        HIDWORD(v30) = LODWORD(a2);
        v17 = (unsigned __int8)v4[16] - v16;
        v36 = v15;
        v31 = (*(float *)(v4 + 5) - *(float *)(v4 - 11)) * v15 + *(float *)(v4 - 11);
        v32 = (*(float *)(v4 + 9) - *(float *)(v4 - 7)) * v15 + *(float *)(v4 - 7);
        v39 = (double)v17 * v15;
        v18 = v39 + 6.7553994e15;
        v19 = (unsigned __int8)*v4;
        v39 = (double)((unsigned __int8)v4[15] - (unsigned int)(unsigned __int8)*(v4 - 1)) * v36;
        v20 = v39 + 6.7553994e15;
        v21 = (unsigned __int8)*(v4 - 2);
        v37 = LODWORD(v20) + (unsigned __int8)*(v4 - 1);
        v39 = (double)((unsigned int)(unsigned __int8)v4[14] - v21) * v36;
        v22 = v39 + 6.7553994e15;
        v23 = LODWORD(v22) + (unsigned __int8)*(v4 - 2);
        v39 = (double)((*(int *)(v4 + 13) & 0xFF) - (*(int *)(v4 - 3) & 0xFFu)) * v36;
        v24 = v39 + 6.7553994e15;
        v33 = (LODWORD(v24) + (*(int *)(v4 - 3) & 0xFF)) | ((v23 | ((v37 | ((LODWORD(v18) + v19) << 8)) << 8)) << 8);
        v25 = v40;
        *v40 = *(int *)v5;
        v26 = (int)(v5 + 4);
        ++v25;
        *v25 = *(int *)v26;
        v26 += 4;
        ++v25;
        ++v38;
        v40 += 4;
        *v25 = *(int *)v26;
        v25[1] = *(int *)(v26 + 4);
		v5 = (char *)&v30 + 4;
      }
      v27 = v40;
      ++v38;
      *v40 = *(int *)v5;
      v28 = (int)(v5 + 4);
      ++v27;
      *v27 = *(int *)v28;
      v28 += 4;
      ++v27;
      v40 += 4;
      *v27 = *(int *)v28;
      v27[1] = *(int *)(v28 + 4);
      v4 += 16;
      --v34;
      if ( !v34 )
          return this->uNumVertices = v38;
    }
  }
  return this->uNumVertices = v38;
}

//----- (00477927) --------------------------------------------------------
int stru6_stru1_indoor_sw_billboard::_477927(float a2)
{
  char *v2; // edi@1
  int v3; // eax@1
  char *v4; // edx@2
  char *v5; // esi@3
  double v6; // st7@6
  signed __int64 v7; // ST84_8@6
  double v8; // ST0C_8@6
  int v9; // esi@6
  double v10; // ST44_8@6
  int v11; // ecx@6
  double v12; // ST34_8@6
  int v13; // ecx@6
  double v14; // ST14_8@6
  double v15; // st7@8
  unsigned int v16; // ecx@8
  signed __int64 v17; // ST64_8@8
  double v18; // ST24_8@8
  int v19; // edi@8
  double v20; // ST3C_8@8
  int v21; // ecx@8
  double v22; // ST2C_8@8
  int v23; // ST9C_4@8
  double v24; // ST1C_8@8
  int *v25; // edi@8
  int v26; // esi@8
  int *v27; // edi@10
  int v28; // esi@10
  int result; // eax@12
  __int64 v30; // [sp+A8h] [bp-30h]@8
  float v31; // [sp+B0h] [bp-28h]@6
  float v32; // [sp+B4h] [bp-24h]@6
  int v33; // [sp+B8h] [bp-20h]@6
  int v34; // [sp+BCh] [bp-1Ch]@2
  stru6_stru1_indoor_sw_billboard *v35; // [sp+C0h] [bp-18h]@1
  float v36; // [sp+C4h] [bp-14h]@6
  int v37; // [sp+C8h] [bp-10h]@6
  int v38; // [sp+CCh] [bp-Ch]@1
  float v39; // [sp+D0h] [bp-8h]@6
  int *v40; // [sp+D4h] [bp-4h]@2

  v2 = (char *)&this->field_64[4 * this->uNumVertices];
  v38 = 0;
  *(int *)v2 = this->field_64[0];
  v2 += 4;
  *(int *)v2 = this->field_64[1];
  v2 += 4;
  *(int *)v2 = this->field_64[2];
  *((int *)v2 + 1) = this->field_64[3];
  v3 = this->uNumVertices;
  v35 = this;
  if ( v3 > 0 )
  {
    v40 = &this->field_64[20];
    v4 = (char *)&this->field_64[3] + 3;
    v34 = v3;
    while ( 1 )
    {
      v5 = v4 - 15;
      if ( *(float *)(v4 - 15) >= (double)a2 && *(float *)(v4 + 1) >= (double)a2 )
	  {
        v4 += 16;
		--v34;
		if ( !v34 )
          return this->uNumVertices = v38;
		continue;
	  }
      if ( *(float *)v5 >= (double)a2 )
	  {
		v6 = (a2 - *(float *)v5) / (*(float *)(v4 + 1) - *(float *)v5);
		v7 = (unsigned __int8)v4[16] - (unsigned int)(unsigned __int8)*v4;
		v36 = v6;
		v31 = (*(float *)(v4 + 5) - *(float *)(v4 - 11)) * v6 + *(float *)(v4 - 11);
		v32 = (*(float *)(v4 + 9) - *(float *)(v4 - 7)) * v6 + *(float *)(v4 - 7);
		*(float *)&v37 = (double)v7 * v6;
		v8 = *(float *)&v37 + 6.7553994e15;
		v9 = (unsigned __int8)*v4;
		*(float *)&v37 = (double)((unsigned __int8)v4[15] - (unsigned int)(unsigned __int8)*(v4 - 1)) * v36;
		v10 = *(float *)&v37 + 6.7553994e15;
		v11 = (unsigned __int8)*(v4 - 2);
		v37 = LODWORD(v10) + (unsigned __int8)*(v4 - 1);
		v39 = (double)((unsigned int)(unsigned __int8)v4[14] - v11) * v36;
		v12 = v39 + 6.7553994e15;
		v13 = LODWORD(v12) + (unsigned __int8)*(v4 - 2);
		v39 = (double)((*(int *)(v4 + 13) & 0xFF) - (*(int *)(v4 - 3) & 0xFFu)) * v36;
		v14 = v39 + 6.7553994e15;
		v33 = (LODWORD(v14) + (*(int *)(v4 - 3) & 0xFF)) | ((v13 | ((v37 | ((LODWORD(v8) + v9) << 8)) << 8)) << 8);
		//this = v35;
		v5 = (char *)&v30 + 4;
	  }
      else if ( *(float *)(v4 + 1) >= (double)a2 )
      {
        v15 = (a2 - *(float *)v5) / (*(float *)(v4 + 1) - *(float *)v5);
        v16 = (unsigned __int8)*v4;
        HIDWORD(v30) = LODWORD(a2);
        v17 = (unsigned __int8)v4[16] - v16;
        v36 = v15;
        v31 = (*(float *)(v4 + 5) - *(float *)(v4 - 11)) * v15 + *(float *)(v4 - 11);
        v32 = (*(float *)(v4 + 9) - *(float *)(v4 - 7)) * v15 + *(float *)(v4 - 7);
        v39 = (double)v17 * v15;
        v18 = v39 + 6.7553994e15;
        v19 = (unsigned __int8)*v4;
        v39 = (double)((unsigned __int8)v4[15] - (unsigned int)(unsigned __int8)*(v4 - 1)) * v36;
        v20 = v39 + 6.7553994e15;
        v21 = (unsigned __int8)*(v4 - 2);
        v37 = LODWORD(v20) + (unsigned __int8)*(v4 - 1);
        v39 = (double)((unsigned int)(unsigned __int8)v4[14] - v21) * v36;
        v22 = v39 + 6.7553994e15;
        v23 = LODWORD(v22) + (unsigned __int8)*(v4 - 2);
        v39 = (double)((*(int *)(v4 + 13) & 0xFF) - (*(int *)(v4 - 3) & 0xFFu)) * v36;
        v24 = v39 + 6.7553994e15;
        v33 = (LODWORD(v24) + (*(int *)(v4 - 3) & 0xFF)) | ((v23 | ((v37 | ((LODWORD(v18) + v19) << 8)) << 8)) << 8);
        v25 = v40;
        *v40 = *(int *)v5;
        v26 = (int)(v5 + 4);
        ++v25;
        *v25 = *(int *)v26;
        v26 += 4;
        ++v25;
        ++v38;
        v40 += 4;
        *v25 = *(int *)v26;
        v25[1] = *(int *)(v26 + 4);
	    v5 = (char *)&v30 + 4;
      }
      v27 = v40;
      ++v38;
      *v40 = *(int *)v5;
      v28 = (int)(v5 + 4);
      ++v27;
      *v27 = *(int *)v28;
      v28 += 4;
      ++v27;
      v40 += 4;
      *v27 = *(int *)v28;
      v27[1] = *(int *)(v28 + 4);
      v4 += 16;
      --v34;
      if ( !v34 )
          return this->uNumVertices = v38;
    }
  }
  return this->uNumVertices = v38;
}

//----- (00477C61) --------------------------------------------------------
int stru6_stru1_indoor_sw_billboard::sub_477C61()
{
  //stru6_stru1_indoor_sw_billboard *v1; // ebx@1
  int v2; // ecx@2
  int v3; // eax@3
  double v4; // st7@4
  double v5; // st7@5
  double v6; // st6@5
  double v7; // st5@6
  float v8; // ST30_4@8
  float v9; // ST24_4@8
  double v10; // st7@8
  double v11; // st6@8
  double v12; // st5@8
  float v13; // ST24_4@13
  int v14; // esi@13
  char *v15; // esi@15
  //signed int v16; // eax@16
  __int16 v17; // fps@16
  unsigned __int8 v18; // c2@16
  unsigned __int8 v19; // c3@16
  double v20; // st6@16
  float v21; // ST18_4@17
  float v22; // ST2C_4@17
  float v23; // ST34_4@17
  float v24; // ST24_4@17
  double v25; // st7@17
  double v26; // st6@17
  float v27; // ST34_4@18
  float v28; // ST30_4@18
  int v29; // eax@19
  signed int v31; // [sp+8h] [bp-28h]@15
  float v32; // [sp+Ch] [bp-24h]@16
  float v33; // [sp+14h] [bp-1Ch]@16
  float v34; // [sp+18h] [bp-18h]@16
  float v35; // [sp+1Ch] [bp-14h]@17
  float v36; // [sp+20h] [bp-10h]@4
  float v37; // [sp+24h] [bp-Ch]@4
  float v38; // [sp+24h] [bp-Ch]@16
  float v39; // [sp+28h] [bp-8h]@9
  float v40; // [sp+28h] [bp-8h]@16
  float v41; // [sp+2Ch] [bp-4h]@6
  float v42; // [sp+2Ch] [bp-4h]@9

  //v1 = this;
  if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
  {
    v2 = 0;
    if ( this->uNumVertices > 0 )
    {
      v3 = (int)&this->field_14[1];
      do
      {
        v4 = *(float *)(v3 - 4);
        LODWORD(v37) = *(int *)v3;
        LODWORD(v36) = *(int *)(v3 + 4);
        if (pGame->pIndoorCameraD3D->sRotationX)
        {
          v5 = v4 - (double)pGame->pIndoorCameraD3D->vPartyPos.x;
          v6 = v37 - (double)pGame->pIndoorCameraD3D->vPartyPos.y;
          //if ( pRenderer->pRenderD3D )
          //{
            v41 = pGame->pIndoorCameraD3D->fRotationYSine * v6 + pGame->pIndoorCameraD3D->fRotationYCosine * v5;
            v7 = pGame->pIndoorCameraD3D->fRotationYSine * v5 - pGame->pIndoorCameraD3D->fRotationYCosine * v6;
          /*}
          else
          {
            v41 = pBLVRenderParams->fCosineY * v5 - pBLVRenderParams->fSineY * v6;
            v7 = pBLVRenderParams->fSineY * v5 + pBLVRenderParams->fCosineY * v6;
          }*/
          v8 = v7;
          v9 = v36 - (double)pGame->pIndoorCameraD3D->vPartyPos.z;
          v10 = pGame->pIndoorCameraD3D->fRotationXCosine * v41 - pGame->pIndoorCameraD3D->fRotationXSine * v9;
          v11 = v8;
          v12 = pGame->pIndoorCameraD3D->fRotationXCosine * v9 + pGame->pIndoorCameraD3D->fRotationXSine * v41;
        }
        else
        {
          v42 = v4 - (double)pGame->pIndoorCameraD3D->vPartyPos.x;
          v39 = v37 - (double)pGame->pIndoorCameraD3D->vPartyPos.y;
          //if ( pRenderer->pRenderD3D )
          //{
            v10 = pGame->pIndoorCameraD3D->fRotationYSine * v39 + pGame->pIndoorCameraD3D->fRotationYCosine * v42;
            v11 = pGame->pIndoorCameraD3D->fRotationYSine * v42 - pGame->pIndoorCameraD3D->fRotationYCosine * v39;
          /*}
          else
          {
            v10 = pBLVRenderParams->fCosineY * v42 - pBLVRenderParams->fSineY * v39;
            v11 = pBLVRenderParams->fSineY * v42 + pBLVRenderParams->fCosineY * v39;
          }*/
          v12 = v36 - (double)pGame->pIndoorCameraD3D->vPartyPos.z;
        }
        v13 = v12;
        ++v2;
        *(int *)(v3 + 84) = LODWORD(v13);
        v14 = *(int *)(v3 + 8);
        *(float *)(v3 + 76) = v10;
        *(int *)(v3 + 88) = v14;
        *(float *)(v3 + 80) = v11;
        v3 += 16;
      }
      while ( v2 < this->uNumVertices );
    }
  }
  else
  {
    v15 = (char *)&this->field_14[1];
    v31 = 3;
    do
    {
      v40 = (double)stru_5C6E00->Cos(pGame->pIndoorCameraD3D->sRotationX) * 0.0000152587890625;
      v32 = (double)stru_5C6E00->Sin(pGame->pIndoorCameraD3D->sRotationX) * 0.0000152587890625;
      v34 = (double)stru_5C6E00->Cos(pGame->pIndoorCameraD3D->sRotationY) * 0.0000152587890625;
      v33 = (double)stru_5C6E00->Sin(pGame->pIndoorCameraD3D->sRotationY) * 0.0000152587890625;
      //v16 = stru_5C6E00->Sin(pODMRenderParams->rotation_y);
      LODWORD(v38) = *(int *)v15;
      //UNDEF(v17);
      v20 = *((float *)v15 - 1) - (double)pGame->pIndoorCameraD3D->vPartyPos.x;
      //if ( v19 | v18 )
	  if (pGame->pIndoorCameraD3D->vPartyPos.x == 0)
      {
        v27 = v20;
        LODWORD(v35) = *((int *)v15 + 1);
        v28 = v38 - (double)pGame->pIndoorCameraD3D->vPartyPos.y;
        v25 = v33 * v28 + v34 * v27;
        v26 = v34 * v28 - v33 * v27;
      }
      else
      {
        v21 = v20;
        v22 = v38 - (double)pGame->pIndoorCameraD3D->vPartyPos.y;
        v23 = v33 * v22 + v34 * v21;
        v24 = *((float *)v15 + 1) - (double)pGame->pIndoorCameraD3D->vPartyPos.z;
        v25 = v32 * v24 + v40 * v23;
        v26 = v34 * v22 - v33 * v21;
        v35 = v40 * v24 - v32 * v23;
      }
      *((int *)v15 + 21) = LODWORD(v35);
      v29 = *((int *)v15 + 2);
      *((float *)v15 + 19) = v25;
      *((int *)v15 + 22) = v29;
      *((float *)v15 + 20) = v26;
      v15 += 16;
      --v31;
    }
    while ( v31 );
  }
  this->uNumVertices = 3;
  return 1;
}

//----- (00477F63) --------------------------------------------------------
bool stru6_stru1_indoor_sw_billboard::sub_477F63()
{
  signed int v1; // ebx@1
  stru6_stru1_indoor_sw_billboard *v2; // esi@1
  double v3; // st7@2
  int v4; // edx@4
  char *v5; // ecx@5
  int v6; // edi@5
  float v7; // ST08_4@13
  signed int v9; // [sp+Ch] [bp-8h]@1
  float v10; // [sp+10h] [bp-4h]@2

  auto a1 = this;

  v1 = 0;
  v2 = a1;
  v9 = 0;
  if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
  {
    v10 = 16192.0;
    v3 = (double)pBLVRenderParams->fov_rad_fixpoint * 0.000015258789;
  }
  else
  {
    v10 = (double)pODMRenderParams->shading_dist_mist;
    v3 = 8.0;
  }
  v4 = a1->uNumVertices;
  if ( v4 <= 0 )
  {
    memcpy(&v2->field_14[40], &v2->field_14[20], 16 * v4);
    return v2->uNumVertices != 0;
  }
  v5 = (char *)&a1->field_14[20];
  v6 = v4;
  do
  {
    if ( v3 >= *(float *)v5 || *(float *)v5 >= (double)v10 )
    {
      if ( v3 < *(float *)v5 )
        v9 = 1;
      else
        v1 = 1;
    }
    v5 += 16;
    --v6;
  }
  while ( v6 );
  if ( !v1 )
  {
    if ( v9 )
    {
      v2->_477927(v10);
      return v2->uNumVertices != 0;
    }
    memcpy(&v2->field_14[40], &v2->field_14[20], 16 * v4);
    return v2->uNumVertices != 0;
  }
  v7 = v3;
  _4775ED(v7);
  return v2->uNumVertices != 0;
}

//----- (0047802A) --------------------------------------------------------
int stru6_stru1_indoor_sw_billboard::sub_47802A()
{
  double v6; // st7@4
  double v12; // st7@16
  signed int v16; // [sp+38h] [bp-Ch]@1
  int a6; // [sp+3Ch] [bp-8h]@5
  int a5; // [sp+40h] [bp-4h]@5

  auto a1 = this;

  v16 = 0;
  if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
  {
	for(int i = 0; i < a1->uNumVertices; i++)
    {
		v6 = (double)pBLVRenderParams->fov_rad_fixpoint * 0.000015258789 / a1->field_B4[i*4];
		if ( pRenderer->pRenderD3D )
		{
			pGame->pIndoorCameraD3D->Project(
				round(a1->field_B4[i*4]),
				round(a1->field_B4[i*4+1]),
				round(a1->field_B4[i*4+2]),
				&a5,
				&a6);
			a1->field_B4[i*4+16] = (double)a5;
			a1->field_B4[i*4+17] = (double)a6;
			a1->field_B4[i*4+18] = round(a1->field_B4[i*4]);
		}
		else
		{
			a1->field_B4[i*4+16] = (double)pBLVRenderParams->uViewportCenterX - v6 * a1->field_B4[i*4+1];
			a1->field_B4[i*4+17] = (double)pBLVRenderParams->uViewportCenterY - v6 * a1->field_B4[i*4+2];
			a1->field_B4[i*4+18] = a1->field_B4[i*4];
		}
		a1->field_B4[i*4+19] = a1->field_B4[i*4+3];
		if ( (double)(signed int)pViewport->uViewportTL_X <= a1->field_B4[i*4+16]
			&& (double)(signed int)pViewport->uViewportBR_X > a1->field_B4[i*4+16]
			&& (double)(signed int)pViewport->uViewportTL_Y <= a1->field_B4[i*4+17]
			&& (double)(signed int)pViewport->uViewportBR_Y > a1->field_B4[i*4+17] )
			v16 = 1;
    }
  }
  else
  {
	for(int i = 0; i < a1->uNumVertices; i++)
    {
		v12 = (double)pODMRenderParams->int_fov_rad / a1->field_B4[i*4];
		a1->field_B4[i*4+20] = (double)pViewport->uScreenCenterX - v12 * a1->field_B4[i*4+1];
		a1->field_B4[i*4+21] = (double)pViewport->uScreenCenterY - v12 * a1->field_B4[i*4+2];
		*((int *)&a1->field_B4[i*4+22]) = (int)a1->field_B4[i*4];
		*((int *)&a1->field_B4[i*4+23]) = a1->field_B4[i*4+3];
		if ( (double)(signed int)pViewport->uViewportTL_X <= a1->field_B4[i*4+20]
			&& (double)(signed int)pViewport->uViewportBR_X > a1->field_B4[i*4+20]
			&& (double)(signed int)pViewport->uViewportTL_Y <= a1->field_B4[i*4+21]
			&& (double)(signed int)pViewport->uViewportBR_Y > a1->field_B4[i*4+21] )
			v16 = 1;
    }
  }
  return v16;
}

// 73C84C: using guessed type char static_RenderBuildingsD3D_byte_73C84C_init_flag;

//----- (00479089) --------------------------------------------------------
bool __fastcall IsBModelVisible(unsigned int uModelID, int *reachable)
{
  BSPModel *v2; // eax@1
  int v3; // edi@1
  int v4; // ebx@1
  int v5; // ST28_4@1
  int v6; // ecx@1
  int v7; // ebx@3
  int v8; // eax@3
  int v9; // eax@3
  //int v10; // ebx@3
  signed int v11; // esi@6
  int v12; // esi@8
  bool result; // eax@9
  //unsigned int v14; // [sp+Ch] [bp-20h]@1
  int v15; // [sp+Ch] [bp-20h]@5
  //int *v16; // [sp+10h] [bp-1Ch]@1
  int v17; // [sp+1Ch] [bp-10h]@1
  int v18; // [sp+20h] [bp-Ch]@1
  int v19; // [sp+20h] [bp-Ch]@3
  int angle; // [sp+24h] [bp-8h]@1
  int v21; // [sp+28h] [bp-4h]@1
  //int v22; // [sp+28h] [bp-4h]@3

  //v16 = unused;
  angle = (signed int)(pODMRenderParams->uCameraFovInDegrees << 11) / 360 >> 1;
  //v14 = uModelID;
  v2 = &pOutdoor->pBModels[uModelID];
  v3 = v2->vBoundingCenter.x - pGame->pIndoorCameraD3D->vPartyPos.x;
  v4 = v2->vBoundingCenter.y - pGame->pIndoorCameraD3D->vPartyPos.y;
  v5 = stru_5C6E00->Cos(pGame->pIndoorCameraD3D->sRotationY);
  v21 = stru_5C6E00->Sin(pGame->pIndoorCameraD3D->sRotationY);
  v18 = stru_5C6E00->Cos(pGame->pIndoorCameraD3D->sRotationX);
  stru_5C6E00->Sin(pGame->pIndoorCameraD3D->sRotationX);
  v6 = v5;
  v17 = v3 * v5 + v4 * v21;
  if (pGame->pIndoorCameraD3D->sRotationX)
    v17 = (unsigned __int64)(v17 * (signed __int64)v18) >> 16;
  v19 = v4 * v6 - v3 * v21;
  v7 = abs(v4);
  v8 = abs(v3);
  v9 = int_get_vector_length(v8, v7, 0);
  //v10 = v14 * 188;
  //v22 = v9;
  *reachable = false;
  if ( v9 < pOutdoor->pBModels[uModelID].sBoundingRadius + 256 )
    *reachable = true;
  v15 = stru_5C6E00->Sin(angle);
  if ( v19 >= 0 )
    v11 = ((unsigned __int64)(v15 * (signed __int64)v17) >> 16) - ((unsigned __int64)(stru_5C6E00->Cos(angle) * (signed __int64)v19) >> 16);
  else
    v11 = ((unsigned __int64)(stru_5C6E00->Cos(angle) * (signed __int64)v19) >> 16) + ((unsigned __int64)(v15 * (signed __int64)v17) >> 16);
  v12 = v11 >> 16;
  if ( v9 <= pODMRenderParams->shading_dist_mist + 2048 )
  {
    //if ( abs(v12) > *(int *)((char *)&pOutdoor->pBModels->sBoundingRadius + v10) + 512 )
	if ( abs(v12) > pOutdoor->pBModels[uModelID].sBoundingRadius + 512 )
    {
      result = v12 < 0;
      LOBYTE(result) = v12 >= 0;
      return result;
    }
    else
      return true;
  }
  return false;
}

//----- (00479295) --------------------------------------------------------
int Polygon::_479295()
{
  ODMFace *v2; // esi@1
  int v3; // ecx@4
  int v4; // eax@4
  int v5; // edx@4
  int v6; // ST14_4@5
  int v7; // ST04_4@5
  stru149 *v8; // ecx@5
  Vec3_int_ thisa; // [sp+Ch] [bp-10h]@8
  int v11; // [sp+18h] [bp-4h]@4

  v2 = this->pODMFace;
  if ( !v2->pFacePlane.vNormal.z )
  {
    v3 = v2->pFacePlane.vNormal.x;
    v4 = -v2->pFacePlane.vNormal.y;
    v5 = 0;
    v11 = 65536;
  }
  else if ( (v2->pFacePlane.vNormal.x || v2->pFacePlane.vNormal.y) && abs(v2->pFacePlane.vNormal.z) < 59082 )
  {
    thisa.x = -v2->pFacePlane.vNormal.y;
    thisa.y = v2->pFacePlane.vNormal.x;
    thisa.z = 0;
    thisa.Normalize_float();
    v4 = thisa.x;
    v3 = thisa.y;
    v5 = 0;
    v11 = 65536;
  }
  else
  {
	  v3 = 0;
	  v4 = 65536;
	  v11 = 0;
	  v5 = -65536;
  }
  v6 = v11;
  sTextureDeltaU = v2->sTextureDeltaU;
  v7 = v3;
  v8 = ptr_38;
  sTextureDeltaV = v2->sTextureDeltaV;
  v8->_48616B_frustum_odm(v4, v7, 0, 0, v5, v6);
  return 1;
}


unsigned short *LoadTgaTexture(const wchar_t *filename, int *out_width = nullptr, int *out_height = nullptr)
{
  #pragma pack(push, 1)
    struct TGAHeader
    {
      unsigned char  tgaSkip;
      unsigned char  colourmaptype;      // type of colour map 0=none, 1=has palette
      unsigned char  tgaType;            // type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed

      short colourmapstart;     // first colour map entry in palette
      short colourmaplength;    // number of colours in palette
      char  colourmapbits;      // number of bits per palette entry 15,16,24,32

      //unsigned char  tgaDontCare2[9];
      short xstart;             // image x origin
      short ystart;             // image y origin

      unsigned short tgaWidth;
      unsigned short tgaHeight;
      unsigned char  tgaBPP;

      char  descriptor;         // image descriptor bits:   00vhaaaa
        //      h horizontal flip
        //      v vertical flip
        //      a alpha bits
    };
  #pragma pack(pop)

  if (out_width)
    *out_width = 0;
  if (out_height)
    *out_height = 0;

  DWORD w;
  auto  file = CreateFileW(filename, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
  if (file == INVALID_HANDLE_VALUE)
    return nullptr;

  TGAHeader header;
  ReadFile(file, &header, sizeof(header), &w, nullptr);
  SetFilePointer(file, header.tgaSkip, nullptr, FILE_CURRENT);

  if (header.tgaBPP != 24 || header.tgaType != 2)
  {
    CloseHandle(file);
    return nullptr;
  }

  int imgSize = header.tgaWidth * header.tgaHeight * 3;
  auto pixels = new unsigned char[imgSize];
  ReadFile(file, pixels, imgSize, &w, nullptr);
  CloseHandle(file);

  if (w != imgSize)
  {
    delete [] pixels;
    return nullptr;
  }

  if (out_width)
    *out_width = header.tgaWidth;
  if (out_height)
    *out_height = header.tgaHeight;

  auto pixels_16bit = new unsigned short[imgSize / 3];
  for (int i = 0; i < imgSize / 3; ++i)
  {
    pixels_16bit[i] = (pixels[i * 3] / 8 & 0x1F) |
                      ((pixels[i * 3 + 1] / 4 & 0x3F) << 5) |
                      ((pixels[i * 3 + 2] / 8 & 0x1F) << 11);
  }
  delete [] pixels;
  return pixels_16bit;
}

unsigned short *skybox_xn, *skybox_xp,
              *skybox_yn, *skybox_yp,
              *skybox_zn, *skybox_zp;
int            skybox_width, skybox_height;

IDirect3DTexture2   *skybox_texture;
IDirectDrawSurface4 *skybox_surface;

bool Skybox_Initialize(const wchar_t *skybox_name)
{
  wchar_t xn_filename[1024], xp_filename[1024],
          yn_filename[1024], yp_filename[1024],
          zn_filename[1024], zp_filename[1024];
  swprintf(xn_filename, wcslen(L"%s_xn.tga"), L"%s_xn.tga", skybox_name); swprintf(xp_filename, wcslen(L"%s_xp.tga"), L"%s_xp.tga", skybox_name);
  swprintf(yn_filename, wcslen(L"%s_yn.tga"), L"%s_yn.tga", skybox_name); swprintf(yp_filename, wcslen(L"%s_yp.tga"), L"%s_yp.tga", skybox_name);
  swprintf(zn_filename, wcslen(L"%s_zn.tga"), L"%s_zn.tga", skybox_name); swprintf(zp_filename, wcslen(L"%s_zp.tga"), L"%s_zp.tga", skybox_name);

  int xn_width, xn_height;
  skybox_xn = LoadTgaTexture(xn_filename, &xn_width, &xn_height);
  if (!skybox_xn)
    return false;

  int xp_width, xp_height;
  skybox_xp = LoadTgaTexture(xp_filename, &xp_width, &xp_height);
  if (!skybox_xp || xp_width != xn_width || xp_height != xn_height)
  {
    delete [] skybox_xn;
    if (skybox_xp) delete [] skybox_xp;
    return false;
  }

  int yn_width, yn_height;
  skybox_yn = LoadTgaTexture(yn_filename, &yn_width, &yn_height);
  if (!skybox_yn || yn_width != xn_width || yn_height != xn_height)
  {
    delete [] skybox_xn;
    if (skybox_xp) delete [] skybox_xp;
    if (skybox_yn) delete [] skybox_yn;
    return false;
  }

  int yp_width, yp_height;
  skybox_yp = LoadTgaTexture(yp_filename, &yp_width, &yp_height);
  if (!skybox_yp || yp_width != xn_width || yp_height != xn_height)
  {
    delete [] skybox_xn;
    if (skybox_xp) delete [] skybox_xp;
    if (skybox_yn) delete [] skybox_yn;
    if (skybox_yp) delete [] skybox_yp;
    return false;
  }

  int zn_width, zn_height;
  skybox_zn = LoadTgaTexture(zn_filename, &zn_width, &zn_height);
  if (!skybox_zn || zn_width != xn_width || zn_height != xn_height)
  {
    delete [] skybox_xn;
    if (skybox_xp) delete [] skybox_xp;
    if (skybox_yn) delete [] skybox_yn;
    if (skybox_yp) delete [] skybox_yp;
    if (skybox_zn) delete [] skybox_zn;
    return false;
  }

  int zp_width, zp_height;
  skybox_zp = LoadTgaTexture(zp_filename, &zp_width, &zp_height);
  if (!skybox_zp || zp_width != xn_width || zp_height != xn_height)
  {
    delete [] skybox_xn;
    if (skybox_xp) delete [] skybox_xp;
    if (skybox_yn) delete [] skybox_yn;
    if (skybox_yp) delete [] skybox_yp;
    if (skybox_zn) delete [] skybox_zn;
    if (skybox_zp) delete [] skybox_zp;
    return false;
  }

  skybox_width = xn_width;
  skybox_height = xn_height;

  
  if (!pRenderer->pRenderD3D->CreateTexture(skybox_width, skybox_height, &skybox_surface, &skybox_texture,
                                            false, false, pRenderer->uMinDeviceTextureDim))
    return false;

  return true;
}

struct vector
{
  float x, y, z;
};

struct matrix
{
  float m[4][4];
};

void VectorNormalize(vector *v)
{
  float invmag = 1.0f / sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
  v->x *= invmag;
  v->y *= invmag;
  v->z *= invmag;
}

void MatrixRotationAxis(matrix *pout, CONST vector *pv, float angle)
{
  memset(pout, 0, sizeof(matrix));
  pout->m[3][0] = 0;
  pout->m[3][1] = 0;
  pout->m[3][2] = 0;
  pout->m[3][3] = 1;

  vector v;
  v.x = pv->x; v.y = pv->y; v.z = pv->z;
  VectorNormalize(&v);

    pout->m[0][0] = (1.0f - cos(angle)) * v.x * v.x + cos(angle);
    pout->m[1][0] = (1.0f - cos(angle)) * v.x * v.y - sin(angle) * v.z;
    pout->m[2][0] = (1.0f - cos(angle)) * v.x * v.z + sin(angle) * v.y;
    pout->m[0][1] = (1.0f - cos(angle)) * v.y * v.x + sin(angle) * v.z;
    pout->m[1][1] = (1.0f - cos(angle)) * v.y * v.y + cos(angle);
    pout->m[2][1] = (1.0f - cos(angle)) * v.y * v.z - sin(angle) * v.x;
    pout->m[0][2] = (1.0f - cos(angle)) * v.z * v.x - sin(angle) * v.y;
   pout->m[1][2] = (1.0f - cos(angle)) * v.z * v.y + sin(angle) * v.x;
    pout->m[2][2] = (1.0f - cos(angle)) * v.z * v.z + cos(angle);
}

void VectorTransform(const matrix *m, const vector *v, vector *out)
{
  out->x = m->m[0][0] * v->x + m->m[1][0] * v->y + m->m[2][0] * v->z + m->m[3][0];
  out->y = m->m[0][1] * v->x + m->m[1][1] * v->y + m->m[2][1] * v->z + m->m[3][1];
  out->z = m->m[0][2] * v->x + m->m[1][2] * v->y + m->m[2][2] * v->z + m->m[3][2];
}

bool DrawSkyD3D_Skybox()
{
  static bool initialized = false,
              initialization_failed = false;
  if (initialization_failed)
    return false;
  
  static int last_camera_rot_y,
             last_camera_rot_x;
  if (!initialized)
  {
    if (!Skybox_Initialize(L"data/skybox/stars"))
    {
      initialization_failed = true;
      return false;
    }
    initialized = true;

    last_camera_rot_y = pParty->sRotationY + 1; // force update for the first run 
    last_camera_rot_x = pParty->sRotationX + 1;
  }

  /*
  r(y) = 
cos y	0	sin y	0
0	1	0	0
-sin y	0	cos y	0
0	0	0	1

x cos y - z sin y
y
x sin y + z cos y
1



r(x) =     // should be r(right) actually
1	0      	0	0
0	cos x	-sin x	0
0	sin x	cos x	0
0	0	    0	1


x
y cos x + z sin x
-y sin x + z cos x
1

  */

  if (last_camera_rot_y == pParty->sRotationY &&
      last_camera_rot_x == pParty->sRotationX)
  {
draw:
    struct RenderVertexD3D3  v[6];

    v[0].pos.x = pViewport->uScreen_TL_X;
    v[0].pos.y = pViewport->uScreen_TL_Y;
    v[0].pos.z = 0.99989998;
    v[0].rhw = 1;
    v[0].diffuse = 0xFFFFFFFF;
    v[0].specular = 0;
    v[0].texcoord.x = 0;
    v[0].texcoord.y = 0;
    
    v[1].pos.x = pViewport->uScreen_TL_X + pViewport->uScreenWidth;
    v[1].pos.y = pViewport->uScreen_TL_Y + pViewport->uScreenHeight;
    v[1].pos.z = 0.99989998;
    v[1].rhw = 1;
    v[1].diffuse = 0xFFFFFFFF;
    v[1].specular = 0;
    v[1].texcoord.x = (float)pViewport->uScreenWidth / skybox_width;
    v[1].texcoord.y = (float)pViewport->uScreenHeight / skybox_height;

    v[2].pos.x = pViewport->uScreen_TL_X + pViewport->uScreenWidth;
    v[2].pos.y = pViewport->uScreen_TL_Y;
    v[2].pos.z = 0.99989998;
    v[2].rhw = 1;
    v[2].diffuse = 0xFFFFFFFF;
    v[2].specular = 0;
    v[2].texcoord.x = (float)pViewport->uScreenWidth / skybox_width;
    v[2].texcoord.y = 0;

    memcpy(&v[3], &v[0], sizeof(*v));

    v[4].pos.x = pViewport->uScreen_TL_X;
    v[4].pos.y = pViewport->uScreen_TL_Y + pViewport->uScreenHeight;
    v[4].pos.z = 0.99989998;
    v[4].rhw = 1;
    v[4].diffuse = 0xFFFFFFFF;
    v[4].specular = 0;
    v[4].texcoord.x = 0;
    v[4].texcoord.y = (float)pViewport->uScreenHeight / skybox_height;

    memcpy(&v[5], &v[1], sizeof(*v));

    pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);
    pRenderer->pRenderD3D->pDevice->SetTexture(0, skybox_texture);
    pRenderer->pRenderD3D->pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1, v, 6, D3DDP_DONOTUPDATEEXTENTS | D3DDP_DONOTLIGHT);
    //pRenderer->pRenderD3D->pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1, v + 1, 3, D3DDP_DONOTUPDATEEXTENTS | D3DDP_DONOTLIGHT);

    return true;
  }


  DDSURFACEDESC2 desc;
  desc.dwSize = sizeof(desc);
  if (!pRenderer->LockSurface_DDraw4(skybox_surface, &desc, DDLOCK_WAIT | DDLOCK_WRITEONLY))
    return false;
  
  last_camera_rot_y = pParty->sRotationY;
  last_camera_rot_x = pParty->sRotationX;

  float aspect = (float)pViewport->uScreenWidth / (float)pViewport->uScreenHeight;
  float fov_x = 3.141592f * (pODMRenderParams->uCameraFovInDegrees + 0) / 360.0f;
  float fov_y = fov_x / aspect;

  float ray_dx = fov_x / (float)pViewport->uScreenWidth,
        ray_dy = fov_y / (float)pViewport->uScreenHeight;
  float party_angle_x = 2 * 3.141592653589 * pParty->sRotationX / 2048.0,
        party_angle_y = 2 * 3.141592653589 * pParty->sRotationY / 2048.0;
  for (int y = 0; y < pViewport->uScreenHeight; ++y)
    for (int x = 0; x < pViewport->uScreenWidth; ++x)
    {
      float angle_x = party_angle_x - (y - pViewport->uScreenHeight / 2) * ray_dy;
      float angle_y = party_angle_y - (x - pViewport->uScreenWidth / 2) * ray_dx;

      float _dir_x_ = 1,
            _dir_y_ = 0,
            _dir_z_ = 0;

      float dir_x_ = _dir_x_ * cosf(angle_y);// - _dir_z_ * sinf(angle_y);  // rotation around y
      //float dir_y_ = _dir_y_;
      float dir_z_ = _dir_x_ * sinf(angle_y);// + _dir_z_ * cosf(angle_y);

      //float dir_x =  dir_x_;                                               // rotation around x
      //float dir_y =  /*dir_y_ * cosf(angle_x)*/ + dir_z_ * sinf(angle_x);
      //float dir_z = /*-dir_y_ * sinf(angle_x)*/ + dir_z_ * cosf(angle_x);

      vector right;                                            // rotate around right actually to avoid space distortion
      right.x = /*dir_y * 0*/ - dir_z_ * 1;
      right.y = /*dir_z_ * 0 - dir_x_ * */0;
      right.z = dir_x_ * 1/* - dir_y_ * 0*/;
      //VectorNormalize(&right);

      matrix rightMatrix;
      MatrixRotationAxis(&rightMatrix, &right, angle_x);

      vector v1, v2;
      v1.x = dir_x_; v1.y = 0; v1.z = dir_z_;
      VectorTransform(&rightMatrix, &v1, &v2);

      float dir_x = v2.x,
            dir_y = v2.y,
            dir_z = v2.z;

      float abs_dir_x = fabsf(dir_x),
            abs_dir_y = fabsf(dir_y),
            abs_dir_z = fabsf(dir_z);

      unsigned short color = (0x1F << 11) | (0x1F << 5) | (5);  //default to orange
      if (abs_dir_x >= abs_dir_y)
      {
        if (abs_dir_x >= abs_dir_z)
        {
          if (dir_x >= 0)
          {
            float instersect_y = dir_y / (2.0f * dir_x); // plane equation for this side is x + 0.5 = 0
            float instersect_z = dir_z / (2.0f * dir_x);

            float u = 1.0f - (instersect_z + 0.5f),
                  v = 1.0f - (instersect_y + 0.5f);
            
            int tx = u * (skybox_width - 1),
                ty = v * (skybox_height - 1);

            color = skybox_xp[ty * skybox_width + tx];
            //color = ty * 0x1F / skybox_height;
          }
          else
          {
            float instersect_y = dir_y / (2.0f * dir_x);
            float instersect_z = dir_z / (2.0f * dir_x);

            float u = 1.0f - (instersect_z + 0.5f),
                  v = instersect_y + 0.5f;
            
            int tx = u * (skybox_width - 1),
                ty = v * (skybox_height - 1);

            color = skybox_xn[ty * skybox_width + tx];
            //color = tx * 0x1F / skybox_height;
          }
        }
        else if (dir_z >= 0)
          goto DIR_ZP;
        else
          goto DIR_ZN;
      }
      else if (abs_dir_y >= abs_dir_z)
      {
        if (dir_y >= 0)
        {
            float instersect_x = dir_x / (2.0f * dir_y);
            float instersect_z = dir_z / (2.0f * dir_y);

            float u = instersect_x + 0.5f,
                  v = instersect_z + 0.5f;
            
            int tx = u * (skybox_width - 1),
                ty = v * (skybox_height - 1);

            color = skybox_yp[ty * skybox_width + tx];
            //color = tx * 0x1F / skybox_height;
        }
        /*else   should never be seen i guess
        {
            __debugbreak();
          // -y
            //Log::Warning(L"(%03u, %03u): -y", x, y);
        }*/
      }
      else if (dir_z >= 0)
      {
DIR_ZP:
        // +z
        float instersect_x = dir_x / (2.0f * dir_z);
        float instersect_y = dir_y / (2.0f * dir_z);
        //float intersect_z = 0.5f;

        float u = instersect_x + 0.5f,
              v = -instersect_y + 0.5f;

        int tx = u * (skybox_width - 1),
            ty = v * (skybox_height - 1);

        color = skybox_zp[ty * skybox_width + tx];
      }
      else
      {
DIR_ZN:
        // -z
        float instersect_x = -dir_x / (2.0f * dir_z);
        float instersect_y = -dir_y / (2.0f * dir_z);
        //float intersect_z = -0.5f;

        float u = 1.0f - instersect_x - 0.5f,
              v = -instersect_y + 0.5f;

        int tx = u * (skybox_width - 1),
            ty = v * (skybox_height - 1);

        color = skybox_zn[ty * skybox_width + tx];
      }

      //pRenderer->pTargetSurface[(pViewport->uScreenY + y) * pRenderer->uTargetSurfacePitch + pViewport->uScreenX + x] = color;
      ((unsigned __int16 *)((char *)desc.lpSurface + y * desc.lPitch))[x] = color;
    }
    
  ErrD3D((skybox_surface)->Unlock(0));
  goto draw;
}

//----- (00479543) --------------------------------------------------------
void Render::DrawSkyD3D()
{
  int v9; // eax@4
  int v10; // ebx@4
  int v13; // edi@6
  int v14; // ecx@6
  int v15; // eax@8
  int v16; // eax@12
  signed __int64 v17; // qtt@13
  signed int v18; // ecx@13
  struct Polygon pSkyPolygon; // [sp+14h] [bp-150h]@1
  double v26; // [sp+120h] [bp-44h]@4
  int v30; // [sp+134h] [bp-30h]@1
  int v32; // [sp+13Ch] [bp-28h]@6
  int v33; // [sp+140h] [bp-24h]@2
  signed __int64 v34; // [sp+144h] [bp-20h]@1
  int v35; // [sp+148h] [bp-1Ch]@4
  int v36; // [sp+14Ch] [bp-18h]@2
  int v37; // [sp+154h] [bp-10h]@8
  int v38; // [sp+158h] [bp-Ch]@1
  int v39; // [sp+15Ch] [bp-8h]@4
  int v40; // [sp+160h] [bp-4h]@7

  v30 = (signed __int64)((double)(pODMRenderParams->int_fov_rad * pGame->pIndoorCameraD3D->vPartyPos.z)
                       / ((double)pODMRenderParams->int_fov_rad + 8192.0)
                       + (double)(pViewport->uScreenCenterY + 7));//include "+ 7"
  v34 = cos((double)pGame->pIndoorCameraD3D->sRotationX * 0.0030664064) * 0x2000;//(double)pODMRenderParams->shading_dist_mist
  v38 = (signed __int64)((double)(pViewport->uScreenCenterY + 7)
                       - (double)pODMRenderParams->int_fov_rad
                       / (v34 + 0.0000001)
                       * (sin((double)pGame->pIndoorCameraD3D->sRotationX * 0.0030664064)
                        * (double)-0x2000//(double)pODMRenderParams->shading_dist_mist
                        - (double)pGame->pIndoorCameraD3D->vPartyPos.z));
  pSkyPolygon.Create_48607B(&stru_8019C8);//заполн€етс€ ptr_38
  pSkyPolygon.ptr_38->_48694B_frustum_sky();
  pSkyPolygon.uTileBitmapID = pOutdoor->uSky_TextureID;//179(original 166)
  pSkyPolygon.pTexture = (Texture *)(SLOWORD(pOutdoor->uSky_TextureID) != -1 ? (int)&pBitmaps_LOD->pTextures[SLOWORD(pOutdoor->uSky_TextureID)] : 0);
  if ( pSkyPolygon.pTexture )
  {
    pSkyPolygon.dimming_level = 0;
    pSkyPolygon.uNumVertices = 4;
  //centering(центруем)--наклон камеры ----------------------------------------
    pSkyPolygon.v_18.x = -stru_5C6E00->Sin(pGame->pIndoorCameraD3D->sRotationX + 16);
    pSkyPolygon.v_18.y = 0;
    pSkyPolygon.v_18.z = -stru_5C6E00->Cos(pGame->pIndoorCameraD3D->sRotationX + 16);
  
  //sky wiew position(положение неба на экране)------------------------------------------
  //                X
  // 0._____________________________.3
  //  |8,8                    468,8 |
  //  |                             |
  //  |                             |
  // Y|                             |
  //  |                             |
  //  |8,351                468,351 |
  // 1._____________________________.2
  // 
    array_50AC10[0].vWorldViewProjX = (double)(signed int)pViewport->uViewportTL_X;
    array_50AC10[0].vWorldViewProjY = (double)(signed int)pViewport->uViewportTL_Y;

    array_50AC10[1].vWorldViewProjX = (double)(signed int)pViewport->uViewportTL_X;
    array_50AC10[1].vWorldViewProjY = (double)v38;

    array_50AC10[2].vWorldViewProjX = (double)(signed int)pViewport->uViewportBR_X;
    array_50AC10[2].vWorldViewProjY = (double)v38;

    array_50AC10[3].vWorldViewProjX = (double)(signed int)pViewport->uViewportBR_X;
    array_50AC10[3].vWorldViewProjY = (double)(signed int)pViewport->uViewportTL_Y;

    pSkyPolygon.sTextureDeltaU = 224 * pMiscTimer->uTotalGameTimeElapsed;
    pSkyPolygon.sTextureDeltaV = 224 * pMiscTimer->uTotalGameTimeElapsed;

    pSkyPolygon.field_24 = 0x2000000u;
    v33 = 65536 / (signed int)(signed __int64)(((double)(pViewport->uViewportBR_X - pViewport->uViewportTL_X) * 0.5) / tan(0.6457717418670654) + 0.5);
    for ( uint i = 0; i < pSkyPolygon.uNumVertices; ++i )
    {
      v26 = array_50AC10[i].vWorldViewProjY + 6.7553994e15;
      //rotate skydome(вращение купола неба)--------------------------------------
      // ¬ игре прин€та сво€ система измерени€ углов. ѕолный угол (180). «начению угла 0 соответствует 
      // направление на север и/или юг (либо на восток и/или запад), значению 65536 еденицам(0х10000) соответствует угол 90.
	  // две переменные хран€т данные по углу обзора. field_14 по западу и востоку. field_20 по югу и северу
      // от -25080 до 25080
      v39 = (unsigned __int64)(pSkyPolygon.ptr_38->viewing_angle_from_west_east * (signed __int64)(v33 * (v30 - array_50AC10[i].vWorldViewProjY))) >> 16;
      v35 = v39 + pSkyPolygon.ptr_38->angle_from_north;

      v39 = (unsigned __int64)(pSkyPolygon.ptr_38->viewing_angle_from_north_south * (signed __int64)(v33 * (v30 - array_50AC10[i].vWorldViewProjY))) >> 16;
      v36 = v39 + pSkyPolygon.ptr_38->angle_from_east;

      v38 = pSkyPolygon.v_18.z;
      v9 = (unsigned __int64)(pSkyPolygon.v_18.z * (signed __int64)(v33 * (v30 - array_50AC10[i].vWorldViewProjY))) >> 16;
      v10 = pSkyPolygon.v_18.x + v9;
      v39 = pSkyPolygon.v_18.x + v9;
      if ( pSkyPolygon.v_18.x + v9 > 0 )
      {
        v10 = 0;
        v39 = 0;
      }
      v38 = v10;
      v13 = v33 * (pViewport->uScreenCenterX - (signed __int64)array_50AC10[i].vWorldViewProjX);
      v34 = -pSkyPolygon.field_24;
      v32 = (signed __int64)array_50AC10[i].vWorldViewProjX;
      v14 = v33 * (v30 - (signed __int64)array_50AC10[i].vWorldViewProjX);
      while ( 1 )
      {
        v40 = v14;
        if ( !v10 )
          goto LABEL_12;
        v37 = abs((int)v34 >> 14);
        v15 = abs(v10);
        if ( v37 <= v15 || v32 <= (signed int)pViewport->uViewportTL_Y )
        {
          if ( v39 <= 0 )
            break;
        }
        v14 = v40;
LABEL_12:
        v37 = pSkyPolygon.v_18.z;
        v16 = (unsigned __int64)(pSkyPolygon.v_18.z * (signed __int64)v14) >> 16;
        --v32;
        v14 += v33;
        v10 = pSkyPolygon.v_18.x + v16;
        v39 = pSkyPolygon.v_18.x + v16;
        v38 = pSkyPolygon.v_18.x + v16;
      }
      LODWORD(v17) = LODWORD(v34) << 16;
      HIDWORD(v17) = v34 >> 16;
      //v40 = v17 / v38;
      v18 = v17 / v38;
      if ( v18 < 0 )
        v18 = pODMRenderParams->shading_dist_mist;
      v37 = v35 + ((unsigned __int64)(pSkyPolygon.ptr_38->angle_from_west * (signed __int64)v13) >> 16);
      v35 = 224 * pMiscTimer->uTotalGameTimeElapsed
          + ((signed int)((unsigned __int64)(v37 * (signed __int64)v18) >> 16) >> 3);
      array_50AC10[i].u = (double)v35 / ((double)pSkyPolygon.pTexture->uTextureWidth * 65536.0);

      v36 = v36 + ((unsigned __int64)(pSkyPolygon.ptr_38->angle_from_south * (signed __int64)v13) >> 16);
      v35 = 224 * pMiscTimer->uTotalGameTimeElapsed
         + ((signed int)((unsigned __int64)(v36 * (signed __int64)v18) >> 16) >> 3);
      array_50AC10[i].v = (double)v35 / ((double)pSkyPolygon.pTexture->uTextureHeight * 65536.0);

      array_50AC10[i].vWorldViewPosition.x = (double)0x2000;//pODMRenderParams->shading_dist_mist
      array_50AC10[i]._rhw = 1.0 / (double)(v18 >> 16);
    }
    pRenderer->DrawSkyPolygon(pSkyPolygon.uNumVertices, &pSkyPolygon, pBitmaps_LOD->pHardwareTextures[(signed __int16)pSkyPolygon.uTileBitmapID]);
    array_50AC10[0].vWorldViewProjY = (double)v38;
    array_50AC10[1].vWorldViewProjY = array_50AC10[1].vWorldViewProjY + 30.0;
    array_50AC10[2].vWorldViewProjY = array_50AC10[2].vWorldViewProjY + 30.0;
    array_50AC10[3].vWorldViewProjY = (double)v38;
    pRenderer->DrawSkyPolygon(pSkyPolygon.uNumVertices, &pSkyPolygon, pBitmaps_LOD->pHardwareTextures[(signed __int16)pSkyPolygon.uTileBitmapID]);
    return;
  }
}

//----- (0047A384) --------------------------------------------------------
void ODM_LoadAndInitialize(const char *pLevelFilename, ODMRenderParams *thisa)
{
  int v2; // ebx@3
  unsigned int v3; // eax@3
  MapInfo *v4; // edi@4
  //int v5; // eax@8
  //SpawnPointMM7 *v6; // edx@14
  size_t v7; // eax@19
  //char *v8; // eax@19
  //char *v9; // eax@21
  char Source[120]; // [sp+Ch] [bp-84h]@19
  const char *pFilename; // [sp+84h] [bp-Ch]@1
  //unsigned int v12; // [sp+88h] [bp-8h]@12
  //int v13; // [sp+8Ch] [bp-4h]@11

  pFilename = pLevelFilename;
  //thisa->AllocSoftwareDrawBuffers();
  pODMRenderParams->Initialize();
  pWeather->bRenderSnow = false;
  pRenderer->ClearZBuffer(0, 479);
  thisa = (ODMRenderParams *)1;
  GetAlertStatus();
  if ( qword_A750D8 )
    qword_A750D8 = 0;
  v2 = pMapStats->GetMapInfo(pCurrentMapName);
  v3 = 0;
  if ( v2 )
  {
    v4 = &pMapStats->pInfos[v2];
    v3 = v4->uRespawnIntervalDays;
  }
  else
  {
    v4 = (MapInfo *)thisa;
  }
  day_attrib &= ~DAY_ATTRIB_FOG;
  dword_6BE13C_uCurrentlyLoadedLocationID = v2;
  pOutdoor->Initialize(
    pFilename,
    (unsigned int)(signed __int64)((double)(signed __int64)pParty->uTimePlayed * 0.234375) / 0x3C / 0x3C / 0x18 + 1,
    v3,
    (int)&thisa);
  if ( !(BYTE1(dword_6BE364_game_settings_1) & 0x20) )
  {
    InitializeActors();
    InitializeSpriteObjects();
  }
  BYTE1(dword_6BE364_game_settings_1) &= 0xDFu;
  //v5 = 0;
  if ( !v2 )
    thisa = 0;
  if ( thisa == (ODMRenderParams *)1 )
  {
    //v13 = 0;
    for (uint i = 0; i < pOutdoor->uNumSpawnPoints; ++i)
    {
      //v12 = 0;
      //while ( 1 )
      //{
      auto spawn = pOutdoor->pSpawnPoints + i;
        //v6 = &pOutdoor->pSpawnPoints[v12 / 0x18];
      if (spawn->uKind == 3 )
        SpawnEncounter(v4, spawn, 0, 0, 0);
      else
        v4->SpawnRandomTreasure(spawn);
        //++v13;
        //v12 += 24;
        //if ( v13 >= (signed int)pOutdoor->uNumSpawnPoints )
        //  break;
        //v5 = 0;
      //}
    }
    RespawnGlobalDecorations();
  }
  pOutdoor->PrepareDecorations();
  pOutdoor->_47F223_LooksLikeGenerateMonsterLoot();
  pOutdoor->InitalizeActors(v2);
  pOutdoor->MessWithLUN();
  v7 = strlen("levels\\");
  strcpy(Source, &pFilename[v7]);
  strcpy(pOutdoor->pLevelFilename, Source);
  pWeather->Initialize();
  pGame->pIndoorCameraD3D->sRotationY = pParty->sRotationY;
  pGame->pIndoorCameraD3D->sRotationX = pParty->sRotationX;
  //pODMRenderParams->RotationToInts();
  pOutdoor->UpdateSunlightVectors();

  float fov_rad;
  float fov_rad_inv;
  //----- (0042394D) --------------------------------------------------------
  //void IndoorCamera::Initialize(int degFov, unsigned int uViewportWidth, unsigned int uViewportHeight)
  {
    //pIndoorCamera->Initialize(65, viewparams->uScreen_BttmR_X - viewparams->uScreen_topL_X + 1,
    //                              viewparams->uScreen_BttmR_Y - viewparams->uScreen_topL_Y + 1);

    int uViewportWidth = viewparams->uScreen_BttmR_X - viewparams->uScreen_topL_X + 1;

    extern float _calc_fov(int viewport_width, int angle_degree);
    fov_rad = _calc_fov(uViewportWidth, 65);
    fov_rad_inv = 65536.0 / fov_rad;
  }
  pODMRenderParams->int_fov_rad = (signed __int64)fov_rad;
  pODMRenderParams->int_fov_rad_inv = (signed __int64)fov_rad_inv;

  for (int i = 0; i < 20000; ++i)
  {
    array_77EC08[i].ptr_38 = &stru_8019C8;

    array_77EC08[i].ptr_48 = nullptr;
  }

  MM7Initialization();
}

//----- (0047C370) --------------------------------------------------------
unsigned int GetLevelFogColor()
{
  signed __int64 v1; // qax@5
  int v2; // eax@6

  if (bUnderwater)
    return 0xFF258F5C;

  if (day_attrib & DAY_ATTRIB_FOG)
  {
      if ( pWeather->bNight ) // night-time fog
      {
        __debugbreak(); // decompilation can be inaccurate, please send savegame to Nomad
        v2 = -(pWeather->bNight != 1);
        return (v2 & 0xE0E0E1) - 0xE0E0E1;
      }
      else
      {
        v1 = (signed __int64)((1.0 - pOutdoor->fFogDensity) * 200.0 + pOutdoor->fFogDensity * 31.0);
        return v1 | (((unsigned int)v1 | (((unsigned int)v1 | 0xFFFFFF00) << 8)) << 8);
      }
  }

  return 0;
}

//----- (0047C3D7) --------------------------------------------------------
int __fastcall sub_47C3D7_get_fog_related_stuff(int a1, int a2, float a3)
{
  int v3; // ecx@1
  double v5; // st7@10
  signed int v7; // ecx@11
  double v8; // st6@12
  double v9; // st7@15
  double v10; // st6@16
  float v11; // ST14_4@17
  double v12; // ST08_8@17

  v3 = pWeather->bNight;
  if ( bUnderwater == 1 )
    v3 = 0;
  if ( pParty->armageddon_timer || !(day_attrib & DAY_ATTRIB_FOG) && !bUnderwater )
    return 0xFF000000;
  if ( v3 )
  {
    v5 = (double)day_fogrange_1;
    if ( a3 < v5 )
	{
      v7 = 0;
      if ( a3 == 0.0 )
	  {
		v7 = 216;
	  }
	  goto LABEL_21;
	}
	else
	{
		v8 = (double)day_fogrange_2;
		if ( a3 > v8 )
		{
		  v7 = 216;
 		  if ( a3 == 0.0 )
		  {
			v7 = 216;
		  }
		  goto LABEL_21;
		}
		v7 = (signed __int64)((a3 - v5) / (v8 - v5) * 216.0);
	}
  }
  else
  {
    v9 = (double)day_fogrange_1;
    if ( a3 < v9 )
    {
      v7 = 0;
	  if ( a3 == 0.0 )
	  {
		v7 = 216;
	  }
      goto LABEL_21;
    }
	else
	{
		v10 = (double)day_fogrange_2;
		if ( a3 > v10 )
		{  
		  v7 = 216;
		  if ( a3 == 0.0 )
		  {
			v7 = 216;
		  }
		  goto LABEL_21;
		}
		else
		{
			v11 = (a3 - v9) * 216.0 / (v10 - v9);
			v12 = v11 + 6.7553994e15;
			v7 = LODWORD(v12);
		}
	}
  }
  if ( v7 > 216 )
  {
    v7 = 216;
  }
  else
  {
	  if ( a3 == 0.0 )
	  {
		v7 = 216;
	  }
  }
LABEL_21:
  if ( a2 )
    v7 = 248;
  return (-1 - v7) << 24;
}

//----- (0047C4FC) --------------------------------------------------------
int __fastcall GetActorTintColor(int max_dimm, int min_dimm, float distance, int a4, RenderBillboard *a5)
{
  //int v5; // esi@1
  signed int v6; // edx@1
  //signed int result; // eax@2
  int v8; // eax@3
  double v9; // st7@12
  //double v10; // ST0C_8@18
  int v11; // ecx@28
  //signed int v12; // edi@28
  //double v13; // ST0C_8@33
  //double v14; // ST0C_8@34
  double v15; // st7@44
  //double v16; // ST0C_8@44
  //double v17; // ST0C_8@44
  int v18; // ST14_4@44
  //double v19; // ST0C_8@44
  signed int v20; // [sp+10h] [bp-4h]@10
  float a3a; // [sp+1Ch] [bp+8h]@33
  //float a3b; // [sp+1Ch] [bp+8h]@34
  float a3c; // [sp+1Ch] [bp+8h]@44
  //float a3d; // [sp+1Ch] [bp+8h]@44
  //float a4b; // [sp+20h] [bp+Ch]@18
  //int a4a; // [sp+20h] [bp+Ch]@33
  //float a4c; // [sp+20h] [bp+Ch]@44
  //float a4d; // [sp+20h] [bp+Ch]@44
  int a5a; // [sp+24h] [bp+10h]@44

  //v5 = a2;
  v6 = 0;

  if (uCurrentlyLoadedLevelType == LEVEL_Indoor)
    return 8 * (31 - max_dimm) | ((8 * (31 - max_dimm) | ((31 - max_dimm) << 11)) << 8);

  if (pParty->armageddon_timer)
    return 0xFFFF0000;

  v8 = pWeather->bNight;
  if (bUnderwater)
    v8 = 0;
  if ( v8 )
  {
    v20 = 1;
    if ( (signed __int64)pParty->pPartyBuffs[PARTY_BUFF_TORCHLIGHT].uExpireTime > 0 )
      v20 = pParty->pPartyBuffs[PARTY_BUFF_TORCHLIGHT].uPower;
    v9 = (double)v20 * 1024.0;
    if ( a4 )
      goto LABEL_19;
    if ( distance <= v9 )
    {
      if ( distance > 0.0 )
      {
        //a4b = distance * 216.0 / v9;
        //v10 = a4b + 6.7553994e15;
        //v6 = LODWORD(v10);
        v6 = floorf(0.5f + distance * 216.0 / v9);
        if (v6 > 216 )
          goto LABEL_19;
      }
    }
    else
    {
      v6 = 216;
    }
    if ( distance != 0.0 )
    {
LABEL_20:
      if ( a5 )
        v6 = 8 * _43F55F_get_billboard_light_level(a5, v6 >> 3);
      if ( v6 > 216 )
        v6 = 216;
      return (255 - v6) | ((255 - v6) << 16) | ((255 - v6) << 8);
    }
LABEL_19:
    v6 = 216;
    goto LABEL_20;
  }



  if (fabsf(distance) < 1.0e-6f)
    return 0xFFF8F8F8;

  // dim in measured in 8-steps
  v11 = 8 * (max_dimm - min_dimm);
    //v12 = v11;
    if ( v11 >= 0 )
    {
      if ( v11 > 216 )
        v11 = 216;
    }
    else
      v11 = 0;

    float fog_density_mult = 216.0f;
    if (a4)
      fog_density_mult += distance / (double)pODMRenderParams->shading_dist_shade * 32.0;

    v6 = v11 + floorf(pOutdoor->fFogDensity * fog_density_mult + 0.5f);
    /*if ( a4 )
    {
      //a3b = pOutdoor->fFogDensity * 216.0;
      //v14 = a3b + 6.7553994e15;
      //a4a = floorf(a3b + 0.5f);//LODWORD(v14);
    }
    else
    {
      //a3a = (distance / (double)pODMRenderParams->shading_dist_shade * 32.0 + 216.0) * pOutdoor->fFogDensity;
      //v13 = a3a + 6.7553994e15;
      //a4a = floorf(a3a + 0.5f);//LODWORD(v13);
    }
    v6 = a4a + v11;*/
    if ( a5 )
      v6 = 8 * _43F55F_get_billboard_light_level(a5, v6 >> 3);
    if ( v6 > 216 )
      v6 = 216;
    if ( v6 < v11 )
      v6 = v11;
    if ( v6 > 8 * pOutdoor->max_terrain_dimming_level )
      v6 = 8 * pOutdoor->max_terrain_dimming_level;
    if ( !bUnderwater )
      return (255 - v6) | ((255 - v6) << 16) | ((255 - v6) << 8);
    else
    {
    v15 = (double)(255 - v6) * 0.0039215689;
    a3c = v15;
    //a4c = v15 * 16.0;
    //v16 = a4c + 6.7553994e15;
    a5a = floorf(v15 * 16.0 + 0.5f);//LODWORD(v16);
    //a4d = a3c * 194.0;
    //v17 = a4d + 6.7553994e15;
    v18 = floorf(a3c * 194.0 + 0.5f);//LODWORD(v17);
    //a3d = a3c * 153.0;
    //v19 = a3d + 6.7553994e15;
     return (int)floorf(a3c * 153.0 + 0.5f)/*LODWORD(v19)*/ | ((v18 | (a5a << 8)) << 8);
    }
}
// 6BE3C4: using guessed type char bUnderwater;

//----- (0047F44B) --------------------------------------------------------
unsigned int WorldPosToGridCellX(int sWorldPosX)
{
  return (sWorldPosX >> 9) + 64; // sar is in original exe, resulting -880 / 512 = -1
                                 //                               and -880 sar 9 = -2
}

//----- (0047F458) --------------------------------------------------------
unsigned int WorldPosToGridCellZ(int sWorldPosZ)
{
  return 64 - (sWorldPosZ >> 9); // sar is in original exe, resulting -880 / 512 = -1
                                 //                               and -880 sar 9 = -2
}

//----- (0047F469) --------------------------------------------------------
int GridCellToWorldPosX(int a1)
{
  return (a1 - 64) << 9;
}

//----- (0047F476) --------------------------------------------------------
int GridCellToWorldPosZ(int a1)
{
  return (64 - a1) << 9;
}


//----- (004811A3) --------------------------------------------------------
void Polygon::DrawBorderTiles()
{
  //__debugbreak();

  pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, false);
  pRenderer->DrawTerrainPolygon(uNumVertices, this,
    pBitmaps_LOD->pHardwareTextures[pRenderer->pHDWaterBitmapIDs[pRenderer->hd_water_current_frame]], false, true);

  pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, true);
  //pRenderer->DrawTerrainPolygon(uNumVertices, this, pBitmaps_LOD->pHardwareTextures[uTileBitmapID], true, true);
}


//----- (00481ED9) --------------------------------------------------------
void  sub_481ED9_MessWithODMRenderParams()
{
  stru_8019C8._48616B_frustum_odm(65536, 0, 0, 0, 65536, 0);
  pODMRenderParams->uNumPolygons = 0;
  //pODMRenderParams->uNumEdges = 0;
  //pODMRenderParams->uNumSpans = 0;
  //pODMRenderParams->uNumSurfs = 0;
  pODMRenderParams->uNumBillboards = 0;
  pODMRenderParams->field_44 = 0;
}

//----- (004823F4) --------------------------------------------------------
bool IsTerrainSlopeTooHigh(int pos_x, int pos_z)
{
  //unsigned int v2; // ebx@1
  //unsigned int v3; // edi@1
  //int v4; // eax@1
  //int v6; // esi@5
  //int v7; // ecx@6
  //int v8; // edx@6
  //int v9; // eax@6
  //int v10; // esi@10
  //int v11; // [sp+14h] [bp-8h]@1
  //int v12; // [sp+18h] [bp-4h]@1

  //v12 = a1;
  //v11 = a2;
  auto grid_x = WorldPosToGridCellX(pos_x);
  auto grid_z = WorldPosToGridCellZ(pos_z) - 1;

  auto party_grid_x1 = GridCellToWorldPosX(grid_x);
  //dword_76D56C_terrain_cell_world_pos_around_party_x = GridCellToWorldPosX(grid_x + 1);
  //dword_76D570_terrain_cell_world_pos_around_party_x = GridCellToWorldPosX(grid_x + 1);
  //dword_76D574_terrain_cell_world_pos_around_party_x = GridCellToWorldPosX(grid_x);
  auto party_grid_z1 = GridCellToWorldPosZ(grid_z);
  //dword_76D55C_terrain_cell_world_pos_around_party_z = GridCellToWorldPosZ(grid_z);
  //dword_76D560_terrain_cell_world_pos_around_party_z = GridCellToWorldPosZ(grid_z + 1);
  //dword_76D564_terrain_cell_world_pos_around_party_z = GridCellToWorldPosZ(grid_z + 1);
  auto party_x1z1_y = pOutdoor->DoGetHeightOnTerrain(grid_x, grid_z);
  auto party_x2z1_y = pOutdoor->DoGetHeightOnTerrain(grid_x + 1, grid_z);
  auto party_x2z2_y = pOutdoor->DoGetHeightOnTerrain(grid_x + 1, grid_z + 1);
  auto party_x1z2_y = pOutdoor->DoGetHeightOnTerrain(grid_x, grid_z + 1);
  //dword_76D554_terrain_cell_world_pos_around_party_y = v4;
  if (party_x1z1_y == party_x2z1_y &&
      party_x2z1_y == party_x2z2_y &&
      party_x2z2_y == party_x1z2_y )
    return false;

  auto dx = abs(pos_x - party_grid_x1),
       dz = abs(party_grid_z1 - pos_z);

  int y1, y2, y3;
  if (dz >= dx)
  {
    y1 = party_x1z2_y;  //  lower-left triangle
    y2 = party_x2z2_y;  //  y3 | \ 
    y3 = party_x1z1_y;  //     |   \ 
                        /*     |     \ 
                               |______ \
                            y1           y2   */
  }
  else
  {
    y1 = party_x2z1_y;  // upper-right
    y2 = party_x1z1_y;  //  y2_______ y1
    y3 = party_x2z2_y;  //    \     |
                        /*      \   |
                                  \ |
                                    y3     */
  }

  int y_min = min(y1, min(y2, y3)),
      y_max = max(y1, max(y2, y3));
  return (y_max - y_min) > 512;

  /*if ( y1 >= y2 )
  {
    y_min = y2;
    if ( y2 < y3 )
      goto LABEL_13;
LABEL_12:
    y_min = y3;
    goto LABEL_13;
  }
  else if ( y1 >= y3 )
    goto LABEL_12;
  else
  y_min = y1;

LABEL_13:
  if ( y1 <= y2 )
  {
    if ( y2 > y3 )
      y3 = y2;
  }
  else
  {
    if ( y1 > y3 )
      y3 = y1;
  }
  return y3 - v10 > 512;*/
}

//----- (0048257A) --------------------------------------------------------
int __fastcall GetTerrainHeightsAroundParty2(int a1, int a2, int *pIsOnWater, int bFloatAboveWater)
{
  //unsigned int v4; // ebx@1
  //unsigned int v5; // edi@1
  int result; // eax@9
  //int v7; // ebx@10
  int v8; // ebx@11
  int v9; // eax@11
  int v10; // ecx@11
  //int v11; // [sp+Ch] [bp-Ch]@1
  //int v12; // [sp+10h] [bp-8h]@1
  int v13; // [sp+10h] [bp-8h]@11
  signed int v14; // [sp+14h] [bp-4h]@3
  int v15; // [sp+24h] [bp+Ch]@11

  //v11 = a1;
  //v12 = a2;
  auto grid_x = WorldPosToGridCellX(a1);
  auto grid_z = WorldPosToGridCellZ(a2) - 1;

  auto grid_x1 = GridCellToWorldPosX(grid_x),
       grid_x2 = GridCellToWorldPosX(grid_x + 1);
  auto grid_z1 = GridCellToWorldPosZ(grid_z),
       grid_z2 = GridCellToWorldPosZ(grid_z + 1);

  auto y_x1z1 = pOutdoor->DoGetHeightOnTerrain(grid_x,     grid_z),
       y_x2z1 = pOutdoor->DoGetHeightOnTerrain(grid_x + 1, grid_z),
       y_x2z2 = pOutdoor->DoGetHeightOnTerrain(grid_x + 1, grid_z + 1),
       y_x1z2 = pOutdoor->DoGetHeightOnTerrain(grid_x,     grid_z + 1);
  //v4 = WorldPosToGridCellX(a1);
  //v5 = WorldPosToGridCellZ(v12) - 1;
  //dword_76D538_terrain_cell_world_pos_around_party_x = GridCellToWorldPosX(v4);
  //dword_76D53C_terrain_cell_world_pos_around_party_x = GridCellToWorldPosX(v4 + 1);
  //dword_76D540_terrain_cell_world_pos_around_party_x = GridCellToWorldPosX(v4 + 1);
  //dword_76D544_terrain_cell_world_pos_around_party_x = GridCellToWorldPosX(v4);
  //dword_76D528_terrain_cell_world_pos_around_party_z = GridCellToWorldPosZ(v5);
  //dword_76D52C_terrain_cell_world_pos_around_party_z = GridCellToWorldPosZ(v5);
  //dword_76D530_terrain_cell_world_pos_around_party_z = GridCellToWorldPosZ(v5 + 1);
  //dword_76D534_terrain_cell_world_pos_around_party_z = GridCellToWorldPosZ(v5 + 1);
  //dword_76D518_terrain_cell_world_pos_around_party_y = pOutdoor->DoGetHeightOnTerrain(v4, v5);
  //dword_76D51C_terrain_cell_world_pos_around_party_y = pOutdoor->DoGetHeightOnTerrain(v4 + 1, v5);
  //dword_76D520_terrain_cell_world_pos_around_party_y = pOutdoor->DoGetHeightOnTerrain(v4 + 1, v5 + 1);
  //dword_76D524_terrain_cell_world_pos_around_party_y = pOutdoor->DoGetHeightOnTerrain(v4, v5 + 1);
  *pIsOnWater = false;
  if ( pOutdoor->ActuallyGetSomeOtherTileInfo(grid_x, grid_z) & 2 )
    *pIsOnWater = true;
  v14 = 0;
  if ( !bFloatAboveWater && *pIsOnWater )
    v14 = -60;
  if ( y_x1z1 != y_x2z1 ||
       y_x2z1 != y_x2z2 ||
       y_x2z2 != y_x1z2 )
  {
    if ( abs(grid_z1 - a2) >= abs(a1 - grid_x1) )
    {
      v8 = y_x1z2;
      v9 = y_x2z2;
      v10 = y_x1z1;
      v15 = a1 - grid_x1;
      v13 = a2 - grid_z2;
    }
    else
    {
      v8 = y_x2z1;
      v9 = y_x1z1;
      v10 = y_x2z2;
      v15 = grid_x2 - a1;
      v13 = grid_z1 - a2;
    }
    return v14 + v8 + fixpoint_mul(v13, (v10 - v8) * 128) + fixpoint_mul(v15, (v9 - v8) * 128);
  }
  else
    return y_x1z1;
}





//----- (00485F53) --------------------------------------------------------
void  sr_485F53(Vec2_int_ *v)
{
  ++v->y;
  if ( v->y > 1000 )
    v->y = 0;
}

//----- (0048607B) --------------------------------------------------------
void Polygon::Create_48607B(stru149 *a2)
{
  this->pTexture = 0;
  this->ptr_38 = a2;
}

//----- (00486089) --------------------------------------------------------
void Polygon::_normalize_v_18()
{
  double v2; // st7@1
  double v3; // st6@1
  double v5; // st5@1

  //v1 = this;
  v2 = (double)this->v_18.x;
  v3 = (double)this->v_18.y;
  v5 = (double)this->v_18.z;
  float len = sqrt(v5 * v5 + v3 * v3 + v2 * v2);
  if (fabsf(len) < 1e-6f)
  {
    v_18.x = 0;
    v_18.y = 0;
    v_18.z = 65536;
  }
  else
  {
    v_18.x = round(v2 / len * 65536.0);
    v_18.y = round(v3 / len * 65536.0);
    v_18.y = round(v5 / len * 65536.0);
  }
}

//----- (0048616B) --------------------------------------------------------
void stru149::_48616B_frustum_odm(int a2, int a3, int a4, int a5, int a6, int a7)
{
  int v7; // ebx@1
  //int v8; // esi@1
  int v9; // edi@1
  //int v10; // eax@1
  int v11; // edx@1
  //int v12; // esi@2
  //int v13; // eax@2
  //int v14; // ST10_4@3
  //int v15; // esi@3
  //int v16; // eax@5
  int v17; // ST0C_4@6
  //int v18; // eax@8
  int v19; // ST0C_4@9
  //int v20; // eax@10
  //int v21; // edx@10
  //int v22; // eax@10
  //int result; // eax@10
  int v24; // [sp+14h] [bp-14h]@1
  int v25; // [sp+18h] [bp-10h]@1
  //int v26; // [sp+1Ch] [bp-Ch]@1
  int v27; // [sp+24h] [bp-4h]@1
  //int v28; // [sp+30h] [bp+8h]@10
  //int v29; // [sp+3Ch] [bp+14h]@10

  v25 = pGame->pIndoorCameraD3D->int_cosine_x;
  v7 = pGame->pIndoorCameraD3D->int_sine_y;
  v27 = pGame->pIndoorCameraD3D->int_sine_x;
  //v8 = -pIndoorCamera->pos.y;
  v9 = pGame->pIndoorCameraD3D->int_cosine_y;
  //v26 = -pIndoorCamera->pos.z;
  v11 = pGame->pIndoorCameraD3D->int_cosine_y * -pGame->pIndoorCameraD3D->vPartyPos.x + pGame->pIndoorCameraD3D->int_sine_y * -pGame->pIndoorCameraD3D->vPartyPos.y;
  v24 = pGame->pIndoorCameraD3D->int_cosine_y * -pGame->pIndoorCameraD3D->vPartyPos.y - pGame->pIndoorCameraD3D->int_sine_y * -pGame->pIndoorCameraD3D->vPartyPos.x;
  if (pGame->pIndoorCameraD3D->sRotationX)
  {
    this->field_0_party_dir_x = fixpoint_mul(v11, pGame->pIndoorCameraD3D->int_cosine_x) +
                                fixpoint_mul((-pGame->pIndoorCameraD3D->vPartyPos.z) << 16, pGame->pIndoorCameraD3D->int_sine_x);
    this->field_4_party_dir_y = v24;
    this->field_8_party_dir_z = fixpoint_mul((-pGame->pIndoorCameraD3D->vPartyPos.z) << 16, v25) - fixpoint_mul(v11, v27);
  }
  else
  {
    this->field_0_party_dir_x = v11;
    this->field_4_party_dir_y = v24;
    this->field_8_party_dir_z = (-pGame->pIndoorCameraD3D->vPartyPos.z) << 16;
  }

  if (pGame->pIndoorCameraD3D->sRotationX)
  {
    v17 = fixpoint_mul(a2, v9) + fixpoint_mul(a3, v7);

    this->angle_from_north = fixpoint_mul(v17, v25) + fixpoint_mul(a4, v27);
    this->angle_from_west = fixpoint_mul(a3, v9) - fixpoint_mul(a2, v7);
    this->viewing_angle_from_west_east = fixpoint_mul(a4, v25) - fixpoint_mul(v17, v27);
  }
  else
  {
    this->angle_from_north = fixpoint_mul(a2, v9) + fixpoint_mul(a3, v7);
    this->angle_from_west = fixpoint_mul(a3, v9) - fixpoint_mul(a2, v7);
    this->viewing_angle_from_west_east = a4;
  }

  if (pGame->pIndoorCameraD3D->sRotationX)
  {
    v19 = fixpoint_mul(a5, v9) + fixpoint_mul(a6, v7);

    this->angle_from_east = fixpoint_mul(v19, v25) + fixpoint_mul(a7, v27);
    this->angle_from_south = fixpoint_mul(a6, v9) - fixpoint_mul(a5, v7);
    this->viewing_angle_from_north_south = fixpoint_mul(a7, v25) - fixpoint_mul(v19, v27);
  }
  else
  {
    this->angle_from_east = fixpoint_mul(a5, v9) + fixpoint_mul(a6, v7);
    this->angle_from_south = fixpoint_mul(a6, v9) - fixpoint_mul(a5, v7);
    this->viewing_angle_from_north_south = a7;
  }

  this->angle_from_east = -this->angle_from_east;
  this->angle_from_south = -this->angle_from_south;
  this->viewing_angle_from_north_south = -this->viewing_angle_from_north_south;

  this->field_24 = fixpoint_dot(this->angle_from_north,  this->field_0_party_dir_x,
                                this->angle_from_west, this->field_4_party_dir_y,
                                this->viewing_angle_from_west_east, this->field_8_party_dir_z);
  this->field_28 = fixpoint_dot(this->angle_from_east, this->field_0_party_dir_x,
                                this->angle_from_south, this->field_4_party_dir_y,
                                this->viewing_angle_from_north_south, this->field_8_party_dir_z);
}

//----- (0048694B) --------------------------------------------------------
void stru149::_48694B_frustum_sky()
{
  this->angle_from_east = -this->angle_from_east;
  this->angle_from_south = -this->angle_from_south;
  this->viewing_angle_from_north_south = -this->viewing_angle_from_north_south;

  this->field_24 = fixpoint_dot(this->angle_from_north, this->field_0_party_dir_x,
                                this->angle_from_west, this->field_4_party_dir_y,
                                this->viewing_angle_from_west_east, this->field_8_party_dir_z);
  this->field_28 = fixpoint_dot(this->angle_from_east, this->field_0_party_dir_x,
                                this->angle_from_south, this->field_4_party_dir_y,
                                this->viewing_angle_from_north_south, this->field_8_party_dir_z);
}

//----- (0044100D) --------------------------------------------------------
bool sub_44100D()
{
  return pCurrentScreen == SCREEN_NPC_DIALOGUE || pCurrentScreen == SCREEN_CHARACTERS ||
         pCurrentScreen > SCREEN_LOADGAME && pCurrentScreen <= SCREEN_E ||
         pCurrentScreen > SCREEN_VIDEO && pCurrentScreen <= SCREEN_INPUT_BLV || pCurrentScreen == SCREEN_CASTING;
}
// 4E28F8: using guessed type int pCurrentScreen;

//----- (00441A4E) --------------------------------------------------------
__int16 __fastcall sub_441A4E(int a1)//for blessing
{
  __int16 result; // ax@1
  int v2; // ebx@1
  char *v3; // esi@1
  int v4; // edi@4
  bool v5; // ecx@4
  SpriteFrame *pFrame; // eax@6
  //SpriteFrame *v7; // edi@6
  int v8; // eax@6
  unsigned __int16 v9; // ax@6
  RenderBillboardTransform_local0 v10; // [sp+Ch] [bp-5Ch]@1
  int v11; // [sp+5Ch] [bp-Ch]@6
  int v12; // [sp+60h] [bp-8h]@1
  int v13; // [sp+64h] [bp-4h]@6

  v10.uParentBillboardID = -1;
  v10.pTarget = pRenderer->pTargetSurface;
  v10.pTargetZ = pRenderer->pActiveZBuffer;
  v10.uTargetPitch = pRenderer->field_10;
  result = 0;
  v2 = a1;
  v10.uViewportX = 0;
  v10.uViewportY = 0;
  v10.uViewportZ = 639;
  v10.uViewportW = 479;
  v12 = 0;
  //v3 = (char *)&pOtherOverlayList->pOverlays[0].field_C;
  //do
  for ( uint i = 0; i < 50; ++i )
  {
    if ( pOtherOverlayList->pOverlays[i].field_6 > 0 )
    {
      result = pOtherOverlayList->pOverlays[i].field_0;
      if ( pOtherOverlayList->pOverlays[i].field_0 >= 300 )
      {
        //v4 = result;
        v5 = pOtherOverlayList->pOverlays[i].field_0 == v2 + 320 
           || pOtherOverlayList->pOverlays[i].field_0 == v2 + 330 
           || pOtherOverlayList->pOverlays[i].field_0 == v2 + 340 
           || pOtherOverlayList->pOverlays[i].field_0 == v2 + 350;
        pOtherOverlayList->pOverlays[i].field_0 = v2 + 310;
        if ( pOtherOverlayList->pOverlays[i].field_0 == v2 + 310 || v5 )
        {
          if ( !pOtherOverlayList->pOverlays[i].field_0 )
          {
            pFrame = pSpriteFrameTable->GetFrame(pOverlayList->pOverlays[pOtherOverlayList->pOverlays[i].field_2].uSpriteFramesetID,
                   pOtherOverlayList->pOverlays[i].field_4);
            //v7 = v6;
            v11 = pOtherOverlayList->pOverlays[i].field_E;
            //v13 = pFrame->scale;
            v13 = (unsigned __int64)(v11 * (signed __int64)pFrame->scale) >> 16;
            v10.uScreenSpaceX = pOtherOverlayList->pOverlays[i].field_8;
            v10.uScreenSpaceY = pOtherOverlayList->pOverlays[i].field_A;
            v10._screenspace_x_scaler_packedfloat = v13;
            v10._screenspace_y_scaler_packedfloat = v13;
            v10.pPalette = PaletteManager::Get_Dark_or_Red_LUT(pFrame->uPaletteIndex, 0, 1);
            v8 = pOtherOverlayList->pOverlays[i].field_2;
            v10.sZValue = 0;
            v10.uFlags = 0;
            v9 = pOverlayList->pOverlays[v8].uOverlayType;
            if ( !v9 || v9 == 2 )
              v10.uScreenSpaceY += pSprites_LOD->pSpriteHeaders[pFrame->pHwSpriteIDs[0]].uHeight >> 1;
            result = pSprites_LOD->pSpriteHeaders[pFrame->pHwSpriteIDs[0]]._4AD2D1(&v10, 0);
            ++v12;
            if ( v12 == 5 )
              break;
          }
        }
      }
    }
    //v3 += 20;
  }
  //while ( (signed int)v3 < (signed int)&pOverlayList->pOverlays );
  return result;
}

//----- (00443801) --------------------------------------------------------
void Initialize2DA()
{
 
  int i;
	char* test_string;
	unsigned char c;
	bool break_loop;
	unsigned int temp_str_len;
	char* tmp_pos;
	int decode_step;

	if ( p2DEventsTXT_Raw )
		free(p2DEventsTXT_Raw);
	p2DEventsTXT_Raw = NULL;
	p2DEventsTXT_Raw = (char *)pEvents_LOD->LoadRaw("2dEvents.txt", 0);
	strtok(p2DEventsTXT_Raw, "\r");
	strtok(NULL, "\r");

	for (i=0;i<525;++i)
		{
		test_string = strtok(NULL, "\r") + 1;
		break_loop = false;
		decode_step=0;
		do 
			{
			c = *(unsigned char*)test_string;
			temp_str_len = 0;
			while((c!='\t')&&(c>0))
				{
				++temp_str_len;
				c=test_string[temp_str_len];
				}		
			tmp_pos=test_string+temp_str_len;
			if (*tmp_pos == 0)
				break_loop = true;
			*tmp_pos = 0;
			if (temp_str_len)
				{
				switch (decode_step)
					{
				case 2:
					{
					if ( !_strnicmp(test_string, "wea", 3) )
						{
						p2DEvents[i].uType = BuildingType_WeaponShop;
						break;
						}
					if ( !_strnicmp(test_string, "arm", 3) )
						{
						p2DEvents[i].uType = BuildingType_ArmorShop;
						break;
						}
					if ( !_strnicmp(test_string, "mag", 3) )
						{
						p2DEvents[i].uType = BuildingType_MagicShop;
						break;
						}
					if ( !_strnicmp(test_string, "alc", 3) )
						{
						p2DEvents[i].uType = BuildingType_AlchemistShop;
						break;
						}
					if ( !_strnicmp(test_string, "sta", 3) )
						{
						p2DEvents[i].uType = BuildingType_Stables;
						break;
						}
					if ( !_strnicmp(test_string, "boa", 3) )
						{
						p2DEvents[i].uType = BuildingType_Boats;
						break;
						}
					if ( !_strnicmp(test_string, "tem", 3) )
						{
						p2DEvents[i].uType = BuildingType_Temple;
						break;
						}
					if ( !_strnicmp(test_string, "tra", 3) )
						{
						p2DEvents[i].uType = BuildingType_Training;
						break;
						}
					if ( !_strnicmp(test_string, "tow", 3) )
						{
						p2DEvents[i].uType = BuildingType_TownHall;
						break;
						}

					if ( !_strnicmp(test_string, "tav", 3) )
						{
						p2DEvents[i].uType = BuildingType_Tavern;
						break;
						}
					if ( !_strnicmp(test_string, "ban", 3) )
						{
						p2DEvents[i].uType = BuildingType_Bank;
						break;
						}
					if ( !_strnicmp(test_string, "fir", 3) )
						{
						p2DEvents[i].uType = BuildingType_FireGuild;
						break;
						}
					if ( !_strnicmp(test_string, "air", 3) )
						{
						p2DEvents[i].uType = BuildingType_AirGuild;
						break;
						}
					if ( !_strnicmp(test_string, "wat", 3) )
						{
						p2DEvents[i].uType = BuildingType_WaterGuild;
						break;
						}
					if ( !_strnicmp(test_string, "ear", 3) )
						{
						p2DEvents[i].uType = BuildingType_EarthGuild;
						break;
						}
					if ( !_strnicmp(test_string, "spi", 3) )
						{
						p2DEvents[i].uType = BuildingType_SpiritGuild;
						break;
						}
					if ( !_strnicmp(test_string, "min", 3) )
						{
						p2DEvents[i].uType = BuildingType_MindGuild;
						break;
						}
					if ( !_strnicmp(test_string, "bod", 3) )
						{
						p2DEvents[i].uType = BuildingType_BodyGuild;
						break;
						}
					if ( !_strnicmp(test_string, "lig", 3) )
						{
						p2DEvents[i].uType = BuildingType_LightGuild;
						break;
						}
					if ( !_strnicmp(test_string, "dar", 3) )
						{
						p2DEvents[i].uType = BuildingType_DarkGuild;
						break;
						}
					if ( !_strnicmp(test_string, "ele", 3) ) // "Element Guild" from mm6
						{
						p2DEvents[i].uType = BuildingType_ElementalGuild;
						break;
						}
					if ( !_strnicmp(test_string, "sel", 3) )
						{
						p2DEvents[i].uType = BuildingType_SelfGuild;
						break;
						}
					if ( !_strnicmp(test_string, "mir", 3) )
						{
						p2DEvents[i].uType = BuildingType_16;
						break;
						}
					if ( !_strnicmp(test_string, "mer", 3) ) // "Merc Guild" from mm6
						{
						p2DEvents[i].uType = BuildingType_TownHall;
						break;
						}
					p2DEvents[i].uType = BuildingType_18;
					}
					break;

				case 4:
					p2DEvents[i].uAnimationID = atoi(test_string);
					break;
				case 5:
					p2DEvents[i].pName = RemoveQuotes(test_string);
					break;
				case 6:
					p2DEvents[i].pProprieterName = RemoveQuotes(test_string);
					break;
				case 7:
					p2DEvents[i].pProprieterTitle = RemoveQuotes(test_string);
					break;
				case 8:
					p2DEvents[i].field_14 = atoi(test_string);
					break;
				case 9:
					p2DEvents[i]._state = atoi(test_string);
					break;
				case 10:
					p2DEvents[i]._rep = atoi(test_string);
					break;
				case 11:
					p2DEvents[i]._per = atoi(test_string);
					break;
				case 12:
					p2DEvents[i].fPriceMultiplier = atof(test_string);
					break;
				case 13:
					p2DEvents[i].flt_24 = atof(test_string);
					break;
				case 15:
					p2DEvents[i].field_1C = atoi(test_string);
					break;
				case 18:
					p2DEvents[i].uOpenTime = atoi(test_string);
					break;
				case 19:
					p2DEvents[i].uCloseTime = atoi(test_string);
					break;
				case 20:
					p2DEvents[i].uExitPicID = atoi(test_string);
					break;
				case 21:
					p2DEvents[i].uExitMapID = atoi(test_string);
					break;
				case 22:
					p2DEvents[i]._quest_related = atoi(test_string);
					break;
				case 23:
					p2DEvents[i].pEnterText = RemoveQuotes(test_string);
					break;
					}
				}
			++decode_step;
			test_string=tmp_pos+1;
			} while ((decode_step<24)&&!break_loop);
		}

}

//----- (00443E31) --------------------------------------------------------
void LoadLevel_InitializeLevelStr()
    {
  
  char Args[100]; 
  int string_num;
  int max_string_length;
  int current_string_length;
  int prev_string_offset;

  if (sizeof(pLevelStrOffsets) != 2000)
    Log::Warning(L"pLevelStrOffsets: deserialization warning");
  memset(pLevelStrOffsets.data(), 0, 2000);
  
  max_string_length = 0;
  string_num = 1;
  prev_string_offset = 0;
  pLevelStrOffsets[0]=0;
  for (uint i = 0; i < uLevelStrFileSize; ++i)
  {
      if ( !pLevelStr[i] )
      {
        pLevelStrOffsets[string_num] = i + 1;
		++string_num;
        if ( i - prev_string_offset > max_string_length )
          max_string_length = i - prev_string_offset;
        prev_string_offset = i;
      }
  }

  uLevelStrNumStrings = string_num - 1;
  if ( max_string_length > 800 )
    Error("MAX_EVENT_TEXT_LENGTH needs to be increased to %lu", max_string_length+1);

  if ( uLevelStrNumStrings > 0 )
  {
    for(uint i = 0; i <uLevelStrNumStrings ; ++i) 
    {
      if ( RemoveQuotes(&pLevelStr[pLevelStrOffsets[i]]) != &pLevelStr[pLevelStrOffsets[i]] )
        ++pLevelStrOffsets[i]; 
    }
  }
}

//----- (00443F95) --------------------------------------------------------
void  OnMapLeave()
{
 _evt_raw *test_event;
  if ( uLevelEVT_NumEvents > 0 )
  {
    for (uint i = 0; i < uLevelEVT_NumEvents; ++i)
    {
		test_event=(_evt_raw*)&pLevelEVT[pLevelEVT_Index[i].uEventOffsetInEVT];
      if ( test_event->_e_type == EVENT_OnMapLeave )
      {
        EventProcessor(pLevelEVT_Index[i].uEventID, 0, 1, pLevelEVT_Index[i].event_sequence_num);
      }
    }
  }
}

//----- (00443FDC) --------------------------------------------------------
void OnMapLoad()
{
	stru176 *v3; // esi@7
	__int16 v4; // cx@9
	__int16 v5; // di@9
	int v6; // eax@9
	signed __int64 v8; // qax@26
	unsigned int v9; // ecx@26
	signed __int64 v10; // qax@26
	signed __int64 v11; // qax@26
	int v12; // ST50_4@26
	signed __int64 v13; // qax@26
	signed __int64 v14; // qax@26
	int v15; // ST58_4@26
	signed __int64 v16; // qax@26
	int hours; // ebx@26
	unsigned __int64 v18; // [sp+Ch] [bp-44h]@12
	signed __int64 seconds; // [sp+14h] [bp-3Ch]@26
	unsigned __int64 v20; // [sp+1Ch] [bp-34h]@7
	int minutes; // [sp+2Ch] [bp-24h]@26
	int years; // [sp+34h] [bp-1Ch]@26
	int weeks; // [sp+38h] [bp-18h]@26
	int v26; // [sp+3Ch] [bp-14h]@15
	int days; // [sp+3Ch] [bp-14h]@26
	int months; // [sp+40h] [bp-10h]@26
	__int16 v29; // [sp+46h] [bp-Ah]@9
	__int16 v30; // [sp+48h] [bp-8h]@9
	__int16 v31; // [sp+4Ah] [bp-6h]@9
	__int16 v32; // [sp+4Ch] [bp-4h]@9
	__int16 v33; // [sp+4Eh] [bp-2h]@9

	for (uint i = 0; i < uLevelEVT_NumEvents; ++i)
	{
		auto pEvent = pLevelEVT_Index[i];

		auto _evt = (_evt_raw *)(&pLevelEVT[pEvent.uEventOffsetInEVT]);

		if (_evt->_e_type == EVENT_PlaySound)
		{
			pSoundList->LoadSound(EVT_DWORD(_evt->v5), 0);
		}
		else if (_evt->_e_type == EVENT_OnMapReload)
		{
			EventProcessor(pEvent.uEventID, 0, 0, pEvent.event_sequence_num);
		}
		else if (_evt->_e_type == EVENT_OnTimer || _evt->_e_type == EVENT_Initialize)
		{
			v3 = &array_5B5928_timers[dword_5B65C8_timers_count];
			v20 = pOutdoor->loc_time.uLastVisitDay;
			if (uCurrentlyLoadedLevelType == LEVEL_Indoor)
				v20 = pIndoor->stru1.uLastVisitDay;

			v3->timer_evt_type = _evt->_e_type;
			v3->timer_evt_ID = pEvent.uEventID;
			v3->timer_evt_seq_num = pEvent.event_sequence_num;

			v3->field_10 = _evt->v5;
			v3->field_12 = _evt->v6;
			v3->field_14 = _evt->v7;
			v3->field_16 = _evt->v8;
			v3->field_18 = _evt->v9;
			v3->field_1A = _evt->v10;


			v6 = ((unsigned short)_evt->v12 << 8) + _evt->v11;

			v3->time_left_to_fire = ((unsigned short)_evt->v12 << 8) + _evt->v11;
            v3->fire_interval = ((unsigned short)_evt->v12 << 8) + _evt->v11;
			if (v3->timer_evt_type  == EVENT_Initialize && !(short)v6 )
			{
				if ( v20 )
					v18 = pParty->uTimePlayed - v20;
				else
					v18 = 0i64;
				v26 = (signed int)(signed __int64)((double)(signed __int64)v18 * 0.234375) / 60 / 60 / 24;

				if ( v26 / 7 / 4 / 12 )
				{
					if ( v3->field_10 )
					{
						++dword_5B65C8_timers_count;
                        v3->next_fire_time = 0;
						continue;
					}
				}
				if (v26 / 7 / 4 != 0 && v3->field_12 != 0 ||
					v26 / 7 != 0 && v3->field_14 != 0 ||
					v26 != 0 || !v20)
				{
					++dword_5B65C8_timers_count;
					v3->next_fire_time = 0;
					continue;
				}
			}
			else
			{
				v8 = (signed __int64)((double)(signed __int64)pParty->uTimePlayed * 0.234375);
				v9 = v8;
				v10 = (signed int)v8 / 60;
				minutes = v10 % 60;
				v11 = (signed int)(v10 / 60);
				v12 = v11 % 24;
				v13 = (signed int)(v11 / 24);
				days = v13 % 7;
				v14 = (signed int)(v13 / 7);
				v15 = v14 % 4;
				v16 = (signed int)(v14 / 4);
				years = v16 / 12;

				auto _1 = (unsigned __int64)((double)pParty->uTimePlayed * 0.234375) >> 32;
				auto _2 = ((__int64)v9 << 32) | _1;

				seconds = _2 % 60;
				//v19 = (signed __int64)__PAIR__((unsigned __int64)(signed __int64)((double)(signed __int64)pParty->uTimePlayed * 0.234375) >> 32,
				//                        v9)
				//    % 60;

				hours = v12;
				weeks = v15;
				months = v16 % 12;
				if ( v3->field_10 )
				{
					++years;
				}
				else
				{
					if ( v3->field_12 )
					{
						++months;
					}
					else
					{
						if ( v3->field_14 )
						{
							++weeks;
						}
						else
						{
							++days;
							minutes = v3->field_18;
							hours = v3->field_16;
							seconds = v3->field_1A;
						}
					}
				}
                v3->next_fire_time = (signed __int64)((double)((seconds
					+ 60 * minutes
					+ 3600 * hours
					+ 0x93A80 * weeks
					+ 0x15180 * days
					+ 0x24EA00 * (months + 12i64 * years)) << 7)
					* 0.033333335);

				++dword_5B65C8_timers_count;
			}
		}
	}
}

//----- (00444360) --------------------------------------------------------
void Level_LoadEvtAndStr(const char *pLevelName)
{
  char pContainerName[120]; // [sp+8h] [bp-98h]@1

  sprintf(pContainerName, "%s.evt", pLevelName);
  uLevelEVT_Size = LoadEventsToBuffer(pContainerName, pLevelEVT.data(), 9216);

  sprintf(pContainerName, "%s.str", pLevelName);
  uLevelStrFileSize = LoadEventsToBuffer(pContainerName, pLevelStr.data(), 9216);
  if (uLevelStrFileSize)
    LoadLevel_InitializeLevelStr();
}

//----- (00444D80) --------------------------------------------------------
int GetTravelTime()
{
  signed int v0; // esi@1

  v0 = uDefaultTravelTime_ByFoot;
  if ( CheckHiredNPCSpeciality(Guide) )
    --v0;
  if ( CheckHiredNPCSpeciality(Tracker) )
    v0 -= 2;
  if ( CheckHiredNPCSpeciality(Pathfinder) )
    v0 -= 3;
  if ( CheckHiredNPCSpeciality(Explorer) )
    --v0;
  if ( v0 < 1 )
    v0 = 1;
  return v0;
}
// 6BD07C: using guessed type int uDefaultTravelTime_ByFoot;

//----- (004451A8) --------------------------------------------------------
void __fastcall sub_4451A8_press_any_key(int a1, int a2, int a4)
{
  int v3; // esi@1
  int v4; // edi@1

  v3 = a2;
  v4 = a1;
  if ( !pGUIWindow2 )
  {
    if ( pParty->uFlags & 2 )
      pGame->Draw();
    pAudioPlayer->StopChannels(-1, -1);
    pMiscTimer->Pause();
    pEventTimer->Pause();
    dword_5C3418 = v4;
    dword_5C341C = v3;
    _591094_decoration = activeLevelDecoration;
    pGUIWindow2 = GUIWindow::Create(0, 0, 640, 480, WINDOW_GreetingNPC, a4, 0);
    pGUIWindow2->CreateButton( 61, 424, 31, 40, 2, 94, UIMSG_SelectCharacter, 1, '1', "", 0);
    pGUIWindow2->CreateButton(177, 424, 31, 40, 2, 94, UIMSG_SelectCharacter, 2, '2', "", 0);
    pGUIWindow2->CreateButton(292, 424, 31, 40, 2, 94, UIMSG_SelectCharacter, 3, '3', "", 0);
    pGUIWindow2->CreateButton(407, 424, 31, 40, 2, 94, UIMSG_SelectCharacter, 4, '4', "", 0);
  }
}

//----- (004452BB) --------------------------------------------------------
void sub_4452BB()
{
  pGUIWindow2->Release();
  pGUIWindow2 = 0;
  activeLevelDecoration = _591094_decoration;
  EventProcessor(dword_5C3418, 0, 1, dword_5C341C);
  activeLevelDecoration = NULL;
  pEventTimer->Resume();
}

//----- (004465DF) --------------------------------------------------------
int sub_4465DF_check_season(int a1)
{
  unsigned int v1; // eax@1
  unsigned int v2; // edx@1
  int v3; // ecx@2
  int v4; // ecx@3
  char v6; // zf@9

  v1 = pParty->uCurrentMonth + 1;
  v2 = pParty->uDaysPlayed + 1;
  if ( a1 )
  {
    v3 = a1 - 1;
    if ( v3 )
    {
      v4 = v3 - 1;
      if ( v4 )
      {
        if ( v4 != 1 )
          return 0;
        if ( v1 != 12 )
        {
          if ( (signed int)v1 >= 1 && (signed int)v1 <= 2 )
            return 1;
          v6 = v1 == 3;
LABEL_24:
          if ( !v6 || (signed int)v2 >= 21 )
            return 0;
          return 1;
        }
      }
      else
      {
        if ( v1 != 9 )
        {
          if ( (signed int)v1 >= 10 && (signed int)v1 <= 11 )
            return 1;
          v6 = v1 == 12;
          goto LABEL_24;
        }
      }
    }
    else
    {
      if ( v1 != 6 )
      {
        if ( (signed int)v1 >= 7 && (signed int)v1 <= 8 )
          return 1;
        v6 = v1 == 9;
        goto LABEL_24;
      }
    }
  }
  else
  {
    if ( v1 != 3 )
    {
      if ( (signed int)v1 >= 4 && (signed int)v1 <= 5 )
        return 1;
      v6 = v1 == 6;
      goto LABEL_24;
    }
  }
  if ( (signed int)v2 >= 21 )
    return 1;
  return 0;
}

//----- (0044861E) --------------------------------------------------------
void __fastcall sub_44861E_set_texture(unsigned int uFaceCog, const char *pFilename)
{
  unsigned int v2; // eax@2
  unsigned __int16 v3; // di@2
  signed int v4; // esi@5
  BLVFace *v5; // eax@7
  unsigned int *v6; // eax@8
  char *v7; // eax@9
  int v8; // edi@16
  char *v9; // esi@16
  ODMFace *v10; // eax@17
  unsigned int *v11; // eax@19
  unsigned __int16 v12; // [sp+Ch] [bp-18h]@2
  const char *Str2; // [sp+10h] [bp-14h]@1
  unsigned int v14; // [sp+14h] [bp-10h]@1
  signed int v15; // [sp+18h] [bp-Ch]@14
  int v16; // [sp+1Ch] [bp-8h]@15
  signed int v17; // [sp+20h] [bp-4h]@4
  signed int v18; // [sp+20h] [bp-4h]@16

  Str2 = pFilename;
  v14 = uFaceCog;
  if ( uFaceCog )
  {
    v2 = pBitmaps_LOD->LoadTexture(pFilename);
    v3 = v2;
    v12 = v2;
    if ( v2 != -1 ? (int)&pBitmaps_LOD->pTextures[v2] : 0 )
    {
      pBitmaps_LOD->pTextures[v2].palette_id2 = pPaletteManager->LoadPalette(pBitmaps_LOD->pTextures[v2].palette_id1);

      if ( uCurrentlyLoadedLevelType == 1 )
      {
        v17 = 1;
        if ( (signed int)pIndoor->uNumFaceExtras > 1 )
        {
          v4 = 1;
          do
          {
            if ( pIndoor->pFaceExtras[v4].sCogNumber == v14 )
            {
              v5 = &pIndoor->pFaces[pIndoor->pFaceExtras[v4].field_C];
              if ( BYTE1(v5->uAttributes) & 0x40 )
              {
                pIndoor->pFaces[pIndoor->pFaceExtras[v4].field_C].uBitmapID = pTextureFrameTable->FindTextureByName(
                                                                              Str2);
                v6 = (unsigned int *)&pIndoor->pFaces[pIndoor->pFaceExtras[v4].field_C].uBitmapID;
                if ( *(_WORD *)v6 )
                {
                  pTextureFrameTable->LoadAnimationSequenceAndPalettes(*(_WORD *)v6);
                }
                else
                {
                  *(_WORD *)v6 = v3;
                  v7 = (char *)&pIndoor->pFaces[pIndoor->pFaceExtras[v4].field_C].uAttributes;
                  v7[1] &= 0xBFu;
                }
              }
              else
              {
                v5->uBitmapID = v3;
              }
            }
            ++v17;
            ++v4;
          }
          while ( v17 < (signed int)pIndoor->uNumFaceExtras );
        }
        pParty->uFlags |= 2u;
      }
      else
      {
        v15 = 0;
        if ( (signed int)pOutdoor->uNumBModels > 0 )
        {
          v16 = 0;
          do
          {
            v8 = 0;
            v9 = (char *)&pOutdoor->pBModels[v16].pVertices;
            v18 = 0;
            if ( (signed int)pOutdoor->pBModels[v16].uNumFaces > 0 )
            {
              do
              {
                v10 = (ODMFace *)(v8 + *((_DWORD *)v9 + 4));
                if ( v10->sCogNumber == v14 )
                {
                  if ( BYTE1(v10->uAttributes) & 0x40 )
                  {
                    *(_WORD *)(*((_DWORD *)v9 + 4) + v8 + 272) = pTextureFrameTable->FindTextureByName(
                                                                   Str2);
                    v11 = (unsigned int *)(*((_DWORD *)v9 + 4) + v8 + 272);
                    if ( *(_WORD *)v11 )
                    {
                      pTextureFrameTable->LoadAnimationSequenceAndPalettes(*(_WORD *)v11);
                    }
                    else
                    {
                      *(_WORD *)v11 = v12;
                      *(_BYTE *)(*((_DWORD *)v9 + 4) + v8 + 29) &= 0xBFu;
                    }
                  }
                  else
                  {
                    v10->uTextureID = v12;
                  }
                }
                ++v18;
                v8 += 308;
              }
              while ( v18 < *((_DWORD *)v9 + 2) );
            }
            ++v15;
            ++v16;
          }
          while ( v15 < (signed int)pOutdoor->uNumBModels );
        }
      }
      pParty->uFlags |= 2u;
    }
  }
}

//----- (0044882F) --------------------------------------------------------
void __fastcall SetDecorationSprite(uint16_t uCog, bool bHide, const char *pFileName)
{
  for (size_t i = 0; i < uNumLevelDecorations; i++)
  {
    if (pLevelDecorations[i].uCog == uCog)
    {
      if (pFileName && strcmp(pFileName, "0"))
      {
        pLevelDecorations[i].uDecorationDescID = pDecorationList->GetDecorIdByName(pFileName);
        pDecorationList->InitializeDecorationSprite(pLevelDecorations[i].uDecorationDescID);
      }

      if (bHide)
        pLevelDecorations[i].uFlags &= ~LEVEL_DECORATION_INVISIBLE;
      else
        pLevelDecorations[i].uFlags |= LEVEL_DECORATION_INVISIBLE;

      pParty->uFlags |= 2u;
    }
  }
}

//----- (0044892E) --------------------------------------------------------
void __fastcall sub_44892E_set_faces_bit(int sCogNumber, int bit, int on)
{
  signed int v3; // esi@2
  signed int v4; // ecx@4
  char *v5; // eax@6
  int v6; // edi@13
  char *v7; // esi@13
  signed int v8; // ecx@13
  ODMFace *v9; // eax@14
  int v10; // [sp+0h] [bp-Ch]@1
  signed int v11; // [sp+4h] [bp-8h]@11
  int v12; // [sp+8h] [bp-4h]@12

  v10 = sCogNumber;
  if ( sCogNumber )
  {
    v3 = 1;
    if ( uCurrentlyLoadedLevelType == 1 )
    {
      if ( (signed int)pIndoor->uNumFaceExtras > 1 )
      {
        v4 = 1;
        do
        {
          if ( pIndoor->pFaceExtras[v4].sCogNumber == v10 )
          {
            v5 = (char *)&pIndoor->pFaces[pIndoor->pFaceExtras[v4].field_C].uAttributes;
            if ( on )
              *(_DWORD *)v5 |= bit;
            else
              *(_DWORD *)v5 &= ~bit;
          }
          ++v3;
          ++v4;
        }
        while ( v3 < (signed int)pIndoor->uNumFaceExtras );
      }
      pParty->uFlags |= 2u;
    }
    else
    {
      v11 = 0;
      if ( (signed int)pOutdoor->uNumBModels > 0 )
      {
        v12 = 0;
        do
        {
          v6 = 0;
          v7 = (char *)&pOutdoor->pBModels[v12].pVertices;
          v8 = 0;
          if ( (signed int)pOutdoor->pBModels[v12].uNumFaces > 0 )
          {
            do
            {
              v9 = (ODMFace *)(v6 + *((_DWORD *)v7 + 4));
              if ( v9->sCogNumber == v10 )
              {
                if ( on )
                  v9->uAttributes |= bit;
                else
                  v9->uAttributes &= ~bit;
              }
              ++v8;
              v6 += 308;
            }
            while ( v8 < *((_DWORD *)v7 + 2) );
          }
          ++v11;
          ++v12;
        }
        while ( v11 < (signed int)pOutdoor->uNumBModels );
      }
    }
    pParty->uFlags |= 2u;
  }
}

//----- (00448A98) --------------------------------------------------------
void __fastcall ToggleActorGroupFlag(unsigned int uGroupID, unsigned int uFlag, unsigned int bToggle)
{
  unsigned int v3; // ebx@1
  unsigned int v4; // ebp@1
  signed int v5; // edi@1
  Actor *v6; // esi@4
  size_t v7; // ecx@10
  Actor *v8; // eax@11
  unsigned __int16 v9; // dx@14

  v4 = uGroupID;
  v5 = Standing;
  v3 = uFlag;
  if ( uGroupID )
  {
    if ( bToggle )
    {
      if ( (signed int)uNumActors > Standing )
      {
        v6 = pActors.data();//[0].uAttributes;
        do
        {
          if ( v6->uGroup == v4 )
          {
            v6->uAttributes |= v3;
            if ( v3 == 0x10000 )
            {
              v6->uAIState = Disabled;
              v6->UpdateAnimation();
            }
          }
          ++v5;
          ++v6;
        }
        while ( v5 < (signed int)uNumActors );
      }
    }
    else
    {
      v7 = uNumActors;
      if ( (signed int)uNumActors > Standing )
      {
        v8 = pActors.data();//[0].uAIState;
        do
        {
          if ( v8->uGroup == v4 )
          {
            if ( v3 == 0x10000 )
            {
              v9 = v8->uAIState;
              if ( v8->uAIState != Dead )
              {
                if ( v9 != 4 && v9 != 11 )
                  v8->uAIState = Standing;
              }
            }
			LODWORD(v8->uAttributes) &= ~v3;
          }
          ++v8;
          --v7;
        }
        while ( v7 );
      }
    }
  }
}

//----- (00448B45) --------------------------------------------------------
void  GameUI_StatusBar_UpdateTimedString(unsigned int bForceHide)
{
  if ( bForceHide || GameUI_Footer_TimeLeft && GetTickCount() >= GameUI_Footer_TimeLeft )
    GameUI_Footer_TimeLeft = 0;
}

//----- (00448B67) --------------------------------------------------------
void OnTimer(int __unused)
{
  if (pEventTimer->bPaused)
    return;
  
  if (__unused)
    _5773C0_unused = 0;
  
  auto v13 = (signed __int64)(pParty->uTimePlayed - _5773B8_event_timer) / 128;
  if (!v13)
    return;

  //uint _v2v3 = pParty->uTimePlayed;
  //v3 = HIDWORD(pParty->uTimePlayed);
  //v2 = LODWORD(pParty->uTimePlayed);

  _5773B8_event_timer = pParty->uTimePlayed;

  for (uint i = 0; i < dword_5B65C8_timers_count; ++i)
  {
        //v4 = (char *)&array_5B5928_timers[0].field_C;
    auto timer = array_5B5928_timers + i;
        //while ( 1 )
        //{
          //v5 = *(short *)v4;
    if (timer->time_left_to_fire)
    {
      if (v13 < timer->time_left_to_fire)
        timer->time_left_to_fire -= v13;
      else
      {
        timer->time_left_to_fire = timer->fire_interval;
        EventProcessor(timer->timer_evt_ID, 0, 1, timer->timer_evt_seq_num);
      }
    }
    else
    {
      if (timer->next_fire_time < pParty->uTimePlayed)
      {
        uint next_trigger_time = 1 * 60 * 60 * 24; // 1 day
        if (timer->field_10)
          next_trigger_time = 336 * 60 * 60 * 24; // 1 year
        else if (timer->field_12)
          next_trigger_time = 28 * 60 * 60 * 24; // 1 month
        else if (timer->field_14)
          next_trigger_time = 7 * 60 * 60 * 24; // 1 week

        timer->next_fire_time += (next_trigger_time * 128) / 3.0f;
        if (timer->next_fire_time < pParty->uTimePlayed) // make sure in wont fire several times in a row if big time interval has lapsed
          timer->next_fire_time = pParty->uTimePlayed;

        EventProcessor(timer->timer_evt_ID, 0, 1, timer->timer_evt_seq_num);
      }
    }
  }
}

//----- (00481D77) --------------------------------------------------------
void _outdoor_project(RenderVertexSoft *v)
{
  double v1; // st7@1
  double v2; // st7@1

  v1 = 1.0 / (v->vWorldViewPosition.x + 0.0000001);
  v->_rhw = v1;
  v2 = v1 * (double)pODMRenderParams->int_fov_rad;
  v->vWorldViewProjX = (double)pViewport->uScreenCenterX - v2 * v->vWorldViewPosition.y;
  v->vWorldViewProjY = (double)pViewport->uScreenCenterY - v2 * v->vWorldViewPosition.z;
}

//----- (00448CF4) --------------------------------------------------------
void __fastcall sub_448CF4_spawn_monsters(__int16 typeindex, __int16 level, int count, int x, int y, int z, int group, unsigned int uUniqueName)
{
  __int16 v8; // di@1
  __int16 v9; // si@1
  unsigned int v10; // eax@1
  size_t v11; // ebx@2
  signed int v12; // esi@2
  Actor *pActor;//Actor_uYawAngle_cr *v13; // ebx@3
  AIDirection a3; // [sp+Ch] [bp-50h]@2
  AIDirection v15; // [sp+28h] [bp-34h]@2
  //SpawnPointMM7 v16; // [sp+44h] [bp-18h]@1

  v8 = typeindex;
  v9 = level;

  SpawnPointMM7 v16; // [sp+44h] [bp-18h]@1
  //SpawnPointMM7::SpawnPointMM7(&v16);

  v16.vPosition.x = x;
  v16.vPosition.y = y;
  v16.vPosition.z = z;
  v16.uGroup = group;
  v16.uRadius = 32;
  v16.uKind = 3;
  v16.uIndex = v8 + 2 * v9 + v9;
  v10 = pMapStats->GetMapInfo(pCurrentMapName);
  if ( v10 )
  {
    v11 = uNumActors;
    SpawnEncounter(&pMapStats->pInfos[v10], &v16, 0, count, 0);
    memcpy(&v15, Actor::GetDirectionInfo(PID(OBJECT_Actor, v11), 4u, &a3, 1), sizeof(v15));
    v12 = v11;
    if ( (signed int)v11 < (signed int)uNumActors )
    {
      for ( pActor = &pActors[v11]; v12 < (signed int)uNumActors; ++pActor )
      {
        pActor->PrepareSprites(0);
        ++v12;
        pActor->uYawAngle = v15.uYawAngle;
        pActor->dword_000334_unique_name = uUniqueName;
      }
    }
  }
}


//----- (0044987B) --------------------------------------------------------
void sub_44987B(const char *pMapName, MapStartPoint start_point)
{
  const char *v2; // edi@1

  v2 = pMapName;
  pAudioPlayer->StopChannels(-1, -1);
  pGameLoadingUI_ProgressBar->Initialize(GUIProgressBar::TYPE_None);
  if ( _stricmp(pCurrentMapName, v2) )
    SaveGame(1, 0);

  uGameState = GAME_STATE_2;
  strcpy(pCurrentMapName, v2);
  uLevel_StartingPointType = start_point;
}
// 6BE35C: using guessed type int uLevel_StartingPointType;

//----- (004498D5) --------------------------------------------------------
void TeleportToStartingPoint(MapStartPoint point)
{
  //unsigned int v1; // ecx@2
  //int v2; // ecx@3
  //int v3; // ecx@4
  unsigned __int16 v4; // ax@11
  signed int v5; // ecx@12
  LevelDecoration *v6; // edx@13
  signed int v7; // ecx@17
  int v8; // eax@17
  int v9; // ecx@17
  const char *v10; // [sp-4h] [bp-84h]@6
  char pName[128]; // [sp+8h] [bp-78h]@11

  switch (point)
  {
    case MapStartPoint_Party: v10 = "Party Start"; break;
    case MapStartPoint_North: v10 = "North Start"; break;
    case MapStartPoint_South: v10 = "South Start"; break;
    case MapStartPoint_East: v10 = "East Start";  break;
    case MapStartPoint_West: v10 = "West Start";  break;
    default:
      Error("Invalid enum value: %u", point);
  }

  strcpy(pName, v10);
  v4 = pDecorationList->GetDecorIdByName(pName);
  if ( v4 )
  {
    v5 = 0;
    if ( (signed int)uNumLevelDecorations > 0 )
    {
      v6 = pLevelDecorations.data();
      while ( v6->uDecorationDescID != (signed __int16)v4 )
      {
        ++v5;
        ++v6;
        if ( v5 >= (signed int)uNumLevelDecorations )
          goto LABEL_20;
      }
      v7 = v5;
      pParty->vPosition.x = pLevelDecorations[v7].vPosition.x;
      pParty->vPosition.y = pLevelDecorations[v7].vPosition.y;
      pParty->vPosition.z = pLevelDecorations[v7].vPosition.z;
      pParty->uFallStartY = pParty->vPosition.z;
      v8 = (signed int)(stru_5C6E00->uIntegerHalfPi * pLevelDecorations[v7].field_1A) / 90;
      v9 = pLevelDecorations[v7].field_10_y_rot;
      pParty->sRotationY = v8;
      if ( v9 )
        pParty->sRotationY = v9;
      pParty->sRotationX = 0;
      pParty->uFallSpeed = 0;
    }
LABEL_20:
    if ( dword_5B65C0 )
    {
      if ( _5B65A8_npcdata_uflags_or_other )
        pParty->vPosition.x = _5B65A8_npcdata_uflags_or_other;
      if ( _5B65AC_npcdata_fame_or_other )
        pParty->vPosition.y = _5B65AC_npcdata_fame_or_other;
      if ( _5B65B0_npcdata_rep_or_other )
      {
        pParty->vPosition.z = _5B65B0_npcdata_rep_or_other;
        pParty->uFallStartY = _5B65B0_npcdata_rep_or_other;
      }
      if ( _5B65B4_npcdata_loword_house_or_other != -1 )
        pParty->sRotationY = _5B65B4_npcdata_loword_house_or_other;
      if ( _5B65B8_npcdata_hiword_house_or_other )
        pParty->sRotationX = _5B65B8_npcdata_hiword_house_or_other;
      if ( dword_5B65BC )
        pParty->uFallSpeed = dword_5B65BC;
    }
    _5B65B4_npcdata_loword_house_or_other = -1;
    dword_5B65C0 = 0;
    dword_5B65BC = 0;
    _5B65B8_npcdata_hiword_house_or_other = 0;
    _5B65B0_npcdata_rep_or_other = 0;
    _5B65AC_npcdata_fame_or_other = 0;
    _5B65A8_npcdata_uflags_or_other = 0;
  }
}



//----- (00449A49) --------------------------------------------------------
__int16 __fastcall sub_449A49_door_switch_animation(unsigned int uDoorID, int a2)
{
  int v2; // eax@1
  int v3; // ebx@1
  signed int v4; // esi@2
  int v5; // edx@2
  BLVDoor *v6; // ecx@8
  unsigned int v7; // edx@18
  signed int v8; // esi@19
  int v9; // eax@19
  //char Args; // [sp+Ch] [bp-78h]@6

  LOWORD(v2) = LOWORD(pIndoor->pDoors);
  v3 = a2;
  if ( !pIndoor->pDoors )
    return v2;
  v4 = 0;
  v5 = (int)&pIndoor->pDoors->uDoorID;
  do
  {
    if ( *(int *)v5 == uDoorID )
      break;
    ++v4;
    v5 += 80;
  }
  while ( v4 < 200 );
  if ( v4 >= 200 )
  {
    Error("Unable to find Door ID: %i!", uDoorID);
  }
  v6 = &pIndoor->pDoors[v4];
  if ( v3 == 2 )
  {
    LOWORD(v2) = v6->uState;
    if ( (short)v2 == 3 || (short)v2 == 1 )
      return v2;
    if ( (short)v2 )
      goto LABEL_14;
  }
  else
  {
    if ( !v3 )
    {
LABEL_14:
      LOWORD(v2) = v6->uState;
      if ( (short)v2 && (short)v2 != 3 )
      {
        v6->uState = BLVDoor::Closing;
        if ( (short)v2 == 2 )
        {
LABEL_17:
          v6->uTimeSinceTriggered = 0;
          return v2;
        }
        v7 = v6->uTimeSinceTriggered;
        v2 = 15360;
        if ( v7 != 15360 )
        {
          v8 = v6->uOpenSpeed;
          v9 = v6->uCloseSpeed;
LABEL_26:
          v2 = (v6->uMoveLength << 7) / v8 - ((signed int)(v7 * v9) / 128 << 7) / v8;
          goto LABEL_27;
        }
        goto LABEL_27;
      }
      return v2;
    }
    if ( v3 != 1 )
      return v2;
  }
  LOWORD(v2) = v6->uState;
  if ( (short)v2 != 2 && (short)v2 != 1 )
  {
    v6->uState = BLVDoor::Opening;
    if ( !(short)v2 )
      goto LABEL_17;
    v7 = v6->uTimeSinceTriggered;
    v2 = 15360;
    if ( v7 != 15360 )
    {
      v8 = v6->uCloseSpeed;
      v9 = v6->uOpenSpeed;
      goto LABEL_26;
    }
LABEL_27:
    v6->uTimeSinceTriggered = v2;
  }
  return v2;
}

//----- (00449B57) --------------------------------------------------------
bool _449B57_test_bit( unsigned __int8 *a1, __int16 a2 )
{
  return (a1[(a2 - 1) >> 3] & (0x80u >> (a2 - 1) % 8)) != 0;
}

//----- (00449B7E) --------------------------------------------------------
void _449B7E_toggle_bit(unsigned char *pArray, __int16 a2, unsigned __int16 bToggle)
{
  signed int v3; // esi@1
  unsigned char set_bit; // edx@1

  v3 = a2 - 1;
  set_bit = 0x80 >> v3 % 8;
  if ( bToggle )
    pArray[v3 / 8] |=  set_bit;
  else
    pArray[v3 / 8] &= ~set_bit;
}

//----- (0044C175) --------------------------------------------------------
void ShowStatusBarString( const char *pString, unsigned int uNumSeconds )
{
  strcpy(GameUI_Footer_TimedString.data(), pString);
  GameUI_Footer_TimeLeft = 1000 * uNumSeconds + GetTickCount();

  for (int i = pFontLucida->GetLineWidth(GameUI_Footer_TimedString.data());
        i > 450;
        i = pFontLucida->GetLineWidth(GameUI_Footer_TimedString.data()) )
    GameUI_Footer_TimedString[strlen(GameUI_Footer_TimedString.data()) - 1] = 0;
}

//----- (0044C1D0) --------------------------------------------------------
void ShowNothingHereStatus()
{
  if ( !GameUI_Footer_TimeLeft )
    ShowStatusBarString(pGlobalTXT_LocalizationStrings[521], 2);// Nothing here
}

//----- (0044C28B) --------------------------------------------------------
int const_2()
{
  return 2;
}

//----- (0044C28F) --------------------------------------------------------
bool TeleportToNWCDungeon()
{
  if (!_stricmp("nwc.blv", pCurrentMapName))
    return false;

  _5B65A8_npcdata_uflags_or_other = 0;
  _5B65AC_npcdata_fame_or_other = 0;
  _5B65B0_npcdata_rep_or_other = 0;
  _5B65B4_npcdata_loword_house_or_other = 0;
  _5B65B8_npcdata_hiword_house_or_other = 0;
  dword_5B65BC = 0;
  dword_5B65C0 = 0;

  pGameLoadingUI_ProgressBar->uType = GUIProgressBar::TYPE_Fullscreen;
  sub_44987B("nwc.blv", MapStartPoint_Party);
  pCurrentScreen = SCREEN_GAME;
  return true;
}

//----- (00401000) --------------------------------------------------------
void mm7__vector_constructor(void *a1, int objSize, int numObjs, int ( *constructor)(int))
{
  void *v4; // esi@2
  int v5; // edi@2

  if ( numObjs - 1 >= 0 )
  {
    v4 = a1;
    v5 = numObjs;
    do
    {
      constructor((int)v4);
      v4 = (char *)v4 + objSize;
      --v5;
    }
    while ( v5 );
  }
}

//----- (0040261D) --------------------------------------------------------
void stru298::Add(__int16 uID, __int16 a3, __int16 x, __int16 y, __int16 z, char a7, char a8)
{
  if (count < 100)
  {
    pIDs[count] = uID;
    pXs[count] = x;
    pYs[count] = y;
    pZs[count] = z;
    field_324[count] = a3;
    field_3EC[count] = a8;
    field_450[count++] = a7;
  }
}

//----- (00402CAE) --------------------------------------------------------
int stru193_math::Cos(int angle)
{
  int v2; // eax@1

  //a2: (angle - uIntegerHalfPi)    for  sin(angle)
  //    (angle)                     for  cos(angle)

  v2 = uDoublePiMask & angle;
  
  if ( v2 > uIntegerPi )
    v2 = uIntegerDoublePi - v2;
  if ( v2 >= uIntegerHalfPi )
    return -pCosTable[uIntegerPi - v2];
  else
    return pCosTable[v2];
}

