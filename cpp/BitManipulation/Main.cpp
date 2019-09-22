#include <iomanip>
#include <iostream>
#include <tuple>

using std::cout;
using std::dec;
using std::endl;
using std::hex;


// #include "Bitstreaming.h"
#include "Bit.h"
#include "Shuffle.h"
#include "Interleave.h"
#include "Scramble.h"

// #include "catch.hpp"


template <int I, typename... Types>
typename std::enable_if<(I == sizeof...(Types)), std::ostream&>::type
_stream(std::ostream& os, std::tuple<Types...> t) { return os; }

template <int I, typename... Types>
typename std::enable_if<(I < sizeof...(Types)), std::ostream&>::type
_stream(std::ostream& os, std::tuple<Types...> t) {
  os << std::get<I>(t);
  return _stream<I+1, Types...>(os, t);
}

template <typename... Types>
std::ostream& stream(std::ostream& os, std::tuple<Types...> t) {
  return _stream<0, Types...>(os, t);
}



int main() {
  // cout << Bitstreaming<0, 0>().length() << endl;
  // cout << Bitstreaming<0, 1>().length() << endl;
  // cout << Bitstreaming<1, 0>().length() << endl;

  // cout << Bit<0, 3>::shift() << endl;
  // // 0 -> 3 -> 1
  // // Final shift amount is 1 (1 - 0)
  // cout << Bit<0, 3, 1>::shift() << endl;

  // auto bits = std::make_tuple('0', '1', '2', '3');
  // auto shuffled = Shuffle<0, 1, 2>::apply(bits);
  // auto shuffled = Interleave<2>::apply(bits);

  // stream(cout, bits) << endl;
  // stream(cout, shuffled) << endl;

  using ilv = Interleave<0, 5, 3>;
  using scram = Scramble<0, 7, ilv, ilv>;

  auto t = std::make_tuple(
    'a', 'b', 'c', 'd', 'e', 'f'
  );

  cout << "0 -> " << ilv::query(0) << endl;
  cout << "1 -> " << ilv::query(1) << endl;
  cout << "2 -> " << ilv::query(2) << endl;
  cout << "3 -> " << ilv::query(3) << endl;
  cout << "4 -> " << ilv::query(4) << endl;
  cout << "5 -> " << ilv::query(5) << endl;
  stream(cout, t) << endl;

  auto ti = decltype(t)();
  std::get<0>(ti) = std::get<ilv::query(0)>(t);
  std::get<1>(ti) = std::get<ilv::query(1)>(t);
  std::get<2>(ti) = std::get<ilv::query(2)>(t);
  std::get<3>(ti) = std::get<ilv::query(3)>(t);
  std::get<4>(ti) = std::get<ilv::query(4)>(t);
  std::get<5>(ti) = std::get<ilv::query(5)>(t);
  stream(cout, ti) << endl;

  cout << "0 query " << hex << scram::query(0) << dec << endl;
  cout << "1 query " << hex << scram::query(1) << dec << endl;
  cout << "2 query " << hex << scram::query(2) << dec << endl;
  cout << "3 query " << hex << scram::query(3) << dec << endl;
  cout << "4 query " << hex << scram::query(4) << dec << endl;
  cout << "5 query " << hex << scram::query(5) << dec << endl;

  cout << "0 mask " << hex << scram::calculate_mask(0) << dec << endl;
  cout << "1 mask " << hex << scram::calculate_mask(1) << dec << endl;
  cout << "2 mask " << hex << scram::calculate_mask(2) << dec << endl;
  cout << "3 mask " << hex << scram::calculate_mask(3) << dec << endl;
  cout << "4 mask " << hex << scram::calculate_mask(4) << dec << endl;
  cout << "5 mask " << hex << scram::calculate_mask(5) << dec << endl;

  cout << "0 shift " << scram::calculate_shift(0) << endl;
  cout << "1 shift " << scram::calculate_shift(1) << endl;
  cout << "2 shift " << scram::calculate_shift(2) << endl;
  cout << "3 shift " << scram::calculate_shift(3) << endl;
  cout << "4 shift " << scram::calculate_shift(4) << endl;
  cout << "5 shift " << scram::calculate_shift(5) << endl;

  int x = 0xf0;
  cout << hex << x << " -> " << scram::apply(x) << dec << endl;
  cout << hex << x << " -> " << Scramble<0, 7, Interleave<1, 6, 3>>::apply(x) << dec << endl;
  return 0;
}
