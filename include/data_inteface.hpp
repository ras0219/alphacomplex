#pragma once

#include "city.hpp"
#include "job\joblist.hpp"
#include <string>
#include <queue>

enum Game_Process {
	NOT_USED,   //empty
	STORAGE,    //currently waiting in-flight
	LOGIC,
	GRAPHICS,
	/*Could add more if desired. physics is a popular one.*/
} state;

//will change the return later to include different conditions
//Fill in sent_from and send_to at this moment.
//data_to_send will take the pointer as it is -- you have to make a copy of it first.
//This will free that data after it is used. 
//Do not use the pointer after giving it here.
template <class data_type>
int send_accross(Game_Process sent_from, Game_Process send_to, data_type* data_to_send);

//NULL if nothing available yet. 
//must free data after using it.
template <class data_type>
data_type* recieve_across(Game_Process my_proces, Game_Process recv_from);


///neccessary data from game_logic
struct LogicDataInterface{
	City* CityPtr;
	job::JobList* JobListPtr;
	queue<string> announce_messages;
	float fps_current;
	float influence;
	~LogicDataInterface()
	{
		delete CityPtr;
		delete JobListPtr;
	}
};

LogicDataInterface* CurrentGfxInterface = nullptr;

//Friendly link for actual game.
void logic_ended_loop();
void gfx_start_loop();