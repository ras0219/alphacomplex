#pragma once
#ifdef _WIN32
// Compatibility layer functions for Windows

#include <cstdarg>

// Port of C99's sprintf/vsnprintf since MSVC doesn't support them...
int vsnsprintf(char* str, size_t size, const char* format, va_list ap);
int snprintf(char* str, size_t size, const char* format, ...);

#endif
