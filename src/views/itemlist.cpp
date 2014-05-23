#include "defs.hpp"
#include "views/itemlist.hpp"
#include "graphics.hpp"
#include "cursor.hpp"
#include "city.hpp"
#include "entities/entity.hpp"
#include "components/item.hpp"
#include "components/citizenname.hpp"
#include "components/clearance.hpp"
#include "entities/citizen.hpp"
#include "components/furniture.hpp"

ItemList::ItemList(struct CityCursor const& cc) : m_curs(cc) { }

void ItemList::render(Graphics& g, render_box const& pos) {
    std::string furn = "Furniture: ";
    if (auto f = m_curs.city->furniture(m_curs.x, m_curs.y)) {
        if (auto i = f->parent->has_get<item::Item>()) {
            furn += i->prop.name;
        } else {
            furn += "???";
        }
    } else {
        furn += "<none>";
    }
    g.drawString(pos.x, pos.y, furn);

    auto y = pos.y + 1;

    auto& ents = m_curs.city->ent(m_curs.x, m_curs.y);

    if (ents.size() == 0) {
        g.drawString(pos.x, y, "<no items here>");
        return;
    }

    for (auto ent : ents) {
        if (ent->has<CitizenName>() && ent->has<Clearance>()) {
            g.drawString(pos.x, y, get_full_name(ent));
            ++y;
            continue;
        }
        if (auto item = ent->has_get<item::Item>()) {
            g.drawString(pos.x, y, item->prop.name);
            ++y;
            continue;
        }
        g.drawString(pos.x, y, "????");
        ++y;
    }
}
