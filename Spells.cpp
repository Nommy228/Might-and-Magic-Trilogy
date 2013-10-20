#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>

#include "Spells.h"
#include "Overlays.h"
#include "LOD.h"
#include "texts.h"

#include "mm7_data.h"
#include "Party.h"
#include "Math.h"
#include "SpriteObject.h"
#include "ObjectList.h"
#include "Indoor.h"
#include "AudioPlayer.h"
#include "Actor.h"
#include "Game.h"
#include "stru6.h"


std::array<TownPortalData, 6> TownPortalList = //4ECBB8
{{
 {Vec3_int_(-5121, 2107, 1), 1536, 0, 21, 0},
 {Vec3_int_(-15148, -10240, 1473), 0, 0, 4, 0},
 {Vec3_int_(-10519, 5375, 753), 512, 0, 3, 0},
 {Vec3_int_(3114, -11055, 513), 0, 0, 10, 0},
 {Vec3_int_(-158, 7624, 1), 512, 0, 7, 0},
 {Vec3_int_(-1837, -4247, 65), 65, 0, 8, 0}
}} ;

struct SpellStats *pSpellStats;


std::array<stru324_spell, 103> stru_4E3ACC =
{{
  {10, 0},
  {1000, 0}, {1010, 0}, {1020, 0}, {1030, 0}, {1040, 0}, {1050, 0}, {1060, 0}, {1070, 0}, {1080, 0}, {1090, 0}, {1100, 0},
  {2000, 0}, {2010, 0}, {2020, 0}, {2030, 0}, {2040, 0}, {2050, 0}, {2060, 0}, {2070, 0}, {2080, 0}, {2090, 0}, {2100, 0},
  {3000, 0}, {3010, 0}, {3020, 0}, {3030, 0}, {3040, 0}, {3050, 0}, {3060, 0}, {3070, 0}, {3080, 0}, {3090, 0}, {3100, 0},
  {4000, 0}, {4010, 0}, {4020, 0}, {4030, 0}, {4040, 0}, {4050, 0}, {4060, 0}, {4070, 0}, {4080, 0}, {4090, 0}, {4100, 0},
  {5000, 0}, {5010, 0}, {5020, 0}, {5030, 0}, {5040, 0}, {5050, 0}, {5060, 0}, {5070, 0}, {5080, 0}, {5090, 0}, {5100, 0},
  {6000, 0}, {6010, 0}, {6020, 0}, {6030, 0}, {6040, 0}, {6050, 0}, {6060, 0}, {6070, 0}, {6080, 0}, {6090, 0}, {6100, 0},
  {7000, 0}, {7010, 0}, {7020, 0}, {7030, 0}, {7040, 0}, {7050, 0}, {7060, 0}, {7070, 0}, {7080, 0}, {7090, 0}, {7100, 0},
  {8000, 0}, {8010, 0}, {8020, 0}, {8030, 0}, {8040, 0}, {8050, 0}, {8060, 0}, {8070, 0}, {8080, 0}, {8090, 0}, {8100, 0},
  {9000, 0}, {9010, 0}, {9020, 0}, {9030, 0}, {9040, 0}, {9050, 0}, {9060, 0}, {9070, 0}, {9080, 0}, {9090, 0}, {9100, 0},
  {545, 0},
  {545, 0},
  {555, 0}
}};
 //9 spellbook pages  11 spells per page 9*11 =99 +1 zero struct at 0. It counted from 1!
std::array<SpellData, 100> pSpellDatas={{
					 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

					 {1, 1, 1, 1, 60, 60, 60, 40, 0, 0},//0
	                 {2, 2, 2, 2, 110, 110, 100, 90, 768, 0},
	                 {3, 3, 3, 3, 120, 120, 120, 120, 0, 0},
	                 {4, 4, 4, 4, 120, 120, 120, 120, 0, 0},
	                 {5, 5, 5, 5, 120, 120, 120, 120, 0, 0},
	                 {8, 8, 8, 8, 100, 100, 90, 80, 1536, 0},
	                 {10, 10, 10, 10, 150, 150, 150, 150, 1536, 0},
	                 {15, 15, 15, 15, 120, 120, 120, 120, 1536, 0},
	                 {20, 20, 20, 20, 100, 100, 100, 90, 264, 0},
	                 {25, 25, 25, 25, 100, 100, 100, 90, 268, 0},
	                 {30, 30, 30, 30, 90, 90, 90, 90, 3855, 0},

	                 {1, 1, 1, 0, 60, 60, 60, 60, 0, 0},  //1
	                 {2, 2, 2, 2, 120, 120, 120, 100, 0, 0},
	                 {3, 3, 3, 3, 120, 120, 120, 120, 0, 0},
	                 {4, 4, 4, 4, 110, 100, 90, 80, 258, 0},
	                 {5, 5, 5, 5,  90, 90,  70, 50, 0, 0},
	                 {8, 8, 8, 8, 120, 120, 120, 120, 0, 0},
	                 {10, 10, 10, 10, 100, 100, 90, 70, 2048, 0},
	                 {15, 15, 15, 15, 200, 200, 200, 200, 0, 0},
	                 {20, 20, 20, 20, 100, 100, 100, 90, 2570, 0},
	                 {25, 25, 25, 25, 250, 250, 250, 250, 0, 0},
	                 {30, 30, 30, 30, 90, 90, 90, 90, 276, 0},

	                 {1, 1, 1, 1, 60, 60, 60, 20, 0, 0},  //2
	                 {2, 2, 2, 2, 110, 100, 90, 70, 514, 0},
	                 {3, 3, 3, 3, 120, 120, 120, 120, 0, 0},
	                 {4, 4, 4, 4, 110, 100, 90, 80, 1024, 0},
	                 {5, 5, 5, 5, 150, 150, 150, 150, 0, 0},
	                 {8, 8, 8, 8, 200, 200, 200, 200, 0, 0},
	                 {10, 10, 10, 10, 100, 100, 90, 80, 2313, 0},
	                 {15, 15, 15, 15, 140, 140, 140, 140, 0, 0},
	                 {20, 20, 20, 20, 200, 200, 200, 200, 0, 0},
	                 {25, 25, 25, 25, 80, 80, 80, 80, 780, 0},
	                 {30, 30, 30, 30, 250, 250, 250, 250, 0, 0},

	                 {1, 1, 1, 1, 80, 80, 80, 80, 0, 0},  //3
	                 {2, 2, 2, 2, 100, 100, 100, 100, 0, 0},
	                 {3, 3, 3, 3, 120, 120, 120, 120, 0, 0},
	                 {4, 4, 4, 4, 110, 100, 90, 80, 773, 0},
	                 {5, 5, 5, 5, 120, 120, 120, 120, 0, 0},
	                 {8, 8, 8, 8, 100, 100, 90, 80, 2304, 0},
	                 {10, 10, 10, 10, 140, 140, 140, 140, 0, 0},
	                 {15, 15, 15, 15, 90, 90, 90, 80, 2048, 0},
	                 {20, 20, 20, 20, 150, 150, 150, 150, 0, 0},
	                 {25, 25, 25, 25, 100, 100, 100, 90, 276, 0},
	                 {30, 30, 30, 30, 90, 90, 90, 90, 25, 0},

	                 {1, 1, 1, 1, 100, 100, 100, 100, 0, 0},  //4
	                 {2, 2, 2, 2, 100, 100, 100, 100, 0, 0},
	                 {3, 3, 3, 3, 90, 90, 90, 90, 0, 0},
	                 {4, 4, 4, 4, 120, 120, 120, 120, 0, 0},
	                 {5, 5, 5, 5, 120, 120, 120, 120, 0, 0},
	                 {8, 8, 8, 8, 120, 120, 120, 120, 0, 0},
	                 {10, 10, 10, 10, 120, 120, 120, 120, 0, 0},
	                 {15, 15, 15, 15, 100, 100, 100, 100, 2058, 0},
	                 {20, 20, 20, 20, 240, 240, 240, 240, 0, 0},
	                 {25, 25, 25, 25, 150, 150, 150, 150, 0, 0},
	                 {30, 30, 30, 30, 1000, 1000, 1000, 1000, 0, 0},

	                 {1, 1, 1, 1, 120, 120, 120, 120, 0, 0},  //5
	                 {2, 2, 2, 2, 110, 110, 110, 110, 771, 0},
	                 {3, 3, 3, 3, 120, 120, 120, 120, 0, 0},
	                 {4, 4, 4, 4, 110, 100, 90, 80, 0, 0},
	                 {5, 5, 5, 5, 100, 100, 100, 100, 0, 0},
	                 {8, 8, 8, 8, 120, 120, 120, 120, 0, 0},
	                 {10, 10, 10, 10, 120, 120, 120, 120, 0, 0},
	                 {15, 15, 15, 15, 80, 80, 80, 80, 0, 0},
	                 {20, 20, 20, 20, 120, 120, 120, 120, 0, 0},
	                 {25, 25, 25, 25, 110, 110, 110, 100, 268, 0},
	                 {30, 30, 30, 30, 120, 120, 120, 120, 0, 0},

	                 {1, 1, 1, 1, 120, 120, 120, 120, 0, 0},  //6
	                 {2, 2, 2, 2, 100, 100, 100, 100, 0, 0},
	                 {3, 3, 3, 3, 120, 120, 120, 120, 0, 0},
	                 {4, 4, 4, 4, 110, 100, 90, 80, 520, 0},
	                 {5, 5, 5, 5, 110, 110, 110, 110, 0, 0},
	                 {8, 8, 8, 8, 120, 120, 120, 120, 0, 0},
	                 {10, 10, 10, 10, 120, 120, 120, 120, 0, 0},
	                 {15, 15, 15, 15, 120, 120, 120, 120, 0, 0},
	                 {20, 20, 20, 20, 120, 120, 120, 120, 0, 0},
	                 {25, 25, 25, 25, 110, 110, 110, 100, 1310, 0},
	                 {30, 30, 30, 30, 100, 100, 100, 100, 0, 0},

	                 {5, 5, 5, 5, 110, 100, 90, 80, 1024, 0},   //7
	                 {10, 10, 10, 10, 120, 110, 100, 90, 4112, 0},
	                 {15, 15, 15, 15, 120, 110, 100, 90, 0, 0},
	                 {20, 20, 20, 20, 160, 140, 120, 100, 0, 0},
	                 {25, 25, 25, 25, 140, 140, 140, 140, 0, 0},
	                 {30, 30, 30, 30, 500, 500, 500, 500, 0, 0},
	                 {35, 35, 35, 35, 135, 135, 120, 100, 281, 0},
	                 {40, 40, 40, 40, 500, 500, 500, 500, 0, 0},
	                 {45, 45, 45, 45, 250, 250, 250, 250, 0, 0},
	                 {50, 50, 50, 50, 150, 150, 150, 135, 5140, 0},
	                 {55, 55, 55, 55, 300, 300, 300, 300, 0, 0},

	                 {10, 10, 10, 10, 140, 140, 140, 140, 0, 0},  //8
	                 {15, 15, 15, 15, 120, 110, 100, 90, 2585, 0},
	                 {20, 20, 20, 20, 120, 100, 90, 120, 0, 0},
	                 {25, 25, 25, 25, 120, 120, 120, 120, 0, 0},
	                 {30, 30, 30, 30, 90, 90, 80, 70, 1542, 0},
	                 {35, 35, 35, 35, 120, 120, 100, 80, 0, 0},
	                 {40, 40, 40, 40, 110, 110, 110, 110, 0, 0},
	                 {45, 45, 45, 45, 200, 200, 200, 150, 0, 0},
	                 {50, 50, 50, 50, 120, 120, 120, 100, 6400, 0},
	                 {55, 55, 55, 55, 250, 250, 250, 250, 306, 0},
	                 {60, 60, 60, 60, 300, 300, 300, 300, 2073, 0}
  }};


std::array<unsigned int, 25> wand_spell_ids =
{
// 135 Wand of Fire               136 Wand of Sparks             137 Wand of Poison             138 Wand of Stunning           139 Wand of Harm
  SPELL_FIRE_FIRE_BOLT,           SPELL_AIR_SPARKS,              SPELL_WATER_POISON_SPRAY,      SPELL_EARTH_STUN,              SPELL_BODY_HARM,
// 140 Fairy Wand of Light        141 Fairy Wand of Ice          142 Fairy Wand of Lashing      143 Fairy Wand of Mind         144 Fairy Wand of Swarms
  SPELL_LIGHT_LIGHT_BOLT,         SPELL_WATER_ICE_BOLT,          SPELL_SPIRIT_SPIRIT_LASH,      SPELL_MIND_MIND_BLAST,         SPELL_EARTH_DEADLY_SWARM,
// 145 Alacorn Wand of Fireballs  146 Alacorn Wand of Acid       147 Alacorn Wand of Lightning  148 Alacorn Wand of Blades     149 Alacorn Wand of Charms
  SPELL_FIRE_FIREBALL,            SPELL_WATER_ACID_BURST,        SPELL_AIR_LIGHNING_BOLT,       SPELL_EARTH_BLADES,            SPELL_MIND_CHARM,
// 150 Arcane Wand of Blasting    151 Arcane Wand of The Fist    152 Arcane Wand of Rocks       153 Arcane Wand of Paralyzing  154 Arcane Wand of Clouds
  SPELL_WATER_ICE_BLAST,          SPELL_BODY_FLYING_FIST,        SPELL_EARTH_ROCK_BLAST,        SPELL_LIGHT_PARALYZE,          SPELL_DARK_TOXIC_CLOUD,
// 155 Mystic Wand of Implosion   156 Mystic Wand of Distortion  157 Mystic Wand of Sharpmetal  158 Mystic Wand of Shrinking   159 Mystic Wand of Incineration
  SPELL_AIR_IMPLOSION,            SPELL_EARTH_MASS_DISTORTION,   SPELL_DARK_SHARPMETAL,         SPELL_DARK_SHRINKING_RAY,      SPELL_FIRE_INCINERATE
};


std::array<std::array<unsigned char, 12>, 9> pSpellbookSpellIndices = // 4E2430
	{{  //0   1   2   3  4    5   6  7    8  9   10  11
		{0,  3,  1,  8, 11,  7,  4, 10,  6,  2,  5,  9},
		{0, 11,  2,  9,  6,  8,  5, 10,  3,  7,  1,  4},
		{0,  4,  8,  9,  1, 10,  3, 11,  7,  6,  2,  5}, 
		{0,  7, 10,  8,  2, 11,  1,  5,  3,  6,  4,  9},
		{0,  5, 10, 11,  7,  2,  8,  1,  4,  9,  3,  6},
		{0,  5,  9,  8,  3,  7,  6,  4,  1, 11,  2, 10}, 
		{0,  1,  6,  9,  3,  5,  8, 11,  7, 10,  4,  2},
		{0,  1, 10, 11,  9,  4,  3,  6,  5,  7,  8,  2},
		{0,  9,  3,  7,  1,  5,  2, 10, 11,  8,  6,  4}
}};

	std::array<std::array<struct SpellBookIconPos, 12>, 9> pIconPos={{

		 {  0,   0,  17,  13, 115,   2, 217,  15,
		 299,   6,  28, 125, 130, 133, 294, 114,
      11, 232, 134, 233, 237, 171, 296, 231},

		 {  0,   0, 19,   9, 117,   3, 206,  13,	
		 285,   7,  16, 123, 113, 101, 201, 118,
		 317, 110,  11, 230, 149, 236, 296, 234},

		 {  0,   0,  17,   9, 140,   0, 210,  34,
		 293,   5,   15,  98,  78, 121, 175, 136,
     301, 115,   15, 226, 154, 225, 272, 220},

		 { 0,    0,   7,   9, 156,   2, 277,   9,
		  11, 117, 111,  82, 180, 102, 303, 108,
		  10, 229, 120, 221, 201, 217, 296, 225},

		 {  0,   0,  18,   8,  89,  15, 192,  14,
		 292,   7,  22, 129, 125, 146, 217, 136,
		 305, 115,  22, 226, 174, 237, 290, 231},

     { 0,    0,  18,  12, 148,   9, 292,   7,
     17, 122, 121,  99, 220,  87, 293, 112,
     13, 236, 128, 213, 220, 223, 315, 223},

     {  0,   0,  23,  14, 127,   8, 204, 0,
     306,   8,  14, 115, 122, 132, 200, 116,
     293, 122,  20, 228, 154, 228, 294, 239},

     {  0,   0,  19,  14, 124,  10, 283,  12,
     8, 105, 113,  89, 190,  82, 298, 108,
     18, 181, 101, 204, 204, 203, 285, 218},

		 {  0,   0, 18,  17, 110,  16, 201,  15,
		 307,  15, 18, 148, 125, 166, 201, 123,
     275, 120, 28, 235, 217, 222, 324, 216}}};


//----- (0042EB31) --------------------------------------------------------
bool SpellBuff::NotExpired()
{
  return (signed __int64)this->uExpireTime > 0 ? true:false;
}


//----- (00458585) --------------------------------------------------------
void SpellBuff::Reset()
{
  uSkill = 0;
  uPower = 0;
  uExpireTime = 0i64;
  uCaster = 0;
  uFlags = 0;
  if (uOverlayID)
  {
    pOtherOverlayList->pOverlays[uOverlayID - 1].Reset();
    pOtherOverlayList->bRedraw = true;
    uOverlayID = 0;
  }
}

//----- (004585CA) --------------------------------------------------------
bool SpellBuff::IsBuffExpiredToTime( __int64 time_end )
    {
  if (uExpireTime && (uExpireTime < time_end))
  {
    uExpireTime = 0;
    uPower = 0;
    uSkill = 0;
    uOverlayID = 0;
    return true;
  }
  return false;
}

//----- (004584E0) --------------------------------------------------------
bool SpellBuff::Apply( signed __int64 uExpireTime, unsigned __int16 uSkillLevel, unsigned __int16 uPower, int uOverlayID, unsigned __int8 caster )
    {
  if (this->uExpireTime && (uExpireTime < this->uExpireTime))
    return false;

  this->uSkill = uSkillLevel;
  this->uPower = uPower;
  this->uExpireTime = uExpireTime;
  if (this->uOverlayID && this->uOverlayID != uOverlayID)
  {
    pOtherOverlayList->pOverlays[this->uOverlayID - 1].Reset();
    pOtherOverlayList->bRedraw = true;
    this->uOverlayID = 0;
  }
  this->uOverlayID = uOverlayID;
  this->uCaster = caster;

  return true;
}

//----- (0045384A) --------------------------------------------------------
void SpellStats::Initialize()
{
	int decode_step;
	char* test_string;
	bool break_loop;
	unsigned char c;
	unsigned int temp_str_len;
	char* tmp_pos;

	if ( pSpellsTXT_Raw )
		free(pSpellsTXT_Raw);
	pSpellsTXT_Raw = NULL;

	pSpellsTXT_Raw = (char *)pEvents_LOD->LoadRaw("spells.txt", 0);
	strtok(pSpellsTXT_Raw, "\r");
	strtok(NULL, "\r");
	for(int i=1; i<100; ++i) 
		{
		test_string=strtok(NULL, "\r")+1;
		decode_step =0;
		break_loop=false;
		do 
			{
			c = *(unsigned char*)test_string;
			temp_str_len = 0;
			while((c!='\t')&&(c>0))
				{
				++temp_str_len;
				c=test_string[temp_str_len];
				}		
			tmp_pos=test_string+temp_str_len;
			if (*tmp_pos == 0)
				break_loop = true;
			*tmp_pos = 0;
			if (temp_str_len)
				{		
				switch (decode_step)
					{	
				case 2:
					pInfos[i].pName=RemoveQuotes(test_string);
					break;
				case 3:
					pInfos[i].uSchool = SPELL_SCHOOL_NONE;
					if (!_stricmp("fire", test_string) )
						{
						pInfos[i].uSchool =SPELL_SCHOOL_FIRE;
						break;
						}
					if ( !_stricmp("air", test_string) )	
						{
						pInfos[i].uSchool = SPELL_SCHOOL_AIR;
						break;
						}
					if ( !_stricmp("water", test_string))	
						{
						pInfos[i].uSchool = SPELL_SCHOOL_WATER;
						break;
						}	
					if ( !_stricmp("earth", test_string))	
						{
						pInfos[i].uSchool = SPELL_SCHOOL_EARTH;
						break;
						}	
					if ( !_stricmp("spirit", test_string))	
						{
						pInfos[i].uSchool = SPELL_SCHOOL_SPIRIT;
						break;
						}
					if ( !_stricmp("mind", test_string))	
						{
						pInfos[i].uSchool = SPELL_SCHOOL_MIND;
						break;
						}
					if ( !_stricmp("body", test_string) )	
						{
						pInfos[i].uSchool = SPELL_SCHOOL_BODY;
						break;
						}			
					if ( !_stricmp("light", test_string) )	
						{
						pInfos[i].uSchool =  SPELL_SCHOOL_LIGHT;
						break;
						}					
					if ( !_stricmp("dark", test_string))	
						{
						pInfos[i].uSchool = SPELL_SCHOOL_DARK;
						break;
						}					

					if ( !_stricmp("magic", test_string))	
						{
						pInfos[i].uSchool = SPELL_SCHOOL_MAGIC;
						break;
						}								
					break;
				case 4:
					pInfos[i].pShortName=RemoveQuotes(test_string);
					break;
				case 5:
					pInfos[i].pDescription=RemoveQuotes(test_string);
					break;
				case 6:
					pInfos[i].pBasicSkillDesc=RemoveQuotes(test_string);
					break;
				case 7:
					pInfos[i].pExpertSkillDesc=RemoveQuotes(test_string);
					break;
				case 8:
					pInfos[i].pMasterSkillDesc=RemoveQuotes(test_string);
					break;
				case 9:
					pInfos[i].pGrandmasterSkillDesc=RemoveQuotes(test_string);
					break;
				case 10:
					int in_string_pos=0;
					unsigned char test;
					int in_str_len=strlen(test_string);
					for (int j=0; j<in_str_len; ++j)
						{
							test= tolower(test_string[j]);
							switch (test)
							{
							case 109:
								pSpellDatas[i].field_12|=1;
								break;
							case 101:
								pSpellDatas[i].field_12|=2;
								break;
							case 99:
								pSpellDatas[i].field_12|=4;
								break;
							case 120:
								pSpellDatas[i].field_12|=8;
								break;
							}					
						}
					break;
					}
				}
			else
				break_loop=true;
			++decode_step;
			test_string=tmp_pos+1;	
			} while ((decode_step<11)&&!break_loop);
			if ((i%11)==0)
				strtok(NULL, "\r");
		}

	}
//----- (00448DF8) --------------------------------------------------------
void __fastcall EventCastSpell(int uSpellID, int uSkillLevel, int uSkill, int fromx, int fromy, int fromz, int tox, int toy, int toz)//sub_448DF8
{
  int v9; // esi@1
  double v10; // st7@4
  double v11; // st6@4
  double v12; // st5@4
  double v13; // st7@6
  int v14; // ST44_4@7
  signed int v15; // ebx@9
  signed int v16; // edx@15
  char *v17; // ecx@16
  unsigned __int16 v18; // ax@20
  char *v19; // ecx@31
  int v20; // edx@35
  signed int v21; // edx@37
  char *v22; // ecx@38
  unsigned __int16 v23; // ax@41
  int i; // esi@42
  signed int v25; // edx@55
  char *v26; // ecx@56
  unsigned __int16 v27; // ax@59
  int j; // esi@60
  signed int v29; // edx@66
  char *v30; // ecx@67
  unsigned __int16 v31; // ax@70
  //Player *v32; // eax@80
  //unsigned __int16 v33; // si@85
  int v34; // eax@96
  int v35; // eax@97
  unsigned __int64 v36; // qax@99
  SpellBuff *v37; // ecx@99
  int v38; // esi@103
  signed __int64 v39; // qax@105
  int v40; // ebx@108
  int v41; // ebx@109
  int v42; // esi@111
  int v43; // ebx@111
  int v44; // eax@117
  //unsigned __int16 v45; // si@137
  unsigned __int16 v46; // [sp-8h] [bp-BCh]@99
  int v47; // [sp-4h] [bp-B8h]@35
  unsigned __int16 v48; // [sp-4h] [bp-B8h]@99
  int v49; // [sp+0h] [bp-B4h]@35
  int v50; // [sp+0h] [bp-B4h]@99
  int v51; // [sp+4h] [bp-B0h]@35
  unsigned __int8 v52; // [sp+4h] [bp-B0h]@99
  float v53; // [sp+14h] [bp-A0h]@4
  float v54; // [sp+18h] [bp-9Ch]@4
  int v55; // [sp+28h] [bp-8Ch]@7
  unsigned int yaw; // [sp+30h] [bp-84h]@7
  int pitch; // [sp+34h] [bp-80h]@7
  //SpriteObject a1; // [sp+38h] [bp-7Ch]@12
  int v59; // [sp+A8h] [bp-Ch]@1
  int v60; // [sp+ACh] [bp-8h]@1
  //int spellnum_; // [sp+B0h] [bp-4h]@1
  //signed int levela; // [sp+BCh] [bp+8h]@80
  int a6_4; // [sp+C8h] [bp+14h]@117
  float a7a; // [sp+CCh] [bp+18h]@6
  signed int a7b; // [sp+CCh] [bp+18h]@12
  int a7c; // [sp+CCh] [bp+18h]@29
  int a7d; // [sp+CCh] [bp+18h]@55
  float a8a; // [sp+D0h] [bp+1Ch]@6
  int a8b; // [sp+D0h] [bp+1Ch]@37
  int a8c; // [sp+D0h] [bp+1Ch]@55
  float toza; // [sp+D4h] [bp+20h]@6

  v9 = 0;
  v59 = uSkillLevel + 1;
  //spellnum_ = uSpellID;
  v60 = 0;
  if ( tox || toy || toz )
  {
    v10 = (double)tox - (double)fromx;
    v53 = v10;
    v11 = (double)toy - (double)fromy;
    v54 = v11;
    v12 = (double)toz;
  }
  else
  {
    v10 = (double)pParty->vPosition.x - (double)fromx;
    v53 = v10;
    v11 = (double)pParty->vPosition.y - (double)fromy;
    v54 = v11;
    v12 = (double)(pParty->vPosition.z + pParty->sEyelevel);
  }
  a7a = v12 - (double)fromz;
  toza = v11 * v11;
  a8a = v10 * v10;
  v13 = sqrt(a7a * a7a + a8a + toza);
  if ( v13 <= 1.0 )
  {
    LOBYTE(v55) = 1;
    yaw = 0;
    pitch = 0;
  }
  else
  {
    v55 = (signed __int64)v13;
    v14 = (signed __int64)sqrt(a8a + toza);
    yaw = stru_5C6E00->Atan2((signed __int64)v53, (signed __int64)v54);
    pitch = stru_5C6E00->Atan2(v14, (signed __int64)a7a);
  }
  v15 = v59;
  if ( v59 <= 0 || v59 > 4 )
    v15 = 1;
  a7b = v15;

  SpriteObject a1; // [sp+38h] [bp-7Ch]@12
  //SpriteObject::SpriteObject(&a1);

  a1.uType = stru_4E3ACC[uSpellID].uType;
  if ( uSpellID > 58 )
  {
    if ( uSpellID == 69 )
      goto LABEL_117;
    if ( uSpellID != 83 )
      return;
    v40 = v15 - 2;
    if ( v40 )
    {
      v41 = v40 - 1;
      if ( !v41 )
      {
        v42 = 14400 * uSkill;
        v43 = 4 * uSkill + 10;
        pGame->pStru6Instance->SetPlayerBuffAnim(0x53u, 0);
        pGame->pStru6Instance->SetPlayerBuffAnim(0x53u, 1u);
        pGame->pStru6Instance->SetPlayerBuffAnim(0x53u, 2u);
        pGame->pStru6Instance->SetPlayerBuffAnim(0x53u, 3u);
        v39 = (signed __int64)((double)(v42 << 7) * 0.033333335);
        v37 = &pParty->pPartyBuffs[PARTY_BUFF_DAY_OF_GODS];
        v36 = pParty->uTimePlayed + v39;
        v37->Apply(v36, a7b, v43, 0, 0);
        goto LABEL_139;
      }
      if ( v41 == 1 )
      {
        v42 = 18000 * uSkill;
        v43 = 5 * uSkill + 10;
       pGame->pStru6Instance->SetPlayerBuffAnim(0x53u, 0);
       pGame->pStru6Instance->SetPlayerBuffAnim(0x53u, 1u);
       pGame->pStru6Instance->SetPlayerBuffAnim(0x53u, 2u);
       pGame->pStru6Instance->SetPlayerBuffAnim(0x53u, 3u);
       v39 = (signed __int64)((double)(v42 << 7) * 0.033333335);
       v37 = &pParty->pPartyBuffs[PARTY_BUFF_DAY_OF_GODS];
       v36 = pParty->uTimePlayed + v39;
       v37->Apply(v36, a7b, v43, 0, 0);
       goto LABEL_139;
      }
    }
    v42 = 10800 * uSkill;
    v43 = 3 * uSkill + 10;
    pGame->pStru6Instance->SetPlayerBuffAnim(0x53u, 0);
    pGame->pStru6Instance->SetPlayerBuffAnim(0x53u, 1u);
    pGame->pStru6Instance->SetPlayerBuffAnim(0x53u, 2u);
    pGame->pStru6Instance->SetPlayerBuffAnim(0x53u, 3u);
    v39 = (signed __int64)((double)(v42 << 7) * 0.033333335);
    v37 = &pParty->pPartyBuffs[PARTY_BUFF_DAY_OF_GODS];
    v36 = pParty->uTimePlayed + v39;
    v37->Apply(v36, a7b, v43, 0, 0);
    goto LABEL_139;
  }
  if ( uSpellID != 58 )
  {
    switch ( uSpellID )
    {
      case 2:
      case 6:
      case 18:
      case 26:
      case 29:
      case 32:
      case 39:
      case 41:
        a1.stru_24.Reset();
        v16 = 0;
        a1.spell_id = uSpellID;
        a1.spell_level = uSkill;
        a1.spell_skill = v15;
        if ( (signed int)pObjectList->uNumObjects <= 0 )
        {
          v18 = 0;
          a1.uObjectDescID = v18;
          *(_QWORD *)&a1.vPosition.y = __PAIR__(fromz, fromy);
          a1.vPosition.x = fromx;
          a1.uAttributes = 16;
          a1.uSectorID = pIndoor->GetSector(fromx, fromy, fromz);
          a1.field_60_distance_related_prolly_lod = v55;
          v20 = yaw;
          a1.uSpriteFrameID = 0;
          a1.spell_caster_pid = 8000 | OBJECT_Item;
          a1.spell_target_pid = 0;
          a1.uFacing = yaw;
          a1.uSoundID = 0;
          v49 = pObjectList->pObjects[(signed __int16)a1.uObjectDescID].uSpeed;
          a1.Create(v20, pitch, v49, 0);
          goto LABEL_139;
        }
        v17 = (char *)&pObjectList->pObjects->uObjectID;
        while ( (short)a1.uType != *(short *)v17 )
        {
          ++v16;
          v17 += 56;
          if ( v16 >= (signed int)pObjectList->uNumObjects )
          {
            v18 = 0;
            a1.uObjectDescID = v18;
            *(_QWORD *)&a1.vPosition.y = __PAIR__(fromz, fromy);
            a1.vPosition.x = fromx;
            a1.uAttributes = 16;
            a1.uSectorID = pIndoor->GetSector(fromx, fromy, fromz);
            a1.field_60_distance_related_prolly_lod = v55;
            v20 = yaw;
            a1.uSpriteFrameID = 0;
            a1.spell_caster_pid = 8000 | OBJECT_Item;
            a1.spell_target_pid = 0;
            a1.uFacing = yaw;
            a1.uSoundID = 0;
            v49 = pObjectList->pObjects[(signed __int16)a1.uObjectDescID].uSpeed;
            a1.Create(v20, pitch, v49, 0);
            goto LABEL_139;
          }
        }
        v18 = v16;
        a1.uObjectDescID = v18;
        *(_QWORD *)&a1.vPosition.y = __PAIR__(fromz, fromy);
        a1.vPosition.x = fromx;
        a1.uAttributes = 16;
        a1.uSectorID = pIndoor->GetSector(fromx, fromy, fromz);
        a1.field_60_distance_related_prolly_lod = v55;
        v20 = yaw;
        a1.uSpriteFrameID = 0;
        a1.spell_caster_pid = 8000 | OBJECT_Item;
        a1.spell_target_pid = 0;
        a1.uFacing = yaw;
        a1.uSoundID = 0;
        v49 = pObjectList->pObjects[(signed __int16)a1.uObjectDescID].uSpeed;
        a1.Create(v20, pitch, v49, 0);
        goto LABEL_139;
      case 24:
        switch ( v15 )
        {
          case 1:
            v60 = 1;
            break;
          case 2:
            v60 = 3;
            break;
          case 3:
            v60 = 5;
            break;
          case 4:
            v60 = 7;
            break;
        }
        a7c = (signed int)(60 * stru_5C6E00->uIntegerDoublePi) / 360;
        if ( v60 != 1 )
        {
          a8b = a7c / (v60 - 1);
          a1.stru_24.Reset();
          v21 = 0;
          a1.spell_id = uSpellID;
          a1.spell_level = uSkill;
          a1.spell_skill = v15;
          if ( (signed int)pObjectList->uNumObjects <= 0 )
          {
            v23 = 0;
          }
          else
          {
            v22 = (char *)&pObjectList->pObjects->uObjectID;
            while ( (short)a1.uType != *(short *)v22 )
            {
              ++v21;
              v22 += 56;
              if ( v21 >= (signed int)pObjectList->uNumObjects )
              {
                v23 = 0;
                a1.uObjectDescID = v23;
                *(_QWORD *)&a1.vPosition.y = __PAIR__(fromz, fromy);
                a1.vPosition.x = fromx;
                a1.uAttributes = 16;
                a1.uSectorID = pIndoor->GetSector(fromx, fromy, fromz);
                a1.field_60_distance_related_prolly_lod = v55;
                a1.uSpriteFrameID = 0;
                a1.spell_caster_pid = 8000 | OBJECT_Item;
                a1.spell_target_pid = 4;
                a1.uSoundID = 0;
                for ( i = a7c / -2; i <= a7c / 2; i += a8b )
                {
                  a1.uFacing = i + yaw;
                  a1.Create((signed __int16)(i + (short)yaw), pitch, pObjectList->pObjects[(signed __int16)a1.uObjectDescID].uSpeed, 0);
                }
                goto LABEL_139;
              }
            }
            v23 = v21;
          }
          a1.uObjectDescID = v23;
          *(_QWORD *)&a1.vPosition.y = __PAIR__(fromz, fromy);
          a1.vPosition.x = fromx;
          a1.uAttributes = 16;
          a1.uSectorID = pIndoor->GetSector(fromx, fromy, fromz);
          a1.field_60_distance_related_prolly_lod = v55;
          a1.uSpriteFrameID = 0;
          a1.spell_caster_pid = 8000 | OBJECT_Item;
          a1.spell_target_pid = 4;
          a1.uSoundID = 0;
          for ( i = a7c / -2; i <= a7c / 2; i += a8b )
          {
            a1.uFacing = i + yaw;
            a1.Create((signed __int16)(i + (short)yaw), pitch, pObjectList->pObjects[(signed __int16)a1.uObjectDescID].uSpeed, 0);
          }
          goto LABEL_139;
        }
        a1.stru_24.Reset();
        v16 = 0;
        a1.spell_id = uSpellID;
        a1.spell_level = uSkill;
        a1.spell_skill = v15;
        if ( (signed int)pObjectList->uNumObjects <= 0 )
       {
         v18 = 0;
         a1.uObjectDescID = v18;
         *(_QWORD *)&a1.vPosition.y = __PAIR__(fromz, fromy);
         a1.vPosition.x = fromx;
         a1.uAttributes = 16;
         a1.uSectorID = pIndoor->GetSector(fromx, fromy, fromz);
         a1.field_60_distance_related_prolly_lod = v55;
         v20 = yaw;
         a1.uSpriteFrameID = 0;
         a1.spell_caster_pid = 8000 | OBJECT_Item;
         a1.spell_target_pid = 0;
         a1.uFacing = yaw;
         a1.uSoundID = 0;
         v51 = 0;
         v49 = pObjectList->pObjects[(signed __int16)a1.uObjectDescID].uSpeed;
         v47 = pitch;
         a1.Create(v20, v47, v49, v51);
         goto LABEL_139;
       }
        v19 = (char *)&pObjectList->pObjects->uObjectID;
        do
        {
          if ( (short)a1.uType == *(short *)v19 )
          {
            v18 = v16;
            a1.uObjectDescID = v18;
            *(_QWORD *)&a1.vPosition.y = __PAIR__(fromz, fromy);
            a1.vPosition.x = fromx;
            a1.uAttributes = 16;
            a1.uSectorID = pIndoor->GetSector(fromx, fromy, fromz);
            a1.field_60_distance_related_prolly_lod = v55;
            v20 = yaw;
            a1.uSpriteFrameID = 0;
            a1.spell_caster_pid = 8000 | OBJECT_Item;
            a1.spell_target_pid = 0;
            a1.uFacing = yaw;
            a1.uSoundID = 0;
            v51 = 0;
            v49 = pObjectList->pObjects[(signed __int16)a1.uObjectDescID].uSpeed;
            v47 = pitch;
            a1.Create(v20, v47, v49, v51);
            goto LABEL_139;
          }
          ++v16;
          v19 += 56;
        }
        while ( v16 < (signed int)pObjectList->uNumObjects );
        v18 = 0;
        a1.uObjectDescID = v18;
        *(_QWORD *)&a1.vPosition.y = __PAIR__(fromz, fromy);
        a1.vPosition.x = fromx;
        a1.uAttributes = 16;
        a1.uSectorID = pIndoor->GetSector(fromx, fromy, fromz);
        a1.field_60_distance_related_prolly_lod = v55;
        v20 = yaw;
        a1.uSpriteFrameID = 0;
        a1.spell_caster_pid = 8000 | OBJECT_Item;
        a1.spell_target_pid = 0;
        a1.uFacing = yaw;
        a1.uSoundID = 0;
        v51 = 0;
        v49 = pObjectList->pObjects[(signed __int16)a1.uObjectDescID].uSpeed;
        v47 = pitch;
        a1.Create(v20, v47, v49, v51);
        goto LABEL_139;
      case 15:
        switch ( v15 )
        {
          case 1:
            v60 = 3;
            break;
          case 2:
            v60 = 5;
            break;
          case 3:
            v60 = 7;
            break;
          case 4:
            v60 = 9;
            break;
        }
        a7d = (signed int)(60 * stru_5C6E00->uIntegerDoublePi) / 360;
        a8c = (signed int)(60 * stru_5C6E00->uIntegerDoublePi) / 360 / (v60 - 1);
        a1.stru_24.Reset();
        v25 = 0;
        a1.spell_id = uSpellID;
        a1.spell_level = uSkill;
        a1.spell_skill = v15;
        if ( (signed int)pObjectList->uNumObjects <= 0 )
        {
          v27 = 0;
          a1.uObjectDescID = v27;
          *(_QWORD *)&a1.vPosition.y = __PAIR__(fromz, fromy);
          a1.vPosition.x = fromx;
          a1.uAttributes = 16;
          a1.uSectorID = pIndoor->GetSector(fromx, fromy, fromz);
          a1.field_60_distance_related_prolly_lod = v55;
          a1.uSpriteFrameID = 0;
          a1.spell_caster_pid = 8000 | OBJECT_Item;
          a1.spell_target_pid = 4;
          a1.uSoundID = 0;
          for ( j = a7d / -2; j <= a7d / 2; j += a8c )
          {
            a1.uFacing = j + yaw;
            a1.Create((signed __int16)(j + (short)yaw), pitch, pObjectList->pObjects[(signed __int16)a1.uObjectDescID].uSpeed, 0);
          }
          goto LABEL_139;
        }
        v26 = (char *)&pObjectList->pObjects->uObjectID;
        while ( (short)a1.uType != *(short *)v26 )
        {
          ++v25;
          v26 += 56;
          if ( v25 >= (signed int)pObjectList->uNumObjects )
          {
            v27 = 0;
            a1.uObjectDescID = v27;
            *(_QWORD *)&a1.vPosition.y = __PAIR__(fromz, fromy);
            a1.vPosition.x = fromx;
            a1.uAttributes = 16;
            a1.uSectorID = pIndoor->GetSector(fromx, fromy, fromz);
            a1.field_60_distance_related_prolly_lod = v55;
            a1.uSpriteFrameID = 0;
            a1.spell_caster_pid = 8000 | OBJECT_Item;
            a1.spell_target_pid = 4;
            a1.uSoundID = 0;
            for ( j = a7d / -2; j <= a7d / 2; j += a8c )
            {
              a1.uFacing = j + yaw;
              a1.Create((signed __int16)(j + (short)yaw), pitch, pObjectList->pObjects[(signed __int16)a1.uObjectDescID].uSpeed, 0);
            }
            goto LABEL_139;
          }
        }
        v27 = v25;
        a1.uObjectDescID = v27;
        *(_QWORD *)&a1.vPosition.y = __PAIR__(fromz, fromy);
        a1.vPosition.x = fromx;
        a1.uAttributes = 16;
        a1.uSectorID = pIndoor->GetSector(fromx, fromy, fromz);
        a1.field_60_distance_related_prolly_lod = v55;
        a1.uSpriteFrameID = 0;
        a1.spell_caster_pid = 8000 | OBJECT_Item;
        a1.spell_target_pid = 4;
        a1.uSoundID = 0;
        for ( j = a7d / -2; j <= a7d / 2; j += a8c )
        {
          a1.uFacing = j + yaw;
          a1.Create(
            (signed __int16)(j + (short)yaw),
            pitch,
            pObjectList->pObjects[(signed __int16)a1.uObjectDescID].uSpeed,
            0);
        }
        goto LABEL_139;
      case 43:
        if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
          return;
        a1.stru_24.Reset();
        v29 = 0;
        a1.spell_id = uSpellID;
        a1.spell_level = uSkill;
        a1.spell_skill = v15;
        if ( (signed int)pObjectList->uNumObjects <= 0 )
        {
          v31 = 0;
          a1.uObjectDescID = v31;
          *(_QWORD *)&a1.vPosition.y = __PAIR__(fromz, fromy);
          a1.vPosition.x = fromx;
          a1.uAttributes = 16;
          a1.uSectorID = pIndoor->GetSector(fromx, fromy, fromz);
          a1.field_60_distance_related_prolly_lod = v55;
          a1.uSpriteFrameID = 0;
          a1.spell_caster_pid = 8000 | OBJECT_Item;
          a1.spell_target_pid = 4;
          a1.uSoundID = 0;
          v51 = 0;
          v49 = pObjectList->pObjects[(signed __int16)a1.uObjectDescID].uSpeed;
          v20 = yaw;
          v47 = (signed int)stru_5C6E00->uIntegerHalfPi / 2;
          a1.Create(v20, v47, v49, v51);
          goto LABEL_139;
        }
        v30 = (char *)&pObjectList->pObjects->uObjectID;
        break;

      case SPELL_FIRE_HASTE:
        if ( v15 > 0 )
        {
          if ( v15 <= 2 )
          {
            v9 = 60 * (uSkill + 60);
          }
          else
          {
            if ( v15 == 3 )
            {
              v9 = 180 * (uSkill + 20);
            }
            else
            {
              if ( v15 == 4 )
                v9 = 240 * (uSkill + 15);
            }
          }
        }
        //levela = 1;
        //v32 = pParty->pPlayers;//[0].pConditions[1];
        //do
        for (uint i = 0; i < 4; ++i)
          if (pParty->pPlayers[i].IsWeak())
            return;
		//while ( v32 <= &pParty->pPlayers[3] );
        //if ( !levela )
        //  return;
        pParty->pPartyBuffs[PARTY_BUFF_HASTE].Apply(pParty->uTimePlayed + (signed int)(signed __int64)((double)(v9 * 128) * 0.033333335),
          v15,
          0,
          0,
          0);
        //v33 = spellnum_;
        pGame->pStru6Instance->SetPlayerBuffAnim(uSpellID, 0);
        pGame->pStru6Instance->SetPlayerBuffAnim(uSpellID, 1);
        pGame->pStru6Instance->SetPlayerBuffAnim(uSpellID, 2);
        pGame->pStru6Instance->SetPlayerBuffAnim(uSpellID, 3);
        goto LABEL_138;
      case 17:
      case 38:
      case 51:
        switch ( v15 )
        {
          case 1:
          case 2:
            v9 = 300 * (uSkill + 12);
            break;
          case 3:
            v9 = 900 * (uSkill + 4);
            break;
          case 4:
            v9 = 3600 * (uSkill + 1);
            break;
        }
        switch ( uSpellID )
        {
          case 17:
            v60 = 0;
            uSkill = 14;
            break;
          case 38:
            v35 = uSkill + 5;
            uSkill = 15;
            v60 = v35;
            break;
          case 51:
            v34 = uSkill + 5;
            uSkill = 9;
            v60 = v34;
            break;
        }
        pGame->pStru6Instance->SetPlayerBuffAnim(uSpellID, 0);
        pGame->pStru6Instance->SetPlayerBuffAnim(uSpellID, 1);
        pGame->pStru6Instance->SetPlayerBuffAnim(uSpellID, 2);
        pGame->pStru6Instance->SetPlayerBuffAnim(uSpellID, 3);
        v52 = 0;
        v50 = 0;
        v48 = v60;
        v46 = v15;
        v36 = pParty->uTimePlayed + (signed int)(signed __int64)((double)(v9 << 7) * 0.033333335);
        v37 = &pParty->pPartyBuffs[uSkill];
        v37->Apply(v36, v46, v48, v50, v52);
        goto LABEL_139;;
      case 8:
        if ( v15 == 2 || v15 == 3 || v15 != 4 )
          v38 = 60 * uSkill;
        else
          v38 = 600 * uSkill;
        pGame->pStru6Instance->SetPlayerBuffAnim(uSpellID, 0);
        pGame->pStru6Instance->SetPlayerBuffAnim(uSpellID, 1);
        pGame->pStru6Instance->SetPlayerBuffAnim(uSpellID, 2);
        pGame->pStru6Instance->SetPlayerBuffAnim(uSpellID, 3);
        v52 = 0;
        v50 = 0;
        v48 = uSkill;
        v46 = v15;
        v39 = (signed __int64)((double)(v38 << 7) * 0.033333335);
        v37 = &pParty->pPartyBuffs[PARTY_BUFF_IMMOLATION];
        v36 = pParty->uTimePlayed + v39;
        v37->Apply(v36, v46, v48, v50, v52);
        goto LABEL_139;
      case 3:
      case 14:
      case 25:
      case 36:
        goto LABEL_117;
      default:
        return;
    }
    while ( (short)a1.uType != *(short *)v30 )
    {
      ++v29;
      v30 += 56;
      if ( v29 >= (signed int)pObjectList->uNumObjects )
      {
        v31 = 0;
        a1.uObjectDescID = v31;
        *(_QWORD *)&a1.vPosition.y = __PAIR__(fromz, fromy);
        a1.vPosition.x = fromx;
        a1.uAttributes = 16;
        a1.uSectorID = pIndoor->GetSector(fromx, fromy, fromz);
        a1.field_60_distance_related_prolly_lod = v55;
        a1.uSpriteFrameID = 0;
        a1.spell_caster_pid = 8000 | OBJECT_Item;
        a1.spell_target_pid = 4;
        a1.uSoundID = 0;
        v51 = 0;
        v49 = pObjectList->pObjects[(signed __int16)a1.uObjectDescID].uSpeed;
        v20 = yaw;
        v47 = (signed int)stru_5C6E00->uIntegerHalfPi / 2;
        a1.Create(v20, v47, v49, v51);
        goto LABEL_139;
      }
    }
    v31 = v29;
    a1.uObjectDescID = v31;
    *(_QWORD *)&a1.vPosition.y = __PAIR__(fromz, fromy);
    a1.vPosition.x = fromx;
    a1.uAttributes = 16;
    a1.uSectorID = pIndoor->GetSector(fromx, fromy, fromz);
    a1.field_60_distance_related_prolly_lod = v55;
    a1.uSpriteFrameID = 0;
    a1.spell_caster_pid = 8000 | OBJECT_Item;
    a1.spell_target_pid = 4;
    a1.uSoundID = 0;
    v51 = 0;
    v49 = pObjectList->pObjects[(signed __int16)a1.uObjectDescID].uSpeed;
    v20 = yaw;
    v47 = (signed int)stru_5C6E00->uIntegerHalfPi / 2;
    a1.Create(v20, v47, v49, v51);
    goto LABEL_139;
  }
LABEL_117:
  v44 = uSkill;
  a6_4 = 3600 * uSkill;
  if ( v15 == 1 )
  {
    v60 = v44;
    goto LABEL_125;
  }
  if ( v15 == 2 )
  {
    v44 = 2 * uSkill;
    v60 = v44;
    goto LABEL_125;
  }
  if ( v15 == 3 )
  {
    v44 = 3 * uSkill;
    v60 = v44;
    goto LABEL_125;
  }
  if ( v15 == 4 )
  {
    v44 = 4 * uSkill;
    v60 = v44;
    goto LABEL_125;
  }
LABEL_125:
  switch ( uSpellID )
  {
    case 3:
      uSkill = 6;
      break;
    case 14:
      uSkill = 0;
      break;
    case 25:
      uSkill = 17;
      break;
    case 36:
      uSkill = 4;
      break;
    case 58:
      uSkill = 12;
      break;
    case 69:
      uSkill = 1;
      break;
  }
  //v45 = spellnum_;
  pGame->pStru6Instance->SetPlayerBuffAnim(uSpellID, 0);
  pGame->pStru6Instance->SetPlayerBuffAnim(uSpellID, 1);
  pGame->pStru6Instance->SetPlayerBuffAnim(uSpellID, 2);
  pGame->pStru6Instance->SetPlayerBuffAnim(uSpellID, 3);
  pParty->pPartyBuffs[uSkill].Apply(pParty->uTimePlayed + (signed int)(signed __int64)((double)a6_4 * 4.2666669), v15, v60, 0, 0);
  //levela = 1;
LABEL_138:
  //if ( levela )
LABEL_139:
    pAudioPlayer->PlaySound((SoundID)word_4EE088_sound_ids[uSpellID], 0, 0, fromx, fromy, 0, 0, 0);
}
//----- (00427769) --------------------------------------------------------
bool __fastcall sub_427769_spell(unsigned int uSpellID)
{
  bool result; // eax@2

  __debugbreak(); // wut is dat?
  if ( pSpellDatas[uSpellID].field_12 & 0xC )
    result = 1;
  else
    result = 0;
  return result;
}
//----- (0043AFE3) --------------------------------------------------------
int __fastcall _43AFE3_calc_spell_damage(int a1, int a2, signed int a3, int a4)
{
  int result; // eax@1
  unsigned int v5; // [sp-4h] [bp-8h]@9

  result = 0;
  if ( a1 == 7 )
  {
    if ( a3 <= 0 )
      return result;
    if ( a3 <= 2 )
    {
      v5 = 6;
    }
    else
    {
      if ( a3 == 3 )
      {
        v5 = 8;
      }
      else
      {
        if ( a3 != 4 )
          return result;
        v5 = 10;
      }
    }
    result = GetDiceResult(a2, v5);
  }
  else
  {
    if ( a1 == 44 )
      result = a4 * (LOBYTE(pSpellDatas[40].field_10) + 2 * a2) / 100;
    else
      result = *((char *)&pSpellDatas[0].field_10 + 20 * a1)
             + GetDiceResult(a2, *((char *)&pSpellDatas[0].field_10 + 20 * a1 + 1));
  }
  return result;
}