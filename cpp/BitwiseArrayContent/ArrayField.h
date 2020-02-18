#ifndef ArrayField_h
#define ArrayField_h

#include <type_traits>

#include "ArrayBaseClasses.h"


// ArrayFieldRange
//   Enable class for ArrayField to allow partial template
//   specialization of the structs below to define a different
//   data type depending on the distance between MSB and LSB

// Default specialization has no "enabled" typedef
template<int Start, int End, int Val, class Enable = void> struct ArrayFieldRange {};

// If "Val" lies between Start and End, inclusive,
// an "enabled" typedef
template<int Start, int End, int Val>
struct ArrayFieldRange<Start, End, Val, typename std::enable_if<Val >= Start && Val <= End>::type> {
  using enabled = void;
};

// ArrayField
//

// Default specialization only provides a WIDTH, MSB, and LSB
template <unsigned MSB_t, unsigned LSB_t=MSB_t, class Enable = void>
class ArrayField : public ArrayFieldBase {
public:
  static constexpr unsigned WIDTH = LSB_t - MSB_t + 1;
  static const unsigned MSB = MSB_t;
  static const unsigned LSB = LSB_t;

};

// Range [1-8] specialization defines DataType to be uint8_t,
// and provides a member, constructor, assignment, and cast
// operators for that type
template <unsigned MSB_t, unsigned LSB_t>
class ArrayField<MSB_t, LSB_t, typename ArrayFieldRange<1, 8, LSB_t-MSB_t+1>::enabled> {
public:
  static constexpr unsigned WIDTH = LSB_t - MSB_t + 1;
  static const unsigned MSB = MSB_t;
  static const unsigned LSB = LSB_t;
  using DataType = uint8_t;

  ArrayField(const uint8_t& inValue = 0): mValue(inValue) {}
  uint8_t operator=(const uint8_t& inValue) { return mValue = inValue; }
  operator uint8_t() const { return mValue; }

private:
  uint8_t mValue;
};

// Range [9-16] specialization defines DataType to be uint16_t,
// and provides a member, constructor, assignment, and cast
// operators for that type
template <unsigned MSB_t, unsigned LSB_t>
class ArrayField<MSB_t, LSB_t, typename ArrayFieldRange<9, 16, LSB_t-MSB_t+1>::enabled> {
public:
  static constexpr unsigned WIDTH = LSB_t - MSB_t + 1;
  static const unsigned MSB = MSB_t;
  static const unsigned LSB = LSB_t;
  using DataType = uint16_t;

  ArrayField(const uint16_t& inValue = 0): mValue(inValue) {}
  uint16_t operator=(const uint16_t& inValue) { return mValue = inValue; }
  operator uint16_t() const { return mValue; }

private:
  uint16_t mValue;

};

// Range [17-32] specialization defines DataType to be uint32_t,
// and provides a member, constructor, assignment, and cast
// operators for that type
template <unsigned MSB_t, unsigned LSB_t>
class ArrayField<MSB_t, LSB_t, typename ArrayFieldRange<17, 32, LSB_t-MSB_t+1>::enabled> {
public:
  static constexpr unsigned WIDTH = LSB_t - MSB_t + 1;
  static const unsigned MSB = MSB_t;
  static const unsigned LSB = LSB_t;
  using DataType = uint32_t;

  ArrayField(const uint32_t& inValue = 0): mValue(inValue) {}
  uint32_t operator=(const uint32_t& inValue) { return mValue = inValue; }
  operator uint32_t() const { return mValue; }

private:
  uint32_t mValue;

};

// Range [33-64] specialization defines DataType to be uint64_t,
// and provides a member, constructor, assignment, and cast
// operators for that type
template <unsigned MSB_t, unsigned LSB_t>
class ArrayField<MSB_t, LSB_t, typename ArrayFieldRange<33, 64, LSB_t-MSB_t+1>::enabled> {
public:
  static constexpr unsigned WIDTH = LSB_t - MSB_t + 1;
  static const unsigned MSB = MSB_t;
  static const unsigned LSB = LSB_t;
  using DataType = uint64_t;

  ArrayField(const uint64_t& inValue = 0): mValue(inValue) {}
  uint64_t operator=(const uint64_t& inValue) { return mValue = inValue; }
  operator uint64_t() const { return mValue; }

private:
  uint64_t mValue;

};


#endif
