#ifndef Rotate_h
#define Rotate_h

#include <type_traits>


struct RotateLeft {};
struct RotateRight {};


template <unsigned MSB, unsigned LSB, typename RotationType, unsigned DISTANCE>
struct Rotate {
  static constexpr unsigned WIDTH = LSB - MSB + 1;

  // Query function, templated only with the index to look up
  // Enabled if QUERY falls between MSB and LSB and we are rotating left
  template <int QUERY>
  static constexpr typename std::enable_if<(
    std::is_base_of<RotateLeft, RotationType>::value and
    MSB <= QUERY and
    QUERY <= LSB
  ), int>::type
  query() {
    return (((QUERY - MSB) + (WIDTH - DISTANCE)) % WIDTH) + MSB;
  }

  // Query function, templated only with the index to look up
  // Enabled if QUERY falls between MSB and LSB and we are rotating right
  template <int QUERY>
  static constexpr typename std::enable_if<(
    std::is_base_of<RotateRight, RotationType>::value and
    MSB <= QUERY and
    QUERY <= LSB
  ), int>::type
  query() {
    return (((QUERY - MSB) + DISTANCE) % WIDTH) + MSB;
  }

  // Query function, templated only with the index to look up
  // Enabled if QUERY does not fall between MSB and LSB
  template <int QUERY>
  static constexpr typename std::enable_if<(!(MSB <= QUERY and QUERY <= LSB)), int>::type
  query() {
    return QUERY;
  }

};


#endif
