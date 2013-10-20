#pragma once
#include "OSAPI.h"







#pragma pack(push, 1)
struct _PROVIDER {char unk_0;};
struct _SAMPLE   {char unk_0;};
//struct _STREAM   {char unk_0;};
struct _REDBOOK  {char unk_0;};
struct _DIG_DRIVER {char unk_0;};
struct _SEQUENCE {char unk_0;};
#pragma pack(pop)



/*   22 */
#pragma pack(push, 1)
struct AudioPlayer_3DSample
{
  inline AudioPlayer_3DSample()
  {
    hSample = 0;
    field_4 = 0;
    field_8 = 0;
    field_C = 0;
  }

  void *hSample;
  int field_4;
  int field_8;
  int field_C;
};
#pragma pack(pop)




/*   26 */
#pragma pack(push, 1)
struct SoundHeader
{
  char pSoundName[40];
  unsigned int uFileOffset;
  unsigned int uCompressedSize;
  unsigned int uDecompressedSize;
};
#pragma pack(pop)

/*   27 */
#pragma pack(push, 1)
struct MixerChannel
{
  inline MixerChannel():
    hSample(nullptr), source_pid(0), uSourceTrackIdx(0), uSourceTrackID(0)
  {}

  _SAMPLE *hSample;
  int source_pid;
  unsigned int uSourceTrackIdx;
  unsigned int uSourceTrackID;
};
#pragma pack(pop)





/*  308 */
enum SoundID
{
  SOUND_Invalid = 0,
  SOUND_EnteringAHouse = 0x6,
  SOUND_7 = 7,
  SOUND_8 = 0x8,
  SOUND_24 = 24,
  SOUND_error = 27,
  SOUND_64 = 64,
  SOUND_Button = 66,
  SOUND_67 = 67,
  SOUND_71 = 71,
  SOUND_Button2 = 75,
  SOUND_78 = 78,
  SOUND_80 = 80,
  SOUND_81 = 81,
  SOUND_83 = 83,
  SOUND_84 = 84,
  SOUND_85 = 85,
  SOUND_103 = 103,
  SOUND_Arcomage_LoseResources = 0x78,
  SOUND_Arcomage_AddResources = 0x79,
  SOUND_Arcomage_TowerWallDamage = 0x7A,
  SOUND_Arcomage_DrawCard = 0x7B,
  SOUND_Arcomage_124 = 0x7C,
  SOUND_Arcomage_ProductionDamage = 0x7D,
  SOUND_Arcomage_ProductionUpgrade = 0x7E,
  SOUND_Arcomage_127 = 0x7F,
  SOUND_Arcomage_128 = 0x80,
  SOUND_Arcomage_TowerUpgrade = 0x81,
  SOUND_Arcomage_130 = 0x82,
  SOUND_Arcomage_131 = 0x83,
  SOUND_Arcomage_WallUpgrade = 0x84,
  SOUND_PlayLute = 133, // 85
  SOUND_PlayFaeriePipes = 134, // 86
  SOUND_PlayGryphonheartsTrumpet = 135, // 87
  SOUND_GoldReceived = 0xC8,
  SOUND_203 = 203,
  SOUND_206 = 206,
  SOUND_207 = 207,
  SOUND_OpenChest = 208,
  SOUND_PlayerCantCastSpell = 0xD1,
  SOUND_EatApple = 211, // D3
  SOUND_Bell = 0xD9,
  SOUND_OpenBook = 230,
  SOUND_CloseBook = 231,
  SOUND_11090 = 11090,
  SOUND_12040 = 12040,
  SOUND_20001 = 0x4E21,
};


enum MusicID: unsigned __int32
{
  MUSIC_Credits = 15
};

/*   20 */
#pragma pack(push, 1)
struct AudioPlayer
{
  //----- (004A9669) --------------------------------------------------------
  AudioPlayer():
    bPlayerReady(false), b3DSoundInitialized(false),
    hAILRedbook(nullptr), hStream(nullptr)
  {
	/*AudioPlayer_3DSample *v0; //ecx@1
	signed int v1; //edi@1

	v0 = p3DSamples;
	v1 = 32;
	do
	{
		v0->field_4 = 0;
		v0->field_8 = 0;
		v0->field_C = 0;
		v0->hSample = 0;
		++v0;
		--v1;
	}
	while (v1);*/
    uMixerChannels = 16;
    field_2D0_time_left = 256;
    uNumRedbookTracks = 0;
    uCurrentMusicTrackLength = 0;
    field_2D4 = 0;
    s3DSoundVolume = 127;
  }

  void SetMusicVolume(int vol);
  void SetMasterVolume(float fVolume);
  void _4AA258(int a2);
  void PlaySound(SoundID eSoundID, signed int a3, unsigned int uNumRepeats, signed int a5, signed int a6, int a7, unsigned int uVolume, int sPlaybackRate);
  void UpdateSounds();
  void StopChannels(int uStartChannel, int uEndChannel);
  void LoadAudioSnd();
  void Initialize();
  LSTATUS CheckA3DSupport(char a2);
  void Release();
  void FreeChannel(MixerChannel *pChannel);
  void _4ABF23(AudioPlayer_3DSample *a2);
  void SetEAXPreferences();
  void SetMapEAX();
  int _4AC0A2();
  void PlayMusicTrack(enum MusicID eTrack);
  void  MessWithChannels();


  unsigned int bEAXSupported;
  unsigned int b3DSoundInitialized;
  int s3DSoundVolume;
  struct _PROVIDER *h3DSoundProvider;
  int uNum3DSamples;
  struct AudioPlayer_3DSample p3DSamples[32];
  int field_214;
  int sRedbookVolume;
  char p3DSoundProvider[128];
  unsigned int bPlayerReady;
  //HWND hWindow;
  class OSWindow *window;
  struct _REDBOOK *hAILRedbook;
  struct _DIG_DRIVER *hDigDriver;
  int dword_0002AC;
  struct _SEQUENCE *hSequence;
  int dword_0002B4;
  struct SoundHeader *pSoundHeaders;
  HANDLE hAudioSnd;
  unsigned int uNumSoundHeaders;
  unsigned int uMasterVolume;
  int dword_0002C8;
  int dword_0002CC;
  int field_2D0_time_left;
  int field_2D4;
  unsigned int uCurrentMusicTrackLength;
  unsigned int uNumRedbookTracks;
  unsigned int uCurrentMusicTrackStartMS;
  unsigned int uCurrentMusicTrackEndMS;
  struct MixerChannel pMixerChannels[16];
  int uMixerChannels;
  int field_3EC;
  char pDeviceNames[16][128];
  int array_000BF0[16];
  int array_000C30[16];
  unsigned int uNumDevices;
  struct _STREAM *hStream;
  char field_C78[8];
  int cGameCDDriveLetter;
};
#pragma pack(pop)








/*  325 */
enum SOUND_DESC_TYPE : __int32
{
  SOUND_DESC_LEVEL = 0x0,
  SOUND_DESC_SYSTEM = 0x1,
  SOUND_DESC_SWAP = 0x2,
  SOUND_DESC_3 = 0x3,
  SOUND_DESC_LOCK = 0x4,
};

/*  326 */
enum SOUND_DESC_FLAGS
{
  SOUND_DESC_LOCKED = 0x1,
  SOUND_DESC_3D = 0x2,
};




#pragma pack(push, 1)
struct SoundData
{
  unsigned int uDataSize;
  char         pData[1];
};

struct SoundDesc_mm6
{
  inline bool Is3D()  {return (uFlags & SOUND_DESC_3D) != 0;}

  char pSoundName[32];
  unsigned int uSoundID;
  SOUND_DESC_TYPE eType;
  int uFlags;
  SoundData *pSoundData[17];
};

struct SoundDesc: public SoundDesc_mm6
{
  void *p3DSound;
  int bDecompressed;
};
#pragma pack(pop)



#pragma pack(push, 1)
struct SoundList
{
  inline SoundList():
    sNumSounds(0), pSounds(nullptr), uTotalLoadedSoundSize(0)
  {}

  void Initialize();
  __int16 LoadSound(int a1, unsigned int a3);
  int LoadSound(unsigned int a2, LPVOID lpBuffer, int uBufferSizeLeft, int *pOutSoundSize, int a6);
  SoundDesc *Release();
  void _4A9D79(int a2);
  void UnloadSound(unsigned int uSoundID, char a3);
  void ToFile();
  void FromFile(void *data_mm6, void *data_mm7, void *data_mm8);
  int FromFileTxt(const char *Args);

  signed int sNumSounds;
  SoundDesc *pSounds;
  unsigned int uTotalLoadedSoundSize;
};
#pragma pack(pop)





/*  241 */
#pragma pack(push, 1)
struct Sound
{
  unsigned int uID;
  char SoundName[120];
  SoundData *pSoundData;
};
#pragma pack(pop)




extern int Aureal3D_SplashScreen;
extern int Aureal3D_SplashAudio;
extern int uFindSound_BinSearch_ResultID; // weak
extern int uLastLoadedSoundID; // weak
extern int sLastTrackLengthMS;
extern std::array<Sound, 3000> pSounds;
extern AudioPlayer *pAudioPlayer;
extern SoundList *pSoundList;

extern unsigned __int8 uSoundVolumeMultiplier;
extern unsigned __int8 uVoicesVolumeMultiplier;
extern unsigned __int8 uMusicVolimeMultiplier;
extern int bWalkSound; // idb

extern std::array<float, 10> pSoundVolumeLevels; // idb








/*  379 */
#pragma pack(push, 1)
struct stru339_spell_sound
{
  int _494836(int uSoundID, int a6);

  char pSounds[44744];
  int field_AEC8[45];
  int field_AF7C[18];
  int field_AFC4;
  int pSoundsSizes[2];
  int pSoundsOffsets[2];
};
#pragma pack(pop)
extern std::array<stru339_spell_sound, 4> stru_A750F8;
extern std::array<stru339_spell_sound, 4> stru_AA1058;
