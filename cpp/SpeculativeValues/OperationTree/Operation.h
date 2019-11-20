#ifndef Operation_h
#define Operation_h

#include <functional>
#include <sstream>
#include <string>

#include "Node.h"


template <typename T>
class Operation : public Node<T> {
public:

  Operation(
    std::string inOperationSymbol,
    std::function<T(const T&, const T&)> inOperation,
    object_ptr<Node<T>> inLhs,
    object_ptr<Node<T>> inRhs
  ):
    Operation(inOperationSymbol, inOperation, inLhs.get(), inRhs.get())
  {
  }

  Operation(
    std::string inOperationSymbol,
    std::function<T(const T&, const T&)> inOperation,
    Node<T>* inLhs,
    object_ptr<Node<T>> inRhs
  ):
    Operation(inOperationSymbol, inOperation, inLhs, inRhs.get())
  {
  }

  Operation(
    std::string inOperationSymbol,
    std::function<T(const T&, const T&)> inOperation,
    object_ptr<Node<T>> inLhs,
    Node<T>* inRhs
  ):
    Operation(inOperationSymbol, inOperation, inLhs.get(), inRhs)
  {
  }

  Operation(
    std::string inOperationSymbol,
    std::function<T(const T&, const T&)> inOperation,
    Node<T>* inLhs,
    Node<T>* inRhs
  ):
    Node<T>(),
    mOperationSymbol(inOperationSymbol),
    mOperation(inOperation),
    mLhs(inLhs),
    mRhs(inRhs)
  {
    mLhs->addOnRealFunction(std::bind(&Operation::makeReal, this));
    mRhs->addOnRealFunction(std::bind(&Operation::makeReal, this));
  }

  virtual ~Operation() {}

  virtual void makeReal() {
    if (mLhs->isReal() and mRhs->isReal()) {
      this->setValue(mOperation(mLhs->getValue(), mRhs->getValue()));
    }
  }

  virtual std::string to_string(bool elaborate = false) {
    if (elaborate or !this->isReal()) {
      std::stringstream ss;
      ss << "(" << mLhs->to_string(elaborate) << " " << mOperationSymbol << " " << mRhs->to_string(elaborate) << ")";
      return ss.str();
    }
    return Node<T>::to_string(elaborate);
  }

private:
  std::string mOperationSymbol;
  std::function<T(const T&, const T&)> mOperation;
  object_ptr<Node<T>> mLhs = object_ptr<Node<T>>();
  object_ptr<Node<T>> mRhs = object_ptr<Node<T>>();

};


#endif
