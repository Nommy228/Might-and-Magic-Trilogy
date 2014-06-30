#define _CRT_SECURE_NO_WARNINGS

#include "Vis.h"
#include "Game.h"
#include "MM7_data.h"
#include "Actor.h"







//----- (00427546) --------------------------------------------------------
int stru319::_427546(int a2)
{
  int result; // eax@2

  if ( a2 >= 0 )
  {
    if ( a2 >= 1 )
      result = (a2 >= 2) + 2;
    else
      result = 1;
  }
  else
  {
    result = 0;
  }
  return result;
}



//----- (0042F184) --------------------------------------------------------
int stru319::FindClosestActor(int pick_depth, int a3, int a4)
{
  int v4; // edi@1
  stru319 *v5; // esi@1
  int v6; // eax@2
  int v7; // eax@4
//  int result; // eax@5
//  int *v9; // edx@8
//  signed int v10; // ebx@10
//  int v11; // edi@11
  //Actor *v12; // esi@12
  //unsigned __int16 v13; // ax@12
//  int v14; // eax@22
  //char v15; // zf@30
//  int v16; // esi@32
//  int v17; // ecx@34
//  stru319 *v18; // eax@39
//  int v19; // edx@39
//  int v20; // ecx@41
//  unsigned __int16 v21; // ax@42
//  unsigned int v22; // [sp+8h] [bp-24h]@11
  //unsigned int v23; // [sp+Ch] [bp-20h]@7
  stru319 *v24; // [sp+10h] [bp-1Ch]@1
//  unsigned int v25; // [sp+14h] [bp-18h]@8
//  int *v26; // [sp+18h] [bp-14h]@8
//  int v27; // [sp+1Ch] [bp-10h]@10
//  int *v28; // [sp+20h] [bp-Ch]@10
  //unsigned int v29; // [sp+24h] [bp-8h]@7
//  int v30; // [sp+28h] [bp-4h]@6
//  int i; // [sp+38h] [bp+Ch]@33
//  signed int v32; // [sp+3Ch] [bp+10h]@32

  v4 = 0;
  v5 = this;
  v24 = this;
  //if ( pRenderer->pRenderD3D )
  {
    v6 = a3 != 0;
    if ( a4 )
      LOBYTE(v6) = v6 | 8;
    v7 = pGame->pVisInstance->PickClosestActor(OBJECT_Actor, pick_depth, v6, 657456, -1);
    if ( v7 != -1 )
      return (unsigned __int16)v7;
    else return 0;
  }
  /*else // software impl
  {
    v30 = 0;
    if ( pRenderer->pActiveZBuffer )
    {
      if ( (signed int)viewparams->uScreen_topL_Y < (signed int)viewparams->uScreen_BttmR_Y )
      {
        v9 = &pRenderer->pActiveZBuffer[viewparams->uScreen_topL_X + 640 * viewparams->uScreen_topL_Y];
        v26 = &pRenderer->pActiveZBuffer[viewparams->uScreen_topL_X + 640 * viewparams->uScreen_topL_Y];
        for ( v25 = viewparams->uScreen_BttmR_Y - viewparams->uScreen_topL_Y; v25; --v25 )
        {
          if ( (signed int)viewparams->uScreen_topL_X < (signed int)viewparams->uScreen_BttmR_X )
          {
            v28 = v9;
            v10 = v4;
            for ( v27 = viewparams->uScreen_BttmR_X - viewparams->uScreen_topL_X; v27; --v27 )
            {
              v22 = *v28;
              v11 = *v28 & 0xFFFF;
              if (PID_TYPE(v11) == OBJECT_Actor)
              {
                if ( pActors[PID_ID(v11)].uAIState != Dead )
                {
                  if ( pActors[PID_ID(v11)].uAIState != Dying && pActors[PID_ID(v11)].uAIState != Removed
                    && pActors[PID_ID(v11)].uAIState != Summoned && pActors[PID_ID(v11)].uAIState != Disabled
                    && (!a3 || pActors[PID_ID(v11)].GetActorsRelation(0)) )
                  {
                    if ( (!a4 || MonsterStats::BelongsToSupertype(pActors[PID_ID(v11)].pMonsterInfo.uID, MONSTER_SUPERTYPE_UNDEAD))
                      && v22 <= pick_depth << 16 )
                    {
                      v14 = 0;
                      if ( v10 > 0 )
                      {
                        for ( v14; v14 < v30; ++v14 )
                        {
                          if ( dword_50BDA0[v14] == v11 )
                            break;
                        }
                      }
                      if ( v14 == v30 && v10 < 100 )
                      {
                        ++v30;
                        dword_50BC10[v10] = v22;
                        dword_50BDA0[v10] = v11;
                        ++v10;
                      }
                    }
                  }
                }
              }
              ++v28;
            }
            v4 = v30;
            v5 = v24;
          }
          v9 = v26 + 640;
          v26 += 640;
        }
      }
      if ( v4 > 0 )
      {
        v16 = (int)dword_50BC10.data();
        for ( v32 = 1; v32 - 1 < v4; ++v32 )
        {
          for ( i = v32; i < v4; ++i )
          {
            v17 = dword_50BC10[i];
            if ( dword_50BC10[i] < *(int *)v16 )
            {
              dword_50BC10[i] = *(int *)v16;
              *(int *)v16 = v17;
            }
          }
          v16 += 4;
        }
        v5 = v24;
        if ( v4 > 0 )
        {
          v18 = v24;
          for ( v19 = v4; v19; --v19 )
          {
            *(int *)&v18->field_0 = (*(int *)&v18[(char *)dword_50BC10.data() - (char *)v24].field_0 >> 3) & 0x1FFF;
            v18 += 4;
          }
        }
      }
      v20 = 0;
      for ( *(int *)&v5[2000].field_0 = v4; v20 < v4; ++v20 )
      {
        v21 = pActors[*(int *)&v5[4 * v20].field_0].uAIState;
        if ( v21 != 4 && v21 != 5 )
          break;
      }
      if ( v20 != v4 )
      {
        result = 8 * *(int *)&v5[4 * v20].field_0;
        LOBYTE(result) = result | 3;
        return result;
      }
    }
  }
  return 0;*/
}



