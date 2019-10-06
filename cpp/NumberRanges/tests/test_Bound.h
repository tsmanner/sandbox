#include "Bound.h"


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
  REQUIRE(LowerBound(0) < LowerBound(1));
  REQUIRE(!(LowerBound(1) < LowerBound(0)));

  REQUIRE(LowerBound() < LowerBound(-1));
  REQUIRE(!(LowerBound(-1) < LowerBound()));

  REQUIRE(!(LowerBound() < LowerBound()));
}


TEST_CASE("UpperBound < UpperBound", "[Bound][UpperBound][lt][less]") {
  REQUIRE(UpperBound(0) < UpperBound(1));
  REQUIRE(!(UpperBound(1) < UpperBound(0)));

  REQUIRE(UpperBound(-1) < UpperBound());
  REQUIRE(!(UpperBound() < UpperBound(-1)));

  REQUIRE(!(UpperBound() < UpperBound()));
}


TEST_CASE("LowerBound < UpperBound", "[Bound][LowerBound][UpperBound][lt][less]") {
  REQUIRE(LowerBound(0) < UpperBound(1));
  REQUIRE(!(LowerBound(1) < UpperBound(0)));

  REQUIRE(LowerBound(-1) < UpperBound());
  REQUIRE(LowerBound() < UpperBound(-1));
  REQUIRE(LowerBound() < UpperBound());
}


TEST_CASE("UpperBound < LowerBound", "[Bound][LowerBound][UpperBound][lt][less]") {
  REQUIRE(UpperBound(0) < LowerBound(1));
  REQUIRE(!(UpperBound(1) < LowerBound(0)));

  REQUIRE(!(UpperBound(-1) < LowerBound()));
  REQUIRE(!(UpperBound() < LowerBound(-1)));
  REQUIRE(!(UpperBound() < LowerBound()));
}


TEST_CASE("LowerBound vs int operator==", "[Bound][LowerBound][lt][less]") {
  REQUIRE(!(LowerBound() == 1));
  REQUIRE(!(LowerBound(0) == 1));
  REQUIRE(LowerBound(0) == 0);
  REQUIRE(!(LowerBound(1) == 0));

  REQUIRE(!(0 == LowerBound(1)));
  REQUIRE(0 == LowerBound(0));
  REQUIRE(!(1 == LowerBound(0)));
  REQUIRE(!(1 == LowerBound()));

}


TEST_CASE("UpperBound vs int operator==", "[Bound][UpperBound][lt][less]") {
  REQUIRE(!(UpperBound(0) == 1));
  REQUIRE(UpperBound(0) == 0);
  REQUIRE(!(UpperBound(1) == 0));
  REQUIRE(!(UpperBound() == 0));

  REQUIRE(!(0 == UpperBound()));
  REQUIRE(!(0 == UpperBound(1)));
  REQUIRE(0 == UpperBound(0));
  REQUIRE(!(1 == UpperBound(0)));

}


TEST_CASE("LowerBound vs int operator!=", "[Bound][LowerBound][lt][less]") {
  REQUIRE(LowerBound() != 1);
  REQUIRE(LowerBound(0) != 1);
  REQUIRE(!(LowerBound(0) != 0));
  REQUIRE(LowerBound(1) != 0);

  REQUIRE(0 != LowerBound(1));
  REQUIRE(!(0 != LowerBound(0)));
  REQUIRE(1 != LowerBound(0));
  REQUIRE(1 != LowerBound());

}


TEST_CASE("UpperBound vs int operator!=", "[Bound][UpperBound][lt][less]") {
  REQUIRE(UpperBound(0) != 1);
  REQUIRE(UpperBound(1) != 0);
  REQUIRE(!(UpperBound(0) != 0));
  REQUIRE(UpperBound() != 0);

  REQUIRE(0 != UpperBound());
  REQUIRE(!(0 != UpperBound(0)));
  REQUIRE(0 != UpperBound(1));
  REQUIRE(1 != UpperBound(0));

}


TEST_CASE("LowerBound vs int operator<", "[Bound][LowerBound][lt][less]") {
  REQUIRE(LowerBound() < 1);
  REQUIRE(LowerBound(0) < 1);
  REQUIRE(!(LowerBound(0) < 0));
  REQUIRE(!(LowerBound(1) < 0));

  REQUIRE(!(1 < LowerBound()));
  REQUIRE(!(1 < LowerBound(0)));
  REQUIRE(!(0 < LowerBound(0)));
  REQUIRE(0 < LowerBound(1));

}


TEST_CASE("UpperBound vs int operator<", "[Bound][UpperBound][lt][less]") {
  REQUIRE(UpperBound(0) < 1);
  REQUIRE(!(UpperBound(0) < 0));
  REQUIRE(!(UpperBound(1) < 0));
  REQUIRE(!(UpperBound() < 0));

  REQUIRE(1 < UpperBound());
  REQUIRE(!(1 < UpperBound(0)));
  REQUIRE(!(0 < UpperBound(0)));
  REQUIRE(0 < UpperBound(1));

}


TEST_CASE("LowerBound vs int operator<=", "[Bound][LowerBound][lt][less]") {
  REQUIRE(LowerBound() <= 1);
  REQUIRE(LowerBound(0) <= 1);
  REQUIRE(LowerBound(0) <= 0);
  REQUIRE(!(LowerBound(1) <= 0));

  REQUIRE(!(1 <= LowerBound()));
  REQUIRE(!(1 <= LowerBound(0)));
  REQUIRE(0 <= LowerBound(0));
  REQUIRE(0 <= LowerBound(1));

}


TEST_CASE("UpperBound vs int operator<=", "[Bound][UpperBound][lt][less]") {
  REQUIRE(UpperBound(0) <= 1);
  REQUIRE(UpperBound(0) <= 0);
  REQUIRE(!(UpperBound(1) <= 0));
  REQUIRE(!(UpperBound() <= 0));

  REQUIRE(1 <= UpperBound());
  REQUIRE(!(1 <= UpperBound(0)));
  REQUIRE(0 <= UpperBound(0));
  REQUIRE(0 <= UpperBound(1));

}


TEST_CASE("LowerBound vs int operator>", "[Bound][LowerBound][lt][less]") {
  REQUIRE(!(LowerBound() > 1));
  REQUIRE(!(LowerBound(0) > 1));
  REQUIRE(!(LowerBound(0) > 0));
  REQUIRE(LowerBound(1) > 0);

  REQUIRE(1 > LowerBound());
  REQUIRE(1 > LowerBound(0));
  REQUIRE(!(0 > LowerBound(0)));
  REQUIRE(!(0 > LowerBound(1)));

}


TEST_CASE("UpperBound vs int operator>", "[Bound][UpperBound][lt][less]") {
  REQUIRE(!(UpperBound(0) > 1));
  REQUIRE(!(UpperBound(0) > 0));
  REQUIRE(UpperBound(1) > 0);
  REQUIRE(UpperBound() > 0);

  REQUIRE(!(1 > UpperBound()));
  REQUIRE(1 > UpperBound(0));
  REQUIRE(!(0 > UpperBound(0)));
  REQUIRE(!(0 > UpperBound(1)));

}


TEST_CASE("LowerBound vs int operator>=", "[Bound][LowerBound][lt][less]") {
  REQUIRE(!(LowerBound() >= 1));
  REQUIRE(!(LowerBound(0) >= 1));
  REQUIRE(LowerBound(0) >= 0);
  REQUIRE(LowerBound(1) >= 0);

  REQUIRE(1 >= LowerBound());
  REQUIRE(1 >= LowerBound(0));
  REQUIRE(0 >= LowerBound(0));
  REQUIRE(!(0 >= LowerBound(1)));

}


TEST_CASE("UpperBound vs int operator>=", "[Bound][UpperBound][lt][less]") {
  REQUIRE(!(UpperBound(0) >= 1));
  REQUIRE(UpperBound(0) >= 0);
  REQUIRE(UpperBound(1) >= 0);
  REQUIRE(UpperBound() >= 0);

  REQUIRE(!(1 >= UpperBound()));
  REQUIRE(1 >= UpperBound(0));
  REQUIRE(0 >= UpperBound(0));
  REQUIRE(!(0 >= UpperBound(1)));

}
