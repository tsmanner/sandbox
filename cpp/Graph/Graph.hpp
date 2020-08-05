#ifndef Graph_Graph_h
#define Graph_Graph_h

// Standard Library
#include <type_traits>
#include <unordered_set>
// Graph
#include "Node.hpp"
#include "Edge.hpp"


template <typename ValueType>
class NodeBucket {
public:
  using node_type = Node<ValueType>;
  using value_type = ValueType;

  const Node<ValueType>& insert(const value_type& inValueType) {
    return *mNodes.insert(node_type(inValueType)).first;
  }

private:
  std::unordered_set<node_type> mNodes;

};


template <typename EdgeType>
class EdgeBucket {
public:
  using edge_type = EdgeType;
  using tail_type = typename EdgeType::tail_type;
  using head_type = typename EdgeType::head_type;

  void insert(const tail_type& inTail, const head_type& inHead) {
    mValues.insert(edge_type(inTail, inHead));
  }

private:
  std::unordered_set<edge_type> mValues;

};


template <typename TailValueType, typename... HeadValueTypes>
class EdgeTypes : public EdgeBucket<Edge<TailValueType, HeadValueTypes>>... {};


template <typename... NodeValueTypes>
class Graph : public NodeBucket<NodeValueTypes>..., public EdgeTypes<NodeValueTypes, NodeValueTypes...>... {
public:

  template <typename ValueType>
  const Node<ValueType>& insert(const ValueType& inValue) {
    return this->NodeBucket<ValueType>::insert(inValue);
  }

  template <typename TailNodeType, typename HeadNodeType>
  void insert(const TailNodeType& inTail, const HeadNodeType& inHead) {
    this->EdgeBucket<Edge<typename TailNodeType::value_type, typename HeadNodeType::value_type>>::insert(inTail, inHead);
  }

private:

};


#endif
