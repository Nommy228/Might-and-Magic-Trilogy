#include <string>

#include "GammaControl.h"
#include "Render.h"
#include "OSInfo.h"

#include "mm7_data.h"



//----- (0044F324) --------------------------------------------------------
void GammaController::GetRamp()
{
  if (pGammaControl)
    ErrD3D(pGammaControl->GetGammaRamp(0, &pDefaultRamp));
  else
    MessageBoxW(nullptr, L"Gamma control not active", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\GammaControl.cpp:83", 0);
}

//----- (0044F377) --------------------------------------------------------
double GammaController::_44F377(DDGAMMARAMP *a1)
{
  double v2; // st7@1
  unsigned __int16 *v3; // ecx@1
  double v4; // st6@2
  double result; // st7@7
  signed int v6; // [sp+0h] [bp-8h]@1
  signed int v7; // [sp+4h] [bp-4h]@1

  v7 = 0;
  v2 = 0.0;
  v6 = 256;
  v3 = a1->green;
  do
  {
    v4 = ((double)*(v3 - 256) + (double)v3[256] + (double)*v3) * 0.000015259022 * 0.33333334;
    if ( v4 == 0.0 )
      --v6;
    else
      v2 = v2 + (double)v7 * 0.0039215689 / v4;
    ++v7;
    ++v3;
  }
  while ( v7 < 256 );
  if ( v6 )
    result = v2 / (double)v6;
  else
    result = 1.0;
  return result;
}

//----- (0044F408) --------------------------------------------------------
void GammaController::SetGammaRamp(DDGAMMARAMP *pRamp)
{
  if (pGammaControl)
    ErrD3D(pGammaControl->SetGammaRamp(0, pRamp));
  else
    MessageBoxW(nullptr, L"Gamma control not active", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\GammaControl.cpp:120", 0);
}

//----- (0044F45B) --------------------------------------------------------
int GammaController::InitGammaRamp(DDGAMMARAMP *pRamp)
{
  unsigned __int16 *v2; // esi@1
  double v3; // st7@2
  signed __int64 v4; // qax@3
  signed int v6; // [sp+Ch] [bp-4h]@1

  v6 = 0;
  v2 = pRamp->green;
  do
  {
    v3 = (double)v6 * 0.0039215689 * this->fGamma;
    if ( v3 >= 1.0 || (v4 = (signed __int64)(v3 * 65535.0), (signed int)v4 > 65535) )
	{
		LODWORD(v4) = 65535;	
	}
    else if ( (signed int)v4 < 0 )
    {
      LODWORD(v4) = 0;
    }
	else
	{
    if ( (signed int)v4 > 65535 )
      LODWORD(v4) = 65535;
	}
    ++v6;
    v2[256] = v4;
    *v2 = v4;
    *(v2 - 256) = v4;
    ++v2;
  }
  while ( v6 < 256 );
  return v4;
}

//----- (0044F4D9) --------------------------------------------------------
void GammaController::Initialize(float gamma)
{
  GammaController *v2; // esi@1
  double v3; // st7@3
  double v4; // st6@4

  v2 = this;
  if (pVersion->pVersionInfo.dwPlatformId != VER_PLATFORM_WIN32_NT ||
      pVersion->pVersionInfo.dwMajorVersion != 4)
  {
    InitializeFromSurface(pRenderer->pFrontBuffer4);
    v3 = 2.8;
    if ( gamma <= 2.8f )
      v4 = gamma;
    else
      v4 = 2.8;
    if ( v4 >= 0.1f )
    {
      if ( gamma <= 2.8f )
        v3 = gamma;
    }
    else
    {
      v3 = 0.1;
    }
    v2->fGamma = v3;
    InitGammaRamp(&v2->field_60C);
    SetGammaRamp(&v2->field_60C);
  }
}


//----- (0044F2B2) --------------------------------------------------------
bool GammaController::IsGammaSupported()
{
  bool result; // eax@3
  HRESULT v1; // eax@4

  if ( pVersion->pVersionInfo.dwPlatformId != VER_PLATFORM_WIN32_NT || pVersion->pVersionInfo.dwMajorVersion != 4 )
  {
    DDCAPS halCaps; // [sp+0h] [bp-180h]@4
    memset(&halCaps, 0, sizeof(DDCAPS));
    halCaps.dwSize = sizeof(DDCAPS);

    ErrD3D(pRenderer->pDirectDraw4->GetCaps(&halCaps, 0));
    return (halCaps.dwCaps2 >> 17) & 1;
  }
  else
    return false;
}



//----- (0044F215) --------------------------------------------------------
GammaController::GammaController()
{
  static float flt_4D8670_default_gamma = 1.0f;

  pGammaControl = nullptr;
  fGamma = flt_4D8670_default_gamma;
}



//----- (0044F24B) --------------------------------------------------------
void GammaController::InitializeFromSurface(IDirectDrawSurface4 *a2)
{
  GammaController *v2; // esi@1
  IDirectDrawGammaControl **v3; // edi@1
  IDirectDrawGammaControl *v4; // eax@1

  v2 = this;
  v3 = &this->pGammaControl;
  this->pSurface = a2;
  v4 = this->pGammaControl;
  if ( v4 )
  {
    v4->Release();
    *v3 = 0;
  }
  if ( IsGammaSupported() )
  {
    ErrD3D(a2->QueryInterface(IID_IDirectDrawGammaControl, (LPVOID *)v3));
    GetRamp();
    memcpy(&v2->field_60C, &v2->pDefaultRamp, 0x600u);
    fGamma = _44F377(&v2->field_60C);
  }
}