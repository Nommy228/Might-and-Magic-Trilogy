#pragma once
#include <stdint.h>
#include <math.h>


//-------------------------------------------------------------------------
// Function declarations
void init_event_triggers();
void check_event_triggers();




//void ShowMM7IntroVideo_and_LoadingScreen();
unsigned int GameOverMenu(void *ecx0);

bool __fastcall sub_4075DB(int a1, int a2, int a3, struct BLVFace *face);
bool __fastcall sub_4077F1(int a1, int a2, int a3, struct ODMFace *face, struct BSPVertexBuffer *a5);
bool __fastcall sub_407A1C(int x, int z, int y, struct Vec3_int_ v); // idb
void reset_some_strus_flt_2Cs();
void  InitializeTurnBasedAnimations(void *);
double get_shading_dist_mist();
void area_of_effect__damage_evaluate();
void __fastcall sub_43A97E(unsigned int uLayingItemID, signed int a2); // idb
double __fastcall sub_43AE12(signed int a1);
void ItemDamageFromActor(unsigned int uObjID, unsigned int uActorID, struct Vec3_int_ *pVelocity);
void FindBillboardsLightLevels_BLV();
int __fastcall _43F55F_get_billboard_light_level(struct RenderBillboard *a1, int uBaseLightLevel);
int __fastcall _43F5C8_get_point_light_level_with_respect_to_lights(unsigned int uBaseLightLevel, int uSectorID, float x, float y, float z);
bool _44100D_should_alter_right_panel();
__int16 __fastcall sub_441A4E(int a1);
void LoadLevel_InitializeLevelStr();
void OnMapLeave();
void OnMapLoad();
void Level_LoadEvtAndStr(const char *pLevelName);
void __fastcall sub_4451A8_press_any_key(int a1, int a2, int a4);
void __fastcall PrepareToLoadODM(unsigned int bLoading, struct ODMRenderParams *a2);
void _461103_load_level_sub();
void MainMenu_Loop();
void ResetCursor_Palettes_LODs_Level_Audio_SFT_Windows();
void PrepareWorld(unsigned int _0_box_loading_1_fullscreen);
void Game_DeinitializeAndTerminate(int exitCode); // idb
void FinalInitialization();
void DoPrepareWorld(unsigned int bLoading, int _1_fullscreen_loading_2_box);
bool __fastcall CheckMM7CD(char c);
void SecondaryInitialization();
void CreateAsyncMouse();
void CreateAsyncKeyboard();
void MM6_Initialize(const wchar_t *pIniFilename);
void MM7Initialization();
void SetCurrentMenuID(enum MENU_STATE); // idb
enum MENU_STATE GetCurrentMenuID();
void OracleDialogue();
const char * _4B254D_SkillMasteryTeacher(int trainerInfo);
void sub_4B3E1E();
void _4B3FE5_training_dialogue(int a4);
void __fastcall DrawTextAtStatusBar(const char *Str, int a5);
__int64 GetExperienceRequiredForLevel(int a1);
void CheckBountyRespawnAndAward();
int HouseDialogPressCloseBtn();
bool __fastcall IsBModelVisible(unsigned int uModelID, int *unused);
void ODM_LoadAndInitialize(const char *pLevelFilename, struct ODMRenderParams *thisa);
unsigned int GetLevelFogColor();
int __fastcall sub_47C3D7_get_fog_specular(int a1, int a2, float a3);
int __fastcall GetActorTintColor(int max_dim, int min_dim, float distance, int a4, struct RenderBillboard *a5);
unsigned int WorldPosToGridCellX(int); // weak
unsigned int WorldPosToGridCellZ(int); // weak
int GridCellToWorldPosX(int); // weak
int GridCellToWorldPosZ(int); // weak
void sub_481ED9_MessWithODMRenderParams();
bool IsTerrainSlopeTooHigh(int pos_x, int pos_y);
int __fastcall GetTerrainHeightsAroundParty2(int a1, int a2, int *a3, int a4);
void sub_487DA9();
double GetFogDensityByTime(struct OutdoorLocation *_this);
void sub_491E3A();
void _493938_regenerate();
void init_summoned_item(struct stru351_summoned_item *_this, __int64 duration);
void _494035_timed_effects__water_walking_damage__etc();
unsigned int __fastcall _494820_training_time(unsigned int a1);
const char *GetReputationString(signed int a1);
char *BuildDialogueString(const char *lpsz, unsigned __int8 uPlayerID, struct ItemGen *a3, char *a4, int a5, __int64 *a6);
unsigned int __fastcall GetMaxMipLevels(unsigned int uDim);
void sub_4B1447_party_fine(int shopId, int stealingResult, int fineToAdd);
unsigned int GetGravityStrength();
void UpdateUserInput_and_MapSpecificStuff();
bool __fastcall _46BFFA_check_object_intercept(unsigned int uLayingItemID, signed int a2);
unsigned int __fastcall sub_46DEF2(signed int a2, unsigned int uLayingItemID);
void _46E0B2_collide_against_decorations();
int _46E44E_collide_against_faces_and_portals(unsigned int b1); // idb
int __fastcall _46E889_collide_against_bmodels(unsigned int ecx0);
int collide_against_floor(int x, int y, int z, unsigned int *pSectorID, unsigned int *pFaceID); // idb
void __fastcall _46ED8A_collide_against_sprite_objects(unsigned int _this);
int _46EF01_collision_chech_player(int a1); // idb
int _46F04E_collide_against_portals();
void UpdateObjects();
int collide_against_floor_approximate(int x, int y, int z, unsigned int *pSectorID, unsigned int *pFaceID); // idb
bool sub_47531C(int a1, int *a2, int pos_x, int pos_y, int pos_z, int dir_x, int dir_y, int dir_z, struct BLVFace *face, int a10);
bool sub_4754BF(int a1, int *a2, int X, int Y, int Z, int dir_x, int dir_y, int dir_z, struct BLVFace *face, int a10, int a11);
int sub_475665(struct BLVFace *face, int a2, __int16 a3);
bool __fastcall sub_4759C9(struct BLVFace *face, int a2, int a3, __int16 a4);
bool __fastcall sub_475D85(Vec3_int_ *a1, Vec3_int_ *a2, int *a3, struct BLVFace *a4);
bool __fastcall sub_475F30(int *a1, struct BLVFace *a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9);
void sub_4452BB();
bool sub_4465DF_check_season(int a1);
void __fastcall sub_44861E_set_texture(unsigned int uFaceCog, const char *pFilename);
void __fastcall SetDecorationSprite(uint16_t uCog, bool bHide, const char *pFileName); // idb
void __fastcall sub_44892E_set_faces_bit(int sCogNumber, int bit, int on);
void  GameUI_StatusBar_UpdateTimedString(unsigned int bForceHide); // idb
void OnTimer(int);
void __fastcall sub_448CF4_spawn_monsters(__int16 typeindex, __int16 level, int count, int x, int y, int z, int group, unsigned int uUniqueName);
bool _449B57_test_bit(unsigned __int8 *a1, __int16 a2);
void _449B7E_toggle_bit(unsigned char *pArray, __int16 a2, unsigned __int16 bToggle); // idb
void ShowStatusBarString(const char *pString, unsigned int uNumSeconds);
void ShowNothingHereStatus();
int const_2();
bool TeleportToNWCDungeon();
inline void __fastcall memset32(void *ptr, unsigned __int32 value, int count)
{
  unsigned __int32* p = (unsigned __int32 *)ptr;
  for ( int i=0; i < count; i++ )
    *p++ = value;
}

