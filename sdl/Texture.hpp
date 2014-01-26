#pragma once

#include "cow_resource.hpp"

struct SDL_Texture;

extern "C" void SDL_DestroyTexture(SDL_Texture*);

namespace SDL {

  // C++ wrapper around SDL_Texture* using RAII ref counting.
  struct Texture {
    Texture() {}
    Texture(SDL_Texture* ptr) : res(ptr) {}

    // Accessors
    SDL_Texture* get() { return res; }
    SDL_Texture const* get() const { return res; }

    // Object-based methods

  private:
    Resource<SDL_Texture, SDL_DestroyTexture> res;
  };

}
