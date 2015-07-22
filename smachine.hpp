#ifndef smachine_hpp_2015_0721_1224
#define smachine_hpp_2015_0721_1224 

class StackMachine final : virtual public Machine {
public:
  enum Op {
    terminate_ = 0,
    dump_,
    push_,
    clear_,
    add_,
    sub_,
    mul_,
    div_,
    push_s_,
    jump_,
    noop_
  };
private:
  void v_execute(const code_t&) override;
  void v_reset() override;

  void dump(const segment_t& seg) const
    {
      using namespace std;
      cout << "<dump>" << endl;
      size_t count = 0;
      for(const auto& i : seg) {
        cout << count << ":" << i << "\n";
        ++count;
      }
      cout << "</dump>" << endl;
    }
private: 
  stack_t stack_;
};

inline
void StackMachine::v_reset() {
  stack_.clear();
}

inline
void StackMachine::v_execute(const code_t& code) 
{

  typename code_t::size_type instr_p = 0;
  bool halt = false;

  auto instruction_in_bounds = [&] {
    return (instr_p > 0) && (instr_p < code.size());
  };

  auto fetch = [&] () -> word_t {
    // TODO: potential huge bug here if using signed types
    if(instr_p < code.size()) {
      return code[instr_p++];
    }
    else { 
      return 0;
    }
  };

  auto push = [&](auto word) {
    stack_.push_back(word);
  };

  auto push_s = [&](auto count) {
    for(unsigned i = 0; i < count; ++i) {
      push(fetch());
    }
  };

  auto clear = [&] {
    stack_.clear();
  };

  auto pop = [&] () -> word_t {
    if(stack_.size() > 0) {
      auto word = stack_.back();
      stack_.pop_back();
      return word;
    } else {
      return 0;
    }
  };

  auto add = [&] 
  {

    auto op2 = pop();
    auto op1 = pop();
    push(op1 + op2);
  };

  auto sub = [&] 
  {

    auto op2 = pop();
    auto op1 = pop();
    push(op1 - op2);
  };

  auto mul = [&]
  {
    auto op2 = pop();
    auto op1 = pop();
    push(op1 * op2);
  };

  auto div = [&]
  {
    auto op2 = pop();
    auto op1 = pop();
    push(op1 / op2);
  };

  auto jump = [&]
  {
    instr_p = pop();
    if(instr_p >= code.size()) {
      halt = true;
    }
  };

  auto identify_instruction = [&] () {
    auto op = fetch();
    switch(op) {
    case terminate_:
      halt = true;
      break;
    case dump_:
      dump(stack_);
      break;
    case push_:
      {
        auto word = fetch();
        push(word);
      }
      break;
    case clear_:
      clear();
      break;
    case add_:
      add();
      break;
    case sub_:
      sub();
      break;
    case mul_:
      mul();
      break;
    case div_:
      div();
      break;
    case jump_:
      jump();
      break;
    case noop_:
      break;
    default:
      break;
    };
  };

  while(halt == false) {
    identify_instruction();
  }
}

#endif//smachine_hpp_2015_0721_1224
