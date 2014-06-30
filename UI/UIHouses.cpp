#define _CRT_SECURE_NO_WARNINGS
#include "UIGuilds.h"
#include "UIPartyCreation.h"
#include "UIShops.h"
#include "..\GUIButton.h"
#include "..\mm7_unsorted_subs.h"
#include "..\SaveLoad.h"
#include "..\Texture.h"
#include "..\mm7_data.h"
#include "..\ErrorHandling.h"
#include "UIHouses.h"
#include "..\Party.h"
#include "..\texts.h"
#include "..\Events.h"
#include "..\Arcomage.h"
#include "..\LOD.h"
#include "..\Mouse.h"
#include "..\GUIWindow.h"
#include "..\GUIFont.h"
#include "..\Overlays.h"
#include "..\Outdoor.h"
#include "..\Outdoor_stuff.h"
#include "..\AudioPlayer.h"
#include "..\VideoPlayer.h"
#include "..\Monsters.h"
#include "..\Viewport.h"
#include "..\Keyboard.h"
#include "..\MapInfo.h"
#include "..\Log.h"
#include "..\Game.h"
#include "..\CastSpellInfo.h"

#include "../Level/Decoration.h"

#include "..\stru159.h"
int uHouse_ExitPic; // weak

int dword_591080; // weak

BuildingType in_current_building_type; // 00F8B198
HOUSE_DIALOGUE_MENU dialog_menu_id; // 00F8B19C

#pragma pack(push, 1)
struct stru365_travel_info
{
  unsigned char uMapInfoID;
  unsigned char pSchedule[7];
  unsigned int uTravelTime;
  int arrival_x;
  int arrival_y;
  int arrival_z;
  int arrival_rot_y;
  unsigned int  uQuestBit;  // quest bit required to set for this travel option to be enabled; otherwise 0
};
#pragma pack(pop)
stru365_travel_info transport_schedule[35] =  // 004F09B0
{
  {MAP_STEADWICK,       {1, 0, 1, 0, 1, 0, 0},  2,  -18048,    4636,   833,  1536,  0},
  {MAP_PIERPONT,        {0, 1, 0, 1, 0, 1, 0},  2,   -2527,   -6773,  1153,   896,  0},
  {MAP_TATALIA,         {1, 0, 1, 0, 1, 0, 0},  2,    4730,  -10580,   320,  1024,  0},
  {MAP_HARMONDALE,      {0, 1, 0, 1, 0, 1, 0},  2,   -5692,   11137,     1,  1024,  0},
  {MAP_DEYJA,           {1, 0, 0, 1, 0, 0, 0},  3,    7227,  -16007,  2625,   640,  0},
  {MAP_BRAKADA_DESERT,  {0, 0, 1, 0, 0, 1, 0},  3,    8923,   17191,     1,   512,  0},
  {MAP_AVLEE,           {1, 0, 1, 0, 1, 0, 0},  3,   17059,   12331,   512,  1152,  0},
  {MAP_DEYJA,           {0, 1, 0, 0, 1, 0, 1},  2,    7227,  -16007,  2625,   640,  0},
  {MAP_HARMONDALE,      {0, 1, 0, 1, 0, 1, 0},  2,   -5692,   11137,     1,  1024,  0},
  {MAP_STEADWICK,       {1, 0, 1, 0, 1, 0, 0},  3,  -18048,    4636,   833,  1536,  0},
  {MAP_PIERPONT,        {0, 1, 0, 1, 0, 1, 0},  2,   -2527,   -6773,  1153,   896,  0},
  {MAP_STEADWICK,       {1, 0, 1, 0, 1, 0, 1},  3,  -18048,    4636,   833,  1536,  0},
  {MAP_HARMONDALE,      {0, 1, 0, 0, 0, 1, 0},  5,   -5692,   11137,     1,  1024,  0},
  {MAP_STEADWICK,       {0, 1, 0, 1, 0, 1, 0},  2,  -18048,    4636,   833,  1536,  0},
  {MAP_PIERPONT,        {0, 1, 0, 1, 0, 1, 0},  3,   -2527,  -16007,  1153,   896,  0},
  {MAP_DEYJA,           {0, 0, 1, 0, 0, 0, 1},  5,    7227,  -16007,  2625,   640,  0},
  {MAP_TATALIA,         {0, 1, 0, 1, 0, 1, 0},  2,   -2183,   -6941,    97,     0,  0},
  {MAP_AVLEE,           {1, 0, 0, 0, 1, 0, 0},  4,    7913,    9476,   193,     0,  0},
  {MAP_EVENMORN_ISLE,   {0, 0, 0, 0, 0, 0, 1},  7,   15616,    6390,   193,  1536,  PARTY_QUEST_EVENMORN_MAP_FOUND},
  {MAP_BRAKADA_DESERT,  {0, 0, 1, 0, 0, 0, 0},  6,   19171,  -19722,   193,  1024,  0},
  {MAP_AVLEE,           {0, 1, 0, 1, 0, 1, 0},  3,    7913,    9476,   193,     0,  0},
  {MAP_BRAKADA_DESERT,  {1, 0, 1, 0, 0, 0, 0},  6,   19171,  -19722,   193,  1024,  0},
  {MAP_TATALIA,         {1, 0, 1, 0, 1, 0, 0},  4,   -2183,   -6941,    97,     0,  0},
  {MAP_PIERPONT,        {0, 0, 0, 0, 0, 1, 0},  6,    -709,  -14087,   193,  1024,  0},
  {MAP_STEADWICK,       {0, 0, 0, 0, 0, 0, 1},  6,  -10471,   13497,   193,  1536,  0},
  {MAP_EVENMORN_ISLE,   {0, 1, 0, 1, 0, 0, 0},  1,   15616,    6390,   193,  1536,  PARTY_QUEST_EVENMORN_MAP_FOUND},
  {MAP_BRAKADA_DESERT,  {0, 1, 0, 1, 0, 0, 0},  1,   19171,  -19722,   193,  1024,  0},
  {MAP_STEADWICK,       {0, 1, 0, 1, 0, 1, 0},  2,  -10471,   13497,   193,  1536,  0},
  {MAP_BRAKADA_DESERT,  {1, 0, 1, 0, 0, 0, 0},  4,   19171,  -19722,   193,  1024,  0},
  {MAP_EVENMORN_ISLE,   {0, 0, 0, 0, 0, 0, 1},  5,   15616,    6390,   193,  1536,  PARTY_QUEST_EVENMORN_MAP_FOUND},
  {MAP_AVLEE,           {0, 0, 0, 0, 1, 0, 0},  5,    7913,    9476,   193,     0,  0},
  {MAP_STEADWICK,       {0, 1, 0, 0, 0, 1, 0},  4,  -10471,   13497,   193,  1536,  0},
  {MAP_PIERPONT,        {1, 0, 1, 0, 1, 0, 0},  3,    -709,  -14087,   193,  1024,  0},
  {MAP_TATALIA,         {0, 0, 0, 1, 0, 0, 0},  5,   -2183,   -6941,    97,     0,  0},
  {MAP_ARENA,           {0, 0, 0, 0, 0, 0, 1},  4,    3844,    2906,   193,  512,   0}
};

unsigned char transport_routes[20][4] =
{
  {  0,   1,   1,  34},  // HOUSE_STABLES_HARMONDALE
  {  2,   3,   4,   5},  // HOUSE_STABLES_STEADWICK
  {  6,   7,   8,   8},  // HOUSE_STABLES_TULAREAN_FOREST
  {  9,  10,  10,  10},  // HOUSE_STABLES_DEYJA
  { 11,  11,  12,  12},  // HOUSE_STABLES_BRACADA_DESERT
  { 13,  13,  13,  13},  // HOUSE_STABLES_TATALIA
  { 14,  14,  15,  15},  // HOUSE_STABLES_AVLEE
  {255, 255, 255, 255},  // HOUSE_STABLES_61
  {255, 255, 255, 255},  // HOUSE_STABLES_62
  {255, 255, 255, 255},  // HOUSE_BOATS_EMERALD_ISLE
  { 16,  17,  18,  19},  // HOUSE_BOATS_ERATHIA
  { 18,  20,  21,  21},  // HOUSE_BOATS_TULAREAN_FOREST
  { 22,  23,  24,  25},  // HOUSE_BOATS_BRACADA_DESERT
  { 22,  22,  23,  23},  // HOUSE_BOATS_EVENMORN_ISLAND
  {255, 255, 255, 255},  // HOUSE_BOATS_68
  { 27,  28,  29,  30},  // HOUSE_BOATS_TATALIA
  { 31,  32,  33,  33},  // HOUSE_BOATS_AVLEE
  { 24,  24,  24,  24},  // HOUSE_BOATS_71
  {255, 255, 255, 255},  // HOUSE_BOATS_72
  {255, 255, 255, 255}   // HOUSE_BOATS_73
};

std::array<const stru159, 196> pAnimatedRooms = //0x4E5F70
{{
        {"",                            0x4, 0x1F4, 0, 0, 0},
        {"Human Armor01",               0x20, 0x2C0, 2, 58, 0},
        {"Necromancer Armor01",         0x20, 0x2D7, 2, 70, 0 },
        {"Dwarven Armor01",             0x20, 0x2EE, 2,  5, 0 },
        {"Wizard Armor",                0x20, 0x3BD, 2, 19, 0 },
        {"Warlock Armor",               0x20, 0x2D6, 2, 35, 0 },
        {"Elf Armor",                   0x20, 0x2BC, 2, 79, 0 },
        {"Human Alchemisht01",          0xE,  0x2BE, 4, 95, 0 },
        {"Necromancer Alchemist01",     0xE,  0x2D6, 4, 69, 0 },
        {"Dwarven Achemist01",          0xE,  0x387, 4,  4, 0 },
        {"Wizard Alchemist",            0xE,  0x232, 4, 25, 0 },
        {"Warlock Alchemist",           0xE,  0x2BE, 4, 42, 0 },
        {"Elf Alchemist",               0xE,  0x38A, 4, 84, 0 },
        {"Human Bank01",                0x6, 0x384, 22, 52, 0 },
        {"Necromancer Bank01",          0x6, 0x2D8, 22, 71, 0 },
        {"Dwarven Bank",                0x6, 0x2F3, 22,  6, 0 },
        {"Wizard Bank",                 0x6, 0x3BA, 22, 20, 0 },
        {"Warlock Bank",                0x6, 0x39F, 22, 36, 0 },
        {"Elf Bank",                    0x6, 0x2BC, 22, 71, 0 },
        {"Boat01",                      0xF,  0x4C, 28, 53, 3 },
        {"Boat01d",                     0xF,  0x4C, 28, 53, 3 },
        {"Human Magic Shop01",          0xA, 0x2C8,  3, 54, 0 },
        {"Necromancer Magic Shop01",    0xE, 0x2DC,  3, 66, 0 },
        {"Dwarven Magic Shop01",        0x2A, 0x2EF, 3, 91, 0 },
        {"Wizard Magic Shop",           0x1E, 0x2DF, 3, 15, 0 },
        {"Warlock Magic Shop",          0x7,  0x3B9, 3, 15, 0 },
        {"Elf Magic Shop",              0x24, 0x2CC, 3, 82, 0 },
        {"Human Stables01",             0x21, 0x31,  27, 48, 3 },
        {"Necromancer Stables",         0x21, 0x2DD, 27, 67, 3 },
        {"",                            0x21, 0x2F0, 27, 91, 3 },
        {"Wizard Stables",              0x21, 0x3BA, 27, 16, 3 },
        {"Warlock Stables",             0x21, 0x181, 27, 77, 3 },
        {"Elf Stables",                 0x21, 0x195, 27, 77, 3 },
        {"Human Tavern01",              0xD, 0x2C2,  21, 49, 0 },
        {"Necromancer Tavern01",        0xD, 0x3B0,  21, 57, 0 },
        {"Dwarven Tavern01",            0xD, 0x2FE,  21, 94, 0 },
        {"Wizard Tavern",               0xD, 0x3BB,  21, 17, 0 },
        {"Warlock Tavern",              0xD, 0x3A8,  21, 33, 0 },
        {"Elf Tavern",                  0xD, 0x2CD,  21, 78, 0 },
        {"Human Temple01",              0x24, 0x2DB, 23, 50, 3 },
        {"Necromancer Temple",          0x24, 0x2DF, 23, 60, 3 },
        {"Dwarven Temple01",            0x24, 0x2F1, 23, 86, 3 },
        {"Wizard Temple",               0x24, 0x2E0, 23, 10, 3 },
        {"Warlock Temple",              0x24, 0x3A4, 23, 27, 3 },
        {"Elf Temple",                  0x24, 0x2CE, 23, 72, 3 },
        {"Human Town Hall",             0x10, 0x39C, 17, 14, 0 },
        {"Necromancer Town Hall01",     0x10, 0x3A4, 17, 61, 0 },
        {"Dwarven Town Hall",           0x10, 0x2DB, 17, 88, 0 },
        {"Wizard Town Hall",            0x10, 0x3BD, 17, 11, 0 },
        {"Warlock Town Hall",           0x10, 0x2DB, 17, 28, 0 },
        {"Elf Town Hall",               0x10, 0x27A, 17, 73, 0 },
        {"Human Training Ground01",     0x18, 0x2C7, 30, 44, 0 },
        {"Necromancer Training Ground", 0x18, 0x3AD, 30, 62, 0 },
        {"Dwarven Training Ground",     0x18, 0x2F2, 30, 89, 0 },
        {"Wizard Training Ground",      0x18, 0x3A3, 30, 12, 0 },
        {"Warlock Training Ground",     0x18, 0x3A6, 30, 29, 0 },
        {"Elf Training Ground",         0x18, 0x19F, 30, 74, 0 },
        {"Human Weapon Smith01",        0x16, 0x2C1, 1, 45, 4 },
        {"Necromancer Weapon Smith01",  0x16, 0x2D9, 1, 63, 4 },
        {"Dwarven Weapon Smith01",      0x16, 0x2EE, 1, 82, 4 },
        {"Wizard Weapon Smith",         0x16, 0x2D5, 1, 13, 4 },
        {"Warlock Weapon Smith",        0x16, 0x2D7, 1, 23, 4 },
        {"Elf Weapon Smith",            0x16, 0x2CA, 1, 75, 4 },
        {"Air Guild",                   0x1D, 0xA4,   6, 1, 3 },
        {"Body Guild",                  0x19, 0x3BF, 11, 2, 0 },
        {"DarkGuild",                   0x19, 0x2D1, 13, 3, 0 },
        {"EarthGuild",                  0x19, 0x2CB, 8, 83, 0 },
        {"Fire Guild",                  0x1C, 0x2BF, 5, 56, 0 },
        {"Light Guild",                 0x1C, 0x2D5, 12, 46, 0 },
        {"Mind Guild",                  0x1C, 0xE5,  10, 40, 0 },
        {"Spirit Guild",                0x1C, 0x2D2,  9, 41, 0 },
        {"Water Guild",                 0x1B, 0x2D3,  7, 24, 0 },
        {"Lord And Judge Out01",        1, 0,        29, 39, 0 },
        {"Human Poor House 1",          8, 0,        29,  0, 0 },
        {"Human Poor House 2",          8, 0,        29,  0, 0 },
        {"Human Poor House 3",          8, 0,        29,  0, 0 },
        {"Human Medium House 1",        8, 0,        29,  0, 0 },
        {"Human Medium House 2",        8, 0,        29,  0, 0 },
        {"Human Medium House 3",        8, 0,        29,  0, 0 },
        {"Human Rich House 1",          8, 0,        29,  0, 0 },
        {"Human Rich House 2",          8, 0,        29,  0, 0 },
        {"Human Rich House 3",          8, 0,        29,  0, 0 },
        {"Elven Poor House 1",          8, 0,        29,  0, 0 },
        {"Elven Poor House 2",          8, 0,        29,  0, 0 },
        {"Elven Poor House 3",          8, 0,        29,  0, 0 },
        {"Elven Medium House 1",        8, 0,        29,  0, 0 },
        {"Elven Medium House 2",        8, 0,        29,  0, 0 },
        {"Elven Medium House 3",        8, 0,        29,  0, 0 },
        {"Elven Rich House 1",          8, 0,        29,  0, 0 },
        {"Elven Rich House 2",          8, 0,        29,  0, 0 },
        {"Elven Rich House 3",          8, 0,        29,  0, 0 },
        {"Dwarven Poor House 1",        8, 0,        29,  0, 0 },
        {"Dwarven Poor House 2",        8, 0,        29,  0, 0 },
        {"Dwarven Poor House 3",        8, 0,        29,  0, 0 },
        {"Dwarven Medium House 1",      8, 0,        29,  0, 0 },
        {"Dwarven Medium house 2",      8, 0,        29,  0, 0 },
        {"Dwarven Medium House 3",      8, 0,        29,  0, 0 },
        {"Dwarven Rich House 1",        8, 0,        29,  0, 0 },
        {"Dwarven Rich House 2",        8, 0,        29,  0, 0 },
        {"Dwarven Rich House 3",        8, 0,        29,  0, 0 },
        {"Wizard Poor House 1",         8, 0,        29,  0, 0 },
        {"Wizard Poor House 2",         8, 0,        29,  0, 0 },
        {"Wizard Poor House 3",         8, 0,        29,  0, 0 },
        {"Wizard Medium House 1",       8, 0,        29,  0, 0 },
        {"Wizard Medium House 2",       8, 0,        29,  0, 0 },
        {"Wizard Mdeium House 3",       8, 0,        29,  0, 0 },
        {"Wizard Rich House 1",         8, 0,        29,  0, 0 },
        {"Wizard Rich House 2",         8, 0,        29,  0, 0 },
        {"Wizard Rich House 3",         8, 0,        29,  0, 0 },
        {"Necromancer Poor House 1",    8, 0,        29,  0, 0 },
        {"Necromancer Poor House 2",    8, 0,        29,  0, 0 },
        {"Necromancer Poor House 3",    8, 0,        29,  0, 0 },
        {"Necromancer Medium House 1",  8, 0,        29,  0, 0 },
        {"Necromancer Medium House 2",  8, 0,        29,  0, 0 },
        {"Necromancer Mdeium House 3",  8, 0,        29,  0, 0 },
        {"Necromancer Rich House 1",    8, 0,        29,  0, 0 },
        {"Necromancer Rich House 2",    8, 0,        29,  0, 0 },
        {"Necromancer Rich House 3",    8, 0,        29,  0, 0 },
        {"Warlock Poor House 1",        8, 0,        29,  0, 0 },
        {"Warlock Poor House 2",        8, 0,        29,  0, 0 },
        {"Warlock Poor House 3",        8, 0,        29,  0, 0 },
        {"Warlock Medium House 1",      8, 0,        29,  0, 0 },
        {"Warlock Medium House 2",      8, 0,        29,  0, 0 },
        {"Warlock Mdeium House 3",      8, 0,        29,  0, 0 },
        {"Warlock Rich House 1",        8, 0,        29,  0, 0 },
        {"Warlock Rich House 2",        8, 0,        29,  0, 0 },
        {"Warlock Rich House 3",        8, 0,        29,  0, 0 },
        {"Out01 Temple of the Moon",    0x24, 0,     25,  0, 0 },
        {"Out01 Dragon Cave",           0x24, 0,     25,  0, 0 },
        {"Out02 Castle Harmondy",       0x24, 0,     25,  0, 0 },
        {"Out02 White Cliff Cave",      0x24, 0,     25,  0, 0 },
        {"Out03 Erathian Sewer",        0x24, 0,     25,  0, 0 },
        {"Out03 Fort Riverstride",      0x24, 0,     25,  0, 0 },
        {"Out03 Castle Gryphonheart",   0x24, 0,     25,  0, 0 },
        {"Out04 Elf Castle",            0x24, 0,     25,  0, 0 },
        {"Out04 Tularean Caves",        0x24, 0,     25,  0, 0 },
        {"Out04 Clanker's Laboratory",  0x24, 0,     25,  0, 0 },
        {"Out05 Hall of the Pit",       0x24, 0,     25,  0, 0 },
        {"Out05 Watchtower 6",          0x24, 0,     25,  0, 0 },
        {"Out06 School of Sorcery",     0x24, 0,     25,  0, 0 },
        {"Out06 Red Dwarf Mines",       0x24, 0,     25,  0, 0 },
        {"Out07 Castle Lambert",        0x24, 0,     25,  0, 0 },
        {"Out07 Walls of Mist",         0x24, 0,     25,  0, 0 },
        {"Out07 Temple of the Light",   0x24, 0,     25,  0, 0 },
        {"Out08 Evil Entrance",         0x24, 0,     25,  0, 0 },
        {"Out08 Breeding Zone",         0x24, 0,     25,  0, 0 },
        {"Out08 Temple of the Dark",    0x24, 0,     25,  0, 0 },
        {"Out09 Grand Temple of the Moon", 0x24, 0,     25,  0, 0 },
        {"Out09 Grand Tempple of the Sun", 0x24, 0,     25,  0, 0 },
        {"Out10 Thunderfist Mountain",  0x24, 0,     25,  0, 0 },
        {"Out10 The Maze",              0x24, 0,     25,  0, 0 },
        {"Out10 Connecting Tunnel Cave #1", 0x24, 0,     25,  0, 0 },
        {"",                            0x24, 0,     25,  0, 0 },
        {"",                            0x24, 0,     25,  0, 0 },
        {"Out11 Stone City",            0x24, 0,     25,  0, 0 },
        {"",                            0x24, 0,     25,  0, 0 },
        {"Out12 Colony Zod",            0x24, 0,     25,  0, 0 },
        {"Out12 Connecting Tunnel Cave #1", 0x24, 0,     25,  0, 0 },
        {"",                            0x24, 0,     25,  0, 0 },
        {"",                            0x24, 0,     25,  0, 0 },
        {"Out13 Mercenary Guild",       0x24, 0,     25,  0, 0 },
        {"Out13 Tidewater Caverns",     0x24, 0,     25,  0, 0 },
        {"Out13 Wine Cellar",           0x24, 0,     25,  0, 0 },
        {"Out14 Titan's Stronghold",    0x24, 0,     25,  0, 0 },
        {"Out14 Temple Of Baa",         0x24, 0,     25,  0, 0 },
        {"Out14 Hall under the Hill",   0x24, 0,     25,  0, 0 },
        {"Out15 The Linclon",           0x24, 0,     25,  0, 0 },
        {"Jail",                        0x24, 0, 31, 0, 0 },
        {"Harmondale Throne Room",      0x24, 0, 29, 0, 0 },
        {"Gryphonheart Throne Room",    0x24, 0, 20, 0, 0 },
        {"Elf Castle Throne Room",      0x24, 0, 20, 0, 0 },
        {"Wizard Castle Throne Room",   0x24, 0, 20, 0, 0 },
        {"Necromancer Casstle Trone Room", 0x24, 0, 20, 0, 0 },
        {"Master Thief",                0x24, 0, 29, 0, 0 },
        {"Dwarven King",                0x24, 0, 20, 0, 0 },
        {"Arms Master",                 0x24, 0, 29, 0, 0 },
        {"Warlock",                     0x24, 0, 29, 0, 0 },
        {"Lord Markam",                 0x24, 0, 20, 0, 0 },
        {"Arbiter Neutral Town",        0x24, 0, 29, 0, 0 },
        {"Arbiter Good Town",           0x24, 0, 29, 0, 0 },
        {"Arbiter Evil Town",           0x24, 0, 29, 0, 0 },
        {"Necromancer Throne Room Empty", 0x24, 0, 20, 0, 0 },
        {"",                            0x24, 0, 29, 0, 0 },
        {"",                            0x24, 0, 29, 0, 0 },
        {"",                            0x24, 0, 29, 0, 0 },
        {"",                            0x24, 0, 29, 0, 0 },
        {"Boat01",                      0xF, 0, 29, 53, 3 },
        {"",                            0x24, 0, 28, 0, 0 },
        {"",                            0x24, 0, 28, 0, 0 },
        {"",                            0x24, 0, 28, 0, 0 },
        {"",                            0x24, 0, 29, 0, 0 },
        {"Arbiter Room Neutral",        0x24, 0, 29, 0, 0 },
        {"Out02 Castle Harmondy Abandoned", 0x24, 0, 25, 0, 0 },
        {"Human Temple02",              0x24, 0x3AB, 23, 27, 0 },
        {"Player Castle Good",          0x24, 0, 25, 0, 0 },
        {"Player Castle Bad",           0x24, 0, 25, 0, 0}
}};



//----- (004B3A72) --------------------------------------------------------
void InitializaDialogueOptions_Tavern(BuildingType type)
{
  int num_buttons; // esi@1

  num_buttons = 0;
  if (type == BuildingType_Tavern)
  {
    num_buttons = 2;
    CreateButtonInColumn(0, 102);
    CreateButtonInColumn(1, 103);
    if ( pParty->HasItem(651) ) //Arcomage Deck
    {
      num_buttons = 3;
      CreateButtonInColumn(2, 104);
    }
  }
  pDialogueWindow->_41D08F_set_keyboard_control_group(num_buttons, 1, 0, 2);
  dword_F8B1E0 = pDialogueWindow->pNumPresenceButton;
}
// F8B1E0: using guessed type int dword_F8B1E0;

//----- (004B3AD4) --------------------------------------------------------
void InitializaDialogueOptions_Shops(BuildingType type)
{
  switch (type)
  {
    case BuildingType_WeaponShop:
    case BuildingType_ArmorShop:
    case BuildingType_MagicShop:
    {
      CreateButtonInColumn(0, 3);
      CreateButtonInColumn(1, 4);
      CreateButtonInColumn(2, 5);
      pDialogueWindow->_41D08F_set_keyboard_control_group(3, 1, 0, 2);
    }
    break;
    
    case BuildingType_AlchemistShop:
    {
      CreateButtonInColumn(0, 3);
      CreateButtonInColumn(1, 4);
      pDialogueWindow->_41D08F_set_keyboard_control_group(2, 1, 0, 2);
    }
    break;
  }

  dword_F8B1E0 = pDialogueWindow->pNumPresenceButton;
}
// F8B1E0: using guessed type int dword_F8B1E0;

//----- (004B3B42) --------------------------------------------------------
void InitializaDialogueOptions(BuildingType type)
{
  switch (type)
  {
    case BuildingType_WeaponShop:
    case BuildingType_ArmorShop:
    case BuildingType_MagicShop:
    case BuildingType_AlchemistShop:
    {
      CreateButtonInColumn(0, 2);
      CreateButtonInColumn(1, 95);
      CreateButtonInColumn(2, 94);
      CreateButtonInColumn(3, 96);
      pDialogueWindow->_41D08F_set_keyboard_control_group(4, 1, 0, 2);
    }
    break;

    case BuildingType_FireGuild:
    {
      CreateButtonInColumn(0, 18);
      CreateButtonInColumn(1, 48);
      CreateButtonInColumn(2, 72);
      pDialogueWindow->_41D08F_set_keyboard_control_group(3, 1, 0, 2);
    }
    break;

    case BuildingType_AirGuild:
    {
      CreateButtonInColumn(0, 18);
      CreateButtonInColumn(1, 49);
      CreateButtonInColumn(2, 72);
      pDialogueWindow->_41D08F_set_keyboard_control_group(3, 1, 0, 2);
    }
    break;

    case BuildingType_WaterGuild:
    {
      CreateButtonInColumn(0, 18);
      CreateButtonInColumn(1, 50);
      CreateButtonInColumn(2, 72);
      pDialogueWindow->_41D08F_set_keyboard_control_group(3, 1, 0, 2);
    }
    break;

    case BuildingType_EarthGuild:
    {
      CreateButtonInColumn(0, 18);
      CreateButtonInColumn(1, 51);
      CreateButtonInColumn(2, 72);
      pDialogueWindow->_41D08F_set_keyboard_control_group(3, 1, 0, 2);
    }
    break;

    case BuildingType_SpiritGuild:
    {
      CreateButtonInColumn(0, 18);
      CreateButtonInColumn(1, 52);
      CreateButtonInColumn(2, 61);
      pDialogueWindow->_41D08F_set_keyboard_control_group(3, 1, 0, 2);
    }
    break;

    case BuildingType_MindGuild:
    {
      CreateButtonInColumn(0, 18);
      CreateButtonInColumn(1, 53);
      CreateButtonInColumn(2, 61);
      pDialogueWindow->_41D08F_set_keyboard_control_group(3, 1, 0, 2);
    }
    break;

    case BuildingType_BodyGuild:
    {
      CreateButtonInColumn(0, 18);
      CreateButtonInColumn(1, 54);
      CreateButtonInColumn(2, 61);
      pDialogueWindow->_41D08F_set_keyboard_control_group(3, 1, 0, 2);
    }
    break;

    case BuildingType_LightGuild:
    {
      CreateButtonInColumn(0, 18);
      CreateButtonInColumn(1, 55);
      pDialogueWindow->_41D08F_set_keyboard_control_group(2, 1, 0, 2);
    }
    break;

    case BuildingType_DarkGuild:
    {
      CreateButtonInColumn(0, 18);
      CreateButtonInColumn(1, 56);
      pDialogueWindow->_41D08F_set_keyboard_control_group(2, 1, 0, 2);
    }
    break;

    case BuildingType_ElementalGuild:
    {
      CreateButtonInColumn(0, 18);
      CreateButtonInColumn(1, 48);
      CreateButtonInColumn(2, 49);
      CreateButtonInColumn(3, 50);
      CreateButtonInColumn(4, 51);
      pDialogueWindow->_41D08F_set_keyboard_control_group(5, 1, 0, 2);
    }
    break;

    case BuildingType_SelfGuild:
    {
      CreateButtonInColumn(0, 18);
      CreateButtonInColumn(1, 52);
      CreateButtonInColumn(2, 53);
      CreateButtonInColumn(3, 54);
      pDialogueWindow->_41D08F_set_keyboard_control_group(4, 1, 0, 2);
    }
    break;

    case BuildingType_16:
    case BuildingType_TownHall:
    {
      int num_buttons = 1;
      CreateButtonInColumn(0, 99);

      if (pParty->uFine)
      {
        num_buttons++;
        CreateButtonInColumn(1, 100);
      }

      pDialogueWindow->_41D08F_set_keyboard_control_group(num_buttons, 1, 0, 2);
    }
    break;

    case BuildingType_Bank:
    {
      CreateButtonInColumn(0, 7);
      CreateButtonInColumn(1, 8);
      pDialogueWindow->_41D08F_set_keyboard_control_group(2, 1, 0, 2);
    }
    break;

    case BuildingType_Temple:
    {
      CreateButtonInColumn(0, 10);
      CreateButtonInColumn(1, 11);
      CreateButtonInColumn(2, 96);
      pDialogueWindow->_41D08F_set_keyboard_control_group(3, 1, 0, 2);
    }
    break;

    case BuildingType_Stables:
    case BuildingType_Boats:
    {
      CreateButtonInColumn(0, 105);
      CreateButtonInColumn(1, 106);
      CreateButtonInColumn(2, 107);
      CreateButtonInColumn(3, 108);
      pDialogueWindow->_41D08F_set_keyboard_control_group(4, 1, 0, 2);
    }
    break;

    case BuildingType_Training:
    {
      CreateButtonInColumn(0, 17);
      CreateButtonInColumn(1, 96);
      pDialogueWindow->_41D08F_set_keyboard_control_group(2, 1, 0, 2);
    }
    break;

    case BuildingType_Tavern:
    {
      CreateButtonInColumn(0, 0xFu);
      CreateButtonInColumn(1, 0x10u);
      CreateButtonInColumn(2, 0x60u);
      if ( (signed int)window_SpeakInHouse->ptr_1C < 108 || (signed int)window_SpeakInHouse->ptr_1C > 120 )
      {
        pDialogueWindow->_41D08F_set_keyboard_control_group(3, 1, 0, 2);
        break;
      }
      CreateButtonInColumn(3, 101);
      pDialogueWindow->_41D08F_set_keyboard_control_group(4, 1, 0, 2);
    }
    break;

    case BuildingType_18:
    case BuildingType_19:
    case BuildingType_Throne_Room:
    case BuildingType_24:
    case BuildingType_Unic:
    case BuildingType_1A:
    case BuildingType_House:
    case BuildingType_Jail:
      break;

    default:
      Error("Invalid enumeration value: %u", type);
  }

/*  if ( a1 > 13 )
  {
    if ( a1 > 22 )
    {
      if ( a1 == 23 )
      {
        CreateButtonInColumn(0, 0xAu);
        CreateButtonInColumn(1, 0xBu);
        v14 = 96;
LABEL_41:
        CreateButtonInColumn(2, v14);
        v17 = 2;
        v11 = 0;
        v10 = 1;
        v9 = 3;
        goto LABEL_42;
      }
      if ( a1 <= 26 )
        goto LABEL_43;
      if ( a1 > 28 )
      {
        if ( a1 != 30 )
          goto LABEL_43;
        CreateButtonInColumn(0, 0x11u);
        v16 = 96;
        goto LABEL_37;
      }
      CreateButtonInColumn(0, 0x69u);
      CreateButtonInColumn(1, 0x6Au);
      CreateButtonInColumn(2, 0x6Bu);
      v12 = 108;
    }
    else
    {
      if ( a1 == 22 )
      {
        CreateButtonInColumn(0, 7u);
        v16 = 8;
        goto LABEL_37;
      }
      v1 = a1 - 14;
      if ( !v1 ) // == 14
      {
        CreateButtonInColumn(0, 0x12u);
        CreateButtonInColumn(1, 0x30u);
        CreateButtonInColumn(2, 0x31u);
        CreateButtonInColumn(3, 0x32u);
        CreateButtonInColumn(4, 0x33u);
        v17 = 2;
        v11 = 0;
        v10 = 1;
        v9 = 5;
        goto LABEL_42;
      }
      v2 = v1 - 1;
      if ( v2 ) // > 15
      {
        v3 = v2 - 2;
        if ( v3 ) // > 17
        {
          if ( v3 != 4 ) // 18, 19, 20
            goto LABEL_43;
          CreateButtonInColumn(0, 0xFu); // 21
          CreateButtonInColumn(1, 0x10u);
          v4 = 3;
          CreateButtonInColumn(2, 0x60u);
          v5 = (signed int)window_SpeakInHouse->ptr_1C;
          if ( v5 < 108 || v5 > 120 )
            goto LABEL_28;
          v4 = 4;
          v6 = 101;
          v7 = 3;
        }
        else // 16, 17
        {
          v4 = 1;
          CreateButtonInColumn(0, 0x63u);
          if ( !pParty->uFine )
          {
LABEL_28:
            v17 = 2;
            v11 = 0;
            v10 = 1;
            v9 = v4;
LABEL_42:
            pDialogueWindow->_41D08F_set_keyboard_control_group(v9, v10, v11, v17);
            goto LABEL_43;
          }
          v4 = 2;
          v7 = 1;
          v6 = 100;
        }
        CreateButtonInColumn(v7, v6);
        goto LABEL_28;
      }
      CreateButtonInColumn(0, 0x12u); // 15
      CreateButtonInColumn(1, 0x34u);
      CreateButtonInColumn(2, 0x35u);
      v12 = 54;
    }
LABEL_39:
    CreateButtonInColumn(3, v12);
    v17 = 2;
    v11 = 0;
    v10 = 1;
    v9 = 4;
    goto LABEL_42;
  }
  if ( a1 == 13 )
  {
    CreateButtonInColumn(0, 0x12u);
    v16 = 56;
LABEL_37:
    CreateButtonInColumn(1, v16);
    v17 = 2;
    v11 = 0;
    v10 = 1;
    v9 = 2;
    goto LABEL_42;
  }
  switch ( a1 )
  {
    case 1:
    case 2:
    case 3:
    case 4:
      CreateButtonInColumn(0, 2u);
      CreateButtonInColumn(1, 0x5Fu);
      CreateButtonInColumn(2, 0x5Eu);
      v12 = 96;
      goto LABEL_39;
    case 5:
      CreateButtonInColumn(0, 0x12u);
      v13 = 48;
      goto LABEL_9;
    case 6:
      CreateButtonInColumn(0, 0x12u);
      v13 = 49;
      goto LABEL_9;
    case 7:
      CreateButtonInColumn(0, 0x12u);
      v13 = 50;
      goto LABEL_9;
    case 8:
      CreateButtonInColumn(0, 0x12u);
      v13 = 51;
LABEL_9:
      CreateButtonInColumn(1, v13);
      v14 = 72;
      goto LABEL_41;
    case 9:
      CreateButtonInColumn(0, 0x12u);
      v15 = 52;
      goto LABEL_13;
    case 10:
      CreateButtonInColumn(0, 0x12u);
      v15 = 53;
      goto LABEL_13;
    case 11:
      CreateButtonInColumn(0, 0x12u);
      v15 = 54;
LABEL_13:
      CreateButtonInColumn(1, v15);
      v14 = 61;
      goto LABEL_41;
    case 12:
      CreateButtonInColumn(0, 0x12u);
      v16 = 55;
      goto LABEL_37;
    default:
      break;
  }
LABEL_43:*/
  dword_F8B1E0 = pDialogueWindow->pNumPresenceButton;
}
// F8B1E0: using guessed type int dword_F8B1E0;


//----- (004B1784) --------------------------------------------------------
bool  HouseUI_CheckIfPlayerCanInteract()
{
  GUIWindow window; // [sp+4h] [bp-54h]@3

  if ( pPlayers[uActiveCharacter]->CanAct() )
  {
    pDialogueWindow->pNumPresenceButton = dword_F8B1E0;
    return true;
  }
  else
  {
    pDialogueWindow->pNumPresenceButton = 0;
    memcpy(&window, pPrimaryWindow, sizeof(window));
    window.uFrameX = 483;
    window.uFrameWidth = 148;
    window.uFrameZ = 334;
    sprintfex(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[427], pPlayers[uActiveCharacter]->pName,
              pGlobalTXT_LocalizationStrings[562]); // "%s is in no condition to %s""do anything"
    window.DrawTitleText(pFontArrus, 0, (212 - pFontArrus->CalcTextHeight(pTmpBuf.data(), &window, 0, 0)) / 2 + 101, ui_house_player_cant_interact_color, pTmpBuf.data(), 3);
    return false;
  }
}

//----- (0044622E) --------------------------------------------------------
bool EnterHouse(enum HOUSE_ID uHouseID)
{
	signed int uOpenTime; // eax@5
	signed int uCloseTime; // esi@5
//	unsigned int v5; // esi@5
//	int v6; // edx@5
	signed int am_pm_flag_open; // ecx@10
	signed int am_pm_flag_close; // eax@10
//	int v9; // esi@10
//	int v11; // ecx@17
//	unsigned int v12; // kr00_4@25
//	int v14; // eax@25
	unsigned int v17; // eax@37
	signed int v18; // edi@37
	signed int v19; // edi@41
	char pContainer[40]; // [sp+Ch] [bp-30h]@32
//	unsigned int v24; // [sp+34h] [bp-8h]@5

	GameUI_Footer_TimedString[0] = 0;
	pFooterString[0] = 0;
	ShowStatusBarString("", 2);
	if ( pMessageQueue_50CBD0->uNumMessages )
		pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
	viewparams->bRedrawGameUI = 1;
	uDialogueType = 0;
	pKeyActionMap->SetWindowInputStatus(WINDOW_INPUT_CANCELLED);
	pKeyActionMap->ResetKeys();
	if (uHouseID == HOUSE_600 || uHouseID == HOUSE_601)
		{
		GameOverMenu(0);
		return 0;
		}
	uOpenTime = p2DEvents[uHouseID - 1].uOpenTime;
	uCloseTime = p2DEvents[uHouseID - 1].uCloseTime;
	current_npc_text = 0;
	dword_F8B1E4 = 0;
	memset(byte_F8B1F0.data(), 0, 4);
	memset(player_levels.data(), 0, 16);
	pRenderer->ClearZBuffer(0, 479);

	if (((uCloseTime - 1 <= uOpenTime)&&((pParty->uCurrentHour <uOpenTime)&&(pParty->uCurrentHour >(uCloseTime - 1))))||
		((uCloseTime - 1 >  uOpenTime)&&((pParty->uCurrentHour < uOpenTime)||(pParty->uCurrentHour > (uCloseTime - 1))))) 

		{
		am_pm_flag_open = 0;
		am_pm_flag_close = 0;
		if (uOpenTime > 12 )
			{
			uOpenTime -= 12;
			am_pm_flag_open = 1;
			}
		if ( uCloseTime > 12 )
			{
			uCloseTime -= 12;
			am_pm_flag_close = 1;
			}
		sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[414], uOpenTime, aAMPMNames[am_pm_flag_open], 
			uCloseTime, aAMPMNames[am_pm_flag_close]); //"This place is open from %d%s to %d%s"
		ShowStatusBarString(pTmpBuf.data(), 2u);
		if ( uActiveCharacter )
			pPlayers[uActiveCharacter]->PlaySound(SPEECH_3, 0);
		return 0;
		}
	else
		{
		//v10 = uHouseID;
		if ( (signed int)uHouseID < 53 ) //entering shops and guilds
			{
			if ( !(pParty->PartyTimes._shop_ban_times[uHouseID])
				||  (pParty->PartyTimes._shop_ban_times[uHouseID] <= pParty->uTimePlayed) )
				{
				pParty->PartyTimes._shop_ban_times[uHouseID] = 0;
				}
			else
				{
				ShowStatusBarString(pGlobalTXT_LocalizationStrings[191], 2u);// "You've been banned from this shop!"
				return 0;
				}
			}
        pAudioPlayer->StopChannels(-1, -1);

		uCurrentHouse_Animation = p2DEvents[uHouseID - 1].uAnimationID;
		in_current_building_type = (BuildingType)pAnimatedRooms[uCurrentHouse_Animation].uBuildingType;
		if ( in_current_building_type == BuildingType_Throne_Room && pParty->uFine )   // going 2 jail
			{
			uCurrentHouse_Animation = (signed __int16)p2DEvents[186].uAnimationID;
			uHouseID = HOUSE_JAIL;
			pParty->uTimePlayed = pParty->uTimePlayed + 0x7620000;
			in_current_building_type = (BuildingType)pAnimatedRooms[p2DEvents[HOUSE_LORD_AND_JUDGE_EMERALD_ISLE].uAnimationID].uBuildingType;
			++pParty->uNumPrisonTerms;
			pParty->uFine = 0;
			for (uint i = 0; i < 4; ++i)
				{
				pParty->pPlayers[i].uTimeToRecovery = 0;
				pParty->pPlayers[i].uNumDivineInterventionCastsThisDay = 0;
				pParty->pPlayers[i].SetVariable(VAR_Award, 87);
				}
			}
		++pIcons_LOD->uTexturePacksCount;
		if ( !pIcons_LOD->uNumPrevLoadedFiles )
			pIcons_LOD->uNumPrevLoadedFiles = pIcons_LOD->uNumLoadedFiles;
		switch (pParty->alignment)
        {
		case PartyAlignment_Good:    sprintf(pContainer, "evt%02d-b", const_2()); break;
		case PartyAlignment_Neutral: sprintf(pContainer, "evt%02d", const_2()); break;
		case PartyAlignment_Evil:    sprintf(pContainer, "evt%02d-c", const_2()); break;
        default: Error("Invalid alignment type: %u", pParty->alignment);
        }

		v17 = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
		pDialogueNPCCount = 0;
		pTexture_Dialogue_Background = &pIcons_LOD->pTextures[v17];
		uTextureID_right_panel_loop = uTextureID_right_panel;
		PrepareHouse(uHouseID);
		v18 = 1;
		uTextureID_right_panel_loop = uTextureID_right_panel;
		if ( uNumDialogueNPCPortraits == 1 )
			pDialogueNPCCount = 1;
		pVideoPlayer->OpenHouseMovie(pAnimatedRooms[uCurrentHouse_Animation].video_name, 1u);
		dword_5C35D4 = 1;
		if ( (signed int)uHouseID < 139 || (signed int)uHouseID > 172 )
        {
			if ( (signed int)uHouseID >= HOUSE_STABLES_HARMONDALE && (signed int)uHouseID <= 73 && !IsTravelAvailable(uHouseID - HOUSE_STABLES_HARMONDALE) )
            {
              return 1;
            }
        }
		else
		{ //guilds
			v19 = guild_mambership_flags[uHouseID - HOUSE_FIRE_GUILD_INITIATE_EMERALD_ISLE]; //guilds flags 
			//v20 = uHouseID;
			//if ( !((unsigned __int8)(0x80u >> v19 % 8) & pPlayers[uActiveCharacter]->_guilds_member_bits[v19 /8]) )
			if(!_449B57_test_bit(pPlayers[uActiveCharacter]->_achieved_awards_bits,v19))
            {
				PlayHouseSound(uHouseID, HouseSound_Greeting_2);
				return 1;
            }
		}
		PlayHouseSound(uHouseID, HouseSound_Greeting);
		dword_5C35D4 = 1;
		return 1;
		}
	}

//----- (0044606A) --------------------------------------------------------
void PrepareHouse(HOUSE_ID house)
{
  __int16 uExitMapID; // ax@2
//  int v7; // ebx@11
//  int v13; // [sp+30h] [bp-30h]@11
  int npc_id_arr[6]; // [sp+34h] [bp-2Ch]@1
  int uAnimationID; // [sp+50h] [bp-10h]@1


  uAnimationID = p2DEvents[house - 1].uAnimationID;
  memset(npc_id_arr, 0, sizeof(npc_id_arr));
  uNumDialogueNPCPortraits = 0;
  uHouse_ExitPic = p2DEvents[house - 1].uExitPicID;
  if ( uHouse_ExitPic )
  {
    uExitMapID = p2DEvents[house - 1]._quest_related;
    if ( uExitMapID > 0 )
    {
	  if(_449B57_test_bit(pParty->_quest_bits,uExitMapID))
      {
        uHouse_ExitPic = 0;
      }
    }
  }

  dword_591080 = pAnimatedRooms[uAnimationID].house_npc_id;
  HouseNPCData[0]=0;
  uNumDialogueNPCPortraits = 0;
  if ( dword_591080 )
  {

    npc_id_arr[0] = dword_591080;
    uNumDialogueNPCPortraits = 1;
  }

  for (uint i = 1; i < pNPCStats->uNumNewNPCs; ++i)
  {
    if (pNPCStats->pNewNPCData[i].Location2D == house )
    {
      if (!(pNPCStats->pNewNPCData[i].uFlags & 0x80))
      {
        HouseNPCData[uNumDialogueNPCPortraits+1-((dword_591080 != 0) ? 1 : 0) ] = &pNPCStats->pNewNPCData[i];     
        npc_id_arr[uNumDialogueNPCPortraits] = pNPCStats->pNewNPCData[i].uPortraitID;
         ++uNumDialogueNPCPortraits;
        if ((pNPCStats->pNewNPCData[i].uFlags & 3) != 2)
          ++pNPCStats->pNewNPCData[i].uFlags;
      }
    }

  }

  for (int i = 0; i < uNumDialogueNPCPortraits; ++i)
  {
   
    char icon_name[128];
    sprintfex(icon_name, "npc%03u", npc_id_arr[i]);
    pDialogueNPCPortraits[i] = pIcons_LOD->LoadTexturePtr(icon_name, TEXTURE_16BIT_PALETTE);
  }

  if (uHouse_ExitPic)
  {
    pDialogueNPCPortraits[uNumDialogueNPCPortraits] = pIcons_LOD->LoadTexturePtr(pHouse_ExitPictures[uHouse_ExitPic], TEXTURE_16BIT_PALETTE);
    ++uNumDialogueNPCPortraits;
    uHouse_ExitPic = p2DEvents[house - 1].uExitMapID;
  }
}
//----- (004B1E92) --------------------------------------------------------
void PlayHouseSound(unsigned int uHouseID, HouseSoundID sound)
{
  if ( pAnimatedRooms[p2DEvents[uHouseID].uAnimationID].uRoomSoundId )
    pAudioPlayer->PlaySound((SoundID)(sound + 100 * (pAnimatedRooms[p2DEvents[uHouseID].uAnimationID].uRoomSoundId + 300)),
        806, 0, -1, 0, 0, 0, 0);
}

//----- (004BCACC) --------------------------------------------------------
void __fastcall OnSelectShopDialogueOption(signed int uMessageParam)
{
  int experience_for_next_level; // eax@5
  int v16; // eax@32
  __int16 v24; // ax@163
  signed int v36; // esi@227
  int pPrice; // ecx@227

  if ( !pDialogueWindow->pNumPresenceButton )
    return;
  pRenderer->ClearZBuffer(0, 479);
  if (dialog_menu_id == HOUSE_DIALOGUE_MAIN)
  {
    if ( in_current_building_type == BuildingType_Training )
    {
      if ( uMessageParam == HOUSE_DIALOGUE_TRAININGHALL_TRAIN )
      {
        experience_for_next_level = 0;
        if ( pPlayers[uActiveCharacter]->uLevel > 0 )
        {
          for( uint i = 0; i < pPlayers[uActiveCharacter]->uLevel; i++ )
            experience_for_next_level += i + 1;
        }
        if (pPlayers[uActiveCharacter]->uLevel < pMaxLevelPerTrainingHallType[(unsigned int)window_SpeakInHouse->ptr_1C - 89] &&
          (signed __int64)pPlayers[uActiveCharacter]->uExperience < 1000 * experience_for_next_level)//test experience
        return;
      }
      pDialogueWindow->Release();
      pDialogueWindow = GUIWindow::Create(0, 0, window->GetWidth(), 345, WINDOW_MainMenu, 0, 0);
      pBtn_ExitCancel = pDialogueWindow->CreateButton(526, 445, 75, 33, 1, 0, UIMSG_Escape, 0, 0, pGlobalTXT_LocalizationStrings[74],// "End Conversation"
                                                       pIcons_LOD->GetTexture(uTextureID_BUTTDESC2), 0);
      pDialogueWindow->CreateButton(8, 8, 450, 320, 1, 0, UIMSG_BuyInShop_Identify_Repair, 0, 0, "", nullptr);
    }
    if ( in_current_building_type != BuildingType_Training )
    {
      if ((in_current_building_type == BuildingType_Stables || in_current_building_type == BuildingType_Boats) &&
           transport_schedule[transport_routes[(unsigned int)window_SpeakInHouse->ptr_1C - HOUSE_STABLES_HARMONDALE][uMessageParam - HOUSE_DIALOGUE_TRANSPORT_SCHEDULE_1]].pSchedule[pParty->uDaysPlayed % 7]
          || in_current_building_type != BuildingType_Temple || uMessageParam != BuildingType_MindGuild )
      {
        pDialogueWindow->Release();
        pDialogueWindow = GUIWindow::Create(0, 0, window->GetWidth(), 345, WINDOW_MainMenu, 0, 0);
        pBtn_ExitCancel = pDialogueWindow->CreateButton(526, 445, 75, 33, 1, 0, UIMSG_Escape, 0, 0, pGlobalTXT_LocalizationStrings[74],// "End Conversation"
                                                         pIcons_LOD->GetTexture(uTextureID_BUTTDESC2), 0);
        pDialogueWindow->CreateButton(8, 8, 450, 320, 1, 0, UIMSG_BuyInShop_Identify_Repair, 0, 0, "", nullptr);
      }
      else if (uActiveCharacter)
      {
        if ( !pPlayers[uActiveCharacter]->IsPlayerHealableByTemple() )
          return;
      }
    }
    dialog_menu_id = (HOUSE_DIALOGUE_MENU)uMessageParam;
    if ( in_current_building_type < BuildingType_19 )
      ShopTexture = &pIcons_LOD->pTextures[pIcons_LOD->LoadTexture(off_4F03B8[(int)in_current_building_type], TEXTURE_16BIT_PALETTE)];
  }
  
  //NEW
  switch (in_current_building_type)
  {
    case BuildingType_FireGuild:
    case BuildingType_AirGuild:
    case BuildingType_WaterGuild:
    case BuildingType_EarthGuild:
    case BuildingType_SpiritGuild:
    case BuildingType_MindGuild:
    case BuildingType_BodyGuild:
    case BuildingType_LightGuild:
    case BuildingType_DarkGuild:
    case BuildingType_ElementalGuild:
    case BuildingType_SelfGuild:
    case BuildingType_16:
    {
      if ( pParty->PartyTimes.Shops_next_generation_time[window_SpeakInHouse->par1C - 139] >= (signed __int64)pParty->uTimePlayed )
      {
        for ( uint i = 0; i < 12; ++i )
        {
          if ( pParty->SpellBooksInGuilds[window_SpeakInHouse->par1C - 139][i].uItemID )
            ItemsInShopTexture[i] = &pIcons_LOD->pTextures[pIcons_LOD->LoadTexture(pParty->SpellBooksInGuilds[window_SpeakInHouse->par1C - 139][i].GetIconName(), TEXTURE_16BIT_PALETTE)];
        }
      }
      else//generation new books
      {
        SpellBookGenerator();
        pParty->PartyTimes.Shops_next_generation_time[window_SpeakInHouse->par1C - 139] = pParty->uTimePlayed + (signed __int64)((double)(0xA8C000
                                  * (signed int)p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].field_1C) * 0.033333335);
      }
      break;
    }
    case BuildingType_TownHall:
    {
      if ( uMessageParam == HOUSE_DIALOGUE_TOWNHALL_MESSAGE )
      {
        if ( pParty->PartyTimes.bountyHunting_next_generation_time[(int)((char *)window_SpeakInHouse->ptr_1C - 102)] < (signed __int64)pParty->uTimePlayed )//new generation
        {
          pParty->monster_for_hunting_killed[(int)((char *)window_SpeakInHouse->ptr_1C - 102)] = false;
          pParty->PartyTimes.bountyHunting_next_generation_time[(int)((char *)window_SpeakInHouse->ptr_1C - 102)] = (signed __int64)((double)(309657600 * (pParty->uCurrentMonth + 12i64 * pParty->uCurrentYear - 14015)) * 0.033333335);
          pParty->monster_id_for_hunting[(int)((char *)window_SpeakInHouse->ptr_1C - 102)] = rand() % 258 + 1;
          v16 = (int)((char *)window_SpeakInHouse->ptr_1C - 102);
          if ( !v16 )
          {
            while ( 1 )
            {
              v24 = pParty->monster_id_for_hunting[(int)((char *)window_SpeakInHouse->ptr_1C - 102)];
              if ( (unsigned __int16)v24 < 115 || (unsigned __int16)v24 > 132 )
              {
                if ( ((unsigned __int16)v24 < 235 || (unsigned __int16)v24 > 252)
                  && ((unsigned __int16)v24 < 133 || (unsigned __int16)v24 > 150)
                  && ((unsigned __int16)v24 < 0x97u || (unsigned __int16)v24 > 0xBAu)
                  && ((unsigned __int16)v24 < 0xBEu || (unsigned __int16)v24 > 0xC0u)
                  && ((unsigned __int16)v24 < 0xC4u || (unsigned __int16)v24 > 0xC6u)
                  && ((unsigned __int16)v24 < 0x2Bu || (unsigned __int16)v24 > 0x2Du)
                  && ((unsigned __int16)v24 < 0xCDu || (unsigned __int16)v24 > 0xCFu)
                  && ((unsigned __int16)v24 < 0x5Eu || (unsigned __int16)v24 > 0x60u)
                  && ((unsigned __int16)v24 < 0xFDu || (unsigned __int16)v24 > 0xFFu)
                  && ((unsigned __int16)v24 < 0x6Du || (unsigned __int16)v24 > 0x6Fu)
                  && ((unsigned __int16)v24 < 0x61u || (unsigned __int16)v24 > 0x63u) )
                  break;
              }
              pParty->monster_id_for_hunting[(int)((char *)window_SpeakInHouse->ptr_1C - 102)] = rand() % 258 + 1;
            }
          }
          if ( v16 == 1 )
          {
            while ( 1 )
            {
              v24 = pParty->monster_id_for_hunting[(int)((char *)window_SpeakInHouse->ptr_1C - 102)];
              if ( (unsigned __int16)v24 < 115 || (unsigned __int16)v24 > 132 )
              {
                if ( ((unsigned __int16)v24 < 0xE8u || (unsigned __int16)v24 > 0xF9u)
                  && ((unsigned __int16)v24 < 0x85u || (unsigned __int16)v24 > 0x96u)
                  && ((unsigned __int16)v24 < 0x97u || (unsigned __int16)v24 > 0xBAu)
                  && ((unsigned __int16)v24 < 0xBEu || (unsigned __int16)v24 > 0xC0u)
                  && ((unsigned __int16)v24 < 0xC4u || (unsigned __int16)v24 > 0xC6u)
                  && ((unsigned __int16)v24 < 0x2Bu || (unsigned __int16)v24 > 0x2Du)
                  && ((unsigned __int16)v24 < 0x52u || (unsigned __int16)v24 > 0x54u)
                  && ((unsigned __int16)v24 < 4 || (unsigned __int16)v24 > 6)
                  && ((unsigned __int16)v24 < 0x37u || (unsigned __int16)v24 > 0x39u)
                  && ((unsigned __int16)v24 < 0x3Au || (unsigned __int16)v24 > 0x3Cu)
                  && ((unsigned __int16)v24 < 0x3Du || (unsigned __int16)v24 > 0x3Fu)
                  && ((unsigned __int16)v24 < 0xFDu || (unsigned __int16)v24 > 0xFFu)
                  && ((unsigned __int16)v24 < 0x61u || (unsigned __int16)v24 > 0x63u)
                  && ((unsigned __int16)v24 < 0xCDu || (unsigned __int16)v24 > 0xCFu) )
                  break;
              }
              pParty->monster_id_for_hunting[(int)((char *)window_SpeakInHouse->ptr_1C - 102)] = rand() % 258 + 1;
            }
          }
          if ( v16 == 2 )
          {
            while ( 1 )
            {
              v24 = pParty->monster_id_for_hunting[(int)((char *)window_SpeakInHouse->ptr_1C - 102)];
              if ( (unsigned __int16)v24 < 0x73u || (unsigned __int16)v24 > 0x84u )
              {
                if ( ((unsigned __int16)v24 < 0xE8u || (unsigned __int16)v24 > 0xF9u)
                  && ((unsigned __int16)v24 < 0x85u || (unsigned __int16)v24 > 0x96u)
                  && ((unsigned __int16)v24 < 0x97u || (unsigned __int16)v24 > 0xBAu)
                  && ((unsigned __int16)v24 < 0xBEu || (unsigned __int16)v24 > 0xC0u)
                  && ((unsigned __int16)v24 < 0xC4u || (unsigned __int16)v24 > 0xC6u)
                  && ((unsigned __int16)v24 < 0x2Bu || (unsigned __int16)v24 > 0x2Du)
                  && ((unsigned __int16)v24 < 0x31u || (unsigned __int16)v24 > 0x33u)
                  && ((unsigned __int16)v24 < 0x34u || (unsigned __int16)v24 > 0x36u)
                  && ((unsigned __int16)v24 < 0xFDu || (unsigned __int16)v24 > 0xFFu)
                  && ((unsigned __int16)v24 < 0x61u || (unsigned __int16)v24 > 0x63u)
                  && ((unsigned __int16)v24 < 0x1Cu || (unsigned __int16)v24 > 0x1Eu) )
                  break;
              }
              pParty->monster_id_for_hunting[(int)((char *)window_SpeakInHouse->ptr_1C - 102)] = rand() % 258 + 1;
            }
          }
          if ( v16 == 3 )
          {
            while ( 1 )
            {
              v24 = pParty->monster_id_for_hunting[(int)((char *)window_SpeakInHouse->ptr_1C - 102)];
              if ( (unsigned __int16)v24 < 0x73u || (unsigned __int16)v24 > 0x84u )
              {
                if ( ((unsigned __int16)v24 < 0xE8u || (unsigned __int16)v24 > 0xF9u)
                  && ((unsigned __int16)v24 < 0x85u || (unsigned __int16)v24 > 0x96u)
                  && ((unsigned __int16)v24 < 0x97u || (unsigned __int16)v24 > 0xBAu)
                  && ((unsigned __int16)v24 < 0xBEu || (unsigned __int16)v24 > 0xC0u)
                  && ((unsigned __int16)v24 < 0xC4u || (unsigned __int16)v24 > 0xC6u)
                  && ((unsigned __int16)v24 < 0x2Bu || (unsigned __int16)v24 > 0x2Du)
                  && ((unsigned __int16)v24 < 0x5Eu || (unsigned __int16)v24 > 0x60u)
                  && ((unsigned __int16)v24 < 0x43u || (unsigned __int16)v24 > 0x45u)
                  && ((unsigned __int16)v24 < 0x4Fu || (unsigned __int16)v24 > 0x51u)
                  && ((unsigned __int16)v24 < 0xC1u || (unsigned __int16)v24 > 0xC3u)
                  && ((unsigned __int16)v24 < 0x13u || (unsigned __int16)v24 > 0x15u)
                  && ((unsigned __int16)v24 < 0xFDu || (unsigned __int16)v24 > 0xFFu)
                  && ((unsigned __int16)v24 < 0x61u || (unsigned __int16)v24 > 0x63u)
                  && ((unsigned __int16)v24 < 0x6Au || (unsigned __int16)v24 > 0x6Cu) )
                  break;
              }
              pParty->monster_id_for_hunting[(int)((char *)window_SpeakInHouse->ptr_1C - 102)] = rand() % 258 + 1;
            }
          }
          if ( v16 == 4 )
          {
            while ( 1 )
            {
              v24 = pParty->monster_id_for_hunting[(int)((char *)window_SpeakInHouse->ptr_1C - 102)];
              if ( (unsigned __int16)v24 < 0x73u || (unsigned __int16)v24 > 0x84u )
              {
                if ( ((unsigned __int16)v24 < 0xE8u || (unsigned __int16)v24 > 0xF9u)
                  && ((unsigned __int16)v24 < 0x85u || (unsigned __int16)v24 > 0x96u)
                  && ((unsigned __int16)v24 < 0x97u || (unsigned __int16)v24 > 0xBAu)
                  && ((unsigned __int16)v24 < 0xBEu || (unsigned __int16)v24 > 0xC0u)
                  && ((unsigned __int16)v24 < 0xC4u || (unsigned __int16)v24 > 0xC6u)
                  && ((unsigned __int16)v24 < 0x2Bu || (unsigned __int16)v24 > 0x2Du)
                  && ((unsigned __int16)v24 < 0x6Du || (unsigned __int16)v24 > 0x6Fu)
                  && ((unsigned __int16)v24 < 0x46u || (unsigned __int16)v24 > 0x48u)
                  && ((unsigned __int16)v24 < 0x100u || (unsigned __int16)v24 > 0x102u)
                  && ((unsigned __int16)v24 < 0xD9u || (unsigned __int16)v24 > 0xDBu)
                  && ((unsigned __int16)v24 < 0xC7u || (unsigned __int16)v24 > 0xC9u)
                  && ((unsigned __int16)v24 < 0xE5u || (unsigned __int16)v24 > 0xE7u)
                  && ((unsigned __int16)v24 < 0xDFu || (unsigned __int16)v24 > 0xE1u)
                  && ((unsigned __int16)v24 < 0x5Bu || (unsigned __int16)v24 > 0x5Du)
                  && ((unsigned __int16)v24 < 0x49u || (unsigned __int16)v24 > 0x4Bu)
                  && ((unsigned __int16)v24 < 0xFDu || (unsigned __int16)v24 > 0xFFu)
                  && ((unsigned __int16)v24 < 0x61u || (unsigned __int16)v24 > 0x63u)
                  && ((unsigned __int16)v24 < 0x10u || (unsigned __int16)v24 > 0x12u) )
                  break;
              }
              pParty->monster_id_for_hunting[(int)((char *)window_SpeakInHouse->ptr_1C - 102)] = rand() % 258 + 1;
            }
          }
        }
        bountyHunting_monster_id_for_hunting = pParty->monster_id_for_hunting[(int)((char *)window_SpeakInHouse->ptr_1C - 102)];
        if ( !pParty->monster_for_hunting_killed[(int)((char *)window_SpeakInHouse->ptr_1C - 102)] )
        {
          bountyHunting_text = pNPCTopics[351].pText;//"� ���� ������ ��������� ������� �� ������ %s..."
          if ( !pParty->monster_id_for_hunting[(int)((char *)window_SpeakInHouse->ptr_1C - 102)] )
            bountyHunting_text = pNPCTopics[353].pText;//"��� ��� ��� �������� � ���� ������ �� ��������"
        }
        else
        {
          if ( pParty->monster_id_for_hunting[(int)((char *)window_SpeakInHouse->ptr_1C - 102)] > 0 )//get prize
          {
            pParty->PartyFindsGold(100 * pMonsterStats->pInfos[(unsigned __int16)pParty->monster_id_for_hunting[(int)((char *)window_SpeakInHouse->ptr_1C - 102)]].uLevel, 0);
            for ( uint i = 0; i < 4; ++i )
              pParty->pPlayers[i].SetVariable(VAR_Award, 86);
            pParty->uNumBountiesCollected += 100 * pMonsterStats->pInfos[pParty->monster_id_for_hunting[(int)((char *)window_SpeakInHouse->ptr_1C - 102)]].uLevel;
            pParty->monster_id_for_hunting[(int)((char *)window_SpeakInHouse->ptr_1C - 102)] = 0;
            pParty->monster_for_hunting_killed[(int)((char *)window_SpeakInHouse->ptr_1C - 102)] = false;
          }
          bountyHunting_text = pNPCTopics[352].pText;//"����������! �� �������..."
        }
      }
      else if ( uMessageParam == HOUSE_DIALOGUE_TOWNHALL_PAY_FINE )
        pKeyActionMap->EnterText(1, 10, window_SpeakInHouse);
      break;
    }
    case BuildingType_Bank:
    {
      if ( dialog_menu_id >= 7 && dialog_menu_id <= 8 )
        pKeyActionMap->EnterText(1, 10, window_SpeakInHouse);
      return;
      break;
    }
    case BuildingType_WeaponShop:
    case BuildingType_ArmorShop:
    case BuildingType_MagicShop:
    case BuildingType_AlchemistShop:
    case BuildingType_Tavern:
    case BuildingType_Temple:
    case BuildingType_Training:
    {
      break;
    }
    default:
    {
      return;
      break;
    }
  }

  switch ( uMessageParam )
  {
    case HOUSE_DIALOGUE_LEARN_SKILLS:
    {
      pDialogueWindow->eWindowType = WINDOW_MainMenu;
      UI_CreateEndConversationButton();
      FillAviableSkillsToTeach(in_current_building_type);
      break;
    }
    case HOUSE_DIALOGUE_TAVERN_ARCOMAGE_MAIN:
    {
      pDialogueWindow->eWindowType = WINDOW_MainMenu;
      UI_CreateEndConversationButton();
      InitializaDialogueOptions_Tavern(in_current_building_type);
      break;
    }
    case HOUSE_DIALOGUE_TAVERN_ARCOMAGE_RULES:
    case HOUSE_DIALOGUE_TAVERN_ARCOMAGE_VICTORY_CONDITIONS:
    {
      dialog_menu_id = (HOUSE_DIALOGUE_MENU)uMessageParam;
      break;
    }
    case HOUSE_DIALOGUE_TAVERN_ARCOMAGE_RESULT:
    {
      pMessageQueue_50CBD0->AddMessage(UIMSG_PlayArcomage, 0, 0);
      dialog_menu_id = HOUSE_DIALOGUE_TAVERN_ARCOMAGE_RESULT;
      break;
    }
    case HOUSE_DIALOGUE_SHOP_BUY_STANDARD:
    case HOUSE_DIALOGUE_SHOP_BUY_SPECIAL:
    {
      if ( pParty->PartyTimes.Shops_next_generation_time[(unsigned int)window_SpeakInHouse->ptr_1C] < (signed __int64)pParty->uTimePlayed )
      {
        GenerateStandartShopItems();
        GenerateSpecialShopItems();
        pParty->PartyTimes.Shops_next_generation_time[window_SpeakInHouse->par1C] = pParty->uTimePlayed + (signed __int64)((double)(11059200 * (signed int)p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].field_1C) * 0.033333335);
      }
      if ( uMessageParam == HOUSE_DIALOGUE_SHOP_BUY_STANDARD )
      {
        if ( uItemsAmountPerShopType[p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].uType] )
        {
          for ( uint i = 0; i < (unsigned __int8)uItemsAmountPerShopType[p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].uType]; ++i )
          {
            if ( pParty->StandartItemsInShops[(int)window_SpeakInHouse->ptr_1C][i].uItemID )
              ItemsInShopTexture[i] = &pIcons_LOD->pTextures[pIcons_LOD->LoadTexture(pParty->StandartItemsInShops[(int)window_SpeakInHouse->ptr_1C][i].GetIconName(), TEXTURE_16BIT_PALETTE)];
          }
        }
        if ( in_current_building_type == BuildingType_WeaponShop )
        {
          if ( uItemsAmountPerShopType[p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].uType] )
          {
            for ( uint i = 0; i < (unsigned __int8)uItemsAmountPerShopType[p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].uType]; ++i )
            {
              if ( pParty->StandartItemsInShops[(int)window_SpeakInHouse->ptr_1C][i].uItemID)
                weapons_Ypos[i] = rand() % (300 - ItemsInShopTexture[i]->uTextureHeight);
            }
          }
        }
      }
      if ( uMessageParam == HOUSE_DIALOGUE_SHOP_BUY_SPECIAL )
      {
        if ( uItemsAmountPerShopType[p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].uType] )
        {
          for ( uint i = 0; i < (unsigned __int8)uItemsAmountPerShopType[p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].uType]; ++i )
          {
            if ( pParty->SpecialItemsInShops[(unsigned int)window_SpeakInHouse->ptr_1C][i].uItemID )
              ItemsInShopTexture[i] = &pIcons_LOD->pTextures[pIcons_LOD->LoadTexture(pParty->SpecialItemsInShops[(unsigned int)window_SpeakInHouse->ptr_1C][i].GetIconName(), TEXTURE_16BIT_PALETTE)];
          }
        }
        if ( in_current_building_type == BuildingType_WeaponShop )
        {
          if ( uItemsAmountPerShopType[p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].uType] )
          {
            for ( uint i = 0; i < (unsigned __int8)uItemsAmountPerShopType[p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].uType]; ++i )
            {
              if (pParty->SpecialItemsInShops[(unsigned int)window_SpeakInHouse->ptr_1C][i].uItemID)
                weapons_Ypos[i] = rand() % (300 - ItemsInShopTexture[i]->uTextureHeight);
            }
          }
        }
      }
      break;
    }
    case HOUSE_DIALOGUE_SHOP_SELL:
    case HOUSE_DIALOGUE_SHOP_IDENTIFY:
    case HOUSE_DIALOGUE_SHOP_REPAIR:
    {
      dialog_menu_id = (HOUSE_DIALOGUE_MENU)uMessageParam;
      pParty->sub_421B2C_PlaceInInventory_or_DropPickedItem();
      break;
    }
    case HOUSE_DIALOGUE_SHOP_DISPLAY_EQUIPMENT:
    {
      pDialogueWindow->eWindowType = WINDOW_MainMenu;
      UI_CreateEndConversationButton();
      InitializaDialogueOptions_Shops(in_current_building_type);
      break;
    }
    default:
    {
      if( uMessageParam >= HOUSE_DIALOGUE_36 && uMessageParam <= HOUSE_DIALOGUE_GUILD_LEARN_SKILL )
      {
        v36 = (signed __int64)(p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].flt_24 * 500.0);
        pPrice = v36 * (100 - pPlayers[uActiveCharacter]->GetMerchant()) / 100;
        if ( pPrice < v36 / 3 )
        pPrice = v36 / 3;
        if(byte_4ED970_skill_learn_ability_by_class_table[pPlayers[uActiveCharacter]->classType][uMessageParam - 36])
        {
          if ( !pPlayers[uActiveCharacter]->pActiveSkills[uMessageParam - 36] )
          {
            if ( pParty->uNumGold < pPrice )
            {
              ShowStatusBarString(pGlobalTXT_LocalizationStrings[155], 2);
              if ( in_current_building_type == BuildingType_Training || in_current_building_type == BuildingType_Tavern )
                PlayHouseSound((unsigned int)window_SpeakInHouse->ptr_1C, HouseSound_Goodbye);
              else
                PlayHouseSound((unsigned int)window_SpeakInHouse->ptr_1C, HouseSound_NotEnoughMoney_TrainingSuccessful);
            }
            else
            {
              Party::TakeGold(pPrice);
              dword_F8B1E4 = 1;
              pPlayers[uActiveCharacter]->pActiveSkills[uMessageParam - 36] = 1;
              pPlayers[uActiveCharacter]->PlaySound(SPEECH_78, 0);
            }
          }
        }
      }
      break;
    }
  }
}
//----- (004B6943) --------------------------------------------------------
void  TravelByTransport()
{
  signed int v4; // ebx@1
  stru365_travel_info *pTravel; // esi@7
  signed int v12; // esi@13
  signed int v13; // edi@14
  DWORD v14; // eax@26
  DWORD v15; // edi@26
  int pTextHeight; // eax@36
  int pRealTextHeight; // esi@36
  int schedule_id; // esi@39
  GUIButton *pButton; // ebx@39
  signed int v25; // eax@41
  MapInfo pMap; // [sp-3Ch] [bp-2CCh]@62
  char pTopicArray[5][100]; // [sp+14h] [bp-27Ch]@37
  GUIWindow travel_window; // [sp+208h] [bp-88h]@1
  int pPrimaryTextHeight; // [sp+260h] [bp-30h]@36
  int index; // [sp+27Ch] [bp-14h]@36
  unsigned int pPrice; // [sp+288h] [bp-8h]@1
  int travel_time; // [sp+28Ch] [bp-4h]@48
  enum PlayerSpeech pSpeech;
  unsigned int pCurrentButton;

  memcpy(&travel_window, window_SpeakInHouse, sizeof(travel_window));
  travel_window.uFrameX = 483;
  travel_window.uFrameWidth = 148;
  travel_window.uFrameZ = 334;

  v4 = p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].uType == BuildingType_Stables ? 25 : 50;
  v4 *= p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].fPriceMultiplier;

  pPrice = v4 * (100 - pPlayers[uActiveCharacter]->GetMerchant()) / 100;
  if ( (signed int)pPrice < v4 / 3 )
    pPrice = v4 / 3;
  if ( dialog_menu_id == HOUSE_DIALOGUE_MAIN)
  {
    if ( HouseUI_CheckIfPlayerCanInteract() )
    {
      index = 0;
      strcpy(pTopicArray[4], "");
      sprintfex(pTmpBuf2.data(), pGlobalTXT_LocalizationStrings[405], pPrice); // Price: %lu gold
      pTextHeight = pFontArrus->CalcTextHeight(pTmpBuf2.data(), &travel_window, 0, 0);
      pRealTextHeight = pTextHeight + (LOBYTE(pFontArrus->uFontHeight) - 3) + 146;
      pPrimaryTextHeight = pRealTextHeight;
      pCurrentButton = 2;
      for ( uint i = pDialogueWindow->pStartingPosActiveItem; i < (unsigned int)(pDialogueWindow->pNumPresenceButton + pDialogueWindow->pStartingPosActiveItem); ++i )
      {
        schedule_id = transport_routes[(unsigned int)window_SpeakInHouse->ptr_1C - HOUSE_STABLES_HARMONDALE][index];
        pButton = pDialogueWindow->GetControl(i);

        if (schedule_id != 255)
        {
          //if (schedule_id >= 25)
            //Log::Warning(L"Transport UI: schedule overflow");
          if ( pCurrentButton >= 6 )
            v25 = true;
          else
            v25 = transport_schedule[schedule_id].pSchedule[pParty->uDaysPlayed % 7];
        }

        if (schedule_id != 255 && v25 && (!transport_schedule[schedule_id].uQuestBit
           || _449B57_test_bit(pParty->_quest_bits, transport_schedule[schedule_id].uQuestBit)) )//������: �������� ��������� ������� ������ ����������� �����������
        {
  //get color for current string(����������� ����� ������� ������)----------
          if ( pDialogueWindow->pCurrentPosActiveItem == pCurrentButton )
            sprintf(pTopicArray[index], "\f%05d", Color16(255, 255, 155));
          else
            sprintf(pTopicArray[index], "\f%05d", Color16(255, 255, 255));
  //hired NPC premium(������ ������� ���)----------------------------------
          travel_time = transport_schedule[schedule_id].uTravelTime;
          if ( (unsigned int)window_SpeakInHouse->ptr_1C >= HOUSE_BOATS_EMERALD_ISLE )
          {
            if ( CheckHiredNPCSpeciality(Sailor) )
              travel_time -= 2;
            if ( CheckHiredNPCSpeciality(Navigator) )
              travel_time -= 3;
            if ( CheckHiredNPCSpeciality(Pirate) )
              travel_time -= 2;
          }
          else
          {
            if ( CheckHiredNPCSpeciality(Horseman) )
              travel_time -= 2;
          }
          if ( CheckHiredNPCSpeciality(Explorer) )
            --travel_time;

          if ( travel_time < 1 )
            travel_time = 1;
          if ( schedule_id != 255 )
          {
            memcpy(&pMap, &pMapStats->pInfos[transport_schedule[schedule_id].uMapInfoID], 0x44u);
            sprintfex(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[404], travel_time, pMap.pName); // Time - %d days, destination %s
            strcat(pTopicArray[index], pTmpBuf.data());
            strcat(pTopicArray[index], "\n \n");
            pButton->uY = pRealTextHeight;
            pTextHeight = pFontArrus->CalcTextHeight(pTmpBuf.data(), &travel_window, 0, 0);
            pButton->uHeight = pTextHeight;
            pButton->uW = pButton->uY + pTextHeight - 1;
            pRealTextHeight += (LOBYTE(pFontArrus->uFontHeight) - 3) + pTextHeight;
          }
        }
        else
        {
          strcpy(pTopicArray[index], "");
          if ( pButton )
          {
            pButton->uW = 0;
            pButton->uHeight = 0;
            pButton->uY = 0;
          }
        }
        ++index;
        ++pCurrentButton;
      }
      if ( pRealTextHeight != pPrimaryTextHeight )
      {
        sprintf(pTmpBuf.data(), "%s\n \n%s%s%s%s%s", pTmpBuf2.data(), pTopicArray[0], pTopicArray[1], pTopicArray[2], pTopicArray[3], pTopicArray[4]);
        travel_window.DrawTitleText(pFontArrus, 0, 146, 0, pTmpBuf.data(), 3);
      }
      else
      {
        travel_window.DrawTitleText(pFontArrus, 0, (174 - pFontArrus->CalcTextHeight(pGlobalTXT_LocalizationStrings[561], &travel_window, 0, 0)) / 2 + 138,//"��������, ��������� � ������ ����"
                          Color16(255, 255, 255), pGlobalTXT_LocalizationStrings[561], 3);
        pAudioPlayer->StopChannels(-1, -1);
      }
    }
  }
  else//����� ������� ������
  {
    if ( dialog_menu_id >= HOUSE_DIALOGUE_TRANSPORT_SCHEDULE_1 && dialog_menu_id <= HOUSE_DIALOGUE_TRANSPORT_SCHEDULE_4 )
    {
      if ( pParty->uNumGold < pPrice )
      {
        ShowStatusBarString(pGlobalTXT_LocalizationStrings[155], 2);//"� ��� �� ������� ������"
        PlayHouseSound((unsigned int)window_SpeakInHouse->ptr_1C, HouseSound_Greeting_2);
        pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, 0);
        return;
      }

      Party::TakeGold(pPrice);

      pTravel = &transport_schedule[transport_routes[(unsigned int)window_SpeakInHouse->ptr_1C - HOUSE_STABLES_HARMONDALE][dialog_menu_id - HOUSE_DIALOGUE_TRANSPORT_SCHEDULE_1]];
      if ( pTravel->pSchedule[pParty->uDaysPlayed % 7] )
      {
        if ( _stricmp(pCurrentMapName, pMapStats->pInfos[pTravel->uMapInfoID].pFilename) )
        {
          SaveGame(1, 0);
          strcpy(pCurrentMapName, pMapStats->pInfos[pTravel->uMapInfoID].pFilename);

          dword_6BE364_game_settings_1 |= GAME_SETTINGS_0001;
          _5B65B8_npcdata_hiword_house_or_other = 0;
          dword_5B65BC = 0;
          _5B65B4_npcdata_loword_house_or_other = pTravel->arrival_rot_y;
          uGameState = GAME_STATE_CHANGE_LOCATION;
          _5B65A8_npcdata_uflags_or_other = pTravel->arrival_x;
          _5B65AC_npcdata_fame_or_other = pTravel->arrival_y;
          _5B65B0_npcdata_rep_or_other = pTravel->arrival_z;
          dword_5B65C0 = pTravel->arrival_x | pTravel->arrival_y | pTravel->arrival_z | pTravel->arrival_rot_y;
        }
        else
        {
          pGame->pIndoorCameraD3D->sRotationY = 0;

          pParty->uFlags |= 2u;
          pParty->vPosition.x = pTravel->arrival_x;
          pParty->vPosition.y = pTravel->arrival_y;
          pParty->vPosition.z = pTravel->arrival_z;
          pParty->uFallStartY = pParty->vPosition.z;
          pParty->sRotationX = 0;
          pParty->sRotationY = pTravel->arrival_rot_y;
        }
        PlayHouseSound((unsigned int)window_SpeakInHouse->ptr_1C, HouseSound_NotEnoughMoney_TrainingSuccessful);
        v12 = pTravel->uTravelTime;
        if ( (signed int)window_SpeakInHouse->ptr_1C >= 63 )
        {
          pSpeech = SPEECH_SetSail;
          v13 = 2500;
          if ( CheckHiredNPCSpeciality(Sailor) )
            v12 -= 2;
          if ( CheckHiredNPCSpeciality(Navigator) )
            v12 -= 3;
          if ( CheckHiredNPCSpeciality(Pirate) )
            v12 -= 2;
        }
        else
        {
          pSpeech = SPEECH_CarriageReady;
          v13 = 1500;
          if ( CheckHiredNPCSpeciality(Horseman) )
            v12 -= 2;
        }
        if ( CheckHiredNPCSpeciality(Explorer) )
          --v12;
        if ( v12 < 1 )
          v12 = 1;
        RestAndHeal(1440 * v12);
        pPlayers[uActiveCharacter]->PlaySound(pSpeech, 0);
        v14 = GetTickCount();
        v15 = v14 + v13;
        if ( v15 < v14 )
          v15 = v14;
        while ( GetTickCount() < v15 )
          sqrt(3.1415926);
        while ( HouseDialogPressCloseBtn() )
          ;
        pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 0, 0);
        return;
      }
      else
      {
        dialog_menu_id = HOUSE_DIALOGUE_MAIN;
        pAudioPlayer->PlaySound(SOUND_error, 0, 0, -1, 0, 0, 0, 0);
      }
    }
  }
}
//----- (004B68EA) --------------------------------------------------------
bool __fastcall IsTravelAvailable(int a1)
{
  for ( uint i = 0; i < 4; ++i )
  {
    if ( transport_schedule[transport_routes[a1][i]].pSchedule[pParty->uDaysPlayed % 7] )
    {
      if (!transport_schedule[transport_routes[a1][i]].uQuestBit || _449B57_test_bit(pParty->_quest_bits, transport_schedule[transport_routes[a1][i]].uQuestBit))
        return true;
    }
  }
  return false;
}

//----- (004B7911) --------------------------------------------------------
void  TownHallDialog()
{
  int v1; // eax@10
  int v2; // esi@10
  signed int pStringSum; // ebx@24
  signed int v16; // ebx@28
  int v17; // ebx@28
  GUIButton *pButton; // eax@30
  int pTextHeight; // eax@30
  unsigned __int16 pTextColor; // ax@30
  GUIWindow window; // [sp+60h] [bp-BCh]@21
  GUIWindow townHall_window; // [sp+B4h] [bp-68h]@1
  int v29; // [sp+10Ch] [bp-10h]@28
  int v31; // [sp+114h] [bp-8h]@29
  GUIFont *pOutString; // [sp+118h] [bp-4h]@21

  memcpy(&townHall_window, window_SpeakInHouse, sizeof(townHall_window));
  townHall_window.uFrameX = 483;
  townHall_window.uFrameWidth = 148;
  townHall_window.uFrameZ = 334;
  sprintf(pTmpBuf.data(), "%s: %d", pGlobalTXT_LocalizationStrings[605], pParty->uFine);//������� �����
  townHall_window.DrawTitleText(pFontArrus, 0, 260, Color16(0xFFu, 0xFFu, 0x9Bu), pTmpBuf.data(), 3);
  switch(dialog_menu_id)
  {
    case HOUSE_DIALOGUE_MAIN:
    {
      pStringSum = 1;
      pTextHeight = 0;
      pShopOptions[0] = const_cast<char *>(pGlobalTXT_LocalizationStrings[604]);//����� �� ��������������
      if ( pParty->uFine > 0 )
      {
        pShopOptions[1] = const_cast<char *>(pGlobalTXT_LocalizationStrings[603]);//��������� �����
        pStringSum = 2;
      }
      for ( uint i = 0; i < pStringSum; ++i )
        pTextHeight += pFontArrus->CalcTextHeight(pShopOptions[i], &townHall_window, 0, 0);
      v29 = (100 - pTextHeight) / pStringSum;
      v16 = 80 - pStringSum * ((100 - pTextHeight) / pStringSum);
      v17 = (v16 / 2) - v29 / 2 + 158;
      if ( pDialogueWindow->pNumPresenceButton > 0 )
      {
        v31 = 2;
        uint j = 0;
        for ( uint i = pDialogueWindow->pStartingPosActiveItem;
              i < pDialogueWindow->pNumPresenceButton + pDialogueWindow->pStartingPosActiveItem; ++i )
        {
          pButton = pDialogueWindow->GetControl(i);
          pButton->uY = v29 + v17;
          pTextHeight = pFontArrus->CalcTextHeight(pShopOptions[j], &townHall_window, 0, 0);
          pButton->uHeight = pTextHeight;
          v17 = pButton->uY + pTextHeight - 1;
          pButton->uW = v17;
          pTextColor = Color16(0xFFu, 0xFFu, 0x9Bu);
          if ( pDialogueWindow->pCurrentPosActiveItem != v31 )
            pTextColor = Color16(0xFFu, 0xFFu, 0xFFu);
          townHall_window.DrawTitleText(pFontArrus, 0, pButton->uY, pTextColor, pShopOptions[j], 3);
          ++v31;
          ++j;
        }
      }
      break;
    }
    case HOUSE_DIALOGUE_TOWNHALL_MESSAGE:
    {
      sprintf(pTmpBuf.data(), "\f%05d%s\f%05d", Color16(0xFFu, 0xFFu, 0x9Bu),
      pMonsterStats->pInfos[bountyHunting_monster_id_for_hunting].pName, Color16(0xFFu, 0xFFu, 0xFFu));
      sprintf(pTmpBuf2.data(), bountyHunting_text, pTmpBuf.data(), 100 * pMonsterStats->pInfos[bountyHunting_monster_id_for_hunting].uLevel);
      current_npc_text = pTmpBuf2.data();
      memcpy(&window, pDialogueWindow, sizeof(window));
      window.uFrameWidth = 458;
      window.uFrameZ = 457;
      pOutString = pFontArrus;
      pTextHeight = pFontArrus->CalcTextHeight(pTmpBuf2.data(), &window, 13, 0) + 7;
      if ( 352 - pTextHeight < 8 )
      {
        pOutString = pFontCreate;
        pTextHeight = pFontCreate->CalcTextHeight(current_npc_text, &window, 13, 0) + 7;
      }
      pRenderer->GetLeather(8, 352 - pTextHeight, pIcons_LOD->GetTexture(uTextureID_Leather), pIcons_LOD->GetTexture(uTextureID_Leather)->uTextureHeight - pTextHeight);
      pRenderer->DrawTextureIndexed(8, 347 - pTextHeight, pTexture_591428);
      window.DrawText(pOutString, 13, 354 - pTextHeight, 0, FitTextInAWindow(current_npc_text, pOutString, &window, 13, 0), 0, 0, 0);
      break;
    }
    case HOUSE_DIALOGUE_TOWNHALL_PAY_FINE:
    {
      if ( window_SpeakInHouse->receives_keyboard_input_2 == WINDOW_INPUT_IN_PROGRESS)
      {
        sprintfex(pTmpBuf.data(), "%s\n%s", pGlobalTXT_LocalizationStrings[606], pGlobalTXT_LocalizationStrings[112]); // "Pay"   "How Much?"
        townHall_window.DrawTitleText(pFontArrus, 0, 146, Color16(0xFFu, 0xFFu, 0x9Bu), pTmpBuf.data(), 3);
        townHall_window.DrawTitleText(pFontArrus, 0, 186, Color16(0xFFu, 0xFFu, 0xFFu), (const char *)pKeyActionMap->pPressedKeysBuffer, 3);
        townHall_window.DrawFlashingInputCursor(pFontArrus->GetLineWidth((const char *)pKeyActionMap->pPressedKeysBuffer) / 2 + 80, 185, pFontArrus);
        return;
      }
      if ( window_SpeakInHouse->receives_keyboard_input_2 == WINDOW_INPUT_CONFIRMED)
      {
        v1 = atoi((const char *)pKeyActionMap->pPressedKeysBuffer);
        v2 = atoi((const char *)pKeyActionMap->pPressedKeysBuffer);
        if ( v1 <= 0 )
        {
          pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, 0);
          return;
        }
        if ( v1 > pParty->uNumGold )
        {
          PlayHouseSound((unsigned int)window_SpeakInHouse->ptr_1C, HouseSound_NotEnoughMoney_TrainingSuccessful);
          v2 = pParty->uNumGold;
        }
        if ( v2 > pParty->uFine )
          v2 = pParty->uFine;
        Party::TakeGold(v2);
        pParty->uFine -= v2;
        if ( pParty->uFine < 0 )
          pParty->uFine = 0;
        if ( uActiveCharacter )
          pPlayers[uActiveCharacter]->PlaySound(SPEECH_81, 0);
      }
      if ( window_SpeakInHouse->receives_keyboard_input_2 == WINDOW_INPUT_CANCELLED)
      {
        window_SpeakInHouse->receives_keyboard_input_2 = WINDOW_INPUT_NONE;
        pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, 0);
      }
      break;
    }
    default:
      break;
  }
  return;
}

//----- (004B7D7E) --------------------------------------------------------
void  BankDialog()
{
  int entered_sum; // eax@19
  unsigned int takes_sum; // esi@19
  unsigned __int16 pColorText; // ax@27
  GUIWindow bank_window; // [sp+Ch] [bp-5Ch]@1

  memcpy(&bank_window, window_SpeakInHouse, sizeof(bank_window));
  bank_window.uFrameX = 483;
  bank_window.uFrameWidth = 148;
  bank_window.uFrameZ = 334;
  sprintf(pTmpBuf.data(), "%s: %d", pGlobalTXT_LocalizationStrings[25], pParty->uNumGoldInBank);//������
  bank_window.DrawTitleText(pFontArrus, 0, 220, Color16(0xFFu, 0xFFu, 0x9Bu), pTmpBuf.data(), 3);
  switch(dialog_menu_id)
  {
    case HOUSE_DIALOGUE_MAIN:
    {
      pColorText = Color16(0xFFu, 0xFFu, 0x9Bu);
      if ( pDialogueWindow->pCurrentPosActiveItem != 2 )
        pColorText = Color16(0xFFu, 0xFFu, 0xFFu);
      bank_window.DrawTitleText(pFontArrus, 0, 146, pColorText, pGlobalTXT_LocalizationStrings[60], 3);
      pColorText = Color16(0xFFu, 0xFFu, 0x9Bu);
      if ( pDialogueWindow->pCurrentPosActiveItem != 3 )
        pColorText = Color16(0xFFu, 0xFFu, 0xFFu);
      bank_window.DrawTitleText(pFontArrus, 0, 176, pColorText, pGlobalTXT_LocalizationStrings[244], 3);
      break;
    }
    case HOUSE_DIALOGUE_BANK_PUT_GOLD:
    {
      if ( window_SpeakInHouse->receives_keyboard_input_2 == WINDOW_INPUT_IN_PROGRESS)
      {
        sprintf(pTmpBuf.data(), "%s\n%s", pGlobalTXT_LocalizationStrings[60], pGlobalTXT_LocalizationStrings[112]);//"��������" "�������?"
        bank_window.DrawTitleText(pFontArrus, 0, 146, Color16(0xFFu, 0xFFu, 0x9Bu), pTmpBuf.data(), 3);
        bank_window.DrawTitleText(pFontArrus, 0, 186, Color16(0xFFu, 0xFFu, 0xFFu), (const char *)pKeyActionMap->pPressedKeysBuffer, 3);
        bank_window.DrawFlashingInputCursor(pFontArrus->GetLineWidth((const char *)pKeyActionMap->pPressedKeysBuffer) / 2 + 80, 185, pFontArrus);
        return;
      }
      if ( window_SpeakInHouse->receives_keyboard_input_2 == WINDOW_INPUT_CONFIRMED)
      {
        entered_sum = atoi((const char *)pKeyActionMap->pPressedKeysBuffer);
        takes_sum = entered_sum;
        if ( !entered_sum )
        {
          pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, 0);
          return;
        }
        if ( entered_sum > pParty->uNumGold )
        {
          PlayHouseSound((unsigned int)window_SpeakInHouse->ptr_1C, HouseSound_NotEnoughMoney_TrainingSuccessful);
          takes_sum = pParty->uNumGold;
        }
        if ( takes_sum )
        {
          Party::TakeGold(takes_sum);
          pParty->uNumGoldInBank += takes_sum;
          if ( uActiveCharacter )
            pPlayers[uActiveCharacter]->PlaySound(SPEECH_81, 0);
        }
        window_SpeakInHouse->receives_keyboard_input_2 = WINDOW_INPUT_NONE;
        pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, 0);
        return;
      }
      if (window_SpeakInHouse->receives_keyboard_input_2 == WINDOW_INPUT_CANCELLED)
      {
        window_SpeakInHouse->receives_keyboard_input_2 = WINDOW_INPUT_NONE;
        pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, 0);
      }
      return;
    }
    case HOUSE_DIALOGUE_BANK_GET_GOLD:
    {
      if ( window_SpeakInHouse->receives_keyboard_input_2 == WINDOW_INPUT_IN_PROGRESS)
      {
        sprintfex(pTmpBuf.data(), "%s\n%s", pGlobalTXT_LocalizationStrings[244], pGlobalTXT_LocalizationStrings[112]);//"�����" "�������?"
        bank_window.DrawTitleText(pFontArrus, 0, 146, Color16(0xFFu, 0xFFu, 0x9Bu), pTmpBuf.data(), 3);
        bank_window.DrawTitleText(pFontArrus, 0, 186, Color16(0xFFu, 0xFFu, 0xFFu), (const char *)pKeyActionMap->pPressedKeysBuffer, 3);
        bank_window.DrawFlashingInputCursor(pFontArrus->GetLineWidth((const char *)pKeyActionMap->pPressedKeysBuffer) / 2 + 80, 185, pFontArrus);
        return;
      }
      if ( window_SpeakInHouse->receives_keyboard_input_2 == WINDOW_INPUT_CONFIRMED)
      {
        window_SpeakInHouse->receives_keyboard_input_2 = WINDOW_INPUT_NONE;
        entered_sum = atoi((const char *)pKeyActionMap->pPressedKeysBuffer);
        takes_sum = entered_sum;
        if ( entered_sum )
        {
          if ( entered_sum > pParty->uNumGoldInBank )
          {
            PlayHouseSound((unsigned int)window_SpeakInHouse->ptr_1C, HouseSound_NotEnoughMoney_TrainingSuccessful);
            takes_sum = pParty->uNumGoldInBank;
          }
          if ( takes_sum )
          {
            Party::SetGold(pParty->uNumGold + takes_sum);
            pParty->uNumGoldInBank -= takes_sum;
          }
        }
        window_SpeakInHouse->receives_keyboard_input_2 = WINDOW_INPUT_NONE;
        pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, 0);
        return;
      }
      if ( window_SpeakInHouse->receives_keyboard_input_2 == WINDOW_INPUT_CANCELLED)
      {
        window_SpeakInHouse->receives_keyboard_input_2 = WINDOW_INPUT_NONE;
        pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, 0);
      }
      return;
    }
    default:
    {
      break;
    }
  }
}
// F8B19C: using guessed type int dword_F8B19C;

//----- (004B8285) --------------------------------------------------------
void  TavernDialog()
{
  int pPriceRoom;
  int pPriceFood;
  int pPriceSkill;
  int pItemNum;
  double v2; // st7@1
  int pNumString; // edi@16
  signed int v9; // esi@16
  unsigned int pColorText; // eax@57
  GUIButton *pButton; // eax@65
  int pSkillCount;
  signed int pOptionsCount; // edi@77
  signed int i; // esi@79
//  signed int v53; // edi@81
  int v54; // edi@81
  const char *pText; // [sp-4h] [bp-278h]@93
  char pTopic1[100]; // [sp+Ch] [bp-268h]@55
  char pTopic4[100]; // [sp+70h] [bp-204h]@59
  char pTopic2[100]; // [sp+D4h] [bp-1A0h]@57
  char pTopic3[100]; // [sp+138h] [bp-13Ch]@59
  GUIWindow dialog_window; // [sp+1F0h] [bp-84h]@1
  int v91; // [sp+270h] [bp-4h]@3
  unsigned int pTopic1Height; // [sp+26Fh] [bp-5h]@55
  unsigned __int8 pTopic2Height; // [sp+267h] [bp-Dh]@57
  unsigned __int8 pTopic3Height; // [sp+253h] [bp-21h]@59
  unsigned __int8 pTopic4Height;
  int pTextHeight;
  int all_text_height; // [sp+260h] [bp-14h]@18
  GUIFont *pOutString;

  memcpy(&dialog_window, window_SpeakInHouse, sizeof(dialog_window));
  dialog_window.uFrameX = 483;
  dialog_window.uFrameWidth = 148;
  dialog_window.uFrameZ = 334;
  v2 = p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].fPriceMultiplier;

  pPriceRoom = ((v2 * v2) / 10) * (100 - pPlayers[uActiveCharacter]->GetMerchant()) / 100;
  if ( pPriceRoom < ((v2 * v2) / 10) / 3 )
    pPriceRoom = ((v2 * v2) / 10) / 3;
  if ( pPriceRoom <= 0 )
    pPriceRoom = 1;

  pPriceFood = ((v2 * v2) * v2 / 100) * (100 - pPlayers[uActiveCharacter]->GetMerchant()) / 100;
  if ( pPriceFood < ((v2 * v2) * v2 / 100) / 3 )
    pPriceFood = ((v2 * v2) * v2 / 100) / 3;
  if ( pPriceFood <= 0 )
    pPriceFood = 1;


  switch(dialog_menu_id)
  {
    case HOUSE_DIALOGUE_MAIN:
    {
      if ( !HouseUI_CheckIfPlayerCanInteract() )
          return;

      sprintf(pTopic1, "\f%05d", pDialogueWindow->pCurrentPosActiveItem == 2 ? Color16(0xFFu, 0xFFu, 0x9Bu) : Color16(0xFFu, 0xFFu, 0xFFu));
      sprintfex(pTmpBuf2.data(), pGlobalTXT_LocalizationStrings[178], pPriceRoom); // Rent room for %d gold
      strcat(pTopic1, pTmpBuf2.data());
      pTopic1Height = pFontArrus->CalcTextHeight(pTopic1, &dialog_window, 0, 0);
      strcat(pTopic1, "\n \n");

      sprintf(pTopic2, "\f%05d", pDialogueWindow->pCurrentPosActiveItem == 3 ? Color16(0xFFu, 0xFFu, 0x9Bu) : Color16(0xFFu, 0xFFu, 0xFFu));
      sprintfex(pTmpBuf2.data(), pGlobalTXT_LocalizationStrings[86], // Buy food for %d days for %d gold
        (unsigned int)p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].fPriceMultiplier, pPriceFood);
      strcat(pTopic2, pTmpBuf2.data());
      pTopic2Height = pFontArrus->CalcTextHeight(pTopic2, &dialog_window, 0, 0);
      strcat(pTopic2, "\n \n");

      sprintf(pTopic3, "\f%05d", pDialogueWindow->pCurrentPosActiveItem == 4 ? Color16(0xFFu, 0xFFu, 0x9Bu) : Color16(0xFFu, 0xFFu, 0xFFu));
      strcat(pTopic3, pGlobalTXT_LocalizationStrings[160]); // Learn Skills
      pTopic3Height = pFontArrus->CalcTextHeight(pTopic3, &dialog_window, 0, 0);
      strcat(pTopic3, "\n \n");
      pTopic4[0] = 0;
      if ( (signed int)window_SpeakInHouse->par1C >= 108 && (signed int)window_SpeakInHouse->par1C <= 120 )
      {
        sprintf(pTopic4, "\f%05d", pDialogueWindow->pCurrentPosActiveItem == 5 ? Color16(0xFFu, 0xFFu, 0x9Bu) : Color16(0xFFu, 0xFFu, 0xFFu));
        strcat(pTopic4, pGlobalTXT_LocalizationStrings[611]); // Play Arcomage
        pTopic4Height = pFontArrus->CalcTextHeight(pTopic4, &dialog_window, 0, 0);
      }

      if ( pDialogueWindow->pNumPresenceButton )
      {
        for ( pItemNum = pDialogueWindow->pStartingPosActiveItem;
              pItemNum < pDialogueWindow->pStartingPosActiveItem + pDialogueWindow->pNumPresenceButton; ++pItemNum )
        {
          pButton = pDialogueWindow->GetControl(pItemNum);
          if ( pButton->msg_param == 15 )
          {
            pButton->uHeight = pTopic1Height;
            pButton->uY = 146;
            pButton->uW = pTopic1Height + 145;
          }
          else if ( pButton->msg_param == 16 )
          {
            pButton->uHeight = pTopic2Height;
            pButton->uY = pTopic1Height + (LOBYTE(pFontArrus->uFontHeight) - 3) + 146;
            pButton->uW = (pTopic1Height + (LOBYTE(pFontArrus->uFontHeight) - 3) + 146) + pTopic2Height - 1;
          }
          else if ( pButton->msg_param == 96 )
          {
            pButton->uY = pTopic1Height + pTopic2Height + 2 * (LOBYTE(pFontArrus->uFontHeight) - 3) + 146;
            pButton->uHeight = pTopic3Height;
            pButton->uW = pTopic3Height + (pTopic1Height + pTopic2Height + 2 * (LOBYTE(pFontArrus->uFontHeight) - 3) + 146) - 1;
          }
          else if ( pButton->msg_param == 101 )
          {
            pButton->uHeight = pTopic4Height;
            pButton->uY = pTopic1Height + 3 * (LOBYTE(pFontArrus->uFontHeight) - 3) + pTopic4Height + pTopic2Height + 146;
            pButton->uW = (pTopic1Height + 3 * (LOBYTE(pFontArrus->uFontHeight) - 3) + pTopic4Height + pTopic2Height + 146) + pTopic4Height - 1;
          }
        }
        sprintfex(pTmpBuf.data(), "%s%s%s%s", pTopic1, pTopic2, pTopic3, pTopic4);
        dialog_window.DrawTitleText(pFontArrus, 0, 146, 0, pTmpBuf.data(), 3);
      }
      break;
    }
    case HOUSE_DIALOGUE_TAVERN_ARCOMAGE_RULES:
    {
      pOutString = pFontArrus;
      strcpy(pTmpBuf.data(), pNPCTopics[354].pText);
      dialog_window.uFrameWidth = game_viewport_width;
      dialog_window.uFrameZ = 452;
      pTextHeight = pFontArrus->CalcTextHeight(pTmpBuf.data(), &dialog_window, 12, 0) + 7;
      if ( 352 - pTextHeight < 8 )
      {
        pOutString = pFontCreate;
        pTextHeight = pFontCreate->CalcTextHeight(pTmpBuf.data(), &dialog_window, 12, 0) + 7;
      }
      pRenderer->GetLeather(8, 352 - pTextHeight, pIcons_LOD->GetTexture(uTextureID_Leather), pIcons_LOD->GetTexture(uTextureID_Leather)->uTextureHeight - pTextHeight);
      pRenderer->DrawTextureIndexed(8, 347 - pTextHeight, pTexture_591428);
      window_SpeakInHouse->DrawText(pOutString, 12, 354 - pTextHeight, 0, FitTextInAWindow(pTmpBuf.data(), pOutString, &dialog_window, 0xCu, 0), 0, 0, 0);
      break;
    }
    case HOUSE_DIALOGUE_TAVERN_ARCOMAGE_VICTORY_CONDITIONS:
    {
      strcpy(pTmpBuf.data(), pNPCTopics[(uint)window_SpeakInHouse->ptr_1C + 247].pText);
      dialog_window.uFrameWidth = game_viewport_width;
      dialog_window.uFrameZ = 452;
      pTextHeight = pFontArrus->CalcTextHeight(pTmpBuf.data(), &dialog_window, 12, 0) + 7;
      pRenderer->GetLeather(8, 352 - pTextHeight, pIcons_LOD->GetTexture(uTextureID_Leather), pIcons_LOD->GetTexture(uTextureID_Leather)->uTextureHeight - pTextHeight);
      pRenderer->DrawTextureIndexed(8, 347 - pTextHeight, pTexture_591428);
      window_SpeakInHouse->DrawText(pFontArrus, 12, 354 - pTextHeight, 0,
                                    FitTextInAWindow(pTmpBuf.data(), pFontArrus, &dialog_window, 0xCu, 0), 0, 0, 0);
      break;
    }
    case HOUSE_DIALOGUE_TAVERN_ARCOMAGE_RESULT:
    {
      if ( pArcomageGame->bGameInProgress == 1 )
        return;
      if ( pArcomageGame->uGameWinner )
      {
        if ( pArcomageGame->uGameWinner == 1 )
          pText = pGlobalTXT_LocalizationStrings[640];// You won!
        else
          pText = pGlobalTXT_LocalizationStrings[641];// You lost!
      }
      else
      {
        pText = pGlobalTXT_LocalizationStrings[639];// A tie!
      }
      strcpy(pTmpBuf.data(), pText);
      dialog_window.DrawTitleText(pFontArrus, 0, (174 - pFontArrus->CalcTextHeight(pTmpBuf.data(), &dialog_window, 0, 0)) / 2 + 138,
                                  Color16(0xFFu, 0xFFu, 0x9Bu), pTmpBuf.data(), 3);
      break;
    }
    case HOUSE_DIALOGUE_TAVERN_REST:
    {
      if ( pParty->uNumGold >= pPriceRoom )
      {
        Party::TakeGold(pPriceRoom);
        PlayHouseSound((unsigned int)window_SpeakInHouse->ptr_1C, HouseSound_NotEnoughMoney_TrainingSuccessful);
        dialog_menu_id = HOUSE_DIALOGUE_NULL;
        HouseDialogPressCloseBtn();
        GetHouseGoodbyeSpeech();
        pVideoPlayer->Unload();
        /*if ( pMessageQueue_50CBD0->uNumMessages )
          pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
        pMessageQueue_50CBD0->pMessages[0].eType = UIMSG_RentRoom;
        pMessageQueue_50CBD0->pMessages[0].param = (int)window_SpeakInHouse->ptr_1C;//107
        pMessageQueue_50CBD0->pMessages[0].field_8 = 1;
        ++pMessageQueue_50CBD0->uNumMessages;*/
        pMessageQueue_50CBD0->AddMessage(UIMSG_RentRoom, (int)window_SpeakInHouse->ptr_1C, 1);
        window_SpeakInHouse->Release();
        window_SpeakInHouse = 0;
        return;
      }
      ShowStatusBarString(pGlobalTXT_LocalizationStrings[155], 2);//� ��� �� ������� ������
      PlayHouseSound((unsigned int)window_SpeakInHouse->ptr_1C, HouseSound_Goodbye);
      pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, 0);
      break;
    }

    case HOUSE_DIALOGUE_LEARN_SKILLS:
    {
      if ( !HouseUI_CheckIfPlayerCanInteract() )
        return;
      pSkillCount = 0;
      v9 = (signed __int64)(p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].flt_24 * 500.0);
      pPriceSkill = v9 * (100 - pPlayers[uActiveCharacter]->GetMerchant()) / 100;
      if ( pPriceSkill < v9 / 3 )
        pPriceSkill = v9 / 3;
      all_text_height = 0;
      for ( int i = pDialogueWindow->pStartingPosActiveItem;
            i < pDialogueWindow->pStartingPosActiveItem + pDialogueWindow->pNumPresenceButton; ++i )
      {
        if ( byte_4ED970_skill_learn_ability_by_class_table[pPlayers[uActiveCharacter]->classType][pDialogueWindow->GetControl(i)->msg_param - 36]
            && !pPlayers[uActiveCharacter]->pActiveSkills[pDialogueWindow->GetControl(i)->msg_param - 36] )
        {
          all_text_height = pFontArrus->CalcTextHeight(pSkillNames[pDialogueWindow->GetControl(i)->msg_param - 36], &dialog_window, 0, 0);
          pSkillCount++;
        }
      }
      if ( pSkillCount )
      {
        sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[401], pPriceSkill);//��������� ������: %lu
        dialog_window.DrawTitleText(pFontArrus, 0, 0x92u, 0, pTmpBuf.data(), 3);
        v91 = (149 - all_text_height) / pSkillCount;
        if ( (149 - all_text_height) / pSkillCount > 32 )
          v91 = 32;
        v54 = (149 - pSkillCount * v91 - all_text_height) / 2 - v91 / 2 + 162;
        for ( int i = pDialogueWindow->pStartingPosActiveItem;
              i < pDialogueWindow->pStartingPosActiveItem + pDialogueWindow->pNumPresenceButton; i++ )
        {
          pButton = pDialogueWindow->GetControl(i);
          if ( !byte_4ED970_skill_learn_ability_by_class_table[pPlayers[uActiveCharacter]->classType][pButton->msg_param - 36]
            || pPlayers[uActiveCharacter]->pActiveSkills[pButton->msg_param - 36] )
          {
            pButton->uW = 0;
            pButton->uHeight = 0;
            pButton->uY = 0;
          }
          else
          {
            pButton->uY = v91 + v54;
            pTextHeight = pFontArrus->CalcTextHeight(pSkillNames[pButton->msg_param - 36], &dialog_window, 0, 0);
            pButton->uHeight = pTextHeight;
            v54 = pTextHeight + pButton->uY - 1;
            pButton->uW = v54;
            pColorText = Color16(0xFFu, 0xFFu, 0x9Bu);
            if ( pDialogueWindow->pCurrentPosActiveItem != i )
              pColorText = Color16(0xFFu, 0xFFu, 0xFFu);
            dialog_window.DrawTitleText(pFontArrus, 0, pButton->uY, pColorText, pSkillNames[pButton->msg_param - 36], 3);
          }
        }
        return;
      }
      sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[544], pPlayers[uActiveCharacter]->pName,//������� ��� %s %s �������� ������ ��� ���-������
              pClassNames[pPlayers[uActiveCharacter]->classType]);
      strcat(pTmpBuf.data(), "\n \n");
      strcat(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[528]);//������ ������ �� ���� ����������.
      pTextHeight = (174 - pFontArrus->CalcTextHeight(pTmpBuf.data(), &dialog_window, 0, 0)) / 2 + 138;
      dialog_window.DrawTitleText(pFontArrus, 0, pTextHeight, Color16(0xFFu, 0xFFu, 0x9Bu), pTmpBuf.data(), 3);
      return;
    }

    case HOUSE_DIALOGUE_TAVERN_BUY_FOOD:
    {
      if ( (double)pParty->uNumFoodRations >= p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].fPriceMultiplier )
      {
        ShowStatusBarString(pGlobalTXT_LocalizationStrings[140], 2);//�� ��� ������ ���!
        if ( uActiveCharacter )
          pPlayers[uActiveCharacter]->PlaySound(SPEECH_67, 0);
        pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, 0);
        return;
      }
      if ( pParty->uNumGold >= pPriceFood )
      {
        Party::TakeGold(pPriceFood);
        pParty->uNumFoodRations = (signed __int64)p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].fPriceMultiplier;
        PlayHouseSound((unsigned int)window_SpeakInHouse->ptr_1C, HouseSound_Greeting_2);
        pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, 0);
        return;
      }
      ShowStatusBarString(pGlobalTXT_LocalizationStrings[155], 2);  // "You don't have enough gold"
      PlayHouseSound((unsigned int)window_SpeakInHouse->ptr_1C, HouseSound_Goodbye);
      pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, 0);
      break;
    }

    case HOUSE_DIALOGUE_TAVERN_ARCOMAGE_MAIN:
    {
      if ( HouseUI_CheckIfPlayerCanInteract() )
      {
        pOptionsCount = 2;
        pShopOptions[0] = const_cast<char *>(pGlobalTXT_LocalizationStrings[620]);//�������
        pShopOptions[1] = const_cast<char *>(pGlobalTXT_LocalizationStrings[622]);//������� ������
        if ( pParty->HasItem(651) )//651 - ������ ��� ���� � ��������� � Items.txt
        {
          pShopOptions[2] = const_cast<char *>(pGlobalTXT_LocalizationStrings[621]);//������
          pOptionsCount = 3;
        }
        for ( i = 0; i < pOptionsCount; ++i )
          all_text_height = pFontArrus->CalcTextHeight(pShopOptions[i], &dialog_window, 0, 0);
        all_text_height = (174 - all_text_height) / pOptionsCount;
        
        v54 = (174 - pOptionsCount * (174 - all_text_height) / pOptionsCount - all_text_height) / 2 - (174 - all_text_height) / pOptionsCount / 2 + 138;
        pNumString = 0;
        for ( pItemNum = pDialogueWindow->pStartingPosActiveItem;
              pItemNum < pDialogueWindow->pNumPresenceButton + pDialogueWindow->pStartingPosActiveItem ; pItemNum++ )
        {
          pButton = pDialogueWindow->GetControl(pItemNum);
          pButton->uY = all_text_height + v54;
          pTextHeight = pFontArrus->CalcTextHeight((const char *)pShopOptions[pNumString], &dialog_window, 0, 0);
          pButton->uHeight = pTextHeight;
          v54 = pButton->uY + pTextHeight - 1;
          pButton->uW = v54;
          pColorText = Color16(0xFFu, 0xFFu, 0x9Bu);
          if ( pDialogueWindow->pCurrentPosActiveItem != pItemNum )
            pColorText = Color16(0xFFu, 0xFFu, 0xFFu);
          dialog_window.DrawTitleText(pFontArrus, 0, pButton->uY, pColorText, (const char *)pShopOptions[pNumString], 3);
          ++pNumString;
        }
      }
      break;
    }
    default:
    {
      break;
    }
  }
}

//----- (004B705E) --------------------------------------------------------
void TempleDialog()
{
  int pPrice; // edi@1
  int pTextHeight; // eax@11
  unsigned __int16 pTextColor; // ax@21
  DDM_DLV_Header *v26; // edi@29
  unsigned int v30; // edx@36
  int v35; // edi@50
  GUIButton *pButton; // edi@64
//  int v47; // edi@71
  GUIWindow tample_window; // [sp+13Ch] [bp-88h]@1
  unsigned __int8 index; // [sp+1B7h] [bp-Dh]@64
  int v64; // [sp+1B8h] [bp-Ch]@6
  unsigned int pCurrentItem; // [sp+1BCh] [bp-8h]@6
  int all_text_height; // [sp+1C0h] [bp-4h]@6

  memcpy(&tample_window, window_SpeakInHouse, sizeof(tample_window));
  tample_window.uFrameX = 483;
  tample_window.uFrameWidth = 148;
  tample_window.uFrameZ = 334;
  pPrice = pPlayers[uActiveCharacter]->GetTempleHealCostModifier(p2DEvents[window_SpeakInHouse->par1C - 1].fPriceMultiplier);
  if ( dialog_menu_id == HOUSE_DIALOGUE_MAIN )
  {
    index = 1;
    pButton = pDialogueWindow->GetControl(pDialogueWindow->pStartingPosActiveItem);
    pButton->uHeight = 0;
    pButton->uY = 0;
    if ( pPlayers[uActiveCharacter]->IsPlayerHealableByTemple() )
    {
      sprintfex(pTmpBuf.data(), "%s %d %s", pGlobalTXT_LocalizationStrings[104], pPrice, pGlobalTXT_LocalizationStrings[97]);//"������" "������"
      pShopOptions[0] = pTmpBuf.data();
      index = 0;
    }
    pShopOptions[1] = const_cast<char *>(pGlobalTXT_LocalizationStrings[68]);//"������������"
    pShopOptions[2] = const_cast<char *>(pGlobalTXT_LocalizationStrings[160]);//"��������� �������"
    all_text_height = 0;
    if ( index < pDialogueWindow->pNumPresenceButton )
    {
      uint i = index;
      for ( uint j = index; j < pDialogueWindow->pNumPresenceButton; ++j )
      {
        all_text_height += pFontArrus->CalcTextHeight(pShopOptions[1 * i], &tample_window, 0, 0);
        i++;
      }
    }
    v64 = (174 - (signed int)all_text_height) / (pDialogueWindow->pNumPresenceButton - index);
    if ( v64 > 32 )
      v64 = 32;
    all_text_height = (174 - v64 * (pDialogueWindow->pNumPresenceButton - index) - (signed int)all_text_height) / 2 - v64 / 2 + 138;
    if ( index + pDialogueWindow->pStartingPosActiveItem < pDialogueWindow->pStartingPosActiveItem + pDialogueWindow->pNumPresenceButton )
    {
      uint i = index;
      for ( pCurrentItem = index + pDialogueWindow->pStartingPosActiveItem;
          (signed int)pCurrentItem < pDialogueWindow->pNumPresenceButton + pDialogueWindow->pStartingPosActiveItem; ++pCurrentItem )
      {
        pButton = pDialogueWindow->GetControl(pCurrentItem);
        pButton->uY = v64 + all_text_height;
        pTextHeight = pFontArrus->CalcTextHeight(pShopOptions[1 * i], &tample_window, 0, 0);
        pButton->uHeight = pTextHeight;
        pButton->uW = pButton->uY + pTextHeight - 1;
        all_text_height = pButton->uW;
        pTextColor = Color16(0xFFu, 0xFFu, 0x9Bu);
        if ( pDialogueWindow->pCurrentPosActiveItem != index + 2 )
          pTextColor = Color16(0xFFu, 0xFFu, 0xFFu);
        tample_window.DrawTitleText(pFontArrus, 0, pButton->uY, pTextColor, pShopOptions[1 * i], 3);
        i++;
        index++;
      }
    }
    return;
  }
  //-------------------------------------------------
  if ( dialog_menu_id == HOUSE_DIALOGUE_TEMPLE_HEAL )
  {
    if ( !pPlayers[uActiveCharacter]->IsPlayerHealableByTemple() )
      return;
    if ( pParty->uNumGold < pPrice )
    {
      ShowStatusBarString(pGlobalTXT_LocalizationStrings[155], 2);//"� ��� �� ������� ������"
      PlayHouseSound((unsigned int)window_SpeakInHouse->ptr_1C, HouseSound_NotEnoughMoney_TrainingSuccessful);
      pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, 0);
      return;
    }
    Party::TakeGold(pPrice);
    v35 = LODWORD(pPlayers[uActiveCharacter]->pConditions[Condition_Zombie]);
    memset(pPlayers[uActiveCharacter], 0, 0xA0u);
    pPlayers[uActiveCharacter]->sHealth = pPlayers[uActiveCharacter]->GetMaxHealth();
    pPlayers[uActiveCharacter]->sMana = pPlayers[uActiveCharacter]->GetMaxMana();
    if ( (signed int)window_SpeakInHouse->ptr_1C != 78 && ((signed int)window_SpeakInHouse->ptr_1C <= 80 || (signed int)window_SpeakInHouse->ptr_1C > 82) )
    {
      if ( (unsigned int)pPlayers[uActiveCharacter]->pConditions[Condition_Zombie] | v35 )// ���� ��������� �����
      {
        pPlayers[uActiveCharacter]->uCurrentFace = pPlayers[uActiveCharacter]->uPrevFace;
        pPlayers[uActiveCharacter]->uVoiceID = pPlayers[uActiveCharacter]->uPrevVoiceID;
        ReloadPlayerPortraits(uActiveCharacter - 1, pPlayers[uActiveCharacter]->uPrevFace);
      }
      pAudioPlayer->PlaySound((SoundID)(SOUND_GoldReceived|0x2), -1, 0, -1, 0, 0, 0, 0);
      pPlayers[uActiveCharacter]->PlaySound(SPEECH_82, 0);
      pOtherOverlayList->_4418B1(20, uActiveCharacter + 99, 0, 65536);
      pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, 0);
      return;
    }
    if ( (unsigned int)pPlayers[uActiveCharacter]->pConditions[Condition_Zombie] | v35 )
      LODWORD(pPlayers[uActiveCharacter]->pConditions[Condition_Zombie]) = v35;
    else
    {
      if ( !pPlayers[uActiveCharacter]->pConditions[Condition_Eradicated]
        && !pPlayers[uActiveCharacter]->pConditions[Condition_Pertified] && !pPlayers[uActiveCharacter]->pConditions[Condition_Dead] )
      {
        pAudioPlayer->PlaySound((SoundID)(SOUND_GoldReceived|0x2), -1, 0, -1, 0, 0, 0, 0);
        pPlayers[uActiveCharacter]->PlaySound(SPEECH_82, 0);
        pOtherOverlayList->_4418B1(20, uActiveCharacter + 99, 0, 65536);
        pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, 0);
        return;
      }
      pPlayers[uActiveCharacter]->uPrevFace = pPlayers[uActiveCharacter]->uCurrentFace;
      pPlayers[uActiveCharacter]->uPrevVoiceID = pPlayers[uActiveCharacter]->uVoiceID;
      pPlayers[uActiveCharacter]->SetCondition(Condition_Zombie, 1);
      pPlayers[uActiveCharacter]->uVoiceID = (pPlayers[uActiveCharacter]->GetSexByVoice() != 0) + 23;
      pPlayers[uActiveCharacter]->uCurrentFace = (pPlayers[uActiveCharacter]->GetSexByVoice() != 0) + 23;
      ReloadPlayerPortraits(uActiveCharacter - 1, (pPlayers[uActiveCharacter]->GetSexByVoice() != 0) + 23);
      pPlayers[uActiveCharacter]->pConditions[Condition_Zombie] = pParty->uTimePlayed;
      //v39 = (GUIWindow *)HIDWORD(pParty->uTimePlayed);
    }
    //HIDWORD(pPlayers[uActiveCharacter]->pConditions[Condition_Zombie]) = (int)v39;
    pPlayers[uActiveCharacter]->pConditions[Condition_Zombie] = pParty->uTimePlayed;
    pAudioPlayer->PlaySound((SoundID)(SOUND_GoldReceived|0x2), -1, 0, -1, 0, 0, 0, 0);
    pPlayers[uActiveCharacter]->PlaySound(SPEECH_82, 0);
    pOtherOverlayList->_4418B1(20, uActiveCharacter + 99, 0, 65536);
    pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, 0);
    return;
  }
  //---------------------------------------------------
  if ( dialog_menu_id == HOUSE_DIALOGUE_TEMPLE_DONATE )
  {
    pPrice = p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].fPriceMultiplier;
    if ( pParty->uNumGold >= (unsigned int)pPrice )
    {
      Party::TakeGold((unsigned int)pPrice);
      v26 = &pOutdoor->ddm;
      if ( uCurrentlyLoadedLevelType != LEVEL_Outdoor )
        v26 = &pIndoor->dlv;
      if ( v26->uReputation > -5 )
      {
        v26->uReputation = v26->uReputation - 1;
        if ( v26->uReputation - 1 < -5 )
          v26->uReputation = -5;
      }
      if ( (unsigned __int8)byte_F8B1EF[uActiveCharacter] == pParty->uDaysPlayed % 7 )
      {
        if ( v26->uReputation <= -5 )
        {
          v30 = pParty->uDaysPlayed % 7 + 1;
          LOBYTE(v30) = v30 | 0x80;
          _42777D_CastSpell_UseWand_ShootArrow(SPELL_AIR_WIZARD_EYE, uActiveCharacter - 1, v30, 48, 0);
        }
        if ( v26->uReputation <= -10 )
        {
          v30 = pParty->uDaysPlayed % 7 + 1;
          LOBYTE(v30) = v30 | 0x80;
          _42777D_CastSpell_UseWand_ShootArrow(SPELL_SPIRIT_PRESERVATION, uActiveCharacter - 1, v30, 48, 0);
        }
        if ( v26->uReputation <= -15 )
        {
          v30 = pParty->uDaysPlayed % 7 + 1;
          LOBYTE(v30) = v30 | 0x80;
          _42777D_CastSpell_UseWand_ShootArrow(SPELL_BODY_PROTECTION_FROM_MAGIC, uActiveCharacter - 1, v30, 48, 0);
        }
        if ( v26->uReputation <= -20 )
        {
          v30 = pParty->uDaysPlayed % 7 + 1;
          LOBYTE(v30) = v30 | 0x80;
          _42777D_CastSpell_UseWand_ShootArrow(SPELL_LIGHT_HOUR_OF_POWER, uActiveCharacter - 1, v30, 48, 0);
        }
        if ( v26->uReputation <= -25 )
        {
          v30 = pParty->uDaysPlayed % 7 + 1;
          LOBYTE(v30) = v30 | 0x80;
          _42777D_CastSpell_UseWand_ShootArrow(SPELL_LIGHT_DAY_OF_PROTECTION, uActiveCharacter - 1, v30, 48, 0);
        }
      }
      ++byte_F8B1EF[uActiveCharacter];
      pPlayers[uActiveCharacter]->PlaySound(SPEECH_83, 0);
      ShowStatusBarString(pGlobalTXT_LocalizationStrings[527], 2); // "Thank You!"
      pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, 0);
      return;
    }
    ShowStatusBarString(pGlobalTXT_LocalizationStrings[155], 2);//"� ��� �� ������� ������"
    PlayHouseSound((unsigned int)window_SpeakInHouse->ptr_1C, HouseSound_NotEnoughMoney_TrainingSuccessful);
    pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, 0);
    return;
  }
  //------------------------------------------------
  if ( dialog_menu_id == HOUSE_DIALOGUE_LEARN_SKILLS )
  {
    if ( HouseUI_CheckIfPlayerCanInteract() )
    {
      all_text_height = 0;
      pCurrentItem = (signed __int64)(p2DEvents[(signed int)window_SpeakInHouse->ptr_1C - 1].flt_24 * 500.0);
      v64 = (signed int)(pCurrentItem * (100 - pPlayers[uActiveCharacter]->GetMerchant())) / 100;
      if ( v64 < (signed int)pCurrentItem / 3 )
        v64 = (signed int)pCurrentItem / 3;
      pCurrentItem = 0;
      for ( int i = pDialogueWindow->pStartingPosActiveItem; i < pDialogueWindow->pNumPresenceButton + pDialogueWindow->pStartingPosActiveItem; ++i )
      {
        if ( byte_4ED970_skill_learn_ability_by_class_table[pPlayers[uActiveCharacter]->classType][pDialogueWindow->GetControl(i)->msg_param - 36]
         && !pPlayers[uActiveCharacter]->pActiveSkills[pDialogueWindow->GetControl(i)->msg_param - 36] )
        {
          all_text_height += pFontArrus->CalcTextHeight(pSkillNames[pDialogueWindow->GetControl(i)->msg_param - 36], &tample_window, 0, 0);
          ++pCurrentItem;
        }
      }
      if ( pCurrentItem )
      {
        sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[401], v64);
        tample_window.DrawTitleText(pFontArrus, 0, 0x92u, 0, pTmpBuf.data(), 3);
        v64 = (149 - (signed int)all_text_height) / (signed int)pCurrentItem;
        if ( v64 > 32 )
          v64 = 32;
        all_text_height = (signed int)(149 - pCurrentItem * v64 - (int)all_text_height) / 2 - v64 / 2 + 162;
        if ( pDialogueWindow->pStartingPosActiveItem < pDialogueWindow->pStartingPosActiveItem + pDialogueWindow->pNumPresenceButton )
        {
          pCurrentItem = 2;
          for ( int i = pDialogueWindow->pStartingPosActiveItem;
                    i < pDialogueWindow->pNumPresenceButton + pDialogueWindow->pStartingPosActiveItem; ++i )
          {
            pButton = pDialogueWindow->GetControl(i);
            if ( !byte_4ED970_skill_learn_ability_by_class_table[pPlayers[uActiveCharacter]->classType][pButton->msg_param - 36]
              || pPlayers[uActiveCharacter]->pActiveSkills[pButton->msg_param - 36] )
            {
              pButton->uW = 0;
              pButton->uHeight = 0;
              pButton->uY = 0;
            }
            else
            {
              pButton->uY = v64 + all_text_height;
              pTextHeight = pFontArrus->CalcTextHeight(pSkillNames[pButton->msg_param - 36], &tample_window, 0, 0);
              pButton->uHeight = pTextHeight;
              pButton->uW = pButton->uY + pTextHeight - 1;
              all_text_height = pButton->uW;
              pTextColor = Color16(0xFFu, 0xFFu, 0x9Bu);
              if ( pDialogueWindow->pCurrentPosActiveItem != pCurrentItem )
                pTextColor = Color16(0xFFu, 0xFFu, 0xFFu);
              tample_window.DrawTitleText(pFontArrus, 0, pButton->uY, pTextColor, pSkillNames[pButton->msg_param - 36], 3);
            }
            pCurrentItem++;
          }
        }
      }
      else
      {
        sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[544], pPlayers[uActiveCharacter]->pName, pClassNames[pPlayers[uActiveCharacter]->classType]);//"������� ��� %s %s �������� ������ ��� ���-������"
        strcat(pTmpBuf.data(), "\n \n");
        strcat(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[528]);//"������ ������ �� ���� ����������."
        pTextHeight = pFontArrus->CalcTextHeight(pTmpBuf.data(), &tample_window, 0, 0);
        tample_window.DrawTitleText(pFontArrus, 0, (174 - pTextHeight) / 2 + 138, Color16(0xFFu, 0xFFu, 0x9Bu), pTmpBuf.data(), 3);
      }
    }
  }
  return;
}

//----- (004B4710) --------------------------------------------------------
void TrainingDialog()
{
  unsigned __int64 v5; // edi@3
  int v8; // edx@4
  double v9; // st7@6
  signed int v10; // esi@6
  int pPrice; // ecx@6
  signed int v14; // esi@14
//  int v16; // eax@16
  int v19; // ecx@24
  int v33; // eax@36
  unsigned int v36; // eax@38
  unsigned int v42; // eax@46
  int index;
  int all_text_height; // eax@68
  int v49; // ebx@69
  GUIButton *pButton; // eax@71
  int pTextHeight; // eax@71
  unsigned __int16 pTextColor; // ax@71
  GUIWindow training_dialog_window; // [sp+Ch] [bp-78h]@1
  int v69; // [sp+70h] [bp-14h]@6
  int v73; // [sp+80h] [bp-4h]@14

  memcpy(&training_dialog_window, window_SpeakInHouse, sizeof(training_dialog_window));
  training_dialog_window.uFrameX = 483;
  training_dialog_window.uFrameWidth = 148;
  training_dialog_window.uFrameZ = 334;
  v5 = 1000ui64 * pPlayers[uActiveCharacter]->uLevel * (pPlayers[uActiveCharacter]->uLevel + 1) / 2;  // E n = n(n + 1) / 2
  //v68 = pMaxLevelPerTrainingHallType[(unsigned int)window_SpeakInHouse->ptr_1C - HOUSE_TRAINING_HALL_EMERALD_ISLE];
  if (pPlayers[uActiveCharacter]->uExperience >= v5)
  {
    v8 = pPlayers[uActiveCharacter]->classType % 4 + 1;
    if ( v8 == 4 )
      v8 = 3;
    v9 = (double)pPlayers[uActiveCharacter]->uLevel;
    v69 = v8;
    v10 = (signed __int64)(v9 * p2DEvents[(signed int)window_SpeakInHouse->ptr_1C - 1].fPriceMultiplier * (double)v8);
    pPrice = v10 * (100 - pPlayers[uActiveCharacter]->GetMerchant()) / 100;
    if ( pPrice < v10 / 3 )
      pPrice = v10 / 3;
  }
//-------------------------------------------------------
  all_text_height = 0;
  if (HouseUI_CheckIfPlayerCanInteract())
  {
    if ( dialog_menu_id == HOUSE_DIALOGUE_MAIN )
    {
      if (HouseUI_CheckIfPlayerCanInteract())
      {
        index = 0;
        pShopOptions[0] = pTmpBuf.data();
        pShopOptions[1] = const_cast<char *>(pGlobalTXT_LocalizationStrings[160]);// "Learn Skills"
        if ( pDialogueWindow->pStartingPosActiveItem < pDialogueWindow->pStartingPosActiveItem + pDialogueWindow->pNumPresenceButton )
        {
          for ( int i = pDialogueWindow->pStartingPosActiveItem;
                i < pDialogueWindow->pNumPresenceButton + pDialogueWindow->pStartingPosActiveItem; ++i )
          {
            if ( pDialogueWindow->GetControl(i)->msg_param == HOUSE_DIALOGUE_TRAININGHALL_TRAIN )
            {
              if (pPlayers[uActiveCharacter]->uLevel >= pMaxLevelPerTrainingHallType[(unsigned int)window_SpeakInHouse->ptr_1C - HOUSE_TRAINING_HALL_EMERALD_ISLE])
                sprintfex(pShopOptions[index], "%s\n \n%s", pGlobalTXT_LocalizationStrings[536], pGlobalTXT_LocalizationStrings[529]); //"With your skills, you should be working here as a teacher."    "Sorry, but we are unable to train you."
              else
              {
                if (pPlayers[uActiveCharacter]->uExperience < v5)
                  sprintfex(pShopOptions[index], pGlobalTXT_LocalizationStrings[538], (uint)(v5 - pPlayers[uActiveCharacter]->uExperience), pPlayers[uActiveCharacter]->uLevel + 1); // "You need %d more experience to train to level %d"
                else
                  sprintfex(pShopOptions[index], pGlobalTXT_LocalizationStrings[537], pPlayers[uActiveCharacter]->uLevel + 1, pPrice); // "Train to level %d for %d gold"
              }
            }
            all_text_height += pFontArrus->CalcTextHeight(pShopOptions[index], &training_dialog_window, 0, 0);
            ++index;
          }
        }
        v49 = (2 * (87 - (174 - all_text_height) / 2) - all_text_height) / 2 - (174 - all_text_height) / 2 / 2 + 138;
        if (pDialogueWindow->pStartingPosActiveItem < pDialogueWindow->pStartingPosActiveItem + pDialogueWindow->pNumPresenceButton)
        {
          index = 0;
          for ( int i = pDialogueWindow->pStartingPosActiveItem;
                i < pDialogueWindow->pStartingPosActiveItem + pDialogueWindow->pNumPresenceButton; ++i )
          {
            pButton = pDialogueWindow->GetControl(i);
            pButton->uY = (174 - all_text_height) / 2 + v49;
            pTextHeight = pFontArrus->CalcTextHeight(pShopOptions[index], &training_dialog_window, 0, 0);
            pButton->uHeight = pTextHeight;
            pButton->uW = pTextHeight + pButton->uY - 1;
            v49 = pButton->uW;
            pTextColor = Color16(0xE1u, 0xCDu, 0x23u);
            if ( pDialogueWindow->pCurrentPosActiveItem != i )
              pTextColor = Color16(255, 255, 255);
            training_dialog_window.DrawTitleText(pFontArrus, 0, pButton->uY, pTextColor, pShopOptions[index], 3);
            ++index;
          }
        }
      }
    }
//------------------------------------------------------------------
    if ( dialog_menu_id == HOUSE_DIALOGUE_TRAININGHALL_TRAIN )
    {
      if ( !HouseUI_CheckIfPlayerCanInteract() )
      {
        v33 = pFontArrus->CalcTextHeight(pNPCTopics[122].pText, &training_dialog_window, 0, 0);
        training_dialog_window.DrawTitleText(pFontArrus, 0, (212 - v33) / 2 + 101, Color16(0xE1u, 0xCDu, 0x23u), pNPCTopics[122].pText, 3);
        pDialogueWindow->pNumPresenceButton = 0;
        return;
      }
      if ( pPlayers[uActiveCharacter]->uLevel < pMaxLevelPerTrainingHallType[(unsigned int)window_SpeakInHouse->ptr_1C - HOUSE_TRAINING_HALL_EMERALD_ISLE] )
      {
        if ( (signed __int64)pPlayers[uActiveCharacter]->uExperience >= v5 )
        {
          if ( pParty->uNumGold >= pPrice)
          {
            Party::TakeGold(pPrice);
            PlayHouseSound((unsigned int)window_SpeakInHouse->ptr_1C, HouseSound_NotEnoughMoney_TrainingSuccessful);
            ++pPlayers[uActiveCharacter]->uLevel;
            pPlayers[uActiveCharacter]->uSkillPoints += pPlayers[uActiveCharacter]->uLevel / 10 + 5;
            pPlayers[uActiveCharacter]->sHealth = pPlayers[uActiveCharacter]->GetMaxHealth();
            pPlayers[uActiveCharacter]->sMana = pPlayers[uActiveCharacter]->GetMaxMana();
            uint max_level_in_party = player_levels[0];
            for (uint _it = 1; _it < 4; ++_it)
            {
              if (player_levels[_it] > max_level_in_party)
                max_level_in_party = player_levels[_it];
            }
            ++player_levels[uActiveCharacter - 1];
            if (player_levels[uActiveCharacter - 1] > max_level_in_party) // if we reach new maximum party level
                                                                          // feature is broken thou, since this array is always zeroed in EnterHouse
            {
              v42 = 60 * (_494820_training_time(pParty->uCurrentHour) + 4) - pParty->uCurrentMinute;
              if ((unsigned int)window_SpeakInHouse->ptr_1C == HOUSE_TRAINING_HALL_94 ||
                  (unsigned int)window_SpeakInHouse->ptr_1C == HOUSE_TRAINING_HALL_95)
                v42 += 720;
              RestAndHeal((signed int)(v42 + 10080));
              if ( uCurrentlyLoadedLevelType == LEVEL_Outdoor )
                pOutdoor->SetFog();
            }
            pPlayers[uActiveCharacter]->PlaySound(SPEECH_87, 0);
            sprintfex(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[430], pPlayers[uActiveCharacter]->pName, pPlayers[uActiveCharacter]->uLevel, pPlayers[uActiveCharacter]->uLevel / 10 + 5);// 
                                                // "%s is now Level %lu and has earned %lu Skill Points!"
            ShowStatusBarString(pTmpBuf.data(), 2);
            pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, 0);
            return;
          }
          ShowStatusBarString(pGlobalTXT_LocalizationStrings[155], 2);// "You don't have enough gold"
          PlayHouseSound((unsigned int)window_SpeakInHouse->ptr_1C, (HouseSoundID)4);
          pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, 0);
          return;
        }
        sprintfex(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[538], (unsigned int)(v5 - pPlayers[uActiveCharacter]->uExperience), pPlayers[uActiveCharacter]->uLevel + 1);// 
                                                // "You need %d more experience to train to level %d"
        v36 = (212 - pFontArrus->CalcTextHeight(pTmpBuf.data(), &training_dialog_window, 0, 0)) / 2 + 88;
      }
      else
      {
        sprintf(pTmpBuf.data(), "%s\n \n%s", pGlobalTXT_LocalizationStrings[536], pGlobalTXT_LocalizationStrings[529]);// 
                                                // ""With your skills, you should be working here as a teacher.""
                                                // ""Sorry, but we are unable to train you.""
        v36 = (212 - pFontArrus->CalcTextHeight(pTmpBuf.data(), &training_dialog_window, 0, 0)) / 2 + 101;
      }
      training_dialog_window.DrawTitleText(pFontArrus, 0, v36, Color16(0xE1u, 0xCDu, 0x23u), pTmpBuf.data(), 3);
      PlayHouseSound((unsigned int)window_SpeakInHouse->ptr_1C, (HouseSoundID)3);
      pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, 0);
      return;
    }
  }
//-------------------------------------------------------------
  if ( dialog_menu_id == HOUSE_DIALOGUE_LEARN_SKILLS )
  {
    if (HouseUI_CheckIfPlayerCanInteract())
    {
      v14 = (signed __int64)(p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].flt_24 * 500.0);
      pPrice = v14 * (100 - pPlayers[uActiveCharacter]->GetMerchant()) / 100;
      if ( pPrice < v14 / 3 )
        pPrice = v14 / 3;
      index = 0;
      for ( int i = pDialogueWindow->pStartingPosActiveItem;
          (signed int)i < pDialogueWindow->pNumPresenceButton + pDialogueWindow->pStartingPosActiveItem; ++i )
      {
        if ( byte_4ED970_skill_learn_ability_by_class_table[pPlayers[uActiveCharacter]->classType][pDialogueWindow->GetControl(i)->msg_param - 36]
             && !pPlayers[uActiveCharacter]->pActiveSkills[pDialogueWindow->GetControl(i)->msg_param - 36] )
        {
          all_text_height += pFontArrus->CalcTextHeight(pSkillNames[pDialogueWindow->GetControl(i)->msg_param - 36], &training_dialog_window, 0, 0);
          ++index;
        }
      }
      if ( index )
      {
        sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[401], pPrice);// "Skill Cost: %lu"
        training_dialog_window.DrawTitleText(pFontArrus, 0, 0x92u, 0, pTmpBuf.data(), 3);
        v73 = (signed int)(149 - all_text_height) / index;
        if ( v73 > 32 )
          v73 = 32;
        v19 = (signed int)(149 - index * v73 - all_text_height) / 2 - v73 / 2 + 162;
        for ( int i = pDialogueWindow->pStartingPosActiveItem;
             (signed int)i < pDialogueWindow->pStartingPosActiveItem + pDialogueWindow->pNumPresenceButton; ++i )
        {
          pButton = pDialogueWindow->GetControl(i);
          if ( !byte_4ED970_skill_learn_ability_by_class_table[pPlayers[uActiveCharacter]->classType][pButton->msg_param - 36]
              || pPlayers[uActiveCharacter]->pActiveSkills[pButton->msg_param - 36] )
          {
            pButton->uW = 0;
            pButton->uHeight = 0;
            pButton->uY = 0;
          }
          else
          {
            pButton->uY = v73 + v19;
            pTextHeight = pFontArrus->CalcTextHeight(pSkillNames[pButton->msg_param - 36], &training_dialog_window, 0, 0);
            pButton->uHeight = pTextHeight;
            pButton->uW = pButton->uY + pTextHeight - 1;
            v19 = pButton->uY + pTextHeight - 1;
            pTextColor = Color16(0xE1u, 0xCDu, 0x23u);
            if ( pDialogueWindow->pCurrentPosActiveItem != i )
              pTextColor = Color16(255, 255, 255);
            training_dialog_window.DrawTitleText(pFontArrus, 0, pButton->uY, pTextColor, pSkillNames[pButton->msg_param - 36], 3);
          }
        }
      }
      else
      {
        sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[544], pPlayers[uActiveCharacter]->pName, pClassNames[pPlayers[uActiveCharacter]->classType]);// 
                                              // "Seek knowledge elsewhere %s the %s"
        strcat(pTmpBuf.data(), "\n \n");
        strcat(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[528]);// "I can offer you nothing further."
        pTextHeight = pFontArrus->CalcTextHeight(pTmpBuf.data(), &training_dialog_window, 0, 0);
        training_dialog_window.DrawTitleText(pFontArrus, 0, (174 - pTextHeight) / 2 + 138, Color16(0xE1u, 0xCDu, 0x23u), pTmpBuf.data(), 3);
      }
    }
  }
  return;
}

//----- (004B6478) --------------------------------------------------------
void sub_4B6478()
{
  unsigned int v2; // eax@1
  signed int v3; // esi@1
  int pPrice; // ebx@1
  unsigned int v5; // esi@5
  int v6; // edi@6
//  int result; // eax@13
  int all_text_height; // eax@20
//  int v13; // eax@21
  GUIButton *pButton; // esi@27
//  unsigned int v16; // eax@28
  int pTextHeight; // eax@29
  unsigned __int16 pTextColor; // ax@29
//  unsigned __int16 v22; // ST14_2@36
  int v27; // [sp-4h] [bp-80h]@8
  GUIWindow dialog_window; // [sp+Ch] [bp-70h]@1
  int v32; // [sp+6Ch] [bp-10h]@1
  int index; // [sp+74h] [bp-8h]@17

  memcpy(&dialog_window, window_SpeakInHouse, sizeof(dialog_window));
  dialog_window.uFrameX = 483;
  dialog_window.uFrameWidth = 148;
  dialog_window.uFrameZ = 334;
  v2 = 52 * (unsigned int)window_SpeakInHouse->ptr_1C;
  v32 = (unsigned __int8)(((p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].uType != 18) - 1) & 0x96) + 100;
  v3 = (signed __int64)((double)v32 * p2DEvents[(unsigned int)window_SpeakInHouse->ptr_1C - 1].fPriceMultiplier);
  pPrice = v3 * (100 - pPlayers[uActiveCharacter]->GetMerchant()) / 100;
  if ( pPrice < v3 / 3 )
    pPrice = v3 / 3;
  if ( dialog_menu_id == HOUSE_DIALOGUE_MAIN)
  {
    if ( !(unsigned __int16)_449B57_test_bit((unsigned __int8 *)pPlayers[uActiveCharacter]->_achieved_awards_bits, word_4F0754[2 * (unsigned int)window_SpeakInHouse->ptr_1C]) )
    {
      pTextHeight = pFontArrus->CalcTextHeight(pNPCTopics[171].pText, &dialog_window, 0, 0);//
      dialog_window.DrawTitleText(pFontArrus, 0, (212 - pTextHeight) / 2 + 101, Color16(0xFFu, 0xFFu, 0x9Bu), pNPCTopics[171].pText, 3);
      pDialogueWindow->pNumPresenceButton = 0;
      return;
    }
    if ( !HouseUI_CheckIfPlayerCanInteract() )
      return;
    all_text_height = 0;
    index = 0;
    for ( int i = pDialogueWindow->pStartingPosActiveItem;
          i < pDialogueWindow->pNumPresenceButton + pDialogueWindow->pStartingPosActiveItem; ++i )
    {
      if ( byte_4ED970_skill_learn_ability_by_class_table[pPlayers[uActiveCharacter]->classType / 3][pDialogueWindow->GetControl(i)->msg_param - 36]
         && !pPlayers[uActiveCharacter]->pActiveSkills[pDialogueWindow->GetControl(i)->msg_param - 36] )
      {
        all_text_height += pFontArrus->CalcTextHeight(pSkillNames[pDialogueWindow->GetControl(i)->msg_param - 36], &dialog_window, 0, 0);
        ++index;
      }
    }
    if ( !index )
    {
      sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[544], pPlayers[uActiveCharacter]->pName, pClassNames[pPlayers[uActiveCharacter]->classType]);
      strcat(pTmpBuf.data(), "\n \n");
      strcat(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[528]);//������ ������ �� ���� ����������.
      pTextHeight = pFontArrus->CalcTextHeight(pTmpBuf.data(), &dialog_window, 0, 0);
      dialog_window.DrawTitleText(pFontArrus, 0, (174 - pTextHeight) / 2 + 138, Color16(0xFFu, 0xFFu, 0x9Bu), pTmpBuf.data(), 3);
      return; 
    }
    sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[401], pPrice);//��������� ������: %lu
    dialog_window.DrawTitleText(pFontArrus, 0, 0x92u, 0, pTmpBuf.data(), 3);
    v32 = (149 - all_text_height) / index;
    if ( v32 > 32 )
      v32 = 32;
    index = (149 - index * v32 - all_text_height) / 2 - v32 / 2 + 162;
    if ( pDialogueWindow->pStartingPosActiveItem < pDialogueWindow->pStartingPosActiveItem + pDialogueWindow->pNumPresenceButton )
    {
      for ( int i = pDialogueWindow->pStartingPosActiveItem;
            i <pDialogueWindow->pNumPresenceButton + pDialogueWindow->pStartingPosActiveItem; ++i )
      {
        pButton = pDialogueWindow->GetControl(i);
        if ( byte_4ED970_skill_learn_ability_by_class_table[pPlayers[uActiveCharacter]->classType / 3][pButton->msg_param - 36] )
        {
          if ( !pPlayers[uActiveCharacter]->pActiveSkills[pButton->msg_param - 36] )
          {
            pButton->uY = v32 + index;
            pTextHeight = pFontArrus->CalcTextHeight(pSkillNames[pButton->msg_param - 36], &dialog_window, 0, 0);
            pButton->uHeight = pTextHeight;
            pButton->uW = pButton->uY + pTextHeight - 1;
            index = pButton->uY + pTextHeight - 1;
            pTextColor = Color16(0xFFu, 0xFFu, 0x9Bu);
            if ( pDialogueWindow->pCurrentPosActiveItem != i )
              pTextColor = Color16(0xFFu, 0xFFu, 0xFFu);
            dialog_window.DrawTitleText(pFontArrus, 0, pButton->uY, pTextColor, pSkillNames[pButton->msg_param - 36], 3);
          }
        }
        else
        {
          pButton->uW = 0;
          pButton->uHeight = 0;
          pButton->uY = 0;
        }
      }
    }
  return;
  }

  if ( HouseUI_CheckIfPlayerCanInteract() )
  {
    v5 = 0;
    __debugbreak();
    //pSkillAvailabilityPerClass[8 + v58->uClass][4 + v23]
    // or
    //byte_4ED970_skill_learn_ability_by_class_table[v58->uClass][v23 - 36]
    // or
    //byte_4ED970_skill_learn_ability_by_class_table[v58->uClass - 1][v23 + 1]
    __debugbreak(); // whacky condition - fix
    if (false
    //if ( !*(&byte_4ED94C[37 * v1->uClass / 3] + dword_F8B19C)
      || (v6 = (int)(&pPlayers[uActiveCharacter]->uIntelligence + dialog_menu_id), *(short *)v6) )
    {
      pAudioPlayer->PlaySound(SOUND_error, 0, 0, -1, 0, 0, 0, 0);
    }
    else
    {
      if ( pParty->uNumGold < pPrice )
      {
        ShowStatusBarString(pGlobalTXT_LocalizationStrings[155], 2);//� ��� �� ������� ������
        v27 = 4;
      }
      else
      {
        Party::TakeGold(pPrice);
        *(short *)v6 = 1;
         v27 = 2;
      }
      PlayHouseSound((unsigned int)window_SpeakInHouse->ptr_1C, (HouseSoundID)v27);
    }
  }
  else
  {
    v5 = 0;
  }
  pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, v5);
  return; // void func
}

//----- (004B2A74) --------------------------------------------------------
void SimpleHouseDialog()
{
  NPCData *pNPC; // esi@6
  char *v15; // esi@14
  GUIButton *pButton; // eax@15
  char *v29; // esi@42
  int v31; // ST1C_4@42
  int index; // esi@51
  int v36;
  signed int all_text_height; // ecx@54
  int v40; // edi@57
  unsigned __int16 pTextColor; // ax@60
  GUIFont *pTextFont; // ebx@64
  int pTextHeight;
  GUIWindow w; // [sp+Ch] [bp-110h]@64
  GUIWindow right_panel_window; // [sp+60h] [bp-BCh]@13
  GUIWindow house_window; // [sp+B4h] [bp-68h]@1
  char *pInString; // [sp+114h] [bp-8h]@12

  memcpy(&house_window, pDialogueWindow, sizeof(house_window));
  if ( pDialogueNPCCount == uNumDialogueNPCPortraits && uHouse_ExitPic )
  {
    house_window.uFrameX = 493;
    house_window.uFrameWidth = 126;
    house_window.uFrameZ = 366;
    house_window.DrawTitleText(pFontCreate, 0, 2, 0, pMapStats->pInfos[uHouse_ExitPic].pName, 3);
    house_window.uFrameX = 483;
    house_window.uFrameWidth = 148;
    house_window.uFrameZ = 334;
    if ( !pTransitionStrings[uHouse_ExitPic] )
    {
      sprintfex(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[411], pMapStats->pInfos[uHouse_ExitPic].pName);//����� � ^Pv[%s]
      house_window.DrawTitleText(pFontCreate, 0, (212 - pFontCreate->CalcTextHeight(pTmpBuf.data(), &house_window, 0, 0)) / 2 + 101, 0, pTmpBuf.data(), 3);
      return;
    }
    house_window.DrawTitleText(pFontCreate, 0,
      (212 - pFontCreate->CalcTextHeight(pTransitionStrings[uHouse_ExitPic], &house_window, 0, 0)) / 2 + 101, 0, pTransitionStrings[uHouse_ExitPic], 3);
    return;
  }
  house_window.uFrameWidth -= 10;
  house_window.uFrameZ -= 10;
  pNPC = HouseNPCData[(unsigned int)((char *)pDialogueNPCCount + -(dword_591080 != 0) )];//- 1
  if ( pNPC->uProfession )
    sprintfex(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[429], pNPC->pName, aNPCProfessionNames[pNPC->uProfession]);//^Pi[%s] %s
  else
    strcpy(pTmpBuf.data(), pNPC->pName);
  house_window.DrawTitleText(pFontCreate, 483, 113, Color16(0x15u, 0x99u, 0xE9u), pTmpBuf.data(), 3);
  if ( !dword_591080 )
  {
    if ( !uDialogueType )
    {
      if ( pNPC->greet )
      {
        house_window.uFrameWidth = game_viewport_width;
        house_window.uFrameZ = 452;
        pInString = pNPCStats->pNPCGreetings[pNPC->greet].pGreetings[((pNPC->uFlags & 3) == 2)];
        //pInString = (char *)*(&pNPCStats->field_17884 + ((pNPC->uFlags & 3) == 2) + 2 * pNPC->greet);
        pRenderer->GetLeather(8, 352 - (pFontArrus->CalcTextHeight(pInString, &house_window, 13, 0) + 7),
           pIcons_LOD->GetTexture(uTextureID_Leather), pIcons_LOD->GetTexture(uTextureID_Leather)->uTextureHeight
           - (pFontArrus->CalcTextHeight(pInString, &house_window, 13, 0) + 7));
        pRenderer->DrawTextureIndexed(8, 347 - (pFontArrus->CalcTextHeight(pInString, &house_window, 13, 0) + 7), pTexture_591428);
        pDialogueWindow->DrawText(pFontArrus, 13, 354 - (pFontArrus->CalcTextHeight(pInString, &house_window, 13, 0) + 7),
                                             0, FitTextInAWindow(pInString, pFontArrus, &house_window, 0xDu, 0), 0, 0, 0);
      }
    }
  }
  //for right panel
  memcpy(&right_panel_window, pDialogueWindow, sizeof(right_panel_window));
  right_panel_window.uFrameX = 483;
  right_panel_window.uFrameWidth = 148;
  right_panel_window.uFrameZ = 334;
  for ( int i = right_panel_window.pStartingPosActiveItem;
            i < right_panel_window.pStartingPosActiveItem + right_panel_window.pNumPresenceButton; ++i )
  {
    pButton = right_panel_window.GetControl(i);
    switch ( pButton->msg_param )
    {
      case 19://evt_A
        v15 = (char *)pNPCTopics[pNPC->evt_A].pTopic;
        if ( !v15 )
        {
          pButton->msg_param = 0;
          v15 = "";
        }
        strcpy(pButton->pButtonName, v15);
        continue;
      case 20://evt_B
        v15 = (char *)pNPCTopics[pNPC->evt_B].pTopic;
        if ( !v15 )
        {
          pButton->msg_param = 0;
          v15 = "";
        }
        if ( uDialogueType != 84 )
        {
          strcpy(pButton->pButtonName, v15);
          continue;
        }
        sprintf(pTmpBuf.data(), format_4E2D80, Color16(0xE1u, 0xCDu, 0x23u), pItemsTable->pItems[contract_approved].pUnidentifiedName);
        sprintf(pTmpBuf2.data(), current_npc_text, pTmpBuf.data());
        current_npc_text = pTmpBuf2.data();
        strcpy(pButton->pButtonName, v15);
        continue;
      case 21://evt_C
        v15 = (char *)pNPCTopics[pNPC->evt_C].pTopic;
        if ( !v15 )
        {
          pButton->msg_param = 0;
          v15 = "";
        }
        strcpy(pButton->pButtonName, v15);
        continue;
      case 22://evt_D
        v15 = (char *)pNPCTopics[pNPC->evt_D].pTopic;
        if ( !v15 )
        {
          pButton->msg_param = 0;
          v15 = "";
        }
        strcpy(pButton->pButtonName, v15);
        continue;
      case 23://evt_E
        v15 = (char *)pNPCTopics[pNPC->evt_E].pTopic;
        if ( !v15 )
        {
          pButton->msg_param = 0;
          v15 = "";
        }
        strcpy(pButton->pButtonName, v15);
        continue;
      case 24://evt_F
        v15 = (char *)pNPCTopics[pNPC->evt_F].pTopic;
        if ( !v15 )
        {
          pButton->msg_param = 0;
          v15 = "";
        }
        strcpy(pButton->pButtonName, v15);
        continue;
      case 76:
        strcpy(pButton->pButtonName, pGlobalTXT_LocalizationStrings[406]);//������
        continue;
      case 77:
        strcpy(pButton->pButtonName, pGlobalTXT_LocalizationStrings[407]);//���������
        continue;
      case 79:
        strcpy(pButton->pButtonName, _4B254D_SkillMasteryTeacher((int)right_panel_window.ptr_1C));
        continue;
      case 82:
        strcpy(pButton->pButtonName, ContractSelectText((int)right_panel_window.ptr_1C));
        continue;
      case 83:
        v29 = pMonsterStats->pInfos[bountyHunting_monster_id_for_hunting].pName;
        v31 = *(int *)v29;
        sprintfex(pTmpBuf.data(), "\f%05d%s\f%05d", Color16(0xFFu, 0xFFu, 0x9Bu), v31, Color16(0xFFu, 0xFFu, 0xFFu));
        sprintfex(pTmpBuf2.data(), bountyHunting_text, pTmpBuf.data(), 100 * (unsigned __int8)v29[8]);
        current_npc_text = pTmpBuf2.data();
        strcpy(pButton->pButtonName, "");
        continue;
    }
    if ( pButton->msg_param > 0 && pButton->msg_param < 13 )
    {
      strcpy(pButton->pButtonName, pGlobalTXT_LocalizationStrings[122]);//��������
      continue;
    }
    if ( pButton->msg_param > 13 && pButton->msg_param < 19 )
    {
      strcpy(pButton->pButtonName, "");
      continue;
    }
    if ( pButton->msg_param != 93 )
    {
      strcpy(pButton->pButtonName, "");
      continue;
    }
  }
  index = 0;
  all_text_height = 0;
  for ( int i = pDialogueWindow->pStartingPosActiveItem;
        i < pDialogueWindow->pStartingPosActiveItem + pDialogueWindow->pNumPresenceButton; ++i )
  {
    pButton = pDialogueWindow->GetControl(i);
    all_text_height += pFontArrus->CalcTextHeight(pButton->pButtonName, &right_panel_window, 0, 0);
    index++;
  }
  if ( index )
  {
    v36 = (174 - all_text_height) / index;
    if ( v36 > 32 )
      v36 = 32;
    v40 = (174 - v36 * index - all_text_height) / 2 - v36 / 2 + 138;
    for ( int i = pDialogueWindow->pStartingPosActiveItem; i < pDialogueWindow->pNumPresenceButton + pDialogueWindow->pStartingPosActiveItem; ++i )
    {
      pButton = pDialogueWindow->GetControl(i);
      pButton->uY = v36 + v40;
      pTextHeight = pFontArrus->CalcTextHeight(pButton->pButtonName, &right_panel_window, 0, 0);
      pButton->uHeight = pTextHeight;
      v40 = pButton->uY + pTextHeight - 1;
      pButton->uW = v40;
      pTextColor = Color16(0xE1u, 0xCDu, 0x23u);
      if ( pDialogueWindow->pCurrentPosActiveItem != i )
        pTextColor = Color16(0xFFu, 0xFFu, 0xFFu);
      right_panel_window.DrawTitleText(pFontArrus, 0, pButton->uY, pTextColor, pButton->pButtonName, 3);
    }
  }
  if ( current_npc_text )
  {
    w.uFrameWidth = 458;
    w.uFrameZ = 457;
    pTextFont = pFontArrus;
    pTextHeight = pFontArrus->CalcTextHeight(current_npc_text, &w, 13, 0) + 7;
    if ( 352 - pTextHeight < 8 )
    {
      pTextFont = pFontCreate;
      pTextHeight = pFontCreate->CalcTextHeight(current_npc_text, &w, 13, 0) + 7;
    }
    pRenderer->GetLeather(8, 352 - pTextHeight, pIcons_LOD->GetTexture(uTextureID_Leather), pIcons_LOD->GetTexture(uTextureID_Leather)->uTextureHeight - pTextHeight);
    pRenderer->DrawTextureIndexed(8, 347 - pTextHeight, pTexture_591428);
    house_window.DrawText(pTextFont, 13, 354 - pTextHeight, 0, FitTextInAWindow(current_npc_text, pTextFont, &w, 13, 0), 0, 0, 0);
  }
}

//----- (004B4F4F) --------------------------------------------------------
void JailDialog()
{
  GUIWindow jail_dialogue_window; // [sp+8h] [bp-54h]@1

  memcpy(&jail_dialogue_window, window_SpeakInHouse, sizeof(jail_dialogue_window));
  jail_dialogue_window.uFrameX = 483;
  jail_dialogue_window.uFrameWidth = 148;
  jail_dialogue_window.uFrameZ = 334;
  jail_dialogue_window.DrawTitleText(pFontArrus, 0, (310 - pFontArrus->CalcTextHeight(pGlobalTXT_LocalizationStrings[672], &jail_dialogue_window, 0, 0)) / 2 + 18,
     Color16(0xFFu, 0xFFu, 0x9Bu), pGlobalTXT_LocalizationStrings[672], 3);//"�� �������������� ������������ � ��������� �� ���� ����������� � ������ ���� ����������."
}


//----- (00443801) --------------------------------------------------------
void InitializeBuildingResidents()
{

  int i;
  char* test_string;
  unsigned char c;
  bool break_loop;
  unsigned int temp_str_len;
  char* tmp_pos;
  int decode_step;

  free(p2DEventsTXT_Raw);
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

//----- (004BD8B5) --------------------------------------------------------
int HouseDialogPressCloseBtn()
{
  if ( pMessageQueue_50CBD0->uNumMessages )
    pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
  pKeyActionMap->SetWindowInputStatus(WINDOW_INPUT_CANCELLED);
  pKeyActionMap->ResetKeys();
  activeLevelDecoration = nullptr;
  current_npc_text = 0;
  if ( pDialogueNPCCount == 0)
    return 0;

  if ( dialog_menu_id == HOUSE_DIALOGUE_SHOP_BUY_SPECIAL && ShopTexture )
  {
    ShopTexture->Release();
    ShopTexture = 0;
  }

  switch(dialog_menu_id)
  {
    case -1:
      _4B4224_UpdateNPCTopics((int)((char *)pDialogueNPCCount - 1));
      pVideoPlayer->_4BF5B2();
      break;

    case HOUSE_DIALOGUE_SHOP_DISPLAY_EQUIPMENT:
    case HOUSE_DIALOGUE_LEARN_SKILLS:
    case HOUSE_DIALOGUE_TAVERN_ARCOMAGE_MAIN:
      pVideoPlayer->_4BF5B2();
      UI_CreateEndConversationButton();
      dialog_menu_id = HOUSE_DIALOGUE_MAIN;
      InitializaDialogueOptions(in_current_building_type);
      break;

    case HOUSE_DIALOGUE_SHOP_SELL:
    case HOUSE_DIALOGUE_SHOP_IDENTIFY:
    case HOUSE_DIALOGUE_SHOP_REPAIR:
      UI_CreateEndConversationButton();
      dialog_menu_id = HOUSE_DIALOGUE_SHOP_DISPLAY_EQUIPMENT;
      InitializaDialogueOptions_Shops(in_current_building_type);
      break;

    case HOUSE_DIALOGUE_TAVERN_ARCOMAGE_RULES:
    case HOUSE_DIALOGUE_TAVERN_ARCOMAGE_VICTORY_CONDITIONS:
    case HOUSE_DIALOGUE_TAVERN_ARCOMAGE_RESULT:
      pVideoPlayer->_4BF5B2();
      UI_CreateEndConversationButton();
      dialog_menu_id = HOUSE_DIALOGUE_TAVERN_ARCOMAGE_MAIN;
      InitializaDialogueOptions_Tavern(in_current_building_type);
      break;

    case HOUSE_DIALOGUE_NULL:
    case HOUSE_DIALOGUE_MAIN:
      pDialogueNPCCount = 0;
      pDialogueWindow->Release();
      dialog_menu_id = HOUSE_DIALOGUE_NULL;
      pDialogueWindow = 0;
      pIcons_LOD->SyncLoadedFilesCount();

      if ( uNumDialogueNPCPortraits == 1 )
        return 0;

      pBtn_ExitCancel = window_SpeakInHouse->pControlsHead;
      if ( uNumDialogueNPCPortraits > 0 )
      {
        for ( uint i = 0; i < (unsigned int)uNumDialogueNPCPortraits; ++i )
        {
          HouseNPCPortraitsButtonsList[i] = window_SpeakInHouse->CreateButton(pNPCPortraits_x[uNumDialogueNPCPortraits - 1][i],
                                            pNPCPortraits_y[uNumDialogueNPCPortraits - 1][i],
                                            63, 73, 1, 0, UIMSG_ClickHouseNPCPortrait, i, 0, byte_591180[i].data(), 0, 0, 0);
        }
      }

      pVideoPlayer->_4BF5B2();
      break;

    default:
      pVideoPlayer->_4BF5B2();
      dialog_menu_id = HOUSE_DIALOGUE_MAIN;
      InitializaDialogueOptions(in_current_building_type);
      break;
  }
  return 1;
}
