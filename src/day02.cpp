// day02.cpp - Advent of Code 2025 day 2

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

#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string_view>

static int64_t nDigits(int64_t n) {
  return static_cast<int64_t>(std::floor(std::log10(static_cast<double>(n)))) +
         1;
}

int64_t powi(int64_t x, int64_t y) {
  assert(y >= 0);
  int64_t ret = 1;
  while (y-- > 0) {
    ret *= x;
  }
  return ret;
}

// When `n` has an even number of digits, return the digits in the top and
// buttom halves.
std::tuple<int64_t, int64_t> getDigitHalves(int64_t n) {
  int64_t digits = nDigits(n);
  assert(digits % 2 == 0 && "need even number of digits");
  int64_t split = powi(10, digits / 2);
  return {n / split, n % split};
}

/// Range of numbers, inclusive of both endpoints.
struct Range {
  int64_t lo;
  int64_t hi;

  static optional<Range> parse(string_view text);

  friend std::ostream &operator<<(std::ostream &os, const Range &r);

  // True when the low and high endpoints have the same number of digits.
  bool hasSameDigitLen() const;

  // If the low and high endpoints have different numbers of digits,
  // add an equivalent series of ranges to `splitRanges` that cover the
  // original range exactly and only have endpoints with the same number
  // of digits. Otherwise, copies the range into `splitRanges`.
  void sameDigitLenSplit(vector<Range> &splitRanges) const;

  // Sum all the numbers in the range that are of the form [num][num] in
  // decimal.
  int64_t repeatedHalvesSum() const;
};

optional<Range> Range::parse(string_view text) {
  string_view::size_type dash = text.find('-');
  if (dash == string_view::npos) {
    return std::nullopt;
  }
  auto maybeLo = parseDecimalInt<int64_t>(text.substr(0, dash));
  auto maybeHi = parseDecimalInt<int64_t>(text.substr(dash + 1));
  if (!maybeLo || !maybeHi) {
    return std::nullopt;
  }
  return {{*maybeLo, *maybeHi}};
}

std::ostream &operator<<(std::ostream &os, const Range &range) {
  return os << range.lo << "-" << range.hi;
}

bool Range::hasSameDigitLen() const { return nDigits(lo) == nDigits(hi); }

void Range::sameDigitLenSplit(vector<Range> &splitRanges) const {
  if (hasSameDigitLen()) {
    splitRanges.push_back(*this);
    return;
  }
  int64_t digitsLo = nDigits(lo);
  int64_t digitsHi = nDigits(hi);
  for (int64_t d = digitsLo; d <= digitsHi; ++d) {
    int64_t thisLo = d == digitsLo ? lo : powi(10, d - 1);
    int64_t thisHi = d == digitsHi ? hi : powi(10, d) - 1;
    splitRanges.emplace_back(thisLo, thisHi);
  }
}

int64_t Range::repeatedHalvesSum() const {
  assert(hasSameDigitLen() && "range should've been split");
  if (nDigits(lo) % 2 != 0) {
    return 0;
  }
  auto [hiLo, loLo] = getDigitHalves(lo);
  auto [hiHi, loHi] = getDigitHalves(hi);
  int64_t combiner = powi(10, nDigits(lo) / 2);
  std::cout << *this << ": [" << hiLo << "/" << loLo << "-" << hiHi << "/"
            << loHi << "]\n";
  auto dupHalf = [&](int64_t v) { return v * combiner + v; };
  // Special case so we don't double-count.
  if (hiLo == hiHi) {
    if (loLo <= hiLo && loHi >= hiHi) {
      return dupHalf(hiLo);
    } else {
      return 0;
    }
  }

  int64_t ret = 0;
  if (loLo <= hiLo) {
    ret += dupHalf(hiLo);
  }
  if (loHi >= hiHi) {
    ret += dupHalf(hiHi);
  }
  for (int64_t v = hiLo + 1; v < hiHi; ++v) {
    ret += dupHalf(v);
  }
  return ret;
}

static int64_t solveA(std::span<Range> input) {
  vector<Range> splitRanges;
  for (const Range &r : input) {
    r.sameDigitLenSplit(splitRanges);
  }
  return std::accumulate(
      splitRanges.begin(), splitRanges.end(), 0ll,
      [](int64_t acc, const Range &r) { return acc + r.repeatedHalvesSum(); });
}

static vector<Range> parse(string_view input) {
  while (input.back() == '\n' || input.back() == '\r') {
    input.remove_suffix(1);
  }
  vector<Range> ret;
  for (auto textChunk : input | std::views::split(","sv)) {
    string_view rangeText{textChunk.cbegin(), textChunk.cend()};
    auto maybeRange = Range::parse({rangeText});
    if (!maybeRange) {
      std::cerr << "Invalid range: " << rangeText << "\n";
      std::exit(1);
    }
    ret.push_back(*maybeRange);
  }
  return ret;
}

int main(int argc, char **argv) {
  std::string rawInput = getInput(argc, argv);
  vector<Range> input = parse(rawInput);
  int64_t partA = solveA(input);
  std::cout << "Part A: " << partA << "\n";
  return 0;
}
