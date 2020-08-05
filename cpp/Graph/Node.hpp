#ifndef Graph_Node_h
#define Graph_Node_h

#include <utility>


class _Node_Base {};


template <typename NodeType>
class Node : public _Node_Base {
public:
  using value_type = NodeType;

  Node(): mValue() {}
  Node(const value_type& inValue): mValue(inValue) {}

  const value_type& get() const { return mValue; }

  inline friend bool operator==(const Node<value_type>& lhs, const Node<value_type>& rhs) { return lhs.get() == rhs.get(); }

private:
  value_type mValue;

};


namespace std {
  template<typename T>
  struct hash<Node<T>>: public std::unary_function<Node<T>, size_t> {
    size_t operator()(const Node<T>& node) const { return std::hash<T>()(node.get()); }
  };
}


#endif
