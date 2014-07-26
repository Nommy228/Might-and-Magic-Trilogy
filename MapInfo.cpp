#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "MapInfo.h"
#include "LOD.h"
#include "ErrorHandling.h"
#include "mm7_data.h"
#include "texts.h"
#include "ObjectList.h"
#include "SpriteObject.h"
#include "Indoor.h"
#include "mm7_unsorted_subs.h"


//----- (00453F62) --------------------------------------------------------
void MapStats::Initialize()
{
  char work_str[32]; // [sp+Ch] [bp-34h]@3
  int work_str_pos;
  int work_str_len;
  int i;
  char* test_string;
  unsigned char c;
  bool break_loop;
  unsigned int temp_str_len;
  char* tmp_pos;
  int decode_step;
//  int item_counter;

  free(pMapStatsTXT_Raw);
  pMapStatsTXT_Raw = (char *)pEvents_LOD->LoadRaw("MapStats.txt", 0);
  strtok(pMapStatsTXT_Raw, "\r");
  strtok(NULL, "\r");
  strtok(NULL, "\r");

  for (i=1; i<77; ++i)
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
        case 1:
          pInfos[i].pName = RemoveQuotes(test_string);
          break;
        case 2:
          pInfos[i].pFilename = RemoveQuotes(test_string);
          break;
        case 3:
          pInfos[i].uNumResets = atoi(test_string);
          break;
        case 4:
          pInfos[i].uFirstVisitedAt = atoi(test_string);
          break;
        case 5:
          pInfos[i]._per = atoi(test_string);
          break;
        case 6:
          pInfos[i].uRespawnIntervalDays = atoi(test_string);
          break;
        case 7:
          pInfos[i]._alert_days = atoi(test_string);
          break;
        case 8:
          pInfos[i]._steal_perm = atoi(test_string);
          break;
        case 9:
          pInfos[i].LockX5 = atoi(test_string);
          break;
        case 10:
          pInfos[i].Trap_D20 = atoi(test_string);
          break;
        case 11:
          pInfos[i].Treasure_prob = atoi(test_string);
          break;
        case 12:
          pInfos[i].Encounter_percent = atoi(test_string);
          break;
        case 13:
          pInfos[i].EncM1percent = atoi(test_string);
          break;
        case 14:
          pInfos[i].EncM2percent = atoi(test_string);
          break;
        case 15:
          pInfos[i].EncM3percent = atoi(test_string);
          break;
        case 16:
          pInfos[i].pEncounterMonster1Texture = RemoveQuotes(test_string);
          break;
        case 18:
          pInfos[i].Dif_M1 = atoi(test_string);
          break;
        case 19:
          pInfos[i].uEncounterMonster1AtLeast = 1;
          pInfos[i].uEncounterMonster1AtMost = 1;
          strcpy(work_str, test_string);
          work_str_pos = 0;
          work_str_len=strlen(work_str);
          if (work_str_len )
          {
            while (work_str[work_str_pos] != '-' )
            {
              ++work_str_pos;
              if (work_str_pos >= work_str_len )
                break;
            }
            work_str[work_str_pos] = 0;
            pInfos[i].uEncounterMonster1AtLeast = atoi(work_str);
            if ( work_str_pos < work_str_len )
              pInfos[i].uEncounterMonster1AtMost = atoi(&work_str[work_str_pos + 1]);
            else
              pInfos[i].uEncounterMonster1AtMost = pInfos[i].uEncounterMonster1AtLeast;
          }
          break;
        case 20:
          pInfos[i].pEncounterMonster2Texture = RemoveQuotes(test_string);
          break;
        case 22:
          pInfos[i].Dif_M2 = atoi(test_string);
          break;
        case 23:
          pInfos[i].uEncounterMonster2AtLeast = 1;
          pInfos[i].uEncounterMonster2AtMost = 1;
          strcpy(work_str, test_string);
          work_str_pos = 0;
          work_str_len=strlen(work_str);
          if (work_str_len )
          {
            while (work_str[work_str_pos] != '-' )
            {
              ++work_str_pos;
              if (work_str_pos >= work_str_len )
                break;
            }
            work_str[work_str_pos] = 0;
            pInfos[i].uEncounterMonster2AtLeast = atoi(work_str);
            if ( work_str_pos < work_str_len )
              pInfos[i].uEncounterMonster2AtMost = atoi(&work_str[work_str_pos + 1]);
            else
              pInfos[i].uEncounterMonster2AtMost = pInfos[i].uEncounterMonster2AtLeast;
          }
          break;
        case 24:
          pInfos[i].pEncounterMonster3Texture = RemoveQuotes(test_string);
          break;
        case 26:
          pInfos[i].Dif_M3 = atoi(test_string);
          break;
        case 27:
          pInfos[i].uEncounterMonster3AtLeast = 1;
          pInfos[i].uEncounterMonster3AtMost = 1;
          strcpy(work_str, test_string);
          work_str_pos = 0;
          work_str_len=strlen(work_str);
          if (work_str_len )
          {
            while (work_str[work_str_pos] != '-' )
            {
              ++work_str_pos;
              if (work_str_pos >= work_str_len )
                break;
            }
            work_str[work_str_pos] = 0;
            pInfos[i].uEncounterMonster3AtLeast = atoi(work_str);
            if ( work_str_pos < work_str_len )
              pInfos[i].uEncounterMonster3AtMost = atoi(&work_str[work_str_pos + 1]);
            else
              pInfos[i].uEncounterMonster3AtMost = pInfos[i].uEncounterMonster3AtLeast;
          }
          break;
        case 28:
          pInfos[i].uRedbookTrackID = atoi(test_string);
          break;
        case 29:
          {
            if ( !strcmp(test_string, "CAVE") )
            {
              pInfos[i].uEAXEnv = 8;
              break;
            }
            if ( !strcmp(test_string, "STONEROOM") )
            {
              pInfos[i].uEAXEnv = 5;
              break;
            }
            if ( !strcmp(test_string, "MOUNTAINS") )
            {
              pInfos[i].uEAXEnv = 17;
              break;
            }
            if ( !strcmp(test_string, "PLAIN") )
            {
              pInfos[i].uEAXEnv = 19;
              break;
            }
            if ( !strcmp(test_string, "FOREST") )
            {
              pInfos[i].uEAXEnv = 15;
              break;
            }
            if ( !strcmp(test_string, "CITY") )
            {
              pInfos[i].uEAXEnv = 16;
              break;
            }
            if ( !strcmp(test_string, "UNDERWATER") )
            {
              pInfos[i].uEAXEnv = 22;
              break;
            }
            if ( !strcmp(test_string, "ARENA") )
            {
              pInfos[i].uEAXEnv = 9;
              break;
            }
            if ( !strcmp(test_string, "GENERIC") )
            {
              pInfos[i].uEAXEnv = 0;
              break;
            }
            if ( !strcmp(test_string, "PADDEDCELL") )
            {
              pInfos[i].uEAXEnv = 1;
              break;
            }
            if ( !strcmp(test_string, "ROOM") )
            {
              pInfos[i].uEAXEnv = 2;
              break;
            }
            if ( !strcmp(test_string, "BATHROOM") )
            {
              pInfos[i].uEAXEnv = 3;
              break;
            }
            if ( !strcmp(test_string, "LIVINGROOM") )
            {
              pInfos[i].uEAXEnv = 4;
              break;
            }
            if ( !strcmp(test_string, "AUDITORIUM") )
            {
              pInfos[i].uEAXEnv = 6;
              break;
            }
            if ( !strcmp(test_string, "CONCERTHALL") )
            {
              pInfos[i].uEAXEnv = 7;
              break;
            }
            if ( !strcmp(test_string, "HANGAR") )
            {
              pInfos[i].uEAXEnv = 10;
              break;
            }
            if ( !strcmp(test_string, "CARPETEDHALLWAY") )
            {
              pInfos[i].uEAXEnv = 11;
              break;
            }
            if ( !strcmp(test_string, "HALLWAY") )
            {
              pInfos[i].uEAXEnv = 12;
              break;
            }
            if ( !strcmp(test_string, "STONECORRIDOR") )
            {
              pInfos[i].uEAXEnv = 13;
              break;
            }
            if ( !strcmp(test_string, "ALLEY") )
            {
              pInfos[i].uEAXEnv = 14;
              break;
            }
            if ( !strcmp(test_string, "QUARRY") )
            {
              pInfos[i].uEAXEnv = 18;
              break;
            }
            if ( !strcmp(test_string, "PARKINGLOT") )
            {
              pInfos[i].uEAXEnv = 20;
              break;
            }
            if ( !strcmp(test_string, "SEWERPIPE") )
            {
              pInfos[i].uEAXEnv = 21;
              break;
            }
            if ( !strcmp(test_string, "DRUGGED") )
            {
              pInfos[i].uEAXEnv = 23;
              break;
            }
            if ( !strcmp(test_string, "DIZZY") )
            {
              pInfos[i].uEAXEnv = 24;
              break;
            }
            if ( !strcmp(test_string, "PSYCHOTIC") )
            {
              pInfos[i].uEAXEnv = 25;
              break;
            }
            pInfos[i].uEAXEnv = 26;

          }
          break;
        }
      }
      else
      { 
        break_loop = true;
      }
      ++decode_step;
      test_string=tmp_pos+1;
    } while ((decode_step<31)&&!break_loop);
  }

  uNumMaps = 77;
}
// 453F62: using guessed type char Str[32];

//----- (00410D99) --------------------------------------------------------
int MapStats::sub_410D99_get_map_index(int a1)
{
  for (int i = 1; i <= pMapStats->uNumMaps; i++)
  {
    if (_stricmp(pMapStats->pInfos[i].pFilename, pGames_LOD->pSubIndices[a1].pFilename) )
      return i;
  }
  Error("Map not found");
  return -1;
}

//----- (004547A3) --------------------------------------------------------
MAP_TYPE MapStats::GetMapInfo(const char *Str2)
{
  Assert(uNumMaps >= 2);

  for (uint i = 1; i < uNumMaps; ++i)
    if (!_stricmp(pInfos[i].pFilename, Str2))
      return (MAP_TYPE)i;

  Error("Map not found!");
  return (MAP_TYPE)-1;
}


//----- (0044FFD8) --------------------------------------------------------
int MapInfo::SpawnRandomTreasure(SpawnPointMM7 *a2)
{
  //MapInfo *v2; // ebx@1
  //SpawnPointMM7 *v3; // esi@1
  //int v4; // eax@1
  int v5; // edx@1
//  int v6; // eax@1
  int v7; // ecx@1
  int v8; // ebx@1
  int v9; // eax@1
  signed int v10; // ebx@1
  signed int result; // eax@1
  signed __int64 v12; // qtt@1
  int v13; // ebx@1
  int v14; // edx@10
  signed int v15; // ebx@20
  unsigned __int16 v16; // dx@20
//  ObjectDesc *v17; // ecx@21
  unsigned __int16 v18; // ax@24
  int v19; // ST0C_4@27
  int v20; // ST08_4@27
  int v21; // ST04_4@27
  int v22; // eax@27
  signed int v23; // ebx@29
  unsigned __int16 v24; // dx@29
//  ObjectDesc *v25; // ecx@30
  unsigned __int16 v26; // ax@33
  //int v27; // ecx@35
  //int v28; // eax@35
  //int v29; // esi@35
  //__int16 v30; // ax@35
  SpriteObject a1a; // [sp+Ch] [bp-7Ch]@1
  //int v32; // [sp+7Ch] [bp-Ch]@1
  //int v33; // [sp+80h] [bp-8h]@1
  int v34; // [sp+84h] [bp-4h]@1

  //auto a1 = this;
  //v2 = a1;
  //v3 = a2;
  //v4 = rand();
  v34 = 0;
  v5 = rand() % 100;
  // v6 = 2 * (v2->Treasure_prob + 7 * v3->uIndex) - 14;
  v7 = (unsigned __int8)byte_4E8168[a2->uIndex - 1][2 * Treasure_prob];
  v8 = (unsigned __int8)byte_4E8168[a2->uIndex - 1][2 * Treasure_prob + 1];
  //v32 = v5;
  //v33 = v7;
  v9 = rand();
  v10 = v8 - v7 + 1;
  v12 = v9;
  result = v9 / v10;
  v13 = v7 + (unsigned __int64)(v12 % v10);
  if ( v13 < 7 )
  {
    if ( v5 < 20 )
      return result;
    if ( v5 >= 60 )
    {
      v19 = a2->vPosition.z;
      v20 = a2->vPosition.y;
      v21 = a2->vPosition.x;
      v22 = rand();
      return sub_450521_ProllyDropItemAt(v13, v22 % 27 + 20, v21, v20, v19, 0);
    }
    if ( a2->uIndex == 1 )
    {
      v14 = rand() % 51 + 50;
      a1a.stru_24.uItemID = 197;
      v34 = v14;
    }
    else if ( a2->uIndex == 2 )
    {
      v14 = rand() % 101 + 100;
      a1a.stru_24.uItemID = 197;
      v34 = v14;
    }
    else if ( a2->uIndex == 3 )
    {
      v14 = rand() % 301 + 200;
      a1a.stru_24.uItemID = 198;
      v34 = v14;
    }
    else if ( a2->uIndex == 4 )
    {
      v14 = rand() % 501 + 500;
      a1a.stru_24.uItemID = 198;
      v34 = v14;
    }
    else if ( a2->uIndex == 5 )
    {
      v14 = rand() % 1001 + 1000;
      a1a.stru_24.uItemID = 199;
      v34 = v14;
    }
    else if ( a2->uIndex == 6 )
    {
      v14 = rand() % 3001 + 2000;
      a1a.stru_24.uItemID = 199;
      v34 = v14;
    }	 
    v15 = 0;
    v16 = pItemsTable->pItems[a1a.stru_24.uItemID].uSpriteID;
    a1a.uType = pItemsTable->pItems[a1a.stru_24.uItemID].uSpriteID;
    v18 = 0;
    for( int i = 0; i < pObjectList->uNumObjects; i++ )
    {
      if ( pObjectList->pObjects[i].uObjectID == v16 )
      {
        v18 = i;
        break;
      }
    }
    a1a.stru_24.SetIdentified();
    a1a.uObjectDescID = v18;
    a1a.stru_24.uSpecEnchantmentType = v34;
  }
  else
  {
    result = a1a.stru_24.GenerateArtifact();
    if ( !result )
      return result;
    v23 = 0;
    v24 = pItemsTable->pItems[a1a.stru_24.uItemID].uSpriteID;
    a1a.uType = pItemsTable->pItems[a1a.stru_24.uItemID].uSpriteID;
    v26 = 0;
    for( int i = 0; i < pObjectList->uNumObjects; i++ )
    {
      if( v24 == pObjectList->pObjects[i].uObjectID )
      {
        v26 = i;
        break;
      }
    }
    a1a.uObjectDescID = v26;
    a1a.stru_24.Reset();
  }
  a1a.vPosition.y = a2->vPosition.y;
  a1a.uAttributes = 0;
  a1a.uSoundID = 0;
  a1a.uFacing = 0;
  a1a.vPosition.z = a2->vPosition.z;
  a1a.vPosition.x = a2->vPosition.x;
  a1a.spell_skill = 0;
  a1a.spell_level = 0;
  a1a.spell_id = 0;
  a1a.spell_target_pid = 0;
  a1a.spell_caster_pid = 0;
  a1a.uSpriteFrameID = 0;
  a1a.uSectorID = pIndoor->GetSector(a2->vPosition.x, a2->vPosition.y, a2->vPosition.z);;
  return a1a.Create(0, 0, 0, 0);
}

