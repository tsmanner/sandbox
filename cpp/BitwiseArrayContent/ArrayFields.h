#ifndef ArrayFields_h
#define ArrayFields_h

#include <type_traits>

#include "ArrayField.h"
#include "ArraySubFields.h"


namespace ArrayFieldsUtility {

  //
  // Template Recursive MSB Calculation
  //

  template <unsigned CurrentMSB, typename CurrentField, typename... RemainingFields>
  constexpr typename std::enable_if<(sizeof...(RemainingFields) == 0), unsigned>::type
  _calculate_msb() {
    return (CurrentMSB <= CurrentField::MSB) ? CurrentMSB : CurrentField::MSB;
  }

  template <unsigned CurrentMSB, typename CurrentField, typename... RemainingFields>
  constexpr typename std::enable_if<(sizeof...(RemainingFields) != 0), unsigned>::type
  _calculate_msb() {
    return _calculate_msb<(CurrentMSB <= CurrentField::MSB) ? CurrentMSB : CurrentField::MSB, RemainingFields...>();
  }

  template <typename FirstField, typename... RemainingFields>
  constexpr unsigned calculate_msb() {
    return _calculate_msb<FirstField::MSB, RemainingFields...>();
  }

  //
  // Template Recursive LSB Calculation
  //

  template <unsigned CurrentLSB, typename CurrentField, typename... RemainingFields>
  constexpr typename std::enable_if<(sizeof...(RemainingFields) == 0), unsigned>::type
  _calculate_lsb() {
    return (CurrentLSB >= CurrentField::LSB) ? CurrentLSB : CurrentField::LSB;
  }

  template <unsigned CurrentLSB, typename CurrentField, typename... RemainingFields>
  constexpr typename std::enable_if<(sizeof...(RemainingFields) != 0), unsigned>::type
  _calculate_lsb() {
    return _calculate_lsb<(CurrentLSB >= CurrentField::LSB) ? CurrentLSB : CurrentField::LSB, RemainingFields...>();
  }

  template <typename FirstField, typename... RemainingFields>
  constexpr unsigned calculate_lsb() {
    return _calculate_lsb<FirstField::LSB, RemainingFields...>();
  }

}  // namespace ArrayFieldsUtility


class ArrayFieldsBase {};


template <typename... ArrayFieldTypes>
class ArrayFields :
  public ArrayFieldsBase,
  public ArrayField<
    ArrayFieldsUtility::calculate_msb<ArrayFieldTypes...>(),
    ArrayFieldsUtility::calculate_lsb<ArrayFieldTypes...>()
  >
{
public:

  using ArrayFieldType = ArrayField<
    ArrayFieldsUtility::calculate_msb<ArrayFieldTypes...>(),
    ArrayFieldsUtility::calculate_lsb<ArrayFieldTypes...>()
  >;

  static constexpr unsigned NumFields = sizeof...(ArrayFieldTypes);

  static const unsigned MSB = ArrayFieldType::MSB;
  static const unsigned LSB = ArrayFieldType::LSB;
  static const unsigned WIDTH = ArrayFieldType::WIDTH;
  using DataType = typename ArrayFieldType::DataType;


  //
  // Calculate Shift
  //   Looks up the field recursively by index, calculating
  //   the distance of this field's LSB from the LSB of
  //   the containing array.
  //

  // Terminal call in template recursion, mask is returned
  template <unsigned QueryIndex, unsigned CurrentIndex, typename CurrentField, typename... RemainingFields>
  static constexpr typename std::enable_if<(QueryIndex < NumFields and QueryIndex == CurrentIndex), DataType>::type
  _calculate_shift() {
    return LSB - CurrentField::LSB;
  }

  // Terminal call in template recursion, just keep looking
  template <unsigned QueryIndex, unsigned CurrentIndex, typename CurrentField, typename... RemainingFields>
  static constexpr typename std::enable_if<(QueryIndex < NumFields and QueryIndex != CurrentIndex), DataType>::type
  _calculate_shift() {
    return _calculate_shift<QueryIndex, CurrentIndex+1, RemainingFields...>();
  }

  // Entry poing function
  template <unsigned QueryIndex>
  static constexpr DataType calculate_shift() {
    return _calculate_shift<QueryIndex, 0, ArrayFieldTypes...>();
  }

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
    return (std::numeric_limits<DataType>::max() >> (std::numeric_limits<DataType>::digits - CurrentField::WIDTH)) << calculate_shift<QueryIndex>();
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

  // Terminal call in template recursion for DataType
  //    mask and shift are applied and returned
  template <typename ArgType, unsigned QueryIndex, unsigned CurrentIndex, typename CurrentField, typename... RemainingFields>
  static typename std::enable_if<(!std::is_base_of<ArrayFieldsBase, ArgType>::value and QueryIndex < NumFields and QueryIndex == CurrentIndex), DataType>::type
  _calculate_field(const ArgType& inData) {
    return (DataType(inData) << (LSB - CurrentField::LSB)) & calculate_mask<QueryIndex>();
  }

  // Terminal call in template recursion for non-DataType
  //    mask and shift are applied and returned
  template <typename ArgType, unsigned QueryIndex, unsigned CurrentIndex, typename CurrentField, typename... RemainingFields>
  static typename std::enable_if<(std::is_base_of<ArrayFieldsBase, ArgType>::value and QueryIndex < NumFields and QueryIndex == CurrentIndex), DataType>::type
  _calculate_field(const ArgType& inData) {
    return DataType(inData.getContent()) << (LSB - CurrentField::LSB);
  }

  // Non-terminal call in template recursion, just keep looking
  template <typename ArgType, unsigned QueryIndex, unsigned CurrentIndex, typename CurrentField, typename... RemainingFields>
  static typename std::enable_if<(QueryIndex < NumFields and QueryIndex != CurrentIndex), DataType>::type
  _calculate_field(const ArgType& inData) {
    return _calculate_field<ArgType, QueryIndex, CurrentIndex+1, RemainingFields...>(inData);
  }

  //
  // Set
  //   Recursively OR reduces the result of applying each
  //   field mask and shift to the input arguments, in
  //   order, by field index.
  //

  // Terminal call in template recursion, final field is returned
  template <unsigned I, typename CurrentArgType, typename... RemainingArgTypes>
  static typename std::enable_if<(sizeof...(RemainingArgTypes) == 0), DataType>::type
  _calculate_content(const CurrentArgType& inCurrentArg) {
    return _calculate_field<CurrentArgType, I, 0, ArrayFieldTypes...>(inCurrentArg);
  }

  // Non-terminal call in template recursion, intermediate field OR'd with
  // the next field is returned
  template <unsigned I, typename CurrentArgType, typename... RemainingArgTypes>
  static typename std::enable_if<(sizeof...(RemainingArgTypes) != 0), DataType>::type
  _calculate_content(const CurrentArgType& inCurrentArg, const RemainingArgTypes&... inRemainingArgs) {
    return _calculate_field<CurrentArgType, I, 0, ArrayFieldTypes...>(inCurrentArg) | _calculate_content<I+1, RemainingArgTypes...>(inRemainingArgs...);
  }

  // Entry point call to calculate content value
  template <typename... ArgTypes>
  static typename std::enable_if<(sizeof...(ArgTypes) != 0), DataType>::type
  calculate_content(const ArgTypes&... inArgs) {
    return _calculate_content<0, ArgTypes...>(inArgs...);
  }

  // Entry point function that initiates field setting
  // with FieldIndex of 0.  Requires exactly one argument
  // per Field. Calls into the recursive _calculate_content function.
  template <typename... ArgTypes>
  typename std::enable_if<(sizeof...(ArgTypes) == NumFields)>::type
  set(const ArgTypes&... inArgs) {
    mContent = calculate_content(inArgs...);
  }

  // Default Constructor
  ArrayFields() {}

  // Value Constructor
  //   set must be called with exactly one argument per field
  template <typename... ArgTypes>
  ArrayFields(const ArgTypes&... inValues) {
    set(inValues...);
  }

  // Get a field by Field Index
  template <unsigned QueryIndex>
  typename std::tuple_element<QueryIndex, std::tuple<ArrayFieldTypes...>>::type
  getField() const {
    typename std::tuple_element<QueryIndex, std::tuple<ArrayFieldTypes...>>::type field;
    field = (mContent & calculate_mask<QueryIndex>()) >> calculate_shift<QueryIndex>();
    return field;
  }

  // Set a field by Field Index
  template <unsigned QueryIndex>
  void setField(const DataType& inValue) {
    // Clear the range in mContent and set the new data
    mContent = (mContent & ~calculate_mask<QueryIndex>()) | (inValue << calculate_shift<QueryIndex>());
  }

  // Get the concatenated array content
  const DataType& getContent() const { return mContent; }
  DataType& getContent() { return mContent; }

  // Set content, wholesale
  void setContent(const DataType& inContent) { mContent = inContent; }

  // Assignment Operator - sets content wholesale
  DataType& operator=(const DataType& inContent) { this->setContent(inContent); return this->getContent(); }
  operator DataType() { return this->getContent(); }

private:
  DataType mContent { 0 };

};


#endif
