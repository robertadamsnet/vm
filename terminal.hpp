/*  
  Copyright (c) 2015 by Robert T. Adams 
  All Rights Reserved.
  
  See LICENSE.txt for licensing information.

*/
#ifndef terminal_hpp_2015_06_23_1509
#define terminal_hpp_2015_06_23_1509 

#include "rect.hpp"

#include <string>
#include <sstream>

class Terminal final {
private:
  Terminal();
  ~Terminal();
public:
  class Cache;

  static Cache            start_cache();

  static int              getch();
  static const Terminal&& no_echo();
  static const Rect&      rect();
  static void             move(int y, int x);
  static void             locate(int x, int y);
  static void             write(const char* cstr, int sz);
  static void             write(const std::string& str);
  static void             refresh();
  static void             erase();

  static void             attr_reverse();
  static void             attr_normal();

  template<class T>
  auto operator<<(T&&) -> Terminal&;

  template<class T>
  auto operator<<(const T&) -> Terminal&;

  static Terminal&&        cout() { return std::move(Terminal()); }
};

template<class T>
auto Terminal::operator<<(const T& t) -> Terminal&
{
  std::stringstream ss;
  ss << t;
  write(ss.str());
  return *this;
}

template<class T>
auto Terminal::operator<<(T&& t) -> Terminal&
{
  std::stringstream ss;
  ss << t;
  return *this;
}


#endif//terminal_hpp_2015_06_23_1509
