#pragma once
#include <array>




/*   44 */
#pragma pack(push, 1)
struct IconFrame
{
  char pAnimationName[12];
  char pTextureName[12];
  __int16 uAnimTime;
  __int16 uAnimLength;
  __int16 uFlags;
  unsigned __int16 uTextureID;
};
#pragma pack(pop)

/*   45 */
#pragma pack(push, 1)
struct IconFrameTable
{
  inline IconFrameTable():
    uNumIcons(0), pIcons(nullptr)
  {}

  unsigned int FindIcon(const char *pIconName);
  IconFrame *GetFrame(unsigned int uIconID, unsigned int uFrameID);
  void InitializeAnimation(unsigned int uIconID);
  void ToFile();
  void FromFile(void *data_mm6, void *data_mm7, void *data_mm8);
  int FromFileTxt(const char *Args);
  int GetIconAnimLength(unsigned int uIconID);

  unsigned int uNumIcons;
  struct IconFrame *pIcons;
};
#pragma pack(pop)












/*   76 */
#pragma pack(push, 1)
struct UIAnimation
{
  unsigned __int16 uIconID;
  __int16 field_2;
  __int16 uAnimTime;
  __int16 uAnimLength;
  __int16 x;
  __int16 y;
  char field_C;
};
#pragma pack(pop)






extern struct IconFrameTable *pIconsFrameTable;


extern struct UIAnimation *pUIAnim_Food;
extern struct UIAnimation *pUIAnim_Gold;
extern struct UIAnimation *pUIAnum_Torchlight;
extern struct UIAnimation *pUIAnim_WizardEye;

extern std::array<struct UIAnimation *, 4> pUIAnims;
