#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include "mm7_unsorted_subs.h"
#include "ErrorHandling.h"

#include "Weather.h"
#include "Texture.h"
#include "mm7_data.h"
#include "VideoPlayer.h"
#include "Mouse.h"

#include "MapInfo.h"
#include "Game.h"
#include "Render.h"
#include "GUIWindow.h"
#include "GUIProgressBar.h"
#include "SpriteObject.h"
#include "Chest.h"
#include "MapsLongTimer.h"
#include "LOD.h"
#include "Actor.h"
#include "Party.h"
#include "OurMath.h"
#include "AudioPlayer.h"
#include "Indoor.h"
#include "Viewport.h"
#include "texts.h"
#include "stru123.h"
#include "stru159.h"
#include "Events.h"
#include "Events2D.h"
#include "UI\UIHouses.h"
#include "Log.h"
#include "MM7.h"
#include "Level/Decoration.h"
#include "LuaVM.h"



std::array<EventIndex, 4400> pSomeOtherEVT_Events;
signed int uSomeOtherEVT_NumEvents;
char *pSomeOtherEVT;
std::array<EventIndex, 4400> pSomeEVT_Events;
signed int uSomeEVT_NumEvents;
char *pSomeEVT;

unsigned int uGlobalEVT_NumEvents;
unsigned int uGlobalEVT_Size;
std::array<char, 46080> pGlobalEVT;
std::array<EventIndex, 4400> pGlobalEVT_Index;

std::array<unsigned int, 500> pLevelStrOffsets;
unsigned int uLevelStrNumStrings;
unsigned int uLevelStrFileSize;
signed int uLevelEVT_NumEvents;
unsigned int uLevelEVT_Size;
std::array<char, 9216> pLevelStr;
std::array<char, 9216> pLevelEVT;
std::array<EventIndex, 4400> pLevelEVT_Index;

_2devent p2DEvents[525];



//----- (00443CE1) --------------------------------------------------------
unsigned int LoadEventsToBuffer(const char *pContainerName, char *pBuffer, unsigned int uBufferSize)
{
  FILE *pLodFile; // eax@1
  unsigned int uTextureSize; // esi@3
  //char Args[60]; // [sp+8h] [bp-B4h]@6
  void *ptr; // [sp+B8h] [bp-4h]@1
  Texture DstBuf; // [sp+6Ch] [bp-50h]@1

  ptr = pEvents_LOD->LoadRaw(pContainerName, 0);
  pLodFile = pEvents_LOD->FindContainer(pContainerName, 0);
  if ( !pLodFile )
    Error("Unable to load %s", pContainerName);

  fread(&DstBuf, 1, 48, pLodFile);
  uTextureSize = DstBuf.uDecompressedSize;
  if ( !DstBuf.uDecompressedSize )
    uTextureSize = DstBuf.uTextureSize;
  memset(&DstBuf, 0, 72);
  if ( uTextureSize >= (signed int)uBufferSize )
    Error("File %s Size %lu - Buffer size %lu", pContainerName, uTextureSize, uBufferSize);

  memcpy(pBuffer, ptr, uTextureSize);
  free(ptr);
  return uTextureSize;
  }

//----- (00443DA1) --------------------------------------------------------
void  Initialize_GlobalEVT()
{
  struct raw_event_header
    {
    unsigned char evt_size;
    unsigned char evt_id_l;
    unsigned char evt_id_h;
    unsigned char evt_sequence_num;
    } ;
  uint events_count;
  unsigned int offset_in;
  raw_event_header *current_hdr;
  uGlobalEVT_NumEvents = 0;
  uGlobalEVT_Size = LoadEventsToBuffer("global.evt", pGlobalEVT.data(), 46080);
  if ( !uGlobalEVT_Size )
    return;
  memset(pGlobalEVT_Index.data(), 0x80, sizeof(pGlobalEVT_Index));//52800
  events_count = uGlobalEVT_NumEvents;
  current_hdr=(raw_event_header *)pGlobalEVT.data();
  offset_in=0;
  for (events_count = 0, offset_in = 0; offset_in < uGlobalEVT_Size; ++events_count)
    {
    pGlobalEVT_Index[events_count].uEventID=current_hdr->evt_id_l+(current_hdr->evt_id_h<<8);
    pGlobalEVT_Index[events_count].event_sequence_num=current_hdr->evt_sequence_num;
    pGlobalEVT_Index[events_count].uEventOffsetInEVT=offset_in;
    offset_in+=current_hdr->evt_size+1;

    current_hdr=(raw_event_header *)&pGlobalEVT[offset_in];
    }
  uGlobalEVT_NumEvents = events_count;

  assert(uGlobalEVT_NumEvents < 4400);
}



//----- (00443EF8) --------------------------------------------------------
void  LoadLevel_InitializeLevelEvt()
  {
    struct raw_event_header
        {
        unsigned char evt_size;
        unsigned char evt_id_l;
        unsigned char evt_id_h;
        unsigned char evt_sequence_num;
        } ;
    uint events_count;
    unsigned int offset_in;
    raw_event_header *current_hdr;

  if (!uLevelEVT_Size)
    return;

  memset(MapsLongTimersList, 0, 3200);
  memset(pLevelEVT_Index.data(), 80, sizeof(EventIndex)*4400);

  uLevelEVT_NumEvents = 0;
  MapsLongTimers_count = 0;

    current_hdr=(raw_event_header *)pLevelEVT.data();
    offset_in=0;
    for (events_count = 0, offset_in = 0; offset_in < uLevelEVT_Size; ++events_count)
        {
        pLevelEVT_Index[events_count].uEventID=current_hdr->evt_id_l+(current_hdr->evt_id_h<<8);
        pLevelEVT_Index[events_count].event_sequence_num=current_hdr->evt_sequence_num;
        pLevelEVT_Index[events_count].uEventOffsetInEVT=offset_in;
        offset_in+=current_hdr->evt_size+1;

        current_hdr=(raw_event_header *)&pLevelEVT[offset_in];
        }
    uLevelEVT_NumEvents = events_count;

/*
EmeraldIsle::Variables:
[0] ???
[1] ???
[2] Luck Fountain uses left
[3] Gold Fountain used this week
[4] Gold Fountain total uses


Emerald Isle #110 // Fire Resistance fountain
0 LocationName
0 if (Player.FireResistance < 50)
  {
1   Set(Player.FireResistance, 50)
2   SetFooterString(22) // +50 Fire Resistance (temporarily)
3   Add(Party.Autonotes, 2)
4   goto return
  }
5 SetFooterString(11) // Refreshing!
6 return



Emerald Isle #111 // ???
  Initialize
  Set(Map.Variables[0], 30)
  Set(Map.Variables[1], 30)


Emerald Isle #114 // month timer - manage luck fountain
0   LocationName
0     if (Player.BaseLuck >= 15)
    {
2     SetFooterString(11) // Refreshing!
3     return
    }
    else
    {
1     if (Map.Variables[2] >= 1)
      {
4       Sub(Map.Variables[2], 1)
5       Add(Player.BaseLuck, 2)
6       SetFooterString(25) // +2 Luck (Permament)
7       return
      }
      else
      {
2       SetFooterString(11) // Refreshing!
3       return
      }
    }

8   Initialize
9   Set Map.Variables[2], 8



Emerald Isle #115 // week timer - gold fountain in the center of town
0 LocationName
0 if (Map.Variables[4] < 3)
  {
1   if (Map.Variables[3] == 0)
    {
2     if (Party.Gold < 201)
      {
3       if (Player.BaseLuck >= 15)
        {
5         Add(Map.Variables[3], 1)
6         Add(Party.Gold, 1000)
7         Add(Map.Variables[4], 1)
8         goto return
        }
        else
        {
4         goto 9
        }
      }
    }
  }
9 SetFooterString(11) // Refreshing!
10  return

11  Initialize
12  Set(Map.Variables[3], 0)





Emerald Isle #220 // day timer - monster spawner
0 LocationName
0 Initialize
1 if (NumAliveActors(group=20) != 0)
2   return
3 SpawnMonsters(1, level=1, count=10, x=-336, y=14512, z=0,  group=20)
4 SpawnMonsters(1, level=2, count=5,  x=16,   y=16352, z=90, group=20)
5 SpawnMonsters(1, level=1, count=10, x=480,  y=18288, z=6,  group=20)



Emerald Isle #200 // margareth dock tip
0 if (!QBits.QuestDone[17])
  {
1   InitiateNPCDialogue(npc=19)
  }
2 return


Emerald Isle #201 // margareth armoury tip
0 if (!QBits.QuestDone[17])
  {
1   InitiateNPCDialogue(npc=20)
  }
2 return
*/
  }


//----- (0044684A) --------------------------------------------------------
void EventProcessor(int uEventID, int targetObj, int canShowMessages, int entry_line)
{
//  unsigned int v3; // eax@5
  signed int v4; // esi@7
  int v11; // eax@14
  char *v12; // eax@15
  const char *v16; // esi@21
  bool v17; // edx@21
  int v18; // ecx@22
  int v19; // ebp@36
  signed int v20; // ecx@40
  int v21; // eax@40
  int v22; // edx@40
  int v23; // eax@40
  unsigned __int16 v24; // ax@45
  LevelDecoration *v26; // eax@55
  int v27; // eax@57
//  int v28; // ecx@57
  int pEventID; // eax@58
  int pNPC_ID; // ecx@58
  int pIndex; // esi@58
  NPCData *pNPC; // ecx@58
//  int v34; // esi@59
//  int v35; // esi@60
//  int v36; // esi@61
//  int v37; // esi@62
  int v38; // eax@78
  int v39; // ecx@78
  int v42; // eax@84
  int v43; // ecx@84
//  void *v46; // eax@91
//  GUIWindow *v47; // eax@93
  GUIButton *v48; // ecx@93
  GUIButton *v49; // esi@94
//  char v50; // al@100
//  Player *v51; // esi@103
//  Player *v52; // ecx@106
//  int v53; // ecx@107
//  char v54; // al@111
//  Player *v55; // esi@114
//  Player *v56; // ecx@117
//  int v57; // ecx@118
  signed int pValue; // ebp@124
  Player *pPlayer; // esi@125
//  int v60; // eax@126
//  int v61; // edx@133
//  int v62; // eax@139
//  int v63; // ebp@145
//  signed int v64; // edi@146
//  unsigned int v65; // edx@148
//  Player *v66; // ecx@148
//  int v67; // esi@148
//  signed int v68; // eax@151
//  int v69; // esi@151
//  Player *v70; // ecx@158
//  unsigned int v71; // eax@159
//  int v72; // esi@159
//  signed int v73; // eax@162
//  int v74; // esi@162
//  int v75; // edx@172
//  Player *v76; // esi@173
//  signed int v77; // ebp@186
//  int v78; // edx@186
//  Player *v79; // esi@187
//  Player *v82; // esi@201
  int v83; // eax@212
  int v84; // ebp@220
  int v90; // eax@243
  const char *v91; // ecx@247
//  int v92; // eax@251
//  char *v93; // eax@252
  int v94; // ecx@262
  int v95; // ebp@262
  int v96; // edx@262
  int v97; // eax@262
  unsigned int v98; // edx@265
  const char *v99; // esi@267
  int v100; // edx@267
  unsigned int v102; // esi@281
//  int v103; // edi@284
  int v104; // eax@288
//  int v105; // edx@294
  int v106; // [sp-20h] [bp-4C8h]@278
  signed int v109; // [sp-14h] [bp-4BCh]@278
  signed int v110; // [sp-10h] [bp-4B8h]@278
//  int v113; // [sp-8h] [bp-4B0h]@106
//  int v114; // [sp-8h] [bp-4B0h]@117
//  int v117; // [sp-4h] [bp-4ACh]@106
//  int v118; // [sp-4h] [bp-4ACh]@117
//  int v121; // [sp-4h] [bp-4ACh]@294
  int curr_seq_num; // [sp+10h] [bp-498h]@4
  int v126; // [sp+1Ch] [bp-48Ch]@262
  int player_choose; // [sp+20h] [bp-488h]@4
  int v128; // [sp+24h] [bp-484h]@21
  int v129; // [sp+24h] [bp-484h]@262
  signed int v130; // [sp+28h] [bp-480h]@0
  int v132; // [sp+30h] [bp-478h]@262
  signed int v133; // [sp+34h] [bp-474h]@1
  int v134; // [sp+38h] [bp-470h]@262
  int v135; // [sp+3Ch] [bp-46Ch]@262
  int v136; // [sp+40h] [bp-468h]@40
  int v137; // [sp+44h] [bp-464h]@40
  int v138; // [sp+48h] [bp-460h]@40
  int v139; // [sp+4Ch] [bp-45Ch]@40
  ItemGen item; // [sp+50h] [bp-458h]@15
  char Source[120]; // [sp+74h] [bp-434h]@15
  char Str[120]; // [sp+ECh] [bp-3BCh]@21
  Actor Dst; // [sp+164h] [bp-344h]@53

  v133 = 0;
  EvtTargetObj = targetObj;
  dword_5B65C4_cancelEventProcessing = 0;
  /*if ( uEventID == 114 )//чтобы проверить скрипт
  {
    if (!lua->DoFile("out01.lua"))
      Log::Warning(L"Error opening out01.lua\n");
    Log::Warning(L"being tested that well\n");
    return;
  }	*/
  if ( !uEventID )
  {
    if ( !GameUI_Footer_TimeLeft )
      ShowStatusBarString(pGlobalTXT_LocalizationStrings[521], 2u);// Nothing here
    return;
  }
  player_choose = (uActiveCharacter == 0)?6:4;  //4 - active or  6 - random player if active =0
  curr_seq_num = entry_line;

  if ( activeLevelDecoration )
  {
    uSomeEVT_NumEvents = uGlobalEVT_NumEvents;
    pSomeEVT = pGlobalEVT.data();
    memcpy(pSomeEVT_Events.data(), pGlobalEVT_Index.data(), sizeof(EventIndex)*4400); //4400 evts
  }
  else
  {
    uSomeEVT_NumEvents = uLevelEVT_NumEvents;
    pSomeEVT = pLevelEVT.data();
    memcpy(pSomeEVT_Events.data(), pLevelEVT_Index.data(), sizeof(EventIndex)*4400);
  }

  for ( v4 = 0; v4 < uSomeEVT_NumEvents; ++v4 )
  {
    if ( dword_5B65C4_cancelEventProcessing )
    {
      if ( v133 == 1 )
        OnMapLeave();
      return;
    }
    if ( pSomeEVT_Events[v4].uEventID == uEventID && pSomeEVT_Events[v4].event_sequence_num == curr_seq_num )
      {
      _evt_raw * _evt = (_evt_raw *)(pSomeEVT + pSomeEVT_Events[v4].uEventOffsetInEVT);

      switch (_evt->_e_type)
      {
      case EVENT_CheckSeason:
        if ( !sub_4465DF_check_season(_evt->v5) )
        {
          ++curr_seq_num;
          //v4 = v124;
          break;
        }
        v4 = -1;
        curr_seq_num = _evt->v6 - 1;
          ++curr_seq_num;
        break;
      case EVENT_ShowMovie:
      {
        strcpy(Source, (char *)&_evt->v7);
        v12 = (char *)&item.uExpireTime + strlen(Source) + 7;
        if ( *v12 == 32 )
          *v12 = 0;
        if (pVideoPlayer->bBufferLoaded)
          pVideoPlayer->Unload();
        pVideoPlayer->bStopBeforeSchedule = 0;
        pVideoPlayer->pResetflag = 0;

        v128 = pCurrentScreen;
        strcpy(Str, Source);
        v16 = RemoveQuotes(Str);
		pVideoPlayer->MovieLoop(v16, 0, _evt->v5, 1);
        if ( !_stricmp(v16, "arbiter good") )
                {
                  pParty->alignment = PartyAlignment_Good;
          v18 = 0;
          LOBYTE(v17) = 1;
          SetUserInterface(PartyAlignment_Good, v17);
          if ( !_evt->v6 || v128 == 3 )
          {
            pCurrentScreen = v128;
            if ( v128 == 3 )
              pGameLoadingUI_ProgressBar->uType = GUIProgressBar::TYPE_Fullscreen;
            if ( v128 == 13 )
              pVideoPlayer->OpenHouseMovie(pAnimatedRooms[uCurrentHouse_Animation].video_name, 1u);
          }
          ++curr_seq_num;
          break;
        }
        if ( !_stricmp(v16, "arbiter evil") )
        {
          v18 = 2;
          pParty->alignment = PartyAlignment_Evil;
          LOBYTE(v17) = 1;
          SetUserInterface(PartyAlignment_Evil, v17);
          if ( !_evt->v6 || v128 == 3 )
          {
            pCurrentScreen = v128;
            if ( v128 == 3 )
              pGameLoadingUI_ProgressBar->uType = GUIProgressBar::TYPE_Fullscreen;
            if ( v128 == 13 )
              pVideoPlayer->OpenHouseMovie(pAnimatedRooms[uCurrentHouse_Animation].video_name, 1u);
          }
          ++curr_seq_num;
          break;
        }
        if ( !_stricmp(v16, "pcout01") )    // moving to harmondale from emerald isle
        {
          Rest(0x2760u);
          pParty->RestAndHeal();
          pParty->days_played_without_rest = 0;
        }
        if ( !_evt->v6 || v128 == 3 )
        {
          pCurrentScreen = v128;
          if ( v128 == 3 )
            pGameLoadingUI_ProgressBar->uType = GUIProgressBar::TYPE_Fullscreen;
          if ( v128 == 13 )
            pVideoPlayer->OpenHouseMovie(pAnimatedRooms[uCurrentHouse_Animation].video_name, 1);
        }
        ++curr_seq_num;
        }
        break;
      case EVENT_CheckSkill:
      {
        v19 = _evt->v7 + ((_evt->v8 + ((_evt->v9 + ((unsigned int)_evt->v10 << 8)) << 8)) << 8);
        if ( player_choose < 0 )
          goto LABEL_47;
        if ( player_choose <= 3 )
          v24 = pParty->pPlayers[0].pActiveSkills[3486 * player_choose + _evt->v5];
        else
          {
          if ( player_choose == 4 )
            v24 = pPlayers[uActiveCharacter]->pActiveSkills[_evt->v5];
          else
            {
            if ( player_choose == 5 )
              {
              v20 = 0;
              v21 = 3486 * v130 + _evt->v5;
              v136 = 1;
              LOWORD(v21) = pParty->pPlayers[0].pActiveSkills[v21];
              v137 = v21 & 0x40;
              v138 = v21 & 0x80;
              v22 = v21 & 0x100;
              v23 = v21 & 0x3F;
              v139 = v22;
              while ( v23 < v19 || !*(&v136 + _evt->v6) )
                {
                ++v20;
                if ( v20 >= 4 )
                  {
                  ++curr_seq_num;
                  break;
                  }
                }
              curr_seq_num = _evt->v11 - 1;
              ++curr_seq_num;
              break;
              }
LABEL_47:
            //v10 = (ByteArray *)&v5[v9];
            v24 = pParty->pPlayers[0].pActiveSkills[_evt->v5 + 3486 * rand() % 4];
            }
          }
        v136 = 1;
        v137 = v24 & 0x40;
        v138 = v24 & 0x80;
        v139 = v24 & 0x100;
        if ( (v24 & 0x3F) >= v19 && *(&v136 + _evt->v6) )
          {
              curr_seq_num = _evt->v11 - 1;
              ++curr_seq_num;
              break;
          }
        ++curr_seq_num;
        }
        break;

      case EVENT_SpeakNPC:
        if ( canShowMessages )
          {
          //Actor::Actor(&Dst);
          memset(&Dst, 0, 0x344u);
          dword_5B65D0_dialogue_actor_npc_id = _evt->v5 + ((_evt->v6 + ((_evt->v7 + ((unsigned int)_evt->v8 << 8)) << 8)) << 8);
          Dst.sNPC_ID = dword_5B65D0_dialogue_actor_npc_id;
          GameUI_InitializeDialogue(&Dst, false);
          }
        else
          bDialogueUI_InitializeActor_NPC_ID = _evt->v5 + ((_evt->v6 + ((_evt->v7 + ((unsigned int)_evt->v8 << 8)) << 8)) << 8);
        ++curr_seq_num;
        break;
      case EVENT_ChangeEvent:
        if ( EVT_DWORD(_evt->v5) )
          stru_5E4C90_MapPersistVars._decor_events[activeLevelDecoration->_idx_in_stru123] = _evt->v5 - 124;
        else
        {
          v26 = (LevelDecoration *)activeLevelDecoration;
          stru_5E4C90_MapPersistVars._decor_events[activeLevelDecoration ->_idx_in_stru123] = 0;
          v26->uFlags |= LEVEL_DECORATION_INVISIBLE;
        }
        ++curr_seq_num;

        break;
      case EVENT_SetNPCGreeting:
        v27 = EVT_DWORD(_evt->v5);
        pNPCStats->pNewNPCData[v27].uFlags &= 0xFFFFFFFCu;
        pNPCStats->pNewNPCData[v27].greet = EVT_DWORD(_evt->v9);
        ++curr_seq_num;
        break;
            case EVENT_SetNPCTopic:
            {
              //v29 = _evt->v5 + ((_evt->v6 + ((_evt->v7 + ((uint)_evt->v8 << 8)) << 8)) << 8);
              pEventID = _evt->v10 + ((_evt->v11 + ((_evt->v12 + ((uint)_evt->v13 << 8)) << 8)) << 8);
              pNPC_ID = _evt->v5 + ((_evt->v6 + ((_evt->v7 + ((uint)_evt->v8 << 8)) << 8)) << 8);
              pIndex = _evt->v9;
              pNPC = &pNPCStats->pNewNPCData[pNPC_ID];
              if ( pIndex == 0 )
                pNPC->evt_A = pEventID;
              if ( pIndex == 1 )
                pNPC->evt_B = pEventID;
              if ( pIndex == 2 )
                pNPC->evt_C = pEventID;
              if ( pIndex == 3 )
                pNPC->evt_D = pEventID;
              if ( pIndex == 4 )
                pNPC->evt_E = pEventID;
              if ( pIndex == 5 )
                pNPC->evt_F = pEventID;
              if ( pNPC_ID == 8 )
              {
                if ( pEventID == 78 )
                {
                  HouseDialogPressCloseBtn();
                  window_SpeakInHouse->Release();
                  pParty->uFlags &= ~2;
                  if ( EnterHouse(HOUSE_DARK_GUILD_PIT) )
                  {
                    pAudioPlayer->StopChannels(-1, -1);
                    window_SpeakInHouse = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_HouseInterior, 170, 0);
                    window_SpeakInHouse->CreateButton( 61, 424, 31, 0, 2, 94, UIMSG_SelectCharacter, 1, '1',  "", 0);
                    window_SpeakInHouse->CreateButton(177, 424, 31, 0, 2, 94, UIMSG_SelectCharacter, 2, '2',  "", 0);
                    window_SpeakInHouse->CreateButton(292, 424, 31, 0, 2, 94, UIMSG_SelectCharacter, 3, '3',  "", 0);
                    window_SpeakInHouse->CreateButton(407, 424, 31, 0, 2, 94, UIMSG_SelectCharacter, 4, '4',  "", 0);
                    window_SpeakInHouse->CreateButton(  0,   0,  0, 0, 1,  0, UIMSG_CycleCharacters, 0, '\t', "", 0);
                    current_npc_text = pNPCTopics[90].pText;
                  }
                }
              }
              ++curr_seq_num;
            }
            break;
      case EVENT_NPCSetItem:
        sub_448518_npc_set_item(EVT_DWORD(_evt->v5),EVT_DWORD(_evt->v9), _evt->v13);
        ++curr_seq_num;
        break;
      case EVENT_SetActorItem:
        Actor::GiveItem(EVT_DWORD(_evt->v5),EVT_DWORD(_evt->v9), _evt->v13);
        ++curr_seq_num;
        break;
      case EVENT_SetNPCGroupNews:
        pNPCStats->pGroups_copy[_evt->v5 + ((_evt->v6 + ((_evt->v7 + ((uint)_evt->v8 << 8)) << 8)) << 8)] = _evt->v9 + ((uint)_evt->v10 << 8);
        ++curr_seq_num;
        break;
      case EVENT_SetActorGroup:
        __debugbreak();
        *(&pActors[0].uGroup + 0x11000000 * _evt->v8 + 209 * (_evt->v5 + ((_evt->v6 + ((uint)_evt->v7 << 8)) << 8))) = _evt->v9 + ((_evt->v10 + ((_evt->v11 + ((uint)_evt->v12 << 8)) << 8)) << 8);
        ++curr_seq_num;
        break;
      case EVENT_ChangeGroup:
        v38 = _evt->v5 + ((_evt->v6 + ((_evt->v7 + ((uint)_evt->v8 << 8)) << 8)) << 8);
        v39 = _evt->v9 + ((_evt->v10 + ((_evt->v11 + ((uint)_evt->v12 << 8)) << 8)) << 8);
        __debugbreak();
        for ( uint actor_id = 0; actor_id < uNumActors; actor_id++ )
        {
          if ( pActors[actor_id].uGroup == v38 )
            pActors[actor_id].uGroup = v39;
        }
        ++curr_seq_num;
        break;
      case EVENT_ChangeGroupAlly:
        v42 = _evt->v5 + ((_evt->v6 + ((_evt->v7 + ((uint)_evt->v8 << 8)) << 8)) << 8);
        v43 = _evt->v9 + ((_evt->v10 + ((_evt->v11 + ((uint)_evt->v12 << 8)) << 8)) << 8);
        __debugbreak();
        for ( uint actor_id = 0; actor_id < uNumActors; actor_id++ )
        {
          if ( pActors[actor_id].uGroup == v42 )
            pActors[actor_id].uAlly = v43;
        }
        ++curr_seq_num;
        break;
      case EVENT_MoveNPC:
        {
        pNPCStats->pNewNPCData[EVT_DWORD(_evt->v5)].Location2D =EVT_DWORD(_evt->v9);
        if ( window_SpeakInHouse )
          {

          if ( window_SpeakInHouse->par1C == 165 )
            {
            HouseDialogPressCloseBtn();
            pVideoPlayer->Unload();
            window_SpeakInHouse->Release();
            pParty->uFlags &= ~2;
            activeLevelDecoration = (LevelDecoration*)1;
            if ( EnterHouse(HOUSE_BODY_GUILD_ERATHIA) )
              {
              pAudioPlayer->PlaySound((SoundID)0, 0, 0, -1, 0, 0, 0, 0);
              window_SpeakInHouse = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_HouseInterior, 165, 0);
              v48 = window_SpeakInHouse->pControlsHead;
              if ( window_SpeakInHouse->pControlsHead )
                {
                do
                  {
                  v49 = v48->pNext;
                  free(v48);
                  v48 = v49;
                  }
                  while ( v49 );
                }
              window_SpeakInHouse->pControlsHead = 0;
              window_SpeakInHouse->pControlsTail = 0;
              window_SpeakInHouse->uNumControls = 0;
              }
            }
          else
            {
            if ( window_SpeakInHouse->par1C == 553 )
              pVideoPlayer->bLoopPlaying = 0;
            }
          }

        }
        ++curr_seq_num;
        break;
      case EVENT_Jmp:
        curr_seq_num = _evt->v5 - 1;
        ++curr_seq_num;
        v4 = -1;

        break;
      case EVENT_ShowFace:
        if ( _evt->v5 <= 3u ) //someone
          pParty->pPlayers[_evt->v5].PlayEmotion((CHARACTER_EXPRESSION_ID)_evt->v6, 0);
        else if ( _evt->v5 == 4 ) //active
          pParty->pPlayers[uActiveCharacter].PlayEmotion((CHARACTER_EXPRESSION_ID)_evt->v6, 0);
        else if ( _evt->v5 == 5 ) //all players
          {
          for(int i = 0; i < 4; ++i)
            pParty->pPlayers[i].PlayEmotion((CHARACTER_EXPRESSION_ID)_evt->v6, 0);
          }
        else  //random player
          pParty->pPlayers[rand() % 4].PlayEmotion((CHARACTER_EXPRESSION_ID)_evt->v6, 0);
          ++curr_seq_num;
          break;
      case EVENT_CharacterAnimation:
        if ( _evt->v5 <= 3 ) //someone
          pParty->pPlayers[_evt->v5].PlaySound((PlayerSpeech) _evt->v6, 0);
        else if ( _evt->v5 == 4 ) //active
          pParty->pPlayers[uActiveCharacter].PlaySound((PlayerSpeech) _evt->v6, 0);
        else if ( _evt->v5 == 5 ) //all
          for(int i = 0; i < 4; ++i)
            pParty->pPlayers[i].PlaySound((PlayerSpeech) _evt->v6, 0);
        else  //random
          pParty->pPlayers[rand() % 4].PlaySound((PlayerSpeech) _evt->v6, 0);
        ++curr_seq_num;
        break;
      case EVENT_ForPartyMember:
        player_choose = _evt->v5;
        ++curr_seq_num;
        break;
      case EVENT_SummonItem:
        SpriteObject::sub_42F7EB_DropItemAt(_evt->v5 + ((_evt->v6 + ((_evt->v7 + ((uint)_evt->v8 << 8)) << 8)) << 8),
          _evt->v9 + ((_evt->v10 + ((_evt->v11 + ((uint)_evt->v12 << 8)) << 8)) << 8),
          _evt->v13 + ((_evt->v14 + ((_evt->v15 + ((uint)_evt->v16 << 8)) << 8)) << 8),
          _evt->v17 + ((_evt->v18 + ((_evt->v19 + ((uint)_evt->v20 << 8)) << 8)) << 8),
          _evt->v21 + ((_evt->v22 + ((_evt->v23 + ((uint)_evt->v24 << 8)) << 8)) << 8),
          _evt->v25, _evt->v26, 0, 0);
        ++curr_seq_num;
        break;
      case EVENT_Compare:
        pValue = EVT_DWORD(_evt->v7);
        if ( player_choose <= 3 )
        {
          if ( pPlayers[player_choose]->CompareVariable((enum VariableType)EVT_WORD(_evt->v5), pValue) )
          {
           // v124 = -1;
            curr_seq_num = _evt->v11 - 1;
          }
        }
        else if ( player_choose == 4 ) //active
        {
          if ( uActiveCharacter )
          {
            if ( pPlayers[uActiveCharacter]->CompareVariable((enum VariableType)EVT_WORD(_evt->v5), pValue) )
            {
             // v124 = -1;
              curr_seq_num = _evt->v11 - 1;
            }
          }
        }
        else  if ( player_choose == 5 )//all
        {
          v130 = 0;
          for(int i = 1; i < 5; ++i)
          {
            if ( pPlayers[i]->CompareVariable((enum VariableType)EVT_WORD(_evt->v5), pValue) )
            {
             // v124 = -1;
              curr_seq_num = _evt->v11 - 1;
              break;
            }
            ++v130;
          }
        }
        else if ( player_choose == 6 ) //random
        {
          if ( pPlayers[rand() % 4 + 1]->CompareVariable((enum VariableType)EVT_WORD(_evt->v5), pValue) )
          {
           // v124 = -1;
            curr_seq_num = _evt->v11 - 1;
          }
        }
        ++curr_seq_num;
        v4 = -1;
        break;
      case EVENT_IsActorAlive:
         if  (IsActorAlive(EVT_BYTE(_evt->v5), EVT_DWORD(_evt->v6), EVT_BYTE(_evt->v10)))
         {
           //v124 = -1;
           curr_seq_num = _evt->v11 - 1;
         }
         ++curr_seq_num;
         v4 = -1;
         break;
      case EVENT_Substract:
        pValue = EVT_DWORD(_evt->v7);
        /*if ( EVT_WORD(_evt->v5) == VAR_PlayerItemInHands )
        {
          if ( pParty->pPickedItem.uItemID == pValue )//In hand
          {
            pMouse->RemoveHoldingItem();
            ++curr_seq_num;
            v4 = v124;
            break;
          }
          //v67 = (int)pPlayers[uActiveCharacter]->pInventoryMatrix.data();
          for ( v65 = 0; v65 < 126; ++v65 )
          {
            v67 = &pPlayers[uActiveCharacter]->pInventoryMatrix[v65];
            if ( v67 > 0 )
            {
              if ( pPlayers[uActiveCharacter]->pInventoryItemList[v67 - 1].uItemID == pValue )
              {
                pPlayers[uActiveCharacter]->RemoveItemAtInventoryIndex(v65);
                //++curr_seq_num;
                //v4 = v124;
                goto substract;
              }
            }
            //v67 += 4;
          }
          //while ( (signed int)v65 < 126 );
          //v69 = (int)&pPlayers[uActiveCharacter]->pEquipment.pIndices;
          for ( v68 = 0; v68 < 16; ++v68 )
          {
            if ( pPlayers[uActiveCharacter]->pInventoryItemList[pPlayers[uActiveCharacter]->pEquipment.pIndices[v68]].uItemID == pValue )
            {
              pPlayers[uActiveCharacter]->pEquipment.pIndices[v68] = 0;
              //++curr_seq_num;
              //v4 = v124;
              goto substract;
            }
            //v69 += 4;
          }
          for (int i = 1; i < 5; i++)
          {
            //v72 = (int)pPlayers[i]->pInventoryMatrix.data();
            for ( int v71 = 0; v71 < 126; ++v71 )
            {
              v72 = &pPlayers[i]->pInventoryMatrix[v71];
              if ( v72 > 0 )
              {
                if ( pPlayers[i]->pInventoryItemList[v72 - 1].uItemID == pValue )
                {
                  pPlayers[i]->RemoveItemAtInventoryIndex(v71);
                  goto substract;
                }
              }
              //v72 += 4;
            }
            for ( v73 = 0; v73 < 16; ++v73 )
            {
              //v74 = (int)&pPlayers[i]->pEquipment;
              if (pPlayers[i]->pEquipment.pIndices[v73])
              {
                if (pPlayers[i]->pInventoryItemList[pPlayers[i]->pEquipment.pIndices[v73] - 1].uItemID == pValue )
                {
                  pPlayers[i]->pEquipment.pIndices[v73] = 0;
                  //v74 += 4;
                  goto substract;
                }
              }
            }
          }
        }*/
        if ( player_choose <= 3 )
          pParty->pPlayers[player_choose].SubtractVariable((enum VariableType)EVT_WORD(_evt->v5), pValue);
        else if ( player_choose == 4 ) //active
        {
          if ( uActiveCharacter )
            pPlayers[uActiveCharacter]->SubtractVariable((enum VariableType)EVT_WORD(_evt->v5), pValue);
        }
        else  if ( player_choose == 5 )//all
        {
          for(int i = 1; i < 5; ++i)
            pPlayers[i]->SubtractVariable((enum VariableType)EVT_WORD(_evt->v5), pValue);
        }
        else if ( player_choose == 6 ) //random
          pParty->pPlayers[rand() % 4].SubtractVariable((enum VariableType)EVT_WORD(_evt->v5), pValue);
        ++curr_seq_num;
        break;
      case EVENT_Set:
        pValue = EVT_DWORD(_evt->v7);
        if ( player_choose <= 3 )
          pParty->pPlayers[player_choose].SetVariable((enum VariableType)EVT_WORD(_evt->v5), pValue);
        else if ( player_choose == 4 ) //active
        {
          if ( uActiveCharacter )
            pPlayers[uActiveCharacter]->SetVariable((enum VariableType)EVT_WORD(_evt->v5), pValue);
        }
        else if ( player_choose == 5 )//all
        {
          //recheck v130
          for ( int i = 1; i < 5; ++i )
            pPlayers[i]->SetVariable((enum VariableType)EVT_WORD(_evt->v5), pValue);
        }
        else if ( player_choose == 6 ) //random
          pParty->pPlayers[rand() % 4].SetVariable((enum VariableType)EVT_WORD(_evt->v5), pValue);
        ++curr_seq_num;
        break;
      case EVENT_Add:
        pValue = EVT_DWORD(_evt->v7);
        if ( player_choose <= 3 )
        {
          pPlayer = &pParty->pPlayers[player_choose];
          pPlayer->AddVariable((enum VariableType)EVT_WORD(_evt->v5), pValue);
        }
        else if ( player_choose == 4 ) //active
        {
          if ( uActiveCharacter )
            pPlayers[uActiveCharacter]->AddVariable((enum VariableType)EVT_WORD(_evt->v5), pValue);
        }
        else if ( player_choose == 5 )//all
        {
          for(int i = 1; i < 5; ++i)
            pPlayers[i]->AddVariable((enum VariableType)EVT_WORD(_evt->v5), pValue);
        }
        else if ( player_choose == 6 ) //random
          pParty->pPlayers[rand() % 4].AddVariable((enum VariableType)EVT_WORD(_evt->v5), pValue);
        v83 = EVT_WORD(_evt->v5);
        if (v83 == 21 ||  // gold well on emerald isle
            v83 == 22 || v83 == 23 || v83 == 24 )
        {
          //__debugbreak(); // bonfire
          viewparams->bRedrawGameUI = true;
        }
        ++curr_seq_num;
        break;
      case EVENT_InputString:
        if ( !entry_line )
        {
          strcpy(GameUI_Footer_TimedString.data(), &pLevelStr[pLevelStrOffsets[EVT_DWORD(_evt->v5 )]]);
          sub_4451A8_press_any_key(uEventID, curr_seq_num, 26);
          if ( v133 == 1 )
            OnMapLeave();
          return;
        }
        v84 = _evt->v13 + ((_evt->v14 + ((_evt->v15 + ((uint)_evt->v16 << 8)) << 8)) << 8);
        if ( !_stricmp(GameUI_Footer_TimedString.data(), &pLevelStr[pLevelStrOffsets[_evt->v9 + ((_evt->v10 + ((_evt->v11 + ((uint)_evt->v12 << 8)) << 8)) << 8)]])
          || !_stricmp(GameUI_Footer_TimedString.data(), &pLevelStr[pLevelStrOffsets[v84]]) )
        {
          v11 = _evt->v17;
          curr_seq_num = v11 - 1;
        }
        ++curr_seq_num;
        v4 = -1;
        break;
      case EVENT_RandomGoTo:
        //v124 = -1;
        v11 = (unsigned __int8)*(&_evt->v5 + rand() % ((_evt->v5 != 0) + (_evt->v6 != 0) + (_evt->v7 != 0) + (_evt->v8 != 0) + (_evt->v9 != 0)
          + (_evt->v10 != 0)));
                curr_seq_num = v11 - 1;
                ++curr_seq_num;
                v4 = -1;
                break;
      case EVENT_ReceiveDamage:
        if ( (unsigned __int8)_evt->v5 <= 3 )
        {
          pParty->pPlayers[(unsigned __int8)_evt->v5].ReceiveDamage(EVT_DWORD(_evt->v7 ), (DAMAGE_TYPE)_evt->v6);
          ++curr_seq_num;
          break;
        }
        if ( _evt->v5 == 4 )
        {
          if ( !uActiveCharacter )
          {
            ++curr_seq_num;
            break;
          }
          pPlayers[uActiveCharacter]->ReceiveDamage(EVT_DWORD(_evt->v7 ), (DAMAGE_TYPE)_evt->v6);
          ++curr_seq_num;
          break;
        }
        if ( _evt->v5 != 5 )
        {
          pParty->pPlayers[rand() % 4].ReceiveDamage(EVT_DWORD(_evt->v7 ), (DAMAGE_TYPE)_evt->v6);
          ++curr_seq_num;
          break;
        }
        for ( uint pl_id = 0;pl_id < 4; pl_id++ )
          pParty->pPlayers[pl_id].ReceiveDamage(EVT_DWORD(_evt->v7 ), (DAMAGE_TYPE)_evt->v6);
          ++curr_seq_num;
          break;
      case EVENT_ToggleIndoorLight:
        pIndoor->ToggleLight(EVT_DWORD(_evt->v5 ), _evt->v9);
        ++curr_seq_num;
        break;
      case EVENT_SetFacesBit:
        sub_44892E_set_faces_bit(EVT_DWORD(_evt->v5),EVT_DWORD(_evt->v9), _evt->v13);
        ++curr_seq_num;
        break;
      case EVENT_ToggleChestFlag:
        Chest::ToggleFlag(EVT_DWORD(_evt->v5 ), EVT_DWORD(_evt->v9 ), _evt->v13);
        ++curr_seq_num;
        break;
      case EVENT_ToggleActorFlag:
        Actor::ToggleFlag(EVT_DWORD(_evt->v5 ), EVT_DWORD(_evt->v9 ), _evt->v13);
        ++curr_seq_num;
        break;
      case EVENT_ToggleActorGroupFlag:
        ToggleActorGroupFlag(EVT_DWORD(_evt->v5 ),  EVT_DWORD(_evt->v9 ), _evt->v13);
        ++curr_seq_num;
        break;
      case EVENT_SetSnow:
        if ( !_evt->v5 )
          pWeather->bRenderSnow = _evt->v6 != 0;
        ++curr_seq_num;
        break;
      case EVENT_StatusText:
        v90 = EVT_DWORD(_evt->v5 );
        if ( activeLevelDecoration )
        {
          if ( activeLevelDecoration == (LevelDecoration*)1 )
            current_npc_text = pNPCTopics[v90-1].pText;//(&dword_721664)[8 * v90];
          if ( canShowMessages == 1 )
            {
            v91 = pNPCTopics[v90-1].pText;//(&dword_721664)[8 * v90];
            //LABEL_248:
            ShowStatusBarString(v91, 2);
            }
        }
        else
        {
          if ( canShowMessages == 1 )
          {
            v91 = &pLevelStr[pLevelStrOffsets[v90]];
            ShowStatusBarString(v91, 2);
          }
        }
        ++curr_seq_num;
        break;
      case EVENT_ShowMessage:
        if ( activeLevelDecoration )
        {
          current_npc_text = pNPCTopics[EVT_DWORD(_evt->v5 )-1].pText;//(&dword_721664)[8 * v92];
          byte_5B0938[0] = 0;
        }
        else
          strcpy(byte_5B0938.data(), &pLevelStr[pLevelStrOffsets[EVT_DWORD(_evt->v5 )]]);
        ++curr_seq_num;
        break;
      case EVENT_CastSpell:
              EventCastSpell(_evt->v5, _evt->v6, _evt->v7, EVT_DWORD(_evt->v8 ),
                    EVT_DWORD(_evt->v12 ), EVT_DWORD(_evt->v16 ), EVT_DWORD(_evt->v20 ),
                    EVT_DWORD(_evt->v24 ), EVT_DWORD(_evt->v28 ));
              ++curr_seq_num;
              break;
      case EVENT_SetTexture:
        sub_44861E_set_texture(EVT_DWORD(_evt->v5 ), (char *)&_evt->v9);
        ++curr_seq_num;
        break;
      case EVENT_SetSprite:
        SetDecorationSprite(EVT_DWORD(_evt->v5 ), _evt->v9, (char *)&_evt->v10);
        ++curr_seq_num;
        break;
      case EVENT_SummonMonsters:
        sub_448CF4_spawn_monsters(_evt->v5, _evt->v6, _evt->v7,EVT_DWORD(_evt->v8 ),
                    EVT_DWORD(_evt->v12 ), EVT_DWORD(_evt->v16 ), EVT_DWORD(_evt->v20 ),
                    EVT_DWORD(_evt->v24 ));
        ++curr_seq_num;
        break;
      case EVENT_MouseOver:
      case EVENT_LocationName:
        --curr_seq_num;
        ++curr_seq_num;
        break;
      case EVENT_ChangeDoorState:
        Door_switch_animation(_evt->v5, _evt->v6);
        ++curr_seq_num;
        break;
      case EVENT_OpenChest:
        if ( !Chest::Open(_evt->v5) )
        {
          if ( v133 == 1 )
            OnMapLeave();
          return;
        }
        ++curr_seq_num;
        break;
      case EVENT_MoveToMap:
        v94 = _evt->v5 + ((_evt->v6 + ((_evt->v7 + ((uint)_evt->v8 << 8)) << 8)) << 8);
        v135 = _evt->v5 + ((_evt->v6 + ((_evt->v7 + ((uint)_evt->v8 << 8)) << 8)) << 8);
        v132 = _evt->v9 + ((_evt->v10 + ((_evt->v11 + ((uint)_evt->v12 << 8)) << 8)) << 8);
        v126 = _evt->v13 + ((_evt->v14 + ((_evt->v15 + ((uint)_evt->v16 << 8)) << 8)) << 8);
        v129 = _evt->v17 + ((_evt->v18 + ((_evt->v19 + ((uint)_evt->v20 << 8)) << 8)) << 8);
        v95 = _evt->v21 + ((_evt->v22 + ((_evt->v23 + ((uint)_evt->v24 << 8)) << 8)) << 8);
        v96 = _evt->v25;
        v97 = v96 + ((_evt->v26 + ((_evt->v27 + ((uint)_evt->v28 << 8)) << 8)) << 8);
        v134 = v96 + ((_evt->v26 + ((_evt->v27 + ((uint)_evt->v28 << 8)) << 8)) << 8);
        if ( _evt->v29 || _evt->v30 )
          {
            pRenderer->Sub01();
          TransitionUI_Load(_evt->v29, _evt->v30, v135, v132, v126, v129, v95, v134, (char *)&_evt->v31);
          dword_5C3418 = uEventID;
          dword_5C341C = curr_seq_num + 1;
          if ( v133 == 1 )
            OnMapLeave();
          return;
          }
        _5B65AC_npcdata_fame_or_other = _evt->v9 + ((_evt->v10 + ((_evt->v11 + ((uint)_evt->v12 << 8)) << 8)) << 8);
        _5B65A8_npcdata_uflags_or_other = v94;
        _5B65B0_npcdata_rep_or_other = v126;
        if ( v129 == -1 )
          v98 = _5B65B4_npcdata_loword_house_or_other;
        else
        {
          v98 = v129 & stru_5C6E00->uDoublePiMask;
          _5B65B4_npcdata_loword_house_or_other = v129 & stru_5C6E00->uDoublePiMask;
        }
        v99 = (char *)&_evt->v31;
        _5B65B8_npcdata_hiword_house_or_other = v95;
        dword_5B65BC = v97;
        v100 = v94 | v132 | v126 | v95 | v97 | v98;
        dword_5B65C0 = v100;
        if ( *v99 == 48 )
          {
          if ( v100 )
            {
            pParty->vPosition.x = v135;
            pParty->vPosition.y = v132;
            pParty->vPosition.z = v126;
            pParty->uFallStartY = v126;
            if ( _5B65B4_npcdata_loword_house_or_other != -1 )
              pParty->sRotationY = _5B65B4_npcdata_loword_house_or_other;
            _5B65B4_npcdata_loword_house_or_other = -1;
            pParty->sRotationX = v95;
            pParty->uFallSpeed = v134;
            dword_5B65C0 = 0;
            dword_5B65BC = 0;
            _5B65B8_npcdata_hiword_house_or_other = 0;
            _5B65B0_npcdata_rep_or_other = 0;
            _5B65AC_npcdata_fame_or_other = 0;
            _5B65A8_npcdata_uflags_or_other = 0;
            v106 = 232;
            pAudioPlayer->PlaySound((SoundID)v106, 0, 0, -1, 0, 0, 0, 0);
            }
          }
        else
          {
          pGameLoadingUI_ProgressBar->uType = (GUIProgressBar::Type)((activeLevelDecoration == NULL) + 1);
          Transition_StopSound_Autosave(v99, MapStartPoint_Party);
          v133 = 1;
          if ( pCurrentScreen == SCREEN_HOUSE )
            {
            if ( uGameState == GAME_STATE_CHANGE_LOCATION )
              {
              pAudioPlayer->StopChannels(-1, -1);
              dialog_menu_id = HOUSE_DIALOGUE_NULL;
              while ( HouseDialogPressCloseBtn() )
                ;
              pVideoPlayer->Unload();
              window_SpeakInHouse->Release();
              window_SpeakInHouse = 0;
              if ( pMessageQueue_50CBD0->uNumMessages )
                pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
              pCurrentScreen = SCREEN_GAME;
              viewparams->bRedrawGameUI = 1;
              pDialogueNPCCount = 0;
              pDialogueWindow->Release();
              dialog_menu_id = HOUSE_DIALOGUE_NULL;
              pDialogueWindow = 0;
              pIcons_LOD->SyncLoadedFilesCount();
              }
            OnMapLeave();
            return;
            }
          }
        ++curr_seq_num;
        break;
      case EVENT_PlaySound:
        v110 = _evt->v13 + ((_evt->v14 + ((_evt->v15 + ((uint)_evt->v16 << 8)) << 8)) << 8);
        v109 = _evt->v9 + ((_evt->v10 + ((_evt->v11 + ((uint)_evt->v12 << 8)) << 8)) << 8);
        v106 = _evt->v5 + ((_evt->v6 + ((_evt->v7 + ((uint)_evt->v8 << 8)) << 8)) << 8);
        pAudioPlayer->PlaySound((SoundID)v106, 0, 0, v109, v110, 0, 0, 0);
        ++curr_seq_num;
        break;
      case EVENT_GiveItem:
        item.Reset();
        v102 = _evt->v7 + ((_evt->v8 + ((_evt->v9 + ((uint)_evt->v10 << 8)) << 8)) << 8);
        pItemsTable->GenerateItem(_evt->v5, _evt->v6, &item);
        if ( v102 )
          item.uItemID = v102;
        pParty->SetHoldingItem(&item);
        ++curr_seq_num;
        break;
      case EVENT_SpeakInHouse:
        if ( EnterHouse((enum HOUSE_ID)EVT_DWORD(_evt->v5)))
          {
            pRenderer->Sub01();
          pAudioPlayer->PlaySound((SoundID)0, 0, 0, -1, 0, 0, 0, 0);
          pAudioPlayer->PlaySound(SOUND_EnteringAHouse, 814, 0, -1, 0, 0, 0, 0);
          v104 = 187;
          if ( uCurrentHouse_Animation != 167 )
            v104 = EVT_DWORD(_evt->v5);
          window_SpeakInHouse = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_HouseInterior, v104, 0);
          window_SpeakInHouse->CreateButton( 61, 424, 31, 0, 2, 94, UIMSG_SelectCharacter, 1,  '1', "", 0);
          window_SpeakInHouse->CreateButton(177, 424, 31, 0, 2, 94, UIMSG_SelectCharacter, 2,  '2', "", 0);
          window_SpeakInHouse->CreateButton(292, 424, 31, 0, 2, 94, UIMSG_SelectCharacter, 3,  '3', "", 0);
          window_SpeakInHouse->CreateButton(407, 424, 31, 0, 2, 94, UIMSG_SelectCharacter, 4,  '4', "", 0);
          window_SpeakInHouse->CreateButton(  0,   0,  0, 0, 1,  0, UIMSG_CycleCharacters, 0, '\t', "", 0);
          }
        ++curr_seq_num;
        break;
      case EVENT_PressAnyKey:
        sub_4451A8_press_any_key(uEventID, curr_seq_num + 1, 33);
        if ( v133 == 1 )
          OnMapLeave();
        return;
      case EVENT_Exit:
        if ( v133 == 1 )
              OnMapLeave();
        return;
      default:
        ++curr_seq_num;
        break;
        }
      }
    }
    if ( v133 == 1 )
      OnMapLeave();
    return;
  }

//----- (00444732) --------------------------------------------------------
char *GetEventHintString(unsigned int uEventID)
{
  signed int event_index; // edx@1
  int event_pos; // esi@4
  char *result; // eax@6
  unsigned int str_index; // eax@9
  int i; // esi@11
 _evt_raw*  test_evt;
 _evt_raw*  last_evt;

  event_index = 0;
  if ( uLevelEVT_NumEvents <= 0 )
    return NULL;

    //v2 = (char *)&pLevelEVT_Index[0].uEventOffsetInEVT;
    while ( 1 )
    {
      if ( pLevelEVT_Index[event_index].uEventID == uEventID )
      {
        test_evt=(_evt_raw*)&pLevelEVT[pLevelEVT_Index[event_index].uEventOffsetInEVT];
        last_evt=test_evt;
        event_pos = pLevelEVT_Index[event_index+1].uEventOffsetInEVT;
        if ( test_evt->_e_type == EVENT_MouseOver )
          break;
      }
      ++event_index;
      if ( event_index >= uLevelEVT_NumEvents )
       return NULL;
    }
    test_evt=(_evt_raw*)&pLevelEVT[event_pos];
    if ( test_evt->_e_type== EVENT_SpeakInHouse )
    {
      str_index = EVT_DWORD(test_evt->v5);
      result = (char *)p2DEvents[str_index - 1].pName;
    }
    else
    {
      for ( i = event_index + 1; pLevelEVT_Index[i].uEventID  == uEventID; ++i )
      {
        event_pos = pLevelEVT_Index[i].uEventOffsetInEVT;
        test_evt=(_evt_raw*)&pLevelEVT[event_pos];
        if ( test_evt->_e_type == EVENT_SpeakInHouse )
        {
          str_index = EVT_DWORD(test_evt->v5);
          if ( str_index < 600 )
            return (char *)p2DEvents[str_index - 1].pName;
        }
      }
      result = &pLevelStr[pLevelStrOffsets[EVT_BYTE(last_evt->v5)]];
    }

  return result;
}
