cmake_minimum_required(VERSION 3.0.2)

add_executable(vm main.cpp terminal.cpp)
target_link_libraries(vm curses)
