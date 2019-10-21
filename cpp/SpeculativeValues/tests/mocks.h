#include "object_ptr.h"


class Owner {
  object_ptr<SpeculativeNode<SpeculativeOperand>> mNode = new SpeculativeSource<SpeculativeOperand>(&Owner::getValue, this);
  SpeculativeOperand mValue;

public:

  Owner(
    const SpeculativeOperand& inValue = SpeculativeOperand()
  ):
    mValue(inValue)
  {
  }

  virtual ~Owner() {}

  void setValue(SpeculativeOperand inValue) { mValue = inValue; }
  const SpeculativeOperand& getValue() const { return mValue; }
  void makeConcrete() { mValue.makeConcrete(); }

  void setNode(decltype(mNode) inNode) { mNode = inNode; }
  decltype(mNode) getNode() { return mNode; }

};
