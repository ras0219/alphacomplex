#pragma once

#include "city.hpp"
#include "pathfind.hpp"
#include "graphics.hpp"
#include "system.hpp"

#include <list>
#include <map>
#include <cassert>
#include <unordered_set>
#include <unordered_map>

using std::list;

struct Aspect {
  enum Kind {
    Skilled,
    Renderable,
    AI,
    NeedsAI,
    Clearance,
    Descriptable,
    Position,
    CitizenName,
    Movable,
    Inventory,
    Room,
    Furniture,
    Foodstuff,
    Pharmaceutical,
    Item,
    JobProvider
  };

  Aspect(Kind k) : kind(k), parent(nullptr) { }
  virtual ~Aspect() {}

  template<class T>
  T& as() {
    assert(kind == T::StaticKind);
    return static_cast<T&>(*this);
  }
  template<class T>
  const T& as() const {
    assert(kind == T::StaticKind);
    return static_cast<const T&>(*this);
  }

  const Kind kind;
  struct Ent* parent;
};
template<Aspect::Kind K, class T>
struct AspectStatic : Aspect {
  static const Aspect::Kind StaticKind = K;
  AspectStatic() : Aspect(K) {
    instances.insert(&this->as<T>());
  }
  ~AspectStatic() { instances.erase(&this->as<T>()); }

  typedef std::unordered_set<T*> set_t;
  typedef typename set_t::iterator iterator;
  static set_t instances;
};
template<Aspect::Kind K, class T>
typename AspectStatic<K,T>::set_t AspectStatic<K,T>::instances;

struct Ent {
  typedef std::unordered_map<int, Aspect*> map_t;
  typedef std::unordered_set<System*> set_t;

  ~Ent() {
    for (auto p : compmap)
      delete p.second;
    for (auto sys : sysset)
      sys->erase(this);
  }

  template<class T>
  inline T* assert_get() {
    assert(has<T>());
    return get<T>();
  }

  template<class T>
  inline T* get() { return static_cast<T*>(compmap[T::StaticKind]); }
  template<class T>
  inline bool has() { return compmap.find(T::StaticKind) != compmap.end(); }

  template<class...Ts>
  inline tuple<Ts*...> get_tuple() {
    return make_tuple(get<Ts>()...);
  }

  inline bool has(Aspect::Kind k) { return compmap.find(k) != compmap.end(); }
  inline void add(Aspect* comp) {
    comp->parent = this;
    compmap[comp->kind] = comp;
  }
  inline void add(System* sys) {
    sysset.insert(sys);
    sys->insert(this);
  }

  map_t compmap;
  set_t sysset;
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
