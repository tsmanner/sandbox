#ifndef SpeculativeOperation_h
#define SpeculativeOperation_h

#include <functional>
#include <ostream>
#include <sstream>
#include <string>

#include "object_ptr.h"


template <typename OperandType> class SpecAtom;
template <typename OperandType> class SpecValue;
template <typename OperandType> class SpecOp;

template <typename OperandType> SpecOp<OperandType> operator+(const SpecAtom<OperandType>& lhs, const SpecAtom<OperandType>& rhs);
template <typename OperandType> SpecOp<OperandType> operator-(const SpecAtom<OperandType>& lhs, const SpecAtom<OperandType>& rhs);
template <typename OperandType> SpecOp<OperandType> operator*(const SpecAtom<OperandType>& lhs, const SpecAtom<OperandType>& rhs);
template <typename OperandType> SpecOp<OperandType> operator/(const SpecAtom<OperandType>& lhs, const SpecAtom<OperandType>& rhs);


template <typename OperandType>
class SpecAtom : public object_ptr_interface {
public:
  virtual OperandType resolve() const = 0;
  virtual void makeConcrete() = 0;

  virtual std::string to_string() const = 0;

  friend std::ostream& operator<<(std::ostream& os, const SpecAtom& sa) {
    return os << sa.to_string();
  }


private:

};


template <typename OperandType>
class SpecValue : public SpecAtom<OperandType> {
public:
  SpecValue(const OperandType& inOperand): SpecAtom<OperandType>(), mOperand(inOperand) {}

  virtual OperandType resolve() const { return mOperand; }
  virtual void makeConcrete() { makeOperandConcrete(getOperand()); }

  OperandType& getOperand() { return mOperand; }

  virtual std::string to_string() const {
    std::stringstream ss;
    ss << mOperand;
    return ss.str();
  }

  friend std::ostream& operator<<(std::ostream& os, const SpecValue& sv) {
    return os << sv.to_string();
  }

private:
  OperandType mOperand;

};


template <typename OperandType>
class SpecOp : public SpecAtom<OperandType> {
public:
  SpecOp(
    std::function<OperandType(const SpecAtom<OperandType>*, const SpecAtom<OperandType>*)> inOperation,
    SpecAtom<OperandType>* inLhs,
    SpecAtom<OperandType>* inRhs,
    const std::string& inOperationSymbol = ' '
  ):
    SpecAtom<OperandType>(),
    mOperation(inOperation),
    mLhs(inLhs),
    mRhs(inRhs),
    mOperationSymbol(inOperationSymbol)
  {
  }

  virtual OperandType resolve() const { return mOperation(getLhs(), getRhs()); }
  virtual void makeConcrete() {
    mLhs->makeConcrete();
    mRhs->makeConcrete();
  }

  const SpecAtom<OperandType>* getLhs() const { return mLhs.get(); }
  const SpecAtom<OperandType>* getRhs() const { return mRhs.get(); }

  virtual std::string to_string() const {
    std::stringstream ss;
    ss << "(" << mLhs->to_string() << " " << mOperationSymbol << " " << mRhs->to_string() << ")";
    return ss.str();
  }

  friend std::ostream& operator<<(std::ostream& os, const SpecOp& so) {
    return os << so.to_string();
  }

private:
  std::function<OperandType(const SpecAtom<OperandType>*, const SpecAtom<OperandType>*)> mOperation;
  object_ptr<SpecAtom<OperandType>> mLhs;
  object_ptr<SpecAtom<OperandType>> mRhs;
  std::string mOperationSymbol;

};


template <typename OperandType> object_ptr<SpecAtom<OperandType>> operator+(object_ptr<SpecAtom<OperandType>> lhs, object_ptr<SpecAtom<OperandType>> rhs) { return object_ptr<SpecAtom<OperandType>>(new SpecOp<OperandType>([](const SpecAtom<OperandType>* lhs, const SpecAtom<OperandType>* rhs) -> OperandType { return lhs->resolve() + rhs->resolve(); }, lhs.get(), rhs.get(), "+")); }
template <typename OperandType> object_ptr<SpecAtom<OperandType>> operator-(object_ptr<SpecAtom<OperandType>> lhs, object_ptr<SpecAtom<OperandType>> rhs) { return object_ptr<SpecAtom<OperandType>>(new SpecOp<OperandType>([](const SpecAtom<OperandType>* lhs, const SpecAtom<OperandType>* rhs) -> OperandType { return lhs->resolve() - rhs->resolve(); }, lhs.get(), rhs.get(), "-")); }
template <typename OperandType> object_ptr<SpecAtom<OperandType>> operator*(object_ptr<SpecAtom<OperandType>> lhs, object_ptr<SpecAtom<OperandType>> rhs) { return object_ptr<SpecAtom<OperandType>>(new SpecOp<OperandType>([](const SpecAtom<OperandType>* lhs, const SpecAtom<OperandType>* rhs) -> OperandType { return lhs->resolve() * rhs->resolve(); }, lhs.get(), rhs.get(), "*")); }
template <typename OperandType> object_ptr<SpecAtom<OperandType>> operator/(object_ptr<SpecAtom<OperandType>> lhs, object_ptr<SpecAtom<OperandType>> rhs) { return object_ptr<SpecAtom<OperandType>>(new SpecOp<OperandType>([](const SpecAtom<OperandType>* lhs, const SpecAtom<OperandType>* rhs) -> OperandType { return lhs->resolve() / rhs->resolve(); }, lhs.get(), rhs.get(), "/")); }

template <typename OperandType> object_ptr<SpecAtom<OperandType>> max(object_ptr<SpecAtom<OperandType>> lhs, object_ptr<SpecAtom<OperandType>> rhs) { return object_ptr<SpecAtom<OperandType>>(new SpecOp<OperandType>([](const SpecAtom<OperandType>* lhs, const SpecAtom<OperandType>* rhs) -> OperandType { return max(lhs->resolve(), rhs->resolve()); }, lhs.get(), rhs.get(), "max")); }
template <typename OperandType> object_ptr<SpecAtom<OperandType>> min(object_ptr<SpecAtom<OperandType>> lhs, object_ptr<SpecAtom<OperandType>> rhs) { return object_ptr<SpecAtom<OperandType>>(new SpecOp<OperandType>([](const SpecAtom<OperandType>* lhs, const SpecAtom<OperandType>* rhs) -> OperandType { return min(lhs->resolve(), rhs->resolve()); }, lhs.get(), rhs.get(), "min")); }


template <typename OperandType, typename... OperandTypes>
typename std::enable_if<(sizeof...(OperandTypes) == 0), object_ptr<SpecAtom<OperandType>>>::type
max(object_ptr<SpecAtom<OperandType>> lhs, object_ptr<SpecAtom<OperandType>> rhs, OperandTypes... inOperands) {
  return max(lhs, rhs);
}


template <typename OperandType, typename... OperandTypes>
typename std::enable_if<(sizeof...(OperandTypes) != 0), object_ptr<SpecAtom<OperandType>>>::type
max(object_ptr<SpecAtom<OperandType>> lhs, object_ptr<SpecAtom<OperandType>> rhs, OperandTypes... inOperands) {
  return max(max(lhs, rhs), inOperands...);
}


template <typename OperandType, typename... OperandTypes>
typename std::enable_if<(sizeof...(OperandTypes) == 0), object_ptr<SpecAtom<OperandType>>>::type
min(object_ptr<SpecAtom<OperandType>> lhs, object_ptr<SpecAtom<OperandType>> rhs, OperandTypes... inOperands) {
  return min(lhs, rhs);
}


template <typename OperandType, typename... OperandTypes>
typename std::enable_if<(sizeof...(OperandTypes) != 0), object_ptr<SpecAtom<OperandType>>>::type
min(object_ptr<SpecAtom<OperandType>> lhs, object_ptr<SpecAtom<OperandType>> rhs, OperandTypes... inOperands) {
  return min(min(lhs, rhs), inOperands...);
}


#endif
