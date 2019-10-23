#ifndef Range_h
#define Range_h

#include <limits>
#include <ostream>

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

  const LowerBound& getLowerBound() const { return mLowerBound; }
  const UpperBound& getUpperBound() const { return mUpperBound; }

  template <typename ValueType>
  bool covers(ValueType inValue) {
    return getLowerBound() <= inValue and inValue <= getUpperBound();
  }

  static bool adjacent(Range lhs, Range rhs) {
    if (
      lhs.getUpperBound().getType() == Bound::cClosed and
      rhs.getLowerBound().getType() == Bound::cClosed and
      lhs.getUpperBound().getValue() + 1 == rhs.getLowerBound().getValue()
    ) {
      return true;
    }
    if (
      rhs.getUpperBound().getType() == Bound::cClosed and
      lhs.getLowerBound().getType() == Bound::cClosed and
      rhs.getUpperBound().getValue() + 1 == lhs.getLowerBound().getValue()
    ) {
      return true;
    }
    if (
      lhs.getLowerBound().getType() == Bound::cClosed and
      rhs.getUpperBound().getType() == Bound::cClosed and
      lhs.getLowerBound().getValue() + 1 == rhs.getUpperBound().getValue()
    ) {
      return true;
    }
    if (
      rhs.getLowerBound().getType() == Bound::cClosed and
      lhs.getUpperBound().getType() == Bound::cClosed and
      rhs.getLowerBound().getValue() + 1 == lhs.getUpperBound().getValue()
    ) {
      return true;
    }
    return false;
  }

  static bool overlaps(Range lhs, Range rhs) {
    return lhs.covers(rhs.getLowerBound()) or
           lhs.covers(rhs.getUpperBound()) or
           rhs.covers(lhs.getUpperBound()) or
           rhs.covers(lhs.getUpperBound());
  }

  // Logical Conjunction - the set of elements in both
  // Ranges must overlap
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
  // Ranges must overlap or be adjacent
  static Range disjunction(Range lhs, Range rhs) {
    if (adjacent(lhs, rhs) or overlaps(lhs, rhs)) {
      return Range(
        std::min(lhs.getLowerBound(), rhs.getLowerBound()),
        std::max(lhs.getUpperBound(), rhs.getUpperBound())
      );
    }
    throw NonOverlappingRangeDisjunction();
  }

  struct Compare {
    bool operator()(Range lhs,Range rhs) {
      if (lhs.getLowerBound() == rhs.getLowerBound()) {
        return lhs.getUpperBound() < rhs.getUpperBound();
      }
      return lhs.getLowerBound() < rhs.getLowerBound();
    }
  };

  size_t size() {
    if (
      getLowerBound().getType() == Bound::cOpen and
      getUpperBound().getType() == Bound::cOpen
    ) {
      return size_t(1) + std::numeric_limits<int>::max() - std::numeric_limits<int>::min();
    }
    else if(
      getLowerBound().getType() == Bound::cOpen
    ) {
      return size_t(1) + getUpperBound().getValue() - std::numeric_limits<int>::min();
    }
    else if(
      getUpperBound().getType() == Bound::cOpen
    ) {
      return size_t(1) + std::numeric_limits<int>::max() - getLowerBound().getValue();
    }
    return size_t(1) + getUpperBound().getValue() - getLowerBound().getValue();
  }

  friend std::ostream& operator<<(std::ostream& os, Range r) {
    os << "[";
    if (r.getLowerBound().getType() == Bound::cClosed) {
      os << r.getLowerBound().getValue();
    }
    if (r.getLowerBound() != r.getUpperBound()) {
      os << ":";
      if (r.getUpperBound().getType() == Bound::cClosed) {
        os << r.getUpperBound().getValue();
      }
    }
    os << "]";
    return os;
  }

private:
  LowerBound mLowerBound;
  UpperBound mUpperBound;

};


#endif
