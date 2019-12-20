#ifndef ConditionalStream_h
#define ConditionalStream_h

#include <iostream>
#include <type_traits>
#include <tuple>


template <typename... Elements>
class ConditionalStream {
public:
  ConditionalStream(Elements... elements): mElements(elements...) {}

  bool enabled() const { return mEnabled; }
  void setEnabled(const bool& inEnabled) { mEnabled = inEnabled; }

  template <int I>
  typename std::enable_if<(I == sizeof...(Elements)), std::ostream&>::type
  _stream(std::ostream& os, const std::tuple<Elements...>& elements) const {
    return os;
  }

  template <int I>
  typename std::enable_if<(I < sizeof...(Elements)), std::ostream&>::type
  _stream(std::ostream& os, const std::tuple<Elements...>& elements) const {
    os << std::get<I>(elements);
    _stream<I+1>(os, elements);
    return os;
  }

  std::ostream& stream(std::ostream& os) const {
    _stream<0>(os, mElements);
    return os;
  }

  inline friend std::ostream& operator<<(std::ostream& os, const ConditionalStream& cs) {
    if (cs.enabled()) return cs.stream(os);
    return os;
  }

private:
  std::tuple<Elements...> mElements;
  bool mEnabled;

};


template <typename... Elements>
ConditionalStream<Elements...> make_conditional_stream(Elements... elements) {
  return ConditionalStream<Elements...>(elements...);
}


#endif
