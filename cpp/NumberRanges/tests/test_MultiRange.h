#include "MultiRange.h"


TEST_CASE("MultiRange Constructor", "[MultiRange]") {
  auto mr = MultiRange();
  CHECK(mr.getRanges().size() == 0);
}


TEST_CASE("MultiRange addRange", "[MultiRange][addRange]") {
  auto mr = MultiRange();
  auto originalRange = Range(-1, 1);
  mr.addRange(originalRange);

  // Make sure there's only 1 thing in there
  CHECK(mr.getRanges().size() == 1);

  // Make sure that 1 thing is the RIGHT thing
  auto beginRange = *mr.getRanges().begin();
  CHECK(originalRange.getLowerBound() == beginRange.getLowerBound());
  CHECK(originalRange.getUpperBound() == beginRange.getUpperBound());

}


TEST_CASE("MultiRange covers", "[MultiRange][covers]") {
  auto mr = MultiRange();
  CHECK(!mr.covers(0));

  mr.addRange(Range(-1, 1));
  CHECK(mr.covers(0));

  mr.addRange(Range(3, 5));
  CHECK(!mr.covers(2));

  mr.addRange(Range(LowerBound(), -5));
  CHECK(mr.covers(-20));
  CHECK(mr.covers(LowerBound()));

  mr.addRange(Range(10, UpperBound()));
  CHECK(mr.covers(20));
  CHECK(mr.covers(UpperBound()));

}


TEST_CASE("MultiRange optimize", "[MultiRange][optimize]") {
  auto mr = MultiRange();
  mr.addRange(Range(-1, 1));
  mr.addRange(Range(3, 5));
  CHECK(mr.getRanges().size() == 2);
  CHECK(!mr.covers(2));

  mr.addRange(Range(2, 2));
  CHECK(mr.getRanges().size() == 1);
  CHECK(mr.covers(-1));
  CHECK(mr.covers(5));

  mr.addRange(Range(7, 10));
  CHECK(mr.getRanges().size() == 2);

  mr.addRange(Range(12, 15));
  CHECK(mr.getRanges().size() == 3);

  mr.addRange(Range(14, 20));
  CHECK(mr.getRanges().size() == 3);

}


TEST_CASE("Disjunction of one RangeSet", "[MultiRange][RangeSet][disjunction]") {
  auto rs = MultiRange::RangeSet();
  rs.insert(Range(-1, 1));
  rs.insert(Range(3, 5));
  rs = MultiRange::disjunction(rs);
  CHECK(rs.size() == 2);

  rs.insert(Range(2, 2));
  rs = MultiRange::disjunction(rs);
  CHECK(rs.size() == 1);
  CHECK(rs.begin()->getLowerBound() == -1);
  CHECK(rs.begin()->getUpperBound() ==  5);

}


TEST_CASE("Conjunction of one RangeSet", "[MultiRange][RangeSet][conjunction]") {
  auto rs = MultiRange::RangeSet();
  rs.insert(Range(-1, 1));
  rs.insert(Range(3, 5));
  rs = MultiRange::conjunction(rs);
  CHECK(rs.size() == 0);

  rs.insert(Range(-1, 1));
  rs.insert(Range(1, 3));
  rs = MultiRange::conjunction(rs);
  CHECK(rs.size() == 1);
  CHECK(rs.begin()->getLowerBound() == 1);
  CHECK(rs.begin()->getUpperBound() == 1);

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

  CHECK(!disjunction.covers(-2));
  CHECK( disjunction.covers(-1));
  CHECK( disjunction.covers( 0));
  CHECK( disjunction.covers( 1));
  CHECK(!disjunction.covers( 2));
  CHECK( disjunction.covers( 3));
  CHECK( disjunction.covers( 4));
  CHECK( disjunction.covers( 5));
  CHECK(!disjunction.covers( 6));
  CHECK( disjunction.covers( 7));
  CHECK( disjunction.covers( 8));
  CHECK(!disjunction.covers( 9));

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

  CHECK( conjunction.covers(-1));
  CHECK(!conjunction.covers( 0));
  CHECK( conjunction.covers( 1));
  CHECK(!conjunction.covers( 2));
  CHECK(!conjunction.covers( 3));
  CHECK( conjunction.covers( 4));
  CHECK(!conjunction.covers( 5));

}


TEST_CASE("MultiRange stream operator") {
  auto mr = MultiRange();
  mr.addRange(Range(-1, 1));
  mr.addRange(Range(3, 5));
  std::stringstream ss;
  ss << mr;
  CHECK(ss.str() == "([-1:1], [3:5])");
}


TEST_CASE("MultiRange size") {
  auto mr = MultiRange();
  mr.addRange(Range(-1, 1));
  mr.addRange(Range(3, 5));
  CHECK(mr.size() == 6);
}


TEST_CASE("MultiRange draw") {
  auto mr = MultiRange();
  mr.addRange(Range(-1, 1));
  mr.addRange(Range(3, 5));
  CHECK(mr.covers(mr.draw()));
}
