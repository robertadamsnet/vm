/*  
  Copyright (c) 2015 by Robert T. Adams 
  All Rights Reserved.
  
  See LICENSE.txt for licensing information.

*/
#include "terminal.hpp"
#include <curses.h>
#include <stdexcept>

namespace {
  class NCInit final {
  public:
    NCInit() {
      if(initscr() == NULL) {
        throw std::runtime_error("NCInit::NCInit(): 'initscr()' fail.");
      }
    }
    ~NCInit() {
      static int death_count = 0;
      if(++death_count > 1) {
        throw std::runtime_error("NCInit destructor called more than once.");
      }
      endwin();
    }
  };
  int cache_count_ = 0;

  inline void cache_hold() {
    ++cache_count_;
  }

  inline void cache_release() {
    --cache_count_;
    refresh();
  }
}

/*
class Terminal::Cache final {
public:
  Cache()  {
    cache_hold();
  }

  ~Cache() {
    cache_release();
  }
};
*/


#pragma push_macro("erase")
#undef erase
void Terminal::erase() {
  Terminal t;
#pragma pop_macro("erase")
  ::erase();
#undef erase
}

Terminal::Terminal() {
  static NCInit nc_init;
}

Terminal::~Terminal() {
}

const Terminal&& Terminal::no_echo() {
  Terminal t;
  noecho();
  return std::move(t);
}

#pragma push_macro("getch")
#undef getch
int Terminal::getch() {
  Terminal t;
#pragma pop_macro("getch")
  return ::getch();
#undef getch
}

#pragma push_macro("refresh")
#undef refresh
void Terminal::refresh() {
  Terminal t;
#pragma pop_macro("refresh")
  //if(!cache_count_)
    ::refresh();
#undef refresh
}

const Rect& Terminal::rect() {
  static Rect rect_;
  Terminal t;
  int x, y;
  getmaxyx(stdscr, y, x);
  rect_ = { 0, 0, x - 1, y - 1};
  return rect_;
}


#pragma push_macro("move")
#undef move
void Terminal::move(int y, int x) {
  Terminal t;
#pragma pop_macro("move")
  ::move(y, x);
#undef move
}


void Terminal::locate(int x, int y) {
  Terminal::move(y, x);
}

void Terminal::write(const char* s, int n) {
  Terminal t;
  addnstr(s, n);
  refresh();
}

void Terminal::write(const std::string& s) {
  write(s.c_str(), s.size());
}

void Terminal::attr_reverse() {
  attrset(A_REVERSE);
}

void Terminal::attr_normal() {
  attrset(A_NORMAL);
}

