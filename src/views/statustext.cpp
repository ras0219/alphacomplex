#include <vector>
#include "views/statustext.hpp"
#include "graphics.hpp"

using namespace std;

extern bool paused;
StatusText statustext;

StatusMsg::StatusMsg(unsigned int p, std::function<int()> c, const std::string& t)
  : priority(p), callback(c), text(t) { }

bool StatusMsg::operator<(const StatusMsg& rhs) const {
  return priority < rhs.priority;
}

StatusText::StatusText() {
  // Add in default statuses
  add({ 10, []() { return paused ? KEEP | EMIT : KEEP | PASS; }, "**PAUSED**" });
}

void StatusText::render(Graphics& g) {
  vector<StatusMsg> keep_queue;

  while (!status_queue.empty()) {
    StatusMsg s = status_queue.top();
    status_queue.pop();
    int retval = s.callback();

    if (!(retval & REMOVE)) {
      keep_queue.push_back(s);
    }

    if (retval & EMIT) {
      g.drawString(g.getWidth() - ((s.text.size() + 1) * 6) - 5, 15, s.text, Graphics::DEFAULT);
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
