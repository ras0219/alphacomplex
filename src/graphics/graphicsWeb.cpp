
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <libwebsockets.h>

#include <thread>

#include "graphics.hpp"
#include "views/widget.hpp"

using namespace std;

struct per_session_data__http {
  int number;
};

static int callback_http(
  struct libwebsocket_context * context,
  struct libwebsocket * wsi,
  enum libwebsocket_callback_reasons reason,
  void * /* user */,
  void * in,
  size_t len)
{
  //struct per_session_data__http *pss = (struct per_session_data__http *)user;

  switch (reason) {
    case LWS_CALLBACK_HTTP:
    {
      char path[256];
      
      char * requested_file = (char *)in;
      char * mimetype = (char *)"text/plain";
      
      if (strcmp(requested_file, "/") == 0)
      {
        requested_file = (char *)"index.html";
        len = 10;
      }
      
      if (len + 8 > 256)
      {
        printf("ERROR: requested file path length exceeds buffer size (%d > 256).", (int)len + 8);
        break;
      }
      
      sprintf(path, "../web/%s", requested_file);

      if (access(path, R_OK) != -1)
      {
        if ((len > 4) && (strcmp(&requested_file[len - 5], ".html") == 0))
        {
          mimetype = (char *)"text/html";
        }
        else if ((len > 3) && (strcmp(&requested_file[len - 4], ".css") == 0))
        {
          mimetype = (char *)"text/css";
        }
        else if ((len > 2) && (strcmp(&requested_file[len - 3], ".js") == 0))
        {
          mimetype = (char *)"application/javascript";
        }
        else if ((len > 3) && (strcmp(&requested_file[len - 4], ".xml") == 0))
        {
          mimetype = (char *)"application/xml";
        }
        else if ((len > 4) && (strcmp(&requested_file[len - 5], ".json") == 0))
        {
          mimetype = (char *)"application/json";
        }
        else if ((len > 3) && (strcmp(&requested_file[len - 4], ".ico") == 0))
        {
          mimetype = (char *)"image/x-icon";
        }
        else if ((len > 3) && (strcmp(&requested_file[len - 4], ".png") == 0))
        {
          mimetype = (char *)"image/png";
        }
        else if ((len > 3) && (strcmp(&requested_file[len - 4], ".jpg") == 0))
        {
          mimetype = (char *)"image/jpg";
        }
        else if ((len > 3) && (strcmp(&requested_file[len - 4], ".gif") == 0))
        {
          mimetype = (char *)"image/gif";
        }
        
        if (libwebsockets_serve_http_file(context, wsi, path, mimetype))
        {
          return -1;
        }
      }

      break;
    }
    case LWS_CALLBACK_HTTP_FILE_COMPLETION:
    {
      return -1;
    }
    default:
      break;
  }

  return 0;
}

static struct libwebsocket_protocols protocols[] = {
  /* first protocol must always be HTTP handler */
  {
    "http-only", /* name */
    callback_http, /* callback */
    sizeof (struct per_session_data__http), /* per_session_data_size */
    0, /* max frame size / rx buffer */
  },
  { NULL, NULL, 0, 0 } /* terminator */
};

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
  void server();
  
  thread server_thread;
};

GraphicsImpl::GraphicsImpl() :
  server_thread(&GraphicsImpl::server, this)
{
  server_thread.detach();
}

void GraphicsImpl::server()
{
  struct libwebsocket_context * context;
  struct lws_context_creation_info info;
  
  memset(&info, 0, sizeof info);

  info.port = 3000;
  info.iface = NULL;
  info.protocols = protocols;
  info.extensions = libwebsocket_get_internal_extensions();
  info.ssl_cert_filepath = NULL;
  info.ssl_private_key_filepath = NULL;
  info.gid = -1;
  info.uid = -1;
  info.options = 0;
  
  context = libwebsocket_create_context(&info);
  if (context == NULL)
  {
    return;
  }
  
  while (1) {
    libwebsocket_service(context, 50);
  }
  libwebsocket_context_destroy(context);
}

void GraphicsImpl::LoadText(
  const std::string /* msg */,
  const std::string /* font_file */)
{
  //printf("LoadText\n");
}

void GraphicsImpl::handle_events(
  Controller* /* c */)
{
  //printf("handle_events\n");
  
  // c->handle_keypress(keysym);
}

void GraphicsImpl::drawString(
  int /* x */,
  int /* y */,
  const string & /* str */,
  const GraphicsImpl::Context /* context */)
{
  //printf("drawString\n");
}

void GraphicsImpl::drawChar(
  int /* x */,
  int /* y */,
  char /* ch */,
  const GraphicsImpl::Context /* context */)
{
  //printf("drawChar\n");
}

void GraphicsImpl::repaint()
{
  //printf("repaint\n");
  
  for (auto p : c)
    p->render(*this);
}

void GraphicsImpl::destroy()
{
  server_thread.join();
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
