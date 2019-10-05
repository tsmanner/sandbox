#include "Bound.h"
#include "LowerBound.h"
#include "UpperBound.h"


TEST_CASE("Bound Default Constructor", "[Bound]") {
  auto b = Bound();
  REQUIRE(b.getType() == Bound::cOpen);
}


TEST_CASE("Bound::set", "[Bound][set]") {
  auto b = Bound();

  b.set(1);
  REQUIRE(b.getType() == Bound::cClosed);
  REQUIRE(b.getValue() == 1);

  b.set<Bound::cClosed>(2);
  REQUIRE(b.getType() == Bound::cClosed);
  REQUIRE(b.getValue() == 2);

  b.set<Bound::cOpen>();
  REQUIRE(b.getType() == Bound::cOpen);
}


TEST_CASE("Bound::operator=", "[Bound][assignment]") {
  auto b = Bound();

  REQUIRE(b.getType() == Bound::cOpen);
  b = 1;
  REQUIRE(b.getType() == Bound::cClosed);
  REQUIRE(b.getValue() == 1);

}


TEST_CASE("LowerBound::operator=", "[Bound][LowerBound][assignment]") {
  auto b = LowerBound();

  REQUIRE(b.getType() == Bound::cOpen);
  b = 1;
  REQUIRE(b.getType() == Bound::cClosed);
  REQUIRE(b.getValue() == 1);

}


TEST_CASE("UpperBound::operator=", "[Bound][UpperBound][assignment]") {
  auto b = UpperBound();

  REQUIRE(b.getType() == Bound::cOpen);
  b = 1;
  REQUIRE(b.getType() == Bound::cClosed);
  REQUIRE(b.getValue() == 1);

}


TEST_CASE("LowerBound < LowerBound", "[Bound][LowerBound][lt][less]") {
  REQUIRE((LowerBound(0) < LowerBound(1)) == true);
  REQUIRE((LowerBound(1) < LowerBound(0)) == false);

  REQUIRE((LowerBound() < LowerBound(-1)) == true);
  REQUIRE((LowerBound(-1) < LowerBound()) == false);

  REQUIRE((LowerBound() < LowerBound()) == false);
}


TEST_CASE("UpperBound < UpperBound", "[Bound][UpperBound][lt][less]") {
  REQUIRE((UpperBound(0) < UpperBound(1)) == true);
  REQUIRE((UpperBound(1) < UpperBound(0)) == false);

  REQUIRE((UpperBound(-1) < UpperBound()) == true);
  REQUIRE((UpperBound() < UpperBound(-1)) == false);

  REQUIRE((UpperBound() < UpperBound()) == false);
}


TEST_CASE("LowerBound < UpperBound", "[Bound][LowerBound][UpperBound][lt][less]") {
  REQUIRE((LowerBound(0) < UpperBound(1)) == true);
  REQUIRE((LowerBound(1) < UpperBound(0)) == false);

  REQUIRE((LowerBound(-1) < UpperBound()) == true);
  REQUIRE((LowerBound() < UpperBound(-1)) == true);
  REQUIRE((LowerBound() < UpperBound()) == true);
}


TEST_CASE("UpperBound < LowerBound", "[Bound][LowerBound][UpperBound][lt][less]") {
  REQUIRE((UpperBound(0) < LowerBound(1)) == true);
  REQUIRE((UpperBound(1) < LowerBound(0)) == false);

  REQUIRE((UpperBound(-1) < LowerBound()) == false);
  REQUIRE((UpperBound() < LowerBound(-1)) == false);
  REQUIRE((UpperBound() < LowerBound()) == false);
}


TEST_CASE("LowerBound vs int operator==", "[Bound][LowerBound][lt][less]") {
  REQUIRE((LowerBound() == 1) == false);
  REQUIRE((LowerBound(0) == 1) == false);
  REQUIRE((LowerBound(0) == 0) == true);
  REQUIRE((LowerBound(1) == 0) == false);

  REQUIRE((0 == LowerBound(1)) == false);
  REQUIRE((0 == LowerBound(0)) == true);
  REQUIRE((1 == LowerBound(0)) == false);
  REQUIRE((1 == LowerBound()) == false);

}


TEST_CASE("UpperBound vs int operator==", "[Bound][UpperBound][lt][less]") {
  REQUIRE((UpperBound(0) == 1) == false);
  REQUIRE((UpperBound(0) == 0) == true);
  REQUIRE((UpperBound(1) == 0) == false);
  REQUIRE((UpperBound() == 0) == false);

  REQUIRE((0 == UpperBound()) == false);
  REQUIRE((0 == UpperBound(1)) == false);
  REQUIRE((0 == UpperBound(0)) == true);
  REQUIRE((1 == UpperBound(0)) == false);

}


TEST_CASE("LowerBound vs int operator!=", "[Bound][LowerBound][lt][less]") {
  REQUIRE((LowerBound() != 1) == true);
  REQUIRE((LowerBound(0) != 1) == true);
  REQUIRE((LowerBound(0) != 0) == false);
  REQUIRE((LowerBound(1) != 0) == true);

  REQUIRE((0 != LowerBound(1)) == true);
  REQUIRE((0 != LowerBound(0)) == false);
  REQUIRE((1 != LowerBound(0)) == true);
  REQUIRE((1 != LowerBound()) == true);

}


TEST_CASE("UpperBound vs int operator!=", "[Bound][UpperBound][lt][less]") {
  REQUIRE((UpperBound(0) != 1) == true);
  REQUIRE((UpperBound(1) != 0) == true);
  REQUIRE((UpperBound(0) != 0) == false);
  REQUIRE((UpperBound() != 0) == true);

  REQUIRE((0 != UpperBound()) == true);
  REQUIRE((0 != UpperBound(0)) == false);
  REQUIRE((0 != UpperBound(1)) == true);
  REQUIRE((1 != UpperBound(0)) == true);

}


TEST_CASE("LowerBound vs int operator<", "[Bound][LowerBound][lt][less]") {
  REQUIRE((LowerBound() < 1) == true);
  REQUIRE((LowerBound(0) < 1) == true);
  REQUIRE((LowerBound(0) < 0) == false);
  REQUIRE((LowerBound(1) < 0) == false);

  REQUIRE((1 < LowerBound()) == false);
  REQUIRE((1 < LowerBound(0)) == false);
  REQUIRE((0 < LowerBound(0)) == false);
  REQUIRE((0 < LowerBound(1)) == true);

}


TEST_CASE("UpperBound vs int operator<", "[Bound][UpperBound][lt][less]") {
  REQUIRE((UpperBound(0) < 1) == true);
  REQUIRE((UpperBound(0) < 0) == false);
  REQUIRE((UpperBound(1) < 0) == false);
  REQUIRE((UpperBound() < 0) == false);

  REQUIRE((1 < UpperBound()) == true);
  REQUIRE((1 < UpperBound(0)) == false);
  REQUIRE((0 < UpperBound(0)) == false);
  REQUIRE((0 < UpperBound(1)) == true);

}


TEST_CASE("LowerBound vs int operator<=", "[Bound][LowerBound][lt][less]") {
  REQUIRE((LowerBound() <= 1) == true);
  REQUIRE((LowerBound(0) <= 1) == true);
  REQUIRE((LowerBound(0) <= 0) == true);
  REQUIRE((LowerBound(1) <= 0) == false);

  REQUIRE((1 <= LowerBound()) == false);
  REQUIRE((1 <= LowerBound(0)) == false);
  REQUIRE((0 <= LowerBound(0)) == true);
  REQUIRE((0 <= LowerBound(1)) == true);

}


TEST_CASE("UpperBound vs int operator<=", "[Bound][UpperBound][lt][less]") {
  REQUIRE((UpperBound(0) <= 1) == true);
  REQUIRE((UpperBound(0) <= 0) == true);
  REQUIRE((UpperBound(1) <= 0) == false);
  REQUIRE((UpperBound() <= 0) == false);

  REQUIRE((1 <= UpperBound()) == true);
  REQUIRE((1 <= UpperBound(0)) == false);
  REQUIRE((0 <= UpperBound(0)) == true);
  REQUIRE((0 <= UpperBound(1)) == true);

}


TEST_CASE("LowerBound vs int operator>", "[Bound][LowerBound][lt][less]") {
  REQUIRE((LowerBound() > 1) == false);
  REQUIRE((LowerBound(0) > 1) == false);
  REQUIRE((LowerBound(0) > 0) == false);
  REQUIRE((LowerBound(1) > 0) == true);

  REQUIRE((1 > LowerBound()) == true);
  REQUIRE((1 > LowerBound(0)) == true);
  REQUIRE((0 > LowerBound(0)) == false);
  REQUIRE((0 > LowerBound(1)) == false);

}


TEST_CASE("UpperBound vs int operator>", "[Bound][UpperBound][lt][less]") {
  REQUIRE((UpperBound(0) > 1) == false);
  REQUIRE((UpperBound(0) > 0) == false);
  REQUIRE((UpperBound(1) > 0) == true);
  REQUIRE((UpperBound() > 0) == true);

  REQUIRE((1 > UpperBound()) == false);
  REQUIRE((1 > UpperBound(0)) == true);
  REQUIRE((0 > UpperBound(0)) == false);
  REQUIRE((0 > UpperBound(1)) == false);

}


TEST_CASE("LowerBound vs int operator>=", "[Bound][LowerBound][lt][less]") {
  REQUIRE((LowerBound() >= 1) == false);
  REQUIRE((LowerBound(0) >= 1) == false);
  REQUIRE((LowerBound(0) >= 0) == true);
  REQUIRE((LowerBound(1) >= 0) == true);

  REQUIRE((1 >= LowerBound()) == true);
  REQUIRE((1 >= LowerBound(0)) == true);
  REQUIRE((0 >= LowerBound(0)) == true);
  REQUIRE((0 >= LowerBound(1)) == false);

}


TEST_CASE("UpperBound vs int operator>=", "[Bound][UpperBound][lt][less]") {
  REQUIRE((UpperBound(0) >= 1) == false);
  REQUIRE((UpperBound(0) >= 0) == true);
  REQUIRE((UpperBound(1) >= 0) == true);
  REQUIRE((UpperBound() >= 0) == true);

  REQUIRE((1 >= UpperBound()) == false);
  REQUIRE((1 >= UpperBound(0)) == true);
  REQUIRE((0 >= UpperBound(0)) == true);
  REQUIRE((0 >= UpperBound(1)) == false);

}
