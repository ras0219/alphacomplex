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

struct Garbage : SmartEntity {
  Garbage(int x_, int y_) : SmartEntity(x_, y_) {
    //cerr << "Created garbage " << (unsigned long)this << endl;
  }
  ~Garbage() {
    //cerr << "Deleted garbage " << (unsigned long)this << endl;
  }

  static const char* RAWNAME;
  virtual const char* rawname() const { return RAWNAME; }

  virtual void render(Graphics& g) const {
    g.putChar(x, y, ';');
  }
};

Job* make_garbage_job(Garbage* g);
