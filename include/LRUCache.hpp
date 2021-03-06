#pragma once

#include <list>
#include <unordered_map>

#include "Cache.hpp"

using namespace std;

template<class Key, class Value>
struct LRUCache : Cache<Key, Value>
{
  using typename Cache<Key, Value>::rev_func_t;
  
  //this saves a lot of space
  using track_order = list<Key>;
  using track_key = unordered_map<Key, pair<Value, typename track_order::iterator> >;

  LRUCache(unsigned int Max_Size, rev_func_t revocationFunction);
  ~LRUCache();
  LRUCache& operator=(const LRUCache&) = delete;

  void put(Key input, Value cched_value);
  //note that this does not advance the counter
  bool exists(Key input);
  bool get(Key input, Value* cached_value);

  float get_hit_rate();

private:
  track_order key_order_tracker;
  track_key key_map;
  unsigned long long hit_success;
  unsigned long long miss_fail;
  unsigned long long evictions;
};



template<class Key, class Value>
LRUCache<Key, Value>::LRUCache(unsigned int Max_Size, rev_func_t revocationFunction)
  : Cache<Key, Value>(Max_Size, revocationFunction)
{
  hit_success = miss_fail = evictions = 0;
}

template<class Key, class Value>
LRUCache<Key, Value>::~LRUCache()
{
  // Grr, C++, why you do this...
  // http://www.parashift.com/c++-faq-lite/nondependent-name-lookup-members.html
  if (this->RevocateObject == nullptr) //nothing to clean, our revocate object is null
    return;
  //make sure all the items are killed
  for (auto& p : key_map)
    this->RevocateObject(&(p.second.first));
}

template<class Key, class Value>
void LRUCache<Key, Value>::put(Key input, Value cched_value)
{
  if (exists(input))
    return; //do nothing, item already exists
  if (key_map.size() >= this->CAPACITY)
  {
    //we need to EVICT!!!!
    evictions++;
    Key evicted_object = key_order_tracker.back();

    auto it = key_map.find(evicted_object);
    Value evicted_value = it->second.first;
    key_order_tracker.pop_back();
    key_map.erase(it);

    this->RevocateObject(&evicted_value);
  }

  //now actually insert it. Push it to front as MRU
  key_order_tracker.push_front(input);
  key_map[input] = make_pair(cched_value, key_order_tracker.begin());
}

template<class Key, class Value>
bool LRUCache<Key, Value>::exists(Key input)
{
  if (key_map.count(input) == 0)
    return false;
  else //==1
    return true;
}


template<class Key, class Value>
bool LRUCache<Key, Value>::get(Key input, Value* cached_value)
{
  auto retr_elem = key_map.find(input);
  if (retr_elem == key_map.end())
    {//did not find
      miss_fail++;
      *cached_value = nullptr;
      return false;
    }
  else
    {
      hit_success++;
      *cached_value = retr_elem->second.first;
      //we now need to push the hit object to top of the list
      key_order_tracker.splice(key_order_tracker.begin(), key_order_tracker, retr_elem->second.second);
    }
  return true;
}

template<class Key, class Value>
float LRUCache<Key, Value>::get_hit_rate()
{
  if ((miss_fail + hit_success) == 0)
    return 1.0f; //succesfully served 100% of requested value
  return ((float)hit_success / miss_fail);
}
