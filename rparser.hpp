#ifndef rparser_hpp_2015_0724_1357
#define rparser_hpp_2015_0724_1357 

#include "ast.hpp"
#include "utils.hpp"

#include <boost/lexical_cast.hpp>

class RParser {
public:
  typedef std::string string_t;
  typedef std::unique_ptr<const Node> node_p;
  typedef std::function<string_t(void)> reload_fn_t;
  void parse(const string_t&, reload_fn_t);
  auto ast() const -> const node_p&;
  
  const std::runtime_error err_expected_register_id() {
    return std::runtime_error("Expected: register id [0-255].");
  }

  const std::runtime_error err_unexpected_symbol(const char* fn, const string_t& s)
  {
    using namespace std;
    stringstream ss;
    ss << fn << ": Unexpected symbol:" << s;
    return std::runtime_error(ss.str()); 
  }

private:
  node_p ast_;  
};

#include "lexer.hpp"
#include "machine.hpp"

inline
auto RParser::ast() const -> const node_p&
{
  return ast_;
}

inline
void RParser::parse(const string_t& s, std::function<string_t()> reload_fn)
{

  Lexer lexer(s);

  auto get_token = [&] {
    auto token = lexer.scan();
    //std::cerr << "<" << lexer.symbol() << ">" << std::endl;
    return token;
  };

  auto get_number = [&] {
    auto token = get_token();
    if(token == Lexer::tk_number) {
      return boost::lexical_cast<word_t>(lexer.symbol());
    } else {
      throw std::runtime_error("Expected number.");
    }
  };

  auto expect_end = [&] {
    auto token = get_token();
    if(token != Lexer::tk_semicolon) {
      throw std::runtime_error("Expected ';'");
    }
  };

  auto token_is_register = [&] {
    using namespace std;
    return lexer.token() == Lexer::tk_literal && lexer.symbol() == "%";
  };


  auto parse_move_register_to_register = [&] (auto source_register) {
    auto token = get_token();
    if(token != Lexer::tk_number) {
      throw err_expected_register_id();
    }
    expect_end();
    auto target_register = 
      boost::lexical_cast<RegisterMachine::op_param_t>(lexer.symbol());
    
    RegisterMachine::move_instr_t instr;
    instr.record.op = RegisterMachine::Move;
    instr.record.source = source_register;
    instr.record.target = target_register;

    return std::make_unique<Instruction>(instr.word);
  };

  auto parse_move_register = [&] () {
    auto token = get_token();
    if(token != Lexer::tk_number) {
      throw err_expected_register_id();
    }
    auto source_register = 
      boost::lexical_cast<RegisterMachine::op_param_t>(lexer.symbol());

    token = get_token();
    if(token_is_register()) {
      return parse_move_register_to_register(source_register);
    }

    throw err_unexpected_symbol("[move][reg][???] << ", lexer.symbol());
  };

  auto parse_move_number_to_register = [&] (auto number) {
    auto token = get_token();
    if(token == Lexer::tk_number) {
      auto target_register = 
        boost::lexical_cast<RegisterMachine::op_param_t>(lexer.symbol());
      RegisterMachine::move_instr_t instr;
      instr.record.op = RegisterMachine::Move;
      instr.record.source = RegisterMachine::Code;
      instr.record.target = target_register;
      return std::make_unique<Instruction>(instr.word, number);    
    }
    throw err_unexpected_symbol("[move][num][%] << ", lexer.symbol());
  };

  auto parse_move_number = [&] {
    auto number = boost::lexical_cast<word_t>(lexer.symbol());

    auto token = get_token();
    if(token_is_register()) {
      return parse_move_number_to_register(number);
    }

    throw err_unexpected_symbol("[move][num][???] << ", lexer.symbol());
  };

  auto parse_move = [&] {
    auto token = get_token();
    if(token_is_register()) {
      return parse_move_register();
    }
    if(token == Lexer::tk_number) {
      return parse_move_number();
    }
    throw err_unexpected_symbol("[move][???] << ", lexer.symbol());
  };

  auto token = get_token();
  if(token == Lexer::tk_end_of_line) {
    lexer.set(reload_fn());
    token = get_token();
    if(token == Lexer::tk_end_of_line) {
      ast_ = std::make_unique<EndOfFile>();
      return;
    }
  }

  auto parse_add_register_register = [&] (auto source_reg) {
    auto token = get_token();
    if(token != Lexer::tk_number) {
      throw err_unexpected_symbol("[add][%reg][%???] << ", lexer.symbol());
    }
    auto target_reg = lexer.symbol_as<word_t>();
    RegisterMachine::add_instr_t instr;
    zerofill(instr);
    instr.record.op = RegisterMachine::Add;
    instr.record.source_reg = source_reg;
    instr.record.target_reg = target_reg;
    return std::make_unique<Instruction>(instr.word);
  };

  auto parse_add_register = [&] {
    auto token = get_token();
    if(token != Lexer::tk_number) {
      throw err_unexpected_symbol("[add][%???] << ", lexer.symbol());
    }
    auto source_reg = lexer.symbol_as<word_t>(); 
    token = get_token();
    if(token_is_register()) {
      return parse_add_register_register(source_reg);
    }
    throw err_unexpected_symbol("[add][%reg][???] << ", lexer.symbol());
  };

  auto parse_add_number_to_register = [&] (auto number) {
    auto token = get_token();
    if(token != Lexer::tk_number) {
      throw err_unexpected_symbol("[add][n][%???] << ", lexer.symbol());
    }
    auto target_reg = lexer.symbol_as<RegisterMachine::op_param_t>();
    RegisterMachine::register_instr_t instr;
    zerofill(instr);
    instr.record.op =     RegisterMachine::Add;
    instr.record.source = RegisterMachine::Code;
    instr.record.target = RegisterMachine::Register;
    instr.record.target_reg = target_reg;
    return std::make_unique<Instruction>(instr.word, number);
  };

  auto parse_add_number = [&] (auto number) {
    auto token = get_token();
    if(token_is_register()) {
      return parse_add_number_to_register(number);
    }
    throw err_unexpected_symbol("[add][n][???] << ", lexer.symbol());
  };
  auto parse_add = [&] {
    auto token = get_token();
    if(token_is_register()) {
      return parse_add_register();
    }
    if(token == Lexer::tk_number) {
      auto number = lexer.symbol_as<word_t>();
      return parse_add_number(number);
    }
    throw err_unexpected_symbol("[add][???] << ", lexer.symbol());
  };

  switch(token) {
  case Lexer::kw_push:
    {
      auto n = get_number();
      expect_end();
      ast_ = std::make_unique<Instruction>(RegisterMachine::Push, n);
    }
    break;
  case Lexer::kw_add:
    ast_ = parse_add();
    break;
  case Lexer::kw_div:
    expect_end();
    ast_ = std::make_unique<Instruction>(RegisterMachine::Div);
    break;
  case Lexer::kw_terminate:
    expect_end();
    ast_ = std::make_unique<Instruction>(RegisterMachine::Terminate);
    break;
  case Lexer::kw_dump:
    expect_end();
    ast_ = std::make_unique<Instruction>(RegisterMachine::Dump);
    break;
  case Lexer::kw_mul:
    expect_end();
    ast_ = std::make_unique<Instruction>(RegisterMachine::Mul);
    break;
  case Lexer::kw_jump:
    {
      auto j = get_number();
      expect_end();
      ast_ = std::make_unique<Instruction>(RegisterMachine::Jump, j);
    }
    break;
  case Lexer::kw_jne:
    break;
  case Lexer::kw_move:
    ast_ = parse_move();
    break; 
  default:
    {
      throw err_unexpected_symbol("[*]", lexer.symbol());
    }
  }
}

auto Lexer::token() -> const Token& 
{
  return token_;
}

#endif//rparser_hpp_2015_0724_1357
