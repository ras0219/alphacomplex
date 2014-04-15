#pragma once

#include <memory>

struct Point;
namespace job { struct Job; }
namespace item { struct Item; }

/// Creates a job to store an item.
///@param dest The place to store the item.
///@param i The item to store.
///@warning Does not add the job to the joblist.
std::shared_ptr<job::Job> make_storage_job(const Point& dest, item::Item* i);
