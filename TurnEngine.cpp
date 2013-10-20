
#include "MM7.h"
#include "mm7_data.h"
#include "Actor.h"
#include "Player.h"
#include "Party.h"
#include "AudioPlayer.h"
#include "SpriteObject.h"
#include "Time.h"
#include "stru298.h"
#include "IconFrameTable.h"
#include "Viewport.h"
#include "FactionTable.h"

#include "TurnEngine.h"


#include "TurnEngine.h"
struct stru262_TurnBased *pTurnEngine = new stru262_TurnBased;


//----- (00404544) --------------------------------------------------------
void stru262_TurnBased::SortTurnQueue()
    {

    int active_actors;
    TurnBased_QueueElem *current_top; // eax@16
    TurnBased_QueueElem *test_element; // ecx@18
    TurnBased_QueueElem temp_elem;
    int i,j;
    unsigned int p_type;
    unsigned int p_id;

    active_actors = this->uActorQueueSize;
    //set non active actors in queue initiative that not allow them to paticipate 
    for( i=0; i<uActorQueueSize; ++i) 
        {
        p_type = PID_TYPE(pQueue[i].uPackedID);
        p_id   = PID_ID(pQueue[i].uPackedID);

        if ( p_type == OBJECT_Actor )
            {
            pActors[p_id].uAttributes |= 0x80u;
            if ( !pActors[p_id].CanAct() )
                {
                --active_actors;
                pQueue[i].actor_initiative = 1001;
                pActors[p_id].uAttributes &= ~0x80;
                }
            }
        else if ( p_type == OBJECT_Player)
            {
            if ( !pParty->pPlayers[p_id].CanAct() )
                {
                --active_actors;
                pQueue[i].actor_initiative = 1001;
                }
            }
        }
     //sort
if (uActorQueueSize>0)
    {
    for( i=0; i<uActorQueueSize-1; ++i) 
        {
       current_top=&pQueue[i];
        for(j=i+1; j<uActorQueueSize;++j )
            {  
            test_element=&pQueue[j];           
            if ( test_element->actor_initiative < current_top->actor_initiative  || // if less initiative -> top
                   ((test_element->actor_initiative == current_top->actor_initiative) && 
                       (
                          ((PID_TYPE(test_element->uPackedID) == OBJECT_Player) && (PID_TYPE(current_top->uPackedID) == OBJECT_Actor)) || //player preferable
                          ((PID_TYPE(test_element->uPackedID) == PID_TYPE(current_top->uPackedID)) && (PID_ID(test_element->uPackedID) < PID_ID(current_top->uPackedID)))  //less id preferable    
                       ) 
                   )
                )
                { //swap
                memcpy(&temp_elem,current_top,sizeof(TurnBased_QueueElem));
                memcpy(current_top,test_element, sizeof(TurnBased_QueueElem));
                memcpy(test_element, &temp_elem, sizeof(TurnBased_QueueElem));
                }
            }
        }
    }
    uActorQueueSize = active_actors;
    if ( PID_TYPE(pQueue[0].uPackedID) == OBJECT_Player) //we have player at queue top
        {
        uActiveCharacter = PID_ID(pQueue[0].uPackedID)+1;
        field_18 |= TE_PLAYER_TURN;
        }
    else
        {
        uActiveCharacter = 0;
        field_18 &= ~TE_PLAYER_TURN;
        }
    for(i=0; i<uActorQueueSize; ++i)
        {
        if ( PID_TYPE(pQueue[i].uPackedID) == OBJECT_Player) //set recovery times
            pParty->pPlayers[PID_ID(pQueue[i].uPackedID)].uTimeToRecovery = (unsigned __int16)((double)pQueue[i].actor_initiative * 0.46875);
        }


    }

//----- (0040471C) --------------------------------------------------------
void stru262_TurnBased::ApplyPlayerAction()
    {
    if ( pParty->bTurnBasedModeOn == 1 )
        {
        if ( pTurnEngine->turn_stage == 2 )
            _406457(0);
        }
    }

//----- (004059DB) --------------------------------------------------------
void stru262_TurnBased::Start()
    {

    int v3; // esi@1
    unsigned int actor_id; // esi@7
    unsigned int v8; // edx@10
    int v17; // edx@22
    AIDirection v30; // [sp+Ch] [bp-68h]@10
    AIDirection v31; // [sp+28h] [bp-4Ch]@10
    AIDirection a3; // [sp+44h] [bp-30h]@10
    Player *pPlayer; // [sp+6Ch] [bp-8h]@1
    int v40b;
    int activ_players[4];
    int players_recovery_time[4];
    int a_players_count;
    int i,j;
    int temp;

    pTurnEngine->field_18 &= ~TE_HAVE_PENDING_ACTIONS;
    pEventTimer->TrackGameTime();
    pAudioPlayer->StopChannels(-1, -1);
    pAudioPlayer->PlaySound(SOUND_207, 0, 0, -1, 0, 0, 0, 0);
    pPlayer = pParty->pPlayers.data();
    dword_50C998_turnbased_icon_1A = 8 * pIconsFrameTable->pIcons[uIconID_TurnStart].uAnimLength;
    dword_50C994 = 0;

    this->turn_initiative = 100;
    this->turns_count = 0;
    this->ai_turn_timer = 64;
    this->turn_stage = 1;
    this->uActorQueueSize = 0;

    for ( v3 = 0; v3 < 4 ; ++v3 )
        {
        if ( pParty->pPlayers[v3].CanAct() )
            {
            this->pQueue[this->uActorQueueSize].uPackedID = PID(OBJECT_Player,v3);
            this->pQueue[this->uActorQueueSize].AI_action_type = TE_AI_PURSUE;
            this->pQueue[this->uActorQueueSize].uActionLength = 0;
            pParty->pTurnBasedPlayerRecoveryTimes[this->uActorQueueSize] = 0;
            ++this->uActorQueueSize;
            }
        }

    for ( v3 = 0; v3 < ai_arrays_size ; ++v3 )
        { 
        actor_id = ai_near_actors_ids[v3];
        if (actor_id == 10)
            continue;
        if ( pActors[actor_id].CanAct() )
            {
            if ( pActors[actor_id].uAttributes & 0x8000 )
                {
                v8 = ai_near_actors_targets_pid[actor_id];
                pActors[actor_id].uAttributes |= 0x80;
                memcpy(&v31, Actor::GetDirectionInfo(PID(OBJECT_Actor,actor_id), v8, &a3, 0), sizeof(AIDirection));
                memcpy(&v30, &v31, sizeof(AIDirection));
                Actor::AI_StandOrBored(actor_id, 4, 32, &v30);
                this->pQueue[this->uActorQueueSize].uPackedID = PID(OBJECT_Actor,actor_id);
                this->pQueue[this->uActorQueueSize].AI_action_type = TE_AI_PURSUE;
                this->pQueue[this->uActorQueueSize].uActionLength = 0;
                ++this->uActorQueueSize;
                }
            }
        }

    a_players_count=0;
    for ( v40b = 0; v40b < this->uActorQueueSize; ++v40b )
        {
        //set initial initiative for turn actors
        if ( PID_TYPE(this->pQueue[v40b].uPackedID) == OBJECT_Player )
            {
            if ( pPlayers[PID_ID(this->pQueue[v40b].uPackedID) + 1]->uTimeToRecovery != 0 )
                {
                this->pQueue[v40b].actor_initiative = (signed int)((double)pPlayers[PID_ID(this->pQueue[v40b].uPackedID) + 1]->uTimeToRecovery * 0.46875);
                }
            else
                {   
                activ_players[a_players_count] = v40b;
                ++a_players_count;
                }
            }
        else if ( PID_TYPE(this->pQueue[v40b].uPackedID) == OBJECT_Actor )
            {
            v17 = rand() % 99;
            if ( v17 < 33 )
                this->pQueue[v40b].actor_initiative = 1;
            else 
                this->pQueue[v40b].actor_initiative= (v17 >= 66)? 5 : 3; 
            }
        else 
            {  //fot non player and actor
            this->pQueue[v40b].actor_initiative = 666;
            }
        this->pQueue[v40b].actor_initiative += 16;
        }

    if ( a_players_count > 0 )
        {
        for (i=0; i<a_players_count; ++i)
            players_recovery_time[i] = pParty->pPlayers[PID_ID(this->pQueue[activ_players[i]].uPackedID)].GetAttackRecoveryTime(0);
        //sort players by recovery time
        for (i=0; i < a_players_count-1; ++i)
            {  
            for(j=i+1; j<a_players_count;++j )
                {       
                if (players_recovery_time[j] < players_recovery_time[i]) //swap values
                    {
                    temp = players_recovery_time[i];
                    players_recovery_time[i] = players_recovery_time[j];
                    players_recovery_time[j] = temp;

                    temp = activ_players[i];
                    activ_players[i] = activ_players[j];
                    activ_players[j] = temp;
                    }
                }
            }

        for (i=0; i<a_players_count; ++i)
            {
            this->pQueue[activ_players[i]].actor_initiative = i+2;
            }
        }
    this->SortTurnQueue();
    }



//----- (00405CFF) --------------------------------------------------------
void stru262_TurnBased::End(bool bPlaySound)
    {
   
    ObjectType objType; // eax@13
    int objID; // esi@13
    int i; 

    this->turn_stage = 0;

    for( i=0; i<uActorQueueSize; ++i) 
        { 
        if ( PID_TYPE(pQueue[i].uPackedID) == OBJECT_Actor )
            pActors[PID_ID(pQueue[i].uPackedID)].uAttributes &= ~0x80;
        }

    for( i=0; i<uNumSpriteObjects; ++i) 
        {
         if (pSpriteObjects[i].uAttributes & 4)
             pSpriteObjects[i].uAttributes &= ~0x04;
        }

    for( i=0; i<uActorQueueSize; ++i) 
        { 
        objType = (ObjectType)PID_TYPE(pQueue[i].uPackedID);
        objID = PID_ID(pQueue[i].uPackedID);
        if ( objType == OBJECT_Player )
            {
            pPlayers[objID + 1]->uTimeToRecovery = (unsigned __int16)((double)pQueue[i].actor_initiative * 2.133333333333333);
            }
        else if ( objType == OBJECT_Actor )
            {
             pActors[objID].pMonsterInfo.uRecoveryTime = (unsigned __int16)((double)pQueue[i].actor_initiative * 2.133333333333333);
            }
        }
   
    pAudioPlayer->StopChannels(-1, -1);
    if ( bPlaySound != 0 )
        pAudioPlayer->PlaySound(SOUND_206, 0, 0, -1, 0, 0, 0, 0);
    pTurnEngine->field_18 &= ~TE_HAVE_PENDING_ACTIONS;
    pEventTimer->StopGameTime();
    dword_50C994 = 0;
    dword_50C998_turnbased_icon_1A = 0;
    }
// 50C994: using guessed type int dword_50C994;
// 50C998: using guessed type int dword_50C998_turnbased_icon_1A;

//----- (00405E14) --------------------------------------------------------
void stru262_TurnBased::AITurnBasedAction()
    {
    AIDirection *v6; // esi@21
    int v7; // eax@21
    AIDirection a3; // [sp+4h] [bp-68h]@21
    AIDirection v14; // [sp+20h] [bp-4Ch]@21
    AIDirection v15; // [sp+3Ch] [bp-30h]@21
    Actor *curr_actor; // [sp+58h] [bp-14h]@2
    int target_pid; // [sp+5Ch] [bp-10h]@6
    int shrinked;
    int i,j;

    for (i =0; i<uNumActors; ++i )
        {
        curr_actor=&pActors[i];
        shrinked=pActors[i].pActorBuffs[ACTOR_BUFF_SHRINK].uExpireTime > 0;
        for (j =0; j<22; ++j) //check expired spell Buffs
            {
            if(j != 10)
                pActors[i].pActorBuffs[j].IsBuffExpiredToTime(pParty->uTimePlayed);
            }
        if (shrinked && pActors[i].pActorBuffs[ACTOR_BUFF_SHRINK].uExpireTime <=0) //buff 3 expired
            {
            pActors[i].uActorHeight = pMonsterList->pMonsters[pActors[i].pMonsterInfo.uID - 1].uMonsterHeight;
            }
        if(!(curr_actor->uAttributes&0x80)&&
              (!curr_actor->pActorBuffs[ACTOR_BUFF_STONED].uExpireTime)&&
              (!curr_actor->pActorBuffs[ACTOR_BUFF_PARALYZED].uExpireTime)) 
            {
            curr_actor->uCurrentActionTime += pMiscTimer->uTimeElapsed;
            if (curr_actor->uCurrentActionTime>=curr_actor->uCurrentActionLength)
                {
                target_pid = ai_near_actors_targets_pid[i];
                v6 = Actor::GetDirectionInfo(PID(OBJECT_Actor,i), target_pid, &a3, 0);  
                memcpy(&v15, v6, sizeof(AIDirection));   
                memcpy(&v14, &v15, sizeof(AIDirection));
                v7 = curr_actor->uAIState;
                if(v7==Dying)
                    {
                    curr_actor->uCurrentActionTime = 0;
                    curr_actor->uCurrentActionLength = 0;
                    curr_actor->uAIState = Dead;
                    curr_actor->UpdateAnimation();
                    }
                else  if ( (v7 > AIState::Removed)&&(v7 < AIState::Disabled))
                    {
                     Actor::AI_StandOrBored(i, target_pid, 32, &v14);
                    }
                }
            }
        }
    
    if ( turn_stage == 1 )
        {
        if ( ai_turn_timer == 64 )
            ActorAISetMovementDecision();
        else  if ( ai_turn_timer > 0 )
            ActorAIDoAdditionalMove();
        else
            {
             ActorAIStopMovement();
             turn_initiative = 100;
            }
        ai_turn_timer -= pEventTimer->uTimeElapsed;
        }
    else if ( turn_stage == 2 )
        {
        if ( !(field_18 &TE_FLAG_1))
            {
            if ( turn_initiative == 100 )
                {
                StartTurn();
                SetAIRecoveryTimes();
                return;
                }
            if ( turn_initiative > 0 || pQueue[0].actor_initiative <= 0 )
                {
                _4065B0();
                SetAIRecoveryTimes();
                }
            }
        NextTurn();
        }
    else if ( turn_stage == 3 )
        {
        if ( (uActionPointsLeft > 0) && (!(field_18 & TE_FLAG_8)) )
           ActorAIChooseNewTargets();
        else
            { 
            field_18 &= ~TE_FLAG_8;
            turn_stage = 1;
            ai_turn_timer = 64; 
            }
        }
    }


//----- (00406051) --------------------------------------------------------
 void stru262_TurnBased::StartTurn()
    {
    int player_num, actor_num, i, j;

    pending_actions = 0;
    //add player to queue if he can act
    for(player_num=0; player_num<4; ++player_num) 
        {
        for(j=0; j<uActorQueueSize; ++j) 
            {
            if (PID_TYPE(pQueue[j].uPackedID)== OBJECT_Player)
                {
                if (pPlayers[PID_ID(pQueue[j].uPackedID) + 1]->CanAct() && (player_num != PID_ID(pQueue[j].uPackedID)) )
                    break;
                }
            }
         if (j==uActorQueueSize )
             {
             pQueue[uActorQueueSize].uPackedID = PID(OBJECT_Player,player_num);
             pQueue[uActorQueueSize].actor_initiative = 100;
             pQueue[uActorQueueSize].uActionLength = 0;
             pQueue[uActorQueueSize].AI_action_type = TE_AI_STAND;
             ++uActorQueueSize;
             }
        }
    //add new arrived actors
    for(actor_num=0; actor_num<ai_arrays_size; ++actor_num) 
        {
        for(j=0; j<uActorQueueSize; ++j) 
            {
            if ((PID_TYPE(pQueue[j].uPackedID)== OBJECT_Actor)&&
                 ai_near_actors_ids[actor_num] == PID_ID(pQueue[j].uPackedID))
                    break;
            }
        if (j==uActorQueueSize )
            {
            pQueue[uActorQueueSize].uPackedID = PID(OBJECT_Actor,ai_near_actors_ids[actor_num]);
            pQueue[uActorQueueSize].actor_initiative = 1;
            pQueue[uActorQueueSize].uActionLength = 0;
            pQueue[uActorQueueSize].AI_action_type = TE_AI_STAND;
            ++uActorQueueSize;
            }
        }

    ++turns_count;
    turn_initiative = 100;

    for(i=0; i<uActorQueueSize; ++i) 
        {
        if (pQueue[i].actor_initiative == 0 )
            pQueue[i].actor_initiative = 100;
        }

    StepTurnQueue();
    for(i=0; i<uActorQueueSize; ++i) 
        {
        if ((PID_TYPE(pQueue[i].uPackedID)==OBJECT_Player)||(pQueue[i].actor_initiative > 0))
            break;
        AI_Action_(i);
        }
    }
// 4F75D8: using guessed type int ai_arrays_size;

 //----- (004061CA) --------------------------------------------------------
 void stru262_TurnBased::NextTurn()
     {
     Actor *curr_actor; // eax@9
     int ai_state; // ecx@9
     int v13; // [sp+10h] [bp-4h]@7
     int i;
     int monster_ai_state;
     int monster; // eax@5

     SortTurnQueue();
     if (PID_TYPE(pQueue[0].uPackedID) == OBJECT_Player)
         uActiveCharacter = PID_ID(pQueue[0].uPackedID) + 1;
     else
         uActiveCharacter = 0;
     viewparams->bRedrawGameUI = 1;

     if ( pending_actions )
         {
         pTurnEngine->field_18 |= TE_HAVE_PENDING_ACTIONS;
         return;
         }
     pTurnEngine->field_18 &= ~TE_HAVE_PENDING_ACTIONS;
     if ( pQueue[0].actor_initiative <= 0 )
         return;

     v13 = 0;
     if (uActorQueueSize > 0 )
         {   

         for (i=0; i<uActorQueueSize; ++i )
             {
             if (PID_TYPE(pQueue[i].uPackedID) == OBJECT_Actor)
                 {
                 curr_actor = &pActors[PID_ID(pQueue[i].uPackedID)];
                 ai_state = curr_actor->uAIState;
                 if ( (ai_state == AIState::Dying) || (ai_state == AIState::Stunned) || (ai_state == AIState::AttackingMelee) ||
                     (ai_state == AIState::AttackingRanged1) || (ai_state == AIState::AttackingRanged2) || 
                     (ai_state == AIState::AttackingRanged3) || (ai_state == AIState::AttackingRanged4) || (ai_state ==AIState::Summoned))
                     {
                     curr_actor->uCurrentActionTime += pEventTimer->uTimeElapsed;
                     if ( curr_actor->uCurrentActionTime < curr_actor->uCurrentActionLength )
                         {
                         v13 = 1;
                         }
                     else if ( ai_state == AIState::Dying )// Dying
                         {
                         curr_actor->uAIState = AIState::Dead;
                         curr_actor->uCurrentActionTime = 0;
                         curr_actor->uCurrentActionLength = 0;
                         curr_actor->UpdateAnimation();
                         }
                     else
                         {
                         if ( ai_state == AIState::Stunned ) //Stunned
                             Actor::AI_StandOrBored(PID_ID(pQueue[i].uPackedID), ai_near_actors_targets_pid[PID_ID(pQueue[i].uPackedID)], 32, 0);
                         }
                     }
                 }
             }
         if ( v13 != 0 )
             {
             field_18 |= TE_FLAG_1;
             return;
             }
         }

     field_18 &= ~TE_FLAG_1;   
     //set all actors to stay
     for (i=0; i<uActorQueueSize; ++i )
         {      
         if(PID_TYPE(pQueue[i].uPackedID) == OBJECT_Actor) 
             {
             monster=PID_ID(pQueue[i].uPackedID);
             monster_ai_state=pActors[monster].uAIState;
             if ((monster_ai_state != AIState::Dead) && 
                 (monster_ai_state != AIState::Dying) &&
                 (monster_ai_state != AIState::Removed) &&
                 (monster_ai_state != AIState::Summoned) &&
                 (monster_ai_state != AIState::Disabled))
                 {
                 pQueue[i].uActionLength = 0;
                 Actor::AI_StandOrBored(monster, ai_near_actors_targets_pid[monster], 32, nullptr);
                 }
             }
         }
     // turn tick
     turn_stage = 3;
     pParty->uTimePlayed += 213i64;
     _494035_timed_effects__water_walking_damage__etc();
     uActionPointsLeft = 130;
     }

 //----- (004063A1) --------------------------------------------------------
 bool stru262_TurnBased::StepTurnQueue()
 {
     int v9; // dx@12 
     int j;

     SortTurnQueue();
     viewparams->bRedrawGameUI = 1;
     if ( pQueue[0].actor_initiative!=0 )
         {
         if (PID_TYPE(pQueue[0].uPackedID) == OBJECT_Player)
             {
             do
                 {
                 for (j=0; j<uActorQueueSize; ++j )
                     {      
                     --pQueue[j].actor_initiative;
                     }
                 --turn_initiative;
                 if (turn_initiative == 0)
                     return true;
                 }
                 while (pQueue[0].actor_initiative != 0);
             }
         else
             {
             if ( pQueue[0].actor_initiative>0 )
                 {            
             v9 = pActors[PID_ID(pQueue[0].uPackedID)].uAIState;
             if (!(v9 == AIState::Dying || v9 == AIState::Dead || 
                   v9 == AIState::Disabled || v9 == AIState::Removed))
                 {
                    do
                     {
                     for (j=0; j<uActorQueueSize; ++j )
                         {      
                         --pQueue[j].actor_initiative;
                         if (pQueue[j].actor_initiative == 0)
                             pQueue[j].uActionLength = 0;
                         }
                     --turn_initiative;
                     if (turn_initiative == 0)
                         return true;
                     }
                     while (pQueue[0].actor_initiative > 0); 
                 }
                 }
             }
         }
     return false;
     }

//----- (00406457) --------------------------------------------------------
void stru262_TurnBased::_406457( int a2 )
    {
    signed int v4; // ecx@2
    signed int v6; // eax@2
    int i;
    v6=0;
    if (  PID_TYPE(pQueue[a2].uPackedID) == OBJECT_Player)
        {
        v4 = PID_ID(pQueue[a2].uPackedID);
        if ( pParty->pTurnBasedPlayerRecoveryTimes[v4] )
            pParty->pTurnBasedPlayerRecoveryTimes[v4] = 0;
        else
            v6 = pPlayers[v4 + 1]->GetAttackRecoveryTime(0);
        if ( v6 < 30 )
            v6 = 30;
        }
    else
        {
        v6 = pMonsterStats->pInfos[pActors[PID_ID(pQueue[a2].uPackedID)].pMonsterInfo.uID].uRecoveryTime;
        }

    pQueue[a2].actor_initiative = v6;
    SortTurnQueue();
    if (PID_TYPE(pQueue[0].uPackedID) == OBJECT_Player)
        uActiveCharacter = PID_ID(pQueue[0].uPackedID) + 1;
    else
        uActiveCharacter = 0;
    viewparams->bRedrawGameUI = 1;

    while ( (pQueue[0].actor_initiative > 0)&&(turn_initiative > 0) )
        {
        for (i=0; i<uActorQueueSize; ++i)
            {
            --pQueue[i].actor_initiative;
            if (pQueue[i].actor_initiative==0)
                pQueue[i].uActionLength=0;
            }
        --turn_initiative;
        }
    }

//----- (0040652A) --------------------------------------------------------
void stru262_TurnBased::SetAIRecoveryTimes()
    {
    int i;
    int monster_ai_state;
    Actor *monster; // eax@5

    for (i=0; i<uActorQueueSize; ++i )
        {
        if (pQueue[i].actor_initiative == 0)
            {
            if(PID_TYPE(pQueue[i].uPackedID) == OBJECT_Player) 
                break;
            monster=&pActors[PID_ID(pQueue[i].uPackedID)];
            monster_ai_state=monster->uAIState;
            if (monster_ai_state == AIState::Standing || 
                monster_ai_state == AIState::Fleeing || 
                monster_ai_state == AIState::Fidgeting)
                {
                pQueue[i].actor_initiative = pMonsterStats->pInfos[monster->pMonsterInfo.uID].uRecoveryTime;
                if (monster->pActorBuffs[7].uExpireTime > 0)
                    pQueue[i].actor_initiative*=2;
                }
            }
        }
    }

//----- (004065B0) --------------------------------------------------------
void stru262_TurnBased::_4065B0()
{
    int i;

    SortTurnQueue();
    if (pQueue[0].actor_initiative <= 0)
        {  
        for (i=0; i<uActorQueueSize; ++i )
            {
            if ((PID_TYPE(pQueue[i].uPackedID)==OBJECT_Player)|| (pQueue[i].actor_initiative > 0) )
                break;
            if ((pQueue[i].uActionLength<=0) && (PID_TYPE(pQueue[i].uPackedID)==OBJECT_Actor))
                AI_Action_(i);
            }
        }
    else
        {
        StepTurnQueue();
        if (PID_TYPE(pQueue[0].uPackedID) == OBJECT_Player) 
            uActiveCharacter = PID_ID(pQueue[0].uPackedID) + 1;
        else
            uActiveCharacter = 0;
        viewparams->bRedrawGameUI = 1;
        }

    for (i=0; i<uActorQueueSize; ++i )
        AIAttacks(i);
}

//----- (00406648) --------------------------------------------------------
void stru262_TurnBased::AIAttacks( unsigned int queue_index )
{
    TurnBased_QueueElem *v1; // ecx@1
    int v3; // eax@1
    unsigned int v4; // ebx@2
    Actor *v5; // esi@2
    char v19; // al@24
    AIDirection a3; // [sp+Ch] [bp-3Ch]@2
    AIDirection a4; // [sp+28h] [bp-20h]@2
    TurnBased_QueueElem *v28; // [sp+44h] [bp-4h]@1
    unsigned int a2a; // [sp+50h] [bp+8h]@2

    v1 = &pQueue[queue_index];
    v28 = v1;
    v3 = v1->uPackedID;
    if (PID_TYPE(pQueue[queue_index].uPackedID) == OBJECT_Actor)
        {
        v4 = PID_ID(v3);
        a2a = ai_near_actors_targets_pid[PID_ID(pQueue[queue_index].uPackedID)];
        memcpy(&a3, Actor::GetDirectionInfo(v1->uPackedID, ai_near_actors_targets_pid[PID_ID(v3)], &a3, 0), sizeof(a3));
        memcpy(&a4, &a3, sizeof(a4));
        v5 = &pActors[PID_ID(v3)];
        LOWORD(v3) = v5->uAIState;
        if (( (short)v3 != AIState::Dead )&& ( (short)v3 != AIState::Disabled )&&( (short)v3 != AIState::Removed ))  
                    {
                    v5->uCurrentActionTime += pEventTimer->uTimeElapsed;
                    if ( (signed int)v5->uCurrentActionTime >= v5->uCurrentActionLength )
                        {
                         switch (v3)
                         {
                           case  AIState::AttackingMelee:
                             v19 = stru_50C198.special_ability_use_check(&pActors[v4], v4);
                             stru_50FE08.Add( v28->uPackedID,  5120,  v5->vPosition.x, v5->vPosition.y, v5->vPosition.z + ((signed int)v5->uActorHeight >> 1), v19,  1);
                             Actor::AI_Stand(v4, a2a, 0, &a4);
                             break;
                           case AIState::AttackingRanged1:
                             Actor::AI_RangedAttack(v4, &a4, v5->pMonsterInfo.uMissleAttack1Type, 0);
                             Actor::AI_Stand(v4, a2a, 0,&a4);
                             break;
                           case AIState::Dying:
                             v5->uCurrentActionTime = 0;
                             v5->uCurrentActionLength = 0;
                             v5->uAIState = Dead;
                             pActors[v4].UpdateAnimation();
                             break;
                           case AIState::Stunned:
                              Actor::AI_Stand(v4, a2a, 0,&a4);
                              break;
                           case AIState::AttackingRanged2:
                             Actor::AI_RangedAttack(v4, &a4, v5->pMonsterInfo.uMissleAttack2Type, 1);
                             Actor::AI_Stand(v4, a2a, 0,&a4);
                             break;
                           case AIState::AttackingRanged3:
                             Actor::AI_SpellAttack(v4, &a4, v5->pMonsterInfo.uSpell1ID, 2, v5->pMonsterInfo.uSpellSkillAndMastery1);
                             Actor::AI_Stand(v4, a2a, 0, &a4);
                             break;
                           case AIState::AttackingRanged4:
                             Actor::AI_SpellAttack(v4, &a4, v5->pMonsterInfo.uSpell2ID, 3, v5->pMonsterInfo.uSpellSkillAndMastery2);
                             Actor::AI_Stand(v4, a2a, 0, &a4);
                             break;
                           default:
                             if ( !(rand() % 2) )
                               Actor::AI_Bored(v4, a2a, &a4);
                             else
                               Actor::AI_Stand(v4, a2a, 64,&a4);
                         }
                    }
        }

        }
        
    }
// 50FE08: using guessed type stru298 stru_50FE08;

//----- (0040680F) --------------------------------------------------------
void stru262_TurnBased::AI_Action_( int queue_index )
    {
    TurnBased_QueueElem *v2; // eax@1
    unsigned int v3; // eax@1
    unsigned int actor_id; // edi@2
    Actor *v5; // ebx@2
    AIDirection *v7; // esi@10
    int v8; // eax@10
    int v9; // ecx@10
    signed int v10; // eax@13
    int v14; // eax@29
    AIDirection a3; // [sp+Ch] [bp-44h]@10
    AIDirection v18; // [sp+28h] [bp-28h]@10
    int a2a; // [sp+44h] [bp-Ch]@2
    int v20; // [sp+48h] [bp-8h]@10
    TurnBased_QueueElem *v21; // [sp+4Ch] [bp-4h]@1
    signed int v22; // [sp+58h] [bp+8h]@10

    v2 = &pQueue[queue_index];
    v21 = v2;
    v2->uActionLength = 0;
    v3 = v2->uPackedID;
     if (PID_TYPE(pQueue[queue_index].uPackedID) == OBJECT_Actor)
        {
   
        actor_id = PID_ID(pQueue[queue_index].uPackedID);
        a2a = v3;
        v5 = &pActors[actor_id];
        v3 = v5->uAIState;
        if (!(v3 == AIState::Dying || v3 == AIState::Dead ||  v3 == AIState::Summoned ||
            v3 == AIState::Disabled || v3 == AIState::Removed))
            {
                
                Actor::_SelectTarget(actor_id, &ai_near_actors_targets_pid[actor_id], true);
                v22 = ai_near_actors_targets_pid[actor_id];
                if ( v5->pMonsterInfo.uHostilityType && !v22)
                    v5->pMonsterInfo.uHostilityType = MonsterInfo::Hostility_Friendly;
                v7 = Actor::GetDirectionInfo(PID(OBJECT_Actor,actor_id), v22, &a3, 0);
                v8 = v5->uActorRadius;
                memcpy(&a3, v7, sizeof(AIDirection));
                memcpy(&v18, &a3, sizeof(AIDirection));
                v9 = a3.uDistance - v8;
                v20 = a3.uDistance - v8;
                if ( v20 < 0 )
                    {
                    v9 = 0;
                    v20 = 0;
                    }

                if (PID_TYPE(v22) == OBJECT_Actor)
                    //v10 = (unsigned __int8)*(&byte_5C8D1A[89 * (pMonsterStats->pInfos[pActors[PID_ID(v22)].pMonsterInfo.uID].uID - 1) / 3] + (v5->pMonsterInfo.uID - 1) / 3);
                    v10 = pFactionTable->relations[(pMonsterStats->pInfos[pActors[PID_ID(v22)].pMonsterInfo.uID].uID) / 3 + 1][(v5->pMonsterInfo.uID - 1) / 3 + 1];
                else
                    v10 = 4;

               switch (v10)
                   {
               case 1:
                    if ( (double)(signed int)v20 < 307.2 )
                        v5->pMonsterInfo.uHostilityType = MonsterInfo::Hostility_Long;
                   break;
               case 2:
                   if ( v20 < 1024 )
                       v5->pMonsterInfo.uHostilityType = MonsterInfo::Hostility_Long;
                   break;
               case 3:
                   if ( v20 < 2560 )
                       v5->pMonsterInfo.uHostilityType = MonsterInfo::Hostility_Long;
                   break;
               case 4:
                   if ( v20 < 5120 )
                       v5->pMonsterInfo.uHostilityType = MonsterInfo::Hostility_Long;
                   break;
                   }
                
                if ( v5->pMonsterInfo.uHostilityType == 4 && v22 && (signed int)v9 < 5120 )
                    {
                    v14 = stru_50C198.special_ability_use_check(v5, actor_id);
                    v21->AI_action_type = TE_AI_STAND;
                    switch (v14)
                        {
                    case 1:
                        if ( v5->pMonsterInfo.uMissleAttack2Type )
                            {
                            Actor::AI_MissileAttack2(actor_id, v22, &v18);
                            v21->AI_action_type = TE_AI_RANGED_ATTACK;
                            }   
                        break;
                    case 2:
                        if(v5->pMonsterInfo.uSpell1ID)
                            {
                             Actor::AI_SpellAttack1(actor_id, v22, &v18);
                            v21->AI_action_type = TE_AI_RANGED_ATTACK;
                            }        
                        break;
                    case 3:
                        if(v5->pMonsterInfo.uSpell2ID)
                            {
                            Actor::AI_SpellAttack2(actor_id, v22, &v18);
                            v21->AI_action_type = TE_AI_RANGED_ATTACK;
                            }
                            
                        break;
                    default:
                        if ( v5->pMonsterInfo.uMissleAttack1Type )
                            {
                            Actor::AI_MissileAttack1(actor_id, v22, &v18);
                            v21->AI_action_type = TE_AI_RANGED_ATTACK;
                            }
                        }
                     if (!v21->AI_action_type)
                        if ( (double)v20 < 307.2)
                            {
                            Actor::AI_MeleeAttack(actor_id, v22, &v18);
                            v21->AI_action_type = TE_AI_MELEE_ATTACK;
                            }
                        else
                            {
                            Actor::AI_Stand(actor_id, v22, 64, &v18);
                            v21->AI_action_type = TE_AI_STAND;
                            }             
            }
                else
                    {
                    Actor::AI_Stand(actor_id, v22, 64, &v18);
                    v21->AI_action_type = TE_AI_STAND;
                    }
               v21->uActionLength = v5->uCurrentActionLength;
        }
        }
    }

//----- (00406A63) --------------------------------------------------------
void stru262_TurnBased::ActorAISetMovementDecision()
    {
  
    AIDirection a3; // [sp+8h] [bp-44h]@5
    AIDirection v7; // [sp+24h] [bp-28h]@5
    unsigned int target_pid; // [sp+40h] [bp-Ch]@5
    int i;

    this->ai_turn_timer = 64;
    dword_50C994 = 0;
    uActiveCharacter = 0;
    for (i=0; i<uActorQueueSize; ++i )
        {
        if (PID_TYPE(pQueue[i].uPackedID) == OBJECT_Actor)
            {
            target_pid = ai_near_actors_targets_pid[PID_ID(pQueue[i].uPackedID)];
            memcpy(&v7, Actor::GetDirectionInfo(pQueue[i].uPackedID, target_pid, &a3, 0), sizeof(AIDirection));
            if ( !ActorMove(i) )
                Actor::AI_Stand(PID_ID(pQueue[i].uPackedID), target_pid, 32, &v7);
            }
        }   
    }
// 50C994: using guessed type int dword_50C994;

//----- (00406AFE) --------------------------------------------------------
void stru262_TurnBased::ActorAIStopMovement()
    {
    AIDirection a3; // [sp+4h] [bp-48h]@5
    AIDirection v7; // [sp+20h] [bp-2Ch]@5
    unsigned int target_pid; 
    int i;

    for (i=0; i<uActorQueueSize; ++i )
        {
        if (PID_TYPE(pQueue[i].uPackedID) == OBJECT_Actor)
            {
            target_pid = ai_near_actors_targets_pid[PID_ID(pQueue[i].uPackedID)];
            memcpy(&v7, Actor::GetDirectionInfo(pQueue[i].uPackedID, target_pid, &a3, 0), sizeof(AIDirection));
            Actor::AI_Stand(PID_ID(pQueue[i].uPackedID), target_pid, 32, &v7);
            pQueue[i].AI_action_type = TE_AI_STAND;
            pQueue[i].uActionLength = 0;
            }
        }   
    turn_stage = 2;
    ai_turn_timer = 100;

    }

//----- (00406B9F) --------------------------------------------------------
void stru262_TurnBased::ActorAIDoAdditionalMove()
    {
    Actor *v6; // ebx@5
    AIDirection a3; // [sp+0h] [bp-50h]@15
    AIDirection v9; // [sp+1Ch] [bp-34h]@15
    unsigned int v13; // [sp+44h] [bp-Ch]@8
    int i;

    for (i=0; i<uActorQueueSize; ++i )
        {
        if (PID_TYPE(pQueue[i].uPackedID) == OBJECT_Actor)
            {
            v6 = &pActors[PID_ID(pQueue[i].uPackedID)];
            if ( !(v6->pActorBuffs[5].uExpireTime > 0|| (v6->pActorBuffs[6].uExpireTime > 0) || 
                    v6->uAIState == AIState::Dead || v6->uAIState == AIState::Removed || v6->uAIState == AIState::Disabled) )
                {
                v13 = ai_near_actors_targets_pid[PID_ID(pQueue[i].uPackedID)];
                memcpy(&v9, Actor::GetDirectionInfo(pQueue[i].uPackedID, v13, &a3, 0), sizeof(AIDirection));
                if ( v6->uAIState == AIState::Pursuing || v6->uAIState == AIState::Tethered ) 
                    {
                    if ( (double)(signed int)v9.uDistance < 307.2 )
                        Actor::AI_Stand(PID_ID(pQueue[i].uPackedID), v13, 32, &v9);
                    }
                else
                    {           
                    v6->uCurrentActionTime += pEventTimer->uTimeElapsed;
                    if ( v6->uCurrentActionTime > v6->uCurrentActionLength )
                        {
                        if ( v6->uAIState == AIState::Dying )
                            {
                            v6->uCurrentActionTime = 0;
                            v6->uCurrentActionLength = 0;
                            v6->uAIState = AIState::Dead;
                            v6->UpdateAnimation();
                            }
                        if ( !ActorMove(i) )
                            Actor::AI_Stand(PID_ID(pQueue[i].uPackedID), v13, 32, &v9);
                        }
                    }
                }
            }
        }
    }

//----- (00406D10) --------------------------------------------------------
bool stru262_TurnBased::ActorMove(signed int queue_position)
    {
    int v2; // ecx@1
    //int v3; // ecx@2
    Actor *actor; // ebx@2

    AIDirection *v9; // esi@10
    int v10; // eax@10
    int v11; // ecx@10
    unsigned __int8 pHostileType; // al@12
    AIDirection a3; // [sp+Ch] [bp-48h]@10
    AIDirection pDir; // [sp+28h] [bp-2Ch]@10
    int v28; // [sp+48h] [bp-Ch]@10
    TurnBased_QueueElem *v29; // [sp+4Ch] [bp-8h]@7
    unsigned int uActorID; // [sp+50h] [bp-4h]@2
    unsigned int a2a; // [sp+5Ch] [bp+8h]@7

    // __debugbreak();//срабатывает при пошаговом режиме после пяти шагов
    v2 = pQueue[queue_position].uPackedID;
    if (PID_TYPE(v2) == OBJECT_Player)
        return 0;
    uActorID = PID_ID(v2);
    //uActorID = v3;
    actor = &pActors[uActorID];
    //v5 = v4->uAIState;
    if ( actor->uAIState == AIState::Dead || actor->uAIState ==  AIState::Dying || 
         actor->uAIState == AIState::Removed|| actor->uAIState == AIState::Disabled || 
         actor->uAIState == AIState::Summoned  )
        return 1;
    v29 = &pTurnEngine->pQueue[queue_position];
    a2a = ai_near_actors_targets_pid[uActorID];
    Actor::_SelectTarget(uActorID, &ai_near_actors_targets_pid[uActorID], true);
    if ( actor->pMonsterInfo.uHostilityType && !ai_near_actors_targets_pid[uActorID] )
        actor->pMonsterInfo.uHostilityType = MonsterInfo::Hostility_Friendly;
    v9 = Actor::GetDirectionInfo(pQueue[queue_position].uPackedID, ai_near_actors_targets_pid[uActorID], &a3, 0);
    v10 = actor->uActorRadius;
    memcpy(&a3, v9, sizeof(AIDirection));
    memcpy(&pDir, &a3, sizeof(AIDirection));
    v11 = a3.uDistance - v10;
    v28 = a3.uDistance - v10;
    if ( v28 < 0 )
        {
        v11 = 0;
        v28 = 0;
        }
    pHostileType = actor->pMonsterInfo.uHostilityType;
   
    switch (pHostileType)
        {
    case 1:
        if ( (double)v28 < 307.2 )
            actor->pMonsterInfo.uHostilityType = MonsterInfo::Hostility_Long;
        break;
    case 2:
        if ( v28 < 1024 )
            actor->pMonsterInfo.uHostilityType = MonsterInfo::Hostility_Long;
        break;
    case 3:
        if ( v28 < 2560 )
            actor->pMonsterInfo.uHostilityType = MonsterInfo::Hostility_Long;
        break;
        }

    if ( actor->pActorBuffs[4].uExpireTime > 0 )
        {
        if (v11 < 10240 )
            {
            Actor::AI_Flee(uActorID, a2a, 0, &pDir);
            v29->AI_action_type = 4;
            }
        else
            {
             Actor::AI_RandomMove(uActorID, a2a, 1024, 0);
             v29->AI_action_type = TE_AI_PURSUE;
            }         
    
        v29->uActionLength = actor->uCurrentActionLength;
        return 1;
        }

    if ( actor->pMonsterInfo.uHostilityType == MonsterInfo::Hostility_Long )
  {     
    if ( !(actor->uAttributes & 0x020000) || actor->pMonsterInfo.uAIType == 1 )
        {
        if ( actor->pMonsterInfo.uAIType == 1 )
            {
            if ( actor->pMonsterInfo.uMovementType == MONSTER_MOVEMENT_TYPE_STAIONARY )
                Actor::AI_Stand(uActorID, a2a, 32, 0);
            else
                Actor::AI_Flee(uActorID, a2a, 32, 0);
            v29->AI_action_type = TE_AI_FLEE;
            v29->uActionLength = actor->uCurrentActionLength;
            return 1;
            }
        if ( actor->pMonsterInfo.uAIType == 2 )
            {
           
            if (((double)actor->pMonsterInfo.uHP * 0.2) > (double)actor->sCurrentHP && (v11 < 10240 ) )
                {
                if ( actor->pMonsterInfo.uMovementType == MONSTER_MOVEMENT_TYPE_STAIONARY )
                    Actor::AI_Stand(uActorID, a2a, 32, 0);
                else
                     Actor::AI_Flee(uActorID, a2a, 32, 0);
                v29->AI_action_type = TE_AI_FLEE;
                v29->uActionLength = actor->uCurrentActionLength;
                return 1;
                }
            }
        if ( actor->pMonsterInfo.uAIType == 3 )
            {

            if ( ((double)actor->pMonsterInfo.uHP * 0.1) > (double)actor->sCurrentHP && (v11 < 10240 ))
                {
                if ( actor->pMonsterInfo.uMovementType == MONSTER_MOVEMENT_TYPE_STAIONARY )
                    Actor::AI_Stand(uActorID, a2a, 32, 0);
                else
                     Actor::AI_Flee(uActorID, a2a, 32, 0);
                v29->AI_action_type = TE_AI_FLEE;
                v29->uActionLength = actor->uCurrentActionLength;
                return 1;
                }

            }
        }

    if ( (double)(signed int)v28 < 307.2 )
        return 0;
    if ( (signed int)v11 < 5120 )
        {
        if ( actor->pMonsterInfo.uMissleAttack1Type && (signed int)v11 < 1024 )
            Actor::AI_Pursue1(uActorID, a2a, uActorID, 32, &pDir);
        else
            Actor::AI_Pursue2(uActorID, a2a, 32, &pDir, 307);
        v29->AI_action_type = TE_AI_PURSUE;
        v29->uActionLength = actor->uCurrentActionLength;
        return 1;
        }
        }
    switch(actor->pMonsterInfo.uMovementType)
        {
        case MONSTER_MOVEMENT_TYPE_SHORT: 
            Actor::AI_RandomMove(uActorID, a2a, 1024, 32);
            break;
        case MONSTER_MOVEMENT_TYPE_MEDIUM:
             Actor::AI_RandomMove(uActorID, a2a, 2560, 32);
             break;
         case MONSTER_MOVEMENT_TYPE_LONG:
             Actor::AI_RandomMove(uActorID, a2a, 5120, 32);
             break;
         case MONSTER_MOVEMENT_TYPE_FREE:
             Actor::AI_RandomMove(uActorID, a2a, 10240, 32);
             break;
         case MONSTER_MOVEMENT_TYPE_STAIONARY:
             Actor::AI_Stand(uActorID, a2a, 32, 0);
              break;
         default:
             return 1;
        }
    v29->AI_action_type = TE_AI_PURSUE;
    v29->uActionLength = actor->uCurrentActionLength;
    return 1;
    }

    //----- (00406FA8) --------------------------------------------------------
void stru262_TurnBased::ActorAIChooseNewTargets()
    {
    Actor *curr_acror; // ebx@4
    AIDirection a3; // [sp+Ch] [bp-6Ch]@8
    AIDirection v9; // [sp+28h] [bp-50h]@8
    AIDirection a4; // [sp+44h] [bp-34h]@8
    unsigned int target_pid; // [sp+60h] [bp-18h]@1
    int uActorID; // [sp+68h] [bp-10h]@4
    int i;

    for (i=0; i<uActorQueueSize; ++i )
        {
        if (PID_TYPE(pQueue[i].uPackedID) == OBJECT_Actor)
            {
            uActorID=PID_ID(pQueue[i].uPackedID);
            curr_acror = &pActors[uActorID];
            if ( !( curr_acror->uAIState == AIState::Summoned|| curr_acror->uAIState == AIState::Dead ||
                    curr_acror->uAIState == AIState::Removed || curr_acror->uAIState == AIState::Disabled) )
                { 
                target_pid = ai_near_actors_targets_pid[uActorID];      
                Actor::_SelectTarget(uActorID, &ai_near_actors_targets_pid[uActorID], true);  
                memcpy(&v9, Actor::GetDirectionInfo(pQueue[i].uPackedID, target_pid, &a3, 0), sizeof(AIDirection));
                memcpy(&a4, &v9, sizeof(AIDirection));     
                curr_acror->uCurrentActionTime += pEventTimer->uTimeElapsed;
                if ( curr_acror->uCurrentActionTime > curr_acror->uCurrentActionLength )
                    {
                    if ( curr_acror->uAIState == AIState::Dying )
                        {
                        curr_acror->uCurrentActionTime = 0;
                        curr_acror->uCurrentActionLength = 0;
                        curr_acror->uAIState = AIState::Dead;
                        curr_acror->UpdateAnimation();
                        break;
                        }
                    if ( rand() % 2 )
                        Actor::AI_Stand(uActorID, target_pid, 64, &a4);
                    else
                        Actor::AI_Bored(uActorID, target_pid, &a4);
                    }
                }
            }
        }
    }

