#pragma once

#include "joblist.hpp"
#include "entity.hpp"

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
  Garbage* g;

  virtual const char* rawname() const { return Garbage::RAWNAME; }

  virtual int description(char* buf, size_t n) const;

  GarbageJob(Garbage* g_) : g(g_) { }
};

