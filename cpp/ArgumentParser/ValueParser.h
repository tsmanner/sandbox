#ifndef ValueParser_h
#define ValueParser_h

#include <string>
#include <utility>

#include <boost/lexical_cast.hpp>


namespace parsing {


template <typename ValueType>
struct ValueParser {
  static std::pair<bool, ValueType> parse(const std::string& inValueString) {
    try {
      return {true, boost::lexical_cast<ValueType>(inValueString)};
    }
    catch (const boost::bad_lexical_cast&) {
      return {false, ValueType()};
    }
  }
};


} // namespace parsing


#endif

