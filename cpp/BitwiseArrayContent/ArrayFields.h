#ifndef ArrayFields_h
#define ArrayFields_h

#include <type_traits>

#include "ArrayField.h"


template <typename... ArrayFieldTypes>
class ArrayFields {
public:

  //
  // Template Recursive MSB Calculation
  //

  template <unsigned CurrentMSB, typename CurrentField, typename... RemainingFields>
  static constexpr typename std::enable_if<(sizeof...(RemainingFields) == 0), unsigned>::type
  _calculate_msb() {
    return (CurrentMSB <= CurrentField::MSB) ? CurrentMSB : CurrentField::MSB;
  }

  template <unsigned CurrentMSB, typename CurrentField, typename... RemainingFields>
  static constexpr typename std::enable_if<(sizeof...(RemainingFields) != 0), unsigned>::type
  _calculate_msb() {
    return _calculate_msb<(CurrentMSB <= CurrentField::MSB) ? CurrentMSB : CurrentField::MSB, RemainingFields...>();
  }

  template <typename FirstField, typename... RemainingFields>
  static constexpr unsigned _calculate_msb_entry_point() {
    return _calculate_msb<FirstField::MSB, RemainingFields...>();
  }

  static constexpr unsigned calculate_msb() {
    return _calculate_msb_entry_point<ArrayFieldTypes...>();
  }

  //
  // Template Recursive LSB Calculation
  //

  template <unsigned CurrentLSB, typename CurrentField, typename... RemainingFields>
  static constexpr typename std::enable_if<(sizeof...(RemainingFields) == 0), unsigned>::type
  _calculate_lsb() {
    return (CurrentLSB >= CurrentField::LSB) ? CurrentLSB : CurrentField::LSB;
  }

  template <unsigned CurrentLSB, typename CurrentField, typename... RemainingFields>
  static constexpr typename std::enable_if<(sizeof...(RemainingFields) != 0), unsigned>::type
  _calculate_lsb() {
    return _calculate_lsb<(CurrentLSB >= CurrentField::LSB) ? CurrentLSB : CurrentField::LSB, RemainingFields...>();
  }

  template <typename FirstField, typename... RemainingFields>
  static constexpr unsigned _calculate_lsb_entry_point() {
    return _calculate_lsb<FirstField::LSB, RemainingFields...>();
  }

  static constexpr unsigned calculate_lsb() {
    return _calculate_lsb_entry_point<ArrayFieldTypes...>();
  }

  static constexpr unsigned MSB = calculate_msb();
  static constexpr unsigned LSB = calculate_lsb();

  using DataType = typename ArrayField<MSB, LSB>::DataType;


  template <unsigned QueryIndex, unsigned CurrentIndex, typename CurrentField, typename... RemainingFields>
  static constexpr typename std::enable_if<(QueryIndex < sizeof...(ArrayFieldTypes) and QueryIndex == CurrentIndex), DataType>::type
  _calculate_mask() {
    return (std::numeric_limits<DataType>::max() >> (std::numeric_limits<DataType>::digits - CurrentField::WIDTH)) << (LSB - CurrentField::LSB);
  }

  template <unsigned QueryIndex, unsigned CurrentIndex, typename CurrentField, typename... RemainingFields>
  static constexpr typename std::enable_if<(QueryIndex < sizeof...(ArrayFieldTypes) and QueryIndex != CurrentIndex), DataType>::type
  _calculate_mask() {
    return _calculate_mask<QueryIndex, CurrentIndex+1, RemainingFields...>();
  }

  template <unsigned QueryIndex, unsigned... QueryIndices>
  static constexpr typename std::enable_if<(sizeof...(QueryIndices) == 0), DataType>::type
  calculate_mask() {
    return _calculate_mask<QueryIndex, 0, ArrayFieldTypes...>();
  }

  template <unsigned QueryIndex, unsigned... QueryIndices>
  static constexpr typename std::enable_if<(sizeof...(QueryIndices) != 0), DataType>::type
  calculate_mask() {
    return _calculate_mask<QueryIndex, 0, ArrayFieldTypes...>() | calculate_mask<QueryIndices...>();
  }


  template <unsigned QueryIndex, unsigned CurrentIndex, typename CurrentField, typename... RemainingFields>
  typename std::enable_if<(QueryIndex < sizeof...(ArrayFieldTypes) and QueryIndex == CurrentIndex), DataType>::type
  _set_field(const DataType& inData) {
    return (inData << (LSB - CurrentField::LSB)) & calculate_mask<QueryIndex>();
  }

  template <unsigned QueryIndex, unsigned CurrentIndex, typename CurrentField, typename... RemainingFields>
  typename std::enable_if<(QueryIndex < sizeof...(ArrayFieldTypes) and QueryIndex != CurrentIndex), DataType>::type
  _set_field(const DataType& inData) {
    return _set_field<QueryIndex, CurrentIndex+1, RemainingFields...>(inData);
  }

  template <unsigned I, typename CurrentArgType, typename... RemainingArgTypes>
  typename std::enable_if<(sizeof...(RemainingArgTypes) == 0), DataType>::type
  _set(const CurrentArgType& inCurrentArg) {
    return _set_field<I, 0, ArrayFieldTypes...>(inCurrentArg);
  }

  template <unsigned I, typename CurrentArgType, typename... RemainingArgTypes>
  typename std::enable_if<(sizeof...(RemainingArgTypes) != 0), DataType>::type
  _set(const CurrentArgType& inCurrentArg, const RemainingArgTypes&... inRemainingArgs) {
    return _set_field<I, 0, ArrayFieldTypes...>(inCurrentArg) | _set<I+1, RemainingArgTypes...>(inRemainingArgs...);
  }

  template <typename... ArgTypes>
  typename std::enable_if<(sizeof...(ArgTypes) == sizeof...(ArrayFieldTypes))>::type
  set(const ArgTypes&... inArgs) {
    mContent = _set<0, ArgTypes...>(inArgs...);
  }

  ArrayFields(const typename ArrayFieldTypes::DataType&... inValues) {
    set(inValues...);
  }

  const DataType& getContent() const { return mContent; }

private:
  DataType mContent;

};


#endif
