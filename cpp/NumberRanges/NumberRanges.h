#ifndef NumberRanges_h
#define NumberRanges_h

#include <exception>
#include <iostream>
#include <set>
#include <utility>


template <typename T>
class Optional {
public:
  Optional(): mValue() {}
  Optional(const T& t): mValue(t), mHasValue(true) {}

  void operator=(const T& t) { mValue = t; mHasValue = true; }

  const bool& has_value() const { return mHasValue; }
  operator T&() { return value(); }
  T& value() {
    if (has_value()) {
      return mValue;
    }
    throw std::exception();
  }

private:
  T mValue;
  bool mHasValue;

};


class Range {
public:
  using Bound = Optional<int>;
  enum eNoBound { cNoBound };

  Range(): mLowerBound(), mUpperBound() {}
  Range(eNoBound start, const int& end): mLowerBound(), mUpperBound(end) {}
  Range(const int& start, eNoBound end): mLowerBound(start), mUpperBound() {}
  Range(const int& start, const int& end): mLowerBound(start), mUpperBound(end) {}

  Bound& getLowerBound() { return mLowerBound; }
  void setLowerBound(const int& inBound) { mLowerBound = inBound; }

  Bound& getUpperBound() { return mUpperBound; }
  void setUpperBound(const int& inBound) { mUpperBound = inBound; }

  bool covers(const int& value) {
    return
      (!getLowerBound().has_value() and !getUpperBound().has_value()) or
      (!getLowerBound().has_value() and value <= getUpperBound()    ) or
      ( getLowerBound() <= value    and !getUpperBound().has_value()) or
      ( getLowerBound() <= value    and value <= getUpperBound()    )
    ;
  }

  static bool overlaps(Range lhs, Range rhs) {
    // Overlap is guaranteed if either range has no bounds,
    // or if both ranges are missing the same bound
    if (
      (!lhs.getLowerBound().has_value() and !lhs.getUpperBound().has_value())
      or
      (!rhs.getLowerBound().has_value() and !rhs.getUpperBound().has_value())
      or
      (!lhs.getLowerBound().has_value() and !rhs.getLowerBound().has_value())
      or
      (!lhs.getUpperBound().has_value() and !rhs.getUpperBound().has_value())
    ) {
      return true;
    }

    if (
      lhs.getLowerBound().has_value() and
      lhs.getUpperBound().has_value() and
      rhs.getLowerBound().has_value() and
      rhs.getUpperBound().has_value()
    ) {
      return
        lhs.covers(rhs.getLowerBound()) or
        lhs.covers(rhs.getUpperBound()) or
        rhs.covers(lhs.getLowerBound()) or
        rhs.covers(lhs.getUpperBound())
      ;
    }

    if (
      lhs.getUpperBound().has_value() and rhs.getLowerBound().has_value()
    ) {
      return rhs.getLowerBound() <= lhs.getUpperBound();
    }

    if (
      lhs.getLowerBound().has_value() and rhs.getUpperBound().has_value()
    ) {
      return lhs.getLowerBound() <= rhs.getUpperBound();
    }

    return false; // Is getting here a problem?
  }

  static Range merge(Range lhs, Range rhs) {
    auto newRange = Range();

    // If this has no lower bound, just take rhs's lower bound
    if (!lhs.getLowerBound().has_value()) {
      newRange.getLowerBound() = rhs.getLowerBound();
    }
    // If we both have lhs lower bound, take the higher one
    else if (lhs.getLowerBound().has_value() and rhs.getLowerBound().has_value()) {
      newRange.getLowerBound() = std::max(lhs.getLowerBound().value(), rhs.getLowerBound().value());
    }

    // If this has no upper bound, just take rhs's upper bound
    if (!lhs.getUpperBound().has_value()) {
      newRange.getUpperBound() = rhs.getUpperBound();
    }
    // If we both have lhs upper bound, take the lower one
    else if (lhs.getUpperBound().has_value() and rhs.getUpperBound().has_value()) {
      newRange.getUpperBound() = std::min(lhs.getUpperBound().value(), rhs.getUpperBound().value());
    }

    return newRange;
  }

  friend bool operator<(Range lhs,Range rhs) {
    // lhs: Has no lower bound
    // rhs: Has lower bound
    // lhs < rhs
    if (!lhs.getLowerBound().has_value() and rhs.getLowerBound().has_value()) return true;
    // lhs: Has lower bound
    // rhs: Has no lower bound
    // rhs < lhs
    if (lhs.getLowerBound().has_value() and !rhs.getLowerBound().has_value()) return false;
    // lhs: Has no lower bound
    // rhs: Has no lower bound
    // lhs == rhs
    if (!lhs.getLowerBound().has_value() and !rhs.getLowerBound().has_value()) return false;
    // lhs: Has lower bound
    // rhs: Has lower bound
    // value compare
    return lhs.getLowerBound().value() < rhs.getLowerBound().value();
  }

  friend std::ostream& operator<<(std::ostream& os, Range r) {
    return os << r.getLowerBound() << ":" << r.getUpperBound();
  }

private:
  Bound mLowerBound;
  Bound mUpperBound;

};


class MultiRange {
public:

  std::set<Range>& getRanges() { return mRanges; }

  void addRange(Range r) { mRanges.insert(r); optimize(); }

private:
  void merge(Range lhs, Range rhs) {
    mRanges.erase(lhs);
    mRanges.erase(rhs);
    mRanges.insert(Range::merge(lhs, rhs));
  }

  void optimize() {
    bool merged;
    do {
      merged = false;
      for (auto iter = mRanges.begin(); iter != mRanges.end(); ) {
        auto current = *iter++;
        if (iter != mRanges.end() and Range::overlaps(current, *iter)) {
          merge(current, *iter);
          merged = true;
          break;
        }
      }
    } while (merged);
  }

  std::set<Range> mRanges;

};


#endif
