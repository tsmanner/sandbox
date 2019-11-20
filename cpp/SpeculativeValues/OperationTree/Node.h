#ifndef Node_h
#define Node_h

#include <functional>
#include <list>
#include <sstream>
#include <string>

#include "object_ptr.h"


template <typename T>
class Node : public object_ptr_interface {
public:
  Node(): object_ptr_interface() {}

  Node(
    T inValue
  ):
    object_ptr_interface(),
    mIsReal(true),
    mValue(inValue)
  {
  }

  virtual ~Node() {}

  //
  // Real
  //
  void makeReal() {
    if (!mIsReal) {
      mIsReal = true;
      for (auto f : mOnRealFunctions) f();
    }
  }
  const bool& isReal() { return mIsReal; }

  //
  // Value
  //
  void setValue(const T& inValue) { mValue = inValue; Node::makeReal(); }
  const T& getValue() { return mValue; }

  //
  // Callbacks
  //
  void addOnRealFunction(std::function<void()> inOnRealFunction) { mOnRealFunctions.push_back(inOnRealFunction); }

  virtual std::string to_string(bool elaborate = false) {
    std::stringstream ss;
    if (isReal()) {
      ss << mValue;
    }
    else {
      ss << "_";
    }
    return ss.str();
  }

private:
  bool mIsReal = false;
  T mValue = T();

  std::list<std::function<void()>> mOnRealFunctions;

};


#endif
