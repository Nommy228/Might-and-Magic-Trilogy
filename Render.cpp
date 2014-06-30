#define _CRT_SECURE_NO_WARNINGS
#include "mm7_unsorted_subs.h"
#include "ZlibWrapper.h"
#include "ErrorHandling.h"

#include "Render.h"
#include "Outdoor_stuff.h"
#include "VideoPlayer.h"
#include "Sprites.h"
#include "Mouse.h"
#include "GammaControl.h"
#include "stru6.h"
#include "GUIWindow.h"
#include "DecalBuilder.h"
#include "ParticleEngine.h"
#include "Outdoor.h"
#include "Party.h"
#include "LOD.h"
#include "Viewport.h"
#include "OurMath.h"
#include "PaletteManager.h"
#include "Timer.h"
#include "Game.h"
#include "LightmapBuilder.h"
#include "ObjectList.h"
#include "SpriteObject.h"
#include "DecorationList.h"
#include "Actor.h"
#include "Log.h"
#include "TileFrameTable.h"
#include "texts.h"
#include "mm7_data.h"
#include "MM7.h"
#include "Lights.h"
#include "Level/Decoration.h"
#include "Vis.h"
#include "Registry.h"

//#pragma comment(lib, "lib\\legacy_dx\\lib\\ddraw.lib")
//#pragma comment(lib, "lib\\legacy_dx\\lib\\dxguid.lib")

struct IDirectDrawClipper *pDDrawClipper;
struct Render *pRenderer; // idb
struct RenderVertexD3D3  pVertices[50];
int uNumDecorationsDrawnThisFrame; // weak
RenderBillboard pBillboardRenderList[500];
unsigned int uNumBillboardsToDraw;
int uNumSpritesDrawnThisFrame; // weak

RenderVertexSoft array_507D30[50];
RenderVertexSoft array_50AC10[50];
RenderVertexSoft array_73D150[20];

RenderVertexD3D3 d3d_vertex_buffer[50];

/*  384 */
#pragma pack(push, 1)
struct PCXHeader_1
{
  char manufacturer;
  char version;
  char encoding;
  char bpp;
  __int16 left;
  __int16 up;
  __int16 right;
  __int16 bottom;
  __int16 hdpi;
  __int16 vdpi;
};
#pragma pack(pop)

/*  385 */
#pragma pack(push, 1)
struct PCXHeader_2
{
  char reserved;
  char planes;
  __int16 pitch;
  __int16 palette_info;
};
#pragma pack(pop)

HRESULT __stdcall D3DZBufferFormatEnumerator(DDPIXELFORMAT *Src, DDPIXELFORMAT *Dst);
HRESULT __stdcall DDrawDisplayModesEnumerator(DDSURFACEDESC2 *pSurfaceDesc, __int16 *a2);
HRESULT __stdcall D3DDeviceEnumerator(const GUID *lpGUID, const char *lpDeviceDesc, const char *lpDeviceName, D3DDEVICEDESC *pHWDesc, D3DDEVICEDESC *pSWDesc, struct RenderD3D_aux *a6);
signed int __stdcall RenderD3D__DeviceEnumerator(GUID *lpGUID, const char *lpDevDesc, const char *lpDriverName, RenderD3D__DevInfo *pOut); // idb

//----- (0049E79F) --------------------------------------------------------
bool Render::CheckTextureStages()
{
  bool v0; // edi@1
  IDirectDrawSurface4 *pSurface2; // [sp+Ch] [bp-14h]@1
  IDirectDrawSurface4 *pSurface1; // [sp+10h] [bp-10h]@1
  DWORD v4; // [sp+14h] [bp-Ch]@1
  IDirect3DTexture2 *pTexture2; // [sp+18h] [bp-8h]@1
  IDirect3DTexture2 *pTexture1; // [sp+1Ch] [bp-4h]@1

  v0 = false;
  pRenderD3D->CreateTexture(64u, 64u, &pSurface1, &pTexture1, true, false, 32u);
  pRenderD3D->CreateTexture(64u, 64u, &pSurface2, &pTexture2, true, false, 32u);

  ErrD3D(pRenderD3D->pDevice->SetTexture(0, pTexture1));
  ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_WRAP));
  ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_COLOROP, 2));
  ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, 2));
  ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, 2));
  ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_MINFILTER, 2));
  ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_MIPFILTER, 1));

  ErrD3D(pRenderD3D->pDevice->SetTexture(0, pTexture2));
  ErrD3D(pRenderD3D->pDevice->SetTextureStageState(1, D3DTSS_ADDRESS, D3DTADDRESS_CLAMP));
  ErrD3D(pRenderD3D->pDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1));
  ErrD3D(pRenderD3D->pDevice->SetTextureStageState(1, D3DTSS_COLOROP, 7));
  ErrD3D(pRenderD3D->pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, 2));
  ErrD3D(pRenderD3D->pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, 1));
  ErrD3D(pRenderD3D->pDevice->SetTextureStageState(1, D3DTSS_MAGFILTER, 2));
  ErrD3D(pRenderD3D->pDevice->SetTextureStageState(1, D3DTSS_MINFILTER, 2));
  ErrD3D(pRenderD3D->pDevice->SetTextureStageState(1, D3DTSS_MIPFILTER, 1));

  if ( !pRenderD3D->pDevice->ValidateDevice(&v4) && v4 == 1 )
    v0 = true;
  ErrD3D(pRenderD3D->pDevice->SetTextureStageState(1, D3DTSS_COLOROP, 1u));
  pTexture1->Release();
  pTexture2->Release();
  pSurface1->Release();
  pSurface2->Release();
  return v0;
}


//----- (00440CB8) --------------------------------------------------------
void Render::DrawBillboardList_BLV()
{
  __int16 v2; // ax@3
  int v5; // eax@11
  RenderBillboardTransform_local0 soft_billboard; // [sp+4h] [bp-50h]@1

  soft_billboard.sParentBillboardID = -1;
  soft_billboard.pTarget = pBLVRenderParams->pRenderTarget;
  soft_billboard.pTargetZ = pBLVRenderParams->pTargetZBuffer;
  soft_billboard.uTargetPitch = pRenderer->uTargetSurfacePitch;
  soft_billboard.uViewportX = pBLVRenderParams->uViewportX;
  soft_billboard.uViewportY = pBLVRenderParams->uViewportY;
  soft_billboard.uViewportZ = pBLVRenderParams->uViewportZ - 1;
  soft_billboard.uViewportW = pBLVRenderParams->uViewportW;

  pODMRenderParams->uNumBillboards = ::uNumBillboardsToDraw;
  for (uint i = 0; i < ::uNumBillboardsToDraw; ++i)
  {
    RenderBillboard* p = &pBillboardRenderList[i];

      soft_billboard.uScreenSpaceX = p->uScreenSpaceX;
      soft_billboard.sParentBillboardID = i;
      soft_billboard.uScreenSpaceY = p->uScreenSpaceY;
      soft_billboard._screenspace_x_scaler_packedfloat = p->_screenspace_x_scaler_packedfloat;
      soft_billboard._screenspace_y_scaler_packedfloat = p->_screenspace_y_scaler_packedfloat;
      soft_billboard.sZValue = p->sZValue;
      soft_billboard.uFlags = p->field_1E;
      soft_billboard.sTintColor = p->sTintColor;
      v2 = p->HwSpriteID;
      if ( v2 != -1 )
      {
        if ( pRenderer->pRenderD3D )
          pRenderer->DrawBillboard_Indoor(&soft_billboard, &pSprites_LOD->pHardwareSprites[v2], p->dimming_level);
        else
        {
          soft_billboard.pPalette = PaletteManager::Get_Dark_or_Red_LUT(p->uPalette, p->dimming_level, 1);
          if (p->field_1E & 0x0100)
            soft_billboard.pPalette = pPaletteManager->field_261600[p->uPalette];
          if ( !(soft_billboard.uFlags & 0x40) && soft_billboard.uFlags & 0x80 )
            soft_billboard.pPalette2 = PaletteManager::Get_Dark_or_Red_LUT(p->uPalette, 0, 1);
          v5 = p->HwSpriteID;
          if ( v5 >= 0 )
            pSprites_LOD->pSpriteHeaders[v5].DrawSprite_sw(&soft_billboard, 1);
        }
      }
  }
}

//----- (004A16A5) --------------------------------------------------------
bool Render::AreRenderSurfacesOk()
{
  return pFrontBuffer4 && pBackBuffer4;
}


//----- (004A19D8) --------------------------------------------------------
unsigned int BlendColors(unsigned int a1, unsigned int a2)
{
  /*signed __int64 v2; // ST10_8@1
  double v3; // st7@1
  float v4; // ST24_4@1
  double v5; // ST10_8@1
  int v6; // ST1C_4@1
  float v7; // ST24_4@1
  double v8; // ST10_8@1
  unsigned __int8 v9; // ST20_1@1
  float v10; // ST24_4@1
  double v11; // ST10_8@1
  float v12; // ST24_4@1
  double v13; // ST08_8@1*/

  uint alpha = (uint)floorf(0.5f + (a1 >> 24) / 255.0f *
                                   (a2 >> 24) / 255.0f * 255.0f),
       red = (uint)floorf(0.5f + ((a1 >> 16) & 0xFF) / 255.0f *
                                 ((a2 >> 16) & 0xFF) / 255.0f * 255.0f),
       green = (uint)floorf(0.5f + ((a1 >> 8) & 0xFF) / 255.0f *
                                   ((a2 >> 8) & 0xFF) / 255.0f * 255.0f),
       blue = (uint)floorf(0.5f + ((a1 >> 0) & 0xFF) / 255.0f *
                                   ((a2 >> 0) & 0xFF) / 255.0f * 255.0f);
  return (alpha << 24) | (red << 16) | (green << 8) | blue;
  /*v2 = a1 >> 24;
  v3 = (double)v2 / 255.0f;
  HIDWORD(v2) = 0;
  LODWORD(v2) = a2 >> 24;
  v4 = v3 * (double)v2 / 255.0f * 255.0;
  v5 = v4 + 6.7553994e15;
  v6 = LODWORD(v5);
  v7 = (double)((a1 >> 16) & 0xFFi64) / 255.0f * (double)((a2 >> 16) & 0xFF) * 0.0039215689 * 255.0;
  v8 = v7 + 6.7553994e15;
  v9 = LOBYTE(v8);
  v10 = (double)((unsigned __int16)a1 >> 8) / 255.0f * (double)((unsigned __int16)a2 >> 8) / 255.0f * 255.0;
  v11 = v10 + 6.7553994e15;
  v12 = (double)(a1 & 0xFFi64) / 255.0f * (double)(unsigned __int8)a2 / 255.0f * 255.0;
  v13 = v12 + 6.7553994e15;
  return LOBYTE(v13) | ((LOBYTE(v11) | (((v6 << 8) | v9) << 8)) << 8);*/
}


void Render::RenderTerrainD3D() // New function
{
  int v6; // ecx@8
  struct Polygon *pTilePolygon; // ebx@8
//  float Light_tile_dist;

  //warning: the game uses CW culling by default, ccw is incosistent
  pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CCW);

  static RenderVertexSoft pTerrainVertices[128 * 128];//vertexCountX and vertexCountZ

  //Генерация местоположения вершин-------------------------------------------------------------------------
  //решётка вершин делится на две части от -64 до 0 и от 0 до 64
  //
  // -64  X  0     64
  //  --------------- 64
  //  |      |      |
  //  |      |      |
  //  |      |      |
  // 0|------+------| Z
  //  |      |      |
  //  |      |      |
  //  |      |      |
  //  ---------------
  //                -64

  int blockScale = 512;
  int heightScale = 32;
  for (unsigned int z = 0; z < 128; ++z)
  {
    for (unsigned int x = 0; x < 128; ++x)
    {
      pTerrainVertices[z * 128 + x].vWorldPosition.x = (-64 + (signed)x) * blockScale;
      pTerrainVertices[z * 128 + x].vWorldPosition.y = (64 - (signed)z) * blockScale;
      pTerrainVertices[z * 128 + x].vWorldPosition.z = heightScale * pOutdoor->pTerrain.pHeightmap[z * 128 + x];
      pGame->pIndoorCameraD3D->ViewTransform(&pTerrainVertices[z * 128 + x], 1);
      pGame->pIndoorCameraD3D->Project(&pTerrainVertices[z * 128 + x], 1, 0);
    }
  }
//-------(Отсечение невидимой части карты)------------------------------------------------------------------------------------------
  float direction = pGame->pIndoorCameraD3D->sRotationY / 256;//direction of the camera(напрвление камеры)
  //0-East(B)
  //1-NorthEast(CB)
  //2-North(C)
  //3-WestNorth(CЗ)
  //4-West(З)
  //5-SouthWest(ЮЗ)
  //6-South(Ю)
  //7-SouthEast(ЮВ)
  int Start_X, End_X, Start_Z, End_Z;
  if ( direction >= 0 && direction < 1.0 )//East(B) - NorthEast(CB)
  {
    Start_X = pODMRenderParams->uMapGridCellX - 2, End_X = 127;
    Start_Z = 0, End_Z = 127;
  }
  else if (direction >= 1.0 && direction < 3.0)//NorthEast(CB) - WestNorth(CЗ)
  {
      Start_X = 0, End_X = 127;
      Start_Z = 0, End_Z = pODMRenderParams->uMapGridCellZ + 1;
  }
  else if (direction >= 3.0 && direction < 5.0)//WestNorth(CЗ) - SouthWest(ЮЗ)
  {
    Start_X = 0, End_X = pODMRenderParams->uMapGridCellX + 2;
    Start_Z = 0, End_Z = 127;
  }
  else if ( direction >= 5.0 && direction < 7.0 )//SouthWest(ЮЗ) - //SouthEast(ЮВ)
  {
    Start_X = 0, End_X = 127;
    Start_Z = pODMRenderParams->uMapGridCellZ - 2, End_Z = 127;
  }
  else//SouthEast(ЮВ) - East(B)
  {
    Start_X = pODMRenderParams->uMapGridCellX - 2, End_X = 127;
    Start_Z = 0, End_Z = 127;
  }
  for (unsigned int z = Start_Z; z < End_Z; ++z)
  {
    for (unsigned int x = Start_X; x < End_X; ++x)
    {
      pTilePolygon = &array_77EC08[pODMRenderParams->uNumPolygons];
      pTilePolygon->flags = 0;
      pTilePolygon->field_32 = 0;
      pTilePolygon->uTileBitmapID = pOutdoor->DoGetTileTexture(x, z);
      pTilePolygon->pTexture = (Texture *)&pBitmaps_LOD->pHardwareTextures[pTilePolygon->uTileBitmapID];
      if (pTilePolygon->uTileBitmapID == 0xFFFF)
        continue;

      //pTile->flags = 0x8010 |pOutdoor->GetSomeOtherTileInfo(x, z);
      pTilePolygon->flags = pOutdoor->GetSomeOtherTileInfo(x, z);
      pTilePolygon->field_32 = 0;
      pTilePolygon->field_59 = 1;
      pTilePolygon->sTextureDeltaU = 0;
      pTilePolygon->sTextureDeltaV = 0;
//  x,z         x+1,z
//  .____________.
//  |            |
//  |            |
//  |            |
//  |            |
//  |            |
//  .____________.
//  x,z+1       x+1,z+1
      memcpy(&array_73D150[0], &pTerrainVertices[z * 128 + x], sizeof(RenderVertexSoft));//x, z
      array_73D150[0].u = 0;
      array_73D150[0].v = 0;
      memcpy(&array_73D150[1], &pTerrainVertices[z * 128 + x + 1], sizeof(RenderVertexSoft));//x + 1, z
      array_73D150[1].u = 1;
      array_73D150[1].v = 0;
      memcpy(&array_73D150[2], &pTerrainVertices[(z + 1) * 128 + x + 1], sizeof(RenderVertexSoft));//x + 1, z + 1
      array_73D150[2].u = 1;
      array_73D150[2].v = 1;
      memcpy(&array_73D150[3], &pTerrainVertices[(z + 1) * 128 + x], sizeof(RenderVertexSoft));//x, z + 1
      array_73D150[3].u = 0;
      array_73D150[3].v = 1;
      //v58 = 0;
      //if (v58 == 4) // if all y == first y;  primitive in xz plane 
        //pTile->field_32 |= 0x0001;
      pTilePolygon->pODMFace = nullptr;
      pTilePolygon->uNumVertices = 4;
      pTilePolygon->field_59 = 5;

//shading (затенение)----------------------------------------------------------------------------
      //uint norm_idx = pTerrainNormalIndices[2 * (z * 128 + x) + 1];
      uint norm_idx = pTerrainNormalIndices[2 * (x * 128 + z) + 2];
      assert(norm_idx < uNumTerrainNormals);

      Vec3_float_* norm = &pTerrainNormals[norm_idx];
      float _f = ((norm->x * (float)pOutdoor->vSunlight.x / 65536.0) -
                  (norm->y * (float)pOutdoor->vSunlight.y / 65536.0) -
                  (norm->z * (float)pOutdoor->vSunlight.z / 65536.0));
      pTilePolygon->dimming_level = 20.0 - floorf(20.0 * _f + 0.5f);
      if ( norm_idx < 0 || norm_idx > uNumTerrainNormals - 1 )
        norm = 0;
      else
        norm = &pTerrainNormals[norm_idx];
      //pGame->pLightmapBuilder->StackLights_TerrainFace(norm, &Light_tile_dist, array_50AC10, 4, 1);//Ritor1: slows
      //pDecalBuilder->_49BE8A(pTilePolygon, norm, &Light_tile_dist, array_50AC10, 4, 1);
      //unsigned int a5 = 4;
//----------------------------------------------------------------------------

      ++pODMRenderParams->uNumPolygons;
      ++pODMRenderParams->field_44;
      assert(pODMRenderParams->uNumPolygons < 20000);

      pTilePolygon->uBModelID = 0;
      pTilePolygon->uBModelFaceID = 0;
      pTilePolygon->field_50 = (8 * (0 | (0 << 6))) | 6;
      for (unsigned int k = 0; k < pTilePolygon->uNumVertices; ++k)
      {
        memcpy(&array_50AC10[k], &array_73D150[k], sizeof(struct RenderVertexSoft));
        array_50AC10[k]._rhw = 1.0 / (array_73D150[k].vWorldViewPosition.x + 0.0000001000000011686097);
      }
//---------Draw distance(Дальность отрисовки)-------------------------------
      int temp =  pODMRenderParams->shading_dist_mist;
      if ( draw_terrain_dist_mist )
        pODMRenderParams->shading_dist_mist = 0x5000;
      bool neer_clip = array_73D150[0].vWorldViewPosition.x < 8.0
                    || array_73D150[1].vWorldViewPosition.x < 8.0
                    || array_73D150[2].vWorldViewPosition.x < 8.0
                    || array_73D150[3].vWorldViewPosition.x < 8.0;
      bool far_clip = (double)pODMRenderParams->shading_dist_mist < array_73D150[0].vWorldViewPosition.x
                   || (double)pODMRenderParams->shading_dist_mist < array_73D150[1].vWorldViewPosition.x
                   || (double)pODMRenderParams->shading_dist_mist < array_73D150[2].vWorldViewPosition.x
                   || (double)pODMRenderParams->shading_dist_mist < array_73D150[3].vWorldViewPosition.x;

         /* int v33 = 0;
          static stru154 static_sub_0048034E_stru_154;
          pGame->pLightmapBuilder->std__vector_000004_size = 0;
          if ( stru_F8AD28.uNumLightsApplied > 0 || pDecalBuilder->uNumDecals > 0 )
          {
            if ( neer_clip )
              v33 = 3;
            else
              v33 = far_clip != 0 ? 5 : 0;
            static_sub_0048034E_stru_154.ClassifyPolygon(norm, Light_tile_dist);
            if ( pDecalBuilder->uNumDecals > 0 )
              pDecalBuilder->ApplyDecals(31 - pTilePolygon->dimming_level, 4, &static_sub_0048034E_stru_154, a5, array_50AC10, 0, *(float *)&v33, -1);
          }
          if ( stru_F8AD28.uNumLightsApplied > 0 )
            pGame->pLightmapBuilder->ApplyLights(&stru_F8AD28, &static_sub_0048034E_stru_154, a5, array_50AC10, 0, v33);*/

      if ( !byte_4D864C || ~pGame->uFlags & 0x80 )
      {
        //if ( neer_clip ) //Ritor1: Даёт искажения на подъёме, возможно требуется ф-ция Безье
        //{
         // pTilePolygon->uNumVertices = ODM_NearClip(pTilePolygon->uNumVertices);
         // ODM_Project(pTilePolygon->uNumVertices);
        //}
        if ( far_clip )
        {
          pTilePolygon->uNumVertices = ODM_FarClip(pTilePolygon->uNumVertices);
          ODM_Project(pTilePolygon->uNumVertices);
        }
      }
      pODMRenderParams->shading_dist_mist = temp;

// check the transparency and texture (tiles) mapping (проверка прозрачности и наложение текстур (тайлов))----------------------
      bool transparent = false;
      if ( !( pTilePolygon->flags & 1 ) ) // не поддерживается TextureFrameTable
      {
        if ( /*pTile->flags & 2 && */pTilePolygon->uTileBitmapID == pRenderer->hd_water_tile_id)
        {
          //transparent = false;
          v6 = pRenderer->pHDWaterBitmapIDs[pRenderer->hd_water_current_frame];
        }
        else
        {
          v6 = pTilePolygon->uTileBitmapID;
          if ( !_strnicmp(pBitmaps_LOD->pTextures[pTilePolygon->uTileBitmapID].pName, "wtrdr", 5) )
            transparent = true;
        }

      assert(v6 < 1000); // many random crashes here

      // for all shore tiles - draw a tile water under them since they're half-empty
      if (!_strnicmp(pBitmaps_LOD->pTextures[pTilePolygon->uTileBitmapID].pName, "wtrdr", 5))  // all shore tile filenames are wtrdrXXX
        DrawBorderTiles(pTilePolygon);

      pRenderer->DrawTerrainPolygon(pTilePolygon->uNumVertices, pTilePolygon, pBitmaps_LOD->pHardwareTextures[v6], transparent, true);
      }
      //else //здесь уже пограничные тайлы воды
        //pTile->DrawBorderTiles();
//--------------------------------------------------------------------------------------------------------------------------------

      --pODMRenderParams->uNumPolygons;
      --pODMRenderParams->field_44;
    }
  }
}

//----- (004811A3) --------------------------------------------------------
void Render::DrawBorderTiles(struct Polygon *poly)
{
  pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, false);
  DrawTerrainPolygon(poly->uNumVertices, poly,
                     pBitmaps_LOD->pHardwareTextures[pHDWaterBitmapIDs[hd_water_current_frame]], false, true);

  pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, true);
  //DrawTerrainPolygon(poly->uNumVertices, poly, pBitmaps_LOD->pHardwareTextures[poly->uTileBitmapID], true, true);
}


//----- (0047BACF) --------------------------------------------------------
void Render::TransformBillboardsAndSetPalettesODM()
{
  //int v0; // edi@1
  //char *v1; // esi@2
  //unsigned int v2; // edx@3
  //int v3; // eax@3
  //int v4; // edi@3
  //int v5; // eax@3
  //__int16 v6; // di@3
  //int v7; // eax@3
  //int v8; // ebx@4
//  unsigned __int16 *v9; // eax@7
//  char v10; // zf@9
  //DWORD v11; // eax@13
//  int v12; // eax@13
//  int v13; // eax@14
  RenderBillboardTransform_local0 billboard; // [sp+4h] [bp-60h]@1
//  int v15; // [sp+54h] [bp-10h]@13
  //int v16; // [sp+58h] [bp-Ch]@1
  //int v17; // [sp+5Ch] [bp-8h]@2
//  int v18; // [sp+60h] [bp-4h]@13

  billboard.sParentBillboardID = -1;
  billboard.pTarget = pRenderer->pTargetSurface;
  billboard.pTargetZ = pRenderer->pActiveZBuffer;
  billboard.uTargetPitch = pRenderer->uTargetSurfacePitch;
  billboard.uViewportX = pViewport->uViewportTL_X;
  billboard.uViewportY = pViewport->uViewportTL_Y;
  billboard.uViewportZ = pViewport->uViewportBR_X - 1;
  billboard.uViewportW = pViewport->uViewportBR_Y;
  pODMRenderParams->uNumBillboards = uNumBillboardsToDraw;

  for (int i = 0; i < ::uNumBillboardsToDraw; ++i)
  {
    billboard.uScreenSpaceX = pBillboardRenderList[i].uScreenSpaceX;
    billboard.uScreenSpaceY = pBillboardRenderList[i].uScreenSpaceY;
    billboard.sParentBillboardID = i;
    billboard._screenspace_x_scaler_packedfloat = pBillboardRenderList[i]._screenspace_x_scaler_packedfloat;
    billboard.sTintColor = pBillboardRenderList[i].sTintColor;
    billboard._screenspace_y_scaler_packedfloat = pBillboardRenderList[i]._screenspace_y_scaler_packedfloat;
    billboard.sZValue = pBillboardRenderList[i].sZValue;
    billboard.uFlags = pBillboardRenderList[i].field_1E;
    if (pBillboardRenderList[i].HwSpriteID != -1)
    {
      if (!pRenderer->pRenderD3D) __debugbreak(); // no sw rendering
      //if (pRenderer->pRenderD3D)
        pRenderer->TransformBillboard(&billboard,
                                      &pSprites_LOD->pHardwareSprites[pBillboardRenderList[i].HwSpriteID],
                                      pBillboardRenderList[i].dimming_level, &pBillboardRenderList[i]);
      /*else
      {
          assert(false);

          auto v1 = (char *)&pBillboard->uScreenSpaceY;
          if ( *(v1 - 10) & 2 )
            v9 = PaletteManager::Get_Dark_or_Red_LUT(*((short *)v1 - 7), 0, 1);
          else
            v9 = sr_GetBillboardPalette((RenderBillboard *)(v1 - 40), *((short *)v1 - 7), pBillboard->sZValue, *((short *)v1 + 1));
          v10 = (*(v1 - 9) & 1) == 0;
          billboard.pPalette = v9;
          if ( !v10 )
            billboard.pPalette = pPaletteManager->field_261600[*((short *)v1 - 7)];
          if ( !(billboard.uFlags & 0x40) && billboard.uFlags & 0x80 )
          {
            v12 = stru_5C6E00->Cos(i * 5 + GetTickCount());
            v15 = abs(v12);
            v18 = (unsigned __int64)(15i64 * v15) >> 16;
            billboard.pPalette2 = PaletteManager::Get_Dark_or_Red_LUT(*((short *)v1 - 7), 15 - v18, 1);
          }
          v13 = *((short *)v1 - 8);
          if ( v13 >= 0 )
            pSprites_LOD->pSpriteHeaders[v13].DrawSprite_sw(&billboard, 1);
      }*/
    }
  }
}

//----- (0047AF11) --------------------------------------------------------
void Render::DrawSpriteObjects_ODM()
{
  SpriteFrame *frame; // eax@10
  unsigned int v6; // eax@10
  int v9; // ecx@10
  int v17; // ecx@25
  int v18; // eax@25
//  int v22; // ST3C_4@29
  signed __int64 v23; // qtt@30
  int v26; // eax@31
//  char v27; // zf@31
  int v30; // [sp+14h] [bp-2Ch]@23
  int v37; // [sp+1Ch] [bp-24h]@23
  int a6; // [sp+20h] [bp-20h]@10
  int v42; // [sp+2Ch] [bp-14h]@23
  int y; // [sp+30h] [bp-10h]@10
  int x; // [sp+34h] [bp-Ch]@10
  int z; // [sp+38h] [bp-8h]@10
  signed __int16 v46; // [sp+3Ch] [bp-4h]@12

  //v41 = 0;
  for (int i = 0; i < uNumSpriteObjects; ++i)
  {
    SpriteObject* object = &pSpriteObjects[i];
    //auto v0 = (char *)&pSpriteObjects[i].uSectorID;
    //v0 = (char *)&pSpriteObjects[0].uSectorID;
    //do
    //{
    if (!object->uObjectDescID)  // item probably pciked up
      continue;

    assert(object->uObjectDescID < pObjectList->uNumObjects);
    ObjectDesc* object_desc = &pObjectList->pObjects[object->uObjectDescID];
    if (object_desc->NoSprite())
      continue;

        //v1 = &pObjectList->pObjects[*((short *)v0 - 13)];
        //if ( !(v1->uFlags & 1) )
        //{
          //v2 = *((short *)v0 - 14)
    //v2 = object->uType;
    if ( (object->uType < 1000 || object->uType >= 10000) && (object->uType < 500 || object->uType >= 600)
       || pGame->pStru6Instance->_4A81CA(object) )
    {
            //a5 = *(short *)v0;
      x = object->vPosition.x;
      y = object->vPosition.y;
      z = object->vPosition.z;
      frame = pSpriteFrameTable->GetFrame(object_desc->uSpriteID, object->uSpriteFrameID);
      a6 = frame->uGlowRadius * object->field_22_glow_radius_multiplier;
      v6 = stru_5C6E00->Atan2(object->vPosition.x - pGame->pIndoorCameraD3D->vPartyPos.x, object->vPosition.y - pGame->pIndoorCameraD3D->vPartyPos.y);
      //LOWORD(v7) = object->uFacing;
      //v8 = v36;
      v9 = ((signed int)(stru_5C6E00->uIntegerPi + ((signed int)stru_5C6E00->uIntegerPi >> 3) + object->uFacing - v6) >> 8) & 7;
      pBillboardRenderList[::uNumBillboardsToDraw].HwSpriteID = frame->pHwSpriteIDs[v9];
      if ( frame->uFlags & 0x20 )
      {
        //v8 = v36;
        z -= fixpoint_mul(frame->scale, pSprites_LOD->pSpriteHeaders[(signed __int16)frame->pHwSpriteIDs[v9]].uHeight) / 2;
      }
      v46 = 0;
      if ( frame->uFlags & 2 )
        v46 = 2;
      //v11 = (int *)(256 << v9);
      if ( (256 << v9) & frame->uFlags )
        v46 |= 4u;
      if ( frame->uFlags & 0x40000 )
        v46 |= 0x40u;
      if ( frame->uFlags & 0x20000 )
        LOBYTE(v46) = v46 | 0x80;
      if ( a6 )
      {
        //LOBYTE(v11) = _4E94D3_light_type;
        pMobileLightsStack->AddLight(x, y, z, object->uSectorID, a6, 0xFFu, 0xFFu, 0xFFu, _4E94D3_light_type);
      }
      if (pGame->pIndoorCameraD3D->sRotationX)
      {
        v30 = fixpoint_mul((x - pGame->pIndoorCameraD3D->vPartyPos.x) << 16, pGame->pIndoorCameraD3D->int_cosine_y)
            + fixpoint_mul((y - pGame->pIndoorCameraD3D->vPartyPos.y) << 16, pGame->pIndoorCameraD3D->int_sine_y);
        v37 = fixpoint_mul((x - pGame->pIndoorCameraD3D->vPartyPos.x) << 16, pGame->pIndoorCameraD3D->int_sine_y);
        v42 = fixpoint_mul((z - pGame->pIndoorCameraD3D->vPartyPos.z) << 16, pGame->pIndoorCameraD3D->int_sine_x)
            + fixpoint_mul(v30, pGame->pIndoorCameraD3D->int_cosine_x);
        if ( v42 >= 0x40000 && v42 <= pODMRenderParams->shading_dist_mist << 16 )
        {
          v17 = fixpoint_mul((y - pGame->pIndoorCameraD3D->vPartyPos.y) << 16, pGame->pIndoorCameraD3D->int_cosine_y) - v37;
          v18 = fixpoint_mul((z - pGame->pIndoorCameraD3D->vPartyPos.z) << 16, pGame->pIndoorCameraD3D->int_cosine_x)
              - fixpoint_mul(v30, pGame->pIndoorCameraD3D->int_sine_x);
          if ( abs(v42) >= abs(v17) )
          {
            LODWORD(v23) = 0;
            HIDWORD(v23) = SLOWORD(pODMRenderParams->int_fov_rad);

            object->uAttributes |= 1;
            pBillboardRenderList[::uNumBillboardsToDraw].uPalette = frame->uPaletteIndex;
            pBillboardRenderList[::uNumBillboardsToDraw].uIndoorSectorID = object->uSectorID;
            pBillboardRenderList[::uNumBillboardsToDraw]._screenspace_x_scaler_packedfloat = fixpoint_mul(frame->scale, v23 / v42);
            pBillboardRenderList[::uNumBillboardsToDraw].pSpriteFrame = frame;
            pBillboardRenderList[::uNumBillboardsToDraw]._screenspace_y_scaler_packedfloat = fixpoint_mul(frame->scale, v23 / v42);
            pBillboardRenderList[::uNumBillboardsToDraw].field_1E = v46;
            pBillboardRenderList[::uNumBillboardsToDraw].world_x = x;
            pBillboardRenderList[::uNumBillboardsToDraw].world_y = y;
            pBillboardRenderList[::uNumBillboardsToDraw].world_z = z;
            pBillboardRenderList[::uNumBillboardsToDraw].uScreenSpaceX = pViewport->uScreenCenterX - ((signed int)(fixpoint_mul(v23 / v42, v17) + 0x8000) >> 16);
            pBillboardRenderList[::uNumBillboardsToDraw].uScreenSpaceY = pViewport->uScreenCenterY - (((unsigned int)fixpoint_mul(v23 / v42, v18) + 0x8000) >> 16);
            HIWORD(v26) = HIWORD(v42);
            LOWORD(v26) = 0;
            pBillboardRenderList[::uNumBillboardsToDraw].sZValue = v26 + (PID(OBJECT_Item,i));
            pBillboardRenderList[::uNumBillboardsToDraw].dimming_level = 0;
            pBillboardRenderList[::uNumBillboardsToDraw].sTintColor = 0;
            if ( !(object->uAttributes & 0x20) )
            {
              if ( !pRenderer->pRenderD3D )
                pBillboardRenderList[::uNumBillboardsToDraw].sZValue = 0;
            }
            //if (::uNumBillboardsToDraw >= 500)
            //  return;
            assert(::uNumBillboardsToDraw < 500);
            ++::uNumBillboardsToDraw;
            ++uNumSpritesDrawnThisFrame;
          }
        }
      }
      else
      {
        v42 = fixpoint_mul((y - pGame->pIndoorCameraD3D->vPartyPos.y) << 16, pGame->pIndoorCameraD3D->int_sine_y)
            + fixpoint_mul((x - pGame->pIndoorCameraD3D->vPartyPos.x) << 16, pGame->pIndoorCameraD3D->int_cosine_y);
        if ( v42 >= 0x40000 && v42 <= pODMRenderParams->shading_dist_mist << 16 )
        {
          v17 = fixpoint_mul((y - pGame->pIndoorCameraD3D->vPartyPos.y) << 16, pGame->pIndoorCameraD3D->int_cosine_y)
              - fixpoint_mul(((x - pGame->pIndoorCameraD3D->vPartyPos.x) << 16), pGame->pIndoorCameraD3D->int_sine_y);
          v18 = (z - pGame->pIndoorCameraD3D->vPartyPos.z) << 16;
          if ( abs(v42) >= abs(v17) )
          {
            LODWORD(v23) = 0;
            HIDWORD(v23) = SLOWORD(pODMRenderParams->int_fov_rad);

            object->uAttributes |= 1;
            pBillboardRenderList[::uNumBillboardsToDraw].uPalette = frame->uPaletteIndex;
            pBillboardRenderList[::uNumBillboardsToDraw].uIndoorSectorID = object->uSectorID;
            pBillboardRenderList[::uNumBillboardsToDraw]._screenspace_x_scaler_packedfloat = fixpoint_mul(frame->scale, v23 / v42);
            pBillboardRenderList[::uNumBillboardsToDraw].pSpriteFrame = frame;
            pBillboardRenderList[::uNumBillboardsToDraw]._screenspace_y_scaler_packedfloat = fixpoint_mul(frame->scale, v23 / v42);
            pBillboardRenderList[::uNumBillboardsToDraw].field_1E = v46;
            pBillboardRenderList[::uNumBillboardsToDraw].world_x = x;
            pBillboardRenderList[::uNumBillboardsToDraw].world_y = y;
            pBillboardRenderList[::uNumBillboardsToDraw].world_z = z;
            pBillboardRenderList[::uNumBillboardsToDraw].uScreenSpaceX = pViewport->uScreenCenterX - ((signed int)(fixpoint_mul(v23 / v42, v17) + 0x8000) >> 16);
            pBillboardRenderList[::uNumBillboardsToDraw].uScreenSpaceY = pViewport->uScreenCenterY - (((unsigned int)fixpoint_mul(v23 / v42, v18) + 0x8000) >> 16);
            HIWORD(v26) = HIWORD(v42);
            LOWORD(v26) = 0;
            pBillboardRenderList[::uNumBillboardsToDraw].sZValue = v26 + (PID(OBJECT_Item,i));
            pBillboardRenderList[::uNumBillboardsToDraw].dimming_level = 0;
            pBillboardRenderList[::uNumBillboardsToDraw].sTintColor = 0;
            if ( !(object->uAttributes & 0x20) )
            {
              if ( !pRenderer->pRenderD3D )
                pBillboardRenderList[::uNumBillboardsToDraw].sZValue = 0;
            }
            //if (::uNumBillboardsToDraw >= 500)
            //  return;
            assert(::uNumBillboardsToDraw < 500);
            ++::uNumBillboardsToDraw;
            ++uNumSpritesDrawnThisFrame;
          }
        }
      }
    }
  }
}

//----- (0049D9BC) --------------------------------------------------------
signed int __stdcall RenderD3D__DeviceEnumerator(GUID *lpGUID, const char *lpDevDesc, const char *lpDriverName, RenderD3D__DevInfo *pOut)
{
  size_t v4; // eax@1
  size_t v5; // eax@1
  IUnknown *v6; // eax@10
  size_t v7; // eax@13
  DDDEVICEIDENTIFIER ddDevId; // [sp+4h] [bp-4F8h]@11
  DDSURFACEDESC2 v10;/*int v10; // [sp+42Ch] [bp-D0h]@16
  int v11; // [sp+430h] [bp-CCh]@16
  int v12; // [sp+434h] [bp-C8h]@16
  int v13; // [sp+438h] [bp-C4h]@16
  int v14; // [sp+474h] [bp-88h]@16*/
  DDSCAPS2 ddsCaps; // [sp+4A8h] [bp-54h]@14
  unsigned int uFreeVideoMem; // [sp+4B8h] [bp-44h]@14
  RenderD3D_aux aux; // [sp+4BCh] [bp-40h]@19
  IDirect3D3 *pDirect3D3; // [sp+4C4h] [bp-38h]@18
  int v19; // [sp+4C8h] [bp-34h]@16
  RenderD3D_D3DDevDesc v20; // [sp+4CCh] [bp-30h]@1
  LPDIRECTDRAW pDirectDraw; // [sp+4F4h] [bp-8h]@4
  IDirectDraw4 *pDirectDraw4; // [sp+4F8h] [bp-4h]@7

  v4 = strlen(lpDriverName);
  v20.pDriverName = new char[v4 + 1];
  v5 = strlen(lpDevDesc);
  v20.pDeviceDesc = new char[v5 + 1];
  strcpy(v20.pDriverName, lpDriverName);
  strcpy(v20.pDeviceDesc, lpDevDesc);
  if ( lpGUID )
  {
    v20.pGUID = new GUID;
    memcpy(v20.pGUID, lpGUID, 0x10u);
  }
  else
    v20.pGUID = 0;

  if (FAILED(DirectDrawCreate(v20.pGUID, &pDirectDraw, 0)))
  {
    delete [] v20.pDriverName;
    delete [] v20.pDeviceDesc;
    delete v20.pGUID;
  }
  else
  {
    if (FAILED(pDirectDraw->QueryInterface(IID_IDirectDraw4, (LPVOID *)&pDirectDraw4)))
    {
      delete [] v20.pDriverName;
      delete [] v20.pDeviceDesc;
      delete v20.pGUID;
      v6 = (IUnknown *)pDirectDraw;
    }
    else
    {
      pDirectDraw->Release();
      if (FAILED( pDirectDraw4->GetDeviceIdentifier(&ddDevId, 1)))
        v20.pDDraw4DevDesc = 0;
      else
      {
        v7 = strlen(ddDevId.szDescription);
        v20.pDDraw4DevDesc = new char[v7 + 1];
        strcpy(v20.pDDraw4DevDesc, ddDevId.szDescription);
      }
      memset(&ddsCaps, 0, 0x10u);
      if (FAILED(pDirectDraw4->GetAvailableVidMem(&ddsCaps, (LPDWORD)&v20.uVideoMem, (LPDWORD)&uFreeVideoMem)))
        v20.uVideoMem = 0;
      memset(&v10, 0, 0x7Cu);
      v10.dwSize = 124;
      v10.dwFlags = 6;
      v10.dwHeight = window->GetWidth();
      v10.dwWidth = window->GetHeight();
      v10.ddpfPixelFormat.dwSize = 32;

      v19 = 0;
      if ( FAILED(pDirectDraw4->EnumDisplayModes(0, 0, &v19, (LPDDENUMMODESCALLBACK2)DDrawDisplayModesEnumerator))
        || !v19
        || FAILED(pDirectDraw4->QueryInterface(IID_IDirect3D3, (LPVOID *)&pDirect3D3)))
      {
        delete [] v20.pDriverName;
        delete [] v20.pDeviceDesc;
        free(v20.pDDraw4DevDesc);
        delete v20.pGUID;
        v6 = (IUnknown *)pDirectDraw4;
      }
      else
      {
        aux.pInfo = pOut;
        aux.ptr_4 = &v20;
        pDirect3D3->EnumDevices((LPD3DENUMDEVICESCALLBACK)D3DDeviceEnumerator, &aux);
        delete [] v20.pDriverName;
        delete [] v20.pDeviceDesc;
        free(v20.pDDraw4DevDesc);
        delete v20.pGUID;
        pDirectDraw4->Release();
        v6 = (IUnknown *)pDirect3D3;
        pDirectDraw4 = 0;
      }
    }
    v6->Release();
  }
  return 1;
}

//----- (0049D784) --------------------------------------------------------
HRESULT __stdcall D3DDeviceEnumerator(const GUID *lpGUID, const char *lpDeviceDesc, const char *lpDeviceName, D3DDEVICEDESC *pHWDesc, D3DDEVICEDESC *pSWDesc, RenderD3D_aux *a6)
{
  int v6; // eax@1
  signed int v7; // edi@1
  signed int v8; // edi@14
  size_t v9; // eax@30
  size_t v10; // eax@30
  size_t v11; // eax@30
  size_t v12; // eax@30
  size_t v13; // eax@30

  v6 = pHWDesc->dwFlags;
  v7 = -1;
  if ( v6 )
  {
    if ( !a6->ptr_4->pGUID )
      v7 = 0;
    if ( v6 && a6->ptr_4->pGUID )
      v7 = 1;
  }
  if ( !strcmp(lpDeviceName, "RGB Emulation") && !a6->ptr_4->pGUID )
    v7 = 2;
  if ( !strcmp(lpDeviceName, "Reference Rasterizer") && !a6->ptr_4->pGUID )
    v7 = 3;
  if ( v7 != -1 )
  {
    v8 = v7;
    a6->pInfo[v8].bIsDeviceCompatible = 1;
    a6->pInfo[v8].uCaps = 0;
    if ( !(pHWDesc->dpcTriCaps.dwSrcBlendCaps & 0x10) )
      a6->pInfo[v8].uCaps |= 2u;
    if ( !(pHWDesc->dpcTriCaps.dwSrcBlendCaps & 2) )
      a6->pInfo[v8].uCaps |= 4u;
    if ( !(pHWDesc->dpcTriCaps.dwSrcBlendCaps & 1) )
      a6->pInfo[v8].uCaps |= 8u;
    if ( !(pHWDesc->dpcTriCaps.dwDestBlendCaps & 0x20) )
      a6->pInfo[v8].uCaps |= 0x10u;
    if ( !(pHWDesc->dpcTriCaps.dwDestBlendCaps & 2) )
      a6->pInfo[v8].uCaps |= 0x20u;
    if ( !(pHWDesc->dpcTriCaps.dwDestBlendCaps & 4) )
      a6->pInfo[v8].uCaps |= 0x40u;
    if ( !(BYTE1(pHWDesc->dwDevCaps) & 0x10) )
      BYTE1(a6->pInfo[v8].uCaps) |= 1u;
    if ( pHWDesc->dpcTriCaps.dwTextureCaps & 0x20 )
      LOBYTE(a6->pInfo[v8].uCaps) |= 0x80u;
    v9 = strlen(lpDeviceName);
    a6->pInfo[v8].pName = new char[v9 + 1];
    strcpy(a6->pInfo[v8].pName, lpDeviceName);
    v10 = strlen(lpDeviceDesc);
    a6->pInfo[v8].pDescription = new char[v10 + 1];
    strcpy(a6->pInfo[v8].pDescription, lpDeviceDesc);
    a6->pInfo[v8].pGUID = new GUID;
    memcpy(a6->pInfo[v8].pGUID, lpGUID, 0x10u);
    v11 = strlen(a6->ptr_4->pDriverName);
    a6->pInfo[v8].pDriverName = new char[v11 + 1];
    strcpy(a6->pInfo[v8].pDriverName, a6->ptr_4->pDriverName);
    v12 = strlen(a6->ptr_4->pDeviceDesc);
    a6->pInfo[v8].pDeviceDesc = new char[v12 + 1];
    strcpy(a6->pInfo[v8].pDeviceDesc, a6->ptr_4->pDeviceDesc);
    v13 = strlen(a6->ptr_4->pDDraw4DevDesc);
    a6->pInfo[v8].pDDraw4DevDesc = new char[v13 + 1];
    strcpy(a6->pInfo[v8].pDDraw4DevDesc, a6->ptr_4->pDDraw4DevDesc);
    if ( a6->ptr_4->pGUID )
    {
      a6->pInfo[v8].pDirectDrawGUID = new GUID;
      memcpy(a6->pInfo[v8].pDirectDrawGUID, a6->ptr_4->pGUID, 0x10u);
    }
    else
    {
      a6->pInfo[v8].pDirectDrawGUID = 0;
    }
    a6->pInfo[v8].uVideoMem = a6->ptr_4->uVideoMem;
  }
  return 1;
}

//----- (0049D75C) --------------------------------------------------------
HRESULT __stdcall DDrawDisplayModesEnumerator(DDSURFACEDESC2 *pSurfaceDesc, __int16 *a2)
{
  HRESULT result; // eax@3

  if ( pSurfaceDesc->ddsCaps.dwCaps | 0x2000 && pSurfaceDesc->ddpfPixelFormat.dwRGBBitCount == 16 )
  {
    *a2 = 1;
    result = 0;
  }
  else
    result = 1;
  return result;
}

//----- (0047A95E) --------------------------------------------------------
void Render::PrepareDecorationsRenderList_ODM()
{
  unsigned int v6; // edi@9
  int v7; // eax@9
  SpriteFrame *v8; // eax@9
  unsigned __int16 *v10; // eax@9
  int v13; // ecx@9
  int v14; // ecx@20
  char v15; // dl@20
  signed int v16; // eax@20
  int v17; // eax@23
  int v18; // ecx@24
  int v19; // eax@24
  int v20; // ecx@24
  int v21; // ebx@26
  int v22; // eax@26
  signed __int64 v24; // qtt@31
  int v25; // ebx@31
  __int16 v29; // cx@37
  int v30; // ecx@37
  int v31; // ebx@37
  Particle_sw local_0; // [sp+Ch] [bp-98h]@7
  unsigned __int16 *v37; // [sp+84h] [bp-20h]@9
  int v38; // [sp+88h] [bp-1Ch]@9
  int v40; // [sp+90h] [bp-14h]@24
  int v41; // [sp+94h] [bp-10h]@24
  int v42; // [sp+98h] [bp-Ch]@9
  int b; // [sp+A0h] [bp-4h]@22

  for (int i = 0; i < uNumLevelDecorations; ++i)
  {
    //LevelDecoration* decor = &pLevelDecorations[i];
    if ((!(pLevelDecorations[i].uFlags & LEVEL_DECORATION_OBELISK_CHEST)
        || pLevelDecorations[i].IsObeliskChestActive()) && !(pLevelDecorations[i].uFlags & LEVEL_DECORATION_INVISIBLE))
    {
      DecorationDesc* decor_desc = &pDecorationList->pDecorations[pLevelDecorations[i].uDecorationDescID];
      if ( (char)decor_desc->uFlags >= 0 )
      {
        if ( !(decor_desc->uFlags & 0x22) )
        {
          v6 = pMiscTimer->uTotalGameTimeElapsed;
          v7 = abs(pLevelDecorations[i].vPosition.x + pLevelDecorations[i].vPosition.y);

          #pragma region "New: seasons change"
          extern bool change_seasons;
          if (change_seasons)
            switch (pParty->uCurrentMonth)
            {
              // case 531 (tree60), 536 (tree65), 537 (tree66) have no autumn/winter sprites
              case 11: case 0: case 1: // winter
                switch (decor_desc->uSpriteID)
                {
                  //case 468: //bush02    grows on swamps, which are evergreeen actually
                  case 548:             // flower10
                  case 547:             // flower09
                  case 541:             // flower03
                  case 539: continue;   // flower01

                  case 483:             // tree01
                  case 486:             // tree04
                  case 492:             // tree10
                    pSpriteFrameTable->InitializeSprite(decor_desc->uSpriteID + 2);
                    v8 = pSpriteFrameTable->GetFrame(decor_desc->uSpriteID + 2, v6 + v7);
                  break;

                  default:
                    v8 = pSpriteFrameTable->GetFrame(decor_desc->uSpriteID, v6 + v7);
                }
              break;

              case 2: case 3: case 4: // spring
                switch (decor_desc->uSpriteID)
                {
                }
                v8 = pSpriteFrameTable->GetFrame(decor_desc->uSpriteID, v6 + v7);
              break;

              case 8: case 9: case 10: // autumn
                switch (decor_desc->uSpriteID)
                {
                  //case 468: //bush02    grows on swamps, which are evergreeen actually
                  case 548:             // flower10
                  case 547:             // flower09
                  case 541:             // flower03
                  case 539: continue;   // flower01
                  
                  case 483:             // tree01
                  case 486:             // tree04
                  case 492:             // tree10
                    pSpriteFrameTable->InitializeSprite(decor_desc->uSpriteID + 1);
                    v8 = pSpriteFrameTable->GetFrame(decor_desc->uSpriteID + 1, v6 + v7);
                  break;

                  default:
                    v8 = pSpriteFrameTable->GetFrame(decor_desc->uSpriteID, v6 + v7);
                }
              break;

              case 5: case 6: case 7: // summer
                //all green by default
                v8 = pSpriteFrameTable->GetFrame(decor_desc->uSpriteID, v6 + v7);
              break;

              default: assert(pParty->uCurrentMonth >= 0 && pParty->uCurrentMonth < 12);
            }
          else
            v8 = pSpriteFrameTable->GetFrame(decor_desc->uSpriteID, v6 + v7);
          #pragma endregion
          //v8 = pSpriteFrameTable->GetFrame(decor_desc->uSpriteID, v6 + v7);

          v10 = (unsigned __int16 *)stru_5C6E00->Atan2(pLevelDecorations[i].vPosition.x - pGame->pIndoorCameraD3D->vPartyPos.x,
                                                       pLevelDecorations[i].vPosition.y - pGame->pIndoorCameraD3D->vPartyPos.y);
          v38 = 0;
          v13 = ((signed int)(stru_5C6E00->uIntegerPi + ((signed int)stru_5C6E00->uIntegerPi >> 3) + pLevelDecorations[i].field_10_y_rot - (signed int)v10) >> 8) & 7;
          v37 = (unsigned __int16 *)v13;
          if ( v8->uFlags & 2 )
            v38 = 2;
          if ( (256 << v13) & v8->uFlags )
            v38 |= 4;
          if ( v8->uFlags & 0x40000 )
            v38 |= 0x40;
          if ( v8->uFlags & 0x20000 )
            LOBYTE(v38) = v38 | 0x80;
          if ( v8->uGlowRadius )
          {
            if ( pRenderer->pRenderD3D && pRenderer->bUseColoredLights )
            {
              v14 = 255;//decor_desc->uColoredLightRed;
              v15 = 255;//decor_desc->uColoredLightGreen;
              v16 = 255;//decor_desc->uColoredLightBlue;
            }
            else
            {
              v16 = 255;
              v14 = 255;
              v15 = 255;
            }
            pStationaryLightsStack->AddLight(pLevelDecorations[i].vPosition.x, pLevelDecorations[i].vPosition.y, pLevelDecorations[i].vPosition.z + decor_desc->uDecorationHeight / 2,
              v8->uGlowRadius, v14, v15, v16, _4E94D0_light_type);
          }
          v17 = (pLevelDecorations[i].vPosition.x - pGame->pIndoorCameraD3D->vPartyPos.x) << 16;
          if (pGame->pIndoorCameraD3D->sRotationX)
          {
            v40 = (pLevelDecorations[i].vPosition.y - pGame->pIndoorCameraD3D->vPartyPos.y) << 16;
            v18 = fixpoint_mul(v17, pGame->pIndoorCameraD3D->int_cosine_y) + fixpoint_mul(v40, pGame->pIndoorCameraD3D->int_sine_y);
            v41 = fixpoint_mul((pLevelDecorations[i].vPosition.z - pGame->pIndoorCameraD3D->vPartyPos.z) << 16, pGame->pIndoorCameraD3D->int_sine_x);
            v19 = fixpoint_mul(v18, pGame->pIndoorCameraD3D->int_cosine_x);
            v20 = v19 + fixpoint_mul((pLevelDecorations[i].vPosition.z - pGame->pIndoorCameraD3D->vPartyPos.z) << 16, pGame->pIndoorCameraD3D->int_sine_x);
            if ( v20 >= 0x40000 && v20 <= pODMRenderParams->shading_dist_mist << 16 )
            {
              v21 = fixpoint_mul(v40, pGame->pIndoorCameraD3D->int_cosine_y) - fixpoint_mul(v17, pGame->pIndoorCameraD3D->int_sine_y);
              v22 = fixpoint_mul((pLevelDecorations[i].vPosition.z - pGame->pIndoorCameraD3D->vPartyPos.z) << 16, pGame->pIndoorCameraD3D->int_cosine_x) - fixpoint_mul(v18, pGame->pIndoorCameraD3D->int_sine_x);
              if ( 2 * abs(v20) >= abs(v21) )
              {
                LODWORD(v24) = 0;
                HIDWORD(v24) = SLOWORD(pODMRenderParams->int_fov_rad);
                v25 = pViewport->uScreenCenterX - ((signed int)(fixpoint_mul(v24 / v20, v21) + 0x8000) >> 16);
                v40 = pViewport->uScreenCenterY - ((signed int)(fixpoint_mul(v24 / v20, v22) + 0x8000) >> 16);
                v41 = fixpoint_mul(v8->scale, v24 / v20);
                if ( pRenderer->pRenderD3D )
                  b = fixpoint_mul(pSprites_LOD->pHardwareSprites[v8->pHwSpriteIDs[(int)v37]].uBufferWidth / 2, v41);
                else
                  b = fixpoint_mul(pSprites_LOD->pSpriteHeaders[v8->pHwSpriteIDs[(int)v37]].uWidth / 2, v41);
                if ( b + v25 >= (signed int)pViewport->uViewportTL_X && v25 - b <= (signed int)pViewport->uViewportBR_X )
                {
                  if (::uNumBillboardsToDraw >= 500)
                    return;
                  pBillboardRenderList[::uNumBillboardsToDraw].HwSpriteID = v8->pHwSpriteIDs[(int)v37];
                  pBillboardRenderList[::uNumBillboardsToDraw]._screenspace_x_scaler_packedfloat = v41;
                  pBillboardRenderList[::uNumBillboardsToDraw]._screenspace_y_scaler_packedfloat = v41;
                  v29 = v38;
                  pBillboardRenderList[::uNumBillboardsToDraw].uScreenSpaceX = v25;
                  HIBYTE(v29) |= 2;
                  pBillboardRenderList[::uNumBillboardsToDraw].uPalette = v8->uPaletteIndex;
                  pBillboardRenderList[::uNumBillboardsToDraw].field_1E = v29;
                  pBillboardRenderList[::uNumBillboardsToDraw].world_x = pLevelDecorations[i].vPosition.x;
                  pBillboardRenderList[::uNumBillboardsToDraw].world_y = pLevelDecorations[i].vPosition.y;
                  pBillboardRenderList[::uNumBillboardsToDraw].world_z = pLevelDecorations[i].vPosition.z;
                  pBillboardRenderList[::uNumBillboardsToDraw].uScreenSpaceY = v40;
                  HIWORD(v30) = HIWORD(v20);
                  v31 = PID(OBJECT_Decoration,i);
                  LOWORD(v30) = 0;
                  pBillboardRenderList[::uNumBillboardsToDraw].uIndoorSectorID = 0;
                  pBillboardRenderList[::uNumBillboardsToDraw].sZValue = v30 + v31;
                  pBillboardRenderList[::uNumBillboardsToDraw].dimming_level = 0;
                  pBillboardRenderList[::uNumBillboardsToDraw].pSpriteFrame = v8;
                  pBillboardRenderList[::uNumBillboardsToDraw].sTintColor = 0;
                  ::uNumBillboardsToDraw++;
                  ++uNumDecorationsDrawnThisFrame;
                }
              }
              continue;
            }
          }
          else
          {
            v42 = (pLevelDecorations[i].vPosition.x - pGame->pIndoorCameraD3D->vPartyPos.x) << 16;
            v40 = (pLevelDecorations[i].vPosition.y - pGame->pIndoorCameraD3D->vPartyPos.y) << 16;
            v20 = fixpoint_mul(v17, pGame->pIndoorCameraD3D->int_cosine_y) + fixpoint_mul(v40, pGame->pIndoorCameraD3D->int_sine_y);
            if ( v20 >= 0x40000 && v20 <= pODMRenderParams->shading_dist_mist << 16 )
            {
              v21 = fixpoint_mul(v40, pGame->pIndoorCameraD3D->int_cosine_y) - fixpoint_mul(v42, pGame->pIndoorCameraD3D->int_sine_y);
              v22 = (pLevelDecorations[i].vPosition.z - pGame->pIndoorCameraD3D->vPartyPos.z) << 16;
              v42 = v22;
              if ( 2 * abs(v20) >= abs(v21) )
              {
                LODWORD(v24) = 0;
                HIDWORD(v24) = SLOWORD(pODMRenderParams->int_fov_rad);
                v25 = pViewport->uScreenCenterX - ((signed int)(fixpoint_mul(v24 / v20, v21) + 0x8000) >> 16);
                v40 = pViewport->uScreenCenterY - ((signed int)(fixpoint_mul(v24 / v20, v42) + 0x8000) >> 16);
                v41 = fixpoint_mul(v8->scale, v24 / v20);
                if ( pRenderer->pRenderD3D )
                  b = fixpoint_mul(pSprites_LOD->pHardwareSprites[v8->pHwSpriteIDs[(int)v37]].uBufferWidth / 2, v41);
                else
                  b = fixpoint_mul(pSprites_LOD->pSpriteHeaders[v8->pHwSpriteIDs[(int)v37]].uWidth / 2, v41);
                if ( b + v25 >= (signed int)pViewport->uViewportTL_X && v25 - b <= (signed int)pViewport->uViewportBR_X )
                {
                  if (::uNumBillboardsToDraw >= 500)
                    return;
                  pBillboardRenderList[::uNumBillboardsToDraw].HwSpriteID = v8->pHwSpriteIDs[(int)v37];
                  pBillboardRenderList[::uNumBillboardsToDraw]._screenspace_x_scaler_packedfloat = v41;
                  pBillboardRenderList[::uNumBillboardsToDraw]._screenspace_y_scaler_packedfloat = v41;
                  v29 = v38;
                  pBillboardRenderList[::uNumBillboardsToDraw].uScreenSpaceX = v25;
                  HIBYTE(v29) |= 2;
                  pBillboardRenderList[::uNumBillboardsToDraw].uPalette = v8->uPaletteIndex;
                  pBillboardRenderList[::uNumBillboardsToDraw].field_1E = v29;
                  pBillboardRenderList[::uNumBillboardsToDraw].world_x = pLevelDecorations[i].vPosition.x;
                  pBillboardRenderList[::uNumBillboardsToDraw].world_y = pLevelDecorations[i].vPosition.y;
                  pBillboardRenderList[::uNumBillboardsToDraw].world_z = pLevelDecorations[i].vPosition.z;
                  pBillboardRenderList[::uNumBillboardsToDraw].uScreenSpaceY = v40;
                  HIWORD(v30) = HIWORD(v20);
                  v31 = PID(OBJECT_Decoration,i);
                  LOWORD(v30) = 0;
                  pBillboardRenderList[::uNumBillboardsToDraw].uIndoorSectorID = 0;
                  pBillboardRenderList[::uNumBillboardsToDraw].sZValue = v30 + v31;
                  pBillboardRenderList[::uNumBillboardsToDraw].dimming_level = 0;
                  pBillboardRenderList[::uNumBillboardsToDraw].pSpriteFrame = v8;
                  pBillboardRenderList[::uNumBillboardsToDraw].sTintColor = 0;
                  ::uNumBillboardsToDraw++;
                  ++uNumDecorationsDrawnThisFrame;
                }
              }
              continue;
            }
          }
        }
      }
      else
      {
        memset(&local_0, 0, 0x68);
        local_0.type = ParticleType_Bitmap | ParticleType_Rotating | ParticleType_8;
        local_0.uDiffuse = 0xFF3C1E;
        local_0.x = (double)pLevelDecorations[i].vPosition.x;
        local_0.y = (double)pLevelDecorations[i].vPosition.y;
        local_0.z = (double)pLevelDecorations[i].vPosition.z;
        local_0.r = 0.0;
        local_0.g = 0.0;
        local_0.b = 0.0;
        local_0.flt_28 = 1.0;
        local_0.timeToLive = (rand() & 0x80) + 128;
        local_0.uTextureID = pBitmaps_LOD->LoadTexture("effpar01");
        pGame->pParticleEngine->AddParticle(&local_0);
      }
    }
  }
}

//----- (0049D717) --------------------------------------------------------
HRESULT __stdcall D3DZBufferFormatEnumerator(DDPIXELFORMAT *Src, DDPIXELFORMAT *Dst)
{
  if ( Src->dwFlags & (0x400 | 0x2000))
  {
    if ( Src->dwRGBBitCount == 16 && !Src->dwRBitMask )
    {
      memcpy(Dst, Src, sizeof(DDPIXELFORMAT));
      return 0;
    }
    if ( !Dst->dwSize )
    {
      memcpy(Dst, Src, sizeof(DDPIXELFORMAT));
      return 1;
    }
  }
  return 1;
}

//----- (0049DC28) --------------------------------------------------------
void RenderD3D::GetAvailableDevices(RenderD3D__DevInfo **pOutDevices)
{
  RenderD3D__DevInfo *v2; // eax@1

  v2 = new RenderD3D__DevInfo[4];// 4 items
  *pOutDevices = v2;
  memset(v2, 0, 0xA0u);
  DirectDrawEnumerateExA((LPDDENUMCALLBACKEXA)RenderD3D__DeviceEnumerator, *pOutDevices, DDENUM_ATTACHEDSECONDARYDEVICES);
}

//----- (0049DC58) --------------------------------------------------------
RenderD3D::RenderD3D()
{
  this->pHost = nullptr;
  this->pDirect3D = nullptr;
  this->pUnk = nullptr;
  this->pBackBuffer = nullptr;
  this->pFrontBuffer = nullptr;
  this->pZBuffer = nullptr;
  this->pDevice = nullptr;
  this->pViewport = nullptr;
  this->field_40 = 1;
  this->field_44 = 10;
  GetAvailableDevices(&this->pAvailableDevices);
}

//----- (0049DC90) --------------------------------------------------------
void RenderD3D::Release()
{
  RenderD3D *v1; // esi@1
  IDirectDraw4 *v2; // eax@2
  signed int v3; // edi@4
  IDirect3DViewport3 *v4; // eax@22
  IUnknown *v5; // eax@24
  IDirectDrawSurface4 *v6; // eax@26
  IDirect3DDevice3 *v7; // eax@28
  IDirect3D3 *v8; // eax@30
  IDirectDrawSurface4 *v9; // eax@32
  IDirectDrawSurface4 *v10; // eax@34
  IDirectDraw4 *v11; // eax@36

  v1 = this;
  if ( !this->bWindowed )
  {
    v2 = this->pHost;
    if ( v2 )
    {
      v2->RestoreDisplayMode();
      v1->pHost->SetCooperativeLevel(v1->hWindow, 8u);
      v1->pHost->FlipToGDISurface();
    }
  }
  v3 = 0;
  do
  {
    free(v1->pAvailableDevices[v3].pDriverName);
    v1->pAvailableDevices[v3].pDriverName = nullptr;
    free(v1->pAvailableDevices[v3].pDeviceDesc);
    v1->pAvailableDevices[v3].pDeviceDesc = nullptr;
    free(v1->pAvailableDevices[v3].pDDraw4DevDesc);
    v1->pAvailableDevices[v3].pDDraw4DevDesc = nullptr;
    free(v1->pAvailableDevices[v3].pDirectDrawGUID);
    v1->pAvailableDevices[v3].pDirectDrawGUID = nullptr;
    free(v1->pAvailableDevices[v3].pName);
    v1->pAvailableDevices[v3].pName = nullptr;
    free(v1->pAvailableDevices[v3].pDescription);
    v1->pAvailableDevices[v3].pDescription = nullptr;
    free(v1->pAvailableDevices[v3].pGUID);
    v1->pAvailableDevices[v3].pGUID = nullptr;
    ++v3;
  }
  while ( v3 < 4 );
  free(v1->pAvailableDevices);
  v1->pAvailableDevices = 0;
  v4 = v1->pViewport;
  if ( v4 )
  {
    v4->Release();
    v1->pViewport = 0;
  }
  v5 = v1->pUnk;
  if ( v5 )
  {
    v5->Release();
    v1->pUnk = 0;
  }
  v6 = v1->pZBuffer;
  if ( v6 )
  {
    v6->Release();
    v1->pZBuffer = 0;
  }
  v7 = v1->pDevice;
  if ( v7 )
  {
    v7->Release();
    v1->pDevice = 0;
  }
  v8 = v1->pDirect3D;
  if ( v8 )
  {
    v8->Release();
    v1->pDirect3D = 0;
  }
  v9 = v1->pBackBuffer;
  if ( v9 )
  {
    v9->Release();
    v1->pBackBuffer = 0;
  }
  v10 = v1->pFrontBuffer;
  if ( v10 )
  {
    v10->Release();
    v1->pFrontBuffer = 0;
  }
  v11 = v1->pHost;
  if ( v11 )
  {
    v11->Release();
    v1->pHost = 0;
  }
}

//----- (0049DE14) --------------------------------------------------------
bool RenderD3D::CreateDevice(unsigned int uDeviceID, int bWindowed, OSWindow *window)
{
  //IDirectDraw4 *v8; // eax@12
  //IDirectDraw4 *v9; // eax@16
  //IDirectDraw4 *v10; // eax@20
  //IDirectDraw4 *v13; // eax@35
  const char *v23; // [sp-4h] [bp-DCh]@9
  const char *v24; // [sp-4h] [bp-DCh]@13
  const char *v25; // [sp-4h] [bp-DCh]@19
  DWORD v26; // [sp-4h] [bp-DCh]@30
  DDSCAPS2 v27; // [sp+Ch] [bp-CCh]@37
  DDSURFACEDESC2 ddsd2; // [sp+1Ch] [bp-BCh]@11
  D3DVIEWPORT2 d3dvp2; // [sp+98h] [bp-40h]@28
  IDirectDrawClipper *v30; // [sp+C4h] [bp-14h]@18
  LPDIRECTDRAW lpDD; // [sp+C8h] [bp-10h]@1

  auto hWnd = window->GetApiHandle();
  int game_width = window->GetWidth();
  int game_height = window->GetHeight();

  this->bWindowed = bWindowed;
  this->hWindow = hWnd;

  if (FAILED(DirectDrawCreate(pAvailableDevices[uDeviceID].pDirectDrawGUID, &lpDD, 0)))
  {
    sprintf(pErrorMessage, "Init - Failed to create DirectDraw interface.\n");
    return 0;
  }

  if (FAILED(lpDD->QueryInterface(IID_IDirectDraw4, (LPVOID *)&pHost)))
  {
    sprintf(pErrorMessage, "Init - Failed to create DirectDraw4 interface.\n");
    if (lpDD)
      lpDD->Release();
    return 0;
  }
  lpDD->Release();
  lpDD = 0;

  if (bWindowed && !pAvailableDevices[uDeviceID].pDirectDrawGUID)
  {
    if (FAILED(pHost->SetCooperativeLevel(hWnd, DDSCL_MULTITHREADED | DDSCL_NORMAL)))
    {
      v23 = "Init - Failed to set cooperative level.\n";
      sprintf(pErrorMessage, v23);
LABEL_65:
      if (pHost)
      {
        pHost->Release();
        pHost = 0;
      }
      return 0;
    }
    memset(&ddsd2, 0, sizeof(DDSURFACEDESC2));
    ddsd2.dwSize = sizeof(DDSURFACEDESC2);
    ddsd2.dwFlags = DDSD_CAPS;
    ddsd2.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    if ( !pHost->CreateSurface(&ddsd2, &pFrontBuffer, 0) )
    {
      ddsd2.dwSize = sizeof(DDSURFACEDESC2);
      pHost->GetDisplayMode(&ddsd2);
      if ( FORCE_16_BITS && ddsd2.ddpfPixelFormat.dwRGBBitCount != 16 )
      {
        v24 = "Init - Desktop isn't in 16 bit mode.\n";
        goto LABEL_14;
      }

      ddsd2.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
      ddsd2.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;
      ddsd2.dwWidth = game_width;
      ddsd2.dwHeight = game_height;
      if (pHost->CreateSurface(&ddsd2, &pBackBuffer, 0) )
      {
        v24 = "Init - Failed to create back buffer.\n";
LABEL_14:
        sprintf(pErrorMessage, v24);
        if (pFrontBuffer)
        {
          pFrontBuffer->Release();
          pFrontBuffer = 0;
        }
        goto LABEL_65;
      }
      if ( pHost->CreateClipper(0, &v30, 0) )
      {
        v25 = "Init - Failed to create clipper.\n";
        goto LABEL_45;
      }
      v30->SetHWnd(0, hWnd);
      pFrontBuffer->SetClipper(v30);

      v30->Release();
      v30 = 0;

      pHost->QueryInterface(IID_IDirect3D3, (LPVOID *)&pDirect3D);

      ddsd2.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
      ddsd2.ddsCaps.dwCaps = DDSCAPS_ZBUFFER;
      ddsd2.dwWidth = game_width;
      ddsd2.dwHeight = game_height;

      if ( pDirect3D->EnumZBufferFormats(*pAvailableDevices[uDeviceID].pGUID,
             (HRESULT (__stdcall *)(DDPIXELFORMAT *, void *))D3DZBufferFormatEnumerator,
             &ddsd2.ddpfPixelFormat) )
        goto LABEL_21;
      if ( uDeviceID == 2 || uDeviceID == 3 )
        ddsd2.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;

      if ( !pHost->CreateSurface(&ddsd2, &pZBuffer, 0) )
      {
        if ( !pBackBuffer->AddAttachedSurface(pZBuffer) )
        {
          if ( !pDirect3D->CreateDevice(*pAvailableDevices[uDeviceID].pGUID,
                  pBackBuffer,
                  &pDevice,
                  0) )
          {
            memset(&d3dvp2, 0, sizeof(D3DVIEWPORT2));
            d3dvp2.dvClipWidth = 2.0;
            d3dvp2.dvClipY = 1.0;
            d3dvp2.dvClipHeight = 2.0;
            d3dvp2.dvMaxZ = 1.0;
            d3dvp2.dvMinZ = 0.0;
            goto LABEL_54;
          }
LABEL_51:
          sprintf(pErrorMessage, "Init - Failed to create D3D device.\n");
          if (pDirect3D)
          {
            pDirect3D->Release();
            pDirect3D = 0;
          }
          goto LABEL_59;
        }
LABEL_48:
        sprintf(pErrorMessage, "Init - Failed to attach z-buffer to back buffer.\n");
        if (pZBuffer)
        {
          pZBuffer->Release();
          pZBuffer = 0;
        }
        goto LABEL_61;
      }
      goto LABEL_44;
    }
LABEL_36:
    v23 = "Init - Failed to create front buffer.\n";
    sprintf(pErrorMessage, v23);
    goto LABEL_65;
  }
  if ( uDeviceID == 1 )
    v26 = 1045;
  else
    v26 = 1041;
  if (pHost->SetCooperativeLevel(hWnd, v26) )
  {
    v23 = "Init - Failed to set cooperative level.\n";
    sprintf(pErrorMessage, v23);
    goto LABEL_65;
  }
  if (pHost->SetDisplayMode(window->GetWidth(), window->GetHeight(), 16, 0, 0) )
  {
    v23 = "Init - Failed to set display mode.\n";
    sprintf(pErrorMessage, v23);
    goto LABEL_65;
  }

  memset(&ddsd2, 0, sizeof(DDSURFACEDESC2));
  ddsd2.dwSize = sizeof(DDSURFACEDESC2);
  ddsd2.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
  ddsd2.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_3DDEVICE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
  ddsd2.dwBackBufferCount = 1;
  if ( pHost->CreateSurface(&ddsd2, &pFrontBuffer, 0) )
    goto LABEL_36;
  //a3a = &pBackBuffer;
  //v14 = *v34;
  memset(&v27, 0, sizeof(DDSCAPS2));
  v27.dwCaps = DDSCAPS_BACKBUFFER;
  //v33 = (IDirect3DDevice3 **)v14->GetAttachedSurface(&v27, &pBackBuffer);
  //hWnda = &pDirect3D;
  pHost->QueryInterface(IID_IDirect3D3, (LPVOID *)&pDirect3D);
  if (FAILED(pFrontBuffer->GetAttachedSurface(&v27, &pBackBuffer)))
  {
    v25 = "Init - Failed to get D3D interface.\n";
    goto LABEL_45;
  }

  ddsd2.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
  ddsd2.ddsCaps.dwCaps = DDSCAPS_ZBUFFER;
  ddsd2.dwWidth = 640;
  ddsd2.dwHeight = 480;
  if ( pDirect3D->EnumZBufferFormats(*pAvailableDevices[uDeviceID].pGUID,
         (HRESULT (__stdcall *)(DDPIXELFORMAT *, void *))D3DZBufferFormatEnumerator,
         &ddsd2.ddpfPixelFormat) )
  {
LABEL_21:
    v25 = "Init - Failed to enumerate Z buffer formats.\n";
    goto LABEL_45;
  }
  if ( uDeviceID == 2 || uDeviceID == 3 )
    BYTE1(ddsd2.ddsCaps.dwCaps) |= 8u;
  //uDeviceIDa = &pZBuffer;
  if (pHost->CreateSurface(&ddsd2, &pZBuffer, 0) )
  {
LABEL_44:
    v25 = "Init - Failed to create z-buffer.\n";
LABEL_45:
    sprintf(pErrorMessage, v25);
    if (pBackBuffer)
    {
      pBackBuffer->Release();
      pBackBuffer = 0;
    }
LABEL_63:
    //v19 = &pFrontBuffer;
    if (pFrontBuffer)
    {
      pFrontBuffer->Release();
      pFrontBuffer= 0;
    }
    goto LABEL_65;
  }
  if (pBackBuffer->AddAttachedSurface(pZBuffer))
    goto LABEL_48;
  //v33 = &pDevice;
  if (pDirect3D->CreateDevice(*pAvailableDevices[uDeviceID].pGUID,
         pBackBuffer,
         &pDevice,
         0) )
    goto LABEL_51;
  memset(&d3dvp2, 0, sizeof(D3DVIEWPORT2));
  d3dvp2.dvClipWidth = 2.0;
  d3dvp2.dvClipY = 1.0;
  d3dvp2.dvClipHeight = 2.0;
  d3dvp2.dvMaxZ = 1.0;

LABEL_54:
  d3dvp2.dwSize = sizeof(D3DVIEWPORT2);
  //v17 = *hWnda;
  d3dvp2.dwWidth = game_width;
  d3dvp2.dwHeight = game_height;
  d3dvp2.dvClipX = -1.0;
  //v18 = v17->lpVtbl;
  //v32 = &v4->pViewport;
  if (pDirect3D->CreateViewport(&pViewport, 0))
  {
    sprintf(pErrorMessage, "Init - Failed to create viewport.\n");
    if (pDevice)
    {
      pDevice->Release();
      pDevice = 0;
    }
    if (pDirect3D)
    {
      pDirect3D->Release();
      pDirect3D = 0;
    }
LABEL_59:
    if (pZBuffer)
    {
      pZBuffer->Release();
      pZBuffer = 0;
    }
LABEL_61:
    if (pBackBuffer)
    {
      pBackBuffer->Release();
      pBackBuffer = 0;
    }
    goto LABEL_63;
  }

  pDevice->AddViewport(pViewport);
  pViewport->SetViewport2(&d3dvp2);
  pDevice->SetCurrentViewport(pViewport);
  return 1;
}

//----- (0049E444) --------------------------------------------------------
unsigned int RenderD3D::GetDeviceCaps()
{
  unsigned int v1; // ebx@1
  RenderD3D *v2; // edi@1
  IDirect3DDevice3 *v3; // eax@1
  unsigned int result; // eax@2
  D3DDEVICEDESC refCaps; // [sp+Ch] [bp-1F8h]@1
  D3DDEVICEDESC halCaps; // [sp+108h] [bp-FCh]@1

  v1 = 0;
  v2 = this;
  memset(&halCaps, 0, 0xFCu);
  halCaps.dwSize = 252;
  memset(&refCaps, 0, 0xFCu);
  v3 = v2->pDevice;
  refCaps.dwSize = 252;
  if ( v3->GetCaps(&halCaps, &refCaps) )
  {
    result = 1;
  }
  else
  {
    if ( !(halCaps.dpcTriCaps.dwSrcBlendCaps & D3DPBLENDCAPS_SRCALPHA) )
      v1 = 2;
    if ( !(halCaps.dpcTriCaps.dwSrcBlendCaps & D3DPBLENDCAPS_ONE) )
      v1 |= 4u;
    if ( !(halCaps.dpcTriCaps.dwSrcBlendCaps & D3DPBLENDCAPS_ZERO) )
      v1 |= 8u;
    if ( !(halCaps.dpcTriCaps.dwDestBlendCaps & D3DPBLENDCAPS_INVSRCALPHA) )
      v1 |= 0x10u;
    if ( !(halCaps.dpcTriCaps.dwDestBlendCaps & D3DPBLENDCAPS_ONE) )
      v1 |= 0x20u;
    if ( !(halCaps.dpcTriCaps.dwDestBlendCaps & D3DPBLENDCAPS_SRCCOLOR) )
      v1 |= 0x40u;
    if ( halCaps.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_SQUAREONLY )
      v1 |= 0x80;
    result = v1;
  }
  return result;
}

//----- (0049E4FC) --------------------------------------------------------
void RenderD3D::ClearTarget(unsigned int bClearColor, unsigned int uClearColor, unsigned int bClearDepth, float z_clear)
{
  uint uClearFlags = 0;

  if (bClearColor)
    uClearFlags |= D3DCLEAR_TARGET;
  if (bClearDepth)
    uClearFlags |= D3DCLEAR_ZBUFFER;
  
  D3DRECT rects[] = {{0, 0, window->GetWidth(), window->GetHeight()}};
  if (uClearFlags)
    pViewport->Clear2(1, rects, uClearFlags, uClearColor, z_clear, 0);
}

//----- (0049E54D) --------------------------------------------------------
void RenderD3D::Present(bool bForceBlit)
{
  RECT v5; // [sp+18h] [bp-18h]@1
  struct tagPOINT Point; // [sp+28h] [bp-8h]@4

  v5.left = 0;
  v5.top = 0;
  v5.bottom = 480;//window->GetHeight(); //Ritor1: проблема с кнопкой "развернуть"
  v5.right = 640; //window->GetWidth();

  if (bWindowed || bForceBlit)
  {
    RECT rc;
    GetClientRect(hWindow, &rc);
    Point.y = 0;
    Point.x = 0;
    ClientToScreen(hWindow, &Point);
    OffsetRect(&rc, Point.x, Point.y);
    pFrontBuffer->Blt(&rc, pBackBuffer, &v5, DDBLT_WAIT, 0);
  }
  else
    pFrontBuffer->Flip(0, 1);
}

//----- (0049E5D4) --------------------------------------------------------
bool RenderD3D::CreateTexture(unsigned int uTextureWidth, unsigned int uTextureHeight, IDirectDrawSurface4 **pOutSurface, IDirect3DTexture2 **pOutTexture, bool bAlphaChannel, bool bMipmaps, unsigned int uMinDeviceTexDim)
{
  unsigned int v8; // edx@4
  unsigned int v9; // ebx@5
  unsigned int v10; // eax@5
  DWORD v11; // edx@5
  //int v12; // edx@7
  DDSURFACEDESC2 ddsd2; // [sp+Ch] [bp-80h]@1
  //RenderD3D *v15; // [sp+88h] [bp-4h]@1

  //v15 = this;
  memset(&ddsd2, 0, 0x7Cu);
  ddsd2.dwSize = 0x7Cu;
  ddsd2.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
  ddsd2.ddsCaps.dwCaps = DDSCAPS_TEXTURE;
  ddsd2.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
  ddsd2.dwHeight = uTextureHeight;
  ddsd2.dwWidth = uTextureWidth;
  if ( bMipmaps )
  {
    if ( (signed int)uTextureHeight <= (signed int)uTextureWidth )
    {
      v8 = GetMaxMipLevels(uTextureHeight) - GetMaxMipLevels(uMinDeviceTexDim);
LABEL_8:
      ddsd2.dwMipMapCount = v8;
      if ( !v8 )
        goto LABEL_12;
      goto LABEL_11;
    }
    if ( (signed int)uTextureWidth < (signed int)uMinDeviceTexDim )
    {
      v8 = GetMaxMipLevels(uMinDeviceTexDim);
      goto LABEL_8;
    }
    v9 = GetMaxMipLevels(uTextureWidth);
    v10 = GetMaxMipLevels(uMinDeviceTexDim);
    ddsd2.dwMipMapCount = v9 - v10;
    if ( v9 == v10 )
    {
      ddsd2.dwFlags = 0x1007u;
      __debugbreak(); // warning C4700: uninitialized local variable 'v11' used
      ddsd2.ddsCaps.dwCaps = v11;
      goto LABEL_12;
    }
  }
  else
  {
    ddsd2.dwMipMapCount = 1;
  }
LABEL_11:
  ddsd2.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_MIPMAPCOUNT;
  ddsd2.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_COMPLEX | DDSCAPS_MIPMAP;
LABEL_12:
  ddsd2.ddpfPixelFormat.dwRGBBitCount = 16;
  ddsd2.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
  if (bAlphaChannel)
  {
    ddsd2.ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_ALPHAPIXELS;
    ddsd2.ddpfPixelFormat.dwRBitMask = 0x7C00;
    ddsd2.ddpfPixelFormat.dwGBitMask = 0x03E0;
    ddsd2.ddpfPixelFormat.dwBBitMask = 0x001F;
    ddsd2.ddpfPixelFormat.dwRGBAlphaBitMask = 0x8000u;
  }
  else
  {
    ddsd2.ddpfPixelFormat.dwFlags = DDPF_RGB;
    ddsd2.ddpfPixelFormat.dwRBitMask = 0xF800;
    ddsd2.ddpfPixelFormat.dwGBitMask = 0x07E0;
    ddsd2.ddpfPixelFormat.dwBBitMask = 0x001F;
    ddsd2.ddpfPixelFormat.dwRGBAlphaBitMask = 0;
  }
  if (FAILED(pHost->CreateSurface(&ddsd2, pOutSurface, 0)))
    return false;
  if (FAILED((*pOutSurface)->QueryInterface(IID_IDirect3DTexture2, (void **)pOutTexture)))
  {
    (*pOutSurface)->Release();
    *pOutSurface = 0;
    return false;
  }
  return true;
}

//----- (004A5190) --------------------------------------------------------
void RenderD3D::HandleLostResources()
{
  pBitmaps_LOD->ReleaseLostHardwareTextures();
  pBitmaps_LOD->_410423_move_textures_to_device();
  pSprites_LOD->ReleaseLostHardwareSprites();
}

//----- (004A2050) --------------------------------------------------------
void Render::DrawPolygon(unsigned int uNumVertices, struct Polygon *a3, ODMFace *a4, IDirect3DTexture2 *pTexture)
{
  Render *v5; // edi@1
  unsigned int v6; // ebx@1
  LightmapBuilder *v7; // esi@3
  int v8; // eax@7
//  ODMFace *v9; // eax@12
//  char *v10; // esi@12
//  double v11; // st7@14
//  double v12; // st7@14
//  int v13; // eax@14
//  ODMFace *v14; // ecx@14
//  double v15; // st7@14
//  float v16; // ST48_4@15
//  int v17; // eax@15
//  char v18; // zf@17
//  HRESULT v19; // eax@18
//  HRESULT v20; // eax@18
//  HRESULT v21; // eax@20
//  HRESULT v22; // eax@20
//  unsigned int v23; // ecx@20
//  char *v24; // eax@21
//  HRESULT v25; // eax@23
//  HRESULT v26; // eax@23
//  HRESULT v27; // eax@24
//  HRESULT v28; // eax@25
//  HRESULT v29; // eax@25
//  HRESULT v30; // eax@25
//  HRESULT v31; // eax@25
//  HRESULT v32; // eax@26
//  unsigned int v33; // ecx@26
//  char *v34; // eax@27
//  int v35; // edx@28
//  HRESULT v36; // eax@29
//  HRESULT v37; // eax@29
//  HRESULT v38; // eax@29
//  HRESULT v39; // eax@29
  //IDirect3DDevice3Vtbl *v40; // ebx@29
  unsigned int v41; // eax@29
//  HRESULT v42; // eax@30
//  HRESULT v43; // eax@30
//  HRESULT v44; // eax@30
//  char *v45; // esi@34
//  int v46; // ecx@35
//  double v47; // st6@35
//  int v48; // eax@36
//  const char *v49; // [sp+4Ch] [bp-1Ch]@0
//  const char *v50; // [sp+4Ch] [bp-1Ch]@20
//  int v51; // [sp+50h] [bp-18h]@0
//  unsigned int v52; // [sp+54h] [bp-14h]@0
  LightmapBuilder *v53; // [sp+58h] [bp-10h]@3
  unsigned int v54; // [sp+5Ch] [bp-Ch]@3
//  unsigned int v55; // [sp+5Ch] [bp-Ch]@34
//  unsigned int v56; // [sp+60h] [bp-8h]@12
//  int v57; // [sp+60h] [bp-8h]@34
  signed int a2; // [sp+64h] [bp-4h]@4

  v5 = this;
  v6 = 0;
  if ( this->uNumD3DSceneBegins && (signed int)uNumVertices >= 3 )
  {
    v7 = pGame->pLightmapBuilder;
    v53 = v7;
    v54 = v7->std__vector_000004_size;
    if ( v7->std__vector_000004_size)
      a2 = -1;
    pGame->AlterGamma_ODM(a4, &a2);
    if ( byte_4D864C && pGame->uFlags & GAME_FLAGS_1_01_lightmap_related)
    {
      v8 = ::GetActorTintColor(a3->dimming_level, 0, array_50AC10[0].vWorldViewPosition.x, 0, 0);
      v7->DrawLightmaps(/*v8, 0*/);
    }
    else
    {
      if ( !v54 || byte_4D864C && pGame->uFlags & 2 )
      {
        ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_WRAP));
        ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CW));
        if (bUsingSpecular)
        {
          ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE));
          ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE));
          ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ZERO));
        }
		for (uint i = 0; i < uNumVertices; ++i)
		{
		
		  d3d_vertex_buffer[i].pos.x = array_50AC10[i].vWorldViewProjX;
		  d3d_vertex_buffer[i].pos.y = array_50AC10[i].vWorldViewProjY;
		  d3d_vertex_buffer[i].pos.z = 1.0 - 1.0 / ((array_50AC10[i].vWorldViewPosition.x * 1000) / (double)pODMRenderParams->shading_dist_mist);
		  d3d_vertex_buffer[i].rhw = 1.0 / (array_50AC10[i].vWorldViewPosition.x + 0.0000001);
		  d3d_vertex_buffer[i].diffuse = ::GetActorTintColor(a3->dimming_level, 0, array_50AC10[i].vWorldViewPosition.x, 0, 0);
          pGame->AlterGamma_ODM(a4, &d3d_vertex_buffer[i].diffuse);

		  if ( this->bUsingSpecular )
          {
            d3d_vertex_buffer[i].specular = sub_47C3D7_get_fog_specular(0, 0, array_50AC10[i].vWorldViewPosition.x);
          }
          else
          {
            d3d_vertex_buffer[i].specular = 0;
		  }
		  d3d_vertex_buffer[i].texcoord.x = array_50AC10[i].u;
		  d3d_vertex_buffer[i].texcoord.y = array_50AC10[i].v;
		  
		}

        if (a4->uAttributes & FACE_OUTLINED)
        {
          int color;
          if (GetTickCount() % 300 >= 150)
            color = 0xFFFF2020;
          else color = 0xFF901010;

          for (uint i = 0; i < uNumVertices; ++i)
            d3d_vertex_buffer[i].diffuse = color;
        }

        pRenderD3D->pDevice->SetTexture(0, pTexture);
        pRenderD3D->pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,
                                           D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1,
                                           d3d_vertex_buffer,
                                           uNumVertices,
                                           D3DDP_DONOTLIGHT);
      }
      else
      {
		for (uint i = 0; i < uNumVertices; ++i)
		{
		
		  d3d_vertex_buffer[i].pos.x = array_50AC10[i].vWorldViewProjX;
		  d3d_vertex_buffer[i].pos.y = array_50AC10[i].vWorldViewProjY;
		  d3d_vertex_buffer[i].pos.z = 1.0 - 1.0 / ((array_50AC10[i].vWorldViewPosition.x * 1000) / (double)pODMRenderParams->shading_dist_mist);
		  d3d_vertex_buffer[i].rhw = 1.0 / (array_50AC10[i].vWorldViewPosition.x + 0.0000001);
		  d3d_vertex_buffer[i].diffuse = GetActorTintColor(a3->dimming_level, 0, array_50AC10[i].vWorldViewPosition.x, 0, 0);
          if ( this->bUsingSpecular )
          {
            d3d_vertex_buffer[i].specular = sub_47C3D7_get_fog_specular(0, 0, array_50AC10[i].vWorldViewPosition.x);
          }
          else
          {
            d3d_vertex_buffer[i].specular = 0;
		  }
		  d3d_vertex_buffer[i].texcoord.x = array_50AC10[i].u;
		  d3d_vertex_buffer[i].texcoord.y = array_50AC10[i].v;
		  
		}
	  
        ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE));
        ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_WRAP));
        if (bUsingSpecular)
          ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE));

        ErrD3D(pRenderD3D->pDevice->SetTexture(0, nullptr));
        ErrD3D(pRenderD3D->pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,
                                                  D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_SPECULAR,
                                                  d3d_vertex_buffer,
                                                  uNumVertices,
                                                  D3DDP_DONOTLIGHT));
        //v50 = (const char *)v5->pRenderD3D->pDevice;
        ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE));
        //(*(void (**)(void))(*(int *)v50 + 88))();
        v53->DrawLightmaps(/*-1, 0*/);
	    for (uint i = 0; i < uNumVertices; ++i)
		{
		  d3d_vertex_buffer[i].diffuse = a2;
		}
        ErrD3D(pRenderD3D->pDevice->SetTexture(0, pTexture));
        ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_WRAP));
        if ( !pRenderer->bUsingSpecular )
          ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE));

        ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE));
        ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ZERO));
        ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_SRCCOLOR));
        ErrD3D(pRenderD3D->pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,
                                                  D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_SPECULAR,
                                                  d3d_vertex_buffer,
                                                  uNumVertices,
                                                  D3DDP_DONOTLIGHT));
        if (bUsingSpecular)
        {
          ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE));

		  for (uint i = 0; i < uNumVertices; ++i)
		  {
		    d3d_vertex_buffer[i].diffuse = pRenderer->uFogColor | d3d_vertex_buffer[i].specular & 0xFF000000;
		    d3d_vertex_buffer[i].specular = 0;
		  }

          ErrD3D(pRenderD3D->pDevice->SetTexture(0, nullptr));
          ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_INVSRCALPHA));
          ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_SRCALPHA));
          ErrD3D(pRenderD3D->pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,
                                                    D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_SPECULAR,
                                                    d3d_vertex_buffer,
                                                    uNumVertices,
                                                    D3DDP_DONOTLIGHT));
          ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, TRUE));
          //v40 = pRenderer->pRenderD3D->pDevice->lpVtbl;
          v41 = GetLevelFogColor();
          pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_FOGCOLOR, GetLevelFogColor() & 0xFFFFFF);
          v6 = 0;
          pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_FOGTABLEMODE, 0);
        }
        ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE));
        ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ZERO));
        ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, v6));
      }
    }
  }
}
// 4D864C: using guessed type char byte_4D864C;

//----- (0049EB79) --------------------------------------------------------
Render::~Render()
{
  Render *v1; // esi@1

  v1 = this;
  free(this->pDefaultZBuffer);
  v1->pD3DBitmaps.Release();
  v1->pD3DSprites.Release();
  Release();
  v1->bWindowMode = 1;
  //nullsub_1();
  //nullsub_1();
}


//----- (0049E992) --------------------------------------------------------
Render::Render()
{
  //Render *v1; // esi@1
//  int v2; // eax@1
//  char v3; // zf@1

  //v1 = this;
  this->pDirectDraw4 = nullptr;
  this->pFrontBuffer4 = nullptr;
  this->pBackBuffer4 = nullptr;
  //this->pColorKeySurface4 = 0;
  //this->pDirectDraw2 = 0;
  //this->pFrontBuffer2 = 0;
  //this->pBackBuffer2 = 0;
  //this->pSomeSurface2 = 0;
  //RenderHWLContainer::RenderHWLContainer(&this->pD3DBitmaps);
  //RenderHWLContainer::RenderHWLContainer(&v1->pD3DSprites);
  this->bWindowMode = 1;
  //this->field_40054 = 0;
  //this->field_10 = 640;
  //this->field_14 = 480;
  //this->field_40030 = 0;
  //this->field_4002C = 0;
  this->pActiveZBuffer = nullptr;
  this->pDefaultZBuffer = nullptr;
  this->raster_clip_y = 0;
  this->raster_clip_x = 0;
  this->raster_clip_z = 639;
  this->raster_clip_w = 479;
  //this->field_4003C = 0x004EED80;
  //this->field_40040 = dword_4EED78;
  this->uClipZ = 640;
  //this->field_40044 = 2;
  //this->field_40048 = 6;
  this->pFrontBuffer4 = nullptr;
  this->pBackBuffer4 = nullptr;
  //this->pColorKeySurface4 = 0;
  this->pDirectDraw4 = nullptr;
  this->pRenderD3D = 0;
  this->uNumSceneBegins = 0;
  this->uNumD3DSceneBegins = 0;
  this->using_software_screen_buffer = 0;
  this->pTargetSurface = nullptr;
  this->uTargetSurfacePitch = 0;
  this->uClipY = 0;
  this->uClipX = 0;
  this->uClipW = 480;
  this->bClip = 1;
  //this->bColorKeySupported = 0;
  this->bRequiredTextureStagesAvailable = 0;
  this->bTinting = 1;
  //LOBYTE(this->field_103668) = 0;
  uNumBillboardsToDraw = 0;
  bFogEnabled = false;

  hd_water_tile_id = -1;
  hd_water_current_frame = 0;
}

bool Render::Initialize(OSWindow *window, bool bColoredLights, uint32_t uDetailLevel, bool bTinting)
{
  //bUserDirect3D = true;//ReadWindowsRegistryInt("Use D3D", 0);

  this->window = window;
  //bStartInWindow = true;
  //windowed_mode_width = windowed_width;
  //windowed_mode_height = windowed_height;

  uDesiredDirect3DDevice = ReadWindowsRegistryInt("D3D Device", 0);

  bUseColoredLights = bColoredLights;//ReadWindowsRegistryInt("Colored Lights", 0);
  uLevelOfDetail = uDetailLevel;//ReadWindowsRegistryInt("Detail Level", 1);

  this->bTinting = bTinting;

  bool r1 = pD3DBitmaps.Load(L"data\\d3dbitmap.hwl");
  bool r2 = pD3DSprites.Load(L"data\\d3dsprite.hwl");

  return r1 && r2;
}


//----- (0049ECC4) --------------------------------------------------------
void Render::ClearBlack()
{
  //if (pRenderD3D)
  {
    if (using_software_screen_buffer)
      pRenderD3D->ClearTarget(true, 0, false, 0.0);
  }
  //else
    //memset(pRenderer->pTargetSurface, 0, 4 * (field_10 * field_14 / 2));
}

//----- (0049ED18) --------------------------------------------------------
void Render::PresentBlackScreen()
{
  IDirectDrawSurface *v2; // eax@3
  DDBLTFX v3; // [sp+4h] [bp-74h]@5
  RECT x; // [sp+68h] [bp-10h]@3

  memset(&v3, 0, sizeof(DDBLTFX));
  GetWindowRect(window->GetApiHandle(), &x);

  {
    v2 = (IDirectDrawSurface *)this->pBackBuffer4;
  }

  v3.dwFillColor = 0;
  v3.dwSize = 100;
  v2->Blt(&x, 0, 0, DDBLT_COLORFILL, &v3);
  pRenderer->Present();
}

//----- (0049EDB6) --------------------------------------------------------
void Render::SavePCXScreenshot()
{
  int v5; // eax@8
  FILE *pOutFile; // edi@10
  unsigned short *v8; // eax@11
//  int v9; // eax@13
//  int v10; // ecx@15
  signed int v12; // eax@18
//  HRESULT v14; // eax@29
  char v15[56]; // [sp+Ch] [bp-158h]@10
  DDSURFACEDESC2 Dst; // [sp+48h] [bp-11Ch]@7
  char color_map[48]; // [sp+C4h] [bp-A0h]@10
  char Filename[40]; // [sp+F4h] [bp-70h]@3
  char *lineB; // [sp+11Ch] [bp-48h]@14
  char *lineG; // [sp+120h] [bp-44h]@14
  //int v24; // [sp+124h] [bp-40h]@11
  FILE *File; // [sp+128h] [bp-3Ch]@3
  PCXHeader_1 header1; // [sp+130h] [bp-34h]@10
  PCXHeader_2 header2; // [sp+140h] [bp-24h]@10
  char *lineRGB; // [sp+148h] [bp-1Ch]@10
  void *surface; // [sp+14Ch] [bp-18h]@8
  int image_width; // [sp+150h] [bp-14h]@4
  int pitch; // [sp+154h] [bp-10h]@4
  char v31; // [sp+15Ah] [bp-Ah]@25
  unsigned char pict_byte; // [sp+15Bh] [bp-9h]@17
  unsigned short *line_picture_data; // [sp+15Ch] [bp-8h]@10
  byte test_byte; // [sp+163h] [bp-1h]@17

  int num_r_bits = 5;
  int num_g_bits = 6;
  int num_b_bits = 5;

  int r_mask = 0xF800;
  int g_mask = 0x7E0;
  int b_mask = 0x1F;

  if ( !this->pRenderD3D || this->using_software_screen_buffer )
  {
    sprintf(Filename, "screen%0.2i.pcx", ScreenshotFileNumber++ % 100);
    File = fopen(Filename, "wb");
    if ( File )
    {
      pitch = this->GetRenderWidth();
      if ( pitch & 1 )
        pitch = pitch + 1;
      if ( this->pRenderD3D )
      {
        memset(&Dst, 0, 0x7C);
        Dst.dwSize = 124;
        if ( !pRenderer->LockSurface_DDraw4(pRenderer->pBackBuffer4, &Dst, DDLOCK_WAIT) )
          return;
        surface = Dst.lpSurface;
        v5 = Dst.lPitch / 2;
      }
      else
      {
        pRenderer->BeginScene();
        surface = pRenderer->pTargetSurface;
        v5 = pRenderer->uTargetSurfacePitch;
      }
      header1.right = GetRenderWidth() - 1;
      header1.left = 0;
      header1.bottom = this->GetRenderHeight() - 1;
      header1.up = 0;
      header2.pitch = pitch;
      memset(color_map, 0, sizeof(color_map));
      memset(v15, 0, sizeof(v15));
      header2.reserved = 0;
      header1.manufacturer = 10;
      pOutFile = File;
      header1.version = 5;
      header1.encoding = 1;
      header1.bpp = 8;
      header1.hdpi = 75;
      header1.vdpi = 75;
      header2.planes = 3;
      header2.palette_info = 1;
      fwrite(&header1, 1, 1, File);
      fwrite(&header1.version, 1, 1, pOutFile);
      fwrite(&header1.encoding, 1, 1, pOutFile);
      fwrite(&header1.bpp, 1, 1, pOutFile);
      fwrite(&header1.left, 2, 1, pOutFile);
      fwrite(&header1.up, 2, 1, pOutFile);
      fwrite(&header1.right, 2, 1, pOutFile);
      fwrite(&header1.bottom, 2, 1, pOutFile);
      fwrite(&header1.hdpi, 2, 1, pOutFile);
      fwrite(&header1.vdpi, 2, 1, pOutFile);
      fwrite(color_map, 0x30u, 1, pOutFile);
      fwrite(&header2, 1, 1, pOutFile);
      fwrite(&header2.planes, 1, 1, pOutFile);
      fwrite(&header2.pitch, 2, 1, pOutFile);
      fwrite(&header2.palette_info, 2, 1, pOutFile);
      fwrite(v15, 0x3Au, 1, pOutFile);
      lineRGB = (char *)malloc(3 * GetRenderWidth() + 6);
      if ( this->GetRenderHeight() > 0 )
      {
        image_width = 3 * pitch;
        //v24 = 2 * v5;
        v8 = (unsigned short *)surface;
        for ( int y = 0; y < this->GetRenderHeight(); y++ )
        {
          line_picture_data = v8;
          if ( GetRenderWidth() > 0 )
          {
            lineG = (char *)lineRGB + pitch;
            lineB = (char *)lineRGB + 2 * pitch;
            for ( uint x = 0; x < this->GetRenderWidth(); x++ )
            {
			  //int p = *line_picture_data; //0x2818
              //int for_rad = (pRenderer->uTargetGBits + pRenderer->uTargetBBits );//16 = 8 + 8
			  //int value = (pRenderer->uTargetRMask & *line_picture_data);//0 = 0xFF0000 & 0x2818
			  //int result = (pRenderer->uTargetRMask & *line_picture_data) >> (pRenderer->uTargetGBits + pRenderer->uTargetBBits );
              lineRGB[x] = (pRenderer->uTargetRMask & *line_picture_data) >> (pRenderer->uTargetGBits + pRenderer->uTargetBBits );// + pRenderer->uTargetRBits - 8);
              lineG[x] = (pRenderer->uTargetGMask & *line_picture_data) >> (pRenderer->uTargetBBits);// + pRenderer->uTargetGBits - 8);
			  //int value2 = (pRenderer->uTargetGMask & *line_picture_data); //10240 = 0xFF00 & 0x2818
			  //int result2 = (pRenderer->uTargetGMask & *line_picture_data) >> (pRenderer->uTargetBBits);
              lineB[x] = (pRenderer->uTargetBMask & *line_picture_data);// << (8 - pRenderer->uTargetBBits);
		      //int value3 = (pRenderer->uTargetBMask & *line_picture_data);//24 = 0xFF & 0x2818
              line_picture_data += 2;
            }
          }
          for ( uint i = 0; i < image_width; i += test_byte )
          {
            pict_byte = lineRGB[i];
            for ( test_byte = 1; test_byte < 0x3F; ++test_byte )
            {
              v12 = i + test_byte;
              if ( lineRGB[v12] != pict_byte )
                break;
              if ( !(v12 % pitch) )
                break;
            }
            if ( i + test_byte > image_width )
              test_byte = 3 * pitch - i;
            if ( test_byte > 1 || pict_byte >= 0xC0 )
            {
              v31 = test_byte | 0xC0;
              fwrite(&v31, 1, 1, pOutFile);
            }
            fwrite(&pict_byte, 1, 1, pOutFile);
          }
          v8 += v5;
        }
      }
      if ( this->pRenderD3D )
        ErrD3D(pRenderer->pBackBuffer4->Unlock(0));
      else
        pRenderer->EndScene();

      free(lineRGB);
      fclose(pOutFile);
    }
  }
}

//----- (0049F1BC) --------------------------------------------------------
void Render::SaveWinnersCertificate(const char *a1)
{
  unsigned int v6; // eax@8
  //FILE *v7; // edi@10
//  int v8; // ecx@11
  unsigned short *v9; // eax@11
  int v10; // eax@13
  signed int v13; // eax@18
//  char v14; // zf@27
//  HRESULT v15; // eax@29
  char v16[56]; // [sp+Ch] [bp-12Ch]@10
  __int16 v17; // [sp+44h] [bp-F4h]@10
  DDSURFACEDESC2 Dst; // [sp+48h] [bp-F0h]@7
//  int v19; // [sp+58h] [bp-E0h]@8
//  unsigned __int16 *v20; // [sp+6Ch] [bp-CCh]@8
  char color_map[48]; // [sp+C4h] [bp-74h]@10
//  unsigned int v22; // [sp+F4h] [bp-44h]@11
  char *lineB; // [sp+F8h] [bp-40h]@14
  int image_width; // [sp+FCh] [bp-3Ch]@11
  int v25; // [sp+100h] [bp-38h]@4
  FILE *File; // [sp+104h] [bp-34h]@3
  char Str; // [sp+108h] [bp-30h]@10
  char v28; // [sp+109h] [bp-2Fh]@10
  char v29; // [sp+10Ah] [bp-2Eh]@10
  char v30; // [sp+10Bh] [bp-2Dh]@10
  __int16 v31; // [sp+10Ch] [bp-2Ch]@10
  __int16 v32; // [sp+10Eh] [bp-2Ah]@10
  __int16 v33; // [sp+110h] [bp-28h]@10
  __int16 v34; // [sp+112h] [bp-26h]@10
  __int16 v35; // [sp+114h] [bp-24h]@10
  __int16 v36; // [sp+116h] [bp-22h]@10
  char v37; // [sp+118h] [bp-20h]@10
  char v38; // [sp+119h] [bp-1Fh]@10
  __int16 v39; // [sp+11Ah] [bp-1Eh]@10
  __int16 v40; // [sp+11Ch] [bp-1Ch]@10
  char *lineRGB; // [sp+120h] [bp-18h]@10
  void *surface; // [sp+124h] [bp-14h]@8
  int pitch; // [sp+128h] [bp-10h]@4
  char v44; // [sp+12Fh] [bp-9h]@25
  char *lineG; // [sp+130h] [bp-8h]@10
  unsigned char pict_byte; // [sp+137h] [bp-1h]@17
  byte test_byte;

  int num_r_bits = 5;
  int num_g_bits = 6;
  int num_b_bits = 5;

  int r_mask = 0xF800;
  int g_mask = 0x7E0;
  int b_mask = 0x1F;

  if ( !this->pRenderD3D || this->using_software_screen_buffer )
  {
    static int _4EFA84_num_winners_certificates = 0;
    ++_4EFA84_num_winners_certificates;

    File = fopen(a1, "wb");
    if ( File )
    {
      v25 = this->GetRenderWidth();
      pitch = v25;
      if ( pitch & 1 )
        pitch = pitch + 1;
      if ( this->pRenderD3D )
      {
        memset(&Dst, 0, 0x7C);
        Dst.dwSize = 124;
        if ( !pRenderer->LockSurface_DDraw4(pRenderer->pBackBuffer4, (DDSURFACEDESC2 *)&Dst, DDLOCK_WAIT) )
          return;
        surface = Dst.lpSurface;
        v6 = Dst.lPitch / 2;
      }
      else
      {
        pRenderer->BeginScene();
        surface = pRenderer->pTargetSurface;
        v6 = pRenderer->uTargetSurfacePitch;
      }
      v33 = this->GetRenderWidth() - 1;
      v31 = 0;
      v34 = (short)this->GetRenderHeight() - 1;
      v32 = 0;
      v39 = pitch;
      memset(&color_map, 0, sizeof(color_map));
      memset(&v16, 0, sizeof(v16));
      v37 = 0;
      Str = 10;
      v17 = 0;
      v28 = 5;
      v29 = 1;
      v30 = 8;
      v35 = 75;
      v36 = 75;
      v38 = 3;
      v40 = 1;
      fwrite(&Str, 1, 1, File);
      fwrite(&v28, 1, 1, File);
      fwrite(&v29, 1, 1, File);
      fwrite(&v30, 1, 1, File);
      fwrite(&v31, 2, 1, File);
      fwrite(&v32, 2, 1, File);
      fwrite(&v33, 2, 1, File);
      fwrite(&v34, 2, 1, File);
      fwrite(&v35, 2, 1, File);
      fwrite(&v36, 2, 1, File);
      fwrite(&color_map, 0x30, 1, File);
      fwrite(&v37, 1, 1, File);
      fwrite(&v38, 1, 1, File);
      fwrite(&v39, 2, 1, File);
      fwrite(&v40, 2, 1, File);
      fwrite(&v16, 0x3A, 1, File);
      lineRGB = (char *)malloc(3 * (v25 + 2));
      if ( (signed int)this->GetRenderHeight() > 0 )
      {
        image_width = 3 * pitch;
        v9 = (unsigned short *)surface;
        for ( uint j = 0; j < this->GetRenderHeight(); j++)
        {
          a1 = (const char *)v9;
          if ( v25 > 0 )
          {
            lineG = (char *)lineRGB + pitch;
            lineB = (char *)lineRGB + 2 * pitch;
            for ( v10 = 0; v10 < v25; v10++ )
            {
              lineRGB[v10] = (signed int)(r_mask & *(short *)a1) >> (num_g_bits + num_b_bits + num_r_bits - 8);
              lineG[v10] = (signed int)(g_mask & *(short *)a1) >> (num_b_bits + num_g_bits - 8);
              lineB[v10] = (b_mask & *(short *)a1) << (8 - num_b_bits);
              a1 += 2;
            }
          }
          for ( uint i = 0; i < image_width; i += test_byte )
          {
            pict_byte = lineRGB[i];
            for ( test_byte = 1; test_byte < 0x3F; test_byte )
            {
              v13 = i + test_byte;
              if ( lineRGB[v13] != pict_byte )
                break;
              if ( !(v13 % pitch) )
                break;
            }
            if ( i + test_byte > image_width )
              test_byte = 3 * pitch - i;
            if ( test_byte > 1 || pict_byte >= 0xC0 )
            {
              v44 = test_byte | 0xC0;
              fwrite(&v44, 1, 1, File);
            }
            fwrite(&pict_byte, 1, 1, File);
          }
          v9 += pitch;
        }
      }
      if ( this->pRenderD3D )
        ErrD3D(pRenderer->pBackBuffer4->Unlock(0));
      else
        pRenderer->EndScene();
      free(lineRGB);
      fclose(File);
    }
  }
}

//----- (0049F5A2) --------------------------------------------------------
void Render::PackPCXpicture( unsigned short* picture_data, int wight, int heidth, void *data_buff, int max_buff_size,unsigned int* packed_size )
{
  void *v8; // esi@3
  void *v9; // esi@3
  unsigned short* v11; // eax@4
//  int v13; // eax@8
//  int v14; // ecx@8
  signed int v15; // eax@11
//  char v16; // zf@20
//  int result; // eax@21
  char v18[58]; // [sp+Ch] [bp-ACh]@3
  char v20[48]; // [sp+48h] [bp-70h]@3
  char *lineG; // [sp+78h] [bp-40h]@7
  char *lineB; // [sp+7Ch] [bp-3Ch]@7
  int v23; // [sp+80h] [bp-38h]@4
  int v24; // [sp+84h] [bp-34h]@4
  int v25; // [sp+88h] [bp-30h]@4
  int v26; // [sp+8Ch] [bp-2Ch]@4
  PCXHeader_1 Src; // [sp+90h] [bp-28h]@3
  PCXHeader_2 v27; // [sp+A0h] [bp-18h]@3
  char *lineRGB; // [sp+A8h] [bp-10h]@3
  int pitch; // [sp+ACh] [bp-Ch]@1
  char v43; // [sp+B3h] [bp-5h]@18
  int i; // [sp+B4h] [bp-4h]@6
  unsigned short* line_picture_data;
  byte test_byte;
  unsigned char pict_byte;

  int num_r_bits = 5;
  int num_g_bits = 6;
  int num_b_bits = 5;

  int r_mask = 0xF800;
  int g_mask = 0x7E0;
  int b_mask = 0x1F;

  pitch = wight;
  if ( wight & 1 )
      pitch = wight + 1;
  Src.left = 0;
  Src.up = 0;
  Src.right = wight - 1;
  Src.bottom = heidth - 1;
  v27.pitch = pitch;
  memset(&v20, 0, 0x30u);
  memset(&v18, 0, 0x38u);
  v8 = data_buff;
  v27.reserved = 0;
  *(_WORD *)&v18[56] = 0;
  Src.manufacturer = 10;
  Src.version = 5;
  Src.encoding = 1;
  Src.bpp = 8;
  Src.hdpi = 75;
  Src.vdpi = 75;
  v27.planes = 3;
  v27.palette_info = 1;
  memcpy(data_buff, &Src, 1);
  v8 = (char *)v8 + 1;
  memcpy(v8, &Src.version, 1);
  v8 = (char *)v8 + 1;
  memcpy(v8, &Src.encoding, 1);
  v8 = (char *)v8 + 1;
  memcpy(v8, &Src.bpp, 1);
  v8 = (char *)v8 + 1;
  memcpy(v8, &Src.left, 2);
  v8 = (char *)v8 + 2;
  memcpy(v8, &Src.up, 2);
  v8 = (char *)v8 + 2;
  memcpy(v8, &Src.right, 2);
  v8 = (char *)v8 + 2;
  memcpy(v8, &Src.bottom, 2);
  v8 = (char *)v8 + 2;
  memcpy(v8, &Src.hdpi, 2);
  v8 = (char *)v8 + 2;
  memcpy(v8, &Src.vdpi, 2);
  v8 = (char *)v8 + 2;
  memcpy(v8, &v20, 0x30u);
  v8 = (char *)v8 + 48;
  memcpy(v8, &v27, 1u);
  v8 = (char *)v8 + 1;
  memcpy(v8, &v27.planes, 1);
  v8 = (char *)v8 + 1;
  memcpy(v8, &v27.pitch, 2);
  v8 = (char *)v8 + 2;
  memcpy(v8, &v27.palette_info, 2);
  v8 = (char *)v8 + 2;
  memcpy(v8, &v18, 0x3Au);
  v9 = (char *)v8 + 58;

  lineRGB = (char*)malloc(3 * (wight + 2));
  if ( heidth > 0 )
  {
    v26 = 3 * pitch;
    v23 = 2 * wight;
    v11 = picture_data;
    v24 = (int)picture_data;
    for ( v25 = heidth; v25; v25-- )
    {
      line_picture_data = v11;
      if ( wight > 0 )
      {
        lineG = (char *)lineRGB + pitch;
        lineB = (char *)lineRGB + 2 * pitch;
        for ( uint i = 0; i < wight; i++ )
        {
          lineRGB[i] = (signed int)(r_mask & *line_picture_data) >> (num_g_bits  + num_b_bits  + num_r_bits  - 8);
          lineG[i] = (signed int)(g_mask & *line_picture_data) >> ( num_b_bits + num_g_bits- 8);
          lineB[i] = (b_mask & *line_picture_data) << (8 - num_b_bits);
          line_picture_data += 1;
        }
      }
      for ( i = 0; i < v26; v9 = (char *)v9 + 1 )
      {
        pict_byte = lineRGB[i];
        for ( test_byte = 1; test_byte < 63; ++test_byte )
        {
          v15 = i + test_byte;
          if ( lineRGB[v15] != pict_byte )
            break;
          if ( !(v15 % pitch) )
            break;
        }
        if ( i + test_byte > v26 )
          test_byte = 3 * pitch - i;
        if ( test_byte > 1 || pict_byte >= 192 )
        {
          v43 = test_byte | 0xC0;
          memcpy(v9, &v43, 1);
          v9 = (char *)v9 + 1;
        }
        memcpy(v9, &pict_byte, 1);
        i += test_byte;
      }
      v11 += wight;
    }
  }
  free(lineRGB);
  *(int *)packed_size = (char *)v9 - data_buff;
}

//----- (0049F8B5) --------------------------------------------------------
void Render::SavePCXImage(const char *Filename, unsigned short* picture_data, int width, int height)
{
  FILE *result; // eax@1
  FILE *pOutFile; // edi@4
  unsigned short* v9; // eax@5
//  int v10; // eax@7
  signed int v12; // eax@12
//  char v13; // zf@21
  char v14[56]; // [sp+4h] [bp-A0h]@4
  __int16 v15; // [sp+3Ch] [bp-68h]@4
  char color_map[48]; // [sp+40h] [bp-64h]@4
  int v18; // [sp+74h] [bp-30h]@5
//  char *v19; // [sp+78h] [bp-2Ch]@5
  int image_width; // [sp+7Ch] [bp-28h]@5
  PCXHeader_1 header1; // [sp+80h] [bp-24h]@4
  PCXHeader_2 header2; // [sp+90h] [bp-14h]@4
  char *lineRGB; // [sp+98h] [bp-Ch]@4
  int pitch; // [sp+9Ch] [bp-8h]@2
  char *lineB; // [sp+A0h] [bp-4h]@8
  char *lineG;
  unsigned short* line_pictute_data;
  byte test_byte;
  char v43;

  int num_r_bits = 5;
  int num_g_bits = 6;
  int num_b_bits = 5;

  int r_mask = 0xF800;
  int g_mask = 0x7E0;
  int b_mask = 0x1F;

  result = fopen(Filename, "wb");
  Filename = (const char *)result;
  if ( result )
  {
    pitch = width;
    if ( width & 1 )
      pitch = width + 1;
    header1.left = 0;
    header1.up = 0;
    header1.right = width - 1;
    header1.bottom = height - 1;
    header2.pitch = pitch;
    memset(color_map, 0, sizeof(color_map));
    header2.reserved = 0;
    memset(v14, 0, sizeof(v14));
    v15 = 0;
    header1.manufacturer = 10;
    header1.version = 5;
    header1.encoding = 1;
    header1.bpp = 8;
    header1.hdpi = 75;
    header1.vdpi = 75;
    header2.planes = 3;
    header2.palette_info = 1;
    fwrite(&header1, 1, 1, (FILE *)Filename);
    pOutFile = (FILE *)Filename;
    fwrite(&header1.version, 1, 1, (FILE *)Filename);
    fwrite(&header1.encoding, 1, 1, pOutFile);
    fwrite(&header1.bpp, 1, 1, pOutFile);
    fwrite(&header1.left, 2, 1, pOutFile);
    fwrite(&header1.up, 2, 1, pOutFile);
    fwrite(&header1.right, 2, 1, pOutFile);
    fwrite(&header1.bottom, 2, 1, pOutFile);
    fwrite(&header1.hdpi, 2, 1, pOutFile);
    fwrite(&header1.vdpi, 2, 1, pOutFile);
    fwrite(color_map, 0x30u, 1, pOutFile);
    fwrite(&header2, 1, 1, pOutFile);
    fwrite(&header2.planes, 1, 1, pOutFile);
    fwrite(&header2.pitch, 2, 1, pOutFile);
    fwrite(&header2.palette_info, 2, 1, pOutFile);
    fwrite(v14, 0x3Au, 1, pOutFile);

    lineRGB = (char *)malloc(3 * (width + 2));
    //При сохранении изображения подряд идущие пиксели одинакового цвета объединяются и вместо указания цвета для каждого пикселя
    //указывается цвет группы пикселей и их количество.
    image_width = 3 * pitch;
    v9 = picture_data;
    for ( v18 = 0; v18 < height; v18++ )//столбец
    {
      line_pictute_data = v9;
      lineG = (char *)lineRGB + pitch;
      lineB = (char *)lineRGB + 2 * pitch;

      for ( int i = 0; i < width; i++ )//строка
      {
        lineRGB[i] = (signed int)(r_mask & *line_pictute_data) >> (num_g_bits + num_b_bits + num_r_bits - 8);
        lineG[i] = (signed int)(g_mask & *line_pictute_data) >> (num_b_bits + num_g_bits - 8);
        lineB[i] = (b_mask & *line_pictute_data) << (8 - num_b_bits);
        line_pictute_data += 1;
      }
      test_byte = 1;
      for ( int i = 0; (signed int)i < image_width; i += test_byte )
      {
        unsigned char pic_byte = lineRGB[i];
         for ( test_byte; test_byte < 63; ++test_byte )// расчёт количества одинаковых цветов
        {
          v12 = i + test_byte;
          if ( lineRGB[v12] != pic_byte )
            break;
          if ( !(v12 % pitch) )
            break;
        }
        if ( i + test_byte > image_width )
          test_byte = 3 * pitch - i;
        if ( test_byte > 1 || pic_byte >= 0xC0 )
        {
          v43 = test_byte | 0xC0;//тест-байт объединения
          fwrite(&v43, 1, 1, pOutFile);
        }
        fwrite(&pic_byte, 1, 1, pOutFile);
      }
      v9 += width;
    }
    free(lineRGB);
    fclose(pOutFile);
  }
}

//----- (0049FBCD) --------------------------------------------------------
void Render::ClearTarget(unsigned int uColor)
{
  //if (pRenderD3D)
  {
    if (using_software_screen_buffer)
      pRenderD3D->ClearTarget(true, uColor, false, 0.0);
  }
  //else
    //memset32(pTargetSurface, uColor, field_10 * field_14 / 2);
}


//----- (0049FC37) --------------------------------------------------------
void Render::Present()
{
  //struct tagRECT Rect; // [sp+8h] [bp-28h]@11
  //RECT a4; // [sp+18h] [bp-18h]@11
  //struct tagPOINT Point; // [sp+28h] [bp-8h]@11

  if ( !pRenderer->pRenderD3D || this->using_software_screen_buffer )
  {
    this->pBeforePresentFunction();
    if ( this->pRenderD3D )
    {
      if ( this->using_software_screen_buffer )
        pRenderD3D->Present(false);
    }
    else
      __debugbreak(); // no sr
    /*{
      if ( this->bWindowMode )
      {
        RestoreFrontBuffer();
        GetClientRect(this->hWnd, &Rect);
        Point.y = 0;
        Point.x = 0;
        ClientToScreen(this->hWnd, &Point);
        OffsetRect(&Rect, Point.x, Point.y);
        a4.top = 0;
        a4.bottom = 480;
        a4.left = 0;
        a4.right = 640;
        PresentRect(&Rect, &a4);
      }
      else
      {
        RestoreFrontBuffer();
        a4.top = 0;
        a4.bottom = 480;
        a4.left = 0;
        a4.right = 640;
        BltBackToFontFast(0, 0, &a4);
      }
    }*/
  }
}

//----- (0049FD3A) --------------------------------------------------------
void Render::_49FD3A_fullscreen()
{
  Render *v2; // esi@1
  IDirectDrawSurface4 *v3; // eax@6
  IDirectDrawSurface4 *v4; // ST0C_4@6
  RECT v5; // [sp+8h] [bp-10h]@6

  Render* a1 = this;
  v2 = a1;
  if ( a1->pRenderD3D )
  {
    if (pFrontBuffer4->IsLost() == DDERR_SURFACELOST)
      pFrontBuffer4->Restore();
    if (pBackBuffer4->IsLost() == DDERR_SURFACELOST)
      pBackBuffer4->Restore();
    v3 = v2->pBackBuffer4;
    v4 = v2->pFrontBuffer4;
    v5.top = 0;
    v5.bottom = window->GetHeight();
    v5.left = 0;
    v5.right = window->GetWidth();
    v3->BltFast(0, 0, v4, &v5, 16u);
  }
}

//----- (0049FDBF) --------------------------------------------------------
void Render::CreateZBuffer()
{
  if (!pDefaultZBuffer)
  {
    pDefaultZBuffer = pActiveZBuffer = (int *)malloc(0x12C000);
    memset32(pActiveZBuffer, 0xFFFF0000, 0x4B000u); //    // inlined Render::ClearActiveZBuffer  (mm8::004A085B)
  }
}

//----- (0049FE05) --------------------------------------------------------
void Render::Release()
{
  Render *v1; // esi@1
  //RenderD3D *v2; // ecx@1
  //char v3; // zf@4
  //void *v4; // ebx@6
//  IDirectDraw *v5; // eax@10
//  IDirectDrawSurface2 *v6; // eax@11
//  IDirectDrawSurface2 *v7; // eax@13
//  IDirectDrawSurface2 *v8; // eax@15
//  IDirectDraw2 *v9; // eax@17
//  IDirectDraw4 *v10; // eax@19
//  IDirectDrawSurface4 *v11; // eax@20
//  IDirectDrawSurface4 *v12; // eax@22
//  IDirectDrawSurface4 *v13; // eax@24
//  IDirectDraw4 *v14; // eax@26
//  unsigned __int16 **v15; // ebx@28
//  void **v16; // esi@29

  v1 = this;
  if (pRenderD3D)
  {
    if ( v1->using_software_screen_buffer )
    {
      pRenderD3D->ClearTarget(true, 0, false, 1.0);
      pRenderD3D->Present(0);
      pRenderD3D->ClearTarget(true, 0, false, 1.0);
    }
    //v1->pColorKeySurface4 = 0;
    v1->pBackBuffer4 = nullptr;
    v1->pFrontBuffer4 = nullptr;
    v1->pDirectDraw4 = nullptr;
    delete [] v1->pTargetSurface_unaligned;
    v1->pTargetSurface = nullptr;
    v1->pTargetSurface_unaligned = nullptr;
    if (pRenderD3D)
    {
      pRenderD3D->Release();
      delete pRenderD3D;
    }
    pRenderD3D = nullptr;
  }
  else
    ;//__debugbreak(); // no sr
  /*{
    if ( bWinNT4_0 == 1 )
    {
      v5 = (IDirectDraw *)v1->pDirectDraw2;
      if ( !v5 )
        return;
      v5->SetCooperativeLevel(v1->hWnd, 8u);
      v1->pDirectDraw2->FlipToGDISurface();
      v6 = v1->pSomeSurface2;
      if ( v6 )
      {
        v6->Release();
        v1->pSomeSurface2 = 0;
      }
      v7 = v1->pBackBuffer2;
      if ( v7 )
      {
        v7->Release();
        v1->pBackBuffer2 = 0;
      }
      v8 = v1->pFrontBuffer2;
      if ( v8 )
      {
        v8->Release();
        v1->pFrontBuffer2 = 0;
      }
      v9 = v1->pDirectDraw2;
      if ( v9 )
      {
        v9->Release();
        v1->pDirectDraw2 = 0;
      }
    }
    else
    {
      v10 = v1->pDirectDraw4;
      if ( !v10 )
        return;
      v10->SetCooperativeLevel(v1->hWnd, 1032u);
      v1->pDirectDraw4->FlipToGDISurface();
      v11 = v1->pColorKeySurface4;
      if ( v11 )
      {
        v11->Release();
        v1->pColorKeySurface4 = 0;
      }
      v12 = v1->pBackBuffer4;
      if ( v12 )
      {
        v12->Release();
        v1->pBackBuffer4 = 0;
      }
      v13 = v1->pFrontBuffer4;
      if ( v13 )
      {
        v13->Release();
        v1->pFrontBuffer4 = 0;
      }
      v14 = v1->pDirectDraw4;
      if ( v14 )
      {
        v14->Release();
        v1->pDirectDraw4 = 0;
      }
    }
    v15 = &v1->pTargetSurface;
    if ( v1->pTargetSurface )
    {
      v16 = (void **)&v1->ptr_400E8;
      free(*v16);
      *v15 = 0;
      *v16 = 0;
    }
  }*/
}




void Present32(unsigned __int32 *src, unsigned int src_pitch,
               unsigned __int32 *dst, unsigned int dst_pitch)
{
        for (uint y = 0; y < 8; ++y)
          memcpy(dst + y * dst_pitch,
                 src + y * src_pitch, src_pitch * sizeof(__int32));

        for (uint y = 8; y < 352; ++y)
        {
          memcpy(dst + y * dst_pitch,
                 src + y * src_pitch, 8 * sizeof(__int32));
          memcpy(dst + 8 + game_viewport_width + y * dst_pitch,
                 src + 8 + game_viewport_width + y * src_pitch, 174/*172*/ * sizeof(__int32));
        }

        for (uint y = 352; y < 480; ++y)
          memcpy(dst + y * dst_pitch,
                 src + y * src_pitch, src_pitch * sizeof(__int32));

        for (uint y = pViewport->uViewportTL_Y; y < pViewport->uViewportBR_Y + 1; ++y)
        {
          for (uint x = pViewport->uViewportTL_X; x < pViewport->uViewportBR_X; ++x)
          {
            //if (src[x + y * src_pitch] != (pRenderer->uTargetGMask | pRenderer->uTargetBMask))
            if (src[x + y * src_pitch] != 0xFF00FCF8)  // FFF8FCF8 =  Color32(Color16(g_mask | b_mask))
              dst[x + y * dst_pitch] = src[x + y * src_pitch];
          }
        }
}

//----- (004A597D) --------------------------------------------------------
void Present_NoColorKey()
{
  //unsigned __int16 *v0; // eax@4
//  unsigned __int16 *v1; // esi@4
  void *v2; // edi@4
  //signed int v4; // ebx@4
  //signed int v5; // ebx@6
  //void *v6; // edi@7
  //const void *v7; // esi@7
//  signed int v8; // ebx@8
  int v9; // eax@10
  unsigned int v10; // esi@10
  unsigned __int32 v11; // edi@10
  //int v12; // ecx@10
  unsigned int v13; // ebx@10
//  int v14; // eax@11
//  int v15; // eax@13
//  int v16; // eax@14
//  int v17; // eax@16
//  HRESULT v18; // eax@22
  DDSURFACEDESC2 Dst; // [sp+Ch] [bp-98h]@3
  //int v20; // [sp+88h] [bp-1Ch]@10
  int v21; // [sp+8Ch] [bp-18h]@10
  __int32 v22; // [sp+90h] [bp-14h]@10
  //unsigned __int32 v23; // [sp+94h] [bp-10h]@10
  unsigned int v24; // [sp+98h] [bp-Ch]@4
  //unsigned int _this; // [sp+9Ch] [bp-8h]@10
  //LPVOID v26; // [sp+A0h] [bp-4h]@4

  int r_mask = 0xF800;
  int g_mask = 0x7E0;
  int b_mask = 0x1F;

  if ( !pRenderer->uNumSceneBegins )
  {
    //if ( pRenderer->using_software_screen_buffer )
    //{
      memset(&Dst, 0, 0x7Cu);
      Dst.dwSize = 124;
      if ( pRenderer->LockSurface_DDraw4(pRenderer->pBackBuffer4, &Dst, DDLOCK_WAIT) )
      {
        //v26 = Dst.lpSurface;
        //pRenderer->pCurrentlyLockedSurfaceDataPtr = (unsigned __int16 *)Dst.lpSurface;
        v24 = g_mask | b_mask | ((g_mask | b_mask) << 16);
        //pRenderer->pCurrentlyLockedSoftSurface = pRenderer->pTargetSurface;
        //pRenderer->uCurrentlyLockedSurfacePitch = Dst.lPitch;
        //v1 = pRenderer->pTargetSurface;
        v2 = Dst.lpSurface;


        /*for (uint y = 0; y < 480; ++y)
        {
          auto pDst = (unsigned short *)((char *)Dst.lpSurface + y * Dst.lPitch);
          for (uint x = 0; x < 640; ++x)
            pDst[x] = pRenderer->uTargetRMask | pRenderer->uTargetBMask;
        }*/

        if (!FORCE_16_BITS)
          Present32((unsigned __int32 *)pRenderer->pTargetSurface, pRenderer->uTargetSurfacePitch, (unsigned __int32 *)Dst.lpSurface, Dst.lPitch / 4);
        else
        {        
        ushort* pSrc = (unsigned short *)pRenderer->pTargetSurface;
        short* pDst = (__int16 *)Dst.lpSurface;

        for (uint y = 0; y < 8; ++y)
          memcpy(pDst + y * Dst.lPitch / 2,

		  pSrc + y * window->GetWidth(), window->GetWidth() * sizeof(__int16));

        for (uint y = 8; y < 352; ++y)
        {
          memcpy(pDst + y * Dst.lPitch / 2,
                 pSrc + y * window->GetWidth(), 8 * sizeof(__int16));
          memcpy(pDst + 8 + game_viewport_width/*462*/ + y * Dst.lPitch / 2,
                 pSrc + 8 + game_viewport_width/*462*/ + y * window->GetWidth(), 174/*172*/ * sizeof(__int16));
        }

        for (uint y = 352; y < window->GetHeight(); ++y)
          memcpy(pDst + y * Dst.lPitch / 2,
                 pSrc + y * window->GetWidth(), window->GetWidth() * sizeof(__int16));


        ushort* pSrc_x1y1 = pSrc + window->GetWidth() * pViewport->uViewportTL_Y + pViewport->uViewportTL_X;
        //_this = (unsigned int)&pSrc[2 * (((signed int)pViewport->uViewportX >> 1) + 320 * pViewport->uViewportY)];
        short* pDst_x1y1 = pDst + Dst.lPitch * pViewport->uViewportTL_Y + pViewport->uViewportTL_X;
        //v23 = (unsigned __int32)((char *)v26 + 4 * (((signed int)pViewport->uViewportX >> 1) + (Dst.lPitch >> 2) * pViewport->uViewportY));
        v9 = ((signed int)pViewport->uViewportTL_X >> 1) - ((signed int)pViewport->uViewportBR_X >> 1);
        //v20 = ((signed int)pViewport->uViewportZ >> 1) - ((signed int)pViewport->uViewportX >> 1);
        v22 = 4 * ((Dst.lPitch / 4) + v9);
        v21 = 4 * v9 + 1280;

        //auto uNumLines = pViewport->uViewportW - pViewport->uViewportY + 1;
        //v26 = (LPVOID)(pViewport->uViewportW - pViewport->uViewportY + 1);
        v10 = (int)pSrc_x1y1;
        v11 = (int)pDst_x1y1;
        int uHalfWidth = (pViewport->uViewportBR_X - pViewport->uViewportTL_X) / 2;
        v13 = v24;

        for (uint y = pViewport->uViewportTL_Y; y < pViewport->uViewportBR_Y + 1; ++y)
        {
          //memcpy(pDst + pViewport->uViewportX + y * Dst.lPitch / 2,
          //       pSrc + pViewport->uViewportX + y * 640, (pViewport->uViewportZ - pViewport->uViewportX) * sizeof(__int16));
          for (uint x = pViewport->uViewportTL_X; x < pViewport->uViewportBR_X; ++x)
          {
            if (pSrc[y * window->GetWidth() + x] != (g_mask | b_mask))
              pDst[y * Dst.lPitch / 2 + x] = pSrc[y * window->GetWidth() + x];
          }
        }
        }

              ErrD3D(pRenderer->pBackBuffer4->Unlock(0));

       /* while ( 1 )
        {
          while ( 1 )
          {
            v14 = *(int *)v10;
            v10 += 4;
            if ( v14 == v13 )
              break;
            if ( (short)v14 == (short)v13 )
            {
              *(int *)v11 = *(int *)v11 & 0xFFFF | v14 & 0xFFFF0000;
              v11 += 4;
              --uHalfWidth;
              if ( !uHalfWidth )
                goto LABEL_21;
            }
            else
            {
              v15 = __ROL__(v14, 16);
              if ( (short)v15 == (short)v13 )
              {
                v17 = __ROR__(v15, 16);
                *(int *)v11 = *(int *)v11 & 0xFFFF0000 | (unsigned __int16)v17;
                v11 += 4;
                --uHalfWidth;
                if ( !uHalfWidth )
                  goto LABEL_21;
              }
              else
              {
                v16 = __ROR__(v15, 16);
                *(int *)v11 = v16;
                v11 += 4;
                --uHalfWidth;
                if ( !uHalfWidth )
                  goto LABEL_21;
              }
            }
          }
          v11 += 4;
          --uHalfWidth;
          if ( !uHalfWidth )
          {
LABEL_21:
            v10 += v21;
            v11 += v22;
            uHalfWidth = v20;
            if ( !--uNumLines )
            {
              ErrD3D(pRenderer->pBackBuffer4->Unlock(0));
              return;
            }
          }
        }*/
      }
    //}
  }
}


//----- (0049FFFB) --------------------------------------------------------
bool Render::InitializeFullscreen()
{
  //Render *v2; // esi@1
  //HWND v3; // ebx@1
  //void *v4; // eax@2
  //RenderD3D *v5; // eax@3
  unsigned int v6; // edx@5
  RenderD3D__DevInfo *v7; // ecx@5
  bool v8; // eax@6
  RenderD3D *v9; // ecx@13
  unsigned int v10; // eax@13
  RenderD3D *v11; // eax@25
//  HRESULT v12; // eax@25
  int v13; // ecx@25
  int v14; // eax@27
  signed int v15; // ebx@31
  //bool v16; // eax@35
  //char v17; // zf@35
//  IDirectDraw4 *v18; // eax@38
//  HRESULT v19; // eax@38
  //int *v20; // eax@39
  int *v22; // eax@42
  int v23; // ecx@42
  D3DDEVICEDESC refCaps; // [sp+Ch] [bp-300h]@25
  DDSURFACEDESC2 pDesc; // [sp+108h] [bp-204h]@40
  D3DDEVICEDESC halCaps; // [sp+184h] [bp-188h]@25
//  DDSURFACEDESC2 ddsd2; // [sp+280h] [bp-8Ch]@38
  void *v28; // [sp+2FCh] [bp-10h]@2
  int v29; // [sp+308h] [bp-4h]@2

  __debugbreak(); // Nomad

  //v2 = this;
  this->using_software_screen_buffer = 0;
  //this->pColorKeySurface4 = 0;
  this->pBackBuffer4 = nullptr;
  this->pFrontBuffer4 = nullptr;
  this->pDirectDraw4 = nullptr;
  //this->bColorKeySupported = 0;
  Release();
  //v3 = hWnd;
  this->window = window;
  CreateZBuffer();

  /*if (!bUserDirect3D)
  {
    CreateDirectDraw();
    SetDirectDrawCooperationMode(hWnd, 1);
    SetDirectDrawDisplayMode(640u, 480u, 16u);
    CreateDirectDrawPrimarySurface();
    v15 = 1;
  }
  else
  {*/
    pRenderD3D = new RenderD3D;
    v28 = pRenderD3D;
    v6 = uDesiredDirect3DDevice;
    v29 = -1;
    v7 = pRenderD3D->pAvailableDevices;
    if ( v7[v6].bIsDeviceCompatible )
    {
      v8 = pRenderD3D->CreateDevice(v6, /*0*/true, window);
    }
    else
    {
      if ( v7[1].bIsDeviceCompatible )
      {
        v8 = pRenderD3D->CreateDevice(1, /*0*/true, window);
      }
      else
      {
        if ( !v7->bIsDeviceCompatible )
          Error("There aren't any D3D devices to create.");

        v8 = pRenderD3D->CreateDevice(0, /*0*/true, window);
      }
    }
    if ( !v8 )
      Error("D3Drend->Init failed.");

    v9 = pRenderD3D;
    pBackBuffer4 = v9->pBackBuffer;
    pFrontBuffer4 = v9->pFrontBuffer;
    pDirectDraw4 = v9->pHost;
    v10 = pRenderD3D->GetDeviceCaps();
    if ( v10 & 1 )
    {
      if ( pRenderD3D )
      {
        pRenderD3D->Release();
        delete pRenderD3D;
      }
      pRenderD3D = nullptr;
      pBackBuffer4 = nullptr;
      pFrontBuffer4 = nullptr;
      pDirectDraw4 = nullptr;
      Error("Direct3D renderer:  The device failed to return capabilities.");
    }
    if ( v10 & 0x3E )
    {
      if ( pRenderD3D )
      {
        pRenderD3D->Release();
        delete pRenderD3D;
      }
      //pColorKeySurface4 = 0;
      pRenderD3D = nullptr;
      pBackBuffer4 = nullptr;
      pFrontBuffer4 = nullptr;
      pDirectDraw4 = nullptr;
      Error("Direct3D renderer:  The device doesn't support the necessary alpha blending modes.");
    }
    if ( (v10 & 0x80u) != 0 )
    {
      if ( pRenderD3D )
      {
        pRenderD3D->Release();
        delete pRenderD3D;
      }
      pRenderD3D = nullptr;
      pBackBuffer4 = nullptr;
      pFrontBuffer4 = nullptr;
      pDirectDraw4 = nullptr;
      Error("Direct3D renderer:  The device doesn't support non-square textures.");
    }
    //LOBYTE(field_10365C) = ~(unsigned __int8)(v10 >> 6) & 1;
    bRequiredTextureStagesAvailable = CheckTextureStages();
    memset(&halCaps, 0, 0xFCu);
    halCaps.dwSize = 252;
    memset(&refCaps, 0, 0xFCu);
    v11 = pRenderD3D;
    refCaps.dwSize = 252;
    ErrD3D(v11->pDevice->GetCaps(&halCaps, &refCaps));
    v13 = halCaps.dwMinTextureWidth;
    if ( (unsigned int)halCaps.dwMinTextureWidth >= halCaps.dwMinTextureHeight )
      v13 = halCaps.dwMinTextureHeight;
    v14 = halCaps.dwMaxTextureWidth;
    uMinDeviceTextureDim = v13;
    if ( (unsigned int)v14 < halCaps.dwMaxTextureHeight )
      v14 = halCaps.dwMaxTextureHeight;
    uMaxDeviceTextureDim = v14;
    if ( (unsigned int)v13 < 4 )
      uMinDeviceTextureDim = 4;
    v15 = 1;
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZENABLE, true));
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, true));
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZFUNC, 2));
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SPECULARENABLE, false));
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_COLORKEYENABLE, false));
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, false));
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_CULLMODE, 1));
    ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, 2));
    ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_MINFILTER, 2));
    ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_MIPFILTER, 3));
    ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, 2));
    ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, 0));
    ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, 2));
    ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, 2));
    ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, 0));
    ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_COLOROP, 4));
  //}
  ddpfPrimarySuface.dwSize = 32;
  GetTargetPixelFormat(&ddpfPrimarySuface);
  ParseTargetPixelFormat();

  if (!pRenderD3D)
  {
    __debugbreak();
    pBeforePresentFunction = 0;//nullsub_1;
  }
  //else
  //{
    /*v16 = IsColorKeySupported(pDirectDraw4);
    v17 = uAcquiredDirect3DDevice == v15;
    bColorKeySupported = v16;
    if ( !v17 )
      bColorKeySupported = 0;
    if ( bColorKeySupported )
    {
      memset(&ddsd2, 0, 0x7Cu);
      ddsd2.ddckCKSrcBlt.dwColorSpaceLowValue = uTargetGMask | uTargetBMask;
      ddsd2.ddckCKSrcBlt.dwColorSpaceHighValue = ddsd2.ddckCKSrcBlt.dwColorSpaceLowValue;
      v18 = pDirectDraw4;
      ddsd2.dwSize = 124;
      ddsd2.dwFlags = 65543;
      ddsd2.ddsCaps.dwCaps = 2112;
      ddsd2.dwWidth = 640;
      ddsd2.dwHeight = 480;
      ErrD3D(v18->CreateSurface(&ddsd2, &pColorKeySurface4, 0));
      pBeforePresentFunction = Present_ColorKey;
    }
    else*/
    {
      pTargetSurface = nullptr;
      pTargetSurface_unaligned = (unsigned int *)malloc(window->GetWidth() * window->GetHeight() * 2 + 32);
      if ( !pTargetSurface_unaligned
        || (memset(&pDesc, 0, 0x7Cu),
            pDesc.dwSize = 124,
            !pRenderer->LockSurface_DDraw4(pRenderer->pBackBuffer4, &pDesc, v15)) )
        return 0;
      pBackBuffer4->Unlock(0);
      v22 = (int *)pTargetSurface_unaligned + 4;
      v23 = (unsigned int)pDesc.lpSurface & 7;
      LOBYTE(v22) = (unsigned __int8)v22 & 0xF8;
      uTargetSurfacePitch = window->GetWidth();
      pBeforePresentFunction = Present_NoColorKey;
      v15 = 1;
      pTargetSurface = (unsigned __int32 *)((char *)v22 + 2 * v23);
    }
    using_software_screen_buffer = v15;
  //}
  bWindowMode = 0;
  pParty->uFlags |= 2u;
  pViewport->SetFOV(flt_6BE3A0 * 65536.0f);
  return v15 != 0;
}

//----- (004A05F3) --------------------------------------------------------
bool Render::SwitchToWindow()
{
  //Render *v2; // esi@1
  //void *v3; // eax@2
  //RenderD3D *v4; // eax@3
  //unsigned int v5; // edx@5
  //RenderD3D__DevInfo *v6; // ecx@5
  bool v7; // eax@7
  //RenderD3D *v8; // ecx@12
  unsigned int v9; // eax@12
//  RenderD3D *v10; // eax@24
//  HRESULT v11; // eax@24
  int v12; // eax@24
  int v13; // eax@26
  //bool v14; // eax@32
  //char v15; // zf@32
//  IDirectDraw4 *v16; // eax@35
//  HRESULT v17; // eax@35
  //int *v18; // eax@36
//  int *v19; // edx@38
//  int v20; // eax@38
//  unsigned int v21; // ecx@38
//  int v22; // eax@41
  D3DDEVICEDESC refCaps; // [sp+Ch] [bp-300h]@24
  DDSURFACEDESC2 pDesc; // [sp+108h] [bp-204h]@37
  D3DDEVICEDESC halCaps; // [sp+184h] [bp-188h]@24
//  DDSURFACEDESC2 ddsd2; // [sp+280h] [bp-8Ch]@35
  //RenderD3D *thisa; // [sp+2FCh] [bp-10h]@2
  int v29; // [sp+308h] [bp-4h]@2

  pParty->uFlags |= PARTY_FLAGS_1_0002;
  pViewport->SetFOV(flt_6BE3A0 * 65536.0f);
  using_software_screen_buffer = 0;
  Release();
  //pColorKeySurface4 = 0;
  pBackBuffer4 = nullptr;
  pFrontBuffer4 = nullptr;
  pDirectDraw4 = nullptr;
  //bColorKeySupported = 0;
  CreateZBuffer();
  /*if (!bUserDirect3D)
  {
    CreateDirectDraw();
    SetDirectDrawCooperationMode(hWnd, 0);
    field_4004C = 1;
    CreateFrontBuffer();
    CreateClipper(hWnd);
    CreateBackBuffer();
    field_40030 = 0;
    field_18_locked_pitch = 0;
  }
  else
  {*/
    /*v3 = malloc(0x148u);
    thisa = (RenderD3D *)v3;
    v29 = 0;
    if ( v3 )
      v4 = RenderD3D::RenderD3D((RenderD3D *)v3);
    else
      v4 = 0;*/
    pRenderD3D = new RenderD3D;
    //v4 = pRenderD3D;
    //v5 = uDesiredDirect3DDevice;
    v29 = -1;
    //v6 = pRenderD3D->pAvailableDevices;
    if (pRenderD3D->pAvailableDevices[uDesiredDirect3DDevice].bIsDeviceCompatible &&
        uDesiredDirect3DDevice != 1 )
    {
      v7 = pRenderD3D->CreateDevice(uDesiredDirect3DDevice, true, window);
    }
    else
    {
      if ( !pRenderD3D->pAvailableDevices[0].bIsDeviceCompatible )
        Error("There aren't any D3D devices to init.");

      v7 = pRenderD3D->CreateDevice(0, true, window);
    }
    if ( !v7 )
      Error("D3Drend->Init failed.");

    //v8 = pRenderD3D;
    //pColorKeySurface4 = 0;
    pBackBuffer4 = pRenderD3D->pBackBuffer;
    pFrontBuffer4 = pRenderD3D->pFrontBuffer;
    pDirectDraw4 = pRenderD3D->pHost;
    v9 = pRenderD3D->GetDeviceCaps();
    if ( v9 & 1 )
    {
      if (pRenderD3D)
      {
        pRenderD3D->Release();
        delete pRenderD3D;
      }
      pRenderD3D = nullptr;
      pBackBuffer4 = nullptr;
      pFrontBuffer4 = nullptr;
      pDirectDraw4 = nullptr;
      Error("Direct3D renderer:  The device failed to return capabilities.");
    }
    if ( v9 & 0x3E )
    {
      if (pRenderD3D)
      {
        pRenderD3D->Release();
        delete pRenderD3D;
      }
      //pColorKeySurface4 = 0;
      pRenderD3D = nullptr;
      pBackBuffer4 = nullptr;
      pFrontBuffer4 = nullptr;
      pDirectDraw4 = nullptr;
      Error("Direct3D renderer:  The device doesn't support the necessary alpha blending modes.");
    }
    if (v9 & 0x80)
    {
      if (pRenderD3D)
      {
        pRenderD3D->Release();
        delete pRenderD3D;
      }
      pRenderD3D = nullptr;
      pBackBuffer4 = nullptr;
      pFrontBuffer4 = nullptr;
      pDirectDraw4 = nullptr;
      Error("Direct3D renderer:  The device doesn't support non-square textures.");
    }
    //LOBYTE(field_10365C) = ~(unsigned __int8)(v9 >> 6) & 1;
    bRequiredTextureStagesAvailable = CheckTextureStages();
    memset(&halCaps, 0, 0xFCu);
    halCaps.dwSize = 252;
    memset(&refCaps, 0, 0xFCu);
    //v10 = v2->pRenderD3D;
    refCaps.dwSize = 252;
    ErrD3D(pRenderD3D->pDevice->GetCaps(&halCaps, &refCaps));
    v12 = halCaps.dwMinTextureWidth;
    if ( (unsigned int)halCaps.dwMinTextureWidth > halCaps.dwMinTextureHeight )
      v12 = halCaps.dwMinTextureHeight;
    uMinDeviceTextureDim = v12;
    v13 = halCaps.dwMaxTextureWidth;
    if ( (unsigned int)halCaps.dwMaxTextureWidth < halCaps.dwMaxTextureHeight )
      v13 = halCaps.dwMaxTextureHeight;
    uMaxDeviceTextureDim = v13;
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZENABLE, 1u));
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, 1u));
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZFUNC, 2u));
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SPECULARENABLE, 0));
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_COLORKEYENABLE, 0));
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_CULLMODE, 1u));
    ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, 2u));
    ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_MINFILTER, 2u));
    ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_MIPFILTER, 3u));
    ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, 2u));
    ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, 0));
    ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, 2u));
    ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, 2u));
    ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, 0));
    ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_COLOROP, 4u));
  //}

  ddpfPrimarySuface.dwSize = 32;
  GetTargetPixelFormat(&ddpfPrimarySuface);
  ParseTargetPixelFormat();

  if ( !pRenderD3D )
  {
    __debugbreak();
    //pBeforePresentFunction = 0;//nullsub_1;
    //goto LABEL_47;
  }
  /*v14 = IsColorKeySupported(pDirectDraw4);
  v15 = uAcquiredDirect3DDevice == 1;
  bColorKeySupported = v14;
  if ( !v15 )
    bColorKeySupported = 0;*/
  //if ( bColorKeySupported )
  if (false)
  {
    /*memset(&ddsd2, 0, 0x7Cu);
    ddsd2.ddckCKSrcBlt.dwColorSpaceLowValue = uTargetGMask | uTargetBMask;
    ddsd2.ddckCKSrcBlt.dwColorSpaceHighValue = ddsd2.ddckCKSrcBlt.dwColorSpaceLowValue;
    v16 = pDirectDraw4;
    ddsd2.dwSize = 124;
    ddsd2.dwFlags = 65543;
    ddsd2.ddsCaps.dwCaps = 2112;
    ddsd2.dwWidth = 640;
    ddsd2.dwHeight = 480;
    ErrD3D(v16->CreateSurface(&ddsd2, &pColorKeySurface4, 0));
    pBeforePresentFunction = Present_ColorKey;*/
LABEL_45:
    using_software_screen_buffer = 1;
//LABEL_47:
    bWindowMode = 1;
    //hWnd = hWnd;
    return 0;
  }
  pTargetSurface = 0;
  pTargetSurface_unaligned = 0;

  uint num_pixels = window->GetWidth() * window->GetHeight();
  pTargetSurface_unaligned = new unsigned int[num_pixels];

  if (!pTargetSurface_unaligned)
    return false;

  memset(&pDesc, 0, 0x7Cu);
  pDesc.dwSize = 124;
  if (!pRenderer->LockSurface_DDraw4(pRenderer->pBackBuffer4, &pDesc, DDLOCK_WAIT))
  {
    delete [] pTargetSurface_unaligned;
    return false;
  }

  memset32(pTargetSurface_unaligned, -1, num_pixels);


      pRenderer->pBackBuffer4->Unlock(0);
      /*v19 = pTargetSurface_unaligned;
      v20 = (unsigned int)pDesc.lpSurface & 7;
      v21 = (unsigned int)ptr_400E8 & 7;
      if ( v21 == v20 )
      {
        pTargetSurface = (unsigned __int16 *)v19;
      }
      else
      {
        if ( (signed int)v21 >= v20 )
          v22 = (int)((char *)v19 + 2 * (v21 - v20) + 16);
        else
          v22 = (int)((char *)v19 + 2 * (v20 - v21) + 16);
        pTargetSurface = (unsigned __int16 *)v22;
      }*/
      pTargetSurface = pTargetSurface_unaligned;
      uTargetSurfacePitch = window->GetWidth();
      pBeforePresentFunction = Present_NoColorKey;
      goto LABEL_45;
}


//----- (0044F2B2) --------------------------------------------------------
bool Render::IsGammaSupported()
{
//  bool result; // eax@3
//  HRESULT v1; // eax@4

  //if ( pVersion->pVersionInfo.dwPlatformId != VER_PLATFORM_WIN32_NT || pVersion->pVersionInfo.dwMajorVersion != 4 )
  {
    DDCAPS halCaps; // [sp+0h] [bp-180h]@4
    memset(&halCaps, 0, sizeof(DDCAPS));
    halCaps.dwSize = sizeof(DDCAPS);

    ErrD3D(pDirectDraw4->GetCaps(&halCaps, 0));
    return (halCaps.dwCaps2 >> 17) & 1;
  }
  /*else
    return false;*/
}

//----- (004A0BEE) --------------------------------------------------------
void Render::RasterLine2D(signed int uX, signed int uY, signed int uZ, signed int uW, unsigned __int16 uColor)
{
  signed int lower_bound; // eax@17
//  signed int left_bound;
  unsigned int v21; // edi@46
  int v22; // esi@47
  int v23; // ebx@47
  signed int v24; // edx@50
  signed int v25; // esi@52
  unsigned __int16 *v26; // ecx@52
  int v27; // ebx@54
  int v28; // edi@55
  int v29; // edx@55
  int v30; // ebx@60
  int v31; // edx@61
  int v32; // edi@61
  signed int upper_bound; // [sp+18h] [bp-4h]@28
  unsigned int uXa; // [sp+24h] [bp+8h]@49
  int uYb; // [sp+28h] [bp+Ch]@47
  bool left_border_x = false;
  bool right_border_x = false;
  bool left_border_z = false;
  bool right_border_z = false;
  bool upper_border_y = false;
  bool bottom_border_y = false;
  bool upper_border_w = false;
  bool bottom_border_w = false;

  if ( uX < this->raster_clip_x )// x выходит за рамки левой границы
    left_border_x = true;
  if ( uX > this->raster_clip_z )// x выходит за рамки правой границы
    right_border_x = true;

  if ( uZ < this->raster_clip_x )// z выходит за рамки левой границы
    left_border_z = true;
  if ( uZ > this->raster_clip_z )// z выходит за рамки правой границы
    right_border_z = true;

  if ( uY < this->raster_clip_y )// y выходит за рамки верхней границы
    upper_border_y = true;
  if ( uY > this->raster_clip_w )// y выходит за рамки нижней границы
    bottom_border_y = true;

  if ( uW < this->raster_clip_y )// w выходит за рамки верхней границы
    upper_border_w = true;
  if ( uW > this->raster_clip_w )// w выходит за рамки нижней границы
    bottom_border_w = true;

  if ( (left_border_x && left_border_z) || (right_border_x && right_border_z )
    || (upper_border_y && upper_border_w) || (bottom_border_y && bottom_border_w))
    return;

  if ( left_border_x || left_border_z || right_border_x || right_border_z
    || upper_border_y || upper_border_w || bottom_border_y || bottom_border_w)
  {
    if ( left_border_x || left_border_z )//if ( (BYTE4(v36) ^ (unsigned __int8)v36) & 8 )//for left (левая граница)
    {
      if ( left_border_x )//left_border = true; х меньше левой границы
      {
        uY += (uW - uY) * ((this->raster_clip_x - uX) / (uZ - uX));//t = near_clip - v0.x / v1.x - v0.x  (формула получения точки пересечения отрезка с плоскостью)
        uX = this->raster_clip_x;
      }
      else if ( left_border_z )//z меньше левой границы
      {
        uZ = this->raster_clip_x;
        uW += (uY - uW) * ((this->raster_clip_x - uZ) / (uX - uZ));
      }
    }

    if ( right_border_x || right_border_z )//if ( (BYTE4(v36) ^ (unsigned __int8)v36) & 4 )//for right (правая граница)
    {
      if ( right_border_x ) //right_border = true; х больше правой границы
      {
        uY += (uY - uW) * ((this->raster_clip_z - uX) / (uZ - uX));
        uX = this->raster_clip_z;
      }
      else if ( right_border_z )//z больше правой границы
      {
        uW += (uW - uY) * ((this->raster_clip_z - uZ) / (uX - uZ));
        uZ = this->raster_clip_z;
      }
    }

    upper_bound = 0;
    if ( uY < this->raster_clip_y )
      upper_bound = 2;
    if ( uY > this->raster_clip_w )
      upper_bound |= 1;

    lower_bound = 0;
    if ( uW < this->raster_clip_y )
      lower_bound = 2;
    if ( uW > this->raster_clip_w )
      lower_bound |= 1;

    if ( !(lower_bound & upper_bound) )//for up and down(для верха и низа)
    {
      lower_bound ^= upper_bound;
      if ( lower_bound & 2 )
      {
        if ( upper_bound & 2 )
        {
          uX += (uZ - uX) * ((this->raster_clip_y - uY) / (uW - uY));
          uY = this->raster_clip_y;
        }
        else
        {
          uZ += (uX - uZ) * ((this->raster_clip_y - uW) / (uY - uW));
          uW = this->raster_clip_y;
        }
      }
      if ( lower_bound & 1 )
      {
        if ( upper_bound & 1 )
        {
          uX += (uZ - uX) * ((this->raster_clip_w - uY) / (uW - uY));
          uY = this->raster_clip_w;
        }
        else
        {
          uZ += (uX - uZ) * ((this->raster_clip_w - uW) / (uY - uW));
          uW = this->raster_clip_w;
        }
      }
    }
  }
  v21 = pRenderer->uTargetSurfacePitch;
  if ( pRenderer->uTargetSurfacePitch )
  {
    //v12 = uX + uY * pRenderer->uTargetSurfacePitch;
    v22 = uW - uY;
    v23 = v22;
    uYb = v22;
    if ( v22 < 0 )
    {
      v23 = -v22;
      uYb = -v22;
      v21 = -pRenderer->uTargetSurfacePitch;
    }
    uXa = uZ - uX;
    if ((signed)(uZ - uX) >= 0)
      v24 = 1;
    else
    {
      uXa = -uXa;
      v24 = -1;
    }
    v25 = 0;

    v26 = (unsigned __int16 *)this->pTargetSurface;
    if ( v26 )
    {
      if ( (signed int)uXa <= v23 )//рисуем вертикальную линию
      {
        v30 = v23 + 1;
        if ( v30 > 0 )
        {
          v31 = 2 * v24;
          v32 = 2 * v21;
          //v12 = (int)&v26[v12];
          int y = 0;
          int x = 0;
          for ( v30; v30; --v30 )
          {
            v25 += uXa;
            //*(short *)v12 = uColor;
            //v12 += v32;
            WritePixel16(uX + x, uY + y, uColor);
            if ( v32 >= 0 )
              y += 1;
            else
              y -= 1;
            if ( v25 > 0 )
            {
              v25 -= uYb;
              //v12 += v31;
              if ( v31 >= 0 )
                x += 1;
              else
                x -= 1;
            }
          }
        }
      }
      else//рисуем горизонтальную линию
      {
        v27 = uXa + 1;
        if ( (signed int)(uXa + 1) > 0 )
        {
          v28 = 2 * v21;
          v29 = 2 * v24;
          int y = 0;
          int x = 0;
          //v12 = (int)&v26[v12];
          for ( v27; v27; --v27 )
          {
            v25 += uYb;
            //*(short *)v12 = uColor;
            //v12 += v29;
            WritePixel16(uX + x, uY + y, uColor);
            if ( v29 >= 0 )
              x += 1;
            else
              x -= 1;
            if ( v25 > (signed int)uXa )
            {
              v25 -= uXa;
              //v12 += v28;
              if ( v28 >= 0 )
                y += 1;
              else
                y -= 1;
            }
          }
        }
      }
    }
  }
  return;
}

//----- (004A0E80) --------------------------------------------------------
void Render::ClearZBuffer(int a2, int a3)
{
  memset32(this->pActiveZBuffer, -65536, 0x4B000);
}

//----- (004A0E97) --------------------------------------------------------
void Render::SetRasterClipRect(unsigned int uX, unsigned int uY, unsigned int uZ, unsigned int uW)
{
  this->raster_clip_x = uX;
  this->raster_clip_y = uY;
  this->raster_clip_z = uZ;
  this->raster_clip_w = uW;
}

//----- (004A0EB6) --------------------------------------------------------
void Render::ParseTargetPixelFormat()
{
  signed int v2; // ecx@1
  DWORD uRedMask; // edx@1
  unsigned int uGreenMask; // esi@5
  signed int v5; // ecx@5
  unsigned int uBlueMask; // edx@9
  signed int v7; // ecx@9
  //unsigned int v8; // ecx@13

  v2 = 0;
  uRedMask = this->ddpfPrimarySuface.dwRBitMask;
  this->uTargetBBits = 0;
  this->uTargetGBits = 0;
  this->uTargetRBits = 0;
  do
  {
    if ( (1 << v2) & uRedMask )
      ++this->uTargetRBits;
    ++v2;
  }
  while ( v2 < 32 );
  uGreenMask = this->ddpfPrimarySuface.dwGBitMask;
  v5 = 0;
  do
  {
    if ( (1 << v5) & uGreenMask )
      ++this->uTargetGBits;
    ++v5;
  }
  while ( v5 < 32 );
  uBlueMask = this->ddpfPrimarySuface.dwBBitMask;
  v7 = 0;
  do
  {
    if ( (1 << v7) & uBlueMask )
      ++this->uTargetBBits;
    ++v7;
  }
  while ( v7 < 32 );
  this->uTargetGMask = uGreenMask;
  this->uTargetRMask = this->ddpfPrimarySuface.dwRBitMask;
  this->uTargetBMask = uBlueMask;
}

//----- (004A0F40) --------------------------------------------------------
bool Render::LockSurface_DDraw4(IDirectDrawSurface4 *pSurface, DDSURFACEDESC2 *pDesc, unsigned int uLockFlags)
{
  //IDirectDrawSurface4 *v4; // esi@1
  HRESULT result; // eax@1
  HRESULT v6; // eax@4
//  int v7; // [sp-8h] [bp-14h]@10
//  unsigned int v8; // [sp-4h] [bp-10h]@10
  char v9; // [sp+Bh] [bp-1h]@1

  //v4 = pSurface;
  v9 = 1;
  result = pSurface->Lock(0, pDesc, uLockFlags, 0);
  if ( result == DDERR_SURFACELOST )
  {
    v6 = pSurface->Restore();
    if ( v6 )
    {
      if ( v6 != DDERR_IMPLICITLYCREATED )
      {
LABEL_20:
        v9 = 0;
        result = (bool)memset(pDesc, 0, 4u);
        goto LABEL_21;
      }
      pRenderer->pFrontBuffer4->Restore();
      pSurface->Restore();
    }
    result = pSurface->Lock(0, pDesc, DDLOCK_WAIT, 0);
    if ( result == DDERR_INVALIDRECT || result == DDERR_SURFACEBUSY )
      goto LABEL_20;
    ErrD3D(result);
    if ( result )
    {
      //v8 = 0;
      //v7 = 2161;
//LABEL_19:
      //CheckHRESULT((CheckHRESULT_stru0 *)&pSurface, result, "E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Screen16.cpp", v7, v8);
      goto LABEL_20;
    }
    if ( pRenderer->pRenderD3D )
      pRenderD3D->HandleLostResources();
    result = pRenderer->pDirectDraw4->RestoreAllSurfaces();
  }
  else
  {
    if ( result )
    {
      if ( result == DDERR_INVALIDRECT || result == DDERR_SURFACEBUSY )
        goto LABEL_20;
      ErrD3D(result);
      //v8 = 0;
      //v7 = 2199;
      //goto LABEL_19;
    }
  }
LABEL_21:
  LOBYTE(result) = v9;
  return result;
}


//----- (004A10E4) --------------------------------------------------------
void Render::CreateDirectDraw()
{
  Render *v1; // edi@1
//  HRESULT v2; // eax@1
//  HRESULT v3; // eax@5
//  int v6; // [sp-Ch] [bp-20h]@3
//  unsigned int v9; // [sp+0h] [bp-14h]@0
  IDirectDraw *lpDD; // [sp+10h] [bp-4h]@1

  v1 = this;
  ErrD3D(DirectDrawCreate(0, &lpDD, 0));

  pDirectDraw4 = nullptr;

  ErrD3D(lpDD->QueryInterface(IID_IDirectDraw4, (void **)&pDirectDraw4));

  lpDD->Release();
  lpDD = nullptr;
}

//----- (004A1169) --------------------------------------------------------
void Render::SetDirectDrawCooperationMode(HWND hWnd, bool bFullscreen)
{
  DWORD flags; // eax@1
//  IDirectDraw *v4; // ecx@3
////  HRESULT v5; // eax@5
//  int v6; // [sp-8h] [bp-8h]@3
//  unsigned int v7; // [sp-4h] [bp-4h]@3

  flags = bFullscreen ? DDSCL_NORMAL | DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN :
                        DDSCL_NORMAL;

  ErrD3D(pDirectDraw4->SetCooperativeLevel(hWnd, flags | DDSCL_MULTITHREADED));
}

//----- (004A11C6) --------------------------------------------------------
void Render::SetDirectDrawDisplayMode(unsigned int uWidth, unsigned int uHeight, unsigned int uBPP)
{
  ErrD3D(pDirectDraw4->SetDisplayMode(uWidth, uHeight, uBPP, 0, 0));
}

//----- (004A121C) --------------------------------------------------------
void Render::CreateFrontBuffer()
{
  Render *v1; // esi@1
  IDirectDraw *pDD; // eax@3
  IDirectDrawSurface **pOutSurf; // esi@3
  struct _DDSURFACEDESC *v4; // edx@3
////  HRESULT v5; // eax@5
  int v6; // [sp-8h] [bp-8Ch]@3
  unsigned int v7; // [sp-4h] [bp-88h]@3
  DDSURFACEDESC2 a2; // [sp+4h] [bp-80h]@3

  v1 = this;
  //if (pVersion->pVersionInfo.dwPlatformId != VER_PLATFORM_WIN32_NT ||
      //pVersion->pVersionInfo.dwMajorVersion != 4 )
  {
    memset(&a2, 0, 0x7Cu);
    pDD = (IDirectDraw *)v1->pDirectDraw4;
    a2.dwSize = 124;
    a2.dwFlags = 1;
    v7 = 0;
    a2.ddsCaps.dwCaps = 512;
    v6 = 2357;
    pOutSurf = (IDirectDrawSurface **)&v1->pFrontBuffer4;
    v4 = (struct _DDSURFACEDESC *)&a2;
  }
  /*else
  {
    memset(&a2.lPitch, 0, 0x6Cu);               // DDSURFACEDESC here
    pDD = (IDirectDraw *)v1->pDirectDraw2;
    a2.lPitch = 108;
    a2.dwBackBufferCount = 1;
    v7 = 0;
    a2.dwTextureStage = 512;
    v6 = 2346;
    pOutSurf = (IDirectDrawSurface **)&v1->pFrontBuffer2;
    v4 = (struct _DDSURFACEDESC *)&a2.lPitch;
  }*/
  ErrD3D(pDD->CreateSurface(v4, pOutSurf, 0));
}

//----- (004A12CD) --------------------------------------------------------
void Render::CreateBackBuffer()
{
  Render *v1; // esi@1
  IDirectDraw *v2; // eax@3
  IDirectDrawSurface **ppBackBuffer; // esi@3
  struct _DDSURFACEDESC *v4; // edx@3
//  HRESULT v5; // eax@5
  int v6; // [sp-8h] [bp-8Ch]@3
  unsigned int v7; // [sp-4h] [bp-88h]@3
  DDSURFACEDESC2 a2; // [sp+4h] [bp-80h]@3

  v1 = this;
  //if (pVersion->pVersionInfo.dwPlatformId != VER_PLATFORM_WIN32_NT ||
    //  pVersion->pVersionInfo.dwMajorVersion != 4 )
  {
    memset(&a2, 0, 0x7Cu);
    v2 = (IDirectDraw *)v1->pDirectDraw4;
    a2.dwSize = 124;
    a2.dwFlags = 7;
    v7 = 0;
    a2.ddsCaps.dwCaps = 2112;
    a2.dwWidth = window->GetWidth();
    a2.dwHeight = window->GetHeight();
    v6 = 2387;
    ppBackBuffer = (IDirectDrawSurface **)&v1->pBackBuffer4;
    v4 = (struct _DDSURFACEDESC *)&a2;
  }
  /*else
  {
    memset(&a2.lPitch, 0, 0x6Cu);
    v2 = (IDirectDraw *)v1->pDirectDraw2;
    a2.lPitch = 108;
    a2.dwBackBufferCount = 7;
    v7 = 0;
    a2.dwTextureStage = 2112;
    a2.dwAlphaBitDepth = 640;
    a2.dwMipMapCount = 480;
    v6 = 2374;
    ppBackBuffer = (IDirectDrawSurface **)&v1->pBackBuffer2;
    v4 = (struct _DDSURFACEDESC *)&a2.lPitch;   // //DDSURFACEDESC here fo ddraw2
  }*/
  ErrD3D(v2->CreateSurface(v4, ppBackBuffer, 0));
}

//----- (004A139A) --------------------------------------------------------
void Render::CreateDirectDrawPrimarySurface()
{
  Render *v1; // esi@1
  //int v2; // ebx@3
//  IDirectDraw2 *v3; // eax@3
//  HRESULT v4; // eax@3
  IDirectDrawSurface *pFrontBuffer; // eax@3
  DDSCAPS2 *v6; // edx@3
  IDirectDraw4 *v7; // eax@4
//  HRESULT v8; // eax@4
  int v9; // ST14_4@5
  IDirectDrawSurface *v10; // ST10_4@5
//  HRESULT v11; // eax@5
  IDirectDrawSurface **ppBackBuffer; // [sp-4h] [bp-A4h]@3
//  const char *v13; // [sp+0h] [bp-A0h]@0
//  int v14; // [sp+4h] [bp-9Ch]@0
//  unsigned int v15; // [sp+8h] [bp-98h]@0
  DDSURFACEDESC2 ddsd2; // [sp+Ch] [bp-94h]@3
  DDSCAPS2 v17; // [sp+88h] [bp-18h]@4
//  int a4; // [sp+98h] [bp-8h]@3

  v1 = this;
  //if (pVersion->pVersionInfo.dwPlatformId != VER_PLATFORM_WIN32_NT ||
      //pVersion->pVersionInfo.dwMajorVersion != 4 )
  {
    //v2 = 0;
    //this->field_4004C = 1;
    memset(&ddsd2, 0, 0x7Cu);
    v7 = v1->pDirectDraw4;
    ddsd2.dwBackBufferCount = 1;
    ddsd2.dwSize = 0x7Cu;
    ddsd2.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    ddsd2.ddsCaps.dwCaps = DDSCAPS_COMPLEX | DDSCAPS_FLIP | DDSCAPS_3DDEVICE | DDSCAPS_PRIMARYSURFACE;
    ErrD3D(v7->CreateSurface(
           &ddsd2,
           &pFrontBuffer4,
           0));
    pFrontBuffer = (IDirectDrawSurface *)v1->pFrontBuffer4;
    ppBackBuffer = (IDirectDrawSurface **)&v1->pBackBuffer4;
  }
  /*else
  {
    v2 = 0;
    this->field_4004C = 1;

    DDSURFACEDESC ddsd;
    memset(&ddsd, 0, sizeof(DDSURFACEDESC));

    ddsd.lpSurface = (LPVOID)1;
    ddsd.lPitch = 108;
    ddsd.dwBackBufferCount = 33;
    ddsd.ddsCaps.dwCaps = 8728;
    ErrD3D(pDirectDraw2->CreateSurface(
           &ddsd,
           (IDirectDrawSurface **)&pFrontBuffer2,
           0));

    pFrontBuffer = (IDirectDrawSurface *)v1->pFrontBuffer2;
    ppBackBuffer = (IDirectDrawSurface **)&v1->pBackBuffer2;
  }*/
  __debugbreak(); // warning C4700: uninitialized local variable 'v6' used
  v9 = (int)v6;
  v10 = pFrontBuffer;                           // BUG

    v17.dwCaps = 4;
  ErrD3D(pFrontBuffer->GetAttachedSurface((DDSCAPS *)&v17, ppBackBuffer));//  hr = this->pFrontBuffer->GetAttachedSurface(&ddsCaps2, ppBackBuffer);
  //CheckHRESULT(&thisa, v11, (const char *)v10, v9, (unsigned int)ppBackBuffer);
  //v1->field_40030 = v2;
  //v1->field_18_locked_pitch = v2;
}

//----- (004A14F4) --------------------------------------------------------
void Render::CreateClipper(HWND a2)
{
  ErrD3D(pDirectDraw4->CreateClipper(0, &pDDrawClipper, 0));
  ErrD3D(pDDrawClipper->SetHWnd(0, a2));
  ErrD3D(pFrontBuffer4->SetClipper(pDDrawClipper));
}

//----- (004A15D8) --------------------------------------------------------
void Render::GetTargetPixelFormat(DDPIXELFORMAT *pOut)
{
  pFrontBuffer4->GetPixelFormat(pOut);
}

//----- (004A1605) --------------------------------------------------------
void Render::LockRenderSurface(void **pOutSurfacePtr, unsigned int *pOutPixelsPerRow)
{
  signed int v4; // eax@3

  //if (pVersion->pVersionInfo.dwPlatformId != VER_PLATFORM_WIN32_NT ||
      //pVersion->pVersionInfo.dwMajorVersion != 4 )
  {
  DDSURFACEDESC2 pDesc; // [sp+4h] [bp-7Ch]@3
    memset(&pDesc, 0, 0x7Cu);
    pDesc.dwSize = 124;
    LockSurface_DDraw4(this->pBackBuffer4, &pDesc, DDLOCK_WAIT);
    *pOutSurfacePtr = pDesc.lpSurface;
    v4 = pDesc.lPitch;
  }
  /*else
  {
  DDSURFACEDESC pDesc; // [sp+4h] [bp-7Ch]@3
    memset(&pDesc.lPitch, 0, 0x6Cu);
    pDesc.lPitch = 108;
    LockSurface_DDraw2(this->pBackBuffer2, &pDesc, 1);
    *pOutSurfacePtr = (void *)pDesc.lpSurface;
    v4 = pDesc.dwReserved;
  }*/
  *pOutPixelsPerRow = v4 >> 1;
}

//----- (004A16E1) --------------------------------------------------------
void Render::UnlockBackBuffer()
{
  ErrD3D(pBackBuffer4->Unlock(0));
}

//----- (004A172E) --------------------------------------------------------
void Render::LockFrontBuffer(void **pOutSurface, unsigned int *pOutPixelsPerRow)
{
  signed int v4; // eax@3

  //if ( pVersion->pVersionInfo.dwPlatformId != VER_PLATFORM_WIN32_NT || pVersion->pVersionInfo.dwMajorVersion != 4 )
  {
  DDSURFACEDESC2 pDesc; // [sp+4h] [bp-7Ch]@3
    memset(&pDesc, 0, 0x7Cu);
    pDesc.dwSize = 124;
    LockSurface_DDraw4(this->pFrontBuffer4, &pDesc, DDLOCK_WAIT);
    *pOutSurface = pDesc.lpSurface;
    v4 = pDesc.lPitch;
  }
  /*else
  {
  DDSURFACEDESC pDesc; // [sp+4h] [bp-7Ch]@3
    memset(&pDesc.lPitch, 0, 0x6Cu);
    pDesc.lPitch = 108;
    LockSurface_DDraw2(this->pFrontBuffer2, &pDesc, 1);
    *pOutSurface = (void *)pDesc.lpSurface;
    v4 = pDesc.dwReserved;
  }*/
  *pOutPixelsPerRow = v4 >> 1;
}

//----- (004A17C7) --------------------------------------------------------
void Render::UnlockFrontBuffer()
{
  ErrD3D(pFrontBuffer4->Unlock(0));
}

//----- (004A1814) --------------------------------------------------------
void Render::RestoreFrontBuffer()
{
  if (pFrontBuffer4->IsLost() == DDERR_SURFACELOST )
    pFrontBuffer4->Restore();
}

//----- (004A184C) --------------------------------------------------------
void Render::RestoreBackBuffer()
{
  if ( pBackBuffer4->IsLost() == DDERR_SURFACELOST )
    pBackBuffer4->Restore();
}

//----- (004A18F5) --------------------------------------------------------
void Render::BltToFront(RECT *pDstRect, IDirectDrawSurface *pSrcSurface, RECT *pSrcRect, unsigned int uBltFlags)
{
  ErrD3D(pFrontBuffer4->Blt(pDstRect, (IDirectDrawSurface4 *)pSrcSurface, pSrcRect, uBltFlags, nullptr));
}

//----- (004A194A) --------------------------------------------------------
void Render::BltBackToFontFast(int a2, int a3, RECT *a4)
{
  IDirectDrawSurface *pFront; // eax@3
  IDirectDrawSurface *pBack; // [sp-Ch] [bp-Ch]@3

  //if ( pVersion->pVersionInfo.dwPlatformId != VER_PLATFORM_WIN32_NT || pVersion->pVersionInfo.dwMajorVersion != 4 )
  {
    pFront = (IDirectDrawSurface *)this->pFrontBuffer4;
    pBack = (IDirectDrawSurface *)this->pBackBuffer4;
  }
  /*else
  {
    pFront = (IDirectDrawSurface *)this->pFrontBuffer2;
    pBack = (IDirectDrawSurface *)this->pBackBuffer2;
  }*/
  pFront->BltFast(0, 0, pBack, a4, DDBLTFAST_WAIT);
}

//----- (004A1B22) --------------------------------------------------------
unsigned int Render::Billboard_ProbablyAddToListAndSortByZOrder(float z)
{
  unsigned int v7; // edx@6

  if (uNumBillboardsToDraw >= 999 )
    return 0;
  if (!uNumBillboardsToDraw)
  {
    uNumBillboardsToDraw = 1;
    return 0;
  }

  for (int left = 0, right = uNumBillboardsToDraw; left < right; ) // binsearch
  {
    v7 = left + (right - left) / 2;
    if (z <= pRenderer->pBillboardRenderListD3D[v7].z_order)
      right = v7;
    else
      left = v7 + 1;
  }

  if (z > pRenderer->pBillboardRenderListD3D[v7].z_order )
  {
    if ( v7 == pRenderer->uNumBillboardsToDraw - 1 )
      v7 = pRenderer->uNumBillboardsToDraw;
    else
    {
      if ( (signed int)pRenderer->uNumBillboardsToDraw > (signed int)v7 )
      {
        for ( uint i = 0; i < pRenderer->uNumBillboardsToDraw - v7; i++ )
        {
          memcpy(&pRenderer->pBillboardRenderListD3D[pRenderer->uNumBillboardsToDraw - i],
                 &pRenderer->pBillboardRenderListD3D[pRenderer->uNumBillboardsToDraw - (i + 1)],
           sizeof(pRenderer->pBillboardRenderListD3D[pRenderer->uNumBillboardsToDraw - i]));
        }
      }
      ++v7;
    }
    uNumBillboardsToDraw++;
    return v7;
  }
  if (z <= pRenderer->pBillboardRenderListD3D[v7].z_order )
  {
    if ( (signed int)pRenderer->uNumBillboardsToDraw > (signed int)v7 )
    {
      for ( uint i = 0; i < pRenderer->uNumBillboardsToDraw - v7; i++ )
      {
        memcpy(&pRenderer->pBillboardRenderListD3D[pRenderer->uNumBillboardsToDraw - i],
               &pRenderer->pBillboardRenderListD3D[pRenderer->uNumBillboardsToDraw -(i + 1)],
        sizeof(pRenderer->pBillboardRenderListD3D[pRenderer->uNumBillboardsToDraw - i]));
      }
    }
    uNumBillboardsToDraw++;
    return v7;
  }
  return v7;
}

//----- (004A1E9D) --------------------------------------------------------
unsigned int Render::GetBillboardDrawListSize()
{
  return pRenderer->uNumBillboardsToDraw;
}

//----- (004A1EA3) --------------------------------------------------------
unsigned int Render::GetParentBillboardID(unsigned int uBillboardID)
{
  return pRenderer->pBillboardRenderListD3D[uBillboardID].sParentBillboardID;
}

//----- (004A1EB6) --------------------------------------------------------
void Render::BeginSceneD3D()
{
  if (!uNumD3DSceneBegins++)
  {
    //if (pRenderD3D)
    {
      pRenderD3D->ClearTarget(true, 0x00F08020, true, 1.0);
      pRenderer->uNumBillboardsToDraw = 0;
      pRenderD3D->pDevice->BeginScene();

      if ( uCurrentlyLoadedLevelType == LEVEL_Outdoor )
        uFogColor = GetLevelFogColor();
      else
        uFogColor = 0;

      if ( uFogColor & 0xFF000000 )
      {
        pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, 1);
        pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_FOGCOLOR, uFogColor & 0xFFFFFF);
        pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_FOGTABLEMODE, 0);
        bUsingSpecular = true;
      }
      else
      {
        pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, 0);
        bUsingSpecular = 0;
      }
    }
    /*else
    {
      LockRenderSurface((void **)&pTargetSurface, &uTargetSurfacePitch);
      if (pTargetSurface)
        field_18_locked_pitch = uTargetSurfacePitch;
      else
        --uNumD3DSceneBegins;
    }*/
  }
}

//----- (004A1FE1) --------------------------------------------------------
void Render::DrawBillboards_And_MaybeRenderSpecialEffects_And_EndScene()
{
  --uNumD3DSceneBegins;
  if (uNumD3DSceneBegins)
    return;

  if (pRenderD3D)
  {
    pGame->draw_debug_outlines();
    DoRenderBillboards_D3D();
    pGame->pStru6Instance->RenderSpecialEffects();
    pRenderD3D->pDevice->EndScene();
  }
  else
    pGame->pStru6Instance->RenderSpecialEffects();
}

//----- (004A2031) --------------------------------------------------------
unsigned int Render::GetActorTintColor(float a2, int tint, int a4, int a5, RenderBillboard *a6)
{
//  __debugbreak(); // should not fire outside decal builder
  return ::GetActorTintColor(tint, a4, a2, a5, a6);
}

/*void Render::DrawTerrainPolygon_new(Polygon *a3, IDirect3DTexture2 *pTexture)//new function
{
  int v5; // ebx@1
  int v6; // edi@1
  int v8; // eax@7
  float v9; // eax@12
  float *v10; // esi@12
  float v11; // ecx@14
  double v12; // st7@14
  double v13; // st7@14
  double v14; // st7@14
  signed int v15; // eax@14
  int v16; // eax@15
  float v17; // ST48_4@15
  char v18; // zf@17
  int v19; // eax@18
  int v20; // eax@18
  int v21; // edx@20
  signed int v22; // ecx@20
  int v23; // eax@20
  const char *v24; // ST4C_4@20
  unsigned int v25; // ST50_4@20
  int v26; // ST54_4@20
  int v27; // eax@20
  _UNKNOWN *v28; // eax@21
  int v29; // ecx@23
  int v30; // eax@23
  int v31; // eax@23
  int v32; // eax@24
  int v33; // eax@25
  int v34; // eax@25
  int v35; // eax@25
  int v36; // eax@25
  signed int v37; // ecx@26
  int v38; // eax@26
  _UNKNOWN *v39; // eax@27
  int v40; // edx@28
  int v41; // eax@29
  int v42; // eax@29
  int v43; // eax@29
  int v44; // eax@29
  unsigned int v46; // eax@29
  int v47; // eax@30
  int v48; // eax@30
  int v49; // eax@30
  double v52; // st6@35
  const char *v55; // [sp+4Ch] [bp-1Ch]@20
  int v57; // [sp+5Ch] [bp-Ch]@3
  signed int v59; // [sp+60h] [bp-8h]@12
  int v61; // [sp+64h] [bp-4h]@4
 int i;

  v6 = (int)this;
  v5 = 0;
 if (!this->uNumD3DSceneBegins)
   return;



 
     this->pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_WRAP);
     this->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);
     if (this->bUsingSpecular)
     {
      this->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
      this->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
      this->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ZERO);
     }

     pVertices[0].pos.x = array_50AC10[0].vWorldViewProjX;
     pVertices[0].pos.y = array_50AC10[0].vWorldViewProjY;
     pVertices[0].pos.z = 1.0 - 1.0 / (1000 * array_50AC10[0].vWorldViewPosition.x / (double)pODMRenderParams->shading_dist_mist);
     pVertices[0].rhw = 1.0 / (array_50AC10[0].vWorldViewPosition.x + 0.0000001000000011686097);
     pVertices[0].diffuse = GetActorTintColor(a3->field_58, 0, array_50AC10[0].vWorldViewPosition.x, 0, 0);
     pVertices[0].specular = 0;
     pVertices[0].texcoord.x = array_50AC10[0].u;
     pVertices[0].texcoord.y = array_50AC10[0].v;
 
     pVertices[1].pos.x = array_50AC10[3].vWorldViewProjX;
     pVertices[1].pos.y = array_50AC10[3].vWorldViewProjY;
     pVertices[1].pos.z = 1.0 - 1.0 / (1000 * array_50AC10[3].vWorldViewPosition.x / (double)pODMRenderParams->shading_dist_mist);
     pVertices[1].rhw = 1.0 / (array_50AC10[3].vWorldViewPosition.x + 0.0000001000000011686097);
     pVertices[1].diffuse = GetActorTintColor(a3->field_58, 0, array_50AC10[3].vWorldViewPosition.x, 0, 0);
     pVertices[1].specular = 0;
     pVertices[1].texcoord.x = array_50AC10[3].u;
     pVertices[1].texcoord.y = array_50AC10[3].v;

     pVertices[2].pos.x = array_50AC10[1].vWorldViewProjX;
     pVertices[2].pos.y = array_50AC10[1].vWorldViewProjY;
     pVertices[2].pos.z = 1.0 - 1.0 / (1000 * array_50AC10[1].vWorldViewPosition.x / (double)pODMRenderParams->shading_dist_mist);
     pVertices[2].rhw = 1.0 / (array_50AC10[1].vWorldViewPosition.x + 0.0000001000000011686097);
     pVertices[2].diffuse = GetActorTintColor(a3->field_58, 0, array_50AC10[1].vWorldViewPosition.x, 0, 0);
     pVertices[2].specular = 0;
     pVertices[2].texcoord.x = array_50AC10[1].u;
     pVertices[2].texcoord.y = array_50AC10[1].v;

     memcpy(pVertices + 3, pVertices + 2, sizeof(RenderVertexD3D3));
     memcpy(pVertices + 4, pVertices + 1, sizeof(RenderVertexD3D3));

     pVertices[5].pos.x = array_50AC10[2].vWorldViewProjX;
     pVertices[5].pos.y = array_50AC10[2].vWorldViewProjY;
     pVertices[5].pos.z = 1.0 - 1.0 / (1000 * array_50AC10[2].vWorldViewPosition.x / (double)pODMRenderParams->shading_dist_mist);
     pVertices[5].rhw = 1.0 / (array_50AC10[2].vWorldViewPosition.x + 0.0000001000000011686097);
     pVertices[5].diffuse = GetActorTintColor(a3->field_58, 0, array_50AC10[2].vWorldViewPosition.x, 0, 0);
     pVertices[5].specular = 0;
     pVertices[5].texcoord.x = array_50AC10[2].u;
     pVertices[5].texcoord.y = array_50AC10[2].v;


     this->pRenderD3D->pDevice->SetTexture(0, pTexture);
     this->pRenderD3D->pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1, pVertices, 6, D3DDP_DONOTLIGHT);

}*/

//----- (004A26BC) --------------------------------------------------------
void Render::DrawTerrainPolygon(unsigned int uNumVertices, struct Polygon *a4, IDirect3DTexture2 *a5, bool transparent, bool clampAtTextureBorders)
{
  //RenderVertexSoft *pVertices; // esi@0
  //int v7; // edi@1
  unsigned int v8; // ebx@1
//  LightmapBuilder *v9; // esi@3
//  unsigned int v10; // edx@3
  int v11; // eax@5
//  int v12; // eax@11
//  char *v13; // esi@11
//  double v14; // st7@13
//  double v15; // st7@13
//  signed int v16; // eax@13
//  int v17; // ecx@13
//  double v18; // st7@13
//  float v19; // ST78_4@14
  int v20; // eax@14
//  char v21; // zf@16
//  HRESULT v22; // eax@17
//  HRESULT v23; // eax@17
//  HRESULT v24; // eax@19
//  HRESULT v25; // eax@19
//  unsigned int v26; // ecx@19
//  char *v27; // eax@20
//  HRESULT v28; // eax@22
//  HRESULT v29; // eax@22
//  HRESULT v30; // eax@23
//  HRESULT v31; // eax@24
//  HRESULT v32; // eax@24
//  HRESULT v33; // eax@24
//  HRESULT v34; // eax@24
//  HRESULT v35; // eax@25
//  HRESULT v36; // eax@25
//  unsigned int v37; // ecx@25
//  char *v38; // eax@26
//  int v39; // edx@27
//  HRESULT v40; // eax@28
//  HRESULT v41; // eax@28
//  HRESULT v42; // eax@28
//  HRESULT v43; // eax@28
  //IDirect3DDevice3Vtbl *v44; // ebx@28
  unsigned int v45; // eax@28
//  HRESULT v46; // eax@29
//  HRESULT v47; // eax@29
//  HRESULT v48; // eax@29
  //IDirect3DDevice3 *v49; // eax@35
  //IDirect3DDevice3Vtbl *v50; // ecx@35
//  int v51; // eax@40
//  char *v52; // esi@40
//  double v53; // st7@42
//  double v54; // st7@42
//  signed int v55; // eax@42
//  int v56; // ecx@42
//  double v57; // st7@42
//  float v58; // ST7C_4@43
//  int v59; // eax@43
  //signed int v60; // [sp+78h] [bp-14h]@31
  //RenderVertexSoft *v61; // [sp+7Ch] [bp-10h]@3
//  const char *v62; // [sp+80h] [bp-Ch]@0
//  const char *v63; // [sp+80h] [bp-Ch]@19
//  int v64; // [sp+84h] [bp-8h]@0
//  LightmapBuilder *v65; // [sp+88h] [bp-4h]@3
//  unsigned int v66; // [sp+88h] [bp-4h]@40
//  unsigned int a6a; // [sp+A0h] [bp+14h]@11
//  int a7;

  //v7 = (int)this;
  v8 = 0;
  if (!this->uNumD3DSceneBegins)
     return;
  if ( uNumVertices < 3)
     return;

  //v61 = pVertices;

  /*  v9 = pGame->pLightmapBuilder;
    v65 = v9;
    v10 = v9->std__vector_000004_size;*/
    if ( byte_4D864C && pGame->uFlags & GAME_FLAGS_1_01_lightmap_related)
    {
      v11 = ::GetActorTintColor(a4->dimming_level, 0, array_50AC10[0].vWorldViewPosition.x, 0, 0);
      pGame->pLightmapBuilder->DrawLightmaps(/*v11, 0*/);
    }
    else
    {
      if (transparent || !pGame->pLightmapBuilder->std__vector_000004_size || 
          byte_4D864C && pGame->uFlags & 2 )
      {
        if (clampAtTextureBorders)
          this->pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_CLAMP);
        else
          this->pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_WRAP);

        if (transparent || this->bUsingSpecular)
        {
          this->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
          if (transparent)
          {
            this->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
            this->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
            //this->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ZERO);
            //this->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
          }
          else
          {
            this->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
            this->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ZERO);
          }
        }

		for (uint i = 0; i < uNumVertices; ++i)
		{
		
		  d3d_vertex_buffer[i].pos.x = array_50AC10[i].vWorldViewProjX;
		  d3d_vertex_buffer[i].pos.y = array_50AC10[i].vWorldViewProjY;
		  d3d_vertex_buffer[i].pos.z = 1.0 - 1.0 / ((array_50AC10[i].vWorldViewPosition.x * 1000) / (double)pODMRenderParams->shading_dist_mist);
		  d3d_vertex_buffer[i].rhw = 1.0 / (array_50AC10[i].vWorldViewPosition.x + 0.0000001);
		  d3d_vertex_buffer[i].diffuse = ::GetActorTintColor(a4->dimming_level, 0, array_50AC10[i].vWorldViewPosition.x, 0, 0);
          if ( this->bUsingSpecular )
          {
            d3d_vertex_buffer[i].specular = sub_47C3D7_get_fog_specular(0, 0, array_50AC10[i].vWorldViewPosition.x);
          }
          else
          {
            d3d_vertex_buffer[i].specular = 0;
		  }
		  d3d_vertex_buffer[i].texcoord.x = array_50AC10[i].u;
		  d3d_vertex_buffer[i].texcoord.y = array_50AC10[i].v;
		}

		this->pRenderD3D->pDevice->SetTexture(0, a5);
        this->pRenderD3D->pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1, d3d_vertex_buffer, uNumVertices, 16);
        if (transparent)
        {
          ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE));
          ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE));
          ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ZERO));
        }
      }
      else
      {
		for (uint i = 0; i < uNumVertices; ++i)
		{
		
		  d3d_vertex_buffer[i].pos.x = array_50AC10[i].vWorldViewProjX;
		  d3d_vertex_buffer[i].pos.y = array_50AC10[i].vWorldViewProjY;
		  d3d_vertex_buffer[i].pos.z = 1.0 - 1.0 / ((array_50AC10[i].vWorldViewPosition.x * 1000) / (double)pODMRenderParams->shading_dist_mist);
		  d3d_vertex_buffer[i].rhw = 1.0 / (array_50AC10[i].vWorldViewPosition.x + 0.0000001);
		  d3d_vertex_buffer[i].diffuse = GetActorTintColor(a4->dimming_level, 0, array_50AC10[i].vWorldViewPosition.x, 0, 0);
          if ( this->bUsingSpecular )
          {
            d3d_vertex_buffer[i].specular = sub_47C3D7_get_fog_specular(0, 0, array_50AC10[i].vWorldViewPosition.x);
          }
          else
          {
            d3d_vertex_buffer[i].specular = 0;
		  }
          __debugbreak(); // warning C4700: uninitialized local variable 'v20' used
		  d3d_vertex_buffer[i].specular = v20;
		  d3d_vertex_buffer[i].texcoord.x = array_50AC10[i].u;
		  d3d_vertex_buffer[i].texcoord.y = array_50AC10[i].v;
		}
        ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE));
        ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_WRAP));
        if (pRenderer->bUsingSpecular)
          ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE));

        ErrD3D(pRenderD3D->pDevice->SetTexture(0, 0));
        ErrD3D(pRenderD3D->pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,
                D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1,
                d3d_vertex_buffer,
                uNumVertices,
                16));
        //v63 = (const char *)v7->pRenderD3D->pDevice;
        ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE));
        //(*(void (**)(void))(*(int *)v63 + 88))();
        pGame->pLightmapBuilder->DrawLightmaps(/*-1, 0*/);
	    for (uint i = 0; i < uNumVertices; ++i)
	    {
		  d3d_vertex_buffer[i].diffuse = -1;
	    }
        ErrD3D(pRenderD3D->pDevice->SetTexture(0, a5));
        ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_WRAP));
        if ( !pRenderer->bUsingSpecular )
        {
          ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE));
        }
        ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE));
        ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ZERO));
        ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_SRCCOLOR));
        ErrD3D(pRenderD3D->pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,
                D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1,
                d3d_vertex_buffer,
                uNumVertices,
                16));
        if ( pRenderer->bUsingSpecular )
        {
          ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE));
          ErrD3D(pRenderD3D->pDevice->SetTexture(0, 0));
		  for (uint i = 0; i < uNumVertices; ++i)
		  {
			d3d_vertex_buffer[i].diffuse = pRenderer->uFogColor | d3d_vertex_buffer[i].specular & 0xFF000000;
			d3d_vertex_buffer[i].specular = 0;
		  }

          ErrD3D(pRenderD3D->pDevice->SetTexture(0, 0));//problem
          ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_INVSRCALPHA));
          ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_SRCALPHA));
          ErrD3D(pRenderD3D->pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,
                  D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1,
                  d3d_vertex_buffer,
                  uNumVertices,
                  16));
          ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, TRUE));
          //v44 = pRenderer->pRenderD3D->pDevice;
          v45 = GetLevelFogColor();
          ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_FOGCOLOR, v45 & 0xFFFFFF));
          v8 = 0;
          ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_FOGTABLEMODE, 0));
        }
        ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE));
        ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ZERO));
        ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, v8));
      }
    }

    //if (pIndoorCamera->flags & INDOOR_CAMERA_DRAW_TERRAIN_OUTLINES || pBLVRenderParams->uFlags & INDOOR_CAMERA_DRAW_TERRAIN_OUTLINES)
    if (pGame->pIndoorCameraD3D->debug_flags & ODM_RENDER_DRAW_TERRAIN_OUTLINES)
      pGame->pIndoorCameraD3D->debug_outline_d3d(d3d_vertex_buffer, uNumVertices, 0x00FFFFFF, 0.0);
  }
// 4A26BC: could not find valid save-restore pair for esi
// 4D864C: using guessed type char byte_4D864C;

//----- (004A2DA3) --------------------------------------------------------
void Render::DrawOutdoorSkyPolygon(unsigned int uNumVertices, struct Polygon *pSkyPolygon, IDirect3DTexture2 *pTexture)
{
  int v7; // eax@7

  if ( !this->uNumD3DSceneBegins )
    return;
  if ( uNumVertices >= 3 )
  {
    this->pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_WRAP);
    if ( this->bUsingSpecular )
    {
      this->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
      this->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
      this->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ZERO);
    }
    for ( uint i = 0; i < uNumVertices; ++i )
    {
      pVertices[i].pos.x = array_50AC10[i].vWorldViewProjX;
      pVertices[i].pos.y = array_50AC10[i].vWorldViewProjY;
      pVertices[i].pos.z = 0.99989998;
      pVertices[i].rhw = array_50AC10[i]._rhw;

      pVertices[i].diffuse = ::GetActorTintColor(31, 0, array_50AC10[i].vWorldViewPosition.x, 1, 0);
      v7 = 0;
      if (this->bUsingSpecular)
        v7 = sub_47C3D7_get_fog_specular(0, 1, array_50AC10[i].vWorldViewPosition.x);
      pVertices[i].specular = v7;
      pVertices[i].texcoord.x = array_50AC10[i].u;
      pVertices[i].texcoord.y = array_50AC10[i].v;
    }
    pRenderer->pRenderD3D->pDevice->SetTexture(0, pTexture);
    pRenderer->pRenderD3D->pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1,
      pVertices, uNumVertices, D3DDP_DONOTUPDATEEXTENTS | D3DDP_DONOTLIGHT);
  }
}

//----- (004A2ED5) --------------------------------------------------------
void Render::DrawIndoorSkyPolygon(signed int uNumVertices, struct Polygon *pSkyPolygon, IDirect3DTexture2 *pTexture)
{
  int v5; // eax@3

  if ( this->uNumD3DSceneBegins )
  {
    if ( uNumVertices >= 3 )
    {
      ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_WRAP));
      v5 = 31 - (pSkyPolygon->dimming_level & 0x1F);
      if ( v5 < pOutdoor->max_terrain_dimming_level )
        v5 = pOutdoor->max_terrain_dimming_level;
      for (uint i = 0; i < uNumVertices; ++i)
      {
        d3d_vertex_buffer[i].pos.x = array_507D30[i].vWorldViewProjX;
        d3d_vertex_buffer[i].pos.y = array_507D30[i].vWorldViewProjY;
        d3d_vertex_buffer[i].pos.z = 1.0 - 1.0 / (array_507D30[i].vWorldViewPosition.x * 0.061758894);
        d3d_vertex_buffer[i].rhw = array_507D30[i]._rhw;
        d3d_vertex_buffer[i].diffuse = 8 * v5 | ((8 * v5 | (v5 << 11)) << 8);
        d3d_vertex_buffer[i].specular = 0;
        d3d_vertex_buffer[i].texcoord.x = array_507D30[i].u;
        d3d_vertex_buffer[i].texcoord.y = array_507D30[i].v;
      }
      ErrD3D(pRenderD3D->pDevice->SetTexture(0, pTexture));
      ErrD3D(pRenderD3D->pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1,
        d3d_vertex_buffer, uNumVertices, 28));
    }
  }
}

//----- (00479A53) --------------------------------------------------------
void Render::DrawIndoorSky(unsigned int uNumVertices, unsigned int uFaceID)
{
  BLVFace *pFace; // esi@1
  double v5; // st7@3
  signed __int64 v6; // qax@3
  int v12; // edx@7
  int v13; // eax@7
//  void *v15; // ecx@9
  int v17; // edi@9
  double v18; // st7@9
  signed int v19; // ebx@9
  void *v20; // ecx@9
  int v21; // ebx@11
  int v22; // eax@14
  signed __int64 v23; // qtt@16
//  double v24; // st7@16
//  unsigned __int8 v25; // sf@16
//  unsigned __int8 v26; // of@16
  double v28; // st7@20
//  char *v29; // ebx@20
//  char *v30; // edx@20
//  unsigned __int8 v31; // c0@21
//  unsigned __int8 v32; // c3@21
  double v33; // st6@23
//  char *v34; // esi@30
  const void *v35; // ecx@31
  int v36; // eax@31
  const void *v37; // edi@31
  signed __int64 v38; // qax@31
  int v39; // ecx@31
  int v40; // ebx@33
  int v41; // eax@36
  signed __int64 v42; // qtt@39
  int v43; // eax@39
//  char v44; // zf@39
//  double v45; // st7@39
//  double v46; // st7@39
//  unsigned int v47; // edx@40
  double v48; // st7@41
//  RenderVertexSoft *v49; // ebx@41
//  void *v50; // edi@43
  double v51; // st7@46
//  RenderVertexSoft *v52; // edx@46
//  void *v53; // edi@48
//  signed int v59; // [sp-4h] [bp-178h]@17
//  struct Polygon *v60; // [sp+0h] [bp-174h]@17
//  IDirect3DTexture2 *v61; // [sp+4h] [bp-170h]@17
  struct Polygon pSkyPolygon; // [sp+14h] [bp-160h]@6
  unsigned int v63; // [sp+120h] [bp-54h]@7
  unsigned int v65; // [sp+128h] [bp-4Ch]@1
  unsigned int v66; // [sp+12Ch] [bp-48h]@7
  //float v67; // [sp+130h] [bp-44h]@7
  __int64 v69; // [sp+13Ch] [bp-38h]@3
  int v70; // [sp+144h] [bp-30h]@3
  int X; // [sp+148h] [bp-2Ch]@9
  int v72; // [sp+14Ch] [bp-28h]@7
  float v73; // [sp+150h] [bp-24h]@16
  unsigned int v74; // [sp+154h] [bp-20h]@3
  unsigned int v74_; // [sp+154h] [bp-20h]@3
  RenderVertexSoft *v75; // [sp+158h] [bp-1Ch]@3
  float v76; // [sp+15Ch] [bp-18h]@9
  int v77; // [sp+160h] [bp-14h]@9
  int v78; // [sp+164h] [bp-10h]@7
  void *v79; // [sp+168h] [bp-Ch]@9
  float v80; // [sp+16Ch] [bp-8h]@3
  const void *v81; // [sp+170h] [bp-4h]@7

  pFace = &pIndoor->pFaces[uFaceID];
  //for floor and wall(for example Selesta)-------------------
  if (pFace->uPolygonType == POLYGON_InBetweenFloorAndWall || pFace->uPolygonType == POLYGON_Floor)
  {
    int v69 = (GetTickCount() / 32) - pGame->pIndoorCameraD3D->vPartyPos.x;
    int v55 = (GetTickCount() / 32) + pGame->pIndoorCameraD3D->vPartyPos.y;
    for (uint i = 0; i < uNumVertices; ++i)
    {
      array_507D30[i].u = (v69 + array_507D30[i].u) * 0.25f;
      array_507D30[i].v = (v55 + array_507D30[i].v) * 0.25f;
    }
    pRenderer->DrawIndoorPolygon(uNumVertices, pFace, pBitmaps_LOD->pHardwareTextures[pFace->uBitmapID], pFace->GetTexture(), PID(OBJECT_BModel, uFaceID), -1, 0);
    return;
  }
//---------------------------------------
  v70 = (signed __int64)((double)(pBLVRenderParams->fov_rad_fixpoint * pGame->pIndoorCameraD3D->vPartyPos.z)//179
                       / (((double)pBLVRenderParams->fov_rad_fixpoint + 16192.0)
                        * 65536.0)
                       + (double)pBLVRenderParams->uViewportCenterY);
  v5 = (double)pGame->pIndoorCameraD3D->sRotationX * 0.0030664064;//0
  v6 = (signed __int64)((double)pBLVRenderParams->uViewportCenterY//183
                      - (double)pBLVRenderParams->fov_rad_fixpoint
                      / ((cos(v5) * 16192.0 + 0.0000001)
                       * 65535.0)
                      * (sin(v5) * -16192.0 - (double)pGame->pIndoorCameraD3D->vPartyPos.z));

  stru_8019C8._48653D_frustum_blv(65536, 0, 0, 0, 65536, 0);
  pSkyPolygon.Create_48607B(&stru_8019C8);
  pSkyPolygon.uTileBitmapID = pFace->uBitmapID;

  pSkyPolygon.pTexture = pBitmaps_LOD->GetTexture(pSkyPolygon.uTileBitmapID);
  if ( !pSkyPolygon.pTexture )
    return;

  pSkyPolygon.dimming_level = 0;
  pSkyPolygon.uNumVertices = uNumVertices;

  pSkyPolygon.v_18.x = -stru_5C6E00->Sin(pGame->pIndoorCameraD3D->sRotationX + 16);
  pSkyPolygon.v_18.y = 0;
  pSkyPolygon.v_18.z = -stru_5C6E00->Cos(pGame->pIndoorCameraD3D->sRotationX + 16);

  memcpy(&array_507D30[uNumVertices], array_507D30, sizeof(array_507D30[uNumVertices]));
  pSkyPolygon.field_24 = 0x2000000;

  extern float _calc_fov(int viewport_width, int angle_degree);
  //v64 = (double)(signed int)(pBLVRenderParams->uViewportZ - pBLVRenderParams->uViewportX) * 0.5;
  //v72 = 65536 / (signed int)(signed __int64)(v64 / tan(0.6457717418670654) + 0.5);
  v72 = 65536.0f / _calc_fov(pBLVRenderParams->uViewportZ - pBLVRenderParams->uViewportX, 74);
  v12 = pSkyPolygon.pTexture->uWidthMinus1;
  v13 = pSkyPolygon.pTexture->uHeightMinus1;
  //v67 = 1.0 / (double)pSkyPolygon.pTexture->uTextureWidth;
  v63 = 224 * pMiscTimer->uTotalGameTimeElapsed & v13;
  v66 = 224 * pMiscTimer->uTotalGameTimeElapsed & v12;
  v78 = 0;
  //v81 = 0;
  float v68 = 1.0 / (double)pSkyPolygon.pTexture->uTextureHeight;
  if ( (signed int)pSkyPolygon.uNumVertices <= 0 )
    return;

  int _507D30_idx = 0;
  for ( _507D30_idx; _507D30_idx < pSkyPolygon.uNumVertices; _507D30_idx++ )
  {
    //v15 = (void *)(v72 * (v70 - (int)array_507D30[_507D30_idx].vWorldViewProjY));
    v77 = fixpoint_mul(pSkyPolygon.ptr_38->viewing_angle_from_west_east, v72 * (v70 - array_507D30[_507D30_idx].vWorldViewProjY));
    v74 = v77 + pSkyPolygon.ptr_38->angle_from_north;

    v77 = fixpoint_mul(pSkyPolygon.ptr_38->viewing_angle_from_north_south, v72 * (v70 - array_507D30[_507D30_idx].vWorldViewProjY));
    v74_ = v77 + pSkyPolygon.ptr_38->angle_from_east;

    v79 = (void *)(fixpoint_mul(pSkyPolygon.v_18.z, v72 * (v70 - (int)array_507D30[_507D30_idx].vWorldViewProjY)));
    v17 = v72 * (pBLVRenderParams->uViewportCenterX - (int)array_507D30[_507D30_idx].vWorldViewProjX);
    v18 = array_507D30[_507D30_idx].vWorldViewProjY - 1.0;
    v19 = -pSkyPolygon.field_24;
    v77 = -pSkyPolygon.field_24;
    X = (int)((char *)v79 + pSkyPolygon.v_18.x);
    LODWORD(v76) = (signed __int64)v18;
    v20 = (void *)(v72 * (v70 - LODWORD(v76)));
    while ( 1 )
    {
      v79 = v20;
      if ( !X )
        goto LABEL_14;
      v21 = abs(v19 >> 14);
      if ( v21 <= abs(X) )//0x800 <= 0x28652
        break;
      if ( SLODWORD(v76) <= (signed int)pViewport->uViewportTL_Y )
        break;
      v19 = v77;
      v20 = v79;
LABEL_14:
      v79 = (void *)fixpoint_mul(pSkyPolygon.v_18.z, (int)v20);
      v22 = fixpoint_mul(pSkyPolygon.v_18.z, (int)v20);
      --LODWORD(v76);
      v20 = (char *)v20 + v72;
      X = v22 + pSkyPolygon.v_18.x;
      v78 = 1;
    }
    if ( !v78 )
    {
      LODWORD(v23) = v77 << 16;
      HIDWORD(v23) = v77 >> 16;//v23 = 0xfffffe0000000000
      v79 = (void *)(v23 / X);//X = FFFF9014(-28652)
      v77 = v17;
      signed __int64 s = v74 + fixpoint_mul(pSkyPolygon.ptr_38->angle_from_west, v17);// s = 0xFFFFFFFF FFFF3EE6
      LODWORD(v80) = v66 + ((signed int)fixpoint_mul(SLODWORD(s), v23 / X) >> 4);
      array_507D30[_507D30_idx].u = ((double)SLODWORD(v80) * 0.000015259022) * (1.0 / (double)pSkyPolygon.pTexture->uTextureWidth);

      signed __int64 s2 = v74_ + fixpoint_mul(pSkyPolygon.ptr_38->angle_from_south, v17);
      LODWORD(v80) = v63 + ((signed int)fixpoint_mul(SLODWORD(s2), v23 / X) >> 4);
      array_507D30[_507D30_idx].v = ((double)SLODWORD(v80) * 0.000015259022) * v68;

      v77 = fixpoint_mul(SLODWORD(s), v23 / X);
      LODWORD(v73) = fixpoint_mul(SLODWORD(s2), v23 / X);
      array_507D30[_507D30_idx]._rhw = 65536.0 / (double)(signed int)v79;

      //if ( (int)v81 >= pSkyPolygon.uNumVertices )
      //{
      //  pRenderer->DrawIndoorSkyPolygon(pSkyPolygon.uNumVertices, &pSkyPolygon,
      //     pBitmaps_LOD->pHardwareTextures[(signed __int16)pSkyPolygon.uTileBitmapID]);
      //  return;
      //}
      continue;
    }
    break;
  }
  if ( _507D30_idx >= pSkyPolygon.uNumVertices )
  {
    pRenderer->DrawIndoorSkyPolygon(pSkyPolygon.uNumVertices, &pSkyPolygon,
       pBitmaps_LOD->pHardwareTextures[(signed __int16)pSkyPolygon.uTileBitmapID]);
    return;
  }
  LODWORD(v73) = 0;
  v80 = v76;
  if ( (signed int)pSkyPolygon.uNumVertices > 0 )
  {
    v28 = (double)SLODWORD(v76);
    LODWORD(v76) = (int)(char *)array_50AC10 + 28;
	uint i = 0;
    for ( v78 = pSkyPolygon.uNumVertices; v78; --v78 )
    {
      ++LODWORD(v73);
      memcpy(&array_50AC10[i], &array_507D30[i], 0x30u);
      LODWORD(v76) += 48;
      if ( v28 < array_507D30[i].vWorldViewProjY | v28 == array_507D30[i].vWorldViewProjY
        || v28 >= array_507D30[i + 1].vWorldViewProjY )
      {
        if ( v28 >= array_507D30[i].vWorldViewProjY || v28 <= array_507D30[i + 1].vWorldViewProjY )
        {
          i++;
          continue;
        }
        v33 = (array_507D30[i + 1].vWorldViewProjX - array_507D30[i].vWorldViewProjX) * v28 / (array_507D30[i + 1].vWorldViewProjY - array_507D30[i].vWorldViewProjY)
            + array_507D30[i + 1].vWorldViewProjX;
      }
      else
      {
        v33 = (array_507D30[i].vWorldViewProjX - array_507D30[i + 1].vWorldViewProjX) * v28 / (array_507D30[i].vWorldViewProjY - array_507D30[i + 1].vWorldViewProjY)
            + array_507D30[i].vWorldViewProjX;
      }
      array_50AC10[i + 1].vWorldViewProjX = v33;
      ++LODWORD(v73);
      *(unsigned int *)LODWORD(v76) = v28;
      LODWORD(v76) += 48;
      i++;
    }
  }
  if ( SLODWORD(v73) <= 0 )
    goto LABEL_40;
  //v34 = (char *)&array_50AC10[0].vWorldViewProjY;
  uint j = 0;
  v65 = v77 >> 14;
  //HIDWORD(v69) = LODWORD(v73);
  for ( int t = (int)LODWORD(v73); t > 1; t-- )
  {
    v35 = (const void *)(v72 * (v70 - (unsigned __int64)(signed __int64)array_50AC10[j].vWorldViewProjY));

    //v78 = pSkyPolygon.ptr_38->viewing_angle_from_west_east;
    //v81 = (const void *)fixpoint_mul(pSkyPolygon.ptr_38->viewing_angle_from_west_east, v35);
    v36 = (int)(fixpoint_mul(pSkyPolygon.ptr_38->viewing_angle_from_west_east, (int)v35) + pSkyPolygon.ptr_38->angle_from_north);

    v81 = v35;
    v74 = v36;
    //v78 = pSkyPolygon.ptr_38->viewing_angle_from_north_south;
    v81 = (const void *)fixpoint_mul(pSkyPolygon.ptr_38->viewing_angle_from_north_south, (int)v35);
    v78 = (int)v35;
    v75 = (RenderVertexSoft *)((char *)v81 + pSkyPolygon.ptr_38->angle_from_east);
    //v81 = (const void *)pSkyPolygon.v_18.z;
    v78 = fixpoint_mul(pSkyPolygon.v_18.z, (int)v35);
    v37 = (const void *)(v72 * (pBLVRenderParams->uViewportCenterX - (unsigned __int64)(signed __int64)array_50AC10[j].vWorldViewProjX));
    v38 = (signed __int64)(array_50AC10[j].vWorldViewProjY - 1.0);
    v81 = 0;
    LODWORD(v76) = v38;
    v39 = v72 * (v70 - v38);
    while ( 1 )
    {
      v78 = v39;
      if ( !X )
        goto LABEL_36;
      v40 = abs(X);
      if ( abs((signed __int64)v65) <= v40 )
        break;
      if ( SLODWORD(v76) <= (signed int)pViewport->uViewportTL_Y )
        break;
      v39 = v78;
LABEL_36:
      v78 = pSkyPolygon.v_18.z;
      v41 = fixpoint_mul(pSkyPolygon.v_18.z, v39);
      --LODWORD(v76);
      v39 += v72;
      X = v41 + pSkyPolygon.v_18.x;
      v81 = (const void *)1;
    }
    if ( v81 )
    {
      v79 = (void *)pSkyPolygon.v_18.z;
      v78 = 2 * LODWORD(v76);
      v81 = (const void *)fixpoint_mul(pSkyPolygon.v_18.z, (((double)v70 - ((double)(2 * LODWORD(v76)) - array_50AC10[j].vWorldViewProjY))
                                                                                         * (double)v72));
      X = (int)((char *)v81 + pSkyPolygon.v_18.x);
    }
    LODWORD(v42) = v77 << 16;
    HIDWORD(v42) = v77 >> 16;
    v79 = (void *)(v42 / X);
    v81 = v37;

    //v78 = pSkyPolygon.ptr_38->angle_from_west;
    v81 = (const void *)fixpoint_mul(pSkyPolygon.ptr_38->angle_from_west, (int)v37);
    v43 = v74 + fixpoint_mul(pSkyPolygon.ptr_38->angle_from_west, (int)v37);
    v74 = (unsigned int)v37;
    LODWORD(v76) = v43;

    //v78 = pSkyPolygon.ptr_38->angle_from_south;
    v75 = (RenderVertexSoft *)((char *)v75 + fixpoint_mul(pSkyPolygon.ptr_38->angle_from_south, (int)v37));
    //v74 = fixpoint_mul(v43, v42 / X);
    v81 = (const void *)fixpoint_mul((int)v75, v42 / X);

    //v34 += 48;
    //v78 = v66 + ((signed int)fixpoint_mul(v43, v42 / X) >> 4);
    //v44 = HIDWORD(v69)-- == 1;
    //v45 = (double)(v66 + ((signed int)fixpoint_mul(v43, v42 / X) >> 4)) * 0.000015259022;
    //v78 = v63 + ((signed int)fixpoint_mul((int)v75, v42 / X) >> 4);
    array_50AC10[j].u = ((double)(v66 + ((signed int)fixpoint_mul(v43, v42 / X) >> 4)) * 0.000015259022) * (1.0 / (double)pSkyPolygon.pTexture->uTextureWidth);
    array_50AC10[j].v = ((double)(v66 + ((signed int)fixpoint_mul(v43, v42 / X) >> 4)) * 0.000015259022) * v68;
    //v46 = (double)(signed int)v79;
    array_50AC10[j].vWorldViewPosition.x = 0.000015258789 * (double)(signed int)v79;
    array_50AC10[j]._rhw = 65536.0 / (double)(signed int)v79;
	++j;
  }
  //while ( !v44 );
LABEL_40:
  uint i = 0;
  if ( SLODWORD(v73) > 0 )
  {
    v48 = (double)SLODWORD(v80);
    for ( HIDWORD(v69) = LODWORD(v73); HIDWORD(v69); --HIDWORD(v69) )
    {
      if ( v48 >= array_50AC10[i].vWorldViewProjY )
      {
        ++i;
        memcpy(&array_507D30[i], &array_50AC10[i], 0x30u);
      }
    }
  }
  pSkyPolygon.uNumVertices = i;
  pRenderer->DrawIndoorSkyPolygon(pSkyPolygon.uNumVertices, &pSkyPolygon, pBitmaps_LOD->pHardwareTextures[(signed __int16)pSkyPolygon.uTileBitmapID]);
  int pNumVertices = 0;
  if ( SLODWORD(v73) > 0 )
  {
    v51 = (double)SLODWORD(v80);
    for ( v80 = v73; v80 != 0.0; --LODWORD(v80) )
    {
      if ( v51 <= array_50AC10[pNumVertices].vWorldViewProjY )
      {
        ++pNumVertices;
        memcpy(&array_507D30[pNumVertices], &array_50AC10[pNumVertices], 0x30u);
      }
    }
  }
  pRenderer->DrawIndoorSkyPolygon(pNumVertices, &pSkyPolygon, pBitmaps_LOD->pHardwareTextures[(signed __int16)pSkyPolygon.uTileBitmapID]);
}


//----- (004A2FC0) --------------------------------------------------------
void Render::DrawIndoorPolygon(unsigned int uNumVertices, BLVFace *pFace, IDirect3DTexture2 *pHwTex, Texture *pTex, int uPackedID, unsigned int uColor, int a8)
{
  //Render *v8; // edi@1
  //unsigned int v9; // esi@3
//  RenderVertexSoft *v12; // ecx@9
//  RenderVertexD3D3 *v13; // eax@9
//  double v14; // st6@10
//  int v15; // edx@10
//  Texture *v16; // edx@10
//  double v17; // st6@10
  //char v18; // zf@10
//  Texture *v19; // edx@10
  //Texture *v23; // edx@16
  //char *v24; // ecx@16
  //char *v25; // eax@16
  //double v26; // st6@17
  //int v27; // esi@17
  //double v28; // st6@17
  //unsigned int v33; // ecx@18
  //char *v34; // eax@19
  //Texture *v45; // edx@23
  //char *v46; // ecx@23
  //char *v47; // eax@23
  //double v48; // st6@24
  //int v49; // esi@24
  //double v50; // st6@24
//  const char *v53; // [sp-Ch] [bp-20h]@21
  //int v54; // [sp-8h] [bp-1Ch]@21
  //unsigned int v55; // [sp-4h] [bp-18h]@21
//  const char *v56; // [sp+0h] [bp-14h]@0
//  int v57; // [sp+4h] [bp-10h]@0
//  unsigned int v58; // [sp+8h] [bp-Ch]@0
  //LightmapBuilder *v59; // [sp+Ch] [bp-8h]@3
  //int a3a; // [sp+10h] [bp-4h]@4

  //v8 = this;
  if (!uNumD3DSceneBegins || uNumVertices < 3)
    return;

  //auto a3 = pFace;
  //auto a6 = uPackedID;
    //v59 = pGame->pLightmapBuilder;
    //v9 = v59->std__vector_000004_size;

  int sCorrectedColor = uColor;
  if (pGame->pLightmapBuilder->std__vector_000004_size)
    sCorrectedColor = -1;
  pGame->AlterGamma_BLV(pFace, &sCorrectedColor);


  if (pFace->uAttributes & FACE_OUTLINED)
  {
//    int color;
    if (GetTickCount() % 300 >= 150)
      uColor = sCorrectedColor = 0xFF20FF20;
    else uColor = sCorrectedColor = 0xFF109010;
  }

  if (byte_4D864C && pGame->uFlags & GAME_FLAGS_1_01_lightmap_related)
  {
      __debugbreak();
      ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, false));
      ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_WRAP));
	  for (uint i = 0; i < uNumVertices; ++i)
	  {
		d3d_vertex_buffer[i].pos.x = array_507D30[i].vWorldViewProjX;
		d3d_vertex_buffer[i].pos.y = array_507D30[i].vWorldViewProjY;
		d3d_vertex_buffer[i].pos.z = 1.0 - 1.0 / (array_507D30[i].vWorldViewPosition.x * 0.061758894);
		d3d_vertex_buffer[i].rhw = 1.0 / array_507D30[i].vWorldViewPosition.x;
		d3d_vertex_buffer[i].diffuse = sCorrectedColor;
		d3d_vertex_buffer[i].specular = 0;
		d3d_vertex_buffer[i].texcoord.x = array_507D30[i].u / (double)pTex->uTextureWidth;
		d3d_vertex_buffer[i].texcoord.y = array_507D30[i].v / (double)pTex->uTextureHeight;
	  }

      ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_WRAP));
      ErrD3D(pRenderD3D->pDevice->SetTexture(0, nullptr));
      ErrD3D(pRenderD3D->pDevice->DrawPrimitive(
              D3DPT_TRIANGLEFAN,
              D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1,
              d3d_vertex_buffer,
              uNumVertices,
              28));
      pGame->pLightmapBuilder->DrawLightmaps(/*-1, 0*/);
  }
  else
  {
    if (!pGame->pLightmapBuilder->std__vector_000004_size ||
        byte_4D864C && pGame->uFlags & 2)
    {
      for (uint i = 0; i < uNumVertices; ++i)
      {
        d3d_vertex_buffer[i].pos.x = array_507D30[i].vWorldViewProjX;
        d3d_vertex_buffer[i].pos.y = array_507D30[i].vWorldViewProjY;
        d3d_vertex_buffer[i].pos.z = 1.0 - 1.0 / (array_507D30[i].vWorldViewPosition.x * 0.061758894);
        d3d_vertex_buffer[i].rhw = 1.0 / array_507D30[i].vWorldViewPosition.x;
        d3d_vertex_buffer[i].diffuse = sCorrectedColor;
        d3d_vertex_buffer[i].specular = 0;
        d3d_vertex_buffer[i].texcoord.x = array_507D30[i].u / (double)pTex->uTextureWidth;
        d3d_vertex_buffer[i].texcoord.y = array_507D30[i].v / (double)pTex->uTextureHeight;
      }

      ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_WRAP));
      ErrD3D(pRenderD3D->pDevice->SetTexture(0, pHwTex));
      ErrD3D(pRenderD3D->pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,
                D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1,
                d3d_vertex_buffer,
                uNumVertices,
                28));
    }
    else
    {
      for (uint i = 0; i < uNumVertices; ++i)
      {
        d3d_vertex_buffer[i].pos.x = array_507D30[i].vWorldViewProjX;
        d3d_vertex_buffer[i].pos.y = array_507D30[i].vWorldViewProjY;
        d3d_vertex_buffer[i].pos.z = 1.0 - 1.0 / (array_507D30[i].vWorldViewPosition.x * 0.061758894);
        d3d_vertex_buffer[i].rhw = 1.0 / array_507D30[i].vWorldViewPosition.x;
        d3d_vertex_buffer[i].diffuse = uColor;
        d3d_vertex_buffer[i].specular = 0;
        d3d_vertex_buffer[i].texcoord.x = array_507D30[i].u / (double)pTex->uTextureWidth;
        d3d_vertex_buffer[i].texcoord.y = array_507D30[i].v / (double)pTex->uTextureHeight;
      }
      //__debugbreak();
        //if ( (signed int)uNumVertices > 0 )
        //{
          //v23 = pTex;
          //v24 = (char *)&array_507D30[0].vWorldViewPosition;
          //v25 = (char *)&d3d_vertex_buffer[0].pos.y;
          //pTex = (Texture *)uNumVertices;
          //uint v18;
          //do
          //{
            //v26 = *(float *)v24 * 0.061758894;
            //v27 = *((int *)v24 + 3);
            //*((int *)v25 + 4) = 0;
            //*((int *)v25 - 1) = v27;
            //*(int *)v25 = *((int *)v24 + 4);
            //*((int *)v25 + 3) = uColor;
            //v25 += 32;
            //*((float *)v25 - 7) = 1.0 - 1.0 / v26;
            //v28 = 1.0 / *(float *)v24;
            //v24 += 48;
            //v18 = pTex == (Texture *)1;
            //pTex = (Texture *)((char *)pTex - 1);
            //*((float *)v25 - 6) = v28;
            //a3 = (BLVFace *)v23->uTextureWidth;
            //*((float *)v25 - 3) = *((float *)v24 - 6) / (double)(signed int)v23->uTextureWidth;
            //a3 = (BLVFace *)v23->uTextureHeight;
            //*((float *)v25 - 2) = *((float *)v24 - 5) / (double)(signed int)v23->uTextureHeight;
          //}
          //while ( !v18 );
        //}
        ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, false));
        ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_WRAP));
        ErrD3D(pRenderD3D->pDevice->SetTexture(0, nullptr));
        ErrD3D(pRenderD3D->pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,
                D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1,
                d3d_vertex_buffer,
                uNumVertices,
                28));

        ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE));
        pGame->pLightmapBuilder->DrawLightmaps(/*-1, 0*/);

        for (uint i = 0; i < uNumVertices; ++i)
          d3d_vertex_buffer[i].diffuse = sCorrectedColor;
        /*v33 = uNumVertices;
        if ( (signed int)uNumVertices > 0 )
        {
          v34 = (char *)&d3d_vertex_buffer[0].diffuse;
          do
          {
            *(int *)v34 = uCorrectedColor;
            v34 += 32;
            --v33;
          }
          while ( v33 );
        }*/
        ErrD3D(pRenderD3D->pDevice->SetTexture(0, pHwTex));
        ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_WRAP));
        ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE));
        ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE));
        ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ZERO));
        ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_SRCCOLOR));
        ErrD3D(pRenderD3D->pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,
                D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1,
                d3d_vertex_buffer,
                uNumVertices,
                28));
        ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE));
        ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ZERO));
        ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE));
    }
  }
}
// 4D864C: using guessed type char byte_4D864C;

//----- (004A43B1) --------------------------------------------------------
void Render::DrawBillboard_Indoor(RenderBillboardTransform_local0 *pSoftBillboard, Sprite *pSprite, int dimming_level)
{
  //RenderBillboardTransform_local0 *v4; // ebx@2
  //double v5; // st7@2
  //float v6; // ST08_4@2
  unsigned int v7; // eax@2
  //int v8; // ecx@2
  //unsigned int v9; // esi@2
  //Sprite *v10; // edi@5
  signed int v11; // eax@9
  signed int v12; // eax@9
  //double v13; // st7@12
  //double v14; // st6@12
  double v15; // st5@12
  double v16; // st4@12
  double v17; // st3@12
  double v18; // st2@12
  int v19; // ecx@14
  double v20; // st3@14
  int v21; // ecx@16
  double v22; // st3@16
  //IDirect3DTexture2 *v23; // eax@18
  //signed int v24; // [sp+18h] [bp-18h]@5
  //signed int v25; // [sp+1Ch] [bp-14h]@5
  //Render *v26; // [sp+20h] [bp-10h]@1
  float v27; // [sp+24h] [bp-Ch]@5
  int v28; // [sp+28h] [bp-8h]@2
  float v29; // [sp+2Ch] [bp-4h]@5
  //float pSoftBillboarda; // [sp+38h] [bp+8h]@2
  float v31; // [sp+3Ch] [bp+Ch]@5
  //float v32; // [sp+3Ch] [bp+Ch]@12
  float a1; // [sp+40h] [bp+10h]@5

  //v26 = this;
  if ( this->uNumD3DSceneBegins )
  {
    //v4 = pSoftBillboard;
    //v5 = (double)pSoftBillboard->zbuffer_depth;
    //pSoftBillboarda = pSoftBillboard->zbuffer_depth;
    //v6 = pSoftBillboard->zbuffer_depth;
    v7 = Billboard_ProbablyAddToListAndSortByZOrder(pSoftBillboard->zbuffer_depth);
    //v8 = dimming_level;
    //v9 = v7;
    v28 = dimming_level & 0xFF000000;
    if ( dimming_level & 0xFF000000 )
      pBillboardRenderListD3D[v7].opacity = RenderBillboardD3D::Opaque_3;
    else
      pBillboardRenderListD3D[v7].opacity = RenderBillboardD3D::Transparent;
    //v10 = a3;
    pBillboardRenderListD3D[v7].field_90 = pSoftBillboard->field_44;
    pBillboardRenderListD3D[v7].sZValue = pSoftBillboard->sZValue;
    pBillboardRenderListD3D[v7].sParentBillboardID = pSoftBillboard->sParentBillboardID;
    //v25 = pSoftBillboard->uScreenSpaceX;
    //v24 = pSoftBillboard->uScreenSpaceY;
    a1 = (pSoftBillboard->_screenspace_x_scaler_packedfloat & 0xFFFF) * 0.000015260186 + HIWORD(pSoftBillboard->_screenspace_x_scaler_packedfloat);
    v29 = (pSoftBillboard->_screenspace_y_scaler_packedfloat & 0xFFFF) * 0.000015260186 + HIWORD(pSoftBillboard->_screenspace_y_scaler_packedfloat);
    v31 = (double)((pSprite->uBufferWidth >> 1) - pSprite->uAreaX);
    v27 = (double)(pSprite->uBufferHeight - pSprite->uAreaY);
    if ( pSoftBillboard->uFlags & 4 )
      v31 = v31 * -1.0;
    if ( pSoftBillboard->sTintColor && this->bTinting )
    {
      v11 = ::GetActorTintColor(dimming_level, 0, pSoftBillboard->zbuffer_depth, 0, 0);
      v12 = BlendColors(pSoftBillboard->sTintColor, v11);
      if ( v28 )
        v12 = (unsigned int)((char *)&array_77EC08[1852].pEdgeList1[17] + 3) & ((unsigned int)v12 >> 1);
    }
    else
    {
      v12 = ::GetActorTintColor(dimming_level, 0, pSoftBillboard->zbuffer_depth, 0, 0);
    }
    //v13 = (double)v25;
    pBillboardRenderListD3D[v7].pQuads[0].specular = 0;
    pBillboardRenderListD3D[v7].pQuads[0].diffuse = v12;
    pBillboardRenderListD3D[v7].pQuads[0].pos.x = pSoftBillboard->uScreenSpaceX - v31 * a1;
    //v14 = (double)v24;
    //v32 = v14;
    pBillboardRenderListD3D[v7].pQuads[0].pos.y = pSoftBillboard->uScreenSpaceY - v27 * v29;
    v15 = 1.0 - 1.0 / (pSoftBillboard->zbuffer_depth * 0.061758894);
    pBillboardRenderListD3D[v7].pQuads[0].pos.z = v15;
    v16 = 1.0 / pSoftBillboard->zbuffer_depth;
    pBillboardRenderListD3D[v7].pQuads[0].rhw = 1.0 / pSoftBillboard->zbuffer_depth;
    pBillboardRenderListD3D[v7].pQuads[0].texcoord.x = 0.0;
    pBillboardRenderListD3D[v7].pQuads[0].texcoord.y = 0.0;
    v17 = (double)((pSprite->uBufferWidth >> 1) - pSprite->uAreaX);
    v18 = (double)(pSprite->uBufferHeight - pSprite->uAreaY - pSprite->uAreaHeight);
    if ( pSoftBillboard->uFlags & 4 )
      v17 = v17 * -1.0;
    pBillboardRenderListD3D[v7].pQuads[1].specular = 0;
    pBillboardRenderListD3D[v7].pQuads[1].diffuse = v12;
    pBillboardRenderListD3D[v7].pQuads[1].pos.x = pSoftBillboard->uScreenSpaceX - v17 * a1;
    pBillboardRenderListD3D[v7].pQuads[1].pos.y = pSoftBillboard->uScreenSpaceY - v18 * v29;
    pBillboardRenderListD3D[v7].pQuads[1].pos.z = v15;
    pBillboardRenderListD3D[v7].pQuads[1].rhw = v16;
    pBillboardRenderListD3D[v7].pQuads[1].texcoord.x = 0.0;
    pBillboardRenderListD3D[v7].pQuads[1].texcoord.y = 1.0;
    v19 = pSprite->uBufferHeight - pSprite->uAreaY - pSprite->uAreaHeight;
    v20 = (double)(pSprite->uAreaX + pSprite->uAreaWidth + (pSprite->uBufferWidth >> 1) - pSprite->uBufferWidth);
    if ( pSoftBillboard->uFlags & 4 )
      v20 = v20 * -1.0;
    pBillboardRenderListD3D[v7].pQuads[2].specular = 0;
    pBillboardRenderListD3D[v7].pQuads[2].diffuse = v12;
    pBillboardRenderListD3D[v7].pQuads[2].pos.x = v20 * a1 + pSoftBillboard->uScreenSpaceX;
    pBillboardRenderListD3D[v7].pQuads[2].pos.y = pSoftBillboard->uScreenSpaceY - (double)v19 * v29;
    pBillboardRenderListD3D[v7].pQuads[2].pos.z = v15;
    pBillboardRenderListD3D[v7].pQuads[2].rhw = v16;
    pBillboardRenderListD3D[v7].pQuads[2].texcoord.x = 1.0;
    pBillboardRenderListD3D[v7].pQuads[2].texcoord.y = 1.0;
    v21 = pSprite->uBufferHeight - pSprite->uAreaY;
    v22 = (double)(pSprite->uAreaX + pSprite->uAreaWidth + (pSprite->uBufferWidth >> 1) - pSprite->uBufferWidth);
    if ( pSoftBillboard->uFlags & 4 )
      v22 = v22 * -1.0;
    pBillboardRenderListD3D[v7].pQuads[3].specular = 0;
    pBillboardRenderListD3D[v7].pQuads[3].diffuse = v12;
    pBillboardRenderListD3D[v7].pQuads[3].pos.x = v22 * a1 + pSoftBillboard->uScreenSpaceX;
    pBillboardRenderListD3D[v7].pQuads[3].pos.y = pSoftBillboard->uScreenSpaceY - (double)v21 * v29;
    pBillboardRenderListD3D[v7].pQuads[3].pos.z = v15;
    pBillboardRenderListD3D[v7].pQuads[3].rhw = v16;
    pBillboardRenderListD3D[v7].pQuads[3].texcoord.x = 1.0;
    pBillboardRenderListD3D[v7].pQuads[3].texcoord.y = 0.0;
    //v23 = pSprite->pTexture;
    pBillboardRenderListD3D[v7].uNumVertices = 4;
    pBillboardRenderListD3D[v7].z_order = pSoftBillboard->zbuffer_depth;
    pBillboardRenderListD3D[v7].pTexture = pSprite->pTexture;
  }
}

//----- (004A354F) --------------------------------------------------------
void Render::MakeParticleBillboardAndPush_BLV(RenderBillboardTransform_local0 *a2, IDirect3DTexture2 *a3, unsigned int uDiffuse, int angle)
{
  //double v5; // st7@3
  //float v6; // ST20_4@3
  //float v7; // ST00_4@3
  unsigned int v8; // esi@3
  //int v9; // eax@3
  //int v10; // ebx@3
  float v11; // ST28_4@3
  //double v12; // st7@3
  //float v13; // ST24_4@3
  //double v14; // st6@3
  //float v15; // ST1C_4@3
  float v16; // ST2C_4@3
  float v17; // ST30_4@3
  signed int v18; // ST18_4@3
  signed int v19; // ST14_4@3
  signed int v20; // ST10_4@3
  signed int v21; // eax@3
  double v22; // st6@3
  float v23; // ST2C_4@3
  float v24; // ST30_4@3
  signed int v25; // ST10_4@3
  signed int v26; // ST14_4@3
  signed int v27; // ST18_4@3
  signed int v28; // eax@3
  double v29; // st6@3
  float v30; // ecx@3
  float v31; // ST2C_4@3
  float v32; // ST30_4@3
  signed int v33; // ST10_4@3
  signed int v34; // ST14_4@3
  signed int v35; // ST18_4@3
  signed int v36; // eax@3
  float v37; // ecx@3
  double v38; // st6@3
  float v39; // ST2C_4@3
  float v40; // ST30_4@3
  signed int v41; // ST10_4@3
  signed int v42; // ST14_4@3
  signed int v43; // ST18_4@3
  signed int v44; // eax@3
  double v45; // st6@3
  float v46; // eax@3

  if ( this->uNumD3DSceneBegins )
  {
    if (a2->zbuffer_depth)
    {
      //v5 = (double)a2->zbuffer_depth;
      //v6 = v5;
      //v7 = v5;
      v8 = Billboard_ProbablyAddToListAndSortByZOrder(a2->zbuffer_depth);
      pBillboardRenderListD3D[v8].opacity = RenderBillboardD3D::Opaque_1;
      pBillboardRenderListD3D[v8].field_90 = a2->field_44;
      pBillboardRenderListD3D[v8].sZValue = a2->sZValue;
      pBillboardRenderListD3D[v8].sParentBillboardID = a2->sParentBillboardID;
      //v9 = a2->uScreenSpaceX;
      //v10 = a2->uScreenSpaceY;
      v11 = (a2->_screenspace_x_scaler_packedfloat & 0xFFFF) * 0.000015260186 + HIWORD(a2->_screenspace_x_scaler_packedfloat);
      //v12 = (double) a2->uScreenSpaceX;
      //v13 = v12;
      //v14 = (double)(a2->uScreenSpaceY - 12);
      //v15 = v14;
      v16 = (double)( a2->uScreenSpaceX - 12) - (double) a2->uScreenSpaceX;
      v17 = (double)(a2->uScreenSpaceY - 25) - (double)(a2->uScreenSpaceY - 12);
      v18 = stru_5C6E00->Cos(angle);
      v19 = stru_5C6E00->Sin(angle);
      v20 = stru_5C6E00->Sin(angle);
      v21 = stru_5C6E00->Cos(angle);
      pBillboardRenderListD3D[v8].pQuads[0].pos.x = (((double)(unsigned __int16)v18 * 0.000015259022
                                                       + (double)(v18 >> 16))
                                                       * v16
                                                       - ((double)(unsigned __int16)v19 * 0.000015259022
                                                       + (double)(v19 >> 16))
                                                       * v17)
                                                       * v11 + (double) a2->uScreenSpaceX;
      v22 = (((double)(unsigned __int16)v21 * 0.000015259022 + (double)(v21 >> 16)) * v17
           + ((double)(unsigned __int16)v20 * 0.000015259022 + (double)(v20 >> 16)) * v16
           - 12.0)
          * v11
          + (double)a2->uScreenSpaceY;
      pBillboardRenderListD3D[v8].pQuads[0].specular = 0;
      pBillboardRenderListD3D[v8].pQuads[0].diffuse = uDiffuse;
      pBillboardRenderListD3D[v8].pQuads[0].pos.y = v22;
      pBillboardRenderListD3D[v8].pQuads[0].pos.z = 1.0 - 1.0 / (a2->zbuffer_depth * 0.061758894);
      pBillboardRenderListD3D[v8].pQuads[0].rhw = 1.0 / a2->zbuffer_depth;
      pBillboardRenderListD3D[v8].pQuads[0].texcoord.x = 0.0;
      pBillboardRenderListD3D[v8].pQuads[0].texcoord.y = 0.0;
      v31 = (double)(a2->uScreenSpaceX + 12) - (double) a2->uScreenSpaceX;
      v32 = (double)a2->uScreenSpaceY - (double)(a2->uScreenSpaceY - 12);
      v25 = stru_5C6E00->Cos(angle);
      v26 = stru_5C6E00->Sin(angle);
      v27 = stru_5C6E00->Sin(angle);
      v28 = stru_5C6E00->Cos(angle);
      pBillboardRenderListD3D[v8].pQuads[1].pos.x = (((double)(unsigned __int16)v25 * 0.000015259022
                                                       + (double)(v25 >> 16))
                                                       * v31
                                                       - ((double)(unsigned __int16)v26 * 0.000015259022
                                                       + (double)(v26 >> 16))
                                                       * v32)
                                                       * v11 + (double) a2->uScreenSpaceX;
      v29 = (((double)(unsigned __int16)v28 * 0.000015259022 + (double)(v28 >> 16)) * v32
           + ((double)(unsigned __int16)v27 * 0.000015259022 + (double)(v27 >> 16)) * v31
           - 12.0)
          * v11
          + (double)a2->uScreenSpaceY;
      pBillboardRenderListD3D[v8].pQuads[1].pos.z = pRenderer->pBillboardRenderListD3D[v8].pQuads[0].pos.z;
      v30 = pBillboardRenderListD3D[v8].pQuads[0].rhw;
      pBillboardRenderListD3D[v8].pQuads[1].pos.y = v29;
      pBillboardRenderListD3D[v8].pQuads[1].specular = 0;
      pBillboardRenderListD3D[v8].pQuads[1].rhw = v30;
      pBillboardRenderListD3D[v8].pQuads[1].diffuse = uDiffuse;
      pBillboardRenderListD3D[v8].pQuads[1].texcoord.x = 0.0;
      pBillboardRenderListD3D[v8].pQuads[1].texcoord.y = 1.0;
      v23 = (double)(a2->uScreenSpaceX - 12) - (double) a2->uScreenSpaceX;
      v24 = (double)a2->uScreenSpaceY - (double)(a2->uScreenSpaceY - 12);
      v33 = stru_5C6E00->Cos(angle);
      v34 = stru_5C6E00->Sin(angle);
      v35 = stru_5C6E00->Sin(angle);
      v36 = stru_5C6E00->Cos(angle);
      pBillboardRenderListD3D[v8].pQuads[2].pos.x = (((double)(unsigned __int16)v33 * 0.000015259022
                                                        + (double)(v33 >> 16))
                                                        * v23
                                                        - ((double)(unsigned __int16)v34 * 0.000015259022
                                                        + (double)(v34 >> 16))
                                                        * v24)
                                                        * v11 + (double) a2->uScreenSpaceX;
      v37 = pBillboardRenderListD3D[v8].pQuads[0].pos.z;
      v38 = (((double)(unsigned __int16)v36 * 0.000015259022 + (double)(v36 >> 16)) * v24
           + ((double)(unsigned __int16)v35 * 0.000015259022 + (double)(v35 >> 16)) * v23
           - 12.0)
          * v11
          + (double)a2->uScreenSpaceY;
      pBillboardRenderListD3D[v8].pQuads[2].specular = 0;
      pBillboardRenderListD3D[v8].pQuads[2].pos.z = v37;
      pBillboardRenderListD3D[v8].pQuads[2].rhw = pBillboardRenderListD3D[v8].pQuads[0].rhw;
      pBillboardRenderListD3D[v8].pQuads[2].diffuse = uDiffuse;
      pBillboardRenderListD3D[v8].pQuads[2].pos.y = v38;
      pBillboardRenderListD3D[v8].pQuads[2].texcoord.x = 1.0;
      pBillboardRenderListD3D[v8].pQuads[2].texcoord.y = 1.0;
      v39 = (double)(a2->uScreenSpaceX + 12) - (double) a2->uScreenSpaceX;
      v40 = (double)(a2->uScreenSpaceY - 25) - (double)(a2->uScreenSpaceY - 12);
      v41 = stru_5C6E00->Cos(angle);
      v42 = stru_5C6E00->Sin(angle);
      v43 = stru_5C6E00->Sin(angle);
      v44 = stru_5C6E00->Cos(angle);
      pBillboardRenderListD3D[v8].pQuads[3].pos.x = (((double)(unsigned __int16)v41 * 0.000015259022
                                                        + (double)(v41 >> 16))
                                                        * v39
                                                        - ((double)(unsigned __int16)v42 * 0.000015259022
                                                        + (double)(v42 >> 16))
                                                        * v40)
                                                        * v11 + (double) a2->uScreenSpaceX;
      v45 = (((double)(unsigned __int16)v44 * 0.000015259022 + (double)(v44 >> 16)) * v40
           + ((double)(unsigned __int16)v43 * 0.000015259022 + (double)(v43 >> 16)) * v39
           - 12.0)
          * v11
          + (double)a2->uScreenSpaceY;
      v46 = pBillboardRenderListD3D[v8].pQuads[0].pos.z;
      pBillboardRenderListD3D[v8].pQuads[3].specular = 0;
      pBillboardRenderListD3D[v8].pQuads[3].pos.z = v46;
      pBillboardRenderListD3D[v8].pQuads[3].rhw = pBillboardRenderListD3D[v8].pQuads[0].rhw;
      pBillboardRenderListD3D[v8].pQuads[3].diffuse = uDiffuse;
      pBillboardRenderListD3D[v8].pTexture = a3;
      pBillboardRenderListD3D[v8].z_order = a2->zbuffer_depth;
      pBillboardRenderListD3D[v8].uNumVertices = 4;
      pBillboardRenderListD3D[v8].pQuads[3].pos.y = v45;
      pBillboardRenderListD3D[v8].pQuads[3].texcoord.x = 1.0;
      pBillboardRenderListD3D[v8].pQuads[3].texcoord.y = 0.0;
    }
  }
}

//----- (004A3AD9) --------------------------------------------------------
void Render::MakeParticleBillboardAndPush_ODM(RenderBillboardTransform_local0 *a2, IDirect3DTexture2 *a3, unsigned int uDiffuse, int angle)
{
  double v5; // st7@2
  float v6; // ST28_4@2
  float v7; // ST00_4@2
  unsigned int v8; // esi@2
  int v9; // eax@2
  int v10; // ebx@2
  float v11; // ST34_4@2
  double v12; // st7@2
  float v13; // ST2C_4@2
  double v14; // st6@2
  float v15; // ST24_4@2
  float v16; // ST38_4@2
  float v17; // ST3C_4@2
  signed int v18; // ST1C_4@2
  int v19; // ST30_4@2
  signed int v20; // ST20_4@2
  signed int v21; // ST18_4@2
  signed int v22; // eax@2
  double v23; // st6@2
  float v24; // ST20_4@2
  float v25; // ST1C_4@2
  float v26; // ST38_4@2
  float v27; // ST3C_4@2
  signed int v28; // ST18_4@2
  signed int v29; // ST14_4@2
  signed int v30; // ST10_4@2
  signed int v31; // eax@2
  double v32; // st6@2
  float v33; // ST38_4@2
  float v34; // ST3C_4@2
  signed int v35; // ST10_4@2
  signed int v36; // ST14_4@2
  signed int v37; // ST18_4@2
  signed int v38; // eax@2
  double v39; // st6@2
  float v40; // ST38_4@2
  float v41; // ST3C_4@2
  signed int v42; // ST10_4@2
  signed int v43; // ST14_4@2
  signed int v44; // ST18_4@2
  signed int v45; // eax@2
  double v46; // st6@2

  if ( this->uNumD3DSceneBegins )
  {
    v5 = (double)a2->zbuffer_depth;
    v6 = v5;
    v7 = v5;
    v8 = Billboard_ProbablyAddToListAndSortByZOrder(LODWORD(v7));
    pBillboardRenderListD3D[v8].opacity = RenderBillboardD3D::Opaque_1;
    pBillboardRenderListD3D[v8].field_90 = a2->field_44;
    pBillboardRenderListD3D[v8].sZValue = a2->sZValue;
    pBillboardRenderListD3D[v8].sParentBillboardID = a2->sParentBillboardID;
    v9 = a2->uScreenSpaceX;
    v10 = a2->uScreenSpaceY;
    v11 = (a2->_screenspace_x_scaler_packedfloat & 0xFFFF) * 0.000015260186 + HIWORD(a2->_screenspace_x_scaler_packedfloat);
    v12 = (double)v9;
    v13 = v12;
    v14 = (double)(v10 - 12);
    v15 = v14;
    v16 = (double)(v9 - 12) - v12;
    v17 = (double)(v10 - 25) - v14;
    v18 = stru_5C6E00->Cos(angle);
    v19 = angle - stru_5C6E00->uIntegerHalfPi;
    v20 = stru_5C6E00->Sin(angle);
    v21 = stru_5C6E00->Sin(angle);
    v22 = stru_5C6E00->Cos(angle);
    pBillboardRenderListD3D[v8].pQuads[0].pos.x = (((double)(unsigned __int16)v18 * 0.000015259022
                                                    + (double)(v18 >> 16))
                                                    * v16
                                                    - ((double)(unsigned __int16)v20 * 0.000015259022
                                                    + (double)(v20 >> 16))
                                                    * v17)
                                                    * v11 + v13;
    v23 = (((double)(unsigned __int16)v22 * 0.000015259022 + (double)(v22 >> 16)) * v17
         + ((double)(unsigned __int16)v21 * 0.000015259022 + (double)(v21 >> 16)) * v16
         - 12.0)
        * v11
        + (double)a2->uScreenSpaceY;
    pBillboardRenderListD3D[v8].pQuads[0].specular = 0;
    pBillboardRenderListD3D[v8].pQuads[0].diffuse = uDiffuse;
    pBillboardRenderListD3D[v8].pQuads[0].pos.y = v23;
    v24 = 1.0 - 1.0 / (v6 * 1000.0 / (double)pODMRenderParams->shading_dist_mist);
    pBillboardRenderListD3D[v8].pQuads[0].pos.z = v24;
    v25 = 1.0 / v6;
    pBillboardRenderListD3D[v8].pQuads[0].rhw = v25;
    pBillboardRenderListD3D[v8].pQuads[0].texcoord.x = 0.0;
    pBillboardRenderListD3D[v8].pQuads[0].texcoord.y = 0.0;
    v26 = (double)(a2->uScreenSpaceX - 12) - v13;
    v27 = (double)a2->uScreenSpaceY - v15;
    v28 = stru_5C6E00->Cos(angle);
    v29 = stru_5C6E00->Sin(v19 + stru_5C6E00->uIntegerHalfPi);
    v30 = stru_5C6E00->Sin(v19 + stru_5C6E00->uIntegerHalfPi);
    v31 = stru_5C6E00->Cos(angle);
    pBillboardRenderListD3D[v8].pQuads[1].pos.x = (((double)(unsigned __int16)v28 * 0.000015259022
                                                     + (double)(v28 >> 16))
                                                     * v26
                                                     - ((double)(unsigned __int16)v29 * 0.000015259022
                                                     + (double)(v29 >> 16))
                                                     * v27)
                                                     * v11 + v13;
    v32 = (((double)(unsigned __int16)v31 * 0.000015259022 + (double)(v31 >> 16)) * v27
         + ((double)(unsigned __int16)v30 * 0.000015259022 + (double)(v30 >> 16)) * v26
         - 12.0)
        * v11
        + (double)a2->uScreenSpaceY;
    pBillboardRenderListD3D[v8].pQuads[1].pos.z = v24;
    pBillboardRenderListD3D[v8].pQuads[1].pos.y = v32;
    pBillboardRenderListD3D[v8].pQuads[1].specular = 0;
    pBillboardRenderListD3D[v8].pQuads[1].rhw = v25;
    pBillboardRenderListD3D[v8].pQuads[1].diffuse = uDiffuse;
    pBillboardRenderListD3D[v8].pQuads[1].texcoord.x = 0.0;
    pBillboardRenderListD3D[v8].pQuads[1].texcoord.y = 1.0;
    v33 = (double)(a2->uScreenSpaceX + 12) - v13;
    v34 = (double)a2->uScreenSpaceY - v15;
    v35 = stru_5C6E00->Cos(angle);
    v36 = stru_5C6E00->Sin(v19 + stru_5C6E00->uIntegerHalfPi);
    v37 = stru_5C6E00->Sin(v19 + stru_5C6E00->uIntegerHalfPi);
    v38 = stru_5C6E00->Cos(angle);
    pBillboardRenderListD3D[v8].pQuads[2].pos.x = (((double)(unsigned __int16)v35 * 0.000015259022
                                                     + (double)(v35 >> 16))
                                                     * v33
                                                     - ((double)(unsigned __int16)v36 * 0.000015259022
                                                     + (double)(v36 >> 16))
                                                     * v34)
                                                     * v11 + v13;
    v39 = (((double)(unsigned __int16)v38 * 0.000015259022 + (double)(v38 >> 16)) * v34
         + ((double)(unsigned __int16)v37 * 0.000015259022 + (double)(v37 >> 16)) * v33
         - 12.0)
        * v11
        + (double)a2->uScreenSpaceY;
    pBillboardRenderListD3D[v8].pQuads[2].specular = 0;
    pBillboardRenderListD3D[v8].pQuads[2].pos.z = v24;
    pBillboardRenderListD3D[v8].pQuads[2].rhw = v25;
    pBillboardRenderListD3D[v8].pQuads[2].diffuse = uDiffuse;
    pBillboardRenderListD3D[v8].pQuads[2].pos.y = v39;
    pBillboardRenderListD3D[v8].pQuads[2].texcoord.x = 1.0;
    pBillboardRenderListD3D[v8].pQuads[2].texcoord.y = 1.0;
    v40 = (double)(a2->uScreenSpaceX + 12) - v13;
    v41 = (double)(a2->uScreenSpaceY - 25) - v15;
    v42 = stru_5C6E00->Cos(angle);
    v43 = stru_5C6E00->Sin(v19 + stru_5C6E00->uIntegerHalfPi);
    v44 = stru_5C6E00->Sin(v19 + stru_5C6E00->uIntegerHalfPi);
    v45 = stru_5C6E00->Cos(angle);
    pBillboardRenderListD3D[v8].pQuads[3].pos.x = (((double)(unsigned __int16)v42 * 0.000015259022
                                                     + (double)(v42 >> 16))
                                                     * v40
                                                     - ((double)(unsigned __int16)v43 * 0.000015259022
                                                     + (double)(v43 >> 16))
                                                     * v41)
                                                     * v11 + v13;
    v46 = (((double)(unsigned __int16)v45 * 0.000015259022 + (double)(v45 >> 16)) * v41
         + ((double)(unsigned __int16)v44 * 0.000015259022 + (double)(v44 >> 16)) * v40
         - 12.0)
        * v11
        + (double)a2->uScreenSpaceY;
    pBillboardRenderListD3D[v8].pQuads[3].specular = 0;
    pBillboardRenderListD3D[v8].pQuads[3].pos.z = v24;
    pBillboardRenderListD3D[v8].pQuads[3].rhw = v25;
    pBillboardRenderListD3D[v8].pQuads[3].diffuse = uDiffuse;
    pBillboardRenderListD3D[v8].pTexture = a3;
    pBillboardRenderListD3D[v8].z_order = v6;
    pBillboardRenderListD3D[v8].uNumVertices = 4;
    pBillboardRenderListD3D[v8].pQuads[3].pos.y = v46;
    pBillboardRenderListD3D[v8].pQuads[3].texcoord.x = 1.0;
    pBillboardRenderListD3D[v8].pQuads[3].texcoord.y = 0.0;
  }
}

//----- (004A4023) --------------------------------------------------------
void Render::TransformBillboard(RenderBillboardTransform_local0 *a2, Sprite *pSprite, int dimming_level, RenderBillboard *pBillboard)
{
  unsigned int v8; // esi@2
  double v14; // st6@14
  double v15; // st5@14
  //unsigned int v16; // ecx@16
  //double v17; // st7@16
  //double v18; // st5@16
  //double v19; // st4@16
  //double v20; // st5@18
  //double v21; // st4@18
  //signed int v23; // [sp+18h] [bp-18h]@5
  //signed int v24; // [sp+1Ch] [bp-14h]@5
  //float v26; // [sp+20h] [bp-10h]@5
  //float v27; // [sp+20h] [bp-10h]@12
  float v29; // [sp+28h] [bp-8h]@5
  float v30; // [sp+2Ch] [bp-4h]@5
  //float pSpritea; // [sp+3Ch] [bp+Ch]@5

  if (!uNumD3DSceneBegins)
    return;

  v8 = Billboard_ProbablyAddToListAndSortByZOrder(a2->zbuffer_depth);

  v30 = (a2->_screenspace_x_scaler_packedfloat & 0xFFFF) / 65530.0 + HIWORD(a2->_screenspace_x_scaler_packedfloat);
  v29 = (a2->_screenspace_y_scaler_packedfloat & 0xFFFF) / 65530.0 + HIWORD(a2->_screenspace_y_scaler_packedfloat);

  unsigned int diffuse = ::GetActorTintColor(dimming_level, 0, a2->zbuffer_depth, 0, pBillboard);
  if (a2->sTintColor & 0x00FFFFFF && bTinting)
  {
    diffuse = BlendColors(a2->sTintColor, diffuse);
    if (a2->sTintColor & 0xFF000000)
      diffuse = 0x007F7F7F & ((unsigned int)diffuse >> 1);
  }

  unsigned int specular = 0;
  if (bUsingSpecular)
    specular = sub_47C3D7_get_fog_specular(0, 0, a2->zbuffer_depth);

  v14 = (double)((int)pSprite->uBufferWidth / 2 - pSprite->uAreaX);
  v15 = (double)((int)pSprite->uBufferHeight - pSprite->uAreaY);
  if (a2->uFlags & 4)
    v14 *= -1.0;
  pBillboardRenderListD3D[v8].pQuads[0].diffuse = diffuse;
  pBillboardRenderListD3D[v8].pQuads[0].pos.x = (double)a2->uScreenSpaceX - v14 * v30;
  pBillboardRenderListD3D[v8].pQuads[0].pos.y = (double)a2->uScreenSpaceY - v15 * v29;
  pBillboardRenderListD3D[v8].pQuads[0].pos.z = 1.0 - 1.0 / (a2->zbuffer_depth * 1000.0 / (double)pODMRenderParams->shading_dist_mist);
  pBillboardRenderListD3D[v8].pQuads[0].rhw = 1.0 / a2->zbuffer_depth;
  pBillboardRenderListD3D[v8].pQuads[0].specular = specular;
  pBillboardRenderListD3D[v8].pQuads[0].texcoord.x = 0.0;
  pBillboardRenderListD3D[v8].pQuads[0].texcoord.y = 0.0;

    v14 = (double)((int)pSprite->uBufferWidth / 2 - pSprite->uAreaX);
    v15 = (double)((int)pSprite->uBufferHeight - pSprite->uAreaHeight - pSprite->uAreaY);
    if ( a2->uFlags & 4 )
      v14 = v14 * -1.0;
    pBillboardRenderListD3D[v8].pQuads[1].specular = specular;
    pBillboardRenderListD3D[v8].pQuads[1].diffuse = diffuse;
    pBillboardRenderListD3D[v8].pQuads[1].pos.x = (double)a2->uScreenSpaceX - v14 * v30;
    pBillboardRenderListD3D[v8].pQuads[1].pos.y = (double)a2->uScreenSpaceY - v15 * v29;
    pBillboardRenderListD3D[v8].pQuads[1].pos.z = 1.0 - 1.0 / (a2->zbuffer_depth * 1000.0 / (double)pODMRenderParams->shading_dist_mist);
    pBillboardRenderListD3D[v8].pQuads[1].rhw = 1.0 / a2->zbuffer_depth;
    pBillboardRenderListD3D[v8].pQuads[1].texcoord.x = 0.0;
    pBillboardRenderListD3D[v8].pQuads[1].texcoord.y = 1.0;

    v14 = (double)((int)pSprite->uAreaWidth + pSprite->uAreaX + pSprite->uBufferWidth / 2 - pSprite->uBufferWidth);
    v15 = (double)((int)pSprite->uBufferHeight - pSprite->uAreaHeight - pSprite->uAreaY);
    if ( a2->uFlags & 4 )
      v14 *= -1.0;
    pBillboardRenderListD3D[v8].pQuads[2].diffuse = diffuse;
    pBillboardRenderListD3D[v8].pQuads[2].specular = specular;
    pBillboardRenderListD3D[v8].pQuads[2].pos.x = (double)a2->uScreenSpaceX + v14 * v30;
    pBillboardRenderListD3D[v8].pQuads[2].pos.y = (double)a2->uScreenSpaceY - v15 * v29;
    pBillboardRenderListD3D[v8].pQuads[2].pos.z = 1.0 - 1.0 / (a2->zbuffer_depth * 1000.0 / (double)pODMRenderParams->shading_dist_mist);
    pBillboardRenderListD3D[v8].pQuads[2].rhw = 1.0 / a2->zbuffer_depth;
    pBillboardRenderListD3D[v8].pQuads[2].texcoord.x = 1.0;
    pBillboardRenderListD3D[v8].pQuads[2].texcoord.y = 1.0;

    v14 = (double)((int)pSprite->uAreaWidth + pSprite->uAreaX + pSprite->uBufferWidth / 2 - pSprite->uBufferWidth);
    v15 = (double)((int)pSprite->uBufferHeight - pSprite->uAreaY);
    if ( a2->uFlags & 4 )
      v14 *= -1.0;
    pBillboardRenderListD3D[v8].pQuads[3].diffuse = diffuse;
    pBillboardRenderListD3D[v8].pQuads[3].specular = specular;
    pBillboardRenderListD3D[v8].pQuads[3].pos.x = (double)a2->uScreenSpaceX + v14 * v30;
    pBillboardRenderListD3D[v8].pQuads[3].pos.y = (double)a2->uScreenSpaceY - v15 * v29;
    pBillboardRenderListD3D[v8].pQuads[3].pos.z = 1.0 - 1.0 / (a2->zbuffer_depth * 1000.0 / (double)pODMRenderParams->shading_dist_mist);
    pBillboardRenderListD3D[v8].pQuads[3].rhw = 1.0 / a2->zbuffer_depth;
    pBillboardRenderListD3D[v8].pQuads[3].texcoord.x = 1.0;
    pBillboardRenderListD3D[v8].pQuads[3].texcoord.y = 0.0;

  pBillboardRenderListD3D[v8].uNumVertices = 4;
  pBillboardRenderListD3D[v8].pTexture = pSprite->pTexture;
  pBillboardRenderListD3D[v8].z_order = a2->zbuffer_depth;
  pBillboardRenderListD3D[v8].field_90 = a2->field_44;
  pBillboardRenderListD3D[v8].sZValue = a2->sZValue;
  pBillboardRenderListD3D[v8].sParentBillboardID = a2->sParentBillboardID;

  if (a2->sTintColor & 0xFF000000)
    pBillboardRenderListD3D[v8].opacity = RenderBillboardD3D::Opaque_3;
  else
    pBillboardRenderListD3D[v8].opacity = RenderBillboardD3D::Transparent;
}


//----- (004A49D0) --------------------------------------------------------
void Render::DrawProjectile(float srcX, float srcY, float a3, float a4, float dstX, float dstY, float a7, float a8, IDirect3DTexture2 *a9)
{
  int absXDifference; // eax@1
  int absYDifference; // eax@1
  unsigned int smallerabsdiff; // ebx@1
  unsigned int largerabsdiff;
  double v16; // st7@7
  double v17; // st7@7
  double v18; // st6@7
  double v20; // st4@8
  double v21; // st4@10
  double v22; // st4@10
  double v23; // st4@10
  double v25; // st4@11
  double v26; // st4@13
  double v28; // st4@13
  RenderVertexD3D3 v29[4]; // [sp+0h] [bp-94h]@7
  int xDifference; // [sp+88h] [bp-Ch]@1
  signed int v32; // [sp+8Ch] [bp-8h]@1
  int yDifference; // [sp+90h] [bp-4h]@1

  xDifference = bankersRounding(dstX - srcX);
  yDifference = bankersRounding(dstY - srcY);
  absYDifference = abs(yDifference);
  absXDifference = abs(xDifference);
  smallerabsdiff = min(absXDifference, absYDifference);
  largerabsdiff = max(absXDifference, absYDifference);
  v32 = (11 * smallerabsdiff >> 5) + largerabsdiff;
  v16 = 1.0 / (double)v32;
  v17 = (double)yDifference * v16 * a4;
  v18 = (double)xDifference * v16 * a4;
  if ( uCurrentlyLoadedLevelType == LEVEL_Outdoor )
  {
    v20 = a3 * 1000.0 / (double)pODMRenderParams->shading_dist_mist;
    v25 = a7 * 1000.0 / (double)pODMRenderParams->shading_dist_mist;
  }
  else
  {
    v20 = a3 * 0.061758894;
    v25 = a7 * 0.061758894;
  }
  v21 = 1.0 / a3;
  v22 = (double)yDifference * v16 * a8;
  v23 = (double)xDifference * v16 * a8;
  v26 = 1.0 - 1.0 / v25;
  v28 = 1.0 / a7;
  v29[0].pos.x = srcX + v17;
  v29[0].pos.y = srcY - v18;
  v29[0].pos.z = 1.0 - 1.0 / v20;
  v29[0].rhw = v21;
  v29[0].diffuse = -1;
  v29[0].specular = 0;
  v29[0].texcoord.x = 1.0;
  v29[0].texcoord.y = 0.0;

  v29[1].pos.x = v22 + dstX;
  v29[1].pos.y = dstY - v23;
  v29[1].pos.z = v26;
  v29[1].rhw = v28;
  v29[1].diffuse = -16711936;
  v29[1].specular = 0;
  v29[1].texcoord.x = 1.0;
  v29[1].texcoord.y = 1.0;

  v29[2].pos.x = dstX - v22;
  v29[2].pos.y = v23 + dstY;
  v29[2].pos.z = v26;
  v29[2].rhw = v28;
  v29[2].diffuse = -1;
  v29[2].specular = 0;
  v29[2].texcoord.x = 0.0;
  v29[2].texcoord.y = 1.0;

  v29[3].pos.x = srcX - v17;
  v29[3].pos.y = v18 + srcY;
  v29[3].pos.z = v29[0].pos.z;
  v29[3].rhw = v21;
  v29[3].diffuse = -1;
  v29[3].specular = 0;
  v29[3].texcoord.x = 0.0;
  v29[3].texcoord.y = 0.0;
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE));
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE));
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE));
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DITHERENABLE, FALSE));
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE));
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE));
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetTexture(0, a9));
  ErrD3D(pRenderer->pRenderD3D->pDevice->DrawPrimitive(
    D3DPT_TRIANGLEFAN,
    D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1,
    v29,
    4,
    24));
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE));
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE));
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ZERO));
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DITHERENABLE, TRUE));
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE));
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CW));
}

//----- (004A4CC9) --------------------------------------------------------
void Render::_4A4CC9_AddSomeBillboard(stru6_stru1_indoor_sw_billboard *a1, int diffuse)
{
  unsigned int v5; // eax@7
//  char *v7; // edx@8
  double v10; // st6@9
  double v11; // st6@10
  int v12; // ebx@13

  if (a1->uNumVertices < 3)
    return;

  float depth = 1000000.0;
  for (uint i = 0; i < a1->uNumVertices; ++i)
  {
    if (a1->field_104[i].z < depth)
      depth = a1->field_104[i * 4].z;
  }

  v5 = Billboard_ProbablyAddToListAndSortByZOrder(depth);
  pBillboardRenderListD3D[v5].field_90 = 0;
  pBillboardRenderListD3D[v5].sParentBillboardID = -1;
  pBillboardRenderListD3D[v5].opacity = RenderBillboardD3D::Opaque_2;
  pBillboardRenderListD3D[v5].pTexture = 0;
  pBillboardRenderListD3D[v5].uNumVertices = a1->uNumVertices;
  pBillboardRenderListD3D[v5].z_order = depth;

  for (uint i = 0; i < a1->uNumVertices; ++i)
  {
    pBillboardRenderListD3D[v5].pQuads[i].pos.x = a1->field_104[i].x;
    pBillboardRenderListD3D[v5].pQuads[i].pos.y = a1->field_104[i].y;

    v10 = a1->field_104[i].z;
    if (uCurrentlyLoadedLevelType == LEVEL_Indoor)
      v11 = v10 * 0.061758894;
    else
      v11 = v10 * 1000.0 / (double)pODMRenderParams->shading_dist_mist;
    pBillboardRenderListD3D[v5].pQuads[i].pos.z = 1.0 - 1.0 / v11;
    pBillboardRenderListD3D[v5].pQuads[i].rhw = 1.0 / a1->field_104[i].z;

    if (diffuse & 0xFF000000)
      v12 = a1->field_104[i].diffuse;
    else
      v12 = diffuse;
    pBillboardRenderListD3D[v5].pQuads[i].diffuse = v12;
    pBillboardRenderListD3D[v5].pQuads[i].specular = 0;

    pBillboardRenderListD3D[v5].pQuads[i].texcoord.x = 0.0;
    pBillboardRenderListD3D[v5].pQuads[i].texcoord.y = 0.0;
  }
}

//----- (004A4DE1) --------------------------------------------------------
bool Render::LoadTexture(const char *pName, unsigned int bMipMaps, IDirectDrawSurface4 **pOutSurface, IDirect3DTexture2 **pOutTexture)
{
//  HRESULT v12; // eax@14
  unsigned __int16 *v13; // ecx@19
  unsigned __int16 *v14; // eax@19
  DWORD v15; // edx@20
//  HRESULT v16; // eax@23
  stru350 Dst; // [sp+Ch] [bp-F8h]@12

  HWLTexture* pHWLTexture = pD3DBitmaps.LoadTexture(pName, bMipMaps);
  if ( pHWLTexture )
  {
    bMipMaps = !strncmp(pName, "HDWTR", 5);
    if ( !pRenderD3D->CreateTexture(
            pHWLTexture->uWidth,
            pHWLTexture->uHeight,
            pOutSurface,
            pOutTexture,
            true,
            bMipMaps,
            uMinDeviceTextureDim) )
      Error("HiScreen16::LoadTexture - D3Drend->CreateTexture() failed: %x", 0);
    //v10 = *pOutSurface;
    //v11 = 0;
    if (bMipMaps)
    {
      Dst._450DDE();
      //v20 = 0;
      Dst._450DF1(&stru_4EFCBC, &stru_4EFCBC);

      IDirectDrawSurface4 *pNextSurf = *pOutSurface;
      while ( 1 )
      {
        DDSCAPS2 v19;
        memset(&v19, 0, sizeof(DDSCAPS2));
        v19.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;

        DDSURFACEDESC2 desc;
        memset(&desc, 0, sizeof(DDSURFACEDESC2));
        desc.dwSize = sizeof(DDSURFACEDESC2);

        if ( LockSurface_DDraw4(pNextSurf, &desc, DDLOCK_WAIT | DDLOCK_WRITEONLY) )
        {
          Dst.sub_451007_scale_image_bicubic(
            pHWLTexture->pPixels,
            pHWLTexture->uWidth,
            pHWLTexture->uHeight,
            pHWLTexture->uWidth,
            (unsigned short *)desc.lpSurface,
            desc.dwWidth,
            desc.dwHeight,
            desc.lPitch >> 1,
            0,
            0);
          ErrD3D(pNextSurf->Unlock(0));
          //bMipMaps = 0x4D86ACu;
        }
        if (FAILED(pNextSurf->GetAttachedSurface(&v19, &pNextSurf)))
          break;
        //v10 = (IDirectDrawSurface4 *)pName;
        //v11 = 0;
      }
      //v20 = -1;
      //nullsub_1();
    }
    else
    {
        DDSCAPS2 v19;
        memset(&v19, 0, sizeof(DDSCAPS2));
        v19.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;

        DDSURFACEDESC2 desc;
        memset(&desc, 0, sizeof(DDSURFACEDESC2));
        desc.dwSize = sizeof(DDSURFACEDESC2);

      if ( LockSurface_DDraw4(*pOutSurface, &desc, DDLOCK_WAIT | DDLOCK_WRITEONLY) )
      {
        bMipMaps = 0;
        v13 = pHWLTexture->pPixels;
        v14 = (unsigned __int16 *)desc.lpSurface;
        for(int bMipMaps = 0; bMipMaps < desc.dwHeight; bMipMaps++)
        {
          for (v15 = 0; v15 < desc.dwWidth; v15++)
          {
            *v14 = *v13;
            ++v14;
            ++v13;
          }
          v14 += (desc.lPitch >> 1) - desc.dwWidth;
        }
        ErrD3D((*pOutSurface)->Unlock(0));
      }
    }
    delete [] pHWLTexture->pPixels;
    delete pHWLTexture;
    return true;
  }
  return false;
}

//----- (004A5048) --------------------------------------------------------
bool Render::MoveSpriteToDevice( Sprite *pSprite )
    {

  HWLTexture *sprite_texture; // eax@1
  unsigned __int16 *v9; // edx@5
  LPVOID v10; // eax@5
  DDSURFACEDESC2 Dst; // [sp+Ch] [bp-7Ch]@4

  sprite_texture = pD3DSprites.LoadTexture(pSprite->pName, pSprite->uPaletteID);
  if ( sprite_texture )
  {
    pSprite->uAreaX = sprite_texture->uAreaX;
    pSprite->uAreaY = sprite_texture->uAreaY;
    pSprite->uBufferWidth = sprite_texture->uBufferWidth;
    pSprite->uBufferHeight = sprite_texture->uBufferHeight;
    pSprite->uAreaWidth = sprite_texture->uAreaWidth;
    pSprite->uAreaHeight = sprite_texture->uAreaHeigth;
    //v6 = v3->uMinDeviceTextureDim;
    //v7 = v3->pRenderD3D;
    if (!pRenderD3D->CreateTexture(sprite_texture->uWidth, sprite_texture->uHeight, &pSprite->pTextureSurface, &pSprite->pTexture, 1u, 0, uMinDeviceTextureDim))
      Error("HiScreen16::LoadTexture - D3Drend->CreateTexture() failed: %x", 0);
    //pSprite = v2->pTextureSurface;
    //pSprite = (Sprite *)pSprite->pName;
    //v8 = pSprite;
    memset(&Dst, 0, sizeof(DDSURFACEDESC2));
    Dst.dwSize = 124;
    if ( LockSurface_DDraw4((IDirectDrawSurface4 *)pSprite->pTextureSurface, &Dst, DDLOCK_WAIT | DDLOCK_WRITEONLY) )
    {
    v9 = sprite_texture->pPixels;
    v10 = Dst.lpSurface;
      for (int i=0; i<sprite_texture->uHeight; ++i)
          {
          for (int j=0; j<sprite_texture->uWidth/2; ++j)
              {
              *(int *)v10 = *(int *)v9;
              v9 += 2;
              v10 = (char *)v10 + 4;
              }
          v10 = (char *)v10+Dst.lPitch-sprite_texture->uWidth*2;
          }
      ErrD3D(pSprite->pTextureSurface->Unlock(0));
    }
    free(sprite_texture->pPixels);
    free(sprite_texture);
    return true;
  }
  return false;
}

//----- (004A51CB) --------------------------------------------------------
void Render::BeginScene()
{
  //Render *v1; // esi@1
  unsigned int v2; // eax@1
/*int v3; // eax@5
  unsigned __int16 **v4; // edi@6
  char *v5; // ebx@7*/
//  DDSURFACEDESC2 Dst; // [sp+Ch] [bp-7Ch]@4

  //v1 = this;
  v2 = this->uNumSceneBegins;
  this->uNumSceneBegins = v2 + 1;
  if ( !v2 )
  {
    if ( this->pRenderD3D )
    {
      /*if ( this->bColorKeySupported )
      {
        memset(&Dst, 0, 0x7Cu);
        Dst.dwSize = 124;
        if ( LockSurface_DDraw4(this->pColorKeySurface4, &Dst, 0x800 | DDLOCK_WAIT) )
        {
          this->pTargetSurface = (unsigned __int16 *)Dst.lpSurface;
          this->uTargetSurfacePitch = Dst.lPitch >> 1;
          this->field_18_locked_pitch = Dst.lPitch >> 1;
        }
        --this->uNumSceneBegins;
      }*/
    }
    else
    {
      if ( !this->pTargetSurface )
      {
        LockRenderSurface((void **)&this->pTargetSurface, &this->uTargetSurfacePitch);
        /*if ( this->pTargetSurface )
        {
          this->field_18_locked_pitch = this->uTargetSurfacePitch;
        }*/
        --this->uNumSceneBegins;
      }
    }
    RestoreFrontBuffer();
  }
}

//----- (004A527D) --------------------------------------------------------
void Render::EndScene()
{
  if ( this->uNumSceneBegins )
  {
    this->uNumSceneBegins--;
    if ( !this->uNumSceneBegins )
    {
      if ( this->pRenderD3D )
      {
        /*if ( this->bColorKeySupported )
        {
          this->pTargetSurface = 0;
          this->uTargetSurfacePitch = 0;
          this->field_18_locked_pitch = 0;
          ErrD3D(this->pColorKeySurface4->Unlock(0));
        }*/
      }
      else
      {
        this->pTargetSurface = 0;
        this->uTargetSurfacePitch = 0;
        //this->field_18_locked_pitch = 0;
        UnlockBackBuffer();
      }
    }
  }
}

//----- (004A52F1) --------------------------------------------------------
void Render::ScreenFade(unsigned int color, float t)
{
  unsigned int v3; // esi@1
  //double v4; // st7@2
  //double v5; // st7@6
  double v6; // st6@6
  unsigned int v7; // eax@6
  double v8; // st5@6
  double v9; // st4@6
//  HRESULT v10; // eax@6
//  HRESULT v11; // eax@6
//  unsigned int result; // eax@6
//  unsigned int v13; // eax@7
//  unsigned __int16 *v14; // ecx@7
//  int *v15; // eax@7
//  unsigned int v16; // ecx@8
//  __int16 v17; // ax@10
//  int v18; // esi@10
//  float v19; // edi@10
//  void *v20; // esi@10
//  int v21; // edx@10
//  int v22; // ecx@11
//  int v23; // edx@12
//  __int16 v24; // ax@15
//  int v25; // esi@15
//  float v26; // edi@15
//  char *v27; // esi@15
//  int v28; // edx@15
//  int v29; // ecx@16
//  int v30; // edx@17
//  int v31; // [sp-Ch] [bp-ACh]@11
//  int v32; // [sp-Ch] [bp-ACh]@16
//  const char *v33; // [sp+0h] [bp-A0h]@0
//  int v34; // [sp+4h] [bp-9Ch]@0
//  unsigned int v35; // [sp+8h] [bp-98h]@0
  RenderVertexD3D3 v36[4]; // [sp+Ch] [bp-94h]@6
//  unsigned int v37; // [sp+8Ch] [bp-14h]@7
//  int v38; // [sp+90h] [bp-10h]@7
//  double v39; // [sp+94h] [bp-Ch]@6
  int v40; // [sp+9Ch] [bp-4h]@6

  v3 = 0;

  //{
  if (t > 1.0f)
    t = 1.0f;
  else if (t < 0.0f)
    t = 0.0f;

    v40 = (char)floorf(t * 255.0f + 0.5f);
    //v5 = (double)(signed int)pViewport->uViewportTL_X;
    v36[0].pos.x = pViewport->uViewportTL_X;
    v6 = (double)(signed int)pViewport->uViewportTL_Y;
    v7 = color | (v40 << 24);
    //this_ = pViewport->uViewportBR_Y + 1;
    v36[0].specular = 0;
    v36[0].pos.y = v6;
    v36[0].diffuse = v7;
    v36[1].diffuse = v7;
    v36[0].pos.z = 0.0;
    v36[2].diffuse = v7;
    v36[3].diffuse = v7;
    v36[0].rhw = 1.0;
    v36[1].specular = 0;
    v36[0].texcoord.x = 0.0;
    v36[2].specular = 0;
    v36[3].specular = 0;
    v36[0].texcoord.y = 0.0;
    v36[1].pos.x = pViewport->uViewportTL_X;
    v8 = (double)(pViewport->uViewportBR_Y + 1);
    v36[1].pos.y = v8;
    v36[1].pos.z = 0.0;
    v36[1].rhw = 1.0;
    v36[1].texcoord.x = 0.0;
    v36[1].texcoord.y = 0.0;
    v9 = (double)(signed int)pViewport->uViewportBR_X;
    v36[2].pos.x = v9;
    v36[2].pos.y = v8;
    v36[2].pos.z = 0.0;
    v36[2].rhw = 1.0;
    v36[2].texcoord.x = 0.0;
    v36[2].texcoord.y = 0.0;
    v36[3].pos.x = v9;
    v36[3].pos.y = v6;
    v36[3].pos.z = 0.0;
    v36[3].rhw = 1.0;
    v36[3].texcoord.x = 0.0;
    v36[3].texcoord.y = 0.0;
    ErrD3D(pRenderD3D->pDevice->SetTexture(0, 0));
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE));
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE));
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE));
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA));
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA));
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DITHERENABLE, FALSE));
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZFUNC, D3DCMP_ALWAYS));
    ErrD3D(pRenderD3D->pDevice->DrawPrimitive(
      D3DPT_TRIANGLEFAN,
      D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1,
      v36,
      4,
      28));
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE));
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ZERO));
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE));
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE));
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DITHERENABLE, TRUE));
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZFUNC, D3DCMP_LESS));
  /*}
  else
  {
    v40 = (1.0 - a3) * 65536.0;
    v39 = v40 + 6.7553994e15;
    LODWORD(a3) = LODWORD(v39);
    v38 = (signed int)(pViewport->uViewportBR_X - pViewport->uViewportTL_X) >> 1;
    HIDWORD(v39) = pViewport->uViewportBR_Y - pViewport->uViewportTL_Y + 1;
    v13 = pViewport->uViewportTL_X + ecx0->uTargetSurfacePitch - pViewport->uViewportBR_X;
    v14 = &ecx0->pTargetSurface[pViewport->uViewportTL_X + pViewport->uViewportTL_Y * ecx0->uTargetSurfacePitch];
    v37 = 2 * v13;
    LODWORD(v40) = (int)v14;

    int __i = 0;
    v15 = dword_F1B430.data();
    do
    {
      v16 = v3;
      v3 += LODWORD(a3);
      dword_F1B430[__i++] = v16 >> 16;
    }
    //while ( (signed int)v15 < (signed int)&Aureal3D_SplashScreen );
    while (__i < 32);

    if ( pRenderer->uTargetGBits == 6 )
    {
      v17 = sr_42690D_colors_cvt(this_);
      v18 = (65536 - LODWORD(a3)) * (v17 & 0x1F);
      this_ = (((65536 - LODWORD(a3)) * (unsigned __int16)(v17 & 0xF800) & 0xF800FFFF | v18 & 0x1F0000 | (65536 - LODWORD(a3)) * (v17 & 0x7E0) & 0x7E00000u) >> 16 << 16) | (((65536 - LODWORD(a3)) * (unsigned __int16)(v17 & 0xF800) & 0xF800FFFF | v18 & 0x1F0000 | (65536 - LODWORD(a3)) * (v17 & 0x7E0) & 0x7E00000u) >> 16);
      v19 = v40;
      v20 = off_4EFDB0;
      v21 = HIDWORD(v39);
      do
      {
        v22 = v38;
        v31 = v21;
        do
        {
          v23 = (*(int *)((char *)v20
                           + ((((unsigned __int16)(*(short *)((char *)v20 + ((*(unsigned int *)LODWORD(v19) & 0xF800u) >> 9)) << 11) | *(unsigned int *)LODWORD(v19) & 0x7FF) & 0x7C0u) >> 4)) << 6) | (*(int *)((char *)v20 + ((((*(int *)((char *)v20 + ((*(unsigned int *)LODWORD(v19) & 0xF800u) >> 9)) << 11) | (*(int *)((char *)v20 + ((*(unsigned int *)LODWORD(v19) & 0xF8000000u) >> 25)) << 27) | *(unsigned int *)LODWORD(v19) & 0x7FF07FF) & 0x7C00000u) >> 20)) << 22) | ((*(int *)((char *)v20 + ((*(unsigned int *)LODWORD(v19) & 0xF800u) >> 9)) << 11) | (*(int *)((char *)v20 + ((*(unsigned int *)LODWORD(v19) & 0xF8000000u) >> 25)) << 27) | *(unsigned int *)LODWORD(v19) & 0x7FF07FF) & 0xF81FF81F;
          result = this_
                 + (*((int *)v20
                    + (((unsigned __int8)(*((char *)v20
                                          + ((((unsigned __int16)(*(short *)((char *)v20
                                                                           + ((*(unsigned int *)LODWORD(v19) & 0xF800u) >> 9)) << 11) | *(unsigned int *)LODWORD(v19) & 0x7FF) & 0x7C0u) >> 4)) << 6) | *(unsigned int *)LODWORD(v19) & 0x1F) & 0x1F)) | (*(int *)((char *)v20 + ((v23 & 0x1F0000u) >> 14)) << 16) | ((*(int *)((char *)v20 + ((((unsigned __int16)(*(short *)((char *)v20 + ((*(unsigned int *)LODWORD(v19) & 0xF800u) >> 9)) << 11) | *(unsigned int *)LODWORD(v19) & 0x7FF) & 0x7C0u) >> 4)) << 6) | (*(int *)((char *)v20 + ((((*(int *)((char *)v20 + ((*(unsigned int *)LODWORD(v19) & 0xF800u) >> 9)) << 11) | (*(int *)((char *)v20 + ((*(unsigned int *)LODWORD(v19) & 0xF8000000u) >> 25)) << 27) | *(unsigned int *)LODWORD(v19) & 0x7FF07FF) & 0x7C00000u) >> 20)) << 22) | ((*(int *)((char *)v20 + ((*(unsigned int *)LODWORD(v19) & 0xF800u) >> 9)) << 11) | (*(int *)((char *)v20 + ((*(unsigned int *)LODWORD(v19) & 0xF8000000u) >> 25)) << 27) | *(unsigned int *)LODWORD(v19) & 0x7FF07FF) & 0xF81FF81F) & 0xFFE0FFE0);
          *(unsigned int *)LODWORD(v19) = result;
          LODWORD(v19) += 4;
          --v22;
        }
        while ( v22 );
        LODWORD(v19) += v37;
        v21 = v31 - 1;
      }
      while ( v31 != 1 );
    }
    else
    {
      v24 = sr_4268E3_smthn_to_a1r5g5b5(this_);
      v25 = (65536 - LODWORD(a3)) * (v24 & 0x1F);
      this_ = (((65536 - LODWORD(a3)) * (v24 & 0x7C00) & 0x7C000000 | v25 & 0x1F0000 | (65536 - LODWORD(a3))
                                                                                    * (v24 & 0x3E0) & 0x3E00000u) >> 16 << 16) | (((65536 - LODWORD(a3)) * (v24 & 0x7C00) & 0x7C000000 | v25 & 0x1F0000 | (65536 - LODWORD(a3)) * (v24 & 0x3E0) & 0x3E00000u) >> 16);
      v26 = v40;
      v27 = (char *)off_4EFDB0;
      v28 = HIDWORD(v39);
      do
      {
        v29 = v38;
        v32 = v28;
        do
        {
          v30 = 32
              * *(int *)&v27[(((unsigned __int16)(*(short *)&v27[(*(unsigned int *)LODWORD(v26) & 0x7C00u) >> 8] << 10) | *(unsigned int *)LODWORD(v26) & 0x3FF) & 0x3E0u) >> 3] | (*(int *)&v27[(((*(int *)&v27[(*(unsigned int *)LODWORD(v26) & 0x7C00u) >> 8] << 10) | (*(int *)&v27[(*(unsigned int *)LODWORD(v26) & 0x7C000000u) >> 24] << 26) | *(unsigned int *)LODWORD(v26) & 0x3FF03FF) & 0x3E00000u) >> 19] << 21) | ((*(int *)&v27[(*(unsigned int *)LODWORD(v26) & 0x7C00u) >> 8] << 10) | (*(int *)&v27[(*(unsigned int *)LODWORD(v26) & 0x7C000000u) >> 24] << 26) | *(unsigned int *)LODWORD(v26) & 0x3FF03FF) & 0x7C1F7C1F;
          result = this_
                 + (*(int *)&v27[4
                                  * (((unsigned __int8)(32
                                                      * v27[(((unsigned __int16)(*(short *)&v27[(*(unsigned int *)LODWORD(v26) & 0x7C00u) >> 8] << 10) | *(unsigned int *)LODWORD(v26) & 0x3FF) & 0x3E0u) >> 3]) | *(unsigned int *)LODWORD(v26) & 0x1F) & 0x1F)] | (*(int *)&v27[(v30 & 0x1F0000u) >> 14] << 16) | (32 * *(int *)&v27[(((unsigned __int16)(*(short *)&v27[(*(unsigned int *)LODWORD(v26) & 0x7C00u) >> 8] << 10) | *(unsigned int *)LODWORD(v26) & 0x3FF) & 0x3E0u) >> 3] | (*(int *)&v27[(((*(int *)&v27[(*(unsigned int *)LODWORD(v26) & 0x7C00u) >> 8] << 10) | (*(int *)&v27[(*(unsigned int *)LODWORD(v26) & 0x7C000000u) >> 24] << 26) | *(unsigned int *)LODWORD(v26) & 0x3FF03FF) & 0x3E00000u) >> 19] << 21) | ((*(int *)&v27[(*(unsigned int *)LODWORD(v26) & 0x7C00u) >> 8] << 10) | (*(int *)&v27[(*(unsigned int *)LODWORD(v26) & 0x7C000000u) >> 24] << 26) | *(unsigned int *)LODWORD(v26) & 0x3FF03FF) & 0x7C1F7C1F) & 0xFFE0FFE0);
          *(unsigned int *)LODWORD(v26) = result;
          LODWORD(v26) += 4;
          --v29;
        }
        while ( v29 );
        LODWORD(v26) += v37;
        v28 = v32 - 1;
      }
      while ( v32 != 1 );
    }
  }*/
}

//----- (004A5B81) --------------------------------------------------------
void Render::SetTextureClipRect(unsigned int uX, unsigned int uY, unsigned int uZ, unsigned int uW)
{
  this->bClip = 1;
  this->uClipY = uY;
  this->uClipX = uX;
  this->uClipW = uW;
  this->uClipZ = uZ;
}

//----- (004A5BB6) --------------------------------------------------------
void Render::ResetTextureClipRect()
{
  this->uClipY = 0;
  this->uClipX = 0;
  this->bClip = 1;
  this->uClipW = 480;
  this->uClipZ = window->GetWidth();
}

unsigned __int32 Color32(unsigned __int16 color16)
{
  unsigned __int32 c = color16;
  unsigned int b = (c & 31) * 8;
  unsigned int g = ((c >> 5) & 63) * 4;
  unsigned int r = ((c >> 11) & 31) * 8;

  return  (r << 16) | (g << 8) | b;//
}

//----- (0040DEF3) --------------------------------------------------------
unsigned __int16 Color16(unsigned __int32 r, unsigned __int32 g, unsigned __int32 b)
{
  //return ((unsigned int)b >> (8 - LOBYTE(pRenderer->uTargetBBits))) | pRenderer->uTargetGMask & (g << (LOBYTE(pRenderer->uTargetGBits) + 
  //                         LOBYTE(pRenderer->uTargetBBits) - 8)) | pRenderer->uTargetRMask & (r << (LOBYTE(pRenderer->uTargetGBits) + 
  //                         LOBYTE(pRenderer->uTargetRBits) + LOBYTE(pRenderer->uTargetBBits) - 8));
  return (b >> (8 - 5)) |
         0x7E0 & (g << (6 + 5 - 8)) |
         0xF800 & (r << (6 + 5 + 5 - 8));
}


//----- (004A5BE3) --------------------------------------------------------
void Render::DrawTextureRGB(unsigned int uOutX, unsigned int uOutY, RGBTexture *a4)
{
  int v4; // edi@3
  //unsigned __int16 *v5; // edx@3
  unsigned __int16 *v6; // esi@3
  unsigned int v8; // eax@5
  unsigned int v9; // ebx@5
  unsigned int v11; // eax@7
  unsigned int v12; // ebx@8
  unsigned int v15; // eax@14
  int v19; // [sp+10h] [bp-8h]@3
//  unsigned int uOutXa; // [sp+20h] [bp+8h]@16
  int v23; // [sp+28h] [bp+10h]@3

  if ( this->uNumSceneBegins && a4 )
  {
    v4 = a4->uWidth;
    //v5 = &this->pTargetSurface[uOutX + uOutY * this->uTargetSurfacePitch];
    v6 = a4->pPixels;
    v23 = a4->uHeight;
    v19 = v4;
    if ( this->bClip )
    {
      if ( (signed int)uOutX < (signed int)this->uClipX )
      {
        v8 = this->uClipX - uOutX;
        v9 = uOutX - this->uClipX;
        v8 *= 2;
        v4 += v9;
        v6 = (unsigned __int16 *)((char *)v6 + v8);
        //v5 = (unsigned __int16 *)((char *)v5 + v8);
      }
      if ( (signed int)uOutY < (signed int)this->uClipY )
      {
        v11 = this->uClipY - uOutY;
        v6 += v19 * v11;
        v23 += uOutY - this->uClipY;
        //v5 += this->uTargetSurfacePitch * v11;
      }
      v12 = max(this->uClipX, uOutX);
      if ( (signed int)(v4 + v12) > (signed int)this->uClipZ )
      {
        v4 = this->uClipZ - max(this->uClipX, uOutX);
      }
      v15 = max(this->uClipY, uOutY);
      if ( (signed int)(v15 + v23) > (signed int)this->uClipW )
      {
        v23 = this->uClipW - max(this->uClipY, uOutY);
      }
    }

    for (int y = 0; y < v23; y++)
    {
      for (int x = 0; x < v4; x++)
      {
        WritePixel16(uOutX + x, uOutY + y, *v6);
        //*v5 = *v6;
        //++v5;
        ++v6;
      }
      v6 += v19 - v4;
      //v5 += this->uTargetSurfacePitch - v4;
    }
  }
}

//----- (004A5D33) --------------------------------------------------------
void Render::CreditsTextureScroll(unsigned int pX, unsigned int pY, int move_X, int move_Y, RGBTexture *pTexture)
{
  //unsigned __int16 *v7; // ebx@3
  int full_width; // ecx@3
  int full_height; // edi@3
  //int v23; // edi@23
  unsigned __int16 *pTexturea; // [sp+28h] [bp+18h]@3

  if ( this->uNumSceneBegins && pTexture )
  {
    /*auto v7 = this->pTargetSurface;
    if (FORCE_16_BITS)
      v7 = (unsigned __int32 *)((char *)v7 + (pX + pY * this->uTargetSurfacePitch) * 2);
    else
      v7 = (unsigned __int32 *)((char *)v7 + (pX + pY * this->uTargetSurfacePitch) * 4);*/
    full_width = pTexture->uWidth - move_X;
    full_height = pTexture->uHeight - move_Y;
    pTexturea = &pTexture->pPixels[move_X + move_Y * pTexture->uWidth];
    if ( this->bClip )
    {
      if ( pX < this->uClipX )//если кадр выходит за правую границу
      {
        pTexturea = (unsigned __int16 *)((char *)pTexturea + (2 * (this->uClipX - pX)));
        full_width += pX - this->uClipX;
        //v7 = (unsigned __int32 *)((char *)v7 + ((FORCE_16_BITS ? 2 : 4) * (this->uClipX - pX)));
      }
      if ( pY < this->uClipY )//если кадр выходит за верхнюю границу
      {
        pTexturea += pTexture->uWidth * (this->uClipY - pY);
        full_height += pY - this->uClipY;
        //v7 = (unsigned __int32 *)((char *)v7 + (FORCE_16_BITS ? 2 : 4) * this->uTargetSurfacePitch * (this->uClipY - pY));
      }
      if ( this->uClipX < pX )//если правая граница окна меньше х координаты кадра
        this->uClipX = pX;
      if ( this->uClipY < pY )//если верхняя граница окна меньше y координаты кадра
        this->uClipY = pY;
      if ( (full_width + this->uClipX) > this->uClipZ )//если ширина кадра выходит за правую границу
      {
        if ( this->uClipX < pX )
          this->uClipX = pX;
        full_width = this->uClipZ - this->uClipX;
      }
      if ( (full_height + this->uClipY) > this->uClipW )//если высота кадра выходит за нижнюю границу
      {
        if ( this->uClipY < pY )
          this->uClipY = pY;
        full_height = this->uClipW - this->uClipY;
      }
    }

    for (int y = 0; y < full_height; ++y)
    {
      for (int x = 0; x < full_width; ++x)
      {
            if ( *pTexturea != Color16(0, 0xFFu, 0xFFu) )
            {
              WritePixel16(pX + x, pY + y, *pTexturea);
              /*if (FORCE_16_BITS)
                *(unsigned __int16 *)v7 = *pTexturea;
              else
                *(unsigned __int32 *)v7 = r5g6b5_2_r8g8b8(*pTexturea);*/
            }
            ++pTexturea;
            //++v7;
        }
        //v7 += this->uTargetSurfacePitch - full_width;
        pTexturea = (unsigned __int16 *)((char *)pTexturea + 2 * (pTexture->uWidth - full_width));
      }
  }
}

//----- (004A6E7E) --------------------------------------------------------
void Render::DrawTranslucent(unsigned int a2, unsigned int a3, Texture *a4)
{
  //unsigned __int16 *v4; // eax@4
  int v5; // edx@4
  unsigned int v6; // edi@4
  unsigned int v7; // edx@5
  unsigned int v8; // edx@6
  unsigned int v9; // edx@7
  unsigned int v10; // edx@8
  unsigned int v11; // ebx@9
  unsigned int v12; // esi@11
  unsigned int v13; // edx@12
  unsigned int v14; // ebx@15
  unsigned int v15; // esi@17
  unsigned int v16; // edi@18
//  char v17; // zf@29
  int v18; // [sp+14h] [bp-Ch]@4
  int v19; // [sp+18h] [bp-8h]@4
  unsigned __int8 *v20; // [sp+1Ch] [bp-4h]@4
//  int v21; // [sp+28h] [bp+8h]@25
//  int v22; // [sp+28h] [bp+8h]@34
//  unsigned int v23; // [sp+2Ch] [bp+Ch]@23
//  unsigned int v24; // [sp+2Ch] [bp+Ch]@32

  if ( this->uNumSceneBegins && a4 && a4->pPalette16 )
  {
    //v4 = &this->pTargetSurface[a2 + a3 * this->uTargetSurfacePitch];
    v20 = a4->pLevelOfDetail0_prolly_alpha_mask;
    v5 = a4->uTextureWidth;
    v6 = a4->uTextureHeight;
    v19 = a4->uTextureWidth;
    v18 = a4->uTextureWidth;
    int clipped_out_x = a2;
    int clipped_out_y = a3;
    if ( this->bClip )
    {
      v7 = this->uClipX;
      if ( (signed int)a2 < (signed int)v7 )
      {
        v8 = v7 - a2;
        v20 += v8;
        v19 += a2 - this->uClipX;
        //v4 += v8;
        clipped_out_x = uClipX;
      }
      v9 = this->uClipY;
      if ( (signed int)a3 < (signed int)v9 )
      {
        v10 = v9 - a3;
        v20 += v18 * v10;
        v6 = a3 - this->uClipY + a4->uTextureHeight;
        //v4 += this->uTargetSurfacePitch * v10;
        clipped_out_y = uClipY;
      }
      v11 = this->uClipX;
      v5 = v19;
      if ( (signed int)v11 < (signed int)a2 )
        v11 = a2;
      v12 = this->uClipZ;
      if ( (signed int)(v19 + v11) > (signed int)v12 )
      {
        v13 = this->uClipX;
        if ( (signed int)v13 < (signed int)a2 )
          v13 = a2;
        v5 = v12 - v13;
      }
      v14 = this->uClipY;
      if ( (signed int)v14 < (signed int)a3 )
        v14 = a3;
      v15 = this->uClipW;
      if ( (signed int)(v6 + v14) > (signed int)v15 )
      {
        v16 = this->uClipY;
        if ( (signed int)v16 < (signed int)a3 )
          v16 = a3;
        v6 = v15 - v16;
      }
    }

    for (int y = 0; y < v6; ++y)
    {
      for (int x = 0; x < v5; ++x)
      {
        if ( *v20 )
          WritePixel16(clipped_out_x + x, clipped_out_y + y, ((unsigned int)a4->pPalette16[*v20] >> 1) & 0x7BEF);
        ++v20;
      }
          v20 += v18 - v5;
    }

    /*if ( pRenderer->uTargetGBits == 5 )
    {
      if ( (signed int)v6 > 0 )
      {
        v23 = v6;
        do
        {
          if ( v5 > 0 )
          {
            v21 = v5;
            do
            {
              if ( *v20 )
                *v4 = ((unsigned int)a4->pPalette16[*v20] >> 1) & 0x3DEF;
              ++v4;
              ++v20;
              --v21;
            }
            while ( v21 );
          }
          v20 += v18 - v5;
          v17 = v23-- == 1;
          v4 += this->uTargetSurfacePitch - v5;
        }
        while ( !v17 );
      }
    }
    else
    {
      if ( (signed int)v6 > 0 )
      {
        v24 = v6;
        do
        {
          if ( v5 > 0 )
          {
            v22 = v5;
            do
            {
              if ( *v20 )
                *v4 = ((unsigned int)a4->pPalette16[*v20] >> 1) & 0x7BEF;
              ++v4;
              ++v20;
              --v22;
            }
            while ( v22 );
          }
          v20 += v18 - v5;
          v17 = v24-- == 1;
          v4 += this->uTargetSurfacePitch - v5;
        }
        while ( !v17 );
      }
    }*/
  }
}

//----- (004A6DF5) --------------------------------------------------------
void Render::_4A6DF5(unsigned __int16 *pBitmap, unsigned int uBitmapPitch, Vec2_int_ *pBitmapXY, void *pTarget, unsigned int uTargetPitch, Vec4_int_ *a7)
{
  int width; // ecx@3
  unsigned __int16 *pixels; // ebx@4
  int height; // esi@4

  if ( !pBitmap || !pTarget)
    return;

  width = a7->z - a7->x;
  height = a7->w - a7->y;
  pixels = (unsigned short *)pTarget + a7->x + uTargetPitch * a7->y;
  for ( int y = 0; y < height; ++y )
  {
    for ( int x = 0; x < width; ++x )
    {
      WritePixel16(a7->x + x, a7->y + y, *pixels);
      ++pixels;
    }
    pixels += uTargetPitch - width;
  }
}

//----- (004A6D87) --------------------------------------------------------
void Render::FillRectFast(unsigned int uX, unsigned int uY, unsigned int uWidth, unsigned int uHeight, unsigned int uColor16)
{
  if (!uNumSceneBegins)
    return;

  unsigned __int32 twoColors = (uColor16 << 16) | uColor16;
  for (uint y = 0; y < uHeight; ++y)
  {
    void *pDst = (char *)pTargetSurface + (FORCE_16_BITS ? 2 : 4) * (uX + (y + uY) * uTargetSurfacePitch);

    memset32(pDst,
             FORCE_16_BITS ? twoColors : 0xFF000000 | Color32(uColor16),  // two colors per int (16bit) or 1 (32bit)
             uWidth / (FORCE_16_BITS ? 2 : 1));                      // two pixels per int (16bit) or 1 (32bit)

    if (FORCE_16_BITS && uWidth & 1) // we may miss one pixel for 16bit
      ((unsigned __int16 *)pTargetSurface)[uX + uWidth - 1 + (y + uY) * uTargetSurfacePitch] = uColor16;
    }
}

//----- (004A6C4F) --------------------------------------------------------
void Render::DrawText(signed int uOutX, signed int uOutY, unsigned __int8 *pFontPixels, unsigned int uCharWidth, 
                       unsigned int uCharHeight, unsigned __int16 *pFontPalette, 
                       unsigned __int16 uFaceColor, unsigned __int16 uShadowColor)
{
  unsigned int v9; // edi@2
  unsigned int v10; // esi@2
  //unsigned __int16 *v11; // eax@2
  unsigned int v12; // ebx@3
  signed int v13; // edx@5
  int v14; // edx@6
  signed int v15; // ebx@7
  unsigned int v16; // edx@9
  signed int v17; // edi@10
  signed int v18; // ebx@13
  unsigned int v19; // edx@15
  signed int v20; // esi@16
  //unsigned int v21; // esi@22
  unsigned __int16 v22; // dx@24
  //char v23; // zf@28
  unsigned __int8 *v24; // [sp+Ch] [bp-4h]@2
  //unsigned int uOutXa; // [sp+18h] [bp+8h]@20

  if (!this->uNumSceneBegins)
    return;

    v9 = uCharWidth;
    v10 = uCharHeight;
    //v11 = &this->pTargetSurface[uOutX + uOutY * this->uTargetSurfacePitch];
    v24 = pFontPixels;

    int clipped_out_x = uOutX, clipped_out_y = uOutY;
    if ( this->bClip )
    {
      v12 = this->uClipX;
      if ( uOutX < (signed int)v12 )
      {
        v24 = &pFontPixels[v12 - uOutX];
        //v11 += v12 - uOutX;
        clipped_out_x = uClipX;
        v9 = uCharWidth + uOutX - v12;
      }
      v13 = this->uClipY;
      if ( uOutY < v13 )
      {
        v14 = v13 - uOutY;
        v24 += uCharWidth * v14;
        v10 = uCharHeight + uOutY - this->uClipY;
        //v11 += this->uTargetSurfacePitch * v14;
        clipped_out_y = uClipY;
      }
      v15 = this->uClipX;
      if ( v15 < uOutX )
        v15 = uOutX;
      v16 = this->uClipZ;
      if ( (signed int)(v9 + v15) > (signed int)v16 )
      {
        v17 = this->uClipX;
        if ( v17 < uOutX )
          v17 = uOutX;
        v9 = v16 - v17;
      }
      v18 = this->uClipY;
      if ( v18 < uOutY )
        v18 = uOutY;
      v19 = this->uClipW;
      if ( (signed int)(v10 + v18) > (signed int)v19 )
      {
        v20 = this->uClipY;
        if ( v20 < uOutY )
          v20 = uOutY;
        v10 = v19 - v20;
      }
    }

    for (int y = 0; y < v10; ++y)
    {
      for (int x = 0; x < v9; ++x)
      {
        if (*v24)
        {
          v22 = uShadowColor;
          if ( *v24 != 1 )
            v22 = uFaceColor;
           WritePixel16(clipped_out_x + x, clipped_out_y + y, v22);
        }
        ++v24;
      } 
      v24 += uCharWidth - v9;
        //v23 = uOutXa-- == 1;
        //v11 += this->uTargetSurfacePitch - v9;
    }
}

//----- (004A6A68) --------------------------------------------------------
void Render::GetLeather(unsigned int a2, unsigned int a3, Texture *a4, __int16 height)
{
  Texture tex; // [sp+Ch] [bp-48h]@1

  memcpy(&tex, a4, sizeof(tex));
  tex.uTextureHeight = a4->uTextureHeight - height;
  if ( (signed __int16)tex.uTextureHeight > 0 )
    DrawTextureIndexed(a2, a3, &tex);
}

//----- (004A6AB1) --------------------------------------------------------
void Render::DrawTextPalette( int x, int y, unsigned char* font_pixels, int a5, unsigned int uFontHeight, unsigned __int16 *pPalette, int a8 )
    {
  int v8; // edi@2
  unsigned int v9; // esi@2
  //unsigned __int16 *v10; // eax@2
  unsigned char *v11; // edx@2
  unsigned int v12; // ebx@3
  signed int v13; // edx@5
  int v14; // edx@6
  signed int v15; // ebx@7
  unsigned int v16; // edx@9
  signed int v17; // edi@10
  signed int v18; // ebx@13
  unsigned int v19; // edx@15
  signed int v20; // esi@16
//  int v21; // ebx@22
//  char v22; // zf@28
//  int v23; // ebx@31
  unsigned __int16 v24; // si@35
  int v25; // [sp+Ch] [bp-4h]@2
//  int v26; // [sp+1Ch] [bp+Ch]@24
//  int v27; // [sp+1Ch] [bp+Ch]@33
  unsigned int v28; // [sp+20h] [bp+10h]@30
//  unsigned int v29; // [sp+24h] [bp+14h]@22
//  unsigned int v30; // [sp+24h] [bp+14h]@31

  int a2 = x;
  int a3 = y;
  uint a6 = uFontHeight;
  if (!this->uNumSceneBegins)
    return;

    v8 = a5;
    v9 = a6;
    //v10 = &pTargetSurface[x + y * uTargetSurfacePitch];
    v11 = (unsigned char *)font_pixels;
    v25 = (int)font_pixels;
    int clipped_out_x = x;
    int clipped_out_y = y;
    if ( this->bClip )
    {
      v12 = this->uClipX;
      if ( a2 < (signed int)v12 )
      {
        v25 = v12 - a2 + (int)font_pixels;
        //v10 += v12 - a2;
        v8 = a5 + a2 - v12;
        clipped_out_x = uClipX;
      }
      v13 = this->uClipY;
      if ( a3 < v13 )
      {
        v14 = v13 - a3;
        v25 += a5 * v14;
        v9 = a6 + a3 - this->uClipY;
        //v10 += this->uTargetSurfacePitch * v14;
        clipped_out_y = uClipY;
      }
      v15 = this->uClipX;
      if ( v15 < a2 )
        v15 = a2;
      v16 = this->uClipZ;
      if ( v8 + v15 > (signed int)v16 )
      {
        v17 = this->uClipX;
        if ( v17 < a2 )
          v17 = a2;
        v8 = v16 - v17;
      }
      v18 = this->uClipY;
      if ( v18 < a3 )
        v18 = a3;
      v19 = this->uClipW;
      if ( (signed int)(v9 + v18) > (signed int)v19 )
      {
        v20 = this->uClipY;
        if ( v20 < a3 )
          v20 = a3;
        v9 = v19 - v20;
      }
      v11 = (unsigned char *)v25;
    }

    if ( a8 )
    {
      v28 = 0x7FF; // 16bit pRenderer->uTargetGMask | pRenderer->uTargetBMask;
      for (int dy = 0; dy < v9; ++dy)
      {
        for (int dx = 0; dx < v8; ++dx)
        {
              if ( *v11 )
                v24 = pPalette[*v11];
              else
                v24 = v28;
              WritePixel16(clipped_out_x + dx, clipped_out_y + dy, v24);
              //*v10 = v24;
              //++v10;
              ++v11;
              //--v27;

        }
          v11 += a5 - v8;
      }
      /*if ( (signed int)v9 > 0 )
      {
        v23 = a5;
        v30 = v9;
        do
        {
          if ( v8 > 0 )
          {
            v27 = v8;
            do
            {
              if ( *v11 )
                v24 = pPalette[*v11];
              else
                v24 = v28;
              *v10 = v24;
              ++v10;
              ++v11;
              --v27;
            }
            while ( v27 );
          }
          v11 += v23 - v8;
          v22 = v30-- == 1;
          v10 += this->uTargetSurfacePitch - v8;
        }
        while ( !v22 );
      }*/
    }
    else
    {
      for (int dy = 0; dy < v9; ++dy)
      {
        for (int dx = 0; dx < v8; ++dx)
        {
            if ( *v11 )       
              WritePixel16(clipped_out_x + dx, clipped_out_y + dy, pPalette[*v11]);
              //*v10 = v24;
              //++v10;
              ++v11;
              //--v27;
        }
          v11 += a5 - v8;
      }

      /*if ( (signed int)v9 > 0 )
      {
        v21 = a5;
        v29 = v9;
        do
        {
          if ( v8 > 0 )
          {
            v26 = v8;
            do
            {
              if ( *v11 )
                *v10 = pPalette[*v11];
              ++v10;
              ++v11;
              --v26;
            }
            while ( v26 );
          }
          v11 += v21 - v8;
          v22 = v29-- == 1;
          v10 += this->uTargetSurfacePitch - v8;
        }
        while ( !v22 );
      }*/
    }
}

//----- (004A68EF) --------------------------------------------------------
void Render::DrawTransparentGreenShade(signed int a2, signed int a3, Texture *pTexture)
{
  DrawMasked(a2, a3, pTexture, 0x07E0);
}


//----- (004A6776) --------------------------------------------------------
void Render::DrawTransparentRedShade(unsigned int a2, unsigned int a3, Texture *a4)
{
  DrawMasked(a2, a3, a4, 0xF800);
  /*Texture *v4; // edi@2
  unsigned int v5; // ebx@4
  unsigned __int16 *v6; // eax@4
  unsigned int v7; // edx@5
  unsigned int v8; // edx@6
  unsigned int v9; // edx@7
  unsigned int v10; // edx@8
  unsigned int v11; // edx@9
  unsigned int v12; // esi@12
  unsigned int v13; // esi@15
  unsigned int v14; // edx@17
  unsigned int v15; // esi@18
  unsigned __int8 *v16; // ebx@22
  char v17; // zf@28
  int v18; // [sp+10h] [bp-10h]@4
  unsigned __int8 *v19; // [sp+18h] [bp-8h]@4
  int v20; // [sp+1Ch] [bp-4h]@4
  int a2a; // [sp+28h] [bp+8h]@24
  unsigned int a3a; // [sp+2Ch] [bp+Ch]@22
  unsigned int a4a; // [sp+30h] [bp+10h]@11

  if ( this->uNumSceneBegins )
  {
    v4 = a4;
    if ( a4 )
    {
      if ( a4->pPalette16 )
      {
        v5 = a4->uTextureHeight;
        v6 = &this->pTargetSurface[a2 + a3 * this->uTargetSurfacePitch];
        v19 = a4->pLevelOfDetail0_prolly_alpha_mask;
        v20 = a4->uTextureWidth;
        v18 = a4->uTextureWidth;
        if ( this->bClip )
        {
          v7 = this->uClipX;
          if ( (signed int)a2 < (signed int)v7 )
          {
            v8 = v7 - a2;
            v19 += v8;
            v20 += a2 - this->uClipX;
            v6 += v8;
          }
          v9 = this->uClipY;
          v5 = a4->uTextureHeight;
          if ( (signed int)a3 < (signed int)v9 )
          {
            v10 = v9 - a3;
            v19 += v18 * v10;
            v5 = a3 - this->uClipY + a4->uTextureHeight;
            v4 = a4;
            v6 += this->uTargetSurfacePitch * v10;
          }
          v11 = this->uClipX;
          if ( (signed int)v11 < (signed int)a2 )
            v11 = a2;
          a4a = this->uClipZ;
          if ( (signed int)(v11 + v20) > (signed int)a4a )
          {
            v12 = this->uClipX;
            if ( (signed int)v12 < (signed int)a2 )
              v12 = a2;
            v20 = a4a - v12;
          }
          v13 = this->uClipY;
          if ( (signed int)v13 < (signed int)a3 )
            v13 = a3;
          v14 = this->uClipW;
          if ( (signed int)(v5 + v13) > (signed int)v14 )
          {
            v15 = this->uClipY;
            if ( (signed int)v15 < (signed int)a3 )
              v15 = a3;
            v5 = v14 - v15;
          }
        }
        if ( (signed int)v5 > 0 )
        {
          a3a = v5;
          v16 = v19;
          do
          {
            if ( v20 > 0 )
            {
              a2a = v20;
              do
              {
                if ( *v16 )
                  *v6 = this->uTargetRMask & v4->pPalette16[*v16];
                ++v6;
                ++v16;
                --a2a;
              }
              while ( a2a );
            }
            v16 += v18 - v20;
            v17 = a3a-- == 1;
            v6 += this->uTargetSurfacePitch - v20;
          }
          while ( !v17 );
        }
      }
    }
  }*/
}

//----- (004A68EF) --------------------------------------------------------
void Render::DrawMasked(signed int a2, signed int a3, Texture *pTexture, unsigned __int16 mask)
{
  unsigned int v5; // ebx@4
  int v10; // edx@8
  signed int v11; // edx@9
  signed int v12; // esi@12
  signed int v13; // esi@15
  signed int v15; // esi@18
  unsigned __int8 *v16; // ebx@22
//  char v17; // zf@28
  int v18; // [sp+10h] [bp-10h]@4
  unsigned __int8 *v19; // [sp+18h] [bp-8h]@4
  int v20; // [sp+1Ch] [bp-4h]@4
//  int v21; // [sp+28h] [bp+8h]@24
//  unsigned int v22; // [sp+2Ch] [bp+Ch]@22

  if (!uNumSceneBegins || !pTexture)
    return;

  if ( pTexture->pPalette16 )
  {
    v5 = pTexture->uTextureHeight;
    //v6 = &this->pTargetSurface[a2 + a3 * this->uTargetSurfacePitch];
    v19 = pTexture->pLevelOfDetail0_prolly_alpha_mask;
    v20 = pTexture->uTextureWidth;
    v18 = pTexture->uTextureWidth;
    int clipped_out_x = a2;
    int clipped_out_y = a3;
    if ( this->bClip )
    {
      if ( a2 < this->uClipX )
      {
        v19 += this->uClipX - a2;
        v20 += a2 - this->uClipX;
        clipped_out_x = uClipX;
      }
      v5 = pTexture->uTextureHeight;
      if ( a3 < this->uClipY )
      {
        v10 = this->uClipY - a3;
        v19 += v18 * v10;
        v5 = a3 - this->uClipY + pTexture->uTextureHeight;
        clipped_out_y = uClipY;
      }
      v11 = this->uClipX;
      if ( this->uClipX < a2 )
        v11 = a2;
      if ( v11 + v20 > (signed int)this->uClipZ )
      {
        v12 = this->uClipX;
        if ( v12 < a2 )
          v12 = a2;
            v20 = this->uClipZ - v12;
          }
          v13 = this->uClipY;
          if ( this->uClipY < a3 )
            v13 = a3;
          if ( (signed int)(v5 + v13) > (signed int)this->uClipW )
          {
            v15 = this->uClipY;
            if ( this->uClipY < a3 )
              v15 = a3;
            v5 = this->uClipW - v15;
          }
        }
        
          v16 = v19;
        for (int y = 0; y < v5; ++y)
        {
          for (int x = 0; x < v20; ++x)
          {
                if ( *v16 )
                  WritePixel16(clipped_out_x + x, clipped_out_y + y, pTexture->pPalette16[*v16] & mask);
                ++v16;
          }
            v16 += v18 - v20;
        }

        /*if ( (signed int)v5 > 0 )
        {
          v22 = v5;
          v16 = v19;
          do
          {
            if ( v20 > 0 )
            {
              v21 = v20;
              do
              {
                if ( *v16 )
                  *v6 = this->uTargetGMask & v4->pPalette16[*v16];
                ++v6;
                ++v16;
                --v21;
              }
              while ( v21 );
            }
            v16 += v18 - v20;
            v17 = v22-- == 1;
            v6 += this->uTargetSurfacePitch - v20;
          }
          while ( !v17 );
        }*/
      }
}


//----- (004A65CC) --------------------------------------------------------
void Render::_4A65CC(unsigned int x, unsigned int y, Texture *a4, Texture *a5, int a6, int a7, int a8)
{
  unsigned int uHeight; // edi@6
  unsigned int v14; // edx@11
  unsigned int v16; // edx@14
  unsigned int v17; // edx@17
  unsigned int v19; // edx@20
  int v20; // eax@27
  int v21; // edx@29
//  int v23; // [sp+Ch] [bp-Ch]@24
  unsigned __int8 *v24; // [sp+14h] [bp-4h]@6
//  int xa; // [sp+20h] [bp+8h]@26
//  unsigned int ya; // [sp+24h] [bp+Ch]@24
  int Width; // [sp+2Ch] [bp+14h]@6

  if ( this->uNumSceneBegins && a4 && a4->pPalette16 && a5 && a5->pPalette16 )
  {
    v24 = a4->pLevelOfDetail0_prolly_alpha_mask;
    Width = a4->uTextureWidth;
    uHeight = a4->uTextureHeight;
    int clipped_out_x = x;
    int clipped_out_y = y;
    if ( this->bClip )
    {
      if ( (signed int)x < (signed int)this->uClipX )
      {
        v24 += this->uClipX - x;
        Width += x - this->uClipX;
        clipped_out_x = uClipX;
      }
      if ( (signed int)y < (signed int)this->uClipY )
      {
        v24 += a4->uTextureWidth * (this->uClipY - y);
        uHeight = y - this->uClipY + a4->uTextureHeight;
        clipped_out_y = uClipY;
      }
      v14 = this->uClipX;
      if ( (signed int)this->uClipX < (signed int)x )
        v14 = x;
      if ( (signed int)(Width + v14) > (signed int)this->uClipZ )
      {
        v16 = this->uClipX;
        if ( (signed int)this->uClipX < (signed int)x )
          v16 = x;
        Width = this->uClipZ - v16;
      }
      v17 = this->uClipY;
      if ( (signed int)this->uClipY < (signed int)y )
        v17 = y;
      if ( (signed int)(uHeight + v17) > (signed int)this->uClipW )
      {
        v19 = this->uClipY;
        if ( (signed int)v19 < (signed int)y )
          v19 = y;
        uHeight = this->uClipW - v19;
      }
    }

    for (int dy = 0; dy < uHeight; ++dy)
    {
      for (int dx = 0; dx < Width; ++dx)
      {
        v20 = *v24;
        if ( v20 >= a7 && v20 <= a8 )
        {
          v21 = a7 + (a6 + v20) % (2 * (a8 - a7));
          if ( (a6 + v20) % (2 * (a8 - a7)) >= a8 - a7 )
            v21 = 2 * a8 - v21 - a7;
          WritePixel16(clipped_out_x + dx, clipped_out_y + dy, a4->pPalette16[v21]);
        }
        ++v24;
      }
      v24 += a4->uTextureWidth - Width;
    }
    /*if ( (signed int)v9 > 0 )
    {
      ya = v9;
      v23 = v22 - v27;
      do
      {
        if ( v27 > 0 )
        {
          xa = v27;
          do
          {
            v20 = *v24;
            if ( v20 >= a7 && v20 <= a8 )
            {
              v21 = a7 + (a6 + v20) % (2 * (a8 - a7));
              if ( (a6 + v20) % (2 * (a8 - a7)) >= a8 - a7 )
                v21 = 2 * a8 - v21 - a7;
              *v8 = a4->pPalette16[v21];
            }
            ++v8;
            ++v24;
            --xa;
          }
          while ( xa );
        }
        v8 += this->uTargetSurfacePitch - v27;
        v24 += v23;
        --ya;
      }
      while ( ya );
    }*/
  }
}

//----- (004A63E6) --------------------------------------------------------
void Render::DrawAura(unsigned int a2, unsigned int a3, Texture *a4, Texture *a5, int a6, int a7, int a8)
{
  unsigned int v14; // edx@11
  unsigned int v16; // edx@14
  unsigned int v17; // edx@17
  unsigned int v19; // edx@20
  int v20; // eax@27
  int v21; // edx@29
//  int v23; // [sp+Ch] [bp-Ch]@24
  int Height; // [sp+10h] [bp-8h]@6
  int Width; // [sp+14h] [bp-4h]@6
  int v27; // [sp+24h] [bp+Ch]@23
  unsigned __int8 *v28; // [sp+28h] [bp+10h]@6

  if ( this->uNumSceneBegins )
  {
    if ( a4 )
    {
      if ( a4->pPalette16 )
      {
        if ( a5 )
        {
          if ( a5->pPalette16 )
          {
            v28 = a4->pLevelOfDetail0_prolly_alpha_mask;
            Width = a4->uTextureWidth;
            Height = a4->uTextureHeight;
            int clipped_out_x = a2;
            int clipped_out_y = a3;
            if ( this->bClip )
            {
              if ( (signed int)a2 < (signed int)this->uClipX )
              {
                v28 += this->uClipX - a2;
                Width += a2 - this->uClipX;
                clipped_out_x = uClipX;
              }

              if ( (signed int)a3 < (signed int)this->uClipY )
              {
                v28 += a4->uTextureWidth * (this->uClipY - a3);
                Height += a3 - this->uClipY;
                clipped_out_y = uClipY;
              }

              v14 = this->uClipX;
              if ( (signed int)this->uClipX < (signed int)a2 )
                v14 = a2;
              if ( (signed int)(Width + v14) > (signed int)this->uClipZ )
              {
                v16 = this->uClipX;
                if ( (signed int)this->uClipX < (signed int)a2 )
                  v16 = a2;
                Width = this->uClipZ - v16;
              }

              v17 = this->uClipY;
              if ( (signed int)this->uClipY < (signed int)a3 )
                v17 = a3;
              if ( (signed int)(Height + v17) > (signed int)this->uClipW )
              {
                v19 = this->uClipY;
                if ( (signed int)this->uClipY < (signed int)a3 )
                  v19 = a3;
                Height = this->uClipW - v19;
              }
            }

            v27 = 0;
            for (int y = 0; y < Height; ++y)
            {
              for (int x = 0; x < Width; ++x)
              {
                  if ( *v28 )
                  {
                    v20 = *(&a5->pLevelOfDetail0_prolly_alpha_mask[x & a5->uWidthMinus1] + a5->uTextureWidth * (v27 & a5->uHeightMinus1));
                    if ( v20 >= a7 )
                    {
                      if ( v20 <= a8 )
                      {
                        v21 = a7 + (a6 + v20) % (2 * (a8 - a7));
                        if ( (a6 + v20) % (2 * (a8 - a7)) >= a8 - a7 )
                          v21 = 2 * a8 - v21 - a7;
                        //v9 = a5;
                        //*v10 = a5->pPalette16[v21];
                        WritePixel16(clipped_out_x + x, clipped_out_y + y, a5->pPalette16[v21]);
                      }
                    }
                  }
                  v28++;
              }
                v28 += a4->uTextureWidth - Width;
            }

            /*if ( v24 > 0 )
            {
              v23 = v22 - v25;
              do
              {
                for ( i = 0; i < v25; ++v28 )
                {
                  if ( *v28 )
                  {
                    v20 = *(&v9->pLevelOfDetail0_prolly_alpha_mask[i & v9->uWidthMinus1] + v9->uTextureWidth * (v27 & v9->uHeightMinus1));
                    if ( v20 >= a7 )
                    {
                      if ( v20 <= a8 )
                      {
                        v21 = a7 + (a6 + v20) % (2 * (a8 - a7));
                        if ( (a6 + v20) % (2 * (a8 - a7)) >= a8 - a7 )
                          v21 = 2 * a8 - v21 - a7;
                        v9 = a5;
                        *v10 = a5->pPalette16[v21];
                      }
                    }
                  }
                  ++i;
                  ++v10;
                }
                ++v27;
                v10 += this->uTargetSurfacePitch - v25;
                v28 += v23;
              }
              while ( v27 < v24 );
            }*/

          }
        }
      }
    }
  }
}

//----- (004A6274) --------------------------------------------------------
void Render::DrawTextureTransparent(unsigned int uX, unsigned int uY, Texture *pTexture)
{
  int uHeight; // ebx@4
  unsigned int v11; // edx@9
  unsigned int v12; // esi@12
  unsigned int v13; // esi@15
  unsigned int v15; // esi@18
  unsigned __int8 *v19; // [sp+18h] [bp-8h]@4
  int uWidth; // [sp+1Ch] [bp-4h]@4

  if ( this->uNumSceneBegins )
  {
    if ( pTexture )
    {
      if ( pTexture->pPalette16 )
      {
        uHeight = pTexture->uTextureHeight;
        v19 = pTexture->pLevelOfDetail0_prolly_alpha_mask;
        uWidth = pTexture->uTextureWidth;

        int clipped_out_x = uX;
        int clipped_out_y = uY;
        if ( this->bClip )
        {
          if ( (signed int)uX < (signed int)this->uClipX )
          {
            v19 += this->uClipX - uX;
            uWidth += uX - this->uClipX;
            clipped_out_x = uClipX;
          }

          uHeight = pTexture->uTextureHeight;
          if ( (signed int)uY < (signed int)this->uClipY )
          {
            v19 += pTexture->uTextureWidth * (this->uClipY - uY);
            uHeight = uY - this->uClipY + pTexture->uTextureHeight;
            clipped_out_y = uClipY;
          }
          v11 = this->uClipX;
          if ( (signed int)this->uClipX < (signed int)uX )
            v11 = uX;

          if ( (signed int)(v11 + uWidth) > (signed int)this->uClipZ )
          {
            v12 = this->uClipX;
            if ( (signed int)this->uClipX < (signed int)uX )
              v12 = uX;
            uWidth = this->uClipZ - v12;
          }
          v13 = this->uClipY;
          if ( (signed int)this->uClipY < (signed int)uY )
            v13 = uY;

          if ( (signed int)(uHeight + v13) > (signed int)this->uClipW )
          {
            v15 = this->uClipY;
            if ( (signed int)this->uClipY < (signed int)uY )
              v15 = uY;
            uHeight = this->uClipW - v15;
          }
        }

        for (int y = 0; y < uHeight; ++y)
        {
          for (int x = 0; x < uWidth; ++x)
          {
            if ( *v19 )
              WritePixel16(clipped_out_x + x, clipped_out_y + y, pTexture->pPalette16[*v19]);
            ++v19;
          }
          v19 += pTexture->uTextureWidth - uWidth;
        }
        /*if ( (signed int)uHeight > 0 )
        {
          uYa = uHeight;
          v16 = v19;
          do
          {
            if ( uWidth > 0 )
            {
              uXa = uWidth;
              do
              {
                if ( *v16 )
                  *v6 = pCurrentTexture->pPalette16[*v16];
                ++v6;
                ++v16;
              }
              while ( uXa-- !=1 );
            }
            v16 += v18 - uWidth;
            uFlag = uYa-- == 1;
            v6 += this->uTargetSurfacePitch - uWidth;
          }
          while ( !uFlag );
        }*/
      }
    }
  }
}

//----- (004A612A) --------------------------------------------------------
void Render::DrawMaskToZBuffer(signed int uOutX, unsigned int uOutY, Texture *pTexture, int zVal)
{
  Texture *v5; // eax@2
  unsigned int v6; // edx@3
  int v7; // ebx@3
  int v8; // edi@3
  signed int v9; // eax@4
  int v10; // eax@5
  unsigned int v11; // esi@6
  signed int v12; // esi@8
  unsigned int v13; // eax@10
  signed int v14; // esi@11
  unsigned int v15; // esi@14
  unsigned int v16; // eax@16
  unsigned int v17; // ecx@17
  int v18; // edx@23
  int v19; // [sp+Ch] [bp-Ch]@3
  int v20; // [sp+10h] [bp-8h]@3
  int uOutXa; // [sp+20h] [bp+8h]@21
  unsigned __int8 *uOutYa; // [sp+24h] [bp+Ch]@3
  int *pZBuffer; // [sp+28h] [bp+10h]@3

  if ( this->uNumSceneBegins )
  {
    v5 = pTexture;
    if ( pTexture )
    {
      v6 = uOutY;
      v7 = pTexture->uTextureHeight;
      pZBuffer = &this->pActiveZBuffer[uOutX + window->GetWidth() * uOutY];
      uOutYa = v5->pLevelOfDetail0_prolly_alpha_mask;
      v8 = v5->uTextureWidth;
      v20 = v5->uTextureWidth;
      v19 = v5->uTextureWidth;
      if ( this->bClip )
      {
        v9 = this->uClipX;
        if ( uOutX < v9 )
        {
          v10 = v9 - uOutX;
          uOutYa += v10;
          v8 += uOutX - this->uClipX;
          v20 = v8;
          pZBuffer += v10;
        }
        v11 = this->uClipY;
        if ( (signed int)v6 < (signed int)v11 )
        {
          uOutYa += v19 * (v11 - v6);
          v7 += v6 - v11;
          pZBuffer += window->GetWidth() * (v11 - v6);
          v8 = v20;
        }
        v12 = this->uClipX;
        if ( v12 < uOutX )
          v12 = uOutX;
        v13 = this->uClipZ;
        if ( v8 + v12 > (signed int)v13 )
        {
          v14 = this->uClipX;
          if ( v14 < uOutX )
            v14 = uOutX;
          v8 = v13 - v14;
        }
        v15 = this->uClipY;
        if ( (signed int)v15 < (signed int)v6 )
          v15 = v6;
        v16 = this->uClipW;
        if ( (signed int)(v7 + v15) > (signed int)v16 )
        {
          v17 = this->uClipY;
          if ( (signed int)v17 >= (signed int)v6 )
            v6 = v17;
          v7 = v16 - v6;
        }
      }
      if ( v7 > 0 )
      {
        uOutXa = v7;
        do
        {
          if ( v8 > 0 )
          {
            v18 = v8;
            do
            {
              if ( *uOutYa )
                *pZBuffer = zVal;
              ++pZBuffer;
              ++uOutYa;
              --v18;
            }
            while ( v18 );
          }
          pZBuffer += window->GetWidth() - v8;
          uOutYa += v19 - v8;
          --uOutXa;
        }
        while ( uOutXa );
      }
    }
  }
}

//----- (004A601E) --------------------------------------------------------
void Render::ZBuffer_Fill_2(signed int a2, signed int a3, Texture *pTexture, int a5)
{
  signed int v5; // edx@3
  int v6; // ebx@3
  int v7; // esi@3
  void *v8; // esi@3
  unsigned int v9; // eax@4
  unsigned int v10; // eax@6
  signed int v11; // edi@8
  unsigned int v12; // eax@10
  signed int v13; // edi@11
  unsigned int v14; // edi@14
  unsigned int v15; // eax@16
  unsigned int v16; // ecx@17
  int v17; // [sp+18h] [bp+Ch]@3
  unsigned int pTexturea; // [sp+1Ch] [bp+10h]@3

  if ( this->uNumSceneBegins && pTexture )
  {
    v5 = a3;
    v6 = pTexture->uTextureHeight;
    v7 = 5 * a3;
    v17 = pTexture->uTextureHeight;
    v8 = &this->pActiveZBuffer[a2 + (v7 << 7)];
    pTexturea = pTexture->uTextureWidth;
    if ( this->bClip )
    {
      v9 = this->uClipX;
      if ( a2 < (signed int)v9 )
      {
        pTexturea += a2 - v9;
        v8 = (char *)v8 + 4 * (v9 - a2);
      }
      v10 = this->uClipY;
      if ( v5 < (signed int)v10 )
      {
        v17 += v5 - v10;
        v8 = (char *)v8 + 2560 * (v10 - v5);
      }
      v11 = this->uClipX;
      if ( v11 < a2 )
        v11 = a2;
      v12 = this->uClipZ;
      if ( (signed int)(pTexturea + v11) > (signed int)v12 )
      {
        v13 = this->uClipX;
        if ( v13 < a2 )
          v13 = a2;
        pTexturea = v12 - v13;
      }
      v14 = this->uClipY;
      if ( (signed int)v14 < v5 )
        v14 = v5;
      v6 = v17;
      v15 = this->uClipW;
      if ( (signed int)(v17 + v14) > (signed int)v15 )
      {
        v16 = this->uClipY;
        if ( (signed int)v16 < v5 )
          v16 = v5;
        v6 = v15 - v16;
      }
    }
    if ( v6 > 0 )
    {
      do
      {
        if ( (signed int)pTexturea > 0 )
        {
          memset32(v8, a5, pTexturea);
          v8 = (char *)v8 + 4 * pTexturea;
        }
        v8 = (char *)v8 + 4 * (window->GetWidth() - pTexturea);
        --v6;
      }
      while ( v6 );
    }
  }
}

//----- (004A5EB2) --------------------------------------------------------
void Render::DrawTextureIndexed(unsigned int uX, unsigned int uY, Texture *a4)
{
  int v5; // ebx@4
  unsigned int v8; // edx@6
  unsigned int v10; // edx@8
  unsigned int v11; // edx@9
  unsigned int v12; // esi@12
  unsigned int v13; // esi@15
  unsigned int v15; // esi@18
//  char v17; // zf@26
  int v18; // [sp+10h] [bp-10h]@4
  unsigned __int8 *v19; // [sp+18h] [bp-8h]@4
  int v20; // [sp+1Ch] [bp-4h]@4
//  int uXa; // [sp+28h] [bp+8h]@24
//  unsigned int uYa; // [sp+2Ch] [bp+Ch]@22

  if ( this->uNumSceneBegins )
  {
    if ( a4 )
    {
      if ( a4->pPalette16 )
      {
        v5 = a4->uTextureHeight;
        //pTarget = &this->pTargetSurface[uX + uY * this->uTargetSurfacePitch];
        v19 = a4->pLevelOfDetail0_prolly_alpha_mask;
        v20 = a4->uTextureWidth;
        v18 = a4->uTextureWidth;

        int clipped_out_x = uX;
        int clipped_out_y = uY;
        if ( this->bClip )
        {
          if ( (signed int)uX < (signed int)this->uClipX )
          {
            v8 = this->uClipX - uX;
            v19 += v8;
            v20 += uX - this->uClipX;
            clipped_out_x = uClipX;
          }

          v5 = a4->uTextureHeight;
          if ( (signed int)uY < (signed int)this->uClipY )
          {
            v10 = this->uClipY - uY;
            v19 += v18 * v10;
            v5 = uY - this->uClipY + a4->uTextureHeight;
            //v4 = a4;
            clipped_out_y = uClipY;
          }

          v11 = this->uClipX;
          if ( (signed int)v11 < (signed int)uX )
            v11 = uX;

          if ( (signed int)(v11 + v20) > (signed int)this->uClipZ )
          {
            v12 = this->uClipX;
            if ( (signed int)v12 < (signed int)uX )
              v12 = uX;
            v20 = this->uClipZ - v12;
          }

          v13 = this->uClipY;
          if ( (signed int)v13 < (signed int)uY )
            v13 = uY;

          if ( (signed int)(v5 + v13) > (signed int)uClipW )
          {
            v15 = this->uClipY;
            if ( (signed int)v15 < (signed int)uY )
              v15 = uY;
            v5 = uClipW - v15;
          }
        }

        for (int y = 0; y < v5; ++y)
        {
          for (int x = 0; x < v20; ++x)
          {
            if ( a4->pPalette16[*v19] != 0x7FF )// 2047
              WritePixel16(clipped_out_x + x, clipped_out_y + y, a4->pPalette16[*v19]);
            ++v19;
          }
          v19 += v18 - v20;
        }
        /*if ( (signed int)v5 > 0 )
        {
          uYa = v5;
          v16 = v19;
          do
          {
            if ( v20 > 0 )
            {
              uXa = v20;
              do
              {
                *pTarget = v4->pPalette16[*v16];
                ++pTarget;
                ++v16;
                --uXa;
              }
              while ( uXa );
            }
            v16 += v18 - v20;
            v17 = uYa-- == 1;
            pTarget += this->uTargetSurfacePitch - v20;
          }
          while ( !v17 );
        }*/
      }
    }
  }
}

//----- (004667E9) --------------------------------------------------------
void Render::ChangeBetweenWinFullscreenModes()
{
  float v0; // ST14_4@17
  int v4; // edx@26
  ObjectDesc *v5; // eax@26
  RGBTexture *v6; // esi@33
  const char *v8; // [sp-4h] [bp-28h]@33
//  struct tagRECT Rect; // [sp+14h] [bp-10h]@15

  /*if ( !pRenderer->bWindowMode && (dword_6BE364_game_settings_1 & 2) )
  {
    ModalWindow(pGlobalTXT_LocalizationStrings[62], UIMSG_0);// "Might and Magic VII requires your desktop to be in 16bit (32k or 65k) Color mode in order to operate in a window."
    return;
  }*/
  if ( pRenderer->bWindowMode || pRenderer->pRenderD3D->pAvailableDevices->bIsDeviceCompatible )
  {
    if ( pEventTimer->bPaused )
      dword_6BE364_game_settings_1 |= GAME_SETTINGS_0800;
    else
      pEventTimer->Pause();
    if ( pMiscTimer->bPaused )
      dword_6BE364_game_settings_1 |= GAME_SETTINGS_1000;
    else
      pMiscTimer->Pause();
    pMouse->bActive = 0;
    if ( pRenderer->pRenderD3D )
    {
      pBitmaps_LOD->ReleaseHardwareTextures();
      pSprites_LOD->ReleaseAll();
    }
    if ( pRenderer->bWindowMode )
    {
      //SetMenu(hWnd, 0);
      //SetWindowLongA(hWnd, -20, 0);
      //SetWindowLongA(hWnd, -16, 0x10000000u);
      window->SetFullscreenMode();
      pRenderer->InitializeFullscreen();
      v0 = (double)(signed int)uGammaPos * 0.1 + 0.6;
      pGame->pGammaController->Initialize(v0);
    }
    else
    {
      //ClipCursor(0);
      window->SetWindowedMode(window->GetWidth(), window->GetHeight());
      pRenderer->SwitchToWindow();
    }
    if ( pRenderer->pRenderD3D )
    {
      pBitmaps_LOD->_410423_move_textures_to_device();
      pSprites_LOD->MoveSpritesToVideoMemory();
    }
    if (!( pPaletteManager->uNumTargetBBits == pRenderer->uTargetBBits
      && pPaletteManager->uNumTargetGBits == pRenderer->uTargetGBits
      && pPaletteManager->uNumTargetRBits == pRenderer->uTargetRBits ))
    {
      pPaletteManager->SetColorChannelInfo(pRenderer->uTargetRBits, pRenderer->uTargetGBits, pRenderer->uTargetBBits);
      pPaletteManager->RecalculateAll();
      pBitmaps_LOD->SetupPalettes(pRenderer->uTargetRBits, pRenderer->uTargetGBits, pRenderer->uTargetBBits);
      pIcons_LOD->SetupPalettes(pRenderer->uTargetRBits, pRenderer->uTargetGBits, pRenderer->uTargetBBits);
      for (int i = 0; i < pObjectList->uNumObjects; i++)
      {
        BYTE3(v4) = 0;
        v5 = &pObjectList->pObjects[i];
        *(short *)((char *)&v4 + 1) = v5->uParticleTrailColorR;
        LOBYTE(v4) = v5->uParticleTrailColorG;
        v5->uParticleTrailColor = v5->uParticleTrailColorB | (v4 << 8);
      }
      SetUserInterface(pParty->alignment, true);
      if ( pVideoPlayer->pVideoFrame.pPixels )
        pVideoPlayer->pVideoFrame.Load(pVideoPlayer->pVideoFrameTextureFilename, 1);
      if ( sCurrentMenuID != MENU_CREATEPARTY )
      {
        if ( sCurrentMenuID == MENU_CREDITSPROC )
          dword_A74C88 = 1; 
      }
      else
      {
        if ( sCurrentMenuID )
        {
          v6 = &pTexture_PCX;
          pTexture_PCX.Release();
          v8 = "makeme.pcx";
        }
        else
        {
          v6 = &pTexture_PCX;
          pTexture_PCX.Release();
          v8 = "title.pcx";
          if ( sCurrentMenuID )
            v8 = "lsave640.pcx";
        }
        v6->Load(v8, 0);
      }
    }
    viewparams->bRedrawGameUI = 1;
    viewparams->InitGrayPalette();
    pMouse->SetCurrentCursorBitmap();
    /*if ( pRenderer->bWindowMode )
    {
      //MoveWindow(hWnd, uWindowX, uWindowY, uWindowWidth, uWindowHeight, 0);
      CenterWindowAndAdjustSize(hWnd, windowed_mode_width, windowed_mode_height);
      ShowWindow(hWnd, SW_SHOWNORMAL);
    }*/
    pMouse->bActive = true;
    if ( pVideoPlayer->AnyMovieLoaded() )
      pVideoPlayer->SelectMovieType();
    if (dword_6BE364_game_settings_1 & GAME_SETTINGS_0800)
      dword_6BE364_game_settings_1 &= ~GAME_SETTINGS_0800;
    else
      pEventTimer->Resume();
    if (dword_6BE364_game_settings_1 & GAME_SETTINGS_1000)
      dword_6BE364_game_settings_1 &= ~GAME_SETTINGS_1000;
    else
      pMiscTimer->Resume();
  }
}


//----- (004524D8) --------------------------------------------------------
HWLTexture *RenderHWLContainer::LoadTexture(const char *pName, int bMipMaps)
{
  //int v11; // eax@13
  //int v12; // ecx@13
  void *v13; // eax@13
  //unsigned __int8 v14; // zf@13
  //unsigned __int8 v15; // sf@13
  int v16; // esi@14
  int v17; // ecx@16
  int v18; // esi@16
  unsigned __int16 *v19; // eax@16
  int v20; // edx@16
  int v21; // ecx@16
  int v22; // eax@16
  int v23; // esi@16
  unsigned __int16 *v26; // [sp+24h] [bp-10h]@13
  int v27; // [sp+28h] [bp-Ch]@14
  int v28; // [sp+2Ch] [bp-8h]@13
  int pDestb; // [sp+3Ch] [bp+8h]@15

  //v3 = this;
  //sprintf(Str1, "%s", pName);
  //v4 = uNumItems;
  if (!uNumItems)
    return nullptr;

  //{
  //v5 = 0, pDesta = uNumItems;
  uint idx1 = 0,
       idx2 = uNumItems;
  while (true)
  {
    uint i = idx1 + (idx2 - idx1) / 2;

    int res = _stricmp(pName, pSpriteNames[i]);
    if (!res)
    {
      fseek(pFile, pSpriteOffsets[i], SEEK_SET);
      break;
    }
    else if (res < 0)
      idx2 = idx1 + (idx2 - idx1) / 2;
    else
      idx1 = i + 1;

    if ( idx1 >= idx2 )
      return false;
  }


  uint uCompressedSize = 0;
  fread(&uCompressedSize, 4, 1, pFile);

    HWLTexture* pTex = new HWLTexture;
    fread(&pTex->uBufferWidth, 4, 1, pFile);
    fread(&pTex->uBufferHeight, 4, 1, pFile);
    fread(&pTex->uAreaWidth, 4, 1, pFile);
    fread(&pTex->uAreaHeigth, 4, 1, pFile);
    fread(&pTex->uWidth, 4, 1, pFile);
    fread(&pTex->uHeight, 4, 1, pFile);
    fread(&pTex->uAreaX, 4, 1, pFile);
    fread(&pTex->uAreaY, 4, 1, pFile);

    pTex->pPixels = new unsigned __int16[pTex->uWidth * pTex->uHeight];
    if (uCompressedSize)
    {
      char* pCompressedData = new char[uCompressedSize];
      {
        fread(pCompressedData, 1, uCompressedSize, pFile);
        uint uDecompressedSize = pTex->uWidth * pTex->uHeight * sizeof(short);
        zlib::MemUnzip(pTex->pPixels, &uDecompressedSize, pCompressedData, uCompressedSize);
      }
      delete [] pCompressedData;
    }
    else
      fread(pTex->pPixels, 2, pTex->uWidth * pTex->uHeight, pFile);

    if ( scale_hwls_to_half )
    {
      pTex->uHeight /= 2;
      pTex->uWidth /= 2;
      v13 = new unsigned __int16[pTex->uWidth * pTex->uHeight];
      v28 = 0;
      v26 = (unsigned __int16 *)v13;
      if ( pTex->uHeight > 0 )
      {
        v16 = pTex->uWidth;
        v27 = 1;
        do
        {
          pDestb = 0;
          if ( v16 > 0 )
          {
            do
            {
              v17 = v16 * v27;
              v18 = v28 * v16;
              v19 = pTex->pPixels;
              v20 = pDestb + 2 * v18;
              v21 = (int)&v19[2 * (pDestb + v17)];
              v22 = (int)&v19[2 * v20];
              LOWORD(v20) = *(unsigned short *)(v21 + 2);
              LOWORD(v21) = *(unsigned short *)v21;
              v23 = pDestb++ + v18;
              extern unsigned int __fastcall _452442_color_cvt(unsigned __int16 a1, unsigned __int16 a2, int a3, int a4);
              v26[v23] = _452442_color_cvt(*(unsigned short *)v22, *(unsigned short *)(v22 + 2), v21, v20);
              v16 = pTex->uWidth;
            }
            while (pDestb < pTex->uWidth);
          }
          ++v28;
          v27 += 2;
        }
        while ( v28 < (signed int)pTex->uHeight );
      }
      delete [] pTex->pPixels;
      pTex->pPixels = v26;
    }
    return pTex;
}

//----- (0045271F) --------------------------------------------------------
bool RenderHWLContainer::Release()
{
  __int32 v4; // eax@6
  FILE *v5; // ST24_4@6
  FILE *File; // [sp+4h] [bp-4h]@6

  if ( this->bDumpDebug)
  {
    File = fopen("logd3d.txt", "w");
    v4 = ftell(this->pFile);
    v5 = this->pFile;
    this->uDataOffset = v4;
    fwrite(&this->uNumItems, 4u, 1u, v5);
    for (int i = 0; i < this->uNumItems; i++)
    {
      fwrite(this->pSpriteNames[i], 1u, 0x14u, this->pFile);
      fprintf(File, "D3D texture name:  %s\t\toffset: %x\n", this->pSpriteNames[i], *(unsigned int *)(&(this->pSpriteNames[i]) + 200000/sizeof(char*)));
    }
    fwrite(this->pSpriteOffsets, 4u, this->uNumItems, this->pFile);
    fseek(this->pFile, 4, 0);
    fwrite(&this->uDataOffset, 4u, 1u, this->pFile);
    fclose(this->pFile);
    fclose(File);
  }
  else
  {
    fclose(this->pFile);
    for (int i = 0; i < this->uNumItems; i++)
    {
      free(this->pSpriteNames[i]);
    }
  }
  return true;
}

//----- (00452347) --------------------------------------------------------
RenderHWLContainer::RenderHWLContainer():
  bDumpDebug(false)
{
  this->pFile = 0;
  uSignature = 0;
  this->uDataOffset = 0;
  memset(&this->uNumItems, 0, 0x61A84u);
  this->uNumItems = 0;
  this->scale_hwls_to_half = false;
}

//----- (0045237F) --------------------------------------------------------
bool RenderHWLContainer::Load(const wchar_t *pFilename)
{
  pFile = _wfopen(pFilename, L"rb");
  if (!pFile)
  {
    Log::Warning(L"Failed to open file: %s", pFilename);
    return false;
  }

  fread(&uSignature, 1, 4, pFile);
  if (uSignature != 'TD3D')
  {
    Log::Warning(L"Invalid format: %s", pFilename);
    return false;
  }
  
  fread(&uDataOffset, 4, 1, pFile);
  fseek(pFile, uDataOffset, SEEK_SET);
  fread(&uNumItems, 4, 1, pFile);

  memset(pSpriteNames, 0, 50000 * sizeof(char *));
  for (uint i = 0; i < uNumItems; ++i)
  {
    pSpriteNames[i] = new char[20];
    fread(pSpriteNames[i], 1, 20, pFile);
  }
  fread(pSpriteOffsets, 4, uNumItems, pFile);

  return true;
}

//----- (004A1C1E) --------------------------------------------------------
void Render::DoRenderBillboards_D3D()
{
  ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_CLAMP));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE));
  
  /*if (pRenderer->uNumBillboardsToDraw)
  {
  auto p = &pRenderer->pBillboardRenderListD3D[0];
  for (int i = 0; i < p->uNumVertices; ++i)
  {
    p->pQuads[i].pos.z -= p->pQuads[i].pos.z * 0.6;
    //p->pQuads[i].rhw = + 0.8 * (1.0f - p->pQuads[i].rhw);
  }
  p->pQuads[0].pos.x = 10;
  p->pQuads[0].pos.y = 10;

  p->pQuads[1].pos.x = 10;
  p->pQuads[1].pos.y = 200;

  p->pQuads[2].pos.x = 100;
  p->pQuads[2].pos.y = 200;

  p->pQuads[3].pos.x = 100;
  p->pQuads[3].pos.y = 10;

      if (p->uOpacity != RenderBillboardD3D::NoBlend)
      SetBillboardBlendOptions(p->uOpacity);
    
    pRenderer->pRenderD3D->pDevice->SetTexture(0, p->pTexture);
    ErrD3D(pRenderer->pRenderD3D->pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,
                                                         D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1,
                                                         p->pQuads, p->uNumVertices,
                                                         D3DDP_DONOTLIGHT | D3DDP_DONOTUPDATEEXTENTS));

  }*/

  for (int i = uNumBillboardsToDraw - 1; i >= 0; --i)
  {
    if (pBillboardRenderListD3D[i].opacity != RenderBillboardD3D::NoBlend)
      SetBillboardBlendOptions(pBillboardRenderListD3D[i].opacity);

    pRenderD3D->pDevice->SetTexture(0, pBillboardRenderListD3D[i].pTexture);
    ErrD3D(pRenderD3D->pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,
                                              D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1,
                                              pBillboardRenderListD3D[i].pQuads, pBillboardRenderListD3D[i].uNumVertices,
                                              D3DDP_DONOTLIGHT | D3DDP_DONOTUPDATEEXTENTS));
  }

  if (bFogEnabled)
  {
    bFogEnabled = false;
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, TRUE));
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_FOGCOLOR, GetLevelFogColor() & 0xFFFFFF));
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_FOGTABLEMODE, 0));
  }
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CW));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ZERO));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DITHERENABLE, TRUE));
}

//----- (004A1DA8) --------------------------------------------------------
void Render::SetBillboardBlendOptions(RenderBillboardD3D::OpacityType a1)
{
  switch (a1)
  {
    case RenderBillboardD3D::Transparent:
    {
      if (bFogEnabled)
      {
        bFogEnabled = false;
        ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, TRUE));
        ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_FOGCOLOR, GetLevelFogColor() & 0xFFFFFF));
        ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_FOGTABLEMODE, 0));
      }

      ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA));
      ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA));
      ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DITHERENABLE, TRUE));
    }
    break;

    case RenderBillboardD3D::Opaque_1:
    case RenderBillboardD3D::Opaque_2:
    case RenderBillboardD3D::Opaque_3:
    {
      if (bUsingSpecular)
      {
        if (!bFogEnabled)
        {
          bFogEnabled = true;
          ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE));
        }
      }

      ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE));
      ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE));
      ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DITHERENABLE, FALSE));
    }
    break;

    default:
      Log::Warning(L"SetBillboardBlendOptions: invalid opacity type (%u)", a1);
      assert(false);
    break;
  }
}
//----- (00424CD7) --------------------------------------------------------
int ODM_NearClip(unsigned int num_vertices)
{
  bool current_vertices_flag; // edi@1
  bool next_vertices_flag; // [sp+Ch] [bp-24h]@6
  double t; // st6@10
  bool bFound;

  bFound = false;

  if (!num_vertices)
    return 0;
  for (uint i = 0; i < num_vertices; ++i)// есть ли пограничные вершины
  {
    if ( array_50AC10[i].vWorldViewPosition.x > 8.0 )
    {
      bFound = true;
      break;
    }
  }
  if ( !bFound )
    return 0;

  memcpy(&array_50AC10[num_vertices], &array_50AC10[0], sizeof(array_50AC10[0]));
  current_vertices_flag = false;
  next_vertices_flag = false;
  if ( array_50AC10[0].vWorldViewPosition.x <= 8.0 )
    current_vertices_flag = true;
  //check for near clip plane(проверка по ближней границе)
  //   
  // v3.__________________. v0
  //   |                  |
  //   |                  |
  //   |                  |
  //  ----------------------- 8.0(near_clip - 8.0)
  //   |                  |
  //   .__________________.
  //  v2                     v1

  int out_num_vertices = 0;
  for (int i = 0; i < num_vertices; ++i)
  {
    next_vertices_flag = array_50AC10[i + 1].vWorldViewPosition.x <= 8.0;//
    if ( current_vertices_flag ^ next_vertices_flag )
    {
      if ( next_vertices_flag )//следующая вершина за ближней границей
      {
        //t = near_clip - v0.x / v1.x - v0.x    (формула получения точки пересечения отрезка с плоскостью)
        t = (8.0 - array_50AC10[i].vWorldViewPosition.x) / (array_50AC10[i + 1].vWorldViewPosition.x - array_50AC10[i].vWorldViewPosition.x);
        array_507D30[out_num_vertices].vWorldViewPosition.x = 8.0;
        array_507D30[out_num_vertices].vWorldViewPosition.y = array_50AC10[i].vWorldViewPosition.y + (array_50AC10[i + 1].vWorldViewPosition.y - array_50AC10[i].vWorldViewPosition.y) * t;
        array_507D30[out_num_vertices].vWorldViewPosition.z = array_50AC10[i].vWorldViewPosition.z + (array_50AC10[i + 1].vWorldViewPosition.z - array_50AC10[i].vWorldViewPosition.z) * t;
        array_507D30[out_num_vertices].u = array_50AC10[i].u + (array_50AC10[i + 1].u - array_50AC10[i].u) * t;
        array_507D30[out_num_vertices].v = array_50AC10[i].v + (array_50AC10[i + 1].v - array_50AC10[i].v) * t;
        array_507D30[out_num_vertices]._rhw = 1.0 / 8.0;
      }
      else// текущая вершина за ближней границей
      {
        t = (8.0 - array_50AC10[i].vWorldViewPosition.x) / (array_50AC10[i].vWorldViewPosition.x - array_50AC10[i + 1].vWorldViewPosition.x);
        array_507D30[out_num_vertices].vWorldViewPosition.x = 8.0;
        array_507D30[out_num_vertices].vWorldViewPosition.y = array_50AC10[i].vWorldViewPosition.y + (array_50AC10[i].vWorldViewPosition.y - array_50AC10[i + 1].vWorldViewPosition.y) * t;
        array_507D30[out_num_vertices].vWorldViewPosition.z = array_50AC10[i].vWorldViewPosition.z + (array_50AC10[i].vWorldViewPosition.z - array_50AC10[i + 1].vWorldViewPosition.z) * t;
        array_507D30[out_num_vertices].u = array_50AC10[i].u + (array_50AC10[i].u - array_50AC10[i + 1].u) * t;
        array_507D30[out_num_vertices].v = array_50AC10[i].v + (array_50AC10[i].v - array_50AC10[i + 1].v) * t;
        array_507D30[out_num_vertices]._rhw = 1.0 / 8.0;
      }
      //array_507D30[out_num_vertices]._rhw = 0x3E000000u;
      ++out_num_vertices;
    }
    if ( !next_vertices_flag )
    {
      memcpy(&array_507D30[out_num_vertices], &array_50AC10[i + 1], sizeof(array_50AC10[i + 1]));
      out_num_vertices++;
    }
    current_vertices_flag = next_vertices_flag;
  }
  return out_num_vertices >= 3 ? out_num_vertices : 0;
}

//----- (00424EE0) --------------------------------------------------------
int ODM_FarClip(unsigned int uNumVertices)
{
  bool current_vertices_flag; // [sp+Ch] [bp-28h]@6
  bool next_vertices_flag; // edi@1
  double t; // st6@10
  signed int depth_num_vertices; // [sp+18h] [bp-1Ch]@1
  bool bFound;
  //Доп инфо "Программирование трёхмерных игр для windows" Ламот стр 910

  bFound = false;

  memcpy(&array_50AC10[uNumVertices], &array_50AC10[0], sizeof(array_50AC10[uNumVertices]));
  depth_num_vertices = 0;
  current_vertices_flag = false;
  if ( array_50AC10[0].vWorldViewPosition.x >= pODMRenderParams->shading_dist_mist )
    current_vertices_flag = true;//настоящая вершина больше границы видимости
  if ( (signed int)uNumVertices <= 0 )
    return 0;
  for (uint i = 0; i < uNumVertices; ++i)// есть ли пограничные вершины
  {
    if ( array_50AC10[i].vWorldViewPosition.x < pODMRenderParams->shading_dist_mist )
    {
      bFound = true;
      break;
    }
  }
  if ( !bFound )
    return 0;
  //check for far clip plane(проверка по дальней границе)
  //   
  // v3.__________________. v0
  //   |                  |
  //   |                  |
  //   |                  |
  //  ----------------------- 8192.0(far_clip - 0x2000)
  //   |                  |
  //   .__________________.
  //  v2                     v1

  for ( uint i = 0; i < uNumVertices; ++i )
  {
    next_vertices_flag = array_50AC10[i + 1].vWorldViewPosition.x >= pODMRenderParams->shading_dist_mist;
    if ( current_vertices_flag ^ next_vertices_flag )//одна из граней за границей видимости
    {
      if ( next_vertices_flag )//следующая вершина больше границы видимости(настоящая вершина меньше границы видимости) - v3
      {
        //t = far_clip - v2.x / v3.x - v2.x (формула получения точки пересечения отрезка с плоскостью)
        t = (pODMRenderParams->shading_dist_mist - array_50AC10[i].vWorldViewPosition.x) / (array_50AC10[i].vWorldViewPosition.x - array_50AC10[i + 1].vWorldViewPosition.x);
        array_507D30[depth_num_vertices].vWorldViewPosition.x = pODMRenderParams->shading_dist_mist;
        //New_y = v2.y + (v3.y - v2.y)*t
        array_507D30[depth_num_vertices].vWorldViewPosition.y = array_50AC10[i].vWorldViewPosition.y + (array_50AC10[i].vWorldViewPosition.y - array_50AC10[i + 1].vWorldViewPosition.y) * t;
        //New_z = v2.z + (v3.z - v2.z)*t
        array_507D30[depth_num_vertices].vWorldViewPosition.z = array_50AC10[i].vWorldViewPosition.z + (array_50AC10[i].vWorldViewPosition.z - array_50AC10[i + 1].vWorldViewPosition.z) * t;
        array_507D30[depth_num_vertices].u = array_50AC10[i].u + (array_50AC10[i].u - array_50AC10[i + 1].u) * t;
        array_507D30[depth_num_vertices].v = array_50AC10[i].v + (array_50AC10[i].v - array_50AC10[i + 1].v) * t;
        array_507D30[depth_num_vertices]._rhw = 1.0 / pODMRenderParams->shading_dist_mist;
      }
      else//настоящая вершина больше границы видимости(следующая вершина меньше границы видимости) - v0
      {
        //t = far_clip - v1.x / v0.x - v1.x
        t = (pODMRenderParams->shading_dist_mist - array_50AC10[i].vWorldViewPosition.x) / (array_50AC10[i + 1].vWorldViewPosition.x - array_50AC10[i].vWorldViewPosition.x);
        array_507D30[depth_num_vertices].vWorldViewPosition.x = pODMRenderParams->shading_dist_mist;
        //New_y = (v0.y - v1.y)*t + v1.y
        array_507D30[depth_num_vertices].vWorldViewPosition.y = array_50AC10[i].vWorldViewPosition.y + (array_50AC10[i + 1].vWorldViewPosition.y - array_50AC10[i].vWorldViewPosition.y) * t;
        //New_z = (v0.z - v1.z)*t + v1.z
        array_507D30[depth_num_vertices].vWorldViewPosition.z = array_50AC10[i].vWorldViewPosition.z + (array_50AC10[i + 1].vWorldViewPosition.z - array_50AC10[i].vWorldViewPosition.z) * t;
        array_507D30[depth_num_vertices].u = array_50AC10[i].u + (array_50AC10[i + 1].u - array_50AC10[i].u) * t;
        array_507D30[depth_num_vertices].v = array_50AC10[i].v + (array_50AC10[i + 1].v - array_50AC10[i].v) * t;
        array_507D30[depth_num_vertices]._rhw = 1.0 / pODMRenderParams->shading_dist_mist;
      }
      ++depth_num_vertices;
    }
    if ( !next_vertices_flag )//оба в границе видимости
    {
      memcpy(&array_507D30[depth_num_vertices], &array_50AC10[i + 1], sizeof(array_507D30[depth_num_vertices]));
      depth_num_vertices++;
    }
    current_vertices_flag = next_vertices_flag;
  }
  return depth_num_vertices >= 3 ? depth_num_vertices : 0;
}

//----- (0047840D) --------------------------------------------------------
void Render::DrawBuildingsD3D()
{
  int v9; // ecx@8
  Texture *pFaceTexture; // eax@10
  unsigned int v16; // edi@22
  int v27; // eax@57
//  int vertex_id; // eax@58
  unsigned int v34; // eax@80
  int v40; // [sp-4h] [bp-5Ch]@2
  int v49; // [sp+2Ch] [bp-2Ch]@10
  int v50; // [sp+30h] [bp-28h]@34
  int v51; // [sp+34h] [bp-24h]@35
  int v52; // [sp+38h] [bp-20h]@36
  int v53; // [sp+3Ch] [bp-1Ch]@8
  int uNumVertices; // [sp+4Ch] [bp-Ch]@34
  int unused; // [sp+50h] [bp-8h]@3

  if ( !pRenderer->pRenderD3D )
  {
    MessageBoxW(nullptr, L"D3D version of RenderBuildings called in software!", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Odbuild.cpp:73", 0);
  }

  unused = 0;
  if ( (signed int)pOutdoor->uNumBModels > 0 )
  {
    for ( uint model_id = 0; model_id < pOutdoor->uNumBModels; model_id++ )
    {
      if ( IsBModelVisible(model_id, &unused) )
      {
        pOutdoor->pBModels[model_id].field_40 |= 1;
        if ( pOutdoor->pBModels[model_id].uNumFaces > 0 )
        {
          for ( int face_id = 0; face_id < pOutdoor->pBModels[model_id].uNumFaces; face_id++ )
          {
            if (!pOutdoor->pBModels[model_id].pFaces[face_id].Invisible())
            {
            v53 = 0;
            array_77EC08[pODMRenderParams->uNumPolygons].flags = 0;
            array_77EC08[pODMRenderParams->uNumPolygons].field_32 = 0;
            v9 = pOutdoor->pBModels[model_id].pFaces[face_id].uTextureID;
            if (pOutdoor->pBModels[model_id].pFaces[face_id].uAttributes & FACE_TEXTURE_FRAME)
              v9 = pTextureFrameTable->GetFrameTexture(v9, pEventTimer->uTotalGameTimeElapsed);
            pFaceTexture = pBitmaps_LOD->GetTexture(v9);
            array_77EC08[pODMRenderParams->uNumPolygons].pTexture = pFaceTexture;
            if (pOutdoor->pBModels[model_id].pFaces[face_id].uAttributes & FACE_FLUID)
              array_77EC08[pODMRenderParams->uNumPolygons].flags |= 2;
            if (pOutdoor->pBModels[model_id].pFaces[face_id].uAttributes & FACE_INDOOR_SKY )
              HIBYTE(array_77EC08[pODMRenderParams->uNumPolygons].flags) |= 4;
            if ( pOutdoor->pBModels[model_id].pFaces[face_id].uAttributes & FACE_UNKNOW8 )
              HIBYTE(array_77EC08[pODMRenderParams->uNumPolygons].flags) |= 4;
            else
            {
              if ( pOutdoor->pBModels[model_id].pFaces[face_id].uAttributes & FACE_UNKNOW9 )
                HIBYTE(array_77EC08[pODMRenderParams->uNumPolygons].flags) |= 8;
            }
            if (pOutdoor->pBModels[model_id].pFaces[face_id].uAttributes & FACE_UNKNOW10)
              array_77EC08[pODMRenderParams->uNumPolygons].flags |= 0x2000;
            else
            {
              if (pOutdoor->pBModels[model_id].pFaces[face_id].uAttributes & FACE_DONT_CACHE_TEXTURE)
                HIBYTE(array_77EC08[pODMRenderParams->uNumPolygons].flags) |= 0x10u;
            }
            array_77EC08[pODMRenderParams->uNumPolygons].sTextureDeltaU = pOutdoor->pBModels[model_id].pFaces[face_id].sTextureDeltaU;
            array_77EC08[pODMRenderParams->uNumPolygons].sTextureDeltaV = pOutdoor->pBModels[model_id].pFaces[face_id].sTextureDeltaV;
            v16 = GetTickCount() >> 4;
            if ( pOutdoor->pBModels[model_id].pFaces[face_id].pFacePlane.vNormal.z && abs(pOutdoor->pBModels[model_id].pFaces[face_id].pFacePlane.vNormal.z) >= 59082 )
            {
              if ( BYTE1(array_77EC08[pODMRenderParams->uNumPolygons].flags) & 4 )
                array_77EC08[pODMRenderParams->uNumPolygons].sTextureDeltaV += v16 & array_77EC08[pODMRenderParams->uNumPolygons].pTexture->uHeightMinus1;
              if ( BYTE1(array_77EC08[pODMRenderParams->uNumPolygons].flags) & 8 )
                array_77EC08[pODMRenderParams->uNumPolygons].sTextureDeltaV -= v16 & array_77EC08[pODMRenderParams->uNumPolygons].pTexture->uHeightMinus1;
            }
            else
            {
              if ( BYTE1(array_77EC08[pODMRenderParams->uNumPolygons].flags) & 4 )
                array_77EC08[pODMRenderParams->uNumPolygons].sTextureDeltaV -= v16 & array_77EC08[pODMRenderParams->uNumPolygons].pTexture->uHeightMinus1;
              if ( BYTE1(array_77EC08[pODMRenderParams->uNumPolygons].flags) & 8 )
                array_77EC08[pODMRenderParams->uNumPolygons].sTextureDeltaV += v16 & array_77EC08[pODMRenderParams->uNumPolygons].pTexture->uHeightMinus1;
            }
            if ( BYTE1(array_77EC08[pODMRenderParams->uNumPolygons].flags) & 0x10 )
              array_77EC08[pODMRenderParams->uNumPolygons].sTextureDeltaU -= v16 & array_77EC08[pODMRenderParams->uNumPolygons].pTexture->uWidthMinus1;
            else
            {
              if ( BYTE1(array_77EC08[pODMRenderParams->uNumPolygons].flags) & 0x20 )
                array_77EC08[pODMRenderParams->uNumPolygons].sTextureDeltaU += v16 & array_77EC08[pODMRenderParams->uNumPolygons].pTexture->uWidthMinus1;
            }
            v50 = 0;
            v49 = 0;
            uNumVertices = pOutdoor->pBModels[model_id].pFaces[face_id].uNumVertices;
            if ( pOutdoor->pBModels[model_id].pFaces[face_id].uNumVertices > 0 )
            {
              for ( uint vertex_id = 1; vertex_id <= pOutdoor->pBModels[model_id].pFaces[face_id].uNumVertices; vertex_id++ )
              {
                array_73D150[vertex_id - 1].vWorldPosition.x = pOutdoor->pBModels[model_id].pVertices.pVertices[pOutdoor->pBModels[model_id].pFaces[face_id].pVertexIDs[vertex_id - 1]].x;
                array_73D150[vertex_id - 1].vWorldPosition.y = pOutdoor->pBModels[model_id].pVertices.pVertices[pOutdoor->pBModels[model_id].pFaces[face_id].pVertexIDs[vertex_id - 1]].y;
                array_73D150[vertex_id - 1].vWorldPosition.z = pOutdoor->pBModels[model_id].pVertices.pVertices[pOutdoor->pBModels[model_id].pFaces[face_id].pVertexIDs[vertex_id - 1]].z;
                array_73D150[vertex_id - 1].u = (array_77EC08[pODMRenderParams->uNumPolygons].sTextureDeltaU + (signed __int16)pOutdoor->pBModels[model_id].pFaces[face_id].pTextureUIDs[vertex_id - 1]) * (1.0 / (double)pFaceTexture->uTextureWidth);
                array_73D150[vertex_id - 1].v = (array_77EC08[pODMRenderParams->uNumPolygons].sTextureDeltaV + (signed __int16)pOutdoor->pBModels[model_id].pFaces[face_id].pTextureVIDs[vertex_id - 1]) * (1.0 / (double)pFaceTexture->uTextureHeight);
              }
              for ( uint i = 1; i <= pOutdoor->pBModels[model_id].pFaces[face_id].uNumVertices; i++ )
              {
                if ( pOutdoor->pBModels[model_id].pVertices.pVertices[pOutdoor->pBModels[model_id].pFaces[face_id].pVertexIDs[0]].z == array_73D150[i - 1].vWorldPosition.z )
                  ++v53;
                pGame->pIndoorCameraD3D->ViewTransform(&array_73D150[i - 1], 1);
                if ( array_73D150[i - 1].vWorldViewPosition.x < 8.0 || array_73D150[i - 1].vWorldViewPosition.x > pODMRenderParams->shading_dist_mist )
                {
                  if ( array_73D150[i - 1].vWorldViewPosition.x >= 8.0 )
                    v49 = 1;
                  else
                    v50 = 1;
                }
                else
                  pGame->pIndoorCameraD3D->Project(&array_73D150[i - 1], 1, 0);
              }
            }
            if ( v53 == pOutdoor->pBModels[model_id].pFaces[face_id].uNumVertices )
              LOBYTE(array_77EC08[pODMRenderParams->uNumPolygons].field_32) |= 1;
            array_77EC08[pODMRenderParams->uNumPolygons].pODMFace = &pOutdoor->pBModels[model_id].pFaces[face_id];
            array_77EC08[pODMRenderParams->uNumPolygons].uNumVertices = pOutdoor->pBModels[model_id].pFaces[face_id].uNumVertices;
            array_77EC08[pODMRenderParams->uNumPolygons].field_59 = 5;
            v51 = fixpoint_mul(-pOutdoor->vSunlight.x, pOutdoor->pBModels[model_id].pFaces[face_id].pFacePlane.vNormal.x);
            v53 = fixpoint_mul(-pOutdoor->vSunlight.y, pOutdoor->pBModels[model_id].pFaces[face_id].pFacePlane.vNormal.y);
            v52 = fixpoint_mul(-pOutdoor->vSunlight.z, pOutdoor->pBModels[model_id].pFaces[face_id].pFacePlane.vNormal.z);
            array_77EC08[pODMRenderParams->uNumPolygons].dimming_level = 20 - fixpoint_mul(20, v51 + v53 + v52);
            if ( array_77EC08[pODMRenderParams->uNumPolygons].dimming_level < 0 )
              array_77EC08[pODMRenderParams->uNumPolygons].dimming_level = 0;
            if ( array_77EC08[pODMRenderParams->uNumPolygons].dimming_level > 31 )
              array_77EC08[pODMRenderParams->uNumPolygons].dimming_level = 31;
            if ( pODMRenderParams->uNumPolygons >= 1999 + 5000)
              return;
            if ( ODMFace::IsBackfaceNotCulled(array_73D150, &array_77EC08[pODMRenderParams->uNumPolygons]) )
            {
              pOutdoor->pBModels[model_id].pFaces[face_id].bVisible = 1;
              array_77EC08[pODMRenderParams->uNumPolygons].uBModelFaceID = face_id;
              array_77EC08[pODMRenderParams->uNumPolygons].uBModelID = model_id;
              v27 = 8 * (face_id | (model_id << 6));
              LOBYTE(v27) = v27 | 6;
              array_77EC08[pODMRenderParams->uNumPolygons].field_50 = v27;
              for ( int vertex_id = 0; vertex_id < pOutdoor->pBModels[model_id].pFaces[face_id].uNumVertices; ++vertex_id)
              {
                memcpy(&array_50AC10[vertex_id], &array_73D150[vertex_id], sizeof(array_50AC10[vertex_id]));
                array_50AC10[vertex_id]._rhw = 1.0 / (array_73D150[vertex_id].vWorldViewPosition.x + 0.0000001);
              }
              static stru154 static_RenderBuildingsD3D_stru_73C834;
              /*static bool __init_flag = false;
              if (!__init_flag)
              {
                __init_flag = true;
                static_RenderBuildingsD3D_byte_73C84C_init_flag |= 1u;
                stru154::stru154(&static_RenderBuildingsD3D_stru_73C834);
                atexit(loc_4789D4);
              }*/

              v40 = (int)&pOutdoor->pBModels[model_id].pFaces[face_id];
              pGame->pLightmapBuilder->ApplyLights_OutdoorFace(&pOutdoor->pBModels[model_id].pFaces[face_id]);
              pDecalBuilder->ApplyDecals_OutdoorFace(&pOutdoor->pBModels[model_id].pFaces[face_id]);
              pGame->pLightmapBuilder->std__vector_000004_size = 0;
              int v31 = 0;
              if ( stru_F8AD28.uNumLightsApplied > 0 || pDecalBuilder->uNumDecals > 0 )
              {
                v31 = v50 ? 3 : v49 != 0 ? 5 : 0;
                static_RenderBuildingsD3D_stru_73C834.GetFacePlaneAndClassify(&pOutdoor->pBModels[model_id].pFaces[face_id], &pOutdoor->pBModels[model_id].pVertices);
                if ( pDecalBuilder->uNumDecals > 0 )
                {
                  v40 = -1;
                  pDecalBuilder->ApplyDecals(31 - array_77EC08[pODMRenderParams->uNumPolygons].dimming_level, 2, &static_RenderBuildingsD3D_stru_73C834,
                                       pOutdoor->pBModels[model_id].pFaces[face_id].uNumVertices, array_50AC10, 0, (char)v31, -1);
                }
              }
              if ( stru_F8AD28.uNumLightsApplied > 0 )
                pGame->pLightmapBuilder->ApplyLights(&stru_F8AD28, &static_RenderBuildingsD3D_stru_73C834, uNumVertices, array_50AC10, 0, (char)v31);
              if ( v50 )
              {
                array_77EC08[pODMRenderParams->uNumPolygons].uNumVertices = ODM_NearClip(pOutdoor->pBModels[model_id].pFaces[face_id].uNumVertices);
                uNumVertices = array_77EC08[pODMRenderParams->uNumPolygons].uNumVertices;
                ODM_Project(array_77EC08[pODMRenderParams->uNumPolygons].uNumVertices);
              }
              if ( v49 )
              {
                array_77EC08[pODMRenderParams->uNumPolygons].uNumVertices = ODM_FarClip(pOutdoor->pBModels[model_id].pFaces[face_id].uNumVertices);
                uNumVertices = array_77EC08[pODMRenderParams->uNumPolygons].uNumVertices;
                ODM_Project(array_77EC08[pODMRenderParams->uNumPolygons].uNumVertices);
              }
              if ( uNumVertices )
              {
                if ( array_77EC08[pODMRenderParams->uNumPolygons].flags & 2 )
                {
                  if ( BYTE1(array_77EC08[pODMRenderParams->uNumPolygons].flags) & 0x3C )
                    v34 = pRenderer->pHDWaterBitmapIDs[0];
                  else
                    v34 = pRenderer->pHDWaterBitmapIDs[pRenderer->hd_water_current_frame];
                  v40 = (int)pBitmaps_LOD->pHardwareTextures[v34];
                }
                else
                  v40 = (int)pBitmaps_LOD->pHardwareTextures[v9];
                pRenderer->DrawPolygon(uNumVertices, &array_77EC08[pODMRenderParams->uNumPolygons], &pOutdoor->pBModels[model_id].pFaces[face_id], (IDirect3DTexture2 *)v40);
              }
            }
          }
          }
        }
      }
    }
  }
  return;
}
//----- (00479543) --------------------------------------------------------
void Render::DrawOutdoorSkyD3D()
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
  int v30; // [sp+134h] [bp-30h]@1
  int v32; // [sp+13Ch] [bp-28h]@6
  int v33; // [sp+140h] [bp-24h]@2
  signed __int64 v34; // [sp+144h] [bp-20h]@1
  int v35; // [sp+148h] [bp-1Ch]@4
  int v36; // [sp+14Ch] [bp-18h]@2
  int v37; // [sp+154h] [bp-10h]@8
  int v38; // [sp+158h] [bp-Ch]@1
  int v39; // [sp+15Ch] [bp-8h]@4

  v30 = (signed __int64)((double)(pODMRenderParams->int_fov_rad * pGame->pIndoorCameraD3D->vPartyPos.z)
                       / ((double)pODMRenderParams->int_fov_rad + 8192.0)
                       + (double)(pViewport->uScreenCenterY));
  v34 = cos((double)pGame->pIndoorCameraD3D->sRotationX * 0.0030664064) * 0x2000;//(double)pODMRenderParams->shading_dist_mist, 8192
  v38 = (signed __int64)((double)(pViewport->uScreenCenterY)
                       - (double)pODMRenderParams->int_fov_rad
                       / (v34 + 0.0000001)
                       * (sin((double)pGame->pIndoorCameraD3D->sRotationX * 0.0030664064)
                        * (double)-0x2000//(double)pODMRenderParams->shading_dist_mist
                        - (double)pGame->pIndoorCameraD3D->vPartyPos.z));
  pSkyPolygon.Create_48607B(&stru_8019C8);//заполняется ptr_38
  pSkyPolygon.ptr_38->_48694B_frustum_sky();
  pSkyPolygon.uTileBitmapID = pOutdoor->sSky_TextureID;//179(original 166)
  pSkyPolygon.pTexture = (Texture *)(SLOWORD(pOutdoor->sSky_TextureID) != -1 ? (int)&pBitmaps_LOD->pTextures[SLOWORD(pOutdoor->sSky_TextureID)] : 0);
  if ( pSkyPolygon.pTexture )
  {
    pSkyPolygon.dimming_level = 0;
    pSkyPolygon.uNumVertices = 4;
  //centering(центруем)-----------------------------------------------------------------
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
    array_50AC10[0].vWorldViewProjX = (double)(signed int)pViewport->uViewportTL_X;//8
    array_50AC10[0].vWorldViewProjY = (double)(signed int)pViewport->uViewportTL_Y;//8

    array_50AC10[1].vWorldViewProjX = (double)(signed int)pViewport->uViewportTL_X;//8
    array_50AC10[1].vWorldViewProjY = (double)v38;//247

    array_50AC10[2].vWorldViewProjX = (double)(signed int)pViewport->uViewportBR_X;//468
    array_50AC10[2].vWorldViewProjY = (double)v38;//247

    array_50AC10[3].vWorldViewProjX = (double)(signed int)pViewport->uViewportBR_X;//468
    array_50AC10[3].vWorldViewProjY = (double)(signed int)pViewport->uViewportTL_Y;//8

    pSkyPolygon.sTextureDeltaU = 224 * pMiscTimer->uTotalGameTimeElapsed;//7168
    pSkyPolygon.sTextureDeltaV = 224 * pMiscTimer->uTotalGameTimeElapsed;//7168

    pSkyPolygon.field_24 = 0x2000000u;//maybe attributes
    v33 = 65536 / (signed int)(signed __int64)(((double)(pViewport->uViewportBR_X - pViewport->uViewportTL_X) / 2) / tan(0.6457717418670654) + 0.5);
    for ( uint i = 0; i < pSkyPolygon.uNumVertices; ++i )
    {
      //rotate skydome(вращение купола неба)--------------------------------------
      // В игре принята своя система измерения углов. Полный угол (180). Значению угла 0 соответствует 
      // направление на север и/или юг (либо на восток и/или запад), значению 65536 еденицам(0х10000) соответствует угол 90.
      // две переменные хранят данные по углу обзора. field_14 по западу и востоку. field_20 по югу и северу
      // от -25080 до 25080
      v39 = fixpoint_mul(pSkyPolygon.ptr_38->viewing_angle_from_west_east, v33 * (v30 - floor(array_50AC10[i].vWorldViewProjY + 0.5)));
      v35 = v39 + pSkyPolygon.ptr_38->angle_from_north;

      v39 = fixpoint_mul(pSkyPolygon.ptr_38->viewing_angle_from_north_south, v33 * (v30 - floor(array_50AC10[i].vWorldViewProjY + 0.f)));
      v36 = v39 + pSkyPolygon.ptr_38->angle_from_east;

      v9 = fixpoint_mul(pSkyPolygon.v_18.z, v33 * (v30 - floor(array_50AC10[i].vWorldViewProjY + 0.5)));
      v10 = pSkyPolygon.v_18.x + v9;
      if ( v10 > 0 )
        v10 = 0;
      v13 = v33 * (pViewport->uScreenCenterX - (signed __int64)array_50AC10[i].vWorldViewProjX);
      v34 = -pSkyPolygon.field_24;
      v32 = (signed __int64)array_50AC10[i].vWorldViewProjY - 1.0;
      v14 = v33 * (v30 - v32);
      while ( 1 )
      {
        if ( v10 )
        {
          v37 = abs((int)v34 >> 14);
          v15 = abs(v10);
          if ( v37 <= v15 || v32 <= (signed int)pViewport->uViewportTL_Y )
          {
            if ( v10 <= 0 )
              break;
          }
        }
        v16 = fixpoint_mul(pSkyPolygon.v_18.z, v14);
        --v32;
        v14 += v33;
        v10 = pSkyPolygon.v_18.x + v16;
      }
      LODWORD(v17) = LODWORD(v34) << 16;
      HIDWORD(v17) = v34 >> 16;
      v18 = v17 / v10;
      if ( v18 < 0 )
        v18 = pODMRenderParams->shading_dist_mist;
      v37 = v35 + fixpoint_mul(pSkyPolygon.ptr_38->angle_from_west, v13);
      v35 = 224 * pMiscTimer->uTotalGameTimeElapsed + ((signed int)fixpoint_mul(v37, v18) >> 3);
      array_50AC10[i].u = (double)v35 / ((double)pSkyPolygon.pTexture->uTextureWidth * 65536.0);

      v36 = v36 + fixpoint_mul(pSkyPolygon.ptr_38->angle_from_south, v13);
      v35 = 224 * pMiscTimer->uTotalGameTimeElapsed + ((signed int)fixpoint_mul(v36, v18) >> 3);
      array_50AC10[i].v = (double)v35 / ((double)pSkyPolygon.pTexture->uTextureHeight * 65536.0);

      array_50AC10[i].vWorldViewPosition.x = (double)0x2000;//pODMRenderParams->shading_dist_mist 8192
      array_50AC10[i]._rhw = 1.0 / (double)(v18 >> 16);
    }
    pRenderer->DrawOutdoorSkyPolygon(pSkyPolygon.uNumVertices, &pSkyPolygon, pBitmaps_LOD->pHardwareTextures[(signed __int16)pSkyPolygon.uTileBitmapID]);
    array_50AC10[0].vWorldViewProjY = (double)v10;
    array_50AC10[1].vWorldViewProjY = array_50AC10[1].vWorldViewProjY + 30.0;
    array_50AC10[2].vWorldViewProjY = array_50AC10[2].vWorldViewProjY + 30.0;
    array_50AC10[3].vWorldViewProjY = (double)v10;
    pRenderer->DrawOutdoorSkyPolygon(pSkyPolygon.uNumVertices, &pSkyPolygon, pBitmaps_LOD->pHardwareTextures[(signed __int16)pSkyPolygon.uTileBitmapID]);
    return;
  }
}
//----- (004226C2) --------------------------------------------------------
bool PauseGameDrawing()
{
  if ( pCurrentScreen != SCREEN_GAME
    && pCurrentScreen != SCREEN_NPC_DIALOGUE
    && pCurrentScreen != SCREEN_CHANGE_LOCATION )
  {
	  if (pCurrentScreen == SCREEN_INPUT_BLV)
		  return pMovie;//pSmackerMovie != 0;
    if ( pCurrentScreen != SCREEN_BRANCHLESS_NPC_DIALOG )
      return true;
  }
  return false;
}


//----- (0045E03A) --------------------------------------------------------
unsigned short *Render::MakeScreenshot(signed int width, signed int height)
{
  unsigned __int16 *v3; // ebx@1
  int v4; // edx@7
  unsigned __int8 v5; // cf@9
  unsigned int v6; // ecx@9
  unsigned __int16 *v7; // edi@9
  int j; // ecx@9
//  unsigned int screen_x; // qax@18
//  HRESULT v14; // eax@21
//  int v15; // edi@29
//  signed __int64 v16; // qax@30
//  signed int v17; // edx@34
//  unsigned __int16 *v18; // edi@36
//  int k; // ecx@36
  DDSURFACEDESC2 Dst; // [sp+4h] [bp-A0h]@6
  unsigned __int16 *pPixels; // [sp+80h] [bp-24h]@1
  float interval_x; // [sp+84h] [bp-20h]@1
  //unsigned __int16 *_this; // [sp+88h] [bp-1Ch]@21
  float interval_y; // [sp+8Ch] [bp-18h]@1
//  unsigned int screen_y; // [sp+90h] [bp-14h]@17
//  int v28; // [sp+98h] [bp-Ch]@16
//  int v29; // [sp+9Ch] [bp-8h]@15

  interval_x = game_viewport_width / (double)width;
  interval_y = game_viewport_height / (double)height;

  pPixels = (unsigned __int16 *)malloc(2 * height * width);
  memset(pPixels, 0 , 2 * height * width);

  v3 = pPixels;

  BeginSceneD3D();

  if (uCurrentlyLoadedLevelType == LEVEL_Indoor)
    pIndoor->Draw();
  else if (uCurrentlyLoadedLevelType == LEVEL_Outdoor)
    pOutdoor->Draw();
  DrawBillboards_And_MaybeRenderSpecialEffects_And_EndScene();
  memset(&Dst, 0, 0x7C);
  Dst.dwSize = sizeof(Dst);

  if ( LockSurface_DDraw4(pBackBuffer4, &Dst, DDLOCK_WAIT) )
  {
    for (uint y = 0; y < height; ++y)
    {
      for (uint x = 0; x < width; ++x)
      {
        //*v3 = pRenderer->ReadPixel16((int)(x* interval_x + 8.0), (int)(y * interval_y + 8.0));//screen_data[screen_y + (int)(x* interval_x + 8.0)];
		  /*
        if (ddpfPrimarySuface.dwRGBBitCount == 32)
        {
          auto p = (unsigned __int32 *)pRenderer->pTargetSurface + (int)(x * interval_x + 8.0) + (int)(y * interval_y + 8.0) * pRenderer->uTargetSurfacePitch;
          *v3 = Color16((*p >> 16) & 255, (*p >> 8) & 255, *p & 255);
        }
        else if (ddpfPrimarySuface.dwRGBBitCount == 16)
        {
          auto p = (unsigned __int16 *)pRenderer->pTargetSurface + (int)(x * interval_x + 8.0) + (int)(y * interval_y + 8.0) * pRenderer->uTargetSurfacePitch;
          *v3 = *p;
        }
		  */
		  pRenderer->pTargetSurface;
        if (Dst.ddpfPixelFormat.dwRGBBitCount == 32)
        {
          auto p = (unsigned __int32 *)Dst.lpSurface + (int)(x * interval_x + 8.0) + y/* (int)(y * interval_y + 8.0)*/ * Dst.lPitch;
          *v3 = Color16((*p >> 16) & 255, (*p >> 8) & 255, *p & 255);
        }
        else if (Dst.ddpfPixelFormat.dwRGBBitCount == 16)
        {
          auto p = (unsigned __int16 *)Dst.lpSurface + (int)(x * interval_x + 8.0) + y * Dst.lPitch;
          *v3 = *p;
        }
        else __debugbreak();
        ++v3;
      }
    }
    ErrD3D(pBackBuffer4->Unlock(0));
  }
  else
  {
    __debugbreak(); // unrefactored - black screenshot
    v4 = height;
    if ( height > 0 )
    {
      do
      {
        if ( width > 0 )
        {
          v5 = width & 1;
          v6 = (unsigned int)width / 2;
          memset(v3, 0, 4 * ((unsigned int)width / 2));
          v7 = &v3[2 * v6];
          for ( j = v5; j; --j )
          {
            *v7 = 0;
            ++v7;
          }
          v3 += width;
        }
        --v4;
      }
      while ( v4 );
    }
  }

  /*if (!pRenderer->pRenderD3D)
  {
    pRenderer->BeginScene();
    if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
    {
      pIndoor->Draw();
    }
    else
    {
      if ( uCurrentlyLoadedLevelType == LEVEL_Outdoor )
        pOutdoor->Draw();
    }
    _this = pRenderer->pTargetSurface;
    v26 = pRenderer->uTargetSurfacePitch;
    if ( pRenderer->pTargetSurface )
    {
      if ( height > 0 )
      {
        for ( v29 = 0; v29 < height; ++v29 )
        {
          if ( width > 0 )
          {
            v15 = v26 * (unsigned __int64)(signed __int64)((double)v29 * v25 + 8.0);
            for ( v28 = 0; v28 < width; v28++ )
            {
              *v3 = _this[v15 + (int)(signed __int64)((double)v28 * v23 + 8.0)];
              ++v3;
            }
          }
        }
      }
    }
    else
    {
      if ( height > 0 )
      {
        for ( v17 = height; v17; --v17 )
        {
          if ( width > 0 )
          {
            memset(v3, 0, 4 * ((unsigned int)width >> 1));
            v18 = &v3[2 * ((unsigned int)width >> 1)];
            for ( k = width & 1; k; --k )
            {
              *v18 = 0;
              ++v18;
            }
            v3 += width;
          }
        }
      }
    }
    pRenderer->EndScene();
  }*/
  return pPixels;
}
//----- (0045E26C) --------------------------------------------------------
void Render::SaveScreenshot(const char *pFilename, unsigned int width, unsigned int height)
{
  auto pixels = pRenderer->MakeScreenshot(width, height);
  pRenderer->SavePCXImage(pFilename, pixels, width, height);
  free(pixels);
}

void Render::PackScreenshot(unsigned int width, unsigned int height, void *data, unsigned int data_size, unsigned int *out_screenshot_size)
{
  auto pixels = pRenderer->MakeScreenshot(150, 112);
  pRenderer->PackPCXpicture(pixels, 150, 112, data, 1000000, out_screenshot_size);
  free(pixels);
}


//----- (0046A7C8) --------------------------------------------------------
int Render::_46А6АС_GetActorsInViewport(int pDepth)
{
  unsigned int v3; // eax@2 применяется в закле Жар печи для подсчёта кол-ва монстров видимых группе и заполнения массива id видимых монстров
  unsigned int v5; // eax@2
  unsigned int v6; // eax@4
  unsigned int v12; // [sp+10h] [bp-14h]@1
  int mon_num; // [sp+1Ch] [bp-8h]@1
  unsigned int a1a; // [sp+20h] [bp-4h]@1

  mon_num = 0;
  v12 = GetBillboardDrawListSize();
  if ( (signed int)GetBillboardDrawListSize() > 0 )
  {
    for ( a1a = 0; (signed int)a1a < (signed int)v12; ++a1a )
    {
      v3 = GetParentBillboardID(a1a);
      v5 = (unsigned __int16)pBillboardRenderList[v3].object_pid;
      if ( PID_TYPE(v5) == OBJECT_Actor)
      {
        if ( pBillboardRenderList[v3].sZValue <= (unsigned int)(pDepth << 16) )
        {
          v6 = PID_ID(v5);
          if ( pActors[v6].uAIState != Dead && pActors[v6].uAIState != Dying && pActors[v6].uAIState != Removed
            && pActors[v6].uAIState != Disabled && pActors[v6].uAIState != Summoned )
          {
            if ( pGame->pVisInstance->DoesRayIntersectBillboard((double)pDepth, a1a) )
            {
              if ( mon_num < 100 )
              {
                _50BF30_actors_in_viewport_ids[mon_num] = v6;
                mon_num++;
              }
            }
          }
        }
      }
    }
  }
  return mon_num;
}




void Render::BeginLightmaps()
{
  ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_CLAMP));

  if (bUsingSpecular)
    pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE);

  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE));
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DITHERENABLE, FALSE));
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetTexture(0, pGame->pIndoorCameraD3D->LoadTextureAndGetHardwarePtr("effpar03")));
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE));
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE));
}

void Render::EndLightmaps()
{
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ZERO));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DITHERENABLE, TRUE));

  if (bUsingSpecular)
  {
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, TRUE));
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_FOGCOLOR, uFogColor));
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_FOGTABLEMODE, 0));
  }
}


void Render::BeginLightmaps2()
{
  if (bUsingSpecular)
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE));
 
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DITHERENABLE, FALSE));

  ErrD3D(pRenderer->pRenderD3D->pDevice->SetTexture(0, pGame->pIndoorCameraD3D->LoadTextureAndGetHardwarePtr("effpar03")));

  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE));
  ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE));
}


void Render::EndLightmaps2()
{
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ZERO));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DITHERENABLE, TRUE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CW));

  if (bUsingSpecular)
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, TRUE));
}



//----- (00437C96) --------------------------------------------------------
void Render::do_draw_debug_line_d3d(const RenderVertexD3D3 *pLineBegin, signed int sDiffuseBegin, const RenderVertexD3D3 *pLineEnd, signed int sDiffuseEnd, float z_stuff)
{
  double v6; // st7@2
  //IDirect3DDevice3 *v7; // eax@2
//  HRESULT v8; // eax@2
  std::string v9; // [sp-18h] [bp-60h]@3
//  const char *v10; // [sp-Ch] [bp-54h]@2
//  const char *v11; // [sp-8h] [bp-50h]@2
//  int v12; // [sp-4h] [bp-4Ch]@2
  RenderVertexD3D3 v13[2]; // [sp+8h] [bp-40h]@2

  //if ( pRenderer->pRenderD3D )
  {
    v6 = 0.001 - z_stuff;
    memcpy(v13, pLineBegin, 0x20u);
    memcpy(&v13[1], pLineEnd, sizeof(v13[1]));
    v13[0].pos.z = v6;
    v13[1].pos.z = v6;
    v13[0].diffuse = sDiffuseBegin;
    v13[1].diffuse = sDiffuseEnd;
    ErrD3D(pRenderD3D->pDevice->SetTexture(0, nullptr));
    ErrD3D(pRenderD3D->pDevice->DrawPrimitive(
           D3DPT_LINELIST,
           452,
           v13,
           2,
           16));
  }
}


void Render::DrawLines(const RenderVertexD3D3 *vertices, unsigned int num_vertices)
{
  ErrD3D(pRenderD3D->pDevice->SetTexture(0, nullptr));
  ErrD3D(pRenderD3D->pDevice->DrawPrimitive(D3DPT_LINELIST,
              D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1,
              (void *)vertices,
              num_vertices,
              D3DDP_DONOTLIGHT));
}


void Render::DrawFansTransparent(const RenderVertexD3D3 *vertices, unsigned int num_vertices)
{
  //ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, false));
  //ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZENABLE, false));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA));
  
  ErrD3D(pRenderD3D->pDevice->SetTexture(0, nullptr));
  ErrD3D(pRenderD3D->pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,
                D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1,
                (void *)vertices,
                num_vertices,
                28));

  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ZERO));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE));
  //ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZENABLE, TRUE));
  //ErrD3D(pRenderer->pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE));
}


void Render::BeginDecals()
{
  // code chunk from 0049C304
  if (bUsingSpecular)
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE));
  ErrD3D(pRenderD3D->pDevice->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_CLAMP));

  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DITHERENABLE, FALSE));

  ErrD3D(pRenderD3D->pDevice->SetTexture(0, pGame->pIndoorCameraD3D->LoadTextureAndGetHardwarePtr("hwsplat04")));
}


void Render::EndDecals()
{
  // code chunk from 0049C304
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CW));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ZERO));

  if (bUsingSpecular)
    ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, TRUE));
}



//----- (0049C095) --------------------------------------------------------
void Render::DrawDecal(Decal *pDecal, float z_bias)
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
    uint uTint = Render::GetActorTintColor(pDecal->pVertices[i].vWorldViewPosition.x, pDecal->field_C14, 0, 0, nullptr);

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

  ErrD3D(pRenderD3D->pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,
            D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1,
            pVerticesD3D,
            pDecal->uNumVertices,
            v21));
}


void Render::DrawSpecialEffectsQuad(const RenderVertexD3D3 *vertices, IDirect3DTexture2 *texture)
{
  ErrD3D(pRenderD3D->pDevice->SetTexture(0, texture));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DITHERENABLE, FALSE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZFUNC, D3DCMP_ALWAYS));
  ErrD3D(pRenderD3D->pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,
                                     D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1,
                                     (void *)vertices, 4, 28));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ZERO));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_DITHERENABLE, TRUE));
  ErrD3D(pRenderD3D->pDevice->SetRenderState(D3DRENDERSTATE_ZFUNC, D3DCMP_LESS));
}