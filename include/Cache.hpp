#pragma once

template<class Key, class Value>
struct Cache
{
  using rev_func_t = void (*)(Value*);
  
  Cache(unsigned int Max_Size, rev_func_t revocationFunction) : CAPACITY(Max_Size), RevocateObject(revocationFunction)
  {
    //hi
  }
  virtual ~Cache() = default; //must implement to evict all cache on destructor.
  Cache& operator=(const Cache&) = delete;
  //Interface to Cache
  virtual bool exists(Key input) = 0;
  virtual void put(Key input, Value cahed_value) = 0;
  virtual bool get(Key input, Value* cached_value) = 0;

  //Testing how optimized we are
  virtual float get_hit_rate() = 0;

protected:
  
  //neccesary for revocation. Will be called internally. Free/Destroy/DestroyObject/etc...
  rev_func_t RevocateObject;
	
  //Constants for use
  const unsigned int CAPACITY; //number of inputs stored.
};
