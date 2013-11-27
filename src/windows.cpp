#include "windows.hpp"

#include <cstdio>
#include <cstdlib>

int vsnsprintf(char* str, size_t size, const char* format, va_list ap) {
  int retval = -1;

  if (size != 0) {
    retval = vsnprintf_s(str, size, _TRUNCATE, format, ap);
  }

  if (retval == -1) {
    retval = _vscprintf(format, ap);
  }

  return retval;
}

int snprintf(char* str, size_t size, const char* format, ...) {
  int retval;
  va_list ap;

  va_start(ap, format);
  retval = vsnsprintf(str, size, format, ap);
  va_end(ap);

  return retval;
}