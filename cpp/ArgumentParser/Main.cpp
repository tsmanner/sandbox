#include <array>
#include <iostream>
#include <string>
#include <tuple>
using std::cout;
using std::endl;

#include "Base.h"
#include "Rule.h"
#include "Streaming.h"
#include "Value.h"

using namespace parsing;


class SubRule : public Rule<Value<int, 2, 5>> {
public:
  SubRule(): Rule("subrule", {"ints"}) {}

  const Value<int, 2, 5>& getInts() const { return std::get<0>(getRules()); }

};


class MyAppArgs :
  public Root<
    Value<int, 1>,
    Value<char, 0, 1>,
    Value<std::string>,
    SubRule
  >
{
public:
  MyAppArgs():
    Root("MyApp",
      {"intval"},
      {"charval"},
      {"strval"},
      {}
    )
  {
    setFailedParseCallback(usage);
  }

  static void usage(
    const Root& inRoot,
    const Token& inStartIter,
    const Token& inCurrentIter,
    const Token& inEndIter
  ) {
      std::cout << "usage: " << stream_help(inRoot) << std::endl << "  got: ";
      std::cout << inRoot.getName();
      for (auto iter = inStartIter; iter != inEndIter; ++iter) {
        std::cout << " " << *iter;
      }
      std::cout << std::endl;
      exit(1);
  }


  const Value<int, 1>& getIntVal() const { return std::get<0>(getRules()); }
  const Value<char, 0, 1>& getCharVal() const { return std::get<1>(getRules()); }
  const Value<std::string>& getStrVal() const { return std::get<2>(getRules()); }

  const SubRule& getSubRule() const { return std::get<3>(getRules()); }

private:

};


int main(int argc, char* argv[]) {
  auto a = MyAppArgs();

  a.parse(argc, argv);

  cout << stream_help(a) << endl;

  cout << stream_value(a) << endl;

  cout << stream_name_and_value(a.getIntVal()) << endl;
  cout << stream_name_and_value(a.getCharVal()) << endl;
  cout << stream_name_and_value(a.getStrVal()) << endl;
  cout << stream_name_and_value(a.getSubRule()) << endl;
  cout << stream_name_and_value(a.getSubRule().getInts()) << endl;

  return 0;
}
