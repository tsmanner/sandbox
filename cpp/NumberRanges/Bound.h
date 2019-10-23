#ifndef Bound_h
#define Bound_h

#include <limits>
#include <type_traits>


class Bound {
public:
  enum eType {
    cClosed
  , cOpen
  };

  Bound(const int& t, const eType& inType): mValue(t), mType(inType) {}

  void operator=(const int& inValue) { set(inValue); }

  template <eType BoundType>
  typename std::enable_if<(BoundType == cOpen), void>::type
  set() {
    mType = BoundType;
  }

  template <eType BoundType>
  typename std::enable_if<(BoundType == cClosed), void>::type
  set(const int& inValue) {
    mType = BoundType;
    mValue = inValue;
  }

  void set(const int& inValue) {
    set<cClosed>(inValue);
  }

  const eType& getType() const { return mType; }
  const int& getValue() const { return mValue; }

private:
  int mValue;
  eType mType;

};


class LowerBound : public Bound {
public:
  LowerBound(): Bound(std::numeric_limits<int>::min(), Bound::cOpen) {}
  LowerBound(const int& inValue): Bound(inValue, Bound::cClosed) {}

  void operator=(const int& inValue) { set(inValue); }

  // Equal
  inline friend bool operator==(const LowerBound& lhs, const int& rhs) { return lhs.getType() != Bound::cOpen and lhs.getValue() == rhs; }
  inline friend bool operator==(const int& lhs, const LowerBound& rhs) { return operator==(rhs, lhs); }

  // Not Equal
  inline friend bool operator!=(const LowerBound& lhs, const int& rhs) { return !(lhs == rhs); }
  inline friend bool operator!=(const int& lhs, const LowerBound& rhs) { return !(lhs == rhs); }

  // Less Than
  inline friend bool operator<(const LowerBound& lhs, const int& rhs) { return lhs.getType() == Bound::cOpen or lhs.getValue() < rhs; }
  inline friend bool operator<(const int& lhs, const LowerBound& rhs) { return rhs.getType() != Bound::cOpen and lhs < rhs.getValue(); }

  // Less Than or Equal
  inline friend bool operator<=(const LowerBound& lhs, const int& rhs) { return (lhs < rhs) or (lhs == rhs); }
  inline friend bool operator<=(const int& lhs, const LowerBound& rhs) { return (lhs < rhs) or (lhs == rhs); }

  // Greater Than
  inline friend bool operator>(const LowerBound& lhs, const int& rhs) { return !(lhs <= rhs); }
  inline friend bool operator>(const int& lhs, const LowerBound& rhs) { return !(lhs <= rhs); }

  // Greater Than or Equal
  inline friend bool operator>=(const LowerBound& lhs, const int& rhs) { return !(lhs < rhs); }
  inline friend bool operator>=(const int& lhs, const LowerBound& rhs) { return !(lhs < rhs); }

};


class UpperBound : public Bound {
public:
  UpperBound(): Bound(std::numeric_limits<int>::max(), Bound::cOpen) {}
  UpperBound(const int& inValue): Bound(inValue, Bound::cClosed) {}

  void operator=(const int& inValue) { set(inValue); }

  // Equal
  inline friend bool operator==(const UpperBound& lhs, const int& rhs) { return lhs.getType() != Bound::cOpen and lhs.getValue() == rhs; }
  inline friend bool operator==(const int& lhs, const UpperBound& rhs) { return operator==(rhs, lhs); }

  // Not Equal
  inline friend bool operator!=(const UpperBound& lhs, const int& rhs) { return !(lhs == rhs); }
  inline friend bool operator!=(const int& lhs, const UpperBound& rhs) { return !(lhs == rhs); }

  // Less Than
  inline friend bool operator<(const UpperBound& lhs, const int& rhs) { return lhs.getType() != Bound::cOpen and lhs.getValue() < rhs; }
  inline friend bool operator<(const int& lhs, const UpperBound& rhs) { return rhs.getType() == Bound::cOpen or lhs < rhs.getValue(); }

  // Less Than or Equal
  inline friend bool operator<=(const UpperBound& lhs, const int& rhs) { return (lhs < rhs) or (lhs == rhs); }
  inline friend bool operator<=(const int& lhs, const UpperBound& rhs) { return (lhs < rhs) or (lhs == rhs); }

  // Greater Than
  inline friend bool operator>(const UpperBound& lhs, const int& rhs) { return !(lhs <= rhs); }
  inline friend bool operator>(const int& lhs, const UpperBound& rhs) { return !(lhs <= rhs); }

  // Greater Than or Equal
  inline friend bool operator>=(const UpperBound& lhs, const int& rhs) { return !(lhs < rhs); }
  inline friend bool operator>=(const int& lhs, const UpperBound& rhs) { return !(lhs < rhs); }

};


//
// Equal
//


inline bool operator==(const LowerBound& lhs, const LowerBound& rhs) {
  // Both are open or both have the same value
  return (lhs.getType() == Bound::cOpen and rhs.getType() == Bound::cOpen)
         or
         (lhs.getValue() == rhs.getValue());
}


inline bool operator==(const LowerBound& lhs, const UpperBound& rhs) {
  // Neither is open and they have the same value
  return lhs.getType() != Bound::cOpen
         and
         rhs.getType() != Bound::cOpen
         and
         lhs.getValue() == rhs.getValue();
}


inline bool operator==(const UpperBound& lhs, const LowerBound& rhs) {
  return rhs == lhs;
}


inline bool operator==(const UpperBound& lhs, const UpperBound& rhs) {
  // Both are open or both have the same value
  return (lhs.getType() == Bound::cOpen and rhs.getType() == Bound::cOpen)
         or
         (lhs.getValue() == rhs.getValue());
}


//
// Not Equal
//


inline bool operator!=(const LowerBound& lhs, const LowerBound& rhs) { return !(lhs == rhs); }
inline bool operator!=(const LowerBound& lhs, const UpperBound& rhs) { return !(lhs == rhs); }
inline bool operator!=(const UpperBound& lhs, const LowerBound& rhs) { return !(lhs == rhs); }
inline bool operator!=(const UpperBound& lhs, const UpperBound& rhs) { return !(lhs == rhs); }


//
// Less Than
//


inline bool operator<(const LowerBound& lhs, const LowerBound& rhs) {
  // Both open ended lower: they're equal
  if (
    lhs.getType() == Bound::cOpen and
    rhs.getType() == Bound::cOpen
  ) {
    return false;
  }
  else if (lhs.getType() == Bound::cOpen) return true;
  else if (rhs.getType() == Bound::cOpen) return false;
  else return lhs.getValue() < rhs.getValue();
}


inline bool operator<(const LowerBound& lhs, const UpperBound& rhs) {
  // Either is open ended, the lower bound is less
  if (
    lhs.getType() == Bound::cOpen or
    rhs.getType() == Bound::cOpen
  ) {
    return true;
  }
  else return lhs.getValue() < rhs.getValue();
}


inline bool operator<(const UpperBound& lhs, const LowerBound& rhs) {
  // Either is open ended, the lower bound is less
  if (
    lhs.getType() == Bound::cOpen or
    rhs.getType() == Bound::cOpen
  ) {
    return false;
  }
  else return lhs.getValue() < rhs.getValue();
}


inline bool operator<(const UpperBound& lhs, const UpperBound rhs) {
  // Both open ended lower: they're equal
  if (
    lhs.getType() == Bound::cOpen and
    rhs.getType() == Bound::cOpen
  ) {
    return false;
  }
  else if (lhs.getType() == Bound::cOpen) return false;
  else if (rhs.getType() == Bound::cOpen) return true;
  else return lhs.getValue() < rhs.getValue();
}


//
// Less Than or Equal
//


inline bool operator<=(const LowerBound& lhs, const LowerBound& rhs) { return (lhs == rhs) or (lhs < rhs); }
inline bool operator<=(const LowerBound& lhs, const UpperBound& rhs) { return (lhs == rhs) or (lhs < rhs); }
inline bool operator<=(const UpperBound& lhs, const LowerBound& rhs) { return (lhs == rhs) or (lhs < rhs); }
inline bool operator<=(const UpperBound& lhs, const UpperBound& rhs) { return (lhs == rhs) or (lhs < rhs); }


//
// Greater Than
//


inline bool operator>(const LowerBound& lhs, const LowerBound& rhs) { return !(lhs <= rhs); }
inline bool operator>(const LowerBound& lhs, const UpperBound& rhs) { return !(lhs <= rhs); }
inline bool operator>(const UpperBound& lhs, const LowerBound& rhs) { return !(lhs <= rhs); }
inline bool operator>(const UpperBound& lhs, const UpperBound& rhs) { return !(lhs <= rhs); }


//
// Greater Than or Equal
//


inline bool operator>=(const LowerBound& lhs, const LowerBound& rhs) { return !(lhs < rhs); }
inline bool operator>=(const LowerBound& lhs, const UpperBound& rhs) { return !(lhs < rhs); }
inline bool operator>=(const UpperBound& lhs, const LowerBound& rhs) { return !(lhs < rhs); }
inline bool operator>=(const UpperBound& lhs, const UpperBound& rhs) { return !(lhs < rhs); }


#endif
