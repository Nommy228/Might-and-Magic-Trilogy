#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <algorithm> 
#include "..\MM7.h"
#include "..\MapInfo.h"
#include "..\Game.h"
#include "..\GUIWindow.h"
#include "..\GUIFont.h"
#include "..\GUIProgressBar.h"
#include "..\Party.h"
#include "..\AudioPlayer.h"
#include "..\Render.h"
#include "..\LOD.h"
#include "..\Viewport.h"
#include "..\Time.h"
#include "..\Awards.h"
#include "..\CastSpellInfo.h"
#include "..\texts.h"
#include "..\Mouse.h"

#include "..\mm7_data.h"

int bRingsShownInCharScreen; // 5118E0

unsigned int ui_mainmenu_copyright_color;

unsigned int ui_character_tooltip_header_default_color;
unsigned int ui_character_default_text_color;
unsigned int ui_character_skill_highlight_color;
unsigned int ui_character_header_text_color;
unsigned int ui_character_bonus_text_color;
unsigned int ui_character_bonus_text_color_neg;
unsigned int ui_character_skill_upgradeable_color;
unsigned int ui_character_skill_default_color;
unsigned int ui_character_stat_default_color;
unsigned int ui_character_stat_buffed_color;
unsigned int ui_character_stat_debuffed_color;
unsigned int ui_character_skillinfo_can_learn;
unsigned int ui_character_skillinfo_can_learn_gm;
unsigned int ui_character_skillinfo_cant_learn;
unsigned int ui_character_condition_normal_color;
unsigned int ui_character_condition_light_color;
unsigned int ui_character_condition_moderate_color;
unsigned int ui_character_condition_severe_color;
std::array<unsigned int, 6> ui_character_award_color;

unsigned int ui_game_minimap_outline_color;
unsigned int ui_game_minimap_actor_friendly_color;
unsigned int ui_game_minimap_actor_hostile_color;
unsigned int ui_game_minimap_actor_corpse_color;
unsigned int ui_game_minimap_decoration_color_1;
unsigned int ui_game_minimap_projectile_color;
unsigned int ui_game_minimap_treasure_color;
std::array<unsigned int, 24> ui_game_character_record_playerbuff_colors;

unsigned int ui_gamemenu_video_gamma_title_color;
unsigned int ui_gamemenu_keys_action_name_color;
unsigned int ui_gamemenu_keys_key_selection_blink_color_1;
unsigned int ui_gamemenu_keys_key_selection_blink_color_2;
unsigned int ui_gamemenu_keys_key_default_color;

unsigned int ui_book_quests_title_color;
unsigned int ui_book_quests_text_color;
unsigned int ui_book_autonotes_title_color;
unsigned int ui_book_autonotes_text_color;
unsigned int ui_book_map_title_color;
unsigned int ui_book_map_coordinates_color;

unsigned int ui_book_calendar_title_color;
unsigned int ui_book_calendar_time_color;
unsigned int ui_book_calendar_day_color;
unsigned int ui_book_calendar_month_color;
unsigned int ui_book_calendar_year_color;
unsigned int ui_book_calendar_moon_color;
unsigned int ui_book_calendar_location_color;

unsigned int ui_book_journal_title_color;
unsigned int ui_book_journal_text_color;
unsigned int ui_book_journal_text_shadow;

unsigned int ui_game_dialogue_npc_name_color;
unsigned int ui_game_dialogue_option_highlight_color;
unsigned int ui_game_dialogue_option_normal_color;

unsigned int ui_house_player_cant_interact_color;


void set_default_ui_skin()
{
  ui_mainmenu_copyright_color = TargetColor(255, 255, 255);

  ui_character_tooltip_header_default_color = TargetColor(255, 255, 155);
  ui_character_default_text_color = TargetColor(255, 255, 255);
  ui_character_header_text_color = TargetColor(255, 255, 155);
  ui_character_bonus_text_color = TargetColor(0, 255, 0);
  ui_character_bonus_text_color_neg = TargetColor(255, 0, 0);

  ui_character_skill_upgradeable_color = TargetColor(0, 175, 255);
  ui_character_skill_default_color = TargetColor(255, 0, 0);
  ui_character_skill_highlight_color = TargetColor(255, 0, 0);  
  
  ui_character_stat_default_color = TargetColor(255, 255, 255);
  ui_character_stat_buffed_color = TargetColor(0,   255,   0);
  ui_character_stat_debuffed_color = TargetColor(255,  35,   0);
  
  ui_character_skillinfo_can_learn = TargetColor(255, 255, 255);
  ui_character_skillinfo_can_learn_gm = TargetColor(255, 255, 0);
  ui_character_skillinfo_cant_learn = TargetColor(255, 0, 0);
  
  ui_character_condition_normal_color = TargetColor(255, 255, 255);
  ui_character_condition_light_color = TargetColor(0, 255, 0);
  ui_character_condition_moderate_color = TargetColor(225, 205, 35);
  ui_character_condition_severe_color = TargetColor(255, 35, 0);

  ui_character_award_color[0] = TargetColor(248, 108, 160);
  ui_character_award_color[1] = TargetColor(112, 220, 248);
  ui_character_award_color[2] = TargetColor(192, 192, 240);
  ui_character_award_color[3] = TargetColor( 64, 244,  96);
  ui_character_award_color[4] = TargetColor(232, 244,  96);
  ui_character_award_color[5] = TargetColor(240, 252, 192);
  
  ui_game_minimap_outline_color = TargetColor(0, 0, 255);
  ui_game_minimap_actor_friendly_color = TargetColor(0, 255, 0);
  ui_game_minimap_actor_hostile_color = TargetColor(255, 0, 0);
  ui_game_minimap_actor_corpse_color = TargetColor(255, 255, 0);
  ui_game_minimap_decoration_color_1 = TargetColor(255, 255, 255);
  ui_game_minimap_projectile_color = TargetColor(255, 0, 0);
  ui_game_minimap_treasure_color = TargetColor(0, 0, 255);

  ui_game_character_record_playerbuff_colors[0] = TargetColor(150, 212, 255);
  ui_game_character_record_playerbuff_colors[1] = TargetColor(225, 225, 225);
  ui_game_character_record_playerbuff_colors[2] = TargetColor(255, 128, 0);
  ui_game_character_record_playerbuff_colors[3] = TargetColor(128, 128, 128);
  ui_game_character_record_playerbuff_colors[4] = TargetColor(225, 225, 225);
  ui_game_character_record_playerbuff_colors[5] = TargetColor(255, 85, 0);
  ui_game_character_record_playerbuff_colors[6] = TargetColor(255, 128, 0);
  ui_game_character_record_playerbuff_colors[7] = TargetColor(255, 85, 0);
  ui_game_character_record_playerbuff_colors[8] = TargetColor(225, 225, 225);
  ui_game_character_record_playerbuff_colors[9] = TargetColor(235, 15, 255);
  ui_game_character_record_playerbuff_colors[10] = TargetColor(192, 192, 240);
  ui_game_character_record_playerbuff_colors[11] = TargetColor(225, 225, 225);
  ui_game_character_record_playerbuff_colors[12] = TargetColor(255, 128, 0);
  ui_game_character_record_playerbuff_colors[13] = TargetColor(150, 212, 255);
  ui_game_character_record_playerbuff_colors[14] = TargetColor(128, 128, 128);
  ui_game_character_record_playerbuff_colors[15] = TargetColor(255, 255, 155);
  ui_game_character_record_playerbuff_colors[16] = TargetColor(255, 255, 155);
  ui_game_character_record_playerbuff_colors[17] = TargetColor(255, 255, 155);
  ui_game_character_record_playerbuff_colors[18] = TargetColor(255, 255, 155);
  ui_game_character_record_playerbuff_colors[19] = TargetColor(255, 255, 155);
  ui_game_character_record_playerbuff_colors[20] = TargetColor(255, 255, 155);
  ui_game_character_record_playerbuff_colors[21] = TargetColor(255, 255, 155);
  ui_game_character_record_playerbuff_colors[22] = TargetColor(0, 128, 255);
  ui_game_character_record_playerbuff_colors[23] = TargetColor(0, 128, 255);

  ui_gamemenu_video_gamma_title_color = TargetColor(255, 255, 155);
  ui_gamemenu_keys_action_name_color = TargetColor(255, 255, 255);
  ui_gamemenu_keys_key_selection_blink_color_1 = TargetColor(50, 0, 0);
  ui_gamemenu_keys_key_selection_blink_color_2 = TargetColor(225, 205, 35);
  ui_gamemenu_keys_key_default_color = TargetColor(255, 255, 255);

  ui_book_quests_title_color = TargetColor(255, 255, 255);
  ui_book_quests_text_color = TargetColor(255, 255, 255);
  ui_book_autonotes_title_color = TargetColor(255, 255, 255);
  ui_book_autonotes_text_color = TargetColor(255, 255, 255);
  ui_book_map_title_color = TargetColor(255, 255, 255);
  ui_book_map_coordinates_color = TargetColor(255, 255, 255);

  ui_book_calendar_title_color = TargetColor(255, 255, 255);
  ui_book_calendar_time_color = TargetColor(75, 75, 75);
  ui_book_calendar_day_color = TargetColor(75, 75, 75);
  ui_book_calendar_month_color = TargetColor(75, 75, 75);
  ui_book_calendar_year_color = TargetColor(75, 75, 75);
  ui_book_calendar_moon_color = TargetColor(75, 75, 75);
  ui_book_calendar_location_color = TargetColor(75, 75, 75);

  ui_book_journal_title_color = TargetColor(255, 255, 255);
  ui_book_journal_text_color = TargetColor(255, 255, 255);
  ui_book_journal_text_shadow = TargetColor(0, 0, 0);

  ui_game_dialogue_npc_name_color = TargetColor(21, 153, 233);
  ui_game_dialogue_option_highlight_color = TargetColor(225, 205, 35);
  ui_game_dialogue_option_normal_color = TargetColor(255, 255, 255);

  ui_house_player_cant_interact_color = TargetColor(255, 255, 155);
}

std::array<unsigned int, 16> papredoll_dbrds;
unsigned int papredoll_drhs[4];
unsigned int papredoll_dlhus[4];
unsigned int papredoll_dlhs[4];
unsigned int papredoll_dbods[5];
int paperdoll_armor_texture[4][17][3];//0x511294
//int paperdoll_array_51132C[165];
unsigned int papredoll_dlaus[5];
unsigned int papredoll_dlads[4];
int papredoll_flying_feet[777]; // idb
int paperdoll_boots_texture[4][6];//0x511638
int paperdoll_cloak_collar_texture[4][10]; // weak
int paperdoll_cloak_texture[4][10];
int paperdoll_helm_texture[2][16]; //511698
int paperdoll_belt_texture[4][7];  //511718

const int paperdoll_Weapon[4][16][2] = {//4E4C30
    {{128, 205},  {30, 144},  {88,  85},  {0, 0},  {0, 0},  {0, 0},  {17, 104},  {0, 0},  {0, 0},  {0, 0},  {0, 0},  {0, 0},  {0, 0},  {0, 0},  {0, 0},  {0, 0}},
    {{131, 201},  {38, 158},  {98,  87},  {0, 0},  {0, 0},  {0, 0},  {21, 100},  {0, 0},  {0, 0},  {0, 0},  {0, 0},  {0, 0},  {0, 0},  {0, 0},  {0, 0},  {0, 0}},
    {{131, 216},  {29, 186},  {88, 119},  {0, 0},  {0, 0},  {0, 0},  {  0,  0},  {0, 0},  {0, 0},  {0, 0},  {0, 0},  {0, 0},  {0, 0},  {0, 0},  {0, 0},  {0, 0}},
    {{123, 216},  {35, 184},  {98, 119},  {0, 0},  {0, 0},  {0, 0},  {  0,  0},  {0, 0},  {0, 0},  {0, 0},  {0, 0},  {0, 0},  {0, 0},  {0, 0},  {0, 0},  {0, 0}},
    };

const int paperdoll_Boot[4][7][2] = //4E5490
    {
    0xE, 0x11D,    0xD, 0x11D,    0xC, 0x10A,    0xA, 0xFF,    0xD, 0xF9,    0xD, 0x137,   0xC, 0x10E,
    0x14, 0x125,   0x13, 0x122,   0x15, 0x120,   0x15, 0x114,  0x13, 0x10A,  0x11, 0x13E,  0x11, 0x116,
    0x1D, 0x121,   0x1C, 0x11F,   0x1B, 0x11B,   0x1C, 0x117,  0x16, 0x116,  0x1B, 0x137,  0x1B, 0x11B,
    0x1F, 0x127,   0x1F, 0x122,   0x1B, 0x11B,   0x1D, 0x117,  0x1D, 0x116,  0x1D, 0x137,  0x1B, 0x11F,
    };
const int paperdoll_Cloak[4][10][2] = //4E5570
    {
    0x11, 0x68,  0xF, 0x68,  0x14, 0x71,  0x19, 0x6B,  0x21, 0x6F,  0x5, 0x68,  0x5, 0x68,  0x14, 0x71,  0x3, 0x6B,  0xF, 0x6F,
    0x15, 0x64,  0xB, 0x6B,  0xE, 0x67,   0x15, 0x6B,  0x1B, 0x6F,  0x3, 0x6B,  0, 0x6B,    0xE, 0x67,   0, 0x6B,    0x3, 0x6F,
    0x10, 0x8A,  0x9, 0x8B,  0x18, 0x98,  0x25, 0x91,  0x29, 0x90,  0x8, 0x8A,  0x9, 0x8B,  0x18, 0x98,  0x3, 0x91,  0x3, 0x90,
    0x14, 0x92,  0x10, 0x92, 0x15, 0x98,  0x1F, 0x91,  0x22, 0x90,  0x8, 0x92,  0xC, 0x92,  0x15, 0x98,  0x3, 0x91,  0x3, 0x90,
    };
const int paperdoll_CloakCollar[4][10][2] = //4E56B0
    {
    0x11, 0x68,  0x34, 0x64,  0x21, 0x69,  0x1D, 0x67,  0x20, 0x67,  0x21, 0x68,  0x34, 0x64,  0x21, 0x69,  0x1D, 0x67,  0x1F, 0x67,
    0x13, 0x64,  0x35, 0x66,  0x29, 0x68,  0x1F, 0x68,  0x1F, 0x6A,  0x21, 0x6A,  0x2B, 0x66,  0x26, 0x68,  0x1F, 0x68,  0x1F, 0x6A,
    0, 0,        0x30, 0x87,  0x1E, 0x86,  0x1B, 0x86,  0x1C, 0x8A,  0x21, 0x87,  0x30, 0x87,  0x1E, 0x86,  0x1B, 0x86,  0x1C, 0x8A,
    0, 0,        0x38, 0x8A,  0x24, 0x8B,  0x1D, 0x8B,  0x21, 0x8C,  0x27, 0x8A,  0x34, 0x8A,  0x24, 0x8B,  0x25, 0x8B,  0x21, 0x8C,
    };
//int dword_4E56B4; // weak
const int paperdoll_Belt[4][7][2] = //4E57F0
    {
    0x3A, 0xB6,  0x37, 0xB2,  0x34, 0xB9,  0x3A, 0xB9,  0x37, 0xB7,  0x38, 0xAC,  0x37, 0xB7,
    0x3E, 0xAD,  0x3A, 0xAC,  0x37, 0xB0,  0x3A, 0xB1,  0x39, 0xB0,  0x3C, 0xA5,  0x39, 0xB0,
    0x3B, 0xD5,  0x37, 0xD2,  0x31, 0xD5,  0x39, 0xD6,  0x37, 0xD8,  0x37, 0xD1,  0x37, 0xD8,
    0x42, 0xD2,  0x3F, 0xD0,  0x3B, 0xD7,  0x3C, 0xD5,  0x3B, 0xD6,  0x3E, 0xCF,  0x36, 0xD6,
    };
const int paperdoll_Helm[4][16][2] = //4E58D0
    {
    0x3E, 0x1F,  0x41, 0x2C,  0x37, 0x2F,  0x31, 0x32,  0x37, 0x2A,  0x39, 0x28,  0x36, 0x34,  0x41, 0x38,  0x40, 0x31,  0x40, 0x21,  0x40, 0x31,  0x3C, 0x33,  0x3D, 0x24,  0x3A, 0x1A,  0x37, 0x2A,  0x41, 0x48,
    0x41, 0x1E,  0x42, 0x2B,  0x37, 0x2F,  0x34, 0x30,  0x39, 0x29,  0x3A, 0x26,  0x36, 0x34,  0x41, 0x37,  0x42, 0x32,  0x40, 0x21,  0x40, 0x31,  0x40, 0x2F,  0x3E, 0x22,  0x3B, 0x1A,  0x39, 0x29,  0x42, 0x47,
    0x3F, 0x47,  0x41, 0x56,  0x37, 0x59,  0x32, 0x5E,  0x37, 0x58,  0x39, 0x54,  0x34, 0x61,  0x40, 0x61,  0x41, 0x5D,  0x3E, 0x4F,  0x3E, 0x5B,  0x3D, 0x5B,  0x3F, 0x4C,  0x3B, 0x45,  0x37, 0x58,  0x41, 0x74,
    0x45, 0x45,  0x46, 0x54,  0x3A, 0x55,  0x38, 0x58,  0x3C, 0x54,  0x3F, 0x52,  0x39, 0x5B,  0x45, 0x5C,  0x47, 0x5C,  0x44, 0x4B,  0x44, 0x57,  0x43, 0x55,  0x44, 0x4A,  0x3E, 0x45,  0x3C, 0x54,  0x47, 0x70,
    };
const int pPaperdoll_Beards[4] = //4E5AD0
    {
    52, 130, 56, 136,
    };
const int pPaperdoll_LeftHand[4][2] = //4E5AE0
    {
    0x67, 0x6A,
    0x65, 0x6C,
    0x74, 0x8D,
    0x74, 0x93,
    };
const int pPaperdoll_SecondLeftHand[4][2] = //4E5B00
    {
    0x1A, 0x6B,
    0x28, 0x6D,
    0x19, 0x8D,
    0x20, 0x92,
    };
const int pPaperdoll_RightHand[4][2] = //4E5B20
    {
    0x1E, 0x90,
    0x22, 0x9E,
    0x19, 0xBA,
    0x1F, 0xB8,
    };
const int pPaperdollLeftEmptyHand[4][2] = //4E5B40
    {
    0x80, 0xCD,
    0x83, 0xC9,
    0x83, 0xD8,
    0x7B, 0xD8,
    };

int pPaperdoll_BodyX = 481; // 004E4C28
int pPaperdoll_BodyY = 0;   // 004E4C2C
const int paperdoll_Armor_Coord[4][17][2] = //4E4E30
    {
    // X     Y
    0x2C, 0x67,  0x30, 0x69,  0x2D, 0x67,  0x2C, 0x64,  0x14, 0x66,  0x22, 0x67,  0x20, 0x66,  0x25, 0x66,  0x12, 0x66,//Human
    0x0A, 0x66,  0x13, 0x64,  0x0E, 0x64,  0x0A, 0x63,  0x14, 0x66,  0x0A, 0x63,  0x0A, 0x66,  0x25, 0x66,

    0x32, 0x68,  0x32, 0x69,  0x35, 0x69,  0x33, 0x68,  0x24, 0x67,  0x30, 0x69,  0x33, 0x68,  0x31, 0x69,  0x19, 0x69,
    0x19, 0x6A,  0x16, 0x66,  0x16, 0x65,  0x0F, 0x6B,  0x24, 0x67,  0x0F, 0x6B,  0x19, 0x6A,  0x31, 0x69,

    0x2A, 0x8C,  0x29, 0x8C,  0x2A, 0x89,  0x29, 0x86,  0x12, 0x87,  0x2D, 0x89,  0x2A, 0x88,  0x25, 0x87,  0x12, 0x8B,
    0x12, 0x8B,  0x11, 0x8A,  0x15, 0x87,  0x09, 0x89,  0x12, 0x87,  0x09, 0x89,  0x12, 0x8B,  0x25, 0x87,

    0x33, 0x90,  0x32, 0x90,  0x34, 0x91,  0x32, 0x8E,  0x21, 0x8B,  0x31, 0x8B,  0x33, 0x8E,  0x2F, 0x8F,  0x16, 0x8D,
    0x18, 0x8C,  0x19, 0x8C,  0x1B, 0x8E,  0x0C, 0x8C,  0x21, 0x8B,  0x0C, 0x8C,  0x18, 0x8C,  0x2F, 0x8F,
    };
const int paperdoll_shoulder_coord[4][17][2] = //4E5050
    {
    0x64, 0x67,  0x61, 0x67,  0x65, 0x68,  0x6E, 0x74,  0x6C, 0x68,  0x61, 0x67,  0x66, 0x68,  0x6C, 0x6A,  0x6E, 0x6D,
    0x67, 0x69,  0x70, 0x67,  0x6E, 0x6D,  0x6C, 0x6F,  0x6C, 0x68,  0x6C, 0x6F,  0x67, 0x69,  0x6C, 0x6A,

    0x60, 0x6B,  0x60, 0x6C,  0x60, 0x6B,  0x61, 0x6A,  0x60, 0x69,  0x60, 0x6A,  0x60, 0x6A,  0x61, 0x69,  0x63, 0x6A,
    0x64, 0x6A,  0x61, 0x66,  0x66, 0x67,  0x64, 0x6C,  0x60, 0x69,  0x64, 0x6C,  0x64, 0x6A,  0x61, 0x69,

    0x6D, 0x8C,  0x75, 0x8C,  0, 0,        0x72, 0x8D,  0x6A, 0x89,  0, 0,        0x73, 0x8C,  0x69, 0x8C,  0x6E, 0x8D,
    0x71, 0x8D,  0x70, 0x8D,  0x72, 0x8D,  0x74, 0x8E,  0x6A, 0x89,  0x74, 0x8E,  0x71, 0x8D,  0x69, 0x8C,

    0x72, 0x91,  0x72, 0x91,  0, 0,        0x6E, 0x92,  0x6F, 0x91,  0, 0,        0, 0,        0x6E, 0x91,  0x71, 0x90,
    0x72, 0x8D,  0x72, 0x90,  0x73, 0x93,  0x73, 0x90,  0x6F, 0x91,  0x73, 0x90,  0x72, 0x8D,  0x6E, 0x91,
    };
const int paperdoll_shoulder_second_coord[4][17][2] = //dword_4E5270
    {
    0, 0,        0x61, 0x67,  0, 0,        0x64, 0x69,  0x64, 0x68,  0, 0,        0, 0,        0x5E, 0x66,  0x5F, 0x69,
    0x55, 0x69,  0x5F, 0x67,  0x5F, 0x68,  0x32, 0x69,  0x64, 0x68,  0x32, 0x69,  0x55, 0x69,  0x5E, 0x66,

    0, 0,        0x60, 0x6C,  0, 0,        0x60, 0x6C,  0x5E, 0x69,  0, 0,        0, 0,        0x5D, 0x6A,  0x5B, 0x6A,
    0x5B, 0x6A,  0x59, 0x69,  0x56, 0x68,  0x38, 0x6E,  0x5E, 0x69,  0x38, 0x6E,  0x5B, 0x6A,  0x5D, 0x6A,

    0, 0,        0x75, 0x8C,  0, 0,        0x72, 0x8D,  0x62, 0x89,  0, 0,        0, 0,        0x69, 0x8C,  0x5E, 0x8D,
    0x61, 0x8D,  0x5F, 0x8D,  0x60, 0x8D,  0x2E, 0x8C,  0x62, 0x89,  0x2E, 0x8C,  0x61, 0x8D,  0x69, 0x8C,

    0, 0,        0x72, 0x91,  0, 0,        0x72, 0x91,  0x67, 0x8F,  0, 0,        0, 0,        0x6E, 0x91,  0x64, 0x93,
    0x65, 0x8C,  0x65, 0x91,  0x67, 0x91,  0x36, 0x90,  0x67, 0x8F,  0x36, 0x90,  0x65, 0x8C,  0x6E, 0x91,
    };

const char *dlad_texnames_by_face[25] =
    {
    "pc01lad", "pc02lad", "pc03lad", "pc04lad", "pc05lad", "pc06lad",
    "pc07lad", "pc08lad", "pc09lad", "pc10lad", "pc11lad", "pc12lad",
    "pc13lad", "pc14lad", "pc15lad", "pc16lad", "pc17lad", "pc18lad",
    "pc19lad", "pc20lad", "pc21lad", "pc22lad", "pc23lad", "pc24lad",
    "pc25lad"
    };
const char *dlau_texnames_by_face[25] =
    {
    "pc01lau", "pc02lau", "pc03lau", "pc04lau", "pc05lau", "pc06lau",
    "pc07lau", "pc08lau", "pc09lau", "pc10lau", "pc11lau", "pc12lau",
    "pc13lau", "pc14lau", "pc15lau", "pc16lau", "pc17lau", "pc18lau",
    "pc19lau", "pc20lau", "pc21lau", "pc22lau", "pc23lau", "pc24lau",
    "pc25lau"
    };
const char *dbod_texnames_by_face[25] =
    {
    "pc01bod", "pc02bod", "pc03bod", "pc04bod", "pc05bod", "pc06bod",
    "pc07bod", "pc08bod", "pc09bod", "pc10bod", "pc11bod", "pc12bod",
    "pc13bod", "pc14bod", "pc15bod", "pc16bod", "pc17bod", "pc18bod",
    "pc19bod", "pc20bod", "pc21bod", "pc22bod", "pc23bod", "pc24bod",
    "pc25bod"
    };
const char *drh_texnames_by_face[25] =
    {

    "pc01rh", "pc02rh", "pc03rh", "pc04rh", "pc05rh", "pc06rh",
    "pc07rh", "pc08rh", "pc09rh", "pc10rh", "pc11rh", "pc12rh",
    "pc13rh", "pc14rh", "pc15rh", "pc16rh", "pc17rh", "pc18rh",
    "pc19rh", "pc20rh", "pc21rh", "pc22rh", "pc23rh", "pc24rh",
    "pc25rh"
    };
const char *dlh_texnames_by_face[25] =
    {
    "pc01lh", "pc02lh", "pc03lh", "pc04lh", "pc05lh", "pc06lh",
    "pc07lh", "pc08lh", "pc09lh", "pc10lh", "pc11lh", "pc12lh",
    "pc13lh", "pc14lh", "pc15lh", "pc16lh", "pc17lh", "pc18lh",
    "pc19lh", "pc20lh", "pc21lh", "pc22lh", "pc23lh", "pc24lh",
    "pc25lh"
    };
const char *dlhu_texnames_by_face[25] =
    {
    "pc01lhu", "pc02lhu", "pc03lhu", "pc04lhu", "pc05lhu", "pc06lhu",
    "pc07lhu", "pc08lhu", "pc09lhu", "pc10lhu", "pc11lhu", "pc12lhu",
    "pc13lhu", "pc14lhu", "pc15lhu", "pc16lhu", "pc17lhu", "pc18lhu",
    "pc19lhu", "pc20lhu", "pc21lhu", "pc22lhu", "pc23lhu", "pc24lhu",
    "pc25lhu"
    };

const  int pArmorSkills[5]  = {PLAYER_SKILL_LEATHER, PLAYER_SKILL_CHAIN,      PLAYER_SKILL_PLATE,        PLAYER_SKILL_SHIELD,  PLAYER_SKILL_DODGE};
const int pWeaponSkills[9] = {PLAYER_SKILL_AXE,     PLAYER_SKILL_BOW,        PLAYER_SKILL_DAGGER,       PLAYER_SKILL_MACE,    PLAYER_SKILL_SPEAR,      
    PLAYER_SKILL_STAFF,    PLAYER_SKILL_SWORD,       PLAYER_SKILL_UNARMED,    PLAYER_SKILL_BLASTER};
const  int pMiscSkills[12]  = {PLAYER_SKILL_ALCHEMY, PLAYER_SKILL_ARMSMASTER, PLAYER_SKILL_BODYBUILDING, PLAYER_SKILL_ITEM_ID, PLAYER_SKILL_MONSTER_ID, 
    PLAYER_SKILL_LEARNING, PLAYER_SKILL_TRAP_DISARM, PLAYER_SKILL_MEDITATION, PLAYER_SKILL_MERCHANT, PLAYER_SKILL_PERCEPTION,
    PLAYER_SKILL_REPAIR, PLAYER_SKILL_STEALING};
const  int pMagicSkills[9]  = {PLAYER_SKILL_FIRE,    PLAYER_SKILL_AIR,        PLAYER_SKILL_WATER,        PLAYER_SKILL_EARTH,   PLAYER_SKILL_SPIRIT,    
    PLAYER_SKILL_MIND,     PLAYER_SKILL_BODY,        PLAYER_SKILL_LIGHT,      PLAYER_SKILL_DARK};

//----- (00421626) --------------------------------------------------------
GUIWindow *CharacterUI_Initialize(unsigned int _this)
{
  GUIWindow *pWindow; // edi@3

  ++pIcons_LOD->uTexturePacksCount;
  if ( !pIcons_LOD->uNumPrevLoadedFiles )
    pIcons_LOD->uNumPrevLoadedFiles = pIcons_LOD->uNumLoadedFiles;

  pEventTimer->Pause();
  pAudioPlayer->StopChannels(-1, -1);
  bRingsShownInCharScreen = false;
  CharacterUI_LoadPaperdollTextures();
  pCurrentScreen = _this;

  pWindow = GUIWindow::Create(0, 0, 640, 480, WINDOW_CharacterRecord, uActiveCharacter, 0);
  pCharacterScreen_StatsBtn = pWindow->CreateButton(pViewport->uViewportTL_X + 12, pViewport->uViewportTL_Y + 308,
                                pIcons_LOD->GetTexture(papredoll_dbrds[9])->uTextureWidth,
                                pIcons_LOD->GetTexture(papredoll_dbrds[9])->uTextureHeight,
                                1, 0, UIMSG_ClickStatsBtn, 0, 'S', pGlobalTXT_LocalizationStrings[216],// Stats
                                pIcons_LOD->GetTexture(papredoll_dbrds[10]),
                                pIcons_LOD->GetTexture(papredoll_dbrds[9]), 0);
  pCharacterScreen_SkillsBtn = pWindow->CreateButton(pViewport->uViewportTL_X + 102, pViewport->uViewportTL_Y + 308,
                                 pIcons_LOD->GetTexture(papredoll_dbrds[7])->uTextureWidth,
                                 pIcons_LOD->GetTexture(papredoll_dbrds[7])->uTextureHeight,
                                 1, 0, UIMSG_ClickSkillsBtn, 0, 'K', pGlobalTXT_LocalizationStrings[205],//Skills
                                 pIcons_LOD->GetTexture(papredoll_dbrds[8]),
                                 pIcons_LOD->GetTexture(papredoll_dbrds[7]), 0);
  pCharacterScreen_InventoryBtn = pWindow->CreateButton(pViewport->uViewportTL_X + 192, pViewport->uViewportTL_Y + 308,
                                    pIcons_LOD->GetTexture(papredoll_dbrds[5])->uTextureWidth,
                                    pIcons_LOD->GetTexture(papredoll_dbrds[5])->uTextureHeight,
                                    1, 0, UIMSG_ClickInventoryBtn, 0, 'I', pGlobalTXT_LocalizationStrings[120], //Inventory
                                    pIcons_LOD->GetTexture(papredoll_dbrds[6]),
                                    pIcons_LOD->GetTexture(papredoll_dbrds[5]), 0);
  pCharacterScreen_AwardsBtn = pWindow->CreateButton(pViewport->uViewportTL_X + 282, pViewport->uViewportTL_Y + 308,
                                 pIcons_LOD->GetTexture(papredoll_dbrds[3])->uTextureWidth,
                                 pIcons_LOD->GetTexture(papredoll_dbrds[3])->uTextureHeight,
                                 1, 0, UIMSG_ClickAwardsBtn, 0, 'A', pGlobalTXT_LocalizationStrings[22], //Awards
                                 pIcons_LOD->GetTexture(papredoll_dbrds[4]),
                                 pIcons_LOD->GetTexture(papredoll_dbrds[3]), 0);
  pCharacterScreen_ExitBtn = pWindow->CreateButton(pViewport->uViewportTL_X + 371, pViewport->uViewportTL_Y + 308,
                 pIcons_LOD->GetTexture(papredoll_dbrds[1])->uTextureWidth,
                 pIcons_LOD->GetTexture(papredoll_dbrds[1])->uTextureHeight,
                 1, 0, UIMSG_ClickExitCharacterWindowBtn, 0, 0, pGlobalTXT_LocalizationStrings[79],//Exit
                 pIcons_LOD->GetTexture(papredoll_dbrds[2]),
                 pIcons_LOD->GetTexture(papredoll_dbrds[1]), 0);
  pWindow->CreateButton(0, 0, 0x1DCu, 0x159u, 1, 122, UIMSG_InventoryLeftClick, 0, 0, "", 0);
  pCharacterScreen_DetalizBtn = pWindow->CreateButton(0x258u, 0x12Cu, 30, 30, 1, 0, UIMSG_ChangeDetaliz, 0, 0, pGlobalTXT_LocalizationStrings[64], 0);
  pCharacterScreen_DollBtn = pWindow->CreateButton(0x1DCu, 0, 0xA4u, 0x159u, 1, 0, UIMSG_ClickPaperdoll, 0, 0, "", 0);

  pWindow->CreateButton( 61, 424, 31, 0, 2, 94, UIMSG_SelectCharacter, 1, '1', "", 0);
  pWindow->CreateButton(177, 424, 31, 0, 2, 94, UIMSG_SelectCharacter, 2, '2', "", 0);
  pWindow->CreateButton(292, 424, 31, 0, 2, 94, UIMSG_SelectCharacter, 3, '3', "", 0);
  pWindow->CreateButton(407, 424, 31, 0, 2, 94, UIMSG_SelectCharacter, 4, '4', "", 0);

  pWindow->CreateButton(0, 0, 0, 0, 1, 0, UIMSG_CycleCharacters, 0, '\t', "", 0);
  FillAwardsData();
  return pWindow;
}

//----- (004219BE) --------------------------------------------------------
GUIWindow *CastSpellInfo::GetCastSpellInInventoryWindow()
{
  GUIWindow *CS_inventory_window; // ebx@1

  pEventTimer->Pause();
  pAudioPlayer->StopChannels(-1, -1);
  bRingsShownInCharScreen = 0;
  CharacterUI_LoadPaperdollTextures();
  pCurrentScreen = SCREEN_CASTING;
  CS_inventory_window = GUIWindow::Create(0, 0, 640, 480, WINDOW_CastSpell_InInventory, (int)this, 0);
  pCharacterScreen_ExitBtn = CS_inventory_window->CreateButton(394, 318, 75, 33, 1, 0, UIMSG_ClickExitCharacterWindowBtn, 0, 0,
                 pGlobalTXT_LocalizationStrings[79], // Close
                 pIcons_LOD->GetTexture(papredoll_dbrds[2]),
                 pIcons_LOD->GetTexture(papredoll_dbrds[1]), 0);
  CS_inventory_window->CreateButton(0, 0, 0x1DCu, 0x159u, 1, 122, UIMSG_InventoryLeftClick, 0, 0, "", 0);
  pCharacterScreen_DollBtn = CS_inventory_window->CreateButton(0x1DCu, 0, 0xA4u, 0x159u, 1, 0, UIMSG_ClickPaperdoll, 0, 0, "", 0);

  CS_inventory_window->CreateButton( 61, 424, 31, 0, 2, 94, UIMSG_SelectCharacter, 1, '1', "", 0);
  CS_inventory_window->CreateButton(177, 424, 31, 0, 2, 94, UIMSG_SelectCharacter, 2, '2', "", 0);
  CS_inventory_window->CreateButton(292, 424, 31, 0, 2, 94, UIMSG_SelectCharacter, 3, '3', "", 0);
  CS_inventory_window->CreateButton(407, 424, 31, 0, 2, 94, UIMSG_SelectCharacter, 4, '4', "", 0);

  return CS_inventory_window;
}

static int CharacterUI_SkillsTab_Draw__DrawSkillTable(Player *player, int x, int y, const int *skill_list, int skill_list_size, int right_margin, const char *skill_group_name)
{
  int y_offset = y;
  
  sprintf(pTmpBuf.data(), "%s\r%03d%s", skill_group_name, right_margin, pGlobalTXT_LocalizationStrings[131]); //"Level"
  pGUIWindow_CurrentMenu->DrawText(pFontArrus, x, y, ui_character_header_text_color, pTmpBuf.data(), 0, 0, 0);

  int num_skills_drawn = 0;
  for (int i = 0; i < skill_list_size; ++i)
  {
    auto skill = (PLAYER_SKILL_TYPE)skill_list[i];
    for (uint j = 0; j < pGUIWindow_CurrentMenu->uNumControls; ++j)
    {
      auto v8 = pGUIWindow_CurrentMenu->pControlsHead;

      for (int v7 = j; v7 > 0; --v7)
        v8 = v8->pNext;

      auto v9 = v8->field_1C;
      if ((short)(v8->field_1C) >= 0)
        continue;
      if ( (v9 & 0x7FFF) != skill )
        continue;

      ++num_skills_drawn;
      y_offset = v8->uY;

      auto skill_value = player->pActiveSkills[skill];
      auto skill_level = skill_value & 0x3F;

      uint skill_color = 0;
      uint skill_mastery_color = 0;
      if (player->uSkillPoints > skill_level)
        skill_color = ui_character_skill_upgradeable_color;

      if (pGUIWindow_CurrentMenu->pCurrentPosActiveItem == j)
      {
        if (player->uSkillPoints > skill_level)
          skill_mastery_color = ui_character_bonus_text_color;
        else
          skill_mastery_color = ui_character_skill_default_color;
        skill_color = skill_mastery_color;
      }

      if (SkillToMastery(skill_value) == 1)
      {
        sprintfex(pTmpBuf.data(), "%s\r%03d%2d", pSkillNames[skill], right_margin, skill_level);
        pGUIWindow_CurrentMenu->DrawText(pFontLucida, x, v8->uY, skill_color, pTmpBuf.data(), 0, 0, 0);
      }
      else
      {
        const char *skill_level_str = nullptr;

        switch (SkillToMastery(skill_value))
        {
          case 4: skill_level_str = pGlobalTXT_LocalizationStrings[96];  break; // "Grand"
          case 3: skill_level_str = pGlobalTXT_LocalizationStrings[432]; break; // Master
          case 2: skill_level_str = pGlobalTXT_LocalizationStrings[433]; break; // Expert
        }

        if (!skill_mastery_color)
          skill_mastery_color = ui_character_header_text_color;
        sprintfex(pTmpBuf.data(), "%s \f%05d%s\f%05d\r%03d%2d", pSkillNames[skill], skill_mastery_color, skill_level_str, skill_color, right_margin, skill_level);
        pGUIWindow_CurrentMenu->DrawText(pFontLucida, x, v8->uY, skill_color, pTmpBuf.data(), 0, 0, 0);
      }
    }
  }

  if (!num_skills_drawn)
  {
    y_offset += LOBYTE(pFontLucida->uFontHeight) - 3;
    pGUIWindow_CurrentMenu->DrawText(pFontLucida, x, y_offset, 0, pGlobalTXT_LocalizationStrings[153], 0, 0, 0); //"None"
  }

  return y_offset;
}

//----- (00419719) --------------------------------------------------------
void CharacterUI_SkillsTab_Draw(Player *player)
{
  pRenderer->DrawTextureIndexed(8, 8, pIcons_LOD->LoadTexturePtr("fr_skill", TEXTURE_16BIT_PALETTE));

  sprintfex(pTmpBuf.data(), "%s \f%05d^Pv[%s]\f00000\r177%s: \f%05d%d\f00000",
            pGlobalTXT_LocalizationStrings[206],        // Skills for
            ui_character_header_text_color,
            player->pName,
            pGlobalTXT_LocalizationStrings[207],        // Skill Points
            player->uSkillPoints ? ui_character_bonus_text_color : ui_character_default_text_color,
            player->uSkillPoints);
  pGUIWindow_CurrentMenu->DrawText(pFontArrus, 24, 18, 0, pTmpBuf.data(), 0, 0, 0);

  int y = 2 * LOBYTE(pFontLucida->uFontHeight) + 13;
  y = CharacterUI_SkillsTab_Draw__DrawSkillTable(player, 24, y, pWeaponSkills, 9, 400, pGlobalTXT_LocalizationStrings[242]); // "Weapons"

  y += 2 * LOBYTE(pFontLucida->uFontHeight) - 10;
  CharacterUI_SkillsTab_Draw__DrawSkillTable(player, 24, y, pMagicSkills, 9, 400, pGlobalTXT_LocalizationStrings[138]); // "Magic"

  y = 2 * LOBYTE(pFontLucida->uFontHeight) + 13;
  y = CharacterUI_SkillsTab_Draw__DrawSkillTable(player, 248, y, pArmorSkills, 5, 177, pGlobalTXT_LocalizationStrings[11]); // "Armor"
 
  y += 2 * LOBYTE(pFontLucida->uFontHeight) - 10;
  y = CharacterUI_SkillsTab_Draw__DrawSkillTable(player, 248, y, pMiscSkills, 12, 177, pGlobalTXT_LocalizationStrings[143]); //"Misc"
}

//----- (0041A000) --------------------------------------------------------
void CharacterUI_AwardsTab_Draw(Player *player)
{
  int items_per_page; // eax@1
  char *v6; // ebx@15
  char Source[100]; // [sp+Ch] [bp-C4h]@1
  GUIWindow awards_window; // [sp+70h] [bp-60h]@1

  pRenderer->DrawTextureIndexed(8, 8, pIcons_LOD->LoadTexturePtr("fr_award", TEXTURE_16BIT_PALETTE));
  sprintfex(pTmpBuf.data(), "%s \f%05d", pGlobalTXT_LocalizationStrings[LOCSTR_AVARDS_FOR], ui_character_header_text_color);
  sprintfex(Source, pGlobalTXT_LocalizationStrings[LOCSTR_S_THE_S], player->pName, pClassNames[player->classType]);
  strcat(pTmpBuf.data(), Source);
  strcat(pTmpBuf.data(), "\f00000");

  pGUIWindow_CurrentMenu->DrawText(pFontArrus, 24, 18, 0, pTmpBuf.data(), 0, 0, 0);
  items_per_page = books_primary_item_per_page;
  awards_window.uFrameX = 12;
  awards_window.uFrameY = 48;
  awards_window.uFrameWidth = 424;
  awards_window.uFrameHeight = 290;
  awards_window.uFrameZ = 435;
  awards_window.uFrameW = 337;
  if (BtnDown_flag && num_achieved_awards + books_primary_item_per_page < full_num_items_in_book)
    items_per_page = books_primary_item_per_page++ + 1;
  if (BtnUp_flag && items_per_page)
  {
    --items_per_page;
    books_primary_item_per_page = items_per_page;
  }

  if ( books_page_number < 0 )
  {
    items_per_page += num_achieved_awards;
    books_primary_item_per_page = items_per_page;
    if ( (signed int)(num_achieved_awards + items_per_page) > full_num_items_in_book )
    {
      items_per_page = full_num_items_in_book - num_achieved_awards;
      books_primary_item_per_page = items_per_page;
    }
  }
  else if ( books_page_number > 0 )
  {
    items_per_page -= num_achieved_awards;
    books_primary_item_per_page = items_per_page;
    if ( items_per_page < 0 )
    {
      items_per_page = 0;
      books_primary_item_per_page = items_per_page;
    }
  }
  BtnDown_flag = 0;
  BtnUp_flag = 0;
  num_achieved_awards = 0;
  books_page_number = 0;

  for ( int i = items_per_page; i < full_num_items_in_book; ++i)
  {
    v6 = (char *)pAwards[achieved_awards[i]].pText;//(char *)dword_723E80_award_related[v20 / 4];
    pTmpBuf[0] = 0;
    switch (achieved_awards[i])
    {
        case Award_Arena_PageWins:    sprintf(pTmpBuf.data(), v6, pParty->uNumArenaPageWins);     break;
        case Award_Arena_SquireWins:  sprintf(pTmpBuf.data(), v6, pParty->uNumArenaSquireWins);   break;
        case Award_Arena_KnightWins:  sprintf(pTmpBuf.data(), v6, pParty->uNumArenaKnightWins);   break;
        case Award_Arena_LordWins:    sprintf(pTmpBuf.data(), v6, pParty->uNumArenaLordWins);     break;
        case Award_ArcomageWins:      sprintf(pTmpBuf.data(), v6, pParty->uNumArcomageWins);      break;
        case Award_ArcomageLoses:     sprintf(pTmpBuf.data(), v6, pParty->uNumArcomageLoses);     break;
        case Award_Deaths:            sprintf(pTmpBuf.data(), v6, pParty->uNumDeaths);            break;
        case Award_BountiesCollected: sprintf(pTmpBuf.data(), v6, pParty->uNumBountiesCollected); break;
        case Award_Fine:              sprintf(pTmpBuf.data(), v6, pParty->uFine);                 break;
        case Award_PrisonTerms:       sprintf(pTmpBuf.data(), v6, pParty->uNumPrisonTerms);       break;
    }

    if (*pTmpBuf.data())
      v6 = pTmpBuf.data();

    awards_window.DrawText(pFontArrus, 0, 0, ui_character_award_color[pAwards[achieved_awards[i]].uPriority % 6], v6, 0, 0, 0);
    awards_window.uFrameY = pFontArrus->CalcTextHeight(v6, &awards_window, 0, 0) + awards_window.uFrameY + 8;
    if (awards_window.uFrameY > awards_window.uFrameHeight)
      break;

    ++num_achieved_awards;
  }
}

//----- (0041A2C1) --------------------------------------------------------
unsigned int __fastcall GetSizeInInventorySlots(unsigned int uNumPixels)
{
  if ( (signed int)uNumPixels < 14 )
    uNumPixels = 14;
  return ((signed int)(uNumPixels - 14) >> 5) + 1;
}

//----- (0041A556) --------------------------------------------------------
void draw_leather()
{
  pRenderer->DrawTextureIndexed(8, 8, pIcons_LOD->GetTexture(uTextureID_Leather));
}

//----- (0041ABFD) --------------------------------------------------------
void CharacterUI_CharacterScreen_Draw(Player *player)
{
  pRenderer->ClearZBuffer(0, 479);
  switch (pWindowList_at_506F50_minus1_indexing_buttons____and_an_int_[0])
  {
    case WINDOW_CharacterWindow_Stats:                                // stats
      CharacterUI_ReleaseButtons();
      ReleaseAwardsScrollBar();
      CharacterUI_StatsTab_Draw(player);
      pRenderer->DrawTextureIndexed(pCharacterScreen_StatsBtn->uX,
                                    pCharacterScreen_StatsBtn->uY,
                                    pIcons_LOD->LoadTexturePtr("ib-cd1-d", TEXTURE_16BIT_PALETTE));
    break;

    case WINDOW_CharacterWindow_Skills:                                // skills
      if (dword_507CC0_activ_ch != uActiveCharacter)
      {
        CharacterUI_ReleaseButtons();
        CharacterUI_SkillsTab_CreateButtons();
      }
      ReleaseAwardsScrollBar();
      CharacterUI_SkillsTab_Draw(player);
      pRenderer->DrawTextureIndexed(pCharacterScreen_SkillsBtn->uX,
                                    pCharacterScreen_SkillsBtn->uY,
                                    pIcons_LOD->LoadTexturePtr("ib-cd2-d", TEXTURE_16BIT_PALETTE));
    break;

    case WINDOW_CharacterWindow_Awards:                                // awards
      CharacterUI_ReleaseButtons();
      ReleaseAwardsScrollBar();
      CreateAwardsScrollBar();
      CharacterUI_AwardsTab_Draw(player);
      pRenderer->DrawTextureIndexed(pCharacterScreen_AwardsBtn->uX,
                                    pCharacterScreen_AwardsBtn->uY,
                                    pIcons_LOD->LoadTexturePtr("ib-cd4-d", TEXTURE_16BIT_PALETTE));
    break;

    case WINDOW_CharacterWindow_Inventory:                             // inventory and other
      CharacterUI_ReleaseButtons();
      ReleaseAwardsScrollBar();
      CharacterUI_InventoryTab_Draw(player, false);
      pRenderer->DrawTextureIndexed(pCharacterScreen_InventoryBtn->uX,
                                    pCharacterScreen_InventoryBtn->uY,
                                    pIcons_LOD->LoadTexturePtr("ib-cd3-d", TEXTURE_16BIT_PALETTE));
    break;

    default: break;
  }

  if (bRingsShownInCharScreen)
    CharacterUI_DrawPaperdollWithRingOverlay(player);
  else
    CharacterUI_DrawPaperdoll(player);
}

//----- (0043CC7C) --------------------------------------------------------
void CharacterUI_DrawPaperdoll(Player *player)
{
  ItemGen *item; // edi@38
  int item_X; // ebx@38
  int index; // eax@65
  int v59; // ebx@129
  unsigned int v75; // ebx@170
  int pArmorShoulderNum; // eax@197
  int v94; // ebx@214
  unsigned int v127; // ebx@314
  unsigned int v153; // eax@370
  char *v166; // [sp-8h] [bp-54h]@16
  const char *container; // [sp-8h] [bp-54h]@79
  char *v181; // [sp-8h] [bp-54h]@337
  int item_Y; // [sp+10h] [bp-3Ch]@38
  int pBodyComplection; // [sp+24h] [bp-28h]@6
  signed int v245; // [sp+34h] [bp-18h]@361
  signed int IsDwarf; // [sp+40h] [bp-Ch]@4

  pIcons_LOD->LoadTexture("sptext01", TEXTURE_16BIT_PALETTE);
  if (player->GetRace() == CHARACTER_RACE_DWARF)
  {
    IsDwarf = 1;
    pBodyComplection = player->GetSexByVoice() == SEX_MALE ? 2 : 3;
  }
  else
  {
    IsDwarf = 0;
    pBodyComplection = player->GetSexByVoice() == SEX_MALE ? 0 : 1;
  }

  int uPlayerID = 0;
  for (uint i = 0; i < 4; ++i)
    if (pPlayers[i + 1] == player)
    {
      uPlayerID = i + 1;
      break;
    }

  pRenderer->ResetTextureClipRect();
  pRenderer->DrawTextureIndexed(467, 0, pIcons_LOD->GetTexture(uTextureID_BACKDOLL));//Подложка
  if ( IsPlayerWearingWatersuit[uPlayerID] )//акваланг
  {
    pRenderer->DrawTextureTransparent(pPaperdoll_BodyX, pPaperdoll_BodyY, pIcons_LOD->GetTexture(papredoll_dbods[uPlayerID - 1]));
    if ( !bRingsShownInCharScreen )
      pRenderer->DrawMaskToZBuffer(pPaperdoll_BodyX, pPaperdoll_BodyY, pIcons_LOD->GetTexture(papredoll_dbods[uPlayerID - 1]), player->pEquipment.uArmor);
    //Рука не занята или ...
    if ( !player->GetItem(&PlayerEquipment::uMainHand)
         || ( player->GetMainHandItem()->GetItemEquipType() != EQUIP_MAIN_HAND)
         && (player->GetMainHandItem()->GetItemEquipType() != PLAYER_SKILL_SPEAR
         || player->GetItem(&PlayerEquipment::uShield)) )
      pRenderer->DrawTextureTransparent(pPaperdoll_BodyX + pPaperdoll_LeftHand[pBodyComplection][0], pPaperdoll_BodyY + pPaperdoll_LeftHand[pBodyComplection][1],
                                        pIcons_LOD->GetTexture(papredoll_dlads[uPlayerID - 1]));
    //-----------------------------------------------------(Hand/Рука)---------------------------------------------------------------
    if ( player->GetItem(&PlayerEquipment::uMainHand) )
    {
      item = player->GetMainHandItem();
      item_X = pPaperdoll_BodyX + paperdoll_Weapon[pBodyComplection][1][0] - pItemsTable->pItems[item->uItemID].uEquipX;
      item_Y = pPaperdoll_BodyY + paperdoll_Weapon[pBodyComplection][1][1] - pItemsTable->pItems[item->uItemID].uEquipY;
      if ( item->uItemID == 64 )  //blaster
        v166 = "item64v1";
      else
        v166 = item->GetIconName();
      if ( !( item->uAttributes & 0xF0 ) )
      {
        if ( item->uAttributes & 2 )
          pRenderer->DrawTransparentRedShade(item_X, item_Y, pIcons_LOD->GetTexture(pIcons_LOD->LoadTexture(v166, TEXTURE_16BIT_PALETTE)));
        else
        {
          if ( item->uAttributes & 1 )
            pRenderer->DrawTextureTransparent(item_X, item_Y, pIcons_LOD->GetTexture(pIcons_LOD->LoadTexture(v166, TEXTURE_16BIT_PALETTE)));
          else
            pRenderer->DrawTransparentGreenShade(item_X, item_Y, pIcons_LOD->GetTexture(pIcons_LOD->LoadTexture(v166, TEXTURE_16BIT_PALETTE)));
        }
      }
      if ( item->uAttributes & 0xF0 )
      {
        if ( ( item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_RED )
          container = "sptext01";
        if ( ( item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_BLUE )
          container = "sp28a";
        if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_GREEN )
          container = "sp30a";
        if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_PURPLE )
          container = "sp91a";
        _50C9A8_item_enchantment_timer -= pEventTimer->uTimeElapsed;
        if ( _50C9A8_item_enchantment_timer <= 0 )
        {
          _50C9A8_item_enchantment_timer = 0;
          item->uAttributes &= 0xFFFFFF0Fu;
          ptr_50C9A4_ItemToEnchant = 0;
        }
        pRenderer->DrawAura(item_X, item_Y, pIcons_LOD->GetTexture(pIcons_LOD->LoadTexture(v166, TEXTURE_16BIT_PALETTE)),
                           pIcons_LOD->LoadTexturePtr(container, TEXTURE_16BIT_PALETTE), GetTickCount() * 0.1, 0, 255);
      }
      if ( !bRingsShownInCharScreen )
        pRenderer->DrawMaskToZBuffer(item_X, item_Y, pIcons_LOD->GetTexture(pIcons_LOD->LoadTexture(v166, TEXTURE_16BIT_PALETTE)), player->pEquipment.uMainHand);
    }
  }
  else// без акваланга
  {
    //----------------(Bow/ Лук)-------------------------------------------------
    if ( player->GetItem(&PlayerEquipment::uBow) )
    {
      item = player->GetBowItem();
      item_X = pPaperdoll_BodyX + paperdoll_Weapon[pBodyComplection][2][0] - pItemsTable->pItems[item->uItemID].uEquipX;
      item_Y = pPaperdoll_BodyY + paperdoll_Weapon[pBodyComplection][2][1] - pItemsTable->pItems[item->uItemID].uEquipY;
      if ( !(item->uAttributes & 0xF0) )// если не применён закл
      {
        if ( item->uAttributes & 2 )
          pRenderer->DrawTransparentRedShade(item_X, item_Y, pIcons_LOD->GetTexture(pIcons_LOD->LoadTexture(item->GetIconName(), TEXTURE_16BIT_PALETTE)));
        else
        {
          if ( !(item->uAttributes & 1) )//не опознанный лук зелёный
            pRenderer->DrawTransparentGreenShade(item_X, item_Y, pIcons_LOD->GetTexture(pIcons_LOD->LoadTexture(item->GetIconName(), TEXTURE_16BIT_PALETTE)));
          else // опознанный лук
            pRenderer->DrawTextureTransparent(item_X, item_Y, pIcons_LOD->GetTexture(pIcons_LOD->LoadTexture(item->GetIconName(), TEXTURE_16BIT_PALETTE)));
        }
      }
      else
      {
        if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_RED )
          container = "sptext01";
        if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_BLUE )
          container = "sp28a";
        if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_GREEN )
          container = "sp30a";
        if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_PURPLE )
          container = "sp91a";
        _50C9A8_item_enchantment_timer -= pEventTimer->uTimeElapsed;
        if ( _50C9A8_item_enchantment_timer <= 0 )
        {
          _50C9A8_item_enchantment_timer = 0;
          item->uAttributes &= 0xFFFFFF0Fu;
          ptr_50C9A4_ItemToEnchant = 0;
        }
        pRenderer->DrawAura(item_X, item_Y, pIcons_LOD->GetTexture(pIcons_LOD->LoadTexture(item->GetIconName(), TEXTURE_16BIT_PALETTE)),
                  pIcons_LOD->LoadTexturePtr(container, TEXTURE_16BIT_PALETTE),
                           GetTickCount() * 0.1, 0, 255);
      }
      if ( !bRingsShownInCharScreen )
        pRenderer->DrawMaskToZBuffer(item_X, item_Y, pIcons_LOD->GetTexture(pIcons_LOD->LoadTexture(item->GetIconName(), TEXTURE_16BIT_PALETTE)),
                player->pEquipment.uBow);
    }
    //-----------------------------(Cloak/Плащ)---------------------------------------------------------
    if ( player->GetItem(&PlayerEquipment::uCloak) )
    {
      item = player->GetCloakItem();
      switch ( item->uItemID )
      {
        case ITEM_RELIC_TWILIGHT:
          index = 5;
          break;
        case ITEM_ARTIFACT_CLOAK_OF_THE_SHEEP:
          index = 6;
          break;
        case ITEM_RARE_SUN_CLOAK:
          index = 7;
          break;
        case ITEM_RARE_MOON_CLOAK:
          index = 8;
          break;
        case ITEM_RARE_VAMPIRES_CAPE:
          index = 9;
          break;
        default:
          index = item->uItemID - 105;
          break;
      }
      if ( index >= 0 && index < 10 )
      {
        item_X = pPaperdoll_BodyX + paperdoll_Cloak[pBodyComplection][index][0];
        item_Y = pPaperdoll_BodyY + paperdoll_Cloak[pBodyComplection][index][1];
        if ( !(item->uAttributes & 0xF0) )
        {
          if ( item->uAttributes & 2 )
            pRenderer->DrawTransparentRedShade(item_X, item_Y, pIcons_LOD->GetTexture(paperdoll_cloak_texture[pBodyComplection][index]));
          else
            pRenderer->DrawTextureTransparent(item_X, item_Y, pIcons_LOD->GetTexture(paperdoll_cloak_texture[pBodyComplection][index]));
        }
        else
        {
          if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_RED )
            container = "sptext01";
          if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_BLUE )
            container = "sp28a";
          if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_GREEN )
            container = "sp30a";
          if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_PURPLE )
            container = "sp91a";
          _50C9A8_item_enchantment_timer -= pEventTimer->uTimeElapsed;
          if ( _50C9A8_item_enchantment_timer <= 0 )
          {
            _50C9A8_item_enchantment_timer = 0;
            item->uAttributes &= 0xFFFFFF0Fu;
            ptr_50C9A4_ItemToEnchant = 0;
          }
          pRenderer->DrawAura(item_X, item_Y, pIcons_LOD->GetTexture(paperdoll_cloak_texture[pBodyComplection][index]),
                           pIcons_LOD->LoadTexturePtr(container, TEXTURE_16BIT_PALETTE), GetTickCount() * 0.1, 0, 255);
        }
        if ( !bRingsShownInCharScreen )
          pRenderer->DrawMaskToZBuffer(item_X, item_Y, pIcons_LOD->GetTexture(paperdoll_cloak_texture[pBodyComplection][index]),
                                       player->pEquipment.uCloak);
      }
    }
    //-------------------------------(Paperdoll/Кукла)-------------------------------------------
    pRenderer->DrawTextureTransparent(pPaperdoll_BodyX, pPaperdoll_BodyY, pIcons_LOD->GetTexture(papredoll_dbods[uPlayerID - 1]));
    //-------------------------------(Armor/Броня)-----------------------------------------------
    if ( player->GetItem(&PlayerEquipment::uArmor) )
    {
      item = player->GetArmorItem();
      switch ( item->uItemID )
      {
        case ITEM_ARTIFACT_GOVERNORS_ARMOR:
          index = 15;
          break;
        case ITEM_ARTIFACT_YORUBA:
          index = 14;
          break;
        case ITEM_RELIC_HARECS_LEATHER:
          index = 13;
          break;
        case ITEM_ELVEN_CHAINMAIL:
          index = 16;
          break;
        default:
          index = item->uItemID - 66;
          break;
      }
      if ( index >= 0 && index < 17 )
      {
        item_X = pPaperdoll_BodyX + paperdoll_Armor_Coord[pBodyComplection][index][0];
        item_Y = pPaperdoll_BodyY + paperdoll_Armor_Coord[pBodyComplection][index][1];
        if ( !(item->uAttributes & 0xF0) )
        {
          if ( item->uAttributes & 2 )
            pRenderer->DrawTransparentRedShade(item_X, item_Y, pIcons_LOD->GetTexture(paperdoll_armor_texture[pBodyComplection][index][0]));
          else
          {
            if ( !(item->uAttributes & 1) )
              pRenderer->DrawTransparentGreenShade(item_X, item_Y, &pIcons_LOD->pTextures[paperdoll_armor_texture[pBodyComplection][index][0]]);
            else
              pRenderer->DrawTextureTransparent(item_X, item_Y, &pIcons_LOD->pTextures[paperdoll_armor_texture[pBodyComplection][index][0]]);
          }
        }
        else
        {
          if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_RED )
            container = "sptext01";
          if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_BLUE )
            container = "sp28a";
          if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_GREEN )
            container = "sp30a";
          if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_PURPLE )
            container = "sp91a";
          _50C9A8_item_enchantment_timer -= pEventTimer->uTimeElapsed;
          if ( _50C9A8_item_enchantment_timer <= 0 )
          {
            _50C9A8_item_enchantment_timer = 0;
            item->uAttributes &= 0xFFFFFF0Fu;
            ptr_50C9A4_ItemToEnchant = 0;
          }
          pRenderer->DrawAura(item_X, item_Y, pIcons_LOD->GetTexture(paperdoll_armor_texture[pBodyComplection][index][0]),
                          pIcons_LOD->LoadTexturePtr(container, TEXTURE_16BIT_PALETTE), GetTickCount() * 0.1, 0, 255);
        }

        if ( !bRingsShownInCharScreen )
          pRenderer->DrawMaskToZBuffer(item_X, item_Y, &pIcons_LOD->pTextures[paperdoll_armor_texture[pBodyComplection][index][0]],
                                       player->pEquipment.uArmor);
      }
    }
    //----------------------------------(End of Armor/Конец Брони)------------------------------------------
    //----------------------------------(Boot/Обувь)--------------------------------------------------------
    if ( player->GetItem(&PlayerEquipment::uBoot) )
    {
      item = player->GetBootItem();
      switch ( item->uItemID )
      {
        case ITEM_ARTIFACT_HERMES_SANDALS:
          index = 5;
          v59 = papredoll_flying_feet[player->uCurrentFace];
          break;
        case ITEM_ARTIFACT_LEAGUE_BOOTS:
          index = 6;
          v59 = paperdoll_boots_texture[pBodyComplection][5];
          break;
        default:
          index = item->uItemID - 115;
          v59 = paperdoll_boots_texture[pBodyComplection][index];
          break;
      }
      if ( index >= 0 && index < 7 )
      {
        item_X = pPaperdoll_BodyX + paperdoll_Boot[pBodyComplection][index][0];
        item_Y = pPaperdoll_BodyY + paperdoll_Boot[pBodyComplection][index][1];
        if ( !(item->uAttributes & 0xF0) )
        {
          if ( item->uAttributes & 2 )
            pRenderer->DrawTransparentRedShade(item_X, item_Y, pIcons_LOD->GetTexture(v59));
          else
          {
            if ( item->uAttributes & 1 )
              pRenderer->DrawTextureTransparent(item_X, item_Y, pIcons_LOD->GetTexture(v59));
            else
              pRenderer->DrawTransparentGreenShade(item_X, item_Y, pIcons_LOD->GetTexture(v59));
          }
        }
        else
        {
          if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_RED )
            container = "sptext01";
          if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_BLUE )
            container = "sp28a";
          if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_GREEN )
            container = "sp30a";
          if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_PURPLE )
            container = "sp91a";
          _50C9A8_item_enchantment_timer -= pEventTimer->uTimeElapsed;
          if ( _50C9A8_item_enchantment_timer <= 0 )
          {
            _50C9A8_item_enchantment_timer = 0;
            item->uAttributes &= 0xFFFFFF0Fu;
            ptr_50C9A4_ItemToEnchant = 0;
          }
          pRenderer->DrawAura(item_X, item_Y, pIcons_LOD->GetTexture(v59), pIcons_LOD->LoadTexturePtr(container, TEXTURE_16BIT_PALETTE),
                  GetTickCount() * 0.1, 0, 255);
        }
        if ( !bRingsShownInCharScreen )
          pRenderer->DrawMaskToZBuffer(item_X, item_Y, pIcons_LOD->GetTexture(v59), player->pEquipment.uBoot);
      }
    }
    //--------------------------------------------(Hand/Рука)------------------------------------------------------
    if ( !player->GetItem(&PlayerEquipment::uMainHand)
        || ( player->GetMainHandItem()->GetItemEquipType() != EQUIP_MAIN_HAND)
        && (player->GetMainHandItem()->GetPlayerSkillType() != PLAYER_SKILL_SPEAR
        || player->GetItem(&PlayerEquipment::uShield)) )
      pRenderer->DrawTextureTransparent(pPaperdoll_BodyX + pPaperdoll_LeftHand[pBodyComplection][0],
                            pPaperdoll_BodyY + pPaperdoll_LeftHand[pBodyComplection][1], pIcons_LOD->GetTexture(papredoll_dlads[uPlayerID - 1]));
    //--------------------------------------------(Belt/Пояс)-------------------------------------------------------
      if ( player->GetItem(&PlayerEquipment::uBelt))
      {
        item = player->GetBeltItem();
        switch ( item->uItemID )
        {
          case ITEM_RILIC_TITANS_BELT:
            index = 5;
            break;
          case ITEM_ARTIFACT_HEROS_BELT:
            index = 6;
            break;
          default:
            index = item->uItemID - 100;
            break;
        }
        if ( index >= 0 && index < 7 )
        {
          item_X = pPaperdoll_BodyX + paperdoll_Belt[pBodyComplection][index][0];
          item_Y = pPaperdoll_BodyY + paperdoll_Belt[pBodyComplection][index][1];
          if ( IsDwarf != 1 || index == 5 )
            v75 = paperdoll_belt_texture[pBodyComplection][index];
          else
            v75 = paperdoll_belt_texture[pBodyComplection - 2][index];
          if ( !(item->uAttributes & 0xF0) )
          {
            if ( item->uAttributes & 2 )
              pRenderer->DrawTransparentRedShade(item_X, item_Y, pIcons_LOD->GetTexture(v75));
            else
            {
              if ( item->uAttributes & 1 )
                pRenderer->DrawTextureTransparent(item_X, item_Y, pIcons_LOD->GetTexture(v75));
              else
                pRenderer->DrawTransparentGreenShade(item_X, item_Y, pIcons_LOD->GetTexture(v75));
            }
            if ( !bRingsShownInCharScreen )
              pRenderer->DrawMaskToZBuffer(item_X, item_Y, pIcons_LOD->GetTexture(v75), player->pEquipment.uBelt);
          }
          else
          {
            if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_RED )
              container = "sptext01";
            if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_BLUE )
              container = "sp28a";
            if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_GREEN )
              container = "sp30a";
            if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_PURPLE )
              container = "sp91a";
            _50C9A8_item_enchantment_timer -= pEventTimer->uTimeElapsed;
            if ( _50C9A8_item_enchantment_timer <= 0 )
            {
              _50C9A8_item_enchantment_timer = 0;
              item->uAttributes &= 0xFFFFFF0Fu;
              ptr_50C9A4_ItemToEnchant = 0;
            }
            pRenderer->DrawAura(item_X, item_Y, pIcons_LOD->GetTexture(v75), pIcons_LOD->LoadTexturePtr(container, TEXTURE_16BIT_PALETTE),
                        GetTickCount() * 0.1, 0, 255);
          }
        }
      }
      //---------------------------------------------(Hand2/Рука2)--------------------------------------------------
      if ( player->GetItem(&PlayerEquipment::uMainHand) )
      {
        if ( player->GetMainHandItem()->GetItemEquipType() == EQUIP_MAIN_HAND
             || player->GetMainHandItem()->GetPlayerSkillType() == PLAYER_SKILL_SPEAR
             && !player->GetItem(&PlayerEquipment::uShield) )
          pRenderer->DrawTextureTransparent(pPaperdoll_BodyX + pPaperdoll_SecondLeftHand[pBodyComplection][0],
                     pPaperdoll_BodyY + pPaperdoll_SecondLeftHand[pBodyComplection][1], pIcons_LOD->GetTexture(papredoll_dlaus[uPlayerID - 1]));
      }
      //--------------------------------(Shoulder/Плечи)---------------------------------------------
      if (player->GetItem(&PlayerEquipment::uArmor))
      {
        item = player->GetArmorItem();
        switch ( item->uItemID )
        {
        case ITEM_ARTIFACT_GOVERNORS_ARMOR:
          index = 15;
          break;
        case ITEM_ARTIFACT_YORUBA:
          index = 14;
          break;
        case ITEM_RELIC_HARECS_LEATHER:
          index = 13;
          break;
        case ITEM_ELVEN_CHAINMAIL:
          index = 16;
          break;
        default:
          index = item->uItemID - 66;
          break;
        }
        if ( index >= 0 && index < 17 )
        {
          if ( player->GetItem(&PlayerEquipment::uMainHand)
            && (player->GetMainHandItem()->GetItemEquipType() == EQUIP_MAIN_HAND
            ||  player->GetMainHandItem()->GetPlayerSkillType() == PLAYER_SKILL_SPEAR
            && !player->GetItem(&PlayerEquipment::uShield) ))//без щита
          {
            v94 = paperdoll_armor_texture[pBodyComplection][index][2];
            if ( paperdoll_armor_texture[pBodyComplection][index][2] == pIcons_LOD->FindTextureByName("pending") )
            {
              v94 = paperdoll_armor_texture[pBodyComplection][index][1];
              item_X = pPaperdoll_BodyX + paperdoll_shoulder_coord[pBodyComplection][index][0];
              item_Y = pPaperdoll_BodyY + paperdoll_shoulder_coord[pBodyComplection][index][1];
            }
            else
            {
              item_X = pPaperdoll_BodyX + paperdoll_shoulder_second_coord[pBodyComplection][index][0];
              item_Y = pPaperdoll_BodyY + paperdoll_shoulder_second_coord[pBodyComplection][index][1];
            }
            if ( v94 != pIcons_LOD->FindTextureByName("pending") )
            {
              if ( !(item->uAttributes & 0xF0) )
              {
                if ( item->uAttributes & 2 )
                  pRenderer->DrawTransparentRedShade(item_X, item_Y, pIcons_LOD->GetTexture(v94));
                else
                {
                  if ( item->uAttributes & 1 )
                    pRenderer->DrawTextureTransparent(item_X, item_Y, pIcons_LOD->GetTexture(v94));
                  else
                    pRenderer->DrawTransparentGreenShade(item_X, item_Y, pIcons_LOD->GetTexture(v94));
                }
              }
              else
              {
                if ( paperdoll_armor_texture[pBodyComplection][index][2] != pIcons_LOD->FindTextureByName("pending") )
                {
                  if ( item->uAttributes & 0xF0 )
                  {
                    if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_RED )
                      container = "sptext01";
                    if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_BLUE )
                      container = "sp28a";
                    if ( ( item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_GREEN )
                      container = "sp30a";
                    if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_PURPLE )
                      container = "sp91a";
                    _50C9A8_item_enchantment_timer -= pEventTimer->uTimeElapsed;
                    if ( _50C9A8_item_enchantment_timer <= 0 )
                    {
                      _50C9A8_item_enchantment_timer = 0;
                      item->uAttributes &= 0xFFFFFF0Fu;
                      ptr_50C9A4_ItemToEnchant = 0;
                    }
                    pRenderer->DrawAura(item_X, item_Y, pIcons_LOD->GetTexture(v94), pIcons_LOD->LoadTexturePtr(container, TEXTURE_16BIT_PALETTE),
                      GetTickCount() * 0.1, 0, 255);
                  }
                }
              }
            }
          }
          else//без ничего или с щитом
          {
            //v94 = paperdoll_armor_texture[pBodyComplection][index][1];
            if ( paperdoll_armor_texture[pBodyComplection][index][1] != pIcons_LOD->FindTextureByName("pending") )
            {
              item_X = pPaperdoll_BodyX + paperdoll_shoulder_coord[pBodyComplection][index][0];
              item_Y = pPaperdoll_BodyY + paperdoll_shoulder_coord[pBodyComplection][index][1];
              if ( !(item->uAttributes & 0xF0) )
              {
                if ( item->uAttributes & 2 )
                  pRenderer->DrawTransparentRedShade(item_X, item_Y, pIcons_LOD->GetTexture(paperdoll_armor_texture[pBodyComplection][index][1]));
                else
                {
                  if ( item->uAttributes & 1 )
                    pRenderer->DrawTextureTransparent(item_X, item_Y, pIcons_LOD->GetTexture(paperdoll_armor_texture[pBodyComplection][index][1]));
                  else
                    pRenderer->DrawTransparentGreenShade(item_X, item_Y, pIcons_LOD->GetTexture(paperdoll_armor_texture[pBodyComplection][index][1]));
                }
              }
              else
              {
                if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_RED )
                  container = "sptext01";
                if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_BLUE )
                  container = "sp28a";
                if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_GREEN )
                  container = "sp30a";
                if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_PURPLE )
                  container = "sp91a";
                _50C9A8_item_enchantment_timer -= pEventTimer->uTimeElapsed;
                if ( _50C9A8_item_enchantment_timer <= 0 )
                {
                  _50C9A8_item_enchantment_timer = 0;
                  item->uAttributes &= 0xFFFFFF0Fu;
                  ptr_50C9A4_ItemToEnchant = 0;
                }
                pRenderer->DrawAura(item_X, item_Y, pIcons_LOD->GetTexture(paperdoll_armor_texture[pBodyComplection][index][1]),
                  pIcons_LOD->LoadTexturePtr(container, TEXTURE_16BIT_PALETTE), GetTickCount() * 0.1, 0, 255);
              }
            }
          }
        }
      }
      //----------------------------------------------(Cloak collar/воротник плаща)-------------------------------------
      if ( player->GetItem(&PlayerEquipment::uCloak) )
      {
        item = player->GetCloakItem();
        switch ( item->uItemID )
        {
          case ITEM_RELIC_TWILIGHT:
            index = 5;
            break;
          case ITEM_ARTIFACT_CLOAK_OF_THE_SHEEP:
            index = 6;
            break;
          case ITEM_RARE_SUN_CLOAK:
            index = 7;
            break;
          case ITEM_RARE_MOON_CLOAK:
            index = 8;
            break;
          case ITEM_RARE_VAMPIRES_CAPE:
            index = 9;
            break;
          default:
            index = item->uItemID - 105;
        }
        if ( index >= 0 && index < 10 )
        {
          item_X = pPaperdoll_BodyX + paperdoll_CloakCollar[pBodyComplection][index][0];
          item_Y = pPaperdoll_BodyY + paperdoll_CloakCollar[pBodyComplection][index][1];
          if ( paperdoll_cloak_collar_texture[pBodyComplection][index] != pIcons_LOD->FindTextureByName("pending") )
          {
            if ( !(item->uAttributes & 0xF0) )
            {
              if ( item->uAttributes & 2 )
                pRenderer->DrawTransparentRedShade(item_X, item_Y, pIcons_LOD->GetTexture(paperdoll_cloak_collar_texture[pBodyComplection][index]));
              else
                pRenderer->DrawTextureTransparent(item_X, item_Y, pIcons_LOD->GetTexture(paperdoll_cloak_collar_texture[pBodyComplection][index]));
              if ( !bRingsShownInCharScreen )
                pRenderer->DrawMaskToZBuffer(item_X, item_Y, pIcons_LOD->GetTexture(paperdoll_cloak_collar_texture[pBodyComplection][index]),
                             player->pEquipment.uCloak);
            }
            else
            {
              if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_RED )
                container = "sptext01";
              if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_BLUE )
                container = "sp28a";
              if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_GREEN )
                container = "sp30a";
              if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_PURPLE )
                container = "sp91a";
              _50C9A8_item_enchantment_timer -= pEventTimer->uTimeElapsed;
              if ( _50C9A8_item_enchantment_timer <= 0 )
              {
                _50C9A8_item_enchantment_timer = 0;
                item->uAttributes &= 0xFFFFFF0Fu;
                ptr_50C9A4_ItemToEnchant = 0;
              }
              pRenderer->DrawAura(item_X, item_Y, pIcons_LOD->GetTexture(paperdoll_cloak_collar_texture[pBodyComplection][index]),
                            pIcons_LOD->LoadTexturePtr(container, TEXTURE_16BIT_PALETTE), GetTickCount() * 0.1, 0, 255);
            }
          }
        }
      }
      //--------------------------------------------(Beards/Борода)-------------------------------------------------------
      if ( player->uCurrentFace == 12 || player->uCurrentFace == 13 )
      {
        if ( papredoll_dbrds[player->uCurrentFace] != pIcons_LOD->FindTextureByName("Pending") )
          pRenderer->DrawTextureTransparent(pPaperdoll_BodyX + pPaperdoll_Beards[2 * player->uCurrentFace - 24],
                     pPaperdoll_BodyY + pPaperdoll_Beards[2 * player->uCurrentFace - 23],
                     pIcons_LOD->GetTexture(papredoll_dbrds[player->uCurrentFace]));
      }
    //--------------------------------------------(Helm/Шлем)------------------------------------------------------------
    if ( player->GetItem(&PlayerEquipment::uHelm) )
    {
      item = player->GetHelmItem();
      switch ( item->uItemID )
      {
        case ITEM_RELIC_TALEDONS_HELM:
          index = 11;
          break;
        case ITEM_RELIC_SCHOLARS_CAP:
          index = 12;
          break;
        case ITEM_RELIC_PHYNAXIAN_CROWN:
          index = 13;
          break;
        case ITEM_ARTIFACT_MINDS_EYE:
          index = 14;
          break;
        case ITEM_RARE_SHADOWS_MASK:
          index = 15;
          break;
        default:
          index = item->uItemID - 89;
      }
      if ( index >= 0 && index < 16 )
      {
        item_X = pPaperdoll_BodyX + paperdoll_Helm[pBodyComplection][index][0];
        item_Y = pPaperdoll_BodyY + paperdoll_Helm[pBodyComplection][index][1];
        if ( IsDwarf != 1 || item->uItemID != 92 )
          v127 = paperdoll_helm_texture[player->GetSexByVoice()][index];
        else
          v127 = papredoll_dbrds[11];
        if ( item->uAttributes & 0xF0 )
        {
          if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_RED )
            container = "sptext01";
          if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_BLUE )
            container = "sp28a";
          if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_GREEN )
            container = "sp30a";
          if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_PURPLE )
            container = "sp91a";
          _50C9A8_item_enchantment_timer -= pEventTimer->uTimeElapsed;
          if ( _50C9A8_item_enchantment_timer <= 0 )
          {
            _50C9A8_item_enchantment_timer = 0;
            item->uAttributes &= 0xFFFFFF0Fu;
            ptr_50C9A4_ItemToEnchant = 0;
          }
          pRenderer->DrawAura(item_X, item_Y, pIcons_LOD->GetTexture(v127), pIcons_LOD->LoadTexturePtr(container, TEXTURE_16BIT_PALETTE),
                        GetTickCount() * 0.1, 0, 255);
        }
        else
        {
          if ( item->uAttributes & 2 )
            pRenderer->DrawTransparentRedShade(item_X, item_Y, pIcons_LOD->GetTexture(v127));
          else
          {
            if ( item->uAttributes & 1 )
              pRenderer->DrawTextureTransparent(item_X, item_Y, pIcons_LOD->GetTexture(v127));
            else
              pRenderer->DrawTransparentGreenShade(item_X, item_Y, pIcons_LOD->GetTexture(v127));
          }
        }
        if ( !bRingsShownInCharScreen )
          pRenderer->DrawMaskToZBuffer(item_X, item_Y, pIcons_LOD->GetTexture(v127), player->pEquipment.uHelm);
      }
    }
    //------------------------------------------------(Hand3/Рука3)-------------------------------------------
    if ( player->GetItem(&PlayerEquipment::uMainHand) )
    {
      item = player->GetMainHandItem();
      item_X = pPaperdoll_BodyX + paperdoll_Weapon[pBodyComplection][1][0] - pItemsTable->pItems[item->uItemID].uEquipX;
      item_Y = pPaperdoll_BodyY + paperdoll_Weapon[pBodyComplection][1][1] - pItemsTable->pItems[item->uItemID].uEquipY;
      if ( item->uItemID == 64 )
        v181 = "item64v1";
      else
        v181 = item->GetIconName();
      if ( !(item->uAttributes & 0xF0) )
      {
        if ( item->uAttributes & 2 )
          pRenderer->DrawTransparentRedShade(item_X, item_Y, pIcons_LOD->GetTexture(pIcons_LOD->LoadTexture(v181, TEXTURE_16BIT_PALETTE)));
        else
        {
          if ( item->uAttributes & 1 )
            pRenderer->DrawTextureTransparent(item_X, item_Y, pIcons_LOD->GetTexture(pIcons_LOD->LoadTexture(v181, TEXTURE_16BIT_PALETTE)));
          else
            pRenderer->DrawTransparentGreenShade(item_X, item_Y, pIcons_LOD->GetTexture(pIcons_LOD->LoadTexture(v181, TEXTURE_16BIT_PALETTE)));
        }
      }
      else
      {
        if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_RED )
          container = "sptext01";
        if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_BLUE )
          container = "sp28a";
        if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_GREEN )
           container = "sp30a";
        if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_PURPLE )
          container = "sp91a";
        _50C9A8_item_enchantment_timer -= pEventTimer->uTimeElapsed;
        if ( _50C9A8_item_enchantment_timer <= 0 )
        {
          _50C9A8_item_enchantment_timer = 0;
          item->uAttributes &= 0xFFFFFF0Fu;
          ptr_50C9A4_ItemToEnchant = 0;
        }
        pRenderer->DrawAura(item_X, item_Y, pIcons_LOD->GetTexture(pIcons_LOD->LoadTexture(v181, TEXTURE_16BIT_PALETTE)),
                   pIcons_LOD->LoadTexturePtr(container, TEXTURE_16BIT_PALETTE), GetTickCount() * 0.1, 0, 255);
      }
      if ( !bRingsShownInCharScreen )
        pRenderer->DrawMaskToZBuffer(item_X, item_Y, pIcons_LOD->GetTexture(pIcons_LOD->LoadTexture(v181, TEXTURE_16BIT_PALETTE)),
                    player->pEquipment.uMainHand);
    }
    //--------------------------------------------------(Shield/Щит)---------------------------------------------
    if ( player->GetItem(&PlayerEquipment::uShield) )
    {
      item = player->GetOffHandItem();
      if ( item->GetPlayerSkillType() == PLAYER_SKILL_DAGGER
        || item->GetPlayerSkillType() == PLAYER_SKILL_SWORD )
      {
        //v151 = item->uItemID - 400;
        item_X = 596;
        v245 = 1;
        switch ( item->uItemID )
        {
          case 400:
            item_Y = 86;
            break;
          case 403:
            item_Y = 28;
            break;
          case 415:
            item_X = 595;
            item_Y = 33;
            break;
          default:
            item_X = pPaperdoll_BodyX + paperdoll_Weapon[pBodyComplection][0][0] - pItemsTable->pItems[item->uItemID].uEquipX;
            item_Y = pPaperdoll_BodyY + paperdoll_Weapon[pBodyComplection][0][1] - pItemsTable->pItems[item->uItemID].uEquipY;
            break;
        }
      }
      else
      {
        v245 = 0;
        item_X = pPaperdoll_BodyX + paperdoll_Weapon[pBodyComplection][0][0] - pItemsTable->pItems[item->uItemID].uEquipX;
        item_Y = pPaperdoll_BodyY + paperdoll_Weapon[pBodyComplection][0][1] - pItemsTable->pItems[item->uItemID].uEquipY;
      }
      v153 = pIcons_LOD->LoadTexture(item->GetIconName(), TEXTURE_16BIT_PALETTE);
      if ( !(item->uAttributes & 0xF0) )
      {
        if ( item->uAttributes & 2 )
          pRenderer->DrawTransparentRedShade(item_X, item_Y, pIcons_LOD->GetTexture(v153));
        else
        {
          //v160 = (char *)pIcons_LOD->GetTexture(v153);
          if ( !(item->uAttributes & 1) )
          pRenderer->DrawTransparentGreenShade(item_X, item_Y, pIcons_LOD->GetTexture(v153));
          else
            pRenderer->DrawTextureTransparent(item_X, item_Y, pIcons_LOD->GetTexture(v153));
        }
      }
      else
      {
        if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_RED )
          container = "sptext01";
        if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_BLUE )
          container = "sp28a";
        if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_GREEN )
          container = "sp30a";
        if ( (item->uAttributes & 0xF0) == ITEM_AURA_EFFECT_PURPLE )
          container = "sp91a";
        _50C9A8_item_enchantment_timer -= pEventTimer->uTimeElapsed;
        if ( _50C9A8_item_enchantment_timer <= 0 )
        {
          _50C9A8_item_enchantment_timer = 0;
          item->uAttributes &= 0xFFFFFF0Fu;
          ptr_50C9A4_ItemToEnchant = 0;
        }
        pRenderer->DrawAura(item_X, item_Y, pIcons_LOD->GetTexture(v153), pIcons_LOD->LoadTexturePtr(container, TEXTURE_16BIT_PALETTE),
                     GetTickCount() * 0.1, 0, 255);
        if ( v245 )
          pRenderer->DrawTextureTransparent(pPaperdoll_BodyX + pPaperdollLeftEmptyHand[pBodyComplection][0],
                 pPaperdoll_BodyY + pPaperdollLeftEmptyHand[pBodyComplection][1], pIcons_LOD->GetTexture(papredoll_dlhs[uPlayerID - 1]));
      }
      if ( !bRingsShownInCharScreen )
        pRenderer->DrawMaskToZBuffer(item_X, item_Y, pIcons_LOD->GetTexture(v153), player->pEquipment.uShield);
    }
  }
  //--------------------------------------------------------(RightHand/Правая рука)--------------------------------------------------
  pRenderer->DrawTextureTransparent(pPaperdoll_BodyX + pPaperdoll_RightHand[pBodyComplection][0], pPaperdoll_BodyY + pPaperdoll_RightHand[pBodyComplection][1], pIcons_LOD->GetTexture(papredoll_drhs[uPlayerID - 1]));
  //--------------------------------------------------------(LeftHand/Левая рука)----------------------------------------------------
  if ( player->GetItem(&PlayerEquipment::uMainHand))
  {
    item = player->GetMainHandItem();
    if ( item->GetItemEquipType() == EQUIP_MAIN_HAND
        || item->GetPlayerSkillType() == PLAYER_SKILL_SPEAR
        && !player->GetItem(&PlayerEquipment::uShield) )
      pRenderer->DrawTextureTransparent(pPaperdoll_BodyX + pPaperdoll_SecondLeftHand[pBodyComplection][0], 
                                        pPaperdoll_BodyY + pPaperdoll_SecondLeftHand[pBodyComplection][1], 
                                        pIcons_LOD->GetTexture(papredoll_dlhus[uPlayerID - 1]));
  }
  if ( !bRingsShownInCharScreen )//рисование лупы
    pRenderer->DrawTextureTransparent(603, 299, pIcons_LOD->GetTexture(uTextureID_MAGNIF_B));
  pRenderer->DrawTextureTransparent(468, 0, pIcons_LOD->GetTexture(uTextureID_right_panel_loop));//обрамление
}

//----- (0041A2D1) --------------------------------------------------------
void CharacterUI_InventoryTab_Draw(Player *player, bool a2)
{
  Texture *pTexture; // esi@6
  int v13; // eax@13
  int v15; // eax@13
  unsigned int v17; // edi@15
  unsigned int uCellX; // [sp+30h] [bp-8h]@5
  unsigned int uCellY; // [sp+34h] [bp-4h]@5

  pRenderer->DrawTextureIndexed(8, 8, pIcons_LOD->GetTexture(uTextureID_CharacterUI_InventoryBackground));
  if (a2)
    pRenderer->DrawTextureIndexed(8, 305, pIcons_LOD->LoadTexturePtr("fr_strip", TEXTURE_16BIT_PALETTE));
  for (uint i = 0; i < 126; ++i)
  {
    if ( player->pInventoryMatrix[i] <= 0 )
      continue;
    if ( !player->pInventoryItemList[player->pInventoryMatrix[i] - 1].uItemID )
      continue;
    uCellY = 32 * (i / 14) + 17;
    uCellX = 32 * (i % 14) + 14;
    uint item_texture_id = pIcons_LOD->LoadTexture(player->pInventoryItemList[player->pInventoryMatrix[i] - 1].GetIconName(), TEXTURE_16BIT_PALETTE);
    pTexture = pIcons_LOD->GetTexture(item_texture_id);
    if (pTexture->uTextureWidth < 14)
      pTexture->uTextureWidth = 14;
    if ( (pTexture->uTextureWidth - 14) / 32 == 0 && pTexture->uTextureWidth < 32)
      uCellX += (32 - pTexture->uTextureWidth) / 2;
    v13 = pTexture->uTextureWidth - 14;
    LOBYTE(v13) = v13 & 0xE0;
    v15 = v13 + 32;
    if (pTexture->uTextureHeight < 14 )
      pTexture->uTextureHeight = 14;
    v17 = uCellX + ((v15 - pTexture->uTextureWidth) >> 1) + pSRZBufferLineOffsets[uCellY + (( (int)((pTexture->uTextureHeight - 14) & 0xFFFFFFE0) - pTexture->uTextureHeight + 32) >> 1)];   //added typecast. without it the value in the brackets got cat to unsigned which messed stuff up
    if (player->pInventoryItemList[player->pInventoryMatrix[i] - 1].uAttributes & 0xF0)
    {
      switch (player->pInventoryItemList[player->pInventoryMatrix[i] - 1].uAttributes & 0xF0)
      {
        case ITEM_AURA_EFFECT_RED:    pTexture = pIcons_LOD->LoadTexturePtr("sptext01", TEXTURE_16BIT_PALETTE); break;
        case ITEM_AURA_EFFECT_BLUE:   pTexture = pIcons_LOD->LoadTexturePtr("sp28a", TEXTURE_16BIT_PALETTE);    break;
        case ITEM_AURA_EFFECT_GREEN:  pTexture = pIcons_LOD->LoadTexturePtr("sp30a", TEXTURE_16BIT_PALETTE);    break;
        case ITEM_AURA_EFFECT_PURPLE: pTexture = pIcons_LOD->LoadTexturePtr("sp91a", TEXTURE_16BIT_PALETTE);    break;
      }
      _50C9A8_item_enchantment_timer -= pEventTimer->uTimeElapsed;
      if (_50C9A8_item_enchantment_timer <= 0)
      {
        _50C9A8_item_enchantment_timer = 0;
        LOBYTE(player->pInventoryItemList[player->pInventoryMatrix[i] - 1].uAttributes) &= 0xF;
        ptr_50C9A4_ItemToEnchant = 0;
      }
      pRenderer->DrawAura(uCellX, uCellY, pTexture, pTexture, GetTickCount() * 0.1, 0, 255);
      ZBuffer_Fill(&pRenderer->pActiveZBuffer[v17], item_texture_id, player->pInventoryMatrix[i]);
    }
    else
    {
      if (player->pInventoryItemList[player->pInventoryMatrix[i] - 1].IsIdentified() || pCurrentScreen != SCREEN_HOUSE)
      {
        if (player->pInventoryItemList[player->pInventoryMatrix[i] - 1].IsBroken())
          pRenderer->DrawTransparentRedShade(uCellX, uCellY, pTexture);
        else
          pRenderer->DrawTextureTransparent(uCellX, uCellY, pTexture);
      }
      else
        pRenderer->DrawTransparentGreenShade(uCellX, uCellY, pTexture);
      ZBuffer_Fill(&pRenderer->pActiveZBuffer[v17], item_texture_id, player->pInventoryMatrix[i]);
      continue;
    }
  }
}

static void CharacterUI_DrawItem(int x, int y, ItemGen *item, int id)
{
  auto item_texture = pIcons_LOD->LoadTexturePtr(item->GetIconName(), TEXTURE_16BIT_PALETTE);

  if (item->uAttributes & 0xF0) // enchant animation
  {
    Texture *enchantment_texture = nullptr;
    switch (item->uAttributes & 0xF0)
    {
      case ITEM_AURA_EFFECT_RED:    enchantment_texture = pIcons_LOD->LoadTexturePtr("sptext01", TEXTURE_16BIT_PALETTE); break;
      case ITEM_AURA_EFFECT_BLUE:   enchantment_texture = pIcons_LOD->LoadTexturePtr("sp28a", TEXTURE_16BIT_PALETTE);    break;
      case ITEM_AURA_EFFECT_GREEN:  enchantment_texture = pIcons_LOD->LoadTexturePtr("sp30a", TEXTURE_16BIT_PALETTE);    break;
      case ITEM_AURA_EFFECT_PURPLE: enchantment_texture = pIcons_LOD->LoadTexturePtr("sp91a", TEXTURE_16BIT_PALETTE);    break;
    }

    _50C9A8_item_enchantment_timer -= pEventTimer->uTimeElapsed;
    if (_50C9A8_item_enchantment_timer <= 0)
    {
      _50C9A8_item_enchantment_timer = 0;
      item->uAttributes &= 0xFFFFFF0F;
      ptr_50C9A4_ItemToEnchant = 0;
    }
    pRenderer->DrawAura(x, y, item_texture, enchantment_texture, GetTickCount() * 0.1, 0, 255);
  }
  else
  {
    if (item->IsBroken())
      pRenderer->DrawTransparentRedShade(x, y, item_texture);
    else if (!item->IsIdentified())
      pRenderer->DrawTransparentGreenShade(x, y, item_texture);
    else
      pRenderer->DrawTextureTransparent(x, y, item_texture);

    pRenderer->ZBuffer_Fill_2(x, y, item_texture, id);
  }
}

//----- (0043E825) --------------------------------------------------------
void CharacterUI_DrawPaperdollWithRingOverlay(Player *player)
{
  CharacterUI_DrawPaperdoll(player);
  pRenderer->DrawTextureTransparent(0x1D9u, 0, pIcons_LOD->GetTexture(uTextureID_BACKHAND));
  pRenderer->DrawTextureTransparent(0x1D4u, 0, pIcons_LOD->GetTexture(uTextureID_right_panel_loop));
  pRenderer->DrawTextureIndexed(pCharacterScreen_DetalizBtn->uX, pCharacterScreen_DetalizBtn->uY,
                                pIcons_LOD->GetTexture(uTextureID_detaliz_close_button));
  for (uint i = 0; i < 6; ++i)
  {
    if (!player->pEquipment.uRings[i])
      continue;
    static int pPaperdollRingsX[6] = {0x1EA, 0x21A, 0x248, 0x1EA, 0x21A, 0x248};
    static int pPaperdollRingsY[6] = {0x0CA, 0x0CA, 0x0CA, 0x0FA, 0x0FA, 0x0FA};
    CharacterUI_DrawItem(pPaperdollRingsX[i], pPaperdollRingsY[i], &player->pInventoryItemList[player->pEquipment.uRings[i] - 1],
                         player->pEquipment.uRings[i]);
  }
  if (player->pEquipment.uAmulet)
    CharacterUI_DrawItem(493, 91, player->GetAmuletItem(), player->pEquipment.uAmulet);
  if (player->pEquipment.uGlove)
    CharacterUI_DrawItem(586, 88, player->GetGloveItem(), player->pEquipment.uGlove);
}

//----- (0043BCA7) --------------------------------------------------------
void CharacterUI_LoadPaperdollTextures()
{
  int v3; // ebx@10
  Player *pPlayer; // edi@12
  int v6; // edi@16
  unsigned int v7; // eax@16
  Player *pPlayer2; // ebx@16
  char *v9; // ebx@16
  unsigned int v22; // eax@76
  int v23; // ecx@76
  unsigned int v24; // eax@78
  int v25; // ecx@78
  unsigned int v27; // eax@80
  int v28; // ecx@80
  signed int v32; // [sp+10h] [bp-28h]@75
  signed int v33; // [sp+10h] [bp-28h]@77
  int v34; // [sp+10h] [bp-28h]@79
  int pItemTXTNum; // [sp+14h] [bp-24h]@75
  int v37; // [sp+14h] [bp-24h]@77
  signed int v38; // [sp+14h] [bp-24h]@79
  char pContainer[128]; // [sp+24h] [bp-14h]@12

  uTextureID_MAGNIF_B = pIcons_LOD->LoadTexture("MAGNIF-B", TEXTURE_16BIT_PALETTE);
  //if ( !pParty->uAlignment || pParty->uAlignment == 1 || pParty->uAlignment == 2 )
  uTextureID_BACKDOLL = pIcons_LOD->LoadTexture("BACKDOLL", TEXTURE_16BIT_PALETTE);
  uTextureID_right_panel_loop = uTextureID_right_panel;
  uTextureID_BACKHAND = pIcons_LOD->LoadTexture("BACKHAND", TEXTURE_16BIT_PALETTE);
  uTextureID_detaliz_close_button = uExitCancelTextureId;
  for ( uint i = 0; i < 4; ++i )
  {
    if ( pPlayers[i + 1]->HasUnderwaterSuitEquipped() )
    {
      if ( pPlayers[i + 1]->GetRace() == CHARACTER_RACE_DWARF )
        v3 = (pPlayers[i + 1]->GetSexByVoice() != 0) + 3;
      else
        v3 = (pPlayers[i + 1]->GetSexByVoice() != 0) + 1;
      wsprintfA(pContainer, "pc23v%dBod", v3);
      papredoll_dbods[i] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);//Body texture
      wsprintfA(pContainer, "pc23v%dlad", v3);
      papredoll_dlads[i] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);// Left Hand
      wsprintfA(pContainer, "pc23v%dlau", v3);
      papredoll_dlaus[i] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);// Left Hand2
      wsprintfA(pContainer, "pc23v%drh", v3);
      papredoll_drhs[i] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);// Right Hand
      wsprintfA(pContainer, "pc23v%dlh", v3);
      papredoll_dlhs[i] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);// Left Fist
      wsprintfA(pContainer, "pc23v%dlhu", v3);
      papredoll_dlhus[i] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE); // Left Fist 2
      pPlayer = pPlayers[i + 1];
      if ( pPlayer->uCurrentFace == 12 || pPlayer->uCurrentFace == 13 )
        papredoll_dbrds[(char)pPlayer->uCurrentFace] = 0;
      papredoll_flying_feet[pPlayer->uCurrentFace] = 0;
      IsPlayerWearingWatersuit[i + 1] = 1;
    }
    else
    {
      papredoll_dbods[i] = pIcons_LOD->LoadTexture(dbod_texnames_by_face[pPlayers[i + 1]->uCurrentFace], TEXTURE_16BIT_PALETTE);
      papredoll_dlads[i] = pIcons_LOD->LoadTexture(dlad_texnames_by_face[pPlayers[i + 1]->uCurrentFace], TEXTURE_16BIT_PALETTE);
      papredoll_dlaus[i] = pIcons_LOD->LoadTexture(dlau_texnames_by_face[pPlayers[i + 1]->uCurrentFace], TEXTURE_16BIT_PALETTE);
      papredoll_drhs[i] = pIcons_LOD->LoadTexture(drh_texnames_by_face[pPlayers[i + 1]->uCurrentFace], TEXTURE_16BIT_PALETTE);
      papredoll_dlhs[i] = pIcons_LOD->LoadTexture(dlh_texnames_by_face[pPlayers[i + 1]->uCurrentFace], TEXTURE_16BIT_PALETTE);
      papredoll_dlhus[i] = pIcons_LOD->LoadTexture(dlhu_texnames_by_face[pPlayers[i + 1]->uCurrentFace], TEXTURE_16BIT_PALETTE);
      if ( pPlayers[i + 1]->uCurrentFace == 12 || pPlayers[i + 1]->uCurrentFace == 13 )
      {
        wsprintfA(pContainer, "pc%02dbrd", pPlayers[i + 1]->uCurrentFace + 1);
        papredoll_dbrds[pPlayers[i + 1]->uCurrentFace] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
      }
      wsprintfA(pContainer, "item281pc%02d", pPlayers[i + 1]->uCurrentFace + 1);
      papredoll_flying_feet[pPlayers[i + 1]->uCurrentFace] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
      IsPlayerWearingWatersuit[i + 1] = 0;
    }
  }

  uTextureID_ar_up_up = pIcons_LOD->LoadTexture("ar_up_up", TEXTURE_16BIT_PALETTE);
  uTextureID_ar_up_dn = pIcons_LOD->LoadTexture("ar_up_dn", TEXTURE_16BIT_PALETTE);
  uTextureID_ar_dn_up = pIcons_LOD->LoadTexture("ar_dn_up", TEXTURE_16BIT_PALETTE);
  uTextureID_ar_dn_dn = pIcons_LOD->LoadTexture("ar_dn_dn", TEXTURE_16BIT_PALETTE);
  papredoll_dbrds[9] = pIcons_LOD->LoadTexture("ib-cd1-d", TEXTURE_16BIT_PALETTE);
  papredoll_dbrds[7] = pIcons_LOD->LoadTexture("ib-cd2-d", TEXTURE_16BIT_PALETTE);
  papredoll_dbrds[5] = pIcons_LOD->LoadTexture("ib-cd3-d", TEXTURE_16BIT_PALETTE);
  papredoll_dbrds[3] = pIcons_LOD->LoadTexture("ib-cd4-d", TEXTURE_16BIT_PALETTE);
  papredoll_dbrds[1] = pIcons_LOD->LoadTexture("ib-cd5-d", TEXTURE_16BIT_PALETTE);
  for ( uint i = 0; i < 54; ++i )// test equipment
  {
    party_has_equipment[i] = 0;
    if ( pParty->pPickedItem.uItemID != i + 66 )
    {
      for ( uint j = 0; j < 4; ++j)
      {
        if ( pParty->pPlayers[j].HasItem(i + 66, 0) )
          party_has_equipment[i] = 1;
      }
    }
  }
  memset(byte_5111F6.data(), 0, sizeof(byte_5111F6));
  for (uint i = 0; i < 4; ++i)
  {
    auto player = &pParty->pPlayers[i];

    if (player->HasItem(ITEM_ARTIFACT_GOVERNORS_ARMOR, 1))    byte_5111F6[0] = 1;
    if (player->HasItem(ITEM_ARTIFACT_YORUBA, 1))             byte_5111F6[1] = 1;
    if (player->HasItem(ITEM_RELIC_HARECS_LEATHER, 1))        byte_5111F6[2] = 1;
    if (player->HasItem(ITEM_ARTIFACT_LEAGUE_BOOTS, 1))       byte_5111F6[3] = 1;
    if (player->HasItem(ITEM_RELIC_TALEDONS_HELM, 1))         byte_5111F6[4] = 1;
    if (player->HasItem(ITEM_RELIC_SCHOLARS_CAP, 1))          byte_5111F6[5] = 1;
    if (player->HasItem(ITEM_RELIC_PHYNAXIAN_CROWN, 1))       byte_5111F6[6] = 1;
    if (player->HasItem(ITEM_ARTIFACT_MINDS_EYE, 1))          byte_5111F6[7] = 1;
    if (player->HasItem(ITEM_RARE_SHADOWS_MASK, 1))           byte_5111F6[8] = 1;
    if (player->HasItem(ITEM_RILIC_TITANS_BELT, 1))           byte_5111F6[9] = 1;
    if (player->HasItem(ITEM_ARTIFACT_HEROS_BELT, 1))         byte_5111F6[10] = 1;
    if (player->HasItem(ITEM_RELIC_TWILIGHT, 1))              byte_5111F6[11] = 1;
    if (player->HasItem(ITEM_ARTIFACT_CLOAK_OF_THE_SHEEP, 1)) byte_5111F6[12] = 1;
    if (player->HasItem(ITEM_RARE_SUN_CLOAK, 1))              byte_5111F6[13] = 1;
    if (player->HasItem(ITEM_RARE_MOON_CLOAK, 1))             byte_5111F6[14] = 1;
    if (player->HasItem(ITEM_RARE_VAMPIRES_CAPE, 1))          byte_5111F6[15] = 1;
    if (player->HasItem(ITEM_ELVEN_CHAINMAIL, 1))             byte_5111F6[16] = 1;
  }

  for (uint i = 0; i < 2; ++i)
  {
    for ( uint j = 0; j < 5; ++j )//Belt
    {
      GetItemTextureFilename(pContainer, j + 100, i + 1, 0);
      paperdoll_belt_texture[i][j] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
    }
    GetItemTextureFilename(pContainer, 535, i + 1, 0);
    paperdoll_belt_texture[i][6] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
    for ( uint j = 0; j < 11; ++j )//Helm
    {
      GetItemTextureFilename(pContainer, j + 89, i + 1, 0);
      paperdoll_helm_texture[i][j] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
    }
    GetItemTextureFilename(pContainer, 521, i + 1, 0);
    paperdoll_helm_texture[i][11] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
    GetItemTextureFilename(pContainer, 522, i + 1, 0);
    paperdoll_helm_texture[i][12] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
    GetItemTextureFilename(pContainer, 523, i + 1, 0);
    paperdoll_helm_texture[i][13] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
    GetItemTextureFilename(pContainer, 532, i + 1, 0);
    paperdoll_helm_texture[i][14] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
    GetItemTextureFilename(pContainer, 544, i + 1, 0);
    paperdoll_helm_texture[i][15] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
    if ( IsDwarfPresentInParty(true) )          //the phynaxian helm uses a slightly different graphic for dwarves
      papredoll_dbrds[11] = pIcons_LOD->LoadTexture("item092v3", TEXTURE_16BIT_PALETTE);
  }
  //v43 = 0;
  for (uint i = 0; i < 4; ++i)
  {
    if ( ShouldLoadTexturesForRaceAndGender(i) )
    {
      GetItemTextureFilename(pContainer, 524, i + 1, 0);
      paperdoll_belt_texture[i][5] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);//Titans belt
      pItemTXTNum = 66;
      for ( v32 = 0; v32 < 13; ++v32 )//simple armor
      {
        GetItemTextureFilename(pContainer, pItemTXTNum, i + 1, 0);
        paperdoll_armor_texture[i][v32][0] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);// armor
        GetItemTextureFilename(pContainer, pItemTXTNum, i + 1, 1);
        paperdoll_armor_texture[i][v32][1] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);// shoulder 1
        GetItemTextureFilename(pContainer, pItemTXTNum, i + 1, 2);
        paperdoll_armor_texture[i][v32][2] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);// shoulder 2
        pItemTXTNum++;
      }
      GetItemTextureFilename(pContainer, 516, i + 1, 0);//artefacts
      paperdoll_armor_texture[i][v32][0] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
      GetItemTextureFilename(pContainer, 516, i + 1, 1);
      paperdoll_armor_texture[i][v32][1] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
      GetItemTextureFilename(pContainer, 516, i + 1, 2);
      paperdoll_armor_texture[i][v32][2] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
      GetItemTextureFilename(pContainer, 505, i + 1, 0);
      paperdoll_armor_texture[i][v32 + 1][0] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
      GetItemTextureFilename(pContainer, 505, i + 1, 1);
      paperdoll_armor_texture[i][v32 + 1][1] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
      GetItemTextureFilename(pContainer, 505, i + 1, 2);
      paperdoll_armor_texture[i][v32 + 1][2] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
      GetItemTextureFilename(pContainer, 504, i + 1, 0);
      paperdoll_armor_texture[i][v32 + 2][0] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
      GetItemTextureFilename(pContainer, 504, i + 1, 1);
      paperdoll_armor_texture[i][v32 + 2][1] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
      GetItemTextureFilename(pContainer, 504, i + 1, 2);
      paperdoll_armor_texture[i][v32 + 2][2] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
      GetItemTextureFilename(pContainer, 533, i + 1, 0);
      paperdoll_armor_texture[i][v32 + 3][0] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
      GetItemTextureFilename(pContainer, 533, i + 1, 1);
      paperdoll_armor_texture[i][v32 + 3][1] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
      GetItemTextureFilename(pContainer, 533, i + 1, 2);
      paperdoll_armor_texture[i][v32 + 3][2] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
      for ( v33 = 0; v33 < 5; ++v33 )//boots
      {
        GetItemTextureFilename(pContainer, v33 + 115, i + 1, 0);
        paperdoll_boots_texture[i][v33] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
      }
      GetItemTextureFilename(pContainer, 512, i + 1, 0);
      paperdoll_boots_texture[i][v33] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
      for ( v38 = 0; v38 < 5; ++v38 )//Cloak
      {
        GetItemTextureFilename(pContainer, v38 + 105, i + 1, 0);
        paperdoll_cloak_texture[i][v38] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
        GetItemTextureFilename(pContainer, v38 + 105, i + 1, 1);
        paperdoll_cloak_collar_texture[i][v38] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
      }
      GetItemTextureFilename(pContainer, 525, i + 1, 0);
      paperdoll_cloak_texture[i][5] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
      GetItemTextureFilename(pContainer, 530, i + 1, 0);
      paperdoll_cloak_texture[i][6] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
      GetItemTextureFilename(pContainer, 547, i + 1, 0);
      paperdoll_cloak_texture[i][7] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
      GetItemTextureFilename(pContainer, 548, i + 1, 0);
      paperdoll_cloak_texture[i][8] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
      GetItemTextureFilename(pContainer, 550, i + 1, 0);
      paperdoll_cloak_texture[i][9] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
      GetItemTextureFilename(pContainer, 525, i + 1, 1);
      paperdoll_cloak_collar_texture[i][5] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
      GetItemTextureFilename(pContainer, 530, i + 1, 1);
      paperdoll_cloak_collar_texture[i][6] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
      GetItemTextureFilename(pContainer, 547, i + 1, 1);
      paperdoll_cloak_collar_texture[i][7] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
      GetItemTextureFilename(pContainer, 548, i + 1, 1);
      paperdoll_cloak_collar_texture[i][8] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
      GetItemTextureFilename(pContainer, 550, i + 1, 1);
      paperdoll_cloak_collar_texture[i][9] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
    }
    //else
    //{
      //v26 = v43;
    //}
    //v43 = v26 + 40;
  }
}

//----- (00419401) --------------------------------------------------------
void CharacterUI_SkillsTab_CreateButtons()
{
  GUIButton *pButton; // eax@3
  unsigned int current_Y; // esi@8
  int buttons_count; // [sp+10h] [bp-14h]@1
  int first_rows; // [sp+14h] [bp-10h]@19
  int skill_id; // [sp+18h] [bp-Ch]@8
  Player *curr_player; // [sp+1Ch] [bp-8h]@8
  int i;
  int uCurrFontHeght;

  buttons_count = 0;
  if ( dword_507CC0_activ_ch )
    CharacterUI_ReleaseButtons();
  dword_507CC0_activ_ch = uActiveCharacter;
  for ( pButton = pGUIWindow_CurrentMenu->pControlsHead; pButton; pButton = pButton->pNext )
  {
    if ( pButton->msg == UIMSG_InventoryLeftClick )
    {
      dword_50698C_uX = pButton->uX;
      dword_506988_uY = pButton->uY;
      dword_506984_uZ = pButton->uZ;
      dword_506980_uW = pButton->uW;
      pButton->uW = 0;
      pButton->uZ = 0;
      pButton->uY = 0;
      pButton->uX = 0;
    }
  }
  first_rows = 0;
  //  a5 = pGUIWindow_CurrentMenu->uNumControls;
  curr_player = &pParty->pPlayers[uActiveCharacter-1];

  uCurrFontHeght=pFontLucida->uFontHeight;
  current_Y = 2 *uCurrFontHeght  + 13;
  for( i = 0; i < 9; ++i )
  {
    skill_id = pWeaponSkills[i];
    if ( curr_player->pActiveSkills[skill_id] & 0x3F )
    {
      current_Y += uCurrFontHeght - 3;
      ++buttons_count;
      ++first_rows;
      pGUIWindow_CurrentMenu->CreateButton(24, current_Y, 204, uCurrFontHeght - 3, 3, skill_id | 0x8000, UIMSG_SkillUp, skill_id, 0, "", 0, 0);
    }
  }
  if ( !first_rows )
    current_Y += uCurrFontHeght - 3;
  current_Y += 2 * uCurrFontHeght - 6;
  for ( i = 0; i < 9; ++i )
  {
    skill_id = pMagicSkills[i];
    if ( curr_player->pActiveSkills[skill_id] & 0x3F && buttons_count < 15 )
    {
      current_Y += uCurrFontHeght - 3;
      ++buttons_count;
      pGUIWindow_CurrentMenu->CreateButton(24, current_Y, 204, uCurrFontHeght - 3, 3, skill_id | 0x8000, UIMSG_SkillUp, skill_id, 0, "", 0, 0);
    }
  }
  first_rows = 0;
  current_Y = 2 * uCurrFontHeght + 13;
  for ( i = 0; i < 5; ++i )
  {
    skill_id = pArmorSkills[i];
    if ( curr_player->pActiveSkills[skill_id] & 0x3F )
    {
      current_Y+= uCurrFontHeght - 3;
      ++buttons_count;
      ++first_rows;
      pGUIWindow_CurrentMenu->CreateButton(246, current_Y, 204, uCurrFontHeght - 3, 3, skill_id | 0x8000, UIMSG_SkillUp, skill_id, 0, "", 0, 0);
    }
  }
  if ( !first_rows )
    current_Y += uCurrFontHeght - 3;
  current_Y += 2 * uCurrFontHeght - 6;
  for ( i = 0; i < 12; ++i )
  {
    skill_id = pMiscSkills[i];
    if ( curr_player->pActiveSkills[skill_id] & 0x3F )
    {
      current_Y += uCurrFontHeght - 3;
      ++buttons_count;
      pGUIWindow_CurrentMenu->CreateButton(246, current_Y, 204, uCurrFontHeght - 3, 3, skill_id | 0x8000, UIMSG_SkillUp, skill_id, 0, "", 0, 0);
    }
  }

  if ( buttons_count )
    pGUIWindow_CurrentMenu->_41D08F_set_keyboard_control_group(buttons_count, 1, 0, pGUIWindow_CurrentMenu->uNumControls);
}

//----- (00418511) --------------------------------------------------------
void CharacterUI_StatsTab_Draw( Player *player )
{
  int pY; // ST34_4@4
  const char *pText; // eax@9
  const char *a2; // [sp+14h] [bp-Ch]@4

  pRenderer->DrawTextureIndexed(8, 8, pIcons_LOD->LoadTexturePtr("fr_stats", TEXTURE_16BIT_PALETTE));
  sprintf(pTmpBuf.data(), "\f%05d", ui_character_header_text_color);
  sprintfex(pTmpBuf2.data(), pGlobalTXT_LocalizationStrings[429], player->pName, pClassNames[player->classType]);//"^Pi[%s] %s" / "%s the %s"
  strcat(pTmpBuf.data(), pTmpBuf2.data());
  sprintfex(pTmpBuf2.data(),  "\f00000\r180%s: \f%05d%d\f00000\n\n\n",
            pGlobalTXT_LocalizationStrings[207], // "Skill points"
            player->uSkillPoints ? ui_character_bonus_text_color : ui_character_default_text_color,
            player->uSkillPoints);
  strcat(pTmpBuf.data(), pTmpBuf2.data());
  pGUIWindow_CurrentMenu->DrawText(pFontArrus, 26, 18, 0, pTmpBuf.data(), 0, 0, 0);

  //First column(Первая колонка)
  pY = 53;
  sprintf(pTmpBuf.data(), "%s\f%05u\r424%d\f00000 /\t185%d\n", pGlobalTXT_LocalizationStrings[144],
          UI_GetHealthManaAndOtherQualitiesStringColor(player->GetActualMight(), player->GetBaseStrength()),
          player->GetActualMight(), player->GetBaseStrength());//Might
  pGUIWindow_CurrentMenu->DrawText(pFontArrus, 26, pY, 0, pTmpBuf.data(), 0, 0, 0);

  pY += LOBYTE(pFontArrus->uFontHeight) - 2;
  sprintf(pTmpBuf.data(), "%s\f%05u\r424%d\f00000 /\t185%d\n", pGlobalTXT_LocalizationStrings[116],
          UI_GetHealthManaAndOtherQualitiesStringColor(player->GetActualIntelligence(), player->GetBaseIntelligence()),
          player->GetActualIntelligence(), player->GetBaseIntelligence());//Intellect
  pGUIWindow_CurrentMenu->DrawText(pFontArrus, 26, pY, 0, pTmpBuf.data(), 0, 0, 0);

  pY += LOBYTE(pFontArrus->uFontHeight) - 2;
  sprintf(pTmpBuf.data(), "%s\f%05u\r424%d\f00000 /\t185%d\n", pGlobalTXT_LocalizationStrings[163],
          UI_GetHealthManaAndOtherQualitiesStringColor(player->GetActualWillpower(), player->GetBaseWillpower()),
          player->GetActualWillpower(), player->GetBaseWillpower());//
  pGUIWindow_CurrentMenu->DrawText(pFontArrus, 26, pY, 0, pTmpBuf.data(), 0, 0, 0);

  pY += LOBYTE(pFontArrus->uFontHeight) - 2;
  sprintf(pTmpBuf.data(), "%s\f%05u\r424%d\f00000 /\t185%d\n", pGlobalTXT_LocalizationStrings[75],
          UI_GetHealthManaAndOtherQualitiesStringColor(player->GetActualEndurance(), player->GetBaseEndurance()),
          player->GetActualEndurance(), player->GetBaseEndurance());//
  pGUIWindow_CurrentMenu->DrawText(pFontArrus, 26, pY, 0, pTmpBuf.data(), 0, 0, 0);

  pY += LOBYTE(pFontArrus->uFontHeight) - 2;
  sprintf(pTmpBuf.data(), "%s\f%05u\r424%d\f00000 /\t185%d\n", pGlobalTXT_LocalizationStrings[1],
          UI_GetHealthManaAndOtherQualitiesStringColor(player->GetActualAccuracy(), player->GetBaseAccuracy()),
          player->GetActualAccuracy(), player->GetBaseAccuracy());
  pGUIWindow_CurrentMenu->DrawText(pFontArrus, 26, pY, 0, pTmpBuf.data(), 0, 0, 0);

  pY += LOBYTE(pFontArrus->uFontHeight) - 2;
  sprintf(pTmpBuf.data(), "%s\f%05u\r424%d\f00000 /\t185%d\n", pGlobalTXT_LocalizationStrings[211],
          UI_GetHealthManaAndOtherQualitiesStringColor(player->GetActualSpeed(), player->GetBaseSpeed()),
          player->GetActualSpeed(), player->GetBaseSpeed());
  pGUIWindow_CurrentMenu->DrawText(pFontArrus, 26, pY, 0, pTmpBuf.data(), 0, 0, 0);

  pY += LOBYTE(pFontArrus->uFontHeight) - 2;
  sprintf(pTmpBuf.data(), "%s\f%05u\r424%d\f00000 /\t185%d\n\n", pGlobalTXT_LocalizationStrings[136],
          UI_GetHealthManaAndOtherQualitiesStringColor(player->GetActualLuck(), player->GetBaseLuck()),
          player->GetActualLuck(), player->GetBaseLuck());
  pGUIWindow_CurrentMenu->DrawText(pFontArrus, 26, pY, 0, pTmpBuf.data(), 0, 0, 0);

  a2 = "%s\f%05u\r424%d\f00000 /\t185%d\n";
  if ( player->GetMaxHealth() >= 1000 )
    a2 = "%s\f%05u\r388%d\f00000 / %d\n";
  pY += 2 * LOBYTE(pFontArrus->uFontHeight) + 5;
  sprintf(pTmpBuf.data(), a2, pGlobalTXT_LocalizationStrings[108],
          UI_GetHealthManaAndOtherQualitiesStringColor(player->sHealth, player->GetMaxHealth()),
          player->sHealth, player->GetMaxHealth());
  pGUIWindow_CurrentMenu->DrawText(pFontArrus, 26, pY, 0, pTmpBuf.data(), 0, 0, 0);

  a2 = "%s\f%05u\r424%d\f00000 /\t185%d\n";
  if ( player->GetMaxMana() >= 1000 )
    a2 = "%s\f%05u\r388%d\f00000 / %d\n";
  pY += LOBYTE(pFontArrus->uFontHeight) - 2;
  sprintf(pTmpBuf.data(), a2, pGlobalTXT_LocalizationStrings[212],
          UI_GetHealthManaAndOtherQualitiesStringColor(player->sMana, player->GetMaxMana()),
          player->sMana, player->GetMaxMana());
  pGUIWindow_CurrentMenu->DrawText(pFontArrus, 26, pY, 0, pTmpBuf.data(), 0, 0, 0);

  pY += LOBYTE(pFontArrus->uFontHeight) - 2;
  sprintf(pTmpBuf.data(), "%s\f%05u\r424%d\f00000 /\t185%d\n\n", pGlobalTXT_LocalizationStrings[12],
          UI_GetHealthManaAndOtherQualitiesStringColor(player->GetActualAC(), player->GetBaseAC()),
          player->GetActualAC(), player->GetBaseAC());
  pGUIWindow_CurrentMenu->DrawText(pFontArrus, 26, pY, 0, pTmpBuf.data(), 0, 0, 0);

  pY += 2 * LOBYTE(pFontArrus->uFontHeight) - 2;
  sprintf(pTmpBuf.data(), "%s: \f%05d%s\n", pGlobalTXT_LocalizationStrings[47], GetConditionDrawColor(player->GetMajorConditionIdx()),
          aCharacterConditionNames[player->GetMajorConditionIdx()]);//Состояние
  pGUIWindow_CurrentMenu->DrawTextInRect(pFontArrus, 26, pY, 0, pTmpBuf.data(), 226, 0);

  pY += LOBYTE(pFontArrus->uFontHeight) + - 1;
  pText = pGlobalTXT_LocalizationStrings[153];//Нет
  if (player->uQuickSpell)
    pText = pSpellStats->pInfos[player->uQuickSpell].pShortName;
  sprintf(pTmpBuf.data(), "%s: %s", pGlobalTXT_LocalizationStrings[172], pText);//Б. применение
  pGUIWindow_CurrentMenu->DrawTextInRect(pFontArrus, 26, pY, 0, pTmpBuf.data(), 226, 0);

  //Second column (Вторая колонка)
  pY = 50;
  sprintf(pTmpBuf.data(), "%s\f%05u\t100%d\f00000 / %d\n", pGlobalTXT_LocalizationStrings[5],
          UI_GetHealthManaAndOtherQualitiesStringColor(player->GetActualAge(), player->GetBaseAge()),
          player->GetActualAge(), player->GetBaseAge());
  pGUIWindow_CurrentMenu->DrawText(pFontArrus, 266, pY, 0, pTmpBuf.data(), 0, 0, 0);

  a2 = "%s\f%05u\t100%d\f00000 / %d\n";
  if ( player->GetBaseLevel() > 99 )
    a2 = "%s\f%05u\t180%d\f00000 / %d\n";
  pY += LOBYTE(pFontArrus->uFontHeight) - 2;
  sprintf(pTmpBuf.data(), a2, pGlobalTXT_LocalizationStrings[131],//Уров.
          UI_GetHealthManaAndOtherQualitiesStringColor(player->GetActualLevel(), player->GetBaseLevel()),
          player->GetActualLevel(), player->GetBaseLevel());
  pGUIWindow_CurrentMenu->DrawText(pFontArrus, 266, pY, 0, pTmpBuf.data(), 0, 0, 0);

  pY += LOBYTE(pFontArrus->uFontHeight) - 2;
  pText = pGlobalTXT_LocalizationStrings[17]; // "Exp."
  if (player->uExperience <= 9999999)
    pText = pGlobalTXT_LocalizationStrings[83]; // "Experience"
  sprintf(pTmpBuf.data(), "%s\r180\f%05d%lu\f00000\n\n", pText, player->GetExperienceDisplayColor(), LODWORD(player->uExperience));
  pGUIWindow_CurrentMenu->DrawText(pFontArrus, 266, pY, 0, pTmpBuf.data(), 0, 0, 0);

  pY += 2 * LOBYTE(pFontArrus->uFontHeight);
  sprintf(pTmpBuf.data(), "%s\t100%+d\n", pGlobalTXT_LocalizationStrings[18], player->GetActualAttack(false));
  pGUIWindow_CurrentMenu->DrawText(pFontArrus, 266, pY, 0, pTmpBuf.data(), 0, 0, 0);

  pY += LOBYTE(pFontArrus->uFontHeight) - 2;
  sprintf(pTmpBuf.data(), "%s\t100 %s\n", pGlobalTXT_LocalizationStrings[53], player->GetMeleeDamageString());
  pGUIWindow_CurrentMenu->DrawText(pFontArrus, 266, pY, 0, pTmpBuf.data(), 0, 0, 0);

  pY += LOBYTE(pFontArrus->uFontHeight) - 2;
  sprintf(pTmpBuf.data(), "%s\t100%+d\n", pGlobalTXT_LocalizationStrings[203], player->GetRangedAttack());
  pGUIWindow_CurrentMenu->DrawText(pFontArrus, 266, pY, 0, pTmpBuf.data(), 0, 0, 0);

  pY += LOBYTE(pFontArrus->uFontHeight) - 2;
  sprintf(pTmpBuf.data(), "%s\t100 %s\n\n", pGlobalTXT_LocalizationStrings[53], player->GetRangedDamageString());
  pGUIWindow_CurrentMenu->DrawText(pFontArrus, 266, pY, 0, pTmpBuf.data(), 0, 0, 0);

  a2 = format_4E2E10;
  if ( player->GetActualResistance(CHARACTER_ATTRIBUTE_RESIST_FIRE) > 99 || player->GetBaseResistance(CHARACTER_ATTRIBUTE_RESIST_FIRE) > 99 )
    a2 = "%s\f%05u\t180%d\f00000 / %d\n";
  pY += 2 * LOBYTE(pFontArrus->uFontHeight) - 4;
  sprintf(pTmpBuf.data(), a2, pGlobalTXT_LocalizationStrings[87],
          UI_GetHealthManaAndOtherQualitiesStringColor(player->GetActualResistance(CHARACTER_ATTRIBUTE_RESIST_FIRE), player->GetBaseResistance(CHARACTER_ATTRIBUTE_RESIST_FIRE)),
          player->GetActualResistance(CHARACTER_ATTRIBUTE_RESIST_FIRE), player->GetBaseResistance(CHARACTER_ATTRIBUTE_RESIST_FIRE));
  pGUIWindow_CurrentMenu->DrawText(pFontArrus, 266, pY, 0, pTmpBuf.data(), 0, 0, 0);

  a2 = format_4E2E10;
  if ( player->GetActualResistance(CHARACTER_ATTRIBUTE_RESIST_AIR) > 99 || player->GetBaseResistance(CHARACTER_ATTRIBUTE_RESIST_AIR) > 99 )
    a2 = "%s\f%05u\t180%d\f00000 / %d\n";
  pY += LOBYTE(pFontArrus->uFontHeight) - 2;
  sprintf(pTmpBuf.data(), a2, pGlobalTXT_LocalizationStrings[6],
          UI_GetHealthManaAndOtherQualitiesStringColor(player->GetActualResistance(CHARACTER_ATTRIBUTE_RESIST_AIR), player->GetBaseResistance(CHARACTER_ATTRIBUTE_RESIST_AIR)),
          player->GetActualResistance(CHARACTER_ATTRIBUTE_RESIST_AIR), player->GetBaseResistance(CHARACTER_ATTRIBUTE_RESIST_AIR));
  pGUIWindow_CurrentMenu->DrawText(pFontArrus, 266, pY, 0, pTmpBuf.data(), 0, 0, 0);

  a2 = format_4E2E10;
  if ( player->GetActualResistance(CHARACTER_ATTRIBUTE_RESIST_WATER) > 99 || player->GetBaseResistance(CHARACTER_ATTRIBUTE_RESIST_WATER) > 99 )
    a2 = "%s\f%05u\t180%d\f00000 / %d\n";
  pY += LOBYTE(pFontArrus->uFontHeight) - 2;
  sprintf(pTmpBuf.data(), a2, pGlobalTXT_LocalizationStrings[240],
          UI_GetHealthManaAndOtherQualitiesStringColor(player->GetActualResistance(CHARACTER_ATTRIBUTE_RESIST_WATER), player->GetBaseResistance(CHARACTER_ATTRIBUTE_RESIST_WATER)),
          player->GetActualResistance(CHARACTER_ATTRIBUTE_RESIST_WATER), player->GetBaseResistance(CHARACTER_ATTRIBUTE_RESIST_WATER));
  pGUIWindow_CurrentMenu->DrawText(pFontArrus, 266, pY, 0, pTmpBuf.data(), 0, 0, 0);

  a2 = format_4E2E10;
  if ( player->GetActualResistance(CHARACTER_ATTRIBUTE_RESIST_EARTH) > 99 )
    a2 = "%s\f%05u\t180%d\f00000 / %d\n";
  pY += LOBYTE(pFontArrus->uFontHeight) - 2;
  sprintf(pTmpBuf.data(), a2, pGlobalTXT_LocalizationStrings[70],
          UI_GetHealthManaAndOtherQualitiesStringColor(player->GetActualResistance(CHARACTER_ATTRIBUTE_RESIST_EARTH), player->GetBaseResistance(CHARACTER_ATTRIBUTE_RESIST_EARTH)),
          player->GetActualResistance(CHARACTER_ATTRIBUTE_RESIST_EARTH), player->GetBaseResistance(CHARACTER_ATTRIBUTE_RESIST_EARTH));
  pGUIWindow_CurrentMenu->DrawText(pFontArrus, 266, pY, 0, pTmpBuf.data(), 0, 0, 0);

  a2 = format_4E2E10;
  if ( player->GetActualResistance(CHARACTER_ATTRIBUTE_RESIST_MIND) > 99 || player->GetBaseResistance(CHARACTER_ATTRIBUTE_RESIST_MIND) > 99 )
    a2 = "%s\f%05u\t180%d\f00000 / %d\n";
  pY += LOBYTE(pFontArrus->uFontHeight) - 2;
  sprintf(pTmpBuf.data(), a2, pGlobalTXT_LocalizationStrings[142],
          UI_GetHealthManaAndOtherQualitiesStringColor(player->GetActualResistance(CHARACTER_ATTRIBUTE_RESIST_MIND), player->GetBaseResistance(CHARACTER_ATTRIBUTE_RESIST_MIND)),
          player->GetActualResistance(CHARACTER_ATTRIBUTE_RESIST_MIND), player->GetBaseResistance(CHARACTER_ATTRIBUTE_RESIST_MIND));
  if ( player->classType == PLAYER_CLASS_LICH && player->GetBaseResistance(CHARACTER_ATTRIBUTE_RESIST_MIND) == 200 )
    sprintf(pTmpBuf.data(), format_4E2E00, pGlobalTXT_LocalizationStrings[142],
         UI_GetHealthManaAndOtherQualitiesStringColor(player->GetActualResistance(CHARACTER_ATTRIBUTE_RESIST_MIND), 200), pGlobalTXT_LocalizationStrings[625]);
  pGUIWindow_CurrentMenu->DrawText(pFontArrus, 266, pY, 0, pTmpBuf.data(), 0, 0, 0);

  a2 = format_4E2E10;
  if ( player->GetActualResistance(CHARACTER_ATTRIBUTE_RESIST_BODY) > 99 || player->GetBaseResistance(CHARACTER_ATTRIBUTE_RESIST_BODY) > 99 )
    a2 = "%s\f%05u\t180%d\f00000 / %d\n";
  pY += LOBYTE(pFontArrus->uFontHeight) - 2;
  sprintf(pTmpBuf.data(), a2, pGlobalTXT_LocalizationStrings[29],
          UI_GetHealthManaAndOtherQualitiesStringColor(player->GetActualResistance(CHARACTER_ATTRIBUTE_RESIST_BODY), player->GetBaseResistance(CHARACTER_ATTRIBUTE_RESIST_BODY)),
          player->GetActualResistance(CHARACTER_ATTRIBUTE_RESIST_BODY), player->GetBaseResistance(CHARACTER_ATTRIBUTE_RESIST_BODY));
  if ( player->classType == PLAYER_CLASS_LICH && player->GetBaseResistance(CHARACTER_ATTRIBUTE_RESIST_BODY) == 200 )
    sprintf(pTmpBuf.data(), format_4E2E00, pGlobalTXT_LocalizationStrings[29],
          UI_GetHealthManaAndOtherQualitiesStringColor(player->GetActualResistance(CHARACTER_ATTRIBUTE_RESIST_BODY), 200), pGlobalTXT_LocalizationStrings[625]);
  pGUIWindow_CurrentMenu->DrawText(pFontArrus, 266, pY, 0, pTmpBuf.data(), 0, 0, 0);
}

bool awardSort (int i, int j)
{
  if (pAwards[i].uPriority == 0)  //none
    return false;
  else if (pAwards[j].uPriority == 0)
    return true;
  else if(pAwards[i].uPriority == 1)  //fines,arena stuff,etc
    return false;
  else if(pAwards[j].uPriority == 1)
    return true;
  else if(pAwards[i].uPriority == 5) //joined guilds
    return false;
  else if(pAwards[j].uPriority == 5)
    return true;
  else
    return (pAwards[i].uPriority < pAwards[j].uPriority);
}

//----- (00419100) --------------------------------------------------------
void FillAwardsData()
{
  Player* pPlayer = pPlayers[uActiveCharacter];

  memset(achieved_awards.data(), 0, 4000);
  num_achieved_awards = 0;

  memset(pTmpBuf2.data(), 0, 0x7D0u);
  BtnDown_flag = 0;
  BtnUp_flag = 0;
  books_page_number = 0;
  books_primary_item_per_page = 0;
  for ( int i = 1; i < 105; ++i )
  {
    if ( _449B57_test_bit(pPlayer->_achieved_awards_bits, i) && pAwards[i].pText )
      achieved_awards[num_achieved_awards++] = (AwardType)i;
  }
  full_num_items_in_book = num_achieved_awards;
  num_achieved_awards = 0;

  //sort awards index 

  if (full_num_items_in_book>0)
  {
    for (int i = 0; i< full_num_items_in_book; ++i)
      achieved_awards[full_num_items_in_book+i] = (AwardType)(rand()%16);
    for (int i = 1; i< full_num_items_in_book; ++i)
    {
      for (int j = i; j< full_num_items_in_book; ++j)
      {
        AwardType tmp;
        if (pAwards[achieved_awards[j]].uPriority < pAwards[achieved_awards[i]].uPriority)
        {
          tmp= achieved_awards[j];
          achieved_awards[j] = achieved_awards[i];
          achieved_awards[i] = tmp;
        }
      }
    }
  }

    //  if (full_num_items_in_book > 0)
    /* {
        std::stable_sort(achieved_awards.begin(), achieved_awards.end(), awardSort);
    }*/
}

//----- (0043EF2B) --------------------------------------------------------
void WetsuitOn( unsigned int uPlayerID )
{
  CHARACTER_RACE player_race; // edi@2
  signed int player_sex; // eax@2
  int texture_num; // ecx@5
  char pContainer[20]; // [sp+4h] [bp-1Ch]@7

  if ( uPlayerID> 0 )
  {
    player_race = pPlayers[uPlayerID]->GetRace();
    player_sex = pPlayers[uPlayerID]->GetSexByVoice();
    if ( player_race == CHARACTER_RACE_DWARF  )
      texture_num = (player_sex != 0) + 3;
    else 
      texture_num = (player_sex != 0) + 1;
    wsprintfA(pContainer, "pc23v%dBod", texture_num);
    papredoll_dbods[uPlayerID - 1] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
    wsprintfA(pContainer, "pc23v%dlad", texture_num);
    papredoll_dlads[uPlayerID - 1] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
    wsprintfA(pContainer, "pc23v%dlau", texture_num);
    papredoll_dlaus[uPlayerID - 1] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
    wsprintfA(pContainer, "pc23v%drh", texture_num);
    papredoll_drhs[uPlayerID - 1] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
    wsprintfA(pContainer, "pc23v%dlh", texture_num);
    papredoll_dlhs[uPlayerID - 1] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
    wsprintfA(pContainer, "pc23v%dlhu", texture_num);
    papredoll_dlhus[uPlayerID - 1] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);

    if ( pPlayers[uPlayerID]->uCurrentFace == 12 || pPlayers[uPlayerID]->uCurrentFace == 13 )
      papredoll_dbrds[pPlayers[uPlayerID]->uCurrentFace] = 0;
    papredoll_flying_feet[pPlayers[uPlayerID]->uCurrentFace] = 0;
    IsPlayerWearingWatersuit[uPlayerID] = 1;
  }
}

//----- (0043F0BD) --------------------------------------------------------
void WetsuitOff( unsigned int uPlayerID )
{
  char pContainer[20]; // [sp+0h] [bp-18h]@4

  if (uPlayerID > 0 )
  {
    papredoll_dbods[uPlayerID - 1] = pIcons_LOD->LoadTexture(dbod_texnames_by_face[pPlayers[uPlayerID]->uCurrentFace], TEXTURE_16BIT_PALETTE);
    papredoll_dlads[uPlayerID - 1] = pIcons_LOD->LoadTexture(dlad_texnames_by_face[pPlayers[uPlayerID]->uCurrentFace], TEXTURE_16BIT_PALETTE);
    papredoll_dlaus[uPlayerID - 1] = pIcons_LOD->LoadTexture(dlau_texnames_by_face[pPlayers[uPlayerID]->uCurrentFace], TEXTURE_16BIT_PALETTE);
    papredoll_drhs [uPlayerID - 1] = pIcons_LOD->LoadTexture(drh_texnames_by_face [pPlayers[uPlayerID]->uCurrentFace], TEXTURE_16BIT_PALETTE);
    papredoll_dlhs [uPlayerID - 1] = pIcons_LOD->LoadTexture(dlh_texnames_by_face [pPlayers[uPlayerID]->uCurrentFace], TEXTURE_16BIT_PALETTE);
    papredoll_dlhus[uPlayerID - 1] = pIcons_LOD->LoadTexture(dlhu_texnames_by_face[pPlayers[uPlayerID]->uCurrentFace], TEXTURE_16BIT_PALETTE);

    if ( pPlayers[uPlayerID]->uCurrentFace == 12 || pPlayers[uPlayerID]->uCurrentFace == 13 )
    {
      wsprintfA(pContainer, "pc%02dbrd", pPlayers[uPlayerID]->uCurrentFace + 1);
      papredoll_dbrds[pPlayers[uPlayerID]->uCurrentFace] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
    }
    wsprintfA(pContainer, "item281pc%02d", pPlayers[uPlayerID]->uCurrentFace + 1);
    papredoll_flying_feet[pPlayers[uPlayerID]->uCurrentFace] = pIcons_LOD->LoadTexture(pContainer, TEXTURE_16BIT_PALETTE);
    IsPlayerWearingWatersuit[uPlayerID] = 0;
  }
}

//----- (00468F8A) --------------------------------------------------------
void  OnPaperdollLeftClick()
{
  int v1; // ecx@1
  unsigned int v2; // edi@1
  unsigned int v3; // edx@4
  unsigned int pSkillType; // esi@5
  unsigned __int16 v5; // ax@7
  int v7; // esi@27
  int v8; // eax@29
  int v17; // eax@44
  unsigned int v18; // ecx@55
  unsigned int v19; // eax@55
  unsigned int v22; // eax@61
  int v23; // eax@62
  int v26; // eax@69
  int v34; // esi@90
  int v36; // esi@93
  ItemGen *v38; // edi@93
  ItemGen _this; // [sp+Ch] [bp-40h]@1
  unsigned int v48; // [sp+30h] [bp-1Ch]@88
  unsigned int v50; // [sp+38h] [bp-14h]@50
  int v51; // [sp+3Ch] [bp-10h]@1
  int v52; // [sp+40h] [bp-Ch]@5
  ITEM_EQUIP_TYPE pEquipType;

  v51 = 0;
  _this.Reset();
  v1 = pPlayers[uActiveCharacter]->pEquipment.uMainHand;
  v2 = pPlayers[uActiveCharacter]->pEquipment.uShield;
  if ( v1 && pPlayers[uActiveCharacter]->pInventoryItemList[v1 - 1].GetItemEquipType() == EQUIP_MAIN_HAND )
    v51 = v1;
  v3 = pParty->pPickedItem.uItemID;
  if ( pParty->pPickedItem.uItemID )
  {
    pEquipType = pParty->pPickedItem.GetItemEquipType();
    pSkillType = pParty->pPickedItem.GetPlayerSkillType();
    if ( pSkillType == 4 )
    {
      if ( v2 )
      {
        LOBYTE(v5) = pPlayers[uActiveCharacter]->GetActualSkillLevel(PLAYER_SKILL_SPEAR);
        if ( (signed int)SkillToMastery(v5) < 3 )
        {
          pPlayers[uActiveCharacter]->PlaySound(SPEECH_39, 0);
          return;
        }
        v3 = pParty->pPickedItem.uItemID;
      }
    }
    else
    {
      if ( (pSkillType == 8 || pSkillType == 1 || pSkillType == 2)
        && v1
        && pPlayers[uActiveCharacter]->pInventoryItemList[v1 - 1].GetPlayerSkillType() == 4 )
      {
        LOBYTE(v5) = pPlayers[uActiveCharacter]->GetActualSkillLevel(PLAYER_SKILL_SPEAR);
        if ( (signed int)SkillToMastery(v5) < 3 )
        {
          pPlayers[uActiveCharacter]->PlaySound(SPEECH_39, 0);
          return;
        }
      }
    }
    if ( !pPlayers[uActiveCharacter]->CanEquip_RaceAndAlignmentCheck(v3) )
    {

        pPlayers[uActiveCharacter]->PlaySound(SPEECH_39, 0);
        return;
    }
    if ( pParty->pPickedItem.uItemID == ITEM_WETSUIT )
    {
      pPlayers[uActiveCharacter]->EquipBody((ITEM_EQUIP_TYPE)3);
      WetsuitOn(uActiveCharacter);
      return;
    }
    switch ( pEquipType )
    {
      case EQUIP_BOW:
      case EQUIP_ARMOUR:
      case EQUIP_HELMET:
      case EQUIP_BELT:
      case EQUIP_CLOAK:
      case EQUIP_GAUNTLETS:
      case EQUIP_BOOTS:
      case EQUIP_AMULET:
        if ( !pPlayers[uActiveCharacter]->HasSkill(pSkillType) )//нет навыка
        {
          pPlayers[uActiveCharacter]->PlaySound(SPEECH_39, 0);
          return;
        }
        if ( pPlayers[uActiveCharacter]->HasUnderwaterSuitEquipped() && (pEquipType != EQUIP_ARMOUR || bUnderwater) )
        {
          pAudioPlayer->PlaySound(SOUND_error, 0, 0, -1, 0, 0, 0, 0);
          return;
        }
        pPlayers[uActiveCharacter]->EquipBody(pEquipType);
        if ( pParty->pPickedItem.uItemID == ITEM_WETSUIT )
          WetsuitOff(uActiveCharacter);
        return;
//------------------------dress rings(одевание колец)----------------------------------
      case EQUIP_RING:
        if ( pPlayers[uActiveCharacter]->HasUnderwaterSuitEquipped() )
        {
          pAudioPlayer->PlaySound(SOUND_error, 0, 0, -1, 0, 0, 0, 0);
          return;
        }
        //слоты для колец
        v7 = 0;
        for ( v52 = 10; (signed int)v52 < 16; ++v52 )
        {
          if ( !pPlayers[uActiveCharacter]->pEquipment.uRings[v7] )
          {
            v8 = pPlayers[uActiveCharacter]->FindFreeInventoryListSlot();
            if ( v8 >= 0 )
            {
              pParty->pPickedItem.uBodyAnchor = v52 + 1;
              memcpy(&pPlayers[uActiveCharacter]->pInventoryItemList[v8], &pParty->pPickedItem, sizeof(pPlayers[uActiveCharacter]->pInventoryItemList[v8]));
              pPlayers[uActiveCharacter]->pEquipment.uRings[v7] = v8 + 1;
              pMouse->RemoveHoldingItem();
              break;
            }
          }
          v7++;
        }
        if ( v52 == 16 )//замещение последнего кольца
        {
          v52 = pPlayers[uActiveCharacter]->pEquipment.uRings[5] - 1;
          memcpy(&_this, &pParty->pPickedItem, sizeof(_this));
          pPlayers[uActiveCharacter]->pInventoryItemList[v52].uBodyAnchor = 0;
          pParty->pPickedItem.Reset();
          pParty->SetHoldingItem(&pPlayers[uActiveCharacter]->pInventoryItemList[v52]);
          _this.uBodyAnchor = 16;
          memcpy(&pPlayers[uActiveCharacter]->pInventoryItemList[v52], &_this, 0x24u);
          pPlayers[uActiveCharacter]->pEquipment.uRings[5] = v52 + 1;
        }
        return;
//------------------dress shield(одеть щит)------------------------------------------------------
      case EQUIP_SHIELD://Щит
        if ( pPlayers[uActiveCharacter]->HasUnderwaterSuitEquipped() )//в акваланге
        {
          pAudioPlayer->PlaySound(SOUND_error, 0, 0, -1, 0, 0, 0, 0);
          return;
        }
        if ( !pPlayers[uActiveCharacter]->HasSkill(pSkillType) )//нет навыка
        {
          pPlayers[uActiveCharacter]->PlaySound(SPEECH_39, 0);
          return;
        }
        if ( v2 )//смена щита щитом
        {
          --v2;
          memcpy(&_this, &pParty->pPickedItem, sizeof(_this));
          pPlayers[uActiveCharacter]->pInventoryItemList[v2].uBodyAnchor = 0;
          pParty->pPickedItem.Reset();
          pParty->SetHoldingItem(&pPlayers[uActiveCharacter]->pInventoryItemList[v2]);
          _this.uBodyAnchor = 1;
          memcpy(&pPlayers[uActiveCharacter]->pInventoryItemList[v2], &_this, 0x24u);
          pPlayers[uActiveCharacter]->pEquipment.uShield = v2 + 1;
          if ( v51 == EQUIP_OFF_HAND )
            return;
        }
        else
        {
          v52 = pPlayers[uActiveCharacter]->FindFreeInventoryListSlot();
          if ( v52 < 0 )
            return;
          if ( !v51 )// обычная установка щита на пустую руку
          {
            pParty->pPickedItem.uBodyAnchor = 1;
            v17 = v52 + 1;
            memcpy(&pPlayers[uActiveCharacter]->pInventoryItemList[v52], &pParty->pPickedItem, sizeof(pPlayers[uActiveCharacter]->pInventoryItemList[v52]));
            pPlayers[uActiveCharacter]->pEquipment.uShield = v17;
            pMouse->RemoveHoldingItem();
            return;
          }
          v1--;//ставим щит когда держит двуручный меч
          memcpy(&_this, &pParty->pPickedItem, sizeof(_this));
          pPlayers[uActiveCharacter]->pInventoryItemList[v1].uBodyAnchor = 0;
          pParty->pPickedItem.Reset();
          pParty->SetHoldingItem(&pPlayers[uActiveCharacter]->pInventoryItemList[v1]);
          _this.uBodyAnchor = 1;
          memcpy(&pPlayers[uActiveCharacter]->pInventoryItemList[v52], &_this, sizeof(pPlayers[uActiveCharacter]->pInventoryItemList[v52]));
          pPlayers[uActiveCharacter]->pEquipment.uShield = v52 + 1;
        }
        pPlayers[uActiveCharacter]->pEquipment.uMainHand = 0;
        return;
//-------------------------taken in hand(взять в руку)-------------------------------------------
      case EQUIP_OFF_HAND:
      case EQUIP_WAND:
        if ( pPlayers[uActiveCharacter]->HasUnderwaterSuitEquipped()
          && pParty->pPickedItem.uItemID != 64
          && pParty->pPickedItem.uItemID != 65 )
        {
          pAudioPlayer->PlaySound(SOUND_error, 0, 0, -1, 0, 0, 0, 0);
          return;
        }
        if ( !pPlayers[uActiveCharacter]->HasSkill(pSkillType) )
        {
          pPlayers[uActiveCharacter]->PlaySound(SPEECH_39, 0);
          return;
        }
        v50 = 0;
        if ( pSkillType == 2 && (unsigned __int16)(pPlayers[uActiveCharacter]->pActiveSkills[2] & 0xFFC0)
          || pSkillType == 1 && (signed int)SkillToMastery(pPlayers[uActiveCharacter]->pActiveSkills[1]) >= 3 )
        {
          v18 = pMouse->uMouseClickX;
          v19 = pMouse->uMouseClickY;
          if ( (signed int)v18 >= 560 )
          {
            if ( !v51 )
            {
              if ( v2 )
              {
                --v2;
                memcpy(&_this, &pParty->pPickedItem, sizeof(_this));
                pPlayers[uActiveCharacter]->pInventoryItemList[v2].uBodyAnchor = 0;
                pParty->pPickedItem.Reset();
                pParty->SetHoldingItem(&pPlayers[uActiveCharacter]->pInventoryItemList[v2]);
                _this.uBodyAnchor = 1;
                memcpy(&pPlayers[uActiveCharacter]->pInventoryItemList[v2], &_this, 0x24u);
                pPlayers[uActiveCharacter]->pEquipment.uShield = v2 + 1;
                if ( pEquipType != EQUIP_WAND )
                  return;
                v50 = _this.uItemID;
                if ( _this.uItemID )
                {
                __debugbreak();  // looks like offset in player's inventory and wand_lut much like case in 0042ECB5
                stru_A750F8[uActiveCharacter - 1]._494836(*((int *)&pSpellDatas[66].uNormalLevelRecovery + v50), uActiveCharacter - 1 + 9);
                }
                break;
              }
              v23 = pPlayers[uActiveCharacter]->FindFreeInventoryListSlot();
              if ( v23 < 0 )
                return;
              pParty->pPickedItem.uBodyAnchor = 1;
              v50 = (unsigned int)&pPlayers[uActiveCharacter]->pInventoryItemList[v23];
              memcpy(&pPlayers[uActiveCharacter]->pInventoryItemList[v23], &pParty->pPickedItem, sizeof(pPlayers[uActiveCharacter]->pInventoryItemList[v23]));
              pPlayers[uActiveCharacter]->pEquipment.uShield = v23 + 1;
              pMouse->RemoveHoldingItem();
              if ( pEquipType != EQUIP_WAND )
                return;
              v22 = *(int *)v50;
              v50 = v22;
              if ( v50 )
              {
                __debugbreak();  // looks like offset in player's inventory and wand_lut much like case in 0042ECB5
                stru_A750F8[uActiveCharacter - 1]._494836(*((int *)&pSpellDatas[66].uNormalLevelRecovery + v50), uActiveCharacter - 1 + 9);
              }
              break;
            }
          }
        }
        if ( !v1 )
        {
          v26 = pPlayers[uActiveCharacter]->FindFreeInventoryListSlot();
          if ( v26 < 0 )
            return;
          pParty->pPickedItem.uBodyAnchor = 2;
          v50 = (unsigned int)&pPlayers[uActiveCharacter]->pInventoryItemList[v26];
          memcpy(&pPlayers[uActiveCharacter]->pInventoryItemList[v26], &pParty->pPickedItem, sizeof(pPlayers[uActiveCharacter]->pInventoryItemList[v26]));
          pPlayers[uActiveCharacter]->pEquipment.uMainHand = v26 + 1;
          pMouse->RemoveHoldingItem();
          if ( pEquipType != EQUIP_WAND )
            return;
          v22 = *(int *)v50;
          v50 = v22;
          if ( v50 )
          {
            __debugbreak();  // looks like offset in player's inventory and wand_lut much like case in 0042ECB5
            stru_A750F8[uActiveCharacter - 1]._494836(*((int *)&pSpellDatas[66].uNormalLevelRecovery + v50), uActiveCharacter - 1 + 9);
          }
          break;
        }
        --v1;
        memcpy(&_this, &pParty->pPickedItem, sizeof(_this));
        pPlayers[uActiveCharacter]->pInventoryItemList[v1].uBodyAnchor = 0;
        pParty->pPickedItem.Reset();
        pParty->SetHoldingItem(&pPlayers[uActiveCharacter]->pInventoryItemList[v1]);
        _this.uBodyAnchor = 2;
        memcpy(&pPlayers[uActiveCharacter]->pInventoryItemList[v1], &_this, 0x24u);
        pPlayers[uActiveCharacter]->pEquipment.uMainHand = v1 + 1;
        if ( pEquipType == EQUIP_WAND )
          v50 = _this.uItemID;
        if ( v51 )
          pPlayers[uActiveCharacter]->pEquipment.uShield = 0;
        if ( v50 )//взять жезл
        {
                __debugbreak();  // looks like offset in player's inventory and wand_lut much like case in 0042ECB5
          stru_A750F8[uActiveCharacter - 1]._494836( *((int *)&pSpellDatas[66].uNormalLevelRecovery + v50), uActiveCharacter - 1 + 9);
        }
        break;
//---------------------------take two hands(взять двумя руками)---------------------------------
      case EQUIP_MAIN_HAND:
        if ( pPlayers[uActiveCharacter]->HasUnderwaterSuitEquipped() )
        {
          pAudioPlayer->PlaySound(SOUND_error, 0, 0, -1, 0, 0, 0, 0);
          return;
        }
        if ( !pPlayers[uActiveCharacter]->HasSkill(pSkillType) )
        {
          pPlayers[uActiveCharacter]->PlaySound(SPEECH_39, 0);
          return;
        }
        if ( v1 )//взять двуручный меч кода нет щита(замещение оружия)
        {
          if ( v2 )
          {
            pAudioPlayer->PlaySound(SOUND_error, 0, 0, -1, 0, 0, 0, 0);
            return;
          }
          --v1;
          memcpy(&_this, &pParty->pPickedItem, sizeof(_this));
          pPlayers[uActiveCharacter]->pInventoryItemList[v1].uBodyAnchor = 0;
          pParty->pPickedItem.Reset();
          pParty->SetHoldingItem(&pPlayers[uActiveCharacter]->pInventoryItemList[v1]);
          _this.uBodyAnchor = 2;
          memcpy(&pPlayers[uActiveCharacter]->pInventoryItemList[v1], &_this, 0x24u);
          pPlayers[uActiveCharacter]->pEquipment.uMainHand = v1 + 1;
        }
        else
        {
          v52 = pPlayers[uActiveCharacter]->FindFreeInventoryListSlot();
          if ( v52 >= 0 )
          {
            if ( v2 )//взять двуручный меч кода есть щит(замещение щитом)
            {
              v2--;
              memcpy(&_this, &pParty->pPickedItem, sizeof(_this));
              pPlayers[uActiveCharacter]->pInventoryItemList[v2].uBodyAnchor = 0;
              pParty->pPickedItem.Reset();
              pParty->SetHoldingItem(&pPlayers[uActiveCharacter]->pInventoryItemList[v2]);
              _this.uBodyAnchor = 2;
              memcpy(&pPlayers[uActiveCharacter]->pInventoryItemList[v52], &_this, sizeof(pPlayers[uActiveCharacter]->pInventoryItemList[v52]));
              pPlayers[uActiveCharacter]->pEquipment.uShield = 0;
              pPlayers[uActiveCharacter]->pEquipment.uMainHand = v52 + 1;
            }
            else
            {
              pParty->pPickedItem.uBodyAnchor = 2;
              memcpy(&pPlayers[uActiveCharacter]->pInventoryItemList[v52], &pParty->pPickedItem, sizeof(pPlayers[uActiveCharacter]->pInventoryItemList[v52]));
              pPlayers[uActiveCharacter]->pEquipment.uMainHand = v52 + 1;
              pMouse->RemoveHoldingItem();
            }
          }
        }
        return;
//-------------------------------------------------------------------------------
      default:
        pPlayers[uActiveCharacter]->UseItem_DrinkPotion_etc(uActiveCharacter, 0);//выпить напиток и др.
        return;
    }
    return;
  }

  v34 = pRenderer->pActiveZBuffer[pMouse->uMouseClickX + pSRZBufferLineOffsets[pMouse->uMouseClickY]] & 0xFFFF;
  if ( v34 )
  {
    v36 = v34 - 1;
    v38 = &pPlayers[uActiveCharacter]->pInventoryItemList[v36];
    pEquipType = v38->GetItemEquipType();
    if ( v38->uItemID == ITEM_WETSUIT )
    {
      if ( bUnderwater )
      {
        pAudioPlayer->PlaySound(SOUND_error, 0, 0, -1, 0, 0, 0, 0);
        return;
      }
      WetsuitOff(uActiveCharacter);
    }
    if ( _50C9A0_IsEnchantingInProgress )//наложить закл на экипировку
    {
      *((char *)pGUIWindow_Settings->ptr_1C + 8) &= 0x7Fu;//CastSpellInfo
      *((short *)pGUIWindow_Settings->ptr_1C + 2) = uActiveCharacter - 1;
      *((int *)pGUIWindow_Settings->ptr_1C + 3) = v36;
      *((short *)pGUIWindow_Settings->ptr_1C + 3) = pEquipType;
      ptr_50C9A4_ItemToEnchant = v38;
      _50C9A0_IsEnchantingInProgress = 0;
      if ( pMessageQueue_50CBD0->uNumMessages )
        pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
      pMouse->SetCursorBitmap("MICON1");
      _50C9D4_AfterEnchClickEventSecondParam = 0;
      _50C9D0_AfterEnchClickEventId = 113;
      _50C9D8_AfterEnchClickEventTimeout = 256;
    }
    else
    {
      if ( !ptr_50C9A4_ItemToEnchant )//снять вещь
      {
        pParty->SetHoldingItem(v38);
        pPlayers[uActiveCharacter]->pEquipment.pIndices[pPlayers[uActiveCharacter]->pInventoryItemList[v36].uBodyAnchor - 1] = 0;
        v38->Reset();
      }
    }
  }
  else//снять лук
  {
    if ( pPlayers[uActiveCharacter]->pEquipment.uBow )
    {
      _this = pPlayers[uActiveCharacter]->pInventoryItemList[pPlayers[uActiveCharacter]->pEquipment.uBow - 1];
      pParty->SetHoldingItem(&_this);
      _this.Reset();
      pPlayers[uActiveCharacter]->pEquipment.uBow = 0;
    }
  }
}

//----- (004196A0) --------------------------------------------------------
void CharacterUI_ReleaseButtons()
{
  GUIButton *i; // esi@2
  GUIButton *j; // esi@7

  if ( dword_507CC0_activ_ch )
  {
    dword_507CC0_activ_ch = 0;
    for ( i = pGUIWindow_CurrentMenu->pControlsHead; i; i = j )
    {
      j = i->pNext;
      if ( BYTE1(i->field_1C) & 0x80 )
      {
        i->Release();
        free(i);
      }
    }
    for ( j = pGUIWindow_CurrentMenu->pControlsHead; j; j = j->pNext )
    {
      if ( j->msg == UIMSG_InventoryLeftClick)
      {
        j->uX = dword_50698C_uX;
        j->uY = dword_506988_uY;
        j->uZ = dword_506984_uZ;
        j->uW = dword_506980_uW;
        pGUIWindow_CurrentMenu->_41D08F_set_keyboard_control_group(1, 0, 0, 0);
      }
    }
  }
}