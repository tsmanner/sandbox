#include "Bound.h"


TEST_CASE("Bound::set", "[Bound][set]") {
  auto b = Bound(0, Bound::cOpen);

  b.set(1);
  CHECK(b.getType() == Bound::cClosed);
  CHECK(b.getValue() == 1);

  b.set<Bound::cClosed>(2);
  CHECK(b.getType() == Bound::cClosed);
  CHECK(b.getValue() == 2);

  b.set<Bound::cOpen>();
  CHECK(b.getType() == Bound::cOpen);
}


TEST_CASE("Bound::operator=", "[Bound][assignment]") {
  auto b = Bound(0, Bound::cOpen);

  CHECK(b.getType() == Bound::cOpen);
  b = 1;
  CHECK(b.getType() == Bound::cClosed);
  CHECK(b.getValue() == 1);

}


TEST_CASE("LowerBound::operator=", "[Bound][LowerBound][assignment]") {
  auto b = LowerBound();

  CHECK(b.getType() == Bound::cOpen);
  b = 1;
  CHECK(b.getType() == Bound::cClosed);
  CHECK(b.getValue() == 1);

}


TEST_CASE("UpperBound::operator=", "[Bound][UpperBound][assignment]") {
  auto b = UpperBound();

  CHECK(b.getType() == Bound::cOpen);
  b = 1;
  CHECK(b.getType() == Bound::cClosed);
  CHECK(b.getValue() == 1);

}


TEST_CASE("LowerBound < LowerBound", "[Bound][LowerBound][lt][less]") {
  CHECK(LowerBound(0) < LowerBound(1));
  CHECK(!(LowerBound(1) < LowerBound(0)));

  CHECK(LowerBound() < LowerBound(-1));
  CHECK(!(LowerBound(-1) < LowerBound()));

  CHECK(!(LowerBound() < LowerBound()));
}


TEST_CASE("UpperBound < UpperBound", "[Bound][UpperBound][lt][less]") {
  CHECK(UpperBound(0) < UpperBound(1));
  CHECK(!(UpperBound(1) < UpperBound(0)));

  CHECK(UpperBound(-1) < UpperBound());
  CHECK(!(UpperBound() < UpperBound(-1)));

  CHECK(!(UpperBound() < UpperBound()));
}


TEST_CASE("LowerBound < UpperBound", "[Bound][LowerBound][UpperBound][lt][less]") {
  CHECK(  LowerBound(0) < UpperBound(1));
  CHECK(!(LowerBound(1) < UpperBound(0)));

  CHECK(LowerBound(-1) < UpperBound());
  CHECK(LowerBound()  < UpperBound(-1));
  CHECK(LowerBound()  < UpperBound());
}


TEST_CASE("UpperBound < LowerBound", "[Bound][LowerBound][UpperBound][lt][less]") {
  CHECK(  UpperBound(0) < LowerBound(1));
  CHECK(!(UpperBound(1) < LowerBound(0)));

  CHECK(!(UpperBound(-1) < LowerBound()));
  CHECK(!(UpperBound()   < LowerBound(-1)));
  CHECK(!(UpperBound()   < LowerBound()));
}


TEST_CASE("LowerBound vs int operator==", "[Bound][LowerBound][eq][equal]") {
  CHECK(!(LowerBound()  == 1));
  CHECK(!(LowerBound(0) == 1));
  CHECK(  LowerBound(0) == 0);
  CHECK(!(LowerBound(1) == 0));

  CHECK(!(0 == LowerBound(1)));
  CHECK(  0 == LowerBound(0));
  CHECK(!(1 == LowerBound(0)));
  CHECK(!(1 == LowerBound()));

}


TEST_CASE("UpperBound vs int operator==", "[Bound][UpperBound][eq][equal]") {
  CHECK(!(UpperBound(0) == 1));
  CHECK(  UpperBound(0) == 0);
  CHECK(!(UpperBound(1) == 0));
  CHECK(!(UpperBound()  == 0));

  CHECK(!(0 == UpperBound()));
  CHECK(!(0 == UpperBound(1)));
  CHECK(  0 == UpperBound(0));
  CHECK(!(1 == UpperBound(0)));

}


TEST_CASE("LowerBound vs UpperBound operator==", "[Bound][LowerBound][UpperBound][eq][equal]") {
  CHECK(!(LowerBound(0) == UpperBound(1)));
  CHECK(  LowerBound(0) == UpperBound(0));
  CHECK(!(LowerBound(1) == UpperBound(0)));
  CHECK(!(LowerBound()  == UpperBound(0)));

  CHECK(!(LowerBound(0) == UpperBound()));
  CHECK(!(LowerBound(0) == UpperBound(1)));
  CHECK(  LowerBound(0) == UpperBound(0));
  CHECK(!(LowerBound(1) == UpperBound(0)));
  CHECK(!(LowerBound()  == UpperBound()));

}


TEST_CASE("LowerBound vs int operator!=", "[Bound][LowerBound][ne][notequal]") {
  CHECK(LowerBound() != 1);
  CHECK(LowerBound(0) != 1);
  CHECK(!(LowerBound(0) != 0));
  CHECK(LowerBound(1) != 0);

  CHECK(0 != LowerBound(1));
  CHECK(!(0 != LowerBound(0)));
  CHECK(1 != LowerBound(0));
  CHECK(1 != LowerBound());

}


TEST_CASE("UpperBound vs int operator!=", "[Bound][UpperBound][ne][notequal]") {
  CHECK(UpperBound(0) != 1);
  CHECK(UpperBound(1) != 0);
  CHECK(!(UpperBound(0) != 0));
  CHECK(UpperBound() != 0);

  CHECK(0 != UpperBound());
  CHECK(!(0 != UpperBound(0)));
  CHECK(0 != UpperBound(1));
  CHECK(1 != UpperBound(0));

}


TEST_CASE("LowerBound vs int operator<", "[Bound][LowerBound][lt][less]") {
  CHECK(LowerBound() < 1);
  CHECK(LowerBound(0) < 1);
  CHECK(!(LowerBound(0) < 0));
  CHECK(!(LowerBound(1) < 0));

  CHECK(!(1 < LowerBound()));
  CHECK(!(1 < LowerBound(0)));
  CHECK(!(0 < LowerBound(0)));
  CHECK(0 < LowerBound(1));

}


TEST_CASE("UpperBound vs int operator<", "[Bound][UpperBound][lt][less]") {
  CHECK(UpperBound(0) < 1);
  CHECK(!(UpperBound(0) < 0));
  CHECK(!(UpperBound(1) < 0));
  CHECK(!(UpperBound() < 0));

  CHECK(1 < UpperBound());
  CHECK(!(1 < UpperBound(0)));
  CHECK(!(0 < UpperBound(0)));
  CHECK(0 < UpperBound(1));

}


TEST_CASE("LowerBound vs int operator<=", "[Bound][LowerBound][le][lessequal]") {
  CHECK(LowerBound() <= 1);
  CHECK(LowerBound(0) <= 1);
  CHECK(LowerBound(0) <= 0);
  CHECK(!(LowerBound(1) <= 0));

  CHECK(!(1 <= LowerBound()));
  CHECK(!(1 <= LowerBound(0)));
  CHECK(0 <= LowerBound(0));
  CHECK(0 <= LowerBound(1));

}


TEST_CASE("UpperBound vs int operator<=", "[Bound][UpperBound][le][lessequal]") {
  CHECK(UpperBound(0) <= 1);
  CHECK(UpperBound(0) <= 0);
  CHECK(!(UpperBound(1) <= 0));
  CHECK(!(UpperBound() <= 0));

  CHECK(1 <= UpperBound());
  CHECK(!(1 <= UpperBound(0)));
  CHECK(0 <= UpperBound(0));
  CHECK(0 <= UpperBound(1));

}


TEST_CASE("LowerBound vs int operator>", "[Bound][LowerBound][gt][greater]") {
  CHECK(!(LowerBound() > 1));
  CHECK(!(LowerBound(0) > 1));
  CHECK(!(LowerBound(0) > 0));
  CHECK(LowerBound(1) > 0);

  CHECK(1 > LowerBound());
  CHECK(1 > LowerBound(0));
  CHECK(!(0 > LowerBound(0)));
  CHECK(!(0 > LowerBound(1)));

}


TEST_CASE("UpperBound vs int operator>", "[Bound][UpperBound][gt][greater]") {
  CHECK(!(UpperBound(0) > 1));
  CHECK(!(UpperBound(0) > 0));
  CHECK(UpperBound(1) > 0);
  CHECK(UpperBound() > 0);

  CHECK(!(1 > UpperBound()));
  CHECK(1 > UpperBound(0));
  CHECK(!(0 > UpperBound(0)));
  CHECK(!(0 > UpperBound(1)));

}


TEST_CASE("LowerBound vs int operator>=", "[Bound][LowerBound][ge][greaterequal]") {
  CHECK(!(LowerBound() >= 1));
  CHECK(!(LowerBound(0) >= 1));
  CHECK(LowerBound(0) >= 0);
  CHECK(LowerBound(1) >= 0);

  CHECK(1 >= LowerBound());
  CHECK(1 >= LowerBound(0));
  CHECK(0 >= LowerBound(0));
  CHECK(!(0 >= LowerBound(1)));

}


TEST_CASE("UpperBound vs int operator>=", "[Bound][UpperBound][ge][greaterequal]") {
  CHECK(!(UpperBound(0) >= 1));
  CHECK(UpperBound(0) >= 0);
  CHECK(UpperBound(1) >= 0);
  CHECK(UpperBound() >= 0);

  CHECK(!(1 >= UpperBound()));
  CHECK(1 >= UpperBound(0));
  CHECK(0 >= UpperBound(0));
  CHECK(!(0 >= UpperBound(1)));

}
