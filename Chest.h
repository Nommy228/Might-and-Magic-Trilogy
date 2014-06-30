#pragma once




/*  348 */
enum CHEST_FLAGS
{
  CHEST_TRAPPED = 0x1,
  CHEST_ITEMS_PLACED = 0x2,
  CHEST_OPENED = 0x4,
};



#pragma pack(push, 1)
struct ChestDesc
{
  char pName[32];
  char uWidth;
  char uHeight;
  __int16 uTextureID;
};
#pragma pack(pop)



#pragma pack(push, 1)
struct ChestList
{
  inline ChestList():  //----- (00458438)
    uNumChests(0), pChests(nullptr)
  {}

  void ToFile();
  void FromFile(void *data_mm6, void *data_mm7, void *data_mm8);
  int FromFileTxt(const char *Args);


  unsigned int uNumChests;
  struct ChestDesc *pChests;
};
#pragma pack(pop)



/*   65 */
#pragma pack(push, 1)
struct Chest //0x14cc
{
  inline bool Initialized() const    {return (uFlags & CHEST_ITEMS_PLACED) != 0;}
  inline void SetInitialized(bool b) {if (b) uFlags |= CHEST_ITEMS_PLACED; else uFlags &= ~CHEST_ITEMS_PLACED;}
  inline bool Trapped() const        {return (uFlags & CHEST_TRAPPED) != 0;}

  static bool CanPlaceItemAt(signed int a1, int a2, signed int uChestID);
  static int CountChestItems(signed int uChestID);
  static int PutItemInChest(int a1, ItemGen *a2, signed int uChestID);
  static void PlaceItemAt(unsigned int put_cell_pos, unsigned int uItemIdx, signed int uChestID);
  static void PlaceItems(signed int uChestID);
  static bool Open(signed int uChestID);
  static void DrawChestUI(signed int uChestID);
  static void ToggleFlag(signed int uChestID, unsigned __int16 uFlag, unsigned int bToggle);
  static void ChestUI_WritePointedObjectStatusString();
  static void OnChestLeftClick();

  unsigned __int16 uChestBitmapID; //0
  unsigned __int16 uFlags; //2
  struct ItemGen igChestItems[140]; //4
  signed __int16 pInventoryIndices[140]; //0x13b4
};
#pragma pack(pop)

void __fastcall sub_420B13(int a1, int a2);


extern size_t uNumChests; // idb
extern struct ChestList *pChestList;
extern std::array<Chest, 20> pChests;