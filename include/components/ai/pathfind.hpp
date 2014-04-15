#pragma once

#include <array>
#include <vector>

#include "defs.hpp"

const std::array<point, 4> offs = {{ { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } }};

struct City;

std::vector<point> pathfind(City& c, int x1, int y1, int x2, int y2);

// Basically does a flood-fill until it finds one
std::vector<point> pathfind(City& c, int x1, int y1, std::vector<point> dests);
