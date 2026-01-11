// utils.cpp - Advent of Code 2025 utilities

// Copyright (c) 2026 Krzysztof Drewniak
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program. If not, see <https://www.gnu.org/licenses/>.

#include "utils.h"

#include <cstdlib>
#include <fstream>
#include <iostream>

std::string getInput(int argc, char **argv, int nArgs, string_view argsDesc) {
  if (argc < nArgs + 1 || argc > nArgs + 2) {
    std::cerr << "Usage: " << argv[0] << " " << argsDesc
              << " [input file name]\n";
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
