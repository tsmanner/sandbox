#include "MultiRange.h"


TEST_CASE("MultiRange Constructor", "[MultiRange]") {
  auto mr = MultiRange();
  REQUIRE(mr.getRanges().size() == 0);
}


TEST_CASE("MultiRange addRange", "[MultiRange][addRange]") {
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


TEST_CASE("MultiRange optimize", "[MultiRange][optimize]") {
  auto mr = MultiRange();
  mr.addRange(Range(-1, 1));
  mr.addRange(Range(3, 5));
  REQUIRE(mr.getRanges().size() == 2);
  REQUIRE(!mr.covers(2));

  mr.addRange(Range(2, 2));
  REQUIRE(mr.getRanges().size() == 1);
  REQUIRE(mr.covers(-1));
  REQUIRE(mr.covers(5));

  mr.addRange(Range(7, 10));
  REQUIRE(mr.getRanges().size() == 2);

  mr.addRange(Range(12, 15));
  REQUIRE(mr.getRanges().size() == 3);

  mr.addRange(Range(14, 20));
  REQUIRE(mr.getRanges().size() == 3);

}


TEST_CASE("Disjunction of one RangeSet", "[MultiRange][RangeSet][disjunction]") {
  auto rs = MultiRange::RangeSet();
  rs.insert(Range(-1, 1));
  rs.insert(Range(3, 5));
  rs = MultiRange::disjunction(rs);
  REQUIRE(rs.size() == 2);

  rs.insert(Range(2, 2));
  rs = MultiRange::disjunction(rs);
  REQUIRE(rs.size() == 1);
  REQUIRE(rs.begin()->getLowerBound() == -1);
  REQUIRE(rs.begin()->getUpperBound() ==  5);

}


TEST_CASE("Conjunction of one RangeSet", "[MultiRange][RangeSet][conjunction]") {
  auto rs = MultiRange::RangeSet();
  rs.insert(Range(-1, 1));
  rs.insert(Range(3, 5));
  rs = MultiRange::conjunction(rs);
  REQUIRE(rs.size() == 0);

  rs.insert(Range(-1, 1));
  rs.insert(Range(1, 3));
  rs = MultiRange::conjunction(rs);
  REQUIRE(rs.size() == 1);
  REQUIRE(rs.begin()->getLowerBound() == 1);
  REQUIRE(rs.begin()->getUpperBound() == 1);

}


TEST_CASE("Disjunction of two MultiRanges", "[MultiRange][disjunction]") {
  auto lhs = MultiRange();
  lhs.addRange(Range(-1, 1));
  lhs.addRange(Range(3, 5));

  auto rhs = MultiRange();
  rhs.addRange(Range(-1, -1));
  rhs.addRange(Range(1, 1));
  rhs.addRange(Range(4, 4));
  rhs.addRange(Range(7, 8));

  auto disjunction = MultiRange::disjunction(lhs, rhs);

  REQUIRE(!disjunction.covers(-2));
  REQUIRE( disjunction.covers(-1));
  REQUIRE( disjunction.covers( 0));
  REQUIRE( disjunction.covers( 1));
  REQUIRE(!disjunction.covers( 2));
  REQUIRE( disjunction.covers( 3));
  REQUIRE( disjunction.covers( 4));
  REQUIRE( disjunction.covers( 5));
  REQUIRE(!disjunction.covers( 6));
  REQUIRE( disjunction.covers( 7));
  REQUIRE( disjunction.covers( 8));
  REQUIRE(!disjunction.covers( 9));

}


TEST_CASE("Conjunction of two MultiRanges", "[MultiRange][conjunction]") {
  auto lhs = MultiRange();
  lhs.addRange(Range(-1, 1));
  lhs.addRange(Range(3, 5));

  auto rhs = MultiRange();
  rhs.addRange(Range(-1, -1));
  rhs.addRange(Range(1, 1));
  rhs.addRange(Range(4, 4));

  auto conjunction = MultiRange::conjunction(lhs, rhs);

  REQUIRE( conjunction.covers(-1));
  REQUIRE(!conjunction.covers( 0));
  REQUIRE( conjunction.covers( 1));
  REQUIRE(!conjunction.covers( 2));
  REQUIRE(!conjunction.covers( 3));
  REQUIRE( conjunction.covers( 4));
  REQUIRE(!conjunction.covers( 5));

}
