#include "data_inteface.hpp"
#include <ctime>
#include <atomic>

#define NUM_OF_INTERFACE_BUFFERS 3

/* Note: currently we have 1 reader 1 writer. Keep that in mind for atomic things*/

template <class data_type>
volatile struct InternalIntefaceBuffer
{
	data_type* data_ptr;
	Game_Process sent_from;
	Game_Process sent_to;
	std::atomic<Game_Process> current_ownership;
	time_t time_sent;
};

//force these guys to be zero initialized
template <class data_type>
InternalIntefaceBuffer available_buffers[NUM_OF_INTERFACE_BUFFERS] = { 0,0,0 /*TODO:NUM_OF_INTERFACE_BUFFERS*/ };

//linked list to establish earliest-latest messages;


template <class data_type>
InternalIntefaceBuffer* get_first_ownership(Game_Process ownership_needed)
{
	for (int i = 0; i < NUM_OF_INTERFACE_BUFFERS; i++)
	{
		std::atomic<Game_Process> found_ownership;
		found_ownership.store(available_buffers[i]);
		if (found_ownership.load() == ownership_needed) return &avialble_buffers[i];
	}
	return NULL;
}

template <class data_type>
int send_accross(Game_Process sent_from, Game_Process send_to, data_type* data_to_send)
{
	InternalIntefaceBuffer* buffer_to_use = get_first_ownership(Game_Process.NOT_USED); //check if we have an empty storage

	if (buffer_to_use == NULL) 
	{
		buffer_to_use = get_first_ownership(Game_Process.STORAGE);
		if (buffer_to_use == NULL) return -1; // bad, bad, bad. we made an error coding somewhere.
		//else 
	}

	buffer_to_use.sent_from = sent_from;
	buffer_to_use.sent_to = sent_to;
	buffer_to_use.data_ptr = data_to_send;
	buffer_to_use.current_ownership.store(Game_Process.STORAGE);
	return 0;
}

//NULL if nothing available yet. 
//stale data if nothing available.
template <class data_type>
data_type* recieve_across(Game_Process my_proces, Game_Process recv_from)
{
	return NULL;
}