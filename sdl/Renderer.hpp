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
    explicit Renderer(SDL_Renderer* ptr) : res(ptr) {}
    Renderer(Renderer&& r) : res(std::move(r.res)) {}

    Renderer& operator=(Renderer&& o) { res = std::move(o.res); return *this; }

    // Accessors
    SDL_Renderer* get() { return res.get(); }
    SDL_Renderer* release() { return res.release(); }

    // Object-based methods
    inline Texture CreateTextureFromSurface(SDL_Surface* s) {
      SDL_Texture* tex = SDL_CreateTextureFromSurface(get(), s);
      if (!tex)
        throw std::runtime_error(std::string("Failed to allocate texture: ") + SDL_GetError());
      return Texture(tex);
    }

    inline void RenderCopy(SDL_Texture* tex, SDL_Rect const* src, SDL_Rect const* dst) {
      if (SDL_RenderCopy(get(), tex, src, dst))
        throw std::runtime_error(std::string("Could not copy string to screen: ") + SDL_GetError());
    }

    inline void RenderClear() {
      SDL_RenderClear(get());
    }
    inline void RenderPresent() {
      SDL_RenderPresent(get());
    }

  private:
    struct DestroyRenderer {
      inline void operator()(SDL_Renderer* s) { SDL_DestroyRenderer(s); }
    };
    std::unique_ptr<SDL_Renderer, DestroyRenderer> res;
  };

}