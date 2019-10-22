#ifndef Shuffle_h
#define Shuffle_h

#include <type_traits>


template <int... INDICES>
class Shuffle {
public:

  // Helper method to return the first element of a template
  // parameter pack.  Used by _query
  template <int I, int... Is>
  static constexpr int get_first() { return I; }

  // Terminal function for the case where the query index
  // is found somewhere in the middle of INDICES
  template <int QUERY, int FIRST, int I, int... Is>
  static constexpr typename std::enable_if<(QUERY == I and sizeof...(Is) != 0), int>::type
  _query() { return get_first<Is...>(); }

  // Terminal function for the case where the query index
  // is the last I in INDICES
  template <int QUERY, int FIRST, int I, int... Is>
  static constexpr typename std::enable_if<(QUERY == I and sizeof...(Is) == 0), int>::type
  _query() { return FIRST; }

  // Terminal function for the case where the query index
  // is not in INDICES
  template <int QUERY, int FIRST, int I, int... Is>
  static constexpr typename std::enable_if<(QUERY != I and sizeof...(Is) == 0), int>::type
  _query() { return QUERY; }

  // Recursive query function that scans the INDICES
  // for the QUERY index
  template <int QUERY, int FIRST, int I, int... Is>
  static constexpr typename std::enable_if<(QUERY != I and sizeof...(Is) != 0), int>::type
  _query() { return _query<QUERY, FIRST, Is...>(); }

  // First query, duplicates FIRST so that we can hold
  // onto it, just in case QUERY is the last element in INDICES
  template <int QUERY, int FIRST, int... Is>
  static constexpr int
  _start_query() { return _query<QUERY, FIRST, FIRST, Is...>(); }

  // Entry point function, templated only with the index to look up
  template <int QUERY>
  static constexpr int query() {
    return _start_query<QUERY, INDICES...>();
  }

};


#endif
