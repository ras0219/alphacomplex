#include "entities/system.hpp"
#include "entities/entity.hpp"

#include <algorithm>
#include <list>
#include <map>
#include <tuple>
#include <cassert>

Ent::~Ent() {
  for (auto p : compmap)
    p.second->on_remove();
  for (auto p : compmap)
    delete p.second;
  for (auto sys : sysset)
    sys->erase(this);
}

template <class T>
T* Ent::assert_get() {
  assert(has<T>());
  return get<T>();
}

template <class T>
T* Ent::get() {
  return static_cast<T*>(compmap[T::StaticKind]);
}

template<class T>
T* Ent::has_get() {
  auto it = compmap.find(T::StaticKind);
  if (it == compmap.end())
    return nullptr;
  return static_cast<T*>(it->second);
}

template<class T>
bool Ent::has()
{
  return compmap.find(T::StaticKind) != compmap.end();
}

template<class...Ts>
std::tuple<Ts*...> Ent::get_tuple() {
  return std::make_tuple(std::get<Ts>()...);
}

bool Ent::has(Component::Kind k)
{
  return compmap.find(k) != compmap.end();
}

void Ent::add(Component* comp) {
  comp->parent = this;
  compmap[comp->kind] = comp;
  comp->on_add();
}
void Ent::add(System* sys) {
  sysset.insert(sys);
  sys->insert(this);
}

template<class C, class...Args>
void Ent::emplace(Args&&...args) {
  add(new C(std::forward<Args>(args)...));
}
