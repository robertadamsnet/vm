/*  
    Copyright (c) 2015 by Robert T. Adams 
    All Rights Reserved.

    See LICENSE.txt for license information.
    */

#include "rparser.hpp"
#include "ast_dump.hpp"
#include "assembler.hpp"
#include <fstream>
#include <boost/algorithm/string/trim.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

/* Flag set by ‘--verbose’. */
//static int verbose_flag;

int main (int argc, char **argv)
try {
  using namespace std;
  int c;
  string input_filespec;
  string output_filespec;
  RParser parser;
  Assembler assembler;
  bool flag_execute = true;
  bool flag_compile = true;

  auto load = [&] {
    using namespace std;
    
    auto inner = [&] {
      boost::trim(input_filespec);
      if(input_filespec.size() == 0) {
        stringstream msg;
        msg << "Invalid filename: \"" << input_filespec << "\"";
        throw(runtime_error(msg.str()));
      }
      cout << "Loading " << input_filespec << "..." << endl;
      using namespace std; 
      ifstream ifs(input_filespec);
      if(ifs.fail()) {
        if(eaccess(input_filespec.c_str(), F_OK)) {
          throw(runtime_error("Specified file does not exist."));
        } 
        else if(eaccess(input_filespec.c_str(), R_OK)) {
          throw(runtime_error("Cannot open file: permission denied."));
        }
        else {
          throw(runtime_error("Unable to read file."));
        }
      }
      unsigned line_count = 0;
      while(ifs.good()) {
        string line;
        auto get_line = [&] () -> RParser::string_t {
          ++line_count; 
          string l;
          getline(ifs, l);
          return l;
        };
        line = get_line();
        try {
          parser.parse(line, get_line);
          if(flag_compile) {
            parser.ast()->visit(assembler);
          }
        } catch(std::exception& e) {
          stringstream msg;
          msg << endl 
              << "Error: " << input_filespec << ":" << line_count << ": " << e.what()
              << endl << line;
          throw(runtime_error(msg.str()));
          return;
        }
      }
      return;
    };

    inner();
    RegisterMachine machine;
    machine.execute(assembler.code());
    cout << endl;
  };


  auto inner = [&] {
    while (true)
    {
      static struct option long_options[] =
      {
        {0, 0, 0, 0}
      };
      int option_index = 0;

      c = getopt_long (argc, argv, "-",
          long_options, &option_index);

      constexpr int options_end = -1;
      if (c == options_end)
        break;

      switch (c)
      {
        case 0:
          /* If this option set a flag, do nothing else now.  */
          if (long_options[option_index].flag != 0)
            break;
          printf ("option %s", long_options[option_index].name);
          if (optarg)
            printf (" with arg %s", optarg);
          printf ("\n");
          break;

        case 'i':
          if(optarg) {
            input_filespec = optarg;
          } else {
            throw(runtime_error("Usage: --input-filespec=<filespec>"));
          }
          break;
        case 'o':
          output_filespec = optarg;
          break;
        case '?':
          /* getopt_long already printed an error message.  */
          break;
        case '\1':
          if(optarg) {
            input_filespec = optarg;
          }          
          break;
        default:
          abort ();
      }
    }
    load();
    return 0;
  };

  return inner();
} catch (const std::exception& e) {
  using namespace std;
  cout << e.what() << endl;
  return -1;
} catch (...) {
  using namespace std;
  cout << "Unknown exception caught, not handled." << endl;
  return -1;
}




