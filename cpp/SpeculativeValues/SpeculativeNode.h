#ifndef SpeculativeNode_h
#define SpeculativeNode_h

#include <string>

#include "object_ptr.h"


template <typename T>
class SpeculativeNode : public object_ptr_interface {
public:
  SpeculativeNode(): object_ptr_interface() {}
  virtual ~SpeculativeNode() {}

  virtual T resolve() = 0;
  virtual bool isConcrete() = 0;
  virtual void makeConcrete() = 0;
  virtual std::string to_string() = 0;

};


#endif
