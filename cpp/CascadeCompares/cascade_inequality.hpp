#ifndef cascade_inequality_hpp
#define cascade_inequality_hpp

#include <type_traits>


// Non-recursive cascade_inequality call (tail)
//   Compare the current values
template <template <typename> class InequalityType, typename ObjectType, typename CurrentGetterType, typename... RemainingGetterTypes>
constexpr typename std::enable_if<(sizeof...(RemainingGetterTypes) == 0), bool>::type
cascade_inequality(const ObjectType& lhs, const ObjectType& rhs, CurrentGetterType inCurrentGetter, RemainingGetterTypes... inRemainingGetters) {
  return InequalityType<typename std::result_of<CurrentGetterType(ObjectType*)>::type>()((lhs.*inCurrentGetter)(), (rhs.*inCurrentGetter)());
}

// Recursive cascade_inequality call
//   If the current values are equal, recurse to the next one
//   Else, compare the current ones
template <template <typename> class InequalityType, typename ObjectType, typename CurrentGetterType, typename... RemainingGetterTypes>
constexpr typename std::enable_if<(sizeof...(RemainingGetterTypes) > 0), bool>::type
cascade_inequality(const ObjectType& lhs, const ObjectType& rhs, CurrentGetterType inCurrentGetter, RemainingGetterTypes... inRemainingGetters) {
  return ((lhs.*inCurrentGetter)() == (rhs.*inCurrentGetter)()) ? (cascade_inequality<InequalityType>(lhs, rhs, inRemainingGetters...)) : (cascade_inequality<InequalityType>(lhs, rhs, inCurrentGetter));
}


#endif
