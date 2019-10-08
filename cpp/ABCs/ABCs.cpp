#include <iomanip>
#include <iostream>


class MyBaseClass {
public:
  virtual int foo() = 0;
};


class MyDerivedClass : public MyBaseClass {
public:
  int foo() { return 5; }
};


class MyDerivedDerivedClass : public MyDerivedClass {
public:
  // int foo() { return 5; }
};


int main() {
  MyBaseClass* mdc = new MyDerivedDerivedClass();

  std::cout << mdc << " : " << mdc->foo() << std::endl;

}
