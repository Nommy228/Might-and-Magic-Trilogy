#pragma once

#pragma pack(push, 1)
struct StorylineRecord
{
  char* pText; //0
  char* pPageTitle; //4
  unsigned __int8 uTime; //8
  char f_9;
  char f_A;
  char f_B;
};
#pragma pack(pop)

/*  173 */  
#pragma pack(push, 1)
struct StorylineText
{
  void Initialize();
  StorylineRecord StoreLine[29];
  int field_15C;
  //int field_0;
 // int field_4[87];
};
#pragma pack(pop)

extern char *pHistoryTXT_Raw;
extern struct StorylineText *pStorylineText;