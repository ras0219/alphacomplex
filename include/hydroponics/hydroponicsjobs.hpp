#pragma once

#include <memory>
namespace job { struct Job; }

struct Furniture;

namespace hydroponics {

    std::shared_ptr<job::Job> make_harvest_job(Furniture* table);
    std::shared_ptr<job::Job> make_plant_job(Furniture* table);

}