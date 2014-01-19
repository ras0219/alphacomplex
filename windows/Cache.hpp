#pragma once


template<typename key, class value>
virtual struct Cache
{
	virtual Cache(unsigned int Max_Size, void (*revocationFunction)(value*))
	{
		CAPACITY = Max_Size;
		RevocateObject = revocationFunction;
	}
	virtual ~Cache() = 0; //must implement to evict all cache on destructor.

	//neccesary for revocation. Will be called internally. Free/Destroy/DestroyObject/etc...
	private void RevocateObject(value* object);
	
	//Interface to Cache
	virtual bool exists(key input) = 0;
	virtual void put(key input, value cahed_value) = 0;
	virtual bool get(key input, value* cached_value) = 0;

	//Testing how optimized we are
	virtual float get_hit_rate() = 0;
	
	//Constants for use
	private const unsigned int CAPACITY; //number of inputs stored.
};