#pragma once

#include <array>
#include <vector>

#include "defs.hpp"
#include "city.hpp"

using namespace std;

constexpr array<point, 4> offs = {{ { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } }};

vector<point> pathfind(City& c, int x1, int y1, int x2, int y2);
