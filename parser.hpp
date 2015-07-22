#ifndef assembler_hpp_2015_0715_1935
#define assembler_hpp_2015_0715_1935 

#include "ast.hpp"
#include <boost/lexical_cast.hpp>

class Parser {
public:
  typedef std::string string_t;
  typedef std::unique_ptr<const Node> node_p;
  typedef std::function<string_t(void)> reload_fn_t;
  void parse(const string_t&, reload_fn_t);
  auto ast() const -> const node_p&;
private:
  node_p ast_;  
};

#include "lexer.hpp"
#include "machine.hpp"

inline
auto Parser::ast() const -> const node_p&
{
  return ast_;
}

inline
void Parser::parse(const string_t& s, std::function<string_t()> reload_fn)
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

  auto token = get_token();
  if(token == Lexer::tk_end_of_line) {
    lexer.set(reload_fn());
    token = get_token();
    if(token == Lexer::tk_end_of_line) {
      ast_ = std::make_unique<EndOfFile>();
      return;
    }
  }
  switch(token) {
  case Lexer::kw_push:
    {
      auto n = get_number();
      expect_end();
      ast_ = std::make_unique<Instruction>(StackMachine::push_, n);
    }
    break;
  case Lexer::kw_add:
    expect_end();
    ast_ = std::make_unique<Instruction>(StackMachine::add_);
    break;
  case Lexer::kw_div:
    expect_end();
    ast_ = std::make_unique<Instruction>(StackMachine::div_);
    break;
  case Lexer::kw_terminate:
    expect_end();
    ast_ = std::make_unique<Instruction>(StackMachine::terminate_);
    break;
  case Lexer::kw_dump:
    expect_end();
    ast_ = std::make_unique<Instruction>(StackMachine::dump_);
    break;
  case Lexer::kw_mul:
    expect_end();
    ast_ = std::make_unique<Instruction>(StackMachine::mul_);
    break;
  default:
    {
      string_t msg;
      msg += "Unexpected symbol: \"";
      msg += lexer.symbol();
      msg += "\"";
      throw std::runtime_error(msg);
    }
  }
}

#endif//assembler_hpp_2015_0715_1935
