#pragma once

#include <stdexcept>

#include "SDL.h"
#include "cow_resource.hpp"
#include "Renderer.hpp"

namespace SDL {

  // C++ wrapper around SDL_Window* using RAII ref counting.
  struct Window {
    Window() {}
    explicit Window(SDL_Window* w) : res(w) {}
    Window(Window&& w) : res(std::move(w.res)) {}

    Window& operator=(Window&& o) { res = std::move(o.res); return *this; }

    // Accessors
    SDL_Window* get() { return res.get(); }
    SDL_Window* release() { return res.release(); }

    // Object-based interface
    inline Renderer CreateRenderer(int index, uint32_t flags) {
      SDL_Renderer* rend = SDL_CreateRenderer(res.get(), index, flags);
      if (!rend)
        throw std::runtime_error(std::string("Could not create window: ") + SDL_GetError());

      SDL_RenderClear(rend);

      return Renderer(rend);
    }

  private:
    struct DestroyWindow {
      inline void operator()(SDL_Window* w) { SDL_DestroyWindow(w); }
    };
    std::unique_ptr<SDL_Window, DestroyWindow> res;
  };

  inline Window CreateWindow(const char* title, int x, int y, int w, int h, uint32_t flags) {
    SDL_Window* win = SDL_CreateWindow(title, x, y, w, h, flags);
    if (!win)
      throw std::runtime_error(std::string("Could not create window: ") + SDL_GetError());
    return Window(win);
  }

}
