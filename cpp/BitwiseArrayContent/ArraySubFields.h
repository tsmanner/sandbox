#ifndef ArraySubFields_h
#define ArraySubFields_h

#include <type_traits>

#include "ArrayBaseClasses.h"



template <unsigned MSB_t, unsigned LSB_t, typename ArrayFieldsType_t>
class ArraySubFields : public ArraySubFieldsBase, public ArrayFieldsType_t {
public:
  using ArrayFieldsType = ArrayFieldsType_t;
  using DataType = typename ArrayFieldsType::DataType;
  static const unsigned NumFields = ArrayFieldsType::NumFields;
  static const unsigned MSB = MSB_t;
  static const unsigned LSB = LSB_t;
  static const unsigned WIDTH = ArrayFieldsType::WIDTH;

  // Assignment Operator - sets content wholesale
  DataType& operator=(const DataType& inContent) { this->ArrayFieldsType::setContent(inContent); return this->ArrayFieldsType::getContent(); }
  operator DataType() { return this->ArrayFieldsType::getContent(); }

};


#endif
