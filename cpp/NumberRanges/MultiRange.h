#ifndef MultiRange_h
#define MultiRange_h

#include <set>
#include <utility>

#include "Range.h"


class MultiRange {
public:
  using RangeSet = std::set<Range, Range::Compare>;

  RangeSet& getRanges() { return mRanges; }
  const RangeSet& getRanges() const { return mRanges; }

  template <typename ValueType>
  bool covers(const ValueType& inValue) {
    for (auto range : mRanges) {
      if (range.covers(inValue)) {
        return true;
      }
    }
    return false;
  }

  void addRange(Range r) {
    mRanges.insert(r);
    optimize();
  }

  void optimize() {
    mRanges = disjunction(mRanges);
  }

  static RangeSet disjunction(const RangeSet& inRangeSet) {
    // Creates a new optimized set of ranges from the disjunction
    // of all overlapping or adjacent Range instances in mRanges
    //
    // Start at the beginning of the set (with the lowest lower bound)
    // and scan, iteratively building the disjunction of each overlapping
    // Range.  Once a non-overlapping range is encountered, insert the
    // new one into the optimized set and select the non-overlapping range
    // as the new base range.  Repeat until done.
    RangeSet disjunctionRangeSet;
    auto iter = inRangeSet.begin();
    auto disjunctionRange = *iter;

    for (; iter != inRangeSet.end(); ++iter) {
      if (
        Range::overlaps(disjunctionRange, *iter)
        or
        Range::adjacent(disjunctionRange, *iter)
      ) {
        disjunctionRange = Range::disjunction(disjunctionRange, *iter);
      }
      else {
        disjunctionRangeSet.insert(disjunctionRange);
        disjunctionRange = *iter;
      }
    }
    disjunctionRangeSet.insert(disjunctionRange);
    return disjunctionRangeSet;
  }

  static RangeSet conjunction(const RangeSet& inRangeSet) {
    // Creates a new optimized set of ranges from the conjunction
    // of all Range instances in mRanges
    //
    // Start at the beginning of the set (with the lowest lower bound)
    // and scan, iteratively building the conjunction of each overlapping
    // Range.  If a non-overlapping range is encountered, the conjunction
    // of all Ranges is empty, so immediately return an empty set.
    RangeSet conjunctionRangeSet;
    auto iter = inRangeSet.begin();
    auto conjunctionRange = *iter;

    for (; iter != inRangeSet.end(); ++iter) {
      if (Range::overlaps(conjunctionRange, *iter)) {
        conjunctionRange = Range::conjunction(conjunctionRange, *iter);
      }
      else {
        return RangeSet();
      }
    }
    conjunctionRangeSet.insert(conjunctionRange);
    return conjunctionRangeSet;
  }

  static MultiRange disjunction(const MultiRange& lhs, const MultiRange& rhs) {
    auto disjunctionMultiRange = MultiRange();
    disjunctionMultiRange.mRanges.insert(lhs.mRanges.begin(), lhs.mRanges.end());
    disjunctionMultiRange.mRanges.insert(rhs.mRanges.begin(), rhs.mRanges.end());
    disjunctionMultiRange.optimize();
    return disjunctionMultiRange;
  }

  static MultiRange conjunction(const MultiRange& lhs, const MultiRange& rhs) {
    auto conjunctionMultiRange = MultiRange();
    for (auto lhsRange : lhs.mRanges) {
      for (auto rhsRange : rhs.mRanges) {
        if (Range::overlaps(lhsRange, rhsRange)) {
          conjunctionMultiRange.addRange(Range::conjunction(lhsRange, rhsRange));
        }
      }
    }
    return conjunctionMultiRange;
  }

private:
  RangeSet mRanges;

};


#endif
