#pragma once

struct render_box {
  int x, y, w, h;

  inline void shrink_top(int dy) {
    y += dy;
    h -= dy;
  }
  inline void shrink_bottom(int dy) {
    h -= dy;
  }
  inline void shrink_left(int dx) {
    x += dx;
    w -= dx;
  }
  inline void shrink_right(int dx) {
    w -= dx;
  }

  enum align {
    left = 0,
    top = 0,
    center = 1,
    right = 2,
    bottom = 2
  };

  inline void set_width(int w2, align a) {
    if (a == left) {
      w = w2;
    } else if (a == right) {
      x += w - w2;
      w = w2;
    } else /* a == center */ {
      x += w - (w + w2) / 2;
      w = w2;
    }
  }

  inline void set_height(int h2, align a) {
    if (a == top) {
      h = h2;
    } else if (a == bottom) {
      y += h - h2;
      h = h2;
    } else /* a == center */ {
      y += h - (h + h2) / 2;
      h = h2;
    }
  }

};

struct Widget {
  virtual void render(struct Graphics& g, render_box const& pos) = 0;
};

template<class Derived, bool dynamic = false>
struct StaticWidget {
  inline void render(struct Graphics& g, render_box const& pos) {
    static_cast<Derived*>(this)->Derived::render(g, pos);
  }
};

//template<class Derived>
//struct StaticWidget<Derived, true> : Widget {};
