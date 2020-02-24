#ifndef Value_h
#define Value_h

#include <list>
#include <iostream>
#include <string>
#include <type_traits>

#include "ValueParser.h"
#include "Base.h"


namespace parsing {


template <typename T, unsigned MIN=1, unsigned MAX=MIN>
class Value {
public:
  Value(const std::string& inName): mName(inName), mValues() {}
  Value(const std::string& inName, const std::list<T>& inDefault): mName(inName), mValues(inDefault) {}
  const std::string& getName() const { return mName; }
  void streamHelp(std::ostream& os) const { os << "<" << *this << "{" << MIN << ", " << MAX << "}>"; }

  void streamValue(std::ostream& os) const {
    if (isSet()) {
      auto iter = getValues().begin();
      os << *iter++;
      for (; iter != getValues().end(); ++iter) {
        os << ", " << *iter;
      }
    }
    else { os << "--"; }
  }

  bool isSet() const { return mValues.size() != 0; }
  const std::list<T>& getValues() const { return mValues; }

  Token parse(
    const Token& inCurrentIter,
    const Token& inEndIter
  ) {
    auto iter = inCurrentIter;
    while (mValues.size() < MAX and iter != inEndIter) {
      auto result = ValueParser<T>::parse(*iter);
      if (result.first) {
        mValues.push_back(result.second);
        ++iter;
      }
      else {
        break;
      }
    }
    if (MIN <= mValues.size()) {
      return iter;
    }
    mValues.clear();
    return inCurrentIter;
  }

private:
  const std::string mName;
  std::list<T> mValues;

};


template <typename T, unsigned MAX>
class Value<T, 0, MAX> {
public:
  Value(const std::string& inName): mName(inName), mValues() {}
  Value(const std::string& inName, const std::list<T>& inDefault): mName(inName), mValues(inDefault) {}
  const std::string& getName() const { return mName; }
  void streamHelp(std::ostream& os) const { os << "[" << *this << "]"; }

  bool isSet() const { return mValues.size() != 0; }
  const std::list<T>& getValues() const { return mValues; }

  void streamValue(std::ostream& os) const {
    if (isSet()) {
      auto iter = getValues().begin();
      os << *iter++;
      for (; iter != getValues().end(); ++iter) {
        os << ", " << *iter;
      }
    }
    else { os << "--"; }
  }

  Token parse(
    const Token& inCurrentIter,
    const Token& inEndIter
  ) {
    auto iter = inCurrentIter;
    while (mValues.size() < MAX and iter != inEndIter) {
      auto result = ValueParser<T>::parse(*iter);
      if (result.first) {
        mValues.push_back(result.second);
        ++iter;
      }
      else {
        break;
      }
    }
    return iter;
  }

private:
  const std::string mName;
  std::list<T> mValues;

};


template <typename T>
class Value<T, 1, 1> {
public:
  Value(const std::string& inName): mName(inName) {}
  Value(const std::string& inName, const T& inDefault): mName(inName), mSet(true), mValue(inDefault) {}
  const std::string& getName() const { return mName; }
  void streamHelp(std::ostream& os) const { os << "<" << *this << ">"; }

  const bool& isSet() const { return mSet; }
  const T& getValue() const { return mValue; }

  void streamValue(std::ostream& os) const {
    if (isSet()) { os << getValue(); }
    else { os << "--"; }
  }

  Token parse(
    const Token& inCurrentIter,
    const Token& inEndIter
  ) {
    if (inCurrentIter != inEndIter) {
      auto iter = inCurrentIter;
      auto result = ValueParser<T>::parse(*iter);
      if (result.first) {
        mValue = result.second;
        ++iter;
      }
      mSet = true;
      return iter;
    }
    mSet = false;
    return inCurrentIter;
  }

  inline friend std::ostream& operator<<(std::ostream& os, const parsing::Value<T, 1, 1>& inValue) {
    return os << inValue.getName();
  }

private:
  const std::string mName;
  bool mSet { false };
  T mValue;

};


// Effectively disable a Value<T, 0, 0> template instantiation
template <typename T>
class Value<T, 0, 0> {};


template <typename T, unsigned MIN, unsigned MAX>
inline std::ostream& operator<<(std::ostream& os, const parsing::Value<T, MIN, MAX>& inValue) {
  return os << inValue.getName();
}


} // namespace parsing


#endif
