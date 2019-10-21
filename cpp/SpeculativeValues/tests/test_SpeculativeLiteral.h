#include <sstream>

#include "catch.hpp"

#include "SpeculativeLiteral.h"


TEST_CASE("SpeculativeLiteral Constructor", "[SpeculativeLiteral]") {
  auto specLiteral = SpeculativeLiteral<int>(5);
  REQUIRE(specLiteral.resolve() == 5);
}

TEST_CASE("SpeculativeLiteral to_string", "[SpeculativeLiteral][string]") {
  auto specLiteral = SpeculativeLiteral<int>(5);
  REQUIRE(specLiteral.to_string() == "5");
}

TEST_CASE("SpeculativeLiteral operator<<", "[SpeculativeLiteral][ostream]") {
  auto specLiteral = SpeculativeLiteral<int>(5);
  std::stringstream ss;
  ss << specLiteral;
  REQUIRE(ss.str() == "5");
}
