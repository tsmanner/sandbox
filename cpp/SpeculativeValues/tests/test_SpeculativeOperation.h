#include <algorithm>
#include <sstream>

#include "catch.hpp"

using std::max;
using std::min;
#include "SpeculativeOperation.h"


// Test all 9 constructor permutations:
// int        int
// int        Node*
// int        object_ptr
// Node*      int
// Node*      Node*
// Node*      object_ptr
// object_ptr int
// object_ptr Node*
// object_ptr object_ptr

TEST_CASE("SpeculativeOperation Constructor int int", "[SpeculativeOperation]") {
  auto specOp = SpeculativeOperation<int>(
    "+",
    [](int lhs, int rhs) -> int { return lhs + rhs; },
    1,
    2
  );
  REQUIRE(specOp.resolve() == 3);
}


TEST_CASE("SpeculativeOperation Constructor int Node*", "[SpeculativeOperation]") {
  auto specOp = SpeculativeOperation<int>(
    "+",
    [](int lhs, int rhs) -> int { return lhs + rhs; },
    1,
    new SpeculativeLiteral<int>(2)
  );
  REQUIRE(specOp.resolve() == 3);
}


TEST_CASE("SpeculativeOperation Constructor int object_ptr", "[SpeculativeOperation]") {
  auto specOp = SpeculativeOperation<int>(
    "+",
    [](int lhs, int rhs) -> int { return lhs + rhs; },
    1,
    object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(2))
  );
  REQUIRE(specOp.resolve() == 3);
}


TEST_CASE("SpeculativeOperation Constructor Node* int", "[SpeculativeOperation]") {
  auto specOp = SpeculativeOperation<int>(
    "+",
    [](int lhs, int rhs) -> int { return lhs + rhs; },
    new SpeculativeLiteral<int>(1),
    2
  );
  REQUIRE(specOp.resolve() == 3);
}


TEST_CASE("SpeculativeOperation Constructor Node* Node*", "[SpeculativeOperation]") {
  auto specOp = SpeculativeOperation<int>(
    "+",
    [](int lhs, int rhs) -> int { return lhs + rhs; },
    new SpeculativeLiteral<int>(1),
    new SpeculativeLiteral<int>(2)
  );
  REQUIRE(specOp.resolve() == 3);
}


TEST_CASE("SpeculativeOperation Constructor Node* object_ptr", "[SpeculativeOperation]") {
  auto specOp = SpeculativeOperation<int>(
    "+",
    [](int lhs, int rhs) -> int { return lhs + rhs; },
    new SpeculativeLiteral<int>(1),
    object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(2))
  );
  REQUIRE(specOp.resolve() == 3);
}


TEST_CASE("SpeculativeOperation Constructor object_ptr int", "[SpeculativeOperation]") {
  auto specOp = SpeculativeOperation<int>(
    "+",
    [](int lhs, int rhs) -> int { return lhs + rhs; },
    object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(1)),
    2
  );
  REQUIRE(specOp.resolve() == 3);
}


TEST_CASE("SpeculativeOperation Constructor object_ptr Node*", "[SpeculativeOperation]") {
  auto specOp = SpeculativeOperation<int>(
    "+",
    [](int lhs, int rhs) -> int { return lhs + rhs; },
    object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(1)),
    new SpeculativeLiteral<int>(2)
  );
  REQUIRE(specOp.resolve() == 3);
}


TEST_CASE("SpeculativeOperation Constructor object_ptr object_ptr", "[SpeculativeOperation]") {
  auto specOp = SpeculativeOperation<int>(
    "+",
    [](int lhs, int rhs) -> int { return lhs + rhs; },
    object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(1)),
    object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(2))
  );
  REQUIRE(specOp.resolve() == 3);
}


// Test to_string and stream operator

TEST_CASE("SpeculativeOperation to_string", "[SpeculativeOperation][string]") {
  auto specOp = SpeculativeOperation<int>(
    "+",
    [](int lhs, int rhs) -> int { return lhs + rhs; },
    1,
    2
  );
  REQUIRE(specOp.to_string() == "(1 + 2)");
}


TEST_CASE("SpeculativeOperation operator<<", "[SpeculativeOperation][ostream]") {
  auto specOp = SpeculativeOperation<int>(
    "+",
    [](int lhs, int rhs) -> int { return lhs + rhs; },
    1,
    2
  );
  std::stringstream ss;
  ss << specOp;
  REQUIRE(ss.str() == "(1 + 2)");
}


// Test all 5 flavors of 6 mathematical operators
// addition +
//     object_ptr object_ptr
//     int        object_ptr
//     object_ptr int
//     int(uint)  object_ptr
//     object_ptr int(uint)
// subtraction -
//     object_ptr object_ptr
//     int        object_ptr
//     object_ptr int
//     int(uint)  object_ptr
//     object_ptr int(uint)
// multiplication *
//     object_ptr object_ptr
//     int        object_ptr
//     object_ptr int
//     int(uint)  object_ptr
//     object_ptr int(uint)
// division /
//     object_ptr object_ptr
//     int        object_ptr
//     object_ptr int
//     int(uint)  object_ptr
//     object_ptr int(uint)
// max
//     object_ptr object_ptr
//     int        object_ptr
//     object_ptr int
//     int(uint)  object_ptr
//     object_ptr int(uint)
//     object_ptr object_ptr
// min
//     int        object_ptr
//     object_ptr int
//     int(uint)  object_ptr
//     object_ptr int(uint)

// Addition
TEST_CASE("Addition int(uint) object_ptr", "[SpeculativeOperation]") {
  auto specLiteral = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(1));
  auto specOp = specLiteral + uint(2);
  REQUIRE(specOp->resolve() == 3);
}

TEST_CASE("Addition object_ptr int(uint)", "[SpeculativeOperation]") {
  auto specLiteral = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(1));
  auto specOp = uint(2) + specLiteral;
  REQUIRE(specOp->resolve() == 3);
}

