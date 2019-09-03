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
  uint8_t x = 0x3e;
  cout << hex << setw(2) << setfill('0') << static_cast<uint16_t>(x) << " " << static_cast<uint16_t>(Parity<6, 2>::calc(x)) << dec << endl;

  uint8_t ff = 0xff;
  uint8_t fe = 0xef;

  cout << hex << setw(2) << setfill('0') << static_cast<uint16_t>(ff) << " " << static_cast<uint16_t>(Parity<6>::calc(ff)) << dec << endl;
  cout << hex << setw(2) << setfill('0') << static_cast<uint16_t>(fe) << " " << static_cast<uint16_t>(Parity<6>::calc(fe)) << dec << endl;

  cout << hex << setw(2) << setfill('0') << static_cast<uint16_t>(ff) << " " << static_cast<uint16_t>(Parity<7>::calc(ff)) << dec << endl;
  cout << hex << setw(2) << setfill('0') << static_cast<uint16_t>(fe) << " " << static_cast<uint16_t>(Parity<7>::calc(fe)) << dec << endl;

  cout << hex << setw(2) << setfill('0') << static_cast<uint16_t>(ff) << " " << static_cast<uint16_t>(Parity<8>::calc(ff)) << dec << endl;
  cout << hex << setw(2) << setfill('0') << static_cast<uint16_t>(fe) << " " << static_cast<uint16_t>(Parity<8>::calc(fe)) << dec << endl;

  return 0;
}
