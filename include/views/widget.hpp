#pragma once

struct render_box {
    enum align {
        left = 0,
        top = 0,
        center = 1,
        right = 2,
        bottom = 2
    };

    int x, y, w, h;

    inline void shrink_top(int dy);
    inline void shrink_bottom(int dy);
    inline void shrink_left(int dx);
    inline void shrink_right(int dx);

    inline void set_width(int w2, align a);
    inline void set_height(int h2, align a);
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

inline void render_box::shrink_top(int dy) {
    y += dy;
    h -= dy;
}
inline void render_box::shrink_bottom(int dy) {
    h -= dy;
}
inline void render_box::shrink_left(int dx) {
    x += dx;
    w -= dx;
}
inline void render_box::shrink_right(int dx) {
    w -= dx;
}

inline void render_box::set_width(int w2, align a) {
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

inline void render_box::set_height(int h2, align a) {
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
