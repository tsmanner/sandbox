#include <iomanip>
#include <iostream>

#include "Parity.h"


using std::cout;
using std::dec;
using std::endl;
using std::hex;
using std::setw;
using std::setfill;


int main() {
  uint16_t ff = 0xffff;
  uint16_t fe = 0xfeff;

  cout << hex << setw(2) << setfill('0') << ff << dec << " " << Parity<14>::calc(ff) << endl;
  cout << hex << setw(2) << setfill('0') << fe << dec << " " << Parity<14>::calc(fe) << endl;

  cout << hex << setw(2) << setfill('0') << ff << dec << " " << Parity<16>::calc(ff) << endl;
  cout << hex << setw(2) << setfill('0') << fe << dec << " " << Parity<16>::calc(fe) << endl;

  return 0;
}
