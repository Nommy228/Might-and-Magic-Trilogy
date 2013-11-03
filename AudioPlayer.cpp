#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <string>

#include "mm7_data.h"
#include "VideoPlayer.h"
#include "AudioPlayer.h"
#include "FrameTableInc.h"
#include "Indoor.h"
#include "SpriteObject.h"
#include "Party.h"
#include "Actor.h"
#include "Game.h"
#include "DecorationList.h"
#include "Time.h"
#include "OSInfo.h"
#include "Math.h"
#include "MapInfo.h"
#include "GUIWindow.h"
#include "Log.h"
#include "ErrorHandling.h"
#include "Level/Decoration.h"

#include "Bink_Smacker.h"

#include "MM7.h"


PCMWAVEFORMAT pcmWaveFormat;

int Aureal3D_SplashScreen;
int Aureal3D_SplashAudio;
int uFindSound_BinSearch_ResultID; // weak
int uLastLoadedSoundID; // weak
int sLastTrackLengthMS;
std::array<Sound, 3000> pSounds;
AudioPlayer *pAudioPlayer;
SoundList *pSoundList;


std::array<stru339_spell_sound, 4> stru_A750F8;
std::array<stru339_spell_sound, 4> stru_AA1058;



unsigned __int8 uSoundVolumeMultiplier = 4;
unsigned __int8 uVoicesVolumeMultiplier = 4;
unsigned __int8 uMusicVolimeMultiplier = 4;
int bWalkSound; // idb

std::array<float, 10> pSoundVolumeLevels = 
{
  0.0000000f, 0.1099999f, 0.2199999f, 0.3300000f, 0.4399999f,
 0.5500000f, 0.6600000f, 0.7699999f, 0.8799999f, 0.9700000f     //changed 0.9900000f to 0.9700000f. for some reason it only works for values below this
};




void ReleaseSoundData(void *_this);




//----- (004A9953) --------------------------------------------------------
void SoundList::Initialize()
{
  SoundDesc *pSoundDesc; // eax@5
  void *pSoundData; // ebx@7
  unsigned int uSoundSize; // eax@7
  char *pSoundBytes; // ebx@7
  AILFILETYPE pType; // eax@7
  int v8; // eax@8
  char pSoundName[120]; // [sp+4h] [bp-A4h]@4
  AILSOUNDINFO pInfo; // [sp+7Ch] [bp-2Ch]@10
  int v12; // [sp+A0h] [bp-8h]@12

  if ( sNumSounds > 1 )
  {
    for ( uint i = 1; i < pSoundList->sNumSounds; ++i )
    {
      sprintf(pSoundName, "%s", pSounds[i].pSoundName);
      pSoundDesc = &pSoundList->pSounds[i];
      if ( pSoundList->pSounds[i].eType != SOUND_DESC_SYSTEM || (sprintf(pSoundName, "%s", pSounds[i].pSoundName),
            pSoundList->pSounds[i].pSoundData[0] = ::LoadSound(pSoundName, (SoundData *)0xFFFFFFFF, pSounds[i].uSoundID),
            !pAudioPlayer->b3DSoundInitialized)
        || (pSoundDesc = &pSoundList->pSounds[i], !(pSoundDesc->uFlags & SOUND_DESC_SWAP))
        || !pSoundDesc->pSoundData[0] )
        continue;
      pSoundData = pSoundDesc->pSoundData[0];
      uSoundSize = *(int *)pSoundData;
      pSoundBytes = (char *)pSoundData + 4;
      pType = AIL_file_type(pSoundBytes, uSoundSize);
      if ( !pType )
      {
        pSoundList->pSounds[i].bDecompressed = false;
        pSoundList->UnloadSound(i, 1);
        continue;
      }
      v8 = pType - 1;
      if ( v8 )
      {
        if ( v8 == 1 )
        {
          if ( AIL_WAV_info(pSoundBytes, &pInfo) && pInfo.uChannels != 2 )
          {
            if ( !AIL_decompress_ADPCM(&pInfo, &pSoundList->pSounds[i].p3DSound, &v12) )
            {
              pSoundList->pSounds[i].p3DSound = 0;
              pSoundList->pSounds[i].bDecompressed = true;
            }
          }
          pSoundList->UnloadSound(i, 1);
          continue;
        }
        pSoundList->pSounds[i].bDecompressed = false;
        pSoundList->UnloadSound(i, 1);
        continue;
      }
      pSoundList->pSounds[i].p3DSound = pSoundList->pSounds[i].pSoundData[0];
      pSoundList->UnloadSound(i, 1);
    }
  }
}

//----- (004A9A67) --------------------------------------------------------
__int16 SoundList::LoadSound(int a1, unsigned int a3)
{
  AILSOUNDINFO v24; // [sp+84h] [bp-28h]@23

  if (bNoSound || !sNumSounds)
    return 0;

  uint       uSoundIdx = 0;
  SoundDesc *pSound = nullptr;
  for (uint i = 1; i < sNumSounds; ++i)
    if (pSounds[i].uSoundID == a1)
    {
      uSoundIdx = i;
      pSound = &pSounds[i];
      break;
    }
  if (!pSound)
    return 0;

  if (pSound->uFlags & SOUND_DESC_SWAP && pSound->p3DSound ||
      ~pSound->uFlags & SOUND_DESC_SWAP && pSound->pSoundData[0])
    return uSoundIdx;

  if (!pSound->pSoundData[0])
    pSound->pSoundData[0] = ::LoadSound(pSound->pSoundName, (SoundData *)0xFFFFFFFF, pSound->uSoundID);

  if (!pSound->pSoundData[0])
    return 0;

  if (a3)
    pSound->uFlags |= SOUND_DESC_SYSTEM;

  if (!pAudioPlayer->b3DSoundInitialized)
    return uSoundIdx;

  if (~pSound->uFlags & SOUND_DESC_SWAP || !pSound->pSoundData[0])
    return uSoundIdx;


  SoundData* pSoundData = pSound->pSoundData[0];
  switch (AIL_file_type((void *)pSoundData->pData, pSoundData->uDataSize))
  {
    default:
    case AILFILETYPE_UNKNOWN:
      pSound->bDecompressed = false;
      return 0;

    case AILFILETYPE_PCM_WAV:
      pSound->p3DSound = pSound->pSoundData[0];
      return uSoundIdx;

    case AILFILETYPE_ADPCM_WAV:
      if (AIL_WAV_info((void *)pSoundData->pData, &v24) && v24.uChannels != 2)
      {
        if (!AIL_decompress_ADPCM(&v24, &pSound->p3DSound, &a1) )
        {
          pSound->p3DSound = nullptr;
          pSound->bDecompressed = true;
          UnloadSound(uSoundIdx, 0);
        }
      }
      return uSoundIdx;
  };
}

//----- (004A9BBD) --------------------------------------------------------
int SoundList::LoadSound(unsigned int a2, LPVOID lpBuffer, int uBufferSizeLeft, int *pOutSoundSize, int a6)
{
  //SoundList *v6; // edi@1
  signed int v7; // esi@1
  //unsigned __int8 v8; // zf@1
  //unsigned __int8 v9; // sf@1
  SoundDesc *v10; // eax@3
  SoundDesc *v11; // ecx@3
  int v12; // ebx@7
  int result; // eax@13
  SoundHeader *v14; // esi@16
  DWORD *v15; // edi@16
  int v16; // ebx@16
  unsigned int v17; // eax@18
  void *v18; // ebx@19
  std::string v19; // [sp-18h] [bp-38h]@22
  const char *v20; // [sp-8h] [bp-28h]@22
  int v21; // [sp-4h] [bp-24h]@22
  //SoundList *v22; // [sp+Ch] [bp-14h]@1
  SoundDesc *Args; // [sp+10h] [bp-10h]@3
  unsigned int v24; // [sp+14h] [bp-Ch]@2
  int v25; // [sp+18h] [bp-8h]@2
  int v26; // [sp+1Ch] [bp-4h]@1

  //v6 = this;
  v7 = 0;
  //v22 = this;
  //v8 = this->uNumSounds == 0;
  //v9 = (this->uNumSounds & 0x80000000u) != 0;
  v26 = 0;
  if (!sNumSounds)
    return 0;


    v24 = 0;
    v25 = 44;
    while ( 1 )
    {
      v10 = pSounds;
      v11 = &v10[v24 / 0x78];
      Args = &v10[v24 / 0x78];
      if ( a2 == v10[v24 / 0x78].uSoundID )
      {
        if ( a6 == v7 && *(int *)&v10->pSoundName[v25] != v7 )
          return v26;
        if ( (signed int)pAudioPlayer->uNumSoundHeaders > v7 )
          break;
      }
LABEL_12:
      ++v26;
      v25 += 120;
      v24 += 120;
      if ( v26 >= (signed int)sNumSounds )
        return 0;
    }
    v12 = 0;
    while ( _stricmp(pAudioPlayer->pSoundHeaders[v12].pSoundName, v11->pSoundName) )
    {
      ++v7;
      ++v12;
      if ( v7 >= (signed int)pAudioPlayer->uNumSoundHeaders )
      {
        v7 = 0;
        goto LABEL_12;
      }
      v11 = Args;
    }
    v14 = &pAudioPlayer->pSoundHeaders[v7];
    v15 = (DWORD *)&v14->uDecompressedSize;
    a2 = v14->uDecompressedSize;
    v16 = a2;
    if ( (signed int)a2 > uBufferSizeLeft )
      Error("Sound %s is size %i bytes, sound buffer size is %i bytes", Args, a2, uBufferSizeLeft);

    SetFilePointer(pAudioPlayer->hAudioSnd, v14->uFileOffset, 0, 0);
    v17 = v14->uCompressedSize;
    if ( (signed int)v17 >= (signed int)*v15 )
    {
      if ( v17 == *v15 )
      {
        ReadFile(pAudioPlayer->hAudioSnd, lpBuffer, *v15, (LPDWORD)&Args, 0);
      }
      else
      {
      MessageBoxW(nullptr, L"Can't load sound file!", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Sound.cpp:666", 0);
      }
    }
    else
    {
      v18 = malloc(v14->uCompressedSize);
      ReadFile(pAudioPlayer->hAudioSnd, v18, v14->uCompressedSize, (LPDWORD)&Args, 0);
      zlib::MemUnzip(lpBuffer, &v14->uDecompressedSize, v18, v14->uCompressedSize);
      free(v18);
      v16 = a2;
    }
    result = v26;
    pSounds->pSoundData[a6 + 30 * v26] = (SoundData *)lpBuffer;
    *pOutSoundSize = v16;

  return result;
}

//----- (004A9D3E) --------------------------------------------------------
SoundDesc *SoundList::Release()
{
  SoundDesc *result; // eax@3
  void *v5; // ecx@3

  if ( (signed int)this->sNumSounds > 0 )
  {
    for ( uint i = 0; i < (signed int)this->sNumSounds; ++i )
    {
      result = this->pSounds;
      v5 = result[i].pSoundData[0];
      if ( v5 )
      {
        ReleaseSoundData(v5);
        this->pSounds[i].pSoundData[0] = 0;
        result = (SoundDesc *)((char *)&this->pSounds[i] + 40);
        *(int *)&result->pSoundName[0] &= 0xFFFFFFFEu;
      }
    }
  }
  return result;
}

//----- (004A9D79) --------------------------------------------------------
void SoundList::_4A9D79(int a2)
{
  SoundList *v2; // esi@1
  int v3; // edi@1
  signed int i; // ebx@1
  SoundDesc *v5; // eax@2
  SOUND_DESC_TYPE v6; // ecx@2
  void *v7; // eax@5

  v2 = this;
  v3 = 0;
  for ( i = 0; i < (signed int)v2->sNumSounds; ++v3 )
  {
    v5 = &v2->pSounds[v3];
    v6 = v5->eType;
    if ( v6 != SOUND_DESC_SYSTEM && (a2 || v6 != SOUND_DESC_LOCK) )
    {
      v7 = v5->pSoundData[0];
      if ( v7 )
      {
        ReleaseSoundData(v7);
        v2->pSounds[v3].pSoundData[0] = 0;
      }
      v2->pSounds[v3].uFlags &= ~SOUND_DESC_SYSTEM;
    }
    ++i;
  }
}

//----- (004A9DCD) --------------------------------------------------------
void SoundList::UnloadSound(unsigned int uSoundID, char a3)
{
  if ( pSounds[uSoundID].eType != SOUND_DESC_SYSTEM )
  {
    if ( (pSounds[uSoundID].uFlags & SOUND_DESC_SWAP) && pSounds[uSoundID].p3DSound && a3 )
    {
      if ( pSounds[uSoundID].bDecompressed)
        AIL_mem_free_lock(pSounds[uSoundID].p3DSound);
      pSounds[uSoundID].p3DSound = 0;
      pSounds[uSoundID].uFlags &= ~SOUND_DESC_SYSTEM;
    }
    if ( pSounds[uSoundID].pSoundData[0] )
    {
      ReleaseSoundData(pSounds[uSoundID].pSoundData[0]);
      pSounds[uSoundID].pSoundData[0] = NULL;
      pSounds[uSoundID].uFlags &= ~SOUND_DESC_SYSTEM;
    }
  }
}


//----- (004A9E3D) --------------------------------------------------------
void SoundList::ToFile()
{
  SoundList *v1; // esi@1
  FILE *v2; // eax@1
  FILE *v3; // edi@1

  v1 = this;
  v2 = fopen("data\\dsounds.bin", "wb");
  v3 = v2;
  if ( !v2 )
    Error("Unable to save dsounds.bin!");

  fwrite(v1, 4u, 1u, v2);
  fwrite(v1->pSounds, 0x78u, v1->sNumSounds, v3);
  fclose(v3);
}

//----- (004A9E89) --------------------------------------------------------
void SoundList::FromFile(void *data_mm6, void *data_mm7, void *data_mm8)
{
  uint num_mm6_sounds = data_mm6 ? *(int *)data_mm6 : 0,
       num_mm7_sounds = data_mm7 ? *(int *)data_mm7 : 0,
       num_mm8_sounds = data_mm8 ? *(int *)data_mm8 : 0;

  sNumSounds = num_mm6_sounds + num_mm7_sounds + num_mm8_sounds;
  assert(sNumSounds);
  assert(!num_mm8_sounds);

  pSounds = (SoundDesc *)malloc(sNumSounds * sizeof(SoundDesc));
  memcpy(pSounds, (char *)data_mm7 + 4, num_mm7_sounds * sizeof(SoundDesc));
  for (uint i = 0; i < num_mm6_sounds; ++i)
  {
    auto src = (SoundDesc_mm6 *)((char *)data_mm6 + 4) + i;
    SoundDesc* dst = &pSounds[num_mm7_sounds + i];

    memcpy(dst, src, sizeof(SoundDesc_mm6));
    dst->p3DSound = nullptr;
    dst->bDecompressed = false;
  }
}

//----- (004A9ED0) --------------------------------------------------------
int SoundList::FromFileTxt(const char *Args)
{
  SoundList *v2; // ebx@1
  __int32 v3; // edi@1
  FILE *v4; // eax@1
  unsigned int v5; // esi@3
  void *v6; // eax@9
  FILE *v7; // ST0C_4@11
  char *i; // eax@11
  int v9; // eax@14
  const char *v10; // ST0C_4@14
  int v11; // eax@18
  SoundDesc *v12; // ecx@18
  char v13; // zf@18
  unsigned int v14; // eax@18
  int v15; // eax@23
  char Buf; // [sp+Ch] [bp-2F0h]@3
  FrameTableTxtLine v18; // [sp+200h] [bp-FCh]@4
  FrameTableTxtLine v19; // [sp+27Ch] [bp-80h]@4
  FILE *File; // [sp+2F8h] [bp-4h]@1
  unsigned int Argsa; // [sp+304h] [bp+8h]@3

  v2 = this;
  free(this->pSounds);
  v3 = 0;
  v2->pSounds = 0;
  v2->sNumSounds = 0;
  v4 = fopen(Args, "r");
  File = v4;
  if ( !v4 )
    Error("SoundListClass::load - Unable to open file: %s.");

  v5 = 0;
  Argsa = 0;
  if ( fgets(&Buf, 490, v4) )
  {
    do
    {
      *strchr(&Buf, 10) = 0;
      memcpy(&v19, txt_file_frametable_parser(&Buf, &v18), sizeof(v19));
      if ( v19.uPropCount && *v19.pProperties[0] != 47 )
        ++Argsa;
    }
    while ( fgets(&Buf, 490, File) );
    v5 = Argsa;
    v3 = 0;
  }
  v2->sNumSounds = v5;
  v6 = malloc(120 * v5);
  v2->pSounds = (SoundDesc *)v6;
  if ( v6 == (void *)v3 )
    Error("SoundListClass::load - Out of Memory!");

  memset(v6, v3, 120 * v2->sNumSounds);
  v7 = File;
  v2->sNumSounds = v3;
  fseek(v7, v3, v3);
  for ( i = fgets(&Buf, 490, File); i; i = fgets(&Buf, 490, File) )
  {
    *strchr(&Buf, 10) = 0;
    memcpy(&v19, txt_file_frametable_parser(&Buf, &v18), sizeof(v19));
    if ( v19.uPropCount && *v19.pProperties[0] != 47 )
    {
      sprintf(v2->pSounds[v2->sNumSounds].pSoundName, "%s", v19.pProperties[0]);
      v9 = atoi(v19.pProperties[1]);
      v10 = v19.pProperties[2];
      v2->pSounds[v2->sNumSounds].uSoundID = v9;
      if ( _stricmp(v10, "system") )
      {
        if ( _stricmp(v19.pProperties[2], "swap") )
        {
          v11 = _stricmp(v19.pProperties[2], "lock");
          v12 = v2->pSounds;
          v13 = v11 == 0;
          v14 = v2->sNumSounds;
          if ( v13 )
            v12[v14].eType = SOUND_DESC_LOCK;
          else
            v12[v14].eType = SOUND_DESC_LEVEL;
        }
        else
        {
          v2->pSounds[v2->sNumSounds].eType = SOUND_DESC_SWAP;
        }
      }
      else
      {
        v2->pSounds[v2->sNumSounds].eType = SOUND_DESC_SYSTEM;
      }
      if ( v19.uPropCount >= 4 && !_stricmp(v19.pProperties[3], "3D") )
      {
        v15 = (int)&v2->pSounds[v2->sNumSounds].uFlags;
        *(int *)v15 |= SOUND_DESC_SWAP;
      }
      ++v2->sNumSounds;
    }
  }
  fclose(File);
  return 1;
}

//----- (004AA13F) --------------------------------------------------------
void AudioPlayer::PlayMusicTrack(MusicID eTrack)
{
  if (!bNoSound && bPlayerReady && hAILRedbook && uMusicVolimeMultiplier)
  {
    AIL_redbook_stop(hAILRedbook);
    AIL_redbook_set_volume(hAILRedbook, (signed)pSoundVolumeLevels[uMusicVolimeMultiplier] * 64.0f);
    AIL_redbook_track_info(hAILRedbook, eTrack, &uCurrentMusicTrackStartMS, &uCurrentMusicTrackEndMS);
    AIL_redbook_play(hAILRedbook, uCurrentMusicTrackStartMS + 1, uCurrentMusicTrackEndMS);
    uCurrentMusicTrackLength = ((uCurrentMusicTrackEndMS - uCurrentMusicTrackStartMS) * 128) / 1000;
  }
}


//----- (004AA1F3) --------------------------------------------------------
void AudioPlayer::SetMusicVolume(int vol)
{
  if (bPlayerReady)
  {
    if (hAILRedbook)
      AIL_redbook_set_volume(hAILRedbook, vol);
  }
}

//----- (004AA214) --------------------------------------------------------
void AudioPlayer::SetMasterVolume(float fVolume)
{
  if ( bPlayerReady )
  {
    uMasterVolume = fVolume;
    if ( hDigDriver )
      AIL_set_digital_master_volume(hDigDriver, fVolume);
    if ( b3DSoundInitialized )
      s3DSoundVolume = fVolume * 0.5f;
  }
}
// 4D8304: using guessed type int __stdcall AIL_set_digital_master_volume(int, int);

//----- (004AA258) --------------------------------------------------------
void AudioPlayer::_4AA258(int a2)
{
  if (!bPlayerReady)
    return;

    if ( this->b3DSoundInitialized && a2 && this->uNum3DSamples > 0 )
    {
      for ( uint i = 0; i < this->uNum3DSamples; ++i )
      {
        if ( this->p3DSamples[i].field_4 == a2 && AIL_3D_sample_status(this->p3DSamples[i].hSample) == 4 )
          AIL_end_3D_sample(this->p3DSamples[i].hSample);
      }
      goto LABEL_9;
    }
    else
    {
LABEL_9:
      if ( this->hDigDriver )
      {
        if ( a2 && this->uMixerChannels > 0 )
        {
          for ( uint i = 0; i < this->uMixerChannels; ++i )
          {
            if ( this->pMixerChannels[i].source_pid == a2 )
            {
              if ( AIL_sample_status(this->pMixerChannels[i].hSample) == AIL::Sample::Playing)
              {
                AIL_end_sample(this->pMixerChannels[i].hSample);
                FreeChannel(&this->pMixerChannels[i]);
              }
            }
          }
        }
      }
    }
}

//----- (004AA306) --------------------------------------------------------
void AudioPlayer::PlaySound(SoundID eSoundID, signed int a3, unsigned int uNumRepeats, signed int PartyX, signed int PartyY, int a7, unsigned int uVolume, int sPlaybackRate)
{
  //AudioPlayer *pAudioPlayer1; // esi@1
  //signed int pNum; // edx@5
  //int *pSoundID; // ecx@6
  int v12; // edi@13
  signed int v13; // ecx@17
  signed int v14; // eax@20
  int v15; // eax@24
  signed int v16; // eax@25
  SpriteObject *pLayingItem; // eax@28
  signed int v18; // eax@29
  Actor *pActor1; // eax@32
  signed int v20; // ecx@32
  double v21; // st7@32
  signed int v22; // ecx@33
  AudioPlayer_3DSample *pAudioPlayer_3DSample; // esi@53
  AudioPlayer_3DSample *pAudioPlayer_3DSample1; // esi@61
  int v25; // esi@67
  double v26; // st7@68
  int v27; // ST18_4@68
  int v28; // ebx@68
  int v29; // eax@68
  AudioPlayer_3DSample *pAudioPlayer_3DSample2; // esi@69
  int v31; // ST18_4@70
  int v32; // ebx@70
  int v33; // eax@70
  int v34; // eax@70
  char v35; // zf@70
  signed int v36; // ebx@74
  //AudioPlayer *pAudioPlayer2; // edi@79
  AudioPlayer_3DSample *pAudioPlayer_3DSample3; // esi@79
  //unsigned int v39; // ebx@80
  int v40; // eax@81
  char *v41; // edi@82
  int v42; // esi@82
  double v43; // st7@91
  SpriteObject *pLayingItem2; // eax@92
  Actor *pActor; // eax@93
  signed int v46; // ecx@93
  double v47; // st7@93
  BLVDoor *pBLVDoor; // eax@97
  double v49; // st7@104
  int v50; // ST18_4@104
  int v51; // ebx@104
  int v52; // eax@104
  float v53; // ST0C_4@106
  float v54; // ST04_4@106
  //AudioPlayer *pAudioPlayer3; // edx@106
  SoundDesc *pSoundDesc; // edx@107
  SpriteObject *pLayingItem3; // eax@114
  //int v58; // edx@115
  //int v59; // ecx@115
  //Actor *pActor2; // eax@118
  //MixerChannel *pMixerChannel; // esi@126
  signed int v62; // esi@133
  //AudioPlayer *pAudioPlayer4; // ebx@133
  //MixerChannel *pMixerChannel2; // edi@134
  //int v65; // ebx@141
  //AudioPlayer *pAudioPlayer5; // ecx@142
  //MixerChannel *pMixerChannel3; // edi@142
  int v68; // eax@143
  //MixerChannel *pMixerChannel4; // edi@149
  //int v70; // ecx@152
  //SoundDesc *pSoundDesc2; // eax@153
  //int v72; // edi@156
  //MixerChannel *pMixerChannel5; // esi@157
  //unsigned int pVolume; // eax@157
  //int pPartyX; // ebx@159
  //int v76; // ebx@160
  //SpriteObject *pLayingItem4; // edi@164
  //unsigned int pPosY; // edx@166
  //unsigned int pPosX; // ecx@166
  //int v80; // eax@167
  //Actor *pActor3; // edi@168
  //BLVDoor *pBLVDoor2; // edi@173
  //int v83; // eax@183
  //int v84; // eax@183
  //_SAMPLE *v85; // ST18_4@186
  unsigned int v86; // [sp+14h] [bp-60h]@84
  //int v87; // [sp+14h] [bp-60h]@115
  RenderVertexSoft pRenderVertexSoft; // [sp+24h] [bp-50h]@1
  //unsigned int v89; // [sp+54h] [bp-20h]@12
  int v90; // [sp+58h] [bp-1Ch]@68
  float v91; // [sp+5Ch] [bp-18h]@68
  //unsigned int v92; // [sp+60h] [bp-14h]@10
  float v93; // [sp+64h] [bp-10h]@1
  signed int varC; // [sp+68h] [bp-Ch]@68
  //AudioPlayer *pAudioPlayer; // [sp+6Ch] [bp-8h]@1
  int v96; // [sp+70h] [bp-4h]@19
  signed int uNumRepeatsb; // [sp+84h] [bp+10h]@93
  float uNumRepeatsa; // [sp+84h] [bp+10h]@104
  float v99; // [sp+8Ch] [bp+18h]@104
  signed int v100; // [sp+90h] [bp+1Ch]@32
  int v101; // [sp+90h] [bp+1Ch]@52
  int v102; // [sp+90h] [bp+1Ch]@60
  int v103; // [sp+90h] [bp+1Ch]@68

  if (!bPlayerReady)
    return;
  if (!uSoundVolumeMultiplier)
    return;
  if (!hDigDriver)
    return;
  if (eSoundID == SOUND_Invalid)
    return;

  //pAudioPlayer = this;
  //v35 = this->bPlayerReady == 0;
  //pAudioPlayer = this;
  //LODWORD(v93) = 10000;
  int sample_volume = 10000;


  int sound_id = 0;
  for (uint i = 0; i < pSoundList->sNumSounds; ++i)
    if (pSoundList->pSounds[i].uSoundID == eSoundID)
    {
      sound_id = i;
      break;
    }

  if (!sound_id)
  {
    Log::Warning(L"SoundID = %u not found", eSoundID);
    return;
  }

  int start_channel = 0,
      end_channel = 0;

  assert(sound_id < pSoundList->sNumSounds);
  SoundDesc* sound_desc = pSoundList->pSounds + sound_id;
  if (!b3DSoundInitialized || sound_desc->Is3D())
  {
    if (!a3)  // generic sound like from UI
    {
      start_channel = 10;
      end_channel = 12;
      goto LABEL_133;
    }
    else if (a3 == -1)  // exclusive sounds - can override
    {
      start_channel = 13;
      end_channel = 13;
      goto LABEL_133;
    }
    else
    {
      if (a3 < 0)    // exclusive sounds - no override
      {
        start_channel = 14;
        end_channel = 14;
        goto LABEL_123;
      }
      else
      {
        int object_type = PID_TYPE(a3),
            object_id = PID_ID(a3);
        switch (object_type)
        {
          case OBJECT_BLVDoor:
          case OBJECT_Player:
          {
            start_channel = 10;
            end_channel = 12;
            goto LABEL_133;
          }
          break;

          case OBJECT_Actor:
          {
            start_channel = 0;
            end_channel = 3;

            assert(object_id < uNumActors);
            Actor* actor = &pActors[object_id];

            sample_volume = GetSoundStrengthByDistanceFromParty(actor->vPosition.x, actor->vPosition.y, actor->vPosition.z);
            if (!sample_volume)
              return;
            goto LABEL_123;
          }

          case OBJECT_Decoration:
          {
            start_channel = 4;
            end_channel = 4;

            assert(object_id < uNumLevelDecorations);
            LevelDecoration* decor = &pLevelDecorations[object_id];

            sample_volume = GetSoundStrengthByDistanceFromParty(decor->vPosition.x, decor->vPosition.y, decor->vPosition.z);
            if (!sample_volume)
              return;
            goto LABEL_123;
          }
          break;

          case OBJECT_Item:
          {
            start_channel = 5;
            end_channel = 7;

            assert(object_id < uNumSpriteObjects);
            SpriteObject* object = &pSpriteObjects[object_id];

            sample_volume = GetSoundStrengthByDistanceFromParty(object->vPosition.x, object->vPosition.y, object->vPosition.z);
            if (!sample_volume)
              return;
            goto LABEL_123;
          }
          break;

          case OBJECT_BModel:
          {
            start_channel = 8;
            end_channel = 9;
            goto LABEL_123;
          }
          break;

          default:
            assert(false);
        }
      }
    }
LABEL_123:
    for (uint i = 0; i < uMixerChannels; ++i)
    {
      MixerChannel* channel = &pMixerChannels[i];
      if (channel->source_pid == a3 &&
          AIL_sample_status(channel->hSample) == AIL::Sample::Playing)
      {
        if (channel->uSourceTrackIdx == sound_id)
          return;                          // already playing the same sound from the same source - return
        AIL_end_sample(channel->hSample);  // requested new sound from the same source - end & switch
        FreeChannel(channel);
      }
    }
LABEL_133:
      v62 = start_channel;
      //pAudioPlayer4 = pAudioPlayer;
      for (v62 = start_channel; v62 <= end_channel; ++v62)
      {
        MixerChannel* channel = pMixerChannels + v62;
        if (AIL_sample_status(channel->hSample) == AIL::Sample::Done)
        {
          AIL_end_sample(channel->hSample);
          if (channel->uSourceTrackIdx)
            FreeChannel(channel);
          break;
        }
      }
      /*if ( start_channel <= end_channel)
      {
        pMixerChannel2 = &pAudioPlayer->pMixerChannels[start_channel];
        while ( AIL_sample_status(pMixerChannel2->hSample) != AIL::Sample::Done)
        {
          ++v62;
          ++pMixerChannel2;
          if ( v62 > end_channel )
            goto LABEL_140;
        }
        AIL_end_sample(pMixerChannel2->hSample);
        if ( pMixerChannel2->uSourceTrackIdx )
          _4ABE55(pMixerChannel2);
      }
LABEL_140:*/

      if (v62 > end_channel)  // no free channel - occupy the quitest one
      {
        v62 = -1;

        int min_volume = sample_volume;
        for (uint i = start_channel; i <= end_channel; ++i)
        {
          MixerChannel* channel = &pMixerChannels[i];

          int volume = AIL_sample_volume(channel->hSample);
          if (volume < min_volume)
          {
            min_volume = volume;
            v62 = i;
          }
        }
        /*v65 = start_channel;
        v91 = v93;
      if (start_channel <= end_channel)
      {
        //pAudioPlayer5 = pAudioPlayer;
        pMixerChannel3 = &pAudioPlayer->pMixerChannels[start_channel];
        do
        {
          v68 = AIL_sample_volume(pMixerChannel3->hSample);
          if ( v68 < SLODWORD(v91) )
          {
            LODWORD(v91) = v68;
            v62 = v65;
          }
          ++v65;
          ++pMixerChannel3;
        }
        while ( v65 <= end_channel );*/

        if (v62 == -1)   // no free channels at all - only channel 13 allows override (a3 == -1)
        {
          if (start_channel != 13)
            return;
          v62 = 13;
        }

        MixerChannel* channel = &pMixerChannels[v62];
        AIL_end_sample(channel->hSample);
        FreeChannel(channel);
      }

      if (v62 > end_channel)//10!=13
        return;

//LABEL_150:
      if (sample_volume == 10000)
        sample_volume = uMasterVolume;

        //v70 = 0;
      if ( !a7 )
      {
          //pSoundDesc = pSoundList->pSounds;
          //pSoundDesc2 = &pSoundList->pSounds[sound_id];
        if (!sound_desc->pSoundData[0])
        {
          if (sound_desc->eType == SOUND_DESC_SWAP)
            pSoundList->LoadSound(eSoundID, 0);
        }
      }
        //v72 = 4 * (a7 + 30 * sound_id) + 44;
      if (!pSoundList->pSounds[sound_id].pSoundData[a7])
        return;

        //pMixerChannel5 = &pAudioPlayer->pMixerChannels[v62];
      MixerChannel* channel = &pMixerChannels[v62];
      AIL_init_sample(channel->hSample);
      AIL_set_sample_file(channel->hSample, (char *)pSoundList->pSounds[sound_id].pSoundData[a7] + 4 * (a7 == 0), -1);
      if (uVolume)
        sample_volume = uVolume;
      AIL_set_sample_volume(channel->hSample, sample_volume);

      int object_type = PID_TYPE(a3),
          object_id = PID_ID(a3);
      if (PartyX != -1)
      {
        if (!PartyX) PartyX = pParty->vPosition.x;
        if (!PartyY) PartyY = pParty->vPosition.y;
        AIL_set_sample_pan(channel->hSample, sub_4AB66C(PartyX, PartyY));
        AIL_set_sample_volume(channel->hSample, GetSoundStrengthByDistanceFromParty(PartyX, PartyY, pParty->vPosition.z));
      }
      else if (object_type == OBJECT_BLVDoor)
      {
        assert(uCurrentlyLoadedLevelType == LEVEL_Indoor);

        assert(object_id < pIndoor->uNumDoors);
        BLVDoor* door = &pIndoor->pDoors[object_id];
        if (!door->uDoorID)
          return;

        if (!GetSoundStrengthByDistanceFromParty(door->pXOffsets[0], door->pYOffsets[0], door->pZOffsets[0]))
        {
          AIL_end_sample(channel->hSample);
          FreeChannel(channel);
          return;
        }
        AIL_set_sample_pan(channel->hSample, sub_4AB66C(door->pXOffsets[0], door->pYOffsets[0]));
      }
      else if (object_type == OBJECT_Item)
      {
        assert(object_id < uNumSpriteObjects);
        SpriteObject* object = &pSpriteObjects[object_id];
        if (!GetSoundStrengthByDistanceFromParty(object->vPosition.x, object->vPosition.y, object->vPosition.z) )
            return;
        AIL_set_sample_pan(channel->hSample, sub_4AB66C(object->vPosition.x, object->vPosition.y));
      }
      else if (object_type == OBJECT_Actor)
      {
        assert(object_id < uNumActors);
        Actor* actor = &pActors[object_id];
        if (!GetSoundStrengthByDistanceFromParty(actor->vPosition.x, actor->vPosition.y, actor->vPosition.z))
          return;
        AIL_set_sample_pan(channel->hSample, sub_4AB66C(actor->vPosition.x, actor->vPosition.y));
      }
      else if (object_type == OBJECT_Decoration)
      {
        assert(object_id < uNumLevelDecorations);
        LevelDecoration* decor = &pLevelDecorations[object_id];
        if (!GetSoundStrengthByDistanceFromParty(decor->vPosition.x, decor->vPosition.y, decor->vPosition.z))
          return;
        AIL_set_sample_pan(channel->hSample, sub_4AB66C(decor->vPosition.x, decor->vPosition.y));
      }

//LABEL_184:
      if (uNumRepeats)
        AIL_set_sample_loop_count(channel->hSample, uNumRepeats - 1);
      channel->uSourceTrackIdx = sound_id;
      channel->source_pid = a3;
      channel->uSourceTrackID = eSoundID;
      int rval = AIL_start_sample(channel->hSample);
      if ( sPlaybackRate )
        AIL_set_sample_playback_rate(channel->hSample, sPlaybackRate);
      if (object_type == OBJECT_Player)
        AIL_sample_ms_position(channel->hSample, &sLastTrackLengthMS, 0);
      return;
  }

  __debugbreak(); // 3d sound stuff, refactor
  v12 = 13;
  if ( a3 < 0 )
  {
    v15 = pAudioPlayer->uNum3DSamples;
    if ( a3 == -1 )
    {
      if ( v15 < 16 )
        v12 = v15 - 1;
      v96 = v12;
      //goto LABEL_46;
	  pRenderVertexSoft.vWorldPosition.x = (double)pParty->vPosition.x;
      pRenderVertexSoft.vWorldPosition.y = (double)pParty->vPosition.y;
      v21 = (double)pParty->sEyelevel + (double)pParty->vPosition.z;
      goto LABEL_47;
    }
    if ( v15 >= 16 )
      v15 = 14;
    v12 = v15;
    //goto LABEL_45;
	v96 = v15;
    pRenderVertexSoft.vWorldPosition.x = (double)pParty->vPosition.x;
    pRenderVertexSoft.vWorldPosition.y = (double)pParty->vPosition.y;
    v21 = (double)pParty->sEyelevel + (double)pParty->vPosition.z;
    goto LABEL_47;
  }
  if ( PID_TYPE(a3) == 2 )
  {
    v22 = pAudioPlayer->uNum3DSamples;
    if ( v22 < 16 )
    {
      v12 = 5 * v22 / 16;
      v96 = 7 * v22 / 16;
    }
    else
    {
      v96 = 7;
      v12 = 5;
    }
    pLayingItem = &pSpriteObjects[PID_ID(a3)];
  }
  else
  {
    if ( PID_TYPE(a3) == 3 )
    {
      v18 = pAudioPlayer->uNum3DSamples;
      v12 = 0;
      if ( v18 < 16 )
        v96 = 3 * v18 / 16;
      else
        v96 = 3;
      pActor1 = &pActors[PID_ID(a3)];
      v20 = pActor1->vPosition.y;
      pRenderVertexSoft.vWorldPosition.x = (double)pActor1->vPosition.x;
      v100 = pActor1->vPosition.z;
      pRenderVertexSoft.vWorldPosition.y = (double)v20;
      v21 = (double)v100;
      goto LABEL_47;
    }
    if ( PID_TYPE(a3) != 5 )
    {
      v13 = pAudioPlayer->uNum3DSamples;
      if ( PID_TYPE(a3) == 6 )
      {
        if ( v13 >= 16 )
        {
          v96 = 9;
          v12 = 8;
          //goto LABEL_46;
		  pRenderVertexSoft.vWorldPosition.x = (double)pParty->vPosition.x;
          pRenderVertexSoft.vWorldPosition.y = (double)pParty->vPosition.y;
		  v21 = (double)pParty->sEyelevel + (double)pParty->vPosition.z;
		  goto LABEL_47;
        }
        v12 = 8 * v13 / 16;
        v14 = 9 * v13;
      }
      else
      {
        if ( v13 >= 16 )
        {
          v96 = 12;
          v12 = 10;
//LABEL_46:
          pRenderVertexSoft.vWorldPosition.x = (double)pParty->vPosition.x;
          pRenderVertexSoft.vWorldPosition.y = (double)pParty->vPosition.y;
          v21 = (double)pParty->sEyelevel + (double)pParty->vPosition.z;
          goto LABEL_47;
        }
        v12 = 10 * v13 / 16;
        v14 = 12 * v13;
      }
      v15 = v14 / 16;
//LABEL_45:
      v96 = v15;
      //goto LABEL_46;
	  pRenderVertexSoft.vWorldPosition.x = (double)pParty->vPosition.x;
      pRenderVertexSoft.vWorldPosition.y = (double)pParty->vPosition.y;
      v21 = (double)pParty->sEyelevel + (double)pParty->vPosition.z;
      goto LABEL_47;
    }
    v16 = pAudioPlayer->uNum3DSamples;
    if ( v16 < 16 )
    {
      v12 = v16 / 4;
      v96 = v16 / 4;
    }
    else
    {
      v12 = 4;
      v96 = 4;
    }
    pLayingItem = (SpriteObject *)&pLevelDecorations[PID_ID(a3)];
  }
  pRenderVertexSoft.vWorldPosition.x = (double)pLayingItem->vPosition.x;
  pRenderVertexSoft.vWorldPosition.y = (double)pLayingItem->vPosition.y;
  v21 = (double)pLayingItem->vPosition.z;
LABEL_47:
  pRenderVertexSoft.vWorldPosition.z = v21;
  if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
    sub_4AAEA6_transform(&pRenderVertexSoft);
  else
    pGame->pIndoorCameraD3D->ViewTransform(&pRenderVertexSoft, 1);
  if ( a3 )
  {
    if ( a3 != -1 )
    {
      v101 = 0;
      if ( pAudioPlayer->uNum3DSamples > 0 )
      {
        pAudioPlayer_3DSample = pAudioPlayer->p3DSamples;
        do
        {
          if ( AIL_3D_sample_status(pAudioPlayer_3DSample->hSample) == 4 
			  && pAudioPlayer_3DSample->field_4 == a3 && AIL_3D_sample_status(pAudioPlayer_3DSample->hSample) == 4 )
          {
            if ( pAudioPlayer_3DSample->field_8 == sound_id )
              return;
            AIL_end_3D_sample(pAudioPlayer_3DSample->hSample);
            pAudioPlayer->_4ABF23(pAudioPlayer_3DSample);
          }
          ++v101;
          ++pAudioPlayer_3DSample;
        }
        while ( v101 < pAudioPlayer->uNum3DSamples );
      }
    }
  }
  v102 = v12;
  if ( v12 <= v96 )
  {
    pAudioPlayer_3DSample1 = &pAudioPlayer->p3DSamples[v12];
    while ( AIL_3D_sample_status(pAudioPlayer_3DSample1->hSample) != 2 )
    {
      ++v102;
      ++pAudioPlayer_3DSample1;
      if ( v102 > v96 )
        goto LABEL_67;
    }
    AIL_end_3D_sample(pAudioPlayer_3DSample1->hSample);
    if ( pAudioPlayer_3DSample1->field_8 )
      pAudioPlayer->_4ABF23(pAudioPlayer_3DSample1);
  }
LABEL_67:
  v25 = v96;
  if ( v102 == v96 + 1 )
  {
    LODWORD(v91) = -1;
    v103 = 0;
    *(float *)&varC = pRenderVertexSoft.vWorldViewPosition.y * -0.012207031;
    v93 = 0.0;
    v26 = pRenderVertexSoft.vWorldViewPosition.x * 0.012207031;
    *(float *)&uVolume = v26;
    v27 = abs((signed __int64)v26);
    v28 = abs((signed __int64)v93);
    v29 = abs((signed __int64)*(float *)&varC);
    v90 = int_get_vector_length(v29, v28, v27);
    sPlaybackRate = v12;
    if ( v12 > v25 )
      goto LABEL_192;
    pAudioPlayer_3DSample2 = &pAudioPlayer->p3DSamples[v12];
    do
    {
      AIL_3D_position(pAudioPlayer_3DSample2->hSample, &varC, &v93, (long *)&uVolume);
      v31 = abs((signed __int64)*(float *)&uVolume);
      v32 = abs((signed __int64)v93);
      v33 = abs((signed __int64)*(float *)&varC);
      v34 = int_get_vector_length(v33, v32, v31);
      v35 = v103 == v34;
      if ( v103 < v34 )
      {
        v103 = v34;
        v35 = 1;
      }
      if ( v35 && v90 < v103 )
      {
        v36 = sPlaybackRate;
        LODWORD(v91) = sPlaybackRate;
      }
      else
      {
        v36 = LODWORD(v91);
      }
      ++sPlaybackRate;
      ++pAudioPlayer_3DSample2;
    }
    while ( sPlaybackRate <= v96 );
    if ( v36 == -1 )
    {
LABEL_192:
      v36 = 13;
      if ( v12 != 13 )
        return;
    }
    //pAudioPlayer2 = pAudioPlayer;
    pAudioPlayer_3DSample3 = &pAudioPlayer->p3DSamples[v36];
    AIL_end_3D_sample(pAudioPlayer_3DSample3->hSample);
    pAudioPlayer->_4ABF23(pAudioPlayer_3DSample3);
    v102 = v36;
  }
  //v39 = v89;
  if ( pSoundList->pSounds[sound_id].p3DSound || (LOWORD(v40) = pSoundList->LoadSound(eSoundID, 0), v40) )
  {
    v41 = (char *)pAudioPlayer + 16 * v102;
    v42 = (int)(v41 + 20);
    if ( AIL_set_3D_sample_file(*((int *)v41 + 5), *(void **)((char *)&pSoundList->pSounds->p3DSound + sound_id * sizeof(SoundDesc))) )
    {
      if ( uNumRepeats )
        v86 = uNumRepeats - 1;
      else
        v86 = 1;
      AIL_set_3D_sample_loop_count(*(int *)v42, v86);
      if ( PartyX == -1 )
      {
        if ( PID_TYPE(a3) == 1 )
        {
          if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
          {
			//goto LABEL_103;
            pBLVDoor = &pIndoor->pDoors[PID_ID(a3)];
            if ( !pBLVDoor->uDoorID )
             return;
            pRenderVertexSoft.vWorldPosition.x = (double)*pBLVDoor->pXOffsets;
            pRenderVertexSoft.vWorldPosition.y = (double)*pBLVDoor->pYOffsets;
            v47 = (double)*pBLVDoor->pZOffsets;
LABEL_101:
            pRenderVertexSoft.vWorldPosition.z = v47;
          //if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
          //{
            sub_4AAEA6_transform(&pRenderVertexSoft);
            //goto LABEL_104;
          //}
		  }
//LABEL_103:
		  if ( uCurrentlyLoadedLevelType != LEVEL_Indoor )
            pGame->pIndoorCameraD3D->ViewTransform(&pRenderVertexSoft, 1);
//LABEL_104:
          AIL_start_3D_sample(*(int *)v42);
          AIL_set_3D_sample_float_distances(*(int **)v42, 100.0, 20.0, 100.0, 20.0);
          AIL_set_3D_sample_volume(*(int **)v42, pAudioPlayer->s3DSoundVolume);
          v99 = pRenderVertexSoft.vWorldViewPosition.y * -0.012207031;
          v49 = pRenderVertexSoft.vWorldViewPosition.x * 0.012207031;
          uNumRepeatsa = v49;
          v50 = abs((signed __int64)v49);
          v51 = abs(0);
          v52 = abs((signed __int64)v99);
          if ( int_get_vector_length(v52, v51, v50) <= 100 )
          {
            AIL_set_3D_position((void *)*(int *)v42, LODWORD(v99), 0.0, LODWORD(uNumRepeatsa));
            v53 = -uNumRepeatsa;
            v54 = -v99;
            AIL_set_3D_orientation((void *)*(int *)v42, LODWORD(v54), 0.0, LODWORD(v53), 0.0, 1.0, 0.0);
            //pAudioPlayer3 = pAudioPlayer;
            *((int *)v41 + 6) = a3;
            *((int *)v41 + 7) = sound_id;
            *(&pAudioPlayer->bEAXSupported + 4 * (v102 + 2)) = eSoundID;
          }
          else
          {
            AIL_end_3D_sample(*(int **)v42);
            pAudioPlayer->_4ABF23((AudioPlayer_3DSample *)(v41 + 20));
          }
          return;
        }
        if ( PID_TYPE(a3) == 2 )
        {
          pLayingItem2 = &pSpriteObjects[PID_ID(a3)];
        }
        else
        {
          if ( PID_TYPE(a3) == 3 )
          {
            pActor = &pActors[PID_ID(a3)];
            v46 = pActor->vPosition.y;
            pRenderVertexSoft.vWorldPosition.x = (double)pActor->vPosition.x;
            uNumRepeatsb = pActor->vPosition.z;
            pRenderVertexSoft.vWorldPosition.y = (double)v46;
            v47 = (double)uNumRepeatsb;
            goto LABEL_101;
          }
          if ( PID_TYPE(a3) != 5 )
          {
            pRenderVertexSoft.vWorldPosition.x = (double)pParty->vPosition.x;
            v43 = (double)pParty->vPosition.y;
            pRenderVertexSoft.vWorldPosition.y = v43;
            v47 = (double)pParty->sEyelevel + (double)pParty->vPosition.z;
            goto LABEL_101;
          }
          pLayingItem2 = (SpriteObject *)&pLevelDecorations[PID_ID(a3)];
        }
        pRenderVertexSoft.vWorldPosition.x = (double)pLayingItem2->vPosition.x;
        pRenderVertexSoft.vWorldPosition.y = (double)pLayingItem2->vPosition.y;
        v47 = (double)pLayingItem2->vPosition.z;
        goto LABEL_101;
      }
      pRenderVertexSoft.vWorldPosition.x = (double)PartyX;
      v43 = (double)PartyY;
      pRenderVertexSoft.vWorldPosition.y = v43;
      v47 = (double)pParty->sEyelevel + (double)pParty->vPosition.z;
      goto LABEL_101;
    }
  }
}

//----- (0040DEA5) --------------------------------------------------------
void  AudioPlayer::MessWithChannels()
{
  pAudioPlayer->StopChannels(-1, -1);
}


//----- (004AAFCF) --------------------------------------------------------
void AudioPlayer::UpdateSounds()
{
  //AudioPlayer *pAudioPlayer; // edi@1
  int v2; // ebx@1
  //unsigned __int8 v3; // zf@1
  //int *v4; // eax@2
  //unsigned __int8 v5; // sf@4
  AudioPlayer_3DSample *v6; // esi@5
  int v7; // ebx@6
  int v8; // ebx@9
  int v9; // ebx@10
  int v10; // ebx@11
  double v11; // st7@13
  SpriteObject *v12; // eax@14
  Actor *v13; // eax@15
  signed int v14; // edx@15
  BLVDoor *pDoor; // eax@19
  double v16; // st7@22
  double v17; // st6@22
  double v18; // st5@23
  double v19; // st4@24
  double v20; // st3@24
  double v21; // st6@28
  double v22; // st7@32
  int v23; // ST1C_4@32
  int v24; // ebx@32
  int v25; // eax@32
  float v26; // ST10_4@34
  float v27; // ST08_4@34
  //MixerChannel *v28; // esi@38
  //unsigned __int8 v29; // of@43
  //MixerChannel *v30; // esi@44
  //int v31; // eax@45
  //SpriteObject *v32; // eax@49
  //Actor *v33; // edi@50
  //int v34; // eax@50
  //unsigned int v35; // edx@51
  //unsigned int v36; // ecx@51
  //SpriteObject *v37; // edi@53
  int v38; // eax@53
  //BLVDoor *v39; // edi@56
  //int v40; // eax@57
  //int v41; // eax@60
  //MixerChannel *v42; // edi@65
  //int v43; // ebx@68
  //LevelDecoration *v44; // esi@68
  //int v45; // ST1C_4@68
  //int v46; // edi@68
  //int v47; // eax@68
  //DecorationDesc *v48; // edi@69
  //__int16 v49; // ax@69
  //__int16 v50; // ax@70
  __int16 v51; // ax@71
  //__int16 v52; // ax@73
  signed int v53; // eax@88
  RenderVertexSoft a1; // [sp+24h] [bp-48h]@1
  float v55; // [sp+54h] [bp-18h]@22
  float v56; // [sp+58h] [bp-14h]@22
  int uNumRepeats; // [sp+5Ch] [bp-10h]@15
  float v58; // [sp+60h] [bp-Ch]@23
  int v59; // [sp+64h] [bp-8h]@4
  //AudioPlayer *thisa; // [sp+68h] [bp-4h]@1

  //pAudioPlayer = this;
  v2 = 0;
  //thisa = this;
  //v3 = this->bPlayerReady == 0;
  //a1.flt_2C = 0.0;
  if (!bPlayerReady)
    return;
  
  if (field_2D0_time_left <= pEventTimer->uTimeElapsed)
    field_2D0_time_left = 32;
  else
  {
    field_2D0_time_left -= pEventTimer->uTimeElapsed;
    return;
  }

  //v3 = this->b3DSoundInitialized == 0;
  if ( b3DSoundInitialized )
  {
    __debugbreak(); // refactor refactor
    //v3 = this->uNum3DSamples == 0;
    //v5 = this->uNum3DSamples < 0;
    v59 = 0;
    if (uNum3DSamples > 0)
    {
      v6 = this->p3DSamples;
      while ( 1 )
      {
        v7 = PID_TYPE(v6->field_4);
        if ( AIL_3D_sample_status(v6->hSample) == 2 )
        {
          AIL_end_3D_sample(v6->hSample);
          pAudioPlayer->_4ABF23(v6);
        }
        if ( AIL_3D_sample_status(v6->hSample) != 4 )
          goto LABEL_35;
        v8 = v7 - 1;//
        if ( v8 )//> 1
          break;
        if ( uCurrentlyLoadedLevelType != LEVEL_Indoor )//==1
          goto LABEL_31;
        pDoor = &pIndoor->pDoors[PID_ID(v6->field_4)];
        if ( pDoor->uDoorID )
        {
          uNumRepeats = *pDoor->pXOffsets;
          a1.vWorldPosition.x = (double)uNumRepeats;
          uNumRepeats = *pDoor->pYOffsets;
          a1.vWorldPosition.y = (double)uNumRepeats;
          uNumRepeats = *pDoor->pZOffsets;
          v11 = (double)uNumRepeats;
          goto LABEL_21;
        }
LABEL_35:
        ++v59;
        ++v6;
        if ( v59 >= pAudioPlayer->uNum3DSamples )
        {
          v2 = 0;
          goto LABEL_37;
        }
      }

      v9 = v8 - 1;//
      if ( v9 )//> 2
      {
        v10 = v9 - 1;//
        if ( !v10 )//3
        {
          v13 = &pActors[PID_ID(v6->field_4)];
          uNumRepeats = v13->vPosition.x;
          v14 = v13->vPosition.y;
          a1.vWorldPosition.x = (double)uNumRepeats;
          uNumRepeats = v13->vPosition.z;
          a1.vWorldPosition.y = (double)v14;
          v11 = (double)uNumRepeats;
          goto LABEL_21;
        }
        if ( v10 != 2 )//4
        {
          a1.vWorldPosition.x = (double)pParty->vPosition.x;
          a1.vWorldPosition.y = (double)pParty->vPosition.y;
          v11 = (double)pParty->sEyelevel + (double)pParty->vPosition.z;
          goto LABEL_21;
        }//5
        v12 = (SpriteObject *)&pLevelDecorations[PID_ID(v6->field_4)];
      }
      else//2
      {
        v12 = &pSpriteObjects[PID_ID(v6->field_4)];
      }
      a1.vWorldPosition.x = (double)v12->vPosition.x;
      a1.vWorldPosition.y = (double)v12->vPosition.y;
      v11 = (double)v12->vPosition.z;
LABEL_21:
      a1.vWorldPosition.z = v11;
      if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
      {
        v16 = pGame->pIndoorCameraD3D->fRotationXCosine;
        v17 = pGame->pIndoorCameraD3D->fRotationXSine;
        v55 = pGame->pIndoorCameraD3D->fRotationYCosine;
        v56 = pGame->pIndoorCameraD3D->fRotationYSine;
        if (pGame->pIndoorCameraD3D->sRotationX)
        {
          v58 = a1.vWorldPosition.x - (double)pParty->vPosition.x;
          *(float *)&uNumRepeats = a1.vWorldPosition.y - (double)pParty->vPosition.y;
          v18 = a1.vWorldPosition.z - (double)pParty->vPosition.z;
          if ( pRenderer->pRenderD3D )
          {
            v19 = *(float *)&uNumRepeats * v56 + v58 * v55;
            v20 = v58 * v56 - *(float *)&uNumRepeats * v55;
          }
          else
          {
            v19 = v58 * v55 - *(float *)&uNumRepeats * v56;
            v20 = v58 * v56 + *(float *)&uNumRepeats * v55;
          }
          a1.vWorldViewPosition.x = v19 * v16 - v18 * v17;
          a1.vWorldViewPosition.y = v20;
          a1.vWorldViewPosition.z = v19 * v17 + v18 * v16;
        }
        else
        {
          v58 = a1.vWorldPosition.x - (double)pParty->vPosition.x;
          *(float *)&uNumRepeats = a1.vWorldPosition.y - (double)pParty->vPosition.y;
          if ( pRenderer->pRenderD3D )
          {
            a1.vWorldViewPosition.x = *(float *)&uNumRepeats * v56 + v58 * v55;
            v21 = v58 * v56 - *(float *)&uNumRepeats * v55;
          }
          else
          {
            a1.vWorldViewPosition.x = v58 * v55 - *(float *)&uNumRepeats * v56;
            v21 = v58 * v56 + *(float *)&uNumRepeats * v55;
          }
          a1.vWorldViewPosition.y = v21;
          a1.vWorldViewPosition.z = a1.vWorldPosition.z - (double)pParty->vPosition.z;
        }
      }
      else
      {
LABEL_31:
        pGame->pIndoorCameraD3D->ViewTransform(&a1, 1u);
      }
      v58 = a1.vWorldViewPosition.y * -0.012207031;
      v22 = a1.vWorldViewPosition.x * 0.012207031;
      *(float *)&uNumRepeats = v22;
      v23 = abs((signed __int64)v22);
      v24 = abs(0);
      v25 = abs((signed __int64)v58);
      if ( int_get_vector_length(v25, v24, v23) <= 100 )
      {
        AIL_set_3D_position(v6->hSample, LODWORD(v58), 0.0, uNumRepeats);
        v26 = -*(float *)&uNumRepeats;
        v27 = -v58;
        AIL_set_3D_orientation(v6->hSample, LODWORD(v27), 0.0, LODWORD(v26), 0.0, 1.0, 0.0);
      }
      else
      {
        AIL_end_3D_sample(v6->hSample);
        pAudioPlayer->_4ABF23(v6);
      }
      goto LABEL_35;
    }
  }

LABEL_37:
  for (uint i = 0; i < uMixerChannels; ++i)
  {
    if (AIL_sample_status(pMixerChannels[i].hSample) == AIL::Sample::Done)
    {
      AIL_end_sample(pMixerChannels[i].hSample);
      FreeChannel(&pMixerChannels[i]);
    }
  }

  for (uint i = 0; i < uMixerChannels; ++i)
  {
    if (pMixerChannels[i].source_pid <= 0)
      continue;

    int source_type = PID_TYPE(pMixerChannels[i].source_pid),
        source_id = PID_ID(pMixerChannels[i].source_pid);
    int source_x,
        source_y,
        source_z;

    switch (source_type)
    {
      case 0:
      case OBJECT_Player:
      case OBJECT_BModel:
        continue;

      case OBJECT_BLVDoor:
      {
        assert(uCurrentlyLoadedLevelType == LEVEL_Indoor);

        assert(source_id < pIndoor->uNumDoors);
        if (!pIndoor->pDoors[source_id].uDoorID)
          continue;

        source_x = pIndoor->pDoors[source_id].pXOffsets[0];
        source_y = pIndoor->pDoors[source_id].pYOffsets[0];
        source_z = pIndoor->pDoors[source_id].pZOffsets[0];
      }
      break;

      case OBJECT_Item:
      {
        assert(source_id < uNumSpriteObjects);

        source_x = pSpriteObjects[source_id].vPosition.x;
        source_y = pSpriteObjects[source_id].vPosition.y;
        source_z = pSpriteObjects[source_id].vPosition.z;
      }
      break;

      case OBJECT_Decoration:
      {
        assert(source_id < uNumLevelDecorations);

        source_x = pLevelDecorations[source_id].vPosition.x;
        source_y = pLevelDecorations[source_id].vPosition.y;
        source_z = pLevelDecorations[source_id].vPosition.z;
      }
      break;

      case OBJECT_Actor:
      {
        assert(source_id < uNumActors);

        source_x = pActors[source_id].vPosition.x;
        source_y = pActors[source_id].vPosition.y;
        source_z = pActors[source_id].vPosition.z;
      }
      break;

      default:
        assert(false);
        continue;
    }

    if (int sound_strength = GetSoundStrengthByDistanceFromParty(source_x, source_y, source_z))
    {
      AIL_set_sample_volume(pMixerChannels[i].hSample, sound_strength);
      AIL_set_sample_pan(pMixerChannels[i].hSample, sub_4AB66C(source_x, source_y));
    }
    else
    {
      AIL_end_sample(pMixerChannels[i].hSample);
      FreeChannel(&pMixerChannels[i]);
    }
  }



  if (pCurrentScreen != SCREEN_GAME)
  {
    MixerChannel* channel = &pMixerChannels[4];
    if (AIL_sample_status(channel->hSample) == AIL::Sample::Playing)
      AIL_end_sample(channel->hSample);
    return;
  }
  if (!_6807E0_num_decorations_with_sounds_6807B8)
    return;

  v55 = 0;
      //v59 = 0;
  for (uint i = 0; i < _6807E0_num_decorations_with_sounds_6807B8; ++i)
  {
      //while ( 1 )
      //{
        LODWORD(v56) = 1;
        //v43 = _6807B8_level_decorations_ids[v59];
        //v44 = &pLevelDecorations[_6807B8_level_decorations_ids[v59]];
        //v45 = abs(v44->vPosition.z - pParty->vPosition.z);
        //v46 = abs(v44->vPosition.y - pParty->vPosition.y);
        //v47 = abs(v44->vPosition.x - pParty->vPosition.x);
    LevelDecoration* decor = &pLevelDecorations[_6807B8_level_decorations_ids[i]];
    if (int_get_vector_length(decor->vPosition.x - pParty->vPosition.x,
                              decor->vPosition.y - pParty->vPosition.y,
                              decor->vPosition.z - pParty->vPosition.z) > 8192)
      continue;

    DecorationDesc* decor_desc = &pDecorationList->pDecorations[decor->uDecorationDescID];
      //v48 = &pDecorationList->pDecorations[decor->uDecorationDescID];
      //v49 = v48->uFlags;
      uNumRepeats = (~(unsigned __int8)decor_desc->uFlags & DECORATION_DESC_SLOW_LOOP) >> 6;
 
    if (decor_desc->SoundOnDawn() || decor_desc->SoundOnDusk())
    {
        //v50 = decor->field_1A;
        v55 = 0.0;
        uNumRepeats = 2;
        if (decor->field_1A)
        {
          v51 = decor->field_1A - 32;
          decor->field_1A = v51;
          if ( v51 < 0 )
            decor->field_1A = 0;
        }
    }

      //v52 = v48->uFlags;
    if (!decor_desc->SoundOnDawn())
    {
      if (!decor_desc->SoundOnDusk())
      {
        if ( v55 == 0.0 )
        {
          if ( v56 != 0.0 )
          {
            v53 = 8 * _6807B8_level_decorations_ids[i];
            LOBYTE(v53) = v53 | OBJECT_Decoration;
            PlaySound((SoundID)decor_desc->uSoundID, v53, uNumRepeats, -1, 0, 0, 0, 0);
          }
          continue;
        }
        if ( !decor->field_1A )
          decor->field_1A = (rand() % 15 + 1) << 7;
        if ( v56 != 0.0 )
        {
          v53 = 8 * _6807B8_level_decorations_ids[i];
          LOBYTE(v53) = v53 | OBJECT_Decoration;
          PlaySound((SoundID)decor_desc->uSoundID, v53, uNumRepeats, -1, 0, 0, 0, 0);
        }
        continue;
      }
      if ( v55 != 0.0 )
      {
        if ( !decor->field_1A )
          decor->field_1A = (rand() % 15 + 1) << 7;
        if ( v56 != 0.0 )
        {
          v53 = 8 * _6807B8_level_decorations_ids[i];
          LOBYTE(v53) = v53 | OBJECT_Decoration;
          PlaySound((SoundID)decor_desc->uSoundID, v53, uNumRepeats, -1, 0, 0, 0, 0);
        }
        continue;
      }
    }
    v56 = 0.0;

    if (pParty->uCurrentHour >= 5 && pParty->uCurrentHour < 6 ||
        pParty->uCurrentHour >= 20 && pParty->uCurrentHour < 21)
    {
        if ( !decor->field_1A && rand() % 100 < 100 )
          LODWORD(v56) = 1;
        LODWORD(v55) = 1;
    }
    if ( v55 == 0.0 )
    {
      if ( v56 != 0.0 )
      {
        v53 = 8 * _6807B8_level_decorations_ids[i];
        LOBYTE(v53) = v53 | OBJECT_Decoration;
        PlaySound((SoundID)decor_desc->uSoundID, v53, uNumRepeats, -1, 0, 0, 0, 0);
      }
      continue;
    }
    if ( !decor->field_1A )
      decor->field_1A = (rand() % 15 + 1) << 7;
    if ( v56 != 0.0 )
    {
      v53 = 8 * _6807B8_level_decorations_ids[i];
      LOBYTE(v53) = v53 | OBJECT_Decoration;
      PlaySound((SoundID)decor_desc->uSoundID, v53, uNumRepeats, -1, 0, 0, 0, 0);
    }
    continue;
  }
}

//----- (004AB66C) --------------------------------------------------------
int __fastcall sub_4AB66C(int a1, int a2)
{
  signed int v2; // eax@1

  v2 = stru_5C6E00->uDoublePiMask & (stru_5C6E00->Atan2(a1 - pParty->vPosition.x, a2 - pParty->vPosition.y)
                                  - stru_5C6E00->uIntegerHalfPi - pParty->sRotationY);
  if ( v2 > (signed int)stru_5C6E00->uIntegerPi )
    v2 = 2 * stru_5C6E00->uIntegerPi - v2;
  return (v2 >> 3) - (v2 >> 10);
}
// 4AB66C: using guessed type int __fastcall sub_4AB66C(int, int);

//----- (004AB6B1) --------------------------------------------------------
int GetSoundStrengthByDistanceFromParty(int x, int y, int z)
{
  int dir_x; // ST08_4@1
  int dir_y; // esi@1
  int dir_z; // eax@1
  int length; // [sp+10h] [bp+8h]@1

  dir_z = abs(z - pParty->vPosition.z);
  dir_y = abs(y - pParty->vPosition.y);
  dir_x = abs(x - pParty->vPosition.x);
  length = int_get_vector_length(dir_x, dir_y, dir_z);
  if ( length <= 8192 )
    return 114 - (unsigned __int64)(signed __int64)((double)length * 0.0001220703125 * 100.0);
  else
    return 0;
}

//----- (004AB71F) --------------------------------------------------------
void AudioPlayer::StopChannels(int uStartChannel, int uEndChannel)
{
  //AudioPlayer *v3; // esi@1
  int v4; // ecx@1
  //AudioPlayer_3DSample *v5; // edi@4
  int v6; // ebx@12
  MixerChannel *pChannel; // edi@14
  //_STREAM *v8; // esi@23
  int v9; // [sp+4h] [bp-4h]@3

  if ( bPlayerReady )
  {
    if ( b3DSoundInitialized )
    {
      v9 = 0;
      if ( uNum3DSamples > 0 )
      {
        //v5 = p3DSamples;//;(char *)&p3DSamples[0].field_8;
        for ( v4 = 0; v4 < uNum3DSamples; ++v4 )
        {
          if ( (uStartChannel == -1 || v4 < uStartChannel || v4 > uEndChannel)
			  && p3DSamples[v4].field_8
			  && pSoundList->pSounds[p3DSamples[v4].field_8].eType != SOUND_DESC_SYSTEM)
          {
            AIL_end_3D_sample(p3DSamples[v4].hSample);
            _4ABF23(&p3DSamples[v4]);
            p3DSamples[v4].field_4 = 0;
            v4 = v9;
          }
          //v5 += 16;
          v9 = v4;
        }
      }
    }
    if ( hDigDriver && uMixerChannels > 0 )
    {
      pChannel = pMixerChannels;
      for ( v6 = 0; v6 < uMixerChannels; ++v6 )
      {
        if ( (uStartChannel == -1 || v6 < uStartChannel || v6 > uEndChannel)
          && pSoundList->pSounds[pChannel->uSourceTrackIdx].eType != SOUND_DESC_SYSTEM)
        {
          AIL_end_sample(pChannel->hSample);
          FreeChannel(pChannel);
          pChannel->source_pid = 0;
        }
        ++pChannel;
      }
    }
    if (hSequence)
      AIL_end_sequence(hSequence);
    //v8 = hStream;
    if (hStream)
      AIL_pause_stream(hStream, 1);
  }
}

//----- (004AB818) --------------------------------------------------------
void AudioPlayer::LoadAudioSnd()
{
  DWORD NumberOfBytesRead; // [sp+Ch] [bp-4h]@3

  hAudioSnd = CreateFileA("Sounds\\Audio.snd", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0x8000080u, 0);
  if (hAudioSnd == INVALID_HANDLE_VALUE)
  {
    Log::Warning(L"Can't open file: %s", L"Sounds\\Audio.snd");
    return;
  }

  ReadFile(hAudioSnd, &uNumSoundHeaders, 4u, &NumberOfBytesRead, 0);
  pSoundHeaders = nullptr;
  pSoundHeaders = (SoundHeader *)malloc(52 * uNumSoundHeaders + 2);
  ReadFile(hAudioSnd, pSoundHeaders, 52 * uNumSoundHeaders, &NumberOfBytesRead, 0);
}

//----- (004AB8CE) --------------------------------------------------------
void AudioPlayer::Initialize()
{
  //AudioPlayer *v2; // esi@1
  int v3; // ebx@1
  //_DIG_DRIVER *v4; // eax@1
  char v5; // dl@5
  _PROVIDER *v6; // eax@9
  //HWND v7; // ST00_4@9
  MixerChannel *pChannel; // edi@14
  _SAMPLE *v9; // eax@15
  //_REDBOOK *v10; // eax@19
  //int v11; // ecx@21
  int v12; // [sp+Ch] [bp-Ch]@9
  char *Str1; // [sp+10h] [bp-8h]@6
  int v14; // [sp+14h] [bp-4h]@5

  //v2 = this;
  v3 = 0;
  //this->hWindow = hWnd;
  this->hAILRedbook = 0;
  this->hDigDriver = 0;
  this->dword_0002AC = 0;
  this->hSequence = 0;
  this->uMasterVolume = 127;
  this->dword_0002C8 = 64;
  this->dword_0002CC = 2;

  MSS32_DLL_Initialize();
  BINKW32_DLL_Initialize();
  SMACKW32_DLL_Initialize();
  
  AIL_startup();
  if (bCanLoadFromCD)
    hAILRedbook = AIL_redbook_open_drive(cMM7GameCDDriveLetter/*cGameCDDriveLetter*/);
  //else
  //  hAILRedbook = AIL_redbook_open(0);
  //v4 = Audio_GetFirstHardwareDigitalDriver();

  hDigDriver = Audio_GetFirstHardwareDigitalDriver();
  if ( hDigDriver )
    SmackSoundUseMSS(hDigDriver);
  if ( ReadWindowsRegistryInt("Disable3DSound", 0) != 1 && pVersion->pVersionInfo.dwPlatformId != VER_PLATFORM_WIN32_NT )
  {
    v14 = 0;
    bEAXSupported = 0;
    b3DSoundInitialized = 0;
    ReadWindowsRegistryString("3DSoundProvider", p3DSoundProvider, 128u, "NONE");
    __debugbreak(); // audioplayer.cpp(1926): warning C4700: uninitialized local variable 'v5' used
    CheckA3DSupport(v5);
    HPROVIDER prov;
    while ( AIL_enumerate_3D_providers(&v14, &prov, &Str1) )
    {
      if ( !strcmp(Str1, p3DSoundProvider) )
      {
        if ( AIL_open_3D_provider(prov) )
        {
          bEAXSupported = 0;
          b3DSoundInitialized = 0;
          h3DSoundProvider = 0;
        }
        else
        {
          v6 = prov;
          //v7 = prov;
          b3DSoundInitialized = 1;
          h3DSoundProvider = v6;
          uNum3DSamples = 4;
          AIL_3D_provider_attribute(prov, "EAX environment selection", &v12);
          if ( v12 != -1 )
            bEAXSupported = 1;
        }
        pAudioPlayer->_4AC0A2();
        break;
      }
    }
  }
  if ( uMixerChannels > 0 )
  {
    pChannel = pMixerChannels;
    do
    {
      v9 = AIL_allocate_sample_handle(hDigDriver);
      pChannel->hSample = v9;
      if ( !v9 )
        break;
      ++v3;
      ++pChannel;
    }
    while ( v3 < uMixerChannels );
  }
  uMixerChannels = v3;
  if ( bPlayerReady )
    StopChannels(-1, -1);
  //v10 = hAILRedbook;
  bPlayerReady = true;
  if ( hAILRedbook )
  {
    AIL_redbook_stop(hAILRedbook);
    uNumRedbookTracks = AIL_redbook_tracks(hAILRedbook);
  }
  pAudioPlayer->sRedbookVolume = AIL_redbook_volume(hAILRedbook);
  pAudioPlayer->SetMasterVolume(pSoundVolumeLevels[uSoundVolumeMultiplier] * 128.0f);
  if ( bPlayerReady && hAILRedbook )
    AIL_redbook_set_volume(hAILRedbook, (unsigned __int64)(pSoundVolumeLevels[uMusicVolimeMultiplier] * 64.0f) >> 32);
  LoadAudioSnd();
}

//----- (004ABAF7) --------------------------------------------------------
_DIG_DRIVER *Audio_GetFirstHardwareDigitalDriver(void)
{
  int v0; // ecx@1
  unsigned int v1; // ebp@2
  size_t v2; // eax@4
  signed int v3; // kr14_4@9
  char *v5; // [sp+10h] [bp-Ch]@2
  unsigned int v6; // [sp+14h] [bp-8h]@1
  _DIG_DRIVER *hDrv; // [sp+18h] [bp-4h]@3
  
  static int dword_4F00DC = 22050;
  static int dword_4F00E0 = 16;
  static int dword_4F00E4 = 2;

  AIL_set_preference(15, 0);
  AIL_set_preference(33, 1);
  v0 = dword_4F00DC;
  pAudioPlayer->pDeviceNames[0][0] = 0;
  pAudioPlayer->uNumDevices = 0;
  v6 = 0;

  if ( dword_4F00DC < 11025 )
    return 0;
  v1 = 0;
  v5 = (char *)pAudioPlayer->array_000BF0;
  while ( 1 )
  {
    while ( 1 )
    {
      pcmWaveFormat.wf.wFormatTag = 1;
      pcmWaveFormat.wf.nChannels = dword_4F00E4;
      pcmWaveFormat.wf.nSamplesPerSec = v0;
      pcmWaveFormat.wf.nBlockAlign = dword_4F00E4 * dword_4F00E0 / 8;
      pcmWaveFormat.wBitsPerSample = dword_4F00E0;
      pcmWaveFormat.wf.nAvgBytesPerSec = v0 * dword_4F00E4 * dword_4F00E0 / 8;
      if ( !AIL_waveOutOpen(&hDrv, 0, -1, &pcmWaveFormat.wf) )
        break;
      if ( !AIL_get_preference(15) )
        goto LABEL_8;
      v3 = dword_4F00DC;
      v0 = dword_4F00DC / 2;
      dword_4F00DC /= 2;
      if ( v3 / 2 < 11025 )
      {
        if ( dword_4F00E0 == 8 )
        {
          v0 = 22050;
          dword_4F00E0 = 8;
          dword_4F00DC = 22050;
        }
        goto LABEL_12;
      }
    }
    strcpy(pAudioPlayer->pDeviceNames[v1 / 8], "Device: ");
    v2 = strlen(pAudioPlayer->pDeviceNames[v1 / 8]);
    AIL_digital_configuration(hDrv, (int *)v5, (int *)(v5 + 64), (char *)pAudioPlayer->pDeviceNames + v2 + v1 * 16);
    ++v6;
    v1 += 8;
    v5 += 4;
    pAudioPlayer->uNumDevices = v6;
    if ( AIL_get_preference(15) )
      return hDrv;
    if ( !strstr(pAudioPlayer->pDeviceNames[v1 / 8 - 1], "Emulated") )
      return hDrv;
    AIL_waveOutClose(hDrv);
    AIL_set_preference(15, 1);
LABEL_8:
    AIL_set_preference(15, 1);
    v0 = dword_4F00DC;
LABEL_12:
    if ( v0 < 11025 )
      return 0;
  }
}

//----- (004ABC9B) --------------------------------------------------------
LSTATUS AudioPlayer::CheckA3DSupport(char a2)
{
  LSTATUS result; // eax@1
  DWORD cbData; // [sp+8h] [bp-Ch]@1
  int Data; // [sp+Ch] [bp-8h]@1
  HKEY hKey; // [sp+10h] [bp-4h]@1
  char v6; // [sp+1Ch] [bp+8h]@0
  v6=0; //to fix
  hKey = 0;
  cbData = 4;
  Data = 0;
  result = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Aureal\\A3D", 0, 0x2001Fu, &hKey);
  if ( !result )
  {
    if ( v6 )
      RegQueryValueExA(hKey, "SplashAudio", 0, 0, (LPBYTE)&Aureal3D_SplashAudio, &cbData);
    else
      Data = Aureal3D_SplashAudio;
    RegSetValueExA(hKey, "SplashAudio", 0, 4u, (const BYTE *)&Data, 4u);
    Data = 0;
    if ( v6 )
      RegQueryValueExA(hKey, "SplashScreen", 0, 0, (LPBYTE)&Aureal3D_SplashScreen, &cbData);
    else
      Data = Aureal3D_SplashScreen;
    RegSetValueExA(hKey, "SplashScreen", 0, 4u, (const BYTE *)&Data, 4u);
    result = RegCloseKey(hKey);
  }
  return result;
}
// 4ABC9B: inconsistent function type and number of purged bytes

//----- (004ABD5B) --------------------------------------------------------
void AudioPlayer::Release() //Освободить
{
  MixerChannel *pMixerChannel; // ebx@3
  char v4; // dl@5
  AudioPlayer_3DSample *p3DSample; // edi@7
  void *v9; // ecx@15

  if ( this->bPlayerReady )
  {
    CloseHandle(pVideoPlayer->hMagicVid);
    CloseHandle(pVideoPlayer->hMightVid);
    pAudioPlayer->StopChannels(-1, -1);
    if ( pAudioPlayer->uMixerChannels > 0 )
    {
      pMixerChannel = pAudioPlayer->pMixerChannels;
      for ( uint i = 0; i < pAudioPlayer->uMixerChannels; ++i )
      {
        AIL_release_sample_handle(pMixerChannel->hSample);
        ++pMixerChannel;
      }
    }
    if ( ReadWindowsRegistryInt("Disable3DSound", 0) != 1 )
    {
      pAudioPlayer->CheckA3DSupport(0);// pAudioPlayer->CheckA3DSupport(v4);
      if ( pAudioPlayer->uNum3DSamples > 0 )
      {
        p3DSample = pAudioPlayer->p3DSamples;
        for ( uint i = 0; i < pAudioPlayer->uNum3DSamples; ++i )
        {
          if ( p3DSample->hSample )
          {
            AIL_release_3D_sample_handle(p3DSample->hSample);
            p3DSample->hSample = 0;
          }
          ++p3DSample;
        }
      }
      if ( pAudioPlayer->h3DSoundProvider )
      {
        AIL_close_3D_provider(pAudioPlayer->h3DSoundProvider);
        pAudioPlayer->h3DSoundProvider = 0;
      }
    }
    if ( pAudioPlayer->hAILRedbook )
    {
      AIL_redbook_stop(pAudioPlayer->hAILRedbook);
      AIL_redbook_set_volume((HREDBOOK)&pAudioPlayer->hAILRedbook, pAudioPlayer->sRedbookVolume);
      AIL_redbook_close(pAudioPlayer->hAILRedbook);
    }
    AIL_shutdown();
    pSoundList->Release();
    v9 = *(void **)&pAudioPlayer->field_C78[0];
    if ( v9 )
      ReleaseSoundData(v9);
    CloseHandle(pAudioPlayer->hAudioSnd);
  }
}

//----- (004ABE55) --------------------------------------------------------
void AudioPlayer::FreeChannel(MixerChannel *pChannel)
{
  //int v2; // ebx@1
  //AudioPlayer *v3; // esi@1
  //SoundDesc *v4; // eax@2
  unsigned __int8 v5; // zf@5
  //unsigned __int8 v6; // sf@5
  //char *v7; // edi@6
  int num_same_sound_on_channels; // eax@8
  //int v9; // ST04_4@8
  int v10; // ecx@12
  int v11; // edi@13
  int v12; // eax@13
  //unsigned __int8 v13; // of@13
  int v14[16]; // [sp+Ch] [bp-48h]@8
  int num_playing_channels; // [sp+4Ch] [bp-8h]@5
  //int v16; // [sp+50h] [bp-4h]@5

  if (!pSoundList->pSounds)
    return;
 
    //v4 = &pSoundList->pSounds[pChannel->uSourceTrackIdx];
    if ( pSoundList->pSounds[pChannel->uSourceTrackIdx].eType == SOUND_DESC_SWAP)
    {
      if ( pSoundList->pSounds[pChannel->uSourceTrackIdx].pSoundData[0] && 
          !(pSoundList->pSounds[pChannel->uSourceTrackIdx].uFlags & SOUND_DESC_SYSTEM) )
      {
        num_playing_channels = 0;
        num_same_sound_on_channels = 0;
        if ( this->uMixerChannels <= 0 )
          goto LABEL_16;
        for ( uint i = 0; i < uMixerChannels; i++ )
        {
          if ( pChannel->uSourceTrackID == pMixerChannels[i].uSourceTrackID )
          {
            v14[num_same_sound_on_channels++] = i;
            if ( AIL_sample_status((HSAMPLE)pMixerChannels[i].hSample) == AIL::Sample::Playing)
              ++num_playing_channels;
          }
        }
        if ( !num_playing_channels )
        {
LABEL_16:
          pSoundList->UnloadSound(pChannel->uSourceTrackIdx, 1);
          v10 = 0;
          if ( num_same_sound_on_channels  > 0 )
          {
            do
            {
              v11 = v14[v10];
              v12 = 16 * (v14[v10++] + 47);
              pMixerChannels[v11].uSourceTrackID = 0;
              //v13 = __OFSUB__(v10, num_same_sound_on_channels);
              //v6 = v10 - num_same_sound_on_channels < 0;
              *(unsigned int *)((char *)&bEAXSupported + v12) = 0;
            }
            while (v10 < num_same_sound_on_channels);
          }
        }
      }
    }
}

//----- (004ABF23) --------------------------------------------------------
void AudioPlayer::_4ABF23(AudioPlayer_3DSample *a2)
{
  int v2; // ebx@1
  //AudioPlayer *v3; // esi@1
  SoundDesc *v4; // eax@2
  unsigned __int8 v5; // zf@5
  unsigned __int8 v6; // sf@5
  char *v7; // edi@6
  int v8; // eax@8
  int v9; // ST04_4@8
  int v10; // ecx@12
  int v11; // eax@13
  unsigned __int8 v12; // of@13
  int v13[16]; // [sp+Ch] [bp-48h]@8
  int v14; // [sp+4Ch] [bp-8h]@5
  int v15; // [sp+50h] [bp-4h]@5

  v2 = 0;
  //v3 = this;
  if ( pSoundList->pSounds )
  {
    v4 = &pSoundList->pSounds[a2->field_8];
    if ( v4->eType == SOUND_DESC_SWAP)
    {
      if ( v4->p3DSound && !(v4->uFlags & SOUND_DESC_SYSTEM) )
      {
        v5 = this->uNum3DSamples == 0;
        v6 = this->uNum3DSamples < 0;
        v14 = 0;
        v15 = 0;
        if ( v6 | v5 )
          goto LABEL_16;
        v7 = (char *)this->p3DSamples;
        do
        {
          if ( a2->field_C == *((int *)v7 + 3) )
          {
            v8 = v15;
            v9 = *(int *)v7;
            ++v15;
            v13[v8] = v2;
            if ( AIL_3D_sample_status((void *)v9) == 4 )
              ++v14;
          }
          ++v2;
          v7 += 16;
        }
        while ( v2 < uNum3DSamples );
        if ( !v14 )
        {
LABEL_16:
          pSoundList->UnloadSound(a2->field_8, 1);
          v10 = 0;
          if ( v15 > 0 )
          {
            do
            {
              v11 = v13[v10++];
              *(&bEAXSupported + 4 * (v11 + 2)) = 0;
              v12 = __OFSUB__(v10, v15);
              v6 = v10 - v15 < 0;
              p3DSamples[v11].field_8 = 0;
            }
            while ( v6 ^ v12 );
          }
        }
      }
    }
  }
}

//----- (004ABFDB) --------------------------------------------------------
void PlayLevelMusic()
{
  unsigned int v0; // eax@1

  v0 = pMapStats->GetMapInfo(pCurrentMapName);
  if ( v0 )
    pAudioPlayer->PlayMusicTrack((MusicID)pMapStats->pInfos[v0].uRedbookTrackID);
}

//----- (004AC004) --------------------------------------------------------
void AudioPlayer::SetEAXPreferences()
{
  float v4; // [sp+4h] [bp-4h]@2

  if ( this->bEAXSupported )
  {
    v4 = 0.0;
    AIL_set_3D_provider_preference(this->h3DSoundProvider, "EAX effect volume", (int *)&v4);
    v4 = 1.0;
    AIL_set_3D_provider_preference(this->h3DSoundProvider, "EAX damping", (int *)&v4);
  }
}
// 4D82DC: using guessed type int __stdcall AIL_set_3D_provider_preference(int, int, int);

//----- (004AC041) --------------------------------------------------------
void AudioPlayer::SetMapEAX()
{
  unsigned int pMapID; // eax@1
  int v3; // [sp+4h] [bp-4h]@3

  pMapID = pMapStats->GetMapInfo(pCurrentMapName);
  if ( this->b3DSoundInitialized && this->bEAXSupported )
  {
    v3 = pMapStats->pInfos[pMapID].uEAXEnv;
    if ( (unsigned int)v3 >= 0x1A )
    {
      SetEAXPreferences();
      this->field_214 = -1;
    }
    else
    {
      AIL_set_3D_provider_preference(this->h3DSoundProvider, "EAX environment selection", &v3);
      this->field_214 = v3;
    }
  }
}
// 4D82DC: using guessed type int __stdcall AIL_set_3D_provider_preference(int, int, int);

//----- (004AC0A2) --------------------------------------------------------
int AudioPlayer::_4AC0A2()
{
  AudioPlayer *v1; // esi@1
  unsigned int v2; // eax@1
  char v3; // zf@1
  int v4; // ebx@1
  int *v5; // edi@2
  int v6; // eax@4
  AudioPlayer_3DSample *v8; // ebx@7
  void *v9; // eax@8
  int v10; // ebx@14
  unsigned int v11; // eax@14
  int v12; // [sp+1Ch] [bp-8h]@1
  int v13; // [sp+20h] [bp-4h]@6

  v1 = this;
  v2 = pMapStats->GetMapInfo(pCurrentMapName);
  v3 = v1->b3DSoundInitialized == 0;
  v4 = v2;
  v12 = v2;
  if ( !v3 )
  {
    v5 = &v1->uNum3DSamples;
    AIL_3D_provider_attribute(v1->h3DSoundProvider, "Maximum supported samples", &v1->uNum3DSamples);
    if ( v1->uNum3DSamples > 32 )
      *v5 = 32;
    v6 = *v5;
    if ( !*v5 )
    {
      v1->b3DSoundInitialized = 0;
      return -1;
    }
    v13 = 0;
    if ( v6 > 0 )
    {
      v8 = v1->p3DSamples;
      while ( 1 )
      {
        v9 = (void *)AIL_allocate_3D_sample_handle(v1->h3DSoundProvider);
        v8->hSample = v9;
        if ( !v9 )
          break;
        AIL_set_3D_sample_float_distances(v9, 4096.0, 256.0, 4096.0, 256.0);
        AIL_set_3D_sample_volume(v8->hSample, v1->s3DSoundVolume);
        ++v13;
        ++v8;
        if ( v13 >= *v5 )
          goto LABEL_12;
      }
      *v5 = v13;
LABEL_12:
      v4 = v12;
    }
    if ( v1->bEAXSupported )
    {
      v10 = v4;
      v11 = pMapStats->pInfos[v10].uEAXEnv;
      v12 = pMapStats->pInfos[v10].uEAXEnv;
      if ( v11 >= 0x1A )
      {
        pAudioPlayer->SetEAXPreferences();
        v1->field_214 = -1;
      }
      else
      {
        AIL_set_3D_provider_preference(v1->h3DSoundProvider, "EAX environment selection", &v12);
        v1->field_214 = v12;
      }
    }
  }
  return 1;
}

//----- (004A96BE) --------------------------------------------------------
void ReleaseSoundData(void *_this)
{
  for ( uint i = 0; (void *)&pSounds[i].pSoundData < (void *)&pSounds[2999].pSoundData; i++ )
  {
    if ( pSounds[i].pSoundData == _this )
    {
      free(_this);
      memset(&pSounds[i], 0, 0x80u);
    }
  }

}

//----- (004A96FF) --------------------------------------------------------
struct SoundHeader *FindSound_BinSearch(unsigned int uStart, unsigned int uEnd, const char *pName)
{
/*  SoundHeader *result; // eax@2
  SoundHeader *pSound;
  signed int v6; // ebx@11

  while ( 1 )
  {
    v6 = uEnd - uStart;
    pSound = &pAudioPlayer->pSoundHeaders[v6 / 2 + uStart];
    if ( !pSound )
      return false;
    result = (SoundHeader *)_stricmp(pName, pSound->pSoundName);
    if ( !_stricmp(pName, pSound->pSoundName) )
      uFindSound_BinSearch_ResultID = v6 / 2 + uStart;
    if ( uStart == uEnd )
    {
      uFindSound_BinSearch_ResultID = -1;
      return result;
    }
    if ( (signed int)result < 0 )
      break;

    if ( v6 <= 4 )
    {
      if ( (signed int)uStart < (signed int)uEnd )
      {
        for ( uint i = uStart; i < (signed int)uEnd; ++i )
        {
          if ( !_stricmp(pName, pAudioPlayer->pSoundHeaders[i].pSoundName) )
          {
            uFindSound_BinSearch_ResultID = i;
            return &pAudioPlayer->pSoundHeaders[i];
          }
        }
      }
      uFindSound_BinSearch_ResultID = -1;
      return false;
    }

    uStart += v6 / 2;
LABEL_10:
	;
  }
  if ( v6 > 4 )
  {
    uEnd = v6 / 2 + uStart;
    goto LABEL_10;
  }
  if ( (signed int)uStart >= (signed int)uEnd )
  {
    uFindSound_BinSearch_ResultID = -1;
    return false;
  }*/
  for ( uint i = uStart; i < (signed int)uEnd; ++i )
  {
    if ( !_stricmp(pName, pAudioPlayer->pSoundHeaders[i].pSoundName) )
    {
      uFindSound_BinSearch_ResultID = i;
      return &pAudioPlayer->pSoundHeaders[i];
    }
  }
  uFindSound_BinSearch_ResultID = -1;
  return false;
}
// F1B4C8: using guessed type int uFindSound_BinSearch_ResultID;

//----- (004A97C6) --------------------------------------------------------
SoundData *LoadSound(const char *pSoundName, SoundData *pOutBuff, unsigned int uID)
{
  DWORD NumberOfBytesRead; // [sp+14h] [bp-8h]@8

  for (uint i = 0; i < 3000; ++i)
  {
    if (pSounds[i].uID == uID)
      return pSounds[i].pSoundData;
  }
  FindSound_BinSearch(0, pAudioPlayer->uNumSoundHeaders, pSoundName);
  if ( uFindSound_BinSearch_ResultID == -1 )
    return 0;
  if ( pOutBuff == (SoundData *)-1 )
    pOutBuff = (SoundData *)malloc(pAudioPlayer->pSoundHeaders[uFindSound_BinSearch_ResultID].uDecompressedSize + 4);
  SetFilePointer(pAudioPlayer->hAudioSnd, pAudioPlayer->pSoundHeaders[uFindSound_BinSearch_ResultID].uFileOffset, 0, 0);
  if ( (signed int)pAudioPlayer->pSoundHeaders[uFindSound_BinSearch_ResultID].uCompressedSize >= (signed int)pAudioPlayer->pSoundHeaders[uFindSound_BinSearch_ResultID].uDecompressedSize )
  {
    pAudioPlayer->pSoundHeaders[uFindSound_BinSearch_ResultID].uCompressedSize = pAudioPlayer->pSoundHeaders[uFindSound_BinSearch_ResultID].uDecompressedSize;
    if ( pAudioPlayer->pSoundHeaders[uFindSound_BinSearch_ResultID].uDecompressedSize )
    {
      ReadFile(pAudioPlayer->hAudioSnd, (char *)pOutBuff + 4, pAudioPlayer->pSoundHeaders[uFindSound_BinSearch_ResultID].uDecompressedSize, &NumberOfBytesRead, 0);
    }
    else
    {
      MessageBoxW(nullptr, L"Can't load sound file!", L"E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\Sound.cpp:448", 0);
    }
  }
  else
  {
    uID = (unsigned int)malloc(pAudioPlayer->pSoundHeaders[uFindSound_BinSearch_ResultID].uCompressedSize);
    ReadFile(pAudioPlayer->hAudioSnd, (LPVOID)uID, pAudioPlayer->pSoundHeaders[uFindSound_BinSearch_ResultID].uCompressedSize, &NumberOfBytesRead, 0);
    zlib::MemUnzip((char *)pOutBuff + 4, &pAudioPlayer->pSoundHeaders[uFindSound_BinSearch_ResultID].uDecompressedSize, (const void *)uID, pAudioPlayer->pSoundHeaders[uFindSound_BinSearch_ResultID].uCompressedSize);
    free((void *)uID);
  }
  if ( pOutBuff )
  {
    *(int *)pOutBuff = pAudioPlayer->pSoundHeaders[uFindSound_BinSearch_ResultID].uDecompressedSize;
    uLastLoadedSoundID = 0;
    if ( pSounds[0].pSoundData )
    {
      for ( uint i = 0; pSounds[i].pSoundData; i++ )
        ++uLastLoadedSoundID;
    }
    strcpy((char *)pSounds[uLastLoadedSoundID].SoundName, pSoundName);
    pSoundList->uTotalLoadedSoundSize += pAudioPlayer->pSoundHeaders[uFindSound_BinSearch_ResultID].uDecompressedSize;
    pSounds[uLastLoadedSoundID].pSoundData = pOutBuff;
    return pOutBuff;
  }
  else
    return 0;
}
