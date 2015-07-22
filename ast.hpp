#ifndef AST_HPP_2015_0715_2241
#define AST_HPP_2015_0715_2241 

#include "machine.hpp"

class EndOfFile;
class Instruction;

class Host {
public:
  virtual void accept(const EndOfFile&) = 0;
  virtual void accept(const Instruction&) = 0;
};

class Node
{
public:
  typedef std::string string_t;
  void visit(Host& h) const { v_visit(h); }
  auto name() const -> const string_t& { return v_name(); }
  auto type() const -> const string_t& { return v_type(); }
private:
  virtual void v_visit(Host&) const = 0;
  virtual auto v_name() const -> const string_t&
  {
    static string_t name = "<unknown>";
    return name;
  }
  virtual auto v_type() const -> const string_t& 
  {
    static string_t type = "object";
    return type;
  }
};

class Instruction final : public Node
{
public:
  template<class...Params>
  Instruction(Params&&...params) : data_({params...})
  {
    
  }
  
  typedef std::vector<word_t> data_t;
  const data_t& data() const { return data_; }
private:
  data_t data_;
  void v_visit(Host& host) const override 
  {
    host.accept(*this);
  }
  auto v_name() const -> const string_t& override
  {
    static string_t name = "instruction";
    return name;
  }
  auto v_type() const -> const string_t& override
  {
    static string_t type = "instruction";
    return type;
  }
};

class EndOfFile final : public Node
{
private:
  void v_visit(Host& host) const override
  {
    host.accept(*this);
  }
  auto v_name() const -> const string_t& override
  {
    static string_t name = "end-of-file";
    return name;
  }
  auto v_type() const -> const string_t& override
  {
    static string_t type = "token";
    return type;
  }
};

#endif//AST_HPP_2015_0715_2241
