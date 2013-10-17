#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>

#include "Overlays.h"
#include "Sprites.h"
#include "FrameTableInc.h"
#include "IconFrameTable.h"
#include "Time.h"
#include "Party.h"
#include "TurnEngine.h"
#include "LOD.h"
#include "Render.h"
#include "GUIWindow.h"

#include "mm7_data.h"





struct OtherOverlayList *pOtherOverlayList = new OtherOverlayList; // idb
struct OverlayList *pOverlayList = new OverlayList;






// inlined
//----- (mm6c::0045BD50) --------------------------------------------------
void OtherOverlayList::Reset()
{
  for (uint i = 0; i < 50; ++i)
    pOverlays[i].Reset();
}

//----- (004418B1) --------------------------------------------------------
int OtherOverlayList::_4418B1(int a2, int a3, int a4, int a5)
{
  return 0;
}

//----- (004418B6) --------------------------------------------------------
int OtherOverlayList::_4418B6(int uOverlayID, __int16 a3, int a4, int a5, __int16 a6)
{
  signed int v6; // edx@1
  OtherOverlayList *v7; // eax@1
  //signed int result; // eax@4
  signed int v9; // esi@6
  __int16 v10; // dx@9
  __int16 v11; // dx@11

  v6 = 0;
  v7 = this;
  while ( v7->pOverlays[0].field_6 > 0 )
  {
    ++v6;
    v7 = (OtherOverlayList *)((char *)v7 + 20);
    if ( v6 >= 50 )
      return 0;
  }
  v7->pOverlays[0].field_0 = 0;
  v7->pOverlays[0].field_A = 0;
  v7->pOverlays[0].field_8 = 0;
  v7->pOverlays[0].field_C = a3;
  v9 = 0;
  if ( (signed int)pOverlayList->uNumOverlays <= 0 )
  {
LABEL_9:
    v10 = 0;
  }
  else
  {
    while ( uOverlayID != pOverlayList->pOverlays[v9].uOverlayID )
    {
      ++v9;
      if ( v9 >= (signed int)pOverlayList->uNumOverlays )
        goto LABEL_9;
    }
    v10 = v9;
  }
  v7->pOverlays[0].field_2 = v10;
  v7->pOverlays[0].field_4 = 0;
  if ( a4 )
    v11 = a4;
  else
    v11 = 8 * pSpriteFrameTable->pSpriteSFrames[pOverlayList->pOverlays[v10].uSpriteFramesetID].uAnimLength;
  v7->pOverlays[0].field_6 = v11;
  v7->pOverlays[0].field_10 = a5;
  v7->pOverlays[0].field_E = a6;

  bRedraw = true;
  return true;
}

//----- (00441964) --------------------------------------------------------
void OtherOverlayList::DrawTurnBasedIcon(int a2)
{
  unsigned int result; // eax@1
  IconFrameTable *v3; // ecx@6
  void *v4; // eax@12
  unsigned int v5; // [sp-8h] [bp-Ch]@4
  int v6; // [sp-4h] [bp-8h]@4
  Texture *v7; // [sp-4h] [bp-8h]@14

  if ( pCurrentScreen != SCREEN_GAME || !pParty->bTurnBasedModeOn)
    return;

  if ( pTurnEngine->turn_stage == 3 )
  {
    v6 = pEventTimer->uStartTime;
    v5 = pIconIDs_Turn[5 - pTurnEngine->uActionPointsLeft / 26];
  }
  else
  {
    if ( pTurnEngine->turn_stage == 1 )
    {
      v6 = dword_50C994;
      v3 = pIconsFrameTable;
      if ( dword_50C998_turnbased_icon_1A )
        v5 = uIconID_TurnStart;
      else
        v5 = uIconID_TurnHour;
      goto LABEL_12;
    }
    if ( pTurnEngine->turn_stage != 2 )
    {
      v4 = this;
      goto LABEL_14;
    }
    v6 = pEventTimer->uStartTime;
    v5 = uIconID_TurnStop;
  }
  v3 = pIconsFrameTable;
LABEL_12:
  v4 = pIconsFrameTable->GetFrame(v5, v6);
LABEL_14:
  v7 = &pIcons_LOD->pTextures[*((short *)v4 + 15)];
  if ( pRenderer->pRenderD3D )
    pRenderer->DrawTextureIndexed(0x18Au, 0x120u, v7);
  else
    pRenderer->DrawTextureTransparent(0x18Au, 0x120u, v7);
  result = dword_50C994;
  if ( dword_50C994 < dword_50C998_turnbased_icon_1A )
  {
    result = pEventTimer->uTimeElapsed + dword_50C994;
    dword_50C994 = result;
    if ( (signed int)result >= dword_50C998_turnbased_icon_1A )
      dword_50C998_turnbased_icon_1A = 0;
  }
}
// 4E28F8: using guessed type int pCurrentScreen;
// 50C994: using guessed type int dword_50C994;
// 50C998: using guessed type int dword_50C998_turnbased_icon_1A;


//----- (00458D97) --------------------------------------------------------
void OverlayList::InitializeSprites()
{
  for (uint i = 0; i < uNumOverlays; ++i)
    pSpriteFrameTable->InitializeSprite(pOverlays[i].uSpriteFramesetID);
}

//----- (00458DBC) --------------------------------------------------------
void OverlayList::ToFile()
{
  OverlayList *v1; // esi@1
  FILE *v2; // eax@1
  FILE *v3; // edi@1

  v1 = this;
  v2 = fopen("data\\doverlay.bin", "wb");
  v3 = v2;
  if ( !v2 )
    Error("Unable to save doverlay.bin!");
  fwrite(v1, 4u, 1u, v2);
  fwrite(v1->pOverlays, 8u, v1->uNumOverlays, v3);
  fclose(v3);
}

//----- (00458E08) --------------------------------------------------------
void OverlayList::FromFile(void *data_mm6, void *data_mm7, void *data_mm8)
{
  uint num_mm6_overlays = data_mm6 ? *(int *)data_mm6 : 0,
       num_mm7_overlays = data_mm7 ? *(int *)data_mm7 : 0,
       num_mm8_overlays = data_mm8 ? *(int *)data_mm8 : 0;

  uNumOverlays = num_mm6_overlays + num_mm7_overlays + num_mm8_overlays;
  Assert(uNumOverlays);
  Assert(!num_mm8_overlays);

  pOverlays = (OverlayDesc *)malloc(uNumOverlays * sizeof(OverlayDesc));
  memcpy(pOverlays,                                       (char *)data_mm7 + 4, num_mm7_overlays * sizeof(OverlayDesc));
  memcpy(pOverlays + num_mm7_overlays,                    (char *)data_mm6 + 4, num_mm6_overlays * sizeof(OverlayDesc));
  memcpy(pOverlays + num_mm6_overlays + num_mm7_overlays, (char *)data_mm8 + 4, num_mm8_overlays * sizeof(OverlayDesc));
}

//----- (00458E4F) --------------------------------------------------------
bool OverlayList::FromFileTxt(const char *Args)
{
  OverlayList *v2; // ebx@1
  __int32 v3; // edi@1
  FILE *v4; // eax@1
  unsigned int v5; // esi@3
  const void *v6; // ST18_4@9
  void *v7; // eax@9
  FILE *v8; // ST0C_4@11
  char *i; // eax@11
  unsigned __int16 v10; // ax@14
  const char *v11; // ST1C_4@14
  int v12; // eax@16
  OverlayDesc *v13; // ecx@16
  char v14; // zf@16
  unsigned int v15; // eax@16
  char Buf; // [sp+10h] [bp-2F0h]@3
  FrameTableTxtLine v18; // [sp+204h] [bp-FCh]@4
  FrameTableTxtLine v19; // [sp+280h] [bp-80h]@4
  FILE *File; // [sp+2FCh] [bp-4h]@1
  unsigned int Argsa; // [sp+308h] [bp+8h]@3

  v2 = this;
  free(this->pOverlays);
  v3 = 0;
  v2->pOverlays = 0;
  v2->uNumOverlays = 0;
  v4 = fopen(Args, "r");
  File = v4;
  if ( !v4 )
    Error("ObjectDescriptionList::load - Unable to open file: %s.");

  v5 = 0;
  Argsa = 0;
  if ( fgets(&Buf, 490, v4) )
  {
    do
    {
      *strchr(&Buf, 10) = 0;
      memcpy(&v19, txt_file_frametable_parser(&Buf, &v18), sizeof(v19));
      if ( v19.uPropCount && *v19.pProperties[0] != 47 )
        ++Argsa;
    }
    while ( fgets(&Buf, 490, File) );
    v5 = Argsa;
    v3 = 0;
  }
  v6 = v2->pOverlays;
  v2->uNumOverlays = v5;
  v7 = malloc(8 * v5);
  v2->pOverlays = (OverlayDesc *)v7;
  if ( v7 == (void *)v3 )
    Error("OverlayDescriptionList::load - Out of Memory!");

  memset(v7, v3, 8 * v2->uNumOverlays);
  v8 = File;
  v2->uNumOverlays = v3;
  fseek(v8, v3, v3);
  for ( i = fgets(&Buf, 490, File); i; i = fgets(&Buf, 490, File) )
  {
    *strchr(&Buf, 10) = 0;
    memcpy(&v19, txt_file_frametable_parser(&Buf, &v18), sizeof(v19));
    if ( v19.uPropCount && *v19.pProperties[0] != 47 )
    {
      v10 = atoi(v19.pProperties[0]);
      v11 = v19.pProperties[1];
      v2->pOverlays[v2->uNumOverlays].uOverlayID = v10;
      if ( _stricmp(v11, "center") )
      {
        v12 = _stricmp(v19.pProperties[1], "transparent");
        v13 = v2->pOverlays;
        v14 = v12 == 0;
        v15 = v2->uNumOverlays;
        if ( v14 )
          v13[v15].uOverlayType = 2;
        else
          v13[v15].uOverlayType = 1;
      }
      else
      {
        v2->pOverlays[v2->uNumOverlays].uOverlayType = 0;
      }
      v2->pOverlays[v2->uNumOverlays++].uSpriteFramesetID = pSpriteFrameTable->FastFindSprite(
                                                              (char *)v19.pProperties[2]);
    }
  }
  fclose(File);
  return 1;
}


//----- (0045855F) --------------------------------------------------------
void OtherOverlay::Reset()
{
  this->field_10 = 65536;
  this->field_A = 0;
  this->field_8 = 0;
  this->field_6 = 0;
  this->field_4 = 0;
  this->field_2 = 0;
  this->field_C = 0;
  this->field_E = 0;
}

//----- (004584B8) --------------------------------------------------------
OtherOverlay::OtherOverlay()
{
  this->field_A = 0;
  this->field_8 = 0;
  this->field_6 = 0;
  this->field_4 = 0;
  this->field_2 = 0;
  this->field_C = 0;
  this->field_E = 0;
  this->field_10 = 65536;
}