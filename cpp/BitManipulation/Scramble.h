#ifndef Scramble_h
#define Scramble_h


template <int MSB, int LSB, typename... ScrambleTypes>
class Scramble {
public:

  template <typename CurrentScramble, typename... RemainingScrambles>
  static constexpr typename std::enable_if<(sizeof...(RemainingScrambles) == 0), int>::type
  _query(const int& index) {
    return CurrentScramble::query(index);
  }

  template <typename CurrentScramble, typename... RemainingScrambles>
  static constexpr typename std::enable_if<(sizeof...(RemainingScrambles) != 0), int>::type
  _query(const int& index) {
    return _query<RemainingScrambles...>(CurrentScramble::query(index));
  }

  static constexpr int query(const int& index) {
    return _query<ScrambleTypes...>(index);
  }

  static constexpr int calculate_mask(const int& index) {
    return int(1) << (LSB - index - 2*MSB);
  }

  static constexpr int calculate_shift(const int& index) {
    return query(index) - index;
  }

  template <int I>
  static constexpr typename std::enable_if<(I < 0), int>::type
  shift(const int& data) {
    return data << (0 - I);
  }

  template <int I>
  static constexpr typename std::enable_if<(I > 0), int>::type
  shift(const int& data) {
    return data >> I;
  }

  template <int I>
  static constexpr typename std::enable_if<(I == 0), int>::type
  shift(const int& data) {
    return data;
  }

  template <int I>
  static constexpr typename std::enable_if<(I == LSB), int>::type
  apply(const int& data) {
    return shift<calculate_shift(I)>(data & calculate_mask(I));
  }

  template <int I=MSB>
  static constexpr typename std::enable_if<(I < LSB), int>::type
  apply(const int& data) {
    return (shift<calculate_shift(I)>(data & calculate_mask(I))) | apply<I+1>(data);
  }

};

#endif
