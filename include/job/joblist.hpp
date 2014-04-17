#pragma once

#include "defs.hpp"
#include "views/widget.hpp"
#include "components/clearance.hpp"

#include <list>
#include <string>
#include <memory>

namespace job {

    struct Job;

    struct JobList {
        using list_t = std::list<std::shared_ptr<Job>>;
        using iterator = list_t::iterator;

        static JobList& getJoblist();

        iterator add_job(Job* j);
        iterator add_job(const std::shared_ptr<Job>& j);
        void remove_jobs();
        Job* find_job(clearance c);

        iterator begin();
        iterator end();

    private:
        list_t jlist;
    };

    struct JobListing : StaticWidget<JobListing> {
        JobList* jlist;
        std::string title;

        JobListing(JobList* l, const char* t) : jlist(l), title(t) { }

        void render(Graphics& g, render_box const& pos);
    };

}
