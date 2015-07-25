#ifndef utils_hpp
#define src_vm_utils_hpp_1245 

#include <cstring>

template<class T>
void zerofill(T&& obj) {
  std::memset(&obj, 0, sizeof(T));
}

#endif//utils_hpp
