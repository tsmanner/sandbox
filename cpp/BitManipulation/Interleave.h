#ifndef Interleave_h
#define Interleave_h

#include <limits>
#include <tuple>
#include <type_traits>

#include <iostream>


template <int MSB, int LSB, int WAYS>
class Interleave {
public:
  static const int LENGTH = LSB - MSB + 1;
  static const int GROUP = LENGTH / WAYS;

  //
  // <0, 5, 2>
  // LENGTH = 6
  //   WAYS = 2 (W)
  //  GROUP = 3 (G)
  //           I  Offset  I/W  I%W  Equation
  //   a -> a  0     0     0    0    I / W
  //   b -> d  1    +2     0    1    I / W + (I % W) * G
  //   c -> b  2    -1     1    0    I / W
  //   d -> e  3    +1     1    1    I / W + (I % W) * G
  //   e -> c  4    -2     2    0    I / W
  //   f -> f  5     0     2    1    I / W + (I % W) * G

  static constexpr int query(const int& index) {
    return (MSB <= index and index <= LSB) ?
           ((index / WAYS) + ((index % WAYS) * GROUP)) :
           index;
  }

};


#endif
