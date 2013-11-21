#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <tuple>
#include <vector>

#include "city.hpp"
#include "pathfind.hpp"
#include "tile.hpp"

using namespace std;

/*************************/

int main() {
  cin >> city;
  cout << "Created city.\n";

  Dwarf d(9,9);
  d.insert_after(city.ent(9,9));

  Graphics g;
  g.clear();

  for (int x=0;x<5;++x) {
    cout << "----------- Step " << x << '\n';
    city.render(g);

    g.print();

    Entity* e = Entity::GLOB_ENTLIST;
    while (e != nullptr) {
      e->update();
      e = e->next;
    }
  }

  // vector<point> path = pathfind(city, 0, 0, 10, 10);

  // for (auto p : path)
  //   cout << p.first << ", " << p.second << endl;
}
