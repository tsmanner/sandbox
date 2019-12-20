#include <iostream>

#include "ConditionalStream.h"

using std::cout;
using std::endl;


int main() {
  int x = 5;
  auto cs = make_conditional_stream(1, " hello world ", x);
  cout << "CS(" << cs.enabled() << "): " << cs << endl;
  cs.setEnabled(false);
  cout << "CS(" << cs.enabled() << "): " << cs << endl;

  return 0;
}
