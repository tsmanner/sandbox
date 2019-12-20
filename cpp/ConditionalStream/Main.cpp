#include <iostream>

#include "ConditionalStream.h"

using std::cout;
using std::endl;


int main() {
  int x = 5;
  auto cs = ConditionalStream(cout);
  cout << "CS(" << cs.enabled() << "): ";
  cs.stream(1, " hello world ", x, cs.endl);
  cs.setEnabled(false);
  cout << "CS(" << cs.enabled() << "): ";
  cs.stream(1, " hello world ", x, cs.endl);

  return 0;
}
