#include "IconFrameTable.h"
#include "LOD.h"
#include "mm7_data.h"
#include "FrameTableInc.h"

//----- (00494F3A) --------------------------------------------------------
unsigned int IconFrameTable::FindIcon(const char *pIconName)
{
  IconFrameTable *v2; // esi@1
  int v3; // ebx@1
  unsigned int uID; // edi@1
  unsigned int result; // eax@4

  v2 = this;
  v3 = 0;
  uID = 0;
  if ( (signed int)this->uNumIcons <= 0 )
  {
LABEL_4:
    result = 0;
  }
  else
  {
    while ( _stricmp(pIconName, v2->pIcons[v3].pAnimationName) )
    {
      ++uID;
      ++v3;
      if ( (signed int)uID >= (signed int)v2->uNumIcons )
        goto LABEL_4;
    }
    result = uID;
  }
  return result;
}

//----- (00494F70) --------------------------------------------------------
IconFrame *IconFrameTable::GetFrame(unsigned int uIconID, unsigned int uFrameID)
{
  IconFrame *v3; // edi@1
  IconFrame *v4; // ecx@1
  __int16 v5; // dx@2
  int v6; // edx@3
  unsigned int v7; // eax@3
  char *i; // ecx@3
  int v9; // esi@5
  IconFrame *result; // eax@6

  v3 = this->pIcons;
  v4 = &v3[uIconID];
  if ( v4->uFlags & 1 && (v5 = v4->uAnimLength) != 0 )
  {
    v6 = ((signed int)uFrameID >> 3) % (unsigned __int16)v5;
    v7 = uIconID;
    for ( i = (char *)&v4->uAnimTime; ; i += 32 )
    {
      v9 = *(short *)i;
      if ( v6 <= v9 )
        break;
      v6 -= v9;
      ++v7;
    }
    result = &v3[v7];
  }
  else
  {
    result = &v3[uIconID];
  }
  return result;
}

//----- (00494FBF) --------------------------------------------------------
void IconFrameTable::InitializeAnimation(unsigned int uIconID)
{
  IconFrameTable *v2; // esi@1
  unsigned int v3; // edi@3
  const char *i; // eax@3
  IconFrame *v5; // eax@5

  v2 = this;
  if ( (signed int)uIconID <= (signed int)this->uNumIcons && (uIconID & 0x80000000u) == 0 )
  {
    v3 = uIconID;
    for ( i = this->pIcons[uIconID].pTextureName; ; i = v5[v3].pTextureName )
    {
      v2->pIcons[v3].uTextureID = pIcons_LOD->LoadTexture(i, TEXTURE_16BIT_PALETTE);
      v5 = v2->pIcons;
      if ( !(v5[v3].uFlags & 1) )
        break;
      ++v3;
    }
  }
}

//----- (0049500A) --------------------------------------------------------
void IconFrameTable::ToFile()
{
  IconFrameTable *v1; // esi@1
  FILE *v2; // eax@1
  FILE *v3; // edi@1

  auto Str = this;

  v1 = Str;
  v2 = fopen("data\\dift.bin", "wb");
  v3 = v2;
  if ( !v2 )
    Error("Unable to save dift.bin!");
  fwrite(v1, 4u, 1u, v2);
  fwrite(v1->pIcons, 0x20u, v1->uNumIcons, v3);
  fclose(v3);
}

//----- (00495056) --------------------------------------------------------
void IconFrameTable::FromFile(void *data_mm6, void *data_mm7, void *data_mm8)
{
  uint num_mm6_frames = data_mm6 ? *(int *)data_mm6 : 0,
       num_mm7_frames = data_mm7 ? *(int *)data_mm7 : 0,
       num_mm8_frames = data_mm8 ? *(int *)data_mm8 : 0;

  uNumIcons = num_mm6_frames + num_mm7_frames + num_mm8_frames;
  Assert(uNumIcons);
  Assert(!num_mm8_frames);

  pIcons = (IconFrame *)malloc(uNumIcons * sizeof(IconFrame));
  memcpy(pIcons,                                   (char *)data_mm7 + 4, num_mm7_frames * sizeof(IconFrame));
  memcpy(pIcons + num_mm7_frames,                  (char *)data_mm6 + 4, num_mm6_frames * sizeof(IconFrame));
  memcpy(pIcons + num_mm6_frames + num_mm7_frames, (char *)data_mm8 + 4, num_mm8_frames * sizeof(IconFrame));
}

//----- (0049509D) --------------------------------------------------------
int IconFrameTable::FromFileTxt(const char *Args)
{
  IconFrameTable *v2; // ebx@1
  FILE *v3; // eax@1
  int v4; // esi@3
  void *v5; // eax@10
  FILE *v6; // ST0C_4@12
  char *i; // eax@12
  const char *v8; // ST00_4@15
  int v9; // eax@16
  int v10; // edx@20
  int v11; // ecx@21
  int v12; // eax@22
  signed int j; // edx@25
  IconFrame *v14; // ecx@26
  int v15; // esi@26
  int k; // eax@27
  signed int result; // eax@11
  char Buf; // [sp+Ch] [bp-2F8h]@3
  FrameTableTxtLine v19; // [sp+200h] [bp-104h]@4
  FrameTableTxtLine v20; // [sp+27Ch] [bp-88h]@4
  int v21; // [sp+2F8h] [bp-Ch]@3
  int v22; // [sp+2FCh] [bp-8h]@3
  FILE *File; // [sp+300h] [bp-4h]@1
  int Argsa; // [sp+30Ch] [bp+8h]@26

  v2 = this;
  //TileTable::dtor((TileTable *)this);
  v3 = fopen(Args, "r");
  File = v3;
  if ( !v3 )
    Error("IconFrameTable::load - Unable to open file: %s.", Args);
  v4 = 0;
  v21 = 0;
  v22 = 1;
  if ( fgets(&Buf, 490, v3) )
  {
    do
    {
      *strchr(&Buf, 10) = 0;
      memcpy(&v20, frame_table_txt_parser(&Buf, &v19), sizeof(v20));
      if ( v20.uPropCount && *v20.pProperties[0] != 47 )
      {
        if ( v20.uPropCount < 3 )
          Error("IconFrameTable::loadText, too few arguments, %s line %i.", Args, v22);
        ++v21;
      }
      ++v22;
    }
    while ( fgets(&Buf, 490, File) );
    v4 = v21;
  }
  v2->uNumIcons = v4;
  v5 = malloc(32 * v4);//, "I Frames");
  v2->pIcons = (IconFrame *)v5;
  if ( v5 )
  {
    v6 = File;
    v2->uNumIcons = 0;
    fseek(v6, 0, 0);
    for ( i = fgets(&Buf, 490, File); i; i = fgets(&Buf, 490, File) )
    {
      *strchr(&Buf, 10) = 0;
      memcpy(&v20, frame_table_txt_parser(&Buf, &v19), sizeof(v20));
      if ( v20.uPropCount && *v20.pProperties[0] != 47 )
      {
        strcpy(v2->pIcons[v2->uNumIcons].pAnimationName, v20.pProperties[0]);
        strcpy(v2->pIcons[v2->uNumIcons].pTextureName, v20.pProperties[1]);
        v8 = v20.pProperties[2];
        v2->pIcons[v2->uNumIcons].uFlags = 0;
        if ( !_stricmp(v8, "new") )
        {
          v9 = (int)&v2->pIcons[v2->uNumIcons].uFlags;
          *(char *)v9 |= 4u;
        }
        v2->pIcons[v2->uNumIcons].uAnimTime = atoi(v20.pProperties[3]);
        v2->pIcons[v2->uNumIcons].uAnimLength = 0;
        v2->pIcons[v2->uNumIcons++].uTextureID = 0;
      }
    }
    fclose(File);
    v10 = 0;
    if ( (signed int)(v2->uNumIcons - 1) > 0 )
    {
      v11 = 0;
      do
      {
        v12 = (int)&v2->pIcons[v11];
        if ( !(*(char *)(v12 + 60) & 4) )
          *(char *)(v12 + 28) |= 1u;
        ++v10;
        ++v11;
      }
      while ( v10 < (signed int)(v2->uNumIcons - 1) );
    }
    for ( j = 0; j < (signed int)v2->uNumIcons; *(short *)(Argsa + 26) = v15 )
    {
      v14 = v2->pIcons;
      Argsa = (int)&v14[j];
      v15 = *(short *)(Argsa + 24);
      if ( *(char *)(Argsa + 28) & 1 )
      {
        ++j;
        for ( k = (int)&v14[j]; *(char *)(k + 28) & 1; k += 32 )
        {
          v15 += *(short *)(k + 24);
          ++j;
        }
        LOWORD(v15) = v14[j].uAnimTime + v15;
      }
      ++j;
    }
    result = 1;
  }
  else
  {
    fclose(File);
    result = 0;
  }
  return result;
}
//----- (0042EB78) --------------------------------------------------------
int IconFrameTable::GetIconAnimLength(unsigned int uIconID)
{
  return 8 * this->pIcons[uIconID].uAnimLength;
}