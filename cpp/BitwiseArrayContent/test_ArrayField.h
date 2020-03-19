#ifndef test_ArrayField_h
#define test_ArrayField_h

#include "catch.hpp"
#include <array>
#include "ArrayField.h"


TEST_CASE("ArrayField DataType") {
  SECTION("1 bit") {
    unsigned width = ArrayField<3>::WIDTH;
    CHECK(width == 1);
    CHECK(std::is_same<typename ArrayField<3>::DataType, uint8_t>::value);
    CHECK(ArrayField<3>(1) == 1);
  }
  SECTION("2-8 bit") {
    CHECK(std::is_same<typename ArrayField<0, 7>::DataType, uint8_t>::value);
    CHECK(std::is_same<typename ArrayField<1, 6>::DataType, uint8_t>::value);
    const uint8_t value = 0b101010;
    CHECK(ArrayField<1, 6>(value) == value);
  }
  SECTION("9-16 bit") {
    CHECK(std::is_same<typename ArrayField<0,  8>::DataType, uint16_t>::value);
    CHECK(std::is_same<typename ArrayField<0, 15>::DataType, uint16_t>::value);
    CHECK(std::is_same<typename ArrayField<1, 14>::DataType, uint16_t>::value);
    const uint16_t value = 0b10101010101010;
    CHECK(ArrayField<1, 14>(value) == value);
  }
  SECTION("17-32 bit") {
    CHECK(std::is_same<typename ArrayField<0, 16>::DataType, uint32_t>::value);
    CHECK(std::is_same<typename ArrayField<0, 31>::DataType, uint32_t>::value);
    CHECK(std::is_same<typename ArrayField<1, 30>::DataType, uint32_t>::value);
    const uint32_t value = 0b101010101010101010101010101010;
    CHECK(ArrayField<1, 30>(value) == value);
  }
  SECTION("33-64 bit") {
    CHECK(std::is_same<typename ArrayField<0, 32>::DataType, uint64_t>::value);
    CHECK(std::is_same<typename ArrayField<0, 63>::DataType, uint64_t>::value);
    CHECK(std::is_same<typename ArrayField<1, 62>::DataType, uint64_t>::value);
    const uint64_t value = 0b10101010101010101010101010101010101010101010101010101010101010;
    CHECK(ArrayField<1, 62>(value) == value);
  }
  SECTION("65-128 bit") {
    CHECK(std::is_same<typename ArrayField<0,  64>::DataType, std::array<uint64_t, 2>>::value);
    CHECK(std::is_same<typename ArrayField<0, 127>::DataType, std::array<uint64_t, 2>>::value);
    CHECK(std::is_same<typename ArrayField<1, 126>::DataType, std::array<uint64_t, 2>>::value);
    const std::array<uint64_t, 2> value = {
      0b01010101010101010101010101010101010101010101010101010101010101,
      0b1010101010101010101010101010101010101010101010101010101010101010
    };
    CHECK(ArrayField<1, 126>(value)[0] == value[0]);
    CHECK(ArrayField<1, 126>(value)[1] == value[1]);
  }
  SECTION("129-192 bit") {
    CHECK(std::is_same<typename ArrayField<0, 128>::DataType, std::array<uint64_t, 3>>::value);
    CHECK(std::is_same<typename ArrayField<0, 191>::DataType, std::array<uint64_t, 3>>::value);
    CHECK(std::is_same<typename ArrayField<1, 190>::DataType, std::array<uint64_t, 3>>::value);
    const std::array<uint64_t, 3> value = {
      0b01010101010101010101010101010101010101010101010101010101010101,
      0b1010101010101010101010101010101010101010101010101010101010101010,
      0b0101010101010101010101010101010101010101010101010101010101010101
    };
    CHECK(ArrayField<1, 190>(value)[0] == value[0]);
    CHECK(ArrayField<1, 190>(value)[1] == value[1]);
    CHECK(ArrayField<1, 190>(value)[2] == value[2]);
  }
}


#endif
