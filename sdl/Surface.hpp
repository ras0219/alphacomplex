#pragma once

#include "cow_resource.hpp"

struct SDL_Surface;

extern "C" void SDL_FreeSurface(SDL_Surface*);

namespace SDL {

  // C++ wrapper around SDL_Surface* using RAII ref counting.
  struct Surface {
    Surface() {}
    explicit Surface(SDL_Surface* ptr) : res(ptr) {}
    Surface(Surface&& s) : res(std::move(s.res)) {}

    Surface& operator=(Surface&& o) { res = std::move(o.res); return *this; }

    // Accessors
    SDL_Surface* get() { return res.get(); }
    SDL_Surface* release() { return res.release(); }

    // Object-based methods

  private:
    struct FreeSurface {
      inline void operator()(SDL_Surface* s) { SDL_FreeSurface(s); }
    };

    std::unique_ptr<SDL_Surface, FreeSurface> res;
  };

}
