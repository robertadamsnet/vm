#ifndef rmachine_hpp_2015_0721_1222
#define rmachine_hpp_2015_0721_1222 

#include <vector>
#include <array>

class RegisterMachine final : virtual public Machine {
public:
  typedef uint8_t op_param_t;
  static constexpr op_param_t register_id_max = std::numeric_limits<op_param_t>::max();
  enum Op {
    Move,
    Add,
    Sub,
    Mul,
    Div,
    Push,
    Dump,
    Jump,
    Terminate,
    MAX
  };

  enum Addr {
    Register,
    Code,
    Stack
  };

  union register_instr_t {
    word_t word;
    struct record_t {
      uint32_t        op;
      op_param_t      source;
      op_param_t      target;  
      op_param_t      source_reg; 
      op_param_t      target_reg;
    } record;
  };

  typedef register_instr_t move_instr_t;
  typedef register_instr_t add_instr_t;

  RegisterMachine();

  void do_dump() const;
private:
  typedef std::vector<word_t> stack_t;
  stack_t stack_;

  std::array<word_t, register_id_max> reg_;


  void v_execute(const code_t&) override;
  void v_reset() override;
};

RegisterMachine::RegisterMachine() {
  reg_.fill(0);
}

#include "terminal.hpp"

void RegisterMachine::do_dump() const {
  Terminal::move(0,0);
  using namespace std;
  auto dump_registers = [&] {
    
    unsigned r = 0;
    while(r < register_id_max) {
      constexpr unsigned registers_per_line = 16;
      if(!(r % registers_per_line)) {
        Terminal::cout() << "\n";
      }
      Terminal::cout() << hex << reg_[r] << " ";
      ++r;
    }
  };

  dump_registers();
}

inline
void RegisterMachine::v_reset() {
  for(auto& i : reg_) {
    i = 0;
  }
}

inline
void RegisterMachine::v_execute(const code_t& code) {
  auto begin = code.begin();
  auto ip = begin;
  auto end = code.end();
  word_t word;
  if(ip == end) {
    word = 0;
  } else {
    word = *ip;
  }

  auto in_bounds = [&] {
    return ip >= begin && ip < end;
  };

  move_instr_t instr;
  auto get_word = [&] {
    if(in_bounds()) {
      word = *ip++;
      instr.word = word;
    }
  };

  auto get_source = [&] {
    switch(instr.record.source) {
    case Register:
      // set source pointer to register indicated by.source_reg
      return reg_[instr.record.source_reg];
    case Code:
      get_word();
      return word;
    case Stack:
      get_word();
      return word < stack_.size() ? stack_[word] : 0;
      break;  
      // TODO: implement this
    }
    return (word_t)0;
  };

  auto do_move = [&] {
    reg_[instr.record.target_reg] = get_source();
  };

  auto do_add = [&] {
    reg_[instr.record.target_reg] += get_source();
  };

  auto do_sub = [&] {
    reg_[instr.record.target_reg] -= get_source();
  };

  auto do_mul = [&] {
    reg_[instr.record.target_reg] *= get_source();
  };

  auto do_div = [&] {
    reg_[instr.record.target_reg] /= get_source();
  };

  auto do_jump = [&] {
    get_word();
    ip = begin + word;
  };

  while(in_bounds()) {
    get_word();
    switch(instr.record.op) {
    case Op::Move:
      do_move(); 
      break;
    case Op::Add:
      do_add();
      break;
    case Op::Sub:
      do_sub();
      break;
    case Op::Mul:
      do_mul();
      break;
    case Op::Div:
      do_div();
      break;
    case Op::Dump:
      do_dump();
      break;
    case Op::Jump:
      do_jump();
      break;
    }
  }
}



#endif//rmachine_hpp_2015_0721_1222
