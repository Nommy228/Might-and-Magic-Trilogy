#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <array>
#include <windows.h>

#include "Registry.h"

//----- (004649EF) --------------------------------------------------------
int __fastcall ReadWindowsRegistryInt(const char *pKey, int uDefValue)
{
  int v3; // [sp+4h] [bp-24h]@1
  DWORD cbData; // [sp+8h] [bp-20h]@1
  LPCSTR lpValueName; // [sp+Ch] [bp-1Ch]@1
  DWORD dwDisposition; // [sp+10h] [bp-18h]@2
  BYTE Data[4]; // [sp+14h] [bp-14h]@5
  HKEY hKey; // [sp+18h] [bp-10h]@1
  HKEY phkResult; // [sp+1Ch] [bp-Ch]@1
  HKEY v10; // [sp+20h] [bp-8h]@1
  HKEY v11; // [sp+24h] [bp-4h]@1

  v3 = uDefValue;
  lpValueName = pKey;
  v11 = 0;
  v10 = 0;
  hKey = 0;
  phkResult = 0;
  cbData = 4;
  if ( !RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE", 0, KEY_READ|KEY_WOW64_32KEY, &hKey) )// for 64 bit
  {
    if ( !RegCreateKeyExA(hKey, "New World Computing", 0, "", 0, 0xF003Fu, 0, &phkResult, &dwDisposition) )
    {
      if ( !RegCreateKeyExA(phkResult, "Might and Magic VII", 0, "", 0, 0xF003Fu, 0, &v10, &dwDisposition) )
      {
        if ( !RegCreateKeyExA(v10, "1.0", 0, "", 0, 0xF003Fu, 0, &v11, &dwDisposition) )
        {
          if ( RegQueryValueExA(v11, lpValueName, 0, 0, Data, &cbData) )
          {
            *(int *)Data = v3;
            RegSetValueExA(v11, lpValueName, 0, 4u, Data, 4u);
          }
          RegCloseKey(v11);
        }
        RegCloseKey(v10);
      }
      RegCloseKey(phkResult);
    }
    RegCloseKey(hKey);
  }
  return *(int *)Data;
}

//----- (00464B02) --------------------------------------------------------
void __fastcall WriteWindowsRegistryString(const char *pKey, const char *pString)
{
  size_t v2; // eax@5
  const char *lpValueName; // [sp+4h] [bp-1Ch]@1
  const char *Str; // [sp+8h] [bp-18h]@1
  DWORD dwDisposition; // [sp+Ch] [bp-14h]@2
  HKEY hKey; // [sp+10h] [bp-10h]@1
  HKEY phkResult; // [sp+14h] [bp-Ch]@1
  HKEY v8; // [sp+18h] [bp-8h]@1
  HKEY v9; // [sp+1Ch] [bp-4h]@1

  Str = pString;
  lpValueName = pKey;
  v9 = 0;
  v8 = 0;
  hKey = 0;
  phkResult = 0;
  if ( !RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE", 0, 0x2001Fu, &hKey) )
  {
    if ( !RegCreateKeyExA(hKey, "New World Computing", 0, "", 0, 0xF003Fu, 0, &phkResult, &dwDisposition) )
    {
      if ( !RegCreateKeyExA(phkResult, "Might and Magic VII", 0, "", 0, 0xF003Fu, 0, &v8, &dwDisposition) )
      {
        if ( !RegCreateKeyExA(v8, "1.0", 0, "", 0, 0xF003Fu, 0, &v9, &dwDisposition) )
        {
          v2 = strlen(Str);
          RegSetValueExA(v9, lpValueName, 0, 1u, (const BYTE *)Str, v2 + 1);
          RegCloseKey(v9);
        }
        RegCloseKey(v8);
      }
      RegCloseKey(phkResult);
    }
    RegCloseKey(hKey);
  }
}

//----- (00464BEF) --------------------------------------------------------
void __fastcall ReadWindowsRegistryString(const char *pKeyName, char *pOutString, int uBufLen, const char *pDefaultValue)
{
  LSTATUS (__stdcall *v4)(HKEY); // esi@1
  LSTATUS result; // eax@7
  DWORD Type; // [sp+Ch] [bp-24h]@5
  LPCSTR lpValueName; // [sp+10h] [bp-20h]@1
  DWORD cbData; // [sp+14h] [bp-1Ch]@1
  LPBYTE Dest; // [sp+18h] [bp-18h]@1
  DWORD dwDisposition; // [sp+1Ch] [bp-14h]@2
  HKEY phkResult; // [sp+20h] [bp-10h]@1
  HKEY hKey; // [sp+24h] [bp-Ch]@1
  HKEY v13; // [sp+28h] [bp-8h]@1
  HKEY v14; // [sp+2Ch] [bp-4h]@1

  cbData = uBufLen;
  Dest = (LPBYTE)pOutString;
  lpValueName = pKeyName;
  v14 = 0;
  v13 = 0;
  hKey = 0;
  phkResult = 0;
  v4 = RegCloseKey;
  if ( RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE", 0, 0x2001Fu, &hKey)
    || RegCreateKeyExA(hKey, "New World Computing", 0, "", 0, 0xF003Fu, 0, &phkResult, &dwDisposition)
    || RegCreateKeyExA(phkResult, "Might and Magic VII", 0, "", 0, 0xF003Fu, 0, &v13, &dwDisposition)
    || RegCreateKeyExA(v13, "1.0", 0, "", 0, 0xF003Fu, 0, &v14, &dwDisposition) )
  {
    result = (LSTATUS)strncpy((char *)Dest, pDefaultValue, uBufLen);
  }
  else if ( RegQueryValueExA(v14, lpValueName, 0, &Type, Dest, &cbData) )
  {
    RegCloseKey(v14);
    v14 = 0;
    result = (LSTATUS)strncpy((char *)Dest, pDefaultValue, uBufLen);
  }
  else
  {
    v4 = RegCloseKey;
    RegCloseKey(v14);
    v14 = 0;
    RegCloseKey(v13);
    v13 = 0;
    RegCloseKey(phkResult);
    phkResult = 0;
    result = RegCloseKey(hKey);
    hKey = 0;
  }
  if ( v14 )
    result = v4(v14);
  if ( v13 )
    result = v4(v13);
  if ( hKey )
    result = v4(hKey);
  if ( phkResult )
    result = v4(phkResult);
}

//----- (00464D32) --------------------------------------------------------
void __fastcall WriteWindowsRegistryInt(const char *pKey, int val)
{
  const char *lpValueName; // [sp+4h] [bp-1Ch]@1
  BYTE Data[4]; // [sp+8h] [bp-18h]@1
  DWORD dwDisposition; // [sp+Ch] [bp-14h]@2
  HKEY hKey; // [sp+10h] [bp-10h]@1
  HKEY phkResult; // [sp+14h] [bp-Ch]@1
  HKEY v7; // [sp+18h] [bp-8h]@1
  HKEY v8; // [sp+1Ch] [bp-4h]@1

  *(int *)Data = val;
  lpValueName = pKey;
  v8 = 0;
  v7 = 0;
  hKey = 0;
  phkResult = 0;
  if ( !RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE", 0, 0x2001Fu, &hKey) )
  {
    if ( !RegCreateKeyExA(hKey, "New World Computing", 0, "", 0, 0xF003Fu, 0, &phkResult, &dwDisposition) )
    {
      if ( !RegCreateKeyExA(phkResult, "Might and Magic VII", 0, "", 0, 0xF003Fu, 0, &v7, &dwDisposition) )
      {
        if ( !RegCreateKeyExA(v7, "1.0", 0, "", 0, 0xF003Fu, 0, &v8, &dwDisposition) )
        {
          RegSetValueExA(v8, lpValueName, 0, 4, Data, 4);
          RegCloseKey(v8);
        }
        RegCloseKey(v7);
      }
      RegCloseKey(phkResult);
    }
    RegCloseKey(hKey);
  }
}