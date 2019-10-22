#ifndef FilterByType_h
#define FilterByType_h

#include <set>
#include <type_traits>


//
// Base class for the FilterableSet
// Holds one set of for later access
//
template <typename T>
class _FilterableSet {
public:

  void insert(const T& t) { mTs.insert(t); }

  const std::set<T>& get() { return mTs; }

private:
  std::set<T> mTs;

};


//
// A multi-type set which filters contents based on type
//   Variadic template that inherits from _FilterableSet
//   for each type in Types...
//
template <typename... Types>
class FilterableSet : public _FilterableSet<Types>... {
public:

  //
  // _contains
  // Template recursive function that determines whether a template
  // parameter pack (T0, Ts...) contains a particular type (T)
  //

  // T was found, end recursion with true
  template <typename T, typename T0, typename... Ts>
  static constexpr typename std::enable_if<(std::is_same<T, T0>()), bool>::type
  _contains() { return true; }

  // T was not found and we're out of types, end recursion with false
  template <typename T, typename T0, typename... Ts>
  static constexpr typename std::enable_if<((sizeof...(Ts) == 0) and !std::is_same<T, T0>()), bool>::type
  _contains() { return  false; }

  // T was not found and there are types remaining, recurse, dropping one type
  template <typename T, typename T0, typename... Ts>
  static constexpr typename std::enable_if<((sizeof...(Ts) != 0) and !std::is_same<T, T0>()), bool>::type
  _contains() { return  _contains<T, Ts...>(); }


  // Insert an element of type T
  // T must be in Types...
  template <typename T>
  void insert(const T& t) { _FilterableSet<T>::insert(t); }


  //
  // Get a set of elements of type T
  //

  // If Types... contains T, get the set of Ts
  template <typename T>
  typename std::enable_if<(_contains<T, Types...>()), const std::set<T>&>::type
  get() { return _FilterableSet<T>::get(); }

  // If Types... does not contain T, return an empty set
  template <typename T>
  typename std::enable_if<(!_contains<T, Types...>()), std::set<T>>::type
  get() { return std::set<T>(); }

private:

};


#endif
