
#include <ncurses.h>

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
};

GraphicsImpl::GraphicsImpl()
{
  // start curses mode
  initscr();
  
  // hide cursor
  curs_set(0);
  
  // get screen size
  getmaxyx(stdscr, height, width);
  
  printf("%d %d\n", height, width);
  
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
    printf("%d\n", (int)ch);
  }
}

void GraphicsImpl::drawString(
  int x,
  int y,
  const string & str,
  const GraphicsImpl::Context /* context */)
{
  mvprintw(y, x, str.c_str());
}

void GraphicsImpl::drawChar(
  int x,
  int y,
  char ch,
  const GraphicsImpl::Context /* context */)
{
  char buffer[2] = { ch, '\0' };
  mvprintw(y, x, buffer);
}

void GraphicsImpl::repaint()
{
  clear();
  
  // OPTIMIZATION: create a buffer, render to buffer then draw buffer.
  
  for (auto p : c)
    p->render(*this);
  
  refresh();
}

void GraphicsImpl::destroy()
{
  if (destroyed) return;
  
  endwin();
  
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
