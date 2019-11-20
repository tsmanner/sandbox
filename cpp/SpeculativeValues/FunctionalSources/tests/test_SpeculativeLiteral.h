#include <sstream>

#include "catch.hpp"

namespace test_SpeculativeLiteral {
  #include "mocks.h"
}

#include "SpeculativeLiteral.h"

namespace test_SpeculativeLiteral {

TEST_CASE("SpeculativeLiteral Constructor", "[SpeculativeLiteral][constructor]") {
  auto specLiteral = SpeculativeLiteral<SpecVal>(5);
  REQUIRE(specLiteral.resolve() == 5);
}

TEST_CASE("SpeculativeLiteral makeConcrete", "[SpeculativeLiteral][makeConcrete]") {
  auto specLiteral = SpeculativeLiteral<SpecVal>(5, false);
  specLiteral.makeConcrete();
  REQUIRE(specLiteral.resolve() == 5);
}

TEST_CASE("SpeculativeLiteral to_string", "[SpeculativeLiteral][string]") {
  auto specLiteral = SpeculativeLiteral<SpecVal>(5);
  REQUIRE(specLiteral.to_string() == "5");
}

TEST_CASE("SpeculativeLiteral operator<<", "[SpeculativeLiteral][ostream]") {
  auto specLiteral = SpeculativeLiteral<SpecVal>(5);
  std::stringstream ss;
  ss << specLiteral;
  REQUIRE(ss.str() == "5");
}

}
