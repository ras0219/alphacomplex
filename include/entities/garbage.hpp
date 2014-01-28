#pragma once

#include "components/ai/job.hpp"
#include "point.hpp"

struct Ent* make_garbage(Point p);
std::shared_ptr<Job> make_garbage_job(Ent* g);
