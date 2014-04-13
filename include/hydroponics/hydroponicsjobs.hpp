#pragma once

#include <memory>

std::shared_ptr<struct Job> make_harvest_job(struct Furniture* table);

std::shared_ptr<struct Job> make_plant_job(struct Furniture* table);
