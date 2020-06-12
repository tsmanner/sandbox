#include <functional>
#include <iostream>
#include <type_traits>

#include "cascade_equality.hpp"
#include "cascade_inequality.hpp"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"



class MyClass {
public:
  MyClass(const int& inX, const int& inY): mX(inX), mY(inY) {}

  const int& getX() const { return mX; }
  const int& getY() const { return mY; }

private:
  const int mX;
  const int mY;

};


std::ostream& operator<<(std::ostream& os, const MyClass& mc) {
  return os << "(" << mc.getX() << ", " << mc.getY() << ")";
}


TEST_CASE("Cascade Compares") {
  // a < b < c < d < e
  //   Note: a < b and a > b will fail if the precedence is reversed
  auto a = MyClass(0, 1);
  auto b = MyClass(1, 0);
  auto c = MyClass(1, 2);
  auto d = MyClass(1, 3);
  auto e = MyClass(2, 3);

  SECTION("Equal") {
    CHECK(cascade_equality(a, a, &MyClass::getX, &MyClass::getY));
    CHECK_FALSE(cascade_equality(a, b, &MyClass::getX, &MyClass::getY));
    CHECK_FALSE(cascade_equality(a, c, &MyClass::getX, &MyClass::getY));
    CHECK_FALSE(cascade_equality(a, d, &MyClass::getX, &MyClass::getY));
    CHECK_FALSE(cascade_equality(a, e, &MyClass::getX, &MyClass::getY));
  }

  SECTION("Unary Less Than") {
    CHECK_FALSE(cascade_inequality<std::less>(b, a, &MyClass::getX, &MyClass::getY));
    CHECK_FALSE(cascade_inequality<std::less>(b, b, &MyClass::getX, &MyClass::getY));
    CHECK(cascade_inequality<std::less>(b, c, &MyClass::getX, &MyClass::getY));
    CHECK(cascade_inequality<std::less>(b, d, &MyClass::getX, &MyClass::getY));
    CHECK(cascade_inequality<std::less>(b, e, &MyClass::getX, &MyClass::getY));
  }

  SECTION("Unary Greater Than") {
    CHECK(cascade_inequality<std::greater>(b, a, &MyClass::getX, &MyClass::getY));
    CHECK_FALSE(cascade_inequality<std::greater>(b, b, &MyClass::getX, &MyClass::getY));
    CHECK_FALSE(cascade_inequality<std::greater>(b, c, &MyClass::getX, &MyClass::getY));
    CHECK_FALSE(cascade_inequality<std::greater>(b, d, &MyClass::getX, &MyClass::getY));
    CHECK_FALSE(cascade_inequality<std::greater>(b, e, &MyClass::getX, &MyClass::getY));
  }

}
