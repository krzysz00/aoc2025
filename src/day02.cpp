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
#include <compare>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string_view>
#include <unordered_set>

using std::span;
using std::strong_ordering;
using intSet = std::unordered_set<int64_t>;

static int64_t nDigits(int64_t n) {
  assert(n > 0 && "non-positive numbers can't happen in this problem");
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

/// Split a number `n` into `k` sub-parts that all have the same number of
/// decimal digits. The number of digits in `n`'s decimal representation
/// must be evenly divisible by `k`.
/// When `n` has an even number of digits, return the digits in the top and
/// buttom halves.
vector<int64_t> getDigitParts(int64_t n, int64_t k) {
  int64_t digits = nDigits(n);
  assert(digits % k == 0 && "need even number of digits");
  int64_t partLen = digits / k;
  int64_t split = powi(10, partLen);
  vector<int64_t> ret(k, 0);
  for (int64_t &d : std::views::reverse(ret)) {
    d = n % split;
    n /= split;
  }
  return ret;
}

/// General implementation of the "will we get to all equal parts if we kept
/// incrementing/decrementing" test needed for boundary cases. Takes a sequence
/// of digits `ds` and returns true if they're all equal or if, for any element
/// d of ds, d <=> d[0] is equal to `trueResult`, or false if any has the
/// opposite
// relationship.
static bool compareWithFirst(const span<int64_t> ds,
                             const strong_ordering trueResult) {
  assert(!ds.empty() && "expected at least one element");
  int64_t highest = ds.front();
  for (int64_t d : ds.subspan(1, std::dynamic_extent)) {
    strong_ordering cmp = d <=> highest;
    if (cmp == trueResult) {
      return true;
    } else if (cmp == strong_ordering::equal) {
      continue;
    } else {
      return false;
    }
  }
  return true;
}

static bool atOrBelowAllEqual(const span<int64_t> ds) {
  return compareWithFirst(ds, strong_ordering::less);
}
static bool atOrAboveAllEqual(const span<int64_t> ds) {
  return compareWithFirst(ds, strong_ordering::greater);
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

  // Find all the numbers in the range that are of the form [num][num]...[num]
  // for some smaller number [num], where [num] occurs `numParts` times, and
  // inserts them into `seen`. Requires that the low and high endpoints have the
  // same number of digits.
  void collectRepeatedParts(int64_t numParts, intSet &seen) const;

  // Like `collectRepeatedParts` above, but iterates over all possible numbers
  // of times that [num] could repeat.
  void collectRepeatedParts(intSet &seen) const;
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

void Range::collectRepeatedParts(int64_t numParts, intSet &seen) const {
  assert(hasSameDigitLen() && "range should've been split");
  int64_t digits = nDigits(lo);
  if (digits % numParts != 0) {
    return;
  }
  vector<int64_t> loParts = getDigitParts(lo, numParts);
  vector<int64_t> hiParts = getDigitParts(hi, numParts);
  int64_t split = digits / numParts;
  int64_t combiner = powi(10, split);
  auto collectRepeat = [&](int64_t v) {
    int64_t acc = v;
    for (int64_t i = 1; i < numParts; ++i) {
      acc = acc * combiner + v;
    }
    seen.insert(acc);
  };

  int64_t loHead = loParts.front(), hiHead = hiParts.front();
  // Special case for when the front parts are equal to avoid false entries.
  if (loHead == hiHead) {
    if (atOrBelowAllEqual(loParts) && atOrAboveAllEqual(hiParts)) {
      collectRepeat(loHead);
    }
    return;
  }

  if (atOrBelowAllEqual(loParts)) {
    collectRepeat(loHead);
  }
  if (atOrAboveAllEqual(hiParts)) {
    collectRepeat(hiHead);
  }
  for (int64_t v = loHead + 1; v < hiHead; ++v) {
    collectRepeat(v);
  }
}

void Range::collectRepeatedParts(intSet &seen) const {
  assert(hasSameDigitLen() && "range should've been split");
  int64_t digits = nDigits(lo);

  // Number of divisors of the number of digits is small enough to
  // brute-force.
  for (int64_t numParts = digits; numParts > 1; --numParts) {
    if (digits % numParts != 0) {
      continue;
    }
    collectRepeatedParts(numParts, seen);
  }
}

static int64_t sumSet(const intSet &set) {
  return std::accumulate(set.begin(), set.end(), 0ll, std::plus<int64_t>());
}

static std::pair<int64_t, int64_t> solve(const span<Range> input) {
  vector<Range> splitRanges;
  for (const Range &r : input) {
    r.sameDigitLenSplit(splitRanges);
  }

  intSet partASeen;
  intSet partBSeen;

  for (const Range &r : splitRanges) {
    r.collectRepeatedParts(2, partASeen);
    r.collectRepeatedParts(partBSeen);
  }

  return {sumSet(partASeen), sumSet(partBSeen)};
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
  auto [partA, partB] = solve(input);
  std::cout << "Part A: " << partA << "\n" << "Part B: " << partB << "\n";
  return 0;
}
