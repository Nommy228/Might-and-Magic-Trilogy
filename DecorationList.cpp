#include "Sprites.h"
#include "DecorationList.h"
#include "MM7.h"
#include "FrameTableInc.h"
#include "mm7_data.h"
#include "Indoor.h"
#include "ErrorHandling.h"

//----- (0045864C) --------------------------------------------------------
void DecorationList::FromFile(void *data_mm6, void *data_mm7, void *data_mm8)
{
  uint num_mm6_decs = data_mm6 ? *(int *)data_mm6 : 0,
       num_mm7_decs = data_mm7 ? *(int *)data_mm7 : 0,
       num_mm8_decs = data_mm8 ? *(int *)data_mm8 : 0;

  uNumDecorations = num_mm6_decs + num_mm7_decs + num_mm8_decs;
  Assert(uNumDecorations);
  Assert(!num_mm8_decs);

  pDecorations = (DecorationDesc *)malloc(uNumDecorations * sizeof(DecorationDesc));
  memcpy(pDecorations, (char *)data_mm7 + 4, num_mm7_decs * sizeof(DecorationDesc));
  for (uint i = 0; i < num_mm6_decs; ++i)
  {
    memcpy(pDecorations + num_mm7_decs + i, (char *)data_mm6 + 4 + i * sizeof(DecorationDesc_mm6), sizeof(DecorationDesc_mm6));
    pDecorations[num_mm7_decs + i].uColoredLightRed = 255;
    pDecorations[num_mm7_decs + i].uColoredLightGreen = 255;
    pDecorations[num_mm7_decs + i].uColoredLightBlue = 255;
    pDecorations[num_mm7_decs + i].__padding = 255;
  }
  memcpy(pDecorations + num_mm6_decs + num_mm7_decs, (char *)data_mm8 + 4, num_mm8_decs * sizeof(DecorationDesc));
}

//----- (00458693) --------------------------------------------------------
void DecorationList::InitializeDecorationSprite(unsigned int uDecID)
{
  pSpriteFrameTable->InitializeSprite(this->pDecorations[uDecID].uSpriteID);
}

//----- (004586B0) --------------------------------------------------------
bool DecorationList::FromFileTxt(const char *Args)
{
  DecorationList *v2; // ebx@1
  FILE *v3; // eax@1
  unsigned int v4; // esi@3
  void *v5; // eax@10
  FILE *v6; // ST18_4@12
  char *i; // eax@12
  unsigned __int16 v8; // ax@16
  const char *v9; // ST20_4@16
  const char *v10; // ST18_4@16
  __int16 v11; // ax@16
  const char *v12; // ST14_4@16
  unsigned __int16 v13; // ax@16
  const char *v14; // ST10_4@16
  __int16 v15; // ax@16
  const char *v16; // ST0C_4@16
  unsigned __int8 v17; // al@16
  const char *v18; // ST08_4@16
  unsigned __int8 v19; // al@16
  const char *v20; // ST04_4@16
  unsigned __int8 v21; // al@16
  const char *v22; // ST00_4@16
  unsigned __int8 v23; // zf@16
  char v24; // sf@16
  unsigned __int8 v25; // of@16
  int j; // edi@17
  const char *v27; // esi@18
  int v28; // eax@19
  int v29; // eax@21
  int v30; // eax@23
  int v31; // eax@25
  int v32; // eax@27
  int v33; // eax@29
  int v34; // eax@31
  int v35; // eax@33
  FrameTableTxtLine v37; // [sp+Ch] [bp-460h]@17
  FrameTableTxtLine v38; // [sp+88h] [bp-3E4h]@13
  char Dest; // [sp+104h] [bp-368h]@17
  char Buf; // [sp+17Ch] [bp-2F0h]@3
  FrameTableTxtLine v41; // [sp+370h] [bp-FCh]@4
  FrameTableTxtLine v42; // [sp+3ECh] [bp-80h]@4
  FILE *File; // [sp+468h] [bp-4h]@1
  unsigned int Argsa; // [sp+474h] [bp+8h]@3

  v2 = this;
  v3 = fopen(Args, "r");
  File = v3;
  if ( !v3 )
    Error("DecorationDescriptionList::load - Unable to open file: %s.");

  v4 = 0;
  Argsa = 0;
  if ( fgets(&Buf, 490, v3) )
  {
    do
    {
      *strchr(&Buf, 10) = 0;
      memcpy(&v42, frame_table_txt_parser(&Buf, &v41), sizeof(v42));
      if ( v42.uPropCount && *v42.pProperties[0] != 47 && v42.uPropCount >= 3 )
        ++Argsa;
    }
    while ( fgets(&Buf, 490, File) );
    v4 = Argsa;
  }
  v2->uNumDecorations = v4;
  v5 = malloc(84 * v4);
  v2->pDecorations = (DecorationDesc *)v5;
  if ( !v5 )
    Error("DecorationDescriptionList::load - Out of Memory!");

  v6 = File;
  v2->uNumDecorations = 0;
  fseek(v6, 0, 0);
  for ( i = fgets(&Buf, 490, File); i; i = fgets(&Buf, 490, File) )
  {
    *strchr(&Buf, 10) = 0;
    memcpy(&v42, frame_table_txt_parser(&Buf, &v38), sizeof(v42));
    if ( v42.uPropCount && *v42.pProperties[0] != 47 && v42.uPropCount >= 3 )
    {
      strcpy(v2->pDecorations[v2->uNumDecorations].pName, v42.pProperties[1]);
      v8 = pSpriteFrameTable->FastFindSprite(v2->pDecorations[v2->uNumDecorations].pName);
      v9 = v42.pProperties[2];
      v2->pDecorations[v2->uNumDecorations].uSpriteID = v8;
      strcpy(v2->pDecorations[v2->uNumDecorations].field_20, v9);
      v10 = v42.pProperties[3];
      v2->pDecorations[v2->uNumDecorations].uType = 0;
      v11 = atoi(v10);
      v12 = v42.pProperties[4];
      v2->pDecorations[v2->uNumDecorations].uRadius = v11;
      v13 = atoi(v12);
      v14 = v42.pProperties[5];
      v2->pDecorations[v2->uNumDecorations].uDecorationHeight = v13;
      v15 = atoi(v14);
      v16 = v42.pProperties[6];
      v2->pDecorations[v2->uNumDecorations].uLightRadius = v15;
      v17 = atoi(v16);
      v18 = v42.pProperties[7];
      v2->pDecorations[v2->uNumDecorations].uColoredLightRed = v17;
      v19 = atoi(v18);
      v20 = v42.pProperties[8];
      v2->pDecorations[v2->uNumDecorations].uColoredLightGreen = v19;
      v21 = atoi(v20);
      v22 = v42.pProperties[9];
      v2->pDecorations[v2->uNumDecorations].uColoredLightBlue = v21;
      v2->pDecorations[v2->uNumDecorations].uSoundID = atoi(v22);
      v25 = __OFSUB__(v42.uPropCount, 10);
      v23 = v42.uPropCount == 10;
      v24 = v42.uPropCount - 10 < 0;
      v2->pDecorations[v2->uNumDecorations].uFlags = 0;
      if ( !((unsigned __int8)(v24 ^ v25) | v23) )
      {
        strcpy(&Dest, v42.pProperties[10]);
        memcpy(&v41, frame_table_txt_parser(&Dest, &v37), sizeof(v41));
        for ( j = 0; j < v41.uPropCount; ++j )
        {
          v27 = v41.pProperties[j];
          if ( _stricmp(v41.pProperties[j], "NBM") )
          {
            if ( _stricmp(v27, "Invisible") )
            {
              if ( _stricmp(v27, "FS") )
              {
                if ( _stricmp(v27, "FM") )
                {
                  if ( _stricmp(v27, "FF") )
                  {
                    if ( _stricmp(v27, "Marker") )
                    {
                      if ( _stricmp(v27, "LoopSlow") )
                      {
                        if ( _stricmp(v27, "EmitFire") )
                        {
                          if ( _stricmp(v27, "Dawn") )
                          {
                            if ( !_stricmp(v27, "Dusk") )
                              HIBYTE(v2->pDecorations[v2->uNumDecorations].uFlags) |= 2u;
                          }
                          else
                          {
                            HIBYTE(v2->pDecorations[v2->uNumDecorations].uFlags) |= 1u;
                          }
                        }
                        else
                        {
                          v35 = (int)&v2->pDecorations[v2->uNumDecorations].uFlags;
                          *(char *)v35 |= 0x80u;
                        }
                      }
                      else
                      {
                        v34 = (int)&v2->pDecorations[v2->uNumDecorations].uFlags;
                        *(char *)v34 |= 0x40u;
                      }
                    }
                    else
                    {
                      v33 = (int)&v2->pDecorations[v2->uNumDecorations].uFlags;
                      *(char *)v33 |= 0x20u;
                    }
                  }
                  else
                  {
                    v32 = (int)&v2->pDecorations[v2->uNumDecorations].uFlags;
                    *(char *)v32 |= 0x10u;
                  }
                }
                else
                {
                  v31 = (int)&v2->pDecorations[v2->uNumDecorations].uFlags;
                  *(char *)v31 |= 8u;
                }
              }
              else
              {
                v30 = (int)&v2->pDecorations[v2->uNumDecorations].uFlags;
                *(char *)v30 |= 4u;
              }
            }
            else
            {
              v29 = (int)&v2->pDecorations[v2->uNumDecorations].uFlags;
              *(char *)v29 |= 2u;
            }
          }
          else
          {
            v28 = (int)&v2->pDecorations[v2->uNumDecorations].uFlags;
            *(char *)v28 |= 1u;
          }
        }
      }
      ++v2->uNumDecorations;
    }
  }
  fclose(File);
  return 1;
}
//----- (00458600) --------------------------------------------------------
void DecorationList::ToFile()
{
  DecorationList *v1; // esi@1
  FILE *v2; // eax@1
  FILE *v3; // edi@1

  v1 = this;
  v2 = fopen("data\\ddeclist.bin", "wb");
  v3 = v2;
  if ( !v2 )
    Error("Unable to save ddeclist.bin!");

  fwrite(v1, 4u, 1u, v2);
  fwrite(v1->pDecorations, 0x54u, v1->uNumDecorations, v3);
  fclose(v3);
}
//----- (004488B6) --------------------------------------------------------
unsigned __int16 DecorationList::GetDecorIdByName(const char *pName)
{
  DecorationList *v2; // esi@1
  signed int uID; // edi@2
  signed int v4; // ebx@3
  unsigned __int16 result; // ax@6

  v2 = this;
  if ( pName && (uID = 1, (signed int)this->uNumDecorations > 1) )
  {
    v4 = 1;
    while ( _stricmp(pName, v2->pDecorations[v4].pName) )
    {
      ++uID;
      ++v4;
      if ( uID >= (signed int)v2->uNumDecorations )
        return 0;
    }
    result = uID;
  }
  else
  {
    result = 0;
  }
  return result;
}