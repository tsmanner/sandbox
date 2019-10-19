#ifndef SpecSched_h
#define SpecSched_h

#include <functional>
#include <sstream>
#include <string>

#include "object_ptr.h"


template <typename T>
class Node : public object_ptr_interface {
public:
  Node(
  ):
    object_ptr_interface()
  {
  }

  virtual T resolve() = 0;
  virtual std::string to_string() = 0;

private:

};


template <typename T>
class Literal : public Node<T> {
public:
  Literal(
    const T& inT
  ):
    Node<T>(),
    mValue(inT)
  {
  }

  virtual T resolve() { return mValue; }

  virtual std::string to_string() {
    std::stringstream ss;
    ss << mValue;
    return ss.str();
  }

  friend std::ostream& operator<<(ostream& os, Literal<T> l) { return os << l.to_string(); }

private:
  T mValue;

};


template <typename T>
class Source : public Node<T> {
public:
  template <typename FunctionType, typename... ArgTypes>
  Source(
    FunctionType inSourceFunc,
    ArgTypes... inArgs
  ):
    Node<T>(),
    mSourceFunc(std::bind(inSourceFunc, inArgs...))
  {
  }

  Source(
    std::function<T()> inSourceFunc
  ):
    mSourceFunc(inSourceFunc)
  {
  }

  virtual ~Source() {
  }

  virtual T resolve() { return mSourceFunc(); }

  virtual std::string to_string() {
    std::stringstream ss;
    ss << mSourceFunc();
    return ss.str();
  }

  friend std::ostream& operator<<(ostream& os, Source<T> s) { return os << s.to_string(); }

private:
  std::function<T()> mSourceFunc;

};


template <typename T>
class Operation : public Node<T> {
public:
  Operation(
    const std::string& inSymbol,
    std::function<T(const T&, const T&)> inFunction,
    Node<T>* inLhs,
    Node<T>* inRhs
  ):
    Node<T>(),
    mSymbol(inSymbol),
    mFunction(inFunction),
    mLhs(inLhs),
    mRhs(inRhs)
  {
  }

  Operation(
    const std::string& inSymbol,
    std::function<T(const T&, const T&)> inFunction,
    object_ptr<Node<T>> inLhs,
    object_ptr<Node<T>> inRhs
  ):
    Operation(inSymbol, inFunction, inLhs.get(), inRhs.get())
  {
  }

  Operation(
    const std::string& inSymbol,
    std::function<T(const T&, const T&)> inFunction,
    const T& inLhs,
    Node<T>* inRhs
  ):
    Node<T>(),
    mSymbol(inSymbol),
    mFunction(inFunction),
    mLhs(new Literal<T>(inLhs)),
    mRhs(inRhs)
  {
  }

  Operation(
    const std::string& inSymbol,
    std::function<T(const T&, const T&)> inFunction,
    const T& inLhs,
    object_ptr<Node<T>> inRhs
  ):
    Operation(inSymbol, inFunction, inLhs, inRhs.get())
  {
  }

  Operation(
    const std::string& inSymbol,
    std::function<T(const T&, const T&)> inFunction,
    Node<T>* inLhs,
    const T& inRhs
  ):
    Node<T>(),
    mSymbol(inSymbol),
    mFunction(inFunction),
    mLhs(inLhs),
    mRhs(new Literal<T>(inRhs))
  {
  }

  Operation(
    const std::string& inSymbol,
    std::function<T(const T&, const T&)> inFunction,
    object_ptr<Node<T>> inLhs,
    const T& inRhs
  ):
    Operation(inSymbol, inFunction, inLhs.get(), inRhs)
  {
  }

  virtual ~Operation() {}

  virtual T resolve() { return mFunction(mLhs->resolve(), mRhs->resolve()); }

  virtual std::string to_string() {
    std::stringstream ss;
    ss << "(" << mLhs->to_string() << " " << mSymbol << " " << mRhs->to_string() << ")";
    return ss.str();
  }

  friend std::ostream& operator<<(ostream& os, Operation<T> o) { return os << o.to_string(); }

private:
  std::string mSymbol;
  std::function<T(const T&, const T&)> mFunction;
  object_ptr<Node<T>> mLhs;
  object_ptr<Node<T>> mRhs;

};


//
// Operators
//

// Addition
template <typename T> object_ptr<Node<T>> operator+(object_ptr<Node<T>> lhs, object_ptr<Node<T>> rhs) { return object_ptr<Node<T>>(new Operation<T>("+", [](const T& l_lhs, const T& l_rhs) -> T { return l_lhs + l_rhs; }, lhs, rhs)); }
template <typename T> object_ptr<Node<T>> operator+(          const T&  lhs, object_ptr<Node<T>> rhs) { return object_ptr<Node<T>>(new Operation<T>("+", [](const T& l_lhs, const T& l_rhs) -> T { return l_lhs + l_rhs; }, lhs, rhs)); }
template <typename T> object_ptr<Node<T>> operator+(object_ptr<Node<T>> lhs,           const T&  rhs) { return object_ptr<Node<T>>(new Operation<T>("+", [](const T& l_lhs, const T& l_rhs) -> T { return l_lhs + l_rhs; }, lhs, rhs)); }

// Subtraction
template <typename T> object_ptr<Node<T>> operator-(object_ptr<Node<T>> lhs, object_ptr<Node<T>> rhs) { return object_ptr<Node<T>>(new Operation<T>("-", [](const T& l_lhs, const T& l_rhs) -> T { return l_lhs - l_rhs; }, lhs, rhs)); }
template <typename T> object_ptr<Node<T>> operator-(          const T&  lhs, object_ptr<Node<T>> rhs) { return object_ptr<Node<T>>(new Operation<T>("-", [](const T& l_lhs, const T& l_rhs) -> T { return l_lhs - l_rhs; }, lhs, rhs)); }
template <typename T> object_ptr<Node<T>> operator-(object_ptr<Node<T>> lhs,           const T&  rhs) { return object_ptr<Node<T>>(new Operation<T>("-", [](const T& l_lhs, const T& l_rhs) -> T { return l_lhs - l_rhs; }, lhs, rhs)); }

// Multiplication
template <typename T> object_ptr<Node<T>> operator*(object_ptr<Node<T>> lhs, object_ptr<Node<T>> rhs) { return object_ptr<Node<T>>(new Operation<T>("*", [](const T& l_lhs, const T& l_rhs) -> T { return l_lhs * l_rhs; }, lhs, rhs)); }
template <typename T> object_ptr<Node<T>> operator*(          const T&  lhs, object_ptr<Node<T>> rhs) { return object_ptr<Node<T>>(new Operation<T>("*", [](const T& l_lhs, const T& l_rhs) -> T { return l_lhs * l_rhs; }, lhs, rhs)); }
template <typename T> object_ptr<Node<T>> operator*(object_ptr<Node<T>> lhs,           const T&  rhs) { return object_ptr<Node<T>>(new Operation<T>("*", [](const T& l_lhs, const T& l_rhs) -> T { return l_lhs * l_rhs; }, lhs, rhs)); }

// Division
template <typename T> object_ptr<Node<T>> operator/(object_ptr<Node<T>> lhs, object_ptr<Node<T>> rhs) { return object_ptr<Node<T>>(new Operation<T>("/", [](const T& l_lhs, const T& l_rhs) -> T { return l_lhs / l_rhs; }, lhs, rhs)); }
template <typename T> object_ptr<Node<T>> operator/(          const T&  lhs, object_ptr<Node<T>> rhs) { return object_ptr<Node<T>>(new Operation<T>("/", [](const T& l_lhs, const T& l_rhs) -> T { return l_lhs / l_rhs; }, lhs, rhs)); }
template <typename T> object_ptr<Node<T>> operator/(object_ptr<Node<T>> lhs,           const T&  rhs) { return object_ptr<Node<T>>(new Operation<T>("/", [](const T& l_lhs, const T& l_rhs) -> T { return l_lhs / l_rhs; }, lhs, rhs)); }

// Maximum
template <typename T> object_ptr<Node<T>> max(object_ptr<Node<T>> lhs, object_ptr<Node<T>> rhs) {
  return object_ptr<Node<T>>(
    new Operation<T>(
      "max",
      [](const T& l_lhs, const T& l_rhs) -> T {
        return max(l_lhs, l_rhs);
      },
      lhs,
      rhs
    )
  );
}
template <typename T> object_ptr<Node<T>> max(          const T&  lhs, object_ptr<Node<T>> rhs) { return object_ptr<Node<T>>(new Operation<T>("max", [](const T& l_lhs, const T& l_rhs) -> T { return max(l_lhs, l_rhs); }, lhs, rhs)); }
template <typename T> object_ptr<Node<T>> max(object_ptr<Node<T>> lhs,           const T&  rhs) { return object_ptr<Node<T>>(new Operation<T>("max", [](const T& l_lhs, const T& l_rhs) -> T { return max(l_lhs, l_rhs); }, lhs, rhs)); }

// Minimum
template <typename T> object_ptr<Node<T>> min(object_ptr<Node<T>> lhs, object_ptr<Node<T>> rhs) { return object_ptr<Node<T>>(new Operation<T>("min", [](const T& l_lhs, const T& l_rhs) -> T { return min(l_lhs, l_rhs); }, lhs, rhs)); }
template <typename T> object_ptr<Node<T>> min(          const T&  lhs, object_ptr<Node<T>> rhs) { return object_ptr<Node<T>>(new Operation<T>("min", [](const T& l_lhs, const T& l_rhs) -> T { return min(l_lhs, l_rhs); }, lhs, rhs)); }
template <typename T> object_ptr<Node<T>> min(object_ptr<Node<T>> lhs,           const T&  rhs) { return object_ptr<Node<T>>(new Operation<T>("min", [](const T& l_lhs, const T& l_rhs) -> T { return min(l_lhs, l_rhs); }, lhs, rhs)); }


#endif
