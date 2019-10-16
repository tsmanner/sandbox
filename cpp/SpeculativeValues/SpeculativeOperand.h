#ifndef SpeculativeOperand_h
#define SpeculativeOperand_h

#include <chrono>
#include <iostream>
#include <random>
#include <sstream>


class SpeculativeOperand {
public:
  static std::default_random_engine generator;

  // Bounded Constructor
  SpeculativeOperand(
    int inLowerBound,
    int inUpperBound
  ):
    mHasValue(false),
    mValue(0),
    mLowerBound(inLowerBound),
    mUpperBound(inUpperBound)
  {
  }

  // Value Constructor
  SpeculativeOperand(
    int inValue
  ):
    mHasValue(true),
    mValue(inValue),
    mLowerBound(inValue),
    mUpperBound(inValue)
  {
  }

  // Copy Constructor
  SpeculativeOperand(
    const SpeculativeOperand& rhs
  ):
    mHasValue(rhs.mHasValue),
    mValue(rhs.mValue),
    mLowerBound(rhs.mLowerBound),
    mUpperBound(rhs.mUpperBound)
  {
  }

  // Move Constructor
  SpeculativeOperand(
    SpeculativeOperand&& rhs
  ):
    mHasValue(rhs.mHasValue),
    mValue(rhs.mValue),
    mLowerBound(rhs.mLowerBound),
    mUpperBound(rhs.mUpperBound)
  {
  }

  // Copy Assignment
  SpeculativeOperand& operator=(const SpeculativeOperand& rhs) {
    mHasValue = rhs.mHasValue;
    mValue = rhs.mValue;
    mLowerBound = rhs.mLowerBound;
    mUpperBound = rhs.mUpperBound;
    return *this;
  }

  // Move Assignment
  SpeculativeOperand& operator=(SpeculativeOperand&& rhs) {
    mHasValue = rhs.mHasValue;
    mValue = rhs.mValue;
    mLowerBound = rhs.mLowerBound;
    mUpperBound = rhs.mUpperBound;
    return *this;
  }


  bool hasValue() { return mHasValue; }

  void makeConcrete() {
    if (!mHasValue) {
      std::uniform_int_distribution<int> distribution(mLowerBound, mUpperBound);
      mValue = distribution(SpeculativeOperand::generator);
      mHasValue = true;
    }
  }

  //
  // Mathematical Operators
  //

  // Addition
  friend SpeculativeOperand operator+(const SpeculativeOperand& lhs, const SpeculativeOperand& rhs) {
    if (lhs.mHasValue and rhs.mHasValue) {
      return SpeculativeOperand(lhs.mValue + rhs.mValue);
    }
    else if (lhs.mHasValue) {
      return SpeculativeOperand(
        lhs.mValue + rhs.mLowerBound,
        lhs.mValue + rhs.mUpperBound
      );
    }
    else if (rhs.mHasValue) {
      return SpeculativeOperand(
        lhs.mLowerBound + rhs.mValue,
        lhs.mUpperBound + rhs.mValue
      );
    }
    return SpeculativeOperand(
      lhs.mLowerBound + rhs.mLowerBound,
      lhs.mUpperBound + rhs.mUpperBound
    );
  }

  // Subtraction
  friend SpeculativeOperand operator-(const SpeculativeOperand& lhs, const SpeculativeOperand& rhs) {
    if (lhs.mHasValue and rhs.mHasValue) {
      return SpeculativeOperand(lhs.mValue - rhs.mValue);
    }
    else if (lhs.mHasValue) {
      return SpeculativeOperand(
        lhs.mValue - rhs.mLowerBound,
        lhs.mValue - rhs.mUpperBound
      );
    }
    else if (rhs.mHasValue) {
      return SpeculativeOperand(
        lhs.mLowerBound - rhs.mValue,
        lhs.mUpperBound - rhs.mValue
      );
    }
    return SpeculativeOperand(
      lhs.mLowerBound - rhs.mLowerBound,
      lhs.mUpperBound - rhs.mUpperBound
    );
  }

  // Multiplication
  friend SpeculativeOperand operator*(const SpeculativeOperand& lhs, const SpeculativeOperand& rhs) {
    if (lhs.mHasValue and rhs.mHasValue) {
      return SpeculativeOperand(lhs.mValue * rhs.mValue);
    }
    else if (lhs.mHasValue) {
      return SpeculativeOperand(
        lhs.mValue * rhs.mLowerBound,
        lhs.mValue * rhs.mUpperBound
      );
    }
    else if (rhs.mHasValue) {
      return SpeculativeOperand(
        lhs.mLowerBound * rhs.mValue,
        lhs.mUpperBound * rhs.mValue
      );
    }
    return SpeculativeOperand(
      lhs.mLowerBound * rhs.mLowerBound,
      lhs.mUpperBound * rhs.mUpperBound
    );
  }

  // Division
  friend SpeculativeOperand operator/(const SpeculativeOperand& lhs, const SpeculativeOperand& rhs) {
    if (lhs.mHasValue and rhs.mHasValue) {
      return SpeculativeOperand(lhs.mValue / rhs.mValue);
    }
    else if (lhs.mHasValue) {
      return SpeculativeOperand(
        lhs.mValue / rhs.mUpperBound,
        lhs.mValue / rhs.mLowerBound
      );
    }
    else if (rhs.mHasValue) {
      return SpeculativeOperand(
        lhs.mLowerBound / rhs.mValue,
        lhs.mUpperBound / rhs.mValue
      );
    }
    return SpeculativeOperand(
      lhs.mUpperBound / rhs.mUpperBound,
      lhs.mLowerBound / rhs.mLowerBound
    );
  }

  //
  // Stream Operator
  //
  friend std::ostream& operator<<(std::ostream& os, const SpeculativeOperand& so) {
    if (so.mHasValue) return os << so.mValue;
    std::stringstream ss;
    ss << "(" << so.mLowerBound << ":" << so.mUpperBound << ")";
    return os << ss.str();
  }

private:
  bool mHasValue;
  int mValue;
  int mLowerBound;
  int mUpperBound;

};

// Initialize the static random number generator
std::default_random_engine SpeculativeOperand::generator = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());


#endif
