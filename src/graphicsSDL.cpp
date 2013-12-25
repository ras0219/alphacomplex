#include "logger.hpp"
#include "graphics.hpp"
#include "component.hpp"
#include "controller.hpp"

#ifndef _WIN32
#include <unistd.h> //let mingw handle it if needed
#endif

#include <cassert>
#include <iostream>
#include <vector>
#include <chrono>
#include <deque>
#include <array>
#include <cstring>

#include "SDL.h"
#include "SDL_ttf.h" //XXX- REMOVE ASAP

//#define TEMP_FONT_PATH "../resources/font/UbuntuMono-R.ttf"
#define TEMP_FONT_PATH "../resources/font/Anonymous_Pro.ttf"
int FONT_HEIGHT = 12; //notice how this is NOT a define. hacky hacky.
int FONT_WIDTH = 6;

using namespace std;
using namespace chrono;


struct GraphicsImpl : Graphics {
  GraphicsImpl();

  // Methods
  void drawString(int x, int y, const std::string& str, Context gc = DEFAULT);
  void drawChar(int x, int y, char str, Context gc = DEFAULT);

  void handle_events(struct Controller*);

  void LoadText(const std::string msg, const std::string font_file);
  void repaint();
  void destroy();

  //<temp>
  std::array<SDL_Texture*, 256> cached_textures;
  //</temp>

  // Data
  int s;

  SDL_Window *win;
  SDL_Renderer *ren;
  SDL_Texture *main_texture;
  SDL_Texture *ttf_texture;
  TTF_Font* best_font;
  SDL_Color font_color;
  int sdl_last_call;
  Logger *graphics_log;
};

GraphicsImpl::GraphicsImpl()
  : win(nullptr), ren(nullptr), main_texture(nullptr),
    ttf_texture(nullptr), font_color({0,0,0,0}),
    sdl_last_call(0), graphics_log(nullptr)
{
  memset(&cached_textures[0], 0, sizeof(SDL_Texture*) * cached_textures.size());
  graphics_log = new Logger("graphics.txt");
  graphics_log->Write("%s","Initializing graphics file for SDL");
  SDL_version compiled,linked;
  SDL_VERSION(&compiled);
  SDL_GetVersion(&linked);
  graphics_log->Write("SDL Version compiled against:%d.%d.%d\nSDL Version linked against:%d.%d.%d",
    compiled.major, compiled.minor,compiled.patch,linked.major,linked.minor,linked.patch);
  int slc = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS); 

  if(slc == -1) {
    //to-do: use a better funciton for this
    std::cout << "Failed to start SDL. "<<SDL_GetError() << std::endl;
    exit(-1); //to-do: use a logger.
  }
  slc = TTF_Init(); //not use for long
  if(slc == -1) {
    std::cout << "Failed to start TTF. "<<TTF_GetError() << std::endl;
    exit(-1); //to-do: use a logger.
  }

  font_color = {0,255,0,0};
  width = 80*FONT_WIDTH; //to-do
  height= 40*FONT_HEIGHT;

  //to-do: center on screen
  win = SDL_CreateWindow("", 100, 100, width, height,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  if(win == nullptr) {
    std::cout << "Failed to make window. "<<SDL_GetError() << std::endl;
    exit(-1); //to-do: use a logger.
  }
  ren = SDL_CreateRenderer(win, -1,
                                  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if(ren == nullptr) {
    std::cout << "Failed to make renderer. "<<SDL_GetError() << std::endl;
    exit(-1); //to-do: use a logger.
  }

  //to-do: load the file
  best_font = TTF_OpenFont(TEMP_FONT_PATH, FONT_HEIGHT);
  if(best_font==NULL)
  {
    std::cout << "Failed to load ttf from  . " << TEMP_FONT_PATH << " : " <<TTF_GetError() << std::endl;
    exit(-1); //to-do: use a logger.
  }
  SDL_RenderClear(ren);
  //debug times.
  //following is not supported on SDL 2.0.0
//  graphics_log->Write("%s. We have %i MB of RAM","Finished initializing graphics for SDL!",SDL_GetSystemRAM());
  int num_drivers = SDL_GetNumVideoDrivers();
  if(num_drivers<0)
  {
   graphics_log->Write("%s\n%s","Error! Unable to figure out how many drivers we got",SDL_GetError());
   return;
  }
  graphics_log->Write("We have %i drivers available",num_drivers);
  for(int driX=0; driX<num_drivers; driX++)
  {
   graphics_log->Write("%i:%s",driX,SDL_GetVideoDriver(driX));
  }
  graphics_log->Write("%s:%s", "Current driver is",SDL_GetCurrentVideoDriver());
}

//maybe rename function?
void GraphicsImpl::LoadText(const std::string msg, const std::string font_file)
{
 (void) font_file;
  if(ttf_texture!=NULL)
   {
    SDL_DestroyTexture(ttf_texture);
    ttf_texture = NULL;
   }
  SDL_Surface *surf = TTF_RenderText_Solid(best_font, msg.c_str(), font_color);
  ttf_texture = SDL_CreateTextureFromSurface(ren, surf);
  SDL_FreeSurface(surf);
  return;
}
void GraphicsImpl::handle_events(Controller* c) {

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch(event.type){
      case SDL_KEYDOWN:
        c->handle_keypress(event.key.keysym.sym);
        break;
    case SDL_WINDOWEVENT: //window moved, max,min, etc
      if(event.window.event == SDL_WINDOWEVENT_RESIZED)
      {
        //int old_size = (0xffff &event.window.data1);
        //int new_size = (0xffff &event.window.data2);
        //disregard horizontal for now
        //FONT_SIZE = new_size/old_size * FONT_SIZE;
        //FONT_SIZE++;
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

void GraphicsImpl::drawString(int x, int y, const string & str, const GraphicsImpl::Context gc) {
  LoadText(str, TEMP_FONT_PATH);
  int w = 0, h = 0;
  int errcode = TTF_SizeText(best_font, str.c_str(), &w, &h);
  if (errcode == -1) assert(false);
  SDL_Rect dstRect = {x, y - 12, w, h};
  sdl_last_call = SDL_RenderCopy(ren,ttf_texture, NULL, &dstRect);
//  main_texture
}

void GraphicsImpl::drawChar(int x, int y, char ch, const GraphicsImpl::Context gc) {
  //ugh. XXX
  char buff[2];
  buff[0]=ch;
  buff[1]=0;
  // old drawString(x,y,buff,gc);
  if(cached_textures[ch] == NULL) //can't load entry, load it
  {
   LoadText(buff,TEMP_FONT_PATH);
   cached_textures[ch]=ttf_texture;
   ttf_texture=NULL; //we manage memory now

  }

   int w = 0, h = 0;
   int errcode = TTF_SizeText(best_font, buff, &w, &h);
   if (errcode == -1) assert(false);
   SDL_Rect dstRect = {x, y - 12, w, h};
   sdl_last_call = SDL_RenderCopy(ren, cached_textures[ch], NULL, &dstRect);
}

void GraphicsImpl::repaint() {
  SDL_RenderClear(ren);
 // SDL_RenderCopy(ren, main_texture, NULL,NULL); TBD

  for(auto p : c)
    p->render(*this);
  
  SDL_RenderPresent(ren);
}

void GraphicsImpl::destroy() {
  if (destroyed) return;
  if(!main_texture) SDL_DestroyTexture(main_texture);
  if(!ren) SDL_DestroyRenderer(ren);
  if(!win) SDL_DestroyWindow(win);
  if(!best_font) TTF_CloseFont(best_font);
  SDL_Quit();
  destroyed = true;
}

Graphics* new_graphics() { return new GraphicsImpl(); }

//// Now for the plug functions
void Graphics::drawString(int x, int y, const std::string& str, Context gc) {
  return static_cast<GraphicsImpl*>(this)->drawString(x,y,str,gc);
}
void Graphics::drawChar(int x, int y, char str, Context gc) {
  return static_cast<GraphicsImpl*>(this)->drawChar(x,y,str,gc);
}
void Graphics::handle_events(Controller* c) {
  return static_cast<GraphicsImpl*>(this)->handle_events(c);
}
void Graphics::repaint() {
  return static_cast<GraphicsImpl*>(this)->repaint();
}
void Graphics::destroy() {
  return static_cast<GraphicsImpl*>(this)->destroy();
}
