#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include "Lights.h"
#include "OSAPI.h"


//----- (00467D88) --------------------------------------------------------
bool LightsStack_MobileLight_::AddLight(__int16 x, __int16 y, __int16 z, __int16 uSectorID, int uRadius, unsigned __int8 r, unsigned __int8 g, unsigned __int8 b, char uLightType)
{
  if (uNumLightsActive >= 400)
  {
    MessageBoxW(nullptr, L"Too many mobile lights!", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\MobileLightStack.cpp:51", 0);
    return false;
  }

  pLights[uNumLightsActive].vPosition.x = x;
  pLights[uNumLightsActive].vPosition.y = y;
  pLights[uNumLightsActive].vPosition.z = z;
  pLights[uNumLightsActive].uRadius = uRadius;
  pLights[uNumLightsActive].field_C = (((uRadius < 0) - 1) & 0x3E) - 31;
  pLights[uNumLightsActive].uSectorID = uSectorID;
  pLights[uNumLightsActive].field_10 = uRadius * uRadius >> 5;
  pLights[uNumLightsActive].uLightColorR = r;
  pLights[uNumLightsActive].uLightColorG = g;
  pLights[uNumLightsActive].uLightColorB = b;
  pLights[uNumLightsActive++].uLightType = uLightType;

  return true;
}

bool LightsStack_StationaryLight_::AddLight( __int16 x, __int16 y, __int16 z, __int16 a5, unsigned char r, unsigned char g, unsigned char b, char uLightType )
{
  unsigned int v9; // eax@1
  std::string v11; // [sp-18h] [bp-18h]@3

  v9 = this->uNumLightsActive;
  if ( (signed int)v9 >= 400 )
  {
    MessageBoxW(nullptr, L"Too many stationary lights!", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\StationaryLightStack.cpp:45", 0);
    return false;
  }

  StationaryLight* pLight = &pLights[uNumLightsActive++];
  pLight->vPosition.x = x;
  pLight->vPosition.y = y;
  pLight->vPosition.z = z;
  pLight->uRadius = a5;
  pLight->uLightColorR = (unsigned __int8)r;
  pLight->uLightColorG = g;
  pLight->uLightColorB = b;
  pLight->uLightType = uLightType;
  return true;
}
