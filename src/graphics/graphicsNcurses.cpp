
#include <stdio.h>
#include <ncurses.h>
#include <string.h>

#include "controller.hpp"
#include "graphics.hpp"
#include "views/widget.hpp"

using namespace std;

struct GraphicsImpl : Graphics {
  GraphicsImpl();

  // Methods
  void drawString(int x, int y, const std::string& str, Context gc = DEFAULT);
  void drawChar(int x, int y, char str, Context gc = DEFAULT);

  void handle_events(struct Controller*);

  void LoadText(const std::string msg, const std::string font_file);
  void repaint();
  void destroy();
  
private:
  bool updateBuffer(int x, int y, char ch);
  
  char * buffer;
  int buffer_size;
};

GraphicsImpl::GraphicsImpl()
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
  
  // create screen buffer
  buffer_size = (width + 1) * (height + 1);
  buffer = new char[buffer_size];
  
  // get input from main window
  keypad(stdscr, TRUE);
  
  // don't echo input
  noecho();
  
  // don't block on getch
  nodelay(stdscr, TRUE);
}

void GraphicsImpl::LoadText(
  const std::string /* msg */,
  const std::string /* font_file */)
{
  // do nothing
}

void GraphicsImpl::handle_events(
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

bool GraphicsImpl::updateBuffer(
  int x,
  int y,
  char ch)
{
  if (buffer[y * (width + 1) + x] !=  ch)
  {
    buffer[y * (width + 1) + x] = ch;
    return true;
  }
  return false;
}

void GraphicsImpl::drawString(
  int x,
  int y,
  const string & str,
  const GraphicsImpl::Context /* context */)
{
  for (const char ch : str)
  {
    if (x >= 0 && y >= 0 && x < width && y < height)
    {
      if (updateBuffer(x, y, ch))
      {
        mvwaddch(stdscr, y, x, ch);
      }
    }
    x++;
  }
}

void GraphicsImpl::drawChar(
  int x,
  int y,
  char ch,
  const GraphicsImpl::Context /* context */)
{
  if (updateBuffer(x, y, ch))
  {
    mvwaddch(stdscr, y, x, ch);
  }
}

void GraphicsImpl::repaint()
{
  clear();
  
  memset(buffer, ' ', buffer_size);
  
  // fill the buffer
  for (auto p : c)
    p->render(*this);
  
  refresh();
}

void GraphicsImpl::destroy()
{
  if (destroyed) return;
  
  endwin();
  
  delete [] buffer;
  
  destroyed = true;
}

Graphics* new_graphics()
{
  return new GraphicsImpl();
}

void Graphics::drawString(
  int x,
  int y,
  const std::string& str,
  Context gc)
{
  return static_cast<GraphicsImpl*>(this)->drawString(x,y,str,gc);
}

void Graphics::drawChar(
  int x,
  int y,
  char str,
  Context gc)
{
  return static_cast<GraphicsImpl*>(this)->drawChar(x,y,str,gc);
}

void Graphics::handle_events(
  Controller* c)
{
  return static_cast<GraphicsImpl*>(this)->handle_events(c);
}

void Graphics::repaint()
{
  return static_cast<GraphicsImpl*>(this)->repaint();
}

void Graphics::destroy()
{
  return static_cast<GraphicsImpl*>(this)->destroy();
}
