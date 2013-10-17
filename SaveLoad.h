#pragma once
#include<array>

#pragma pack(push, 1)
struct SaveFile_
	{
	char field_0[20];
	char pSaveFileName[260];
	};
#pragma pack(pop)

/*  243 */
#pragma pack(push, 1)
struct SavegameList
{
  static void Initialize(unsigned int a1);

  /*inline void Reset()
  {
    for (int i = 0; i < 20; ++i)
      field_0[i] = 0;
    for (int i = 0; i < 44; ++i)
      pSavesNames[i][0] = 0;
  }*/
  SaveFile_ pFileList[45];
  
};
#pragma pack(pop)


/*  244 */
#pragma pack(push, 1)
struct SavegameHeader
{
  char pName[20];
  char pLocationName[20];
  unsigned __int64 uWordTime;
  char field_30[52];
};
#pragma pack(pop)



extern unsigned int uNumSavegameFiles;
extern std::array<unsigned int, 45> pSavegameUsedSlots;
extern struct SavegameList  *pSavegameList;
extern std::array<SavegameHeader, 45>  pSavegameHeader;