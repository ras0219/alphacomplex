#include "job/joblist.hpp"
#include "job/job.hpp"
#include "graphics.hpp"
#include "entities/garbage.hpp"

#include <iostream>

namespace job {

    JobList::iterator JobList::add_job(Job* j) { return add_job(shared_ptr<Job>(j)); }

    JobList::iterator JobList::add_job(const shared_ptr<Job>& j) {
        jlist.push_back(j);
        return --jlist.end();
    }

    JobList::iterator JobList::begin() { return jlist.begin(); }
    JobList::iterator JobList::end() { return jlist.end(); }

    void JobList::remove_jobs() {
        auto it = jlist.begin();
        while (it != jlist.end()) {
            if ((*it)->state == Job::COMPLETED) {
                jlist.erase(it++);
                continue;
            }
            ++it;
        }
    }

    Job* JobList::find_job(clearance c) {
        auto it = jlist.begin();
        while (it != jlist.end()) {
            auto j = *it;
            if (j->completed()) {
                jlist.erase(it++);
                continue;
            }
            if (j->available() && j->clearance() & c)
                return j.get();
            ++it;
        }
        return nullptr;
    }

    void JobListing::render(Graphics& g, render_box const& pos) {
        g.drawString(pos.x, pos.y, title, Graphics::DEFAULT);
        g.drawString(pos.x, pos.y + 1, "---------", Graphics::DEFAULT);

        int yoffset = 3;
        for (auto j : *jlist) {
            if (j->completed())
                continue;

            // Mark in-progress jobs
            if (j->unavailable())
                g.drawChar(pos.x, pos.y + yoffset, '~');

            auto desc = j->description();
            g.drawString(pos.x + 1, pos.y + yoffset, desc);
            yoffset++;
        }
    }

    JobList jobs;

    JobList& JobList::getJoblist() { return jobs; }

}
