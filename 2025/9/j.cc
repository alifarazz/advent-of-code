// hope you have fmt installed: https://github.com/fmtlib/fmt/
// g++ -Wall -std=c++23 -Ofast -march=native i.cc -o i.elf -lfmt
// ./i.elf <input
#include <algorithm>
#include <charconv>
#include <fmt/base.h>
#include <functional>
#include <ios>
#include <iostream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include <fmt/format.h>
#include <fmt/ranges.h>

int main() {
  std::ios::sync_with_stdio(false);
  std::string input((std::istreambuf_iterator<char>(std::cin)),
                    std::istreambuf_iterator<char>());

  std::vector<std::pair<long, long>> vs;
  for (const char *cur = input.data(); cur < input.data() + input.size();) {
    long x, y;
    cur = std::from_chars(cur, input.data() + input.size(), x).ptr + 1;
    cur = std::from_chars(cur, input.data() + input.size(), y).ptr + 1;
    vs.push_back({x, y});
  }
  long largest = -1;
  for (size_t i = 0; i < vs.size(); i++) {
    for (size_t j = i + 1; j < vs.size(); j++) {
      auto x = std::abs(vs[i].first - vs[j].first) + 1;
      auto y = std::abs(vs[i].second - vs[j].second) + 1;
      largest = std::max(largest, x * y);
    }
  }
  fmt::println("{}", vs);
  fmt::println("{}", largest);
}
