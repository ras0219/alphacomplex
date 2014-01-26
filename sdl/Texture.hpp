#pragma once

#include "cow_resource.hpp"

struct SDL_Texture;

extern "C" void SDL_DestroyTexture(SDL_Texture*);

namespace SDL {

  // C++ wrapper around SDL_Texture* using RAII ref counting.
  struct Texture {
    Texture() {}
    explicit Texture(SDL_Texture* ptr) : res(ptr) {}

    Texture(Texture&& t) : res(std::move(t.res)) {}
    Texture(const Texture&) = delete;

    Texture& operator=(Texture&& o) { res = std::move(o.res); return *this; }

    // Accessors
    SDL_Texture* get() { return res.get(); }
    SDL_Texture* release() { return res.release(); }

    // Object-based methods

  private:

    struct DestroyTexture {
      inline void operator()(SDL_Texture* s) { SDL_DestroyTexture(s); }
    };

    std::unique_ptr<SDL_Texture, DestroyTexture> res;
  };

}
