#ifndef Graph_Edge_h
#define Graph_Edge_h


template <typename TailType, typename HeadType>
class Edge {
public:
  using tail_type = Node<TailType>;
  using head_type = Node<HeadType>;

  Edge(): mTail(), mHead() {}
  Edge(const tail_type& inTail, const head_type& inHead): mTail(inTail), mHead(inHead) {}

  const tail_type& tail() const { return mTail; }
  const head_type& head() const { return mHead; }

  inline friend bool operator==(const Edge<TailType, HeadType>& lhs, const Edge<TailType, HeadType>& rhs) { return lhs.tail() == rhs.tail() and lhs.head() == rhs.head(); }

private:
  const tail_type& mTail;
  const head_type& mHead;

};


namespace std {
  template <typename TailType, typename HeadType>
  struct hash<Edge<TailType, HeadType>>: public std::unary_function<Edge<TailType, HeadType>, size_t> {
    size_t operator()(const Edge<TailType, HeadType>& edge) const {
      return
        std::hash<typename Edge<TailType, HeadType>::tail_type>()(edge.tail()) ^
        std::hash<typename Edge<TailType, HeadType>::head_type>()(edge.head())
      ;
    }
  };
}


#endif
