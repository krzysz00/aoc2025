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
