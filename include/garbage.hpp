#pragma once

#include "room.hpp"
#include "job.hpp"
#include "entity.hpp"

// struct CleaningRoom : Room {
//   CleaningRoom(int x_, int y_, int w_, int h_)
//     : Room(x_,y_,w_,h_) { }

//   virtual const char* rawname() { return RAWNAME; }

//   static const char* RAWNAME;
// };

Ent* make_garbage(Position p);
Job* make_garbage_job(Ent* g);
