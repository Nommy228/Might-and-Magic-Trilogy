#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <string.h>

#include "LOD.h"
#include "texts.h"
#include "StorylineTextTable.h"

char *pHistoryTXT_Raw;
struct StorylineText *pStorylineText;  

//----- (00453E6D) --------------------------------------------------------
void StorylineText::Initialize()
	{

	int i;
	char* test_string;
	unsigned char c;
	bool break_loop;
	unsigned int temp_str_len;
	char* tmp_pos;
	int decode_step;

	if ( pHistoryTXT_Raw )
		free(pHistoryTXT_Raw);
	pHistoryTXT_Raw = NULL;
	pHistoryTXT_Raw = (char *)pEvents_LOD->LoadRaw("history.txt", 0);
	strtok(pHistoryTXT_Raw, "\r");

	StoreLine[0].pText=nullptr;
	StoreLine[0].pPageTitle=nullptr;
	StoreLine[0].uTime=0;
	StoreLine[0].f_9=0;
	StoreLine[0].f_A=0;
	StoreLine[0].f_B=0;

	for (i=0;i<29;++i)
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
					StoreLine[i+1].pText=RemoveQuotes(test_string);
					break;
				case 2:
					StoreLine[i+1].uTime=atoi(test_string);  //strange but in text here string not digit
					break;
				case 3:
					StoreLine[i+1].pPageTitle=RemoveQuotes(test_string);
					break;
					}
				}
			else
				{ 
				break_loop = true;
				}
			++decode_step;
			test_string=tmp_pos+1;
			} while ((decode_step<4)&&!break_loop);
		}
	}