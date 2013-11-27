#pragma once

#include "room.hpp"
#include "job.hpp"
#include "entity.hpp"

struct CleaningRoom : Room {
  CleaningRoom(int x_, int y_, int w_, int h_)
    : Room(x_,y_,w_,h_) { }

  virtual const char* rawname() { return RAWNAME; }

  static const char* RAWNAME;
};

struct Garbage : LocEntity {
  Garbage(int x_, int y_, City& c) : LocEntity(x_, y_, c) { }

  static const char* RAWNAME;
  virtual const char* rawname() const { return RAWNAME; }

  virtual char render() const { return ';'; }
};

Job* make_garbage_job(Garbage* g, City& city);
