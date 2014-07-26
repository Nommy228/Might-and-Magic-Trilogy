#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define _CRT_SECURE_NO_WARNINGS
#include "Viewport.h"

#include "mm7_unsorted_subs.h"
#include "Party.h"
#include "Indoor.h"
#include "OurMath.h"
#include "mm7_data.h"
#include "Actor.h"
#include "Outdoor.h"
#include "Events.h"
#include "BSPModel.h"
#include "Mouse.h"
#include "SpriteObject.h"
#include "ObjectList.h"
#include "DecorationList.h"
#include "texts.h"
#include "Game.h"
#include "Vis.h"
#include "LOD.h"
#include "GUIWindow.h"
#include "TurnEngine.h"
#include "stru123.h"
#include "MM7.h"
#include "Level/Decoration.h"


//----- (004C0262) --------------------------------------------------------
void Viewport::SetScreen( signed int sTL_X, signed int sTL_Y, signed int sBR_X, signed int sBR_Y )
    {
    unsigned int tl_x; // edx@1
    unsigned int br_x; // esi@1
    unsigned int tl_y; // edi@3
    unsigned int br_y; // eax@3

    tl_x = sTL_X;
    br_x = sBR_X;
    if ( sTL_X > sBR_X )
    {
      br_x = sTL_X;                           // swap x's
      tl_x = sBR_X;
    }
    tl_y = sTL_Y;
    br_y = sBR_Y;
    if ( sTL_Y > sBR_Y )
    {
      br_y = sTL_Y;                           // swap y's 
      tl_y = sBR_Y;
    }
    this->uScreen_TL_X = tl_x;
    this->uScreen_TL_Y = tl_y;
    this->uScreen_BR_X = br_x;
    this->uScreen_BR_Y = br_y;
    this->uScreenWidth = br_x - tl_x + 1;
    this->uScreenHeight = br_y - tl_y + 1;
    this->uScreenCenterX = (signed int)(br_x + tl_x) /2;
    //if ( pRenderer->pRenderD3D == 0 )
    //    this->uScreenCenterY = this->uScreen_BR_Y - fixpoint_mul(field_30, uScreenHeight);
    //else
        this->uScreenCenterY = (br_y + tl_y)/2;
    SetViewport(this->uScreen_TL_X, this->uScreen_TL_Y, this->uScreen_BR_X, this->uScreen_BR_Y);
    }

//----- (004C02F8) --------------------------------------------------------
void Viewport::SetFOV(int field_of_view)
{
  this->field_of_view = field_of_view;
  SetScreen(uScreen_TL_X, uScreen_TL_Y, uScreen_BR_X, uScreen_BR_Y);
}

//----- (004C0312) --------------------------------------------------------
void Viewport::SetViewport( signed int sTL_X, signed int sTL_Y, signed int sBR_X, signed int sBR_Y )
    {
    signed int tl_x; // ebx@1
    signed int tl_y; // edi@3
    signed int br_x; // edx@5
    signed int br_y; // eax@7

    tl_x = sTL_X;
    if ( sTL_X < this->uScreen_TL_X )
        tl_x = this->uScreen_TL_X;
    tl_y = sTL_Y;
    if ( sTL_Y < this->uScreen_TL_Y )
        tl_y = this->uScreen_TL_Y;
    br_x = sBR_X;
    if ( sBR_X > this->uScreen_BR_X )
        br_x = this->uScreen_BR_X;
    br_y = sBR_Y;
    if ( sBR_Y > this->uScreen_BR_Y )
        br_y = this->uScreen_BR_Y;
    this->uViewportTL_Y = tl_y;
    this->uViewportTL_X = tl_x;
    this->uViewportBR_X = br_x;
    this->uViewportBR_Y = br_y;
    }

//----- (00443219) --------------------------------------------------------
 void ViewingParams::_443219()
 {
   this->sViewCenterY += 512;
   
   AdjustPosition();
 }

//----- (00443225) --------------------------------------------------------
void ViewingParams::_443225()
{
  this->sViewCenterX -= 512;
  
  AdjustPosition();
}

//----- (00443231) --------------------------------------------------------
void ViewingParams::_443231()
{
  this->sViewCenterY -= 512;

  AdjustPosition();
}

//----- (0044323D) --------------------------------------------------------
void ViewingParams::_44323D()
{
  this->sViewCenterX += 512;

  AdjustPosition();
}

//----- (00443249) --------------------------------------------------------
void ViewingParams::CenterOnParty()
{
  this->uMapBookMapZoom = fixpoint_mul(0x8000, this->uMapBookMapZoom);
  if (this->uMapBookMapZoom < 384)
    this->uMapBookMapZoom = 384;

  this->sViewCenterX = pParty->vPosition.x;
  this->sViewCenterY = pParty->vPosition.y;

  AdjustPosition();
}

//----- (00443291) --------------------------------------------------------
void ViewingParams::CenterOnParty2()
{
    int v2; // eax@1

    if (uCurrentlyLoadedLevelType == LEVEL_Outdoor)
      v2 = 1536;
    else if (uCurrentlyLoadedLevelType == LEVEL_Indoor)
      v2 = 3072;
    else assert(false);
    
    this->uMapBookMapZoom *= 2;
    if (this->uMapBookMapZoom > v2 )
        this->uMapBookMapZoom = v2;

    this->sViewCenterX = pParty->vPosition.x;
    this->sViewCenterY = pParty->vPosition.y;
    AdjustPosition();
}

//----- (004432E7) --------------------------------------------------------
void ViewingParams::AdjustPosition()
{
    ViewingParams *v1; // esi@1
    int v2; // ebx@1
    signed int v3; // edx@1
    int v4; // ecx@1
    int v5; // edi@3
    int v6; // eax@3
    int v7; // eax@5


    v1 = this;
    v2 = this->indoor_center_y;
    v3 = 88 >> (this->uMapBookMapZoom / 384);
    v4 = (44 - v3) << 9;
    if ( v1->sViewCenterY > v2 + v4 )
        v1->sViewCenterY = v2 + v4;

    v5 = v1->indoor_center_x;
    v6 = (v3 - 44) << 9;
    if ( v1->sViewCenterX < v5 + v6 )
        v1->sViewCenterX = v5 + v6;

    v7 = v2 + v6;
    if ( v1->sViewCenterY < v7 )
        v1->sViewCenterY = v7;

    if ( v1->sViewCenterX > v5 + v4 )
        v1->sViewCenterX = v5 + v4;
    }

//----- (00443343) --------------------------------------------------------
void ViewingParams::InitGrayPalette()
    {
    for  (unsigned short i=0; i<256; ++i)
        pPalette[i]=Color16(i, i, i);
    }

//----- (00443365) --------------------------------------------------------
void ViewingParams::_443365()
{
  Vec3_short_ *v3; // eax@4
  Vec3_short_ *v6; // eax@12
  int minimum_y; // [sp+10h] [bp-10h]@2
  int maximum_y; // [sp+14h] [bp-Ch]@2
  int minimum_x; // [sp+18h] [bp-8h]@2
  int maximum_x; // [sp+1Ch] [bp-4h]@2

  InitGrayPalette();
  if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
  {
    minimum_x = 0x40000000;
    minimum_y = 0x40000000;

    maximum_x = -0x40000000;
    maximum_y = -0x40000000;
    for (int i=0; i<pIndoor->pMapOutlines->uNumOutlines; ++i)
    {
      v3 = &pIndoor->pVertices[pIndoor->pMapOutlines->pOutlines[i].uFace1ID];

      if ( v3->x < minimum_x )
        minimum_x = v3->x;
      if ( v3->x > maximum_x )
        maximum_x = v3->x;
      if ( v3->y < minimum_y )
        minimum_y = v3->x;
      if ( v3->y > maximum_y )
        maximum_y = v3->x;

      v6 = &pIndoor->pVertices[pIndoor->pMapOutlines->pOutlines[i].uFace2ID];

      if ( v6->x < minimum_x )
        minimum_x = v3->x;
      if ( v6->x > maximum_x )
        maximum_x = v3->x;

      if ( v6->y < minimum_y )
        minimum_y = v3->y;
      if ( v6->y > maximum_y )
        maximum_y = v3->y;
    }

    uMinimapZoom = 1024;
    indoor_center_x = (signed int)(minimum_x + maximum_x) / 2;
    field_28 = 10;
    indoor_center_y = (signed int)(minimum_y + maximum_y) / 2;
  }
  else
  {
    indoor_center_x = 0;
    indoor_center_y = 0;
    uMinimapZoom = _576E2C_current_minimap_zoom;
    field_28 = dword_576E28;
  }
  uMapBookMapZoom = 384;
}
//----- (0042213C) --------------------------------------------------------
void OnGameViewportClick()
{
  signed int v0; // ebx@2
  POINT *v1; // esi@3
  signed int v6; // eax@14
  char *v7; // esi@15
  int v9; // eax@19
  unsigned int pTextureID; // eax@19
  int pEventID; // ecx@21
  int v15; // ecx@29
//  signed int v16; // edx@30
//  int v18; // ebx@47
//  signed int v21; // eax@58
  SpriteObject a1; // [sp+Ch] [bp-80h]@1
  POINT a2; // [sp+84h] [bp-8h]@3

  int clickable_distance = 512;

  v1 = pMouse->GetCursorPos(&a2);
  //if ( pRenderer->pRenderD3D )
    v0 = pGame->pVisInstance->get_picked_object_zbuf_val();
  //else
  //  v0 = pRenderer->pActiveZBuffer[v1->x + pSRZBufferLineOffsets[v1->y]];

  if ( PID_TYPE(v0) == OBJECT_Item)
  {
    int item_id = PID_ID(v0);
    //v21 = (signed int)(unsigned __int16)v0 >> 3;
    if ( !(pObjectList->pObjects[pSpriteObjects[item_id].uObjectDescID].uFlags & 0x10) && item_id < 1000 && pSpriteObjects[item_id].uObjectDescID
      && (unsigned int)v0 < 0x2000000 )
    {
      if ( pSpriteObjects[item_id].stru_24.GetItemEquipType() == 18 )
      {
        pParty->PartyFindsGold(pSpriteObjects[item_id].stru_24.uSpecEnchantmentType, 0);
        viewparams->bRedrawGameUI = 1;
      }
      else
      {
        sprintfex(pTmpBuf2.data(), pGlobalTXT_LocalizationStrings[471], pItemsTable->pItems[pSpriteObjects[item_id].stru_24.uItemID].pUnidentifiedName);//You found an item (%s)!
        ShowStatusBarString(pTmpBuf2.data(), 2);
        if ( pSpriteObjects[item_id].stru_24.uItemID == ITEM_ARTIFACT_SPLITTER )
          _449B7E_toggle_bit(pParty->_quest_bits, 184, 1);
        if ( pSpriteObjects[item_id].stru_24.uItemID == 455 )
          _449B7E_toggle_bit(pParty->_quest_bits, 185, 1);
        if ( !pParty->AddItemToParty(&pSpriteObjects[item_id].stru_24) )
          pParty->SetHoldingItem(&pSpriteObjects[item_id].stru_24);
      }
      SpriteObject::OnInteraction(item_id);
      return;
    }
    if ( !pParty->pPickedItem.uItemID )
      return;
		__debugbreak();//no checker
    v6 = 0;
    a1.uType = pItemsTable->pItems[pParty->pPickedItem.uItemID].uSpriteID;
    if ( (signed int)pObjectList->uNumObjects <= 0 )
      LOWORD(v6) = 0;
    else
    {
      v7 = (char *)&pObjectList->pObjects->uObjectID;
      while ( pItemsTable->pItems[pParty->pPickedItem.uItemID].uSpriteID != *(short *)v7 )
      {
        ++v6;
        v7 += 56;
        if ( v6 >= (signed int)pObjectList->uNumObjects )
        {
          LOWORD(v6) = 0;
          break;
        }
      }
    }
    a1.uObjectDescID = v6;
    a1.vPosition.y = pParty->vPosition.y;
    a1.spell_caster_pid = OBJECT_Player;
    a1.vPosition.x = pParty->vPosition.x;
    a1.vPosition.z = pParty->sEyelevel + pParty->vPosition.z;
    a1.uSoundID = 0;
    a1.uFacing = 0;
    a1.uAttributes = 8;
    a1.uSectorID = pIndoor->GetSector(pParty->vPosition.x, pParty->vPosition.y, pParty->sEyelevel + pParty->vPosition.z);
    a1.uSpriteFrameID = 0;
    memcpy(&a1.stru_24, &pParty->pPickedItem, 0x24u);

    extern int UnprojectX(int);
    //v9 = UnprojectX(v1->x);
    a1.Create(pParty->sRotationY + UnprojectX(v1->x), 184, 200, 0);
    pTextureID = pIcons_LOD->LoadTexture(pParty->pPickedItem.GetIconName(), TEXTURE_16BIT_PALETTE);
    if (pTextureID != -1)
      pIcons_LOD->pTextures[pTextureID].Release();
    pMouse->RemoveHoldingItem();
    pIcons_LOD->SyncLoadedFilesCount();
    return;
  }
  if ( PID_TYPE(v0) == OBJECT_Actor)
  {
    int mon_id = PID_ID(v0);
    //a2.y = v16;
    if ( pActors[mon_id].uAIState == Dead )
    {
      if ( (unsigned int)v0 < 0x2000000 )
      {
        pActors[mon_id].LootActor();
        return;
      }
      if ( !pParty->pPickedItem.uItemID )
        return;
		__debugbreak();//no checker
      v6 = 0;
      a1.uType = pItemsTable->pItems[pParty->pPickedItem.uItemID].uSpriteID;
      if ( (signed int)pObjectList->uNumObjects <= 0 )
        LOWORD(v6) = 0;
      else
      {
        v7 = (char *)&pObjectList->pObjects->uObjectID;
        while ( pItemsTable->pItems[pParty->pPickedItem.uItemID].uSpriteID != *(short *)v7 )
        {
          ++v6;
          v7 += 56;
          if ( v6 >= (signed int)pObjectList->uNumObjects )
          {
            LOWORD(v6) = 0;
            break;
          }
        }
      }
      a1.uObjectDescID = v6;
      a1.vPosition.y = pParty->vPosition.y;
      a1.spell_caster_pid = OBJECT_Player;
      a1.vPosition.x = pParty->vPosition.x;
      a1.vPosition.z = pParty->sEyelevel + pParty->vPosition.z;
      a1.uSoundID = 0;
      a1.uFacing = 0;
      a1.uAttributes = 8;
      a1.uSectorID = pIndoor->GetSector(pParty->vPosition.x, pParty->vPosition.y, pParty->sEyelevel + pParty->vPosition.z);
      a1.uSpriteFrameID = 0;
      memcpy(&a1.stru_24, &pParty->pPickedItem, 0x24u);

      extern int UnprojectX(int);
      //v9 = UnprojectX(v1->x);
      a1.Create(pParty->sRotationY + UnprojectX(v1->x), 184, 200, 0);
      pTextureID = pIcons_LOD->LoadTexture(pParty->pPickedItem.GetIconName(), TEXTURE_16BIT_PALETTE);
      if (pTextureID != -1)
        pIcons_LOD->pTextures[pTextureID].Release();
      pMouse->RemoveHoldingItem();
      pIcons_LOD->SyncLoadedFilesCount();
      return;
    }
    if ( GetAsyncKeyState(VK_SHIFT) >= 0 )
    {
      if ( !pActors[mon_id].GetActorsRelation(0) && !(pActors[mon_id].uAttributes & 0x80000) )
      {
        if ( HIWORD(v0) >= clickable_distance)
        {
          if ( !pParty->pPickedItem.uItemID )
            return;
		__debugbreak();//no checker
          v6 = 0;
          a1.uType = pItemsTable->pItems[pParty->pPickedItem.uItemID].uSpriteID;
          if ( (signed int)pObjectList->uNumObjects <= 0 )
            LOWORD(v6) = 0;
          else
          {
            v7 = (char *)&pObjectList->pObjects->uObjectID;
            while ( pItemsTable->pItems[pParty->pPickedItem.uItemID].uSpriteID != *(short *)v7 )
            {
              ++v6;
              v7 += 56;
              if ( v6 >= (signed int)pObjectList->uNumObjects )
              {
                LOWORD(v6) = 0;
                break;
              }
            }
          }
          a1.uObjectDescID = v6;
          a1.vPosition.y = pParty->vPosition.y;
          a1.spell_caster_pid = OBJECT_Player;
          a1.vPosition.x = pParty->vPosition.x;
          a1.vPosition.z = pParty->sEyelevel + pParty->vPosition.z;
          a1.uSoundID = 0;
          a1.uFacing = 0;
          a1.uAttributes = 8;
          a1.uSectorID = pIndoor->GetSector(pParty->vPosition.x, pParty->vPosition.y, pParty->sEyelevel + pParty->vPosition.z);
          a1.uSpriteFrameID = 0;
          memcpy(&a1.stru_24, &pParty->pPickedItem, 0x24u);

          extern int UnprojectX(int);
          //v9 = UnprojectX(v1->x);
          a1.Create(pParty->sRotationY + UnprojectX(v1->x), 184, 200, 0);
          pTextureID = pIcons_LOD->LoadTexture(pParty->pPickedItem.GetIconName(), TEXTURE_16BIT_PALETTE);
          if (pTextureID != -1)
            pIcons_LOD->pTextures[pTextureID].Release();
          pMouse->RemoveHoldingItem();
          pIcons_LOD->SyncLoadedFilesCount();
          return;
        }
        if ( !pActors[mon_id].CanAct() )
          return;
        //v18 = mon_id;
        Actor::AI_FaceObject(mon_id, 4, 0, 0);
        if ( !pActors[mon_id].sNPC_ID )
        {
          if ( pNPCStats->pGroups_copy[pActors[mon_id].uGroup] )
          {
            if ( pNPCStats->pCatchPhrases[pNPCStats->pGroups_copy[pActors[mon_id].uGroup]] )
            {
              pParty->uFlags |= 2;
              strcpy(byte_5B0938.data(), pNPCStats->pCatchPhrases[pNPCStats->pGroups_copy[pActors[mon_id].uGroup]]);
              sub_4451A8_press_any_key(0, 0, 0);
            }
          }
          return;
        }
        pMessageQueue_50CBD0->AddGUIMessage(UIMSG_StartNPCDialogue, mon_id, 0);
        return;
      }
      if ( pParty->bTurnBasedModeOn == true && pTurnEngine->turn_stage == TE_MOVEMENT )
      {
        pTurnEngine->field_18 |= TE_FLAG_8;
        return;
      }
      pMessageQueue_50CBD0->AddGUIMessage(UIMSG_Attack, 0, 0);
    }
    else
    {
      if ( pParty->bTurnBasedModeOn == true && pTurnEngine->turn_stage == TE_MOVEMENT )
      {
        pParty->uFlags |= PARTY_FLAGS_1_FALLING;
        return;
      }
      if ( uActiveCharacter && sub_427769_isSpellQuickCastableOnShiftClick(pPlayers[uActiveCharacter]->uQuickSpell))
        pMessageQueue_50CBD0->AddGUIMessage(UIMSG_CastQuickSpell, 0, 0);
    }
    return;
  }
  if ( PID_TYPE(v0) == OBJECT_Decoration)
  {
    if ( (signed int)(((unsigned int)v0 >> 16) - pDecorationList->pDecorations[pLevelDecorations[(signed int)(unsigned __int16)v0 >> 3].uDecorationDescID].uRadius) >= clickable_distance )
    {
      if ( !pParty->pPickedItem.uItemID )
        return;
		__debugbreak();//no checker
      v6 = 0;
      a1.uType = pItemsTable->pItems[pParty->pPickedItem.uItemID].uSpriteID;
      if ( (signed int)pObjectList->uNumObjects <= 0 )
        LOWORD(v6) = 0;
      else
      {
        v7 = (char *)&pObjectList->pObjects->uObjectID;
        while ( pItemsTable->pItems[pParty->pPickedItem.uItemID].uSpriteID != *(short *)v7 )
        {
          ++v6;
          v7 += 56;
          if ( v6 >= (signed int)pObjectList->uNumObjects )
          {
            LOWORD(v6) = 0;
            break;
          }
        }
      }
      a1.uObjectDescID = v6;
      a1.vPosition.y = pParty->vPosition.y;
      a1.spell_caster_pid = OBJECT_Player;
      a1.vPosition.x = pParty->vPosition.x;
      a1.vPosition.z = pParty->sEyelevel + pParty->vPosition.z;
      a1.uSoundID = 0;
      a1.uFacing = 0;
      a1.uAttributes = 8;
      a1.uSectorID = pIndoor->GetSector(pParty->vPosition.x, pParty->vPosition.y, pParty->sEyelevel + pParty->vPosition.z);
      a1.uSpriteFrameID = 0;
      memcpy(&a1.stru_24, &pParty->pPickedItem, 0x24u);

      extern int UnprojectX(int);
      //v9 = UnprojectX(v1->x);
      a1.Create(pParty->sRotationY + UnprojectX(v1->x), 184, 200, 0);
      pTextureID = pIcons_LOD->LoadTexture(pParty->pPickedItem.GetIconName(), TEXTURE_16BIT_PALETTE);
      if (pTextureID != -1)
        pIcons_LOD->pTextures[pTextureID].Release();
      pMouse->RemoveHoldingItem();
      pIcons_LOD->SyncLoadedFilesCount();
      return;
    }
    if ( !pLevelDecorations[(signed int)(unsigned __int16)v0 >> 3].uEventID )
    {
      if ( pLevelDecorations[(signed int)(unsigned __int16)v0 >> 3].IsInteractive() )
      {
        v15 = stru_5E4C90_MapPersistVars._decor_events[pLevelDecorations[(signed int)(unsigned __int16)v0 >> 3]._idx_in_stru123 - 75] + 380;
        activeLevelDecoration = &pLevelDecorations[(signed int)(unsigned __int16)v0 >> 3];
        EventProcessor(v15, 0, 1);
        activeLevelDecoration = nullptr;
      }
      return;
    }
    pEventID = pLevelDecorations[(signed int)(unsigned __int16)v0 >> 3].uEventID;
    EventProcessor(pEventID, (unsigned __int16)v0, 1);
    return;
  }
  if ( PID_TYPE(v0) == OBJECT_BModel && HIWORD(v0) < clickable_distance)
  {
    if ( uCurrentlyLoadedLevelType == LEVEL_Indoor)
    {
      if ( !pIndoor->pFaces[PID_ID(v0)].Clickable() )
      {
        if ( !pParty->pPickedItem.uItemID )
        {
          ShowNothingHereStatus();
          if ( !pParty->pPickedItem.uItemID )
            return;
        }
		__debugbreak();//no checker
        v6 = 0;
        a1.uType = pItemsTable->pItems[pParty->pPickedItem.uItemID].uSpriteID;
        if ( (signed int)pObjectList->uNumObjects <= 0 )
          LOWORD(v6) = 0;
        else
        {
          v7 = (char *)&pObjectList->pObjects->uObjectID;
          while ( pItemsTable->pItems[pParty->pPickedItem.uItemID].uSpriteID != *(short *)v7 )
          {
            ++v6;
            v7 += 56;
            if ( v6 >= (signed int)pObjectList->uNumObjects )
            {
              LOWORD(v6) = 0;
              break;
            }
          }
        }
        a1.uObjectDescID = v6;
        a1.vPosition.y = pParty->vPosition.y;
        a1.spell_caster_pid = OBJECT_Player;
        a1.vPosition.x = pParty->vPosition.x;
        a1.vPosition.z = pParty->sEyelevel + pParty->vPosition.z;
        a1.uSoundID = 0;
        a1.uFacing = 0;
        a1.uAttributes = 8;
        a1.uSectorID = pIndoor->GetSector(pParty->vPosition.x, pParty->vPosition.y, pParty->sEyelevel + pParty->vPosition.z);
        a1.uSpriteFrameID = 0;
        memcpy(&a1.stru_24, &pParty->pPickedItem, 0x24u);

        extern int UnprojectX(int);
        v9 = UnprojectX(v1->x);
        a1.Create(pParty->sRotationY + v9, 184, 200, 0);
        pTextureID = pIcons_LOD->LoadTexture(pParty->pPickedItem.GetIconName(), TEXTURE_16BIT_PALETTE);
        if (pTextureID != -1)
          pIcons_LOD->pTextures[pTextureID].Release();
        pMouse->RemoveHoldingItem();
        pIcons_LOD->SyncLoadedFilesCount();
        return;
      }
      pEventID = pIndoor->pFaceExtras[pIndoor->pFaces[PID_ID(v0)].uFaceExtraID].uEventID;
    }
    if ( uCurrentlyLoadedLevelType == LEVEL_Outdoor)
    {
      if ( !pOutdoor->pBModels[(signed int)(v0 & 0xFFFF) >> 9].pFaces[PID_ID(v0) & 0x3F].Clickable())
      {
       if ( !pParty->pPickedItem.uItemID )
        {
          ShowNothingHereStatus();
          if ( !pParty->pPickedItem.uItemID )
            return;
        }
		__debugbreak();//no checker
        v6 = 0;
        a1.uType = pItemsTable->pItems[pParty->pPickedItem.uItemID].uSpriteID;
        if ( (signed int)pObjectList->uNumObjects <= 0 )
          LOWORD(v6) = 0;
        else
        {
          v7 = (char *)&pObjectList->pObjects->uObjectID;
          while ( pItemsTable->pItems[pParty->pPickedItem.uItemID].uSpriteID != *(short *)v7 )
          {
            ++v6;
            v7 += 56;
            if ( v6 >= (signed int)pObjectList->uNumObjects )
            {
              LOWORD(v6) = 0;
              break;
            }
          }
        }
        a1.uObjectDescID = v6;
        a1.vPosition.y = pParty->vPosition.y;
        a1.spell_caster_pid = OBJECT_Player;
        a1.vPosition.x = pParty->vPosition.x;
        a1.vPosition.z = pParty->sEyelevel + pParty->vPosition.z;
        a1.uSoundID = 0;
        a1.uFacing = 0;
        a1.uAttributes = 8;
        a1.uSectorID = pIndoor->GetSector(pParty->vPosition.x, pParty->vPosition.y, pParty->sEyelevel + pParty->vPosition.z);
        a1.uSpriteFrameID = 0;
        memcpy(&a1.stru_24, &pParty->pPickedItem, 0x24u);

        extern int UnprojectX(int);
        v9 = UnprojectX(v1->x);
        a1.Create(pParty->sRotationY + v9, 184, 200, 0);
        pTextureID = pIcons_LOD->LoadTexture(pParty->pPickedItem.GetIconName(), TEXTURE_16BIT_PALETTE);
        if (pTextureID != -1)
          pIcons_LOD->pTextures[pTextureID].Release();
        pMouse->RemoveHoldingItem();
        pIcons_LOD->SyncLoadedFilesCount();
        return;
      }
      pEventID = pOutdoor->pBModels[(signed int)(v0 & 0xFFFF) >> 9].pFaces[PID_ID(v0) & 0x3F].sCogTriggeredID;
    }
    EventProcessor(pEventID, (unsigned __int16)v0, 1);
    return;
  }
  if ( PID_TYPE(v0) != OBJECT_BModel || HIWORD(v0) >= clickable_distance )
  {
    if ( !pParty->pPickedItem.uItemID )
      return;
	//__debugbreak();//no checker
    v6 = 0;
    a1.uType = pItemsTable->pItems[pParty->pPickedItem.uItemID].uSpriteID;
    if ( (signed int)pObjectList->uNumObjects <= 0 )
      LOWORD(v6) = 0;
    else
    {
      v7 = (char *)&pObjectList->pObjects->uObjectID;
      while ( pItemsTable->pItems[pParty->pPickedItem.uItemID].uSpriteID != *(short *)v7 )
      {
        ++v6;
        v7 += 56;
        if ( v6 >= (signed int)pObjectList->uNumObjects )
        {
          LOWORD(v6) = 0;
          break;
        }
      }
    }
    a1.uObjectDescID = v6;
    a1.vPosition.y = pParty->vPosition.y;
    a1.spell_caster_pid = OBJECT_Player;
    a1.vPosition.x = pParty->vPosition.x;
    a1.vPosition.z = pParty->sEyelevel + pParty->vPosition.z;
    a1.uSoundID = 0;
    a1.uFacing = 0;
    a1.uAttributes = 8;
    a1.uSectorID = pIndoor->GetSector(pParty->vPosition.x, pParty->vPosition.y, pParty->sEyelevel + pParty->vPosition.z);
    a1.uSpriteFrameID = 0;
    memcpy(&a1.stru_24, &pParty->pPickedItem, 0x24u);

    extern int UnprojectX(int);
    //v9 = UnprojectX(v1->x);
    a1.Create(pParty->sRotationY + UnprojectX(v1->x), 184, 200, 0);
    pTextureID = pIcons_LOD->LoadTexture(pParty->pPickedItem.GetIconName(), TEXTURE_16BIT_PALETTE);
    if (pTextureID != -1)
      pIcons_LOD->pTextures[pTextureID].Release();
    pMouse->RemoveHoldingItem();
    pIcons_LOD->SyncLoadedFilesCount();
    return;
  }
}
