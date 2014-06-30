#pragma once
#include <windows.h>
#include <stdio.h>



inline void Assert(bool condition, const char *format, ...)
{
  if (condition)
    return;
  
  va_list va;
  va_start(va, format);
    char msg[4096];
    vsprintf(msg, format, va);
    MessageBoxA(nullptr, msg, "Assert", 0);
  va_end(va);

  __debugbreak();
}

inline void Error(const char *format, ...)
{
  va_list va;
  va_start(va, format);
    char msg[4096];
    vsprintf(msg, format, va);
    MessageBoxA(nullptr, msg, "Error", 0);
  va_end(va);

  __debugbreak();
}



inline void log(char *format, ...)
{
  va_list va;
  va_start(va, format);
  char msg[256];
  vsprintf(msg, format, va);
  va_end(va);
  DWORD w;

  WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), msg, strlen(msg), &w, 0);
}