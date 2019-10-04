#include <iomanip>
#include <iostream>
using std::cout;
using std::endl;

#include "NumberRanges.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"


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

  // 0111
  // 0110
  REQUIRE(Range::overlaps(Range(Range::cNoBound, 1), Range(-1, 1)));
  REQUIRE(Range::overlaps(Range(Range::cNoBound, 1), Range(-1, Range::cNoBound)));

  // 1101
  // 1001
  REQUIRE(Range::overlaps(Range(-1, 1), Range(Range::cNoBound, 1)));
  REQUIRE(Range::overlaps(Range(-1, Range::cNoBound), Range(Range::cNoBound, 1)));

  // 1011
  // 1110
  REQUIRE(Range::overlaps(Range(-1, Range::cNoBound), Range(-1, 1)));
  REQUIRE(Range::overlaps(Range(-1, 1), Range(-1, Range::cNoBound)));

}


TEST_CASE("Nonbounded Range No Overlap", "[Range][overlap]") {
  //
  // Non-overlap Cases
  //

  // 0111
  // 0110
  REQUIRE(!Range::overlaps(Range(Range::cNoBound, -2), Range(-1, 1)));
  REQUIRE(!Range::overlaps(Range(Range::cNoBound, -2), Range(-1, Range::cNoBound)));

  // 1101
  // 1001
  REQUIRE(!Range::overlaps(Range(2, 4), Range(Range::cNoBound, 1)));
  REQUIRE(!Range::overlaps(Range(2, Range::cNoBound), Range(Range::cNoBound, 1)));

  // 1011
  // 1110
  REQUIRE(!Range::overlaps(Range(2, Range::cNoBound), Range(-1, 1)));
  REQUIRE(!Range::overlaps(Range(-4, -2), Range(-1, Range::cNoBound)));

}


TEST_CASE("MultiRange Overlapping addRange", "[MultiRange]") {
  auto mr = MultiRange();
  mr.addRange(Range(-1, Range::cNoBound));
  mr.addRange(Range(-1, 1));
  REQUIRE(mr.getRanges().size() == 1);
}