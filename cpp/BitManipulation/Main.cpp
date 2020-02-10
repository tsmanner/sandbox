#include <iomanip>
#include <iostream>
#include <tuple>

using std::cout;
using std::dec;
using std::endl;
using std::hex;


#include "BitRange.h"
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
  REQUIRE(Interleave<0, 5, 2>::query<0>() == 0);
  REQUIRE(Interleave<0, 5, 2>::query<1>() == 2);
  REQUIRE(Interleave<0, 5, 2>::query<2>() == 4);
  REQUIRE(Interleave<0, 5, 2>::query<3>() == 1);
  REQUIRE(Interleave<0, 5, 2>::query<4>() == 3);
  REQUIRE(Interleave<0, 5, 2>::query<5>() == 5);
}


//
// Shuffle
//   Rotates bits through a variadic list of indices
//

// Simple 4-bit shuffle test pattern
// 3 -> 0 -> 1 -> 3
// 2 unchanged
TEST_CASE("Query Shuffle", "[shuffle][query]") {
  REQUIRE(Shuffle<3, 0, 1>::query<0>() == 1);
  REQUIRE(Shuffle<3, 0, 1>::query<1>() == 3);
  REQUIRE(Shuffle<3, 0, 1>::query<2>() == 2);
  REQUIRE(Shuffle<3, 0, 1>::query<3>() == 0);
}


//
// Reverse
//

// Left aligned 4-bit reversal test pattern
// 0123 -> 3210
TEST_CASE("Left Aligned Reverse", "[reverse][query]") {
  REQUIRE(Reverse<0, 3>::query<0>() == 3);
  REQUIRE(Reverse<0, 3>::query<1>() == 2);
  REQUIRE(Reverse<0, 3>::query<2>() == 1);
  REQUIRE(Reverse<0, 3>::query<3>() == 0);
}

// Unaligned 4-bit reversal test pattern
// 2345 -> 5432
TEST_CASE("Unaligned Reverse", "[reverse][query]") {
  REQUIRE(Reverse<2, 5>::query<2>() == 5);
  REQUIRE(Reverse<2, 5>::query<3>() == 4);
  REQUIRE(Reverse<2, 5>::query<4>() == 3);
  REQUIRE(Reverse<2, 5>::query<5>() == 2);
}


//
// BitRange
//

// Make sure that a BitRange with no scrambles doesn't
// modify the data.
TEST_CASE("BitRange - No Scrambles", "[bitrange][apply]") {
  REQUIRE(BitRange<0, 3>::apply(0x3) == 0x3);
}

TEST_CASE("Applied Interleave", "[interleave][bitrange][apply]") {
  //                                                 0011 -> 0101
  REQUIRE(BitRange<0, 3, Interleave<0, 3, 2>>::apply(0x3) == 0x5);
}
