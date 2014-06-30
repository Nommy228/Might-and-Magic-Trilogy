#define _CRT_SECURE_NO_WARNINGS
#include "ErrorHandling.h"
#include "Weather.h"
#include "Sprites.h"
#include "stru6.h"
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
#include "Log.h"
#include "Actor.h"
#include "Events.h"
#include "Viewport.h"
#include "OurMath.h"
#include "SpriteObject.h"
#include "ObjectList.h"
#include "PaletteManager.h"
#include "DecorationList.h"
#include "Timer.h"
#include "TurnEngine.h"
#include "Events2D.h"
#include "MapsLongTimer.h"
#include "stru298.h"
#include "texts.h"
#include "Level/Decoration.h"
#include "mm7_unsorted_subs.h"
#include "mm7_data.h"
#include "SaveLoad.h"

#include "MM7.h"


//----- (0044100D) --------------------------------------------------------
bool _44100D_should_alter_right_panel()
{
  return pCurrentScreen == SCREEN_NPC_DIALOGUE || pCurrentScreen == SCREEN_CHARACTERS ||
         pCurrentScreen == SCREEN_HOUSE || pCurrentScreen == SCREEN_E ||
         pCurrentScreen == SCREEN_CHANGE_LOCATION || pCurrentScreen == SCREEN_INPUT_BLV || pCurrentScreen == SCREEN_CASTING;
}


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
  //int v10; // ecx@15
  int pFloor; // eax@16
  int v15; // eax@24
  int v16; // edx@25
  int v17; // eax@29
  unsigned int v18; // eax@33
  int v21; // eax@35
  int v22; // ecx@36
  int v23; // eax@40
  unsigned int v24; // eax@44
  int a3; // [sp+10h] [bp-48h]@28
  int v26; // [sp+14h] [bp-44h]@15
  int i; // [sp+18h] [bp-40h]@1
  int a10; // [sp+1Ch] [bp-3Ch]@1
  int v29; // [sp+20h] [bp-38h]@14
  int v32; // [sp+2Ch] [bp-2Ch]@15
  int pSectorsArray[10]; // [sp+30h] [bp-28h]@1

  pSector = &pIndoor->pSectors[stru_721530.uSectorID];
  i = 1;
  a10 = b1;
  pSectorsArray[0] = stru_721530.uSectorID;
  for ( v2 = 0; v2 < pSector->uNumPortals; ++v2 )
  {
    pFace = &pIndoor->pFaces[pSector->pPortals[v2]];
    if ( stru_721530.sMaxX <= pFace->pBounding.x2 && stru_721530.sMinX >= pFace->pBounding.x1
      && stru_721530.sMaxY <= pFace->pBounding.y2 && stru_721530.sMinY >= pFace->pBounding.y1
      && stru_721530.sMaxZ <= pFace->pBounding.z2 && stru_721530.sMinZ >= pFace->pBounding.z1
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
  for ( v29 = 0; v29 < i; v29++ )
  {
    pSector = &pIndoor->pSectors[pSectorsArray[v29]];
    v32 = pSector->uNumFloors + pSector->uNumWalls + pSector->uNumCeilings;
    for ( v26 = 0;  v26 < v32; v26++ )
    {
      pFloor = pSector->pFloors[v26];
      pFace = &pIndoor->pFaces[pSector->pFloors[v26]];
      if ( !pFace->Portal()
        && stru_721530.sMaxX <= pFace->pBounding.x2 && stru_721530.sMinX >= pFace->pBounding.x1
        && stru_721530.sMaxY <= pFace->pBounding.y2 && stru_721530.sMinY >= pFace->pBounding.y1
        && stru_721530.sMaxZ <= pFace->pBounding.z2 && stru_721530.sMinZ >= pFace->pBounding.z1
        && pFloor != stru_721530.field_84 )
      {
      v15 = ( pFace->pFacePlane_old.dist + stru_721530.normal.x * pFace->pFacePlane_old.vNormal.x
                                         + stru_721530.normal.y * pFace->pFacePlane_old.vNormal.y
                                         + stru_721530.normal.z * pFace->pFacePlane_old.vNormal.z ) >> 16;
      if ( v15 > 0 )
      {
        v16 = ( pFace->pFacePlane_old.dist + stru_721530.normal2.x * pFace->pFacePlane_old.vNormal.x
                                           + stru_721530.normal2.y * pFace->pFacePlane_old.vNormal.y
                                           + stru_721530.normal2.z * pFace->pFacePlane_old.vNormal.z ) >> 16;
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
        || (v21 = ( pFace->pFacePlane_old.dist + stru_721530.position.x * pFace->pFacePlane_old.vNormal.x
                                               + stru_721530.position.y * pFace->pFacePlane_old.vNormal.y
                                               + stru_721530.position.z * pFace->pFacePlane_old.vNormal.z) >> 16, v21 <= 0)
        || (v22 = ( pFace->pFacePlane_old.dist + stru_721530.field_4C * pFace->pFacePlane_old.vNormal.x
                                               + stru_721530.field_50 * pFace->pFacePlane_old.vNormal.y
                                               + stru_721530.field_54 * pFace->pFacePlane_old.vNormal.z) >> 16, v21 > stru_721530.prolly_normal_d)
        && v22 > stru_721530.prolly_normal_d || v22 > v21 )
        continue;
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
      }
    }
    result = v29 + 1;
  }
  return result;
}
// 46E44E: using guessed type int var_28[10];

//----- (0046E889) --------------------------------------------------------
int __fastcall _46E889_collide_against_bmodels(unsigned int ecx0)
{
  int result; // eax@1
  //int v3; // ebx@9
  int v8; // eax@19
  int v9; // ecx@20
  int v10; // eax@24
  unsigned int v14; // eax@28
  int v15; // eax@30
  int v16; // ecx@31
  unsigned int v17; // eax@36
  int v21; // eax@42
  unsigned int v22; // eax@43
  //int a11; // [sp+70h] [bp-18h]@1
  //int a10; // [sp+80h] [bp-8h]@1
  int a2; // [sp+84h] [bp-4h]@23

  //a11 = ecx0;

  BLVFace face; // [sp+Ch] [bp-7Ch]@1

  result = 0;
  for ( uint i = 0; i < (signed int)pOutdoor->uNumBModels; ++i )
  {
    if ( stru_721530.sMaxX <= pOutdoor->pBModels[i].sMaxX && stru_721530.sMinX >= pOutdoor->pBModels[i].sMinX 
      && stru_721530.sMaxY <= pOutdoor->pBModels[i].sMaxY && stru_721530.sMinY >= pOutdoor->pBModels[i].sMinY
      && stru_721530.sMaxZ <= pOutdoor->pBModels[i].sMaxZ && stru_721530.sMinZ >= pOutdoor->pBModels[i].sMinZ )
    {
      for ( uint j = 0; j < pOutdoor->pBModels[i].uNumFaces; ++j )
      {
        if ( stru_721530.sMaxX <= pOutdoor->pBModels[i].pFaces[j].pBoundingBox.x2 && stru_721530.sMinX >= pOutdoor->pBModels[i].pFaces[j].pBoundingBox.x1
          && stru_721530.sMaxY <= pOutdoor->pBModels[i].pFaces[j].pBoundingBox.y2 && stru_721530.sMinY >= pOutdoor->pBModels[i].pFaces[j].pBoundingBox.y1
          && stru_721530.sMaxZ <= pOutdoor->pBModels[i].pFaces[j].pBoundingBox.z2 && stru_721530.sMinZ >= pOutdoor->pBModels[i].pFaces[j].pBoundingBox.z1 )
        {
          face.pFacePlane_old.vNormal.x = pOutdoor->pBModels[i].pFaces[j].pFacePlane.vNormal.x;
          face.pFacePlane_old.vNormal.y = pOutdoor->pBModels[i].pFaces[j].pFacePlane.vNormal.y;
          face.pFacePlane_old.vNormal.z = pOutdoor->pBModels[i].pFaces[j].pFacePlane.vNormal.z;

          face.pFacePlane_old.dist = pOutdoor->pBModels[i].pFaces[j].pFacePlane.dist; //incorrect

          face.uAttributes = pOutdoor->pBModels[i].pFaces[j].uAttributes;

          face.pBounding.x1 = pOutdoor->pBModels[i].pFaces[j].pBoundingBox.x1;
          face.pBounding.y1 = pOutdoor->pBModels[i].pFaces[j].pBoundingBox.y1;
          face.pBounding.z1 = pOutdoor->pBModels[i].pFaces[j].pBoundingBox.z1;

          face.pBounding.x2 = pOutdoor->pBModels[i].pFaces[j].pBoundingBox.x2;
          face.pBounding.y2 = pOutdoor->pBModels[i].pFaces[j].pBoundingBox.y2;
          face.pBounding.z2 = pOutdoor->pBModels[i].pFaces[j].pBoundingBox.z2;

          face.zCalc1 = pOutdoor->pBModels[i].pFaces[j].zCalc1;
          face.zCalc2 = pOutdoor->pBModels[i].pFaces[j].zCalc2;
          face.zCalc3 = pOutdoor->pBModels[i].pFaces[j].zCalc3;

          face.pXInterceptDisplacements = pOutdoor->pBModels[i].pFaces[j].pXInterceptDisplacements;
          face.pYInterceptDisplacements = pOutdoor->pBModels[i].pFaces[j].pYInterceptDisplacements;
          face.pZInterceptDisplacements = pOutdoor->pBModels[i].pFaces[j].pZInterceptDisplacements;

          face.uPolygonType = (PolygonType)pOutdoor->pBModels[i].pFaces[j].uPolygonType;

          face.uNumVertices = pOutdoor->pBModels[i].pFaces[j].uNumVertices;

          face.uBitmapID = pOutdoor->pBModels[i].pFaces[j].uTextureID;

          face.pVertexIDs = pOutdoor->pBModels[i].pFaces[j].pVertexIDs;

          if ( !face.Ethereal() && !face.Portal() )
          {
            v8 = (face.pFacePlane_old.dist + face.pFacePlane_old.vNormal.x * stru_721530.normal.x
                                        + face.pFacePlane_old.vNormal.y * stru_721530.normal.y
                                        + face.pFacePlane_old.vNormal.z * stru_721530.normal.z ) >> 16;
            if ( v8 > 0 )
            {
              v9 = (face.pFacePlane_old.dist + face.pFacePlane_old.vNormal.x * stru_721530.normal2.x
                                          + face.pFacePlane_old.vNormal.y * stru_721530.normal2.y
                                          + face.pFacePlane_old.vNormal.z * stru_721530.normal2.z) >> 16;
              if ( v8 <= stru_721530.prolly_normal_d || v9 <= stru_721530.prolly_normal_d )
              {
                if ( v9 <= v8 )
                {
                  a2 = stru_721530.field_6C;
                  if ( sub_4754BF(stru_721530.prolly_normal_d, &a2, stru_721530.normal.x, stru_721530.normal.y, stru_721530.normal.z,
                            stru_721530.direction.x, stru_721530.direction.y, stru_721530.direction.z, &face, i, ecx0) )
                  {
                    v10 = a2;
                  }
                  else
                  {
                    a2 = stru_721530.prolly_normal_d + stru_721530.field_6C;
                    if ( !sub_475F30( &a2, &face, stru_721530.normal.x, stru_721530.normal.y, stru_721530.normal.z,
                             stru_721530.direction.x, stru_721530.direction.y, stru_721530.direction.z, i) )
                      goto LABEL_29;
                    v10 = a2 - stru_721530.prolly_normal_d;
                    a2 -= stru_721530.prolly_normal_d;
                  }
                  if ( v10 < stru_721530.field_7C )
                  {
                    stru_721530.field_7C = v10;
                    v14 = 8 * (j | (i << 6));
                    LOBYTE(v14) = v14 | 6;
                    stru_721530.uFaceID = v14;
                  }
                }
              }
            }
LABEL_29:
            if ( stru_721530.field_0 & 1 )
            {
              v15 = (face.pFacePlane_old.dist + face.pFacePlane_old.vNormal.x * stru_721530.position.x
                                           + face.pFacePlane_old.vNormal.y * stru_721530.position.y
                                           + face.pFacePlane_old.vNormal.z * stru_721530.position.z) >> 16;
              if ( v15 > 0 )
              {
                v16 = (face.pFacePlane_old.dist + face.pFacePlane_old.vNormal.x * stru_721530.field_4C
                                             + face.pFacePlane_old.vNormal.y * stru_721530.field_50
                                             + face.pFacePlane_old.vNormal.z * stru_721530.field_54) >> 16;
                if ( v15 <= stru_721530.prolly_normal_d || v16 <= stru_721530.prolly_normal_d )
                {
                  if ( v16 <= v15 )
                  {
                    a2 = stru_721530.field_6C;
                    if ( sub_4754BF(stru_721530.field_8_radius, &a2, stru_721530.position.x, stru_721530.position.y, stru_721530.position.z,
                             stru_721530.direction.x, stru_721530.direction.y, stru_721530.direction.z, &face, i, ecx0) )
                    {
                      if ( a2 < stru_721530.field_7C )
                      {
                        stru_721530.field_7C = a2;
                        v17 = 8 * (j | (i << 6));
                        LOBYTE(v17) = v17 | 6;
                        stru_721530.uFaceID = v17;
                      }
                    }
                    else
                    {
                      a2 = stru_721530.field_6C + stru_721530.field_8_radius;
                      if ( sub_475F30(&a2, &face, stru_721530.position.x, stru_721530.position.y, stru_721530.position.z,
                                     stru_721530.direction.x, stru_721530.direction.y, stru_721530.direction.z, i) )
                      {
                        v21 = a2 - stru_721530.prolly_normal_d;
                        a2 -= stru_721530.prolly_normal_d;
                        if ( a2 < stru_721530.field_7C )
                        {
                          stru_721530.field_7C = v21;
                          v22 = 8 * (j | (i << 6));
                          LOBYTE(v22) = v22 | 6;
                          stru_721530.uFaceID = v22;
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
    result = i;
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
  ObjectDesc *object; // edx@4
  int v10; // ecx@12
  int v11; // esi@13

  for ( uint i = 0; i < uNumSpriteObjects; ++i )
  {
    if ( pSpriteObjects[i].uObjectDescID )
    {
      object = &pObjectList->pObjects[pSpriteObjects[i].uObjectDescID];
      if ( !(object->uFlags & OBJECT_DESC_NO_COLLISION) )
      {
        if ( stru_721530.sMaxX <= pSpriteObjects[i].vPosition.x + object->uRadius && stru_721530.sMinX >= pSpriteObjects[i].vPosition.x - object->uRadius
          && stru_721530.sMaxY <= pSpriteObjects[i].vPosition.y + object->uRadius && stru_721530.sMinY >= pSpriteObjects[i].vPosition.y - object->uRadius
          && stru_721530.sMaxZ <= pSpriteObjects[i].vPosition.z + object->uHeight && stru_721530.sMinZ >= pSpriteObjects[i].vPosition.z )
        {
          if ( abs(((pSpriteObjects[i].vPosition.x - stru_721530.normal.x) * stru_721530.direction.y
                  - (pSpriteObjects[i].vPosition.y - stru_721530.normal.y) * stru_721530.direction.x) >> 16)
                  <= object->uHeight + stru_721530.prolly_normal_d )
          {
            v10 = ((pSpriteObjects[i].vPosition.x - stru_721530.normal.x) * stru_721530.direction.x
                 + (pSpriteObjects[i].vPosition.y - stru_721530.normal.y) * stru_721530.direction.y) >> 16;
            if ( v10 > 0 )
            {
              v11 = stru_721530.normal.z + ((unsigned __int64)(stru_721530.direction.z * (signed __int64)v10) >> 16);
              if ( v11 >= pSpriteObjects[i].vPosition.z - stru_721530.prolly_normal_d )
              {
                if ( v11 <= object->uHeight + stru_721530.prolly_normal_d + pSpriteObjects[i].vPosition.z )
                {
                  if ( v10 < stru_721530.field_7C )
                    sub_46DEF2(_this, i);
                }
              }
            }
          }
        }
      }
    }
  }
}

//----- (0046EF01) --------------------------------------------------------
int _46EF01_collision_chech_player(int a1)
{
  int result; // eax@1
  int v3; // ebx@7
  int v4; // esi@7
  int v5; // edi@8
  int v6; // ecx@9
  int v7; // edi@12
  int v10; // [sp+14h] [bp-8h]@7
  int v11; // [sp+18h] [bp-4h]@7

  result = pParty->vPosition.x;
  //v9 = pParty->uPartyHeight;
  if ( stru_721530.sMaxX <= pParty->vPosition.x + (2 * pParty->field_14_radius) && stru_721530.sMinX >= pParty->vPosition.x - (2 * pParty->field_14_radius)
    && stru_721530.sMaxY <= pParty->vPosition.y + (2 * pParty->field_14_radius) && stru_721530.sMinY >= pParty->vPosition.y - (2 * pParty->field_14_radius)
    && stru_721530.sMaxZ <= pParty->vPosition.z + pParty->uPartyHeight && stru_721530.sMinZ >= pParty->vPosition.z )
  {
    v3 = stru_721530.prolly_normal_d + (2 * pParty->field_14_radius);
    v11 = pParty->vPosition.x - stru_721530.normal.x;
    v4 = ((pParty->vPosition.x - stru_721530.normal.x) * stru_721530.direction.y
        - (pParty->vPosition.y - stru_721530.normal.y) * stru_721530.direction.x) >> 16;
    v10 = pParty->vPosition.y - stru_721530.normal.y;
    result = abs(((pParty->vPosition.x - stru_721530.normal.x) * stru_721530.direction.y
                - (pParty->vPosition.y - stru_721530.normal.y) * stru_721530.direction.x) >> 16);
    if ( result <= stru_721530.prolly_normal_d + (2 * pParty->field_14_radius) )
    {
      result = v10 * stru_721530.direction.y;
      v5 = (v10 * stru_721530.direction.y + v11 * stru_721530.direction.x) >> 16;
      if ( v5 > 0 )
      {
        v6 = fixpoint_mul(stru_721530.direction.z, v5) + stru_721530.normal.z;
        result = pParty->vPosition.z;
        if ( v6 >= pParty->vPosition.z )
        {
          result = pParty->uPartyHeight + pParty->vPosition.z;
          if ( v6 <= (signed int)(pParty->uPartyHeight + pParty->vPosition.z) || a1 )
          {
            result = integer_sqrt(v3 * v3 - v4 * v4);
            v7 = v5 - integer_sqrt(v3 * v3 - v4 * v4);
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
  return result;
}

//----- (0046F04E) --------------------------------------------------------
int _46F04E_collide_against_portals()
{
  unsigned int v1; // eax@1
  BLVFace *face; // eax@3
  int v4; // ecx@9
  int v5; // edx@9
  signed int result; // eax@21
  unsigned int v10; // [sp+8h] [bp-Ch]@1
  int a3; // [sp+Ch] [bp-8h]@13
  int v12; // [sp+10h] [bp-4h]@15

  v1 = 0xFFFFFFu;
  v10 = 0xFFFFFFu;
  for ( uint i = 0; i < pIndoor->pSectors[stru_721530.uSectorID].uNumPortals; ++i )
  {
    if ( pIndoor->pSectors[stru_721530.uSectorID].pPortals[i] != stru_721530.field_80 )
    {
      face = &pIndoor->pFaces[pIndoor->pSectors[stru_721530.uSectorID].pPortals[i]];
      if ( stru_721530.sMaxX <= face->pBounding.x2 && stru_721530.sMinX >= face->pBounding.x1
        && stru_721530.sMaxY <= face->pBounding.y2 && stru_721530.sMinY >= face->pBounding.y1
        && stru_721530.sMaxZ <= face->pBounding.z2 && stru_721530.sMinZ >= face->pBounding.z1 )
      {
        v4 = (stru_721530.normal.x * face->pFacePlane_old.vNormal.x + face->pFacePlane_old.dist
            + stru_721530.normal.y * face->pFacePlane_old.vNormal.y
            + stru_721530.normal.z * face->pFacePlane_old.vNormal.z) >> 16;
        v5 = (stru_721530.normal2.z * face->pFacePlane_old.vNormal.z + face->pFacePlane_old.dist
            + stru_721530.normal2.x * face->pFacePlane_old.vNormal.x
            + stru_721530.normal2.y * face->pFacePlane_old.vNormal.y) >> 16;
        if ( (v4 < stru_721530.prolly_normal_d || v5 < stru_721530.prolly_normal_d)
          && (v4 > -stru_721530.prolly_normal_d || v5 > -stru_721530.prolly_normal_d)
          && (a3 = stru_721530.field_6C, sub_475D85(&stru_721530.normal, &stru_721530.direction, &a3, face))
          && a3 < (signed int)v10 )
        {
          v10 = a3;
          v12 = pIndoor->pSectors[stru_721530.uSectorID].pPortals[i];
        }
      }
    }
  }
  v1 = v10;
  if ( stru_721530.field_7C >= (signed int)v1 && (signed int)v1 <= stru_721530.field_6C )
  {
    stru_721530.field_80 = v12;
    if ( pIndoor->pFaces[v12].uSectorID == stru_721530.uSectorID )
      stru_721530.uSectorID = pIndoor->pFaces[v12].uBackSectorID;
    else
      stru_721530.uSectorID = pIndoor->pFaces[v12].uSectorID;
    stru_721530.field_7C = 268435455;
    result = 0;
  }
  else
    result = 1;
  return result;
}

//----- (0047050A) --------------------------------------------------------
int stru141_actor_collision_object::_47050A(int dt)
{
  int v7; // eax@1
  signed int result; // eax@4
  int v17; // eax@5
  int v18; // eax@7
  int v21; // eax@9
  int v22; // eax@11

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
    result = 1;
  return result;
}

//----- (004706C6) --------------------------------------------------------
void UpdateActors_ODM()
{
  int v3; // ebx@6
  int v5; // eax@10
  //int v6; // ecx@10
  signed int v8; // ebx@17
//  unsigned __int8 v10; // sf@17
//  unsigned __int16 v11; // ax@21
  __int16 v20; // ax@42
  int v25; // eax@45
  signed int v26; // ecx@50
  int v28; // eax@54
  signed int v29; // ebx@57
  signed int v30; // eax@57
  int v31; // edi@57
  signed int i; // ebx@57
  unsigned int v33; // ecx@58
  int v35; // edi@64
  int v36; // eax@64
  unsigned int v39; // edi@71
  ODMFace *face; // edi@75
  int v46; // ecx@82
  signed int v47; // ebx@85
  int v48; // edi@85
//  int v55; // eax@107
//  unsigned int v56; // edi@107
//  int v57; // ST10_4@107
  unsigned int v58; // edi@107
  unsigned int v59; // ebx@107
//  signed int v60; // eax@107
  int v61; // eax@124
  Vec3_int_ v62; // [sp+Ch] [bp-44h]@42
  int v63; // [sp+18h] [bp-38h]@64
  int v64; // [sp+1Ch] [bp-34h]@64
  bool v67; // [sp+28h] [bp-28h]@10
  unsigned int v69; // [sp+30h] [bp-20h]@6
  unsigned int v70; // [sp+34h] [bp-1Ch]@10
  int v71; // [sp+38h] [bp-18h]@62
  int uIsAboveFloor; // [sp+3Ch] [bp-14h]@10
  int v72b;
  int uIsFlying; // [sp+44h] [bp-Ch]@8
  unsigned int v75; // [sp+48h] [bp-8h]@1
  int uIsOnWater; // [sp+4Ch] [bp-4h]@10

  for (v75 = 0; v75 < uNumActors; ++v75)
  {
    if ( pActors[v75].uAIState == Removed
      || pActors[v75].uAIState == Disabled
      || pActors[v75].uAIState == Summoned
      || !pActors[v75].uMovementSpeed )
      continue;
    v3 = 0;
    v69 = 0;
    if ( MonsterStats::BelongsToSupertype(pActors[v75].pMonsterInfo.uID, MONSTER_SUPERTYPE_WATER_ELEMENTAL) )
      v3 = 1;
    pActors[v75].uSectorID = 0;
    uIsFlying = pActors[v75].pMonsterInfo.uFlying;
    if ( !pActors[v75].CanAct() )
      uIsFlying = 0;
    v70 = IsTerrainSlopeTooHigh(pActors[v75].vPosition.x, pActors[v75].vPosition.y);
    v5 = ODM_GetFloorLevel(pActors[v75].vPosition.x, pActors[v75].vPosition.y, pActors[v75].vPosition.z,
           pActors[v75].uActorHeight, &uIsOnWater, (int *)&v69, v3);
    //v6 = pActors[v75].vPosition.z;
    uIsAboveFloor = 0;
    v67 = v69 == 0;
    if ( pActors[v75].vPosition.z > v5 + 1 )
      uIsAboveFloor = 1;
    if ( pActors[v75].uAIState == Dead && uIsOnWater && !uIsAboveFloor )
    {
      pActors[v75].uAIState = Removed;
      continue;
    }
    if ( pActors[v75].uCurrentActionAnimation == ANIM_Walking )
    {
      v8 = pActors[v75].uMovementSpeed;
      if ( (signed __int64)pActors[v75].pActorBuffs[ACTOR_BUFF_SLOWED].uExpireTime > 0 )
        v8 = (signed __int64)((double)v8 * 0.5);
      if ( pActors[v75].uAIState == Fleeing || pActors[v75].uAIState == Pursuing )
        v8 *= 2;
      if ( pParty->bTurnBasedModeOn == true && pTurnEngine->turn_stage == TE_WAIT )
        v8 *= flt_6BE3AC_debug_recmod1_x_1_6;
      if ( v8 > 1000 )
        v8 = 1000;

      pActors[v75].vVelocity.x = fixpoint_mul(stru_5C6E00->Cos(pActors[v75].uYawAngle), v8);
      pActors[v75].vVelocity.y = fixpoint_mul(stru_5C6E00->Sin(pActors[v75].uYawAngle), v8);
      if ( uIsFlying )
      {
        pActors[v75].vVelocity.z = fixpoint_mul(stru_5C6E00->Sin(pActors[v75].uPitchAngle), v8);
      }
      //v7 = v68;
    }
    else
    {
      pActors[v75].vVelocity.x = fixpoint_mul(55000, pActors[v75].vVelocity.x);
      pActors[v75].vVelocity.y = fixpoint_mul(55000, pActors[v75].vVelocity.y);
      if ( uIsFlying )
        pActors[v75].vVelocity.z = fixpoint_mul(55000, pActors[v75].vVelocity.z);
    }
    if ( pActors[v75].vPosition.z < v5 )
    {
      pActors[v75].vPosition.z = v5;
      pActors[v75].vVelocity.z = uIsFlying != 0 ? 0x14 : 0;
    }
    //v17 = 0;
    if ( !uIsAboveFloor || uIsFlying )
    {
      if ( v70 && !uIsAboveFloor && v67 )
      {
        pActors[v75].vPosition.z = v5;
        ODM_GetTerrainNormalAt(pActors[v75].vPosition.x, pActors[v75].vPosition.y, &v62);
        v20 = GetGravityStrength();
        //v21 = v62.y;
        //v22 = v62.z;
        //v23 = v62.y * v0->vVelocity.y;
        pActors[v75].vVelocity.z += -8 * LOWORD(pEventTimer->uTimeElapsed) * v20;
        int v73 = abs(v62.x * pActors[v75].vVelocity.x + v62.z * pActors[v75].vVelocity.z + v62.y * pActors[v75].vVelocity.y) >> 16;
        //v72b = v21;
        pActors[v75].vVelocity.x += fixpoint_mul(v73, v62.x);
        pActors[v75].vVelocity.y += fixpoint_mul(v73, v62.y);
        pActors[v75].vVelocity.z += fixpoint_mul(v73, v62.z);
        //v17 = 0;
      }
    }
    else
    {
      pActors[v75].vVelocity.z -= LOWORD(pEventTimer->uTimeElapsed) * GetGravityStrength();
    }
    if ( pParty->armageddon_timer != 0 && pActors[v75].CanAct() )
    {
      pActors[v75].vVelocity.x += rand() % 100 - 50;
      pActors[v75].vVelocity.y += rand() % 100 - 50;
      pActors[v75].vVelocity.z += rand() % 100 - 20;
      v25 = rand();
      pActors[v75].uAIState = Stunned;
      pActors[v75].uYawAngle += v25 % 32 - 16;
      pActors[v75].UpdateAnimation();
    }
    if ( pActors[v75].vVelocity.x * pActors[v75].vVelocity.x + pActors[v75].vVelocity.y * pActors[v75].vVelocity.y < 400 && v70 == 0 )
    {
      pActors[v75].vVelocity.y = 0;
      pActors[v75].vVelocity.x = 0;
    }
    stru_721530.field_0 = 1;
    if ( !uIsFlying )
      v26 = 40;
    else
      v26 = pActors[v75].uActorRadius;

    stru_721530.field_84 = -1;
    stru_721530.field_8_radius = v26;
    stru_721530.prolly_normal_d = v26;
    stru_721530.height = pActors[v75].uActorHeight;
    stru_721530.field_70 = 0;

    for ( v69 = 0; v69 < 100; ++v69 )
    {
      stru_721530.position.x = pActors[v75].vPosition.x;
      stru_721530.normal.x = stru_721530.position.x;
      stru_721530.position.y = pActors[v75].vPosition.y;
      stru_721530.normal.y = stru_721530.position.y;
      v28 = pActors[v75].vPosition.z;
      stru_721530.normal.z = v28 + v26 + 1;
      stru_721530.position.z = v28 - v26 + stru_721530.height - 1;
      if ( stru_721530.position.z < stru_721530.normal.z )
        stru_721530.position.z = v28 + v26 + 1;
      stru_721530.velocity.x = pActors[v75].vVelocity.x;
      stru_721530.uSectorID = 0;
      stru_721530.velocity.y = pActors[v75].vVelocity.y;
      stru_721530.velocity.z = pActors[v75].vVelocity.z;
      if ( stru_721530._47050A(0) )
        break;
      _46E889_collide_against_bmodels(1);
      v29 = WorldPosToGridCellZ(pActors[v75].vPosition.y);
      v30 = WorldPosToGridCellX(pActors[v75].vPosition.x);
      _46E26D_collide_against_sprites(v30, v29);
      _46EF01_collision_chech_player(0);
      _46ED8A_collide_against_sprite_objects(PID(OBJECT_Actor,v75));
      v31 = 0;
      for ( i = 0; v31 < ai_arrays_size; ++v31 )
      {
        v33 = ai_near_actors_ids[v31];
        if ( v33 != v75 && Actor::_46DF1A_collide_against_actor(v33, 40) )
          ++i;
      }
      v71 = i > 1;
      if ( stru_721530.field_7C < stru_721530.field_6C )
        v70 = fixpoint_mul(stru_721530.field_7C, stru_721530.direction.z);
      //v34 = 0;
      v35 = stru_721530.normal2.z - stru_721530.prolly_normal_d - 1;
      v36 = ODM_GetFloorLevel(stru_721530.normal2.x, stru_721530.normal2.y,
              stru_721530.normal2.z - stru_721530.prolly_normal_d - 1,
              pActors[v75].uActorHeight, (int *)&v63, &v64, 0);
      if ( uIsOnWater )
      {
        if ( v35 < v36 + 60 )
        {
          if ( pActors[v75].uAIState == Dead || pActors[v75].uAIState == Dying || pActors[v75].uAIState == Removed
            || pActors[v75].uAIState == Disabled )
          {
            if ( v64 )
              v61 = v36 + 30;
            else
              v61 = v5 + 60;
            SpriteObject::sub_42F960_create_object(pActors[v75].vPosition.x, pActors[v75].vPosition.y, v61);
            pActors[v75].uAIState = Removed;
            return;
          }
        }
      }
      if ( stru_721530.field_7C >= stru_721530.field_6C )
      {
        pActors[v75].vPosition.x = LOWORD(stru_721530.normal2.x);
        pActors[v75].vPosition.y = LOWORD(stru_721530.normal2.y);
        pActors[v75].vPosition.z = LOWORD(stru_721530.normal2.z) - LOWORD(stru_721530.prolly_normal_d) - 1;
        break;
      }
      //v72b = fixpoint_mul(stru_721530.field_7C, stru_721530.field_58.x);
      pActors[v75].vPosition.x += fixpoint_mul(stru_721530.field_7C, stru_721530.direction.x);
      //v72b = (unsigned __int64)(stru_721530.field_7C * (signed __int64)stru_721530.field_58.y) >> 16;
      pActors[v75].vPosition.y += fixpoint_mul(stru_721530.field_7C, stru_721530.direction.y);
      //v72b = (unsigned __int64)(stru_721530.field_7C * (signed __int64)stru_721530.field_58.z) >> 16;
      pActors[v75].vPosition.z += fixpoint_mul(stru_721530.field_7C, stru_721530.direction.z);
      stru_721530.field_70 += stru_721530.field_7C;
      v39 = PID_ID(stru_721530.uFaceID);
      switch ( PID_TYPE(stru_721530.uFaceID) )
      {
        case OBJECT_Actor:
          if ( pTurnEngine->turn_stage != TE_ATTACK && pTurnEngine->turn_stage != TE_MOVEMENT || pParty->bTurnBasedModeOn != TE_WAIT )
          {
			//if(pParty->bTurnBasedModeOn == 1)
				//v34 = 0;
            if ( pActors[v75].pMonsterInfo.uHostilityType )
            {
              if ( v71 == 0 )
                Actor::AI_Flee(v75, stru_721530.uFaceID, 0, (AIDirection *)0);
              else
                Actor::AI_StandOrBored(v75, 4, 0, (AIDirection *)0);
            }
            else if ( v71 )
              Actor::AI_StandOrBored(v75, 4, 0, (AIDirection *)0);
            else if ( pActors[v39].pMonsterInfo.uHostilityType == MonsterInfo::Hostility_Friendly )
              Actor::AI_Flee(v75, stru_721530.uFaceID, 0, (AIDirection *)0);
            else
              Actor::AI_FaceObject(v75, stru_721530.uFaceID, 0, (AIDirection *)0);
          }
          break;
        case OBJECT_Player:
          if ( !pActors[v75].GetActorsRelation(0) )
          {
            Actor::AI_FaceObject(v75, stru_721530.uFaceID, 0, (AIDirection *)0);
            break;
          }
          //v52 = HIDWORD(pParty->pPartyBuffs[PARTY_BUFF_INVISIBILITY].uExpireTime) == 0;
          //v53 = SHIDWORD(pParty->pPartyBuffs[PARTY_BUFF_INVISIBILITY].uExpireTime) < 0;
          pActors[v75].vVelocity.y = 0;
          pActors[v75].vVelocity.x = 0;
          //if ( !v53 && (!(v53 | v52) || LODWORD(pParty->pPartyBuffs[PARTY_BUFF_INVISIBILITY].uExpireTime) > 0) )
          if ( (signed __int64)pParty->pPartyBuffs[PARTY_BUFF_INVISIBILITY].uExpireTime < 0)
            pParty->pPartyBuffs[PARTY_BUFF_INVISIBILITY].Reset();
          viewparams->bRedrawGameUI = 1;
          break;
        case OBJECT_Decoration:
          v47 = integer_sqrt(pActors[v75].vVelocity.x * pActors[v75].vVelocity.x + pActors[v75].vVelocity.y * pActors[v75].vVelocity.y);
          v48 = stru_5C6E00->Atan2( pActors[v75].vPosition.x - pLevelDecorations[v39].vPosition.x,
                  pActors[v75].vPosition.y - pLevelDecorations[v39].vPosition.y);
          //v49 = v48;
          pActors[v75].vVelocity.x = fixpoint_mul(stru_5C6E00->Cos(v48), v47);
          pActors[v75].vVelocity.y = fixpoint_mul(stru_5C6E00->Sin(v48), v47);
          break;
        case OBJECT_BModel:
          face = &pOutdoor->pBModels[stru_721530.uFaceID >> 9].pFaces[v39 & 0x3F];
          if ( !face->Ethereal() )
          {
            if ( face->uPolygonType == 3 )
            {
              pActors[v75].vVelocity.z = 0;
              pActors[v75].vPosition.z = LOWORD(pOutdoor->pBModels[stru_721530.uFaceID >> 9].pVertices.pVertices[face->pVertexIDs[0]].z) + 1;
              if ( pActors[v75].vVelocity.x * pActors[v75].vVelocity.x
                 + pActors[v75].vVelocity.y * pActors[v75].vVelocity.y < 400 )
              {
                pActors[v75].vVelocity.y = 0;
                pActors[v75].vVelocity.x = 0;
              }
            }
            else
            {
              v72b = abs(face->pFacePlane.vNormal.y * pActors[v75].vVelocity.y + face->pFacePlane.vNormal.z * pActors[v75].vVelocity.z
                                                                    + face->pFacePlane.vNormal.x * pActors[v75].vVelocity.x) >> 16;
              if ( (stru_721530.speed >> 3) > v72b )
                v72b = stru_721530.speed >> 3;

              pActors[v75].vVelocity.x += fixpoint_mul(v72b, face->pFacePlane.vNormal.x);
              pActors[v75].vVelocity.y += fixpoint_mul(v72b, face->pFacePlane.vNormal.y);
              pActors[v75].vVelocity.z += fixpoint_mul(v72b, face->pFacePlane.vNormal.z);
              if ( face->uPolygonType != 4 )
              {
                v46 = stru_721530.prolly_normal_d
                    - ((face->pFacePlane.dist
                      + face->pFacePlane.vNormal.x * pActors[v75].vPosition.x
                      + face->pFacePlane.vNormal.y * pActors[v75].vPosition.y
                      + face->pFacePlane.vNormal.z * pActors[v75].vPosition.z) >> 16);
                if ( v46 > 0 )
                {
                  pActors[v75].vPosition.x += fixpoint_mul(v46, face->pFacePlane.vNormal.x);
                  pActors[v75].vPosition.y += fixpoint_mul(v46, face->pFacePlane.vNormal.y);
                  pActors[v75].vPosition.z += fixpoint_mul(v46, face->pFacePlane.vNormal.z);
                }
                pActors[v75].uYawAngle = stru_5C6E00->Atan2(pActors[v75].vVelocity.x, pActors[v75].vVelocity.y);
              }
            }
          }
          break;
      }

      pActors[v75].vVelocity.x = fixpoint_mul(58500, pActors[v75].vVelocity.x);
      pActors[v75].vVelocity.y = fixpoint_mul(58500, pActors[v75].vVelocity.y);
      pActors[v75].vVelocity.z = fixpoint_mul(58500, pActors[v75].vVelocity.z);

      v26 = stru_721530.prolly_normal_d;
    }

    v58 = ((unsigned int)~pOutdoor->ActuallyGetSomeOtherTileInfo(WorldPosToGridCellX(pActors[v75].vPosition.x), WorldPosToGridCellZ(pActors[v75].vPosition.y) - 1) >> 1) & 1;
    v59 = ((unsigned int)~pOutdoor->ActuallyGetSomeOtherTileInfo(WorldPosToGridCellX(pActors[v75].vPosition.x), WorldPosToGridCellZ(pActors[v75].vPosition.y) - 1) >> 1) & 1;
    if ( WorldPosToGridCellX(pActors[v75].vPosition.x) == WorldPosToGridCellX(pActors[v75].vPosition.x) 
      && WorldPosToGridCellZ(pActors[v75].vPosition.y) == WorldPosToGridCellZ(pActors[v75].vPosition.y)
      && v58  || v67 != 0 )
    {
      if ( MonsterStats::BelongsToSupertype(pActors[v75].pMonsterInfo.uID, MONSTER_SUPERTYPE_WATER_ELEMENTAL) )
      {
        v58 = v58 == 0;
        v59 = v59 == 0;
      }
      if ( !uIsFlying && v58 && !v59 )
      {
        pActors[v75].vPosition.x = pActors[v75].vPosition.x;
        pActors[v75].vPosition.y = pActors[v75].vPosition.y;
        if ( pActors[v75].CanAct() )
        {
          pActors[v75].uYawAngle -= 32;
          pActors[v75].uCurrentActionTime = 0;
          pActors[v75].uCurrentActionLength = 128;
          pActors[v75].uAIState = Fleeing;
        }
      }
    }
  }
}

//----- (0047253E) --------------------------------------------------------
void UpdateObjects()
{
  ObjectDesc *object; // eax@5
  int v5; // ecx@6
  signed int v7; // eax@9
  signed int v11; // eax@17
  int v12; // edi@27
  int v18; // [sp+4h] [bp-10h]@27
  int v19; // [sp+8h] [bp-Ch]@27

  for (uint i = 0; i < uNumSpriteObjects; ++i)
  {
    if (pSpriteObjects[i].uAttributes & OBJECT_40)
      pSpriteObjects[i].uAttributes &= ~OBJECT_40;
    else
    {
      object = &pObjectList->pObjects[pSpriteObjects[i].uObjectDescID];
      if (pSpriteObjects[i].AttachedToActor())
      {
        v5 = PID_ID(pSpriteObjects[i].spell_target_pid);
        pSpriteObjects[i].vPosition.x = pActors[v5].vPosition.x;
        pSpriteObjects[i].vPosition.y = pActors[v5].vPosition.y;
        pSpriteObjects[i].vPosition.z = pActors[v5].vPosition.z + pActors[v5].uActorHeight;
        if ( !pSpriteObjects[i].uObjectDescID )
          continue;
        pSpriteObjects[i].uSpriteFrameID += pEventTimer->uTimeElapsed;
        if ( !(object->uFlags & OBJECT_DESC_TEMPORARY))
          continue;
        if ( pSpriteObjects[i].uSpriteFrameID >= 0 )
        {
          v7 = object->uLifetime;
          if (pSpriteObjects[i].uAttributes & ITEM_BROKEN)
             v7 = pSpriteObjects[i].field_20;
          if ( pSpriteObjects[i].uSpriteFrameID < v7 )
             continue;
        }
        SpriteObject::OnInteraction(i);
        continue;
      }
      if ( pSpriteObjects[i].uObjectDescID )
      {
        pSpriteObjects[i].uSpriteFrameID += pEventTimer->uTimeElapsed;
        if ( object->uFlags & OBJECT_DESC_TEMPORARY )
        {
          if ( pSpriteObjects[i].uSpriteFrameID < 0 )
          {
            SpriteObject::OnInteraction(i);
            continue;
          }
          v11 = object->uLifetime;
          if (pSpriteObjects[i].uAttributes & ITEM_BROKEN)
            v11 = pSpriteObjects[i].field_20;
        }
        if ( !(object->uFlags & OBJECT_DESC_TEMPORARY) || pSpriteObjects[i].uSpriteFrameID < v11 )
        {
          if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
            SpriteObject::UpdateObject_fn0_BLV(i);
          else
            SpriteObject::UpdateObject_fn0_ODM(i);
          if ( pParty->bTurnBasedModeOn != 1 || !(pSpriteObjects[i].uSectorID & 4) )
            continue;
          v12 = abs(pParty->vPosition.x - pSpriteObjects[i].vPosition.x);
          v18 = abs(pParty->vPosition.y - pSpriteObjects[i].vPosition.y);
          v19 = abs(pParty->vPosition.z - pSpriteObjects[i].vPosition.z);
          if ( int_get_vector_length(v12, v18, v19) <= 5120 )
            continue;
          SpriteObject::OnInteraction(i);
          continue;
        }
        if ( !(object->uFlags & OBJECT_DESC_INTERACTABLE) )
        {
          SpriteObject::OnInteraction(i);
          continue;
        }
        _46BFFA_check_object_intercept(i, PID(OBJECT_Item,i));
      }
    }
  }
}

//----- (0047272C) --------------------------------------------------------
int collide_against_floor_approximate(int x, int y, int z, unsigned int *pSectorID, unsigned int *pFaceID)
{
  int result; // eax@1

  *pSectorID = pIndoor->GetSector(x - 2, y, z + 40);
  result = collide_against_floor(x - 2, y, z + 40, pSectorID, pFaceID);
  if ( result == -30000 || !*pSectorID )
  {
    *pSectorID = pIndoor->GetSector(x + 2, y, z + 40);
    result = collide_against_floor(x + 2, y, z + 40, pSectorID, pFaceID);
    if ( result == -30000 || !*pSectorID )
    {
      *pSectorID = pIndoor->GetSector(x, y - 2, z + 40);
      result = collide_against_floor(x, y - 2, z + 40, pSectorID, pFaceID);
      if ( result == -30000 || !*pSectorID )
      {
        *pSectorID = pIndoor->GetSector(x, y + 2, z + 40);
        result = collide_against_floor(x, y + 2, z + 40, pSectorID, pFaceID);
        if ( result == -30000 || !*pSectorID )
        {
          *pSectorID = pIndoor->GetSector(x, y, z + 140);
          result = collide_against_floor(x, y, z + 140, pSectorID, pFaceID);
        }
      }
    }
  }
  return result;
}

//----- (0047531C) --------------------------------------------------------
bool sub_47531C(int a1, int *a2, int pos_x, int pos_y, int pos_z, int dir_x, int dir_y, int dir_z, BLVFace *face, int a10)
{
  int v11; // ST1C_4@3
  int v12; // edi@3
  int v13; // esi@3
  int v14; // edi@4
  signed __int64 v15; // qtt@6
  //__int16 v16; // si@7
  int a7a; // [sp+30h] [bp+18h]@7
  int a9b; // [sp+38h] [bp+20h]@3
  int a9a; // [sp+38h] [bp+20h]@3
  int a10b; // [sp+3Ch] [bp+24h]@3
  signed int a10a; // [sp+3Ch] [bp+24h]@4
  int a10c; // [sp+3Ch] [bp+24h]@5

  if ( a10 && face->Ethereal() )
    return 0;
  v11 = fixpoint_mul(dir_x, face->pFacePlane_old.vNormal.x);
  a10b = fixpoint_mul(dir_y, face->pFacePlane_old.vNormal.y);
  a9b = fixpoint_mul(dir_z, face->pFacePlane_old.vNormal.z);
  v12 = v11 + a9b + a10b;
  a9a = v11 + a9b + a10b;
  v13 = (a1 << 16)
      - pos_x * face->pFacePlane_old.vNormal.x
      - pos_y * face->pFacePlane_old.vNormal.y
      - pos_z * face->pFacePlane_old.vNormal.z
      - face->pFacePlane_old.dist;
  if ( abs((a1 << 16)
         - pos_x * face->pFacePlane_old.vNormal.x
         - pos_y * face->pFacePlane_old.vNormal.y
         - pos_z * face->pFacePlane_old.vNormal.z - face->pFacePlane_old.dist) >= a1 << 16 )
  {
    a10c = abs(v13) >> 14;
    if ( a10c > abs(v12) )
      return 0;
    LODWORD(v15) = v13 << 16;
    HIDWORD(v15) = v13 >> 16;
    v14 = a1;
    a10a = v15 / a9a;
  }
  else
  {
    a10a = 0;
    v14 = abs(v13) >> 16;
  }
  //v16 = pos_y + ((unsigned int)fixpoint_mul(a10a, dir_y) >> 16);
  LOWORD(a7a) = (short)pos_x + ((unsigned int)fixpoint_mul(a10a, dir_x) >> 16) - fixpoint_mul(v14, face->pFacePlane_old.vNormal.x);
  HIWORD(a7a) = pos_y + ((unsigned int)fixpoint_mul(a10a, dir_y) >> 16) - fixpoint_mul(v14, face->pFacePlane_old.vNormal.y);
  if ( !sub_475665(face, a7a, (short)pos_z + ((unsigned int)fixpoint_mul(a10a, dir_z) >> 16) - fixpoint_mul(v14, face->pFacePlane_old.vNormal.z)) )
    return 0;
  *a2 = a10a >> 16;
  if ( a10a >> 16 < 0 )
    *a2 = 0;
  return 1;
}

//----- (004754BF) --------------------------------------------------------
bool sub_4754BF(int a1, int *a2, int X, int Y, int Z, int dir_x, int dir_y, int dir_z, BLVFace *face, int a10, int a11)
{
  int v12; // ST1C_4@3
  int v13; // edi@3
  int v14; // esi@3
  int v15; // edi@4
  signed __int64 v16; // qtt@6
  //__int16 v17; // si@7
  int a7a; // [sp+30h] [bp+18h]@7
  int a1b; // [sp+38h] [bp+20h]@3
  int a1a; // [sp+38h] [bp+20h]@3
  int a11b; // [sp+40h] [bp+28h]@3
  signed int a11a; // [sp+40h] [bp+28h]@4
  int a11c; // [sp+40h] [bp+28h]@5

  if ( a11 && face->Ethereal() )
    return false;
  v12 = fixpoint_mul(dir_x, face->pFacePlane_old.vNormal.x);
  a11b = fixpoint_mul(dir_y, face->pFacePlane_old.vNormal.y);
  a1b = fixpoint_mul(dir_z, face->pFacePlane_old.vNormal.z);
  v13 = v12 + a1b + a11b;
  a1a = v12 + a1b + a11b;
  v14 = (a1 << 16)
      - X * face->pFacePlane_old.vNormal.x
      - Y * face->pFacePlane_old.vNormal.y
      - Z * face->pFacePlane_old.vNormal.z
      - face->pFacePlane_old.dist;
  if ( abs((a1 << 16)
         - X * face->pFacePlane_old.vNormal.x
         - Y * face->pFacePlane_old.vNormal.y
         - Z * face->pFacePlane_old.vNormal.z - face->pFacePlane_old.dist) >= a1 << 16 )
  {
    a11c = abs(v14) >> 14;
    if ( a11c > abs(v13) )
      return false;
    LODWORD(v16) = v14 << 16;
    HIDWORD(v16) = v14 >> 16;
    v15 = a1;
    a11a = v16 / a1a;
  }
  else
  {
    a11a = 0;
    v15 = abs(v14) >> 16;
  }
  //v17 = Y + ((unsigned int)fixpoint_mul(a11a, dir_y) >> 16);
  LOWORD(a7a) = (short)X + ((unsigned int)fixpoint_mul(a11a, dir_x) >> 16) - fixpoint_mul(v15, face->pFacePlane_old.vNormal.x);
  HIWORD(a7a) = Y + ((unsigned int)fixpoint_mul(a11a, dir_y) >> 16) - fixpoint_mul(v15, face->pFacePlane_old.vNormal.y);
  if ( !sub_4759C9(face, a10, a7a, (short)Z + ((unsigned int)fixpoint_mul(a11a, dir_z) >> 16) - fixpoint_mul(v15, face->pFacePlane_old.vNormal.z)) )
    return false;
  *a2 = a11a >> 16;
  if ( a11a >> 16 < 0 )
    *a2 = 0;
  return true;
}

//----- (00475665) --------------------------------------------------------
int sub_475665(BLVFace *face, int a2, __int16 a3)
{
  bool v16; // edi@14
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

  if ( face->uAttributes & FACE_XY_PLANE )
  {
    v26 = (signed __int16)a2;
    v27 = SHIWORD(a2);
    if ( face->uNumVertices )
    {
      for ( v28 = 0; v28 < face->uNumVertices; v28++ )
      {
        word_720C10_intercepts_xs[2 * v28] = face->pXInterceptDisplacements[v28] + pIndoor->pVertices[face->pVertexIDs[v28]].x;
        word_720B40_intercepts_zs[2 * v28] = face->pYInterceptDisplacements[v28] + pIndoor->pVertices[face->pVertexIDs[v28]].y;
        word_720C10_intercepts_xs[2 * v28 + 1] = face->pXInterceptDisplacements[v28 + 1] + pIndoor->pVertices[face->pVertexIDs[v28 + 1]].x;
        word_720B40_intercepts_zs[2 * v28 + 1] = face->pYInterceptDisplacements[v28 + 1] + pIndoor->pVertices[face->pVertexIDs[v28 + 1]].y;
      }
    }
  }
  else
  {
    if ( face->uAttributes & FACE_XZ_PLANE )
    {
      v26 = (signed __int16)a2;
      v27 = a3;
      if ( face->uNumVertices )
      {
        for ( v29 = 0; v29 < face->uNumVertices; v29++ )
        {
          word_720C10_intercepts_xs[2 * v29] = face->pXInterceptDisplacements[v29] + pIndoor->pVertices[face->pVertexIDs[v29]].x;
          word_720B40_intercepts_zs[2 * v29] = face->pZInterceptDisplacements[v29] + pIndoor->pVertices[face->pVertexIDs[v29]].z;
          word_720C10_intercepts_xs[2 * v29 + 1] = face->pXInterceptDisplacements[v29 + 1] + pIndoor->pVertices[face->pVertexIDs[v29 + 1]].x;
          word_720B40_intercepts_zs[2 * v29 + 1] = face->pZInterceptDisplacements[v29 + 1] + pIndoor->pVertices[face->pVertexIDs[v29 + 1]].z;
        }
      }
    }
    else
    {
      v26 = SHIWORD(a2);
      v27 = a3;
      if ( face->uNumVertices )
      {
        for ( v30 = 0; v30 < face->uNumVertices; v30++ )
        {
          word_720C10_intercepts_xs[2 * v30] = face->pYInterceptDisplacements[v30] + pIndoor->pVertices[face->pVertexIDs[v30]].y;
          word_720B40_intercepts_zs[2 * v30] = face->pZInterceptDisplacements[v30] + pIndoor->pVertices[face->pVertexIDs[v30]].z;
          word_720C10_intercepts_xs[2 * v30 + 1] = face->pYInterceptDisplacements[v30 + 1] + pIndoor->pVertices[face->pVertexIDs[v30 + 1]].y;
          word_720B40_intercepts_zs[2 * v30 + 1] = face->pZInterceptDisplacements[v30 + 1] + pIndoor->pVertices[face->pVertexIDs[v30 + 1]].z;
        }
      }
    }
  }
  v31 = 0;
  word_720C10_intercepts_xs[2 * face->uNumVertices] = word_720C10_intercepts_xs[0];
  word_720B40_intercepts_zs[2 * face->uNumVertices] = word_720B40_intercepts_zs[0];
  v16 = word_720B40_intercepts_zs[0] >= v27;
  if ( 2 * face->uNumVertices <= 0 )
    return 0;
  for ( v25 = 0; v25 < 2 * face->uNumVertices; ++v25 )
  {
    if ( v31 >= 2 )
      break;
    if ( v16 ^ (word_720B40_intercepts_zs[v25 + 1] >= v27) )
    {
      if ( word_720C10_intercepts_xs[v25 + 1] >= v26 )
        v20 = 0;
      else
        v20 = 2;
      v21 = v20 | (word_720C10_intercepts_xs[v25] < v26);
      if ( v21 != 3 )
      {
        v22 = word_720C10_intercepts_xs[v25 + 1] - word_720C10_intercepts_xs[v25];
        LODWORD(v23) = v22 << 16;
        HIDWORD(v23) = v22 >> 16;
        if ( !v21
          || (word_720C10_intercepts_xs[v25] + ((signed int)(((unsigned __int64)(v23
           / (word_720B40_intercepts_zs[v25 + 1] - word_720B40_intercepts_zs[v25])
           * ((v27 - (signed int)word_720B40_intercepts_zs[v25]) << 16)) >> 16)
               + 32768) >> 16) >= v26) )
          ++v31;
      }
    }
    v16 = word_720B40_intercepts_zs[v25 + 1] >= v27;
  }
  result = 1;
  if ( v31 != 1 )
    result = 0;
  return result;
}

//----- (004759C9) --------------------------------------------------------
bool __fastcall sub_4759C9(BLVFace *face, int a2, int a3, __int16 a4)
{
  bool v12; // edi@14
  signed int v16; // ebx@18
  int v17; // edi@20
  signed int v18; // ST14_4@22
  signed __int64 v19; // qtt@22
  bool result; // eax@25
  int v21; // [sp+14h] [bp-10h]@14
  signed int v22; // [sp+18h] [bp-Ch]@1
  int v23; // [sp+1Ch] [bp-8h]@2
  signed int v24; // [sp+20h] [bp-4h]@2
  signed int a4d; // [sp+30h] [bp+Ch]@14

  if ( face->uAttributes & FACE_XY_PLANE )
  {
    v23 = (signed __int16)a3;
    v24 = SHIWORD(a3);
    if ( face->uNumVertices )
    {
      for ( v22 = 0; v22 < face->uNumVertices; ++v22 )
      {
        word_720A70_intercepts_xs_plus_xs[2 * v22] = face->pXInterceptDisplacements[v22] + LOWORD(pOutdoor->pBModels[a2].pVertices.pVertices[face->pVertexIDs[v22]].x);
        word_7209A0_intercepts_ys_plus_ys[2 * v22] = face->pYInterceptDisplacements[v22] + LOWORD(pOutdoor->pBModels[a2].pVertices.pVertices[face->pVertexIDs[v22]].y);
        word_720A70_intercepts_xs_plus_xs[2 * v22 + 1] = face->pXInterceptDisplacements[v22 + 1] + LOWORD(pOutdoor->pBModels[a2].pVertices.pVertices[face->pVertexIDs[v22 + 1]].x);
        word_7209A0_intercepts_ys_plus_ys[2 * v22 + 1] = face->pYInterceptDisplacements[v22 + 1] + LOWORD(pOutdoor->pBModels[a2].pVertices.pVertices[face->pVertexIDs[v22 + 1]].y);
      }
    }
  }
  else
  {
    if ( face->uAttributes & FACE_XZ_PLANE )
    {
      v23 = (signed __int16)a3;
      v24 = a4;
      if ( face->uNumVertices )
      {
        for ( v22 = 0; v22 < face->uNumVertices; ++v22 )
        {
          word_720A70_intercepts_xs_plus_xs[2 * v22] = face->pXInterceptDisplacements[v22] + LOWORD(pOutdoor->pBModels[a2].pVertices.pVertices[face->pVertexIDs[v22]].x);
          word_7209A0_intercepts_ys_plus_ys[2 * v22] = face->pZInterceptDisplacements[v22] + LOWORD(pOutdoor->pBModels[a2].pVertices.pVertices[face->pVertexIDs[v22]].z);
          word_720A70_intercepts_xs_plus_xs[2 * v22 + 1] = face->pXInterceptDisplacements[v22 + 1] + LOWORD(pOutdoor->pBModels[a2].pVertices.pVertices[face->pVertexIDs[v22 + 1]].x);
          word_7209A0_intercepts_ys_plus_ys[2 * v22 + 1] = face->pZInterceptDisplacements[v22 + 1] + LOWORD(pOutdoor->pBModels[a2].pVertices.pVertices[face->pVertexIDs[v22 + 1]].z);
        }
      }
    }
    else
    {
      v23 = SHIWORD(a3);
      v24 = a4;
      if ( face->uNumVertices )
      {
        for ( v22 = 0; v22 < face->uNumVertices; ++v22 )
        {
          word_720A70_intercepts_xs_plus_xs[2 * v22] = face->pYInterceptDisplacements[v22] + LOWORD(pOutdoor->pBModels[a2].pVertices.pVertices[face->pVertexIDs[v22]].y);
          word_7209A0_intercepts_ys_plus_ys[2 * v22] = face->pZInterceptDisplacements[v22] + LOWORD(pOutdoor->pBModels[a2].pVertices.pVertices[face->pVertexIDs[v22]].z);
          word_720A70_intercepts_xs_plus_xs[2 * v22 + 1] = face->pYInterceptDisplacements[v22 + 1] + LOWORD(pOutdoor->pBModels[a2].pVertices.pVertices[face->pVertexIDs[v22 + 1]].y);
          word_7209A0_intercepts_ys_plus_ys[2 * v22 + 1] = face->pZInterceptDisplacements[v22 + 1] + LOWORD(pOutdoor->pBModels[a2].pVertices.pVertices[face->pVertexIDs[v22 + 1]].z);
        }
      }
    }
  }
  a4d = 0;
  word_720A70_intercepts_xs_plus_xs[2 * face->uNumVertices] = word_720A70_intercepts_xs_plus_xs[0];
  word_7209A0_intercepts_ys_plus_ys[2 * face->uNumVertices] = word_7209A0_intercepts_ys_plus_ys[0];
  v12 = word_7209A0_intercepts_ys_plus_ys[0] >= v24;
  if ( 2 * face->uNumVertices <= 0 )
    return 0;
  for ( v21 = 0; v21 < 2 * face->uNumVertices; ++v21 )
  {
    if ( a4d >= 2 )
      break;
    if ( v12 ^ (word_7209A0_intercepts_ys_plus_ys[v21 + 1] >= v24) )
    {
      if ( word_720A70_intercepts_xs_plus_xs[v21 + 1] >= v23 )
        v16 = 0;
      else
        v16 = 2;
      v17 = v16 | (word_720A70_intercepts_xs_plus_xs[v21] < v23);
      if ( v17 != 3 )
      {
        v18 = word_720A70_intercepts_xs_plus_xs[v21 + 1] - word_720A70_intercepts_xs_plus_xs[v21];
        LODWORD(v19) = v18 << 16;
        HIDWORD(v19) = v18 >> 16;
        if ( !v17
          || (word_720A70_intercepts_xs_plus_xs[v21] + ((signed int)(((unsigned __int64)(v19
              / (word_7209A0_intercepts_ys_plus_ys[v21 + 1] - word_7209A0_intercepts_ys_plus_ys[v21])
              * ((v24 - (signed int)word_7209A0_intercepts_ys_plus_ys[v21]) << 16)) >> 16)
              + 0x8000) >> 16) >= v23) )
          ++a4d;
      }
    }
    v12 = word_7209A0_intercepts_ys_plus_ys[v21 + 1] >= v24;
  }
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
//  int v16; // [sp+18h] [bp-Ch]@2
  int v17; // [sp+20h] [bp-4h]@10
  int a4b; // [sp+30h] [bp+Ch]@2
  int a4c; // [sp+30h] [bp+Ch]@9
  signed int a4a; // [sp+30h] [bp+Ch]@10

  v4 = a4;
  v15 = a2;
  v14 = a1;
  v5 = fixpoint_mul(a2->x, a4->pFacePlane_old.vNormal.x);
  a4b = fixpoint_mul(a2->y, a4->pFacePlane_old.vNormal.y);
  v6 = fixpoint_mul(a2->z, v4->pFacePlane_old.vNormal.z);
  v7 = v5 + v6 + a4b;
  //(v16 = v5 + v6 + a4b) == 0;
  if ( a4->uAttributes & 0x20000000 || !v7 || v7 > 0 && !v4->Portal())
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
  v11 = v14;
  LODWORD(v10) = v9 << 16;
  HIDWORD(v10) = v9 >> 16;
  a4a = v10 / v7;
  v17 = v10 / v7;
  LOWORD(v12) = LOWORD(v14->x) + (((unsigned int)fixpoint_mul(v17, v15->x) + 0x8000) >> 16);
  HIWORD(v12) = LOWORD(v11->y) + (((unsigned int)fixpoint_mul(v17, v15->y) + 0x8000) >> 16);
  if ( a4c > abs(v7) || ( v17 > *a3 << 16) || !sub_475665(v4, v12, LOWORD(v11->z) + (((unsigned int)fixpoint_mul(v17, v15->z) + 0x8000) >> 16)) )
    return 0;
  *a3 = a4a >> 16;
  return 1;
}

//----- (00475F30) --------------------------------------------------------
bool __fastcall sub_475F30(int *a1, BLVFace *a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9)
{
  int v10; // ST20_4@2
  int v11; // ST28_4@2
  int v12; // ST24_4@2
  int v13; // zf@2
  int v14; // edi@2
  signed int v16; // esi@5
  int v17; // ST20_4@9
  signed __int64 v18; // qtt@10
  int v19; // ST14_4@11
  int v22; // [sp+1Ch] [bp-8h]@2
  int v23; // [sp+1Ch] [bp-8h]@10
  signed int v24; // [sp+20h] [bp-4h]@10

  v10 = fixpoint_mul(a6, a2->pFacePlane_old.vNormal.x);
  v11 = fixpoint_mul(a7, a2->pFacePlane_old.vNormal.y);
  v12 = fixpoint_mul(a8, a2->pFacePlane_old.vNormal.z);
  v13 = v10 + v12 + v11;
  v14 = v10 + v12 + v11;
  v22 = v10 + v12 + v11;
  if ( a2->Ethereal() || !v13 || v14 > 0 && !a2->Portal())
    return 0;
  v16 = -(a2->pFacePlane_old.dist + a4 * a2->pFacePlane_old.vNormal.y + a3 * a2->pFacePlane_old.vNormal.x + a5 * a2->pFacePlane_old.vNormal.z);
  if ( v14 <= 0 )
  {
    if ( a2->pFacePlane_old.dist + a4 * a2->pFacePlane_old.vNormal.y + a3 * a2->pFacePlane_old.vNormal.x + a5 * a2->pFacePlane_old.vNormal.z < 0 )
      return 0;
  }
  else
  {
    if ( v16 < 0 )
      return 0;
  }
  v17 = abs(-(a2->pFacePlane_old.dist + a4 * a2->pFacePlane_old.vNormal.y + a3 * a2->pFacePlane_old.vNormal.x + a5 * a2->pFacePlane_old.vNormal.z)) >> 14;
  LODWORD(v18) = v16 << 16;
  HIDWORD(v18) = v16 >> 16;
  v24 = v18 / v22;
  v23 = v18 / v22;
  LOWORD(v19) = a3 + (((unsigned int)fixpoint_mul(v23, a6) + 0x8000) >> 16);
  HIWORD(v19) = a4 + (((unsigned int)fixpoint_mul(v23, a7) + 0x8000) >> 16);
  if ( v17 > abs(v14) || v23 > *a1 << 16 || !sub_4759C9(a2, a9, v19, a5 + (((unsigned int)fixpoint_mul(v23, a8) + 0x8000) >> 16)) )
    return 0;
  *a1 = v24 >> 16;
  return 1;
}

//----- (004760D5) --------------------------------------------------------
PartyAction ActionQueue::Next()
{
  if (!uNumActions)
    return PARTY_INVALID;

  PartyAction result = pActions[0];
  for (unsigned int i = 0; i < uNumActions - 1; ++i)
    pActions[i] = pActions[i + 1];
  --uNumActions;

  return result;
}

//----- (004775ED) --------------------------------------------------------
int stru6_stru1_indoor_sw_billboard::_4775ED(float a2)
{
  char *v2; // edi@1
  //int v3; // eax@1
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
//  int result; // eax@12
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

//  __debugbreak();//нужно почистить, срабатывает при применении закла “очечный взрыв и при стрельбе из жезла
  v2 = (char *)&this->field_64[4 * this->uNumVertices];
  v38 = 0;
  *(int *)v2 = this->field_64[0];
  v2 += 4;
  *(int *)v2 = this->field_64[1];
  v2 += 4;
  *(int *)v2 = this->field_64[2];
  *((int *)v2 + 1) = this->field_64[3];
  //v3 = this->uNumVertices;
  v35 = this;
  if ( this->uNumVertices > 0 )
  {
    v40 = &this->field_64[20];
    v4 = (char *)&this->field_64[3] + 3;
    
    //while ( 1 )
	for ( v34 = this->uNumVertices; v34; --v34 )
    {
      v5 = v4 - 15;
      if ( *(float *)(v4 - 15) <= (double)a2 && *(float *)(v4 + 1) <= (double)a2 )
	  {
        v4 += 16;
		//--v34;
		//if ( !v34 )
		  //return this->uNumVertices = v38;
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
      //--v34;
      //if ( !v34 )
          //return this->uNumVertices = v38;
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
//  int result; // eax@12
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

  __debugbreak();//нужно почистить
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
//  __int16 v17; // fps@16
//  unsigned __int8 v18; // c2@16
//  unsigned __int8 v19; // c3@16
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

  //__debugbreak();//нужно почистить, срабатывает при применении  закла “очечный взрыв
  if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
  {
    
    if ( this->uNumVertices > 0 )
    {
      v3 = (int)&this->field_14[1];
      //do
      for ( v2 = 0; v2 < this->uNumVertices; ++v2 )
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
        //++v2;
        *(int *)(v3 + 84) = LODWORD(v13);
        v14 = *(int *)(v3 + 8);
        *(float *)(v3 + 76) = v10;
        *(int *)(v3 + 88) = v14;
        *(float *)(v3 + 80) = v11;
        v3 += 16;
      }
      //while ( v2 < this->uNumVertices );
    }
  }
  else
  {
    v15 = (char *)&this->field_14[1];
    //do
    for ( v31 = 3; v31; --v31 )
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
      //--v31;
    }
    //while ( v31 );
  }
  this->uNumVertices = 3;
  return 1;
}

//----- (00477F63) --------------------------------------------------------
bool stru6_stru1_indoor_sw_billboard::sub_477F63()
{
  signed int v1; // ebx@1
  double v3; // st7@2
  //int v4; // edx@4
  char *v5; // ecx@5
  int v6; // edi@5
  float v7; // ST08_4@13
  signed int v9; // [sp+Ch] [bp-8h]@1
  float v10; // [sp+10h] [bp-4h]@2

  //__debugbreak();// почистить
  v1 = 0;
  v9 = 0;
  if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
  {
    v10 = 16192.0;
    v3 = (double)pBLVRenderParams->fov_rad_fixpoint * 0.000015258789;
  }
  else if ( uCurrentlyLoadedLevelType == LEVEL_Outdoor )
  {
    v10 = (double)pODMRenderParams->shading_dist_mist;
    v3 = 8.0;
  }
  else
    __debugbreak();//Error
  if ( this->uNumVertices <= 0 )
  {
    memcpy(&this->field_14[40], &this->field_14[20], 16 * this->uNumVertices);
    return this->uNumVertices != 0;
  }
  v5 = (char *)&this->field_14[20];
  for ( v6 = 0; v6 < this->uNumVertices; v6++ )
  {
    if ( v3 >= *(float *)v5 || *(float *)v5 >= (double)v10 )
    {
      if ( v3 < *(float *)v5 )
        v9 = 1;
      else
        v1 = 1;
    }
    v5 += 16;
  }
  if ( !v1 )
  {
    if ( v9 )
    {
      this->_477927(v10);
      return this->uNumVertices != 0;
    }
    memcpy(&this->field_14[40], &this->field_14[20], 16 * this->uNumVertices);
    return this->uNumVertices != 0;
  }
  v7 = v3;
  _4775ED(v7);
  return this->uNumVertices != 0;
}

//----- (0047802A) --------------------------------------------------------
int stru6_stru1_indoor_sw_billboard::sub_47802A()
{
  double v6; // st7@4
  signed int v16; // [sp+38h] [bp-Ch]@1
  int a6; // [sp+3Ch] [bp-8h]@5
  int a5; // [sp+40h] [bp-4h]@5

//  __debugbreak(); //необходимо проверить this->field_B4[i*4+16]
  v16 = 0;
  if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
  {
    for(int i = 0; i < this->uNumVertices; i++)
    {
      v6 = (double)pBLVRenderParams->fov_rad_fixpoint * 0.000015258789 / this->field_B4[i*4];
      //if ( pRenderer->pRenderD3D )
      {
        pGame->pIndoorCameraD3D->Project(round_to_int(this->field_B4[i*4]), round_to_int(this->field_B4[i*4+1]), round_to_int(this->field_B4[i*4+2]),
           &a5, &a6);
        this->field_B4[i*4+16] = (double)a5;
        this->field_B4[i*4+17] = (double)a6;
        this->field_B4[i*4+18] = round_to_int(this->field_B4[i*4]);
      }
      /*else
      {
        this->field_B4[i*4+16] = (double)pBLVRenderParams->uViewportCenterX - v6 * this->field_B4[i*4+1];
        this->field_B4[i*4+17] = (double)pBLVRenderParams->uViewportCenterY - v6 * this->field_B4[i*4+2];
        this->field_B4[i*4+18] = this->field_B4[i*4];
      }*/
      this->field_B4[i*4+19] = this->field_B4[i*4+3];
      if ( (double)(signed int)pViewport->uViewportTL_X <= this->field_B4[i*4+16] && (double)(signed int)pViewport->uViewportBR_X > this->field_B4[i*4+16]
        && (double)(signed int)pViewport->uViewportTL_Y <= this->field_B4[i*4+17] && (double)(signed int)pViewport->uViewportBR_Y > this->field_B4[i*4+17] )
        v16 = 1;
    }
  }
  else
  {
    for(int i = 0; i < this->uNumVertices; i++)
    {
       this->field_B4[i*4+20] = (double)pViewport->uScreenCenterX - (double)pODMRenderParams->int_fov_rad / this->field_B4[i*4] * this->field_B4[i*4+1];
       this->field_B4[i*4+21] = (double)pViewport->uScreenCenterY - (double)pODMRenderParams->int_fov_rad / this->field_B4[i*4] * this->field_B4[i*4+2];
       *((int *)&this->field_B4[i*4+22]) = (int)this->field_B4[i*4];
       *((int *)&this->field_B4[i*4+23]) = this->field_B4[i*4+3];
       if ( (double)(signed int)pViewport->uViewportTL_X <= this->field_B4[i*4+20] && (double)(signed int)pViewport->uViewportBR_X > this->field_B4[i*4+20]
         && (double)(signed int)pViewport->uViewportTL_Y <= this->field_B4[i*4+21] && (double)(signed int)pViewport->uViewportBR_Y > this->field_B4[i*4+21] )
         v16 = 1;
    }
  }
  return v16;
}

// 73C84C: using guessed type char static_RenderBuildingsD3D_byte_73C84C_init_flag;

//----- (00479089) --------------------------------------------------------
bool __fastcall IsBModelVisible(unsigned int uModelID, int *reachable)
{
  int v3; // edi@1
  int v4; // ebx@1
  int v9; // eax@3
  signed int v11; // esi@6
  int v12; // esi@8
  bool result; // eax@9
  int v17; // [sp+1Ch] [bp-10h]@1
  int v19; // [sp+20h] [bp-Ch]@3
  int angle; // [sp+24h] [bp-8h]@1

  angle = (signed int)(pODMRenderParams->uCameraFovInDegrees << 11) / 360 / 2;
  v3 = pOutdoor->pBModels[uModelID].vBoundingCenter.x - pGame->pIndoorCameraD3D->vPartyPos.x;
  v4 = pOutdoor->pBModels[uModelID].vBoundingCenter.y - pGame->pIndoorCameraD3D->vPartyPos.y;
  stru_5C6E00->Sin(pGame->pIndoorCameraD3D->sRotationX);
  v17 = v3 * stru_5C6E00->Cos(pGame->pIndoorCameraD3D->sRotationY) + v4 * stru_5C6E00->Sin(pGame->pIndoorCameraD3D->sRotationY);
  if (pGame->pIndoorCameraD3D->sRotationX)
    v17 = fixpoint_mul(v17, stru_5C6E00->Cos(pGame->pIndoorCameraD3D->sRotationX));
  v19 = v4 * stru_5C6E00->Cos(pGame->pIndoorCameraD3D->sRotationY) - v3 * stru_5C6E00->Sin(pGame->pIndoorCameraD3D->sRotationY);
  v9 = int_get_vector_length(abs(v3), abs(v4), 0);
  //v10 = v14 * 188;
  //v22 = v9;
  *reachable = false;
  if ( v9 < pOutdoor->pBModels[uModelID].sBoundingRadius + 256 )
    *reachable = true;
  if ( v19 >= 0 )
    v11 = fixpoint_mul(stru_5C6E00->Sin(angle), v17) - fixpoint_mul(stru_5C6E00->Cos(angle), v19);
  else
    v11 = fixpoint_mul(stru_5C6E00->Cos(angle), v19) + fixpoint_mul(stru_5C6E00->Sin(angle), v17);
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
  int v3; // ecx@4
  int v4; // eax@4
  int v5; // edx@4
//  int v6; // ST14_4@5
  Vec3_int_ thisa; // [sp+Ch] [bp-10h]@8
  int v11; // [sp+18h] [bp-4h]@4

  if ( !this->pODMFace->pFacePlane.vNormal.z )
  {
    v3 = this->pODMFace->pFacePlane.vNormal.x;
    v4 = -this->pODMFace->pFacePlane.vNormal.y;
    v5 = 0;
    v11 = 65536;
  }
  else if ( (this->pODMFace->pFacePlane.vNormal.x || this->pODMFace->pFacePlane.vNormal.y)
      && abs(this->pODMFace->pFacePlane.vNormal.z) < 59082 )
  {
    thisa.x = -this->pODMFace->pFacePlane.vNormal.y;
    thisa.y = this->pODMFace->pFacePlane.vNormal.x;
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
  sTextureDeltaU = this->pODMFace->sTextureDeltaU;
  sTextureDeltaV = this->pODMFace->sTextureDeltaV;
  ptr_38->_48616B_frustum_odm(v4, v3, 0, 0, v5, v11);
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
  void*  file = CreateFileW(filename, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
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
  unsigned char* pixels = new unsigned char[imgSize];
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

  unsigned short* pixels_16bit = new unsigned short[imgSize / 3];
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
    delete [] skybox_xp;
    return false;
  }

  int yn_width, yn_height;
  skybox_yn = LoadTgaTexture(yn_filename, &yn_width, &yn_height);
  if (!skybox_yn || yn_width != xn_width || yn_height != xn_height)
  {
    delete [] skybox_xn;
    delete [] skybox_xp;
    delete [] skybox_yn;
    return false;
  }

  int yp_width, yp_height;
  skybox_yp = LoadTgaTexture(yp_filename, &yp_width, &yp_height);
  if (!skybox_yp || yp_width != xn_width || yp_height != xn_height)
  {
    delete [] skybox_xn;
    delete [] skybox_xp;
    delete [] skybox_yn;
    delete [] skybox_yp;
    return false;
  }

  int zn_width, zn_height;
  skybox_zn = LoadTgaTexture(zn_filename, &zn_width, &zn_height);
  if (!skybox_zn || zn_width != xn_width || zn_height != xn_height)
  {
    delete [] skybox_xn;
    delete [] skybox_xp;
    delete [] skybox_yn;
    delete [] skybox_yp;
    delete [] skybox_zn;
    return false;
  }

  int zp_width, zp_height;
  skybox_zp = LoadTgaTexture(zp_filename, &zp_width, &zp_height);
  if (!skybox_zp || zp_width != xn_width || zp_height != xn_height)
  {
    delete [] skybox_xn;
    delete [] skybox_xp;
    delete [] skybox_yn;
    delete [] skybox_yp;
    delete [] skybox_zn;
    delete [] skybox_zp;
    return false;
  }

  skybox_width = xn_width;
  skybox_height = xn_height;

  __debugbreak();
  //if (!pRenderer->pRenderD3D->CreateTexture(skybox_width, skybox_height, &skybox_surface, &skybox_texture,
                                            //false, false, pRenderer->uMinDeviceTextureDim))
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
    v[0].diffuse = -1;
    v[0].specular = 0;
    v[0].texcoord.x = 0;
    v[0].texcoord.y = 0;
    
    v[1].pos.x = pViewport->uScreen_TL_X + pViewport->uScreenWidth;
    v[1].pos.y = pViewport->uScreen_TL_Y + pViewport->uScreenHeight;
    v[1].pos.z = 0.99989998;
    v[1].rhw = 1;
    v[1].diffuse = -1;
    v[1].specular = 0;
    v[1].texcoord.x = (float)pViewport->uScreenWidth / skybox_width;
    v[1].texcoord.y = (float)pViewport->uScreenHeight / skybox_height;

    v[2].pos.x = pViewport->uScreen_TL_X + pViewport->uScreenWidth;
    v[2].pos.y = pViewport->uScreen_TL_Y;
    v[2].pos.z = 0.99989998;
    v[2].rhw = 1;
    v[2].diffuse = -1;
    v[2].specular = 0;
    v[2].texcoord.x = (float)pViewport->uScreenWidth / skybox_width;
    v[2].texcoord.y = 0;

    memcpy(&v[3], &v[0], sizeof(*v));

    v[4].pos.x = pViewport->uScreen_TL_X;
    v[4].pos.y = pViewport->uScreen_TL_Y + pViewport->uScreenHeight;
    v[4].pos.z = 0.99989998;
    v[4].rhw = 1;
    v[4].diffuse = -1;
    v[4].specular = 0;
    v[4].texcoord.x = 0;
    v[4].texcoord.y = (float)pViewport->uScreenHeight / skybox_height;

    memcpy(&v[5], &v[1], sizeof(*v));

    __debugbreak();
    /*
    pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);
    pRenderer->pRenderD3D->pDevice->SetTexture(0, skybox_texture);
    pRenderer->pRenderD3D->pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1, v, 6, D3DDP_DONOTUPDATEEXTENTS | D3DDP_DONOTLIGHT);
*/
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
  if ( _A750D8_player_speech_timer )
    _A750D8_player_speech_timer = 0;
  v2 = pMapStats->GetMapInfo(pCurrentMapName);
  v3 = 0;
  if ( v2 )
  {
    v4 = &pMapStats->pInfos[v2];
    v3 = v4->uRespawnIntervalDays;
  }
  else
    v4 = (MapInfo *)thisa;
  day_attrib &= ~DAY_ATTRIB_FOG;
  dword_6BE13C_uCurrentlyLoadedLocationID = v2;
  pOutdoor->Initialize(
    pFilename,
    (unsigned int)(signed __int64)((double)(signed __int64)pParty->uTimePlayed * 0.234375) / 0x3C / 0x3C / 0x18 + 1,
    v3,
    (int)&thisa);
  if ( !(dword_6BE364_game_settings_1 & GAME_SETTINGS_2000) )
  {
    Actor::InitializeActors();
    SpriteObject::InitializeSpriteObjects();
  }
  dword_6BE364_game_settings_1 &= ~GAME_SETTINGS_2000;
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
      SpawnPointMM7* spawn = pOutdoor->pSpawnPoints + i;
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
  pOutdoor->ArrangeSpriteObjects();
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
int __fastcall sub_47C3D7_get_fog_specular(int a1, int a2, float a3)
{
  int v3; // ecx@1
  signed int v7; // ecx@11

  v3 = pWeather->bNight;
  if ( bUnderwater == 1 )
    v3 = 0;
  if ( pParty->armageddon_timer || !(day_attrib & DAY_ATTRIB_FOG) && !bUnderwater )
    return 0xFF000000;
  if ( v3 )
  {
    if ( a3 < (double)day_fogrange_1 )
    {
      v7 = 0;
      if ( a3 == 0.0 )
        v7 = 216;
      if ( a2 )
        v7 = 248;
      return (-1 - v7) << 24;
    }
    else
    {
      if ( a3 > (double)day_fogrange_2 )
      {
        v7 = 216;
        if ( a3 == 0.0 )
          v7 = 216;
        if ( a2 )
          v7 = 248;
        return (-1 - v7) << 24;
      }
      v7 = (signed __int64)((a3 - (double)day_fogrange_1) / ((double)day_fogrange_2 - (double)day_fogrange_1) * 216.0);
    }
  }
  else
  {
    if ( a3 < (double)day_fogrange_1 )
    {
      v7 = 0;
      if ( a3 == 0.0 )
        v7 = 216;
      if ( a2 )
        v7 = 248;
      return (-1 - v7) << 24;
    }
    else
    {
      if ( a3 > (double)day_fogrange_2 )
      {
        v7 = 216;
        if ( a3 == 0.0 )
          v7 = 216;
        if ( a2 )
          v7 = 248;
        return (-1 - v7) << 24;
      }
      else
        v7 = floorf(((a3 - (double)day_fogrange_1) * 216.0 / ((double)day_fogrange_2 - (double)day_fogrange_1)) + 0.5f);
    }
  }
  if ( v7 > 216 )
    v7 = 216;
  else
  {
    if ( a3 == 0.0 )
      v7 = 216;
  }
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
//  float a3a; // [sp+1Ch] [bp+8h]@33
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
    {
      v6 = 216;
      goto LABEL_20;
    }
    if ( distance <= v9 )
    {
      if ( distance > 0.0 )
      {
        //a4b = distance * 216.0 / v9;
        //v10 = a4b + 6.7553994e15;
        //v6 = LODWORD(v10);
        v6 = floorf(0.5f + distance * 216.0 / v9);
        if (v6 > 216 )
        {
          v6 = 216;
          goto LABEL_20;
        }
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
//LABEL_19:
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
  //int y_min; // esi@10
  //int v11; // [sp+14h] [bp-8h]@1
  //int v12; // [sp+18h] [bp-4h]@1

  //v12 = a1;
  //v11 = a2;
  unsigned int grid_x = WorldPosToGridCellX(pos_x);
  unsigned int grid_z = WorldPosToGridCellZ(pos_z) - 1;

  int party_grid_x1 = GridCellToWorldPosX(grid_x);
  //dword_76D56C_terrain_cell_world_pos_around_party_x = GridCellToWorldPosX(grid_x + 1);
  //dword_76D570_terrain_cell_world_pos_around_party_x = GridCellToWorldPosX(grid_x + 1);
  //dword_76D574_terrain_cell_world_pos_around_party_x = GridCellToWorldPosX(grid_x);
  int party_grid_z1 = GridCellToWorldPosZ(grid_z);
  //dword_76D55C_terrain_cell_world_pos_around_party_z = GridCellToWorldPosZ(grid_z);
  //dword_76D560_terrain_cell_world_pos_around_party_z = GridCellToWorldPosZ(grid_z + 1);
  //dword_76D564_terrain_cell_world_pos_around_party_z = GridCellToWorldPosZ(grid_z + 1);
  int party_x1z1_y = pOutdoor->DoGetHeightOnTerrain(grid_x, grid_z);
  int party_x2z1_y = pOutdoor->DoGetHeightOnTerrain(grid_x + 1, grid_z);
  int party_x2z2_y = pOutdoor->DoGetHeightOnTerrain(grid_x + 1, grid_z + 1);
  int party_x1z2_y = pOutdoor->DoGetHeightOnTerrain(grid_x, grid_z + 1);
  //dword_76D554_terrain_cell_world_pos_around_party_y = v4;
  if (party_x1z1_y == party_x2z1_y &&
      party_x2z1_y == party_x2z2_y &&
      party_x2z2_y == party_x1z2_y )
    return false;

  int dx = abs(pos_x - party_grid_x1),
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

  int y_min = min(y1, min(y2, y3));// не верно при подъЄме на склон
  int y_max = max(y1, max(y2, y3));
  return (y_max - y_min) > 512;
}

//----- (0048257A) --------------------------------------------------------
int __fastcall GetTerrainHeightsAroundParty2(int a1, int a2, int *pIsOnWater, int bFloatAboveWater)
{
//  int result; // eax@9
  int v8; // ebx@11
  int v9; // eax@11
  int v10; // ecx@11
  int v13; // [sp+10h] [bp-8h]@11
  signed int v14; // [sp+14h] [bp-4h]@3
  int v15; // [sp+24h] [bp+Ch]@11

  unsigned int grid_x = WorldPosToGridCellX(a1);
  unsigned int grid_z = WorldPosToGridCellZ(a2) - 1;

  int grid_x1 = GridCellToWorldPosX(grid_x),
       grid_x2 = GridCellToWorldPosX(grid_x + 1);
  int grid_z1 = GridCellToWorldPosZ(grid_z),
       grid_z2 = GridCellToWorldPosZ(grid_z + 1);

  int y_x1z1 = pOutdoor->DoGetHeightOnTerrain(grid_x,     grid_z),
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
  //double v2; // st7@1
  //double v3; // st6@1
  //double v5; // st5@1

 // v2 = (double)this->v_18.x;
  //v3 = (double)this->v_18.y;
 // v5 = (double)this->v_18.z;
  float len = sqrt((double)this->v_18.z * (double)this->v_18.z + (double)this->v_18.y * (double)this->v_18.y + (double)this->v_18.x * (double)this->v_18.x);
  if (fabsf(len) < 1e-6f)
  {
    v_18.x = 0;
    v_18.y = 0;
    v_18.z = 65536;
  }
  else
  {
    v_18.x = round_to_int((double)this->v_18.x / len * 65536.0);
    v_18.y = round_to_int((double)this->v_18.y / len * 65536.0);
    v_18.y = round_to_int((double)this->v_18.z / len * 65536.0);
  }
}

//----- (0048616B) --------------------------------------------------------
void stru149::_48616B_frustum_odm(int a2, int a3, int a4, int a5, int a6, int a7)
{
  int v7; // ebx@1
  int v9; // edi@1
  int v11; // edx@1
  int v17; // ST0C_4@6
  int v19; // ST0C_4@9
  int v24; // [sp+14h] [bp-14h]@1
  int v25; // [sp+18h] [bp-10h]@1
  int v27; // [sp+24h] [bp-4h]@1

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


//----- (00441A4E) --------------------------------------------------------
__int16 __fastcall sub_441A4E(int a1)//for blessing
{
  __int16 result; // ax@1
  int v2; // ebx@1
//  char *v3; // esi@1
//  int v4; // edi@4
  bool v5; // ecx@4
  SpriteFrame *pFrame; // eax@6
  //SpriteFrame *v7; // edi@6
  int v8; // eax@6
//  unsigned __int16 v9; // ax@6
  RenderBillboardTransform_local0 v10; // [sp+Ch] [bp-5Ch]@1
  int v11; // [sp+5Ch] [bp-Ch]@6
  int v12; // [sp+60h] [bp-8h]@1
  //int v13; // [sp+64h] [bp-4h]@6

  v10.sParentBillboardID = -1;
  v10.pTarget = pRenderer->pTargetSurface;
  v10.pTargetZ = pRenderer->pActiveZBuffer;
  v10.uTargetPitch = pRenderer->GetRenderWidth();
  result = 0;
  v2 = a1;
  v10.uViewportX = 0;
  v10.uViewportY = 0;
  v10.uViewportZ = window->GetWidth() - 1;
  v10.uViewportW = window->GetHeight() - 1;
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
            //v13 = fixpoint_mul(v11, pFrame->scale);
            v10.uScreenSpaceX = pOtherOverlayList->pOverlays[i].field_8;
            v10.uScreenSpaceY = pOtherOverlayList->pOverlays[i].field_A;
            v10._screenspace_x_scaler_packedfloat = fixpoint_mul(v11, pFrame->scale);
            v10._screenspace_y_scaler_packedfloat = fixpoint_mul(v11, pFrame->scale);
            v10.pPalette = PaletteManager::Get_Dark_or_Red_LUT(pFrame->uPaletteIndex, 0, 1);
            v8 = pOtherOverlayList->pOverlays[i].field_2;
            v10.sZValue = 0;
            v10.uFlags = 0;
            //v9 = pOverlayList->pOverlays[v8].uOverlayType;
            if ( !pOverlayList->pOverlays[v8].uOverlayType || pOverlayList->pOverlays[v8].uOverlayType == 2 )
              v10.uScreenSpaceY += pSprites_LOD->pSpriteHeaders[pFrame->pHwSpriteIDs[0]].uHeight / 2;
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


//----- (00443E31) --------------------------------------------------------
void LoadLevel_InitializeLevelStr()
    {
  
//  char Args[100]; 
  int string_num;
  int max_string_length;
//  int current_string_length;
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
  int v6; // eax@9
  unsigned __int64 v8; // qax@26
  int hours; // ebx@26
  unsigned __int64 v18; // [sp+Ch] [bp-44h]@12
  unsigned int seconds; // [sp+14h] [bp-3Ch]@26
  unsigned __int64 v20; // [sp+1Ch] [bp-34h]@7
  unsigned int minutes; // [sp+2Ch] [bp-24h]@26
  unsigned int years; // [sp+34h] [bp-1Ch]@26
  unsigned int weeks; // [sp+38h] [bp-18h]@26
  int v26; // [sp+3Ch] [bp-14h]@15
  unsigned int days; // [sp+3Ch] [bp-14h]@26
  unsigned int months; // [sp+40h] [bp-10h]@26

  for (uint i = 0; i < uLevelEVT_NumEvents; ++i)
  {
    EventIndex pEvent = pLevelEVT_Index[i];

    _evt_raw* _evt = (_evt_raw *)(&pLevelEVT[pEvent.uEventOffsetInEVT]);

    if (_evt->_e_type == EVENT_PlaySound)
      pSoundList->LoadSound(EVT_DWORD(_evt->v5), 0);
    else if (_evt->_e_type == EVENT_OnMapReload)
      EventProcessor(pEvent.uEventID, 0, 0, pEvent.event_sequence_num);
    else if (_evt->_e_type == EVENT_OnTimer || _evt->_e_type == EVENT_Initialize)
    {
      //v3 = &MapsLongTimersList[MapsLongTimers_count];
      v20 = pOutdoor->loc_time.uLastVisitDay;
      if (uCurrentlyLoadedLevelType == LEVEL_Indoor)
        v20 = pIndoor->stru1.uLastVisitDay;

      MapsLongTimersList[MapsLongTimers_count].timer_evt_type = _evt->_e_type;
      MapsLongTimersList[MapsLongTimers_count].timer_evt_ID = pEvent.uEventID;
      MapsLongTimersList[MapsLongTimers_count].timer_evt_seq_num = pEvent.event_sequence_num;

      MapsLongTimersList[MapsLongTimers_count].YearsInterval = _evt->v5;
      MapsLongTimersList[MapsLongTimers_count].MonthsInterval = _evt->v6;
      MapsLongTimersList[MapsLongTimers_count].WeeksInterval = _evt->v7;
      MapsLongTimersList[MapsLongTimers_count].HoursInterval = _evt->v8;
      MapsLongTimersList[MapsLongTimers_count].MinutesInterval = _evt->v9;
      MapsLongTimersList[MapsLongTimers_count].SecondsInterval = _evt->v10;


      v6 = ((unsigned short)_evt->v12 << 8) + _evt->v11;

      MapsLongTimersList[MapsLongTimers_count].time_left_to_fire = ((unsigned short)_evt->v12 << 8) + _evt->v11;
      MapsLongTimersList[MapsLongTimers_count].IntervalHalfMins = ((unsigned short)_evt->v12 << 8) + _evt->v11;
      if (MapsLongTimersList[MapsLongTimers_count].timer_evt_type  == EVENT_Initialize && !(short)v6 )
      {
        if ( v20 )
          v18 = pParty->uTimePlayed - v20;
        else
          v18 = 0;
        v26 = (signed int)(signed __int64)((double)(signed __int64)v18 * 0.234375) / 60 / 60 / 24;

        if ( v26 / 7 / 4 / 12 != 0 && MapsLongTimersList[MapsLongTimers_count].YearsInterval ||
          v26 / 7 / 4 != 0 && MapsLongTimersList[MapsLongTimers_count].MonthsInterval != 0 ||
          v26 / 7 != 0 && MapsLongTimersList[MapsLongTimers_count].WeeksInterval != 0 ||
          v26 != 0 || !v20)
        {
          ++MapsLongTimers_count;
          MapsLongTimersList[MapsLongTimers_count].NextStartTime = 0;
          continue;
        }
      }
      else
      {
        v8 = (__int64)((double)pParty->uTimePlayed * 0.234375);
        seconds = v8 % 60;
        minutes = (v8 / 60) % 60;
        hours   = ((v8 / 60) / 60) % 24;
        days    = (((v8 / 60) / 60) / 24) % 7;
        weeks   = ((((v8 / 60) / 60) / 24) / 7) % 4;
        months  = (((((v8 / 60) / 60) / 24) / 7) / 4) % 12;
        years   = (((((v8 / 60) / 60) / 24) / 7) / 4) / 12;

        if ( MapsLongTimersList[MapsLongTimers_count].YearsInterval )
          ++years;
        else if ( MapsLongTimersList[MapsLongTimers_count].MonthsInterval )
          ++months;
        else if ( MapsLongTimersList[MapsLongTimers_count].WeeksInterval )
          ++weeks;
        else
        {
          ++days;
          hours = MapsLongTimersList[MapsLongTimers_count].HoursInterval;
          minutes = MapsLongTimersList[MapsLongTimers_count].MinutesInterval;
          seconds = MapsLongTimersList[MapsLongTimers_count].SecondsInterval;
        }
        MapsLongTimersList[MapsLongTimers_count].NextStartTime = (signed __int64)((double)((seconds
                    + 60 * minutes
                    + 3600 * hours
                    + 86400 * days
                    + 604800 * weeks
                    + 2419200 * months 
                    + 29030400 * years) << 7)
                    * 0.033333335);

        ++MapsLongTimers_count;
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


//----- (004451A8) --------------------------------------------------------
void __fastcall sub_4451A8_press_any_key(int a1, int a2, int a4)
{
  if ( !pGUIWindow2 )
  {
    if ( pParty->uFlags & 2 )
      pGame->Draw();
    pAudioPlayer->StopChannels(-1, -1);
    pMiscTimer->Pause();
    pEventTimer->Pause();
    dword_5C3418 = a1;
    dword_5C341C = a2;
    _591094_decoration = activeLevelDecoration;
    pGUIWindow2 = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_GreetingNPC, a4, 0);
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
  activeLevelDecoration = nullptr;
  pEventTimer->Resume();
}

//----- (004465DF) --------------------------------------------------------
bool sub_4465DF_check_season(int a1)
{
  unsigned int monthPlusOne; // eax@1
  unsigned int daysPlusOne; // edx@1

  monthPlusOne = pParty->uCurrentMonth + 1;
  daysPlusOne = pParty->uDaysPlayed + 1;

  switch (a1)
  {
  case 3: //winter 12.21 -> 3.20
    return (monthPlusOne == 12 && daysPlusOne >= 21 || monthPlusOne == 1 || monthPlusOne == 2 || monthPlusOne == 3 && daysPlusOne <= 20);
		break;
  case 2:// autumn/fall 9.21 -> 12.20
    return (monthPlusOne == 9 && daysPlusOne >= 21 || monthPlusOne == 10 || monthPlusOne == 11 || monthPlusOne == 12 && daysPlusOne <= 20);
		break;
  case 1://summer 6.21 -> 9.20
    return (monthPlusOne == 6 && daysPlusOne >= 21 || monthPlusOne == 7 || monthPlusOne == 8 || monthPlusOne == 9 && daysPlusOne <= 20);
		break;
  case 0: //spring 3.21 -> 6.20
    return (monthPlusOne == 3 && daysPlusOne >= 21 || monthPlusOne == 4 || monthPlusOne == 5 || monthPlusOne == 6 && daysPlusOne <= 20);
		break;
  }
  Error("Unknown season");
  return false;
}

//----- (0044861E) --------------------------------------------------------
void __fastcall sub_44861E_set_texture(unsigned int uFaceCog, const char *pFilename)
{
  unsigned int texture; // eax@2

  if ( uFaceCog )
  {
    texture = pBitmaps_LOD->LoadTexture(pFilename);
    if ( texture != -1 ? (int)&pBitmaps_LOD->pTextures[texture] : 0 )
    {
      pBitmaps_LOD->pTextures[texture].palette_id2 = pPaletteManager->LoadPalette(pBitmaps_LOD->pTextures[texture].palette_id1);

      if ( uCurrentlyLoadedLevelType == 1 )
      {
        if ( (signed int)pIndoor->uNumFaceExtras > 1 )
        {
          for ( uint i = 1; i < (signed int)pIndoor->uNumFaceExtras; ++i )
          {
            if ( pIndoor->pFaceExtras[i].sCogNumber == uFaceCog )
            {
              if ( pIndoor->pFaces[pIndoor->pFaceExtras[i].field_C].uAttributes & FACE_TEXTURE_FRAME )
              {
                pIndoor->pFaces[pIndoor->pFaceExtras[i].field_C].uBitmapID = pTextureFrameTable->FindTextureByName(pFilename);
                if ( pIndoor->pFaces[pIndoor->pFaceExtras[i].field_C].uBitmapID)
                {
                  pTextureFrameTable->LoadAnimationSequenceAndPalettes(pIndoor->pFaces[pIndoor->pFaceExtras[i].field_C].uBitmapID);
                }
                else
                {
                  pIndoor->pFaces[pIndoor->pFaceExtras[i].field_C].uBitmapID = texture;
                  pIndoor->pFaces[pIndoor->pFaceExtras[i].field_C].uAttributes &= 0xBF00;
                }
              }
              else
              {
                pIndoor->pFaces[pIndoor->pFaceExtras[i].field_C].uBitmapID = texture;
              }
            }
          }
        }
        pParty->uFlags |= 2;
      }
      else
      {
        for ( uint j = 0; j < (unsigned int)pOutdoor->uNumBModels; ++j )
        {
          for ( uint i = 0; i < pOutdoor->pBModels[j].uNumFaces; ++i )
          {
            if ( pOutdoor->pBModels[j].pFaces[i].sCogNumber == uFaceCog )
            {
              if ( pOutdoor->pBModels[j].pFaces[i].uAttributes & FACE_TEXTURE_FRAME )
              {
                pOutdoor->pBModels[j].pFaces[i].uTextureID = pTextureFrameTable->FindTextureByName(pFilename);
                if ( pOutdoor->pBModels[j].pFaces[i].uTextureID )
                  pTextureFrameTable->LoadAnimationSequenceAndPalettes(pOutdoor->pBModels[j].pFaces[i].uTextureID);
                else
                {
                  pOutdoor->pBModels[j].pFaces[i].uTextureID = texture;
                  pOutdoor->pBModels[j].pFaces[i].uAttributes &= 0xBF00;
                }
              }
              else
                pOutdoor->pBModels[j].pFaces[i].uTextureID = texture;
            }
          }
        }
      }
      pParty->uFlags |= 2;
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

      pParty->uFlags |= 2;
    }
  }
}

//----- (0044892E) --------------------------------------------------------
void __fastcall sub_44892E_set_faces_bit(int sCogNumber, int bit, int on)
{
  if ( sCogNumber )
  {
    if ( uCurrentlyLoadedLevelType == 1 )
    {
      for ( uint i = 1; i < (unsigned int)pIndoor->uNumFaceExtras; ++i )
      {
        if ( pIndoor->pFaceExtras[i].sCogNumber == sCogNumber )
        {
          if ( on )
            pIndoor->pFaces[pIndoor->pFaceExtras[i].field_C].uAttributes |= bit;
          else
            pIndoor->pFaces[pIndoor->pFaceExtras[i].field_C].uAttributes &= ~bit;
        }
      }
      pParty->uFlags |= 2;
    }
    else
    {
      for ( uint j = 0; j < (unsigned int)pOutdoor->uNumBModels; ++j )
      {
        for ( uint i = 0; i < (unsigned int)pOutdoor->pBModels[j].uNumFaces; ++i )
        {
          if ( pOutdoor->pBModels[j].pFaces[i].sCogNumber == sCogNumber )
          {
            if ( on )
              pOutdoor->pBModels[j].pFaces[i].uAttributes |= bit;
            else
              pOutdoor->pBModels[j].pFaces[i].uAttributes &= ~bit;
          }
        }
      }
    }
    pParty->uFlags |= 2;
  }
}


//----- (00448B45) --------------------------------------------------------
void  GameUI_StatusBar_UpdateTimedString(unsigned int bForceHide)
{
  if ( bForceHide || GameUI_Footer_TimeLeft && GetTickCount() >= GameUI_Footer_TimeLeft )
    GameUI_Footer_TimeLeft = 0;
}

//----- (00448B67) --------------------------------------------------------
void OnTimer(int)
{
  if (pEventTimer->bPaused)
    return;
  
  long long v13 = (signed __int64)(pParty->uTimePlayed - _5773B8_event_timer) / 128;
  if (!v13)
    return;

  //uint _v2v3 = pParty->uTimePlayed;
  //v3 = HIDWORD(pParty->uTimePlayed);
  //v2 = LODWORD(pParty->uTimePlayed);

  _5773B8_event_timer = pParty->uTimePlayed;

  for (uint i = 0; i < MapsLongTimers_count; ++i)
  {
        //v4 = (char *)&array_5B5928_timers[0].field_C;
    MapsLongTimer* timer = &MapsLongTimersList[i];
        //while ( 1 )
        //{
          //v5 = *(short *)v4;
    if (timer->time_left_to_fire)
    {
      if (v13 < timer->time_left_to_fire)
        timer->time_left_to_fire -= v13;
      else
      {
        timer->time_left_to_fire = timer->IntervalHalfMins;
        EventProcessor(timer->timer_evt_ID, 0, 1, timer->timer_evt_seq_num);
      }
    }
    else
    {
      if (timer->NextStartTime < pParty->uTimePlayed)
      {
        uint next_trigger_time = 1 * 60 * 60 * 24; // 1 day
        if (timer->YearsInterval)
          next_trigger_time = 336 * 60 * 60 * 24; // 1 year
        else if (timer->MonthsInterval)
          next_trigger_time = 28 * 60 * 60 * 24; // 1 month
        else if (timer->WeeksInterval)
          next_trigger_time = 7 * 60 * 60 * 24; // 1 week

        timer->NextStartTime += (next_trigger_time * 128) / 3.0f;
        if (timer->NextStartTime < pParty->uTimePlayed) // make sure in wont fire several times in a row if big time interval has lapsed
          timer->NextStartTime = pParty->uTimePlayed;

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
  unsigned int map_id; // eax@1
  size_t old_num_actors; // ebx@2
  AIDirection v15; // [sp+28h] [bp-34h]@2
  SpawnPointMM7 pSpawnPoint; // [sp+44h] [bp-18h]@1

  pSpawnPoint.vPosition.x = x;
  pSpawnPoint.vPosition.y = y;
  pSpawnPoint.vPosition.z = z;
  pSpawnPoint.uGroup = group;
  pSpawnPoint.uRadius = 32;
  pSpawnPoint.uKind = 3;
  pSpawnPoint.uIndex = typeindex + 2 * level + level;
  map_id = pMapStats->GetMapInfo(pCurrentMapName);
  if ( map_id )
  {
    old_num_actors = uNumActors;
    SpawnEncounter(&pMapStats->pInfos[map_id], &pSpawnPoint, 0, count, 0);
    Actor::GetDirectionInfo(PID(OBJECT_Actor, old_num_actors), 4, &v15, 1);
    for ( uint i = (unsigned int)old_num_actors; i < (unsigned int)uNumActors; ++i )
    {
      pActors[i].PrepareSprites(0);
      pActors[i].uYawAngle = v15.uYawAngle;
      pActors[i].dword_000334_unique_name = uUniqueName;
    }
  }
}

//----- (0044987B) --------------------------------------------------------
void Transition_StopSound_Autosave(const char *pMapName, MapStartPoint start_point)
{
  pAudioPlayer->StopChannels(-1, -1);
  pGameLoadingUI_ProgressBar->Initialize(GUIProgressBar::TYPE_None);
  if ( _stricmp(pCurrentMapName, pMapName) )
    SaveGame(1, 0);

  uGameState = GAME_STATE_CHANGE_LOCATION;
  strcpy(pCurrentMapName, pMapName);
  uLevel_StartingPointType = start_point;
}
// 6BE35C: using guessed type int uLevel_StartingPointType;

//----- (004498D5) --------------------------------------------------------
void TeleportToStartingPoint(MapStartPoint point)
{
  const char *model_name; // [sp-4h] [bp-84h]@6
  char pName[128]; // [sp+8h] [bp-78h]@11

  switch (point)
  {
    case MapStartPoint_Party: model_name = "Party Start"; break;
    case MapStartPoint_North: model_name = "North Start"; break;
    case MapStartPoint_South: model_name = "South Start"; break;
    case MapStartPoint_East: model_name = "East Start";  break;
    case MapStartPoint_West: model_name = "West Start";  break;
    default:
      Error("Invalid enum value: %u", point);
  }

  strcpy(pName, model_name);
  if ( pDecorationList->GetDecorIdByName(pName) )
  {
    
    if ( (signed int)uNumLevelDecorations > 0 )
    {
      for ( uint i = 0; i < (signed int)uNumLevelDecorations; ++i )
      {
        if ( pLevelDecorations[i].uDecorationDescID == (signed __int16)pDecorationList->GetDecorIdByName(pName) )
        {
          pParty->vPosition.x = pLevelDecorations[i].vPosition.x;
          pParty->vPosition.y = pLevelDecorations[i].vPosition.y;
          pParty->vPosition.z = pLevelDecorations[i].vPosition.z;
          pParty->uFallStartY = pParty->vPosition.z;
          pParty->sRotationY = (signed int)(stru_5C6E00->uIntegerHalfPi * pLevelDecorations[i].field_1A) / 90;
          if ( pLevelDecorations[i].field_10_y_rot )
            pParty->sRotationY = pLevelDecorations[i].field_10_y_rot;
          pParty->sRotationX = 0;
          pParty->uFallSpeed = 0;
        }
      }
    }
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

  for (int i = pFontLucida->GetLineWidth(GameUI_Footer_TimedString.data()); i > 450;
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
  Transition_StopSound_Autosave("nwc.blv", MapStartPoint_Party);
  pCurrentScreen = SCREEN_GAME;
  return true;
}

//----- (00401000) --------------------------------------------------------
void mm7__vector_constructor(void *a1, int objSize, int numObjs, int ( *constructor)(int))
{
  void *v4; // esi@2

  if ( numObjs > 0 )
  {
    v4 = a1;
    for ( int i = numObjs; i; --i )
    {
      constructor((int)v4);
      v4 = (char *)v4 + objSize;
    }
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


