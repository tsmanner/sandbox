#ifndef Range_h
#define Range_h

#include "Bound.h"


class Range {
public:
  struct NonOverlappingRangeConjunction : std::exception {
    const char* what() { return "Attempt to calculate the conjunction of two non-overlapping Ranges"; }
  };

  struct NonOverlappingRangeDisjunction : std::exception {
    const char* what() { return "Attempt to calculate the disjunction of two non-overlapping Ranges"; }
  };

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

  template <typename ValueType>
  bool covers(ValueType value) {
    return getLowerBound() <= value and value <= getUpperBound();
  }

  static bool overlaps(Range lhs, Range rhs) {
    return lhs.covers(rhs.getLowerBound()) or
           lhs.covers(rhs.getUpperBound()) or
           rhs.covers(lhs.getUpperBound()) or
           rhs.covers(lhs.getUpperBound());
  }

  // Logical Conjunction - the set of elements in both
  static Range conjunction(Range lhs, Range rhs) {
    if (overlaps(lhs, rhs)) {
      return Range(
        std::max(lhs.getLowerBound(), rhs.getLowerBound()),
        std::min(lhs.getUpperBound(), rhs.getUpperBound())
      );
    }
    throw NonOverlappingRangeConjunction();
  }

  // Logical Disjunction - the set of elements in either
  static Range disjunction(Range lhs, Range rhs) {
    if (overlaps(lhs, rhs)) {
      return Range(
        std::min(lhs.getLowerBound(), rhs.getLowerBound()),
        std::max(lhs.getUpperBound(), rhs.getUpperBound())
      );
    }
    throw NonOverlappingRangeDisjunction();
  }


  friend inline bool sort_compare(Range lhs,Range rhs) { return lhs.getLowerBound() < rhs.getLowerBound(); }


  friend std::ostream& operator<<(std::ostream& os, Range r) {
    os << "[";
    if (r.getLowerBound().getType() == Bound::cClosed) {
      os << r.getLowerBound().getValue();
    }
    os << ":";
    if (r.getUpperBound().getType() == Bound::cClosed) {
      os << r.getUpperBound().getValue();
    }
    os << "]";
    return os;
  }

private:
  LowerBound mLowerBound;
  UpperBound mUpperBound;

};


#endif
