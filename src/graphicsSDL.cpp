#include "graphics.hpp"
#include "component.hpp"
#include "input.hpp"

#include <unistd.h> //let mingw handle it if needed
#include <cassert>
#include <iostream>
#include <vector>
#include <chrono>
#include <deque>

#include <X11/keysym.h>
#include "SDL.h"
#include "SDL_ttf.h" //XXX- REMOVE ASAP

#define TEMP_FONT_PATH "../resources/font/Ubuntu-Regular.ttf"
int FONT_SIZE = 10; //notice how this is NOT a define. hacky hacky.

using namespace std;
using namespace chrono;

struct GraphicsInternal {
  SDL_Window *win;
  SDL_Renderer *ren;
  SDL_Texture *main_texture;
  SDL_Texture *ttf_texture;
  SDL_Color font_color;
  int sdl_last_call;
  int width, height;
};

Graphics::Graphics() : pImpl(new GraphicsInternal()) {
  //best constructor NA
  pImpl->win=NULL;
  pImpl->ren=NULL;
  pImpl->main_texture=NULL;
  pImpl->sdl_last_call=pImpl->width=pImpl->height=0;
  //constructor ends
  pImpl->sdl_last_call = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS); 
  if(pImpl->sdl_last_call == -1)
  {
    //to-do: use a better funciton for this
    std::cout << "Failed to start SDL. "<<SDL_GetError() << std::endl;
    exit(-1); //to-do: use a logger.
  }
  pImpl->sdl_last_call= TTF_Init(); //not use for long
  pImpl->font_color={0,255,0,0};

  if(pImpl->sdl_last_call ==-1)
  {
    std::cout << "Failed to start TTF. "<<TTF_GetError() << std::endl;
    exit(-1); //to-do: use a logger.
  }

  pImpl->width = 80*FONT_SIZE; //to-do
  pImpl->height= 40*FONT_SIZE;

  //to-do: center on screen
  pImpl->win = SDL_CreateWindow("", 100,100,pImpl->width, pImpl->height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  if(pImpl->win == NULL)
  {
    std::cout << "Failed to make window. "<<SDL_GetError() << std::endl;
    exit(-1); //to-do: use a logger.
  }
  pImpl->ren = SDL_CreateRenderer(pImpl->win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if(pImpl->ren == NULL)
  {
    std::cout << "Failed to make renderer. "<<SDL_GetError() << std::endl;
    exit(-1); //to-do: use a logger.
  }
  //to-do: load the file
  SDL_RenderClear(pImpl->ren);

}

void Graphics::LoadText(const std::string msg, const std::string font_file, int font_size)
{
  if(pImpl->ttf_texture!=NULL) SDL_DestroyTexture(pImpl->ttf_texture);
  pImpl->ttf_texture = NULL;
  TTF_Font * font = TTF_OpenFont(font_file.c_str(), font_size);
  if(font==NULL)
  {
    std::cout << "Failed to load ttf from  . " << font_file << " : " <<TTF_GetError() << std::endl;
    exit(-1); //to-do: use a logger.
  }
  SDL_Surface *surf = TTF_RenderText_Blended(font, msg.c_str(), pImpl->font_color);
  pImpl->ttf_texture = SDL_CreateTextureFromSurface(pImpl->ren, surf);
  SDL_FreeSurface(surf);
  TTF_CloseFont(font);
  return;
}
void Graphics::handle_events(Controller* c) {

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch(event.type){
      case SDL_KEYDOWN:
              c->handle_keypress(event.key.keysym.sym); //not going to work on windows
              break;
      case SDL_WINDOWEVENT: //window moved, max,min, etc
              if(event.window.event == SDL_WINDOWEVENT_RESIZED)
              {
                int old_size = (0xffff &event.window.data1);
                int new_size = (0xffff &event.window.data2);
                //disregard horizontal for now
                //FONT_SIZE = new_size/old_size * FONT_SIZE;
                FONT_SIZE++;
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
  LoadText(str, TEMP_FONT_PATH, FONT_SIZE);
  SDL_Rect dstRect = {x,y, (int)(str.length() * FONT_SIZE), FONT_SIZE};
  pImpl->sdl_last_call = SDL_RenderCopy(pImpl->ren,pImpl->ttf_texture, NULL, &dstRect);
//  pImpl->main_texture
}

void Graphics::drawChar(int x, int y, char ch, const Graphics::Context gc) {
  //ugh. XXX
  char buff[3];
  buff[1]=ch;
  buff[2]=0;
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
