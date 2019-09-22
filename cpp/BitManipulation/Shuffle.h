#ifndef Shuffle_h
#define Shuffle_h

#include <tuple>
#include <type_traits>

#include <iostream>

template <int... INDICES>
class Shuffle {
public:

  //
  // Shuffle <0, 1, 2>
  //
  // tmp = [2]
  // [2] = [1]
  // [1] = [0]
  // [0] = tmp
  //
  // Template Function Structure
  //
  // template <int FIRST, int... REMAINING, int LAST> EntryFunction(t)
  //   tmp = std::get<LAST>(t)
  //   IterativeFunction<BitStringType, FIRST, REMAINING..., LAST>(t)
  //   std::get<FIRST>(t) = tmp
  //
  // template <int CURRENT, int NEXT, int... REMAINING> IterativeFunction(t)
  //   IterativeFunction<NEXT, REMAINING>(t)
  //   std::get<NEXT>(t) = std::get<CURRENT>(t)

  template <typename BitStringType, int CURRENT, int NEXT, int...REMAINING>
  static typename std::enable_if<(sizeof...(REMAINING) == 0)>::type
  IterativeFunction(BitStringType& bitstring) {
    std::get<NEXT>(bitstring) = std::get<CURRENT>(bitstring);
  }

  template <typename BitStringType, int CURRENT, int NEXT, int...REMAINING>
  static typename std::enable_if<(sizeof...(REMAINING) != 0)>::type
  IterativeFunction(BitStringType& bitstring) {
    IterativeFunction<BitStringType, NEXT, REMAINING...>(bitstring);
    std::get<NEXT>(bitstring) = std::get<CURRENT>(bitstring);
  }

  template <typename BitStringType, int FIRST, int... REMAINING>
  static void
  EntryFunction(BitStringType& bitstring) {
    auto tmp = std::get<sizeof...(REMAINING)>(bitstring);
    IterativeFunction<BitStringType, FIRST, REMAINING...>(bitstring);
    std::get<FIRST>(bitstring) = tmp;
  }

  template <typename BitStringType>
  static BitStringType
  apply(BitStringType bitstring) {
    EntryFunction<BitStringType, INDICES...>(bitstring);
    return bitstring;
  }

};


#endif
