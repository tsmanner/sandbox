#ifndef cascade_equality_hpp
#define cascade_equality_hpp

#include <type_traits>


// Non-recursive cascade_equality call (tail)
//   Equality compare the current values
template <typename ObjectType, typename CurrentGetterType, typename... RemainingGetterTypes>
constexpr typename std::enable_if<(sizeof...(RemainingGetterTypes) == 0), bool>::type
cascade_equality(const ObjectType& lhs, const ObjectType& rhs, CurrentGetterType inCurrentGetter, RemainingGetterTypes... inRemainingGetters) {
  return (lhs.*inCurrentGetter)() == (rhs.*inCurrentGetter)();
}

// Recursive cascade_equality call
//   Equality compare the current values AND with the recursive call
template <typename ObjectType, typename CurrentGetterType, typename... RemainingGetterTypes>
constexpr typename std::enable_if<(sizeof...(RemainingGetterTypes) > 0), bool>::type
cascade_equality(const ObjectType& lhs, const ObjectType& rhs, CurrentGetterType inCurrentGetter, RemainingGetterTypes... inRemainingGetters) {
  return cascade_equality(lhs, rhs, inCurrentGetter) and cascade_equality(lhs, rhs, inRemainingGetters...);
}


#endif
