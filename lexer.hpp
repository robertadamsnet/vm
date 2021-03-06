#ifndef lexer_hpp_2015_0714_1641
#define lexer_hpp_2015_0714_1641 

class Lexer {
public:
  enum Token {
    tk_start,
    tk_whitespace, 
    tk_identifier, 
    tk_number, 
    tk_literal, 
    tk_end_of_line,
    tk_semicolon,

    kw_push, 
    kw_add,
    kw_div,
    kw_mul,
    kw_terminate,
    kw_dump,
    kw_hex_dump,
    kw_jump,
    kw_jne,
    kw_move,
    TOKEN_MAX
  };

  typedef std::string string_t;

  Lexer(const string_t&);

  auto reset() -> void;

  auto scan() -> const Token&;

  auto token() -> const Token&;

  auto set(const string_t&) -> void;

  auto source() const -> const string_t&;

  auto symbol() const -> string_t;

  template<class T>
  auto symbol_as() const -> T;

private:
  const string_t*      source_p_;
  Token                token_     = tk_start;
  string_t::size_type  start_;
  string_t::size_type  end_;

  auto inner_scan() -> Token;
};

template<class T>
auto Lexer::symbol_as() const -> T 
{
  return boost::lexical_cast<T>(symbol());
}

inline
void Lexer::reset()
{
  start_ = 0;
  end_ = 0;
}

inline
auto Lexer::source() const -> const string_t&
{
  return *source_p_;
}

inline
auto Lexer::set(const string_t& s) -> void 
{
  source_p_ = &s;
  reset();
}

inline
Lexer::Lexer(const string_t& s) : source_p_(&s)
{
  reset();
}

inline
auto Lexer::scan() -> const Token&
{
  token_ = inner_scan();
  return token_;
}

inline
auto Lexer::inner_scan() -> Token
{
  start_ = end_;
  int ch;
  auto peek = [&] {
    ch = source()[end_];
  };

  auto accept = [&] {
    ++end_;
  };

  auto skip_space = [&] {
    while(isspace(ch)) {
      accept();
      peek();
    }
    start_ = end_;
  };

  auto get_keyword = [&] {
    auto sym = symbol();
    if(sym == "push") {
      return kw_push;
    }
    if(sym == "add") {
      return kw_add;
    }
    if(sym == "div") {
      return kw_div;
    }
    if(sym == "mul") {
      return kw_mul;
    }
    if(sym == "terminate") {
      return kw_terminate;
    }
    if(sym == "dump") {
      return kw_dump;
    }
    if(sym == "hex_dump") {
      return kw_hex_dump;
    }
    if(sym == "jump") {
      return kw_jump;
    }
    if(sym == "jne") {
      return kw_jne;
    }
    if(sym == "move") {
      return kw_move;
    }
    return tk_identifier;
  };

  auto get_identifier = [&] {
    while(isalnum(ch) || ch == '_') {
      accept();
      peek();
    }
    return get_keyword();
  };

  auto get_number = [&] {
    while(isdigit(ch) || (ch == '.')) {
      accept();
      peek();
    }
    return tk_number;
  };

  while(start_ < source().size()) {
    peek();

    if(isspace(ch)) {
      skip_space();
    }
    if(isalpha(ch)) {
      return get_identifier();
    }
    if(isdigit(ch)) {
      return get_number();
    }
    if(ch == ';') {
      accept();
      return tk_semicolon;
    }
    accept();
    return tk_literal;
  } 
  return tk_end_of_line;
}

inline
auto Lexer::symbol() const -> string_t {
  return string_t(source(), start_, end_ - start_);
}

#endif//lexer_hpp_2015_0714_1641
