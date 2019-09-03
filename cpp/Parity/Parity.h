#include <math.h>


// TODO: Use the ParityWidth to mask/shift parity bits and control recursion depth
//       so that a user can calculate parity over multiple equally sized Parity
//       groups at once.
//       i.e. Parity<16>::calc(uint16_t(0xfeff))     ->  0b1
//       i.e. Parity<16, 2>::calc(uint16_t(0xfeff))  ->  0b10
template <unsigned DataWidth, unsigned ParityWidth = 1, unsigned DataShift = 1>
struct Parity {
  // Unsigned precision can be calculated as 1 plus the log base 2 of -1
  template <typename UnsignedType>
  static constexpr typename std::enable_if<std::is_unsigned<UnsignedType>::value, unsigned>::type
  precision() { return std::log2(UnsignedType(-1)) + 1; }

  // Masking calc implementation
  //   Input data needs to be masked if the precision of the data type exceeds the
  //   number of bits parity is being calculated over.
  template <typename DataType>
  static inline typename std::enable_if<(DataWidth < precision<DataType>()), DataType>::type
  calc(const DataType& data) {
    static constexpr DataType mask = DataType(~(DataType(-1) << DataWidth));
    return _calc(data & mask) & 1;
  }

  // Non-masking calc implementation
  //   Input data does not need to be masked if the precision is equal to the
  //   number of bits parity is being calculated over.
  template <typename DataType>
  static inline typename std::enable_if<(DataWidth == precision<DataType>()), DataType>::type
  calc(const DataType& data) {
    return _calc(data) & 1;
  }

  // Calculate each step of the parity with a cascading XOR, accumulating
  // partial parity bits in every 2^Nth bit, until 2^N > DataWidth.
  template <typename DataType>
  static inline typename std::enable_if<(DataWidth > DataShift), DataType>::type
  _calc(const DataType& data) {
    return Parity<DataWidth, ParityWidth, DataShift * 2>::_calc(data ^ (data >> DataShift));
  }

  // End template recursion
  template <typename DataType>
  static inline typename std::enable_if<(DataWidth <= DataShift), DataType>::type
  _calc(const DataType& data) { return data; }

};
