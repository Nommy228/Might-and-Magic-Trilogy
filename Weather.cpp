#include <stdlib.h>

#include "Weather.h"
#include "Viewport.h"
#include "Render.h"

#include "mm7_data.h"   ////TODO: remove this once LOWORD/HIWRD stuff is refactored




struct Weather *pWeather = new Weather;



//----- (004C2AA6) --------------------------------------------------------
int Weather::DrawSnow()
{
  unsigned __int16 *v1; // edi@1
  __int16 *v2; // esi@2
  int v3; // eax@2
  signed int v4; // eax@6
  int v5; // edx@8
  __int16 *v6; // esi@11
  __int16 *v7; // eax@11
  int v8; // ecx@11
  int v9; // eax@12
  int v10; // ecx@16
  int v11; // edx@18
  Weather *v12; // eax@20
  __int16 *v13; // esi@20
  int v14; // eax@21
  int v15; // eax@21
  int v16; // edx@23
  int v17; // eax@24
  int v18; // edx@26
  int v19; // ecx@27
  int v20; // eax@27
  char v21; // zf@27
  unsigned int v23; // [sp+Ch] [bp-20h]@1
  unsigned int v24; // [sp+10h] [bp-1Ch]@1
  unsigned int v25; // [sp+14h] [bp-18h]@1
  Weather *v26; // [sp+18h] [bp-14h]@1
  signed int v27; // [sp+18h] [bp-14h]@20
  unsigned int v28; // [sp+1Ch] [bp-10h]@1
  unsigned int v29; // [sp+20h] [bp-Ch]@1
  __int16 *v30; // [sp+24h] [bp-8h]@2
  signed int v31; // [sp+28h] [bp-4h]@1
  signed int v32; // [sp+28h] [bp-4h]@10

  v26 = this;
  v25 = viewparams->uScreen_BttmR_X;
  v28 = viewparams->uScreen_topL_Y;
  v31 = 0;
  v1 = pRenderer->pTargetSurface;
  v29 = viewparams->uScreen_topL_X;
  v23 = viewparams->uScreen_BttmR_Y;
  v24 = viewparams->uScreen_BttmR_X - viewparams->uScreen_topL_X;
  do
  {
    v2 = &v26->field_0[2 * v31];
    v30 = &v26->field_0[2 * v31 + 1];
    *v2 += rand() % 3 + 1;
    *v30 += rand() % 2 + 1;
    v3 = *v2;
    if ( v3 < (signed int)(v25 - 1) )
    {
      if ( v3 < (signed int)v29 )
        *v2 = v25 - rand() % 8;
    }
    else
    {
      *v2 = v29 + rand() % 8;
    }
    v4 = *v30;
    if ( v4 < (signed int)v28 || v4 >= (signed int)v23 )
    {
      v5 = rand() % (signed int)(v24 - 2);
      *v30 = v28;
      *v2 = v29 + v5;
    }
    ++v31;
    v1[*v2 + 640 * *v30] = 0xFFFFu;
  }
  while ( v31 < 700 );
  v32 = 700;
  do
  {
    v6 = &v26->field_0[2 * v32];
    *v6 += rand() % 5 - 3;
    v7 = &v26->field_0[2 * v32 + 1];
    v8 = *v6;
    *v7 += 4;
    if ( v8 < (signed int)v29 )
    {
      LOWORD(v9) = v25 - rand() % 4 - 2;
LABEL_15:
      *v6 = v9;
      v7 = &v26->field_0[2 * v32 + 1];
      goto LABEL_16;
    }
    if ( v8 >= (signed int)(v25 - 2) )
    {
      v9 = rand() % 4 + v29 + 2;
      goto LABEL_15;
    }
LABEL_16:
    v10 = *v7;
    if ( v10 < (signed int)v28 || v10 >= (signed int)(v23 - 1) )
    {
      v11 = rand() % (signed int)(v24 - 2);
      v26->field_0[2 * v32 + 1] = v28;
      v7 = &v26->field_0[2 * v32 + 1];
      *v6 = v29 + v11;
    }
    *(&v1[640 * *v7 + 1] + *v6) = 0xFFFFu;
    *(&v1[640 * *v7 + 641] + *v6) = 0xFFFFu;
    v1[*v6 + 640 * (*v7 + 1)] = 0xFFFFu;
    ++v32;
    v1[*v6 + 640 * *v7] = 0xFFFFu;
  }
  while ( v32 < 950 );
  v12 = v26;
  v27 = 50;
  v13 = &v12->field_0[1901];
  do
  {
    v14 = rand();
    *v13 += 8;
    *(v13 - 1) += v14 % 11 - 5;
    v15 = *(v13 - 1);
    if ( v15 < (signed int)v29 || v15 >= (signed int)(v25 - 5) )
    {
      v16 = rand() % (signed int)(v24 - 5);
      *v13 = v28;
      *(v13 - 1) = v29 + v16;
    }
    v17 = *v13;
    if ( v17 < (signed int)v28 || v17 >= (signed int)(v23 - 5) )
    {
      v18 = rand() % (signed int)(v24 - 5);
      *v13 = v28;
      *(v13 - 1) = v29 + v18;
    }
    *(&v1[640 * *v13 + 1] + *(v13 - 1)) = 0xFFFFu;
    *(&v1[640 * *v13 + 641] + *(v13 - 1)) = 0xFFFFu;
    v1[*(v13 - 1) + 640 * (*v13 + 1)] = 0xFFFFu;
    v1[*(v13 - 1) + 640 * *v13] = 0xFFFFu;
    *(&v1[640 * *v13 + 1281] + *(v13 - 1)) = 0xFFFFu;
    *(&v1[640 * *v13 + 1921] + *(v13 - 1)) = 0xFFFFu;
    v1[*(v13 - 1) + 640 * (*v13 + 3)] = 0xFFFFu;
    v1[*(v13 - 1) + 640 * (*v13 + 2)] = 0xFFFFu;
    *(&v1[640 * *v13 + 3] + *(v13 - 1)) = 0xFFFFu;
    *(&v1[640 * *v13 + 643] + *(v13 - 1)) = 0xFFFFu;
    *(&v1[640 * *v13 + 642] + *(v13 - 1)) = 0xFFFFu;
    *(&v1[640 * *v13 + 2] + *(v13 - 1)) = 0xFFFFu;
    *(&v1[640 * *v13 + 1283] + *(v13 - 1)) = 0xFFFFu;
    *(&v1[640 * *v13 + 1923] + *(v13 - 1)) = 0xFFFFu;
    *(&v1[640 * *v13 + 1922] + *(v13 - 1)) = 0xFFFFu;
    v19 = *(v13 - 1);
    v20 = 5 * *v13;
    v13 += 2;
    v21 = v27-- == 1;
    *(&v1[128 * v20 + 1282] + v19) = 0xFFFFu;
  }
  while ( !v21 );
  return 0;
}

//----- (004C2EA0) --------------------------------------------------------
int Weather::Initialize()
{
  Weather *v1; // edi@1
  signed int v2; // esi@1
  signed int v3; // ebx@1
  signed int v4; // ebp@1

  v1 = this;
  v2 = 0;
  v3 = pViewport->uScreen_BR_X - pViewport->uScreen_TL_X - 4;
  v4 = pViewport->uScreen_BR_Y - pViewport->uScreen_TL_Y - 4;
  do
  {
    v1->field_0[2 * v2++] = LOWORD(pViewport->uViewportTL_X) + rand() % v3;
    *((short *)v1 + 2 * v2 - 1) = LOWORD(pViewport->uViewportTL_Y) + rand() % v4;
  }
  while ( v2 < 1000 );
  return 0;
}

//----- (004C2EFA) --------------------------------------------------------
int Weather::Draw()
{
  if (bRenderSnow)
    DrawSnow();
  return 0;
}

//----- (004C2F0B) --------------------------------------------------------
bool Weather::OnPlayerTurn(__int16 a2)
{
  int v3; // edx@3
  signed int v4; // ebx@3
  unsigned int screen_width; // esi@3
  __int16 *v6; // eax@5
  signed int v7; // edi@5
  unsigned int screen_x; // [sp+0h] [bp-Ch]@3
  unsigned int screen_z_minus_4; // [sp+4h] [bp-8h]@3
  Weather *v10; // [sp+8h] [bp-4h]@1

  v10 = this;
  if ( this->bRenderSnow != 1 )
    return 0;
  HIWORD(v3) = HIWORD(viewparams->uScreen_topL_X);
  v4 = 0;
  screen_width = viewparams->uScreen_BttmR_X - viewparams->uScreen_topL_X;
  screen_x = viewparams->uScreen_topL_X;
  screen_z_minus_4 = viewparams->uScreen_BttmR_X - 4;

  short* _this = this->field_0;
  while ( 1 )
  {
    v6 = &_this[2 * v4];
    *v6 += a2;
    LOWORD(v3) = *v6;
    v7 = *v6;
    if ( v7 < (signed int)screen_z_minus_4 )
    {
      if ( v7 >= (signed int)screen_x )
        goto LABEL_10;
      v3 += screen_width;
    }
    else
    {
      v3 = v3 - screen_width + 4;
    }
    *v6 = v3;
LABEL_10:
    ++v4;
    if ( v4 >= 1000 )
      return 1;
    //_this = v10;
  }
}