#define _CRT_SECURE_NO_WARNINGS
#include "ErrorHandling.h"
#include "LOD.h"
#include "Render.h"
#include "PaletteManager.h"
#include "Viewport.h"
#include "mm7_data.h"
#include "ZlibWrapper.h"

#include "mm7_data.h"

#include "Sprites.h"






LODFile_IconsBitmaps *pEvents_LOD = nullptr;

LODFile_IconsBitmaps *pIcons_LOD = nullptr;
LODFile_IconsBitmaps *pIcons_LOD_mm6 = nullptr;
LODFile_IconsBitmaps *pIcons_LOD_mm8 = nullptr;

LODFile_IconsBitmaps *pBitmaps_LOD = nullptr;
LODFile_IconsBitmaps *pBitmaps_LOD_mm6 = nullptr;
LODFile_IconsBitmaps *pBitmaps_LOD_mm8 = nullptr;

LODFile_Sprites *pSprites_LOD = nullptr;
LODFile_Sprites *pSprites_LOD_mm6 = nullptr;
LODFile_Sprites *pSprites_LOD_mm8 = nullptr;

LODWriteableFile *pNew_LOD = nullptr;
LODWriteableFile *pGames_LOD = nullptr;




int _6A0CA4_lod_binary_search; // weak
int _6A0CA8_lod_unused; // weak


// inlined
//----- (mm6c::00408860) --------------------------------------------------
void LODFile_IconsBitmaps::_inlined_sub2()
{
  ++uTexturePacksCount;
  if (!uNumPrevLoadedFiles)
    uNumPrevLoadedFiles = uNumLoadedFiles;
}

// inlined
//----- (mm6c::0045BE60) --------------------------------------------------
void LODFile_IconsBitmaps::_inlined_sub1()
{
  dword_11B84 = uNumLoadedFiles;
}

// inlined
//----- (mm6c::0045C310) --------------------------------------------------
void LODFile_Sprites::_inlined_sub1()
{
  field_ECA0 = uNumLoadedSprites;
}

// inlined
//----- (mm6c::0045C5B0) --------------------------------------------------
void LODFile_IconsBitmaps::_inlined_sub0()
{
  dword_11B80 = uNumLoadedFiles;
  if (dword_11B84 < uNumLoadedFiles)
    dword_11B84 = uNumLoadedFiles;
}


// inlined
//----- (mm6c::0045C660) --------------------------------------------------
void LODFile_Sprites::_inlined_sub0()
{
  field_ECA4 = uNumLoadedSprites;
  if (field_ECA0 < uNumLoadedSprites)
    field_ECA0 = uNumLoadedSprites;
}

//----- (004355F7) --------------------------------------------------------
void LODFile_IconsBitmaps::RemoveTexturesFromTextureList()
{
  if ( this->uTexturePacksCount )
  {
    if ( (this->uNumLoadedFiles - 1) >= this->uNumPrevLoadedFiles )
    {
      for ( uint i = this->uNumLoadedFiles - 1; i >= this->uNumPrevLoadedFiles; --i )
      {
        this->pTextures[i].Release();
        if ( this->pHardwareTextures )
        {
          if ( this->pHardwareTextures[i] )
          {
            this->pHardwareTextures[i]->Release();
            this->pHardwareTextures[i] = 0;
          }
        }
        if ( this->pHardwareSurfaces )
        {
          if ( this->pHardwareSurfaces[i] )
          {
            this->pHardwareSurfaces[i]->Release();
            this->pHardwareSurfaces[i] = 0;
          }
        }
      }
    }
    this->uNumLoadedFiles = this->uNumPrevLoadedFiles;
    this->uNumPrevLoadedFiles = 0;
    this->uTexturePacksCount = 0;
  }
}

//----- (004114F2) --------------------------------------------------------
void LODFile_IconsBitmaps::RemoveTexturesPackFromTextureList()
{
  if ( this->uTexturePacksCount )
  {
    this->uTexturePacksCount--;
    if ( !this->uTexturePacksCount )
    {
      if ( (this->uNumLoadedFiles - 1) >= this->uNumPrevLoadedFiles )
      {
        for ( uint i = this->uNumLoadedFiles - 1; i >= this->uNumPrevLoadedFiles; --i )
        {
          this->pTextures[i].Release();
          if ( this->pHardwareTextures )
          {
            if ( this->pHardwareTextures[i] )
            {
              this->pHardwareTextures[i]->Release();
              this->pHardwareTextures[i] = 0;
            }
          }
          if ( this->pHardwareSurfaces )
          {
            if ( this->pHardwareSurfaces[i] )
            {
              this->pHardwareSurfaces[i]->Release();
              this->pHardwareSurfaces[i] = 0;
            }
          }
        }
      }
      this->uNumLoadedFiles = this->uNumPrevLoadedFiles;
      this->uNumPrevLoadedFiles = 0;
    }
  }
}

//----- (004AC67E) --------------------------------------------------------
int LODFile_Sprites::LoadSpriteFromFile(LODSprite *pSpriteHeader, const char *pContainer)
{
  FILE *v3; // eax@1
  FILE *v4; // ebx@1
  int result; // eax@2
  LODSprite *v6; // esi@3
  LODSprite_stru0 *v7; // eax@3
  size_t v8; // ST10_4@3
  int *v9; // ebx@3
  int v10; // eax@3
  void *v11; // eax@5
  LODSprite_stru0 *v12; // eax@6
  void *v13; // ecx@6
  LODSprite_stru0 *i; // edx@6
  FILE *File; // [sp+4h] [bp-4h]@1
  void *DstBufa; // [sp+10h] [bp+8h]@4
  int Sizea; // [sp+14h] [bp+Ch]@3

  v3 = FindContainer(pContainer, 0);
  v4 = v3;
  File = v3;
  if ( v3 )
  {
    v6 = pSpriteHeader;
    fread(pSpriteHeader, 1u, 0x20u, v3);
    strcpy(pSpriteHeader->pName, pContainer);
    Sizea = pSpriteHeader->uSpriteSize;
    v7 = (LODSprite_stru0 *)malloc(8 * v6->uHeight);
    v8 = 8 * pSpriteHeader->uHeight;
    pSpriteHeader->pSpriteLines = v7;
    fread(v7, 1u, v8, v4);

    v9 = &pSpriteHeader->uDecompressedSize;
    v10 = pSpriteHeader->uDecompressedSize;
    if ( v10 )
    {
      pSpriteHeader->pDecompressedBytes = malloc(v10);
      DstBufa = malloc(Sizea);
      fread(DstBufa, 1u, Sizea, File);
      zlib::MemUnzip(v6->pDecompressedBytes, (unsigned int *)&v6->uDecompressedSize, DstBufa, v6->uSpriteSize);
      v6->uSpriteSize = *v9;
      free(DstBufa);
    }
    else
    {
      v11 = malloc(Sizea);
      pSpriteHeader->pDecompressedBytes = v11;
      fread(v11, 1u, Sizea, File);
    }
    v12 = v6->pSpriteLines;
    v13 = v6->pDecompressedBytes;
    for ( i = &v12[v6->uHeight]; v12 < i; i = &v6->pSpriteLines[v6->uHeight] )
    {
      v12->pos = (char *)v12->pos + (unsigned int)v13;
      ++v12;
    }
    result = 1;
  }
  else
  {
    result = -1;
  }
  return result;
}

//----- (004AC795) --------------------------------------------------------
bool LODFile_Sprites::LoadSprites(const char *pFilename)
{
  if (LoadHeader(pFilename, 1))
    return false;
  else
    return LoadSubIndices("sprites08") == 0;
}

//----- (004AC7C0) --------------------------------------------------------
int LODFile_Sprites::LoadSprite(const char *pContainerName, unsigned int uPaletteID)
    {  

    FILE *sprite_file; // eax@12
    LODSprite temp_sprite_hdr; // [sp+Ch] [bp-3Ch]@12
    int i;//, sprite_indx;

    //find if already loaded
    //if ( pRenderer->pRenderD3D )
        {
        for (i=0; i<uNumLoadedSprites;++i)
            {
            if (!(_stricmp(pHardwareSprites[i].pName, pContainerName)))
                return i;
            } 
        }
    /*else
        {
        for (i=0; i<uNumLoadedSprites;++i)
            {
            if (!(_stricmp(pSpriteHeaders[i].pName, pContainerName)))
                return i;
            } 
        }*/

    if (uNumLoadedSprites >= 1500 )
        return -1;
    //if not loaded - load from file   

    //if ( pRenderer->pRenderD3D && can_load_hardware_sprites )
        {
        if ( !pHardwareSprites )
            {
            pHardwareSprites = (Sprite *)malloc(1500*sizeof(Sprite));//0xEA60u
            for (i=0; i<1500;++i)
                {
                pHardwareSprites[i].pName=nullptr;
                pHardwareSprites[i].pTextureSurface=nullptr;
                pHardwareSprites[i].pTexture=nullptr;
                } 
            }
        temp_sprite_hdr.uHeight = 0;
        temp_sprite_hdr.uPaletteId = 0;
        temp_sprite_hdr.word_1A = 0;
        temp_sprite_hdr.pSpriteLines = nullptr;
        temp_sprite_hdr.pDecompressedBytes = nullptr;
        sprite_file = FindContainer(pContainerName, 0);
        if ( !sprite_file )
            return -1;
        //fread(&temp_sprite_hdr, 1, sizeof(LODSprite), sprite_file);
        fread(&temp_sprite_hdr, 1, 0x20, sprite_file);
        pHardwareSprites[uNumLoadedSprites].uBufferWidth = temp_sprite_hdr.uWidth;
        pHardwareSprites[uNumLoadedSprites].uBufferHeight = temp_sprite_hdr.uHeight;
        pSpriteHeaders[uNumLoadedSprites].uWidth = temp_sprite_hdr.uWidth;
        pSpriteHeaders[uNumLoadedSprites].uHeight = temp_sprite_hdr.uHeight;
        LoadSpriteFromFile( &pSpriteHeaders[uNumLoadedSprites], pContainerName);        //this line is not present here in the original. necessary for Grayface's mouse picking fix
        }
    /*else
        {
        sprite_indx = LoadSpriteFromFile( &pSpriteHeaders[uNumLoadedSprites], pContainerName);
        pSpriteHeaders[uNumLoadedSprites].word_1A = 0;

        if ( sprite_indx != -1 )
            {
            pSpriteHeaders[uNumLoadedSprites].uPaletteId = pPaletteManager->LoadPalette(pSpriteHeaders[uNumLoadedSprites].uPaletteId);
            }
        else
            {
            if ( uNumLoadedSprites<=0 )
                uNumLoadedSprites=0;
            else
                {
                for (i=0; i<uNumLoadedSprites;++i)
                    {
                    if (!(_stricmp(pSpriteHeaders[i].pName, "pending")))
                        return i;
                    } 
                }
            if ( LoadSpriteFromFile(&pSpriteHeaders[uNumLoadedSprites], "pending") == -1 )
                return -1;
            pSpriteHeaders[uNumLoadedSprites].uPaletteId = pPaletteManager->LoadPalette(pSpriteHeaders[uNumLoadedSprites].uPaletteId);
            }
        }*/

    //if ( pRenderer->pRenderD3D )
        {
        pHardwareSprites[uNumLoadedSprites].pName = (const char *)malloc(20);
        strcpy((char *)pHardwareSprites[uNumLoadedSprites].pName, pContainerName);
        pHardwareSprites[uNumLoadedSprites].uPaletteID = uPaletteID;
        pRenderer->MoveSpriteToDevice(&pHardwareSprites[uNumLoadedSprites]);
        }
    ++uNumLoadedSprites;
    return uNumLoadedSprites - 1;

   }

//----- (004ACADA) --------------------------------------------------------
void LODFile_Sprites::ReleaseLostHardwareSprites()
{
  LODFile_Sprites *v1; // esi@1
  signed int v2; // ebx@2
  int v3; // edi@3
  IDirectDrawSurface *v4; // eax@4
  IDirect3DTexture2 *v5; // eax@6
  IDirectDrawSurface *v6; // ST00_4@8

  v1 = this;
  if ( this->pHardwareSprites )
  {
    v2 = 0;
    if ( (signed int)this->uNumLoadedSprites > 0 )
    {
      v3 = 0;
      do
      {
        v4 = (IDirectDrawSurface *)v1->pHardwareSprites[v3].pTextureSurface;
        if ( v4 && v4->IsLost() == DDERR_SURFACELOST )
        {
          v5 = v1->pHardwareSprites[v3].pTexture;
          if ( v5 )
          {
            v5->Release();
            v1->pHardwareSprites[v3].pTexture = nullptr;
          }
          v6 = (IDirectDrawSurface *)v1->pHardwareSprites[v3].pTextureSurface;
          v6->Release();
          v1->pHardwareSprites[v3].pTextureSurface = nullptr;
          pRenderer->MoveSpriteToDevice(&v1->pHardwareSprites[v3]);
        }
        ++v2;
        ++v3;
      }
      while ( v2 < (signed int)v1->uNumLoadedSprites );
    }
  }
}

//----- (004ACB70) --------------------------------------------------------
void LODFile_Sprites::ReleaseAll()
{
  LODFile_Sprites *v1; // esi@1
  signed int v2; // ebx@2
  int v3; // edi@3
  Sprite *v4; // eax@4
  IDirect3DTexture2 *v5; // eax@5
  Sprite *v6; // eax@7
  IDirectDrawSurface *v7; // eax@8

  v1 = this;
  if ( this->pHardwareSprites )
  {
    v2 = 0;
    if ( (signed int)this->uNumLoadedSprites > 0 )
    {
      v3 = 0;
      do
      {
        v4 = v1->pHardwareSprites;
        if ( v4 )
        {
          v5 = v4[v3].pTexture;
          if ( v5 )
          {
            v5->Release();
            v1->pHardwareSprites[v3].pTexture = nullptr;
          }
          v6 = v1->pHardwareSprites;
          if ( v6 )
          {
            v7 = (IDirectDrawSurface *)v6[v3].pTextureSurface;
            if ( v7 )
            {
              v7->Release();
              v1->pHardwareSprites[v3].pTextureSurface = nullptr;
            }
          }
        }
        ++v2;
        ++v3;
      }
      while ( v2 < (signed int)v1->uNumLoadedSprites );
    }
  }
}

//----- (004ACBE0) --------------------------------------------------------
void LODFile_Sprites::MoveSpritesToVideoMemory()
{
  LODFile_Sprites *v1; // esi@1
  int v2; // ebx@1
  signed int v3; // edi@2

  v1 = this;
  v2 = 0;
  if ( this->pHardwareSprites )
  {
    v3 = 0;
    if ( (signed int)this->uNumLoadedSprites > 0 )
    {
      do
      {
        pRenderer->MoveSpriteToDevice(&v1->pHardwareSprites[v2]);
        ++v3;
        ++v2;
      }
      while ( v3 < (signed int)v1->uNumLoadedSprites );
    }
  }
}

//----- (004ACC38) --------------------------------------------------------
int LODSprite::DrawSprite_sw(RenderBillboardTransform_local0 *a2, char a3)
{
  RenderBillboardTransform_local0 *v3; // edi@1
  int result; // eax@1
  int v5; // esi@2
  int v6; // ST18_4@2
  signed int v7; // eax@2
  signed int v8; // ebx@2
  int v9; // ebx@2
  int *v10; // ecx@2
  int v11; // esi@2
  unsigned int v12; // edx@4
  int v13; // esi@13
  int v14; // esi@17
  int v15; // ecx@17
  char *v16; // edx@17
  int v17; // esi@17
  int v18; // ecx@18
  int v19; // esi@18
  LODSprite_stru0 *v20; // edx@21
  int v21; // eax@22
  int v22; // esi@22
  int v23; // eax@25
  int v24; // ecx@25
  signed __int64 v25; // qtt@27
  int v26; // eax@27
  unsigned __int16 *v27; // eax@29
  LODSprite_stru0 *v28; // edx@29
  signed int v29; // ecx@30
  int v30; // ecx@37
  int v31; // ecx@38
  signed int v32; // ecx@41
  int v33; // ecx@47
  int v34; // ecx@56
  int v35; // esi@58
  __int16 v36; // ax@58
  int v37; // ecx@59
  int v38; // eax@59
  int v39; // ecx@62
  signed int v40; // ST30_4@64
  signed __int64 v41; // qtt@64
  int v42; // ecx@64
  unsigned __int16 *v43; // eax@66
  LODSprite_stru0 *v44; // ecx@66
  int v45; // edx@69
  int v46; // edx@77
  //unsigned __int16 *pTarget; // [sp+Ch] [bp-50h]@2
  signed int v48; // [sp+10h] [bp-4Ch]@2
  signed int v49; // [sp+14h] [bp-48h]@2
  int v50; // [sp+14h] [bp-48h]@19
  int v51; // [sp+14h] [bp-48h]@57
  int v52; // [sp+18h] [bp-44h]@13
  int v53; // [sp+1Ch] [bp-40h]@2
  int v54; // [sp+1Ch] [bp-40h]@22
  int v55; // [sp+1Ch] [bp-40h]@32
  int v56; // [sp+1Ch] [bp-40h]@69
  int v57; // [sp+20h] [bp-3Ch]@2
  int v58; // [sp+24h] [bp-38h]@1
  int v59; // [sp+28h] [bp-34h]@2
  int v60; // [sp+28h] [bp-34h]@13
  unsigned __int16 *v61; // [sp+2Ch] [bp-30h]@2
  int v62; // [sp+30h] [bp-2Ch]@2
  void *v63; // [sp+30h] [bp-2Ch]@29
  void *v64; // [sp+30h] [bp-2Ch]@66
  int v65; // [sp+34h] [bp-28h]@2
  int v66; // [sp+34h] [bp-28h]@22
  int v67; // [sp+34h] [bp-28h]@59
  int v68; // [sp+38h] [bp-24h]@13
  unsigned int v69; // [sp+3Ch] [bp-20h]@2
  int v70; // [sp+40h] [bp-1Ch]@2
  signed int v71; // [sp+40h] [bp-1Ch]@15
  int v72; // [sp+44h] [bp-18h]@2
  unsigned __int16 *v73; // [sp+44h] [bp-18h]@29
  unsigned __int16 *v74; // [sp+44h] [bp-18h]@66
  int v75; // [sp+48h] [bp-14h]@4
  int v76; // [sp+48h] [bp-14h]@22
  int v77; // [sp+48h] [bp-14h]@59
  //LODSprite *v78; // [sp+4Ch] [bp-10h]@1
  int v79; // [sp+50h] [bp-Ch]@4
  int v80; // [sp+50h] [bp-Ch]@21
  int v81; // [sp+50h] [bp-Ch]@62
  int v82; // [sp+50h] [bp-Ch]@67
  int v83; // [sp+50h] [bp-Ch]@75
  int *pTargetZ; // [sp+54h] [bp-8h]@4
  int v85; // [sp+58h] [bp-4h]@18
  int v86; // [sp+58h] [bp-4h]@56
  signed int v87; // [sp+64h] [bp+8h]@2
  int v88; // [sp+68h] [bp+Ch]@18
  int v89; // [sp+68h] [bp+Ch]@56

  v3 = a2;
  //v78 = this;
  result = a2->_screenspace_x_scaler_packedfloat;
  v58 = a2->_screenspace_x_scaler_packedfloat;
  if ( result <= 0 )
    return result;
  v5 = a2->_screenspace_y_scaler_packedfloat;
  v6 = a2->_screenspace_x_scaler_packedfloat;
  v87 = (signed __int64)0x100000000ui64 / result;
  v48 = (signed __int64)0x100000000ui64 / result;
  v62 = (signed __int64)0x100000000ui64 / v5;
  v7 = this->uHeight;
  v8 = (signed int)((signed __int64)0x100000000ui64 / v5) >> 1;
  v53 = v8;
  v70 = (this->uHeight << 16) - v8;
  v49 = v7;
  v69 = v3->uTargetPitch;

  __debugbreak(); // target surface  will most likely be 32bit, but this sub awaits 16bits
  auto pTarget = (unsigned __int16 *)v3->pTarget;
  v57 = v3->sZValue;
  v61 = v3->pPalette;
  v9 = (v6 * this->uWidth + 0x8000) >> 16;
  v72 = v3->uScreenSpaceY;
  result = (v5 * v7 + 0x8000) >> 16;
  v10 = (int *)(v72 - result + 1);
  v11 = v3->uScreenSpaceX - (v9 >> 1) + 1;
  v65 = v72 - result + 1;
  v59 = v11 + v9 - 1;
  if ( v3->uFlags & 0x800 )
  {
    v10 = (int *)((char *)v10 + (v49 >> 1));
    v72 += v49 >> 1;
    v65 = (int)v10;
  }
  v12 = v72;
  pTargetZ = v10;
  v75 = v3->uScreenSpaceX - (v9 >> 1) + 1;
  v79 = v11 + v9 - 1;
  if ( !(v3->uFlags & 8) )
  {
    if ( v65 < (signed int)v3->uViewportY )
      pTargetZ = (int *)v3->uViewportY;
    if ( v72 > (signed int)v3->uViewportW )
      v12 = v3->uViewportW;
    if ( v11 < (signed int)v3->uViewportX )
      v75 = v3->uViewportX;
    if ( v59 > (signed int)v3->uViewportZ )
      v79 = v3->uViewportZ;
  }
  v68 = v75 - v11;
  v13 = -v62;
  v60 = v59 - v79;
  v52 = -v62;
  if ( v3->uFlags & 1 )
  {
    v13 = v62;
    v70 = v53;
    v52 = v62;
  }
  v71 = v13 * (v72 - v12) + v70;
  if ( LOBYTE(viewparams->field_20) )
  {
    if ( a3 )
      return result;
  }
  v14 = 5 * v12;
  v15 = v69 * v12;
  result = v12 - v72 + result - 1;
  v16 = (char *)pTargetZ - v65;
  v17 = v14 << 7;
  if ( v3->uFlags & 4 )
  {
    v34 = v79 + v15;
    v89 = v34;
    v86 = v79 + v17;
    if ( result < (signed int)v16 )
      return result;
    v51 = result - (int)v16 + 1;
    while ( 1 )
    {
      v35 = v71 >> 16;
      v36 = this->pSpriteLines[v35].a1;
      if ( v36 == -1 )
      {
        v34 -= v69;
        v89 = v34;
        goto LABEL_84;
      }
      v37 = v9 - ((unsigned __int64)(v36 * (signed __int64)v58) >> 16);
      v67 = v87 * ((unsigned __int64)(this->pSpriteLines[v35].a2 * (signed __int64)v58) >> 16);
      v38 = v9 - v60;
      v77 = v9 - v60;
      if ( v9 - v60 <= (signed int)(v9 - ((unsigned __int64)(this->pSpriteLines[v35].a2 * (signed __int64)v58) >> 16))
        || v68 >= v37 )
      {
        v89 -= v69;
        v34 = v89;
LABEL_84:
        v86 -= window->GetWidth();
        goto LABEL_85;
      }
      if ( v38 < v37 )
        v81 = (v87 >> 1) + v87 * (v37 - v38);
      else
      {
        v77 = v37;
        v81 = v87 >> 1;
        v39 = v37 - v9;
        v89 += v39 + v60;
        v86 += v60 + v39;
      }
      v40 = ((this->pSpriteLines[v35].a2 + 1) << 16) - v81 - v67;
      LODWORD(v41) = v40 << 16;
      HIDWORD(v41) = v40 >> 16;
      v42 = v77 - (((signed int)((unsigned __int64)(v41 / v48) - 0x8000) >> 16) + 1);
      if ( v68 >= v42 )
        v42 = v68;
      v43 = &pTarget[v89];
      v74 = &v43[v42 - v77 + 1];
      v44 = &this->pSpriteLines[v35];
      v64 = v44->pos;
      if ( !v57 )
      {
        v83 = v67 + v81;
        if ( ((v83 - (v44->a1 << 16)) & 0xFFFF0000) < 0 )
        {
          v83 += v87;
          --v43;
          --pTargetZ;
        }
        while ( v43 >= v74 )
        {
          v46 = (v83 - ((signed int)this->pSpriteLines[v35].a1 << 16)) >> 16;
          if ( *((char *)v64 + v46) )
            *v43 = v61[*((char *)v64 + v46)];
          v83 += v87;
          --v43;
        }
        goto LABEL_81;
      }
      pTargetZ = &v3->pTargetZ[v86];
      v82 = v67 + v81;
      if ( ((v82 - (v44->a1 << 16)) & 0xFFFF0000) < 0 )
        goto LABEL_72;
LABEL_73:
      if ( v43 >= v74 )
        break;
LABEL_81:
      v89 += v9 - v77 - v60 - v69;
      v34 = v89;
      v86 = v86 + v9 - v77 - v60 - window->GetWidth();
LABEL_85:
      result = v52;
      v71 += v52;
      --v51;
      if ( !v51 )
        return result;
    }
    v45 = (v82 - ((signed int)this->pSpriteLines[v35].a1 << 16)) >> 16;
    v56 = *((char *)v64 + v45);
    if ( *((char *)v64 + v45) && v57 <= (unsigned int)*pTargetZ )
    {
      *pTargetZ = v57;
      *v43 = v61[v56];
    }
LABEL_72:
    v82 += v87;
    --v43;
    --pTargetZ;
    goto LABEL_73;
  }
  v18 = v75 + v15;
  v19 = v75 + v17;
  v88 = v18;
  v85 = v19;
  if ( result >= (signed int)v16 )
  {
    v50 = result - (int)v16 + 1;
    while ( 1 )
    {
      v20 = &this->pSpriteLines[v71 >> 16];
      v80 = v71 >> 16;
      if ( v20->a1 != -1 )
        break;
      v18 -= v69;
      v85 = v19 - window->GetWidth();
      v88 = v18;
LABEL_54:
      result = v52;
      v71 += v52;
      --v50;
      if ( !v50 )
        return result;
      v19 = v85;
    }
    v21 = (v58 * v20->a1 + 32768) >> 16;
    v66 = v21 * v87;
    v76 = v68;
    v54 = v20->a2;
    v22 = v9 - v60;
    if ( v68 >= (v58 * v54 + 32768) >> 16 || v22 <= v21 )
    {
      v88 -= v69;
      v85 -= window->GetWidth();
      goto LABEL_51;
    }
    if ( v68 > v21 )
    {
      v24 = (v87 >> 1) + v87 * (v68 - v21);
    }
    else
    {
      v76 = (v58 * v20->a1 + 0x8000) >> 16;
      v23 = v21 - v68;
      v88 += v23;
      v24 = v87 >> 1;
      v85 += v23;
    }
    LODWORD(v25) = (((v54 + 1) << 16) - v24 - v66) << 16;
    HIDWORD(v25) = (((v54 + 1) << 16) - v24 - v66) >> 16;
    v26 = v76 + ((signed int)(v25 / v48) >> 16) + 1;
    if ( v22 > v26 )
      v22 = v26;
    v27 = &pTarget[v88];
    v73 = &v27[v22 - v76 - 1];
    v28 = &this->pSpriteLines[v80];
    v63 = v28->pos;
    if ( v57 )
    {
      pTargetZ = &v3->pTargetZ[v85];
      v29 = v66 - (v28->a1 << 16) + v24;
      if ( (v29 & 0xFFFF0000) >= 0 )
        goto LABEL_36;
      while ( 1 )
      {
        v29 += v87;
        ++v27;
        ++pTargetZ;
LABEL_36:
        if ( v27 >= v73 )
          break;
        v55 = *((char *)v63 + (v29 >> 16));
        if ( *((char *)v63 + (v29 >> 16)) && v57 <= (unsigned int)*pTargetZ )
        {
          *pTargetZ = v57;
          *v27 = v61[v55];
        }
      }
      v30 = v29 >> 16;
      if ( v30 > this->pSpriteLines[v80].a2 - (signed int)this->pSpriteLines[v80].a1
        || (v31 = *((char *)v63 + v30)) == 0
        || v57 > (unsigned int)*pTargetZ )
        goto LABEL_50;
      *pTargetZ = v57;
    }
    else
    {
      v32 = v66 - (v28->a1 << 16) + v24;
      if ( (v32 & 0xFFFF0000) < 0 )
      {
        v32 += v87;
        ++v27;
        ++pTargetZ;
      }
      while ( v27 < v73 )
      {
        if ( *((char *)v63 + (v32 >> 16)) )
          *v27 = v61[*((char *)v63 + (v32 >> 16))];
        v32 += v87;
        ++v27;
      }
      v33 = v32 >> 16;
      if ( v33 > this->pSpriteLines[v80].a2 - (signed int)this->pSpriteLines[v80].a1
        || (v31 = *((char *)v63 + v33)) == 0 )
        goto LABEL_50;
    }
    *v27 = v61[v31];
LABEL_50:
    v88 += v68 - v76 - v69;
    v85 = v85 + v68 - v76 - window->GetWidth();
LABEL_51:
    v18 = v88;
    goto LABEL_54;
  }
  return result;
}

//----- (004AD2D1) --------------------------------------------------------
int LODSprite::_4AD2D1(struct RenderBillboardTransform_local0 *a2, int a3)
{
  int result; // eax@1
  unsigned int v4; // esi@1
  int v5; // edi@1
  LODSprite_stru0 *v6; // edx@2
  __int16 v7; // bx@2
  int v8; // ecx@3
  unsigned __int16 *v9; // esi@3
  int v10; // ebx@3
  void *v11; // edx@3
  unsigned __int16 *v12; // ecx@3
  int v13; // ebx@4
  LODSprite *v14; // [sp+8h] [bp-10h]@1
  unsigned __int16 *v15; // [sp+10h] [bp-8h]@1
  unsigned __int16 *v16; // [sp+14h] [bp-4h]@1
  int i; // [sp+20h] [bp+8h]@1

  result = (int)a2;
  v14 = this;
  v4 = a2->uTargetPitch;

  __debugbreak(); // sub expects 16bit target surface, we may have 32bit
  v16 = (unsigned short *)a2->pTarget;
  v15 = a2->pPalette;
  v5 = this->uHeight - 1;
  for ( i = v4 * a2->uScreenSpaceY - (this->uWidth >> 1) + a2->uScreenSpaceX + 1; v5 >= 0; --v5 )
  {
    v6 = &this->pSpriteLines[v5];
    v7 = v6->a1;
    if ( v6->a1 != -1 )
    {
      v8 = v7;
      v9 = &v16[v7 + i];
      v10 = v6->a2;
      v11 = v6->pos;
      v12 = &v9[v10 - v8];
      while ( v9 <= v12 )
      {
        v13 = *(char *)v11;
        v11 = (char *)v11 + 1;
        if ( v13 )
          *v9 = v15[v13];
        ++v9;
      }
      v4 = *(int *)(result + 48);
      //this = v14;
    }
    i -= v4;
  }
  return result;
}

//----- (0046454B) --------------------------------------------------------
void LODFile_IconsBitmaps::ReleaseAll2()
{
  LODFile_IconsBitmaps *v1; // esi@1
  int v2; // edi@1
  Texture *v3; // ebx@2
  struct IDirect3DTexture2 **v4; // eax@3
  struct IDirect3DTexture2 *v5; // eax@4
  struct IDirectDrawSurface **v6; // eax@6
  struct IDirectDrawSurface *v7; // eax@7
  int v8; // eax@10

  v1 = this;
  v2 = this->uNumLoadedFiles - 1;
  if ( v2 >= this->dword_11B84 )
  {
    v3 = &this->pTextures[v2];
    do
    {
      v3->Release();
      v4 = v1->pHardwareTextures;
      if ( v4 )
      {
        v5 = v4[v2];
        if ( v5 )
        {
          v5->Release();
          v1->pHardwareTextures[v2] = 0;
        }
      }
      v6 = v1->pHardwareSurfaces;
      if ( v6 )
      {
        v7 = v6[v2];
        if ( v7 )
        {
          v7->Release();
          v1->pHardwareSurfaces[v2] = 0;
        }
      }
      --v2;
      --v3;
    }
    while ( v2 >= v1->dword_11B84 );
  }
  v8 = v1->dword_11B84;
  v1->uTexturePacksCount = 0;
  v1->uNumPrevLoadedFiles = 0;
  v1->uNumLoadedFiles = v8;
}

//----- (004645DC) --------------------------------------------------------
void LODFile_Sprites::DeleteSomeOtherSprites()
{
  int *v1; // esi@1
  int *v2; // edi@1

  v1 = (int *)&this->uNumLoadedSprites;
  v2 = &this->field_ECA0;
  DeleteSpritesRange(field_ECA0, uNumLoadedSprites);
  *v1 = *v2;
}


//----- (00461431) --------------------------------------------------------
void LOD::File::Close()
{
  if (isFileOpened )
  {
    this->pContainerName[0] = 0;
    this->uCurrentIndexDir = 0;
    free(pSubIndices);
    free(pRoot);
    pSubIndices = nullptr;
    pRoot = nullptr;
    fclose(pFile);
    isFileOpened = false;
    _6A0CA8_lod_unused = 0;
  }
}



//----- (00461492) --------------------------------------------------------
int LODWriteableFile::CreateNewLod(LOD::FileHeader *pHeader, LOD::Directory *pDir, const char *lod_name)
{
  if (isFileOpened)
    return 1;
  if ( !pDir->pFilename[0] )
    return 2;
  strcpy(pHeader->pSignature, "LOD");
  pHeader->LODSize = 100;
  pHeader->uNumIndices = 1;
  pDir->field_F = 0;
  pDir->uDataSize = 0;
  pDir->uOfsetFromSubindicesStart = 288;
  strcpy(pLODName, lod_name);

  pFile = fopen(pLODName, "wb+");
  if (!pFile)
    return 3;
  fwrite(pHeader,sizeof(LOD::FileHeader), 1, pFile);
  fwrite(pDir, sizeof(LOD::Directory), 1, pFile);
  fclose(pFile);
  pFile = nullptr;
  return 0;
}

//----- (0046153F) --------------------------------------------------------
void LOD::File::ResetSubIndices()
{
  if ( isFileOpened )
  {
    pContainerName[0] = 0;
    uCurrentIndexDir = 0;
    uOffsetToSubIndex = 0;
    uNumSubDirs = 0;
    uLODDataSize = 0;
    free(pSubIndices);
    pSubIndices = nullptr;
  }
}

//----- (00450C8B) --------------------------------------------------------
void LODFile_Sprites::DeleteSomeSprites()
{
  int *v1; // esi@1
  int *v2; // edi@1

  v1 = (int *)&this->uNumLoadedSprites;
  v2 = &this->field_ECA8;
  DeleteSpritesRange(this->field_ECA8, this->uNumLoadedSprites);
  *v1 = *v2;
}

//----- (00450CA9) --------------------------------------------------------
void LODFile_Sprites::DeleteSpritesRange(int uStartIndex, int uStopIndex)
{
  LODFile_Sprites *v3; // edi@1
  int v4; // esi@3
  LODSprite *v5; // ebx@3
  LODSprite *v6; // esi@7
  int v7; // edi@7
  int a2a; // [sp+10h] [bp+8h]@3

  v3 = this;
  if ( this->pHardwareSprites )
  {
    if ( uStartIndex < uStopIndex )
    {
      v4 = uStartIndex;
      v5 = &this->pSpriteHeaders[uStartIndex];
      a2a = uStopIndex - uStartIndex;
      do
      {
        v5->Release();
        pHardwareSprites[v4].Release();
        ++v4;
        ++v5;
        --a2a;
      }
      while ( a2a );
    }
  }
  else
  {
    if ( uStartIndex < uStopIndex )
    {
      v6 = &this->pSpriteHeaders[uStartIndex];
      v7 = uStopIndex - uStartIndex;
      do
      {
        v6->Release();
        ++v6;
        --v7;
      }
      while ( v7 );
    }
  }
}

//----- (00450D1D) --------------------------------------------------------
void LODSprite::Release()
{
  LODSprite *v1; // esi@1

  v1 = this;
  if ( !(HIBYTE(this->word_1A) & 4) )
  {
    free(this->pDecompressedBytes);
    free(v1->pSpriteLines);
  }
  v1->word_1A = 0;
  v1->pDecompressedBytes = nullptr;
  v1->pSpriteLines = nullptr;
  v1->pName[0] = 0;
  v1->word_16 = 0;
  v1->uPaletteId = 0;
  v1->uTexturePitch = 0;
  v1->uHeight = 0;
  v1->uWidth = 0;
  v1->uSpriteSize = 0;
}

//----- (00450D68) --------------------------------------------------------
void Sprite::Release()
{
  free((void *)pName);
  pName = nullptr;

  if (pTextureSurface)
    pTextureSurface->Release();
  pTextureSurface = nullptr;

  if (pTexture)
    pTexture->Release();
  pTexture = nullptr;
}




//----- (0040FAEE) --------------------------------------------------------
//----- (0040FA2E) --------------------------------------------------------
bool LODFile_IconsBitmaps::Load(const char *pLODFilename, const char *pFolderName)
{
  ReleaseAll();

  if (LoadHeader(pLODFilename, 1))
    return false;

  return LoadSubIndices(pFolderName) == 0;
}


//----- (0040FA60) --------------------------------------------------------
void LODFile_IconsBitmaps::ReleaseAll()
{
  LODFile_IconsBitmaps *v1; // esi@1
  unsigned int v2; // edi@1
  Texture *v3; // ebp@2
  struct IDirect3DTexture2 **v4; // eax@3
  struct IDirect3DTexture2 *v5; // eax@4
  struct IDirectDrawSurface **v6; // eax@6
  struct IDirectDrawSurface *v7; // eax@7

  v1 = this;
  v2 = this->uNumLoadedFiles - 1;
  if ( (v2 & 0x80000000u) == 0 )
  {
    v3 = &this->pTextures[v2];
    do
    {
      v3->Release();
      v4 = v1->pHardwareTextures;
      if ( v4 )
      {
        v5 = v4[v2];
        if ( v5 )
        {
          v5->Release();
          v1->pHardwareTextures[v2] = 0;
        }
      }
      v6 = v1->pHardwareSurfaces;
      if ( v6 )
      {
        v7 = v6[v2];
        if ( v7 )
        {
          v7->Release();
          v1->pHardwareSurfaces[v2] = 0;
        }
      }
      --v2;
      --v3;
    }
    while ( (v2 & 0x80000000u) == 0 );
  }
  v1->uTexturePacksCount = 0;
  v1->uNumPrevLoadedFiles = 0;
  v1->dword_11B84 = 0;
  v1->dword_11B80 = 0;
  v1->uNumLoadedFiles = 0;
}

//----- (0040F9F0) --------------------------------------------------------
unsigned int LODFile_IconsBitmaps::FindTextureByName(const char *pName)
{
  LODFile_IconsBitmaps *v2; // esi@1
  unsigned int v3; // edi@1
  Texture *v4; // ebx@2
  unsigned int result; // eax@5

  v2 = this;
  v3 = 0;
  if ( (signed int)this->uNumLoadedFiles <= 0 )
  {
LABEL_5:
    result = -1;
  }
  else
  {
    v4 = this->pTextures;
    while ( _stricmp(v4->pName, pName) )
    {
      ++v3;
      ++v4;
      if ( (signed int)v3 >= (signed int)v2->uNumLoadedFiles )
        goto LABEL_5;
    }
    result = v3;
  }
  return result;
}

//----- (0040F9C5) --------------------------------------------------------
void LODFile_IconsBitmaps::SyncLoadedFilesCount()
    {
  signed int loaded_files; // eax@1
  Texture *pTex; // edx@1

  loaded_files = this->uNumLoadedFiles;
  for ( pTex = &this->pTextures[loaded_files]; !pTex->pName[0]; --pTex )
    --loaded_files;
  if ( loaded_files < (signed int)this->uNumLoadedFiles )
  {
    ++loaded_files;
    this->uNumLoadedFiles = loaded_files;
  }
 
}



//----- (0046249B) --------------------------------------------------------
LODFile_Sprites::~LODFile_Sprites()
{
  LODFile_Sprites *v1; // esi@1
  signed int v2; // ebx@1
  LODSprite *v3; // edi@3
  char *v4; // edi@6
  int thisa; // [sp+4h] [bp-10h]@3
  LODSprite *thisb; // [sp+4h] [bp-10h]@7

  v1 = this;
  v2 = 0;
  if ( this->pHardwareSprites )
  {
    if ( (signed int)this->uNumLoadedSprites > 0 )
    {
      thisa = 0;
      v3 = this->pSpriteHeaders;
      do
      {
        v3->Release();
        v1->pHardwareSprites[thisa].Release();
        ++thisa;
        ++v2;
        ++v3;
      }
      while ( v2 < (signed int)v1->uNumLoadedSprites );
    }
  }
  else
  {
    v4 = (char *)&this->uNumLoadedSprites;
    if ( (signed int)this->uNumLoadedSprites > 0 )
    {
      thisb = this->pSpriteHeaders;
      do
      {
        thisb->Release();
        ++thisb;
        ++v2;
      }
      while ( v2 < *(int *)v4 );
    }
  }
  //_eh_vector_destructor_iterator_(v1->pSpriteHeaders, 40, 1500, LODSprite::dtor);
  //LOD::File::vdtor((LOD::File *)v1);
}
// 4CC2B4: using guessed type int __stdcall _eh vector destructor iterator_(int, int, int, int);


//----- (00462463) --------------------------------------------------------
LODSprite::~LODSprite()
{
  LODSprite *v1; // esi@1

  v1 = this;
  if ( !(HIBYTE(this->word_1A) & 4) )
  {
    free(pDecompressedBytes);
    free(pSpriteLines);
  }
  pDecompressedBytes = nullptr;
  pSpriteLines = nullptr;
}

//----- (004623E5) --------------------------------------------------------
LODFile_Sprites::LODFile_Sprites():
  LOD::File()
{
  /*_eh_vector_constructor_iterator_(
    v1->pSpriteHeaders,
    40,
    1500,
    (void ( *)(void *))LODSprite::LODSprite,
    (void ( *)(void *))LODSprite::dtor);*/
  field_ECA4 = 0;
  field_ECA0 = 0;
  pHardwareSprites = 0;
  //can_load_hardware_sprites = 0;
  field_ECB4 = 0;
  uNumLoadedSprites = 0;
}

//----- (00462303) --------------------------------------------------------
LODFile_IconsBitmaps::~LODFile_IconsBitmaps()
{
  LODFile_IconsBitmaps *v1; // esi@1
  unsigned int v2; // edi@1
  struct IDirect3DTexture2 **v3; // eax@3
  struct IDirect3DTexture2 *v4; // eax@4
  struct IDirectDrawSurface **v5; // eax@6
  struct IDirectDrawSurface *v6; // eax@7
  Texture *thisa; // [sp+4h] [bp-10h]@2

  v1 = this;
  v2 = this->uNumLoadedFiles - 1;
  if ( (v2 & 0x80000000u) == 0 )
  {
    thisa = &this->pTextures[v2];
    do
    {
      thisa->Release();
      v3 = v1->pHardwareTextures;
      if ( v3 )
      {
        v4 = v3[v2];
        if ( v4 )
        {
          v4->Release();
          v1->pHardwareTextures[v2] = 0;
        }
      }
      v5 = v1->pHardwareSurfaces;
      if ( v5 )
      {
        v6 = v5[v2];
        if ( v6 )
        {
          v6->Release();
          v1->pHardwareSurfaces[v2] = 0;
        }
      }
      --thisa;
      --v2;
    }
    while ( (v2 & 0x80000000u) == 0 );
  }
  free(v1->pHardwareSurfaces);
  free(v1->pHardwareTextures);
  free(v1->ptr_011BB4);
  //LOD::File::vdtor((LOD::File *)v1);
}

//----- (00462272) --------------------------------------------------------
LODFile_IconsBitmaps::LODFile_IconsBitmaps():
  LOD::File()
{
  LODFile_IconsBitmaps *v1; // esi@1
//  Texture *v2; // ebx@1
//  signed int v3; // [sp+4h] [bp-10h]@1

  v1 = this;
  /*v2 = v1->pTextures;
  v3 = 1000;
  do
  {
    Texture::Texture(v2);
    ++v2;
    --v3;
  }
  while ( v3 );*/
  v1->uTexturePacksCount = 0;
  v1->uNumPrevLoadedFiles = 0;
  v1->dword_11B84 = 0;
  v1->dword_11B80 = 0;
  v1->uNumLoadedFiles = 0;
  v1->dword_011BA4 = 0;
  //v1->can_load_hardware_sprites = 0;
  v1->pHardwareSurfaces = 0;
  v1->pHardwareTextures = 0;
  v1->ptr_011BB4 = 0;
}


//----- (004621A7) --------------------------------------------------------
bool LODWriteableFile::_4621A7()//закрыть и загрузить записываемый ф-л(при сохранении)
{
  CloseWriteFile();
  return LoadFile(pLODName, 0);
}

//----- (00461FD4) ---LODFile_sub_461FD4---text:004632EA  --------------------------------------------------
int LODWriteableFile::FixDirectoryOffsets()
{
  int total_size; // edi@1
  int temp_offset; // ecx@5
  FILE *tmp_file; // eax@9
  size_t write_size; // edi@12
  int result;
  char Filename[256]; // [sp+Ch] [bp-228h]@9
  char NewFilename[256]; // [sp+10Ch] [bp-128h]@15
  int i;

  total_size = 0;
  for ( i = 0; i < uNumSubDirs; i++ )
    total_size += pSubIndices[i].uDataSize;
  //fix offsets
  temp_offset = sizeof(LOD::Directory) * uNumSubDirs;
  for ( i = 0; i < uNumSubDirs; i++ )
  {
    pSubIndices[i].uOfsetFromSubindicesStart=temp_offset;
    temp_offset+=pSubIndices[i].uDataSize;
  }
  strcpy(Filename, "lod.tmp");
  tmp_file = fopen(Filename, "wb+");

  if ( tmp_file )
  {
    fwrite((const void *)&header, sizeof(LOD::FileHeader), 1, tmp_file);

    LOD::Directory Lindx;
    strcpy(Lindx.pFilename, "chapter");
    Lindx.uOfsetFromSubindicesStart = uOffsetToSubIndex; //10h 16
    Lindx.uDataSize = sizeof(LOD::Directory) * uNumSubDirs + total_size;		   //14h 20
    Lindx.dword_000018 = 0;		   //18h 24 
    Lindx.uNumSubIndices = uNumSubDirs;		   //1ch 28
    Lindx.word_00001E = 0;		   // 1Eh 30
    fwrite(&Lindx, sizeof(LOD::Directory), 1, tmp_file);
    fwrite(pSubIndices, sizeof(LOD::Directory), uNumSubDirs, tmp_file);
    fseek(pOutputFileHandle, 0, 0);
    if ( total_size > 0 )
    {
      do
      {
        write_size = uIOBufferSize;
        if ( total_size <= (signed int)uIOBufferSize )
          write_size =total_size;
        fread(pIOBuffer, 1,  write_size,  pOutputFileHandle);
        fwrite(pIOBuffer, 1,  write_size, tmp_file);
        total_size -=  write_size;
      }
      while ( total_size > 0 );
    }
    strcpy(NewFilename, (const char *)&pLODName);
    fclose(tmp_file);
    fclose(pOutputFileHandle);
    CloseWriteFile();
    remove("lodapp.tmp");
    remove(NewFilename);
    rename(Filename, NewFilename);
    CloseWriteFile();
    LoadFile( (const char *)&pLODName, 0);
    result = 0;
  }
  else
    result = 5;
  return result;
}

//----- (00461F71) --------------------------------------------------------
bool LOD::File::AppendDirectory(LOD::Directory *pDir, const void *pData)
{
  Assert(uNumSubDirs < 299);

  memcpy(&pSubIndices[uNumSubDirs++], pDir, sizeof(LOD::Directory));
  fwrite(pData, 1, pDir->uDataSize, pOutputFileHandle);
  return true;
}

//----- (00461F1E) --------------------------------------------------------
int LODWriteableFile::CreateTempFile()
{
  if (!isFileOpened)
    return 1;

  if (pIOBuffer && uIOBufferSize )
  {
    uCurrentIndexDir = 0;
    uNumSubDirs = 0;
    pOutputFileHandle = fopen("lodapp.tmp", "wb+");
    return pOutputFileHandle ? 1 : 7;  
  }
  else
    return 5;
}

//----- (00461EE9) --------------------------------------------------------
void LODWriteableFile::CloseWriteFile()
{
  if (isFileOpened)
  {
    pContainerName[0] = 0;
    uCurrentIndexDir = 0;
    _6A0CA8_lod_unused = 0;
    
    isFileOpened = false;
    fflush(pFile);
    fclose(pFile);
    pFile = nullptr;
  }
  //else 
    //__debugbreak();
}
// 6A0CA8: using guessed type int 6A0CA8_lod_unused;


//----- (00461B48) --------------------------------------------------------
unsigned int LODWriteableFile::Write(const LOD::Directory *pDir, const void *pDirData, int a4)
{
  char Filename[256]; 
  char NewFilename[256];
  FILE *tmp_file; 
  int comp_res;
  bool bRewrite_data;
  int offset_to_data;
  int total_data_size;
  int size_correction;
  int to_copy_size;
  int read_size;
  int curr_position;
  int insert_index;

  //insert new data in sorted index lod file
  bRewrite_data = false;
  insert_index = -1;
  if ( !isFileOpened )//sometimes gives crash
    return 1;
  if ( !pSubIndices )
    return 2;
  if ( !pIOBuffer || !uIOBufferSize )
    return 3;

  for ( int i = 0; i < uNumSubDirs; i++ )
  {
    comp_res = _stricmp(pSubIndices[i].pFilename, pDir->pFilename);
    if( comp_res == 0 )
    {
      insert_index = i;
      if ( a4 == 0 )
      {
        bRewrite_data = true;
        break;
      }
      if ( a4 == 1 )
      {
        if ( pSubIndices[i].uNumSubIndices < pDir->uNumSubIndices )
        {
          if ( pSubIndices[i].word_00001E < pDir->word_00001E )
            return 4;
        }
        else
          bRewrite_data = true;
        break;
      }
      if ( a4 == 2 )
        return 4;
    }
    else if ( comp_res > 0 )
    {
      if ( insert_index == -1 )
      {
        insert_index=i;
        break;
      }
    }
  }
  strcpy(Filename, "lod.tmp");
  tmp_file = fopen(Filename, "wb+");
  if ( !tmp_file )
    return 5;
  if (!bRewrite_data)
    size_correction = 0;
  else
    size_correction = pSubIndices[insert_index].uDataSize;

  //create chapter index
  LOD::Directory Lindx;
  strcpy(Lindx.pFilename, "chapter");
  Lindx.dword_000018 = 0;
  Lindx.word_00001E = 0;
  Lindx.uNumSubIndices = uNumSubDirs;
  Lindx.uOfsetFromSubindicesStart = sizeof(LOD::FileHeader) + sizeof(LOD::Directory);
  total_data_size = uLODDataSize + pDir->uDataSize-size_correction;
  if (!bRewrite_data)
  {
    total_data_size += sizeof(LOD::Directory);
    Lindx.uNumSubIndices++;
  }

  Lindx.uDataSize = total_data_size;
  uNumSubDirs = Lindx.uNumSubIndices;
  //move indexes +1 after insert point
  if ( !bRewrite_data && (insert_index < uNumSubDirs) )//перезаписывание файлов дл€ освобождени€ места дл€ нового ф-ла
  {
    for( int i = uNumSubDirs; i > insert_index; --i )
      memcpy(&pSubIndices[i], &pSubIndices[i - 1], sizeof(LOD::Directory));
  }
  //insert
  memcpy(&pSubIndices[insert_index], pDir, sizeof(LOD::Directory));//записать текущий файл
  //correct offsets to data
  if (uNumSubDirs > 0)
  {
    offset_to_data = sizeof(LOD::Directory) * uNumSubDirs;
    for ( int i = 0; i < uNumSubDirs; i++ )
    {
      pSubIndices[i].uOfsetFromSubindicesStart = offset_to_data;
      offset_to_data += pSubIndices[i].uDataSize;
    }
  }

  //construct  lod file	with added data
  fwrite(&header, sizeof(LOD::FileHeader), 1, tmp_file);
  fwrite(&Lindx, sizeof(LOD::Directory), 1, tmp_file);
  fseek(pFile,Lindx.uOfsetFromSubindicesStart, SEEK_SET);
  fwrite(pSubIndices, sizeof(LOD::Directory), uNumSubDirs, tmp_file);

  offset_to_data = sizeof(LOD::Directory) * uNumSubDirs;
  if ( !bRewrite_data )
    offset_to_data -= sizeof(LOD::Directory);

  fseek(pFile, offset_to_data, SEEK_CUR);
  //copy from open lod to temp 	lod	  first half
  to_copy_size = pSubIndices[insert_index].uOfsetFromSubindicesStart - pSubIndices[0].uOfsetFromSubindicesStart;
  while(to_copy_size > 0)
  {
    read_size = uIOBufferSize;
    if ( to_copy_size <= uIOBufferSize )
      read_size = to_copy_size;
    fread(pIOBuffer, 1, read_size, pFile);
    fwrite(pIOBuffer, 1, read_size, tmp_file);
    to_copy_size -= read_size;
  }
  // add container data
  fwrite(pDirData, 1, pDir->uDataSize, tmp_file);
  if ( bRewrite_data )
    fseek(pFile,size_correction , SEEK_CUR);

  //add remainng data  last half
  curr_position = ftell(pFile);
  fseek(pFile, 0, SEEK_END);
  to_copy_size = ftell(pFile) - curr_position;
  fseek(pFile, curr_position, SEEK_SET);
  while ( to_copy_size > 0 )
  {
    read_size = uIOBufferSize;
    if ( to_copy_size <= uIOBufferSize )
      read_size = to_copy_size;
    fread(pIOBuffer, 1, read_size, pFile);
    fwrite(pIOBuffer, 1, read_size, tmp_file);
    to_copy_size -= read_size;
  }
  //replace	  old file by new with added  data
  strcpy(NewFilename, (const char *)&pLODName);
  fclose(tmp_file);
  fclose(pFile);
  CloseWriteFile();
  remove(NewFilename);
  rename(Filename, NewFilename);
  CloseWriteFile();
  //reload new
  LoadFile((const char *)&pLODName, 0);//isFileOpened == true, next file
  return 0;
}

//----- (00461A43) --------------------------------------------------------
bool LODWriteableFile::LoadFile(const char *pFilename, bool bWriting)
{
  if (bWriting & 1)
    pFile = fopen(pFilename, "rb");
  else
    pFile = fopen(pFilename, "rb+");
  if (!pFile)
  {
	  __debugbreak();
    return false;// возможно файл не закрыт, поэтому не открываетс€
  }

  strcpy(pLODName, pFilename);
  fread(&header, sizeof(LOD::FileHeader), 1, pFile);
  
  LOD::Directory lod_indx;
  fread( &lod_indx,sizeof(LOD::Directory), 1, pFile);

  fseek(pFile, 0, SEEK_SET);
  isFileOpened = true;
  strcpy(pContainerName, "chapter");
  uCurrentIndexDir = 0;
  uLODDataSize = lod_indx.uDataSize;
  uNumSubDirs = lod_indx.uNumSubIndices;
  Assert(uNumSubDirs <= 300);

  uOffsetToSubIndex = lod_indx.uOfsetFromSubindicesStart;
  fseek(pFile, uOffsetToSubIndex, SEEK_SET);

  fread(pSubIndices, sizeof(LOD::Directory), uNumSubDirs, pFile);
  return true;
}


//----- (00461A11) --------------------------------------------------------
void LOD::File::FreeSubIndexAndIO()
{
  free(pSubIndices);
  free(pIOBuffer);// delete [] pIOBuffer;
  pIOBuffer = nullptr;
  pSubIndices = nullptr;
}


//----- (00461954) --------------------------------------------------------
void LOD::File::AllocSubIndicesAndIO(unsigned int uNumSubIndices, unsigned int uBufferSize)
{
  if (pSubIndices)
  {
    MessageBoxA(0, "Attempt to reset a LOD subindex!", "MM6", MB_ICONEXCLAMATION);
    free(pSubIndices);
    pSubIndices = nullptr;
  }
  pSubIndices =(LOD::Directory *)malloc(32 * uNumSubIndices);
  if (pIOBuffer)
  {
    MessageBoxA(0, "Attempt to reset a LOD IObuffer!", "MM6", MB_ICONEXCLAMATION);
    free(pIOBuffer);
    pIOBuffer = nullptr;
    uIOBufferSize = 0;
  }
  if ( uBufferSize )
  {
    pIOBuffer = (unsigned __int8 *)malloc(uBufferSize);
    uIOBufferSize = uBufferSize;
  }
}



//----- (0046188A) --------------------------------------------------------
int LOD::File::LoadSubIndices(const char *pContainer)
{

  unsigned int uDir; // edi@1
  LOD::Directory *curr_index; // eax@7

  ResetSubIndices();
  uDir = 0;

  for (uDir=0; uDir <header.uNumIndices;++uDir)
      {
       if (!_stricmp(pContainer, pRoot[uDir].pFilename))
       {
       strcpy( pContainerName, pContainer);
	  uCurrentIndexDir = uDir;
	  curr_index=(LOD::Directory *)&pRoot[uDir];
	  uOffsetToSubIndex =curr_index->uOfsetFromSubindicesStart ;
	  uNumSubDirs =curr_index->uNumSubIndices;// *(_WORD *)(v8 + 28);
	  fseek( pFile, uOffsetToSubIndex, SEEK_SET);
	  pSubIndices = (LOD::Directory *)malloc(sizeof(LOD::Directory)*(uNumSubDirs + 5));

	  if ( pSubIndices)
		  fread( pSubIndices, sizeof(LOD::Directory),  uNumSubDirs,  pFile);
	  return 0;
       }
      }
 return 3;
}

//----- (004617D5) --------------------------------------------------------
bool LOD::File::LoadHeader(const char *pFilename, bool bWriting)
{
  const char *v6; // [sp-4h] [bp-Ch]@4

  if ( this->isFileOpened )
    Close();
  if ( bWriting & 1 )
    v6 = "rb";
  else
    v6 = "rb+";

  pFile = fopen(pFilename, v6);
  if ( pFile )
  {
    strcpy(pLODName, pFilename);
    fread(&header, sizeof(LOD::FileHeader), 1, pFile);
    pRoot = (LOD::Directory *)malloc(160);
    if ( pRoot )
    {
      fread(pRoot, sizeof(LOD::Directory), header.uNumIndices, pFile);
      fseek(pFile, 0, SEEK_SET);
      isFileOpened = true;
      return false;
    }
    else
    {
      fclose(pFile);
      return true;
    }
  }
  return true;
}


//----- (00461790) --------------------------------------------------------
LOD::File::~File()
{
  LOD::File *v1; // esi@1

  v1 = this;
  if ( this->isFileOpened )
  {
    fclose(this->pFile);
    free(v1->pSubIndices);
  }
}


//----- (0046175B) --------------------------------------------------------
LOD::File::File():
  pRoot(nullptr),
  isFileOpened(false)
{
  memset(pLODName, 0, 256);
  memset(pContainerName, 0, 16);
  this->pFile = nullptr;
  this->pSubIndices = nullptr;
  this->pIOBuffer = nullptr;
  this->isFileOpened = false;
  this->uIOBufferSize = 0;
  Close();
}


//----- (0046172B) --------------------------------------------------------
LOD::Directory::Directory()
{
  memset(pFilename, 0, 16);
  this->pFilename[0] = 0;
  this->uOfsetFromSubindicesStart = 0;
  this->uDataSize = 0;
  this->uNumSubIndices = 0;
  this->dword_000018 = 0;
  this->word_00001E = 0;
}

//----- (0046165E) --------------------------------------------------------
int LOD::File::CalcIndexFast(int startIndex, int maxIndex, const char *pContainerName)
{
  int v4; // esi@1
  int v5; // ebx@2
  int result; // eax@2
  int v7; // edi@10
  int v8; // esi@11
  int v9; // esi@17
  LOD::File *v10; // [sp+Ch] [bp-4h]@1

  v4 = startIndex;
  v10 = this;
  while ( 1 )                                   // binary search in LOD indices
  {
    while ( 1 )
    {
      v5 = maxIndex - v4;
      result = _stricmp((const char *)pContainerName, (const char *)(&v10->pSubIndices[(maxIndex - v4) / 2] + v4));
      if ( !result )
        _6A0CA4_lod_binary_search = (maxIndex - v4) / 2 + v4;
      if ( v4 == maxIndex )
        goto LABEL_14;
      if ( result < 0 )
        break;
      if ( v5 <= 4 )
      {
        v7 = v4;
        if ( v4 < maxIndex )
        {
          v9 = v4;
          do
          {
            result = _stricmp((const char *)pContainerName, (const char *)&v10->pSubIndices[v9]);
            if ( !result )
              goto LABEL_21;
            ++v7;
            ++v9;
          }
          while ( v7 < maxIndex );
        }
LABEL_14:
        _6A0CA4_lod_binary_search = -1;
        return result;
      }
      v4 += (maxIndex - v4) / 2;
    }
    if ( v5 <= 4 )
      break;
    maxIndex = (maxIndex - v4) / 2 + v4;
  }
  v7 = v4;
  if ( v4 >= maxIndex )
    goto LABEL_14;
  v8 = v4;
  while ( 1 )
  {
    result = _stricmp((const char *)pContainerName, (const char *)&v10->pSubIndices[v8]);
    if ( !result )
      break;
    ++v7;
    ++v8;
    if ( v7 >= maxIndex )
      goto LABEL_14;
  }
LABEL_21:
  _6A0CA4_lod_binary_search = v7;
  return result;
}
// 6A0CA4: using guessed type int _6A0CA4_lod_binary_search;


//----- (0046161C) --------------------------------------------------------
bool LOD::File::DoesContainerExist(const char *pContainer)
{
  LOD::File *this_dup; // esi@1
  int i; // ebx@1
  signed int i_dup; // edi@1
  bool result; // eax@4

  this_dup = this;
  i = 0;
  i_dup = 0;
  if ( (signed int)this->uNumSubDirs <= 0 )
  {
LABEL_4:
    result = 0;
  }
  else
  {
    while ( _stricmp((const char *)pContainer, (const char *)&this_dup->pSubIndices[i]) )
    {
      ++i_dup;
      ++i;
      if ( i_dup >= (signed int)this_dup->uNumSubDirs )
        goto LABEL_4;
    }
    result = 1;
  }
  return result;
}


//----- (00461397) --------------------------------------------------------
int LODFile_Sprites::_461397()
{
  int result; // eax@1
  //int *pfield_ECA0; // edi@1
  int v3; // esi@1
  int v4; // ecx@3

  result = this->uNumLoadedSprites;
  //pfield_ECA0 = &this->field_ECA0;
  v3 = this->field_ECA0;
  this->field_ECA8 = result;
  if ( result < v3 )
    this->field_ECA8 = v3;
  v4 = this->field_ECA4;
  if ( v3 < v4 )
    field_ECA0 = v4;
  return result;
}

//----- (00461580) --------------------------------------------------------
FILE *LOD::File::FindContainer(const char *pContainer_Name, bool bLinearSearch)
{
  unsigned int v4; // eax@4
  if (!isFileOpened)
    return 0;


  if (bLinearSearch)
  {
    for (uint i = 0; i < uNumSubDirs; ++i)
      if (!_stricmp(pContainer_Name, pSubIndices[i].pFilename))
      {
        v4 = pSubIndices[i].uOfsetFromSubindicesStart;
        fseek(pFile, uOffsetToSubIndex + v4, SEEK_SET);
        return pFile;
      }

    return nullptr;
  }
  else
  {
    CalcIndexFast(0, uNumSubDirs, pContainer_Name);
    if ( _6A0CA4_lod_binary_search < 0 )
      return 0;
    v4 = pSubIndices[_6A0CA4_lod_binary_search].uOfsetFromSubindicesStart;
    fseek(pFile, uOffsetToSubIndex + v4, SEEK_SET);
    return pFile;
  }
}

//----- (0041097D) --------------------------------------------------------
void LODFile_IconsBitmaps::SetupPalettes(unsigned int uTargetRBits, unsigned int uTargetGBits, unsigned int uTargetBBits)
{
  int v4; // edx@1
  LODFile_IconsBitmaps *v5; // esi@1
  int v6; // ecx@1
  unsigned __int8 v7; // zf@4
  unsigned __int8 v8; // sf@4
  unsigned __int16 **v9; // edi@5
  FILE *v10; // eax@7
  FILE *v11; // ebx@7
  signed int v12; // ebx@8
  int v13; // eax@9
  int v14; // edx@9
  int v16; // [sp+4Ch] [bp-8h]@4
  FILE *File; // [sp+50h] [bp-4h]@7

  v4 = uTargetGBits;
  v5 = this;
  v6 = uTargetBBits;
  if ( v5->uTextureRedBits != uTargetRBits
    || v5->uTextureGreenBits != uTargetGBits
    || v5->uTextureBlueBits != uTargetBBits )
  {
    v16 = 0;
    v7 = v5->uNumLoadedFiles == 0;
    v8 = (v5->uNumLoadedFiles & 0x80000000u) != 0;
    v5->uTextureRedBits = uTargetRBits;
    v5->uTextureGreenBits = v4;
    v5->uTextureBlueBits = v6;
    if ( !(v8 | v7) )
    {
      v9 = &v5->pTextures[0].pPalette16;
      do
      {
        Texture DstBuf; // [sp+4h] [bp-50h]@6
        //Texture::Texture(&DstBuf);

        if ( *v9 )
        {
          v10 = FindContainer((const char *)v9 - 64, 0);
          v11 = v10;
          File = v10;
          if ( v10 )
          {
            fread(&DstBuf, 1u, 0x30u, v10);
            fseek(v11, DstBuf.uTextureSize, 1);
            v12 = 0;
            do
            {
              fread((char *)&uTargetRBits + 3, 1u, 1u, File);
              fread((char *)&uTargetBBits + 3, 1u, 1u, File);
              v13 = fread((char *)&uTargetGBits + 3, 1u, 1u, File);
              LOWORD(v13) = (unsigned __int8)(BYTE3(uTargetRBits) >> (8 - LOBYTE(v5->uTextureRedBits)));
              (*v9)[v12] = v13 << (LOBYTE(v5->uTextureGreenBits) + LOBYTE(v5->uTextureBlueBits));
              LOWORD(v14) = (unsigned __int8)(BYTE3(uTargetBBits) >> (8 - LOBYTE(v5->uTextureGreenBits)));
              (*v9)[v12] |= v14 << v5->uTextureBlueBits;
              (*v9)[v12] |= BYTE3(uTargetGBits) >> (8 - LOBYTE(v5->uTextureBlueBits));
              ++v12;
            }
            while ( v12 < 256 );
          }
        }
        ++v16;
        v9 += 18;
      }
      while ( v16 < (signed int)v5->uNumLoadedFiles );
    }
  }
}



//----- (0041088B) --------------------------------------------------------
void *LOD::File::LoadRaw(const char *pContainer, int a3)
{
  LOD::File *v3; // esi@1
  FILE *v4; // eax@1
  FILE *v5; // esi@1
  void *v6; // eax@5
  void *v7; // ebx@7
  void *v8; // edi@7
  void *v9; // eax@9
  Texture DstBuf; // [sp+Ch] [bp-4Ch]@1
  FILE *File; // [sp+54h] [bp-4h]@1
  unsigned int Argsa; // [sp+60h] [bp+8h]@3

  v3 = this;
  v4 = FindContainer(pContainer, 0);
  v5 = v4;
  File = v4;
  if ( !v4 )
    Error("Unable to load %s", pContainer);

  fread(&DstBuf, 1u, 0x30u, v4);
  Argsa = DstBuf.uTextureSize;
  if ( DstBuf.uDecompressedSize )
  {
    if ( a3 )
      v6 = malloc(DstBuf.uDecompressedSize+1);
    else
      v6 = malloc(DstBuf.uDecompressedSize+1);
    v7 = v6;
    v8 = malloc(DstBuf.uTextureSize+1);
    fread(v8, 1u, Argsa, File);
    zlib::MemUnzip(v7, &DstBuf.uDecompressedSize, v8, DstBuf.uTextureSize);
    DstBuf.uTextureSize = DstBuf.uDecompressedSize;
    free(v8);
  }
  else
  {
    if ( a3 )
      v9 = malloc(DstBuf.uTextureSize+1);
    else
      v9 = malloc(DstBuf.uTextureSize+1);
    v7 = v9;
    fread(v9, 1u, Argsa, v5);
  }
  return v7;
}



//----- (00410522) --------------------------------------------------------
int LODFile_IconsBitmaps::_410522(Texture *pDst, const char *pContainer, unsigned int uTextureType)
{
  LODFile_IconsBitmaps *v4; // edi@1
  Texture *v5; // esi@5
  unsigned int v6; // eax@5
  void *v7; // eax@6
  unsigned int v8; // ST28_4@6
  void *v9; // ST2C_4@6
  unsigned __int8 *v10; // eax@7
  FILE *v11; // ST28_4@7
  void *v12; // eax@9
  FILE *v13; // ST28_4@9
  signed int v14; // eax@12
  int v15; // ecx@12
  int v16; // ecx@12
  int v17; // eax@12
  signed int v18; // ebx@14
  int v19; // eax@15
  int v20; // edx@15
  signed int v21; // ecx@18
  signed int v22; // ecx@23
  //char Args[100]; // [sp+4h] [bp-68h]@3
  FILE *File; // [sp+68h] [bp-4h]@1

  v4 = this;
  File = FindContainer(pContainer, 0);
  if ( !File )
  {
    File = FindContainer("pending", 0);
    if ( !File )
      Error("Can't find %s!", pContainer);

  }
  v5 = pDst;
  fread(pDst, 1u, 0x30u, File);
  strcpy(v5->pName, pContainer);
  pDst = (Texture *)v5->uTextureSize;
  v6 = v5->uDecompressedSize;
  v5->pLevelOfDetail0_prolly_alpha_mask = 0;
  if ( v6 )
  {
    v7 = malloc(v6);
    v8 = v5->uTextureSize;
    v5->pLevelOfDetail0_prolly_alpha_mask = (unsigned __int8 *)v7;
    pContainer = (const char *)malloc(v8);
    fread((void *)pContainer, 1u, (size_t)pDst, File);
    zlib::MemUnzip(v5->pLevelOfDetail0_prolly_alpha_mask, &v5->uDecompressedSize, pContainer, v5->uTextureSize);
    v9 = (void *)pContainer;
    v5->uTextureSize = v5->uDecompressedSize;
    free(v9);
  }
  else
  {
    v10 = (unsigned __int8 *)malloc(0);
    v11 = File;
    v5->pLevelOfDetail0_prolly_alpha_mask = v10;
    fread(v10, 1u, (size_t)pDst, v11);
  }
  v5->pPalette24 = 0;
  if ( uTextureType == 1 )
  {
    v12 = malloc(0x300u);
    v13 = File;
    v5->pPalette24 = (unsigned __int8 *)v12;
    fread(v12, 1u, 0x300u, v13);
LABEL_10:
    v5->pPalette16 = 0;
    goto LABEL_11;
  }
  if ( uTextureType != 2 )
    goto LABEL_10;
  v18 = 0;
  v5->pPalette16 = 0;
  v5->pPalette16 = (unsigned __int16 *)malloc(0x400u);
  do
  {
    fread((char *)&pContainer + 3, 1u, 1u, File);
    fread((char *)&uTextureType + 3, 1u, 1u, File);
    v19 = fread((char *)&pDst + 3, 1u, 1u, File);
    LOWORD(v19) = (unsigned __int8)(BYTE3(pContainer) >> (8 - LOBYTE(v4->uTextureRedBits)));
    v5->pPalette16[v18] = v19 << (LOBYTE(v4->uTextureBlueBits) + LOBYTE(v4->uTextureGreenBits));
    LOWORD(v20) = (unsigned __int8)(BYTE3(uTextureType) >> (8 - LOBYTE(v4->uTextureGreenBits)));
    v5->pPalette16[v18] += v20 << v4->uTextureBlueBits;
    v5->pPalette16[v18] += (unsigned __int8)(BYTE3(pDst) >> (8 - LOBYTE(v4->uTextureBlueBits)));
    ++v18;
  }
  while ( v18 < 256 );
LABEL_11:
  if ( v5->pBits & 2 )
  {
    v14 = v5->uSizeOfMaxLevelOfDetail;
    v15 = (int)&v5->pLevelOfDetail0_prolly_alpha_mask[v14];
    v5->pLevelOfDetail1 = (unsigned __int8 *)v15;
    v16 = (v14 >> 2) + v15;
    v5->pLevelOfDetail2 = (unsigned __int8 *)v16;
    v17 = v16 + (v14 >> 4);
  }
  else
  {
    v17 = 0;
    v5->pLevelOfDetail2 = 0;
    v5->pLevelOfDetail1 = 0;
  }
  v5->pLevelOfDetail3 = (unsigned __int8 *)v17;
  v21 = 1;
  while ( 1 << v21 != v5->uTextureWidth )
  {
    ++v21;
    if ( v21 >= 15 )
      goto LABEL_23;
  }
  v5->uWidthLn2 = v21;
LABEL_23:
  v22 = 1;
  while ( 1 << v22 != v5->uTextureHeight )
  {
    ++v22;
    if ( v22 >= 15 )
      goto LABEL_28;
  }
  v5->uHeightLn2 = v22;
LABEL_28:
  switch ( v5->uWidthLn2 )
  {
    case 2:
      v5->uWidthMinus1 = 3;
      break;
    case 3:
      v5->uWidthMinus1 = 7;
      break;
    case 4:
      v5->uWidthMinus1 = 15;
      break;
    case 5:
      v5->uWidthMinus1 = 31;
      break;
    case 6:
      v5->uWidthMinus1 = 63;
      break;
    case 7:
      v5->uWidthMinus1 = 127;
      break;
    case 8:
      v5->uWidthMinus1 = 255;
      break;
    case 9:
      v5->uWidthMinus1 = 511;
      break;
    case 10:
      v5->uWidthMinus1 = 1023;
      break;
    case 11:
      v5->uWidthMinus1 = 2047;
      break;
    case 12:
      v5->uWidthMinus1 = 4095;
      break;
    default:
      break;
  }
  switch ( v5->uHeightLn2 )
  {
    case 2:
      v5->uHeightMinus1 = 3;
      break;
    case 3:
      v5->uHeightMinus1 = 7;
      break;
    case 4:
      v5->uHeightMinus1 = 15;
      break;
    case 5:
      v5->uHeightMinus1 = 31;
      break;
    case 6:
      v5->uHeightMinus1 = 63;
      break;
    case 7:
      v5->uHeightMinus1 = 127;
      break;
    case 8:
      v5->uHeightMinus1 = 255;
      break;
    case 9:
      v5->uHeightMinus1 = 511;
      break;
    case 10:
      v5->uHeightMinus1 = 1023;
      break;
    case 11:
      v5->uHeightMinus1 = 2047;
      break;
    case 12:
      v5->uHeightMinus1 = 4095;
      break;
    default:
      return 1;
  }
  return 1;
}


//----- (00410423) --------------------------------------------------------
void LODFile_IconsBitmaps::_410423_move_textures_to_device()
{
  LODFile_IconsBitmaps *v1; // esi@1
  unsigned int v2; // edi@1
  char *v3; // ebx@2
  size_t v4; // eax@9
  char *v5; // ST1C_4@9
  void *v6; // eax@12
  signed int v7; // esi@13

  v1 = this;
  v2 = this->uNumLoadedFiles - 1;
  if ( (v2 & 0x80000000u) == 0 )
  {
    v3 = &this->pTextures[v2].pName[2];
    do
    {
      if ( v1->ptr_011BB4[v2] )
      {
        if ( *(v3 - 2) != 'w' || *(v3 - 1) != 't' || *v3 != 'r' || v3[1] != 'd' || v3[2] != 'r' )
        {
          pRenderer->LoadTexture(
            v3 - 2,
            *((short *)v3 + 17),
            (IDirectDrawSurface4 **)&v1->pHardwareSurfaces[v2],
            &v1->pHardwareTextures[v2]);
        }
        else
        {
          v4 = strlen(v3 - 2);
          v5 = (char *)malloc(v4 + 2);
          *v5 = 'h';
          strcpy(v5 + 1, v3 - 2);
          pRenderer->LoadTexture(
            v5,
            *((short *)v3 + 17),
            (IDirectDrawSurface4 **)&v1->pHardwareSurfaces[v2],
            &v1->pHardwareTextures[v2]);
          free(v5);
        }
      }
      --v2;
      v3 -= 72;
    }
    while ( (v2 & 0x80000000u) == 0 );
  }
  v6 = v1->ptr_011BB4;
  if ( v6 )
  {
    v7 = v1->uNumLoadedFiles;
    if ( v7 > 1 )
      memset(v6, 0, v7 - 1);
  }
}


//----- (004103BB) --------------------------------------------------------
void LODFile_IconsBitmaps::ReleaseHardwareTextures()
{
  //LODFile_IconsBitmaps *v1; // esi@1
//  unsigned int v2; // edi@1
  //struct IDirect3DTexture2 **v3; // eax@2
  //struct IDirect3DTexture2 *v4; // eax@3
  //struct IDirectDrawSurface **v5; // eax@5
  //struct IDirectDrawSurface *v6; // eax@6

  //v1 = this;
  
  //while ( 1 )
  for ( uint i = 0; i < this->uNumLoadedFiles; i++ )
  {
    
    //if ( (v2 & 0x80000000u) != 0 )
      //break;
    //v3 = this->pHardwareTextures;
    if ( this->pHardwareTextures )
    {
      //v4 = this->pHardwareTextures[v2];
      if ( this->pHardwareTextures[i] )
      {
        this->pHardwareTextures[i]->Release();
        this->pHardwareTextures[i] = 0;
        this->ptr_011BB4[i] = 1;
      }
    }
    //v5 = this->pHardwareSurfaces;
    if ( this->pHardwareSurfaces )
    {
      //v6 = this->pHardwareSurfaces[v2];
      if ( this->pHardwareSurfaces[i] )
      {
        this->pHardwareSurfaces[i]->Release();
        this->pHardwareSurfaces[i] = 0;
        this->ptr_011BB4[i] = 1;
      }
    }
  }
}


//----- (0041033D) --------------------------------------------------------
void LODFile_IconsBitmaps::ReleaseLostHardwareTextures()
{
  LODFile_IconsBitmaps *v1; // edi@1
  unsigned int i; // ebx@1
  struct IDirectDrawSurface **pHardwareSurfaces; // eax@2
  int v4; // esi@3
  struct IDirectDrawSurface *pSurface; // eax@3
  struct IDirect3DTexture2 **v6; // eax@5
  struct IDirect3DTexture2 *v7; // eax@6

  v1 = this;
  for ( i = this->uNumLoadedFiles - 1; (i & 0x80000000u) == 0; --i )
  {
    pHardwareSurfaces = v1->pHardwareSurfaces;
    if ( pHardwareSurfaces )
    {
      v4 = i;
      pSurface = pHardwareSurfaces[i];
      if ( pSurface )
      {
        if ( pSurface->IsLost() == DDERR_SURFACELOST )
        {
          v6 = v1->pHardwareTextures;
          if ( v6 )
          {
            v7 = v6[v4];
            if ( v7 )
            {
              v7->Release();
              v1->pHardwareTextures[v4] = 0;
            }
          }
          v1->pHardwareSurfaces[v4]->Release();
          v1->pHardwareSurfaces[v4] = 0;
          v1->ptr_011BB4[i] = 1;
        }
      }
    }
  }
}

//----- (004101B1) --------------------------------------------------------
int LODFile_IconsBitmaps::ReloadTexture(Texture *pDst, const char *pContainer, int mode)
{
  LODFile_IconsBitmaps *v4; // edi@1
  FILE *v5; // eax@1
  Texture *v6; // esi@2
  unsigned int v7; // ebx@6
  unsigned int v8; // ecx@6
  signed int result; // eax@7
  size_t *v10; // ebx@8
  signed int v11; // ebx@12
  int v12; // eax@13
  int v13; // edx@13
  FILE *File; // [sp+Ch] [bp-8h]@1
  unsigned __int8 v15; // [sp+11h] [bp-3h]@13
  unsigned __int8 v16; // [sp+12h] [bp-2h]@13
  unsigned __int8 DstBuf; // [sp+13h] [bp-1h]@13
  void *DstBufa; // [sp+1Ch] [bp+8h]@10
  void *Sourcea; // [sp+20h] [bp+Ch]@10
  unsigned int Counta; // [sp+24h] [bp+10h]@6

  v4 = this;
  v5 = FindContainer(pContainer, 0);
  File = v5;
  if ( v5
    && (v6 = pDst, pDst->pLevelOfDetail0_prolly_alpha_mask)
    && mode == 2
    && pDst->pPalette16
    && !pDst->pPalette24
    && (v7 = pDst->uTextureSize,
        fread(pDst, 1u, 0x30u, v5),
        strcpy(pDst->pName, pContainer),
        v8 = pDst->uTextureSize,
        Counta = pDst->uTextureSize,
        (signed int)v8 <= (signed int)v7) )
  {
    v10 = &pDst->uDecompressedSize;
    if ( !pDst->uDecompressedSize || v4->dword_011BA4 )
    {
      fread(pDst->pLevelOfDetail0_prolly_alpha_mask, 1u, v8, File);
    }
    else
    {
      Sourcea = malloc(pDst->uDecompressedSize);
      DstBufa = malloc(pDst->uTextureSize);
      fread(DstBufa, 1u, Counta, File);
      zlib::MemUnzip(Sourcea, &v6->uDecompressedSize, DstBufa, v6->uTextureSize);
      v6->uTextureSize = *v10;
      free(DstBufa);
      memcpy(v6->pLevelOfDetail0_prolly_alpha_mask, Sourcea, *v10);
      free(Sourcea);
    }
    v11 = 0;
    do
    {
      fread(&DstBuf, 1u, 1u, File);
      fread(&v16, 1u, 1u, File);
      v12 = fread(&v15, 1u, 1u, File);
      LOWORD(v12) = (unsigned __int8)(DstBuf >> (8 - LOBYTE(v4->uTextureRedBits)));
      v6->pPalette16[v11] = v12 << (LOBYTE(v4->uTextureBlueBits) + LOBYTE(v4->uTextureGreenBits));
      LOWORD(v13) = (unsigned __int8)(v16 >> (8 - LOBYTE(v4->uTextureGreenBits)));
      v6->pPalette16[v11] += v13 << v4->uTextureBlueBits;
      v6->pPalette16[v11] += (unsigned __int8)(v15 >> (8 - LOBYTE(v4->uTextureBlueBits)));
      ++v11;
    }
    while ( v11 < 256 );
    result = 1;
  }
  else
  {
    result = -1;
  }
  return result;
}


//----- (0040FC08) --------------------------------------------------------
int LODFile_IconsBitmaps::LoadTextureFromLOD(Texture *pOutTex, const char *pContainer, enum TEXTURE_TYPE eTextureType)
{
  Texture *v8; // esi@3
  size_t v11; // eax@14
  enum TEXTURE_TYPE v12; // eax@14
  signed int v13; // esi@14
  unsigned int v14; // eax@21
//  unsigned int v15; // ecx@25
//  unsigned int *v16; // ebx@25
//  void *v17; // eax@27
//  unsigned int v18; // ST28_4@27
  void *v19; // ST3C_4@27
  //Allocator *v20; // ebx@29
  void *v21; // eax@29
  size_t v22; // ST2C_4@29
  const void *v23; // ecx@29
//  unsigned __int16 v24; // ax@29
//  unsigned __int16 v25; // cx@29
//  __int16 v26; // dx@29
//  unsigned int v27; // ecx@29
//  Texture *v28; // eax@29
//  unsigned int v29; // ST28_4@30
  void *v30; // eax@30
//  unsigned int v31; // ST2C_4@30
  void *v32; // eax@32
  void *v33; // eax@34
  signed int v34; // eax@37
  unsigned __int8 *v35; // ecx@37
  unsigned __int8 *v36; // ecx@37
  unsigned __int8 *v37; // eax@37
  signed int v38; // ebx@39
  int v39; // eax@40
  int v40; // edx@40
  signed int v41; // ecx@43
  signed int v42; // ecx@48

  //v4 = pContainer;
  //v5 = this;
  //v6 = FindContainer(pContainer, 0);
  //File = v6;
  FILE* pFile = FindContainer(pContainer, false);
  if (!pFile)
    return -1;
  v8 = pOutTex;
  fread(pOutTex, 1, 0x30, pFile);
  strcpy(pOutTex->pName, pContainer);
  if (/*pRenderer->pRenderD3D &&*/ pOutTex->pBits & 2)
  {
    if (!pHardwareSurfaces || !pHardwareTextures)
    {
      pHardwareSurfaces = new IDirectDrawSurface *[1000];
      memset(pHardwareSurfaces, 0, 1000 * sizeof(IDirectDrawSurface4 *));

      pHardwareTextures = new IDirect3DTexture2 *[1000];
      memset(pHardwareTextures, 0, 1000 * sizeof(IDirect3DTexture2 *));

      ptr_011BB4 = new char[1000];
      memset(ptr_011BB4, 0, 1000);
    }
    if (_strnicmp(pContainer, "wtrdr", 5))//*v4 != 'w' || v4[1] != 't' || v4[2] != 'r' || v4[3] != 'd' || v4[4] != 'r' )
    {
      if (_strnicmp(pContainer, "WtrTyl", 6))//if ( *v4 != 'W' || v4[1] != 't' || v4[2] != 'r' || v4[3] != 'T' || v4[4] != 'y' || v4[5] != 'l' )
        v14 = uNumLoadedFiles;
      else
      {
        pRenderer->hd_water_tile_id = uNumLoadedFiles;
        v14 = uNumLoadedFiles;
      }
      v13 = pRenderer->LoadTexture(pContainer, pOutTex->palette_id1, (IDirectDrawSurface4 **)&pHardwareSurfaces[v14], &pHardwareTextures[v14]);
    }
    else
    {
      v11 = strlen(pContainer);
      v12 = (enum TEXTURE_TYPE)(int)malloc(v11 + 2);
      eTextureType = v12;
      *(char *)v12 = 104;
      strcpy((char *)(v12 + 1), pContainer);
      v13 = pRenderer->LoadTexture((const char *)eTextureType, pOutTex->palette_id1,
              (IDirectDrawSurface4 **)&pHardwareSurfaces[uNumLoadedFiles], &pHardwareTextures[uNumLoadedFiles]);
      free((void *)eTextureType);
    }
    return v13;
  }
  //v15 = v8->uTextureSize;
  //v16 = &v8->uDecompressedSize;
  //pOutTex = (Texture *)v8->uTextureSize;
  if ( !v8->uDecompressedSize || dword_011BA4 )
  {
    v32 = malloc(v8->uTextureSize);
    v8->pLevelOfDetail0_prolly_alpha_mask = (unsigned __int8 *)v32;
    fread(v32, 1, (size_t)v8->uTextureSize, pFile);
  }
  else
  {
    //v17 = malloc(v8->uDecompressedSize);
    //v18 = v8->uTextureSize;
    pContainer = (const char *)malloc(v8->uDecompressedSize);
    v19 = malloc(v8->uTextureSize);
    fread(v19, 1, (size_t)v8->uTextureSize, pFile);
    zlib::MemUnzip((void *)pContainer, &v8->uDecompressedSize, v19, v8->uTextureSize);
    v8->uTextureSize = v8->uDecompressedSize;
    free(v19);
    if ( /*bUseLoResSprites*/false && v8->pBits & 2 )
    {
      pOutTex = (Texture *)(((signed int)v8->uSizeOfMaxLevelOfDetail >> 2)
                          + ((signed int)v8->uSizeOfMaxLevelOfDetail >> 4)
                          + ((signed int)v8->uSizeOfMaxLevelOfDetail >> 6));
      v21 = malloc((unsigned int)pOutTex);
      v22 = (size_t)pOutTex;
      v23 = &pContainer[v8->uTextureWidth * v8->uTextureHeight];
      v8->pLevelOfDetail0_prolly_alpha_mask = (unsigned __int8 *)v21;
      memcpy(v21, v23, v22);
      v8->uTextureWidth = (signed __int16)v8->uTextureWidth / 2;
      //v24 = v8->uTextureWidth;
      v8->uTextureHeight = (signed __int16)v8->uTextureHeight / 2;
      //v25 = v8->uTextureHeight;
      --v8->uWidthLn2;
      --v8->uHeightLn2;
      v8->uWidthMinus1 = v8->uTextureWidth - 1;
      //v26 = v25 - 1;
      //v27 = (signed __int16)v8->uTextureWidth * (signed __int16)v25;
      //v28 = pOutTex;
      v8->uHeightMinus1 = v8->uTextureHeight - 1;
      v8->uSizeOfMaxLevelOfDetail = (signed __int16)v8->uTextureWidth * (signed __int16)v8->uTextureHeight;
      v8->uTextureSize = (unsigned int)pOutTex;
    }
    else
    {
      //v29 = v8->uDecompressedSize;
      v30 = malloc(v8->uDecompressedSize);
      //v31 = v8->uDecompressedSize;
      v8->pLevelOfDetail0_prolly_alpha_mask = (unsigned __int8 *)v30;
      memcpy(v30, pContainer, v8->uDecompressedSize);
    }
    free((void *)pContainer);
  }
  free(v8->pPalette16);
  free(v8->pPalette24);
  if ( eTextureType == TEXTURE_24BIT_PALETTE )
  {
    v33 = malloc(0x300);
    v8->pPalette24 = (unsigned __int8 *)v33;
    fread(v33, 1, 0x300u, pFile);
  }
  else
  {
    v8->pPalette24 = 0;
    if ( eTextureType == TEXTURE_16BIT_PALETTE )
    {
      v8->pPalette16 = (unsigned __int16 *)malloc(0x200);
      for ( v38 = 0; v38 < 256; ++v38 )
      {
        fread((char *)&eTextureType + 3, 1, 1, pFile);
        fread((char *)&pContainer + 3, 1, 1, pFile);
        v39 = fread((char *)&pOutTex + 3, 1, 1, pFile);
        LOWORD(v39) = (unsigned __int8)(BYTE3(eTextureType) >> (8 - LOBYTE(uTextureRedBits)));
        v8->pPalette16[v38] = v39 << (LOBYTE(uTextureBlueBits) + LOBYTE(uTextureGreenBits));
        LOWORD(v40) = (unsigned __int8)(BYTE3(pContainer) >> (8 - LOBYTE(uTextureGreenBits)));
        v8->pPalette16[v38] += v40 << uTextureBlueBits;
        v8->pPalette16[v38] += (unsigned __int8)(BYTE3(pOutTex) >> (8 - LOBYTE(uTextureBlueBits)));
      }
      goto LABEL_36;
    }
  }
  v8->pPalette16 = 0;
LABEL_36:
  if ( v8->pBits & 2 )
  {
    v34 = v8->uSizeOfMaxLevelOfDetail;
    v35 = &v8->pLevelOfDetail0_prolly_alpha_mask[v34];
    v8->pLevelOfDetail1 = v35;
    v36 = &v35[v34 >> 2];
    v8->pLevelOfDetail2 = v36;
    v37 = &v36[v34 >> 4];
  }
  else
  {
    v37 = 0;
    v8->pLevelOfDetail2 = 0;
    v8->pLevelOfDetail1 = 0;
  }
  v8->pLevelOfDetail3 = v37;
  for ( v41 = 1; v41 < 15; ++v41 )
  {
    if ( 1 << v41 == v8->uTextureWidth )
      v8->uWidthLn2 = v41;
  }
  for ( v42 = 1; v42 < 15; ++v42 )
  {
    if ( 1 << v42 == v8->uTextureHeight )
      v8->uHeightLn2 = v42;
  }

  switch ( v8->uWidthLn2 )
  {
    case 2:
      v8->uWidthMinus1 = 3;
      break;
    case 3:
      v8->uWidthMinus1 = 7;
      break;
    case 4:
      v8->uWidthMinus1 = 15;
      break;
    case 5:
      v8->uWidthMinus1 = 31;
      break;
    case 6:
      v8->uWidthMinus1 = 63;
      break;
    case 7:
      v8->uWidthMinus1 = 127;
      break;
    case 8:
      v8->uWidthMinus1 = 255;
      break;
    case 9:
      v8->uWidthMinus1 = 511;
      break;
    case 10:
      v8->uWidthMinus1 = 1023;
      break;
    case 11:
      v8->uWidthMinus1 = 2047;
      break;
    case 12:
      v8->uWidthMinus1 = 4095;
      break;
    default:
      break;
  }
  switch ( v8->uHeightLn2 )
  {
    case 2:
      v8->uHeightMinus1 = 3;
      break;
    case 3:
      v8->uHeightMinus1 = 7;
      break;
    case 4:
      v8->uHeightMinus1 = 15;
      break;
    case 5:
      v8->uHeightMinus1 = 31;
      break;
    case 6:
      v8->uHeightMinus1 = 63;
      break;
    case 7:
      v8->uHeightMinus1 = 127;
      break;
    case 8:
      v8->uHeightMinus1 = 255;
      break;
    case 9:
      v8->uHeightMinus1 = 511;
      break;
    case 10:
      v8->uHeightMinus1 = 1023;
      break;
    case 11:
      v8->uHeightMinus1 = 2047;
      break;
    case 12:
      v8->uHeightMinus1 = 4095;
      break;
    default:
      return 1;
  }
  return 1;
}


Texture *LODFile_IconsBitmaps::LoadTexturePtr(const char *pContainer, enum TEXTURE_TYPE uTextureType)
{
  uint id = LoadTexture(pContainer, uTextureType);

  Assert(id != -1 && L"Texture not found");

  return &pTextures[id];
}

//----- (0040FB20) --------------------------------------------------------
unsigned int LODFile_IconsBitmaps::LoadTexture(const char *pContainer, enum TEXTURE_TYPE uTextureType)
{
  //LODFile_IconsBitmaps *v3; // esi@1
  //unsigned int v4; // edi@1
  //Texture *v5; // ebx@2

  //v3 = this;
  //v4 = 0;
  areWeLoadingTexture = 1;

  for (uint i = 0; i < uNumLoadedFiles; ++i)
  {
    if (!_stricmp(pContainer, pTextures[i].pName))
      return i;
  }

//  if (!uNumLoadedFiles)
//  {
//LABEL_5:
    Assert(uNumLoadedFiles < 1000);
    /*if (uNumLoadedFiles >= 1000)
    {
      Log::Warning(L"Maximum texture number exceeded");
      AbortWithError();
    }*/
    if (LoadTextureFromLOD(&pTextures[uNumLoadedFiles], pContainer, uTextureType) == -1)
    {
      for ( uint i = 0; i < uNumLoadedFiles; ++i )
      {
        if (!_stricmp(pTextures[i].pName, "pending"))
          return i;
      }
      LoadTextureFromLOD(&pTextures[uNumLoadedFiles], "pending", uTextureType);
    }
    areWeLoadingTexture = 0;
    ++uNumLoadedFiles;
    return uNumLoadedFiles - 1;
//  }
//  v5 = pTextures;
//  while ( _stricmp(v5->pName, pContainer) )
//  {
//    ++v4;
//    ++v5;
//    if (v4 >= uNumLoadedFiles )
//      goto LABEL_5;
//  }
//  return v4;
}

Texture * LODFile_IconsBitmaps::GetTexture( int idx )
{
  Assert(idx < MAX_LOD_TEXTURES, "Texture index out of bounds (%u)", idx);
  if (idx == -1) 
  {
    //Log::Warning(L"Texture id = %d missing", idx);
    return pTextures + LoadDummyTexture();
  }
  return pTextures + idx;
}

//----- (0046082C) --------------------------------------------------------
bool Initialize_GamesLOD_NewLOD()
{
  pGames_LOD = new LODWriteableFile;
  pGames_LOD->AllocSubIndicesAndIO(300, 0);
  if (pGames_LOD->LoadFile("data\\games.lod", 1))
  {
    pNew_LOD = new LODWriteableFile;
    pNew_LOD->AllocSubIndicesAndIO(300, 100000);
    return true;
  }
  return false;
}
