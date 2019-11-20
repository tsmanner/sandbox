#ifndef SpeculativeOperand_h
#define SpeculativeOperand_h

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <sstream>


class SpeculativeOperand {
public:
  static std::default_random_engine generator;

  // Default Constructor
  SpeculativeOperand(
  ):
    mHasValue(false),
    mValue(0),
    mLowerBound(0),
    mUpperBound(0)
  {
  }

  // Bounded Constructor
  SpeculativeOperand(
    const int& inLowerBound,
    const int& inUpperBound
  ):
    mHasValue(false),
    mValue(0),
    mLowerBound(inLowerBound),
    mUpperBound(inUpperBound)
  {
    if (inLowerBound == inUpperBound) makeConcrete();
  }

  // Value Constructor
  SpeculativeOperand(
    const int& inValue
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
  int getValue() { return mValue; }

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

  //
  // Addition
  //
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
  friend SpeculativeOperand operator+(const int& lhs, const SpeculativeOperand& rhs) { return SpeculativeOperand(lhs) + rhs; }
  friend SpeculativeOperand operator+(const SpeculativeOperand& lhs, const int& rhs) { return lhs + SpeculativeOperand(rhs); }

  //
  // Subtraction
  //
  friend SpeculativeOperand operator-(const SpeculativeOperand& lhs, const SpeculativeOperand& rhs) {
    if (lhs.mHasValue and rhs.mHasValue) {
      return SpeculativeOperand(lhs.mValue - rhs.mValue);
    }
    else if (lhs.mHasValue) {
      return SpeculativeOperand(
        lhs.mValue - rhs.mUpperBound,
        lhs.mValue - rhs.mLowerBound
      );
    }
    else if (rhs.mHasValue) {
      return SpeculativeOperand(
        lhs.mLowerBound - rhs.mValue,
        lhs.mUpperBound - rhs.mValue
      );
    }
    return SpeculativeOperand(
      lhs.mLowerBound - rhs.mUpperBound,
      lhs.mUpperBound - rhs.mLowerBound
    );
  }
  friend SpeculativeOperand operator-(const int& lhs, const SpeculativeOperand& rhs) { return SpeculativeOperand(lhs) - rhs; }
  friend SpeculativeOperand operator-(const SpeculativeOperand& lhs, const int& rhs) { return lhs - SpeculativeOperand(rhs); }

  //
  // Multiplication
  //
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
  friend SpeculativeOperand operator*(const int& lhs, const SpeculativeOperand& rhs) { return SpeculativeOperand(lhs) * rhs; }
  friend SpeculativeOperand operator*(const SpeculativeOperand& lhs, const int& rhs) { return lhs * SpeculativeOperand(rhs); }

  //
  // Division
  //
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
  friend SpeculativeOperand operator/(const int& lhs, const SpeculativeOperand& rhs) { return SpeculativeOperand(lhs) / rhs; }
  friend SpeculativeOperand operator/(const SpeculativeOperand& lhs, const int& rhs) { return lhs / SpeculativeOperand(rhs); }

  //
  // Max
  //
  friend SpeculativeOperand max(const SpeculativeOperand& lhs, const SpeculativeOperand& rhs) {
    if (lhs.mHasValue and rhs.mHasValue) {
      return SpeculativeOperand(std::max(lhs.mValue, rhs.mValue));
    }
    else if (lhs.mHasValue) {
      return SpeculativeOperand(
        std::max(lhs.mValue, rhs.mLowerBound),
        std::max(lhs.mValue, rhs.mUpperBound)
      );
    }
    else if (rhs.mHasValue) {
      return SpeculativeOperand(
        std::max(lhs.mLowerBound, rhs.mValue),
        std::max(lhs.mUpperBound, rhs.mValue)
      );
    }
    return SpeculativeOperand(
      std::max(lhs.mLowerBound, rhs.mLowerBound),
      std::max(lhs.mUpperBound, rhs.mUpperBound)
    );
  }
  friend SpeculativeOperand max(const int& lhs, const SpeculativeOperand& rhs) { return max(SpeculativeOperand(lhs), rhs); }
  friend SpeculativeOperand max(const SpeculativeOperand& lhs, const int& rhs) { return max(lhs, SpeculativeOperand(rhs)); }

  //
  // Min
  //
  friend SpeculativeOperand min(const SpeculativeOperand& lhs, const SpeculativeOperand& rhs) {
    if (lhs.mHasValue and rhs.mHasValue) {
      return SpeculativeOperand(std::min(lhs.mValue, rhs.mValue));
    }
    else if (lhs.mHasValue) {
      return SpeculativeOperand(
        std::min(lhs.mValue, rhs.mLowerBound),
        std::min(lhs.mValue, rhs.mUpperBound)
      );
    }
    else if (rhs.mHasValue) {
      return SpeculativeOperand(
        std::min(lhs.mLowerBound, rhs.mValue),
        std::min(lhs.mUpperBound, rhs.mValue)
      );
    }
    return SpeculativeOperand(
      std::min(lhs.mLowerBound, rhs.mLowerBound),
      std::min(lhs.mUpperBound, rhs.mUpperBound)
    );
  }
  friend SpeculativeOperand min(const int& lhs, const SpeculativeOperand& rhs) { return min(SpeculativeOperand(lhs), rhs); }
  friend SpeculativeOperand min(const SpeculativeOperand& lhs, const int& rhs) { return min(lhs, SpeculativeOperand(rhs)); }

  //
  // Stream Operator
  //
  friend std::ostream& operator<<(std::ostream& os, const SpeculativeOperand& so) {
    if (so.mHasValue) return os << so.mValue;
    std::stringstream ss;
    ss << "[" << so.mLowerBound << ":" << so.mUpperBound << "]";
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

void makeOperandConcrete(SpeculativeOperand& inSpeculativeOperand) { inSpeculativeOperand.makeConcrete(); }

#endif
