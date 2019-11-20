#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <string>
using std::cout;
using std::endl;
using std::setw;

#include "MultiRange.h"
#include "object_ptr.h"

#include "Node.h"
#include "Operation.h"


struct ThingToSchedule;


struct Constraint {
  MultiRange mRange;

  ThingToSchedule* mTail;
  ThingToSchedule* mHead;

};


struct Scheduler;


struct ThingToSchedule {
  ThingToSchedule(const std::string& inName): mName(inName) {}

  void setScheduler(Scheduler* inScheduler) { mScheduler = inScheduler; }

  MultiRange getSchedulableCycles();

  object_ptr<Node<int>> mTime = object_ptr<Node<int>>(new Node<int>());
  std::string mName;
  Range mScheduleRange = Range(0, UpperBound());
  Scheduler* mScheduler = nullptr;

};


struct Scheduler {
  void schedule() {
    for (auto iter = mPending.begin(); iter != mPending.end();) {
      auto thing = *iter;
      if (thing->mTime->isReal()) {
        if (mSchedule.find(thing->mTime->getValue()) != mSchedule.end()) {
          cout
            << "Collision at time " << thing->mTime->getValue()
            << ", entry `" << mSchedule[thing->mTime->getValue()]->mName
            << "` will be overwritten by `"
            << thing->mName << "`"
            << endl;
        }
        mSchedule[thing->mTime->getValue()] = thing;
        iter = mPending.erase(iter);
      }
      else {
        ++iter;
      }
    }
  }

  MultiRange getSchedulableRange() {
    auto mr = MultiRange();
    mr.addRange(Range(0, UpperBound()));  // 0 to +inf
    for (auto p : mSchedule) {
      auto notThisCycle = MultiRange();
      notThisCycle.addRange(Range(LowerBound(), p.first-1));
      notThisCycle.addRange(Range(p.first+1, UpperBound()));
      mr = MultiRange::conjunction(mr, notThisCycle);
    }
    return mr;
  }

  void dumpSchedule(std::ostream& os) {
    os << "Schedule:" << endl;
    for (auto p : mSchedule) {
      os << "  " << setw(2) << p.first << ": " << p.second->mName << " " << p.second->mTime->to_string(true) << endl;
    }
  }

  void addThingToSchedule(ThingToSchedule* thing) {
    mPending.insert(thing);
    thing->mTime->addOnRealFunction(std::bind(&Scheduler::schedule, this));
    thing->mScheduler = this;
  }

  std::set<ThingToSchedule*> mPending;
  std::map<int, ThingToSchedule*> mSchedule;

};


//
// ThingToSchedule
//
MultiRange ThingToSchedule::getSchedulableCycles() {
  MultiRange mr = MultiRange();
  mr.addRange(mScheduleRange);  // -inf to +inf
  if (mScheduler) {
    mr = MultiRange::conjunction(mr, mScheduler->getSchedulableRange());
  }
  return mr;
}


int main() {
  //
  // c = a + b
  //

  auto a = new ThingToSchedule("a");
  auto b = new ThingToSchedule("b");
  auto c = new ThingToSchedule("c");
  a->mScheduleRange = Range(0, 20);
  b->mScheduleRange = Range(0, 20);
  c->mTime = new Operation<int>(
    "+",
    [](const int& lhs, const int& rhs) -> int { return lhs + rhs; },
    new Node<int>(6),
    new Operation<int>(
      "max",
      [](const int& lhs, const int& rhs) -> int { return std::max(lhs, rhs); },
      a->mTime,
      b->mTime
    )
  );
  c->mScheduleRange = Range(6, 40);  // ([0:20] + ([0:20] max 6)) -> ([0:20] + [6:20]) -> [6:40]

  cout << a->mTime->to_string() << endl;
  cout << b->mTime->to_string() << endl;
  cout << c->mTime->to_string() << endl;

  Scheduler scheduler = Scheduler();
  scheduler.addThingToSchedule(a);
  scheduler.addThingToSchedule(b);
  scheduler.addThingToSchedule(c);

  cout << scheduler.getSchedulableRange() << endl;

  scheduler.dumpSchedule(cout);
  a->mTime->setValue(a->getSchedulableCycles().draw());
  scheduler.dumpSchedule(cout);
  b->mTime->setValue(b->getSchedulableCycles().draw());
  scheduler.dumpSchedule(cout);

  cout << scheduler.getSchedulableRange() << endl;

  return 0;
}
