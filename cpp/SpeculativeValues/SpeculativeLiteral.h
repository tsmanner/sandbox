#ifndef SpeculativeLiteral_h
#define SpeculativeLiteral_h

#include <ostream>
#include <sstream>
#include <string>

#include "object_ptr.h"

#include "SpeculativeNode.h"


template <typename T>
class SpeculativeLiteral : public SpeculativeNode<T> {
public:
  SpeculativeLiteral(
    const T& inT
  ):
    SpeculativeNode<T>(),
    mValue(inT)
  {
  }

  template <typename... ArgTypes>
  SpeculativeLiteral(
    ArgTypes... inArgs
  ):
    SpeculativeNode<T>(),
    mValue(inArgs...)
  {
  }

  virtual ~SpeculativeLiteral() {}

  virtual T resolve() { return mValue; }

  virtual void makeConcrete() { mValue.makeConcrete(); }

  virtual std::string to_string() {
    std::stringstream ss;
    ss << mValue;
    return ss.str();
  }

  friend std::ostream& operator<<(std::ostream& os, SpeculativeLiteral<T> l) { return os << l.to_string(); }

private:
  T mValue;

};


#endif
