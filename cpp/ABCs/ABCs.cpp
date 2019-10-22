#include <iomanip>
#include <iostream>


class MyBaseClass {
public:
  virtual int foo() = 0;
  int bar() { return 0xb; }
};


class MyDerivedClass : public MyBaseClass {
public:
  int foo() { return 5; }
  int bar() { return 0; }

};


class MyDerivedDerivedClass : public MyDerivedClass {
public:
  // int foo() { return 5; }
};


int main() {
  auto mdc = new MyDerivedClass();

  std::cout << mdc << " : " << mdc->foo() << std::endl;
  std::cout << mdc << " : " << mdc->bar() << std::endl;

}
