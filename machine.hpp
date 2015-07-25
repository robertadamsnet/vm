#ifndef machine_hpp_2015_0714_1649
#define machine_hpp_2015_0714_1649 

#include <vector>
#include <iostream>

typedef uint64_t word_t;
typedef std::vector<word_t> segment_t;

typedef segment_t stack_t;
typedef segment_t code_t;

typedef word_t result_t;

class Machine {
public:
  void execute(const code_t& c) { v_execute(c); }
  void reset() { v_reset(); }
private:
  virtual void v_execute(const code_t&) = 0;
  virtual void v_reset() = 0;
};


#include "smachine.hpp"
//TODO: Implement a register machine?  Maybe?  If ya wanna?
//#include "rmachine.hpp"


#endif//machine_hpp_2015_0714_1649
