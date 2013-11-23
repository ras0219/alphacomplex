#pragma once

#include "room.hpp"
#include "joblist.hpp"
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

struct GarbageJob : Job {
  GarbageJob(Garbage* g_) : g(g_) { }

  virtual const char* rawname() const { return Garbage::RAWNAME; }
  virtual int description(char* buf, size_t n) const;

  Garbage* g;
};

struct SupplyJob : JobStep {
  SupplyJob(int x_, int y_, Job* j)
    : JobStep(j), x(x_), y(y_) { }

  virtual const char* rawname() const { return RAWNAME; }
  virtual int description(char* buf, size_t n) const;

  static const char* RAWNAME;
  int x, y;
};
