#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <string>

#include "Render.h"
#include "Arcomage.h"
#include "VideoPlayer.h"
#include "AudioPlayer.h"
#include "Mouse.h"
#include "Game.h"
#include "Viewport.h"
#include "Time.h"
#include "GUIFont.h"
#include "Party.h"
#include "GUIWindow.h"
#include "Events2D.h"
#include "VectorTypes.h"
#include "texts.h"
#include <windef.h>
#include "mm7_data.h"



void SetStartConditions();
void SetStartGameData();
void FillPlayerDeck();
void InitalHandsFill();
void GetNextCardFromDeck(int player_num);
int  GetEmptyCardSlotIndex(int player_num);
void IncreaseResourcesInTurn(int player_num);
void TurnChange();
bool IsGameOver();
int  CalculateCardPower(ArcomagePlayer* player, ArcomagePlayer* enemy, ArcomageCard* pCard, int mastery);
char PlayerTurn(int player_num);
void DrawGameUI(int  animation_stage); 
void DrawSparks();
void DrawRectanglesForText();
void DrawPlayersText();
void DrawPlayerLevels(int a1, char *text, POINT *pXY); 
void DrawBricksCount(int a1, char* text, POINT *pXY); 
void DrawGemsCount(int a1, char* text, POINT* pXY);
void DrawBeastsCount(int a1, char *text, POINT *pXY);
void DrawPlayersTowers();
void DrawPlayersWall();
void DrawCards();
void DrawCardAnimation(int a1);
int GetPlayerHandCardCount(int player_num);
int DrawCardsRectangles(int player_num);
bool DiscardCard(int player_num, int card_slot_index);
bool PlayCard(int player_num, int card_slot_num);
bool CanCardBePlayed(int player_num, int hand_card_indx);
void ApplyCardToPlayer(int player_num, unsigned int uCardID); 
int  am_40D2B4(POINT* a1, int a2); // weak
int  ApplyDamageToBuildings(int player_num, int damage); // weak
void GameResultsApply();

void am_DrawText(int a1, const char *pText, POINT *pXY);
void  am_BeginScene(unsigned __int16 *pPcxPixels, int a2, int a3); // idb
void __fastcall Blt_Chroma(RECT  *pSrcXYZW, POINT *pTargetXY, int a3, int a4);
void  Blt_Copy(RECT *pSrcRect, POINT *pTargetXY, int a3);
void  am_EndScene();
void DrawRect(RECT *pXYZW, unsigned __int16 uColor, char bSolidFill);
void DrawSquare( POINT *pTargetXY, unsigned __int16 uColor );
void DrawPixel(POINT *pTargetXY, unsigned __int16 uColor);
int  rand_interval(int min, int max); // idb
void __fastcall am_IntToString(int val, char *pOut);

//----- (0040DEDB) --------------------------------------------------------
unsigned int R8G8B8_to_TargetFormat(int uColor)
{
  return TargetColor(LOBYTE(uColor), BYTE1(uColor), BYTE2(uColor));
}

/*  388 */
#pragma pack(push, 1)
struct ArcomageStartConditions
{
  __int16 max_tower;
  __int16 max_resources;
  __int16 tower_height;
  __int16 wall_height;
  __int16 quarry_level;
  __int16 magic_level;
  __int16 zoo_level;
  __int16 bricks_amount;
  __int16 gems_amount;
  __int16 beasts_amount;
  int mastery_lvl;
};
#pragma pack(pop)
const ArcomageStartConditions start_conditions[13] =
{
  { 30, 100, 15,  5,  2,  2,  2, 10, 10, 10,  0},
  { 50, 150, 20,  5,  2,  2,  2,  5,  5,  5,  1},
  { 50, 150, 20,  5,  2,  2,  2,  5,  5,  5,  2},
  { 75, 200, 25, 10,  3,  3,  3,  5,  5,  5,  2},
  { 75, 200, 20, 10,  3,  3,  3,  5,  5,  5,  1},
  {100, 300, 30, 15,  4,  4,  4, 10, 10, 10,  1},
  {100, 300, 30, 15,  4,  4,  4, 10, 10, 10,  2},
  {150, 400, 20, 10,  5,  5,  5, 25, 25, 25,  0},
  {200, 500, 20, 10,  1,  1,  1, 15, 15, 15,  2},
  {100, 300, 20, 50,  1,  1,  5,  5,  5, 25,  0},
  {125, 350, 10, 20,  3,  1,  2, 15,  5, 10,  2},
  {125, 350, 10, 20,  3,  1,  2, 15,  5, 10,  1},
  {100, 300, 50, 50,  5,  3,  5, 20, 10, 20,  0}
};

#define SIG_trpg  0x67707274
#define SIG_xxxx  0x78787878

ArcomageGame *pArcomageGame = new ArcomageGame;

ArcomagePlayer am_Players[2]; // 00505708
AcromageCardOnTable shown_cards[10]; // 004FAA78
stru272 array_4FABD0[10]; // 4FABD0


ArcomageDeck playDeck; // 00505288
ArcomageDeck deckMaster; // 005054C8

char Player2Name[] = "Enemy";
char Player1Name[] = "Player";

/*  267 */
#pragma pack(push, 1)
struct stru273
{
  bool _40DD2F();
  bool Inside(RECT*pXYZW);

  int x;
  int y;
  char curr_mouse_left;
  char mouse_left_state_changed;
  char curr_mouse_right;
  char mouse_right_state_changed;
};
#pragma pack(pop)


#pragma pack(push, 1)
struct am_2
    {  
    int slot_index;
    int card_power;
    };
#pragma pack(pop)

am_2  cards_power[10];
std::array<__int16, 12> am_sounds;

char byte_4E185C  = 1; // weak
char am_byte_4E185D = 1; // weak
char use_start_bonus = 1; // weak

int start_tower_height;
int start_wall_height; 
int start_quarry_level; 
int start_magic_level; 
int start_zoo_level; 


int minimum_cards_at_hand  = 5;  // 004E1874
int quarry_bonus           = 1; 
int magic_bonus            = 1; 
int zoo_bonus              = 1; 

int max_tower_height       = 50; 
int max_resources_amount   = 100 ; 

int opponent_mastery = 1 ; // weak

bool am_gameover; // 004FAA2C
char byte_4FAA2D; // weak

int current_player_num; // 004FAA6C
char need_to_discard_card; // 04FAA77

int current_card_slot_index;  // 004FABBC
int played_card_id; // 4FABC0
int uCardID; // 4FAA50

int deck_walk_index; // 004FABC8

int start_bricks_amount; 
int start_gems_amount; 
int start_beasts_amount; 

POINT amuint_4FAA3C_blt_xy;
POINT am_uint_4FAA44_blt_xy;
POINT amuint_4FAA54_blt_xy;
POINT amuint_4FAA5C_blt_xy;

int dword_4FAA64; // weak
int dword_4FAA68; // weak

int dword_4FABB8; // weak

char byte_4FAA00; // weak

int amuint_4FAA34; // weak
int amuint_4FAA38; // weak
int amuint_4FAA4C; // weak

char byte_4FAA2E; // weak

int dword_4FAA70; // weak
char byte_4FAA74; // weak
char am_byte_4FAA75; // weak
char am_byte_4FAA76; // weak

int amuint_4FABC4; // weak


char byte_505880; // weak
char byte_505881; // weak

//----- (0040DD2F) --------------------------------------------------------
bool stru273::_40DD2F()
{

  this->x = pArcomageGame->mouse_x;
  this->y = pArcomageGame->mouse_y;
  this->curr_mouse_left = pArcomageGame->mouse_left;
  this->mouse_left_state_changed = pArcomageGame->mouse_left == pArcomageGame->prev_mouse_left;
  this->curr_mouse_right = pArcomageGame->mouse_right;
  this->mouse_right_state_changed = pArcomageGame->mouse_right == pArcomageGame->prev_mouse_right;
  pArcomageGame->prev_mouse_left = pArcomageGame->mouse_left;
  pArcomageGame->prev_mouse_right = pArcomageGame->mouse_right;
  return true;
}

//----- (0040DD93) --------------------------------------------------------
bool stru273::Inside(RECT*pXYZW )
    {

  return (x >= pXYZW->left) && (x <= pXYZW->right) && 
          (y>= pXYZW->top) && (y <= pXYZW->bottom);
}

//----- (0040DFD1) --------------------------------------------------------
stru272_stru0 *stru272_stru0::New()
{
    stru272_stru0 *v2 = (stru272_stru0 *)malloc(0x5Cu);
    v2->signature = SIG_trpg;
    v2->position_in_sparks_arr = 0;
    v2->field_30 = 0.0;
    v2->field_58 = 0;
    v2->field_59 = 0;

    return v2;
}

//----- (0040DFFE) --------------------------------------------------------
int stru272_stru0::Free()
    {

    if ( this->signature == SIG_trpg )
        {
        this->signature = SIG_xxxx;
        free(this);
        return 0;
        }
    else
        return 2;

    }

//----- (0040E01A) --------------------------------------------------------
int stru272_stru0::StartFill( stru272_stru2* a2 )
    {
    stru272_stru0* a1 = this;
    if ( a1->signature == SIG_trpg )
        {
        a1->field_4 = a2->field_20;
        a1->field_C = a2->effect_area.left << 16;
        a1->field_10 = a2->effect_area.top << 16;
        a1->field_14 = a2->effect_area.right << 16;
        a1->field_18 = a2->effect_area.bottom << 16;
        a1->field_1C = a2->field_10;
        a1->field_20 = a2->field_14;
        a1->field_24 = a2->field_18;
        a1->field_28 = (float)(a2->field_1Cf * 65536.0);
        a1->field_2C = a2->field_24f;
        a1->field_34 = (int)(a2->field_28f * 65536.0);
        a1->field_38 = (int)(a2->field_2Cf * 65536.0);
        a1->field_3C = a2->field_30;
        a1->field_40 = a2->field_34;
        a1->field_54 = a2->sparks_array;
        a1->field_59 = 1;

        return 0;
        }
    else
       return 2;
    }

//----- (0040E0F5) --------------------------------------------------------
int stru272_stru0::Clear(char a2, char a3)
    {
    if ( signature == SIG_trpg)
        {
        if ( a2 )
            {
            position_in_sparks_arr = 0;
            field_30 = 0.0;
            }
        if ( field_59 && a3 )
            {

            for (int i=0; i<field_4; ++i)
                field_54[i].have_spark = 0;
             field_58 = 0;
            }
        return 0;
        }
    else
        return 2;
    }

//----- (0040E133) --------------------------------------------------------
int stru272_stru0::DrawEffect()
    {

    stru272_stru0 *v1; // edi@1
    int v3; // ST18_4@3
    double v4; // st7@3
    double v5; // st6@4
    char v6; // bl@8
    stru272_stru1 *v7; // esi@8
    int v8; // ecx@10
    signed int v9; // eax@10
    int v10; // ecx@10
    signed int v11; // eax@10
    int v12; // ebx@12
    int v13; // ST1C_4@12
    int v14; // ebx@12
    int v15; // ST1C_4@12
    signed int v16; // edx@12
    int v17; // ebx@12
    int v18; // ST1C_4@12
    signed int v19; // edx@12
    int v20; // [sp+8h] [bp-10h]@8
    int v21; // [sp+Ch] [bp-Ch]@8
    float v22; // [sp+14h] [bp-4h]@3

    v1 = this;
    if ( this->signature != SIG_trpg )
        return 2;
    v3 = this->position_in_sparks_arr;
    v22 = this->field_30;
    v4 = v3;
    if ( v3 > 0 )
        {
        v5 = v22 + this->field_2C;
        v22 = v5;
        if ( v5 > v4 )
            v22 = v4;
        }
    if ( v22 >= 1.0 || this->field_58 )
        {
        v6 = 0;
        v7 = this->field_54;
        v20 = this->field_28;
        v21 = this->field_4;
        while ( v7->have_spark <= 0 )
            {
            if ( v22 >= 1.0 )
                {
                v12 = v1->field_40;
                v13 = v1->field_3C;
                v7->have_spark = rand_interval(v13,v12);
                v7->field_14 = (rand() % 17 - 8) << 16;
                v7->field_18 = (rand() % 17 - 8) << 16;

                v14 = v1->field_14 - 1;
                v15 = v1->field_C;
                v16 = rand_interval(v15,v14);
                v7->field_C = v16;
                v7->spark_position.x = v16 >> 16;

                v17 = v1->field_18 - 1;
                v18 = v1->field_10;
                v19 = rand_interval(v17, v18);
                v7->field_10 = v19;
                v7->spark_position.y = v19 >> 16;
                --v1->position_in_sparks_arr;
                v22 = v22 - 1.0;
                goto LABEL_13;
                }
LABEL_14:
            ++v7;
            --v21;
            if ( !v21 )
                {
                v1->field_58 = v6;
                v1->field_30 = v22;
                return 0;
                }
            }
        v8 = v7->field_14;
        --v7->have_spark;
        v9 = v8 + v7->field_C;

        v10 = v20 + v7->field_18;
        v7->field_C = v9;
        v7->spark_position.x = v9 >> 16;

        v11 = v10 + v7->field_10;
        v7->field_18 = v10;
        v7->field_10 = v11;
        v7->spark_position.y = v11 >> 16;
LABEL_13:
        v6 = 1;
        goto LABEL_14;
        }
    return 0;
    }

//----- (0040E2A7) --------------------------------------------------------
int stru272_stru0::_40E2A7()
    {

    if (signature == SIG_trpg )
        {
        if ( position_in_sparks_arr <= 0 )
            return field_58 != 0 ? 2 : 0;
        else
            return 1;
        }
    else
        return 3;
    }


//----- (0040DFAF) --------------------------------------------------------
void ArcomageGame::OnMouseClick(char right_left, bool bDown)
{
  if ( right_left )
    pArcomageGame->mouse_right = bDown;
  else
    pArcomageGame->mouse_left = bDown;
}

//----- (0040DFC1) --------------------------------------------------------
void ArcomageGame::OnMouseMove(int x, int y)
{
  pArcomageGame->mouse_x = x;
  pArcomageGame->mouse_y = y;
}


//----- (0040DF47) --------------------------------------------------------
void DoBlt_Copy(unsigned __int16 *pPixels)
{
  RECT pSrcRect;
  POINT pTargetPoint; // [sp+1Ch] [bp-8h]@1

  pRenderer->Present();

  pTargetPoint.x = 0;
  pTargetPoint.y = 0;

  pSrcRect.left   = 0;
  pSrcRect.top    = 0;
  pSrcRect.right  = 640;
  pSrcRect.bottom = 480;

  pRenderer->BeginScene();
  pArcomageGame->pBlit_Copy_pixels = pPixels;
  Blt_Copy(&pSrcRect, &pTargetPoint, 2);
  pRenderer->EndScene();
  pArcomageGame->pBlit_Copy_pixels = 0;
}

//----- (0040DDC9) --------------------------------------------------------
void ArcomageGame::PlaySound( unsigned int event_id )
    {
  SoundID play_sound_id; // eax@10

  switch ( event_id )
      {
  case 40:
  case 43:
  case 46:
      play_sound_id = SOUND_Arcomage_LoseResources;
      break;
  case 39:
  case 41:
  case 42:
  case 44:
  case 45:
  case 47:
      play_sound_id = SOUND_Arcomage_AddResources;
      break;
  case 0:
  case 12:
  case 14:
  case 15:
  case 16:
  case 48:
  case 50:
  case 53:
      play_sound_id = SOUND_Arcomage_TowerWallDamage;
      break;
  case 21:
  case 22:
  case 23:
      play_sound_id = SOUND_Arcomage_DrawCard;
      break;
  case 56:
      play_sound_id = SOUND_Arcomage_124;
      break;
  case 31:
  case 34:
  case 37:
      play_sound_id = SOUND_Arcomage_ProductionDamage;
      break;
  case 1:
  case 30:
  case 32:
  case 33:
  case 35:
  case 36:
  case 38:
      play_sound_id = SOUND_Arcomage_ProductionUpgrade;
     break;
  case 20:
      play_sound_id = SOUND_Arcomage_127;
      break;
  case 3:
      play_sound_id = SOUND_Arcomage_128;
      break;
  case 52:
  case 54:
      play_sound_id = SOUND_Arcomage_TowerUpgrade;
      break;
  case 10:
  case 11:
  case 13:
      play_sound_id = SOUND_Arcomage_130;
      break;
  case 55u:
      play_sound_id = SOUND_Arcomage_131;
      break;
  case 49:
  case 51:
      play_sound_id = SOUND_Arcomage_WallUpgrade;
      break;
  default:
      return;
      }
  pAudioPlayer->PlaySound(play_sound_id, 0, 0, -1, 0, 0, 0, 0);
}

//----- (0040DC2D) --------------------------------------------------------
bool ArcomageGame::MsgLoop(int a1, ArcomageGame_stru1 *a2)
{
  void *v2; // ebp@1
  BOOL v3; // eax@1
  //int v4; // edx@6
  //int v5; // edx@6
  //int v6; // eax@6
  //int v7; // edx@7

  v2 = a2;
  pArcomageGame->field_0 = 0;
  pArcomageGame->stru1.field_0 = 0;
  v3 = PeekMessageA(&pArcomageGame->msg, 0, 0, 0, PM_REMOVE);
  if ( pArcomageGame->msg.message == WM_QUIT )
    Game_DeinitializeAndTerminate(0);
  if ( v3 > 0 )
  {
    TranslateMessage(&pArcomageGame->msg);
    DispatchMessageA(&pArcomageGame->msg);
  }
  /*if (pAsyncMouse)
  {
    EnterCriticalSection(&pGame->pThreadWardInstance->csAsyncMouse);
    v4 = *((unsigned int *)pAsyncMouse + 7);
    pArcomageGame->mouse_x = *((unsigned int *)pAsyncMouse + 6);
    pArcomageGame->mouse_y = v4;
    v5 = *((unsigned int *)pAsyncMouse + 27);
    v6 = *(unsigned int *)v5;
    if ( *(unsigned int *)v5 != v5 )
    {
      do
      {
        v7 = *(unsigned int *)(v6 + 20);
        if ( v7 & 1 )
        {
          pArcomageGame->stru1.field_0 = 7;
        }
        else
        {
          if ( v7 & 2 )
            pArcomageGame->stru1.field_0 = 8;
        }
        v6 = *(unsigned int *)v6;
      }
      while ( v6 != *((unsigned int *)pAsyncMouse + 27) );
    }
    pAsyncMouse->_46B944();
    if ( !*((unsigned char *)pAsyncMouse + 90) )
      pArcomageGame->field_F6 = 1;
    LeaveCriticalSection(&pGame->pThreadWardInstance->csAsyncMouse);
  }*/
  memcpy(v2, &pArcomageGame->stru1, 0xCu);
  return pArcomageGame->stru1.field_0 != 0;
}


//----- (0040D7D5) --------------------------------------------------------
void am_BeginScene(unsigned __int16 *pPcxPixels, int a2, int a3)
{
  pRenderer->BeginScene();
  pArcomageGame->pBlit_Copy_pixels = pPcxPixels;
}

//----- (0040D7EC) --------------------------------------------------------
void __fastcall Blt_Chroma(RECT *pSrcRect, POINT *pTargetPoint, int a3, int blend_mode)
{

unsigned __int16 *pSrc; // eax@2
unsigned __int16 *dest_surf_pos; // edi@3
int uSrcTotalWidth; // ecx@4
unsigned int v10; // esi@9
int v21; // [sp+Ch] [bp-18h]@8
unsigned __int16 *src_surf_pos; // [sp+10h] [bp-14h]@9
__int32 src_width; // [sp+14h] [bp-10h]@3
__int32 src_height; // [sp+18h] [bp-Ch]@3
int uSrcPitch; // [sp+1Ch] [bp-8h]@5


if ( !pRenderer->uNumSceneBegins )
    return;
if ( !pArcomageGame->pBlit_Copy_pixels )
    return;

dest_surf_pos = &pRenderer->pTargetSurface[pTargetPoint->x + pTargetPoint->y * pRenderer->uTargetSurfacePitch];
src_width = pSrcRect->right - pSrcRect->left;
src_height = pSrcRect->bottom - pSrcRect->top;

if ( pArcomageGame->pBlit_Copy_pixels == pArcomageGame->pBackgroundPixels )
    uSrcTotalWidth = pArcomageGame->pGameBackground.uWidth;
else if ( pArcomageGame->pBlit_Copy_pixels == pArcomageGame->pSpritesPixels )
    uSrcTotalWidth = pArcomageGame->pSprites.uWidth;

//v20 = 157;
//v19 = "E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\am_nw.cpp";
//v21 = &v18;
//std__string_char_40E2C8(&v18, "Problem in Blit_Chroma", &a3a);
//466D09_xcpt_string(&v21, v18, v19, v20);
//pSrc = pArcomageGame.pBlit_Copy_pixels;
//LABEL_9:
pSrc = pArcomageGame->pBlit_Copy_pixels;
uSrcPitch = uSrcTotalWidth;

src_surf_pos = &pSrc[pSrcRect->left + uSrcPitch * pSrcRect->top];
v10 = 0xFFu >> (8 - (unsigned __int8)pRenderer->uTargetBBits);
v21 = (pRenderer->uTargetGBits != 6 ? 0x31EF : 0x7BEF);


if ( blend_mode == 2 )
    {
        uSrcPitch =  (uSrcPitch - src_width);
        for (int i=0; i< src_height; ++i)
            {
            for (int j=0; j< src_width; ++j)
                {
                if ( *src_surf_pos != v10 )
                    *dest_surf_pos = *src_surf_pos;
                ++dest_surf_pos;
                ++src_surf_pos;
                }
            src_surf_pos+=uSrcPitch;
            dest_surf_pos += pRenderer->uTargetSurfacePitch - src_width;
            }
    }
else 
    {
    uSrcPitch = (uSrcPitch - src_width);
    for (int i=0; i< src_height; ++i)
        {
        for (int j=0; j< src_width; ++j)
            {
            if ( *src_surf_pos != v10 )
                *dest_surf_pos = (v21 & (*dest_surf_pos >> 1)) + (v21 & (*src_surf_pos >> 1));
            ++dest_surf_pos;
            ++src_surf_pos;
            }
        src_surf_pos+=uSrcPitch;
        dest_surf_pos += pRenderer->uTargetSurfacePitch - src_width;
        }
    }

}

//----- (0040D9B1) --------------------------------------------------------
void Blt_Copy( RECT *pSrcRect, POINT *pTargetPoint, int blend_mode )
    {

    unsigned __int16 *pSrc; // eax@2
    unsigned __int16 *dest_surf_pos; // edi@3
    int uSrcTotalWidth; // ecx@4
    int v21; // [sp+Ch] [bp-18h]@8
    unsigned __int16 *src_surf_pos; // [sp+10h] [bp-14h]@9
    __int32 src_width; // [sp+14h] [bp-10h]@3
    __int32 src_height; // [sp+18h] [bp-Ch]@3
    int uSrcPitch; // [sp+1Ch] [bp-8h]@5

    if ( !pRenderer->uNumSceneBegins )
        return;
    if ( !pArcomageGame->pBlit_Copy_pixels )
        return;

    dest_surf_pos = &pRenderer->pTargetSurface[pTargetPoint->x + pTargetPoint->y * pRenderer->uTargetSurfacePitch];
    src_width = pSrcRect->right - pSrcRect->left;
    src_height = pSrcRect->bottom - pSrcRect->top;

    if ( pArcomageGame->pBlit_Copy_pixels == pArcomageGame->pBackgroundPixels )
        uSrcTotalWidth = pArcomageGame->pGameBackground.uWidth;
    else if ( pArcomageGame->pBlit_Copy_pixels == pArcomageGame->pSpritesPixels )
        uSrcTotalWidth = pArcomageGame->pSprites.uWidth;

    //v20 = 157;
    //v19 = "E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\am_nw.cpp";
    //v21 = &v18;
    //std__string_char_40E2C8(&v18, "Problem in Blit_Chroma", &a3a);
    //466D09_xcpt_string(&v21, v18, v19, v20);
    //pSrc = pArcomageGame.pBlit_Copy_pixels;
    //LABEL_9:
    pSrc = pArcomageGame->pBlit_Copy_pixels;
    uSrcPitch = uSrcTotalWidth;

    src_surf_pos = &pSrc[pSrcRect->left + uSrcPitch * pSrcRect->top];
    v21 = (pRenderer->uTargetGBits != 6 ? 0x31EF : 0x7BEF);

    if ( blend_mode == 2 )
        {
        uSrcPitch =  (uSrcPitch - src_width);
        for (int i=0; i< src_height; ++i)
            {
            for (int j=0; j< src_width; ++j)
                {    
                *dest_surf_pos = *src_surf_pos;
                ++dest_surf_pos;
                ++src_surf_pos;
                }
            src_surf_pos+=uSrcPitch;
            dest_surf_pos += pRenderer->uTargetSurfacePitch - src_width;
            }
        }
    else 
        {
        uSrcPitch =  (uSrcPitch - src_width);
        for (int i=0; i< src_height; ++i)
            {
            for (int j=0; j< src_width; ++j)
                {
                 *dest_surf_pos = (v21 & (*dest_surf_pos >> 1)) + (v21 & (*src_surf_pos >> 1));
                ++dest_surf_pos;
                ++src_surf_pos;
                }
            src_surf_pos+=uSrcPitch;
            dest_surf_pos += pRenderer->uTargetSurfacePitch - src_width;
            }
        }
}

//----- (0040DB10) --------------------------------------------------------
void  am_EndScene()
{
  pRenderer->EndScene();
  pArcomageGame->pBlit_Copy_pixels = NULL;
}

//----- (0040D7B7) --------------------------------------------------------
bool ArcomageGame::LoadSprites()
{
  pArcomageGame->pSprites.Load("sprites.pcx", 2);
  pArcomageGame->pSpritesPixels = pArcomageGame->pSprites.pPixels;
  return true;
}

//----- (0040D799) --------------------------------------------------------
bool ArcomageGame::LoadBackground()
{
  pArcomageGame->pGameBackground.Load("layout.pcx", 2);
  pArcomageGame->pBackgroundPixels = pArcomageGame->pGameBackground.pPixels;
  return true;
}

int CalculateCardPower(ArcomagePlayer* player, ArcomagePlayer* enemy, ArcomageCard* pCard, int mastery)
    {
    enum V_INDX{
        P_TOWER_M10,
        P_WALL_M10,
        E_TOWER,
        E_WALL,
        E_BUILDINGS,
        E_QUARRY,
        E_MAGIC,
        E_ZOO,
        E_RES,
        V_INDEX_MAX
        };

    const int mastery_coeff[V_INDEX_MAX][2]= {{10, 5}, //P_TOWER_M10
                                              {2, 1},  //P_WALL_M10
                                              {1, 10}, //E_TOWER
                                              {1, 3},  //E_WALL
                                              {1, 7},  //E_BUILDINGS
                                              {1, 5},  //E_QUARRY
                                              {1, 40},  //E_MAGIC
                                              {1, 40},  //E_ZOO
                                              {1, 2}   //E_RES
        };
    int card_power= 0;
    int element_power;

   
    if ( pCard->to_player_tower  == 99 || pCard->to_pl_enm_tower  == 99|| 
            pCard->to_player_tower2 == 99 || pCard->to_pl_enm_tower2 == 99 )
        element_power = enemy->tower_height - player->tower_height; 
    else
        element_power = pCard->to_player_tower + pCard->to_pl_enm_tower + pCard->to_player_tower2 + pCard->to_pl_enm_tower2;
        
    if ( player->tower_height >= 10 )
      card_power += mastery_coeff[P_TOWER_M10][mastery]*element_power; 
    else
      card_power += 20*element_power; 


    if ( pCard->to_player_wall  == 99 || pCard->to_pl_enm_wall  == 99 || 
         pCard->to_player_wall2 == 99 || pCard->to_pl_enm_wall2 == 99 )
        element_power = enemy->wall_height - player->wall_height;
    else
        element_power = pCard->to_player_wall  + pCard->to_pl_enm_wall + 
                        pCard->to_player_wall2 + pCard->to_pl_enm_wall2;
    if ( player->wall_height >= 10 )
        card_power += mastery_coeff[P_WALL_M10][mastery]*element_power; //1
    else
        card_power += 5*element_power; 


    card_power += 7 * (pCard->to_player_buildings  + pCard->to_pl_enm_buildings + 
                       pCard->to_player_buildings2 + pCard->to_pl_enm_buildings2);

    if ( pCard->to_player_quarry_lvl  == 99 || pCard->to_pl_enm_quarry_lvl  == 99 || 
         pCard->to_player_quarry_lvl2 == 99 || pCard->to_pl_enm_quarry_lvl2 == 99 )
        element_power = enemy->quarry_level - player->quarry_level;
    else
        element_power = pCard->to_player_quarry_lvl  + pCard->to_pl_enm_quarry_lvl + 
                        pCard->to_player_quarry_lvl2 + pCard->to_pl_enm_quarry_lvl;

    card_power += 40 * element_power;

    if ( pCard->to_player_magic_lvl  == 99 || pCard->to_pl_enm_magic_lvl  == 99 || 
         pCard->to_player_magic_lvl2 == 99 || pCard->to_pl_enm_magic_lvl2 == 99 )
        element_power = enemy->magic_level - player->magic_level;
    else
        element_power = pCard->to_player_magic_lvl  + pCard->to_pl_enm_magic_lvl + 
                        pCard->to_player_magic_lvl2 + pCard->to_pl_enm_magic_lvl2;
    card_power += 40 *element_power;

    if ( pCard->to_player_zoo_lvl  == 99 || pCard->to_pl_enm_zoo_lvl  == 99 || 
         pCard->to_player_zoo_lvl2 == 99 || pCard->to_pl_enm_zoo_lvl2 == 99 )
        element_power = enemy->zoo_level - player->zoo_level;
    else
        element_power =  pCard->to_player_zoo_lvl  + pCard->to_pl_enm_zoo_lvl + 
                         pCard->to_player_zoo_lvl2 + pCard->to_pl_enm_zoo_lvl2;
    card_power += 40 *element_power;

    if ( pCard->to_player_bricks  == 99 || pCard->to_pl_enm_bricks  == 99 || 
         pCard->to_player_bricks2 == 99 || pCard->to_pl_enm_bricks2 == 99 )
        element_power = enemy->resource_bricks - player->resource_bricks;
    else
        element_power = pCard->to_player_bricks  + pCard->to_pl_enm_bricks + 
                        pCard->to_player_bricks2 + pCard->to_pl_enm_bricks2;
    card_power += 2 *element_power;


    if ( pCard->to_player_gems  == 99 || pCard->to_pl_enm_gems  == 99 || 
         pCard->to_player_gems2 == 99 || pCard->to_pl_enm_gems2 == 99 )
         element_power = enemy->resource_gems - player->resource_gems;
    else
        element_power = pCard->to_player_gems  + pCard->to_pl_enm_gems + 
                        pCard->to_player_gems2 + pCard->to_pl_enm_gems2;
    card_power += 2 *element_power;

    if ( pCard->to_player_beasts  == 99 || pCard->to_pl_enm_beasts  == 99 || 
        pCard->to_player_beasts2 == 99 || pCard->to_pl_enm_beasts2 == 99 )
        element_power = enemy->resource_beasts - player->resource_beasts;
    else
        element_power = pCard->to_player_beasts  + pCard->to_pl_enm_beasts + 
                        pCard->to_player_beasts2 + pCard->to_pl_enm_beasts2;
    card_power += 2 *element_power;

    if ( pCard->to_enemy_tower == 99 || pCard->to_enemy_tower2 == 99 )
        element_power = player->tower_height - enemy->tower_height;  
    else
        element_power = -(pCard->to_enemy_tower + pCard->to_enemy_tower2);
    card_power += mastery_coeff[E_TOWER][mastery]*element_power;

    if ( pCard->to_enemy_wall == 99 || pCard->to_enemy_wall2 == 99 )
        element_power = player->wall_height - enemy->wall_height;  
    else
        element_power = -(pCard->to_enemy_wall + pCard->to_enemy_wall2);
    card_power += mastery_coeff[E_WALL][mastery]*element_power;

     card_power -= mastery_coeff[E_BUILDINGS][mastery]*(pCard->to_enemy_buildings + pCard->to_enemy_buildings2); 
 
    if ( pCard->to_enemy_quarry_lvl == 99 || pCard->to_enemy_quarry_lvl2 == 99 )
        element_power = player->quarry_level - enemy->quarry_level;  //5
    else
        element_power = -(pCard->to_enemy_quarry_lvl + pCard->to_enemy_quarry_lvl2); //5
    card_power += mastery_coeff[E_QUARRY][mastery]*element_power;

    if ( pCard->to_enemy_magic_lvl == 99 || pCard->to_enemy_magic_lvl2 == 99 )
        element_power = player->magic_level - enemy->magic_level;  //40
    else
        element_power = -(pCard->to_enemy_magic_lvl + pCard->to_enemy_magic_lvl2);
    card_power += mastery_coeff[E_MAGIC][mastery]*element_power;

    if ( pCard->to_enemy_zoo_lvl == 99 || pCard->to_enemy_zoo_lvl2 == 99 )
        element_power = player->zoo_level - enemy->zoo_level; //40
    else
        element_power = -(pCard->to_enemy_zoo_lvl + pCard->to_enemy_zoo_lvl2);
    card_power += mastery_coeff[E_ZOO][mastery]*element_power;

    if ( pCard->to_enemy_bricks == 99 || pCard->to_enemy_bricks2 == 99 )
        element_power = player->resource_bricks - enemy->resource_bricks;  //2
    else
        element_power = -(pCard->to_enemy_bricks + pCard->to_enemy_bricks2);
    card_power += mastery_coeff[E_RES][mastery]*element_power;

    if ( pCard->to_enemy_gems == 99 || pCard->to_enemy_gems2 == 99 )
        element_power = player->resource_gems - enemy->resource_gems; //2
    else
        element_power = -(pCard->to_enemy_gems + pCard->to_enemy_gems2);
    card_power += mastery_coeff[E_RES][mastery]*element_power;

    if ( pCard->to_enemy_beasts == 99 || pCard->to_enemy_beasts2 == 99 )
        element_power = player->resource_beasts - enemy->resource_beasts;  //2
    else
        element_power = -(pCard->to_enemy_beasts + pCard->to_enemy_beasts2);
    card_power += mastery_coeff[E_RES][mastery]*element_power;

    if ( pCard->field_30 || pCard->field_4D )
         card_power *= 10;

    if ( pCard->field_24 == 1 )
        element_power = player->resource_bricks - pCard->needed_bricks;
    else if ( pCard->field_24 == 2 )
        element_power = player->resource_gems   - pCard->needed_gems;
    else if (pCard->field_24 == 3)
        element_power = player->resource_beasts - pCard->needed_beasts;
    if ( element_power > 3 )
        element_power = 3;
    card_power += 5 * element_power;

    if ( enemy->tower_height <= pCard->to_enemy_tower2 + pCard->to_enemy_tower )
        card_power += 9999;
  
    if (pCard->to_enemy_tower2    + pCard->to_enemy_tower + 
        pCard->to_enemy_wall      + pCard->to_enemy_wall2 + 
        pCard->to_enemy_buildings + pCard->to_enemy_buildings2 >= enemy->wall_height + enemy->tower_height) 
        card_power += 9999;

    if ( (pCard->to_player_tower2 + pCard->to_pl_enm_tower2 + 
          pCard->to_player_tower  + pCard->to_pl_enm_tower  + player->tower_height) >= max_tower_height )
        card_power += 9999;

    return card_power;
    }


//----- (00408BB4) --------------------------------------------------------
bool OpponentsAITurn(int player_num)
{
    char result; // al@3
    signed int v2; // ebx@8
    int v3; // esi@8
    int all_player_cards_count; // eax@9
    int random_card_slot; // edi@9
    int v6; // eax@11
    signed int v7; // eax@11
    int v8; // ebx@5
    int v9; // eax@5
    int v10; // ecx@18
    int v11; // eax@20
    ArcomageCard *v12; // ecx@20
    char v13; // dl@20
    char v14; // dl@24
    int v15; // edx@25
    int v16; // edx@27
    char v17; // dl@31
    int v18; // edx@32
    char v19; // dl@35
    char v20; // dl@39
    int v21; // edx@40
    char v22; // dl@45
    int v23; // edx@46
    char v24; // dl@52
    int v25; // edx@53
    char v26; // dl@58
    int v27; // edx@59
    char v28; // dl@64
    int v29; // edx@65
    char v30; // dl@70
    int v31; // edx@71
    char v32; // dl@76
    int v33; // edx@77
    char v34; // dl@82
    int v35; // edx@83
    char v36; // dl@86
    char v37; // dl@90
    int v38; // edi@93
    char v39; // dl@93
    int v40; // esi@93
    char v41; // bl@94
    char v42; // dl@98
    char v43; // dl@102
    char v44; // dl@106
    char v45; // dl@110
    char v46; // dl@114
    ArcomagePlayer *v47; // ebx@115
    ArcomagePlayer *v48; // edx@115
    int v49; // esi@123
    int v50; // edx@123
    int v51; // edx@126
    int v52; // esi@129
    int v53; // edi@137
    int j; // esi@137
    am_2 *v55; // eax@139
    int v56; // ecx@141
    int v57; // edx@141
    int v58; // ebx@141
    int v59; // eax@144
    int v60; // esi@145
    int v61; // ebx@4
    int v62; // eax@4
    int v63; // ecx@167
    int v64; // eax@169
    ArcomageCard *pCard; // ecx@169
    char v66; // dl@169
    char v67; // dl@173
    int v68; // edx@174
    int v69; // edx@176
    char v70; // dl@180
    char v71; // dl@183
    char v72; // dl@187
    int v73; // edx@188
    char v74; // dl@193
    char v75; // dl@199
    int v76; // edx@200
    char v77; // dl@205
    int v78; // edx@206
    char v79; // dl@211
    int v80; // edx@212
    char v81; // dl@217
    int v82; // edx@218
    char v83; // dl@223
    int v84; // edx@224
    char v85; // dl@229
    int v86; // edx@230
    char v87; // dl@233
    int v88; // edx@234
    char v89; // dl@237
    int v90; // edx@238
    int v91; // esi@240
    char v92; // dl@240
    int v93; // esi@240
    char v94; // bl@241
    int v95; // edx@243
    int v96; // edi@244
    char v97; // dl@245
    int v98; // edx@246
    char v99; // dl@249
    int v100; // edx@250
    char v101; // dl@253
    int v102; // edx@254
    char v103; // dl@257
    int v104; // edx@258
    char v105; // dl@261
    ArcomagePlayer *v106; // ebx@262
    ArcomagePlayer *v107; // edx@262
    int v108; // esi@270
    int v109; // edx@270
    int v110; // edx@273
    int v111; // esi@276
    int v112; // edi@284
   // int j; // esi@284
    am_2 *v114; // eax@286
    int v115; // ecx@288
    int v116; // edx@288
    int v117; // ebx@288
    int v118; // eax@291
    int v119; // esi@292
    int v120; // [sp-28h] [bp-28h]@129
    int v121; // [sp-28h] [bp-28h]@276
    int k; // [sp-24h] [bp-24h]@18
    int i; // [sp-24h] [bp-24h]@167
    int v124; // [sp-20h] [bp-20h]@5
    int v125; // [sp-20h] [bp-20h]@4
    int v126; // [sp-1Ch] [bp-1Ch]@17
    int v127; // [sp-1Ch] [bp-1Ch]@139
    int v128; // [sp-1Ch] [bp-1Ch]@166
    int v129; // [sp-1Ch] [bp-1Ch]@286
    int v130; // [sp-18h] [bp-18h]@5
    int cards_count; // [sp-18h] [bp-18h]@4
    int v132; // [sp-14h] [bp-14h]@0
    int v133; // [sp-14h] [bp-14h]@93
    int v134; // [sp-14h] [bp-14h]@129
    int v135; // [sp-14h] [bp-14h]@240
    int v136; // [sp-14h] [bp-14h]@276
    ArcomagePlayer *enemy; // [sp-10h] [bp-10h]@5
    ArcomagePlayer *enemy1; // [sp-10h] [bp-10h]@4
    ArcomagePlayer *player; // [sp-Ch] [bp-Ch]@5
    ArcomagePlayer *player1; // [sp-Ch] [bp-Ch]@4
    char v141; // [sp-8h] [bp-8h]@35
    char v142; // [sp-8h] [bp-8h]@183
    char v143; // [sp-5h] [bp-5h]@20
    char v144; // [sp-5h] [bp-5h]@169

     byte_4FAA00 = 1;
    if ( opponent_mastery == 0)
        {
        if ( need_to_discard_card==0 )  //am_byte_4FAA77
            {
           for(int i = 0; i<10 ; ++i )
                {
                all_player_cards_count = GetPlayerHandCardCount(player_num);
                random_card_slot = rand_interval(0, all_player_cards_count - 1);
                if ( CanCardBePlayed(player_num, random_card_slot) )
                    return PlayCard(player_num, random_card_slot);
                 }
            }
            all_player_cards_count = GetPlayerHandCardCount(player_num);
            random_card_slot= rand_interval(0, all_player_cards_count - 1);
            return DiscardCard(player_num, random_card_slot);
        }
    else if (( opponent_mastery == 1 )|| ( opponent_mastery == 2 ))
        {
        player = &am_Players[player_num];
        enemy = &am_Players[(player_num + 1) % 2];
        all_player_cards_count = GetPlayerHandCardCount(player_num);
        for(int i = 0; i<10 ; ++i )
            {
            if ( i >= all_player_cards_count )
                {
                cards_power[i].slot_index = -1;
                cards_power[i].card_power = -9999;
                }
            else
                {
                cards_power[i].slot_index = i;
                cards_power[i].card_power = 0;
                }
            }
        for(int i = 0; i< all_player_cards_count ; ++i )
            {   
               v12 = &pCards[am_Players[player_num].cards_at_hand[cards_power[i].slot_index]];          
               cards_power[i].card_power = CalculateCardPower(player, enemy, v12, opponent_mastery-1);
            }

            for (int j = all_player_cards_count - 1; j >= 0; --j )
                {
                    for (int m=0; m<j; ++m )
                        {
                        if ( cards_power[m].card_power < cards_power[m+1].card_power )
                            {
                            v56 = cards_power[m].slot_index;
                            v57 = cards_power[m].card_power;
                            cards_power[m].slot_index = cards_power[m+1].slot_index;
                            cards_power[m].card_power = cards_power[m+1].card_power;
                            cards_power[m+1].slot_index = v56;
                            cards_power[m+1].card_power = v57;
                            }
                        }
                }                           
            v59 = all_player_cards_count - 1;
            if ( need_to_discard_card )
                {
                if ( all_player_cards_count - 1 >= 0 )
                    {
                    while ( !pCards[am_Players[player_num].cards_at_hand[cards_power[v59].slot_index]].can_be_discarded )
                        {
                        --v59;
                        if ( v59 < 0 )
                            return DiscardCard(player_num, v132);
                        }
                    v132 = cards_power[v59].slot_index;
                    }
                }
            else
                {
                v60 = 0;
                if ( all_player_cards_count - 1 >= 0 )
                    {
                    while ( !pCards[am_Players[player_num].cards_at_hand[cards_power[v59].slot_index]].can_be_discarded )
                        {
                        --v59;
                        if ( v59 < 0 )
                            goto LABEL_150;
                        }
                    v132 = cards_power[v59].slot_index;
                    }
LABEL_150:
                if ( (all_player_cards_count - 1)> 0 )
                    {
                    while ( !CanCardBePlayed(player_num, cards_power[v60].slot_index) || cards_power[v60].card_power < 0 )
                        {
                        ++v60;
                        if ( v60 >= all_player_cards_count - 1 )
                            return DiscardCard(player_num, v132);
                        }
                    return PlayCard(player_num, cards_power[v60].slot_index);
                    }
                }
            return DiscardCard(player_num, v132);
        }
   

  return true;//result != 0;
}



//----- (00409E6A) --------------------------------------------------------
void ArcomageGame::Loop()
{
  int v0; // edx@2
  int v1; // ecx@14
  signed int v2; // eax@15
  stru272 *v3; // esi@17
  __int16 *v4; // esi@23

  while ( !pArcomageGame->GameOver )
  {
    pArcomageGame->field_F6 = 1;
    byte_4FAA24 = 1;
    IncreaseResourcesInTurn(current_player_num);
LABEL_8:
    while ( byte_4FAA24 )
    {
      played_card_id = -1;
      GetNextCardFromDeck(current_player_num);
      while ( 1 )
      {
        byte_4FAA24 = PlayerTurn(current_player_num);
        if ( GetPlayerHandCardCount(current_player_num) <= minimum_cards_at_hand )
          break;
        need_to_discard_card = 1;
        if ( pArcomageGame->field_F4 )
          goto LABEL_8;
      }
      need_to_discard_card = 0;
    }
    pArcomageGame->GameOver = IsGameOver();
    if ( !pArcomageGame->GameOver )
      TurnChange();
    if ( pArcomageGame->field_F4 )
      pArcomageGame->GameOver = 1;
  }
  GameResultsApply();
  if ( am_gameover )
  {
     dword_4FAA70 = 0;
  }
  else
  {
    dword_4FAA70 = -1;
  }

  for(int i=0; i<10; ++i)
  {
      array_4FABD0[i].field_40->Clear(1, 1);
      array_4FABD0[i].field_40->Free();
  }

  pArcomageGame->pGameBackground.Release();
  pArcomageGame->pSprites.Release();
  pArcomageGame->bGameInProgress = false;
  viewparams->bRedrawGameUI = true;
  if ( pVideoPlayer->AnyMovieLoaded() )
    pVideoPlayer->_4BF5B2();
  for(int i=0; i<12; ++i)
    pSoundList->UnloadSound(am_sounds[i], 1);
}




//----- (00409FE9) --------------------------------------------------------
void SetStartGameData()
{
  signed int j; // edx@7
  int card_id_counter; // edx@13
  signed int i; // ecx@13
  signed int card_dispenser_counter; // eax@13

  current_player_num = dword_505890;
  SetStartConditions();
  for(i=0; i<2; ++i )
  {
    if ( i )
    {
      strcpy(am_Players[1].pPlayerName, pArcomageGame->pPlayer2Name);
      if ( byte_4E185C )
        am_Players[1].IsHisTurn = 0;
      else
        am_Players[1].IsHisTurn = 1;
    }
    else
    {
      strcpy(am_Players[0].pPlayerName, pArcomageGame->pPlayer1Name);
      am_Players[0].IsHisTurn = 1;
    }
    am_Players[i].tower_height    = start_tower_height;
    am_Players[i].wall_height     = start_wall_height;
    am_Players[i].quarry_level    = start_quarry_level;
    am_Players[i].magic_level     = start_magic_level;
    am_Players[i].zoo_level       = start_zoo_level;
    am_Players[i].resource_bricks = start_bricks_amount;
    am_Players[i].resource_gems   = start_gems_amount;
    am_Players[i].resource_beasts = start_beasts_amount;

    for (j=0; j<10; ++j )
    {
      am_Players[i].cards_at_hand[j] = -1;
      if ( am_byte_4E185D )
      {
        am_Players[i].card_shift[j].x = -1;
        am_Players[i].card_shift[j].y = -1;
      }
      else
      {
      am_Players[i].card_shift[j].x = 0;
      am_Players[i].card_shift[j].y = 0;
      }
    }  
  }
  strcpy(deckMaster.name, "Master Deck");
  for (i=0, card_dispenser_counter=-2, card_id_counter=0; i<DECK_SIZE; ++i, ++card_dispenser_counter)
      {
      deckMaster.cardsInUse[i] = 0;
      deckMaster.cards_IDs[i] =card_id_counter;
      switch ( card_dispenser_counter )
          {
      case 0:
      case 2:
      case 6:
      case 9:
      case 13:
      case 18:
      case 23:
      case 33:
      case 36:
      case 38:
      case 44:
      case 46:
      case 52:
      case 57:
      case 69:
      case 71:
      case 75:
      case 79:
      case 81:
      case 84:
      case 89:
          break;
      default:
          ++card_id_counter;
          }
      }

   FillPlayerDeck();
}

//----- (0040A198) --------------------------------------------------------
void FillPlayerDeck()
    {
  signed int m;
  int rand_deck_pos; 
  char card_taken_flags[DECK_SIZE]; 
  int i,j;

  ArcomageGame::PlaySound(20);
  memset(deckMaster.cardsInUse, 0,DECK_SIZE );
  memset(card_taken_flags, 0, DECK_SIZE);

  for (i=0; i<2; ++i)
  {
    for (j=0; j<10; ++j)
    {
      if ( am_Players[i].cards_at_hand[j] > -1 )
      {
        for (m = 0; m<DECK_SIZE; ++m)
            {
            if (deckMaster.cards_IDs[m] == am_Players[i].cards_at_hand[j] && deckMaster.cardsInUse[m] == 0)
                {
                    deckMaster.cardsInUse[m] = 1;
                    break;
                }
            }
      }
    }
  }

  for (i=0; i<DECK_SIZE; ++i)
  {
    do
      rand_deck_pos = rand() % DECK_SIZE;
    while (card_taken_flags[rand_deck_pos] == 1 );

    card_taken_flags[rand_deck_pos]=1;
    playDeck.cards_IDs[i]  = deckMaster.cards_IDs[rand_deck_pos];
    playDeck.cardsInUse[i] = deckMaster.cardsInUse[rand_deck_pos];
  }

  deck_walk_index = 0;
  amuint_4FABC4 = -1;
  pArcomageGame->field_F6 = 1;

}

//----- (0040A255) --------------------------------------------------------
void InitalHandsFill()
    {
  for (int i = 0; i < minimum_cards_at_hand; ++i )
  {
      GetNextCardFromDeck(0);
      GetNextCardFromDeck(1);
  }
  pArcomageGame->field_F6 = 1;

}
// 4E1874: using guessed type int dword_4E1874;

//----- (0040A283) --------------------------------------------------------
void GetNextCardFromDeck( int player_num )
    {
  signed int deck_index; // eax@1
  int new_card_id; // edi@1
  signed int card_slot_indx; // eax@7

  deck_index = deck_walk_index;
  for(;;)
  {
    if ( deck_index >= DECK_SIZE )
    {
      FillPlayerDeck();
      deck_index = deck_walk_index;
    }
    if ( !playDeck.cardsInUse[deck_index] )
        {
        new_card_id = playDeck.cards_IDs[deck_index];
        ++deck_index;
        deck_walk_index = deck_index;
        break;
        }
    ++deck_index;
    deck_walk_index = deck_index;
  }
  
  ArcomageGame::PlaySound(21);
  card_slot_indx = GetEmptyCardSlotIndex(player_num);
  if ( card_slot_indx != -1 )
  {
    amuint_4FAA4C = card_slot_indx;
    am_Players[player_num].cards_at_hand[card_slot_indx] = new_card_id;
    am_Players[player_num].card_shift[card_slot_indx].x = rand_interval(-4, 4);
    am_Players[player_num].card_shift[card_slot_indx].y = rand_interval(-4, 4);
    pArcomageGame->field_F6 = 1;
    byte_4FAA2D = 1;
  }

}


//----- (0040A324) --------------------------------------------------------
int GetEmptyCardSlotIndex( int player_num )
    {
  int i; 
  for (i=0; i<10; ++i )
      {
       if (am_Players[player_num].cards_at_hand[i]==-1)
           break;
      }
  if ( i >= 10 )
      return -1;
  else
     return i;  
}

//----- (0040A346) --------------------------------------------------------
void IncreaseResourcesInTurn(int player_num)
{
  am_Players[player_num].resource_bricks += quarry_bonus + am_Players[player_num].quarry_level;
  am_Players[player_num].resource_gems   += magic_bonus  + am_Players[player_num].magic_level;
  am_Players[player_num].resource_beasts += zoo_bonus    + am_Players[player_num].zoo_level;
}


//----- (0040A383) --------------------------------------------------------
void TurnChange()
{
  char player_name[64]; // [sp+4h] [bp-64h]@4
 // RECT v6; // [sp+44h] [bp-24h]@6
 
  ArcomageGame_stru1 v10; // [sp+54h] [bp-14h]@7
  POINT v11; // [sp+60h] [bp-8h]@4

  if ( !pArcomageGame->field_F4 )
  {
    if ( am_Players[0].IsHisTurn != 1 || am_Players[1].IsHisTurn != 1 )
    {
      ++current_player_num;
      am_byte_4FAA75 = 1;
      if ( current_player_num >= 2 )
        current_player_num = 0;
    }
    else
    {
      //nullsub_1();
   //   v11.x = 0;
   //   v11.y = 0;
      strcpy(player_name, "The Next Player is: ");//"След"
     // v0 = 0;
      v11.y = 200;
      v11.x = 320; // - 12 * v0 / 2;
      am_DrawText(-1, player_name, &v11);
      am_byte_4FAA75 = 1;
      ++current_player_num;
      if ( current_player_num >= 2  )
        current_player_num = 0;
      strcpy(player_name, am_Players[current_player_num].pPlayerName);
     // v4 = 0;
      v11.y = 260;
      v11.x = 320;// - 12 * v4 / 2;
      am_DrawText(-1, player_name, &v11);
     /* v6.left = 0;
      v6.right = 640;
      v6.top = 0;
      v6.bottom = 480;*/
      //nullsub_1();
      pRenderer->Present();
      //nullsub_1();
      while ( 1 )
      {  
          while ( !ArcomageGame::MsgLoop(20, &v10) )
            ;
          if ( v10.field_0 == 1 )
              {
              if ( v10.field_4 )
                  break;
              //nullsub_1();
              continue;
              }
           if (( v10.field_0 > 4 )&& ( v10.field_0 <= 8 )) 
               break;
           if ( v10.field_0 == 10 )
               {
               pArcomageGame->field_F4 = 1;
               byte_4FAA74 = 1;
               break;
               }
      }
      /*  v11.x = 0;
      v11.y = 0;
      v6.left = 0;
      v6.right = 640;
      v6.top = 0;
      v6.bottom = 480;*/
      //nullsub_1();
      pRenderer->Present();
    }
  }
}


//----- (0040A514) --------------------------------------------------------
bool IsGameOver()
{
  bool result; // eax@1

  result = false;
  for(int i=0; i<2; ++i)
  {
    if ( am_Players[i].tower_height <= 0 )
       result  = true;
    if ( am_Players[i].tower_height >= max_tower_height )
       result  = true;
    if (  am_Players[i].resource_bricks >= max_resources_amount
      ||  am_Players[i].resource_gems   >= max_resources_amount
      ||  am_Players[i].resource_beasts >= max_resources_amount )
      result  = true;
  }

  return am_gameover = result;
}


//----- (0040A560) --------------------------------------------------------
char PlayerTurn( int player_num )
    {
  int v1; // ebp@0
  unsigned __int64 v2; // qax@3
  unsigned __int64 v3; // kr00_8@3
 
  int v5; // esi@67
  char Dest[100]; // [sp+8h] [bp-A8h]@67
  int v8; // [sp+6Ch] [bp-44h]@3
  RECT pSrcXYZW; // [sp+70h] [bp-40h]@75
  int v10[4]; // [sp+80h] [bp-30h]@69
  POINT pTargetXY; // [sp+90h] [bp-20h]@75
  ArcomageGame_stru1 a2; // [sp+98h] [bp-18h]@8
  int animation_stage; // [sp+A4h] [bp-Ch]@1
  char v15; // [sp+AEh] [bp-2h]@63
  bool break_loop; // [sp+AFh] [bp-1h]@1

  uCardID = -1;
  break_loop = false;
  animation_stage = 20;
  byte_4FAA00 = 0;
  dword_4FAA68 = 0;
  amuint_4FAA38 = 10;
  amuint_4FAA34 = 5;
  if ( amuint_4FAA4C != -1 )
    byte_4FAA2D = 1;
  do
  {
   
      do
      {
        v3 = pEventTimer->Time() - pArcomageGame->event_timer_time;
      }
      while (v3 < 6);
    
    pArcomageGame->event_timer_time = (unsigned int)pEventTimer->Time();
    if ( pArcomageGame->field_F4 )
      break_loop = 1;
    ArcomageGame::MsgLoop(0, &a2);
    switch ( a2.field_0 )
    {
      case 2:
        if ( a2.field_4 == 129 && a2.field_8 == 1 )
        {
          pAudioPlayer->StopChannels(-1, -1);
          dword_4FAA68 = 0;
          break_loop = true;
          pArcomageGame->field_F4 = 1;
        }
        break;
      case 9:
        pArcomageGame->field_F6 = 1;
        break;
      case 10:
        pAudioPlayer->StopChannels(-1, -1);
        byte_4FAA74 = 1;
        break_loop = true;
        pArcomageGame->field_F4 = 1;
        break;
    }

    if (am_Players[current_player_num].IsHisTurn != 1 && 
        !byte_4FAA00 && !byte_4FAA2E && !byte_4FAA2D )
    {
      if ( am_byte_4FAA75 )
        am_byte_4FAA76 = 1;
      OpponentsAITurn(current_player_num);
      byte_4FAA2E = 1;
    }
    if ( amuint_4FAA4C != -1 && amuint_4FAA38 > 10 )
      amuint_4FAA38 = 10;
    if ( byte_4FAA2E || byte_4FAA2D ||am_Players[current_player_num].IsHisTurn != 1 )
    {
      pArcomageGame->field_F6 = 1;
      if ( byte_4FAA2D )
      {
        --amuint_4FAA38;
        if ( amuint_4FAA38 < 0 )
        {
          byte_4FAA2D = 0;
          amuint_4FAA38 = 10;
          break_loop = false;
        }
      }
      if ( byte_4FAA2E )
      {
        --animation_stage;
        if ( animation_stage < 0 )
        {
          if ( dword_4FAA68 > 1 )
          {
            --dword_4FAA68;
            byte_4FAA00 = 0;
          }
          else
          {
            break_loop = true;
          }
          byte_4FAA2E = 0;
          animation_stage = 20;
        }
      }
    }
    else
    {
      if ( need_to_discard_card )
      {
        if ( a2.field_0 == 7 && DiscardCard(player_num, current_card_slot_index) )
        {
          if ( am_byte_4FAA75 )
            am_byte_4FAA76 = 1;
          if ( dword_4FAA64 > 0 )
          {
            --dword_4FAA64;
            need_to_discard_card = GetPlayerHandCardCount(player_num) > minimum_cards_at_hand;
          }
          byte_4FAA2E = 1;
        }
        if ( a2.field_0 == 8 && DiscardCard(player_num, current_card_slot_index) )
        {
          if ( am_byte_4FAA75 )
            am_byte_4FAA76 = 1;
          if ( dword_4FAA64 > 0 )
          {
            --dword_4FAA64;
            need_to_discard_card = GetPlayerHandCardCount(player_num) > minimum_cards_at_hand;
          }
          byte_4FAA2E = 1;
        }
      }
      else
      {
        if ( a2.field_0 == 7 )
        {
          if ( PlayCard(player_num, current_card_slot_index) )
          {
            byte_4FAA2E = 1;
            if ( am_byte_4FAA75 )
              am_byte_4FAA76 = 1;
          }
        }
        if ( a2.field_0 == 8 )
        {
          if ( DiscardCard(player_num, current_card_slot_index) )
          {
            byte_4FAA2E = 1;
            if ( am_byte_4FAA75 )
              am_byte_4FAA76 = 1;
          }
        }
      }
    }
    //nullsub_1();
    //if ( false )
    //{
    //  if ( !v15 )
    //  {
    //    //nullsub_1();
    //    v15 = 1;
    //  }
    //}
    //else
    //{
    //  v15 = 0;
  //  }
    //if ( false )
    //{
    //  //nullsub_1();
    //  //nullsub_1();
    // // assert(false && "Invalid strcpy params");
    // // inv_strcpy(nullptr, Dest);
    //  v5 = 0;//unk::const_0(&unk_4E19FC, 0);
    //  //nullsub_1();
    //  if ( v5 == 1 )
    //  {
    //    pAudioPlayer->StopChannels(-1, -1);
    //    v16 = 1;
    //    pArcomageGame->field_F4 = 1;
    //    dword_4FAA68 = 0;
    //  }
    //  /*v10[0] = 0;
    //  v10[2] = 640;
    //  v10[1] = 0;
    //  v10[3] = 480;*/
    //  //nullsub_1();
    //}
    if ( dword_4FABB8 != DrawCardsRectangles(player_num) )
    {
        dword_4FABB8 = DrawCardsRectangles(player_num);
        pArcomageGame->field_F6 = 1;
    }
    if ( pArcomageGame->field_F6 )
    {
      DrawGameUI(animation_stage);
      DoBlt_Copy(pArcomageGame->pBackgroundPixels);
      pArcomageGame->field_F6 = 0;
    }
    if ( pArcomageGame->field_F9 )
    {
      pTargetXY.x = 0;
      pTargetXY.y = 0;
      pSrcXYZW.left = 0;
      pSrcXYZW.right = 640;
      pSrcXYZW.top = 0;
      pSrcXYZW.bottom = 480;
      am_BeginScene(pArcomageGame->pBackgroundPixels, -1, 1);
      Blt_Copy(&pSrcXYZW, &pTargetXY, 2);
      am_EndScene();
      DrawGameUI(animation_stage);
      pRenderer->Present();
      pArcomageGame->field_F9 = 0;
    }
  }
  while ( !break_loop );
  return dword_4FAA68 > 0;
}


//----- (0040A9AF) --------------------------------------------------------
void DrawGameUI( int animation_stage )
{
  am_BeginScene(pArcomageGame->pSpritesPixels, -1, 1);
  DrawRectanglesForText();
  am_EndScene();

  am_BeginScene(pArcomageGame->pSpritesPixels, -1, 1);
  DrawCards();
  DrawPlayersTowers();
  DrawPlayersWall();
  DrawPlayersText();
  am_EndScene();

  am_BeginScene(pArcomageGame->pSpritesPixels, -1, 1);
  DrawCardAnimation(animation_stage);

  for( int i=0; i<10; ++i)
      {
      if (array_4FABD0[i].have_effect)
          array_4FABD0[i].field_40->DrawEffect();
      }
  current_card_slot_index = DrawCardsRectangles(current_player_num);
  DrawSparks();
  am_EndScene();
}

//----- (0040AA4E) --------------------------------------------------------
void DrawSparks()
    {
  int rgb_pixel_color; // [sp-4h] [bp-2Ch]@4
  unsigned int pixel_color;

  for (int i=0; i<10; ++i)
      {
        if(array_4FABD0[i].have_effect && (array_4FABD0[i].field_40->_40E2A7()==2))
            {
            rgb_pixel_color = 0x0000FF00;
            if ( !array_4FABD0[i].effect_sign )
                rgb_pixel_color =  0x000000FF;

             pixel_color = R8G8B8_to_TargetFormat(rgb_pixel_color);
             for(int j=0; j<150; ++j )
                 {
                 if (array_4FABD0[i].effect_sparks[j].have_spark > 0)
                     {
                      if (j%2)
                          DrawPixel(&array_4FABD0[i].effect_sparks[j].spark_position, pixel_color);
                      else
                          DrawSquare(&array_4FABD0[i].effect_sparks[j].spark_position, pixel_color);
                     }
                 }
            }
      }

}

//----- (0040AB0A) --------------------------------------------------------
void DrawRectanglesForText()
{ 
  RECT pSrcRect; // [sp+Ch] [bp-18h]@1
  POINT pTargetXY; // [sp+1Ch] [bp-8h]@1

//resources rectangles
  pSrcRect.left    = 765;
  pSrcRect.top     = 0;
  pSrcRect.right   = 843;
  pSrcRect.bottom  = 216;

  pTargetXY.x = 8;
  pTargetXY.y = 56;
  Blt_Copy(&pSrcRect, &pTargetXY, 2);

  pTargetXY.x = 555;
  pTargetXY.y = 56;
  Blt_Copy(&pSrcRect, &pTargetXY, 2);

  //players name rectangle
  pSrcRect.left   = 283;
  pSrcRect.top    = 166;
  pSrcRect.right  = 361;
  pSrcRect.bottom = 190;
  pTargetXY.x = 8;
  pTargetXY.y = 13;
  Blt_Chroma(&pSrcRect, &pTargetXY, pArcomageGame->field_54, 2);

  pTargetXY.x = 555;
  pTargetXY.y = 13;
  Blt_Chroma(&pSrcRect, &pTargetXY, pArcomageGame->field_54, 2);

  //tower height rectangle
  pSrcRect.left   = 234;
  pSrcRect.top    = 166;
  pSrcRect.right  = 283;
  pSrcRect.bottom = 190;
  pTargetXY.x = 100;
  pTargetXY.y = 296;
  Blt_Chroma(&pSrcRect, &pTargetXY, pArcomageGame->field_54, 2);

  pTargetXY.x = 492;
  pTargetXY.y = 296;
  Blt_Chroma(&pSrcRect, &pTargetXY, pArcomageGame->field_54, 2);

  //wall height rectangle
  pSrcRect.left   = 192; 
  pSrcRect.top    = 166;
  pSrcRect.right  = 234;
  pSrcRect.bottom = 190;
  pTargetXY.x = 168;
  pTargetXY.y = 296;
  Blt_Chroma(&pSrcRect, &pTargetXY, pArcomageGame->field_54, 2);

  pTargetXY.x = 430;
  pTargetXY.y = 296;
  Blt_Chroma(&pSrcRect, &pTargetXY, pArcomageGame->field_54, 2);
}

//----- (0040AC5F) --------------------------------------------------------
void DrawPlayersText()
    {
 
  int res_value; // ecx@18
  char text_buff[32]; // [sp+Ch] [bp-28h]@2
  POINT text_position; // [sp+2Ch] [bp-8h]@2

  if ( need_to_discard_card )
  {
    strcpy(text_buff, pGlobalTXT_LocalizationStrings[266]);// DISCARD A CARD
    text_position.x = 320 - pArcomageGame->pfntArrus->GetLineWidth(text_buff) / 2;
    text_position.y = 306;
    am_DrawText(-1, text_buff, &text_position);
  }
  strcpy( text_buff, am_Players[0].pPlayerName);
  if ( !current_player_num )
    strcat(text_buff, "***");
  text_position.x = 47 - pArcomageGame->pfntComic->GetLineWidth(text_buff) / 2;
  text_position.y = 21;
  am_DrawText(-1, text_buff, &text_position);

  strcpy(text_buff, am_Players[1].pPlayerName);
  if ( current_player_num == 1 )
    strcat(text_buff, "***" );
  text_position.x = 595 - pArcomageGame->pfntComic->GetLineWidth(text_buff) / 2;
  text_position.y = 21;
  am_DrawText(-1, text_buff, &text_position);

  am_IntToString(am_Players[0].tower_height, text_buff);
  text_position.x = 123 - pArcomageGame->pfntComic->GetLineWidth(text_buff) / 2;
  text_position.y = 305;
  am_DrawText(-1, text_buff, &text_position);

  am_IntToString(am_Players[1].tower_height, text_buff);
  text_position.x = 515 - pArcomageGame->pfntComic->GetLineWidth(text_buff) / 2;
  text_position.y = 305;
  am_DrawText(-1, text_buff, &text_position);

  am_IntToString(am_Players[0].wall_height, text_buff);
  text_position.x = 188 - pArcomageGame->pfntComic->GetLineWidth(text_buff) / 2;
  text_position.y = 305;
  am_DrawText(-1, text_buff, &text_position);

  am_IntToString(am_Players[1].wall_height, text_buff);
  text_position.x = 451 - pArcomageGame->pfntComic->GetLineWidth(text_buff) / 2;
  text_position.y = 305;
  am_DrawText(-1, text_buff, &text_position);

  res_value = am_Players[0].quarry_level;
  if ( use_start_bonus )
    res_value =am_Players[0].quarry_level + quarry_bonus;
  am_IntToString(res_value, text_buff);
  text_position.x = 14;// - 6 * 0 / 2;
  text_position.y = 92;
  DrawPlayerLevels(-1, text_buff, &text_position);

  res_value = am_Players[1].quarry_level;
  if ( use_start_bonus )
    res_value = am_Players[1].quarry_level + quarry_bonus;
  am_IntToString(res_value, text_buff);
//  v2 = 0;
  text_position.y = 92;
  text_position.x = 561; //- 6 * v2 / 2;
  DrawPlayerLevels(-1, text_buff, &text_position);

  res_value = am_Players[0].magic_level;
  if ( use_start_bonus )
    res_value = am_Players[0].magic_level + magic_bonus;    
  am_IntToString(res_value, text_buff);
 // v4 = 0;
  text_position.y = 164;
  text_position.x = 14; //- 6 * v4 / 2;
  DrawPlayerLevels(-1, text_buff, &text_position);

  res_value = am_Players[1].magic_level;
  if ( use_start_bonus )
    res_value = am_Players[1].magic_level + magic_bonus;
  am_IntToString(res_value, text_buff);
 // v6 = 0;
  text_position.y = 164;
  text_position.x = 561; //- 6 * v6 / 2;
  DrawPlayerLevels(-1, text_buff, &text_position);

  res_value = am_Players[0].zoo_level;
  if ( use_start_bonus )
    res_value = am_Players[0].zoo_level + zoo_bonus;
  am_IntToString(res_value, text_buff);
 // v8 = 0;
  text_position.y = 236;
  text_position.x = 14;// - 6 * v8 / 2;
  DrawPlayerLevels(-1, text_buff, &text_position);

  res_value = am_Players[1].zoo_level;
  if ( use_start_bonus )
    res_value =  am_Players[1].zoo_level + zoo_bonus;
  am_IntToString(res_value, text_buff);
 // v10 = 0;
  text_position.y = 236;
  text_position.x = 561;// - 6 * v10 / 2;
  DrawPlayerLevels(-1, text_buff, &text_position);

  am_IntToString(am_Players[0].resource_bricks, text_buff);
  text_position.y = 114;
  text_position.x = 10;
  DrawBricksCount(-1, text_buff, &text_position);

  am_IntToString(am_Players[1].resource_bricks, text_buff);
  text_position.x = 557;
  text_position.y = 114;
  DrawBricksCount(-1, text_buff, &text_position);

  am_IntToString(am_Players[0].resource_gems, text_buff);
  text_position.x = 10;
  text_position.y = 186;
  DrawGemsCount(-1, text_buff, &text_position);

  am_IntToString(am_Players[1].resource_gems, text_buff);
  text_position.x = 557;
  text_position.y = 186;
  DrawGemsCount(-1, text_buff, &text_position);

  am_IntToString(am_Players[0].resource_beasts, text_buff);
  text_position.x = 10;
  text_position.y = 258;
  DrawBeastsCount(-1, text_buff, &text_position);

  am_IntToString(am_Players[1].resource_beasts, text_buff);
  text_position.x = 557;
  text_position.y = 258;
  DrawBeastsCount(-1, text_buff, &text_position);
}


//----- (0040B102) --------------------------------------------------------
void DrawPlayerLevels( int a1, char *text, POINT *pXY )
    {
  char *v3; // esi@1
  unsigned char test_char; // bl@2
  int v7; // eax@3
  RECT pSrcRect;
  POINT pTargetPoint;

  v3 = text;
  am_BeginScene(pArcomageGame->pSpritesPixels, -1, 1);
  pTargetPoint.x = pXY->x;
  pTargetPoint.y = pXY->y;
  do
  {
    test_char = *v3;
    ++v3;
    if ( test_char )
    {
      v7 = 22 * test_char;
      pSrcRect.right = v7 - 842;
      pSrcRect.left = v7 - 864;
      pSrcRect.top = 190;
      pSrcRect.bottom = 207;
      Blt_Chroma(&pSrcRect, &pTargetPoint, pArcomageGame->field_54, 1);
      pTargetPoint.x += 22;
    }
  }
  while ( test_char!= 0 );
  am_EndScene();
}

//----- (0040B17E) --------------------------------------------------------
void DrawBricksCount( int a1, char* text, POINT *pXY )
    {

    char *v3; // esi@1
    unsigned char test_char; // bl@2
    int v7; // eax@3
    RECT pSrcRect;
    POINT pTargetPoint;

    v3 = text;
    am_BeginScene(pArcomageGame->pSpritesPixels, -1, 1);
    pTargetPoint.x = pXY->x;
    pTargetPoint.y = pXY->y;
    do
        {
        test_char = *v3;
        ++v3;
        if ( test_char )
            {
            v7 = 13 * test_char;
            pSrcRect.left = v7 - 370;
            pSrcRect.right = v7 - 357;
            pSrcRect.top = 128;
            pSrcRect.bottom = 138;
            Blt_Copy(&pSrcRect, &pTargetPoint, 2);
            pTargetPoint.x += 13;
            }
        }
        while ( test_char!= 0 );
        am_EndScene();

}

//----- (0040B1F3) --------------------------------------------------------
void DrawGemsCount( int a1, char* text, POINT* pXY )
    {

    char *v3; // esi@1
    unsigned char test_char; // bl@2
    int v7; // eax@3
    RECT pSrcRect;
    POINT pTargetPoint;

    v3 = text;
    am_BeginScene(pArcomageGame->pSpritesPixels, -1, 1);
    pTargetPoint.x = pXY->x;
    pTargetPoint.y = pXY->y;
    do
        {
        test_char = *v3;
        ++v3;
        if ( test_char )
            {
            v7 = 13 * test_char;
            pSrcRect.left = v7 - 370;
            pSrcRect.right = v7 - 357;
            pSrcRect.top = 138;
            pSrcRect.bottom = 148;
            Blt_Copy(&pSrcRect, &pTargetPoint, 2);
            pTargetPoint.x += 13;
            }
        }
        while ( test_char!= 0 );
        am_EndScene();
  
}

//----- (0040B268) --------------------------------------------------------
void DrawBeastsCount( int a1, char *text, POINT *pXY )
    {

    char *v3; // esi@1
    unsigned char test_char; // bl@2
    int v7; // eax@3
    RECT pSrcRect;
    POINT pTargetPoint;

    v3 = text;
    am_BeginScene(pArcomageGame->pSpritesPixels, -1, 1);
    pTargetPoint.x = pXY->x;
    pTargetPoint.y = pXY->y;
    do
        {
        test_char = *v3;
        ++v3;
        if ( test_char )
            {
            v7 = 13 * test_char;
            pSrcRect.left = v7 - 370;
            pSrcRect.right = v7 - 357;
            pSrcRect.top = 148;
            pSrcRect.bottom = 158;
            Blt_Copy(&pSrcRect, &pTargetPoint, 2);
            pTargetPoint.x += 13;
            }
        }
        while ( test_char!= 0 );
        am_EndScene();

}

//----- (0040B2DD) --------------------------------------------------------
void DrawPlayersTowers()
    {
int tower_height; // eax@1
int tower_top; // esi@3
RECT pSrcXYZW; // [sp+0h] [bp-18h]@3
POINT pTargetXY; // [sp+10h] [bp-8h]@3

tower_height= am_Players[0].tower_height;
if ( am_Players[0].tower_height > max_tower_height )
      tower_height = max_tower_height;
pSrcXYZW.top = 0;
pSrcXYZW.left = 892;
pSrcXYZW.right = 937;
tower_top = 200 * tower_height / max_tower_height;
pSrcXYZW.bottom = tower_top;
pTargetXY.x = 102;
pTargetXY.y = 297 - tower_top;
Blt_Copy(&pSrcXYZW, &pTargetXY, 2);

pSrcXYZW.top = 0;
pSrcXYZW.left = 384;
pSrcXYZW.right = 452;
pSrcXYZW.bottom = 94;
pTargetXY.y = 203 - tower_top;
pTargetXY.x = 91;
Blt_Chroma(&pSrcXYZW, &pTargetXY, pArcomageGame->field_54, 2);


tower_height = am_Players[1].tower_height;
if (am_Players[1].tower_height  > max_tower_height )
    tower_height = max_tower_height;
tower_top = 200 * tower_height / max_tower_height;
pSrcXYZW.top    = 0;
pSrcXYZW.left   = 892;
pSrcXYZW.right  = 937;
pSrcXYZW.bottom = tower_top;

pTargetXY.x = 494;
pTargetXY.y = 297 - tower_top;
Blt_Copy(&pSrcXYZW, &pTargetXY, 2);
//draw tower up cone
pSrcXYZW.left   = 384;
pSrcXYZW.right  = 452;
pSrcXYZW.top    =  94;
pSrcXYZW.bottom = 188;

pTargetXY.x = 483;
pTargetXY.y = 203 - tower_top;
Blt_Chroma(&pSrcXYZW, &pTargetXY, pArcomageGame->field_54, 2);
 
}
// 4E1884: using guessed type int dword_4E1884;

//----- (0040B400) --------------------------------------------------------
void DrawPlayersWall()
    {
int v0; // eax@1
int v1; // eax@4
LONG v2; // ecx@4
int v3; // eax@5
int v4; // eax@8
LONG v5; // esi@8
RECT pSrcXYZW; // [sp+4h] [bp-18h]@4
POINT pTargetXY; // [sp+14h] [bp-8h]@4

v0 = am_Players[0].wall_height;

if ( am_Players[0].wall_height > 100 )
    v0 = 100;

if ( am_Players[0].wall_height > 0 )
       {

pSrcXYZW.top = 0;
pSrcXYZW.left = 843;
v1 = 200 * v0 / 100;
pSrcXYZW.right = 867;
pSrcXYZW.bottom = v1;
pTargetXY.x = 177;
pTargetXY.y = 297 - v1;
Blt_Chroma(&pSrcXYZW, &pTargetXY, pArcomageGame->field_54, 2);
 }

v3 = am_Players[1].wall_height;
if ( am_Players[1].wall_height > 100 )
 v3 = 100;

    
    if ( am_Players[1].wall_height > 0 )
        {
 
pSrcXYZW.top = 0;
pSrcXYZW.left = 843;
v4 = 200 * v3 / 100;
pSrcXYZW.right = 867;
pSrcXYZW.bottom = v4;
pTargetXY.x = 439;
pTargetXY.y =  297 - v4;
Blt_Chroma(&pSrcXYZW, &pTargetXY, pArcomageGame->field_54, 2);
        }
}

//----- (0040B4B9) --------------------------------------------------------
void DrawCards()
    {

int v0; // esi@1
char v1; // bl@1
int v2; // edi@1
int v3; // edx@2
int v4; // eax@3
int v5; // ecx@3
int v6; // eax@3
unsigned int v7; // ecx@4
int v8; // eax@16
int v9; // ecx@16
int v10; // eax@16
signed int v11; // edi@18
signed int v12; // esi@20
int v13; // ecx@20
int v14; // eax@23
signed int v15; // eax@25
int v16; // ecx@25
POINT *v17; // esi@26
signed int v18; // eax@29
AcromageCardOnTable *v19; // ecx@29
int v20; // ecx@31
int v21; // [sp-4h] [bp-2Ch]@8
RECT pSrcXYZW; // [sp+Ch] [bp-1Ch]@8
POINT pTargetXY; // [sp+1Ch] [bp-Ch]@1
int v24; // [sp+24h] [bp-4h]@1

v0 = GetPlayerHandCardCount(current_player_num);
v2 = 0;
pTargetXY.y = 327;
v24 = (640 - 96 * v0) / (v0 + 1);
pTargetXY.x = (640 - 96 * v0) / (v0 + 1);
while ( v2 < v0 )
    {
    v3 = current_player_num;
    if ( am_byte_4E185D)
        {

        pTargetXY.x += am_Players[current_player_num].card_shift[v2].x ;
        pTargetXY.y += am_Players[current_player_num].card_shift[v2].y;
        }
    v7 = am_Players[current_player_num].cards_at_hand[v2];
    if ( v7 == -1 )
        {
        ++v0;
        goto LABEL_15;
        }
    if ( v2 != amuint_4FAA4C )
        {
        if ( am_Players[current_player_num].IsHisTurn == 0 && byte_505881 == 0 )
            {
            pSrcXYZW.left = 192;
            pSrcXYZW.right = 288;
            pSrcXYZW.top = 0;
            pSrcXYZW.bottom = 128;
            Blt_Copy(&pSrcXYZW, &pTargetXY, 2);
            goto LABEL_13;
            }
        pArcomageGame->GetCardRect(v7, &pSrcXYZW);
        if ( !CanCardBePlayed(current_player_num, v2) )
            {
            Blt_Copy(&pSrcXYZW, &pTargetXY, 0);
            goto LABEL_13;
            }
        Blt_Copy(&pSrcXYZW, &pTargetXY, 2);

        }
LABEL_13:
    v3 = current_player_num;
    pTargetXY.x += v24 + 96;
LABEL_15:
    if ( am_byte_4E185D )
        {
        pTargetXY.x -= am_Players[current_player_num].card_shift[v2].x ;
        pTargetXY.y -= am_Players[current_player_num].card_shift[v2].y ;
        }
    ++v2;
    }

for (v11=0; v11<10; ++v11)
    {
    if ( am_byte_4FAA76 == 0 )
        {
        if ( shown_cards[v11].uCardId != -1 )
            {
            pArcomageGame->GetCardRect(shown_cards[v11].uCardId, &pSrcXYZW);
            Blt_Copy(&pSrcXYZW, &shown_cards[v11].field_18_point, 0);
            }
        if ( shown_cards[v11].field_4 != 0 )
            {
            pTargetXY.x = shown_cards[v11].field_18_point.x + 12;
            pTargetXY.y = shown_cards[v11].field_18_point.y + 40;
            pSrcXYZW.left   = 843;
            pSrcXYZW.right  = 916;
            pSrcXYZW.top    = 200;
            pSrcXYZW.bottom = 216;
            Blt_Chroma(&pSrcXYZW, &pTargetXY, pArcomageGame->field_54, 2);
            }
        }
    else if ( amuint_4FAA34 <= 0 )
        {
        if ( v11 == 9 )
            {
            am_byte_4FAA76 = 0;
            am_byte_4FAA75 = 0;
            amuint_4FAA34 = 5;
            }
        if ( shown_cards[v11].uCardId != -1 )
            amuint_4FABC4 = shown_cards[v11].uCardId;
        shown_cards[v11].uCardId = -1;
        shown_cards[v11].field_18_point.x = shown_cards[v11].field_8.x;
        shown_cards[v11].field_18_point.y = shown_cards[v11].field_8.y;
        shown_cards[v11].field_4 = 0;
        }
    else
        {
        if ( shown_cards[v11].uCardId != -1 )
            {
            shown_cards[v11].field_18_point.x += shown_cards[v11].field_10_xplus;
            shown_cards[v11].field_18_point.y += shown_cards[v11].field_14_y_plus;
            pArcomageGame->GetCardRect(shown_cards[v11].uCardId, &pSrcXYZW);
            Blt_Copy(&pSrcXYZW, &shown_cards[v11].field_18_point, 0);
            }
        }
    }
    if ( am_byte_4FAA76 != 0 )
        --amuint_4FAA34;

    pSrcXYZW.left   = 192;
    pSrcXYZW.right  = 288;
    pSrcXYZW.top    = 0;
    pSrcXYZW.bottom = 128;
    pTargetXY.x     = 120;
    pTargetXY.y     = 18;
    Blt_Copy(&pSrcXYZW, &pTargetXY, 0);

}


//----- (0040B76F) --------------------------------------------------------
void DrawCardAnimation( int animation_stage )
    {

int v1; // eax@3
int v2; // eax@3
int v3; // esi@4
int v4; // eax@4
POINT *v5; // edx@6
RECT *v6; // ecx@6

int v8; // eax@15
AcromageCardOnTable *v9; // ecx@15
int v10; // ecx@19
int v11; // eax@20
int v12; // ecx@20
int v13; // eax@20
unsigned int v14; // ecx@21
double v15; // st7@22
POINT *v16; // edx@23
int v17; // eax@32
char v18; // zf@37
int v19; // eax@41
int v20; // eax@46
int v21; // ecx@46
int v22; // eax@46
int v23; // [sp-4h] [bp-2Ch]@28
RECT pSrcXYZW; // [sp+Ch] [bp-1Ch]@6
POINT pTargetXY; // [sp+1Ch] [bp-Ch]@20
int v26; // [sp+24h] [bp-4h]@1

v26 = animation_stage;
if ( amuint_4FAA4C != -1 )
    {
    if ( amuint_4FAA38 >= 9 )
        {
        am_uint_4FAA44_blt_xy.y = 18;
        am_uint_4FAA44_blt_xy.x = 120;
        v1 = GetPlayerHandCardCount(current_player_num);
        v2 = (640 - 96 * v1) / v1 + 96;
        if ( am_byte_4E185D )
            {
           // v3 = 188 * current_player_num + 8 * amuint_4FAA4C;
           // amuint_4FAA3C_blt_xy.x = (amuint_4FAA4C * v2 + *(am_Players[0].arr_6C[0] + v3) - 120) / 10;
            amuint_4FAA3C_blt_xy.x=(amuint_4FAA4C * v2 + am_Players[current_player_num].card_shift[amuint_4FAA4C].x-120)/10;
            v4 = (am_Players[current_player_num].card_shift[amuint_4FAA4C].y+309) /10;//(*(&am_Players[0].arr_6C[0][1] + v3) + 309) / 10;
            }
        else
            {
            amuint_4FAA3C_blt_xy.x = (amuint_4FAA4C * v2 - 120) / 10;
            v4 = 30;
            }
        am_uint_4FAA44_blt_xy.y += v4;
        am_uint_4FAA44_blt_xy.x += amuint_4FAA3C_blt_xy.x;
        
        amuint_4FAA3C_blt_xy.y = v4;
        
        pSrcXYZW.left = 192;
        pSrcXYZW.top = 0;
        pSrcXYZW.right = 288;
        pSrcXYZW.bottom = 128;
        Blt_Copy(&pSrcXYZW, &am_uint_4FAA44_blt_xy, 2);
        }
    else
        {

    
    pSrcXYZW.left  = 192;
    pSrcXYZW.top    = 0;
    pSrcXYZW.right  = 288;
    pSrcXYZW.bottom = 128;
    am_uint_4FAA44_blt_xy.x += amuint_4FAA3C_blt_xy.x;
    am_uint_4FAA44_blt_xy.y += amuint_4FAA3C_blt_xy.y;
    Blt_Copy(&pSrcXYZW, &am_uint_4FAA44_blt_xy, 2);
    if ( !amuint_4FAA38 )
        {
        
    amuint_4FAA4C = -1;
        }
        }
    }
LABEL_11:

if ( uCardID != -1 )
    {
    if ( v26 <= 10 )
        {
        if ( v26 == 10 )
            {
            pArcomageGame->GetCardRect(uCardID, &pSrcXYZW);
          //  v8 = 0;
            for (v8=0; v8<10; ++v8)
                {
                if (shown_cards[v8].uCardId==-1)
                    {
                    shown_cards[v8].uCardId = uCardID;
                    shown_cards[v8].field_4 = 1;
                    break;
                    }
                }
            /* v9 = shown_cards;
            while ( v9->uCardId != -1 )
            {
            ++v9;
            ++v8;
            if ( v9 >= &dword_4FABB8 )
            goto LABEL_20;
            }

            v10 = v8;
            shown_cards[v10].uCardId = uCardID;
            shown_cards[v10].field_4 = 1;*/
//LABEL_20:
            pTargetXY.x = shown_cards[v8].field_8.x;
            pTargetXY.y = shown_cards[v8].field_8.y;
            Blt_Copy(&pSrcXYZW, &pTargetXY, 0);
            uCardID = -1;
            }
        }
    else
        {
        pArcomageGame->GetCardRect(uCardID, &pSrcXYZW);
        amuint_4FAA5C_blt_xy.x += amuint_4FAA54_blt_xy.x;
        amuint_4FAA5C_blt_xy.y += amuint_4FAA54_blt_xy.y;
        Blt_Copy(&pSrcXYZW, &amuint_4FAA5C_blt_xy, 0);
        }
    }

if ( played_card_id != -1 )
    {
    v15 = v26;
    if ( v15 > 15.0 )
        {
        pArcomageGame->GetCardRect(played_card_id, &pSrcXYZW);
        amuint_4FAA5C_blt_xy.x += amuint_4FAA54_blt_xy.x;
        amuint_4FAA5C_blt_xy.y += amuint_4FAA54_blt_xy.y;
        Blt_Copy(&pSrcXYZW, &amuint_4FAA5C_blt_xy, 2);
        return;
        }
    if ( v15 == 15.0 )
        {
        ApplyCardToPlayer(current_player_num, played_card_id);
        pArcomageGame->GetCardRect(played_card_id, &pSrcXYZW);
        pTargetXY.x = 272;
        pTargetXY.y = 173;
        Blt_Copy(&pSrcXYZW, &pTargetXY, 2);
        return;
        }
    if ( v15 == 5.0 )
        {
        amuint_4FAA5C_blt_xy.x = 272;
        amuint_4FAA5C_blt_xy.y = 173;
        for (v17=0; v17<10; ++v17)
            {
            if (shown_cards[v17].uCardId==-1)
                break;
            }
        
            amuint_4FAA54_blt_xy.x = (shown_cards[v17].field_8.x - 272) / 5;
            amuint_4FAA54_blt_xy.y = (shown_cards[v17].field_8.y - 173) / 5;
            pArcomageGame->GetCardRect(played_card_id, &pSrcXYZW);
            pTargetXY.x = 272;
            pTargetXY.y = 173;
            Blt_Copy(&pSrcXYZW, &pTargetXY, 2);
            return;
        }
    if ( v15 >= 5.0 )
        {
        v18 = v26 == 0;
        }
    else
        {
        v18 = v26 == 0;
        if ( v26 > 0 )
            {
            pArcomageGame->GetCardRect(played_card_id, &pSrcXYZW);
            amuint_4FAA5C_blt_xy.x += amuint_4FAA54_blt_xy.x;
            amuint_4FAA5C_blt_xy.y += amuint_4FAA54_blt_xy.y;
            Blt_Copy(&pSrcXYZW, &amuint_4FAA5C_blt_xy, 0);
            return;
            }
        }
    if ( !v18 )
        {

        pArcomageGame->GetCardRect(played_card_id, &pSrcXYZW);
        pTargetXY.x = 272;
        pTargetXY.y = 173;
        Blt_Copy(&pSrcXYZW, &pTargetXY, 2);
        return;
        }

    pArcomageGame->GetCardRect(played_card_id, &pSrcXYZW);
    for (v19=0; v19<10; ++v19)
        {
        if (shown_cards[v19].uCardId==-1)
            {
            shown_cards[v19].uCardId = played_card_id;
            break;
            }       
        }
        pTargetXY.x = shown_cards[v19].field_8.x;
    pTargetXY.y = shown_cards[v19].field_8.y;
    Blt_Copy(&pSrcXYZW, &pTargetXY, 0);
    played_card_id = -1;
    }
}

//----- (0040BB12) --------------------------------------------------------
void ArcomageGame::GetCardRect(unsigned int uCardID, RECT *pCardRect)
{
  int v3; // edx@1
  int v4; // ecx@1

  v3 = pCards[uCardID].slot % 10;
  v4 = (pCards[uCardID].slot / 10 << 7) + 220;
  pCardRect->top = v4;
  pCardRect->left = 96 * v3;
  pCardRect->bottom = v4 + 128;
  pCardRect->right = 96 * v3 + 96;
}

//----- (0040BB49) --------------------------------------------------------
int GetPlayerHandCardCount( int player_num )
    {
  int card_count; // eax@1

  card_count = 0;
  for(int i=0; i<10; ++i)
      {
      if (am_Players[player_num].cards_at_hand[i]!=-1)
            ++card_count;
      }
  return card_count;
  
}

//----- (0040BB67) --------------------------------------------------------
signed int DrawCardsRectangles( int player_num )
{
//int v1; // esi@1
signed int result; // eax@1
int v3; // ebx@1
char v4; // zf@1
int v5; // eax@3
int v6; // eax@4
int v7; // ecx@6
char *v8; // eax@6
int v9; // eax@10
int v10; // ecx@10
int v11; // eax@10
char v12; // ch@2@11
int v13; // ST00_4@12
unsigned __int16 v14; // ax@12
int v15; // eax@13
int v16; // ecx@13
int v17; // eax@13
char v18; // al@16
char v19; // ch@2@16
int v20; // ST00_4@19
unsigned __int16 v21; // ax@19
RECT pXYZW; // [sp+Ch] [bp-3Ch]@3
stru273 v26; // [sp+1Ch] [bp-2Ch]@2
int v25; // [sp+2Ch] [bp-1Ch]@3
__int32 var18; // [sp+30h] [bp-18h]@3
int i; // [sp+34h] [bp-14h]@4
int v28; // [sp+38h] [bp-10h]@1
int hand_index; // [sp+3Ch] [bp-Ch]@3
int v30; // [sp+40h] [bp-8h]@1
char v31; // [sp+44h] [bp-4h]@12
char v32; // [sp+45h] [bp-3h]@12
char v33; // [sp+46h] [bp-2h]@12

__debugbreak(); // need do fix rectangle not fit to card

v30 = 0;
result = -1;
//v3 = 188 * a1;

//v4 = LOBYTE(am_Players[a1].field_20) == 0;
v28 = -1;
if ( am_Players[player_num].IsHisTurn )
    {
    if ( v26._40DD2F() )
        {
        v5 = GetPlayerHandCardCount(player_num);
        hand_index = 0;
        v25 = v5;
        pXYZW.top = 327;
        pXYZW.bottom = 455;
        pXYZW.left = (640 - 96 * v5) / (v5 + 1);
        var18 = pXYZW.left + 96;
        pXYZW.right = pXYZW.left + 96;
        if ( v5 > 0 )
            {
           // v6 = 47 * v1;
           // for ( i = player_num; ; v6 = i )
            for(;;)
                {
                v7 = v30;
              //  v8 = &am_Players[0].cards_at_hand[v30 + v6];

                for (v7=0; v7<10; ++v7)
                    {
                    if (am_Players[player_num].cards_at_hand[v7]!= -1 )
                        break;
                    }
                //if ( *v8 )
                //    {
                //    do
                //        {
                //        //v8 += 4;
                //        ++v7;
                //        }
                //        while ( *v8 == -1 );
                        v30 = v7;
                  //  }
                if ( am_byte_4E185D )
                    {
                 //   v9 = v3 + 8 * v7;
                    v10 = am_Players[player_num].card_shift[v7].x;//*(am_player[0].arr_6C[0] + v9);
                    v11 = am_Players[player_num].card_shift[v7].y;//*(&am_player[0].arr_6C[0][1] + v9);
                    pXYZW.left += v10;
                    pXYZW.right += v10;
                    pXYZW.top += v11;
                    pXYZW.bottom += v11;
                    }
                if (v26.Inside(&pXYZW) )
                    break;
           
                v13=0;
                v14 = R8G8B8_to_TargetFormat(v13);
                DrawRect(&pXYZW, v14, 0);
                pXYZW.left += var18;
                pXYZW.right += var18;
                if ( am_byte_4E185D )
                    {
                    v16 = am_Players[player_num].card_shift[v30].x;//*(am_player[0].arr_6C[0] + v15);
                    v17 = am_Players[player_num].card_shift[v30].y;//*(&am_player[0].arr_6C[0][1] + v15);
                    pXYZW.left -= v16;
                    pXYZW.right -= v16;
                    pXYZW.top -= v17;
                    pXYZW.bottom -= v17;
                    }
                ++v30;
                ++hand_index;
                if ( hand_index >= v25 )
                    return v28;
                }
            v28 = hand_index;
            v18 = CanCardBePlayed(player_num, hand_index);
            if ( v18 )
                {
                 v20=0x00FFFFFF;
                }
            else
                {
                v20=0x000000FF;
                }
           
            v21 = R8G8B8_to_TargetFormat(v20);
            DrawRect(&pXYZW, v21, 0);
            }
        }
    result = v28;
    }
return result;


}
// 4E185D: using guessed type char am_byte_4E185D;

//----- (0040BCFB) --------------------------------------------------------
bool DiscardCard( int player_num, signed int card_slot_index )
    {
  int v2; // esi@2
  signed int v3; // edi@2
  char *v4; // eax@2
  int v5; // ecx@7
  int v6; // ebx@7
  int v7; // esi@8
  int v8; // eax@8
  int v9; // esi@8
  int v10; // ecx@8
  int v11; // eax@8
  int v12; // eax@8
  signed int v13; // edx@9
  unsigned int v14; // eax@12
  char result; // al@12
  int i;

  if ( card_slot_index <= -1 )
    return false;
  v2 = 0;

  for(i=0; i<10; ++i)
  {
    if ( am_Players[player_num].cards_at_hand[i] != -1 )
    {
      if ( card_slot_index == v2 )
        break;
      ++v2;
    }
  }

  if ( pCards[am_Players[player_num].cards_at_hand[i]].can_be_discarded) 
  {
    ArcomageGame::PlaySound(22);
    v8 = GetPlayerHandCardCount(current_player_num);
    v10=am_Players[player_num].card_shift[i].x + (640 - 96 * v8) / (v8 + 1);
    amuint_4FAA5C_blt_xy.x = v10;
    amuint_4FAA5C_blt_xy.y = am_Players[player_num].card_shift[i].y + 327;//v11;
    v12 = 0;

    if ( !am_byte_4FAA75 )
    {
      for (v12=0; v12<10; ++v12)
          {
           if (shown_cards[v12].uCardId== -1)
               break;
          }
    }
    pArcomageGame->field_F6 = 1;
    amuint_4FAA54_blt_xy.x = (shown_cards[v12].field_8.x - v10) / 10;
    amuint_4FAA54_blt_xy.y = (shown_cards[v12].field_8.y - 327) / 10;
    uCardID = am_Players[player_num].cards_at_hand[i];
    am_Players[player_num].cards_at_hand[i] = -1;
    need_to_discard_card = 0;
    return true;
  }
  else
    return false;
}


//----- (0040BE0E) --------------------------------------------------------
bool PlayCard( int player_num, int card_slot_num )
{
  int v3; // ebp@1
  int v4; // ecx@2
  ArcomagePlayer *v5; // esi@2
  int card_index; // edi@2
  int v7; // eax@2
  int v8; // ebx@8
  int cards_at_hand; // eax@8
  int v10; // ebx@8
  int v11; // ebp@8
  int v12; // ecx@8
  int v13; // eax@8
  ArcomageCard *pCard; // eax@8
  int v15; // ecx@8
  int v16; // ecx@8

  if ( card_slot_num <= -1 )
    return false;

  v4 = 0;
  for (card_index=0; card_index<10; ++card_index)
  {
    if ( am_Players[player_num].cards_at_hand[card_index] != -1 )
    {
      if ( card_slot_num == v4 )
        break;
      ++v4;
    }
  }

  if (CanCardBePlayed(player_num, card_index) )
  {
    ArcomageGame::PlaySound(23);
    cards_at_hand = GetPlayerHandCardCount(current_player_num);
    pArcomageGame->field_F6 = 1;
    v12 =  am_Players[player_num].card_shift[card_index].x + 
         (640 - 96 * cards_at_hand) / (cards_at_hand + 1)+ 
         96 * card_index ;
        
  //  v13 = *(int *)((char *)&am_Players[0].arr_6C[0][1] + v10) + 327;
    amuint_4FAA5C_blt_xy.x = v12;//v12;
    amuint_4FAA5C_blt_xy.y = am_Players[player_num].card_shift[card_index].y + 327;//v13;

    amuint_4FAA54_blt_xy.x = (272 - v12) / 5;
    amuint_4FAA54_blt_xy.y = -30;

    pCard = &pCards[am_Players[player_num].cards_at_hand[card_index]];
    am_Players[player_num].resource_bricks -= pCard->needed_bricks;
    am_Players[player_num].resource_beasts -= pCard->needed_beasts;
    am_Players[player_num].resource_gems   -= pCard->needed_gems;
    played_card_id = am_Players[player_num].cards_at_hand[card_index];
    am_Players[player_num].cards_at_hand[card_index] = -1;
    return true;
  }
  else
   return false;

}

//----- (0040BF15) --------------------------------------------------------
bool CanCardBePlayed(int player_num, int hand_card_indx)
{
  bool result; // eax@1
  ArcomageCard *test_card; // ecx@1
  ArcomagePlayer *pPlayer; // esi@1

  pPlayer = &am_Players[player_num];
  result = true;
  test_card = &pCards[am_Players[player_num].cards_at_hand[hand_card_indx]];
  if ( test_card->needed_quarry_level > pPlayer->quarry_level )
    result = false;
  if ( test_card->needed_magic_level > pPlayer->magic_level )
    result = false;
  if ( test_card->needed_zoo_level > pPlayer->zoo_level )
    result = false;
  if ( test_card->needed_bricks > pPlayer->resource_bricks )
    result = false;
  if ( test_card->needed_gems > pPlayer->resource_gems )
    result = false;
  if ( test_card->needed_beasts > pPlayer->resource_beasts )
    result = false;
  return result;
}




//----- (0040BF77) --------------------------------------------------------
void ApplyCardToPlayer( int player_num, unsigned int uCardID )
    {

  
#define APPLY_TO_PLAYER( PLAYER, ENEMY,FIELD, VAL, RES )\
   if (VAL != 0) {\
        if (VAL == 99) {\
          if ( PLAYER->##FIELD < ENEMY->##FIELD ) {\
            PLAYER->##FIELD = ENEMY->##FIELD;\
            RES = ENEMY->##FIELD - PLAYER->##FIELD;\
          }\
        } else {\
        PLAYER->##FIELD += (signed int)(VAL);\
        if ( PLAYER->##FIELD<0 ) PLAYER->##FIELD= 0;\
        RES = (signed int)(VAL);\
        }\
   }

#define APPLY_TO_ENEMY( PLAYER, ENEMY,FIELD, VAL, RES ) APPLY_TO_PLAYER(ENEMY, PLAYER, FIELD, VAL, RES)

#define APPLY_TO_BOTH( PLAYER, ENEMY, FIELD, VAL, RES_P, RES_E ) \
       if ( VAL != 0) {\
          if ( VAL == 99 )  { \
            if ( PLAYER->##FIELD != ENEMY->##FIELD )  { \
                if ( PLAYER->##FIELD <= ENEMY->##FIELD )  { \
                    PLAYER->##FIELD = ENEMY->##FIELD; \
                    RES_P = ENEMY->##FIELD - PLAYER->##FIELD; \
                }  else { \
                    ENEMY->##FIELD = PLAYER->##FIELD; \
                    RES_E = PLAYER->##FIELD - ENEMY->##FIELD; \
                    } \
                }\
            }  else { \
            PLAYER->##FIELD += (signed int)(VAL);\
            ENEMY->##FIELD  += (signed int)(VAL); if (PLAYER->##FIELD < 0 ) {PLAYER->##FIELD = 0;} \
            if ( ENEMY->##FIELD < 0 ) {ENEMY->##FIELD = 0;} \
            RES_P = (signed int)(VAL);  RES_E = (signed int)(VAL); \
            }\
       }
        ArcomagePlayer *player; // esi@1
        int v3; // eax@1
        ArcomagePlayer *enemy; // edi@1
        int v5; // eax@2
        char v6; // sf@2
        unsigned __int8 v7; // of@2
        int v8; // eax@3
        int v9; // eax@4
        char v10; // zf@5
        int v11; // eax@8
        unsigned __int8 v12; // zf@8
        char v13; // sf@8
        unsigned __int8 v14; // of@8
        int v15; // eax@9
        int v16; // eax@10
        char v17; // zf@12
        char v18; // al@15
        int v19; // eax@16
        int v20; // eax@17
        int v21; // eax@22
        int v22; // eax@23
        int v23; // eax@26
        signed int v24; // ebx@26
        int v103;
        int v104;

        POINT v184; // [sp+Ch] [bp-64h]@488
        int enemy_num; // [sp+14h] [bp-5Ch]@1
        ArcomageCard *pCard; // [sp+18h] [bp-58h]@1
        int buildings_e; // [sp+1Ch] [bp-54h]@1
        int buildings_p; // [sp+20h] [bp-50h]@1
        int quarry_p; // [sp+28h] [bp-48h]@1
        int dmg_e; // [sp+2Ch] [bp-44h]@1
        int dmg_p; // [sp+30h] [bp-40h]@1
        int bricks_p; // [sp+34h] [bp-3Ch]@1
        int tower_e; // [sp+38h] [bp-38h]@1
        int tower_p; // [sp+3Ch] [bp-34h]@1
        int wall_e; // [sp+40h] [bp-30h]@1
        int wall_p; // [sp+44h] [bp-2Ch]@1
        int beasts_e; // [sp+48h] [bp-28h]@1
        int beasts_p; // [sp+4Ch] [bp-24h]@1
        int gems_e; // [sp+50h] [bp-20h]@1
        int gems_p; // [sp+54h] [bp-1Ch]@1
        int bricks_e; // [sp+58h] [bp-18h]@1
        int zoo_e; // [sp+5Ch] [bp-14h]@1
        int zoo_p; // [sp+60h] [bp-10h]@1
        int magic_e; // [sp+64h] [bp-Ch]@1
        int magic_p; // [sp+68h] [bp-8h]@1
        int quarry_e; // [sp+6Ch] [bp-4h]@1

        quarry_p  = 0;
        magic_p   = 0;
        zoo_p     = 0;
        bricks_p  = 0;
        gems_p    = 0;
        beasts_p  = 0;
        wall_p    = 0;
        tower_p   = 0;
        buildings_p = 0;
        dmg_p     = 0;
        quarry_e  = 0;
        magic_e   = 0;
        zoo_e     = 0;
        bricks_e  = 0;
        gems_e    = 0;
        beasts_e  = 0;
        wall_e    = 0;
        tower_e   = 0;
        buildings_e = 0;
        dmg_e      = 0;

        player = &am_Players[player_num];
        pCard = &pCards[uCardID];
        enemy_num = (player_num + 1) % 2;
        enemy = &am_Players[enemy_num];
        switch ( pCard->compare_param )
            {
        case 2:
            v5 = player->quarry_level;
            v7 = __OFSUB__(v5, enemy->quarry_level);
            v6 = v5 - enemy->quarry_level < 0;
            v18 = v6 ^ v7;
            goto LABEL_25;
        case 3:
            v8 = player->magic_level;
            v7 = __OFSUB__(v8, enemy->magic_level);
            v6 = v8 - enemy->magic_level < 0;
            v18 = v6 ^ v7;
            goto LABEL_25;
        case 4:
            v9 = player->zoo_level;
            v7 = __OFSUB__(v9, enemy->zoo_level);
            v6 = v9 - enemy->zoo_level < 0;
            v18 = v6 ^ v7;
            goto LABEL_25;
        case 5:
            v10 = player->quarry_level == enemy->quarry_level;
            v18 = v10;
            goto LABEL_25;
        case 6:
            v10 = player->magic_level == enemy->magic_level;
            v18 = v10;
            goto LABEL_25;
        case 7:
            v10 = player->zoo_level == enemy->zoo_level;
            v18 = v10;
            goto LABEL_25;
        case 8:
            v11 = player->quarry_level;
            v14 = __OFSUB__(v11, enemy->quarry_level);
            v12 = v11 == enemy->quarry_level;
            v13 = v11 - enemy->quarry_level < 0;
            v18 = !((v13 ^ v14) | v12);
            goto LABEL_25;
        case 9:
            v15 = player->magic_level;
            v14 = __OFSUB__(v15, enemy->magic_level);
            v12 = v15 == enemy->magic_level;
            v13 = v15 - enemy->magic_level < 0;
            v18 = !((v13 ^ v14) | v12);
            goto LABEL_25;
        case 10:
            v16 = player->zoo_level;
            v14 = __OFSUB__(v16, enemy->zoo_level);
            v12 = v16 == enemy->zoo_level;
            v13 = v16 - enemy->zoo_level < 0;
            v18 = !((v13 ^ v14) | v12);
            goto LABEL_25;
        case 11:
            v10 = player->wall_height == 0;
            v18 = v10;
            goto LABEL_25;
        case 12:
            v17 = player->wall_height == 0;
            v18 = !v17;
            goto LABEL_25;
        case 13:
            v10 = enemy->wall_height == 0;
            v18 = v10;
            goto LABEL_25;
        case 14:
            v17 = enemy->wall_height == 0;
            v18 = !v17;
            goto LABEL_25;
        case 15:
            v19 = player->wall_height;
            v7 = __OFSUB__(v19, enemy->wall_height);
            v6 = v19 - enemy->wall_height < 0;
            v18 = v6 ^ v7;
            goto LABEL_25;
        case 16:
            v20 = player->tower_height;
            v7 = __OFSUB__(v20, enemy->tower_height);
            v6 = v20 - enemy->tower_height < 0;
            v18 = v6 ^ v7;
            goto LABEL_25;
        case 17:
            v10 = player->wall_height == enemy->wall_height;
            v18 = v10;
            goto LABEL_25;
        case 18:
            v10 = player->tower_height == enemy->tower_height;
            v18 = v10;
            goto LABEL_25;
        case 19:
            v21 = player->wall_height;
            v14 = __OFSUB__(v21, enemy->wall_height);
            v12 = v21 == enemy->wall_height;
            v13 = v21 - enemy->wall_height < 0;
            v18 = !((v13 ^ v14) | v12);
            goto LABEL_25;
        case 20:
            v22 = player->tower_height;
            v14 = __OFSUB__(v22, enemy->tower_height);
            v12 = v22 == enemy->tower_height;
            v13 = v22 - enemy->tower_height < 0;
            v18 = !((v13 ^ v14) | v12);
LABEL_25:
            if ( v18 )
                goto LABEL_26;
            goto LABEL_231;
        default:
LABEL_26:
            v23 = pCard->draw_extra_card_count;
            v24 = 0;
            dword_4FAA68 = v23 + (pCard->field_30 == 1);
            dword_4FAA64 = v23;
            if ( v23 > 0 )
                {
                do
                    {
                    GetNextCardFromDeck(player_num);
                    ++v24;
                    }
                    while ( v24 < pCard->draw_extra_card_count );
                }

            need_to_discard_card = GetPlayerHandCardCount(player_num) > minimum_cards_at_hand;


            APPLY_TO_PLAYER(player, enemy, quarry_level,    pCard->to_player_quarry_lvl, quarry_p);
            APPLY_TO_PLAYER(player, enemy, magic_level,     pCard->to_player_magic_lvl,  magic_p);
            APPLY_TO_PLAYER(player, enemy, zoo_level,       pCard->to_player_zoo_lvl,    zoo_p);
            APPLY_TO_PLAYER(player, enemy, resource_bricks, pCard->to_player_bricks,     bricks_p);
            APPLY_TO_PLAYER(player, enemy, resource_gems,   pCard->to_player_gems,       gems_p);
            APPLY_TO_PLAYER(player, enemy, resource_beasts, pCard->to_player_beasts,     beasts_p);
            if ( pCard->to_player_buildings )
                {
                dmg_p = ApplyDamageToBuildings(player_num, (signed int)pCard->to_player_buildings);
                buildings_p = (signed int)pCard->to_player_buildings - dmg_p;
                }
           APPLY_TO_PLAYER(player, enemy, wall_height,  pCard->to_player_wall, wall_p);
           APPLY_TO_PLAYER(player, enemy, tower_height, pCard->to_player_tower, tower_p);

           APPLY_TO_ENEMY(player, enemy, quarry_level,    pCard->to_enemy_quarry_lvl, quarry_e);
           APPLY_TO_ENEMY(player, enemy, magic_level,     pCard->to_enemy_magic_lvl,  magic_e);
           APPLY_TO_ENEMY(player, enemy, zoo_level,       pCard->to_enemy_zoo_lvl,    zoo_e);
           APPLY_TO_ENEMY(player, enemy, resource_bricks, pCard->to_enemy_bricks,     bricks_e);
           APPLY_TO_ENEMY(player, enemy, resource_gems,   pCard->to_enemy_gems,       gems_e);
           APPLY_TO_ENEMY(player, enemy, resource_beasts, pCard->to_enemy_beasts,     beasts_e);
            if ( pCard->to_enemy_buildings )
                {
                dmg_e = ApplyDamageToBuildings(enemy_num, (signed int)pCard->to_enemy_buildings);
                buildings_e = (signed int)pCard->to_enemy_buildings - dmg_e;
                }
            APPLY_TO_ENEMY(player, enemy, wall_height,  pCard->to_enemy_wall,  wall_e);
            APPLY_TO_ENEMY(player, enemy, tower_height, pCard->to_enemy_tower, tower_e);
            
            APPLY_TO_BOTH(player, enemy, quarry_level,    pCard->to_pl_enm_quarry_lvl, quarry_p, quarry_e);
            APPLY_TO_BOTH(player, enemy, magic_level,     pCard->to_pl_enm_magic_lvl,  magic_p,  magic_e);
            APPLY_TO_BOTH(player, enemy, zoo_level,       pCard->to_pl_enm_zoo_lvl,    zoo_p,    zoo_e);
            APPLY_TO_BOTH(player, enemy, resource_bricks, pCard->to_pl_enm_bricks,     bricks_p, bricks_e);
            APPLY_TO_BOTH(player, enemy, resource_gems,   pCard->to_pl_enm_gems,       gems_p,   gems_e);
            APPLY_TO_BOTH(player, enemy, resource_beasts, pCard->to_pl_enm_beasts,     beasts_p, beasts_e);
            if ( pCard->to_pl_enm_buildings )
                {
                dmg_p = ApplyDamageToBuildings(player_num, (signed int)pCard->to_pl_enm_buildings);
                dmg_e = ApplyDamageToBuildings(enemy_num,  (signed int)pCard->to_pl_enm_buildings);
                buildings_p = (signed int)pCard->to_pl_enm_buildings - dmg_p;
                buildings_e = (signed int)pCard->to_pl_enm_buildings - dmg_e;
                }
            APPLY_TO_BOTH(player, enemy, wall_height, pCard->to_pl_enm_wall, wall_p, wall_e);
            APPLY_TO_BOTH(player, enemy, tower_height, pCard->to_pl_enm_tower, tower_p, tower_e);
            break;
        case 0:
LABEL_231:
            v103 = pCard->can_draw_extra_card2;
            v104 = 0;
            dword_4FAA68 = v103 + (pCard->field_4D == 1);
            dword_4FAA64 = v103;
            if ( v103 > 0 )
                {
                do
                    {
                    GetNextCardFromDeck(player_num);
                    ++v104;
                    }
                    while ( v104 < pCard->can_draw_extra_card2 );
                }

            need_to_discard_card = GetPlayerHandCardCount(player_num) > minimum_cards_at_hand;

            APPLY_TO_PLAYER(player, enemy, quarry_level,    pCard->to_player_quarry_lvl2, quarry_p);
            APPLY_TO_PLAYER(player, enemy, magic_level,     pCard->to_player_magic_lvl2,  magic_p);
            APPLY_TO_PLAYER(player, enemy, zoo_level,       pCard->to_player_zoo_lvl2,    zoo_p);
            APPLY_TO_PLAYER(player, enemy, resource_bricks, pCard->to_player_bricks2,     bricks_p);
            APPLY_TO_PLAYER(player, enemy, resource_gems,   pCard->to_player_gems2,       gems_p);
            APPLY_TO_PLAYER(player, enemy, resource_beasts, pCard->to_player_beasts2,     beasts_p);
            if ( pCard->to_player_buildings2 )
                {
                dmg_p = ApplyDamageToBuildings(player_num, (signed int)pCard->to_player_buildings2);
                buildings_p = (signed int)pCard->to_player_buildings2 - dmg_p;
                }
            APPLY_TO_PLAYER(player, enemy, wall_height,  pCard->to_player_wall2,  wall_p);
            APPLY_TO_PLAYER(player, enemy, tower_height, pCard->to_player_tower2, tower_p);

            APPLY_TO_ENEMY(player, enemy, quarry_level,    pCard->to_enemy_quarry_lvl2, quarry_e);
            APPLY_TO_ENEMY(player, enemy, magic_level,     pCard->to_enemy_magic_lvl2,  magic_e);
            APPLY_TO_ENEMY(player, enemy, zoo_level,       pCard->to_enemy_zoo_lvl2,    zoo_e);
            APPLY_TO_ENEMY(player, enemy, resource_bricks, pCard->to_enemy_bricks2,     bricks_e);
            APPLY_TO_ENEMY(player, enemy, resource_gems,   pCard->to_enemy_gems2,       gems_e);
            APPLY_TO_ENEMY(player, enemy, resource_beasts, pCard->to_enemy_beasts2,     beasts_e);
            if ( pCard->to_enemy_buildings2 )
                {
                dmg_e = ApplyDamageToBuildings(enemy_num, (signed int)pCard->to_enemy_buildings2);
                buildings_e = (signed int)pCard->to_enemy_buildings2 - dmg_e;
                }
            APPLY_TO_ENEMY(player, enemy, wall_height,  pCard->to_enemy_wall2,  wall_e);
            APPLY_TO_ENEMY(player, enemy, tower_height, pCard->to_enemy_tower2, tower_e);

            APPLY_TO_BOTH(player, enemy, quarry_level,    pCard->to_pl_enm_quarry_lvl2, quarry_p, quarry_e);
            APPLY_TO_BOTH(player, enemy, magic_level,     pCard->to_pl_enm_magic_lvl2,  magic_p,  magic_e);
            APPLY_TO_BOTH(player, enemy, zoo_level,       pCard->to_pl_enm_zoo_lvl2,    zoo_p,    zoo_e);
            APPLY_TO_BOTH(player, enemy, resource_bricks, pCard->to_pl_enm_bricks2,     bricks_p, bricks_e);
            APPLY_TO_BOTH(player, enemy, resource_gems,   pCard->to_pl_enm_gems2,       gems_p,   gems_e);
            APPLY_TO_BOTH(player, enemy, resource_beasts, pCard->to_pl_enm_beasts2,     beasts_p, beasts_e);

            if ( pCard->to_pl_enm_buildings2 )
                {
                dmg_p = ApplyDamageToBuildings(player_num, (signed int)pCard->to_pl_enm_buildings2);
                dmg_e = ApplyDamageToBuildings(enemy_num,  (signed int)pCard->to_pl_enm_buildings2);
                buildings_p = (signed int)pCard->to_pl_enm_buildings2 - dmg_p;
                buildings_e = (signed int)pCard->to_pl_enm_buildings2 - dmg_e;
                }
            APPLY_TO_BOTH(player, enemy, wall_height, pCard->to_pl_enm_wall2, wall_p, wall_e);
            APPLY_TO_BOTH(player, enemy, tower_height, pCard->to_pl_enm_tower2, tower_p, tower_e);    
            break;
            }
          //  }
        if ( quarry_p > 0 || quarry_e > 0 )
            pArcomageGame->PlaySound(30);
        if ( quarry_p < 0 || quarry_e < 0 )
            pArcomageGame->PlaySound(31);
        if ( magic_p > 0 || magic_e > 0 )
            pArcomageGame->PlaySound(33);
        if ( magic_p < 0 || magic_e < 0 )
            pArcomageGame->PlaySound(34);
        if ( zoo_p > 0 || zoo_e > 0 )
            pArcomageGame->PlaySound(36);
        if ( zoo_p < 0 || zoo_e < 0 )
            pArcomageGame->PlaySound(37);
        if ( bricks_p > 0 || bricks_e > 0 )
            pArcomageGame->PlaySound(39);
        if ( bricks_p < 0 || bricks_e < 0 )
            pArcomageGame->PlaySound(40);
        if ( gems_p > 0 || gems_e > 0 )
            pArcomageGame->PlaySound(42);
        if ( gems_p < 0 || gems_e < 0 )
            pArcomageGame->PlaySound(43);
        if ( beasts_p > 0 || beasts_e > 0 )
            pArcomageGame->PlaySound(45u);
        if ( beasts_p < 0 || beasts_e < 0 )
            pArcomageGame->PlaySound(46);
        if ( buildings_p || buildings_e || dmg_p || dmg_e )
            pArcomageGame->PlaySound(48);
        if ( wall_p > 0 || wall_e > 0 )
            pArcomageGame->PlaySound(49);
        if ( wall_p < 0 || wall_e < 0 )
            pArcomageGame->PlaySound(50);
        if ( tower_p > 0 || tower_e > 0 )
            pArcomageGame->PlaySound(52);
        if ( tower_p < 0 || tower_e < 0 )
            pArcomageGame->PlaySound(53);
        if ( player_num )
            {
            if ( quarry_p )
                {
                v184.x = 573;
                v184.y = 92;
                am_40D2B4(&v184, quarry_p);
                }
            if ( quarry_e )
                {
                v184.x = 26;
                v184.y = 92;
                am_40D2B4(&v184, quarry_e);
                }
            if ( magic_p )
                {
                v184.x = 573;
                v184.y = 164;
                am_40D2B4(&v184, magic_p);
                }
            if ( magic_e )
                {
                v184.x = 26;
                v184.y = 164;
                am_40D2B4(&v184, magic_e);
                }
            if ( zoo_p )
                {
                v184.x = 573;
                v184.y = 236;
                am_40D2B4(&v184, zoo_p);
                }
            if ( zoo_e )
                {
                v184.x = 26;
                v184.y = 236;
                am_40D2B4(&v184, zoo_e);
                }
            if ( bricks_p )
                {
                v184.x = 563;
                v184.y = 114;
                am_40D2B4(&v184, bricks_p);
                }
            if ( bricks_e )
                {
                v184.x = 16;
                v184.y = 114;
                am_40D2B4(&v184, bricks_e);
                }
            if ( gems_p )
                {
                v184.x = 563;
                v184.y = 186;
                am_40D2B4(&v184, gems_p);
                }
            if ( gems_e )
                {
                v184.x = 16;
                v184.y = 186;
                am_40D2B4(&v184, gems_e);
                }
            if ( beasts_p )
                {
                v184.x = 563;
                v184.y = 258;
                am_40D2B4(&v184, beasts_p);
                }
            if ( beasts_e )
                {
                v184.x = 16;
                v184.y = 258;
                am_40D2B4(&v184, beasts_e);
                }
            if ( wall_p )
                {
                v184.x = 442;
                v184.y = 296;
                am_40D2B4(&v184, wall_p);
                }
            if ( wall_e )
                {
                v184.x = 180;
                v184.y = 296;
                am_40D2B4(&v184, wall_e);
                }
            if ( tower_p )
                {
                v184.x = 514;
                v184.y = 296;
                am_40D2B4(&v184, tower_p);
                }
            if ( tower_e )
                {
                v184.x = 122;
                v184.y = 296;
                am_40D2B4(&v184, tower_e);
                }
            if ( dmg_p )
                {
                v184.x = 442;
                v184.y = 296;
                am_40D2B4(&v184, dmg_p);
                }
            if ( buildings_p )
                {
                v184.x = 514;
                v184.y = 296;
                am_40D2B4(&v184, buildings_p);
                }
            if ( dmg_e )
                {
                v184.x = 180;
                v184.y = 296;
                am_40D2B4(&v184, dmg_e);
                }
            if ( buildings_e )
                {
                v184.x = 122;
                v184.y = 296;
                am_40D2B4(&v184, buildings_e);
                }
            }
        else
            {
            if ( quarry_p )
                {
                v184.x = 26;
                v184.y = 92;
                am_40D2B4(&v184, quarry_p);
                }
            if ( quarry_e )
                {
                v184.x = 573;
                v184.y = 92;
                am_40D2B4(&v184, quarry_e);
                }
            if ( magic_p )
                {
                v184.x = 26;
                v184.y = 164;
                am_40D2B4(&v184, magic_p);
                }
            if ( magic_e )
                {
                v184.x = 573;
                v184.y = 164;
                am_40D2B4(&v184, magic_e);
                }
            if ( zoo_p )
                {
                v184.x = 26;
                v184.y = 236;
                am_40D2B4(&v184, zoo_p);
                }
            if ( zoo_e )
                {
                v184.x = 573;
                v184.y = 236;
                am_40D2B4(&v184, zoo_e);
                }
            if ( bricks_p )
                {
                v184.x = 16;
                v184.y = 114;
                am_40D2B4(&v184, bricks_p);
                }
            if ( bricks_e )
                {
                v184.x = 563;
                v184.y = 114;
                am_40D2B4(&v184, bricks_e);
                }
            if ( gems_p )
                {
                v184.x = 16;
                v184.y = 186;
                am_40D2B4(&v184, gems_p);
                }
            if ( gems_e )
                {
                v184.x = 563;
                v184.y = 186;
                am_40D2B4(&v184, gems_e);
                }
            if ( beasts_p )
                {
                v184.x = 16;
                v184.y = 258;
                am_40D2B4(&v184, beasts_p);
                }
            if ( beasts_e )
                {
                v184.x = 563;
                v184.y = 258;
                am_40D2B4(&v184, beasts_e);
                }
            if ( wall_p )
                {
                v184.x = 180;
                v184.y = 296;
                am_40D2B4(&v184, wall_p);
                }
            if ( wall_e )
                {
                v184.x = 442;
                v184.y = 296;
                am_40D2B4(&v184, wall_e);
                }
            if ( tower_p )
                {
                v184.x = 122;
                v184.y = 296;
                am_40D2B4(&v184, tower_p);
                }
            if ( tower_e )
                {
                v184.x = 514;
                v184.y = 296;
                am_40D2B4(&v184, tower_e);
                }
            if ( dmg_p )
                {
                v184.x = 180;
                v184.y = 296;
                am_40D2B4(&v184, dmg_p);
                }
            if ( buildings_p )
                {
                v184.x = 122;
                v184.y = 296;
                am_40D2B4(&v184, buildings_p);
                }
            if ( dmg_e )
                {
                v184.x = 442;
                v184.y = 296;
                am_40D2B4(&v184, dmg_e);
                }
            if ( buildings_e )
                {
                v184.x = 514;
                v184.y = 296;
                am_40D2B4(&v184, buildings_e);
                }
            }
#undef APPLY_TO_BOTH
#undef APPLY_TO_ENEMY        
#undef APPLY_TO_PLAYER

}




//----- (0040D2B4) --------------------------------------------------------
int am_40D2B4( POINT* startXY, int effect_value )
    {
  int v2; // ebp@1
  POINT *v3; // edi@1
  int result; // eax@3
  int v6;
  stru272_stru0 *v8; // ecx@12
  signed int v11; // [sp+10h] [bp-8h]@1
 

  v11 = 0;
  v2 = effect_value;

  while ( array_4FABD0[v11].have_effect )
      {
      result = array_4FABD0[v11].field_40->_40E2A7();
      if ( !result )
          {
          array_4FABD0[v11].have_effect = 0;
          --v11;
          }
      ++v11;
      if ( v11 >= 10 )
          return result;
      }
  v6 = v11;
  array_4FABD0[v11].have_effect = 1;
  if ( effect_value <= 0 )
      {
      array_4FABD0[v6].effect_sign = 0;
      effect_value = -effect_value;
      }
  else
      {
      array_4FABD0[v6].effect_sign = 1;
      }
  array_4FABD0[v6].field_4.effect_area.left = startXY->x - 20;
  array_4FABD0[v6].field_4.effect_area.right = startXY->x + 20;
  array_4FABD0[v6].field_4.effect_area.top = startXY->y - 20;
  array_4FABD0[v6].field_4.effect_area.bottom = startXY->y + 20;
  array_4FABD0[v6].field_4.field_10 = -60;
  array_4FABD0[v6].field_4.field_14 = 60;
  array_4FABD0[v6].field_4.field_18 = 180;
  array_4FABD0[v6].field_4.field_1Cf = 0.5;
  array_4FABD0[v6].field_4.field_20 = 150;
  array_4FABD0[v6].field_4.field_24f= 50.0;
  array_4FABD0[v6].field_4.field_28f = 3.0;
  array_4FABD0[v6].field_4.field_2Cf = 8.0;
  array_4FABD0[v6].field_4.field_30 = 5;
  array_4FABD0[v6].field_4.field_34 = 15;
  array_4FABD0[v6].field_4.sparks_array = &array_4FABD0[v6].effect_sparks[0];
  v8 = array_4FABD0[v6].field_40;
  v8->StartFill(&array_4FABD0[v6].field_4);
  if ( 10 * effect_value > 150 )
      effect_value = 15;

  if ( v8->signature != SIG_trpg )
    return 2;
  if ( !v8->field_59 )
    return 3;
  v8->position_in_sparks_arr = 10 * effect_value;
  v8->field_30 = 0.0;
  v8->field_58 = 0;
  v8->field_44 = 0;
  v8->field_4C = 0;
  v8->field_48 = 0;
  v8->field_50 = 0;
  for (int i=0; i<v8->field_4; ++i)
      v8->field_54[i].have_spark=0;
  return 0;
}


//----- (0040D402) --------------------------------------------------------
int ApplyDamageToBuildings( int player_num, int damage )
    {
  ArcomagePlayer *v2; // ecx@1
  int v3; // esi@1
  int result; // eax@3


  v3 = am_Players[player_num].wall_height;
  if ( v3 <= 0 )
  {
    result = 0;
  }
  else
  {
    if ( v3 >= -damage )
    {
      result = damage;
      am_Players[player_num].wall_height += damage;

    }
    else
        {      
    damage += v3;
    result = -v3;
    am_Players[player_num].wall_height = 0;
    am_Players[player_num].tower_height += damage;
        }
  }

  if ( am_Players[player_num].tower_height < 0 )
    am_Players[player_num].tower_height = 0;
  return result;
}
// 40D402: using guessed type int __fastcall am_40D402(uint, uint);

//----- (0040D444) --------------------------------------------------------
void GameResultsApply()
    {
  int v0; // esi@1
  int v1; // edi@1
  int v2; // eax@1
  int v3; // eax@23
  int v4; // edx@25
  int v5; // ecx@28
  int v6; // eax@28
  GUIWindow *v7; // ecx@50
  signed int v8; // eax@50
  char v9; // eax@52
  signed int v10; // eax@54
  char *v11; // esi@59
  char *v12; // esi@65
  signed int v14; // [sp-4h] [bp-58h]@46
  char pText[64]; // [sp+Ch] [bp-48h]@1
  POINT xy; // [sp+4Ch] [bp-8h]@1

  v0 = -1;
  v1 = -1;
  //nullsub_1();
  xy.x = 0;
  xy.y = 0;
  strcpy(pText, "The Winner is: ");//"Победи"
 // v2 = 0;
  xy.y = 160;
  xy.x = 320; //- 12 * v2 / 2;
//  am_DrawText(-1, pText, xy);
  if ( am_Players[0].tower_height < max_tower_height )
  {
    if ( am_Players[1].tower_height < max_tower_height )
      goto LABEL_10;
    v0 = 2;
  }
  else
  {
    if ( am_Players[1].tower_height < max_tower_height )
    {
      v0 = 1;
    }
    else
    {
      if ( am_Players[0].tower_height == am_Players[1].tower_height )
      {
        v0 = 0;
        v1 = 4;
        goto LABEL_10;
      }
      v0 = (am_Players[0].tower_height <= am_Players[1].tower_height) + 1;
    }
  }
  v1 = 0;
LABEL_10:
  if ( am_Players[0].tower_height <= 0 )
  {
    if ( am_Players[1].tower_height > 0 )
    {
      v0 = 2;
    }
    else
    {
      if ( am_Players[0].tower_height == am_Players[1].tower_height )
      {
LABEL_20:
        if ( am_Players[0].wall_height == am_Players[1].wall_height )
        {
          v0 = 0;
          v1 = 4;
        }
        else
        {
          v0 = (am_Players[0].wall_height <= am_Players[1].wall_height) + 1;
          v1 = 1;
        }
        goto LABEL_23;
      }
      v0 = (am_Players[0].tower_height <= am_Players[1].tower_height) + 1;
    }
    goto LABEL_17;
  }
  if ( am_Players[1].tower_height <= 0 )
  {
    v0 = 1;
LABEL_17:
    v1 = 2;
  }
  if ( !v0 && v1 == 4 )
    goto LABEL_20;
LABEL_23:
  v3 = am_Players[0].resource_bricks;
  if ( am_Players[0].resource_bricks <= am_Players[0].resource_gems )
  {
    v3 = am_Players[0].resource_beasts;
    v4 = am_Players[0].resource_gems;
    if ( am_Players[0].resource_gems > am_Players[0].resource_beasts )
      goto LABEL_28;
  }
  else
  {
    if ( am_Players[0].resource_bricks <= am_Players[0].resource_beasts )
    {
      v4 = am_Players[0].resource_beasts;
      goto LABEL_28;
    }
  }
  v4 = v3;
LABEL_28:
  v5 = am_Players[1].resource_bricks;
  v6 = am_Players[1].resource_gems;
  if ( am_Players[1].resource_bricks <= am_Players[1].resource_gems )
  {
    v5 = am_Players[1].resource_beasts;
    if ( am_Players[1].resource_gems > am_Players[1].resource_beasts )
      goto LABEL_33;
    goto LABEL_32;
  }
  v6 = am_Players[1].resource_beasts;
  if ( am_Players[1].resource_bricks > am_Players[1].resource_beasts )
LABEL_32:
    v6 = v5;
LABEL_33:
  if ( v0 == -1 )
  {
    if ( v1 != -1 )
      goto LABEL_49;
    if ( v4 < max_resources_amount )
    {
      if ( v6 < max_resources_amount )
        goto LABEL_49;
      v0 = 2;
    }
    else
    {
      if ( v6 < max_resources_amount )
      {
        v0 = 1;
      }
      else
      {
        if ( v4 == v6 )
          goto LABEL_46;
        v0 = (v4 <= v6) + 1;
      }
    }
    v1 = 3;
  }
  if ( !v0 && v1 == 4 )
  {
    if ( v4 != v6 )
    {
      v14 = 5;
      v0 = (v4 <= v6) + 1;
      goto LABEL_48;
    }
LABEL_46:
    v0 = 0;
    v14 = 4;
LABEL_48:
    v1 = v14;
  }
LABEL_49:
  pArcomageGame->field_B0 = v1;
  pArcomageGame->uGameResult = v0;
  if ( v0 == 1 )
  {

    v8 = (signed int)window_SpeakInHouse->par1C;
    if (( v8 >= 108 )&&( v8 <= 120 ))
    {
        if ( !pParty->pArcomageWins[v8-108] )
        {
          pParty->pArcomageWins[v8-108] = 1;
          signed int _a = (signed int)(p2DEvents[ window_SpeakInHouse->par1C - 1].fPriceMultiplier * 100.0);
          pParty->PartyFindsGold(_a, 0);
        }
      
    }
    v10 = 108;
    do
    {
      if ( !pParty->pArcomageWins[v10-108] )
        break;
      ++v10;
    }
    while ( v10 <= 120 );

    if ( v10 == 121 )
      _449B7E_toggle_bit(pParty->_quest_bits, 238, 1u);

   
    for (int i=0; i<4; ++i  )
    {
      v11 = (char *)&pParty->pPlayers[i]._achieved_awards_bits;
      if ( !(unsigned __int16)_449B57_test_bit((unsigned __int8 *)v11, 1) )
        _449B7E_toggle_bit((unsigned char *)v11, PLAYER_GUILD_BITS__ARCOMAGE_WIN, 1);
      
    }
   
    ++pParty->uNumArcomageWins;
    if ( pParty->uNumArcomageWins > 1000000 )
      pParty->uNumArcomageWins = 1000000;
  }
  else
  {
  for (int i=0; i<4; ++i  )
      {
      v12 = (char *)&pParty->pPlayers[i]._achieved_awards_bits;
      if ( !(unsigned __int16)_449B57_test_bit((unsigned __int8 *)v12, 1) )
        _449B7E_toggle_bit((unsigned char *)v12, PLAYER_GUILD_BITS__ARCOMAGE_LOSE, 1);
    }
   
    ++pParty->uNumArcomageLoses;
    if ( pParty->uNumArcomageLoses > 1000000 )
      pParty->uNumArcomageLoses = 1000000;
  }
}



//----- (00409C8B) --------------------------------------------------------
void PrepareArcomage()
    {
  signed __int64 v1; // qax@4
  int v2; // esi@4
  int v3; // esi@5
  signed int v4; // edi@5
  int v6; // edx@9
  RECT pXYZW; // [sp+8h] [bp-1Ch]@5
  POINT pXY; // [sp+18h] [bp-Ch]@5

  pAudioPlayer->StopChannels(-1, -1);
  strcpy(pArcomageGame->pPlayer1Name, Player1Name);
  strcpy(pArcomageGame->pPlayer2Name, Player2Name);
  am_byte_4FAA76 = 0;
  am_byte_4FAA75 = 0;

  for (int i=0; i<10; ++i)
      {
      v2 = (i+1) % 4;
      v3 = (i+1) / 4;
      shown_cards[i].uCardId = -1;
      shown_cards[i].field_4 = 0;
      shown_cards[i].field_8.x = 100 * v2 + 120;
      shown_cards[i].field_8.y = 138 * v3 + 18;
      shown_cards[i].field_10_xplus = -100 * v2 / 5;
      shown_cards[i].field_14_y_plus = -138 * v3 / 5;
      shown_cards[i].field_18_point.x = shown_cards[i].field_8.x;
      shown_cards[i].field_18_point.y = shown_cards[i].field_8.y;
      }

  pXY.x = 0;
  pXY.y = 0;
  ArcomageGame::LoadBackground();
  pXYZW.left = 0;
  pXYZW.right = 640;
  pXYZW.top = 0;
  pXYZW.bottom = 480;
  am_BeginScene(pArcomageGame->pBackgroundPixels, -1, 1);
  Blt_Copy(&pXYZW, &pXY, 2);
  am_EndScene();
  pRenderer->Present();
  ArcomageGame::LoadSprites();
  pRenderer->Present();

  v4 = 120;
  for (int i=0; i<12; ++i)
    am_sounds[i] = pSoundList->LoadSound(v4++, 0);

  for (int i=0; i<10; ++i)
     array_4FABD0[i].field_40 = stru272_stru0::New();
  
  current_card_slot_index = -1;
  amuint_4FAA4C = -1;
  byte_4FAA74 = 0;
  pArcomageGame->field_F4 = 0;
  am_gameover = false;
  byte_505880 = 0;
  dword_4FAA70 = 0;
  need_to_discard_card = 0;
  SetStartGameData();
  InitalHandsFill();
  //nullsub_1();
  pArcomageGame->GameOver = 0;
  pArcomageGame->pfntComic = pFontComic;
  pArcomageGame->pfntArrus = pFontArrus;
}


//----- (0040D711) --------------------------------------------------------
ArcomageGame::ArcomageGame()
{
  field_4 = 0;
  //RGBTexture::RGBTexture(&this->pGameBackground);
  //RGBTexture::RGBTexture(&v1->pSprites);
  bGameInProgress = 0;
  field_F9 = 0;
}


//----- (00409BE8) --------------------------------------------------------
void SetStartConditions()
{
  const ArcomageStartConditions *st_cond; // eax@1
    
  st_cond = &start_conditions[window_SpeakInHouse->par1C - 108];
  start_tower_height = st_cond->tower_height;
  start_wall_height  = st_cond->wall_height;
  start_quarry_level = st_cond->quarry_level - 1;
  start_magic_level  = st_cond->magic_level - 1;
  start_zoo_level  = st_cond->zoo_level - 1;
  minimum_cards_at_hand = 5;
  quarry_bonus = 1;
  magic_bonus = 1;
  zoo_bonus = 1;
  max_tower_height = st_cond->max_tower;
  max_resources_amount = st_cond->max_resources;

  opponent_mastery = st_cond->mastery_lvl;

  start_bricks_amount = st_cond->bricks_amount;
  start_gems_amount   = st_cond->gems_amount;
  start_beasts_amount = st_cond->beasts_amount;
  
}


//----- (0040D75D) --------------------------------------------------------
void am_DrawText( int a1, const char *pText, POINT *pXY )
    {
    pPrimaryWindow->DrawText( pFontComic, pXY->x, pXY->y - ((pFontComic->uFontHeight - 3) >> 1) + 3, 0, pText, 0, 0, 0);
    }


//----- (0040DB27) --------------------------------------------------------
void DrawRect( RECT *pXYZW, unsigned __int16 uColor, char bSolidFill )
    {
    RECT *v3; // esi@1

    v3 = pXYZW;
    pRenderer->BeginScene();
    pRenderer->SetRasterClipRect(0, 0, 639u, 479u);
    if ( bSolidFill )
        {
        for ( int i = v3->top; i <= v3->bottom;  ++i )
            pRenderer->RasterLine2D(v3->left, i, v3->right, i, uColor);
        }
    else
        {
        pRenderer->RasterLine2D(v3->left, v3->top, v3->right, v3->top, uColor);
        pRenderer->RasterLine2D(v3->right, v3->top, v3->right, v3->bottom, uColor);
        pRenderer->RasterLine2D(v3->right, v3->bottom, v3->left, v3->bottom, uColor);
        pRenderer->RasterLine2D(v3->left, v3->bottom, v3->left, v3->top, uColor);
        }
    pRenderer->EndScene();
    }

void DrawSquare( POINT *pTargetXY, unsigned __int16 uColor )
    {
    pRenderer->BeginScene();
    if ( pRenderer->uNumSceneBegins )
        {
        int xVal = pTargetXY->x;
        int yVal = pTargetXY->y;
        if ( xVal >= 0 && xVal <= 639 && yVal >= 0 && yVal <= 479)
            {
            pRenderer->pTargetSurface[xVal + pRenderer->uTargetSurfacePitch * yVal] = uColor;
            pRenderer->pTargetSurface[xVal+1 + pRenderer->uTargetSurfacePitch * yVal] = uColor;
            pRenderer->pTargetSurface[xVal + pRenderer->uTargetSurfacePitch * (yVal+1)] = uColor;
            pRenderer->pTargetSurface[xVal+1 + pRenderer->uTargetSurfacePitch * (yVal+1)] = uColor;
            }
        pRenderer->EndScene();
        }
    }

//----- (0040DBD3) --------------------------------------------------------
void DrawPixel( POINT *pTargetXY, unsigned __int16 uColor )
    {
    pRenderer->BeginScene();
    if ( pRenderer->uNumSceneBegins )
        {
        int xVal = pTargetXY->x;
        int yVal = pTargetXY->y;
        if ( xVal >= 0 && xVal <= 639 && yVal >= 0 && yVal <= 479)
            {
            pRenderer->pTargetSurface[xVal + pRenderer->uTargetSurfacePitch * yVal] = uColor;
            }
        pRenderer->EndScene();
        }
    }


//----- (0040DDB1) --------------------------------------------------------
int rand_interval( int min, int max )
    {
    return min + rand() % (max - min + 1);
    }


//----- (0040DEC8) --------------------------------------------------------
void __fastcall am_IntToString(int val, char *pOut)
    {
    sprintfex(pOut, "%d", val);
    }

void set_stru1_field_8_InArcomage(int inValue)
{
  switch(inValue)
  {
  case 91:LOBYTE(pArcomageGame->stru1.field_8) = 123;break;
  case 92:LOBYTE(pArcomageGame->stru1.field_8) = 124;break;
  case 93:LOBYTE(pArcomageGame->stru1.field_8) = 125;break;
  case 96:LOBYTE(pArcomageGame->stru1.field_8) = 126;break;
  case 61:LOBYTE(pArcomageGame->stru1.field_8) = 43;break;
  case 55:LOBYTE(pArcomageGame->stru1.field_8) = 38;break;
  case 56:LOBYTE(pArcomageGame->stru1.field_8) = 42;break;
  case 57:LOBYTE(pArcomageGame->stru1.field_8) = 40;break;
  case 59:LOBYTE(pArcomageGame->stru1.field_8) = 58;break;
  case 54:LOBYTE(pArcomageGame->stru1.field_8) = 94;break;
  case 50:LOBYTE(pArcomageGame->stru1.field_8) = 64;break;
  case 51:LOBYTE(pArcomageGame->stru1.field_8) = 35;break;
  case 52:LOBYTE(pArcomageGame->stru1.field_8) = 36;break;
  case 53:LOBYTE(pArcomageGame->stru1.field_8) = 37;break;
  case 49:LOBYTE(pArcomageGame->stru1.field_8) = 33;break;
  case 39:LOBYTE(pArcomageGame->stru1.field_8) = 34;break;
  case 44:LOBYTE(pArcomageGame->stru1.field_8) = 60;break;
  case 46:LOBYTE(pArcomageGame->stru1.field_8) = 62;break;
  case 47:LOBYTE(pArcomageGame->stru1.field_8) = 63;break;
  case 48:LOBYTE(pArcomageGame->stru1.field_8) = 41;break;
  default:LOBYTE(pArcomageGame->stru1.field_8) = inValue;break;
  }
}