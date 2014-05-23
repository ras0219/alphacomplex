#pragma once

#include "widget.hpp"

struct ItemList : StaticWidget<ItemList> {
    ItemList(struct CityCursor const& cc);

    void render(Graphics& g, render_box const& pos);

    // Can't assign to types with references
    ItemList& operator=(ItemList&) = delete;

private:
    struct CityCursor const& m_curs;
};