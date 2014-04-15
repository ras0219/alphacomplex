#pragma once

#include <memory>

struct Point;
namespace job { struct Job; }
namespace ecs { struct Ent; }
namespace item { struct ItemProperties; }

/// Spawns a piece of garbage at point p and registers a job to clean it up.
void spawn_garbage(const Point& p);

/// Creates a piece of garbage.
///@warning Does not add the garbage to the city.
ecs::Ent* make_garbage(const Point& p);

/// Creates a job to clean up a piece of garbage.
///@param g A piece of "garbage".
///@warning Does not add the job to the joblist.
std::shared_ptr<job::Job> make_garbage_job(ecs::Ent* g);

/// The set of properties for garbage.
extern item::ItemProperties garbage_properties;
