#pragma once

// #include "city.hpp"
#include "pathfind.hpp"
#include "graphics.hpp"

#include <list>
#include <map>
#include <cassert>

using std::list;

struct City;

struct EComp {
  EComp(Kind k_) : k(k_), parent(nullptr) { }
  virtual ~EComp() {}

  inline void set_parent(Ent* p) { parent = p; }
  inline Kind kind() const { return k; }

  template<class T>
  T* as() {
    assert(k == T::StaticKind);
    return static_cast<T*>(this);
  }
  template<class T>
  const T* as() const {
    assert(k == T::StaticKind);
    return static_cast<const T*>(this);
  }

  enum Kind {
    Skilled,
    Renderable,
    AI,
    Clearance,
    Descriptable,
    Position
  };
  const Kind k;
  struct Ent* parent;
};
template<EComp::Kind K>
struct ECompStatic : EComp {
  static const EComp::Kind StaticKind = K;
  ECompStatic() : EComp(K) { }
};
template<EComp::Kind K>
struct ECompStaticList : ECompStatic<K> {
  ECompStaticList() { glob_list.insert(this); ptr = --glob_list.end(); }
  virtual ~ECompStaticList() { glob_list.erase(ptr); }

  typedef list< ECompStaticList<K> > list_t;

  static list_t glob_list;
  list_t::iterator ptr;
};

struct Position {
  int x;
  int y;
  City* c;
};

struct PositionComp : ECompStatic<EComp::Position> {
  PositionComp(Position p) : pos(p) { }

  inline City* city() { return pos.c; }
  inline int x() const { return pos.x; }
  inline int y() const { return pos.y; }

  inline void move(int tx, int ty) {
    remove();
    pos.x = tx;
    pos.y = ty;
    insert();
  }

  // These methods should not normally be called.
  inline void insert() {
    pos.city->ent(pos.x, pos.y).push_back(parent);
  }
  inline void remove() {
    auto v = pos.city->ent(pos.x, pos.y);
    auto it = std::find(v.begin(), v.end(), parent);
    assert(it != v.end());
    v.erase(it);
  }

  Position pos;
};

struct Ent {
  PositionComp pos;

  typedef std::unordered_map<EComp::Kind, EComp*> map_t;

  inline EComp* get(EComp::Kind k) {
    return compmap[k];
  }
  template<class T>
  inline T* get() { return compmap[T::StaticKind]->as<T>(); }

  inline bool has(EComp::Kind k) { return compmap.find(k) != compmap.end(); }
  inline void add(EComp* comp) {
    comp->set_parent(this);
    compmap[comp->kind()] = comp;
  }

  map_t compmap;
};

// inline Ent* make_dwarf(int x, int y, City* c) {
//   Ent* e = new Ent();
//   e->add(new 
// }

// struct Dwarf : Ent {
//   Dwarf(int x_, int y_, City& c, char pic_ = 'S') : AIEntity(x_, y_, c), pic(pic_) { }

//   virtual const char* rawname() const { return RAWNAME; }
//   virtual int description(char* buf, size_t n) const;
//   virtual char render() const;
//   virtual void update();

//   char pic;
//   int energy = 0;
//   Direction facing = EAST;
//   static const char* RAWNAME;
// };
