#include "utils.h"

#include <cstdlib>
#include <fstream>
#include <iostream>

std::string getInput(int argc, char **argv, int nArgs, string_view argsDesc) {
  if (argc < nArgs + 1 || argc > nArgs + 2) {
    std::cerr << "Usage: " << argv[0] << " " << argsDesc << " [input file name]\n";
    std::exit(1);
  }

  if (argc == nArgs + 1) {
    std::istreambuf_iterator<char> slurper{std::cin}, end;
    return std::string(slurper, end);
  }

  std::ifstream file(argv[argc - 1]);
  std::istreambuf_iterator<char> slurper(file), end;
  std::string ret(slurper, end);
  file.close();
  return ret;
}
