#ifndef Reverse_h
#define Reverse_h

#include <type_traits>


template <int MSB, int LSB>
class Reverse {
public:

  // Query function, templated only with the index to look up
  // Enabled if QUERY falls between MSB and LSB
  template <int QUERY>
  static constexpr typename std::enable_if<(MSB <= QUERY and QUERY <= LSB), int>::type
  query() {
    return LSB + MSB - QUERY;
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
