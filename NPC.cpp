#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define _CRT_SECURE_NO_WARNINGS
#include "mm7_unsorted_subs.h"
#include "texts.h"
#include "LOD.h"
#include "Autonotes.h"
#include "Awards.h"
#include "mm7_data.h"
#include "MM7.h"
#include "Party.h"
#include "NPC.h"
#include "GUIWindow.h"
#include "MediaPlayer.h"
#include "Events.h"
#include "UI\UIHouses.h"
#include "Indoor.h"
#include "MapInfo.h"
#include "Level/Decoration.h"
#include "Actor.h"
#include "AudioPlayer.h"
#include "CastSpellInfo.h"
#include "Overlays.h"

int pDialogueNPCCount;
std::array<struct Texture *, 6> pDialogueNPCPortraits;
int uNumDialogueNPCPortraits; // weak
struct NPCStats *pNPCStats = nullptr;

int NPCStats::dword_AE336C_LastMispronouncedNameFirstLetter = -1;
int NPCStats::dword_AE3370_LastMispronouncedNameResult = -1;

void  InitializeAwards();
void  InitializeScrolls();
void  InitializeMerchants();
void  InitializeTransitions();
void  InitializeAutonotes();
void  InitializeQuests();
bool   CheckPortretAgainstSex(int portret_num, int sex); 

//----- (004459F9) --------------------------------------------------------
NPCData *__fastcall GetNPCData(signed int npcid)
{
  unsigned int v1; // esi@1
  NPCData *result; // eax@5
  int v3; // esi@9
  int v4; // ecx@9
  //int v5; // edx@9
  //NPCData *v6; // eax@9
//  char *v7; // ebx@14
//  NPCData *v8; // edi@14
  char v9; // al@22
//  char v10;
  //std::string v10; // [sp-18h] [bp-2Ch]@4
//  int v11;
  //const char *v11; // [sp-8h] [bp-1Ch]@4
//  int v12; // [sp-4h] [bp-18h]@4
//  int v13; 
//  char *v14;
  //std::string *v13; // [sp+Ch] [bp-8h]@4
//  int a3; // [sp+13h] [bp-1h]@4
  int i;

  /*v1 = npcid;
  if ( (npcid & 0x80000000u) == 0 )
  {
    if ( (signed int)npcid < 5000 )
    {
      if ( (signed int)npcid >= 501 )
      {
    MessageBoxW(nullptr, L"NPC id exceeds MAX_DATA!", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Events.cpp:1984", 0);
      }
      return &pNPCStats->pNewNPCData[v1];
    }
    return &pNPCStats->array_13EF4[npcid - 5000];
  }
  if ( (signed int)npcid >= 5000 )
    return &pNPCStats->array_13EF4[npcid - 5000];
  if ( (sDialogue_SpeakingActorNPC_ID & 0x80000000u) == 0 )
  {
    result = 0;
  }
  else
  {
    v3 = abs((int)sDialogue_SpeakingActorNPC_ID) - 1;
    v4 = 0;
    v5 = 0;
    v6 = pParty->pHirelings;
    do
    {
      if ( v6->pName )
        pTmpBuf[v4++] = v5;
      ++v6;
      ++v5;
    }
    while ( (signed int)v6 < (signed int)&pParty->pPickedItem );
    v13 = 0;
    if ( (signed int)pNPCStats->uNumNewNPCs > 0 )
    {
      v7 = &pTmpBuf[v4];
      v8 = pNPCStats->pNewNPCData;
      do
      {
        if ( v8->uFlags & 0x80
          && (!pParty->pHirelings[0].pName || strcmp(v8->pName, pParty->pHirelings[0].pName))
          && (!pParty->pHirelings[1].pName || strcmp(v8->pName, pParty->pHirelings[1].pName)) )
          *v7++ = (char)v13 + 2;
        v13 = (std::string *)((char *)v13 + 1);
        ++v8;
      }
      while ( (signed int)v13 < (signed int)pNPCStats->uNumNewNPCs );
    }
    v9 = pTmpBuf[v3];
    if ( (unsigned __int8)v9 >= 2u )
      result = &pNPCStats->pNPCData[(unsigned __int8)v9 + 499];
    else
      result = &pParty->pHirelings[(unsigned __int8)v9];
  }
  return result;*/
  v1 = npcid;
  if ( npcid >= 0 )
  {
    if ( npcid < 5000 )
    {
      if ( npcid >= 501 )
      {
        MessageBoxW(nullptr, L"NPC id exceeds MAX_DATA!", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Events.cpp:1984", 0);
      }
      return &pNPCStats->pNewNPCData[v1];// - 1];
    }
    return &pNPCStats->pAdditionalNPC[npcid - 5000];
  }


  if ( npcid >= 5000 )
    return &pNPCStats->pAdditionalNPC[npcid - 5000];
  if (sDialogue_SpeakingActorNPC_ID >= 0)
  {
    result = 0;
  }
  else
  {
    v3 = abs(sDialogue_SpeakingActorNPC_ID) - 1;
    v4 = 0;

    for (i = 0; i < 2; ++i)
    {
      if (pParty->pHirelings[i].pName)
        pTmpBuf[v4++] = i;
    }

    if (pNPCStats->uNumNewNPCs > 0)
    {
      for (i = 0; i < pNPCStats->uNumNewNPCs; ++i)
      {
        if (pNPCStats->pNewNPCData[i].Hired())
        {
          if (!pParty->pHirelings[0].pName || strcmp((char *)pNPCStats->pNewNPCData[i].pName, (char *)pParty->pHirelings[0].pName))
          {
            if (!pParty->pHirelings[1].pName || strcmp((char *)pNPCStats->pNewNPCData[i].pName, (char *)pParty->pHirelings[1].pName))
              pTmpBuf[v4++] = i + 2;
          }
        }
      }
    }

    v9 = pTmpBuf[v3];
    if ( v9 >= 2 )
     result = &pNPCStats->pNPCData[499 + v9];
    else
      result = &pParty->pHirelings[v9];
  }
  return result;
}

//----- (00445B2C) --------------------------------------------------------
struct NPCData * GetNewNPCData( signed int npcid, int* npc_indx )
    {

  int* v3; // edi@1
  NPCData *result; // eax@5
  int v5; // esi@9
  int v6; // ecx@9
  char v11; // al@23

  v3 = npc_indx;
  if ( npcid >= 0 )
  {
    if ( npcid < 5000 )
    {
      if ( npcid >= 501 )
      {
        MessageBoxW(nullptr, L"NPC id exceeds MAX_DATA!", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Events.cpp:2040", 0);
      }
      *v3 = npcid;
      return &pNPCStats->pNewNPCData[npcid];
    }
    *npc_indx = npcid - 5000;
    return &pNPCStats->pAdditionalNPC[npcid - 5000];
  }
  if ( npcid >= 5000 )
      {
      *npc_indx = npcid - 5000;
      return &pNPCStats->pAdditionalNPC[npcid - 5000];
      }
  if ( sDialogue_SpeakingActorNPC_ID >= 0 )
  {
    *npc_indx = 0;
    result = nullptr;
  }
  else
  {
    v5 = abs(sDialogue_SpeakingActorNPC_ID) - 1;
    v6 = 0;
    for (int i=0; i<2; ++i)
    {
      if ( pParty->pHirelings[i].pName )
        pTmpBuf[v6++] = i;
     
    }     
    for (int i=0; i< pNPCStats->uNumNewNPCs; ++i)
        {
        if ( pNPCStats->pNewNPCData[i].Hired()
            && (!pParty->pHirelings[0].pName || strcmp(pNPCStats->pNewNPCData[i].pName, pParty->pHirelings[0].pName))
            && (!pParty->pHirelings[1].pName || strcmp(pNPCStats->pNewNPCData[i].pName, pParty->pHirelings[1].pName)) )
            {
                pTmpBuf[v6++]=i+2;
            }
        }
    v11 = pTmpBuf[v5];

    if ( v11 >= 2u )
    {
      *v3 = v11 - 2;
      result = &pNPCStats->pNewNPCData[v11 - 2];
    }
    else
    {
      *v3 = v11;
      result = &pParty->pHirelings[v11];
    }
  }
  return result;
}

//----- (00476977) --------------------------------------------------------
void NPCStats::InitializeNPCText()
	{
	int i;
	char* test_string;
	unsigned char c;
	bool break_loop;
	unsigned int temp_str_len;
	char* tmp_pos;
	int decode_step;

	free(pNPCTextTXT_Raw);
	pNPCTextTXT_Raw = (char *)pEvents_LOD->LoadRaw("npctext.txt", 0);
	strtok(pNPCTextTXT_Raw, "\r");

	for (i=0; i<789; ++i)
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
				if ( decode_step == 1)
					pNPCTopics[i].pText =RemoveQuotes(test_string);
				}
			else
				{ 
				break_loop = true;
				}
			++decode_step;
			test_string=tmp_pos+1;
			} while ((decode_step<2)&&!break_loop);
		}
	free(pNPCTopicTXT_Raw);
	pNPCTopicTXT_Raw = (char *)pEvents_LOD->LoadRaw("npctopic.txt", 0);
	strtok(pNPCTopicTXT_Raw, "\r");

	for ( i = 1; i <= 579; ++i )//NPC topics count limit
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
				if ( decode_step == 1)
					pNPCTopics[i].pTopic = RemoveQuotes(test_string);
				}
			else
				{ 
				break_loop = true;
				}
			++decode_step;
			test_string=tmp_pos+1;
			} while ((decode_step<2)&&!break_loop);
		}

	free(pNPCDistTXT_Raw);
	pNPCDistTXT_Raw = (char *)pEvents_LOD->LoadRaw("npcdist.txt", 0);
	strtok(pNPCDistTXT_Raw, "\r");
	strtok(NULL, "\r");

	for (i=1; i<59; ++i)
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
				if ((decode_step>0)&&(decode_step<77))
					{
					pProfessionChance[decode_step].professionChancePerArea[i]=atoi(test_string);
					}
				else if (decode_step==0)
					{
					pProfessionChance[0].professionChancePerArea[i]=10;
					}
				}
			else
				{ 
				break_loop = true;
				}
			++decode_step;
			test_string=tmp_pos+1;
			} while ((decode_step<78)&&!break_loop);
		}

	for ( i = 0; i < 77; ++i )
		{
		pProfessionChance[i].uTotalprofChance=0;
		for ( int ii = 1; ii < 59; ++ii )
			{
			pProfessionChance[i].uTotalprofChance+=pProfessionChance[i].professionChancePerArea[ii];
			}
		pProfessionChance[i].professionChancePerArea[0]=0;
		pProfessionChance[i].professionChancePerArea[59]=0;
		}

	free(pNPCDistTXT_Raw);
	pNPCDistTXT_Raw = nullptr;
	}

//----- (00476C60) --------------------------------------------------------
void NPCStats::_476C60()
	{
	for (unsigned int i = 1; i < uNumNewNPCs; ++i)
		pNewNPCData[i].pName = pNPCUnicNames[i - 1];

	if (pParty->pHirelings[0].pName)
		pParty->pHirelings[0].pName = pParty->pHireling1Name;
	if (pParty->pHirelings[1].pName)
		pParty->pHirelings[1].pName = pParty->pHireling2Name;
	}

//----- (00476CB5) --------------------------------------------------------
void NPCStats::InitializeNPCData()
	{
	int i;
	char* test_string;
	unsigned char c;
	bool break_loop;
	unsigned int temp_str_len;
	char* tmp_pos;
	int decode_step;

	pNPCDataTXT_Raw = (char *)pEvents_LOD->LoadRaw("npcdata.txt", 0);
	strtok(pNPCDataTXT_Raw, "\r");
	strtok(NULL, "\r");

	for (i=0; i<500; ++i)
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
				{  //i+1
				switch (decode_step)
					{
				case 1:
					pNPCUnicNames[i] = RemoveQuotes(test_string);
					pNPCData[i+1].pName=pNPCUnicNames[i];
					break;
				case 2:
					pNPCData[i+1].uPortraitID = atoi(test_string);
					break;
				case 6:
					pNPCData[i+1].Location2D = atoi(test_string);
					break;
				case 7:
					pNPCData[i+1].uProfession = atoi(test_string);
					break;
				case 8:
					pNPCData[i+1].greet = atoi(test_string);
					break;
				case 9:
					pNPCData[i+1].joins = (*test_string == 'y')?1:0;
					break;
				case 10:
					pNPCData[i+1].evt_A = atoi(test_string);
					break;
				case 11:
					pNPCData[i+1].evt_B = atoi(test_string);
					break;
				case 12:
					pNPCData[i+1].evt_C = atoi(test_string);
					break;
				case 13:
					pNPCData[i+1].evt_D = atoi(test_string);
					break;
				case 14:
					pNPCData[i+1].evt_E = atoi(test_string);
					break;
				case 15:
					pNPCData[i+1].evt_F = atoi(test_string);
					break;
					}
				}
			++decode_step;
			test_string=tmp_pos+1;
			} while ((decode_step<16)&&!break_loop);
		}
	uNumNewNPCs = 501;
	pNPCGreetTXT_Raw = (char*)pEvents_LOD->LoadRaw("npcgreet.txt", 0);
	strtok(pNPCGreetTXT_Raw, "\r");
	for ( i = 1; i <= 205; ++i )
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
				{  //i+1
				switch (decode_step)
					{
				case 1:
					pNPCGreetings[i].pGreetings[0] = RemoveQuotes(test_string);
					break;
				case 2:
					pNPCGreetings[i].pGreetings[1] = RemoveQuotes(test_string);
					break;
					}
				}
			++decode_step;
			test_string=tmp_pos+1;
			} while ((decode_step<3)&&!break_loop);
		}

	pNCPGroupTXT_Raw = (char*)pEvents_LOD->LoadRaw("npcgroup.txt", 0);
	strtok(pNCPGroupTXT_Raw, "\r");

	for (i=0; i<51; ++i)
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
				{  //i+1
				if (decode_step==1)
					{
					pGroups[i] = atoi(test_string);
					}
				}
			++decode_step;
			test_string=tmp_pos+1;
			} while ((decode_step<2)&&!break_loop);
		}

	pNPCNewsTXT_Raw = (char*)pEvents_LOD->LoadRaw("npcnews.txt", 0);
	strtok(pNPCNewsTXT_Raw, "\r");


	for (i=0; i<51; ++i)
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
				{  //i+1
				if (decode_step==1)
					pCatchPhrases[i] = RemoveQuotes(test_string);
				}
			++decode_step;
			test_string=tmp_pos+1;
			} while ((decode_step<2)&&!break_loop);
		}
	}

//----- (0047702F) --------------------------------------------------------
void NPCStats::Initialize()
		{
		int i;
		char* test_string;
		unsigned char c;
		bool break_loop;
		unsigned int temp_str_len;
		char* tmp_pos;
		int decode_step;

		InitializeNPCData();
		InitializeNPCText();
		InitializeQuests();
		InitializeAutonotes();
		InitializeAwards();
		InitializeTransitions();
		InitializeMerchants();
		InitializeScrolls();

		pNPCNamesTXT_Raw = (char *)pEvents_LOD->LoadRaw("npcnames.txt", 0);
		strtok(pNPCNamesTXT_Raw, "\r");

		uNewlNPCBufPos = 0;

		for (i=0; i<540; ++i)
			{
			test_string = strtok(NULL, "\r") + 1;
			break_loop = false;
			decode_step=0;
			do 
				{
				c = *(unsigned char*)test_string;
				temp_str_len = 0;
				if (c=='\t')
					{
					if ( (decode_step == 1)&&(!uNumNPCNames[1]))
						uNumNPCNames[1]=i;
					}
				else
					{
					while((c!='\n')&&(c!='\t')&&(c>0))
						{
						++temp_str_len;
						c=test_string[temp_str_len];
						}		
					tmp_pos=test_string+temp_str_len;
					if (*tmp_pos == 0)
						break_loop = true;

					if (temp_str_len)
						{
						*tmp_pos = 0;
						if ( decode_step == 0)
							pNPCNames[i][0] =RemoveQuotes(test_string);
						else if ( decode_step == 1)
							pNPCNames[i][1] =RemoveQuotes(test_string);
						}
					else
						{ 
						if ( (decode_step == 1)&&(!uNumNPCNames[1]))
							uNumNPCNames[1]=i;
						}
					}
				++decode_step;
				test_string=tmp_pos+1;
				} while ((decode_step<2)&&!break_loop);
			}
		uNumNPCNames[0] = i;

		pNPCProfTXT_Raw = (char *)pEvents_LOD->LoadRaw("npcprof.txt", 0);
		strtok(pNPCProfTXT_Raw, "\r");
		strtok(NULL, "\r");
		strtok(NULL, "\r");
		strtok(NULL, "\r");

		for (i=1; i<59; ++i)
			{
			test_string = strtok(NULL, "\r") + 1;
			break_loop = false;
			decode_step=0;
			do 
				{
                //while (*test_string == '\t')  // some steps are separated by multiple \t's
                  //++test_string;
                
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
					switch(decode_step)
						{
					case 2:
						pProfessions[i].uHirePrice = atoi(test_string);
						break;
					case 3:
						pProfessions[i].pActionText = RemoveQuotes(test_string);
						break;
					case 4:
						pProfessions[i].pBenefits= RemoveQuotes(test_string);
						break;
					case 5:
						pProfessions[i].pJoinText = RemoveQuotes(test_string);
						break;
					case 6:
						pProfessions[i].pDismissText = RemoveQuotes(test_string);
						}
					}
				else
					{ 
					if (!decode_step)
						break_loop = true;
					}
				++decode_step;
				test_string=tmp_pos+1;
				} while ((decode_step<7)&&!break_loop);
			}
		uNumNPCProfessions = 59;
		}

//----- (00477266) --------------------------------------------------------
void NPCStats::Release()
	{
	free(pNPCTopicTXT_Raw);
	pNPCTopicTXT_Raw = nullptr;
	free(pNPCTextTXT_Raw);
	pNPCTextTXT_Raw = nullptr;
	free(pNPCNewsTXT_Raw);
	pNPCNewsTXT_Raw = nullptr;
	free(pNPCProfTXT_Raw);
	pNPCProfTXT_Raw = nullptr;
	free(pNPCNamesTXT_Raw);
	pNPCNamesTXT_Raw = nullptr;
	free(pNPCDataTXT_Raw);
	pNPCDataTXT_Raw = nullptr;
	free(pNPCDistTXT_Raw);
	pNPCDistTXT_Raw = nullptr;
	free(pNPCGreetTXT_Raw);
	pNPCGreetTXT_Raw = nullptr;
	free(pNCPGroupTXT_Raw);
	pNCPGroupTXT_Raw = nullptr;
	}

//----- (0047730C) --------------------------------------------------------
bool  CheckPortretAgainstSex(int a1, int)
	{
	return true;
	}
// 47730C: using guessed type int __stdcall const_1(int);

//----- (0047732C) --------------------------------------------------------
void NPCStats::InitializeAdditionalNPCs(NPCData *pNPCDataBuff, int npc_uid, int uLocation2D, int uMapId)
	{
	int rep_gen;
	int uNPCSex; // esi@1
	int uGeneratedPortret; // ecx@23
	int test_prof_summ; // ecx@37
	int gen_profession; // eax@37
	int max_prof_cap; // edx@37
//	signed int result; // eax@39
	int uRace; // [sp+Ch] [bp-Ch]@1
	bool break_gen; // [sp+10h] [bp-8h]@1
	signed int gen_attempts; // [sp+14h] [bp-4h]@1
	int uPortretMin; // [sp+24h] [bp+Ch]@1
	int uPortretMax;

	static const unsigned __int8 NPCSexGenTable[86] ={
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                          
		1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0,
		0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0 };
	static const unsigned __int8 NPCRaceGenTable[86] ={
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0};

	unsigned __int8 seed = (unsigned __int8)((double)(npc_uid - 1)/3.0);
	uNPCSex = NPCSexGenTable[seed];
	uRace = NPCRaceGenTable[seed];
	pNPCDataBuff->uSex = uNPCSex;
	pNPCDataBuff->pName = pNPCNames[rand() % uNumNPCNames[uNPCSex]][uNPCSex];

	gen_attempts = 0;
	break_gen = false;

	do
		{
		switch ( uRace )
			{
		case 0:
			if ( uNPCSex == 0 )
				{
				uPortretMin = 2;
				uPortretMax = 100;
				}
			else
				{
				uPortretMin = 201;
				uPortretMax =  250;
				}
		case 1:
			if ( uNPCSex == 0 )
				{
				uPortretMin = 400;
				uPortretMax = 430;
				}
			else
				{
				uPortretMin = 460;
				uPortretMax =  490;
				}
			break;
		case 2:
			if ( uNPCSex == 0 )
				{
				uPortretMin = 500;
				uPortretMax =  520;
				}
			else
				{
				uPortretMin = 530;
				uPortretMax = 550;	
				}
			break;
		case 3:
			if ( uNPCSex == 0 )
				{
				uPortretMin = 300;
				uPortretMax = 330;
				}
			else
				{
				uPortretMin = 360;
				uPortretMax = 387;
				}

			break;
			}

		uGeneratedPortret = uPortretMin + rand() % (uPortretMax - uPortretMin + 1);
		if ( CheckPortretAgainstSex(uGeneratedPortret, uNPCSex))
			break_gen = true;
		++gen_attempts;
		if ( gen_attempts >= 4 )
			{
			uGeneratedPortret = uPortretMin;
			break_gen = true;
			}
		}
		while(!break_gen);

		pNPCDataBuff->uPortraitID = uGeneratedPortret;
		pNPCDataBuff->uFlags = 0;
		pNPCDataBuff->fame = 0;
		//generate reputation
		rep_gen = rand() % 100 + 1;

		if ( rep_gen >= 60 )
			{
			if ( rep_gen >= 90 )
				{
				if ( rep_gen >= 95 )
					{
					if ( rep_gen >= 98 )
						pNPCDataBuff->rep = -600;
					else
						pNPCDataBuff->rep = 400;
					}
				else
					pNPCDataBuff->rep = -300;
				}
			else
				pNPCDataBuff->rep = 200;
			}
		else
			pNPCDataBuff->rep = 0;

		max_prof_cap = rand() % pProfessionChance[uMapId].uTotalprofChance+1;
		test_prof_summ = 0;
		gen_profession = 0;

		if ( max_prof_cap > 0 )
			{
			do
				test_prof_summ += pProfessionChance[uMapId].professionChancePerArea[gen_profession++];
			while ( test_prof_summ < max_prof_cap );
			}
		pNPCDataBuff->uProfession = gen_profession - 1;
		pNPCDataBuff->Location2D = uLocation2D;
		pNPCDataBuff->field_24 = 1;
		pNPCDataBuff->joins = 1;
		pNPCDataBuff->evt_A = 0;
		pNPCDataBuff->evt_B = 0;
		pNPCDataBuff->evt_C = 0;
		pNPCDataBuff->evt_D = 0;
		pNPCDataBuff->evt_E = 0;
		pNPCDataBuff->evt_F = 0;
	}


//----- (00495366) --------------------------------------------------------
char *NPCStats::sub_495366_MispronounceName(unsigned __int8 firstLetter, unsigned __int8 genderId)
{
  int pickedName; // edx@2

  if ( firstLetter == dword_AE336C_LastMispronouncedNameFirstLetter)
    pickedName = dword_AE3370_LastMispronouncedNameResult;
  else
  {
    dword_AE336C_LastMispronouncedNameFirstLetter = firstLetter;
    if ( this->uNumNPCNames[genderId] == 0 )
      pickedName = rand() % this->uNumNPCNames[(genderId + 1) % 2];  //originally without " + 1) % 2", but that would yield a div by zero
    else
    {
      int rangeBottom = 0;
      int rangeTop = 0;
      for ( uint i = 0; i < this->uNumNPCNames[genderId]; ++i )
      {
        if (tolower(this->pNPCNames[i][genderId][0]))
        {
          if ( rangeBottom )
            rangeTop = i;
          else
            rangeBottom = i;
        }
      }
      if ( rangeTop != 0 )
        pickedName = rangeBottom + rand() % (rangeTop - rangeBottom);
      else
        pickedName = rand() % this->uNumNPCNames[genderId];
    }
  }
  dword_AE3370_LastMispronouncedNameResult = pickedName;
  return this->pNPCNames[pickedName][genderId];
}

//----- (00476387) --------------------------------------------------------
bool PartyHasDragon()
{
  return pNPCStats->pNewNPCData[57].Hired();
}

//----- (00476395) --------------------------------------------------------
//0x26 Wizard eye at skill level 2
bool  CheckHiredNPCSpeciality(unsigned int uProfession)
    {

    if ( bNoNPCHiring == 1 )
        return 0;

    for (uint i=0; i<pNPCStats->uNumNewNPCs; ++i )
        {
        if ( pNPCStats->pNewNPCData[i].uProfession == uProfession && 
            (pNPCStats->pNewNPCData[i].uFlags & 0x80) )//Uninitialized memory access
            return true;
        }
    if ( pParty->pHirelings[0].uProfession == uProfession ||
         pParty->pHirelings[1].uProfession == uProfession)
        return true;
    else
        return false;

    }

//----- (004763E0) --------------------------------------------------------
void  InitializeAwards()
{
	int i;
	char* test_string;
	unsigned char c;
	bool break_loop;
	unsigned int temp_str_len;
	char* tmp_pos;
	int decode_step;

	free(pAwardsTXT_Raw);
	pAwardsTXT_Raw = (char *)pEvents_LOD->LoadRaw("awards.txt", 0);
	strtok(pAwardsTXT_Raw, "\r");

	for (i=1; i<105; ++i)
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
				if (decode_step==1)
					pAwards[i].pText=RemoveQuotes(test_string);
				else if (decode_step==2)
					pAwards[i].uPriority = atoi(test_string);
				}
			else
				{ 
				break_loop = true;
				}
			++decode_step;
			test_string=tmp_pos+1;
			} while ((decode_step<3)&&!break_loop);
		}
	}
// 7241C8: using guessed type int dword_7241C8;

//----- (004764C2) --------------------------------------------------------
void  InitializeScrolls()
	{

	int i;
	char* test_string;
	unsigned char c;
	bool break_loop;
	unsigned int temp_str_len;
	char* tmp_pos;
	int decode_step;

	free(pScrollsTXT_Raw);
	pScrollsTXT_Raw = (char *)pEvents_LOD->LoadRaw("scroll.txt", 0);
	strtok(pScrollsTXT_Raw, "\r");
	for (i=0; i<82; ++i)
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
				if ( decode_step == 1)
					pScrolls[i]=RemoveQuotes(test_string);
				}
			else
				{ 
				break_loop = true;
				}
			++decode_step;
			test_string=tmp_pos+1;
			} while ((decode_step<2)&&!break_loop);
		}
	}

//----- (00476590) --------------------------------------------------------
void  InitializeMerchants()
	{
	int i;
	char* test_string;
	unsigned char c;
	bool break_loop;
	unsigned int temp_str_len;
	char* tmp_pos;
	int decode_step;

	free(pMerchantsTXT_Raw);
	pMerchantsTXT_Raw = (char *)pEvents_LOD->LoadRaw("merchant.txt", 0);
	strtok(pMerchantsTXT_Raw, "\r");

	for (i=0; i<7; ++i)
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
					pMerchantsBuyPhrases[i]=RemoveQuotes(test_string);
					break;
				case 2:
					pMerchantsSellPhrases[i]=RemoveQuotes(test_string);
					break;
				case 3:
					pMerchantsRepairPhrases[i]=RemoveQuotes(test_string); 
					break;
				case 4:
					pMerchantsIdentifyPhrases[i]=RemoveQuotes(test_string); 
					break;
					}
				}
			else
				{ 
				break_loop = true;
				}
			++decode_step;
			test_string=tmp_pos+1;
			} while ((decode_step<5)&&!break_loop);
		}

	}

//----- (00476682) --------------------------------------------------------
void InitializeTransitions()
	{
	int i;
	char* test_string;
	unsigned char c;
	bool break_loop;
	unsigned int temp_str_len;
	char* tmp_pos;
	int decode_step;

	free(pTransitionsTXT_Raw);
	pTransitionsTXT_Raw = (char *)pEvents_LOD->LoadRaw("trans.txt", 0);
	strtok(pTransitionsTXT_Raw, "\r");

	for (i=0; i<464; ++i)
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
				if ( decode_step == 1)
					pTransitionStrings[i + 1]=RemoveQuotes(test_string);
				}
			else
				{ 
				break_loop = true;
				}
			++decode_step;
			test_string=tmp_pos+1;
			} while ((decode_step<2)&&!break_loop);
		}
	}

//----- (00476750) --------------------------------------------------------
void  InitializeAutonotes()
	{
	int i;
	char* test_string;
	unsigned char c;
	bool break_loop;
	unsigned int temp_str_len;
	char* tmp_pos;
	int decode_step;

	free(pAutonoteTXT_Raw);
	pAutonoteTXT_Raw = (char *)pEvents_LOD->LoadRaw("autonote.txt", 0);
	strtok(pAutonoteTXT_Raw, "\r");

	for (i=0; i<195; ++i)
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
				case  1:
					pAutonoteTxt[i+1].pText=RemoveQuotes(test_string);
					break;
				case  2:
					{
					if ( !_stricmp(test_string, "potion"))
						{
						pAutonoteTxt[i+1].eType = AUTONOTE_POTION_RECEPIE;
						break;
						}
					if ( !_stricmp(test_string, "stat") )
						{
						pAutonoteTxt[i+1].eType = AUTONOTE_STAT_HINT;
						break;
						}
					if ( !_stricmp(test_string, "seer") )
						{
						pAutonoteTxt[i+1].eType = AUTONOTE_SEER;
						break;
						}
					if ( !_stricmp(test_string, "obelisk") )
						{
						pAutonoteTxt[i+1].eType = AUTONOTE_OBELISK;
						break;
						}
					if ( !_stricmp(test_string, "teacher") )
						{
						pAutonoteTxt[i+1].eType = AUTONOTE_TEACHER;
						break;
						}
					pAutonoteTxt[i+1].eType =AUTONOTE_MISC;
					break;
					}
					}
				}
			else
				{ 
				break_loop = true;
				}
			++decode_step;
			test_string=tmp_pos+1;
			} while ((decode_step<3)&&!break_loop);
		}
	}


//----- (004768A9) --------------------------------------------------------
void  InitializeQuests()
	{
	int i;
	char* test_string;
	unsigned char c;
	bool break_loop;
	unsigned int temp_str_len;
	char* tmp_pos;
	int decode_step;

	free(pQuestsTXT_Raw);
	pQuestsTXT_Raw = (char *)pEvents_LOD->LoadRaw("quests.txt", 0);
	strtok(pQuestsTXT_Raw, "\r");
    memset(pQuestTable.data(),0, sizeof(pQuestTable));
	for (i=0; i<512; ++i)
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
				if ( decode_step == 1)
					pQuestTable[i+1] =RemoveQuotes(test_string);
				}
			else
				{ 
				break_loop = true;
				}
			++decode_step;
			test_string=tmp_pos+1;
			} while ((decode_step<2)&&!break_loop);
		}
	}

//----- (004B2001) --------------------------------------------------------
void __fastcall ClickNPCTopic(signed int uMessageParam)
{
  //signed int v1; // eax@1
  NPCData *pCurrentNPCInfo; // ebp@1
  int pEventNumber; // ecx@8
  Player *v4; // esi@20
  //int v5; // eax@28
  //int v6; // eax@31
  //int v7; // eax@34
  //int v8; // eax@37
  //int v9; // eax@40
  //unsigned int v10; // eax@43
  char *v12; // eax@53
  char *v13; // eax@56
  char *v14; // eax@57
  char *v15; // eax@58
  //unsigned int v16; // ebp@62
  char *v17; // ecx@63
  char *v18; // eax@65
//  const char *v19; // ecx@68
  //unsigned int v20; // eax@69
  signed int pPrice; // ecx@70
  char *v22; // [sp-Ch] [bp-18h]@73
  //int v23; // [sp-8h] [bp-14h]@49
  char *v24; // [sp-8h] [bp-14h]@73
  //int v25; // [sp-4h] [bp-10h]@49

  uDialogueType = uMessageParam + 1;
  pCurrentNPCInfo = HouseNPCData[pDialogueNPCCount -((dword_591080 != 0)?1:0 )];//- 1
  if ( uMessageParam <= 24 )
  {
  switch ( uMessageParam )
  {
    case 13:
      current_npc_text = pNPCStats->pProfessions[pCurrentNPCInfo->uProfession].pJoinText;//(char *)*(&pNPCStats->field_13A64 + 5 * v2->uProfession);
      current_npc_text = BuildDialogueString(current_npc_text, uActiveCharacter - 1, 0, 0, 0, 0);
      NPCHireableDialogPrepare();
      dialogue_show_profession_details = false;
      goto _return;
    case 19:
      pEventNumber = pCurrentNPCInfo->evt_A;
      break;
    case 20:
      pEventNumber = pCurrentNPCInfo->evt_B;
      break;
    case 21:
      pEventNumber = pCurrentNPCInfo->evt_C;
      break;
    case 22:
      pEventNumber = pCurrentNPCInfo->evt_D;
      break;
    case 23:
      pEventNumber = pCurrentNPCInfo->evt_E;
      break;
    case 24:
      pEventNumber = pCurrentNPCInfo->evt_F;
      break;
    default:
      goto _return;
  }
  /*switch ( pEventNumber )
  {
    case 139:
      OracleDialogue();
      goto _return;
    case 311:
      CheckBountyRespawnAndAward();
    goto _return;
  }*/
  if ( pEventNumber < 200 || pEventNumber > 310 )
    {
      if ( pEventNumber < 400 || pEventNumber > 410 )
      {
        if ( pEventNumber == 139 )
        {
          OracleDialogue();
        }
        else
        { 
          if ( pEventNumber == 311 )
          {
            CheckBountyRespawnAndAward();
          }
          else
          {
            current_npc_text = 0;
            activeLevelDecoration = (LevelDecoration*)1;
            EventProcessor(pEventNumber, 0, 1);
            activeLevelDecoration = nullptr;
          }
        }
      }
      else
      {
        dword_F8B1D8 = uMessageParam;
        DrawJoinGuildWindow(pEventNumber - 400);
      }
    }
    else
    {
      _4B3FE5_training_dialogue(pEventNumber);
    }
    goto _return;
  }
  if ( uMessageParam != 76 )
  {
    if ( uMessageParam == 77 )
    {
      //v16 = pCurrentNPCInfo->uProfession;
    __debugbreak();  // probably hirelings found in buildings, not present in MM7, changed "pCurrentNPCInfo->uProfession - 1" to "pCurrentNPCInfo->uProfession", have to check in other versions whether it's ok
      if (dialogue_show_profession_details)
        v17 = pNPCStats->pProfessions[pCurrentNPCInfo->uProfession].pJoinText;
      else
        v17 = pNPCStats->pProfessions[pCurrentNPCInfo->uProfession].pBenefits;
      current_npc_text = v17;
      v18 = BuildDialogueString(v17, uActiveCharacter - 1, 0, 0, 0, 0);
      dialogue_show_profession_details = ~dialogue_show_profession_details;
      current_npc_text = v18;
    }
    else
    {
      if ( uMessageParam == 79 )
      {
        if ( contract_approved )
        {
          Party::TakeGold(gold_transaction_amount);
          if ( uActiveCharacter )
          {
            v12 = (char *)&pPlayers[uActiveCharacter]->pActiveSkills[dword_F8B1AC_award_bit_number];
            *(short *)v12 &= 0x3Fu;
            switch ( dword_F8B1B0_MasteryBeingTaught )
            {
              case 2:
                v15 = (char *)&pPlayers[uActiveCharacter]->pActiveSkills[dword_F8B1AC_award_bit_number];
                *v15 |= 0x40u;
                break;
              case 3:
                v14 = (char *)&pPlayers[uActiveCharacter]->pActiveSkills[dword_F8B1AC_award_bit_number];
                *v14 |= 0x80u;
                break;
              case 4:
                v13 = (char *)&pPlayers[uActiveCharacter]->pActiveSkills[dword_F8B1AC_award_bit_number];
                v13[1] |= 1u;
                break;
            }
            pPlayers[uActiveCharacter]->PlaySound(SPEECH_85, 0);
          }
          pMessageQueue_50CBD0->AddGUIMessage(UIMSG_Escape, 1, 0);
          /*if ( (signed int)pMessageQueue_50CBD0->uNumMessages < 40 )
          {
            pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].eType = UIMSG_Escape;
            pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].param = 1;
            *(&pMessageQueue_50CBD0->uNumMessages + 3 * pMessageQueue_50CBD0->uNumMessages + 3) = 0;
            ++pMessageQueue_50CBD0->uNumMessages;
          }*/
        }
      }
      else
      {
        if ( uMessageParam == 82 && contract_approved ) //join guild
        {
          Party::TakeGold(gold_transaction_amount);
          v4 = pParty->pPlayers.data();
          do
          {
            v4->SetVariable(VAR_Award, dword_F8B1AC_award_bit_number);
            ++v4;
          }
          while ( (signed int)v4 < (signed int)pParty->pHirelings.data() );
          switch ( dword_F8B1D8 )
          {
            case 19:
              pEventNumber = pCurrentNPCInfo->evt_A;
              if ( pEventNumber >= 400 && pEventNumber <= 416 )
                pCurrentNPCInfo->evt_A = 0;
              break;
            case 20:
              pEventNumber = pCurrentNPCInfo->evt_B;
              if ( pEventNumber >= 400 && pEventNumber <= 416 )
                pCurrentNPCInfo->evt_B = 0;
              break;
            case 21:
              pEventNumber = pCurrentNPCInfo->evt_C;
              if ( pEventNumber >= 400 && pEventNumber <= 416 )
                pCurrentNPCInfo->evt_C = 0;
              break;
            case 22:
              pEventNumber = pCurrentNPCInfo->evt_D;
              if ( pEventNumber >= 400 && pEventNumber <= 416 )
                pCurrentNPCInfo->evt_D = 0;
              break;
            case 23:
              pEventNumber = pCurrentNPCInfo->evt_E;
              if ( pEventNumber >= 400 && pEventNumber <= 416 )
                pCurrentNPCInfo->evt_E = 0;
              break;
            case 24:
              pEventNumber = pCurrentNPCInfo->evt_F;
              if ( pEventNumber >= 400 && pEventNumber <= 416)
                pCurrentNPCInfo->evt_F = 0;
              break;
          }
          pMessageQueue_50CBD0->AddGUIMessage(UIMSG_Escape, 1, 0);
          /*if ( (signed int)pMessageQueue_50CBD0->uNumMessages < 40 )
          {
            pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].eType = UIMSG_Escape;
            pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].param = 1;
            *(&pMessageQueue_50CBD0->uNumMessages + 3 * pMessageQueue_50CBD0->uNumMessages + 3) = 0;
            ++pMessageQueue_50CBD0->uNumMessages;
          }*/
          //v11 = uActiveCharacter;
          if ( uActiveCharacter )
          {
            pPlayers[uActiveCharacter]->PlaySound((PlayerSpeech)SPEECH_86, 0);
            goto _return;
          }
        }
      }
    }
    goto _return;
  }
  if ( pParty->pHirelings[0].pName && pParty->pHirelings[1].pName )
  {
    ShowStatusBarString(pGlobalTXT_LocalizationStrings[533], 2);// ""I cannot join you, you're party is full""
    goto _return;
  }
  if ( pCurrentNPCInfo->uProfession != 51 ) //burglars have no hiring price
  {
    __debugbreak();  // probably hirelings found in buildings, not present in MM7, changed "pCurrentNPCInfo->uProfession - 1" to "pCurrentNPCInfo->uProfession", have to check in other versions whether it's ok
    pPrice = pNPCStats->pProfessions[pCurrentNPCInfo->uProfession].uHirePrice;
    if ( pParty->uNumGold < (unsigned int)pPrice )
    {
      ShowStatusBarString(pGlobalTXT_LocalizationStrings[155], 2);
      dialogue_show_profession_details = false;
      uDialogueType = 13;
      current_npc_text = pNPCStats->pProfessions[pCurrentNPCInfo->uProfession].pJoinText;
      current_npc_text = BuildDialogueString(current_npc_text, uActiveCharacter - 1, 0, 0, 0, 0);
      if ( uActiveCharacter )
        pPlayers[uActiveCharacter]->PlaySound(SPEECH_NotEnoughGold, 0);
      ShowStatusBarString(pGlobalTXT_LocalizationStrings[155], 2);
      goto _return;
    }
    Party::TakeGold(pPrice);
  }
  //LOBYTE(v2->uFlags) |= 0x80u;
  pCurrentNPCInfo->uFlags |= 128;
  pParty->hirelingScrollPosition = 0;
  pParty->CountHirelings();
  if ( pParty->pHirelings[0].pName )
  {
    memcpy(&pParty->pHirelings[1], pCurrentNPCInfo, sizeof(pParty->pHirelings[1]));
    v24 = pCurrentNPCInfo->pName;
    v22 = pParty->pHireling2Name;
  }
  else
  {
    memcpy(pParty->pHirelings.data(), pCurrentNPCInfo, 0x4Cu);
    v24 = pCurrentNPCInfo->pName;
    v22 = pParty->pHireling1Name;
  }
  strcpy(v22, v24);
  pParty->hirelingScrollPosition = 0;
  pParty->CountHirelings();
  PrepareHouse((HOUSE_ID)(int)window_SpeakInHouse->ptr_1C);
  dialog_menu_id = HOUSE_DIALOGUE_MAIN;

  pMessageQueue_50CBD0->AddGUIMessage(UIMSG_Escape, 1, 0);
  /*if ( (signed int)pMessageQueue_50CBD0->uNumMessages < 40 )
  {
    pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].eType = UIMSG_Escape;
    pMessageQueue_50CBD0->pMessages[pMessageQueue_50CBD0->uNumMessages].param = 1;
    *(&pMessageQueue_50CBD0->uNumMessages + 3 * pMessageQueue_50CBD0->uNumMessages + 3) = 0;
    ++pMessageQueue_50CBD0->uNumMessages;
  }*/
  if ( uActiveCharacter )
    pPlayers[uActiveCharacter]->PlaySound((PlayerSpeech)61, 0);
_return:
  BackToHouseMenu();
}
//----- (004B29F2) --------------------------------------------------------
const char * ContractSelectText( int pEventCode )
{
  static const int dialogue_base=110;
  contract_approved = 0;
  dword_F8B1AC_award_bit_number = pEventCode + 50;
  gold_transaction_amount = price_for_membership[pEventCode];
  if ( pPlayers[uActiveCharacter]->CanAct() )
  {
    if ( (unsigned __int16)_449B57_test_bit((unsigned __int8 *)pPlayers[uActiveCharacter]->_achieved_awards_bits, dword_F8B1AC_award_bit_number) )
      return pNPCTopics[dialogue_base+13].pText;
    else
    {
      if ( (unsigned int)gold_transaction_amount <= pParty->uNumGold )
      {
        contract_approved = 1;
        return pNPCTopics[pEventCode + dialogue_base].pText;
      }
      else
        return pNPCTopics[dialogue_base+14].pText; 
    }
  }
  else
    return pNPCTopics[dialogue_base+12].pText; 
}
//----- (004B40E6) --------------------------------------------------------
void NPCHireableDialogPrepare()
    {
  signed int v0; // ebx@1
  NPCData *v1; // edi@1

  v0 = 0;
  v1 = HouseNPCData[(unsigned int)((char *)pDialogueNPCCount + -(dword_591080 != 0) )];//- 1
  pDialogueWindow->Release();
  pDialogueWindow = GUIWindow::Create(0, 0, window->GetWidth(), 350, WINDOW_MainMenu, 0, 0);
  pBtn_ExitCancel = pDialogueWindow->CreateButton( 471, 0x1BDu,  0xA9u,   0x23u,  1,  0,  UIMSG_Escape,  0,   0,
                 pGlobalTXT_LocalizationStrings[34], //"Cancel"
                 pIcons_LOD->GetTexture(uExitCancelTextureId),
                 0);
  pDialogueWindow->CreateButton(0, 0, 0, 0, 1, 0, UIMSG_BuyInShop_Identify_Repair, 0, 0, "", 0);
  if ( pNPCStats->pProfessions[v1->uProfession].pBenefits)//*(&pNPCStats->field_13A5C + 5 * v1->uProfession) )
  {
    pDialogueWindow->CreateButton( 480,  0xA0u,  0x8Cu,  0x1Eu,   1,  0,  UIMSG_ClickNPCTopic,  0x4Du,   0,
      pGlobalTXT_LocalizationStrings[407], 0);//"More Information"   
    v0 = 1;
  }
  pDialogueWindow->CreateButton(  0x1E0u,  30 * v0 + 160,  0x8Cu,  0x1Eu,  1,  0,  UIMSG_ClickNPCTopic,  0x4Cu,  0,
    pGlobalTXT_LocalizationStrings[406],  0); //"Hire"
  pDialogueWindow->_41D08F_set_keyboard_control_group(v0 + 1, 1, 0, 2);
  dialog_menu_id = HOUSE_DIALOGUE_OTHER;
}

//----- (004B4224) --------------------------------------------------------
void _4B4224_UpdateNPCTopics( int _this )
	{
  int num_menu_buttons; // ebx@1
  int i; // ebp@5
 // signed int v4; // ebp@9
  int v6; // eax@16
  int v8; // eax@21
  int v10; // eax@26
  int v12; // eax@31
  int v14; // eax@36
  int v16; // eax@41
  NPCData *v17; // [sp+10h] [bp-4h]@4

  num_menu_buttons = 0;
  pDialogueNPCCount = (_this + 1);
  if ( _this + 1 == uNumDialogueNPCPortraits && uHouse_ExitPic )
  {
    pDialogueWindow->Release();
    pDialogueWindow = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_MainMenu, 0, 0);
    sprintfex(sHouseName.data(), pGlobalTXT_LocalizationStrings[LOCSTR_ENTER_S], pMapStats->pInfos[uHouse_ExitPic].pName);
    pBtn_ExitCancel = pDialogueWindow->CreateButton(566, 445, 75, 33, 1, 0, UIMSG_Escape, 0, 'N', pGlobalTXT_LocalizationStrings[34], pIcons_LOD->GetTexture(uTextureID_BUTTDESC2), 0);// "Cancel"
    pBtn_YES        = pDialogueWindow->CreateButton(486, 445, 75, 33, 1, 0, UIMSG_BF,     1, 'Y', sHouseName.data(), pIcons_LOD->GetTexture(uTextureID_BUTTYES2), 0);
    pDialogueWindow->CreateButton( pNPCPortraits_x[0][0], pNPCPortraits_y[0][0], 63u, 73u, 1, 0,  UIMSG_BF, 1u, 0x20u,  sHouseName.data(), 0);
    pDialogueWindow->CreateButton(8, 8, 460, 344, 1, 0, UIMSG_BF, 1, 0x59u, sHouseName.data(), 0);
  }
  else
  {
    v17 = HouseNPCData[_this + 1 - ((dword_591080 != 0)?1:0 )];//+ 1
    if ( dialog_menu_id == HOUSE_DIALOGUE_OTHER )
    {
      pDialogueWindow->Release();
    }
    else
    {
      for ( i = 0; i < uNumDialogueNPCPortraits; ++i )
        HouseNPCPortraitsButtonsList[i]->Release();
    }
    pDialogueWindow = GUIWindow::Create(0, 0, window->GetWidth(), 345, WINDOW_MainMenu, 0, 0);
    pBtn_ExitCancel = pDialogueWindow->CreateButton(  471,  445,  169, 35,  1,   0, UIMSG_Escape,  0,  0,
                   pGlobalTXT_LocalizationStrings[74],// "End Conversation"
                   pIcons_LOD->GetTexture(uExitCancelTextureId),   0);
    pDialogueWindow->CreateButton(8, 8, 450, 320, 1, 0, UIMSG_BuyInShop_Identify_Repair, 0, 0, "", 0);
    if ( pDialogueNPCCount == 1 && dword_591080 )
    {
      InitializaDialogueOptions(in_current_building_type);
    }
    else
    {
      if ( v17->joins )
      {
        num_menu_buttons = 1;
        pDialogueWindow->CreateButton(480u, 160u, 140u, 30, 1, 0, UIMSG_ClickNPCTopic, 0xDu, 0, "", 0);
      }
      if ( v17->evt_A)
      {
        if ( num_menu_buttons < 4 )
        {
          v6 = NPC_EventProcessor(v17->evt_A);
          if ( v6 == 1 || v6 == 2 )
            pDialogueWindow->CreateButton(  480u, 30 * num_menu_buttons++ + 160,  140u, 30u, 1, 0, UIMSG_ClickNPCTopic, 0x13u,  0, "",  0);
        }
      }
      if ( v17->evt_B )
      {
        if ( num_menu_buttons < 4 )
        {
          v8 = NPC_EventProcessor(v17->evt_B);
          if ( v8 == 1 || v8 == 2 )
            pDialogueWindow->CreateButton( 480u,  30 * num_menu_buttons++ + 160,  140u, 30u,  1, 0,  UIMSG_ClickNPCTopic,  0x14u,  0, "",  0);
        }
      }
      if ( v17->evt_C )
      {
        if ( num_menu_buttons < 4 )
        {
          v10 = NPC_EventProcessor(v17->evt_C);
          if ( v10 == 1 || v10 == 2 )
            pDialogueWindow->CreateButton(  480u,  30 * num_menu_buttons++ + 160,  140u, 30u,  1,  0, UIMSG_ClickNPCTopic, 0x15u, 0, "",  0);
        }
      }
 
      if ( v17->evt_D )
      {
        if ( num_menu_buttons < 4 )
        {
          v12 = NPC_EventProcessor(v17->evt_D);
          if ( v12 == 1 || v12 == 2 )
            pDialogueWindow->CreateButton( 0x1E0u, 30 * num_menu_buttons++ + 160,  0x8Cu, 0x1Eu, 1, 0, UIMSG_ClickNPCTopic, 0x16u,  0, "",  0);
        }
      }
      if ( v17->evt_E )
      {
        if ( num_menu_buttons < 4 )
        {
          v14 = NPC_EventProcessor(v17->evt_E);
          if ( v14 == 1 || v14 == 2 )
            pDialogueWindow->CreateButton( 0x1E0u, 30 * num_menu_buttons++ + 160,  0x8Cu,  0x1Eu,  1,  0,  UIMSG_ClickNPCTopic, 0x17u,  0, "",  0);
        }
      }
      if ( v17->evt_F )
      {
        if ( num_menu_buttons < 4 )
        {
          v16 = NPC_EventProcessor(v17->evt_F);
          if ( v16 == 1 || v16 == 2 )
            pDialogueWindow->CreateButton( 0x1E0u, 30 * num_menu_buttons++ + 160,  0x8Cu,  0x1Eu, 1, 0, UIMSG_ClickNPCTopic, 0x18u, 0, "",  0);
        }
      }
      pDialogueWindow->_41D08F_set_keyboard_control_group(num_menu_buttons, 1, 0, 2);
      dword_F8B1E0 = pDialogueWindow->pNumPresenceButton;
    }
    dialog_menu_id = HOUSE_DIALOGUE_MAIN;
  }
 
}
//----- (004466C4) --------------------------------------------------------
int NPC_EventProcessor(int npc_event_id, int entry_line)
	{
  signed int event_index; // ebp@1
  int evt_seq_num; // esi@3
  bool ready_to_exit; // [sp+Ch] [bp-Ch]@3
  signed int npc_activity; // [sp+10h] [bp-8h]@3
  int result;

  event_index = 0;
  if ( !npc_event_id )
    return 0;
  evt_seq_num = entry_line;
  pSomeOtherEVT = pGlobalEVT.data();
  uSomeOtherEVT_NumEvents = uGlobalEVT_NumEvents;
  memcpy(pSomeOtherEVT_Events.data(), pGlobalEVT_Index.data(), sizeof(EventIndex)*4400);
  npc_activity = 1;
  ready_to_exit = false;
  if ( uSomeOtherEVT_NumEvents <= 0 )
    return 2;
  do
  {
    if ( (pSomeOtherEVT_Events[event_index].uEventID == npc_event_id) && (pSomeOtherEVT_Events[event_index].event_sequence_num == evt_seq_num) )
    {
	  _evt_raw *_evt = (_evt_raw *)&pSomeOtherEVT[pSomeOtherEVT_Events[event_index].uEventOffsetInEVT];
	  switch(_evt->_e_type)
		  {
	  case EVENT_Exit:
		   //exit
		  if ( ready_to_exit )
			  result = npc_activity != 0;
		  else
			   result = 2;
		  return result;
		  break;
	  case EVENT_OnCanShowDialogItemCmp:
		  ready_to_exit = true;
		  //v8 = (unsigned __int8)v7[7] + (((unsigned __int8)v7[8] + (((unsigned __int8)v7[9] + ((unsigned __int8)v7[10] << 8)) << 8)) << 8);
		  for(int i=0; i<4; ++i)
			  {  
			//  if (pParty->pPlayers[i].CompareVariable((enum VariableType)((unsigned __int8)pSomeOtherEVT[v6 + 5] + ((unsigned __int8)pSomeOtherEVT[v6 + 6] << 8)),
				//  v8))
			  if (pParty->pPlayers[i].CompareVariable((enum VariableType)EVT_WORD(_evt->v5), EVT_DWORD(_evt->v7)))
				  {
				  event_index = -1;
				  evt_seq_num = EVT_BYTE(_evt->v11)-1;//(unsigned __int8)pSomeOtherEVT[v6 + 11] - 1;
				  break;
				  }
			}
		  break;
	  case EVENT_EndCanShowDialogItem :
		  if ( ready_to_exit )
			  result = npc_activity != 0;
		  else
			  result = 2;
		  return result;
		  break;
	  case EVENT_SetCanShowDialogItem :
		  ready_to_exit = true;
		  npc_activity = EVT_BYTE(_evt->v5); //(unsigned __int8)v7[5];
		  break;
	  case EVENT_IsActorAssasinated :
		//  if (IsActorAlive( (unsigned __int8)v7[5], 
		//	  (unsigned __int8)v7[6] + (((unsigned __int8)v7[7] + (((unsigned __int8)v7[8] + ((unsigned __int8)v7[9] << 8)) << 8)) << 8),
			//  (unsigned __int8)v7[10]) )
			if (IsActorAlive( EVT_BYTE(_evt->v5),  EVT_DWORD(_evt->v6), EVT_BYTE(_evt->v10)))
			  {  // drop linear sequense, going to new seq
				event_index = -1;
				evt_seq_num = EVT_BYTE(_evt->v11)-1;//(unsigned __int8)pSomeOtherEVT[v6 + 11] - 1;
			  }
		  break;	  
		  }
		++evt_seq_num;
    }
    ++event_index;
  }
  while ( event_index < uSomeOtherEVT_NumEvents );
  if ( ready_to_exit )
    result = npc_activity != 0;
  else
    result = 2;
  return result;
}
//----- (00445C8B) --------------------------------------------------------
int __fastcall GetGreetType(signed int SpeakingNPC_ID)
{
  signed int v1; // ebx@1
  int v3; // edi@6
  int v4; // ecx@6
  int v5; // edx@6
  NPCData *v6; // eax@6
  char *v7; // ebp@11
  NPCData *v8; // esi@11

  v1 = 0;
  if ( SpeakingNPC_ID >= 0 )
  {
    if ( SpeakingNPC_ID < 5000 )
      return 1;//QuestNPC_greet
    return 2;//HiredNPC_greet
  }
  if ( SpeakingNPC_ID >= 5000 )
    return 2;
  v3 = abs((int)sDialogue_SpeakingActorNPC_ID) - 1;
  v4 = 0;
  v5 = 0;
  v6 = pParty->pHirelings.data();
  do
  {
    if ( v6->pName )
      pTmpBuf[v4++] = v5;
    ++v6;
    ++v5;
  }
  while ( (signed int)v6 < (signed int)&pParty->pPickedItem );
  if ( (signed int)pNPCStats->uNumNewNPCs > 0 )
  {
    v7 = &pTmpBuf[v4];
    v8 = pNPCStats->pNewNPCData;
    do
    {
      if (v8->Hired() && (!pParty->pHirelings[0].pName || strcmp(v8->pName, pParty->pHirelings[0].pName)) )
      {
        if ( !pParty->pHirelings[1].pName || strcmp(v8->pName, pParty->pHirelings[1].pName) )
          *v7++ = v1 + 2;
      }
      ++v1;
      ++v8;
    }
    while ( v1 < (signed int)pNPCStats->uNumNewNPCs );
  }
  return ((unsigned __int8)pTmpBuf[v3] < 2) + 1;
}
//----- (00445308) --------------------------------------------------------
const char *GetProfessionActionText(int a1)
{
  if ( a1 == 10
    || a1 == 11
    || a1 == 12
    || a1 == 33
    || a1 == 34
    || a1 == 39
    || a1 == 40
    || a1 == 41
    || a1 == 42
    || a1 == 43
    || a1 == 52 )
    return pNPCStats->pProfessions[a1 - 1].pActionText;
  else
    return pNPCTopics[407].pTopic;
}

//----- (004BB756) --------------------------------------------------------
int UseNPCSkill(NPCProf profession)
{
  switch (profession)
  {
    case Healer:
    {
      for (int i = 0; i < 4; ++i)
        pParty->pPlayers[i].sHealth = pParty->pPlayers[i].GetMaxHealth();
    }
    break;

    case ExpertHealer:
    {
      for (int i = 0; i < 4; ++i)
      {
        __debugbreak();
        pParty->pPlayers[i].sHealth = pParty->pPlayers[i].GetMaxHealth();

        for (int j = 0; j < 14; ++j)
          pParty->pPlayers[i].pConditions[j] = 0;
        pParty->pPlayers[i].pConditions[Condition_Good] = 0;
      }
    }
    break;

    case MasterHealer:
    {
      for (int i = 0; i < 4; ++i)
      {
        __debugbreak();	//Ritor1:needed cleaned(���������� ���������)
        Player* player = &pParty->pPlayers[i];
        pParty->pPlayers[i].sHealth = pParty->pPlayers[i].GetMaxHealth();

        int v5 = LODWORD(player->pConditions[19]);//*((int *)v4 - 32);
        int v6 = HIDWORD(player->pConditions[19]);//*((int *)v4 - 31);
        memset(&pParty->pPlayers[i].pConditions, 0, sizeof(pParty->pPlayers[i].pConditions));

        *(int *)&player->pActiveSkills[PLAYER_SKILL_SHIELD] = v5;
        *(int *)&player->pActiveSkills[PLAYER_SKILL_CHAIN] = v6;
      }
    }
    break;

    case Cook://�����
    {
      if (pParty->uNumFoodRations >= 13)
        return 1;

      Party::GiveFood(1);
    }
    break;

    case Chef:
    {
      if (pParty->uNumFoodRations >= 13)
        return 1;

      if (pParty->uNumFoodRations == 13)
        Party::GiveFood(1);
      else
        Party::GiveFood(2);
    }
    break;

    case WindMaster:
    {
      if (uCurrentlyLoadedLevelType == LEVEL_Indoor)
      {
        ShowStatusBarString(pGlobalTXT_LocalizationStrings[494], 2);//������ ��������� ������ ����� � ���������!
        pAudioPlayer->PlaySound(SOUND_203, 0, 0, -1, 0, 0, 0, 0);
      }
      else
      {
        int v19 = pOtherOverlayList->_4418B1(10008, 203, 0, 65536);
        pParty->pPartyBuffs[PARTY_BUFF_FLY].Apply(pParty->uTimePlayed + 60 * (256 * 2), 3, 1, v19, 0);
        pParty->pPartyBuffs[PARTY_BUFF_FLY].uFlags |= 1;
        pAudioPlayer->PlaySound(SOUND_11090, 0, 0, -1, 0, 0, 0, 0);
      }
    }
    break;

    case WaterMaster:
    {
      int v20 = pOtherOverlayList->_4418B1(10005, 201, 0, 65536);
      pParty->pPartyBuffs[PARTY_BUFF_WATER_WALK].Apply(pParty->uTimePlayed + 60 * (256 * (2 + 1)), 3, 0, v20, 0);
      pParty->pPartyBuffs[PARTY_BUFF_WATER_WALK].uFlags |= 1;
      pAudioPlayer->PlaySound(SOUND_12040, 0, 0, -1, 0, 0, 0, 0);
    }
    break;

    case GateMaster:
    {
      pMessageQueue_50CBD0->AddGUIMessage(UIMSG_Escape, 0, 0);
      dword_50C9DC = 195;
      ptr_50C9E0 = GetNPCData(sDialogue_SpeakingActorNPC_ID);
    }
    break;

    case Acolyte:      _42777D_CastSpell_UseWand_ShootArrow(46, 0, 133, 0, 0); break;
    case Piper:        _42777D_CastSpell_UseWand_ShootArrow(51, 0, 133, 0, 0); break;
    case FallenWizard: _42777D_CastSpell_UseWand_ShootArrow(86, 0, 133, 0, 0); break;
      
    case Teacher:
    case Instructor:
    case Armsmaster:
    case Weaponsmaster:
    case Apprentice:
    case Mystic:
    case Spellmaster:
    case Trader:
    case Merchant:
    case Scout:
    case Herbalist:
    case Apothecary:
    case Tinker:
    case Locksmith:
    case Fool:
    case ChimneySweep:
    case Porter:
    case QuarterMaster:
    case Factor:
    case Banker:
    case Horseman:
    case Bard:
    case Enchanter:
    case Cartographer:
    case Explorer:
    case Pirate:
    case Squire:
    case Psychic:
    case Gypsy:
    case Diplomat:
    case Duper:
    case Burglar:
    case Acolyte2:
    case Initiate:
    case Prelate:
    case Monk:
    case Sage:
    case Hunter:
      break;

    default:
      assert(false && "Invalid enum value");
  }
  return 0;
}