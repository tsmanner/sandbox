#ifndef MultiRange_h
#define MultiRange_h

#include <set>
#include <utility>

#include "Range.h"


class MultiRange {
public:

//   std::set<Range>& getRanges() { return mRanges; }

//   void addRange(Range r) { mRanges.insert(r); optimize(); }

private:
//   void merge(Range lhs, Range rhs) {
//     mRanges.erase(lhs);
//     mRanges.erase(rhs);
//     mRanges.insert(Range::merge(lhs, rhs));
//   }

//   void optimize() {
//     bool merged;
//     do {
//       merged = false;
//       for (auto iter = mRanges.begin(); iter != mRanges.end(); ) {
//         auto current = *iter++;
//         if (iter != mRanges.end() and Range::overlaps(current, *iter)) {
//           merge(current, *iter);
//           merged = true;
//           break;
//         }
//       }
//     } while (merged);
//   }

  std::set<Range> mRanges;

};


#endif
