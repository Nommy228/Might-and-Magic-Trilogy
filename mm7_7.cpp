#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "mm7_data.h"
#include "Render.h"
#include "texts.h"
#include "Party.h"

int __cdecl sub_401022()
{
  int result; // eax@1
  signed int v1; // ecx@1
  int *v2; // edx@1
/*
  result = (int)dword_4F8580;
  v1 = 60;
  v2 = &dword_4F8580[1];
  do
  {
    v2[2] = 0;
    v2[1] = 0;
    *v2 = 0;
    v2 += 3;
    --v1;
  }
  while ( v1 );
  dword_4F8580[0] = 1;
  return result;
  */

  for( int i = 0; i < 62; i++ )
  {
	  dword_4F8580[i] = 0;
  }
  dword_4F8580[0] = 1;
  return 0;
}

//----- (00423B4A) --------------------------------------------------------
void  sub_423B4A()
{
  float *v0; // eax@1
  signed int v1; // ecx@1

  v0 = &array_507D30[0].flt_2C;
  v1 = 50;
  do
  {
    *v0 = 0.0;
    v0 += 12;
    --v1;
  }
  while ( v1 );
}

/*
GUIProgressBar *__cdecl crt_construct_576E30()
{
  RGBTexture::RGBTexture(&pGameLoadingUI_ProgressBar.pLoadingBg);
  RGBTexture::RGBTexture(&pGameLoadingUI_ProgressBar.field_40);
  RGBTexture::RGBTexture(&pGameLoadingUI_ProgressBar.field_68);
  RGBTexture::RGBTexture(&pGameLoadingUI_ProgressBar.field_90);
  RGBTexture::RGBTexture(&pGameLoadingUI_ProgressBar.field_B8);
  Texture::Texture(&pGameLoadingUI_ProgressBar.field_E0);
  Texture::Texture(&pGameLoadingUI_ProgressBar.pBardata);
  Texture::Texture(&pGameLoadingUI_ProgressBar.pLoadingProgress);
  pGameLoadingUI_ProgressBar.uProgressCurrent = 0;
  pGameLoadingUI_ProgressBar.uProgressMax = 0;
  pGameLoadingUI_ProgressBar.field_9 = 0;
  pGameLoadingUI_ProgressBar.field_8 = 0;
  pGameLoadingUI_ProgressBar.uHeight = 0;
  pGameLoadingUI_ProgressBar.uWidth = 0;
  pGameLoadingUI_ProgressBar.uY = 0;
  pGameLoadingUI_ProgressBar.uX = 0;
  pGameLoadingUI_ProgressBar.field_17 = 0;
  pGameLoadingUI_ProgressBar.field_16 = 0;
  pGameLoadingUI_ProgressBar.field_15 = 0;
  pGameLoadingUI_ProgressBar.field_14 = 0;
  pGameLoadingUI_ProgressBar.field_13 = 0;
  pGameLoadingUI_ProgressBar.field_12 = 0;
  pGameLoadingUI_ProgressBar.field_11 = 0;
  pGameLoadingUI_ProgressBar.field_10 = 0;
  return &pGameLoadingUI_ProgressBar;
}
*/

void __cdecl crt_init_globals_46BD9F()
{
  pPartyActionQueue->uNumActions = 0;
}

void constructors()
{
	sub_401022();
	sub_423B4A();
	crt_init_globals_46BD9F();

	//no call to these anywhere
	uGameUIFontMain_initialize();
	uGameUIFontShadow_initialize();
	SetSomeItemsNames();
}

/*

X = called already in our code
byte = constructor byte setter

.data:004DF004                 dd offset sub_4C9A95
IOStream

.data:004DF008                 dd offset sub_4C9B06
IOStream

.data:004DF00C                 dd offset sub_401022
added

.data:004DF010                 dd offset sub_408B98
.data:004DF014                 dd offset sub_409BCC
.data:004DF018                 dd offset crt_sub_40D6EB
byte

.data:004DF01C                 dd offset crt_construct_40D707_ptr_pArcomageGame
X

.data:004DF020                 dd offset crt_sub_40E4A6
.data:004DF024                 dd offset crt_deconstruct_40E503
.data:004DF028                 dd offset crt_sub_40F760
.data:004DF02C                 dd offset crt_sub_40F7EA
.data:004DF030                 dd offset crt_sub_40F9A9
.data:004DF034                 dd offset crt_sub_410AD9
byte

.data:004DF038                 dd offset SetMoonPhaseNames
X

.data:004DF03C                 dd offset crt_sub_413FD0
byte

.data:004DF040                 dd offset j_SetMonthNames
.data:004DF044                 dd offset j_SetDayNames
.data:004DF048                 dd offset j_SetSpellSchoolNames
.data:004DF04C                 dd offset j_SetAttributeNames
X InitializeGameText

.data:004DF050                 dd offset uGameUIFontMain_initialize
.data:004DF054                 dd offset uGameUIFontShadow_initialize
added

.data:004DF058                 dd offset crt_construct_507ABC
.data:004DF05C                 dd offset crt_construct_507A94
.data:004DF060                 dd offset crt_construct_507A6C
.data:004DF064                 dd offset crt_construct_507A44
.data:004DF068                 dd offset crt_construct_507A1C
.data:004DF06C                 dd offset crt_construct_506F20
.data:004DF070                 dd offset crt_construct_414D90_ptr_506E40
X (RGBTexture constructor)

.data:004DF074                 dd offset crt_sub_423903
byte

.data:004DF078                 dd offset IndoorCamera__IndoorCamera
X

.data:004DF07C                 dd offset crt_sub_423ACD
byte

.data:004DF080                 dd offset j_reset_some_strus_flt_2Cs
deleted

.data:004DF084                 dd offset j_sub_423B4A
added

.data:004DF088                 dd offset crt_sub_4262DD
byte

.data:004DF08C                 dd offset crt_construct_pEventTimer
.data:004DF090                 dd offset crt_construct_pMiscTimer
X

.data:004DF094                 dd offset crt_sub_42640D
.data:004DF098                 dd offset crt_sub_4268C7
.data:004DF09C                 dd offset crt_sub_42692B
byte

.data:004DF0A0                 dd offset sub_426947
X initialized

.data:004DF0A4                 dd offset crt_construct_50C0C8
X CastSpellInfo

.data:004DF0A8                 dd offset crt_sub_42FBB7
byte

.data:004DF0AC                 dd offset crt_construct_50CDB4
seems unused CastSpellInfo variable

.data:004DF0B0                 dd offset sub_4361D3
.data:004DF0B4                 dd offset sub_4382A0
.data:004DF0B8                 dd offset sub_43850A
.data:004DF0BC                 dd offset crt_438AEF
.data:004DF0C0                 dd offset crt_438B7F
.data:004DF0C4                 dd offset ctr_deconstruct_43B4E0
byte

.data:004DF0C8                 dd offset crt_43B4FC
deleted, constructor for stru_51076C

.data:004DF0CC                 dd offset crt_deconstruct_43B51C
.data:004DF0D0                 dd offset crt_deconstruct_43B5F0
.data:004DF0D4                 dd offset crt_deconstruct_43B64C
byte

.data:004DF0D8                 dd offset _crt_construct_stru165
X BloodsplatContainer

.data:004DF0DC                 dd offset crt_deconstruct_43B751
.data:004DF0E0                 dd offset crt_deconstruct_43B9E3
.data:004DF0E4                 dd offset crt_deconstruct_43BC8B
.data:004DF0E8                 dd offset crt_deconstruct_43F22A
byte

.data:004DF0EC                 dd offset crt_construct_51B778
X BspRenderer

.data:004DF0F0                 dd offset crt_construct_519AB8
X LightsStack_MobileLight

.data:004DF0F4                 dd offset crt_construct_5187F0
X LightsStack_StationaryLight


.data:004DF0F8                 dd offset crt_deconstruct_440B28
.data:004DF0FC                 dd offset crt_deconstruct_440F5C
byte

.data:004DF100                 dd offset crt_construct_576E30
added but have to RECHECK

.data:004DF104                 dd offset crt_deconstruct_4437E5
byte

.data:004DF108                 dd offset sub_446219
X InitializeGameText

.data:004DF10C                 dd offset crt_construct_5773C4
unused RGBTexture

.data:004DF110                 dd offset sub_44C346
.data:004DF114                 dd offset crt_deconstruct_44C42C
.data:004DF118                 dd offset crt_deconstruct_44D49E
byte

.data:004DF11C                 dd offset _inline_SpriteFrameTable__SpriteFrameTable_global
.data:004DF120                 dd offset _inline_TextureFrameTable__TextureFrameTable_global
X

.data:004DF124                 dd offset sub_44E49B
.data:004DF128                 dd offset sub_44F1F9
.data:004DF12C                 dd offset sub_44F560
.data:004DF130                 dd offset sub_450DC2
.data:004DF134                 dd offset crt_deconstruct_45232B
byte

.data:004DF138                 dd offset crt_construct_stru193_math@5C6E00
X

.data:004DF13C                 dd offset crt_452B74
byte

.data:004DF140                 dd offset j_SetSomeItemsNames
added

.data:004DF144                 dd offset crt_deconstruct_458364
byte

.data:004DF148                 dd offset _intline_DecorationList__DecorationList_global
X

.data:004DF14C                 dd offset j_crt_construct_6836D0
X LevelDecorations

.data:004DF150                 dd offset _inline_ObjectList__ObjectList_global
X

.data:004DF154                 dd offset j_crt_construct_665230
X SpriteObjects

.data:004DF158                 dd offset j_crt_construct_5FF158
X Actors

.data:004DF15C                 dd offset _inline_MonsterList__MonsterList_global
X

.data:004DF160                 dd offset _inline_ChestList__ChestList_global
X

.data:004DF164                 dd offset j_crt_construct_5E5150
X Chests

.data:004DF168                 dd offset _inline_OverlayList__OverlayList_global
X

.data:004DF16C                 dd offset crt_construct_5E4D58
X OverlayList

.data:004DF170                 dd offset sub_459C05
byte

.data:004DF174                 dd offset sub_459C21
X KeyboardActionMapping

.data:004DF178                 dd offset crt_deconstruct_45AFBD
.data:004DF17C                 dd offset crt_deconstruct_45B0EE
.data:004DF180                 dd offset sub_45BA89
byte

.data:004DF184                 dd offset crt_call_global_ctor_45BAA5
X stru_69BD44 = "effpar03" , need RECHECK

.data:004DF188                 dd offset crt_deconstruct_45DEF7
.data:004DF18C                 dd offset crt_deconstruct_45DF53
.data:004DF190                 dd offset crt_deconstruct_45DF6F
.data:004DF194                 dd offset crt_deconstruct_45DF8B
byte

.data:004DF198                 dd offset crt_construct_ptr_6A0A60
X LODFile

.data:004DF19C                 dd offset crt_construct_ptr_6A0820
X LODFile

.data:004DF1A0                 dd offset j_crt_construct_ptr_6A0118
X RGBTexture

.data:004DF1A4                 dd offset sub_461415
.data:004DF1A8                 dd offset crt_deconstruct_4621BE
.data:004DF1AC                 dd offset crt_deconstruct_46220E
byte

.data:004DF1B0                 dd offset crt_construct_ptr_702B28
X Allocator

.data:004DF1B4                 dd offset sub_46224A
X unused

.data:004DF1B8                 dd offset crt_construct_ptr_06F0E80
X LODFile_IconsBitmaps

.data:004DF1BC                 dd offset sub_4623CF
X LODFile_Sprites

.data:004DF1C0                 dd offset crt_call_ctors__pIcons_LOD
.data:004DF1C4                 dd offset crt_call_ctors__pEvents_LOD
.data:004DF1C8                 dd offset _inline_IndoorLocation__IndoorLocation_global
X

.data:004DF1CC                 dd offset crt_init_globals_462620
X initialized

.data:004DF1D0                 dd offset crt_construct_ptr_6BE158
unused stru289

.data:004DF1D4                 dd offset j_crt_init_globals_462659
X initialized

.data:004DF1D8                 dd offset crt_construct_ptr_06BE070
X ODMRenderParams

.data:004DF1DC                 dd offset crt_init_globals_46269B
unused SW variable

.data:004DF1E0                 dd offset crt_construct_ptr_6BE048
X TileTable

.data:004DF1E4                 dd offset crt_construct_ptr_6A0DF0
X OutdoorLocation

.data:004DF1E8                 dd offset sub_466CA6
.data:004DF1EC                 dd offset crt_deconstruct_467D29
.data:004DF1F0                 dd offset sub_467E32
.data:004DF1F4                 dd offset crt_deconstruct_46AC2C
byte

.data:004DF1F8                 dd offset crt_construct_46AC48_ptr_720990
X std__string_720990 = "micon1" , need RECHECK

.data:004DF1FC                 dd offset crt_deconstruct_46BD83
byte

.data:004DF200                 dd offset crt_init_globals_46BD9F
added

.data:004DF204                 dd offset nullsub_26
X

.data:004DF208                 dd offset sub_476102
byte

.data:004DF20C                 dd offset sub_47611E
X Texture

.data:004DF210                 dd offset sub_476128
dword_7241C8 = 1 RECHECK

.data:004DF214                 dd offset sub_476133
X InitializeGameText

.data:004DF218                 dd offset sub_477595
.data:004DF21C                 dd offset sub_47836D
.data:004DF220                 dd offset crt_sub_4783D9
byte

.data:004DF224                 dd offset j__sub_4783FA_construct_global_73D150


.data:004DF228                 dd offset sub_47A368


.data:004DF22C                 dd offset sub_47C778


.data:004DF230                 dd offset sub_47CDBE


.data:004DF234                 dd offset sub_47CDDA


.data:004DF238                 dd offset sub_47F483


.data:004DF23C                 dd offset sub_47F49F


.data:004DF240                 dd offset sub_47F4B9


.data:004DF244                 dd offset sub_482A74


.data:004DF248                 dd offset sub_485F37


.data:004DF24C                 dd offset sub_486A0C


.data:004DF250                 dd offset crt_deconstruct_487DF7


.data:004DF254                 dd offset sub_488E07


.data:004DF258                 dd offset sub_489572


.data:004DF25C                 dd offset crt_deconstruct_489B44


.data:004DF260                 dd offset crt_deconstruct_489BBA


.data:004DF264                 dd offset crt_construct_489BD6_ptr_080D198


.data:004DF268                 dd offset sub_48AAA9


.data:004DF26C                 dd offset sub_48C20E


.data:004DF270                 dd offset sub_48C22A


.data:004DF274                 dd offset loc_48C234


.data:004DF278                 dd offset loc_48C243


.data:004DF27C                 dd offset loc_48C252


.data:004DF280                 dd offset loc_48C3C0


.data:004DF284                 dd offset loc_48C474


.data:004DF288                 dd offset sub_49801C


.data:004DF28C                 dd offset sub_498077


.data:004DF290                 dd offset sub_498A25


.data:004DF294                 dd offset sub_49AFE5


.data:004DF298                 dd offset crt_deconstruct_49B36B


.data:004DF29C                 dd offset crt_construct_stru187@AE5BA8



.data:004DF2A0                 dd offset crt_construct_ptr_AE5B94


.data:004DF2A4                 dd offset sub_49C594


.data:004DF2A8                 dd offset sub_49D6C8


.data:004DF2AC                 dd offset sub_49D6E4


.data:004DF2B0                 dd offset sub_49E71A


.data:004DF2B4                 dd offset sub_49E736


.data:004DF2B8                 dd offset sub_4A198F


.data:004DF2BC                 dd offset sub_4A19AB


.data:004DF2C0                 dd offset sub_4A51AF


.data:004DF2C4                 dd offset sub_4A7047


.data:004DF2C8                 dd offset sub_4A94EB


.data:004DF2CC                 dd offset sub_4A963E


.data:004DF2D0                 dd offset sub_4A965A


.data:004DF2D4                 dd offset AudioPlayer__AudioPlayer


.data:004DF2D8                 dd offset sub_4AC1AD


.data:004DF2DC                 dd offset sub_4AC662


.data:004DF2E0                 dd offset sub_4ACC1C


.data:004DF2E4                 dd offset sub_4AD369


.data:004DF2E8                 dd offset crt_deconstruct_4AD44B


.data:004DF2EC                 dd offset crt_construct_4AD467_ptr_F79D68


.data:004DF2F0                 dd offset crt_deconstruct_4AD4DE


.data:004DF2F4                 dd offset crt_construct_ptr_F7CE30


.data:004DF2F8                 dd offset sub_4B142B


.data:004DF2FC                 dd offset crt_sub_4BE344


.data:004DF300                 dd offset j_Random__ctor


.data:004DF304                 dd offset crt_sub_4BE6B5


.data:004DF308                 dd offset crt_sub_4BE6D4


.data:004DF30C                 dd offset crt_j_VideoPlayer__ctor


.data:004DF310                 dd offset sub_4C021E


.data:004DF314                 dd offset sub_4C023A


.data:004DF318                 dd offset sub_4C035A


.data:004DF31C                 dd offset sub_4C03A3


.data:004DF320                 dd offset sub_4C03BF


.data:004DF324                 dd offset sub_4C03F1


.data:004DF328                 dd offset sub_4C0423


.data:004DF32C                 dd offset sub_4C044B


.data:004DF330                 dd offset sub_4C047D


.data:004DF334                 dd offset sub_4C2A6E


.data:004DF338                 dd offset sub_4C2A8A


.data:004DF33C                 dd offset sub_4C2F7C


.data:004DF340                 dd offset unknown_libname_24 ; Microsoft VisualC 2-10/net runtime


.data:004DF34C                 dd offset ___onexitinit


.data:004DF350                 dd offset ___lconv_init


.data:004DF354                 dd offset ___initstdio


.data:004DF358                 dd offset ___initmbctable


.data:004DF35C                 dd offset sub_4D298A


.data:004DF368                 dd offset ___endstdio


.data:004DF374                 dd offset sub_4D299B


*/