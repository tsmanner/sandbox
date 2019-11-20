#include "catch.h"

#include "Node.h"


TEST_CASE("Node constructor", "") {
  CHECK(Node().isReal() == false);
}
