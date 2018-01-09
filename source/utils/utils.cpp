#include "mcv_platform.h"

void dbg(const char* format, ...) {
  va_list argptr;
  va_start(argptr, format);
  char dest[1024 * 16];
  _vsnprintf(dest, sizeof(dest), format, argptr);
  va_end(argptr);
  ::OutputDebugString(dest);
}

void fatal(const char* format, ...) {
  va_list argptr;
  va_start(argptr, format);
  char dest[1024 * 16];
  _vsnprintf(dest, sizeof(dest), format, argptr);
  va_end(argptr);
  ::OutputDebugString(dest);
  MessageBox(nullptr, dest, "Error!", MB_OK);
}


