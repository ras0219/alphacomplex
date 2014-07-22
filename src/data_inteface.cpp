#include "data_inteface.hpp"
#include <ctime>
#include "city.hpp" 
#include "triple_buffering_strategy.hpp"

/* Note: currently we have 1 reader 1 writer. Keep that in mind for atomic things
	Some information on this page is not needed... but will help for debugging.
	to-do: allocate buffers on the stack instead of the heap.
*/

Game_Process my_process = Game_Process::NOT_USED;

template <class data_type>
struct InternalIntefaceBuffer
{
	volatile data_type* data_ptr;
	volatile Game_Process sent_from;
	volatile Game_Process sent_to;
	volatile Game_Process current_ownership;
	volatile time_t time_sent;
};

//this will be useful with multiple send-recv
template <class data_type>
InternalIntefaceBuffer<data_type>* get_first_ownership(Game_Process ownership_needed)
{
	for (int i = 0; i < NUM_OF_INTERFACE_BUFFERS; i++)
	{
		std::atomic<Game_Process> found_ownership;
		found_ownership.store(available_buffers[i]);
		if (found_ownership.load() == ownership_needed) return &avialble_buffers[i];
	}
	return nullptr;
}

//takes the pointer and stores it in store.
template <class data_type>
int send_accross(Game_Process sent_from, Game_Process send_to, data_type* data_to_send)
{
	InternalIntefaceBuffer* buffer_to_use = new InternalIntefaceBuffer(0);
	if (buffer_to_use == nullptr)
		throw new exception("We are out of memory! Failed to allocate internalinterfacebuffer!");

	memset(buffer_to_use, 0, sizeof(buffer_to_use));
		

	buffer_to_use.sent_from = sent_from;
	buffer_to_use.sent_to = sent_to;
	buffer_to_use.data_ptr = data_to_send;
	buffer_to_use.current_ownership = Game_Process.STORAGE;
	time_sent = std::clock();
	return 0;
}

//NULL if nothing available yet. 
//stale data if nothing available.
template <class data_type>
data_type* recieve_across(Game_Process my_proces, Game_Process recv_from)
{
	data_type* recv_ptr;
	recv_ptr = triple_buffer_recieve<data_type>();
	if (recv_ptr == nullptr) return nullptr;
	else
	{
		if (my_process != Game_Process::GRAPHICS) throw new exception("Not implemented yet to retrieve info from not Graphics");
		if (recv_from != Game_Process::LOGIC) throw new exception("Not implemented yet to retrive info not from logic yet.");
		recv_ptr->current_ownership = Game_Process::GRAPHICS;
		return recv_ptr;
	}
	
}



void logic_ended_loop(void)
{
	//LogicDataInterface* newInterface = new LogicDataInterface();

	//magick!
}

void gfx_start_loop()
{
	//LogicDataInterface* t_ptr = recieve_across<LogicDataInterface>(Game_Process::GRAPHICS, Game_Process::LOGIC);
	//if (t_ptr == nullptr) return;
	//else
	{
		//delete CurrentGfxInterface;
		//CurrentGfxInterface = t_ptr;
		return;
	}
}