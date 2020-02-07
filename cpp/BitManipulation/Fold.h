#ifndef Fold_h
#define Fold_h


template <int MSB, int LSB, int STEP, typename Scramble>
class Fold {
public:

  // Query function, templated only with the index to look up
  // Enabled if QUERY falls between MSB and LSB
  template <int QUERY>
  static constexpr typename std::enable_if<(MSB <= QUERY and QUERY <= LSB), int>::type
  query() {
    // Query the Scramble by normalizing this value to the range [0:STEP)
    //    (QUERY - MSB) % STEP
    // then put it back into the folding range
    //    ScrambleResult + RANGE_MSB
    //    StepCount = (QUERY - MSB) / STEP
    //    RANGE_MSB = MSB + StepCount * STEP
    //    Result = ScrambleResult + MSB + (((QUERY - MSB) / STEP) * STEP)
    return Scramble::template query<(QUERY - MSB) % STEP>() + MSB + (((QUERY - MSB) / STEP) * STEP);
  }

  // Query function, templated only with the index to look up
  // Enabled if QUERY does not fall between MSB and LSB
  template <int QUERY>
  static constexpr typename std::enable_if<(!(MSB <= QUERY and QUERY <= LSB)), int>::type
  query() {
    return QUERY;
  }

};


#endif
