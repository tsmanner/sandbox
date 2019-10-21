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
TEST_CASE("Addition object_ptr object_ptr", "[SpeculativeOperation]") {
  auto specLiteralLhs = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(1));
  auto specLiteralRhs = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(2));
  auto specOp = specLiteralLhs + specLiteralRhs;
  REQUIRE(specOp->resolve() == 3);
}

TEST_CASE("Addition int object_ptr", "[SpeculativeOperation]") {
  auto specLiteral = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(1));
  auto specOp = int(2) + specLiteral;
  REQUIRE(specOp->resolve() == 3);
}

TEST_CASE("Addition object_ptr int", "[SpeculativeOperation]") {
  auto specLiteral = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(1));
  auto specOp = specLiteral + int(2);
  REQUIRE(specOp->resolve() == 3);
}

TEST_CASE("Addition int(uint) object_ptr", "[SpeculativeOperation]") {
  auto specLiteral = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(1));
  auto specOp = uint(2) + specLiteral;
  REQUIRE(specOp->resolve() == 3);
}

TEST_CASE("Addition object_ptr int(uint)", "[SpeculativeOperation]") {
  auto specLiteral = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(1));
  auto specOp = specLiteral + uint(2);
  REQUIRE(specOp->resolve() == 3);
}

// Subtraction
TEST_CASE("Subtraction object_ptr object_ptr", "[SpeculativeOperation]") {
  auto specLiteralLhs = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(1));
  auto specLiteralRhs = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(2));
  auto specOp = specLiteralLhs - specLiteralRhs;
  REQUIRE(specOp->resolve() == -1);
}

TEST_CASE("Subtraction int object_ptr", "[SpeculativeOperation]") {
  auto specLiteral = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(1));
  auto specOp = int(2) - specLiteral;
  REQUIRE(specOp->resolve() == 1);
}

TEST_CASE("Subtraction object_ptr int", "[SpeculativeOperation]") {
  auto specLiteral = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(1));
  auto specOp = specLiteral - int(2);
  REQUIRE(specOp->resolve() == -1);
}

TEST_CASE("Subtraction int(uint) object_ptr", "[SpeculativeOperation]") {
  auto specLiteral = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(1));
  auto specOp = uint(2) - specLiteral;
  REQUIRE(specOp->resolve() == 1);
}

TEST_CASE("Subtraction object_ptr int(uint)", "[SpeculativeOperation]") {
  auto specLiteral = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(1));
  auto specOp = specLiteral - uint(2);
  REQUIRE(specOp->resolve() == -1);
}

// Multiplication
TEST_CASE("Multiplication object_ptr object_ptr", "[SpeculativeOperation]") {
  auto specLiteralLhs = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(4));
  auto specLiteralRhs = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(2));
  auto specOp = specLiteralLhs * specLiteralRhs;
  REQUIRE(specOp->resolve() == 8);
}

TEST_CASE("Multiplication int object_ptr", "[SpeculativeOperation]") {
  auto specLiteral = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(4));
  auto specOp = int(2) * specLiteral;
  REQUIRE(specOp->resolve() == 8);
}

TEST_CASE("Multiplication object_ptr int", "[SpeculativeOperation]") {
  auto specLiteral = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(4));
  auto specOp = specLiteral * int(2);
  REQUIRE(specOp->resolve() == 8);
}

TEST_CASE("Multiplication int(uint) object_ptr", "[SpeculativeOperation]") {
  auto specLiteral = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(4));
  auto specOp = uint(2) * specLiteral;
  REQUIRE(specOp->resolve() == 8);
}

TEST_CASE("Multiplication object_ptr int(uint)", "[SpeculativeOperation]") {
  auto specLiteral = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(4));
  auto specOp = specLiteral * uint(2);
  REQUIRE(specOp->resolve() == 8);
}

// Division
TEST_CASE("Division object_ptr object_ptr", "[SpeculativeOperation]") {
  auto specLiteralLhs = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(4));
  auto specLiteralRhs = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(2));
  auto specOp = specLiteralLhs / specLiteralRhs;
  REQUIRE(specOp->resolve() == 2);
}

TEST_CASE("Division int object_ptr", "[SpeculativeOperation]") {
  auto specLiteral = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(4));
  auto specOp = int(8) / specLiteral;
  REQUIRE(specOp->resolve() == 2);
}

TEST_CASE("Division object_ptr int", "[SpeculativeOperation]") {
  auto specLiteral = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(4));
  auto specOp = specLiteral / int(2);
  REQUIRE(specOp->resolve() == 2);
}

TEST_CASE("Division int(uint) object_ptr", "[SpeculativeOperation]") {
  auto specLiteral = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(4));
  auto specOp = uint(8) / specLiteral;
  REQUIRE(specOp->resolve() == 2);
}

TEST_CASE("Division object_ptr int(uint)", "[SpeculativeOperation]") {
  auto specLiteral = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(4));
  auto specOp = specLiteral / uint(2);
  REQUIRE(specOp->resolve() == 2);
}

// Maximum
TEST_CASE("Maximum object_ptr object_ptr", "[SpeculativeOperation]") {
  auto specLiteralLhs = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(4));
  auto specLiteralRhs = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(2));
  auto specOp = max(specLiteralLhs, specLiteralRhs);
  REQUIRE(specOp->resolve() == 4);
}

TEST_CASE("Maximum int object_ptr", "[SpeculativeOperation]") {
  auto specLiteral = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(4));
  auto specOp = max(int(2), specLiteral);
  REQUIRE(specOp->resolve() == 4);
}

TEST_CASE("Maximum object_ptr int", "[SpeculativeOperation]") {
  auto specLiteral = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(4));
  auto specOp = max(specLiteral, int(2));
  REQUIRE(specOp->resolve() == 4);
}

TEST_CASE("Maximum int(uint) object_ptr", "[SpeculativeOperation]") {
  auto specLiteral = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(4));
  auto specOp = max(uint(2), specLiteral);
  REQUIRE(specOp->resolve() == 4);
}

TEST_CASE("Maximum object_ptr int(uint)", "[SpeculativeOperation]") {
  auto specLiteral = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(4));
  auto specOp = max(specLiteral, uint(2));
  REQUIRE(specOp->resolve() == 4);
}

// Minimum
TEST_CASE("Minimum object_ptr object_ptr", "[SpeculativeOperation]") {
  auto specLiteralLhs = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(4));
  auto specLiteralRhs = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(2));
  auto specOp = min(specLiteralLhs, specLiteralRhs);
  REQUIRE(specOp->resolve() == 2);
}

TEST_CASE("Minimum int object_ptr", "[SpeculativeOperation]") {
  auto specLiteral = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(4));
  auto specOp = min(int(2), specLiteral);
  REQUIRE(specOp->resolve() == 2);
}

TEST_CASE("Minimum object_ptr int", "[SpeculativeOperation]") {
  auto specLiteral = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(4));
  auto specOp = min(specLiteral, int(2));
  REQUIRE(specOp->resolve() == 2);
}

TEST_CASE("Minimum int(uint) object_ptr", "[SpeculativeOperation]") {
  auto specLiteral = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(4));
  auto specOp = min(uint(2), specLiteral);
  REQUIRE(specOp->resolve() == 2);
}

TEST_CASE("Minimum object_ptr int(uint)", "[SpeculativeOperation]") {
  auto specLiteral = object_ptr<SpeculativeNode<int>>(new SpeculativeLiteral<int>(4));
  auto specOp = min(specLiteral, uint(2));
  REQUIRE(specOp->resolve() == 2);
}

