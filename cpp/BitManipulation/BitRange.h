#ifndef BitRange_h
#define BitRange_h


template <int MSB, int LSB, typename... Scrambles>
class BitRange {
public:

  //
  // Index Query
  //   Implemented with a recursive template that cascades
  //   an index query through all scrambles in Scrambles
  //   in order.
  //

  // Terminal call in template recursion.  Only called on the last scramble,
  // returning the index query for it.
  template <int INDEX, typename CurrentScramble, typename... RemainingScrambles>
  static constexpr typename std::enable_if<(sizeof...(RemainingScrambles) == 0), int>::type
  _query() {
    return CurrentScramble::template query<INDEX>();
  }

  // Recursive call that will forward the query from the current scramble
  // to the next one.  Enabled for all but the final scramble.
  template <int INDEX, typename CurrentScramble, typename... RemainingScrambles>
  static constexpr typename std::enable_if<(sizeof...(RemainingScrambles) != 0), int>::type
  _query() {
    return _query<CurrentScramble::template query<INDEX>(), RemainingScrambles...>();
  }

  // Entry point to query, sugar to make the front-end cleaner
  // by not requiring users to list the scrambles again.
  template <int INDEX>
  static constexpr int query() {
    return _query<INDEX, Scrambles...>();
  }

  //
  // Mask calculation for the bit described by index
  //
  template <typename DataType, int INDEX>
  static constexpr int calculate_mask() {
    return DataType(1) << (LSB - INDEX - 2*MSB);
  }

  //
  // Shift calculation for the bit described by index
  //   Calls recursive query function to get the final
  //   bit position.
  //
  template <int INDEX>
  static constexpr int calculate_shift() {
    return query<INDEX>() - INDEX;
  }

  //
  // Perform a right shift, allowing for negative distances
  //

  // Negative shift - shift left by the absolute value of I
  template <typename DataType, int I>
  static constexpr typename std::enable_if<(I < 0), DataType>::type
  shift(const DataType& data) {
    return data << (0 - I);
  }

  // Positive shift - shift right by I
  template <typename DataType, int I>
  static constexpr typename std::enable_if<(I > 0), DataType>::type
  shift(const DataType& data) {
    return data >> I;
  }

  // No shift - just return the data
  template <typename DataType, int I>
  static constexpr typename std::enable_if<(I == 0), DataType>::type
  shift(const DataType& data) {
    return data;
  }

  //
  // Apply
  //   Applies the scrambles to an actual data value by
  //   recursively iterating over each bit and cascading
  //   a mask, shift, and bitwise OR.
  //

  // Terminal call in template recursion.  The final bit is returned.
  template <typename DataType, int I>
  static constexpr typename std::enable_if<(I == LSB), DataType>::type
  _apply(const DataType& data) {
    return shift<DataType, calculate_shift<I>()>(data & calculate_mask<DataType, I>());
  }

  // Recursive call that bitwise ORs this bit with the next one
  template <typename DataType, int I=MSB>
  static constexpr typename std::enable_if<(I < LSB), DataType>::type
  _apply(const DataType& data) {
    return (shift<DataType, calculate_shift<I>()>(data & calculate_mask<DataType, I>())) | _apply<DataType, I+1>(data);
  }

  // Entry point function that can use template deduction to allow
  // users to provide data without explicitely providing the data
  // type.  Calls into the recursive _apply function.
  template <typename DataType>
  static constexpr typename std::enable_if<(sizeof...(Scrambles) != 0), DataType>::type
  apply(const DataType& data) {
    return _apply<DataType>(data);
  }

  // Entry point function enabled if there are no scrambles to apply,
  // that just returns the data without modification.
  template <typename DataType>
  static constexpr typename std::enable_if<(sizeof...(Scrambles) == 0), DataType>::type
  apply(const DataType& data) {
    return data;
  }

};

#endif
