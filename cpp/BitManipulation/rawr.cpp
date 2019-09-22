#include <iomanip>
#include <iostream>

using std::cout;
using std::endl;

#include "Shuffle.h"


int main() {
  using s = Shuffle<0, 1, 4>;

  cout << s::query<0>() << endl;
  cout << s::query<1>() << endl;
  cout << s::query<2>() << endl;
  cout << s::query<3>() << endl;
  cout << s::query<4>() << endl;

  return 0;
}
