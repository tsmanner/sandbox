#ifndef SpeculativeOperation_h
#define SpeculativeOperation_h

#include <functional>


template <typename OperandType> class SpecAtom;
template <typename OperandType> class SpecValue;
template <typename OperandType> class SpecOp;

template <typename OperandType> SpecOp<OperandType> operator+(const SpecAtom<OperandType>& lhs, const SpecAtom<OperandType>& rhs);
template <typename OperandType> SpecOp<OperandType> operator-(const SpecAtom<OperandType>& lhs, const SpecAtom<OperandType>& rhs);
template <typename OperandType> SpecOp<OperandType> operator*(const SpecAtom<OperandType>& lhs, const SpecAtom<OperandType>& rhs);
template <typename OperandType> SpecOp<OperandType> operator/(const SpecAtom<OperandType>& lhs, const SpecAtom<OperandType>& rhs);


template <typename OperandType>
class SpecAtom {
public:
  virtual OperandType resolve() const = 0;

private:

};


template <typename OperandType>
class SpecValue : public SpecAtom<OperandType> {
public:
  SpecValue(const OperandType& inOperand): mOperand(inOperand) {}

  virtual OperandType resolve() const { return mOperand; }

  OperandType& getOperand() { return mOperand; }

  friend std::ostream& operator<<(std::ostream& os, const SpecValue& sv) {
    return os << sv.mOperand;
  }

private:
  OperandType mOperand;

};


template <typename OperandType>
class SpecOp : public SpecAtom<OperandType> {
public:
  SpecOp(
    std::function<OperandType(const SpecAtom<OperandType>*, const SpecAtom<OperandType>*)> inOperation,
    const SpecAtom<OperandType>* inLhs,
    const SpecAtom<OperandType>* inRhs
  ):
    mOperation(inOperation),
    mLhs(inLhs),
    mRhs(inRhs)
  {
  }

  virtual OperandType resolve() const { return mOperation(getLhs(), getRhs()); }

  const SpecAtom<OperandType>* getLhs() const { return mLhs; }
  const SpecAtom<OperandType>* getRhs() const { return mRhs; }

private:
  std::function<OperandType(const SpecAtom<OperandType>*, const SpecAtom<OperandType>*)> mOperation;
  const SpecAtom<OperandType>* mLhs;
  const SpecAtom<OperandType>* mRhs;

};


template <typename OperandType>
class SpecOpAdd : public SpecOp<OperandType> {
public:
  SpecOpAdd(
    const SpecAtom<OperandType>* inLhs,
    const SpecAtom<OperandType>* inRhs
  ):
    SpecOp<OperandType>(add, inLhs, inRhs)
  {
  }

  static OperandType add(const SpecAtom<OperandType>* lhs, const SpecAtom<OperandType>* rhs) { return lhs->resolve() + rhs->resolve(); }

};


template <typename OperandType>
class SpecOpSub : public SpecOp<OperandType> {
public:
  SpecOpSub(
    const SpecAtom<OperandType>* inLhs,
    const SpecAtom<OperandType>* inRhs
  ):
    SpecOp<OperandType>(subtract, inLhs, inRhs)
  {
  }

  static OperandType subtract(const SpecAtom<OperandType>* lhs, const SpecAtom<OperandType>* rhs) { return lhs->resolve() - rhs->resolve(); }

};


template <typename OperandType>
class SpecOpMul : public SpecOp<OperandType> {
public:
  SpecOpMul(
    const SpecAtom<OperandType>* inLhs,
    const SpecAtom<OperandType>* inRhs
  ):
    SpecOp<OperandType>(multiply, inLhs, inRhs)
  {
  }

  static OperandType multiply(const SpecAtom<OperandType>* lhs, const SpecAtom<OperandType>* rhs) { return lhs->resolve() * rhs->resolve(); }

};


template <typename OperandType>
class SpecOpDiv : public SpecOp<OperandType> {
public:
  SpecOpDiv(
    const SpecAtom<OperandType>* inLhs,
    const SpecAtom<OperandType>* inRhs
  ):
    SpecOp<OperandType>(divide, inLhs, inRhs)
  {
  }

  static OperandType divide(const SpecAtom<OperandType>* lhs, const SpecAtom<OperandType>* rhs) { return lhs->resolve() / rhs->resolve(); }

};

template <typename OperandType> SpecOp<OperandType> operator+(const SpecAtom<OperandType>& lhs, const SpecAtom<OperandType>& rhs) { return SpecOpAdd<OperandType>(&lhs, &rhs); }
template <typename OperandType> SpecOp<OperandType> operator-(const SpecAtom<OperandType>& lhs, const SpecAtom<OperandType>& rhs) { return SpecOpSub<OperandType>(&lhs, &rhs); }
template <typename OperandType> SpecOp<OperandType> operator*(const SpecAtom<OperandType>& lhs, const SpecAtom<OperandType>& rhs) { return SpecOpMul<OperandType>(&lhs, &rhs); }
template <typename OperandType> SpecOp<OperandType> operator/(const SpecAtom<OperandType>& lhs, const SpecAtom<OperandType>& rhs) { return SpecOpDiv<OperandType>(&lhs, &rhs); }


#endif
