#include <iomanip>
#include <iostream>
#include <tuple>

using std::cout;
using std::dec;
using std::endl;
using std::hex;


#include "BitRange.h"
#include "Fold.h"
#include "Interleave.h"
#include "Reverse.h"
#include "Shuffle.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"


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


//
// Interleave
//   Interleaves a slice of a bit-range N-ways
//

// Simple 4-bit interleave test pattern
// abcd -> acbd
// 0123 -> 0213
TEST_CASE("Query Interleave", "[interleave][query]") {
  CHECK(Interleave<0, 5, 2>::query<0>() == 0);
  CHECK(Interleave<0, 5, 2>::query<1>() == 2);
  CHECK(Interleave<0, 5, 2>::query<2>() == 4);
  CHECK(Interleave<0, 5, 2>::query<3>() == 1);
  CHECK(Interleave<0, 5, 2>::query<4>() == 3);
  CHECK(Interleave<0, 5, 2>::query<5>() == 5);
}


//
// Shuffle
//   Rotates bits through a variadic list of indices
//

// Simple 4-bit shuffle test pattern
// 3 -> 0 -> 1 -> 3
// 2 unchanged
TEST_CASE("Query Shuffle", "[shuffle][query]") {
  CHECK(Shuffle<3, 0, 1>::query<0>() == 1);
  CHECK(Shuffle<3, 0, 1>::query<1>() == 3);
  CHECK(Shuffle<3, 0, 1>::query<2>() == 2);
  CHECK(Shuffle<3, 0, 1>::query<3>() == 0);
}


//
// Reverse
//

// Left aligned 4-bit reversal test pattern
// 0123 -> 3210
TEST_CASE("Left Aligned Reverse", "[reverse][query]") {
  CHECK(Reverse<0, 3>::query<0>() == 3);
  CHECK(Reverse<0, 3>::query<1>() == 2);
  CHECK(Reverse<0, 3>::query<2>() == 1);
  CHECK(Reverse<0, 3>::query<3>() == 0);
}

// Unaligned 4-bit reversal test pattern
// 2345 -> 5432
TEST_CASE("Unaligned Reverse", "[reverse][query]") {
  CHECK(Reverse<2, 5>::query<2>() == 5);
  CHECK(Reverse<2, 5>::query<3>() == 4);
  CHECK(Reverse<2, 5>::query<4>() == 3);
  CHECK(Reverse<2, 5>::query<5>() == 2);
}


//
// BitRange
//

// Make sure that a BitRange with no scrambles doesn't
// modify the data.
TEST_CASE("BitRange - No Scrambles", "[bitrange][apply]") {
  CHECK(BitRange<0, 3>::apply(0x3) == 0x3);
}

TEST_CASE("Applied Interleave", "[interleave][bitrange][apply]") {
  //                                                 0011 -> 0101
  CHECK(BitRange<0, 3, Interleave<0, 3, 2>>::apply(0x3) == 0x5);
}


//
// Fold
//

TEST_CASE("Fold") {
  using f = Fold<0, 15, 2, Shuffle<0, 1>>;
  CHECK(f::query< 0>() ==  1);
  CHECK(f::query< 1>() ==  0);
  CHECK(f::query< 2>() ==  3);
  CHECK(f::query< 3>() ==  2);
  CHECK(f::query< 4>() ==  5);
  CHECK(f::query< 5>() ==  4);
  CHECK(f::query< 6>() ==  7);
  CHECK(f::query< 7>() ==  6);
  CHECK(f::query< 8>() ==  9);
  CHECK(f::query< 9>() ==  8);
  CHECK(f::query<10>() == 11);
  CHECK(f::query<11>() == 10);
  CHECK(f::query<12>() == 13);
  CHECK(f::query<13>() == 12);
  CHECK(f::query<14>() == 15);
  CHECK(f::query<15>() == 14);
}
