#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "stru6.h"
#include "Weather.h"
#include "Sprites.h"
#include "LightmapBuilder.h"
#include "Outdoor.h"
#include "Outdoor_stuff.h"
#include "Party.h"
#include "SpriteObject.h"
#include "LOD.h"
#include "PaletteManager.h"
#include "GUIProgressBar.h"
#include "AudioPlayer.h"
#include "DecorationList.h"
#include "Math.h"
#include "ObjectList.h"
#include "Game.h"
#include "Actor.h"
#include "Chest.h"
#include "stru123.h"
#include "Time.h"
#include "Viewport.h"
#include "Events.h"
#include "ParticleEngine.h"

#include "mm7_data.h"
#include "MM7.h"
#include "Lights.h"


#include "MapInfo.h"
#include "Outdoor_stuff.h"
#include "BSPModel.h"
#include "GUIWindow.h"
#include "Level/Decoration.h"

MapStartPoint uLevel_StartingPointType; // weak

OutdoorLocation *pOutdoor = new OutdoorLocation;
ODMRenderParams *pODMRenderParams;


stru149 stru_8019C8;
std::array<struct Polygon, 2000 + 18000> array_77EC08;


struct FogProbabilityTableEntry
{
  unsigned char small_fog_chance;
  unsigned char average_fog_chance;
  unsigned char dense_fog_chance;
  unsigned char __unused;
} fog_probability_table[15] =
{
  { 20,  10,   5,   0}, // MAP_EMERALD_ISLE
  { 20,  10,   5,   0}, // MAP_HARMONDALE
  { 20,  10,   5,   0}, // MAP_STEADWICK
  { 20,  10,   5,   0}, // MAP_PIERPONT
  { 20,  10,   5,   0}, // MAP_DEYJA
  { 10,   5,   0,   0}, // MAP_BRAKADA_DESERT
  {  0,   0,   0,   0}, // MAP_CELESTIA
  {  0,   0,   0,   0}, // MAP_THE_PIT
  { 20,  30,  50,   0}, // MAP_EVENMORN_ISLE
  { 30,  20,  10,   0}, // MAP_MOUNT_NIGHON
  { 10,   5,   0,   0}, // MAP_BARROW_DOWNS
  { 20,  10,   5,   0}, // MAP_LAND_OF_GIANTS
  { 20,  10,   5,   0}, // MAP_TATALIA
  { 20,  10,   5,   0}, // MAP_AVLEE
  {  0, 100,   0,   0}  // MAP_SHOALS
};

//for future sky textures?
std::array<int, 9> dword_4EC268={{3,3,3,3,3,3,3,3,3}}; // weak
std::array<int, 7> dword_4EC28C={{3,3,3,3,3,3,3}}; // weak
int dword_4EC2A8=9; // weak
int dword_4EC2AC=7; // weak



//----- (0047A59E) --------------------------------------------------------
void OutdoorLocation::ExecDraw(unsigned int bRedraw)
{
  //unsigned int v2; // ebx@1
  int v3; // ST18_4@3
  int v4; // ST04_4@19
  int v5; // eax@19

  pGame->pIndoorCameraD3D->debug_flags = 0;
  if (viewparams->draw_d3d_outlines)
    pGame->pIndoorCameraD3D->debug_flags |= ODM_RENDER_DRAW_D3D_OUTLINES;

  if (bRedraw || pRenderer->pRenderD3D)
  {
    //pODMRenderParams->RotationToInts();
    sub_481ED9_MessWithODMRenderParams();
  }

  pODMRenderParams->uMapGridCellX = WorldPosToGridCellX(pParty->vPosition.x);
  pODMRenderParams->uMapGridCellZ = WorldPosToGridCellZ(pParty->vPosition.y);
  assert(pODMRenderParams->uMapGridCellX <= 127 && pODMRenderParams->uMapGridCellZ <= 127);

  if (bRedraw)
  {
    sub_487DA9();
  }
  if ( pParty->uCurrentMinute != pOutdoor->uLastSunlightUpdateMinute )
    pOutdoor->UpdateSunlightVectors();
  pOutdoor->UpdateFog();
  //pGame->pIndoorCameraD3D->sr_Reset_list_0037C();

  if (pRenderer->pRenderD3D) // d3d - redraw always
  {
    pRenderer->DrawSkyD3D();
    pRenderer->DrawBuildingsD3D();
    pRenderer->RenderTerrainD3D();
  }
  else
  {
    __debugbreak(); // no SW Rendering
    /*if (!bRedraw)
      pRenderer->OnOutdoorRedrawSW();
    else
    {
      pRenderer->DrawBuildingsSW();  
      pRenderer->DrawBezierTerrain();
      sr_sub_486F92_MessWithEdgesAndSpans();
      pODMRenderParams->ApplyLightmapsSW();
    }*/
  }


  pMobileLightsStack->uNumLightsActive = 0;
  pStationaryLightsStack->uNumLightsActive = 0;
  /*if ( !pRenderer->pRenderD3D )
  {
    pRenderer->ExecOutdoorDrawSW();
    pGame->pIndoorCameraD3D->sr_438240_draw_lits();
  }*/
  pGame->PushStationaryLights(-1);
  pGame->PrepareBloodsplats();
  if (bRedraw)
  {
    v4 = WorldPosToGridCellZ(pParty->vPosition.y);
    v5 = WorldPosToGridCellX(pParty->vPosition.x);
    pOutdoor->UpdateDiscoveredArea(v5, v4, 1);
  }
  pGame->uFlags2 &= 0xFFFFFFFEu;
  if (pRenderer->pRenderD3D && pRenderer->bUsingSpecular)
    pGame->pLightmapBuilder->uFlags |= 1u;
  else
    pGame->pLightmapBuilder->uFlags &= 0xFFFFFFFEu;

  uNumDecorationsDrawnThisFrame = 0;
  uNumSpritesDrawnThisFrame = 0;
  uNumBillboardsToDraw = 0;

  PrepareActorsDrawList();
  if (!pODMRenderParams->bDoNotRenderDecorations)
    pRenderer->PrepareDecorationsRenderList_ODM();

  pRenderer->DrawSpriteObjects_ODM();
  pRenderer->TransformBillboardsAndSetPalettesODM();
}


//----- (00441CFF) --------------------------------------------------------
void OutdoorLocation::Draw()
{
  bool redrawWorld = true;
  if ( !(pParty->uFlags & 2) && !(pGame->uFlags2 & 1) )
    redrawWorld = false;
  pOutdoor->ExecDraw(redrawWorld);

  pGame->DrawParticles();
  pWeather->Draw();
  trail_particle_generator.UpdateParticles();
}

//----- (00488E23) --------------------------------------------------------
double OutdoorLocation::GetFogDensityByTime()
{
  int v1; // eax@3
  double v2; // st7@3

  if ( pParty->uCurrentHour < 5 )
    goto LABEL_9;
  if ( pParty->uCurrentHour >= 6 )
  {
    if ( pParty->uCurrentHour < 0x14 )
    {
      pWeather->bNight = 0;
      v2 = 0.0;
      return v2 * 0.016666668;
    }
    if ( pParty->uCurrentHour < 0x15 )
    {
      v1 = 0;
      v2 = (double)(pParty->uCurrentHour - 20) * 60.0 + (double)(signed int)pParty->uCurrentMinute;
      goto LABEL_4;
    }
LABEL_9:
    v2 = 60.0;
    pWeather->bNight = 1;
    return v2 * 0.016666668;
  }
  v1 = 0;
  v2 = 60.0 - (double)(60 * pParty->uCurrentHour + pParty->uCurrentMinute - 300);
LABEL_4:
  pWeather->bNight = v1;
  return v2 * 0.016666668;
}

//----- (00488EB1) --------------------------------------------------------
int OutdoorLocation::GetSomeOtherTileInfo(int sX, int sY)
{
  //OutdoorLocation *v3; // esi@1
  unsigned int v4; // edi@1
  unsigned int v5; // eax@1
  int result; // eax@5

/*  v3 = this;
  v4 = WorldPosToGridCellZ(sY);
  v5 = WorldPosToGridCellX(sX);
  if ( (v5 & 0x80000000u) != 0 || (signed int)v5 > 127 || (v4 & 0x80000000u) != 0 || (signed int)v4 > 127 )
    result = 0;
  else
    result = ActuallyGetSomeOtherTileInfo(v5, v4);
  return result;*/
 v4 = WorldPosToGridCellZ(sY);
 v5 = WorldPosToGridCellX(sX);
 if ( v5 < 0 || v5 > 127 || v4 < 0 || v4 > 127 )
   return 0;
 return ActuallyGetSomeOtherTileInfo(v5, v4);
}
// 47F44B: using guessed type int __stdcall WorldPosToGridCellX(int);
// 47F458: using guessed type int __stdcall WorldPosToGridCellZ(int);

//----- (00488EEF) --------------------------------------------------------
unsigned int OutdoorLocation::GetTileTexture(int sX, int sY)
{
  //OutdoorLocation *v3; // esi@1
  unsigned int v4; // edi@1
  unsigned int v5; // eax@1
//  unsigned int result; // eax@5

  /*v3 = this;
  v4 = WorldPosToGridCellZ(sZ);
  v5 = WorldPosToGridCellX(sX);
  if ( v5< 0 || (signed int)v5 > 127 || v4 < 0 || (signed int)v4 > 127 )//if ( (v5 & 0x80000000u) != 0 || (signed int)v5 > 127 || (v4 & 0x80000000u) != 0 || (signed int)v4 > 127 )
    result = -1;
  else
    result = DoGetTileTexture(v5, v4);
  return result;*/
  v4 = WorldPosToGridCellZ(sY);
  v5 = WorldPosToGridCellX(sX);
  if ( v5 < 0 || v5 > 127 || v4 < 0 || v4 > 127 )
    return -1;
  return DoGetTileTexture(v5, v4);
}
// 47F44B: using guessed type int __stdcall WorldPosToGridCellX(int);
// 47F458: using guessed type int __stdcall WorldPosToGridCellZ(int);

//----- (00488F2E) --------------------------------------------------------
int OutdoorLocation::GetHeightOnTerrain(int sX, int sZ)
   /* Функция предоставляет возможность перемещать камеру таким образом, чтобы она имитировала ходьбу по ландшафту. 
   То есть нам надо менять высоту камеры (координату Y) в зависимости от того, в каком месте ландшафта мы находимся. 
   Для этого мы сначала должны определить по координатам X и Z камеры квадрат ландшафта в котором мы находимся. 
   Все это делает функция Terrain::getHeight; в своих параметрах она получает координаты X и Z камеры и возвращает высоту, 
   на которой должна быть расположена камера, чтобы она оказалась над ландшафтом.*/
{
  int result; // eax@5

  if ( sX < 0 || sX > 127 || sZ < 0 || sZ > 127 )
    result = 0;
  else
    result = DoGetHeightOnTerrain(sX, sZ);
  return result;
}

//----- (00488F5C) --------------------------------------------------------
bool OutdoorLocation::Initialize(const char *pFilename, int File, size_t uRespawnInterval, int thisa)
{
  OutdoorLocation *v5; // esi@1
  bool result; // eax@2

  v5 = this;
  if ( pFilename )
  {
    Release();
    pBitmaps_LOD->ReleaseAll2();
    pSprites_LOD->DeleteSomeOtherSprites();
    pSpriteFrameTable->ResetSomeSpriteFlags();
    pIcons_LOD->ReleaseAll2();
    sub_46080D();
    TryLoadLevelFromLOD();
    if ( !Load(pFilename, (ODMFace *)File, uRespawnInterval, thisa) )
    {
      MessageBoxA(0, "Error!", "Couldn't Load Map!", 0);
      CreateDebugLocation();
    }
    ::day_attrib = v5->loc_time.day_attrib;
    ::day_fogrange_1 = v5->loc_time.day_fogrange_1;
    ::day_fogrange_2 = v5->loc_time.day_fogrange_2;
    if ( Is_out15odm_underwater() )
      SetUnderwaterFog();
    _6BE134_odm_main_tile_group = v5->pTileTypes[0].tileset;
    result = 1;
  }
  else
  {
    result = 0;
  }
  return result;
}



char foot_travel_destinations[15][4] =
{
// north           south               east              west                    from
  {MAP_INVALID,    MAP_INVALID,        MAP_INVALID,      MAP_INVALID},        // MAP_EMERALD_ISLE
  {MAP_PIERPONT,   MAP_BARROW_DOWNS,   MAP_PIERPONT,     MAP_STEADWICK},      // MAP_HARMONDALE
  {MAP_DEYJA,      MAP_BRAKADA_DESERT, MAP_HARMONDALE,   MAP_TATALIA},        // MAP_STEADWICK
  {MAP_AVLEE,      MAP_HARMONDALE,     MAP_INVALID,      MAP_DEYJA},          // MAP_PIERPONT
  {MAP_PIERPONT,   MAP_STEADWICK,      MAP_PIERPONT,     MAP_STEADWICK},      // MAP_DEYJA
  {MAP_STEADWICK,  MAP_INVALID,        MAP_BARROW_DOWNS, MAP_INVALID},        // MAP_BRAKADA_DESERT
  {MAP_INVALID,    MAP_INVALID,        MAP_INVALID,      MAP_INVALID},        // MAP_CELESTIA
  {MAP_INVALID,    MAP_INVALID,        MAP_INVALID,      MAP_INVALID},        // MAP_THE_PIT
  {MAP_INVALID,    MAP_INVALID,        MAP_INVALID,      MAP_INVALID},        // MAP_EVENMORN_ISLE
  {MAP_INVALID,    MAP_INVALID,        MAP_INVALID,      MAP_INVALID},        // MAP_MOUNT_NIGHON
  {MAP_HARMONDALE, MAP_BRAKADA_DESERT, MAP_HARMONDALE,   MAP_BRAKADA_DESERT}, // MAP_BARROW_DOWNS
  {MAP_INVALID,    MAP_INVALID,        MAP_INVALID,      MAP_INVALID},        // MAP_LAND_OF_GIANTS
  {MAP_INVALID,    MAP_INVALID,        MAP_STEADWICK,    MAP_INVALID},        // MAP_TATALIA
  {MAP_INVALID,    MAP_PIERPONT,       MAP_PIERPONT,     MAP_INVALID},        // MAP_AVLEE
  {MAP_INVALID,    MAP_INVALID,        MAP_INVALID,      MAP_INVALID}         // MAP_SHOALS
};
unsigned char foot_travel_times[15][4] =
{
// north south east  west    from
  {0,    0,    0,    0},  // MAP_EMERALD_ISLE
  {5,    5,    7,    5},  // MAP_HARMONDALE
  {5,    5,    5,    5},  // MAP_STEADWICK
  {5,    5,    0,    5},  // MAP_PIERPONT
  {7,    5,    5,    4},  // MAP_DEYJA
  {5,    0,    5,    0},  // MAP_BRAKADA_DESERT
  {0,    0,    0,    0},  // MAP_CELESTIA
  {0,    0,    0,    0},  // MAP_THE_PIT
  {0,    0,    0,    0},  // MAP_EVENMORN_ISLE
  {0,    0,    0,    0},  // MAP_MOUNT_NIGHON
  {5,    7,    7,    5},  // MAP_BARROW_DOWNS
  {0,    0,    0,    0},  // MAP_LAND_OF_GIANTS
  {0,    0,    5,    0},  // MAP_TATALIA
  {0,    7,    5,    0},  // MAP_AVLEE
  {0,    0,    0,    0},  // MAP_SHOALS
};


MapStartPoint foot_travel_arrival_points[15][4] =
{
// north                south                east                 west                     from
  {MapStartPoint_Party, MapStartPoint_Party, MapStartPoint_Party, MapStartPoint_Party}, // MAP_EMERALD_ISLE
  {MapStartPoint_South, MapStartPoint_North, MapStartPoint_South, MapStartPoint_East},  // MAP_HARMONDALE
  {MapStartPoint_South, MapStartPoint_North, MapStartPoint_West,  MapStartPoint_East},  // MAP_STEADWICK
  {MapStartPoint_East,  MapStartPoint_North, MapStartPoint_Party, MapStartPoint_East},  // MAP_PIERPONT
  {MapStartPoint_West,  MapStartPoint_North, MapStartPoint_West,  MapStartPoint_North}, // MAP_DEYJA
  {MapStartPoint_South, MapStartPoint_Party, MapStartPoint_West,  MapStartPoint_Party}, // MAP_BRAKADA_DESERT
  {MapStartPoint_Party, MapStartPoint_Party, MapStartPoint_Party, MapStartPoint_Party}, // MAP_CELESTIA
  {MapStartPoint_Party, MapStartPoint_Party, MapStartPoint_Party, MapStartPoint_Party}, // MAP_THE_PIT
  {MapStartPoint_Party, MapStartPoint_Party, MapStartPoint_Party, MapStartPoint_Party}, // MAP_EVENMORN_ISLE
  {MapStartPoint_Party, MapStartPoint_Party, MapStartPoint_Party, MapStartPoint_Party}, // MAP_MOUNT_NIGHON
  {MapStartPoint_South, MapStartPoint_East,  MapStartPoint_South, MapStartPoint_East},  // MAP_BARROW_DOWNS
  {MapStartPoint_Party, MapStartPoint_Party, MapStartPoint_Party, MapStartPoint_Party}, // MAP_LAND_OF_GIANTS
  {MapStartPoint_Party, MapStartPoint_Party, MapStartPoint_West,  MapStartPoint_Party}, // MAP_TATALIA
  {MapStartPoint_Party, MapStartPoint_North, MapStartPoint_North, MapStartPoint_Party}, // MAP_AVLEE
  {MapStartPoint_Party, MapStartPoint_Party, MapStartPoint_Party, MapStartPoint_Party}, // MAP_SHOALS
};


//----- (0048902E) --------------------------------------------------------
bool OutdoorLocation::GetTravelDestination(signed int sPartyX, signed int sPartyZ, char *pOut, signed int a5)
{
  OutdoorLocation *v5; // esi@1
  char *v6; // eax@3
  int v7; // eax@3
  //int v8; // edx@3
  signed int v9; // esi@7
  //int v10; // esi@23
  signed int v11; // eax@23
  //int v12; // ST14_4@25
  signed int v14; // [sp-4h] [bp-84h]@6
  char Str[140]; // [sp+8h] [bp-78h]@3
  //int a5a; // [sp+94h] [bp+14h]@3

  auto Source = this;

  v5 = Source;
  if ( a5 < 10
    || strlen(Source->pLevelFilename) != 9
    || (strcpy(Str, v5->pLevelFilename),
        _strlwr(Str),
        v6 = strtok(Str, "out"),
        v6[2] = 0,
        v7 = atoi(v6),
        //v8 = v7,
        //a5a = v7,
        v7 < 1)
    || v7 > 15 )
    return 0;
  if ( sPartyX < -22528 )
  {
    v14 = 4;
LABEL_7:
    v9 = v14;
    goto LABEL_14;
  }
  if ( sPartyX > 22528 )
  {
    v14 = 3;
    goto LABEL_7;
  }
  if ( sPartyZ < -22528 )
  {
    v14 = 2;
    goto LABEL_7;
  }
  if ( sPartyZ <= 22528 )
    return 0;
  v9 = 1;
LABEL_14:
  if ( v7 == 14 )
  {
    if ( v9 == 4 ) // to & from Shoals
    {
      if ( pPlayers[1]->HasUnderwaterSuitEquipped() &&
           pPlayers[2]->HasUnderwaterSuitEquipped() &&
           pPlayers[3]->HasUnderwaterSuitEquipped() &&
           pPlayers[4]->HasUnderwaterSuitEquipped())
      {
        uDefaultTravelTime_ByFoot = 1;
        strcpy(pOut, "out15.odm");
        uLevel_StartingPointType = MapStartPoint_East;
LABEL_18:
        LOWORD(pParty->uFlags) &= 0xFD7Bu;
        return 1;
      }
      //v8 = v7;
    }
  }
  else
  {
    if ( v7 == 15 && v9 == 3 )
    {
      uDefaultTravelTime_ByFoot = 1;
      strcpy(pOut, "out14.odm");
      uLevel_StartingPointType = MapStartPoint_West;
      goto LABEL_18;
    }
  }
  //v10 = v9 + 4 * v7;
  //v11 = (unsigned __int8)aTiletableLoadU[v10 + 39];
  v11 = foot_travel_destinations[v7 - 1][v9 - 1];
  if (v11 == MAP_INVALID)
    return false;

  assert(v11 <= MAP_SHOALS);

  uDefaultTravelTime_ByFoot = foot_travel_times[v7 - 1][v9 - 1];
  uLevel_StartingPointType = foot_travel_arrival_points[v7 - 1][v9 - 1];
  sprintf(pOut, "out%02d.odm", v11);
  return 1;
  //}
  //return 0;
}
// 6BD07C: using guessed type int uDefaultTravelTime_ByFoot;
// 6BE35C: using guessed type int uLevel_StartingPointType;

//----- (0048917E) --------------------------------------------------------
void OutdoorLocation::MessWithLUN()
{
  OutdoorLocation *v1; // esi@1
  unsigned int *v2; // ebp@1
  unsigned int v3; // eax@1
  unsigned __int16 v4; // ax@1
  signed int v5; // ebx@1

  v1 = this;
  v2 = this->pSpriteIDs_LUN;
  this->pSpriteIDs_LUN[0] = -1;
  this->pSpriteIDs_LUN[2] = pSpriteFrameTable->FastFindSprite("LUN1-4");
  v1->pSpriteIDs_LUN[4] = pSpriteFrameTable->FastFindSprite("LUN1-2");
  v1->pSpriteIDs_LUN[6] = pSpriteFrameTable->FastFindSprite("LUN3-4");
  v1->uSpriteID_LUNFULL = pSpriteFrameTable->FastFindSprite("LUNFULL");
  v1->uSpriteID_LUN3_4_cp = pSpriteFrameTable->FastFindSprite("LUN3-4");
  v1->uSpriteID_LUN1_2_cp = pSpriteFrameTable->FastFindSprite("LUN1-2");
  v3 = pSpriteFrameTable->FastFindSprite("LUN1-4");
  v1->field_D60 = -1;
  v1->uSpriteID_LUN1_4_cp = v3;
  v1->field_CF0 = 4;
  v1->field_CF8 = 4;
  v1->field_D00 = 4;
  LOWORD(v3) = *(short *)v2;
  v1->pSpriteIDs_LUN[1] = 0;
  v1->pSpriteIDs_LUN[3] = 0;
  v1->pSpriteIDs_LUN[5] = 0;
  v1->pSpriteIDs_LUN[7] = 0;
  v1->field_CE8 = 0;
  LOWORD(v1->field_D3C) = v3;
  v1->field_D40 = 0;
  v1->field_D44 = 0;
  v1->field_D48 = 0;
  v1->field_D4C = 131072;
  v1->field_D5C = 0;
  v1->field_D64 = 0;
  v4 = pSpriteFrameTable->FastFindSprite("LUN-SUN");
  v1->field_D28 = -1;
  v1->field_D08 = 0;
  v1->field_D0C = 0;
  v1->field_D10 = 0;
  v1->field_D24 = 0;
  v1->field_D2C = 0;
  v1->uSpriteID_LUN_SUN = v4;
  v1->field_D14 = -131072;
  v5 = 8;
  do
  {
    pSpriteFrameTable->InitializeSprite(*v2);
    v2 += 2;
    --v5;
  }
  while ( v5 );
  pSpriteFrameTable->InitializeSprite(v1->uSpriteID_LUN_SUN);
}

//----- (004892E6) --------------------------------------------------------
void OutdoorLocation::UpdateSunlightVectors()
{
  //unsigned int result; // eax@1
  OutdoorLocation *v2; // esi@1
  unsigned int v3; // edi@3
  //int v4; // ebx@3
  int v5; // eax@3
  //int v6; // eax@3
  //int v7; // ecx@3
  double v8; // st7@4

  v2 = this;
  if ( pParty->uCurrentHour >= 5 && pParty->uCurrentHour < 21 )
  {
    v3 = pParty->uCurrentMinute + 60 * (pParty->uCurrentHour - 5);
    //v4 = (signed int)(v3 * stru_5C6E00->uIntegerPi) / 960;
    v5 = stru_5C6E00->Cos((signed int)(v3 * stru_5C6E00->uIntegerPi) / 960);
    v2->inv_sunlight_y = 0;
    v2->inv_sunlight_x = v5;
    //v6 = stru_5C6E00->Sin(v4);
    //v7 = v2->field_D18;
    v2->inv_sunlight_z = stru_5C6E00->Sin((signed int)(v3 * stru_5C6E00->uIntegerPi) / 960);
    v2->vSunlight.x = -v2->inv_sunlight_x;
    v2->vSunlight.y = -v2->inv_sunlight_y;
    v2->vSunlight.z = -v2->inv_sunlight_z;
    if ( (signed int)v3 >= 480 )
      v8 = (double)(signed int)(960 - v3);
    else
      v8 = (double)(signed int)v3;
    v2->max_terrain_dimming_level = (signed __int64)(20.0 - v8 / 480.0 * 20.0);
    v2->uLastSunlightUpdateMinute = pParty->uCurrentMinute;
  }
}

//----- (004893C1) --------------------------------------------------------
void OutdoorLocation::UpdateFog()
{
  fFogDensity = GetFogDensityByTime();
}

//----- (004893CF) --------------------------------------------------------
int OutdoorLocation::GetNumFoodRequiredToRestInCurrentPos(int x, signed int y, int z)
{
  OutdoorLocation *v4; // ebx@1
  signed int v5; // edi@4
  int v6; // eax@4
  int v7; // eax@4
  int v8; // eax@4
  int v9; // eax@5
  int v10; // eax@6
  int v11; // eax@7
  int v12; // eax@8
  int v15; // [sp+8h] [bp-8h]@2
  int v16; // [sp+Ch] [bp-4h]@2

  v4 = this;
  if ( pParty->uFlags & 8
    || (v15 = 0,
        v16 = 0,
        ODM_GetFloorLevel(x, y, z, pParty->uDefaultPartyHeight, &v15, &v16, 0),
        v16)
    || v15 )
    return 2;
  v5 = WorldPosToGridCellX(pParty->vPosition.x);
  v6 = WorldPosToGridCellZ(pParty->vPosition.y);
  v7 = _47ED83(v5, v6 - 1);
  v8 = pTileTable->pTiles[GetTileIdByTileMapId(v7)].tileset;
  if ( v8 )
  {
    v9 = v8 - 1;
    if ( !v9 )
      return 3;
    v10 = v9 - 1;
    if ( !v10 )
      return 5;
    v11 = v10 - 1;
    if ( !v11 || (v12 = v11 - 3) == 0 )
      return 4;
    if ( v12 == 1 )
      return 3;
    return 2;
  }
  return 1;
}


//----- (00489487) --------------------------------------------------------
void OutdoorLocation::SetFog()
{
  strcpy(pOutdoor->pLevelFilename, pCurrentMapName);

  auto map_id = pMapStats->GetMapInfo(pCurrentMapName);
  if (map_id == MAP_INVALID || map_id == MAP_CELESTIA || map_id == MAP_THE_PIT || map_id > MAP_SHOALS)
    return;

  uint chance = rand() % 100;

  if (chance < fog_probability_table[map_id - 1].small_fog_chance)
  {
    ::day_fogrange_1 = 4096;
    ::day_fogrange_2 = 8192;
    ::day_attrib |= DAY_ATTRIB_FOG;
  }
  else if (chance < fog_probability_table[map_id - 1].small_fog_chance +
                    fog_probability_table[map_id - 1].average_fog_chance)
  {
    ::day_fogrange_2 = 4096;
    ::day_fogrange_1 = 0;
    ::day_attrib |= DAY_ATTRIB_FOG;
  }
  else if (fog_probability_table[map_id - 1].dense_fog_chance &&
           chance < fog_probability_table[map_id - 1].small_fog_chance + 
                    fog_probability_table[map_id - 1].average_fog_chance +
                    fog_probability_table[map_id - 1].dense_fog_chance)
  {
    ::day_fogrange_2 = 2048;
    ::day_fogrange_1 = 0;
    ::day_attrib |= DAY_ATTRIB_FOG;
  }
  else
    ::day_attrib &= ~DAY_ATTRIB_FOG;

  if ( Is_out15odm_underwater() )
    SetUnderwaterFog();
  pOutdoor->loc_time.day_fogrange_1 = ::day_fogrange_1;
  pOutdoor->loc_time.day_fogrange_2 = ::day_fogrange_2;
  pOutdoor->loc_time.day_attrib = ::day_attrib;
}

//----- (00482170) --------------------------------------------------------
bool ODMFace::IsBackfaceCulled(ODMFace *a1, RenderVertexSoft *a2, struct Polygon *a3)
{
  struct Polygon *v3; // edi@1
  RenderVertexSoft *v4; // esi@1
  unsigned int v5; // edx@1
  RenderVertexSoft *v6; // ecx@2
  double v7; // st7@5
  double v8; // st6@5
  double v9; // st5@5
  double v10; // st6@9
  double v11; // st5@9
  double v12; // st4@9
  bool result; // eax@16
  double v14; // ST2C_8@17
  double v15; // ST20_8@17
  double v16; // ST0C_8@17
  double v17; // ST0C_8@17
  float v18; // [sp+8h] [bp-38h]@5
  float v19; // [sp+10h] [bp-30h]@5
  float v20; // [sp+14h] [bp-2Ch]@5
  float v21; // [sp+18h] [bp-28h]@5
  float v22; // [sp+1Ch] [bp-24h]@5
  float v23; // [sp+24h] [bp-1Ch]@5
  float v24; // [sp+28h] [bp-18h]@5
  float v25; // [sp+30h] [bp-10h]@5
  float v26; // [sp+34h] [bp-Ch]@5
  float v27; // [sp+38h] [bp-8h]@5
  float v28; // [sp+3Ch] [bp-4h]@5
  float a3a; // [sp+48h] [bp+8h]@5
  float a3b; // [sp+48h] [bp+8h]@17
  float a3c; // [sp+48h] [bp+8h]@17
  float a3d; // [sp+48h] [bp+8h]@17
  float a3e; // [sp+48h] [bp+8h]@17

  v3 = a3;
  v4 = a2;
  v5 = a3->uNumVertices;
  if ( (signed int)v5 < 3 )
    goto LABEL_20;
  v6 = &v4[v5 - 1];
  if ( v4->vWorldPosition.z == v4[1].vWorldPosition.z && v4[1].vWorldPosition.z == v6->vWorldPosition.z )
    *(int *)&a3->flags |= 0x10u;
  v19 = v4[1].vWorldViewPosition.x - v4->vWorldViewPosition.x;
  v18 = v4[1].vWorldViewPosition.y - v4->vWorldViewPosition.y;
  v20 = v4[1].vWorldViewPosition.z - v4->vWorldViewPosition.z;
  v21 = v6->vWorldViewPosition.x - v4->vWorldViewPosition.x;
  v22 = v6->vWorldViewPosition.y - v4->vWorldViewPosition.y;
  v23 = v6->vWorldViewPosition.z - v4->vWorldViewPosition.z;
  v28 = v4[1].vWorldPosition.x - v4->vWorldPosition.x;
  v27 = v4[1].vWorldPosition.y - v4->vWorldPosition.y;
  a3a = v4[1].vWorldPosition.z - v4->vWorldPosition.z;
  v7 = v6->vWorldPosition.x - v4->vWorldPosition.x;
  v8 = v6->vWorldPosition.y - v4->vWorldPosition.y;
  v9 = v6->vWorldPosition.z - v4->vWorldPosition.z;
  v26 = v27 * v9 - v8 * a3a;
  v24 = v7 * a3a - v9 * v28;
  v25 = v8 * v28 - v7 * v27;
  if ( v26 == 0.0 && v24 == 0.0 && v25 == 0.0 )
  {
    while ( 1 )
    {
      --v5;
      if ( (signed int)v5 < 2 )
        break;
      v10 = v6->vWorldPosition.x - v4->vWorldPosition.x;
      v11 = v6->vWorldPosition.y - v4->vWorldPosition.y;
      v12 = v6->vWorldPosition.z - v4->vWorldPosition.z;
      v26 = v27 * v12 - v11 * a3a;
      v24 = v10 * a3a - v12 * v28;
      v25 = v11 * v28 - v10 * v27;
      if ( v26 != 0.0 )
        break;
      if ( v24 != 0.0 || v25 != 0.0 )
        break;
      --v6;
    }
    v21 = v6->vWorldViewPosition.x - v4->vWorldViewPosition.x;
    v22 = v6->vWorldViewPosition.y - v4->vWorldViewPosition.y;
    v23 = v6->vWorldViewPosition.z - v4->vWorldViewPosition.z;
  }
  if ( ((double)pGame->pIndoorCameraD3D->vPartyPos.x - v4->vWorldPosition.x) * v26
     + ((double)pGame->pIndoorCameraD3D->vPartyPos.z - v4->vWorldPosition.z) * v25
     + ((double)pGame->pIndoorCameraD3D->vPartyPos.y - v4->vWorldPosition.y) * v24 > 0.0 )
  {
    a3b = v23 * v18 - v22 * v20;
    v14 = a3b + 6.7553994e15;
    v3->v_18.x = LODWORD(v14);
    a3c = v21 * v20 - v23 * v19;
    v15 = a3c + 6.7553994e15;
    v3->v_18.y = LODWORD(v15);
    a3d = v22 * v19 - v21 * v18;
    v16 = a3d + 6.7553994e15;
    v3->v_18.z = LODWORD(v16);
    v3->_normalize_v_18();
    a3e = -((double)v3->v_18.x * v4->vWorldViewPosition.x)
        - (double)v3->v_18.y * v4->vWorldViewPosition.y
        - (double)v3->v_18.z * v4->vWorldViewPosition.z;
    v17 = a3e + 6.7553994e15;
    v3->field_24 = LODWORD(v17);
    result = 1;
  }
  else
  {
LABEL_20:
    result = 0;
  }
  return result;
}


//----- (0047C7A9) --------------------------------------------------------
void OutdoorLocationTerrain::_47C7A9()
{
  this->field_10 = 0;
  this->field_12 = 0;
  this->field_16 = 0;
  this->field_14 = 0;
  this->field_1C = 0;
  this->field_18 = 0;
}

//----- (0047C7C2) --------------------------------------------------------
void OutdoorLocationTerrain::Release()//очистить локацию
{
  if (pHeightmap)
  {
    free(this->pHeightmap);
    pHeightmap = nullptr;
  }
  if (pTilemap)
  {
    free(pTilemap);
    pTilemap = nullptr;
  }
  if (pAttributemap)
  {
    free(pAttributemap);
    pAttributemap = nullptr;
  }
  if (pDmap)
  {
    free(pDmap);
    pDmap = nullptr;
  }

  _47C7A9();
}

//----- (0047C80A) --------------------------------------------------------
void OutdoorLocationTerrain::FillDMap( int X, int Y, int W, int Z )
    {
  OutdoorLocationTerrain *pOutLocTerrain; // ebx@1
  double v6; // st7@1
  double v7; // st7@2
  double v8; // st7@2
  int result; // eax@3
  int v10; // eax@4
  int v11; // ecx@5
  int v12; // ecx@6
  int v13; // edi@7
  int v14; // edx@9
  int v15; // eax@15
  unsigned __int8 *pMapHeight; // ebx@15
  int v17; // eax@15
  int v18; // ecx@15
  int v19; // esi@15
  int v20; // edi@15
  int v21; // edx@15
  int v22; // ecx@15
  int v23; // ebx@15
  int v24; // ecx@15
  int v25; // ST28_4@15
  double v26; // st7@15
  double v27; // st6@15
  double v28; // st5@15
  double v29; // st7@15
  double v30; // st7@16
  double v31; // st7@17
  int v32; // eax@21
  double v33; // st7@21
  double v34; // st6@21
  double v35; // st5@21
  double v36; // st7@21
  double v37; // st7@22
  double v38; // st7@23
  int v39; // [sp+14h] [bp-34h]@8
  int v40; // [sp+18h] [bp-30h]@15
  int v41; // [sp+1Ch] [bp-2Ch]@15
  int v42; // [sp+20h] [bp-28h]@15
  OutdoorLocationTerrain *pOutLocTerrain2; // [sp+24h] [bp-24h]@1
  int v44; // [sp+28h] [bp-20h]@21
  float v45; // [sp+2Ch] [bp-1Ch]@1
  float v46; // [sp+30h] [bp-18h]@1
  float v47; // [sp+34h] [bp-14h]@1
  int v48; // [sp+38h] [bp-10h]@7
  int v49; // [sp+3Ch] [bp-Ch]@10
  int v50; // [sp+40h] [bp-8h]@9
  float v51; // [sp+44h] [bp-4h]@15
  float v52; // [sp+44h] [bp-4h]@21
  float v53; // [sp+50h] [bp+8h]@15
  float v54; // [sp+50h] [bp+8h]@21
  int v55; // [sp+54h] [bp+Ch]@15
  float v56; // [sp+54h] [bp+Ch]@15
  float v57; // [sp+54h] [bp+Ch]@21

  v46 = -64.0;
  v47 = -64.0;
  pOutLocTerrain = this;
  v45 = 64.0;
  pOutLocTerrain2 = this;
  v6 = sqrt(12288.0);
  if ( v6 != 0.0 )
  {
    v7 = 1.0 / v6;
    v45 = 64.0 * v7;
    v8 = v7 * -64.0;
    v46 = v8;
    v47 = v8;
  }
  result = Y;
  if ( Y > Z )
  {
    v10 = Z ^ Y;
    Z ^= Y ^ Z;
    result = Z ^ v10;
  }
  v11 = X;
  if ( X > W )
  {
    v12 = W ^ X;
    W ^= X ^ W;
    v11 = W ^ v12;
  }
  v13 = result - 1;
  v48 = result - 1;
  if ( result - 1 <= Z )
  {
    v39 = v11 - 1;
    do
    {
      v14 = v39;
      v50 = v39;
      if ( v39 <= W )
      {
        result = (v39 - 63) << 9;
        v49 = (v39 - 63) << 9;
        do
        {
          if ( v13 >= 0 && result >= -32256 && v13 <= 127 && result <= 32768 )
          {
            v15 = pOutLocTerrain->field_10;
            v55 = v15;
            pMapHeight = pOutLocTerrain->pHeightmap;
            v17 = (int)(&pMapHeight[v13 * v15] + v14);
            v18 = -v13;
            v19 = (64 - v13) << 9;
            v20 = 32 * *(char *)v17;
            v21 = 32 * *(char *)(v17 + 1);
            v22 = (v18 + 63) << 9;
            v41 = v22;
            v23 = (int)(&pMapHeight[v55 * (v48 + 1)] + v50);
            v24 = v22 - v19;
            v40 = 32 * *(char *)v23;
            v42 = 32 * *(char *)(v23 + 1);


            v25 = v49 - 512 - v49;
            v26 = (double)-((v20 - v21) * v24);
            v51 = v26;
            v27 = (double)-(v25 * (v42 - v21));
            v53 = v27;
            v28 = (double)(v25 * v24);
            v56 = v28;
            v29 = sqrt(v28 * v28 + v27 * v27 + v26 * v26);
            if ( v29 != 0.0 )
            {
              v30 = 1.0 / v29;
              v51 = v51 * v30;
              v53 = v53 * v30;
              v56 = v30 * v56;
            }
            v31 = (v56 * v47 + v53 * v46 + v51 * v45) * 31.0;
            if ( v31 < 0.0 )
              v31 = 0.0;
            if ( v31 > 31.0 )
              v31 = 31.0;
            v44 = 2 * (v50 + v48 * pOutLocTerrain2->field_10);
            pOutLocTerrain = pOutLocTerrain2;
            *((char *)pOutLocTerrain2->pDmap + v44 + 1) = (signed __int64)v31;

            v32 = v49 - (v49 - 512);
            v33 = (double)-((v42 - v40) * (v19 - v41));
            v52 = v33;
            v34 = (double)-(v32 * (v20 - v40));
            v54 = v34;
            v35 = (double)(v32 * (v19 - v41));
            v57 = v35;
            v36 = sqrt(v35 * v35 + v34 * v34 + v33 * v33);
            if ( v36 != 0.0 )
            {
              v37 = 1.0 / v36;
              v52 = v52 * v37;
              v54 = v54 * v37;
              v57 = v37 * v57;
            }
            v38 = (v57 * v47 + v54 * v46 + v52 * v45) * 31.0;
            if ( v38 < 0.0 )
              v38 = 0.0;
            if ( v38 > 31.0 )
              v38 = 31.0;
            v13 = v48;
            *((char *)pOutLocTerrain2->pDmap + v44) = (signed __int64)v38;
            v14 = v50;
            result = v49;
          }
          ++v14;
          result += 512;
          v50 = v14;
          v49 = result;
        }
        while ( v14 <= W );
      }
      ++v13;
      v48 = v13;
    }
    while ( v13 <= Z );
  }
}

//----- (0047CB57) --------------------------------------------------------
int OutdoorLocationTerrain::_47CB57(int a1, int a2, int a3)
{
  signed int result; // eax@2
  unsigned __int16 *v5; // edx@3
  double v6; // st7@3
  char v7; // bl@3
  int v8; // eax@3
  int v9; // eax@4
  int v10; // eax@5
  double v11; // st6@7
  signed int v12; // edi@7
  int v13; // esi@9
  char *v14; // esi@10
  signed int v15; // ecx@10
  char v16[256]; // [sp+4h] [bp-124h]@9
  unsigned __int16 *v17; // [sp+104h] [bp-24h]@3
  unsigned int v18; // [sp+108h] [bp-20h]@3
  unsigned int v19; // [sp+10Ch] [bp-1Ch]@3
  unsigned int v20; // [sp+110h] [bp-18h]@3
  unsigned int v21; // [sp+114h] [bp-14h]@3
  float v22; // [sp+118h] [bp-10h]@3
  float v23; // [sp+11Ch] [bp-Ch]@3
  int i; // [sp+120h] [bp-8h]@3
  unsigned int v25; // [sp+124h] [bp-4h]@5
  signed int a2a; // [sp+134h] [bp+Ch]@3
  unsigned int a2b; // [sp+134h] [bp+Ch]@7
  float a3a; // [sp+138h] [bp+10h]@7
  int a3b; // [sp+138h] [bp+10h]@9

  if ( pRenderer->pRenderD3D )
  {
    result = 0;
  }
  else
  {
    v5 = PaletteManager::Get_Dark_or_Red_LUT(a2, 0, 1);
    v6 = 0.0;
    v7 = LOBYTE(pRenderer->uTargetBBits);
    v19 = pRenderer->uTargetRMask;
    v21 = pRenderer->uTargetGMask;
    v22 = 0.0;
    v20 = pRenderer->uTargetBMask;
    v8 = 0;
    v17 = v5;
    v23 = 0.0;
    v18 = pRenderer->uTargetBBits;
    a2a = 0;
    for ( i = 0; i < a3; ++i )
    {
      v9 = *(char *)(v8 + a1);
      if ( v9 )
      {
        v10 = v5[v9];
        v6 = v6 + (double)((signed int)(v19 & v10) >> (LOBYTE(pRenderer->uTargetBBits) + LOBYTE(pRenderer->uTargetGBits)));
        ++a2a;
        v25 = v20 & v10;
        v22 = (double)((signed int)(v21 & v10) >> SLOBYTE(pRenderer->uTargetBBits)) + v22;
        v23 = (double)(signed int)(v20 & v10) + v23;
      }
      v8 = i + 1;
    }
    v11 = 1.0 / (double)a2a;
    a3a = v11;
    v25 = (signed __int64)(a3a * v22);
    i = (signed __int64)(a3a * v23);
    v12 = 0;
    a2b = pRenderer->uTargetBBits + pRenderer->uTargetGBits;
    while ( 1 )
    {
      v13 = v17[v12];
      a3b = abs((__int64)(signed __int64)(v11 * v6) - ((signed int)(v19 & v17[v12]) >> a2b));
      BYTE3(a3b) = abs((signed)v25 - ((signed int)(v21 & v13) >> v7)) + a3b;
      v16[v12++] = abs((signed)i - (signed)(v20 & v13)) + BYTE3(a3b);
      if ( v12 >= 256 )
        break;
      v7 = v18;
    }
    result = 0;
    v14 = (char *)&pPaletteManager->field_D1600[42][23][116];
    v15 = 0;
    do
    {
      if ( (unsigned __int8)v16[v15] < (signed int)v14 )
      {
        v14 = (char *)(unsigned __int8)v16[v15];
        result = v15;
      }
      ++v15;
    }
    while ( v15 < 256 );
  }
  return result;
}
// 47CB57: using guessed type char var_124[256];

//----- (0047CCE2) --------------------------------------------------------
bool OutdoorLocationTerrain::ZeroLandscape()
{
  OutdoorLocationTerrain *v1; // esi@1

  v1 = this;
  memset(this->pHeightmap, 0, 0x4000u);
  memset(v1->pTilemap, 90, 0x4000u);
  memset(v1->pAttributemap, 0, 0x4000u);
  memset(v1->pDmap, 0, 0x8000u);
  v1->field_12 = 128;
  v1->field_10 = 128;
  v1->field_16 = 7;
  v1->field_14 = 7;
  v1->field_1C = 127;
  v1->field_18 = 127;
  return 1;
}

//----- (0047CD44) --------------------------------------------------------
bool OutdoorLocationTerrain::Initialize()
{
  pHeightmap = (unsigned __int8 *)malloc(0x4000u);//height map
  pTilemap = (unsigned __int8 *)malloc(0x4000u);//tile map
  pAttributemap = (unsigned __int8 *)malloc(0x4000u);//карта атрибутов
  pDmap = (struct DMap *)malloc(0x8000u);
  if (pHeightmap && pTilemap && pAttributemap && pDmap )
    return true;
  else
    return false;
}

//----- (0047CDE2) --------------------------------------------------------
void OutdoorLocation::CreateDebugLocation()
{
  OutdoorLocation *v1; // esi@1
  void *v2; // eax@1
  void *v3; // ST14_4@1
  void *v4; // eax@1
  void *v5; // ST14_4@1
  void *v6; // eax@1
  unsigned int v7; // eax@1
  //char v8; // zf@1

  v1 = this;
  strcpy(this->pLevelFilename, "blank");
  strcpy(v1->pLocationFileName, "i6.odm");
  strcpy(v1->pLocationFileDescription, "MM6 Outdoor v1.00");
  v1->uNumBModels = 0;
  v1->pTileTypes[0].tileset = Tileset_Grass;
  v1->pTileTypes[1].tileset = Tileset_Water;
  v1->pTileTypes[2].tileset = Tileset_6;
  v1->pTileTypes[3].tileset = Tileset_RoadGrassCobble;
  v1->LoadTileGroupIds();
  v1->LoadRoadTileset();
  free(v1->pBModels);
  free(v1->pSpawnPoints);
  v1->pBModels = 0;
  v1->pSpawnPoints = 0;
  v1->pTerrain.Initialize();
  v1->pTerrain.ZeroLandscape();
  v1->pTerrain.FillDMap(0, 0, 128, 128);
  free(v1->pCmap);
  v1->pCmap = 0;
  v2 = malloc(0x8000u);
  v3 = v1->pOMAP;
  v1->pCmap = v2;
  free(v3);
  v1->pOMAP = 0;
  v4 = malloc(0x10000u);
  v1->pOMAP = (unsigned int *)v4;
  memset(v4, 0, 0x10000u);
  v5 = v1->pFaceIDLIST;
  v1->numFaceIDListElems = 0;
  free(v5);
  v1->pFaceIDLIST = 0;
  v6 = malloc(2);
  v1->pFaceIDLIST = (unsigned __int16 *)v6;
  *(short *)v6 = 0;
  strcpy(v1->pSkyTextureName, pDefaultSkyTexture.data());
  v1->uSky_TextureID = pBitmaps_LOD->LoadTexture(v1->pSkyTextureName);
  strcpy(v1->pGroundTileset, byte_6BE124_cfg_textures_DefaultGroundTexture.data());
  v7 = pBitmaps_LOD->LoadTexture(v1->pGroundTileset);
  v1->uMainTile_BitmapID = v7;

  if ( v1->uSky_TextureID == -1 )
    Error("Invalid Sky Tex Handle");

  if ( v1->uMainTile_BitmapID == -1 )
    Error("Invalid Ground Tex Handle");
}



//----- (0047CF9C) --------------------------------------------------------
void OutdoorLocation::Release()
{
  //OutdoorLocation *v1; // esi@1
  //signed int v2; // edi@1
  //int v3; // ebx@2
  //void *v4; // ST24_4@4
  //char *v5; // ebx@4
  //void **v6; // esi@4

  //v1 = this;
  strcpy(pLevelFilename, "blank");
  strcpy(pLocationFileName, "default.odm");
  strcpy(pLocationFileDescription, "MM6 Outdoor v1.00");
  strcpy(pSkyTextureName, "sky043");
  strcpy(pGroundTileset, "hm005");

  if (pBModels)
  {
    for (uint i = 0; i < uNumBModels; ++i)
      pBModels[i].Release();

    free(pBModels);
    pBModels = nullptr;
    uNumBModels = 0;
  }

  if (pSpawnPoints)
  {
    free(pSpawnPoints);
    pSpawnPoints = nullptr;
  }
  uNumSpawnPoints = 0;

  pTerrain.Release();

  if (pCmap)
  {
    free(pCmap);
    pCmap = nullptr;
  }
  if (pOMAP)
  {
    free(pOMAP);
    pOMAP = nullptr;
  }
  if (pFaceIDLIST)
  {
    free(pFaceIDLIST);
    pFaceIDLIST = nullptr;
  }
  if (pTerrainNormals)
  {
    free(pTerrainNormals);
    pTerrainNormals = nullptr;
  }
}

//----- (0047D0A6) --------------------------------------------------------
bool OutdoorLocation::Load(const char *pFilename, ODMFace *File, size_t pNumItems, int thisa)//загрузка локации
{
  //OutdoorLocation *pOutdoorLocation; // esi@1
  /*bool result; // eax@9
  bool v7; // ebx@9
  size_t v8; // eax@10
  void *v9; // eax@10
  void *v10; // eax@10
  void *v11; // eax@10
  int v12; // ebx@11
  BSPModel *v13; // eax@12
  void *v14; // eax@12
  BSPModel *v15; // ecx@12
  void *v16; // eax@12
  BSPModel *v17; // ecx@12
  BSPModel *v18; // eax@12
  __int16 v19; // ax@15
  __int16 v20; // ax@16
  int v21; // ecx@16
  ODMFace *v22; // ebx@26
  SpriteObject *pItems; // ecx@27
  unsigned int v24; // eax@28
  //unsigned __int8 v25; // zf@28
  //unsigned __int8 v26; // sf@28
  ODMFace *v27; // eax@28
  const char *i; // edx@29
  unsigned __int16 v29; // ax@33
  unsigned __int16 v30; // ax@37
  int v31; // ecx@37
  int v32; // eax@38
  void *v33; // eax@38
  TileDesc *v34; // eax@43
  unsigned int v35; // eax@43
  unsigned int v36; // edi@43
  unsigned int v37; // edi@45*/
  //size_t v38; // eax@50
  FILE *v39; // eax@50
  //unsigned int v40; // edi@56
  //void *v41; // eax@56
  //void *v42; // ebx@56
  //const void *v43; // ebx@59
  //const void *v44; // ebx@59
  //unsigned int v45; // eax@59
  //BSPModel *v46; // eax@59
  //unsigned int v47; // ecx@59
  //int v48; // ebx@60
  //BSPModel *v49; // eax@61
  //BSPModel *v50; // eax@61
  //BSPModel *v51; // eax@61
  //BSPModel *v52; // eax@61
  //BSPModel *v53; // eax@61
  //BSPModel *v54; // ecx@61
  //BSPModel *v55; // ecx@61
  //BSPModel *v56; // ecx@61
  //void *v57; // ST24_4@61
  //BSPModel *v58; // ecx@61
  //void *v59; // ST18_4@61
  //BSPModel *v60; // eax@61
  //__int16 v61; // ax@64
  unsigned __int16 v62; // ax@65
  //ODMFace *v63; // ecx@65
  //unsigned __int16 v64; // ax@80
  //const char *v65; // ecx@80
  //int v66; // eax@81
  //void *v67; // eax@81
  //int v68; // ecx@81
  //void *v69; // eax@81
  //unsigned int v70; // eax@81
  //SpawnPointMM7 *v71; // eax@81
  //unsigned int v72; // ecx@81
  //size_t v73; // eax@81
  //int v74; // edi@87
  //void *v75; // edi@88
  //unsigned int v76; // edx@94
  //int v77; // ecx@94
  //char *v78; // eax@95
  //unsigned int v79; // edx@97
  //unsigned int v80; // eax@99
  //int v81; // eax@107
  //void *v82; // edi@114
  //size_t v83; // eax@120
  //const void *v84; // edi@120
  //const void *v85; // edi@120
  //BSPModel *v86; // eax@124
  //unsigned int v87; // eax@124
  //BSPModel *v88; // eax@126
  //BSPModel *v89; // eax@127
  //ODMFace *v90; // eax@129
  //const void *v91; // edi@138
  //const void *v92; // edi@141
  //const void *v93; // edi@141
  //const void *v94; // edi@144
  //const void *v95; // edi@144
  //const char *v96; // edi@147
  //unsigned int v97; // eax@147
  //TileDesc *v98; // eax@147
  //unsigned int v99; // eax@147
  //int v100; // ecx@150
  //unsigned int v101; // eax@157
  int v102; // edi@159
  //void *v103; // [sp-14h] [bp-B94h]@55
  //void *v104; // [sp-10h] [bp-B90h]@59
  //size_t v105; // [sp-Ch] [bp-B8Ch]@59
  //char *v106; // [sp-8h] [bp-B88h]@59
  int v107; // [sp-4h] [bp-B84h]@12
  int v108; // [sp+0h] [bp-B80h]@10
  char Src[968]; // [sp+10h] [bp-B70h]@110
  char Dst[968]; // [sp+3D8h] [bp-7A8h]@50
  char Str[256]; // [sp+7A0h] [bp-3E0h]@50
  /*char DstBuf; // [sp+8A0h] [bp-2E0h]@10
  __int32 Offset; // [sp+8A4h] [bp-2DCh]@10
  __int32 v114; // [sp+8B0h] [bp-2D0h]@10
  __int32 v115; // [sp+8BCh] [bp-2C4h]@10
  __int32 v116; // [sp+8C8h] [bp-2B8h]@10
  __int32 v117; // [sp+8D4h] [bp-2ACh]@10
  __int32 v118; // [sp+8E0h] [bp-2A0h]@10
  __int32 v119; // [sp+8ECh] [bp-294h]@10
  __int32 v120; // [sp+8F8h] [bp-288h]@10
  __int32 v121; // [sp+904h] [bp-27Ch]@10
  __int32 v122; // [sp+910h] [bp-270h]@10
  __int32 v123; // [sp+91Ch] [bp-264h]@10
  __int32 v124; // [sp+928h] [bp-258h]@26
  __int32 v125; // [sp+934h] [bp-24Ch]@35
  __int32 v126; // [sp+940h] [bp-240h]@38
  __int32 v127; // [sp+94Ch] [bp-234h]@38
  __int32 v128; // [sp+958h] [bp-228h]@38
  __int32 v129; // [sp+964h] [bp-21Ch]@38
  __int32 v130; // [sp+970h] [bp-210h]@38
  __int32 v131; // [sp+97Ch] [bp-204h]@38
  __int32 v132; // [sp+988h] [bp-1F8h]@38
  __int32 v133; // [sp+994h] [bp-1ECh]@38
  __int32 v134; // [sp+9A0h] [bp-1E0h]@38
  __int32 v135; // [sp+9ACh] [bp-1D4h]@38
  __int32 v136; // [sp+9D0h] [bp-1B0h]@10*/
  //char FileName[8]; // [sp+A20h] [bp-160h]@8
  //char v138; // [sp+A28h] [bp-158h]@12
  //int v139; // [sp+B1Ch] [bp-64h]@10
  //char pContainer[32]; // [sp+B20h] [bp-60h]@1
  //int *v141; // [sp+B40h] [bp-40h]@50
  //__int64 v142; // [sp+B44h] [bp-3Ch]@55
  //size_t pSource; // [sp+B4Ch] [bp-34h]@56
  //int v144; // [sp+B50h] [bp-30h]@61
  //int v145; // [sp+B54h] [bp-2Ch]@68
  ODMHeader header; // [sp+B58h] [bp-28h]@50
  //unsigned int pDestLen; // [sp+B68h] [bp-18h]@13
  //FILE *ptr; // [sp+B6Ch] [bp-14h]@12
  //void *v149; // [sp+B70h] [bp-10h]@19
  char *Str2; // [sp+B74h] [bp-Ch]@12
  //int v151; // [sp+B78h] [bp-8h]@59
  //void *uSourceLen; // [sp+B7Ch] [bp-4h]@59

  //pOutdoorLocation = this;
  //strcpy(pContainer, pFilename);

  if (bUnderwater)
  {
    pPaletteManager->pPalette_tintColor[0] = 0x10;
    pPaletteManager->pPalette_tintColor[1] = 0xC2;
    pPaletteManager->pPalette_tintColor[2] = 0x99;
    pPaletteManager->SetMistColor(37, 143, 92);
  }
  else
  {
    pPaletteManager->pPalette_tintColor[0] = 0;
    pPaletteManager->pPalette_tintColor[1] = 0;
    pPaletteManager->pPalette_tintColor[2] = 0;
    if (pPaletteManager->pPalette_mistColor[0] != 0x80 ||
        pPaletteManager->pPalette_mistColor[1] != 0x80 ||
        pPaletteManager->pPalette_mistColor[2] != 0x80)
    {
      pPaletteManager->SetMistColor(128, 128, 128);
      pPaletteManager->RecalculateAll();
    }
  }

  _6807E0_num_decorations_with_sounds_6807B8 = 0;
  /*sprintf(FileName, "levels\\%s", pContainer);
  if ( GetFileAttributesA(FileName) != -1 )
  {
    result = (bool)fopen(FileName, "rb");
    v7 = result;
    File = (ODMFace *)result;
    if ( !result )
      return result;
    *(int *)thisa = 1;
    v8 = strlen(pContainer);
    v108 = 2;
    *((char *)&v139 + v8) = 0;
    viewparams->uTextureID_LocationMap = pIcons_LOD->LoadTexture(pContainer, (enum TEXTURE_TYPE)v108);
    fread(&DstBuf, 0x180u, 1u, (FILE *)v7);
    fseek((FILE *)v7, Offset, 0);
    fread(this, 0xB0u, 1u, (FILE *)v7);
    LoadTileGroupIds();
    LoadRoadTileset();
    strcpy(pGroundTileset, "grastyl");
    fseek((FILE *)v7, v114, 0);
    fread(&uNumBModels, 4u, 1u, (FILE *)v7);
    fseek((FILE *)v7, v115, 0);
    fread(&uNumSpriteObjects, 4u, 1u, (FILE *)v7);
    fseek((FILE *)v7, v116, 0);
    fread(&uNumLevelDecorations, 4u, 1u, (FILE *)v7);
    fseek((FILE *)v7, v117, 0);
    fread(&uNumActors, 4u, 1u, (FILE *)v7);
    fseek((FILE *)v7, v118, 0);
    fread(&uNumChests, 4u, 1u, (FILE *)v7);
    pTerrain.Initialize();
    fseek((FILE *)v7, v119, 0);
    fread(pTerrain.pHeightmap, 1u, 0x4000u, (FILE *)v7);
    fseek((FILE *)v7, v120, 0);
    fread(pTerrain.pTilemap, 1u, 0x4000u, (FILE *)v7);
    fseek((FILE *)v7, v121, 0);
    fread(pTerrain.pAttributemap, 1u, 0x4000u, (FILE *)v7);
    pTerrain._47C80A(0, 0, 128, 128);
    free(ptr_D4);
    ptr_D4 = 0;
    v9 = malloc(0, 0x8000u, "CMAP");
    v108 = (int)pOMAP;
    ptr_D4 = v9;
    free((void *)v108);
    pOMAP = 0;
    v10 = malloc(0, 0x10000u, "OMAP");
    v108 = 0;
    pOMAP = (unsigned int *)v10;
    fseek((FILE *)v7, v136, v108);
    fread(&uNumTerrainNormals, 4u, 1u, (FILE *)v7);
    fread(pTerrainSomeOtherData, 1u, 0x20000u, (FILE *)v7);
    fread(pTerrainNormalIndices, 1u, 0x10000u, (FILE *)v7);
    pTerrainNormals = (Vec3_float_ *)malloc(pTerrainNormals, 12 * uNumTerrainNormals, "TerNorm");
    fread(pTerrainNormals, 1u, 12 * uNumTerrainNormals, (FILE *)v7);
    v11 = malloc(pBModels, 188 * uNumBModels, "BDdata");
    v108 = 0;
    pBModels = (BSPModel *)v11;
    fseek((FILE *)v7, v122, v108);
    fread(pBModels, 0xBCu, uNumBModels, (FILE *)v7);
    fseek((FILE *)v7, v123, 0);
    pNumItems = 0;
    if ( (signed int)uNumBModels > 0 )
    {
      v12 = 0;
      while ( 1 )
      {
        pBModels[v12].pVertices.pVertices = 0;
        pBModels[v12].pFaces = 0;
        pBModels[v12].pFacesOrdering = 0;
        pBModels[v12].pNodes = 0;
        FileName[0] = 0;
        v108 = (int)&pBModels[v12];
        sprintfex(FileName, "%s", v108);
        v13 = pBModels;
        v138 = 0;
        pBModels[v12].pVertices.pVertices = (Vec3_int_ *)malloc(v13[v12].pVertices.pVertices, 12 * v13[v12].pVertices.uNumVertices,
                                                               FileName);
        pBModels[v12].pFaces = (ODMFace *)malloc(pBModels[v12].pFaces, 308 * pBModels[v12].uNumFaces,
                                                FileName);
        pBModels[v12].pFacesOrdering = (unsigned __int16 *)malloc(pBModels[v12].pFacesOrdering,
                                                                 2 * pBModels[v12].uNumFaces, FileName);
        v14 = malloc(pBModels[v12].pNodes, 8 * pBModels[v12].uNumNodes, FileName);
        v15 = pBModels;
        v108 = (int)File;
        v15[v12].pNodes = (BSPNode *)v14;
        fread(pBModels[v12].pVertices.pVertices, 0xCu, pBModels[v12].pVertices.uNumVertices, (FILE *)v108);
        fread(pBModels[v12].pFaces, 0x134u, pBModels[v12].uNumFaces, (FILE *)File);
        fread(pBModels[v12].pFacesOrdering, 2u, pBModels[v12].uNumFaces, (FILE *)File);
        fread(pBModels[v12].pNodes, 8u, pBModels[v12].uNumNodes, (FILE *)File);
        v16 = malloc(10 * pBModels[v12].uNumFaces);
        v107 = (int)File;
        v17 = pBModels;
        ptr = (FILE *)v16;
        fread(v16, 0xAu, v17[v12].uNumFaces, (FILE *)File);
        v18 = pBModels;
        Str2 = 0;
        if ( (signed int)v18[v12].uNumFaces > 0 )
          break;
LABEL_25:
        free(ptr);
        ++pNumItems;
        ++v12;
        if ( (signed int)pNumItems >= (signed int)uNumBModels )
          goto LABEL_26;
      }
      pDestLen = 0;
      pFilename = (char *)ptr;
      while ( 1 )
      {
        thisa = (int)((char *)v18[v12].pFaces + pDestLen);
        if ( !(*(char *)(thisa + 29) & 0x40) )
          break;
        v19 = pTextureFrameTable->FindTextureByName(pFilename);
        *(short *)(thisa + 272) = v19;
        if ( !v19 )
        {
          v20 = pBitmaps_LOD->LoadTexture(pFilename);
          v21 = thisa;
          *(char *)(v21 + 29) &= 0xBFu;
LABEL_19:
          *(short *)(v21 + 272) = v20;
          v149 = (void *)(v20 != -1 ? &pBitmaps_LOD->pTextures[v20] : 0);
          auto pTex = (Texture *)v149;
          if (pTex)
            pTex->palette_id2 = pPaletteManager->LoadPalette(pTex->palette_id1);
          goto LABEL_20;
        }
        pTextureFrameTable->LoadAnimationSequenceAndPalettes(*(unsigned __int16 *)((char *)&pBModels[v12].pFaces->uTextureID + pDestLen));
LABEL_20:
        if ( *(short *)(thisa + 292) )
        {
          if ( ((ODMFace *)thisa)->HasEventHint() )
            *(char *)(thisa + 30) |= 0x10u;
          else
            *(char *)(thisa + 30) &= 0xEFu;
        }
        ++Str2;
        v18 = pBModels;
        pDestLen += 308;
        pFilename += 10;
        if ( (signed int)Str2 >= (signed int)v18[v12].uNumFaces )
          goto LABEL_25;
      }
      v20 = pBitmaps_LOD->LoadTexture(pFilename);
      v21 = thisa;
      goto LABEL_19;
    }
LABEL_26:
    v22 = File;
    fseek((FILE *)File, v124, 0);
    fread(pSpriteObjects, 0x70u, uNumSpriteObjects, (FILE *)v22);
    if ( (signed int)uNumSpriteObjects > 0 )
    {
      pItems = pSpriteObjects;
      pNumItems = uNumSpriteObjects;
      do
      {
        v24 = pItems->stru_24.uItemID;
        thisa = 0;
        v27 = (ODMFace *)(48 * v24);
        v25 = pObjectList->uNumObjects == 0;
        v26 = (pObjectList->uNumObjects & 0x80000000u) != 0;
        LOWORD(v27) = *(short *)((char *)&v27->pFacePlane.vNormal.x + (int)((char *)&pItemsTable + 24));
        File = v27;
        pItems->uItemType = (unsigned __int16)v27;
        if ( v26 | v25 )
        {
LABEL_33:
          v29 = 0;
        }
        else
        {
          for ( i = (const char *)&pObjectList->pObjects->uObjectID; (short)v27 != *(short *)i; i = pFilename )
          {
            ++thisa;
            pFilename = (char *)i + 56;
            if ( thisa >= (signed int)pObjectList->uNumObjects )
              goto LABEL_33;
            LOWORD(v27) = (short)File;
          }
          v29 = thisa;
        }
        pItems->uObjectDescID = v29;
        ++pItems;
        --pNumItems;
      }
      while ( pNumItems );
    }
    fseek((FILE *)v22, v125, 0);
    fread(pLevelDecorations, 0x20u, uNumLevelDecorations, (FILE *)v22);
    pNumItems = 0;
    if ( (signed int)uNumLevelDecorations > 0 )
    {
      thisa = (int)pLevelDecorations;
      do
      {
        fread(FileName, 1u, 0x20u, (FILE *)v22);
        v30 = pDecorationList->GetDecorIdByName(FileName);
        v31 = thisa;
        ++pNumItems;
        thisa += 32;
        *(short *)v31 = v30;
      }
      while ( (signed int)pNumItems < (signed int)uNumLevelDecorations );
    }
    fseek((FILE *)v22, v126, 0);
    fread(pActors, 0x344u, uNumActors, (FILE *)v22);
    fseek((FILE *)v22, v127, 0);
    fread(pChests, 0x14CCu, uNumChests, (FILE *)v22);
    fseek((FILE *)v22, v128, 0);
    fread(&field_DC, 4u, 1u, (FILE *)v22);
    free(pFaceIDLIST);
    v32 = field_DC;
    pFaceIDLIST = 0;
    v33 = malloc(0, 2 * v32, "IDLIST");
    v108 = (int)v22;
    pFaceIDLIST = (unsigned __int16 *)v33;
    fread(v33, 2u, field_DC, (FILE *)v108);
    fseek((FILE *)v22, v129, 0);
    fread(pOMAP, 4u, 0x4000u, (FILE *)v22);
    fseek((FILE *)v22, v130, 0);
    fread(&uNumSpawnPoints, 4u, 1u, (FILE *)v22);
    pSpawnPoints = (SpawnPointMM7 *)malloc(pSpawnPoints, 24 * uNumSpawnPoints, "Spawn");
    fseek((FILE *)v22, v131, 0);
    fread(pSpawnPoints, 0x18u, uNumSpawnPoints, (FILE *)v22);
    fseek((FILE *)v22, v132, 0);
    fread(&ddm, 0x28u, 1u, (FILE *)v22);
    fseek((FILE *)v22, v133, 0);
    fread(&stru_5E4C90, 1u, 0xC8u, (FILE *)v22);
    fseek((FILE *)v22, v134, 0);
    fread(&uLastVisitDay, 1u, 0x38u, (FILE *)v22);
    fseek((FILE *)v22, v135, 0);
    fread(&uLastVisitDay, 1u, 4u, (FILE *)v22);
    thisa = (int)pTileTypes;
    pTileTable->InitializeTileset(4);
    pTileTable->InitializeTileset(pTileTypes[0].uTileGroup);
    pTileTable->InitializeTileset(pTileTypes[1].uTileGroup);
    pTileTable->InitializeTileset(pTileTypes[2].uTileGroup);
    pTileTable->InitializeTileset(pTileTypes[3].uTileGroup);
    if ( this != (OutdoorLocation *)-96 && pSkyTextureName[0] )
    {
      v108 = 0;
      v107 = (int)pSkyTextureName;
    }
    else
    {
      v108 = 0;
      v107 = (int)pDefaultSkyTexture;
    }
    uSky_TextureID = pBitmaps_LOD->LoadTexture((const char *)v107, (enum TEXTURE_TYPE)v108);
    strcpy(pGroundTileset, byte_6BE124_cfg_textures_DefaultGroundTexture);
    v34 = pTileTable->GetTileById(pTileTypes[0].uTileID);
    v35 = pBitmaps_LOD->LoadTexture(v34->pTileName);
    v36 = uSky_TextureID;
    uMainTile_BitmapID = v35;
    if ( v36 != -1 )
      pBitmaps_LOD->pTextures[v36].palette_id2 = pPaletteManager->LoadPalette(pBitmaps_LOD->pTextures[v36].palette_id1);

    v37 = uMainTile_BitmapID;
    if ( v37 != -1 )
      pBitmaps_LOD->pTextures[v37].palette_id2 = pPaletteManager->LoadPalette(pBitmaps_LOD->pTextures[v37].palette_id1);

    _47F0E2();
    pGameLoadingUI_ProgressBar->Progress();
    fclose((FILE *)v22);
    goto LABEL_150;
  }*/

  assert(sizeof(BSPModel) == 188);

  if (!pGames_LOD->DoesContainerExist(pFilename))
    Error("Unable to find %s in Games.LOD", pFilename);


  char pMinimapTextureFilename[1024];
  strcpy(pMinimapTextureFilename, pFilename);
  pMinimapTextureFilename[strlen(pMinimapTextureFilename) - 4] = 0;
  viewparams->uTextureID_LocationMap = pIcons_LOD->LoadTexture(pMinimapTextureFilename, TEXTURE_16BIT_PALETTE);

  //strcpy(FileName, pContainer);
  strcpy(Str, pFilename);
  strcpy(Str + strlen(Str) - 4, ".odm");
  //v141 = &v139;
  //v38 = strlen(pFilename);
  //strcpy((char *)&v139 + v38, ".odm");
  v39 = pGames_LOD->FindContainer(Str, 1);
  //Str[strlen(Str) - 4] = 0;

  header.uCompressedSize = 0;
  header.uDecompressedSize = 0;
  //ptr = v39;
  header.uVersion = 91969;
  header.pMagic[0] = 'm';
  header.pMagic[1] = 'v';
  header.pMagic[2] = 'i';
  header.pMagic[3] = 'i';
  fread(&header, 0x10u, 1u, v39);
  if (header.uVersion != 91969 ||
      header.pMagic[0] != 'm' ||
      header.pMagic[1] != 'v' ||
      header.pMagic[2] != 'i' ||
      header.pMagic[3] != 'i')
  {
    MessageBoxW(nullptr, L"Can't load file!",
                L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Odmap.cpp:507", 0);
  }
  //v40 = header.uCompressedSize;
  //pSource = header.uDecompressedSize;
  //v41 = malloc(header.uDecompressedSize);
  auto pSrcMem = (unsigned char *)malloc(header.uDecompressedSize);
  auto pSrc = pSrcMem;
  //v42 = v41;
  //HIDWORD(v142) = (uint32)pSrc;
  if (header.uCompressedSize < header.uDecompressedSize)
  {
    auto pComressedSrc = (char *)malloc(header.uCompressedSize);
    fread(pComressedSrc, header.uCompressedSize, 1, v39);

    uint actualDecompressedSize = header.uDecompressedSize;
    zlib::MemUnzip(pSrc, &actualDecompressedSize, pComressedSrc, header.uCompressedSize);
    free(pComressedSrc);
  }
  else
  {
    fread(pSrc, header.uDecompressedSize, 1, v39);
  }

  memcpy(pLevelFilename, pSrc, 0x20);
  memcpy(pLocationFileName, pSrc + 0x20, 0x20);
  memcpy(pLocationFileDescription, pSrc + 0x40, 0x20);
  memcpy(pSkyTextureName, pSrc + 3 * 32, 32);
  memcpy(pGroundTileset, pSrc + 0x80, 0x20);
  memcpy(pTileTypes, pSrc + 0xA0, 0x10);
  pSrc += 0xB0;

  //v43 = (char *)pSrc + 176;
  LoadTileGroupIds();
  LoadRoadTileset();
  strcpy(pGroundTileset, "grastyl");
  pGameLoadingUI_ProgressBar->Progress();
  pTerrain.Initialize();
  //v108 = 16384;
  //v107 = (int)v43;
  //v106 = (char *)pTerrain.pHeightmap;
  memcpy(pTerrain.pHeightmap, pSrc, 0x4000);
  pSrc += 0x4000;

  //v43 = (char *)v43 + 16384;
  //v105 = 16384;
  //v104 = (void *)v43;
  //v103 = pTerrain.pTilemap;
  memcpy(pTerrain.pTilemap, pSrc, 0x4000);
  pSrc += 0x4000;

  //v43 = (char *)v43 + 16384;
  memcpy(pTerrain.pAttributemap, pSrc, 0x4000);
  pSrc += 0x4000;

  //v43 = (char *)v43 + 16384;
  //v108 = (int)ptr_D4;
  if (pCmap)
  {
    free(pCmap);
    pCmap = nullptr;
  }
  pCmap = malloc(0x8000);
  pTerrain.FillDMap(0, 0, 128, 128);

  pGameLoadingUI_ProgressBar->Progress();
  memcpy(&uNumTerrainNormals, pSrc, 4);
  //v43 = (char *)v43 + 4;
  memcpy(pTerrainSomeOtherData.data(), pSrc + 4, 0x20000);
  pSrc += 4 + 0x20000;
  //v43 = (char *)v43 + 131072;
  memcpy(pTerrainNormalIndices.data(), pSrc, 0x10000);
  pSrc += 0x10000;
  //v43 = (char *)v43 + 65536;

  //pFilename = (char *)(12 * uNumTerrainNormals);
  pTerrainNormals = (Vec3_float_ *)malloc(sizeof(Vec3_float_) * uNumTerrainNormals);
  memcpy(pTerrainNormals, pSrc, 12 * uNumTerrainNormals);
  pSrc += 12 * uNumTerrainNormals;
  //v44 = (char *)v43 + (int)pFilename;
  //v44 = (char *)v44 + 4;
  //v45 = uNumBModels;
  //v108 = (int)"BDdata";

  pGameLoadingUI_ProgressBar->Progress();

  //v107 = 188 * v45;
  //v106 = (char *)pBModels;
  //v46 = (BSPModel *)malloc(v106, 188 * v45, "BDdata");
  //v47 = uNumBModels;
  memcpy(&uNumBModels, pSrc, 4);
  pBModels = (BSPModel *)malloc(188 * uNumBModels);
  //pFilename = (char *)(188 * v47);
  memcpy(pBModels, pSrc + 4, 188 * uNumBModels);
  pSrc += 4 + 188 * uNumBModels;
  
  pGameLoadingUI_ProgressBar->Progress();

  //uSourceLen = (char *)v44 + (int)pFilename;
  //v151 = 0;
  for (uint i = 0; i < uNumBModels; ++i)
  {
    //v48 = 0;
    auto model = &pBModels[i];

    model->pVertices.pVertices = nullptr;
    model->pFaces = nullptr;
    model->pFacesOrdering = nullptr;
    model->pNodes = nullptr;
      //FileName[0] = 0;
      //v108 = (int)&pBModels[i];
      //sprintf(FileName, "%s", v108);
      //v49 = pBModels;
      //v138 = 0;
      //v50 = &pBModels[v48];
      //v108 = (int)FileName;
      //v107 = 12 * v50->pVertices.uNumVertices;
      //v106 = (char *)v50->pVertices.pVertices;
    assert(sizeof(Vec3_int_) == 12);
    uint verticesSize = model->pVertices.uNumVertices * sizeof(Vec3_int_);
    model->pVertices.pVertices = (Vec3_int_ *)malloc(verticesSize);
    memcpy(model->pVertices.pVertices, pSrc, verticesSize);
    pSrc += verticesSize;
      //v51 = &pBModels[v48];
      //v108 = (int)FileName;
      //v107 = 308 * v51->uNumFaces;
      //v106 = (char *)v51->pFaces;
    assert(sizeof(ODMFace) == 308);
    uint facesSize = model->uNumFaces * sizeof(ODMFace);
    model->pFaces = (ODMFace *)malloc(facesSize);
    memcpy(model->pFaces, pSrc, facesSize);
    pSrc += facesSize;
      //v52 = &pBModels[v48];
      //v108 = (int)FileName;
      //v107 = 2 * v52->uNumFaces;
      //v106 = (char *)v52->pFacesOrdering;
    uint facesOrderingSize = model->uNumFaces * sizeof(short);
    model->pFacesOrdering = (unsigned __int16 *)malloc(facesOrderingSize);
    memcpy(model->pFacesOrdering, pSrc, facesOrderingSize);
    pSrc += facesOrderingSize;
      //v53 = &pBModels[v48];
      //v108 = (int)FileName;
      //v107 = 8 * v53->uNumNodes;
      //v106 = (char *)v53->pNodes;
    assert(sizeof(BSPNode) == 8);
    uint nodesSize = model->uNumNodes * sizeof(BSPNode);
    model->pNodes = (BSPNode *)malloc(nodesSize);
    memcpy(model->pNodes, pSrc, nodesSize);
    pSrc += nodesSize;
      //v54 = &pBModels[v48];
      //v108 = 12 * v54->pVertices.uNumVertices;
      //pFilename = (char *)v108;
      //v107 = (int)uSourceLen;
      //v106 = (char *)v54->pVertices.pVertices;
      //memcpy(v106, uSourceLen, v108);
      //uSourceLen = (char *)uSourceLen + (int)pFilename;
      //v55 = &pBModels[v48];
      //v105 = 308 * v55->uNumFaces;
      //v104 = uSourceLen;
      //v103 = v55->pFaces;
      //pFilename = (char *)v105;
      //memcpy(v103, uSourceLen, v105);
      //v56 = &pBModels[v48];
      //uSourceLen = (char *)uSourceLen + (int)pFilename;
      //v57 = v56->pFacesOrdering;
      //pFilename = (char *)(2 * v56->uNumFaces);
      //memcpy(v57, uSourceLen, (size_t)pFilename);
      //v58 = &pBModels[v48];
      //uSourceLen = (char *)uSourceLen + (int)pFilename;
      //v59 = v58->pNodes;
      //pFilename = (char *)(8 * v58->uNumNodes);
      //memcpy(v59, uSourceLen, (size_t)pFilename);
      //uSourceLen = (char *)uSourceLen + (int)pFilename;
    //ptr = (FILE *)malloc(10 * model->uNumFaces);
    auto textureFilenames = (const char *)malloc(10 * model->uNumFaces);
      //pFilename = (char *)(10 * pBModels[v48].uNumFaces);
    memcpy((char *)textureFilenames, pSrc, 10 * model->uNumFaces);
    pSrc += 10 * model->uNumFaces;
      //v144 = 0;
      //uSourceLen = (char *)uSourceLen + (int)pFilename;
      //v60 = pBModels;
    for (uint j = 0; j < model->uNumFaces; ++j)
    {
      auto texFilename = textureFilenames + j * 10;
    //v149 = 0;
    //Str2 = (char *)ptr;

      auto *face = &model->pFaces[j];
      //pFilename = (char *)v149 + (unsigned int)v60[v48].pFaces;
      if (~face->uAttributes & FACE_DONT_CACHE_TEXTURE)
      {
        v62 = pBitmaps_LOD->LoadTexture(texFilename);
//        v63 = (ODMFace *)pFilename;
        goto LABEL_68;
      }
      //v61 = pTextureFrameTable->FindTextureByName(texFilename);
      face->uTextureID = pTextureFrameTable->FindTextureByName(texFilename);
      if (!face->uTextureID)
      {
        v62 = pBitmaps_LOD->LoadTexture(texFilename);
        //v63 = (ODMFace *)pFilename;
        face->uAttributes &= ~FACE_DONT_CACHE_TEXTURE;
LABEL_68:
        face->uTextureID = v62;
        //v145 = (signed __int16)v62 != -1 ? &pBitmaps_LOD->pTextures[(signed __int16)v62] : 0;
        //v108 = ((signed __int16)v62 != -1 ? pBitmaps_LOD->pTextures[(signed __int16)v62].palette_id1 : 36);
        if ((signed __int16)v62 != -1)
          pBitmaps_LOD->pTextures[v62].palette_id2 = pPaletteManager->LoadPalette(pBitmaps_LOD->pTextures[v62].palette_id1);
        goto LABEL_69;
      }
      pTextureFrameTable->LoadAnimationSequenceAndPalettes(face->uTextureID);
LABEL_69:
      if (face->sCogTriggeredID)
      {
        if (face->HasEventHint())
          face->uAttributes |= 0x1000;
        else
          face->uAttributes &= ~0x1000;
      }
      //++v144;
      //v60 = pBModels;
      //v149 = (char *)v149 + 308;
      //Str2 += 10;
      //if ( v144 >= (signed int)v60[v48].uNumFaces )
        //goto LABEL_74;
      }

//LABEL_74:
      free((void *)textureFilenames);
//      ++v151;
//      ++v48;
//      if ( v151 >= (signed int)uNumBModels )
//        goto LABEL_75;
  }
//LABEL_75:
  pGameLoadingUI_ProgressBar->Progress();

  memcpy(&uNumLevelDecorations, pSrc, 4u);
  //uSourceLen = (char *)uSourceLen + 4;
  if (uNumLevelDecorations > 3000)
    MessageBoxW(nullptr, L"Can't load file!",
                L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Odmap.cpp:678", 0);

  assert(sizeof(LevelDecoration) == 32);
  //pFilename = (char *)(32 * uNumLevelDecorations);
  memcpy(pLevelDecorations.data(), pSrc + 4, uNumLevelDecorations * sizeof(LevelDecoration));
  pSrc += 4 + sizeof(LevelDecoration) * uNumLevelDecorations;

  pGameLoadingUI_ProgressBar->Progress();

  //v151 = 0;
  //uSourceLen = (char *)uSourceLen + (int)pFilename;
  for (uint i = 0; i < uNumLevelDecorations; ++i)
  {
    char name[256];
    memcpy(name, pSrc, sizeof(LevelDecoration));
    pSrc += sizeof(LevelDecoration);

    pLevelDecorations[i].uDecorationDescID = pDecorationList->GetDecorIdByName(name);
  }

  pGameLoadingUI_ProgressBar->Progress();

  memcpy(&numFaceIDListElems, pSrc, 4);

  //uSourceLen = (char *)uSourceLen + 4;
  //v108 = (int)pFaceIDLIST;
  if (pFaceIDLIST)
  {
    free(pFaceIDLIST);
    pFaceIDLIST = nullptr;
  }
  //v66 = field_DC;
  //pFaceIDLIST = 0;
  //v67 = malloc(0, 2 * v66, "IDLIST");
  uint faceIDListSize = 2 * numFaceIDListElems;
  pFaceIDLIST = (unsigned short *)malloc(faceIDListSize);
  //v68 = field_DC;
  //pFaceIDLIST = (unsigned __int16 *)v67;
  //pFilename = (char *)(2 * v68);
  memcpy(pFaceIDLIST, pSrc + 4, faceIDListSize);
  pSrc += 4 + faceIDListSize;

  //uSourceLen = (char *)uSourceLen + (int)pFilename;
  pGameLoadingUI_ProgressBar->Progress();

  //v108 = (int)pOMAP;
  //free((void *)v108);
  //pOMAP = 0;
  if (pOMAP)
  {
    free(pOMAP);
    pOMAP = nullptr;
  }
  //v69 = malloc(0, 0x10000u, "OMAP");
  pOMAP = (unsigned int *)malloc(0x10000);
  //v108 = 65536;
  //pOMAP = (unsigned int *)v69;
  memcpy(pOMAP, pSrc, 65536);
  pSrc += 65536;

  //uSourceLen = (char *)uSourceLen + 65536;
  pGameLoadingUI_ProgressBar->Progress();

  memcpy(&uNumSpawnPoints, pSrc, 4);
  //uSourceLen = (char *)uSourceLen + 4;
  pGameLoadingUI_ProgressBar->Progress();
  //v70 = uNumSpawnPoints;
  //v108 = (int)"Spawn";
  //v107 = 24 * v70;
  //v106 = (char *)pSpawnPoints;
  assert(sizeof(SpawnPointMM7) == 24);
  uint spawnPointsSize = uNumSpawnPoints * sizeof(SpawnPointMM7);
  pSpawnPoints = (SpawnPointMM7 *)malloc(spawnPointsSize);
  //v72 = uNumSpawnPoints;
  //pSpawnPoints = v71;
  memcpy(pSpawnPoints, pSrc + 4, spawnPointsSize);
  pSrc += 4 + spawnPointsSize;

  pGameLoadingUI_ProgressBar->Progress();

  free(pSrcMem);

  //v108 = (int)".ddm";
  //v73 = strlen(pContainer);
  strcpy(Str + strlen(Str) - 4, ".ddm");
  //strcpy((char *)v141 + v73, (const char *)v108);
  v39 = pNew_LOD->FindContainer(Str, 1);//
  fread(&header, 0x10u, 1u, v39);
  Str2 = 0;
  if (header.uVersion != 91969 ||
      header.pMagic[0] != 'm' ||
      header.pMagic[1] != 'v' ||
      header.pMagic[2] != 'i' ||
      header.pMagic[3] != 'i' )
  {
    MessageBoxW(nullptr, L"Can't load file!", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Odmap.cpp:746", 0);
    Str2 = (char *)1;
  }
  //v74 = 0;
  //pFilename = (char *)header.uCompressedSize;
  //v149 = 0;
  //pDestLen = header.uDecompressedSize;
  if ( !Str2 )
  {
    pSrcMem = (unsigned char *)malloc(header.uDecompressedSize);
    pSrc = pSrcMem;
    //v149 = v75;
    if (header.uCompressedSize == header.uDecompressedSize)
      fread(pSrc, header.uDecompressedSize, 1u, v39);
    else if (header.uCompressedSize < header.uDecompressedSize)
    {
      auto compressedMem = malloc(header.uCompressedSize);
      fread(compressedMem, header.uCompressedSize, 1, v39);

      uint actualDecompressedSize = header.uDecompressedSize;
      zlib::MemUnzip(pSrc, &actualDecompressedSize, compressedMem, header.uCompressedSize);
      free(compressedMem);
    }
    else
      MessageBoxW(nullptr, L"Can't load file!",
                  L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Odmap.cpp:765", 0);

    assert(sizeof(DDM_DLV_Header) == 0x28);
    memcpy(&ddm, pSrc, sizeof(DDM_DLV_Header));
    pSrc += sizeof(DDM_DLV_Header);
    //v74 = (int)((char *)v75 + 40);
  }
  //v76 = uNumBModels;
  //v77 = actualNumFacesInLevel = 0;
  uint actualNumFacesInLevel = 0;
  for (uint i = 0; i < uNumBModels; ++i)
    actualNumFacesInLevel += pBModels[i].uNumFaces;

  //v79 = ddm.uNumFacesInBModels;
  if (ddm.uNumFacesInBModels)
  {
    if ( ddm.uNumBModels )
    {
      //v80 = ddm.uNumDecorations;
      if (ddm.uNumDecorations)
      {
        if (ddm.uNumFacesInBModels != actualNumFacesInLevel ||
            ddm.uNumBModels != uNumBModels ||
            ddm.uNumDecorations != uNumLevelDecorations )
          Str2 = (char *)1;
      }
    }
  }

  if ( BYTE1(dword_6BE364_game_settings_1) & 0x20 )
    pNumItems = 29030400;
  if ( Str2 )
  {
    memset(Dst, 0, 0x3C8u);
    memset(Src, 0, 0x3C8u);
    goto LABEL_112;
  }
  //v81 = ddm.uLastRepawnDay;
  if ((unsigned int)((char *)File - ddm.uLastRepawnDay) >= pNumItems || !ddm.uLastRepawnDay)
  {
    memcpy(Dst, pSrc, 0x3C8u);
    memcpy(Src, pSrc + 968, 0x3C8u);
LABEL_112:
    free(pSrcMem);

    ddm.uLastRepawnDay = (int)File;
    if (Str2 == 0)
      ++ddm.uNumRespawns;
    v108 = 0;
    *(int *)thisa = 1;
    v39 = pGames_LOD->FindContainer(Str, 0);
    fread(&header, 0x10, 1u, v39);
    //pFilename = (char *)header.uCompressedSize;
    //pDestLen = header.uDecompressedSize;
    //v82 = malloc(header.uDecompressedSize);
    pSrcMem = (unsigned char *)malloc(header.uDecompressedSize);
    //v149 = v82;
    if (header.uCompressedSize == header.uDecompressedSize)
      fread(pSrcMem, header.uDecompressedSize, 1, v39);
    else if (header.uCompressedSize < header.uDecompressedSize)
    {
      auto compressedMem = malloc(header.uCompressedSize);
      fread(compressedMem, header.uCompressedSize, 1u, v39);

      uint actualDecompressedSize = header.uDecompressedSize;
      zlib::MemUnzip(pSrcMem, &actualDecompressedSize, compressedMem, header.uCompressedSize);
      free(compressedMem);
    }
    else
      MessageBoxW(nullptr, L"Can't load file!", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Odmap.cpp:857", 0);

    pSrc = pSrcMem + 40;
    goto LABEL_120;

  }
  *(int *)thisa = 0;
LABEL_120:
  //v108 = (int)".odm";
  //v83 = strlen(pContainer);
  //strcpy((char *)v141 + v83, (const char *)v108);
  memcpy(uUndiscoveredArea, pSrc, 0x3C8);
  //v84 = (const void *)(v74 + 968);
  memcpy(uDicovered_area, pSrc + 0x3C8, 0x3C8);
  pSrc += 2 * 0x3C8;
  //v85 = (char *)v84 + 968;

  pGameLoadingUI_ProgressBar->Progress();

  if ( *(int *)thisa )
  {
    memcpy(uUndiscoveredArea, Dst, 0x3C8u);
    memcpy(uDicovered_area, Src, 0x3C8u);
  }

  //v25 = uNumBModels == 0;
  //v26 = (uNumBModels & 0x80000000u) != 0;
  //v151 = 0;
  for (uint i = 0; i < uNumBModels; ++i)
  {
    auto model = pBModels[i];
    //pNumItems = 0;
    //do
    //{
      //v86 = pBModels;
      //thisa = 0;
      //v87 = (unsigned int)((char *)v86 + pNumItems);
    for (uint j = 0; j < model.uNumFaces; ++j)
    {
      auto face = model.pFaces[j];

      //if ( *(int *)(v87 + 76) > 0 )
      //{
        //File = 0;
        //do
        //{
          //v106 = (char *)&File->uAttributes + *(int *)(v87 + 84);
      memcpy(&face.uAttributes, pSrc, 4);
      pSrc += 4;
          //v88 = pBModels;
          //++File;
          //v85 = (char *)v85 + 4;
          //++thisa;
          //v87 = (unsigned int)((char *)v88 + pNumItems);
        //}
        //while ( thisa < *(int *)(v87 + 76) );
    }

      //v89 = pBModels;
      //thisa = 0;
    for (uint j = 0; j < model.uNumFaces; ++j)
    {
      auto face = model.pFaces[j];
        //pFilename = 0;
        //do
        //{
          //v90 = (ODMFace *)&pFilename[*(unsigned int *)((char *)&v89->pFaces + pNumItems)];
          //File = v90;
      if (face.sCogTriggeredID)
      {
        if (face.HasEventHint())
          face.uAttributes |= 0x1000u;
        else
          face.uAttributes &= 0xFFFFEFFFu;
      }
          //++thisa;
          //v89 = pBModels;
          //pFilename += 308;
        //}
        //while ( thisa < *(signed int *)((char *)&v89->uNumFaces + pNumItems) );
    }
      //++v151;
      //pNumItems += 188;
    //}
    //while ( v151 < (signed int)uNumBModels );
  }

  pGameLoadingUI_ProgressBar->Progress();

  //v151 = 0;
  for (uint i = 0; i < uNumLevelDecorations; ++i)
  {
    //thisa = (int)&pLevelDecorations[0].field_2;
    //do
    //{
    memcpy(&pLevelDecorations[i].uFlags, pSrc, 2);
    pSrc += 2;
      //thisa += 32;
      //v85 = (char *)v85 + 2;
      //++v151;
    //}
    //while ( v151 < (signed int)uNumLevelDecorations );
  }

  pGameLoadingUI_ProgressBar->Progress();

  memcpy(&uNumActors, pSrc, 4);
  //v91 = (char *)v85 + 4;
  if (uNumActors > 500)
    MessageBoxW(nullptr, L"Can't load file!",
                L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Odmap.cpp:939", 0);

  pGameLoadingUI_ProgressBar->Progress();

  assert(sizeof(Actor) == 836);
  //pFilename = (char *)(836 * uNumActors);
  memcpy(pActors.data(), pSrc + 4, uNumActors * sizeof(Actor));
  pSrc += 4 + uNumActors * sizeof(Actor);
  //v92 = (char *)v91 + (int)pFilename;
  pGameLoadingUI_ProgressBar->Progress();

  memcpy(&uNumSpriteObjects, pSrc, 4);
  assert(uNumSpriteObjects <= 1000 && "Too many objects");
  assert(sizeof(SpriteObject) == 112);

  pGameLoadingUI_ProgressBar->Progress();

  //pFilename = (char *)(112 * uNumSpriteObjects);
  memcpy(pSpriteObjects.data(), pSrc + 4, uNumSpriteObjects * sizeof(SpriteObject));
  pSrc += 4 + uNumSpriteObjects * sizeof(SpriteObject);

  //v94 = (char *)v93 + (int)pFilename;
  pGameLoadingUI_ProgressBar->Progress();

  memcpy(&uNumChests, pSrc, 4);
  //v95 = (char *)v94 + 4;
  if (uNumChests > 20)
    MessageBoxW(nullptr, L"Can't load file!",
                L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Odmap.cpp:968", 0);

  pGameLoadingUI_ProgressBar->Progress();

  assert(sizeof(Chest) == 5324);
  //pFilename = (char *)(5324 * uNumChests);
  memcpy(pChests.data(), pSrc + 4 , uNumChests * sizeof(Chest));
  pSrc += 4 + uNumChests * sizeof(Chest);
  //v96 = (char *)v95 + (int)pFilename;
  pGameLoadingUI_ProgressBar->Progress();

  memcpy(&stru_5E4C90_MapPersistVars, pSrc, 0xC8);
  pSrc += 0xC8;

  pGameLoadingUI_ProgressBar->Progress();
  memcpy(&loc_time, pSrc, 0x38u);

  free(pSrcMem);

  pTileTable->InitializeTileset(Tileset_Dirt);
  pTileTable->InitializeTileset(Tileset_Snow);
  //v108 = pTileTypes[0].uTileGroup;
  pTileTable->InitializeTileset(pTileTypes[0].tileset);
  //v108 = pTileTypes[1].uTileGroup;
  pTileTable->InitializeTileset(pTileTypes[1].tileset);
  //v108 = pTileTypes[2].uTileGroup;
  pTileTable->InitializeTileset(pTileTypes[2].tileset);
  //v108 = pTileTypes[3].uTileGroup;
  pTileTable->InitializeTileset(pTileTypes[3].tileset);
  strcpy(pGroundTileset, byte_6BE124_cfg_textures_DefaultGroundTexture.data());
  //v97 = pTileTypes[0].uTileID;
  //v108 = 0;
  auto v98 = pTileTable->GetTileById(pTileTypes[0].uTileID);
  //v99 = pBitmaps_LOD->LoadTexture(v98->pTileName, TEXTURE_DEFAULT);
  uMainTile_BitmapID = pBitmaps_LOD->LoadTexture(v98->pTileName, TEXTURE_DEFAULT);
  if (uMainTile_BitmapID != -1)
    pBitmaps_LOD->pTextures[uMainTile_BitmapID].palette_id2 = pPaletteManager->LoadPalette(pBitmaps_LOD->pTextures[uMainTile_BitmapID].palette_id1);

  _47F0E2();

LABEL_150:
  //v100 = HIDWORD(uLastVisitDay);
  //LODWORD(v142) = LODWORD(uLastVisitDay);
  //HIDWORD(v142) = v100;
  if (loc_time.uLastVisitDay)
  {
    if ( (signed int)((signed int)(signed __int64)((double)loc_time.uLastVisitDay * 0.234375) / 60 / 60 / 24) % 28 != pParty->uDaysPlayed )
    {
      if ( rand() % 100 >= 20 )
        v108 = dword_4EC268[rand() % dword_4EC2A8];
      else
        v108 = dword_4EC28C[rand() % dword_4EC2AC];
      sprintf(loc_time.sky_texture_name, "plansky%d", v108);
    }
  }
  else
  {
    strcpy(loc_time.sky_texture_name, "plansky3");
  }
  //v101 = pBitmaps_LOD->LoadTexture(field_4F8);
  uSky_TextureID = pBitmaps_LOD->LoadTexture(loc_time.sky_texture_name);
  if (uSky_TextureID != -1)
    pBitmaps_LOD->pTextures[uSky_TextureID].palette_id2 = pPaletteManager->LoadPalette(pBitmaps_LOD->pTextures[uSky_TextureID].palette_id1);

  pPaletteManager->RecalculateAll();
  pSoundList->LoadSound(53, 0);
  pSoundList->LoadSound(92, 0);
  pSoundList->LoadSound(57, 0);
  pSoundList->LoadSound(96, 0);
  pSoundList->LoadSound(64, 0);
  pSoundList->LoadSound(103, 0);
  for (int i=0; i<3;++i) 
  {
    
      switch ( pTileTypes[i].tileset )
      {
        case Tileset_Grass:
            pSoundList->LoadSound(54, 0);
            pSoundList->LoadSound(93, 0);
            break;
        case Tileset_Snow:
          pSoundList->LoadSound(58, 0);
          pSoundList->LoadSound(97, 0);
          break;
        case Tileset_2:
          pSoundList->LoadSound(52, 0);
          pSoundList->LoadSound(91, 0);
          break;
        case Tileset_3:
          pSoundList->LoadSound(51, 0);
          pSoundList->LoadSound(90, 0);
          break;
        case Tileset_Water:
          pSoundList->LoadSound(62, 0);
          pSoundList->LoadSound(101, 0);
          break;
        case Tileset_6:
          pSoundList->LoadSound(49, 0);
          pSoundList->LoadSound(88, 0);
          break;
        case Tileset_Swamp:
          pSoundList->LoadSound(61, 0);
          pSoundList->LoadSound(100, 0);
          break;
      }   
  }
  return true;
}

//----- (0047ECC1) --------------------------------------------------------
int OutdoorLocation::GetTileIdByTileMapId(signed int a2)
{
  signed int result; // eax@2
  int v3; // eax@3

  if ( a2 >= 90 )
  {
    v3 = (a2 - 90) / 36;
    if ( v3 && v3 != 1 && v3 != 2 )
    {
      if ( v3 == 3 )
        result = this->pTileTypes[3].uTileID;
      else
        result = a2;
    }
    else
    {
      result = this->pTileTypes[v3].uTileID;
    }
  }
  else
  {
    result = 0;
  }
  return result;
}

//----- (0047ED08) --------------------------------------------------------
unsigned int OutdoorLocation::DoGetTileTexture(unsigned int uX, unsigned int uY)
{
  int v3; // esi@5
  unsigned int result; // eax@9

  assert(uX < 128 && uY < 128);

 v3 = this->pTerrain.pTilemap[uY * 128 + uX];
 if (v3 < 198) // < Tileset_3
 {
  if (v3 >= 90)
    v3 = v3 + this->pTileTypes[(v3 - 90) / 36].uTileID - 36 * ((v3 - 90) / 36) - 90;
 }
 else
   v3 = v3 + this->pTileTypes[3].uTileID - 198;

  #pragma region "New: seasons change"
  extern bool change_seasons;
  if (change_seasons)
    switch (pParty->uCurrentMonth)
    {
      case 11: case 0: case 1: // winter
        if (v3 >= 90) // Tileset_Grass begins at TileID = 90
        {
          if (v3 <= 95) // some grastyl entries
            v3 = 348;
          else if (v3 <= 113)  // rest of grastyl & all grdrt*
            v3 = 348 + (v3 - 96);
        }
      /*switch (v3)
      {
        case 102: v3 = 354; break;  // grdrtNE -> SNdrtne
        case 104: v3 = 356; break;  // grdrtNW -> SNdrtnw
        case 108: v3 = 360; break;  // grdrtN  -> SNdrtn
      }*/
      break;

      case 2: case 3: case 4: // spring
      case 8: case 9: case 10: // autumn
        if (v3 >= 90 && v3 <= 113) // just convert all Tileset_Grass to dirt
          v3 = 1;
      break;

      case 5: case 6: case 7: // summer
        //all tiles are green grass by default
      break;

      default: assert(pParty->uCurrentMonth >= 0 && pParty->uCurrentMonth < 12);
    }
  #pragma endregion

 return pTileTable->pTiles[v3].uBitmapID;
}

//----- (0047ED83) --------------------------------------------------------
int OutdoorLocation::_47ED83(signed int a2, signed int a3)
{
  assert(a2 < 128 && a3 < 128);

  return *(&this->pTerrain.pTilemap[128 * a3] + a2);
}

//----- (0047EDB3) --------------------------------------------------------
int OutdoorLocation::ActuallyGetSomeOtherTileInfo(unsigned int uX, unsigned int uY)
{
  assert(uX < 128 && uY < 128);
  int v3; // esi@5

  v3 = this->pTerrain.pTilemap[uY * 128 + uX];
  if ( v3 >= 90 )
    v3 = v3 + this->pTileTypes[(v3 - 90) / 36].uTileID - 36 * ((v3 - 90) / 36) - 90;
  return pTileTable->pTiles[v3].uAttributes;
}

//----- (0047EE16) --------------------------------------------------------
int OutdoorLocation::DoGetHeightOnTerrain(unsigned int uX, unsigned int uZ)
{
  assert(uX < 128 && uZ < 128);

  return 32 * pTerrain.pHeightmap[uZ * 128 + uX];
}

//----- (0047EE49) --------------------------------------------------------
int OutdoorLocation::GetSoundIdByPosition( signed int X_pos, signed int Y_pos, int running )
    {
  signed int v4; // eax@5
  signed int v5; // eax@7
  int v6; // eax@8
  signed int v8; // eax@9
  int modif=0;

  if ( X_pos < 0 || X_pos > 127 || Y_pos < 0 || Y_pos > 127 )
    v4 = 0;
  else
    v4 = this->pTerrain.pTilemap[128 * Y_pos + X_pos];
  v5 = GetTileIdByTileMapId(v4);
  if (running)
      modif=-39;
  if ( !v5 )
    return 92+modif;
   
  switch (pTileTable->pTiles[v5].tileset)
  {
  case 0:  return 93+ modif; 
  case 1:  return 97+ modif; 
  case 2:  return 91+ modif;
  case 3:  return 90+ modif;
  case 4:  return 101+ modif;
  case 5:  return 95+ modif;
  case 6:  return 88+ modif;
  case 7: return 100+ modif;
  case 8: return 93+ modif;
 default:
     v8=pTileTable->pTiles[v5].tileset;
     if ( (v8 > 9 && v8 <= 17) || (v8 > 21 && v8 <= 27) )
        return 96+ modif;
     else
        return 95+ modif;
  }

}

//----- (0047EF60) --------------------------------------------------------
int OutdoorLocation::UpdateDiscoveredArea(int X_grid_pos, int Y_grid_poa, int a4)
{
  int v4; // ecx@1
  int v5; // edx@2
  int v6; // edi@2
  int v7; // esi@2
  int v8; // eax@4
  int v9; // ebx@4
  int v10; // eax@5
  int v11; // eax@7
  int v12; // esi@12
  char v13; // al@12
  int v15; // [sp+4h] [bp-14h]@7
  int v16; // [sp+8h] [bp-10h]@5
  int v17; // [sp+Ch] [bp-Ch]@2
  OutdoorLocation *v18; // [sp+10h] [bp-8h]@1
  int v19; // [sp+14h] [bp-4h]@2
  int i; // [sp+24h] [bp+Ch]@5

  v18 = this;
  v4 = Y_grid_poa - 10;
  if ( Y_grid_poa - 10 < Y_grid_poa + 10 )
  {
    v5 = v4 - Y_grid_poa;
    v6 = (Y_grid_poa - 30);
    v7 = X_grid_pos;
    v19 = v4 - Y_grid_poa;
    v17 = Y_grid_poa + 10 - v4;
    while ( 1 )
    {
      v8 = abs(v5);
      v9 = v7 - 10;
      if ( v7 - 10 < v7 + 10 )
      {
        v16 = v8 * v8;
        v10 = v9 - v7;
        for ( i = v9 - v7; ; v10 = i )
        {
          v11 = abs(v10);
          v15 = v11 * v11 + v16;
          if ( v15 <= 100 && v9 >= 20 && v9 <= 107 && v6 >= 0 && v6 <= 87 )
          {
            v13 = 1 << (7 - (v9 - 20) % 8);
            v18->uDicovered_area[v6][(v9 - 20)/8] |= v13;
            if ( v15 <= 49 )
              v18->uUndiscoveredArea[v6][(v9 - 20)/8] |= v13;
            v7 = X_grid_pos;
          }
          ++v9;
          ++i;
          if ( v9 >= v7 + 10 )
            break;
        }
      }
      ++v6 ;
      ++v19;
      --v17;
      if ( !v17 )
        break;
      v5 = v19;
    }
  }
  return 1;
}

//----- (0047F04C) --------------------------------------------------------
bool OutdoorLocation::_47F04C(signed int x_pos, signed int y_pos)
{
  bool result; // eax@5

  if ( x_pos < 0 || x_pos >= 88 || y_pos < 0 || y_pos >= 88 )
    result = 0;
  else
    result = (uUndiscoveredArea[y_pos][x_pos/8] & (unsigned __int8)(1 << (7 - (x_pos) % 8))) != 0;
  return result;
}

//----- (0047F097) --------------------------------------------------------
bool OutdoorLocation::_47F097(signed int x_pos, signed int y_pos)
{
  bool result; // eax@5

  if ( x_pos < 0 || x_pos >= 88 || y_pos < 0 || y_pos >= 88 )
    result = 0;
  else
    result = (uDicovered_area[y_pos][x_pos/8] & (unsigned __int8)(1 << (7 - (x_pos) % 8))) != 0;
  return result;
}

//----- (0047F0E2) --------------------------------------------------------
bool OutdoorLocation::_47F0E2()
{
  signed int v1; // edi@1
  OutdoorLocation *v2; // ebx@1
  Texture *v3; // ebp@2

  v1 = 0;
  v2 = this;
  if ( (signed int)pBitmaps_LOD->uNumLoadedFiles > 0 )
  {
    v3 = pBitmaps_LOD->pTextures;
    do
    {
      if ( v1 != -1 ? (int)v3 : 0 )
        v3->uDecompressedSize = v2->pTerrain._47CB57((int)v3->pLevelOfDetail0_prolly_alpha_mask,
                                                      v3->palette_id2,
                                                      v3->uTextureWidth * v3->uTextureHeight);
      ++v1;
      ++v3;
    }
    while ( v1 < (signed int)pBitmaps_LOD->uNumLoadedFiles );
  }
  return 1;
}

//----- (0047F138) --------------------------------------------------------
bool OutdoorLocation::PrepareDecorations()
{
  signed int v1; // ebx@1
  __int16 v4; // ax@5
  int v5; // eax@7
  __int16 v6; // ax@14
  signed int v8; // [sp+Ch] [bp-4h]@1

  v1 = 0;
  v8 = 0;
  if ( !_stricmp(pCurrentMapName, "out09.odm") )
    v8 = 1;

  for (uint i = 0; i < uNumLevelDecorations; ++i)
  {
    auto decor = &pLevelDecorations[i];

      pDecorationList->InitializeDecorationSprite(decor->uDecorationDescID);
      v4 = pDecorationList->pDecorations[decor->uDecorationDescID].uSoundID;
      if ( v4 && _6807E0_num_decorations_with_sounds_6807B8 < 9 )
      {
        pSoundList->LoadSound(v4, 0);
        v5 = _6807E0_num_decorations_with_sounds_6807B8++;
        _6807B8_level_decorations_ids[v5] = i;
      }
      if ( v8 && decor->uCog == 20 )
        decor->uFlags |= LEVEL_DECORATION_OBELISK_CHEST;
      if ( !decor->uEventID )
      {
        if ( decor->IsInteractive() )
        {
          if ( v1 < 124 )
          {
            v6 = v1 + 75;
            decor->_idx_in_stru123 = v6;
            if ( !stru_5E4C90_MapPersistVars._decor_events[v1++] )
              decor->uFlags |= LEVEL_DECORATION_INVISIBLE;
          }
        }
      }
  }

  pGameLoadingUI_ProgressBar->Progress();
  return true;
}
// 6807E0: using guessed type int _6807E0_num_decorations_6807B8;

//----- (0047F223) --------------------------------------------------------
int OutdoorLocation::_47F223_LooksLikeGenerateMonsterLoot()
{
  signed int v1; // ebx@1
  char *v2; // esi@2
  int v3; // eax@7
  OutdoorLocation *v5; // [sp+0h] [bp-4h]@1

  v5 = this;
  v1 = 0;
  if ( (signed int)uNumSpriteObjects > 0 )
  {
    v2 = (char *)&pSpriteObjects[0].uObjectDescID;
    do
    {
      if ( *(short *)v2 )
      {
        if ( !(v2[24] & 8) && !(pObjectList->pObjects[*(short *)v2].uFlags & 0x10) )
          *(int *)(v2 + 10) = GetTerrainHeightsAroundParty2(*(int *)(v2 + 2), *(int *)(v2 + 6), (int *)&v5, 0);
        v3 = *(int *)(v2 + 34);
        if ( v3 )
        {
          if ( v3 != 220 && pItemsTable->pItems[v3].uEquipType == 14 && !*(int *)(v2 + 38) )
            *(int *)(v2 + 38) = rand() % 15 + 5;
          pItemsTable->SetSpecialBonus((ItemGen *)(v2 + 34));
        }
      }
      ++v1;
      v2 += 112;
    }
    while ( v1 < (signed int)uNumSpriteObjects );
  }
  pGameLoadingUI_ProgressBar->Progress();
  return 1;
}

//----- (0047F2D3) --------------------------------------------------------
bool OutdoorLocation::InitalizeActors(int a1)
{
  Actor *v3; // esi@2
  int v4; // eax@3
  __int16 v5; // ax@11
  int alert_status; // [sp+348h] [bp-8h]@1
  int v9; // [sp+34Ch] [bp-4h]@1

  alert_status = 0;
  for(int i=0; i<uNumActors; ++i) 
      {
      v4 = pActors[i].uAttributes;
      if ( !(v4 & 0x100000) )
          {
          if ( alert_status != 1 )
              {
              pActors[i].uCurrentActionTime = 0;
              pActors[i].uCurrentActionLength = 0;
              if ( v4 & 0x10000 )
                  pActors[i].uAIState = AIState::Disabled;
              if ( pActors[i].uAIState != AIState::Removed && pActors[i].uAIState != AIState::Disabled &&
                    (pActors[i].sCurrentHP == 0 || pActors[i].pMonsterInfo.uHP == 0) )
                  pActors[i].uAIState = AIState::Dead;
              pActors[i].vVelocity.x = 0;
              pActors[i].vVelocity.y = 0;
              pActors[i].vVelocity.z = 0;
              pActors[i].UpdateAnimation();
			  pActors[i].pMonsterInfo.uHostilityType = MonsterInfo::Hostility_Friendly;
              pActors[i].PrepareSprites(0);
              }
          else
              {
              pActors[i].uAIState = AIState::Disabled;
              pActors[i].uAttributes = v4 | 0x10000;
              }
          }
      else if ( a1 == 0 )
          {
          pActors[i].uAIState = AIState::Disabled;
          pActors[i].uAttributes = v4 | 0x10000;
          }
      else if ( alert_status != 0 )
          {
          pActors[i].uCurrentActionTime = 0;
          pActors[i].uCurrentActionLength = 0;
          if ( v4 & 0x10000 )
              pActors[i].uAIState = AIState::Disabled;
          v5 = pActors[i].uAIState;
          if ( pActors[i].uAIState != AIState::Removed && pActors[i].uAIState != AIState::Disabled && 
              (pActors[i].sCurrentHP == 0 || pActors[i].pMonsterInfo.uHP == 0) )
              pActors[i].uAIState = AIState::Dead;
          pActors[i].vVelocity.x = 0;
          pActors[i].vVelocity.y = 0;
          pActors[i].vVelocity.z = 0;
          pActors[i].UpdateAnimation();
		  pActors[i].pMonsterInfo.uHostilityType = MonsterInfo::Hostility_Friendly;
          pActors[i].PrepareSprites(0);
          }
      else 
          {
          pActors[i].uAIState = AIState::Disabled;
          pActors[i].uAttributes = v4 | 0x10000;
          alert_status = GetAlertStatus();
          }
      }

  pGameLoadingUI_ProgressBar->Progress();
  //no use for this
//  Actor thisa;
//  thisa.pMonsterInfo.uID = 45;
//  thisa.PrepareSprites(0);
  return 1;
}

//----- (0047F3EA) --------------------------------------------------------
bool OutdoorLocation::LoadRoadTileset()
{
  pTileTypes[3].uTileID = pTileTable->GetTileForTerrainType(pTileTypes[3].tileset, 1);
  pTileTable->InitializeTileset(pTileTypes[3].tileset);
  return 1;
}

//----- (0047F420) --------------------------------------------------------
bool OutdoorLocation::LoadTileGroupIds()
{
  for (uint i = 0; i < 3; ++i)
    pTileTypes[i].uTileID = pTileTable->GetTileForTerrainType(pTileTypes[i].tileset, 1);

  return true;
}

//----- (0047B42C) --------------------------------------------------------
void OutdoorLocation::PrepareActorsDrawList()
{
  unsigned int result; // eax@1
  //Actor *v1; // edi@2
  //__int16 v2; // ax@3
  int z; // esi@5
  float v4; // ST48_4@8
  //double v5; // ST38_8@8
  //float v6; // ST48_4@10
  //double v7; // ST30_8@10
  unsigned int v8; // eax@11
  int v9; // edx@11
  __int16 v10; // dx@11
  unsigned int v11; // eax@13
  signed int v12; // eax@16
  __int16 v13; // cx@21
  SpriteFrame *v14; // eax@24
  SpriteFrame *v15; // ebx@25
  //int *v16; // eax@25
  int v17; // eax@35
  int v18; // ST78_4@36
  int v19; // eax@36
  int v20; // ecx@38
  int v21; // eax@38
  int v22; // ecx@41
  int v23; // ST5C_4@43
  int v24; // esi@44
  signed __int64 v25; // qtt@45
  int v26; // ST54_4@45
  int v27; // ecx@45
  RenderBillboard *v28; // esi@45
  //__int16 v29; // ax@46
  unsigned __int8 v30; // zf@46
  unsigned __int8 v31; // sf@46
  signed __int16 v32; // ax@49
  signed int v33; // ecx@50
  int v34; // ecx@54
  MonsterDesc *v35; // edx@54
  int v36; // ecx@54
  //unsigned __int8 v37; // zf@54
  //unsigned __int8 v38; // sf@54
  unsigned int v39; // [sp-8h] [bp-68h]@23
  unsigned int v40; // [sp-4h] [bp-64h]@23
  int v41; // [sp+24h] [bp-3Ch]@11
  int v42; // [sp+28h] [bp-38h]@38
  int v43; // [sp+28h] [bp-38h]@45
  int v44; // [sp+2Ch] [bp-34h]@36
  int v45; // [sp+2Ch] [bp-34h]@44
  int v46; // [sp+2Ch] [bp-34h]@45
  int v47; // [sp+30h] [bp-30h]@36
  int v48; // [sp+30h] [bp-30h]@41
  signed int v49; // [sp+34h] [bp-2Ch]@5
  int v50; // [sp+34h] [bp-2Ch]@36
  int v51; // [sp+34h] [bp-2Ch]@41
  int v52; // [sp+34h] [bp-2Ch]@50
  int v53; // [sp+38h] [bp-28h]@36
  //int v54; // [sp+3Ch] [bp-24h]@2
  int y; // [sp+40h] [bp-20h]@5
  int x; // [sp+44h] [bp-1Ch]@5
  int v57; // [sp+48h] [bp-18h]@45
  int v58; // [sp+4Ch] [bp-14h]@45
  //signed int v59; // [sp+50h] [bp-10h]@1
  int X; // [sp+54h] [bp-Ch]@36
  //__int16 v61; // [sp+58h] [bp-8h]@5
  signed __int16 v62; // [sp+5Ch] [bp-4h]@25

  //result = 0;
  //v59 = 0;
  for (int i = 0; i < uNumActors; ++i)
  {
    //v54 = 0;
    //v1 = pActors;//[0].vPosition.z;
    //do
    //{
    auto actor = &pActors[i];
	  //v2 = actor->uAIState;

	actor->uAttributes &= 0xFFFFFFF7u;
    if (actor->uAIState == Removed || actor->uAIState == Disabled)
      continue;

	  z = actor->vPosition.z;
      v49 = 0;
	  x = actor->vPosition.x;
	  y = actor->vPosition.y;
      if (actor->uAIState == Summoned)
      {
		if (PID_TYPE(actor->uSummonerID) != OBJECT_Actor ||
             pActors[PID_ID(actor->uSummonerID)].pMonsterInfo.uSpecialAbilityDamageDiceSides != 1 )
        {
          z += floorf(actor->uActorHeight * 0.5f + 0.5f);
        }
        else
        {
          v49 = 1;
		  pGame->pStru6Instance->_4A7F74(actor->vPosition.x, actor->vPosition.y, z);
          v4 = (1.0 - (double)actor->uCurrentActionTime / (double)actor->uCurrentActionLength) *
			   (double)(2 * actor->uActorHeight);
          z -= floorf(v4 + 0.5f);
          if ( z > actor->vPosition.z )
            z = actor->vPosition.z;
        }
      }
      v8 = stru_5C6E00->Atan2(actor->vPosition.x - pGame->pIndoorCameraD3D->vPartyPos.x,
                              actor->vPosition.y - pGame->pIndoorCameraD3D->vPartyPos.y);
	  LOWORD(v9) = actor->uYawAngle;
      v41 = ((signed int)(stru_5C6E00->uIntegerPi + ((signed int)stru_5C6E00->uIntegerPi >> 3) + v9 - v8) >> 8) & 7;
	  v10 = actor->uCurrentActionAnimation;
      if ( pParty->bTurnBasedModeOn )
      {
        if ( v10 != 1 )
        {
LABEL_17:
			v12 = actor->uCurrentActionTime;
          goto LABEL_18;
        }
        v11 = pMiscTimer->uTotalGameTimeElapsed;
      }
      else
      {
        if ( v10 != 1 )
          goto LABEL_17;
        v11 = pEventTimer->uTotalGameTimeElapsed;
      }
      v12 = 32 * i + v11;
LABEL_18:
      if ( (signed __int64)actor->pActorBuffs[5].uExpireTime > 0 || (signed __int64)actor->pActorBuffs[6].uExpireTime > 0 )
        v12 = 0;
	  v13 = actor->uAIState;
      if ( v13 == 17 && !v49 )
      {
        v40 = v12;
        v39 = uSpriteID_Spell11;
LABEL_24:
        v14 = pSpriteFrameTable->GetFrame(v39, v40);
        goto LABEL_25;
      }
      v40 = v12;
	  v39 = actor->pSpriteIDs[v10];
      if ( v13 != 16 )
        goto LABEL_24;
      v14 = pSpriteFrameTable->GetFrameBy_x(v39, v12);
LABEL_25:
      v62 = 0;
      v15 = v14;
      //v16 = (int *)v14->uFlags;
      if (v14->uFlags & 2)
        v62 = 2;
      if (v14->uFlags & 0x40000)
        v62 |= 0x40u;
      if (v14->uFlags & 0x20000)
        LOBYTE(v62) = v62 | 0x80;
      if ((256 << v41) & v14->uFlags)
        v62 |= 4u;
      if ( v15->uGlowRadius )
      {
        //LOBYTE(v16) = byte_4E94D3;
        pMobileLightsStack->AddLight(x, y, z, 0, v15->uGlowRadius, 0xFFu, 0xFFu, 0xFFu, byte_4E94D3);
      }
      v17 = (x - pGame->pIndoorCameraD3D->vPartyPos.x) << 16;
      if (pGame->pIndoorCameraD3D->sRotationX)
      {
        v18 = (y - pGame->pIndoorCameraD3D->vPartyPos.y) << 16;
        v47 = ((unsigned __int64)(v17 * (signed __int64)pGame->pIndoorCameraD3D->int_cosine_y) >> 16)
            + ((unsigned __int64)(v18 * (signed __int64)pGame->pIndoorCameraD3D->int_sine_y) >> 16);
        v50 = (unsigned __int64)(v17 * (signed __int64)pGame->pIndoorCameraD3D->int_sine_y) >> 16;
        v53 = (unsigned __int64)(v18 * (signed __int64)pGame->pIndoorCameraD3D->int_cosine_y) >> 16;
        v44 = (z - pGame->pIndoorCameraD3D->vPartyPos.z) << 16;
        v19 = ((unsigned __int64)(v44 * (signed __int64)pGame->pIndoorCameraD3D->int_sine_x) >> 16)
            + ((unsigned __int64)(v47 * (signed __int64)pGame->pIndoorCameraD3D->int_cosine_x) >> 16);
        X = ((unsigned __int64)(v44 * (signed __int64)pGame->pIndoorCameraD3D->int_sine_x) >> 16)
          + ((unsigned __int64)(v47 * (signed __int64)pGame->pIndoorCameraD3D->int_cosine_x) >> 16);
        if ( v19 < 262144 || v19 > pODMRenderParams->shading_dist_mist << 16 )
          continue;
        v20 = v53 - v50;
        v42 = v53 - v50;
        v21 = ((unsigned __int64)(v44 * (signed __int64)pGame->pIndoorCameraD3D->int_cosine_x) >> 16)
            - ((unsigned __int64)(v47 * (signed __int64)pGame->pIndoorCameraD3D->int_sine_x) >> 16);
      }
      else
      {
        v48 = (y - pGame->pIndoorCameraD3D->vPartyPos.y) << 16;
        v51 = (unsigned __int64)(v17 * (signed __int64)pGame->pIndoorCameraD3D->int_cosine_y) >> 16;
        v22 = (unsigned __int64)(v48 * (signed __int64)pGame->pIndoorCameraD3D->int_sine_y) >> 16;
        X = v22 + v51;
        if ( v22 + v51 < 262144 || v22 + v51 > pODMRenderParams->shading_dist_mist << 16 )
          continue;
        v23 = (unsigned __int64)(((x - pGame->pIndoorCameraD3D->vPartyPos.x) << 16)
                               * (signed __int64)pGame->pIndoorCameraD3D->int_sine_y) >> 16;
        v20 = ((unsigned __int64)(v48 * (signed __int64)pGame->pIndoorCameraD3D->int_cosine_y) >> 16) - v23;
        v42 = ((unsigned __int64)(v48 * (signed __int64)pGame->pIndoorCameraD3D->int_cosine_y) >> 16) - v23;
        v21 = (z - pGame->pIndoorCameraD3D->vPartyPos.z) << 16;
      }
      v45 = v21;
      v24 = abs(v20);
      if ( abs(X) >= v24 )
      {
        LODWORD(v25) = 0;
        HIDWORD(v25) = SLOWORD(pODMRenderParams->int_fov_rad);
        v58 = v25 / X;
        v26 = v25 / X;
        LODWORD(v25) = 0;
        HIDWORD(v25) = SLOWORD(pODMRenderParams->int_fov_rad);
        v57 = v25 / X;
        v27 = pViewport->uScreenCenterX
            - ((signed int)(((unsigned __int64)(v26 * (signed __int64)v42) >> 16) + 32768) >> 16);
        v43 = pViewport->uScreenCenterX
            - ((signed int)(((unsigned __int64)(v26 * (signed __int64)v42) >> 16) + 32768) >> 16);
        v46 = pViewport->uScreenCenterY - ((signed int)(((unsigned __int64)(v25 / X * v45) >> 16) + 32768) >> 16);
        result = uNumBillboardsToDraw;
        v28 = &pBillboardRenderList[uNumBillboardsToDraw];
        if (uNumBillboardsToDraw >= 500)
          return;
        ++uNumBillboardsToDraw;
        ++uNumSpritesDrawnThisFrame;
		actor->uAttributes |= 8u;
        v28->uHwSpriteID = v15->pHwSpriteIDs[v41];
        v28->uIndoorSectorID = 0;
        v28->uPalette = v15->uPaletteIndex;
        v28->_screenspace_x_scaler_packedfloat = (unsigned __int64)(v15->scale * (signed __int64)v58) >> 16;
        v30 = HIDWORD(actor->pActorBuffs[3].uExpireTime) == 0;
        v31 = SHIDWORD(actor->pActorBuffs[3].uExpireTime) < 0;
        v28->_screenspace_y_scaler_packedfloat = (unsigned __int64)(v15->scale * (signed __int64)v57) >> 16;
        if ( v31 || v31 | v30 && LODWORD(actor->pActorBuffs[3].uExpireTime) <= 0u )
        {
          if ( (signed __int64)actor->pActorBuffs[10].uExpireTime > 0i64 )
          {
            v52 = (unsigned __int64)(pGame->pStru6Instance->_4A806F(actor)
                                   * (signed __int64)v28->_screenspace_y_scaler_packedfloat) >> 16;
LABEL_53:
            LOWORD(v27) = v43;
            v28->_screenspace_y_scaler_packedfloat = v52;
          }
        }
        else
        {
          v32 = actor->pActorBuffs[3].uPower;
          if ( v32 )
          {
            v33 = actor->pActorBuffs[3].uPower;
            v28->_screenspace_x_scaler_packedfloat = (unsigned __int64)(65536 / (unsigned __int16)v32 * (signed __int64)v28->_screenspace_x_scaler_packedfloat) >> 16;
            v52 = (unsigned __int64)(65536 / v33 * (signed __int64)v28->_screenspace_y_scaler_packedfloat) >> 16;
            goto LABEL_53;
          }
        }
        v28->uScreenSpaceX = v27;
        v28->uScreenSpaceY = v46;
        v28->world_x = x;
        v28->world_y = y;
        v28->world_z = z;
        HIWORD(v34) = HIWORD(X);
        LOWORD(v34) = 0;
        v28->dimming_level = 0;
        v28->sZValue = v34 + PID(OBJECT_Actor, i);
        v28->field_14_actor_id = i;
        v35 = pMonsterList->pMonsters;
		v36 = actor->pMonsterInfo.uID;
        v28->field_1E = v62 | 0x200;
        v28->pSpriteFrame = v15;
        v28->uTintColor = pMonsterList->pMonsters[actor->pMonsterInfo.uID-1].uTintColor;//*((int *)&v35[v36] - 36);
        if (actor->pActorBuffs[ACTOR_BUFF_STONED].uExpireTime)
          v28->field_1E = v62 | 0x200;
      }
//LABEL_58:
      //++v59;
      //v54 += 32;
      //result = v59;
      //++v1;
    //}
    //while ( v59 < (signed int)uNumActors );
  }
  //return result;
}

//----- (0044C1E8) --------------------------------------------------------
bool ODMFace::HasEventHint()
{
  signed int event_index; // eax@1
  _evt_raw* start_evt;
  _evt_raw* end_evt;

  event_index = 0;
  if ( (uLevelEVT_NumEvents - 1) <= 0 )
    return false;
  while ( pLevelEVT_Index[event_index].uEventID != this->sCogTriggeredID )
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
//----- (0046D49E) --------------------------------------------------------
int ODM_GetFloorLevel(int X, signed int Y, int Z, int __unused, int *pIsOnWater, int *bmodel_pid, int bWaterWalk)
{
  BSPModel *pBModel; // esi@4
  ODMFace *pFace; // ecx@11
  int v14; // edx@20
  signed int v18; // edx@26
  int v19; // eax@28
  int v20; // edx@30
  int v21; // ST1C_4@30
  signed int v22; // edx@30
  signed __int64 v23; // qtt@30
  int v24; // eax@36
  signed int v25; // ecx@38
  int result; // eax@42
  signed int v27; // ecx@43
  int v28; // edi@44
  signed int v29; // edx@44
  int v30; // esi@45
  int v31; // eax@45
  ODMFace *v32; // eax@57
  int v33; // ecx@59
  int v36; // [sp+14h] [bp-2Ch]@24
  int v38; // [sp+1Ch] [bp-24h]@2
  int v39; // [sp+20h] [bp-20h]@9
  signed int pBModelNum; // [sp+28h] [bp-18h]@1
  int pFaceNum; // [sp+2Ch] [bp-14h]@8
  bool v43; // [sp+30h] [bp-10h]@22
  bool v44; // [sp+34h] [bp-Ch]@24
  signed int v46; // [sp+3Ch] [bp-4h]@1
  signed int v48; // [sp+58h] [bp+18h]@22
  signed int v49; // [sp+58h] [bp+18h]@43

  v46 = 1;
  dword_721160[0] = -1;
  dword_721110[0] = -1;
  odm_floor_level[0] = GetTerrainHeightsAroundParty2(X, Y, pIsOnWater, bWaterWalk);
  
  for ( pBModelNum = 0; pBModelNum < pOutdoor->uNumBModels; ++pBModelNum )
  {
    pBModel = &pOutdoor->pBModels[pBModelNum];
    if ( X <= pBModel->sMaxX && X >= pBModel->sMinX &&
         Y <= pBModel->sMaxY && Y >= pBModel->sMinY )
    {
      if ( pBModel->uNumFaces > 0 )
      {
        v39 = 0;
        for ( pFaceNum = 0; pFaceNum < pBModel->uNumFaces; ++pFaceNum )
        {
          pFace = &pBModel->pFaces[pFaceNum];
          if ( (pFace->uPolygonType == POLYGON_Floor || pFace->uPolygonType == POLYGON_InBetweenFloorAndWall)
            && !(pFace->uAttributes & 0x20000000)
            && X <= pFace->pBoundingBox.x2 && X >= pFace->pBoundingBox.x1
            && Y <= pFace->pBoundingBox.y2 && Y >= pFace->pBoundingBox.y1 )
          {
            
              for ( uint i = 0; i < pFace->uNumVertices; ++i)
              {
                word_721040[2 * i] = pFace->pXInterceptDisplacements[i] + pBModel->pVertices.pVertices[pFace->pVertexIDs[i]].x;
                word_720F70[2 * i] = pFace->pXInterceptDisplacements[i + 1] + pBModel->pVertices.pVertices[pFace->pVertexIDs[i]].y;
                word_721040[2 * i + 1] = pFace->pXInterceptDisplacements[i] + pBModel->pVertices.pVertices[pFace->pVertexIDs[i + 1]].x;
                word_720F70[2 * i + 1] = pFace->pXInterceptDisplacements[i + 1] + pBModel->pVertices.pVertices[pFace->pVertexIDs[i + 1]].y;
              }
            word_721040[2 * pFace->uNumVertices] = word_721040[0];
            word_720F70[2 * pFace->uNumVertices] = word_720F70[0];
            v43 = word_720F70[0] >= Y;
            v48 = 0;
            if ( 2 * pFace->uNumVertices > 0 )
            {
              for ( int i = 0; i < 2 * pFace->uNumVertices; ++i )
              {
                if ( v48 >= 2 )
                  break;
                v36 = word_720F70[i + 1];
                v44 = word_720F70[i + 1] >= Y;
                if ( v43 != v44 )
                {
                  v18 = word_721040[i + 1] >= X ? 0 : 2;
                  v19 = v18 | (word_721040[i] < X);
                  if ( v19 != 3 )
                  {
                    if ( !v19 )
                      ++v48;
                    else
                    {
                      LODWORD(v23) = (Y - word_720F70[i]) << 16;
                      HIDWORD(v23) = (Y - word_720F70[i]) >> 16;
                      v22 = ((((word_721040[i + 1] - word_721040[i]) * v23 / (v36 - word_720F70[i])) >> 16) + word_721040[i]);
                      if ( v22 >= X) 
                        ++v48;
                    }
                  }
                }
                v43 = v44;
              }
              if ( v48 == 1 )
              {
                if ( v46 >= 20 )
                  break;
                if ( pFace->uPolygonType == POLYGON_Floor )
                  v24 = pBModel->pVertices.pVertices[pFace->pVertexIDs[0]].z;
                else
                  v24 = ((unsigned __int64)(pFace->zCalc1 * (signed __int64)X) >> 16) + ((unsigned __int64)(pFace->zCalc2 * (signed __int64)Y) >> 16)
                + HIWORD(pFace->zCalc3);
                v25 = v46++;
                odm_floor_level[v25] = v24;
                dword_721160[v25] = pBModelNum;
                dword_721110[v25] = pFaceNum;
              }
            }
          }

        }
      }
    }
  }
  if ( v46 == 1 )
  {
    *bmodel_pid = 0;
    return odm_floor_level[0];
  }
  v27 = 0;
  if ( v46 <= 1 )
    *bmodel_pid = 0;
  else
  {
    //v29 = 1;
    for ( v49 = 1; v49 < v46; ++v49 )
    {
      if ( odm_floor_level[v49] == odm_floor_level[0] )
      {
        v27 = v49;
        //++v29;
        break;
      }
      if ( odm_floor_level[0] > Z + 5 )
      {
        if ( odm_floor_level[v49] >= odm_floor_level[0] )
        {
          //++v29;
          break;
        }
        v27 = v49;
        //++v29;
        break;
      }
      if ( odm_floor_level[v49] > odm_floor_level[0] && odm_floor_level[v49] <= Z + 5 )
      {
        v27 = v49;
        //++v29;
      }
    }
    if ( !v27 )
      *bmodel_pid = 0;
    else
      *bmodel_pid = dword_721110[v27] | (dword_721160[v27] << 6);
  }
  if ( v27 )
  {
    v32 = &pOutdoor->pBModels[dword_721160[v27]].pFaces[dword_721110[v27]];
    *pIsOnWater = false;
    if ( v32->Fluid())
      *pIsOnWater = true;
  }
  if ( odm_floor_level[v27] >= odm_floor_level[0] )
    odm_floor_level[0] = odm_floor_level[v27];
  return odm_floor_level[0];
}

//not sure if right- or left-handed coordinate space assumed, so this could be normal of inverse normal
// for a right-handed system, that would be an inverse normal
//----- (0046DCC8) --------------------------------------------------------
void ODM_GetTerrainNormalAt(int pos_x, int pos_z, Vec3_int_ *out)
{
  auto grid_x = WorldPosToGridCellX(pos_x);
  auto grid_z = WorldPosToGridCellZ(pos_z) - 1;

  auto grid_pos_x1 = GridCellToWorldPosX(grid_x);
  auto grid_pos_x2 = GridCellToWorldPosX(grid_x + 1);
  auto grid_pos_z1 = GridCellToWorldPosZ(grid_z);
  auto grid_pos_z2 = GridCellToWorldPosZ(grid_z + 1);

  auto x1z1_y = pOutdoor->DoGetHeightOnTerrain(grid_x, grid_z);
  auto x2z1_y = pOutdoor->DoGetHeightOnTerrain(grid_x + 1, grid_z);
  auto x2z2_y = pOutdoor->DoGetHeightOnTerrain(grid_x + 1, grid_z + 1);
  auto x1z2_y = pOutdoor->DoGetHeightOnTerrain(grid_x, grid_z + 1);

  float side1_dx, side1_dy, side1_dz,
        side2_dx, side2_dy, side2_dz;

  auto dx = abs(pos_x - grid_pos_x1),
       dz = abs(grid_pos_z1 - pos_z);
  if (dz >= dx)
  {
    side1_dy = (double)(x1z1_y - x1z2_y);
    side2_dy = (double)(x2z2_y - x1z2_y);
    side2_dx = (double)(grid_pos_x2 - grid_pos_x1);
    side1_dx = (double)(grid_pos_x1 - grid_pos_x2);
    side2_dz = 0.0;//(double)(grid_pos_z2 - grid_pos_z2);  // bug?  z2 - z2 
    side1_dz = (double)(grid_pos_z1 - grid_pos_z2);  //       z1 - z2 yes
    //Log::Warning(L"%S %S %u\n", __FILE__, __FUNCTION__, __LINE__);
    /*       |\
       side1 |  \
             |____\
             side 2      */
  }
  else
  {
    side1_dy = (double)(x2z2_y - x2z1_y);
    side2_dy = (double)(x1z1_y - x2z1_y);
    side2_dx = (double)(grid_pos_x1 - grid_pos_x2);
    side1_dx = (double)(grid_pos_x2 - grid_pos_x1);
    side2_dz = 0.0;//(double)(grid_pos_z1 - grid_pos_z1); 
    side1_dz = (double)(grid_pos_z2 - grid_pos_z1);

    /*   side 2
         _____
         \    |
           \  | side 1
             \|       */
  }
  
  float nx = side1_dy * side2_dz - side1_dz * side2_dy;
  float ny = side1_dx * side2_dy - side1_dy * side2_dx;
  float nz = side1_dz * side2_dx - side1_dx * side2_dz;

  float mag = sqrt(nx * nx + ny * ny + nz * nz);
  if (fabsf(mag) < 1e-6f)
  {
    out->y = 0;
    out->x = 0;
    out->z = 65536;
  }
  else
  {
    float invmag = 1.0 / mag;
    out->x = invmag * nx * 65536.0;
    out->y = invmag * ny * 65536.0;
    out->z = invmag * nz * 65536.0;
  }
}
//----- (004014E6) --------------------------------------------------------
void MakeActorAIList_ODM()
{
  int v1; // eax@4
  int v2; // ebx@4
  unsigned int v3; // ecx@4
  int v4; // edx@5
  int v5; // edx@7
  unsigned int v6; // edx@9
  unsigned int v7; // ST20_4@10
  int v9; // edi@10
  int v10; // ebx@14
  int v21; // [sp+Ch] [bp-14h]@4
  int v22; // [sp+10h] [bp-10h]@4

  pParty->uFlags &= 0xFFFFFFCFu;

  ai_arrays_size = 0;
  for (uint i = 0; i < uNumActors; ++i)
  {
    auto actor = &pActors[i];

    actor->uAttributes &= 0xFFFFFBFF;
    if (!actor->CanAct())
    {
      actor->uAttributes &= 0xFFFFBFFF;
      continue;
    }

    v22 = abs(pParty->vPosition.z - actor->vPosition.z);
    v21 = abs(pParty->vPosition.y - actor->vPosition.y);
    v1 = abs(pParty->vPosition.x - actor->vPosition.x);
      v2 = v21;
      v3 = v22;
      if ( v1 < v21 )
      {
        v4 = v1;
        v1 = v21;
        v2 = v4;
      }
      if ( v1 < v22 )
      {
        v5 = v1;
        v1 = v22;
        v3 = v5;
      }
      if ( v2 < (signed int)v3 )
      {
        v6 = v3;
        v3 = v2;
        v2 = v6;
      }
      v7 = ((unsigned int)(11 * v2) >> 5) + (v3 >> 2) + v1;
	  //v8 = actor->uActorRadius;
      v9 = v7 - actor->uActorRadius;
      //v23 = v7 - v8;
      if ( v9 < 0 )
      {
        v9 = 0;
        //v23 = 0;
      }

      if (v9 < 5632)
      {
        v10 = actor->uAttributes & 0xFEFFFFFF;
        actor->uAttributes = v10;
        if ( v10 & 0x80000 || actor->GetActorsRelation(0) )
        {
          //v11 = (pParty->uFlags & 0x10) == 0;
          actor->uAttributes = v10 | 0x1000000;
          if (v9 < 5120 )
            pParty->SetYellowAlert();
          if (v9 < 307)
            pParty->SetRedAlert();
        }
		actor->uAttributes |= 0x00004000;
        ai_near_actors_distances[ai_arrays_size] = v9;
        ai_near_actors_ids[ai_arrays_size++] = i;
      }
      else
		  actor->uAttributes &= 0xFFFFBFFF;
  }

  /*
  result = v27;
  if ( v27 > 0 )
  {
    v14 = 0;
    v15 = 1;
    v26 = 1;
    do
    {
      while ( 1 )
      {
        v24 = v15;
        if ( v15 >= result )
          break;
        v16 = ai_near_actors_distances[v14];
        if ( v16 > ai_near_actors_distances[v15] )
        {
          v17 = &ai_near_actors_ids[v15];
          v18 = ai_near_actors_ids[v14];
          ai_near_actors_ids[v14] = *v17;
          *v17 = v18;
          v15 = v24;
          ai_near_actors_distances[v14] = ai_near_actors_distances[v24];
          ai_near_actors_distances[v24] = v16;
        }
        result = v27;
        ++v15;
      }
      ++v14;
      v15 = v26 + 1;
      v26 = v15;
    }
    while ( v15 - 1 < result );
  }*/

  for (uint i = 0; i < ai_arrays_size; ++i)
    for (uint j = 0; j < i; ++j)
      if (ai_near_actors_distances[j] > ai_near_actors_distances[i])
      {
        int tmp = ai_near_actors_distances[j];
        ai_near_actors_distances[j] = ai_near_actors_distances[i];
        ai_near_actors_distances[i] = tmp;

        tmp = ai_near_actors_ids[j];
        ai_near_actors_ids[j] = ai_near_actors_ids[i];
        ai_near_actors_ids[i] = tmp;
      }


  if (ai_arrays_size > 30)
    ai_arrays_size = 30;

  for (uint i = 0; i < ai_arrays_size; ++i)
    pActors[ai_near_actors_ids[i]].uAttributes |= 0x0400;
}
//----- (0046BE0A) --------------------------------------------------------
void ODM_UpdateUserInputAndOther()
{
  bool v0; // eax@5
  char pOut[32]; // [sp+8h] [bp-20h]@5

  UpdateObjects();
  ODM_ProcessPartyActions();
  if ( pParty->vPosition.x < -22528
    || pParty->vPosition.x > 22528
    || pParty->vPosition.y < -22528
    || pParty->vPosition.y > 22528 )
  {
    strcpy(pOutdoor->pLevelFilename, pCurrentMapName);
    v0 = pOutdoor->GetTravelDestination(pParty->vPosition.x, pParty->vPosition.y, pOut, 32);
    if ( !bUnderwater && (pParty->uFlags & (PARTY_FLAGS_1_STANDING_ON_WATER | PARTY_FLAGS_1_FALLING | 0x04) || pParty->uFlags & 0x0200 || pParty->bFlying) || !v0 )
    {
      if ( pParty->vPosition.x < -22528 )
        pParty->vPosition.x = -22528;
      if ( pParty->vPosition.x > 22528 )
        pParty->vPosition.x = 22528;
      if ( pParty->vPosition.y < -22528 )
        pParty->vPosition.y = -22528;
      if ( pParty->vPosition.y > 22528 )
        pParty->vPosition.y = 22528;
    }
    else
    {
      pAudioPlayer->StopChannels(-1, -1);
      TravelUI_Load();
    }
  }
  UpdateActors_ODM();
  check_event_triggers();
}
//----- (0041F54A) --------------------------------------------------------
void LoadActualSkyFrame()
{
  if ( pTexture_RestUI_CurrentSkyFrame )
    pTexture_RestUI_CurrentSkyFrame->Release();
  if ( pTexture_RestUI_CurrentHourglassFrame )
    pTexture_RestUI_CurrentHourglassFrame->Release();
  pIcons_LOD->SyncLoadedFilesCount();
  sprintf(pTmpBuf.data(), "TERRA%03d", pParty->uCurrentMinute / 6 + 10 * pParty->uCurrentHour);
  pTexture_RestUI_CurrentSkyFrame = pIcons_LOD->LoadTexturePtr(pTmpBuf.data(), TEXTURE_16BIT_PALETTE);
}


//----- (004626BA) --------------------------------------------------------
OutdoorLocation::OutdoorLocation()
{
  subconstuctor();
  uLastSunlightUpdateMinute = 0;

  uNumBModels = 0;
  pBModels = nullptr;
}

//----- (004626CD) --------------------------------------------------------
void OutdoorLocation::subconstuctor()
{
  //OutdoorLocationTerrain::OutdoorLocationTerrain(&this->pTerrain);
  field_F0 = 0;
  field_F4 = 0x40000000u;
  //DLVHeader::DLVHeader(&v1->ddm);
  pSpawnPoints = 0;
  pBModels = 0;
  pCmap = 0;
  pFaceIDLIST = 0;
  pOMAP = 0;
}




//----- (00481E55) --------------------------------------------------------
void ODM_Project(unsigned int uNumVertices)
{
  double v1; // st7@2
  double v2; // st6@2
  double v3; // st5@2
  int v4; // eax@2
  unsigned int v5; // edx@2
  double v6; // st4@3
  double v7; // st3@3

  if ( (signed int)uNumVertices > 0 )
  {
    v1 = (double)pODMRenderParams->int_fov_rad;
    v2 = (double)pViewport->uScreenCenterX;
    v3 = (double)pViewport->uScreenCenterY;
    v4 = 0;
    v5 = uNumVertices;
    do
    {
      v6 = v1 * array_507D30[v4]._rhw;
      v7 = v6 * array_507D30[v4].vWorldViewPosition.y;
      memcpy(&array_50AC10[v4], &array_507D30[v4], sizeof(array_50AC10[v4]));
      array_50AC10[v4].vWorldViewProjX = v2 - v7;
      array_50AC10[v4].vWorldViewProjY = v3 - v6 * array_507D30[v4].vWorldViewPosition.z;
      ++v4;
      --v5;
    }
    while ( v5 );
  }
}
//----- (00485F64) --------------------------------------------------------
void ODMRenderParams::Initialize()
{
  int v1; // eax@1
  int v2; // eax@2
  signed __int64 v3; // qtt@4
  int v4; // eax@4

  this->uCameraFovInDegrees = 75;
  v1 = stru_5C6E00->uPiMask & 0xD5;
  if ( v1 >= (signed int)stru_5C6E00->uIntegerHalfPi )
    v2 = -stru_5C6E00->pTanTable[stru_5C6E00->uIntegerPi - v1];
  else
    v2 = stru_5C6E00->pTanTable[v1];
  LODWORD(v3) = (viewparams->uSomeZ - viewparams->uSomeX) << 31;
  HIDWORD(v3) = (viewparams->uSomeZ - viewparams->uSomeX) << 15 >> 16;
  v4 = (signed int)(v3 / v2) >> 16;
  this->int_fov_rad = v4;
  this->field_4C = 360000;
  this->int_fov_rad_inv = 65536 / v4;
  this->field_50 = 115;
  //sr_6BE060[1] = 1;
  //RotationToInts();
}
