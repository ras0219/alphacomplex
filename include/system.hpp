#pragma once

struct Ent;

struct System {
  virtual void update() = 0;
  virtual void insert(Ent* e) = 0;
  virtual void erase(Ent* e) = 0;
};
