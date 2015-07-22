#ifndef rmachine_hpp_2015_0721_1222
#define rmachine_hpp_2015_0721_1222 

template<class T>
class RegisterSet final {
public:
  typedef T register_t;
  
};

class RegisterMachine final : virtual public Machine {
private:
  static constexpr unsigned reg_max = 4;
  word_t registers_[reg_max];
  enum Op {
    Move
  };

  void v_execute(const code_t&) override;
  void v_reset() override;
};

inline
void RegisterMachine::v_reset() {
  for(auto& i : registers_) {
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

  auto get_word = [&] {
    ++ip;
    if(in_bounds()) {
      word = *ip;
    }  
  };

  auto do_move = [&] {
    get_word();
    union {
      word_t word;
      struct {
        int8_t source;
        int8_t dest;
      };
    } params;
    params.word = word;

    word_t* source_p;
    word_t* dest_p;

    switch(params.source) {
    case 0:
    case 1:
    case 2:
    case 3:
      source_p = &registers_[params.source];
      break;
    default:
      throw std::runtime_error("Source register ID out of range.");
    }
    switch(params.dest) {
    case 0:
    case 1:
    case 2:
    case 3:
      dest_p = &registers_[params.dest];
      break;
    default:
      throw std::runtime_error("Destination register ID out of range.");
    }

    *dest_p = *source_p;
  };

  while(in_bounds()) {
    auto instr = *ip;  
    switch(instr) {
    case Op::Move:
      {
        do_move(); 
      }
      break; 
    }
  }
}



#endif//rmachine_hpp_2015_0721_1222
