#ifndef test_ArrayFields_h
#define test_ArrayFields_h

#include "catch.hpp"
#include "ArrayFields.h"


TEST_CASE("ArrayFields") {
  using FieldsType = ArrayFields<
    ArrayField<0>,    // 1 bit
    ArrayField<1, 2>, // 2 bits
    ArrayField<3, 5>, // 3 bits
    ArrayField<6, 9>  // 4 bits
  >;

  // Check the typedef's properties
  unsigned width = FieldsType::WIDTH;
  CHECK(width == 10);
  unsigned numFields = FieldsType::NumFields;
  CHECK(numFields == 4);
  CHECK(std::is_same<typename FieldsType::DataType, uint16_t>::value);

  SECTION("Content Calculation statics") {
    CHECK(FieldsType::calculate_content(1, 2, 3, 4) == 0b1100110100);
    CHECK(FieldsType::_calculate_content<0, uint16_t, uint16_t, uint16_t, uint16_t>(1, 2, 3, 4) == 0b1100110100);
    CHECK(FieldsType::_calculate_content<1, uint16_t, uint16_t, uint16_t>(2, 3, 4) == 0b100110100);
    CHECK(FieldsType::_calculate_content<2, uint16_t, uint16_t>(3, 4) == 0b0110100);
    CHECK(FieldsType::_calculate_content<3, uint16_t>(4) == 0b0100);
  }

  SECTION("Field Calculation statics") {
    CHECK(FieldsType::_calculate_field<ArrayField<0>,    0, 0, ArrayField<0>, ArrayField<1, 2>, ArrayField<3, 5>, ArrayField<6, 9>>(1) == 0b1000000000);
    CHECK(FieldsType::_calculate_field<ArrayField<1, 2>, 1, 0, ArrayField<0>, ArrayField<1, 2>, ArrayField<3, 5>, ArrayField<6, 9>>(2) == 0b0100000000);
    CHECK(FieldsType::_calculate_field<ArrayField<3, 5>, 2, 0, ArrayField<0>, ArrayField<1, 2>, ArrayField<3, 5>, ArrayField<6, 9>>(3) == 0b0000110000);
    CHECK(FieldsType::_calculate_field<ArrayField<6, 9>, 3, 0, ArrayField<0>, ArrayField<1, 2>, ArrayField<3, 5>, ArrayField<6, 9>>(4) == 0b0000000100);
  }

  SECTION("Field Index Calculation statics") {
    CHECK(FieldsType::calculate_field_index<0>() == 0);
    CHECK(FieldsType::calculate_field_index<1>() == 1);
    CHECK(FieldsType::calculate_field_index<2>() == 1);
    CHECK(FieldsType::calculate_field_index<3>() == 2);
    CHECK(FieldsType::calculate_field_index<4>() == 2);
    CHECK(FieldsType::calculate_field_index<5>() == 2);
    CHECK(FieldsType::calculate_field_index<6>() == 3);
    CHECK(FieldsType::calculate_field_index<7>() == 3);
    CHECK(FieldsType::calculate_field_index<8>() == 3);
    CHECK(FieldsType::calculate_field_index<9>() == 3);
  }

  SECTION("Mask Calculation statics") {
    CHECK(FieldsType::calculate_mask<0>() == 0b1000000000);
    CHECK(FieldsType::calculate_mask<1>() == 0b0110000000);
    CHECK(FieldsType::calculate_mask<2>() == 0b0001110000);
    CHECK(FieldsType::calculate_mask<3>() == 0b0000001111);
    CHECK(FieldsType::calculate_mask<0, 1>() == 0b1110000000);
    CHECK(FieldsType::calculate_mask<0, 3>() == 0b1000001111);
  }

  SECTION("Shift Calculation statics") {
    CHECK(FieldsType::calculate_shift<0>() == 9);
    CHECK(FieldsType::calculate_shift<1>() == 7);
    CHECK(FieldsType::calculate_shift<2>() == 4);
    CHECK(FieldsType::calculate_shift<3>() == 0);
  }

  SECTION("Default Construction") {
    auto fields = FieldsType();
    CHECK(fields.getContent() == 0);
    CHECK(fields.getField<0>() == 0);
    CHECK(fields.getField<1>() == 0);
    CHECK(fields.getField<2>() == 0);
    CHECK(fields.getField<3>() == 0);
  }

  SECTION("Value Construction") {
    auto fields = FieldsType(1, 2, 3, 4);
    CHECK(fields.getContent() == 0b1100110100);
    CHECK(uint16_t(fields) == 0b1100110100);
    CHECK(fields.getField<0>() == 0b1);
    CHECK(fields.getField<1>() == 0b10);
    CHECK(fields.getField<2>() == 0b011);
    CHECK(fields.getField<3>() == 0b0100);

    SECTION("setContent") {
      fields.setContent(0b0011001011);
      CHECK(fields.getContent() == 0b0011001011);
      CHECK(fields.getField<0>() == 0b0);
      CHECK(fields.getField<1>() == 0b01);
      CHECK(fields.getField<2>() == 0b100);
      CHECK(fields.getField<3>() == 0b1011);
    }
    SECTION("assignment") {
      fields = 0b0011001011;
      CHECK(fields.getContent() == 0b0011001011);
      CHECK(fields.getField<0>() == 0b0);
      CHECK(fields.getField<1>() == 0b01);
      CHECK(fields.getField<2>() == 0b100);
      CHECK(fields.getField<3>() == 0b1011);
    }
    SECTION("set") {
      fields.set(0b0, 0b01, 0b100, 0b1011);
      CHECK(fields.getContent() == 0b0011001011);
      CHECK(fields.getField<0>() == 0b0);
      CHECK(fields.getField<1>() == 0b01);
      CHECK(fields.getField<2>() == 0b100);
      CHECK(fields.getField<3>() == 0b1011);
    }
    SECTION("setField") {
      fields.setField<0>(0b0);
      fields.setField<1>(0b01);
      fields.setField<2>(0b100);
      fields.setField<3>(0b1011);
      CHECK(fields.getContent() == 0b0011001011);
      CHECK(fields.getField<0>() == 0b0);
      CHECK(fields.getField<1>() == 0b01);
      CHECK(fields.getField<2>() == 0b100);
      CHECK(fields.getField<3>() == 0b1011);
    }
  }
}


#endif
