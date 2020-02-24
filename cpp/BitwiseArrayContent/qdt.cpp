#include <bitset>
#include <functional>
#include <iostream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <set>
#include <type_traits>
#include <unordered_set>
#include <utility>
using std::bitset;
using std::cout;
using std::dec;
using std::endl;
using std::hex;
using std::ostream;
using std::pair;
using std::set;
using std::setw;
using std::setfill;
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

#include "ArrayContent.h"
#include "ArrayField.h"
#include "ArrayFields.h"
#include "ArrayReduce.h"
#include "ArraySubFields.h"
#include "../BitManipulation/Interleave.h"
#include "../BitManipulation/Reverse.h"
#include "../BitManipulation/Rotate.h"




struct BitTranslation {
  unsigned origin;
  unsigned destination;

  //  Equal
  //  Not Equal
  //  Less
  //  Less or Equal
  //  Greater
  //  Greater or Equal

  bool origin_eq(const BitTranslation& lhs, const BitTranslation& rhs) { return lhs.origin == rhs.origin; }
  bool origin_ne(const BitTranslation& lhs, const BitTranslation& rhs) { return lhs.origin != rhs.origin; }
  bool origin_lt(const BitTranslation& lhs, const BitTranslation& rhs) { return lhs.origin <  rhs.origin; }
  bool origin_le(const BitTranslation& lhs, const BitTranslation& rhs) { return lhs.origin <= rhs.origin; }
  bool origin_gt(const BitTranslation& lhs, const BitTranslation& rhs) { return lhs.origin >  rhs.origin; }
  bool origin_ge(const BitTranslation& lhs, const BitTranslation& rhs) { return lhs.origin >= rhs.origin; }

  bool destination_eq(const BitTranslation& lhs, const BitTranslation& rhs) { return lhs.destination == rhs.destination; }
  bool destination_ne(const BitTranslation& lhs, const BitTranslation& rhs) { return lhs.destination != rhs.destination; }
  bool destination_lt(const BitTranslation& lhs, const BitTranslation& rhs) { return lhs.destination <  rhs.destination; }
  bool destination_le(const BitTranslation& lhs, const BitTranslation& rhs) { return lhs.destination <= rhs.destination; }
  bool destination_gt(const BitTranslation& lhs, const BitTranslation& rhs) { return lhs.destination >  rhs.destination; }
  bool destination_ge(const BitTranslation& lhs, const BitTranslation& rhs) { return lhs.destination >= rhs.destination; }

};

bool operator<(const BitTranslation& lhs, const BitTranslation& rhs) {
  return lhs.destination < rhs.destination;
  // return lhs.origin < rhs.origin;
}


int main() {
  using UnrotatedContentType = BufferedArrayContent<
    ArrayFields<
      ArrayField< 0, 59>
    >,
    Interleave<0, 55, 7>
  >;

  using RotatedContentType = BufferedArrayContent<
    ArrayFields<
      ArrayField< 0, 59>
    >,
    Interleave<0, 55, 7>,
    Rotate<31, 59, RotateRight, 4>,
    Rotate<32, 34, RotateLeft, 1>
  >;

  {
    std::stringstream ss1;
    std::stringstream ss2;
    auto s = set<BitTranslation>();

    reduce<UnrotatedContentType>(
      [](
        const unsigned& origin,
        const unsigned& destination,
        set<BitTranslation>& inSet
      ) {
        auto t = BitTranslation();
        t.origin = origin;
        t.destination = destination;
        inSet.insert(t);
      },
      s
    );

    for (auto& trans : s) {
      ss1 << setw(3) << trans.origin;
      ss2 << setw(3) << trans.destination;
    }

    cout << ss1.str() << endl;
    cout << ss2.str() << endl;
  }

  cout << endl;

  {
    std::stringstream ss1;
    std::stringstream ss2;
    auto s = set<BitTranslation>();

    reduce<RotatedContentType>(
      [](
        const unsigned& origin,
        const unsigned& destination,
        set<BitTranslation>& inSet
      ) {
        auto t = BitTranslation();
        t.origin = origin;
        t.destination = destination;
        inSet.insert(t);
      },
      s
    );

    for (auto& trans : s) {
      ss1 << setw(3) << trans.origin;
      ss2 << setw(3) << trans.destination;
    }

    cout << ""
      << ss1.str() << endl
      << ss2.str() << endl;
  }

}
