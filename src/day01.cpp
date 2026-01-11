// AOC day 1

#include "utils.h"

#include <cstdint>
#include <iostream>
#include <ranges>
#include <span> // Yay C++ finally getting LLVM's ArrayRef.

enum class Dir : uint8_t {
    L = 0,
    R = 1
};

struct Entry {
    Dir dir;
    int32_t turns;

    static optional<Entry> parse(string_view line);
};

std::optional<Entry> Entry::parse(string_view line) {
  Entry ret;
  switch (line.front()) {
    case 'L':
    ret.dir = Dir::L;
    break;
    case 'R':
    ret.dir = Dir::R;
    break;
    default:
    return std::nullopt;
  }
  optional<int32_t> turns = parseDecimalInt<int32_t>(line.substr(1));
  if (!turns) {
    return std::nullopt;
  }
  ret.turns = *turns;
  return ret;
}

vector<Entry> parse(string_view input) {
  vector<Entry> ret;
  // Note: turns out C++ ranges abuse | to let things look like Unix pipes,
  // which is cool.
  for (auto [lineNo, text] : std::views::split(input, "\n"sv) | std::views::enumerate) {
    if (text.empty()) {
        continue;
    }
    auto maybeParsed = Entry::parse(string_view(text));
    if (!maybeParsed) {
        std::cerr << "Malformed input on linne " << lineNo + 1 << "\n";
        std::exit(1);
    }
    ret.push_back(*maybeParsed);
  }
  return ret;
}

int32_t solveA(const std::span<Entry> data) {
  int32_t state = 50;
  int32_t ret = 0;
  constexpr int32_t NUM_POS = 100;
  for (const Entry& e : data) {
    if (e.dir == Dir::L) {
        state -= e.turns;
    } else if (e.dir == Dir::R) {
        state += e.turns;
    }

    // The usual trick for mathematical mod.
    state = ((state % NUM_POS) + NUM_POS) % NUM_POS;
    if (state == 0) {
        ret += 1;
    }
}
return ret;
}

int main(int argc, char** argv) {
    std::string input = getInput(argc, argv);
    vector<Entry> parsed = parse(input);
    int32_t partA = solveA(parsed);
    std::cout << "Part A: " << partA << "\n";
    return 0;
}
