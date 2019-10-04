#ifndef Range_h
#define Range_h

#include "Bound.h"


class Range {
public:
  Range(
    LowerBound inLowerBound = LowerBound(),
    UpperBound inUpperBound = UpperBound()
  ):
    mLowerBound(inLowerBound),
    mUpperBound(inUpperBound)
  {
  }

  Range(
    const int& inLowerBound,
    UpperBound inUpperBound
  ):
    mLowerBound(inLowerBound),
    mUpperBound(inUpperBound)
  {
  }

  Range(
    LowerBound inLowerBound,
    const int& inUpperBound
  ):
    mLowerBound(inLowerBound),
    mUpperBound(inUpperBound)
  {
  }

  Range(
    const int& inLowerBound,
    const int& inUpperBound
  ):
    mLowerBound(inLowerBound),
    mUpperBound(inUpperBound)
  {
  }

  LowerBound& getLowerBound() { return mLowerBound; }
  UpperBound& getUpperBound() { return mUpperBound; }

  bool covers(const int& value) {
    return getLowerBound() <= value and value <= getUpperBound();
  }

//   static bool overlaps(Range lhs, Range rhs) {
//     // Overlap is guaranteed if either range has no bounds,
//     // or if both ranges are missing the same bound
//     if (
//       (!lhs.getLowerBound().has_value() and !lhs.getUpperBound().has_value())
//       or
//       (!rhs.getLowerBound().has_value() and !rhs.getUpperBound().has_value())
//       or
//       (!lhs.getLowerBound().has_value() and !rhs.getLowerBound().has_value())
//       or
//       (!lhs.getUpperBound().has_value() and !rhs.getUpperBound().has_value())
//     ) {
//       return true;
//     }

//     if (
//       lhs.getLowerBound().has_value() and
//       lhs.getUpperBound().has_value() and
//       rhs.getLowerBound().has_value() and
//       rhs.getUpperBound().has_value()
//     ) {
//       return
//         lhs.covers(rhs.getLowerBound()) or
//         lhs.covers(rhs.getUpperBound()) or
//         rhs.covers(lhs.getLowerBound()) or
//         rhs.covers(lhs.getUpperBound())
//       ;
//     }

//     if (
//       lhs.getUpperBound().has_value() and rhs.getLowerBound().has_value()
//     ) {
//       return rhs.getLowerBound() <= lhs.getUpperBound();
//     }

//     if (
//       lhs.getLowerBound().has_value() and rhs.getUpperBound().has_value()
//     ) {
//       return lhs.getLowerBound() <= rhs.getUpperBound();
//     }

//     return false; // Is getting here a problem?
//   }

//   static Range merge(Range lhs, Range rhs) {
//     auto newRange = Range();

//     // If this has no lower bound, just take rhs's lower bound
//     if (!lhs.getLowerBound().has_value()) {
//       newRange.getLowerBound() = rhs.getLowerBound();
//     }
//     // If we both have lhs lower bound, take the higher one
//     else if (lhs.getLowerBound().has_value() and rhs.getLowerBound().has_value()) {
//       newRange.getLowerBound() = std::max(lhs.getLowerBound().value(), rhs.getLowerBound().value());
//     }

//     // If this has no upper bound, just take rhs's upper bound
//     if (!lhs.getUpperBound().has_value()) {
//       newRange.getUpperBound() = rhs.getUpperBound();
//     }
//     // If we both have lhs upper bound, take the lower one
//     else if (lhs.getUpperBound().has_value() and rhs.getUpperBound().has_value()) {
//       newRange.getUpperBound() = std::min(lhs.getUpperBound().value(), rhs.getUpperBound().value());
//     }

//     return newRange;
//   }

//   friend bool operator<(Range lhs,Range rhs) {
//     // lhs: Has no lower bound
//     // rhs: Has lower bound
//     // lhs < rhs
//     if (!lhs.getLowerBound().has_value() and rhs.getLowerBound().has_value()) return true;
//     // lhs: Has lower bound
//     // rhs: Has no lower bound
//     // rhs < lhs
//     if (lhs.getLowerBound().has_value() and !rhs.getLowerBound().has_value()) return false;
//     // lhs: Has no lower bound
//     // rhs: Has no lower bound
//     // lhs == rhs
//     if (!lhs.getLowerBound().has_value() and !rhs.getLowerBound().has_value()) return false;
//     // lhs: Has lower bound
//     // rhs: Has lower bound
//     // value compare
//     return lhs.getLowerBound().value() < rhs.getLowerBound().value();
//   }

//   friend std::ostream& operator<<(std::ostream& os, Range r) {
//     return os << r.getLowerBound() << ":" << r.getUpperBound();
//   }

private:
  LowerBound mLowerBound;
  UpperBound mUpperBound;

};


#endif
