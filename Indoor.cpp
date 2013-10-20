#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "LightmapBuilder.h"
#include "DecalBuilder.h"
#include "stru9.h"
#include "stru10.h"
#include "stru367.h"

#include "BSPModel.h"
#include "Outdoor.h"
#include "SpriteObject.h"
#include "Events.h"
#include "Game.h"
#include "Viewport.h"
#include "Time.h"
#include "Party.h"
#include "Math.h"
#include "LOD.h"
#include "DecorationList.h"
#include "ObjectList.h"
#include "Actor.h"
#include "Chest.h"
#include "GUIProgressBar.h"
#include "stru123.h"
#include "AudioPlayer.h"
#include "Log.h"
#include "TurnEngine.h"
#include "PaletteManager.h"
#include "MapInfo.h"
#include "Lights.h"

#include "mm7_data.h"
#include "MM7.h"
#include "Sprites.h"
#include "Game.h"
#include "stru6.h"
#include "ParticleEngine.h"
#include "Outdoor_stuff.h"
#include "texts.h"
#include "GUIWindow.h"
#include "Level/Decoration.h"





IndoorLocation *pIndoor = new IndoorLocation;
BLVRenderParams *pBLVRenderParams = new BLVRenderParams;

struct DecorationList *pDecorationList;

std::array<LevelDecoration, 3000> pLevelDecorations;
size_t uNumLevelDecorations;
LevelDecoration* activeLevelDecoration;

LEVEL_TYPE uCurrentlyLoadedLevelType = LEVEL_null;

stru320 stru_F8AD28; // idb
stru337 stru_F81018;
BspRenderer_PortalViewportData stru_F8A590;
BspRenderer *pBspRenderer = new BspRenderer; // idb
stru141_actor_collision_object stru_721530;
std::array<stru352, 480> stru_F83B80;



unsigned __int16 pDoorSoundIDsByLocationID[78] =
{
  300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300,
  300, 300, 300, 404, 302, 306, 308, 304, 308, 302, 400, 302, 300,
  308, 308, 306, 308, 308, 304, 300, 404, 406, 300, 400, 406, 404,
  306, 302, 408, 304, 300, 300, 300, 300, 300, 300, 300, 300, 300,
  300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 404, 304,
  400, 300, 300, 404, 304, 400, 300, 300, 404, 304, 400, 300, 300
};


std::array<const char *, 11> _4E6BDC_loc_names=
    {
    "mdt12.blv",
    "d18.blv",
    "mdt14.blv",
    "d37.blv",
    "mdk01.blv",
    "mdt01.blv",
    "mdr01.blv",
    "mdt10.blv",
    "mdt09.blv",
    "mdt15.blv",
    "mdt11.blv"
    };


//----- (0043F39E) --------------------------------------------------------
void __fastcall PrepareDrawLists_BLV()
{
  //int *v1; // ecx@1
  //double v2; // ST30_8@3
  //double v3; // ST30_8@6
  //double v4; // ST28_8@6
  int v5; // eax@4
  //int v6; // eax@7
  unsigned int v7; // ebx@8
  BLVSector *v8; // esi@8
  //unsigned __int16 *v9; // edi@8
  //int i; // [sp+18h] [bp-8h]@7
  //unsigned __int8 v11; // [sp+1Ch] [bp-4h]@3
  //signed int v12; // [sp+1Ch] [bp-4h]@8

  pBLVRenderParams->Reset();
  pMobileLightsStack->uNumLightsActive = 0;
  //uNumMobileLightsApplied = 0;
  uNumDecorationsDrawnThisFrame = 0;
  uNumSpritesDrawnThisFrame = 0;
  uNumBillboardsToDraw = 0;

  if ( !byte_4D864C || !(pGame->uFlags & 0x1000) )  // lightspot around party
  {
    //v2 = pParty->flt_TorchlightColorB + 6.7553994e15;
    //v11 = LOBYTE(v2);
    v5 = 800;
    if (pParty->TorchlightActive())
      v5 *= pParty->pPartyBuffs[PARTY_BUFF_TORCHLIGHT].uPower;
    //LOBYTE(v1) = byte_4E94D0;
    //v4 = pParty->flt_TorchlightColorR + 6.7553994e15;
    //v3 = pParty->flt_TorchlightColorG + 6.7553994e15;
    pMobileLightsStack->AddLight(pGame->pIndoorCameraD3D->vPartyPos.x,
                                 pGame->pIndoorCameraD3D->vPartyPos.y,
                                 pGame->pIndoorCameraD3D->vPartyPos.z,
                                 pBLVRenderParams->uPartySectorID,
                                 v5,
                                 floorf(pParty->flt_TorchlightColorR + 0.5f),
                                 floorf(pParty->flt_TorchlightColorG + 0.5f),
                                 floorf(pParty->flt_TorchlightColorB + 0.5f),
                                 byte_4E94D0);
  }
  PrepareBspRenderList_BLV();
  PrepareItemsRenderList_BLV();
  PrepareActorRenderList_BLV();

  //v6 = 0;
  for (uint i = 0; i < pBspRenderer->uNumVisibleNotEmptySectors; ++i)
  {
    v7 = pBspRenderer->pVisibleSectorIDs_toDrawDecorsActorsEtcFrom[i];
    //v12 = 0;
    v8 = &pIndoor->pSectors[pBspRenderer->pVisibleSectorIDs_toDrawDecorsActorsEtcFrom[i]];
    //v9 = v8->pDecorationIDs;

    for (uint j = 0; j < v8->uNumDecorations; ++j)
    //if ( v8->uNumDecorations > 0 )
    {
    //  do
      PrepareDecorationsRenderList_BLV(v8->pDecorationIDs[j], v7);
    //  while ( v12 < v8->uNumDecorations );
    }
    //v6 = i + 1;
  }
  FindBillboardsLightLevels_BLV();
  pGame->PrepareBloodsplats();
}



//----- (004407D9) --------------------------------------------------------
void BLVRenderParams::Reset()
{
  //IndoorLocation_drawstru *v2; // ebx@1
  //int v4; // ST08_4@1
  //int v5; // ST04_4@1
  //int v6; // ST00_4@1
  int v7; // eax@1
  int v8; // ST08_4@2
  int v9; // ST04_4@2
  int v10; // ST00_4@2
  //unsigned int v11; // edi@4
  //unsigned int v12; // ecx@4
  //int v13; // edx@4
  //signed int v14; // ecx@4
  //unsigned int v15; // edx@4
  //unsigned int v16; // eax@4
  double v17; // st7@5
  int v18; // eax@5
  double v19; // st7@5
  int v20; // eax@5
  double v21; // st7@5
  int v22; // eax@5
  unsigned int v23; // edx@5
  unsigned int v24; // ecx@5
  int v25; // eax@5
  int v26; // eax@5
  signed int v27; // eax@6
  //int result; // eax@6
  int v29; // [sp+24h] [bp+8h]@5

  this->field_0_timer_ = pEventTimer->uTotalGameTimeElapsed;

  pGame->pIndoorCameraD3D->debug_flags = 0;
  if (viewparams->draw_sw_outlines)
    pGame->pIndoorCameraD3D->debug_flags |= BLV_RENDER_DRAW_SW_OUTLINES;
  if (viewparams->draw_d3d_outlines)
    pGame->pIndoorCameraD3D->debug_flags |= BLV_RENDER_DRAW_D3D_OUTLINES;

  //v2 = a2;
  //this->field_0_timer_ = a2->field_0_timer;
  //this->uFlags = a2->uFlags;
  //this->vPartyPos.x = a2->vPosition.x;
  //this->vPartyPos.y = a2->vPosition.y;
  //this->vPartyPos.z = a2->vPosition.z;
  //v4 = this->vPartyPos.z;
  //v5 = this->vPartyPos.y;
  //this->sPartyRotY = a2->sRotationY;
  //v6 = this->vPartyPos.x;
  //this->sPartyRotX = a2->sRotationX;
  v7 = pIndoor->GetSector(pGame->pIndoorCameraD3D->vPartyPos.x,
                          pGame->pIndoorCameraD3D->vPartyPos.y,
                          pGame->pIndoorCameraD3D->vPartyPos.z);
  this->uPartySectorID = v7;
  if ( !v7 )
  {
    __debugbreak(); // shouldnt happen, please provide savegame
    /*v8 = this->vPartyPos.z;
    this->vPartyPos.x = pParty->vPosition.x;
    v9 = pParty->vPosition.y;
    v10 = this->vPartyPos.x;
    this->vPartyPos.y = pParty->vPosition.y;*/
    this->uPartySectorID = pIndoor->GetSector(v10, v9, v8);
  }
  if ( pRenderer->pRenderD3D )
  {
    //this->sCosineY = stru_5C6E00->Cos(pGame->pIndoorCameraD3D->sRotationY);
    //this->sSineY = stru_5C6E00->Sin(pGame->pIndoorCameraD3D->sRotationY);
    //this->sCosineNegX = stru_5C6E00->Cos(-pGame->pIndoorCameraD3D->sRotationX);
    //this->sSineNegX = stru_5C6E00->Sin(-pGame->pIndoorCameraD3D->sRotationX);
    //this->fCosineY = cos((3.141592653589793 + 3.141592653589793) * (double)pGame->pIndoorCameraD3D->sRotationY * 0.00048828125);
    //this->fSineY = sin((3.141592653589793 + 3.141592653589793) * (double)pGame->pIndoorCameraD3D->sRotationY * 0.00048828125);
    //this->fCosineNegX = cos((3.141592653589793 + 3.141592653589793) * (double)-pGame->pIndoorCameraD3D->sRotationX * 0.00048828125);
    //this->fSineNegX = sin((3.141592653589793 + 3.141592653589793) * (double)-pGame->pIndoorCameraD3D->sRotationX * 0.00048828125);
    this->field_64 = pViewport->field_30;
    
    this->uViewportX = pViewport->uScreen_TL_X;
    this->uViewportY = pViewport->uScreen_TL_Y;
    this->uViewportZ = pViewport->uScreen_BR_X;
    this->uViewportW = pViewport->uScreen_BR_Y;

    this->uViewportWidth = uViewportZ - uViewportX + 1;
    this->uViewportHeight = uViewportW - uViewportY + 1;
    this->uViewportCenterX = (uViewportZ + uViewportX) / 2;
    this->uViewportCenterY = (uViewportY + uViewportW) / 2;
  }
  else
  {
    __debugbreak(); // no sw
    /*this->sCosineY = stru_5C6E00->Cos(-this->sPartyRotY);
    this->sSineY = stru_5C6E00->Sin(-this->sPartyRotY);
    this->sCosineNegX = stru_5C6E00->Cos(-this->sPartyRotX);
    this->sSineNegX = stru_5C6E00->Sin(-this->sPartyRotX);
    v17 = cos((double)-this->sPartyRotY * 0.0030664064);
    v18 = this->sPartyRotY;
    this->fCosineY = v17;
    v19 = sin((double)-v18 * 0.0030664064);
    v20 = this->sPartyRotX;
    this->fSineY = v19;
    v21 = cos((double)-v20 * 0.0030664064);
    v22 = this->sPartyRotX;
    this->fCosineNegX = v21;
    this->fSineNegX = sin((double)-v22 * 0.0030664064);
    v23 = this->uViewportX;
    this->field_64 = a2->field_3C;
    v24 = this->uViewportZ;
    this->field_70 = this->uViewportZ - v23 + 1;
    v25 = this->uViewportW - this->uViewportY + 1;
    this->uViewportHeight = v25;
    v29 = v25;
    v26 = this->field_64;
    this->uViewportCenterX = (signed int)(v24 + v23) >> 1;
    this->uViewportCenterY = this->uViewportW - ((unsigned __int64)(v26 * (signed __int64)v29) >> 16);*/
  }
  //v27 = (unsigned int)(signed __int64)((double)this->uViewportWidth * 0.5
  //                                             / tan((double)(v2->fov_deg >> 1) * 0.01745329)
  //                                             + 0.5) << 16;
  extern float _calc_fov(int viewport_width, int angle_degree);
  this->fov_rad_fixpoint = fixpoint_from_int(_calc_fov(uViewportWidth, 65), 0);
  this->fov_rad_inv_fixpoint = 0x100000000i64 / this->fov_rad_fixpoint;
  this->pRenderTarget = pRenderer->pTargetSurface;
  this->uTargetWidth = 640;
  this->uTargetHeight = 480;
  this->pTargetZBuffer = pRenderer->pActiveZBuffer;
  this->field_8C = 0;
  this->field_84 = 0;
  this->uNumFacesRenderedThisFrame = 0;
  this->field_88 = 0;
  pBLVRenderParams->field_90 = 64;
  pBLVRenderParams->field_94 = 6;
}

//----- (00440B44) --------------------------------------------------------
void IndoorLocation::ExecDraw(bool bD3D)
{
  int v2; // eax@3
  //IndoorCameraD3D_Vec4 *v3; // edx@4
  //unsigned int v5; // ecx@9
  //RenderVertexSoft *v6; // [sp-4h] [bp-8h]@4

  if (bD3D)
  {
    pIndoor->GetSector(pParty->vPosition.x, pParty->vPosition.y, pParty->vPosition.z);
    for (uint i = 0; i < pBspRenderer->num_faces; ++i)
    {
      //v2 = pBspRenderer->faces[i].uNodeID;
      if (pBspRenderer->nodes[pBspRenderer->faces[i].uNodeID].viewing_portal_id == -1)
        IndoorLocation::ExecDraw_d3d(pBspRenderer->faces[i].uFaceID, nullptr, 4, nullptr);
      else
        IndoorLocation::ExecDraw_d3d(pBspRenderer->faces[i].uFaceID, pBspRenderer->nodes[pBspRenderer->faces[i].uNodeID].std__vector_0007AC, 4, pBspRenderer->nodes[pBspRenderer->faces[i].uNodeID].pPortalBounding);
    }
  }
  else for (uint j = 0; j < pBspRenderer->num_faces; ++j )
  {
    __debugbreak(); // no SW
    //pBLVRenderParams->field_7C = &pBspRenderer->nodes[pBspRenderer->faces[j].uNodeID].PortalScreenData;
    //IndoorLocation::ExecDraw_sw(pBspRenderer->faces[j].uFaceID);
  }
}


/*
//----- (00440BED) --------------------------------------------------------
void __fastcall sub_440BED(IndoorLocation_drawstru *_this)
{
  unsigned __int16 *v1; // edi@7
  BspRenderer_stru0 *v2; // esi@8
  int v3; // ecx@9
  unsigned int v4; // edx@9
  short *v5; // eax@10
  signed int v6; // [sp+8h] [bp-8h]@7
  int v7; // [sp+Ch] [bp-4h]@8
  short *v8;

  PrepareDrawLists_BLV(_this);
  if (pBLVRenderParams->uPartySectorID)
    IndoorLocation::ExecDraw(pRenderer->pRenderD3D != 0);
  pRenderer->DrawBillboardList_BLV();

  if ( !pRenderer->pRenderD3D )
  {
    if (pBLVRenderParams->uFlags & INDOOR_CAMERA_DRAW_D3D_OUTLINES)
      pBspRenderer->DrawFaceOutlines();
    if (pBLVRenderParams->uFlags & INDOOR_CAMERA_DRAW_SW_OUTLINES)
    {
      v1 = pBLVRenderParams->pRenderTarget;
	  v7 = 0;
	  for(int i=0; i < pBspRenderer->num_nodes; i++)
	  {		 
		  BspRenderer_stru0 *pNode = &pBspRenderer->nodes[i];
		  v4 = pRenderer->uTargetSurfacePitch * pNode->PortalScreenData._viewport_space_y;
		  if ( pNode->PortalScreenData._viewport_space_y <= pNode->PortalScreenData._viewport_space_w )
		  {
			//v5 = (char *)&pBspRenderer->nodes[0].field_C.array_3D8[pNode->field_C._viewport_space_y + v7];
			v5 = &pNode->PortalScreenData.viewport_right_side[pNode->PortalScreenData._viewport_space_y];
			v8 = &pNode->PortalScreenData.viewport_left_side[pNode->PortalScreenData._viewport_space_y];
			do
			{
				v1[v4 + *v8] = 255;
				++pNode->PortalScreenData._viewport_space_y;
				v1[v4 + *v5] = 255;
				v4 += pRenderer->uTargetSurfacePitch;
				++v5;
				++v8;
			}
			while ( pNode->PortalScreenData._viewport_space_y <= pNode->PortalScreenData._viewport_space_w );
		  }
	  }
    }
  }
}
*/




//----- (00441BD4) --------------------------------------------------------
void IndoorLocation::Draw()
{
  //int v0; // eax@1
  //IndoorLocation_drawstru _this; // [sp+0h] [bp-4Ch]@5
  int v2; // [sp+44h] [bp-8h]@5
  int v3; // [sp+48h] [bp-4h]@5

  /*_this.uFlags = 0;
  if (viewparams->draw_sw_outlines)
    _this.uFlags |= BLV_RENDER_DRAW_SW_OUTLINES;
  if (viewparams->draw_d3d_outlines)
    _this.uFlags |= BLV_RENDER_DRAW_D3D_OUTLINES;
  
  _this.uFlags |= BLV_RENDER_DRAW_SW_OUTLINES;
  _this.uFlags |= BLV_RENDER_DRAW_D3D_OUTLINES;

  _this.field_0_timer = pEventTimer->uTotalGameTimeElapsed;
  //_this.fov_deg = 65;
  //_this.vPosition.x = pParty->vPosition.x - fixpoint_mul(stru_5C6E00->Cos(pParty->sRotationY), pParty->y_rotation_granularity);
  //_this.vPosition.y = pParty->vPosition.y - fixpoint_mul(stru_5C6E00->Sin(pParty->sRotationY), pParty->y_rotation_granularity);
  //_this.vPosition.z = pParty->vPosition.z + pParty->sEyelevel;
  //_this.sRotationX = pParty->sRotationX;
  //_this.sRotationY = pParty->sRotationY;
  _this.pRenderTarget = pRenderer->pTargetSurface;
  _this.uViewportX = pViewport->uScreen_TL_X;
  _this.uViewportY = pViewport->uScreen_TL_Y;
  _this.uViewportZ = pViewport->uScreen_BR_X;
  _this.uViewportW = pViewport->uScreen_BR_Y;
  _this.field_3C = pViewport->field_30;

  _this.uTargetWidth = 640;
  _this.uTargetHeight = 480;
  _this.pTargetZ = pRenderer->pActiveZBuffer;*/

  //sub_440BED(&_this); -- inlined
  {
    PrepareDrawLists_BLV();
    if (pBLVRenderParams->uPartySectorID)
      IndoorLocation::ExecDraw(pRenderer->pRenderD3D != 0);
    pRenderer->DrawBillboardList_BLV();
  }

  pParty->uFlags &= ~2;
  pGame->DrawParticles();
  trail_particle_generator.UpdateParticles();
}

//----- (004C0EF2) --------------------------------------------------------
void BLVFace::FromODM(ODMFace *face)
{
  this->pFacePlane_old.vNormal.x = face->pFacePlane.vNormal.x;
  this->pFacePlane_old.vNormal.y = face->pFacePlane.vNormal.y;
  this->pFacePlane_old.vNormal.z = face->pFacePlane.vNormal.z;
  this->pFacePlane_old.dist = face->pFacePlane.dist;
  this->pFacePlane.vNormal.x = (double)(face->pFacePlane.vNormal.x & 0xFFFF) * 0.000015259022
                             + (double)(face->pFacePlane.vNormal.x >> 16);
  this->pFacePlane.vNormal.y = (double)(face->pFacePlane.vNormal.y & 0xFFFF) * 0.000015259022
                             + (double)(face->pFacePlane.vNormal.y >> 16);
  this->pFacePlane.vNormal.z = (double)(face->pFacePlane.vNormal.z & 0xFFFF) * 0.000015259022
                             + (double)(face->pFacePlane.vNormal.z >> 16);
  this->pFacePlane.dist = (double)(face->pFacePlane.dist & 0xFFFF) * 0.000015259022 + (double)(face->pFacePlane.dist >> 16);
  this->uAttributes = face->uAttributes;
  this->pBounding.x1 = face->pBoundingBox.x1;
  this->pBounding.y1 = face->pBoundingBox.y1;
  this->pBounding.z1 = face->pBoundingBox.z1;
  this->pBounding.x2 = face->pBoundingBox.x2;
  this->pBounding.y2 = face->pBoundingBox.y2;
  this->pBounding.z2 = face->pBoundingBox.z2;
  this->zCalc1 = face->zCalc1;
  this->zCalc2 = face->zCalc2;
  this->zCalc3 = face->zCalc3;
  this->pXInterceptDisplacements = face->pXInterceptDisplacements;
  this->pYInterceptDisplacements = face->pYInterceptDisplacements;
  this->pZInterceptDisplacements = face->pZInterceptDisplacements;
  this->uPolygonType = (PolygonType)face->uPolygonType;
  this->uNumVertices = face->uNumVertices;
  this->uBitmapID = face->uTextureID;
  this->pVertexIDs = face->pVertexIDs;
}

//----- (004B0A25) --------------------------------------------------------
void IndoorLocation::ExecDraw_d3d(unsigned int uFaceID, IndoorCameraD3D_Vec4 *pVertices, unsigned int uNumVertices, RenderVertexSoft *pPortalBounding)
{
  //unsigned int v4; // esi@1
  char *v5; // eax@4
  signed int v6; // ecx@4
  char *v7; // eax@8
  signed int v8; // ecx@8
  //BLVFace *v9; // esi@13
  //IndoorCameraD3D *v10; // edi@16
  //int v11; // ebx@17
  //Vec3_short_ *v12; // ecx@18
  //char *v13; // edx@18
  //int v14; // eax@19
  //unsigned __int8 v15; // sf@19
  //unsigned __int8 v16; // of@19
  int v17; // ebx@25
  //double v18; // st7@27
  //double v19; // st6@27
  //double v20; // st5@27
  //char v21; // dl@27
  //unsigned int v22; // eax@44
  unsigned int v23; // eax@35
  //DWORD v24; // eax@37
  //int v25; // eax@38
  //char *v26; // edi@38
  IDirect3DTexture2 *v27; // eax@42
  Texture *v28; // [sp+Ch] [bp-1Ch]@15
  //int i; // [sp+10h] [bp-18h]@38
  //LightmapBuilder *pStru4; // [sp+14h] [bp-14h]@16
  //IndoorCameraD3D *v31; // [sp+18h] [bp-10h]@16
  //IndoorCameraD3D_Vec4 *a7; // [sp+1Ch] [bp-Ch]@1
  //unsigned int uFaceID_; // [sp+20h] [bp-8h]@1
  unsigned int uNumVerticesa; // [sp+24h] [bp-4h]@17
  int a4a; // [sp+34h] [bp+Ch]@25
  //unsigned int a4b; // [sp+34h] [bp+Ch]@38

  //v4 = uFaceID;
  //a7 = pVertices;
  //uFaceID_ = uFaceID;
  if (uFaceID >= pIndoor->uNumFaces)
    return;

  static RenderVertexSoft static_vertices_F7C228[64];
  static RenderVertexSoft static_vertices_F7B628[64];
  static stru154 stru_F7B60C; // idb

    //v9 = &pIndoor->pFaces[uFaceID];
  auto pFace = &pIndoor->pFaces[uFaceID];
  if (pFace->uNumVertices < 3)
    return;


  if (pFace->Invisible())
    return;
  
  ++pBLVRenderParams->uNumFacesRenderedThisFrame;
  pFace->uAttributes |= 0x80u;

  if (!pFace->GetTexture())
    return;

  v28 = pFace->GetTexture();

  if (!pGame->pIndoorCameraD3D->IsCulled(pFace))
  {
    uNumVerticesa = pFace->uNumVertices;
    for (uint i = 0; i < pFace->uNumVertices; ++i)
    {
      static_vertices_F7C228[i].vWorldPosition.x = pIndoor->pVertices[pFace->pVertexIDs[i]].x;
      static_vertices_F7C228[i].vWorldPosition.y = pIndoor->pVertices[pFace->pVertexIDs[i]].y;
      static_vertices_F7C228[i].vWorldPosition.z = pIndoor->pVertices[pFace->pVertexIDs[i]].z;
      static_vertices_F7C228[i].u = (signed short)pFace->pVertexUIDs[i];
      static_vertices_F7C228[i].v = (signed short)pFace->pVertexVIDs[i];
    }

    if (!pVertices ||
        (pGame->pStru9Instance->_498377(pPortalBounding, 4, pVertices, static_vertices_F7C228, &uNumVerticesa), uNumVerticesa) )
    {
      if (pGame->pIndoorCameraD3D->CalcPortalShape(static_vertices_F7C228, &uNumVerticesa,
                     static_vertices_F7B628, pGame->pIndoorCameraD3D->std__vector_000034_prolly_frustrum, 4, false, 0) != 1 || uNumVerticesa )
      {
        a4a = SHIWORD(stru_F8AD28.uCurrentAmbientLightLevel);
        v17 = (248 - 8 * SHIWORD(stru_F8AD28.uCurrentAmbientLightLevel)) | (((248 - 8 * SHIWORD(stru_F8AD28.uCurrentAmbientLightLevel))
            | ((248 - 8 * SHIWORD(stru_F8AD28.uCurrentAmbientLightLevel)) << 8)) << 8);
        sub_4B0E07(uFaceID);
        pGame->pLightmapBuilder->ApplyLights_IndoorFace(uFaceID);
        pDecalBuilder->ApplyBloodsplatDecals_IndoorFace(uFaceID);
        pGame->pIndoorCameraD3D->ViewTransfrom_OffsetUV(static_vertices_F7B628, uNumVerticesa, array_507D30, &stru_F8AD28);
        pGame->pIndoorCameraD3D->Project(array_507D30, uNumVerticesa, 0);
        pGame->pLightmapBuilder->std__vector_000004_size = 0;
        if (stru_F8AD28.uNumLightsApplied > 0 ||
            pDecalBuilder->uNumDecals > 0)
        {
          stru_F7B60C.face_plane.vNormal.x = pFace->pFacePlane.vNormal.x;
          stru_F7B60C.polygonType = pFace->uPolygonType;
          stru_F7B60C.face_plane.vNormal.y = pFace->pFacePlane.vNormal.y;
          stru_F7B60C.face_plane.vNormal.z = pFace->pFacePlane.vNormal.z;
          stru_F7B60C.face_plane.dist = pFace->pFacePlane.dist;
        }

        if (stru_F8AD28.uNumLightsApplied > 0 && !(pFace->uAttributes & FACE_DO_NOT_LIGHT))
          pGame->pLightmapBuilder->ApplyLights(&stru_F8AD28, &stru_F7B60C, uNumVerticesa, array_507D30, pVertices, 0);

        if (pDecalBuilder->uNumDecals > 0)
          pDecalBuilder->ApplyDecals(a4a, 1, &stru_F7B60C, uNumVerticesa, array_507D30, pVertices, 0, pFace->uSectorID);

        if (pFace->Fluid())
        {
          if (pFace->uBitmapID == pRenderer->hd_water_tile_id)
          {
            v23 = pRenderer->pHDWaterBitmapIDs[pRenderer->hd_water_current_frame];
            v27 = pBitmaps_LOD->pHardwareTextures[v23];
          }
          else
          {
            //auto v24 = GetTickCount() / 4;
            //auto v25 = v24 - stru_5C6E00->uIntegerHalfPi;
            uint eightSeconds = GetTickCount() % 8000;
            float angle = (eightSeconds / 8000.0f) * 2 * 3.1415f;

            //animte lava back and forth
            for (uint i = 0; i < uNumVerticesa; ++i)
              //array_507D30[i].v += (double)(pBitmaps_LOD->pTextures[pFace->uBitmapID].uHeightMinus1 & (unsigned int)(stru_5C6E00->SinCos(v25) >> 8));
              array_507D30[i].v += pBitmaps_LOD->pTextures[pFace->uBitmapID].uHeightMinus1 * cosf(angle);
            v23 = pFace->uBitmapID;
            v27 = pBitmaps_LOD->pHardwareTextures[v23];
          }
        }
        else if (pFace->uAttributes & 0x4000)
        {
          v23 = pTextureFrameTable->GetFrameTexture(pFace->uBitmapID, pBLVRenderParams->field_0_timer_);
          v27 = pBitmaps_LOD->pHardwareTextures[v23];
        }
        else
        {
          v17 = 0xFF808080;
          v23 = pFace->uBitmapID;
          v27 = pBitmaps_LOD->pHardwareTextures[v23];
        }

        if (pFace->uAttributes & FACE_DO_NOT_LIGHT)
          pRenderer->DrawIndoorPolygonNoLight(uNumVerticesa, uFaceID);
        else
          pRenderer->DrawIndoorPolygon(uNumVerticesa, pFace, v27, v28, PID(OBJECT_BModel, uFaceID), v17, 0);
        return;
      }
    }
  }
}




//----- (004B0E07) --------------------------------------------------------
unsigned int __fastcall sub_4B0E07(unsigned int uFaceID)
{
  BLVFace *v1; // edi@1
  BLVFaceExtra *v2; // ecx@1
  Texture *v3; // esi@1
  unsigned int v4; // eax@1
  unsigned int v5; // ecx@1
  unsigned int result; // eax@1
  unsigned int v7; // ecx@5

  v1 = &pIndoor->pFaces[uFaceID];
  v2 = &pIndoor->pFaceExtras[v1->uFaceExtraID];
  v3 = pBitmaps_LOD->GetTexture(v1->uBitmapID);
  stru_F8AD28.pDeltaUV[0] = v2->sTextureDeltaU;
  stru_F8AD28.pDeltaUV[1] = v2->sTextureDeltaV;
  v4 = GetTickCount();
  v5 = v1->uAttributes;
  result = v4 >> 3;
  if ( v5 & 4 )
  {
    stru_F8AD28.pDeltaUV[1] -= result & v3->uHeightMinus1;
  }
  else
  {
    if ( v5 & 0x20 )
      stru_F8AD28.pDeltaUV[1] += result & v3->uHeightMinus1;
  }
  v7 = v1->uAttributes;
  if ( BYTE1(v7) & 8 )
  {
    stru_F8AD28.pDeltaUV[0] -= result & v3->uWidthMinus1;
  }
  else
  {
    if ( v7 & 0x40 )
      stru_F8AD28.pDeltaUV[0] += result & v3->uWidthMinus1;
  }
  return result;
}

//----- (004B0EA8) --------------------------------------------------------
void BspRenderer::AddFaceToRenderList_d3d(unsigned int node_id, unsigned int uFaceID)
{
  //unsigned int v3; // edx@1
  //stru170 *v4; // ebx@1
  //BLVFace *v5; // eax@1
  //int v6; // ecx@2
  unsigned __int16 pTransitionSector; // ax@11
  Vec3_short_ *v8; // esi@15
  int v9; // edx@15
  //signed int v10; // eax@18
  //signed int v11; // edi@19
  //signed int v12; // ecx@19
  //signed int v13; // esi@19
  signed int v14; // edx@20
  int v15; // edx@24
  //int v16; // esi@29
  //BLVFace *v17; // edi@34
  //unsigned __int16 v18; // ax@34
  char *v19; // eax@38
  signed int v20; // ecx@38
  char *v21; // eax@42
  signed int v22; // ecx@42
  //signed int v23; // edx@45
  //char *v24; // ecx@46
  //int v25; // eax@47
  //Vec3_short_ *v26; // eax@47
  //double v27; // st7@47
  //signed int v28; // ST28_4@47
  char v29; // al@48
  //signed int v30; // eax@51
  //int v31; // eax@52
  //unsigned int v32; // eax@55
  //__int16 v33; // cx@56
  //signed int v34; // [sp+Ch] [bp-14h]@18
  //int a0; // [sp+14h] [bp-Ch]@2
  //IndoorCameraD3D *a0a; // [sp+14h] [bp-Ch]@36
  //signed int v37; // [sp+18h] [bp-8h]@19
  //stru10 *v38; // [sp+18h] [bp-8h]@36
  //BLVFace *v39; // [sp+1Ch] [bp-4h]@1

  //v3 = uFaceID;
  //v4 = this;
  //v5 = &pIndoor->pFaces[uFaceID];
  nodes[num_nodes].viewing_portal_id = -1;
  //v39 = &pIndoor->pFaces[uFaceID];

  auto pFace = &pIndoor->pFaces[uFaceID];

  if (!pFace->Portal())
  {
    if (num_faces < 1000)
    {
      faces[num_faces].uFaceID = uFaceID;
      faces[num_faces++].uNodeID = node_id;
    }
    return;
  }

  if (nodes[node_id].uFaceID == uFaceID)
    return;
  if (!node_id &&
      pGame->pIndoorCameraD3D->vPartyPos.x >= pFace->pBounding.x1 - 16 &&  // we are probably standing at the portal plane
      pGame->pIndoorCameraD3D->vPartyPos.x <= pFace->pBounding.x2 + 16 &&
      pGame->pIndoorCameraD3D->vPartyPos.y >= pFace->pBounding.y1 - 16 &&
      pGame->pIndoorCameraD3D->vPartyPos.y <= pFace->pBounding.y2 + 16 &&
      pGame->pIndoorCameraD3D->vPartyPos.z >= pFace->pBounding.z1 - 16 &&
      pGame->pIndoorCameraD3D->vPartyPos.z <= pFace->pBounding.z2 + 16 )
  {
    if ( abs(pFace->pFacePlane_old.dist + pGame->pIndoorCameraD3D->vPartyPos.x * pFace->pFacePlane_old.vNormal.x
                                        + pGame->pIndoorCameraD3D->vPartyPos.y * pFace->pFacePlane_old.vNormal.y
                                        + pGame->pIndoorCameraD3D->vPartyPos.z * pFace->pFacePlane_old.vNormal.z) <= 589824 ) // we sure are standing at the portal plane
    {
      pTransitionSector = pFace->uSectorID;
      if ( nodes[0].uSectorID == pTransitionSector )  // draw back sector
        pTransitionSector = pFace->uBackSectorID;
      nodes[num_nodes].uSectorID = pTransitionSector;
      nodes[num_nodes].uFaceID = uFaceID;
      nodes[num_nodes].uViewportX = pBLVRenderParams->uViewportX;
      nodes[num_nodes].uViewportZ = pBLVRenderParams->uViewportZ;
      nodes[num_nodes].uViewportY = pBLVRenderParams->uViewportY;
      nodes[num_nodes].uViewportW = pBLVRenderParams->uViewportW;
      nodes[num_nodes].PortalScreenData.GetViewportData(pBLVRenderParams->uViewportX, pBLVRenderParams->uViewportY,
                                       pBLVRenderParams->uViewportZ, pBLVRenderParams->uViewportW);
      AddBspNodeToRenderList(++num_nodes - 1);
      return;
    }
  }

  v9 = pFace->pFacePlane_old.vNormal.x * (pIndoor->pVertices[pFace->pVertexIDs[0]].x - pGame->pIndoorCameraD3D->vPartyPos.x)
     + pFace->pFacePlane_old.vNormal.y * (pIndoor->pVertices[pFace->pVertexIDs[0]].y - pGame->pIndoorCameraD3D->vPartyPos.y)
     + pFace->pFacePlane_old.vNormal.z * (pIndoor->pVertices[pFace->pVertexIDs[0]].z - pGame->pIndoorCameraD3D->vPartyPos.z);
  if (nodes[node_id].uSectorID != pFace->uSectorID)
    v9 = -v9;
  if (v9 >= 0)
    return;

  auto num_vertices = GetPortalScreenCoord(uFaceID);
  if (num_vertices < 2)
    return;

  auto face_min_screenspace_x = PortalFace._screen_space_x[0],
       face_max_screenspace_x = PortalFace._screen_space_x[0];
  auto face_min_screenspace_y = PortalFace._screen_space_y[0],
       face_max_screenspace_y = PortalFace._screen_space_y[0];
  for (uint i = 1; i < num_vertices; ++i)
  {
    if (face_min_screenspace_x > PortalFace._screen_space_x[i])
      face_min_screenspace_x = PortalFace._screen_space_x[i];
    if (face_max_screenspace_x < PortalFace._screen_space_x[i])
      face_max_screenspace_x = PortalFace._screen_space_x[i];

    if (face_min_screenspace_y > PortalFace._screen_space_y[i])
      face_min_screenspace_y = PortalFace._screen_space_y[i];
    if (face_max_screenspace_y < PortalFace._screen_space_y[i])
      face_max_screenspace_y = PortalFace._screen_space_y[i];
  }
	  //_screen_space_x = 719, 568, 493
	  //savegame: qw , 0Bh and 0x1D4h
	  //problem here when standing near/on portal, condition is false because of face_min_screenspace_x > p->uViewportZ
  if (face_max_screenspace_x >= nodes[node_id].uViewportX &&
      face_min_screenspace_x <= nodes[node_id].uViewportZ &&
      face_max_screenspace_y >= nodes[node_id].uViewportY &&
      face_min_screenspace_y <= nodes[node_id].uViewportW &&
      PortalFrustrum(num_vertices, &nodes[num_nodes].PortalScreenData, &nodes[node_id].PortalScreenData, uFaceID))
  {
    pTransitionSector = pFace->uSectorID;
    if (nodes[node_id].uSectorID == pTransitionSector )
      pTransitionSector = pFace->uBackSectorID;
    nodes[num_nodes].uSectorID = pTransitionSector;
    nodes[num_nodes].uFaceID = uFaceID;
    nodes[num_nodes].uViewportX = pBLVRenderParams->uViewportX;
    nodes[num_nodes].uViewportZ = pBLVRenderParams->uViewportZ;
    nodes[num_nodes].uViewportY = pBLVRenderParams->uViewportY;
    nodes[num_nodes].uViewportW = pBLVRenderParams->uViewportW;
    v29 = false;
    if (nodes[node_id].viewing_portal_id == -1)//первый портал на который я смотрю
      v29 = pGame->pStru10Instance->CalcPortalShape(pFace, nodes[num_nodes].std__vector_0007AC, nodes[num_nodes].pPortalBounding);
    else//следующие порталы в портале
    {
      static RenderVertexSoft static_subAddFaceToRenderList_d3d_stru_F7AA08[64];
      static RenderVertexSoft static_subAddFaceToRenderList_d3d_stru_F79E08[64];

      for (uint k = 0; k < pFace->uNumVertices; ++k)
      {
        static_subAddFaceToRenderList_d3d_stru_F7AA08[k].vWorldPosition.x = pIndoor->pVertices[pFace->pVertexIDs[k]].x;
        static_subAddFaceToRenderList_d3d_stru_F7AA08[k].vWorldPosition.y = pIndoor->pVertices[pFace->pVertexIDs[k]].y;
        static_subAddFaceToRenderList_d3d_stru_F7AA08[k].vWorldPosition.z = pIndoor->pVertices[pFace->pVertexIDs[k]].z;
      }

      unsigned int pNewNumVertices = pFace->uNumVertices;
      pGame->pIndoorCameraD3D->CalcPortalShape(static_subAddFaceToRenderList_d3d_stru_F7AA08, &pNewNumVertices,
                                               static_subAddFaceToRenderList_d3d_stru_F79E08, nodes[node_id].std__vector_0007AC, 4, 0, 0);

      v29 = pGame->pStru10Instance->_49C5DA(pFace, static_subAddFaceToRenderList_d3d_stru_F79E08, &pNewNumVertices,
                                            nodes[num_nodes].std__vector_0007AC, nodes[num_nodes].pPortalBounding);
    }
    if ( 1 )
    {
      assert(num_nodes < 150);

      nodes[num_nodes].viewing_portal_id = uFaceID;
      AddBspNodeToRenderList(++num_nodes - 1);
    }
    if (pGame->pIndoorCameraD3D->debug_flags & BLV_RENDER_DRAW_SW_OUTLINES)
      pGame->pIndoorCameraD3D->PrepareAndDrawDebugOutline(pFace, 0x1E1EFF);
    //pGame->pIndoorCameraD3D->DebugDrawPortal(pFace);
  }
}


//----- (004AFB86) --------------------------------------------------------
void BspRenderer::AddFaceToRenderList_sw(unsigned int node_id, unsigned int uFaceID)
{
  BspRenderer *v3; // ebx@1
  BLVFace *v4; // eax@1
  char *v5; // ecx@2
  unsigned __int16 v6; // ax@11
  int v7; // ecx@13
  Vec3_short_ *v8; // esi@16
  int v9; // edx@16
  signed int v10; // eax@19
  signed int v11; // edi@20
  signed int v12; // ecx@20
  signed int v13; // esi@20
  int v14; // edx@21
  int v15; // edx@25
  unsigned __int16 v16; // ax@35
  signed int v17; // eax@37
  int v18; // eax@38
  signed int v19; // [sp+Ch] [bp-14h]@19
  char *v20; // [sp+14h] [bp-Ch]@2
  BLVFace *v21; // [sp+18h] [bp-8h]@1
  signed int v22; // [sp+1Ch] [bp-4h]@20
  signed int v23; // [sp+28h] [bp+8h]@20

  v3 = this;
  v4 = &pIndoor->pFaces[uFaceID];
  v21 = v4;
  if (v4->Portal())
  {
    v5 = (char *)this + 2252 * node_id;
    v20 = v5;
    if ( uFaceID == *((short *)v5 + 2982) )
      return;
    if (!node_id
      && pGame->pIndoorCameraD3D->vPartyPos.x >= v4->pBounding.x1 - 16
      && pGame->pIndoorCameraD3D->vPartyPos.x <= v4->pBounding.x2 + 16
      && pGame->pIndoorCameraD3D->vPartyPos.y >= v4->pBounding.y1 - 16
      && pGame->pIndoorCameraD3D->vPartyPos.y <= v4->pBounding.y2 + 16
      && pGame->pIndoorCameraD3D->vPartyPos.z >= v4->pBounding.z1 - 16
      && pGame->pIndoorCameraD3D->vPartyPos.z <= v4->pBounding.z2 + 16 )
    {
      if ( abs(v4->pFacePlane_old.dist + pGame->pIndoorCameraD3D->vPartyPos.x * v4->pFacePlane_old.vNormal.x
                                       + pGame->pIndoorCameraD3D->vPartyPos.y * v4->pFacePlane_old.vNormal.y
                                       + pGame->pIndoorCameraD3D->vPartyPos.z * v4->pFacePlane_old.vNormal.z) <= 589824 )
      {
        v6 = v21->uSectorID;
        if ( v3->nodes[0].uSectorID == v6 )
          v6 = v21->uBackSectorID;
        v3->nodes[v3->num_nodes].uSectorID = v6;
        v3->nodes[v3->num_nodes].uFaceID = uFaceID;
        v3->nodes[v3->num_nodes].uViewportX = LOWORD(pBLVRenderParams->uViewportX);
        v3->nodes[v3->num_nodes].uViewportZ = LOWORD(pBLVRenderParams->uViewportZ);
        v3->nodes[v3->num_nodes].uViewportY = LOWORD(pBLVRenderParams->uViewportY);
        v3->nodes[v3->num_nodes].uViewportW = LOWORD(pBLVRenderParams->uViewportW);
        v3->nodes[v3->num_nodes++].PortalScreenData.GetViewportData(
          SLOWORD(pBLVRenderParams->uViewportX),
          pBLVRenderParams->uViewportY,
          SLOWORD(pBLVRenderParams->uViewportZ),
          pBLVRenderParams->uViewportW);
        v7 = v3->num_nodes - 1;
        goto LABEL_14;
      }
      v4 = v21;
      v5 = v20;
    }
    v8 = &pIndoor->pVertices[*v4->pVertexIDs];
    v9 = v4->pFacePlane_old.vNormal.x * (v8->x - pGame->pIndoorCameraD3D->vPartyPos.x)
       + v4->pFacePlane_old.vNormal.y * (v8->y - pGame->pIndoorCameraD3D->vPartyPos.y)
       + v4->pFacePlane_old.vNormal.z * (v8->z - pGame->pIndoorCameraD3D->vPartyPos.z);
    if ( *((short *)v5 + 2004) != v4->uSectorID )
      v9 = -v9;
    if ( v9 < 0 )
    {
      v10 = GetPortalScreenCoord(uFaceID);
      v19 = v10;
      if ( v10 )
      {
        v11 = PortalFace._screen_space_x[0];
        v12 = PortalFace._screen_space_y[0];
        v23 = PortalFace._screen_space_x[0];
        v13 = 1;
        v22 = PortalFace._screen_space_y[0];
        if ( v10 > 1 )
        {
          do
          {
            v14 = PortalFace._screen_space_x[v13];
            if ( v14 < v23 )
              v23 = PortalFace._screen_space_x[v13];
            if ( v14 > v11 )
              v11 = PortalFace._screen_space_x[v13];
            v15 = PortalFace._screen_space_y[v13];
            if ( v15 < v22 )
              v22 = PortalFace._screen_space_y[v13];
            if ( v15 > v12 )
              v12 = PortalFace._screen_space_y[v13];
            v10 = v19;
            ++v13;
          }
          while ( v13 < v19 );
        }
        if ( v11 >= *((short *)v20 + 2005)
          && v23 <= *((short *)v20 + 2007)
          && v12 >= *((short *)v20 + 2006)
          && v22 <= *((short *)v20 + 2008)
          && PortalFrustrum(v10, &v3->nodes[v3->num_nodes].PortalScreenData, (BspRenderer_PortalViewportData *)(v20 + 4020), uFaceID) )
        {
          v16 = v21->uSectorID;
          if ( *((short *)v20 + 2004) == v16 )
            v16 = v21->uBackSectorID;
          v3->nodes[v3->num_nodes].uSectorID = v16;
          v3->nodes[v3->num_nodes].uFaceID = uFaceID;
          v3->nodes[v3->num_nodes].uViewportX = LOWORD(pBLVRenderParams->uViewportX);
          v3->nodes[v3->num_nodes].uViewportZ = LOWORD(pBLVRenderParams->uViewportZ);
          v3->nodes[v3->num_nodes].uViewportY = LOWORD(pBLVRenderParams->uViewportY);
          v3->nodes[v3->num_nodes].uViewportW = LOWORD(pBLVRenderParams->uViewportW);
          v17 = v3->num_nodes;
          if ( v17 < 150 )
          {
            v18 = v17 + 1;
            v3->num_nodes = v18;
            v7 = v18 - 1;
LABEL_14:
            AddBspNodeToRenderList(v7);
            return;
          }
        }
      }
    }
  }
  else
  {
    if (num_faces < 1000)
    {
      faces[num_faces].uFaceID = uFaceID;
      faces[num_faces++].uNodeID = node_id;
    }
  }
}

//----- (004B0967) --------------------------------------------------------
void BspRenderer::DrawFaceOutlines()
{
  signed int i; // edi@1
  int v1; // esi@2
  unsigned int v2; // ecx@4
  int v3; // eax@4
  int v4; // eax@6
  unsigned __int16 *v5; // edx@6
  int v6; // ecx@7
  int v7; // esi@8

  for ( i = 0; i < (signed int)pBspRenderer->num_faces; ++i )
  {
    v1 = pBspRenderer->faces[i].uFaceID;
    if ( v1 >= 0 )
    {
      if ( v1 < (signed int)pIndoor->uNumFaces )
      {
        v2 = pBspRenderer->faces[i].uFaceID;
        pBLVRenderParams->field_7C = &pBspRenderer->nodes[pBspRenderer->faces[i].uNodeID].PortalScreenData;
        v3 = GetPortalScreenCoord(v2);
        if ( v3 )
        {
          if ( PortalFrustrum(v3, &stru_F8A590, pBLVRenderParams->field_7C, v1) )
          {
            v4 = stru_F8A590._viewport_space_y;
            v5 = pBLVRenderParams->pRenderTarget;
            if ( stru_F8A590._viewport_space_y <= stru_F8A590._viewport_space_w )
            {
              v6 = 640 * stru_F8A590._viewport_space_y;
              do
              {
                v5[v6 + stru_F8A590.viewport_left_side[v4]] = -1;
                v7 = v6 + stru_F8A590.viewport_right_side[v4];
                v6 += 640;
                v5[v7] = -1;
                ++v4;
              }
              while ( v4 <= stru_F8A590._viewport_space_w );
            }
          }
        }
      }
    }
  }
}


//----- (004AE5BA) --------------------------------------------------------
Texture *BLVFace::GetTexture()
{
  unsigned int v1; // ecx@2

  if ( uAttributes & 0x4000 )
    v1 = pTextureFrameTable->GetFrameTexture(this->uBitmapID, pBLVRenderParams->field_0_timer_);
  else
    v1 = uBitmapID;
  return pBitmaps_LOD->GetTexture(v1);
}


//----- (00498B15) --------------------------------------------------------
void IndoorLocation::Release()
{
  IndoorLocation *v1; // esi@1
  char *v2; // ebp@1
  void *v3; // ST00_4@1

  v1 = this;
  v2 = (char *)&this->ptr_0002B4_doors_ddata;
  free(this->ptr_0002B4_doors_ddata);
  *(int *)v2 = 0;
  free(v1->ptr_0002B0_sector_rdata);
  v1->ptr_0002B0_sector_rdata = 0;
  free(v1->ptr_0002B8_sector_lrdata);
  v1->ptr_0002B8_sector_lrdata = 0;
  free(v1->pLFaces);
  v1->pLFaces = 0;
  free(v1->pSpawnPoints);
  v3 = v1->pVertices;
  v1->pSpawnPoints = 0;
  v1->uNumSectors = 0;
  v1->uNumFaces = 0;
  v1->uNumVertices = 0;
  v1->uNumNodes = 0;
  v1->uNumDoors = 0;
  v1->uNumLights = 0;
  free(v3);
  free(v1->pFaces);
  free(v1->pFaceExtras);
  free(v1->pSectors);
  free(v1->pLights);
  free(v1->pDoors);
  free(v1->pNodes);
  free(v1->pMapOutlines);
  v1->pVertices = 0;
  v1->pFaces = 0;
  v1->pFaceExtras = 0;
  v1->pSectors = 0;
  v1->pLights = 0;
  v1->pDoors = 0;
  v1->pNodes = 0;
  v1->pMapOutlines = 0;
  v1->bLoaded = 0;
}

//----- (00498C45) --------------------------------------------------------
bool IndoorLocation::Alloc()
    {
  pVertices   = (Vec3_short_ *)   malloc(15000 * sizeof(Vec3_short_));//0x15F90u
  pFaces      = (BLVFace *)       malloc(10000 * sizeof(BLVFace));//0xEA600u
  pFaceExtras = (BLVFaceExtra *)  malloc(5000 * sizeof(BLVFaceExtra)); //0x2BF20u
  pSectors =    (BLVSector *)     malloc(512 * sizeof(BLVSector));//0xE800u
  pLights =     (BLVLightMM7 *)   malloc(400 * sizeof(BLVLightMM7));//0x1900u
  pDoors =      (BLVDoor *)       malloc(200 * sizeof(BLVDoor));//0x3E80u
  pNodes =      (BSPNode *)       malloc(5000 * sizeof(BSPNode));//0x9C40u
  pMapOutlines =(BLVMapOutlines *)malloc(sizeof(BLVMapOutlines));//0x14824u
  if (pVertices && pFaces && pFaceExtras && pSectors && pLights && pDoors && pNodes && pMapOutlines )
  {
    memset(pVertices,  0, 15000*sizeof(Vec3_short_));
    memset(pFaces,     0, 10000*sizeof(BLVFace));
    memset(pFaceExtras,0,  5000*sizeof(BLVFaceExtra));
    memset(pSectors,   0,   512*sizeof(BLVSector));
    memset(pLights,    0,   400*sizeof(BLVLightMM7));
    memset(pDoors,     0,   200*sizeof(BLVDoor));
    memset(pNodes,     0,  5000*sizeof(BSPNode));
    memset(pMapOutlines,0,  sizeof(BLVMapOutlines));
    return true;
  }
  else
  {
    return false;
  }
}



//----- (00444810) --------------------------------------------------------
unsigned int IndoorLocation::GetLocationIndex(const char *Str1)
{
  const char *v1; // edi@1
  signed int v2; // esi@1

  for (uint i = 0; i < 11; ++i)
    if (!_stricmp(Str1, _4E6BDC_loc_names[i]))
      return i + 1;
  return 0;
}



//----- (004488F7) --------------------------------------------------------
void IndoorLocation::ToggleLight(signed int sLightID, unsigned int bToggle)
{
  if ( uCurrentlyLoadedLevelType == LEVEL_Indoor && (sLightID <= pIndoor->uNumLights - 1) && (sLightID >= 0) )
  {
    if ( bToggle )
      pIndoor->pLights[sLightID].uAtributes &= 0xFFFFFFF7u;
    else
      pIndoor->pLights[sLightID].uAtributes |= 8u;
    pParty->uFlags |= 2u;
  }
}


//----- (00498E0A) --------------------------------------------------------
bool IndoorLocation::Load(char *pFilename, int a3, size_t _i, char *pDest)
{
  /*unsigned int v5; // ebx@1
  //IndoorLocation *v6; // esi@1
  FILE *v7; // edi@3
  bool result; // eax@3
  char *v9; // ecx@4
  void *v10; // eax@4
  //unsigned __int8 v11; // zf@4
  //unsigned __int8 v12; // sf@4
  int v13; // eax@5
  size_t v14; // ecx@6
  char *v15; // ecx@6
  int v16; // edx@6
  size_t v17; // ecx@6
  char *v18; // ecx@6
  int v19; // edx@6
  unsigned __int16 *v20; // edx@6
  unsigned __int16 *v21; // edx@6
  unsigned __int16 *v22; // edx@6
  __int16 v23; // ax@10
  char *v24; // ecx@10
  __int16 v25; // cx@10
  __int16 v26; // ax@11
  char *v27; // ecx@11
  unsigned __int16 v28; // ax@17
  BLVFaceExtra *v29; // ecx@17
  char *v30; // edx@17
  int v31; // ecx@20
  void *v32; // eax@25
  int v33; // eax@26
  unsigned __int16 *v34; // edx@27
  size_t v35; // ecx@27
  char *v36; // ecx@27
  int v37; // edx@27
  size_t v38; // ecx@27
  char *v39; // ecx@27
  int v40; // edx@27
  unsigned __int16 *v41; // edx@27
  unsigned __int16 *v42; // edx@27
  unsigned __int16 *v43; // edx@27
  unsigned __int16 *v44; // edx@27
  size_t v45; // ecx@27
  unsigned __int16 *v46; // edx@27
  void *v47; // eax@28
  BLVSector *v48; // eax@29
  size_t v49; // ecx@29
  unsigned __int16 *v50; // edx@31
  void *v51; // eax@32
  int v52; // eax@33
  unsigned __int16 *v53; // edx@34
  size_t v54; // ecx@34
  char *v55; // ecx@34
  int v56; // edx@34
  size_t v57; // ecx@34
  char *v58; // ecx@34
  int v59; // edx@34
  unsigned __int16 *v60; // edx@34
  unsigned __int16 *v61; // edx@34
  unsigned __int16 *v62; // edx@34
  unsigned __int16 *v63; // edx@34
  int v64; // ecx@34
  BLVDoor *v65; // ecx@36
  char *v66; // eax@37
  int v67; // edx@38
  int v68; // ecx@38
  BLVFace *v69; // edx@38
  int v70; // ecx@38
  int v71; // edx@38
  int v72; // eax@38
  unsigned __int16 v73; // ax@42
  char *v74; // ecx@42
  SpriteObject *v75; // ecx@44
  size_t v76; // eax@45
  int j; // edx@46
  unsigned __int16 v78; // ax@50
  void *v79; // eax@52
  void **v80; // esi@52
  unsigned int v145; // eax@103
  void *v146; // eax@103
  unsigned int v147; // ecx@103
  int v148; // ebx@103
  unsigned int *v149; // ecx@103*/
  //size_t v150; // eax@103
  //unsigned int v151; // ebx@109
  //unsigned int v152; // ecx@116
  //unsigned int v153; // eax@117
  //size_t v154; // ebx@126
  //unsigned int v155; // ebx@134
  //size_t v156; // eax@140
  //signed int v157; // ebx@142
  //int v158; // ebx@148
  //BLVFace *v159; // eax@149
//  BLVFaceExtra *v160; // ecx@149
  //BLVFaceExtra *v161; // ecx@149
  //signed int v162; // ebx@154
  //unsigned int v163; // ebx@157
  //unsigned int v164; // ebx@157
  //unsigned int v165; // edx@158
  //char *v166; // ecx@158
  //unsigned __int16 v167; // ax@161
  //__int16 v168; // ax@165
  //unsigned int v169; // ebx@168
  //void *v170; // eax@168
  //size_t v171; // ebx@168
  //int v172; // edx@168
  //BLVDoor *v173; // ecx@169
  //int k; // eax@169
  //BLVDoor *v175; // ecx@172
  //int v176; // edx@172
  //BLVDoor *v177; // ecx@172
  //int v178; // edx@172
  //BLVDoor *v179; // ecx@172
  //int v180; // edx@172
  //BLVDoor *v181; // ecx@172
  //int v182; // edx@172
  //BLVDoor *v183; // ecx@172
  //int v184; // edx@172
  //BLVDoor *v185; // ecx@172
  //int v186; // edx@172
  //BLVDoor *v187; // ecx@172
  //int v188; // edx@172
  //unsigned __int16 *v189; // ebx@172
  //char *v190; // edx@173
  //BLVDoor *v191; // ecx@174
  //BLVDoor *l; // eax@175
  //signed int v193; // ebx@176
  //int v194; // ecx@176
  //BLVFaceExtra *v195; // ecx@176
  //BLVFace *v196; // ebx@178
  //std::string v197; // [sp-18h] [bp-680h]@66
  //void *v198; // [sp-14h] [bp-67Ch]@72
  //size_t v199; // [sp-10h] [bp-678h]@72
  //size_t v200; // [sp-Ch] [bp-674h]@72
  //const char *v201; // [sp-8h] [bp-670h]@4
  //int v202; // [sp-4h] [bp-66Ch]@4
  char v203[875]; // [sp+Ch] [bp-65Ch]@130
  char FileName[260]; // [sp+378h] [bp-2F0h]@1
  //char DstBuf; // [sp+47Ch] [bp-1ECh]@4
  __int32 Offset; // [sp+480h] [bp-1E8h]@4
  __int32 v207; // [sp+48Ch] [bp-1DCh]@4
  __int32 v208; // [sp+498h] [bp-1D0h]@4
  __int32 v209; // [sp+4A4h] [bp-1C4h]@4
  __int32 v210; // [sp+4B0h] [bp-1B8h]@4
  __int32 v211; // [sp+4BCh] [bp-1ACh]@15
  __int32 v212; // [sp+4C8h] [bp-1A0h]@15
  __int32 v213; // [sp+4D4h] [bp-194h]@25
  __int32 v214; // [sp+4E0h] [bp-188h]@25
  __int32 v215; // [sp+4ECh] [bp-17Ch]@32
  __int32 v216; // [sp+4F8h] [bp-170h]@32
  __int32 v217; // [sp+504h] [bp-164h]@40
  __int32 v218; // [sp+510h] [bp-158h]@40
  __int32 v219; // [sp+51Ch] [bp-14Ch]@43
  __int32 v220; // [sp+528h] [bp-140h]@43
  __int32 v221; // [sp+534h] [bp-134h]@52
  __int32 v222; // [sp+540h] [bp-128h]@52
  __int32 v223; // [sp+54Ch] [bp-11Ch]@52
  __int32 v224; // [sp+558h] [bp-110h]@52
  __int32 v225; // [sp+564h] [bp-104h]@52
  __int32 v226; // [sp+570h] [bp-F8h]@52
  __int32 v227; // [sp+57Ch] [bp-ECh]@52
  __int32 v228; // [sp+588h] [bp-E0h]@52
  __int32 v229; // [sp+594h] [bp-D4h]@52
  __int32 v230; // [sp+5A0h] [bp-C8h]@52
  __int32 v231; // [sp+5ACh] [bp-BCh]@52
  __int32 v232; // [sp+5B8h] [bp-B0h]@52
  __int32 v233; // [sp+5C4h] [bp-A4h]@52
  __int32 v234; // [sp+5D0h] [bp-98h]@52
  //char pName[40]; // [sp+5FCh] [bp-6Ch]@42
  //size_t pSource; // [sp+624h] [bp-44h]@67
  //char Dst[12]; // [sp+628h] [bp-40h]@9
  //char *v238; // [sp+634h] [bp-34h]@38
  ODMHeader header; // [sp+638h] [bp-30h]@61
  //void *ptr; // [sp+648h] [bp-20h]@66
  //size_t Count; // [sp+64Ch] [bp-1Ch]@109
  //int uSourceLen; // [sp+653h] [bp-15h]@66
  FILE *File; // [sp+658h] [bp-10h]@56
  //BLVSector *v244; // [sp+65Ch] [bp-Ch]@72
  //int v245; // [sp+660h] [bp-8h]@72
  //BLVFace *Src; // [sp+664h] [bp-4h]@73
  signed int Argsa; // [sp+670h] [bp+8h]@4
  signed int Argsb; // [sp+670h] [bp+8h]@7
  signed int Argsc; // [sp+670h] [bp+8h]@15
  signed int Argsd; // [sp+670h] [bp+8h]@18
  int Argse; // [sp+670h] [bp+8h]@25
  int Argsf; // [sp+670h] [bp+8h]@28
  int Argsg; // [sp+670h] [bp+8h]@32
  int Argsh; // [sp+670h] [bp+8h]@35
  signed int Argsi; // [sp+670h] [bp+8h]@40
  signed int Argsj; // [sp+670h] [bp+8h]@45
  //int Argsk; // [sp+670h] [bp+8h]@143
  //void *Argsl; // [sp+670h] [bp+8h]@155
  //signed int Argsm; // [sp+670h] [bp+8h]@161
  //signed int Argsn; // [sp+670h] [bp+8h]@175

  //v5 = 0;
  //v6 = this;
  _6807E0_num_decorations_with_sounds_6807B8 = 0;

  #pragma region "loading from txt"
  /*sprintf(FileName, "levels\\%s", pFilename);
  if ( GetFileAttributesA(FileName) != -1 )
  {
    Release();
    if ( Alloc() )
    {
      v7 = fopen(FileName, "rb");
      result = 1;
      if ( !v7 )
        return result;
      v9 = pDest;
      bLoaded = 1;
      v202 = (int)v7;
      v201 = (const char *)1;
      *(int *)v9 = 1;
      fread(&DstBuf, 0x180u, (size_t)v201, (FILE *)v202);
      fseek(v7, Offset, 0);
      fread(&blv, 0x88u, 1u, v7);
      fseek(v7, v207, 0);
      fread(&uNumVertices, 1u, 4u, v7);
      fseek(v7, v208, 0);
      fread(pVertices, 6u, uNumVertices, v7);
      fseek(v7, v209, 0);
      fread(&uNumFaces, 4u, 1u, v7);
      fseek(v7, v210, 0);
      fread(pFaces, 0x60u, uNumFaces, v7);
      v10 = malloc(ptr_2AC, blv.uFaces_fdata_Size, "L.FData");
      v202 = (int)v7;
      ptr_2AC = (unsigned __int16 *)v10;
      fread(v10, 1u, blv.uFaces_fdata_Size, (FILE *)v202);
      v11 = uNumFaces == 0;
      v12 = (uNumFaces & 0x80000000u) != 0;
      pDest = 0;
      Argsa = 0;
      if ( !(v12 | v11) )
      {
        v13 = 0;
        do
        {
          pFaces[v13].pVertexIDs = (unsigned __int16 *)&pDest[(unsigned int)ptr_2AC];
          v14 = (size_t)&pFaces[v13];
          i = v14;
          v15 = &pDest[2 * *(char *)(v14 + 93) + 2];
          v16 = (int)&v15[(unsigned int)ptr_2AC];
          pDest = v15;
          *(int *)(i + 52) = v16;
          v17 = (size_t)&pFaces[v13];
          i = v17;
          v18 = &pDest[2 * *(char *)(v17 + 93) + 2];
          v19 = (int)&v18[(unsigned int)ptr_2AC];
          pDest = v18;
          *(int *)(i + 56) = v19;
          i = (size_t)&pFaces[v13];
          v20 = ptr_2AC;
          pDest += 2 * *(char *)(i + 93) + 2;
          *(int *)(i + 60) = (int)(char *)v20 + (int)pDest;
          i = (size_t)&pFaces[v13];
          v21 = ptr_2AC;
          pDest += 2 * *(char *)(i + 93) + 2;
          *(int *)(i + 64) = (int)(char *)v21 + (int)pDest;
          i = (size_t)&pFaces[v13];
          ++v13;
          v22 = ptr_2AC;
          pDest += 2 * *(char *)(i + 93) + 2;
          ++Argsa;
          *(int *)(i + 68) = (int)(char *)v22 + (int)pDest;
          pDest += 2 * *((char *)&pFaces[v13] - 3) + 2;
        }
        while ( Argsa < (signed int)uNumFaces );
      }
      Argsb = 0;
      if ( (signed int)uNumFaces > 0 )
      {
        pDest = 0;
        do
        {
          fread(Dst, 1u, 0xAu, v7);
          if ( pDest[(unsigned int)pFaces + 45] & 0x40 )
          {
            v23 = pTextureFrameTable->FindTextureByName(Dst);
            v24 = pDest;
            *(short *)&pDest[(unsigned int)pFaces + 74] = v23;
            v25 = *(short *)&v24[(unsigned int)pFaces + 74];
            if ( v25 )
            {
              pTextureFrameTable->LoadAnimationSequenceAndPalettes(v25);
            }
            else
            {
              v26 = pBitmaps_LOD->LoadTexture(Dst);
              v27 = pDest;
              *(short *)&pDest[(unsigned int)pFaces + 74] = v26;
              v27[(unsigned int)pFaces + 45] &= 0xBFu;
            }
          }
          else
          {
            *(short *)&pDest[(unsigned int)pFaces + 74] = pBitmaps_LOD->LoadTexture(Dst);
          }
          ++Argsb;
          pDest += 96;
        }
        while ( Argsb < (signed int)uNumFaces );
      }
      fseek(v7, v211, 0);
      fread(&uNumFaceExtras, 4u, 1u, v7);
      fseek(v7, v212, 0);
      fread(pFaceExtras, 0x24u, uNumFaceExtras, v7);
      Argsc = 0;
      if ( (signed int)uNumFaceExtras > 0 )
      {
        pDest = 0;
        do
        {
          fread(Dst, 1u, 0xAu, v7);
          v28 = pBitmaps_LOD->LoadTexture(Dst);
          v29 = pFaceExtras;
          v30 = pDest;
          ++Argsc;
          pDest += 36;
          *(unsigned __int16 *)((char *)&v29->uAdditionalBitmapID + (int)v30) = v28;
        }
        while ( Argsc < (signed int)uNumFaceExtras );
      }
      Argsd = 0;
      if ( (signed int)uNumFaces > 0 )
      {
        pDest = 0;
        do
        {
          v31 = (int)&pFaceExtras[*(short *)&pDest[(unsigned int)pFaces + 72]];
          if ( *(short *)(v31 + 26) )
          {
            if ( ((BLVFaceExtra *)v31)->HasEventint() )
              pDest[(unsigned int)pFaces + 46] |= 0x10u;
            else
              pDest[(unsigned int)pFaces + 46] &= 0xEFu;
          }
          ++Argsd;
          pDest += 96;
        }
        while ( Argsd < (signed int)uNumFaces );
      }
      fseek(v7, v213, 0);
      fread(&uNumSectors, 4u, 1u, v7);
      fseek(v7, v214, 0);
      fread(pSectors, 0x74u, uNumSectors, v7);
      v32 = malloc(ptr_0002B0_sector_rdata, blv.uSector_rdata_Size, "L.RData");
      v202 = (int)v7;
      ptr_0002B0_sector_rdata = (unsigned __int16 *)v32;
      fread(v32, 1u, blv.uSector_rdata_Size, (FILE *)v202);
      v11 = uNumSectors == 0;
      v12 = uNumSectors < 0;
      pDest = 0;
      Argse = 0;
      if ( !(v12 | v11) )
      {
        v33 = 0;
        do
        {
          pSectors[v33].pFloors = (unsigned __int16 *)&pDest[(unsigned int)ptr_0002B0_sector_rdata];
          i = (size_t)&pSectors[v33];
          v34 = ptr_0002B0_sector_rdata;
          pDest += 2 * *(short *)(i + 4);
          *(int *)(i + 16) = (int)(char *)v34 + (int)pDest;
          v35 = (size_t)&pSectors[v33];
          i = v35;
          v36 = &pDest[2 * *(short *)(v35 + 12)];
          v37 = (int)&v36[(unsigned int)ptr_0002B0_sector_rdata];
          pDest = v36;
          *(int *)(i + 24) = v37;
          v38 = (size_t)&pSectors[v33];
          i = v38;
          v39 = &pDest[2 * *(short *)(v38 + 20)];
          v40 = (int)&v39[(unsigned int)ptr_0002B0_sector_rdata];
          pDest = v39;
          *(int *)(i + 32) = v40;
          i = (size_t)&pSectors[v33];
          v41 = ptr_0002B0_sector_rdata;
          pDest += 2 * *(short *)(i + 28);
          *(int *)(i + 40) = (int)(char *)v41 + (int)pDest;
          i = (size_t)&pSectors[v33];
          v42 = ptr_0002B0_sector_rdata;
          pDest += 2 * *(short *)(i + 36);
          *(int *)(i + 48) = (int)(char *)v42 + (int)pDest;
          i = (size_t)&pSectors[v33];
          v43 = ptr_0002B0_sector_rdata;
          pDest += 2 * *(short *)(i + 44);
          *(int *)(i + 64) = (int)(char *)v43 + (int)pDest;
          i = (size_t)&pSectors[v33];
          v44 = ptr_0002B0_sector_rdata;
          pDest += 2 * *(short *)(i + 60);
          *(int *)(i + 72) = (int)(char *)v44 + (int)pDest;
          v45 = (size_t)&pSectors[v33];
          ++v33;
          i = v45;
          v46 = ptr_0002B0_sector_rdata;
          pDest += 2 * *(short *)(v45 + 68);
          ++Argse;
          *(int *)(v45 + 80) = (int)(char *)v46 + (int)pDest;
          pDest += 2 * *((short *)&pSectors[v33] - 20);
        }
        while ( Argse < uNumSectors );
      }
      v47 = malloc(
              ptr_0002B8_sector_lrdata,
              blv.uSector_lrdata_Size,
              "L.RLData");
      v202 = (int)v7;
      ptr_0002B8_sector_lrdata = (unsigned __int16 *)v47;
      fread(v47, 1u, blv.uSector_lrdata_Size, (FILE *)v202);
      v11 = uNumSectors == 0;
      v12 = uNumSectors < 0;
      pDest = 0;
      Argsf = 0;
      if ( !(v12 | v11) )
      {
        v48 = pSectors;
        v49 = 0;
        for ( i = 0; ; v49 = i )
        {
          v50 = ptr_0002B8_sector_lrdata;
          i += 116;
          ++Argsf;
          *(BLVLightMM7 **)((char *)&v48->pLights + v49) = (BLVLightMM7 *)((char *)v50 + (int)pDest);
          v48 = pSectors;
          pDest += 2 * *(__int16 *)((char *)&v48->uNumLights + v49);
          if ( Argsf >= uNumSectors )
            break;
        }
      }
      fseek(v7, v215, 0);
      fread(&uNumDoors, 4u, 1u, v7);
      fseek(v7, v216, 0);
      fread(pDoors, 0x50u, 0xC8u, v7);
      v51 = malloc(ptr_0002B4_doors_ddata, blv.uDoors_ddata_Size, "L.DData");
      v202 = (int)v7;
      ptr_0002B4_doors_ddata = (unsigned __int16 *)v51;
      fread(v51, 1u, blv.uDoors_ddata_Size, (FILE *)v202);
      v11 = uNumDoors == 0;
      v12 = uNumDoors < 0;
      pDest = 0;
      Argsg = 0;
      if ( !(v12 | v11) )
      {
        v52 = 0;
        do
        {
          pDoors[v52].pVertexIDs = (unsigned __int16 *)&pDest[(unsigned int)ptr_0002B4_doors_ddata];
          i = (size_t)&pDoors[v52];
          v53 = ptr_0002B4_doors_ddata;
          pDest += 2 * *(short *)(i + 68);
          *(int *)(i + 40) = (int)(char *)v53 + (int)pDest;
          v54 = (size_t)&pDoors[v52];
          i = v54;
          v55 = &pDest[2 * *(short *)(v54 + 70)];
          v56 = (int)&v55[(unsigned int)ptr_0002B4_doors_ddata];
          pDest = v55;
          *(int *)(i + 44) = v56;
          v57 = (size_t)&pDoors[v52];
          i = v57;
          v58 = &pDest[2 * *(short *)(v57 + 72)];
          v59 = (int)&v58[(unsigned int)ptr_0002B4_doors_ddata];
          pDest = v58;
          *(int *)(i + 48) = v59;
          i = (size_t)&pDoors[v52];
          v60 = ptr_0002B4_doors_ddata;
          pDest += 2 * *(short *)(i + 70);
          *(int *)(i + 52) = (int)(char *)v60 + (int)pDest;
          i = (size_t)&pDoors[v52];
          v61 = ptr_0002B4_doors_ddata;
          pDest += 2 * *(short *)(i + 70);
          *(int *)(i + 56) = (int)(char *)v61 + (int)pDest;
          i = (size_t)&pDoors[v52];
          v62 = ptr_0002B4_doors_ddata;
          pDest += 2 * *(short *)(i + 74);
          *(int *)(i + 60) = (int)(char *)v62 + (int)pDest;
          i = (size_t)&pDoors[v52];
          v63 = ptr_0002B4_doors_ddata;
          pDest += 2 * *(short *)(i + 74);
          *(int *)(i + 64) = (int)(char *)v63 + (int)pDest;
          ++Argsg;
          v64 = pDoors[v52].uNumOffsets;
          ++v52;
          pDest += 2 * v64;
        }
        while ( Argsg < uNumDoors );
      }
      Argsh = 0;
      if ( uNumDoors > 0 )
      {
        v65 = pDoors;
        pDest = 0;
        do
        {
          i = 0;
          v66 = &pDest[(int)v65];
          if ( *(short *)&pDest[(int)v65 + 70] > 0 )
          {
            do
            {
              v67 = *((int *)v66 + 10);
              a3 = 2 * i;
              v68 = *(short *)(v67 + 2 * i);
              v69 = pFaces;
              ++i;
              v70 = (int)&pFaceExtras[v69[v68].uFaceExtraID];
              v71 = *((int *)v66 + 12);
              v72 = a3;
              v238 = (char *)v70;
              *(short *)(a3 + v71) = *(short *)(v70 + 20);
              *(__int16 *)(*(char **)((char *)&pDoors->pDeltaVs + (unsigned int)pDest) + v72) = *((short *)v238 + 11);
              v65 = pDoors;
              v66 = &pDest[(int)v65];
            }
            while ( (signed int)i < *(short *)&pDest[(int)v65 + 70] );
          }
          ++Argsh;
          pDest += 80;
        }
        while ( Argsh < uNumDoors );
      }
      fseek(v7, v217, 0);
      fread(&uNumLevelDecorations, 4u, 1u, v7);
      fseek(v7, v218, 0);
      fread(pLevelDecorations, 0x20u, uNumLevelDecorations, v7);
      Argsi = 0;
      if ( (signed int)uNumLevelDecorations > 0 )
      {
        pDest = (char *)pLevelDecorations;
        do
        {
          fread(pName, 1u, 0x20u, v7);
          v73 = pDecorationList->GetDecorIdByName(pName);
          v74 = pDest;
          ++Argsi;
          pDest += 32;
          *(short *)v74 = v73;
        }
        while ( Argsi < (signed int)uNumLevelDecorations );
      }
      fseek(v7, v219, 0);
      fread(&uNumSpriteObjects, 4u, 1u, v7);
      fseek(v7, v220, 0);
      fread(pSpriteObjects, 0x70u, uNumSpriteObjects, v7);
      if ( (signed int)uNumSpriteObjects > 0 )
      {
        v75 = pSpriteObjects;
        pDest = (char *)uNumSpriteObjects;
        do
        {
          Argsj = 0;
          v76 = 48 * v75->stru_24.uItemID;
          v11 = pObjectList->uNumObjects == 0;
          v12 = (pObjectList->uNumObjects & 0x80000000u) != 0;
          LOWORD(v76) = *(unsigned __int16 *)((char *)&pItemsTable->pItems[0].uSpriteID + v76);
          i = v76;
          v75->uItemType = v76;
          if ( v12 | v11 )
          {
LABEL_50:
            v78 = 0;
          }
          else
          {
            for ( j = (int)&pObjectList->pObjects->uObjectID; (short)v76 != *(short *)j; j = a3 )
            {
              ++Argsj;
              a3 = j + 56;
              if ( Argsj >= (signed int)pObjectList->uNumObjects )
                goto LABEL_50;
              LOWORD(v76) = i;
            }
            v78 = Argsj;
          }
          v75->uObjectDescID = v78;
          ++v75;
          --pDest;
        }
        while ( pDest );
      }
      fseek(v7, v221, 0);
      fread(&uNumActors, 4u, 1u, v7);
      fseek(v7, v222, 0);
      fread(pActors, 0x344u, uNumActors, v7);
      fseek(v7, v228, 0);
      fread(&uNumChests, 4u, 1u, v7);
      fseek(v7, v229, 0);
      fread(pChests, 0x14CCu, uNumChests, v7);
      fseek(v7, v224, 0);
      fread(&uNumLights, 4u, 1u, v7);
      fseek(v7, v225, 0);
      fread(pLights, 0x10u, uNumLights, v7);
      fseek(v7, v226, 0);
      fread(&uNumNodes, 4u, 1u, v7);
      fseek(v7, v227, 0);
      fread(pNodes, 8u, uNumNodes, v7);
      fseek(v7, v230, 0);
      fread(&uNumSpawnPoints, 4u, 1u, v7);
      v79 = malloc(pSpawnPoints, 24 * uNumSpawnPoints, "Spawn");
      v202 = 0;
      pSpawnPoints = (SpawnPointMM7 *)v79;
      fseek(v7, v231, v202);
      fread(pSpawnPoints, 0x18u, uNumSpawnPoints, v7);
      fseek(v7, v232, 0);
      fread(&dlv, 0x28u, 1u, v7);
      fseek(v7, v233, 0);
      fread(&stru_5E4C90, 1u, 0xC8u, v7);
      fseek(v7, v234, 0);
      fread(&uLastVisitDay, 1u, 0x38u, v7);
      fseek(v7, v223, 0);
      v80 = (void **)&pMapOutlines;
      fread(*v80, 4u, 1u, v7);
      fread((char *)*v80 + 4, 0xCu, *(int *)*v80, v7);
      fclose(v7);
      goto LABEL_179;
    }
    return 4;
  }*/
  #pragma endregion

  if (bLoaded)
  {
    Log::Warning(L"BLV is already loaded");
    return 3;
  }

  if ( !pGames_LOD->DoesContainerExist(pFilename) )
    Error("Unable to find %s in Games.LOD", pFilename);

  //v238 = pFilename - 4;
  //v81 = strlen(pFilename);
  strcpy(this->pFilename, pFilename);
  strcpy(&pFilename[strlen(pFilename) - 4], ".blv");
  File = pGames_LOD->FindContainer(pFilename, 1);
  //File = v82;

  Release();
  if ( !Alloc() )
    return 4;
  
  header.uVersion = 91969;
  header.pMagic[0] = 'm';
  header.pMagic[1] = 'v';
  header.pMagic[2] = 'i';
  header.pMagic[3] = 'i';
  header.uCompressedSize = 0;
  header.uDecompressedSize = 0;
  fread(&header, sizeof(ODMHeader), 1u, File);
  if (header.uVersion != 91969 ||
      header.pMagic[0] != 'm'  ||
      header.pMagic[1] != 'v'  ||
      header.pMagic[2] != 'i'  ||
      header.pMagic[3] != 'i' )
  {
    MessageBoxW(nullptr, L"Can't load file!", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Polydata.cpp:792", 0);
  }
  //v83 = header.uCompressedSize;
  //pSource = header.uDecompressedSize;
  //v84 = malloc(header.uDecompressedSize);
  //v85 = v84;
  //ptr = v84;
  auto pRawBLV = malloc(header.uDecompressedSize);
  memset(pRawBLV, 0, header.uDecompressedSize);

  if (header.uCompressedSize == header.uDecompressedSize)
    fread(pRawBLV, header.uDecompressedSize, 1, File);
  else if (header.uCompressedSize < header.uDecompressedSize)
  {
    auto pTmpMem = malloc(header.uCompressedSize);
    {
      fread(pTmpMem, header.uCompressedSize, 1, File);

      uint uDecompressedSize = header.uDecompressedSize;
      zlib::MemUnzip(pRawBLV, &uDecompressedSize, pTmpMem, header.uCompressedSize);

      if (uDecompressedSize != header.uDecompressedSize)
        Log::Warning(L"uDecompressedSize != header.uDecompressedSize in BLV");
    }
    free(pTmpMem);
  }
  else
  {
    MessageBoxW(nullptr, L"Can't load file!", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Polydata.cpp:803", 0);
    return 0;
  }
  
  bLoaded = true;

  auto pData = (char *)pRawBLV;
  
  pGameLoadingUI_ProgressBar->Progress();

  memcpy(&blv, pData, 136);
  memcpy(&uNumVertices, pData += 136, 4);
  memcpy(pVertices, pData += 4, uNumVertices * sizeof(Vec3_short_));
  
  pGameLoadingUI_ProgressBar->Progress();

  memcpy(&uNumFaces, pData += uNumVertices * sizeof(Vec3_short_), 4);

  pGameLoadingUI_ProgressBar->Progress();

  memcpy(pFaces, pData += 4, uNumFaces * sizeof (BLVFace));
  pLFaces = (unsigned __int16 *)malloc(blv.uFaces_fdata_Size);

  memcpy(pLFaces, pData += uNumFaces * sizeof (BLVFace), blv.uFaces_fdata_Size);

  for (uint i = 0, j = 0; i < uNumFaces; ++i)
  {
    auto pFace = pFaces + i;

    pFace->pVertexIDs = pLFaces + j;
    
    j += pFace->uNumVertices + 1;
    pFace->pXInterceptDisplacements = (short *)(pLFaces + j);

    j += pFace->uNumVertices + 1;
    pFace->pYInterceptDisplacements = (short *)(pLFaces + j);

    j += pFace->uNumVertices + 1;
    pFace->pZInterceptDisplacements = (short *)(pLFaces + j);

    j += pFace->uNumVertices + 1;
    pFace->pVertexUIDs = (__int16 *)(pLFaces + j);

    j += pFace->uNumVertices + 1;
    pFace->pVertexVIDs = (__int16 *)(pLFaces + j);

    j += pFace->uNumVertices + 1;
      /*v93 = &pFaces[v92];
      Src = v93;
      v94 = (BLVSector *)((char *)v244 + 2 * v93->uNumVertices + 2);
      v95 = (unsigned __int16 *)((char *)v94 + (unsigned int)ptr_2AC);
      v244 = v94;
      Src->pXInterceptDisplacements = v95;
      v96 = (int)&pFaces[v92];
      Src = (BLVFace *)v96;
      v97 = (BLVSector *)((char *)v244 + 2 * *(char *)(v96 + 93) + 2);
      v98 = (unsigned __int16 *)((char *)v97 + (unsigned int)ptr_2AC);
      v244 = v97;
      Src->pYInterceptDisplacements = v98;
      Src = &pFaces[v92];
      v99 = ptr_2AC;
      v244 = (BLVSector *)((char *)v244 + 2 * Src->uNumVertices + 2);
      Src->pZInterceptDisplacements = (unsigned __int16 *)((char *)v99 + (int)v244);
      Src = &pFaces[v92];
      v100 = ptr_2AC;
      v244 = (BLVSector *)((char *)v244 + 2 * Src->uNumVertices + 2);
      Src->pVertexUIDs = (unsigned __int16 *)((char *)v100 + (int)v244);
      Src = &pFaces[v92];
      ++v92;
      v101 = ptr_2AC;
      v244 = (BLVSector *)((char *)v244 + 2 * Src->uNumVertices + 2);
      ++v245;
      Src->pVertexVIDs = (unsigned __int16 *)((char *)v101 + (int)v244);
      v244 = (BLVSector *)((char *)v244 + 2 * *((char *)&pFaces[v92] - 3) + 2);*/
  }

  pGameLoadingUI_ProgressBar->Progress();

  pData += blv.uFaces_fdata_Size;

  for (uint i = 0; i < uNumFaces; ++i)
  {
    auto pFace = pFaces + i;

    char pTexName[16];
    strncpy(pTexName, pData, 10);
    pData += 10;

    if (pFace->uAttributes & 0x4000)
    {
      pFace->uBitmapID = pTextureFrameTable->FindTextureByName(pTexName);
      if (pFace->uBitmapID)
        pTextureFrameTable->LoadAnimationSequenceAndPalettes(pFace->uBitmapID);
      else
      {
        pFace->uBitmapID = pBitmaps_LOD->LoadTexture(pTexName);
        pFace->uAttributes &= ~0x4000;
      }
    }
    else
      pFace->uBitmapID = pBitmaps_LOD->LoadTexture(pTexName);
  }

  pGameLoadingUI_ProgressBar->Progress();

  memcpy(&uNumFaceExtras, pData, 4u);
  memcpy(pFaceExtras, pData += 4, uNumFaceExtras * sizeof(BLVFaceExtra));
  pData += uNumFaceExtras * sizeof(BLVFaceExtra);

  pGameLoadingUI_ProgressBar->Progress();

  //v108 = (char *)v107 + 36 * uNumFaceExtras;
  //v245 = 0;
  //*(int *)((char *)&uSourceLen + 1) = 0;
  for (uint i = 0; i < uNumFaceExtras; ++i)
  {
    char pTexName[32];
    strncpy(pTexName, pData, 10);
    pData += 10;

    if (!strcmp(pTexName, ""))
      pFaceExtras[i].uAdditionalBitmapID = -1;
    else
      pFaceExtras[i].uAdditionalBitmapID = pBitmaps_LOD->LoadTexture(pTexName);
  }


  for (uint i = 0; i < uNumFaces; ++i)
  {
    auto pFace = pFaces + i;
    auto pFaceExtra = pFaceExtras + pFace->uFaceExtraID;

    if (pFaceExtra->uEventID)
    {
      if (pFaceExtra->HasEventint())
        pFace->uAttributes |= 0x100000;
      else
        pFace->uAttributes &= ~0x100000;
    }
  }

  pGameLoadingUI_ProgressBar->Progress();

  memcpy(&uNumSectors, pData, 4);
  memcpy(pSectors, pData + 4, uNumSectors * sizeof(BLVSector));
  pData += 4 + uNumSectors * sizeof(BLVSector);

  pGameLoadingUI_ProgressBar->Progress();

  ptr_0002B0_sector_rdata = (unsigned short *)malloc(blv.uSector_rdata_Size);//, "L.RData");
  memcpy(ptr_0002B0_sector_rdata, pData, blv.uSector_rdata_Size);
  pData += blv.uSector_rdata_Size;

  for (uint i = 0, j = 0; i < uNumSectors; ++i)
  {
    auto pSector = pSectors + i;

    pSector->pFloors = ptr_0002B0_sector_rdata + j;
    j += pSector->uNumFloors;

    pSector->pWalls = ptr_0002B0_sector_rdata + j;
    j += pSector->uNumWalls;

    pSector->pCeilings = ptr_0002B0_sector_rdata + j;
    j += pSector->uNumCeilings;

    pSector->pFluids = ptr_0002B0_sector_rdata + j;
    j += pSector->uNumFluids;

    pSector->pPortals = ptr_0002B0_sector_rdata + j;
    j += pSector->uNumPortals;

    pSector->pFaceIDs = ptr_0002B0_sector_rdata + j;
    j += pSector->uNumFaces;

    pSector->pCogs = ptr_0002B0_sector_rdata + j;
    j += pSector->uNumCogs;

    pSector->pDecorationIDs = ptr_0002B0_sector_rdata + j;
    j += pSector->uNumDecorations;

    pSector->pMarkers = ptr_0002B0_sector_rdata + j;
    j += pSector->uNumMarkers;


    //do
    //{
      /*pSectors[v118].pFloors = (unsigned __int16 *)((char *)Src + (unsigned int)ptr_0002B0_sector_rdata);
      v244 = &pSectors[v118];
      v119 = ptr_0002B0_sector_rdata;
      Src = (BLVFace *)((char *)Src + 2 * v244->field_4);
      v244->pWalls = (unsigned __int16 *)((char *)v119 + (int)Src);
      v120 = (int)&pSectors[v118];
      v244 = (BLVSector *)v120;
      v121 = (BLVFace *)((char *)Src + 2 * *(short *)(v120 + 12));
      v122 = (unsigned __int16 *)((char *)v121 + (unsigned int)ptr_0002B0_sector_rdata);
      Src = v121;
      v244->pCeilings = v122;
      v123 = (int)&pSectors[v118];
      v244 = (BLVSector *)v123;
      v124 = (BLVFace *)((char *)Src + 2 * *(short *)(v123 + 20));
      v125 = (unsigned __int16 *)((char *)v124 + (unsigned int)ptr_0002B0_sector_rdata);
      Src = v124;
      v244->pFluids = v125;
      v244 = &pSectors[v118];
      v126 = ptr_0002B0_sector_rdata;
      Src = (BLVFace *)((char *)Src + 2 * v244->field_1C);
      v244->pPortals = (unsigned __int16 *)((char *)v126 + (int)Src);
      v244 = &pSectors[v118];
      v127 = ptr_0002B0_sector_rdata;
      Src = (BLVFace *)((char *)Src + 2 * v244->uNumPortals);
      v244->pFaceIDs = (unsigned __int16 *)((char *)v127 + (int)Src);
      v244 = &pSectors[v118];
      v128 = ptr_0002B0_sector_rdata;
      Src = (BLVFace *)((char *)Src + 2 * v244->field_2C);
      v244->pCogs = (unsigned __int16 *)((char *)v128 + (int)Src);
      v244 = &pSectors[v118];
      v129 = ptr_0002B0_sector_rdata;
      Src = (BLVFace *)((char *)Src + 2 * v244->field_3C);
      v244->pDecorationIDs = (unsigned __int16 *)((char *)v129 + (int)Src);
      v130 = (int)&pSectors[v118];
      ++v118;
      v244 = (BLVSector *)v130;
      v131 = ptr_0002B0_sector_rdata;
      Src = (BLVFace *)((char *)Src + 2 * *(short *)(v130 + 68));
      ++v245;
      *(int *)(v130 + 80) = (int)(char *)v131 + (int)Src;
      Src = (BLVFace *)((char *)Src + 2 * *((short *)&pSectors[v118] - 20));*/
    //}
    //while ( v245 < uNumSectors );
  }

  ptr_0002B8_sector_lrdata = (unsigned __int16 *)malloc(blv.uSector_lrdata_Size);//, "L.RLData");
  memcpy(ptr_0002B8_sector_lrdata, pData, blv.uSector_lrdata_Size);
  pData += blv.uSector_lrdata_Size;

  pGameLoadingUI_ProgressBar->Progress();

  for (uint i = 0, j = 0; i < uNumSectors; ++i)
  {
    pSectors[i].pLights = ptr_0002B8_sector_lrdata + j;
    j += pSectors[i].uNumLights;
  }

  pGameLoadingUI_ProgressBar->Progress();

  memcpy(&uNumDoors, pData, 4);
  pData += 4;

  pGameLoadingUI_ProgressBar->Progress();
  pGameLoadingUI_ProgressBar->Progress();
  
  memcpy(&uNumLevelDecorations, pData, 4);
  memcpy(pLevelDecorations.data(), pData + 4, uNumLevelDecorations * sizeof(LevelDecoration));
  pData += 4 + uNumLevelDecorations * sizeof(LevelDecoration);

  for (uint i = 0; i < uNumLevelDecorations; ++i)
  {
    pLevelDecorations[i].uDecorationDescID = pDecorationList->GetDecorIdByName(pData);

    pData += 32;
  }

  pGameLoadingUI_ProgressBar->Progress();

  memcpy(&uNumLights, pData, 4);
  memcpy(pLights, pData + 4, uNumLights * sizeof(BLVLightMM7));
  pData += 4 + uNumLights * sizeof(BLVLightMM7);

  pGameLoadingUI_ProgressBar->Progress();
  pGameLoadingUI_ProgressBar->Progress();

  memcpy(&uNumNodes, pData, 4);
  memcpy(pNodes, pData + 4, uNumNodes * sizeof(BSPNode));
  pData += 4 + uNumNodes * sizeof(BSPNode);
  
  pGameLoadingUI_ProgressBar->Progress();
  pGameLoadingUI_ProgressBar->Progress();

  memcpy(&uNumSpawnPoints, pData, 4);
  pSpawnPoints = (SpawnPointMM7 *)malloc(uNumSpawnPoints * sizeof(SpawnPointMM7));
  memcpy(pSpawnPoints, pData + 4, uNumSpawnPoints * sizeof(SpawnPointMM7));
  pData += 4 + uNumSpawnPoints * sizeof(SpawnPointMM7);
  
  pGameLoadingUI_ProgressBar->Progress();
  pGameLoadingUI_ProgressBar->Progress();

  //v201 = (const char *)v148;
  //v200 = (size_t)pMapOutlines;
  memcpy(&pMapOutlines->uNumOutlines, pData, 4);
  memcpy(pMapOutlines->pOutlines, pData + 4, pMapOutlines->uNumOutlines * sizeof(BLVMapOutline));
  //v149 = pMapOutlines;
  //v199 = 12 * *v149;
  //memcpy(v149 + 1, (const void *)(v148 + 4), v199);
  free(pRawBLV);
  pRawBLV = nullptr;

  void *pRawDLV = nullptr;
  strcpy(&pFilename[strlen(pFilename) - 4], ".dlv");
  File = pNew_LOD->FindContainer(pFilename, 1);
  fread(&header, 0x10u, 1u, File);//(FILE *)v245);
  bool _v244 = false;
  if (header.uVersion != 91969 ||
      header.pMagic[0] != 'm'  ||
      header.pMagic[1] != 'v'  ||
      header.pMagic[2] != 'i'  ||
      header.pMagic[3] != 'i' )
  {
    MessageBoxW(nullptr, L"Can't load file!", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Polydata.cpp:1090", 0);
    _v244 = true;
  }
  else
  {
    pRawDLV = malloc(header.uDecompressedSize);
    if (header.uCompressedSize == header.uDecompressedSize)
      fread(pRawDLV, 1, header.uCompressedSize, File);
    else if (header.uCompressedSize < header.uDecompressedSize)
    {
      auto pTmpMem = malloc(header.uCompressedSize);
      {
        fread(pTmpMem, header.uCompressedSize, 1, File);

        uint uDecompressedSize = header.uDecompressedSize;
        zlib::MemUnzip(pRawDLV, &uDecompressedSize, pTmpMem, header.uCompressedSize);

        if (uDecompressedSize != header.uDecompressedSize)
          Log::Warning(L"uDecompressedSize != header.uDecompressedSize in DLV");
      }
      free(pTmpMem);
    }
    else
      MessageBoxW(nullptr, L"Can't load file!", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Polydata.cpp:1108", 0);

    pData = (char *)pRawDLV;
  }
  
  memcpy(&dlv, pData, 40);
  pData += 40;

  //v152 = dlv.uNumFacesInBModels;
  if (dlv.uNumFacesInBModels)
  {
    //v153 = dlv.uNumDecorations;
    if (dlv.uNumDecorations)
    {
      if (dlv.uNumFacesInBModels != uNumFaces ||
          dlv.uNumDecorations != uNumLevelDecorations)
        _v244 = true;
    }
  }

  if (dword_6BE364_game_settings_1 & 0x2000 )
    _i = 29030400;
  bool _a = false;
  if ( a3 - dlv.uLastRepawnDay >= _i && _stricmp(pCurrentMapName, "d29.dlv") )
    _a = true;

  //v154 = 875;
  if (_v244)
  {
    memset(v203, 0, 0x36B);
    goto LABEL_132;
  }
  if (_a || !dlv.uLastRepawnDay )
  {
    memcpy(v203, pData, 0x36B);
LABEL_132:
    free(pRawDLV);
    dlv.uLastRepawnDay = a3;
    if (_v244)
      ++dlv.uNumRespawns;
    //v201 = pFilename;
    *(int *)pDest = 1;
    File = pGames_LOD->FindContainer(pFilename, 0);
    fread(&header, 0x10u, 1u, File);
    auto v155 = header.uCompressedSize;
    auto Count = header.uDecompressedSize;
    auto Src = (BLVFace *)malloc(header.uDecompressedSize);
    pRawDLV = Src;
    if ( v155 <= Count )
    {
      if ( v155 == Count )
      {
        fread(Src, 1u, Count, File);
      }
      else
      {
        auto _uSourceLen = malloc(v155);
        fread(_uSourceLen, v155, 1u, File);
        zlib::MemUnzip(Src, &Count, _uSourceLen, v155);
        free(_uSourceLen);
      }
    }
    else
    {
    MessageBoxW(nullptr, L"Can't load file!", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Polydata.cpp:1195", 0);
    }
    pData = ((char *)Src + 40);
    //v154 = 875;
    goto LABEL_140;
  }
  *(int *)pDest = 0;
LABEL_140:
  //v202 = (int)".blv";
  //v156 = strlen(pFilename);
  strcpy(&pFilename[strlen(pFilename) - 4], ".blv");
  memcpy(_visible_outlines, pData, 875);
  pData += 875;

  if ( *(int *)pDest )
    memcpy(_visible_outlines, v203, 875);

  for (uint i = 0; i < pMapOutlines->uNumOutlines; ++i)
  {
    auto pVertex = pMapOutlines->pOutlines + i;
    if ((unsigned __int8)(1 << (7 - i % 8)) & _visible_outlines[i / 8])
      pVertex->uFlags |= 1;
  }


  for (uint i = 0; i < uNumFaces; ++i)
  {
    auto pFace = pFaces + i;
    auto pFaceExtra = pFaceExtras + pFace->uFaceExtraID;

    memcpy(&pFace->uAttributes, pData, 4);
    pData += 4;

    if (pFaceExtra->uEventID)
    {
      if (pFaceExtra->HasEventint())
        pFace->uAttributes |= 0x100000;
      else
        pFace->uAttributes &= ~0x100000;
    }
  }

  pGameLoadingUI_ProgressBar->Progress();

  for (uint i = 0; i < uNumLevelDecorations; ++i)
  {
    memcpy(&pLevelDecorations[i].uFlags, pData, 2);
    pData += 2;
  }

  pGameLoadingUI_ProgressBar->Progress();

  memcpy(&uNumActors, pData, 4);
  memcpy(pActors.data(), pData + 4, uNumActors * sizeof(Actor));
  pData += 4 + uNumActors * sizeof(Actor);

  pGameLoadingUI_ProgressBar->Progress();
  pGameLoadingUI_ProgressBar->Progress();

  memcpy(&uNumSpriteObjects, pData, 4);
  memcpy(pSpriteObjects.data(), pData + 4, uNumSpriteObjects * sizeof(SpriteObject));
  pData += 4 + uNumSpriteObjects * sizeof(SpriteObject);

  pGameLoadingUI_ProgressBar->Progress();

  for (uint i = 0; i < uNumSpriteObjects; ++i)
  {
    auto pItem = &pSpriteObjects[i];
 
    if (pItem->stru_24.uItemID && !(pItem->uAttributes & 0x0100))
    {
      pItem->uType = pItemsTable->pItems[pItem->stru_24.uItemID - 1].uSpriteID;

      uint uObjectID = 0;
      for (uint j = 0; j < pObjectList->uNumObjects; ++j)
        if (pItem->uType == pObjectList->pObjects[j].uObjectID)
        {
          pItem->uObjectDescID = j;
          break;
        }
    }
  }

  pGameLoadingUI_ProgressBar->Progress();

  memcpy(&uNumChests, pData, 4);
  memcpy(pChests.data(), pData + 4, uNumChests * sizeof(Chest));
  pData += 4 + uNumChests * sizeof(Chest);

  pGameLoadingUI_ProgressBar->Progress();
  pGameLoadingUI_ProgressBar->Progress();

  memcpy(pDoors, pData, 0x3E80);
  pData += 0x3E80;

  //v201 = (const char *)blv.uDoors_ddata_Size;
  //v200 = (size_t)ptr_0002B4_doors_ddata;
  //v170 = malloc(ptr_0002B4_doors_ddata, blv.uDoors_ddata_Size, "L.DData");
  //v171 = blv.uDoors_ddata_Size;
  ptr_0002B4_doors_ddata = (unsigned __int16 *)malloc(blv.uDoors_ddata_Size);//, "L.DData");
  memcpy(ptr_0002B4_doors_ddata, pData, blv.uDoors_ddata_Size);
  pData += blv.uDoors_ddata_Size;

  //Src = (BLVFace *)((char *)Src + v171);
  //v172 = 0;
  //v245 = 0;
  //if (uNumDoors > 0)
  for (uint i = 0, j = 0; i < uNumDoors; ++i)
  {
    auto pDoor = pDoors + i;

    pDoor->pVertexIDs = ptr_0002B4_doors_ddata + j;
    j += pDoor->uNumVertices;

    pDoor->pFaceIDs = ptr_0002B4_doors_ddata + j;
    j += pDoor->uNumFaces;

    pDoor->pSectorIDs = ptr_0002B4_doors_ddata + j;
    j += pDoor->field_48;

    pDoor->pDeltaUs = (short *)(ptr_0002B4_doors_ddata + j);
    j += pDoor->uNumFaces;

    pDoor->pDeltaVs = (short *)(ptr_0002B4_doors_ddata + j);
    j += pDoor->uNumFaces;

    pDoor->pXOffsets = ptr_0002B4_doors_ddata + j;
    j += pDoor->uNumOffsets;

    pDoor->pYOffsets = ptr_0002B4_doors_ddata + j;
    j += pDoor->uNumOffsets;

    pDoor->pZOffsets = ptr_0002B4_doors_ddata + j;
    j += pDoor->uNumOffsets;
    /*v173 = pDoors;
    for ( k = 0; ; v172 = v188 + 2 * *((short *)&v173[k] - 3) )
    {
      v173[k].pVertexIDs = (unsigned __int16 *)((char *)ptr_0002B4_doors_ddata + v172);
      v175 = &pDoors[k];
      v176 = v172 + 2 * v175->uNumVertices;
      v175->pFaceIDs = (unsigned __int16 *)((char *)ptr_0002B4_doors_ddata + v176);
      v177 = &pDoors[k];
      v178 = v176 + 2 * v177->uNumFaces;
      v177->pSectorIDs = (unsigned __int16 *)((char *)ptr_0002B4_doors_ddata + v178);
      v179 = &pDoors[k];
      v180 = v178 + 2 * v179->field_48;
      v179->pDeltaUs = (__int16 *)((char *)ptr_0002B4_doors_ddata + v180);
      v181 = &pDoors[k];
      v182 = v180 + 2 * v181->uNumFaces;
      v181->pDeltaVs = (__int16 *)((char *)ptr_0002B4_doors_ddata + v182);
      v183 = &pDoors[k];
      v184 = v182 + 2 * v183->uNumFaces;
      v183->pXOffsets = (unsigned __int16 *)((char *)ptr_0002B4_doors_ddata + v184);
      v185 = &pDoors[k];
      v186 = v184 + 2 * v185->uNumOffsets;
      v185->pYOffsets = (unsigned __int16 *)((char *)ptr_0002B4_doors_ddata + v186);
      v187 = &pDoors[k];
      ++k;
      v188 = v186 + 2 * v187->uNumOffsets;
      v189 = (unsigned __int16 *)((char *)ptr_0002B4_doors_ddata + v188);
      ++v245;
      v187->pZOffsets = v189;
      v173 = pDoors;
      if ( v245 >= uNumDoors )
        break;
    }*/
  }
  //v190 = 0;
  //v245 = 0;
  for (uint i = 0; i < uNumDoors; ++i)
  {
    auto pDoor = pDoors + i;

    for (uint j = 0; j < pDoor->uNumFaces; ++j)
    {
      auto pFace = pFaces + pDoor->pFaceIDs[j];
      auto pFaceExtra = pFaceExtras + pFace->uFaceExtraID;

      pDoor->pDeltaUs[j] = pFaceExtra->sTextureDeltaU;
      pDoor->pDeltaVs[j] = pFaceExtra->sTextureDeltaV;
    }
    //v191 = pDoors;
    //pDest = 0;
    //do
   // {
      /*Argsn = 0;
      for ( l = (BLVDoor *)&v190[(int)v191];
            Argsn < *(short *)&v190[(int)v191 + 70];
            l = (BLVDoor *)&v190[(int)v191] )
      {
        v193 = Argsn;
        v194 = l->pFaceIDs[Argsn++];
        v195 = &pFaceExtras[pFaces[v194].uFaceExtraID];
        l->pDeltaUs[v193] = v195->sTextureDeltaU;
        v190 = pDest;
        *(short *)(v193 * 2 + *(int *)&pDest[(unsigned int)pDoors + 52]) = v195->sTextureDeltaV;
        v191 = pDoors;
      }
      ++v245;
      v190 += 80;
      pDest = v190;*/
    //}
    //while ( v245 < uNumDoors );
  }


  pGameLoadingUI_ProgressBar->Progress();

  memcpy(&stru_5E4C90_MapPersistVars, pData, 0xC8);
  pData += 0xC8;

  pGameLoadingUI_ProgressBar->Progress();

  memcpy(&stru1, pData, 0x38u);
  pData += 0x38;

  free(pRawDLV);
  //v5 = 0;

  pSoundList->LoadSound(64, 0);
  pSoundList->LoadSound(103, 0);
  pSoundList->LoadSound(63, 0);
  pSoundList->LoadSound(102, 0);
  pSoundList->LoadSound(50, 0);
  pSoundList->LoadSound(89, 0);

  return 0;
}


//----- (0049AC17) --------------------------------------------------------
int IndoorLocation::GetSector(int sX, int sY, int sZ)
{
  int v4; // esi@1
  //unsigned __int8 v5; // zf@1
  //unsigned __int8 v6; // sf@1
  //unsigned __int8 v7; // of@1
  //BLVSector *v8; // eax@3
  //int v9; // edi@9
  //int v10; // eax@9
  //int v11; // edx@9
  //int v12; // eax@10
  //unsigned __int16 v13; // ax@12
  //int v14; // edi@14
  //BLVFace *v15; // eax@14
  //BLVFace *v16; // edi@14
  //PolygonType v17; // al@14
  //unsigned __int16 *pVertexIDs; // esi@16
  //Vec3_short_ *v19; // eax@16
  //int v20; // edx@16
  //unsigned __int16 *v21; // esi@17
  //int v22; // edi@19
  //int v23; // edi@20
  //Vec3_short_ *v24; // edx@20
  signed int v25; // edx@21
  int v26; // ebx@23
  //int v27; // edx@26
  //signed __int64 v28; // qtt@26
  ///Vec3_short_ *v29; // eax@26
  //Vec3_short_ *v30; // edx@26
  //int v31; // edx@26
  //signed int v32; // edi@27
  signed __int64 v33; // qtt@27
  //Vec3_short_ *v34; // edx@27
  //int v35; // edx@32
  int v37; // edi@38
  int pSectorID; // ebx@40
  int v39; // eax@41
  BLVFace *pFace; // esi@42
  PolygonType pPolygonType; // dl@42
  int v42; // edx@43
  int v43[50]; // [sp+Ch] [bp-108h]@1
  //int v44; // [sp+D4h] [bp-40h]@9
  //int v45; // [sp+D8h] [bp-3Ch]@14
  //int v46; // [sp+DCh] [bp-38h]@14
  //int v47; // [sp+E0h] [bp-34h]@19
  //int v48; // [sp+E4h] [bp-30h]@9
  //Vec3_short_ *v49; // [sp+E8h] [bp-2Ch]@19
  bool v50; // [sp+ECh] [bp-28h]@19
  int v51; // [sp+F0h] [bp-24h]@9
  //unsigned int v52; // [sp+F4h] [bp-20h]@2
  int v53; // [sp+F8h] [bp-1Ch]@10
  int v54; // [sp+FCh] [bp-18h]@16
  int v55; // [sp+100h] [bp-14h]@1
  int v56; // [sp+104h] [bp-10h]@1
  int v57; // [sp+108h] [bp-Ch]@16
  //Vec3_short_ *v58; // [sp+10Ch] [bp-8h]@20
  int v59; // [sp+110h] [bp-4h]@16

  v4 = 0;
  v43[0] = 0;

  v55 = 0;

  if (uNumSectors < 2)
    return 0;

  for (uint i = 1; i < uNumSectors; ++i)
  {
    auto pSector = pSectors + i;

    if (pSector->pBounding.x1 > sX || pSector->pBounding.x2 < sX ||
        pSector->pBounding.y1 > sY || pSector->pBounding.y2 < sY ||
        pSector->pBounding.z1 - 64 > sZ || pSector->pBounding.z2 + 64 < sZ)
      continue;

          //Log::Warning(L"Sector[%u]", i);
    v51 = pSector->uNumFloors + pSector->uNumPortals;
    if (!v51)
      continue;


    for (uint j = 0; j < v51; ++j)
    {
      uint uFaceID;
      if (j < pSector->uNumFloors)
        uFaceID = pSector->pFloors[j];
      else
        uFaceID = pSector->pPortals[j - pSector->uNumFloors];

      auto pFace = pFaces + uFaceID;
      if (pFace->uPolygonType != POLYGON_Floor &&
          pFace->uPolygonType != POLYGON_InBetweenFloorAndWall)
        continue;

      v54 = 0;
      v50 = pVertices[pFace->pVertexIDs[0]].y >= sY;

      for (uint k = 1; k <= pFace->uNumVertices; k++)
      {
        v59 = v50;

        if (v54 >= 2)
          break;

        auto v2 = &pVertices[pFace->pVertexIDs[k]];
        v50 = v2->y >= sY;

        if (v59 == v50)
          continue;

        auto v1 = &pVertices[pFace->pVertexIDs[k - 1]];
        v25 = v2->x >= sX ? 0 : 2;
        v26 = v25 | (v1->x < sX);

        if (v26 == 3)
          continue;

        if (!v26)
          ++v54;
        else
        {
          if (v1->x >= v2->x)
          {
                        /*int _a58;
                        int _a59;

                        v32 = v1->x - v2->x;
                        LODWORD(v33) = v32 << 16;
                        HIDWORD(v33) = v32 >> 16;*/
                        //fixpoint_div(v1->x - v2->x, v1->y - v2->y);
                        //_a58 = v33 / (v1->y - v2->y);
                        //_a59 = fixpoint_mul(_a58, sY - v2->y);
            auto x_div_y = fixpoint_div(v1->x - v2->x, v1->y - v2->y);
            auto res = fixpoint_mul(x_div_y, sY - v2->y);  // a / b * c  - looks like projection 
            if (res + v2->x > sX)
              ++v54;
          }
          else
          {
            auto x_div_y = fixpoint_div(v2->x - v1->x, v2->y - v1->y);
            auto res = fixpoint_mul(x_div_y, sY - v1->y);

            if (res + v1->x > sX)
              ++v54;

                        /*int _a58;
                        int _a59;
                        auto v32 = v2->x - v1->x;
                        LODWORD(v33) = v32 << 16;
                        HIDWORD(v33) = v32 >> 16;
                        _a58 = v33 / (v2->y - v1->y);
                        _a59 = fixpoint_mul(_a58, sY - v1->y);

                        if (_a59 + pVertices[k].x > sX)
                          ++v54;*/
          }
        }
      }

      if (pFace->uNumVertices && v54 == 1)
        v43[v55++] = uFaceID;
    }
  }

  v4 = v43[0];
  if ( v55 == 1 )
    return this->pFaces[v4].uSectorID;
  v37 = 0;
  if ( !v55 )
    return 0;
  pSectorID = 0;
  v53 = 0xFFFFFFu;
  if ( v55 > 0 )
  {
    v39 = sY;
    while ( 1 )
    {
      pFace = &this->pFaces[v43[v37]];
      //pPolygonType = pFace->uPolygonType;
      if ( pFace->uPolygonType == POLYGON_Floor )
      {
        v42 = this->pVertices[*pFace->pVertexIDs].z;
        v39 = sZ - v42;
        //goto LABEL_47;
      }
      if ( pFace->uPolygonType == POLYGON_InBetweenFloorAndWall )
      {
        //v51 = pFace->zCalc1;
        v57 = (unsigned __int64)(pFace->zCalc1 * (signed __int64)(sX << 16)) >> 16;
        //v56 = sY << 16;
        //v51 = pFace->zCalc2;
        v56 = (unsigned __int64)(pFace->zCalc2 * (signed __int64)(sY << 16)) >> 16;
        v42 = (v56 + pFace->zCalc3 + v57 + 32768) >> 16;
        v39 = sZ - v42;
      }
//LABEL_47:
      if ( v39 >= 0 )
      {
        if ( v39 < v53 )
        {
          pSectorID = pFace->uSectorID;
          v53 = v39;
        }
      }
      ++v37;
      if ( v37 >= v55 )
        return pSectorID;
    }
  }
  return pSectorID;
}
// 49AC17: using guessed type int var_108[50];


//----- (00498A41) --------------------------------------------------------
char BLVFace::_get_normals(Vec3_int_ *a2, Vec3_int_ *a3)
{
  BLVFace *v3; // ebx@1
  int v4; // eax@1
  signed __int64 v5; // qax@2
  double v6; // st7@8
  Vec3_float_ a1; // [sp+Ch] [bp-Ch]@8

  v3 = this;
  LOBYTE(v4) = this->uPolygonType;
  if ( (char)v4 == POLYGON_VerticalWall )
  {
    a2->x = -this->pFacePlane_old.vNormal.y;
    LODWORD(v5) = this->pFacePlane_old.vNormal.x;
LABEL_9:
    a2->y = v5;
    v4 = 0;
    a2->z = 0;
    a3->y = 0;
    a3->z = 0xFFFF0000u;
LABEL_11:
    a3->x = v4;
    goto LABEL_12;
  }
  if ( (char)v4 == POLYGON_Floor || (char)v4 == POLYGON_Ceiling )
  {
LABEL_10:
    v4 = 0;
    a2->x = 0x10000u;
    a2->z = 0;
    a2->y = 0;
    a3->y = 0xFFFF0000u;
    a3->z = 0;
    goto LABEL_11;
  }
  if ( (char)v4 == POLYGON_InBetweenFloorAndWall || (char)v4 == POLYGON_InBetweenCeilingAndWall )
  {
    if ( abs(this->pFacePlane_old.vNormal.z) < 46441 )
    {
      a1.x = (double)-v3->pFacePlane_old.vNormal.y;
      a1.y = (double)v3->pFacePlane_old.vNormal.x;
      a1.z = 0.0;
      a1.Normalize();
      v6 = a1.y * 65536.0;
      a2->x = (signed __int64)(a1.x * 65536.0);
      v5 = (signed __int64)v6;
      goto LABEL_9;
    }
    goto LABEL_10;
  }
LABEL_12:
  if ( BYTE2(v3->uAttributes) & 0x80 )
  {
    a2->x = -a2->x;
    a2->y = -a2->y;
    a2->z = -a2->z;
  }
  if ( BYTE3(v3->uAttributes) & 1 )
  {
    a3->x = -a3->x;
    a3->y = -a3->y;
    a3->z = -a3->z;
  }
  return v4;
}

//----- (0044C23B) --------------------------------------------------------
bool BLVFaceExtra::HasEventint()
	{
   signed int event_index; // eax@1
  _evt_raw* start_evt;
  _evt_raw* end_evt;

  event_index = 0;
  if ( (uLevelEVT_NumEvents - 1) <= 0 )
    return false;
  while ( pLevelEVT_Index[event_index].uEventID != this->uEventID )
  {
    ++event_index;
    if ( event_index >= (signed int)(uLevelEVT_NumEvents - 1) )
      return false;
  }
  end_evt=(_evt_raw*)&pLevelEVT[pLevelEVT_Index[event_index+1].uEventOffsetInEVT];
  start_evt=(_evt_raw*)&pLevelEVT[pLevelEVT_Index[event_index].uEventOffsetInEVT];
  if ( (end_evt->_e_type != EVENT_Exit) || (start_evt->_e_type!= EVENT_MouseOver) )
    return false;
  else
    return true;
}




//----- (0046F228) --------------------------------------------------------
void  BLV_UpdateDoors()
{
  //int v0; // ebx@1
  //int v1; // edi@1
  //BLVDoor *v2; // esi@3
  //unsigned __int16 v3; // ax@3
  //unsigned int v4; // ecx@5
  //int v5; // eax@8
  //int v6; // ecx@8
  //int v7; // eax@12
  //int v8; // eax@16
  //unsigned __int8 v9; // zf@18
  //char v10; // sf@18
  //unsigned __int8 v11; // of@18
  //int v12; // edi@19
  //int v13; // ecx@19
  //__int16 v14; // ax@19
  BLVFace *v15; // ebx@24
  //unsigned __int16 *v16; // ecx@24
  Vec3_short_ *v17; // esi@24
  int v18; // eax@24
  int v19; // edx@24
  signed int v20; // eax@24
  //Vec3_short_ *v21; // ecx@24
  //double v22; // st7@24
  //double v23; // st6@24
  int v24; // esi@25
  int v25; // eax@25
  //BLVDoor *v26; // edi@25
  signed __int64 v27; // qtt@27
  BLVFaceExtra *v28; // esi@32
  int v29; // ecx@34
  int v30; // edx@34
  unsigned __int64 v31; // qax@34
  int v32; // eax@34
  //unsigned __int16 *v33; // eax@35
  Vec3_short_ *v34; // eax@35
  int v35; // ecx@35
  int v36; // edx@35
  signed int v37; // eax@35
  signed int v38; // edx@35
  int v39; // eax@35
  int v40; // edx@35
  //unsigned __int8 v41; // cf@35
  //unsigned __int16 *v42; // edi@36
  Vec3_short_ *v43; // edi@36
  //int v44; // ecx@36
  //int v45; // edi@36
  //int v46; // ecx@36
  //__int16 *v47; // edx@44
  //int v48; // ecx@44
  unsigned int v49; // ecx@46
  unsigned __int16 v50; // ax@48
  unsigned int v51; // eax@51
  unsigned __int16 v52; // ax@54
  int v53; // ecx@57
  //int v54; // edx@57
  unsigned __int64 v55; // qax@57
  int v56; // ecx@58
  int v57; // eax@58
  //int v58; // eax@59
  //SoundID v59; // [sp-24h] [bp-88h]@12
  //signed int v60; // [sp-20h] [bp-84h]@12
  //unsigned int v61; // [sp-1Ch] [bp-80h]@12
  //signed int v62; // [sp-18h] [bp-7Ch]@12
  //signed int v63; // [sp-14h] [bp-78h]@12
  //int v64; // [sp-10h] [bp-74h]@12
  //unsigned int v65; // [sp-Ch] [bp-70h]@12
  //int v66; // [sp-8h] [bp-6Ch]@12
  Vec3_int_ v67;
  //int v67; // [sp+8h] [bp-5Ch]@31
  //int v68; // [sp+Ch] [bp-58h]@34
  //int v69; // [sp+10h] [bp-54h]@34
  Vec3_int_ v70;
  //int v70; // [sp+14h] [bp-50h]@31
  //int v71; // [sp+18h] [bp-4Ch]@34
  //int v72; // [sp+1Ch] [bp-48h]@34
  int v73; // [sp+20h] [bp-44h]@24
  //__int16 v74; // [sp+24h] [bp-40h]@24
  int v75; // [sp+28h] [bp-3Ch]@36
  int v76; // [sp+2Ch] [bp-38h]@36
  int v77; // [sp+30h] [bp-34h]@36
  //int v78; // [sp+34h] [bp-30h]@36
  //int v79; // [sp+38h] [bp-2Ch]@19
  //unsigned int v80; // [sp+3Ch] [bp-28h]@2
  //int v81; // [sp+40h] [bp-24h]@1
  int v82; // [sp+44h] [bp-20h]@35
  int v83; // [sp+48h] [bp-1Ch]@34
  int v84; // [sp+4Ch] [bp-18h]@34
  int v85; // [sp+50h] [bp-14h]@19
  SoundID eDoorSoundID; // [sp+54h] [bp-10h]@1
  //BLVDoor *v87; // [sp+58h] [bp-Ch]@3
  int v88; // [sp+5Ch] [bp-8h]@18
  int v89; // [sp+60h] [bp-4h]@6

  //v0 = 0;
  //v1 = 0;
  eDoorSoundID = (SoundID)pDoorSoundIDsByLocationID[dword_6BE13C_uCurrentlyLoadedLocationID];
  //v81 = 0;
  //if ( pIndoor->uNumDoors > 0 )
  for (uint i = 0; i < pIndoor->uNumDoors; ++i)
  {
    auto door = pIndoor->pDoors + i;
    //v80 = 0;
    //do
    //{
      //v2 = &pIndoor->pDoors[v80 / 0x50];
      //v87 = &pIndoor->pDoors[v80 / 0x50];
      //v3 = door->uState;
      if (door->uState == BLVDoor::Closed || door->uState == BLVDoor::Open)
      {
        door->uAttributes &= 0xFFFFFFFDu;
        goto LABEL_62;
      }
      door->uTimeSinceTriggered += pEventTimer->uTimeElapsed;
      //v4 = door->uTimeSinceTriggered;
      if (door->uState == BLVDoor::Opening)
      {
        v89 = (signed int)(door->uTimeSinceTriggered * door->uCloseSpeed) / 128;
        if ( v89 >= door->uMoveLength )
        {
          v89 = door->uMoveLength;
          door->uState = BLVDoor::Open;
LABEL_10:
          if ( !(door->uAttributes & 6) && door->uNumVertices != 0)
          {
            //v66 = 0;
            //v65 = 0;
            //v64 = 0;
            //v7 = PID(OBJECT_BLVDoor,i);
            //v63 = 0;
            //v62 = -1;
            //LOBYTE(v7) = PID(OBJECT_BLVDoor,i);
            //v61 = 0;
            //v60 = PID(OBJECT_BLVDoor,i);
            //v59 = (SoundID)((int)eDoorSoundID + 1);
//LABEL_17:
            pAudioPlayer->PlaySound((SoundID)((int)eDoorSoundID + 1), PID(OBJECT_BLVDoor,i), 0, -1, 0, 0, 0, 0);
            //goto LABEL_18;
          }
          goto LABEL_18;
        }
      }
      else
      {
        auto v5 = (signed int)(door->uTimeSinceTriggered * door->uOpenSpeed) / 128;
        //v6 = door->uMoveLength;
        if ( v5 >= door->uMoveLength)
        {
          v89 = 0;
          door->uState = BLVDoor::Closed;
          goto LABEL_10;
        }
        v89 = door->uMoveLength - v5;
      }
      if ( !(door->uAttributes & 6) && door->uNumVertices)
      {
        //v66 = 0;
        //v65 = 0;
        //v64 = 0;
        //v8 = PID(OBJECT_BLVDoor,i);
        //v63 = 0;
        //v62 = -1;
        //LOBYTE(v8) = 8 * v1 | 1;
        //v61 = 1;
        //v60 = PID(OBJECT_BLVDoor,i);
        //v59 = eDoorSoundID;
        pAudioPlayer->PlaySound(eDoorSoundID, PID(OBJECT_BLVDoor,i), 1, -1, 0, 0, 0, 0);
        //goto LABEL_18;
      }
LABEL_18:
      //v11 = __OFSUB__(v2->uNumVertices, 0);
      //v9 = v2->uNumVertices == 0;
      //v10 = (signed __int16)(v2->uNumVertices - 0) < 0;
      //v88 = 0;
      //if (door->uNumVertices > 0)
      for (uint j = 0; j < door->uNumVertices; ++j)
      {
        //do
        //{
          //v12 = v88;
          //v13 = door->pVertexIDs[v88];
          //v85 = v89;
          //v79 = door->vDirection.x;
          //v85 = (unsigned __int64)(v79 * (signed __int64)v89) >> 16;
          //v13 *= 6;
          pIndoor->pVertices[door->pVertexIDs[j]].x = ((unsigned int)(door->vDirection.x * v89) >> 16) + door->pXOffsets[j];
          //v85 = v89;
          //v79 = door->vDirection.y;
          //v85 = (unsigned __int64)(v79 * (signed __int64)v89) >> 16;
          pIndoor->pVertices[door->pVertexIDs[j]].y = ((unsigned int)(door->vDirection.y * v89) >> 16) + door->pYOffsets[j];
          //v85 = v89;
          //v79 = door->vDirection.z;
          //v85 = (unsigned __int64)(v79 * (signed __int64)v89) >> 16;
          //v14 = ((unsigned int)(door->vDirection.z * v89) >> 16) + door->pZOffsets[j];
          pIndoor->pVertices[door->pVertexIDs[j]].z = ((unsigned int)(door->vDirection.z * v89) >> 16) + door->pZOffsets[j];
          //++v88;
        //}
        //while ( v88 < door->uNumVertices );
        //v1 = v81;
      }
      //v11 = __OFSUB__(v2->uNumFaces, 0);
      //v9 = v2->uNumFaces == 0;
      //v10 = (signed __int16)(v2->uNumFaces - 0) < 0;
      v88 = 0;
      if (door->uNumFaces > 0)
      {
        while ( 1 )
        {
          v15 = &pIndoor->pFaces[door->pFaceIDs[v88]];
          //v16 = v15->pVertexIDs;
          v17 = &pIndoor->pVertices[v15->pVertexIDs[0]];
          v18 = v15->pFacePlane_old.vNormal.y;
          v73 = *(int *)&v17->x;
          //v74 = v17->z;
          v19 = v15->pFacePlane_old.vNormal.z;
          v20 = -(v19 * (int)v17->z + (signed __int16)v73 * v15->pFacePlane_old.vNormal.x + SHIWORD(v73) * v18);
          v15->pFacePlane_old.dist = v20;
          //v21 = &pIndoor->pVertices[v15->pVertexIDs[0]];
          //v79 = v21->x;
          //v22 = (double)v21->x;
          //v23 = (double)v21->y;
          //v79 = v21->z;
          v15->pFacePlane.dist = -((double)v17->z * v15->pFacePlane.vNormal.z
                                 + (double)v17->y * v15->pFacePlane.vNormal.y
                                 + (double)v17->x * v15->pFacePlane.vNormal.x);
          if ( v19 )
          {
            v24 = abs(v20 >> 15);
            v25 = abs(v15->pFacePlane_old.vNormal.z);
            //v26 = v87;
            if ( v24 > v25 )
              Error("Door Error\ndoor id: %i\nfacet no: %i\n\nOverflow dividing facet->d [%i] by facet->nz [%i]",
                door->uDoorID,
                door->pFaceIDs[v88],
                v15->pFacePlane_old.dist,
                v15->pFacePlane_old.vNormal.z);
            //v79 = v15->pFacePlane_old.vNormal.z;
            //v85 = v15->pFacePlane_old.dist;
            LODWORD(v27) = v15->pFacePlane_old.dist << 16;
            HIDWORD(v27) = v15->pFacePlane_old.dist >> 16;
            //v85 = v27 / v15->pFacePlane_old.vNormal.z;
            v15->zCalc3 = -v27 / v15->pFacePlane_old.vNormal.z;
          }
          /*else
          {
            v26 = v87;
          }*/
          if ( BYTE2(v15->uAttributes) & 4 || pRenderer->pRenderD3D )
            v15->_get_normals(&v70, &v67);
          v28 = &pIndoor->pFaceExtras[v15->uFaceExtraID];
          if ( !pRenderer->pRenderD3D )
          {
            if ( !(BYTE2(v15->uAttributes) & 4) )
              goto LABEL_59;
            //v79 = door->vDirection.x;
            v83 = (unsigned __int64)(door->vDirection.x * (signed __int64)v70.x) >> 16;
            //v85 = v71;
            //v79 = door->vDirection.y;
            v85 = (unsigned __int64)(door->vDirection.y * (signed __int64)v70.y) >> 16;
            //v84 = v72;
            //v79 = door->vDirection.z;
            v84 = (unsigned __int64)(door->vDirection.z * (signed __int64)v70.z) >> 16;
            //v79 = v83 + v85 + v84;
            v29 = v89;
            //v83 = (unsigned __int64)(v79 * (signed __int64)v89) >> 16;
            v28->sTextureDeltaU = -((v83 + v85 + v84) * (signed __int64)v89) >> 16;
            //v79 = door->vDirection.x;
            v85 = (unsigned __int64)(door->vDirection.x * (signed __int64)v67.x) >> 16;
            //v83 = v68;
            //v79 = door->vDirection.y;
            v83 = (unsigned __int64)(door->vDirection.y * (signed __int64)v67.y) >> 16;
            //v84 = v69;
            //v79 = door->vDirection.z;
            v84 = (unsigned __int64)(door->vDirection.z * (signed __int64)v67.z) >> 16;
            //v30 = v83 + v84;
            //v79 = v85 + v30;
            v31 = (v85 + v83 + v84) * (signed __int64)v29;
            //v83 = v31 >> 16;
            v32 = v31 >> 16;
            goto LABEL_58;
          }
          v28->sTextureDeltaU = 0;
          v28->sTextureDeltaV = 0;
          //v33 = v15->pVertexIDs;
          //v85 = 1;
          v34 = &pIndoor->pVertices[v15->pVertexIDs[0]];
          v35 = v34->z;
          v36 = v34->y;
          v82 = v34->x;
          //v79 = v36;
          v37 = v70.x * v82 + v70.y * v36 + v70.z * v35;
          v38 = v67.x * v82 + v67.y * v36 + v67.z * v35;
          v39 = v37 >> 16;
          *v15->pVertexUIDs = v39;
          v40 = v38 >> 16;
          *v15->pVertexVIDs = v40;
          //v41 = v15->uNumVertices < 1u;
          //v9 = v15->uNumVertices == 1;
          //v83 = v40;
          v84 = v39;
          v82 = v40;
          //if (v15->uNumVertices > 1)
          for (uint j = 1; j < v15->uNumVertices; ++j)
          {
            //do
            //{
              //v42 = v15->pVertexIDs;
              //v75 = 2 * v85;
              v43 = &pIndoor->pVertices[v15->pVertexIDs[j]];
              //v79 = v43->z;
              //v44 = v43->y;
              //v45 = v43->x;
              //v78 = v43->y;
              v76 = ((__int64)v70.z * v43->z + (__int64)v70.x * v43->x + (__int64)v70.y * v43->y) >> 16;
              //v46 = (v67 * v45 + v68 * v44 + v79 * v69) >> 16;
              v77 = ((__int64)v67.x * v43->x + (__int64)v67.y * v43->y + (__int64)v43->z * v67.z) >> 16;
              if ( v76 < v39 )
                v39 = v76;
              if ( v77 < v40 )
                v40 = v77;
              if ( v76 > v84 )
                v84 = v76;
              if ( v77 > v82 )
                v82 = v77;
              //v40 = v83;
              //v47 = v15->pVertexUIDs;
              //v48 = v75;
              //++v85;
              v15->pVertexUIDs[j] = v76;
              v15->pVertexVIDs[j] = v77;
            //}
            //while ( v85 < v15->uNumVertices );
            //v26 = v87;
          }
          v49 = v15->uAttributes;
          if ( BYTE1(v49) & 0x10 )
            goto LABEL_50;
          if ( SBYTE1(v49) < 0 )
          {
            v50 = v15->uBitmapID;
            if ( v50 != -1 )
              break;
          }
LABEL_51:
          v51 = v15->uAttributes;
          if ( v51 & 8 )
          {
            v28->sTextureDeltaV -= v40;
          }
          else
          {
            if ( v51 & 0x20000 )
            {
              v52 = v15->uBitmapID;
              if ( v52 != -1 )
                v28->sTextureDeltaV -= v82 + pBitmaps_LOD->GetTexture(v52)->uTextureHeight;
            }
          }
          if ( BYTE2(v15->uAttributes) & 4 )
          {
            //v75 = door->vDirection.x;
            v84 = (unsigned __int64)(door->vDirection.x * (signed __int64)v70.x) >> 16;
            //v82 = v71;
            //v75 = door->vDirection.y;
            v82 = (unsigned __int64)(door->vDirection.y * (signed __int64)v70.y) >> 16;
            //v83 = v72;
            //v75 = door->vDirection.z;
            v83 = (unsigned __int64)(door->vDirection.z * (signed __int64)v70.z) >> 16;
            v75 = v84 + v82 + v83;
            v53 = v89;
            v82 = (unsigned __int64)(v75 * (signed __int64)v89) >> 16;
            v28->sTextureDeltaU = -v82;
            //v75 = door->vDirection.x;
            v84 = (unsigned __int64)(door->vDirection.x * (signed __int64)v67.x) >> 16;
            //v82 = v68;
            //v75 = door->vDirection.y;
            v82 = (unsigned __int64)(door->vDirection.y * (signed __int64)v67.y) >> 16;
            //v83 = v69;
            //v75 = door->vDirection.z;
            v83 = (unsigned __int64)(door->vDirection.z * (signed __int64)v67.z) >> 16;
            //v54 = v82 + v83;
            v75 = v84 + v82 + v83;
            v55 = v75 * (signed __int64)v53;
            //v82 = v55 >> 16;
            v32 = v55 >> 16;
LABEL_58:
            v56 = v88;
            v57 = -v32;
            v28->sTextureDeltaV = v57;
            v28->sTextureDeltaU += door->pDeltaUs[v56];
            v28->sTextureDeltaV = v57 + door->pDeltaVs[v56];
          }
LABEL_59:
          //v58 = door->uNumFaces;
          ++v88;
          if ( v88 >= door->uNumFaces)
          {
            //v1 = v81;
            //v0 = 0;
            goto LABEL_62;
          }
          //v2 = v87;
        }
        LOWORD(v39) = v84;
        if (v15->uBitmapID != -1)
          LOWORD(v39) += pBitmaps_LOD->pTextures[v15->uBitmapID].uTextureWidth;
LABEL_50:
        v28->sTextureDeltaU -= v39;
        goto LABEL_51;
      }
LABEL_62:
      ;
      //v80 += 80;
      //++v1;
      //v81 = v1;
    //}
    //while ( v1 < pIndoor->uNumDoors );
  }
}
// 6BE13C: using guessed type int dword_6BE13C_uCurrentlyLoadedLocationID;

//----- (0046F90C) --------------------------------------------------------
void  UpdateActors_BLV()
{
  Actor *v0; // esi@2
  unsigned __int16 v1; // ax@2
  int v2; // edi@6
  int v3; // eax@6
  int v4; // eax@8
  __int16 v5; // ax@11
  signed int v6; // ebx@14
  unsigned __int8 v7; // zf@14
  unsigned __int8 v8; // sf@14
  signed __int16 v9; // ax@17
  signed __int64 v10; // qax@18
  unsigned __int16 v11; // ax@21
  int v12; // eax@29
  unsigned __int64 v13; // qax@29
  int v14; // eax@30
  unsigned __int64 v15; // qax@30
  int v16; // ecx@33
  BLVFace *v17; // edx@33
  int v18; // ecx@33
  BLVFace *v19; // eax@34
  int v20; // ecx@46
  //int v21; // eax@46
  int v22; // edi@46
  int v23; // eax@48
  unsigned int v24; // eax@51
  int v25; // eax@52
  int v26; // ebx@54
  int v27; // ST08_4@54
  int v28; // edi@54
  int v29; // eax@54
  int v30; // ecx@62
  int v31; // ebx@62
  int v32; // eax@62
  int v33; // eax@64
  unsigned int v34; // ecx@64
  int v35; // ecx@64
  signed int v36; // edx@85
  signed int v37; // ebx@85
  BLVFace *v38; // edi@89
  int v39; // ecx@90
  int v40; // ebx@90
  PolygonType v41; // al@94
  int v42; // eax@96
  __int16 v43; // dx@96
  int v44; // ecx@96
  int v45; // edi@101
  //int v46; // edi@101
  //int v47; // eax@101
  //unsigned __int64 v48; // qax@101
  unsigned __int8 v49; // zf@103
  unsigned __int8 v50; // sf@103
  unsigned __int8 v51; // of@103
  AIDirection v52; // [sp+0h] [bp-60h]@75
  AIDirection v53; // [sp+1Ch] [bp-44h]@116
  int v54; // [sp+38h] [bp-28h]@53
  unsigned int uSectorID; // [sp+3Ch] [bp-24h]@6
  int v56; // [sp+40h] [bp-20h]@6
  unsigned int _this; // [sp+44h] [bp-1Ch]@51
  int v58; // [sp+48h] [bp-18h]@51
  int v59; // [sp+4Ch] [bp-14h]@8
  unsigned int uFaceID; // [sp+50h] [bp-10h]@6
  int v61; // [sp+54h] [bp-Ch]@14
  int v62; // [sp+58h] [bp-8h]@6
  unsigned int v63; // [sp+5Ch] [bp-4h]@1

  v63 = 0;
  if ( (signed int)uNumActors > 0 )
  {
    while ( 1 )
    {
      v0 = &pActors[v63];
      v1 = v0->uAIState;
      if ( v1 == 11 || v1 == 19 || v1 == 17 || !v0->uMovementSpeed )
        goto LABEL_123;
      uSectorID = v0->uSectorID;
      v2 = collide_against_floor(v0->vPosition.x, v0->vPosition.y, v0->vPosition.z, &uSectorID, &uFaceID);
      v0->uSectorID = uSectorID;
      v3 = v0->pMonsterInfo.uFlying;
      v56 = v2;
      v62 = v3;
      if ( !v0->CanAct() )
        v62 = 0;
      v4 = v0->vPosition.z;
      v59 = 0;
      if ( v4 > v2 + 1 )
        v59 = 1;
      if ( v2 <= -30000 )
      {
        v5 = pIndoor->GetSector(v0->vPosition.x, v0->vPosition.y, v4);
        v0->uSectorID = v5;
        if ( !v5
          || (v56 = BLV_GetFloorLevel(v0->vPosition.x, v0->vPosition.y, v0->vPosition.z, v5, &uFaceID), v56 == -30000) )
          goto LABEL_123;
      }
      if ( v0->uCurrentActionAnimation == ANIM_Walking)
      {
        v6 = v0->uMovementSpeed;
        v7 = HIDWORD(v0->pActorBuffs[7].uExpireTime) == 0;
        v8 = SHIDWORD(v0->pActorBuffs[7].uExpireTime) < 0;
        v61 = v0->uMovementSpeed;
        if ( !v8 && (!(v8 | v7) || LODWORD(v0->pActorBuffs[7].uExpireTime)) )
        {
          v9 = v0->pActorBuffs[7].uPower;
          if ( v9 )
            LODWORD(v10) = v6 / (unsigned __int16)v9;
          else
            v10 = (signed __int64)((double)v61 * 0.5);
          v6 = v10;
          v61 = v10;
        }
        v11 = v0->uAIState;
        if ( v11 == 6 || v11 == 7 )
        {
          v6 *= 2;
          v61 = v6;
        }
        if ( pParty->bTurnBasedModeOn == 1 && pTurnEngine->turn_stage == 1 )
          v6 = (signed __int64)((double)v61 * flt_6BE3AC_debug_recmod1_x_1_6);
        if ( v6 > 1000 )
          v6 = 1000;
        v12 = stru_5C6E00->Cos(v0->uYawAngle);
        uSectorID = v12;
        v13 = v12 * (signed __int64)v6;
        v61 = v13 >> 16;
        v0->vVelocity.x = WORD1(v13);
        uSectorID = stru_5C6E00->Sin(v0->uYawAngle);
        v61 = (unsigned __int64)(uSectorID * (signed __int64)v6) >> 16;
        v7 = v62 == 0;
        v0->vVelocity.y = (unsigned int)(uSectorID * v6) >> 16;
        if ( !v7 )
        {
          v14 = stru_5C6E00->Sin(v0->uPitchAngle);
          uSectorID = v14;
          v15 = v14 * (signed __int64)v6;
          v61 = v15 >> 16;
          v0->vVelocity.z = WORD1(v15);
        }
      }
      else
      {
        v61 = v0->vVelocity.x;
        uSectorID = 55000;
        v61 = (unsigned __int64)(55000i64 * v61) >> 16;
        v0->vVelocity.x = v61;
        v61 = v0->vVelocity.y;
        v61 = (unsigned __int64)(uSectorID * (signed __int64)v61) >> 16;
        v7 = v62 == 0;
        v0->vVelocity.y = v61;
        if ( !v7 )
        {
          uSectorID = 55000;
          v61 = v0->vVelocity.z;
          v61 = (unsigned __int64)(55000i64 * v61) >> 16;
          v0->vVelocity.z = v61;
        }
      }
      v16 = v0->vPosition.z;
      v17 = pIndoor->pFaces;
      v51 = __OFSUB__(v16, v56);
      v8 = v16 - v56 < 0;
      v18 = uFaceID;
      if ( v8 ^ v51 )
      {
        v0->vPosition.z = v56 + 1;
        v19 = &v17[v18];
        if ( v19->uPolygonType == 3 )
        {
          if ( v0->vVelocity.z < 0 )
            v0->vVelocity.z = 0;
        }
        else
        {
          if ( v19->pFacePlane_old.vNormal.z < 45000 )
            v0->vVelocity.z -= LOWORD(pEventTimer->uTimeElapsed) * GetGravityStrength();
        }
      }
      else
      {
        if ( v59 && !v62 )
          v0->vVelocity.z += -8 * LOWORD(pEventTimer->uTimeElapsed) * GetGravityStrength();
      }
      if ( v0->vVelocity.x * v0->vVelocity.x + v0->vVelocity.y * v0->vVelocity.y + v0->vVelocity.z * v0->vVelocity.z >= 400 )
        break;
      v0->vVelocity.z = 0;
      v0->vVelocity.y = 0;
      v0->vVelocity.x = 0;
      if ( BYTE2(v17[v18].uAttributes) & 0x40 )
      {
        if (v0->uAIState == Dead)
          v0->uAIState = Removed;
      }
LABEL_123:
      ++v63;
      if ( (signed int)v63 >= (signed int)uNumActors )
        return;
    }
    v20 = v0->uActorRadius;
    stru_721530.field_84 = -1;
    stru_721530.field_70 = 0;
    uSectorID = 0;
    stru_721530.field_0 = 1;
    stru_721530.field_8_radius = v20;
    stru_721530.prolly_normal_d = v20;
    stru_721530.height = v0->uActorHeight;
    v22 = 0;
    while ( 1 )
    {
      stru_721530.position.x = v0->vPosition.x;
      stru_721530.normal.x = stru_721530.position.x;
      stru_721530.position.y = v0->vPosition.y;
      stru_721530.normal.y = stru_721530.position.y;
      v23 = v0->vPosition.z;
      stru_721530.normal.z = v23 + v20 + 1;
      stru_721530.position.z = v23 - v20 + stru_721530.height - 1;
      if ( stru_721530.position.z < stru_721530.normal.z )
        stru_721530.position.z = v23 + v20 + 1;
      stru_721530.velocity.x = v0->vVelocity.x;
      stru_721530.velocity.y = v0->vVelocity.y;
      stru_721530.velocity.z = v0->vVelocity.z;
      stru_721530.uSectorID = v0->uSectorID;
      if ( !stru_721530._47050A(v22) )
      {
        v58 = v22;
        v24 = 8 * v63;
        LOBYTE(v24) = PID(OBJECT_Actor,v63);
        v61 = v22;
        _this = v24;
        do
        {
          _46E44E_collide_against_faces_and_portals(1u);
          _46E0B2_collide_against_decorations();
          _46EF01_collision_chech_player(0);
          _46ED8A_collide_against_sprite_objects(_this);
          v25 = 0;
          v56 = 0;
          if ( ai_arrays_size > v22 )
          {
            do
            {
              v54 = ai_near_actors_ids[v25];
              if ( v54 != v63 )
              {
                v26 = v54;
                v27 = abs(pActors[v54].vPosition.z - v0->vPosition.z);
                v28 = abs(pActors[v26].vPosition.y - v0->vPosition.y);
                v29 = abs(pActors[v26].vPosition.x - v0->vPosition.x);
                if ( int_get_vector_length(v29, v28, v27) >= v0->uActorRadius + (signed int)pActors[v26].uActorRadius
                  && _46DF1A_collide_against_actor(v54, 40) )
                  ++v58;
                v22 = 0;
              }
              v25 = v56++ + 1;
            }
            while ( v56 < ai_arrays_size );
          }
          if ( _46F04E_collide_against_portals() )
            break;
          ++v61;
        }
        while ( v61 < 100 );
        v56 = v58 > 1;
        if ( stru_721530.field_7C >= stru_721530.field_6C )
        {
          v30 = stru_721530.normal2.x;
          v31 = stru_721530.normal2.y;
          v32 = stru_721530.normal2.z - stru_721530.prolly_normal_d - 1;
        }
        else
        {
          v30 = v0->vPosition.x + fixpoint_mul(stru_721530.field_7C, stru_721530.direction.x);
          v31 = v0->vPosition.y + fixpoint_mul(stru_721530.field_7C, stru_721530.direction.y);
          v32 = v0->vPosition.z + fixpoint_mul(stru_721530.field_7C, stru_721530.direction.z);
        }
        v33 = collide_against_floor(v30, v31, v32, &stru_721530.uSectorID, &uFaceID);
        v34 = pIndoor->pFaces[uFaceID].uAttributes;
        v35 = v34 & 0x400000;
        if (v35 && v0->uAIState == Dead)
        {
          v0->uAIState = Removed;
          goto LABEL_120;
        }
        if ( v59 != v22 || v62 != v22 || v35 == v22 )
        {
          if ( v33 == -30000 )
            goto LABEL_120;
          if ( v0->uCurrentActionAnimation != 1 || v33 >= v0->vPosition.z - 100 || v59 != v22 || v62 != v22 )
          {
            if ( stru_721530.field_7C >= stru_721530.field_6C )
            {
              v0->vPosition.x = LOWORD(stru_721530.normal2.x);
              v0->vPosition.y = LOWORD(stru_721530.normal2.y);
              v0->vPosition.z = LOWORD(stru_721530.normal2.z) - LOWORD(stru_721530.prolly_normal_d) - 1;
              v0->uSectorID = LOWORD(stru_721530.uSectorID);
              goto LABEL_123;
            }
            //v58 = (unsigned __int64)(stru_721530.field_7C * (signed __int64)stru_721530.direction.x) >> 16;
            v0->vPosition.x += fixpoint_mul(stru_721530.field_7C, stru_721530.direction.x);
            //v58 = (unsigned __int64)(stru_721530.field_7C * (signed __int64)stru_721530.direction.y) >> 16;
            v0->vPosition.y += fixpoint_mul(stru_721530.field_7C, stru_721530.direction.y);
            //v58 = (unsigned __int64)(stru_721530.field_7C * (signed __int64)stru_721530.direction.z) >> 16;
            v36 = stru_721530.uFaceID;
            v0->vPosition.z += fixpoint_mul(stru_721530.field_7C, stru_721530.direction.z);
            v0->uSectorID = LOWORD(stru_721530.uSectorID);
            stru_721530.field_70 += stru_721530.field_7C;
            v37 = PID_ID(v36);
            if ( PID_TYPE(v36) != OBJECT_Actor)
            {
              if ( PID_TYPE(v36) != OBJECT_Player)
              {
                if ( PID_TYPE(v36) == OBJECT_Decoration)
                {
                  _this = integer_sqrt(v0->vVelocity.x * v0->vVelocity.x + v0->vVelocity.y * v0->vVelocity.y);
                  v45 = stru_5C6E00->Atan2(
                          v0->vPosition.x - pLevelDecorations[v37].vPosition.x,
                          v0->vPosition.y - pLevelDecorations[v37].vPosition.y);
                  v0->vVelocity.x = fixpoint_mul(stru_5C6E00->Cos(v45), _this);
                  v0->vVelocity.y = fixpoint_mul(stru_5C6E00->Sin(v45), _this);
                  goto LABEL_119;
                }
                if ( PID_TYPE(v36) == OBJECT_BModel)
                {
                  stru_721530.field_84 = v36 >> 3;
                  v38 = &pIndoor->pFaces[v37];
                  if ( v38->uPolygonType == 3 )
                  {
                    v39 = v0->vVelocity.x;
                    v40 = v0->vVelocity.y * v0->vVelocity.y;
                    v0->vVelocity.z = 0;
                    v0->vPosition.z = pIndoor->pVertices[*v38->pVertexIDs].z + 1;
                    if ( v39 * v39 + v40 < 400 )
                    {
                      v0->vVelocity.y = 0;
                      v0->vVelocity.x = 0;
                      goto LABEL_119;
                    }
                  }
                  else
                  {
                    v61 = abs(v38->pFacePlane_old.vNormal.x * v0->vVelocity.x + v38->pFacePlane_old.vNormal.y
                                                                              * v0->vVelocity.y
                                                                              + v38->pFacePlane_old.vNormal.z
                                                                              * v0->vVelocity.z) >> 16;
                    if ( (stru_721530.speed >> 3) > v61 )
                      v61 = stru_721530.speed >> 3;

                    v0->vVelocity.x += fixpoint_mul(v61, v38->pFacePlane_old.vNormal.x);
                    v0->vVelocity.y += fixpoint_mul(v61, v38->pFacePlane_old.vNormal.y);
                    v0->vVelocity.z += fixpoint_mul(v61, v38->pFacePlane_old.vNormal.z);
                    v41 = v38->uPolygonType;
                    if ( v41 != 4 && v41 != 3 )
                    {
                      v42 = v38->pFacePlane_old.vNormal.x;
                      v43 = v0->vPosition.z;
                      v44 = stru_721530.prolly_normal_d
                          - ((v38->pFacePlane_old.dist
                            + v38->pFacePlane_old.vNormal.z * v0->vPosition.z
                            + v38->pFacePlane_old.vNormal.y * v0->vPosition.y
                            + v42 * v0->vPosition.x) >> 16);
                      if ( v44 > 0 )
                      {
                        v0->vPosition.x += (unsigned int)(v44 * v42) >> 16;
                        v0->vPosition.y += (unsigned int)(v44 * v38->pFacePlane_old.vNormal.y) >> 16;
                        v0->vPosition.z = v43 + ((unsigned int)(v44 * v38->pFacePlane_old.vNormal.z) >> 16);
                      }
                      v0->uYawAngle = stru_5C6E00->Atan2(v0->vVelocity.x, v0->vVelocity.y);
                    }
                  }
                  if ( BYTE3(v38->uAttributes) & 8 )
                    EventProcessor(pIndoor->pFaceExtras[v38->uFaceExtraID].uEventID, 0, 1);
                }
LABEL_119:
                v0->vVelocity.x = fixpoint_mul(58500, v0->vVelocity.x);
                v0->vVelocity.y = fixpoint_mul(58500, v0->vVelocity.y);
                v0->vVelocity.z = fixpoint_mul(58500, v0->vVelocity.z);
                v22 = 0;
                goto LABEL_120;
              }
              if ( v0->GetActorsRelation(0) )
              {
                v51 = __OFSUB__(HIDWORD(pParty->pPartyBuffs[PARTY_BUFF_INVISIBILITY].uExpireTime), v22);
                v49 = HIDWORD(pParty->pPartyBuffs[PARTY_BUFF_INVISIBILITY].uExpireTime) == v22;
                v50 = HIDWORD(pParty->pPartyBuffs[PARTY_BUFF_INVISIBILITY].uExpireTime) - v22 < 0;
                v0->vVelocity.y = v22;
                v0->vVelocity.x = v22;
                if ( !(v50 ^ v51)
                  && (!((unsigned __int8)(v50 ^ v51) | v49) || LODWORD(pParty->pPartyBuffs[PARTY_BUFF_INVISIBILITY].uExpireTime) > v22) )
                  pParty->pPartyBuffs[PARTY_BUFF_INVISIBILITY].Reset();
                viewparams->bRedrawGameUI = 1;
                goto LABEL_119;
              }
              v36 = stru_721530.uFaceID;
LABEL_118:
              Actor::AI_FaceObject(v63, v36, v22, (AIDirection *)v22);
              goto LABEL_119;
            }
            if ( pParty->bTurnBasedModeOn == 1 && (pTurnEngine->turn_stage == 2 || pTurnEngine->turn_stage == 3) )
              goto LABEL_119;
            if ( v0->pMonsterInfo.uHostilityType )
            {
              if ( v56 == v22 )
                goto LABEL_114;
            }
            else
            {
              if ( v56 == v22 )
              {
                if ( !pActors[v37].pMonsterInfo.uHostilityType )
                  goto LABEL_118;
LABEL_114:
                Actor::AI_Flee(v63, v36, v22, (AIDirection *)v22);
                goto LABEL_119;
              }
            }
            Actor::AI_StandOrBored(v63, 4, v22, &v53);
            goto LABEL_119;
          }
          if ( v0->vPosition.x & 1 )
            v0->uYawAngle += 100;
          else
            v0->uYawAngle -= 100;
        }
        else
        {
          if ( pParty->bTurnBasedModeOn == 1 && (pTurnEngine->turn_stage == 2 || pTurnEngine->turn_stage == 3) )
            goto LABEL_123;
          if ( !v0->pMonsterInfo.uHostilityType || v56 != v22 )
          {
            Actor::AI_StandOrBored(v63, 4, v22, &v52);
            goto LABEL_123;
          }
        }
      }
LABEL_120:
      ++uSectorID;
      if ( uSectorID >= 100 )
        goto LABEL_123;
      v20 = stru_721530.prolly_normal_d;
    }
  }
}
// 46DF1A: using guessed type int __fastcall 46DF1A_collide_against_actor(int, int);
// 4F75D8: using guessed type int ai_arrays_size;






//----- (00460A78) --------------------------------------------------------
void  PrepareToLoadBLV(unsigned int bLoading)
{
  unsigned int v1; // ebx@1
  unsigned int v2; // eax@8
  MapInfo *v3; // edi@9
  int v4; // eax@11
  int v5; // eax@12
  int v6; // eax@13
  int v7; // esi@27
  SpawnPointMM7 *v8; // edx@28
  //char v9; // bl@33
  //int v10; // eax@34
  //char *v11; // ecx@37
  //int v12; // edi@44
  //unsigned int v13; // edi@48
  char *v14; // esi@49
  __int16 v15; // ax@50
  int v16; // ecx@52
  DecorationDesc *v17; // eax@54
  int v18; // ecx@55
  //int *v19; // edi@58
  //unsigned char v20; // dl@58
  int v21; // eax@64
  char v22; // cl@64
  //signed int v23; // ebx@67
  //char *v24; // esi@68
  //int v25; // eax@70
  //char *v26; // esi@78
  //int v27; // eax@79
  char v28; // zf@81
  __int16 v29; // ax@85
  signed int v30; // edi@94
  int v31; // esi@94
  //char Args; // [sp+350h] [bp-C4h]@16
  int v34[4]; // [sp+3E8h] [bp-2Ch]@96
  int v35; // [sp+3F8h] [bp-1Ch]@1
  //char b[4]; // [sp+3FCh] [bp-18h]@58
  MapInfo *v37; // [sp+400h] [bp-14h]@1
  int v38; // [sp+404h] [bp-10h]@1
  unsigned int v39; // [sp+408h] [bp-Ch]@8
  int pDest; // [sp+40Ch] [bp-8h]@1
  //int i; // [sp+410h] [bp-4h]@27

  v37 = (MapInfo *)bLoading;
  v1 = 0;
  v35 = 0;
  pGameLoadingUI_ProgressBar->Reset(0x20u);
  bUnderwater = 0;
  bNoNPCHiring = 0;
  v38 = 0;
  pDest = 1;
  uCurrentlyLoadedLevelType = LEVEL_Indoor;
  pGame->uFlags2 &= 0xFFFFFFF7u;
  if ( Is_out15odm_underwater() )
  {
    bUnderwater = 1;
    pGame->uFlags2 |= 8u;
  }
  if ( !_stricmp(pCurrentMapName, "out15.odm") || !_stricmp(pCurrentMapName, "d23.blv") )
    bNoNPCHiring = 1;
  pPaletteManager->pPalette_tintColor[0] = 0;
  pPaletteManager->pPalette_tintColor[1] = 0;
  pPaletteManager->pPalette_tintColor[2] = 0;
  pPaletteManager->RecalculateAll();
  if ( qword_A750D8 )
    qword_A750D8 = 0i64;
  v2 = pMapStats->GetMapInfo(pCurrentMapName);
  v39 = v2;
  if ( v2 )
  {
    v3 = &pMapStats->pInfos[v2];
    v1 = v3->uRespawnIntervalDays;
    v38 = GetAlertStatus();
  }
  else
  {
    v3 = v37;
  }
  dword_6BE13C_uCurrentlyLoadedLocationID = v39;
  free(ptr_6A0D08);
  ptr_6A0D08 = 0;
  _6A0D0C_txt_lod_loading = 0;
  TryLoadLevelFromLOD();
  pStationaryLightsStack->uNumLightsActive = 0;
  v4 = pIndoor->Load(pCurrentMapName,
         (unsigned int)(signed __int64)((double)pParty->uTimePlayed * 0.234375) / 60 / 60 / 24 + 1,
         v1,
         (char *)&pDest)
     - 1;
  if ( !v4 )
    Error("Unable to open %s", pCurrentMapName);

  v5 = v4 - 1;
  if ( !v5 )
    Error("File %s is not a BLV File", pCurrentMapName);

  v6 = v5 - 1;
  if ( !v6 )
    Error("Attempt to open new level before clearing old");
  if ( v6 == 1 )
    Error("Out of memory loading indoor level");
  if ( !(dword_6BE364_game_settings_1 & 0x2000) )
  {
    InitializeActors();
    InitializeSpriteObjects();
  }
  BYTE1(dword_6BE364_game_settings_1) &= 0xDFu;
  if ( !v39 )
    pDest = 0;
  if ( pDest == 1 )
  {
    v7 = 0;
    for (uint i = 0; i < pIndoor->uNumSpawnPoints; ++v7 )
    {
      v8 = &pIndoor->pSpawnPoints[v7];
      if ( pIndoor->pSpawnPoints[v7].uKind == 3 )
        SpawnEncounter(v3, v8, 0, 0, 0);
      else
        v3->SpawnRandomTreasure(v8);
      ++i;
    }
    RespawnGlobalDecorations();
  }

  pSoundList->LoadSound(pDoorSoundIDsByLocationID[v39], 0);
  pSoundList->LoadSound(pDoorSoundIDsByLocationID[v39] + 1, 0);

  for (uint i = 0; i < pIndoor->uNumDoors; ++i)
  {
    auto pDoor = pIndoor->pDoors + i;

    if (pDoor->uAttributes & 0x01)
    {
      pDoor->uState = BLVDoor::Opening;
      pDoor->uTimeSinceTriggered = 15360;
      pDoor->uAttributes = 2;
    }

    if (pDoor->uState == BLVDoor::Closed)
    {
      pDoor->uState = BLVDoor::Closing;
      pDoor->uTimeSinceTriggered = 15360;
      pDoor->uAttributes = 2;
    }
    else if (pDoor->uState == BLVDoor::Open)
    {
      pDoor->uState = BLVDoor::Opening;
      pDoor->uTimeSinceTriggered = 15360;
      pDoor->uAttributes = 2;
    }
  }

/*
  i = 0;
  v9 = 2;
  if ( pIndoor->uNumDoors > 0 )
  {
    v10 = 0;
    while ( 1 )
    {
      if ( pIndoor->pDoors[v10].uAttributes & 1 )
      {
        pIndoor->pDoors[v10].uState = 1;
        pIndoor->pDoors[v10].uTimeSinceTriggered = 15360;
        pIndoor->pDoors[v10].uAttributes = 2;
      }
      v11 = (char *)&pIndoor->pDoors[v10].uState;
      if ( !*(short *)v11 )
        break;
      if ( *(short *)v11 == 2 )
      {
        *(short *)v11 = 1;
        goto LABEL_41;
      }
LABEL_42:
      ++i;
      ++v10;
      if ( i >= pIndoor->uNumDoors )
        goto LABEL_43;
    }
    *(short *)v11 = 3;
LABEL_41:
    pIndoor->pDoors[v10].uTimeSinceTriggered = 15360;
    pIndoor->pDoors[v10].uAttributes = 2;
    goto LABEL_42;
  }
LABEL_43:
*/

  for (uint i = 0; i < pIndoor->uNumFaces; ++i)
  {
    auto pFace = pIndoor->pFaces + i;
    if (!pFace->uBitmapID != -1)
      pBitmaps_LOD->pTextures[pFace->uBitmapID].palette_id2 = pPaletteManager->LoadPalette(pBitmaps_LOD->pTextures[pFace->uBitmapID].palette_id1);
 }
  
  pGameLoadingUI_ProgressBar->Progress();
  
  for (uint i = 0; i < uNumLevelDecorations; ++i)
  {
    auto pDecortaion = &pLevelDecorations[i];
    
    pDecorationList->InitializeDecorationSprite(pDecortaion->uDecorationDescID);

    v15 = pDecorationList->pDecorations[pDecortaion->uDecorationDescID].uSoundID;
    if (v15 && _6807E0_num_decorations_with_sounds_6807B8 < 9)
    {
        pSoundList->LoadSound(v15, 0);
        v16 = _6807E0_num_decorations_with_sounds_6807B8++;
        _6807B8_level_decorations_ids[v16] = i;
    }

    if (!(pDecortaion->uFlags & LEVEL_DECORATION_INVISIBLE))
    {
      v17 = &pDecorationList->pDecorations[pDecortaion->uDecorationDescID];
      if (!v17->DontDraw())
        {
          v18 = v17->uLightRadius;
          if ( v18 )
          {
            unsigned char r = 255,
                          g = 255,
                          b = 255;
            if (pRenderer->pRenderD3D && pRenderer->bUseColoredLights)
            {
              r = v17->uColoredLightRed;
              g = v17->uColoredLightGreen;
              b = v17->uColoredLightBlue;
            }
            pStationaryLightsStack->AddLight(pDecortaion->vPosition.x,
                                             pDecortaion->vPosition.y,
                                             pDecortaion->vPosition.z + v17->uDecorationHeight,
              v18,
              r,
              g,
              b,
              byte_4E94D0);
          }
        }
      }

      if (!pDecortaion->uEventID)
      {
        if (pDecortaion->IsInteractive())
        {
          if ( v35 < 124 )
          {
            v21 = v35 + 1;
            pDecortaion->_idx_in_stru123 = v35 + 75;
            v22 = stru_5E4C90_MapPersistVars._decor_events[v21 + 74 - 75];
            v35 = v21;
            if ( !v22 )
              pDecortaion->uFlags |= LEVEL_DECORATION_INVISIBLE;
          }
        }
      }
  }

  pGameLoadingUI_ProgressBar->Progress();

  for (uint i = 0; i < uNumSpriteObjects; ++i)
  {
    auto p = &pSpriteObjects[i];
    if (p->uObjectDescID)
    {
      auto uItemID = p->stru_24.uItemID;
      if (uItemID)
      {
        if (uItemID != 220 && pItemsTable->pItems[uItemID].uEquipType == EQUIP_POTION &&
            !p->stru_24.uEnchantmentType)
          p->stru_24.uEnchantmentType = rand() % 15 + 5;
        pItemsTable->SetSpecialBonus(&p->stru_24);
      }
    }
  }

  for (uint i = 0; i < uNumActors; ++i)
  //if ( (signed int)uNumActors > (signed int)v13 )
  {
    Actor *pActor = &pActors[i];
    //v26 = (char *)&pActors[0].uAttributes;
    //do
    //{
      //v27 = *(int *)v26;
      if (pActor->uAttributes & 0x100000)
      {
        if ( !v39 )
        {
          pActor->pMonsterInfo.field_3E = 19;
          pActor->uAttributes |= 0x10000;
          continue;
        }
        v28 = v38 == 0;
      }
      else
      {
        v28 = v38 == 1;
      }

      if ( !v28 )
      {
        pActor->PrepareSprites(0);
        v29 = pActor->pMonsterInfo.field_3E;
        pActor->pMonsterInfo.uHostilityType = MonsterInfo::Hostility_Friendly;
        if ( v29 != 11 && v29 != 19 && (!pActor->sCurrentHP || !pActor->pMonsterInfo.uHP) )
        {
          pActor->pMonsterInfo.field_3E = 5;
          pActor->UpdateAnimation();
        }
      }
      else
      {
          pActor->pMonsterInfo.field_3E = 19;
          pActor->uAttributes |= 0x10000;
      }
    //}
    //while ( i < (signed int)uNumActors );
  }

  pGameLoadingUI_ProgressBar->Progress();

  Actor this_;
  this_.pMonsterInfo.uID = 45;
  this_.PrepareSprites(0);
  if ( !v37 )
  {
    pParty->sRotationX = 0;
    pParty->sRotationY = 0;
    pParty->vPosition.z = 0;
    pParty->vPosition.y = 0;
    pParty->vPosition.x = 0;
    pParty->uFallStartY = 0;
    pParty->uFallSpeed = 0;
    TeleportToStartingPoint(uLevel_StartingPointType);
  }
  viewparams->_443365();
  PlayLevelMusic();
  if ( !v37 )
  {
    v30 = 0;
    v31 = 1;
    do
    {
      if ( pPlayers[v31]->CanAct() )
        v34[v30++] = v31;
      ++v31;
    }
    while ( v31 <= 4 );
    if ( v30 )
    {
      if ( pDest )
      {
        qword_A750D8 = 256i64;
        PlayerSpeechID = SPEECH_46;
        uSpeakingCharacter = LOWORD(v34[rand() % v30]);
      }
    }
  }
}
//----- (0046CEC3) --------------------------------------------------------
int BLV_GetFloorLevel(int x, int y, int z, unsigned int uSectorID, unsigned int *pFaceID)
{
  int v13; // ecx@13
  signed int v14; // ebx@14
  int v15; // eax@16
  //int v16; // edx@19
  //int v17; // ST18_4@19
  //signed int v18; // edx@19
  //signed __int64 v19; // qtt@19
  int v21; // eax@27
  //int v22; // ecx@29
  signed int v28; // eax@45
  int v29; // ebx@47
  int v30; // edx@49
  //int v31; // ST10_4@49
  //signed int v32; // edx@49
  signed __int64 v33; // qtt@49
  //signed int v34; // eax@54
  //signed int v35; // esi@56
  //int result; // eax@57
  int v38; // edx@62
  //int v44; // [sp+20h] [bp-20h]@10
  bool v47; // [sp+24h] [bp-1Ch]@43
  bool v48; // [sp+28h] [bp-18h]@10
  bool v49; // [sp+28h] [bp-18h]@41
  bool v50; // [sp+2Ch] [bp-14h]@12
  signed int v53; // [sp+30h] [bp-10h]@10
  signed int v54; // [sp+30h] [bp-10h]@41
  signed int v55; // [sp+34h] [bp-Ch]@1

  //LOG_DECOMPILATION_WARNING();

  static int blv_floor_id[50]; // 00721200
  static int blv_floor_level[50]; // 007212C8

  static __int16 word_721390_ys[104]; // idb
  static __int16 word_721460_xs[104]; // idb

  auto pSector = &pIndoor->pSectors[uSectorID];
  v55 = 0;
  for (uint i = 0; i < pSector->uNumFloors; ++i)
  {
    auto pFloor = &pIndoor->pFaces[pSector->pFloors[i]];
    if (pFloor->Clickable())
      continue;

    assert(pFloor->uNumVertices);
    if (x <= pFloor->pBounding.x2 && x >= pFloor->pBounding.x1 &&
        y <= pFloor->pBounding.y2 && y >= pFloor->pBounding.y1)
    {
      for (uint j = 0; j < pFloor->uNumVertices; ++j)
      {
        word_721460_xs[2 * j] =     pFloor->pXInterceptDisplacements[j] + pIndoor->pVertices[pFloor->pVertexIDs[j]].x;
        word_721460_xs[2 * j + 1] = pFloor->pXInterceptDisplacements[j] + pIndoor->pVertices[pFloor->pVertexIDs[j + 1]].x;
        word_721390_ys[2 * j] =     pFloor->pYInterceptDisplacements[j] + pIndoor->pVertices[pFloor->pVertexIDs[j]].y;
        word_721390_ys[2 * j + 1] = pFloor->pYInterceptDisplacements[j] + pIndoor->pVertices[pFloor->pVertexIDs[j + 1]].y;
      }
      word_721460_xs[2 * pFloor->uNumVertices] = word_721460_xs[0];
      word_721390_ys[2 * pFloor->uNumVertices] = word_721390_ys[0];

      v50 = word_721390_ys[0] >= y;
      v53 = 0;

      for (uint j = 0; j < 2 * pFloor->uNumVertices; ++j)
      {
        if (v53 >= 2)
          break;

        v48 = v50;
        v50 = word_721390_ys[j + 1] >= y;

          v13 = i;
          if (v48 == v50)
            continue;

            v14 = word_721460_xs[j + 1] >= x ? 0 : 2;
            v15 = v14 | (word_721460_xs[j] < x);

          if (v15 == 3)
            continue;
          else if (!v15)
            ++v53;
          else
          {
            auto a_div_b = fixpoint_div(y - word_721390_ys[j], word_721390_ys[j + 1] - word_721390_ys[j]);
            auto res = fixpoint_mul((signed int)word_721460_xs[j + 1] - (signed int)word_721460_xs[j], a_div_b);

            if (res + word_721460_xs[j] >= x)
                ++v53;
          }
      }


        if ( v53 == 1 )
        {
          if ( v55 >= 50 )
            break;
          if ( pFloor->uPolygonType == POLYGON_Floor || pFloor->uPolygonType == POLYGON_Ceiling )
          {
            v21 = pIndoor->pVertices[pFloor->pVertexIDs[0]].z;
          }
          else
          {
            v21 = fixpoint_mul(pFloor->zCalc1, x) + fixpoint_mul(pFloor->zCalc2, y) + (short)(pFloor->zCalc3 >> 16);
          }
          blv_floor_level[v55] = v21;
          blv_floor_id[v55] = pSector->pFloors[i];
          v55++;
        }
    }
  }


  if ( pSector->field_0 & 8 )
  {
    for (uint i = 0; i < pSector->uNumPortals; ++i)
    {
      auto portal = &pIndoor->pFaces[pSector->pPortals[i]];
      if (portal->uPolygonType != POLYGON_Floor)
        continue;

      if (!portal->uNumVertices)
        continue;

      if (x <= portal->pBounding.x2 && x >= portal->pBounding.x1 &&
          y <= portal->pBounding.y2 && y >= portal->pBounding.y1 )
      {
        for (uint j = 0; j < portal->uNumVertices; ++j)
        {
          word_721460_xs[2 * j] =     portal->pXInterceptDisplacements[j] + pIndoor->pVertices[portal->pVertexIDs[j]].x;
          word_721460_xs[2 * j + 1] = portal->pXInterceptDisplacements[j + 1] + pIndoor->pVertices[portal->pVertexIDs[j + 1]].x;
          word_721390_ys[2 * j] =     portal->pYInterceptDisplacements[j] + pIndoor->pVertices[portal->pVertexIDs[j]].y;
          word_721390_ys[2 * j + 1] = portal->pYInterceptDisplacements[j + 1] + pIndoor->pVertices[portal->pVertexIDs[j + 1]].y;
        }
        word_721460_xs[2 * portal->uNumVertices] = word_721460_xs[0];
        word_721390_ys[2 * portal->uNumVertices] = word_721390_ys[0];
        v54 = 0;
        v47 = word_721390_ys[0] >= y;

          for (uint j = 0; j < 2 * portal->uNumVertices; ++j)
          {
            v49 = v47;
            if ( v54 >= 2 )
              break;
            v47 = word_721390_ys[j + 1] >= y;
            if ( v49 != v47 )
            {
              v28 = word_721460_xs[j + 1] >= x ? 0 : 2;
              v29 = v28 | (word_721460_xs[j] < x);
              if ( v29 != 3 )
              {
                if ( !v29 )
                  ++v54;
                else
                {
                  auto a_div_b = fixpoint_div(y - word_721390_ys[j], word_721390_ys[j + 1] - word_721390_ys[j]);
                  auto res = fixpoint_mul(word_721460_xs[j + 1] - word_721460_xs[j], a_div_b);
                  if (res + word_721460_xs[j] >= x)
                    ++v54;
                }
              }
            }
          }
          if ( v54 == 1 )
          {
            if ( v55 >= 50 )
              break;
            blv_floor_level[v55] = -29000;
            blv_floor_id[v55] = pSector->pPortals[i];
            v55++;
          }
      }
    }
  }
  if ( v55 == 1 )
  {
    *pFaceID = blv_floor_id[0];
    return blv_floor_level[0];
  }
  if ( !v55 )
    return -30000;
  *pFaceID = blv_floor_id[0];
  //result = blv_floor_level[0];

    /*for ( v35 = 1; v35 < v55; ++v35 )
    {
      if ( blv_floor_level[0] <= z + 5 )
      {
        if ( blv_floor_level[v35] >= blv_floor_level[0] || blv_floor_level[v35] > z + 5 )
          continue;
        blv_floor_level[0] = blv_floor_level[v35];
        *pFaceID = blv_floor_id[v35];
        continue;
      }
      if ( blv_floor_level[v35] < blv_floor_level[0] )
      {
        blv_floor_level[0] = blv_floor_level[v35];
        *pFaceID = blv_floor_id[v35];
      }
    }*/

    
  int result = blv_floor_level[0];
  for (uint i = 1; i < v55; ++i)
  {
      v38 = blv_floor_level[i];
      if ( result <= z + 5 )
      {
        if ( v38 > result && v38 <= z + 5 )
        {
          result = blv_floor_level[i];
          *pFaceID = blv_floor_id[i];
        }
      }
      else if ( v38 < result )
      {
        result = blv_floor_level[i];
        *pFaceID = blv_floor_id[i];
      }
  }

  return result;
}
//----- (004016FA) --------------------------------------------------------
int  MakeActorAIList_BLV()
{
  Actor *v0; // esi@2
  int v1; // eax@4
  int v2; // ebx@4
  unsigned int v3; // ecx@4
  int v4; // edx@5
  int v5; // edx@7
  unsigned int v6; // edx@9
  unsigned int v7; // ST24_4@10
  int v8; // eax@10
  int v9; // edi@10
  int v10; // ebx@14
  char v11; // zf@16
  int v12; // eax@22
  int v13; // edx@24
  int v14; // ecx@25
  int v15; // ebx@26
  unsigned int *v16; // ecx@27
  unsigned int v17; // esi@27
  int v18; // ecx@31
  signed int v19; // edi@31
  Actor *v20; // esi@32
  bool v21; // eax@33
  int v22; // eax@34
  signed int v23; // ebx@36
  Actor *v24; // esi@37
  signed int v25; // eax@40
  int v26; // eax@43
  int v27; // ebx@45
  int j; // edi@45
  unsigned int v29; // eax@46
  int v30; // eax@48
  int v31; // ecx@51
  int v32; // eax@51
  signed int v33; // eax@53
  __int64 v34; // qax@55
  char *v35; // ecx@56
  int v37; // [sp+Ch] [bp-18h]@1
  int v38; // [sp+10h] [bp-14h]@4
  int v39; // [sp+14h] [bp-10h]@4
  int v40; // [sp+18h] [bp-Ch]@10
  int v41; // [sp+18h] [bp-Ch]@29
  int i; // [sp+18h] [bp-Ch]@31
  signed int v43; // [sp+1Ch] [bp-8h]@1
  signed int v44; // [sp+1Ch] [bp-8h]@25
  int v45; // [sp+20h] [bp-4h]@1

//  __debugbreak(); // refactor for blv ai
  pParty->uFlags &= 0xFFFFFFCFu;
  v37 = pIndoor->GetSector(pParty->vPosition.x, pParty->vPosition.y, pParty->vPosition.z);
  v45 = 0;
  v43 = 0;
  if ( (signed int)uNumActors > 0 )
  {
    v0 = pActors.data();//[0].uAttributes;
    do
    {
      BYTE1(v0->uAttributes) &= 0xFBu;
      if ( ! v0->CanAct() )
        goto LABEL_60;
	  v39 = abs(pParty->vPosition.z - v0->vPosition.z);
	  v38 = abs(pParty->vPosition.y - v0->vPosition.y);
	  v1 = abs(pParty->vPosition.x - v0->vPosition.x);
      v2 = v38;
      v3 = v39;
      if ( v1 < v38 )
      {
        v4 = v1;
        v1 = v38;
        v2 = v4;
      }
      if ( v1 < v39 )
      {
        v5 = v1;
        v1 = v39;
        v3 = v5;
      }
      if ( v2 < (signed int)v3 )
      {
        v6 = v3;
        v3 = v2;
        v2 = v6;
      }
      v7 = ((unsigned int)(11 * v2) >> 5) + (v3 >> 2) + v1;
      v8 = v0->uActorRadius;
      v9 = v7 - v8;
      v40 = v7 - v8;
      if ( v40 < 0 )
      {
        v9 = 0;
        v40 = 0;
      }
      if ( v9 < 10240 )
      {
        v10 = v0->uAttributes & 0xFEFFFFFF;
        v0->uAttributes = v10;
        if ( v10 & 0x80000 || v0->GetActorsRelation(0) )
        {
          v11 = (pParty->uFlags & 0x10) == 0;
          v0->uAttributes = v10 | 0x1000000;
          if ( v11 && (double)v40 < 307.2 )
            pParty->uFlags |= 0x10u;
          if ( !(pParty->uFlags & 0x20) && v9 < 5120 )
            pParty->uFlags |= 0x20u;
        }
        v12 = v45++;
        ai_near_actors_distances[v12] = v9;
        ai_near_actors_ids[v12] = v43;
      }
      else
      {
LABEL_60:
        BYTE1(v0->uAttributes) &= 0xBFu;
      }
      ++v43;
      ++v0;
    }
    while ( v43 < (signed int)uNumActors );
  }
  v13 = 0;
  if ( v45 > 0 )
  {
    v14 = 1;
    v44 = 1;
    do
    {
      while ( 1 )
      {
        v41 = v14;
        if ( v14 >= v45 )
          break;
        v15 = ai_near_actors_distances[v13];
        if ( v15 > ai_near_actors_distances[v14] )
        {
          v16 = &ai_near_actors_ids[v14];
          v17 = ai_near_actors_ids[v13];
          ai_near_actors_ids[v13] = *v16;
          *v16 = v17;
          v14 = v41;
          ai_near_actors_distances[v13] = ai_near_actors_distances[v41];
          ai_near_actors_distances[v41] = v15;
        }
        ++v14;
      }
      ++v13;
      v14 = v44 + 1;
      v44 = v14;
    }
    while ( v14 - 1 < v45 );
  }
  v18 = 0;
  v19 = 0;
  for ( i = 0; v18 < v45; i = v18 )
  {
    v20 = &pActors[ai_near_actors_ids[v18]];
    if ( v20->uAttributes & 0x8000
      || (v21 = sub_4070EF_prolly_detect_player(PID(OBJECT_Actor,ai_near_actors_ids[v18]), 4u), v18 = i, v21) )
    {
      v22 = ai_near_actors_ids[v18];
      v20->uAttributes |= 0x8000u;
      ai_array_4F6638_actor_ids[v19] = v22;
      ai_array_4F5E68[v19++] = ai_near_actors_distances[v18];
      if ( v19 >= 30 )
        break;
    }
    ++v18;
  }
  v23 = 0;
  ai_arrays_size = v19;
  if ( (signed int)uNumActors > 0 )
  {
    v24 = pActors.data();//[0].uAttributes;
    do
    {
      if ( v24->CanAct() && v24->uSectorID == v37 )
      {
        v25 = 0;
        if ( v19 <= 0 )
        {
LABEL_43:
          v26 = ai_arrays_size;
          BYTE1(v24->uAttributes) |= 0x40u;
          ++ai_arrays_size;
          ai_array_4F6638_actor_ids[v26] = v23;
        }
        else
        {
          while ( ai_array_4F6638_actor_ids[v25] != v23 )
          {
            ++v25;
            if ( v25 >= v19 )
              goto LABEL_43;
          }
        }
      }
      ++v23;
      ++v24;
    }
    while ( v23 < (signed int)uNumActors );
  }
  v27 = ai_arrays_size;
  for ( j = 0; j < v45; ++j )
  {
    v29 = ai_near_actors_ids[j];
    if ( pActors[v29].uAttributes & 0xC000 && pActors[v29].CanAct() )
    {
      v30 = 0;
      if ( v27 <= 0 )
      {
LABEL_51:
        v31 = ai_arrays_size;
        v32 = ai_near_actors_ids[j];
        ++ai_arrays_size;
        ai_array_4F6638_actor_ids[v31] = v32;
      }
      else
      {
        while ( ai_array_4F6638_actor_ids[v30] != ai_near_actors_ids[j] )
        {
          ++v30;
          if ( v30 >= v27 )
            goto LABEL_51;
        }
      }
    }
  }
  v33 = ai_arrays_size;
  if ( ai_arrays_size > 30 )
  {
    v33 = 30;
    ai_arrays_size = 30;
  }
  memcpy(ai_near_actors_ids.data(), ai_array_4F6638_actor_ids.data(), 4 * v33);
  memcpy(ai_near_actors_distances.data(), ai_array_4F5E68.data(), 4 * ai_arrays_size);
  v34 = (unsigned int)ai_arrays_size;
  if ( ai_arrays_size > 0 )
  {
    do
    {
      v35 = (char *)&pActors[ai_near_actors_ids[HIDWORD(v34)]].uAttributes;
      v35[1] |= 4u;
      ++HIDWORD(v34);
    }
    while ( SHIDWORD(v34) < (signed int)v34 );
  }
  return v34;
}
//----- (0043FDED) --------------------------------------------------------
void PrepareActorRenderList_BLV()
{
  RenderBillboard *v0; // esi@0
  unsigned __int16 v3; // ax@3
  unsigned int v4; // eax@5
  unsigned __int16 v5; // cx@5
  int v6; // esi@5
  unsigned int v7; // eax@7
  int v8; // eax@10
  SpriteFrame *v9; // eax@16
  SpriteFrame *v10; // ebx@18
  //int *v11; // eax@18
  int v12; // ecx@28
  //IndoorCameraD3D **v14; // eax@36
  double v15; // st7@36
  float v16; // eax@36
  //double v17; // ST30_8@36
  signed __int64 v18; // qtt@36
  int v19; // ST5C_4@36
  signed __int64 v20; // qtt@37
  int v21; // ST5C_4@37
  signed __int16 v22; // cx@39
  int v23; // ST50_4@40
  signed int v24; // ecx@40
  int v25; // edx@44
  __int16 v26; // ax@44
  //MonsterDesc *v27; // edx@44
  //int v28; // ecx@44
  unsigned __int8 v29; // zf@44
  unsigned __int8 v30; // sf@44
  unsigned int v31; // [sp-8h] [bp-5Ch]@15
  int v32; // [sp+1Ch] [bp-38h]@5
  int a3; // [sp+20h] [bp-34h]@5
  int a2; // [sp+24h] [bp-30h]@5
  int a1a; // [sp+28h] [bp-2Ch]@5
  __int16 a5; // [sp+2Ch] [bp-28h]@5
  int a5a; // [sp+2Ch] [bp-28h]@36
  int a5b; // [sp+2Ch] [bp-28h]@40
  __int16 v41; // [sp+3Ch] [bp-18h]@18
  int a6; // [sp+40h] [bp-14h]@34
  int v43; // [sp+44h] [bp-10h]@34
  int z; // [sp+48h] [bp-Ch]@32
  signed int y; // [sp+4Ch] [bp-8h]@32
  int x; // [sp+50h] [bp-4h]@32

  for (uint i = 0; i < uNumActors; ++i)
  {
    auto p = &pActors[i];

    if (p->uAIState == Removed ||
        p->uAIState == Disabled)
      continue;

    a5 = p->uSectorID;
    a2 = p->vPosition.y;
    a1a = p->vPosition.x;
    a3 = p->vPosition.z;
    v4 = stru_5C6E00->Atan2(a1a - pGame->pIndoorCameraD3D->vPartyPos.x, a2 - pGame->pIndoorCameraD3D->vPartyPos.y);
    LOWORD(v0) = p->uYawAngle;
    v5 = p->uCurrentActionAnimation;
    v6 = ((signed int)((char *)v0 + ((signed int)stru_5C6E00->uIntegerPi >> 3) - v4 + stru_5C6E00->uIntegerPi) >> 8) & 7;
    v32 = v6;
    if ( pParty->bTurnBasedModeOn )
    {
      if ( v5 == 1 )
      {
        v7 = pMiscTimer->uTotalGameTimeElapsed;
        goto LABEL_10;
      }
    }
    else
    {
      if ( v5 == 1 )
      {
        v7 = pBLVRenderParams->field_0_timer_;
LABEL_10:
        v8 = i * 32 + v7;
        goto LABEL_12;
      }
    }
    v8 = p->uCurrentActionTime;
LABEL_12:
    if (p->pActorBuffs[5].uExpireTime > 0i64 || p->pActorBuffs[6].uExpireTime > 0i64 )
      v8 = 0;
    v31 = p->pSpriteIDs[v5];
    if (p->uAIState == Resurrected)
      v9 = pSpriteFrameTable->GetFrameBy_x(v31, v8);
    else
      v9 = pSpriteFrameTable->GetFrame(v31, v8);
    v41 = 0;
    v10 = v9;
    //v11 = (int *)v9->uFlags;
    if (v9->uFlags & 2)
      v41 = 2;
    if (v9->uFlags & 0x40000)
      v41 |= 0x40u;
    if (v9->uFlags & 0x20000)
      LOBYTE(v41) = v41 | 0x80;
    v0 = (RenderBillboard *)(256 << v6);
    if ( (unsigned int)v0 & v9->uFlags)
      v41 |= 4u;
    if ( v10->uGlowRadius )
    {
      //LOBYTE(v11) = byte_4E94D3;
      pMobileLightsStack->AddLight(
        a1a,
        a2,
        a3,
        a5,
        v10->uGlowRadius,
        0xFFu,
        0xFFu,
        0xFFu,
        byte_4E94D3);
    }
    v12 = 0;
    if ( pBspRenderer->uNumVisibleNotEmptySectors <= 0 )
      continue;
    while (pBspRenderer->pVisibleSectorIDs_toDrawDecorsActorsEtcFrom[v12] != p->uSectorID)
    {
      ++v12;
      if ( v12 >= pBspRenderer->uNumVisibleNotEmptySectors )
        goto _continue;
    }
    if ( !pGame->pIndoorCameraD3D->ApplyViewTransform_TrueIfStillVisible_BLV(a1a, a2, a3, &x, &y, &z, 1)
      || (v0 = (RenderBillboard *)abs(x), (signed int)v0 < abs(y)) )
      continue;
    pGame->pIndoorCameraD3D->Project(x, y, z, &v43, &a6);
    v0 = &pBillboardRenderList[uNumBillboardsToDraw];
    if (uNumBillboardsToDraw >= 500)
      break;
    ++uNumBillboardsToDraw;
    ++uNumSpritesDrawnThisFrame;
    p->uAttributes |= 8u;
    v29 = pRenderer->pRenderD3D == 0;
    v0->uHwSpriteID = v10->pHwSpriteIDs[v32];
    v0->uPalette = v10->uPaletteIndex;
    v0->uIndoorSectorID = a5;
    if ( v29 )
    {
      LODWORD(v20) = pBLVRenderParams->fov_rad_fixpoint << 16;
      HIDWORD(v20) = pBLVRenderParams->fov_rad_fixpoint >> 16;
      v21 = v20 / x;
      v0->_screenspace_x_scaler_packedfloat = (unsigned __int64)(v10->scale * v20 / x) >> 16;
      a5a = (unsigned __int64)(v10->scale * (signed __int64)v21) >> 16;
    }
    else
    {
      //v14 = &pGame->pIndoorCameraD3D;
      v0->fov_x = pGame->pIndoorCameraD3D->fov_x;
      v15 = pGame->pIndoorCameraD3D->fov_y;
      v16 = v0->fov_x;
      v0->fov_y = v15;
      //v17 = v16 + 6.7553994e15;
      LODWORD(v18) = 0;
      HIDWORD(v18) = floorf(v16 + 0.5f);
      v19 = v18 / x;
      v0->_screenspace_x_scaler_packedfloat = (unsigned __int64)(v10->scale * v18 / x) >> 16;
      a5a = (unsigned __int64)(v10->scale * (signed __int64)v19) >> 16;
    }
    v0->_screenspace_y_scaler_packedfloat = a5a;
    if ( (signed __int64)p->pActorBuffs[3].uExpireTime <= 0 )
    {
      if ( (signed __int64)p->pActorBuffs[10].uExpireTime > 0 )
      {
        a5b = (unsigned __int64)(pGame->pStru6Instance->_4A806F(p) * (signed __int64)v0->_screenspace_y_scaler_packedfloat) >> 16;
        goto LABEL_43;
      }
    }
    else
    {
      v22 = p->pActorBuffs[3].uPower;
      if ( v22 )
      {
        v23 = (unsigned __int64)(65536 / (unsigned __int16)v22 * (signed __int64)v0->_screenspace_x_scaler_packedfloat) >> 16;
        v24 = p->pActorBuffs[3].uPower;
        v0->_screenspace_x_scaler_packedfloat = v23;
        a5b = (unsigned __int64)(65536 / v24 * (signed __int64)v0->_screenspace_y_scaler_packedfloat) >> 16;
LABEL_43:
        v0->_screenspace_y_scaler_packedfloat = a5b;
        goto LABEL_44;
      }
    }
LABEL_44:
    HIWORD(v25) = HIWORD(x);
    v0->world_x = a1a;
    v0->world_y = a2;
    v0->world_z = a3;
    v0->uScreenSpaceX = v43;
    v0->uScreenSpaceY = a6;
    LOWORD(v25) = 0;
    LOBYTE(v26) = v41;

    //v0->sZValue = v25 + (PID(OBJECT_Actor,i));
    v0->actual_z = HIWORD(x);
    v0->object_pid = PID(OBJECT_Actor,i);

    v29 = HIDWORD(p->pActorBuffs[5].uExpireTime) == 0;
    v30 = HIDWORD(p->pActorBuffs[5].uExpireTime) < 0;
    v0->field_1E = v41;
    v0->pSpriteFrame = v10;
    v0->uTintColor = pMonsterList->pMonsters[p->pMonsterInfo.uID - 1].uTintColor;
    if ( !v30 && (!(v30 | v29) || LODWORD(p->pActorBuffs[5].uExpireTime)) )
    {
      HIBYTE(v26) = HIBYTE(v41) | 1;
      v0->field_1E = v26;
    }
    
_continue:
    ;
  }
}
//----- (0044028F) --------------------------------------------------------
void PrepareItemsRenderList_BLV()
{
  ObjectDesc *v1; // ebx@4
  __int16 v2; // ax@5
  RenderBillboard *v3; // esi@12
  SpriteFrame *v4; // eax@12
  SpriteFrame *v5; // ebx@12
  unsigned int v6; // eax@12
  int v7; // ecx@12
  int v8; // edx@12
  int v9; // ecx@12
  unsigned __int16 v10; // ax@12
  int *v11; // eax@20
  //char v12; // zf@26
  __int64 v18; // ST5C_4@27
  signed __int64 v19; // qtt@28
  int v20; // ST5C_4@28
  //int v21; // edx@29
  __int16 v22; // ax@29
  //int v23; // eax@29
  SpriteFrame *v24; // [sp+1Ch] [bp-40h]@12
  //__int16 a5; // [sp+28h] [bp-34h]@12
  int a6; // [sp+2Ch] [bp-30h]@12
  int a2; // [sp+30h] [bp-2Ch]@12
  int a1; // [sp+34h] [bp-28h]@12
  int v30; // [sp+38h] [bp-24h]@12
  int v31; // [sp+38h] [bp-24h]@27
  int a3; // [sp+40h] [bp-1Ch]@12
  signed __int16 v34; // [sp+44h] [bp-18h]@14
  int v35; // [sp+48h] [bp-14h]@25
  int v36; // [sp+4Ch] [bp-10h]@25
  signed int z; // [sp+50h] [bp-Ch]@24
  signed int y; // [sp+54h] [bp-8h]@24
  signed int x; // [sp+58h] [bp-4h]@24

  for (uint i = 0; i < uNumSpriteObjects; ++i)
  {
    auto p = &pSpriteObjects[i];
    if (p->uObjectDescID)
    {
      v1 = &pObjectList->pObjects[p->uObjectDescID];
        if ( !(v1->uFlags & 1) )
         {
          if ( ((v2 = p->uType, v2 < 1000) || v2 >= 10000)
            && (v2 < 500 || v2 >= 600)
            && (v2 < 811 || v2 >= 815)
            || pGame->pStru6Instance->_4A81CA(p))
          {
            //a5 = p->uSectorID;
            a1 = p->vPosition.x;
            a2 = p->vPosition.y;
            a3 = p->vPosition.z;
            v3 = &pBillboardRenderList[uNumBillboardsToDraw];
            v4 = pSpriteFrameTable->GetFrame(v1->uSpriteID, p->uSpriteFrameID);
            v5 = v4;
            v24 = v4;
            v30 = v4->uFlags;
            a6 = v4->uGlowRadius * p->field_22_glow_radius_multiplier;
            v6 = stru_5C6E00->Atan2(p->vPosition.x - pGame->pIndoorCameraD3D->vPartyPos.x,
                                    p->vPosition.y - pGame->pIndoorCameraD3D->vPartyPos.y);
            LOWORD(v7) = p->uFacing;
            v8 = v30;
            v9 = ((signed int)(stru_5C6E00->uIntegerPi + ((signed int)stru_5C6E00->uIntegerPi >> 3) + v7 - v6) >> 8) & 7;
            v10 = v5->pHwSpriteIDs[v9];
            v3->uHwSpriteID = v10;
            if ( v30 & 0x20 )
            {
              v8 = v30;
              a3 -= (signed int)((unsigned __int64)(v5->scale * (signed __int64)pSprites_LOD->pSpriteHeaders[(signed __int16)v10].uHeight) >> 16) >> 1;
            }
            v34 = 0;
            if ( v8 & 2 )
              v34 = 2;
            if ( v8 & 0x40000 )
              v34 |= 0x40u;
            if ( v8 & 0x20000 )
              LOBYTE(v34) = v34 | 0x80;
            v11 = (int *)(256 << v9);
            if ( (256 << v9) & v8 )
              v34 |= 4u;
            if ( a6 )
            {
              LOBYTE(v11) = byte_4E94D3;
              pMobileLightsStack->AddLight(
                a1,
                a2,
                a3,
                p->uSectorID,
                a6,
                v1->uParticleTrailColorR,
                v1->uParticleTrailColorG,
                v1->uParticleTrailColorB,
                byte_4E94D3);
            }
            if ( pGame->pIndoorCameraD3D->ApplyViewTransform_TrueIfStillVisible_BLV(
                   a1,
                   a2,
                   a3,
                   &x,
                   &y,
                   &z,
                   1) )
            {
              pGame->pIndoorCameraD3D->Project(x, y, z, &v36, &v35);

              assert(uNumBillboardsToDraw < 500);
              //if ( (signed int)uNumBillboardsToDraw >= 500 )
              //  return;
              ++uNumBillboardsToDraw;
              ++uNumSpritesDrawnThisFrame;
              p->uAttributes |= 1u;
              //v12 = pRenderer->pRenderD3D == 0;
              v3->uPalette = v24->uPaletteIndex;
              v3->uIndoorSectorID = p->uSectorID;
              if ( pRenderer->pRenderD3D )
              {
                v3->fov_x = pGame->pIndoorCameraD3D->fov_x;
                v3->fov_y = pGame->pIndoorCameraD3D->fov_y;
                LODWORD(v18) = 0;
                HIDWORD(v18) = (int)floorf(v3->fov_x + 0.5f);
                v18 = v18 / x;
                v3->_screenspace_x_scaler_packedfloat = (unsigned __int64)(v24->scale * v18) >> 16;
                v31 = (unsigned __int64)(v24->scale * v18) >> 16;
              }
              else
              {
                __debugbreak(); // sw rendering
                /*LODWORD(v19) = pBLVRenderParams->field_40 << 16;
                HIDWORD(v19) = pBLVRenderParams->field_40 >> 16;
                v20 = v19 / x;
                v3->_screenspace_x_scaler_packedfloat = (unsigned __int64)(v24->scale * v19 / x) >> 16;
                v31 = (unsigned __int64)(v24->scale * (signed __int64)v20) >> 16;*/
              }
              //HIWORD(v21) = HIWORD(x);
              //LOWORD(v21) = 0;
              v3->_screenspace_y_scaler_packedfloat = v31;
              v3->field_1E = v34;
              v3->world_x = a1;
              v3->world_y = a2;
              v3->world_z = a3;
              v3->uScreenSpaceX = v36;
              v22 = v35;
              v3->uTintColor = 0;
              v3->uScreenSpaceY = v22;
              //v23 = 8 * i;
              //LOBYTE(v23) = PID(OBJECT_Item,i);
              v3->pSpriteFrame = v24;
              //v12 = (p->uAttributes & 0x20) == 0;
              //v3->sZValue = v21 + v23;
              v3->actual_z = HIWORD(x);
              v3->object_pid = PID(OBJECT_Item,i);
              if (p->uAttributes & 0x20)
              {
                if ( !pRenderer->pRenderD3D )
                  v3->sZValue = 0;
              }
            }
          }
        }
      }
  }
}

//----- (00440639) --------------------------------------------------------
void AddBspNodeToRenderList(unsigned int node_id)
{
  BLVSector *pSector; // esi@1

  pSector = &pIndoor->pSectors[pBspRenderer->nodes[node_id].uSectorID];
  if ( pRenderer->pRenderD3D )
  {
    for (uint i = 0; i < pSector->uNumNonBSPFaces; ++i)
      //Log::Warning(L"Non-BSP face: %X", v3->pFaceIDs[v2]);
      pBspRenderer->AddFaceToRenderList_d3d(node_id, pSector->pFaceIDs[i]);
  }
  else
  {
    for (uint i = 0; i < pSector->uNumNonBSPFaces; ++i)
      pBspRenderer->AddFaceToRenderList_sw(node_id, pSector->pFaceIDs[i]);
  }
  if ( pSector->field_0 & 0x10 )
    sub_4406BC(node_id, pSector->uFirstBSPNode);
}

//----- (004406BC) --------------------------------------------------------
void __fastcall sub_4406BC(unsigned int node_id, unsigned int uFirstNode)
{
  BLVSector *pSector; // esi@2
  BSPNode *pNode; // edi@2
  BLVFace *pFace; // eax@2
  int v5; // ecx@2
  __int16 v6; // ax@6
  int v7; // ebp@10
  int v8; // ebx@10
  __int16 v9; // di@18
  //int v10; // [sp+10h] [bp-Ch]@1
  //bool v11; // [sp+14h] [bp-8h]@5
  BspRenderer_stru0 *node; // [sp+18h] [bp-4h]@1

  //Log::Warning(L"sub_4406BC(%u, %u)", a1, uFirstNode);

  //v10 = a1;
  node = &pBspRenderer->nodes[node_id];
  while ( 1 )
  {
    pSector = &pIndoor->pSectors[node->uSectorID];
    pNode = &pIndoor->pNodes[uFirstNode];
    pFace = &pIndoor->pFaces[pSector->pFaceIDs[pNode->uCoplanarOffset]];
    v5 = pFace->pFacePlane_old.dist +
         pGame->pIndoorCameraD3D->vPartyPos.x * pFace->pFacePlane_old.vNormal.x +
         pGame->pIndoorCameraD3D->vPartyPos.y * pFace->pFacePlane_old.vNormal.y +
         pGame->pIndoorCameraD3D->vPartyPos.z * pFace->pFacePlane_old.vNormal.z;//plane equation
    if (pFace->Portal() && pFace->uSectorID != node->uSectorID )
      v5 = -v5;
    //v11 = v5 > 0;
    if ( v5 <= 0 )
      v6 = pNode->uFront;
    else
      v6 = pNode->uBack;
    if ( v6 != -1 )
      sub_4406BC(node_id, v6);
    v7 = pNode->uCoplanarOffset;
    v8 = v7 + pNode->uCoplanarSize;

    //Log::Warning(L"Node %u: %X to %X (%hX)", uFirstNode, v7, v8, v2->pFaceIDs[v7]);
    
    if ( pRenderer->pRenderD3D )
    {
      while ( v7 < v8 )
        pBspRenderer->AddFaceToRenderList_d3d(node_id, pSector->pFaceIDs[v7++]);
    }
    else
    {
      while ( v7 < v8 )
        pBspRenderer->AddFaceToRenderList_sw(node_id, pSector->pFaceIDs[v7++]);
    }
    v9 = v5 > 0 ? pNode->uFront : pNode->uBack;
    if ( v9 == -1 )
      break;
    uFirstNode = v9;
  }
}
//----- (0043FA33) --------------------------------------------------------
void PrepareDecorationsRenderList_BLV(unsigned int uDecorationID, unsigned int uSectorID)
{
  LevelDecoration *v2; // esi@1
  DecorationDesc *v3; // ebx@2
  __int16 v4; // ax@2
  double v5; // st7@3
  int v6; // eax@5
  int v7; // edx@5
  unsigned int v8; // edi@5
  int v9; // edi@5
  int v10; // eax@7
  SpriteFrame *v11; // eax@7
  SpriteFrame *v12; // esi@7
  int v13; // eax@7
  int v14; // ebx@16
  RenderBillboard *v15; // ecx@17
  char v16; // zf@18
  IndoorCameraD3D **v17; // eax@19
  double v18; // st7@19
  //float v19; // eax@19
  signed __int64 v20; // qtt@19
  signed __int64 v21; // qtt@20
  //int v22; // edx@21
  //int v23; // eax@21
  Particle_sw local_0; // [sp+Ch] [bp-A0h]@3
  //double v25; // [sp+74h] [bp-38h]@19
  //unsigned int v26; // [sp+7Ch] [bp-30h]@1
  int a2; // [sp+80h] [bp-2Ch]@5
  int a3; // [sp+84h] [bp-28h]@5
  int a1; // [sp+88h] [bp-24h]@5
  int v30; // [sp+8Ch] [bp-20h]@7
  //float v31; // [sp+90h] [bp-1Ch]@1
  int a5; // [sp+94h] [bp-18h]@17
  int z; // [sp+98h] [bp-14h]@15
  int a6; // [sp+9Ch] [bp-10h]@17
  int y; // [sp+A0h] [bp-Ch]@15
  int x; // [sp+A4h] [bp-8h]@15
  int v37; // [sp+A8h] [bp-4h]@5

  //v26 = uDecorationID;
  //LODWORD(v31) = uSectorID;
  v2 = &pLevelDecorations[uDecorationID];
  if (v2->uFlags & LEVEL_DECORATION_INVISIBLE)
    return;

    v3 = &pDecorationList->pDecorations[v2->uDecorationDescID];
    v4 = v3->uFlags;
    if (v3->uFlags & DECORATION_DESC_EMITS_FIRE)
    {
      memset(&local_0, 0, 0x68u);               // fire,  like at the Pit's tavern
      v5 = (double)v2->vPosition.x;
      local_0.type = ParticleType_Bitmap | ParticleType_Rotating | ParticleType_8;
      local_0.uDiffuse = 0xFF3C1E;
      local_0.x = v5;
      local_0.y = (double)v2->vPosition.y;
      local_0.z = (double)v2->vPosition.z;
      local_0.r = 0.0;
      local_0.g = 0.0;
      local_0.b = 0.0;
      local_0.flt_28 = 1.0;
      local_0.timeToLive = (rand() & 0x80) + 128;
      local_0.uTextureID = pBitmaps_LOD->LoadTexture("effpar01");
      pGame->pParticleEngine->AddParticle(&local_0);
      return;
    }


      if (v4 & DECORATION_DESC_DONT_DRAW)
        return;

        v6 = v2->vPosition.x;
        v7 = v2->vPosition.z;
        a2 = v2->vPosition.y;
        a1 = v6;
        a3 = v7;
        v8 = v2->field_10_y_rot
           + ((signed int)stru_5C6E00->uIntegerPi >> 3)
           - stru_5C6E00->Atan2(v6 - pGame->pIndoorCameraD3D->vPartyPos.x, a2 - pGame->pIndoorCameraD3D->vPartyPos.y);
        v37 = pBLVRenderParams->field_0_timer_;
        v9 = ((signed int)(stru_5C6E00->uIntegerPi + v8) >> 8) & 7;
        if (pParty->bTurnBasedModeOn)
          v37 = pMiscTimer->uTotalGameTimeElapsed;
        v10 = abs(v2->vPosition.x + v2->vPosition.y);
        v11 = pSpriteFrameTable->GetFrame(v3->uSpriteID, v37 + v10);
        v30 = 0;
        v12 = v11;
        v13 = v11->uFlags;
        if ( v13 & 2 )
          v30 = 2;
        if ( v13 & 0x40000 )
          v30 |= 0x40u;
        if ( v13 & 0x20000 )
          LOBYTE(v30) = v30 | 0x80;
        if ( (256 << v9) & v13 )
          v30 |= 4u;
        if ( pGame->pIndoorCameraD3D->ApplyViewTransform_TrueIfStillVisible_BLV(a1, a2, a3, &x, &y, &z, 1) )
        {
          v14 = abs(x);
          if ( v14 >= abs(y) )
          {
            pGame->pIndoorCameraD3D->Project(x, y, z, &a5, &a6);

            v15 = &pBillboardRenderList[uNumBillboardsToDraw];
            assert(uNumBillboardsToDraw < 500);

              ++uNumBillboardsToDraw;
              ++uNumDecorationsDrawnThisFrame;
              v16 = pRenderer->pRenderD3D == 0;
              v15->uHwSpriteID = v12->pHwSpriteIDs[v9];
              v15->uPalette = v12->uPaletteIndex;
              v15->uIndoorSectorID = uSectorID;
              if ( v16 )
              {
                LODWORD(v21) = pBLVRenderParams->fov_rad_fixpoint << 16;
                HIDWORD(v21) = pBLVRenderParams->fov_rad_fixpoint >> 16;
                v37 = v21 / x;
                //LODWORD(v31) = v12->scale;
                v37 = v21 / x;
                v15->_screenspace_x_scaler_packedfloat = (unsigned __int64)(v12->scale * v21 / x) >> 16;
                v37 = (unsigned __int64)(v12->scale * (signed __int64)v37) >> 16;
              }
              else
              {
                v17 = &pGame->pIndoorCameraD3D;
                v15->fov_x = pGame->pIndoorCameraD3D->fov_x;
                v18 = (*v17)->fov_y;
                //v19 = v15->fov_x;
                v15->fov_y = v18;
                //v31 = v19;
                //v25 = v19 + 6.7553994e15;
                //v25 = floorf(v15->fov_x + 0.5f);
                LODWORD(v20) = 0;
                HIDWORD(v20) = floorf(v15->fov_x + 0.5f);
                v37 = v20 / x;
                //LODWORD(v31) = v12->scale;
                v37 = (unsigned __int64)(v12->scale * v20 / x) >> 16;
                v15->_screenspace_x_scaler_packedfloat = (unsigned __int64)(v12->scale * v20 / x) >> 16;
                //v31 = v15->fov_y;
                //v25 = v31 + 6.7553994e15;
                //v25 = floorf(v15->fov_y + 0.5f);
                LODWORD(v20) = 0;
                HIDWORD(v20) = floorf(v15->fov_y + 0.5f);
                v37 = v20 / x;
                v37 = (unsigned __int64)(v12->scale * v20 / x) >> 16;
              }
              //HIWORD(v22) = HIWORD(x);
              //LOWORD(v22) = 0;
              v15->_screenspace_y_scaler_packedfloat = v37;
              v15->field_1E = v30;
              v15->world_x = a1;
              v15->world_y = a2;
              v15->world_z = a3;
              v15->uScreenSpaceX = a5;
              v15->uScreenSpaceY = a6;
              //v23 = 8 * uDecorationID;
              //LOBYTE(v23) = PID(OBJECT_Decoration,uDecorationID);

              //v15->sZValue = v22 + v23;
              v15->actual_z = HIWORD(x);
              v15->object_pid = PID(OBJECT_Decoration,uDecorationID);

              v15->uTintColor = 0;
              v15->pSpriteFrame = v12;
          }
        }
}
//----- (0043F953) --------------------------------------------------------
void PrepareBspRenderList_BLV()
{
  pBspRenderer->num_faces = 0;

  if (pBLVRenderParams->uPartySectorID)
  {
    pBspRenderer->nodes[0].uSectorID = pBLVRenderParams->uPartySectorID;
    pBspRenderer->nodes[0].uViewportW = pBLVRenderParams->uViewportW;
    pBspRenderer->nodes[0].uViewportZ = pBLVRenderParams->uViewportZ;
    pBspRenderer->nodes[0].uViewportY = pBLVRenderParams->uViewportY;
    pBspRenderer->nodes[0].uViewportX = pBLVRenderParams->uViewportX;
    pBspRenderer->nodes[0].PortalScreenData.GetViewportData(pBLVRenderParams->uViewportX, pBLVRenderParams->uViewportY,
                                           pBLVRenderParams->uViewportZ, pBLVRenderParams->uViewportW);
    pBspRenderer->nodes[0].uFaceID = -1;
    pBspRenderer->nodes[0].viewing_portal_id = -1;
    pBspRenderer->num_nodes = 1;
    AddBspNodeToRenderList(0);
  }

  pBspRenderer->MakeVisibleSectorList();
}

//----- (0043F9E1) --------------------------------------------------------
void BspRenderer_PortalViewportData::GetViewportData(__int16 x, int y, __int16 z, int w)
{
  _viewport_space_y = y;
  _viewport_space_w = w;

  for (uint i = 0; i < 480; ++i)
  {
    if ( i < y || i > w )
    {
      viewport_left_side[i] = 640;
      viewport_right_side[i] = -1;
    }
    else
    {
      viewport_left_side[i] = x;
      viewport_right_side[i] = z;
    } 
  }
}
//----- (0048653D) --------------------------------------------------------
void stru149::_48653D_frustum_blv(int a2, int a3, int a4, int a5, int a6, int a7)//portal frustum culling
{
  //stru149 *v7; // esi@1
  int v8; // edi@1
  int v9; // eax@1
  //int v10; // edx@1
  //int v11; // ecx@1
  //int v12; // eax@1
  //int v13; // ebx@2
  //int v14; // ecx@2
  //int v15; // eax@2
  int v16; // ST14_4@3
  int v17; // ST10_4@3
  //int v18; // eax@5
  int v19; // ST10_4@6
  //int v20; // eax@8
  int v21; // ST10_4@9
  //int v22; // eax@10
  //int v23; // ecx@10
  //int v24; // eax@10
  //int result; // eax@10
  //int v26; // [sp+14h] [bp-14h]@1
  //int v27; // [sp+18h] [bp-10h]@1
  int v28; // [sp+1Ch] [bp-Ch]@1
  int v29; // [sp+24h] [bp-4h]@1
  //int v30; // [sp+30h] [bp+8h]@10
  //int v31; // [sp+3Ch] [bp+14h]@10

  v8 = stru_5C6E00->Cos(pGame->pIndoorCameraD3D->sRotationY);
  v29 = stru_5C6E00->Sin(pGame->pIndoorCameraD3D->sRotationY);
  v28 = stru_5C6E00->Cos(pGame->pIndoorCameraD3D->sRotationX);
  v9 = stru_5C6E00->Sin(pGame->pIndoorCameraD3D->sRotationX);
  //v11 = -pBLVRenderParams->vPartyPos.y;
  //v26 = -pBLVRenderParams->vPartyPos.x;
  //v27 = v9;
  //v12 = -pBLVRenderParams->vPartyPos.z;
  if (pGame->pIndoorCameraD3D->sRotationX)
  {
    v16 = v8 * -pGame->pIndoorCameraD3D->vPartyPos.x + v29 * -pGame->pIndoorCameraD3D->vPartyPos.y;
    v17 = -65536 * pGame->pIndoorCameraD3D->vPartyPos.z;
    this->field_0_party_dir_x = fixpoint_mul(v16, v28) + fixpoint_mul((-pGame->pIndoorCameraD3D->vPartyPos.z) << 16, v9);
    this->field_4_party_dir_y = v8 * -pGame->pIndoorCameraD3D->vPartyPos.y - v29 * -pGame->pIndoorCameraD3D->vPartyPos.x;
    this->field_8_party_dir_z = fixpoint_mul(v17, v28) - fixpoint_mul(v16, v9);
  }
  else
  {
    this->field_0_party_dir_x = v8 * -pGame->pIndoorCameraD3D->vPartyPos.x + v29 * -pGame->pIndoorCameraD3D->vPartyPos.y;
    this->field_4_party_dir_y = v8 * -pGame->pIndoorCameraD3D->vPartyPos.y - v29 * -pGame->pIndoorCameraD3D->vPartyPos.x;
    this->field_8_party_dir_z = (-pGame->pIndoorCameraD3D->vPartyPos.z) << 16;
  }

  if (pGame->pIndoorCameraD3D->sRotationX)
  {
    v19 = fixpoint_mul(a2, v8) + fixpoint_mul(a3, v29);

    this->angle_from_north = fixpoint_mul(v19, v28) + fixpoint_mul(a4, v9);
    this->angle_from_west = fixpoint_mul(a3, v8) - fixpoint_mul(a2, v29);
    this->viewing_angle_from_west_east = fixpoint_mul(a4, v28) - fixpoint_mul(v19, v9);
  }
  else
  {
    this->angle_from_north = fixpoint_mul(a2, v8) + fixpoint_mul(a3, v29);
    this->angle_from_west = fixpoint_mul(a3, v8) - fixpoint_mul(a2, v29);
    this->viewing_angle_from_west_east = a4;
  }

  if (pGame->pIndoorCameraD3D->sRotationX)
  {
    v21 = fixpoint_mul(a5, v8) + fixpoint_mul(a6, v29);

    this->angle_from_east = fixpoint_mul(v21, v28) + fixpoint_mul(a7, v9);
    this->angle_from_south = fixpoint_mul(a6, v8) - fixpoint_mul(a5, v29);
    this->viewing_angle_from_north_south = fixpoint_mul(a7, v28) - fixpoint_mul(v21, v9);
  }
  else
  {
    this->angle_from_east = fixpoint_mul(a5, v8) + fixpoint_mul(a6, v29);
    this->angle_from_south = fixpoint_mul(a6, v8) - fixpoint_mul(a5, v29);
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
//----- (00407A1C) --------------------------------------------------------
bool __fastcall sub_407A1C(int x, int z, int y, Vec3_int_ v)
{
  unsigned int v4; // esi@1
  Vec3_int_ v5; // ST08_12@2
  int v6; // edi@2
  int v7; // ebx@2
  int v8; // esi@2
  signed int v9; // ecx@2
  int v10; // eax@2
  int v11; // ecx@4
  int v12; // eax@4
  int v13; // ebx@4
  int v14; // edx@6
  char *v15; // edi@16
  ODMFace *v16; // esi@19
  int v17; // ST34_4@25
  int v18; // ST38_4@25
  int v19; // eax@25
  char v20; // zf@25
  int v21; // ebx@25
  int v22; // eax@26
  signed int v23; // edi@26
  int v24; // ST34_4@30
  //signed __int64 v25; // qtt@31
  //int v26; // eax@31
  Vec3_int_ v27; // ST08_12@37
  Vec3_int_ v28; // ST08_12@37
  int v29; // edi@37
  int v30; // ebx@37
  int v31; // esi@37
  signed int v32; // ecx@37
  int v33; // eax@37
  int v34; // ecx@39
  int v35; // eax@39
  int v36; // ebx@39
  int v37; // edx@41
  char *v38; // edi@51
  ODMFace *v39; // esi@54
  int v40; // ebx@60
  int v41; // eax@61
  signed int v42; // edi@61
  //signed __int64 v43; // qtt@66
  //int v44; // eax@66
  Vec3_int_ v45; // ST08_12@73
  int v46; // edi@73
  int v47; // ebx@73
  int v48; // esi@73
  signed int v49; // ecx@73
  int v50; // eax@73
  int v51; // edx@75
  int v52; // ecx@75
  int v53; // eax@75
  int v54; // ebx@75
  int v55; // edi@77
  int v56; // ecx@77
  int v57; // eax@81
  int v58; // esi@81
  int v59; // eax@90
  BLVSector *v60; // edx@90
  int v61; // ecx@90
  BLVFace *v62; // esi@91
  int v63; // ST34_4@98
  int v64; // ST30_4@98
  int v65; // eax@98
  int v66; // ebx@98
  int v67; // eax@99
  signed int v68; // edi@99
  int v69; // ST2C_4@103
  //signed __int64 v70; // qtt@104
  //int v71; // eax@104
  Vec3_int_ v72; // ST08_12@111
  Vec3_int_ v73; // ST08_12@111
  int v74; // edi@111
  int v75; // ebx@111
  int v76; // esi@111
  signed int v77; // ecx@111
  int v78; // eax@111
  int v79; // edx@113
  int v80; // ecx@113
  int v81; // eax@113
  int v82; // ebx@113
  int v83; // edi@115
  int v84; // ecx@115
  int v85; // eax@119
  int v86; // esi@119
  int v87; // ecx@128
  BLVSector *v88; // eax@128
  int v89; // ecx@128
  BLVFace *v90; // esi@129
  int v91; // ebx@136
  int v92; // eax@137
  signed int v93; // edi@137
  //signed __int64 v94; // qtt@142
  //int v95; // eax@142
  Vec3_int_ v97; // [sp-18h] [bp-94h]@1
  int v98; // [sp-Ch] [bp-88h]@88
  int v99; // [sp-Ch] [bp-88h]@126
  int v100; // [sp-8h] [bp-84h]@88
  int v101; // [sp-8h] [bp-84h]@126
  int v102; // [sp-4h] [bp-80h]@88
  int v103; // [sp-4h] [bp-80h]@126
  int v104; // [sp+Ch] [bp-70h]@13
  int v105; // [sp+Ch] [bp-70h]@48
  int v106; // [sp+10h] [bp-6Ch]@18
  int v107; // [sp+10h] [bp-6Ch]@98
  int v108; // [sp+10h] [bp-6Ch]@104
  int v109; // [sp+18h] [bp-64h]@25
  int v110; // [sp+18h] [bp-64h]@31
  int i; // [sp+18h] [bp-64h]@90
  int v112; // [sp+18h] [bp-64h]@128
  signed int v113; // [sp+20h] [bp-5Ch]@1
  signed int v114; // [sp+24h] [bp-58h]@1
  //unsigned __int64 a4; // [sp+28h] [bp-54h]@1
  unsigned int a4_8; // [sp+30h] [bp-4Ch]@1
  int v117; // [sp+34h] [bp-48h]@4
  int v118; // [sp+34h] [bp-48h]@39
  int v119; // [sp+34h] [bp-48h]@75
  int v120; // [sp+34h] [bp-48h]@113
  int v121; // [sp+38h] [bp-44h]@4
  int v122; // [sp+38h] [bp-44h]@39
  int v123; // [sp+38h] [bp-44h]@76
  int v124; // [sp+38h] [bp-44h]@114
  int v125; // [sp+3Ch] [bp-40h]@4
  int v126; // [sp+3Ch] [bp-40h]@39
  int v127; // [sp+3Ch] [bp-40h]@77
  int v128; // [sp+3Ch] [bp-40h]@115
  int v129; // [sp+40h] [bp-3Ch]@11
  int v130; // [sp+40h] [bp-3Ch]@46
  int v131; // [sp+40h] [bp-3Ch]@78
  int v132; // [sp+40h] [bp-3Ch]@116
  int v133; // [sp+44h] [bp-38h]@10
  int v134; // [sp+44h] [bp-38h]@45
  int v135; // [sp+44h] [bp-38h]@81
  int v136; // [sp+44h] [bp-38h]@119
  int v137; // [sp+48h] [bp-34h]@7
  int v138; // [sp+48h] [bp-34h]@42
  int v139; // [sp+48h] [bp-34h]@82
  int v140; // [sp+48h] [bp-34h]@120
  int v141; // [sp+4Ch] [bp-30h]@6
  int v142; // [sp+4Ch] [bp-30h]@41
  int v143; // [sp+4Ch] [bp-30h]@75
  int v144; // [sp+4Ch] [bp-30h]@113
  int v145; // [sp+50h] [bp-2Ch]@5
  int v146; // [sp+50h] [bp-2Ch]@40
  int v147; // [sp+50h] [bp-2Ch]@75
  int v148; // [sp+50h] [bp-2Ch]@113
  int v149; // [sp+54h] [bp-28h]@4
  int v150; // [sp+54h] [bp-28h]@39
  int v151; // [sp+54h] [bp-28h]@75
  int v152; // [sp+54h] [bp-28h]@113
  int sDepth; // [sp+58h] [bp-24h]@17
  int sDeptha; // [sp+58h] [bp-24h]@52
  int sDepthb; // [sp+58h] [bp-24h]@90
  char *a5; // [sp+5Ch] [bp-20h]@16
  char *a5a; // [sp+5Ch] [bp-20h]@51
  signed int a5b; // [sp+5Ch] [bp-20h]@83
  signed int a5c; // [sp+5Ch] [bp-20h]@121
  signed int v160; // [sp+60h] [bp-1Ch]@12
  signed int v161; // [sp+60h] [bp-1Ch]@47
  int v162; // [sp+60h] [bp-1Ch]@128
  int v163; // [sp+64h] [bp-18h]@2
  int outx; // [sp+68h] [bp-14h]@2
  int outy; // [sp+6Ch] [bp-10h]@2
  int outz; // [sp+70h] [bp-Ch]@2
  Vec3_int_ pOut; // [sp+74h] [bp-8h]@2
  int ya; // [sp+84h] [bp+8h]@60
  int yb; // [sp+84h] [bp+8h]@136
  int ve; // [sp+88h] [bp+Ch]@60
  int va; // [sp+88h] [bp+Ch]@60
  int vb; // [sp+88h] [bp+Ch]@66
  int vf; // [sp+88h] [bp+Ch]@136
  int vc; // [sp+88h] [bp+Ch]@136
  int vd; // [sp+88h] [bp+Ch]@142
  int v_4; // [sp+8Ch] [bp+10h]@60
  int v_4a; // [sp+8Ch] [bp+10h]@65
  int v_4b; // [sp+8Ch] [bp+10h]@136
  int v_4c; // [sp+8Ch] [bp+10h]@141
  int v_8; // [sp+90h] [bp+14h]@53

  //__debugbreak();

  v4 = stru_5C6E00->Atan2(v.x - x, v.y - z);
  v114 = 0;
  v97.z = y;
  v97.x = x;
  v97.y = z;
  v113 = 0;
  a4_8 = v4;
  if ( uCurrentlyLoadedLevelType != LEVEL_Outdoor)
  {
    Vec3_int_::Rotate(32, stru_5C6E00->uIntegerHalfPi + v4, 0, v97, &pOut.x, &pOut.y, &outz);
    v45.z = v.z;
    v45.x = v.x;
    v45.y = v.y;
    Vec3_int_::Rotate(32, stru_5C6E00->uIntegerHalfPi + v4, 0, v45, &outx, &outy, &v163);
    v46 = outy - pOut.y;
    v47 = v163 - outz;
    v48 = outx - pOut.x;
    v49 = integer_sqrt(v48 * v48 + v46 * v46 + v47 * v47);
    v50 = 65536;
    if ( v49 )
      v50 = 65536 / v49;
    v51 = outx;
    v143 = v48 * v50;
    v52 = v46 * v50;
    v53 = v47 * v50;
    v54 = pOut.x;
    v147 = v52;
    v151 = v53;
    v119 = pOut.x;
    if ( pOut.x < outx )
    {
      v123 = outx;
    }
    else
    {
      v119 = outx;
      v123 = pOut.x;
    }
    v55 = pOut.y;
    v56 = outy;
    v127 = pOut.y;
    if ( pOut.y < outy )
    {
      v131 = outy;
    }
    else
    {
      v127 = outy;
      v131 = pOut.y;
    }
    v57 = v163;
    v58 = outz;
    v135 = outz;
    if ( outz < v163 )
    {
      v139 = v163;
    }
    else
    {
      v135 = v163;
      v139 = outz;
    }
    a5b = 0;
    while ( !v114 )
    {
      if ( a5b )
      {
        v102 = v58;
        v100 = v55;
        v98 = v54;
      }
      else
      {
        v102 = v57;
        v100 = v56;
        v98 = v51;
      }
      v59 = pIndoor->GetSector(v98, v100, v102);
      v60 = pIndoor->pSectors;
      v61 = 116 * v59;
      sDepthb = 0;
      for ( i = 116 * v59;
            sDepthb < *(__int16 *)((char *)&pIndoor->pSectors->uNumWalls + v61)
                    + 2 * *(__int16 *)((char *)&pIndoor->pSectors->uNumFloors + v61);
            ++sDepthb )
      {
        v62 = &pIndoor->pFaces[(*(unsigned __int16 **)((char *)&v60->pWalls + v61))[sDepthb]];
        if ( v62->Portal()
          || v119 > v62->pBounding.x2
          || v123 < v62->pBounding.x1
          || v127 > v62->pBounding.y2
          || v131 < v62->pBounding.y1
          || v135 > v62->pBounding.z2
          || v139 < v62->pBounding.z1
          || (v63 = (unsigned __int64)(v143 * (signed __int64)v62->pFacePlane_old.vNormal.x) >> 16,
              v64 = (unsigned __int64)(v151 * (signed __int64)v62->pFacePlane_old.vNormal.z) >> 16,
              v65 = (unsigned __int64)(v147 * (signed __int64)v62->pFacePlane_old.vNormal.y) >> 16,
              v20 = v63 + v64 + v65 == 0,
              v66 = v63 + v64 + v65,
              v107 = v63 + v64 + v65,
              v20) )
          goto LABEL_107;
        v67 = outz * v62->pFacePlane_old.vNormal.z;
        v68 = -(v62->pFacePlane_old.dist
              + v67
              + pOut.y * v62->pFacePlane_old.vNormal.y
              + pOut.x * v62->pFacePlane_old.vNormal.x);
        if ( v66 <= 0 )
        {
          if ( v62->pFacePlane_old.dist
             + v67
             + pOut.y * v62->pFacePlane_old.vNormal.y
             + pOut.x * v62->pFacePlane_old.vNormal.x < 0 )
            goto LABEL_107;
        }
        else
        {
          if ( v62->pFacePlane_old.dist
             + v67
             + pOut.y * v62->pFacePlane_old.vNormal.y
             + pOut.x * v62->pFacePlane_old.vNormal.x > 0 )
            goto LABEL_107;
        }
        v69 = abs(-(v62->pFacePlane_old.dist
                  + v67
                  + pOut.y * v62->pFacePlane_old.vNormal.y
                  + pOut.x * v62->pFacePlane_old.vNormal.x)) >> 14;
        if ( v69 <= abs(v66) )
        {
          //LODWORD(v70) = v68 << 16;
          //HIDWORD(v70) = v68 >> 16;
          //v71 = v70 / v107;
          //v108 = v70 / v107;
          v108 = fixpoint_div(v68, v107);
          if ( v108 >= 0 )
          {
            if ( sub_4075DB(
                   pOut.x + ((signed int)(fixpoint_mul(v108, v143) + 32768) >> 16),
                   pOut.y + ((signed int)(fixpoint_mul(v108, v147) + 32768) >> 16),
                   outz + ((signed int)(fixpoint_mul(v108, v151) + 32768) >> 16),
                   v62) )
            {
              v114 = 1;
              break;
            }
          }
        }
        v61 = i;
LABEL_107:
        v60 = pIndoor->pSectors;
      }
      ++a5b;
      if ( a5b >= 2 )
        break;
      v57 = v163;
      v56 = outy;
      v51 = outx;
      v58 = outz;
      v55 = pOut.y;
      v54 = pOut.x;
    }
    v72.z = y;
    v72.x = x;
    v72.y = z;
    Vec3_int_::Rotate(32, a4_8 - stru_5C6E00->uIntegerHalfPi, 0, v72, &pOut.x, &pOut.y, &outz);
    v73.z = v.z;
    v73.x = v.x;
    v73.y = v.y;
    Vec3_int_::Rotate(32, a4_8 - stru_5C6E00->uIntegerHalfPi, 0, v73, &outx, &outy, &v163);
    v74 = outy - pOut.y;
    v75 = v163 - outz;
    v76 = outx - pOut.x;
    v77 = integer_sqrt(v76 * v76 + v74 * v74 + v75 * v75);
    v78 = 65536;
    if ( v77 )
      v78 = 65536 / v77;
    v79 = outx;
    v144 = v76 * v78;
    v80 = v74 * v78;
    v81 = v75 * v78;
    v82 = pOut.x;
    v148 = v80;
    v152 = v81;
    v120 = pOut.x;
    if ( pOut.x < outx )
    {
      v124 = outx;
    }
    else
    {
      v120 = outx;
      v124 = pOut.x;
    }
    v83 = pOut.y;
    v84 = outy;
    v128 = pOut.y;
    if ( pOut.y < outy )
    {
      v132 = outy;
    }
    else
    {
      v128 = outy;
      v132 = pOut.y;
    }
    v85 = v163;
    v86 = outz;
    v136 = outz;
    if ( outz < v163 )
    {
      v140 = v163;
    }
    else
    {
      v136 = v163;
      v140 = outz;
    }
    a5c = 0;
    while ( 1 )
    {
      if ( v113 )
        return !v114 || !v113;
      if ( a5c )
      {
        v103 = v86;
        v101 = v83;
        v99 = v82;
      }
      else
      {
        v103 = v85;
        v101 = v84;
        v99 = v79;
      }
      v87 = pIndoor->GetSector(v99, v101, v103);
      v88 = pIndoor->pSectors;
      v89 = 116 * v87;
      v162 = 0;
      v112 = v89;
      if ( *(__int16 *)((char *)&pIndoor->pSectors->uNumWalls + v89)
         + 2 * *(__int16 *)((char *)&pIndoor->pSectors->uNumFloors + v89) > 0 )
        break;
LABEL_148:
      ++a5c;
      if ( a5c >= 2 )
        return !v114 || !v113;
      v85 = v163;
      v84 = outy;
      v79 = outx;
      v86 = outz;
      v83 = pOut.y;
      v82 = pOut.x;
    }
    while ( 1 )
    {
      v90 = &pIndoor->pFaces[(*(unsigned __int16 **)((char *)&v88->pWalls + v89))[v162]];
      if ( v90->Portal()
        || v120 > v90->pBounding.x2
        || v124 < v90->pBounding.x1
        || v128 > v90->pBounding.y2
        || v132 < v90->pBounding.y1
        || v136 > v90->pBounding.z2
        || v140 < v90->pBounding.z1
        || (yb = fixpoint_mul(v144, v90->pFacePlane_old.vNormal.x),
            v_4b = fixpoint_mul(v148, v90->pFacePlane_old.vNormal.y),
            vf = fixpoint_mul(v152, v90->pFacePlane_old.vNormal.z),
            v20 = yb + vf + v_4b == 0,
            v91 = yb + vf + v_4b,
            vc = yb + vf + v_4b,
            v20) )
        goto LABEL_145;
      v92 = outz * v90->pFacePlane_old.vNormal.z;
      v93 = -(v90->pFacePlane_old.dist
            + v92
            + pOut.y * v90->pFacePlane_old.vNormal.y
            + pOut.x * v90->pFacePlane_old.vNormal.x);
      if ( v91 <= 0 )
      {
        if ( v90->pFacePlane_old.dist
           + v92
           + pOut.y * v90->pFacePlane_old.vNormal.y
           + pOut.x * v90->pFacePlane_old.vNormal.x < 0 )
          goto LABEL_145;
      }
      else
      {
        if ( v90->pFacePlane_old.dist
           + v92
           + pOut.y * v90->pFacePlane_old.vNormal.y
           + pOut.x * v90->pFacePlane_old.vNormal.x > 0 )
          goto LABEL_145;
      }
      v_4c = abs(-(v90->pFacePlane_old.dist
                 + v92
                 + pOut.y * v90->pFacePlane_old.vNormal.y
                 + pOut.x * v90->pFacePlane_old.vNormal.x)) >> 14;
      if ( v_4c <= abs(v91) )
      {
        //LODWORD(v94) = v93 << 16;
        //HIDWORD(v94) = v93 >> 16;
        //v95 = v94 / vc;
        //vd = v94 / vc;
        vd = fixpoint_div(v93, vc);
        if ( vd >= 0 )
        {
          if ( sub_4075DB(
                 pOut.x + ((signed int)(fixpoint_mul(vd, v144) + 32768) >> 16),
                 pOut.y + ((signed int)(fixpoint_mul(vd, v148) + 32768) >> 16),
                 outz + ((signed int)(fixpoint_mul(vd, v152) + 32768) >> 16),
                 v90) )
          {
            v113 = 1;
            goto LABEL_148;
          }
        }
      }
      v89 = v112;
LABEL_145:
      v88 = pIndoor->pSectors;
      ++v162;
      if ( v162 >= *(__int16 *)((char *)&pIndoor->pSectors->uNumWalls + v89)
                 + 2 * *(__int16 *)((char *)&pIndoor->pSectors->uNumFloors + v89) )
        goto LABEL_148;
    }
  }
  Vec3_int_::Rotate(32, stru_5C6E00->uIntegerHalfPi + v4, 0, v97, &pOut.x, &pOut.y, &outz);
  v5.z = v.z;
  v5.x = v.x;
  v5.y = v.y;
  Vec3_int_::Rotate(32, stru_5C6E00->uIntegerHalfPi + v4, 0, v5, &outx, &outy, &v163);
  v6 = outy - pOut.y;
  v7 = v163 - outz;
  v8 = outx - pOut.x;
  v9 = integer_sqrt(v8 * v8 + v6 * v6 + v7 * v7);
  v10 = 65536;
  if ( v9 )
    v10 = 65536 / v9;
  v125 = v8 * v10;
  v11 = v10;
  v12 = v7 * v10;
  v13 = pOut.x;
  v117 = v12;
  v121 = v6 * v11;
  v149 = pOut.x;
  if ( pOut.x < outx )
  {
    v145 = outx;
  }
  else
  {
    v149 = outx;
    v145 = pOut.x;
  }
  v14 = outy;
  v141 = pOut.y;
  if ( pOut.y < outy )
  {
    v137 = outy;
  }
  else
  {
    v141 = outy;
    v137 = pOut.y;
  }
  v133 = outz;
  if ( outz < v163 )
  {
    v129 = v163;
  }
  else
  {
    v133 = v163;
    v129 = outz;
  }
  v160 = 0;
  if ( (signed int)pOutdoor->uNumBModels > 0 )
  {
    v104 = 0;
    while ( 1 )
    {
      v15 = (char *)&pOutdoor->pBModels[v104].pVertices;
      a5 = (char *)&pOutdoor->pBModels[v104].pVertices;
      if ( sub_4088E9(v13, pOut.y, outx, v14, pOutdoor->pBModels[v104].vPosition.x, pOutdoor->pBModels[v104].vPosition.y) <= pOutdoor->pBModels[v104].sBoundingRadius + 128 )
      {
        sDepth = 0;
        if ( *((int *)v15 + 2) > 0 )
          break;
      }
LABEL_36:
      ++v160;
      ++v104;
      if ( v160 >= (signed int)pOutdoor->uNumBModels )
        goto LABEL_37;
      v14 = outy;
      v13 = pOut.x;
    }
    v106 = 0;
    while ( 1 )
    {
      v16 = (ODMFace *)(v106 + *((int *)a5 + 4));
      if ( v149 > v16->pBoundingBox.x2
        || v145 < v16->pBoundingBox.x1
        || v141 > v16->pBoundingBox.y2
        || v137 < v16->pBoundingBox.y1
        || v133 > v16->pBoundingBox.z2
        || v129 < v16->pBoundingBox.z1
        || (v17 = fixpoint_mul(v125, v16->pFacePlane.vNormal.x),
            v18 = fixpoint_mul(v121, v16->pFacePlane.vNormal.y),
            v19 = fixpoint_mul(v117, v16->pFacePlane.vNormal.z),
            v20 = v17 + v18 + v19 == 0,
            v21 = v17 + v18 + v19,
            v109 = v17 + v18 + v19,
            v20) )
        goto LABEL_33;
      v22 = pOut.y * v16->pFacePlane.vNormal.y;
      v23 = -(v16->pFacePlane.dist + v22 + outz * v16->pFacePlane.vNormal.z + pOut.x * v16->pFacePlane.vNormal.x);
      if ( v21 <= 0 )
      {
        if ( v16->pFacePlane.dist + v22 + outz * v16->pFacePlane.vNormal.z + pOut.x * v16->pFacePlane.vNormal.x < 0 )
          goto LABEL_33;
      }
      else
      {
        if ( v16->pFacePlane.dist + v22 + outz * v16->pFacePlane.vNormal.z + pOut.x * v16->pFacePlane.vNormal.x > 0 )
          goto LABEL_33;
      }
      v24 = abs(-(v16->pFacePlane.dist + v22 + outz * v16->pFacePlane.vNormal.z + pOut.x * v16->pFacePlane.vNormal.x)) >> 14;
      if ( v24 <= abs(v21) )
      {
        //LODWORD(v25) = v23 << 16;
        //HIDWORD(v25) = v23 >> 16;
        //v26 = v25 / v109;
        //v110 = v25 / v109;
        v110 = fixpoint_div(v23, v109);
        if ( v110 >= 0 )
        {
          if ( sub_4077F1(
                 pOut.x + ((signed int)(fixpoint_mul(v110, v125) + 32768) >> 16),
                 pOut.y + ((signed int)(fixpoint_mul(v110, v121) + 32768) >> 16),
                 outz + ((signed int)(fixpoint_mul(v110, v117) + 32768) >> 16),
                 v16,
                 (BSPVertexBuffer *)a5) )
          {
            v114 = 1;
            goto LABEL_36;
          }
        }
      }
LABEL_33:
      ++sDepth;
      v106 += 308;
      if ( sDepth >= *((int *)a5 + 2) )
        goto LABEL_36;
    }
  }
LABEL_37:
  v27.z = y;
  v27.x = x;
  v27.y = z;
  Vec3_int_::Rotate(32, a4_8 - stru_5C6E00->uIntegerHalfPi, 0, v27, &pOut.x, &pOut.y, &outz);
  v28.z = v.z;
  v28.x = v.x;
  v28.y = v.y;
  Vec3_int_::Rotate(32, a4_8 - stru_5C6E00->uIntegerHalfPi, 0, v28, &outx, &outy, &v163);
  v29 = outy - pOut.y;
  v30 = v163 - outz;
  v31 = outx - pOut.x;
  v32 = integer_sqrt(v31 * v31 + v29 * v29 + v30 * v30);
  v33 = 65536;
  if ( v32 )
    v33 = 65536 / v32;
  v126 = v31 * v33;
  v34 = v33;
  v35 = v30 * v33;
  v36 = pOut.x;
  v118 = v35;
  v122 = v29 * v34;
  v150 = pOut.x;
  if ( pOut.x < outx )
  {
    v146 = outx;
  }
  else
  {
    v150 = outx;
    v146 = pOut.x;
  }
  v37 = outy;
  v142 = pOut.y;
  if ( pOut.y < outy )
  {
    v138 = outy;
  }
  else
  {
    v142 = outy;
    v138 = pOut.y;
  }
  v134 = outz;
  if ( outz < v163 )
  {
    v130 = v163;
  }
  else
  {
    v134 = v163;
    v130 = outz;
  }
  v161 = 0;
  if ( (signed int)pOutdoor->uNumBModels > 0 )
  {
    v105 = 0;
    while ( 1 )
    {
      v38 = (char *)&pOutdoor->pBModels[v105].pVertices;
      a5a = (char *)&pOutdoor->pBModels[v105].pVertices;
      if ( sub_4088E9(v36, pOut.y, outx, v37, pOutdoor->pBModels[v105].vPosition.x, pOutdoor->pBModels[v105].vPosition.y) <= pOutdoor->pBModels[v105].sBoundingRadius + 128 )
      {
        sDeptha = 0;
        if ( *((int *)v38 + 2) > 0 )
          break;
      }
LABEL_71:
      ++v161;
      ++v105;
      if ( v161 >= (signed int)pOutdoor->uNumBModels )
        return !v114 || !v113;
      v37 = outy;
      v36 = pOut.x;
    }
    v_8 = 0;
    while ( 1 )
    {
      v39 = (ODMFace *)(v_8 + *((int *)a5a + 4));
      if ( v150 > v39->pBoundingBox.x2
        || v146 < v39->pBoundingBox.x1
        || v142 > v39->pBoundingBox.y2
        || v138 < v39->pBoundingBox.y1
        || v134 > v39->pBoundingBox.z2
        || v130 < v39->pBoundingBox.z1
        || (ya = fixpoint_mul(v126, v39->pFacePlane.vNormal.x),
            ve = fixpoint_mul(v122, v39->pFacePlane.vNormal.y),
            v_4 = fixpoint_mul(v118, v39->pFacePlane.vNormal.z),
            v20 = ya + ve + v_4 == 0,
            v40 = ya + ve + v_4,
            va = ya + ve + v_4,
            v20) )
        goto LABEL_68;
      v41 = pOut.y * v39->pFacePlane.vNormal.y;
      v42 = -(v39->pFacePlane.dist + v41 + outz * v39->pFacePlane.vNormal.z + pOut.x * v39->pFacePlane.vNormal.x);
      if ( v40 <= 0 )
      {
        if ( v39->pFacePlane.dist + v41 + outz * v39->pFacePlane.vNormal.z + pOut.x * v39->pFacePlane.vNormal.x < 0 )
          goto LABEL_68;
      }
      else
      {
        if ( v39->pFacePlane.dist + v41 + outz * v39->pFacePlane.vNormal.z + pOut.x * v39->pFacePlane.vNormal.x > 0 )
          goto LABEL_68;
      }
      v_4a = abs(-(v39->pFacePlane.dist + v41 + outz * v39->pFacePlane.vNormal.z + pOut.x * v39->pFacePlane.vNormal.x)) >> 14;
      if ( v_4a <= abs(v40) )
      {
        //LODWORD(v43) = v42 << 16;
        //HIDWORD(v43) = v42 >> 16;
        //vb = v43 / va;
        vb = fixpoint_div(v42, va);
        if ( vb >= 0 )
        {
          if ( sub_4077F1(
                 pOut.x + ((signed int)(fixpoint_mul(vb, v126) + 32768) >> 16),
                 pOut.y + ((signed int)(fixpoint_mul(vb, v122) + 32768) >> 16),
                 outz + ((signed int)(fixpoint_mul(vb, v118) + 32768) >> 16),
                 v39,
                 (BSPVertexBuffer *)a5a) )
          {
            v113 = 1;
            goto LABEL_71;
          }
        }
      }
LABEL_68:
      ++sDeptha;
      v_8 += 308;
      if ( sDeptha >= *((int *)a5a + 2) )
        goto LABEL_71;
    }
  }
  return !v114 || !v113;
}
//----- (0043F333) --------------------------------------------------------
void BspRenderer::MakeVisibleSectorList()
{
  int v6; // ebx@3

  uNumVisibleNotEmptySectors = 0;
  for (uint i = 0; i < num_nodes; ++i)
  {
      if (!uNumVisibleNotEmptySectors)
      {
        pVisibleSectorIDs_toDrawDecorsActorsEtcFrom[uNumVisibleNotEmptySectors++] = nodes[i].uSectorID;
        continue;
      }
      
      v6 = 0;
        while (pVisibleSectorIDs_toDrawDecorsActorsEtcFrom[v6] != nodes[i].uSectorID )
        {
          ++v6;
          if ( v6 >= uNumVisibleNotEmptySectors)
          {
            pVisibleSectorIDs_toDrawDecorsActorsEtcFrom[uNumVisibleNotEmptySectors++] = nodes[i].uSectorID;
          }
        }

  }
}
//----- (0046A334) --------------------------------------------------------
char __fastcall DoInteractionWithTopmostZObject(int a1, int a2)
{
  int v2; // edx@1
  BLVFace *v4; // eax@9
  unsigned int v5; // ecx@9
  unsigned __int16 v6; // ax@11
  //ODMFace *v7; // eax@16
  LevelDecoration *v8; // esi@19
  __int16 v9; // ax@19
  int v10; // eax@22
  int v11; // ecx@22
  int v12; // edi@23
  Actor *v13; // esi@23
  unsigned __int16 v14; // ax@23
  unsigned __int16 v15; // ax@33
  const char *v16; // eax@34
  int v17; // edi@36
  int v18; // eax@36
  ItemGen *v19; // esi@39
  unsigned int v20; // eax@39
  int v21; // ecx@40
  std::string v22; // [sp-18h] [bp-2Ch]@5
  const char *v23; // [sp-8h] [bp-1Ch]@5
  int v24; // [sp-4h] [bp-18h]@5
  char v25; // [sp+8h] [bp-Ch]@5
  int v26; // [sp+Ch] [bp-8h]@1
  int a3; // [sp+13h] [bp-1h]@5

  v26 = a2;
  v2 = a1;
  switch ( PID_TYPE(a1) )
  {
    case OBJECT_Item: // take the item
      v17 = PID_ID(a1);
      v26 = PID_ID(a1);
      v18 = PID_ID(a1);
      if ( pObjectList->pObjects[pSpriteObjects[v18].uObjectDescID].uFlags & 0x10
        || v17 >= 1000
        || !pSpriteObjects[v18].uObjectDescID )
        return 1;
      v19 = &pSpriteObjects[v18].stru_24;
      v20 = pSpriteObjects[v18].stru_24.uItemID;
      if ( pItemsTable->pItems[v20].uEquipType == EQUIP_GOLD)
      {
        pParty->PartyFindsGold(v19->uSpecEnchantmentType, 0);
        viewparams->bRedrawGameUI = 1;
        v21 = v17;
      }
      else
      {
        if ( pParty->pPickedItem.uItemID )
          return 1;
        v24 = (int)pItemsTable->pItems[v20].pUnidentifiedName;
        sprintfex(pTmpBuf2.data(), pGlobalTXT_LocalizationStrings[471], v24);
        ShowStatusBarString(pTmpBuf2.data(), 2u);
        if ( v19->uItemID == 506 )
          _449B7E_toggle_bit(pParty->_quest_bits, 184, 1u);
        if ( v19->uItemID == 455 )
          _449B7E_toggle_bit(pParty->_quest_bits, 185, 1u);
        if ( !pParty->AddItemToParty(v19) )
          pParty->SetHoldingItem(v19);
        v21 = v26;
      }
      SpriteObject::OnInteraction(v21);
      break;

    case OBJECT_Actor:
      v12 = PID_ID(a1);
      v13 = &pActors[PID_ID(a1)];
      v14 = v13->uAIState;
      if ( v14 == 4 || v14 == 17 )
        return 1;
      if ( v14 == 5 )
      {
        stru_50C198.LootActor(&pActors[PID_ID(a1)]);
      }
      else
      {
        if ( !v13->GetActorsRelation(0) && !(BYTE2(v13->uAttributes) & 8) && v13->CanAct() )
        {
          Actor::AI_FaceObject(v12, 4u, 0, 0);
          if ( v13->sNPC_ID )
          {
            pMessageQueue_50CBD0->AddMessage(UIMSG_StartNPCDialogue, v12, 0);
          }
          else
          {
            v15 = pNPCStats->pGroups_copy[v13->uGroup];
            if ( v15 )
            {
              v16 = pNPCStats->pCatchPhrases[v15];
              if ( v16 )
              {
                pParty->uFlags |= 2u;
                strcpy(byte_5B0938.data(), v16);
                sub_4451A8_press_any_key(0, 0, 0);
              }
            }
          }
        }
      }
      break;

    case OBJECT_Decoration:
      v8 = &pLevelDecorations[PID_ID(a1)];
      v9 = v8->uEventID;
      if ( v9 )
      {
        EventProcessor(v9, a1, 1);
        v8->uFlags |= LEVEL_DECORATION_VISIBLE_ON_MAP;
      }
      else
      {
        if ( !pLevelDecorations[PID_ID(a1)].IsInteractive() )
          return 1;
        v10 = v8->_idx_in_stru123;
        v24 = 1;
        v11 = stru_5E4C90_MapPersistVars._decor_events[v10 - 75] + 380;
        activeLevelDecoration = v8;
        EventProcessor(v11, 0, 1);
        activeLevelDecoration = NULL;
      }
      break;

    default:
      MessageBoxW(nullptr, L"Warning: Invalid ID reached!", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Mouse.cpp:2020", 0);
      return 1;

    case OBJECT_BModel:
      if ( uCurrentlyLoadedLevelType == LEVEL_Outdoor )
      {
        int bmodel_id = a1 >> 9,
            face_id = PID_ID(a1) & 0x3F;
        if (bmodel_id >= pOutdoor->uNumBModels)
          return 1;
        auto face = &pOutdoor->pBModels[bmodel_id].pFaces[face_id];
        if (face->uAttributes & 0x100000 || face->sCogTriggeredID == 0 )
          return 1;
        EventProcessor((signed __int16)face->sCogTriggeredID, v2, 1);
      }
      else
      {
        v4 = &pIndoor->pFaces[PID_ID(a1)];
        v5 = v4->uAttributes;
        if ( !(v5 & 0x2000000) )
        {
          ShowNothingHereStatus();
          return 1;
        }
        if ( v5 & 0x100000 || (v6 = pIndoor->pFaceExtras[v4->uFaceExtraID].uEventID) == 0 )
          return 1;
        if ( pCurrentScreen != SCREEN_BRANCHLESS_NPC_DIALOG )
          EventProcessor((signed __int16)v6, v2, 1);
      }
      return 0;
      break;
  }
  return 0;
}
//----- (0046BDF1) --------------------------------------------------------
void  BLV_UpdateUserInputAndOther()
{
  UpdateObjects();
  BLV_ProcessPartyActions();
  UpdateActors_BLV();
  BLV_UpdateDoors();
  check_event_triggers();
}
//----- (00424829) --------------------------------------------------------
// Finds out if current portal can be seen through the previous portal
bool PortalFrustrum(int pNumVertices, BspRenderer_PortalViewportData *far_portal, BspRenderer_PortalViewportData *near_portal, int uFaceID)
{
  int min_y; // esi@5
  int max_y; // edx@5
  int current_ID; // eax@12
  int v13; // eax@22
  int v15; // ecx@29
  int v18; // eax@39
  int v19; // eax@44
  int v20; // ecx@44
  int v22; // edi@46
  int v24; // edx@48
  int v26; // eax@55
  signed int v27; // edi@55
  int v29; // edx@57
  int v31; // eax@64
  signed int v32; // edi@64
  int v34; // eax@66
  int v35; // dx@66
  __int16 v36; // dx@67
  //__int16 v37; // di@67
  __int16 v38; // dx@67
  int v46; // edx@87
  int v49; // esi@93
  int v53; // [sp+Ch] [bp-34h]@44
  int v54; // [sp+10h] [bp-30h]@0
  int min_y_ID2; // [sp+14h] [bp-2Ch]@12
  int v59; // [sp+14h] [bp-2Ch]@87
  int v61; // [sp+1Ch] [bp-24h]@29
  int v62; // [sp+20h] [bp-20h]@0
  signed int direction1; // [sp+24h] [bp-1Ch]@3
  signed int direction2; // [sp+28h] [bp-18h]@3
  int min_y_ID; // [sp+2Ch] [bp-14h]@5
  int v69; // [sp+34h] [bp-Ch]@29
  int v70; // [sp+34h] [bp-Ch]@46

  if ( pNumVertices <= 1 )
    return false;
  min_y = PortalFace._screen_space_y[0];
  min_y_ID = 0;
  max_y = PortalFace._screen_space_y[0];
  //face direction(направление фейса)
  if ( !PortalFace.direction )
  {
    direction1 = 1;
    direction2 = -1;
  }
  else 
  {
    direction1 = -1;
    direction2 = 1;
  }

  //get min and max y for portal(дать минимальное и максимальное значение y для портала)
  for ( uint i = 1; i < pNumVertices; ++i )
  {
    if (PortalFace._screen_space_y[i] < min_y)
    {
      min_y_ID = i;
      min_y = PortalFace._screen_space_y[i];
    }
    else if (PortalFace._screen_space_y[i] > max_y)
    {
      max_y = PortalFace._screen_space_y[i];
    }
  }
  if ( max_y == min_y )
    return false;

  //*****************************************************************************************************************************
  far_portal->_viewport_space_y = min_y;
  far_portal->_viewport_space_w = max_y;
  current_ID = min_y_ID;
  min_y_ID2 = min_y_ID;

  for ( uint i = 0; i < pNumVertices; ++i )
  {
    current_ID += direction2;
    if ( current_ID < pNumVertices )
    {
      if ( current_ID < 0 )
        current_ID += pNumVertices;
    }
    else
      current_ID -= pNumVertices;
    if ( PortalFace._screen_space_y[current_ID] <= PortalFace._screen_space_y[min_y_ID] )//определение минимальной у
    {
      min_y_ID2 = current_ID;
      min_y_ID = current_ID;
    }
    if ( PortalFace._screen_space_y[current_ID] == max_y )
      break;
  }

  v13 = min_y_ID2 + direction2;
  if ( v13 < pNumVertices )
  {
    if ( v13 < 0 )
      v13 += pNumVertices;
  }
  else
    v13 -= pNumVertices;
  if ( PortalFace._screen_space_y[v13] != PortalFace._screen_space_y[min_y_ID2] )
  {
    v62 = PortalFace._screen_space_x[min_y_ID2] << 16;
    v54 = ((PortalFace._screen_space_x[v13] - PortalFace._screen_space_x[min_y_ID2]) << 16) /
           (PortalFace._screen_space_y[v13] - PortalFace._screen_space_y[min_y_ID2]);
    far_portal->viewport_left_side[min_y] = (short)PortalFace._screen_space_x[min_y_ID2];
  }
  //****************************************************************************************************************************************
  //
  v15 = min_y_ID;
  v61 = min_y_ID;
  for ( v69 = 0; v69 < pNumVertices; ++v69 )
  {
    v15 += direction1;
    if ( v15 < pNumVertices )
    {
      if ( v15 < 0 )
        v15 += pNumVertices;
    }
    else
      v15 -= pNumVertices;
    if ( PortalFace._screen_space_y[v15] <= PortalFace._screen_space_y[min_y_ID] )
    {
      v61 = v15;
      min_y_ID = v15;
    }
    if ( PortalFace._screen_space_y[v15] == max_y )
      break;
  }
  v18 = direction1 + v61;
  if ( v18 < pNumVertices )
  {
    if ( v18 < 0 )
      v18 += pNumVertices;
  }
  else
    v18 -= pNumVertices;
  v19 = v18;
  v20 = v61;
  if ( PortalFace._screen_space_y[v19] != PortalFace._screen_space_y[v61] )
  {
    v61 = PortalFace._screen_space_x[v20] << 16;
    v53 = ((PortalFace._screen_space_x[v19] - PortalFace._screen_space_x[v20]) << 16) /
           (PortalFace._screen_space_y[v19] - PortalFace._screen_space_y[v20]);
    far_portal->viewport_right_side[max_y] = (short)PortalFace._screen_space_x[v20];
  }
  //****************************************************************************************************************************************
  v22 = min_y;
  if ( min_y <= max_y )
  {
    for ( v70 = min_y; v70 <= max_y; ++v70 )
    {
      v24 = v13;
      if ( v22 >= PortalFace._screen_space_y[v13] && v22 != max_y )
      {
        v13 = direction2 + v13;
        if ( v13 < pNumVertices )
        {
          if ( v13 < 0 )
            v13 += pNumVertices;
        }
        else
          v13 -= pNumVertices;
        v26 = v13;
        if ( PortalFace._screen_space_y[v26] - PortalFace._screen_space_y[v24] > 0 )
        {
          v54 = ((PortalFace._screen_space_x[v26] - PortalFace._screen_space_x[v24]) << 16) / (PortalFace._screen_space_y[v26] - PortalFace._screen_space_y[v24]);
          v62 = PortalFace._screen_space_x[v24] << 16;
        }
      }
      v29 = v18;
      if ( v70 >= PortalFace._screen_space_y[v18] && v70 != max_y )
      {
        v18 += direction1;
        if ( v18 < pNumVertices )
        {
          if ( v18 < 0 )
            v18 += pNumVertices;
        }
        else
          v18 -= pNumVertices;
        v31 = v18;
        if ( PortalFace._screen_space_y[v31] - PortalFace._screen_space_y[v29] > 0 )
        {
          v53 = ((PortalFace._screen_space_x[v31] - PortalFace._screen_space_x[v29]) << 16) / (PortalFace._screen_space_y[v31] - PortalFace._screen_space_y[v29]);
          v61 = PortalFace._screen_space_x[v29] << 16;
        }
      }
      far_portal->viewport_left_side[v70] = HIWORD(v62);
      far_portal->viewport_right_side[v70] = HIWORD(v61);
      if ( far_portal->viewport_left_side[v70] > far_portal->viewport_right_side[v70] )
      {
        v36 = far_portal->viewport_left_side[v70] ^ far_portal->viewport_right_side[v70];
        //v37 = far_portal->viewport_right_side[v70];
        far_portal->viewport_left_side[v70] = v36;
        v38 = far_portal->viewport_right_side[v70] ^ v36;
        far_portal->viewport_left_side[v70] ^= v38;
        far_portal->viewport_right_side[v70] = v38;
      }
      v62 += v54;
      v22 = v70 + 1;
      v61 += v53;
    }
  }
  //*****************************************************************************************************************************
  // check portals coordinates and determine max, min(проверка координат порталов и определение макс, мин-ой у)
  if ( max_y < near_portal->_viewport_space_y )
    return false;
  if ( min_y > near_portal->_viewport_space_w )
    return false;
  if ( min_y < near_portal->_viewport_space_y )
    min_y = near_portal->_viewport_space_y;
  if ( max_y > near_portal->_viewport_space_w )
    max_y = near_portal->_viewport_space_w;
  if ( min_y <= max_y )
  {
    for ( min_y; min_y <= max_y; ++min_y )
    {
      if ( far_portal->viewport_right_side[min_y] >= near_portal->viewport_left_side[min_y]
        && far_portal->viewport_left_side[min_y] <= near_portal->viewport_right_side[min_y] )
        break;
    }
  }
  if ( max_y < min_y )
    return false;
  for ( max_y; max_y >= min_y; --max_y )
  {
    if ( far_portal->viewport_right_side[max_y] >= near_portal->viewport_left_side[max_y] 
      && far_portal->viewport_left_side[max_y] <= near_portal->viewport_right_side[max_y] )
      break;
  }
  if ( min_y >= max_y )
    return false;
  //*************************************************************************************************************************************
  v59 = min_y;
  for ( v46 = max_y - min_y + 1; v46; --v46 )
  {
    if ( far_portal->viewport_left_side[v59] < near_portal->viewport_left_side[v59] )
      far_portal->viewport_left_side[v59] = near_portal->viewport_left_side[v59];
    if ( far_portal->viewport_right_side[v59] > near_portal->viewport_right_side[v59] )
      far_portal->viewport_right_side[v59] = near_portal->viewport_right_side[v59];
    ++v59;
  }
  far_portal->_viewport_space_y = min_y;
  far_portal->_viewport_space_w = max_y;
  far_portal->_viewport_space_x = far_portal->viewport_left_side[min_y];
  far_portal->_viewport_space_z = far_portal->viewport_right_side[min_y];
  far_portal->_viewport_x_minID = min_y;
  far_portal->_viewport_z_maxID = min_y;
  v49 = min_y + 1;
  if ( v49 <= max_y )
  {
    for ( v49; v49 <= max_y; ++v49 )
    {
      if ( far_portal->viewport_left_side[v49] < far_portal->_viewport_space_x )
      {
        far_portal->_viewport_space_x = far_portal->viewport_left_side[v49];
        far_portal->_viewport_x_minID = v49;
      }
      if ( far_portal->viewport_right_side[v49] > far_portal->_viewport_space_z )
      {
        far_portal->_viewport_space_z = far_portal->viewport_right_side[v49];
        far_portal->_viewport_z_maxID = v49;
      }
    }
  }
  return true;
}
//----- (00423B5D) --------------------------------------------------------
int __fastcall GetPortalScreenCoord(unsigned int uFaceID)
{
  BLVFace *pFace; // ebx@1
  int pNextVertices; // edx@11
  int t; // ST28_4@12
  int pScreenX; // eax@22
  int pScreenY; // eax@27
  signed int left_num_vertices; // edi@31
  signed int right_num_vertices; // ebx@41
  signed int top_num_vertices; // edi@51
  int bottom_num_vertices; // ebx@61
  signed int v62; // edx@75
  int v63; // ecx@76
  int v64; // esi@76
  int v65; // ecx@83
  signed int v71; // [sp+14h] [bp-14h]@75
  bool current_vertices_flag; // [sp+18h] [bp-10h]@9
  int thisf; // [sp+18h] [bp-10h]@74
  signed int depth_num_vertices; // [sp+1Ch] [bp-Ch]@9
  int v80; // [sp+1Ch] [bp-Ch]@76
  bool next_vertices_flag; // [sp+20h] [bp-8h]@10
  //Доп инфо "Программирование трёхмерных игр для windows" Ламот стр 910

  pFace = &pIndoor->pFaces[uFaceID];
  memset(&PortalFace, 0, sizeof(stru367));

  //get direction the face(определение направленности фейса)*********************************************************************************
  if ( pFace->pFacePlane_old.vNormal.x * (pIndoor->pVertices[pIndoor->pFaces[uFaceID].pVertexIDs[0]].x - pGame->pIndoorCameraD3D->vPartyPos.x)
     + pFace->pFacePlane_old.vNormal.y * (pIndoor->pVertices[pIndoor->pFaces[uFaceID].pVertexIDs[0]].y - pGame->pIndoorCameraD3D->vPartyPos.y)
     + pFace->pFacePlane_old.vNormal.z * (pIndoor->pVertices[pIndoor->pFaces[uFaceID].pVertexIDs[0]].z - pGame->pIndoorCameraD3D->vPartyPos.z) < 0 )
  {
    PortalFace.direction = true;
  }
  else
  {
    PortalFace.direction = false;
    if ( !(pFace->Portal()) )
      return 0;
  }
  //*****************************************************************************************************************************************
  //generate/cinvertetion in camera location coordinates(генерация/конвертирование в координаты пространства камеры)

  //for new coordinates:
  //int x = 0x AAAA BBBB;
  //AAAA - integer(целая часть), BBBB - fractional(дробная)
  //float v = HIWORD(x) + LOWORD(x) / 65535.0f;
  //0x0351A281 это 849(351 в шестнадцатиричной) в целой части и A281 в дробной(хотя как точно BBBB считалась не помню)
  //if in HIWORD: FFFF = -1
  //FFFE = -2
  //FFFD = -3
  //....
  //8000 = -32767
  //7FFF = 32767
  //7FFE = 32766
  //если в LOWORD например лежит FFFF то не совсем понятно, что это
  //потому что если и hiword и loword равны FFFF FFFF то двойное отрицание как бы, нужно тестировать что конкретно получается чтобы понять что это значит
  //всё что больше 7FFF для верхнего слова это идёт уже с минусом/Nomad/

  if ( (signed int)pFace->uNumVertices > 0 )
  {
    for (uint i = 0; i < pFace->uNumVertices; ++i)
    {
      pGame->pIndoorCameraD3D->ApplyViewTransform_TrueIfStillVisible_BLV(pIndoor->pVertices[pFace->pVertexIDs[i]].x,
                                                                     pIndoor->pVertices[pFace->pVertexIDs[i]].y,
                                                                     pIndoor->pVertices[pFace->pVertexIDs[i]].z,
        &PortalFace._view_transformed_z[i + 3], &PortalFace._view_transformed_x[i + 3], &PortalFace._view_transformed_y[i + 3], 0);
    }
  }
  //*****************************************************************************************************************************************
  //check vertices for the nearest plane(проверка вершин есть ли в области за ближайшей плоскостью)
  if ( pFace->uNumVertices <= 0 )
    return 0;
  bool bFound = false;
  for (uint i = 0; i < pFace->uNumVertices; ++i)
  {
    if ( PortalFace._view_transformed_z[i + 3] >= 524288 )// 8.0(0x80000)
    {
      bFound = true;
      break;
    }
  }
  if ( !bFound )
    return 0;
  //*****************************************************************************************************************************************
  //check for near clip plane(проверка по ближней границе)
  //   
  //     v0                 v1
  //      ._________________.
  //     /                   \
  //    /                     \
  // v5.                       . v2
  //   |                       |
  //   |                       |
  //   |                       |
  //  ---------------------------- 8.0(near_clip)
  //   |                       |
  //   ._______________________.
  //  v4                        v3
  depth_num_vertices = 0;
  PortalFace._view_transformed_z[pFace->uNumVertices + 3] = PortalFace._view_transformed_z[3];
  PortalFace._view_transformed_x[pFace->uNumVertices + 3] = PortalFace._view_transformed_x[3];
  PortalFace._view_transformed_y[pFace->uNumVertices + 3] = PortalFace._view_transformed_y[3];
  current_vertices_flag = PortalFace._view_transformed_z[3] >= 524288;// 8.0(0x80000)
  if ( pFace->uNumVertices >= 1 )
  {
    for ( uint i = 1; i <= pFace->uNumVertices; ++i)
    {
      next_vertices_flag = PortalFace._view_transformed_z[i + 3] >= 524288;// 8.0(0x80000)
      if ( current_vertices_flag ^ next_vertices_flag )//или текущая или следующая вершина за ближней границей
      {
        if ( next_vertices_flag )//следующая вершина за ближней границей
        {
          //t = near_clip - v0.z / v1.z - v0.z
          t = fixpoint_div(524288 - PortalFace._view_transformed_z[i + 2], PortalFace._view_transformed_z[i + 3] - PortalFace._view_transformed_z[i + 2]);
          //New_x = (v1.x - v0.x)*t + v0.x
          PortalFace._view_transformed_x[depth_num_vertices] = ((unsigned __int64)((PortalFace._view_transformed_x[i + 3]
                                                           - PortalFace._view_transformed_x[i + 2]) * t) >> 16) + PortalFace._view_transformed_x[i + 2];
          //New_y = (v1.y - v0.y)*t + v0.y
          PortalFace._view_transformed_y[depth_num_vertices] = ((unsigned __int64)((PortalFace._view_transformed_y[i + 3] - PortalFace._view_transformed_y[i + 2])
                                   * t) >> 16) + PortalFace._view_transformed_y[i + 2];
          //New_z = 8.0(0x80000)
          PortalFace._view_transformed_z[depth_num_vertices] = 524288;
        }
        else// текущая вершина за ближней границей
        {
          //t = near_clip - v1.z / v0.z - v1.z
          t = fixpoint_div(524288 - PortalFace._view_transformed_z[i + 3], PortalFace._view_transformed_z[i + 2] - PortalFace._view_transformed_z[i + 3]);
          //New_x = (v0.x - v1.x)*t + v1.x
          PortalFace._view_transformed_x[depth_num_vertices] = ((unsigned __int64)((PortalFace._view_transformed_x[i + 2]
                                                   - PortalFace._view_transformed_x[i + 3]) * t) >> 16) + PortalFace._view_transformed_x[i + 3];
          //New_y = (v0.x - v1.y)*t + v1.y
          PortalFace._view_transformed_y[depth_num_vertices] = ((unsigned __int64)((PortalFace._view_transformed_y[i + 2] - PortalFace._view_transformed_y[i + 3])
                                   * t) >> 16) + PortalFace._view_transformed_y[i + 3];
          //New_z = 8.0(0x80000)
          PortalFace._view_transformed_z[depth_num_vertices] = 524288;
        }
        depth_num_vertices++;
      }
      if ( next_vertices_flag )//если следующая вершина за ближней границей 
      {
        pNextVertices = depth_num_vertices++;
        PortalFace._view_transformed_z[pNextVertices] = PortalFace._view_transformed_z[i + 3];
        PortalFace._view_transformed_x[pNextVertices] = PortalFace._view_transformed_x[i + 3];
        PortalFace._view_transformed_y[pNextVertices] = PortalFace._view_transformed_y[i + 3];
      }
      current_vertices_flag = next_vertices_flag;
    }
  }
  //результат: нет моргания на границе портала(когда проходим сквозь портал)
  //************************************************************************************************************************************
  //convertion in screen coordinates(конвертирование в координаты экрана)
  PortalFace._view_transformed_z[depth_num_vertices] = PortalFace._view_transformed_z[0];
  PortalFace._view_transformed_x[depth_num_vertices] = PortalFace._view_transformed_x[0];
  PortalFace._view_transformed_y[depth_num_vertices] = PortalFace._view_transformed_y[0];
  for ( uint i = 0; i < depth_num_vertices; ++i )
  {
    if ( (abs(PortalFace._view_transformed_x[i]) >> 13) <= abs(PortalFace._view_transformed_z[i]) )
      pScreenX = fixpoint_div(PortalFace._view_transformed_x[i],  PortalFace._view_transformed_z[i]);
    else
    {
      if ( PortalFace._view_transformed_x[i] >= 0 )
      {
        if (PortalFace._view_transformed_z[i] >= 0)
          pScreenX = 0x400000;   // 64.0
        else
          pScreenX = 0xFFC00000;  // -63.0
      }
      else
      {
        if (PortalFace._view_transformed_z[i] >= 0)
          pScreenX = 0xFFC00000;  // -63.0
        else
          pScreenX = 0x400000; // 64.0
      }
    }

    if ( (abs(PortalFace._view_transformed_y[i]) >> 13) <= abs(PortalFace._view_transformed_z[i]) )
      pScreenY = fixpoint_div(PortalFace._view_transformed_y[i],  PortalFace._view_transformed_z[i]);
    else
    {
      if ( PortalFace._view_transformed_y[i] >= 0 )
      {
        if (PortalFace._view_transformed_z[i] >= 0)
          pScreenY = 0x400000;   // 64.0
        else
          pScreenY = 0xFFC00000;  // -63.0
      }
      else
      {
        if (PortalFace._view_transformed_z[i] >= 0)
          pScreenY = 0xFFC00000;  // -63.0
        else
          pScreenY = 0x400000;  // 64.0
      }
    }
    PortalFace._screen_space_x[i + 12] = pBLVRenderParams->uViewportCenterX - ((unsigned __int64)(SHIWORD(pBLVRenderParams->fov_rad_fixpoint) * (signed __int64)pScreenX) >> 16);
    PortalFace._screen_space_y[i + 12] = pBLVRenderParams->uViewportCenterY - ((unsigned __int64)(SHIWORD(pBLVRenderParams->fov_rad_fixpoint) * (signed __int64)pScreenY) >> 16);
  }
  // результат: при повороте камеры, когда граница портала сдвигается к краю экрана, портал остается прозрачным(видимым)
  //******************************************************************************************************************************************
  //координаты как в Ида-базе игры так и в данном проекте перевёрнутые,т.е. портал который в правой части экрана имеет экранные координаты 
  //которые для левой части экрана. Например, x(оригинал) = 8, у нас х = 468(противоположный край экрана), точно также и с у.
  //
  //check for left_clip plane(порверка по левой границе)
  left_num_vertices = 0;
  PortalFace._screen_space_x[depth_num_vertices + 12] = PortalFace._screen_space_x[12];
  PortalFace._screen_space_y[depth_num_vertices + 12] = PortalFace._screen_space_y[12];
  current_vertices_flag = PortalFace._screen_space_x[12] >= (signed int)pBLVRenderParams->uViewportX;//8.0
  if ( depth_num_vertices < 1 )
    return 0;
  for ( uint i = 1; i <= depth_num_vertices; ++i )
  {
    next_vertices_flag = PortalFace._screen_space_x[i + 12] >= (signed int)pBLVRenderParams->uViewportX;
    if ( current_vertices_flag ^ next_vertices_flag )
    {
      if ( next_vertices_flag )
      {
        //t = left_clip - v0.x / v1.x - v0.x
        t = fixpoint_div(pBLVRenderParams->uViewportX - PortalFace._screen_space_x[i + 11], PortalFace._screen_space_x[i + 12] - PortalFace._screen_space_x[i + 11]);
        //New_y = (v1.y - v0.y)*t + v0.y
        PortalFace._screen_space_y[left_num_vertices + 9] = ((signed int)((PortalFace._screen_space_y[i + 12]- PortalFace._screen_space_y[i + 11])
                                                            * t) >> 16) + PortalFace._screen_space_y[i + 11];
        //New_x = left_clip
        PortalFace._screen_space_x[left_num_vertices + 9] = pBLVRenderParams->uViewportX;
      }
      else
      {
        //t = left_clip - v1.x / v0.x - v1.x
        t = fixpoint_div(pBLVRenderParams->uViewportX - PortalFace._screen_space_x[i + 12], PortalFace._screen_space_x[i + 11] - PortalFace._screen_space_x[i + 12]);
        //New_y = (v0.y - v1.y)*t + v1.y
        PortalFace._screen_space_y[left_num_vertices + 9] = ((signed int)(( PortalFace._screen_space_y[i + 11] - PortalFace._screen_space_y[i + 12])
                                                            * t) >> 16) + PortalFace._screen_space_y[i + 12];
        //New_x = left_clip
        PortalFace._screen_space_x[left_num_vertices + 9] = pBLVRenderParams->uViewportX;
      }
      left_num_vertices++;
    }
    if ( next_vertices_flag )
    {
      pNextVertices = left_num_vertices++;
      PortalFace._screen_space_x[pNextVertices + 9] = PortalFace._screen_space_x[i + 12];
      PortalFace._screen_space_y[pNextVertices + 9] = PortalFace._screen_space_y[i + 12];
    }
    current_vertices_flag = next_vertices_flag;
  }
//*********************************************************************************************************************************
//for right_clip plane(проверка по правой плоскости)
  right_num_vertices = 0;
  PortalFace._screen_space_x[left_num_vertices + 9] = PortalFace._screen_space_x[9];
  PortalFace._screen_space_y[left_num_vertices + 9] = PortalFace._screen_space_y[9];
  current_vertices_flag = PortalFace._screen_space_x[9] <= (signed int)pBLVRenderParams->uViewportZ;//468.0
  if (left_num_vertices < 1)
    return 0;
  for ( uint i = 1; i <= left_num_vertices; ++i )
  {
    next_vertices_flag = PortalFace._screen_space_x[i + 9] <= (signed int)pBLVRenderParams->uViewportZ;
    if ( current_vertices_flag ^ next_vertices_flag )
    {
      if ( next_vertices_flag )
      {
        //t = right_clip - v1.x / v0.x - v1.x
        t = fixpoint_div(pBLVRenderParams->uViewportZ - PortalFace._screen_space_x[i + 8], PortalFace._screen_space_x[i + 9] - PortalFace._screen_space_x[i + 8]);
        //New_y = (v0.y - v1.y)*t + v1.y
        PortalFace._screen_space_y[right_num_vertices + 6] = ((signed int)((PortalFace._screen_space_y[i + 9] - PortalFace._screen_space_y[i + 8])
                                                           * t) >> 16) + PortalFace._screen_space_y[i + 8];
        //New_x = right_clip
        PortalFace._screen_space_x[right_num_vertices + 6] = pBLVRenderParams->uViewportZ;
      }
      else
      {
        //t = right_clip - v0.x / v1.x - v0.x
        t = fixpoint_div(pBLVRenderParams->uViewportZ - PortalFace._screen_space_x[i + 9], PortalFace._screen_space_x[i + 8] - PortalFace._screen_space_x[i + 9]);
        //New_y = (v1.y - v0.y)*t + v0.y
        PortalFace._screen_space_y[right_num_vertices + 6] = ((signed int)((PortalFace._screen_space_y[i + 8] - PortalFace._screen_space_y[i + 9])
                                                           * t) >> 16) + PortalFace._screen_space_y[i + 9];
        //New_x = right_clip
        PortalFace._screen_space_x[right_num_vertices + 6] = pBLVRenderParams->uViewportZ;
      }
      right_num_vertices++;
    }
    if ( next_vertices_flag )
    {
      pNextVertices = right_num_vertices++;
      PortalFace._screen_space_x[pNextVertices + 6] = PortalFace._screen_space_x[i + 9];
      PortalFace._screen_space_y[pNextVertices + 6] = PortalFace._screen_space_y[i + 9];
    }
    current_vertices_flag = next_vertices_flag;
  }
  //************************************************************************************************************************************
  // for top clip plane
  top_num_vertices = 0;
  PortalFace._screen_space_x[right_num_vertices + 6] = PortalFace._screen_space_x[6];
  PortalFace._screen_space_y[right_num_vertices + 6] = PortalFace._screen_space_y[6];

  current_vertices_flag = PortalFace._screen_space_y[6] >= (signed int)pBLVRenderParams->uViewportY;//8.0
  if ( right_num_vertices < 1 )
    return 0;
  for ( uint i = 1; i <= right_num_vertices; ++i )
  {
    next_vertices_flag = PortalFace._screen_space_y[i + 6] >= (signed int)pBLVRenderParams->uViewportY;
    if ( current_vertices_flag ^ next_vertices_flag )
    {
      if ( next_vertices_flag )
      {
        t = fixpoint_div(pBLVRenderParams->uViewportY  - PortalFace._screen_space_y[i + 5], PortalFace._screen_space_y[i + 6] - PortalFace._screen_space_y[i + 5]);
        PortalFace._screen_space_x[top_num_vertices + 3] = ((signed int)((PortalFace._screen_space_x[i + 6] - PortalFace._screen_space_x[i + 5])
            * t) >> 16) + PortalFace._screen_space_x[i + 5];
        PortalFace._screen_space_y[top_num_vertices + 3] = pBLVRenderParams->uViewportY;
      }
      else
      {
        t = fixpoint_div(pBLVRenderParams->uViewportY - PortalFace._screen_space_y[i + 6], PortalFace._screen_space_y[i + 5] - PortalFace._screen_space_y[i + 6]);
        PortalFace._screen_space_x[top_num_vertices + 3] = ((signed int)((PortalFace._screen_space_x[i + 5]- PortalFace._screen_space_x[i + 6])
            * t) >> 16) + PortalFace._screen_space_x[i + 6];
        PortalFace._screen_space_y[top_num_vertices + 3] = pBLVRenderParams->uViewportY;
      }
      top_num_vertices++;
    }
    current_vertices_flag = next_vertices_flag;
    if ( next_vertices_flag )
    {
      pNextVertices = top_num_vertices++;
      PortalFace._screen_space_x[pNextVertices + 3] = PortalFace._screen_space_x[i + 6];
      PortalFace._screen_space_y[pNextVertices + 3] = PortalFace._screen_space_y[i + 6];
    }
  }
//**********************************************************************************************************************************
//for bottom_clip plane(проверка по нижней плоскости)
  bottom_num_vertices = 0;
  PortalFace._screen_space_x[top_num_vertices + 3] = PortalFace._screen_space_x[3];
  PortalFace._screen_space_y[top_num_vertices + 3] = PortalFace._screen_space_y[3];
  current_vertices_flag = PortalFace._screen_space_y[3] <= (signed int)pBLVRenderParams->uViewportW;//351.0
  if ( top_num_vertices < 1 )
    return 0;
  for ( uint i =1; i <= top_num_vertices; ++i )
  {
    next_vertices_flag = PortalFace._screen_space_y[i + 3] <= (signed int)pBLVRenderParams->uViewportW;
    if ( current_vertices_flag ^ next_vertices_flag )
    {
      if ( next_vertices_flag )
      {
        t = fixpoint_div(pBLVRenderParams->uViewportW - PortalFace._screen_space_y[i + 2], PortalFace._screen_space_y[i + 3] - PortalFace._screen_space_y[i + 2]);
        PortalFace._screen_space_x[bottom_num_vertices] = ((signed int)((PortalFace._screen_space_x[i + 3] - PortalFace._screen_space_x[i + 2])
            * t) >> 16) + PortalFace._screen_space_x[i + 2];
        PortalFace._screen_space_y[bottom_num_vertices] = pBLVRenderParams->uViewportW;
      }
      else
      {
        t = fixpoint_div(pBLVRenderParams->uViewportW - PortalFace._screen_space_y[i + 3], PortalFace._screen_space_y[i + 2] - PortalFace._screen_space_y[i + 3]);
        PortalFace._screen_space_x[bottom_num_vertices] = ((signed int)((PortalFace._screen_space_x[i + 2] - PortalFace._screen_space_x[i + 3])
            * t) >> 16) + PortalFace._screen_space_x[i + 3];
        PortalFace._screen_space_y[bottom_num_vertices] = pBLVRenderParams->uViewportW;
      }
      bottom_num_vertices++;
    }
    if ( next_vertices_flag )
    {
      pNextVertices = bottom_num_vertices++;
      PortalFace._screen_space_x[pNextVertices] = PortalFace._screen_space_x[i + 3];
      PortalFace._screen_space_y[pNextVertices] = PortalFace._screen_space_y[i + 3];
    }
    current_vertices_flag = next_vertices_flag;
  }
//***************************************************************************************************************************************

  if ( !bottom_num_vertices )
    return 0;
  PortalFace._screen_space_x[bottom_num_vertices] = PortalFace._screen_space_x[0];
  PortalFace._screen_space_y[bottom_num_vertices] = PortalFace._screen_space_y[0];
//check for software(проверка для софтвар)
  if ( !pRenderer->pRenderD3D && bottom_num_vertices > 3 )
  {
    PortalFace._screen_space_x[bottom_num_vertices + 1] = PortalFace._screen_space_x[1];
    PortalFace._screen_space_y[bottom_num_vertices + 1] = PortalFace._screen_space_y[1];
    thisf = PortalFace.direction == true ? 1 : - 1;
    if ( bottom_num_vertices > 0 )
    {
      v62 = 1;
      v71 = 1;
      do
      {
        v63 = v62 - 1;
        v64 = v62 + 1;
        v80 = v62 + 1;
        if ( v62 - 1 >= bottom_num_vertices )
          v63 -= bottom_num_vertices;
        if ( v62 >= bottom_num_vertices )
          v62 -= bottom_num_vertices;
        if ( v64 >= bottom_num_vertices )
          v64 -= bottom_num_vertices;
        if ( thisf * ((PortalFace._screen_space_y[v64] - PortalFace._screen_space_y[v63])
                   * (PortalFace._screen_space_x[v62] - PortalFace._screen_space_x[v63])
                   - (PortalFace._screen_space_y[v62] - PortalFace._screen_space_y[v63])
                   * (PortalFace._screen_space_x[v64] - PortalFace._screen_space_x[v63])) < 0 )
        {
          v62 = v80;
          v71 = v80;
        }
        else
        {
          v62 = v71;
          v65 = v71;
          if ( v71 < bottom_num_vertices || (v65 = v71 - bottom_num_vertices, v71 - bottom_num_vertices < bottom_num_vertices) )
          {
            memcpy(&PortalFace._screen_space_y[v65], &PortalFace._screen_space_y[v65 + 1],
              4 * ((unsigned int)(4 * (bottom_num_vertices - v65)) >> 2));
            memcpy(&PortalFace._screen_space_x[v65], &PortalFace._screen_space_x[v65 + 1],
              4 * ((unsigned int)(4 * (bottom_num_vertices - v65)) >> 2));
          }
          --bottom_num_vertices;
        }
      }
      while ( v62 - 1 < bottom_num_vertices );
    }
    PortalFace._screen_space_x[bottom_num_vertices] = PortalFace._screen_space_x[0];
    PortalFace._screen_space_y[bottom_num_vertices] = PortalFace._screen_space_y[0];
  }
  return bottom_num_vertices;
}

//----- (004AAEA6) --------------------------------------------------------
int __fastcall sub_4AAEA6_transform(RenderVertexSoft *a1)
{
  double v4; // st5@2
  double v5; // st4@3
  float v11; // [sp+8h] [bp-8h]@2
  float v12; // [sp+8h] [bp-8h]@6
  float v13; // [sp+Ch] [bp-4h]@2
  float v14; // [sp+Ch] [bp-4h]@6

  if (pGame->pIndoorCameraD3D->sRotationX)
  {
    v13 = a1->vWorldPosition.x - (double)pParty->vPosition.x;
    v11 = a1->vWorldPosition.y - (double)pParty->vPosition.y;
    v4 = a1->vWorldPosition.z - (double)pParty->vPosition.z;
    //if ( pRenderer->pRenderD3D )
    //{
      v5 = v11 * pGame->pIndoorCameraD3D->fRotationYSine + v13 * pGame->pIndoorCameraD3D->fRotationYCosine;
      a1->vWorldViewPosition.y = v13 * pGame->pIndoorCameraD3D->fRotationYSine - v11 * pGame->pIndoorCameraD3D->fRotationYCosine;
    /*}
    else
    {
      v5 = v13 * pBLVRenderParams->fCosineY - v11 * pBLVRenderParams->fSineY;
      a1->vWorldViewPosition.y = v13 * pBLVRenderParams->fSineY + v11 * pBLVRenderParams->fCosineY;
    }*/
    a1->vWorldViewPosition.x = v5 * pGame->pIndoorCameraD3D->fRotationXCosine - v4 * pGame->pIndoorCameraD3D->fRotationXSine;
    a1->vWorldViewPosition.z = v5 * pGame->pIndoorCameraD3D->fRotationXSine + v4 * pGame->pIndoorCameraD3D->fRotationXCosine;
  }
  else
  {
    v14 = a1->vWorldPosition.x - (double)pParty->vPosition.x;
    v12 = a1->vWorldPosition.y - (double)pParty->vPosition.y;
    a1->vWorldViewPosition.z = a1->vWorldPosition.z - (double)pParty->vPosition.z;
    //if ( pRenderer->pRenderD3D )
    //{
      a1->vWorldViewPosition.x = v12 * pGame->pIndoorCameraD3D->fRotationYSine + v14 * pGame->pIndoorCameraD3D->fRotationYCosine;
      a1->vWorldViewPosition.y = v14 * pGame->pIndoorCameraD3D->fRotationYSine - v12 * pGame->pIndoorCameraD3D->fRotationYCosine;
    /*}
    else
    {
      a1->vWorldViewPosition.x = v14 * pBLVRenderParams->fCosineY - v12 * pBLVRenderParams->fSineY;
      a1->vWorldViewPosition.y = v14 * pBLVRenderParams->fSineY + v12 * pBLVRenderParams->fCosineY;
    }*/
  }
  return 0;
}