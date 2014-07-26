#pragma once
#include <string>
#include "VectorTypes.h"
#include "OSAPI.h"
#include <array>
#include <assert.h>
#include "NZIArray.h"

//typedef char _UNKNOWN;
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
extern unsigned int game_viewport_width;
extern unsigned int game_viewport_height;
extern int game_viewport_x;
extern int game_viewport_y;
extern int game_viewport_z;
extern int game_viewport_w;

extern int pWindowList_at_506F50_minus1_indexing[1];
extern int dword_4C9890[10]; // weak
extern int dword_4C9920[16]; // weak
extern char byte_4D864C; // weak
extern float flt_4D86CC; // weak
extern int dword_4D86D8; // weak


extern float flt_4D84E8;

extern int pCurrentScreen; // 004E28F8
extern unsigned int uGammaPos;
extern std::array<int, 8> BtnTurnCoord;
extern std::array<__int16, 4> RightClickPortraitXmin;
extern std::array<__int16, 4> RightClickPortraitXmax;
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
extern int dword_4E455C; // weak
extern std::array<int, 6> dword_4E4560;
extern std::array<int, 6> dword_4E4578;
extern std::array<int, 6> dword_4E4590;
extern std::array<int, 6> dword_4E45A8;
extern std::array<float, 10> flt_4E4A80;

extern std::array< std::array<int, 2>, 14> pPartySpellbuffsUI_XYs;
extern std::array<unsigned char, 14> byte_4E5DD8; // weak
extern std::array<unsigned __int8, 14> pPartySpellbuffsUI_smthns;
extern std::array< std::array<int, 6>, 6> pNPCPortraits_x; // 004E5E50
extern std::array< std::array<int, 6>, 6> pNPCPortraits_y; // 004E5EE0
extern std::array<const char *, 11> pHouse_ExitPictures;
extern std::array<const char *, 11> _4E6BDC_loc_names;
extern std::array<__int16, 11> word_4E8152;
extern std::array< std::array<char, 14>, 7> byte_4E8168;
extern char _4E94D0_light_type; // weak
extern char _4E94D2_light_type; // weak
extern char _4E94D3_light_type; // weak
extern std::array<unsigned int, 2> saveload_dlg_xs;
extern std::array<unsigned int, 2> saveload_dlg_ys;
extern std::array<unsigned int, 2> saveload_dlg_zs;
extern std::array<unsigned int, 2> saveload_dlg_ws;
extern int dword_4E98BC_bApplicationActive; // weak
extern std::array<char*, 465> pTransitionStrings; // 4EB080
extern std::array<int, 9> dword_4EC268; // weak
extern std::array<int, 7> dword_4EC28C; // weak
extern std::array<const char*, 25> pPlayerPortraitsNames;

extern std::array< std::array<unsigned char, 25>, 48> byte_4ECF08;
extern std::array<std::array<unsigned char, 8>, 110> SoundSetAction; // weak
extern std::array<__int16, 4> pPlayerPortraitsXCoords_For_PlayerBuffAnimsDrawing;
extern std::array< std::array<char, 37>, 36> byte_4ED970_skill_learn_ability_by_class_table;
extern std::array<unsigned int, 2> pHiredNPCsIconsOffsetsX;
extern std::array<unsigned int, 2> pHiredNPCsIconsOffsetsY;
extern std::array<int, 2> Party_Spec_Motion_status_ids; //dword_4EE07C
extern std::array<__int16, 101> word_4EE088_sound_ids; // weak
extern std::array<short, 28> word_4EE150;
extern int ScreenshotFileNumber; // dword_4EFA80
extern std::array<const char *, 19> off_4F03B8; // idb
extern __int16 word_4F0576[]; // weak

extern std::array<int, 32> guild_mambership_flags;
extern std::array<__int16, 49> word_4F0754;
extern std::array<__int16, 54> _4F0882_evt_VAR_PlayerItemInHands_vals;
extern std::array<unsigned short, 6> pMaxLevelPerTrainingHallType;
extern std::array<int, 11> price_for_membership; // weak
extern std::array<__int16, 32> word_4F0F30; // weak
extern std::array<int, 500> ai_array_4F5E68;
extern std::array<int, 500> ai_array_4F6638_actor_ids;
extern std::array<int, 500> ai_near_actors_targets_pid;
extern int ai_arrays_size; // weak
extern std::array<int, 500> ai_near_actors_distances;
extern std::array<unsigned int, 500> ai_near_actors_ids;
extern std::array<int, 182> dword_4F8580; // weak
extern int dword_4FA9B0[]; // weak
extern int dword_4FA9B4[]; // weak

extern char byte_4FAA24; // weak
//extern HWND dword_4FAA28; // idb

extern std::array<unsigned int, 480> pSRZBufferLineOffsets;
extern int areWeLoadingTexture; // weak
extern std::array<char, 777> books_num_items_per_page; // weak
extern int lloyds_beacon_spell_id; // dword_506338
extern int lloyds_beacon_sound_id; // dword_50633C
extern signed int sRecoveryTime; // idb
extern unsigned int uRequiredMana; // idb
extern int _506348_current_lloyd_playerid; // weak
extern __int64 lloyds_beacon_spell_level; // qword_506350
extern char byte_506360; // weak
extern int dword_506364; // weak
//extern Texture *dword_50640C[]; // weak
extern unsigned int uExitCancelTextureId;
extern int books_page_number; // number for page in books(����� �������� � ������)
extern int books_primary_item_per_page; // number primary item in book page(����� ��������� ������ �� ��������)
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
extern std::array<int, 100> dword_50BC10; // weak
extern std::array<int, 100> dword_50BDA0; // weak
extern std::array<int, 100> _50BF30_actors_in_viewport_ids; // weak
extern char town_portal_caster_id; // weak
extern int some_active_character; // weak
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
extern int _50C9D0_AfterEnchClickEventId; // weak
extern int _50C9D4_AfterEnchClickEventSecondParam; // weak
extern int _50C9D8_AfterEnchClickEventTimeout; // weak
extern int dword_50C9DC; // weak
extern struct NPCData *ptr_50C9E0;
//extern int dword_50C9E8; // idb
//extern int dword_50C9EC[]; // 50C9EC
extern int dword_50CDC8;
extern Vec3_int_ layingitem_vel_50FDFC;
extern char pStartingMapName[32]; // idb
extern std::array<unsigned __int8, 5> IsPlayerWearingWatersuit;
extern std::array<char, 54> party_has_equipment;
extern std::array<char, 17> byte_5111F6_OwnedArtifacts;
extern std::array<unsigned int, 16> papredoll_dbrds;

extern int bRingsShownInCharScreen; // weak

extern unsigned int uNumBlueFacesInBLVMinimap;
extern std::array<unsigned __int16, 50> pBlueFacesInBLVMinimapIDs;
extern std::array<int, 14> pTextureIDs_PartyBuffIcons;
extern unsigned int uIconIdx_FlySpell;
extern unsigned int uIconIdx_WaterWalk;
extern int dword_576E28; // weak
extern int _576E2C_current_minimap_zoom; // weak
extern __int64 _5773B8_event_timer; // weak

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
extern int MapsLongTimers_count; // dword_5B65C8
extern int npcIdToDismissAfterDialogue; // weak
extern int dword_5B65D0_dialogue_actor_npc_id; // weak
extern int dword_5C3418; // weak
extern int dword_5C341C; // weak
//extern std::array<char, 777> byte_5C3427; // weak
extern std::array<char, 200> GameUI_Footer_TimedString;
extern std::array<char, 200> pFooterString;
extern unsigned int GameUI_Footer_TimeLeft;
extern int bForceDrawFooter; // weak
extern int _5C35C0_force_party_death; // weak
extern int bDialogueUI_InitializeActor_NPC_ID; // weak
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
extern int pSaveListPosition; // weak
extern unsigned int uLoadGameUI_SelectedSlot;
extern HWND hInsertCDWindow; // idb
extern int uCPUSpeed; // weak
extern char cMM7GameCDDriveLetter; // idb
extern enum MENU_STATE sCurrentMenuID;
extern unsigned int uGameState;
extern int uDefaultTravelTime_ByFoot; // weak
extern int day_attrib; // weak
extern int day_fogrange_1; // weak
extern int day_fogrange_2; // weak
extern struct TileTable *pTileTable; // idb
extern int outdoor_grid_band_1; // idb
extern int outdoor_grid_band_2; // idb
extern int outdoor_grid_band_3; // idb
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
extern char pCurrentMapName[32]; // idb
extern unsigned int uLevelMapStatsID;

extern bool FORCE_16_BITS; // related to GAME_SETTINGS_INVALID_RESOLUTION
#define GAME_SETTINGS_0001                            0x0001
#define GAME_SETTINGS_INVALID_RESOLUTION              0x0002
#define GAME_SETTINGS_NO_INTRO                        0x0004
#define GAME_SETTINGS_NO_LOGO                         0x0008
#define GAME_SETTINGS_NO_SOUND                        0x0010
#define GAME_SETTINGS_NO_WALK_SOUND                   0x0020
#define GAME_SETTINGS_NO_HOUSE_ANIM                   0x0040
#define GAME_SETTINGS_0080_SKIP_USER_INPUT_THIS_FRAME 0x0080
#define GAME_SETTINGS_APP_INACTIVE                    0x0100
#define GAME_SETTINGS_0200_EVENT_TIMER                0x0200
#define GAME_SETTINGS_0400_MISC_TIMER                 0x0400
#define GAME_SETTINGS_0800                            0x0800
#define GAME_SETTINGS_1000                            0x1000
#define GAME_SETTINGS_2000                            0x2000
#define GAME_SETTINGS_4000                            0x4000
extern int dword_6BE364_game_settings_1;  // GAME_SETTINGS_*

#define DEBUG_SETTINGS_RUN_IN_WIDOW     0x0001
#define DEBUG_SETTINGS_0002_SHOW_FR     0x0002
#define DEBUG_SETTINGS_NO_ACTORS        0x0004
#define DEBUG_SETTINGS_NO_DECORATIONS   0x0008
#define DEBUG_SETTINGS_NO_DAMAGE        0x0010
extern int dword_6BE368_debug_settings_2; // DEBUG_SETTINGS_*

extern unsigned __int8 bCanLoadFromCD;
extern int bShowDamage; // idb
extern unsigned int bAlwaysRun;
extern unsigned int bFlipOnExit;
extern unsigned int uTurnSpeed;
extern float flt_6BE3A0; // weak
extern float flt_6BE3A4_debug_recmod1;
extern float flt_6BE3A8_debug_recmod2;
extern float flt_6BE3AC_debug_recmod1_x_1_6;
extern std::array<char, 20> byte_6BE3B0; // idb
extern char bUnderwater; // weak
extern char bNoNPCHiring; // weak
extern unsigned int bNoVideo;
extern bool bNoIntro;
extern bool bNoLogo;
extern bool bNoCD;
extern bool bNoSound;
extern std::array<int, 100> dword_720020_zvalues;
extern std::array<int, 299> dword_7201B0_zvalues;
extern int uTextureID_720980; // weak
extern std::array<__int16, 104> word_7209A0_intercepts_ys_plus_ys;
extern std::array<__int16, 104> word_720A70_intercepts_xs_plus_xs;
extern std::array<__int16, 104> word_720B40_intercepts_zs;
extern std::array<__int16, 102> word_720C10_intercepts_xs;
extern std::array<__int16, 777> word_720CE0_ys; // idb
extern std::array<__int16, 777> word_720DB0_xs; // idb
extern std::array<int, 20> dword_720E80;
extern std::array<int, 20> dword_720ED0;
extern std::array<int, 20> ceiling_height_level;
extern std::array<__int16, 104> odm_floor_face_vert_coord_Y; // word_720F70
extern std::array<__int16, 104> odm_floor_face_vert_coord_X; // word_721040
extern std::array<int, 20> current_Face_id; // dword_721110
extern std::array<int, 20> current_BModel_id; // dword_721160
extern std::array<int, 20> odm_floor_level; // idb dword_7211B0
extern int blv_prev_party_x; // weak
extern int blv_prev_party_z; // weak
extern int blv_prev_party_y; // weak
extern char *dword_722F10; // idb
extern std::array<const char *, 513> pQuestTable;
extern char *dword_723718_autonote_related; // idb
extern std::array<const char *, 82> pScrolls;
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
extern int dword_A74C88; // weak
extern unsigned int uPlayerCreationUI_SkySliderPos;
extern int uPlayerCreationUI_ArrowAnim;
extern unsigned int uPlayerCreationUI_SelectedCharacter;
extern int dword_A74CDC; // weak
extern struct Texture *pTexture_PlayerFaceMask;
extern struct Texture *pTexture_PlayerFaceEradicated;
extern struct Texture *pTexture_PlayerFaceDead;
extern std::array< std::array<struct Texture *, 56>, 4> pTextures_PlayerFaces;
extern __int64 _A750D8_player_speech_timer; // qword_A750D8
extern enum PlayerSpeech PlayerSpeechID;
extern int uSpeakingCharacter; // weak
extern std::array<const char *, 36> pClassNames;
extern std::array<const char *, 19> aCharacterConditionNames;
extern std::array<const char *, 38> pSkillNames;
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
extern int dword_F8B1B0_MasteryBeingTaught; // weak
extern int gold_transaction_amount; // F8B1B4
extern std::array<char *, 4> pShopOptions;
extern int dword_F8B1D8; // weak
extern int dword_F8B1E0; // weak
extern int dword_F8B1E4; // weak
extern const char *current_npc_text; // 0xF8B1E8
extern char dialogue_show_profession_details; // weak
extern std::array<char, 777> byte_F8B1EF; // weak
extern std::array<char, 4> byte_F8B1F0;

extern int bGameoverLoop; // weak
extern std::array<__int16, 104> intersect_face_vertex_coords_list_a; // word_F8BC48
extern std::array<__int16, 104> intersect_face_vertex_coords_list_b; // word_F8BD18
extern bool wizard_eye;
extern bool change_seasons;
extern bool all_magic;
extern bool debug_information;
extern bool show_picked_face;
extern bool draw_portals_loops;
extern bool new_speed;
extern bool bSnow;
extern bool draw_terrain_dist_mist;
