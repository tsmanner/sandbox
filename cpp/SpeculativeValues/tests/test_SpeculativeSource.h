#include <sstream>

#include "catch.hpp"

#include "SpeculativeSource.h"


TEST_CASE("SpeculativeSource Constructor Function", "[SpeculativeSource]") {
  auto specSource = SpeculativeSource<int>([]() -> int { return 5; });
  REQUIRE(specSource.resolve() == 5);
}

TEST_CASE("SpeculativeSource Constructor Function with args", "[SpeculativeSource]") {
  auto specSource = SpeculativeSource<int>([](int x) -> int { return x; }, 5);
  REQUIRE(specSource.resolve() == 5);
}

TEST_CASE("SpeculativeSource to_string", "[SpeculativeSource][string]") {
  auto specSource = SpeculativeSource<int>([]() -> int { return 5; });
  REQUIRE(specSource.to_string() == "5");
}

TEST_CASE("SpeculativeSource operator<<", "[SpeculativeSource][ostream]") {
  auto specSource = SpeculativeSource<int>([]() -> int { return 5; });
  std::stringstream ss;
  ss << specSource;
  REQUIRE(ss.str() == "5");
}
