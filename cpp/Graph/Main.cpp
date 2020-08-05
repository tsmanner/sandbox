#include <string>
#include <type_traits>

#include "Graph.hpp"
#include "Edge.hpp"
#include "Node.hpp"


int main() {
  // using graph_type = Graph<
  //   Nodes<
  //     int,
  //     float,
  //     std::string
  //   >,
  //   Edges<
  //     Edge<int, int>
  //   >
  // >;
  using graph_type = Graph<
    int,
    float,
    std::string
  >;

  auto g = graph_type();
  const auto& node_1 = g.insert(1);
  const auto& node_2 = g.insert(2);
  const auto& node_hello_world = g.insert(std::string("hello world"));
  g.insert(node_1, node_2);
  g.insert(node_1, node_hello_world);
  return 0;
}
