#ifndef ast_compile_hpp_2015_07_15_2353
#define ast_compile_hpp_2015_07_15_2353 

#include "ast.hpp"

#include "machine.hpp"

class Assembler final  : public Host {
public:
  void accept(const EndOfFile&) override
  {

  }

  void accept(const Instruction& node) override
  {
    for(const auto& d : node.data()) {
      code_.push_back(d);
    }
  }

  const code_t& code() const { return code_; }

  void reset() {
    code_.clear();
  }
private:
  code_t code_;
};

#endif//ast_compile_hpp_2015_07_15_2353
