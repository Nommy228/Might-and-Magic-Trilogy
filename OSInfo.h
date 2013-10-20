#pragma once
#include "OSAPI.h"

/*  293 */
#pragma pack(push, 1)
struct OSVersion
{
  //----- (004AD48F) --------------------------------------------------------
  OSVersion()
  {
    bInitialized = false;
    pVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
    if (GetVersionExA(&pVersionInfo))
      bInitialized = true;
  }

  //----- (004AD4BA) --------------------------------------------------------
  virtual ~OSVersion()
  {
    bInitialized = false;
  }


  //void ( ***vdestructor_ptr)(OSVersion *, bool);
  OSVERSIONINFOA pVersionInfo;
  int bInitialized;
};
#pragma pack(pop)


extern struct OSVersion *pVersion; // idb