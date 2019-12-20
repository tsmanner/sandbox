#ifndef ConditionalStream_h
#define ConditionalStream_h

#include <iostream>
#include <type_traits>


template <typename StreamType>
class ConditionalStream {
public:
  ConditionalStream(StreamType& inStream): mStream(inStream), mEnabled(true) {}

  bool enabled() const { return mEnabled; }
  void setEnabled(const bool& inEnabled) { mEnabled = inEnabled; }

  template <typename Element, typename... Elements>
  typename std::enable_if<(sizeof...(Elements) == 0), StreamType&>::type
  _stream(const Element& element, const Elements&... elements) const {
    return mStream << element;
  }

  template <typename Element, typename... Elements>
  typename std::enable_if<(sizeof...(Elements) != 0), StreamType&>::type
  _stream(const Element& element, const Elements&... elements) const {
    mStream << element;
    _stream<Elements...>(elements...);
    return mStream;
  }

  template <typename... Elements>
  StreamType& stream(const Elements&... elements) const {
    if (enabled()) _stream(elements...);
    return mStream;
  }

  struct EndlineProxy {
    friend StreamType& operator<<(StreamType& inStream, const ConditionalStream::EndlineProxy& inEndl) {
      return inStream << std::endl;
    }
  };

  const EndlineProxy& endl{};

private:
  StreamType& mStream;
  bool mEnabled;

};


#endif
