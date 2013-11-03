#pragma once
#include <string>
#include "VectorTypes.h"
#include "OSAPI.h"
#include <array>
#include <assert.h>
#include "NZIArray.h"

typedef char _UNKNOWN;
typedef unsigned int uint;

















#if defined(__GNUC__)
  typedef          long long ll;
  typedef unsigned long long ull;
  #define __int64 long long
  #define __int32 int
  #define __int16 short
  #define __int8  char
  #define MAKELL(num) num ## LL
  #define FMT_64 "ll"
#elif defined(_MSC_VER)
  typedef          __int64 ll;
  typedef unsigned __int64 ull;
  #define MAKELL(num) num ## i64
  #define FMT_64 "I64"
#elif defined (__BORLANDC__)
  typedef          __int64 ll;
  typedef unsigned __int64 ull;
  #define MAKELL(num) num ## i64
  #define FMT_64 "L"
#else
  #error "unknown compiler"
#endif
typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;

typedef          char   int8;
typedef   signed char   sint8;
typedef unsigned char   uint8;
typedef          short  int16;
typedef   signed short  sint16;
typedef unsigned short  uint16;
typedef          int    int32;
typedef   signed int    sint32;
typedef unsigned int    uint32;
typedef ll              int64;
typedef ll              sint64;
typedef ull             uint64;

// Partially defined types:
#define _BYTE  uint8
#define _WORD  uint16
#define _DWORD uint32
#define _QWORD uint64
#if !defined(_MSC_VER)
#define _LONGLONG __int128
#endif

#ifndef _WINDOWS_
typedef int8 BYTE;
typedef int16 WORD;
typedef int32 DWORD;
typedef int32 LONG;
#endif
typedef int64 QWORD;
#ifndef __cplusplus
typedef int bool;       // we want to use bool in our C programs
#endif

// Some convenience macros to make partial accesses nicer
// first unsigned macros:
#ifdef HIBYTE
#undef HIBYTE
#endif // HIBYTE
#ifdef HIWORD
#undef HIWORD
#endif // HIWORD
#ifdef LOBYTE
#undef LOBYTE
#endif // LOBYTE
#ifdef LOWORD
#undef LOWORD
#endif // LOWORD

#define LOBYTE(x)   (*((_BYTE*)&(x)))   // low byte
#define LOWORD(x)   (*((_WORD*)&(x)))   // low word
#define LODWORD(x)  (*((_DWORD*)&(x)))  // low dword
#define HIBYTE(x)   (*((_BYTE*)&(x)+1))
#define HIWORD(x)   (*((_WORD*)&(x)+1))
#define HIDWORD(x)  (*((_DWORD*)&(x)+1))
#define BYTEn(x, n)   (*((_BYTE*)&(x)+n))
#define WORDn(x, n)   (*((_WORD*)&(x)+n))
#define BYTE1(x)   BYTEn(x,  1)         // byte 1 (counting from 0)
#define BYTE2(x)   BYTEn(x,  2)
#define BYTE3(x)   BYTEn(x,  3)
#define BYTE4(x)   BYTEn(x,  4)
#define BYTE5(x)   BYTEn(x,  5)
#define BYTE6(x)   BYTEn(x,  6)
#define BYTE7(x)   BYTEn(x,  7)
#define BYTE8(x)   BYTEn(x,  8)
#define BYTE9(x)   BYTEn(x,  9)
#define BYTE10(x)  BYTEn(x, 10)
#define BYTE11(x)  BYTEn(x, 11)
#define BYTE12(x)  BYTEn(x, 12)
#define BYTE13(x)  BYTEn(x, 13)
#define BYTE14(x)  BYTEn(x, 14)
#define BYTE15(x)  BYTEn(x, 15)
#define WORD1(x)   WORDn(x,  1)
#define WORD2(x)   WORDn(x,  2)         // third word of the object, unsigned
#define WORD3(x)   WORDn(x,  3)
#define WORD4(x)   WORDn(x,  4)
#define WORD5(x)   WORDn(x,  5)
#define WORD6(x)   WORDn(x,  6)
#define WORD7(x)   WORDn(x,  7)

// now signed macros (the same but with sign extension)
#define SLOBYTE(x)   (*((int8*)&(x)))
#define SLOWORD(x)   (*((int16*)&(x)))
#define SLODWORD(x)  (*((int32*)&(x)))
#define SHIBYTE(x)   (*((int8*)&(x)+1))
#define SHIWORD(x)   (*((int16*)&(x)+1))
#define SHIDWORD(x)  (*((int32*)&(x)+1))
#define SBYTEn(x, n)   (*((int8*)&(x)+n))
#define SWORDn(x, n)   (*((int16*)&(x)+n))
#define SBYTE1(x)   SBYTEn(x,  1)
#define SBYTE2(x)   SBYTEn(x,  2)
#define SBYTE3(x)   SBYTEn(x,  3)
#define SBYTE4(x)   SBYTEn(x,  4)
#define SBYTE5(x)   SBYTEn(x,  5)
#define SBYTE6(x)   SBYTEn(x,  6)
#define SBYTE7(x)   SBYTEn(x,  7)
#define SBYTE8(x)   SBYTEn(x,  8)
#define SBYTE9(x)   SBYTEn(x,  9)
#define SBYTE10(x)  SBYTEn(x, 10)
#define SBYTE11(x)  SBYTEn(x, 11)
#define SBYTE12(x)  SBYTEn(x, 12)
#define SBYTE13(x)  SBYTEn(x, 13)
#define SBYTE14(x)  SBYTEn(x, 14)
#define SBYTE15(x)  SBYTEn(x, 15)
#define SWORD1(x)   SWORDn(x,  1)
#define SWORD2(x)   SWORDn(x,  2)
#define SWORD3(x)   SWORDn(x,  3)
#define SWORD4(x)   SWORDn(x,  4)
#define SWORD5(x)   SWORDn(x,  5)
#define SWORD6(x)   SWORDn(x,  6)
#define SWORD7(x)   SWORDn(x,  7)



// Generate a reference to pair of operands
template<class T>  int16 __PAIR__( int8  high, T low) { return ((( int16)high) << sizeof(high)*8) | uint8(low); }
template<class T>  int32 __PAIR__( int16 high, T low) { return ((( int32)high) << sizeof(high)*8) | uint16(low); }
template<class T>  int64 __PAIR__( int32 high, T low) { return ((( int64)high) << sizeof(high)*8) | uint32(low); }
template<class T> uint16 __PAIR__(uint8  high, T low) { return (((uint16)high) << sizeof(high)*8) | uint8(low); }
template<class T> uint32 __PAIR__(uint16 high, T low) { return (((uint32)high) << sizeof(high)*8) | uint16(low); }
template<class T> uint64 __PAIR__(uint32 high, T low) { return (((uint64)high) << sizeof(high)*8) | uint32(low); }

// rotate left
template<class T> T __ROL__(T value, uint count)
{
  const uint nbits = sizeof(T) * 8;
  count %= nbits;

  T high = value >> (nbits - count);
  value <<= count;
  value |= high;
  return value;
}

// rotate right
template<class T> T __ROR__(T value, uint count)
{
  const uint nbits = sizeof(T) * 8;
  count %= nbits;

  T low = value << (nbits - count);
  value >>= count;
  value |= low;
  return value;
}

// carry flag of left shift
template<class T> int8 __MKCSHL__(T value, uint count)
{
  const uint nbits = sizeof(T) * 8;
  count %= nbits;

  return (value >> (nbits-count)) & 1;
}

// carry flag of right shift
template<class T> int8 __MKCSHR__(T value, uint count)
{
  return (value >> (count-1)) & 1;
}

// sign flag
template<class T> int8 __SETS__(T x)
{
  if ( sizeof(T) == 1 )
    return int8(x) < 0;
  if ( sizeof(T) == 2 )
    return int16(x) < 0;
  if ( sizeof(T) == 4 )
    return int32(x) < 0;
  return int64(x) < 0;
}

// overflow flag of subtraction (x-y)
template<class T, class U> int8 __OFSUB__(T x, U y)
{
  if ( sizeof(T) < sizeof(U) )
  {
    U x2 = x;
    int8 sx = __SETS__(x2);
    return (sx ^ __SETS__(y)) & (sx ^ __SETS__(x2-y));
  }
  else
  {
    T y2 = y;
    int8 sx = __SETS__(x);
    return (sx ^ __SETS__(y2)) & (sx ^ __SETS__(x-y2));
  }
}

// overflow flag of addition (x+y)
template<class T, class U> int8 __OFADD__(T x, U y)
{
  if ( sizeof(T) < sizeof(U) )
  {
    U x2 = x;
    int8 sx = __SETS__(x2);
    return ((1 ^ sx) ^ __SETS__(y)) & (sx ^ __SETS__(x2+y));
  }
  else
  {
    T y2 = y;
    int8 sx = __SETS__(x);
    return ((1 ^ sx) ^ __SETS__(y2)) & (sx ^ __SETS__(x+y2));
  }
}

// carry flag of subtraction (x-y)
template<class T, class U> int8 __CFSUB__(T x, U y)
{
  int size = sizeof(T) > sizeof(U) ? sizeof(T) : sizeof(U);
  if ( size == 1 )
    return uint8(x) < uint8(y);
  if ( size == 2 )
    return uint16(x) < uint16(y);
  if ( size == 4 )
    return uint32(x) < uint32(y);
  return uint64(x) < uint64(y);
}

// carry flag of addition (x+y)
template<class T, class U> int8 __CFADD__(T x, U y)
{
  int size = sizeof(T) > sizeof(U) ? sizeof(T) : sizeof(U);
  if ( size == 1 )
    return uint8(x) > uint8(x+y);
  if ( size == 2 )
    return uint16(x) > uint16(x+y);
  if ( size == 4 )
    return uint32(x) > uint32(x+y);
  return uint64(x) > uint64(x+y);
}


//-------------------------------------------------------------------------
// Data declarations

extern int game_starting_year;
extern int game_viewport_width;
extern int game_viewport_height;
extern int game_viewport_x;
extern int game_viewport_y;
extern int game_viewport_z;
extern int game_viewport_w;

extern int pWindowList_at_506F50_minus1_indexing[1];
extern int dword_4C9890[10]; // weak
extern int dword_4C9920[16]; // weak
extern _UNKNOWN unk_4D8548; // weak
extern char byte_4D864C; // weak
extern float flt_4D86CC; // weak
extern int dword_4D86D8; // weak
extern int dword_4DAFCC; // weak
extern int (__stdcall *off_4DAFDC)(char); // weak
extern char asc_4DB724[]; // idb
extern int dword_4DBD94; // weak
extern int dword_4DF390; // weak
extern char Str2[]; // idb


extern float flt_4D84E8;


extern char aIxf[4]; // idb
extern _UNKNOWN unk_4E19FC; // weak
extern char aD[]; // idb
extern char asc_4E1AB0[]; // idb
extern char aWb_0[]; // idb
extern char a24bitPcxOnly[]; // idb
extern char Mode[]; // idb
extern char a16bitPcx[]; // idb
extern char aUnableToLoadS[]; // idb
extern char aBitmaps[]; // idb
extern char aIcons[]; // idb
extern char aPending[]; // idb
extern char aCanTFindS[]; // idb

extern __int16 word_4E1D3A[]; // weak

//extern std::array<char, 7> aSbwb00; // weak
extern char aW[2]; // idb
extern char aA[2]; // idb
extern char aSD[]; // idb
extern char aSS03d03dS000S0[]; // idb
extern char aLuS[]; // idb
extern char aS_1[]; // idb
extern char aSbSc02d[]; // idb
extern char aSbSs02d[]; // idb
extern char aTabDb[]; // idb
extern char aTabDa[]; // idb
extern char aIbM6D[]; // idb
extern char aIbM6U[]; // idb
extern char aIbM5D[]; // idb
extern char aIbM5U[]; // idb
extern char aPagemask[]; // idb
extern char aBook[]; // idb
extern char aSpell_fnt[]; // idb
extern char aAutonote_fnt[]; // idb
extern char aBook2_fnt[]; // idb
extern char pFontFile[]; // idb
extern char aFontpal[]; // idb
extern char aMapbordr[]; // idb
extern char aLloydDD_pcx[]; // idb
extern char aDataLloydDD_pc[]; // idb
extern char aLb_bordr[]; // idb
extern char aTphell[]; // idb
extern char aTpheaven[]; // idb
extern char aTpisland[]; // idb
extern char aTpwarlock[]; // idb
extern char aTpelf[]; // idb
extern char aTpharmndy[]; // idb
extern char aTownport[]; // idb
extern char aSbquiknot[]; // idb
extern char aTabAn8a[]; // idb
extern char aTabAn8b[]; // idb
extern char aTabAn4a[]; // idb
extern char aTabAn4b[]; // idb
extern char aTabAn5a[]; // idb
extern char aTabAn5b[]; // idb
extern char aTabAn3a[]; // idb
extern char aTabAn3b[]; // idb
extern char aTabAn2a[]; // idb
extern char aTabAn2b[]; // idb
extern char aTabAn1a[]; // idb
extern char aTabAn1b[]; // idb
extern char aDivbar[]; // idb
extern char aSbautnot[]; // idb
extern char aTabwoff[]; // idb
extern char aTabwon[]; // idb
extern char aTabeoff[]; // idb
extern char aTabeon[]; // idb
extern char aTabsoff[]; // idb
extern char aTabson[]; // idb
extern char aTabnoff[]; // idb
extern char aTabnon[]; // idb
extern char aZootOff[]; // idb
extern char aZoomOff[]; // idb
extern char aZootOn[]; // idb
extern char aZoomOn[]; // idb
extern char aSbmap[]; // idb
extern char aMoon_ful[]; // idb
extern char aMoon_2[]; // idb
extern char aMoon_4[]; // idb
extern char aMoon_new[]; // idb
extern char aSbdateTime[]; // idb
extern char aTabAn7a[]; // idb
extern char aTabAn6a[]; // idb
extern char aTabAn7b[]; // idb
extern char aTabAn6b[]; // idb
extern char aSbplayrnot[]; // idb
extern char aPending_0[]; // idb
extern char aUnknown[8]; // weak
extern char aS100110S[]; // idb
extern char aS100110D[]; // idb
extern char aS100110DS[]; // idb
extern char aS100110D02dSS[]; // idb
extern int pCurrentScreen; // 004E28F8
extern unsigned int uGammaPos;
extern std::array<int, 8> BtnTurnCoord;
extern std::array<__int16, 4> RightClickPortraitXmin;
extern std::array<__int16, 4> RightClickPortraitXmax;
extern void *off_4E2A12; // weak
//extern int pArmorSkills[5];
//extern int pWeaponSkills[9];
//extern int pMiscSkills[12];
//extern int pMagicSkills[9];
extern std::array<unsigned int, 4> pHealthBarPos;
extern std::array<unsigned int, 4> pManaBarPos;
//extern std::array<char, 80> _4E2B21_buff_spell_tooltip_colors;
extern std::array<char, 88>  monster_popup_y_offsets; // weak
extern unsigned char hourglass_icon_idx; // weak


extern const char *format_4E2D80;
extern char format_4E2D90[8];
extern const char *format_4E2DE8; // idb
extern const char *format_4E2E00; // idb
extern const char *format_4E2E10; // format text of resistance in Stats screen
extern __int16 word_4E3C66[]; // idb
extern int dword_4E455C; // weak
extern std::array<int, 6> dword_4E4560;
extern std::array<int, 6> dword_4E4578;
extern std::array<int, 6> dword_4E4590;
extern std::array<int, 6> dword_4E45A8;
extern _UNKNOWN dword_4E49D4; // idb
extern int dword_4E4A18[]; // weak
extern int dword_4E4A1C[]; // weak
extern int dword_4E4A40[]; // weak
extern int dword_4E4A44[]; // weak
extern std::array<float, 10> flt_4E4A80;

extern std::array< std::array<int, 2>, 14> pPartySpellbuffsUI_XYs;
extern std::array<unsigned char, 14> byte_4E5DD8; // weak
extern std::array<unsigned __int8, 14> pPartySpellbuffsUI_smthns;
extern std::array< std::array<int, 6>, 6> pNPCPortraits_x; // 004E5E50
extern std::array< std::array<int, 6>, 6> pNPCPortraits_y; // 004E5EE0
extern std::array<const char *, 11> pHouse_ExitPictures;
extern std::array<const char *, 11> _4E6BDC_loc_names;
extern int bWinNT4_0; // weak
extern std::array<__int16, 11> word_4E8152;
extern std::array< std::array<char, 14>, 7> byte_4E8168;
//extern stru355 stru_4E82A4;// = {0x20, 0x41, 0, 0x20, 0xFF0000, 0xFF00, 0xFF, 0xFF000000};  moved to texture.h
//extern stru355 stru_4EFCBC;// = {0x20, 0x41, 0, 0x10, 0x7C00, 0x3E0, 0x1F, 0x8000};  moved to texture.h
extern char byte_4E94D0; // weak
extern char _4E94D2_light_type; // weak
extern char byte_4E94D3; // weak
extern std::array<unsigned int, 2> saveload_dlg_xs;
extern std::array<unsigned int, 2> saveload_dlg_ys;
extern std::array<unsigned int, 2> saveload_dlg_zs;
extern std::array<unsigned int, 2> saveload_dlg_ws;
extern int dword_4E98BC_bApplicationActive; // weak
extern std::array<char*, 465> pTransitionStrings; // 4EB080
extern std::array<int, 9> dword_4EC268; // weak
extern std::array<int, 7> dword_4EC28C; // weak
extern int dword_4EC2A8; // weak
extern int dword_4EC2AC; // weak
extern std::array<const char*, 25> pPlayerPortraitsNames;

extern std::array< std::array<unsigned char, 25>, 48> byte_4ECF08;
extern std::array<std::array<unsigned char, 8>, 110> SoundSetAction; // weak
extern std::array<__int16, 4> pPlayerPortraitsXCoords_For_PlayerBuffAnimsDrawing;
extern std::array< std::array<char, 37>, 36> byte_4ED970_skill_learn_ability_by_class_table;
extern _UNKNOWN unk_4EDF40; // weak
extern std::array<unsigned int, 2> pHiredNPCsIconsOffsetsX;
extern std::array<unsigned int, 2> pHiredNPCsIconsOffsetsY;
extern std::array<int, 2> dword_4EE07C; // weak
extern _UNKNOWN unk_4EE084; // weak
extern std::array<__int16, 100> word_4EE088_sound_ids; // weak
extern std::array<short, 28> word_4EE150;
extern int dword_4EED78; // weak
extern _UNKNOWN unk_4EED80; // weak
extern int dword_4EFA80; // weak
extern int dword_4EFA84; // weak
extern void *off_4EFDB0; // weak
extern int dword_4F031C[]; // weak
extern std::array<const char *, 19> off_4F03B8; // idb
extern __int16 word_4F0576[]; // weak

extern std::array<int, 32> guild_mambership_flags;
extern std::array<__int16, 49> word_4F0754;
extern std::array<__int16, 54> _4F0882_evt_VAR_PlayerItemInHands_vals;
extern std::array<unsigned short, 6> pMaxLevelPerTrainingHallType;
extern std::array<int, 11> price_for_membership; // weak
extern std::array<Vec2_int_, 20> pMonsterArenaPlacements;
extern std::array<__int16, 32> word_4F0F30; // weak
extern double dbl_4F2870; // weak
extern int dword_4F288C; // weak
extern double dbl_4F5372; // weak
extern int dword_4F5428[]; // weak
extern int dword_4F542C[]; // weak
extern _UNKNOWN crtunk_4F54B8; // weak
extern std::array<int, 52> dword_4F5B24_ys; // idb
extern std::array<int, 52> dword_4F5BF4_xs; // idb
extern std::array<int, 52> dword_4F5CC8_ys; // idb
extern std::array<int, 52> dword_4F5D98_xs; // idb
extern std::array<int, 500> ai_array_4F5E68;
extern std::array<int, 500> ai_array_4F6638_actor_ids;
extern std::array<int, 500> ai_near_actors_targets_pid;
extern int ai_arrays_size; // weak
extern std::array<int, 500> ai_near_actors_distances;
extern std::array<unsigned int, 500> ai_near_actors_ids;
extern std::array<int, 182> dword_4F8580; // weak
extern int dword_4FA9B0[]; // weak
extern int dword_4FA9B4[]; // weak

extern _UNKNOWN unk_4FAA20; // weak
extern char byte_4FAA24; // weak
//extern HWND dword_4FAA28; // idb



extern int dword_505890; // weak
extern std::array<unsigned int, 480> pSRZBufferLineOffsets;
extern int areWeLoadingTexture; // weak
extern std::array<char, 777> books_num_items_per_page; // weak
extern int dword_506338; // weak
extern int dword_50633C; // idb
extern signed int sRecoveryTime; // idb
extern unsigned int uRequiredMana; // idb
extern int _506348_current_lloyd_playerid; // weak
extern __int64 qword_506350; // weak
extern char byte_506360; // weak
extern int dword_506364; // weak
//extern Texture *dword_50640C[]; // weak
extern unsigned int uExitCancelTextureId;
extern int books_page_number; // number for page in books(номер страницы в книгах)
extern int books_primary_item_per_page; // number primary item in book page(номер начальной записи на странице)
extern int Autonotes_Instructors_page_flag; // dword_50652C
extern int Autonotes_Misc_page_flag; //dword_506530
extern int Book_PageBtn6_flag; //dword_506534
extern int Book_PageBtn5_flag; //dword_506538
extern int Book_PageBtn4_flag; //dword_50653C
extern int Book_PageBtn3_flag; //dword_506540
extern int BtnDown_flag; //BtnDown_flag
extern int BtnUp_flag; //BtnUp_flag
extern int quick_spell_at_page; // weak
extern char byte_506550; // weak
extern std::array<const char *, 5> aMoonPhaseNames;
extern int _506568_autonote_type; // 506568
extern char bRecallingBeacon; // weak
extern int uLastPointedObjectID; // weak
//extern unsigned __int8 bMonsterInfoUI_bDollInitialized;
extern std::array<const char *, 44> aSpellNames;
extern int pMainScreenNum; // weak
extern int dword_506980_uW; // weak
extern int dword_506984_uZ; // weak
extern int dword_506988_uY; // weak
extern int dword_50698C_uX; // weak
extern int uGameMenuUI_CurentlySelectedKeyIdx; // 506E68
extern std::array<bool, 28> GameMenuUI_InvaligKeyBindingsFlags; // 506E6C
extern std::array<unsigned int, 28> pPrevVirtualCidesMapping;
extern int KeyboardPageNum; // weak
extern int uRestUI_FoodRequiredToRest;
extern int dword_506F14; // weak
extern int _506F18_num_minutes_to_sleep; // weak
extern int dword_506F1C; // weak
extern std::array<int, 20> pVisibleWindowsIdxs; // weak
extern int uNumVisibleWindows;
extern char bFlashHistoryBook; // weak
extern char bFlashAutonotesBook; // weak
extern char bFlashQuestBook; // weak
extern int uTextureID_PlayerBuff_PainReflection; // weak
extern int uTextureID_PlayerBuff_Hammerhands; // weak
extern int uTextureID_PlayerBuff_Preservation; // weak
extern int uTextureID_PlayerBuff_Bless; // weak
extern int uTextureID_Btn_QuickReference; // weak
extern int uTextureID_Compas; // dword_5079B4
extern int dword_5079C8; // weak
extern int dword_5079CC; // weak
extern int dword_5079D0; // weak
extern int uTextureID_Minimap_Loop; // dword_5079D8
extern struct GUIButton *pBtn_ZoomOut; // idb
extern struct GUIButton *pBtn_ZoomIn; // idb
extern unsigned int uGameUIFontShadow;
extern unsigned int uGameUIFontMain;
extern int dword_507B00_spell_info_to_draw_in_popup; // weak
extern std::array<const char *, 12> aMonthNames;
extern std::array<const char *, 7> aDayNames;
extern std::array<const char *, 9> aSpellSchoolNames;
extern std::array<const char *, 7> aAttributeNames;
extern int dword_507B94; // weak
extern unsigned int uActiveCharacter;
extern int dword_507BF0_is_there_popup_onscreen; // weak
extern int awards_scroll_bar_created; // weak
extern int dword_507CC0_activ_ch; // weak
extern __int64 GameUI_RightPanel_BookFlashTimer; // weak
extern int _507CD4_RestUI_hourglass_anim_controller; // weak
extern bool OpenedTelekinesis; // weak
extern std::array<int, 50> dword_50B570; // weak
extern std::array<int, 50> dword_50B638; // weak
extern struct stru367 PortalFace;
/*extern int PortalFace.field_0; // weak
extern int PortalFace.field_38[]; // idb
extern int PortalFace._view_transformed_ys[];
extern int PortalFace.field_128[];
extern int PortalFace._view_transformed_zs[];
extern int PortalFace.field_218[];
extern int PortalFace._view_transformed_xs[];
extern int PortalFace._screen_space_y[];
extern int PortalFace.field_3E4[];
extern int PortalFace.field_2F0[]; // idb
extern int PortalFace._ys[];
extern int PortalFace._ys2[]; // idb
extern int PortalFace._screen_space_x[]; // idb
extern int PortalFace.field_3D4[]; // weak
extern int PortalFace._xs[]; // weak
extern int dword_50BAE8[]; // weak
extern int PortalFace._xs2[]; // weak
extern int PortalFace._xs3[]; // weak*/
extern std::array<int, 100> dword_50BC10; // weak
extern std::array<int, 100> dword_50BDA0; // weak
extern std::array<int, 100> dword_50BF30; // weak
extern char town_portal_caster_id; // weak
extern int some_active_character; // weak
extern _UNKNOWN unk_50C190; // weak
extern int dword_50C968; // weak
extern std::array<unsigned int, 5> pIconIDs_Turn;
extern unsigned int uIconID_TurnStop;
extern unsigned int uIconID_TurnHour;
extern int uIconID_CharacterFrame; // idb
extern unsigned int uIconID_TurnStart;
extern int dword_50C994; // weak
extern int dword_50C998_turnbased_icon_1A; // weak
extern int uSpriteID_Spell11; // idb
extern bool _50C9A0_IsEnchantingInProgress; // weak
extern int _50C9A8_item_enchantment_timer; // weak
extern int dword_50C9AC; // weak
extern int _50C9D0_AfterEnchClickEventId; // weak
extern int _50C9D4_AfterEnchClickEventSecondParam; // weak
extern int _50C9D8_AfterEnchClickEventTimeout; // weak
extern int dword_50C9DC; // weak
extern struct NPCData *ptr_50C9E0;
//extern int dword_50C9E8; // idb
//extern int dword_50C9EC[]; // 50C9EC
extern int dword_50CDC8;
extern int bProcessorIsNotIntel; // weak
extern Vec3_int_ layingitem_vel_50FDFC;
extern char pStartingMapName[32]; // idb
extern std::array<unsigned __int8, 5> IsPlayerWearingWatersuit;
extern std::array<char, 54> party_has_equipment;
extern std::array<char, 17> byte_5111F6;
extern std::array<unsigned int, 16> papredoll_dbrds;

extern int bRingsShownInCharScreen; // weak

extern std::array<std::array<unsigned __int16, 137>, 117> pOdmMinimap;
extern unsigned int uNumBlueFacesInBLVMinimap;
extern std::array<unsigned __int16, 50> pBlueFacesInBLVMinimapIDs;
extern std::array<int, 14> pTextureIDs_PartyBuffIcons;
extern unsigned int uIconIdx_FlySpell;
extern unsigned int uIconIdx_WaterWalk;
extern int dword_576E28; // weak
extern int _576E2C_current_minimap_zoom; // weak
extern __int64 _5773B8_event_timer; // weak
extern int _5773C0_unused; // weak


extern int dword_591084; // weak
extern struct Actor *pDialogue_SpeakingActor;
extern unsigned int uDialogueType;
extern signed int sDialogue_SpeakingActorNPC_ID;
extern struct LevelDecoration *_591094_decoration;
extern std::array<char, 200> sHouseName; // idb
extern int uCurrentHouse_Animation; // weak
extern char *dword_591164_teleport_map_name; // idb
extern int dword_591168_teleport_speedz; // weak
extern int dword_59116C_teleport_directionx; // weak
extern int dword_591170_teleport_directiony; // weak
extern int dword_591174_teleportz; // weak
extern int dword_591178_teleporty; // weak
extern int dword_59117C_teleportx; // weak
extern std::array<std::array<char, 100>, 6> byte_591180; // idb
extern std::array<struct NPCData *, 7> HouseNPCData; //0this array size temporarily increased to 60 from 6 to work aroud house overflow
extern GUIButton* HouseNPCPortraitsButtonsList[6];
extern struct Texture *pTexture_591428;
extern struct Texture *pTexture_outside; // idb
extern struct Texture *pTexture_Dialogue_Background;
extern _UNKNOWN unk_597F10; // weak
extern std::array<char, 2000> byte_5B0938;
extern int EvtTargetObj; // weak
extern int _unused_5B5924_is_travel_ui_drawn; // 005B5924
extern int _5B65A8_npcdata_uflags_or_other; // weak
extern int _5B65AC_npcdata_fame_or_other; // weak
extern int _5B65B0_npcdata_rep_or_other; // weak
extern int _5B65B4_npcdata_loword_house_or_other; // weak
extern int _5B65B8_npcdata_hiword_house_or_other; // weak
extern int dword_5B65BC; // weak
extern int dword_5B65C0; // weak
extern int dword_5B65C4_cancelEventProcessing; // weak
extern int dword_5B65C8_timers_count; // weak
extern int npcIdToDismissAfterDialogue; // weak
extern int dword_5B65D0_dialogue_actor_npc_id; // weak
extern int dword_5C3418; // weak
extern int dword_5C341C; // weak
extern int _5C3420_pDecoration;
//extern std::array<char, 777> byte_5C3427; // weak
extern std::array<char, 200> GameUI_Footer_TimedString;
extern std::array<char, 200> pFooterString;
extern unsigned int GameUI_Footer_TimeLeft;
extern int bForceDrawFooter; // weak
extern int dword_5C35C0; // weak
extern int bDialogueUI_InitializeActor_NPC_ID; // weak
extern int dword_5C35C8; // weak
extern char *p2DEventsTXT_Raw;

extern int dword_5C35D4; // weak
extern std::array<const char *, 2> aAMPMNames;
extern char byte_5C45AF[]; // weak
extern std::array<char, 10000> pTmpBuf3;
extern std::array<char, 4096> pFinalMessage; // idb
extern std::array<char, 2000> pTmpBuf;
extern std::array<char, 2000> pTmpBuf2;
extern std::array<char, 100> Journal_limitation_factor; // weak
extern int ui_current_text_color; // weak
extern __int64 qword_5C6DF0; // weak
extern int dword_5C6DF8; // weak
extern std::array<char, 104> item__getname_buffer; // idb
extern std::array<char *, 36> pClassDescriptions;
extern std::array<char *, 7> pAttributeDescriptions;
extern std::array<char *, 38> pGrandSkillDesc;
extern std::array<char *, 38> pMasterSkillDesc;
extern std::array<char *, 38> pExpertSkillDesc;
extern std::array<char *, 38> pNormalSkillDesc;
extern std::array<char *, 38> pSkillDesc;
extern char *pHealthPointsAttributeDescription;
extern char *pSpellPointsAttributeDescription;
extern char *pArmourClassAttributeDescription;
extern char *pPlayerConditionAttributeDescription; // idb
extern char *pFastSpellAttributeDescription;
extern char *pPlayerAgeAttributeDescription;
extern char *pPlayerLevelAttributeDescription;
extern char *pPlayerExperienceAttributeDescription;
extern char *pAttackBonusAttributeDescription;
extern char *pAttackDamageAttributeDescription;
extern char *pMissleBonusAttributeDescription;
extern char *pMissleDamageAttributeDescription;
extern char *pFireResistanceAttributeDescription;
extern char *pAirResistanceAttributeDescription;
extern char *pWaterResistanceAttributeDescription;
extern char *pEarthResistanceAttributeDescription;
extern char *pMindResistanceAttributeDescription;
extern char *pBodyResistanceAttributeDescription;
extern char *pSkillPointsAttributeDescription;
extern char *pClassTXT_Raw;
extern char *pStatsTXT_Raw;
extern char *pSkillDescTXT_Raw;
extern struct FactionTable *pFactionTable;
//extern std::array<char, 777> byte_5C8D1A; // weak

extern std::array<char, 777> byte_5E4C15; // weak
extern std::array<const char *, 14> pSomeItemsNames;

extern char *pMonstersTXT_Raw;
extern char *pMonsterPlacementTXT_Raw;
extern char *pSpellsTXT_Raw;
extern char *pMapStatsTXT_Raw;
extern char *pHostileTXT_Raw;
extern char *pPotionsTXT_Raw;
extern char *pPotionNotesTXT_Raw;

extern std::array<int, 777> _6807B8_level_decorations_ids; // idb
extern int _6807E0_num_decorations_with_sounds_6807B8; // weak
extern std::array<int, 3000> event_triggers; // 6807E8
extern size_t num_event_triggers; // 6836C8
extern std::array<int, 64> dword_69B010;
extern float flt_69B138_dist; // weak
extern char byte_69BD41_unused; // weak
extern unsigned int uTextureID_x_u;
extern unsigned int uTextureID_LS_saveU;
extern unsigned int uTextureID_LS_loadU;
extern unsigned int uTextureID_AR_DN_DN;
extern unsigned int uTextureID_AR_UP_DN;
extern unsigned int uTextureID_LS_;
extern unsigned int uTextureID_x_d;
extern unsigned int uTextureID_save_up;
extern unsigned int uTextureID_load_up;
extern unsigned int uTextureID_loadsave;
extern _UNKNOWN _69FBB4_ptr_iterator_end; // weak
extern _UNKNOWN unk_6A0758; // weak
extern int pSaveListPosition; // weak
extern unsigned int uLoadGameUI_SelectedSlot;
extern HWND hInsertCDWindow; // idb
extern int uCPUSpeed; // weak
extern char cMM7GameCDDriveLetter; // idb
extern void *ptr_6A0D08;
extern int _6A0D0C_txt_lod_loading; // weak
extern int _6A0D10_txt_lod_loading__unused; // weak
extern enum MENU_STATE uCurrentMenuID;
extern unsigned int uGameState;
extern int uDefaultTravelTime_ByFoot; // weak
extern int day_attrib; // weak
extern int day_fogrange_1; // weak
extern int day_fogrange_2; // weak
extern struct TileTable *pTileTable; // idb
extern int texmapping_terrain_subdivsize; // weak
extern int texmapping_terrain_subdivpow2; // weak
extern int texmapping_building_subdivsize; // weak
extern int texmapping_building_subdivpow2; // weak
extern int mipmapping_building_mm1; // weak
extern int mipmapping_building_mm2; // weak
extern int mipmapping_building_mm3; // weak
extern int mipmapping_terrain_mm1; // weak
extern int mipmapping_terrain_mm2; // weak
extern int mipmapping_terrain_mm3; // weak
extern int outdoor_grid_band_1; // idb
extern int outdoor_grid_band_2; // idb
extern int outdoor_grid_band_3; // idb
extern char outdoor_day_top_r; // weak
extern char outdoor_day_top_g; // weak
extern char outdoor_day_top_b; // weak
extern char outdoor_day_bottom_r; // weak
extern char outdoor_day_bottom_g; // weak
extern char outdoor_day_bottom_b; // weak
extern char outdoor_night_top_r; // weak
extern char outdoor_night_top_g; // weak
extern char outdoor_night_top_b; // weak
extern char outdoor_night_bottom_r; // weak
extern char outdoor_night_bottom_g; // weak
extern char outdoor_night_bottom_b; // weak
extern std::array<char, 777> pDefaultSkyTexture; // idb
extern std::array<char, 16> byte_6BE124_cfg_textures_DefaultGroundTexture; // idb
extern int _6BE134_odm_main_tile_group; // weak
extern int dword_6BE138; // weak
extern int dword_6BE13C_uCurrentlyLoadedLocationID; // weak
extern float fWalkSpeedMultiplier; // weak
extern float fBackwardWalkSpeedMultiplier; // weak
extern float fTurnSpeedMultiplier; // weak
extern float flt_6BE150_look_up_down_dangle; // weak
//extern char pMM7WindowClassName[]; // idb
//extern HINSTANCE hInstance; // idb
//extern char *pCmdLine;
//extern HWND hWnd; // idb
extern class OSWindow *window;
extern int dword_6BE340; // weak
extern char pCurrentMapName[32]; // idb
extern unsigned int uLevelMapStatsID;
extern int dword_6BE364_game_settings_1; // weak
extern int dword_6BE368_debug_settings_2; // weak
extern unsigned __int8 bUseLoResSprites;
extern unsigned __int8 bUseRegistry;
extern unsigned __int8 bCanLoadFromCD;
extern int bShowDamage; // idb
extern unsigned int bAlwaysRun;
extern unsigned int bFlipOnExit;
extern int dword_6BE384_2dacceloff; // weak
extern char byte_6BE388_graphicsmode; // weak
extern unsigned int uTurnSpeed;
extern float flt_6BE3A0; // weak
extern float flt_6BE3A4_debug_recmod1;
extern float flt_6BE3A8_debug_recmod2;
extern float flt_6BE3AC_debug_recmod1_x_1_6;
extern std::array<char, 20> byte_6BE3B0; // idb
extern char bUnderwater; // weak
extern char bNoNPCHiring; // weak
extern int _702AC0_unused; // weak
extern int _702AC4_unused; // weak
extern char _702ACC_unused; // weak
extern int bDebugResouces; // weak
extern unsigned int bNoVideo;
extern bool bNoIntro;
extern bool bNoLogo;
extern bool bNoCD;
extern bool bNoSound;
extern std::array<int, 100> dword_720020_zvalues;
extern std::array<int, 299> dword_7201B0_zvalues;
extern int dword_7207F0[]; // idb
extern int uTextureID_720980; // weak
extern int _720984_unused; // weak
extern char _72098C_unused; // weak
extern std::array<__int16, 104> word_7209A0_intercepts_ys_plus_ys;
extern std::array<__int16, 104> word_720A70_intercepts_xs_plus_xs;
extern std::array<__int16, 104> word_720B40_intercepts_zs;
extern std::array<__int16, 102> word_720C10_intercepts_xs;
extern int dword_720CDC;
extern std::array<__int16, 777> word_720CE0_ys; // idb
extern std::array<__int16, 777> word_720DB0_xs; // idb
extern std::array<int, 20> dword_720E80;
extern std::array<int, 20> dword_720ED0;
extern std::array<int, 20> dword_720F20;
extern std::array<__int16, 777> word_720F70; // idb
extern std::array<__int16, 777> word_721040; // idb
extern std::array<int, 777> dword_721110; // idb
extern std::array<int, 777> dword_721160; // idb
extern std::array<int, 20> odm_floor_level; // idb dword_7211B0
extern int blv_prev_party_x; // weak
extern int blv_prev_party_z; // weak
extern int blv_prev_party_y; // weak
extern char *dword_721660; // idb
extern char *dword_721664; // idb
extern char *dword_722F10; // idb
extern std::array<const char *, 513> pQuestTable;
extern _UNKNOWN unk_723714; // weak
extern char *dword_723718_autonote_related; // idb
extern int dword_72371C[]; // weak
extern std::array<const char *, 82> pScrolls;
extern int dword_723E80_award_related[]; // weak
extern int dword_723E84[]; // weak
extern int dword_7241C8; // weak

extern std::array<const char *, 59> aNPCProfessionNames;
extern char *pAwardsTXT_Raw;
extern char *pScrollsTXT_Raw;
extern char *pMerchantsTXT_Raw;
extern std::array<const char *, 7> pMerchantsBuyPhrases;
extern std::array<const char *, 7> pMerchantsSellPhrases;
extern std::array<const char *, 7> pMerchantsRepairPhrases;
extern std::array<const char *, 7> pMerchantsIdentifyPhrases;
extern char *pTransitionsTXT_Raw;
extern char *pAutonoteTXT_Raw;
extern char *pQuestsTXT_Raw;
extern unsigned int    uNumTerrainNormals;
extern struct Vec3_float_ *pTerrainNormals;
extern std::array<unsigned short, 128 * 128 * 2>  pTerrainNormalIndices;
extern std::array<unsigned int, 128 * 128 * 2>    pTerrainSomeOtherData;
extern char byte_76D5C0;
extern struct unk_F7B60C stru_76D578;
extern struct unk_F7B60C stru_76D590;
extern struct unk_F7B60C stru_76D5A8;
extern std::array<int, 128> terrain_76D5C8;
extern std::array<int, 128> terrain_76D7C8;
extern std::array<int, 128> terrain_76D9C8;
extern std::array<int, 128> terrain_76DBC8;
extern std::array<int, 128> terrain_76DDC8;
extern std::array<int, 128> terrain_76DFC8;
extern std::array<int, 128> terrain_76E1C8;
extern std::array<int, 128> terrain_76E3C8;
struct unk_F7B60C
{
 int   some_data_ptr;  // dword @ 000000
 float flt_000004;
 float flt_000008;
 float flt_00000C;
 float flt_000010;
 char  byte_000014;
 char  unk_000015[3];
};
extern _UNKNOWN unk_801A00; // weak
extern _UNKNOWN unk_801A0C; // weak
extern char byte_80AA10; // weak
extern int dword_80AA14; // weak
extern int dword_80AA18; // weak
extern int dword_80AA1C; // weak
extern int dword_80AA20; // weak
extern unsigned int uNumElementsIn80AA28;
extern std::array<struct Polygon *, 2000> ptr_80AA28;
extern _UNKNOWN unk_80D190; // weak
extern int dword_A74C88; // weak
extern unsigned int uPlayerCreationUI_SkySliderPos;
extern int uPlayerCreationUI_ArrowAnim;
extern unsigned int uPlayerCreationUI_SelectedCharacter;
extern int dword_A74CDC; // weak
extern struct Texture *pTexture_PlayerFaceMask;
extern struct Texture *pTexture_PlayerFaceEradicated;
extern struct Texture *pTexture_PlayerFaceDead;
extern std::array< std::array<struct Texture *, 56>, 4> pTextures_PlayerFaces;
extern int dword_A75070; // weak
extern NZIArray<struct Player *, 5> pPlayers;
extern __int64 qword_A750D8; // weak
extern enum PlayerSpeech PlayerSpeechID;
extern int uSpeakingCharacter; // weak
extern std::array<const char *, 36> pClassNames;
extern std::array<const char *, 19> aCharacterConditionNames;
extern std::array<const char *, 38> pSkillNames;
extern int dword_AE336C; // weak
extern int dword_AE3370; // weak
extern char byte_AE5B91; // weak
extern std::array<int, 32> dword_F1B430; // weak
extern std::array<int, 4> player_levels;
extern std::array<__int16, 6> weapons_Ypos; // word_F8B158
extern struct Texture *ShopTexture; // idb
extern std::array<struct Texture *, 12> ItemsInShopTexture;
extern __int16 bountyHunting_monster_id_for_hunting; // weak
extern const char *bountyHunting_text; // idb
extern int contract_approved; // weak
extern int dword_F8B1AC_award_bit_number; // idb
extern int dword_F8B1B0; // weak
extern int gold_transaction_amount; // F8B1B4
extern std::array<char *, 4> pShopOptions;
extern _UNKNOWN unk_F8B1C8; // weak
extern int dword_F8B1D8; // weak
extern int dword_F8B1DC; // weak
extern int dword_F8B1E0; // weak
extern int dword_F8B1E4; // weak
extern const char *current_npc_text; // 0xF8B1E8
extern char dialogue_show_profession_details; // weak
extern std::array<char, 777> byte_F8B1EF; // weak
extern std::array<char, 4> byte_F8B1F0;
extern int dword_F8B1F4; // weak




//extern _UNKNOWN unk_F8BA50; // weak
extern char byte_F8BC0C; // weak
extern int bGameoverLoop; // weak
extern std::array<__int16, 104> intersect_face_vertex_coords_list_a; // word_F8BC48
extern std::array<__int16, 104> intersect_face_vertex_coords_list_b; // word_F8BD18
//extern _UNKNOWN unk_F8EA04; // weak
//extern _UNKNOWN unk_F8F8F8; // weak
extern int dword_F93F20; // weak
extern int dword_F93F70; // weak















//-------------------------------------------------------------------------
// Function declarations


void init_event_triggers();
void check_event_triggers();




void ShowIntroVideo_and_LoadingScreen();
unsigned int GameOverMenu(void *ecx0);
int __fastcall SpawnRandomTreasure(struct MapInfo *a1, struct SpawnPointMM7 *a2);
void DamageMonsterFromParty(signed int a1, unsigned int uActorID_Monster, struct Vec3_int_ *pVelocity);

void MakeActorAIList_ODM();
int MakeActorAIList_BLV();
void UpdateActorAI();
bool __fastcall sub_4070EF_prolly_detect_player(unsigned int uObjID, unsigned int uObj2ID);
bool __fastcall sub_4075DB(int a1, int a2, int a3, struct BLVFace *a4);
bool __fastcall sub_4077F1(int a1, int a2, int a3, struct ODMFace *a4, struct BSPVertexBuffer *a5);
bool __fastcall sub_407A1C(int x, int z, int y, struct Vec3_int_ v); // idb
void InitializeActors();
void InitializeSpriteObjects();
int __fastcall sub_4088E9(int a1, int a2, int a3, int a4, int a5, int a6);
unsigned int SearchAliveActors(unsigned int *pTotalActors);
unsigned int SearchActorByMonsterID(unsigned int *pTotalActors, int uMonsterID);
unsigned int SearchActorByGroup(unsigned int *pTotalActors, unsigned int uGroup);
unsigned int SearchActorByID(unsigned int *pTotalActors, unsigned int a2);
void PrepareArcomage();
unsigned short TargetColor(unsigned __int16 r, unsigned __int16 g, unsigned __int16 b); // idb
void DoBlt_Copy(unsigned __int16 *pPixels); // idb
void __fastcall ZBuffer_Fill(int *pZBuffer, int uTextureId, int iZValue);
void __fastcall ZBuffer_DoFill(int *pZBuffer, struct Texture *pTex, int uZValue);
void __fastcall ZBuffer_DoFill2(int *pZBuffer, struct Texture *a2, int a3); // idb
void SetMoonPhaseNames();
int __fastcall sub_410D99_get_map_index(int a1);
void __fastcall LoadThumbnailLloydTexture(unsigned int uSlot, unsigned int uPlayer);
void SetAttributeNames();
void uGameUIFontMain_initialize();
void uGameUIFontShadow_initialize();
void Inventory_ItemPopupAndAlchemy();
void UI_OnMouseRightClick(struct Vec2_int_ *_this);
void UI_OnMouseLeftClick(int *pXY); // idb
unsigned int UI_GetHealthManaAndOtherQualitiesStringColor(signed int current_pos, signed int base_pos);
unsigned int __fastcall GetSizeInInventorySlots(unsigned int uNumPixels);
struct GUIButton *__fastcall GUI_HandleHotkey(unsigned __int8 uHotkey); // idb
int __fastcall GUI_ReplaceHotkey(unsigned __int8 uOldHotkey, unsigned __int8 uNewHotkey, char bFirstCall);
void DrawBuff_remaining_time_string(int uY, struct GUIWindow *window, __int64 remaining_time, struct GUIFont *Font);
bool UI_OnKeyDown(unsigned int vkKey);
void GameUI_DrawItemInfo(struct ItemGen* inspect_item); // idb
void MonsterPopup_Draw(unsigned int uActorID, struct GUIWindow *window);
void LoadActualSkyFrame();
void Sleep6Hours();
void ChestUI_WritePointedObjectStatusString();
void OnChestLeftClick();
void GameUI_WritePointedObjectStatusString();
void __fastcall GameUI_OnPlayerPortraitLeftClick(unsigned int uPlayerID); // idb
void OnGameViewportClick();
bool PauseGameDrawing();
void SetUserInterface(enum PartyAlignment alignment, bool bReplace);
void reset_some_strus_flt_2Cs();
int __fastcall GetPortalScreenCoord(unsigned int uFaceID);
bool PortalFrustrum(int pNumVertices, struct BspRenderer_PortalViewportData *a2, struct BspRenderer_PortalViewportData *near_portal, int uFaceID);
int ODM_NearClip(unsigned int uVertexID); // idb
int ODM_FarClip(unsigned int uNumVertices);
bool __fastcall sub_427769_spell(unsigned int uSpellID);
void _42777D_CastSpell_UseWand_ShootArrow(int a1, unsigned int uPlayerID, unsigned int a4, __int16 a5, int a6);
void _42ECB5_PlayerAttacksActor();
void  InitializeTurnBasedAnimations(void *);
bool __fastcall sub_42F7EB_DropItemAt(unsigned int uSpriteID, int x, int y, int z, int a4, int count, int a7, unsigned __int16 attributes, ItemGen *a9);
void __fastcall sub_42F960_create_object(int x, int y, int z); // idb
void CompactLayingItemsList();
void _42FA66_do_explosive_impact(int a1, int a2, int a3, int a4, __int16 a5, signed int a6);
bool _42FB5C_check_spell(signed int a1);
void sub_42FBDD();
void CloseWindowBackground();
void ProcessInputActions();
void GameUI_MsgProc();
void back_to_game();
void GUI_MainMenuMessageProc();
double get_shading_dist_mist();
void Vec3_short__to_RenderVertexSoft(struct RenderVertexSoft *_this, struct Vec3_short_ *a2);
void area_of_effect__damage_evaluate();
void __fastcall DamagePlayerFromMonster(unsigned int uObjID, int a2, struct Vec3_int_ *pPos, unsigned int a4);
void __fastcall sub_43A97E(unsigned int uLayingItemID, signed int a2); // idb
double __fastcall sub_43AE12(signed int a1);
int __fastcall _43AFE3_calc_spell_damage(int a1, int a2, signed int a3, int a4);
void __fastcall sub_43B057(unsigned int uObjID, unsigned int uActorID, struct Vec3_int_ *pVelocity);
void sub_43B1B0(signed int a1, unsigned int a2, struct Vec3_int_ *pVelocity, signed int a4);
void CharacterUI_LoadPaperdollTextures();
int __fastcall GetItemTextureFilename(char *pOut, signed int item_id, int index, int shoulder);
bool IsDwarfPresentInParty(bool b);
bool  ShouldLoadTexturesForRaceAndGender(unsigned int _this);
void WetsuitOn(unsigned int uPlayerID); // idb
void WetsuitOff(unsigned int uPlayerID);
void __fastcall PrepareDrawLists_BLV();
void FindBillboardsLightLevels_BLV();
int __fastcall _43F55F_get_billboard_light_level(struct RenderBillboard *a1, int uBaseLightLevel);
int __fastcall _43F5C8_get_point_light_level_with_respect_to_lights(unsigned int uBaseLightLevel, int uSectorID, float x, float y, float z);
void PrepareBspRenderList_BLV();
void PrepareDecorationsRenderList_BLV(unsigned int uDecorationID, unsigned int uSectorID);
void PrepareActorRenderList_BLV();
void PrepareItemsRenderList_BLV();
void AddBspNodeToRenderList(unsigned int node_id);
void __fastcall sub_4406BC(unsigned int node_id, unsigned int uFirstNode); // idb
bool sub_44100D();
__int16 __fastcall sub_441A4E(int a1);
void DrawBook_Map_sub(unsigned int tl_x, unsigned int tl_y, unsigned int br_x, int br_y, int _48074); // idb
void Initialize2DA();
void LoadLevel_InitializeLevelStr();
void OnMapLeave();
void OnMapLoad();
void Level_LoadEvtAndStr(const char *pLevelName);
const char *GetMapBookHintText();//sub_444564
char *GetEventHintString(unsigned int uEventID); // idb
int GetTravelTime();
void __fastcall sub_4451A8_press_any_key(int a1, int a2, int a4);
unsigned int SkillToMastery(unsigned int skill_value);
unsigned int __fastcall GetSpellColor(signed int a1);
unsigned short * MakeScreenshot(signed int width, signed int height);
void SaveScreenshot(const char *pFilename);
void __fastcall LoadGame(unsigned int uSlot); // idb
void SaveGame(bool IsAutoSAve, bool NotSaveWorld);
void __fastcall DoSavegame(unsigned int uSlot); // idb
void TryLoadLevelFromLOD();
void sub_46080D();
bool Initialize_GamesLOD_NewLOD();
void SaveNewGame();
void PrepareToLoadBLV(unsigned int bLoading);
void __fastcall PrepareToLoadODM(unsigned int bLoading, struct ODMRenderParams *a2);
void _461103_load_level_sub();
void MainMenu_Loop();
char sub_4637E0_is_there_popup_onscreen();
void ResetCursor_Palettes_LODs_Level_Audio_SFT_Windows();
void PrepareWorld(unsigned int _0_box_loading_1_fullscreen);
void Game_DeinitializeAndTerminate(int exitCode); // idb
void FinalInitialization();
char Is_out15odm_underwater();
void SetUnderwaterFog();
void DoPrepareWorld(unsigned int bLoading, int _1_fullscreen_loading_2_box);
int __fastcall ReadWindowsRegistryInt(const char *pKey, int uDefValue); // idb
void __fastcall WriteWindowsRegistryString(const char *pKey, const char *pString);
void __fastcall ReadWindowsRegistryString(const char *pKeyName, char *pOutString, int uBufLen, const char *pDefaultValue);
void __fastcall WriteWindowsRegistryInt(const char *pKey, int val);
bool __fastcall CheckMM7CD(char c);
void SecondaryInitialization();
void CreateAsyncMouse();
void CreateAsyncKeyboard();
void MM6_Initialize(const wchar_t *pIniFilename);
void MM7Initialization();
void SetCurrentMenuID(enum MENU_STATE); // idb
enum MENU_STATE GetCurrentMenuID();
void CreateMsgScrollWindow(signed int mscroll_id);
void free_book_subwindow();
void CreateScrollWindow();
void OnPaperdollLeftClick();
void OnPressSpace();
char __fastcall DoInteractionWithTopmostZObject(int a1, int a2);
void OracleDialogue();
void __fastcall ClickNPCTopic(signed int uMessageParam);
char * _4B254D_SkillMasteryTeacher(int _this);
const char *ContractSelectText(int pEventCode);
void SimpleHouseDialog();
void CreateButtonInColumn(int a1, unsigned int a2);
void FillAviableSkillsToTeach(int _this);
void sub_4B3E1E();
void DrawJoinGuildWindow(int pEventCode);
void __fastcall _4B3FE5_training_dialogue(int a4);
void NPCHireableDialogPrepare();
void _4B4224_UpdateNPCTopics(int _this);
void __fastcall DrawTextAtStatusBar(const char *Str, int a5);
__int64 GetExperienceRequiredForLevel(int a1);
void CheckBountyRespawnAndAward();
void Arena_SelectionFightLevel();
void ArenaFight();
void SpellBookGenerator();
void UI_CreateEndConversationButton();
void __fastcall OnSelectShopDialogueOption(signed int uMessageParam);
int HouseDialogPressCloseBtn();
bool __fastcall MerchandiseTest(ItemGen *item, int _2da_idx);
void UIShop_Buy_Identify_Repair();
bool __fastcall IsBModelVisible(unsigned int uModelID, int *unused);
void ODM_LoadAndInitialize(const char *pLevelFilename, struct ODMRenderParams *thisa);
unsigned int GetLevelFogColor();
int __fastcall sub_47C3D7_get_fog_related_stuff(int a1, int a2, float a3);
int __fastcall GetActorTintColor(int max_dim, int min_dim, float distance, int a4, struct RenderBillboard *a5);
unsigned int WorldPosToGridCellX(int); // weak
unsigned int WorldPosToGridCellZ(int); // weak
int GridCellToWorldPosX(int); // weak
int GridCellToWorldPosZ(int); // weak
void sub_481ED9_MessWithODMRenderParams();
bool IsTerrainSlopeTooHigh(int pos_x, int pos_y);
int __fastcall GetTerrainHeightsAroundParty2(int a1, int a2, int *a3, int a4);
void sub_487DA9();
double GetFogDensityByTime(struct OutdoorLocation *_this);
bool __fastcall HSV2RGB(float *a1, float *a2, float *a3, float a4, float a5, float a6);
void __fastcall RGB2HSV(float *a1, float *a2, float a3, float a4, float a5, float *a6);
unsigned int ReplaceHSV(unsigned int uColor, float a2, float gamma, float a4);
bool PlayerCreation_Choose4Skills();
int PlayerCreation_GetUnspentAttributePointCount();
void LoadPlayerPortraintsAndVoices();
void ReloadPlayerPortraits(int player_id, int face_id);
void sub_491E3A();
int CycleCharacter(bool backwards);
void __fastcall Rest(unsigned int uHoursToSleep);
int _493938_regenerate();
void init_summoned_item(struct stru351_summoned_item *_this, __int64 duration);
void _494035_timed_effects__water_walking_damage__etc();
unsigned int __fastcall _494820_training_time(unsigned int a1);
char *__fastcall sub_495366(unsigned __int8 a1, unsigned __int8 a2);
const char *GetReputationString(signed int a1);
char *BuildDialogueString(const char *lpsz, unsigned __int8 uPlayerID, struct ItemGen *a3, char *a4, int a5, __int64 *a6);
void PlayerCreationUI_Draw();
void PlayerCreationUI_Initialize();
void DeleteCCharFont();
bool PlayerCreationUI_Loop();
unsigned int __fastcall GetMaxMipLevels(unsigned int uDim);
bool CheckTextureStages();
bool AreRenderSurfacesOk();
unsigned int BlendColors(unsigned int c1, unsigned int c2); // weak
void DoRenderBillboards_D3D();
int __fastcall sr_4A46E6_draw_particle_segment(unsigned int x, signed int y, signed int z, int a4, unsigned int lightColor);
void Present_ColorKey();
void Present_NoColorKey();
unsigned int ModulateColor(unsigned int diffuse, float multiplier); // idb
struct SoundHeader *FindSound_BinSearch(unsigned int uStart, unsigned int uEnd, const char *pName);
struct SoundData *LoadSound(const char *pSoundName, struct SoundData *pOutBuff, unsigned int uID);
int __fastcall sub_4AAEA6_transform(struct RenderVertexSoft *a1);
int __fastcall sub_4AB66C(int, int); // weak
int GetSoundStrengthByDistanceFromParty(int x, int y, int z);
struct _DIG_DRIVER *Audio_GetFirstHardwareDigitalDriver(void);
void PlayLevelMusic();
unsigned int __fastcall sub_4B0E07(unsigned int uFaceID); // idb
void sub_4B1447_party_fine(int a1, int a2, int a3);
void sub_4B1523(int *_this);
void ShowPopupShopItem();
void RestAndHeal(__int64 uNumMinutes); // idb
void GetHouseGoodbyeSpeech();
int __fastcall _46A6AC_spell_render(int a1, int a2, int a3);
int __fastcall _46A6AC_spell_render_d3d(int a1, int a2, int a3);
int __fastcall _46A89E_immolation_effect(int a1, int a2, int a3);
int sub_46A99B();
unsigned int GetGravityStrength();
void UpdateUserInput_and_MapSpecificStuff();
void BLV_UpdateUserInputAndOther();
void ODM_UpdateUserInputAndOther();
bool __fastcall _46BFFA_check_object_intercept(unsigned int uLayingItemID, signed int a2);
int BLV_GetFloorLevel(int x, int y, int z, unsigned int uSectorID, unsigned int *pFaceID);
int ODM_GetFloorLevel(int X, signed int Y, int Z, int, int *pOnWater, int *bmodel_pid, int bWaterWalk);
int __fastcall sub_46D8E3(int a1, signed int a2, int a3, int a4);
void ODM_GetTerrainNormalAt(int pos_x, int pos_z, Vec3_int_ *out);
unsigned int __fastcall sub_46DEF2(signed int a2, unsigned int uLayingItemID);
void _46E0B2_collide_against_decorations();
void __fastcall _46E26D_collide_against_sprites(signed int a1, signed int a2);
int _46E44E_collide_against_faces_and_portals(unsigned int b1); // idb
int __fastcall _46E889_collide_against_bmodels(unsigned int ecx0);
int collide_against_floor(int x, int y, int z, unsigned int *pSectorID, unsigned int *pFaceID); // idb
void __fastcall _46ED8A_collide_against_sprite_objects(unsigned int _this);
int _46EF01_collision_chech_player(int a1); // idb
int _46F04E_collide_against_portals();
void BLV_UpdateDoors();
void UpdateActors_BLV();
void UpdateActors_ODM();
void UpdateObjects();
int collide_against_floor_approximate(int x, int y, int z, unsigned int *pSectorID, unsigned int *pFaceID); // idb
void BLV_ProcessPartyActions();
void ODM_ProcessPartyActions();
bool __fastcall sub_47531C(int a1, int *a2, int a3, int a4, int a5, int a6, int a7, int a8, struct BLVFace *a9, int a10);
bool __fastcall sub_4754BF(int a1, int *a2, int a3, int a4, int a5, int a6, int a7, int a8, struct BLVFace *a9, int a10, int a11);
int sub_475665(struct BLVFace *_this, int a2, __int16 a3);
bool __fastcall sub_4759C9(struct BLVFace *a1, int a2, int a3, __int16 a4);
bool __fastcall sub_475D85(Vec3_int_ *a1, Vec3_int_ *a2, int *a3, struct BLVFace *a4);
bool __fastcall sub_475F30(int *a1, struct BLVFace *a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9);
void sub_4452BB();
const char *GetProfessionActionText(int a1);
struct NPCData *__fastcall GetNPCData(signed int npcid);
struct NPCData * GetNewNPCData(signed int npcid, int* npc_indx);
int __fastcall GetGreetType(signed int SpeakingNPC_ID);
void  DialogueEnding();
void PrepareHouse(enum HOUSE_ID house); // idb
bool EnterHouse(enum HOUSE_ID uHouseID);
int sub_4465DF_check_season(int a1);
int __fastcall IsActorAlive(unsigned int uType, unsigned int uParam, unsigned int uNumAlive); // idb
int NPC_EventProcessor(int npc_event_id, int entry_line = 0);
void __fastcall sub_448518_npc_set_item(int npc, unsigned int item, int a3);
void __fastcall sub_44861E_set_texture(unsigned int uFaceCog, const char *pFilename);
void __fastcall SetDecorationSprite(uint16_t uCog, bool bHide, const char *pFileName); // idb
void __fastcall sub_44892E_set_faces_bit(int sCogNumber, int bit, int on);
void __fastcall ToggleActorGroupFlag(unsigned int uGroupID, unsigned int uFlag, unsigned int bToggle);
void  GameUI_StatusBar_UpdateTimedString(unsigned int bForceHide); // idb
void OnTimer(int);
void __fastcall sub_448CF4_spawn_monsters(__int16 typeindex, __int16 level, int count, int x, int y, int z, int group, unsigned int uUniqueName);
void __fastcall EventCastSpell(int uSpellID, int uSkillLevel, int uSkill, int fromx, int fromy, int fromz, int tox, int toy, int toz);//sub_448DF8
__int16 __fastcall sub_449A49_door_switch_animation(unsigned int uDoorID, int a2); // idb
bool _449B57_test_bit(unsigned __int8 *a1, __int16 a2);
void _449B7E_toggle_bit(unsigned char *pArray, __int16 a2, unsigned __int16 bToggle); // idb
void ShowStatusBarString(const char *pString, unsigned int uNumSeconds);
void ShowNothingHereStatus();
int const_2();
bool TeleportToNWCDungeon();
void SpawnEncounter(struct MapInfo *pMapInfo, struct SpawnPointMM7 *spawn, int a3, int a4, int a5);
int __fastcall sub_44FA4C_spawn_light_elemental(int a1, int a2, int a3);
int __fastcall sub_450521_ProllyDropItemAt(int ecx0, signed int a2, int a3, int a4, int a5, unsigned __int16 a6);
int __fastcall _45063B_spawn_some_monster(struct MapInfo *a1, int a2);
void RespawnGlobalDecorations();
bool __fastcall SpawnActor(unsigned int uMonsterID);
int GetAlertStatus();
int integer_sqrt(int val);
int __fastcall MakeColorMaskFromBitDepth(int a1);
void __fastcall fill_pixels_fast(unsigned int a1, unsigned __int16 *pPixels, unsigned int uNumPixels);
int __fastcall GetDiceResult(unsigned int uNumDice, unsigned int uDiceSides); // idb
inline int round(float x) { return (int)floor(x + 0.5f); }
inline void __fastcall memset32(void *ptr, unsigned __int32 value, int count)
{
  unsigned __int32* p = (unsigned __int32 *)ptr;
  for ( int i=0; i < count; i++ )
    *p++ = value;
}
//inline void __fastcall j_memset32(int a2, void *a1, unsigned int a3) {memset32(a1, a2, a3);}

#define ErrD3D(hr) do {extern void ErrHR(HRESULT, const char *, const char *, const char *, int); ErrHR(hr, "Direct3D", __FUNCTION__, __FILE__, __LINE__);} while(0)


namespace zlib
{
  int MemZip(void *dest, unsigned int *destLen, void *source, unsigned int sourceLen);
  int MemUnzip(void *dest, unsigned int *destLen, const void *source, unsigned int sourceLen);
};