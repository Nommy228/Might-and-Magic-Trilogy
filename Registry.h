#pragma once



int __fastcall ReadWindowsRegistryInt(const char *pKey, int uDefValue); // idb
void __fastcall WriteWindowsRegistryString(const char *pKey, const char *pString);
void __fastcall ReadWindowsRegistryString(const char *pKeyName, char *pOutString, int uBufLen, const char *pDefaultValue);
void __fastcall WriteWindowsRegistryInt(const char *pKey, int val);