#pragma once
#include <queue>
#include <functional>
#include <string>

#include "widget.hpp"

struct StatusMsg {
protected:
  unsigned int priority;
  std::function<int()> callback;
  std::string text;

  friend class StatusText;

public:
  StatusMsg() = delete;
  StatusMsg(unsigned int p, std::function<int()> c, const std::string& t);

  virtual bool operator<(const StatusMsg& rhs) const;
};

class StatusText : StaticWidget<StatusText> {
protected:
  std::priority_queue<StatusMsg> status_queue;
public:
  // Convenience enum: This is meant to be returned as a bitfield, e.g. return StatusText::EMIT | StatusText::KEEP
  // PASS and KEEP are default (e.g. do not output the status and do not remove the status from the list of possible statuses)
  enum Result {
    PASS = 0,  // do not output status text
    EMIT = 1,  // output status text
    KEEP = 0,  // keep status in priority queue for later output
    REMOVE = 2 // remove status from priority queue so it is never considered again for output
  };

  StatusText();

  void render(Graphics& g, render_box const& pos);
  void add(const StatusMsg& status);

  static StatusText instance;
};
