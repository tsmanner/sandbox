#include <iomanip>
#include <iostream>

using std::cout;
using std::endl;

#include "FilterByType.h"


int main() {
  int x = 0;
  uint y = 1;
  auto fc = FilterableCollection<int, uint>();
  fc.insert(x);
  fc.insert(y);

  cout << "ints: { ";
  for (auto element : fc.get<int>()) {
    cout << element << " ";
  }
  cout << "}" << endl;

  cout << "uints: { ";
  for (auto element : fc.get<uint>()) {
    cout << element << " ";
  }
  cout << "}" << endl;

  cout << "chars: { ";
  for (auto element : fc.get<char>()) {
    cout << element << " ";
  }
  cout << "}" << endl;

  return 0;
}
