#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "..\..\MM7.h"
#include "..\..\Render.h"
#include "..\..\Mouse.h"
#include "..\UIBooks.h"
#include "..\..\GUIWindow.h"
#include "..\..\GUIFont.h"
#include "..\..\Party.h"
#include "..\..\AudioPlayer.h"
#include "..\..\LOD.h"
#include "..\..\Viewport.h"
#include "..\..\texts.h"
#include "..\..\mm7_data.h"

std::array<char *, 9> spellbook_texture_filename_suffices = {{"f", "a", "w", "e", "s", "m", "b", "l", "d"}}; // weak

//----- (00411300) --------------------------------------------------------
void LoadSpellbook(unsigned int spell_school)
{
  char pContainer[20]; // [sp+Ch] [bp-1Ch]@7

  byte_506550 = 0;
  if ( pPlayers[uActiveCharacter]->uQuickSpell && (unsigned __int8)pPlayers[uActiveCharacter]->uQuickSpell / 11 == spell_school )
    quick_spell_at_page = (unsigned __int8)pPlayers[uActiveCharacter]->uQuickSpell - 11 * spell_school;
  else
    quick_spell_at_page = 0;

  for (uint i = 1; i <= 11; ++i)
  {
    if (pPlayers[uActiveCharacter]->spellbook.pChapters[spell_school].bIsSpellAvailable[i - 1])
    {
      sprintf(pContainer, "SB%sS%02d", spellbook_texture_filename_suffices[spell_school], pSpellbookSpellIndices[spell_school][i]);
      SBPageSSpellsTextureList[i] = pIcons_LOD->LoadTexturePtr(pContainer, TEXTURE_16BIT_PALETTE);

      sprintf(pContainer, "SB%sC%02d", spellbook_texture_filename_suffices[spell_school], pSpellbookSpellIndices[spell_school][i]);
      SBPageCSpellsTextureList[i] = pIcons_LOD->LoadTexturePtr(pContainer, TEXTURE_16BIT_PALETTE);
    }
  }
}

//----- (00411597) --------------------------------------------------------
void OnCloseBook()
{
  free(pSpellFont);
  pSpellFont = nullptr;
  free(pBookFont);
  pBookFont = nullptr;
  free(pBook2Font);
  pBook2Font = nullptr;
  free(pAutonoteFont);
  pAutonoteFont = nullptr;
  pTexture_mapbordr->Release();
  pAudioPlayer->PlaySound(SOUND_CloseBook, 0, 0, -1, 0, 0, 0, 0);
  pIcons_LOD->RemoveTexturesPackFromTextureList();
  dword_506364 = 0;
}

//----- (00412AF9) --------------------------------------------------------
static void BookUI_Spellbook_DrawCurrentSchoolBackground()
{
  int pTexID = 0;
  if ( uActiveCharacter )
    pTexID = pParty->pPlayers[uActiveCharacter - 1].lastOpenedSpellbookPage;
  pRenderer->DrawTextureIndexed(8, 8, pSpellBookPagesTextr[pTexID]);
  pRenderer->DrawTextureIndexed(476, 450, pSBQuickSpellBtnTextr);
  pRenderer->DrawTextureIndexed(561, 450, pSpellBookCloseBtnTextr);
}

//----- (00412B58) --------------------------------------------------------
void DrawSpellBookContent(Player *player)
{
  Texture *PendingTexture; // edi@1
  Texture *pTexture; // edx@5
  int v10; // eax@13
  unsigned int pX_coord; // esi@18
  unsigned int pY_coord; // edi@18
  Texture *pPageTexture; // eax@21
  signed int v22; // [sp-4h] [bp-24h]@22
  POINT a2; // [sp+18h] [bp-8h]@13

  static unsigned int texture_tab_coord1[9][2]=
      {{406, 9}, {406, 46}, {406, 84}, {406,121}, {407,158}, {405, 196}, {405, 234}, {405, 272}, {405,309} };

  static unsigned int texture_tab_coord0[9][2]=
      {{415, 10}, {415, 46}, {415, 83}, {415,121}, {415,158}, {416, 196}, {416, 234}, {416, 271}, {416,307} };

  BookUI_Spellbook_DrawCurrentSchoolBackground();

  PendingTexture = pIcons_LOD->GetTexture(pIcons_LOD->FindTextureByName("Pending"));
  pRenderer->ClearZBuffer(0, 479);
  if ( (11 * player->lastOpenedSpellbookPage) || ((11 * player->lastOpenedSpellbookPage) + 11) )//??? maybe structure need fix
  {
    for ( uint i = 1; i <= 11; ++i )
    {
      if (player->_achieved_awards_bits[(11 * player->lastOpenedSpellbookPage) + i + 63] )
      {
        if ( SBPageSSpellsTextureList[i] != PendingTexture )
        {
          if ( quick_spell_at_page == i )
            pTexture = SBPageCSpellsTextureList[i];
          else
            pTexture = SBPageSSpellsTextureList[i];
          if ( pTexture->pLevelOfDetail0_prolly_alpha_mask )
          {
            pX_coord = pViewport->uViewportTL_X +  pIconPos[player->lastOpenedSpellbookPage][pSpellbookSpellIndices[player->lastOpenedSpellbookPage][i]].Xpos;
            pY_coord = pViewport->uViewportTL_Y +  pIconPos[player->lastOpenedSpellbookPage][pSpellbookSpellIndices[player->lastOpenedSpellbookPage][i]].Ypos;
            if ( BYTE1(pTexture->pBits) & 2 )
              pRenderer->DrawTextureTransparent(pX_coord, pY_coord, pTexture);
            else
              pRenderer->DrawTextureIndexed(pX_coord, pY_coord, pTexture);
            pRenderer->DrawMaskToZBuffer(pIconPos[player->lastOpenedSpellbookPage][pSpellbookSpellIndices[player->lastOpenedSpellbookPage][i]].Xpos,
                                         pIconPos[player->lastOpenedSpellbookPage][pSpellbookSpellIndices[player->lastOpenedSpellbookPage][i]].Ypos, pTexture, i);
          }
        }
      }
    }
  }

  pMouse->GetCursorPos(&a2);
  v10 = pRenderer->pActiveZBuffer[a2.x + pSRZBufferLineOffsets[a2.y]] & 0xFFFF;
  if ( v10 )
  {
    if ( SBPageCSpellsTextureList[v10]->pLevelOfDetail0_prolly_alpha_mask )
    {
      pX_coord = pViewport->uViewportTL_X + pIconPos[player->lastOpenedSpellbookPage][pSpellbookSpellIndices[player->lastOpenedSpellbookPage][v10]].Xpos;
      pY_coord = pViewport->uViewportTL_Y + pIconPos[player->lastOpenedSpellbookPage][pSpellbookSpellIndices[player->lastOpenedSpellbookPage][v10]].Ypos;
      if ( BYTE1(SBPageCSpellsTextureList[v10]->pBits) & 2 )
        pRenderer->DrawTextureTransparent(pX_coord, pY_coord, SBPageCSpellsTextureList[v10]);
      else
        pRenderer->DrawTextureIndexed(pX_coord, pY_coord, SBPageCSpellsTextureList[v10]);
    }
  }
  pX_coord = (unsigned int)&player->pActiveSkills[12];
  pY_coord = (unsigned int)&player->pActiveSkills[12];
  for ( uint i = 0; i < 9; i++ )
  {
    if ( player->pActiveSkills[12 + i] )
    {
      if ( player->lastOpenedSpellbookPage == i )
      {
        pPageTexture = pTextures_tabs[i][1];
        pX_coord = texture_tab_coord1[i][0];
        pY_coord = texture_tab_coord1[i][1];
      }
      else
      {
        pPageTexture = pTextures_tabs[i][0];
        pX_coord = texture_tab_coord0[i][0];
        pY_coord = texture_tab_coord0[i][1];
      }
      pRenderer->DrawTextureTransparent(pX_coord, pY_coord, pPageTexture);
    }
  }
}