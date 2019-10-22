#include <iostream>
#include <unordered_set>
#include <string>
#include <strings.h>

using std::cout;
using std::endl;
using std::string;
using std::string_view;
using std::unordered_set;


struct Compare {

  bool operator()(const string& lhs, const string& rhs) {
    return lhs == rhs;
  }

  bool operator()(const string& lhs, const string_view& rhs) {
    return lhs == rhs;
  }

  bool operator()(const string_view& lhs, const string& rhs) {
    return lhs == rhs;
  }

  bool operator()(const string_view& lhs, const string_view& rhs) {
    return lhs == rhs;
  }

};




int main() {
  cout << endl << endl << "main():" << endl;

  auto s = unordered_set<string, std::hash<string>, std::equal_to<>>();
  s.insert("hello");
  s.insert("world");

  s.find(
    // string_view("hello")
    std::hash<string>()("hello")
  );

  cout << endl << endl;
  return 0;
}
