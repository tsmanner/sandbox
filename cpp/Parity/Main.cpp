#include <iostream>
#include <bitset>
#include <limits>
#include <math.h>
#include <type_traits>

#include "Parity.h"

#define CATCH_CONFIG_MAIN
#include "../_Catch2_/catch.hpp"


/*
 * Required Test Patterns
 * Each bit should be tested with a 0 and a 1,
 * with an even and odd total number of `1`s,
 * and calculating both even and odd parity.
 * To accomplish this, we can sweep a `1`
 * through each bit, and then toggle the LSB
 * to ensure that the bit being tested is
 * contributing to parity.  i.e. for 4 bits
 * the test pattern should be:
 *  Value -> Even
 *   0000 ->    0 | Special Case the LSB
 *   0001 ->    1 | Special Case the LSB
 *   0010 ->    1
 *   0011 ->    0
 *   0100 ->    1
 *   0101 ->    0
 *   1000 ->    1
 *   1001 ->    0
 */


template <typename DataType, DataType BITS, DataType BIT>
typename std::enable_if<(BIT == BITS)>::type
single_bit_parity_test_pattern() {}


template <typename DataType, DataType BITS, DataType BIT = 0>
typename std::enable_if<(0 < BIT and BIT < BITS)>::type
single_bit_parity_test_pattern() {
  // Create the test value with a `1` in the LSB
  constexpr DataType test_value_1 = (DataType(1) << BIT) | 1;
  // Create the test value with a `0` in the LSB
  constexpr DataType test_value_0 = (DataType(1) << BIT) & ~DataType(1);
  // std::cout << "TestValue0 " << std::bitset<BITS>(test_value_0) << std::endl;
  // std::cout << "TestValue1 " << std::bitset<BITS>(test_value_1) << std::endl;
  // Test `0` LSB
  REQUIRE( Parity<BITS>::even(test_value_0) == 1 );
  REQUIRE( Parity<BITS>::odd(test_value_0)  == 0 );
  // Test `1` LSB
  REQUIRE( Parity<BITS>::even(test_value_1) == 0 );
  REQUIRE( Parity<BITS>::odd(test_value_1)  == 1 );
  // Make the template recursive call to check the next bit
  single_bit_parity_test_pattern<DataType, BITS, (BIT + 1)>();
}


template <typename DataType, DataType BITS, DataType BIT = 0>
typename std::enable_if<(BIT == 0)>::type
single_bit_parity_test_pattern() {
  // Create the test value `0`
  constexpr DataType test_value_0 = DataType(0);
  // Create the test value `1`
  constexpr DataType test_value_1 = DataType(1);
  // std::cout << "TestValue0 " << std::bitset<BITS>(test_value_0) << std::endl;
  // std::cout << "TestValue1 " << std::bitset<BITS>(test_value_1) << std::endl;
  // Test `0`
  REQUIRE( Parity<BITS>::even(test_value_0) == 0 );
  REQUIRE( Parity<BITS>::odd(test_value_0)  == 1 );
  // Test `1`
  REQUIRE( Parity<BITS>::even(test_value_1) == 1 );
  REQUIRE( Parity<BITS>::odd(test_value_1)  == 0 );
  // Make the template recursive call to check the next bit
  single_bit_parity_test_pattern<DataType, BITS, (BIT + 1)>();
}

TEST_CASE("uint8_t 5to1", "[even][uint8_t][parity width 1]") {               single_bit_parity_test_pattern<uint8_t, 5>(); }
TEST_CASE("uint8_t 8to1", "[even][uint8_t][parity width 1][size aligned]") { single_bit_parity_test_pattern<uint8_t, 8>(); }

TEST_CASE("uint16_t 9to1",  "[even][uint16_t][parity width 1]") {               single_bit_parity_test_pattern<uint16_t,  9>(); }
TEST_CASE("uint16_t 16to1", "[even][uint16_t][parity width 1][size aligned]") { single_bit_parity_test_pattern<uint16_t, 16>(); }

TEST_CASE("uint32_t 17to1", "[even][uint32_t][parity width 1]") {               single_bit_parity_test_pattern<uint32_t, 17>(); }
TEST_CASE("uint32_t 32to1", "[even][uint32_t][parity width 1][size aligned]") { single_bit_parity_test_pattern<uint32_t, 32>(); }


/*
 * Test Odd Parity
 * It is sufficient to prove that Odd parity is NOT Even Parity
 * as long as Even Parity is rigorously proven to be correct.
 */

TEST_CASE("odd parity 1", "[odd][parity width 1]") {
  REQUIRE( (~(Parity<8>::even(0)) & 1) == Parity<8>::odd(0) );
  REQUIRE( (~(Parity<8>::even(1)) & 1) == Parity<8>::odd(1) );
}

TEST_CASE("odd parity 2", "[odd][parity width 2]") {
  REQUIRE( (~(Parity<8, 2>::even(0b00000000)) & 3) == Parity<8, 2>::odd(0b00000000) );
  REQUIRE( (~(Parity<8, 2>::even(0b00000001)) & 3) == Parity<8, 2>::odd(0b00000001) );
  REQUIRE( (~(Parity<8, 2>::even(0b00010000)) & 3) == Parity<8, 2>::odd(0b00010000) );
  REQUIRE( (~(Parity<8, 2>::even(0b00010001)) & 3) == Parity<8, 2>::odd(0b00010001) );
}

TEST_CASE("parity mask", "[even]") {
  // Introduce an out-of-bounds 1 to make sure it isn't used
  REQUIRE( Parity<4>::even(0b10001) == 1 );
}
