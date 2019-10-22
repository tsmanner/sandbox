#ifndef SpeculativeNode_h
#define SpeculativeNode_h

#include <string>

#include "object_ptr.h"


template <typename T>
class SpeculativeValue {
public:
  SpeculativeValue() {}
  SpeculativeValue(const T& inValue): mIsConcrete(true), mConcreteValue(inValue) {}

  const bool& isConcrete() const { return mIsConcrete; }
  const T& getConcreteValue() const { return mConcreteValue; }

private:
  bool mIsConcrete = false;
  T mConcreteValue = T();

};


#endif
