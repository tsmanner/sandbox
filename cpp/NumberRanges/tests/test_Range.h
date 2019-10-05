#include <sstream>

#include "Range.h"


TEST_CASE("Range Default Constructor [:]", "[Range]") {
  auto r = Range();
  REQUIRE(r.getLowerBound().getType() == Bound::cOpen);
  REQUIRE(r.getUpperBound().getType() == Bound::cOpen);
}


TEST_CASE("Range Value Constructor [:1]", "[Range]") {
  auto r = Range(LowerBound(), 1);
  REQUIRE(r.getLowerBound().getType() == Bound::cOpen);
  REQUIRE(r.getUpperBound().getType() == Bound::cClosed);
  REQUIRE(r.getUpperBound().getValue() == 1);
}


TEST_CASE("Range Value Constructor [-1:]", "[Range]") {
  auto r = Range(-1, UpperBound());
  REQUIRE(r.getLowerBound().getType() == Bound::cClosed);
  REQUIRE(r.getLowerBound().getValue() == -1);
  REQUIRE(r.getUpperBound().getType() == Bound::cOpen);
}


TEST_CASE("Range Value Constructor [-1:1]", "[Range]") {
  auto r = Range(-1, 1);
  REQUIRE(r.getLowerBound().getType() == Bound::cClosed);
  REQUIRE(r.getLowerBound().getValue() == -1);
  REQUIRE(r.getUpperBound().getType() == Bound::cClosed);
  REQUIRE(r.getUpperBound().getValue() == 1);
}


TEST_CASE("Range [:] Covers Value", "[Range][covers]") {
  auto r = Range();
  REQUIRE(r.covers(-872346));
  REQUIRE(r.covers(0));
  REQUIRE(r.covers(872346));
}


TEST_CASE("Range [:1] Covers Value", "[Range][covers]") {
  auto r = Range(LowerBound(), 1);
  REQUIRE( r.covers(-1));
  REQUIRE( r.covers(1));
  REQUIRE(!r.covers(872346));
}


TEST_CASE("Range [-1:] Covers Value", "[Range][covers]") {
  auto r = Range(-1, UpperBound());
  REQUIRE(!r.covers(-872346));
  REQUIRE( r.covers(-1));
  REQUIRE( r.covers(1));
}


TEST_CASE("Range [-1:1] Covers Value", "[Range][covers]") {
  auto r = Range(-1, 1);
  REQUIRE(!r.covers(-872346));
  REQUIRE( r.covers(-1));
  REQUIRE( r.covers(0));
  REQUIRE( r.covers(1));
  REQUIRE(!r.covers(872346));
}


TEST_CASE("Bounded Range Overlap", "[Range][overlap]") {
  //
  // Overlap Cases:
  //

  // ---
  //   ---
  REQUIRE(Range::overlaps(Range(-1, 1), Range(1, 3)));
  //   ---
  // ---
  REQUIRE(Range::overlaps(Range(-1, 1), Range(-3, -1)));
  //  ---
  // -----
  REQUIRE(Range::overlaps(Range(-1, 1), Range(-2, 2)));
  // -----
  //  ---
  REQUIRE(Range::overlaps(Range(-2, 2), Range(-1, 1)));

  //
  // Non-overlap Cases
  //

  // ---
  //    ---
  REQUIRE(!Range::overlaps(Range(-1, 1), Range(2, 4)));
  //    ---
  // ---
  REQUIRE(!Range::overlaps(Range(-4, -2), Range(-1, 1)));
  // ---
  //     ---
  REQUIRE(!Range::overlaps(Range(-1, 1), Range(3, 5)));
  //     ---
  // ---
  REQUIRE(!Range::overlaps(Range(-5, -3), Range(-1, 1)));

}


TEST_CASE("Nonbounded Range Overlap", "[Range][overlap]") {
  //
  // Overlap Cases:
  //

  // If either range has no bounds, it must overlap the other.
  REQUIRE(Range::overlaps(Range(), Range()));
  REQUIRE(Range::overlaps(Range(-1, 1), Range()));
  REQUIRE(Range::overlaps(Range(), Range(-1, 1)));

  REQUIRE(Range::overlaps(Range(LowerBound(),            1), Range(          -1,           1)));
  REQUIRE(Range::overlaps(Range(LowerBound(),            1), Range(          -1, UpperBound())));
  REQUIRE(Range::overlaps(Range(          -1,            1), Range(LowerBound(),           1)));
  REQUIRE(Range::overlaps(Range(          -1, UpperBound()), Range(LowerBound(),           1)));
  REQUIRE(Range::overlaps(Range(          -1, UpperBound()), Range(          -1,           1)));
  REQUIRE(Range::overlaps(Range(          -1,            1), Range(          -1, UpperBound())));

}


TEST_CASE("Nonbounded Range No Overlap", "[Range][overlap]") {
  //
  // Non-overlap Cases
  //

  REQUIRE(!Range::overlaps(Range(LowerBound(), -2           ), Range(          -1,            1)));
  REQUIRE(!Range::overlaps(Range(LowerBound(), -2           ), Range(          -1, UpperBound())));
  REQUIRE(!Range::overlaps(Range(           2,  4           ), Range(LowerBound(),            1)));
  REQUIRE(!Range::overlaps(Range(           2,  UpperBound()), Range(LowerBound(),            1)));
  REQUIRE(!Range::overlaps(Range(           2,  UpperBound()), Range(          -1,            1)));
  REQUIRE(!Range::overlaps(Range(          -4, -2           ), Range(          -1, UpperBound())));

}


TEST_CASE("Range conjunction - overlapping ranges", "[Range][conjunction]") {
  auto r = Range::conjunction(Range(-2, 0), Range(0, 2));
  REQUIRE(r.getLowerBound() == 0);
  REQUIRE(r.getUpperBound() == 0);

  r = Range::conjunction(Range(LowerBound(), 0), Range(0, 2));
  REQUIRE(r.getLowerBound() == 0);
  REQUIRE(r.getUpperBound() == 0);

  r = Range::conjunction(Range(-2, UpperBound()), Range(0, 2));
  REQUIRE(r.getLowerBound() == 0);
  REQUIRE(r.getUpperBound() == 2);

  r = Range::conjunction(Range(), Range(0, 2));
  REQUIRE(r.getLowerBound() == 0);
  REQUIRE(r.getUpperBound() == 2);
}


TEST_CASE("Range conjunction - non-overlapping ranges", "[Range][conjunction]") {
  try {
    Range::conjunction(Range(-2, -1), Range(1, 2));
  } catch (Range::NonOverlappingRangeConjunction& e) {
    REQUIRE(true);
  }
}


TEST_CASE("Range disjunction - overlapping ranges", "[Range][disjunction]") {
  auto r = Range::disjunction(Range(-2, 0), Range(0, 2));
  REQUIRE(r.getLowerBound() == -2);
  REQUIRE(r.getUpperBound() ==  2);

  r = Range::disjunction(Range(LowerBound(), 0), Range(0, 2));
  REQUIRE(r.getLowerBound().getType() == Bound::cOpen);
  REQUIRE(r.getUpperBound() == 2);

  r = Range::disjunction(Range(-2, UpperBound()), Range(0, 2));
  REQUIRE(r.getLowerBound() == -2);
  REQUIRE(r.getUpperBound().getType() == Bound::cOpen);

  r = Range::disjunction(Range(), Range(0, 2));
  REQUIRE(r.getLowerBound().getType() == Bound::cOpen);
  REQUIRE(r.getUpperBound().getType() == Bound::cOpen);
}


TEST_CASE("Range disjunction - non-overlapping ranges", "[Range][disjunction]") {
  try {
    Range::disjunction(Range(-2, -1), Range(1, 2));
  } catch (Range::NonOverlappingRangeDisjunction& e) {
    REQUIRE(true);
  }
}


TEST_CASE("Range stream operator", "[Range]") {
  {
    std::stringstream ss;
    ss << Range();
    REQUIRE(ss.str() == "[:]");
  }
  {
    std::stringstream ss;
    ss << Range(LowerBound(), 1);
    REQUIRE(ss.str() == "[:1]");
  }
  {
    std::stringstream ss;
    ss << Range(-1, UpperBound());
    REQUIRE(ss.str() == "[-1:]");
  }
  {
    std::stringstream ss;
    ss << Range(-1, 1);
    REQUIRE(ss.str() == "[-1:1]");
  }
}


// TEST_CASE("MultiRange Overlapping addRange", "[MultiRange]") {
//   auto mr = MultiRange();
//   mr.addRange(Range(-1, UpperBound()));
//   mr.addRange(Range(-1, 1));
//   REQUIRE(mr.getRanges().size() == 1);
// }
