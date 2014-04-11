#pragma once

#include <time.h>

struct CityProperties
{
public:
  CityProperties();
  CityProperties(int iHeight, int iWidth);
  CityProperties(int iHeight, int iWidth, int iSeed);

  int height, width, seed, top, left, bottom, right;
};

/// Randomly populate a city using fractal generation
void randomgen(struct City& city, struct CityProperties const& props);
