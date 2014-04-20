#include <vector>
#include "views/statustext.hpp"
#include "graphics.hpp"

using namespace std;

extern bool paused;

StatusText StatusText::instance;

StatusMsg::StatusMsg(unsigned int p, std::function<int()> c, const std::string& t)
  : priority(p), callback(c), text(t) { }

bool StatusMsg::operator<(const StatusMsg& rhs) const {
  return priority < rhs.priority;
}

StatusText::StatusText() {
  // Add in default statuses
  add({ 10, []() { return paused ? KEEP | EMIT : KEEP | PASS; }, "**PAUSED**" });
}

void StatusText::render(Graphics& g, render_box const& pos) {
  vector<StatusMsg> keep_queue;

  while (!status_queue.empty()) {
    StatusMsg s = status_queue.top();
    status_queue.pop();
    int retval = s.callback();

    if (!(retval & REMOVE)) {
      keep_queue.push_back(s);
    }

    if (retval & EMIT) {
		g.drawString(pos.x + pos.w - ((int)s.text.size() + 1) - 1, pos.y, s.text, true, Graphics::colors_to_context[Graphics::RED]);
      break;
    }
  }

  for (auto i : keep_queue) {
    status_queue.push(i);
  }
}

void StatusText::add(const StatusMsg& status) {
  status_queue.push(status);
}
