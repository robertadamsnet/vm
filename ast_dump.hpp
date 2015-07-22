#ifndef ast_dump_hpp_2015_0716_1212
#define ast_dump_hpp_2015_0716_1212 

#include "ast.hpp"

class AstDump final : public Host {
public:
  void accept(const EndOfFile& n) override           { dump(n); }
  void accept(const Instruction& n) override    { dump(n); }
private:
  void dump(const Node& n) {
    using namespace std;
    cout << n.name() << ":" << n.type() << "\n";
  };
};

#include <iostream>


#endif//ast_dump_hpp_2015_0716_1212
