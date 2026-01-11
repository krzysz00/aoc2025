// utils.h - Advent of code 2025 utilities

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

#ifndef UTILS_H
#define UTILS_H

#include <cstdint>
#include <optional>
#include <string_view>
#include <vector>

/// Let's get our common using delcarations in.
using std::optional;
using std::string_view;
using std::operator""sv;
using std::vector;

/// AOC quick-and-dirty widget that reads in all the data from a file,
/// using either argv[nArgs + 1] if it exists or stdin, and returns it as
/// a string. Checks if there are nArgs arguments present in argv and exits
/// the program otherwise.
std::string getInput(int argc, char **argv, int nArgs = 0,
                     std::string_view argsDes = {});

/// Parse the string slice `slice` as a decimal integer.
template <typename IntT> optional<IntT> parseDecimalInt(string_view slice) {
  IntT ret = 0;
  for (const char &c : slice) {
    if (c < '0' || c > '9') {
      return std::nullopt;
    }
    ret = ret * 10 + static_cast<IntT>(c - '0');
  }
  return ret;
}

#endif // UTILS_H
