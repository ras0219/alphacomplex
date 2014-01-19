#include "Cache.hpp"
#include <list>
#include <map>


using namespace std;

template<typename key, class value>
struct LRUCache : public Cache
{
	LRUCache(unsigned int Max_Size, void(*revocationFunction)(value*));
	~LRUCache();
	void RevocateObject(key cached_value);

	void put(key input, value cched_value);
	bool exists(key input);
	bool get(key input, value* cached_value);

	float get_hit_rate();

	list<value> key_tracker_type;
	unordered_map<key, list::iterator > key_to_iterator;
};