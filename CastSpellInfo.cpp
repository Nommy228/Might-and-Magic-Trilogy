#include "CastSpellInfo.h"
#include "Actor.h"
#include "Party.h"
#include "mm7_data.h"
#include "MM7.h"
#include "BSPModel.h"
#include "Mouse.h"
#include "stru6.h"
#include "Game.h"
#include "GUIWindow.h"
#include "GUIFont.h"
#include "AudioPlayer.h"
#include "Outdoor.h"
#include "Overlays.h"
#include "Events.h"
#include "Viewport.h"
#include "Math.h"
#include "SpriteObject.h"
#include "ObjectList.h"
#include "stru123.h"
#include "Time.h"
#include "IconFrameTable.h"
#include "Awards.h"
#include "TurnEngine.h"
#include "texts.h"
#include "LOD.h"
#include "Level/Decoration.h"

const size_t CastSpellInfoCount = 10;
std::array<CastSpellInfo, CastSpellInfoCount> pCastSpellInfo;

//----- (00427E01) --------------------------------------------------------
void CastSpellInfoHelpers::_427E01_cast_spell()
{
  int v2; // edi@1
  CastSpellInfo *pCastSpell; // ebx@2
  signed int v6; // eax@14
  unsigned __int16 v11; // cx@45
  unsigned __int8 v14; // zf@53
  signed int i; // esi@76
  __int16 v23; // ax@88
  int v51; // eax@146
  __int16 v52; // ax@153
  signed __int64 v54; // qax@164
  Actor *v55; // edi@164
  signed __int64 v58; // qax@177
  Actor *pActor; // edi@177
  unsigned __int16 v60; // ax@184
  int v61; // ecx@184
  ItemGen *v69; // esi@211
  double v90; // st7@245
  Player *v92; // eax@255
  int v105; // edi@271
  __int16 v108; // ST1C_2@274
  __int16 v109; // ST18_2@274
  stru6 *v110; // eax@274
  int v111; // eax@274
  Actor *v112; // esi@278
  int v113; // eax@278
  int v114; // ebx@278
  unsigned int v115; // ecx@278
  int v116; // edx@279
  int v117; // edx@281
  int v118; // edx@283
  signed int v122; // eax@286
  int v127; // eax@296
  int v153; // ecx@322
  int v154; // eax@322
  int v155; // eax@323
  int v156; // eax@323
  int v162; // edi@340
  signed int v164; // eax@340
  signed int v165; // edi@340
  signed int v168; // edi@343
  int v169; // eax@344
  signed int v174; // edi@355
  int v188; // esi@369
  int v189; // edi@369
  signed int v191; // edi@379
  signed int v205; // edi@405
  int v206; // eax@407
  __int16 v207; // cx@407
  int v208; // eax@411
  signed int v209; // ecx@412
  int v210; // edx@412
  signed int v211; // ecx@412
  int v212; // eax@413
  int v213; // eax@413
  int v214; // eax@413
  int v215; // eax@415
  double v216; // st7@415
  double v217; // st6@415
  signed __int64 v218; // qtt@423
  char v223; // al@438

  int v227; // esi@453
  unsigned int v228; // edi@454
  int v229; // edi@466

  ItemGen *v240; // ecx@472
  double v241; // st7@478
  ItemGen *v245; // edi@492
  int v254; // eax@513
  int v255; // esi@513
  int v256; // ecx@513
  int v257; // edx@514
  int v258; // ecx@514
  char v259; // al@516
  int v260; // eax@518
  int v261; // esi@519
  int v262; // edx@521
  int *v263; // ecx@521
  int v264; // esi@521
  int v265; // edx@521
  int *ii; // eax@522
  int v267; // eax@524
  int v268; // eax@524
  char v271; // al@531
  int v277; // edx@548
  int v278; // ecx@548
  char v279; // al@550
  int v280; // eax@552
  int *v281; // esi@553
  int v282; // edx@555
  int *v283; // ecx@555
  int v284; // esi@555
  int v285; // edx@555
  int *l; // eax@556
  ItemGen *v294; // esi@575
  int v295; // edx@575
  int kk; // edi@575
  char v313; // al@606pGame->GetStru6()
  const char *v317; // ecx@617
  Player *v318; // ecx@619
  unsigned int v319; // edi@627
  int v323; // edi@635
  char *v324; // eax@635
  Player *v325; // ecx@640
  int v328; // ecx@651
  int v329; // ecx@652
  int v330; // edi@654
  signed int v342; // edi@668
  signed int v343; // edi@670
  unsigned __int64 v344; // ST08_8@670
  Player *v351; // edi@680
  Player *v357; // edi@694
  Actor *v369; // edi@705
  int v373; // eax@715
  int v374; // eax@717
  int v376; // eax@717
  Player *v377; // ecx@719
  int v381; // edi@727
  int v382; // ecx@727
  Player *v383; // eax@728
  int v384; // eax@733
  int v388; // edi@740
  int v396; // eax@752
  int v397; // eax@757
  int v398; // eax@757
  int v399; // eax@757
  Actor *v417; // eax@787
  int v418; // ecx@789
  __int16 v419; // ax@791
  signed int v420; // eax@793
  ItemGen *v421; // edx@793
  const char *v422; // eax@801
  signed int v426; // eax@815
  Actor *v433; // edi@832
  int v435; // ecx@837
  int v440; // eax@843
  int v441; // eax@847
  signed int v445; // edi@857
  //int v446; // ecx@862
  //LevelDecoration *v447; // edi@864
  __int16 v448; // ax@864
  char *v449; // esi@870
  int v450; // eax@870
  signed int v460; // eax@895
  Actor *v461; // eax@897
  unsigned __int16 v462; // cx@897
  signed int v463; // edx@897
  int v470; // edi@913
  int v471; // eax@917
  int v472; // eax@917
  int v498; // edi@931
  int v499; // eax@935
  int v500; // eax@935
  Player *v501; // edi@939
  int v505; // eax@943
  int v507; // edi@944
  signed int v509; // eax@944
  //signed int v510; // edi@944
  Actor *v518; // edx@957
  __int16 v519; // cx@958
  int v531; // eax@982
  int v533; // edi@983
  signed int v535; // eax@983
  signed int v536; // edi@983
  stru6 *v537; // eax@984
  double v549; // st7@991
  unsigned __int16 v550; // di@991
  Player *v553; // edi@1001
  unsigned __int16 v562; // di@1005
  signed int v563; // eax@1010
  unsigned int v564; // ecx@1011
  signed int v565; // eax@1012
  Player **v566; // ecx@1012
  int v567; // eax@1012
  Player *v571; // eax@1013
  char *v572; // ecx@1013
  signed int v576; // eax@1025
  Player *v577; // eax@1026
  int v578; // eax@1028
  __int16 v579; // ax@1029
  int v581; // edi@1031
  char *v585; // esi@1034
  signed int v587; // eax@1035
  char v591; // al@1048
  ItemGen *v592; // esi@1052
  int v596; // esi@1066
  unsigned int v597; // edi@1067
  int v600; // edi@1086
  int v601; // edx@1086
  int v602; // eax@1086
  int v603; // ecx@1086
  NPCData *pNPCData; // eax@1089
  int v606; // edx@1091
  AwardType *v607; // ecx@1100
  __int16 v608; // ax@1102
  signed int v609; // eax@1104
  int v610; // edi@1106
  unsigned int v611; // eax@1106
  Player *v612; // edi@1106
  DDM_DLV_Header *v613; // eax@1108
  int v615; // edi@1119
  Player *v619; // edi@1123
  signed __int64 v623; // qax@1127
  int v625; // edi@1129
  signed int v627; // eax@1129
  signed int v628; // edi@1129
  int v629; // ecx@1130
  Player *v630; // eax@1131
  int v631; // eax@1137
  int v635; // edi@1142
  int v642; // edi@1156
  int v643; // eax@1156
  int v644; // eax@1156
  signed int v645; // eax@1158
  unsigned int v656; // [sp-4h] [bp-E88h]@639
  int v657; // [sp-4h] [bp-E88h]@807
  int v659; // [sp+0h] [bp-E84h]@123
  int v660; // [sp+0h] [bp-E84h]@146
  Actor *v661; // [sp+0h] [bp-E84h]@164
  unsigned __int64 v663; // [sp+0h] [bp-E84h]@639
  const char *v664; // [sp+0h] [bp-E84h]@802
  int v665; // [sp+0h] [bp-E84h]@807
  int v666; // [sp+4h] [bp-E80h]@12
  PLAYER_SKILL_TYPE v667; // [sp+4h] [bp-E80h]@25
  int v671; // [sp+4h] [bp-E80h]@146
  int v675; // [sp+4h] [bp-E80h]@800
  int v676; // [sp+4h] [bp-E80h]@807
  //int v677; // [sp+4h] [bp-E80h]@861
  int v679[800]; // [sp+14h] [bp-E70h]@515
  AIDirection a3; // [sp+C94h] [bp-1F0h]@21
  int v681[4]; // [sp+CB0h] [bp-1D4h]@1137
  int v682[4]; // [sp+CC0h] [bp-1C4h]@731
  ItemGen v683; // [sp+CD0h] [bp-1B4h]@791
  int v684; // [sp+D04h] [bp-180h]@416
  unsigned __int64 v685; // [sp+D08h] [bp-17Ch]@416
  unsigned __int64 v687; // [sp+D24h] [bp-160h]@327
  Vec3_int_ v688; // [sp+D2Ch] [bp-158h]@943
  Vec3_int_ v691; // [sp+D38h] [bp-14Ch]@137
  Vec3_int_ v694; // [sp+D44h] [bp-140h]@982
  Vec3_int_ v697; // [sp+D50h] [bp-134h]@129
  Vec3_int_ v700; // [sp+D5Ch] [bp-128h]@339
  Vec3_int_ v701; // [sp+D68h] [bp-11Ch]@286
  Vec3_int_ v704; // [sp+D74h] [bp-110h]@132
  Vec3_int_ v707; // [sp+D80h] [bp-104h]@1127
  int v710; // [sp+D8Ch] [bp-F8h]@1156
  __int64 v712; // [sp+D94h] [bp-F0h]@991
  int v713; // [sp+D9Ch] [bp-E8h]@324
  int n; // [sp+DA0h] [bp-E4h]@1
  AIDirection v715; // [sp+DA4h] [bp-E0h]@21
  int v716; // [sp+DC0h] [bp-C4h]@272
  __int64 v717; // [sp+DC4h] [bp-C0h]@271
  float v718; // [sp+DCCh] [bp-B8h]@176
  signed int sRecoveryTime; // [sp+DD0h] [bp-B4h]@53
  char *y; // [sp+DD4h] [bp-B0h]@325
  int v721; // [sp+DD8h] [bp-ACh]@163
  int v723; // [sp+E4Ch] [bp-38h]@1
  ItemGen *_this; // [sp+E50h] [bp-34h]@23
  float v725; // [sp+E54h] [bp-30h]@23
  Player *v726; // [sp+E58h] [bp-2Ch]@131
  float v727; // [sp+E5Ch] [bp-28h]@1
  unsigned int uRequiredMana; // [sp+E60h] [bp-24h]@53
  Player *pPlayer; // [sp+E64h] [bp-20h]@8
  int v730; // [sp+E68h] [bp-1Ch]@53
  Player *v730b;
  ItemGen *v730c;
  int v731; // [sp+E6Ch] [bp-18h]@48
  signed int v732; // [sp+E70h] [bp-14h]@325
  unsigned __int64 v733; // [sp+E74h] [bp-10h]@1
  signed int a2; // [sp+E7Ch] [bp-8h]@14
  int amount; // [sp+E80h] [bp-4h]@1

  SpriteObject pSpellSprite; // [sp+DDCh] [bp-A8h]@1

  v2 = 0;
  amount = 0;
  LODWORD(v733) = 0;
  v723 = 0;
  v727 = 0.0;
  for(n = 0; n < CastSpellInfoCount; ++n)
  {
    pCastSpell = &pCastSpellInfo[n];
    HIDWORD(v733) = (int)pCastSpell;
    if ( pCastSpell->uSpellID == 0 )
      continue;

    if (pParty->Invisible())
      pParty->pPartyBuffs[PARTY_BUFF_INVISIBILITY].Reset();

    if ( pCastSpell->uFlags & 0x3CA )
    {
      if ( !pParty->pPlayers[pCastSpell->uPlayerID].CanAct() )
        _427D48();
      continue;
    }
    pPlayer = &pParty->pPlayers[pCastSpell->uPlayerID];

    a2 = pCastSpell->spell_target_pid;
    if (!pCastSpell->spell_target_pid)
    {
      if (pCastSpell->uSpellID == SPELL_LIGHT_DESTROY_UNDEAD ||
          pCastSpell->uSpellID == SPELL_SPIRIT_TURN_UNDEAD ||
          pCastSpell->uSpellID == SPELL_DARK_CONTROL_UNDEAD )
        v666 = 1;
      else
        v666 = 0;

      a2 = stru_50C198.FindClosestActor(5120, 1, v666);
      v6 = pMouse->uPointingObjectID;
      if ( pMouse->uPointingObjectID && PID_TYPE(v6) == OBJECT_Actor && pActors[PID_ID(v6)].CanAct() )
        a2 = pMouse->uPointingObjectID;
    }


    pSpellSprite.uType = stru_4E3ACC[pCastSpell->uSpellID].uType;
    if (pSpellSprite.uType)
    {
      if (PID_TYPE(a2) == OBJECT_Actor)
      {
        Actor::GetDirectionInfo(PID(OBJECT_Player, pCastSpell->uPlayerID + 1), a2, &v715, 0);
        v2 = v723;
      }
      else
      {
        v715.uYawAngle = pParty->sRotationY;
        v715.uPitchAngle = pParty->sRotationX;
      }
    }

    LODWORD(v725) = 0;
    _this = 0;
    if (pCastSpell->forced_spell_skill_level)
    {
      v11 = pCastSpell->forced_spell_skill_level;
      v723 = v11 & 0x3F; // 6 bytes
      v2 = v723;
    }
    else
    {
      //v667 = PLAYER_SKILL_STAFF;
      if (pCastSpell->uSpellID < SPELL_AIR_WIZARD_EYE)
        v667 = PLAYER_SKILL_FIRE;
      else if (pCastSpell->uSpellID < SPELL_WATER_AWAKEN)
        v667 = PLAYER_SKILL_AIR;
      else if (pCastSpell->uSpellID < SPELL_EARTH_STUN)
        v667 = PLAYER_SKILL_WATER;
      else if (pCastSpell->uSpellID < SPELL_SPIRIT_DETECT_LIFE)
        v667 = PLAYER_SKILL_EARTH;
      else if (pCastSpell->uSpellID < SPELL_MIND_REMOVE_FEAR)
        v667 = PLAYER_SKILL_SPIRIT;
      else if (pCastSpell->uSpellID < SPELL_BODY_CURE_WEAKNESS)
        v667 = PLAYER_SKILL_MIND;
      else if (pCastSpell->uSpellID < SPELL_LIGHT_LIGHT_BOLT)
        v667 = PLAYER_SKILL_BODY;
      else if (pCastSpell->uSpellID < SPELL_DARK_REANIMATE)
        v667 = PLAYER_SKILL_LIGHT;
      else if (pCastSpell->uSpellID < SPELL_BOW_ARROW)
        v667 = PLAYER_SKILL_DARK;
      else if (pCastSpell->uSpellID == SPELL_BOW_ARROW)
        v667 = PLAYER_SKILL_BOW;
      else assert(false && "Unknown spell");

      LODWORD(v725) = v667;
      v723 = pPlayer->GetActualSkillLevel(v667) & 0x3F;
      v2 = v723;
      v11 = pPlayer->pActiveSkills[LODWORD(v725)];
    }

	v731 = SkillToMastery(v11);

    if (pCastSpell->forced_spell_skill_level)
      uRequiredMana = 0;
    else 
      uRequiredMana = pSpellDatas[pCastSpell->uSpellID - 1].mana_per_skill[v731 - 1];
    sRecoveryTime = pSpellDatas[pCastSpell->uSpellID - 1].recovery_per_skill[v731 - 1];

    if (LODWORD(v725) == PLAYER_SKILL_DARK && pParty->uCurrentHour == 0 && pParty->uCurrentMinute == 0 ||
        LODWORD(v725) == PLAYER_SKILL_LIGHT && pParty->uCurrentHour == 12 && pParty->uCurrentMinute == 0)
      uRequiredMana = 0;

    if (pCastSpell->uSpellID < SPELL_BOW_ARROW && pPlayer->sMana < uRequiredMana)
    {
      ShowStatusBarString(pGlobalTXT_LocalizationStrings[586], 2u); // "Not enough spell points"
      pCastSpell->uSpellID = 0;
      continue;
    }
    v730 = pCastSpell->uSpellID;
    if (pPlayer->IsCursed() && pCastSpell->uSpellID < SPELL_BOW_ARROW && rand() % 100 < 50)
    {
      if (!pParty->bTurnBasedModeOn)
      {
        //v646 = pPlayer;
        pPlayer->SetRecoveryTime((signed __int64)(flt_6BE3A4_debug_recmod1 * 213.3333333333333));
        //v647 = n;
      }
      else
      {
        //v647 = n;
        //v646 = pPlayer;
        //v648 = sRecoveryTime;
        //v649 = pPlayer;
        pParty->pTurnBasedPlayerRecoveryTimes[pCastSpellInfo[n].uPlayerID] = 100;
        pPlayer->SetRecoveryTime(sRecoveryTime);
        pTurnEngine->ApplyPlayerAction();
      }
      ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2u); // "Spell failed"
      pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
      pCastSpellInfo[n].uSpellID = 0;
      pPlayer->sMana -= uRequiredMana;
      return;
    }

    switch ( pCastSpell->uSpellID )
    {
		case SPELL_101:
			assert(false && "Unknown spell effect #101 (prolly flaming bow arrow");
		case SPELL_BOW_ARROW:
		{
			amount = 1;
			if ( SkillToMastery(pPlayer->pActiveSkills[PLAYER_SKILL_BOW]) >= 3 )
				amount = 2;
			sRecoveryTime = pPlayer->GetAttackRecoveryTime(true);
			pSpellSprite.stru_24.Reset();
			pSpellSprite.spell_level = v2;
			pSpellSprite.spell_id = pCastSpell->uSpellID;
			pSpellSprite.spell_skill = v731;
			pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
			if ( pPlayer->WearsItem(ITEM_ARTEFACT_ULLYSES, EQUIP_BOW) )
				pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(0xBD6u);
			pSpellSprite.vPosition.x = pParty->vPosition.x;
			pSpellSprite.vPosition.y = pParty->vPosition.y;
			pSpellSprite.uAttributes = 0;
			pSpellSprite.uSpriteFrameID = 0;
			pSpellSprite.vPosition.z = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3;
			pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
			pSpellSprite.spell_target_pid = a2;
			pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
			pSpellSprite.uFacing = LOWORD(v715.uYawAngle);
			pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
			pPlayer = &pParty->pPlayers[pCastSpell->uPlayerID];
			memcpy(&pSpellSprite.stru_24, &pPlayer->pInventoryItemList[pPlayer->pEquipment.uBow-1], sizeof(pSpellSprite.stru_24));
			pSpellSprite.uAttributes = 256;
			if ( pParty->bTurnBasedModeOn == 1 )
				pSpellSprite.uAttributes = 260;
			for ( i = 0; i < amount; ++i )
			{
				if ( i )
					pSpellSprite.vPosition.z += 32;
				pSpellSprite.uSectorID = pIndoor->GetSector(pSpellSprite.vPosition.x, pSpellSprite.vPosition.y, pSpellSprite.vPosition.z);
				if ( pSpellSprite.Create(v715.uYawAngle, v715.uPitchAngle, pObjectList->pObjects[(signed __int16)pSpellSprite.uObjectDescID].uSpeed,
					pCastSpell->uPlayerID + 1) != -1 && pParty->bTurnBasedModeOn == 1 )
				++pTurnEngine->pending_actions;
			}
			break;
		}
		case SPELL_LASER_PROJECTILE:
		{
			sRecoveryTime = pPlayer->GetAttackRecoveryTime(0);
			pSpellSprite.stru_24.Reset();
			pSpellSprite.spell_id = pCastSpell->uSpellID;
			pSpellSprite.spell_level = v723;
			pSpellSprite.spell_skill = v731;
			pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
			pSpellSprite.vPosition.x = pParty->vPosition.x;
			pSpellSprite.vPosition.y = pParty->vPosition.y;
			pSpellSprite.uAttributes = 0;
			pSpellSprite.uSpriteFrameID = 0;
			pSpellSprite.vPosition.z = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 2;
			pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
			pSpellSprite.spell_target_pid = a2;
			pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
			pSpellSprite.uFacing = LOWORD(v715.uYawAngle);
			pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
			pPlayer = &pParty->pPlayers[pCastSpell->uPlayerID];
			memcpy(&pSpellSprite.stru_24, &pPlayer->pInventoryItemList[pPlayer->pEquipment.uMainHand-1],sizeof(pSpellSprite.stru_24));
			//	&pParty->pPlayers[pCastSpell->uPlayerID].spellbook.pDarkSpellbook.bIsSpellAvailable[36
			//		* pParty->pPlayers[pCastSpell->uPlayerID].pEquipment.uMainHand + 5], );
			v23 = pIndoor->GetSector(pParty->vPosition.x, pParty->vPosition.y, pSpellSprite.vPosition.z);
			HIBYTE(pSpellSprite.uAttributes) |= 1u;
			pSpellSprite.uSectorID = v23;
			if ( pParty->bTurnBasedModeOn == 1 )
				LOBYTE(pSpellSprite.uAttributes) |= 4u;
			if ( pSpellSprite.Create(v715.uYawAngle, v715.uPitchAngle, pObjectList->pObjects[(signed __int16)pSpellSprite.uObjectDescID].uSpeed,
					pCastSpell->uPlayerID + 1) != -1 && pParty->bTurnBasedModeOn == 1 )
				++pTurnEngine->pending_actions;
			break;
		}
		case SPELL_FIRE_TORCH_LIGHT:
		{
			switch (v731)
			{
				case 1: amount = 2; break;
				case 2: amount = 3; break;
				case 3:
				case 4: amount = 4; break;
				default:
				assert(false);
			}
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			pParty->pPartyBuffs[PARTY_BUFF_TORCHLIGHT].Apply(pParty->uTimePlayed + (signed __int64)((double)(signed int)3600 * v2 * 4.2666669), v731, amount, 0, 0);
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_FIRE_FIRE_SPIKE:
		{
			switch (v731)
			{
				case 1: amount = 3; break;
				case 2: amount = 5; break;
				case 3: amount = 7; break;
				case 4: amount = 9; break;
				default:
				assert(false);
			}

			//v31 = v3->uPlayerID;
			//v32 = 8 * v31;
			//LOBYTE(v32) = v32 | OBJECT_Player;

			//if ( (signed int)uNumSpriteObjects > 0 )
			int _v733 = 0;
			for (uint i = 0; i < uNumSpriteObjects; ++i)
			{
				SpriteObject* object = &pSpriteObjects[i];
				if (object->uType && object->spell_id == SPELL_FIRE_FIRE_SPIKE && object->spell_caster_pid == PID(OBJECT_Player, pCastSpell->uPlayerID))
				++_v733;
				/*v33 = (char *)&pSpriteObjects[0].field_48;
				v730 = uNumSpriteObjects;
				do
				{
				if ( *((short *)v33 - 36) && *(int *)v33 == 7 && *((int *)v33 + 4) == v32 )
					++HIDWORD(v733);
				v33 += 112;
				--v730;
				}
				while ( v730 );*/
			}
			if (_v733 > amount)
			{
				ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2); // Spell failed
				pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
				pCastSpell->uSpellID = 0;
				continue;
			}
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			pSpellSprite.stru_24.Reset();
			pSpellSprite.spell_id = pCastSpell->uSpellID;
			pSpellSprite.spell_level = v2;
			pSpellSprite.spell_skill = v731;
			pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
			pSpellSprite.vPosition.y = pParty->vPosition.y;
			pSpellSprite.vPosition.x = pParty->vPosition.x;
			pSpellSprite.uAttributes = 0;
			pSpellSprite.vPosition.z = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3;
			pSpellSprite.uSectorID = pIndoor->GetSector(pParty->vPosition.x, pParty->vPosition.y, pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3);
			pSpellSprite.uSpriteFrameID = 0;
			pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
			pSpellSprite.spell_target_pid = a2;
			pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
			pSpellSprite.uFacing = LOWORD(v715.uYawAngle);
			pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
			if ( pParty->bTurnBasedModeOn == 1 )
				LOBYTE(pSpellSprite.uAttributes) |= 4u;
			v659 = pObjectList->pObjects[(signed __int16)pSpellSprite.uObjectDescID].uSpeed;
			if ( pSpellSprite.Create(pParty->sRotationY, pParty->sRotationX + 10, v659, pCastSpell->uPlayerID + 1) != -1 && pParty->bTurnBasedModeOn == 1 )
				++pTurnEngine->pending_actions;
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_AIR_IMPLOSION:
		{
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			if (!a2)
			{
				ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2); // Spell failed
				pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
				pCastSpell->uSpellID = 0;
				continue;
			}
			if (PID_TYPE(a2) != OBJECT_Actor)
			{
				LODWORD(v727) = 1;
				break;
			}
			v697.x = 0;
			v697.y = 0;
			v697.z = 0;
			pSpellSprite.stru_24.Reset();
			pSpellSprite.spell_id = pCastSpell->uSpellID;
			pSpellSprite.spell_level = v2;
			pSpellSprite.spell_skill = v731;
			pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
			pSpellSprite.uAttributes = 0;
			pSpellSprite.uSectorID = 0;
			pSpellSprite.uSpriteFrameID = 0;
			pSpellSprite.field_60_distance_related_prolly_lod = 0;
			pSpellSprite.uFacing = 0;
			pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
			pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
			pSpellSprite.vPosition.x = pActors[PID_ID(a2)].vPosition.x;
			pSpellSprite.vPosition.z = pActors[PID_ID(a2)].vPosition.z;
			pSpellSprite.vPosition.y = pActors[PID_ID(a2)].vPosition.y;
			pSpellSprite.spell_target_pid = PID(OBJECT_Actor, PID_ID(a2));
			int obj_id = pSpellSprite.Create(0, 0, 0, 0);
			DamageMonsterFromParty(PID(OBJECT_Item, obj_id), PID_ID(a2), &v697);
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_EARTH_MASS_DISTORTION:
		{
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			if ( !stru_50C198.GetMagicalResistance(&pActors[PID_ID(a2)], 3u) )
			{
				LODWORD(v727) = 1;
				break;
			}
			pActors[PID_ID(a2)].pActorBuffs[ACTOR_BUFF_MASS_DISTORTION].Apply(pMiscTimer->uTotalGameTimeElapsed + 128, 0, 0, 0, 0);
			v704.x = 0;
			v704.y = 0;
			v704.z = 0;
			pSpellSprite.stru_24.Reset();
			pSpellSprite.spell_id = pCastSpell->uSpellID;
			pSpellSprite.spell_level = v2;
			pSpellSprite.spell_skill = v731;
			pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
			pSpellSprite.uAttributes = 0;
			pSpellSprite.uSectorID = 0;
			pSpellSprite.uSpriteFrameID = 0;
			pSpellSprite.field_60_distance_related_prolly_lod = 0;
			pSpellSprite.uFacing = 0;
			pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
			pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
			pSpellSprite.vPosition.x = pActors[PID_ID(a2)].vPosition.x;
			pSpellSprite.vPosition.y = pActors[PID_ID(a2)].vPosition.y;
			pSpellSprite.vPosition.z = pActors[PID_ID(a2)].vPosition.z;
			pSpellSprite.spell_target_pid = PID(OBJECT_Actor, PID_ID(a2));
			int obj_id = pSpellSprite.Create(0, 0, 0, 0);
			DamageMonsterFromParty(PID(OBJECT_Item, obj_id), PID_ID(a2), &v704);
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_LIGHT_DESTROY_UNDEAD:
		{
			if ( !pPlayer->CanCastSpell(uRequiredMana) || !a2 || PID_TYPE(a2) != OBJECT_Actor)
				break;
			//v730 = a2 >> 3;
			//HIDWORD(v733) = (int)&pActors[PID_ID(a2)];
			v691.x = 0;
			v691.y = 0;
			v691.z = 0;
			pSpellSprite.stru_24.Reset();
			pSpellSprite.spell_id = pCastSpell->uSpellID;
			pSpellSprite.spell_level = v2;
			pSpellSprite.spell_skill = v731;
			pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
			pSpellSprite.vPosition.x = pActors[PID_ID(a2)].vPosition.x;
			pSpellSprite.vPosition.y = pActors[PID_ID(a2)].vPosition.y;
			pSpellSprite.vPosition.z = pActors[PID_ID(a2)].vPosition.z;
			pSpellSprite.uAttributes = 0;
			pSpellSprite.uSectorID = pIndoor->GetSector(pSpellSprite.vPosition.x, pSpellSprite.vPosition.y, pSpellSprite.vPosition.z);
			pSpellSprite.uSpriteFrameID = 0;
			pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
			pSpellSprite.spell_target_pid = a2;
			pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
			pSpellSprite.uFacing = LOWORD(v715.uYawAngle);
			pSpellSprite.uAttributes |= 0x80u;
			pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
			int obj_id = pSpellSprite.Create(0, 0, 0, 0);
			if ( !MonsterStats::BelongsToSupertype(pActor->pMonsterInfo.uID, MONSTER_SUPERTYPE_UNDEAD) )
			{
				ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2); // Spell failed
				pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
				pCastSpell->uSpellID = 0;
				continue;
			}
			DamageMonsterFromParty(PID(OBJECT_Item, obj_id), PID_ID(a2), &v691);
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_FIRE_FIRE_BOLT:
		case SPELL_FIRE_FIREBALL:
		case SPELL_FIRE_INCINERATE:
		case SPELL_AIR_LIGHNING_BOLT:
		case SPELL_WATER_ICE_BOLT:
		case SPELL_WATER_ICE_BLAST:
		case SPELL_EARTH_STUN:
		case SPELL_EARTH_DEADLY_SWARM:
		case SPELL_MIND_MIND_BLAST:
		case SPELL_MIND_PSYCHIC_SHOCK:
		case SPELL_BODY_HARM:
		case SPELL_LIGHT_LIGHT_BOLT:
		case SPELL_DARK_DRAGON_BREATH:
		{
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			pSpellSprite.stru_24.Reset();
			pSpellSprite.spell_id = pCastSpell->uSpellID;
			pSpellSprite.spell_level = v2;
			pSpellSprite.spell_skill = v731;
			pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
			pSpellSprite.vPosition.y = pParty->vPosition.y;
			pSpellSprite.vPosition.x = pParty->vPosition.x;
			pSpellSprite.uAttributes = 0;
			pSpellSprite.vPosition.z = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3;
			if (uCurrentlyLoadedLevelType == LEVEL_Indoor)
				pSpellSprite.uSectorID = pIndoor->GetSector(pParty->vPosition.x, pParty->vPosition.y, pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3);
			else
				pSpellSprite.uSectorID = 0;
			pSpellSprite.uSpriteFrameID = 0;
			pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
			pSpellSprite.spell_target_pid = a2;
			pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
			pSpellSprite.uFacing = LOWORD(v715.uYawAngle);
			pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
			if ( pParty->bTurnBasedModeOn == 1 )
				LOBYTE(pSpellSprite.uAttributes) |= 4u;
			if ( pCastSpell->uSpellID == SPELL_AIR_LIGHNING_BOLT )
				LOBYTE(pSpellSprite.uAttributes) |= 0x40u;
			v659 = pObjectList->pObjects[(signed __int16)pSpellSprite.uObjectDescID].uSpeed;
			if ( pSpellSprite.Create(v715.uYawAngle, v715.uPitchAngle, v659, pCastSpell->uPlayerID + 1) != -1 && pParty->bTurnBasedModeOn == 1 )
				++pTurnEngine->pending_actions;
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_WATER_ACID_BURST:
		case SPELL_EARTH_BLADES:
		case SPELL_BODY_FLYING_FIST:
		case SPELL_DARK_TOXIC_CLOUD:
		{
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			pSpellSprite.stru_24.Reset();
			pSpellSprite.spell_id = pCastSpell->uSpellID;
			pSpellSprite.spell_level = v2;
			pSpellSprite.spell_skill = v731;
			pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
			v51 = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 2;
			pSpellSprite.vPosition.y = pParty->vPosition.y;
			v671 = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 2;
			v660 = pParty->vPosition.y;
			pSpellSprite.uAttributes = 0;
			pSpellSprite.vPosition.x = pParty->vPosition.x;
			pSpellSprite.vPosition.z = v51;
			v52 = pIndoor->GetSector(pParty->vPosition.x, v660, v671);
			pSpellSprite.uSpriteFrameID = 0;
			pSpellSprite.uSectorID = v52;
			pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
			pSpellSprite.spell_target_pid = a2;
			pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
			pSpellSprite.uFacing = LOWORD(v715.uYawAngle);
			pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
			if ( pParty->bTurnBasedModeOn == 1 )
				LOBYTE(pSpellSprite.uAttributes) |= 4u;
			v659 = pObjectList->pObjects[(signed __int16)pSpellSprite.uObjectDescID].uSpeed;
			if ( pSpellSprite.Create(v715.uYawAngle, v715.uPitchAngle, v659, pCastSpell->uPlayerID + 1) != -1 && pParty->bTurnBasedModeOn == 1 )
				++pTurnEngine->pending_actions;
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_LIGHT_SUNRAY:
		{
			if ( uCurrentlyLoadedLevelType == LEVEL_Indoor
				|| uCurrentlyLoadedLevelType == LEVEL_Outdoor && (pParty->uCurrentHour < 5 || pParty->uCurrentHour >= 21) )
			{
				ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2); // Spell failed
				pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
				pCastSpell->uSpellID = 0;
				continue;
			}
			if ( pPlayer->CanCastSpell(uRequiredMana) )
			{
				pSpellSprite.stru_24.Reset();
				pSpellSprite.spell_id = pCastSpell->uSpellID;
				pSpellSprite.spell_skill = v731;
				pSpellSprite.spell_level = v2;
				pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
				pSpellSprite.vPosition.y = pParty->vPosition.y;
				v51 = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3;
				v671 = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3;
				v660 = pParty->vPosition.y;
				pSpellSprite.uAttributes = 0;
				pSpellSprite.vPosition.x = pParty->vPosition.x;
				pSpellSprite.vPosition.z = v51;
				v52 = pIndoor->GetSector(pParty->vPosition.x, v660, v671);
				pSpellSprite.uSpriteFrameID = 0;
				pSpellSprite.uSectorID = v52;
				pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
				pSpellSprite.spell_target_pid = a2;
				pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
				pSpellSprite.uFacing = LOWORD(v715.uYawAngle);
				pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
				if ( pParty->bTurnBasedModeOn == 1 )
					LOBYTE(pSpellSprite.uAttributes) |= 4u;
				v659 = pObjectList->pObjects[(signed __int16)pSpellSprite.uObjectDescID].uSpeed;
				if ( pSpellSprite.Create(v715.uYawAngle, v715.uPitchAngle, v659, pCastSpell->uPlayerID + 1) != -1 && pParty->bTurnBasedModeOn == 1 )
					++pTurnEngine->pending_actions;
				LODWORD(v727) = 1;
			}
			break;
		}
		case SPELL_LIGHT_PARALYZE:
		{
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			if (PID_TYPE(a2) != OBJECT_Actor || (v730 = PID_ID(a2), v721 = (int)&pActors[PID_ID(a2)],
					!stru_50C198.GetMagicalResistance(&pActors[PID_ID(a2)], 9)) )
			{
				LODWORD(v727) = 1;
				break;
			}
			Actor::AI_Stand(PID_ID(a2), 4u, 0x80u, 0);
			v54 = (signed __int64)((double)(23040 * v2) * 0.033333335);
			v55 = &pActors[PID_ID(a2)];
			v55->pActorBuffs[ACTOR_BUFF_PARALYZED].Apply(pParty->uTimePlayed + (signed int)v54, v731, 0, 0, 0);
			BYTE2(v55->uAttributes) |= 8u;
			v55->vVelocity.x = 0;
			//v672 = 0;
			v55->vVelocity.y = 0;
			v661 = v55;
			pGame->GetStru6()->_4A7E89_sparkles_on_actor_after_it_casts_buff(v661, 0);
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_EARTH_SLOW:
		{
			switch (v731)
			{
				case 1: LODWORD(v733) = 180 * v2; amount = 2; break;
				case 2: LODWORD(v733) = 300 * v2; amount = 2; break;
				case 3: LODWORD(v733) = 300 * v2; amount = 4; break;
				case 4: LODWORD(v733) = 300 * v2; amount = 8; break;
				default:
				assert(false);
			}
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			if (PID_TYPE(a2) != OBJECT_Actor
				|| (v721 = 836 * PID_ID(a2),
					LODWORD(v718) = (int)&pActors[PID_ID(a2)],
					!stru_50C198.GetMagicalResistance(&pActors[PID_ID(a2)], 3u)) )
			{
				LODWORD(v727) = 1;
				break;
			}
			v58 = (signed __int64)((double)(23040 * v2) * 0.033333335);
			//v59 = v721;
			pActor = &pActors[PID_ID(a2)];
			//((SpellBuff *)((char *)&pActors[0].pActorBuffs[ACTOR_BUFF_SLOWED] + v721))->Apply(
			pActor->pActorBuffs[ACTOR_BUFF_SLOWED].Apply(pParty->uTimePlayed + (signed int)v58,	v731, amount, 0, 0);
			//*((char *)&pActors[0].uAttributes + v59 + 2) |= 8u;
			BYTE2(pActor->uAttributes) |= 8u;
			//v672 = 0;
			v661 = (Actor *)LODWORD(v718);
			pGame->GetStru6()->_4A7E89_sparkles_on_actor_after_it_casts_buff(v661, 0);
			LODWORD(v727) = 1;
			break;
      }
		case SPELL_MIND_CHARM:
		{
		if ( !pPlayer->CanCastSpell(uRequiredMana) )
			break;

		if ( !stru_50C198.GetMagicalResistance(&pActors[PID_ID(a2)], 7u) )
		{
			LODWORD(v727) = 1;
			break;
		}

		uint power = 300 * v2;
		if ( v731 == 2 )
		{
			power = 600 * v2;
		}
		else if ( v731 == 3 )
			power  = 29030400;

		//((SpellBuff *)((char *)&pActors[0].pActorBuffs[ACTOR_BUFF_BERSERK] + v730))->Reset();
		pActors[PID_ID(a2)].pActorBuffs[ACTOR_BUFF_BERSERK].Reset();
		//((SpellBuff *)((char *)&pActors[0].pActorBuffs[ACTOR_BUFF_ENSLAVED] + v730))->Reset();
		pActors[PID_ID(a2)].pActorBuffs[ACTOR_BUFF_ENSLAVED].Reset();
		//((SpellBuff *)((char *)&pActors[0].pActorBuffs[ACTOR_BUFF_CHARM] + v730))->Apply(
		pActors[PID_ID(a2)].pActorBuffs[ACTOR_BUFF_CHARM].Apply(pParty->uTimePlayed + (signed int)(signed __int64)((double)(signed int)(power << 7) * 0.033333335),
			v731, 0, 0, 0);
		pSpellSprite.stru_24.Reset();
		pSpellSprite.spell_id = pCastSpell->uSpellID;
		pSpellSprite.spell_level = v2;
		pSpellSprite.spell_skill = v731;
		v60 = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
		v61 = PID_ID(a2);
			v600 = pActors[v61].vPosition.y;
			v601 = pActors[v61].vPosition.x;
			pSpellSprite.uObjectDescID = v60;
			pSpellSprite.vPosition.x = v601;
			v602 = pActors[v61].uActorHeight;
			v603 = pActors[v61].vPosition.z;
			pSpellSprite.vPosition.y = v600;
			v676 = v603 + v602;
			v665 = v600;
			pSpellSprite.vPosition.z = v603 + v602;
			v657 = v601;
			pSpellSprite.uAttributes = 0;
			pSpellSprite.uSectorID = pIndoor->GetSector(v657, v665, v676);
			pSpellSprite.uSpriteFrameID = 0;
			pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
			pSpellSprite.spell_target_pid = a2;
			pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
			pSpellSprite.uFacing = LOWORD(v715.uYawAngle);
			LOBYTE(pSpellSprite.uAttributes) |= 0x80u;
			pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
			pSpellSprite.Create(0, 0, 0, pCastSpell->uPlayerID + 1);
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_DARK_SHRINKING_RAY:
		{
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			pSpellSprite.stru_24.Reset();
			pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
			pSpellSprite.vPosition.x = pParty->vPosition.x;
			pSpellSprite.vPosition.y = pParty->vPosition.y;
			pSpellSprite.uAttributes = 0;
			pSpellSprite.vPosition.z = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3;
			pSpellSprite.uSectorID = pIndoor->GetSector(
								pParty->vPosition.x,
								pParty->vPosition.y,
								pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3);
			pSpellSprite.uSpriteFrameID = 0;
			pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
			pSpellSprite.spell_target_pid = a2;
			pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
			pSpellSprite.uFacing = LOWORD(v715.uYawAngle);
			pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
			pSpellSprite.spell_skill = v731;
			pSpellSprite.spell_id = SPELL_FIRE_PROTECTION_FROM_FIRE;
			pSpellSprite.spell_level = 300 * v2;
				if ( pParty->bTurnBasedModeOn == 1 )
				LOBYTE(pSpellSprite.uAttributes) |= 4u;

				v659 = pObjectList->pObjects[(signed __int16)pSpellSprite.uObjectDescID].uSpeed;

				if ( pSpellSprite.Create(v715.uYawAngle, v715.uPitchAngle, v659, pCastSpell->uPlayerID + 1) != -1 && pParty->bTurnBasedModeOn == 1 )
				++pTurnEngine->pending_actions;
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_FIRE_FIRE_AURA:
		{
			switch (v731)
			{
				case 1: LODWORD(v733) = 3600 * v2; amount = 10; break;
				case 2: LODWORD(v733) = 3600 * v2; amount = 11; break;
				case 3: LODWORD(v733) = 3600 * v2; amount = 12; break;
				case 4: LODWORD(v733) = 0; amount = 12; break;
				default:
				assert(false);
			}
			
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			v730c = &pParty->pPlayers[pCastSpell->uPlayerID_2].pInventoryItemList[a2];
			ItemDesc* _itm = &pItemsTable->pItems[v730c->uItemID];
			v730c->UpdateTempBonus(pParty->uTimePlayed);
			if ( v730c->uItemID < 64 || v730c->uItemID > 65 
				&& !v730c->IsBroken()
				&& !v730c->uSpecEnchantmentType
				&& !v730c->uEnchantmentType
				&& ( _itm->uEquipType == 0 || _itm->uEquipType == 1 || _itm->uEquipType == 2)
				&& !pItemsTable->IsMaterialNonCommon(v730c) )
			{
				v69 = v730c;
				v14 = v731 == 4;
				v730c->uSpecEnchantmentType = amount;
				if ( !v14 )
				{
					v69->uExpireTime = pParty->uTimePlayed
										+ (signed int)(signed __int64)((double)(signed int)((int)v733 << 7)
																		* 0.033333335);
					v69->uAttributes |= 8u;
				}
				v69->uAttributes |= 0x10u;
				_50C9A8_item_enchantment_timer = 256;
				LODWORD(v727) = 1;
				break;
			}

			_50C9D0_AfterEnchClickEventId = 113;
			_50C9D4_AfterEnchClickEventSecondParam = 0;
			_50C9D8_AfterEnchClickEventTimeout = 1;

			ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2); // Spell failed
			pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
			pCastSpell->uSpellID = 0;
			continue;
		}
		case SPELL_BODY_REGENERATION:
		{
			switch (v731)
			{
				case 1: LODWORD(v733) = 3600 * v2; amount = 1; break;
				case 2: LODWORD(v733) = 3600 * v2; amount = 1; break;
				case 3: LODWORD(v733) = 3600 * v2; amount = 3; break;
				case 4: LODWORD(v733) = 3600 * v2; amount = 10; break;
				default:
				assert(false);
			}
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;

			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pCastSpell->uPlayerID_2);

			pParty->pPlayers[pCastSpell->uPlayerID_2].pPlayerBuffs[PLAYER_BUFF_REGENERATION].Apply(pParty->uTimePlayed + (signed int)(signed __int64)((double)(signed int)v733 * 4.2666669), v731, amount, 0, 0);
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_FIRE_PROTECTION_FROM_FIRE:
		case SPELL_AIR_PROTECTION_FROM_AIR:
		case SPELL_WATER_PROTECTION_FROM_WATER:
		case SPELL_EARTH_PROTECTION_FROM_EARTH:
		case SPELL_MIND_PROTECTION_FROM_MIND:
		case SPELL_BODY_PROTECTION_FROM_BODY:
		{
			switch (v731)
			{
				case 1: 
				case 2: 
				case 3: 
				case 4: LODWORD(v733) = 3600 * v2; amount = v731 * v2; break;
				default:
				assert(false);
			}

			switch (pCastSpell->uSpellID)
			{
			  case SPELL_FIRE_PROTECTION_FROM_FIRE:
				LODWORD(v725) = PARTY_BUFF_RESIST_FIRE;
				break;
			  case SPELL_AIR_PROTECTION_FROM_AIR:
				LODWORD(v725) = PARTY_BUFF_RESIST_AIR;
				break;
			  case SPELL_WATER_PROTECTION_FROM_WATER:
				LODWORD(v725) = PARTY_BUFF_RESIST_WATER;
				break;
			  case SPELL_EARTH_PROTECTION_FROM_EARTH:
				LODWORD(v725) = PARTY_BUFF_RESIST_EARTH;
				break;
			  case SPELL_MIND_PROTECTION_FROM_MIND:
				LODWORD(v725) = PARTY_BUFF_RESIST_MIND;
				break;
			  case SPELL_BODY_PROTECTION_FROM_BODY:
				LODWORD(v725) = PARTY_BUFF_RESIST_BODY;
				break;
			  default:
				assert(false);
				continue;
			}
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
			  break;
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 0);
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 1);
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 2);
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 3);
			v90 = (double)(signed int)v733 * 4.2666669;
			pParty->pPartyBuffs[LODWORD(v725)].Apply(pParty->uTimePlayed + (signed int)(signed __int64)v90, v731, amount, 0, 0);
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_FIRE_HASTE:
		{
			switch (v731)
			{
				case 1: LODWORD(v733) = 60 * (v2 + 60); break;
				case 2: LODWORD(v733) = 60 * (v2 + 60); break;
				case 3: LODWORD(v733) = 180 * (v2 + 20); break;
				case 4: LODWORD(v733) = 240 * (v2 + 15); break;
				default:
				assert(false);
			}
			if ( pPlayer->CanCastSpell(uRequiredMana) )
			{
				v92 = pParty->pPlayers.data();//[0].pConditions[1];
				LODWORD(v727) = 1;
				do
				{
					if ( v92->pConditions[1] )
						LODWORD(v727) = 0;
					++v92;
				}
				while ( v92 <= &pParty->pPlayers[3] );
				if (LODWORD(v727))
				{
					pParty->pPartyBuffs[PARTY_BUFF_HASTE].Apply(pParty->uTimePlayed + (signed int)(signed __int64)((double)(signed int)((int)v733 << 7) * 0.033333335), v731, 0, 0, 0);

					pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 0);
					pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 1);
					pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 2);
					pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 3);
				}
			}
			break;
		}
		case SPELL_SPIRIT_BLESS:
		{
			switch (v731)
			{
				case 1: LODWORD(v733) = 300 * (v2 + 12); break;
				case 2: LODWORD(v733) = 300 * (v2 + 12); break;
				case 3: LODWORD(v733) = 900 * (v2 + 4); break;
				case 4: LODWORD(v733) = 3600 * (v2 + 1); break;
				default:
				assert(false);
			}
			amount = v2 + 5;
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			if ( v731 == 1 )
			{
				v108 = pCastSpell->uPlayerID_2;
				v109 = pCastSpell->uSpellID;
				v110 = pGame->GetStru6();
				pGame->GetStru6()->SetPlayerBuffAnim(v109, v108);
				v111 = pOtherOverlayList->_4418B1(10000, pCastSpell->uPlayerID_2 + 310, 0, 65536);
				//v668 = 0;
				v716 = v111;
				//v658 = v111;
				//v653 = amount;
				//v651 = 1;
				//v28 = pParty->uTimePlayed + (signed int)(signed __int64)((double)(signed int)((int)v733 << 7) * 0.033333335);
				//v27 = (char *)&pParty->pPlayers[v3->uPlayerID_2].pPlayerBuffs[PLAYER_BUFF_BLESS];
		//LABEL_103:
				//HIDWORD(v650) = HIDWORD(v28);
		//LABEL_104:
				//LODWORD(v650) = v28;
				pParty->pPlayers[pCastSpell->uPlayerID_2].pPlayerBuffs[PLAYER_BUFF_BLESS].Apply(pParty->uTimePlayed + (signed int)(signed __int64)((double)(signed int)((int)v733 << 7) * 0.033333335), 1, amount, v111, 0);
				LODWORD(v727) = 1;
				break;
			}
			v105 = 0;
			v717 = (signed int)(signed __int64)((double)(signed int)((int)v733 << 7) * 0.033333335);
			v730b = pParty->pPlayers.data();//[0].pPlayerBuffs[1];
			do
			{
				pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, v105);
				v716 = pOtherOverlayList->_4418B1(10000, v105 + 310, 0, 65536);
				v730b->pPlayerBuffs[1].Apply(pParty->uTimePlayed + v717, v731, amount, v716, 0);
				++v730b;
				++v105;
			}
			while ( v730b <= &pParty->pPlayers[3] );
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_SPIRIT_SPIRIT_LASH:
		{
			if ( pPlayer->CanCastSpell(uRequiredMana) && a2 && PID_TYPE(a2) == OBJECT_Actor)
			{
				//v730 = a2 >> 3;
				v112 = &pActors[PID_ID(a2)];
				//v726 = (Player *)abs(v112->vPosition.z - pParty->vPosition.z);
				v721 = abs(v112->vPosition.y - pParty->vPosition.y);
				v113 = abs(v112->vPosition.x - pParty->vPosition.x);
				_this = (ItemGen *)v113;
				v114 = v721;
				v115 = (unsigned int)abs(v112->vPosition.z - pParty->vPosition.z);
				if ( v113 < v721 )
				{
					v116 = v113;
					v113 = v721;
					v114 = v116;
				}
				if ( v113 < (signed int)v115 )
				{
					v117 = v113;
					v113 = (int)v115;
					v115 = v117;
				}
				if ( v114 < (signed int)v115 )
				{
					v118 = v115;
					v115 = v114;
					v114 = v118;
				}
				_this = (ItemGen *)(((unsigned int)(11 * v114) >> 5) + (v115 >> 2) + v113);
				if ( (double)(signed int)_this <= 307.2 )
				{
					v701.x = 0;
					v701.y = 0;
					v701.z = 0;
					pSpellSprite.stru_24.Reset();
					pSpellSprite.spell_id = pCastSpell->uSpellID;
					pSpellSprite.spell_level = v723;
					pSpellSprite.spell_skill = v731;
					pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
					pSpellSprite.uAttributes = 0;
					pSpellSprite.uSectorID = 0;
					pSpellSprite.uSpriteFrameID = 0;
					pSpellSprite.field_60_distance_related_prolly_lod = 0;
					pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
					pSpellSprite.uFacing = 0;
					pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
					pSpellSprite.vPosition.x = v112->vPosition.x;
					pSpellSprite.vPosition.y = v112->vPosition.y;
					pSpellSprite.vPosition.z = v112->vPosition.z - (unsigned int)(signed __int64)((double)(signed int)v112->uActorHeight * unk_4D8548);
					pSpellSprite.spell_target_pid = PID(OBJECT_Actor, a2);
					v122 = pSpellSprite.Create(0, 0, 0, 0);
					DamageMonsterFromParty(PID(OBJECT_Item, v122), PID_ID(a2), &v701);
					LODWORD(v727) = 1;
				}
				else
				{
					ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2u);
					pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
					pCastSpell->uSpellID = 0;
				}
				//pCastSpell = (CastSpellInfo *)HIDWORD(v733);
			}
			break;
		}
		case SPELL_AIR_SHIELD:
		case SPELL_EARTH_STONESKIN:
		case SPELL_SPIRIT_HEROISM:
		{
			switch (v731)
			{
				case 1: LODWORD(v733) = 300 * (v2 + 12); break;
				case 2: LODWORD(v733) = 300 * (v2 + 12); break;
				case 3: LODWORD(v733) = 900 * (v2 + 4); break;
				case 4: LODWORD(v733) = 3600 * (v2 + 1); break;
				default:
				assert(false);
			}
			v127 = pCastSpell->uSpellID;
			if ( v127 == 17 )
			{
				amount = 0;
				LODWORD(v725) = 14;
			}
			else
			{
				if ( v127 == 38 )
				{
					LODWORD(v725) = 15;
					amount = v2 + 5;
				}
				else
				{
					if ( v127 != 51 )
						continue;
					LODWORD(v725) = 9;
					amount = v2 + 5;
				}
			}
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 0);
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 1);
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 2);
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 3);
			v90 = (double)(signed int)((int)v733 << 7) * 0.033333335;
	//LABEL_304:
			pParty->pPartyBuffs[LODWORD(v725)].Apply(pParty->uTimePlayed + (signed int)(signed __int64)v90, v731, amount, 0, 0);
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_FIRE_IMMOLATION:
		{
			if ( v731 == 4 )
				LODWORD(v733) = 600 * v2;
			else
				LODWORD(v733) = 60 * v2;
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;

			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 0);
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 1);
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 2);
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 3);

			pParty->pPartyBuffs[PARTY_BUFF_IMMOLATION].Apply(pParty->uTimePlayed + (signed __int64)((double)(signed int)((int)v733 << 7) * 0.033333335), v731, v2, 0, 0);
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_FIRE_METEOR_SHOWER:
		{
			//v149 = v731 - 1;
			//if ( v149 && (v150 = v149 - 1) != 0 && (v151 = v150 - 1) != 0 )
			int i;
			if ( v731 == 4 )
			{
				i = 20;
			}
			else
			{
				i = 16;
			}
			if ( uCurrentlyLoadedLevelType == LEVEL_Indoor)
			{
				ShowStatusBarString(pGlobalTXT_LocalizationStrings[491], 2);  // Can't cast Meteor Shower indoors!
				pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
				pCastSpell->uSpellID = 0;
				continue;
			}
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			LODWORD(v725) = PID_TYPE(a2);
			if (PID_TYPE(a2) == OBJECT_Actor)
			{
				uRequiredMana = pActors[PID_ID(a2)].vPosition.x;
				v153 = pActors[PID_ID(a2)].vPosition.y;
				v154 = pActors[PID_ID(a2)].vPosition.z;
				LODWORD(v727) = v153;
			}
			else
			{
				v155 = stru_5C6E00->Cos(pParty->sRotationY);
				uRequiredMana = pParty->vPosition.x + fixpoint_mul(2048, v155);
				v156 = stru_5C6E00->Sin(pParty->sRotationY);
				LODWORD(v727) = pParty->vPosition.y + fixpoint_mul(2048, v156);
				v154 = pParty->vPosition.z;
			}
			unsigned __int64 k = 0;
			int j = 0;
			if ( (signed int)i > 0 )
			{
				v730 = LODWORD(v725) == 3 ? a2 : 0;
				do
				{
					a2 = rand() % 1000;
					if ( sqrt((double)(rand() % 1000) - 2500 * 
								(double)(rand() % 1000) - 2500 + 
								j * j + k * k) <= 1.0 )
					{
						LODWORD(v687) = 0;
						HIDWORD(v687) = 0;
					}
					else
					{
						v687 = __PAIR__(
								stru_5C6E00->Atan2((signed __int64)sqrt((float)(j * j + k * k)), 
													(double)(rand() % 1000) - 2500),
													stru_5C6E00->Atan2(j, k));
					}
					pSpellSprite.stru_24.Reset();
					pSpellSprite.spell_id = pCastSpell->uSpellID;
					pSpellSprite.spell_level = v2;
					pSpellSprite.spell_skill = v731;
					pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
					pSpellSprite.uAttributes = 0;
					pSpellSprite.vPosition.x = uRequiredMana;
					pSpellSprite.vPosition.y = LODWORD(v727);
					pSpellSprite.uSectorID = 0;
					pSpellSprite.vPosition.z = a2 + v154;
					pSpellSprite.uSpriteFrameID = 0;
					pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
					pSpellSprite.spell_target_pid = v730;
					pSpellSprite.field_60_distance_related_prolly_lod = stru_50C198._427546(a2 + 2500);
					pSpellSprite.uFacing = v687;
					pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
					if ( pParty->bTurnBasedModeOn == 1 )
						pSpellSprite.uAttributes = 4;
					if ( pSpellSprite.Create(v687, SHIDWORD(v687), pObjectList->pObjects[(signed __int16)pSpellSprite.uObjectDescID].uSpeed, 0) != -1
						&& pParty->bTurnBasedModeOn == 1 )
						++pTurnEngine->pending_actions;
					j = rand() % 1024 - 512;
					k = rand() % 1024 - 512;
				}
				while ( i-- != 1 );
			}
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_FIRE_INFERNO:
		{
			//v67 = 2;
			if ( uCurrentlyLoadedLevelType == LEVEL_Outdoor)
			{
				ShowStatusBarString(pGlobalTXT_LocalizationStrings[492], 2); // Can't cast Inferno outdoors!
				pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
				pCastSpell->uSpellID = 0;
				continue;
			}
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			int _v726 = _46A6AC_spell_render((int)dword_50BF30.data(), 100, 4096);
			v700.z = 0;
			v700.y = 0;
			v700.x = 0;
			pSpellSprite.stru_24.Reset();
			pSpellSprite.spell_id = pCastSpell->uSpellID;
			pSpellSprite.spell_level = v2;
			pSpellSprite.spell_skill = v731;
			pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
			pSpellSprite.uAttributes = 0;
			pSpellSprite.uSectorID = 0;
			pSpellSprite.uSpriteFrameID = 0;
			pSpellSprite.field_60_distance_related_prolly_lod = 0;
			pSpellSprite.uFacing = 0;
			pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
			pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
			a2 = 0;
			if ( (signed int)_v726 > 0 )
			{
				do
				{
					v162 = dword_50BF30[a2];
					pSpellSprite.vPosition.x = pActors[v162].vPosition.x;
					pSpellSprite.vPosition.y = pActors[v162].vPosition.y;
					pSpellSprite.vPosition.z = pActors[v162].vPosition.z - (unsigned int)(signed __int64)((double)pActors[v162].uActorHeight * unk_4D8548);
					pSpellSprite.spell_target_pid = PID(OBJECT_Actor, dword_50BF30[a2]);
					v164 = pSpellSprite.Create(0, 0, 0, 0);
					v165 = a2;
					DamageMonsterFromParty(PID(OBJECT_Item, v164), dword_50BF30[a2], &v700);
					pGame->GetStru6()->_4A81CA(&pSpellSprite);
					pGame->GetStru6()->FadeScreen__like_Turn_Undead_and_mb_Armageddon(0xFF3C1Eu, 0x40u);
					a2 = v165 + 1;
				}
				while ( v165 + 1 < (signed int)_v726 );
			}
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_AIR_WIZARD_EYE:
		{
			LODWORD(v733) = 3600 * v2;
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			v168 = 0;
			do
			{
				v169 = pOtherOverlayList->_4418B1(2000, v168++ + 100, 0, 65536);
				v716 = v169;
			}
			while ( v168 < 4 );

			pParty->pPartyBuffs[PARTY_BUFF_WIZARD_EYE].Apply(pParty->uTimePlayed + (signed __int64)((double)(signed int)((int)v733 << 7) * 0.033333335), v731, 0, 0, 0);
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_AIR_FEATHER_FALL:
		{
			switch (v731)
			{
				case 1: LODWORD(v733) = 300 * v2; break;
				case 2: LODWORD(v733) = 600 * v2; break;
				case 3: LODWORD(v733) = 3600 * v2; break;
				case 4: LODWORD(v733) = 3600 * v2; break;
				default:
				assert(false);
			}
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			v174 = 0;
			do
				pOtherOverlayList->_4418B1(2010, v174++ + 100, 0, 65536);
			while ( v174 < 4 );
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 0);
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 1);
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 2);
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 3);

			pParty->pPartyBuffs[PARTY_BUFF_FEATHER_FALL].Apply(pParty->uTimePlayed + (signed int)(signed __int64)((double)(signed int)((int)v733 << 7) * 0.033333335), v731, 0, 0, 0);
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_AIR_SPARKS:
		{
			switch (v731)
			{
				case 1: amount = 3; break;
				case 2: amount = 5; break;
				case 3: amount = 7; break;
				case 4: amount = 9; break;
				default:
				assert(false);
			}
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			int _v726 = (signed int)(60 * stru_5C6E00->uIntegerDoublePi) / 360;
			pSpellSprite.stru_24.Reset();
			pSpellSprite.spell_id = pCastSpell->uSpellID;
			pSpellSprite.spell_level = v2;
			pSpellSprite.spell_skill = v731;
			pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
			pSpellSprite.vPosition.y = pParty->vPosition.y;
			pSpellSprite.vPosition.x = pParty->vPosition.x;
			pSpellSprite.uAttributes = 0;
			pSpellSprite.vPosition.z = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3;
			pSpellSprite.uSectorID = pIndoor->GetSector(
								pParty->vPosition.x,
								pParty->vPosition.y,
								pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3);
			pSpellSprite.uSpriteFrameID = 0;
			pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
			pSpellSprite.spell_target_pid = a2;
			pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
			pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
			if ( pParty->bTurnBasedModeOn == 1 )
				LOBYTE(pSpellSprite.uAttributes) |= 4u;
			v188 = (signed int)_v726 / -2;
			v189 = (signed int)_v726 / 2;
			while ( v188 <= v189 )
			{
				pSpellSprite.uFacing = v188 + LOWORD(v715.uYawAngle);
				if ( pSpellSprite.Create(
						(signed __int16)(v188 + LOWORD(v715.uYawAngle)),
						v715.uPitchAngle,
						pObjectList->pObjects[(signed __int16)pSpellSprite.uObjectDescID].uSpeed,
						pCastSpell->uPlayerID + 1) != -1
						&& pParty->bTurnBasedModeOn == 1 )
				    ++pTurnEngine->pending_actions;
				v188 += _v726 / (amount - 1);
			}
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_AIR_JUMP:
		{
			if ( pParty->uFlags & PARTY_FLAGS_1_FALLING)
			{
				ShowStatusBarString(pGlobalTXT_LocalizationStrings[493], 2);  // Can't cast Jump while airborne!
				pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
				break;
			}
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			v191 = 0;
			do
				pOtherOverlayList->_4418B1(2040, v191++ + 100, 0, 65536);
			while ( v191 < 4 );
			BYTE1(pParty->uFlags) |= 1u;
			pParty->uFallSpeed = 1000;
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_AIR_INVISIBILITY:
		{
			switch (v731)
			{
				case 1: LODWORD(v733) = 600 * v2; amount = v2; break;
				case 2: LODWORD(v733) = 600 * v2; amount = 2 * v2; break;
				case 3: LODWORD(v733) = 600 * v2; amount = 3 * v2; break;
				case 4: LODWORD(v733) = 3600 * v2; amount = 4 * v2; break;
				default:
				assert(false);
			}
			if (pParty->uFlags & (PARTY_FLAGS_1_ALERT_RED | PARTY_FLAGS_1_ALERT_YELLOW))
			{
				ShowStatusBarString(pGlobalTXT_LocalizationStrings[638], 2);  // There are hostile creatures nearby!
				pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
				pCastSpell->uSpellID = 0;
				continue;
			}
			if ( pPlayer->CanCastSpell(uRequiredMana) )
			{
				pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 0);
				pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 1);
				pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 2);
				pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 3);

				pParty->pPartyBuffs[PARTY_BUFF_INVISIBILITY].Apply(pParty->uTimePlayed + (signed int)(signed __int64)((double)(signed int)((int)v733 << 7) * 0.033333335), v731, amount, 0, 0);
				LODWORD(v727) = 1;
			}
			break;
		}
		case SPELL_AIR_FLY:
		{
			if ( uCurrentlyLoadedLevelType == LEVEL_Indoor)
			{
				ShowStatusBarString(pGlobalTXT_LocalizationStrings[494], 2);  // Can not cast Fly indoors!
				pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
				break;
			}
			if ( !pPlayers[pCastSpell->uPlayerID + 1]->GetMaxMana() )
			{
				ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2);  // Spell failed
				pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
				break;
			}
			LODWORD(v733) = 3600 * v2;
			if ( v731 == 1 || v731 == 2 || v731 == 3 )
				amount = 1;
			else
				amount = 0;
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			v205 = 0;
			do
				pOtherOverlayList->_4418B1(2090, v205++ + 100, 0, 65536);
			while ( v205 < 4 );
			v206 = pOtherOverlayList->_4418B1(10008, 203, 0, 65536);
			v207 = pCastSpell->uPlayerID + 1;
			v716 = v206;

			pParty->pPartyBuffs[PARTY_BUFF_FLY].Apply(pParty->uTimePlayed + (signed __int64)((double)(signed int)((int)v733 << 7) * 0.033333335), v731, amount, v206, v207);
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_AIR_STARBURST:
		{
			//v67 = 2;
			if ( uCurrentlyLoadedLevelType == LEVEL_Indoor)
			{
				ShowStatusBarString(pGlobalTXT_LocalizationStrings[495], 2);  // Can't cast Starburst indoors!
				pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
				pCastSpell->uSpellID = 0;
				continue;
			}
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			v208 = PID_TYPE(a2);
			LODWORD(v725) = PID_TYPE(a2);
			if ( v208 == 3 )
			{
				v209 = PID_ID(a2);
				LODWORD(v718) = pActors[v209].vPosition.x;
				v210 = pActors[v209].vPosition.y;
				v211 = pActors[v209].vPosition.z;
				v713 = v210;
			}
			else
			{
				v212 = stru_5C6E00->Cos(pParty->sRotationY);
				LODWORD(v718) = pParty->vPosition.x + fixpoint_mul(2048, v212);
				v213 = stru_5C6E00->Sin(pParty->sRotationY);
				v214 = fixpoint_mul(2048, v213);
				v211 = pParty->vPosition.z;
				v713 = pParty->vPosition.y + v214;
				v208 = LODWORD(v725);
			}
			signed int _v733 = 0;
			*(float *)&v732 = (double)v211;
			LODWORD(v725) = v211 + 2500;
			v721 = 0;
			*(float *)&y = (double)(v211 + 2500);
			v730 = v208 == 3 ? a2 : 0;
			a2 = 20;
			do
			{
				v215 = rand();
				v216 = (double)v721;
				v217 = (double)_v733;
				*(float *)&uRequiredMana = v217;
				_this = (ItemGen *)(v215 % 1000);
				*((float *)&v733 + 1) = (double)(v215 % 1000) + *(float *)&v732 - *(float *)&y;
				*(float *)&v721 = v217 * v217;
				//*(float *)&v726 = v216 * v216;
				if ( sqrt(*((float *)&v733 + 1) * *((float *)&v733 + 1) + v216 * v216 + *(float *)&v721) <= 1.0 )
				{
					LODWORD(v685) = 0;
					HIDWORD(v685) = 0;
				}
				else
				{
					v684 = (signed __int64)sqrt(v216 * v216 + *(float *)&v721);
					v685 = __PAIR__(
								stru_5C6E00->Atan2(v684, (signed __int64)*((float *)&v733 + 1)),
								stru_5C6E00->Atan2((signed __int64)v216, (signed __int64)*(float *)&uRequiredMana));
				}
				pSpellSprite.stru_24.Reset();
				pSpellSprite.spell_id = pCastSpell->uSpellID;
				pSpellSprite.spell_level = v2;
				pSpellSprite.spell_skill = v731;
				pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
				pSpellSprite.uAttributes = 0;
				pSpellSprite.vPosition.x = LODWORD(v718);
				pSpellSprite.vPosition.y = v713;
				pSpellSprite.uSectorID = 0;
				pSpellSprite.vPosition.z = (int)((char *)_this + LODWORD(v725));
				pSpellSprite.uSpriteFrameID = 0;
				pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
				pSpellSprite.spell_target_pid = v730;
				//__debugbreak();//звездопад
				pSpellSprite.field_60_distance_related_prolly_lod = stru_50C198._427546(((int)_this) + 2500);
				pSpellSprite.uFacing = v685;
				pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
				if ( pParty->bTurnBasedModeOn == 1 )
				pSpellSprite.uAttributes = 4;
				if ( pSpellSprite.Create(v685, SHIDWORD(v685), pObjectList->pObjects[(signed __int16)pSpellSprite.uObjectDescID].uSpeed, 0) != -1
					   && pParty->bTurnBasedModeOn == 1 )
				  ++pTurnEngine->pending_actions;
				v721 = rand() % 1024 - 512;
				v218 = rand();
				v14 = a2-- == 1;
				_v733 = (unsigned __int64)(v218 % 1024) - 512;
			}
			while ( !v14 );
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_WATER_AWAKEN:
		{
			switch (v731)
			{
				case 1: amount = 180 * v2; break;
				case 2: amount = 3600 * v2; break;
				case 3: amount = 86400 * v2; break;
				case 4: amount = 0; break;
				default:
				assert(false);
			}
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			for( int i=0; i < 4; i++ )
			{
				pPlayer = &pParty->pPlayers[i];
				if ( v731 == 4 )
				{
					if ( pPlayer->pConditions[2] )
					{
						//*((int *)v222 + 4) = 0;
						//*((int *)v222 + 5) = 0;
						pPlayer->pConditions[2] = 0;
						pPlayer->PlaySound(SPEECH_103, 0);
					}
				}
				else
				{
					v223 = pPlayer->DiscardConditionIfLastsLongerThan(
								2u,
								pParty->uTimePlayed - (signed int)(signed __int64)((double)(amount << 7) * 0.033333335));
					if ( v223 )
						pPlayer->PlaySound(SPEECH_103, 0);
				}
			}
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_WATER_POISON_SPRAY:
		{
			switch (v731)
			{
				case 1: amount = 1; break;
				case 2: amount = 3; break;
				case 3: amount = 5; break;
				case 4: amount = 7; break;
				default:
				assert(false);
			}
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			signed int _v733 = (signed int)(60 * stru_5C6E00->uIntegerDoublePi) / 360;
			if ( amount == 1 )
			{
				pSpellSprite.stru_24.Reset();
				pSpellSprite.spell_id = pCastSpell->uSpellID;
				pSpellSprite.spell_skill = v731;
				pSpellSprite.spell_level = v2;
				pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
				pSpellSprite.vPosition.y = pParty->vPosition.y;
				v51 = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3;
				v671 = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3;
				v660 = pParty->vPosition.y;
//	LABEL_153:
				pSpellSprite.uAttributes = 0;
				pSpellSprite.vPosition.x = pParty->vPosition.x;
				pSpellSprite.vPosition.z = v51;
				v52 = pIndoor->GetSector(pParty->vPosition.x, v660, v671);
				pSpellSprite.uSpriteFrameID = 0;
				pSpellSprite.uSectorID = v52;
				pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
				pSpellSprite.spell_target_pid = a2;
				pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
				pSpellSprite.uFacing = LOWORD(v715.uYawAngle);
				pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
				if ( pParty->bTurnBasedModeOn == 1 )
					LOBYTE(pSpellSprite.uAttributes) |= 4u;
				v659 = pObjectList->pObjects[(signed __int16)pSpellSprite.uObjectDescID].uSpeed;
				if ( pSpellSprite.Create(v715.uYawAngle, v715.uPitchAngle, v659, pCastSpell->uPlayerID + 1) != -1 && pParty->bTurnBasedModeOn == 1 )
					++pTurnEngine->pending_actions;
			}
			else
			{
				pSpellSprite.stru_24.Reset();
				pSpellSprite.spell_id = pCastSpell->uSpellID;
				pSpellSprite.spell_level = v2;
				pSpellSprite.spell_skill = v731;
				pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
				pSpellSprite.vPosition.y = pParty->vPosition.y;
				pSpellSprite.vPosition.x = pParty->vPosition.x;
				pSpellSprite.uAttributes = 0;
				pSpellSprite.vPosition.z = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3;
				pSpellSprite.uSectorID = pIndoor->GetSector(pParty->vPosition.x, pParty->vPosition.y, pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3);
				pSpellSprite.uSpriteFrameID = 0;
				pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
				pSpellSprite.spell_target_pid = a2;
				pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
				pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
				if ( pParty->bTurnBasedModeOn == 1 )
					LOBYTE(pSpellSprite.uAttributes) |= 4u;
				v227 = _v733 / -2;
				signed int _y = _v733 / 2;
				if ( _v733 / -2 <= _v733 / 2 )
				{
					v228 = v715.uYawAngle;
					do
					{
						pSpellSprite.uFacing = v227 + v228;
						if ( pSpellSprite.Create(
								v227 + v228,
								v715.uPitchAngle,
								pObjectList->pObjects[(signed __int16)pSpellSprite.uObjectDescID].uSpeed,
								pCastSpell->uPlayerID + 1) != -1
								&& pParty->bTurnBasedModeOn == 1 )
							++pTurnEngine->pending_actions;
						v227 += _v733 / (amount - 1);
					}
					while ( v227 <= _y );
				}
			}
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_WATER_WATER_WALK:
		{
			if ( !pPlayers[pCastSpell->uPlayerID + 1]->GetMaxMana() )
			{
				ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2);  // Spell failed
				pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
				break;
			}
			if ( v731 == 2 || v731 != 3 && v731 != 4 )
				v229 = 600 * v2;
			else
				v229 = 3600 * v2;
			LODWORD(v733) = v229;
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			v716 = pOtherOverlayList->_4418B1(10005, 201, 0, 65536);
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 0);
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 1);
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 2);
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 3);

			pParty->pPartyBuffs[PARTY_BUFF_WATER_WALK].Apply(
				pParty->uTimePlayed + (signed int)(signed __int64)((double)(v229 << 7) * 0.033333335),
				v731,
				amount,
				v716,
				pCastSpell->uPlayerID + 1);
			if ( v731 == 4 )
				pParty->pPartyBuffs[PARTY_BUFF_WATER_WALK].uFlags = 1;
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_WATER_RECHARGE_ITEM:
		{
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			v240 = &pParty->pPlayers[pCastSpell->uPlayerID_2].pInventoryItemList[a2];
			
			if ( v240->GetItemEquipType() != 12 || v240->uAttributes & 2 )
			{
				_50C9D0_AfterEnchClickEventId = 113;
				_50C9D4_AfterEnchClickEventSecondParam = 0;
				_50C9D8_AfterEnchClickEventTimeout = 1;

				ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2);  // Spell failed
				pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
				pCastSpell->uSpellID = 0;
				continue;
			}
			if ( v731 == 1 || v731 == 2 )
			{
				v241 = (double)v723 * 0.0099999998 + 0.5;
			}
			else if ( v731 == 3 )
			{
				v241 = (double)v723 * 0.0099999998 + 0.69999999;
			}
			else if ( v731 == 4 )
			{
				v241 = (double)v723 * 0.0099999998 + 0.80000001;
			}
			else
			{
				v241 = 0.0;
			}
			if ( v241 > 1.0 )
				v241 = 1.0;
			int uNewCharges = v240->uMaxCharges * v241;
			v240->uMaxCharges = uNewCharges;
			v240->uNumCharges = uNewCharges;
			if ( uNewCharges <= 0 )
			{
				v240 = 0;
				_50C9D0_AfterEnchClickEventId = 113;
				_50C9D4_AfterEnchClickEventSecondParam = 0;
				_50C9D8_AfterEnchClickEventTimeout = 1;
				ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2u);
				pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
				pCastSpell->uSpellID = 0;
				v2 = v723;
				continue;
			}
			v240->uAttributes |= 0x40u;
			_50C9A8_item_enchantment_timer = 256;
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_WATER_ENCHANT_ITEM:
		{
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
        break;
			uRequiredMana = 0;
			amount = 10 * v2;
			v730 = 1;
			pPlayer = &pParty->pPlayers[pCastSpell->uPlayerID_2];
			v245 = &pPlayer->pInventoryItemList[a2];
			ItemDesc *_v725 = &pItemsTable->pItems[v245->uItemID];
			if ( 
				v731 == 1 || v731 == 2 && _v725->uEquipType > 2 ||
				v731 == 3 || v731 == 4 && 
				v245->uItemID <= 134 &&
				v245->uSpecEnchantmentType == 0 &&
				v245->uEnchantmentType == 0 &&
				v245->m_enchantmentStrength== 0 &&
				!v245->IsBroken() )
			{
				if ( v245->GetValue() < 450 || 
					(v245->GetValue() < 250 && (v731 == 3 || v731 == 4) && _v725->uEquipType >= 0 && _v725->uEquipType <= 2)
					)
				{
					v730 = 0;
				}

                __debugbreak(); // castspellinfo.cpp(1962): warning C4700: uninitialized local variable 'v271' used
				if ( rand() % 100 < 10 * v2 || 
					(rand() % 100 < 80 && (v731 == 3 || v731 == 4 )) ||
					v245->GetValue() < 450 || 
					(v245->GetValue() < 250 && (v731 == 3 || v731 == 4) && v271 >= 0 && v271 <= 2)
					)
				{
					v313 = _v725->uEquipType;
					if ( _v725->uEquipType >= 3 && _v725->uEquipType <= 11 )
					{
						v295 = rand() % 10;// pItemsTable->field_116D8[pItemsTable->pItems[_this->uItemID].uEquipType];
						v245->uEnchantmentType = 0;
                        __debugbreak(); // castspellinfo.cpp(1971): warning C4700: uninitialized local variable 'v294' used
						for ( kk = pItemsTable->pEnchantments[0].to_item[v245->GetItemEquipType() + 1];
							;
							kk += pItemsTable->pEnchantments[v294->uEnchantmentType].to_item[v245->GetItemEquipType() + 1] )
						{
							++v245->uEnchantmentType;
							if ( kk >= v295 )
								break;
						}
						v255 = 10;//pItemsTable->field_116D8[17];
						v256 = 10;//pItemsTable->field_116D8[16];
						v245->m_enchantmentStrength = v256 + rand() % (v255 - v256 + 1);
						v245->uAttributes |= 0x20u;
						_50C9A8_item_enchantment_timer = 256;
						LODWORD(v727) = 1;
						break;
					}
					else if ( v731 == 3 || v731 == 4)
					{
					v257 = pItemsTable->pSpecialEnchantments_count;
					v258 = 0;
					v725 = 0.0;
					int _v733 = 0;
					if ( pItemsTable->pSpecialEnchantments_count > 0 )
					{
						v730 = (int)&v679;
						do
						{
							v259 = LOBYTE(pItemsTable->pSpecialEnchantments[v258 + 1].pBonusStatement);
							if ( !v259 || v259 == 1 )
							{
								v260 = *(&pItemsTable->pSpecialEnchantments[0].to_item_apply[v245->GetItemEquipType()
																					+ 4]
										+ v258 * 28);
								LODWORD(v725) += v260;
								if ( v260 )
								{
									v261 = v730;
									v730 += 4;
									*(int *)v261 = _v733;
								}
							}
							++_v733;
							++v258;
						}
						while ( _v733 < v257 );
					}
					v262 = rand() % SLODWORD(v725);
					v263 = v679;
					v245->uSpecEnchantmentType = v679[0];
					v264 = pItemsTable->pSpecialEnchantments[*v263].to_item_apply[v245->GetItemEquipType() + 4];
					v265 = v262 + 1;
					if ( v264 < v265 )
					{
						for ( ii = v679; ; ii = (int *)v732 )
						{
							v267 = (int)(ii + 1);
							v732 = v267;
							v268 = *(int *)v267;
							*(int *)(v245 + 12) = v268;
							v264 += pItemsTable->pSpecialEnchantments[v268].to_item_apply[v245->GetItemEquipType()
																				+ 4];
							if ( v264 >= v265 )
								break;
						}
					}

						v277 = pItemsTable->pSpecialEnchantments_count;
						v278 = 0;
						v725 = 0.0;
						_v733 = 0;
						if ( pItemsTable->pSpecialEnchantments_count > 0 )
						{
							int *_v730 = v679;
							do
							{
								v279 = LOBYTE(pItemsTable->pSpecialEnchantments[v278].pBonusStatement);
								if ( !v279 || v279 == 1 )
								{
									v280 = *(&pItemsTable->pSpecialEnchantments[v278].to_item_apply[v245->GetItemEquipType()]);
									_v733 += v280;
									if ( v280 )
									{
										v281 = _v730;
										++_v730;
										*v281 = _v733;
									}
								}
								++_v733;
								++v278;
							}
							while ( _v733 < v277 );
						}
						v282 = rand() % _v733;
						v283 = v679;
						v245->uSpecEnchantmentType = v679[0];
						v284 = pItemsTable->pSpecialEnchantments[*v283].to_item_apply[v245->GetItemEquipType() ];
						v285 = v282 + 1;
						for ( l = v679; v284 < v285; ++l )
						{
							v245->uSpecEnchantmentType = *(l+1);
							v284 += pItemsTable->pSpecialEnchantments[*(l+1)].to_item_apply[v245->GetItemEquipType()];
						}
						++v245->uSpecEnchantmentType;
						v245->uAttributes |= 0x20u;
						_50C9A8_item_enchantment_timer = 256;
						LODWORD(v727) = 1;
						break;
					}
				}
				else
				{
					if ( !(BYTE1(v245->uAttributes) & 2) )
					{
						v245->uAttributes |= 2;
					}
				}
						
			}

			if ( LODWORD(v727) == 0 )
			{
				v317 = pGlobalTXT_LocalizationStrings[428];
				if ( v730 == 0 )
					v317 = pGlobalTXT_LocalizationStrings[585];
				ShowStatusBarString(v317, 2u);
				pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
				v318 =  &pParty->pPlayers[pCastSpell->uPlayerID_2];
				pCastSpell->uSpellID = 0;
				v318->PlaySound(SPEECH_43, 0);
			}

			break;
		}
		case SPELL_WATER_TOWN_PORTAL:
		{
			amount = 10 * v2;
			if ( pPlayer->sMana < (signed int)uRequiredMana )
				break;
			if ( pParty->uFlags & (PARTY_FLAGS_1_ALERT_RED | PARTY_FLAGS_1_ALERT_YELLOW) && v731 != 4 || rand() % 100 >= amount && v731 != 4 )
			{
				ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2); // Spell failed
				pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
				pCastSpell->uSpellID = 0;
				continue;
			}
			town_portal_caster_id = LOBYTE(pCastSpell->uPlayerID);
			pMessageQueue_50CBD0->AddMessage(UIMSG_OnCastTownPortal, 0, 0);
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_WATER_LLOYDS_BEACON:
		{
			LODWORD(v733) = 604800 * v2;
			if ( !_stricmp(pCurrentMapName, "d05.blv") )
			{
				ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2); // Spell failed
				pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
				pCastSpell->uSpellID = 0;
				continue;
			}
			v319 = uRequiredMana;
			if ( pPlayer->sMana >= (signed int)uRequiredMana )
			{
				pEventTimer->Pause();
				pMessageQueue_50CBD0->AddMessage(UIMSG_OnCastLloydsBeacon, 0, 0);
				qword_506350 = (signed int)v733;
				_506348_current_lloyd_playerid = pCastSpell->uPlayerID;
				::uRequiredMana = v319;
				::sRecoveryTime = sRecoveryTime;
				dword_50633C = pCastSpell->sound_id;
				dword_506338 = pCastSpell->uSpellID;
				pCastSpell->uFlags |= 0x20u;
			}
			break;
		}
		case SPELL_EARTH_STONE_TO_FLESH:
		{
			__debugbreak(); // missing GM ?
			switch (v731)
			{
				case 1: amount = 3600 * v2; break;
				case 2: amount = 3600 * v2; break;
				case 3: amount = 86400 * v2; break;
				case 4: break;
				default:
				assert(false);
			}
	//LABEL_634:
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			v323 = pCastSpell->uPlayerID_2;
			v324 = (char *)&pParty->pPlayers[v323].pConditions[15];
			if ( !pParty->pPlayers[v323].pConditions[15] )
			{
				LODWORD(v727) = 1;
				break;
			}
			if ( v731 == 4 )
			{
				*(int *)v324 = 0;
				*((int *)v324 + 1) = 0;
				LODWORD(v727) = 1;
				break;
			}
			v732 = amount << 7;
			v663 = (signed __int64)((double)(signed __int64)pParty->uTimePlayed - (double)(amount << 7) * 0.033333335);
			v656 = 15;
			v325 = &pParty->pPlayers[v323];
			v325->DiscardConditionIfLastsLongerThan(v656, v663);
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_EARTH_ROCK_BLAST:
		{
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			pSpellSprite.stru_24.Reset();
			pSpellSprite.spell_id = pCastSpell->uSpellID;
			pSpellSprite.spell_level = v2;
			pSpellSprite.spell_skill = v731;
			pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
			pSpellSprite.vPosition.y = pParty->vPosition.y;
			pSpellSprite.vPosition.x = pParty->vPosition.x;
			pSpellSprite.uAttributes = 0;
			pSpellSprite.vPosition.z = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3;
			pSpellSprite.uSectorID = pIndoor->GetSector(
								pParty->vPosition.x,
								pParty->vPosition.y,
								pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3);
			pSpellSprite.uSpriteFrameID = 0;
			pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
			pSpellSprite.spell_target_pid = a2;
			pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
			pSpellSprite.uFacing = LOWORD(pParty->sRotationY);
			pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
			if ( pParty->bTurnBasedModeOn == 1 )
				LOBYTE(pSpellSprite.uAttributes) |= 4u;

			v659 = pObjectList->pObjects[(signed __int16)pSpellSprite.uObjectDescID].uSpeed;
			if ( pSpellSprite.Create(pParty->sRotationY, pParty->sRotationX, v659, pCastSpell->uPlayerID + 1) != -1 && pParty->bTurnBasedModeOn == 1 )
				++pTurnEngine->pending_actions;
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_EARTH_DEATH_BLOSSOM:
		{
			if ( uCurrentlyLoadedLevelType == LEVEL_Indoor)
			{
				ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2); // Spell failed
				pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
				pCastSpell->uSpellID = 0;
				continue;
			}
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			pSpellSprite.uType = 4090;
			pSpellSprite.stru_24.Reset();
			pSpellSprite.spell_id = pCastSpell->uSpellID;
			pSpellSprite.spell_level = v2;
			pSpellSprite.spell_skill = v731;
			pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
			pSpellSprite.vPosition.x = pParty->vPosition.x;
			pSpellSprite.vPosition.y = pParty->vPosition.y;
			pSpellSprite.uAttributes = 0;
			pSpellSprite.uSectorID = 0;
			pSpellSprite.uSpriteFrameID = 0;
			pSpellSprite.vPosition.z = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 3;
			pSpellSprite.field_60_distance_related_prolly_lod = 0;
			pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
			pSpellSprite.spell_target_pid = a2;
			pSpellSprite.uFacing = LOWORD(pParty->sRotationY);
			pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
			if ( pParty->bTurnBasedModeOn == 1 )
				pSpellSprite.uAttributes = 4;

			v659 = pObjectList->pObjects[(signed __int16)pSpellSprite.uObjectDescID].uSpeed;
			if ( pSpellSprite.Create(pParty->sRotationY, stru_5C6E00->uIntegerHalfPi / 2, v659, 0) != -1 && pParty->bTurnBasedModeOn == 1 )
				++pTurnEngine->pending_actions;
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_SPIRIT_DETECT_LIFE:
		{
			v328 = v731 - 2;
			if ( v328 )
			{
				v329 = v328 - 1;
				if ( v329 && v329 != 1 )
					v330 = 600 * v2;
				else
					v330 = 3600 * v2;
			}
			else
			{
				v330 = 1800 * v2;
			}
			LODWORD(v733) = v330;
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 0);
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 1);
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 2);
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 3);

			pParty->pPartyBuffs[PARTY_BUFF_DETECT_LIFE].Apply(pParty->uTimePlayed + (signed int)(signed __int64)((double)(signed int)((int)v733 << 7) * 0.033333335), v731, 0, 0, 0);
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_SPIRIT_FATE:
		{
			switch (v731)
			{
				case 1: amount = 1 * v2; break;
				case 2: amount = 2 * v2; break;
				case 3: amount = 4 * v2; break;
				case 4: amount = 6 * v2; break;
				default:
				assert(false);
			}
			LODWORD(v733) = 300;
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			v342 = pCastSpell->spell_target_pid;
			if ( v342 == 0 )
			{
				pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pCastSpell->uPlayerID_2);

				pParty->pPlayers[pCastSpell->uPlayerID_2].pPlayerBuffs[PLAYER_BUFF_FATE].Apply(pParty->uTimePlayed + 1280, v731, amount, 0, 0);
				LODWORD(v727) = 1;
				break;
			}
			if (PID_TYPE(v342) == OBJECT_Actor)
			{
				v343 = PID_ID(v342);
				HIDWORD(v344) = 0 + ((pParty->uTimePlayed + 1280) >> 32);
				LODWORD(v344) = LODWORD(pParty->uTimePlayed) + 1280;
				pActors[v343].pActorBuffs[ACTOR_BUFF_FATE].Apply(v344, v731, amount, 0, 0);
				BYTE2(pActors[v343].uAttributes) |= 8u;
				//v672 = 0;
				v661 = &pActors[v343];
				pGame->GetStru6()->_4A7E89_sparkles_on_actor_after_it_casts_buff(v661, 0);
			}
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_SPIRIT_REMOVE_CURSE:
		{
			switch (v731)
			{
				case 1: amount = 3600 * v2; break;
				case 2: amount = 3600 * v2; break;
				case 3: amount = 86400 * v2; break;
				case 4: amount = 0; break;
				default:
				assert(false);
			}
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			v351 = &pParty->pPlayers[pCastSpell->uPlayerID_2];
			if ( !v351->pConditions[0] )
			{
				LODWORD(v727) = 1;
				break;
			}
			if ( v731 == 4 )
			{
				LODWORD(v351->pConditions[0]) = 0;
				HIDWORD(v351->pConditions[0]) = 0;
			}
			else
			{
				v732 = amount << 7;
				v351->DiscardConditionIfLastsLongerThan(0, (signed __int64)((double)(signed __int64)pParty->uTimePlayed - (double)(amount << 7) * 0.033333335));
				if ( HIDWORD(pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[0]) | LODWORD(pParty->pPlayers[pCastSpell->uPlayerID_2].pConditions[0]) )
				{
					LODWORD(v727) = 1;
					break;
				}
			}
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pCastSpell->uPlayerID_2);
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_SPIRIT_PRESERVATION:
		{
			if ( v731 == 4 )
				LODWORD(v733) = 900 * (v2 + 4);
			else
				LODWORD(v733) = 300 * (v2 + 12);
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			if ( v731 == 1 || v731 == 2 )
			{
				pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pCastSpell->uPlayerID_2);
				pParty->pPlayers[pCastSpell->uPlayerID_2].pPlayerBuffs[PLAYER_BUFF_PRESERVATION].Apply(pParty->uTimePlayed + (signed int)(signed __int64)((double)(signed int)((int)v733 << 7) * 0.033333335), v731, 0, 0, 0);
				LODWORD(v727) = 1;
				break;
			}
			a2 = 0;
			v717 = (signed int)(signed __int64)((double)(signed int)((int)v733 << 7) * 0.033333335);
			v357 = pParty->pPlayers.data();//[0].pPlayerBuffs[11];
			do
			{
				pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, a2);
				v357->pPlayerBuffs[PLAYER_BUFF_PRESERVATION].Apply(pParty->uTimePlayed + v717, v731, 0, 0, 0);
				++a2;
				++v357;// = (SpellBuff *)((char *)v357 + 6972);
			}
			while ( v357 <= &pParty->pPlayers[3] );
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_SPIRIT_TURN_UNDEAD:
		{
			if ( v731 == 1 || v731 == 2)
				LODWORD(v733) = 60 * (v2 + 3);
			else
				LODWORD(v733) = 300 * v2 + 180;
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			int _v726 = _46A6AC_spell_render((int)dword_50BF30.data(), 100, 4096);
			pGame->GetStru6()->FadeScreen__like_Turn_Undead_and_mb_Armageddon(0xFFFFFFu, 0xC0u);
			++pSpellSprite.uType;
			pSpellSprite.stru_24.Reset();
			pSpellSprite.spell_id = pCastSpell->uSpellID;
			pSpellSprite.spell_level = v2;
			pSpellSprite.spell_skill = v731;
			pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
			pSpellSprite.uAttributes = 0;
			pSpellSprite.uSectorID = 0;
			pSpellSprite.uSpriteFrameID = 0;
			pSpellSprite.field_60_distance_related_prolly_lod = 0;
			pSpellSprite.uFacing = 0;
			pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
			pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
			for ( a2 = 0; a2 < (signed int)_v726; ++a2 )
			{
				v369 = &pActors[dword_50BF30[a2]];
				if ( MonsterStats::BelongsToSupertype(v369->pMonsterInfo.uID, MONSTER_SUPERTYPE_UNDEAD) )
				{
					pSpellSprite.vPosition.x = v369->vPosition.x;
					pSpellSprite.vPosition.y = v369->vPosition.y;
					pSpellSprite.vPosition.z = v369->vPosition.z - (unsigned int)(signed __int64)((double)v369->uActorHeight * unk_4D8548);

					pSpellSprite.spell_target_pid = PID(OBJECT_Actor, dword_50BF30[a2]);
					pSpellSprite.Create(0, 0, 0, 0);
					v369->pActorBuffs[ACTOR_BUFF_AFRAID].Apply(
						pParty->uTimePlayed + (signed __int64)((double)(signed int)((int)v733 << 7) * 0.033333335),
						v731, 0, 0, 0);
				}
			}
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_SPIRIT_RAISE_DEAD:
		{
			if ( v731 == 4 )
				amount = 0;
			else
				amount = 86400 * v2;
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			pOtherOverlayList->_4418B1(5080, pCastSpell->uPlayerID_2 + 100, 0, 65536);
			v373 = pCastSpell->uPlayerID_2;
			if ( !(HIDWORD(pParty->pPlayers[v373].pConditions[14]) | LODWORD(pParty->pPlayers[v373].pConditions[14])) )
			{
				LODWORD(v727) = 1;
				break;
			}
			v14 = v731 == 4;
			pParty->pPlayers[v373].sHealth = 1;
			if ( v14 )
			{
				v374 = pCastSpell->uPlayerID_2;
				LODWORD(pParty->pPlayers[v374].pConditions[14]) = 0;
				HIDWORD(pParty->pPlayers[v374].pConditions[14]) = 0;
				v376 = pCastSpell->uPlayerID_2;
				LODWORD(pParty->pPlayers[v376].pConditions[13]) = 0;
				HIDWORD(pParty->pPlayers[v376].pConditions[13]) = 0;
			}
			else
			{
				*(float *)&a2 = (double)(amount << 7) * 0.033333335;
				pParty->pPlayers[pCastSpell->uPlayerID_2].DiscardConditionIfLastsLongerThan(
				0xEu,
				(signed __int64)((double)(signed __int64)pParty->uTimePlayed - *(float *)&a2));
				pParty->pPlayers[pCastSpell->uPlayerID_2].DiscardConditionIfLastsLongerThan(
				0xDu,
				(signed __int64)((double)(signed __int64)pParty->uTimePlayed - *(float *)&a2));
			}
			v377 = &pParty->pPlayers[pCastSpell->uPlayerID_2];
			v377->SetCondition(1, 0);
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_SPIRIT_SHARED_LIFE:
		{
			if ( v731 == 4 )
				amount = 4 * v2;
			else
				amount = 3 * v2;
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			v381 = 0;
			signed int _v733 = amount;
			v730 = 0;
			v382 = 1;
			do
			{
				v383 = pPlayers[v382];
				if ( !v383->pConditions[14] && !v383->pConditions[15] && !v383->pConditions[16] )
				v682[v381++] = v382;
				++v382;
			}
			while ( v382 <= 4 );
			v384 = 0;
			v730 = v381;
			if ( v381 > 0 )
			{
				do
					_v733 += pPlayers[v682[v384++]]->sHealth;
				while ( v384 < v381 );
			}
			v732 = (signed __int64)((double)_v733 / (double)v730);
			_v733 = 0;
			if ( v381 > 0 )
			{
				do
				{
					//v385 = (ItemGen **)&pPlayers[v682[HIDWORD(v733)]];
					v726 = pPlayers[v682[_v733]];
					v726->sHealth = v732;
					//v386 = v726->GetMaxHealth();
					if ( v726->sHealth > v726->GetMaxHealth())
						v726->sHealth = v726->GetMaxHealth();
					if ( v726->sHealth > 0 )
					{
						v726->SetUnconcious(0);
					}
					v388 = _v733;

					pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, LOWORD(v682[_v733]) - 1);
					_v733 = v388 + 1;
				}
				while ( v388 + 1 < v730 );
			}
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_SPIRIT_RESSURECTION:
		{
			switch (v731)
			{
				case 1: amount = 180 * v2; break;
				case 2: amount = 10800 * v2; break;
				case 3: amount = 259200 * v2; break;
				case 4: amount = 0; break;
				default:
				assert(false);
			}
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			v396 = pCastSpell->uPlayerID_2;
			if ( HIDWORD(pParty->pPlayers[v396].pConditions[16]) | LODWORD(pParty->pPlayers[v396].pConditions[16])
				|| HIDWORD(pParty->pPlayers[v396].pConditions[14]) | LODWORD(pParty->pPlayers[v396].pConditions[14]) )
			{
				if ( !(HIDWORD(pParty->pPlayers[v396].pConditions[1]) | LODWORD(pParty->pPlayers[v396].pConditions[1])) )
					pParty->pPlayers[v396].PlaySound(SPEECH_25, 0);
				if ( v731 == 4 )
				{
					v397 = pCastSpell->uPlayerID_2;
					LODWORD(pParty->pPlayers[v397].pConditions[16]) = 0;
					HIDWORD(pParty->pPlayers[v397].pConditions[16]) = 0;
					v398 = pCastSpell->uPlayerID_2;
					LODWORD(pParty->pPlayers[v398].pConditions[14]) = 0;
					HIDWORD(pParty->pPlayers[v398].pConditions[14]) = 0;
					v399 = pCastSpell->uPlayerID_2;
					LODWORD(pParty->pPlayers[v399].pConditions[13]) = 0;
					HIDWORD(pParty->pPlayers[v399].pConditions[13]) = 0;
				}
				else
				{
					*(float *)&a2 = (double)(amount << 7) * 0.033333335;
					pParty->pPlayers[pCastSpell->uPlayerID_2].DiscardConditionIfLastsLongerThan( 0x10u,
						(signed __int64)((double)(signed __int64)pParty->uTimePlayed - *(float *)&a2));
					pParty->pPlayers[pCastSpell->uPlayerID_2].DiscardConditionIfLastsLongerThan( 0xEu,
						(signed __int64)((double)(signed __int64)pParty->uTimePlayed - *(float *)&a2));
					pParty->pPlayers[pCastSpell->uPlayerID_2].DiscardConditionIfLastsLongerThan( 0xDu,
						(signed __int64)((double)(signed __int64)pParty->uTimePlayed - *(float *)&a2));
				}
				pParty->pPlayers[pCastSpell->uPlayerID_2].SetCondition(1u, 1);
				pParty->pPlayers[pCastSpell->uPlayerID_2].sHealth = 1;
				pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pCastSpell->uPlayerID_2);
			}
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_MIND_CURE_PARALYSIS:
		{
			switch (v731)
			{
				case 1: amount = 3600 * v2; break;
				case 2: amount = 3600 * v2; break;
				case 3: amount = 86400 * v2; break;
				case 4: amount = 0; break;
				default:
				assert(false);
			}
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;

			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pCastSpell->uPlayerID_2);
			v323 = pCastSpell->uPlayerID_2;
			v324 = (char *)&pParty->pPlayers[v323].pConditions[12];
			if ( !pParty->pPlayers[v323].pConditions[12] )
			{
				LODWORD(v727) = 1;
				break;
			}
			if ( v731 == 4 )
			{
				*(int *)v324 = 0;
				*((int *)v324 + 1) = 0;
				LODWORD(v727) = 1;
				break;
			}
			v663 = (signed __int64)((double)(signed __int64)pParty->uTimePlayed - (double)(amount << 7) * 0.033333335);
			v656 = 12;
			v325 = &pParty->pPlayers[v323];
			v325->DiscardConditionIfLastsLongerThan(v656, v663);
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_MIND_REMOVE_FEAR:
		{
			switch (v731)
			{
				case 1: amount = 180 * v2; break;
				case 2: amount = 3600 * v2; break;
				case 3: amount = 86400 * v2; break;
				case 4: amount = 0; break;
				default:
				assert(false);
			}

			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;

			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pCastSpell->uPlayerID_2);
			v323 = pCastSpell->uPlayerID_2;
			v324 = (char *)&pParty->pPlayers[v323].pConditions[3];
			if ( !pParty->pPlayers[v323].pConditions[3] )
			{
				LODWORD(v727) = 1;
				break;
			}
			if ( v731 == 4 )
			{
				*(int *)v324 = 0;
				*((int *)v324 + 1) = 0;
				LODWORD(v727) = 1;
				break;
			}
			v663 = (signed __int64)((double)(signed __int64)pParty->uTimePlayed - (double)(amount << 7) * 0.033333335);
			v656 = 3;
			v325 = &pParty->pPlayers[v323];
			v325->DiscardConditionIfLastsLongerThan(v656, v663);
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_MIND_TELEPATHY:
		{
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			if (PID_TYPE(a2) != OBJECT_Actor)
			{
				LODWORD(v727) = 1;
				break;
			}
			v417 = &pActors[PID_ID(a2)];
			Actor * _v730 = v417;
			if ( !(BYTE2(v417->uAttributes) & 0x80) )
			{
				v417->SetRandomGoldIfTheresNoItem();
				v417 = _v730;
			}
			v418 = v417->array_000234[3].uItemID;
			signed int _v733 = 0;
			if ( pItemsTable->pItems[v418].uEquipType == 18 )
				_v733 = v417->array_000234[3].uSpecEnchantmentType;

			//ItemGen::ItemGen(&v683);
			v683.Reset();

			v419 = _v730->uCarriedItemID;
			if (v419)
			{
				v683.uItemID = v419;
				//goto LABEL_799;
			}
			else
			{
				v420 = 0;
				v421 = _v730->array_000234;
				while ( !v421->uItemID || v421->GetItemEquipType() == 18 )
				{
					++v420;
					++v421;
					if ( v420 >= 4 )
						break;
				}
				if ( v420 < 4 )
				{
					memcpy(&v683, &_v730->array_000234[v420], sizeof(v683));
					v2 = v723;
					//v1 = 0;
				}
			}
//	LABEL_799:
			if ( _v733 != 0 )
			{
				v675 = _v733;
				if (v683.uItemID)
				{
					v422 = v683.GetDisplayName();
					sprintf(pTmpBuf2.data(), "(%s), and %d gold", v422, v675);

				}
				else
				{
					v664 = "%d gold";
					sprintf(pTmpBuf2.data(), v664, v675);
				}
			}
			else
			{
				if (v683.uItemID)
				{
					const char *_v675 = v683.GetDisplayName();
					v664 = "(%s)";
					sprintf(pTmpBuf2.data(), v664, _v675);
				}
				else
				{
					strcpy(pTmpBuf2.data(), "nothing");
					ShowStatusBarString(pTmpBuf2.data(), 2u);
				}
			}
			ShowStatusBarString(pTmpBuf2.data(), 2u);
			pSpellSprite.stru_24.Reset();
			pSpellSprite.spell_id = pCastSpell->uSpellID;
			pSpellSprite.spell_level = v2;
			pSpellSprite.spell_skill = v731;
			pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
			pSpellSprite.vPosition.x = _v730->vPosition.x;
			pSpellSprite.vPosition.y = _v730->vPosition.y;
			v676 = _v730->uActorHeight;
			v665 = pSpellSprite.vPosition.y;
			pSpellSprite.vPosition.z = _v730->uActorHeight;
			v657 = pSpellSprite.vPosition.x;
			pSpellSprite.uAttributes = 0;
			pSpellSprite.uSectorID = pIndoor->GetSector(v657, v665, v676);
			pSpellSprite.uSpriteFrameID = 0;
			pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
			pSpellSprite.spell_target_pid = a2;
			pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
			pSpellSprite.uFacing = LOWORD(v715.uYawAngle);
			LOBYTE(pSpellSprite.uAttributes) |= 0x80u;
			pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
			pSpellSprite.Create(0, 0, 0, pCastSpell->uPlayerID + 1);
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_MIND_BERSERK:
		{
			switch (v731)
			{
				case 1: amount = 300 * v2; break;
				case 2: amount = 300 * v2; break;
				case 3: amount = 600 * v2; break;
				case 4: amount = 3600; break;
				default:
				assert(false);
			}
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			v426 = PID_ID(a2);
			if (PID_TYPE(a2) != OBJECT_Actor)
			{
				LODWORD(v727) = 1;
				break;
			}
			v730 = 836 * v426;
			if ( stru_50C198.GetMagicalResistance(&pActors[v426], 7u) )
			{
				pActors[v426].pActorBuffs[ACTOR_BUFF_CHARM].Reset();
				pActors[v426].pActorBuffs[ACTOR_BUFF_ENSLAVED].Reset();
				pActors[v426].pActorBuffs[ACTOR_BUFF_BERSERK].Apply(
				pParty->uTimePlayed + (signed __int64)((double)(amount << 7) * 0.033333335),
				v731, 0, 0, 0);
				pActors[v426].pMonsterInfo.uHostilityType = MonsterInfo::Hostility_Long;
			}
			pSpellSprite.stru_24.Reset();
			pSpellSprite.spell_id = pCastSpell->uSpellID;
			pSpellSprite.spell_level = v2;
			pSpellSprite.spell_skill = v731;
			v60 = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
			v61 = v426;
			v600 = pActors[v61].vPosition.y;
			v601 = pActors[v61].vPosition.x;
			pSpellSprite.uObjectDescID = v60;
			pSpellSprite.vPosition.x = v601;
			v602 = pActors[v61].uActorHeight;
			v603 = pActors[v61].vPosition.z;
			pSpellSprite.vPosition.y = v600;
			v676 = v603 + v602;
			v665 = v600;
			pSpellSprite.vPosition.z = v603 + v602;
			v657 = v601;
			pSpellSprite.uAttributes = 0;
			pSpellSprite.uSectorID = pIndoor->GetSector(v657, v665, v676);
			pSpellSprite.uSpriteFrameID = 0;
			pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
			pSpellSprite.spell_target_pid = a2;
			pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
			pSpellSprite.uFacing = LOWORD(v715.uYawAngle);
			LOBYTE(pSpellSprite.uAttributes) |= 0x80u;
			pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
			pSpellSprite.Create(0, 0, 0, pCastSpell->uPlayerID + 1);
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_MIND_ENSLAVE:
		{
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			amount = 600 * v2;
			if (PID_TYPE(a2) != OBJECT_Actor)
			{
				LODWORD(v727) = 1;
				break;
			}
			v730 = 836 * PID_ID(a2);
			if ( MonsterStats::BelongsToSupertype(pActors[PID_ID(a2)].pMonsterInfo.uID, MONSTER_SUPERTYPE_UNDEAD) )
				break;
			if ( stru_50C198.GetMagicalResistance(&pActors[PID_ID(a2)], 7u) )
			{
				pActors[PID_ID(a2)].pActorBuffs[ACTOR_BUFF_BERSERK].Reset();
				pActors[PID_ID(a2)].pActorBuffs[ACTOR_BUFF_CHARM].Reset();
				pActors[PID_ID(a2)].pActorBuffs[ACTOR_BUFF_ENSLAVED].Apply(pParty->uTimePlayed + (signed __int64)((double)(amount << 7) * 0.033333335),
				v731, 0, 0, 0);
			}
			pSpellSprite.stru_24.Reset();
			pSpellSprite.spell_id = pCastSpell->uSpellID;
			pSpellSprite.spell_level = v2;
			pSpellSprite.spell_skill = v731;
			v60 = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
			v61 = PID_ID(a2);
			v600 = pActors[v61].vPosition.y;
			v601 = pActors[v61].vPosition.x;
			pSpellSprite.uObjectDescID = v60;
			pSpellSprite.vPosition.x = v601;
			v602 = pActors[v61].uActorHeight;
			v603 = pActors[v61].vPosition.z;
			pSpellSprite.vPosition.y = v600;
			v676 = v603 + v602;
			v665 = v600;
			pSpellSprite.vPosition.z = v603 + v602;
			v657 = v601;
			pSpellSprite.uAttributes = 0;
			pSpellSprite.uSectorID = pIndoor->GetSector(v657, v665, v676);
			pSpellSprite.uSpriteFrameID = 0;
			pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
			pSpellSprite.spell_target_pid = a2;
			pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
			pSpellSprite.uFacing = LOWORD(v715.uYawAngle);
			LOBYTE(pSpellSprite.uAttributes) |= 0x80u;
			pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
			pSpellSprite.Create(0, 0, 0, pCastSpell->uPlayerID + 1);
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_MIND_MASS_FEAR:
		{
			if ( v731 == 4 )
				amount = 300 * v2;
			else
				amount = 180 * v2;
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			int _v726 = _46A6AC_spell_render((int)dword_50BF30.data(), 100, 4096);
			pGame->GetStru6()->FadeScreen__like_Turn_Undead_and_mb_Armageddon(0xA0A0Au, 0xC0u);
			++pSpellSprite.uType;
			pSpellSprite.stru_24.Reset();
			pSpellSprite.spell_id = pCastSpell->uSpellID;
			pSpellSprite.spell_level = v2;
			pSpellSprite.spell_skill = v731;
			pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
			pSpellSprite.uAttributes = 0;
			pSpellSprite.uSectorID = 0;
			pSpellSprite.uSpriteFrameID = 0;
			pSpellSprite.field_60_distance_related_prolly_lod = 0;
			pSpellSprite.uFacing = 0;
			pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
			pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
			for ( a2 = 0; a2 < (signed int)_v726; ++a2 )
			{
				v433 = &pActors[dword_50BF30[a2]];
				if ( MonsterStats::BelongsToSupertype(v433->pMonsterInfo.uID, MONSTER_SUPERTYPE_UNDEAD) )
					break;
				pSpellSprite.vPosition.x = v433->vPosition.x;
				pSpellSprite.vPosition.y = v433->vPosition.y;
				pSpellSprite.vPosition.z = v433->vPosition.z - (unsigned int)(signed __int64)((double)v433->uActorHeight * unk_4D8548);

				pSpellSprite.spell_target_pid = PID(OBJECT_Actor, dword_50BF30[a2]);
				pSpellSprite.Create(0, 0, 0, 0);
				if ( stru_50C198.GetMagicalResistance(v433, 7u) )
				{
					v433->pActorBuffs[ACTOR_BUFF_AFRAID].Apply(pParty->uTimePlayed + (signed __int64)((double)(amount << 7) * 0.033333335),
						v731, 0, 0, 0);
				}
			}
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_MIND_CURE_INSANITY:
		{
			v435 = v731 - 2;
			if ( v731 == 4 )
				amount = 0;
			else
				amount = 86400 * v2;
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;

			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pCastSpell->uPlayerID_2);
			v440 = pCastSpell->uPlayerID_2;
			if ( HIDWORD(pParty->pPlayers[v440].pConditions[5]) | LODWORD(pParty->pPlayers[v440].pConditions[5]) )
			{
				if ( !(HIDWORD(pParty->pPlayers[v440].pConditions[1]) | LODWORD(pParty->pPlayers[v440].pConditions[1])) )
					pParty->pPlayers[v440].PlaySound(SPEECH_25, 0);
				if ( v731 == 4 )
				{
					v441 = pCastSpell->uPlayerID_2;
					LODWORD(pParty->pPlayers[v441].pConditions[5]) = 0;
					HIDWORD(pParty->pPlayers[v441].pConditions[5]) = 0;
				}
				else
				{
					pParty->pPlayers[pCastSpell->uPlayerID_2].DiscardConditionIfLastsLongerThan(5u,
						(signed __int64)((double)(signed __int64)pParty->uTimePlayed - (double)(amount << 7) * 0.033333335));
				}
				v377 = &pParty->pPlayers[pCastSpell->uPlayerID_2];
				v377->SetCondition(1, 0);
			}
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_EARTH_TELEKINESIS:
		{
			switch (v731)
			{
				case 1: amount = 2 * v2; break;
				case 2: amount = 2 * v2; break;
				case 3: amount = 3 * v2; break;
				case 4: amount = 4 * v2; break;
				default:
				assert(false);
			}
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			v445 = PID_ID(a2);
			if (PID_TYPE(a2) == OBJECT_Item)
			{
				v449 = (char *)&pSpriteObjects[v445].stru_24;
				v450 = *(int *)v449;
				if ( pItemsTable->pItems[v450].uEquipType == 18 )
				{
					pParty->PartyFindsGold(*((int *)v449 + 3), 0);
					viewparams->bRedrawGameUI = 1;
				}
				else
				{
					sprintfex(pTmpBuf2.data(), pGlobalTXT_LocalizationStrings[471], pItemsTable->pItems[v450].pUnidentifiedName);
					ShowStatusBarString(pTmpBuf2.data(), 2u);
					if ( !pParty->AddItemToParty(&pSpriteObjects[v445].stru_24) )
						pParty->SetHoldingItem(&pSpriteObjects[v445].stru_24);
				}
				SpriteObject::OnInteraction(v445);
			}
			else
			{
				if (PID_TYPE(a2) == OBJECT_Actor)
				{
					pActors[v445].LootActor();
				}
				else
				{
					if (PID_TYPE(a2) != OBJECT_Decoration)
					{
						if (PID_TYPE(a2) != OBJECT_BModel)
						{
							LODWORD(v727) = 1;
							break;
						}
						OpenedTelekinesis = true;
						if ( uCurrentlyLoadedLevelType == LEVEL_Indoor)
							v448 = pIndoor->pFaceExtras[pIndoor->pFaces[v445].uFaceExtraID].uEventID;
						else
							v448 = pOutdoor->pBModels[a2 >> 9].pFaces[v445 & 0x3F].sCogTriggeredID;
						EventProcessor(v448, a2, 1);
						LODWORD(v727) = 1;
						break;
					}
					OpenedTelekinesis = true;
					if ( pLevelDecorations[v445].uEventID )
					{
						EventProcessor(v448, a2, 1);
						LODWORD(v727) = 1;
						break;
					}
					if ( pLevelDecorations[v445].IsInteractive() )
					{
						activeLevelDecoration = &pLevelDecorations[v445];
						EventProcessor(stru_5E4C90_MapPersistVars._decor_events[pLevelDecorations[v445]._idx_in_stru123 - 75] + 380, 0, 1);
						activeLevelDecoration = NULL;
					}
				}
			}
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_BODY_CURE_WEAKNESS:
		{
			switch (v731)
			{
				case 1: amount = 180 * v2; break;
				case 2: amount = 3600 * v2; break;
				case 3: amount = 86400 * v2; break;
				case 4: amount = 0; break;
				default:
				assert(false);
			}
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;

			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pCastSpell->uPlayerID_2);
			v323 = pCastSpell->uPlayerID_2;
			v324 = (char *)&pParty->pPlayers[v323].pConditions[1];
			if ( !pParty->pPlayers[v323].pConditions[1] )
			{
				LODWORD(v727) = 1;
				break;
			}
			if ( v731 == 4 )
			{
				*(int *)v324 = 0;
				*((int *)v324 + 1) = 0;
				LODWORD(v727) = 1;
				break;
			}
			v663 = (signed __int64)((double)(signed __int64)pParty->uTimePlayed - (double)(amount << 7) * 0.033333335);
			v656 = 1;
			v325 = &pParty->pPlayers[v323];
			v325->DiscardConditionIfLastsLongerThan(v656, v663);
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_BODY_FIRST_AID:
		{
			switch (v731)
			{
				case 1: amount = 2 * v2 + 5; break;
				case 2: amount = 3 * v2 + 5; break;
				case 3: amount = 4 * v2 + 5; break;
				case 4: amount = 5 * v2 + 5; break;
				default:
				assert(false);
			}
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			v460 = pCastSpell->spell_target_pid;
			if (!v460)
			{
				pParty->pPlayers[pCastSpell->uPlayerID_2].Heal(amount);
				pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pCastSpell->uPlayerID_2);
			}
			else
			{
				if (PID_TYPE(v460) == OBJECT_Actor)
				{
					v461 = &pActors[PID_ID(v460)];
					v462 = v461->uAIState;
					v463 = v461->pMonsterInfo.uHP;
					if ( v462 != 5 )
					{
						if ( v462 != 4 )
						{
							if ( v462 != 19 )
							{
								if ( v462 != 11 )
								{
									v461->sCurrentHP += amount;
									if ( v461->sCurrentHP > v463 )
										v461->sCurrentHP = v463;
								}
							}
						}
					}
				}
			}
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_BODY_CURE_POISON:
		{
			switch (v731)
			{
				case 1: amount = 3600 * v2; break;
				case 2: amount = 3600 * v2; break;
				case 3: amount = 86400 * v2; break;
				case 4: amount = 0; break;
				default:
				assert(false);
			}
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;

			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pCastSpell->uPlayerID_2);
			v470 = pCastSpell->uPlayerID_2;
			if ( !(HIDWORD(pParty->pPlayers[v470].pConditions[6]) | LODWORD(pParty->pPlayers[v470].pConditions[6]))
				&& !(HIDWORD(pParty->pPlayers[v470].pConditions[8]) | LODWORD(pParty->pPlayers[v470].pConditions[8]))
				&& !(HIDWORD(pParty->pPlayers[v470].pConditions[10]) | LODWORD(pParty->pPlayers[v470].pConditions[10])) )
			{
				LODWORD(v727) = 1;
				break;
			}
			if ( v731 == 4 )
			{
				LODWORD(pParty->pPlayers[v470].pConditions[6]) = 0;
				HIDWORD(pParty->pPlayers[v470].pConditions[6]) = 0;
				v471 = pCastSpell->uPlayerID_2;
				LODWORD(pParty->pPlayers[v471].pConditions[8]) = 0;
				HIDWORD(pParty->pPlayers[v471].pConditions[8]) = 0;
				v472 = pCastSpell->uPlayerID_2;
				LODWORD(pParty->pPlayers[v472].pConditions[10]) = 0;
				HIDWORD(pParty->pPlayers[v472].pConditions[10]) = 0;
				LODWORD(v727) = 1;
				break;
			}
			*(float *)&a2 = (double)(amount << 7) * 0.033333335;
			pParty->pPlayers[v470].DiscardConditionIfLastsLongerThan(
				6u,
				(signed __int64)((double)(signed __int64)pParty->uTimePlayed - *(float *)&a2));
			pParty->pPlayers[pCastSpell->uPlayerID_2].DiscardConditionIfLastsLongerThan(
				8u,
				(signed __int64)((double)(signed __int64)pParty->uTimePlayed - *(float *)&a2));
			v663 = (signed __int64)((double)(signed __int64)pParty->uTimePlayed - *(float *)&a2);
			v656 = 10;
			v325 = &pParty->pPlayers[pCastSpell->uPlayerID_2];
			v325->DiscardConditionIfLastsLongerThan(v656, v663);
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_BODY_PROTECTION_FROM_MAGIC:
		{
			amount = v2;
			LODWORD(v733) = 3600 * v2;
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;

			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 0);
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 1);
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 2);
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 3);

			pParty->pPartyBuffs[PARTY_BUFF_PROTECTION_FROM_MAGIC].Apply(pParty->uTimePlayed + (signed __int64)((double)(signed int)((int)v733 << 7) * 0.033333335), v731, v2, 0, 0);
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_BODY_HAMMERHANDS:
		{
			LODWORD(v733) = 3600 * v2;
			amount = v2;
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			if ( v731 == 4 )
			{
				pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 0);
				pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 1);
				pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 2);
				pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 3);
				v732 = (int)v733 << 7;
				v717 = (signed int)(signed __int64)((double)(signed int)((int)v733 << 7) * 0.033333335);
				pParty->pPlayers[0].pPlayerBuffs[PLAYER_BUFF_HAMMERHANDS].Apply(pParty->uTimePlayed + v717, 4u, v2, v2, 0);
				pParty->pPlayers[1].pPlayerBuffs[PLAYER_BUFF_HAMMERHANDS].Apply(pParty->uTimePlayed + v717, 4u, v2, v2, 0);
				pParty->pPlayers[2].pPlayerBuffs[PLAYER_BUFF_HAMMERHANDS].Apply(pParty->uTimePlayed + v717, 4u, v2, v2, 0);

				pParty->pPlayers[3].pPlayerBuffs[PLAYER_BUFF_HAMMERHANDS].Apply(pParty->uTimePlayed + v717, 4, v2, v2, 0);
				LODWORD(v727) = 1;
				break;
			}
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pCastSpell->uPlayerID_2);

			pParty->pPlayers[pCastSpell->uPlayerID_2].pPlayerBuffs[PLAYER_BUFF_HAMMERHANDS].Apply(pParty->uTimePlayed + (signed int)(signed __int64)((double)(signed int)((int)v733 << 7) * 0.033333335), v731, v2, v2, 0);
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_BODY_CURE_DISEASE:
		{
			if ( v731 == 4 )
				amount = 0;
			else
				amount = 86400 * v2;
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;

			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pCastSpell->uPlayerID_2);
			v498 = pCastSpell->uPlayerID_2;
			if ( !(HIDWORD(pParty->pPlayers[v498].pConditions[7]) | LODWORD(pParty->pPlayers[v498].pConditions[7]))
				&& !(HIDWORD(pParty->pPlayers[v498].pConditions[9]) | LODWORD(pParty->pPlayers[v498].pConditions[9]))
				&& !(HIDWORD(pParty->pPlayers[v498].pConditions[11]) | LODWORD(pParty->pPlayers[v498].pConditions[11])) )
			{
				LODWORD(v727) = 1;
				break;
			}
			if ( v731 == 4 )
			{
				LODWORD(pParty->pPlayers[v498].pConditions[7]) = 0;
				HIDWORD(pParty->pPlayers[v498].pConditions[7]) = 0;
				v499 = pCastSpell->uPlayerID_2;
				LODWORD(pParty->pPlayers[v499].pConditions[9]) = 0;
				HIDWORD(pParty->pPlayers[v499].pConditions[9]) = 0;
				v500 = pCastSpell->uPlayerID_2;
				LODWORD(pParty->pPlayers[v500].pConditions[11]) = 0;
				HIDWORD(pParty->pPlayers[v500].pConditions[11]) = 0;
			}
			else
			{
				*(float *)&a2 = (double)(amount << 7) * 0.033333335;
				pParty->pPlayers[v498].DiscardConditionIfLastsLongerThan(
				7u,
				(signed __int64)((double)(signed __int64)pParty->uTimePlayed - *(float *)&a2));
				pParty->pPlayers[pCastSpell->uPlayerID_2].DiscardConditionIfLastsLongerThan(
				9u,
				(signed __int64)((double)(signed __int64)pParty->uTimePlayed - *(float *)&a2));
				v663 = (signed __int64)((double)(signed __int64)pParty->uTimePlayed - *(float *)&a2);
				v656 = 11;
				v325 = &pParty->pPlayers[pCastSpell->uPlayerID_2];
				v325->DiscardConditionIfLastsLongerThan(v656, v663);
			}
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_BODY_POWER_CURE:
		{
			amount = 5 * v2 + 10;
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			v501 = pParty->pPlayers.data();
			int v1 = 0;
			do
			{
				pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, v1);
				v501->Heal(amount);
				++v501;
				++v1;
			}
			while ( (signed int)v501 < (signed int)pParty->pHirelings.data() );
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_LIGHT_DISPEL_MAGIC:
		{
			sRecoveryTime -= v2;
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;

			pGame->GetStru6()->FadeScreen__like_Turn_Undead_and_mb_Armageddon(0xAFF0Au, 0xC0u);
			v505 = _46A6AC_spell_render((int)dword_50BF30.data(), 100, 4096);
			++pSpellSprite.uType;
			//signed int _v733 = v505;
			v688.x = 0;
			v688.y = 0;
			v688.z = 0;
			pSpellSprite.stru_24.Reset();
			pSpellSprite.spell_id = pCastSpell->uSpellID;
			pSpellSprite.spell_level = v2;
			pSpellSprite.spell_skill = v731;
			pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
			pSpellSprite.uAttributes = 0;
			pSpellSprite.uSectorID = 0;
			pSpellSprite.uSpriteFrameID = 0;
			pSpellSprite.field_60_distance_related_prolly_lod = 0;
			pSpellSprite.uFacing = 0;
			pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
			pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
			a2 = 0;
			for ( a2 = 0; a2 < v505; ++a2 )
			{
				v507 = dword_50BF30[a2];
				pSpellSprite.vPosition.x = pActors[v507].vPosition.x;
				pSpellSprite.vPosition.y = pActors[v507].vPosition.y;
				pSpellSprite.vPosition.z = pActors[v507].vPosition.z - (unsigned int)(signed __int64)((double)pActors[v507].uActorHeight * unk_4D8548);
				pSpellSprite.spell_target_pid = PID(OBJECT_Actor, dword_50BF30[a2]);
				v509 = pSpellSprite.Create(0, 0, 0, 0);
				DamageMonsterFromParty(PID(OBJECT_Item, v509), dword_50BF30[a2], &v688);
			}
			for ( a2 = 0; a2 < v505; ++a2 )
			{
				pActor = &pActors[dword_50BF30[a2]];
				pSpellSprite.vPosition.x = pActor->vPosition.x;
				pSpellSprite.vPosition.y = pActor->vPosition.y;
				pSpellSprite.vPosition.z = pActor->vPosition.z - (unsigned int)(signed __int64)((double)pActor->uActorHeight * unk_4D8548);
				pSpellSprite.spell_target_pid = PID(OBJECT_Actor, dword_50BF30[a2]);
				pSpellSprite.Create(0, 0, 0, 0);
				for (int i = 0; i < 22; ++i)
				{
					pActor->pActorBuffs[i].Reset();
				}
			}
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_LIGHT_SUMMON_ELEMENTAL:
		{
			switch (v731)
			{
				case 1: v733 = 300 * v2; amount = 1; break;
				case 2: v733 = 300 * v2; amount = 1; break;
				case 3: v733 = 900 * v2; amount = 3; break;
				case 4: v733 = 900 * v2; amount = 5; break;
				default:
				assert(false);
			}
			//v733 = __PAIR__(0, v516);
			signed int _v733 = 0;
			if ( (signed int)uNumActors > 0 )
			{
				v518 = pActors.data();//[0].uAIState;
				size_t _v726 = uNumActors;
				do
				{
					v519 = v518->uAIState;
					if ( v518->uAIState != 5 && v519 != 11 && v519 != 19 && PID(OBJECT_Player, pCastSpell->uPlayerID) == v518->uSummonerID )
						++_v733;
					++v518;
					--_v726;
				}
				while ( _v726 != 0 );
			}
			if ( _v733 >= amount )
			{
				ShowStatusBarString(pGlobalTXT_LocalizationStrings[648], 2);  // This character can't summon any more monsters!
				pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
				pCastSpell->uSpellID = 0;
				continue;
			}
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			sub_44FA4C_spawn_light_elemental(pCastSpell->uPlayerID, v731, v733);
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_LIGHT_DAY_OF_THE_GODS:
		{
			switch (v731)
			{
				case 1: LODWORD(v733) = 10800 * v2; amount = 3 * v2 + 10; break;
				case 2: LODWORD(v733) = 10800 * v2; amount = 3 * v2 + 10; break;
				case 3: LODWORD(v733) = 14400 * v2; amount = 4 * v2 + 10; break;
				case 4: LODWORD(v733) = 18000 * v2; amount = 5 * v2 + 10; break;
				default:
				assert(false);
			}
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;

			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 0);
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 1);
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 2);
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 3);

			pParty->pPartyBuffs[PARTY_BUFF_DAY_OF_GODS].Apply(pParty->uTimePlayed + (signed __int64)((double)(signed int)((int)v733 << 7) * 0.033333335), v731, amount, 0, 0);
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_LIGHT_PRISMATIC_LIGHT:
		{
			//v67 = 2;
			if ( uCurrentlyLoadedLevelType == LEVEL_Outdoor)
			{
				ShowStatusBarString(pGlobalTXT_LocalizationStrings[497], 2);  // Can't cast Prismatic Light outdoors!
				pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
				pCastSpell->uSpellID = 0;
				continue;
			}
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			v531 = _46A6AC_spell_render((int)dword_50BF30.data(), 100, 4096);
			++pSpellSprite.uType;
			v694.x = 0;
			v694.y = 0;
			v694.z = 0;
			pSpellSprite.stru_24.Reset();
			pSpellSprite.spell_id = pCastSpell->uSpellID;
			pSpellSprite.spell_level = v2;
			pSpellSprite.spell_skill = v731;
			pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
			pSpellSprite.uAttributes = 0;
			pSpellSprite.uSectorID = 0;
			pSpellSprite.uSpriteFrameID = 0;
			pSpellSprite.field_60_distance_related_prolly_lod = 0;
			pSpellSprite.uFacing = 0;
			pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
			pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
			a2 = 0;
			if ( (signed int)v531 > 0 )
			{
				do
				{
					v533 = dword_50BF30[a2];
					pSpellSprite.vPosition.x = pActors[v533].vPosition.x;
					pSpellSprite.vPosition.y = pActors[v533].vPosition.y;
					pSpellSprite.vPosition.z = pActors[v533].vPosition.z - (unsigned int)(signed __int64)((double)pActors[v533].uActorHeight * unk_4D8548);
					pSpellSprite.spell_target_pid = PID(OBJECT_Actor, dword_50BF30[a2]);
					v535 = pSpellSprite.Create(0, 0, 0, 0);
					v536 = a2;
					DamageMonsterFromParty(PID(OBJECT_Item, v535), dword_50BF30[a2], &v694);
					a2 = v536 + 1;
				}
				while ( v536 + 1 < (signed int)v531 );
			}
			v537 = pGame->GetStru6();
			pGame->GetStru6()->_4A8BFC();
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_LIGHT_DAY_OF_PROTECTION:
		{
			switch (v731)
			{
				case 1: LODWORD(v733) = 14400 * v2; amount = 4 * v2; break;
				case 2: LODWORD(v733) = 14400 * v2; amount = 4 * v2; break;
				case 3: LODWORD(v733) = 14400 * v2; amount = 4 * v2; break;
				case 4: LODWORD(v733) = 18000 * v2; amount = 5 * v2; break;
				default:
				assert(false);
			}
			v730 = LODWORD(v733);
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;

			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 0);
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 1);
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 2);
			pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 3);

			v732 = v730 << 7;
			v549 = (double)(v730 << 7) * 0.033333335;
			*((float *)&v733 + 1) = v549;
			v712 = (signed __int64)v549;
			pParty->pPartyBuffs[PARTY_BUFF_RESIST_BODY].Apply(pParty->uTimePlayed + (signed __int64)v549, v731, amount, 0, 0);
			pParty->pPartyBuffs[PARTY_BUFF_RESIST_MIND].Apply(pParty->uTimePlayed + v712, v731, amount, 0, 0);
			pParty->pPartyBuffs[PARTY_BUFF_RESIST_FIRE].Apply(pParty->uTimePlayed + v712, v731, amount, 0, 0);
			pParty->pPartyBuffs[PARTY_BUFF_RESIST_WATER].Apply(pParty->uTimePlayed + v712, v731, amount, 0, 0);
			pParty->pPartyBuffs[PARTY_BUFF_RESIST_AIR].Apply(pParty->uTimePlayed + v712, v731, amount, 0, 0);
			pParty->pPartyBuffs[PARTY_BUFF_RESIST_EARTH].Apply(pParty->uTimePlayed + v712, v731, amount, 0, 0);
			v550 = v2 + 5;
			pParty->pPartyBuffs[PARTY_BUFF_FEATHER_FALL].Apply(
				(signed __int64)((double)(signed __int64)pParty->uTimePlayed + *((float *)&v733 + 1)),
				v731,
				v550, 0, 0);

			pParty->pPartyBuffs[PARTY_BUFF_WIZARD_EYE].Apply((signed __int64)((double)(signed __int64)pParty->uTimePlayed + *((float *)&v733 + 1)), v731, v550, 0, 0);
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_LIGHT_HOUR_OF_POWER:
		{
			switch (v731)
			{
				case 1: LODWORD(v733) = 4; amount = 4; break;
				case 2: LODWORD(v733) = 4; amount = 4; break;
				case 3: LODWORD(v733) = 12; amount = 12; break;
				case 4: LODWORD(v733) = 20; amount = 15; break;
				default:
				assert(false);
			}
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			y = (char *)(60 * (v2 * LODWORD(v733) + 60));
			v732 = (300 * amount * v2 + 60) << 7;
			v730 = v2 + 5;
			int _v726 = 0;
			v553 = pParty->pPlayers.data();//[0].pConditions[1];
			*((float *)&v733) = (double)v732 * 0.033333335;
			do
			{
				pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 0);
				pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 1);
				pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 2);
				pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, 3);

				//((SpellBuff *)(v553 + 6056))->Apply(
				v553->pPlayerBuffs[4].Apply((signed __int64)((double)(signed __int64)pParty->uTimePlayed + *((float *)&v733)), v731, v730, 0, 0);
				if ( *(_QWORD *)v553 )
					_v726 = 1;
				++v553;
			}
			while ( v553 <= &pParty->pPlayers[3] );
			v562 = v731;
			pParty->pPartyBuffs[PARTY_BUFF_HEROISM].Apply((signed __int64)((double)(signed __int64)pParty->uTimePlayed + *((float *)&v733)), v731, v730, 0, 0);
			pParty->pPartyBuffs[PARTY_BUFF_SHIELD].Apply((signed __int64)((double)(signed __int64)pParty->uTimePlayed + *((float *)&v733)), v562, 0, 0, 0);
			pParty->pPartyBuffs[PARTY_BUFF_STONE_SKIN].Apply((signed __int64)((double)(signed __int64)pParty->uTimePlayed + *((float *)&v733)), v562, v730, 0, 0);
			if (!_v726)
			{
				pParty->pPartyBuffs[PARTY_BUFF_HASTE].Apply((signed __int64)((double)(signed int)((int)y << 7) * 0.033333335 + (double)(signed __int64)pParty->uTimePlayed), v562, v730, 0, 0);
			}
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_LIGHT_DIVINE_INTERVENTION:
		{
			amount = 3;
			if ( pPlayer->uNumDivineInterventionCastsThisDay >= 3u )
			{
				ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2); // Spell failed
				pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
				pCastSpell->uSpellID = 0;
				continue;
			}
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			a2 = 0;
			_this = (ItemGen *)&pPlayers[1];
			do
			{
				v563 = 0;
				do
				{
					v564 = _this->uItemID;
					*(int *)(v563 + _this->uItemID) = 0;
					v563 += 8;
					*(int *)(v563 + v564 - 4) = 0;
				}
				while ( v563 <= 128 );
				v565 = ((Player *)_this->uItemID)->GetMaxHealth();
				v566 = (Player **)_this;
				*(int *)(_this->uItemID + 6460) = v565;
				v567 = (*v566)->GetMaxMana();
				*(int *)(_this->uItemID + 6464) = v567;
				pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, a2);
				++a2;
				_this = (ItemGen *)((char *)_this + 4);
			}
			while ( (signed int)_this < (signed int)&qword_A750D8 );
			v571 = pPlayer;
			v572 = (char *)&pPlayer->sAgeModifier;
			if ( pPlayer->sAgeModifier + 10 >= 120 )
				*(short *)v572 = 120;
			else
				*(short *)v572 = pPlayer->sAgeModifier + 10;
			sRecoveryTime += -5 * v2;
			++v571->uNumDivineInterventionCastsThisDay;
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_DARK_REANIMATE:
		{
			switch (v731)
			{
				case 1: amount = 2 * v2; break;
				case 2: amount = 3 * v2; break;
				case 3: amount = 4 * v2; break;
				case 4: amount = 5 * v2; break;
				default:
				assert(false);
			}
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			v576 = pCastSpell->spell_target_pid;
			if (!v576)
			{
				v585 = (char *)&pParty->pPlayers[pCastSpell->uPlayerID_2];
				pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pCastSpell->uPlayerID_2);
				if ( *((_QWORD *)v585 + 14) )
				{
				((Player *)v585)->SetCondition(0x11u, 1);
				v587 = ((Player *)v585)->GetSexByVoice();
				ReloadPlayerPortraits(pCastSpell->uPlayerID_2, (v587 != 0) + 23);
				*((_QWORD *)v585 + 17) = pParty->uTimePlayed;
				}
				break;
			}
			v577 = (Player *)(PID_ID(v576));
			//v726 = v577;
			if ( v577 == (Player *)-1 )
			{
				ShowStatusBarString(pGlobalTXT_LocalizationStrings[496], 2);  // No valid target exists!
				pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
				pCastSpell->uSpellID = 0;
				continue;
			}
			v578 = (int)&pActors[(int)v577];
			v721 = v578;
			if ( *(short *)(v578 + 40) > 0 || (v579 = *(short *)(v578 + 176), v579 != 5) && v579 != 4 )
			{
				ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2); // Spell failed
				pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
				pCastSpell->uSpellID = 0;
				continue;
			}
			++pSpellSprite.uType;
			pSpellSprite.stru_24.Reset();
			pSpellSprite.spell_id = pCastSpell->uSpellID;
			pSpellSprite.spell_level = v2;
			pSpellSprite.spell_skill = v731;
			pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
			pSpellSprite.uAttributes = 0;
			pSpellSprite.uSectorID = 0;
			pSpellSprite.uSpriteFrameID = 0;
			pSpellSprite.field_60_distance_related_prolly_lod = 0;
			v581 = v721;
			pSpellSprite.uFacing = 0;
			pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
			pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
			pSpellSprite.vPosition.x = *(short *)(v721 + 142);
			pSpellSprite.vPosition.y = *(short *)(v721 + 144);
			v732 = *(short *)(v721 + 138);
			pSpellSprite.vPosition.z = *(short *)(v721 + 146) - (unsigned int)(signed __int64)((double)v732 * unk_4D8548);
			pSpellSprite.spell_target_pid = PID(OBJECT_Actor, (int)v577);
			pSpellSprite.Create(0, 0, 0, 0);
			if ( *(char *)(v581 + 52) > amount )
				break;
			Actor::Resurrect((unsigned int)v577);
			*(char *)(v581 + 61) = 0;
			*(char *)(v581 + 53) = 0;
			*(char *)(v581 + 54) = 0;
			*(char *)(v581 + 55) = 0;
			*(char *)(v581 + 56) = 0;
			*(char *)(v581 + 57) = 0;
			*(int *)(v581 + 712) = 9999;
			*(char *)(v581 + 38) &= 0xF7u;
			*(int *)(v581 + 708) = 0;
			((SpellBuff *)(v581 + 356))->Reset();
			((SpellBuff *)(v581 + 228))->Reset();
			((SpellBuff *)(v581 + 404))->Reset();
			if ( *(short *)(v581 + 40) > 10 * amount )
				*(short *)(v581 + 40) = 10 * amount;
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_DARK_VAMPIRIC_WEAPON:
		{
			amount = 16;
			if ( v731 == 4 )
			{
				LODWORD(v733) = 0;
			}
			else
			{
				LODWORD(v733) = 3600 * v2;
			}
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			__debugbreak();
			//HIDWORD(v733) = (int)(char *)&pParty + 6972 * pCastSpell->uPlayerID_2 + 36 * a2 + 3040;
			ItemGen *_v733 = &pParty->pPlayers[pCastSpell->uPlayerID_2].pInventoryItemList[a2];
			ItemDesc *_v732 = &pItemsTable->pItems[_v733->uItemID];
			_v733->UpdateTempBonus(pParty->uTimePlayed);
			if ( _v733->uItemID >= 64 && _v733->uItemID <= 65
				|| LOBYTE(_v733->uAttributes) & 2
				|| _v733->uSpecEnchantmentType != 0
				|| _v733->uEnchantmentType != 0
				|| (v591 = _v732->uEquipType) != 0 && v591 != 1 && v591 != 2
				|| pItemsTable->IsMaterialNonCommon(_v733) )
			{
				_50C9D0_AfterEnchClickEventId = 113;
				_50C9D4_AfterEnchClickEventSecondParam = 0;
				_50C9D8_AfterEnchClickEventTimeout = 1;

				ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2); // Spell failed
				pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
				pCastSpell->uSpellID = 0;
				continue;
			}
			v592 = _v733;
			v14 = v731 == 4;
			_v733->uSpecEnchantmentType = 16;
			if ( !v14 )
			{
				v732 = (int)v733 << 7;
				v592->uExpireTime = pParty->uTimePlayed + (signed int)(signed __int64)((double)(signed int)((int)v733 << 7) * 0.033333335);
				v592->uAttributes |= 8u;
			}
			LOBYTE(v592->uAttributes) |= 0x80u;
			_50C9A8_item_enchantment_timer = 256;
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_DARK_SHARPMETAL:
		{
			switch (v731)
			{
				case 1: amount = 5; break;
				case 2: amount = 5; break;
				case 3: amount = 7; break;
				case 4: amount = 9; break;
				default:
				assert(false);
			}
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			signed int _v726 = ((signed int)(60 * stru_5C6E00->uIntegerDoublePi) / 360);
			pSpellSprite.stru_24.Reset();
			pSpellSprite.spell_id = pCastSpell->uSpellID;
			pSpellSprite.spell_level = v2;
			pSpellSprite.spell_skill = v731;
			pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
			pSpellSprite.vPosition.x = pParty->vPosition.x;
			pSpellSprite.vPosition.y = pParty->vPosition.y;
			pSpellSprite.uAttributes = 0;
			pSpellSprite.vPosition.z = pParty->vPosition.z + (signed int)pParty->uPartyHeight / 2;
			pSpellSprite.uSectorID = pIndoor->GetSector(
								pParty->vPosition.x,
								pParty->vPosition.y,
								pParty->vPosition.z + (signed int)pParty->uPartyHeight / 2);
			pSpellSprite.uSpriteFrameID = 0;
			pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
			pSpellSprite.spell_target_pid = a2;
			pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
			pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
			if ( pParty->bTurnBasedModeOn == 1 )
				LOBYTE(pSpellSprite.uAttributes) |= 4u;
			v596 = (signed int)_v726 / -2;
			y = (char *)((signed int)_v726 / 2);
			if ( (signed int)_v726 / -2 <= (signed int)_v726 / 2 )
			{
				v597 = v715.uYawAngle;
				do
				{
					pSpellSprite.uFacing = v596 + v597;
					if ( pSpellSprite.Create(
							v596 + v597,
							v715.uPitchAngle,
							pObjectList->pObjects[(signed __int16)pSpellSprite.uObjectDescID].uSpeed,
							pCastSpell->uPlayerID + 1) != -1
							&& pParty->bTurnBasedModeOn == 1 )
						++pTurnEngine->pending_actions;
					v596 += _v726 / (amount - 1);
				}
				while ( v596 <= (signed int)y );
			}
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_DARK_CONTROL_UNDEAD:
		{
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			switch (v731)
			{
				case 1: LODWORD(v733) = 180 * v2; break;
				case 2: LODWORD(v733) = 180 * v2; break;
				case 3: LODWORD(v733) = 300 * v2; break;
				case 4: LODWORD(v733) = 29030400; break;
				default:
				assert(false);
			}
			if (PID_TYPE(a2) != OBJECT_Actor)
			{
				LODWORD(v727) = 1;
				break;
			}
			pActor = &pActors[PID_ID(a2)];
			if ( !MonsterStats::BelongsToSupertype(pActor->pMonsterInfo.uID, MONSTER_SUPERTYPE_UNDEAD) )
				break;
			if ( !stru_50C198.GetMagicalResistance(pActor, 0xAu) )
			{
				ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2); // Spell failed
				pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
				pCastSpell->uSpellID = 0;
				continue;
			}
			pActor->pActorBuffs[ACTOR_BUFF_BERSERK].Reset();
			pActor->pActorBuffs[ACTOR_BUFF_CHARM].Reset();
			pActor->pActorBuffs[ACTOR_BUFF_ENSLAVED].Apply(pParty->uTimePlayed + (signed __int64)((double)(signed int)((int)v733 << 7) * 0.033333335),
				v731, 0, 0, 0);
			pSpellSprite.stru_24.Reset();
			pSpellSprite.spell_id = pCastSpell->uSpellID;
			pSpellSprite.spell_level = v2;
			pSpellSprite.spell_skill = v731;
			v60 = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
			
			v600 = pActor->vPosition.y;
			v601 = pActor->vPosition.x;
			pSpellSprite.uObjectDescID = v60;
			pSpellSprite.vPosition.x = v601;
			v602 = pActor->uActorHeight;
			v603 = pActor->vPosition.z;
			pSpellSprite.vPosition.y = v600;
			v676 = v603 + v602;
			v665 = v600;
			pSpellSprite.vPosition.z = v603 + v602;
			v657 = v601;
			pSpellSprite.uAttributes = 0;
			pSpellSprite.uSectorID = pIndoor->GetSector(v657, v665, v676);
			pSpellSprite.uSpriteFrameID = 0;
			pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
			pSpellSprite.spell_target_pid = a2;
			pSpellSprite.field_60_distance_related_prolly_lod = LOBYTE(v715.uDistance);
			pSpellSprite.uFacing = LOWORD(v715.uYawAngle);
			LOBYTE(pSpellSprite.uAttributes) |= 0x80u;
			pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
			pSpellSprite.Create(0, 0, 0, pCastSpell->uPlayerID + 1);
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_DARK_SACRIFICE:
		{
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			int _v733 = 0;
			memset(&achieved_awards, 0, 4000);
			int i = 0;
			pNPCData = pParty->pHirelings.data();
			do
			{
				if ( pNPCData->pName != 0)
				{
					v606 = _v733++;
					achieved_awards[v606] = (AwardType)(i + 1);
				}
				++i;
				++pNPCData;
			}
			while ( pNPCData <= &pParty->pHirelings[1] );
			i = 0;
			if ( (signed int)pNPCStats->uNumNewNPCs > 0)
			{
				pNPCData = pNPCStats->pNewNPCData;
				//__debugbreak(); // data offset
				AwardType *_v734 = &achieved_awards[_v733];
				do
				{
					if ( pNPCData->uFlags & 0x80
						&& (!pParty->pHirelings[0].pName || strcmp(pNPCData->pName, pParty->pHirelings[0].pName))
						&& (!pParty->pHirelings[1].pName || strcmp(pNPCData->pName, pParty->pHirelings[1].pName)) )
					{
						v607 = _v734;
						++_v734;
						*v607 = (AwardType)(i + 3);
					}
					++i;
					++pNPCData;
				}
				while ( (signed int)i < (signed int)pNPCStats->uNumNewNPCs );
			}
			v608 = pCastSpell->uPlayerID_2;
			if ( v608 != 4 && v608 != 5
				|| (v609 = (signed int)*(&pFontCChar + v608 + pParty->hirelingScrollPosition), v609 <= 0)
				|| v609 >= 3 )
			{
				ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2); // Spell failed
				pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
				pCastSpell->uSpellID = 0;
				continue;
			}
			v610 = 76 * v609;
			//*((int *)&pParty->pPlayers[3].pInstalledBeacons[4].uBeaconTime + 19 * v609) = 0;
			pParty->pHirelings[v609-1].evt_B = 0;
			v611 = pIconsFrameTable->FindIcon("spell96");
			//*(int *)((char *)&pParty->pPlayers[3].pInstalledBeacons[4].uBeaconTime + v610 + 4) = pIconsFrameTable->GetIconAnimLength(v611);
			pParty->pHirelings[v609-1].evt_C = pIconsFrameTable->GetIconAnimLength(v611);
			//*(int *)((char *)&pParty->pPlayers[3].pInstalledBeacons[3].field_18 + v610) = 1;
			pParty->pHirelings[v609-1].evt_A = 1;

			v612 = pParty->pPlayers.data();
			do
			{
				v612->sHealth = v612->GetMaxHealth();
				v612->sMana = v612->GetMaxMana();
				++v612;
			}
			while ( v612 <= &pParty->pPlayers[3] );
			v613 = &pOutdoor->ddm;
			if ( uCurrentlyLoadedLevelType != LEVEL_Outdoor)
				v613 = &pIndoor->dlv;
			v613->uReputation += 15;
			if ( v613->uReputation > 10000 )
				v613->uReputation = 10000;
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_DARK_PAIN_REFLECTION:
		{
			switch (v731)
			{
				case 1: LODWORD(v733) = 300 * (v2 + 12); break;
				case 2: LODWORD(v733) = 300 * (v2 + 12); break;
				case 3: LODWORD(v733) = 300 * (v2 + 12); break;
				case 4: LODWORD(v733) = 900 * (v2 + 4); break;
				default:
				assert(false);
			}
			amount = v2 + 5;
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			if ( v731 != 3 && v731 != 4 )
			{
				pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, pCastSpell->uPlayerID_2);
				pParty->pPlayers[pCastSpell->uPlayerID_2].pPlayerBuffs[PLAYER_BUFF_PAIN_REFLECTION].Apply(pParty->uTimePlayed + (signed int)(signed __int64)((double)(signed int)((int)v733 << 7) * 0.033333335), v731, amount, v716, 0);
				LODWORD(v727) = 1;
				break;
			}
			a2 = 0;
			v717 = (signed int)(signed __int64)((double)(signed int)((int)v733 << 7) * 0.033333335);
			v619 = pParty->pPlayers.data();//[0].pPlayerBuffs[10];
			do
			{
				pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, a2);
				v619->pPlayerBuffs[PLAYER_BUFF_PAIN_REFLECTION].Apply(pParty->uTimePlayed + v717, v731, amount, v716, 0);
				++a2;
				++v619;
			}
			while ( v619 <= &pParty->pPlayers[3] );
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_DARK_SOULDRINKER:
		{
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			pGame->GetIndoorCamera();
			v623 = (signed __int64)pGame->pIndoorCameraD3D->GetPickDepth();
			signed int _v733 = _46A6AC_spell_render((int)dword_50BF30.data(), 100, v623);
			v707.x = 0;
			v707.y = 0;
			v707.z = 0;
			pSpellSprite.stru_24.Reset();
			pSpellSprite.spell_id = pCastSpell->uSpellID;
			pSpellSprite.spell_level = v2;
			pSpellSprite.spell_skill = v731;
			pSpellSprite.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellSprite.uType);
			pSpellSprite.uAttributes = 0;
			pSpellSprite.uSectorID = 0;
			pSpellSprite.uSpriteFrameID = 0;
			pSpellSprite.field_60_distance_related_prolly_lod = 0;
			pSpellSprite.uFacing = 0;
			pSpellSprite.spell_caster_pid = PID(OBJECT_Player, pCastSpell->uPlayerID);
			pSpellSprite.uSoundID = LOWORD(pCastSpell->sound_id);
			a2 = 0;
			int _v726 = 0;
			if ( _v733 > 0 )
			{
				_v726 = (_v733 * (7 * v2 + 25));
				do
				{
					v625 = dword_50BF30[a2];
					pSpellSprite.vPosition.x = pActors[v625].vPosition.x;
					pSpellSprite.vPosition.y = pActors[v625].vPosition.y;
					//v732 = pActors[v625].uActorHeight;
					pSpellSprite.vPosition.z = pActors[v625].vPosition.z - (unsigned int)(signed __int64)((double)pActors[v625].uActorHeight * unk_4D8548);
					pSpellSprite.spell_target_pid = PID(OBJECT_Actor, dword_50BF30[a2]);
					v627 = pSpellSprite.Create(0, 0, 0, 0);
					v628 = a2;
					DamageMonsterFromParty(PID(OBJECT_Item, v627), dword_50BF30[a2], &v707);
					a2 = v628 + 1;
				}
				while ( v628 + 1 < _v733 );
			}
			v730 = 0;
			v629 = 1;
			do
			{
				v630 = pPlayers[v629];
				if ( !v630->pConditions[2]
				&& !v630->pConditions[12]
				&& !v630->pConditions[13]
				&& !v630->pConditions[14]
				&& !v630->pConditions[15]
								&& !v630->pConditions[16] )
				{
					v631 = v730++;
					v681[v631] = v629;
				}
				++v629;
			}
			while ( v629 <= 4 );
			v732 = (signed __int64)((double)(signed int)_v726 / (double)v730);
			_v733 = 0;
			if ( v730 > 0 )
			{
				do
				{
					//v632 = 4 * v681[HIDWORD(v733)] + 10965188;
					v726 = pPlayers[v681[_v733]];
					//v633 = pPlayers[v681[HIDWORD(v733)]];
					v726->sHealth += v732;
					//v726 = *(Player **)v632;
					//v634 = v726->GetMaxHealth();
					if ( v726->sHealth > v726->GetMaxHealth())
						v726->sHealth = v726->GetMaxHealth();
					v635 = _v733;

					pGame->GetStru6()->SetPlayerBuffAnim(pCastSpell->uSpellID, WORD2(v733));
					_v733 = v635 + 1;
				}
				while ( v635 + 1 < v730 );
			}
			pGame->GetStru6()->FadeScreen__like_Turn_Undead_and_mb_Armageddon(0, 0x40u);
			LODWORD(v727) = 1;
			break;
		}
		case SPELL_DARK_ARMAGEDDON:
		{
			if ( uCurrentlyLoadedLevelType == LEVEL_Indoor)
			{
				ShowStatusBarString(pGlobalTXT_LocalizationStrings[499], 2);  // Can't cast Armageddon indoors!
				pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
				pCastSpell->uSpellID = 0;
				continue;
			}
			if ( v731 == 4)
				amount = 4;
			else
				amount = 3;
			if ( pPlayer->uNumArmageddonCasts >= amount || pParty->armageddon_timer > 0 )
			{
				ShowStatusBarString(pGlobalTXT_LocalizationStrings[428], 2); // Spell failed
				pAudioPlayer->PlaySound(SOUND_PlayerCantCastSpell, 0, 0, -1, 0, 0, 0, 0);
				pCastSpell->uSpellID = 0;
				continue;
			}
			if ( !pPlayer->CanCastSpell(uRequiredMana) )
				break;
			pParty->armageddon_timer = 256;
			pParty->armageddonDamage = v2;
			++pPlayer->uNumArmageddonCasts;
			if ( pParty->bTurnBasedModeOn == 1 )
				++pTurnEngine->pending_actions;
			int _v726 = 50;
			do
			{
				v642 = rand() % 4096 - 2048;
				v643 = rand();
				v721 = v642 + pParty->vPosition.x;
				y = (char *)(pParty->vPosition.y + v643 % 4096 - 2048);
				v732 = GetTerrainHeightsAroundParty2(v642 + pParty->vPosition.x, (int)y, &v710, 0);
				v644 = rand();
				sub_42F7EB_DropItemAt(0xFE6u, v721, (int)y, v732 + 16, v644 % 500 + 500, 1, 0, 0, 0);
				--_v726;
			}
			while ( _v726 != 0 );
			LODWORD(v727) = 1;
			break;
		}

		default:
			break;
	}
	if ( pCastSpell->uFlags & 0x20 )
	{
		if ( v727 != 0.0 )
			pAudioPlayer->PlaySound((SoundID)word_4EE088_sound_ids[pCastSpell->uSpellID], 0, 0, -1, 0, pCastSpell->sound_id, 0, 0);
	}
	else
	{
		if ( sRecoveryTime < 0 )
			sRecoveryTime = 0;
		if ( pParty->bTurnBasedModeOn )
		{
			v645 = sRecoveryTime;
			pParty->pTurnBasedPlayerRecoveryTimes[pCastSpell->uPlayerID] = sRecoveryTime;
			pPlayer->SetRecoveryTime(v645);
			if ( !some_active_character )
				pTurnEngine->ApplyPlayerAction();
		}
		else
		{
			pPlayer->SetRecoveryTime(
				(signed __int64)(flt_6BE3A4_debug_recmod1 * (double)sRecoveryTime * 2.133333333333333));
		}
		if ( v727 != 0.0 )
		{
			pPlayer->PlaySound(SPEECH_49, 0);
			if ( v727 != 0.0 )
				pAudioPlayer->PlaySound((SoundID)word_4EE088_sound_ids[pCastSpell->uSpellID], 0, 0, -1, 0, pCastSpell->sound_id, 0,	0);
		}
	}
	pCastSpell->uSpellID = 0;
	v2 = v723;
	continue;
  }
  
}
//----- (00427DA0) --------------------------------------------------------
size_t PushCastSpellInfo(uint16_t uSpellID, uint16_t uPlayerID, __int16 skill_level, uint16_t uFlags, int spell_sound_id)
{
  for (size_t i = 0; i < CastSpellInfoCount; i++)
  {
    if (!pCastSpellInfo[i].uSpellID)
    {
      pCastSpellInfo[i].uSpellID = uSpellID;
      pCastSpellInfo[i].uPlayerID = uPlayerID;
      if (uFlags & 0x10)
        pCastSpellInfo[i].uPlayerID_2 = uPlayerID;
      pCastSpellInfo[i].field_6 = 0;
      pCastSpellInfo[i].spell_target_pid = 0;
      pCastSpellInfo[i].uFlags = uFlags;
      pCastSpellInfo[i].forced_spell_skill_level = skill_level;
      pCastSpellInfo[i].sound_id = spell_sound_id;
      return i;
    }
  }
  return -1;
}
//----- (00427D48) --------------------------------------------------------
void CastSpellInfoHelpers::_427D48()
{
  for (size_t i = 0; i < CastSpellInfoCount; i++)
  {
    if (pCastSpellInfo[i].uSpellID && pCastSpellInfo[i].uFlags & 0x3CA)
    {
      pCastSpellInfo[i].uSpellID = 0;
      pGUIWindow_Settings->Release();
      pGUIWindow_Settings = 0;
      pMouse->SetCursorBitmap("MICON1");
      GameUI_Footer_TimeLeft = 0;
      _50C9A0_IsEnchantingInProgress = 0;
      back_to_game();
    }
  }
}
//----- (0042777D) --------------------------------------------------------
void _42777D_CastSpell_UseWand_ShootArrow(int a1, unsigned int uPlayerID, unsigned int a4, __int16 a5, int a6)
{
  //bool result; // eax@2
  //__int16 v6; // bx@4
  //Player *v7; // eax@4
  //char v8; // zf@12
  unsigned __int16 v9; // cx@16
  unsigned int v10; // eax@18
  unsigned __int8 v11; // sf@18
  unsigned __int8 v12; // of@18
  unsigned __int16 v13; // cx@21
  unsigned int v14; // eax@23
  //stru277 *v15; // esi@27
  //stru277 *v16; // eax@28
  //GUIWindow *v17; // eax@43
  //GUIWindow *v18; // [sp-30h] [bp-48h]@44
  //unsigned int v19; // [sp-2Ch] [bp-44h]@44
  //unsigned int v20; // [sp-28h] [bp-40h]@44
  //unsigned int v21; // [sp-24h] [bp-3Ch]@44
  //unsigned int v22; // [sp-20h] [bp-38h]@44
  //int v23; // [sp-1Ch] [bp-34h]@44
  //int v24; // [sp-18h] [bp-30h]@44
  //unsigned int v25; // [sp-14h] [bp-2Ch]@43
  //unsigned int v26; // [sp-10h] [bp-28h]@43
  //unsigned __int8 v27; // [sp-Ch] [bp-24h]@43
  //char *v28; // [sp-8h] [bp-20h]@43
  //Texture *v29; // [sp-4h] [bp-1Ch]@43
  //__int16 a3a; // [sp+10h] [bp-8h]@1
  //__int16 a2a; // [sp+14h] [bp-4h]@1

  //a3a = uPlayerID;
  //a2a = a1;
  
  //if ( pParty->bTurnBasedModeOn != 1
  //  || (result = pTurnEngine->field_4, pTurnEngine->field_4 != 1) && pTurnEngine->field_4 != 3 )
  if (pParty->bTurnBasedModeOn)
  {
    if (pTurnEngine->turn_stage == 1 || pTurnEngine->turn_stage == 3)
      return;
  }


    //v6 = a5;
    //v7 = &pParty->pPlayers[uPlayerID];
  assert(uPlayerID < 4);
  Player* player = &pParty->pPlayers[uPlayerID];
    if ( !(a5 & 0x10) )
    {
      switch ( a1 )
      {
        case SPELL_SPIRIT_FATE:
        case SPELL_BODY_FIRST_AID:
        case SPELL_DARK_REANIMATE:
          //HIBYTE(v6) = HIBYTE(a5) | 1;
          a5 |= 0x0100;
          break;

        case SPELL_FIRE_FIRE_AURA:
        case SPELL_WATER_RECHARGE_ITEM:
        case SPELL_WATER_ENCHANT_ITEM:
        case SPELL_DARK_VAMPIRIC_WEAPON:
          //LOBYTE(v6) = a5 | 0x80;
          a5 |= 0x0080;
          break;

        case SPELL_FIRE_FIRE_BOLT:
        case SPELL_FIRE_FIREBALL:
        case SPELL_FIRE_INCINERATE:
        case SPELL_AIR_LIGHNING_BOLT:
        case SPELL_AIR_IMPLOSION:
        case SPELL_WATER_POISON_SPRAY:
        case SPELL_WATER_ICE_BOLT:
        case SPELL_WATER_ACID_BURST:
        case SPELL_WATER_ICE_BLAST:
        case SPELL_EARTH_STUN:
        case SPELL_EARTH_SLOW:
        case SPELL_EARTH_DEADLY_SWARM:
        case SPELL_EARTH_BLADES:
        case SPELL_EARTH_MASS_DISTORTION:
        case SPELL_SPIRIT_SPIRIT_LASH:
        case SPELL_MIND_MIND_BLAST:
        case SPELL_MIND_CHARM:
        case SPELL_MIND_PSYCHIC_SHOCK:
        case SPELL_BODY_HARM:
        case SPELL_BODY_FLYING_FIST:
        case SPELL_LIGHT_LIGHT_BOLT:
        case SPELL_LIGHT_DESTROY_UNDEAD:
        case SPELL_LIGHT_SUNRAY:
        case SPELL_DARK_TOXIC_CLOUD:
        case SPELL_DARK_SHRINKING_RAY:
        case SPELL_DARK_SHARPMETAL:
        case SPELL_DARK_DRAGON_BREATH:
          if ( !a6 )
            a5 |= 0x0008;
          break;
        case SPELL_MIND_TELEPATHY:
        case SPELL_MIND_BERSERK:
        case SPELL_MIND_ENSLAVE:
        case SPELL_LIGHT_PARALYZE:
        case SPELL_DARK_CONTROL_UNDEAD:
//LABEL_9:
          //v6 = a5 | 8;
          a5 |= 0x0008;
          break;

        case SPELL_EARTH_TELEKINESIS:
          a5 |= 0x0040;
          break;

        case SPELL_SPIRIT_BLESS:
          if (a4 && ~a4 & 0x01C0)
            //goto LABEL_25;
          {
            a5 |= 0x0002;
            break;
          }
          else if ((player->pActiveSkills[PLAYER_SKILL_SPIRIT] & 0x1C0) == 0)
            //goto LABEL_25;
          {
            a5 |= 0x0002;
            break;
          }
          break;

        case SPELL_SPIRIT_PRESERVATION:
          v9 = a4;
          if ( !a4 )
            v9 = player->pActiveSkills[PLAYER_SKILL_SPIRIT];
          //goto LABEL_18;
          v10 = SkillToMastery(v9);
          v12 = __OFSUB__(v10, 3);
          v11 = ((v10 - 3) & 0x80000000u) != 0;
          //goto LABEL_24;
          if ( v11 ^ v12 )
          {
            a5 |= 0x0002;
            break;
          }
          break;

        case SPELL_DARK_PAIN_REFLECTION:
          v9 = a4;
          if ( !a4 )
            v9 = player->pActiveSkills[PLAYER_SKILL_DARK];
//LABEL_18:
          v10 = SkillToMastery(v9);
          v12 = __OFSUB__(v10, 3);
          v11 = ((v10 - 3) & 0x80000000u) != 0;
          //goto LABEL_24;
          if ( v11 ^ v12 )
          {
            a5 |= 0x0002;
            break;
          }
          break;

        case SPELL_BODY_HAMMERHANDS:
          v13 = a4;
          if ( !a4 )
            v13 = player->pActiveSkills[PLAYER_SKILL_BODY];
          v14 = SkillToMastery(v13);
          v12 = __OFSUB__(v14, 4);
          v11 = ((v14 - 4) & 0x80000000u) != 0;
//LABEL_24:
          if ( v11 ^ v12 )
            //goto LABEL_25;
          {
            a5 |= 0x0002;
            break;
          }
          break;

        case SPELL_EARTH_STONE_TO_FLESH:
        case SPELL_SPIRIT_REMOVE_CURSE:
        case SPELL_SPIRIT_RAISE_DEAD:
        case SPELL_SPIRIT_RESSURECTION:
        case SPELL_MIND_REMOVE_FEAR:
        case SPELL_MIND_CURE_PARALYSIS:
        case SPELL_MIND_CURE_INSANITY:
        case SPELL_BODY_CURE_WEAKNESS:
        case SPELL_BODY_REGENERATION:
        case SPELL_BODY_CURE_POISON:
        case SPELL_BODY_CURE_DISEASE:
//LABEL_25:
          //v6 = a5 | 2;
          a5 |= 0x0002;
          break;

        case SPELL_DARK_SACRIFICE:
          //HIBYTE(v6) = HIBYTE(a5) | 2;
          a5 |= 0x0200;
          break;
        default:
          break;
      }
    }


    if (a5 & 0x3CA)
    {
      for (uint i = 0; i < CastSpellInfoCount; ++i)
        if (pCastSpellInfo[i].uFlags & 0x3CA)
        {
          pCastSpellInfo[i].uSpellID = 0;
          break;
        }
    }
      
    for (uint i = 0; i < CastSpellInfoCount; ++i)
    {
      CastSpellInfo* spell = &pCastSpellInfo[i];
      if (!spell->uSpellID)
        continue;

      spell->uSpellID = 0;
      if (spell->uFlags & 0x3CA)
      {
        pGUIWindow_Settings->Release();
        pGUIWindow_Settings = nullptr;
        pMouse->SetCursorBitmap("MICON1");
        GameUI_Footer_TimeLeft = 0;
        _50C9A0_IsEnchantingInProgress = 0;
        back_to_game();
      }
    }

    int result = PushCastSpellInfo(a1, uPlayerID, a4, a5, a6);
    if ( result != -1 )
    {
      if ( a5 & 2 )
      {
        if ( pGUIWindow_Settings )
          return;
        pGUIWindow_Settings = GUIWindow::Create(0, 0, 640, 480, WINDOW_CastSpell, (int)&pCastSpellInfo[result], 0);
        pGUIWindow_Settings->CreateButton(52, 422, 35, 0, 2, 0, UIMSG_CastSpell_Character_Big_Improvement, 0, 49, "", 0);
        pGUIWindow_Settings->CreateButton(165, 422, 35, 0, 2, 0, UIMSG_CastSpell_Character_Big_Improvement, 1, 50, "", 0);
        pGUIWindow_Settings->CreateButton(280, 422, 35, 0, 2, 0, UIMSG_CastSpell_Character_Big_Improvement, 2, 51, "", 0);
        pGUIWindow_Settings->CreateButton(390, 422, 35, 0, 2, 0, UIMSG_CastSpell_Character_Big_Improvement, 3, 52, "", 0);
        pParty->sub_421B2C_PlaceInInventory_or_DropPickedItem();
        return;
      }
      if ( a5 & 8 )
      {
        if ( pGUIWindow_Settings )
          return;

        pGUIWindow_Settings = GUIWindow::Create(0, 0, 640, 480, WINDOW_CastSpell, (int)&pCastSpellInfo[result], 0);
        pGUIWindow_Settings->CreateButton(game_viewport_x, game_viewport_y, game_viewport_width, game_viewport_height, 1, 0, UIMSG_CastSpell_Shoot_Monster, 0, 0, "", 0);
        pParty->sub_421B2C_PlaceInInventory_or_DropPickedItem();
        return;
      }
      if ( a5 & 0x40 )
      {
        if ( pGUIWindow_Settings )
          return;

        pGUIWindow_Settings = GUIWindow::Create(0, 0, 640, 480, WINDOW_CastSpell, (int)&pCastSpellInfo[result], 0);
        pGUIWindow_Settings->CreateButton(game_viewport_x, game_viewport_y, game_viewport_width, game_viewport_height, 1, 0, UIMSG_CastSpell_Telekinesis, 0, 0, "", 0);
        pParty->sub_421B2C_PlaceInInventory_or_DropPickedItem();
        return;
      }
      if ( (char)a5 < 0 )
      {
        if ( pGUIWindow_Settings )
          return;
        ++pIcons_LOD->uTexturePacksCount;
        if ( !pIcons_LOD->uNumPrevLoadedFiles )
          pIcons_LOD->uNumPrevLoadedFiles = pIcons_LOD->uNumLoadedFiles;
        pGUIWindow_Settings = pCastSpellInfo[result].GetCastSpellInInventoryWindow();
        _50C9A0_IsEnchantingInProgress = 1;
        some_active_character = uActiveCharacter;
        pParty->sub_421B2C_PlaceInInventory_or_DropPickedItem();
        return;
      }
      if ( HIBYTE(a5) & 1 )
      {
        if ( pGUIWindow_Settings )
          return;
        pGUIWindow_Settings = GUIWindow::Create(0, 0, 640, 480, WINDOW_CastSpell, (int)&pCastSpellInfo[result], 0);
        pGUIWindow_Settings->CreateButton(0x34u, 0x1A6u, 0x23u, 0, 2, 0, UIMSG_CastSpell_Character_Small_Improvement, 0, 0x31u, "", 0);
        pGUIWindow_Settings->CreateButton(0xA5u, 0x1A6u, 0x23u, 0, 2, 0, UIMSG_CastSpell_Character_Small_Improvement, 1, 0x32u, "", 0);
        pGUIWindow_Settings->CreateButton(0x118u, 0x1A6u, 0x23u, 0, 2, 0, UIMSG_CastSpell_Character_Small_Improvement, 2, 0x33u, "", 0);
        pGUIWindow_Settings->CreateButton(0x186u, 0x1A6u, 0x23u, 0, 2, 0, UIMSG_CastSpell_Character_Small_Improvement, 3, 0x34u, "", 0);
        pGUIWindow_Settings->CreateButton(8, 8, game_viewport_width, game_viewport_height, 1, 0, UIMSG_CastSpell_Monster_Improvement, 0, 0, "", NULL);
        pParty->sub_421B2C_PlaceInInventory_or_DropPickedItem();
      }
      if ( HIBYTE(a5) & 2 && !pGUIWindow_Settings )
      {
        pGUIWindow_Settings = GUIWindow::Create(0, 0, 640, 480, WINDOW_CastSpell, (int)&pCastSpellInfo[result], 0);
        pBtn_NPCLeft = pGUIWindow_Settings->CreateButton(469, 178,
                       pIcons_LOD->GetTexture(uTextureID_Btn_NPCLeft)->uTextureWidth,
                       pIcons_LOD->GetTexture(uTextureID_Btn_NPCLeft)->uTextureHeight,
                       1, 0, UIMSG_ScrollNPCPanel, 0, 0, "",
                       pIcons_LOD->GetTexture(uTextureID_Btn_NPCLeft), 0);
        pBtn_NPCRight = pGUIWindow_Settings->CreateButton(626, 178,
                       pIcons_LOD->GetTexture(uTextureID_Btn_NPCRight)->uTextureWidth,
                       pIcons_LOD->GetTexture(uTextureID_Btn_NPCRight)->uTextureHeight,
                       1, 0, UIMSG_ScrollNPCPanel, 1, 0, "",
                       pIcons_LOD->GetTexture(uTextureID_Btn_NPCRight), 0);
        pGUIWindow_Settings->CreateButton(491, 149, 64, 74, 1, 0, UIMSG_HiredNPC_CastSpell, 4, 0x35u, "", 0);
        pGUIWindow_Settings->CreateButton(561, 149, 64, 74, 1, 0, UIMSG_HiredNPC_CastSpell, 5, 0x36u, "", 0);
      }
    }
}
