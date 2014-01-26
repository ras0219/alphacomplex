#pragma once

#include <stdexcept>

#include "cow_resource.hpp"
#include "Surface.hpp"
#include "Texture.hpp"
#include "SDL.h"

namespace SDL {

  // C++ wrapper around SDL_Window* using RAII ref counting.
  struct Renderer {
    Renderer() {}
    Renderer(SDL_Renderer* ptr) : res(ptr) {}

    // Accessors
    SDL_Renderer* get() { return res; }
    SDL_Renderer const* get() const { return res; }

    // Object-based methods
    inline Texture CreateTextureFromSurface(Surface& s) {
      SDL_Texture* tex = SDL_CreateTextureFromSurface(get(), s.get());
      if (!tex)
        throw std::runtime_error(std::string("Failed to allocate texture: ") + SDL_GetError());
      return Texture(tex);
    }

    inline void RenderCopy(Texture& tex, SDL_Rect const* src, SDL_Rect const* dst) {
      if (SDL_RenderCopy(get(), tex.get(), src, dst))
        throw std::runtime_error(std::string("Could not copy string to screen: ") + SDL_GetError());
    }

    inline void RenderClear() {
      SDL_RenderClear(get());
    }
    inline void RenderPresent() {
      SDL_RenderPresent(get());
    }

  private:
    Resource<SDL_Renderer, SDL_DestroyRenderer> res;
  };

}