#include <algorithm>
#include <iomanip>
#include <iostream>
#include <type_traits>
using std::cout;
using std::enable_if;
using std::endl;
using std::ostream;
using std::setfill;
using std::setw;

#include "../TableFormat/TableFormat.h"
using TableT = Table<5, 3>;

using std::max;
#include "SpecSched.h"
#include "SpeculativeOperand.h"


class Owner {
  object_ptr<Node<SpeculativeOperand>> mNode = new Source<SpeculativeOperand>(&Owner::getValue, this);
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


int main() {
  auto a = Owner(SpeculativeOperand(1, 5));
  auto b = Owner(SpeculativeOperand(2, 7));
  auto c = Owner();

  c.setNode(max(a.getNode(), b.getNode()) + SpeculativeOperand(2));

  TableT::div(cout) << endl;
  TableT::row(cout, "a", "b", "c") << endl;
  TableT::div(cout) << endl;
  TableT::row(cout, a.getNode()->resolve(), b.getNode()->resolve(), c.getNode()->resolve()) << " " << c.getNode()->to_string() << endl; a.makeConcrete();
  TableT::row(cout, a.getNode()->resolve(), b.getNode()->resolve(), c.getNode()->resolve()) << " " << c.getNode()->to_string() << endl; b.makeConcrete();
  TableT::row(cout, a.getNode()->resolve(), b.getNode()->resolve(), c.getNode()->resolve()) << " " << c.getNode()->to_string() << endl;
  TableT::div(cout) << endl;

  return 0;
}
