#include <algorithm>
#include <sstream>

#include "catch.hpp"

namespace test_SpeculativeOperation {
  #include "mocks.h"
}
using namespace test_SpeculativeOperation;

using std::max;
using std::min;
#include "SpeculativeOperation.h"

namespace test_SpeculativeOperation {

// Test all 9 constructor permutations:
// SpecVal    SpecVal
// SpecVal    Node*
// SpecVal    object_ptr
// Node*      SpecVal
// Node*      Node*
// Node*      object_ptr
// object_ptr SpecVal
// object_ptr Node*
// object_ptr object_ptr

TEST_CASE("SpeculativeOperation Constructor SpecVal SpecVal", "[SpeculativeOperation][constructor]") {
  auto specOp = SpeculativeOperation<SpecVal>(
    "+",
    [](SpecVal lhs, SpecVal rhs) -> SpecVal { return lhs + rhs; },
    1,
    2
  );
  REQUIRE(specOp.resolve() == 3);
}


TEST_CASE("SpeculativeOperation Constructor SpecVal Node*", "[SpeculativeOperation][constructor]") {
  auto specOp = SpeculativeOperation<SpecVal>(
    "+",
    [](SpecVal lhs, SpecVal rhs) -> SpecVal { return lhs + rhs; },
    1,
    new SpeculativeLiteral<SpecVal>(2)
  );
  REQUIRE(specOp.resolve() == 3);
}


TEST_CASE("SpeculativeOperation Constructor SpecVal object_ptr", "[SpeculativeOperation][constructor]") {
  auto specOp = SpeculativeOperation<SpecVal>(
    "+",
    [](SpecVal lhs, SpecVal rhs) -> SpecVal { return lhs + rhs; },
    1,
    object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(2))
  );
  REQUIRE(specOp.resolve() == 3);
}


TEST_CASE("SpeculativeOperation Constructor Node* SpecVal", "[SpeculativeOperation][constructor]") {
  auto specOp = SpeculativeOperation<SpecVal>(
    "+",
    [](SpecVal lhs, SpecVal rhs) -> SpecVal { return lhs + rhs; },
    new SpeculativeLiteral<SpecVal>(1),
    2
  );
  REQUIRE(specOp.resolve() == 3);
}


TEST_CASE("SpeculativeOperation Constructor Node* Node*", "[SpeculativeOperation][constructor]") {
  auto specOp = SpeculativeOperation<SpecVal>(
    "+",
    [](SpecVal lhs, SpecVal rhs) -> SpecVal { return lhs + rhs; },
    new SpeculativeLiteral<SpecVal>(1),
    new SpeculativeLiteral<SpecVal>(2)
  );
  REQUIRE(specOp.resolve() == 3);
}


TEST_CASE("SpeculativeOperation Constructor Node* object_ptr", "[SpeculativeOperation][constructor]") {
  auto specOp = SpeculativeOperation<SpecVal>(
    "+",
    [](SpecVal lhs, SpecVal rhs) -> SpecVal { return lhs + rhs; },
    new SpeculativeLiteral<SpecVal>(1),
    object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(2))
  );
  REQUIRE(specOp.resolve() == 3);
}


TEST_CASE("SpeculativeOperation Constructor object_ptr SpecVal", "[SpeculativeOperation][constructor]") {
  auto specOp = SpeculativeOperation<SpecVal>(
    "+",
    [](SpecVal lhs, SpecVal rhs) -> SpecVal { return lhs + rhs; },
    object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(1)),
    2
  );
  REQUIRE(specOp.resolve() == 3);
}


TEST_CASE("SpeculativeOperation Constructor object_ptr Node*", "[SpeculativeOperation][constructor]") {
  auto specOp = SpeculativeOperation<SpecVal>(
    "+",
    [](SpecVal lhs, SpecVal rhs) -> SpecVal { return lhs + rhs; },
    object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(1)),
    new SpeculativeLiteral<SpecVal>(2)
  );
  REQUIRE(specOp.resolve() == 3);
}


TEST_CASE("SpeculativeOperation Constructor object_ptr object_ptr", "[SpeculativeOperation][constructor]") {
  auto specOp = SpeculativeOperation<SpecVal>(
    "+",
    [](SpecVal lhs, SpecVal rhs) -> SpecVal { return lhs + rhs; },
    object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(1)),
    object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(2))
  );
  REQUIRE(specOp.resolve() == 3);
}


// makeConcrete

TEST_CASE("SpeculativeOperation makeConcrete", "[SpeculativeOperation][makeConcrete]") {
  auto specOp = SpeculativeOperation<SpecVal>(
    "+",
    [](SpecVal lhs, SpecVal rhs) -> SpecVal { return lhs + rhs; },
    object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(1, false)),
    object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(2, false))
  );
  specOp.makeConcrete();
  REQUIRE(specOp.resolve() == 3);
}


// Test to_string and stream operator

TEST_CASE("SpeculativeOperation to_string", "[SpeculativeOperation][string]") {
  auto specOp = SpeculativeOperation<SpecVal>(
    "+",
    [](SpecVal lhs, SpecVal rhs) -> SpecVal { return lhs + rhs; },
    1,
    2
  );
  REQUIRE(specOp.to_string() == "(1 + 2)");
}


TEST_CASE("SpeculativeOperation operator<<", "[SpeculativeOperation][ostream]") {
  auto specOp = SpeculativeOperation<SpecVal>(
    "+",
    [](SpecVal lhs, SpecVal rhs) -> SpecVal { return lhs + rhs; },
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
//     SpecVal    object_ptr
//     object_ptr   SpecVal
//     SpecVal(uint)  object_ptr
//     object_ptr SpecVal(uint)
// subtraction -
//     object_ptr object_ptr
//     SpecVal    object_ptr
//     object_ptr SpecVal
//     SpecVal(uint)  object_ptr
//     object_ptr SpecVal(uint)
// multiplication *
//     object_ptr object_ptr
//     SpecVal    object_ptr
//     object_ptr SpecVal
//     SpecVal(uint)  object_ptr
//     object_ptr SpecVal(uint)
// division /
//     object_ptr object_ptr
//     SpecVal    object_ptr
//     object_ptr SpecVal
//     SpecVal(uint)  object_ptr
//     object_ptr SpecVal(uint)
// max
//     object_ptr object_ptr
//     SpecVal    object_ptr
//     object_ptr SpecVal
//     SpecVal(uint)  object_ptr
//     object_ptr SpecVal(uint)
// min
//     object_ptr object_ptr
//     SpecVal    object_ptr
//     object_ptr SpecVal
//     SpecVal(uint)  object_ptr
//     object_ptr SpecVal(uint)

// Addition
TEST_CASE("Addition object_ptr object_ptr", "[SpeculativeOperation][add][addition][plus]") {
  auto specLiteralLhs = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(1));
  auto specLiteralRhs = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(2));
  auto specOp = specLiteralLhs + specLiteralRhs;
  REQUIRE(specOp->resolve() == 3);
}

TEST_CASE("Addition SpecVal object_ptr", "[SpeculativeOperation][add][addition][plus]") {
  auto specLiteral = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(1));
  auto specOp = SpecVal(2) + specLiteral;
  REQUIRE(specOp->resolve() == 3);
}

TEST_CASE("Addition object_ptr SpecVal", "[SpeculativeOperation][add][addition][plus]") {
  auto specLiteral = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(1));
  auto specOp = specLiteral + SpecVal(2);
  REQUIRE(specOp->resolve() == 3);
}

TEST_CASE("Addition SpecVal(uint) object_ptr", "[SpeculativeOperation][add][addition][plus]") {
  auto specLiteral = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(1));
  auto specOp = uint(2) + specLiteral;
  REQUIRE(specOp->resolve() == 3);
}

TEST_CASE("Addition object_ptr SpecVal(uint)", "[SpeculativeOperation][add][addition][plus]") {
  auto specLiteral = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(1));
  auto specOp = specLiteral + uint(2);
  REQUIRE(specOp->resolve() == 3);
}

// Subtraction
TEST_CASE("Subtraction object_ptr object_ptr", "[SpeculativeOperation][subtract][subtraction][minus]") {
  auto specLiteralLhs = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(1));
  auto specLiteralRhs = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(2));
  auto specOp = specLiteralLhs - specLiteralRhs;
  REQUIRE(specOp->resolve() == -1);
}

TEST_CASE("Subtraction SpecVal object_ptr", "[SpeculativeOperation][subtract][subtraction][minus]") {
  auto specLiteral = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(1));
  auto specOp = SpecVal(2) - specLiteral;
  REQUIRE(specOp->resolve() == 1);
}

TEST_CASE("Subtraction object_ptr SpecVal", "[SpeculativeOperation][subtract][subtraction][minus]") {
  auto specLiteral = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(1));
  auto specOp = specLiteral - SpecVal(2);
  REQUIRE(specOp->resolve() == -1);
}

TEST_CASE("Subtraction SpecVal(uint) object_ptr", "[SpeculativeOperation][subtract][subtraction][minus]") {
  auto specLiteral = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(1));
  auto specOp = uint(2) - specLiteral;
  REQUIRE(specOp->resolve() == 1);
}

TEST_CASE("Subtraction object_ptr SpecVal(uint)", "[SpeculativeOperation][subtract][subtraction][minus]") {
  auto specLiteral = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(1));
  auto specOp = specLiteral - uint(2);
  REQUIRE(specOp->resolve() == -1);
}

// Multiplication
TEST_CASE("Multiplication object_ptr object_ptr", "[SpeculativeOperation][multiply][multiplication]") {
  auto specLiteralLhs = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(4));
  auto specLiteralRhs = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(2));
  auto specOp = specLiteralLhs * specLiteralRhs;
  REQUIRE(specOp->resolve() == 8);
}

TEST_CASE("Multiplication SpecVal object_ptr", "[SpeculativeOperation][multiply][multiplication]") {
  auto specLiteral = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(4));
  auto specOp = SpecVal(2) * specLiteral;
  REQUIRE(specOp->resolve() == 8);
}

TEST_CASE("Multiplication object_ptr SpecVal", "[SpeculativeOperation][multiply][multiplication]") {
  auto specLiteral = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(4));
  auto specOp = specLiteral * SpecVal(2);
  REQUIRE(specOp->resolve() == 8);
}

TEST_CASE("Multiplication SpecVal(uint) object_ptr", "[SpeculativeOperation][multiply][multiplication]") {
  auto specLiteral = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(4));
  auto specOp = uint(2) * specLiteral;
  REQUIRE(specOp->resolve() == 8);
}

TEST_CASE("Multiplication object_ptr SpecVal(uint)", "[SpeculativeOperation][multiply][multiplication]") {
  auto specLiteral = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(4));
  auto specOp = specLiteral * uint(2);
  REQUIRE(specOp->resolve() == 8);
}

// Division
TEST_CASE("Division object_ptr object_ptr", "[SpeculativeOperation][divide][division]") {
  auto specLiteralLhs = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(4));
  auto specLiteralRhs = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(2));
  auto specOp = specLiteralLhs / specLiteralRhs;
  REQUIRE(specOp->resolve() == 2);
}

TEST_CASE("Division SpecVal object_ptr", "[SpeculativeOperation][divide][division]") {
  auto specLiteral = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(4));
  auto specOp = SpecVal(8) / specLiteral;
  REQUIRE(specOp->resolve() == 2);
}

TEST_CASE("Division object_ptr SpecVal", "[SpeculativeOperation][divide][division]") {
  auto specLiteral = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(4));
  auto specOp = specLiteral / SpecVal(2);
  REQUIRE(specOp->resolve() == 2);
}

TEST_CASE("Division SpecVal(uint) object_ptr", "[SpeculativeOperation][divide][division]") {
  auto specLiteral = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(4));
  auto specOp = uint(8) / specLiteral;
  REQUIRE(specOp->resolve() == 2);
}

TEST_CASE("Division object_ptr SpecVal(uint)", "[SpeculativeOperation][divide][division]") {
  auto specLiteral = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(4));
  auto specOp = specLiteral / uint(2);
  REQUIRE(specOp->resolve() == 2);
}

// Maximum
TEST_CASE("Maximum object_ptr object_ptr", "[SpeculativeOperation][max][maximum]") {
  auto specLiteralLhs = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(4));
  auto specLiteralRhs = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(2));
  auto specOp = max(specLiteralLhs, specLiteralRhs);
  REQUIRE(specOp->resolve() == 4);
}

TEST_CASE("Maximum SpecVal object_ptr", "[SpeculativeOperation][max][maximum]") {
  auto specLiteral = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(4));
  auto specOp = max(SpecVal(2), specLiteral);
  REQUIRE(specOp->resolve() == 4);
}

TEST_CASE("Maximum object_ptr SpecVal", "[SpeculativeOperation][max][maximum]") {
  auto specLiteral = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(4));
  auto specOp = max(specLiteral, SpecVal(2));
  REQUIRE(specOp->resolve() == 4);
}

TEST_CASE("Maximum SpecVal(uint) object_ptr", "[SpeculativeOperation][max][maximum]") {
  auto specLiteral = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(4));
  auto specOp = max(uint(2), specLiteral);
  REQUIRE(specOp->resolve() == 4);
}

TEST_CASE("Maximum object_ptr SpecVal(uint)", "[SpeculativeOperation][max][maximum]") {
  auto specLiteral = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(4));
  auto specOp = max(specLiteral, uint(2));
  REQUIRE(specOp->resolve() == 4);
}

// Minimum
TEST_CASE("Minimum object_ptr object_ptr", "[SpeculativeOperation][min][minimum]") {
  auto specLiteralLhs = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(4));
  auto specLiteralRhs = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(2));
  auto specOp = min(specLiteralLhs, specLiteralRhs);
  REQUIRE(specOp->resolve() == 2);
}

TEST_CASE("Minimum SpecVal object_ptr", "[SpeculativeOperation][min][minimum]") {
  auto specLiteral = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(4));
  auto specOp = min(SpecVal(2), specLiteral);
  REQUIRE(specOp->resolve() == 2);
}

TEST_CASE("Minimum object_ptr SpecVal", "[SpeculativeOperation][min][minimum]") {
  auto specLiteral = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(4));
  auto specOp = min(specLiteral, SpecVal(2));
  REQUIRE(specOp->resolve() == 2);
}

TEST_CASE("Minimum SpecVal(uint) object_ptr", "[SpeculativeOperation][min][minimum]") {
  auto specLiteral = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(4));
  auto specOp = min(uint(2), specLiteral);
  REQUIRE(specOp->resolve() == 2);
}

TEST_CASE("Minimum object_ptr SpecVal(uint)", "[SpeculativeOperation][min][minimum]") {
  auto specLiteral = object_ptr<SpeculativeNode<SpecVal>>(new SpeculativeLiteral<SpecVal>(4));
  auto specOp = min(specLiteral, uint(2));
  REQUIRE(specOp->resolve() == 2);
}

}
