#include <iomanip>
#include <iostream>

using std::cout;
using std::endl;
using std::setw;

#include "Interleave.h"
#include "Fold.h"
#include "Rotate.h"
#include "Shuffle.h"


int main() {
  using s = Shuffle<0, 1, 4>;

  cout
    << " 0 -> " << setw(2) << s::query<0>() << endl
    << " 1 -> " << setw(2) << s::query<1>() << endl
    << " 2 -> " << setw(2) << s::query<2>() << endl
    << " 3 -> " << setw(2) << s::query<3>() << endl
    << " 4 -> " << setw(2) << s::query<4>() << endl
    << endl;


  // using f = Fold<3, 7, 5, s>;
  // using f = Fold<0, 9, 5, s>;
  using f = Fold<0, 15, 2, Shuffle<0, 1>>;

  cout
    << " 0 -> " << setw(2) << f::query< 0>() << endl
    << " 1 -> " << setw(2) << f::query< 1>() << endl
    << " 2 -> " << setw(2) << f::query< 2>() << endl
    << " 3 -> " << setw(2) << f::query< 3>() << endl
    << " 4 -> " << setw(2) << f::query< 4>() << endl
    << " 5 -> " << setw(2) << f::query< 5>() << endl
    << " 6 -> " << setw(2) << f::query< 6>() << endl
    << " 7 -> " << setw(2) << f::query< 7>() << endl
    << " 8 -> " << setw(2) << f::query< 8>() << endl
    << " 9 -> " << setw(2) << f::query< 9>() << endl
    << "10 -> " << setw(2) << f::query<10>() << endl
    << "11 -> " << setw(2) << f::query<11>() << endl
    << "12 -> " << setw(2) << f::query<12>() << endl
    << "13 -> " << setw(2) << f::query<13>() << endl
    << "14 -> " << setw(2) << f::query<14>() << endl
    << "15 -> " << setw(2) << f::query<15>() << endl
    << endl;

  using RotateType = Rotate<1, 14, RotateRight, 1>;
  cout
    << " 0 -> " << setw(2) << RotateType::query< 0>() << endl
    << " 1 -> " << setw(2) << RotateType::query< 1>() << endl
    << " 2 -> " << setw(2) << RotateType::query< 2>() << endl
    << " 3 -> " << setw(2) << RotateType::query< 3>() << endl
    << " 4 -> " << setw(2) << RotateType::query< 4>() << endl
    << " 5 -> " << setw(2) << RotateType::query< 5>() << endl
    << " 6 -> " << setw(2) << RotateType::query< 6>() << endl
    << " 7 -> " << setw(2) << RotateType::query< 7>() << endl
    << " 8 -> " << setw(2) << RotateType::query< 8>() << endl
    << " 9 -> " << setw(2) << RotateType::query< 9>() << endl
    << "10 -> " << setw(2) << RotateType::query<10>() << endl
    << "11 -> " << setw(2) << RotateType::query<11>() << endl
    << "12 -> " << setw(2) << RotateType::query<12>() << endl
    << "13 -> " << setw(2) << RotateType::query<13>() << endl
    << "14 -> " << setw(2) << RotateType::query<14>() << endl
    << "15 -> " << setw(2) << RotateType::query<15>() << endl
    << endl;

  return 0;
}
