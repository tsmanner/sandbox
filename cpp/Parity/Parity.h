#include <limits>
#include <math.h>


// To use `enable_if` with a struct, we have to fudge a partial specialization of
// the template.
template <unsigned DataWidth, unsigned ParityWidth = 1, unsigned DataShift = 1, typename EnableType = void>
struct Parity;

// Partial specialization of Parity that enforces ParityWidth being a factor of DataWidth
template <unsigned DataWidth, unsigned ParityWidth, unsigned DataShift>
struct Parity<DataWidth, ParityWidth, DataShift, typename std::enable_if<(DataWidth % ParityWidth == 0)>::type> {
  // static const ParityGroupSize calculation as DataType
  template <typename DataType> struct ParityGroupSize {
    static const DataType value = DataWidth / ParityWidth;
  };

  // static const DataMask calculation as DataType
  template <typename DataType> struct DataMask {
    static const DataType value = std::numeric_limits<DataType>::max() >> (std::numeric_limits<DataType>::digits - DataWidth);
  };

  // static const ParityMask calculation as DataType
  template <typename DataType> struct ParityMask {
    static const DataType value = std::numeric_limits<DataType>::max() >> (std::numeric_limits<DataType>::digits - ParityWidth);
  };

  // Masking calc implementation
  //   Input data needs to be masked if the precision of the data type exceeds the
  //   number of bits parity is being calculated over.
  template <typename DataType>
  static inline typename std::enable_if<(ParityWidth != 1), DataType>::type
  calc(const DataType& data) {
    return (
      (Parity<ParityGroupSize<DataType>::value, ParityWidth-1>::calc(data >> ParityGroupSize<DataType>::value) << 1) |
       Parity<ParityGroupSize<DataType>::value>::calc(data)
    ) & ParityMask<DataType>::value;
  }

  template <typename DataType>
  static inline typename std::enable_if<(DataWidth < std::numeric_limits<DataType>::digits and ParityWidth == 1), DataType>::type
  calc(const DataType& data) {
    return _calc(data & DataMask<DataType>::value) & 1;
  }

  // Non-masking calc implementation
  //   Input data does not need to be masked if the precision is equal to the
  //   number of bits parity is being calculated over.
  template <typename DataType>
  static inline typename std::enable_if<(DataWidth == std::numeric_limits<DataType>::digits and ParityWidth == 1), DataType>::type
  calc(const DataType& data) {
    return _calc(data) & 1;
  }

  // Calculate each step of the parity with a cascading XOR, accumulating
  // partial parity bits in every 2^Nth bit, until 2^N > DataWidth.
  template <typename DataType>
  static inline typename std::enable_if<((DataWidth / ParityWidth) > DataShift), DataType>::type
  _calc(const DataType& data) {
    return Parity<DataWidth, ParityWidth, DataShift * 2>::_calc(data ^ (data >> DataShift));
  }

  // End template recursion
  template <typename DataType>
  static inline typename std::enable_if<((DataWidth / ParityWidth) <= DataShift), DataType>::type
  _calc(const DataType& data) { return data; }

};
