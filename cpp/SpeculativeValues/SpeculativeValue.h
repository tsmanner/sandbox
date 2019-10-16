#ifndef SpeculativeValue_h
#define SpeculativeValue_h

#include <chrono>
#include <ostream>
#include <random>
#include <set>
#include <sstream>


class SpeculativeValue {
public:
  static std::default_random_engine generator;

  SpeculativeValue(
    int inLowerBound,
    int inUpperBound
  ):
    mLowerBound(inLowerBound),
    mUpperBound(inUpperBound)
  {
  }

  SpeculativeValue(
    int inOffset
  ):
    mHasValue(true),
    mOffset(inOffset),
    mLowerBound(inOffset),
    mUpperBound(inOffset)
  {
  }

  void setLowerBound(int inLowerBound) { mLowerBound = inLowerBound; }
  void setUpperBound(int inUpperBound) { mUpperBound = inUpperBound; }

  void addUpstreamValue(SpeculativeValue& inSpeculativeValue) {
    inSpeculativeValue.mDownstreamValues.insert(this);
    mUpstreamValues.insert(&inSpeculativeValue);
  }

  void addDownstreamValue(SpeculativeValue& inSpeculativeValue) {
    inSpeculativeValue.mUpstreamValues.insert(this);
    mDownstreamValues.insert(&inSpeculativeValue);
  }

  void setOffset(const int& inOffset) {
    mHasValue = true;
    mOffset = inOffset;
  }

  const int& getOffset() { return mOffset; }

  // True if all upstreams have values
  bool isConcrete() const {
    for (auto upstream : mUpstreamValues) {
      if (!upstream->isConcrete()) return false;
    }
    return mHasValue;
  }

  bool canBeMadeConcrete() {
    for (auto upstream : mUpstreamValues) {
      if (!upstream->isConcrete()) return false;
    }
    // False if it is already concrete
    return !mHasValue;
  }

  int getConcreteValue() const {
    int value = mOffset;
    for (auto upstream : mUpstreamValues) {
      value += upstream->getConcreteValue();
    }
    return value;
  }

  void makeConcrete() {
    if (!mHasValue) {
      std::uniform_int_distribution<int> distribution(mLowerBound, mUpperBound);
      setOffset(distribution(SpeculativeValue::generator));
    }
  }

  friend std::ostream& operator<<(std::ostream& os, SpeculativeValue& sv) {
    // If both me and my upstream have a concrete value, just
    // print that
    return os << sv.to_string();
  }

  std::string to_string(bool decompose=false) const {
    std::stringstream ss;
    if (!decompose and isConcrete()) {
      ss << getConcreteValue();
      return ss.str();
    }
    if (mUpstreamValues.size()) {
      ss << "(";
      for (auto iter = mUpstreamValues.begin(); iter != mUpstreamValues.end(); ++iter) {
        if (iter != mUpstreamValues.begin()) ss << " + ";
        ss << (*iter)->to_string(decompose);
      }
      ss << ") + ";
    }
    if (mHasValue) ss << mOffset;
    else ss << "[" << mLowerBound << ":" << mUpperBound << "]";
    return ss.str();
  }


private:
  // Set if I have a value
  bool mHasValue = false;
  // My eventual value
  int mOffset = 0;

  int mLowerBound = 0;
  int mUpperBound = 0;

  std::set<SpeculativeValue*> mUpstreamValues;
  // Assume all downstream values are relative to my eventual concrete value
  std::set<SpeculativeValue*> mDownstreamValues;

};


std::default_random_engine SpeculativeValue::generator = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());


#endif
