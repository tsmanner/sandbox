#ifndef Rule_h
#define Rule_h

#include <functional>
#include <iostream>
#include <list>
#include <string>
#include <tuple>

#include "Base.h"


namespace parsing {


template <typename... Rules>
class Rule {
public:
  Rule(
    const std::string& inName,
    const Rules&... inRules
  ):
    mName(inName),
    mRules(inRules...)
  {
  }

  void streamHelp(std::ostream& os) const {
    os << getName();
    _streamHelp<0>(os);
  }

  void streamValue(std::ostream& os) const {
    if (isSet()) { os << getName(); _streamValue<0>(os); }
  }

  const std::string& getName() const { return mName; }
  const bool& isSet() const { return mSet; }
  const std::tuple<Rules...>& getRules() const { return mRules; }

  Token parse(
    const Token& inCurrentIter,
    const Token& inEndIter
  ) {
    auto iter = inCurrentIter;
    if (iter != inEndIter and *iter == getName()) {
      iter = _parse<0>(++iter, inEndIter);
      mSet = true;
    }
    return iter;
  }

protected:
  template <unsigned I>
  typename std::enable_if<(I == sizeof...(Rules)), Token>::type
  _parse(
    const Token& inCurrentIter,
    const Token& inEndIter
  ) { return inCurrentIter; }

  template <unsigned I>
  typename std::enable_if<(I < sizeof...(Rules)), Token>::type
  _parse(
    const Token& inCurrentIter,
    const Token& inEndIter
  ) {
    auto iter = std::get<I>(mRules).parse(inCurrentIter, inEndIter);
    if (iter != inEndIter) iter = _parse<I+1>(iter, inEndIter);
    return iter;
  }

  template <unsigned I>
  typename std::enable_if<(I == sizeof...(Rules))>::type
  _streamHelp(std::ostream& os) const {}

  template <unsigned I>
  typename std::enable_if<(I < sizeof...(Rules))>::type
  _streamHelp(std::ostream& os) const {
    os << " ";
    std::get<I>(mRules).streamHelp(os);
    _streamHelp<I+1>(os);
  }

  template <unsigned I>
  typename std::enable_if<(I == sizeof...(Rules))>::type
  _streamValue(std::ostream& os) const {}

  template <unsigned I>
  typename std::enable_if<(I < sizeof...(Rules))>::type
  _streamValue(std::ostream& os) const {
    os << " ";
    std::get<I>(mRules).streamValue(os);
    _streamValue<I+1>(os);
  }

  bool mSet { false };

private:
  const std::string mName;
  std::tuple<Rules...> mRules;

};


template <typename... Rules>
class Root : public Rule<Rules...> {
public:
  Root(
    const std::string& inRootName,
    Rules... inRules
  ):
    Rule<Rules...>(inRootName, inRules...)
  {
  }

  void parse(
    const Token& inStartIter,
    const Token& inEndIter
  ) {
    auto finalIter = this->Rule<Rules...>::template _parse<0>(inStartIter, inEndIter);
    if (finalIter != inEndIter) {
      mFailedParseCallback(*this, inStartIter, finalIter, inEndIter);
    }
    Rule<Rules...>::mSet = true;
  }

  void parse(const std::list<std::string>& inTokens) {
    parse(inTokens.begin(), inTokens.end());
  }

  void parse(int argc, char* argv[]) {
    std::list<std::string> tokens;
    for (int i = 1; i < argc; ++i) {
      tokens.push_back(argv[i]);
    }
    parse(tokens.begin(), tokens.end());
  }

  void setFailedParseCallback(const std::function<void(const Root<Rules...>&, const Token&, const Token&, const Token&)> inCallback) {
    mFailedParseCallback = inCallback;
  }

  static void defaultFailedParseCallback(
    const Root<Rules...>& inRoot,
    const Token& inStartIter,
    const Token& inCurrentIter,
    const Token& inEndIter
  ) {
      std::cout << stream_help(inRoot) << std::endl;
      exit(1);
  }

private:
  std::function<void(const Root<Rules...>&, const Token&, const Token&, const Token&)> mFailedParseCallback { defaultFailedParseCallback };

};


} // namespace parsing

#endif
