#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>

#include "Items.h"
#include "MapInfo.h"
#include "GUIWindow.h"
#include "Events2D.h"
#include "Chest.h"
#include "LOD.h"
#include "Monsters.h"
#include "Party.h"
#include "FactionTable.h"
#include "StorylineTextTable.h"
#include "texts.h"
#include "mm7_data.h"



struct ITEM_VARIATION
	{
	unsigned __int16 treasure_level;
	unsigned __int16 item_class[4];
	};


std::array<const char, 5> uItemsAmountPerShopType={ 0, 6, 8, 12, 12};

const ITEM_VARIATION shopWeap_variation_ord[15] ={
	{ 0, { 0, 0, 0, 0 }},
	{ 1, { 23, 27, 20, 20 }},
	{ 1, { 23, 24, 28, 20 }},
	{ 2, { 23, 24, 25, 20 }},
	{ 2, { 27, 27, 26, 26 }},
	{ 4, { 24, 30, 25, 27 }},
	{ 4, { 24, 30, 25, 27 }},
	{ 3, { 30, 24, 20, 20 }},
	{ 2, { 20, 20, 20, 20 }},
	{ 3, { 27, 27, 26, 26 }},
	{ 3, { 28, 28, 25, 25 }},
	{ 2, { 23, 23, 24, 24 }},
	{ 3, { 23, 23, 26, 26 }},
	{ 2, { 30, 26, 26, 26 }},
	{ 2, { 28, 25, 28, 29 }}};

const ITEM_VARIATION shopArmr_variation_ord[28] ={
	{ 1, { 35, 35, 38, 38 }},
	{ 1, { 31, 31, 31, 34 }},
	{ 1, { 35, 35, 38, 38 }},
	{ 1, { 31, 31, 32, 34 }},
	{ 2, { 35, 35, 38, 38 }},
	{ 2, { 31, 32, 32, 33 }},
	{ 2, { 35, 35, 38, 38 }},
	{ 2, { 31, 31, 32, 32 }},
	{ 4, { 35, 35, 38, 38 }},
	{ 4, { 31, 32, 33, 34 }},
	{ 4, { 35, 35, 38, 38 }},
	{ 4, { 31, 32, 33, 34 }},
	{ 3, { 35, 35, 38, 38 }},
	{ 3, { 31, 31, 31, 31 }},
	{ 2, { 35, 35, 38, 38 }},
	{ 2, { 31, 32, 34, 34 }},
	{ 3, { 35, 35, 38, 38 }},
	{ 3, { 31, 31, 32, 32 }},
	{ 3, { 35, 35, 38, 38 }},
	{ 3, { 32, 32, 32, 33 }},
	{ 3, { 35, 35, 38, 38 }},
	{ 3, { 31, 31, 31, 32 }},
	{ 3, { 35, 35, 38, 38 }},
	{ 3, { 33, 31, 32, 34 }},
	{ 3, { 35, 35, 38, 38 }},
	{ 3, { 33, 31, 32, 34 }},
	{ 4, { 35, 35, 38, 38 }},
	{ 4, { 33, 31, 32, 34 }}};



const unsigned __int16 shopMagic_treasure_lvl[14]= {0, 1, 1, 2, 2, 4, 4, 3, 2, 2, 2, 2, 2, 2};
const unsigned __int16 shopAlch_treasure_lvl[13] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 2, 2, 2, 2};

const ITEM_VARIATION shopWeap_variation_spc[15]={
	{ 0, { 0, 0, 0, 0 }},
	{ 2, { 25, 30, 20, 20}},
	{ 2, { 23, 24, 28, 20}},
	{ 3, { 23, 24, 25, 20}},
	{ 3, { 27, 27, 26, 26}},
	{ 5, { 23, 26, 28, 27}},
	{ 5, { 23, 26, 28, 27}},
	{ 4, { 30, 24, 20, 20}},
	{ 3, { 20, 20, 20, 20}},
	{ 4, { 27, 27, 26, 26}},
	{ 4, { 28, 28, 25, 25}},
	{ 4, { 23, 23, 24, 24}},
	{ 4, { 24, 24, 27, 20}},
	{ 4, { 30, 26, 26, 26}},
	{ 4, { 28, 25, 28, 29}}};

const ITEM_VARIATION shopArmr_variation_spc[28]={
	{ 2, { 35, 35, 38, 38 }},
	{ 2, { 31, 31, 31, 34 }},
	{ 2, { 35, 35, 38, 38 }},
	{ 2, { 31, 31, 32, 34 }},
	{ 3, { 35, 35, 38, 38 }},
	{ 3, { 31, 32, 32, 33 }},
	{ 3, { 35, 35, 38, 38 }},
	{ 3, { 31, 31, 32, 32 }},
	{ 5, { 35, 35, 38, 38 }},
	{ 5, { 31, 32, 33, 34 }},
	{ 5, { 35, 35, 38, 38 }},
	{ 5, { 31, 32, 33, 34 }},
	{ 4, { 35, 35, 38, 38 }},
	{ 4, { 31, 31, 31, 31 }},
	{ 3, { 35, 35, 38, 38 }},
	{ 3, { 31, 32, 34, 34 }},
	{ 4, { 35, 35, 38, 38 }},
	{ 4, { 31, 31, 32, 33 }},
	{ 4, { 35, 35, 38, 38 }},
	{ 4, { 32, 32, 33, 34 }},
	{ 4, { 35, 35, 38, 38 }},
	{ 4, { 31, 31, 31, 32 }},
	{ 4, { 35, 35, 38, 38 }},
	{ 4, { 32, 32, 32, 32 }},
	{ 4, { 35, 35, 38, 38 }},
	{ 4, { 34, 34, 34, 34 }},
	{ 5, { 35, 35, 38, 38 }},
	{ 5, { 33, 33, 33, 33 }}
	};

const unsigned __int16 shopMagicSpc_treasure_lvl[14]  =  {0, 2, 2, 3, 3, 5, 5, 4, 3, 3, 3, 3, 3, 3};
const unsigned __int16 shopAlchSpc_treasure_lvl[13]   =  {0, 2, 2, 3, 3, 4, 4, 5, 5, 3, 2, 2, 2};


std::array< std::array<char, 14>, 7> byte_4E8168={{  //byte_4E8178
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 
    { 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
    { 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3},
    { 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4},
    { 2, 2, 2, 2, 3, 4, 4, 4, 4, 5, 5, 5, 5, 5},
    { 2, 2, 2, 2, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6},
    { 2, 2, 2, 2, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7}}};

int  sub_4BE571(int a1, int *a2, int a3, int a4);

ItemGen *ptr_50C9A4_ItemToEnchant;

struct ItemsTable *pItemsTable; // 005D29E0





//----- (00439DF3) --------------------------------------------------------
int ItemGen::_439DF3_get_additional_damage(int *damage_type, bool *draintargetHP)
	{
	*damage_type = 0;
	if ( !uItemID )
		return 0;
	UpdateTempBonus(pParty->uTimePlayed);
	if (uItemID == 501 )  //Iron Feather -sword
		{
		*damage_type = 1;
		return rand() % 10 + 6;
		}
	if (uItemID == 507 ) //Ghoulsbane  -axe
		{
		*damage_type = 0;
		return rand() % 16 + 3;
		}
	if ( uItemID == 510 ) //Ullyses  -bow
		{
		*damage_type = 2;
		return rand() % 4 + 9;
		}
	if ( uItemID == 517 ) //Old Nick -dagger
		{
		*damage_type = 8;
		return 8;
		}

	switch (uSpecEnchantmentType)
		{
	case 4:  //Adds 3-4 points of Cold damage.
		*damage_type = 2;
		return rand() % 2 + 3;
		break;
	case 5:  //Adds 6-8 points of Cold damage.
		*damage_type = 2;
		return rand() % 3 + 6;
		break;
	case 6: //Adds 9-12 points of Cold damage.
		*damage_type = 2;
		return rand() % 4 + 9;
		break;
	case 7: //Adds 2-5 points of Electrical damage.
		*damage_type = 1;
		return rand() % 4 + 2;
		break;
	case 8: //Adds 4-10 points of Electrical damage.
		*damage_type = 1;
		return rand() % 7 + 4;
		break;
	case 9: //Adds 6-15 points of Electrical damage.
		*damage_type = 1;
		return rand() % 10 + 6;
		break;
	case 10: //Adds 1-6 points of Fire damage.
		*damage_type = 0;
		return GetDiceResult(1, 6);
		break;
	case 11: //Adds 2-12 points of Fire damage.
		*damage_type = 0;
		return GetDiceResult(2, 6);
		break;
	case 12: //Adds 3-18 points of Fire damage.
		*damage_type = 0;
		return GetDiceResult(3, 6);
		break;
	case 13: //Adds 5 points of Body damage.
		*damage_type = 8;
		return 5;
		break;
	case 14: //Adds 8 points of Body damage.
		*damage_type = 8;
		return 8;
		break;
	case 15: //Adds 12 points of Body damage.
		*damage_type = 8;
		return 12;
		break;
	case 16: //Drain Hit Points from target.
	case 41: //Drain Hit Points from target and Increased Weapon speed.
		*damage_type = 10;
		*draintargetHP = true;
		return 0;
		break;
	case 46:  //Adds 10-20 points of Fire damage and +25 Might.
		*damage_type = 0;
		return rand() % 11 + 10;
		break;
	default:
		*damage_type = 0;
		return 0;

		}

	}


//----- (00402F07) --------------------------------------------------------
void ItemGen::Reset()
{
  this->uHolderPlayer = 0;
  this->uAttributes = 0;
  this->uNumCharges = 0;
  this->uSpecEnchantmentType = 0;
  this->m_enchantmentStrength = 0;
  this->uEnchantmentType = 0;
  this->uItemID = 0;
  this->uBodyAnchor = 0;
  this->uExpireTime = 0i64;
}

//----- (00458260) --------------------------------------------------------
void ItemGen::UpdateTempBonus(__int64 uTimePlayed)
{
  if ( this->uAttributes & ITEM_TEMP_BONUS )
  {
    if ( uTimePlayed > (signed __int64)this->uExpireTime )
    {
      this->uEnchantmentType = 0;
      this->uSpecEnchantmentType = 0;
      this->uAttributes = this->uAttributes&(~ITEM_TEMP_BONUS);
    }
  }
}

//----- (0045814E) --------------------------------------------------------
void ItemsTable::Release()
{
  if ( pMonstersTXT_Raw )
    free(pMonstersTXT_Raw);
  if ( pMonsterPlacementTXT_Raw )
    free(pMonsterPlacementTXT_Raw);
  if ( pSkillDescTXT_Raw )
    free(pSkillDescTXT_Raw);
  if (pSpcItemsTXT_Raw )
    free(pSpcItemsTXT_Raw);
  if ( pStdItemsTXT_Raw )
    free(pStdItemsTXT_Raw);
  if ( pRndItemsTXT_Raw )
    free(pRndItemsTXT_Raw);
  if ( pItemsTXT_Raw )
    free(pItemsTXT_Raw);
  if ( pHostileTXT_Raw )
    free(pHostileTXT_Raw);
  if ( pHistoryTXT_Raw )
    free(pHistoryTXT_Raw);
  if ( pPotionsTXT_Raw )
    free(pPotionsTXT_Raw);
  if ( pPotionNotesTXT_Raw )
    free(pPotionNotesTXT_Raw);
  pSpcItemsTXT_Raw = NULL;
  pSkillDescTXT_Raw = NULL;
  pStdItemsTXT_Raw = NULL;
  pRndItemsTXT_Raw = NULL;
  pItemsTXT_Raw = NULL;
}


//----- (00456D84) --------------------------------------------------------
void ItemsTable::Initialize()
	{
	int i,j;
	char* test_string;
	unsigned char c;
	bool break_loop;
	unsigned int temp_str_len;
	char* tmp_pos;
	int decode_step;
	int item_counter;

	pMapStats = new MapStats;
	pMapStats->Initialize();

	pMonsterStats = new MonsterStats;
	pMonsterStats->Initialize();
	pMonsterStats->InitializePlacements();

	pSpellStats = new SpellStats;
	pSpellStats->Initialize();

	LoadPotions();
	LoadPotionNotes();

	pFactionTable = new FactionTable;
	pFactionTable->Initialize();

	pStorylineText = new StorylineText;
	pStorylineText->Initialize();

	pStdItemsTXT_Raw = NULL;
	pStdItemsTXT_Raw = (char *)pEvents_LOD->LoadRaw("stditems.txt", 0);
	strtok(pStdItemsTXT_Raw, "\r");
	strtok(NULL, "\r");
	strtok(NULL, "\r");
	strtok(NULL, "\r");
	//Standard Bonuses by Group	
	for (i=0;i<24;++i)
		{
		test_string = strtok(NULL, "\r") + 1;
		break_loop = false;
		decode_step=0;
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
				case 0: 
					pEnchantments[i].pBonusStat=RemoveQuotes(test_string);
					break;
				case 1:
					pEnchantments[i].pOfName= RemoveQuotes(test_string);
					break;
				default:
					pEnchantments[i].to_item[decode_step-2]=atoi(test_string);
					}
				}
			else
				{ 
				if (decode_step)
					break_loop = true;
				}
			++decode_step;
			test_string=tmp_pos+1;
			} while ((decode_step<11)&&!break_loop);
		}

	memset(&pEnchantmentsSumm, 0, 36);
	for(i=0;i<9;++i)
		{
		for (j=0;j<24;++j)
			pEnchantmentsSumm[i]+=pEnchantments[j].to_item[i];
		}

	//Bonus range for Standard by Level
	strtok(NULL, "\r");
	strtok(NULL, "\r");
	strtok(NULL, "\r");
	strtok(NULL, "\r");
	strtok(NULL, "\r");
	for(i=0;i<6;++i) //counted from 1
		{
		test_string = strtok(NULL, "\r") + 1;
		break_loop = false;
		decode_step=0;
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
			if  (decode_step==2)
				bonus_ranges[i].minR = atoi(test_string);
			else if (decode_step==3)
				bonus_ranges[i].maxR =atoi(test_string);
			++decode_step;
			test_string=tmp_pos+1;
			} while ((decode_step<4)&&!break_loop);
		}


	pSpcItemsTXT_Raw = 0;
	pSpcItemsTXT_Raw = (char *)pEvents_LOD->LoadRaw("spcitems.txt", 0);
	strtok(pSpcItemsTXT_Raw, "\r");
	strtok(NULL, "\r");
	strtok(NULL, "\r");
	strtok(NULL, "\r");
	for (i=0;i<72;++i)
		{
		test_string = strtok(NULL, "\r") + 1;
		break_loop = false;
		decode_step=0;
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
				case 0: 
					pSpecialEnchantments[i].pBonusStatement=RemoveQuotes(test_string);
					break;
				case 1:
					pSpecialEnchantments[i].pNameAdd= RemoveQuotes(test_string);
					break;
				case 14:
					int res;
					res=atoi(test_string);
					if(!res)
						{
						++test_string; 
						while (*test_string==' ')//fix X 2 case
							++test_string; 
						res=atoi(test_string);
						}				
					pSpecialEnchantments[i].iValue=res;
					break;
				case 15:
					pSpecialEnchantments[i].iTreasureLevel=  tolower(*test_string) - 97;;
					break;
				default:
					pSpecialEnchantments[i].to_item_apply[decode_step-2]=atoi(test_string);
					}
				}
			else
				{ 
				if (decode_step)
					break_loop = true;
				}
			++decode_step;
			test_string=tmp_pos+1;
			} while ((decode_step<16)&&!break_loop);
		}

	pSpecialEnchantments_count = 71;
	memset(&pSpecialEnchantmentsSumm, 0, 96);
	for(i=0;i<12;++i)
		{
		for (j=0;j<=pSpecialEnchantments_count;++j)
			pSpecialEnchantmentsSumm[i]+=pSpecialEnchantments[j].to_item_apply[i];
		}

	Initialize2DA();

	pItemsTXT_Raw = NULL;
	pItemsTXT_Raw = (char*) pEvents_LOD->LoadRaw("items.txt", 0);
	strtok(pItemsTXT_Raw, "\r");
	strtok(NULL, "\r");
	uAllItemsCount = 0;
	item_counter = 0;
	while (true)
		{
		test_string = strtok(NULL, "\r") + 1;
		break_loop = false;
		decode_step=0;
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
				case 0: //Item #
					item_counter=atoi(test_string);
					uAllItemsCount=item_counter;
					break;
				case 1: //Pic File
					pItems[item_counter].pIconName = RemoveQuotes(test_string);
					break;
				case 2: //Name
					pItems[item_counter].pName = RemoveQuotes(test_string);
					break;
				case 3: //Value
					pItems[item_counter].uValue=atoi(test_string);
					break;
				case 4: //Equip Stat
					{
					if ( !_stricmp(test_string, "weapon") )
						{
						pItems[item_counter].uEquipType = EQUIP_OFF_HAND;
						break;
						}
					if ( !_stricmp(test_string, "weapon2") )
						{
						pItems[item_counter].uEquipType = EQUIP_MAIN_HAND;
						break;
						}
					if ( !_stricmp(test_string, "weapon1or2") )
						{
						pItems[item_counter].uEquipType = EQUIP_OFF_HAND;
						break;
						}
					if ( !(_stricmp(test_string, "missile")&&_stricmp(test_string, "bow")))
						{
						pItems[item_counter].uEquipType = EQUIP_BOW;
						break;
						}
					if ( !_stricmp(test_string, "armor") )
						{
						pItems[item_counter].uEquipType = EQUIP_ARMOUR;
						break;
						}
					if ( !_stricmp(test_string, "shield") )
						{
						pItems[item_counter].uEquipType = EQUIP_SHIELD;
						break;
						}
					if ( !_stricmp(test_string, "helm") )
						{
						pItems[item_counter].uEquipType = EQUIP_HELMET;
						break;
						}
					if ( !_stricmp(test_string, "belt") )
						{
						pItems[item_counter].uEquipType = EQUIP_BELT;
						break;
						}
					if ( !_stricmp(test_string, "cloak") )
						{
						pItems[item_counter].uEquipType = EQUIP_CLOAK;
						break;
						}
					if ( !_stricmp(test_string, "gauntlets") )
						{
						pItems[item_counter].uEquipType = EQUIP_GAUNTLETS;
						break;
						}
					if ( !_stricmp(test_string, "boots") )
						{
						pItems[item_counter].uEquipType = EQUIP_BOOTS;
						break;
						}
					if ( !_stricmp(test_string, "ring") )
						{
						pItems[item_counter].uEquipType = EQUIP_RING;
						break;
						}
					if ( !_stricmp(test_string, "amulet") )
						{
						pItems[item_counter].uEquipType = EQUIP_AMULET;
						break;
						}
					if ( !_stricmp(test_string, "weaponw") )
						{
						pItems[item_counter].uEquipType = EQUIP_WAND;
						break;
						}
					if ( !(_stricmp(test_string, "herb")&&_stricmp(test_string, "reagent")))
						{
						pItems[item_counter].uEquipType = EQUIP_REAGENT;
						break;
						}
					if ( !_stricmp(test_string, "bottle") )
						{
						pItems[item_counter].uEquipType = EQUIP_POTION;
						break;
						}
					if ( !_stricmp(test_string, "sscroll") )
						{
						pItems[item_counter].uEquipType = EQUIP_SPELL_SCROLL;
						break;
						}
					if ( !_stricmp(test_string, "book") )
						{
						pItems[item_counter].uEquipType = EQUIP_BOOK;
						break;
						}
					if ( !_stricmp(test_string, "mscroll") )
						{
						pItems[item_counter].uEquipType = EQUIP_MESSAGE_SCROLL;
						break;
						}
					if ( !_stricmp(test_string, "gold") )
						{
						pItems[item_counter].uEquipType = EQUIP_GOLD;
						break;
						}
					if ( !_stricmp(test_string, "gem") )
						{
						pItems[item_counter].uEquipType = EQUIP_GEM;
						break;
						}
					pItems[item_counter].uEquipType = EQUIP_NONE;
					break;
					}
				case 5: //Skill Group
					{
					if ( !_stricmp(test_string, "staff") )
						{
						pItems[item_counter].uSkillType = PLAYER_SKILL_STAFF;
						break;
						}
					if ( !_stricmp(test_string, "sword") )
						{
						pItems[item_counter].uSkillType = PLAYER_SKILL_SWORD;
						break;
						}
					if ( !_stricmp(test_string, "dagger") )
						{
						pItems[item_counter].uSkillType = PLAYER_SKILL_DAGGER;
						break;
						}
					if ( !_stricmp(test_string, "axe") )
						{
						pItems[item_counter].uSkillType = PLAYER_SKILL_AXE;
						break;
						}
					if ( !_stricmp(test_string, "spear") )
						{
						pItems[item_counter].uSkillType = PLAYER_SKILL_SPEAR;
						break;
						}
					if ( !_stricmp(test_string, "bow") )
						{
						pItems[item_counter].uSkillType = PLAYER_SKILL_BOW;
						break;
						}
					if ( !_stricmp(test_string, "mace") )
						{
						pItems[item_counter].uSkillType = PLAYER_SKILL_MACE;
						break;
						}
					if ( !_stricmp(test_string, "blaster") )
						{
						pItems[item_counter].uSkillType = PLAYER_SKILL_BLASTER;
						break;
						}
					if ( !_stricmp(test_string, "shield") )
						{
						pItems[item_counter].uSkillType = PLAYER_SKILL_SHIELD;
						break;
						}
					if ( !_stricmp(test_string, "leather") )
						{
						pItems[item_counter].uSkillType = PLAYER_SKILL_LEATHER;
						break;
						}
					if ( !_stricmp(test_string, "chain") )
						{
						pItems[item_counter].uSkillType = PLAYER_SKILL_CHAIN;
						break;
						}
					if ( !_stricmp(test_string, "plate") )
						{
						pItems[item_counter].uSkillType = PLAYER_SKILL_PLATE;
						break;
						}
					if ( !_stricmp(test_string, "club") )
						{
						pItems[item_counter].uSkillType = PLAYER_SKILL_CLUB;
						break;
						}
					pItems[item_counter].uSkillType = PLAYER_SKILL_MISC;
					break;
					}
				case 6: //Mod1
					{
					int ii;
					char* test_char;
					int tst_len;
					tst_len=strlen(test_string);
					pItems[item_counter].uDamageDice=0;
					pItems[item_counter].uDamageRoll=0;
					test_char=test_string;
					for (ii=0; ii<tst_len; ++ii)
						{
						if (tolower(*test_char)=='d')
							{
							*test_char=0;
							pItems[item_counter].uDamageDice=atoi(test_string);
							pItems[item_counter].uDamageRoll=atoi(test_char+1);
							*test_char='d';
							break;
							}
						++test_char;
						}
					test_char=test_string;
					if ((ii==tst_len)&&(tolower(*test_char)!='s'))
						{
						pItems[item_counter].uDamageDice=atoi(test_char);
						pItems[item_counter].uDamageRoll=1;
						}
					break;
					}
				case 7: //Mod2
					pItems[item_counter].uDamageMod=atoi(test_string);
					break;
				case 8: //material
					{
					if ( !_stricmp(test_string, "artifact") )
						{
						pItems[item_counter].uMaterial = MATERIAL_ARTEFACT;
						break;
						}
					if ( !_stricmp(test_string, "relic") )
						{
						pItems[item_counter].uMaterial = MATERIAL_RELIC;
						break;
						}
					if ( !_stricmp(test_string, "special") )
						{
						pItems[item_counter].uMaterial = MATERIAL_SPECIAL;
						break;
						}
					pItems[item_counter].uMaterial = MATERIAL_COMMON;
					break;}
				case 9:  //ID/Rep/St
					pItems[item_counter].uItemID_Rep_St=atoi(test_string);
					break;
				case 10: //Not identified name
					pItems[item_counter].pUnidentifiedName = RemoveQuotes(test_string);
					break;
				case 11: //Sprite Index
					pItems[item_counter].uSpriteID=atoi(test_string);
					break;
				case 12: //VarA
					{
					pItems[item_counter]._additional_value=0;
					pItems[item_counter]._bonus_type=0;
					if (pItems[item_counter].uMaterial==MATERIAL_SPECIAL)
						{
						for(int ii=0; ii<24; ++ii)
							{
							if (!_stricmp(test_string,pEnchantments[ii].pOfName))
								{
								pItems[item_counter]._bonus_type=ii+1;
								break;
								}
							}
						if (!pItems[item_counter]._bonus_type)
							{
							for(int ii=0; ii<72; ++ii)
								{
								if (!_stricmp(test_string,pSpecialEnchantments[ii].pNameAdd))
									{
									pItems[item_counter]._additional_value=ii+1;
									}
								}
							}
						}

					break;
					}
				case 13: //VarB
					if ((pItems[item_counter].uMaterial==MATERIAL_SPECIAL)&&(pItems[item_counter]._bonus_type))
						{
						char b_s=atoi(test_string);
						if (b_s)
							pItems[item_counter]._bonus_strength=b_s;
						else
							pItems[item_counter]._bonus_strength=1;
						}
					else
						pItems[item_counter]._bonus_strength=0;
					break;
				case 14: //Equip X
					pItems[item_counter].uEquipX=atoi(test_string);
					break;
				case 15: //Equip Y
					pItems[item_counter].uEquipY=atoi(test_string);
					break;
				case 16: //Notes
					pItems[item_counter].pDescription = RemoveQuotes(test_string);
					break;

					}
				}
			else
				{ 
				if (decode_step)
					break_loop = true;
				}
			++decode_step;
			test_string=tmp_pos+1;
			} while ((decode_step<17)&&!break_loop);
			++item_counter;
			if (item_counter>799)
				break;
		}


	pRndItemsTXT_Raw = NULL;
	uAllItemsCount = item_counter;
	pRndItemsTXT_Raw = (char *)pEvents_LOD->LoadRaw("rnditems.txt", 0);
	strtok(pRndItemsTXT_Raw, "\r");
	strtok(NULL, "\r");
	strtok(NULL, "\r");
	strtok(NULL, "\r");
	item_counter = 0;
	while (true)
		{
		test_string = strtok(NULL, "\r") + 1;
		break_loop = false;
		decode_step=0;
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
				case 0: 
					item_counter=atoi(test_string);
					break;
				case 2:
					pItems[item_counter].uChanceByTreasureLvl1=atoi(test_string);
					break;
				case 3:
					pItems[item_counter].uChanceByTreasureLvl2=atoi(test_string);
					break;
				case 4:
					pItems[item_counter].uChanceByTreasureLvl3=atoi(test_string);
					break;
				case 5:
					pItems[item_counter].uChanceByTreasureLvl4=atoi(test_string);
					break;
				case 6:
					pItems[item_counter].uChanceByTreasureLvl5=atoi(test_string);
					break;
				case 7:
					pItems[item_counter].uChanceByTreasureLvl6=atoi(test_string);
					break;
					}
				}
			else
				{ 
				if (decode_step)
					break_loop = true;
				}
			++decode_step;
			test_string=tmp_pos+1;
			} while ((decode_step<8)&&!break_loop);
			++item_counter;
			if (item_counter>618)
				break;
		}

	//ChanceByTreasureLvl Summ - to calculate chance
	memset(&uChanceByTreasureLvlSumm, 0, 24);
	for(i=0;i<6;++i)
		{
		for (j=1;j<item_counter;++j)
			uChanceByTreasureLvlSumm[i]+=pItems[j].uChanceByTreasureLvl[i];
		}

	strtok(NULL, "\r");
	strtok(NULL, "\r");
	strtok(NULL, "\r");
	strtok(NULL, "\r");
	strtok(NULL, "\r");
	for (i=0;i<3;++i)
		{
		test_string = strtok(NULL, "\r") + 1;
		break_loop = false;
		decode_step=0;
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
					switch (i)
						{
					case 0:
						uBonusChanceStandart[0]=atoi(test_string);
						break;
					case 1:
						uBonusChanceSpecial[0]=atoi(test_string);
						break;
					case 2:
						uBonusChanceWpSpecial[0]=atoi(test_string);
						break;
						}
					break;
				case 3:
					switch (i)
						{
					case 0:
						uBonusChanceStandart[1]=atoi(test_string);
						break;
					case 1:
						uBonusChanceSpecial[1]=atoi(test_string);
						break;
					case 2:
						uBonusChanceWpSpecial[1]=atoi(test_string);
						break;
						}
					break;
				case 4: 
					switch (i)
						{
					case 0:
						uBonusChanceStandart[2]=atoi(test_string);
						break;
					case 1:
						uBonusChanceSpecial[2]=atoi(test_string);
						break;
					case 2:
						uBonusChanceWpSpecial[2]=atoi(test_string);
						break;
						}
					break;
				case 5:
					switch (i)
						{
					case 0:
						uBonusChanceStandart[3]=atoi(test_string);
						break;
					case 1:
						uBonusChanceSpecial[3]=atoi(test_string);
						break;
					case 2:
						uBonusChanceWpSpecial[3]=atoi(test_string);
						break;
						}
					break;
				case 6: 
					switch (i)
						{
					case 0:
						uBonusChanceStandart[4]=atoi(test_string);
						break;
					case 1:
						uBonusChanceSpecial[4]=atoi(test_string);
						break;
					case 2:
						uBonusChanceWpSpecial[4]=atoi(test_string);
						break;
						}
					break;
				case 7:
					switch (i)
						{
					case 0:
						uBonusChanceStandart[5]=atoi(test_string);
						break;
					case 1:
						uBonusChanceSpecial[5]=atoi(test_string);
						break;
					case 2:
						uBonusChanceWpSpecial[5]=atoi(test_string);
						break;
						}
					break;
					}
				}
			else
				{ 
				if (decode_step)
					break_loop = true;
				}
			++decode_step;
			test_string=tmp_pos+1;
			} while ((decode_step<8)&&!break_loop);
		}

	if ( pRndItemsTXT_Raw )
		{
		free(pRndItemsTXT_Raw);
		pRndItemsTXT_Raw = NULL;
		}

	pSkillDescTXT_Raw = NULL;
	pSkillDescTXT_Raw = (char *)pEvents_LOD->LoadRaw("skilldes.txt", 0);
	strtok(pSkillDescTXT_Raw, "\r");
	for (i=0; i<37; ++i)
		{
		test_string = strtok(NULL, "\r") + 1;
		break_loop = false;
		decode_step=0;
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
				case 1:
					pSkillDesc[i] = RemoveQuotes(test_string);
					break;
				case 2:
					pNormalSkillDesc[i] = RemoveQuotes(test_string);
					break;
				case 3:
					pExpertSkillDesc[i] = RemoveQuotes(test_string);
					break;
				case 4:
					pMasterSkillDesc[i] = RemoveQuotes(test_string);
					break;
				case 5:
					pGrandSkillDesc[i] = RemoveQuotes(test_string);
					break;	  
					}
				}
			else
				{ 
				if (decode_step)
					break_loop = true;
				}
			++decode_step;
			test_string=tmp_pos+1;
			} while ((decode_step<6)&&!break_loop);
		}

	pStatsTXT_Raw = 0;
	pStatsTXT_Raw = (char *)pEvents_LOD->LoadRaw("stats.txt", 0);
	strtok(pStatsTXT_Raw, "\r");
	for (i=0; i<26; ++i)
		{
		test_string = strtok(NULL, "\r") + 1;
		break_loop = false;
		decode_step=0;
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
				switch (i)
					{
				case 0:
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
					pAttributeDescriptions[i] = RemoveQuotes(test_string);
					break;
				case 7:
					pHealthPointsAttributeDescription = RemoveQuotes(test_string);
					break;
				case 8:
					pArmourClassAttributeDescription = RemoveQuotes(test_string);
					break;
				case 9:
					pSpellPointsAttributeDescription = RemoveQuotes(test_string);
					break;
				case 10:
					pPlayerConditionAttributeDescription = RemoveQuotes(test_string);
					break;
				case 11:
					pFastSpellAttributeDescription = RemoveQuotes(test_string);
					break;
				case 12:
					pPlayerAgeAttributeDescription = RemoveQuotes(test_string);
					break;
				case 13:
					pPlayerLevelAttributeDescription = RemoveQuotes(test_string);
					break;
				case 14:
					pPlayerExperienceAttributeDescription = RemoveQuotes(test_string);
					break;
				case 15:
					pAttackBonusAttributeDescription = RemoveQuotes(test_string);
					break;
				case 16:
					pAttackDamageAttributeDescription = RemoveQuotes(test_string);
					break;
				case 17:
					pMissleBonusAttributeDescription = RemoveQuotes(test_string);
					break;
				case 18:
					pMissleDamageAttributeDescription = RemoveQuotes(test_string);
					break;
				case 19:
					pFireResistanceAttributeDescription = RemoveQuotes(test_string);
					break;
				case 20:
					pAirResistanceAttributeDescription = RemoveQuotes(test_string);
					break;
				case 21:
					pWaterResistanceAttributeDescription = RemoveQuotes(test_string);
					break;
				case 22:
					pEarthResistanceAttributeDescription = RemoveQuotes(test_string);
					break;
				case 23:
					pMindResistanceAttributeDescription = RemoveQuotes(test_string);
					break;
				case 24:
					pBodyResistanceAttributeDescription = RemoveQuotes(test_string);
					break;
				case 25:
					pSkillPointsAttributeDescription = RemoveQuotes(test_string);
					break;  
					}
				}
			else
				{ 
				if (decode_step)
					break_loop = true;
				}
			++decode_step;
			test_string=tmp_pos+1;
			} while ((decode_step<2)&&!break_loop);
		}


	pClassTXT_Raw = 0;
	pClassTXT_Raw = (char *)pEvents_LOD->LoadRaw("class.txt", 0);
	strtok(pClassTXT_Raw, "\r");
	for (i=0; i<36; ++i)
		{
		test_string = strtok(NULL, "\r") + 1;
		break_loop = false;
		decode_step=0;
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
				//if(i) 
					pClassDescriptions[i]=RemoveQuotes(test_string);
				}
			else
				{ 
				if (decode_step)
					break_loop = true;
				}
			++decode_step;
			test_string=tmp_pos+1;
			} while ((decode_step<2)&&!break_loop);
		}


  
  ItemGen::PopulateSpecialBonusMap();
  ItemGen::PopulateArtifactBonusMap();
  ItemGen::PopulateRegularBonusMap();
  

	}

//----- (00456D17) --------------------------------------------------------
void ItemsTable::SetSpecialBonus(ItemGen *pItem)
{
  if ( pItems[pItem->uItemID].uMaterial == MATERIAL_SPECIAL )
  {
    pItem->uEnchantmentType = pItems[pItem->uItemID]._bonus_type;
    pItem->uSpecEnchantmentType = pItems[pItem->uItemID]._additional_value;
    pItem->m_enchantmentStrength = pItems[pItem->uItemID]._bonus_strength;
  }
}

//----- (00456D43) --------------------------------------------------------
bool ItemsTable::IsMaterialSpecial(ItemGen *pItem)
{
  return this->pItems[pItem->uItemID].uMaterial == MATERIAL_SPECIAL;
}

//----- (00456D5E) --------------------------------------------------------
bool ItemsTable::IsMaterialNonCommon(ItemGen *pItem)
{
  return pItems[pItem->uItemID].uMaterial == MATERIAL_SPECIAL ||
	      pItems[pItem->uItemID].uMaterial == MATERIAL_RELIC || 
		  pItems[pItem->uItemID].uMaterial == MATERIAL_ARTEFACT;
}


//----- (00453B3C) --------------------------------------------------------
void ItemsTable::LoadPotions()
	{

	CHAR Text[90]; // [sp+Ch] [bp-6Ch]@26
	char* test_string;
	unsigned int uRow;
	unsigned int uColumn;
	unsigned __int8 potion_value;

	if ( pPotionsTXT_Raw )
		free(pPotionsTXT_Raw);
	pPotionsTXT_Raw = NULL;
	pPotionsTXT_Raw = (char *)pEvents_LOD->LoadRaw("potion.txt", 0);
	test_string = strtok(pPotionsTXT_Raw,"\t\r\n");
	while ( 1 )
		{
		if ( !test_string )
			{
			MessageBoxA(0, "Error Pre-Parsing Potion Table", "Load Error", MB_ICONHAND|MB_CANCELTRYCONTINUE);
			return;
			}
		if ( !strcmp(test_string, "222") )
			break;

		test_string = strtok(NULL, "\t\r\n");
		}
	while ( 1 )
		{
		test_string = strtok(NULL, "\t\r\n");

		if ( !test_string )
			{
			MessageBoxA(0, "Error Pre-Parsing Potion Table", "Load Error", MB_ICONHAND|MB_CANCELTRYCONTINUE);
			return;
			}
		if ( !strcmp(test_string, "222") )
			break;
		}

	for (uRow = 0;uRow < 50; ++uRow)
		{
		int skip_count;
		for (skip_count = 0;skip_count < 6; ++skip_count)
			{
			if ( !strtok(NULL, "\r\t\n") )
				break;
			}
		if ( skip_count != 6 )
			break;
		for (uColumn = 0; uColumn < 50; ++uColumn)
			{
			test_string = strtok(NULL, "\r\t\n");   
			if ( !test_string )
				break;
			potion_value = atoi(test_string);
			unsigned char c=*test_string;
			if ( !potion_value )
				{
				if ( tolower(c) == 'e' )
					potion_value = atoi(test_string + 1);
				else
					potion_value = 0;
				}
			this->potion_data[uRow][uColumn]=potion_value;
			}
		if ( uColumn != 50 )
			break;
		strtok(NULL, "\r\t\n");
		}

	if ( uRow != 50 )
		{
		wsprintfA(Text, "Error Parsing Potion Table at Row: %d Column: %d", uRow, uColumn);
		MessageBoxA(0, Text, "Parsing Error", MB_ICONHAND|MB_CANCELTRYCONTINUE);
		}
	if ( pPotionsTXT_Raw )
		{
		free(pPotionsTXT_Raw);
		pPotionsTXT_Raw = 0;
		}
	}

//----- (00453CE5) --------------------------------------------------------
void ItemsTable::LoadPotionNotes()
	{

	CHAR Text[90]; 
	char* test_string;
	unsigned int uRow;
	unsigned int uColumn;
	unsigned __int8 potion_note;

	if ( pPotionNotesTXT_Raw )
		free(pPotionNotesTXT_Raw);
	pPotionNotesTXT_Raw = NULL;
	pPotionNotesTXT_Raw = (char *)pEvents_LOD->LoadRaw("potnotes.txt", 0);
	test_string = strtok(pPotionNotesTXT_Raw ,"\t\r\n");
	while ( 1 )
		{
		if ( !test_string )
			{
			MessageBoxA(0, "Error Pre-Parsing Potion Table", "Load Error", MB_ICONHAND|MB_CANCELTRYCONTINUE);
			return;
			}	
		if ( !strcmp(test_string, "222") )
			break;
		test_string = strtok(NULL, "\t\r\n");
		}
	while ( 1 )
		{
		test_string = strtok(NULL, "\t\r\n");

		if ( !test_string )
			{
			MessageBoxA(0, "Error Pre-Parsing Potion Table", "Load Error", MB_ICONHAND|MB_CANCELTRYCONTINUE);
			return;
			}
		if ( !strcmp(test_string, "222") )
			break;
		}

	for (uRow = 0;uRow < 50; ++uRow)
		{
		int skip_count;
		for (skip_count = 0;skip_count < 6; ++skip_count)
			{
			if ( !strtok(NULL, "\r\t\n") )
				break;
			}
		if ( skip_count != 6 )
			break;
		for (uColumn = 0; uColumn < 50; ++uColumn)
			{
			test_string = strtok(NULL, "\r\t\n");   
			if ( !test_string )
				break;
			potion_note = atoi(test_string);
			unsigned char c=*test_string;
			if ( !potion_note )
				{
				if ( tolower(c) == 'e' )
					potion_note = atoi(test_string + 1);
				else
					potion_note = 0;
				}
			this->potion_note[uRow][uColumn]=potion_note;
			}
		if ( uColumn != 50 )
			break;
		strtok(NULL, "\r\t\n");
		}
	if ( uRow != 50 )
		{
		wsprintfA(Text, "Error Parsing Potion Table at Row: %d Column: %d", uRow, uColumn);
		MessageBoxA(0, Text, "Parsing Error", MB_ICONHAND|MB_CANCELTRYCONTINUE);
		}
	}


//----- (00456442) --------------------------------------------------------
unsigned int ItemGen::GetValue()
	{
	unsigned int uBaseValue; // edi@1
	unsigned int bonus;

	uBaseValue = pItemsTable->pItems[this->uItemID].uValue;
	if ( this->uAttributes & ITEM_TEMP_BONUS || pItemsTable->IsMaterialNonCommon(this) )
		return uBaseValue;
	if (uEnchantmentType )
		return uBaseValue + 100 * m_enchantmentStrength;;
	if (uSpecEnchantmentType )
		{
		bonus = pItemsTable->pSpecialEnchantments[uSpecEnchantmentType].iTreasureLevel;
		if ( bonus > 10 )
			return uBaseValue + bonus;
		else
			return uBaseValue * bonus;
		} 
	return uBaseValue;
	}

//----- (00456499) --------------------------------------------------------
const char *ItemGen::GetDisplayName()
{
  if (IsIdentified())
    return GetIdentifiedName();
  else
    return pItemsTable->pItems[uItemID].pUnidentifiedName;
}

//----- (004564B3) --------------------------------------------------------
const char *ItemGen::GetIdentifiedName()
{
  unsigned __int8 equip_type; 
  const char *player_name; 
  const char *nameModificator; 
  const char *format_str; 

  equip_type = GetItemEquipType();
  if ( (equip_type == EQUIP_REAGENT) || (equip_type == EQUIP_POTION) || (equip_type == EQUIP_GOLD) )
  {
    sprintf(item__getname_buffer.data(), "%s", pItemsTable->pItems[uItemID].pName);
    return item__getname_buffer.data();
  }
  sprintf(item__getname_buffer.data(), "%s", pItemsTable->pItems[uItemID].pName);
  if ( uItemID == ITEM_LICH_JAR_FULL )  //Lich Jar
  {
    if ( (uHolderPlayer >0 )&& (uHolderPlayer <= 4) )
      {
        player_name = pPlayers[uHolderPlayer]->pName;
        strlen(player_name);
        if ( player_name[strlen(player_name) - 1] == 's' )
          format_str = pGlobalTXT_LocalizationStrings[655]; //"%s' Jar"
        else
          format_str = pGlobalTXT_LocalizationStrings[654]; //"%s's Jar"
		sprintf(item__getname_buffer.data(), format_str, pPlayers[uHolderPlayer]->pName);
		return item__getname_buffer.data();
      }
  }
  if ( !pItemsTable->IsMaterialNonCommon(this) )
  {
    if ( uEnchantmentType )
    {
      strcat(item__getname_buffer.data(), " ");
      nameModificator = pItemsTable->pEnchantments[uEnchantmentType-1].pOfName;
    }
    else
    {
      if ( !uSpecEnchantmentType )
        return item__getname_buffer.data();
      if ( uSpecEnchantmentType == 16 //Drain Hit Points from target.
        || uSpecEnchantmentType == 39 //Double damage vs Demons.
        || uSpecEnchantmentType == 40 //Double damage vs Dragons
        || uSpecEnchantmentType == 45 //+5 Speed and Accuracy
        || uSpecEnchantmentType == 56 //+5 Might and Endurance.
        || uSpecEnchantmentType == 57 //+5 Intellect and Personality.
        || uSpecEnchantmentType == 58 //Increased Value.
        || uSpecEnchantmentType == 60 //+3 Unarmed and Dodging skills
        || uSpecEnchantmentType == 61 //+3 Stealing and Disarm skills.
        || uSpecEnchantmentType == 59  //Increased Weapon speed.
        || uSpecEnchantmentType == 63 //Double Damage vs. Elves.
        || uSpecEnchantmentType == 64 //Double Damage vs. Undead.
        || uSpecEnchantmentType == 67 //Adds 5 points of Body damage and +2 Disarm skill.
        || uSpecEnchantmentType == 68 ) //Adds 6-8 points of Cold damage and +5 Armor Class.
      {  //enchantment and name positions inverted!
        sprintf( item__getname_buffer.data(), "%s %s",
				  pItemsTable->pSpecialEnchantments[uSpecEnchantmentType-1].pNameAdd,
				  pItemsTable->pItems[uItemID].pName);
        return item__getname_buffer.data();
      }
      strcat(item__getname_buffer.data(), " ");
	  nameModificator = pItemsTable->pSpecialEnchantments[uSpecEnchantmentType-1].pNameAdd;
    }
    strcat(item__getname_buffer.data(), nameModificator);
  }
  return item__getname_buffer.data();
}


//----- (00456620) --------------------------------------------------------
void ItemsTable::GenerateItem(int treasure_level, unsigned int uTreasureType, ItemGen *out_item)
    {

    ItemsTable *v5; // edi@1
    int v6; // ebx@3
    int *v7; // ecx@33
    //int v8; // eax@34
    //int v9; // eax@39
    int current_chance; // ebx@43
    int tmp_chance; // ecx@47
    unsigned int *v12; // edx@48
    unsigned int v13; // eax@49
    signed int v14; // ebx@52
    int v15; // eax@53
    signed int v16; // eax@55
    int v17; // ebx@57
    int v18; // edx@62
    signed int v19; // ebx@70
    unsigned __int8 v20; // al@81
    int v21; // eax@84
    int v22; // ebx@85
    int v23; // eax@86
    int v24; // ebx@86
    int special_chance; // edx@86
    int v26; // edx@89
    unsigned int v27; // eax@89
    int i; // ebx@89
    unsigned int v29; // ecx@90
    int v30; // ebx@91
    int v31; // eax@91
    int v32; // ecx@91
    int v33; // eax@91
    int v34; // eax@97
    unsigned __int8 v35; // sf@97
    unsigned __int8 v36; // of@97
    int v37; // ebx@98
    int v38; // edx@99
    signed int v39; // ebx@101
    int v40; // ecx@102
    char v41; // zf@107
    char v42; // al@108
    char v43; // al@111
    int *v44; // edx@118
    int v45; // eax@120
    int v46; // edx@120
    int j; // eax@121
    unsigned int v48; // ecx@123
    int v49; // eax@123
    int v50; // eax@123
    int val_list[800]; // [sp+Ch] [bp-C88h]@33
    int total_chance; // [sp+C8Ch] [bp-8h]@33
    int v53; // [sp+C90h] [bp-4h]@1
    int v54; // [sp+C9Ch] [bp+8h]@3
    //int v55; // [sp+CA0h] [bp+Ch]@34
    signed int v56; // [sp+CA0h] [bp+Ch]@55
    int v57; // [sp+CA0h] [bp+Ch]@62
    int *v58; // [sp+CA0h] [bp+Ch]@102
    int v59; // [sp+CA0h] [bp+Ch]@123
    //signed int a2a; // [sp+CA4h] [bp+10h]@33
    int a2b; // [sp+CA4h] [bp+10h]@101
    int a2c; // [sp+CA4h] [bp+10h]@120

    v5 = this;
    if (!out_item)
        out_item = (ItemGen *)malloc(sizeof(ItemGen));
    memset(out_item, 0, sizeof(*out_item));


    v6 = treasure_level - 1;
    v54 = treasure_level - 1;
    if ( uTreasureType ) //generate known treasure type
        {
        ITEM_EQUIP_TYPE   requested_equip;
        PLAYER_SKILL_TYPE requested_skill = PLAYER_SKILL_INVALID;
        switch (uTreasureType)
            {
        case 20: requested_equip = EQUIP_OFF_HAND; break;
        case 21: requested_equip = EQUIP_ARMOUR; break;
        case 22: requested_skill = PLAYER_SKILL_MISC; break;
        case 23: requested_skill = PLAYER_SKILL_SWORD; break;
        case 24: requested_skill = PLAYER_SKILL_DAGGER; break;
        case 25: requested_skill = PLAYER_SKILL_AXE; break;
        case 26: requested_skill = PLAYER_SKILL_SPEAR; break;
        case 27: requested_skill = PLAYER_SKILL_BOW; break;
        case 28: requested_skill = PLAYER_SKILL_MACE; break;
        case 29: requested_skill = PLAYER_SKILL_CLUB; break;
        case 30: requested_skill = PLAYER_SKILL_STAFF; break;
        case 31: requested_skill = PLAYER_SKILL_LEATHER; break;
        case 32: requested_skill = PLAYER_SKILL_CHAIN; break;
        case 33: requested_skill = PLAYER_SKILL_PLATE; break;
        case 34: requested_equip = EQUIP_SHIELD; break;
        case 35: requested_equip = EQUIP_HELMET; break;
        case 36: requested_equip = EQUIP_BELT; break;
        case 37: requested_equip = EQUIP_CLOAK; break;
        case 38: requested_equip = EQUIP_GAUNTLETS; break;
        case 39: requested_equip = EQUIP_BOOTS; break;
        case 40: requested_equip = EQUIP_RING; break;
        case 41: requested_equip = EQUIP_AMULET; break;
        case 42: requested_equip = EQUIP_WAND; break;
        case 43: requested_equip = EQUIP_SPELL_SCROLL; break;
        case 44: requested_equip = EQUIP_POTION; break;
        case 45: requested_equip = EQUIP_REAGENT; break;
        case 46: requested_equip = EQUIP_GEM; break;
        default:
            __debugbreak(); // check this condition
            requested_equip = (ITEM_EQUIP_TYPE)(uTreasureType - 1);
            break;
            }
        memset(val_list, 0, sizeof(val_list));
        total_chance = 0;
        j=0;
        //a2a = 1;
        if (requested_skill == PLAYER_SKILL_INVALID)  // no skill for this item needed
            {
            for (uint i = 1; i < 500; ++i)
                {
                if (pItems[i].uEquipType == requested_equip)
                    {
                    val_list[j] = i;
                    ++j;
                    total_chance += pItems[i].uChanceByTreasureLvl[treasure_level - 1];
                    }
                }
            }
        else  //have needed skill
            {
            for (uint i = 1; i < 500; ++i)
                {
                if (pItems[i].uSkillType == requested_skill)
                    {
                    val_list[j] = i;
                    ++j;
                    total_chance += pItems[i].uChanceByTreasureLvl[treasure_level - 1];
                    }
                }
            }

        current_chance = 0;
        if ( total_chance )
            current_chance = rand() % total_chance;

        out_item->uItemID = val_list[0];
        if (!out_item->uItemID)
            out_item->uItemID = 1;

        if ( pItems[out_item->uItemID].uChanceByTreasureLvl[treasure_level - 1] < current_chance )
            {
            j=0;
            tmp_chance=pItems[out_item->uItemID].uChanceByTreasureLvl[treasure_level - 1];
            do
                {
                ++j;
                out_item->uItemID = val_list[j];
                tmp_chance += pItems[val_list[j]].uChanceByTreasureLvl[treasure_level - 1];
                }
                while ( tmp_chance < current_chance );
            }

        if (out_item->GetItemEquipType() == EQUIP_POTION && out_item->uItemID != ITEM_POTION_BOTTLE )
            {// if it potion set potion spec
            out_item->uEnchantmentType = 0;
            for (int i=0; i<2; ++i)
                out_item->uEnchantmentType += rand() % 4 + 1;
            out_item->uEnchantmentType = out_item->uEnchantmentType * treasure_level; 
            }
        }
    else
        {
   //artifact
        v56 = 0;
        for(int i=0; i<29; ++i) 
            v56 += pParty->pIsArtifactFound[i];

        v17 = rand() % 29;

        if ( v6 == 5 && (rand() % 100 < 5) && !pParty->pIsArtifactFound[v17] && v56 < 13 )
            {
            pParty->pIsArtifactFound[v17] = 1;
            out_item->uAttributes = 0;
            out_item->uItemID = v17 + 500;
            SetSpecialBonus(out_item);
            return;
            }

        v57 = 0;
        v18 = rand() % v5->uChanceByTreasureLvlSumm[treasure_level - 1];
        out_item->uItemID = 0;
        if ( v18 > 0 )
            {
            do
                {      
            v57 += pItems[out_item->uItemID + 1].uChanceByTreasureLvl[v6];
            ++out_item->uItemID;
                }
            while ( v57 < v18 );
            }

        if ( !v18 )
            out_item->uItemID = 1;
        if ( !out_item->uItemID )
            out_item->uItemID = 1;
        if (out_item->GetItemEquipType() == EQUIP_POTION && out_item->uItemID != ITEM_POTION_BOTTLE )
            {// if it potion set potion spec
            out_item->uEnchantmentType = 0;
            for (int i=0; i<2; ++i)
                out_item->uEnchantmentType += rand() % 4 + 1;
            out_item->uEnchantmentType = out_item->uEnchantmentType * treasure_level; 
            }
        out_item->uEnchantmentType = out_item->uEnchantmentType * treasure_level; 
        }

    if ( out_item->uItemID == ITEM_SPELLBOOK_LIGHT_DIVINE_INTERVENTION
        && !(unsigned __int16)_449B57_test_bit(pParty->_quest_bits, 239) )
        out_item->uItemID = ITEM_SPELLBOOK_LIGHT_SUN_BURST;
    if ( pItemsTable->pItems[out_item->uItemID + 1].uItemID_Rep_St )
        out_item->uAttributes = 0;
    else
        out_item->uAttributes = 1;

    if ( out_item->GetItemEquipType() != EQUIP_POTION )
        {
        out_item->uSpecEnchantmentType = 0;
        out_item->uEnchantmentType = 0;
        }
    //try get special enhansment
    switch (out_item->GetItemEquipType())
        {
    case EQUIP_OFF_HAND:
    case EQUIP_MAIN_HAND :   
    case EQUIP_BOW :    
        if ( !uBonusChanceWpSpecial[v6] )
            return;
        if ((rand() % 100)>=uBonusChanceWpSpecial[v6])
            return;
        break;
    case      EQUIP_ARMOUR :        
    case      EQUIP_SHIELD :         
    case      EQUIP_HELMET  :       
    case      EQUIP_BELT   :          
    case      EQUIP_CLOAK  :        
    case      EQUIP_GAUNTLETS :      
    case      EQUIP_BOOTS  :        
    case      EQUIP_RING   : 
        
        if ( !uBonusChanceStandart[v6] )
            return;
        special_chance = rand() % 100;
        if ( special_chance < uBonusChanceStandart[v6])
            {
              v26 = rand() %pEnchantmentsSumm[out_item->GetItemEquipType()-3]; 
            out_item->uEnchantmentType = 0;
            v27=pEnchantments[out_item->uEnchantmentType].to_item[out_item->GetItemEquipType()-3];
            if (v26>v27 )
                {
                do 
                {
                ++out_item->uEnchantmentType;
                v27+=pEnchantments[out_item->uEnchantmentType].to_item[out_item->GetItemEquipType()-3];
                } while (v26>v27);
            }
            ++out_item->uEnchantmentType;

            v33 = rand() % (bonus_ranges[v6].maxR - bonus_ranges[v6].minR + 1);
            out_item->m_enchantmentStrength = v33 + bonus_ranges[v6].minR;
            v32 = out_item->uEnchantmentType - 1;
            if ( v32 == 21 || v32 == 22 || v32 == 23 ) //Armsmaster skill, Dodge skill, Unarmed skill 
                out_item->m_enchantmentStrength = out_item->m_enchantmentStrength/2;
            if ( out_item->m_enchantmentStrength <= 0 )
                out_item->m_enchantmentStrength = 1;
            return;
            
            }
        if ( !uBonusChanceSpecial[v6])
            return;
        v34 = uBonusChanceStandart[v6] + uBonusChanceSpecial[v6];
        if ( special_chance>v34 )
            return;
        break;
    case EQUIP_WAND:
        out_item->uNumCharges = rand() % 6 + out_item->GetDamageMod() + 1;
        out_item->uMaxCharges = out_item->uNumCharges;
    default:
        return;
        }

    j=0;
    int spc_sum=0;
    int spc;
    memset(&val_list, 0, 3200);
    for (int i=0; i<pSpecialEnchantments_count;++i)
        {
        int tr_lv= pSpecialEnchantments[i].iTreasureLevel;
        switch ( treasure_level - 1 )
            {
        case 2:
            if ((tr_lv==1)||(tr_lv==0))
                {
                spc=pSpecialEnchantments[i].to_item_apply[out_item->GetItemEquipType()];
                spc_sum+=spc;
                if(spc)
                    {
                    val_list[j++]=i;  
                    }
                }
            break;
        case 3:
            if ((tr_lv==2)||(tr_lv==1)||(tr_lv==0))
                {
                spc=pSpecialEnchantments[i].to_item_apply[out_item->GetItemEquipType()];
                spc_sum+=spc;
                if(spc)
                    {
                    val_list[j++]=i;  
                    }
                }
            break;
        case 4:
            if ((tr_lv==3)||(tr_lv==2)||(tr_lv==1))
                {
                spc=pSpecialEnchantments[i].to_item_apply[out_item->GetItemEquipType()];
                spc_sum+=spc;
                if(spc)
                    {
                    val_list[j++]=i;  
                    }
                }
            break;
        case 5:
            if (tr_lv==3)
                {
                spc=pSpecialEnchantments[i].to_item_apply[out_item->GetItemEquipType()];
                spc_sum+=spc;     
                if(spc)
                    {
                    val_list[j++]=i;  
                    }
                }
            break;
            }
        }

    v46 = rand()%spc_sum+1;
    j=0;
    out_item->uSpecEnchantmentType =val_list[j];
    v45=pSpecialEnchantments[val_list[j]].to_item_apply[out_item->GetItemEquipType()];
    if (v45<v46)
        {
        do 
            {
            ++j;
            out_item->uSpecEnchantmentType=val_list[j];
            v45+=pSpecialEnchantments[val_list[j]].to_item_apply[out_item->GetItemEquipType()];
            } while (v45<v46);
        }
    ++out_item->uSpecEnchantmentType;
}

//----- (004505CC) --------------------------------------------------------
bool ItemGen::GenerateArtifact()
{
  signed int uNumArtifactsNotFound; // esi@1
  int artifacts_list[32]; 

  memset(artifacts_list, 0,sizeof(artifacts_list));
  uNumArtifactsNotFound = 0;

  for (int i=500;i<529;++i)
     if ( !pParty->pIsArtifactFound[i-500] )
        artifacts_list[uNumArtifactsNotFound++] = i;

  Reset();
  if ( uNumArtifactsNotFound )
  {
    uItemID = artifacts_list[rand() % uNumArtifactsNotFound];
    pItemsTable->SetSpecialBonus(this);
    return true;
  }
  else
    return false;

}

std::map<int, std::map<CHARACTER_ATTRIBUTE_TYPE, CEnchantment*>* >ItemGen::regularBonusMap;
std::map<int, std::map<CHARACTER_ATTRIBUTE_TYPE, CEnchantment*>* >ItemGen::specialBonusMap;
std::map<int, std::map<CHARACTER_ATTRIBUTE_TYPE, CEnchantment*>* >ItemGen::artifactBonusMap;

#define NEWBONUSINTOSPECIALLIST(x,y) AddToMap(ItemGen::specialBonusMap, enchId, x, y);
#define NEWBONUSINTOSPECIALLIST2(x,y,z) AddToMap(ItemGen::specialBonusMap, enchId, x, y, z);

#define NEWBONUSINTOREGULARLIST(x) AddToMap(ItemGen::regularBonusMap, enchId, x);

#define NEWBONUSINTOARTIFACTLIST(x,y) AddToMap(ItemGen::artifactBonusMap, itemId, x, y);
#define NEWBONUSINTOARTIFACTLIST2(x,y,z) AddToMap(ItemGen::artifactBonusMap, itemId, x, y, z);

void ItemGen::AddToMap( std::map<int, std::map<CHARACTER_ATTRIBUTE_TYPE, CEnchantment*>* > &maptoadd, int enchId, CHARACTER_ATTRIBUTE_TYPE attrId, int bonusValue /*= 0*/, unsigned __int16 Player::* skillPtr /*= NULL*/ )
{
  auto key = maptoadd.find(enchId);
  std::map<CHARACTER_ATTRIBUTE_TYPE, CEnchantment*>* currMap;
  if (key == maptoadd.end())
  {
    currMap = new std::map<CHARACTER_ATTRIBUTE_TYPE, CEnchantment*>;
    maptoadd[enchId] = currMap;
  }
  else
  {
    currMap = key->second;
  }
  Assert(currMap->find(attrId) == currMap->end(), "Attribute %d already present for enchantment %d", attrId, enchId);
  (*currMap)[attrId] = new CEnchantment(bonusValue, skillPtr);
}

void ItemGen::PopulateSpecialBonusMap()
{
  int enchId = 1;// of Protection, +10 to all Resistances
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_RESIST_FIRE, 10);
  NEWBONUSINTOSPECIALLIST( CHARACTER_ATTRIBUTE_RESIST_AIR, 10);
  NEWBONUSINTOSPECIALLIST( CHARACTER_ATTRIBUTE_RESIST_WATER, 10);
  NEWBONUSINTOSPECIALLIST( CHARACTER_ATTRIBUTE_RESIST_EARTH, 10);
  NEWBONUSINTOSPECIALLIST( CHARACTER_ATTRIBUTE_RESIST_MIND, 10);
  NEWBONUSINTOSPECIALLIST( CHARACTER_ATTRIBUTE_RESIST_BODY, 10);

  enchId = 2;//of The Gods, +10 to all Seven Statistics
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_STRENGTH, 10);
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_INTELLIGENCE, 10);
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_WILLPOWER, 10);
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_ENDURANCE, 10);
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_ACCURACY, 10);
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_SPEED, 10);
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_LUCK, 10);

  enchId = 26;//of Air Magic
  NEWBONUSINTOSPECIALLIST2(CHARACTER_ATTRIBUTE_SKILL_AIR,0, &Player::skillAir);

  enchId = 27;//of Body Magic
  NEWBONUSINTOSPECIALLIST2(CHARACTER_ATTRIBUTE_SKILL_BODY,0, &Player::skillBody);

  enchId = 28;//of Dark Magic
  NEWBONUSINTOSPECIALLIST2(CHARACTER_ATTRIBUTE_SKILL_DARK,0, &Player::skillDark);

  enchId = 29;//of Earth Magic
  NEWBONUSINTOSPECIALLIST2(CHARACTER_ATTRIBUTE_SKILL_EARTH,0, &Player::skillEarth);

  enchId = 30;//of Fire Magic
  NEWBONUSINTOSPECIALLIST2(CHARACTER_ATTRIBUTE_SKILL_FIRE,0, &Player::skillFire);

  enchId = 31;//of Light Magic
  NEWBONUSINTOSPECIALLIST2(CHARACTER_ATTRIBUTE_SKILL_LIGHT,0, &Player::skillLight);

  enchId = 32;//of Mind Magic
  NEWBONUSINTOSPECIALLIST2(CHARACTER_ATTRIBUTE_SKILL_MIND,0, &Player::skillMind);

  enchId = 33;//of Spirit Magic
  NEWBONUSINTOSPECIALLIST2(CHARACTER_ATTRIBUTE_SKILL_SPIRIT,0, &Player::skillSpirit);

  enchId = 34;//of Water Magic
  NEWBONUSINTOSPECIALLIST2(CHARACTER_ATTRIBUTE_SKILL_WATER,0, &Player::skillWater);

  enchId = 42;//of Doom
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_STRENGTH, 1);
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_INTELLIGENCE, 1);
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_WILLPOWER, 1);
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_ENDURANCE, 1);
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_ACCURACY, 1);
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_SPEED, 1);
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_LUCK, 1);
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_HEALTH, 1);
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_MANA, 1);
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_AC_BONUS, 1);
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_RESIST_FIRE, 1);
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_RESIST_AIR, 1);
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_RESIST_WATER, 1);
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_RESIST_EARTH, 1);
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_RESIST_MIND, 1);
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_RESIST_BODY, 1);

  enchId = 43;//of Earth
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_ENDURANCE, 10);
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_AC_BONUS, 10);
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_HEALTH, 10);

  enchId = 44;//of Life
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_HEALTH, 10);

  enchId = 45;//Rogues
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_SPEED, 5);
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_ACCURACY, 5);

  enchId = 46;//of The Dragon
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_STRENGTH, 25);

  enchId = 47;//of The Eclipse
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_MANA, 10);

  enchId = 48;//of The Golem
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_ENDURANCE, 15);
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_AC_BONUS, 5);

  enchId = 49;//of The Moon
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_INTELLIGENCE, 10);
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_LUCK, 10);

  enchId = 50;//of The Phoenix
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_RESIST_FIRE, 30);

  enchId = 51;//of The Sky
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_MANA, 10);
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_SPEED, 10);
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_INTELLIGENCE, 10);

  enchId = 52;//of The Stars
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_ENDURANCE, 10);
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_ACCURACY, 10);

  enchId = 53;//of The Sun
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_STRENGTH, 10);
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_WILLPOWER, 10);

  enchId = 54;//of The Troll
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_ENDURANCE, 15);

  enchId = 55;//of The Unicorn
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_LUCK, 15);

  enchId = 56;//Warriors
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_STRENGTH, 5);
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_ENDURANCE, 5);

  enchId = 57;//Wizards
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_INTELLIGENCE, 5);
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_WILLPOWER, 5);

  enchId = 60;//Monks'
  NEWBONUSINTOSPECIALLIST2(CHARACTER_ATTRIBUTE_SKILL_DODGE, 3, &Player::skillDodge);
  NEWBONUSINTOSPECIALLIST2(CHARACTER_ATTRIBUTE_SKILL_UNARMED, 3, &Player::skillUnarmed);

  enchId = 61;//Thieves'
  NEWBONUSINTOSPECIALLIST2(CHARACTER_ATTRIBUTE_SKILL_TRAP_DISARM, 3, &Player::skillStealing);
  NEWBONUSINTOSPECIALLIST2(CHARACTER_ATTRIBUTE_SKILL_STEALING, 3, &Player::skillDisarmTrap);

  enchId = 62;//of Identifying
  NEWBONUSINTOSPECIALLIST2(CHARACTER_ATTRIBUTE_SKILL_ITEM_ID, 3, &Player::skillItemId);
  NEWBONUSINTOSPECIALLIST2(CHARACTER_ATTRIBUTE_SKILL_MONSTER_ID, 3, &Player::skillMonsterId);

  enchId = 67;//Assassins'
  NEWBONUSINTOSPECIALLIST2(CHARACTER_ATTRIBUTE_SKILL_TRAP_DISARM, 2, &Player::skillDisarmTrap);

  enchId = 68;//Barbarians'
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_AC_BONUS, 5);

  enchId = 69;//of the Storm
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_RESIST_AIR, 20);

  enchId = 70;//of the Ocean
  NEWBONUSINTOSPECIALLIST(CHARACTER_ATTRIBUTE_RESIST_WATER, 10);
  NEWBONUSINTOSPECIALLIST2(CHARACTER_ATTRIBUTE_SKILL_ALCHEMY, 2, &Player::skillAlchemy);
}

void ItemGen::PopulateRegularBonusMap()
{
  int enchId = 1;//of Might
  NEWBONUSINTOREGULARLIST(CHARACTER_ATTRIBUTE_STRENGTH);

  enchId = 2;//of Thought
  NEWBONUSINTOREGULARLIST(CHARACTER_ATTRIBUTE_INTELLIGENCE);

  enchId = 3;//of Charm
  NEWBONUSINTOREGULARLIST(CHARACTER_ATTRIBUTE_WILLPOWER);

  enchId = 4;//of Vigor 
  NEWBONUSINTOREGULARLIST(CHARACTER_ATTRIBUTE_ENDURANCE);

  enchId = 5;//of Precision 
  NEWBONUSINTOREGULARLIST(CHARACTER_ATTRIBUTE_ACCURACY);

  enchId = 6;//of Speed 
  NEWBONUSINTOREGULARLIST(CHARACTER_ATTRIBUTE_SPEED);

  enchId = 7;//of Luck 
  NEWBONUSINTOREGULARLIST(CHARACTER_ATTRIBUTE_LUCK);

  enchId = 8;//of Health 
  NEWBONUSINTOREGULARLIST(CHARACTER_ATTRIBUTE_HEALTH);

  enchId = 9;//of Magic 
  NEWBONUSINTOREGULARLIST(CHARACTER_ATTRIBUTE_MANA);

  enchId = 10;//of Defense 
  NEWBONUSINTOREGULARLIST(CHARACTER_ATTRIBUTE_AC_BONUS);

  enchId = 11;//of Fire Resistance 
  NEWBONUSINTOREGULARLIST(CHARACTER_ATTRIBUTE_RESIST_FIRE);

  enchId = 12;//of Air Resistance 
  NEWBONUSINTOREGULARLIST(CHARACTER_ATTRIBUTE_RESIST_AIR);

  enchId = 13;//of Water Resistance 
  NEWBONUSINTOREGULARLIST(CHARACTER_ATTRIBUTE_RESIST_WATER);

  enchId = 14;//of Earth Resistance 
  NEWBONUSINTOREGULARLIST(CHARACTER_ATTRIBUTE_RESIST_EARTH);

  enchId = 15;//of Mind Resistance 
  NEWBONUSINTOREGULARLIST(CHARACTER_ATTRIBUTE_RESIST_MIND);

  enchId = 16;//of Body Resistance 
  NEWBONUSINTOREGULARLIST(CHARACTER_ATTRIBUTE_RESIST_BODY);

  enchId = 17;//of Alchemy 
  NEWBONUSINTOREGULARLIST(CHARACTER_ATTRIBUTE_SKILL_ALCHEMY);

  enchId = 18;//of Stealing 
  NEWBONUSINTOREGULARLIST(CHARACTER_ATTRIBUTE_SKILL_STEALING);

  enchId = 19;//of Disarming 
  NEWBONUSINTOREGULARLIST(CHARACTER_ATTRIBUTE_SKILL_TRAP_DISARM);

  enchId = 20;//of Items 
  NEWBONUSINTOREGULARLIST(CHARACTER_ATTRIBUTE_SKILL_ITEM_ID);

  enchId = 21;//of Monsters 
  NEWBONUSINTOREGULARLIST(CHARACTER_ATTRIBUTE_SKILL_MONSTER_ID);

  enchId = 22;//of Arms 
  NEWBONUSINTOREGULARLIST(CHARACTER_ATTRIBUTE_SKILL_ARMSMASTER);

  enchId = 23;//of Dodging 
  NEWBONUSINTOREGULARLIST(CHARACTER_ATTRIBUTE_SKILL_DODGE);

  enchId = 24;//of the Fist
  NEWBONUSINTOREGULARLIST(CHARACTER_ATTRIBUTE_SKILL_UNARMED);
}

void ItemGen::PopulateArtifactBonusMap()
{
  int itemId;
  itemId = ITEM_ARTIFACT_PUCK;
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_STRENGTH, 40);

  itemId = ITEM_ARTIFACT_IRON_FEATHER;
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_STRENGTH, 40);

  itemId = ITEM_ARTIFACT_WALLACE;
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_WILLPOWER, 40);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_SKILL_ARMSMASTER, 10);

  itemId = ITEM_ARTIFACT_CORSAIR;
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_LUCK, 40);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_SKILL_TRAP_DISARM, 5);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_SKILL_STEALING, 5);

  itemId = ITEM_ARTIFACT_GOVERNORS_ARMOR;
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_STRENGTH, 10);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_INTELLIGENCE, 10);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_WILLPOWER, 10);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_ENDURANCE, 10);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_ACCURACY, 10);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_SPEED, 10);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_LUCK, 10);

  itemId = ITEM_ARTIFACT_YORUBA;
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_ENDURANCE, 25);

  itemId = ITEM_ARTIFACT_SPLITTER;
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_RESIST_FIRE, 50);

  itemId = ITEM_ARTEFACT_ULLYSES,
    NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_ACCURACY, 50);

  itemId = ITEM_ARTEFACT_HANDS_OF_THE_MASTER,
    NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_SKILL_DODGE, 10);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_SKILL_UNARMED, 10);

  itemId = ITEM_ARTIFACT_LEAGUE_BOOTS;
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_SPEED, 40);
  NEWBONUSINTOARTIFACTLIST2(CHARACTER_ATTRIBUTE_SKILL_WATER, 0, &Player::skillWater);

  itemId = ITEM_ARTIFACT_RULERS_RING;
  NEWBONUSINTOARTIFACTLIST2(CHARACTER_ATTRIBUTE_SKILL_MIND, 0, &Player::skillMind);
  NEWBONUSINTOARTIFACTLIST2(CHARACTER_ATTRIBUTE_SKILL_DARK, 0, &Player::skillDark);

  itemId = ITEM_RELIC_MASH;
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_STRENGTH, 150);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_INTELLIGENCE, -40);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_WILLPOWER, -40);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_SPEED, -40);

  itemId = ITEM_RELIC_ETHRICS_STAFF;
  NEWBONUSINTOARTIFACTLIST2(CHARACTER_ATTRIBUTE_SKILL_DARK, 0, &Player::skillDark);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_SKILL_MEDITATION, 15);

  itemId = ITEM_RELIC_HARECS_LEATHER;
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_SKILL_TRAP_DISARM, 5);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_SKILL_STEALING, 5);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_LUCK, 50);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_RESIST_FIRE, -10);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_RESIST_WATER, -10);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_RESIST_AIR, -10);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_RESIST_EARTH, -10);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_RESIST_MIND, -10);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_RESIST_BODY, -10);

  itemId = ITEM_RELIC_OLD_NICK;
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_SKILL_TRAP_DISARM, 5);

  itemId = ITEM_RELIC_AMUCK;
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_STRENGTH, 100);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_ENDURANCE, 100);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_AC_BONUS, -15);

  itemId = ITEM_RELIC_GLORY_SHIELD;
  NEWBONUSINTOARTIFACTLIST2(CHARACTER_ATTRIBUTE_SKILL_SPIRIT, 0, &Player::skillSpirit);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_SKILL_SHIELD, 5);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_RESIST_MIND, -10);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_RESIST_BODY, -10);

  itemId = ITEM_RELIC_KELEBRIM;
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_ENDURANCE, 50);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_RESIST_EARTH, -30);

  itemId = ITEM_RELIC_TALEDONS_HELM;
  NEWBONUSINTOARTIFACTLIST2(CHARACTER_ATTRIBUTE_SKILL_LIGHT, 0, &Player::skillLight);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_WILLPOWER, 15);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_STRENGTH, 15);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_LUCK, -40);

  itemId = ITEM_RELIC_SCHOLARS_CAP;
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_SKILL_LEARNING, +15);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_ENDURANCE, -50);

  itemId = ITEM_RELIC_PHYNAXIAN_CROWN;
  NEWBONUSINTOARTIFACTLIST2(CHARACTER_ATTRIBUTE_SKILL_FIRE, 0, &Player::skillFire);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_RESIST_WATER, +50);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_WILLPOWER, 30);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_AC_BONUS, -20);

  itemId = ITEM_RILIC_TITANS_BELT;
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_STRENGTH, 75);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_SPEED, -40);

  itemId = ITEM_RELIC_TWILIGHT;
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_SPEED, 50);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_LUCK, 50);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_RESIST_FIRE, -15);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_RESIST_WATER, -15);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_RESIST_AIR, -15);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_RESIST_EARTH, -15);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_RESIST_MIND, -15);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_RESIST_BODY, -15);

  itemId = ITEM_RELIC_ANIA_SELVING;
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_ACCURACY, 150);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_SKILL_BOW, 5);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_AC_BONUS, -25);

  itemId = ITEM_RELIC_JUSTICE;
  NEWBONUSINTOARTIFACTLIST2(CHARACTER_ATTRIBUTE_SKILL_MIND, 0, &Player::skillMind);
  NEWBONUSINTOARTIFACTLIST2(CHARACTER_ATTRIBUTE_SKILL_BODY, 0, &Player::skillBody);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_SPEED, -40);

  itemId = ITEM_RELIC_MEKORIGS_HAMMER;
  NEWBONUSINTOARTIFACTLIST2(CHARACTER_ATTRIBUTE_SKILL_SPIRIT, 0, &Player::skillSpirit);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_STRENGTH, 75);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_RESIST_AIR, -50);

  itemId = ITEM_ARTIFACT_HERMES_SANDALS;
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_SPEED, 100);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_ACCURACY, 50);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_RESIST_AIR, 50);

  itemId = ITEM_ARTIFACT_CLOAK_OF_THE_SHEEP;
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_WILLPOWER, -20);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_INTELLIGENCE, -20);

  itemId = ITEM_ARTIFACT_MINDS_EYE;
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_WILLPOWER, 15);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_INTELLIGENCE, 15);

  itemId = ITEM_ELVEN_CHAINMAIL;
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_SPEED, 15);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_ACCURACY, 15);

  itemId = ITEM_FORGE_GAUNTLETS;
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_STRENGTH, 15);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_ENDURANCE, 15);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_RESIST_FIRE, 30);

  itemId = ITEM_ARTIFACT_HEROS_BELT;
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_STRENGTH, 15);
  NEWBONUSINTOARTIFACTLIST(CHARACTER_ATTRIBUTE_SKILL_ARMSMASTER, 5);
}

void ItemGen::GetItemBonusSpecialEnchantment( Player* owner, CHARACTER_ATTRIBUTE_TYPE attrToGet, int* additiveBonus, int* halfSkillBonus )
{
  auto bonusList = ItemGen::specialBonusMap.find(this->uSpecEnchantmentType);
  if (bonusList == ItemGen::specialBonusMap.end())
  {
    return;
  }
  std::map<CHARACTER_ATTRIBUTE_TYPE, CEnchantment*>* currList = bonusList->second;
  if (currList->find(attrToGet) != currList->end())
  {
    CEnchantment* currBonus = (*currList)[attrToGet];
    if (currBonus->statPtr != NULL)
    {
      if (currBonus->statBonus == 0)
      {
        *halfSkillBonus = owner->*currBonus->statPtr / 2;
      }
      else
      {
        if (*additiveBonus < currBonus->statBonus)
        {
          *additiveBonus = currBonus->statBonus;
        }
      }
    }
    else
    {
      *additiveBonus += currBonus->statBonus;
    }
  }
}

void ItemGen::GetItemBonusArtifact( Player* owner, CHARACTER_ATTRIBUTE_TYPE attrToGet, int* bonusSum )
{
  auto bonusList = ItemGen::artifactBonusMap.find(this->uItemID);
  if (bonusList == ItemGen::artifactBonusMap.end())
  {
    return;
  }
  std::map<CHARACTER_ATTRIBUTE_TYPE, CEnchantment*>* currList = bonusList->second;
  if (currList->find(attrToGet) != currList->end())
  {
    CEnchantment* currBonus = (*currList)[attrToGet];
    if (currBonus->statPtr != NULL)
    {
      *bonusSum = owner->*currBonus->statPtr / 2;
    }
    else
    {
      *bonusSum += currBonus->statBonus;
    }
  }
}

bool ItemGen::IsRegularEnchanmentForAttribute( CHARACTER_ATTRIBUTE_TYPE attrToGet )
{
  auto bonusList = ItemGen::specialBonusMap.find(this->uEnchantmentType);
  if (bonusList == ItemGen::specialBonusMap.end())
  {
    return false;
  }
  std::map<CHARACTER_ATTRIBUTE_TYPE, CEnchantment*>* currList = bonusList->second;
  return (currList->find(attrToGet) != currList->end());
  return false;
}

ITEM_EQUIP_TYPE ItemGen::GetItemEquipType()
{
  return pItemsTable->pItems[this->uItemID].uEquipType;
}

unsigned char ItemGen::GetPlayerSkillType()
{
  return pItemsTable->pItems[this->uItemID].uSkillType;
}

char* ItemGen::GetIconName()
{
  return pItemsTable->pItems[this->uItemID].pIconName;
}

unsigned __int8 ItemGen::GetDamageDice()
{
  return pItemsTable->pItems[this->uItemID].uDamageDice;
}

unsigned __int8 ItemGen::GetDamageRoll()
{
  return pItemsTable->pItems[this->uItemID].uDamageRoll;
}

unsigned __int8 ItemGen::GetDamageMod()
{
  return pItemsTable->pItems[this->uItemID].uDamageMod;
}
//----- (004B8E3D) --------------------------------------------------------
void GenerateStandartShopItems()
	{
	signed int item_count; 
	signed int shop_index; 
	int treasure_lvl; 
	int item_class; 
	int mdf;

	shop_index = (signed int)window_SpeakInHouse->ptr_1C;
	if ( uItemsAmountPerShopType[p2DEvents[shop_index - 1].uType] )
		{
	    for (item_count=0; item_count<=uItemsAmountPerShopType[p2DEvents[shop_index - 1].uType]; ++item_count )
		{
		   if (shop_index<=14) //weapon shop
			   {
			   treasure_lvl = shopWeap_variation_ord[shop_index].treasure_level;
			   item_class =shopWeap_variation_ord[shop_index].item_class[rand() % 4];
			   }
		   else if (shop_index<=28) //armor shop
			   {
			   mdf =0;
			   if (item_count > 3)
					 ++mdf;// rechek offsets
			    treasure_lvl = shopArmr_variation_ord[2*(shop_index-15)+mdf].treasure_level;
				item_class =shopArmr_variation_ord[2*(shop_index-15)+mdf].item_class[rand() % 4];
			   }
		   else if (shop_index<=41)  //magic shop
			   {
			   treasure_lvl = shopMagic_treasure_lvl[shop_index-28];
			   item_class = 22;  //misc
			   }
		   else if (shop_index<=53) //alchemist shop
			   {
			    if (item_count<6)
					{
					pParty->StandartItemsInShops[shop_index][item_count].Reset();
					pParty->StandartItemsInShops[shop_index][item_count].uItemID = 220;  //potion bottle
					continue;
					}
				else
					{
					treasure_lvl = shopAlch_treasure_lvl[shop_index-41];
					item_class = 45;  //reagent
					}
			   }
		   pItemsTable->GenerateItem(treasure_lvl, item_class, &pParty->StandartItemsInShops[shop_index][item_count]);
		   pParty->StandartItemsInShops[shop_index][item_count].SetIdentified();  //identified
		}
		}
	pParty->InTheShopFlags[shop_index] = 0;
	}

//----- (004B8F94) --------------------------------------------------------
void  GenerateSpecialShopItems()
	{
	signed int item_count; 
	signed int shop_index; 
	int treasure_lvl; 
	int item_class; 
	int mdf;

	shop_index = (signed int)window_SpeakInHouse->ptr_1C;
	if ( uItemsAmountPerShopType[p2DEvents[shop_index - 1].uType] )
		{
		for (item_count=0; item_count<=uItemsAmountPerShopType[p2DEvents[shop_index - 1].uType]; ++item_count )
			{
			if (shop_index<=14) //weapon shop
				{
				treasure_lvl = shopWeap_variation_spc[shop_index].treasure_level;
				item_class =  shopWeap_variation_spc[shop_index].item_class[rand() % 4];
				}
			else if (shop_index<=28) //armor shop
				{
				mdf =0;
				if (item_count > 3)
					++mdf;
				treasure_lvl = shopArmr_variation_spc[2*(shop_index-15)+mdf].treasure_level;
				item_class =shopArmr_variation_spc[2*(shop_index-15)+mdf].item_class[rand() % 4];
				}
			else if (shop_index<=41)  //magic shop
				{
				treasure_lvl = shopMagicSpc_treasure_lvl[shop_index-28];
				item_class = 22;  //misc
				}
			else if (shop_index<=53) //alchemist shop
				{
				if (item_count<6)
					{
					pParty->SpecialItemsInShops[shop_index][item_count].Reset();
					pParty->SpecialItemsInShops[shop_index][item_count].uItemID = rand() % 32 + 740;  //mscrool
					continue;
					}
				else
					{
					treasure_lvl = shopAlchSpc_treasure_lvl[shop_index-41];
					item_class = 44;  //potion
					}
				}
			pItemsTable->GenerateItem(treasure_lvl, item_class, &pParty->SpecialItemsInShops[shop_index][item_count]);
			pParty->SpecialItemsInShops[shop_index][item_count].SetIdentified();  //identified
			}
		}
	pParty->InTheShopFlags[shop_index] = 0;
	}


//----- (00450218) --------------------------------------------------------
void GenerateItemsInChest()
    {
    unsigned int v0; // eax@1
    Chest *v1; // ebx@1
    MapInfo *v2; // esi@1
    ItemGen *v3; // ebx@2
    int v4; // ebp@4
    int v5; // edi@4
    int v6; // esi@4
    int v7; // eax@4
    signed int v8; // esi@4
    int v9; // edx@4
    int v10; // esi@8
    int v11; // ebp@25
    int v12; // esi@25
    signed int v13; // ebp@27
    ItemGen *v14; // edi@28
    signed int v15; // edx@32
    signed __int64 v16; // qtt@32
    int v17; // esi@34
    signed int v18; // [sp+10h] [bp-18h]@1
    int v19; // [sp+14h] [bp-14h]@4
    MapInfo *v20; // [sp+18h] [bp-10h]@1
    Chest *v21; // [sp+1Ch] [bp-Ch]@1
    int v22; // [sp+20h] [bp-8h]@26
    signed int v23; // [sp+24h] [bp-4h]@2

    v18 = rand() % 100;  //main random
    v0 = pMapStats->GetMapInfo(pCurrentMapName);
    //	v1 = pChests;
    v2 = &pMapStats->pInfos[v0];
    //v21 = pChests;
    //v20 = &pMapStats->pInfos[v0];
    for(int i=1; i<20;++i)
        {
        for(int j=0; j<140;++j)
            {

            v3 = &pChests[i].igChestItems[j];
            if ( v3->uItemID < 0 )
                {
                v4 = rand() % 5; //additional items in chect
                v5 = (unsigned __int8)byte_4E8168[abs((int)v3->uItemID)-1][2*v2->Treasure_prob];
                v6 = (unsigned __int8)byte_4E8168[abs((int)v3->uItemID)-1][2*v2->Treasure_prob+1];
                v8 = v6 - v5 + 1;
                v9 = v5 + rand() % v8;  //treasure level 
                if (v9<7)
                    {
                    if (v18<20)
                        {
                        v3->Reset();
                        }
                    else if (v18<60) //generate gold
                        {
                        v10=0;
                        v3->Reset();
                        switch (v9)
                            {
                        case 1: //small gold
                            v10 = rand() % 51 + 50;
                            v3->uItemID = 197;
                            break;
                        case 2://small gold
                            v10 = rand() % 101 + 100;
                            v3->uItemID = 197;
                            break;
                        case 3:  //medium
                            v10 = rand() % 301 + 200;
                            v3->uItemID = 198;
                            break;
                        case 4: //medium
                            v10 = rand() % 501 + 500;
                            v3->uItemID = 198;
                            break;
                        case 5: //big
                            v10 = rand() % 1001 + 1000;
                            v3->uItemID = 199;
                            break;
                        case 6: //big
                            v10 = rand() % 3001 + 2000;
                            v3->uItemID = 199;
                            break;
                            }
                        v3->SetIdentified();
                        v3->uSpecEnchantmentType = v10;
                        }
                    else
                        {
                        pItemsTable->GenerateItem(v9, 0, v3);
                        }

                    v12 = 0;
                    //generate more items
                    for (v11=0; v11<v4; ++v11)
                        {

                        if ( v12 >= 140 )
                            break;
                        while ( !(pChests[i].igChestItems[v12].uItemID==0) &&(v12<140))
                            {
                            ++v12;
                            }
                        v14=&pChests[i].igChestItems[v12];
                        v18 = rand() % 100;
                        if (v18<20)
                            {
                            v3->Reset();
                            }
                        else if (v18<60) //generate gold
                            {
                            v10=0;
                            v3->Reset();
                            switch (v9)
                                {
                            case 1: //small gold
                                v10 = rand() % 51 + 50;
                                v14->uItemID = 197;
                                break;
                            case 2://small gold
                                v10 = rand() % 101 + 100;
                                v14->uItemID = 197;
                                break;
                            case 3:  //medium
                                v10 = rand() % 301 + 200;
                                v14->uItemID = 198;
                                break;
                            case 4: //medium
                                v10 = rand() % 501 + 500;
                                v14->uItemID = 198;
                                break;
                            case 5: //big
                                v10 = rand() % 1001 + 1000;
                                v14->uItemID = 199;
                                break;
                            case 6: //big
                                v10 = rand() % 3001 + 2000;
                                v14->uItemID = 199;
                                break;
                                }
                            v14->SetIdentified();
                            v14->uSpecEnchantmentType = v10;
                            }
                        else
                            {
                            pItemsTable->GenerateItem(v9, 0, v14);
                            }                       
                        ++v12;
                        }
                    }
                else
                    v3->GenerateArtifact();
                }
            }			
        }

    }


	

// 4505CC: using guessed type int var_A0[32];
	//----- (004B3703) --------------------------------------------------------
void FillAviableSkillsToTeach( int _this )
	{
	const char *v30; // ecx@65
	unsigned int v29; // edx@56
	int v15; // ecx@19
	int v33; // [sp-4h] [bp-2Ch]@23
	int v34; // [sp-4h] [bp-2Ch]@43
	int v21; // ecx@34
	int v35[5]; // [sp+Ch] [bp-1Ch]@8
	int v37=0; // [sp+24h] [bp-4h]@1*
	int i=0;

	dword_F8B1DC = 0;

	switch (_this)
		{
	case 1:  //shop weapon
		for (int i=0; i<2; ++i)
			{
			for (int j=0; j<4; ++j)
				{
				if ( i )
					v21 = shopWeap_variation_spc[(unsigned int)window_SpeakInHouse->ptr_1C].item_class[j];
				else
					v21 = shopWeap_variation_ord[(unsigned int)window_SpeakInHouse->ptr_1C].item_class[j];

				switch (v21)
					{
				case 23:  v34 = 37;	break;
				case 24:  v34 = 38;	break;
				case 25:  v34 = 39;	break;
				case 26:  v34 = 40;	break;
				case 27:  v34 = 41;	break;
				case 28:  v34 = 42; break;
				case 30:  v34 = 36;	break;
				default:
					continue;
					}	
				v37 = sub_4BE571(v34, v35, v37, 5);
				}
			}
		break;
	case 2: //shop armor

		for (int i=0; i<2; ++i)
			{
			for (int j=0; j<2; ++j)
				{
				for (int k=0; k<4; ++k)
					{
					if ( i )
						v15 = shopArmr_variation_spc[(unsigned int)window_SpeakInHouse->ptr_1C-15+j].item_class[k];
					else
						v15 = shopArmr_variation_ord[(unsigned int)window_SpeakInHouse->ptr_1C-15+j].item_class[k];
					switch (v15)
						{
					case 31: v33 = 45; break;
					case 32: v33 = 46; break;
					case 33: v33 = 47; break;
					case 34: v33 = 44; break;
					default:
						continue;
						}
					v37 = sub_4BE571(v33, v35, v37, 5);
					}
				}
			}
		break;
	case 3:  //shop magic
		v37 = 2;
		v35[0] = 57;
		v35[1] = 59;
		break;
	case 4: //shop alchemist
		v37 = 2;
		v35[0] = 71;
		v35[1] = 68;
		break;
	case 21:  //tavern
		v37 = 3;
		v35[0] = 70;
		v35[1] = 65;
		v35[2] = 62;
		break;
	case 23:  //temple
		v37 = 3;
		v35[0] = 67;
		v35[1] = 66;
		v35[2] = 58;
		break;
	case 30:  ///trainig
		v37 = 2;
		v35[0] = 69;
		v35[1] = 60;
		break;
		}
	for(i=0;i<v37;++i) 
		{
		v29=v35[i];
		switch(v29)
			{
		case 40 :v30 = pSkillNames[4];	break;
		case 5 : v30 = pSkillNames[23];	break;
		case 36 :v30 = pSkillNames[0];	break;
		case 37 :v30 = pSkillNames[1];	break;
		case 38 :v30 = pSkillNames[2];	break;
		case 39 :v30 = pSkillNames[3];	break;
		case 41 :v30 = pSkillNames[5];	break;
		case 42 :v30 = pSkillNames[6];	break;
		case 44 :v30 = pSkillNames[8];	break;
		case 45 :v30 = pSkillNames[9];	break;
		case 46 :v30 = pSkillNames[10];	break;
		case 47 :v30 = pSkillNames[11];	break;
		case 66 :v30 = pSkillNames[30];	break;
		case 57 :v30 = pSkillNames[21];	break;
		case 58 :v30 = pSkillNames[22];	break;
		case 60 :v30 = pSkillNames[24];	break;
		case 62 :v30 = pSkillNames[26];	break;
		case 65 :v30 = pSkillNames[29];	break;
		case 67:v30 = pSkillNames[31];	break;
		case 68:v30 = pSkillNames[32];	break;
		case 69:v30 = pSkillNames[33];	break;
		case 70:v30 = pSkillNames[34];	break;
		case 71:v30 = pSkillNames[35]; break;
		default:
			v30 = pGlobalTXT_LocalizationStrings[127]; //"No Text!"
			}
		pShopOptions[dword_F8B1DC] = const_cast<char *>(v30);
		++dword_F8B1DC;
		CreateButtonInColumn(i+1, v29);
		}
	pDialogueWindow->_41D08F_set_keyboard_control_group(i, 1, 0, 2);
	dword_F8B1E0 = pDialogueWindow->pNumPresenceButton;
	}

	//----- (004BE571) --------------------------------------------------------
int  sub_4BE571(int a1, int *a2, int a3, int a4)
	{
	int result; // eax@1
	int i; // esi@3

	result = a3;
	if ( a3 < a4 )
		{
		for ( i = 0; i < a3; ++i )
			{
			if ( a1 == a2[i] )
				break;
			}
		if ( i == a3 )
			{
			a2[a3] = a1;
			result = a3 + 1;
			}
		return result;
		}
	else
		{
		return  a4;
		}
	}
//----- (0043C91D) --------------------------------------------------------
int __fastcall GetItemTextureFilename(char *pOut, signed int item_id, int index, int shoulder)
{
  int result; // eax@2
  char v5; // zf@3
  const char *v6; // [sp-Ch] [bp-18h]@88
  signed int v7; // [sp-8h] [bp-14h]@61
  int v8; // [sp-4h] [bp-10h]@61
  signed int v9; // [sp-4h] [bp-10h]@69

  result = 0; //BUG   fn is void
  if ( item_id <= 500 )
  {
    //v5 = *((char *)&pBloodsplatContainer->std__vector_pBloodsplats[62].field_20 + a2 + 2) == 0;
    v5 = party_has_equipment[(item_id - 100) + 32 + 2] == 0;
    switch ( item_id )
    {
      case 516:
        v5 = byte_5111F6[2] == 0;
        break;
      case 505:
        v5 = byte_5111F6[1] == 0;
        break;
      case 504:
        v5 = byte_5111F6[0] == 0;
        break;
      case 533:
        v5 = byte_5111F6[16] == 0;
        break;
      case 512:
        v5 = byte_5111F6[3] == 0;
        break;
      case 521:
        v5 = byte_5111F6[4] == 0;
        break;
      case 522:
        v5 = byte_5111F6[5] == 0;
        break;
      case 523:
        v5 = byte_5111F6[6] == 0;
        break;
      case 532:
        v5 = byte_5111F6[7] == 0;
        break;
      case 544:
        v5 = byte_5111F6[8] == 0;
        break;
      case 524:
        v5 = byte_5111F6[9] == 0;
        break;
      case 535:
        v5 = byte_5111F6[10] == 0;
        break;
      case 525:
        v5 = byte_5111F6[11] == 0;
        break;
      case 530:
        v5 = byte_5111F6[12] == 0;
        break;
      case 547:
        v5 = byte_5111F6[13] == 0;
        break;
      case 548:
        v5 = byte_5111F6[14] == 0;
        break;
      case 550:
        v5 = byte_5111F6[15] == 0;
        break;
      default:
        break;
    }
    if ( v5 )
      return result;
    result = 516;
    if ( item_id < 66 || item_id > 78 )
    {
      if ( item_id == 516 )
      {
        if ( !shoulder )
          return sprintf(pOut, "item%3.3dv%d", 234, index);
        if ( shoulder == 1 )
          return sprintf(pOut, "item%3.3dv%da1", 234, index);
        if ( shoulder == 2 )
          return sprintf(pOut, "item%3.3dv%da2", 234, index);
      }
      if ( item_id != 504 && item_id != 505 && item_id != 533 )
      {
        if ( (item_id < 100 || item_id > 104) && item_id != 524 && item_id != 535 )
        {
          if ( item_id >= 115 && item_id <= 119 || item_id == 512 )
          {
            if ( item_id == 512 )
              item_id = 312;
            return sprintf(pOut, "item%3.3dv%d", item_id, index);
          }
          if ( (item_id < 89 || item_id > 99) && item_id != 521 && item_id != 522 && item_id != 523 && item_id != 532 && item_id != 544 )
          {
            result = 525;
            if ( (item_id < 105 || item_id > 109) && item_id != 525 && item_id != 530 && item_id != 547 && item_id != 548 && item_id != 550 )
              return result;
            switch ( item_id )
            {
              case 525:
                item_id = 325;
                break;
              case 530:
                item_id = 330;
                break;
              case 547:
                item_id = 347;
                break;
              case 548:
                item_id = 348;
                break;
              case 550:
                item_id = 350;
                break;
            }
            if ( !shoulder )
              return sprintf(pOut, "item%3.3dv%d", item_id, index);
            return sprintf(pOut, "item%3.3dv%da1", item_id, index);
          }
          if ( item_id == 521 )
            return sprintf(pOut, "item%3.3dv%d", 239, index);
          if ( item_id == 522 )
            return sprintf(pOut, "item%3.3dv%d", 240, index);
          if ( item_id == 523 )
            return sprintf(pOut, "item%3.3dv%d", 241, index);
          if ( item_id != 532 )
          {
            if ( item_id == 544 )
              item_id = 344;
            return sprintf(pOut, "item%3.3dv%d", item_id, index);
          }
          return sprintf(pOut, "item%3.3dv%d", 93, index);
        }
        if ( item_id == 524 )
          return sprintf(pOut, "item%3.3dv%d", 324, index);
        if ( item_id == 535 )
          item_id = 104;
        return sprintf(pOut, "item%3.3dv%d", item_id, index);
      }
    }
    if ( item_id != 516 )
    {
      switch ( item_id )
      {
        case 504:
          item_id = 235;
          break;
        case 505:
          item_id = 236;
          break;
        case 533:
          item_id = 73;
          break;
      }
      if ( !shoulder )
        return sprintf(pOut, "item%3.3dv%d", item_id, index);
      if ( shoulder == 1 )
        return sprintf(pOut, "item%3.3dv%da1", item_id, index);
      if ( shoulder == 2 )
        return sprintf(pOut, "item%3.3dv%da2", item_id, index);
    }
    if ( !shoulder )
      return sprintf(pOut, "item%3.3dv%d", 234, index);
    if ( shoulder == 1 )
      return sprintf(pOut, "item%3.3dv%da1", 234, index);
    if ( shoulder == 2 )
      return sprintf(pOut, "item%3.3dv%da2", 234, index);
  }
  result = item_id - 504;
  return result;
}

