#pragma once

template <bool b>
struct DebugPolicy {
public:
  enum {
    debugging = b,
  };
};
