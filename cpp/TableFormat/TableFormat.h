#ifndef TableFormat_h
#define TableFormat_h


#include <iomanip>
#include <iostream>
#include <type_traits>


template <int WIDTH, int COLUMNS>
struct Table {

  template <typename T, typename... Ts>
  static typename enable_if<(sizeof...(Ts) == 0), std::ostream&>::type
  cell(std::ostream& os, const T& t, const Ts&... ts) {
    return os << " " << std::setw(WIDTH) << t << " |";
  }


  template <typename T, typename... Ts>
  static typename enable_if<(sizeof...(Ts) != 0), std::ostream&>::type
  cell(std::ostream& os, const T& t, const Ts&... ts) {
    return cell(os << " " << std::setw(WIDTH) << t << " |", ts...);
  }


  template <typename... Ts>
  static std::ostream&
  row(std::ostream& os, const Ts&... ts) {
    return cell(os << "|", ts...);
  }


  template <int COLUMN>
  static typename enable_if<(COLUMN == COLUMNS), std::ostream&>::type
  div(std::ostream& os) {
    return os;
  }


  template <int COLUMN>
  static typename enable_if<(0 < COLUMN and COLUMN < COLUMNS), std::ostream&>::type
  div(std::ostream& os) {
    return div<COLUMN+1>(os << std::setfill('-') << std::setw(WIDTH + 3) << "+") << std::setfill(' ');
  }

  template <int COLUMN=0>
  static typename enable_if<(COLUMN == 0), std::ostream&>::type
  div(std::ostream& os) {
    return div<COLUMN+1>(os << "+" << std::setfill('-') << std::setw(WIDTH + 3) << "+") << std::setfill(' ');
  }
};


#endif
