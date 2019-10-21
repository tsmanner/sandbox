#include <sstream>

#include "catch.hpp"

namespace test_SpeculativeSource {
  #include "mocks.h"
}
using namespace test_SpeculativeSource;

#include "SpeculativeSource.h"

namespace test_SpeculativeSource {

TEST_CASE("SpeculativeSource Constructor Function", "[SpeculativeSource][constructor]") {
  auto five = SpecVal(5, false);
  auto specSource = SpeculativeSource<SpecVal>([&]() -> SpecVal& { return five; });
  five.makeConcrete();
  REQUIRE(specSource.resolve() == 5);
}

TEST_CASE("SpeculativeSource Constructor Function with args", "[SpeculativeSource][constructor]") {
  auto five = SpecVal(5, false);
  auto specSource = SpeculativeSource<SpecVal>([](SpecVal* x) -> SpecVal& { return *x; }, &five);
  five.makeConcrete();
  REQUIRE(specSource.resolve() == 5);
}

TEST_CASE("SpeculativeSource makeConcrete", "[SpeculativeSource][makeConcrete]") {
  auto five = SpecVal(5, false);
  auto specSource = SpeculativeSource<SpecVal>([&]() -> SpecVal& { return five; });
  specSource.makeConcrete();
  REQUIRE(specSource.resolve() == 5);
}

TEST_CASE("SpeculativeSource to_string", "[SpeculativeSource][string]") {
  auto five = SpecVal(5, false);
  auto specSource = SpeculativeSource<SpecVal>([&]() -> SpecVal& { return five; });
  five.makeConcrete();
  REQUIRE(specSource.to_string() == "5");
}

TEST_CASE("SpeculativeSource operator<<", "[SpeculativeSource][ostream]") {
  auto five = SpecVal(5, false);
  auto specSource = SpeculativeSource<SpecVal>([&]() -> SpecVal& { return five; });
  five.makeConcrete();
  std::stringstream ss;
  ss << specSource;
  REQUIRE(ss.str() == "5");
}

}
