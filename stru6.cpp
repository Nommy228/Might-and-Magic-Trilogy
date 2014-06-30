#define _CRT_SECURE_NO_WARNINGS
#include "stru6.h"

#include "mm7_data.h"
#include "LightmapBuilder.h"
#include "SpriteObject.h"
#include "IndoorCameraD3D.h"
#include "ParticleEngine.h"
#include "Game.h"
#include "LOD.h"
#include "Sprites.h"
#include "DecalBuilder.h"
#include "ObjectList.h"
#include "Actor.h"
#include "Random.h"
#include "Spells.h"
#include "IconFrameTable.h"
#include "Timer.h"
#include "Viewport.h"
#include "PaletteManager.h"
#include "Overlays.h"
#include "stru160.h"
#include "OurMath.h"
#include "Lights.h"

#include "MM7.h"





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


//----- (0042620A) --------------------------------------------------------
bool sr_42620A(RenderVertexSoft *p)
{
//  __int16 v1; // fps@1
  unsigned __int8 v2; // c0@2
  char v3; // c2@2
  unsigned __int8 v4; // c3@2
//  bool result; // eax@2
  unsigned __int8 v6; // c0@4
  char v7; // c2@4
  unsigned __int8 v8; // c3@4
  unsigned __int8 v9; // c0@6
  char v10; // c2@6
  unsigned __int8 v11; // c3@6
  double v12; // st6@7
  float v13; // ST04_4@7
  float v14; // ST00_4@7
  double v15; // st7@7
  double v16; // st6@8
  float v17; // ST04_4@8
  float v18; // ST00_4@8
  double v19; // st7@8

  //UNDEF(v1);
  if ( p->vWorldViewPosition.x < 300.0
    || (v2 = 300.0 < p[1].vWorldViewPosition.x,
        v3 = 0,
        v4 = 300.0 == p[1].vWorldViewPosition.x,
        //BYTE1(result) = HIBYTE(v1),
        !(v2 | v4)) )
  {
    if ( p->vWorldViewPosition.x < 300.0 )
    {
      v6 = 300.0 < p[1].vWorldViewPosition.x;
      v7 = 0;
      v8 = 300.0 == p[1].vWorldViewPosition.x;
      //BYTE1(result) = HIBYTE(v1);
      if ( !(v6 | v8) )
      {
        //LOBYTE(result) = 0;
        return false;
      }
    }
    v9 = 300.0 < p->vWorldViewPosition.x;
    v10 = 0;
    v11 = 300.0 == p->vWorldViewPosition.x;
    //BYTE1(result) = HIBYTE(v1);
    if ( v9 | v11 )
    {
      v16 = 1.0 / (p->vWorldViewPosition.x - p[1].vWorldViewPosition.x);
      v17 = (p->vWorldViewPosition.y - p[1].vWorldViewPosition.y) * v16;
      v18 = (p->vWorldViewPosition.z - p[1].vWorldViewPosition.z) * v16;
      v19 = 300.0 - p[1].vWorldViewPosition.x;
      p[1].vWorldViewPosition.x = v19 + p[1].vWorldViewPosition.x;
      p[1].vWorldViewPosition.y = v17 * v19 + p[1].vWorldViewPosition.y;
      p[1].vWorldViewPosition.z = v19 * v18 + p[1].vWorldViewPosition.z;
    }
    else
    {
      v12 = 1.0 / (p[1].vWorldViewPosition.x - p->vWorldViewPosition.x);
      v13 = (p[1].vWorldViewPosition.y - p->vWorldViewPosition.y) * v12;
      v14 = (p[1].vWorldViewPosition.z - p->vWorldViewPosition.z) * v12;
      v15 = 300.0 - p->vWorldViewPosition.x;
      p->vWorldViewPosition.x = v15 + p->vWorldViewPosition.x;
      p->vWorldViewPosition.y = v13 * v15 + p->vWorldViewPosition.y;
      p->vWorldViewPosition.z = v15 * v14 + p->vWorldViewPosition.z;
    }
  }
  //LOBYTE(result) = 1;
  return true;
}



//----- (004775C4) --------------------------------------------------------
stru6_stru1_indoor_sw_billboard::~stru6_stru1_indoor_sw_billboard()
{
    delete [] pArray1;
    pArray1 = nullptr;

    delete [] pArray2;
    pArray2 = nullptr;
}


//----- (00478211) --------------------------------------------------------
void stru6_stru1_indoor_sw_billboard::Initialize(int a2)
{
  uNumVec4sInArray1 = 66;
  pArray1 = new stru16x[66];

  uNumVec3sInArray2 = 128;
  pArray2 = new stru160[128];

  for (uint i = 0; i < uNumVec4sInArray1; ++i)
  {
    pArray1[i].field_0 = array_4EB8B8[i].field_0;
    pArray1[i].field_4 = array_4EB8B8[i].field_4;
    pArray1[i].field_8 = array_4EB8B8[i].field_8;
    pArray1[i].field_C = a2;
  }
  memcpy(pArray2, array_4EBBD0_x.data()/*array_4EBBD0*/, uNumVec3sInArray2 * sizeof(stru160));
}

//----- (0047829F) --------------------------------------------------------
void stru6_stru1_indoor_sw_billboard::_47829F_sphere_particle(float x_offset, float y_offset, float z_offset, float scale, int diffuse)
{
  int v7 = 0;

  for (unsigned int i = 0; i < uNumVec3sInArray2; ++i)
  {
    for (unsigned int j = 0; j < 3; ++j)
    {
      field_14[j].x = x_offset + scale * *(&pArray1->field_0 + 4 * *(int *)((char *)&pArray2->field_0 + v7));
      field_14[j].y = y_offset + scale * *(&pArray1->field_4 + 4 * *(int *)((char *)&pArray2->field_0 + v7));
      field_14[j].z = z_offset + scale * *(&pArray1->field_8 + 4 * *(int *)((char *)&pArray2->field_0 + v7));
      //int v10 = *(int *)((char *)&pArray2->field_0 + v7);

      field_14[j].diffuse = *((int *)&pArray1[1].field_0 + 4 * (*(int *)((char *)&pArray2->field_0 + v7)));
      v7 += 4;
    }

    uNumVertices = 3;
    if ( sub_477C61() && sub_477F63() )
    {
      if ( sub_47802A() )
        pRenderer->_4A4CC9_AddSomeBillboard(this, diffuse);
    }
  }
}




//----- (004A71FE) --------------------------------------------------------
void stru6::DoAddProjectile(float srcX, float srcY, float srcZ, float dstX, float dstY, float dstZ, unsigned int uTextureID)
{
  //int v8; // eax@1

  //v8 = uNumProjectiles;
  if (uNumProjectiles < 32)
  {
    pProjectiles[uNumProjectiles].srcX = srcX;
    pProjectiles[uNumProjectiles].srcY = srcY;
    pProjectiles[uNumProjectiles].srcZ = srcZ;
    pProjectiles[uNumProjectiles].dstX = dstX;
    pProjectiles[uNumProjectiles].dstY = dstY;
    pProjectiles[uNumProjectiles].dstZ = dstZ;
    pProjectiles[uNumProjectiles++].uTextureID = uTextureID;
  }
}

//----- (004A7298) --------------------------------------------------------
void stru6::DrawProjectiles()
{
  float v10; // ST1C_4@8
  float v11; // ST0C_4@8
  IDirect3DTexture2 *v12; // [sp+20h] [bp-78h]@6
  RenderVertexSoft v[2]; // [sp+30h] [bp-68h]@1
  
  for (uint i = 0; i < uNumProjectiles; ++i)
  {
    ProjectileAnim* p = &pProjectiles[i];

    v[0].vWorldPosition.x = p->srcX;
    v[0].vWorldPosition.y = p->srcY;
    v[0].vWorldPosition.z = p->srcZ;
    v[1].vWorldPosition.x = p->dstX;
    v[1].vWorldPosition.y = p->dstY;
    v[1].vWorldPosition.z = p->dstZ;
    pGame->pIndoorCameraD3D->ViewTransform(v, 2);

    sr_42620A(v);

    pGame->pIndoorCameraD3D->Project(v, 2, 0);

    if (p->uTextureID != -1)
      v12 = pBitmaps_LOD->pHardwareTextures[p->uTextureID];
    else
      v12 = 0;

    v10 = pGame->pIndoorCameraD3D->fov_x / v[1].vWorldViewPosition.x * 20.0;
    v11 = pGame->pIndoorCameraD3D->fov_x / v[0].vWorldViewPosition.x * 20.0;
    pRenderer->DrawProjectile(
        v[0].vWorldViewProjX,
        v[0].vWorldViewProjY,
        v[0].vWorldViewPosition.x,
         v11,
        v[1].vWorldViewProjX,
        v[1].vWorldViewProjY,
        v[1].vWorldViewPosition.x,
         v10,
         v12);
   }
}

//----- (004A73AA) --------------------------------------------------------
void stru6::_4A73AA_hanging_trace_particles___like_fire_strike_ice_blast_etc(SpriteObject *a2, unsigned int uDiffuse, unsigned int uTextureID)
{
  stru6 *v4; // edi@1
  SpriteObject *v5; // esi@1
  int v6; // eax@1
  stru6_stru2 *v7; // eax@2
  double v8; // st7@2
  double v9; // st6@2
  double v10; // st7@3
  Particle_sw local_0; // [sp+8h] [bp-68h]@1
  float x; // [sp+78h] [bp+8h]@2

  v4 = this;
  memset(&local_0, 0, 0x68u);
  v5 = a2;
  v6 = a2->field_54;
  if ( v6 )
  {
    v7 = &v4->array_4[v6 & 0x1F];
    x = ((double)a2->vPosition.x - v7->flt_0_x) * 0.5 + v7->flt_0_x;
    v8 = ((double)v5->vPosition.y - v7->flt_4_y) * 0.5 + v7->flt_4_y;
    v9 = ((double)v5->vPosition.z - v7->flt_8_z) * 0.5 + v7->flt_8_z;
    local_0.type = ParticleType_Bitmap | ParticleType_Rotating | ParticleType_8;
    local_0.uDiffuse = uDiffuse;
    local_0.x = x + 4.0;
    local_0.y = v8;
    local_0.z = v9;
    local_0.r = 0.0;
    local_0.g = 0.0;
    local_0.b = 0.0;
    local_0.timeToLive = (rand() & 0x40) + 96;
    local_0.uTextureID = uTextureID;
    local_0.flt_28 = 1.0;
    pGame->pParticleEngine->AddParticle(&local_0);
    local_0.x = x - 4.0;
    pGame->pParticleEngine->AddParticle(&local_0);
    local_0.x = (double)v5->vPosition.x + 4.0;
    local_0.y = (double)v5->vPosition.y;
    local_0.z = (double)v5->vPosition.z;
    pGame->pParticleEngine->AddParticle(&local_0);
    local_0.x = (double)v5->vPosition.x - 4.0;
    pGame->pParticleEngine->AddParticle(&local_0);
    v4->array_4[v5->field_54 & 0x1F].flt_0_x = (double)v5->vPosition.x;
    v4->array_4[v5->field_54 & 0x1F].flt_4_y = (double)v5->vPosition.y;
    v4->array_4[v5->field_54 & 0x1F].flt_8_z = (double)v5->vPosition.z;
  }
  else
  {
    a2->field_54 = v4->field_0++;
    v4->array_4[a2->field_54 & 0x1F].flt_0_x = (double)a2->vPosition.x;
    v4->array_4[a2->field_54 & 0x1F].flt_4_y = (double)a2->vPosition.y;
    v4->array_4[a2->field_54 & 0x1F].flt_8_z = (double)a2->vPosition.z;
    v10 = (double)a2->vPosition.x;
    local_0.type = ParticleType_Bitmap | ParticleType_Rotating | ParticleType_8;
    local_0.uDiffuse = uDiffuse;
    local_0.x = v10 + 4.0;
    local_0.y = (double)a2->vPosition.y;
    local_0.z = (double)a2->vPosition.z;
    local_0.r = 0.0;
    local_0.g = 0.0;
    local_0.b = 0.0;
    local_0.flt_28 = 1.0;
    local_0.timeToLive = (rand() & 0x7F) + 128;
    local_0.uTextureID = uTextureID;
    pGame->pParticleEngine->AddParticle(&local_0);
    local_0.x = (double)a2->vPosition.x - 4.0;
    pGame->pParticleEngine->AddParticle(&local_0);
  }
}

//----- (004A75CC) --------------------------------------------------------
void stru6::_4A75CC_single_spell_collision_particle(SpriteObject *a1, unsigned int uDiffuse, unsigned int uTextureID)
{
  double v4; // st7@1
  signed int v5; // edi@1
  Particle_sw local_0; // [sp+8h] [bp-68h]@1

  memset(&local_0, 0, 0x68u);
  local_0.type = ParticleType_Bitmap | ParticleType_Rotating | ParticleType_1;
  local_0.x = (double)a1->vPosition.x;
  local_0.y = (double)a1->vPosition.y;
  v4 = (double)a1->vPosition.z;
  local_0.uDiffuse = uDiffuse;
  local_0.z = v4;
  v5 = 10;
  local_0.timeToLive = (rand() & 0x7F) + 128;
  local_0.uTextureID = uTextureID;
  local_0.flt_28 = 1.0;
  do
  {
    local_0.r = (double)(rand() & 0x1FF) - 255.0;
    local_0.g = (double)(rand() & 0x1FF) - 255.0;
    local_0.b = (double)(rand() & 0x1FF) - 255.0;
    pGame->pParticleEngine->AddParticle(&local_0);
    --v5;
  }
  while ( v5 );
}

//----- (004A7688) --------------------------------------------------------
void stru6::_4A7688_fireball_collision_particle(SpriteObject *a2)
{
  double v3; // st7@1
  double v4; // st7@2
  Particle_sw local_0; // [sp+1Ch] [bp-7Ch]@1

  memset(&local_0, 0, 0x68u);

  v3 = (double)a2->uSpriteFrameID / (double)pObjectList->pObjects[a2->uObjectDescID].uLifetime;
  if ( v3 >= 0.75 )
    v4 = (1.0 - v3) * 4.0;
  else
    v4 = v3 * 1.333333333333333;

  local_0.type = ParticleType_Bitmap | ParticleType_Rotating | ParticleType_1;
  local_0.uDiffuse = 0xFF3C1E;
  local_0.x = (double)a2->vPosition.x;
  local_0.y = (double)a2->vPosition.y;
  local_0.z = (double)a2->vPosition.z;
  local_0.timeToLive = (rand() & 0x7F) + 128;
  local_0.uTextureID = pBitmaps_LOD->LoadTexture("effpar01", TEXTURE_DEFAULT);
  local_0.flt_28 = 1.0;

  // 10 fireball sparks 
  for (unsigned int i = 0; i < 10; ++i)
  {
    local_0.r = (rand() & 0x1FF) - 255;
    local_0.g = (rand() & 0x1FF) - 255;
    local_0.b = (rand() & 0x1FF) - 255;
    pGame->pParticleEngine->AddParticle(&local_0);
  }

  pStru1->_47829F_sphere_particle((double)a2->vPosition.x, (double)a2->vPosition.y, (double)a2->vPosition.z, 
                                              (double)floorf(0.5f + (512.0 * v3)), ModulateColor(0xFF3C1E, v4));
}

//----- (004A77FD) --------------------------------------------------------
void stru6::_4A77FD_implosion_particle_d3d(SpriteObject *a1)
{
  double v4; // st7@1
  double v5; // st7@2
  int v7; // eax@4
  float v8; // ST0C_4@4
  float v9; // ST08_4@4
  float v10; // ST04_4@4
  float v11; // ST00_4@4
  float v12; // [sp+28h] [bp-4h]@1

  v4 = (double)a1->uSpriteFrameID / (double)pObjectList->pObjects[a1->uObjectDescID].uLifetime;
  v12 = 512.0 - v4 * 512.0;
  if ( v4 >= 0.75 )
    v5 = v4 * 4.0;
  else
    v5 = v4 * 1.333333333333333;

  v7 = ModulateColor(0x7E7E7E, v5);
  v8 = (double)floorf(0.5f + v12);
  v9 = (double)a1->vPosition.z;
  v10 = (double)a1->vPosition.y;
  v11 = (double)a1->vPosition.x;
  pStru1->_47829F_sphere_particle(v11, v10, v9, v8, v7);
}

//----- (004A78AE) --------------------------------------------------------
void stru6::_4A78AE_sparks_spell(SpriteObject *a1)
{
  ObjectDesc *v2; // esi@1
  unsigned int v3; // eax@1
  double v4; // st7@1
  Particle_sw local_0; // [sp+8h] [bp-68h]@1

  v2 = &pObjectList->pObjects[a1->uObjectDescID];
  memset(&local_0, 0, 0x68u);
  v3 = a1->uSpriteFrameID;
  local_0.x = (double)a1->vPosition.x;
  v4 = (double)a1->vPosition.y;
  local_0.type = ParticleType_Sprite;
  local_0.uDiffuse = 0x7F7F7F;
  local_0.timeToLive = 1;
  local_0.y = v4;
  local_0.z = (double)a1->vPosition.z;
  local_0.r = 0.0;
  local_0.g = 0.0;
  local_0.b = 0.0;
  local_0.uTextureID = pSpriteFrameTable->GetFrame(v2->uSpriteID, v3)->pHwSpriteIDs[0];
  LODWORD(local_0.flt_28) = 0x40000000u;
  pGame->pParticleEngine->AddParticle(&local_0);
}

//----- (004A7948) --------------------------------------------------------
void stru6::_4A7948_mind_blast_after_effect(SpriteObject *a1)
{
  ObjectDesc *v2; // esi@1
  unsigned int v3; // eax@1
  double v4; // st7@1
  char v5; // al@1
  signed int v6; // edi@1
  Particle_sw Dst; // [sp+8h] [bp-68h]@1

  v2 = &pObjectList->pObjects[a1->uObjectDescID];
  memset(&Dst, 0, 0x68u);
  v3 = a1->uSpriteFrameID;
  Dst.x = (double)a1->vPosition.x;
  v4 = (double)a1->vPosition.y;
  Dst.type = ParticleType_Sprite | ParticleType_Rotating | ParticleType_1;
  Dst.uDiffuse = 0x7F7F7F;
  Dst.y = v4;
  Dst.z = (double)a1->vPosition.z;
  Dst.uTextureID = pSpriteFrameTable->GetFrame(v2->uSpriteID, v3)->pHwSpriteIDs[0];
  v5 = rand();
  v6 = 10;
  Dst.flt_28 = 1.0;
  Dst.timeToLive = (v5 & 0x7F) + 128;
  do
  {
    Dst.r = (double)(rand() & 0x1FF) - 255.0;
    Dst.g = (double)(rand() & 0x1FF) - 255.0;
    Dst.b = (double)(rand() & 0x1FF) - 255.0;
    pGame->pParticleEngine->AddParticle(&Dst);
    --v6;
  }
  while ( v6 );
}

//----- (004A7A27) --------------------------------------------------------
bool stru6::AddMobileLight(SpriteObject *a1, unsigned int uDiffuse, int uRadius)
{
  return pMobileLightsStack->AddLight(a1->vPosition.x, a1->vPosition.y, a1->vPosition.z, a1->uSectorID, uRadius,
           (uDiffuse & 0x00FF0000) >> 16,
           (uDiffuse & 0x0000FF00) >> 8,
           uDiffuse & 0x000000FF,
           _4E94D3_light_type);
}
// 4E94D3: using guessed type char _4E94D3_light_type;

//----- (004A7A66) --------------------------------------------------------
void stru6::_4A7A66_miltiple_spell_collision_partifles___like_after_sparks_or_lightning(SpriteObject *a1, unsigned int uDiffuse, unsigned int uTextureID, float a4)
{
  int v5; // eax@1
  char v6; // al@1
  double v7; // st6@1
  double v8; // st6@1
  double v9; // st7@1
  double v10; // st6@1
  Particle_sw local_0; // [sp+0h] [bp-6Ch]@1
  float v12; // [sp+68h] [bp-4h]@1
  float a1a; // [sp+74h] [bp+8h]@1
  float uDiffusea; // [sp+78h] [bp+Ch]@1
  float uTextureIDa; // [sp+7Ch] [bp+10h]@1

  memset(&local_0, 0, 0x68u);
  local_0.type = ParticleType_Bitmap | ParticleType_Rotating | ParticleType_1;
  local_0.x = (double)a1->vPosition.x;
  v5 = a1->vPosition.z;
  local_0.y = (double)a1->vPosition.y;
  local_0.uDiffuse = uDiffuse;
  local_0.z = (double)(v5 + 32);
  v6 = rand();
  local_0.flt_28 = 1.0;
  v7 = 0.0 * a4;
  local_0.timeToLive = (v6 & 0x7F) + 128;
  local_0.uTextureID = uTextureID;
  a1a = v7;
  local_0.r = v7;
  local_0.g = a4;
  local_0.b = a4;
  pGame->pParticleEngine->AddParticle(&local_0);
  v8 = 0.70710677 * a4;
  uDiffusea = v8;
  local_0.r = v8;
  local_0.g = v8;
  local_0.b = a4;
  pGame->pParticleEngine->AddParticle(&local_0);
  local_0.g = a1a;
  local_0.r = a4;
  local_0.b = a4;
  pGame->pParticleEngine->AddParticle(&local_0);
  local_0.r = uDiffusea;
  local_0.b = a4;
  v9 = -uDiffusea;
  uTextureIDa = v9;
  local_0.g = v9;
  pGame->pParticleEngine->AddParticle(&local_0);
  v10 = -1.0 * a4;
  local_0.r = a1a;
  v12 = v10;
  local_0.g = v10;
  local_0.b = a4;
  pGame->pParticleEngine->AddParticle(&local_0);
  local_0.b = a4;
  local_0.r = uTextureIDa;
  local_0.g = uTextureIDa;
  pGame->pParticleEngine->AddParticle(&local_0);
  local_0.r = v12;
  local_0.g = a1a;
  local_0.b = a4;
  pGame->pParticleEngine->AddParticle(&local_0);
  local_0.r = uTextureIDa;
  local_0.g = uDiffusea;
  local_0.b = a4;
  pGame->pParticleEngine->AddParticle(&local_0);
}

//----- (004A7C07) --------------------------------------------------------
void stru6::_4A7C07(SpriteObject *a2)
{
  stru6 *v2; // edi@1
  SpriteObject *v3; // esi@1
  int v4; // eax@1
  ObjectDesc *v5; // ebx@1
  stru6_stru2 *v6; // eax@2
  double v7; // st6@2
  double v8; // st5@2
  double v9; // st4@2
  char v10; // al@2
  double v11; // st7@2
  double v12; // st7@3
  Particle_sw local_0; // [sp+Ch] [bp-68h]@1
  float a2a; // [sp+7Ch] [bp+8h]@2

  v2 = this;
  memset(&local_0, 0, 0x68u);
  v3 = a2;
  v4 = a2->field_54;
  v5 = &pObjectList->pObjects[a2->uObjectDescID];
  if ( v4 )
  {
    v6 = &v2->array_4[v4 & 0x1F];
    v7 = ((double)a2->vPosition.x - v6->flt_0_x) * 0.5 + v6->flt_0_x;
    v8 = ((double)a2->vPosition.y - v6->flt_4_y) * 0.5 + v6->flt_4_y;
    v9 = ((double)a2->vPosition.z - v6->flt_8_z) * 0.5 + v6->flt_8_z;
    local_0.type = ParticleType_Sprite;
    local_0.uDiffuse = 0xFFFFFF;
    a2a = v9;
    local_0.x = v7;
    local_0.z = a2a;
    local_0.y = v8;
    local_0.r = 0.0;
    local_0.g = 0.0;
    local_0.b = 0.0;
    v10 = rand();
    LODWORD(local_0.flt_28) = 0x40400000u;
    local_0.timeToLive = (v10 & 0x3F) + 64;
    local_0.uTextureID = pSpriteFrameTable->GetFrame(v5->uSpriteID, v3->uSpriteFrameID)->pHwSpriteIDs[0];
    pGame->pParticleEngine->AddParticle(&local_0);
    v11 = (double)v3->vPosition.x;
    LODWORD(local_0.flt_28) = 0x40800000u;
    local_0.x = v11;
    local_0.y = (double)v3->vPosition.y;
    local_0.z = (double)v3->vPosition.z;
    local_0.timeToLive = (rand() & 0x3F) + 64;
    pGame->pParticleEngine->AddParticle(&local_0);
    v2->array_4[v3->field_54 & 0x1F].flt_0_x = (double)v3->vPosition.x;
    v2->array_4[v3->field_54 & 0x1F].flt_4_y = (double)v3->vPosition.y;
    v2->array_4[v3->field_54 & 0x1F].flt_8_z = (double)v3->vPosition.z;
  }
  else
  {
    a2->field_54 = v2->field_0++;
    v2->array_4[a2->field_54 & 0x1F].flt_0_x = (double)a2->vPosition.x;
    v2->array_4[a2->field_54 & 0x1F].flt_4_y = (double)a2->vPosition.y;
    v2->array_4[a2->field_54 & 0x1F].flt_8_z = (double)a2->vPosition.z;
    v12 = (double)a2->vPosition.x;
    local_0.type = ParticleType_Sprite;
    local_0.uDiffuse = 0xFFFFFF;
    LODWORD(local_0.flt_28) = 0x40000000u;
    local_0.x = v12;
    local_0.y = (double)a2->vPosition.y;
    local_0.z = (double)a2->vPosition.z;
    local_0.r = 0.0;
    local_0.g = 0.0;
    local_0.b = 0.0;
    local_0.timeToLive = (rand() & 0x3F) + 64;
    local_0.uTextureID = pSpriteFrameTable->GetFrame(v5->uSpriteID, a2->uSpriteFrameID)->pHwSpriteIDs[0];
    pGame->pParticleEngine->AddParticle(&local_0);
  }
}

//----- (004A7E05) --------------------------------------------------------
void stru6::AddProjectile(SpriteObject *a2, int a3, unsigned int uTextureID)
{
  if (a2->field_54)
  {
    DoAddProjectile(array_4[a2->field_54 & 0x1F].flt_0_x,
                    array_4[a2->field_54 & 0x1F].flt_4_y,
                    array_4[a2->field_54 & 0x1F].flt_8_z,
                    a2->vPosition.x,
                    a2->vPosition.y,
                    a2->vPosition.z,
                    uTextureID);
  }
  else
  {
    a2->field_54 = field_0++;
    array_4[a2->field_54 & 0x1F].flt_0_x = (double)a2->vPosition.x;
    array_4[a2->field_54 & 0x1F].flt_4_y = (double)a2->vPosition.y;
    array_4[a2->field_54 & 0x1F].flt_8_z = (double)a2->vPosition.z;
  }
}

//----- (004A7E89) --------------------------------------------------------
void stru6::_4A7E89_sparkles_on_actor_after_it_casts_buff(Actor *pActor, unsigned int uDiffuse)
{
  Actor *v3; // edi@1
  int v4; // ebx@3
  //int result; // eax@5
  Particle_sw Dst; // [sp+Ch] [bp-6Ch]@1
  int v7; // [sp+74h] [bp-4h]@2
  signed int pActora; // [sp+80h] [bp+8h]@1

  memset(&Dst, 0, 0x68u);
  Dst.type = ParticleType_Bitmap | ParticleType_Rotating | ParticleType_8;
  Dst.timeToLive = (rand() & 0x7F) + 128;
  v3 = pActor;
  Dst.uTextureID = pBitmaps_LOD->LoadTexture("effpar02", TEXTURE_DEFAULT);
  pActora = 50;
  Dst.flt_28 = 1.0;
  do
  {
    v7 = (unsigned __int8)rand() + v3->vPosition.x - 127;
    Dst.x = (double)v7;
    v7 = (unsigned __int8)rand() + v3->vPosition.y - 127;
    Dst.y = (double)v7;
    v7 = v3->vPosition.z + (unsigned __int8)rand();
    Dst.z = (double)v7;
    if ( uDiffuse )
    {
      Dst.uDiffuse = uDiffuse;
    }
    else
    {
      v4 = rand() << 16;
      Dst.uDiffuse = rand() | v4;
    }
    pGame->pParticleEngine->AddParticle(&Dst);
    --pActora;
  }
  while ( pActora );
}

//----- (004A7F74) --------------------------------------------------------
void stru6::_4A7F74(int x, int y, int z)
{
  stru6 *v4; // esi@1
  char v5; // al@1
  signed int v6; // edi@1
  unsigned int v7; // eax@1
  double v8; // st7@2
  double v9; // st7@2
  Particle_sw local_0; // [sp+8h] [bp-78h]@1
  double v11; // [sp+70h] [bp-10h]@1
  double v12; // [sp+78h] [bp-8h]@1
  float z1; // [sp+88h] [bp+8h]@2

  v4 = this;
  memset(&local_0, 0, 0x68u);
  local_0.type = ParticleType_Bitmap | ParticleType_Rotating | ParticleType_1;
  local_0.uDiffuse = 0x7E7E7E;
  v5 = rand();
  local_0.flt_28 = 1.0;
  v6 = 8;
  local_0.timeToLive = (v5 & 0x7F) + 128;
  v7 = v4->uTextureID_effpar1;
  v12 = (double)x;
  local_0.uTextureID = v7;
  v11 = (double)y;
  do
  {
    v8 = pRnd->GetRandom();
    local_0.x = v8 * 40.0 - 20.0 + v12;
    v9 = pRnd->GetRandom();
    z1 = (double)z;
    local_0.z = z1;
    local_0.y = v9 * 40.0 - 20.0 + v11;
    local_0.r = pRnd->GetRandom() * 400.0 - 200.0;
    local_0.g = pRnd->GetRandom() * 400.0 - 200.0;
    local_0.b = pRnd->GetRandom() * 150.0 + 50.0;
    pGame->pParticleEngine->AddParticle(&local_0);
    --v6;
  }
  while ( v6 );
}

//----- (004A806F) --------------------------------------------------------
int stru6::_4A806F(Actor *pActor)
{
  int v2; // ecx@1
  unsigned int v3; // eax@1
  double v4; // st7@2
  float v5; // ST00_4@6

  v2 = (int)pActor;
  v3 = LODWORD(pActor->pActorBuffs[ACTOR_BUFF_MASS_DISTORTION].uExpireTime) - pMiscTimer->uTotalGameTimeElapsed;
  if ( (signed int)v3 <= 64 )
  {
    if ( (v3 & 0x80000000u) != 0 )
    {
      pActor->pActorBuffs[ACTOR_BUFF_MASS_DISTORTION].uExpireTime = 0i64;
      v4 = 1.0;
    }
    else
    {
      v2 = v3 * v3;
      v4 = 1.0 - (double)(signed int)(v3 * v3) * 0.0001953125;
    }
  }
  else
  {
    v2 = (v3 - 64) * (v3 - 64);
    v4 = (double)v2 * 0.0001953125 + 0.2;
  }
  v5 = v4;
  return fixpoint_from_float(v5);
}

//----- (004A81CA) --------------------------------------------------------
bool stru6::_4A81CA(SpriteObject *a2)
{
  //stru6 *v2; // ebx@1
  int result; // eax@1
//  int v4; // eax@27
  //unsigned int diffuse; // esi@41
  //int v6; // ecx@49
  int v7; // eax@54
  int v8; // eax@55
  char v9; // zf@56
  int v10; // eax@59
  int v11; // eax@61
  int v12; // eax@85
  int v13; // eax@86
  int v14; // eax@96
  int v15; // eax@111
  int v16; // eax@118
  int v17; // eax@139
  int v18; // eax@140
  int v19; // eax@141
  int v20; // eax@151
  //unsigned int v21; // [sp-8h] [bp-20h]@66
  //SpriteObject *v22; // [sp-8h] [bp-20h]@81
  //unsigned int v23; // [sp-4h] [bp-1Ch]@4
  //unsigned int v24; // [sp-4h] [bp-1Ch]@5
//  unsigned int v25; // [sp-4h] [bp-1Ch]@30
  //unsigned int v26; // [sp-4h] [bp-1Ch]@57
  //unsigned int v27; // [sp-4h] [bp-1Ch]@66
  //int v28; // [sp-4h] [bp-1Ch]@81
  //unsigned int v29; // [sp+0h] [bp-18h]@4
  //unsigned int v30; // [sp+0h] [bp-18h]@5
  //int v31; // [sp+0h] [bp-18h]@30
  //unsigned int v32; // [sp+0h] [bp-18h]@45
  //float v33; // [sp+0h] [bp-18h]@57
  //float v34; // [sp+0h] [bp-18h]@66
  //signed int v35; // [sp+0h] [bp-18h]@81
  char pContainer[7]; // [sp+10h] [bp-8h]@81

  //__debugbreak(); // need to refactor carefully & collect data
  //v2 = this;
  result = a2->uType;

  switch (a2->uType)
  {
    case 500:   // blue rat bolt for example
    case 530:
      _4A73AA_hanging_trace_particles___like_fire_strike_ice_blast_etc(a2, 0x00AAAFF, uTextureID_effpar1);
    return false;

    case 510:   // dragon fire for example
      _4A73AA_hanging_trace_particles___like_fire_strike_ice_blast_etc(a2, 0xFF3C1E, uTextureID_effpar1);
    return false;

    case 545:
    case 550:
    case 555: return true;

    case 556:
      AddMobileLight(a2, 0xFF0000, 256);
    return false;

    case 600:
      AddMobileLight(a2, 0xFF3C1E, 256);
    return true;

    case 1010: // Fire Bolt
    {
      _4A73AA_hanging_trace_particles___like_fire_strike_ice_blast_etc(a2, 0xFF3C1E, uTextureID_effpar1);
      AddMobileLight(a2, 0xFF3C1E, 256);
    }
    return false;

    case 1011: // Fire Bolt impact
    {
      _4A75CC_single_spell_collision_particle(a2, 0xFF3C1E, uTextureID_effpar1);
      AddMobileLight(a2, 0xFF3C1E, 256);
    }
    return false;

    case 1050: // Fireball
    {
      _4A73AA_hanging_trace_particles___like_fire_strike_ice_blast_etc(a2, 0xFF3C1E, uTextureID_effpar1);
      AddMobileLight(a2, 0xFF3C1E, 256);
    }
    return false;

    case 1051: // Fireball hit
    {
      AddMobileLight(a2, 0xFF3C1E, 256);
      //if (pRenderer->pRenderD3D)
      {
        result = PID_TYPE(a2->spell_caster_pid);
        if (PID_TYPE(a2->spell_caster_pid) != OBJECT_Actor &&
            PID_TYPE(a2->spell_caster_pid) != OBJECT_Item)
        {
          if (field_204 != 4)
          {
            field_204++;
            _4A7688_fireball_collision_particle(a2);
          }
          return false;
        }
      }
    }
    return true;

    case 1081:
    {
      _4A7A66_miltiple_spell_collision_partifles___like_after_sparks_or_lightning(a2, 0xFF3C1E, uTextureID_effpar1, 300.0);
      _4A7A66_miltiple_spell_collision_partifles___like_after_sparks_or_lightning(a2, 0xFF3C1E, uTextureID_effpar1, 250.0);
      _4A7A66_miltiple_spell_collision_partifles___like_after_sparks_or_lightning(a2, 0xFF3C1E, uTextureID_effpar1, 200.0);
      AddMobileLight(a2, 0xFF3C1E, 256);
    }
    return false;

    case 2101:
    {
      _4A7A66_miltiple_spell_collision_partifles___like_after_sparks_or_lightning(a2, 0xC8C814, uTextureID_effpar1, 200.0);
      AddMobileLight(a2, 0xC8C814, 256);
    }
    return false;

    case 3060: // Acid Burst
      _4A73AA_hanging_trace_particles___like_fire_strike_ice_blast_etc(a2, 0x0AB450, uTextureID_effpar1);
      AddMobileLight(a2, 0x0AB450, 256);
    return false;

    case 9000: return true;
  }

  if ( result < 545 )
  {
    //__debugbreak(); // find out what kind of spells are these
					// when summoned light elemental attacks
	//cast spell from trees in Avlee
      result -= 500;
      switch ( result )
      {
        case 1:
        case 31:
          _4A75CC_single_spell_collision_particle(a2, 0xAAAFF, uTextureID_effpar1);
          return true;
        case 5:
          _4A73AA_hanging_trace_particles___like_fire_strike_ice_blast_etc(a2, 0x5C310E, uTextureID_effpar1);
          return false;
        case 15:
          _4A73AA_hanging_trace_particles___like_fire_strike_ice_blast_etc(a2, 0x0062D0, uTextureID_effpar1);
          return false;
        case 16:
          _4A75CC_single_spell_collision_particle(a2, 0x0062D0, uTextureID_effpar1);
          return false;
        case 20:
          _4A73AA_hanging_trace_particles___like_fire_strike_ice_blast_etc(a2, 0x0AB450, uTextureID_effpar1);
          return false;
        case 21:
          _4A75CC_single_spell_collision_particle(a2, 0x0AB450, uTextureID_effpar1);
          return false;
        case 25:
          _4A73AA_hanging_trace_particles___like_fire_strike_ice_blast_etc(a2, 0xC8C805, uTextureID_effpar1);
          return false;
        case 26:
          _4A75CC_single_spell_collision_particle(a2, 0xC8C805, uTextureID_effpar1);
          return false;
        case 35:
          _4A73AA_hanging_trace_particles___like_fire_strike_ice_blast_etc(a2, 0xFFFFFF, uTextureID_effpar1);
          return false;
        case 36:
          _4A75CC_single_spell_collision_particle(a2, 0xFFFFFF, uTextureID_effpar1);
        return false;

        case 40:
        _4A73AA_hanging_trace_particles___like_fire_strike_ice_blast_etc(a2, 0x7E7E7E, uTextureID_effpar1);
        return false;

        case 11:
          _4A75CC_single_spell_collision_particle(a2, 0xFF3C1E, uTextureID_effpar1);
          return false;
        case 6:
          _4A75CC_single_spell_collision_particle(a2, 0x5C310E, uTextureID_effpar1);
          return false;

        case 41:
          _4A75CC_single_spell_collision_particle(a2, 0x7E7E7E, uTextureID_effpar1);
          return false;
        default:
          return false;
      }
      return false;
  }

  if ( result <= 4051 )
  {
    if ( result != 4051 )
    {
      if ( result <= 2031 )
      {
        if ( result != 2031 )
        {
          if ( result < 1051 )
          {
            if ( result < 600 )
            {
              _4A75CC_single_spell_collision_particle(a2, 0xFF3C1E, uTextureID_effpar1);
              return false;
            }
            if ( result < 811 )
              return false;
            if ( result <= 814 )
              return true;
            return false;

          }
          if ( result <= 1081 )
          {
            if (result != 1081)
            {
              result -= 1060;
              if ( !result )
                return true;
              v7 = result - 1;
              if ( v7 )
              {
                v8 = v7 - 9;
                if ( !v8 )
                {
                  _4A75CC_single_spell_collision_particle(a2, 0xFF3C1E, uTextureID_effpar1);
                  return false;
                }
                result = v8 - 10;
                v9 = result == 0;
//LABEL_129:
                if ( v9 )
                  return true;
                return false;
              }
              _4A7A66_miltiple_spell_collision_partifles___like_after_sparks_or_lightning(a2, 0xFF3C1E, uTextureID_effpar1, 250.0);
              AddMobileLight(a2, 0xFF3C1E, 256);
              return false;
            }
//LABEL_84:
//            _4A7A66_miltiple_spell_collision_partifles___like_after_sparks_or_lightning(a2, diffuse, v26, v33);
            //goto LABEL_43;
            return false;
          }
          v10 = result - 1090;
          if ( v10 )
          {
            result = v10 - 10;
            if ( !result )
              return true;
            v11 = result - 1;
            if ( !v11 )
            {
              //diffuse = 0xFF3C1E;
              _4A75CC_single_spell_collision_particle(a2, 0xFF3C1E, uTextureID_effpar1);
              //v32 = v2->uTextureID_effpar1;
              _4A75CC_single_spell_collision_particle(a2, 0xFF3C1E, uTextureID_effpar1);
              //goto LABEL_43;
              AddMobileLight(a2, 0xFF3C1E, 256);
              return false;
            }
            result = v11 - 929;
            if ( result )
              return false;
//LABEL_63:
            //if ( !pRenderer->pRenderD3D )
            //  return true;
            _4A78AE_sparks_spell(a2);
            AddMobileLight(a2, 0x64640F, 128);
            return false;
          }

          _4A7A66_miltiple_spell_collision_partifles___like_after_sparks_or_lightning(a2, 0xFF3C1Eu, uTextureID_effpar1, 250.0);
          return false;
        }
        //goto LABEL_67;
      }
      if ( result < 3060 )
      {
        if ( result < 2101 )
        {
          if ( result != 2060 )
          {
            if ( result != 2061 )
            {
              if ( result <= 2079 )
                return false;
              if ( result <= 2081 )
              {
                //if ( pRenderer->pRenderD3D )
                  _4A77FD_implosion_particle_d3d(a2);
                /*else
                  _4A80DC_implosion_particle_sw(a2);*/
                return false;
              }
              v9 = result == 2100;
                if ( v9 )
                  return true;
                return false;
            }
            _4A7A66_miltiple_spell_collision_partifles___like_after_sparks_or_lightning(a2, 0xC8C814, uTextureID_effpar2, 200.0);
            AddMobileLight(a2, 0xC8C814, 256);
            return false;
          }
          //if ( !pRenderer->pRenderD3D )
          //  return true;
          memcpy(pContainer, "sp18h1", 7);
          pRnd->SetRange(1, 6);
          pContainer[5] = pRnd->GetInRange() + '0';
          AddProjectile(a2, 100, pBitmaps_LOD->LoadTexture(pContainer));
          return false;
        }
        v12 = result - 3010;
        if ( !v12 )
        {
          _4A73AA_hanging_trace_particles___like_fire_strike_ice_blast_etc(a2, 0xAB450, uTextureID_effpar1);
          AddMobileLight(a2, 0xAB450, 256);
          return false;
        }
        v13 = v12 - 1;
        if ( !v13 )
        {
          _4A75CC_single_spell_collision_particle(a2, 0xAB450, uTextureID_effpar1);
          AddMobileLight(a2, 0xAB450, 256);
          return false;
        }
        result = v13 - 19;
        if ( !result )
          return true;
        --result;
        if ( result )
          return false;

        _4A75CC_single_spell_collision_particle(a2, 0x9EB9F1, uTextureID_effpar1);
        AddMobileLight(a2, 0x9EB9F1, 256);
        return false;
      }
      if ( result > 4000 )
      {
        result -= 4030;
        if ( !result || (--result, !result) )
          return true;
        result -= 19;
        v9 = result == 0;
                if ( v9 )
                  return true;
                return false;
      }
      if ( result == 4000 )
      {
        //if ( !pRenderer->pRenderD3D )
        //  return true;
        _4A7C07(a2);
        return false;
      }
      result -= 3061;
      if ( !result || (result -= 29) == 0 )
        return true;
      v14 = result - 1;
      if ( !v14 )
      {
        _4A75CC_single_spell_collision_particle(a2, 0x9EB9F1, uTextureID_effpar1);
        AddMobileLight(a2, 0x9EB9F1, 256);
        return false;
      }
      result = v14 - 1;
      if ( result )
        return false;
      _4A73AA_hanging_trace_particles___like_fire_strike_ice_blast_etc(a2, 0x9EB9F1, uTextureID_effpar1);
      return false;
    }
    _4A7948_mind_blast_after_effect(a2);
    return false;
  }
  if ( result <= 7090 )
  {
    if ( result == 7090 )
      return true;
    if ( result > 6030 )
    {
      if ( result > 6091 )
      {
        result -= 6100;
        if ( !result )
          return true;
        result -= 930;
        if ( result )
        {
          --result;
          if (!result)
            _4A75CC_single_spell_collision_particle(a2, 0xF00000, uTextureID_effpar1);
          return false;
        }
            //if ( !pRenderer->pRenderD3D )
            //  return true;
            _4A78AE_sparks_spell(a2);
            AddMobileLight(a2, 0x64640F, 128);
            return false;
      }
      if ( result != 6091 )
      {
        result -= 6040;
        if ( !result || (result -= 20) == 0 || (result -= 10) == 0 )
          return true;
        result -= 20;
        v9 = result == 0;
                if ( v9 )
                  return true;
                return false;
      }
    }
    else
    {
      if ( result == 6030 )
        return true;
      if ( result <= 4091 )
      {
        if ( result != 4091 )
        {
          result -= 4070;
          if ( !result )
            return true;
          v15 = result - 1;
          if ( v15 )
          {
            result = v15 - 9;
            if ( !result )
              return true;
            result -= 10;
            if ( !result )
            {
              _4A73AA_hanging_trace_particles___like_fire_strike_ice_blast_etc(a2, 0x7E7E7E, uTextureID_effpar1);
              return true;
            }
            return false;
          }
          _4A75CC_single_spell_collision_particle(a2, 0x5C310E, uTextureID_effpar1);
          return false;
        }
        _4A7A66_miltiple_spell_collision_partifles___like_after_sparks_or_lightning(a2, 0x7E7E7E, uTextureID_effpar1, 200.0);
        return false;
      }
      v16 = result - 4092;
      if ( !v16 )
      {
        _4A73AA_hanging_trace_particles___like_fire_strike_ice_blast_etc(a2, 0x7E7E7E, uTextureID_effpar1);
        return false;
      }
      result = v16 - 8;
      if ( !result )
        return false;
      result -= 1910;
      if ( !result )
        return true;
      --result;
      if ( result )
        return false;
    }
//LABEL_122:
    _4A7948_mind_blast_after_effect(a2);
    return false;
  }
  if ( result <= 9000 )
  {
    if ( result <= 8030 )
    {
      if ( result == 8030 )
        return true;
      v17 = result - 7091;
      if ( v17 )
      {
        v18 = v17 - 909;
        if ( !v18 )
        {
          _4A73AA_hanging_trace_particles___like_fire_strike_ice_blast_etc(a2, 0xFFFFFF, uTextureID_effpar3);
          AddMobileLight(a2, 0xFFFFFF, 128);
          return false;
        }
        v19 = v18 - 1;
        if ( v19 )
        {
          result = v19 - 9;
          if ( !result )
          {
            AddMobileLight(a2, 0xFFFFFF, 64);
            return false;
          }
          return false;
        }
        _4A75CC_single_spell_collision_particle(a2, 0xFFFFFF, uTextureID_effpar2);
        AddMobileLight(a2, 0xFFFFFF, 256);
        return false;
      }
      else
      {
        _4A75CC_single_spell_collision_particle(a2, 0xF00000, uTextureID_effpar1);
        AddMobileLight(a2, 0xF00000, 256);
        return false;
      }
    }
    result -= 8040;
    if ( !result || (result -= 20) == 0 )
      return true;
    v20 = result - 30;
    if ( v20 )
    {
      result = v20 - 1;
      if ( result )
        return false;

      _4A75CC_single_spell_collision_particle(a2, 0xFFFFFF, uTextureID_effpar3);
      return false;
    }
    AddMobileLight(a2, 0xFFFFFFu, 128);
    //if ( !pRenderer->pRenderD3D )
    //  return true;
    AddProjectile(a2, 100, -1);
    return false;
  }
  if ( result > 9050 )
  {
    result -= 9070;
    if ( !result || (result -= 10) == 0 || (--result, !result) )
      return true;
    return false;
  }
  if ( result == 9050 || (result -= 9010) == 0 || (result -= 20) == 0 || (result -= 10) == 0 )
    return true;
  --result;
  if ( !result )
  {
//LABEL_164:
    _4A75CC_single_spell_collision_particle(a2, 0x7E7E7E, uTextureID_effpar1);
    return false;
  }
//LABEL_168:
  return false;
}

//----- (004A89BD) --------------------------------------------------------
void stru6::SetPlayerBuffAnim(unsigned __int16 uSpellID, unsigned __int16 uPlayerID)
{
  //stru6 *v3; // edi@1
  PlayerBuffAnim *v4; // esi@1
  const char *v6; // [sp-4h] [bp-10h]@2

  v4 = &pPlayerBuffs[uPlayerID];
  v4->uSpellAnimTimeElapsed = 0;
  v4->bRender = uSpellID != 0;

  switch (uSpellID)
  {
    case SPELL_DISEASE:
      v6 = "zapp";
    break;
      
    case BECOME_MAGIC_GUILD_MEMBER:
    case SPELL_AIR_FEATHER_FALL:
    case SPELL_SPIRIT_DETECT_LIFE:
    case SPELL_SPIRIT_FATE:
      v6 = "spboost1";
    break;
      
    case SPELL_QUEST_COMPLETED:
    case SPELL_AIR_INVISIBILITY:
    case SPELL_WATER_WATER_WALK:
    case SPELL_SPIRIT_PRESERVATION:
      v6 = "spboost2";
    break;
      
    case SPELL_152:
      __debugbreak(); // spell id == 152 wtf
    case SPELL_LIGHT_HOUR_OF_POWER:
    case SPELL_LIGHT_DAY_OF_THE_GODS:
    case SPELL_LIGHT_DAY_OF_PROTECTION:
    case SPELL_LIGHT_DIVINE_INTERVENTION:
      v6 = "spboost3";
    break;

    case SPELL_SPIRIT_REMOVE_CURSE:
    case SPELL_MIND_REMOVE_FEAR:
    case SPELL_BODY_CURE_WEAKNESS:
      v6 = "spheal1";
    break;

    case SPELL_SPIRIT_SHARED_LIFE:
    case SPELL_MIND_CURE_PARALYSIS:
    case SPELL_MIND_CURE_INSANITY:
    case SPELL_BODY_FIRST_AID:
    case SPELL_BODY_CURE_POISON:
    case SPELL_BODY_CURE_DISEASE:
    case SPELL_DARK_SACRIFICE:
      v6 = "spheal2";
    break;

    case SPELL_BODY_POWER_CURE:
    case SPELL_DARK_SOULDRINKER:
      v6 = "spheal3";
    break;

    case SPELL_FIRE_PROTECTION_FROM_FIRE:
    case SPELL_FIRE_IMMOLATION:
      v6 = "spell03";
    break;

    case SPELL_FIRE_HASTE:                  v6 = "spell05"; break;
    case SPELL_AIR_PROTECTION_FROM_AIR:     v6 = "spell14"; break;
    case SPELL_AIR_SHIELD:                  v6 = "spell17"; break;
    case SPELL_WATER_PROTECTION_FROM_WATER: v6 = "spell25"; break;
    case SPELL_EARTH_PROTECTION_FROM_EARTH: v6 = "spell36"; break;
    case SPELL_EARTH_STONESKIN:             v6 = "spell38"; break;
    case SPELL_SPIRIT_BLESS:                v6 = "spell46"; break;
    case SPELL_SPIRIT_HEROISM:              v6 = "spell51"; break;
    case SPELL_SPIRIT_RESSURECTION:         v6 = "spell55"; break;
    case SPELL_MIND_PROTECTION_FROM_MIND:   v6 = "spell58"; break;
    case SPELL_BODY_PROTECTION_FROM_BODY:   v6 = "spell69"; break;
    case SPELL_BODY_REGENERATION:           v6 = "spell71"; break;
    case SPELL_BODY_HAMMERHANDS:            v6 = "spell73"; break;
    case SPELL_BODY_PROTECTION_FROM_MAGIC:  v6 = "spell75"; break;

    default:
      v4->bRender = false;
      return;
  }
  
  v4->uSpellIconID = pIconsFrameTable->FindIcon(v6);
  if (v4->bRender)
    v4->uSpellAnimTime = 8 * pIconsFrameTable->pIcons[v4->uSpellIconID].uAnimLength;
}

//----- (004A8BDF) --------------------------------------------------------
void stru6::FadeScreen__like_Turn_Undead_and_mb_Armageddon(unsigned int uDiffuseColor, unsigned int uFadeTime)
{
  this->uFadeTime = uFadeTime;
  this->uFadeLength = uFadeTime;
  this->uFadeColor = uDiffuseColor;
}

//----- (004A8BFC) --------------------------------------------------------
int stru6::_4A8BFC() //for SPELL_LIGHT_PRISMATIC_LIGHT
{
  uAnimLength = 8 * pSpriteFrameTable->pSpriteSFrames[pSpriteFrameTable->FastFindSprite("spell84")].uAnimLength;
  return uAnimLength;
}

//----- (004A8C27) --------------------------------------------------------
void stru6::RenderSpecialEffects()
{
  double v4; // st7@4
  double v5; // st6@4
  //double v6; // st7@4
  float v7; // ST14_4@6
  unsigned int v8; // ST14_4@8
  unsigned int v9; // eax@8
  SpriteFrame *v10; // eax@8
  int v11; // edi@8
  double v12; // st7@9
  double v13; // st7@9
  double v14; // st7@9
  double v15; // st6@9
//  HRESULT v16; // eax@9
//  HRESULT v17; // eax@9
//  signed __int64 v18; // qtt@10
//  const char *v19; // [sp+4h] [bp-E8h]@0
//  int v20; // [sp+8h] [bp-E4h]@0
//  unsigned int v21; // [sp+Ch] [bp-E0h]@0
  RenderVertexD3D3 vd3d[4]; // [sp+60h] [bp-8Ch]@9
//  int v24; // [sp+E0h] [bp-Ch]@10
  SpriteFrame *v70; // [sp+E4h] [bp-8h]@8
  unsigned int v26; // [sp+E8h] [bp-4h]@3

  if (uNumProjectiles)
  {
    DrawProjectiles();
    uNumProjectiles = 0;
  }

  field_204 = 0;
  if ( uFadeTime > 0 )
  {
    v4 = (double)uFadeTime / (double)uFadeLength;
    v5 = 1.0 - v4 * v4;
    //v6 = v5;
    if ( v5 > 0.9 )
      v5 = 1.0 - (v5 - 0.9) * 10.0;
    v7 = v5;
    pRenderer->ScreenFade(uFadeColor, v7);
    uFadeTime -= pEventTimer->uTimeElapsed;
  }

  if (uAnimLength > 0)
  {
    v8 = 8
       * pSpriteFrameTable->pSpriteSFrames[pSpriteFrameTable->FastFindSprite("spell84")].uAnimLength
       - uAnimLength;
    v9 = pSpriteFrameTable->FastFindSprite("spell84");
    v10 = pSpriteFrameTable->GetFrame(v9, v8);
    v11 = v10->pHwSpriteIDs[0];
    v70 = v10;
    uAnimLength -= pEventTimer->uTimeElapsed;
    //if ( pRenderer->pRenderD3D )
    {
      v12 = (double)(signed int)pViewport->uViewportTL_X;
      vd3d[0].pos.x = v12;
      vd3d[1].pos.x = v12;
      v13 = (double)(signed int)pViewport->uViewportBR_X;
      v26 = pViewport->uViewportBR_Y + 1;
      vd3d[0].diffuse = 0x7F7F7Fu;
      vd3d[1].diffuse = 0x7F7F7Fu;
      vd3d[2].pos.x = v13;
      vd3d[3].pos.x = v13;
      v14 = (double)(signed int)pViewport->uViewportTL_Y;
      vd3d[2].diffuse = 0x7F7F7Fu;
      vd3d[3].diffuse = 0x7F7F7Fu;
      vd3d[0].specular = 0;
      vd3d[1].specular = 0;
      vd3d[0].pos.y = v14;
      v15 = (double)(pViewport->uViewportBR_Y + 1);
      vd3d[2].specular = 0;
      vd3d[3].specular = 0;
      vd3d[1].pos.y = v15;
      vd3d[2].pos.y = v15;
      vd3d[3].pos.y = v14;
      vd3d[0].pos.z = 0.0;
      vd3d[1].pos.z = 0.0;
      vd3d[2].pos.z = 0.0;
      vd3d[3].pos.z = 0.0;
      vd3d[0].rhw = 1.0;
      vd3d[1].rhw = 1.0;
      vd3d[2].rhw = 1.0;
      vd3d[3].rhw = 1.0;
      vd3d[0].texcoord.x = 0.0;
      vd3d[1].texcoord.x = 0.0;
      vd3d[2].texcoord.x = 1.0;
      vd3d[3].texcoord.x = 1.0;
      vd3d[0].texcoord.y = 0.0;
      vd3d[1].texcoord.y = 1.0;
      vd3d[2].texcoord.y = 1.0;
      vd3d[3].texcoord.y = 0.0;

      pRenderer->DrawSpecialEffectsQuad(vd3d, pSprites_LOD->pHardwareSprites[v11].pTexture);
    }
    /*else
    {
      vsr.pTarget = pRenderer->pTargetSurface;
      vsr.sParentBillboardID = -1;
      vsr.pTargetZ = pRenderer->pActiveZBuffer;
      vsr.uScreenSpaceX = (signed int)(pViewport->uViewportBR_X - pViewport->uViewportTL_X) / 2;
      vsr.uScreenSpaceY = pViewport->uViewportBR_Y;
      v24 = 16777216;
      LODWORD(v18) = 0;
      HIDWORD(v18) = (signed __int16)(LOWORD(pViewport->uViewportBR_X) - LOWORD(pViewport->uViewportTL_X));
      vsr._screenspace_x_scaler_packedfloat = v18 / 0x1000000;
      LODWORD(v18) = 0;
      HIDWORD(v18) = (signed __int16)(LOWORD(pViewport->uViewportBR_Y) - LOWORD(pViewport->uViewportTL_Y));
      v26 = v18 / 16777216;
      vsr._screenspace_y_scaler_packedfloat = v18 / 0x1000000;
      vsr.pPalette = PaletteManager::Get_Dark_or_Red_LUT(v70->uPaletteIndex, 0, 1);
      vsr.uTargetPitch = pRenderer->uTargetSurfacePitch;
      vsr.sParentBillboardID = -1;
      vsr.uViewportX = pViewport->uViewportTL_X;
      vsr.uViewportZ = pViewport->uViewportBR_X;
      vsr.uViewportY = pViewport->uViewportTL_Y;
      vsr.sZValue = 0;
      vsr.uViewportW = pViewport->uViewportBR_Y;
      vsr.uFlags = 0;
      if ( v11 >= 0 )
        pSprites_LOD->pSpriteHeaders[v11].DrawSprite_sw(&vsr, 1);
    }*/
  }
}

//----- (004A902A) --------------------------------------------------------
void stru6::DrawPlayerBuffAnims()
{
  for (uint i = 0; i < 4; ++i)
  {
    PlayerBuffAnim* buff = &pPlayerBuffs[i];
    if (!buff->bRender)
      continue;

    buff->uSpellAnimTimeElapsed += pEventTimer->uTimeElapsed;
    if (buff->uSpellAnimTimeElapsed >= buff->uSpellAnimTime)
    {
      buff->bRender = false;
      continue;
    }

    IconFrame* icon = pIconsFrameTable->GetFrame(buff->uSpellIconID, buff->uSpellAnimTimeElapsed);
    pRenderer->DrawTextureTransparent(pPlayerPortraitsXCoords_For_PlayerBuffAnimsDrawing[i], 385, pIcons_LOD->GetTexture(icon->uTextureID));

    pOtherOverlayList->bRedraw = true;
  }
}

//----- (004A90A0) --------------------------------------------------------
void stru6::LoadAnimations()
{
  uTextureID_effpar1 = pBitmaps_LOD->LoadTexture("effpar01", TEXTURE_DEFAULT);
  uTextureID_effpar2 = pBitmaps_LOD->LoadTexture("effpar02", TEXTURE_DEFAULT);
  uTextureID_effpar3 = pBitmaps_LOD->LoadTexture("effpar03", TEXTURE_DEFAULT);
  uSpriteID_sp57c = pSprites_LOD->LoadSprite("sp57c", 6);

  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("zapp"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("spheal1"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("spheal2"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("spheal3"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("spboost1"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("spboost2"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("spboost3"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("spell03"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("spell05"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("spell14"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("spell17"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("spell21"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("spell25"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("spell27"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("spell36"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("spell38"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("spell46"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("spell51"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("spell55"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("spell58"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("spell69"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("spell71"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("spell73"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("spell75"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("spell96"));

  pSpriteFrameTable->InitializeSprite(pSpriteFrameTable->FastFindSprite("spell01"));
  pSpriteFrameTable->InitializeSprite(pSpriteFrameTable->FastFindSprite("spell02"));
  pSpriteFrameTable->InitializeSprite(pSpriteFrameTable->FastFindSprite("spell03"));
  pSpriteFrameTable->InitializeSprite(pSpriteFrameTable->FastFindSprite("spell09"));
  pSpriteFrameTable->InitializeSprite(pSpriteFrameTable->FastFindSprite("spell11"));
  pSpriteFrameTable->InitializeSprite(pSpriteFrameTable->FastFindSprite("spell18"));
  pSpriteFrameTable->InitializeSprite(pSpriteFrameTable->FastFindSprite("spell22"));
  pSpriteFrameTable->InitializeSprite(pSpriteFrameTable->FastFindSprite("spell26"));
  pSpriteFrameTable->InitializeSprite(pSpriteFrameTable->FastFindSprite("spell29"));
  pSpriteFrameTable->InitializeSprite(pSpriteFrameTable->FastFindSprite("spell39"));
  pSpriteFrameTable->InitializeSprite(pSpriteFrameTable->FastFindSprite("spell39c"));
  pSpriteFrameTable->InitializeSprite(pSpriteFrameTable->FastFindSprite("spell41"));
  pSpriteFrameTable->InitializeSprite(pSpriteFrameTable->FastFindSprite("spell57c"));
  pSpriteFrameTable->InitializeSprite(pSpriteFrameTable->FastFindSprite("spell62"));
  pSpriteFrameTable->InitializeSprite(pSpriteFrameTable->FastFindSprite("spell65"));
  pSpriteFrameTable->InitializeSprite(pSpriteFrameTable->FastFindSprite("spell66"));
  pSpriteFrameTable->InitializeSprite(pSpriteFrameTable->FastFindSprite("spell70"));
  pSpriteFrameTable->InitializeSprite(pSpriteFrameTable->FastFindSprite("spell76"));
  pSpriteFrameTable->InitializeSprite(pSpriteFrameTable->FastFindSprite("spell84"));
  pSpriteFrameTable->InitializeSprite(pSpriteFrameTable->FastFindSprite("spell90"));
  pSpriteFrameTable->InitializeSprite(pSpriteFrameTable->FastFindSprite("spell92"));
  pSpriteFrameTable->InitializeSprite(pSpriteFrameTable->FastFindSprite("spell93"));
  pSpriteFrameTable->InitializeSprite(pSpriteFrameTable->FastFindSprite("spell97"));
  pSpriteFrameTable->InitializeSprite(pSpriteFrameTable->FastFindSprite("spell97c"));
  pSpriteFrameTable->InitializeSprite(pSpriteFrameTable->FastFindSprite("spell97c"));
}
