#pragma once
#include "VectorTypes.h"


#define GAME_FLAGS_1_01                 0x01
#define GAME_FLAGS_1_DRAW_BLV_DEBUGS    0x08
#define GAME_FLAGS_2_SATURATE_LIGHTMAPS 0x02
#define GAME_FLAGS_2_ALTER_GRAVITY      0x08
#define GAME_FLAGS_2_TARGETING_MODE     0x10
#define GAME_FLAGS_2_DRAW_BLOODSPLATS   0x20



/*  320 */
enum GAME_STATE
{
  GAME_STATE_PLAYING = 0,
  GAME_FINISHED = 1,
  GAME_STATE_2 = 2,
  GAME_STATE_LOADING_GAME = 3,
  GAME_STATE_NEWGAME_OUT_GAMEMENU = 4,
  GAME_STATE_5 = 5,
  GAME_STATE_STARTING_NEW_GAME = 6,
  GAME_STATE_GAME_QUITTING_TO_MAIN_MENU = 7,
  GAME_STATE_PARTY_DIED = 8,
  GAME_STATE_FINAL_WINDOW = 9,
  GAME_STATE_A = 10
};



/*  105 */
#pragma pack(push, 1)
struct Game__StationaryLight
{
  Vec3_float_ vPosition;
  Vec3_float_ vRGBColor;
  float flt_18;
};
#pragma pack(pop)


/*  108 */
#pragma pack(push, 1)
struct Game_stru0
{
  int field_0;
  unsigned __int8 *ptr_4;
  int field_8;
  int field_C;
};
#pragma pack(pop)


/*  279 */
#pragma pack(push, 1)
//Game_stru1
struct Game_Bloodsplat
{
  float x;
  float y;
  float z;
  float r;
  float g;
  float b;
  float radius;
};
#pragma pack(pop)


class Vis;
class LightmapBuilder;
class ParticleEngine;
class Mouse;
class Keyboard;
class ThreadWard;
class CShow;
class GammaController;
struct stru9;
struct stru10;

/*  104 */
#pragma pack(push, 1)
struct Game
{
  static Game *Create();
  static void  Destroy();

protected: Game();
protected: virtual ~Game();


public:
  void _44E904();
  bool InitializeGammaController();
  bool PickMouse(float fPickDepth, unsigned int uMouseX, unsigned int uMouseY, bool bOutline, struct Vis_SelectionFilter *sprite_filter, struct Vis_SelectionFilter *face_filter);
  bool PickKeyboard(bool bOutline, struct Vis_SelectionFilter *sprite_filter, struct Vis_SelectionFilter *face_filter);
  void OutlineSelection();
  signed int _44EC23(struct Polygon *a2, int *a3, signed int a4);
  signed int _44ED0A(struct BLVFace *a2, int *a3, signed int a4);
  bool AlterGamma_BLV(struct BLVFace *pFace, unsigned int *pColor);
  bool AlterGamma_ODM(struct ODMFace *pFace, unsigned int *pColor);
  bool draw_debug_outlines();
  bool _44EEA7();
  bool _44F07B();
  void ToggleFlags(unsigned int uMask);
  void ToggleFlags2(unsigned int uFlag);
  void _44F0FD();
  void PushStationaryLights(int a2);
  void PrepareBloodsplats();
  void Deinitialize();
  void Loop();
  void DrawParticles();
  void Draw();

  //----- (0042EB6A) --------------------------------------------------------
  struct stru6 *GetStru6() {return this->pStru6Instance;}
  //----- (0042EB71) --------------------------------------------------------
  struct IndoorCameraD3D *GetIndoorCamera() {return this->pIndoorCameraD3D;}


  //void ( ***vdestructor_ptr)(Game *, bool);
  Game__StationaryLight pStationaryLights[25];
  char field_2C0[1092];
  unsigned int uNumStationaryLights;
  Game_Bloodsplat pBloodsplats[20];
  int field_938;
  int field_93C;
  int field_940;
  int field_944;
  int field_948;
  int field_94C;
  int field_950;
  int field_954;
  int field_958;
  int field_95C;
  int field_960;
  int field_964;
  int field_968;
  int field_96C;
  int field_970;
  Mouse *pMouse;
  int field_978;
  Game_stru0 stru_97C;
  char field_98C[1148];
  int uNumBloodsplats;
  int field_E0C;
  __int64 field_E10;
  int uNumStationaryLights_in_pStationaryLightsStack;
  unsigned int bGammaControlInitialized;
  unsigned int uFlags;
  unsigned int uFlags2;
  float fSaturation;
  unsigned __int64 uSomeGammaStartTime;
  __int64 uSomeGammaDeltaTime;
  void/*ThreadWard*/ *pThreadWardInstance;
  ParticleEngine *pParticleEngine;
  Mouse *pMouseInstance;
  LightmapBuilder *pLightmapBuilder;
  Vis *pVisInstance;
  stru6 *pStru6Instance;
  IndoorCameraD3D *pIndoorCameraD3D;
  stru9 *pStru9Instance;
  stru10 *pStru10Instance;
  void/*stru11*/ *pStru11Instance;
  void/*stru12*/ *pStru12Instance;
  void/*CShow*/ *pCShow;
  Keyboard *pKeyboardInstance;
  GammaController *pGammaController;
  int field_E74;
};
#pragma pack(pop)






extern Game *pGame;