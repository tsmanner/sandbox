#ifndef test_ArraySubFields_h
#define test_ArraySubFields_h

#include "catch.hpp"
#include "ArrayContent.h"
#include "ArrayField.h"
#include "ArrayFields.h"

#include "../BitManipulation/Interleave.h"
#include "../BitManipulation/Reverse.h"


TEST_CASE("ArraySubFields No Scrambling") {
  using EntryType = ArrayFields<
    ArrayField<0>,    // 1 bit
    ArrayField<1, 2>, // 2 bits
    ArrayField<3, 5>, // 3 bits
    ArrayField<6, 9>  // 4 bits
  >;
  using ContentType = ArrayContent<
    ArrayFields<
      ArraySubFields< 0,  9, EntryType>,
      ArraySubFields<10, 19, EntryType>
    >
  >;

  // Check the typedef's properties
  unsigned width = ContentType::WIDTH;
  CHECK(width == 20);
  unsigned numFields = ContentType::NumFields;
  CHECK(numFields == 2);
  unsigned numScrambles = ContentType::NumScrambles;
  CHECK(numScrambles == 0);
  CHECK(std::is_same<typename ContentType::DataType, uint32_t>::value);

  SECTION("Query") {
    CHECK(ContentType::query< 0>() ==  0);
    CHECK(ContentType::query< 1>() ==  1);
    CHECK(ContentType::query< 2>() ==  2);
    CHECK(ContentType::query< 3>() ==  3);
    CHECK(ContentType::query< 4>() ==  4);
    CHECK(ContentType::query< 5>() ==  5);
    CHECK(ContentType::query< 6>() ==  6);
    CHECK(ContentType::query< 7>() ==  7);
    CHECK(ContentType::query< 8>() ==  8);
    CHECK(ContentType::query< 9>() ==  9);
    CHECK(ContentType::query<10>() == 10);
    CHECK(ContentType::query<11>() == 11);
    CHECK(ContentType::query<12>() == 12);
    CHECK(ContentType::query<13>() == 13);
    CHECK(ContentType::query<14>() == 14);
    CHECK(ContentType::query<15>() == 15);
    CHECK(ContentType::query<16>() == 16);
    CHECK(ContentType::query<17>() == 17);
    CHECK(ContentType::query<18>() == 18);
    CHECK(ContentType::query<19>() == 19);
  }

  SECTION("Scramble") {
    CHECK(ContentType::scramble(0b11111111110000000000) == 0b11111111110000000000);
  }

  SECTION("Unscramble") {
    CHECK(ContentType::unscramble(0b11111111110000000000) == 0b11111111110000000000);
  }

}


TEST_CASE("ArraySubFields With SubField Scrambling") {
  using EntryType = ArrayFields<
    ArrayField<0>,     // 1 bit
    ArrayField<1,  2>, // 2 bits
    ArrayField<3,  5>, // 3 bits
    ArrayField<6, 11>  // 6 bits
  >;
  using ContentType = ArrayContent<
    ArrayFields<
      ArraySubFields< 0, 11, EntryType>,
      ArraySubFields<12, 23, EntryType>
    >,
    Interleave<0, 23, 2>,
    Reverse<12, 23>
  >;

  // Check the typedef's properties
  unsigned width = ContentType::WIDTH;
  CHECK(width == 24);
  unsigned numFields = ContentType::NumFields;
  CHECK(numFields == 2);
  unsigned numScrambles = ContentType::NumScrambles;
  CHECK(numScrambles == 2);
  CHECK(std::is_same<typename ContentType::DataType, uint32_t>::value);

  SECTION("Query") {
    CHECK(ContentType::query< 0>() ==  0);
    CHECK(ContentType::query<12>() ==  1);
    CHECK(ContentType::query< 1>() ==  2);
    CHECK(ContentType::query<13>() ==  3);
    CHECK(ContentType::query< 2>() ==  4);
    CHECK(ContentType::query<14>() ==  5);
    CHECK(ContentType::query< 3>() ==  6);
    CHECK(ContentType::query<15>() ==  7);
    CHECK(ContentType::query< 4>() ==  8);
    CHECK(ContentType::query<16>() ==  9);
    CHECK(ContentType::query< 5>() == 10);
    CHECK(ContentType::query<17>() == 11);
    CHECK(ContentType::query<23>() == 12);
    CHECK(ContentType::query<11>() == 13);
    CHECK(ContentType::query<22>() == 14);
    CHECK(ContentType::query<10>() == 15);
    CHECK(ContentType::query<21>() == 16);
    CHECK(ContentType::query< 9>() == 17);
    CHECK(ContentType::query<20>() == 18);
    CHECK(ContentType::query< 8>() == 19);
    CHECK(ContentType::query<19>() == 20);
    CHECK(ContentType::query< 7>() == 21);
    CHECK(ContentType::query<18>() == 22);
    CHECK(ContentType::query< 6>() == 23);
  }

  SECTION("Scramble") {
    CHECK(ContentType::scramble(0b111111111111000000000000) == 0b101010101010010101010101);
  }

  SECTION("Unscramble") {
    CHECK(ContentType::unscramble(0b101010101010010101010101) == 0b111111111111000000000000);
  }

  SECTION("Instance Checks") {
    auto array = ContentType(
      EntryType(0b1, 0b11, 0b111, 0b111111),
      EntryType(0b0, 0b00, 0b000, 0b000000)
    );
    CHECK(array.getContent() == 0b111111111111000000000000);
    CHECK(array.getScrambledContent() == 0b101010101010010101010101);
    SECTION("Assignment") {
      array = 0b000000000000111111111111;
      CHECK(array.getContent() == 0b000000000000111111111111);
      CHECK(array.getScrambledContent() == 0b010101010101101010101010);
    }
  }

}


TEST_CASE("ArraySubFields With Both Scrambling") {
  using EntryType = ArrayContent<
    ArrayFields<
      ArrayField<0>,     // 1 bit
      ArrayField<1,  2>, // 2 bits
      ArrayField<3,  5>, // 3 bits
      ArrayField<6, 11>  // 6 bits
    >,
    Interleave<0, 11, 2>
  >;
  using ContentType = ArrayContent<
    ArrayFields<
      ArraySubFields< 0, 11, EntryType>,
      ArraySubFields<12, 23, EntryType>
    >,
    Reverse<12, 23>
  >;

  // Check the typedef's properties
  unsigned width = ContentType::WIDTH;
  CHECK(width == 24);
  unsigned numFields = ContentType::NumFields;
  CHECK(numFields == 2);
  unsigned numScrambles = ContentType::NumScrambles;
  CHECK(numScrambles == 1);
  CHECK(std::is_same<typename ContentType::DataType, uint32_t>::value);

  SECTION("Query") {
    CHECK(ContentType::query< 0>() ==  0);
    CHECK(ContentType::query< 6>() ==  1);
    CHECK(ContentType::query< 1>() ==  2);
    CHECK(ContentType::query< 7>() ==  3);
    CHECK(ContentType::query< 2>() ==  4);
    CHECK(ContentType::query< 8>() ==  5);
    CHECK(ContentType::query< 3>() ==  6);
    CHECK(ContentType::query< 9>() ==  7);
    CHECK(ContentType::query< 4>() ==  8);
    CHECK(ContentType::query<10>() ==  9);
    CHECK(ContentType::query< 5>() == 10);
    CHECK(ContentType::query<11>() == 11);

    CHECK(ContentType::query<12>() == 23);
    CHECK(ContentType::query<13>() == 21);
    CHECK(ContentType::query<14>() == 19);
    CHECK(ContentType::query<15>() == 17);
    CHECK(ContentType::query<16>() == 15);
    CHECK(ContentType::query<17>() == 13);
    CHECK(ContentType::query<18>() == 22);
    CHECK(ContentType::query<19>() == 20);
    CHECK(ContentType::query<20>() == 18);
    CHECK(ContentType::query<21>() == 16);
    CHECK(ContentType::query<22>() == 14);
    CHECK(ContentType::query<23>() == 12);

  }

  SECTION("Scramble") {
    CHECK(ContentType::scramble(0b111111000000111111000000) == 0b101010101010010101010101);
  }

  SECTION("Unscramble") {
    CHECK(ContentType::unscramble(0b101010101010010101010101) == 0b111111000000111111000000);
  }

  SECTION("Instance Checks") {
    auto array = ContentType(
      EntryType(0b1, 0b11, 0b111, 0b000000),
      EntryType(0b1, 0b11, 0b111, 0b000000)
    );
    CHECK(array.getContent() == 0b111111000000111111000000);
    CHECK(array.getScrambledContent() == 0b101010101010010101010101);
    SECTION("Assignment") {
      array = 0b000000111111000000111111;
      CHECK(array.getContent() == 0b000000111111000000111111);
      CHECK(array.getScrambledContent() == 0b010101010101101010101010);
    }
  }

}


#endif
