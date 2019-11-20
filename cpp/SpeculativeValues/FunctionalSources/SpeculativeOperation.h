#ifndef SpeculativeOperation_h
#define SpeculativeOperation_h

#include <functional>
#include <ostream>
#include <sstream>
#include <string>

#include "object_ptr.h"

#include "SpeculativeNode.h"


template <typename T>
class SpeculativeOperation : public SpeculativeNode<T> {
public:
  // int        int
  SpeculativeOperation(
    const std::string& inSymbol,
    std::function<T(const T&, const T&)> inFunction,
    const T& inLhs,
    const T& inRhs
  ):
    SpeculativeOperation(inSymbol, inFunction, new SpeculativeLiteral<T>(inLhs), new SpeculativeLiteral<T>(inRhs))
  {
  }

  // int        Node*
  SpeculativeOperation(
    const std::string& inSymbol,
    std::function<T(const T&, const T&)> inFunction,
    const T& inLhs,
    SpeculativeNode<T>* inRhs
  ):
    SpeculativeOperation(inSymbol, inFunction, new SpeculativeLiteral<T>(inLhs), inRhs)
  {
  }

  // int        object_ptr
  SpeculativeOperation(
    const std::string& inSymbol,
    std::function<T(const T&, const T&)> inFunction,
    const T& inLhs,
    object_ptr<SpeculativeNode<T>> inRhs
  ):
    SpeculativeOperation(inSymbol, inFunction, inLhs, inRhs.get())
  {
  }

  // Node*      int
  SpeculativeOperation(
    const std::string& inSymbol,
    std::function<T(const T&, const T&)> inFunction,
    SpeculativeNode<T>* inLhs,
    const T& inRhs
  ):
    SpeculativeOperation(inSymbol, inFunction, inLhs, new SpeculativeLiteral<T>(inRhs))
  {
  }

  // Node*      Node*
  // This is the base constructor, all others
  // resolve to here
  SpeculativeOperation(
    const std::string& inSymbol,
    std::function<T(const T&, const T&)> inFunction,
    SpeculativeNode<T>* inLhs,
    SpeculativeNode<T>* inRhs
  ):
    SpeculativeNode<T>(),
    mSymbol(inSymbol),
    mFunction(inFunction),
    mLhs(inLhs),
    mRhs(inRhs)
  {
  }

  // Node*      object_ptr
  SpeculativeOperation(
    const std::string& inSymbol,
    std::function<T(const T&, const T&)> inFunction,
    SpeculativeNode<T>* inLhs,
    object_ptr<SpeculativeNode<T>> inRhs
  ):
    SpeculativeOperation(inSymbol, inFunction, inLhs, inRhs.get())
  {
  }

  // object_ptr int
  SpeculativeOperation(
    const std::string& inSymbol,
    std::function<T(const T&, const T&)> inFunction,
    object_ptr<SpeculativeNode<T>> inLhs,
    const T& inRhs
  ):
    SpeculativeOperation(inSymbol, inFunction, inLhs.get(), new SpeculativeLiteral<T>(inRhs))
  {
  }

  // object_ptr Node*
  SpeculativeOperation(
    const std::string& inSymbol,
    std::function<T(const T&, const T&)> inFunction,
    object_ptr<SpeculativeNode<T>> inLhs,
    SpeculativeNode<T>* inRhs
  ):
    SpeculativeOperation(inSymbol, inFunction, inLhs.get(), inRhs)
  {
  }

  // object_ptr object_ptr
  SpeculativeOperation(
    const std::string& inSymbol,
    std::function<T(const T&, const T&)> inFunction,
    object_ptr<SpeculativeNode<T>> inLhs,
    object_ptr<SpeculativeNode<T>> inRhs
  ):
    SpeculativeOperation(inSymbol, inFunction, inLhs.get(), inRhs.get())
  {
  }

  virtual ~SpeculativeOperation() {}

  virtual T resolve() { return mFunction(mLhs->resolve(), mRhs->resolve()); }

  virtual void makeConcrete() { mLhs->makeConcrete(); mRhs->makeConcrete(); };

  virtual std::string to_string() {
    std::stringstream ss;
    ss << "(" << mLhs->to_string() << " " << mSymbol << " " << mRhs->to_string() << ")";
    return ss.str();
  }

  friend std::ostream& operator<<(std::ostream& os, SpeculativeOperation<T> o) { return os << o.to_string(); }

private:
  std::string mSymbol;
  std::function<T(const T&, const T&)> mFunction;
  object_ptr<SpeculativeNode<T>> mLhs;
  object_ptr<SpeculativeNode<T>> mRhs;

};


//
// Operators
//

// Addition
template <typename T> object_ptr<SpeculativeNode<T>> operator+(object_ptr<SpeculativeNode<T>> lhs, object_ptr<SpeculativeNode<T>> rhs) { return object_ptr<SpeculativeNode<T>>(new SpeculativeOperation<T>("+", [](const T& l_lhs, const T& l_rhs) -> T { return l_lhs + l_rhs; }, lhs, rhs)); }
template <typename T> object_ptr<SpeculativeNode<T>> operator+(                     const T&  lhs, object_ptr<SpeculativeNode<T>> rhs) { return object_ptr<SpeculativeNode<T>>(new SpeculativeOperation<T>("+", [](const T& l_lhs, const T& l_rhs) -> T { return l_lhs + l_rhs; }, lhs, rhs)); }
template <typename T> object_ptr<SpeculativeNode<T>> operator+(object_ptr<SpeculativeNode<T>> lhs,                      const T&  rhs) { return object_ptr<SpeculativeNode<T>>(new SpeculativeOperation<T>("+", [](const T& l_lhs, const T& l_rhs) -> T { return l_lhs + l_rhs; }, lhs, rhs)); }
// If the other operand isn't a `T`, attempt to construct one from it
template <typename T, typename OtherT> object_ptr<SpeculativeNode<T>> operator+(                const OtherT&  lhs, object_ptr<SpeculativeNode<T>> rhs) { return T(lhs) + rhs; }
template <typename T, typename OtherT> object_ptr<SpeculativeNode<T>> operator+(object_ptr<SpeculativeNode<T>> lhs,                 const OtherT&  rhs) { return lhs + T(rhs); }

// Subtraction
template <typename T> object_ptr<SpeculativeNode<T>> operator-(object_ptr<SpeculativeNode<T>> lhs, object_ptr<SpeculativeNode<T>> rhs) { return object_ptr<SpeculativeNode<T>>(new SpeculativeOperation<T>("-", [](const T& l_lhs, const T& l_rhs) -> T { return l_lhs - l_rhs; }, lhs, rhs)); }
template <typename T> object_ptr<SpeculativeNode<T>> operator-(                     const T&  lhs, object_ptr<SpeculativeNode<T>> rhs) { return object_ptr<SpeculativeNode<T>>(new SpeculativeOperation<T>("-", [](const T& l_lhs, const T& l_rhs) -> T { return l_lhs - l_rhs; }, lhs, rhs)); }
template <typename T> object_ptr<SpeculativeNode<T>> operator-(object_ptr<SpeculativeNode<T>> lhs,                      const T&  rhs) { return object_ptr<SpeculativeNode<T>>(new SpeculativeOperation<T>("-", [](const T& l_lhs, const T& l_rhs) -> T { return l_lhs - l_rhs; }, lhs, rhs)); }
// If the other operand isn't a `T`, attempt to construct one from it
template <typename T, typename OtherT> object_ptr<SpeculativeNode<T>> operator-(                const OtherT&  lhs, object_ptr<SpeculativeNode<T>> rhs) { return T(lhs) - rhs; }
template <typename T, typename OtherT> object_ptr<SpeculativeNode<T>> operator-(object_ptr<SpeculativeNode<T>> lhs,                 const OtherT&  rhs) { return lhs - T(rhs); }

// Multiplication
template <typename T> object_ptr<SpeculativeNode<T>> operator*(object_ptr<SpeculativeNode<T>> lhs, object_ptr<SpeculativeNode<T>> rhs) { return object_ptr<SpeculativeNode<T>>(new SpeculativeOperation<T>("*", [](const T& l_lhs, const T& l_rhs) -> T { return l_lhs * l_rhs; }, lhs, rhs)); }
template <typename T> object_ptr<SpeculativeNode<T>> operator*(                     const T&  lhs, object_ptr<SpeculativeNode<T>> rhs) { return object_ptr<SpeculativeNode<T>>(new SpeculativeOperation<T>("*", [](const T& l_lhs, const T& l_rhs) -> T { return l_lhs * l_rhs; }, lhs, rhs)); }
template <typename T> object_ptr<SpeculativeNode<T>> operator*(object_ptr<SpeculativeNode<T>> lhs,                      const T&  rhs) { return object_ptr<SpeculativeNode<T>>(new SpeculativeOperation<T>("*", [](const T& l_lhs, const T& l_rhs) -> T { return l_lhs * l_rhs; }, lhs, rhs)); }
// If the other operand isn't a `T`, attempt to construct one from it
template <typename T, typename OtherT> object_ptr<SpeculativeNode<T>> operator*(                const OtherT&  lhs, object_ptr<SpeculativeNode<T>> rhs) { return T(lhs) * rhs; }
template <typename T, typename OtherT> object_ptr<SpeculativeNode<T>> operator*(object_ptr<SpeculativeNode<T>> lhs,                 const OtherT&  rhs) { return lhs * T(rhs); }

// Division
template <typename T> object_ptr<SpeculativeNode<T>> operator/(object_ptr<SpeculativeNode<T>> lhs, object_ptr<SpeculativeNode<T>> rhs) { return object_ptr<SpeculativeNode<T>>(new SpeculativeOperation<T>("/", [](const T& l_lhs, const T& l_rhs) -> T { return l_lhs / l_rhs; }, lhs, rhs)); }
template <typename T> object_ptr<SpeculativeNode<T>> operator/(                     const T&  lhs, object_ptr<SpeculativeNode<T>> rhs) { return object_ptr<SpeculativeNode<T>>(new SpeculativeOperation<T>("/", [](const T& l_lhs, const T& l_rhs) -> T { return l_lhs / l_rhs; }, lhs, rhs)); }
template <typename T> object_ptr<SpeculativeNode<T>> operator/(object_ptr<SpeculativeNode<T>> lhs,                      const T&  rhs) { return object_ptr<SpeculativeNode<T>>(new SpeculativeOperation<T>("/", [](const T& l_lhs, const T& l_rhs) -> T { return l_lhs / l_rhs; }, lhs, rhs)); }
// If the other operand isn't a `T`, attempt to construct one from it
template <typename T, typename OtherT> object_ptr<SpeculativeNode<T>> operator/(                const OtherT&  lhs, object_ptr<SpeculativeNode<T>> rhs) { return T(lhs) / rhs; }
template <typename T, typename OtherT> object_ptr<SpeculativeNode<T>> operator/(object_ptr<SpeculativeNode<T>> lhs,                 const OtherT&  rhs) { return lhs / T(rhs); }

// Maximum
template <typename T> object_ptr<SpeculativeNode<T>> max(object_ptr<SpeculativeNode<T>> lhs, object_ptr<SpeculativeNode<T>> rhs) { return object_ptr<SpeculativeNode<T>>(new SpeculativeOperation<T>("max", [](const T& l_lhs, const T& l_rhs) -> T { return max(l_lhs, l_rhs); }, lhs, rhs)); }
template <typename T> object_ptr<SpeculativeNode<T>> max(                     const T&  lhs, object_ptr<SpeculativeNode<T>> rhs) { return object_ptr<SpeculativeNode<T>>(new SpeculativeOperation<T>("max", [](const T& l_lhs, const T& l_rhs) -> T { return max(l_lhs, l_rhs); }, lhs, rhs)); }
template <typename T> object_ptr<SpeculativeNode<T>> max(object_ptr<SpeculativeNode<T>> lhs,                      const T&  rhs) { return object_ptr<SpeculativeNode<T>>(new SpeculativeOperation<T>("max", [](const T& l_lhs, const T& l_rhs) -> T { return max(l_lhs, l_rhs); }, lhs, rhs)); }
// If the other operand isn't a `T`, attempt to construct one from it
template <typename T, typename OtherT> object_ptr<SpeculativeNode<T>> max(                const OtherT&  lhs, object_ptr<SpeculativeNode<T>> rhs) { return max(T(lhs), rhs); }
template <typename T, typename OtherT> object_ptr<SpeculativeNode<T>> max(object_ptr<SpeculativeNode<T>> lhs,                 const OtherT&  rhs) { return max(lhs, T(rhs)); }

// Minimum
template <typename T> object_ptr<SpeculativeNode<T>> min(object_ptr<SpeculativeNode<T>> lhs, object_ptr<SpeculativeNode<T>> rhs) { return object_ptr<SpeculativeNode<T>>(new SpeculativeOperation<T>("min", [](const T& l_lhs, const T& l_rhs) -> T { return min(l_lhs, l_rhs); }, lhs, rhs)); }
template <typename T> object_ptr<SpeculativeNode<T>> min(                     const T&  lhs, object_ptr<SpeculativeNode<T>> rhs) { return object_ptr<SpeculativeNode<T>>(new SpeculativeOperation<T>("min", [](const T& l_lhs, const T& l_rhs) -> T { return min(l_lhs, l_rhs); }, lhs, rhs)); }
template <typename T> object_ptr<SpeculativeNode<T>> min(object_ptr<SpeculativeNode<T>> lhs,                      const T&  rhs) { return object_ptr<SpeculativeNode<T>>(new SpeculativeOperation<T>("min", [](const T& l_lhs, const T& l_rhs) -> T { return min(l_lhs, l_rhs); }, lhs, rhs)); }
// If the other operand isn't a `T`, attempt to construct one from it
template <typename T, typename OtherT> object_ptr<SpeculativeNode<T>> min(                const OtherT&  lhs, object_ptr<SpeculativeNode<T>> rhs) { return min(T(lhs), rhs); }
template <typename T, typename OtherT> object_ptr<SpeculativeNode<T>> min(object_ptr<SpeculativeNode<T>> lhs,                 const OtherT&  rhs) { return min(lhs, T(rhs)); }


#endif
