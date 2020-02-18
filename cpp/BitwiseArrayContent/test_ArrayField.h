#ifndef test_ArrayField_h
#define test_ArrayField_h

#include "catch.hpp"
#include "ArrayField.h"


TEST_CASE("ArrayField DataType") {
  SECTION("1 bit") {
    unsigned width = ArrayField<3>::WIDTH;
    CHECK(width == 1);
    CHECK(std::is_same<typename ArrayField<3>::DataType, uint8_t>::value);
  }
  SECTION("2-8 bit") {
    CHECK(std::is_same<typename ArrayField<0, 7>::DataType, uint8_t>::value);
    CHECK(std::is_same<typename ArrayField<1, 6>::DataType, uint8_t>::value);
  }
  SECTION("9-16 bit") {
    CHECK(std::is_same<typename ArrayField<0, 15>::DataType, uint16_t>::value);
    CHECK(std::is_same<typename ArrayField<1, 14>::DataType, uint16_t>::value);
  }
  SECTION("17-32 bit") {
    CHECK(std::is_same<typename ArrayField<0, 31>::DataType, uint32_t>::value);
    CHECK(std::is_same<typename ArrayField<1, 30>::DataType, uint32_t>::value);
  }
  SECTION("33-64 bit") {
    CHECK(std::is_same<typename ArrayField<0, 63>::DataType, uint64_t>::value);
    CHECK(std::is_same<typename ArrayField<1, 62>::DataType, uint64_t>::value);
  }
}


#endif
