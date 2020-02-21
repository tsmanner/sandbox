#ifndef Array_Reduce_h
#define Array_Reduce_h

#include <type_traits>


template <typename ArrayType, unsigned I, typename FunctionType, typename... ArgTypes>
typename std::enable_if<(I > ArrayType::LSB)>::type
_reduce(FunctionType&& inFunction, ArgTypes&&... inArgs) {}

template <typename ArrayType, unsigned I, typename FunctionType, typename... ArgTypes>
typename std::enable_if<(I <= ArrayType::LSB)>::type
_reduce(FunctionType&& inFunction, ArgTypes&&... inArgs) {
  inFunction(I, ArrayType::template query<I>(), inArgs...);
  _reduce<ArrayType, I+1>(inFunction, inArgs...);
}

template <typename ArrayType, typename FunctionType, typename... ArgTypes>
void reduce(FunctionType&& inFunction, ArgTypes&&... inArgs) {
  _reduce<ArrayType, ArrayType::MSB>(inFunction, inArgs...);
}


#endif
