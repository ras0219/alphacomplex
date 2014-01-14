
#include <stdio.h>
#include <ncurses.h>
#include <string.h>

#include "controller.hpp"
#include "graphics.hpp"
#include "views/widget.hpp"

using namespace std;

struct Graphics_Ncurses : Graphics {
  Graphics_Ncurses();

  // Methods
  void drawString(int x, int y, const std::string& str, Context gc = DEFAULT);
  void drawChar(int x, int y, char str, Context gc = DEFAULT);

  void handle_events(struct Controller*);

  void LoadText(const std::string msg, const std::string font_file);
  void repaint();
  void destroy();
  
private:
  void updateBuffer(int x, int y, char ch);
  
  char * view_buffer;
  char * back_buffer;
  int buffer_size;
};

Graphics_Ncurses::Graphics_Ncurses()
{
  // start curses mode
  initscr();
  
  // hide cursor
  if (ERR == curs_set(0))
  {
    printf("ERROR: cannot hide cursor\n");
  }
  
  // get screen size
  getmaxyx(stdscr, height, width);
  
  // create view + back buffer
  buffer_size = width * height;
  
  view_buffer = new char[buffer_size];
  back_buffer = new char[buffer_size];
  
  memset(view_buffer, ' ', buffer_size);
  
  // get input from main window
  keypad(stdscr, TRUE);
  
  // don't echo input
  noecho();
  
  // don't block on getch
  nodelay(stdscr, TRUE);
}

void Graphics_Ncurses::LoadText(
  const std::string /* msg */,
  const std::string /* font_file */)
{
  // do nothing
}

void Graphics_Ncurses::handle_events(
  Controller* c)
{
  if (destroyed) return;
  
  KeySym ch = getch();
  if (ch != ERR)
  {
    c->handle_keypress(ch);
  }
  clrtoeol();
}

void Graphics_Ncurses::updateBuffer(
  int x,
  int y,
  char ch)
{
  back_buffer[y * width + x] = ch;
}

void Graphics_Ncurses::drawString(
  int x,
  int y,
  const string & str,
  const Graphics_Ncurses::Context /* context */)
{
  for (const char ch : str)
  {
    if (x >= 0 && y >= 0 && x < width && y < height)
    {
      updateBuffer(x, y, ch);
    }
    x++;
  }
}

void Graphics_Ncurses::drawChar(
  int x,
  int y,
  char ch,
  const Graphics_Ncurses::Context /* context */)
{
  updateBuffer(x, y, ch);
}

void Graphics_Ncurses::repaint()
{
  memset(back_buffer, ' ', buffer_size);
  
  // fill the buffer
  for (auto p : c)
    p->render(*this, { 0, 0, getWidth(), getHeight() });
  
  int index = 0;
  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
      if (back_buffer[index] != view_buffer[index])
      {
        mvwaddch(stdscr, y, x, back_buffer[index]);
      }
      index++;
    }
  }
  
  char * tmp = view_buffer;
  view_buffer = back_buffer;
  back_buffer = tmp;
}

void Graphics_Ncurses::destroy()
{
  if (destroyed) return;
  
  endwin();
  
  delete [] view_buffer;
  delete [] back_buffer;
  
  destroyed = true;
}

Graphics* new_graphics()
{
  return new Graphics_Ncurses();
}

void Graphics::drawString(
  int x,
  int y,
  const std::string& str,
  Context gc)
{
  return static_cast<Graphics_Ncurses*>(this)->drawString(x,y,str,gc);
}

void Graphics::drawChar(
  int x,
  int y,
  char str,
  Context gc)
{
  return static_cast<Graphics_Ncurses*>(this)->drawChar(x,y,str,gc);
}

void Graphics::handle_events(
  Controller* c)
{
  return static_cast<Graphics_Ncurses*>(this)->handle_events(c);
}

void Graphics::repaint()
{
  return static_cast<Graphics_Ncurses*>(this)->repaint();
}

void Graphics::destroy()
{
  return static_cast<Graphics_Ncurses*>(this)->destroy();
}
