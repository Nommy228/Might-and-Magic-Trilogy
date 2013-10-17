#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif


#include "Texture.h"
#include "mm7_data.h"
#include "Render.h"
#include "Mouse.h"
#include "Items.h"
#include "Party.h"
#include "LOD.h"
#include "Game.h"

#include "TurnEngine.h"
#include "Viewport.h"
#include "GUIWindow.h"
#include "Vis.h"
#include "Actor.h"
#include "MM7.h"
#include "AudioPlayer.h"


Mouse *pMouse;





//----- (00469860) --------------------------------------------------------
void Mouse::GetClickPos(unsigned int *pX, unsigned int *pY)
{
  *pX = uMouseClickX;
  *pY = uMouseClickY;
}

//----- (004698A6) --------------------------------------------------------
void Mouse::RemoveHoldingItem()
{
  pParty->pPickedItem.Reset();
  if (_stricmp(pCurrentCursorName, "MICON2"))
    SetCursorBitmap("MICON1");
}

//----- (004698D8) --------------------------------------------------------
void Mouse::SetCursorBitmapFromItemID(unsigned int uItemID)
{
  pMouse->SetCursorBitmap(pItemsTable->pItems[uItemID].pIconName);
}

//----- (004698F6) --------------------------------------------------------
void Mouse::SetCurrentCursorBitmap()
{
  SetCursorBitmap(pCurrentCursorName);
}

//----- (00469903) --------------------------------------------------------
void Mouse::SetCursorBitmap(const char *pName)
{
  if ( !this->bInitialized || !pName )
    return;
  if ( _stricmp("MICON2", pName) )
    pGame->uFlags2 &= 0xFFFFFFEFu;
  else
    pGame->uFlags2 |= 0x10u;
  if ( _stricmp(this->pCurrentCursorName, pName) )
    strcpy(this->pCurrentCursorName, pName);
  ClearCursor();
  if ( _strnicmp(pName, "MICON1", 5) )
  {
    this->uCursorTextureID = pIcons_LOD->LoadTexture(pName, TEXTURE_16BIT_PALETTE);
    this->uCursorTextureID_2 = pIcons_LOD->LoadTexture(pName, TEXTURE_16BIT_PALETTE);
    this->AllocCursorSystemMem();
    this->field_C = 0;
    this->bRedraw = 1;
    this->bActive = 1;
    if ( !areWeLoadingTexture )
    {
      if (uCursorTextureID != -1)
        pIcons_LOD->pTextures[uCursorTextureID].Release();
      pIcons_LOD->SyncLoadedFilesCount();
    }
    return;
  }
  this->bActive = false;
  this->field_C = 1;
  window->SetCursor(pName);
}
// 506128: using guessed type int areWeLoadingTexture;

//----- (00469AE4) --------------------------------------------------------
LONG Mouse::_469AE4()
{
  Mouse *v1; // esi@1
  LONG v2; // ecx@2
  LONG result; // eax@2
  struct tagPOINT Point; // [sp+Ch] [bp-8h]@2

  v1 = this;
  this->field_8 = 1;
  /*if (pAsyncMouse)
  {
    v2 = *((int *)pAsyncMouse + 6);
    Point.x = *((int *)pAsyncMouse + 6);
    result = *((int *)pAsyncMouse + 7);
  }
  else
  {*/
    GetCursorPos(&Point);
    if ( pRenderer->bWindowMode )
      ScreenToClient(window->GetApiHandle(), &Point);
    result = Point.y;
    v2 = Point.x;
  //}
  v1->uMouseClickX = v2;
  v1->uMouseClickY = result;
  if ( pRenderer->bWindowMode )
    goto LABEL_16;
  //if (pAsyncMouse)
  //  goto LABEL_24;
  if ( v2 < 0 )
    v2 = 0;
  if ( result < 0 )
    result = 0;
  if ( v2 > 639 )
    v2 = 639;
  if ( result > 479 )
  {
    result = 479;
LABEL_16:
    //if (pAsyncMouse)
    //  goto LABEL_24;
    if ( pRenderer->bWindowMode && (v2 < 0 || result < 0 || v2 > 639 || result > 479) )
      goto LABEL_23;
  }
  if ( v1->field_C )
LABEL_23:
    v1->bActive = 0;
LABEL_24:
  v1->field_8 = 0;
  return result;
}

//----- (00469BA3) --------------------------------------------------------
void Mouse::ClearCursor()
{
  Mouse *v1; // esi@1
  void *v2; // eax@1
  void **v3; // esi@5

  v1 = this;
  v2 = this->pCursorBitmap_sysmem;
  this->bActive = 0;
  if ( v2 )
  {
    free(v2);
    v1->pCursorBitmap_sysmem = 0;
  }
  if ( v1->pCursorBitmap2_sysmem )
  {
    free(v1->pCursorBitmap2_sysmem);
    v1->pCursorBitmap2_sysmem = 0;
  }
  v3 = &v1->ptr_90;
  if ( *v3 )
    free(*v3);
  *v3 = 0;
}

//----- (00469BE6) --------------------------------------------------------
void Mouse::AllocCursorSystemMem()
{
  bActive = 0;
  if (!pCursorBitmap_sysmem)
    pCursorBitmap_sysmem = (unsigned __int16 *)DoAllocCursorMem();
  if (!pCursorBitmap2_sysmem)
    pCursorBitmap2_sysmem = (unsigned __int8 *)DoAllocCursorMem();
}

//----- (00469C0D) --------------------------------------------------------
void *Mouse::DoAllocCursorMem()
{
  auto tex = pIcons_LOD->GetTexture(uCursorTextureID);
  return malloc(4 * tex->uTextureWidth * tex->uTextureHeight);
}

//----- (00469C39) --------------------------------------------------------
POINT *Mouse::GetCursorPos(POINT *a2)
{
  a2->x = this->uMouseClickX;
  a2->y = this->uMouseClickY;
  return a2;
}

//----- (00469C65) --------------------------------------------------------
void Mouse::Initialize(OSWindow *window)
{
  this->window = window;
  this->bActive = false;
  this->bInitialized = true;
  this->pCursorBitmapPos.x = 0;
  this->pCursorBitmapPos.y = 0;
  this->uMouseClickX = 0;
  this->uMouseClickY = 0;
  this->pCursorBitmap_sysmem = 0;
  this->field_34 = 0;
  this->pCursorBitmap2_sysmem = 0;

  SetCursorBitmap("MICON3");
  SetCursorBitmap("MICON2");
  SetCursorBitmap("MICON1");
}

// inlined
//----- (0045FE00) mm6 chinese --------------------------------------------
void Mouse::SetActive(bool active)
{
  bActive = active;
}

//----- (00469CC2) --------------------------------------------------------
void Mouse::Deactivate()
{
  if (bInitialized)
    SetActive(false);
}

//----- (00469CCD) --------------------------------------------------------
void Mouse::DrawCursor()
{
  Mouse *v1; // esi@1
  char v11; // zf@6
  signed int v3; // eax@6
  signed int v4; // ecx@6
  Vec4_int_ *v5; // edx@21
  int v6; // edi@21
  int v7; // ebx@21
  int v8; // eax@29
  unsigned int v9; // eax@31

  v1 = this;
  if ( this->bInitialized )
  {
    if ( !this->field_8 && this->bActive && !this->field_C )
      pMouse->_469AE4();
    v11 = v1->field_C == 0;
    v3 = v1->uMouseClickX;
    v4 = v1->uMouseClickY;
    v1->field_F4 = 1;
    if ( !v11 )
      goto LABEL_12;
    if ( pRenderer->bWindowMode )
    {
      if ( v3 < 0 || v4 < 0 || v3 > 639 || v4 > 479 )
      {
LABEL_12:
        v1->field_F4 = 0;
        return;
      }
    }
    else
    {
      if ( v3 < 0 )
        v3 = 0;
      if ( v4 < 0 )
        v4 = 0;
      if ( v3 > 639 )
        v3 = 639;
      if ( v4 > 479 )
        v4 = 479;
    }
    v5 = &v1->pCursorBitmapRect;
    v6 = v3 + v1->uCursorBitmapPitch;
    v7 = v4 + v1->field_5C[0];
    v1->pCursorBitmapRect.y = v4;
    v1->pCursorBitmapRect.x = v3;
    v1->pCursorBitmapRect.z = v6;
    v1->pCursorBitmapRect.w = v7;
    if ( v3 < 0 )
      v5->x = 0;
    if ( v4 < 0 )
      v1->pCursorBitmapRect.y = 0;
    if ( v6 > 640 )
      v1->pCursorBitmapRect.z = 640;
    if ( v7 > 480 )
      v1->pCursorBitmapRect.w = 480;
    v8 = v1->pCursorBitmapRect.z;
    v1->bActive = 0;
    v1->uCursorBitmapWidth = v8 - v5->x;
    v11 = v1->bRedraw == 0;
    v1->uCursorBitmapHeight = v1->pCursorBitmapRect.w - v1->pCursorBitmapRect.y;
    if ( !v11 )
    {
      if ( pMouse->ptr_90 )
        v9 = 2 * pMouse->uCursorBitmapPitch;
      else
        v9 = 0;
      pRenderer->_4A6DF5(
        v1->pCursorBitmap_sysmem,
        v9,
        &v1->pCursorBitmapPos,
        pRenderer->pTargetSurface,
        pRenderer->uTargetSurfacePitch,
        &v1->pCursorBitmapRect);
      v1->bRedraw = false;
    }
  }
}

//----- (00469E1C) --------------------------------------------------------
void Mouse::_469E1C()
{
  bActive = true;
}

//----- (00469E24) --------------------------------------------------------
void Mouse::_469E24()
{
  if (pCursorBitmap3_sysmembits_16bit)
  {
    free(pCursorBitmap3_sysmembits_16bit);
    pCursorBitmap3_sysmembits_16bit = 0;
  }
}

//----- (00469E3B) --------------------------------------------------------
void Mouse::DrawCursorToTarget()
{
  if (!pCursorBitmap3_sysmembits_16bit)
    return;

  auto pSrc = pCursorBitmap3_sysmembits_16bit;
  for (uint y = field_44; y < field_4C; ++y)
    for (uint x = field_40; x < field_48; ++x)
      pRenderer->pTargetSurface[y * pRenderer->uTargetSurfacePitch + x] = *pSrc++;
}

//----- (00469EA4) --------------------------------------------------------
void Mouse::_469EA4()
{
  //Mouse *v1; // esi@1
  unsigned int pTextureID; // eax@2
  Texture *pTexture; // edi@2
  unsigned int v4; // ecx@3
  unsigned int v5; // eax@3
  unsigned int v6; // ebx@5
  int v7; // ecx@15
  int v8; // ecx@25
  int v9; // ebx@26
  unsigned int v10; // eax@26
  int v11; // edx@27
  unsigned __int16 *v12; // edx@29
  unsigned __int16 *v13; // ebx@29
  unsigned int a2; // [sp+Ch] [bp-1Ch]@5
  unsigned int v15; // [sp+10h] [bp-18h]@5
  unsigned int v16; // [sp+14h] [bp-14h]@25
  unsigned __int16 *v17; // [sp+18h] [bp-10h]@25
  int v18; // [sp+1Ch] [bp-Ch]@27
  int v19; // [sp+20h] [bp-8h]@15
  unsigned __int16 *v20; // [sp+20h] [bp-8h]@28
  int pTextureWidth; // [sp+24h] [bp-4h]@12
  unsigned __int16 *v22; // [sp+24h] [bp-4h]@25

  //v1 = this;
  if ( pParty->pPickedItem.uItemID )
  {
    pTextureID = pIcons_LOD->LoadTexture(pParty->pPickedItem.GetIconName(), TEXTURE_16BIT_PALETTE);
    pTexture = (Texture *)(pTextureID != -1 ? (int)&pIcons_LOD->pTextures[pTextureID] : 0);

      v4 = pMouse->uMouseClickX;
      v5 = pMouse->uMouseClickY;

    v6 = v5;
    a2 = v4;
    v15 = v5;
    if ( (signed int)v4 <= 639 && (signed int)v5 <= 479 )
    {
      if ( (v4 & 0x80000000u) != 0 )
        a2 = 0;
      if ( (v5 & 0x80000000u) != 0 )
      {
        v6 = 0;
        v15 = 0;
      }
      if ( (signed int)(pTexture->uTextureWidth + a2) <= 640 )
        pTextureWidth = pTexture->uTextureWidth;
      else
        pTextureWidth = 640 - a2;
      if ( (signed int)(pTexture->uTextureHeight + v6) <= 480 )
      {
        v19 = pTexture->uTextureHeight;
        v7 = pTexture->uTextureHeight;
      }
      else
      {
        v7 = 480 - v6;
        v19 = 480 - v6;
      }
      if ( !this->pCursorBitmap3_sysmembits_16bit
        || a2 != this->field_40
        || v6 != this->field_44
        || a2 + pTextureWidth != this->field_48
        || v6 + v7 != this->field_4C )
      {
        if ( this->pCursorBitmap3_sysmembits_16bit )
          free(this->pCursorBitmap3_sysmembits_16bit);
        this->pCursorBitmap3_sysmembits_16bit = (unsigned __int16 *)malloc(2 * pTexture->uTextureHeight * pTexture->uTextureWidth);
        this->field_40 = a2;
        this->field_48 = a2 + pTextureWidth;
        this->field_44 = v6;
        this->field_4C = v6 + v19;
      }
      v8 = this->field_44;
      v22 = this->pCursorBitmap3_sysmembits_16bit;
      v17 = pRenderer->pTargetSurface;
      v16 = pRenderer->uTargetSurfacePitch;
      if ( v8 < this->field_4C )
      {
        v9 = this->field_48;
        v10 = pRenderer->uTargetSurfacePitch * v8;
        do
        {
          v11 = this->field_40;
          v18 = this->field_40;
          if ( v11 < v9 )
          {
            v20 = &v17[v10 + v11];
            do
            {
              v12 = v20;
              v13 = v22;
              ++v18;
              ++v20;
              ++v22;
              *v13 = *v12;
              v9 = this->field_48;
            }
            while ( v18 < v9 );
          }
          v10 += v16;
          ++v8;
        }
        while ( v8 < this->field_4C );
        v6 = v15;
      }
      if (pParty->pPickedItem.IsBroken())
        pRenderer->DrawTransparentRedShade(a2, v6, pTexture);
      else if (!pParty->pPickedItem.IsIdentified())
        pRenderer->DrawTransparentGreenShade(a2, v6, pTexture);
      else
        pRenderer->DrawTextureTransparent(a2, v6, pTexture);
    }
  }
  else
  {
    if ( this->pCursorBitmap3_sysmembits_16bit )
    {
      free(this->pCursorBitmap3_sysmembits_16bit);
      this->pCursorBitmap3_sysmembits_16bit = 0;
    }
  }
}

//----- (0046A080) --------------------------------------------------------
void Mouse::Activate(int bActive)
{
  bActive = bActive;
}

//----- (0046A08A) --------------------------------------------------------
void Mouse::SetMouseClick(int x, int y)
{
  uMouseClickX = x;
  uMouseClickY = y;
}
//----- (004175C0) --------------------------------------------------------
void UI_OnMouseLeftClick(int *pXY)
{
  int *pXY_; // esi@1
  signed int y; // eax@7
  signed int x; // ecx@7
  unsigned int v4; // eax@11
  signed int v5; // eax@17
  signed int v6; // esi@19
  int v7; // esi@30
  GUIWindow *v8; // edx@31
  GUIButton *i; // esi@37
  signed int v10; // eax@50
  int v11; // ecx@52
  unsigned int pX; // [sp+14h] [bp-8h]@7
  unsigned int pY; // [sp+18h] [bp-4h]@7

  pXY_ = pXY;
  if ( pCurrentScreen == SCREEN_VIDEO
    || sub_4637E0_is_there_popup_onscreen() )
    return;
  if ( pGUIWindow2 && pGUIWindow2->ptr_1C == (void *)33 )
  {
    sub_4452BB();
    return;
  }
  if ( pXY_ )
  {
    x = *pXY_;
    y = pXY_[1];
    pX = *pXY_;
    pY = y;
  }
  else
  {
    pMouse->GetClickPos(&pX, &pY);
    y = pY;
    x = pX;
  }

  extern bool _507B98_ctrl_pressed;
  if ( pCurrentScreen != SCREEN_GAME || !_507B98_ctrl_pressed ) // stealing cursor
    goto LABEL_30;
  v4 = GetCurrentMenuID();
  x = pX;
  if ( v4 != 0xFFFFFFFF
    || (signed int)pX < (signed int)pViewport->uViewportTL_X
    || (signed int)pX > (signed int)pViewport->uViewportBR_X )
  {
    y = pY;
LABEL_30:
    v7 = uNumVisibleWindows;
    if ( uNumVisibleWindows < 0 )
      return;
    while ( 1 )
    {
      //v8 = (GUIWindow *)&pWindowList_at_506F50_minus1_indexing[21 * pVisibleWindowsIdxs[v7]];
      v8 = &pWindowList[pVisibleWindowsIdxs[v7] - 1];
      if ( x >= (signed int)v8->uFrameX )
      {
        if ( x <= (signed int)v8->uFrameZ && y >= (signed int)v8->uFrameY && y <= (signed int)v8->uFrameW )
          break;
      }
      --v7;
      if ( v7 < 0 )
        return;
    }
    for ( i = v8->pControlsHead; ; i = i->pNext )
    {
      if ( !i )
        return;
      if ( i->uButtonType == 1 )
        goto LABEL_41;
      if ( i->uButtonType != 2 )
        break;
      if ( (signed int)(signed __int64)sqrt((double)((x - i->uX) * (x - i->uX) + (y - i->uY) * (y - i->uY))) < (signed int)i->uWidth )
        goto LABEL_50;
      y = pY;
      x = pX;
LABEL_45:
      ;
    }
    if ( i->uButtonType != 3 )
      goto LABEL_45;
LABEL_41:
    if ( x >= (signed int)i->uX && x <= (signed int)i->uZ && y >= (signed int)i->uY && y <= (signed int)i->uW )
    {
LABEL_50:
      i->field_2C_is_pushed = 1;
      v10 = pMessageQueue_50CBD0->uNumMessages;
      if ( pMessageQueue_50CBD0->uNumMessages )
      {
        v10 = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
        pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
      }
      //v11 = i->uControlParam;
      pMessageQueue_50CBD0->AddMessage(i->msg, i->msg_param, 0);
      return;
    }
    goto LABEL_45;
  }
  y = pY;
  if ( (signed int)pY < (signed int)pViewport->uViewportTL_Y || (signed int)pY > (signed int)pViewport->uViewportBR_Y )
    goto LABEL_30;
  if ( pRenderer->pRenderD3D )
    v5 = pGame->pVisInstance->get_picked_object_zbuf_val();
  else
    v5 = pRenderer->pActiveZBuffer[pX + pSRZBufferLineOffsets[pY]];
  v6 = (unsigned __int16)v5;

  auto type = PID_TYPE(v6);
  if (type == OBJECT_Actor
    && uActiveCharacter
    && v5 < 0x2000000
    && pPlayers[uActiveCharacter]->CanAct()
    && pPlayers[uActiveCharacter]->CanSteal() )
  {
    /*if ( (signed int)pMessageQueue_50CBD0->uNumMessages < 40 )
    {
      pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].eType = UIMSG_1B;
      pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].param = v6 >> 3;
      *(&pMessageQueue_50CBD0->uNumMessages + 3 * pMessageQueue_50CBD0->uNumMessages + 3) = 0;
      ++pMessageQueue_50CBD0->uNumMessages;
    }*/
    pMessageQueue_50CBD0->AddMessage(UIMSG_STEALFROMACTOR, PID_ID(v6), 0);

    if ( pParty->bTurnBasedModeOn == 1 )
    {
      if ( pTurnEngine->turn_stage == 3 )
        pTurnEngine->field_18 |= TE_FLAG_8;
    }
  }
}


//----- (0041CD4F) --------------------------------------------------------
bool UI_OnKeyDown(unsigned int vkKey)
{
  //unsigned int v1; // edi@1
  //unsigned int v2; // eax@2
  int v3; // esi@3
  int v4; // ecx@10
  GUIButton *pButton; // eax@11
  int v6; // edx@12
  int v7; // ecx@20
  char v8; // zf@21
  //GUIButton *v9; // ecx@24
  int v10; // esi@24
  //int v11; // edx@26
  int v12; // edx@28
  int v13; // esi@32
  //GUIButton *v14; // eax@37
  int v15; // edx@38
  int v17; // ecx@50
  int v18; // edx@50
  //GUIButton *v19; // ecx@54
  int v20; // esi@54
  //int v21; // edx@56
  int v22; // ecx@59
  int v23; // edx@59
  int v24; // ecx@60
  int v25; // esi@63
  //unsigned int v26; // [sp+Ch] [bp-14h]@1
  //int v27; // [sp+10h] [bp-10h]@1
  int v28; // [sp+14h] [bp-Ch]@10
  int v29; // [sp+14h] [bp-Ch]@36
  unsigned int uClickX; // [sp+18h] [bp-8h]@10
  unsigned int uClickY; // [sp+1Ch] [bp-4h]@10

  //v1 = 0;
  //v27 = uNumVisibleWindows;
  if ( uNumVisibleWindows < 0 )
    return false;
  //v2 = pMessageQueue_50CBD0->uNumMessages;
  for (int i = uNumVisibleWindows; i >= 0; --i)
  //while ( 1 )
  {
    v3 = pVisibleWindowsIdxs[i] - 1;
    if (!pWindowList[v3].receives_keyboard_input)
      continue;

    switch (vkKey)
    {
      case VK_LEFT:
      {
        v12 = pWindowList[v3].field_34;
        if ( pWindowList[v3].pCurrentPosActiveItem - pWindowList[v3].pStartingPosActiveItem - v12 >= 0 )
        {
          v8 = pCurrentScreen == SCREEN_PARTY_CREATION;
          pWindowList[v3].pCurrentPosActiveItem -= v12;
          if ( v8 )
          {
            pAudioPlayer->PlaySound(SOUND_Button, 0, 0, -1, 0, 0, 0, 0);
            //v2 = pMessageQueue_50CBD0->uNumMessages;
          }
        }
        if ( pWindowList[v3].field_30 != 0 )
        {
          break;
        }
        pButton = pWindowList[v3].pControlsHead;
        v13 = pWindowList[v3].pCurrentPosActiveItem;
        if ( v13 > 0)
        {
          do
          {
            pButton = pButton->pNext;
            --v13;
          }
          while ( v13 );
        }
        pMessageQueue_50CBD0->AddMessage(pButton->msg, pButton->msg_param, 0);
        break;
      }
      case VK_RIGHT:
      {
        v7 = pWindowList[v3].pCurrentPosActiveItem + pWindowList[v3].field_34;
        if ( v7 < pWindowList[v3].pNumPresenceButton + pWindowList[v3].pStartingPosActiveItem )
        {
          v8 = pCurrentScreen == SCREEN_PARTY_CREATION;
          pWindowList[v3].pCurrentPosActiveItem = v7;
          if ( v8 )
          {
            pAudioPlayer->PlaySound(SOUND_Button, 0, 0, -1, 0, 0, 0, 0);
            //v2 = pMessageQueue_50CBD0->uNumMessages;
          }
        }
        if ( pWindowList[v3].field_30 != 0 )
        {
          break;
        }
        pButton = pWindowList[v3].pControlsHead;
        v10 = pWindowList[v3].pCurrentPosActiveItem;
        if ( v10 > 0)
        {
          do
          {
            pButton = pButton->pNext;
            --v10;
          }
          while ( v10 );
        }
        pMessageQueue_50CBD0->AddMessage(pButton->msg, pButton->msg_param, 0);
        break;
      }
      case VK_DOWN:
      {
        v17 = pWindowList[v3].pStartingPosActiveItem;
        v18 = pWindowList[v3].pCurrentPosActiveItem;
        if ( v18 >= pWindowList[v3].pNumPresenceButton + v17 - 1 )
          pWindowList[v3].pCurrentPosActiveItem = v17;
        else
          pWindowList[v3].pCurrentPosActiveItem = v18 + 1;
        if ( pWindowList[v3].field_30 != 0 )
          return true;
        pButton = pWindowList[v3].pControlsHead;
        v20 = pWindowList[v3].pCurrentPosActiveItem;
        if ( v20 > 0)
        {
          do
          {
            pButton = pButton->pNext;
            --v20;
          }
          while ( v20 );
        }
        pMessageQueue_50CBD0->AddMessage(pButton->msg, pButton->msg_param, 0);
        return true;
      }
      case VK_SELECT:
      {
        pMouse->GetClickPos(&uClickX, &uClickY);
        v4 = pWindowList[v3].pStartingPosActiveItem;
        v28 = v4 + pWindowList[v3].pNumPresenceButton;
        if ( v4 < v4 + pWindowList[v3].pNumPresenceButton )
        {
          while ( 1 )
          {
            pButton = pWindowList[v3].pControlsHead;
            if ( v4 > 0 )
            {
              v6 = v4;
              do
              {
                pButton = pButton->pNext;
                --v6;
              }
              while ( v6 );
            }
            if ( (signed int)uClickX >= (signed int)pButton->uX//test for StatsTab in PlayerCreation Window
               && (signed int)uClickX <= (signed int)pButton->uZ
               && (signed int)uClickY >= (signed int)pButton->uY
               && (signed int)uClickY <= (signed int)pButton->uW )
              break;
            ++v4;
            if ( v4 >= v28 )
            {
              //v1 = 0;
              //v2 = pMessageQueue_50CBD0->uNumMessages;
              //--i;
              //if ( i < 0 )
                return false;
              //continue;
            }
          }
          pWindowList[v3].pCurrentPosActiveItem = v4;
          return true;
        }
        //v2 = pMessageQueue_50CBD0->uNumMessages;
        break;
      }
      case VK_UP:
      {
        v22 = pWindowList[v3].pCurrentPosActiveItem;
        v23 = pWindowList[v3].pStartingPosActiveItem;
        if ( v22 <= v23 )
          v24 = pWindowList[v3].pNumPresenceButton + v23 - 1;
        else
          v24 = v22 - 1;
        v8 = pWindowList[v3].field_30 == 0;
        pWindowList[v3].pCurrentPosActiveItem = v24;
        if ( !v8 )
          return true;
        pButton = pWindowList[v3].pControlsHead;
        v25 = pWindowList[v3].pCurrentPosActiveItem;
        if ( v25 > 0)
        {
          do
          {
            pButton = pButton->pNext;
            --v25;
          }
          while ( v25 );
        }
        pMessageQueue_50CBD0->AddMessage(pButton->msg, pButton->msg_param, 0);
        return true;
      }
      case VK_NEXT:
      {  
        //if ( pWindowList[v3].field_30 != 0 )   //crashed at skill draw
        //{
        //  pMouse->GetClickPos(&uClickX, &uClickY);
        //  v4 = pWindowList[v3].pStartingPosActiveItem;
        //  v29 = v4 + pWindowList[v3].pNumPresenceButton; //num buttons more than buttons 
        //  if ( v4 < v29 )
        //  {
        //    while ( 1 )
        //    {
        //      pButton = pWindowList[v3].pControlsHead;
        //      if ( v4 > 0 )
        //      {
        //        v15 = v4;
        //        do
        //        {
        //          pButton = pButton->pNext;
        //          --v15;
        //        }
        //        while ( v15 );
        //      }
        //      if ( (signed int)uClickX >= (signed int)pButton->uX
        //        && (signed int)uClickX <= (signed int)pButton->uZ
        //        && (signed int)uClickY >= (signed int)pButton->uY
        //        && (signed int)uClickY <= (signed int)pButton->uW )
        //      {
        //        pWindowList[v3].pCurrentPosActiveItem = v4;
        //        return true;
        //      }
        //      ++v4;
        //      if ( v4 >= v29 )
        //      {
        //        //v1 = 0;
        //        //v2 = pMessageQueue_50CBD0->uNumMessages;
        //        break;
        //      }
        //    }
        //  }
        //  else
        //  {
        //    //v2 = pMessageQueue_50CBD0->uNumMessages;
        //  }
        //}
        break;
      }
      default:
      {
        break;
      }
    }
  }
}