#pragma once

#include <time.h>

struct CityProperties
{
public:
  CityProperties(){}
  CityProperties(int iHeight, int iWidth)
  {
    height = iHeight;
    width = iWidth;
    seed = (int)time(0);
    top = 0;
    left = 0;
    bottom = height - 1;
    right = width - 1;
  }

  CityProperties(int iHeight, int iWidth, int iSeed)
  {
    height = iHeight;
    width = iWidth;
    seed = iSeed;
    top = 0;
    left = 0;
    bottom = height - 1;
    right = width - 1;
  }

  int height, width, seed, top, left, bottom, right;
};

/// Randomly populate a city using fractal generation
void randomgen(struct City& city, struct CityProperties const& props);
