#include "triple_buffering_strategy.hpp"
#include <mutex>

//we could write a lock-free queue, but it'll be slower and less portable.
//although do we care about portability?
std::mutex tripl_buf_mutex;  // protects ptr_new/ptr_old.

template <typename data_type>
struct ptr_new {
	static data_type* value;
};

template <typename data_type>
struct ptr_old {
	static data_type * value;
};

template<class T>
T* ptr_new<T>::value = nullptr;
template<class T>
T* ptr_old<T>::value = nullptr;

//everything comes into new
//second frame gets pushed to old
//third frame forces ptr_old to get freed. then ptr_new becomes ptr_old and new frame gets ptr_new
template <class data_type>
void triple_buffer_send(data_type* bfr)
{
	std::lock_guard<std::mutex> lock(tripl_buf_mutex);
	if (ptr_new == nullptr) //least likely case
	{
		ptr_new = bfr;
		return;
	}
	if (ptr_old == nullptr) //most likely case. ptr_new is taken, but ptr_old is not yet.
	{
		ptr_old= bfr;
		return;
	}
	//else replace both
	free(ptr_old);
	ptr_old = ptr_new;
	ptr_new = bfr;
	return;
}

//"pop" ptr_old if available. otherwise try ptr_new. otherwise nullptr. 
template <class data_type>
data_type* triple_buffer_recieve()
{
	std::lock_guard<std::mutex> lock(tripl_buf_mutex);
	data_type* return_ptr=nullptr;
	if (ptr_old != nullptr)
	{
		return_ptr = ptr_old;
		ptr_old = nullptr;
		return return_ptr;
	}
	if (ptr_new != nullptr)
	{
		return_ptr = ptr_new;
		ptr_new = nullptr;
		return return_ptr;
	}
	//else
	//no ptrs ready
	return nullptr;
}