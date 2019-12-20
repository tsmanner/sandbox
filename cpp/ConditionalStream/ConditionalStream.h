#ifndef ConditionalStream_h
#define ConditionalStream_h

#include <iostream>
#include <type_traits>


class ConditionalStream {
public:
  ConditionalStream(std::ostream& os): mStream(os), mEnabled(true) {}

  bool enabled() const { return mEnabled; }
  void setEnabled(const bool& inEnabled) { mEnabled = inEnabled; }

  template <typename Element, typename... Elements>
  typename std::enable_if<(sizeof...(Elements) == 0), std::ostream&>::type
  _stream(const Element& element, const Elements&... elements) const {
    return mStream << element;
  }

  template <typename Element, typename... Elements>
  typename std::enable_if<(sizeof...(Elements) != 0), std::ostream&>::type
  _stream(const Element& element, const Elements&... elements) const {
    mStream << element;
    _stream<Elements...>(elements...);
    return mStream;
  }

  template <typename... Elements>
  std::ostream& stream(const Elements&... elements) const {
    if (enabled()) _stream(elements...);
    return mStream;
  }

  struct EndlineProxy {
    friend std::ostream& operator<<(std::ostream& os, const ConditionalStream::EndlineProxy& inEndl) {
      return os << std::endl;
    }
  };

  const EndlineProxy& endl{};

private:
  std::ostream& mStream;
  bool mEnabled;

};


#endif
