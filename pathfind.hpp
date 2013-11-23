#pragma once

#include <array>
#include <vector>

#include "defs.hpp"

using namespace std;

const array<point, 4> offs = {{ { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } }};

struct City;
vector<point> pathfind(City& c, int x1, int y1, int x2, int y2);
