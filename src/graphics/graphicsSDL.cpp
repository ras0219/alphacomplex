#include "logger.hpp"
#include "graphics.hpp"
#include "views/widget.hpp"
#include "controller.hpp"

#ifndef _WIN32
#include <unistd.h> //let mingw handle it if needed
#endif

#include <cmath>
#include <cassert>
#include <iostream>
#include <vector>
#include <chrono>
#include <deque>
#include <array>
#include <cstring>
#include <utility>
#include <memory>

#include "SDL.h"
#include "SDL_ttf.h" //XXX- REMOVE ASAP
#include "LRUCache.hpp"

//#define TEMP_FONT_PATH "../resources/font/UbuntuMono-R.ttf"
#define TEMP_FONT_PATH "../resources/font/Anonymous_Pro.ttf"
#define NUMBER_OF_TILES 255
#define STRING_CACHE_SIZE 128

using namespace std;
using namespace chrono;

using NativeKeyboardKey = unsigned long;

// C++ wrapper around SDL_Surface* using RAII ref counting.
struct Surface {
  Surface() {}
  Surface(SDL_Surface* surf) : ptr(surf, SDL_FreeSurface) {}

  operator SDL_Surface*() { return ptr.get(); }
  operator SDL_Surface const*() const { return ptr.get(); }

  SDL_Surface& operator*() { return *ptr; }
  SDL_Surface const& operator*() const { return *ptr; }

  operator void const*() const { return ptr.get(); }

  std::shared_ptr<SDL_Surface> ptr;
};

// C++ wrapper around SDL_Texture* using RAII ref counting.
struct Texture {
  Texture() {}
  Texture(SDL_Texture* tex) : ptr(tex, SDL_DestroyTexture) {}

  operator SDL_Texture*() { return ptr.get(); }
  operator SDL_Texture const*() const { return ptr.get(); }

  SDL_Texture& operator*() { return *ptr; }
  SDL_Texture const& operator*() const { return *ptr; }

  operator void const*() const { return ptr.get(); }

  std::shared_ptr<SDL_Texture> ptr;
};

struct Graphics_SDL : Graphics {
  Graphics_SDL();

public:
  // Graphics interface Methods
  void drawString(int x, int y, const std::string& str, Context gc = DEFAULT);
  void drawChar(int x, int y, char str, Context gc = DEFAULT);

  void handle_events(struct Controller*);

  void repaint();
  void destroy();

private:
  // Initialization methods
  void version_check();
  void init_window();
  void init_renderer();
  void init_font();
  void log_stats();

private:
  // Internal use methods
  inline void throw_sdl_error(const std::string& desc);
  inline void throw_ttf_error(const std::string& desc);

  KeyboardKey map_key(NativeKeyboardKey key);
  void LoadText(const std::string& font_file);

  // Data
  int s;

  SDL_Window *win;
  SDL_Renderer *ren;
  Texture main_texture;
  Texture ttf_texture;
  unsigned int tiles_per_row;
  TTF_Font* best_font;
  SDL_Color font_color;
  Logger graphics_log;
  LRUCache<std::string, Texture> string_texture_cache;
};

void FreeSDLTexture(Texture*) { }

void Graphics_SDL::throw_sdl_error(const std::string& desc) {
  auto err = SDL_GetError();
  graphics_log.Write("%s\n%s", desc.c_str(), err);

  throw std::runtime_error(err);
}

void Graphics_SDL::throw_ttf_error(const std::string& desc) {
  auto err = TTF_GetError();
  graphics_log.Write("%s\n%s", desc.c_str(), err);

  throw std::runtime_error(err);
}

Graphics_SDL::Graphics_SDL()
  : win(nullptr), ren(nullptr), font_color({ 0, 255, 0, 0 })
  , graphics_log("graphics.txt"), string_texture_cache(STRING_CACHE_SIZE, FreeSDLTexture)
{
  graphics_log.Write("Initializing graphics file for SDL");
  version_check();

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
    throw_sdl_error("Error initializing SDL.");

  if (TTF_Init())
    throw_ttf_error("Error initializing TTF.");

  init_window();
  init_renderer();
  log_stats();
  init_font();
}

void Graphics_SDL::destroy() {
  if (destroyed) return;
  if (!ren) SDL_DestroyRenderer(ren);
  if (!win) SDL_DestroyWindow(win);
  if (!best_font) TTF_CloseFont(best_font);
  SDL_Quit();
  destroyed = true;
}

void Graphics_SDL::version_check() {
  SDL_version compiled, linked;
  SDL_VERSION(&compiled);
  SDL_GetVersion(&linked);

  graphics_log.Write("SDL Version compiled against: %d.%d.%d", compiled.major, compiled.minor, compiled.patch);
  graphics_log.Write("SDL Version linked against: %d.%d.%d", linked.major, linked.minor, linked.patch);
}

void Graphics_SDL::init_window() {
  width = 80;
  height = 40;

  //to-do: center on screen
  win = SDL_CreateWindow("", 100, 100, width * FONT_WIDTH, height * FONT_HEIGHT,
                         SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  if (win == nullptr)
    throw_sdl_error("Failed to make window.");
}

void Graphics_SDL::init_renderer() {
  ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (ren == nullptr)
    throw_sdl_error("Failed to make renderer.");

  SDL_RenderClear(ren);
}

void Graphics_SDL::init_font() {
  graphics_log.Write("Loading font:");
  graphics_log.Write(TEMP_FONT_PATH);

  best_font = TTF_OpenFont(TEMP_FONT_PATH, FONT_HEIGHT);
  if (best_font == nullptr)
    throw_ttf_error("Failed to load font.");

  LoadText(TEMP_FONT_PATH);
}

void Graphics_SDL::log_stats() {
  //debug times.
  //following is not supported on SDL 2.0.0
  //  graphics_log->Write("%s. We have %i MB of RAM","Finished initializing graphics for SDL!",SDL_GetSystemRAM());
  
  
  // Check drivers
  int num_drivers = SDL_GetNumVideoDrivers();

  if (num_drivers < 0)
    throw_sdl_error("Unable to determine drivers.");

  graphics_log.Write("We have %i drivers available", num_drivers);

  for (int driX = 0; driX < num_drivers; ++driX) {
    graphics_log.Write("%i: %s", driX, SDL_GetVideoDriver(driX));
  }
  graphics_log.Write("Current driver is: %s", SDL_GetCurrentVideoDriver());

  // Check renderers  
  SDL_RendererInfo ren_info;
  memset(&ren_info, 0, sizeof(ren_info));

  int num_render = SDL_GetNumRenderDrivers();
  if (num_render < 0)
    throw_sdl_error("Unable to determine renderers.");

  graphics_log.Write("We have %i renderers available", num_render);

  for(int driR = 0; driR < num_render; ++driR) {
    if (SDL_GetRenderDriverInfo(driR, &ren_info)) {
      // Nonfatal error.
      graphics_log.Write("%i:%s\n%s", driR, "Failed to get any info.", SDL_GetError());
    } else {
      graphics_log.Write("%i: Name:%s\n Flags: %i\n Num of Texture Formats: %i\n Max_Wid: %i\n Max_H: %i",
                         driR, ren_info.name, ren_info.flags, ren_info.num_texture_formats, ren_info.max_texture_width,
                         ren_info.max_texture_height);
      for (unsigned int driF = 0; driF < ren_info.num_texture_formats; ++driF) {
        uint32_t inf = ren_info.texture_formats[driF];
        graphics_log.Write("\n\tFormat:%i\n\tPixelFlag:%i\n\tPixelType:%i\n\tPixelOrder:%i\n\tPixelLayout:%i\n\tBitsPerPixel:%i\n\tBytesPerPixel:%i\n\tPixelFormatIndexed:%s\n\tPixelFormatAlpha:%s\n\tFourCC:%s",
                           driF, SDL_PIXELFLAG(inf), SDL_PIXELTYPE(inf), SDL_PIXELORDER(inf), SDL_PIXELLAYOUT(inf),
                           SDL_BITSPERPIXEL(inf), SDL_BYTESPERPIXEL(inf), SDL_ISPIXELFORMAT_INDEXED(inf) ? "True" : "False",
                           SDL_ISPIXELFORMAT_ALPHA(inf) ? "True" : "False", SDL_ISPIXELFORMAT_FOURCC(inf) ? "True" : "False");
      }
    }
  }
  SDL_GetRendererInfo(ren, &ren_info);
  graphics_log.Write("Current renderer is: %s", ren_info.name);
}

void Graphics_SDL::LoadText(const std::string&) {
  //to-do: refactor this in a slightly different architecture. 
  const int number_of_tiles = NUMBER_OF_TILES;

  // gk: make as close to square
  // ras: I'm very skeptical about this calculation...
  const int rows = 1 + (const int)sqrt((float)number_of_tiles);
  tiles_per_row = rows;
  int tile_texture_width = rows * FONT_WIDTH;
  int tile_texture_height = rows * FONT_HEIGHT;

  SDL_Rect copy_dimension;
  copy_dimension.w = FONT_WIDTH;
  copy_dimension.h = FONT_HEIGHT;
  Surface ttf_surface = SDL_CreateRGBSurface(0, tile_texture_width, tile_texture_height, 32, 0, 0, 0, 0);

  if (!ttf_surface)
    throw_sdl_error("Could not create character surface.");

  //temporary before choosing a tile\font
  for (int count = 0; count < number_of_tiles; ++count) {
    Surface surf = TTF_RenderGlyph_Shaded(best_font, (uint16_t)count, font_color, { 0, 0, 0, 0 });
    if (surf) {
      //if we draw the right character
      copy_dimension.x = (count % rows) * FONT_WIDTH;
      copy_dimension.y = (count / rows) * FONT_HEIGHT;
      if (SDL_BlitSurface(surf, NULL, ttf_surface, &copy_dimension))
        throw_sdl_error("Could not blit character");
    } else {
      // TODO: What does this even mean????
    }
  }

  //send the big one over
  ttf_texture = SDL_CreateTextureFromSurface(ren, ttf_surface);
  if (SDL_SaveBMP(ttf_surface, "font_tilemap.bmp")) {
    graphics_log.Write("Could not save bitmap of font tilemap.\n%s", SDL_GetError());
  }
}

KeyboardKey Graphics_SDL::map_key(NativeKeyboardKey key) {
    switch (key) {
        case SDLK_ESCAPE:
            return KEY_Escape;
        case SDLK_SPACE:
            return KEY_space;
        case SDLK_LEFT:
            return KEY_Left;
        case SDLK_RIGHT:
            return KEY_Right;
        case SDLK_UP:
            return KEY_Up;
        case SDLK_DOWN:
            return KEY_Down;
        case SDLK_RETURN:
            return KEY_Return;
        case SDLK_TAB:
            return KEY_Tab;
        case SDLK_h:
            return KEY_h;
        case SDLK_u:
            return KEY_u;
        case SDLK_r:
            return KEY_r;
        case SDLK_a:
            return KEY_a;
        case SDLK_q:
            return KEY_q;
        case SDLK_e:
            return KEY_e;
        case SDLK_d:
            return KEY_d;
        case SDLK_f:
            return KEY_f;
        case SDLK_PAGEUP:
            return KEY_Pageup;
        case SDLK_PAGEDOWN:
            return KEY_Pagedown;
        default:
            return KEY_Undefined;
    }
}

void Graphics_SDL::handle_events(Controller* c) {

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch(event.type){
      case SDL_KEYDOWN:
        c->handle_keypress(map_key(event.key.keysym.sym));
        break;
    case SDL_WINDOWEVENT: //window moved, max,min, etc
      if(event.window.event == SDL_WINDOWEVENT_RESIZED)
      {
		  height = (0xffff & event.window.data2) / FONT_HEIGHT;
		  width = (0xffff & event.window.data1) / FONT_WIDTH;
        //int old_size = (0xffff &event.window.data1);
        //int new_size = (0xffff &event.window.data2);
        
      }
      break;
    case SDL_QUIT: //send q to controller
      destroy();
      return;
      break;
    default: break; // dog
    }
  }
  //all done with events! :)
}

void Graphics_SDL::drawString(int x, int y, const string & str, const Graphics_SDL::Context) {
  //To-Do: give a choice to caller if he needs KERNING or CACHING.

  Texture retr_texture;
  if (string_texture_cache.get(str, &retr_texture) == false) {
		Surface my_font_surface = TTF_RenderText_Shaded(best_font, str.c_str(), font_color, { 0, 0, 0, 0 });
		if (!my_font_surface)
          //we tried
          return;

		retr_texture = SDL_CreateTextureFromSurface(ren, my_font_surface);
		if (!retr_texture)
          //we tried
          return;

		string_texture_cache.put(str, retr_texture);
	}

	int w = 0, h = 0;
	TTF_SizeText(best_font, str.c_str(), &w, &h);
	SDL_Rect dstRect = { x * FONT_WIDTH, y * FONT_HEIGHT, w, h };
    /* for (auto ch : str)
     * // Look at this performance
     * drawChar(x++, y, ch, gc);
     * // It's so good.
     */
 
    if (SDL_RenderCopy(ren, retr_texture, NULL, &dstRect))
      throw_sdl_error("Could not copy string to screen.");
}

void Graphics_SDL::drawChar(int x, int y, char ch, const Graphics_SDL::Context) {
  SDL_Rect dstRect = { x * FONT_WIDTH, y * FONT_HEIGHT, FONT_WIDTH, FONT_HEIGHT };
  int x_cord = (ch % tiles_per_row) * FONT_WIDTH;
  int y_cord = (ch / tiles_per_row) * FONT_HEIGHT;
  SDL_Rect srcRect = { x_cord, y_cord, FONT_WIDTH, FONT_HEIGHT };

  if (SDL_RenderCopy(ren, ttf_texture, &srcRect, &dstRect))
    throw_sdl_error("Could not copy character to screen.");
}

void Graphics_SDL::repaint() {
  SDL_RenderClear(ren);
  // SDL_RenderCopy(ren, main_texture, NULL,NULL); TBD

  for (auto p : c)
    p->render(*this, { 0, 0, getWidth(), getHeight() });
  
  SDL_RenderPresent(ren);
}

Graphics* new_graphics() { return new Graphics_SDL(); }

//// Now for the plug functions
void Graphics::drawString(int x, int y, const std::string& str, Context gc) {
  return static_cast<Graphics_SDL*>(this)->drawString(x,y,str,gc);
}
void Graphics::drawChar(int x, int y, char str, Context gc) {
  return static_cast<Graphics_SDL*>(this)->drawChar(x,y,str,gc);
}
void Graphics::handle_events(Controller* c) {
  return static_cast<Graphics_SDL*>(this)->handle_events(c);
}
void Graphics::repaint() {
  return static_cast<Graphics_SDL*>(this)->repaint();
}
void Graphics::destroy() {
  return static_cast<Graphics_SDL*>(this)->destroy();
}
