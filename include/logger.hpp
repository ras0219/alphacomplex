#pragma once

#include <stdio.h>
#include <time.h>
#include <iostream>
#include <string>

#define LOG_PREFIX "log-"

//feel free to C++ this. 
//as long as we have a printf-like-function
struct Logger {
  std::string file_location;
  FILE *out_file;

  Logger(const char* filename);
  ~Logger();
  
  //return: bytes written, or errors
  //adds timestamp at start, \n at end
  int Write(const char* format, ...);


};
