#include "graphics.hpp"
#include "component.hpp"
#include "controller.hpp"

#include <unistd.h> //let mingw handle it if needed
#include <cassert>
#include <iostream>
#include <vector>
#include <chrono>
#include <deque>

#include <X11/keysym.h>
#include "SDL.h"
#include "SDL_ttf.h" //XXX- REMOVE ASAP

//#define TEMP_FONT_PATH "../resources/font/UbuntuMono-R.ttf"
#define TEMP_FONT_PATH "../resources/font/Anonymous_Pro.ttf"
int FONT_HEIGHT = 12; //notice how this is NOT a define. hacky hacky.
int FONT_WIDTH = 6;

using namespace std;
using namespace chrono;

struct GraphicsInternal {
  SDL_Window *win;
  SDL_Renderer *ren;
  SDL_Texture *main_texture;
  SDL_Texture *ttf_texture;
  TTF_Font* best_font;
  SDL_Color font_color;
  int sdl_last_call;
  int width, height;

  GraphicsInternal()
    : win(nullptr), ren(nullptr), main_texture(nullptr),
      ttf_texture(nullptr), font_color({0,0,0}),
      sdl_last_call(0), width(0), height(0)
    { }
};


Graphics::Graphics() : pImpl(nullptr) {
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


  pImpl = new GraphicsInternal();
  pImpl->font_color = {0,255,0,0};
  pImpl->width = 80*FONT_WIDTH; //to-do
  pImpl->height= 40*FONT_HEIGHT;

  //to-do: center on screen
  pImpl->win = SDL_CreateWindow("", 100, 100, pImpl->width, pImpl->height,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  if(pImpl->win == nullptr) {
    std::cout << "Failed to make window. "<<SDL_GetError() << std::endl;
    exit(-1); //to-do: use a logger.
  }
  pImpl->ren = SDL_CreateRenderer(pImpl->win, -1,
                                  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if(pImpl->ren == nullptr) {
    std::cout << "Failed to make renderer. "<<SDL_GetError() << std::endl;
    exit(-1); //to-do: use a logger.
  }

  //to-do: load the file
  pImpl->best_font = TTF_OpenFont(TEMP_FONT_PATH, FONT_HEIGHT);
  if(pImpl->best_font==NULL)
  {
    std::cout << "Failed to load ttf from  . " << TEMP_FONT_PATH << " : " <<TTF_GetError() << std::endl;
    exit(-1); //to-do: use a logger.
  }
  SDL_RenderClear(pImpl->ren);
}

void Graphics::LoadText(const std::string msg, const std::string font_file)
{
  if(pImpl->ttf_texture!=NULL)
   {
    SDL_DestroyTexture(pImpl->ttf_texture);
    pImpl->ttf_texture = NULL;
   }
  SDL_Surface *surf = TTF_RenderText_Solid(pImpl->best_font, msg.c_str(), pImpl->font_color);
  pImpl->ttf_texture = SDL_CreateTextureFromSurface(pImpl->ren, surf);
  SDL_FreeSurface(surf);
  return;
}
void Graphics::handle_events(Controller* c) {

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch(event.type){
      case SDL_KEYDOWN:
        c->handle_keypress(event.key.keysym.sym);
        break;
    case SDL_WINDOWEVENT: //window moved, max,min, etc
      if(event.window.event == SDL_WINDOWEVENT_RESIZED)
      {
        int old_size = (0xffff &event.window.data1);
        int new_size = (0xffff &event.window.data2);
        //disregard horizontal for now
        //FONT_SIZE = new_size/old_size * FONT_SIZE;
        //FONT_SIZE++;
      }
      break;
    case SDL_QUIT: //send q to controller
      c->handle_keypress((KeySym)XK_Escape);
      destroy();
      return;
      break;
    default: break; // dog
    }
  }
  //all done with events! :)
}

void Graphics::drawString(int x, int y, const string & str, const Graphics::Context gc) {
  LoadText(str, TEMP_FONT_PATH);
  int w = 0, h = 0;
  assert(TTF_SizeText(pImpl->best_font, str.c_str(), &w, &h) != -1);
  SDL_Rect dstRect = {x, y - 12, w, h};
  pImpl->sdl_last_call = SDL_RenderCopy(pImpl->ren,pImpl->ttf_texture, NULL, &dstRect);
//  pImpl->main_texture
}

void Graphics::drawChar(int x, int y, char ch, const Graphics::Context gc) {
  //ugh. XXX
  char buff[2];
  buff[0]=ch;
  buff[1]=0;
  drawString(x,y,buff,gc);

}

void Graphics::repaint() {
  SDL_RenderClear(pImpl->ren);
 // SDL_RenderCopy(pImpl->ren, pImpl->main_texture, NULL,NULL); TBD

  for(auto p : c)
    p->render(*this);
  
  SDL_RenderPresent(pImpl->ren);
}

void Graphics::destroy() {
  if (destroyed) return;
  if(!pImpl->main_texture) SDL_DestroyTexture(pImpl->main_texture);
  if(!pImpl->ren) SDL_DestroyRenderer(pImpl->ren);
  if(!pImpl->win) SDL_DestroyWindow(pImpl->win);
  if(!pImpl->best_font) TTF_CloseFont(pImpl->best_font);
  SDL_Quit();
  destroyed = true;
}

Graphics::~Graphics() { 
  destroy();
  delete pImpl;
  pImpl = 0;
}

int Graphics::getWidth() { return pImpl->width; }
int Graphics::getHeight() { return pImpl->height; }
