#pragma once
#include "lib\legacy_dx\d3d.h"

#pragma pack(push, 1)
class GammaController
{
public:
  GammaController();
  inline ~GammaController() {} //----- (0044F22E)
  
  void Initialize/*_44F4D9*/(float gamma);

  void GetRamp();
  double _44F377(DDGAMMARAMP *a1);
  void SetGammaRamp(DDGAMMARAMP *pRamp);
  int InitGammaRamp(DDGAMMARAMP *pRamp);

  /*protected*/void InitializeFromSurface(struct IDirectDrawSurface4 *a2);



  void ( ***vdestructor_ptr)(GammaController *, bool);
  IDirectDrawSurface4 *pSurface;
  IDirectDrawGammaControl *pGammaControl;
  DDGAMMARAMP pDefaultRamp;
  DDGAMMARAMP field_60C;
  float fGamma;//field_C0C;
};
#pragma pack(pop)
