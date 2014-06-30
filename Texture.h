#pragma once
#include <stdio.h>
#include<array>



/*  194 */
#pragma pack(push, 1)
struct RGBTexture
{
  RGBTexture();
  void Release();
  int LoadPCXFile(const char *Filename, unsigned int a3);
  unsigned int LoadFromFILE(FILE *pFile, unsigned int mode, unsigned int bCloseFile);
  int DecodePCX(char *pPcx, unsigned __int16 *pOutPixels, unsigned int uNumPixels);
  int Load(const char *pContainer, int mode);
  int Reload(const char *pContainer);

  char pName[16];
  unsigned int uNumPixels;
  unsigned __int16 uWidth;
  unsigned __int16 uHeight;
  __int16 field_18;
  __int16 field_1A;
  __int16 field_1C;
  __int16 field_1E;
  int _allocation_flags; // & 1 - malloc, else custom allocator
  unsigned __int16 *pPixels;
};
#pragma pack(pop)



#pragma pack(push, 1)
struct Texture
{
  Texture();
  void Release();
  void *UnzipPalette();

  char pName[16];
  unsigned int uSizeOfMaxLevelOfDetail;
  unsigned int uTextureSize;
  unsigned __int16 uTextureWidth;
  unsigned __int16 uTextureHeight;
  __int16 uWidthLn2;
  __int16 uHeightLn2;
  __int16 uWidthMinus1;
  __int16 uHeightMinus1;
  short palette_id1;
  short palette_id2;
  unsigned int uDecompressedSize;
  int pBits;
  unsigned __int8 *pLevelOfDetail0_prolly_alpha_mask;
  unsigned __int8 *pLevelOfDetail1;
  unsigned __int8 *pLevelOfDetail2;
  unsigned __int8 *pLevelOfDetail3;
  unsigned __int16 *pPalette16;
  unsigned __int8 *pPalette24;
};
#pragma pack(pop)

















/*  323 */
enum TEXTURE_FRAME_TABLE_FLAGS
{
  TEXTURE_FRAME_TABLE_MORE_FRAMES = 0x1,
  TEXTURE_FRAME_TABLE_FIRST = 0x2,
};



/*   41 */
#pragma pack(push, 1)
struct TextureFrame
{
  char pTextureName[12];
  __int16 uTextureID;
  __int16 uAnimTime;
  __int16 uAnimLength;
  __int16 uFlags;
};
#pragma pack(pop)

/*   40 */
#pragma pack(push, 1)
struct TextureFrameTable
{
  //----- (0044D4C9) --------------------------------------------------------
  inline TextureFrameTable()
  {
    pTextures = 0;
    sNumTextures = 0;
  }
  int FromFileTxt(const char *Args);
  void ToFile();
  void FromFile(void *data_mm6, void *data_mm7, void *data_mm8);
  void LoadAnimationSequenceAndPalettes(signed int uIconID);
  int GetFrameTexture(int uFrameID, int time);
  unsigned int FindTextureByName(const char *Str2);


  signed int sNumTextures;
  struct TextureFrame *pTextures;
};
#pragma pack(pop)

extern struct TextureFrameTable *pTextureFrameTable;



extern std::array<struct RGBTexture, 45> pSavegameThumbnails;
extern std::array<struct Texture *, 2> pTexture_LloydBeacons;
extern struct Texture *pTexture_50635C;
extern struct Texture *pTex_book_button8_off;
extern struct Texture *pTex_book_button8_on;
extern struct Texture *pTex_book_button7_off;
extern struct Texture *pTex_book_button7_on;
extern struct Texture *pTex_book_button6_off;
extern struct Texture *pTex_book_button6_on;
extern struct Texture *pTex_book_button5_off;
extern struct Texture *pTex_book_button5_on;
extern struct Texture *pTex_book_button4_off;
extern struct Texture *pTex_book_button4_on;
extern struct Texture *pTex_book_button3_off;
extern struct Texture *pTex_book_button3_on;
extern struct Texture *pTex_book_button2_off;
extern struct Texture *pTex_book_button1_off;
extern struct Texture *pTex_book_button2_on;
extern struct Texture *pTex_book_button1_on;
extern std::array<struct Texture *, 6> pTexture_TownPortalIcons; // [0]Harmonale, [1]Pierpont, [2]Nighon, [3]Evenmorn Island, [4]Celestia, [5]The Pit

extern std::array<struct Texture *, 12> SBPageCSpellsTextureList;
extern std::array<struct Texture *, 12> SBPageSSpellsTextureList;
extern struct Texture *pSBQuickSpellBtnTextr;
extern struct Texture *pSpellBookClickCloseBtnTextr;
extern struct Texture *pSBClickQuickSpellBtnTextr;
extern struct Texture *pSpellBookCloseBtnTextr;
extern std::array<std::array<struct Texture *, 2>, 9> pTextures_tabs;
extern struct Texture *pTexture_mapbordr; // idb
extern struct Texture *pTexture_pagemask; // idb
extern std::array<struct Texture *, 9> pSpellBookPagesTextr;
extern struct Texture *pSpellBookPagesTextr_9;
extern struct Texture *pSpellBookPagesTextr_10;
extern struct Texture *pSpellBookPagesTextr_11;
extern struct Texture *pSpellBookPagesTextr_12;
extern struct Texture *pSpellBookPagesTextr_13;
extern struct Texture *pTexture_AutonotesBook;
extern struct Texture *pTexture_CurrentBook;
extern struct Texture *pTex_moon_new;
extern struct Texture *pTex_moon_4;
extern struct Texture *pTex_moon_2;
extern struct Texture *pTex_moon_2_2;
extern struct Texture *pTex_moon_ful;



extern RGBTexture stru_506E40; // weak
extern RGBTexture pTexture_PCX;


extern int uTextureID_RestUI_restb4; // weak
extern int uTextureID_RestUI_restexit; // weak
extern int uTextureID_RestUI_restb3; // weak
extern int uTextureID_RestUI_restb1; // weak
extern int uTextureID_RestUI_restb2; // weak
extern int uTextureID_RestUI_restmain; // weak
extern unsigned int uTextureID_Leather;
extern int uTextureID_ar_dn_dn; // weak
extern int uTextureID_ar_dn_up; // weak
extern int uTextureID_ar_up_dn; // weak
extern int uTextureID_ar_up_up; // weak


extern int uTextureID_507698; // weak
extern int uTextureID_50769C; // weak
extern int uTextureID_5076A0; // weak
extern int uTextureID_5076A4; // weak
extern int uTextureID_5076A8; // weak
extern int uTextureID_5076AC; // weak
extern int uTextureID_5076B0; // weak
extern int uTextureID_5076B4; // weak
extern int uTextureID_Parchment; // weak
extern unsigned int uTextureID_mhp_yel;
extern unsigned int uTextureID_mhp_red;
extern unsigned int uTextureID_mhp_grn;
extern unsigned int uTextureID_mhp_capr;
extern unsigned int uTextureID_mhp_capl;
extern unsigned int uTextureID_mhp_bd;
extern unsigned int uTextureID_BUTTDESC2;
extern unsigned int uTextureID_x_x_u;
extern unsigned int uTextureID_BUTTMAKE2;
extern unsigned int uTextureID_BUTTMAKE;
extern unsigned int uTextureID_BUTTYES2;
extern unsigned int uTextureID_x_ok_u;
extern std::array<struct Texture *, 22> pPlayerPortraits;
extern std::array<struct Texture *, 9> pTexture_IC_KNIGHT;
extern struct Texture *pTexture_MAKESKY;
extern struct Texture *pTexture_MAKETOP;
extern std::array<struct Texture *, 20> pTextures_arrowr;
extern std::array<struct Texture *, 20> pTextures_arrowl;
extern struct Texture *pTexture_presleft;
extern struct Texture *pTexture_pressrigh;
extern struct Texture *pTexture_buttminu;
extern struct Texture *pTexture_buttplus;
extern unsigned int uTextureID_Quit1;
extern unsigned int uTextureID_Resume1;
extern unsigned int uTextureID_Controls1;
extern unsigned int uTextureID_Save1;
extern unsigned int uTextureID_Load1;
extern unsigned int uTextureID_New1;
extern unsigned int uTextureID_Options;


extern unsigned int uTextureID_ib_td5_A;
extern unsigned int uTextureID_ib_td4_A;
extern unsigned int uTextureID_ib_td3_A;
extern unsigned int uTextureID_ib_td2_A;
extern unsigned int uTextureID_ib_td1_A;
extern int uTextureID_CharacterUI_InventoryBackground; // weak
extern int uTextureID_50795C; // weak


extern unsigned int uTextureID_Btn_GameSettings;


extern unsigned int uTextureID_Btn_Rest;
extern unsigned int uTextureID_Btn_CastSpell;
extern unsigned int uTextureID_Btn_ZoomIn;
extern unsigned int uTextureID_Btn_ZoomOut;
extern unsigned int uTextureID_FONTPAL;
extern unsigned int uTextureID_Btn_NPCRight;
extern unsigned int uTextureID_Btn_NPCLeft;
extern std::array<unsigned int, 8> pTextureIDs_pMapDirs;


extern unsigned int uTextureID_BarRed;
extern unsigned int uTextureID_BarYellow;
extern unsigned int uTextureID_BarGreen;
extern unsigned int uTextureID_BarBlue;


extern unsigned int uTextureID_right_panel; // weak


extern RGBTexture *pTexture_StatusBar;
extern RGBTexture *pTexture_LeftFrame;
extern RGBTexture *pTexture_TopFrame;
extern RGBTexture *pTexture_BottomFrame;
extern RGBTexture *pTexture_RightFrame;


extern unsigned int uTextureID_right_panel_loop; // weak


extern struct Texture *pTexture_Leather;
extern struct Texture *pTexture_RestUI_CurrentSkyFrame; // idb
extern struct Texture *pTexture_RestUI_CurrentHourglassFrame; // idb


extern std::array<unsigned int, 5> uTextureID_Optkb;

extern unsigned int optvid_base_texture_id;
extern unsigned int bloodsplats_texture_id;
extern unsigned int us_colored_lights_texture_id;
extern unsigned int tinting_texture_id;
extern unsigned int uTextureID_507C20; // weak
extern unsigned int uTextureID_507C24; // weak
extern std::array<unsigned int, 10> pTextureIDs_GammaPositions;
extern unsigned int not_available_bloodsplats_texture_id;
extern unsigned int not_available_us_colored_lights_texture_id;
extern unsigned int not_available_tinting_texture_id;


struct OptionsMenuSkin
{
       OptionsMenuSkin();
  void Relaease();

  unsigned int uTextureID_Background;      // 507C60
  unsigned int uTextureID_TurnSpeed[3];    // 507C64
  unsigned int uTextureID_ArrowLeft;       // 507C70
  unsigned int uTextureID_ArrowRight;      // 507C74
  unsigned int uTextureID_unused_0;        // 507C78
  unsigned int uTextureID_unused_1;        // 507C7C
  unsigned int uTextureID_unused_2;        // 507C80
  unsigned int uTextureID_FlipOnExit;      // 507C84
  unsigned int uTextureID_SoundLevels[10]; // 507C88
  unsigned int uTextureID_AlwaysRun;       // 507CB0
  unsigned int uTextureID_WalkSound;       // 507CB4
  unsigned int uTextureID_ShowDamage;      // 507CB8
};
extern OptionsMenuSkin options_menu_skin; // 507C60


extern unsigned int uTextureID_detaliz_close_button; // weak
extern unsigned int uTextureID_MAGNIF_B; // weak
extern unsigned int uTextureID_BACKDOLL; // weak
extern unsigned int uTextureID_BACKHAND; // weak


extern struct Texture pTex_F7CE30;


extern RGBTexture stru_5773C4; // idb

extern struct stru355 stru_4E82A4;// = {0x20, 0x41, 0, 0x20, 0xFF0000, 0xFF00, 0xFF, 0xFF000000};  moved to texture.h
extern struct stru355 stru_4EFCBC;// = {0x20, 0x41, 0, 0x10, 0x7C00, 0x3E0, 0x1F, 0x8000};  moved to texture.h









struct stru355
{
  int field_0;
  int field_4;
  int field_8;
  int field_C;
  int field_10;
  int field_14;
  int field_18;
  int field_1C;
};

/*  390 */
#pragma pack(push, 1)
struct stru350
{
  stru350 *_450DDE();
  bool _450DF1(const struct stru355 *p1, const struct stru355 *p2);
  unsigned int _450F55(int a2);
  int _450FB1(int a2);
  int sub_451007_scale_image_bicubic(unsigned short *pSrc, int srcWidth, int srcHeight, int srcPitch,
                                     unsigned short *pDst, int dstWidth, int dstHeight, int dstPitch,
                                     int a9, int a10);

  struct stru355 field_0;
  struct stru355 field_20;
  int field_40;
  int field_44;
  int field_48;
  int field_4C;
  int field_50;
  int field_54;
  int field_58;
  int field_5C;
};
#pragma pack(pop)
