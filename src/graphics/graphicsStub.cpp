
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
  // TODO
};

GraphicsImpl::GraphicsImpl()
{
  // TODO
}

void GraphicsImpl::LoadText(
  const std::string /* msg */,
  const std::string /* font_file */)
{
  // TODO
}

void GraphicsImpl::handle_events(
  Controller* /* c */)
{
  // TODO
  
  // c->handle_keypress(keysym);
}

void GraphicsImpl::drawString(
  int /* x */,
  int /* y */,
  const string & /* str */,
  const GraphicsImpl::Context /* context */)
{
  // TODO
}

void GraphicsImpl::drawChar(
  int /* x */,
  int /* y */,
  char /* ch */,
  const GraphicsImpl::Context /* context */)
{
  // TODO
}

void GraphicsImpl::repaint()
{
  // TODO
  
  for (auto p : c)
    p->render(*this);
}

void GraphicsImpl::destroy()
{
  //
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
