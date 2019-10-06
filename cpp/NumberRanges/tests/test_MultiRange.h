#include "MultiRange.h"


TEST_CASE("MultiRange Constructor", "[MultiRange]") {
  auto mr = MultiRange();
  REQUIRE(mr.getRanges().size() == 0);
}


TEST_CASE("MultiRange addRange", "[MultiRange][optimize]") {
  auto mr = MultiRange();
  auto originalRange = Range(-1, 1);
  mr.addRange(originalRange);

  // Make sure there's only 1 thing in there
  REQUIRE(mr.getRanges().size() == 1);

  // Make sure that 1 thing is the RIGHT thing
  auto beginRange = *mr.getRanges().begin();
  REQUIRE(originalRange.getLowerBound() == beginRange.getLowerBound());
  REQUIRE(originalRange.getUpperBound() == beginRange.getUpperBound());

}


TEST_CASE("MultiRange optimize", "[MultiRange][optimize]") {
  auto mr = MultiRange();
  mr.addRange(Range(-1, 1));
  mr.addRange(Range(3, 5));
  REQUIRE(mr.getRanges().size() == 2);

  mr.addRange(Range(2, 2));
  REQUIRE(mr.getRanges().size() == 1);

  mr.addRange(Range(7, 10));
  REQUIRE(mr.getRanges().size() == 2);

  mr.addRange(Range(12, 15));
  REQUIRE(mr.getRanges().size() == 3);

  mr.addRange(Range(14, 20));
  REQUIRE(mr.getRanges().size() == 3);

}


TEST_CASE("MultiRange covers", "[MultiRange][covers]") {
  auto mr = MultiRange();
  REQUIRE(!mr.covers(0));

  mr.addRange(Range(-1, 1));
  REQUIRE(mr.covers(0));

  mr.addRange(Range(3, 5));
  REQUIRE(!mr.covers(2));

  mr.addRange(Range(LowerBound(), -5));
  REQUIRE(mr.covers(-20));
  REQUIRE(mr.covers(LowerBound()));

  mr.addRange(Range(10, UpperBound()));
  REQUIRE(mr.covers(20));
  REQUIRE(mr.covers(UpperBound()));

}
