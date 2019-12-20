#ifndef ConditionalStream_h
#define ConditionalStream_h

#include <iostream>
#include <type_traits>
#include <tuple>


class ConditionalStream {
public:
  ConditionalStream(std::ostream& os): mStream(os), mEnabled(true) {}

  bool enabled() const { return mEnabled; }
  void setEnabled(const bool& inEnabled) { mEnabled = inEnabled; }

  template <int I, typename... Elements>
  static typename std::enable_if<(I == sizeof...(Elements)), std::ostream&>::type
  _stream(std::ostream& os, const std::tuple<const Elements&...>& elements) {
    return os;
  }

  template <int I, typename... Elements>
  static typename std::enable_if<(I < sizeof...(Elements)), std::ostream&>::type
  _stream(std::ostream& os, const std::tuple<const Elements&...>& elements) {
    os << std::get<I>(elements);
    _stream<I+1, Elements...>(os, elements);
    return os;
  }

  template <typename... Elements>
  std::ostream& stream(const Elements&... elements) const {
    if (enabled()) _stream<0, Elements...>(mStream, std::tuple<const Elements&...>(elements...));
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
