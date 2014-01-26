#pragma once

#include "cow_resource.hpp"

struct SDL_Surface;

extern "C" void SDL_FreeSurface(SDL_Surface*);

namespace SDL {

  // C++ wrapper around SDL_Surface* using RAII ref counting.
  struct Surface {
    Surface() {}
    Surface(SDL_Surface* ptr) : res(ptr) {}

    // Accessors
    SDL_Surface* get() { return res; }
    SDL_Surface const* get() const { return res; }

    // Object-based methods

  private:
    Resource<SDL_Surface, SDL_FreeSurface> res;
  };

}
