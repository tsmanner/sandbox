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
  uint16_t x = 0x3e;
  cout << hex << setw(2) << setfill('0') << static_cast<uint16_t>(x & Parity<6, 3>::DataMask<decltype(x)>::value) << " " << static_cast<uint16_t>(Parity<6, 2>::even(x)) << dec << endl;
  cout << hex << setw(2) << setfill('0') << static_cast<uint16_t>(x & Parity<6, 3>::DataMask<decltype(x)>::value) << " " << static_cast<uint16_t>(Parity<6, 2>::odd(x)) << dec << endl;

  uint16_t y = 0x551e;
  cout << hex << setw(2) << setfill('0') << static_cast<uint16_t>(y & Parity<6, 3>::DataMask<decltype(y)>::value) << " " << static_cast<uint16_t>(Parity<6, 3>::even(y)) << dec << endl;

  uint8_t ff = 0xff;
  uint8_t fe = 0xef;

  cout << hex << setw(2) << setfill('0') << static_cast<uint16_t>(ff & Parity<6>::DataMask<decltype(ff)>::value) << " " << static_cast<uint16_t>(Parity<6>::even(ff)) << dec << endl;
  cout << hex << setw(2) << setfill('0') << static_cast<uint16_t>(fe & Parity<6>::DataMask<decltype(fe)>::value) << " " << static_cast<uint16_t>(Parity<6>::even(fe)) << dec << endl;

  cout << hex << setw(2) << setfill('0') << static_cast<uint16_t>(ff & Parity<7>::DataMask<decltype(ff)>::value) << " " << static_cast<uint16_t>(Parity<7>::even(ff)) << dec << endl;
  cout << hex << setw(2) << setfill('0') << static_cast<uint16_t>(fe & Parity<7>::DataMask<decltype(fe)>::value) << " " << static_cast<uint16_t>(Parity<7>::even(fe)) << dec << endl;

  cout << hex << setw(2) << setfill('0') << static_cast<uint16_t>(ff & Parity<8>::DataMask<decltype(ff)>::value) << " " << static_cast<uint16_t>(Parity<8>::even(ff)) << dec << endl;
  cout << hex << setw(2) << setfill('0') << static_cast<uint16_t>(fe & Parity<8>::DataMask<decltype(fe)>::value) << " " << static_cast<uint16_t>(Parity<8>::even(fe)) << dec << endl;

  return 0;
}
