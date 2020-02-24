#ifndef Streaming_h
#define Streaming_h

#include <ostream>


namespace parsing {


template <typename T>
struct HelpStreamer {
  HelpStreamer(const T& inT): mT(inT) {}

  inline friend std::ostream& operator<<(std::ostream& os, const HelpStreamer<T>& inStreamer) {
    inStreamer.mT.streamHelp(os);
    return os;
  }

private:
  const T& mT;

};


template <typename T>
HelpStreamer<T> stream_help(const T& inT) { return HelpStreamer<T>(inT); }


template <typename T>
struct ValueStreamer {
  ValueStreamer(const T& inT): mT(inT) {}

  inline friend std::ostream& operator<<(std::ostream& os, const ValueStreamer<T>& inStreamer) {
    inStreamer.mT.streamValue(os);
    return os;
  }

private:
  const T& mT;

};


template <typename T>
ValueStreamer<T> stream_value(const T& inT) { return ValueStreamer<T>(inT); }


template <typename T>
struct NameAndValueStreamer {
  NameAndValueStreamer(const T& inT): mT(inT) {}

  inline friend std::ostream& operator<<(std::ostream& os, const NameAndValueStreamer<T>& inStreamer) {
    os << inStreamer.mT.getName() << " = ";
    inStreamer.mT.streamValue(os);
    return os;
  }

private:
  const T& mT;

};


template <typename T>
NameAndValueStreamer<T> stream_name_and_value(const T& inT) { return NameAndValueStreamer<T>(inT); }


} // namespace parsing


#endif
