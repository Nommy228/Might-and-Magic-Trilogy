#include <string>
#include "Lights.h"


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
