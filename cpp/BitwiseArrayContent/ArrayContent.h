#ifndef ArrayContent_h
#define ArrayContent_h

#include <type_traits>


template <typename ArrayFieldsType_t, typename... ScrambleTypes>
class ArrayContent : public ArrayFieldsType_t {
public:
  using ArrayFieldsType = ArrayFieldsType_t;
  using DataType = typename ArrayFieldsType::DataType;
  static const unsigned MSB = ArrayFieldsType::MSB;
  static const unsigned LSB = ArrayFieldsType::LSB;

  //
  // Index Query
  //   Implemented with a recursive template that cascades
  //   an index query through all ScrambleTypes in ScrambleTypes
  //   in order.
  //

  // Terminal call in template recursion.  Only called on the last scramble,
  // returning the index query for it.
  template <int INDEX, typename CurrentScramble, typename... RemainingScrambleTypes>
  static constexpr typename std::enable_if<(sizeof...(RemainingScrambleTypes) == 0), int>::type
  _query() {
    return CurrentScramble::template query<INDEX>();
  }

  // Recursive call that will forward the query from the current scramble
  // to the next one.  Enabled for all but the final scramble.
  template <int INDEX, typename CurrentScramble, typename... RemainingScrambleTypes>
  static constexpr typename std::enable_if<(sizeof...(RemainingScrambleTypes) != 0), int>::type
  _query() {
    return _query<CurrentScramble::template query<INDEX>(), RemainingScrambleTypes...>();
  }

  // Entry point to query, sugar to make the front-end cleaner
  // by not requiring users to list the ScrambleTypes again.
  template <int INDEX>
  static constexpr int query() {
    return _query<INDEX, ScrambleTypes...>();
  }

  //
  // Mask calculation for the bit described by index
  //
  template <int INDEX>
  static constexpr int calculate_bit_mask() {
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
  template <int I>
  static constexpr typename std::enable_if<(I < 0), DataType>::type
  shift_bits(const DataType& data) {
    return data << (0 - I);
  }

  // Positive shift - shift right by I
  template <int I>
  static constexpr typename std::enable_if<(I > 0), DataType>::type
  shift_bits(const DataType& data) {
    return data >> I;
  }

  // No shift - just return the data
  template <int I>
  static constexpr typename std::enable_if<(I == 0), DataType>::type
  shift_bits(const DataType& data) {
    return data;
  }

  //
  // Apply
  //   Applies the ScrambleTypes to an actual data value by
  //   recursively iterating over each bit and cascading
  //   a mask, shift, and bitwise OR.
  //

  // Terminal call in template recursion.  The final bit is returned.
  template <int I>
  static constexpr typename std::enable_if<(I == LSB), DataType>::type
  _apply(const DataType& data) {
    return shift_bits<calculate_shift<I>()>(data & calculate_bit_mask<I>());
  }

  // Recursive call that bitwise ORs this bit with the next one
  template <int I=MSB>
  static constexpr typename std::enable_if<(I < LSB), DataType>::type
  _apply(const DataType& data) {
    return (shift_bits<calculate_shift<I>()>(data & calculate_bit_mask<I>())) | _apply<I+1>(data);
  }

  // Entry point function that can use template deduction to allow
  // users to provide data without explicitly providing the data
  // type.  Calls into the recursive _apply function.
  template <typename ReturnType = DataType>
  static constexpr typename std::enable_if<(sizeof...(ScrambleTypes) != 0), ReturnType>::type
  apply(const DataType& data) {
    return _apply(data);
  }

  // Entry point function enabled if there are no ScrambleTypes to apply,
  // that just returns the data without modification.
  template <typename ReturnType = DataType>
  static constexpr typename std::enable_if<(sizeof...(ScrambleTypes) == 0), ReturnType>::type
  apply(const DataType& data) {
    return data;
  }

  // Default Constructor
  ArrayContent(): ArrayFieldsType() {}

  // Variadic Constructor to set all values
  template <typename... ArgTypes>
  ArrayContent(
    const ArgTypes&... inArgs
  ):
    ArrayFieldsType(inArgs...),
    mScrambledContent(apply(this->getContent()))
  {
  }

  // Update the unscrambled and scrambled content
  //   set must be called with exactly one argument per field
  template <typename... ArgTypes>
  typename std::enable_if<(sizeof...(ArgTypes) == ArrayFieldsType::NumFields)>::type
  set(const ArgTypes&... inArgs) {
    ArrayFieldsType::set(inArgs...);
    mScrambledContent = apply(this->getContent());
  }

  // Set a field by Field Index and update scrambled content
  template <unsigned QueryIndex>
  void setField(const DataType& inValue) {
    ArrayFieldsType::template setField<QueryIndex>(inValue);
    mScrambledContent = apply(this->getContent());
  }

  // Get the scrambled content
  const DataType& getScrambledContent() const { return mScrambledContent; }

private:
  DataType mScrambledContent { 0 };

};


#endif
