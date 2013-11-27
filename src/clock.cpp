#include "clock.hpp"

#include <sstream>
#include <iomanip>

using namespace std;

unsigned int gametime = 0;
unsigned int animtime = 0;

string format_time(unsigned int t) {
  unsigned int secs = t % 60;
  t /= 60;
  unsigned int mins = t % 60;
  t /= 60;
  unsigned int hours = t % 24;
  t /= 24;
  unsigned int days = t;

  stringstream ss;
  ss << setfill('0') << setw(2) << hours
     << ":" << setw(2) << mins
     << ":" << setw(2) << secs
     << " Daycycle " << days;
  string ret = ss.str();
  return ret;
}
