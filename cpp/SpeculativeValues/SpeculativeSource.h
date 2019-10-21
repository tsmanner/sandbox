#ifndef SpeculativeSource_h
#define SpeculativeSource_h

#include <ostream>
#include <sstream>
#include <string>

#include "object_ptr.h"

#include "SpeculativeNode.h"


template <typename T>
class SpeculativeSource : public SpeculativeNode<T> {
public:
  template <typename FunctionType, typename... ArgTypes>
  SpeculativeSource(
    FunctionType inSourceFunction,
    ArgTypes... inArgs
  ):
    SpeculativeNode<T>(),
    mSourceFunction(std::bind(inSourceFunction, inArgs...))
  {
  }

  virtual ~SpeculativeSource() {}

  virtual T resolve() { return mSourceFunction(); }

  virtual std::string to_string() {
    std::stringstream ss;
    ss << mSourceFunction();
    return ss.str();
  }

  friend std::ostream& operator<<(std::ostream& os, SpeculativeSource<T> s) { return os << s.to_string(); }

private:
  std::function<T()> mSourceFunction;

};


#endif
