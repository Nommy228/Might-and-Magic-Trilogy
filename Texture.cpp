#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>

#include "Texture.h"
#include "FrameTableInc.h"
#include "LOD.h"
#include "PaletteManager.h"
#include "Render.h"

#include "mm7_data.h"










struct TextureFrameTable *pTextureFrameTable;





std::array<Texture *, 2> pTexture_LloydBeacons;
Texture *pTexture_50635C;
Texture *pTex_book_button8_off;
Texture *pTex_book_button8_on;
Texture *pTex_book_button7_off;
Texture *pTex_book_button7_on;
Texture *pTex_book_button6_off;
Texture *pTex_book_button6_on;
Texture *pTex_book_button5_off;
Texture *pTex_book_button5_on;
Texture *pTex_book_button4_off;
Texture *pTex_book_button4_on;
Texture *pTex_book_button3_off;
Texture *pTex_book_button3_on;
Texture *pTex_book_button2_off;
Texture *pTex_book_button1_off;
Texture *pTex_book_button2_on;
Texture *pTex_book_button1_on;
std::array<Texture *, 6> pTexture_TownPortalIcons; // [0]Harmonale, [1]Pierpont, [2]Nighon, [3]Evenmorn Island, [4]Celestia, [5]The Pit
std::array<Texture *, 12> SBPageCSpellsTextureList;
std::array<Texture *, 12> SBPageSSpellsTextureList;
Texture *pSBQuickSpellBtnTextr;
Texture *pSpellBookClickCloseBtnTextr;
Texture *pSBClickQuickSpellBtnTextr;
Texture *pSpellBookCloseBtnTextr;
std::array<std::array<Texture *, 2>, 9> pTextures_tabs;
Texture *pTexture_mapbordr; // idb
Texture *pTexture_pagemask; // idb
std::array<Texture *, 9> pSpellBookPagesTextr;
Texture *pSpellBookPagesTextr_9;
Texture *pSpellBookPagesTextr_10;
Texture *pSpellBookPagesTextr_11;
Texture *pSpellBookPagesTextr_12;
Texture *pSpellBookPagesTextr_13;
Texture *pTexture_AutonotesBook;
Texture *pTexture_CurrentBook;
Texture *pTex_moon_new;
Texture *pTex_moon_4;
Texture *pTex_moon_2;
Texture *pTex_moon_2_2;
Texture *pTex_moon_ful;



RGBTexture stru_506E40; // weak
RGBTexture pTexture_PCX;


int uTextureID_RestUI_restb4; // weak
int uTextureID_RestUI_restexit; // weak
int uTextureID_RestUI_restb3; // weak
int uTextureID_RestUI_restb1; // weak
int uTextureID_RestUI_restb2; // weak
int uTextureID_RestUI_restmain; // weak
unsigned int uTextureID_Leather;
int uTextureID_ar_dn_dn; // weak
int uTextureID_ar_dn_up; // weak
int uTextureID_ar_up_dn; // weak
int uTextureID_ar_up_up; // weak


int uTextureID_507698; // weak
int uTextureID_50769C; // weak
int uTextureID_5076A0; // weak
int uTextureID_5076A4; // weak
int uTextureID_5076A8; // weak
int uTextureID_5076AC; // weak
int uTextureID_5076B0; // weak
int uTextureID_5076B4; // weak
int uTextureID_Parchment; // weak
unsigned int uTextureID_mhp_yel;
unsigned int uTextureID_mhp_red;
unsigned int uTextureID_mhp_grn;
unsigned int uTextureID_mhp_capr;
unsigned int uTextureID_mhp_capl;
unsigned int uTextureID_mhp_bd;
unsigned int uTextureID_BUTTDESC2;
unsigned int uTextureID_x_x_u;
unsigned int uTextureID_BUTTMAKE2;
unsigned int uTextureID_BUTTMAKE;
unsigned int uTextureID_BUTTYES2;
unsigned int uTextureID_x_ok_u;
std::array<Texture *, 22> pPlayerPortraits;
std::array<Texture *, 9> pTexture_IC_KNIGHT;
Texture *pTexture_MAKESKY;
Texture *pTexture_MAKETOP;
std::array<Texture *, 20> pTextures_arrowr;
std::array<Texture *, 20> pTextures_arrowl;
Texture *pTexture_presleft;
Texture *pTexture_pressrigh;
Texture *pTexture_buttminu;
Texture *pTexture_buttplus;
unsigned int uTextureID_Quit1; // weak
unsigned int uTextureID_Resume1; // weak
unsigned int uTextureID_Controls1; // weak
unsigned int uTextureID_Save1; // weak
unsigned int uTextureID_Load1; // weak
unsigned int uTextureID_New1; // weak
unsigned int uTextureID_Options; // weak


unsigned int uTextureID_ib_td5_A;
unsigned int uTextureID_ib_td4_A;
unsigned int uTextureID_ib_td3_A;
unsigned int uTextureID_ib_td2_A;
unsigned int uTextureID_ib_td1_A;
int uTextureID_CharacterUI_InventoryBackground; // weak
int uTextureID_50795C; // weak


unsigned int uTextureID_Btn_GameSettings;


unsigned int uTextureID_Btn_Rest;
unsigned int uTextureID_Btn_CastSpell;
unsigned int uTextureID_Btn_ZoomIn;
unsigned int uTextureID_Btn_ZoomOut;
unsigned int uTextureID_FONTPAL;
unsigned int uTextureID_Btn_NPCRight;
unsigned int uTextureID_Btn_NPCLeft;
std::array<unsigned int, 8> pTextureIDs_pMapDirs;


unsigned int uTextureID_BarRed;
unsigned int uTextureID_BarYellow;
unsigned int uTextureID_BarGreen;
unsigned int uTextureID_BarBlue;


unsigned int uTextureID_right_panel; // weak


RGBTexture *pTexture_StatusBar = new RGBTexture;
RGBTexture *pTexture_LeftFrame = new RGBTexture;
RGBTexture *pTexture_TopFrame = new RGBTexture;
RGBTexture *pTexture_BottomFrame = new RGBTexture;
RGBTexture *pTexture_RightFrame = new RGBTexture;


unsigned int uTextureID_right_panel_loop;


Texture *pTexture_Leather;
Texture *pTexture_RestUI_CurrentSkyFrame; // idb
Texture *pTexture_RestUI_CurrentHourglassFrame; // idb


std::array<unsigned int, 5> uTextureID_Optkb;

unsigned int optvid_base_texture_id;
unsigned int bloodsplats_texture_id;
unsigned int us_colored_lights_texture_id;
unsigned int tinting_texture_id;
unsigned int uTextureID_507C20; // weak
unsigned int uTextureID_507C24; // weak
std::array<unsigned int, 10> pTextureIDs_GammaPositions;
unsigned int not_available_bloodsplats_texture_id;
unsigned int not_available_us_colored_lights_texture_id;
unsigned int not_available_tinting_texture_id;


unsigned int uTextureID_detaliz_close_button; // weak
unsigned int uTextureID_MAGNIF_B; // weak
unsigned int uTextureID_BACKDOLL; // weak
unsigned int uTextureID_BACKHAND; // weak

stru355 stru_4E82A4 = {0x20, 0x41, 0, 0x20, 0xFF0000, 0xFF00, 0xFF, 0xFF000000};
stru355 stru_4EFCBC = {0x20, 0x41, 0, 0x10, 0x7C00, 0x3E0, 0x1F, 0x8000};

Texture pTex_F7CE30;


RGBTexture stru_5773C4; // idb











/*  245 */
#pragma pack(push, 1)
struct PCXHeader1
{
  char magic;
  char version;
  char encoding;
  char bpp;
  __int16 left;
  __int16 up;
  __int16 right;
  __int16 bottom;
  __int16 hres;
  __int16 vres;
};
#pragma pack(pop)

/*  246 */
#pragma pack(push, 1)
struct PCXHeader2
{
  char reserved;
  char planes;
  __int16 pitch;
  __int16 palette_info;
};
#pragma pack(pop)









//----- (0044E054) --------------------------------------------------------
void TextureFrameTable::ToFile()
{
  TextureFrameTable *v1; // esi@1
  FILE *v2; // eax@1
  FILE *v3; // edi@1

  v1 = this;
  v2 = fopen("data\\dtft.bin", "wb");
  v3 = v2;
  if ( !v2 )
    Error("Unable to save dtft.bin!");
  fwrite(v1, 4u, 1u, v2);
  fwrite(v1->pTextures, 0x14u, v1->sNumTextures, v3);
  fclose(v3);
}

//----- (0044E0A0) --------------------------------------------------------
void TextureFrameTable::FromFile(void *data_mm6, void *data_mm7, void *data_mm8)
{
  uint num_mm6_frames = data_mm6 ? *(int *)data_mm6 : 0,
       num_mm7_frames = data_mm7 ? *(int *)data_mm7 : 0,
       num_mm8_frames = data_mm8 ? *(int *)data_mm8 : 0;

  sNumTextures = num_mm6_frames + num_mm7_frames + num_mm8_frames;
  Assert(sNumTextures);
  Assert(!num_mm8_frames);

  pTextures = (TextureFrame *)malloc(sNumTextures * sizeof(TextureFrame));

  memcpy(pTextures,                                   (char *)data_mm7 + 4, num_mm7_frames * sizeof(TextureFrame));
  memcpy(pTextures + num_mm7_frames,                  (char *)data_mm6 + 4, num_mm6_frames * sizeof(TextureFrame));
  memcpy(pTextures + num_mm6_frames + num_mm7_frames, (char *)data_mm8 + 4, num_mm8_frames * sizeof(TextureFrame));
}

//----- (0044E0ED) --------------------------------------------------------
void TextureFrameTable::LoadAnimationSequenceAndPalettes( signed int uIconID )
    {
  TextureFrameTable *v3; // ebx@1
  unsigned int i; // edi@3

  v3 = this;
  if ( (uIconID <= this->sNumTextures) && uIconID >= 0 )
  {
    for ( i = uIconID; ; ++i )
    {
      v3->pTextures[i].uTextureID = pBitmaps_LOD->LoadTexture(v3->pTextures[i].pTextureName, TEXTURE_DEFAULT);

      if (v3->pTextures[i].uTextureID != -1)
        pBitmaps_LOD->pTextures[v3->pTextures[i].uTextureID].palette_id2 = pPaletteManager->LoadPalette(pBitmaps_LOD->pTextures[v3->pTextures[i].uTextureID].palette_id1);
      //result = (unsigned int)v3->pTextures;
      //if ( !(*(char *)(result + i * 20 + 18) & 1) )
      if( v3->pTextures[i].uFlags & 1)
        break;
    }
  }
  return;
}

//----- (0044E163) --------------------------------------------------------
unsigned int TextureFrameTable::FindTextureByName(const char *Str2)
{
  TextureFrameTable *v2; // esi@1
  int v3; // ebx@1
  unsigned int uID; // edi@1
  unsigned int result; // eax@4

  v2 = this;
  v3 = 0;
  uID = 0;
  if ( (signed int)this->sNumTextures <= 0 )
  {
LABEL_4:
    result = -1;
  }
  else
  {
    while ( _stricmp(v2->pTextures[v3].pTextureName, Str2) )
    {
      ++uID;
      ++v3;
      if ( (signed int)uID >= (signed int)v2->sNumTextures )
        goto LABEL_4;
    }
    result = uID;
  }
  return result;
}

//----- (0044E19A) --------------------------------------------------------
int TextureFrameTable::GetFrameTexture(int uFrameID, signed int a3)
{
  int v3; // esi@1
  TextureFrame *v4; // edi@1
  TextureFrame *v5; // ecx@1
  __int16 v6; // dx@2
  int v7; // edx@3
  char *i; // eax@3
  int v9; // ecx@5
  unsigned int result; // eax@6

  v3 = uFrameID;
  v4 = this->pTextures;
  v5 = &v4[uFrameID];
  if ( v5->uFlags & 1 && (v6 = v5->uAnimLength) != 0 )
  {
    v7 = (a3 >> 3) % v6;
    for ( i = (char *)&v5->uAnimTime; ; i += 20 )
    {
      v9 = *(short *)i;
      if ( v7 <= v9 )
        break;
      v7 -= v9;
      ++v3;
    }
    result = v4[v3].uTextureID;
  }
  else
  {
    result = v5->uTextureID;
  }
  return result;
}






//----- (0040F806) --------------------------------------------------------
void *Texture::UnzipPalette()
{
  Texture *v1; // esi@1

  void *v2; // edi@1
  Texture *pSource; // [sp+0h] [bp-4h]@1

  pSource = this;
  v1 = this;
  v2 = malloc(this->uDecompressedSize);
  zlib::MemUnzip(v2, (unsigned int *)&pSource, v1->pLevelOfDetail0_prolly_alpha_mask, v1->uTextureSize);
  return v2;
}

//----- (0040F77C) --------------------------------------------------------
void Texture::Release()
{
  if (this)
  {
    pName[0] = 0;

    if (pBits & 0x0400)
    {
		__debugbreak();
	}
	
	  free(pLevelOfDetail0_prolly_alpha_mask);
      free(pPalette16);
      free(pPalette24);
    

    pLevelOfDetail0_prolly_alpha_mask = nullptr;
    pLevelOfDetail1 = nullptr;
    pLevelOfDetail2 = nullptr;
    pLevelOfDetail3 = nullptr;

    pPalette16 = nullptr;
    pPalette24 = nullptr;

    uSizeOfMaxLevelOfDetail = 0;
    uTextureSize = 0;
    uTextureHeight = 0;
    uTextureWidth = 0;
    uHeightLn2 = 0;
    uWidthLn2 = 0;
    palette_id1 = 0;
    palette_id2 = 0;
    pBits &= 0xFFFF0000;
  }
}

//----- (0040F5F5) --------------------------------------------------------
int RGBTexture::Reload(const char *pContainer)
{
  RGBTexture *v2; // esi@1
  FILE *v3; // eax@3
  FILE *v4; // edi@3
  void *v5; // ebx@7
  signed int result; // eax@11
  unsigned int v7; // ecx@12
  unsigned __int16 *v8; // ST20_4@14
  int v9; // eax@14
  char color_map[48]; // [sp+8h] [bp-98h]@9
  Texture DstBuf; // [sp+38h] [bp-68h]@1
  PCXHeader1 header1; // [sp+80h] [bp-20h]@9
  PCXHeader2 header2; // [sp+90h] [bp-10h]@9
  FILE *File; // [sp+98h] [bp-8h]@3
  size_t Count; // [sp+9Ch] [bp-4h]@6
  void *uSourceLena; // [sp+A8h] [bp+8h]@7

  v2 = this;
  if ( !v2->pPixels )
    return 2;
  v3 = pIcons_LOD->FindContainer(pContainer, 0);
  v4 = v3;
  File = v3;
  if ( !v3 )
    Error("Unable to load %s", pContainer);

  fread(&DstBuf, 1u, 0x30u, v3);
  Count = DstBuf.uTextureSize;
  if ( DstBuf.uDecompressedSize )
  {
    v5 = malloc(DstBuf.uDecompressedSize);
    uSourceLena = malloc(DstBuf.uTextureSize);
    fread(uSourceLena, 1u, Count, File);
    zlib::MemUnzip(v5, &DstBuf.uDecompressedSize, uSourceLena, DstBuf.uTextureSize);
    DstBuf.uTextureSize = DstBuf.uDecompressedSize;
    free(uSourceLena);
  }
  else
  {
    v5 = malloc(DstBuf.uTextureSize);
    fread(v5, 1u, Count, v4);
  }
  memcpy(&header1, v5, 0x10u);
  memcpy(color_map, (char *)v5 + 16, 0x30u);
  memcpy(&header2, (char *)v5 + 64, 6u);
  if ( header1.bpp != 8 )
    return 3;
  v7 = (signed __int16)(header1.right - header1.left + 1);
  if ( (signed int)(v7 * (signed __int16)(header1.bottom - header1.up + 1)) <= (signed int)v2->uNumPixels )
  {
    v2->uWidth = header1.right - header1.left + 1;
    v8 = v2->pPixels;
    v9 = v7 * v2->uHeight;
    v2->uNumPixels = v9;
    v2->uHeight = v9;
    v2->DecodePCX((char *)v5, v8, v7);
    free(v5);
    result = 0;
  }
  else
  {
    result = -1;
  }
  return result;
}

//----- (0040F5BE) --------------------------------------------------------
Texture::Texture()
{
  pName[0] = 0;
  uSizeOfMaxLevelOfDetail = 0;
  uTextureSize = 0;
  uTextureHeight = 0;
  uTextureWidth = 0;
  uHeightLn2 = 0;
  uWidthLn2 = 0;
  palette_id1 = 0;
  palette_id2 = 0;
  pLevelOfDetail0_prolly_alpha_mask = 0;
  pLevelOfDetail3 = 0;
  pLevelOfDetail2 = 0;
  pLevelOfDetail1 = 0;
  pPalette16 = 0;
  pPalette24 = 0;
}

//----- (0040F414) --------------------------------------------------------
int RGBTexture::Load(const char *pContainer, int mode)
{
  FILE *v4; // eax@1
  void *v6; // ebx@5
  char color_map[48]; // [sp+Ch] [bp-98h]@7
  Texture DstBuf; // [sp+3Ch] [bp-68h]@1
  PCXHeader1 header1; // [sp+84h] [bp-20h]@7
  PCXHeader2 header2; // [sp+94h] [bp-10h]@7
  size_t Count; // [sp+A0h] [bp-4h]@4
  char *Str1a; // [sp+ACh] [bp+8h]@5


  v4 = pIcons_LOD->FindContainer(pContainer, 0);
  if ( !v4 )
    Error("Unable to load %s", pContainer);

  fread(&DstBuf, 1u, 0x30u, v4);
  Count = DstBuf.uTextureSize;
  if ( DstBuf.uDecompressedSize )
  {
    Str1a = (char *)malloc(DstBuf.uDecompressedSize);
    v6 = malloc(DstBuf.uTextureSize);
    fread(v6, 1, Count, v4);
    zlib::MemUnzip(Str1a, &DstBuf.uDecompressedSize, v6, DstBuf.uTextureSize);
    DstBuf.uTextureSize = DstBuf.uDecompressedSize;
    free(v6);
  }
  else
  {
    Str1a = (char *)malloc(DstBuf.uTextureSize);
    fread(Str1a, 1, Count, v4);
  }
  memcpy(&header1, Str1a, 0x10u);
  memcpy(color_map, Str1a + 16, 0x30u);
  memcpy(&header2, Str1a + 64, 6);
  if ( header1.bpp != 8 )
    return 3;
  this->uWidth = header1.right - header1.left + 1;
  this->uHeight = header1.bottom - header1.up + 1;
  this->uNumPixels = (signed __int16)this->uWidth * (signed __int16)this->uHeight;
  this->pPixels = (unsigned __int16 *)malloc(2 * this->uNumPixels + 4);
  if ( this->pPixels )
  {
    if ( mode )
	{
      if ( mode != 2 )
      {
        if ( !this->pPixels )
          return 2;
        this->DecodePCX(Str1a, this->pPixels, this->uWidth);
        free(Str1a);
        return 0;
      }
      this->_allocation_flags |= 1;
      this->pPixels = (unsigned __int16 *)malloc(2 * this->uNumPixels + 4);
      if ( !this->pPixels )
        return 2;
      this->DecodePCX(Str1a, this->pPixels, this->uWidth);
      free(Str1a);
      return 0;	
	}
    free(this->pPixels);
  }
  if ( !mode )
  {
    this->pPixels = (unsigned __int16 *)malloc(2 * this->uNumPixels + 4);
    if ( !this->pPixels )
      return 2;
    this->DecodePCX(Str1a, this->pPixels, this->uWidth);
    free(Str1a);
    return 0;	
  }
  if ( mode != 2 )
  {
    if ( !this->pPixels )
      return 2;
    this->DecodePCX(Str1a, this->pPixels, this->uWidth);
    free(Str1a);
    return 0;
  }
  this->_allocation_flags |= 1;
  this->pPixels = (unsigned __int16 *)malloc(2 * this->uNumPixels + 4);
  if ( !this->pPixels )
    return 2;
  this->DecodePCX(Str1a, this->pPixels, this->uWidth);
  free(Str1a);
  return 0;
}

//----- (0040F037) --------------------------------------------------------
signed int RGBTexture::DecodePCX(char *pPcx, unsigned __int16 *pOutPixels, unsigned int uNumPixels)
{
  signed int result; // eax@2
  unsigned char test_byte; // edx@3
  unsigned int read_offset; // ebx@37
  unsigned int row_position; // edi@40
  unsigned char value; // cl@63
  char count; // [sp+50h] [bp-Ch]@43
  unsigned short current_line; // [sp+54h] [bp-8h]@38
  unsigned short *dec_position; 
  unsigned short *temp_dec_position; 
  PCXHeader1 psx_head1;
  PCXHeader2 psx_head2;
	short int width, height;
	BYTE  color_map[48];	// Colormap for 16-color images

 
 memcpy(&psx_head1, pPcx , 16);
 memcpy(&color_map, pPcx + 16, 48);
 memcpy(&psx_head2, pPcx + 64, 6);


 if (psx_head1.bpp!=8)
		return 3; 
 uWidth=(short int )(psx_head1.right-psx_head1.left+1);  // word @ 000014
 uHeight=(short int )(psx_head1.bottom-psx_head1.up+1);  // word @ 000016
 

 uNumPixels=uWidth*uHeight;		  // dword @ 000010
 
 memset(pOutPixels, 0, uNumPixels * sizeof(__int16));
  short i=1;
  while ( (1<<i) !=uWidth)
  {
	  ++i;
      if (i >= 15)
	  break;
  }
  field_18=i;
  short i_=1;
  while ( (1<<i_) !=uHeight)
  {
	  ++i_;
      if (i_ >= 15)
	  break;
  }
  field_1A=i_;
  switch (field_18)
	  {
  case 2:   field_1C = 3;    break;
  case 3:   field_1C = 7;    break;
  case 4:   field_1C = 15;   break;
  case 5:   field_1C = 31;   break;
  case 6:   field_1C = 63;   break;
  case 7:   field_1C = 127;  break;
  case 8:   field_1C = 255;  break;
  case 9:   field_1C = 511;  break;
  case 10:  field_1C = 1023; break;
  case 11:  field_1C = 2047; break;
  case 12:  field_1C = 4095; break;
	  }

  switch (field_1A)
	  {
  case 2:   field_1E = 3;    break;
  case 3:   field_1E = 7;    break;
  case 4:   field_1E = 15;   break;
  case 5:   field_1E = 31;   break;
  case 6:   field_1E = 63;   break;
  case 7:   field_1E = 127;  break;
  case 8:   field_1E = 255;  break;
  case 9:   field_1E = 511;  break;
  case 10:  field_1E = 1023; break;
  case 11:  field_1E = 2047; break;
  case 12:  field_1E = 4095; break;
	  }

 
 read_offset = 128;
 if (psx_head2.planes != 3)
 	  return 0;
  current_line = 0;
  if ( uHeight > 0 )
  {
   dec_position = pOutPixels;
   do
   {
    temp_dec_position = dec_position;
    row_position = 0;
	//decode red line
    if (psx_head2.pitch)
    {
     do
     {
      test_byte = pPcx[read_offset];
      ++read_offset;
      if ((test_byte & 0xC0) == 0xC0)
      {	
	    value = pPcx[read_offset];
       	++read_offset; 
      
       if ((test_byte & 0x3F) > 0)
       {
        count = test_byte & 0x3F;
        do
        {

         ++row_position;
          //*temp_dec_position =0xFF000000;
		  //*temp_dec_position|=(unsigned long)value<<16;
         *temp_dec_position |= LOWORD(pRenderer->uTargetRMask) & ((unsigned __int8)value << (LOBYTE(pRenderer->uTargetGBits)
                                                                                    + LOBYTE(pRenderer->uTargetRBits)
                                                                                    + LOBYTE(pRenderer->uTargetBBits)
                                                                                    - 8));

		   temp_dec_position++;
       if (row_position == psx_head2.pitch)
			 break;
        }
        while (count-- != 1);
       }
      }
      else
      {
       ++row_position;
	   //*temp_dec_position =0xFF000000; 
	  //*temp_dec_position|= (unsigned long)test_byte<<16;
       
       *temp_dec_position |= LOWORD(pRenderer->uTargetRMask) & ((unsigned __int8)test_byte << (LOBYTE(pRenderer->uTargetGBits)
                                                                                    + LOBYTE(pRenderer->uTargetRBits)
                                                                                    + LOBYTE(pRenderer->uTargetBBits)
                                                                                    - 8));

       temp_dec_position++;
      }
    
     }
     while (row_position < psx_head2.pitch);
    }
 
    temp_dec_position = dec_position;
	row_position=0;
	//decode green line
    while (row_position <  psx_head2.pitch)
    {
     test_byte = *(pPcx + read_offset);
     ++read_offset;
     if ((test_byte & 0xC0) == 0xC0)
     {
      value = *(pPcx + read_offset);
	  ++read_offset;
      if ((test_byte & 0x3F) > 0)
      {
       count = test_byte & 0x3F;
       do
       {
        //*temp_dec_position|= (unsigned int)value<<8;
		//temp_dec_position++;
         
         *temp_dec_position|= pRenderer->uTargetGMask & (unsigned __int16)((unsigned __int8)value << (LOBYTE(pRenderer->uTargetGBits)
                                                                                             + LOBYTE(pRenderer->uTargetBBits)
                                                                                             - 8));
         
       temp_dec_position++;
        ++row_position;
        if (row_position == psx_head2.pitch)
			break;
  
       }
       while (count-- != 1);
      }
     }
     else
     {
      //*temp_dec_position |=(unsigned int) test_byte<<8;
	  //temp_dec_position++;
       
         *temp_dec_position|= pRenderer->uTargetGMask & (unsigned __int16)((unsigned __int8)test_byte << (LOBYTE(pRenderer->uTargetGBits)
                                                                                             + LOBYTE(pRenderer->uTargetBBits)
                                                                                             - 8));
       temp_dec_position++;
      ++row_position;
     }
    }

    temp_dec_position = dec_position;
	row_position=0;
	//decode blue line
    while (row_position < psx_head2.pitch)
    {
     test_byte = *(pPcx + read_offset);
	 read_offset++;
     if ((test_byte & 0xC0) ==  0xC0)
     {
       value = *(pPcx + read_offset);
	  ++read_offset;
      if ((test_byte & 0x3F) > 0)
      {
       count = test_byte & 0x3F;
       do
       {
        //*temp_dec_position|= value;
		 //temp_dec_position++;

         *temp_dec_position |= value >> (8 - LOBYTE(pRenderer->uTargetBBits));
       temp_dec_position++;

        ++row_position;
        if (row_position == psx_head2.pitch)
		 break;
       }
       while (count-- != 1);
      }
     }
     else
     {
      //*temp_dec_position|= test_byte;
	   //temp_dec_position++;
         *temp_dec_position |= test_byte >> (8 - LOBYTE(pRenderer->uTargetBBits));
       temp_dec_position++;

      ++row_position;
     }
     
    }
    ++current_line;
    dec_position += uWidth;
   }
   while (current_line < uHeight);
  }
   return 0;
/*
  RGBTexture *v4; // esi@1
  signed int result; // eax@2
  unsigned __int16 v6; // ax@3
  unsigned __int16 *v7; // ecx@3
  unsigned int v8; // edx@3
  signed int v9; // ecx@3
  signed int v10; // ecx@8
  signed int v11; // ebx@37
  unsigned __int16 *v12; // eax@40
  int v13; // edi@40
  int v14; // ebx@41
  char v15; // bl@42
  unsigned __int16 *v16; // eax@50
  int v17; // ebx@51
  char v18; // bl@52
  unsigned __int16 *v19; // eax@61
  unsigned __int8 v20; // dl@62
  unsigned __int8 v21; // dl@63
  unsigned __int8 v22; // cl@63
  char color_map[48]; // [sp+8h] [bp-54h]@1
  PCXHeader1 header1; // [sp+38h] [bp-24h]@1
  PCXHeader2 header2; // [sp+48h] [bp-14h]@1
  unsigned int v26; // [sp+50h] [bp-Ch]@43
  int v27; // [sp+54h] [bp-8h]@38
  unsigned __int16 *v28; // [sp+58h] [bp-4h]@3
  int pOutPixelsa; // [sp+68h] [bp+Ch]@41
  int pOutPixelsb; // [sp+68h] [bp+Ch]@51

  v4 = this;
  memcpy(&header1, pPcx, 0x10u);
  memcpy(color_map, pPcx + 16, 0x30u);
  memcpy(&header2, pPcx + 64, 6u);
  if ( header1.bpp == 8 )
  {
    v6 = header1.right - header1.left + 1;
    LOWORD(v7) = header1.bottom - header1.up + 1;
    v4->uWidth = v6;
    v4->uHeight = (unsigned __int16)v7;
    v7 = (unsigned __int16 *)(signed __int16)v7;
    v28 = v7;
    v4->uNumPixels = (signed __int16)v7 * (signed __int16)v6;
    HIWORD(v8) = 0;
    v9 = 1;
    while ( 1 << v9 != (signed __int16)v6 )
    {
      ++v9;
      if ( v9 >= 15 )
        goto LABEL_8;
    }
    v4->field_18 = v9;
LABEL_8:
    v10 = 1;
    while ( (unsigned __int16 *)(1 << v10) != v28 )
    {
      ++v10;
      if ( v10 >= 15 )
        goto LABEL_13;
    }
    v4->field_1A = v10;
LABEL_13:
    switch ( v4->field_18 )
    {
      case 2:
        v4->field_1C = 3;
        break;
      case 3:
        v4->field_1C = 7;
        break;
      case 4:
        v4->field_1C = 15;
        break;
      case 5:
        v4->field_1C = 31;
        break;
      case 6:
        v4->field_1C = 63;
        break;
      case 7:
        v4->field_1C = 127;
        break;
      case 8:
        v4->field_1C = 255;
        break;
      case 9:
        v4->field_1C = 511;
        break;
      case 10:
        v4->field_1C = 1023;
        break;
      case 11:
        v4->field_1C = 2047;
        break;
      case 12:
        v4->field_1C = 4095;
        break;
      default:
        break;
    }
    switch ( v4->field_1A )
    {
      case 2:
        v4->field_1E = 3;
        break;
      case 3:
        v4->field_1E = 7;
        break;
      case 4:
        v4->field_1E = 15;
        break;
      case 5:
        v4->field_1E = 31;
        break;
      case 6:
        v4->field_1E = 63;
        break;
      case 7:
        v4->field_1E = 127;
        break;
      case 8:
        v4->field_1E = 255;
        break;
      case 9:
        v4->field_1E = 511;
        break;
      case 10:
        v4->field_1E = 1023;
        break;
      case 11:
        v4->field_1E = 2047;
        break;
      case 12:
        v4->field_1E = 4095;
        break;
      default:
        break;
    }
    v11 = 128;
    if ( header2.planes == 3 )
    {
      v27 = 0;
      if ( (signed int)v28 > 0 )
      {
        v28 = pOutPixels;
        do
        {
          v12 = v28;
          v13 = 0;
          if ( header2.pitch )
          {
            do
            {
              LOBYTE(v8) = pPcx[v11];
              v14 = v11 + 1;
              pOutPixelsa = v14;
              if ( (v8 & 0xC0) == -64 )
              {
                pOutPixelsa = v14 + 1;
                v15 = pPcx[v14];
                if ( (signed int)(v8 & 0x3F) > 0 )
                {
                  v26 = v8 & 0x3F;
                  do
                  {
                    ++v13;
                    *v12 = LOWORD(pRenderer->uTargetRMask) & ((unsigned __int8)v15 << (LOBYTE(pRenderer->uTargetGBits)
                                                                                    + LOBYTE(pRenderer->uTargetRBits)
                                                                                    + LOBYTE(pRenderer->uTargetBBits)
                                                                                    - 8));
                    ++v12;
                    if ( v13 == (unsigned __int16)header2.pitch )
                      v12 = &v12[uNumPixels - (unsigned __int16)header2.pitch - 1];
                    --v26;
                  }
                  while ( v26 );
                }
              }
              else
              {
                LOWORD(v8) = (unsigned __int8)v8;
                v8 = pRenderer->uTargetRMask & (v8 << (LOBYTE(pRenderer->uTargetGBits)
                                                    + LOBYTE(pRenderer->uTargetRBits)
                                                    + LOBYTE(pRenderer->uTargetBBits)
                                                    - 8));
                ++v13;
                *v12 = v8;
                ++v12;
              }
              v11 = pOutPixelsa;
            }
            while ( v13 < (unsigned __int16)header2.pitch );
          }
          v16 = v28;
          while ( v13 < 2 * (unsigned __int16)header2.pitch )
          {
            LOBYTE(v8) = pPcx[v11];
            v17 = v11 + 1;
            pOutPixelsb = v17;
            if ( (v8 & 0xC0) == -64 )
            {
              pOutPixelsb = v17 + 1;
              v18 = pPcx[v17];
              if ( (signed int)(v8 & 0x3F) > 0 )
              {
                v26 = v8 & 0x3F;
                do
                {
                  *v16 |= pRenderer->uTargetGMask & (unsigned __int16)((unsigned __int8)v18 << (LOBYTE(pRenderer->uTargetGBits)
                                                                                             + LOBYTE(pRenderer->uTargetBBits)
                                                                                             - 8));
                  ++v13;
                  ++v16;
                  if ( v13 == (unsigned __int16)header2.pitch )
                    v16 = &v16[uNumPixels - (unsigned __int16)header2.pitch - 1];
                  --v26;
                }
                while ( v26 );
              }
            }
            else
            {
              LOWORD(v8) = (unsigned __int8)v8;
              v8 = pRenderer->uTargetGMask & (v8 << (LOBYTE(pRenderer->uTargetGBits) + LOBYTE(pRenderer->uTargetBBits) - 8));
              *v16 |= v8;
              ++v13;
              ++v16;
            }
            v11 = pOutPixelsb;
          }
          v19 = v28;
          while ( v13 < 3 * (unsigned __int16)header2.pitch )
          {
            v20 = pPcx[v11++];
            if ( (v20 & 0xC0) == -64 )
            {
              v21 = v20 & 0x3F;
              v22 = pPcx[v11++];
              if ( (signed int)v21 > 0 )
              {
                v26 = v21;
                do
                {
                  *v19 |= v22 >> (8 - LOBYTE(pRenderer->uTargetBBits));
                  ++v13;
                  ++v19;
                  if ( v13 == (unsigned __int16)header2.pitch )
                  {
                    v8 = uNumPixels - (unsigned __int16)header2.pitch;
                    v19 = &v19[uNumPixels - (unsigned __int16)header2.pitch - 1];
                  }
                  --v26;
                }
                while ( v26 );
              }
            }
            else
            {
              *v19 |= v20 >> (8 - LOBYTE(pRenderer->uTargetBBits));
              ++v13;
              ++v19;
            }
          }
          ++v27;
          v28 += uNumPixels;
        }
        while ( v27 < v4->uHeight );
      }
    }
    result = 0;
  }
  else
  {
    result = 3;
  }
  return result;
  */
}

//----- (0040EAD8) --------------------------------------------------------
unsigned int RGBTexture::LoadFromFILE(FILE *pFile, unsigned int mode, unsigned int bCloseFile)
{
  signed int result; // eax@2
  unsigned char test_byte; // edx@3
  //unsigned int read_offset; // ebx@37
  unsigned int row_position; // edi@40
  unsigned char value; // cl@63
  char count; // [sp+50h] [bp-Ch]@43
  unsigned short current_line; // [sp+54h] [bp-8h]@38
  unsigned short *dec_position; 
  unsigned short *temp_dec_position; 
  PCXHeader1 psx_head1;
  PCXHeader2 psx_head2;
	short int width, height;
	BYTE  color_map[48];	// Colormap for 16-color images

  if (!pFile)
    return 1;
  
 
 fread(&psx_head1, 1, 16, pFile);
 fread(&color_map, 1, 48, pFile);
 fread(&psx_head2, 1, 6, pFile);

  if (psx_head1.bpp!=8)
		return 3; 
 uWidth=(short int )(psx_head1.right-psx_head1.left+1);  // word @ 000014
 uHeight=(short int )(psx_head1.bottom-psx_head1.up+1);  // word @ 000016
 

 uNumPixels=uWidth*uHeight;		  // dword @ 000010


      if ( mode == 0 )
      {
        if ( pPixels )
        {
          free(pPixels);
          pPixels = 0;
        }
        pPixels = (unsigned __int16 *)malloc(2 * uNumPixels + 4);
      }
      else
      {
        if ( mode != 1 && mode == 2 )
        {
          pPixels = (unsigned __int16 *)malloc((uNumPixels + 2) * sizeof(unsigned __int16));
          _allocation_flags |= 1;
        }
      }




      auto pOutPixels = pPixels;
 
 memset(pOutPixels, 0, uNumPixels * sizeof(__int16));

  short i=1;
  while ( (1<<i) !=uWidth)
  {
	  ++i;
      if (i >= 15)
	  break;
  }
  field_18=i;
  short i_=1;
  while ( (1<<i_) !=uHeight)
  {
	  ++i_;
      if (i_ >= 15)
	  break;
  }
  field_1A=i_;
  switch (field_18)
	  {
  case 2:   field_1C = 3;    break;
  case 3:   field_1C = 7;    break;
  case 4:   field_1C = 15;   break;
  case 5:   field_1C = 31;   break;
  case 6:   field_1C = 63;   break;
  case 7:   field_1C = 127;  break;
  case 8:   field_1C = 255;  break;
  case 9:   field_1C = 511;  break;
  case 10:  field_1C = 1023; break;
  case 11:  field_1C = 2047; break;
  case 12:  field_1C = 4095; break;
	  }

  switch (field_1A)
	  {
  case 2:   field_1E = 3;    break;
  case 3:   field_1E = 7;    break;
  case 4:   field_1E = 15;   break;
  case 5:   field_1E = 31;   break;
  case 6:   field_1E = 63;   break;
  case 7:   field_1E = 127;  break;
  case 8:   field_1E = 255;  break;
  case 9:   field_1E = 511;  break;
  case 10:  field_1E = 1023; break;
  case 11:  field_1E = 2047; break;
  case 12:  field_1E = 4095; break;
	  }


  fseek(pFile, 128 - 70, SEEK_CUR);


  for (uint y = 0; y < uHeight; ++y)
  {
    unsigned __int16 *pDst = pPixels + y * uWidth;

    uint x = 0;
    do
    {
      uint ctrl = 0;
      fread(&ctrl, 1, 1, pFile);
      if ((ctrl & 0xC0) == 0xC0)
      {
        uint uNumPixels = ctrl & 0x3F;
        uint clr = 0;
        fread(&clr, 1, 1, pFile);
        for (uint i = 0; i < uNumPixels; ++i)
          pDst[x++] = pRenderer->uTargetRMask & (clr << (pRenderer->uTargetGBits +
                                                         pRenderer->uTargetRBits +
                                                         pRenderer->uTargetBBits - 8));
      }
      else
      {
        pDst[x++] = pRenderer->uTargetRMask & (ctrl << (pRenderer->uTargetGBits +
                                                        pRenderer->uTargetRBits +
                                                        pRenderer->uTargetBBits - 8));
      }
    } while (x < psx_head2.pitch);





    x = 0;
    do
    {
      uint ctrl = 0;
      fread(&ctrl, 1, 1, pFile);
      if ((ctrl & 0xC0) == 0xC0)
      {
        uint uNumPixels = ctrl & 0x3F;
        uint clr = 0;
        fread(&clr, 1, 1, pFile);
        for (uint i = 0; i < uNumPixels; ++i)
          pDst[x++] |= pRenderer->uTargetGMask & (clr << (pRenderer->uTargetGBits +
                                                         pRenderer->uTargetBBits - 8));
      }
      else
      {
        pDst[x++] |= pRenderer->uTargetGMask & (ctrl << (pRenderer->uTargetGBits +
                                                        pRenderer->uTargetBBits - 8));
      }
    } while (x < psx_head2.pitch);
 




    x = 0;
    do
    {
      uint ctrl = 0;
      fread(&ctrl, 1, 1, pFile);
      if ((ctrl & 0xC0) == 0xC0)
      {
        uint uNumPixels = ctrl & 0x3F;
        uint clr = 0;
        fread(&clr, 1, 1, pFile);
        for (uint i = 0; i < uNumPixels; ++i)
          pDst[x++] |= pRenderer->uTargetBMask & (clr >> (8 - pRenderer->uTargetBBits));
      }
      else
      {
        pDst[x++] |= pRenderer->uTargetBMask & (ctrl >> (8 - pRenderer->uTargetRBits));
      }
    } while (x < psx_head2.pitch);
  }

  if (bCloseFile)
    fclose(pFile);
  return 0;
}

//----- (0040E51F) --------------------------------------------------------
void RGBTexture::Release()
{
  RGBTexture *v1; // esi@1
  char v2; // zf@1
  void *v3; // [sp-4h] [bp-Ch]@1

  v1 = this;
  v2 = (this->_allocation_flags & 1) == 0;
  v3 = this->pPixels;
  this->pName[0] = 0;
  if ( v2 )
    free(v3);
  else
    free(v3);
  v1->_allocation_flags = 0;
  v1->pPixels = 0;
  v1->uNumPixels = 0;
  v1->uHeight = 0;
  v1->uWidth = 0;
  v1->field_1A = 0;
  v1->field_18 = 0;
}

//----- (0040E55E) --------------------------------------------------------
int RGBTexture::_40E55E(const char *Filename, unsigned int a3)
{
  RGBTexture *v3; // esi@1
  signed int result; // eax@2
  char *v6; // eax@3
  int v7; // edx@3
  char v8; // cl@4
  unsigned __int16 v9; // cx@9
  unsigned __int16 v10; // ax@9
  int v11; // eax@9
  char v12; // zf@9
  void *v13; // eax@12
  signed int v14; // ecx@19
  signed int v15; // ecx@24
  int v16; // eax@57
  unsigned __int16 *v17; // ecx@57
  unsigned __int16 *v18; // edi@57
  signed int v19; // eax@59
  unsigned __int16 *v20; // edi@64
  signed int v21; // eax@66
  unsigned __int16 *v22; // edi@71
  signed int v23; // eax@73
  int v24; // eax@78
  char v25[48]; // [sp+Ch] [bp-54h]@3
  PCXHeader1 pcx_header1;
  PCXHeader2 pcx_header2;
  int v37; // [sp+54h] [bp-Ch]@3
  int v38; // [sp+58h] [bp-8h]@57
  FILE *File; // [sp+5Ch] [bp-4h]@1

  v3 = this;
  File = fopen(Filename, "rb");
  if ( !File )
    return 1;
  
  fread(&pcx_header1, sizeof(pcx_header1), 1, File);
  fread(&v25,48,1,File);
  fread(&pcx_header2, 4, 1, File);

  v6 = (char *)Filename;
  v37 = 0;
  v7 = (char *)v3 - Filename;
  do
  {
    v8 = *v6;
    if ( !*v6 )
      break;
    if ( v8 == 46 )
      break;
    ++v37;
    (v6++)[v7] = v8;
  }
  while ( v37 < 15 );
  v3->pName[v37] = 0;
  if ( pcx_header1.bpp != 8 )
    return 3;
  v9 = pcx_header1.right - pcx_header1.left + 1;
  v10 = pcx_header1.bottom - pcx_header1.up;
  v3->uWidth = v9;
  ++v10;
  v3->uHeight = v10;
  v11 = (signed __int16)v9 * (signed __int16)v10;
  v12 = a3 == 0;
  v3->uNumPixels = v11;
  if ( v12 )
  {
    if ( v3->pPixels )
      free(v3->pPixels);
    v13 = malloc(2 * v3->uNumPixels + 4);
    goto LABEL_16;
  }
  if ( a3 != 1 && a3 == 2 )
  {
    v13 = malloc(4 * v11 + 8);
    v3->_allocation_flags |= 1u;
LABEL_16:
    v3->pPixels = (unsigned __int16 *)v13;
  }
  if ( v3->pPixels )
  {
    v14 = 1;
    while ( 1 << v14 != v3->uWidth )
    {
      ++v14;
      if ( v14 >= 15 )
        goto LABEL_24;
    }
    v3->field_18 = v14;
LABEL_24:
    v15 = 1;
    while ( 1 << v15 != v3->uHeight )
    {
      ++v15;
      if ( v15 >= 15 )
        goto LABEL_29;
    }
    v3->field_1A = v15;
LABEL_29:
    switch ( v3->field_18 )
    {
      case 2:
        v3->field_1C = 3;
        break;
      case 3:
        v3->field_1C = 7;
        break;
      case 4:
        v3->field_1C = 15;
        break;
      case 5:
        v3->field_1C = 31;
        break;
      case 6:
        v3->field_1C = 63;
        break;
      case 7:
        v3->field_1C = 127;
        break;
      case 8:
        v3->field_1C = 255;
        break;
      case 9:
        v3->field_1C = 511;
        break;
      case 10:
        v3->field_1C = 1023;
        break;
      case 11:
        v3->field_1C = 2047;
        break;
      case 12:
        v3->field_1C = 4095;
        break;
      default:
        break;
    }
    switch ( v3->field_1A )
    {
      case 2:
        v3->field_1E = 3;
        break;
      case 3:
        v3->field_1E = 7;
        break;
      case 4:
        v3->field_1E = 15;
        break;
      case 5:
        v3->field_1E = 31;
        break;
      case 6:
        v3->field_1E = 63;
        break;
      case 7:
        v3->field_1E = 127;
        break;
      case 8:
        v3->field_1E = 255;
        break;
      case 9:
        v3->field_1E = 511;
        break;
      case 10:
        v3->field_1E = 1023;
        break;
      case 11:
        v3->field_1E = 2047;
        break;
      case 12:
        v3->field_1E = 4095;
        break;
      default:
        break;
    }
    fseek(File, 128, 0);
    ftell(File);
	if ( pcx_header2.planes == 1 )
      Error("24bit PCX Only!");

    if ( pcx_header2.planes == 3 )
    {
      v37 = 0;
      if ( v3->uHeight > 0 )
      {
        do
        {
          v16 = v37 * v3->uWidth;
          v17 = v3->pPixels;
          v38 = 0;
          v18 = &v17[v16];
		  if ( pcx_header2.pitch )
          {
            do
            {
              fread((char *)&Filename + 3, 1u, 1u, File);
              if ( (BYTE3(Filename) & 0xC0) == -64 )
              {
                BYTE3(Filename) &= 0x3Fu;
                fread((char *)&a3 + 3, 1u, 1u, File);
                v19 = 0;
                if ( BYTE3(Filename) )
                {
                  do
                  {
                    ++v38;
                    *v18 = LOWORD(pRenderer->uTargetRMask) & (BYTE3(a3) << (LOBYTE(pRenderer->uTargetGBits)
                                                                         + LOBYTE(pRenderer->uTargetRBits)
                                                                         + LOBYTE(pRenderer->uTargetBBits)
                                                                         - 8));
                    ++v18;
                    ++v19;
                  }
                  while ( v19 < BYTE3(Filename) );
                }
              }
              else
              {
                ++v38;
                *v18 = LOWORD(pRenderer->uTargetRMask) & (BYTE3(Filename) << (LOBYTE(pRenderer->uTargetGBits)
                                                                           + LOBYTE(pRenderer->uTargetRBits)
                                                                           + LOBYTE(pRenderer->uTargetBBits)
                                                                           - 8));
                ++v18;
              }
            }
			while ( v38 < (unsigned __int16)pcx_header2.pitch);
          }
          v20 = &v3->pPixels[v37 * v3->uWidth];
		  while ( v38 < 2 * (unsigned __int16)pcx_header2.pitch )
          {
            fread((char *)&Filename + 3, 1u, 1u, File);
            if ( (BYTE3(Filename) & 0xC0) == -64 )
            {
              BYTE3(Filename) &= 0x3Fu;
              fread((char *)&a3 + 3, 1u, 1u, File);
              v21 = 0;
              if ( BYTE3(Filename) )
              {
                do
                {
                  *v20 |= pRenderer->uTargetGMask & (unsigned __int16)(BYTE3(a3) << (LOBYTE(pRenderer->uTargetGBits)
                                                                                  + LOBYTE(pRenderer->uTargetBBits)
                                                                                  - 8));
                  ++v38;
                  ++v20;
                  ++v21;
                }
                while ( v21 < BYTE3(Filename) );
              }
            }
            else
            {
              *v20 |= pRenderer->uTargetGMask & (unsigned __int16)(BYTE3(Filename) << (LOBYTE(pRenderer->uTargetGBits)
                                                                                    + LOBYTE(pRenderer->uTargetBBits)
                                                                                    - 8));
              ++v38;
              ++v20;
            }
          }
          v22 = &v3->pPixels[v37 * v3->uWidth];
		  while ( v38 < 3 * (unsigned __int16)pcx_header2.pitch )
          {
            fread((char *)&Filename + 3, 1u, 1u, File);
            if ( (BYTE3(Filename) & 0xC0) == -64 )
            {
              BYTE3(Filename) &= 0x3Fu;
              fread((char *)&a3 + 3, 1u, 1u, File);
              v23 = 0;
              if ( BYTE3(Filename) )
              {
                do
                {
                  *v22 |= BYTE3(a3) >> (8 - LOBYTE(pRenderer->uTargetBBits));
                  ++v38;
                  ++v22;
                  ++v23;
                }
                while ( v23 < BYTE3(Filename) );
              }
            }
            else
            {
              *v22 |= BYTE3(Filename) >> (8 - LOBYTE(pRenderer->uTargetBBits));
              ++v38;
              ++v22;
            }
          }
          v24 = v3->uHeight;
          ++v37;
        }
        while ( v37 < v24 );
      }
    }
    fclose(File);
    result = 0;
  }
  else
  {
    result = 2;
  }
  return result;
}


//----- (0040D73D) --------------------------------------------------------
RGBTexture::RGBTexture()
{
  pName[0] = 0;
  pPixels = 0;
  uNumPixels = 0;
  uHeight = 0;
  uWidth = 0;
  field_1A = 0;
  field_18 = 0;
  _allocation_flags = 0;
}
//----- (0044E1EC) --------------------------------------------------------
int TextureFrameTable::FromFileTxt(const char *Args)
{
  TextureFrameTable *v2; // ebx@1
  FILE *v3; // eax@1
  int v4; // esi@3
  const void *v5; // ST0C_4@10
  void *v6; // eax@10
  FILE *v7; // ST0C_4@12
  char *i; // eax@12
  signed int v9; // esi@15
  int v10; // eax@17
  int v11; // edx@22
  int v12; // ecx@23
  int v13; // eax@24
  signed int j; // eax@27
  TextureFrame *v15; // edx@28
  int v16; // esi@28
  int k; // ecx@29
  char Buf; // [sp+Ch] [bp-2F8h]@3
  FrameTableTxtLine v20; // [sp+200h] [bp-104h]@4
  int v21; // [sp+27Ch] [bp-88h]@4
  char *Str1; // [sp+280h] [bp-84h]@5
  char *Str; // [sp+284h] [bp-80h]@15
  int v24; // [sp+2F8h] [bp-Ch]@3
  int v25; // [sp+2FCh] [bp-8h]@3
  FILE *File; // [sp+300h] [bp-4h]@1
  int Argsa; // [sp+30Ch] [bp+8h]@28

  v2 = this;
  v3 = fopen(Args, "r");
  File = v3;
  if ( !v3 )
    Error("CTextureFrameTable::load - Unable to open file: %s.", Args);

  v4 = 0;
  v24 = 0;
  v25 = 1;
  if ( fgets(&Buf, 490, v3) )
  {
    do
    {
      *strchr(&Buf, 10) = 0;
      memcpy(&v21, txt_file_frametable_parser(&Buf, &v20), 0x7Cu);
      __debugbreak(); // warning C4700: uninitialized local variable 'Str1' used
      if ( v21 && *Str1 != 47 )
      {
        if ( v21 < 2 )
          Error("CTextureFrameTable::load, too few arguments, %s line %i.", Args, v25);
        ++v24;
      }
      ++v25;
    }
    while ( fgets(&Buf, 490, File) );
    v4 = v24;
  }
  v5 = v2->pTextures;
  v2->sNumTextures = v4;
  v6 = malloc(20 * v4);
  v2->pTextures = (TextureFrame *)v6;
  if ( !v6 )
    Error("CTextureFrameTable::load - Out of Memory!");
  v7 = File;
  v2->sNumTextures = 0;
  fseek(v7, 0, 0);
  for ( i = fgets(&Buf, 490, File); i; i = fgets(&Buf, 490, File) )
  {
    *strchr(&Buf, 10) = 0;
    memcpy(&v21, txt_file_frametable_parser(&Buf, &v20), 0x7Cu);
    if ( v21 && *Str1 != 47 )
    {
      strcpy(v2->pTextures[v2->sNumTextures].pTextureName, Str1);
      __debugbreak(); // warning C4700: uninitialized local variable 'Str' used
      v2->pTextures[v2->sNumTextures].uAnimTime = atoi(Str);
      v9 = 2;
      for ( v2->pTextures[v2->sNumTextures].uFlags = 0; v9 < v21; ++v9 )
      {
        if ( !_stricmp((&Str1)[4 * v9], "New") )
        {
          v10 = (int)&v2->pTextures[v2->sNumTextures].uFlags;
          *(char *)v10 |= 2u;
        }
      }
      ++v2->sNumTextures;
    }
  }
  fclose(File);
  v11 = 0;
  if ( (signed int)(v2->sNumTextures - 1) > 0 )
  {
    v12 = 0;
    do
    {
      v13 = (int)&v2->pTextures[v12];
      if ( !(*(char *)(v13 + 38) & 2) )
        *(char *)(v13 + 18) |= 1u;
      ++v11;
      ++v12;
    }
    while ( v11 < (signed int)(v2->sNumTextures - 1) );
  }
  for ( j = 0; j < (signed int)v2->sNumTextures; *(short *)(Argsa + 16) = v16 )
  {
    v15 = v2->pTextures;
    Argsa = (int)&v15[j];
    v16 = *(short *)(Argsa + 14);
    if ( *(char *)(Argsa + 18) & 1 )
    {
      ++j;
      for ( k = (int)&v15[j]; *(char *)(k + 18) & 1; k += 20 )
      {
        v16 += *(short *)(k + 14);
        ++j;
      }
      LOWORD(v16) = v15[j].uAnimTime + v16;
    }
    ++j;
  }
  return 1;
}
