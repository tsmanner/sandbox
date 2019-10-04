#ifndef Bound_h
#define Bound_h
#include <type_traits>

class Bound {
public:
  class Open {};
  class Closed {};

  enum eType {
    cClosed
  , cOpen
  };

  Bound(): mValue(), mType(cOpen) {}
  Bound(const int& t): mValue(t), mType(cClosed) {}

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
  LowerBound(): Bound() {}
  LowerBound(const int& inValue): Bound(inValue) {}

  void operator=(const int& inValue) { set(inValue); }

  // Equal
  inline friend bool operator==(LowerBound lhs, const int& rhs) { return lhs.getType() != Bound::cOpen and lhs.getValue() == rhs; }
  inline friend bool operator==(const int& lhs, LowerBound rhs) { return operator==(rhs, lhs); }

  // Not Equal
  inline friend bool operator!=(LowerBound lhs, const int& rhs) { return !(lhs == rhs); }
  inline friend bool operator!=(const int& lhs, LowerBound rhs) { return !(lhs == rhs); }

  // Less Than
  inline friend bool operator<(LowerBound lhs, const int& rhs) { return lhs.getType() == Bound::cOpen or lhs.getValue() < rhs; }
  inline friend bool operator<(const int& lhs, LowerBound rhs) { return rhs.getType() != Bound::cOpen and lhs < rhs.getValue(); }

  // Less Than or Equal
  inline friend bool operator<=(LowerBound lhs, const int& rhs) { return (lhs < rhs) or (lhs == rhs); }
  inline friend bool operator<=(const int& lhs, LowerBound rhs) { return (lhs < rhs) or (lhs == rhs); }

  // Greater Than
  inline friend bool operator>(LowerBound lhs, const int& rhs) { return !(rhs <= lhs); }
  inline friend bool operator>(const int& lhs, LowerBound rhs) { return !(rhs <= lhs); }

  // Greater Than or Equal
  inline friend bool operator>=(LowerBound lhs, const int& rhs) { return !(rhs < lhs); }
  inline friend bool operator>=(const int& lhs, LowerBound rhs) { return !(rhs < lhs); }

};


class UpperBound : public Bound {
public:
  UpperBound(): Bound() {}
  UpperBound(const int& inValue): Bound(inValue) {}

  void operator=(const int& inValue) { set(inValue); }

  // Equal
  inline friend bool operator==(UpperBound lhs, const int& rhs) { return lhs.getType() != Bound::cOpen and lhs.getValue() == rhs; }
  inline friend bool operator==(const int& lhs, UpperBound rhs) { return operator==(rhs, lhs); }

  // Not Equal
  inline friend bool operator!=(UpperBound lhs, const int& rhs) { return !(lhs == rhs); }
  inline friend bool operator!=(const int& lhs, UpperBound rhs) { return !(lhs == rhs); }

  // Less Than
  inline friend bool operator<(UpperBound lhs, const int& rhs) { return lhs.getType() != Bound::cOpen and lhs.getValue() < rhs; }
  inline friend bool operator<(const int& lhs, UpperBound rhs) { return rhs.getType() == Bound::cOpen or lhs < rhs.getValue(); }

  // Less Than or Equal
  inline friend bool operator<=(UpperBound lhs, const int& rhs) { return (lhs < rhs) or (lhs == rhs); }
  inline friend bool operator<=(const int& lhs, UpperBound rhs) { return (lhs < rhs) or (lhs == rhs); }

  // Greater Than
  inline friend bool operator>(UpperBound lhs, const int& rhs) { return !(rhs <= lhs); }
  inline friend bool operator>(const int& lhs, UpperBound rhs) { return !(rhs <= lhs); }

  // Greater Than or Equal
  inline friend bool operator>=(UpperBound lhs, const int& rhs) { return !(rhs < lhs); }
  inline friend bool operator>=(const int& lhs, UpperBound rhs) { return !(rhs < lhs); }

};


inline bool operator<(LowerBound lhs, LowerBound rhs) {
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


inline bool operator<(LowerBound lhs, UpperBound rhs) {
  // Either is open ended, the lower bound is less
  if (
    lhs.getType() == Bound::cOpen or
    rhs.getType() == Bound::cOpen
  ) {
    return true;
  }
  else return lhs.getValue() < rhs.getValue();
}


inline bool operator<(UpperBound lhs, LowerBound rhs) {
  // Either is open ended, the lower bound is less
  if (
    lhs.getType() == Bound::cOpen or
    rhs.getType() == Bound::cOpen
  ) {
    return false;
  }
  else return lhs.getValue() < rhs.getValue();
}


inline bool operator<(UpperBound lhs, UpperBound rhs) {
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


#endif
