#ifndef ArrayField_h
#define ArrayField_h

#include <type_traits>


template<int Start, int End, int Val, class Enable = void> struct ArrayFieldRange {};

template<int Start, int End, int Val>
struct ArrayFieldRange<Start, End, Val, typename std::enable_if<Val >= Start && Val <= End>::type> {
  using enabled = void;
};

template <unsigned MSB_t, unsigned LSB_t, class Enable = void>
class ArrayField {
public:
  static constexpr unsigned WIDTH = LSB_t - MSB_t + 1;
  static const unsigned MSB = MSB_t;
  static const unsigned LSB = LSB_t;
};

template <unsigned MSB_t, unsigned LSB_t>
class ArrayField<MSB_t, LSB_t, typename ArrayFieldRange<0, 8, LSB_t-MSB_t+1>::enabled> {
public:
  static constexpr unsigned WIDTH = LSB_t - MSB_t + 1;
  static const unsigned MSB = MSB_t;
  static const unsigned LSB = LSB_t;
  using DataType = uint8_t;

  ArrayField(const uint8_t& inValue = 0): mValue(inValue) {}
  uint8_t operator=(const uint8_t& inValue) { return mValue = inValue; }
  operator uint8_t() { return mValue; }

private:
  uint8_t mValue;
};

template <unsigned MSB_t, unsigned LSB_t>
class ArrayField<MSB_t, LSB_t, typename ArrayFieldRange<9, 16, LSB_t-MSB_t+1>::enabled> {
public:
  static constexpr unsigned WIDTH = LSB_t - MSB_t + 1;
  static const unsigned MSB = MSB_t;
  static const unsigned LSB = LSB_t;
  using DataType = uint16_t;

  ArrayField(const uint16_t& inValue = 0): mValue(inValue) {}
  uint16_t operator=(const uint16_t& inValue) { return mValue = inValue; }
  operator uint16_t() { return mValue; }

private:
  uint16_t mValue;

};


template <unsigned MSB_t, unsigned LSB_t>
class ArrayField<MSB_t, LSB_t, typename ArrayFieldRange<17, 32, LSB_t-MSB_t+1>::enabled> {
public:
  static constexpr unsigned WIDTH = LSB_t - MSB_t + 1;
  static const unsigned MSB = MSB_t;
  static const unsigned LSB = LSB_t;
  using DataType = uint32_t;

  ArrayField(const uint32_t& inValue = 0): mValue(inValue) {}
  uint32_t operator=(const uint32_t& inValue) { return mValue = inValue; }
  operator uint32_t() { return mValue; }

private:
  uint32_t mValue;

};


template <unsigned MSB_t, unsigned LSB_t>
class ArrayField<MSB_t, LSB_t, typename ArrayFieldRange<33, 64, LSB_t-MSB_t+1>::enabled> {
public:
  static constexpr unsigned WIDTH = LSB_t - MSB_t + 1;
  static const unsigned MSB = MSB_t;
  static const unsigned LSB = LSB_t;
  using DataType = uint64_t;

  ArrayField(const uint64_t& inValue = 0): mValue(inValue) {}
  uint64_t operator=(const uint64_t& inValue) { return mValue = inValue; }
  operator uint64_t() { return mValue; }

private:
  uint64_t mValue;

};


#endif
