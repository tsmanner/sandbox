#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>
#include <set>
#include <sstream>

using std::cout;
using std::endl;
using std::setw;

#include "SpeculativeOperation.h"
#include "SpeculativeOperand.h"


int main() {
  //
  // Speculative Operation
  //

  // auto x = SpecValue<SpeculativeOperand>(SpeculativeOperand(1, 2));
  // auto y = SpecValue<SpeculativeOperand>(SpeculativeOperand(2, 3));
  // auto z = SpecValue<SpeculativeOperand>(SpeculativeOperand(3, 4));
  // auto xPyMz = x + y - z;
  // auto xPy = x + y;
  // auto xPy_Mz = xPy - z;

  using ValT = SpecValue<SpeculativeOperand>;
  using ValTP = object_ptr<SpecAtom<SpeculativeOperand>>;

  auto xv = new ValT(SpeculativeOperand(1, 2));
  auto yv = new ValT(SpeculativeOperand(1, 3));
  auto zv = new ValT(SpeculativeOperand(1, 4));
  auto x = ValTP(xv);
  auto y = ValTP(yv);
  auto z = ValTP(zv);
  auto xPyMz = x + y - z;
  auto xyzMax = max(x, y, z);
  auto xyzMin = min(x, y, z);

  cout << "+---------+---------+---------+---------+---------+---------+" << endl;
  cout << "|    x    |    y    |    z    |  x+y-z  |   max   |   min   |" << endl;
  cout << "+---------+---------+---------+---------+---------+---------+" << endl;
  cout << "| " << setw(7) << *x << " | " << setw(7) << *y << " | " << setw(7) << *z << " | " << setw(7) << xPyMz->resolve() << " | " << setw(7) << xyzMax->resolve() << " | " << setw(7) << xyzMin->resolve() << " | " << *xPyMz << endl; xv->getOperand().makeConcrete();
  cout << "| " << setw(7) << *x << " | " << setw(7) << *y << " | " << setw(7) << *z << " | " << setw(7) << xPyMz->resolve() << " | " << setw(7) << xyzMax->resolve() << " | " << setw(7) << xyzMin->resolve() << " | " << *xPyMz << endl; yv->getOperand().makeConcrete();
  cout << "| " << setw(7) << *x << " | " << setw(7) << *y << " | " << setw(7) << *z << " | " << setw(7) << xPyMz->resolve() << " | " << setw(7) << xyzMax->resolve() << " | " << setw(7) << xyzMin->resolve() << " | " << *xPyMz << endl; zv->getOperand().makeConcrete();
  cout << "| " << setw(7) << *x << " | " << setw(7) << *y << " | " << setw(7) << *z << " | " << setw(7) << xPyMz->resolve() << " | " << setw(7) << xyzMax->resolve() << " | " << setw(7) << xyzMin->resolve() << " | " << *xPyMz << endl;
  cout << "+---------+---------+---------+---------+---------+---------+" << endl;

  return 0;
}
