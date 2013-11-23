#pragma once
template <bool b>
struct Debug {
  enum {
    value = b,
  };
};
