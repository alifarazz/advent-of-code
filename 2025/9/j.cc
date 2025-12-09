// hope you have fmt installed: https://github.com/fmtlib/fmt/
// g++ -Wall -std=c++23 -Ofast -march=native j.cc -o j.elf -lfmt
// ./j.elf <input
#include <vector>
#include <string>
#include <iostream>
#include <charconv>
#include <algorithm>

#include <fmt/base.h>

int main() {
  std::ios::sync_with_stdio(false);
  std::string input((std::istreambuf_iterator<char>(std::cin)),
                    std::istreambuf_iterator<char>());

  const auto input_end = input.data() + input.size();
  std::vector<std::pair<long, long>> vs;
  vs.reserve(std::count(input.begin(), input.end(), '\n'));
  for (const char *cur = input.data(); cur < input_end;) {
    long x{}, y{};
    cur = std::from_chars(cur, input_end, x).ptr + 1;
    cur = std::from_chars(cur, input_end, y).ptr + 1;
    vs.push_back({x, y});
  }
  long silver = -1, gold = -1;
  for (size_t i = 0; i < vs.size() - 1; i++) {
    for (size_t j = i + 1; j < vs.size(); j++) {
      auto [Xnw, Xse] = std::minmax(vs[i].first, vs[j].first);
      auto [Ynw, Yse] = std::minmax(vs[i].second, vs[j].second);
      auto area = (Yse - Ynw + 1) * (Xse - Xnw + 1);
      silver = std::max(silver, area);
      bool has_edge_inside = false;
      for (size_t k = 0; k < vs.size(); k++) {
        auto &a = vs[k];
        auto &b = vs[(k == vs.size() - 1 ? 0 : k + 1)];
        auto [xnw, xse] = std::minmax(a.first, b.first);
        auto [ynw, yse] = std::minmax(a.second, b.second);
        if (Xnw < xse && 
                  xnw < Xse &&
            Ynw < yse && 
                  ynw < Yse) {
          has_edge_inside = true;
          break;
        }
      }
      if (!has_edge_inside)
        gold = std::max(gold, area);
    }
  }
  fmt::println("{}", silver);
  fmt::println("{}", gold);
}
