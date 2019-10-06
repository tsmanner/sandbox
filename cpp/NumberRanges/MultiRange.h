#ifndef MultiRange_h
#define MultiRange_h

#include <set>
#include <utility>

#include "Range.h"


class MultiRange {
public:
  using RangeSet = std::set<Range, Range::Compare>;

  RangeSet& getRanges() { return mRanges; }

  void addRange(Range r) {
    mRanges.insert(r);
    optimize();
  }

  template <typename ValueType>
  bool covers(const ValueType& inValue) {
    for (auto range : mRanges) {
      if (range.covers(inValue)) {
        return true;
      }
    }
    return false;
  }

  void optimize() {
    // Creates a new optimized set of ranges from the disjunction
    // of all Range instances in mRanges
    //
    // Start at the beginning of the set (with the lowest lower bound)
    // and scan, iteratively building the disjunction of each overlapping
    // Range.  Once a non-overlapping range is encountered, insert the
    // new one into the optimized set and select the non-overlapping range
    // as the new base range.  Repeat until done.
    RangeSet optimizedRanges;
    auto iter = mRanges.begin();
    auto optimizedRange = *iter;

    for (; iter != mRanges.end(); ++iter) {
      if (
        Range::overlaps(optimizedRange, *iter)
        or
        Range::adjacent(optimizedRange, *iter)
      ) {
        optimizedRange = Range::disjunction(optimizedRange, *iter);
      }
      else {
        optimizedRanges.insert(optimizedRange);
        optimizedRange = *iter;
      }
    }
    optimizedRanges.insert(optimizedRange);
    mRanges = optimizedRanges;
  }

private:
  RangeSet mRanges;

};


#endif
