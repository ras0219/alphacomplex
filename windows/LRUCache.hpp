#pragma once

#include "Cache.hpp"
#include <list>
#include <map>

using namespace std;

template<class Key, class Value>
struct LRUCache : Cache<Key, Value>
{
  LRUCache(unsigned int Max_Size, rev_func_t revocationFunction);
  ~LRUCache();

  void put(Key input, Value cched_value);
  bool exists(Key input);
  bool get(Key input, Value* cached_value);

  float get_hit_rate();

private:
  using track_list = list<Value>;
  
  track_list key_tracker_type;
  unordered_map<key, typename track_list::iterator> key_to_iterator;
};
