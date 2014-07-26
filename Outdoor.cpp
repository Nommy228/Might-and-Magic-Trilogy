#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define _CRT_SECURE_NO_WARNINGS
#include "ErrorHandling.h"
#include "mm7_unsorted_subs.h"
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
#include "OurMath.h"
#include "ObjectList.h"
#include "Game.h"
#include "Actor.h"
#include "Chest.h"
#include "stru123.h"
#include "Timer.h"
#include "Viewport.h"
#include "Events.h"
#include "ParticleEngine.h"
#include "TurnEngine.h"

#include "mm7_data.h"
#include "MM7.h"
#include "Lights.h"


#include "MapInfo.h"
#include "Outdoor_stuff.h"
#include "BSPModel.h"
#include "GUIWindow.h"
#include "Level/Decoration.h"
#include "OurMath.h"
#include "ZlibWrapper.h"

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

  pGame->pIndoorCameraD3D->debug_flags = 0;
  if (viewparams->draw_d3d_outlines)
    pGame->pIndoorCameraD3D->debug_flags |= ODM_RENDER_DRAW_D3D_OUTLINES;

  if (bRedraw || true/*pRenderer->pRenderD3D*/)
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

  //if (pRenderer->pRenderD3D) // d3d - redraw always
  {
    pRenderer->DrawOutdoorSkyD3D();
    pRenderer->DrawBuildingsD3D();
    pRenderer->RenderTerrainD3D();
    //pRenderer->DrawBezierTerrain();
  }
  /*else
  {
    if (!bRedraw)
      pRenderer->OnOutdoorRedrawSW();
    else
    {
      pRenderer->DrawBuildingsSW();  
      pRenderer->DrawBezierTerrain();
      sr_sub_486F92_MessWithEdgesAndSpans();
      pODMRenderParams->ApplyLightmapsSW();
    }
  }*/


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
    pOutdoor->UpdateDiscoveredArea(WorldPosToGridCellX(pParty->vPosition.x), WorldPosToGridCellZ(pParty->vPosition.y), 1);
  pGame->uFlags2 &= 0xFFFFFFFEu;
  if (/*pRenderer->pRenderD3D*/true && pRenderer->bUsingSpecular)
    pGame->pLightmapBuilder->uFlags |= 1;
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
  //pWeather->Draw();//если раскомментировать скорость снега быстрее
  trail_particle_generator.UpdateParticles();
}

//----- (00488E23) --------------------------------------------------------
double OutdoorLocation::GetFogDensityByTime()
{
  if ( pParty->uCurrentHour < 5 )//ночь
  {
    pWeather->bNight = true;
    return 60.0 * 0.016666668;
  }
  else if ( pParty->uCurrentHour >= 5 && pParty->uCurrentHour < 6 )//рассвет
  {
    pWeather->bNight = false;
    return (60.0 - (double)(60 * pParty->uCurrentHour + pParty->uCurrentMinute - 300)) * 0.016666668;
  }
  else if ( pParty->uCurrentHour >= 6 && pParty->uCurrentHour < 20 )//день
  {
    pWeather->bNight = false;
     return 0.0;
  }
  else if ( pParty->uCurrentHour >= 20 && pParty->uCurrentHour < 21 )//сумерки
  {
    pWeather->bNight = false;
    return ((double)(pParty->uCurrentHour - 20) * 60.0 + (double)(signed int)pParty->uCurrentMinute) * 0.016666668;
  }
  else//ночь
  {
    pWeather->bNight = true;
    return 60.0 * 0.016666668;
  }
}

//----- (00488EB1) --------------------------------------------------------
int OutdoorLocation::GetSomeOtherTileInfo(int sX, int sY)
{
  //OutdoorLocation *v3; // esi@1
  unsigned int v4; // edi@1
  unsigned int v5; // eax@1
//  int result; // eax@5

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
unsigned int OutdoorLocation::GetTileTexture(signed int sX, signed int sY)
{
  //OutdoorLocation *v3; // esi@1
  signed int v4; // edi@1
  signed int v5; // eax@1
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
bool OutdoorLocation::Initialize(const char *pFilename, int File, size_t uRespawnInterval, int *thisa)
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
  char *mapNumberAsStr; // eax@3
  int mapNumberAsInt; // eax@3
  signed int direction; // esi@7
  signed int destinationMap; // eax@23
  char Str[140]; // [sp+8h] [bp-78h]@3

  strcpy(Str, this->pLevelFilename);//настоящая локация
  _strlwr(Str);
  mapNumberAsStr = strtok(Str, "out");
  mapNumberAsStr[2] = 0;
  mapNumberAsInt = atoi(mapNumberAsStr);
  if ( a5 < 10 || strlen(this->pLevelFilename) != 9 || mapNumberAsInt < 1 || mapNumberAsInt > 15 ) //длина  .odm и количество локаций
    return 0;
  if ( sPartyX < -22528 )//граница карты
    direction = 4;
  else if ( sPartyX > 22528 )
    direction = 3;
  else if ( sPartyZ < -22528 )
    direction = 2;
  else if ( sPartyZ > 22528 )
    direction = 1;
  else
    return false;

  if ( mapNumberAsInt == MAP_AVLEE && direction == 4) // to Shoals
  {
      if ( pPlayers[1]->HasUnderwaterSuitEquipped() &&
           pPlayers[2]->HasUnderwaterSuitEquipped() &&
           pPlayers[3]->HasUnderwaterSuitEquipped() &&
           pPlayers[4]->HasUnderwaterSuitEquipped())
      {
        uDefaultTravelTime_ByFoot = 1;
        strcpy(pOut, "out15.odm");
        uLevel_StartingPointType = MapStartPoint_East;
        LOWORD(pParty->uFlags) &= 0xFD7Bu;
        return true;
      }
  }
  else if ( mapNumberAsInt == MAP_SHOALS && direction == 3 ) //from Shoals
  {
    uDefaultTravelTime_ByFoot = 1;
    strcpy(pOut, "out14.odm");//Авли
    uLevel_StartingPointType = MapStartPoint_West;
    LOWORD(pParty->uFlags) &= 0xFD7Bu;
    return true;
  }
  destinationMap = foot_travel_destinations[mapNumberAsInt - 1][direction - 1];
  if (destinationMap == MAP_INVALID)
    return false;

  assert(destinationMap <= MAP_SHOALS);

  uDefaultTravelTime_ByFoot = foot_travel_times[mapNumberAsInt - 1][direction - 1];
  uLevel_StartingPointType = foot_travel_arrival_points[mapNumberAsInt - 1][direction - 1];
  sprintf(pOut, "out%02d.odm", destinationMap); //локация направления
  return true;
}
// 6BD07C: using guessed type int uDefaultTravelTime_ByFoot;
// 6BE35C: using guessed type int uLevel_StartingPointType;

//----- (0048917E) --------------------------------------------------------
void OutdoorLocation::MessWithLUN()
{
  this->pSpriteIDs_LUN[0] = -1;
  this->pSpriteIDs_LUN[1] = 0;
  this->pSpriteIDs_LUN[2] = pSpriteFrameTable->FastFindSprite("LUN1-4");
  this->pSpriteIDs_LUN[3] = 0;
  this->pSpriteIDs_LUN[4] = pSpriteFrameTable->FastFindSprite("LUN1-2");
  this->pSpriteIDs_LUN[5] = 0;
  this->pSpriteIDs_LUN[6] = pSpriteFrameTable->FastFindSprite("LUN3-4");
  this->pSpriteIDs_LUN[7] = 0;
  this->uSpriteID_LUNFULL = pSpriteFrameTable->FastFindSprite("LUNFULL");
  this->uSpriteID_LUN1_2_cp = pSpriteFrameTable->FastFindSprite("LUN1-2");
  this->uSpriteID_LUN1_4_cp = pSpriteFrameTable->FastFindSprite("LUN1-4");
  this->uSpriteID_LUN3_4_cp = pSpriteFrameTable->FastFindSprite("LUN3-4");
  this->field_D60 = -1;
  this->field_CF0 = 4;
  this->field_CF8 = 4;
  this->field_D00 = 4;
  this->field_CE8 = 0;
  this->field_D3C = (int)this->pSpriteIDs_LUN;
  this->field_D40 = 0;
  this->field_D44 = 0;
  this->field_D48 = 0;
  this->field_D4C = 131072;
  this->field_D5C = 0;
  this->field_D64 = 0;
  this->field_D28 = -1;
  this->field_D08 = 0;
  this->field_D0C = 0;
  this->field_D10 = 0;
  this->field_D24 = 0;
  this->field_D2C = 0;
  this->uSpriteID_LUN_SUN = pSpriteFrameTable->FastFindSprite("LUN-SUN");
  this->field_D14 = -131072;
  for ( uint i = 0; i < 8; i++ )
    pSpriteFrameTable->InitializeSprite(this->pSpriteIDs_LUN[i]);//v2 += 2;
  pSpriteFrameTable->InitializeSprite(this->uSpriteID_LUN_SUN);
}

//----- (004892E6) --------------------------------------------------------
void OutdoorLocation::UpdateSunlightVectors()
{
  unsigned int v3; // edi@3
  double v8; // st7@4

  if ( pParty->uCurrentHour >= 5 && pParty->uCurrentHour < 21 )
  {
    v3 = pParty->uCurrentMinute + 60 * (pParty->uCurrentHour - 5);
    this->inv_sunlight_y = 0;
    this->inv_sunlight_x = stru_5C6E00->Cos((v3 * stru_5C6E00->uIntegerPi) / 960);
    this->inv_sunlight_z = stru_5C6E00->Sin((v3 * stru_5C6E00->uIntegerPi) / 960);
    this->vSunlight.x = -this->inv_sunlight_x;
    this->vSunlight.y = -this->inv_sunlight_y;
    this->vSunlight.z = -this->inv_sunlight_z;
    if ( v3 >= 480 )
      v8 = 960 - v3;
    else
      v8 = v3;
    this->max_terrain_dimming_level = (int)(20.0 - v8 / 480.0 * 20.0);
    this->uLastSunlightUpdateMinute = pParty->uCurrentMinute;
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
  int v7; // eax@4
  int is_on_water; // [sp+8h] [bp-8h]@2
  int bmodel_standing_on_pid; // [sp+Ch] [bp-4h]@2

  is_on_water = 0;
  bmodel_standing_on_pid = 0;
  ODM_GetFloorLevel(x, y, z, pParty->uDefaultPartyHeight, &is_on_water, &bmodel_standing_on_pid, 0);
  if ( pParty->uFlags & 8 || bmodel_standing_on_pid || is_on_water )//на bmodel,и или на воде
    return 2;
  v7 = _47ED83(WorldPosToGridCellX(pParty->vPosition.x), WorldPosToGridCellZ(pParty->vPosition.y) - 1);
  switch ( pTileTable->pTiles[GetTileIdByTileMapId(v7)].tileset )
  {
    case Tileset_Grass://на траве
      return 1;
    case Tileset_Snow://на снегу
      return 3;
    case Tilset_Desert://на песке
      return 5;
    case Tileset_3:
    case Tileset_Dirt:// на грязи
      return 4;
    case Tileset_Water:// on water(на воде)
      return 3;//еденицы еды
    default:
      return 2;
  }
}

//----- (00489487) --------------------------------------------------------
void OutdoorLocation::SetFog()
{
  strcpy(pOutdoor->pLevelFilename, pCurrentMapName);

  MAP_TYPE map_id = pMapStats->GetMapInfo(pCurrentMapName);
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
bool ODMFace::IsBackfaceNotCulled(RenderVertexSoft *a2, struct Polygon *polygon)
{
  unsigned int numOfVertices; // edx@1
  RenderVertexSoft *currVertex; // ecx@2
  double v7; // st7@5
  double v8; // st6@5
  double v9; // st5@5
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

  numOfVertices = polygon->uNumVertices;
  if ( numOfVertices < 3 )
    return false;
  currVertex = &a2[numOfVertices - 1];
  if ( a2->vWorldPosition.z == a2[1].vWorldPosition.z && a2[1].vWorldPosition.z == currVertex->vWorldPosition.z )
    polygon->flags |= 0x10u;

  v28 = a2[1].vWorldPosition.x - a2->vWorldPosition.x;
  v27 = a2[1].vWorldPosition.y - a2->vWorldPosition.y;
  a3a = a2[1].vWorldPosition.z - a2->vWorldPosition.z;


  for (int i = 0; i < numOfVertices; i++)
  {
    v7 = currVertex->vWorldPosition.x - a2->vWorldPosition.x;
    v8 = currVertex->vWorldPosition.y - a2->vWorldPosition.y;
    v9 = currVertex->vWorldPosition.z - a2->vWorldPosition.z;
    v26 = v27 * v9 - v8 * a3a;
    v24 = v7 * a3a - v9 * v28;
    v25 = v8 * v28 - v7 * v27;
    if ( v24 != 0.0 || v25 != 0.0 || v26 != 0.0 )
      break;
    currVertex--;
  }

  if ( ((double)pGame->pIndoorCameraD3D->vPartyPos.x - a2->vWorldPosition.x) * v26
    + ((double)pGame->pIndoorCameraD3D->vPartyPos.z - a2->vWorldPosition.z) * v25
    + ((double)pGame->pIndoorCameraD3D->vPartyPos.y - a2->vWorldPosition.y) * v24 > 0.0 )
  {

    v19 = a2[1].vWorldViewPosition.x - a2->vWorldViewPosition.x;
    v18 = a2[1].vWorldViewPosition.y - a2->vWorldViewPosition.y;
    v20 = a2[1].vWorldViewPosition.z - a2->vWorldViewPosition.z;
    v21 = currVertex->vWorldViewPosition.x - a2->vWorldViewPosition.x;
    v22 = currVertex->vWorldViewPosition.y - a2->vWorldViewPosition.y;
    v23 = currVertex->vWorldViewPosition.z - a2->vWorldViewPosition.z;

    a3b = v23 * v18 - v22 * v20;
    polygon->v_18.x = bankersRounding(a3b);
    a3c = v21 * v20 - v23 * v19;
    polygon->v_18.y = bankersRounding(a3c);
    a3d = v22 * v19 - v21 * v18;
    polygon->v_18.z = bankersRounding(a3d);
    polygon->_normalize_v_18();
    a3e = -((double)polygon->v_18.x * a2->vWorldViewPosition.x)
      - (double)polygon->v_18.y * a2->vWorldViewPosition.y
      - (double)polygon->v_18.z * a2->vWorldViewPosition.z;
    polygon->field_24 = bankersRounding(a3e);
    return true;
  }
  else
    return false;
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
  free(this->pHeightmap);
  pHeightmap = nullptr;
  free(pTilemap);
  pTilemap = nullptr;
  free(pAttributemap);
  pAttributemap = nullptr;
  free(pDmap);
  pDmap = nullptr;

  _47C7A9();
}

//----- (0047C80A) --------------------------------------------------------
void OutdoorLocationTerrain::FillDMap( int X, int Y, int W, int Z )
{
  double v6; // st7@1
  double v7; // st7@2
  double v8; // st7@2
  int result; // eax@3
  int v10; // eax@4
  int v11; // ecx@5
  int v12; // ecx@6
  int v13; // edi@7
  int v14; // edx@9
//  int v15; // eax@15
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
  int v44; // [sp+28h] [bp-20h]@21
  float v45; // [sp+2Ch] [bp-1Ch]@1
  float v46; // [sp+30h] [bp-18h]@1
  float v47; // [sp+34h] [bp-14h]@1
  //int v48; // [sp+38h] [bp-10h]@7
  int v49; // [sp+3Ch] [bp-Ch]@10
  int v50; // [sp+40h] [bp-8h]@9
  float v51; // [sp+44h] [bp-4h]@15
  float v52; // [sp+44h] [bp-4h]@21
  float v53; // [sp+50h] [bp+8h]@15
  float v54; // [sp+50h] [bp+8h]@21
//  int v55; // [sp+54h] [bp+Ch]@15
  float v56; // [sp+54h] [bp+Ch]@15
  float v57; // [sp+54h] [bp+Ch]@21

  v46 = -64.0;
  v47 = -64.0;
  v45 = 64.0;
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
  //v48 = result - 1;
  if ( result - 1 <= Z )
  {
    v39 = v11 - 1;
    for ( v13 = result - 1; v13 <= Z; v13++ )
    {
      v50 = v39;
      if ( v39 <= W )
      {
        result = (v39 - 63) << 9;
        v49 = (v39 - 63) << 9;
        for ( v14 = v39; v14 <= W; v14++ )
        {
          if ( v13 >= 0 && result >= -32256 && v13 <= 127 && result <= 32768 )
          {
            //v15 = pOutLocTerrain->field_10;
            //v55 = pOutLocTerrain->field_10;
            pMapHeight = this->pHeightmap;
            v17 = (int)(&pMapHeight[v13 * this->field_10] + v14);
            v18 = -v13;
            v19 = (64 - v13) << 9;
            v20 = 32 * *(char *)v17;
            v21 = 32 * *(char *)(v17 + 1);

            v22 = (v18 + 63) << 9;
            v41 = v22;
            v23 = (int)(&pMapHeight[this->field_10 * (v13 + 1)] + v14);
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
            v44 = 2 * (v14 + v13 * this->field_10);
            //pOutLocTerrain = pOutLocTerrain2;
            *((char *)this->pDmap + v44 + 1) = (signed __int64)v31;

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
            //v13 = v48;
            *((char *)this->pDmap + v44) = (signed __int64)v38;
            //v14 = v50;
            result = v49;
          }
          //++v14;
          result += 512;
          //v50 = v14;
          v49 = result;
        }
      }
      //++v13;
      //v48 = v13;
    }
    //while ( v13 <= Z );
  }
}

//----- (0047CB57) --------------------------------------------------------
int OutdoorLocationTerrain::_47CB57(int a1, int a2, int a3)
{
  signed int result; // eax@2
//  unsigned __int16 *v5; // edx@3
//  double v6; // st7@3
//  int v8; // eax@3
//  int v9; // eax@4
//  int v10; // eax@5
//  double v11; // st6@7
//  signed int v12; // edi@7
//  int v13; // esi@9
//  char *v14; // esi@10
//  signed int v15; // ecx@10
//  char v16[256]; // [sp+4h] [bp-124h]@9
//  unsigned __int16 *v17; // [sp+104h] [bp-24h]@3
//  float v22; // [sp+118h] [bp-10h]@3
//  float v23; // [sp+11Ch] [bp-Ch]@3
//  int i; // [sp+120h] [bp-8h]@3
//  unsigned int v25; // [sp+124h] [bp-4h]@5
//  signed int a2a; // [sp+134h] [bp+Ch]@3
//  unsigned int a2b; // [sp+134h] [bp+Ch]@7
//  float a3a; // [sp+138h] [bp+10h]@7
//  int a3b; // [sp+138h] [bp+10h]@9

  int num_r_bits = 5;
  int num_g_bits = 6;
  int num_b_bits = 5;

  int r_mask = 0xF800;
  int g_mask = 0x7E0;
  int b_mask = 0x1F;

  //if ( pRenderer->pRenderD3D )
    result = 0;
  /*else
  {
    __debugbreak();
    v5 = PaletteManager::Get_Dark_or_Red_LUT(a2, 0, 1);
    v6 = 0.0;
    v22 = 0.0;
    v8 = 0;
    v17 = v5;
    v23 = 0.0;
    a2a = 0;
    for ( i = 0; i < a3; ++i )
    {
      v9 = *(char *)(v8 + a1);
      if ( v9 )
      {
        v10 = v5[v9];
        v6 = v6 + (double)((signed int)(r_mask & v10) >> (num_b_bits + num_g_bits));
        ++a2a;
        v25 = b_mask & v10;
        v22 = (double)((signed int)(g_mask & v10) >> num_b_bits) + v22;
        v23 = (double)(signed int)(b_mask & v10) + v23;
      }
      v8 = i + 1;
    }
    v11 = 1.0 / (double)a2a;
    a3a = v11;
    v25 = (signed __int64)(a3a * v22);
    i = (signed __int64)(a3a * v23);
    v12 = 0;
    a2b = num_b_bits + num_g_bits;
    while ( 1 )
    {
      v13 = v17[v12];
      a3b = abs((__int64)(signed __int64)(v11 * v6) - ((signed int)(r_mask & v17[v12]) >> a2b));
      BYTE3(a3b) = abs((signed)v25 - ((signed int)(g_mask & v13) >> num_b_bits)) + a3b;
      v16[v12++] = abs((signed)i - (signed)(b_mask & v13)) + BYTE3(a3b);
      if ( v12 >= 256 )
        break;
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
  }*/
  return result;
}

//----- (0047CCE2) --------------------------------------------------------
bool OutdoorLocationTerrain::ZeroLandscape()
{
  memset(this->pHeightmap, 0, 0x4000u);
  memset(this->pTilemap, 90, 0x4000u);
  memset(this->pAttributemap, 0, 0x4000u);
  memset(this->pDmap, 0, 0x8000u);
  this->field_12 = 128;
  this->field_10 = 128;
  this->field_16 = 7;
  this->field_14 = 7;
  this->field_1C = 127;
  this->field_18 = 127;
  return true;
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
  //OutdoorLocation *v1; // esi@1
  void *v2; // eax@1
  void *v3; // ST14_4@1
  void *v4; // eax@1
  void *v5; // ST14_4@1
  void *v6; // eax@1
  //unsigned int v7; // eax@1
  //char v8; // zf@1

  //v1 = this;
  strcpy(this->pLevelFilename, "blank");
  strcpy(this->pLocationFileName, "i6.odm");
  strcpy(this->pLocationFileDescription, "MM6 Outdoor v1.00");
  this->uNumBModels = 0;
  this->pTileTypes[0].tileset = Tileset_Grass;
  this->pTileTypes[1].tileset = Tileset_Water;
  this->pTileTypes[2].tileset = Tileset_6;
  this->pTileTypes[3].tileset = Tileset_RoadGrassCobble;
  this->LoadTileGroupIds();
  this->LoadRoadTileset();
  free(this->pBModels);
  free(this->pSpawnPoints);
  this->pBModels = 0;
  this->pSpawnPoints = 0;
  this->pTerrain.Initialize();
  this->pTerrain.ZeroLandscape();
  this->pTerrain.FillDMap(0, 0, 128, 128);
  free(this->pCmap);
  this->pCmap = 0;
  v2 = malloc(0x8000u);
  v3 = this->pOMAP;
  this->pCmap = v2;
  free(v3);
  this->pOMAP = 0;
  v4 = malloc(0x10000u);
  this->pOMAP = (unsigned int *)v4;
  memset(v4, 0, 0x10000u);
  v5 = this->pFaceIDLIST;
  this->numFaceIDListElems = 0;
  free(v5);
  this->pFaceIDLIST = 0;
  v6 = malloc(2);
  this->pFaceIDLIST = (unsigned __int16 *)v6;
  *(short *)v6 = 0;
  strcpy(this->pSkyTextureName, pDefaultSkyTexture.data());
  this->sSky_TextureID = pBitmaps_LOD->LoadTexture(this->pSkyTextureName);
  strcpy(this->pGroundTileset, byte_6BE124_cfg_textures_DefaultGroundTexture.data());
  //v7 = pBitmaps_LOD->LoadTexture(this->pGroundTileset);
  this->sMainTile_BitmapID = pBitmaps_LOD->LoadTexture(this->pGroundTileset);

  if ( this->sSky_TextureID == -1 )
    Error("Invalid Sky Tex Handle");

  if ( this->sMainTile_BitmapID == -1 )
    Error("Invalid Ground Tex Handle");
}

//----- (0047CF9C) --------------------------------------------------------
void OutdoorLocation::Release()
{
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

  free(pSpawnPoints);
  pSpawnPoints = nullptr;
  uNumSpawnPoints = 0;

  pTerrain.Release();

  free(pCmap);
  pCmap = nullptr;
  free(pOMAP);
  pOMAP = nullptr;
  free(pFaceIDLIST);
  pFaceIDLIST = nullptr;
  free(pTerrainNormals);
  pTerrainNormals = nullptr;
}

//----- (0047D0A6) --------------------------------------------------------
bool OutdoorLocation::Load(const char *pFilename, ODMFace *File, size_t pNumItems, int *thisa)//загрузка локации
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
//  int v102; // edi@159
  //void *v103; // [sp-14h] [bp-B94h]@55
  //void *v104; // [sp-10h] [bp-B90h]@59
  //size_t v105; // [sp-Ch] [bp-B8Ch]@59
  //char *v106; // [sp-8h] [bp-B88h]@59
//  int v107; // [sp-4h] [bp-B84h]@12
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
    sSky_TextureID = pBitmaps_LOD->LoadTexture((const char *)v107, (enum TEXTURE_TYPE)v108);
    strcpy(pGroundTileset, byte_6BE124_cfg_textures_DefaultGroundTexture);
    v34 = pTileTable->GetTileById(pTileTypes[0].uTileID);
    v35 = pBitmaps_LOD->LoadTexture(v34->pTileName);
    v36 = sSky_TextureID;
    sMainTile_BitmapID = v35;
    if ( v36 != -1 )
      pBitmaps_LOD->pTextures[v36].palette_id2 = pPaletteManager->LoadPalette(pBitmaps_LOD->pTextures[v36].palette_id1);

    v37 = sMainTile_BitmapID;
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
  uchar* pSrcMem = (unsigned char *)malloc(header.uDecompressedSize);
  uchar* pSrc = pSrcMem;
  //v42 = v41;
  //HIDWORD(v142) = (uint32)pSrc;
  if (header.uCompressedSize < header.uDecompressedSize)
  {
    char* pComressedSrc = (char *)malloc(header.uCompressedSize);
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
  free(pCmap);
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
    //BSPModel* model = &pBModels[i];

    pBModels[i].pVertices.pVertices = nullptr;
    pBModels[i].pFaces = nullptr;
    pBModels[i].pFacesOrdering = nullptr;
    pBModels[i].pNodes = nullptr;
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
    uint verticesSize = pBModels[i].pVertices.uNumVertices * sizeof(Vec3_int_);
    pBModels[i].pVertices.pVertices = (Vec3_int_ *)malloc(verticesSize);
    memcpy(pBModels[i].pVertices.pVertices, pSrc, verticesSize);
    pSrc += verticesSize;
      //v51 = &pBModels[v48];
      //v108 = (int)FileName;
      //v107 = 308 * v51->uNumFaces;
      //v106 = (char *)v51->pFaces;
    assert(sizeof(ODMFace) == 308);
    uint facesSize = pBModels[i].uNumFaces * sizeof(ODMFace);
    pBModels[i].pFaces = (ODMFace *)malloc(facesSize);
    memcpy(pBModels[i].pFaces, pSrc, facesSize);
    pSrc += facesSize;
      //v52 = &pBModels[v48];
      //v108 = (int)FileName;
      //v107 = 2 * v52->uNumFaces;
      //v106 = (char *)v52->pFacesOrdering;
    uint facesOrderingSize = pBModels[i].uNumFaces * sizeof(short);
    pBModels[i].pFacesOrdering = (unsigned __int16 *)malloc(facesOrderingSize);
    memcpy(pBModels[i].pFacesOrdering, pSrc, facesOrderingSize);
    pSrc += facesOrderingSize;
      //v53 = &pBModels[v48];
      //v108 = (int)FileName;
      //v107 = 8 * v53->uNumNodes;
      //v106 = (char *)v53->pNodes;
    assert(sizeof(BSPNode) == 8);
    uint nodesSize = pBModels[i].uNumNodes * sizeof(BSPNode);
    pBModels[i].pNodes = (BSPNode *)malloc(nodesSize);
    memcpy(pBModels[i].pNodes, pSrc, nodesSize);
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
    const char* textureFilenames = (const char *)malloc(10 * pBModels[i].uNumFaces);
      //pFilename = (char *)(10 * pBModels[v48].uNumFaces);
    memcpy((char *)textureFilenames, pSrc, 10 * pBModels[i].uNumFaces);
    pSrc += 10 * pBModels[i].uNumFaces;
      //v144 = 0;
      //uSourceLen = (char *)uSourceLen + (int)pFilename;
      //v60 = pBModels;
    for (uint j = 0; j < pBModels[i].uNumFaces; ++j)
    {
      const char* texFilename = &textureFilenames[j * 10];
    //v149 = 0;
    //Str2 = (char *)ptr;

      //ODMFace* face = &pBModels[i].pFaces[j];
      //pFilename = (char *)v149 + (unsigned int)v60[v48].pFaces;
      if ( !(pBModels[i].pFaces[j].uAttributes & FACE_DONT_CACHE_TEXTURE))
      {
        v62 = pBitmaps_LOD->LoadTexture(texFilename);
//        v63 = (ODMFace *)pFilename;
        goto LABEL_68;
      }
      //v61 = pTextureFrameTable->FindTextureByName(texFilename);
      pBModels[i].pFaces[j].uTextureID = pTextureFrameTable->FindTextureByName(texFilename);
      if (!pBModels[i].pFaces[j].uTextureID)
      {
        v62 = pBitmaps_LOD->LoadTexture(texFilename);
        //v63 = (ODMFace *)pFilename;
        pBModels[i].pFaces[j].uAttributes &= ~FACE_DONT_CACHE_TEXTURE;
LABEL_68:
        pBModels[i].pFaces[j].uTextureID = v62;
        //v145 = (signed __int16)v62 != -1 ? &pBitmaps_LOD->pTextures[(signed __int16)v62] : 0;
        //v108 = ((signed __int16)v62 != -1 ? pBitmaps_LOD->pTextures[(signed __int16)v62].palette_id1 : 36);
        if ((signed __int16)v62 != -1)
          pBitmaps_LOD->pTextures[v62].palette_id2 = pPaletteManager->LoadPalette(pBitmaps_LOD->pTextures[v62].palette_id1);
        goto LABEL_69;
      }
      pTextureFrameTable->LoadAnimationSequenceAndPalettes(pBModels[i].pFaces[j].uTextureID);
LABEL_69:
      if (pBModels[i].pFaces[j].sCogTriggeredID)
      {
        if (pBModels[i].pFaces[j].HasEventHint())
          pBModels[i].pFaces[j].uAttributes |= FACE_UNKNOW;
        else
          pBModels[i].pFaces[j].uAttributes &= ~FACE_UNKNOW;
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

  memcpy(&uNumLevelDecorations, pSrc, 4);
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
  free(pFaceIDLIST);
  pFaceIDLIST = nullptr;
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
  free(pOMAP);
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
  v39 = pNew_LOD->FindContainer(Str, 1);//error
  fread(&header, 0x10u, 1, v39);
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
      void* compressedMem = malloc(header.uCompressedSize);
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

  if (dword_6BE364_game_settings_1 & GAME_SETTINGS_2000)
    pNumItems = 0x1BAF800;
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
    *thisa = 1;
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
      void* compressedMem = malloc(header.uCompressedSize);
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
  *thisa = 0;
LABEL_120:
  //v108 = (int)".odm";
  //v83 = strlen(pContainer);
  //strcpy((char *)v141 + v83, (const char *)v108);
  memcpy(uFullyRevealedCellOnMap, pSrc, 0x3C8);
  //v84 = (const void *)(v74 + 968);
  memcpy(uPartiallyRevealedCellOnMap, pSrc + 0x3C8, 0x3C8);
  pSrc += 2 * 0x3C8;
  //v85 = (char *)v84 + 968;

  pGameLoadingUI_ProgressBar->Progress();

  if ( *thisa )
  {
    memcpy(uFullyRevealedCellOnMap, Dst, 0x3C8u);
    memcpy(uPartiallyRevealedCellOnMap, Src, 0x3C8u);
  }

  for (uint i = 0; i < uNumBModels; ++i)
  {
    BSPModel model = pBModels[i];
    for (uint j = 0; j < model.uNumFaces; ++j)
    {
      ODMFace face = model.pFaces[j];
      memcpy(&face.uAttributes, pSrc, 4);
      pSrc += 4;
    }

    for (uint j = 0; j < model.uNumFaces; ++j)
    {
      ODMFace face = model.pFaces[j];
      if (face.sCogTriggeredID)
      {
        if (face.HasEventHint())
          face.uAttributes |= FACE_UNKNOW3;
        else
          face.uAttributes &= 0xFFFFEFFFu;
      }
    }
  }

  pGameLoadingUI_ProgressBar->Progress();

  for (uint i = 0; i < uNumLevelDecorations; ++i)
  {
    memcpy(&pLevelDecorations[i].uFlags, pSrc, 2);
    pSrc += 2;
  }

  pGameLoadingUI_ProgressBar->Progress();

  memcpy(&uNumActors, pSrc, 4);
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
  pTileTable->InitializeTileset(pTileTypes[0].tileset);
  pTileTable->InitializeTileset(pTileTypes[1].tileset);
  pTileTable->InitializeTileset(pTileTypes[2].tileset);
  pTileTable->InitializeTileset(pTileTypes[3].tileset);
  strcpy(pGroundTileset, byte_6BE124_cfg_textures_DefaultGroundTexture.data());
  TileDesc* v98 = pTileTable->GetTileById(pTileTypes[0].uTileID);
  sMainTile_BitmapID = pBitmaps_LOD->LoadTexture(v98->pTileName, TEXTURE_DEFAULT);
  if (sMainTile_BitmapID != -1)
    pBitmaps_LOD->pTextures[sMainTile_BitmapID].palette_id2 = pPaletteManager->LoadPalette(pBitmaps_LOD->pTextures[sMainTile_BitmapID].palette_id1);

  _47F0E2();

//LABEL_150:
  if ( pWeather->bRenderSnow ) //Ritor1: it's include for snow
    strcpy(loc_time.sky_texture_name, "sky19");
  else if (loc_time.uLastVisitDay)
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
    strcpy(loc_time.sky_texture_name, "plansky3");

  //v101 = pBitmaps_LOD->LoadTexture(field_4F8);
  sSky_TextureID = pBitmaps_LOD->LoadTexture(loc_time.sky_texture_name);
  if (sSky_TextureID != -1)
    pBitmaps_LOD->pTextures[sSky_TextureID].palette_id2 = pPaletteManager->LoadPalette(pBitmaps_LOD->pTextures[sSky_TextureID].palette_id1);

  pPaletteManager->RecalculateAll();
  pSoundList->LoadSound(53, 0);
  pSoundList->LoadSound(92, 0);
  pSoundList->LoadSound(57, 0);
  pSoundList->LoadSound(96, 0);
  pSoundList->LoadSound(64, 0);
  pSoundList->LoadSound(103, 0);
  for (int i=0; i < 3;++i) 
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
      case Tilset_Desert:
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
      if ( v3 == Tileset_3 )
        result = this->pTileTypes[3].uTileID;
      else
        result = a2;
    }
    else
      result = this->pTileTypes[v3].uTileID;
  }
  else
    result = 0;
  return result;
}

//----- (0047ED08) --------------------------------------------------------
unsigned int OutdoorLocation::DoGetTileTexture(signed int sX, signed int sY)
{
  int v3; // esi@5
//  unsigned int result; // eax@9

  assert(sX < 128 && sY < 128);

 v3 = this->pTerrain.pTilemap[sY * 128 + sX];
 if (v3 < 198) // < Tileset_3
 {
  if (v3 >= 90)
    v3 = v3 + this->pTileTypes[(v3 - 90) / 36].uTileID - 36 * ((v3 - 90) / 36) - 90;
 }
 else
   v3 = v3 + this->pTileTypes[3].uTileID - 198;

  #pragma region "New: seasons change"

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
int OutdoorLocation::ActuallyGetSomeOtherTileInfo(signed int sX, signed int sY)
{
  assert(sX < 128 && sY < 128);
  int v3; // esi@5

  v3 = this->pTerrain.pTilemap[sY * 128 + sX];
  if ( v3 >= 90 )
    v3 = v3 + this->pTileTypes[(v3 - 90) / 36].uTileID - 36 * ((v3 - 90) / 36) - 90;
  return pTileTable->pTiles[v3].uAttributes;
}

//----- (0047EE16) --------------------------------------------------------
int OutdoorLocation::DoGetHeightOnTerrain(signed int sX, signed int sZ)
{
  assert(sX < 128 && sZ < 128);

  return 32 * pTerrain.pHeightmap[sZ * 128 + sX];
}

//----- (0047EE49) --------------------------------------------------------
int OutdoorLocation::GetSoundIdByPosition( signed int X_pos, signed int Y_pos, int running )
    {
  signed int v4; // eax@5
  signed int v5; // eax@7
//  int v6; // eax@8
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
  for (int i = -10; i < 10; i++)
  {
    int currYpos = Y_grid_poa + i - 20;
    for (int j = -10; j < 10; j++)
    {
      int currXpos = X_grid_pos + j - 20;
      int distanceSquared = i * i + j * j;
      if ( distanceSquared <= 100 && currYpos >= 0 && currYpos <= 87 && currXpos >= 0 && currXpos <= 87 )
      {
        unsigned char v13 = 1 << (7 - currXpos % 8);
        this->uPartiallyRevealedCellOnMap[currYpos][currXpos / 8] |= v13;
        if ( distanceSquared <= 49 )
          this->uFullyRevealedCellOnMap[currYpos][currXpos / 8] |= v13;
      }
    }
  }
  return 1;
}


//----- (0047F04C) --------------------------------------------------------
bool OutdoorLocation::IsMapCellFullyRevealed(signed int x_pos, signed int y_pos)
{
  if ( x_pos < 0 || x_pos >= 88 || y_pos < 0 || y_pos >= 88 )
    return false;
  else
    return (uFullyRevealedCellOnMap[y_pos][x_pos/8] & (1 << (7 - (x_pos) % 8))) != 0;
}

//----- (0047F097) --------------------------------------------------------
bool OutdoorLocation::IsMapCellPartiallyRevealed(signed int x_pos, signed int y_pos)
{
  if ( x_pos < 0 || x_pos >= 88 || y_pos < 0 || y_pos >= 88 )
    return false;
  else
    return (uPartiallyRevealedCellOnMap[y_pos][x_pos/8] & (1 << (7 - (x_pos) % 8))) != 0;
}

//----- (0047F0E2) --------------------------------------------------------
bool OutdoorLocation::_47F0E2()
{
  for ( uint i = 0; i < (signed int)pBitmaps_LOD->uNumLoadedFiles; ++i )
  {
    //if ( i != -1 ? (int)&pBitmaps_LOD->pTextures[i] : 0 )
      pBitmaps_LOD->pTextures[i].uDecompressedSize = this->pTerrain._47CB57((int)pBitmaps_LOD->pTextures[i].pLevelOfDetail0_prolly_alpha_mask,
                                                    pBitmaps_LOD->pTextures[i].palette_id2,
                                                    pBitmaps_LOD->pTextures[i].uTextureWidth * pBitmaps_LOD->pTextures[i].uTextureHeight);
  }
  return 1;
}

//----- (0047F138) --------------------------------------------------------
bool OutdoorLocation::PrepareDecorations()
{
  signed int v1; // ebx@1
  signed int v8; // [sp+Ch] [bp-4h]@1

  v1 = 0;
  v8 = 0;
  if ( !_stricmp(pCurrentMapName, "out09.odm") )
    v8 = 1;

  for (uint i = 0; i < uNumLevelDecorations; ++i)
  {
    LevelDecoration* decor = &pLevelDecorations[i];

      pDecorationList->InitializeDecorationSprite(decor->uDecorationDescID);
      if ( pDecorationList->pDecorations[decor->uDecorationDescID].uSoundID && _6807E0_num_decorations_with_sounds_6807B8 < 9 )
      {
        pSoundList->LoadSound(pDecorationList->pDecorations[decor->uDecorationDescID].uSoundID, 0);
        _6807B8_level_decorations_ids[_6807E0_num_decorations_with_sounds_6807B8++] = i;
      }
      if ( v8 && decor->uCog == 20 )
        decor->uFlags |= LEVEL_DECORATION_OBELISK_CHEST;
      if ( !decor->uEventID )
      {
        if ( decor->IsInteractive() )
        {
          if ( v1 < 124 )
          {
            decor->_idx_in_stru123 = v1 + 75;
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
void OutdoorLocation::ArrangeSpriteObjects()
{
  OutdoorLocation *v5; // [sp+0h] [bp-4h]@1

  v5 = this;
  if ( (signed int)uNumSpriteObjects > 0 )
  {
    for ( int i = 0; i < (signed int)uNumSpriteObjects; ++i )
    {
      if ( pSpriteObjects[i].uObjectDescID )
      {
        if ( !(pSpriteObjects[i].uAttributes & 8) && !(pObjectList->pObjects[pSpriteObjects[i].uObjectDescID].uFlags & 0x10) )
          pSpriteObjects[i].vPosition.z = GetTerrainHeightsAroundParty2(pSpriteObjects[i].vPosition.x, pSpriteObjects[i].vPosition.y, (int *)&v5, 0);
        if ( pSpriteObjects[i].stru_24.uItemID )
        {
          if ( pSpriteObjects[i].stru_24.uItemID != 220
            && pItemsTable->pItems[pSpriteObjects[i].stru_24.uItemID].uEquipType == EQUIP_POTION
            && !pSpriteObjects[i].stru_24.uEnchantmentType )
             pSpriteObjects[i].stru_24.uEnchantmentType = rand() % 15 + 5;
          pItemsTable->SetSpecialBonus(&pSpriteObjects[i].stru_24);
        }
      }
    }
  }
  pGameLoadingUI_ProgressBar->Progress();
}

//----- (0047F2D3) --------------------------------------------------------
bool OutdoorLocation::InitalizeActors(int a1)
{
  int alert_status; // [sp+348h] [bp-8h]@1
//  int v9; // [sp+34Ch] [bp-4h]@1

  alert_status = 0;
  for( int i = 0; i < uNumActors; ++i ) 
  {
    if ( !(pActors[i].uAttributes & 0x100000) )
    {
      if ( alert_status != 1 )
      {
        pActors[i].uCurrentActionTime = 0;
        pActors[i].uCurrentActionLength = 0;
        if ( pActors[i].uAttributes & 0x10000 )
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
        pActors[i].uAttributes = pActors[i].uAttributes | 0x10000;
      }
    }
    else if ( a1 == 0 )
    {
      pActors[i].uAIState = AIState::Disabled;
      pActors[i].uAttributes = pActors[i].uAttributes | 0x10000;
    }
    else if ( alert_status != 0 )
    {
      pActors[i].uCurrentActionTime = 0;
      pActors[i].uCurrentActionLength = 0;
      if ( pActors[i].uAttributes & 0x10000 )
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
      pActors[i].uAttributes = pActors[i].uAttributes | 0x10000;
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
  int z; // esi@5
  float v4; // ST48_4@8
  unsigned int v8; // eax@11
  int v9; // edx@11
  //__int16 v10; // dx@11
  //unsigned int v11; // eax@13
  signed int v12; // eax@16
  //__int16 v13; // cx@21
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
  //RenderBillboard *v28; // esi@45
  //__int16 v29; // ax@46
//  unsigned __int8 v30; // zf@46
//  unsigned __int8 v31; // sf@46
//  signed __int16 v32; // ax@49
//  signed int v33; // ecx@50
  int v34; // ecx@54
  //MonsterDesc *v35; // edx@54
  //int v36; // ecx@54
  //unsigned __int8 v37; // zf@54
  //unsigned __int8 v38; // sf@54
  //unsigned int v39; // [sp-8h] [bp-68h]@23
  //unsigned int v40; // [sp-4h] [bp-64h]@23
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
//  int v52; // [sp+34h] [bp-2Ch]@50
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
    //Actor* actor = &pActors[i];
	  //v2 = actor->uAIState;

    pActors[i].uAttributes &= 0xFFFFFFF7u;
    if (pActors[i].uAIState == Removed || pActors[i].uAIState == Disabled)
      continue;

	  z = pActors[i].vPosition.z;
      v49 = 0;
	  x = pActors[i].vPosition.x;
	  y = pActors[i].vPosition.y;
      if (pActors[i].uAIState == Summoned)
      {
		if (PID_TYPE(pActors[i].uSummonerID) != OBJECT_Actor ||
             pActors[PID_ID(pActors[i].uSummonerID)].pMonsterInfo.uSpecialAbilityDamageDiceSides != 1 )
        {
          z += floorf(pActors[i].uActorHeight * 0.5f + 0.5f);
        }
        else
        {
          v49 = 1;
		  pGame->pStru6Instance->_4A7F74(pActors[i].vPosition.x, pActors[i].vPosition.y, z);
          v4 = (1.0 - (double)pActors[i].uCurrentActionTime / (double)pActors[i].uCurrentActionLength) *
			   (double)(2 * pActors[i].uActorHeight);
          z -= floorf(v4 + 0.5f);
          if ( z > pActors[i].vPosition.z )
            z = pActors[i].vPosition.z;
        }
      }
      v8 = stru_5C6E00->Atan2(pActors[i].vPosition.x - pGame->pIndoorCameraD3D->vPartyPos.x,
                              pActors[i].vPosition.y - pGame->pIndoorCameraD3D->vPartyPos.y);
	  LOWORD(v9) = pActors[i].uYawAngle;
      v41 = ((signed int)(stru_5C6E00->uIntegerPi + ((signed int)stru_5C6E00->uIntegerPi >> 3) + v9 - v8) >> 8) & 7;
      if ( pParty->bTurnBasedModeOn )
      {
        v12 = pActors[i].uCurrentActionTime;
        if ( pActors[i].uCurrentActionAnimation == 1 )
          v12 = 32 * i + pMiscTimer->uTotalGameTimeElapsed;
      }
      else
      {
        v12 = pActors[i].uCurrentActionTime;
        if ( pActors[i].uCurrentActionAnimation == 1 )
          v12 = 32 * i + pEventTimer->uTotalGameTimeElapsed;
      }
      if ( (signed __int64)pActors[i].pActorBuffs[ACTOR_BUFF_STONED].uExpireTime > 0 || (signed __int64)pActors[i].pActorBuffs[ACTOR_BUFF_PARALYZED].uExpireTime > 0 )
        v12 = 0;
      if ( pActors[i].uAIState == 17 && !v49 )
        v14 = pSpriteFrameTable->GetFrame(uSpriteID_Spell11, v12);
      else if ( pActors[i].uAIState == 16 )
        v14 = pSpriteFrameTable->GetFrameBy_x(pActors[i].pSpriteIDs[pActors[i].uCurrentActionAnimation], v12);
      else
        v14 = pSpriteFrameTable->GetFrame(pActors[i].pSpriteIDs[pActors[i].uCurrentActionAnimation], v12);
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
        //LOBYTE(v16) = _4E94D3_light_type;
        pMobileLightsStack->AddLight(x, y, z, 0, v15->uGlowRadius, 0xFFu, 0xFFu, 0xFFu, _4E94D3_light_type);
      }
      v17 = (x - pGame->pIndoorCameraD3D->vPartyPos.x) << 16;
      if (pGame->pIndoorCameraD3D->sRotationX)
      {
        v18 = (y - pGame->pIndoorCameraD3D->vPartyPos.y) << 16;
        v47 = (fixpoint_mul(v17, pGame->pIndoorCameraD3D->int_cosine_y) + fixpoint_mul(v18, pGame->pIndoorCameraD3D->int_sine_y));
        v50 = fixpoint_mul(v17, pGame->pIndoorCameraD3D->int_sine_y);
        v53 = fixpoint_mul(v18, pGame->pIndoorCameraD3D->int_cosine_y);
        v44 = (z - pGame->pIndoorCameraD3D->vPartyPos.z) << 16;
        v19 = (fixpoint_mul(v44, pGame->pIndoorCameraD3D->int_sine_x) + fixpoint_mul(v47, pGame->pIndoorCameraD3D->int_cosine_x));
        X = fixpoint_mul(v44, pGame->pIndoorCameraD3D->int_sine_x) + fixpoint_mul(v47, pGame->pIndoorCameraD3D->int_cosine_x);
        if ( v19 < 262144 || v19 > pODMRenderParams->shading_dist_mist << 16 )
          continue;
        v20 = v53 - v50;
        v42 = v53 - v50;
        v21 = (fixpoint_mul(v44, pGame->pIndoorCameraD3D->int_cosine_x) - fixpoint_mul(v47, pGame->pIndoorCameraD3D->int_sine_x));
      }
      else
      {
        v48 = (y - pGame->pIndoorCameraD3D->vPartyPos.y) << 16;
        v51 = fixpoint_mul(v17, pGame->pIndoorCameraD3D->int_cosine_y);
        v22 = fixpoint_mul(v48, pGame->pIndoorCameraD3D->int_sine_y);
        X = v22 + v51;
        if ( v22 + v51 < 262144 || v22 + v51 > pODMRenderParams->shading_dist_mist << 16 )
          continue;
        v23 = fixpoint_mul(((x - pGame->pIndoorCameraD3D->vPartyPos.x) << 16), pGame->pIndoorCameraD3D->int_sine_y);
        v20 = fixpoint_mul(v48, pGame->pIndoorCameraD3D->int_cosine_y) - v23;
        v42 = fixpoint_mul(v48, pGame->pIndoorCameraD3D->int_cosine_y) - v23;
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
        v27 = pViewport->uScreenCenterX - ((signed int)(fixpoint_mul(v26, v42) + 32768) >> 16);
        v43 = pViewport->uScreenCenterX - ((signed int)(fixpoint_mul(v26, v42) + 32768) >> 16);
        v46 = pViewport->uScreenCenterY - ((signed int)(fixpoint_mul(v25 / X, v45) + 32768) >> 16);
        result = uNumBillboardsToDraw;
        //v28 = &pBillboardRenderList[uNumBillboardsToDraw];
        if (uNumBillboardsToDraw >= 500)
          return;
        ++uNumBillboardsToDraw;
        ++uNumSpritesDrawnThisFrame;
        pActors[i].uAttributes |= 8;
        pBillboardRenderList[uNumBillboardsToDraw - 1].HwSpriteID = v15->pHwSpriteIDs[v41];
        pBillboardRenderList[uNumBillboardsToDraw - 1].uIndoorSectorID = 0;
        pBillboardRenderList[uNumBillboardsToDraw - 1].uPalette = v15->uPaletteIndex;
        pBillboardRenderList[uNumBillboardsToDraw - 1]._screenspace_x_scaler_packedfloat = fixpoint_mul(v15->scale, v58);
        pBillboardRenderList[uNumBillboardsToDraw - 1]._screenspace_y_scaler_packedfloat = fixpoint_mul(v15->scale, v57);
        if ( pActors[i].pActorBuffs[ACTOR_BUFF_SHRINK].uExpireTime <= 0 )
        {
          if ( pActors[i].pActorBuffs[ACTOR_BUFF_MASS_DISTORTION].uExpireTime > 0 )
          {
            pBillboardRenderList[uNumBillboardsToDraw - 1]._screenspace_y_scaler_packedfloat = fixpoint_mul(pGame->pStru6Instance->_4A806F(&pActors[i]),
                   pBillboardRenderList[uNumBillboardsToDraw - 1]._screenspace_y_scaler_packedfloat);
            LOWORD(v27) = v43;
          }
        }
        else
        {
          if ( pActors[i].pActorBuffs[ACTOR_BUFF_SHRINK].uPower )
          {
            pBillboardRenderList[uNumBillboardsToDraw - 1]._screenspace_y_scaler_packedfloat = fixpoint_mul(65536 / pActors[i].pActorBuffs[ACTOR_BUFF_SHRINK].uPower,
                     pBillboardRenderList[uNumBillboardsToDraw - 1]._screenspace_y_scaler_packedfloat);
            LOWORD(v27) = v43;
          }
        }
        pBillboardRenderList[uNumBillboardsToDraw - 1].uScreenSpaceX = v27;
        pBillboardRenderList[uNumBillboardsToDraw - 1].uScreenSpaceY = v46;
        pBillboardRenderList[uNumBillboardsToDraw - 1].world_x = x;
        pBillboardRenderList[uNumBillboardsToDraw - 1].world_y = y;
        pBillboardRenderList[uNumBillboardsToDraw - 1].world_z = z;
        HIWORD(v34) = HIWORD(X);
        LOWORD(v34) = 0;
        pBillboardRenderList[uNumBillboardsToDraw - 1].dimming_level = 0;
        pBillboardRenderList[uNumBillboardsToDraw - 1].sZValue = v34 + PID(OBJECT_Actor, i);
        pBillboardRenderList[uNumBillboardsToDraw - 1].field_14_actor_id = i;
        //v35 = pMonsterList->pMonsters;
        //v36 = pActors[i].pMonsterInfo.uID;
        pBillboardRenderList[uNumBillboardsToDraw - 1].field_1E = v62 | 0x200;
        pBillboardRenderList[uNumBillboardsToDraw - 1].pSpriteFrame = v15;
        pBillboardRenderList[uNumBillboardsToDraw - 1].sTintColor = pMonsterList->pMonsters[pActors[i].pMonsterInfo.uID - 1].sTintColor;//*((int *)&v35[v36] - 36);
        if (pActors[i].pActorBuffs[ACTOR_BUFF_STONED].uExpireTime)
          pBillboardRenderList[uNumBillboardsToDraw - 1].field_1E = v62 | 0x200;
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
//  int v14; // edx@20
  signed int v18; // edx@26
  int v19; // eax@28
//  int v20; // edx@30
//  int v21; // ST1C_4@30
  signed int v22; // edx@30
  signed __int64 v23; // qtt@30
  int v24; // eax@36
  signed int v25; // ecx@38
//  int result; // eax@42
  signed int current_floor_level; // ecx@43
//  int v28; // edi@44
  signed int v29; // edx@44
//  int v30; // esi@45
//  int v31; // eax@45
//  int v33; // ecx@59
//  int v36; // [sp+14h] [bp-2Ch]@24
//  int v38; // [sp+1Ch] [bp-24h]@2
  int v39; // [sp+20h] [bp-20h]@9
  signed int pBModelNum; // [sp+28h] [bp-18h]@1
  int pFaceNum; // [sp+2Ch] [bp-14h]@8
  bool current_vertices_Y; // [sp+30h] [bp-10h]@22
  bool next_vertices_Y; // [sp+34h] [bp-Ch]@24
  signed int v46; // [sp+3Ch] [bp-4h]@1
  signed int number_hits; // [sp+58h] [bp+18h]@22
  signed int next_floor_level; // [sp+58h] [bp+18h]@43

  v46 = 1;
  current_BModel_id[0] = -1;
  current_Face_id[0] = -1;
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
          if ( pFace->Ethereal() )
            continue;
          if ( (pFace->uPolygonType == POLYGON_Floor || pFace->uPolygonType == POLYGON_InBetweenFloorAndWall)
            && X <= pFace->pBoundingBox.x2 && X >= pFace->pBoundingBox.x1
            && Y <= pFace->pBoundingBox.y2 && Y >= pFace->pBoundingBox.y1 )
          {
            for ( uint i = 0; i < pFace->uNumVertices; ++i)
            {
              odm_floor_face_vert_coord_X[2 * i] = pFace->pXInterceptDisplacements[i] + pBModel->pVertices.pVertices[pFace->pVertexIDs[i]].x;
              odm_floor_face_vert_coord_Y[2 * i] = pFace->pYInterceptDisplacements[i] + pBModel->pVertices.pVertices[pFace->pVertexIDs[i]].y;
              odm_floor_face_vert_coord_X[2 * i + 1] = pFace->pXInterceptDisplacements[i] + pBModel->pVertices.pVertices[pFace->pVertexIDs[i + 1]].x;
              odm_floor_face_vert_coord_Y[2 * i + 1] = pFace->pYInterceptDisplacements[i] + pBModel->pVertices.pVertices[pFace->pVertexIDs[i + 1]].y;
            }
            odm_floor_face_vert_coord_X[2 * pFace->uNumVertices] = odm_floor_face_vert_coord_X[0];
            odm_floor_face_vert_coord_Y[2 * pFace->uNumVertices] = odm_floor_face_vert_coord_Y[0];

            current_vertices_Y = odm_floor_face_vert_coord_Y[0] >= Y;
            number_hits = 0;
            if ( 2 * pFace->uNumVertices > 0 )
            {
              for ( int i = 0; i < 2 * pFace->uNumVertices; ++i )
              {
                if ( number_hits >= 2 )
                  break;
                //v36 = odm_floor_face_vert_coord_Y[i + 1];
                next_vertices_Y = odm_floor_face_vert_coord_Y[i + 1] >= Y;
                if ( current_vertices_Y != next_vertices_Y )//проверка по Y
                {
                  v18 = odm_floor_face_vert_coord_X[i + 1] >= X ? 0 : 2;
                  v19 = v18 | (odm_floor_face_vert_coord_X[i] < X);
                  if ( v19 != 3 )
                  {
                    if ( !v19 )
                      ++number_hits;
                    else
                    {
                      LODWORD(v23) = (Y - odm_floor_face_vert_coord_Y[i]) << 16;
                      HIDWORD(v23) = (Y - odm_floor_face_vert_coord_Y[i]) >> 16;
                      v22 = ((((odm_floor_face_vert_coord_X[i + 1] - odm_floor_face_vert_coord_X[i]) * v23 / (odm_floor_face_vert_coord_Y[i + 1]
                              - odm_floor_face_vert_coord_Y[i])) >> 16) + odm_floor_face_vert_coord_X[i]);
                      if ( v22 >= X) 
                        ++number_hits;
                    }
                  }
                }
                current_vertices_Y = next_vertices_Y;
              }
              if ( number_hits == 1 )
              {
                if ( v46 >= 20 )
                  break;
                if ( pFace->uPolygonType == POLYGON_Floor )
                  v24 = pBModel->pVertices.pVertices[pFace->pVertexIDs[0]].z;
                else
                {
                  int a = fixpoint_mul(pFace->zCalc1, X);
                  int b = fixpoint_mul(pFace->zCalc2, Y);
                  int c = ((signed __int64)pFace->zCalc3 >> 16);
                  v24 = a + b + c;
                }
                v25 = v46++;
                odm_floor_level[v25] = v24;
                current_BModel_id[v25] = pBModelNum;
                current_Face_id[v25] = pFaceNum;
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
  current_floor_level = 0;
  v29 = 0;
  if ( v46 <= 1 )
    *bmodel_pid = 0;
  else
  {
    current_floor_level = odm_floor_level[0];
    for ( uint i = 1; i < v46; ++i )
    {
      next_floor_level = odm_floor_level[i];
      if ( current_floor_level <= Z + 5 )
      {
        if ( next_floor_level > current_floor_level && next_floor_level <= Z + 5 )
        {
          current_floor_level = next_floor_level;
          v29 = i;
        }
      }
      else if ( next_floor_level < current_floor_level )
      {
        current_floor_level = next_floor_level;
        v29 = i;
      }
    }
    if ( !v29 )
      *bmodel_pid = 0;
    else
      *bmodel_pid = current_Face_id[v29] | (current_BModel_id[v29] << 6);
  }
  if ( v29 )
  {
    *pIsOnWater = false;
    if ( pOutdoor->pBModels[current_BModel_id[v29]].pFaces[current_Face_id[v29]].Fluid())
      *pIsOnWater = true;
  }
  if ( odm_floor_level[v29] >= odm_floor_level[0] )
    odm_floor_level[0] = odm_floor_level[v29];
  return odm_floor_level[0];
}

//not sure if right- or left-handed coordinate space assumed, so this could be normal of inverse normal
// for a right-handed system, that would be an inverse normal
//----- (0046DCC8) --------------------------------------------------------
void ODM_GetTerrainNormalAt(int pos_x, int pos_z, Vec3_int_ *out)
{
  uint grid_x = WorldPosToGridCellX(pos_x);
  uint grid_z = WorldPosToGridCellZ(pos_z) - 1;

  int grid_pos_x1 = GridCellToWorldPosX(grid_x);
  int grid_pos_x2 = GridCellToWorldPosX(grid_x + 1);
  int grid_pos_z1 = GridCellToWorldPosZ(grid_z);
  int grid_pos_z2 = GridCellToWorldPosZ(grid_z + 1);

  int x1z1_y = pOutdoor->DoGetHeightOnTerrain(grid_x, grid_z);
  int x2z1_y = pOutdoor->DoGetHeightOnTerrain(grid_x + 1, grid_z);
  int x2z2_y = pOutdoor->DoGetHeightOnTerrain(grid_x + 1, grid_z + 1);
  int x1z2_y = pOutdoor->DoGetHeightOnTerrain(grid_x, grid_z + 1);

  float side1_dx, side1_dy, side1_dz,
        side2_dx, side2_dy, side2_dz;

  int dx = abs(pos_x - grid_pos_x1),
       dz = abs(grid_pos_z1 - pos_z);
  if (dz >= dx)
  {
    side2_dx = (double)(grid_pos_x2 - grid_pos_x1);
    side2_dz = 0.0;//(double)(grid_pos_z2 - grid_pos_z2);  // bug?  z2 - z2 
    side2_dy = (double)(x2z2_y - x1z2_y);

    side1_dx = 0.0;//(double)(grid_pos_x1 - grid_pos_x1);
    side1_dz = (double)(grid_pos_z1 - grid_pos_z2);  //       z1 - z2 yes
    side1_dy = (double)(x1z1_y - x1z2_y);
    //Log::Warning(L"%S %S %u\n", __FILE__, __FUNCTION__, __LINE__);
    /*       |\
       side1 |  \
             |____\
             side 2      */
  }
  else
  {
    side2_dx = (double)(grid_pos_x1 - grid_pos_x2);
    side2_dz = 0.0;//(double)(grid_pos_z1 - grid_pos_z1);
    side2_dy = (double)(x1z1_y - x2z1_y);

    side1_dx = 0.0;//(double)(grid_pos_x2 - grid_pos_x1);
    side1_dz = (double)(grid_pos_z2 - grid_pos_z1);
    side1_dy = (double)(x2z2_y - x2z1_y);
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
void OutdoorLocation::LoadActualSkyFrame()
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
  for ( uint i = 0; i < uNumVertices; i++ )
   {
    memcpy(&array_50AC10[i], &array_507D30[i], sizeof(array_50AC10[i]));
    array_50AC10[i].vWorldViewProjX = (double)pViewport->uScreenCenterX 
        - ((double)pODMRenderParams->int_fov_rad * array_507D30[i]._rhw) * array_507D30[i].vWorldViewPosition.y;
    array_50AC10[i].vWorldViewProjY = (double)pViewport->uScreenCenterY
        - ((double)pODMRenderParams->int_fov_rad * array_507D30[i]._rhw) * array_507D30[i].vWorldViewPosition.z;
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
//----- (00473893) --------------------------------------------------------
void ODM_ProcessPartyActions()
{
  int v1; // edi@1
  int v2; // ebx@1
  int floor_level; // eax@14
  int v6; // esi@45
  ODMFace *face; // ecx@45
  //signed int v33; // eax@143
  int v34; // esi@143
  int v35; // esi@147
  int v36; // eax@155
//  signed int v37; // esi@159
//  signed int v38; // eax@159
//  signed int i; // esi@159
  int v40; // esi@162
  bool v42; // eax@180
  signed int v43; // ecx@184
  signed int v44; // edx@184
  int v45; // ecx@200
  BSPModel *pModel; // eax@203
  bool pModel_;
  ODMFace *pODMFace; // esi@203
  int v48; // eax@203
  int v54; // eax@215
  int v55; // eax@217
//  BSPModel *v58; // eax@228
  //unsigned __int64 v59; // qax@228
//  BSPModel *v60; // eax@228
  //unsigned __int64 v61; // qax@228
//  int v62; // eax@241
  unsigned int v66; // esi@263
  signed int v68; // ecx@263
  int v69; // eax@263
//  unsigned int v76; // edi@293
  bool v77; // edx@297
  bool v78; // ecx@303
  int v79; // ecx@314
  __int16 v80; // dx@317
  //int v81; // ebx@318
  //int v82; // ecx@318
  int pTerrainHeight; // eax@321
//  int v86; // [sp-20h] [bp-B4h]@246
  int v87; // [sp-20h] [bp-B4h]@248
//  signed int v88; // [sp-1Ch] [bp-B0h]@246
//  unsigned int v89; // [sp-18h] [bp-ACh]@246
//  signed int v90; // [sp-14h] [bp-A8h]@246
//  signed int v91; // [sp-10h] [bp-A4h]@246
//  int v92; // [sp-Ch] [bp-A0h]@246
//  unsigned int v94; // [sp-8h] [bp-9Ch]@246
//  int v96; // [sp-4h] [bp-98h]@246
  int v97; // [sp+Ch] [bp-88h]@180
  Vec3_int_ v98;
  bool not_high_fall; // [sp+1Ch] [bp-78h]@33
  int v102; // [sp+20h] [bp-74h]@1
  int trigger_id; // [sp+24h] [bp-70h]@1
  bool bFeatherFall; // [sp+28h] [bp-6Ch]@4
  int bonus;
  int on_ground; // [sp+2Ch] [bp-68h]@24
  bool bWaterWalk; // [sp+30h] [bp-64h]@1
  int ceiling_height; // [sp+3Ch] [bp-58h]@28
  int v110; // [sp+40h] [bp-54h]@180
  int v111; // [sp+44h] [bp-50h]@14
  bool hovering; // [sp+48h] [bp-4Ch]@1
  int v113; // [sp+4Ch] [bp-48h]@1
  bool party_running_flag; // [sp+50h] [bp-44h]@1
  int _walk_speed; // [sp+54h] [bp-40h]@48
  int pX; // [sp+58h] [bp-3Ch]@1
  int pY; // [sp+5Ch] [bp-38h]@1
  int v118; // [sp+60h] [bp-34h]@1
  int _angle_x; // [sp+68h] [bp-2Ch]@48
  unsigned int v122; // [sp+70h] [bp-24h]@180
  int pZ; // [sp+74h] [bp-20h]@1
  bool party_walking_flag; // [sp+78h] [bp-1Ch]@1
  int _angle_y; // [sp+7Ch] [bp-18h]@48
  int v128; // [sp+88h] [bp-Ch]@1
  int v129; // [sp+8Ch] [bp-8h]@92

  v1 = 0;
  trigger_id = 0;
  v2 = 0;
  //*(float *)&v128 = 0.0;
  int fall_speed = pParty->uFallSpeed;
  v128 = 0;
  v129 = 0;
  pX = pParty->vPosition.x;
  pY = pParty->vPosition.y;
  pZ = pParty->vPosition.z;
  v113 = pParty->field_6F0;
  hovering = false;
  bool partyAtHighSlope = IsTerrainSlopeTooHigh(pParty->vPosition.x, pParty->vPosition.y);
  party_running_flag = false;
  party_walking_flag = false;
  v102 = 0;
  pModel_ = false;
  bWaterWalk = false;

  if (!pParty->FeatherFallActive())//Проверка падение пера
  {
    bFeatherFall = false;
    for (int i = 0; i < 4; ++i)
      if (pParty->pPlayers[i].WearsItemAnyWhere(ITEM_ARTIFACT_LADYS_ESCORT))  // seems like flying boots
      {
        bFeatherFall = true;
        break;
      }
  }
  else
    bFeatherFall = true;

  pParty->uFlags &= ~PARTY_FLAGS_1_STANDING_ON_WATER;
  if (pParty->WaterWalkActive())//Проверка хождения по воде
  {
    //LOBYTE(pParty->uFlags) &= 0x7Fu;
    bWaterWalk = true;
    *(short *)&stru_5E4C90_MapPersistVars._decor_events[20 * pParty->pPartyBuffs[PARTY_BUFF_WATER_WALK].uOverlayID + 119] |= 1u;
    if (!(pParty->pPartyBuffs[PARTY_BUFF_WATER_WALK].uFlags & 1) &&
          pParty->pPlayers[pParty->pPartyBuffs[PARTY_BUFF_WATER_WALK].uCaster - 1].sMana <= 0 )
      bWaterWalk = false;
  }

  int bmodel_standing_on_pid;
  int is_on_water = false;
  floor_level = ODM_GetFloorLevel(pX, pY, pZ, pParty->uPartyHeight, &is_on_water, &bmodel_standing_on_pid, bWaterWalk);
  int is_not_on_bmodel = bmodel_standing_on_pid == 0;

  v111 = floor_level;
  if ( bFeatherFall )
    pParty->uFallStartY = floor_level;
  else
    floor_level = pParty->uFallStartY;

  if ( floor_level - pZ > 512 && !bFeatherFall && pZ <= v111 + 1 )//падение на 3D Model
  {
    if (pParty->uFlags & PARTY_FLAGS_1_LANDING)
      pParty->uFlags &= ~PARTY_FLAGS_1_LANDING;
    else for (int i = 0; i < 4; ++i)     // receive falling damage
    {
      if ( !pParty->pPlayers[i].HasEnchantedItemEquipped(72) && !pParty->pPlayers[i].WearsItem(ITEM_ARTIFACT_HERMES_SANDALS, EQUIP_BOOTS) )
      {
        pParty->pPlayers[i].ReceiveDamage(
            (signed int)((pParty->uFallStartY - pZ) * (unsigned __int64)(pParty->pPlayers[i].GetMaxHealth() / 10)) / 256, DMGT_PHISYCAL);
        bonus = 20 - pParty->pPlayers[i].GetParameterBonus(pParty->pPlayers[i].GetActualEndurance());
        pParty->pPlayers[i].SetRecoveryTime((signed __int64)((double)bonus * flt_6BE3A4_debug_recmod1 * 2.133333333333333));
      }
    }
  }

  ceiling_height = -1;
  if ( pParty->bFlying )
    ceiling_height = GetCeilingHeight(pX, pY, pZ + pParty->uPartyHeight, (int)&v102);//высота потолка
  //v107 = bmodel_standing_on_pid == 0;
  on_ground = v111 + 1;
  if ( pZ <= on_ground )//полёт: посадка
  {
    ceiling_height = -1;
    pParty->bFlying = false;
  }
  else
    hovering = true;
  not_high_fall = pZ - v111 <= 32;

  if ( bWalkSound && pParty->walk_sound_timer)//timer update
  {
    if (pParty->walk_sound_timer >= pEventTimer->uTimeElapsed)
      pParty->walk_sound_timer -= pEventTimer->uTimeElapsed;
    else
      pParty->walk_sound_timer = 0;
  }

  if (!bUnderwater && pParty->pPartyBuffs[PARTY_BUFF_FLY].uExpireTime <= 0)// конец действия полёта
    pParty->bFlying = false;

  if (!hovering)//
  {
    if ( pParty->floor_face_pid != PID(OBJECT_BModel, bmodel_standing_on_pid) )
    {
      if (bmodel_standing_on_pid)
      {
        if ( (bmodel_standing_on_pid >> 6) < pOutdoor->uNumBModels )
        {
          face = pOutdoor->pBModels[bmodel_standing_on_pid >> 6].pFaces;
          v6 = bmodel_standing_on_pid & 0x3F;
          /*if ( *(char *)(v7->pFacePlane.vNormal.x + 308 * v6 + 31) & 4 )
          {
            pParty->field_6F4_packedid = PID(OBJECT_BModel,v108);
            v103 = *(short *)(v7->pFacePlane.vNormal.x + 308 * v6 + 292);
          }*/
          if ( face[v6].uAttributes & FACE_PRESSURE_PLATE )
          {
            pParty->floor_face_pid = PID(OBJECT_BModel, bmodel_standing_on_pid);
            trigger_id = face[v6].sCogTriggeredID; //
          }
        }
      }
    }
    pParty->floor_face_pid = PID(OBJECT_BModel, bmodel_standing_on_pid);
  }

  _walk_speed = pParty->uWalkSpeed;
  _angle_y = pParty->sRotationY;
  _angle_x = pParty->sRotationX;
  //v126 = pEventTimer->dt_in_some_format;
  /*v119 = (Player **)((unsigned __int64)(pEventTimer->dt_in_some_format
                                      * (signed __int64)((signed int)(pParty->field_20_prolly_turn_speed
                                                                    * stru_5C6E00->uIntegerPi)
                                                       / 180)) >> 16);*/
  __int64 dturn = (unsigned __int64)(pEventTimer->dt_in_some_format * (signed __int64)((signed int)(pParty->y_rotation_speed * stru_5C6E00->uIntegerPi) / 180)) >> 16;
  while (pPartyActionQueue->uNumActions)
  {
    switch (pPartyActionQueue->Next())
    {
      case PARTY_FlyUp://полёт вверх
      {
        if (!pParty->FlyActive() && !bUnderwater)
          break;

        pParty->bFlying = false;
        if (bUnderwater ||
            pParty->pPartyBuffs[PARTY_BUFF_FLY].uFlags & 1 ||
            pParty->pPlayers[pParty->pPartyBuffs[PARTY_BUFF_FLY].uCaster - 1].sMana > 0 )
        {
          extern int max_flight_height;
          if ( pParty->vPosition.z < max_flight_height || hovering )
          {
            pZ += 30;
            v113 += 30;
            pParty->bFlying = true;
            if ( pZ > max_flight_height )
            {
              pZ = max_flight_height;
              v113 = max_flight_height;
            }
            v1 = 0;
            v2 = 0;
            fall_speed = 0;
            *(float *)&v128 = 0.0;
            if ( v102 && pZ < ceiling_height && (signed int)(pParty->uPartyHeight + pZ) >= ceiling_height )//столкновение с потолком
            {
              pParty->field_6E0 = 0;
              pParty->field_6E4 = 0;
              pPartyActionQueue->uNumActions = 0;
              pParty->uFlags |= PARTY_FLAGS_1_LANDING;
              pParty->vPosition.z = ceiling_height - pParty->uPartyHeight - 31;
              pParty->field_6F0 = pZ;
              pParty->bFlying = false;
              pZ = ceiling_height - pParty->uPartyHeight - 31;
              v113 = pParty->field_6F0;
            }
            pParty->uFallSpeed = 0;
            pModel_ = true;
          }
        }
      }
      break;

      case PARTY_FlyDown://полёт вниз
        if (pParty->FlyActive() || bUnderwater)
        {
          pParty->bFlying = false;
          if ( bUnderwater
            || pParty->pPartyBuffs[PARTY_BUFF_FLY].uFlags & 1
            || pParty->pPlayers[pParty->pPartyBuffs[PARTY_BUFF_FLY].uCaster - 1].sMana > 0 )//*(int *)&pParty->pArtifactsFound[6972 * pParty->pPartyBuffs[PARTY_BUFF_FLY].uCaster + 10] > 0 )
          {
            pZ -= 30;
            v113 -= 30;
            pParty->uFallSpeed = 0;
            fall_speed = 0;
            pParty->bFlying = true;
            pModel_ = true;
            if ( pZ <= v111 )
            {
              pParty->bFlying = false;
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

      case PARTY_StrafeLeft://хождение боком в влево
      {
        *(float *)&v128 = pParty->uWalkSpeed;

        float sin_y = sinf(2 * 3.141592653589 * _angle_y / 2048.0);
        int dx = sin_y * pParty->uWalkSpeed * fWalkSpeedMultiplier;
        v2 -= 3 * dx / 4;
        
        float cos_y = cosf(2 * 3.141592653589 * _angle_y / 2048.0);
        int dy = cos_y * pParty->uWalkSpeed * fWalkSpeedMultiplier;
        v1 += 3 * dy / 4;

        v128 = v1;
        party_walking_flag = true;
      }
      break;

      case PARTY_StrafeRight://хождение боком в вправо
      {
        *(float *)&v128 = pParty->uWalkSpeed;

        float sin_y = sinf(2 * 3.141592653589 * _angle_y / 2048.0);
        int dx = sin_y * pParty->uWalkSpeed * fWalkSpeedMultiplier;
        v2 += 3 * dx / 4;
        
        float cos_y = cosf(2 * 3.141592653589 * _angle_y / 2048.0);
        int dy = cos_y * pParty->uWalkSpeed * fWalkSpeedMultiplier;
        v1 -= 3 * dy / 4;

        v128 = v1;
        party_walking_flag = true;
      }
      break;

      case PARTY_WalkForward:// идти вперёд
      {
        *(float *)&v128 = _walk_speed;

        float sin_y = sinf(2 * 3.141592653589 * _angle_y / 2048.0),
              cos_y = cosf(2 * 3.141592653589 * _angle_y / 2048.0);

        int dx = cos_y * pParty->uWalkSpeed * fWalkSpeedMultiplier;
        int dy = sin_y * pParty->uWalkSpeed * fWalkSpeedMultiplier;

        if ( new_speed )
        {
          v2 += dx * 12;
          v1 += dy * 12;
        }
        else
        {
          v2 += dx;
          v1 += dy;
        }

        v128 = v1;
        party_walking_flag = true;
      }
      break;

      case PARTY_RunForward://бежать вперёд
      {
        *(float *)&v128 = _walk_speed;

        float sin_y = sinf(2 * 3.141592653589 * _angle_y / 2048.0);
        float cos_y = cosf(2 * 3.141592653589 * _angle_y / 2048.0);

        int dx = cos_y * pParty->uWalkSpeed * fWalkSpeedMultiplier;
        int dy = sin_y * pParty->uWalkSpeed * fWalkSpeedMultiplier;

        if (pParty->bFlying)//лететь вперёд
        {
          v2 += 4 * dx;
          v1 += 4 * dy;

          v128 = v1;
        }
        else if (partyAtHighSlope && !bmodel_standing_on_pid)//сбегание со склона
        {
          v2 += dx;
          v1 += dy;

          v128 = v1;
          party_walking_flag = true;
        }
        else
        {
          /*v2 += (unsigned __int64)(stru_5C6E00->Cos(_angle_y)
                           * (signed __int64)(signed int)(2 * (unsigned __int64)(signed __int64)((double)_walk_speed * fWalkSpeedMultiplier))) >> 16;
          v1 += (unsigned __int64)((signed int)stru_5C6E00->Sin(_angle_y)
                           * (signed __int64)(signed int)(2 * (unsigned __int64)(signed __int64)((double)_walk_speed * fWalkSpeedMultiplier))) >> 16;*/

          v2 += 2 * dx;
          v1 += 2 * dy;
          
          v128 = v1;
          party_running_flag = true;
        }
      }
      break;


      case PARTY_WalkBackward://идти назад
      {
        *(float *)&v128 = _walk_speed;

        float sin_y = sinf(2 * 3.141592653589 * _angle_y / 2048.0),
              cos_y = cosf(2 * 3.141592653589 * _angle_y / 2048.0);

        int dx = cos_y * pParty->uWalkSpeed * fBackwardWalkSpeedMultiplier;
        v2 -= dx;
        
        int dy = sin_y * pParty->uWalkSpeed * fBackwardWalkSpeedMultiplier;
        v1 -= dy;

        v128 = v1;
        party_walking_flag = true;
      }
      break;


      case PARTY_RunBackward://бежать назад
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
          party_walking_flag = true;
        }
      }
      break;

      case PARTY_CenterView://смотреть прямо
        _angle_x = 0;
      break;

      case PARTY_LookUp://смотреть вверх
        _angle_x += (signed __int64)(flt_6BE150_look_up_down_dangle * 25.0);
        if ( _angle_x > 128 )
          _angle_x = 128;
        if (uActiveCharacter)
          pPlayers[uActiveCharacter]->PlaySound(SPEECH_63, 0);
      break;

      case PARTY_LookDown://смотреть вниз
        _angle_x += (signed __int64)(flt_6BE150_look_up_down_dangle * -25.0);
        if ( _angle_x < -128 )
          _angle_x = -128;
        if (uActiveCharacter)
          pPlayers[uActiveCharacter]->PlaySound(SPEECH_64, 0);
      break;

      case PARTY_Jump://прыжок
        if ( (!partyAtHighSlope || bmodel_standing_on_pid) && !hovering && pParty->field_24 && !(pParty->uFlags & 4) && !(pParty->uFlags & 0x200) )
        {
          //v126 = pParty->field_24 << 6;
          hovering = true;
          fall_speed = (signed __int64)((double)(pParty->field_24 << 6) * 1.5 + (double)fall_speed);
        }
      break;

      case PARTY_Land://приземление(клавиша Home)
        if (pParty->bFlying)
        {
          pParty->uFlags |= PARTY_FLAGS_1_LANDING;
          pParty->uFallSpeed = 0;
        }
        pParty->bFlying = false;
        pPartyActionQueue->uNumActions = 0;
      break;

      default:
        assert(false);


    }
  }

  pParty->sRotationY = _angle_y;
  pParty->sRotationX = _angle_x;
  //-------------------------------------------
  if ( pParty->bFlying )
  {
    v129 = fixpoint_mul(4, stru_5C6E00->Cos(GetTickCount()));
    pZ = v113 + v129;
    if ( pModel_ )
      pZ = v113; //взлетание выше
    if (pParty->FlyActive())
      stru_5E4C90_MapPersistVars._decor_events[20 * pParty->pPartyBuffs[PARTY_BUFF_FLY].uOverlayID + 119] &= 0xFE;
    pParty->uFallStartY = pZ;
  }
  else if ( pZ < v111 )// подъём в горку
  {
    if ( is_on_water && fall_speed )
      SpriteObject::sub_42F960_create_object(pX, pY, v111);
    fall_speed = 0;
    pZ = v111;
    pParty->uFallStartY = v111;
    v113 = pZ;
    if (pParty->FlyActive())
      stru_5E4C90_MapPersistVars._decor_events[20 * pParty->pPartyBuffs[PARTY_BUFF_FLY].uOverlayID + 119] |= 1;
  }
  else
  {
    v113 = pZ;
    if (pParty->FlyActive())
      stru_5E4C90_MapPersistVars._decor_events[20 * pParty->pPartyBuffs[PARTY_BUFF_FLY].uOverlayID + 119] |= 1;
  }
  //------------------------------------------
  if (hovering && !pParty->bFlying)//расчёт скорости падения
  {
    //v33 = -(pEventTimer->uTimeElapsed * GetGravityStrength());
    v34 = fall_speed + 2 * -(pEventTimer->uTimeElapsed * GetGravityStrength());
    fall_speed += 2 * -(pEventTimer->uTimeElapsed * GetGravityStrength());
  }
  else if (!partyAtHighSlope)
    v34 = fall_speed;
  else if (!hovering)
  {
    if ( !bmodel_standing_on_pid )
    {
      // rolling down the hill
      // how it's done: you get a little bit pushed in the air along terrain normal, getting in the air
      // and falling to the gravity, gradually sliding downwards. nice trick
      pZ = v111;
      ODM_GetTerrainNormalAt(pX, pY, &v98);
      v35 = fall_speed + (8 * -(pEventTimer->uTimeElapsed * GetGravityStrength()));
      v129 = abs(v2 * v98.x + v1 * v98.y + v35 * v98.z) >> 16;
      v2 += fixpoint_mul(v129, v98.x);
      v1 += fixpoint_mul(v129, v98.y);
      v34 = v35 + fixpoint_mul(v129, v98.z);
      v128 = v1;
      fall_speed = v34;
    }
  }
  else
    v34 = fall_speed;

  if ( hovering )//блок для крика падения
  {
    if ( !bUnderwater && v34 <= 0)
    {
      if ( v34 < -500 && !pParty->bFlying && pParty->vPosition.z - v111 > 1000 && !pParty->FeatherFallActive())
      { // falling scream
        for (int i = 0; i < 4; ++i)
        {
          if (!pParty->pPlayers[i].HasEnchantedItemEquipped(72)
           && !pParty->pPlayers[i].WearsItem(ITEM_ARTIFACT_HERMES_SANDALS, EQUIP_BOOTS)
           && pParty->pPlayers[i].CanAct())
            pParty->pPlayers[i].PlaySound(SPEECH_Falling_scream, 0);//крик падения
        }
      }
    }
  }
  else
    pParty->uFallStartY = pZ;

  if ( v2 * v2 + v1 * v1 < 400 && !partyAtHighSlope )
  {
    *(float *)&v128 = 0.0;
    v2 = 0;
  }
//--(столкновения)-------------------------------------------------------------------
  stru_721530.field_84 = -1;
  stru_721530.field_70 = 0;
  stru_721530.prolly_normal_d = pParty->field_14_radius;
  stru_721530.field_8_radius = pParty->field_14_radius / 2;
  stru_721530.field_0 = 1;
  stru_721530.height = pParty->uPartyHeight - 32;
  for ( uint i = 0; i < 100; i++ )
  {
    stru_721530.position.x = pX;
    stru_721530.position.y = pY;
    stru_721530.position.z = stru_721530.height + pZ + 1;

    stru_721530.normal.x = pX;
    stru_721530.normal.y = pY;
    stru_721530.normal.z = stru_721530.prolly_normal_d + pZ + 1;

    stru_721530.velocity.x = v2;
    stru_721530.velocity.y = v128;
    stru_721530.velocity.z = fall_speed;

    stru_721530.uSectorID = 0;
    v36 = 0;
    if ( pParty->bTurnBasedModeOn == true && pTurnEngine->turn_stage == TE_MOVEMENT )
      v36 = 13312;
    if ( stru_721530._47050A(v36) )
      break;
    _46E889_collide_against_bmodels(1);
    //v37 = WorldPosToGridCellZ(pParty->vPosition.y);
    //v38 = WorldPosToGridCellX(pParty->vPosition.x);
    _46E26D_collide_against_sprites(WorldPosToGridCellX(pParty->vPosition.x), WorldPosToGridCellZ(pParty->vPosition.y));
    _46ED8A_collide_against_sprite_objects(4);
    for ( uint actor_id = 0; actor_id < (signed int)uNumActors; ++actor_id )
      Actor::_46DF1A_collide_against_actor(actor_id, 0);
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
      //pModel = (BSPModel *)fixpoint_mul(stru_721530.field_7C, stru_721530.direction.z);
      v40 = fixpoint_mul(stru_721530.field_7C, stru_721530.direction.z) + pZ;
    }
    v122 = v40;
    ODM_GetFloorLevel(_angle_x, _angle_y, v40, pParty->uPartyHeight, &is_on_water, &bmodel_standing_on_pid, 0);
    v129 = ODM_GetFloorLevel(_angle_x, pY, v40, pParty->uPartyHeight, &is_on_water, &v97, 0);
    int v119 = ODM_GetFloorLevel(pX, _angle_y, v40, pParty->uPartyHeight, &is_on_water, &v110, 0);
    bool v42_ = (BSPModel *)IsTerrainSlopeTooHigh(_angle_x, pY);
    v42 = IsTerrainSlopeTooHigh(pX, _angle_y);
    is_not_on_bmodel = false;
    //v118 = v42;
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
      if ( v42_ && v129 > pZ )
        v44 = 0;
      if ( v42 && v119 > pZ )
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
        int new_ = ODM_GetFloorLevel(_angle_x, _angle_y, v40, pParty->uPartyHeight, &is_on_water, &bmodel_standing_on_pid, 0);
        if ( IsTerrainSlopeTooHigh(_angle_x, _angle_y) && new_ <= pZ )
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
      viewparams->bRedrawGameUI = true;
    }

    if ( PID_TYPE(stru_721530.uFaceID) == OBJECT_Decoration)
    {
      v129 = stru_5C6E00->Atan2(_angle_x - pLevelDecorations[(signed int)stru_721530.uFaceID >> 3].vPosition.x,
                               _angle_y - pLevelDecorations[(signed int)stru_721530.uFaceID >> 3].vPosition.y);
      v2 = fixpoint_mul(stru_5C6E00->Cos(v129), integer_sqrt(v2 * v2 + v128 * v128));
      v122 = fixpoint_mul(stru_5C6E00->Sin(v129), integer_sqrt(v2 * v2 + v128 * v128));
      v128 = fixpoint_mul(stru_5C6E00->Sin(v129), integer_sqrt(v2 * v2 + v128 * v128));
    }

    if ( PID_TYPE(stru_721530.uFaceID) == OBJECT_BModel)
    {
      pParty->bFlying = false;
      pModel = &pOutdoor->pBModels[(signed int)stru_721530.uFaceID >> 9];
      pODMFace = &pModel->pFaces[((signed int)stru_721530.uFaceID >> 3) & 0x3F];
      v48 = pODMFace->pBoundingBox.z2 - pODMFace->pBoundingBox.z1;
      v129 = v48 <= 32;
      v119 = pODMFace->pFacePlane.vNormal.z < 46378;
      if ( bUnderwater == 1 )
        v119 = 0;
      if ( pODMFace->uPolygonType == POLYGON_Floor )
      {
        if ( fall_speed < 0 )
          fall_speed = 0;
        pZ = pModel->pVertices.pVertices[pODMFace->pVertexIDs[0]].z + 1;
        if ( v2 * v2 + v128 * v128 < 400 )
        {
          v2 = 0;
          *(float *)&v128 = 0.0;
        }
        if ( pParty->floor_face_pid != v45 && pODMFace->Pressure_Plate() )
        {
          pParty->floor_face_pid = v45;
          trigger_id = pODMFace->sCogTriggeredID; 
        }
      }
      if ( !v129 && (pODMFace->uPolygonType != POLYGON_InBetweenFloorAndWall || v119) )// упёрся в столб
      {
        v118 = abs(v128 * pODMFace->pFacePlane.vNormal.y + fall_speed * pODMFace->pFacePlane.vNormal.z
             + v2 * pODMFace->pFacePlane.vNormal.x) >> 16;
        if ((stru_721530.speed >> 3) > v118 )
          v118 = stru_721530.speed >> 3;
        v2 += fixpoint_mul(v118, pODMFace->pFacePlane.vNormal.x);
        v128 += fixpoint_mul(v118, pODMFace->pFacePlane.vNormal.y);
        v54 = 0;
        if ( !v119 )
          v54 = fixpoint_mul(v118, pODMFace->pFacePlane.vNormal.z);
        pParty->uFallSpeed += v54;
        v55 = stru_721530.prolly_normal_d - ((signed int)(pODMFace->pFacePlane.dist + v122 * pODMFace->pFacePlane.vNormal.z
                    + _angle_y * pODMFace->pFacePlane.vNormal.y + _angle_x * pODMFace->pFacePlane.vNormal.x) >> 16);
        if ( v55 > 0 )
        {
          pX = _angle_x + fixpoint_mul(pODMFace->pFacePlane.vNormal.x, v55);
          pY = _angle_y + fixpoint_mul(pODMFace->pFacePlane.vNormal.y, v55);
          if ( !v119 )
            pZ = v122 + fixpoint_mul(pODMFace->pFacePlane.vNormal.z, v55);
        }
        if ( pParty->floor_face_pid != stru_721530.uFaceID && pODMFace->Pressure_Plate() )
        {
          pParty->floor_face_pid = stru_721530.uFaceID;
          trigger_id = pODMFace->sCogTriggeredID; //
        }
      }
      if ( pODMFace->uPolygonType == POLYGON_InBetweenFloorAndWall )
      {
        v118 = abs(v128 * pODMFace->pFacePlane.vNormal.y
             + fall_speed * pODMFace->pFacePlane.vNormal.z + v2 * pODMFace->pFacePlane.vNormal.x) >> 16;
        if ((stru_721530.speed >> 3) > v118 )
          v118 = stru_721530.speed >> 3;
        v2 += fixpoint_mul(v118, pODMFace->pFacePlane.vNormal.x);
        v128 += fixpoint_mul(v118, pODMFace->pFacePlane.vNormal.y);
        fall_speed += fixpoint_mul(v118, pODMFace->pFacePlane.vNormal.z);
        if ( v2 * v2 + v128 * v128 >= 400 )
        {
          if ( pParty->floor_face_pid != stru_721530.uFaceID && pODMFace->Pressure_Plate() )
          {
            pParty->floor_face_pid = stru_721530.uFaceID;
            trigger_id = pODMFace->sCogTriggeredID; //
          }
        }
        else
        {
          v2 = 0;
          fall_speed = 0;
          *(float *)&v128 = 0.0;
        }
      }
    }
    v2 = fixpoint_mul(58500, v2);
    v128 = fixpoint_mul(58500, v128);
    v122 = fixpoint_mul(58500, v122);
    fall_speed = fixpoint_mul(58500, fall_speed);
  }

  //Воспроизведение звуков ходьбы/бега------------------------
  uint pX_ = abs(pParty->vPosition.x - pX);
  uint pY_ = abs(pParty->vPosition.y - pY);
  uint pZ_ = abs(pParty->vPosition.z - pZ);
  if ( bWalkSound && pParty->walk_sound_timer <= 0 )
  {
    pAudioPlayer->_4AA258(804);//stop sound
    if ( party_running_flag && (!hovering || not_high_fall) )
    {
      if ( integer_sqrt(pX_ * pX_ + pY_ * pY_ + pZ_ * pZ_) >= 16 )
      {
        if ( !is_not_on_bmodel && pOutdoor->pBModels[pParty->floor_face_pid >> 9].pFaces[(pParty->floor_face_pid >> 3) & 0x3F].Visible() )
          pAudioPlayer->PlaySound(SOUND_RunAlong3DModel, 804, 1, -1, 0, 0, 0, 0);//бег на 3D Modelи
        else
        {
          v87 = pOutdoor->GetSoundIdByPosition(WorldPosToGridCellX(pParty->vPosition.x), WorldPosToGridCellZ(pParty->vPosition.y) - 1, 1);
          pAudioPlayer->PlaySound((SoundID)v87, 804, 1, -1, 0, 0, 0, 0);//бег по земле 56
        }
        pParty->walk_sound_timer = 96;//таймер для бега
      }
    }
    else if( party_walking_flag && (!hovering || not_high_fall) )
    {
      if ( integer_sqrt(pX_ * pX_ + pY_ * pY_ + pZ_ * pZ_) >= 8 )
      {
        if ( !is_not_on_bmodel && pOutdoor->pBModels[pParty->floor_face_pid >> 9].pFaces[(pParty->floor_face_pid >> 3) & 0x3F].Visible() )
          pAudioPlayer->PlaySound(SOUND_WalkAlong3DModel, 804, 1, -1, 0, 0, 0, 0);// хождение на 3D Modelи
        else
        {
          v87 = pOutdoor->GetSoundIdByPosition(WorldPosToGridCellX(pParty->vPosition.x), WorldPosToGridCellZ(pParty->vPosition.y) - 1, 0);
          pAudioPlayer->PlaySound((SoundID)v87, 804, 1, -1, 0, 0, 0, 0);// хождение по земле
        }
        pParty->walk_sound_timer = 144;//таймер для ходьбы
      }
    }
  }
  if ( integer_sqrt(pX_ * pX_ + pY_ * pY_ + pZ_ * pZ_) < 8 )//отключить  звук ходьбы при остановке
    pAudioPlayer->_4AA258(804);
//------------------------------------------------------------------------
  if ( !hovering || !not_high_fall )//  или не высокое падение
    pParty->uFlags &= ~PARTY_FLAGS_1_FALLING;
  else
    pParty->uFlags |= PARTY_FLAGS_1_FALLING;
  int pMap_X = WorldPosToGridCellX(pParty->vPosition.x);
  int pMap_Y = WorldPosToGridCellZ(pParty->vPosition.y) - 1;
  unsigned int v114_a = WorldPosToGridCellX(pX);
  v66 = WorldPosToGridCellZ(pY) - 1;
  unsigned int v122_a = (~(unsigned int)pOutdoor->ActuallyGetSomeOtherTileInfo(pMap_X, pMap_Y) / 2) & 1;
  v122 = (~(unsigned int)pOutdoor->ActuallyGetSomeOtherTileInfo(v114_a, pMap_Y) / 2) & 1;
  v69 = (~(unsigned int)pOutdoor->ActuallyGetSomeOtherTileInfo(pMap_X, v66) / 2) & 1;

  //-(обновление координат группы)---------------------------------------
  v68 = 0;
  if ( v114_a == pMap_X && v66 == pMap_Y && v122 && v69 )
    v68 = 1;
  if ( !is_not_on_bmodel ) // на bmodel,и
    v68 = 1;
  if ( v68 )
  {
    pParty->vPosition.x = pX;
    pParty->vPosition.y = pY;
    pParty->vPosition.z = pZ;
    pParty->field_6F0 = v113;
    pParty->uFallSpeed = fall_speed;
    if ( pZ > 8160 ) //ограничение высоты
    {
      pZ = 8160;
      pParty->uFallStartY = 8160;
      pParty->vPosition.z = 8160;
    }

    if ( !trigger_id //падение на землю
      || (EventProcessor(trigger_id, 0, 1),
         pParty->vPosition.x == pX)
      && pParty->vPosition.y == pY
      && pParty->vPosition.z == pZ )
    {
      if ( pParty->vPosition.z < v111 )
      {
        pParty->uFallSpeed = 0;
        //v73 = v105;
        pParty->vPosition.z = on_ground;
        if ( pParty->uFallStartY - pZ > 512 && !bFeatherFall && pZ <= on_ground && !bUnderwater )//Fall to the ground(падение на землю с высоты)
        {
          if ( pParty->uFlags & PARTY_FLAGS_1_LANDING )
            pParty->uFlags &= ~PARTY_FLAGS_1_LANDING;
          else
          {
            for ( uint i = 1; i <= 4; ++i )
            {
              pPlayers[i]->ReceiveDamage((signed int)((pParty->uFallStartY - pZ) * (unsigned __int64)(signed __int64)((double)pPlayers[i]->GetMaxHealth() * 0.1)) / 256,
                DMGT_PHISYCAL);
              v110 = 20 - pPlayers[i]->GetParameterBonus(pPlayers[i]->GetActualEndurance());
              pPlayers[i]->SetRecoveryTime((signed __int64)((double)v110 * flt_6BE3A4_debug_recmod1 * 2.133333333333333));
            }
            //v73 = pParty->vPosition.z;
          }
        }
        pParty->uFallStartY = pZ;
      }
      if ( v102 && pParty->vPosition.z < ceiling_height )
      {
        if ( (signed int)(pParty->uPartyHeight + pParty->vPosition.z) >= ceiling_height )
        {
          pParty->vPosition.z = ceiling_height - pParty->uPartyHeight - 1;
          pParty->field_6F0 = ceiling_height - pParty->uPartyHeight - 1;
        }
      }
      pParty->uFlags &= ~0x204;
    }
    return;
  }
//-----------------------------------------------------------------
  //v76 = pParty->bFlying;
  if ( pParty->bFlying || !not_high_fall || bWaterWalk || !v122_a )// полёт или высокое падение или хождение по воде или 
    v77 = 1;
  else
    v77 = v122 != 0;
  bool party_drowning_flag = false;
  if ( !pParty->bFlying && not_high_fall && !bWaterWalk ) //не полёт и не высокое падение и не хождение по воде
  {
    if ( v122_a )
      v78 = v69 != 0;
    else
    {
      party_drowning_flag = true;//утопление
      v78 = true;
    }
  }
  else
    v78 = true;

  if ( v77 )
    pParty->vPosition.x = pX;
  if ( v78 )
    pParty->vPosition.y = pY;

  if ( v78 || v77)
  {
    if ( bWaterWalk )
    {
      pParty->uFlags &= ~PARTY_FLAGS_1_STANDING_ON_WATER;
      //v79 = 20 * pParty->pPartyBuffs[PARTY_BUFF_WATER_WALK].uOverlayID + 6180178;
      //*(short *)&stru_5E4C90._decor_events[20 * pParty->pPartyBuffs[PARTY_BUFF_WATER_WALK].uOverlayID + 119] |= 1u;
      v79 = (int)&stru_5E4C90_MapPersistVars._decor_events[20 * pParty->pPartyBuffs[PARTY_BUFF_WATER_WALK].uOverlayID + 119];
      *(short *)v79 |= 1;
      if ( !v122 || !v69 )
      {
        if ( !pParty->bFlying )
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

  //v81 = pZ;
  //v82 = pZ;
  pParty->vPosition.z = pZ;
  if ( pZ > 8160 )//опять ограничение высоты
  {
    //v82 = 8160;
    pParty->uFallStartY = 8160;
    pParty->vPosition.z = 8160;
  }
  LOWORD(pParty->uFlags) &= 0xFDFBu;
  pParty->uFallSpeed = fall_speed;
  pParty->field_6F0 = v113;
  if ( party_drowning_flag )//группа тонет
  {
    pTerrainHeight = GetTerrainHeightsAroundParty2(pParty->vPosition.x, pParty->vPosition.y, &v110, 1);
    if ( pParty->vPosition.z <= pTerrainHeight + 1 )//положение группы всегда +1
      pParty->uFlags |= PARTY_FLAGS_1_WATER_DAMAGE;
  }

  if ( !trigger_id//падение на воду
    || (EventProcessor(trigger_id, 0, 1),
       pParty->vPosition.x == pX)
    && pParty->vPosition.y == pY
    && pParty->vPosition.z == pZ )
  {
    if ( pParty->vPosition.z < v111 )
    {
      //v82 = on_ground;
      pParty->uFallSpeed = 0;
      pParty->vPosition.z = on_ground;
      if ( pParty->uFallStartY - pZ > 512 && !bFeatherFall && pZ <= on_ground && !bUnderwater )//Fall to the water(падение на воду с высоты)
      {
        if ( pParty->uFlags & PARTY_FLAGS_1_LANDING )
          pParty->uFlags &= ~PARTY_FLAGS_1_LANDING;
        else
        {
          for ( uint i = 1; i <= 4; ++i )
          {
            v110 = pPlayers[i]->GetMaxHealth();
            pPlayers[i]->ReceiveDamage((signed int)((pParty->uFallStartY - pZ) * (unsigned __int64)(signed __int64)((double)v110 * 0.1)) / 256,
              DMGT_PHISYCAL);
            v110 = 20 - pPlayers[i]->GetParameterBonus(pPlayers[i]->GetActualEndurance());
            pPlayers[i]->SetRecoveryTime((signed __int64)((double)v110 * flt_6BE3A4_debug_recmod1 * 2.133333333333333));
          }
          //v82 = pParty->vPosition.z;
        }
      }
      pParty->uFallStartY = pZ;
    }
    if ( v102 && pParty->vPosition.z < ceiling_height && (signed int)(pParty->uPartyHeight + pParty->vPosition.z) >= ceiling_height )
    {
      pParty->vPosition.z = pParty->vPosition.z + pParty->uPartyHeight - ceiling_height + 1;
      pParty->field_6F0 = pParty->vPosition.z + pParty->uPartyHeight - ceiling_height + 1;
    }
  }
}

//----- (0046D8E3) --------------------------------------------------------
int GetCeilingHeight(int Party_X, signed int Party_Y, int Party_ZHeight, int pFaceID)
{
  signed int v13; // eax@25
  int v14; // edx@27
  int v16; // ST18_4@29
  signed int v17; // edx@29
  signed __int64 v18; // qtt@29
  int v19; // eax@35
  signed int v20; // ecx@37
  signed int v22; // ebx@42
//  int v24; // edx@44
//  int v25; // eax@44
  int v27; // [sp+10h] [bp-34h]@21
  bool v34; // [sp+30h] [bp-14h]@21
  bool v35; // [sp+34h] [bp-10h]@23
  signed int v37; // [sp+38h] [bp-Ch]@21
  signed int v38; // [sp+38h] [bp-Ch]@42
  signed int v39; // [sp+3Ch] [bp-8h]@1

  dword_720ED0[0] = -1;
  dword_720E80[0] = -1;
  v39 = 1;
  ceiling_height_level[0] = 10000;//нет потолка
  for ( uint i = 0; i < (signed int)pOutdoor->uNumBModels; ++i )
  {
    if ( Party_X <= pOutdoor->pBModels[i].sMaxX && Party_X >= pOutdoor->pBModels[i].sMinX
      && Party_Y <= pOutdoor->pBModels[i].sMaxY && Party_Y >= pOutdoor->pBModels[i].sMinY )
    {
      for ( uint j = 0; j < pOutdoor->pBModels[i].uNumFaces; ++j )
      {
        if ( (pOutdoor->pBModels[i].pFaces[j].uPolygonType == POLYGON_Ceiling
           || pOutdoor->pBModels[i].pFaces[j].uPolygonType == POLYGON_InBetweenCeilingAndWall)
           && !pOutdoor->pBModels[i].pFaces[j].Ethereal()
           && Party_X <= pOutdoor->pBModels[i].pFaces[j].pBoundingBox.x2 && Party_X >= pOutdoor->pBModels[i].pFaces[j].pBoundingBox.x1
           && Party_Y <= pOutdoor->pBModels[i].pFaces[j].pBoundingBox.y2 && Party_Y >= pOutdoor->pBModels[i].pFaces[j].pBoundingBox.y1 )
        {
          for ( uint v = 0; v < pOutdoor->pBModels[i].pFaces[j].uNumVertices; v++ )
          {
            word_720DB0_xs[2 * v] = pOutdoor->pBModels[i].pFaces[j].pXInterceptDisplacements[v] + LOWORD(pOutdoor->pBModels[i].pVertices.pVertices[pOutdoor->pBModels[i].pFaces[j].pVertexIDs[v]].x);
            word_720CE0_ys[2 * v] = pOutdoor->pBModels[i].pFaces[j].pXInterceptDisplacements[v] + LOWORD(pOutdoor->pBModels[i].pVertices.pVertices[pOutdoor->pBModels[i].pFaces[j].pVertexIDs[v]].y);
            word_720DB0_xs[2 * v + 1] = pOutdoor->pBModels[i].pFaces[j].pXInterceptDisplacements[v] + LOWORD(pOutdoor->pBModels[i].pVertices.pVertices[pOutdoor->pBModels[i].pFaces[j].pVertexIDs[v + 1]].x);
            word_720CE0_ys[2 * v + 1] = pOutdoor->pBModels[i].pFaces[j].pXInterceptDisplacements[v] + LOWORD(pOutdoor->pBModels[i].pVertices.pVertices[pOutdoor->pBModels[i].pFaces[j].pVertexIDs[v + 1]].y);
          }
          v27 = 2 * pOutdoor->pBModels[i].pFaces[j].uNumVertices;
          word_720DB0_xs[2 * pOutdoor->pBModels[i].pFaces[j].uNumVertices] = word_720DB0_xs[0];
          word_720CE0_ys[2 * pOutdoor->pBModels[i].pFaces[j].uNumVertices] = word_720CE0_ys[0];
          v34 = word_720CE0_ys[0] >= Party_Y;
          v37 = 0;
          for ( uint v = 0; v < v27; ++v )
          {
            if ( v37 >= 2 )
              break;
            v35 = word_720CE0_ys[v + 1] >= Party_Y;
            if ( v34 != v35 )
            {
              v13 = word_720DB0_xs[v + 1] >= Party_X ? 0 : 2;
              v14 = v13 | (word_720DB0_xs[v] < Party_X);
              if ( v14 != 3 )
              {
                if ( !v14 || ( v16 = word_720CE0_ys[v + 1] - word_720CE0_ys[v],
                  v17 = Party_Y - word_720CE0_ys[v],
                  LODWORD(v18) = v17 << 16,
                  HIDWORD(v18) = v17 >> 16,
                  (signed int)(((unsigned __int64)(((signed int)word_720DB0_xs[v + 1]
                  - (signed int)word_720DB0_xs[v]) * v18 / v16) >> 16) + word_720DB0_xs[v]) >= Party_X) )
                  ++v37;
              }
            }
            v34 = v35;
          }
          if ( v37 == 1 )
          {
            if ( v39 >= 20 )
              break;
            if ( pOutdoor->pBModels[i].pFaces[j].uPolygonType == POLYGON_Ceiling )
              v19 = pOutdoor->pBModels[i].pVertices.pVertices[pOutdoor->pBModels[i].pFaces[j].pVertexIDs[0]].z;
            else
              v19 = fixpoint_mul(pOutdoor->pBModels[i].pFaces[j].zCalc1, Party_X) + fixpoint_mul(pOutdoor->pBModels[i].pFaces[j].zCalc2, Party_Y)
                  + HIWORD(pOutdoor->pBModels[i].pFaces[j].zCalc3);
            v20 = v39++;
            ceiling_height_level[v20] = v19;
            dword_720ED0[v20] = i;
            dword_720E80[v20] = j;
          }
        }
      }
    }
  }
  if ( !v39 )
  {
    pFaceID = 0;
    return ceiling_height_level[0];
  }
  v22 = 0;
  for ( v38 = 0; v38 < v39; ++v38 )
  {
    if ( ceiling_height_level[v38] == ceiling_height_level[0] )
      v22 = v38;
    else if ( ceiling_height_level[v38] < ceiling_height_level[0] && ceiling_height_level[0] > Party_ZHeight + 15 )
      v22 = v38;
    else if ( ceiling_height_level[v38] > ceiling_height_level[0] && ceiling_height_level[v38] <= Party_ZHeight + 15 )
      v22 = v38;
  }
  if ( v22 )
  {
    *(int *)pFaceID = dword_720E80[v22] | (dword_720ED0[v22] << 6);
    return ceiling_height_level[v22];//если есть преграда
  }
  pFaceID = 0;
  return ceiling_height_level[v22];// нет никакой преграды
}


//----- (00464839) --------------------------------------------------------
char Is_out15odm_underwater()
{
  return _stricmp(pCurrentMapName, "out15.odm") == 0;
}

//----- (00464851) --------------------------------------------------------
void SetUnderwaterFog()
{
  day_fogrange_1 = 50;
  day_fogrange_2 = 5000;
}