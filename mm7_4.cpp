#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "Texture.h"
#include "mm7_data.h"
#include "VideoPlayer.h"
#include "Sprites.h"
#include "BSPModel.h"
#include "Mouse.h"
#include "stru6.h"

#include "LightmapBuilder.h"
#include "MM7.h"
#include "MapInfo.h"
#include "Game.h"
#include "GUIWindow.h"
#include "GUIFont.h"
#include "Party.h"
#include "AudioPlayer.h"
#include "Outdoor.h"
#include "Outdoor_stuff.h"
#include "LOD.h"
#include "Actor.h"
#include "Events.h"
#include "Viewport.h"
#include "FrameTableInc.h"
#include "Math.h"
#include "SpriteObject.h"
#include "ObjectList.h"
#include "DecorationList.h"
#include "Time.h"
#include "IconFrameTable.h"
#include "PlayerFrameTable.h"
#include "Awards.h"
#include "TurnEngine.h"
#include "Events2D.h"
#include "stru159.h"
#include "texts.h"
#include "Log.h"
#include "UI\UIHouses.h"
#include "Lights.h"
#include "Level/Decoration.h"

//----- (0046CC4B) --------------------------------------------------------
void check_event_triggers()
{
  LevelDecoration *v1; // esi@2

  for (size_t i = 0; i < num_event_triggers; i++)
  {
    v1 = &pLevelDecorations[event_triggers[i]];

    if (v1->uFlags & LEVEL_DECORATION_TRIGGERED_BY_TOUCH
        && v1->vPosition.GetDistanceTo(pParty->vPosition) < v1->uTriggerRange)
    {
      EventProcessor(v1->uEventID, PID(OBJECT_Decoration,i), 1);
    }
    else if (v1->uFlags & LEVEL_DECORATION_TRIGGERED_BY_MONSTER)
    {
      for (size_t j = 0; j < uNumActors; j++)
      {
        if (v1->vPosition.GetDistanceTo(pActors[j].vPosition) < v1->uTriggerRange)
          EventProcessor(v1->uEventID, 0, 1);
      }
    }
    else if (v1->uFlags & LEVEL_DECORATION_TRIGGERED_BY_OBJECT)
    {
      for (size_t j = 0; j < uNumSpriteObjects; j++)
      {
        if (v1->vPosition.GetDistanceTo(pSpriteObjects[j].vPosition) < v1->uTriggerRange)
          EventProcessor(v1->uEventID, 0, 1);
      }
    }
  }
}
// 6836C8: using guessed type int 6836C8_num_decorations_6807E8;

//----- (0046D8E3) --------------------------------------------------------
int __fastcall sub_46D8E3(int a1, signed int a2, int a3, int a4)
{
  int v4; // ebx@1
  unsigned int v5; // ecx@1
  BSPModel *v6; // edi@3
  ODMFace *v7; // esi@10
  unsigned __int8 v8; // al@10
  signed __int16 *v9; // eax@18
  int v10; // edx@19
  int v11; // ecx@21
  int v12; // ecx@23
  signed int v13; // eax@25
  int v14; // edx@27
  int v15; // edx@29
  int v16; // ST18_4@29
  signed int v17; // edx@29
  signed __int64 v18; // qtt@29
  int v19; // eax@35
  signed int v20; // ecx@37
  signed int v22; // ebx@42
  unsigned int v23; // esi@43
  int v24; // edx@44
  int v25; // eax@44
  int v26; // [sp+Ch] [bp-38h]@7
  int v27; // [sp+10h] [bp-34h]@21
  int v28; // [sp+18h] [bp-2Ch]@21
  int v29; // [sp+1Ch] [bp-28h]@2
  unsigned int v30; // [sp+20h] [bp-24h]@8
  signed int v31; // [sp+24h] [bp-20h]@1
  signed int v32; // [sp+28h] [bp-1Ch]@1
  int v33; // [sp+2Ch] [bp-18h]@7
  bool v34; // [sp+30h] [bp-14h]@21
  bool v35; // [sp+34h] [bp-10h]@23
  signed int v36; // [sp+38h] [bp-Ch]@17
  signed int v37; // [sp+38h] [bp-Ch]@21
  signed int v38; // [sp+38h] [bp-Ch]@42
  signed int v39; // [sp+3Ch] [bp-8h]@1
  signed int v40; // [sp+40h] [bp-4h]@1

  dword_720ED0[0] = -1;
  dword_720E80[0] = -1;
  v4 = a1;
  v5 = 0;
  v40 = a2;
  v31 = v4;
  v39 = 1;
  dword_720F20[0] = 10000;
  if ( (signed int)pOutdoor->uNumBModels > 0 )
  {
    v29 = 0;
    for ( v32 = 0; v32 < (signed int)pOutdoor->uNumBModels; ++v32 )
    {
      v6 = &pOutdoor->pBModels[v29];
      if ( v4 <= pOutdoor->pBModels[v29].sMaxX )
      {
        if ( v4 >= v6->sMinX )
        {
          if ( v40 <= v6->sMaxY )
          {
            if ( v40 >= v6->sMinY )
            {
              v33 = v5;
              v26 = v6->uNumFaces;
              if ( (signed int)v6->uNumFaces > (signed int)v5 )
              {
                v30 = v5;
                while ( 1 )
                {
                  v7 = (ODMFace *)((char *)v6->pFaces + v30);
                  v8 = v7->uPolygonType;
                  if ( (v8 == 5 || v8 == 6)
                    && !(BYTE3(v7->uAttributes) & 0x20)
                    && v4 <= v7->pBoundingBox.x2
                    && v4 >= v7->pBoundingBox.x1
                    && v40 <= v7->pBoundingBox.y2
                    && v40 >= v7->pBoundingBox.y1 )
                  {
                    v36 = v5;
                    if ( v7->uNumVertices )
                    {
                      v9 = v7->pXInterceptDisplacements;
                      do
                      {
                        v10 = 2 * v36;
                        word_720DB0_xs[2 * v36] = *v9 + LOWORD(v6->pVertices.pVertices[*(v9 - 60)].x);
                        word_720CE0_ys[2 * v36] = v9[20] + LOWORD(v6->pVertices.pVertices[*(v9 - 60)].y);
                        word_720DB0_xs[2 * v36++ + 1] = *v9 + LOWORD(v6->pVertices.pVertices[*(v9 - 59)].x);
                        word_720CE0_ys[v10 + 1] = v9[20] + LOWORD(v6->pVertices.pVertices[*(v9 - 59)].y);
                        ++v9;
                      }
                      while ( v36 < v7->uNumVertices );
                      v4 = v31;
                    }
                    v27 = 2 * v7->uNumVertices;
                    word_720DB0_xs[2 * v7->uNumVertices] = word_720DB0_xs[0];
                    word_720CE0_ys[v27] = word_720CE0_ys[0];
                    v11 = 0;
                    v34 = word_720CE0_ys[0] >= v40;
                    v37 = 0;
                    v28 = 0;
                    if ( v27 > 0 )
                    {
                      do
                      {
                        if ( v37 >= 2 )
                          break;
                        v12 = v11;
                        v4 = v31;
                        v35 = word_720CE0_ys[v12 + 1] >= v40;
                        if ( v34 != v35 )
                        {
                          v13 = word_720DB0_xs[v12 + 1] >= v31 ? 0 : 2;
                          v14 = v13 | (word_720DB0_xs[v12] < v31);
                          if ( v14 != 3 )
                          {
                            if ( !v14
                              || (v15 = word_720CE0_ys[v12],
                                  v16 = word_720CE0_ys[v12 + 1] - v15,
                                  v17 = v40 - v15,
                                  LODWORD(v18) = v17 << 16,
                                  HIDWORD(v18) = v17 >> 16,
                                  (signed int)(((unsigned __int64)(((signed int)word_720DB0_xs[v12 + 1]
                                                                  - (signed int)word_720DB0_xs[v12])
                                                                 * v18
                                                                 / v16) >> 16)
                                             + word_720DB0_xs[v12]) >= v31) )
                              ++v37;
                          }
                        }
                        v11 = v28 + 1;
                        v34 = v35;
                        ++v28;
                      }
                      while ( v28 < v27 );
                      if ( v37 == 1 )
                      {
                        if ( v39 >= 20 )
                          break;
                        if ( v7->uPolygonType == 5 )
                          v19 = v6->pVertices.pVertices[v7->pVertexIDs[0]].z;
                        else
                          v19 = ((unsigned __int64)(v7->zCalc1 * (signed __int64)v4) >> 16)
                              + ((unsigned __int64)(v7->zCalc2 * (signed __int64)v40) >> 16)
                              + HIWORD(v7->zCalc3);
                        v20 = v39++;
                        dword_720F20[v20] = v19;
                        dword_720ED0[v20] = v32;
                        dword_720E80[v20] = v33;
                      }
                    }
                  }
                  ++v33;
                  v30 += 308;
                  if ( v33 >= v26 )
                    break;
                  v5 = 0;
                }
              }
            }
          }
        }
      }
      //++v32;
      ++v29;
      v5 = 0;
    }
    //while ( v32 < (signed int)pOutdoor->uNumBModels );
    if ( !v39 )
    {
      *(int *)a4 = 0;
      return dword_720F20[0];
    }
  }
  v22 = 0;
  v38 = v5;
  if ( v39 > (signed int)v5 )
  {
	  v23 = 0;
	  do
	  {
		v24 = dword_720F20[v5 / 4];
		v25 = *(int *)((char *)dword_720F20.data() + v23);
		if ( v24 == v25 )
		{
		  v22 = v38;
		  v23 = v5;
		}
		else if ( v25 > a3 + 15 )
		{
		  if ( v24 < v25 )
		  {
			  v22 = v38;
			  v23 = v5;
		  }
		}
		else if ( v24 > v25 && v24 <= a3 + 15 )
		{
		  v22 = v38;
		  v23 = v5;
		}
		++v38;
		v5 += 4;
	  }
	  while ( v38 < v39 );
	  if ( v22 )
	  {
		*(int *)a4 = dword_720E80[v22] | (dword_720ED0[v22] << 6);
		return dword_720F20[v22];
	  }
  }
  *(int *)a4 = 0;
  return dword_720F20[v22];
}

//----- (0046DEF2) --------------------------------------------------------
unsigned int __fastcall sub_46DEF2(signed int a2, unsigned int uLayingItemID)
{
  unsigned int result; // eax@1

  result = uLayingItemID;
  if ( pObjectList->pObjects[pSpriteObjects[uLayingItemID].uObjectDescID].uFlags & 0x10 )
    result = _46BFFA_check_object_intercept(uLayingItemID, a2);
  return result;
}

//----- (0046E0B2) --------------------------------------------------------
void  _46E0B2_collide_against_decorations()
{
  BLVSector *v0; // ebp@1
  LevelDecoration *v1; // edi@2
  DecorationDesc *v2; // esi@3
  int v3; // edx@4
  int v4; // eax@4
  int v5; // ecx@6
  int v6; // ebx@8
  int v7; // esi@8
  int v8; // ebx@10
  int v9; // esi@11
  int v10; // edi@12
  int v11; // eax@12
  int v12; // esi@14
  unsigned int v13; // eax@17
  signed int i; // [sp+4h] [bp-14h]@1
  int v15; // [sp+8h] [bp-10h]@10
  int v16; // [sp+Ch] [bp-Ch]@10
  int v17; // [sp+10h] [bp-8h]@10
  int v18; // [sp+14h] [bp-4h]@8

  v0 = &pIndoor->pSectors[stru_721530.uSectorID];
  for ( i = 0; i < v0->uNumDecorations; ++i )
  {
    v1 = &pLevelDecorations[v0->pDecorationIDs[i]];
    if (!(v1->uFlags & LEVEL_DECORATION_INVISIBLE))
    {
      v2 = &pDecorationList->pDecorations[v1->uDecorationDescID];
      if (!v2->CanMoveThrough())
      {
        v3 = v2->uRadius;
        v4 = v1->vPosition.x;
        if ( stru_721530.sMaxX <= v4 + v3 )
        {
          if ( stru_721530.sMinX >= v4 - v3 )
          {
            v5 = v1->vPosition.y;
            if ( stru_721530.sMaxY <= v5 + v3 )
            {
              if ( stru_721530.sMinY >= v5 - v3 )
              {
                v6 = v2->uDecorationHeight;
                v7 = v1->vPosition.z;
                v18 = v6;
                if ( stru_721530.sMaxZ <= v7 + v6 )
                {
                  if ( stru_721530.sMinZ >= v7 )
                  {
                    v16 = v4 - stru_721530.normal.x;
                    v15 = v5 - stru_721530.normal.y;
                    v8 = stru_721530.prolly_normal_d + v3;
                    v17 = ((v4 - stru_721530.normal.x) * stru_721530.direction.y
                         - (v5 - stru_721530.normal.y) * stru_721530.direction.x) >> 16;
                    if ( abs(v17) <= stru_721530.prolly_normal_d + v3 )
                    {
                      v9 = (v16 * stru_721530.direction.x + v15 * stru_721530.direction.y) >> 16;
                      if ( v9 > 0 )
                      {
                        v10 = v1->vPosition.z;
                        v11 = stru_721530.normal.z + fixpoint_mul(stru_721530.direction.z, v9);
                        if ( v11 >= v10 )
                        {
                          if ( v11 <= v18 + v10 )
                          {
                            v12 = v9 - integer_sqrt(v8 * v8 - v17 * v17);
                            if ( v12 < 0 )
                              v12 = 0;
                            if ( v12 < stru_721530.field_7C )
                            {
                              stru_721530.field_7C = v12;
                              v13 = 8 * v0->pDecorationIDs[i];
                              LOBYTE(v13) = v13 | 5;
                              stru_721530.uFaceID = v13;
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

//----- (00487DA9) --------------------------------------------------------
void sub_487DA9()
{
  for (int i = 0; i < 20000; ++i)
    array_77EC08[i].field_108 = 0;
}

//----- (0048A959) --------------------------------------------------------
unsigned int ReplaceHSV(unsigned int uColor, float h_replace, float s_replace, float v_replace)
{
  float r = ((uColor & 0x00FF0000) >> 16) / 255.0f,
        g = ((uColor & 0x0000FF00) >> 8) / 255.0f,
        b = (uColor & 0x000000FF) / 255.0f;

  float h, s, v;
  RGB2HSV(&h, &s, r, g, b, &v);

  if ( h_replace != -1.0 )
    h = h_replace;
  if ( s_replace != -1.0 )
    s = s_replace;
  if ( v_replace != -1.0 )
    v = v_replace;
  HSV2RGB(&r, &g, &b, h, s, v);

  return (((uint)round(r * 255.0f) & 0xFF) << 16) |
         (((uint)round(g * 255.0f) & 0xFF) << 8) |
         (((uint)round(b * 255.0f) & 0xFF));
}

//----- (0048B561) --------------------------------------------------------
int fixpoint_from_float(float val)
{
  //  float X.Yf -> int XXXX YYYY
  int left = floorf((val - 0.5f) + 0.5f);
  int right = floorf((val - left) * 65536.0f);
  return (left << 16) | right;
}

int fixpoint_from_int(int lhv, int rhv)
{
  return (lhv << 16) | rhv;
}

//----- (00491E3A) --------------------------------------------------------
void sub_491E3A()
{
  Player *v0; // ebx@1
  signed int v1; // esi@3
  char *v2; // eax@4
  unsigned int v3; // eax@7
  unsigned int v4; // edx@8
  char *v5; // ecx@9
  int v6; // edi@17
  Texture *v7; // ebx@18
  struct IDirect3DTexture2 **v8; // eax@19
  struct IDirect3DTexture2 *v9; // eax@20
  struct IDirectDrawSurface **v10; // eax@22
  struct IDirectDrawSurface *v11; // eax@23
  int v12; // eax@26

  v0 = pParty->pPlayers.data();
  do
  {
    if (SoundSetAction[24][0])
    {
      v1 = 0;
      if ( (signed int)pSoundList->sNumSounds <= 0 )
      {
LABEL_7:
        v3 = 0;
      }
      else
      {
        v2 = (char *)&pSoundList->pSounds->uSoundID;
        while ( *(int *)v2 != 2 * (SoundSetAction[24][0] + 50 * v0->uVoiceID) + 4998 )
        {
          ++v1;
          v2 += 120;
          if ( v1 >= (signed int)pSoundList->sNumSounds )
            goto LABEL_7;
        }
        v3 = v1;
      }
      pSoundList->UnloadSound(v3, 1);
      v4 = 0;
      if ( (signed int)pSoundList->sNumSounds <= 0 )
      {
LABEL_12:
        v4 = 0;
      }
      else
      {
        v5 = (char *)&pSoundList->pSounds->uSoundID;
        while ( *(int *)v5 != 2 * (SoundSetAction[24][0] + 50 * v0->uVoiceID) + 4999 )
        {
          ++v4;
          v5 += 120;
          if ( (signed int)v4 >= (signed int)pSoundList->sNumSounds )
            goto LABEL_12;
        }
      }
      pSoundList->UnloadSound(v4, 1);
    }
    ++v0;
  }
  while ( (signed int)v0 < (signed int)pParty->pHirelings.data() );
  v6 = pIcons_LOD->uNumLoadedFiles - 1;
  if ( v6 >= pIcons_LOD->pFacesLock )
  {
    v7 = &pIcons_LOD->pTextures[v6];
    do
    {
      v7->Release();
      v8 = pIcons_LOD->pHardwareTextures;
      if ( v8 )
      {
        v9 = v8[v6];
        if ( v9 )
        {
          v9->Release();
          pIcons_LOD->pHardwareTextures[v6] = 0;
        }
      }
      v10 = pIcons_LOD->pHardwareSurfaces;
      if ( v10 )
      {
        v11 = v10[v6];
        if ( v11 )
        {
          v11->Release();
          pIcons_LOD->pHardwareSurfaces[v6] = 0;
        }
      }
      --v6;
      --v7;
    }
    while ( v6 >= pIcons_LOD->pFacesLock );
  }
  v12 = pIcons_LOD->pFacesLock;
  pIcons_LOD->pFacesLock = 0;
  pIcons_LOD->uNumLoadedFiles = v12;
}
// 4ED498: using guessed type char byte_4ED498;

//----- (00493938) --------------------------------------------------------
int  _493938_regenerate()
{
  int v0; // edi@1
  signed __int64 v1; // qax@1
  int v2; // ecx@1
  int result; // eax@1
  int v4; // eax@2
  int v5; // edi@5
  int *v6; // ecx@5
  char v7; // sf@5
  int *v8; // ecx@10
  int v9; // edi@15
  signed int v10; // eax@15
  __int16 *v11; // edx@16
  int v12; // eax@20
  int v13; // ebx@20
  unsigned int *v14; // esi@21
  unsigned int v15; // ecx@21
  unsigned int v16; // eax@21
  int v17; // edx@21
  int v18; // eax@21
  signed int v19; // eax@21
  signed int v20; // ebx@25
  Player *v21; // esi@25
  ITEM_EQUIP_TYPE v22; // edi@30
  //int v23; // edx@31
  signed int v24; // ecx@32
  signed int v25; // eax@33
  int v26; // eax@35
  int v27; // eax@36
  int v28; // eax@37
  int v29; // eax@40
  int v30; // eax@41
  signed int v31; // ecx@53
  char *v32; // eax@53
  char *v33; // edi@82
  int v34; // ecx@88
  int v35; // eax@88
  char *v36; // edi@99
  int v37; // edi@104
  int v38; // edi@106
  int v39; // edi@111
  int v40; // ecx@113
  char v41[400]; // [sp+4h] [bp-22Ch]@20
  SpriteObject a1; // [sp+194h] [bp-9Ch]@15
  Vec3_int_ a3; // [sp+204h] [bp-2Ch]@15
  int v44; // [sp+210h] [bp-20h]@22
  int v45; // [sp+214h] [bp-1Ch]@25
  int v46; // [sp+218h] [bp-18h]@25
  int v47; // [sp+21Ch] [bp-14h]@25
  int v48; // [sp+220h] [bp-10h]@25
  int v49; // [sp+224h] [bp-Ch]@24
  int v50; // [sp+228h] [bp-8h]@25
  int v51; // [sp+22Ch] [bp-4h]@2

  v0 = (signed int)(signed __int64)((double)(signed __int64)pParty->uTimePlayed * 0.234375) / 60;
  v1 = (signed __int64)((double)pParty->uLastRegenerationTime * 0.234375);
  v2 = (signed int)v1 / 60;
  result = (signed int)v1 / 60 + 5;
  if ( v0 >= result )
  {
    v51 = 0;
    v4 = (v0 - v2) / 5;
    if (pParty->FlyActive())
    {
      if ( pParty->bFlying )
      {
        if ( !(pParty->pPartyBuffs[PARTY_BUFF_FLY].uFlags & 1) )
        { // colliding with something in the air - fall down
          v5 = v4 * pParty->pPartyBuffs[PARTY_BUFF_FLY].uPower;
          //__debugbreak();
		  v6 = (int *)&pParty->pPlayers[pParty->pPartyBuffs[PARTY_BUFF_FLY].uCaster].pConditions[0];//&stru_AA1058[4].pSounds[6972 * pParty->pPartyBuffs[PARTY_BUFF_FLY].uCaster + 2000];
          v7 = *v6 < v5;
          *v6 -= v5;
          if ( v7 )
          {
            *v6 = 0;
            pParty->uFlags &= 0xFFFFFFBFu;
            pParty->bFlying = 0;
            v51 = 1;
          }
        }
      }
    }

    if (pParty->WaterWalkActive())
    {
      if (pParty->uFlags & PARTY_FLAGS_1_STANDING_ON_WATER )
      {
        if ( !(pParty->pPartyBuffs[PARTY_BUFF_WATER_WALK].uFlags & 1) )
        { // taking on water
          //__debugbreak();
          v8 = (int *)&pParty->pPlayers[pParty->pPartyBuffs[PARTY_BUFF_WATER_WALK].uCaster].pConditions[0];//&stru_AA1058[4].pSounds[6972 * pParty->pPartyBuffs[PARTY_BUFF_WATER_WALK].uCaster + 2000];
          v7 = *v8 < v4;
          *v8 -= v4;
          if ( v7 )
          {
            *v8 = 0;
            pParty->uFlags &= ~PARTY_FLAGS_1_STANDING_ON_WATER;
            v51 = 1;
          }
        }
      }
    }

    if (pParty->ImmolationActive())
    {
      //SpriteObject::SpriteObject(&a1);
      v9 = 0;
      a3.z = 0;
      a3.y = 0;
      a3.x = 0;
      a1.stru_24.Reset();
      a1.spell_level = pParty->pPartyBuffs[PARTY_BUFF_IMMOLATION].uPower;
      a1.spell_skill = pParty->ImmolationSkillLevel();
      v10 = 0;
      a1.uType = 1070;
      a1.spell_id = SPELL_FIRE_IMMOLATION;
      if ( (signed int)pObjectList->uNumObjects <= 0 )
      {
LABEL_19:
        LOWORD(v10) = 0;
      }
      else
      {
        v11 = &pObjectList->pObjects->uObjectID;
        while ( stru_4E3ACC[8].uType != *v11 )
        {
          ++v10;
          v11 += 28;
          if ( v10 >= (signed int)pObjectList->uNumObjects )
            goto LABEL_19;
        }
      }
      a1.uObjectDescID = v10;
      a1.field_60_distance_related_prolly_lod = 0;
      v12 = 8 * pParty->pPartyBuffs[PARTY_BUFF_IMMOLATION].uCaster;
      LOBYTE(v12) = v12 | OBJECT_Player;
      a1.uAttributes = 0;
      a1.uSectorID = 0;
      a1.uSpriteFrameID = 0;
      a1.spell_caster_pid = v12;
      a1.uFacing = 0;
      a1.uSoundID = 0;
      v13 = _46A89E_immolation_effect((int)v41, 100, 307);
      if ( v13 > 0 )
      {
        do
        {
          v14 = (unsigned int *)&v41[4 * v9];
          v15 = *v14;
          v16 = *v14;
          a1.vPosition.x = pActors[v16].vPosition.x;
          v17 = pActors[v16].vPosition.y;
          a1.vPosition.z = pActors[v16].vPosition.z;
          v18 = 8 * v15;
          LOBYTE(v18) = PID(OBJECT_Actor,v15);
          a1.vPosition.y = v17;
          a1.spell_target_pid = v18;
          v19 = a1.Create(0, 0, 0, 0);
          DamageMonsterFromParty(PID(OBJECT_Item,v19), *v14, &a3);
          ++v9;
        }
        while ( v9 < v13 );
      }
    }
    v44 = 0;
    if (PartyHasDragon())
      v44 = 1;
    v49 = 0;
LABEL_25:
    v20 = 0;
    v21 = &pParty->pPlayers[v49];
    v50 = 0;
    v47 = 0;
    v45 = 0;
    v48 = 0;
    v46 = 0;
    if ( v44 && v21->classType == PLAYER_CLASS_WARLOCK )
      v50 = 1;
    if ( v21->classType == PLAYER_CLASS_LICH )
      v48 = 1;
    v22 = (ITEM_EQUIP_TYPE)0;
    while ( 1 )
    {
      if ( v21->HasItemEquipped(v22) )
      {
        //v23 = v21->pEquipment.pIndices;
        uint _idx = v21->pEquipment.pIndices[v22];
        v24 = v21->pInventoryItemList[_idx].uItemID;
        if ( v24 > 134 )
        {
          if ( v24 == 529 )
            goto LABEL_43;
          if ( v24 == 535 )
            goto LABEL_44;
          if ( v24 == 515 )
          {
            v47 = 1;
          }
          else
          {
            if ( v24 == 532 )
              goto LABEL_50;
          }
        }
        else
        {
          v25 = v21->pInventoryItemList[_idx].uSpecEnchantmentType;
          if ( v25 <= 50 )
          {
            if ( v25 != 50 )
            {
              v26 = v25 - 37;
              if ( v26 )
              {
                v27 = v26 - 1;
                if ( !v27 )
                  goto LABEL_50;
                v28 = v27 - 6;
                if ( v28 )
                {
                  if ( v28 != 3 )
                    goto LABEL_51;
LABEL_50:
                  v50 = 1;
                  goto LABEL_51;
                }
              }
            }
LABEL_44:
            v20 = 1;
            goto LABEL_51;
          }
          v29 = v25 - 54;
          if ( !v29 )
            goto LABEL_44;
          v30 = v29 - 1;
          if ( !v30 )
            goto LABEL_50;
          if ( v30 == 11 )
          {
LABEL_43:
            v20 = 1;
            v50 = 1;
            goto LABEL_51;
          }
        }
      }
LABEL_51:
      v22 = (ITEM_EQUIP_TYPE)((int)v22 + 1);
      if ( (signed int)v22 >= 16 )
      {
        if ( v48 )
        {
          v31 = 0;
          v32 = (char *)v21->pInventoryItemList.data();
          while ( *(int *)v32 != 601 || (unsigned __int8)v32[26] != v49 + 1 )
          {
            ++v31;
            v32 += 36;
            if ( v31 >= 138 )
              goto LABEL_59;
          }
          v48 = 0;
          v45 = 1;
        }
LABEL_59:
        if ( v21->pConditions[17] )
          v46 = 1;
        if ( v20 && !v21->pConditions[14] && !v21->pConditions[16] )
        {
          ++v21->sHealth;
          if ( v21->sHealth > v21->GetMaxHealth() )
            v21->sHealth = v21->GetMaxHealth();
          if ( v21->pConditions[13] && v21->sHealth > 0 )
          {
            LODWORD(v21->pConditions[13]) = 0;
            HIDWORD(v21->pConditions[13]) = 0;
          }
          v51 = 1;
        }
        if ( SHIDWORD(v21->pPlayerBuffs[12].uExpireTime) >= 0
          && (SHIDWORD(v21->pPlayerBuffs[12].uExpireTime) > 0 || LODWORD(v21->pPlayerBuffs[12].uExpireTime))
          && !v21->pConditions[14]
          && !v21->pConditions[16] )
        {
          v21->sHealth += 5 * v21->pPlayerBuffs[12].uPower;
          if ( v21->sHealth > v21->GetMaxHealth() )
            v21->sHealth = v21->GetMaxHealth();
          if ( v21->pConditions[13] && v21->sHealth > 0 )
          {
            LODWORD(v21->pConditions[13]) = 0;
            HIDWORD(v21->pConditions[13]) = 0;
          }
          v51 = 1;
        }
        if ( v50 )
        {
          v33 = (char *)&v21->sMana;
          ++*(int *)v33;
          if ( v21->sMana > v21->GetMaxMana() )
            *(int *)v33 = v21->GetMaxMana();
          v51 = 1;
        }
        if ( v47 && !v21->pConditions[14] && !v21->pConditions[16] )
        {
          v34 = LODWORD(v21->pConditions[13]);
          --v21->sHealth;
          v35 = v21->sHealth;
          if ( !(HIDWORD(v21->pConditions[13]) | v34) && v35 < 0 )
            v21->pConditions[13] = pParty->uTimePlayed;
          if ( v35 < 1 )
          {
            if ( v21->sHealth + v21->uEndurance + v21->GetItemsBonus(CHARACTER_ATTRIBUTE_ENDURANCE) >= 1
              || (signed __int64)v21->pPlayerBuffs[11].uExpireTime > 0 )
            {
              v21->pConditions[13] = pParty->uTimePlayed;
            }
            else
            {
              if ( !v21->pConditions[14] )
                v21->pConditions[14] = pParty->uTimePlayed;
            }
          }
          v51 = 1;
        }
        if ( v45 )
        {
          v36 = (char *)&v21->sMana;
          ++*(int *)v36;
          if ( v21->sMana > v21->GetMaxMana() )
            *(int *)v36 = v21->GetMaxMana();
        }
        if ( v48 && !v21->pConditions[14] && !v21->pConditions[16] )
        {
          v37 = v21->sHealth;
          if ( v37 > v21->GetMaxHealth() / 2 )
            v21->sHealth = v37 - 2;
          v38 = v21->sMana;
          if ( v38 > v21->GetMaxMana() / 2 )
            v21->sMana = v38 - 2;
        }
        if ( v46 && !v21->pConditions[14] && !v21->pConditions[16] )
        {
          v39 = v21->sHealth;
          if ( v39 > v21->GetMaxHealth() / 2 )
            v21->sHealth = v39 - 1;
          v40 = v21->sMana;
          if ( v40 > 0 )
            v21->sMana = v40 - 1;
        }
        ++v49;
        if ( v49 >= 4 )
        {
          result = HIDWORD(pParty->uTimePlayed);
          pParty->uLastRegenerationTime = pParty->uTimePlayed;
          if ( !viewparams->bRedrawGameUI )
          {
            result = v51;
            viewparams->bRedrawGameUI = v51;
          }
          return result;
        }
        goto LABEL_25;
      }
    }
  }
  return result;
}

//----- (00493F79) --------------------------------------------------------
void init_summoned_item(stru351_summoned_item *_this, __int64 duration)
{
  signed __int64 v2; // ST2C_8@1
  signed __int64 v3; // qax@1
  //signed __int64 v4; // ST1C_8@1
  unsigned __int64 v5; // qax@1
  unsigned int v6; // ebx@1

  v2 = (signed __int64)((double)duration * 0.234375);
  v3 = v2 / 60 / 60;
  //v4 = v3;
  v5 = (unsigned int)v3 / 0x18;
  v6 = (unsigned int)(v5 / 7) >> 2;
  _this->field_0_expire_second = v2 % 60;
  _this->field_4_expire_minute = v2 / 60 % 60;
  _this->field_8_expire_hour = v3 % 24;
  _this->field_10_expire_week = v5 / 7 & 3;
  _this->field_C_expire_day = (unsigned int)v5 % 0x1C;
  _this->field_14_exprie_month = v6 % 0xC;
  _this->field_18_expire_year = v6 / 0xC + game_starting_year;
}

//----- (00494035) --------------------------------------------------------
void _494035_timed_effects__water_walking_damage__etc()
{
  signed __int64 v0; // qax@1
  signed __int64 v1; // ST30_8@1
  signed __int64 v2; // ST38_8@1
  unsigned __int64 v3; // qax@1
  unsigned int v4; // edi@1
  //signed int v5; // eax@4
  //char *v6; // ecx@5
  //Player *v7; // esi@8
  //char *v8; // ecx@12
  //Player *pPlayer; // esi@15
  //void *v10; // esi@25
  unsigned int v11; // ecx@27
  signed int v12; // edi@29
  //Player *v13; // ecx@30
  //Player *v14; // esi@35
  //double v15; // st7@35
  Player **v16; // esi@43
  Player *v17; // edi@44
  double v18; // st7@44
  //float v19; // ST28_4@48
  //double v20; // ST38_8@48
  Player *v21; // esi@51
  signed int v22; // edi@53
  int v23; // eax@59
  int v24; // ecx@60
  int v25; // eax@63
  int v26; // ecx@64
  int v27; // eax@67
  int v28; // ecx@68
  int v29; // eax@71
  int v30; // ecx@72
  int v31; // eax@75
  int v32; // ecx@76
  int v33; // eax@79
  int v34; // ecx@80
  int v35; // eax@83
  int v36; // ecx@84
  int v37; // eax@87
  int v38; // ecx@88
  int v39; // eax@91
  int v40; // ecx@92
  int v41; // eax@95
  int v42; // ecx@96
  bool v43; // ebx@102
  //SpellBuff *v44; // edi@104
  //signed int v45; // ebp@104
  bool v46; // edi@111
  //SpellBuff *v47; // esi@113
  //Player **v48; // esi@119
  //signed int v49; // edi@121
  //char *v50; // esi@122
  signed int v51; // edx@128
  signed int v52; // ecx@130
  int v53; // eax@131
  Player *v54; // eax@141
  //unsigned int v55; // [sp-8h] [bp-38h]@18
  unsigned int v56; // [sp-8h] [bp-38h]@55
  //int v57; // [sp-4h] [bp-34h]@18
  //int v58; // [sp-4h] [bp-34h]@33
  int v59; // [sp-4h] [bp-34h]@55
  //unsigned int v60; // [sp+10h] [bp-20h]@1
  unsigned int v61; // [sp+14h] [bp-1Ch]@1
  Player **v62; // [sp+14h] [bp-1Ch]@50
  //unsigned int a2; // [sp+18h] [bp-18h]@1
  signed int a2a; // [sp+18h] [bp-18h]@47
  signed int v65; // [sp+1Ch] [bp-14h]@47

  //a2 = pParty->uCurrentHour;
  v61 = pParty->uDaysPlayed;
  //auto prev_time = pEventTimer->uTimeElapsed;
  pParty->uTimePlayed += pEventTimer->uTimeElapsed;
  v0 = (signed __int64)(pParty->uTimePlayed * 0.234375) / 60;
  v1 = v0;
  v0 /= 60i64;
  v2 = v0;
  v3 = (unsigned int)v0 / 24;
  v4 = (unsigned int)(v3 / 7) >> 2;
  pParty->uCurrentTimeSecond = (signed __int64)((double)(signed __int64)pParty->uTimePlayed * 0.234375) % 60;
  pParty->uCurrentMinute = v1 % 60;
  pParty->uCurrentMonthWeek = v3 / 7 & 3;
  pParty->uCurrentHour = v2 % 24;
  pParty->uDaysPlayed = (unsigned int)v3 % 28;
  pParty->uCurrentMonth = v4 % 12;
  pParty->uCurrentYear = v4 / 0xC + game_starting_year;
  if ( pParty->uCurrentHour >= 3 && (pParty->uCurrentHour < 3 || pParty->uDaysPlayed > v61) ) // new day dawns
  {
    pParty->pHirelings[0].bHasUsedTheAbility = false;
    pParty->pHirelings[1].bHasUsedTheAbility = false;

    for (uint i = 0; i < pNPCStats->uNumNewNPCs; ++i)
      pNPCStats->pNewNPCData[i].bHasUsedTheAbility = false;

    ++pParty->days_played_without_rest;
    if (pParty->days_played_without_rest > 1)
    {
      for (uint i = 0; i < 4; ++i)
        pParty->pPlayers[i].SetCondWeakWithBlockCheck(0);

      if (pParty->uNumFoodRations)
        Party::TakeFood(1);
      else
        for (uint i = 0; i < 4; ++i)
          pParty->pPlayers[i].sHealth = pParty->pPlayers[i].sHealth / (pParty->days_played_without_rest + 1) + 1;

      if (pParty->days_played_without_rest > 3)
        for (uint i = 0; i < 4; ++i)
        {
          Player* player = &pParty->pPlayers[i];

          player->Zero();

          if (!player->IsPertified() && !player->IsEradicated() && !player->IsDead())
          {
            if (rand() % 100 < 5 * pParty->days_played_without_rest)
              player->SetCondDeadWithBlockCheck(0);
            if (rand() % 100 < 10 * pParty->days_played_without_rest)
              player->SetCondInsaneWithBlockCheck(0);
          }
        }
    }
    if (uCurrentlyLoadedLevelType == LEVEL_Outdoor)
      pOutdoor->SetFog();

    for (uint i = 0; i < 4; ++i)
      pParty->pPlayers[i].uNumDivineInterventionCastsThisDay = 0;
  }

  v11 = LODWORD(pParty->uTimePlayed);
  if ( pParty->uFlags & 4 && pParty->field_6FC < (signed __int64)pParty->uTimePlayed )//water damage error
  {
    //v12 = 1;
    pParty->field_6FC = LODWORD(pParty->uTimePlayed) + 128;
    viewparams->bRedrawGameUI = 1;
    //while ( 1 )
    for ( v12 = 1; v12 <= 4; ++v12 )
    {
      if ( pPlayers[v12]->WearsItem(ITEM_RELIC_HARECS_LEATHER, EQUIP_ARMOUR)
        || pPlayers[v12]->HasEnchantedItemEquipped(71)
        || pPlayers[v12]->pPlayerBuffs[23].uExpireTime > 0 )
      {
        //v58 = 0;
        pPlayers[v12]->PlayEmotion(CHARACTER_EXPRESSION_37, 0);
      }
      else
      {
        //v58 = 0;
        if ( !pPlayers[v12]->HasUnderwaterSuitEquipped() )
        {
          //v14 = pPlayers[v12];
          //v15 = (double)pPlayers[v12]->GetMaxHealth() * 0.1;
          pPlayers[v12]->ReceiveDamage((signed __int64)pPlayers[v12]->GetMaxHealth() * 0.1, DMGT_FIRE);
          if ( pParty->uFlags & 4 )
          {
            strcpy(GameUI_Footer_TimedString.data(), pGlobalTXT_LocalizationStrings[660]);
            GameUI_Footer_TimeLeft = 128;
          }
        }
		else
			pPlayers[v12]->PlayEmotion(CHARACTER_EXPRESSION_37, 0);
      }
      //++v12;
      //if ( v12 > 4 )
      //{
        //v11 = LODWORD(pParty->uTimePlayed);
        //break;
      }
      v11 = LODWORD(pParty->uTimePlayed);
    //}
  }
  if ( pParty->uFlags & 0x200 && pParty->field_6FC < (signed __int64)__PAIR__(HIDWORD(pParty->uTimePlayed), v11) )
  {
    viewparams->bRedrawGameUI = 1;
    pParty->field_6FC = v11 + 128;
    v16 = &pPlayers[1];
    do
    {
      v17 = *v16;
      v18 = (double)(*v16)->GetMaxHealth() * 0.1;
      v17->ReceiveDamage((signed __int64)v18, DMGT_FIRE);
      if ( pParty->uFlags & 0x200 )
      {
        strcpy(GameUI_Footer_TimedString.data(), pGlobalTXT_LocalizationStrings[661]);
        GameUI_Footer_TimeLeft = 128;
      }
      ++v16;
    }
    while ( (signed int)v16 <= (signed int)&pPlayers[4] );
  }
  _493938_regenerate();
  v65 = 4;
  a2a = pEventTimer->uTimeElapsed;
  if ( pParty->uFlags2 & PARTY_FLAGS_2_RUNNING )
  {
    //v19 = (double)(signed int)prev_time * 0.5;
    //v20 = v19 + 6.7553994e15;
    a2a *= 0.5f;
    if (a2a < 1)
      a2a = 1;
  }
  v62 = &pPlayers[1];
  do
  {
    v21 = *v62;
    if ( (*v62)->uTimeToRecovery )
      v21->Recover(a2a);
    v22 = v21->sHealth;
    if ( v21->GetItemsBonus(CHARACTER_ATTRIBUTE_ENDURANCE) + v22 + v21->uEndurance >= 1
      || (signed __int64)v21->pPlayerBuffs[11].uExpireTime > 0 )
    {
      if ( v22 < 1 )
	  {
		  v59 = 0;
		  v56 = 13;
		  v21->SetCondition(v56, v59);
	  }
    }
    else
    {
      v59 = 0;
      v56 = 14;
	  v21->SetCondition(v56, v59);
    }
    v23 = (int)&v21->field_E0;
    if ( v21->field_E0 )
    {
      v24 = *(int *)v23 - pEventTimer->uTimeElapsed;
      if ( v24 > 0 )
      {
        *(int *)v23 = v24;
      }
      else
      {
        *(int *)v23 = 0;
        viewparams->bRedrawGameUI = 1;
      }
    }
    v25 = (int)&v21->field_E4;
    if ( v21->field_E4 )
    {
      v26 = *(int *)v25 - pEventTimer->uTimeElapsed;
      if ( v26 > 0 )
      {
        *(int *)v25 = v26;
      }
      else
      {
        *(int *)v25 = 0;
        viewparams->bRedrawGameUI = 1;
      }
    }
    v27 = (int)&v21->field_E8;
    if ( v21->field_E8 )
    {
      v28 = *(int *)v27 - pEventTimer->uTimeElapsed;
      if ( v28 > 0 )
      {
        *(int *)v27 = v28;
      }
      else
      {
        *(int *)v27 = 0;
        viewparams->bRedrawGameUI = 1;
      }
    }
    v29 = (int)&v21->field_EC;
    if ( v21->field_EC )
    {
      v30 = *(int *)v29 - pEventTimer->uTimeElapsed;
      if ( v30 > 0 )
      {
        *(int *)v29 = v30;
      }
      else
      {
        *(int *)v29 = 0;
        viewparams->bRedrawGameUI = 1;
      }
    }
    v31 = (int)&v21->field_F0;
    if ( v21->field_F0 )
    {
      v32 = *(int *)v31 - pEventTimer->uTimeElapsed;
      if ( v32 > 0 )
      {
        *(int *)v31 = v32;
      }
      else
      {
        *(int *)v31 = 0;
        viewparams->bRedrawGameUI = 1;
      }
    }
    v33 = (int)&v21->field_F4;
    if ( v21->field_F4 )
    {
      v34 = *(int *)v33 - pEventTimer->uTimeElapsed;
      if ( v34 > 0 )
      {
        *(int *)v33 = v34;
      }
      else
      {
        *(int *)v33 = 0;
        viewparams->bRedrawGameUI = 1;
      }
    }
    v35 = (int)&v21->field_F8;
    if ( v21->field_F8 )
    {
      v36 = *(int *)v35 - pEventTimer->uTimeElapsed;
      if ( v36 > 0 )
      {
        *(int *)v35 = v36;
      }
      else
      {
        *(int *)v35 = 0;
        viewparams->bRedrawGameUI = 1;
      }
    }
    v37 = (int)&v21->field_FC;
    if ( v21->field_FC )
    {
      v38 = *(int *)v37 - pEventTimer->uTimeElapsed;
      if ( v38 > 0 )
      {
        *(int *)v37 = v38;
      }
      else
      {
        *(int *)v37 = 0;
        viewparams->bRedrawGameUI = 1;
      }
    }
    v39 = (int)&v21->field_100;
    if ( v21->field_100 )
    {
      v40 = *(int *)v39 - pEventTimer->uTimeElapsed;
      if ( v40 > 0 )
      {
        *(int *)v39 = v40;
      }
      else
      {
        *(int *)v39 = 0;
        viewparams->bRedrawGameUI = 1;
      }
    }
    v41 = (int)&v21->field_104;
    if ( v21->field_104 )
    {
      v42 = *(int *)v41 - pEventTimer->uTimeElapsed;
      if ( v42 > 0 )
      {
        *(int *)v41 = v42;
      }
      else
      {
        *(int *)v41 = 0;
        viewparams->bRedrawGameUI = 1;
      }
    }
    if ( v21->pConditions[2] | v21->pConditions[12] | v21->pConditions[13] | v21->pConditions[14] | v21->pConditions[15] | v21->pConditions[16] )
      --v65;
    v43 = (signed __int64)v21->pPlayerBuffs[7].uExpireTime > 0;

    for (uint k = 0; k < 24; ++k)
      v21->pPlayerBuffs[k].IsBuffExpiredToTime(pParty->uTimePlayed);

    if ( v43 && (signed __int64)v21->pPlayerBuffs[7].uExpireTime <= 0 )
      v21->SetCondition(1u, 0);
    ++v62;
  }
  while ( (signed int)v62 <= (signed int)&pPlayers[4] );
  v46 = (signed __int64)pParty->pPartyBuffs[PARTY_BUFF_HASTE].uExpireTime > 0;

  for (uint i = 0; i < 20; ++i)
  {
    if (pParty->pPartyBuffs[i].IsBuffExpiredToTime(pParty->uTimePlayed) == 1)
      viewparams->bRedrawGameUI = 1;
  }

  if ( v46 && (signed __int64)pParty->pPartyBuffs[PARTY_BUFF_HASTE].uExpireTime <= 0 )
  {
    for (uint i = 0; i < 4; ++i)
      pParty->pPlayers[i].SetCondition(1, 0);
  }

  for (uint i = 0; i < 2; ++i)
  {
    SpellBuff* pBuf = &pParty->pPartyBuffs[dword_4EE07C[i]];
    if (pBuf->uExpireTime == 0)
      continue;

    if ( !(pBuf->uFlags & 1) )
    {
      if (!pPlayers[pBuf->uCaster]->CanAct())
      {
        pBuf->Reset();
        if (dword_4EE07C[i] == 7 )
          pParty->bFlying = false;
      }
    }
  }

  v51 = v65;
  if ( v65 )
    goto LABEL_135;
  if ( pCurrentScreen != SCREEN_REST )
  {
    v52 = (signed int)&pPlayers[1];
    while ( 1 )
    {
      v53 = *(int *)v52;
      if ( *(_QWORD *)(*(int *)v52 + 16) )
        break;
      v52 += 4;
      if ( v52 > (signed int)&pPlayers[4] )
        goto LABEL_135;
    }
    *(int *)(v53 + 16) = 0;
    *(int *)(v53 + 20) = 0;
    v51 = 1;
LABEL_135:
    if ( pCurrentScreen != SCREEN_REST
      && (!v51 || dword_5C35C0) )
      uGameState = GAME_STATE_PARTY_DIED;
  }
  if ( uActiveCharacter )
  {
    if ( pCurrentScreen != SCREEN_REST )
    {
      v54 = pPlayers[uActiveCharacter];
      if ( v54->pConditions[2]
        || v54->pConditions[12]
        || v54->pConditions[13]
        || v54->pConditions[14]
        || v54->pConditions[15]
        || v54->pConditions[16] )
      {
        viewparams->bRedrawGameUI = 1;
        uActiveCharacter = pParty->GetNextActiveCharacter();
      }
    }
  }
}

//----- (00494820) --------------------------------------------------------
unsigned int __fastcall _494820_training_time(unsigned int a1)
{
  signed int v1; // eax@1

  v1 = 5;
  if ( a1 % 24 >= 5 )
    v1 = 29;
  return v1 - a1 % 24;
}

//----- (00494836) --------------------------------------------------------
int stru339_spell_sound::_494836(int uSoundID, int a6)
{
  int v3; // esi@1
  int result; // eax@1
  stru339_spell_sound *v5; // ebx@1
  int *v6; // edi@2
  unsigned int v7; // eax@3
  int v8; // [sp+Ch] [bp-8h]@3
  int v9; // [sp+10h] [bp-4h]@2
  int a2a; // [sp+1Ch] [bp+8h]@1
  return 0;
  v3 = 0;
  result = word_4EE088_sound_ids[uSoundID];
  v5 = this;
  a2a = word_4EE088_sound_ids[uSoundID];
  if ( result )
  {
    v9 = 0;
    v6 = this->pSoundsOffsets;
    do
    {
      v7 = a2a++;
      result = pSoundList->LoadSound(v7, (char *)v5 + v3, 44744 - v3, &v8, a6);
      if ( !result )
        break;
      a6 += 4;
      result = v8 + 256;
      *v6 = v3;
      v3 += result;
      ++v9;
      *(v6 - 2) = result;
      ++v6;
    }
    while ( v9 < 2 );
  }
  return result;
}
// 4EE088: using guessed type __int16 word_4EE088_sound_ids[];

//----- (00494AED) --------------------------------------------------------
unsigned int PlayerFrameTable::GetFrameIdByExpression(CHARACTER_EXPRESSION_ID expression)
{
  unsigned int _uNumFrames; // edx@1
  unsigned int result; // eax@1
  PlayerFrame *v4; // ecx@2

  _uNumFrames = this->uNumFrames;
  result = 0;
  if ( (signed int)this->uNumFrames <= 0 )
  {
    result = 0;
  }
  else
  {
    v4 = this->pFrames;
    while ( v4->expression != expression )
    {
      ++result;
      ++v4;
      if ( (signed int)result >= (signed int)_uNumFrames )
        return 0;
    }
  }
  return result;
}

//----- (00494B10) --------------------------------------------------------
PlayerFrame *PlayerFrameTable::GetFrameBy_x(unsigned int uFramesetID, unsigned int uFrameID)
{
  unsigned int v3; // esi@1
  PlayerFrame *v4; // edi@1
  PlayerFrame *v5; // ecx@1
  __int16 v6; // dx@2
  int v7; // edx@3
  char *i; // eax@3
  int v9; // ecx@5
  PlayerFrame *result; // eax@6

  v3 = uFramesetID;
  v4 = this->pFrames;
  v5 = &v4[uFramesetID];
  if ( v5->uFlags & 1 && (v6 = v5->uAnimLength) != 0 )
  {
    v7 = ((signed int)uFrameID >> 3) % (unsigned __int16)v6;
    for ( i = (char *)&v5->uAnimTime; ; i += 10 )
    {
      v9 = *(short *)i;
      if ( v7 <= v9 )
        break;
      v7 -= v9;
      ++v3;
    }
    result = &v4[v3];
  }
  else
  {
    result = &v4[uFramesetID];
  }
  return result;
}

//----- (00494B5E) --------------------------------------------------------
PlayerFrame *PlayerFrameTable::GetFrameBy_y(int *pFramesetID, int *pAnimTime, int a4)
{
  PlayerFrameTable *v4; // edi@1
  int v5; // esi@1
  int v6; // eax@2

  v4 = this;
  v5 = a4 + *pAnimTime;
  if ( v5 < 8 * this->pFrames[*pFramesetID].uAnimTime )
  {
    *pAnimTime = v5;
  }
  else
  {
    v6 = rand() % 4 + 21;
    *pFramesetID = v6;
    *pAnimTime = 8 * v5 % v4->pFrames[v6].uAnimTime;
  }
  return &v4->pFrames[*pFramesetID];
}

//----- (00494BC3) --------------------------------------------------------
void PlayerFrameTable::ToFile()
{
  PlayerFrameTable *v1; // esi@1
  FILE *v2; // eax@1
  FILE *v3; // edi@1

  PlayerFrameTable* Str = this;

  v1 = Str;
  v2 = fopen("data\\dpft.bin", "wb");
  v3 = v2;
  if ( !v2 )
    Error("Unable to save dpft.bin");
  fwrite(v1, 4u, 1u, v2);
  fwrite(v1->pFrames, 0xAu, v1->uNumFrames, v3);
  fclose(v3);
}

//----- (00494C0F) --------------------------------------------------------
void PlayerFrameTable::FromFile(void *data_mm6, void *data_mm7, void *data_mm8)
{
  uint num_mm6_frames = data_mm6 ? *(int *)data_mm6 : 0,
       num_mm7_frames = data_mm7 ? *(int *)data_mm7 : 0,
       num_mm8_frames = data_mm8 ? *(int *)data_mm8 : 0;
  uNumFrames = num_mm6_frames + num_mm7_frames + num_mm8_frames;
  assert(uNumFrames);
  assert(!num_mm8_frames);

  pFrames = (PlayerFrame *)malloc(uNumFrames * sizeof(PlayerFrame));
  memcpy(pFrames,                                   (char *)data_mm7 + 4, num_mm7_frames * sizeof(PlayerFrame));
  memcpy(pFrames + num_mm7_frames,                  (char *)data_mm6 + 4, num_mm6_frames * sizeof(PlayerFrame));
  memcpy(pFrames + num_mm6_frames + num_mm7_frames, (char *)data_mm8 + 4, num_mm8_frames * sizeof(PlayerFrame));
}

//----- (00494C5A) --------------------------------------------------------
int PlayerFrameTable::FromFileTxt(const char *Args)
{
  PlayerFrameTable *v2; // ebx@1
  FILE *v3; // eax@1
  int v4; // esi@3
  void *v5; // eax@10
  FILE *v6; // ST0C_4@12
  char *i; // eax@12
  __int16 v8; // ax@15
  const char *v9; // ST10_4@15
  unsigned __int16 v10; // ax@15
  const char *v11; // ST0C_4@15
  int j; // esi@15
  int v13; // eax@17
  int v14; // edx@22
  int v15; // ecx@23
  int v16; // eax@24
  signed int k; // eax@27
  PlayerFrame *v18; // edx@28
  int v19; // esi@28
  int l; // ecx@29
  char Buf; // [sp+Ch] [bp-2F8h]@3
  FrameTableTxtLine v23; // [sp+200h] [bp-104h]@4
  FrameTableTxtLine v24; // [sp+27Ch] [bp-88h]@4
  int v25; // [sp+2F8h] [bp-Ch]@3
  int v26; // [sp+2FCh] [bp-8h]@3
  FILE *File; // [sp+300h] [bp-4h]@1
  int Argsa; // [sp+30Ch] [bp+8h]@28

  v2 = this;
  //TileTable::dtor((TileTable *)this);
  v3 = fopen(Args, "r");
  File = v3;
  if ( !v3 )
    Error("PlayerFrameTable::load - Unable to open file: %s.", Args);
  v4 = 0;
  v25 = 0;
  v26 = 1;
  if ( fgets(&Buf, 490, v3) )
  {
    do
    {
      *strchr(&Buf, 10) = 0;
      memcpy(&v24, txt_file_frametable_parser(&Buf, &v23), sizeof(v24));
      if ( v24.uPropCount && *v24.pProperties[0] != 47 )
      {
        if ( v24.uPropCount < 3 )
          Error("PlayerFrameTable::load, too few arguments, %s line %i.", Args, v26);
        ++v25;
      }
      ++v26;
    }
    while ( fgets(&Buf, 490, File) );
    v4 = v25;
  }
  v2->uNumFrames = v4;
  v5 = malloc(10 * v4);
  v2->pFrames = (PlayerFrame *)v5;
  if ( !v5 )
    Error("PlayerFrameTable::load - Out of Memory!");
  v6 = File;
  v2->uNumFrames = 0;
  fseek(v6, 0, 0);
  for ( i = fgets(&Buf, 490, File); i; i = fgets(&Buf, 490, File) )
  {
    *strchr(&Buf, 10) = 0;
    memcpy(&v24, txt_file_frametable_parser(&Buf, &v23), sizeof(v24));
    if ( v24.uPropCount && *v24.pProperties[0] != 47 )
    {
      v8 = atoi(v24.pProperties[0]);
      v9 = v24.pProperties[1];
      v2->pFrames[v2->uNumFrames].expression = (CHARACTER_EXPRESSION_ID)v8;
      v10 = atoi(v9);
      v11 = v24.pProperties[2];
      v2->pFrames[v2->uNumFrames].uTextureID = v10;
      v2->pFrames[v2->uNumFrames].uAnimTime = atoi(v11);
      v2->pFrames[v2->uNumFrames].uAnimLength = 0;
      v2->pFrames[v2->uNumFrames].uFlags = 0;
      for ( j = 3; j < v24.uPropCount; ++j )
      {
        if ( !_stricmp(v24.pProperties[j], "New") )
        {
          v13 = (int)&v2->pFrames[v2->uNumFrames].uFlags;
          *(char *)v13 |= 4u;
        }
      }
      ++v2->uNumFrames;
    }
  }
  fclose(File);
  v14 = 0;
  if ( (signed int)(v2->uNumFrames - 1) > 0 )
  {
    v15 = 0;
    do
    {
      v16 = (int)&v2->pFrames[v15];
      if ( !(*(char *)(v16 + 18) & 4) )
        *(char *)(v16 + 8) |= 1u;
      ++v14;
      ++v15;
    }
    while ( v14 < (signed int)(v2->uNumFrames - 1) );
  }
  for ( k = 0; k < (signed int)v2->uNumFrames; *(short *)(Argsa + 6) = v19 )
  {
    v18 = v2->pFrames;
    Argsa = (int)&v18[k];
    v19 = *(short *)(Argsa + 4);
    if ( *(char *)(Argsa + 8) & 1 )
    {
      ++k;
      for ( l = (int)&v18[k]; *(char *)(l + 8) & 1; l += 10 )
      {
        v19 += *(short *)(l + 4);
        ++k;
      }
      LOWORD(v19) = v18[k].uAnimTime + v19;
    }
    ++k;
  }
  return 1;
}

//----- (00495366) --------------------------------------------------------
char *__fastcall sub_495366(unsigned __int8 a1, unsigned __int8 a2)
{
  int v2; // edi@1
  int v3; // edx@2
  int v4; // esi@3
  int v5; // ebx@5
  signed int v7; // [sp+Ch] [bp-14h]@1
  signed int v8; // [sp+10h] [bp-10h]@1
  int **v9; // [sp+14h] [bp-Ch]@4
  signed int v10; // [sp+18h] [bp-8h]@3
  unsigned __int8 v11; // [sp+1Ch] [bp-4h]@1

  v2 = a1;
  v11 = a2;
  v8 = 0;
  v7 = 0;
  if ( dword_AE336C == a1 )
  {
    v3 = dword_AE3370;
  }
  else
  {
    v4 = a2;
    dword_AE336C = a1;
    v10 = 0;
    if ( (signed int)pNPCStats->uNumNPCNames[v4] <= 0 )
	{
		v3 = rand() % (signed int)pNPCStats->uNumNPCNames[v4];
	}
	else
	{
    v9 = (int **)((char *)pNPCStats->pNPCNames + v4 * 4);
    do
    {
      v5 = tolower(*(char *)*v9);
      if ( v5 == tolower(v2) )
      {
        if ( v8 )
          v7 = v10;
        else
          v8 = v10;
      }
      ++v10;
      v9 += 2;
    }
    while ( v10 < (signed int)pNPCStats->uNumNPCNames[v4] );
    if ( v8 && v8 != v7 )
      v3 = v8 + rand() % (v7 - v8);
    else
      v3 = rand() % (signed int)pNPCStats->uNumNPCNames[v4];
	}
  }
  dword_AE3370 = v3;
  return pNPCStats->pNPCNames[0][v11 + 2 * v3];
}


//----- (00495430) --------------------------------------------------------
const char * GetReputationString( signed int a1 )
    {
  if (a1 >= 25)
    return pGlobalTXT_LocalizationStrings[379]; // Hated
  else if (a1 >= 6)
    return pGlobalTXT_LocalizationStrings[392]; // Unfriendly
  else if (a1 >= -5)
    return pGlobalTXT_LocalizationStrings[399]; // Neutral;
  else if (a1 >= -24)
    return pGlobalTXT_LocalizationStrings[402]; // Friendly
  else
    return pGlobalTXT_LocalizationStrings[434]; // Respected;
}

//----- (00495461) --------------------------------------------------------
char *BuildDialogueString(const char *lpsz, unsigned __int8 uPlayerID, ItemGen *a3, char *a4, int a5, __int64 *a6)
{
  Player *pPlayer; // ebx@3
  const char *pText; // esi@7
  int v17; // eax@10
  signed __int64 v18; // qax@18
  unsigned __int8 *v20; // ebx@32
  int v21; // ecx@34
  int pReputation; // eax@45
  int v29; // eax@68
  __int16 v55[56]; // [sp+10h] [bp-128h]@34
  stru351_summoned_item v56; // [sp+80h] [bp-B8h]@107
  char a1[100]; // [sp+B8h] [bp-80h]@3
  int v63; // [sp+12Ch] [bp-Ch]@32

  if ( IsBadStringPtrA(lpsz, 1) )
    return "Invalid String Passed";

  a1[0] = 0;
  pPlayer = &pParty->pPlayers[uPlayerID];
  memset(pTmpBuf2.data(), 0, sizeof(pTmpBuf2));

  NPCData *npc = nullptr;
  if ( dword_5C35D4 )
    npc = HouseNPCData[(unsigned int)((char *)pDialogueNPCCount + -(dword_591080 != 0))]; //- 1
  else
    npc = GetNPCData(sDialogue_SpeakingActorNPC_ID);

  //pText = a4;
  uint len = strlen(lpsz);
  for (int i = 0, dst = 0; i < len; ++i)
  {
    char c = lpsz[i];
    if (c != '%')
      pTmpBuf2[dst++] = c;
    else
    {
      v17 = 10 * (int)(lpsz[i + 1] - '0') + lpsz[i + 2] - '0';

      switch ( v17 )
      {
        case 1://Подробнее
          strcat(pTmpBuf2.data(), npc->pName);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 2:
          strcat(pTmpBuf2.data(), pPlayer->pName);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 3:
        case 4:
          strcat(pTmpBuf2.data(), a1);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 5:
          v18 = (signed __int64)((double)(signed __int64)pParty->uTimePlayed * 0.234375) / 60 / 60 % 24;
          pText = pGlobalTXT_LocalizationStrings[397];// "evening"
          if ( SHIDWORD(v18) <= 0 && SHIDWORD(v18) >= 0 && (unsigned int)v18 >= 5 && SHIDWORD(v18) <= 0 )
          {
            if ( SHIDWORD(v18) >= 0 && (unsigned int)v18 >= 11 )
            {
              if ( v18 < 20 )
                pText = pGlobalTXT_LocalizationStrings[396];// "day"
            }
            else
            {
              pText = pGlobalTXT_LocalizationStrings[395];// "morning"
            }
          }
          strcat(pTmpBuf2.data(), pText);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 6:
          if ( pPlayer->uSex )
            pText = pGlobalTXT_LocalizationStrings[387];// "lady"
          else
            pText = pGlobalTXT_LocalizationStrings[385];// "sir"
          strcat(pTmpBuf2.data(), pText);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 7:
          if ( pPlayer->uSex )
            pText = pGlobalTXT_LocalizationStrings[389];// "Lady"
          else
            pText = pGlobalTXT_LocalizationStrings[386];// "Sir"
          strcat(pTmpBuf2.data(), pText);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 8:
          v63 = 0;
          v20 = (unsigned __int8 *)pPlayer->_achieved_awards_bits;
          for ( uint _i = 0; _i < 28; ++_i )
          {
            if ( (unsigned __int16)_449B57_test_bit(v20, word_4EE150[i]) )
            {
              v21 = v63;
              ++v63;
              v55[v63] = word_4EE150[i];
            }
          }
          if ( v63 )
          {
            if ( dword_A74CDC == -1 )
              dword_A74CDC = rand() % v63;
            pText = (char *)pAwards[v55[dword_A74CDC]].pText;//(char *)dword_723E80_award_related[2 * v55[v24]];
          }
          else
            pText = (char *)pNPCTopics[55].pText;
          strcat(pTmpBuf2.data(), pText);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 9:
          if ( npc->uSex )
            pText = pGlobalTXT_LocalizationStrings[384];// "her"
          else
            pText = pGlobalTXT_LocalizationStrings[383];// "his"
          strcat(pTmpBuf2.data(), pText);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 10:
          if ( pPlayer->uSex )
            pText = pGlobalTXT_LocalizationStrings[389];// "Lady"
          else
            pText = pGlobalTXT_LocalizationStrings[388];// "Lord"
          strcat(pTmpBuf2.data(), pText);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 11:
          pReputation = pParty->GetPartyReputation();
          if ( pReputation >= 25 )
            pText = pGlobalTXT_LocalizationStrings[379];
          else//v25 < 25
          {
            if ( pReputation < 6 )
            {
              if ( pReputation >= -5 )//6 >= v25 >= -5
                pText = pGlobalTXT_LocalizationStrings[399];
              else// v25 < -5
              {
                if ( pReputation < -24 )//-24 > v25
                  pText = pGlobalTXT_LocalizationStrings[434];
                else// -5 > v25 > -24
                  pText = pGlobalTXT_LocalizationStrings[402];
              }
            }
            else//25 > v25 > 6
              pText = pGlobalTXT_LocalizationStrings[392];
          }
          strcat(pTmpBuf2.data(), pText);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 12:
          pReputation = npc->rep;
          if ( pReputation >= 25 )
            pText = pGlobalTXT_LocalizationStrings[379];
          else
          {
            if ( pReputation < 6 )
            {
              if ( pReputation >= -5 )
                pText = pGlobalTXT_LocalizationStrings[399];
              else
              {
                if ( pReputation < -24 )
                  pText = pGlobalTXT_LocalizationStrings[434];
                else
                  pText = pGlobalTXT_LocalizationStrings[402];
              }
            }
            else
              pText = pGlobalTXT_LocalizationStrings[392];
          }
          strcat(pTmpBuf2.data(), pText);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 13:
          strcat(pTmpBuf2.data(), sub_495366(pPlayer->pName[0], pPlayer->uSex));
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 14:
          if ( npc->uSex )
            pText = pGlobalTXT_LocalizationStrings[391];// "sister"
          else
            pText = pGlobalTXT_LocalizationStrings[390];// "brother"
          strcat(pTmpBuf2.data(), pText);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 15:
          strcat(pTmpBuf2.data(), pGlobalTXT_LocalizationStrings[393]);// "daughter"
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 16:
          if ( npc->uSex )
            pText = pGlobalTXT_LocalizationStrings[391];// "sister"
          else
            pText = pGlobalTXT_LocalizationStrings[390];// "brother"
          strcat(pTmpBuf2.data(), pText);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 17://текст наёмного НПС
        {
          uint pay_percentage = pNPCStats->pProfessions[npc->uProfession - 1].uHirePrice / 100;
          if ( !pay_percentage )
            pay_percentage = 1;
          sprintf(a1, "%lu", pay_percentage);
          strcat(pTmpBuf2.data(), a1);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        }
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 26:
          strncpy(a1, lpsz + i + 1, 2);
          sprintf(a1, "%lu", atoi(a1));
          strcat(pTmpBuf2.data(), a1);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 23:
          if ( pMapStats->GetMapInfo(pCurrentMapName) )
            pText = pMapStats->pInfos[pMapStats->GetMapInfo(pCurrentMapName)].pName;
          else
            pText = pGlobalTXT_LocalizationStrings[394];// "Unknown"
          strcat(pTmpBuf2.data(), pText);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 24://название товара в продаже
          sprintfex(a1, format_4E2D80, TargetColor(255, 255, 155), a3->GetDisplayName());
          strcat(pTmpBuf2.data(), a1);
          dst = strlen(pTmpBuf2.data());
          i += 2;
          break;
        case 25:
          v29 = pPlayer->GetBaseBuyingPrice(a3->GetValue(), p2DEvents[(signed int)a4 - 1].fPriceMultiplier);
          switch ( a5 )
            {
              case 3:
                v29 = pPlayer->GetBaseSellingPrice(a3->GetValue(), p2DEvents[(signed int)a4 - 1].fPriceMultiplier);
                break;
              case 4:
                v29 = pPlayer->GetBaseIdentifyPrice(p2DEvents[(signed int)a4 - 1].fPriceMultiplier);
                break;
              case 5:
                v29 = pPlayer->GetBaseRepairPrice(a3->GetValue(), p2DEvents[(signed int)a4 - 1].fPriceMultiplier);
                break;
              case 6:
                v29 = pPlayer->GetBaseSellingPrice(a3->GetValue(), p2DEvents[(signed int)a4 - 1].fPriceMultiplier) / 2;
                break;
            }
            sprintfex(a1, "%lu", v29);
            strcat(pTmpBuf2.data(), a1);
            dst = strlen(pTmpBuf2.data());
            i += 2;
            break;
          case 27://текст продажи
            v29 = pPlayer->GetBuyingPrice(a3->GetValue(), p2DEvents[(signed int)a4 - 1].fPriceMultiplier);
            if ( a5 == 3 )
            {
              v29 = pPlayer->GetPriceSell(a3->GetValue(), p2DEvents[(signed int)a4 - 1].fPriceMultiplier);
              if (a3->IsBroken())
                v29 = 1;
              sprintfex(a1, "%lu", v29);
              strcat(pTmpBuf2.data(), a1);
              dst = strlen(pTmpBuf2.data());
              i += 2;
              break;
            }
            if ( a5 != 4 )
            {
              if ( a5 == 5 )
                v29 = pPlayer->GetPriceRepair(a3->GetValue(), p2DEvents[(signed int)a4 - 1].fPriceMultiplier);
              else
              {
                if ( a5 == 6 )
                {
                  v29 = pPlayer->GetPriceSell(a3->GetValue(), p2DEvents[(signed int)a4 - 1].fPriceMultiplier) / 2;
                  if (a3->IsBroken())
                    v29 = 1;
                  if (!v29)
                    v29 = 1;
                  sprintfex(a1, "%lu", v29);
                  strcat(pTmpBuf2.data(), a1);
                  dst = strlen(pTmpBuf2.data());
                  i += 2;
                  break;
                }
              }
              sprintfex(a1, "%lu", v29);
              strcat(pTmpBuf2.data(), a1);
              dst = strlen(pTmpBuf2.data());
              i += 2;
              break;
            }
            sprintfex(a1, "%lu", pPlayer->GetPriceIdentification(p2DEvents[(signed int)a4 - 1].fPriceMultiplier));
            strcat(pTmpBuf2.data(), a1);
            dst = strlen(pTmpBuf2.data());
            i += 2;
            break;
          case 28://профессия
            strcat(pTmpBuf2.data(), (char *)p2DEvents[(signed int)a4 - 1].pProprieterTitle);
            dst = strlen(pTmpBuf2.data());
            i += 2;
            break;
          case 29:
            sprintfex(a1, "%lu", pPlayer->GetPriceIdentification(p2DEvents[(signed int)a4 - 1].fPriceMultiplier));
            strcat(pTmpBuf2.data(), a1);
            dst = strlen(pTmpBuf2.data());
            i += 2;
            break;
          case 30:
            if ( !a6 )
            {
              strcat(pTmpBuf2.data(), a4);
              dst = strlen(pTmpBuf2.data());
              i += 2;
              break;
            }
            init_summoned_item(&v56, *a6);
            sprintfex(a1, pGlobalTXT_LocalizationStrings[378], aMonthNames[v56.field_14_exprie_month], v56.field_C_expire_day + 1, v56.field_18_expire_year);
            strcat(pTmpBuf2.data(), a1);
            dst = strlen(pTmpBuf2.data());
            i += 2;
            break;
          case 31:
          case 32:
          case 33:
          case 34:
            strcat(pTmpBuf2.data(), pParty->pPlayers[v17 - 31].pName);
            dst = strlen(pTmpBuf2.data());
            i += 2;
            break;
          default:
            if ( v17 <= 50 || v17 > 70 )
            {
              strncpy(a1, lpsz + i + 1, 2);
              sprintf(a1, "%lu", atoi(a1));
              strcat(pTmpBuf2.data(), a1);
              dst = strlen(pTmpBuf2.data());
              i += 2;
              break;
            }
            if ( v17 - 51 >= 20 )
            {
              strcat(pTmpBuf2.data(), a4);
              dst = strlen(pTmpBuf2.data());
              i += 2;
              break;
            }
            init_summoned_item(&v56, pParty->PartyTimes._s_times[v17-51]);
            sprintfex(a1, pGlobalTXT_LocalizationStrings[378], aMonthNames[v56.field_14_exprie_month], v56.field_C_expire_day + 1, v56.field_18_expire_year);
            strcat(pTmpBuf2.data(), a1);
            dst = strlen(pTmpBuf2.data());
            i += 2;
          break;
        }
      }
  }
  return pTmpBuf2.data();
}

//----- (0049B04D) --------------------------------------------------------
void stru154::GetFacePlaneAndClassify(ODMFace *a2, BSPVertexBuffer *a3)
{
  //stru154 *v3; // edi@1
  //signed int v4; // eax@1
  //signed int result; // eax@9
  //signed int v6; // [sp-8h] [bp-18h]@8
  Vec3_float_ v; // [sp+4h] [bp-Ch]@1
  float v7;

  v.x = 0.0;
  v.y = 0.0;
  v.z = 0.0;
  GetFacePlane(a2, a3, &v, &v7);

  if (fabsf(a2->pFacePlane.vNormal.z) < 1e-6f)
    polygonType = POLYGON_VerticalWall;
  else if (fabsf(a2->pFacePlane.vNormal.x) < 1e-6f &&
           fabsf(a2->pFacePlane.vNormal.y) < 1e-6f)
    polygonType = POLYGON_Floor;
  else
    polygonType = POLYGON_InBetweenFloorAndWall;

  face_plane.vNormal.x = v.x;
  face_plane.vNormal.y = v.y;
  face_plane.vNormal.z = v.z;
  face_plane.dist = v7;
}

//----- (0049B0C9) --------------------------------------------------------
void stru154::ClassifyPolygon(Vec3_float_ *pNormal, float dist)
{
  if (fabsf(pNormal->z) < 1e-6f)
    polygonType = POLYGON_VerticalWall;
  else if (fabsf(pNormal->x) < 1e-6f &&
           fabsf(pNormal->y) < 1e-6f)
    polygonType = POLYGON_Floor;
  else
    polygonType = POLYGON_InBetweenFloorAndWall;

  face_plane.vNormal.x = pNormal->x;
  face_plane.dist = dist;
  face_plane.vNormal.y = pNormal->y;
  face_plane.vNormal.z = pNormal->z;
}

//----- (0049B13D) --------------------------------------------------------
void stru154::GetFacePlane(ODMFace *pFace, BSPVertexBuffer *pVertices, Vec3_float_ *pOutNormal, float *pOutDist)
{
  ODMFace *v5; // ebx@1
  //int v6; // eax@1
  //unsigned __int16 *v7; // ebx@2
  //Vec3_int_ *v8; // eax@3
  Vec3_int_ *v9; // ecx@3
  //double v10; // st7@3
  //int v11; // ecx@3
  Vec3_int_ *v12; // ecx@3
  //double v13; // st7@3
  //double v14; // st6@3
  //double v15; // st5@3
  //int v16; // ecx@3
  Vec3_int_ *v17; // eax@3
  //double v18; // st5@3
  Vec3_float_ *v19; // eax@3
  //int result; // eax@8
  //float v21; // ecx@10
  //double v22; // st7@10
  //double v23; // st6@10
  Vec3_float_ v2; // [sp+4h] [bp-64h]@3
  //float v25; // [sp+18h] [bp-50h]@3
  float v26; // [sp+1Ch] [bp-4Ch]@3
  float v27; // [sp+20h] [bp-48h]@3
  float v28; // [sp+24h] [bp-44h]@3
  //float v29; // [sp+2Ch] [bp-3Ch]@3
  //float v30; // [sp+30h] [bp-38h]@3
  //float v31; // [sp+34h] [bp-34h]@3
  //float v32; // [sp+38h] [bp-30h]@3
  //float v33; // [sp+3Ch] [bp-2Ch]@3
  Vec3_float_ v1; // [sp+40h] [bp-28h]@1
  //float v35; // [sp+4Ch] [bp-1Ch]@3
  //float v36; // [sp+50h] [bp-18h]@3
  //float v37; // [sp+54h] [bp-14h]@3
  Vec3_float_ v38; // [sp+58h] [bp-10h]@3
  //int v39; // [sp+64h] [bp-4h]@1

  //v39 = 0;
  v1.x = 0.0;
  v5 = pFace;
  //v6 = pFace->uNumVertices;
  v1.y = 0.0;
  v1.z = 0.0;

  if (pFace->uNumVertices >= 2)
  {
    int i = 0;
    while ( i < pFace->uNumVertices - 2 )
    {
      v9 = &pVertices->pVertices[pFace->pVertexIDs[i]];
      v12 = &pVertices->pVertices[pFace->pVertexIDs[i + 1]];
      v17 = &pVertices->pVertices[pFace->pVertexIDs[i + 2]];
	  i++;
      v1.x = v12->x - v9->x;
      v26 = v17->x - v12->x;
      v1.y = v12->y - v9->y;
      v27 = v17->y - v12->y;
      v1.z = v12->z - v9->z;
      v28 = v17->z - v12->z;
      v19 = Vec3_float_::Cross(&v1, &v2, v26, v27, v28);
      v38.x = v19->x;
      v38.y = v19->y;
      v38.z = v19->z;
      if ( v38.x != 0.0  || v38.y != 0.0 || v38.z != 0.0)
      {
        v38.Normalize();

        pOutNormal->x = v38.x;
        pOutNormal->y = v38.y;
        pOutNormal->z = v38.z;

        *pOutDist = -(v9->x * v38.x + v9->y * v38.y + v9->z * v38.z);
        return;
      }
    }
  }

  pOutNormal->x = (double)(v5->pFacePlane.vNormal.x & 0xFFFF) / 65535.0f + (double)(v5->pFacePlane.vNormal.x >> 16);
  pOutNormal->y = (double)(v5->pFacePlane.vNormal.y & 0xFFFF) / 65535.0f + (double)(v5->pFacePlane.vNormal.y >> 16);
  pOutNormal->z = (double)(v5->pFacePlane.vNormal.z & 0xFFFF) / 65535.0f + (double)(v5->pFacePlane.vNormal.z >> 16);
  *pOutDist = (double)(v5->pFacePlane.dist & 0xFFFF) / 65535.0f + (double)(v5->pFacePlane.dist >> 16);
}

//----- (0049D700) --------------------------------------------------------
unsigned int __fastcall GetMaxMipLevels(unsigned int uDim)
{
  unsigned int v1; // eax@1
  int v2; // ecx@1
  unsigned int v3; // eax@1

  v1 = uDim;
  v2 = 0;
  v3 = v1 - 1;
  while ( v3 & 1 )
  {
    v3 >>= 1;
    ++v2;
  }
  return v3 == 0 ? v2 : 0;
}

//----- (004A19D8) --------------------------------------------------------
unsigned int BlendColors(unsigned int a1, unsigned int a2)
{
  /*signed __int64 v2; // ST10_8@1
  double v3; // st7@1
  float v4; // ST24_4@1
  double v5; // ST10_8@1
  int v6; // ST1C_4@1
  float v7; // ST24_4@1
  double v8; // ST10_8@1
  unsigned __int8 v9; // ST20_1@1
  float v10; // ST24_4@1
  double v11; // ST10_8@1
  float v12; // ST24_4@1
  double v13; // ST08_8@1*/

  uint alpha = (uint)floorf(0.5f + (a1 >> 24) / 255.0f *
                                   (a2 >> 24) / 255.0f * 255.0f),
       red = (uint)floorf(0.5f + ((a1 >> 16) & 0xFF) / 255.0f *
                                 ((a2 >> 16) & 0xFF) / 255.0f * 255.0f),
       green = (uint)floorf(0.5f + ((a1 >> 8) & 0xFF) / 255.0f *
                                   ((a2 >> 8) & 0xFF) / 255.0f * 255.0f),
       blue = (uint)floorf(0.5f + ((a1 >> 0) & 0xFF) / 255.0f *
                                   ((a2 >> 0) & 0xFF) / 255.0f * 255.0f);
  return (alpha << 24) | (red << 16) | (green << 8) | blue;
  /*v2 = a1 >> 24;
  v3 = (double)v2 / 255.0f;
  HIDWORD(v2) = 0;
  LODWORD(v2) = a2 >> 24;
  v4 = v3 * (double)v2 / 255.0f * 255.0;
  v5 = v4 + 6.7553994e15;
  v6 = LODWORD(v5);
  v7 = (double)((a1 >> 16) & 0xFFi64) / 255.0f * (double)((a2 >> 16) & 0xFF) * 0.0039215689 * 255.0;
  v8 = v7 + 6.7553994e15;
  v9 = LOBYTE(v8);
  v10 = (double)((unsigned __int16)a1 >> 8) / 255.0f * (double)((unsigned __int16)a2 >> 8) / 255.0f * 255.0;
  v11 = v10 + 6.7553994e15;
  v12 = (double)(a1 & 0xFFi64) / 255.0f * (double)(unsigned __int8)a2 / 255.0f * 255.0;
  v13 = v12 + 6.7553994e15;
  return LOBYTE(v13) | ((LOBYTE(v11) | (((v6 << 8) | v9) << 8)) << 8);*/
}

//----- (004A46E6) --------------------------------------------------------
int __fastcall sr_4A46E6_draw_particle_segment(unsigned int x, signed int y, signed int _z, int a4, unsigned int lightColor)
{
  int v5; // eax@1
  int z; // eax@1
  unsigned int v7; // eax@9
  unsigned int v8; // ecx@9
  int v9; // eax@9
  unsigned int v10; // eax@10
  int *v11; // esi@10
  int *v12; // edi@10
  int v13; // ecx@10
  int v14; // edx@10
  unsigned int v15; // eax@22
  int *v16; // esi@22
  int *v17; // edi@22
  int v18; // ecx@22
  int v19; // edx@22
  unsigned __int16 *pTarget; // [sp+Ch] [bp-8h]@9
  int *pTargetZ; // [sp+10h] [bp-4h]@9
  unsigned int v22; // [sp+1Ch] [bp+8h]@9
  signed int v23; // [sp+20h] [bp+Ch]@1

  v5 = a4;
  v23 = _z >> 16;
  z = x + v5;
  if ( z >= (signed int)pViewport->uViewportTL_X
    && (signed int)x <= (signed int)pViewport->uViewportBR_X
    && y >= (signed int)pViewport->uViewportTL_Y
    && y <= (signed int)pViewport->uViewportBR_Y )
  {
    if ( (signed int)x < (signed int)pViewport->uViewportTL_X )
      x = pViewport->uViewportTL_X;
    if ( z > (signed int)pViewport->uViewportBR_X )
      z = pViewport->uViewportBR_X;
    pTarget = &pRenderer->pTargetSurface[x + y * pRenderer->uTargetSurfacePitch];
    v22 = z - x;
    pTargetZ = &pRenderer->pActiveZBuffer[x + 640 * y];
    v7 = lightColor >> 3;
    v8 = lightColor & 0xF0;
    v9 = v7 & 0x1E0000;
    if ( pRenderer->uTargetGBits == 5 )
    {
      v10 = (v8 | (((unsigned __int16)(lightColor & 0xF000) | (unsigned int)v9) >> 3)) >> 4;
      v11 = (int *)pTarget;
      v12 = pTargetZ;
      v13 = v22;
      v14 = (v10 << 16) | v10;
      z = (unsigned __int8)pTarget & 2;
      if ( (unsigned __int8)pTarget & 2 )
      {
        z = (unsigned int)*pTargetZ >> 16;
        if ( z > v23 )
        {
          z = v14 + ((*pTarget & 0x7BDEu) >> 1);
          *pTarget = z;
        }
        v13 = v22 - 1;
        v11 = (int *)(pTarget + 1);
        v12 = pTargetZ + 1;
      }
      while ( v13 != 1 )
      {
        if ( v13 < 1 )
          return z;
        z = (unsigned int)*v12 >> 16;
        if ( z <= v23 )
        {
          v13 -= 2;
          ++v11;
          v12 += 2;
        }
        else
        {
          v12 += 2;
          z = v14 + ((*v11 & 0x7BDE7BDEu) >> 1);
          v13 -= 2;
          *v11 = z;
          ++v11;
        }
      }
      z = (unsigned int)*v12 >> 16;
      if ( z > v23 )
      {
        z = v14 + ((*(short *)v11 & 0x7BDEu) >> 1);
        *(short *)v11 = z;
      }
    }
    else
    {
      v15 = (v8 | (((unsigned __int16)(lightColor & 0xF800) | (unsigned int)v9) >> 2)) >> 4;
      v16 = (int *)pTarget;
      v17 = pTargetZ;
      v18 = v22;
      v19 = (v15 << 16) | v15;
      z = (unsigned __int8)pTarget & 2;
      if ( (unsigned __int8)pTarget & 2 )
      {
        z = (unsigned int)*pTargetZ >> 16;
        if ( z > v23 )
        {
          z = v19 + ((*pTarget & 0xF7DEu) >> 1);
          *pTarget = z;
        }
        v18 = v22 - 1;
        v16 = (int *)(pTarget + 1);
        v17 = pTargetZ + 1;
      }
      while ( v18 != 1 )
      {
        if ( v18 < 1 )
          return z;
        z = (unsigned int)*v17 >> 16;
        if ( z <= v23 )
        {
          v18 -= 2;
          ++v16;
          v17 += 2;
        }
        else
        {
          v17 += 2;
          z = v19 + ((*v16 & 0xF7DEF7DEu) >> 1);
          v18 -= 2;
          *v16 = z;
          ++v16;
        }
      }
      z = (unsigned int)*v17 >> 16;
      if ( z > v23 )
      {
        z = v19 + ((*(short *)v16 & 0xF7DEu) >> 1);
        *(short *)v16 = z;
      }
    }
  }
  return z;
}

//----- (004A57E9) --------------------------------------------------------
void  Present_ColorKey()
{
  HRESULT v0; // eax@3
  HRESULT v1; // eax@3
  HRESULT v2; // eax@3
  HRESULT v3; // eax@3
  HRESULT v4; // eax@3
  RECT a2; // [sp+4h] [bp-14h]@3
  //CheckHRESULT_stru0 this; // [sp+14h] [bp-4h]@3

  if ( !pRenderer->uNumSceneBegins )
  {
    if ( pRenderer->field_40110 )
    {
      a2.bottom = pViewport->uViewportTL_Y;
      a2.left = 0;
      a2.top = 0;
      a2.right = 640;
      ErrD3D(pRenderer->pBackBuffer4->BltFast(0, 0, pRenderer->pColorKeySurface4, &a2, 16u));
      a2.right = 640;
      a2.left = 0;
      a2.top = pViewport->uViewportBR_Y + 1;
      a2.bottom = 480;
      ErrD3D(pRenderer->pBackBuffer4->BltFast(
             0,
             pViewport->uViewportBR_Y + 1,
             pRenderer->pColorKeySurface4,
             &a2,
             16u));
      a2.right = pViewport->uViewportTL_X;
      a2.bottom = pViewport->uViewportBR_Y + 1;
      a2.left = 0;
      a2.top = pViewport->uViewportTL_Y;
      ErrD3D(pRenderer->pBackBuffer4->BltFast(
             0,
             pViewport->uViewportTL_Y,
             pRenderer->pColorKeySurface4,
             &a2,
             16u));
      a2.left = pViewport->uViewportBR_X;
      a2.top = pViewport->uViewportTL_Y;
      a2.right = 640;
      a2.bottom = pViewport->uViewportBR_Y + 1;
      ErrD3D(pRenderer->pBackBuffer4->BltFast(
             pViewport->uViewportBR_X,
             pViewport->uViewportTL_Y,
             pRenderer->pColorKeySurface4,
             &a2,
             16u));
      a2.right = pViewport->uViewportBR_X;
      a2.bottom = pViewport->uViewportBR_Y + 1;
      a2.left = pViewport->uViewportTL_X;
      a2.top = pViewport->uViewportTL_Y;
      ErrD3D(pRenderer->pBackBuffer4->BltFast(
             pViewport->uViewportTL_X,
             pViewport->uViewportTL_Y,
             pRenderer->pColorKeySurface4,
             &a2,
             17u));
    }
  }
}

//----- (004A597D) --------------------------------------------------------
void Present_NoColorKey()
{
  //unsigned __int16 *v0; // eax@4
  unsigned __int16 *v1; // esi@4
  void *v2; // edi@4
  //signed int v4; // ebx@4
  //signed int v5; // ebx@6
  //void *v6; // edi@7
  //const void *v7; // esi@7
  signed int v8; // ebx@8
  int v9; // eax@10
  unsigned int v10; // esi@10
  unsigned __int32 v11; // edi@10
  //int v12; // ecx@10
  unsigned int v13; // ebx@10
  int v14; // eax@11
  int v15; // eax@13
  int v16; // eax@14
  int v17; // eax@16
  HRESULT v18; // eax@22
  DDSURFACEDESC2 Dst; // [sp+Ch] [bp-98h]@3
  int v20; // [sp+88h] [bp-1Ch]@10
  int v21; // [sp+8Ch] [bp-18h]@10
  __int32 v22; // [sp+90h] [bp-14h]@10
  //unsigned __int32 v23; // [sp+94h] [bp-10h]@10
  unsigned int v24; // [sp+98h] [bp-Ch]@4
  //unsigned int _this; // [sp+9Ch] [bp-8h]@10
  //LPVOID v26; // [sp+A0h] [bp-4h]@4

  if ( !pRenderer->uNumSceneBegins )
  {
    if ( pRenderer->field_40110 )
    {
      memset(&Dst, 0, 0x7Cu);
      Dst.dwSize = 124;
      if ( pRenderer->LockSurface_DDraw4(pRenderer->pBackBuffer4, &Dst, DDLOCK_WAIT) )
      {
        //v26 = Dst.lpSurface;
        pRenderer->pCurrentlyLockedSurfaceDataPtr = (unsigned __int16 *)Dst.lpSurface;
        v24 = pRenderer->uTargetGMask | pRenderer->uTargetBMask |
              ((pRenderer->uTargetGMask | pRenderer->uTargetBMask) << 16);
        pRenderer->pCurrentlyLockedSoftSurface = pRenderer->pTargetSurface;
        pRenderer->uCurrentlyLockedSurfacePitch = Dst.lPitch;
        v1 = pRenderer->pTargetSurface;
        v2 = Dst.lpSurface;


        /*for (uint y = 0; y < 480; ++y)
        {
          auto pDst = (unsigned short *)((char *)Dst.lpSurface + y * Dst.lPitch);
          for (uint x = 0; x < 640; ++x)
            pDst[x] = pRenderer->uTargetRMask | pRenderer->uTargetBMask;
        }*/
        
        ushort* pSrc = pRenderer->pTargetSurface;
        short* pDst = (__int16 *)Dst.lpSurface;

        for (uint y = 0; y < 8; ++y)
          memcpy(pDst + y * Dst.lPitch / 2,

		  pSrc + y * 640, 640 * sizeof(__int16));

        for (uint y = 8; y < 352; ++y)
        {
          memcpy(pDst + y * Dst.lPitch / 2,
                 pSrc + y * 640, 8 * sizeof(__int16));
          memcpy(pDst + 8 + game_viewport_width/*462*/ + y * Dst.lPitch / 2,
                 pSrc + 8 + game_viewport_width/*462*/ + y * 640, 174/*172*/ * sizeof(__int16));
        }

        for (uint y = 352; y < 480; ++y)
          memcpy(pDst + y * Dst.lPitch / 2,
                 pSrc + y * 640, 640 * sizeof(__int16));


        ushort* pSrc_x1y1 = pSrc + 640 * pViewport->uViewportTL_Y + pViewport->uViewportTL_X;
        //_this = (unsigned int)&pSrc[2 * (((signed int)pViewport->uViewportX >> 1) + 320 * pViewport->uViewportY)];
        short* pDst_x1y1 = pDst + Dst.lPitch * pViewport->uViewportTL_Y + pViewport->uViewportTL_X;
        //v23 = (unsigned __int32)((char *)v26 + 4 * (((signed int)pViewport->uViewportX >> 1) + (Dst.lPitch >> 2) * pViewport->uViewportY));
        v9 = ((signed int)pViewport->uViewportTL_X >> 1) - ((signed int)pViewport->uViewportBR_X >> 1);
        //v20 = ((signed int)pViewport->uViewportZ >> 1) - ((signed int)pViewport->uViewportX >> 1);
        v22 = 4 * ((Dst.lPitch / 4) + v9);
        v21 = 4 * v9 + 1280;

        //auto uNumLines = pViewport->uViewportW - pViewport->uViewportY + 1;
        //v26 = (LPVOID)(pViewport->uViewportW - pViewport->uViewportY + 1);
        v10 = (int)pSrc_x1y1;
        v11 = (int)pDst_x1y1;
        int uHalfWidth = v20 = (pViewport->uViewportBR_X - pViewport->uViewportTL_X) / 2;
        v13 = v24;

        for (uint y = pViewport->uViewportTL_Y; y < pViewport->uViewportBR_Y + 1; ++y)
        {
          //memcpy(pDst + pViewport->uViewportX + y * Dst.lPitch / 2,
          //       pSrc + pViewport->uViewportX + y * 640, (pViewport->uViewportZ - pViewport->uViewportX) * sizeof(__int16));
          for (uint x = pViewport->uViewportTL_X; x < pViewport->uViewportBR_X; ++x)
          {
            if (pSrc[y * 640 + x] != (pRenderer->uTargetGMask | pRenderer->uTargetBMask))
              pDst[y * Dst.lPitch / 2 + x] = pSrc[y * 640 + x];
          }
        }

              ErrD3D(pRenderer->pBackBuffer4->Unlock(0));

       /* while ( 1 )
        {
          while ( 1 )
          {
            v14 = *(int *)v10;
            v10 += 4;
            if ( v14 == v13 )
              break;
            if ( (short)v14 == (short)v13 )
            {
              *(int *)v11 = *(int *)v11 & 0xFFFF | v14 & 0xFFFF0000;
              v11 += 4;
              --uHalfWidth;
              if ( !uHalfWidth )
                goto LABEL_21;
            }
            else
            {
              v15 = __ROL__(v14, 16);
              if ( (short)v15 == (short)v13 )
              {
                v17 = __ROR__(v15, 16);
                *(int *)v11 = *(int *)v11 & 0xFFFF0000 | (unsigned __int16)v17;
                v11 += 4;
                --uHalfWidth;
                if ( !uHalfWidth )
                  goto LABEL_21;
              }
              else
              {
                v16 = __ROR__(v15, 16);
                *(int *)v11 = v16;
                v11 += 4;
                --uHalfWidth;
                if ( !uHalfWidth )
                  goto LABEL_21;
              }
            }
          }
          v11 += 4;
          --uHalfWidth;
          if ( !uHalfWidth )
          {
LABEL_21:
            v10 += v21;
            v11 += v22;
            uHalfWidth = v20;
            if ( !--uNumLines )
            {
              ErrD3D(pRenderer->pBackBuffer4->Unlock(0));
              return;
            }
          }
        }*/
      }
    }
  }
}

//----- (004A7063) --------------------------------------------------------
unsigned int ModulateColor(unsigned int diffuse, float multiplier)
{
  float alpha = multiplier * ((diffuse >> 24) & 0xFF);
  int   a = (int)floorf(alpha + 0.5f);
  a = max(0, min(255, a));

  float red = multiplier * ((diffuse >> 16) & 0xFF);
  int   r = (int)floorf(red + 0.5f);
  r = max(0, min(255, r));
  
  float green = multiplier * ((diffuse >> 8) & 0xFF);
  int   g = (int)floorf(green + 0.5f);
  g = max(0, min(255, g));
  
  float blue = multiplier * ((diffuse >> 0) & 0xFF);
  int   b = (int)floorf(blue + 0.5f);
  b = max(0, min(255, b));

  return (a << 24) | (r << 16) | (g << 8) | b;
}



//----- (004B1447) --------------------------------------------------------
void sub_4B1447_party_fine(int a1, int a2, int a3)
{
  signed int v3; // esi@1
  char v4; // sf@8
  int v5; // eax@8
  unsigned __int64 v6; // qax@12
  DDM_DLV_Header *v7; // eax@14

  v3 = 0;
  if ( a2 )
  {
    if ( a2 == 1 )
      v3 = 2;
    if ( a2 == 2 )
      v3 = 2;
      goto LABEL_13;
  }
  else
  {
    v3 = 1;
  }
  if ( pParty->uFine < 4000000 )
  {
    v4 = a3 + pParty->uFine < 0;
    v5 = a3 + pParty->uFine;
    pParty->uFine += a3;
    if ( v4 )
    {
      v5 = 0;
      pParty->uFine = 0;
    }
    if ( v5 > 4000000 )
      pParty->uFine = 4000000;
  }

  pParty->PartyTimes._shop_ban_times[a1] = pParty->uTimePlayed + 368640;
 
LABEL_13:
  pParty->InTheShopFlags[a1] = 1;
  if ( v3 )
  {
    v7 = &pOutdoor->ddm;
    if ( uCurrentlyLoadedLevelType != LEVEL_Outdoor )
      v7 = &pIndoor->dlv;
    v7->uReputation += v3;
    if ( v7->uReputation > 10000 )
      v7->uReputation = 10000;
  }
  for ( uint i = 1; i <= 4; ++i )
  {
    if ( pParty->uFine )
    {
      if ( !_449B57_test_bit(pPlayers[i]->_achieved_awards_bits, 1) )
        _449B7E_toggle_bit(pPlayers[i]->_achieved_awards_bits, 1, 1);
    }
  }
}

//----- (004B1523) --------------------------------------------------------
void  sub_4B1523(int *_this)
{
  int v1; // esi@1
  int v2; // edx@1
  unsigned int v3; // eax@2
  int v4; // eax@4
  LONG v5; // ecx@4
  int v6; // eax@10
  char *v7; // ST44_4@12
  unsigned __int16 v8; // ax@12
  GUIWindow a1; // [sp+Ch] [bp-68h]@4
  unsigned int v11; // [sp+60h] [bp-14h]@1
  POINT a2; // [sp+64h] [bp-10h]@1
  int v13; // [sp+6Ch] [bp-8h]@4
  int v14; // [sp+70h] [bp-4h]@4

  v1 = *_this - 399;
  v2 = (*_this - 400) % 11 + 1;
  v11 = 4 * (*_this - 400) / 11;
 // sprintf(pTmpBuf.data(), "%s%03d", spellbook_texture_filename_suffices[v11 / 4], v2); not used
  if ( pMouse->GetCursorPos(&a2)->y <= 320 )
    v3 = pMouse->GetCursorPos(&a2)->y + 30;
  else
    v3 = 30;
  a1.Hint = 0;
  a1.uFrameY = v3;
  a1.uFrameWidth = 328;
  a1.uFrameHeight = 68;
  a1.uFrameX = 90;
  a1.uFrameZ = 417;
  a1.uFrameW = v3 + 67;
  a2.y = pFontSmallnum->GetLineWidth(pGlobalTXT_LocalizationStrings[431]);
  v14 = pFontSmallnum->GetLineWidth(pGlobalTXT_LocalizationStrings[433]);
  v13 = pFontSmallnum->GetLineWidth(pGlobalTXT_LocalizationStrings[432]);
  v4 = pFontSmallnum->GetLineWidth(pGlobalTXT_LocalizationStrings[96]);
  v5 = a2.y;
  if ( v14 > a2.y )
    v5 = v14;
  if ( v13 > v5 )
    v5 = v13;
  if ( v4 > v5 )
    v5 = v4;
  sprintf(pTmpBuf2.data(), "%s\n\n%s\t%03d:\t%03d%s\t000\n%s\t%03d:\t%03d%s\t000\n%s\t%03d:\t%03d%s\t000\n%s\t%03d:\t%03d%s",
    pSpellStats->pInfos[v1].pDescription, pGlobalTXT_LocalizationStrings[431],        // "Normal"
    v5 + 3, v5 + 10, pSpellStats->pInfos[v1].pBasicSkillDesc, pGlobalTXT_LocalizationStrings[433],        // "Expert"
    v5 + 3, v5 + 10, pSpellStats->pInfos[v1].pExpertSkillDesc, pGlobalTXT_LocalizationStrings[432],        // "Master"
    v5 + 3, v5 + 10, pSpellStats->pInfos[v1].pMasterSkillDesc, pGlobalTXT_LocalizationStrings[96],         // "Grand"
    v5 + 3, v5 + 10, pSpellStats->pInfos[v1].pGrandmasterSkillDesc);
  v6 = pFontSmallnum->CalcTextHeight(pTmpBuf2.data(), &a1, 0, 0);
  a1.uFrameHeight += v6;
  if ( (signed int)a1.uFrameHeight < 150 )
    a1.uFrameHeight = 150;
  a1.uFrameWidth = game_viewport_width;
  a1.DrawMessageBox(0);
  a1.uFrameWidth -= 12;
  a1.uFrameHeight -= 12;
  v7 = pSpellStats->pInfos[v1].pName;
  a1.uFrameZ = a1.uFrameX + a1.uFrameWidth - 1;
  a1.uFrameW = a1.uFrameHeight + a1.uFrameY - 1;
  v8 = TargetColor(0xFFu, 0xFFu, 0x9Bu);
  a1.DrawTitleText(pFontArrus, 0x78u, 0xCu, v8, v7, 3u);
  a1.DrawText(pFontSmallnum, 120, 44, 0, pTmpBuf2.data(), 0, 0, 0);
  a1.uFrameZ = a1.uFrameX + 107;
  a1.uFrameWidth = 108;
  a1.DrawTitleText(pFontComic, 0xCu, 0x4Bu, 0, pSkillNames[v11 / 4 + 12], 3u);
  sprintfex(pTmpBuf.data(), "%s\n%d", pGlobalTXT_LocalizationStrings[522], *(&pSpellDatas[0].uNormalLevelMana + 10 * v1));
  a1.DrawTitleText(pFontComic, 0xCu, a1.uFrameHeight - LOBYTE(pFontComic->uFontHeight) - 16, 0, pTmpBuf.data(), 3);
}

//----- (004B1ECE) --------------------------------------------------------
void OracleDialogue()
{
  __int16 *v0; // edi@1
  int v1; // ebx@3
  Player *v2; // esi@3
  int v3; // eax@4
  signed int v4; // eax@9
  int v5; // ebx@11
  Player *v6; // esi@13
  ItemGen *v7; // eax@14
  signed int v8; // edi@14
  ItemGen *v9; // [sp+Ch] [bp-Ch]@11
  signed int v10; // [sp+10h] [bp-8h]@13
  int v11; // [sp+14h] [bp-4h]@1
  Player *v12; // [sp+14h] [bp-4h]@11

  contract_approved = 0;
  v11 = 0;
  uDialogueType = 84;
  current_npc_text = (char *)pNPCTopics[667].pText;
  v0 = _4F0882_evt_VAR_PlayerItemInHands_vals.data();
  while ( 1 )
  {
    if ( (unsigned __int16)_449B57_test_bit(pParty->_quest_bits, *v0) )
    {
      v1 = 0;
      v2 = pParty->pPlayers.data();
      do
      {
        LOBYTE(v3) = v2->CompareVariable(VAR_PlayerItemInHands, *(v0+1));
        if ( v3 )
          break;
        ++v2;
        ++v1;
      }
      while ( (signed int)v2 < (signed int)pParty->pHirelings.data() );
      if ( v1 == 4 )
        break;
    }
    ++v11;
    v0 += 2;
    if ( v0 > &_4F0882_evt_VAR_PlayerItemInHands_vals[53] )
	  break;
  }
  if ( v0 <= &_4F0882_evt_VAR_PlayerItemInHands_vals[53] )
  {
	  current_npc_text = (char *)pNPCTopics[666].pText; // Here's %s that you lost. Be careful
	  v4 = _4F0882_evt_VAR_PlayerItemInHands_vals[2 * v11];
	  contract_approved = _4F0882_evt_VAR_PlayerItemInHands_vals[2 * v11];
	  pParty->pPlayers[0].AddVariable(VAR_PlayerItemInHands, v4);
  }
  if ( contract_approved == 601 )
  {
    v5 = 0;
    v12 = pParty->pPlayers.data();//[0].uClass;
    v9 = 0;
    while ( 1 )
    {
      if ( v12->classType == PLAYER_CLASS_LICH )
      {
        v10 = 0;
        v6 = pParty->pPlayers.data();//[0].pInventoryItems[0].field_1A;
        do
        {
		  v7 = v6->pInventoryItemList.data();
          v8 = 138;
          do
          {
			if ( v7->uItemID == ITEM_LICH_JAR_FULL )
            {
			  if ( !v7->uHolderPlayer )
				  v9 = v7;
			  if ( v7->uHolderPlayer == v5 )
                v10 = 1;
            }
            ++v7;
            --v8;
          }
          while ( v8 );
          ++v6;
        }
		while ( v6 <= &pParty->pPlayers[3] );
        if ( !v10 )
          break;
      }
      ++v12;
      ++v5;
	  if ( v12 > &pParty->pPlayers[3] )
        return;
    }
    if ( v9 )
	  v9->uHolderPlayer = v5;
  }
}

//----- (004B254D) --------------------------------------------------------
char * _4B254D_SkillMasteryTeacher(int _this)
{
  //Player *v1; // esi@1
  int v2; // edx@1
  int v3; // ecx@1
  int v4; // edi@1
  int pClassType; // eax@7
  int v6; // eax@7
  int v7; // ebx@7
  //int v8; // ebx@8
  signed int v9; // esi@8
  int v10; // eax@8
  char *v11; // ecx@8
  int v12; // edi@9
  char *v13; // edx@9
  signed int v14; // edi@10
  unsigned int v16; // eax@29
  //int v17; // eax@36
  char v18; // cl@46
  __int16 v19; // dx@56
  int v20; // eax@60
  //char *v21; // [sp-Ch] [bp-38h]@82
  //const char *v22; // [sp-8h] [bp-34h]@21
  //unsigned int v23; // [sp-8h] [bp-34h]@38
  //char *v24; // [sp-8h] [bp-34h]@82
  const char *v25; // [sp-4h] [bp-30h]@14
  //int v26; // [sp-4h] [bp-30h]@38
  //int v27; // [sp-4h] [bp-30h]@82
  char v28[4]; // [sp+Ch] [bp-20h]@9
  int v29; // [sp+10h] [bp-1Ch]@13
  int v30; // [sp+14h] [bp-18h]@15
  int v31; // [sp+18h] [bp-14h]@16
  unsigned __int16 a1[2]; // [sp+1Ch] [bp-10h]@7
  //int v33; // [sp+20h] [bp-Ch]@7
  int v34; // [sp+24h] [bp-8h]@7
  char *v35; // [sp+28h] [bp-4h]@1

  contract_approved = 0;
  v2 = (_this - 200) % 3;
  v3 = (_this - 200) / 3;
  v4 = v2;
  v35 = (char *)pNPCTopics[127].pText;
  dword_F8B1AC_award_bit_number = v3;
  if ( v2 )
  {
    if ( v2 == 1 )
    {
      gold_transaction_amount = 5000;
      dword_F8B1B0 = 3;
    }
    else
    {
      if ( v2 == 2 )
      {
        gold_transaction_amount = 8000;
        dword_F8B1B0 = 4;
      }
    }
  }
  else
  {
    gold_transaction_amount = 2000;
    dword_F8B1B0 = 2;
  }
  pClassType = pPlayers[uActiveCharacter]->classType;
  //v33 = pClassType;
  v6 = byte_4ED970_skill_learn_ability_by_class_table[pClassType][v3];
  *(int *)a1 = pPlayers[uActiveCharacter]->pActiveSkills[v3];
  v7 = a1[0] & 0x3F;
  v34 = v2 + 2;
  if ( v6 < v2 + 2 )
  {
    //v8 = v33;
    v9 = 0;
    v10 = pClassType - pClassType % 4;
    v11 = &byte_4ED970_skill_learn_ability_by_class_table[pClassType - pClassType % 4][v3];
    do
    {
      v12 = (unsigned __int8)*v11;
      v13 = &v28[4 * v9];
      *(int *)v13 = 0;
      if ( v12 < v34 )
      {
        v14 = 1;
      }
      else
      {
        v14 = 1;
        *(int *)v13 = 1;
      }
      ++v9;
      v11 += 37;
    }
    while ( v9 < 4 );
    __debugbreak(); // warning C4700: uninitialized local variable 'v29' used
    if ( v29 == v14 )
    {
      v25 = pClassNames[v10 + 1];
    }
    else
    {
      __debugbreak(); // warning C4700: uninitialized local variable 'v30' used
      if ( v30 == v14 )//crash
      {
        __debugbreak(); // warning C4700: uninitialized local variable 'v31' used
        if ( v31 == v14 )
        {
          sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[634], pClassNames[v10 + 2], pClassNames[v10 + 3]);//Вы должны достичь звания %s или %s для обучения этому уровню навыка.
          return pTmpBuf.data();
        }
        v25 = pClassNames[v10 + 2];
      }
      else
      {
        if ( v31 != v14 )
        {
          sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[632], pClassNames[pClassType]);//Этот уровень навыка не может быть постигнут классом %s.
          return pTmpBuf.data();
        }
        v25 = pClassNames[v10 + 3];
      }
    }
    sprintf(pTmpBuf.data(), pGlobalTXT_LocalizationStrings[633], v25);//Вы должны достичь звания %s для обучения этому уровню навыка.
    return pTmpBuf.data();
  }
  if ( !pPlayers[uActiveCharacter]->CanAct() )
    return (char *)pNPCTopics[122].pText;
  if ( !v7 )
    return (char *)pNPCTopics[131].pText;
  v16 = SkillToMastery(a1[0]);
  if ( (signed int)v16 > v4 + 1 )
    return (char *)pNPCTopics[v4 + 128].pText;
  if ( v34 != 2 )
  {
    if ( v34 == 3 )
    {
      if ( (signed int)v16 >= 2 && v7 >= 7 )
      {
        switch ( dword_F8B1AC_award_bit_number )
        {
          case 12:
          case 13:
          case 14:
          case 15:
          case 16:
          case 17:
          case 18:
            gold_transaction_amount = 4000;
            goto LABEL_42;
          case 19:
            v19 = 114;
            if ( !(unsigned __int16)_449B57_test_bit(pParty->_quest_bits, v19) )
              return v35;
            if ( !gold_transaction_amount )
              goto LABEL_79;
            goto LABEL_42;
          case 20:
            v19 = 110;
            if ( !(unsigned __int16)_449B57_test_bit(pParty->_quest_bits, v19) )
              return v35;
            if ( !gold_transaction_amount )
              goto LABEL_79;
            goto LABEL_42;
          case 22:
            v20 = pPlayers[uActiveCharacter]->GetBaseWillpower();
            if ( v20 < 50 )
              return v35;
            if ( !gold_transaction_amount )
              goto LABEL_79;
            goto LABEL_42;
          case 24:
            gold_transaction_amount = 2500;
            v20 = pPlayers[uActiveCharacter]->GetBaseEndurance();
            if ( v20 < 50 )
              return v35;
            if ( !gold_transaction_amount )
              goto LABEL_79;
            goto LABEL_42;
          case 36:
            v20 = pPlayers[uActiveCharacter]->GetBaseIntelligence();
            if ( v20 < 50 )
              return v35;
            if ( !gold_transaction_amount )
              goto LABEL_79;
            goto LABEL_42;
          case 21:
          case 23:
          case 25:
          case 26:
          case 29:
          case 32:
          case 34:
          case 35:
            gold_transaction_amount = 2500;
            goto LABEL_42;
          case 8:
          case 9:
          case 10:
          case 11:
            gold_transaction_amount = 3000;
            goto LABEL_42;
          case 7:
            gold_transaction_amount = 0;
            if ( !gold_transaction_amount )
              goto LABEL_79;
            goto LABEL_42;
          default:
            if ( !gold_transaction_amount )
              goto LABEL_79;
            goto LABEL_42;
        }
        gold_transaction_amount = 0;
        if ( !gold_transaction_amount )
          goto LABEL_79;
        goto LABEL_42;
      }
    }
    else
    {
      if ( v34 != 4 )
      {
        if ( !gold_transaction_amount )
          goto LABEL_79;
        goto LABEL_42;
      }
      if ( (signed int)v16 >= 3 && v7 >= 10 )
      {
        switch ( dword_F8B1AC_award_bit_number )
        {
          case 19:
            if ( pPlayers[uActiveCharacter]->ProfessionOrGuildFlagsCorrect(0x22u, 1) == 1 )
            {
              if ( !gold_transaction_amount )
                goto LABEL_79;
              goto LABEL_42;
            }
            if ( pPlayers[uActiveCharacter]->ProfessionOrGuildFlagsCorrect(0x1Au, 1) == 1 )
            {
              if ( !gold_transaction_amount )
                goto LABEL_79;
              goto LABEL_42;
            }
            return v35;
          case 20:
            if ( pPlayers[uActiveCharacter]->ProfessionOrGuildFlagsCorrect(0x23u, 1) == 1 )
            {
              if ( !gold_transaction_amount )
                goto LABEL_79;
              goto LABEL_42;
            }
            if ( pPlayers[uActiveCharacter]->ProfessionOrGuildFlagsCorrect(0x1Bu, 1) == 1 )
            {
              if ( !gold_transaction_amount )
                goto LABEL_79;
              goto LABEL_42;
            }
            return v35;
          case 30:
            v18 = LOBYTE(pPlayers[uActiveCharacter]->pActiveSkills[31]);
            if ( (v18 & 0x3Fu) < 0xA )
              return v35;
            if ( !gold_transaction_amount )
              goto LABEL_79;
            goto LABEL_42;
          case 31:
            v18 = LOBYTE(pPlayers[uActiveCharacter]->pActiveSkills[30]);
            if ( (v18 & 0x3Fu) < 0xA )
              return v35;
            if ( !gold_transaction_amount )
              goto LABEL_79;
            goto LABEL_42;
          case 21:
          case 23:
          case 24:
          case 25:
          case 26:
          case 29:
          case 32:
          case 34:
          case 35:
            gold_transaction_amount = 6000;
            goto LABEL_42;
          case 8:
          case 9:
          case 10:
          case 11:
            gold_transaction_amount = 7000;
            goto LABEL_42;
          case 7:
            break;
          default:
            if ( !gold_transaction_amount )
              goto LABEL_79;
            goto LABEL_42;
        }
        gold_transaction_amount = 0;
        if ( !gold_transaction_amount )
          goto LABEL_79;
        goto LABEL_42;
      }
    }
    return v35;
  }
  if ( v7 < 4 )
    return v35;
  if ( dword_F8B1AC_award_bit_number > 27 )
  {
    if ( dword_F8B1AC_award_bit_number != 29
      && dword_F8B1AC_award_bit_number != 32
      && (dword_F8B1AC_award_bit_number <= 33 || dword_F8B1AC_award_bit_number > 35) )
    {
      if ( !gold_transaction_amount )
        goto LABEL_79;
      goto LABEL_42;
    }
    gold_transaction_amount = 500;
    if ( !gold_transaction_amount )
      goto LABEL_79;
    goto LABEL_42;
  }
  if ( dword_F8B1AC_award_bit_number >= 23 )
  {
    gold_transaction_amount = 500;
    if ( !gold_transaction_amount )
      goto LABEL_79;
    goto LABEL_42;
  }
  if ( dword_F8B1AC_award_bit_number == 7 )
  {
    gold_transaction_amount = 0;
    goto LABEL_79;
  }
  if ( dword_F8B1AC_award_bit_number <= 7 )
  {
    if ( !gold_transaction_amount )
      goto LABEL_79;
    goto LABEL_42;
  }
  if ( dword_F8B1AC_award_bit_number > 18 )
  {
    if ( dword_F8B1AC_award_bit_number != 21 )
    {
      if ( !gold_transaction_amount )
        goto LABEL_79;
      goto LABEL_42;
    }
    gold_transaction_amount = 500;
    if ( !gold_transaction_amount )
      goto LABEL_79;
    goto LABEL_42;
  }
  gold_transaction_amount = 1000;
LABEL_42:
  if ( gold_transaction_amount > pParty->uNumGold )
    return (char *)pNPCTopics[124].pText;
LABEL_79:
  contract_approved = 1;
  if ( v34 == 2 )
  {
    sprintfex(pTmpBuf2.data(), pGlobalTXT_LocalizationStrings[534],//Получить степень ^Pr[%s] в навыке ^Pr[%s] за ^I[%lu] золот^L[ой;ых;ых]
              pGlobalTXT_LocalizationStrings[433], pSkillNames[dword_F8B1AC_award_bit_number], gold_transaction_amount);//Эксперт
    return pTmpBuf2.data();
  }
  if ( v34 == 3 )
  {
    sprintfex(pTmpBuf2.data(), pGlobalTXT_LocalizationStrings[534],
              pGlobalTXT_LocalizationStrings[432], pSkillNames[dword_F8B1AC_award_bit_number], gold_transaction_amount);//Мастер
    return pTmpBuf2.data();
  }
  if ( v34 == 4 )
    sprintfex(pTmpBuf2.data(), pGlobalTXT_LocalizationStrings[534],
              pGlobalTXT_LocalizationStrings[225], pSkillNames[dword_F8B1AC_award_bit_number], gold_transaction_amount);//Великий Магистр
  return pTmpBuf2.data();
}

//----- (004B3E1E) --------------------------------------------------------
void  sub_4B3E1E()
{
  NPCData *v0; // ST40_4@1
  signed int v1; // edi@1
  //GUIWindow *v2; // ecx@1

  __debugbreak();
  v0 = GetNPCData(sDialogue_SpeakingActorNPC_ID);
  v1 = 0;
  pDialogueWindow->eWindowType = WINDOW_MainMenu;
  pDialogueWindow->Release();
  pDialogueWindow = GUIWindow::Create(0, 0, 640, 480, WINDOW_Dialogue, 1, 0);
  if ( pNPCStats->pProfessions[v0->uProfession].pBenefits)//*(&pNPCStats->field_13A5C + 5 * v0->uProfession) )
  {
    pDialogueWindow->CreateButton(480, 160, 140, 28, 1, 0, UIMSG_SelectNPCDialogueOption, 77, 0, pGlobalTXT_LocalizationStrings[407], 0);
    v1 = 1;
  }
  pDialogueWindow->CreateButton(480, 30 * v1 + 160, 140, 30, 1, 0, UIMSG_SelectNPCDialogueOption, 76, 0, pGlobalTXT_LocalizationStrings[406], 0);//Нанять
  pDialogueWindow->_41D08F_set_keyboard_control_group(v1 + 1, 1, 0, 1);
}


//----- (004B3FE5) --------------------------------------------------------
void __fastcall _4B3FE5_training_dialogue(int a4)
{
  int v1; // edi@1
  const char *v2; // edi@1
  
  __debugbreak();
  v1 = a4;
  uDialogueType = 78;
  current_npc_text = (char *)pNPCTopics[a4 + 168].pText;
  _4B254D_SkillMasteryTeacher(a4);
  pDialogueWindow->Release();
  pDialogueWindow = GUIWindow::Create(0, 0, 640, 0x15Eu, WINDOW_MainMenu, v1, 0);
  v2 = "";
  pBtn_ExitCancel = pDialogueWindow->CreateButton( 0x1D7u, 0x1BDu, 0xA9u,  0x23u,   1,  0, UIMSG_Escape, 0,   0,
                 pGlobalTXT_LocalizationStrings[34], pIcons_LOD->GetTexture(uExitCancelTextureId), 0);
  pDialogueWindow->CreateButton(0, 0, 0, 0, 1, 0, UIMSG_BuyInShop_Identify_Repair, 0, 0, "", 0);
  if ( contract_approved )
    v2 = pGlobalTXT_LocalizationStrings[535];
  pDialogueWindow->CreateButton(0x1E0u, 0xA0u, 0x8Cu, 0x1Eu, 1, 0, UIMSG_ClickNPCTopic, 0x4Fu, 0, v2, 0);
  pDialogueWindow->_41D08F_set_keyboard_control_group(1, 1, 0, 2);
  dialog_menu_id = HOUSE_DIALOGUE_OTHER;
}
// F8B19C: using guessed type int dword_F8B19C;
// F8B1A8: using guessed type int dword_F8B1A8;

//----- (004B46A5) --------------------------------------------------------
void __fastcall DrawTextAtStatusBar( const char *Str, int a5 )
{
  int v4; // eax@1
  pRenderer->DrawTextureRGB(0, 352, pTexture_StatusBar);
  v4 = pFontLucida->AlignText_Center(450, Str);
  pPrimaryWindow->DrawText(pFontLucida, v4 + 11, 357, a5, Str, 0, 0, 0);
}

//----- (004B46F8) --------------------------------------------------------
__int64 GetExperienceRequiredForLevel(int level)
{
  __int64 v1; // eax@1
  int i; // edx@1

  v1 = 0;
  for ( i = 0; i < level; ++i )
    v1 += i + 1;
  return 1000 * v1;
}

//----- (004BC49B) --------------------------------------------------------
void OnSelectNPCDialogueOption(DIALOGUE_TYPE newDialogueType)
{
	//unsigned int v1; // esi@1
    NPCData *speakingNPC; // ebp@1
    //unsigned int v3; // eax@1
    int npc_event_id; // ecx@10
    signed int v5; // edi@14
    char *v6; // esi@15
    const char *v7; // ecx@22
    signed int v8; // edi@37
    //unsigned int v9; // eax@56
    unsigned int v10; // ecx@57
    void *v11; // [sp-Ch] [bp-1Ch]@46
    int v12; // [sp-8h] [bp-18h]@46
    char *v13; // [sp-8h] [bp-18h]@60
    size_t v14; // [sp-4h] [bp-14h]@46
    const char *v15; // [sp-4h] [bp-14h]@60

    //v1 = _this;
	speakingNPC = GetNPCData(sDialogue_SpeakingActorNPC_ID);
    //v3 = v1;
	uDialogueType = newDialogueType;
	if (!speakingNPC->uFlags)
	{
		speakingNPC->uFlags = 1;
		//v3 = uDialogueType;
	}

	if(newDialogueType == DIALOGUE_PROFESSION_DETAILS)
	{
		dialogue_show_profession_details = ~dialogue_show_profession_details;
	}
	else if(newDialogueType == DIALOGUE_76)
	{
		if (speakingNPC->Hired())
		{
			v8 = 0;
			if ( (signed int)pNPCStats->uNumNewNPCs > 0 )
			{
				v6 = (char *)pNPCStats->pNewNPCData;
				while ( !(v6[8] & 0x80) || strcmp(speakingNPC->pName, *(const char **)v6) )
				{
					++v8;
					v6 += 76;
					if ( v8 >= (signed int)pNPCStats->uNumNewNPCs )
						break;
				}
				if( v8 < (signed int)pNPCStats->uNumNewNPCs )
					v6[8] &= 0x7Fu;
			}
			if ( pParty->pHirelings[0].pName && !_stricmp(pParty->pHirelings[0].pName, speakingNPC->pName) )
			{
				v11 = pParty->pHirelings.data();
				memset(v11, 0, sizeof(NPCData));
			}
			else if ( pParty->pHirelings[1].pName && !_stricmp(pParty->pHirelings[1].pName, speakingNPC->pName) )
			{
				v11 = &pParty->pHirelings[1];
				memset(v11, 0, sizeof(NPCData));
			}
			pParty->hirelingScrollPosition = 0;
			pParty->CountHirelings();
			dword_591084 = 0;
			pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, 0);
			dword_7241C8 = 0;
			return;
		}
		if ( pParty->pHirelings[0].pName && pParty->pHirelings[1].pName )
		{
			v7 = pGlobalTXT_LocalizationStrings[533]; // ""I cannot join you, you're party is full""
			ShowStatusBarString(v7, 2u);
		}
		else
		{
			//v9 = v2->uProfession;
			if ( speakingNPC->uProfession != 51 )
			{
				v10 = pNPCStats->pProfessions[speakingNPC->uProfession - 1].uHirePrice;
																	if ( pParty->uNumGold < v10 )
			{
				ShowStatusBarString(pGlobalTXT_LocalizationStrings[155], 2u);// "You don't have enough gold"
				dialogue_show_profession_details = false;
				uDialogueType = 13;
				if ( uActiveCharacter )
					pPlayers[uActiveCharacter]->PlaySound(SPEECH_NotEnoughGold, 0);
				v7 = pGlobalTXT_LocalizationStrings[155];
				ShowStatusBarString(pGlobalTXT_LocalizationStrings[155], 2u);
				if ( !dword_7241C8 )
					pGame->Draw();
				dword_7241C8 = 0;
				return;
			}
				Party::TakeGold(v10);
			}
			LOBYTE(speakingNPC->uFlags) |= 0x80u;
			if ( pParty->pHirelings[0].pName )
			{
				memcpy(&pParty->pHirelings[1], speakingNPC, sizeof(pParty->pHirelings[1]));
				v15 = speakingNPC->pName;
				v13 = pParty->pHireling2Name;
			}
			else
			{
				memcpy(pParty->pHirelings.data(), speakingNPC, 0x4Cu);
				v15 = speakingNPC->pName;
				v13 = pParty->pHireling1Name;
			}
			strcpy(v13, v15);
			pParty->hirelingScrollPosition = 0;
			pParty->CountHirelings();

			pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, 0);

			if ( sDialogue_SpeakingActorNPC_ID >= 0 )
				pDialogue_SpeakingActor->uAIState = Removed;
			if ( uActiveCharacter )
				pPlayers[uActiveCharacter]->PlaySound(SPEECH_61, 0);
		}
	}
	else if ( (signed int)newDialogueType > 84 && (signed int)newDialogueType <= 88 )
	{
		ArenaFight();
		return;
	}
	else if(newDialogueType == DIALOGUE_USE_NPC_ABILITY)
	{
		if (UseNPCSkill((NPCProf)speakingNPC->uProfession) == 0)
		{
			if ( speakingNPC->uProfession != GateMaster )
				speakingNPC->bHasUsedTheAbility = 1;

			pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, 0);
		}
		else
			ShowStatusBarString(pGlobalTXT_LocalizationStrings[140], 2); //"Your packs are already full!"
	}
	else if(newDialogueType == DIALOGUE_13)
	{
		if (!speakingNPC->Hired())
		{
			sub_4B3E1E();
			dialogue_show_profession_details = false;
		}
		else
		{
			v5 = 0;
			if ( (signed int)pNPCStats->uNumNewNPCs > 0 )
			{
				v6 = (char *)pNPCStats->pNewNPCData;
				while ( !(v6[8] & 0x80) || strcmp(speakingNPC->pName, *(const char **)v6) )
				{
					++v5;
					v6 += 76;
					if ( v5 >= (signed int)pNPCStats->uNumNewNPCs )
						break;
				}
				if ( v5 < (signed int)pNPCStats->uNumNewNPCs )
					v6[8] &= 0x7Fu;
			}
			if ( pParty->pHirelings[0].pName && !_stricmp(pParty->pHirelings[0].pName, speakingNPC->pName) )
			{
				v11 = pParty->pHirelings.data();
				memset(v11, 0, sizeof(NPCData));
			}
			else if ( pParty->pHirelings[1].pName && !_stricmp(pParty->pHirelings[1].pName, speakingNPC->pName) )
			{
				v11 = &pParty->pHirelings[1];
				memset(v11, 0, sizeof(NPCData));
			}
			pParty->hirelingScrollPosition = 0;
			pParty->CountHirelings();
			dword_591084 = 0;
			pMessageQueue_50CBD0->AddMessage(UIMSG_Escape, 1, 0);
			dword_7241C8 = 0;
			return;
		}
	}
	else if(newDialogueType >= 19 && newDialogueType <= 24)
	{
		switch(newDialogueType)
		{
			case DIALOGUE_19:  npc_event_id = speakingNPC->evt_A; break;
			case DIALOGUE_20:  npc_event_id = speakingNPC->evt_B; break;
			case DIALOGUE_21:  npc_event_id = speakingNPC->evt_C; break;
			case DIALOGUE_22:	 npc_event_id = speakingNPC->evt_D; break;
			case DIALOGUE_23:  npc_event_id = speakingNPC->evt_E; break;
			case DIALOGUE_24:	 npc_event_id = speakingNPC->evt_F; break;
		}
		if ( (npc_event_id >= 200) && (npc_event_id <= 310) )
			_4B3FE5_training_dialogue(npc_event_id); //200-310
		else if (( npc_event_id >= 400) && (npc_event_id <= 410) )
		{ //400-410
			dword_F8B1D8 = newDialogueType;
			DrawJoinGuildWindow(npc_event_id - 400);
		}
		else
		{
			switch ( npc_event_id )
			{
				case 139:
					OracleDialogue();
					break;
				case 311:
					CheckBountyRespawnAndAward();
					break;
				case 399:
					Arena_SelectionFightLevel();
					break;
				default:
					activeLevelDecoration = (LevelDecoration*)1;
					current_npc_text = 0;
					EventProcessor(npc_event_id, 0, 1);
					activeLevelDecoration = NULL;
					break;
			}
		}
	}
	if ( !dword_7241C8 )
		pGame->Draw();
	dword_7241C8 = 0;
}

//----- (004BDAAF) --------------------------------------------------------
bool __fastcall MerchandiseTest(ItemGen *item, int _2da_idx)
{
  int v6; // edx@8
  int v7; // edx@9
  int v8; // edx@10
  unsigned __int8 v9; // zf@16
  char v10; // sf@16
  unsigned __int8 v11; // of@16
  bool test;

  if ( (p2DEvents[_2da_idx - 1].uType != 4 || (signed int)item->uItemID < 740 || (signed int)item->uItemID > 771)
    && ((signed int)item->uItemID >= 600 || (signed int)item->uItemID >= 529 && (signed int)item->uItemID <= 599) || item->IsStolen())
    return 0;
  switch( p2DEvents[_2da_idx - 1].uType )
  {
    case BuildingType_WeaponShop:
    {
      test = item->GetItemEquipType() <= 2;
      break;
    }
    case BuildingType_ArmorShop:
    {
      test = item->GetItemEquipType() >= 3;
      break;
    }
    case BuildingType_MagicShop:
    {
      test = item->GetPlayerSkillType() == 38 || item->GetItemEquipType() == 16;
      break;
    }
    case BuildingType_AlchemistShop:
    {
      test = item->GetItemEquipType() == 13 || item->GetItemEquipType() == 14 
            || (item->GetItemEquipType() > 14 && !(item->GetItemEquipType() != 17 
            || (signed int)item->uItemID < 740) && item->uItemID != 771);
      break;
    }
    default:
    {
      test = false;
      break;
    }
  }
  return test;
/*
  if ( p2DEvents[a2 - 1].uType == 1 )
  {
    v11 = __OFSUB__(v5, 2);
    v9 = v5 == 2;
    v10 = v5 - 2 < 0;
    goto LABEL_23;
  }
  if ( p2DEvents[a2 - 1].uType > 2 )
  {
    if ( p2DEvents[a2 - 1].uType == 3 )
    {
      if ( pItemsTable->pItems[v4].uSkillType != 38 )
        return v5 == 16;
      return 1;
    }
    if ( p2DEvents[a2 - 1].uType != 4 || v5 < 13 )
      return 0;
	if ( p2DEvents[a2 - 1].uType == 4)
	{
		if ( v5 < 13 )
		  return 0;
		if ( v5 <= 14 )
		  return 1;
		if ( v5 != 17 || (signed int)v3 < 740 )
		  return 0;
		v11 = __OFSUB__(v3, 771);
		v9 = v3 == 771;
		v10 = ((v3 - 771) & 0x80000000u) != 0;
LABEL_23:
		if ( !((unsigned __int8)(v10 ^ v11) | v9) )
		  return 0;
		return 1;
	}
  }
  if ( p2DEvents[a2 - 1].uType == 2 )
  {
	  if ( v5 >= 3 )
	  {
		v11 = __OFSUB__(v5, 9);
		v9 = v5 == 9;
		v10 = v5 - 9 < 0;
		goto LABEL_23;
	  }
  }
  return 0;
*/
}



