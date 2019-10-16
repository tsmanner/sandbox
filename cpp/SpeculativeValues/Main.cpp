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

  auto x = SpecValue<SpeculativeOperand>(SpeculativeOperand(1, 2));
  auto y = SpecValue<SpeculativeOperand>(SpeculativeOperand(2, 3));
  auto z = SpecValue<SpeculativeOperand>(SpeculativeOperand(3, 4));
  auto xPyMz = x + y - z;
  auto xPy = x + y;
  auto xPy_Mz = xPy - z;

  cout << "+---------+---------+---------+---------+" << endl;
  cout << "|    x    |    y    |    z    |  x+y-z  |" << endl;
  cout << "+---------+---------+---------+---------+" << endl;
  cout << "| " << setw(7) << x << " | " << setw(7) << y << " | " << setw(7) << z << " | " << setw(7) << xPyMz.resolve() << " |" << endl; x.getOperand().makeConcrete();
  cout << "| " << setw(7) << x << " | " << setw(7) << y << " | " << setw(7) << z << " | " << setw(7) << xPyMz.resolve() << " |" << endl; z.getOperand().makeConcrete();
  cout << "| " << setw(7) << x << " | " << setw(7) << y << " | " << setw(7) << z << " | " << setw(7) << xPyMz.resolve() << " |" << endl; y.getOperand().makeConcrete();
  cout << "| " << setw(7) << x << " | " << setw(7) << y << " | " << setw(7) << z << " | " << setw(7) << xPyMz.resolve() << " |" << endl;
  cout << "+---------+---------+---------+---------+" << endl;

  return 0;
}
