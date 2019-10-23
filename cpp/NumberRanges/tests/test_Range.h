#include <cstdlib>
#include <limits>
#include <sstream>

#include "Range.h"


TEST_CASE("Range Default Constructor [:]", "[Range]") {
  auto r = Range();
  CHECK(r.getLowerBound().getType() == Bound::cOpen);
  CHECK(r.getUpperBound().getType() == Bound::cOpen);
}


TEST_CASE("Range Value Constructor [:1]", "[Range]") {
  auto r = Range(LowerBound(), 1);
  CHECK(r.getLowerBound().getType() == Bound::cOpen);
  CHECK(r.getUpperBound().getType() == Bound::cClosed);
  CHECK(r.getUpperBound().getValue() == 1);
}


TEST_CASE("Range Value Constructor [-1:]", "[Range]") {
  auto r = Range(-1, UpperBound());
  CHECK(r.getLowerBound().getType() == Bound::cClosed);
  CHECK(r.getLowerBound().getValue() == -1);
  CHECK(r.getUpperBound().getType() == Bound::cOpen);
}


TEST_CASE("Range Value Constructor [-1:1]", "[Range]") {
  auto r = Range(-1, 1);
  CHECK(r.getLowerBound().getType() == Bound::cClosed);
  CHECK(r.getLowerBound().getValue() == -1);
  CHECK(r.getUpperBound().getType() == Bound::cClosed);
  CHECK(r.getUpperBound().getValue() == 1);
}


TEST_CASE("Range [:] Covers Value", "[Range][covers]") {
  auto r = Range();
  CHECK(r.covers(-872346));
  CHECK(r.covers(0));
  CHECK(r.covers(872346));
}


TEST_CASE("Range [:1] Covers Value", "[Range][covers]") {
  auto r = Range(LowerBound(), 1);
  CHECK( r.covers(-1));
  CHECK( r.covers(1));
  CHECK(!r.covers(872346));
}


TEST_CASE("Range [-1:] Covers Value", "[Range][covers]") {
  auto r = Range(-1, UpperBound());
  CHECK(!r.covers(-872346));
  CHECK( r.covers(-1));
  CHECK( r.covers(1));
}


TEST_CASE("Range [-1:1] Covers Value", "[Range][covers]") {
  auto r = Range(-1, 1);
  CHECK(!r.covers(-872346));
  CHECK( r.covers(-1));
  CHECK( r.covers(0));
  CHECK( r.covers(1));
  CHECK(!r.covers(872346));
}


TEST_CASE("Bounded Range Overlap", "[Range][overlap]") {
  //
  // Overlap Cases:
  //

  // ---
  //   ---
  CHECK(Range::overlaps(Range(-1, 1), Range(1, 3)));
  //   ---
  // ---
  CHECK(Range::overlaps(Range(-1, 1), Range(-3, -1)));
  //  ---
  // -----
  CHECK(Range::overlaps(Range(-1, 1), Range(-2, 2)));
  // -----
  //  ---
  CHECK(Range::overlaps(Range(-2, 2), Range(-1, 1)));

  //
  // Non-overlap Cases
  //

  // ---
  //    ---
  CHECK(!Range::overlaps(Range(-1, 1), Range(2, 4)));
  //    ---
  // ---
  CHECK(!Range::overlaps(Range(-4, -2), Range(-1, 1)));
  // ---
  //     ---
  CHECK(!Range::overlaps(Range(-1, 1), Range(3, 5)));
  //     ---
  // ---
  CHECK(!Range::overlaps(Range(-5, -3), Range(-1, 1)));

}


TEST_CASE("Nonbounded Range Overlap", "[Range][overlap]") {
  //
  // Overlap Cases:
  //
  CHECK(Range::overlaps(Range(LowerBound(),            1), Range(          -1,           1)));
  CHECK(Range::overlaps(Range(LowerBound(),            1), Range(          -1, UpperBound())));
  CHECK(Range::overlaps(Range(          -1,            1), Range(LowerBound(),           1)));
  CHECK(Range::overlaps(Range(          -1, UpperBound()), Range(LowerBound(),           1)));
  CHECK(Range::overlaps(Range(          -1, UpperBound()), Range(          -1,           1)));
  CHECK(Range::overlaps(Range(          -1,            1), Range(          -1, UpperBound())));
  // If either range has no bounds, it must overlap the other.
  CHECK(Range::overlaps(Range(), Range()));
  CHECK(Range::overlaps(Range(-1, 1), Range()));
  CHECK(Range::overlaps(Range(), Range(-1, 1)));

}


TEST_CASE("Nonbounded Range No Overlap", "[Range][overlap]") {
  //
  // Non-overlap Cases
  //
  CHECK(!Range::overlaps(Range(LowerBound(), -2           ), Range(          -1,            1)));
  CHECK(!Range::overlaps(Range(LowerBound(), -2           ), Range(          -1, UpperBound())));
  CHECK(!Range::overlaps(Range(           2,  4           ), Range(LowerBound(),            1)));
  CHECK(!Range::overlaps(Range(           2,  UpperBound()), Range(LowerBound(),            1)));
  CHECK(!Range::overlaps(Range(           2,  UpperBound()), Range(          -1,            1)));
  CHECK(!Range::overlaps(Range(          -4, -2           ), Range(          -1, UpperBound())));

}


TEST_CASE("Bounded Range Adjacency", "[Range][adjacent]") {
  //
  // Adjacent Cases:
  //
  CHECK(Range::adjacent(Range(-4, -2), Range(-1,  1)));
  CHECK(Range::adjacent(Range(-4, -2), Range(-1,  1)));
  CHECK(Range::adjacent(Range( 2,  4), Range(-1,  1)));
  CHECK(Range::adjacent(Range( 2,  4), Range(-1,  1)));
  CHECK(Range::adjacent(Range( 2,  4), Range(-1,  1)));
  CHECK(Range::adjacent(Range(-4, -2), Range(-1,  1)));

  CHECK(Range::adjacent(Range(-1,  1), Range( 2,  2)));
  CHECK(Range::adjacent(Range( 2,  2), Range( 3,  5)));

}


TEST_CASE("Bounded Range No Adjacency", "[Range][adjacent]") {
  //
  // Non-adjacent Overlap Cases
  //
  CHECK(!Range::adjacent(Range(-3,  1), Range(-1,  1)));
  CHECK(!Range::adjacent(Range(-1,  1), Range(-3,  1)));
  CHECK(!Range::adjacent(Range(-3,  3), Range(-1,  1)));
  CHECK(!Range::adjacent(Range(-1,  1), Range(-3,  3)));
  CHECK(!Range::adjacent(Range( 1,  3), Range(-1,  1)));
  CHECK(!Range::adjacent(Range(-1,  1), Range( 1,  3)));
  //
  // Non-adjacent Non-overlap Cases
  //
  CHECK(!Range::adjacent(Range(-5, -3), Range(-1,  1)));
  CHECK(!Range::adjacent(Range(-1,  1), Range(-5, -3)));
  CHECK(!Range::adjacent(Range( 3,  5), Range(-1,  1)));
  CHECK(!Range::adjacent(Range(-1,  1), Range( 3,  5)));
}


TEST_CASE("Nonbounded Range Adjacency", "[Range][adjacent]") {
  //
  // Adjacent Cases:
  //
  CHECK(Range::adjacent(Range(LowerBound(),           -2), Range(          -1,           1)));
  CHECK(Range::adjacent(Range(LowerBound(),           -2), Range(          -1, UpperBound())));
  CHECK(Range::adjacent(Range(           2,            4), Range(LowerBound(),           1)));
  CHECK(Range::adjacent(Range(           2, UpperBound()), Range(LowerBound(),           1)));
  CHECK(Range::adjacent(Range(           2, UpperBound()), Range(          -1,           1)));
  CHECK(Range::adjacent(Range(          -4,           -2), Range(          -1, UpperBound())));

}


TEST_CASE("Nonbounded Range No Adjacency", "[Range][adjacent]") {
  //
  // Non-adjacent Overlap Cases
  //
  CHECK(!Range::adjacent(Range(LowerBound(),  1           ), Range(          -1,            1)));
  CHECK(!Range::adjacent(Range(LowerBound(),  1           ), Range(          -1, UpperBound())));
  CHECK(!Range::adjacent(Range(          -1,  1           ), Range(LowerBound(),            1)));
  CHECK(!Range::adjacent(Range(          -1,  UpperBound()), Range(LowerBound(),            1)));
  CHECK(!Range::adjacent(Range(          -1,  UpperBound()), Range(          -1,            1)));
  CHECK(!Range::adjacent(Range(          -1,  1           ), Range(          -1, UpperBound())));
  //
  // Non-adjacent Non-overlap Cases
  //
  CHECK(!Range::adjacent(Range(LowerBound(), -3           ), Range(          -1,            1)));
  CHECK(!Range::adjacent(Range(LowerBound(), -3           ), Range(          -1, UpperBound())));
  CHECK(!Range::adjacent(Range(           3,  5           ), Range(LowerBound(),            1)));
  CHECK(!Range::adjacent(Range(           3,  UpperBound()), Range(LowerBound(),            1)));
  CHECK(!Range::adjacent(Range(           3,  UpperBound()), Range(          -1,            1)));
  CHECK(!Range::adjacent(Range(          -5, -3           ), Range(          -1, UpperBound())));
  // If either range has no bounds, it must not be adjacent to the other.
  CHECK(!Range::adjacent(Range(), Range()));
  CHECK(!Range::adjacent(Range(-1, 1), Range()));
  CHECK(!Range::adjacent(Range(), Range(-1, 1)));

}


TEST_CASE("Range conjunction - overlapping ranges", "[Range][conjunction]") {
  auto r = Range::conjunction(Range(-2, 0), Range(0, 2));
  CHECK(r.getLowerBound() == 0);
  CHECK(r.getUpperBound() == 0);

  r = Range::conjunction(Range(LowerBound(), 0), Range(0, 2));
  CHECK(r.getLowerBound() == 0);
  CHECK(r.getUpperBound() == 0);

  r = Range::conjunction(Range(-2, UpperBound()), Range(0, 2));
  CHECK(r.getLowerBound() == 0);
  CHECK(r.getUpperBound() == 2);

  r = Range::conjunction(Range(), Range(0, 2));
  CHECK(r.getLowerBound() == 0);
  CHECK(r.getUpperBound() == 2);
}


TEST_CASE("Range conjunction - non-overlapping ranges", "[Range][conjunction]") {
  try {
    Range::conjunction(Range(-2, -1), Range(1, 2));
  } catch (Range::NonOverlappingRangeConjunction& e) {
    CHECK(true);
  }
}


TEST_CASE("Range disjunction - overlapping ranges", "[Range][disjunction]") {
  auto r = Range::disjunction(Range(-2, 0), Range(0, 2));
  CHECK(r.getLowerBound() == -2);
  CHECK(r.getUpperBound() ==  2);

  r = Range::disjunction(Range(LowerBound(), 0), Range(0, 2));
  CHECK(r.getLowerBound().getType() == Bound::cOpen);
  CHECK(r.getUpperBound() == 2);

  r = Range::disjunction(Range(-2, UpperBound()), Range(0, 2));
  CHECK(r.getLowerBound() == -2);
  CHECK(r.getUpperBound().getType() == Bound::cOpen);

  r = Range::disjunction(Range(), Range(0, 2));
  CHECK(r.getLowerBound().getType() == Bound::cOpen);
  CHECK(r.getUpperBound().getType() == Bound::cOpen);
}


TEST_CASE("Range disjunction - non-overlapping ranges", "[Range][disjunction]") {
  try {
    Range::disjunction(Range(-2, -1), Range(1, 2));
  } catch (Range::NonOverlappingRangeDisjunction& e) {
    CHECK(true);
  }
}


TEST_CASE("Range Compare", "[Range][Compare]") {
  // Lower Bound is Less
  CHECK( Range::Compare()(Range(-1,  1), Range( 0,  2)));
  CHECK( Range::Compare()(Range(-1,  1), Range( 0,  1)));
  CHECK( Range::Compare()(Range(-1,  1), Range( 0,  0)));
  // Lower Bound is Equal
  CHECK( Range::Compare()(Range(-1,  1), Range(-1,  2)));
  // Ranges are Equal
  CHECK(!Range::Compare()(Range(-1,  1), Range(-1,  1)));
  // Lower Bound is Equal
  CHECK(!Range::Compare()(Range(-1,  1), Range(-1,  0)));
  // Lower Bound is Greater
  CHECK(!Range::Compare()(Range(-1,  1), Range(-2,  2)));
  CHECK(!Range::Compare()(Range(-1,  1), Range(-2,  1)));
  CHECK(!Range::Compare()(Range(-1,  1), Range(-2,  0)));
}


TEST_CASE("Range size", "[Range][size]") {
  CHECK(Range().size() == size_t(std::numeric_limits<uint>::max()) + 1);
  CHECK(Range(LowerBound(), 1).size()  == size_t(0) - std::numeric_limits<int>::min() + 2);
  CHECK(Range(-1, UpperBound()).size() == size_t(std::numeric_limits<int>::max()) + 2);
  CHECK(Range(-1, 1).size() == 3);
}


TEST_CASE("Range stream operator", "[Range]") {
  {
    std::stringstream ss;
    ss << Range();
    CHECK(ss.str() == "[:]");
  }
  {
    std::stringstream ss;
    ss << Range(1, 1);
    CHECK(ss.str() == "[1]");
  }
  {
    std::stringstream ss;
    ss << Range(LowerBound(), 1);
    CHECK(ss.str() == "[:1]");
  }
  {
    std::stringstream ss;
    ss << Range(-1, UpperBound());
    CHECK(ss.str() == "[-1:]");
  }
  {
    std::stringstream ss;
    ss << Range(-1, 1);
    CHECK(ss.str() == "[-1:1]");
  }
}
