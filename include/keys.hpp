#pragma once

#define FONT_HEIGHT 12
#define FONT_WIDTH 6

#ifdef GRAPHICS_X11
  #include <X11/Xlib.h>
  #include <X11/keysym.h>
  
  #define KEY_Escape  XK_Escape
  #define KEY_space   XK_space
  #define KEY_Left    XK_Left
  #define KEY_Right   XK_Right
  #define KEY_Up      XK_Up
  #define KEY_Down    XK_Down
  #define KEY_Return  XK_Return

  #define KEY_Tab     XK_Tab

  #define KEY_h       XK_h
  #define KEY_u       XK_u
  #define KEY_r       XK_r
  #define KEY_a       XK_a
  #define KEY_q       XK_q
  #define KEY_e       XK_e
  #define KEY_d       XK_d
  #define KEY_f       XK_f
#endif

#ifdef GRAPHICS_SDL
  #include "SDL.h"

  using KeySym = unsigned long;

  #define KEY_Escape  SDLK_ESCAPE
  #define KEY_space   SDLK_SPACE
  #define KEY_Left    SDLK_LEFT
  #define KEY_Right   SDLK_RIGHT
  #define KEY_Up      SDLK_UP
  #define KEY_Down    SDLK_DOWN
  #define KEY_Return  SDLK_RETURN

  #define KEY_Tab     SDLK_TAB

  #define KEY_h       SDLK_h
  #define KEY_u       SDLK_u
  #define KEY_r       SDLK_r
  #define KEY_a       SDLK_a
  #define KEY_q       SDLK_q
  #define KEY_e       SDLK_e
  #define KEY_d       SDLK_d
  #define KEY_f       SDLK_f
#endif

#ifdef GRAPHICS_WEB
  typedef unsigned long KeySym;
  
  #define KEY_Escape  27
  #define KEY_space   32
  #define KEY_Left    37
  #define KEY_Right   39
  #define KEY_Up      38
  #define KEY_Down    40
  #define KEY_Return  13
  #define KEY_Tab     9
  #define KEY_h       72
  #define KEY_u       85
  #define KEY_r       82
  #define KEY_a       65
  #define KEY_q       81
  #define KEY_e       69
  #define KEY_d       68
  #define KEY_f       70
#endif

#ifdef GRAPHICS_NCURSES
  #include <ncurses.h>
  
  using KeySym = long;
  
  #define KEY_Escape  27
  #define KEY_space   ' '
  #define KEY_Left    KEY_LEFT
  #define KEY_Right   KEY_RIGHT
  #define KEY_Up      KEY_UP
  #define KEY_Down    KEY_DOWN
  #define KEY_Return  '\n'
  #define KEY_Tab     '\t'
  #define KEY_h       'h'
  #define KEY_u       'u'
  #define KEY_r       'r'
  #define KEY_a       'a'
  #define KEY_q       'q'
  #define KEY_e       'e'
  #define KEY_d       'd'
  #define KEY_f       'f'
#endif
