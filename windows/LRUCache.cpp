#include "LRUCache.hpp"

using namespace std;

template<typename key, class value>
LRUCache::LRUCache(unsigned int Max_Size)
{
	//sets up max size
	Cache::Cache(Max_Size);

}
template<typename key, class value>
LRUCache::~LRUCache()
{
	if (RevocateObject == nullptr) //nothing to clean, our revocate object is null
		return; 

}

template<typename key, class value>
void LRUCache::put(key input, value cched_value)
{

}

template<typename key, class value>
bool LRUCache::exists(key input)
{
	
}


template<typename key, class value>
bool LRUCache::get(key input, value* cached_value)
{

}

template<typename key, class value>
float LRUCache::get_hit_rate()
{

}