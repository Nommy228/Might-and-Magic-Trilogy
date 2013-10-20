#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "GUIProgressBar.h"
#include "LOD.h"
#include "Mouse.h"
#include "Party.h"
#include "Render.h"
#include "IconFrameTable.h"

#include "mm7_data.h"




struct GUIProgressBar *pGameLoadingUI_ProgressBar = new GUIProgressBar;




//----- (00Initialize) --------------------------------------------------------
bool GUIProgressBar::Initialize(Type type)
{
  //GUIProgressBar *v2; // esi@1
  signed int v4; // eax@7
  int v5; // ecx@8
  int v6; // edi@8
  int v7; // edx@14
  //const char *v8; // [sp-8h] [bp-84h]@20
  //unsigned int v9; // [sp-4h] [bp-80h]@20
  char Str1[64]; // [sp+4h] [bp-78h]@16

  switch (type)
  {
    case TYPE_None:
      return true;

    case TYPE_Box:
    case TYPE_Fullscreen:
      break;

    default:
      Error("Invalid GUIProgressBar type: %u", type);
  }

  //v2 = this;
  if (pLoadingBg.pPixels)
    return false;

  uType = type;

  v4 = 1;
  if (uType == TYPE_Fullscreen)
  {
    v5 = 0;
    v6 = (int)&field_10;
    do
    {
      if ( *(char *)(v6 + v4) == 1 )
        ++v5;
      ++v4;
    }
    while ( v4 <= 5 );
    if ( v5 == 5 )
      memset(&field_10, 0, 8u);
    v7 = rand() % 5 + 1;
    if ( *(&field_10 + v7) == 1 )
    {
      do
        v7 = rand() % 5 + 1;
      while ( *(char *)(v6 + v7) == 1 );
    }
    sprintf(Str1, "loading%d.pcx", v7);
    pLoadingBg.Load(Str1, 2);
    uProgressCurrent = 0;
    uX = 122;
    uY = 151;
    uWidth = 449;
    uHeight = 56;
    uProgressMax = 26;
    pIcons_LOD->_410522(&pLoadingProgress, "loadprog", 2u);
    Draw();
    return true;
  }

  /*if ( !pParty->uAlignment )
  {
    v9 = 2;
    v8 = "bardata-b";
    goto LABEL_23;
  }
  if ( pParty->uAlignment == 1 )
  {
    v9 = 2;
    v8 = "bardata";
    goto LABEL_23;
  }
  if ( pParty->uAlignment == 2 )
  {
    v9 = 2;
    v8 = "bardata-c";
LABEL_23:
    pIcons_LOD->_410522(&pBardata, v8, v9);
  }*/
  switch (pParty->alignment)
  {
    case PartyAlignment_Good:    pIcons_LOD->_410522(&pBardata, "bardata-b", 2); break;
    case PartyAlignment_Neutral: pIcons_LOD->_410522(&pBardata, "bardata", 2); break;
    case PartyAlignment_Evil:    pIcons_LOD->_410522(&pBardata, "bardata-c", 2); break;
    default: Error("Invalid alignment type: %u", pParty->alignment);
  }

  uProgressCurrent = 0;
  uProgressMax = 26;
  Draw();
  return true;
}

//----- (004435BB) --------------------------------------------------------
void GUIProgressBar::Reset(unsigned __int8 uMaxProgress)
{
  field_9 = 0;
  uProgressCurrent = 0;
  uProgressMax = uMaxProgress;
}

//----- (004435CD) --------------------------------------------------------
void GUIProgressBar::Progress()
{
  unsigned __int8 v1; // al@1

  ++this->uProgressCurrent;
  v1 = this->uProgressMax;
  if ( this->uProgressCurrent > v1 )
    this->uProgressCurrent = v1;
  Draw();
}

//----- (004435E2) --------------------------------------------------------
void GUIProgressBar::Release()
{
  GUIProgressBar *v1; // esi@1
  char v2; // al@5
  int v3; // edi@7

  v1 = this;
  if ( v1->uType == 1 )
  {
    if ( !v1->pLoadingBg.pPixels )
      return;
    v2 = v1->uProgressMax;
    if ( v1->uProgressCurrent != v2 )
    {
      v1->uProgressCurrent = v2 - 1;
      Progress();
    }
    free(v1->pLoadingBg.pPixels);
    v3 = (int)&v1->pLoadingProgress.pLevelOfDetail0_prolly_alpha_mask;
    free(v1->pLoadingProgress.pLevelOfDetail0_prolly_alpha_mask);
    free(v1->pLoadingProgress.pPalette16);
    v1->pLoadingProgress.pPalette16 = 0;
    v1->pLoadingBg.pPixels = 0;
  }
  else
  {
    if ( !v1->pBardata.pLevelOfDetail0_prolly_alpha_mask )
      return;
    free(v1->pBardata.pLevelOfDetail0_prolly_alpha_mask);
    v3 = (int)&v1->pBardata.pPalette16;
    free(v1->pBardata.pPalette16);
    v1->pBardata.pLevelOfDetail0_prolly_alpha_mask = 0;
  }
  *(int *)v3 = 0;
}

//----- (00443670) --------------------------------------------------------
void GUIProgressBar::Draw()
{
  IconFrame *v3; // eax@10
  float v4; // ST2C_4@10

  //v1 = this;
  pRenderer->BeginScene();
  if (uType != TYPE_Fullscreen)
  {
    if (pBardata.pLevelOfDetail0_prolly_alpha_mask)
    {
      if (pRenderer->pRenderD3D)
      {
        if (!pRenderer->bWindowMode)
          pRenderer->_49FD3A();
      }
      pRenderer->DrawTextureIndexed(0x50u, 0x7Au, &pBardata);
      v3 = pIconsFrameTable->GetFrame(uIconID_TurnHour, 0);
      pRenderer->DrawTextureTransparent(0x64u, 0x92u, &pIcons_LOD->pTextures[v3->uTextureID]);
      v4 = (double)(113 * uProgressCurrent) / (double)uProgressMax;
      pRenderer->FillRectFast(
        174,
        164,
        floorf(v4 + 0.5f),//COERCE_UNSIGNED_INT64(v4 + 6.7553994e15),
        16,
        pRenderer->uTargetRMask);
        pRenderer->EndScene();
		pRenderer->Present();
		return;
    }
    pRenderer->EndScene();
    return;
  }

  if (!pLoadingBg.pPixels)
  {
	pRenderer->EndScene();
    return;
  }

  pRenderer->DrawTextureRGB(0, 0, &pLoadingBg);
  pRenderer->SetRasterClipRect(0, 0, 0x27Fu, 0x1DFu);
  pRenderer->SetTextureClipRect(0xACu, 0x1CBu,
    15 * (signed int)(signed __int64)((double)(300 * uProgressCurrent) / (double)uProgressMax) / 15 + 172,
    0x1D7u);
  pRenderer->DrawTextureTransparent(0xACu, 0x1CBu, &pLoadingProgress);
  pRenderer->ResetTextureClipRect();
  pRenderer->EndScene();
  pRenderer->Present();
}