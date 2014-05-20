#include "triple_buffering_strategy.hpp"
#include <mutex>

//we could write a lock-free queue, but it'll be slower and less portable.
//although do we care about portability?
std::mutex tripl_buf_mutex;  // protects ptr_new/ptr_old.

template <class data_type>
data_type* ptr_new=nullptr, ptr_old=nullptr;

//everything comes into new
//second frame gets pushed to old
//third frame forces ptr_old to get freed. then ptr_new becomes ptr_old and new frame gets ptr_new
template <class data_type>
void triple_buffer_send(data_type* bfr)
{
	tripl_buf_mutex.lock();
	if (ptr_new == nullptr) //least likely case
	{
		ptr_new = bfr;
		tripl_buf_mutex.unlock();
		return;
	}
	if (ptr_old == nullptr) //most likely case. ptr_new is taken, but ptr_old is not yet.
	{
		ptr_old= bfr;
		tripl_buf_mutex.unlock();
		return;
	}
	//else replace both
	free(ptr_old);
	ptr_old = ptr_new;
	ptr_new = bfr;
	tripl_buf_mutex.unlock();
	return;
}

//"pop" ptr_old if available. otherwise try ptr_new. otherwise nullptr. 
template <class data_type>
data_type* triple_buffer_recieve()
{
	tripl_buf_mutex.lock();
	data_type* return_ptr=nullptr;
	if (ptr_old != nullptr)
	{
		return_ptr = ptr_old;
		ptr_old = nullptr;
		tripl_buf_mutex.unlock();
		return return_ptr;
	}
	if (ptr_new != nullptr)
	{
		return_ptr = ptr_new;
		ptr_new = nullptr;
		tripl_buf_mutex.unlock();
		return return_ptr;
	}
	//else
	//no ptrs ready
	tripl_buf_mutex.unlock();
	return nullptr;
}