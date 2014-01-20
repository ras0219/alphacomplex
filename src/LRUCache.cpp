#include "LRUCache.hpp"

template<class Key, class Value>
LRUCache<Key, Value>::LRUCache(unsigned int Max_Size, rev_func_t revocationFunction)
  : Cache<Key, Value>(Max_Size, revocationFunction)
{
}

template<class Key, class Value>
LRUCache<Key, Value>::~LRUCache()
{
  if (RevocateObject == nullptr) //nothing to clean, our revocate object is null
    return;

}

template<class Key, class Value>
void LRUCache<Key, Value>::put(Key input, Value cched_value)
{
}

template<class Key, class Value>
bool LRUCache<Key, Value>::exists(Key input)
{
  return false;
}


template<class Key, class Value>
bool LRUCache<Key, Value>::get(Key input, Value* cached_value)
{
  return false;
}

template<class Key, class Value>
float LRUCache<Key, Value>::get_hit_rate()
{
  return 0.0f;
}
