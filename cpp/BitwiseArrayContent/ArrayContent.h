#ifndef ArrayContent_h
#define ArrayContent_h

#include <type_traits>

#include "ArrayBaseClasses.h"


template <typename ArrayFieldsType_t, typename... ScrambleTypes>
class ArrayContent : public ArrayContentBase, public ArrayFieldsType_t {
public:
  using ArrayFieldsType = ArrayFieldsType_t;
  using DataType = typename ArrayFieldsType::DataType;
  static const unsigned NumFields = ArrayFieldsType::NumFields;
  static constexpr unsigned NumScrambles = sizeof...(ScrambleTypes);
  static const DataType MSB = ArrayFieldsType::MSB;
  static const DataType LSB = ArrayFieldsType::LSB;
  static const unsigned WIDTH = ArrayFieldsType::WIDTH;

  //
  // Index Query
  //   Implemented with a recursive template that cascades
  //   an index query through all ScrambleTypes in ScrambleTypes
  //   in order.
  //

  // Terminal call in template recursion.  Only called on the last scramble,
  // returning the index query for it.
  template <unsigned INDEX, typename CurrentScramble, typename... RemainingScrambleTypes>
  static constexpr typename std::enable_if<(
    sizeof...(RemainingScrambleTypes) == 0
  ), unsigned>::type
  _query() {
    return CurrentScramble::template query<ArrayFieldsType::template query<INDEX>()>();
  }

  // Recursive call that will forward the query from the current scramble
  // to the next one.  Enabled for all but the final scramble.
  template <unsigned INDEX, typename CurrentScramble, typename... RemainingScrambleTypes>
  static constexpr typename std::enable_if<(
    sizeof...(RemainingScrambleTypes) != 0
  ), unsigned>::type
  _query() {
    return _query<CurrentScramble::template query<ArrayFieldsType::template query<INDEX>()>(), RemainingScrambleTypes...>();
  }

  // No scrambles at all, just query the field
  template <unsigned INDEX>
  static constexpr unsigned _query() {
    return ArrayFieldsType::template query<INDEX>();
  }

  // Call for a bit that lies within a Field that is an ArrayContent.
  // Forwards the query to the ArrayContent first, then applies the current
  // scrambling


  // Entry point to query, sugar to make the front-end cleaner
  // by not requiring users to list the ScrambleTypes again.
  template <unsigned INDEX>
  static constexpr unsigned query() {
    return _query<INDEX, ScrambleTypes...>();
  }

  //
  // Mask calculation for the bit described by index
  //
  template <DataType INDEX>
  static constexpr DataType calculate_bit_mask() {
    return DataType(1) << (LSB - INDEX - (2 * MSB));
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

  // Negative shift - shift left by the absolute value of SHIFT
  template <int SHIFT>
  static constexpr typename std::enable_if<(SHIFT < 0), DataType>::type
  shift_bits(const DataType& data) {
    return data << (0 - SHIFT);
  }

  // Positive shift - shift right by SHIFT
  template <int SHIFT>
  static constexpr typename std::enable_if<(SHIFT > 0), DataType>::type
  shift_bits(const DataType& data) {
    return data >> SHIFT;
  }

  // No shift - just return the data
  template <int SHIFT>
  static constexpr typename std::enable_if<(SHIFT == 0), DataType>::type
  shift_bits(const DataType& data) {
    return data;
  }

  //
  // scramble
  //   Applies the ScrambleTypes to an actual data value by
  //   recursively iterating over each bit and cascading
  //   a mask, shift, and bitwise OR.
  //

  // Terminal call in template recursion.  The final bit is returned.
  template <DataType INDEX>
  static constexpr typename std::enable_if<(INDEX == LSB), DataType>::type
  _scramble(const DataType& data) {
    return shift_bits<calculate_shift<INDEX>()>(data & calculate_bit_mask<INDEX>());
  }

  // Recursive call that bitwise ORs this bit with the next one
  template <DataType INDEX=MSB>
  static constexpr typename std::enable_if<(INDEX < LSB), DataType>::type
  _scramble(const DataType& data) {
    return (shift_bits<calculate_shift<INDEX>()>(data & calculate_bit_mask<INDEX>())) | _scramble<INDEX+1>(data);
  }

  // Entry point function that can use template deduction to allow
  // users to provide data without explicitly providing the data
  // type.  Calls into the recursive _scramble function.
  template <typename ReturnType = DataType>
  static constexpr typename std::enable_if<(sizeof...(ScrambleTypes) != 0), ReturnType>::type
  scramble(const DataType& data) {
    return _scramble(data);
  }

  // Entry point function enabled if there are no ScrambleTypes to scramble,
  // that just returns the data without modification.
  template <typename ReturnType = DataType>
  static constexpr typename std::enable_if<(sizeof...(ScrambleTypes) == 0), ReturnType>::type
  scramble(const DataType& data) {
    return data;
  }

  //
  // unscramble
  //   Applies the ScrambleTypes to an actual data value,
  //   in reverse, by recursively iterating over each bit
  //   and cascading a mask, shift, and bitwise OR.
  //

  // Terminal call in template recursion.  The final bit is returned.
  template <DataType INDEX>
  static constexpr typename std::enable_if<(INDEX == LSB), DataType>::type
  _unscramble(const DataType& data) {
    return shift_bits<0-calculate_shift<INDEX>()>(data & calculate_bit_mask<query<INDEX>()>());
  }

  // Recursive call that bitwise ORs this bit with the next one
  template <DataType INDEX=MSB>
  static constexpr typename std::enable_if<(INDEX < LSB), DataType>::type
  _unscramble(const DataType& data) {
    return (shift_bits<0-calculate_shift<INDEX>()>(data & calculate_bit_mask<query<INDEX>()>())) | _unscramble<INDEX+1>(data);
  }

  // Entry point function that can use template deduction to allow
  // users to provide data without explicitly providing the data
  // type.  Calls into the recursive _scramble function.
  template <typename ReturnType = DataType>
  static constexpr typename std::enable_if<(sizeof...(ScrambleTypes) != 0), ReturnType>::type
  unscramble(const DataType& data) {
    return _unscramble(data);
  }

  // Entry point function enabled if there are no ScrambleTypes to scramble,
  // that just returns the data without modification.
  template <typename ReturnType = DataType>
  static constexpr typename std::enable_if<(sizeof...(ScrambleTypes) == 0), ReturnType>::type
  unscramble(const DataType& data) {
    return data;
  }

  // Default Constructor
  ArrayContent(): ArrayFieldsType() {}

  // Variadic Constructor to set all values
  template <typename... ArgTypes>
  ArrayContent(
    const ArgTypes&... inArgs
  ):
    ArrayFieldsType(inArgs...)
  {
  }

  // Calculate and return the scrambled content
  virtual DataType getScrambledContent() const { return scramble(this->getContent()); }

  // Assignment Operator - sets content wholesale
  DataType& operator=(const DataType& inContent) { this->setContent(inContent); return this->getContent(); }

};



template <typename ArrayFieldsType_t, typename... ScrambleTypes>
class BufferedArrayContent : public ArrayContent<ArrayFieldsType_t, ScrambleTypes...> {
public:
  using ArrayFieldsType = ArrayFieldsType_t;
  using DataType = typename ArrayFieldsType::DataType;
  static const unsigned MSB = ArrayFieldsType::MSB;
  static const unsigned LSB = ArrayFieldsType::LSB;

  // Default Constructor
  BufferedArrayContent(): ArrayContent<ArrayFieldsType_t, ScrambleTypes...>() {}

  // Variadic Constructor to set all values
  template <typename... ArgTypes>
  BufferedArrayContent(
    const ArgTypes&... inArgs
  ):
    ArrayContent<ArrayFieldsType_t, ScrambleTypes...>(inArgs...),
    mScrambledContent(this->scramble(this->getContent()))
  {
  }

  // Update the unscrambled and scrambled content
  //   set must be called with exactly one argument per field
  template <typename... ArgTypes>
  typename std::enable_if<(sizeof...(ArgTypes) == ArrayFieldsType::NumFields)>::type
  set(const ArgTypes&... inArgs) {
    ArrayFieldsType::set(inArgs...);
    mScrambledContent = this->scramble(this->getContent());
  }

  // Set a field by Field Index and update scrambled content
  template <unsigned QueryIndex>
  void setField(const DataType& inValue) {
    ArrayFieldsType::template setField<QueryIndex>(inValue);
    mScrambledContent = this->scramble(this->getContent());
  }

  // Set content, wholesale
  void setContent(const DataType& inContent) {
    ArrayFieldsType::setContent(inContent);
    mScrambledContent = this->scramble(this->getContent());
  }

  // Get the buffered scrambled content
  virtual DataType getScrambledContent() const { return mScrambledContent; }

  // Assignment Operator - sets content wholesale
  DataType& operator=(const DataType& inContent) { this->setContent(inContent); return this->getContent(); }

private:
  DataType mScrambledContent { 0 };

};


#endif
