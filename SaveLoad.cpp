#define _CRT_SECURE_NO_WARNINGS
#include <io.h>
#include <direct.h>
#include "mm7_unsorted_subs.h"
#include "ErrorHandling.h"
#include "ZlibWrapper.h"

#include "SaveLoad.h"
#include "BSPModel.h"
#include "Party.h"
#include "LOD.h"
#include "Outdoor.h"
#include "AudioPlayer.h"
#include "Actor.h"
#include "Chest.h"
#include "Timer.h"
#include "GUIWindow.h"
#include "GUIFont.h"
#include "Overlays.h"
#include "SpriteObject.h"
#include "Viewport.h"
#include "stru123.h"
#include "texts.h"
#include "Log.h"
#include "VideoPlayer.h"
#include "Level/Decoration.h"
#include "Render.h"

#include "mm7_data.h"


struct SavegameList *pSavegameList = new SavegameList;
unsigned int uNumSavegameFiles;
std::array<unsigned int, 45> pSavegameUsedSlots;
std::array<struct RGBTexture, 45> pSavegameThumbnails;
std::array<SavegameHeader, 45> pSavegameHeader;

//----- (0045EE8A) --------------------------------------------------------
void __fastcall LoadGame(unsigned int uSlot)
{
  bool v25; // esi@62
  bool v26; // eax@62
  SavegameHeader header; // [sp+Ch] [bp-E4h]@23
  char Str[123]; // [sp+70h] [bp-80h]@25

  MapsLongTimers_count = 0;
  if (!pSavegameUsedSlots[uSlot])
  {
    pAudioPlayer->PlaySound(SOUND_error, 0, 0, -1, 0, 0, 0, 0);
    Log::Warning(L"LoadGame: slot %u is empty", uSlot);
    return;
  }

  for (uint i = 1; i < 5; ++i)
    for (uint j = 1; j < 6; ++j)
    {
      sprintf(pTmpBuf.data(), "data\\lloyd%d%d.pcx", i, j);
      remove(pTmpBuf.data());
    }

  if (SoundSetAction[24][0])
    for (uint i = 0; i < 4; ++i)
    {
      for (uint j = 0; j < pSoundList->sNumSounds; ++j)
        if (pSoundList->pSL_Sounds[j].uSoundID == 2 * (SoundSetAction[24][0] + 50 * pParty->pPlayers[i].uVoiceID) + 4998)
        {
          pSoundList->UnloadSound(j, 1);
          break;
        }

      for (uint j = 0; j < pSoundList->sNumSounds; ++j)
        if (pSoundList->pSL_Sounds[j].uSoundID == 2 * (SoundSetAction[24][0] + 50 * pParty->pPlayers[i].uVoiceID) + 4999)
        {
          pSoundList->UnloadSound(j, 1);
          break;
        }
    }

  sprintf(pTmpBuf.data(), "saves\\%s", pSavegameList->pFileList[uSlot].pSaveFileName);

  pNew_LOD->CloseWriteFile();
  if (!CopyFileA(pTmpBuf.data(), "data\\new.lod", 0))
    int e = GetLastError();

  pNew_LOD->LoadFile("data\\new.lod", 0);
  FILE *file = pNew_LOD->FindContainer("header.bin", 1);
  if (!file)
  {
    sprintf(Str, pGlobalTXT_LocalizationStrings[612], 100);//����������� ���� ����������! Code=%d
    Log::Warning(L"%S", Str);
    MessageBoxA(nullptr, Str, "E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\LoadSave.cpp:549", 0);
  }
  Assert(sizeof(SavegameHeader) == 100);
  fread(&header, sizeof(SavegameHeader), 1, file);

  file = pNew_LOD->FindContainer("party.bin", 1);
  if (!file)
  {
    sprintf(Str, pGlobalTXT_LocalizationStrings[612], 101);//����������� ���� ����������! Code=%d
    Log::Warning(L"%S", Str);
    MessageBoxA(nullptr, Str, "E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\LoadSave.cpp:559", 0);
  }
  if (sizeof(Party) != 0x16238)
    Log::Warning(L"class Party: deserialization warning");
  fread(pParty, sizeof(Party), 1, file);


  file = pNew_LOD->FindContainer("clock.bin", 1);
  if (!file)
  {
    sprintf(Str, pGlobalTXT_LocalizationStrings[612], 102);//����������� ���� ����������! Code=%d
    Log::Warning(L"%S", Str);
    MessageBoxA(nullptr, Str, "E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\LoadSave.cpp:569", 0);
  }
  if (sizeof(Timer) != 0x28)
    Log::Warning(L"class Timer: deserialization warning");
  fread(pEventTimer, sizeof(Timer), 1, file);

  file = pNew_LOD->FindContainer("overlay.bin", 1);
  if (!file)
  {
    sprintf(Str, pGlobalTXT_LocalizationStrings[612], 103);//����������� ���� ����������! Code=%d
    Log::Warning(L"%S", Str);
    MessageBoxA(nullptr, Str, "E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\LoadSave.cpp:579", 0);
  }
  if (sizeof(OtherOverlayList) != 0x3F0)
    Log::Warning(L"class OtherOverlayList: deserialization warning");
  fread(pOtherOverlayList, sizeof(OtherOverlayList), 1, file);

  file = pNew_LOD->FindContainer("npcdata.bin", 0);
  if (!file)
  {
    sprintf(Str, pGlobalTXT_LocalizationStrings[612], 104);//����������� ���� ����������! Code=%d
    Log::Warning(L"%S", Str);
    MessageBoxA(nullptr, Str, "E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\LoadSave.cpp:590", 0);
  }
  if (sizeof(pNPCStats->pNewNPCData) != 0x94BC)
    Log::Warning(L"NPCStats: deserialization warning");
  fread(pNPCStats->pNewNPCData, sizeof(pNPCStats->pNewNPCData), 1, file);
  pNPCStats->_476C60();

  file = pNew_LOD->FindContainer("npcgroup.bin", 0);
  if (!file)
  {
    sprintf(Str, pGlobalTXT_LocalizationStrings[612], 105);//����������� ���� ����������! Code=%d
    Log::Warning(L"%S", Str);
    MessageBoxA(nullptr, Str, "E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\LoadSave.cpp:600", 0);
  }
  if (sizeof(pNPCStats->pGroups_copy) != 0x66)
    Log::Warning(L"NPCStats: deserialization warning");
  fread(pNPCStats->pGroups_copy, sizeof(pNPCStats->pGroups_copy), 1, file);

  uActiveCharacter = 0;
  for (uint i = 0; i < 4; ++i)
    if (pParty->pPlayers[i].CanAct())
    {
      uActiveCharacter = i + 1;
      break;
    }

  for (uint i = 0; i < 4; ++i)
  {
    if ( pParty->pPlayers[i].uQuickSpell )
      AA1058_PartyQuickSpellSound[i].AddPartySpellSound(pParty->pPlayers[i].uQuickSpell, i + 1);

    for (uint j = 0; j < 2; ++j)
    {
      uint uEquipIdx = pParty->pPlayers[i].pEquipment.pIndices[j];
      if (uEquipIdx)
      {
        int pItemID = pParty->pPlayers[i].pInventoryItemList[uEquipIdx - 1].uItemID;
        if (pItemsTable->pItems[pItemID].uEquipType == EQUIP_WAND && pItemID )//����
        {
          __debugbreak();  // looks like offset in player's inventory and wand_lut much like case in 0042ECB5
          stru_A750F8[i].AddPartySpellSound(wand_spell_ids[pItemID - ITEM_WAND_FIRE], i + 9);
        }
      }
    }
  }

  pGUIWindow_CurrentMenu->Release();
  pCurrentScreen = SCREEN_GAME;

  viewparams->bRedrawGameUI = true;

  SetUserInterface(pParty->alignment, true);

  pEventTimer->Resume();
  pEventTimer->StopGameTime();

  v25 = pGames_LOD->DoesContainerExist(header.pLocationName);
  sprintf(pTmpBuf.data(), "levels\\%s", header.pLocationName);
  v26 = _access(pTmpBuf.data(), 4) != -1;
  if ( !v25 && !v26 )
    Error("Unable to find: %s!", header.pLocationName);

  strcpy(pCurrentMapName, header.pLocationName);
  dword_6BE364_game_settings_1 |= GAME_SETTINGS_2000 | GAME_SETTINGS_0001;

  for (uint i = 0; i < uNumSavegameFiles; ++i)
    pSavegameThumbnails[i].Release();

  pIcons_LOD->RemoveTexturesPackFromTextureList();
  pAudioPlayer->SetMusicVolume(pSoundVolumeLevels[uMusicVolimeMultiplier] * 64.0f);
  pAudioPlayer->SetMasterVolume(pSoundVolumeLevels[uSoundVolumeMultiplier] * 128.0f);
  if (uTurnSpeed)
    pParty->sRotationY = uTurnSpeed * pParty->sRotationY / (signed int)uTurnSpeed;
  MM7Initialization();
  bFlashQuestBook = false;
  viewparams->bRedrawGameUI = true;
}

//----- (0045F469) --------------------------------------------------------
void SaveGame( bool IsAutoSAve, bool NotSaveWorld )
{
  int text_pos; // eax@6
  FILE *pLLoidFile; // edi@24
  char* compressed_buf; // edi@30
  char *data_write_pos; // esi@41
  CHAR Buffer[128]; // [sp+Ch] [bp-264h]@59
  char Dir[255]; // [sp+8Ch] [bp-1E4h]@51
  char Drive[255]; // [sp+ACh] [bp-1C4h]@51
  SavegameHeader save_header; // [sp+CCh] [bp-1A4h]@10
  char Filename[255]; // [sp+130h] [bp-140h]@51
  char Ext[255]; // [sp+150h] [bp-120h]@51
  char Source[32]; // [sp+170h] [bp-100h]@51
  char work_string[120]; // [sp+190h] [bp-E0h]@8
  int pPositionY; // [sp+208h] [bp-68h]@2
  int pPositionX; // [sp+20Ch] [bp-64h]@2
  int sPRotationY; // [sp+210h] [bp-60h]@2
  int sPRotationX; // [sp+214h] [bp-5Ch]@2
  ODMHeader odm_data; // [sp+218h] [bp-58h]@30
  int res; // [sp+224h] [bp-4Ch]@30
  int pPositionZ; // [sp+228h] [bp-48h]@2
  size_t Size; // [sp+250h] [bp-20h]@26
  char *uncompressed_buff; // [sp+258h] [bp-18h]@2
  unsigned int compressed_block_size; // [sp+260h] [bp-10h]@23
 
  //v66 = a2;
  strcpy(byte_6BE3B0.data(), pCurrentMapName);//byte_6BE3B0 - save_map_name
  if (!_stricmp(pCurrentMapName, "d05.blv")) // arena
    return;

  uncompressed_buff = (char*)malloc(1000000);

  LOD::Directory pLodDirectory; // [sp+22Ch] [bp-44h]@2
  pPositionX = pParty->vPosition.x;
  pPositionY = pParty->vPosition.y;
  pPositionZ = pParty->vPosition.z;
  sPRotationY = pParty->sRotationY;
  sPRotationX = pParty->sRotationX;
  pParty->vPosition.x = pParty->vPrevPosition.x;
  pParty->vPosition.z = pParty->vPrevPosition.z;
  pParty->vPosition.y = pParty->vPrevPosition.y;

  pParty->uFallStartY = pParty->vPrevPosition.z;

  pParty->sRotationY = pParty->sPrevRotationY;
  pParty->sRotationX = pParty->sPrevRotationX;
  if (uCurrentlyLoadedLevelType == LEVEL_Indoor)
    pIndoor->stru1.uLastVisitDay = pParty->uTimePlayed;
  else
    pOutdoor->loc_time.uLastVisitDay = pParty->uTimePlayed;

  pRenderer->PackScreenshot(150, 112, uncompressed_buff, 1000000, &pLodDirectory.uDataSize);//�������� ���������
  strcpy(pLodDirectory.pFilename, "image.pcx");

  if (pCurrentScreen == SCREEN_SAVEGAME)
  {
    pRenderer->DrawTextureIndexed(8, 8, pIcons_LOD->GetTexture(uTextureID_loadsave));
    pRenderer->DrawTextureIndexed(18, 141, pIcons_LOD->GetTexture(uTextureID_save_up));
    text_pos = pFontSmallnum->AlignText_Center(186, pGlobalTXT_LocalizationStrings[190]);
    pGUIWindow_CurrentMenu->DrawText(pFontSmallnum, text_pos + 25, 219, 0, pGlobalTXT_LocalizationStrings[190], 0, 0, 0); //����������
    text_pos  = pFontSmallnum->AlignText_Center(186, pSavegameHeader[uLoadGameUI_SelectedSlot].pName);
    pGUIWindow_CurrentMenu->DrawTextInRect(pFontSmallnum, text_pos  + 25, 259, 0, pSavegameHeader[uLoadGameUI_SelectedSlot].pName, 185, 0);
    text_pos  = pFontSmallnum->AlignText_Center(186, pGlobalTXT_LocalizationStrings[165]);
    pGUIWindow_CurrentMenu->DrawText(pFontSmallnum, text_pos  + 25, 299, 0, pGlobalTXT_LocalizationStrings[165], 0, 0, 0); //����������, ���������
    pRenderer->Present();
  }

  if (pNew_LOD->Write(&pLodDirectory, uncompressed_buff, 0))
  {
    sprintf(work_string, pGlobalTXT_LocalizationStrings[612], 200); //����������� ���� ����������! Code=%d
    MessageBoxA(nullptr, work_string, "E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\LoadSave.cpp:773", 0);
  }

  Assert(sizeof(SavegameHeader) == 100);
  memset(save_header.pName, 0, 20);
  memset(save_header.pLocationName, 0, 20);
  memset(save_header.field_30, 0, 52);
  strcpy(save_header.pLocationName, pCurrentMapName);
  save_header.uWordTime = pParty->uTimePlayed;
  strcpy(pLodDirectory.pFilename, "header.bin");
  pLodDirectory.uDataSize = sizeof(SavegameHeader);
  if (pNew_LOD->Write(&pLodDirectory, &save_header, 0))
  {
    sprintf(work_string, pGlobalTXT_LocalizationStrings[612], 201);
    MessageBoxA(nullptr, work_string, "E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\LoadSave.cpp:784", 0);
  }
  strcpy(pLodDirectory.pFilename, "party.bin");
  pLodDirectory.uDataSize = sizeof(Party); //90680;
  if ( pNew_LOD->Write(&pLodDirectory, pParty, 0) )
  {
    sprintf(work_string, pGlobalTXT_LocalizationStrings[612], 202);//Save game corrupted!  Code=%d
    MessageBoxA(nullptr, work_string, "E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\LoadSave.cpp:793", 0);
  }
  strcpy(pLodDirectory.pFilename, "clock.bin");
  pLodDirectory.uDataSize =sizeof(Timer);// 40;
  if ( pNew_LOD->Write(&pLodDirectory, pEventTimer, 0) )
  {
    sprintf(work_string, pGlobalTXT_LocalizationStrings[612], 203);
    MessageBoxA(nullptr, work_string, "E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\LoadSave.cpp:802", 0);
  }
  strcpy(pLodDirectory.pFilename, "overlay.bin");
  pLodDirectory.uDataSize =sizeof(OtherOverlayList);// 1008;
  if ( pNew_LOD->Write(&pLodDirectory, pOtherOverlayList, 0) )
  {
    sprintf(work_string, pGlobalTXT_LocalizationStrings[612], 204);
    MessageBoxA(nullptr, work_string, "E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\LoadSave.cpp:811", 0);
  }
  strcpy(pLodDirectory.pFilename, "npcdata.bin");
  pLodDirectory.uDataSize = 501 * sizeof(NPCData);
  Assert(pLodDirectory.uDataSize == 38076);
  if ( pNew_LOD->Write(&pLodDirectory, pNPCStats->pNewNPCData, 0) )
  {
    sprintf(work_string, pGlobalTXT_LocalizationStrings[612], 205);
    MessageBoxA(nullptr, work_string, "E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\LoadSave.cpp:820", 0);
  }
  strcpy(pLodDirectory.pFilename, "npcgroup.bin");
  pLodDirectory.uDataSize = 102;
  if ( pNew_LOD->Write(&pLodDirectory, pNPCStats->pGroups_copy, 0) )
  {
    sprintf(work_string, pGlobalTXT_LocalizationStrings[612], 206);
    MessageBoxA(nullptr, work_string, "E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\LoadSave.cpp:829", 0);
  }
  for (int i =  1; i <= 4; ++i) // 4 - players
  {
    for (int j =  1; j <= 5; ++j) // 5 - images
    {
      sprintf(work_string, "data\\lloyd%d%d.pcx", i, j);
      pLLoidFile = fopen(work_string, "rb");
      if ( pLLoidFile )
      {
        __debugbreak();
        sprintf(work_string, "lloyd%d%d.pcx", i, j);
        fseek(pLLoidFile, 0, SEEK_END);
        pLodDirectory.uDataSize = ftell(pLLoidFile);
        rewind(pLLoidFile);
        fread(uncompressed_buff, pLodDirectory.uDataSize, 1, pLLoidFile);
        strcpy(pLodDirectory.pFilename, work_string);
        fclose(pLLoidFile);
        remove(work_string);
        if ( pNew_LOD->Write(&pLodDirectory, uncompressed_buff, 0) )
        {
          sprintf(work_string, pGlobalTXT_LocalizationStrings[612], 207);
          MessageBoxA(nullptr, work_string, "E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\LoadSave.cpp:857", 0);
          Size = 5080748;
        }
      }
	}
  }
  if ( !NotSaveWorld )//autosave for change location
  {
    //__debugbreak();
    CompactLayingItemsList();
    compressed_buf = (char*)malloc(1000000);
    odm_data.uVersion = 91969;
    odm_data.pMagic[0] = 'm';
    odm_data.pMagic[1] = 'v';
    odm_data.pMagic[2] = 'i';
    odm_data.pMagic[3] = 'i';
    odm_data.uCompressedSize = 0;
    odm_data.uDecompressedSize = 0;
    data_write_pos = uncompressed_buff;
    memcpy((void *)compressed_buf, &odm_data, 0x10);
    if ( uCurrentlyLoadedLevelType == LEVEL_Indoor )
    {
      pIndoor->dlv.uNumFacesInBModels = pIndoor->uNumFaces;
      pIndoor->dlv.uNumBModels = 0;
      pIndoor->dlv.uNumDecorations = uNumLevelDecorations;
      memcpy(data_write_pos, &pIndoor->dlv,sizeof(DDM_DLV_Header) );//0x28
      data_write_pos += sizeof(DDM_DLV_Header);
      memcpy(data_write_pos, pIndoor->_visible_outlines, 0x36B);
      data_write_pos += 875;
      for (int i =  0; i <(signed int)pIndoor->uNumFaces; ++i)
      {
        memcpy(data_write_pos, &pIndoor->pFaces[i].uAttributes, 4);
        data_write_pos += 4;
      }

      for (int i =  0; i <(signed int)uNumLevelDecorations; ++i)
      {
        memcpy(data_write_pos, &pLevelDecorations[i].uFlags, 2);
        data_write_pos += 2;
      }
      memcpy(data_write_pos, &uNumActors, 4);
      data_write_pos += 4;
      memcpy(data_write_pos, &pActors, uNumActors * sizeof(Actor));
      data_write_pos += uNumActors * sizeof(Actor);
      memcpy(data_write_pos, &uNumSpriteObjects, 4);
      data_write_pos += 4;
      memcpy(data_write_pos, pSpriteObjects.data(), 112 * uNumSpriteObjects);
      data_write_pos += 112 * uNumSpriteObjects;
      memcpy(data_write_pos, &uNumChests, 4);
      data_write_pos += 4;
      memcpy(data_write_pos, pChests.data(), sizeof(Chest)*uNumChests);//5324 *
      data_write_pos += sizeof(Chest)* uNumChests;
      memcpy(data_write_pos, pIndoor->pDoors, 0x3E80);
      data_write_pos += 16000;
      memcpy(data_write_pos, pIndoor->ptr_0002B4_doors_ddata, pIndoor->blv.uDoors_ddata_Size);
      data_write_pos += pIndoor->blv.uDoors_ddata_Size;
      memcpy(data_write_pos, &stru_5E4C90_MapPersistVars, 0xC8);
      data_write_pos += 200;
      memcpy(data_write_pos, &pIndoor->stru1, 0x38);
      data_write_pos += 56;

    }
    else//for Outdoor
    {
      pOutdoor->ddm.uNumFacesInBModels = 0;
      for ( int i = 0; i < pOutdoor->uNumBModels; ++i )
        pOutdoor->ddm.uNumFacesInBModels += pOutdoor->pBModels[i].uNumFaces;
      pOutdoor->ddm.uNumBModels = pOutdoor->uNumBModels;
      pOutdoor->ddm.uNumDecorations = uNumLevelDecorations;
      memcpy(data_write_pos, &pOutdoor->ddm, sizeof(DDM_DLV_Header));//0x28
      data_write_pos += sizeof(DDM_DLV_Header);
      memcpy(data_write_pos, pOutdoor->uFullyRevealedCellOnMap, 0x3C8);
      data_write_pos += 968;
      memcpy(data_write_pos, pOutdoor->uPartiallyRevealedCellOnMap, 0x3C8);
      data_write_pos += 968;
      for (int i =  0; i < pOutdoor->uNumBModels ; ++i)  
        for (int j =  0; j < pOutdoor->pBModels[i].uNumFaces;++j)//*(int *)&pOutdoor->pBModels->pModelName[v24]; ++j)
        {
          memcpy(data_write_pos, &(pOutdoor->pBModels[i].pFaces[j].uAttributes), 4);
          data_write_pos += 4;
        }

      for (int i = 0; i < (signed int)uNumLevelDecorations; ++i)
      {
        memcpy(data_write_pos, &pLevelDecorations[i].uFlags, 2);
        data_write_pos += 2;
      }
      memcpy(data_write_pos, &uNumActors, 4);
      data_write_pos += 4;
      memcpy(data_write_pos, &pActors, uNumActors * sizeof(Actor));
      data_write_pos +=  uNumActors * sizeof(Actor);
      memcpy(data_write_pos, &uNumSpriteObjects, 4);
      data_write_pos += 4;
      memcpy(data_write_pos, &pSpriteObjects, uNumSpriteObjects * sizeof(SpriteObject));
      data_write_pos += uNumSpriteObjects * sizeof(SpriteObject);
      memcpy(data_write_pos, &uNumChests, 4);
      data_write_pos += 4;
      memcpy(data_write_pos, pChests.data(), sizeof(Chest)* uNumChests);
      data_write_pos += sizeof(Chest) * uNumChests;
      memcpy(data_write_pos, &stru_5E4C90_MapPersistVars, 0xC8);
      data_write_pos += 200;
      memcpy(data_write_pos, &pOutdoor->loc_time, 0x38);
      data_write_pos += 56;
    }
    strcpy(Source, pCurrentMapName);
    _splitpath(Source, Drive, Dir, Filename, Ext);
    Ext[1] = 'd';
   
    Size = (int)data_write_pos - (int)uncompressed_buff;
    compressed_block_size = 999984;
    res = zlib::MemZip((char *)compressed_buf + 16, (unsigned int *)&compressed_block_size, uncompressed_buff,Size);
    if (res || (signed int)compressed_block_size > (signed int)Size )
    {
      memcpy((void *)(compressed_buf + 16), uncompressed_buff, Size);
      compressed_block_size = Size;
    }
    compressed_block_size += 16;
    memcpy(&((ODMHeader *)compressed_buf)->uCompressedSize, &compressed_block_size, 4);
    memcpy(&((ODMHeader *)compressed_buf)->uDecompressedSize, &Size, 4);
    sprintf(Source, "%s%s", Filename, Ext);
    strcpy(pLodDirectory.pFilename, Source);
    pLodDirectory.uDataSize = compressed_block_size;
    if ( pNew_LOD->Write(&pLodDirectory, (const void *)compressed_buf, 0) )
    {
      sprintf(work_string, pGlobalTXT_LocalizationStrings[612], 208);
      MessageBoxA(nullptr, work_string, "E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\LoadSave.cpp:1071", 0);
    }
    free((void *)compressed_buf);
  }
  free(uncompressed_buff);
  if ( IsAutoSAve )
  {
    if ( !CopyFileA("data\\new.lod", "saves\\autosave.mm7", 0) )
    {
      FormatMessageA(0x1000, 0, GetLastError(), 0x400, Buffer, 0x80, 0);
      sprintf(work_string, pGlobalTXT_LocalizationStrings[612], 300);
      MessageBoxA(nullptr, work_string, "E:\\WORK\\MSDEV\\MM7\\MM7\\Code\\LoadSave.cpp:1097", 0);
    }
  }
  pParty->vPosition.x = pPositionX;
  pParty->vPosition.y = pPositionY;
  pParty->vPosition.z = pPositionZ;
  pParty->uFallStartY = pPositionZ;
  pParty->sRotationY = sPRotationY;
  pParty->sRotationX = sPRotationX;
}

//----- (00460078) --------------------------------------------------------
void __fastcall DoSavegame(unsigned int uSlot)
{
  if ( _stricmp(pCurrentMapName, "d05.blv") )//Not Arena(�� �����)
  {
    LOD::Directory pDir; // [sp+Ch] [bp-28h]@2
    SaveGame(0, 0);
    strcpy(pSavegameHeader[uSlot].pLocationName, pCurrentMapName);//���� �������� �����
    pSavegameHeader[uSlot].uWordTime = pParty->uTimePlayed;//������� �����
    strcpy(pDir.pFilename, "header.bin");
    pDir.uDataSize = 100;
    pNew_LOD->Write(&pDir, &pSavegameHeader[uSlot], 0);
    sprintf(pTmpBuf.data(), "saves\\save%03d.mm7", uSlot);
    pNew_LOD->CloseWriteFile();//������� 
    CopyFileA("data\\new.lod", pTmpBuf.data(), 0);//���������� ����� � ���������� saves
  }
  GUI_UpdateWindows();
  pGUIWindow_CurrentMenu->Release();
  pCurrentScreen = SCREEN_GAME;
  //v3 = pSavegameThumbnails;
  viewparams->bRedrawGameUI = true;
  for (uint i = 0; i < 45; i++)
    pSavegameThumbnails[i].Release();

  if ( _stricmp(pCurrentMapName, "d05.blv") )
    pNew_LOD->_4621A7();
  else
    ShowStatusBarString(pGlobalTXT_LocalizationStrings[583], 2);// "No saving in the Arena"
  pIcons_LOD->RemoveTexturesFromTextureList();
  pEventTimer->Resume();
  ShowStatusBarString(pGlobalTXT_LocalizationStrings[656], 2);// "Game Saved!"
  viewparams->bRedrawGameUI = true;
}
// 4E28F8: using guessed type int pCurrentScreen;

//----- (0045E297) --------------------------------------------------------
void SavegameList::Initialize(unsigned int bHideEmptySlots)
{
  //memset(pSavegameList, 0, sizeof(pSavegameList));//Ritor1: �������� ���������
  pSavegameList->Reset();
  uNumSavegameFiles = 0;

  _chdir("saves");
  {
    if (!bHideEmptySlots && _access(pGlobalTXT_LocalizationStrings[613], 0) != -1 ) // AutoSave.MM7
      strcpy(pSavegameList->pFileList[uNumSavegameFiles++].pSaveFileName, pGlobalTXT_LocalizationStrings[613]);

    for (uint i = 0; i < 40; ++i)
    {
      sprintf(pTmpBuf.data(), "save%03d.mm7", i);
      if (_access(pTmpBuf.data(), 0) == -1)
        continue;

      uint idx = i;
      if (!bHideEmptySlots)
        idx = uNumSavegameFiles;
      strcpy(pSavegameList->pFileList[idx].pSaveFileName, pTmpBuf.data());

      ++uNumSavegameFiles;
    }
  }
  _chdir("..");
}
//----- (0046086A) --------------------------------------------------------
void SaveNewGame()
{
  FILE *file; // eax@7
  void *pSave; // [sp+170h] [bp-8h]@3

  if ( pMovie )
    pVideoPlayer->Unload();
  pSave = malloc(1000000);
  pNew_LOD->CloseWriteFile();
  remove("data\\new.lod");//������� new.lod

  LOD::FileHeader header; // [sp+Ch] [bp-16Ch]@3 ���������
  strcpy(header.LodVersion, "MMVII");
  strcpy(header.LodDescription, "newmaps for MMVII");
  header.LODSize = 100;
  header.dword_0000A8 = 0;

  LOD::Directory a3; // [sp+14Ch] [bp-2Ch]@3
  a3.dword_000018 = 0;
  a3.word_00001E = 0;
  strcpy(a3.pFilename, "current");
  pNew_LOD->CreateNewLod(&header, &a3, "data\\new.lod");//�������� new.lod � �����������
  if (pNew_LOD->LoadFile("data\\new.lod", false))//��������� ���� new.lod(isFileOpened = true)
  {
    pNew_LOD->CreateTempFile();//�������� ��������� ���� OutputFileHandle
    pNew_LOD->uNumSubDirs = 0;

    LOD::Directory pDir; // [sp+10Ch] [bp-6Ch]@4
    for (int i = pGames_LOD->uNumSubDirs / 2; i < pGames_LOD->uNumSubDirs; ++i)//����������� ������ � 76 �� 151
    {
      memcpy(&pDir, &pGames_LOD->pSubIndices[i], sizeof(pDir));//����������� �������� ����� � pDir
      file = pGames_LOD->FindContainer(pGames_LOD->pSubIndices[i].pFilename, 1);
      fread(pSave, pGames_LOD->pSubIndices[i].uDataSize, 1, file);
      pNew_LOD->AppendDirectory(&pDir, pSave);
    }

    LOD::Directory save_game_dir; // [sp+12Ch] [bp-4Ch]@9
    strcpy(pSavegameHeader[0].pLocationName, "out01.odm");
    strcpy(save_game_dir.pFilename, "header.bin");
    save_game_dir.uDataSize = sizeof(SavegameHeader);
    pNew_LOD->AppendDirectory(&save_game_dir, &pSavegameHeader[0]);

    pNew_LOD->FixDirectoryOffsets();

    pParty->vPrevPosition.x = 12552;
    pParty->vPrevPosition.y = 1816;
    pParty->vPrevPosition.z = 0;

    pParty->vPosition.x = 12552;
    pParty->vPosition.y = 1816;
    pParty->vPosition.z = 0;

    pParty->uFallStartY = 0;

    pParty->sPrevRotationX = 0;
    pParty->sPrevRotationY = 512;

    pParty->sRotationX = 0;
    pParty->sRotationY = 512;

    SaveGame(1, 1);
  }
  free(pSave);
}

