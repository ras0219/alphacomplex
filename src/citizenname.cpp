#include "citizenname.hpp"

#include <cstdlib>

vector<string> names = {
  "Alex",
  "Alice",
  "Ansi",
  "Arnold",
  "Ben",
  "Bob",
  "Bill",
  "Casie",
  "Cal",
  "David",
  "Dan",
  "John",
  "Jim",
  "James",
  "Zim"
};

vector<string> sectors = {
  "QNX",
  "ABC",
  "TRN",
  "NZB",
  "EST",
  "PST",
  "CST",
  "ZNM",
  "OOO",
  "OPS",
  "UPS",
  "QAZ",
  "SDF",
  "MLY",
  "TME",
  "GOX"
};


CitizenName* random_citizenname() {
  return new CitizenName(names[rand() % names.size()],
                         sectors[rand() % sectors.size()]);
}
