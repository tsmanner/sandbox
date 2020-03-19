#ifndef test_ArrayContent_h
#define test_ArrayContent_h

#include "catch.hpp"
#include "ArrayContent.h"
#include "ArrayField.h"
#include "ArrayFields.h"

#include "../BitManipulation/Interleave.h"
#include "../BitManipulation/Reverse.h"


TEST_CASE("ArrayContent No Scrambling") {
  using ContentType = ArrayContent<
    ArrayFields<
      ArrayField<0>,    // 1 bit
      ArrayField<1, 2>, // 2 bits
      ArrayField<3, 5>, // 3 bits
      ArrayField<6, 9>  // 4 bits
    >
  >;

  // Check the typedef's properties
  unsigned width = ContentType::WIDTH;
  CHECK(width == 10);
  unsigned numFields = ContentType::NumFields;
  CHECK(numFields == 4);
  unsigned numScrambles = ContentType::NumScrambles;
  CHECK(numScrambles == 0);
  CHECK(std::is_same<typename ContentType::DataType, uint16_t>::value);

  SECTION("Query") {
    CHECK(ContentType::query<0>() == 0);
    CHECK(ContentType::query<1>() == 1);
    CHECK(ContentType::query<2>() == 2);
    CHECK(ContentType::query<3>() == 3);
    CHECK(ContentType::query<4>() == 4);
    CHECK(ContentType::query<5>() == 5);
    CHECK(ContentType::query<6>() == 6);
    CHECK(ContentType::query<7>() == 7);
    CHECK(ContentType::query<8>() == 8);
    CHECK(ContentType::query<9>() == 9);
  }

  SECTION("shift_bits") {
    CHECK(ContentType::shift_bits<-2>(0b00100) == 0b10000);
    CHECK(ContentType::shift_bits<0>(0b00100) == 0b00100);
    CHECK(ContentType::shift_bits<2>(0b00100) == 0b00001);
  }

  SECTION("Scramble") {
    CHECK(ContentType::scramble(0b00100) == 0b00100);
  }

  SECTION("Unscramble") {
    CHECK(ContentType::unscramble(0b00100) == 0b00100);
  }

}


TEST_CASE("ArrayContent No Scrambling Non-Contiguous Fields") {
  using ContentType = ArrayContent<
    ArrayFields<
      ArrayField<0>,    // 1 bit
      ArrayField<1>,    // 1 bits
      ArrayField<3, 5>, // 3 bits
      ArrayField<6, 9>  // 4 bits
    >
  >;

  // Check the typedef's properties
  unsigned width = ContentType::WIDTH;
  CHECK(width == 10);
  unsigned numFields = ContentType::NumFields;
  CHECK(numFields == 4);
  unsigned numScrambles = ContentType::NumScrambles;
  CHECK(numScrambles == 0);
  CHECK(std::is_same<typename ContentType::DataType, uint16_t>::value);

  SECTION("Query") {
    CHECK(ContentType::query<0>() == 0);
    CHECK(ContentType::query<1>() == 1);
    CHECK(ContentType::query<2>() == 2);
    CHECK(ContentType::query<3>() == 3);
    CHECK(ContentType::query<4>() == 4);
    CHECK(ContentType::query<5>() == 5);
    CHECK(ContentType::query<6>() == 6);
    CHECK(ContentType::query<7>() == 7);
    CHECK(ContentType::query<8>() == 8);
    CHECK(ContentType::query<9>() == 9);
  }

  SECTION("Scramble") {
    CHECK(ContentType::scramble(0b00100) == 0b00100);
  }

  SECTION("Unscramble") {
    CHECK(ContentType::unscramble(0b00100) == 0b00100);
  }

}


TEST_CASE("ArrayContent With Scrambling") {
  using ContentType = ArrayContent<
    ArrayFields<
      ArrayField<0>,     // 1 bit
      ArrayField<1,  2>, // 2 bits
      ArrayField<3,  5>, // 3 bits
      ArrayField<6, 11>  // 6 bits
    >,
    Interleave<0, 11, 2>,
    Reverse<6, 11>
  >;

  // Check the typedef's properties
  unsigned width = ContentType::WIDTH;
  CHECK(width == 12);
  unsigned numFields = ContentType::NumFields;
  CHECK(numFields == 4);
  unsigned numScrambles = ContentType::NumScrambles;
  CHECK(numScrambles == 2);
  CHECK(std::is_same<typename ContentType::DataType, uint16_t>::value);

  SECTION("Query") {
    CHECK(ContentType::query< 0>() ==  0);
    CHECK(ContentType::query< 6>() ==  1);
    CHECK(ContentType::query< 1>() ==  2);
    CHECK(ContentType::query< 7>() ==  3);
    CHECK(ContentType::query< 2>() ==  4);
    CHECK(ContentType::query< 8>() ==  5);
    CHECK(ContentType::query<11>() ==  6);
    CHECK(ContentType::query< 5>() ==  7);
    CHECK(ContentType::query<10>() ==  8);
    CHECK(ContentType::query< 4>() ==  9);
    CHECK(ContentType::query< 9>() == 10);
    CHECK(ContentType::query< 3>() == 11);
  }

  SECTION("Scramble") {
    CHECK(ContentType::scramble(0b111111000000) == 0b101010010101);
  }

  SECTION("Unscramble") {
    CHECK(ContentType::unscramble(0b101010010101) == 0b111111000000);
  }

  SECTION("Instance Checks") {
    auto array = ContentType(0b1, 0b11, 0b111, 0b000000);
    CHECK(array.getContent() == 0b111111000000);
    CHECK(array.getScrambledContent() == 0b101010010101);
    SECTION("Assignment") {
      array = 0b000000111111;
      CHECK(array.getContent() == 0b000000111111);
      CHECK(array.getScrambledContent() == 0b010101101010);
    }
  }

}


TEST_CASE("ArrayContent With Scrambling Non-Contiguous Fields") {
  using ContentType = ArrayContent<
    ArrayFields<
      ArrayField<0>,     // 1 bit
      ArrayField<1>,     // 1 bits
      ArrayField<3,  5>, // 3 bits
      ArrayField<6, 11>  // 6 bits
    >,
    Interleave<0, 11, 2>,
    Reverse<6, 11>
  >;

  // Check the typedef's properties
  unsigned width = ContentType::WIDTH;
  CHECK(width == 12);
  unsigned numFields = ContentType::NumFields;
  CHECK(numFields == 4);
  unsigned numScrambles = ContentType::NumScrambles;
  CHECK(numScrambles == 2);
  CHECK(std::is_same<typename ContentType::DataType, uint16_t>::value);

  SECTION("Query") {
    CHECK(ContentType::query< 0>() ==  0);
    CHECK(ContentType::query< 6>() ==  1);
    CHECK(ContentType::query< 1>() ==  2);
    CHECK(ContentType::query< 7>() ==  3);
    CHECK(ContentType::query< 2>() ==  4);
    CHECK(ContentType::query< 8>() ==  5);
    CHECK(ContentType::query<11>() ==  6);
    CHECK(ContentType::query< 5>() ==  7);
    CHECK(ContentType::query<10>() ==  8);
    CHECK(ContentType::query< 4>() ==  9);
    CHECK(ContentType::query< 9>() == 10);
    CHECK(ContentType::query< 3>() == 11);
  }

  SECTION("Scramble") {
    CHECK(ContentType::scramble(0b111111000000) == 0b101010010101);
  }

  SECTION("Unscramble") {
    CHECK(ContentType::unscramble(0b101010010101) == 0b111111000000);
  }

  SECTION("Instance Checks") {
    auto array = ContentType(0b1, 0b11, 0b111, 0b000000);
    CHECK(array.getContent() == 0b110111000000);
    CHECK(array.getScrambledContent() == 0b101000010101);
    SECTION("Assignment") {
      array = 0b000000111111;
      CHECK(array.getContent() == 0b000000111111);
      CHECK(array.getScrambledContent() == 0b010101101010);
    }
  }

}


TEST_CASE("Single Field Array") {
  using ContentType = ArrayContent<
    ArrayFields<
      ArrayField<0>
    >
  >;
  ContentType c;
}


#endif
