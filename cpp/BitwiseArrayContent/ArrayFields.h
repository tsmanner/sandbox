#ifndef ArrayFields_h
#define ArrayFields_h

#include <type_traits>

#include "ArrayField.h"


template <typename... ArrayFieldTypes>
class ArrayFields {
public:
  static constexpr unsigned NumFields = sizeof...(ArrayFieldTypes);

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

  // Provide MSB and LSB as const unsigned values
  static constexpr unsigned MSB = calculate_msb();
  static constexpr unsigned LSB = calculate_lsb();

  // Use a dummy ArrayField to figure out what size of
  // unsigned integer we should use to store the content in.
  using DataType = typename ArrayField<MSB, LSB>::DataType;

  //
  // Calculate Mask
  //   Looks up the field recursively by index, calculating
  //   the mask that covers the Field's bit range within
  //   the containing array.  Entry point is variadic so
  //   multiple masks can be OR reduced and returned as one.
  //

  // Terminal call in template recursion, mask is returned
  template <unsigned QueryIndex, unsigned CurrentIndex, typename CurrentField, typename... RemainingFields>
  static constexpr typename std::enable_if<(QueryIndex < NumFields and QueryIndex == CurrentIndex), DataType>::type
  _calculate_mask() {
    return (std::numeric_limits<DataType>::max() >> (std::numeric_limits<DataType>::digits - CurrentField::WIDTH)) << (LSB - CurrentField::LSB);
  }

  // Terminal call in template recursion, just keep looking
  template <unsigned QueryIndex, unsigned CurrentIndex, typename CurrentField, typename... RemainingFields>
  static constexpr typename std::enable_if<(QueryIndex < NumFields and QueryIndex != CurrentIndex), DataType>::type
  _calculate_mask() {
    return _calculate_mask<QueryIndex, CurrentIndex+1, RemainingFields...>();
  }

  // Terminal call in template recursion for the OR reducing front end.
  // Calls _calculate_mask for the final field
  template <unsigned QueryIndex, unsigned... QueryIndices>
  static constexpr typename std::enable_if<(sizeof...(QueryIndices) == 0), DataType>::type
  calculate_mask() {
    return _calculate_mask<QueryIndex, 0, ArrayFieldTypes...>();
  }

  // Non-terminal call in template recursion for the OR reducing front end.
  // Calls _calculate_mask for the current field and ORs it with a recursive
  // call to calculate_mask for the remaining Field Indices.
  template <unsigned QueryIndex, unsigned... QueryIndices>
  static constexpr typename std::enable_if<(sizeof...(QueryIndices) != 0), DataType>::type
  calculate_mask() {
    return _calculate_mask<QueryIndex, 0, ArrayFieldTypes...>() | calculate_mask<QueryIndices...>();
  }

  //
  // Set Field
  //   Looks up the field recursively by index, shifting
  //   and masking the data into the right set of bits.
  //

  // Terminal call in template recursion, mask and shift are applied and returned
  template <unsigned QueryIndex, unsigned CurrentIndex, typename CurrentField, typename... RemainingFields>
  typename std::enable_if<(QueryIndex < NumFields and QueryIndex == CurrentIndex), DataType>::type
  _set_field(const DataType& inData) {
    return (inData << (LSB - CurrentField::LSB)) & calculate_mask<QueryIndex>();
  }

  // Non-terminal call in template recursion, just keep looking
  template <unsigned QueryIndex, unsigned CurrentIndex, typename CurrentField, typename... RemainingFields>
  typename std::enable_if<(QueryIndex < NumFields and QueryIndex != CurrentIndex), DataType>::type
  _set_field(const DataType& inData) {
    return _set_field<QueryIndex, CurrentIndex+1, RemainingFields...>(inData);
  }

  //
  // Set
  //   Recursively OR reduces the result of applying each
  //   field mask and shift to the input arguments, in
  //   order, by field index.
  //

  // Terminal call in template recursion, final field is returned
  template <unsigned I, typename CurrentArgType, typename... RemainingArgTypes>
  typename std::enable_if<(sizeof...(RemainingArgTypes) == 0), DataType>::type
  _set(const CurrentArgType& inCurrentArg) {
    return _set_field<I, 0, ArrayFieldTypes...>(inCurrentArg);
  }

  // Non-terminal call in template recursion, intermediate field OR'd with
  // the next field is returned
  template <unsigned I, typename CurrentArgType, typename... RemainingArgTypes>
  typename std::enable_if<(sizeof...(RemainingArgTypes) != 0), DataType>::type
  _set(const CurrentArgType& inCurrentArg, const RemainingArgTypes&... inRemainingArgs) {
    return _set_field<I, 0, ArrayFieldTypes...>(inCurrentArg) | _set<I+1, RemainingArgTypes...>(inRemainingArgs...);
  }

  // Entry point function that initiates field setting
  // with FieldIndex of 0.  Requires exactly one argument
  // per Field. Calls into the recursive _set function.
  template <typename... ArgTypes>
  typename std::enable_if<(sizeof...(ArgTypes) == NumFields)>::type
  set(const ArgTypes&... inArgs) {
    mContent = _set<0, ArgTypes...>(inArgs...);
  }

  // Update the content
  //   set must be called with exactly one argument per field
  ArrayFields(const typename ArrayFieldTypes::DataType&... inValues) {
    set(inValues...);
  }

  // Get the concatenated array content
  const DataType& getContent() const { return mContent; }

private:
  DataType mContent;

};


#endif
