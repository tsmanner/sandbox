#include <functional>
#include <iostream>

using std::bind;
using std::cout;
using std::endl;
using std::function;
using std::placeholders::_1;
using std::placeholders::_2;


template <typename... FunctionArgs>
struct EventBase {
  using Func = function<void(FunctionArgs...)>;

  template <typename FunctionType, typename ObjectType>
  static Func binder(FunctionType function, ObjectType* object) {
    return [=](FunctionArgs... inFunctionArgs) -> void { return bind(function, object, inFunctionArgs...)(); };
  }
};


struct S {
  void func(int x, int y, int z) {
    cout << this << " (" << x << ", " << y << ")" << endl;
  }

  void func0() {
    cout << this << endl;
  }

};


struct Event : public EventBase<int, int, int> {};
struct EmptyEvent : public EventBase<> {};


int main() {
  S s = S();
  Event::Func f = Event::binder(&S::func, &s);
  f(1, 2, 3);

  EmptyEvent::Func fe = EmptyEvent::binder(&S::func0, &s);
  fe();

  return 0;
}
