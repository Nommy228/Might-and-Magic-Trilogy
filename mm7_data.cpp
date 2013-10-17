#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "mm7_data.h"

#include "GUIWindow.h"
#include "Party.h"



void ErrHR(HRESULT hr, const char *pAPI, const char *pFunction, const char *pFile, int line)
{
  if (SUCCEEDED(hr))
    return;

  char msg[4096];
  sprintf(msg, "%s error (%08X) in\n\t%s\nin\n\t%s:%u",
          pAPI, hr, pFunction, pFile, line);

  char caption[1024];
  sprintf(caption, "%s error", pAPI);

  MessageBoxA(nullptr, msg, caption, MB_ICONEXCLAMATION);
}




#include "Math.h"
struct stru193_math *stru_5C6E00 = new stru193_math;

#include "MapInfo.h"
struct MapStats *pMapStats;

#include "Viewport.h"
struct Viewport *pViewport = new Viewport;
struct ViewingParams *viewparams = new ViewingParams;



#include "stru123.h"
stru123 stru_5E4C90_MapPersistVars;

#include "stru298.h"
stru298 stru_50FE08; // weak

#include "Autonotes.h"
std::array<Autonote, 196> pAutonoteTxt; // weak

#include "Awards.h"
std::array<Award, 105> pAwards;
std::array<AwardType, 1000> achieved_awards;
int num_achieved_awards;
int full_num_items_in_book;

#include "stru160.h"
std::array<stru160, 66> array_4EB8B8 =
{{
  stru160( 0.0000000f,  0.0000000f,  1.0000000f),
  stru160( 0.0000000f,  0.3826830f,  0.9238799f),
  stru160( 0.3826830f,  0.0000000f,  0.9238799f),
  stru160( 0.4082480f,  0.4082480f,  0.8164970f),
  stru160( 0.0000000f,  0.7071070f,  0.7071070f),
  stru160( 0.7071070f,  0.0000000f,  0.7071070f),
  stru160( 0.4082480f,  0.8164970f,  0.4082480f),
  stru160( 0.8164970f,  0.4082480f,  0.4082480f),
  stru160( 0.7071070f,  0.7071070f,  0.0000000f),
  stru160( 0.0000000f,  0.9238799f,  0.3826830f),
  stru160( 0.3826830f,  0.9238799f,  0.0000000f),
  stru160( 0.0000000f,  1.0000000f,  0.0000000f),
  stru160( 0.9238799f,  0.3826830f,  0.0000000f),
  stru160( 0.9238799f,  0.0000000f,  0.3826830f),
  stru160( 1.0000000f,  0.0000000f,  0.0000000f),
  stru160( 0.0000000f, -0.3826830f,  0.9238799f),
  stru160( 0.4082480f, -0.4082480f,  0.8164970f),
  stru160( 0.0000000f, -0.7071070f,  0.7071070f),
  stru160( 0.8164970f, -0.4082480f,  0.4082480f),
  stru160( 0.4082480f, -0.8164970f,  0.4082480f),
  stru160( 0.7071070f, -0.7071070f,  0.0000000f),
  stru160( 0.9238799f, -0.3826830f,  0.0000000f),
  stru160( 0.3826830f, -0.9238799f,  0.0000000f),
  stru160( 0.0000000f, -0.9238799f,  0.3826830f),
  stru160( 0.0000000f, -1.0000000f,  0.0000000f),
  stru160(-0.3826830f,  0.0000000f,  0.9238799f),
  stru160(-0.4082480f, -0.4082480f,  0.8164970f),
  stru160(-0.7071070f,  0.0000000f,  0.7071070f),
  stru160(-0.4082480f, -0.8164970f,  0.4082480f),
  stru160(-0.8164970f, -0.4082480f,  0.4082480f),
  stru160(-0.7071070f, -0.7071070f,  0.0000000f),
  stru160(-0.3826830f, -0.9238799f,  0.0000000f),
  stru160(-0.9238799f, -0.3826830f,  0.0000000f),
  stru160(-0.9238799f,  0.0000000f,  0.3826830f),
  stru160(-1.0000000f,  0.0000000f,  0.0000000f),
  stru160(-0.4082480f,  0.4082480f,  0.8164970f),
  stru160(-0.8164970f,  0.4082480f,  0.4082480f),
  stru160(-0.4082480f,  0.8164970f,  0.4082480f),
  stru160(-0.7071070f,  0.7071070f,  0.0000000f),
  stru160(-0.9238799f,  0.3826830f,  0.0000000f),
  stru160(-0.3826830f,  0.9238799f,  0.0000000f),
  stru160( 0.0000000f,  0.0000000f, -1.0000000f),
  stru160( 0.0000000f,  0.3826830f, -0.9238799f),
  stru160( 0.3826830f,  0.0000000f, -0.9238799f),
  stru160( 0.4082480f,  0.4082480f, -0.8164970f),
  stru160( 0.0000000f,  0.7071070f, -0.7071070f),
  stru160( 0.7071070f,  0.0000000f, -0.7071070f),
  stru160( 0.4082480f,  0.8164970f, -0.4082480f),
  stru160( 0.8164970f,  0.4082480f, -0.4082480f),
  stru160( 0.0000000f,  0.9238799f, -0.3826830f),
  stru160( 0.9238799f,  0.0000000f, -0.3826830f),
  stru160( 0.0000000f, -0.3826830f, -0.9238799f),
  stru160( 0.4082480f, -0.4082480f, -0.8164970f),
  stru160( 0.0000000f, -0.7071070f, -0.7071070f),
  stru160( 0.8164970f, -0.4082480f, -0.4082480f),
  stru160( 0.4082480f, -0.8164970f, -0.4082480f),
  stru160( 0.0000000f, -0.9238799f, -0.3826830f),
  stru160(-0.3826830f,  0.0000000f, -0.9238799f),
  stru160(-0.4082480f, -0.4082480f, -0.8164970f),
  stru160(-0.7071070f,  0.0000000f, -0.7071070f),
  stru160(-0.4082480f, -0.8164970f, -0.4082480f),
  stru160(-0.8164970f, -0.4082480f, -0.4082480f),
  stru160(-0.9238799f,  0.0000000f, -0.3826830f),
  stru160(-0.4082480f,  0.4082480f, -0.8164970f),
  stru160(-0.8164970f,  0.4082480f, -0.4082480f),
  stru160(-0.4082480f,  0.8164970f, -0.4082480f)
}};
std::array<stru160, 128> array_4EBBD0;
std::array<__int32, 128*3> array_4EBBD0_x =
{{
  0x00000000, 0x00000001, 0x00000002, 0x00000001,
  0x00000003, 0x00000002, 0x00000001, 0x00000004,
  0x00000003, 0x00000003, 0x00000005, 0x00000002,
  0x00000004, 0x00000006, 0x00000003, 0x00000006,
  0x00000007, 0x00000003, 0x00000006, 0x00000008,
  0x00000007, 0x00000007, 0x00000005, 0x00000003,
  0x00000004, 0x00000009, 0x00000006, 0x00000009,
  0x0000000A, 0x00000006, 0x00000009, 0x0000000B,
  0x0000000A, 0x0000000A, 0x00000008, 0x00000006,
  0x00000008, 0x0000000C, 0x00000007, 0x0000000C,
  0x0000000D, 0x00000007, 0x0000000C, 0x0000000E,
  0x0000000D, 0x0000000D, 0x00000005, 0x00000007,
  0x00000000, 0x00000002, 0x0000000F, 0x00000002,
  0x00000010, 0x0000000F, 0x00000002, 0x00000005,
  0x00000010, 0x00000010, 0x00000011, 0x0000000F,
  0x00000005, 0x00000012, 0x00000010, 0x00000012,
  0x00000013, 0x00000010, 0x00000012, 0x00000014,
  0x00000013, 0x00000013, 0x00000011, 0x00000010,
  0x00000005, 0x0000000D, 0x00000012, 0x0000000D,
  0x00000015, 0x00000012, 0x0000000D, 0x0000000E,
  0x00000015, 0x00000015, 0x00000014, 0x00000012,
  0x00000014, 0x00000016, 0x00000013, 0x00000016,
  0x00000017, 0x00000013, 0x00000016, 0x00000018,
  0x00000017, 0x00000017, 0x00000011, 0x00000013,
  0x00000000, 0x0000000F, 0x00000019, 0x0000000F,
  0x0000001A, 0x00000019, 0x0000000F, 0x00000011,
  0x0000001A, 0x0000001A, 0x0000001B, 0x00000019,
  0x00000011, 0x0000001C, 0x0000001A, 0x0000001C,
  0x0000001D, 0x0000001A, 0x0000001C, 0x0000001E,
  0x0000001D, 0x0000001D, 0x0000001B, 0x0000001A,
  0x00000011, 0x00000017, 0x0000001C, 0x00000017,
  0x0000001F, 0x0000001C, 0x00000017, 0x00000018,
  0x0000001F, 0x0000001F, 0x0000001E, 0x0000001C,
  0x0000001E, 0x00000020, 0x0000001D, 0x00000020,
  0x00000021, 0x0000001D, 0x00000020, 0x00000022,
  0x00000021, 0x00000021, 0x0000001B, 0x0000001D,
  0x00000000, 0x00000019, 0x00000001, 0x00000019,
  0x00000023, 0x00000001, 0x00000019, 0x0000001B,
  0x00000023, 0x00000023, 0x00000004, 0x00000001,
  0x0000001B, 0x00000024, 0x00000023, 0x00000024,
  0x00000025, 0x00000023, 0x00000024, 0x00000026,
  0x00000025, 0x00000025, 0x00000004, 0x00000023,
  0x0000001B, 0x00000021, 0x00000024, 0x00000021,
  0x00000027, 0x00000024, 0x00000021, 0x00000022,
  0x00000027, 0x00000027, 0x00000026, 0x00000024,
  0x00000026, 0x00000028, 0x00000025, 0x00000028,
  0x00000009, 0x00000025, 0x00000028, 0x0000000B,
  0x00000009, 0x00000009, 0x00000004, 0x00000025,
  0x00000029, 0x0000002A, 0x0000002B, 0x0000002A,
  0x0000002C, 0x0000002B, 0x0000002A, 0x0000002D,
  0x0000002C, 0x0000002C, 0x0000002E, 0x0000002B,
  0x0000002D, 0x0000002F, 0x0000002C, 0x0000002F,
  0x00000030, 0x0000002C, 0x0000002F, 0x00000008,
  0x00000030, 0x00000030, 0x0000002E, 0x0000002C,
  0x0000002D, 0x00000031, 0x0000002F, 0x00000031,
  0x0000000A, 0x0000002F, 0x00000031, 0x0000000B,
  0x0000000A, 0x0000000A, 0x00000008, 0x0000002F,
  0x00000008, 0x0000000C, 0x00000030, 0x0000000C,
  0x00000032, 0x00000030, 0x0000000C, 0x0000000E,
  0x00000032, 0x00000032, 0x0000002E, 0x00000030,
  0x00000029, 0x0000002B, 0x00000033, 0x0000002B,
  0x00000034, 0x00000033, 0x0000002B, 0x0000002E,
  0x00000034, 0x00000034, 0x00000035, 0x00000033,
  0x0000002E, 0x00000036, 0x00000034, 0x00000036,
  0x00000037, 0x00000034, 0x00000036, 0x00000014,
  0x00000037, 0x00000037, 0x00000035, 0x00000034,
  0x0000002E, 0x00000032, 0x00000036, 0x00000032,
  0x00000015, 0x00000036, 0x00000032, 0x0000000E,
  0x00000015, 0x00000015, 0x00000014, 0x00000036,
  0x00000014, 0x00000016, 0x00000037, 0x00000016,
  0x00000038, 0x00000037, 0x00000016, 0x00000018,
  0x00000038, 0x00000038, 0x00000035, 0x00000037,
  0x00000029, 0x00000033, 0x00000039, 0x00000033,
  0x0000003A, 0x00000039, 0x00000033, 0x00000035,
  0x0000003A, 0x0000003A, 0x0000003B, 0x00000039,
  0x00000035, 0x0000003C, 0x0000003A, 0x0000003C,
  0x0000003D, 0x0000003A, 0x0000003C, 0x0000001E,
  0x0000003D, 0x0000003D, 0x0000003B, 0x0000003A,
  0x00000035, 0x00000038, 0x0000003C, 0x00000038,
  0x0000001F, 0x0000003C, 0x00000038, 0x00000018,
  0x0000001F, 0x0000001F, 0x0000001E, 0x0000003C,
  0x0000001E, 0x00000020, 0x0000003D, 0x00000020,
  0x0000003E, 0x0000003D, 0x00000020, 0x00000022,
  0x0000003E, 0x0000003E, 0x0000003B, 0x0000003D,
  0x00000029, 0x00000039, 0x0000002A, 0x00000039,
  0x0000003F, 0x0000002A, 0x00000039, 0x0000003B,
  0x0000003F, 0x0000003F, 0x0000002D, 0x0000002A,
  0x0000003B, 0x00000040, 0x0000003F, 0x00000040,
  0x00000041, 0x0000003F, 0x00000040, 0x00000026,
  0x00000041, 0x00000041, 0x0000002D, 0x0000003F,
  0x0000003B, 0x0000003E, 0x00000040, 0x0000003E,
  0x00000027, 0x00000040, 0x0000003E, 0x00000022,
  0x00000027, 0x00000027, 0x00000026, 0x00000040,
  0x00000026, 0x00000028, 0x00000041, 0x00000028,
  0x00000031, 0x00000041, 0x00000028, 0x0000000B,
  0x00000031, 0x00000031, 0x0000002D, 0x00000041
}};

#include "stru176.h"
stru176 array_5B5928_timers[100];

#include "ObjectList.h"
struct ObjectList *pObjectList;

#include "IconFrameTable.h"
struct IconFrameTable *pIconsFrameTable;

struct
{
  struct UIAnimation _pUIAnim_Food;
  struct UIAnimation _pUIAnim_Gold;
  struct UIAnimation _pUIAnum_Torchlight;
  struct UIAnimation _pUIAnim_WizardEye;
} _uianim;

struct UIAnimation *pUIAnim_Food = &_uianim._pUIAnim_Food;
struct UIAnimation *pUIAnim_Gold = &_uianim._pUIAnim_Gold;
struct UIAnimation *pUIAnum_Torchlight = &_uianim._pUIAnum_Torchlight;
struct UIAnimation *pUIAnim_WizardEye = &_uianim._pUIAnim_WizardEye;

std::array<struct UIAnimation *, 4> pUIAnims =
{
  &_uianim._pUIAnim_Food,
  &_uianim._pUIAnim_Gold,
  &_uianim._pUIAnum_Torchlight,
  &_uianim._pUIAnim_WizardEye
};



#include "PlayerFrameTable.h"
struct PlayerFrameTable *pPlayerFrameTable; // idb

#include "MM7.h"


std::array<stat_coord, 26> stat_string_coord = //4E2940
{{
  {0x1A, 0x39, 0xDC, 0x12},
  {0x1A, 0x4A, 0xDC, 0x12},
  {0x1A, 0x5B, 0xDC, 0x12},
  {0x1A, 0x6C, 0xDC, 0x12},
  {0x1A, 0x7D, 0xDC, 0x12},
  {0x1A, 0x8E, 0xDC, 0x12},
  {0x1A, 0x9F, 0xDC, 0x12},
  {0x1A, 0xC6, 0xDC, 0x12},
  {0x1A, 0xD7, 0xDC, 0x12},
  {0x1A, 0xE8, 0xDC, 0x12},
  {0x1A, 0x10C, 0xDC, 0x12},
  {0x1A, 0x11E, 0xDC, 0x12},
  {0x111, 0x36, 0xBA, 0x12},
  {0x111, 0x47, 0xBA, 0x12},
  {0x111, 0x58, 0xBA, 0x12},
  {0x111, 0x7E, 0xBA, 0x12},
  {0x111, 0x8F, 0xBA, 0x12},
  {0x111, 0xA0, 0xBA, 0x12},
  {0x111, 0xB1, 0xBA, 0x12},
  {0x111, 0xCA, 0xBA, 0x12},
  {0x111, 0xDD, 0xBA, 0x12},
  {0x111, 0xF0, 0xBA, 0x12},
  {0x111, 0x103, 0xBA, 0x12},
  {0x111, 0x116, 0xBA, 0x12},
  {0x111, 0x129, 0xBA, 0x12},
  {0x13E, 0x12, 0x89, 0x12},
}
};




namespace zlib
{
  #include "lib\zlib\zlib.h"
  int MemUnzip(void *dest, unsigned int *destLen, const void *source, unsigned int sourceLen)
  {
    //return uncompress((zlib::Bytef *)dest, (zlib::uLongf *)destLen, (zlib::Bytef *)source, sourceLen);
    return uncompress((Bytef *)dest, (uLongf *)destLen, (Bytef *)source, sourceLen);
    return 0;
  }

  int MemZip(void *dest, unsigned int *destLen, void *source, unsigned int sourceLen)
  {
    //return compress((zlib::Bytef *)dest, (zlib::uLongf *)destLen, (zlib::Bytef *)source, sourceLen);
    return compress((Bytef *)dest, (uLongf *)destLen, (Bytef *)source, sourceLen);
    return 0;
  }
};

#include "stru314.h"
#include "stru367.h"



//-------------------------------------------------------------------------
// Data declarations

int game_starting_year = 0;
int game_viewport_width;
int game_viewport_height;
int game_viewport_x;
int game_viewport_y;
int game_viewport_z;
int game_viewport_w;



std::array<unsigned int, 2> saveload_dlg_xs = {82, 0};
std::array<unsigned int, 2> saveload_dlg_ys = {60, 0};
std::array<unsigned int, 2> saveload_dlg_zs = {460, 640};
std::array<unsigned int, 2> saveload_dlg_ws = {344, 480};
int pWindowList_at_506F50_minus1_indexing[1];
int dword_4C9890[10]; // weak
int dword_4C9920[16]; // weak
_UNKNOWN unk_4D8548; // weak
char byte_4D864C; // weak
float flt_4D86CC = 1.0f; // weak
int dword_4D86D8; // weak
int dword_4DAFCC; // weak
int (__stdcall *off_4DAFDC)(char); // weak
char asc_4DB724[777]; // idb
int dword_4DBD94; // weak
std::array<int, 5> dword_4DF380 = {{0,1024,2560,5120,10240}}; // weak
//int dword_4DF390; // weak
char Str2[777]; // idb







float flt_4D84E8 = 0.0f;

char aIxf[4]; // idb
_UNKNOWN unk_4E19FC; // weak
char aD[777]; // idb
char asc_4E1AB0[777]; // idb
char aWb_0[777]; // idb
char a24bitPcxOnly[777]; // idb
char Mode[777]; // idb
char a16bitPcx[777]; // idb
char aUnableToLoadS[777]; // idb
char aBitmaps[777]; // idb
char aIcons[777]; // idb
char aPending[777]; // idb
char aCanTFindS[777]; // idb

//std::array<char, 7> aSbwb00; // weak
char aW[2]; // idb
char aA[2]; // idb
char aSD[777]; // idb
char aSS03d03dS000S0[777]; // idb
char aLuS[777]; // idb
char aS_1[777]; // idb
char aSbSc02d[777]; // idb
char aSbSs02d[777]; // idb
char aTabDb[777]; // idb
char aTabDa[777]; // idb
char aIbM6D[777]; // idb
char aIbM6U[777]; // idb
char aIbM5D[777]; // idb
char aIbM5U[777]; // idb
char aPagemask[777]; // idb
char aBook[777]; // idb
char aSpell_fnt[777]; // idb
char aAutonote_fnt[777]; // idb
char aBook2_fnt[777]; // idb
char pFontFile[777]; // idb
char aFontpal[777]; // idb
char aMapbordr[777]; // idb
char aLloydDD_pcx[777]; // idb
char aDataLloydDD_pc[777]; // idb
char aLb_bordr[777]; // idb
char aTphell[777]; // idb
char aTpheaven[777]; // idb
char aTpisland[777]; // idb
char aTpwarlock[777]; // idb
char aTpelf[777]; // idb
char aTpharmndy[777]; // idb
char aTownport[777]; // idb
char aSbquiknot[777]; // idb
char aTabAn8a[777]; // idb
char aTabAn8b[777]; // idb
char aTabAn4a[777]; // idb
char aTabAn4b[777]; // idb
char aTabAn5a[777]; // idb
char aTabAn5b[777]; // idb
char aTabAn3a[777]; // idb
char aTabAn3b[777]; // idb
char aTabAn2a[777]; // idb
char aTabAn2b[777]; // idb
char aTabAn1a[777]; // idb
char aTabAn1b[777]; // idb
char aDivbar[777]; // idb
char aSbautnot[777]; // idb
char aTabwoff[777]; // idb
char aTabwon[777]; // idb
char aTabeoff[777]; // idb
char aTabeon[777]; // idb
char aTabsoff[777]; // idb
char aTabson[777]; // idb
char aTabnoff[777]; // idb
char aTabnon[777]; // idb
char aZootOff[777]; // idb
char aZoomOff[777]; // idb
char aZootOn[777]; // idb
char aZoomOn[777]; // idb
char aSbmap[777]; // idb
char aMoon_ful[777]; // idb
char aMoon_2[777]; // idb
char aMoon_4[777]; // idb
char aMoon_new[777]; // idb
char aSbdateTime[777]; // idb
char aTabAn7a[777]; // idb
char aTabAn6a[777]; // idb
char aTabAn7b[777]; // idb
char aTabAn6b[777]; // idb
char aSbplayrnot[777]; // idb
char aPending_0[777]; // idb
char aUnknown[8]; // weak
char aS100110S[777]; // idb
char aS100110D[777]; // idb
char aS100110DS[777]; // idb
char aS100110D02dSS[777]; // idb
int pCurrentScreen = SCREEN_VIDEO; // 004E28F8
unsigned int uGammaPos;
std::array<int, 8> BtnTurnCoord = 
{{
  0xA4, 0x5D, 0x16, 0xB, 0x5, 0xD, 0x7, 0x3B,
}}; // weak
std::array<__int16, 4> RightClickPortraitXmin={{0x14, 0x83, 0xF2, 0x165}};
std::array<__int16, 4> RightClickPortraitXmax={{0x53, 0xC6, 0x138, 0x1A7}};
void *off_4E2A12; // stat_string_control_button_count

std::array<unsigned int, 4> pHealthBarPos = {{22, 137, 251, 366}};
std::array<unsigned int, 4> pManaBarPos = {{102, 217, 331, 447}};

std::array<char, 88> monster_popup_y_offsets =
{{
  -20,  20,   0, -40,   0,   0,   0,   0,   0,   0, -50,
   20,   0, -10, -10, -20,  10, -10,   0,   0,   0, -20,
   10, -10,   0,   0,   0, -20, -10,   0,   0,   0, -40,
  -20,   0,   0,   0, -50, -30, -30, -30, -30, -30, -30,
    0,   0,   0,   0,   0,   0, -20, -20, -20,  20,  20,
   20,  10,  10,  10,  10,  10,  10, -90, -60, -40, -20,
  -20, -80, -10,   0,   0, -40,   0,   0,   0, -20,  10,
    0,   0,   0,   0,   0,   0, -60,   0,   0,   0,   0
}};
unsigned char hourglass_icon_idx = 12; // weak


const char *format_4E2D80 = "\f%05d%s\f00000\n";
//const char *format_4E2DC8 = "\f%05d";
const char *format_4E2DE8 = "\f%05d%s\f00000 - ";
const char *format_4E2E00 = "%s\f%05u\xD\r180%s\n"; // idb
const char *format_4E2E10 = "%s\f%05u\t110%d\f00000 / %d\n";
__int16 word_4E3C66[777]; // idb
int dword_4E455C; // weak
std::array<int, 6> dword_4E4560;
std::array<int, 6> dword_4E4578;
std::array<int, 6> dword_4E4590;
std::array<int, 6> dword_4E45A8;
_UNKNOWN dword_4E49D4; // idb
int dword_4E4A18[777]; // weak
int dword_4E4A1C[777]; // weak
int dword_4E4A40[777]; // weak
int dword_4E4A44[777]; // weak
std::array<float, 10> flt_4E4A80 =
{
  0.050000001, 0.1,  0.30000001,  0.5,    0.60000002,
  1.0,         6.0, 25.0,        50.0,  100.0
};


std::array< std::array<int, 2>, 14> pPartySpellbuffsUI_XYs =
{{
  {477, 247}, {497, 247}, {522, 247}, {542, 247}, {564, 247}, {581, 247}, {614, 247},
  {477, 279}, {497, 279}, {522, 279}, {542, 279}, {564, 279}, {589, 279}, {612, 279}
}};
std::array<unsigned char, 14> byte_4E5DD8 =
{{
  PARTY_BUFF_FEATHER_FALL, PARTY_BUFF_RESIST_FIRE, PARTY_BUFF_RESIST_AIR, PARTY_BUFF_RESIST_WATER, PARTY_BUFF_RESIST_MIND,           PARTY_BUFF_RESIST_EARTH, PARTY_BUFF_RESIST_BODY,
  PARTY_BUFF_HEROISM,      PARTY_BUFF_HASTE,       PARTY_BUFF_SHIELD,     PARTY_BUFF_STONE_SKIN,   PARTY_BUFF_PROTECTION_FROM_MAGIC, PARTY_BUFF_IMMOLATION,   PARTY_BUFF_DAY_OF_GODS
}};
std::array<unsigned __int8, 14> pPartySpellbuffsUI_smthns =
{{
  14, 1, 10, 4, 7,  2, 9,
   3, 6, 15, 8, 3, 12, 0
}};

std::array< std::array<int, 6>, 6> pNPCPortraits_x =     // 004E5E50
{{
  {521,   0,   0,   0,   0,  0},
  {521, 521,   0,   0,   0,  0},
  {521, 521, 521,   0,   0,  0},
  {521, 486, 564, 521,   0,  0},
  {521, 486, 564, 486, 564,  0},
  {486, 564, 486, 564, 486, 564}
}};
std::array< std::array<int, 6>, 6> pNPCPortraits_y =     // 004E5EE0
{{
  {38,   0,   0,   0,   0,   0},
  {38, 165,   0,   0,   0,   0},
  {38, 133, 228,   0,   0,   0},
  {38, 133, 133, 228,   0,   0},
  {38, 133, 133, 228, 228,   0},
  {38,  38, 133, 133, 228, 228}
}};
std::array<const char *, 11> pHouse_ExitPictures=
{
	"",
	"ticon01",
	"ticon02",
	"ticon03",
	"ticon04",
	"ticon05",
	"istairup",
	"itrap",
	"outside",
	"idoor",
	"isecdoor"
};

int bWinNT4_0; // weak
std::array<__int16, 11> word_4E8152 = {0, 0, 0, 90, 8, 2, 70, 20, 10, 50, 30};

char byte_4E94D0 = 5; // weak
char byte_4E94D1 = 9; // weak
char _4E94D2_light_type = 6; // weak
char byte_4E94D3 = 10; // weak
int dword_4E98BC_bApplicationActive; // weak
//char *off_4EB080; // idb
std::array<char*, 465> pTransitionStrings = {"", nullptr};  // 004EB080
std::array<const char*, 25> pPlayerPortraitsNames =
{{
  "pc01-", "pc02",  "pc03", "pc04",  "pc05-",
  "pc06",  "pc07",  "pc08", "pc09-", "pc10",
  "pc11-", "pc12",  "pc13", "pc14",  "pc15",
  "pc16",  "pc17-", "pc18", "pc19",  "pc20",
  "pc21-", "pc22-", "pc23", "pc24-", "pc25-"
}};



std::array< std::array<unsigned char, 25>, 48> byte_4ECF08 =      // 4ECF08
{{
  {2, 1, 2, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1, 1},//1
  {2, 1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2},//2
  {2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},//3
  {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},//4
  {2, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},//5
  {2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},//6
  {2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},//7
  {2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1, 1},//8
  {2, 1, 2, 2, 2, 2, 1, 2, 1, 2, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 2, 2, 2, 2},//9
  {2, 1, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},//10
  {2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 1, 1, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2},//11
  {1, 1, 2, 1, 2, 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 1, 2, 1, 1, 2, 1, 2, 1, 2, 2},//12
  {2, 1, 2, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},//13
  {1, 1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 1, 2, 1, 2, 1, 2, 2},//14
  {2, 1, 1, 2, 1, 1, 2, 1, 2, 2, 2, 1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2},//15
  {1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1, 1},//16
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},//17
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 2, 2, 1, 2, 1, 1, 1, 2, 1, 2, 1, 1},//18
  {1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},//19
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},//20
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},//21
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},//22
  {2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1},//23
  {2, 2, 2, 1, 2, 2, 2, 2, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2},//24
  {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 1, 1, 1, 1, 2, 2, 2, 1, 2, 1, 1},//25
  {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},//26
  {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2, 1, 2, 2, 2, 2, 2, 2, 2},//27
  {1, 2, 2, 1, 2, 2, 2, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},//28
  {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 2, 2, 2, 2, 2, 1, 2, 2, 1, 2, 1, 1},//29
  {1, 2, 2, 1, 2, 1, 2, 2, 1, 2, 2, 1, 1, 1, 2, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1},//30
  {1, 1, 2, 1, 2, 1, 2, 2, 2, 2, 2, 1, 2, 1, 2, 1, 2, 2, 2, 2, 2, 1, 2, 1, 1},//31
  {1, 1, 2, 1, 1, 2, 2, 2, 2, 1, 2, 2, 1, 1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2},//32
  {1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1},//33
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1},//34
  {1, 1, 1, 2, 1, 2, 2, 1, 1, 2, 2, 1, 2, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2},//35
  {1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2},//36
  {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2},//37
  {2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 1, 2, 1, 1},//38
  {1, 1, 1, 1, 2, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1},//39
  {1, 2, 2, 1, 2, 2, 2, 2, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},//40
  {2, 1, 2, 1, 2, 1, 2, 2, 2, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1},//41
  {1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2, 1, 1},//42
  {1, 2, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},//43
  {1, 2, 1, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 2, 1, 1, 2, 1, 2, 2},//44
  {2, 1, 1, 1, 2, 2, 1, 1, 2, 1, 2, 1, 2, 1, 2, 2, 1, 2, 2, 2, 1, 2, 1, 2, 2},//45
  {1, 1, 1, 2, 2, 2, 1, 1, 2, 1, 2, 1, 2, 2, 2, 2, 2, 1, 1, 2, 1, 2, 1, 2, 2},//46
  {1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 2, 2, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1},//47
  {1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 2, 2, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1},//48
}};
std::array<std::array<unsigned char, 8>, 110> SoundSetAction = // 4ED3D8
{{
  { 0,  0,  0,  0,  0,  0,  0,  0},
  {38,  0,  0, 37, 38, 45,  0,  0},
  {37,  0,  0, 47, 53,  0,  0,  0},
  { 2,  0,  0, 25,  0,  0,  0,  0},
  { 1,  0,  0, 47,  0,  0,  0,  0},
  { 3,  0,  0, 41, 42,  0,  0,  0},
  { 0,  0,  0, 33,  0,  0,  0,  0},
  { 5,  0,  0, 21, 22, 23, 24, 45},
  { 6,  0,  0, 47,  0,  0,  0,  0},
  { 7,  0,  0, 43, 48,  0,  0,  0},
  { 8,  0,  0, 38,  0,  0,  0,  0},
  { 9,  0,  0, 39,  0,  0,  0,  0},
  {13,  0,  0, 37, 38,  0,  0,  0},
  {23,  0,  0, 25,  0,  0,  0,  0},
  { 0,  0,  0, 37, 38, 45,  0,  0},
  {25,  0,  0, 25,  0,  0,  0,  0},
  {26,  0,  0, 38, 47,  0,  0,  0},
  {27,  0,  0, 41, 42,  0,  0,  0},
  {28,  0,  0, 25,  0,  0,  0,  0},
  { 0,  0,  0, 51,  0,  0,  0,  0},
  {30,  0,  0, 25,  0,  0,  0,  0},
  {29,  0,  0, 27,  0,  0,  0,  0},
  {32, 33,  0, 37, 38,  0,  0,  0},
  {34,  0,  0, 53,  0,  0,  0,  0},
  {15,  0,  0, 34, 35, 36,  0,  0},
  { 0,  0,  0,  3,  0,  0,  0,  0},
  {22,  0,  0,  5,  0,  0,  0,  0},
  {20,  0,  0,  8,  0,  0,  0,  0},
  {20,  0,  0,  9,  0,  0,  0,  0},
  {19,  0,  0,  7,  0,  0,  0,  0},
  {21,  0,  0,  2,  0,  0,  0,  0},
  {18,  0,  0,  6,  0,  0,  0,  0},
  { 0,  0,  0, 11,  0,  0,  0,  0},
  {17,  0,  0,  0,  0,  0,  0,  0},
  { 0,  0,  0, 12,  0,  0,  0,  0},
  {17,  0,  0,  0,  0,  0,  0,  0},
  { 0,  0,  0, 37,  0,  0,  0,  0},
  { 0,  0,  0, 49,  0,  0,  0,  0},
  {24,  0,  0, 25,  0,  0,  0,  0},
  {31,  0,  0, 25,  0,  0,  0,  0},
  { 0,  0,  0, 41, 42,  0,  0,  0},
  { 0,  0,  0, 39, 48,  0,  0,  0},
  { 0,  0,  0, 46,  0,  0,  0,  0}, 
  { 0,  0,  0, 39, 48,  0,  0,  0},
  { 0,  0,  0, 41, 42,  0,  0,  0},
  { 0,  0,  0, 39, 48,  0,  0,  0},
  {41,  0,  0, 21,  0,  0,  0,  0},
  {40,  0,  0, 21,  0,  0,  0,  0},
  {16,  0,  0, 46,  0,  0,  0,  0},
  { 0,  0,  0, 40,  0,  0,  0,  0},
  { 0,  0,  0, 39,  0,  0,  0,  0},
  { 0,  0,  0, 37, 38,  0,  0,  0},
  { 0,  0,  0, 48, 49, 50,  0,  0},
  { 0,  0,  0, 44,  0,  0,  0,  0},
  { 0,  0,  0, 48,  0,  0,  0,  0},
  { 0,  0,  0, 37, 38, 45,  0,  0},
  { 0,  0,  0, 41, 42,  0,  0,  0},
  { 0,  0,  0, 47,  0,  0,  0,  0},
  { 0,  0,  0, 39, 48,  0,  0,  0},
  { 0,  0,  0, 39, 48,  0,  0,  0},
  { 0,  0,  0, 37, 38,  0,  0,  0},
  { 0,  0,  0, 37, 38,  0,  0,  0},
  { 0,  0,  0, 37, 38,  0,  0,  0},
  { 0,  0,  0, 31,  0,  0,  0,  0},
  { 0,  0,  0, 32,  0,  0,  0,  0},
  {47,  0,  0, 46,  0,  0,  0,  0},
  {22,  0,  0,  5,  0,  0,  0,  0},
  { 0,  0,  0, 25,  0,  0,  0,  0},
  { 0,  0,  0, 37,  0,  0,  0,  0},
  { 0,  0,  0, 52,  0,  0,  0,  0},
  { 0,  0,  0, 45,  0,  0,  0,  0},
  { 0,  0,  0, 27,  0,  0,  0,  0},
  { 0,  0,  0, 37, 38,  0,  0,  0},
  { 0,  0,  0, 49,  0,  0,  0,  0},
  { 0,  0,  0, 48,  0,  0,  0,  0},
  { 0,  0,  0, 37, 38, 45,  0,  0},
  { 0,  0,  0, 48,  0,  0,  0,  0},
  { 0,  0,  0, 37, 38, 45,  0,  0},
  { 0,  0,  0, 47,  0,  0,  0,  0},
  { 0,  0,  0, 48,  0,  0,  0,  0},
  {44,  0,  0, 39, 48,  0,  0,  0},
  { 0,  0,  0, 48, 49,  0,  0,  0},
  { 0,  0,  0, 38,  0,  0,  0,  0},
  { 0,  0,  0, 31,  0,  0,  0,  0},
  {32, 33,  0, 37, 38,  0,  0,  0},
  { 0,  0,  0, 37, 38,  0,  0,  0},
  { 0,  0,  0, 37, 38,  0,  0,  0},
  { 0,  0,  0, 47,  0,  0,  0,  0},
  { 0,  0,  0, 38,  0,  0,  0,  0},
  { 0,  0,  0, 48,  0,  0,  0,  0},
  { 0,  0,  0,  0,  0,  0,  0,  0},
  { 0,  0,  0, 38,  0,  0,  0,  0},
  { 0,  0,  0, 47,  0,  0,  0,  0},
  { 0,  0,  0, 38,  0,  0,  0,  0},
  { 0,  0,  0,  0,  0,  0,  0,  0},
  { 0,  0,  0,  0,  0,  0,  0,  0},
  { 0,  0,  0, 38,  0,  0,  0,  0},
  { 0,  0,  0, 38,  0,  0,  0,  0},
  { 0,  0,  0,  5,  0,  0,  0,  0},
  { 0,  0,  0, 37, 38,  0,  0,  0},
  { 0,  0,  0, 44,  0,  0,  0,  0},
  { 0,  0,  0, 48,  0,  0,  0,  0},
  { 4,  0,  0,  0,  0,  0,  0,  0},
  { 0,  0,  0, 58,  0,  0,  0,  0},
  {10,  0,  0, 45,  0,  0,  0,  0},
  {11,  0,  0, 47,  0,  0,  0,  0},
  {12,  0,  0, 43, 48,  0,  0,  0},
  {39,  0,  0, 48,  0,  0,  0,  0},
  {14,  0,  0, 48,  0,  0,  0,  0},
  {17,  0,  0,  0,  0,  0,  0,  0}
}};
std::array<__int16, 4> pPlayerPortraitsXCoords_For_PlayerBuffAnimsDrawing = {34, 149, 264, 379};
std::array< std::array<char, 37>, 36> byte_4ED970_skill_learn_ability_by_class_table = 
{{
 2, 3, 2, 3, 3, 2, 3, 4, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 3, 0, 2, 1, 0, 1, 2, 2, 0, 3, 0, 0, 1,//Knight
 2, 3, 2, 3, 3, 2, 3, 4, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 3, 0, 2, 1, 0, 1, 2, 2, 0, 3, 0, 0, 1,
 2, 4, 2, 3, 4, 2, 3, 4, 4, 3, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 4, 4, 0, 2, 1, 0, 1, 2, 2, 0, 4, 0, 0, 1,
 2, 4, 2, 3, 4, 2, 3, 4, 4, 3, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 4, 4, 0, 2, 1, 0, 1, 2, 2, 0, 4, 0, 0, 1,
 
 0, 3, 3, 0, 0, 2, 2, 4, 1, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 1, 2, 0, 3, 0, 0, 3, 3, 2, 0, 3, 3, 2, 2,//Thief
 0, 3, 3, 0, 0, 2, 2, 4, 1, 3, 2, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 3, 3, 1, 2, 0, 3, 0, 0, 3, 3, 2, 0, 3, 3, 2, 2,
 0, 3, 4, 0, 0, 2, 2, 4, 1, 4, 2, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 3, 3, 1, 2, 0, 3, 0, 0, 4, 3, 2, 0, 3, 4, 2, 2,
 0, 3, 4, 0, 0, 2, 2, 4, 1, 4, 2, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 3, 3, 1, 2, 0, 3, 0, 0, 4, 3, 2, 0, 3, 4, 2, 2,

 3, 2, 2, 0, 2, 1, 0, 4, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 2, 1, 0, 1, 3, 3, 2, 3, 1, 0, 3,//Monk
 3, 2, 2, 0, 2, 1, 0, 4, 0, 3, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 3, 0, 2, 1, 0, 2, 3, 3, 2, 3, 1, 0, 3,
 4, 2, 2, 0, 2, 1, 0, 4, 0, 3, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 4, 0, 2, 1, 0, 2, 4, 4, 2, 3, 1, 0, 4,
 4, 2, 2, 0, 2, 1, 0, 4, 0, 3, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 4, 0, 2, 1, 0, 3, 4, 4, 2, 3, 2, 0, 4,

 1, 3, 2, 2, 2, 2, 3, 4, 3, 2, 2, 3, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 2, 3, 3, 2, 1, 3, 0, 0, 1, 1, 0, 2, 0, 0, 1,//Paladin
 1, 3, 2, 2, 2, 2, 3, 4, 3, 2, 2, 3, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 2, 3, 3, 2, 1, 3, 0, 0, 1, 1, 0, 2, 0, 0, 1,
 1, 3, 2, 2, 2, 2, 4, 4, 4, 2, 2, 3, 0, 0, 0, 0, 3, 3, 3, 1, 0, 0, 2, 4, 3, 2, 1, 4, 0, 0, 1, 1, 0, 2, 0, 0, 1,
 1, 3, 2, 2, 2, 2, 4, 4, 4, 2, 2, 3, 0, 0, 0, 0, 3, 3, 3, 0, 1, 0, 2, 4, 3, 2, 1, 4, 0, 0, 1, 1, 0, 2, 0, 0, 1,

 1, 2, 2, 2, 3, 3, 0, 4, 0, 3, 3, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 3, 0, 0, 1, 2, 1, 0, 2, 0, 0, 3,//Archer
 1, 2, 2, 2, 3, 3, 0, 4, 0, 3, 3, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 3, 0, 0, 1, 2, 1, 0, 2, 0, 0, 3,
 1, 2, 2, 2, 3, 4, 0, 4, 0, 3, 4, 0, 3, 3, 3, 3, 0, 0, 0, 1, 0, 0, 2, 2, 2, 2, 4, 0, 0, 2, 2, 1, 0, 2, 0, 0, 3,
 1, 2, 2, 2, 3, 4, 0, 4, 0, 3, 4, 0, 3, 3, 3, 3, 0, 0, 0, 0, 1, 0, 2, 2, 2, 2, 4, 0, 0, 2, 2, 1, 0, 2, 0, 0, 3,

 1, 2, 2, 3, 2, 3, 0, 4, 2, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 1, 3, 2, 0, 2, 2, 1, 3, 2, 2, 1, 2,//Ranger
 1, 2, 2, 3, 2, 3, 0, 4, 2, 3, 3, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 2, 1, 3, 2, 0, 2, 2, 1, 3, 2, 2, 1, 2,
 1, 2, 2, 4, 2, 3, 0, 4, 2, 3, 3, 0, 2, 2, 2, 2, 2, 2, 2, 0, 0, 1, 1, 1, 2, 1, 3, 2, 0, 2, 2, 1, 4, 2, 2, 1, 2,
 1, 2, 2, 4, 2, 3, 0, 4, 2, 3, 3, 0, 2, 2, 2, 2, 2, 2, 2, 0, 0, 1, 1, 1, 2, 1, 3, 2, 0, 2, 2, 1, 4, 2, 2, 1, 2,

 1, 0, 0, 0, 0, 2, 3, 4, 3, 2, 2, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 3, 3, 1, 3, 2, 3, 0, 0, 0, 0, 2, 0, 0, 2, 3,//Cleric
 1, 0, 0, 0, 0, 2, 3, 4, 3, 2, 2, 0, 0, 0, 0, 0, 3, 3, 3, 0, 0, 0, 3, 3, 1, 3, 2, 3, 0, 0, 0, 0, 2, 0, 0, 2, 3,
 1, 0, 0, 0, 0, 2, 3, 4, 3, 2, 2, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 4, 3, 1, 3, 2, 3, 0, 0, 0, 0, 2, 0, 0, 2, 3,
 1, 0, 0, 0, 0, 2, 3, 4, 3, 2, 2, 0, 0, 0, 0, 0, 4, 4, 4, 0, 4, 0, 4, 3, 1, 3, 2, 3, 0, 0, 0, 0, 2, 0, 0, 2, 3,

 1, 0, 3, 0, 0, 1, 2, 4, 2, 2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 0, 0, 2, 2, 0, 0, 3, 2, 0, 0, 0, 0, 0, 2, 1, 0, 3, 3,//Druid
 1, 0, 3, 0, 0, 1, 2, 4, 2, 2, 0, 0, 3, 3, 3, 3, 3, 3, 3, 0, 0, 2, 2, 0, 0, 3, 2, 0, 0, 0, 0, 0, 2, 1, 0, 3, 3,
 1, 0, 3, 0, 0, 1, 2, 4, 2, 2, 0, 0, 3, 3, 3, 3, 3, 3, 3, 0, 0, 2, 2, 0, 0, 4, 2, 0, 0, 0, 0, 0, 2, 1, 0, 4, 3,
 1, 0, 3, 0, 0, 1, 2, 4, 2, 2, 0, 0, 3, 3, 3, 3, 3, 3, 3, 0, 0, 2, 2, 0, 0, 4, 2, 0, 0, 0, 0, 0, 2, 1, 0, 4, 3,

 3, 0, 2, 0, 0, 1, 0, 4, 0, 2, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 3, 1, 2, 0, 3, 2, 1, 0, 0, 0, 0, 3, 0, 0, 3, 3,//Sorcerer
 3, 0, 2, 0, 0, 1, 0, 4, 0, 2, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 3, 1, 2, 0, 3, 2, 1, 0, 0, 0, 0, 3, 0, 0, 3, 3,
 3, 0, 2, 0, 0, 1, 0, 4, 0, 2, 0, 0, 4, 4, 4, 4, 0, 0, 0, 4, 0, 4, 1, 2, 0, 3, 2, 1, 0, 0, 0, 0, 4, 0, 0, 3, 3,
 3, 0, 2, 0, 0, 1, 0, 4, 0, 2, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 4, 4, 1, 2, 0, 3, 2, 1, 0, 0, 0, 0, 4, 0, 0, 3, 3,
}};
_UNKNOWN unk_4EDF40; // weak
std::array<unsigned int, 2> pHiredNPCsIconsOffsetsX = {489, 559};
std::array<unsigned int, 2> pHiredNPCsIconsOffsetsY = {152, 152};
std::array<int, 2> dword_4EE07C; // weak
_UNKNOWN unk_4EE084; // weak
std::array<__int16, 100> word_4EE088_sound_ids =
{{
      0, 10000, 10010, 10020, 10030, 10040, 10050, 10060, 10070, 10080,
  10090, 10100, 11000, 11010, 11020, 11030, 11040, 11050, 11060, 11070,
  11080, 11090, 11100, 12000, 12010, 12020, 12030, 12040, 12050, 12060,
  12070, 12080, 12090, 12100, 13000, 13010, 13020, 13030, 13040, 13050,
  13060, 13070, 13080, 13090, 13100, 14000, 14010, 14020, 14030, 14040,
  14050, 14060, 14070, 14080, 14090, 14100, 15000, 15010, 15020, 15030,
  15040, 15050, 15060, 15070, 15080, 15090, 15100, 16000, 16010, 16020,
  16030, 16040, 16050, 16060, 16070, 16080, 16090, 16100, 17000, 17010,
  17020, 17030, 17040, 17050, 17060, 17070, 17080, 17090, 17100, 18000,
  18010, 18020, 18030, 18040, 18050, 18060, 18070, 18080, 18090, 18100
}};
std::array<short, 28> word_4EE150 =
{{
   1,  2,  3,  4,  5,  7, 32, 33, 36, 37, 38, 40, 41, 42, 43, 45, 46,
  47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 60
}};

int dword_4EED78; // weak
_UNKNOWN unk_4EED80; // weak
int dword_4EFA80; // weak
int dword_4EFA84; // weak
void *off_4EFDB0; // weak
int dword_4F031C[777]; // weak
std::array<const char *, 19> off_4F03B8 =
{{
  "",         "WEPNTABL", "ARMORY",   "MAGSHELF",
  "MAGSHELF", "MAGSHELF", "MAGSHELF", "MAGSHELF",
  "MAGSHELF", "MAGSHELF", "MAGSHELF", "MAGSHELF",
  "MAGSHELF", "MAGSHELF", "MAGSHELF", "MAGSHELF",
  "MAGSHELF", "MAGSHELF", "MAGSHELF"
}};

std::array<int, 32> guild_mambership_flags={{
   54, 54, 54, 54, 52, 52, 52, 52, 55, 55, 55, 55, 53, 53,
	   53, 53, 58, 58, 58, 58, 57, 57, 57, 57, 56, 56,
     56, 56, 59, 59, 60, 60}};
std::array<__int16, 49> word_4F0754;
std::array<__int16, 54> _4F0882_evt_VAR_PlayerItemInHands_vals =
{{
	0x0D4, 0x270, 0x0D5, 0x21C, 0x0D6, 0x2BE, 0x0D7, 0x2BD, 0x0D8, 0x289, 
	0x0D9, 0x258, 0x0DA, 0x2AB, 0x0DB, 0x281, 0x0DC, 0x280, 0x0DD,
	0x284, 0x0DE, 0x285, 0x0DF, 0x283, 0x0E0, 0x282, 0x0E1, 0x27F,
	0x0E2, 0x1E7, 0x0E3, 0x287, 0x0E4, 0x272, 0x0E5, 0x267, 0x0E6,
	0x275, 0x0E7, 0x25A, 0x0E8, 0x2A4, 0x0E9, 0x2A5, 0x0EA, 0x2A3,
	0x0EB, 0x25C, 0x0EC, 0x25D, 0x0ED, 0x259, 0x0F1, 0x21E
}};
std::array<unsigned short, 6> pMaxLevelPerTrainingHallType = {5, 15, 25, 25, 200, 200};

std::array<int, 11> price_for_membership={100, 100, 50, 50, 50, 50, 50, 50, 50, 1000, 1000}; // weak


std::array<Vec2_int_, 20> pMonsterArenaPlacements;
std::array<__int16, 32> word_4F0F30 ={{ 4, 7, 10, 11,                                              
						   4, 7, 10, 11,
					       4, 7, 10, 11,
						   4, 7, 10, 11,
					       4, 7, 10, 11,
					       4, 7, 10, 11,
					       4, 7, 10, 11,
					       7, 11,
                 7, 11}};
double dbl_4F2870; // weak
int dword_4F288C; // weak
double dbl_4F5372; // weak
int dword_4F5428[777]; // weak
int dword_4F542C[777]; // weak
_UNKNOWN crtunk_4F54B8; // weak
std::array<int, 52> dword_4F5B24_ys; // idb
std::array<int, 52> dword_4F5BF4_xs; // idb
std::array<int, 52> dword_4F5CC8_ys; // idb
std::array<int, 52> dword_4F5D98_xs; // idb
std::array<int, 500> ai_array_4F5E68;
std::array<int, 500> ai_array_4F6638_actor_ids;
std::array<int, 500> ai_near_actors_targets_pid;
int ai_arrays_size; // weak
std::array<int, 500> ai_near_actors_distances;
std::array<unsigned int, 500> ai_near_actors_ids;
std::array<int, 182> dword_4F8580; // weak



_UNKNOWN unk_4FAA20; // weak
char byte_4FAA24; // weak
//HWND dword_4FAA28; // idb




int dword_505890; // weak
std::array<unsigned int, 480> pSRZBufferLineOffsets;
int areWeLoadingTexture; // weak
std::array<char, 777> books_num_items_per_page; // weak
int dword_506338; // weak
int dword_50633C; // idb
signed int sRecoveryTime; // idb
unsigned int uRequiredMana; // idb
int _506348_current_lloyd_playerid; // weak
__int64 qword_506350; // weak
char byte_506360; // weak
int dword_506364; // weak
//Texture *dword_506404[12]; // weak
//Texture *dword_50640C[12]; // weak
unsigned int uExitCancelTextureId;
int books_page_number; // weak
int books_primary_item_per_page; // weak
int Autonotes_Instructors_page_flag; // dword_50652C
int Autonotes_Misc_page_flag; //dword_506530
int Book_PageBtn6_flag; //dword_506534
int Book_PageBtn5_flag; //dword_506538
int Book_PageBtn4_flag; // dword_50653C
int Book_PageBtn3_flag; //dword_506540
int BtnDown_flag; //BtnDown_flag
int BtnUp_flag; //BtnUp_flag
int quick_spell_at_page; // weak
char byte_506550; // weak
std::array<const char *, 5> aMoonPhaseNames;
int _506568_autonote_type; // weak
char bRecallingBeacon; // weak
int uLastPointedObjectID; // weak
//unsigned __int8 bMonsterInfoUI_bDollInitialized;
std::array<const char *, 44> aSpellNames;
int pMainScreenNum; // weak
int dword_506980_uW; // weak
int dword_506984_uZ; // weak
int dword_506988_uY; // weak
int dword_50698C_uX; // weak
int uGameMenuUI_CurentlySelectedKeyIdx; // 506E68
std::array<unsigned int, 28> pPrevVirtualCidesMapping;
int KeyboardPageNum; // weak
int uRestUI_FoodRequiredToRest;
int dword_506F14; // weak
int _506F18_num_minutes_to_sleep; // weak
int dword_506F1C; // weak
std::array<int, 20> pVisibleWindowsIdxs; // weak
int uNumVisibleWindows;
char bFlashHistoryBook; // weak
char bFlashAutonotesBook; // weak
char bFlashQuestBook; // weak
int uTextureID_PlayerBuff_PainReflection; // weak
int uTextureID_PlayerBuff_Hammerhands; // weak
int uTextureID_PlayerBuff_Preservation; // weak
int uTextureID_PlayerBuff_Bless; // weak
int uTextureID_Btn_QuickReference; // weak
int uTextureID_Compas; // dword_5079B4
int dword_5079C8; // weak
int dword_5079CC; // weak
int dword_5079D0; // weak
int uTextureID_Minimap_Loop; // dword_5079D8
struct GUIButton *pBtn_ZoomOut; // idb
struct GUIButton *pBtn_ZoomIn; // idb
unsigned int uGameUIFontShadow;
unsigned int uGameUIFontMain;
int dword_507B00_spell_info_to_draw_in_popup; // weak
std::array<const char *, 12> aMonthNames;
std::array<const char *, 7> aDayNames;
std::array<const char *, 9> aSpellSchoolNames;
std::array<const char *, 7> aAttributeNames;
int dword_507B94; // weak
unsigned int uActiveCharacter;
int dword_507BF0_is_there_popup_onscreen; // weak
int awards_scroll_bar_created; // weak
int dword_507CC0_activ_ch; // weak
__int64 GameUI_RightPanel_BookFlashTimer; // weak
int _507CD4_RestUI_hourglass_anim_controller; // weak
bool OpenedTelekinesis;
std::array<int, 50> dword_50B570; // weak
std::array<int, 50> dword_50B638; // weak
stru367 PortalFace;
/*int PortalFace.field_0; // weak
int PortalFace.field_38[777]; // idb
int PortalFace._view_transformed_ys[45];
int PortalFace.field_128[777];
int PortalFace._view_transformed_zs[45];
int PortalFace.field_218[777];
int PortalFace._view_transformed_xs[45];
int PortalFace._screen_space_y[777];
int PortalFace.field_3E4[777];
int PortalFace.field_2F0[2]; // idb
int PortalFace._ys[3 + 45];
int PortalFace._ys2[48]; // idb
int PortalFace._screen_space_x[777]; // idb
int PortalFace.field_3D4[777]; // weak
int PortalFace._xs[777]; // weak
int dword_50BAE8[777]; // weak
int PortalFace._xs2[3 + 45]; // weak
int PortalFace._xs3[48]; // weak*/
std::array<int, 100> dword_50BC10; // weak
std::array<int, 100> dword_50BDA0; // weak
std::array<int, 100> dword_50BF30; // weak
char town_portal_caster_id; // weak
int some_active_character; // weak
//_UNKNOWN unk_50C190; // weak
int dword_50C968; // weak
std::array<unsigned int, 5> pIconIDs_Turn;
unsigned int uIconID_TurnStop;
unsigned int uIconID_TurnHour;
int uIconID_CharacterFrame; // idb
unsigned int uIconID_TurnStart;
int dword_50C994 = 0; // weak
int dword_50C998_turnbased_icon_1A = 0; // weak
int uSpriteID_Spell11; // idb
bool _50C9A0_IsEnchantingInProgress; // weak
int _50C9A8_item_enchantment_timer = 0; // weak
int dword_50C9AC; // weak
int _50C9D0_AfterEnchClickEventId; // weak
int _50C9D4_AfterEnchClickEventSecondParam; // weak
int _50C9D8_AfterEnchClickEventTimeout; // weak
int dword_50C9DC; // weak
struct NPCData *ptr_50C9E0;
//int dword_50C9E8; // idb
//int dword_50C9EC[120]; // weak
int dword_50CDC8;
int bProcessorIsNotIntel; // weak
Vec3_int_ layingitem_vel_50FDFC;
char pStartingMapName[32]; // idb
std::array<unsigned __int8, 5> IsPlayerWearingWatersuit;
std::array<char, 54> party_has_equipment;
std::array<char, 17> byte_5111F6;

std::array<std::array<unsigned __int16, 137>, 117> pOdmMinimap;
unsigned int uNumBlueFacesInBLVMinimap;
std::array<unsigned __int16, 50> pBlueFacesInBLVMinimapIDs;
std::array<int, 14> pTextureIDs_PartyBuffIcons;
unsigned int uIconIdx_FlySpell;
unsigned int uIconIdx_WaterWalk;
int dword_576E28; // weak
int _576E2C_current_minimap_zoom; // weak
__int64 _5773B8_event_timer; // weak
int _5773C0_unused; // weak


int dword_591084; // weak
struct Actor *pDialogue_SpeakingActor;
unsigned int uDialogueType;
int sDialogue_SpeakingActorNPC_ID;
struct LevelDecoration *_591094_decoration;
std::array<char, 200> sHouseName; // idb
int uCurrentHouse_Animation; // weak
char *dword_591164_teleport_map_name; // idb
int dword_591168_teleport_speedz; // weak
int dword_59116C_teleport_directionx; // weak
int dword_591170_teleport_directiony; // weak
int dword_591174_teleportz; // weak
int dword_591178_teleporty; // weak
int dword_59117C_teleportx; // weak
std::array<std::array<char, 100>, 6> byte_591180; // idb
std::array<struct NPCData *, 7> HouseNPCData;//0 zero element holds standart house npc
GUIButton* HouseNPCPortraitsButtonsList[6];//dword_5913F4
struct Texture *pTexture_591428;
struct Texture *pTexture_outside; // idb
struct Texture *pTexture_Dialogue_Background;
_UNKNOWN unk_597F10; // weak
std::array<char, 2000> byte_5B0938;
int EvtTargetObj; // 0x5B5920
int _unused_5B5924_is_travel_ui_drawn = false; // 005B5924
int _5B65A8_npcdata_uflags_or_other; // weak
int _5B65AC_npcdata_fame_or_other; // weak
int _5B65B0_npcdata_rep_or_other; // weak
int _5B65B4_npcdata_loword_house_or_other; // weak
int _5B65B8_npcdata_hiword_house_or_other; // weak
int dword_5B65BC; // weak
int dword_5B65C0; // weak
int dword_5B65C4_cancelEventProcessing; // weak
int dword_5B65C8_timers_count; // weak
int npcIdToDismissAfterDialogue; // weak
signed int dword_5B65D0_dialogue_actor_npc_id; // weak
int dword_5C3418; // weak
int dword_5C341C; // weak
int _5C3420_pDecoration;
//std::array<char, 777> byte_5C3427; // weak
std::array<char, 200> GameUI_Footer_TimedString;
std::array<char, 200> pFooterString;
unsigned int GameUI_Footer_TimeLeft;
int bForceDrawFooter; // weak
int dword_5C35C0; // weak
int bDialogueUI_InitializeActor_NPC_ID; // weak
int dword_5C35C8; // weak
char *p2DEventsTXT_Raw;

int dword_5C35D4; // weak
std::array<const char *, 2> aAMPMNames;
char byte_5C45AF[777]; // weak

std::array<char, 4096> pFinalMessage; // idb
std::array<char, 2000> pTmpBuf;
std::array<char, 2000> pTmpBuf2;
std::array<char, 100> Journal_limitation_factor; // weak
int ui_current_text_color; // weak
__int64 qword_5C6DF0; // weak
int dword_5C6DF8; // weak
std::array<char, 104> item__getname_buffer; // idb
std::array<char *, 36> pClassDescriptions;
std::array<char *, 7> pAttributeDescriptions;
std::array<char *, 38> pGrandSkillDesc;
std::array<char *, 38> pMasterSkillDesc;
std::array<char *, 38> pExpertSkillDesc;
std::array<char *, 38> pNormalSkillDesc;
std::array<char *, 38> pSkillDesc;
char *pHealthPointsAttributeDescription;
char *pSpellPointsAttributeDescription;
char *pArmourClassAttributeDescription;
char *pPlayerConditionAttributeDescription; // idb
char *pFastSpellAttributeDescription;
char *pPlayerAgeAttributeDescription;
char *pPlayerLevelAttributeDescription;
char *pPlayerExperienceAttributeDescription;
char *pAttackBonusAttributeDescription;
char *pAttackDamageAttributeDescription;
char *pMissleBonusAttributeDescription;
char *pMissleDamageAttributeDescription;
char *pFireResistanceAttributeDescription;
char *pAirResistanceAttributeDescription;
char *pWaterResistanceAttributeDescription;
char *pEarthResistanceAttributeDescription;
char *pMindResistanceAttributeDescription;
char *pBodyResistanceAttributeDescription;
char *pSkillPointsAttributeDescription;
char *pClassTXT_Raw;
char *pStatsTXT_Raw;
char *pSkillDescTXT_Raw;

struct FactionTable *pFactionTable;
//std::array<char, 777> byte_5C8D1A; // weak

std::array<char, 777> byte_5E4C15; // weak
std::array<const char *, 14> pSomeItemsNames;

char *pMonstersTXT_Raw;
char *pMonsterPlacementTXT_Raw;
char *pSpellsTXT_Raw;
char *pMapStatsTXT_Raw;
char *pHostileTXT_Raw;
char *pPotionsTXT_Raw;
char *pPotionNotesTXT_Raw;

std::array<int, 777> _6807B8_level_decorations_ids; // idb
int _6807E0_num_decorations_with_sounds_6807B8; // weak
std::array<int, 3000> event_triggers;     // 6807E8
size_t num_event_triggers; // 6836C8
std::array<int, 64> dword_69B010;
float flt_69B138_dist; // weak
char byte_69BD41_unused; // weak
unsigned int uTextureID_x_u;
unsigned int uTextureID_LS_saveU;
unsigned int uTextureID_LS_loadU;
unsigned int uTextureID_AR_DN_DN;
unsigned int uTextureID_AR_UP_DN;
unsigned int uTextureID_LS_;
unsigned int uTextureID_x_d;
unsigned int uTextureID_save_up;
unsigned int uTextureID_load_up;
unsigned int uTextureID_loadsave;
_UNKNOWN _69FBB4_ptr_iterator_end; // weak
_UNKNOWN unk_6A0758; // weak
int pSaveListPosition; // weak
unsigned int uLoadGameUI_SelectedSlot;
HWND hInsertCDWindow; // idb
int uCPUSpeed; // weak
char cMM7GameCDDriveLetter; // idb
void *ptr_6A0D08;
int _6A0D0C_txt_lod_loading; // weak
int _6A0D10_txt_lod_loading__unused; // weak
MENU_STATE uCurrentMenuID;
unsigned int uGameState;
int uDefaultTravelTime_ByFoot; // weak
int day_attrib; // weak
int day_fogrange_1; // weak
int day_fogrange_2; // weak
struct TileTable *pTileTable; // idb
int texmapping_terrain_subdivsize; // weak
int texmapping_terrain_subdivpow2; // weak
int texmapping_building_subdivsize; // weak
int texmapping_building_subdivpow2; // weak
int mipmapping_building_mm1; // weak
int mipmapping_building_mm2; // weak
int mipmapping_building_mm3; // weak
int mipmapping_terrain_mm1; // weak
int mipmapping_terrain_mm2; // weak
int mipmapping_terrain_mm3; // weak
int outdoor_grid_band_1; // idb
int outdoor_grid_band_2; // idb
int outdoor_grid_band_3; // idb
char outdoor_day_top_r; // weak
char outdoor_day_top_g; // weak
char outdoor_day_top_b; // weak
char outdoor_day_bottom_r; // weak
char outdoor_day_bottom_g; // weak
char outdoor_day_bottom_b; // weak
char outdoor_night_top_r; // weak
char outdoor_night_top_g; // weak
char outdoor_night_top_b; // weak
char outdoor_night_bottom_r; // weak
char outdoor_night_bottom_g; // weak
char outdoor_night_bottom_b; // weak
std::array<char, 777> pDefaultSkyTexture; // idb
std::array<char, 16> byte_6BE124_cfg_textures_DefaultGroundTexture; // idb
int _6BE134_odm_main_tile_group; // weak
int dword_6BE138; // weak
int dword_6BE13C_uCurrentlyLoadedLocationID; // weak
float fWalkSpeedMultiplier = 1.0f; // weak
float fBackwardWalkSpeedMultiplier = 1.0f; // weak
float fTurnSpeedMultiplier = 1.0f; // weak
float flt_6BE150_look_up_down_dangle = 1.0f; // weak
//HWND hWnd; // idb
class OSWindow *window = nullptr;
int dword_6BE340; // weak
char pCurrentMapName[32]; // idb
unsigned int uLevelMapStatsID;
int dword_6BE364_game_settings_1 = 0; // weak
int dword_6BE368_debug_settings_2 = 0; // weak
unsigned __int8 bUseLoResSprites = false;
unsigned __int8 bUseRegistry = true;
unsigned __int8 bCanLoadFromCD = false;
int bShowDamage; // idb
unsigned int bAlwaysRun;
unsigned int bFlipOnExit;
int dword_6BE384_2dacceloff; // weak
char byte_6BE388_graphicsmode; // weak
unsigned int uTurnSpeed;
float flt_6BE3A0; // weak
float flt_6BE3A4_debug_recmod1;
float flt_6BE3A8_debug_recmod2;
float flt_6BE3AC_debug_recmod1_x_1_6;
std::array<char, 20> byte_6BE3B0; // idb
char bUnderwater = false; // weak
char bNoNPCHiring = false; // weak
int _702AC0_unused = 0; // weak
int _702AC4_unused = 0; // weak
char _702ACC_unused = 0; // weak
int bDebugResouces; // weak
unsigned int bNoVideo = false;
bool bNoIntro = false;
bool bNoLogo = false;
bool bNoCD = false;
bool bNoSound = false;
std::array<int, 100> dword_720020_zvalues;
std::array<int, 299> dword_7201B0_zvalues;
int uTextureID_720980; // weak
int _720984_unused; // weak
char _72098C_unused; // weak
std::array<__int16, 104> word_7209A0_intercepts_ys_plus_ys;
std::array<__int16, 104> word_720A70_intercepts_xs_plus_xs;
std::array<__int16, 104> word_720B40_intercepts_zs;
std::array<__int16, 102> word_720C10_intercepts_xs;
int dword_720CDC;
std::array<__int16, 777> word_720CE0_ys; // idb
std::array<__int16, 777> word_720DB0_xs; // idb
std::array<int, 20> dword_720E80;
std::array<int, 20> dword_720ED0;
std::array<int, 20> dword_720F20;
std::array<__int16, 777> word_720F70; // idb
std::array<__int16, 777> word_721040; // idb
std::array<int, 777> dword_721110; // idb
std::array<int, 777> dword_721160; // idb
std::array<int, 20> odm_floor_level; // idb
int blv_prev_party_x; // weak
int blv_prev_party_z; // weak
int blv_prev_party_y; // weak
char *dword_721660; // idb
char *dword_721664; // idb
std::array<NPCTopic, 789> pNPCTopics;
char *dword_722F10; // idb
std::array<const char *, 513> pQuestTable;
_UNKNOWN unk_723714; // weak
char *dword_723718_autonote_related; // idb
int dword_72371C[777]; // weak
std::array<const char *, 82> pScrolls;
int dword_723E80_award_related[777]; // weak
int dword_723E84[777]; // weak
int dword_7241C8; // weak
struct unk_F7B60C stru_73C834; // struct @ MM7.exe::0073C834

std::array<const char *, 59> aNPCProfessionNames;
char *pAwardsTXT_Raw;
char *pScrollsTXT_Raw;
char *pMerchantsTXT_Raw;
std::array<const char *, 7> pMerchantsBuyPhrases;
std::array<const char *, 7> pMerchantsSellPhrases;
std::array<const char *, 7> pMerchantsRepairPhrases;
std::array<const char *, 7> pMerchantsIdentifyPhrases;
char *pTransitionsTXT_Raw;
char *pAutonoteTXT_Raw;
char *pQuestsTXT_Raw;
 unsigned int    uNumTerrainNormals;
 struct Vec3_float_ *pTerrainNormals;
 std::array<unsigned short, 128 * 128 * 2>  pTerrainNormalIndices;
 std::array<unsigned int, 128 * 128 * 2>    pTerrainSomeOtherData;
struct unk_F7B60C stru_76D578; // struct @ MM7.exe::0076D578
struct unk_F7B60C stru_76D590; // struct @ MM7.exe::0076D590
struct unk_F7B60C stru_76D5A8; // struct @ MM7.exe::0076D5A8
char byte_76D5C0; // weak
std::array<int, 128> terrain_76D5C8;
std::array<int, 128> terrain_76D7C8;
std::array<int, 128> terrain_76D9C8;
std::array<int, 128> terrain_76DBC8;
std::array<int, 128> terrain_76DDC8;
std::array<int, 128> terrain_76DFC8;
std::array<int, 128> terrain_76E1C8;
std::array<int, 128> terrain_76E3C8;

_UNKNOWN unk_801A00; // weak
_UNKNOWN unk_801A0C; // weak
char byte_80AA10; // weak
int dword_80AA14; // weak
int dword_80AA18; // weak
int dword_80AA1C; // weak
int dword_80AA20; // weak
unsigned int uNumElementsIn80AA28;
std::array<struct Polygon *, 2000> ptr_80AA28;
_UNKNOWN unk_80D190; // weak
int dword_A74C88; // weak
unsigned int uPlayerCreationUI_SkySliderPos;
int uPlayerCreationUI_ArrowAnim;
unsigned int uPlayerCreationUI_SelectedCharacter;
int dword_A74CDC; // weak
struct Texture *pTexture_PlayerFaceMask;
struct Texture *pTexture_PlayerFaceEradicated;
struct Texture *pTexture_PlayerFaceDead;
std::array< std::array<struct Texture *, 56>, 4> pTextures_PlayerFaces;
NZIArray<struct Player *, 5> pPlayers;
__int64 qword_A750D8; // weak
enum PlayerSpeech PlayerSpeechID;
int uSpeakingCharacter; // weak
std::array<const char *, 36> pClassNames;
std::array<const char *, 19> aCharacterConditionNames;
std::array<const char *, 38> pSkillNames;
int dword_AE336C; // weak
int dword_AE3370; // weak
char byte_AE5B91; // weak
std::array<int, 32> dword_F1B430; // weak
//int dword_F8B144; // nexindex [-1] to the following
std::array<int, 4> player_levels = {{1, 1, 1, 1}};
std::array<__int16, 6> weapons_Ypos; // word_F8B158
struct Texture *ShopTexture; // idb
std::array<struct Texture *, 12> ItemsInShopTexture;
__int16 bountyHunting_monster_id_for_hunting; // word_F8B1A0
const char *bountyHunting_text; // word_F8B1A4
int contract_approved; // weak
int dword_F8B1AC_award_bit_number; // idb
int dword_F8B1B0; // weak
int gold_transaction_amount; // F8B1B4
std::array<char *, 4> pShopOptions;
_UNKNOWN unk_F8B1C8; // weak
int dword_F8B1D8; // weak
int dword_F8B1DC; // weak
int dword_F8B1E0; // weak
int dword_F8B1E4; // weak
const char *current_npc_text; // idb
char dialogue_show_profession_details = false; // F8B1EC
std::array<char, 777> byte_F8B1EF; // weak
std::array<char, 4> byte_F8B1F0;
int dword_F8B1F4; // weak


//_UNKNOWN unk_F8BA50; // weak
char byte_F8BC0C; // weak
int bGameoverLoop = 0; // weak
std::array<__int16, 104> intersect_face_vertex_coords_list_a; // word_F8BC48
std::array<__int16, 104> intersect_face_vertex_coords_list_b; // word_F8BD18
int dword_F93F20; // weak
int dword_F93F70; // weak
