#pragma once

#include <cstdint>

#include <array>

namespace CastSpellInfoHelpers
{
  void _427D48();
  void _427E01_cast_spell();
};

/*  271 */
#pragma pack(push, 1)
struct CastSpellInfo
{
  //----- (00426987) --------------------------------------------------------
  inline CastSpellInfo()
  {
    memset(this, 0, sizeof(*this));
  }

  struct GUIWindow *GetCastSpellInInventoryWindow();

  uint16_t uSpellID;
  uint16_t uPlayerID;
  uint16_t uPlayerID_2;
  __int16 field_6;
  uint16_t uFlags;
  __int16 forced_spell_skill_level;
  int spell_target_pid;
  int sound_id;
};
#pragma pack(pop)

void _42777D_CastSpell_UseWand_ShootArrow(int a1, unsigned int uPlayerID, unsigned int a4, __int16 a5, int a6);