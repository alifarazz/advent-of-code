#include <array>
#include <bits/stdc++.h>
#include <cinttypes>
#include <cstdint>
#include <cstdio>

int main() {
  std::array<std::vector<std::uint8_t>, 100> rules;

  std::array<char, 7> rule_line;
  while (fgets(rule_line.data(), rule_line.size(), stdin)) {
    if (rule_line[0] == '\n')
      break;
    std::uint8_t a = (rule_line[0] - '0') * 10 + rule_line[1] - '0';
    std::uint8_t b = (rule_line[3] - '0') * 10 + rule_line[4] - '0';
    rules[a].push_back(b);
  }

  for (auto i : {29,47,53,61,75,97}) {
    std::cout << i << ": ";
    for (auto v: rules[i])
      std::cout << (int)v << ' ';
    std::cout << std::endl;    
  }

  std::uint32_t acc_silver = 0;
  std::vector<std::uint8_t> pages;
  while (fgets(rule_line.data(), 4, stdin)) {
    if (rule_line[0] == '\n')
      break;
    std::uint8_t v = (rule_line[0] - '0') * 10 + rule_line[1] - '0';
    pages.push_back(v);
    if (rule_line[2] == '\n') {
      // calculate if its valid
      for (auto v: pages) {
        std::cout << (int)v << " ";
      }
      std::cout << std::endl;
      
      bool ok = true;
      for (size_t i = 0; i < pages.size() && ok; i++) {
        for (size_t j = i + 1; j < pages.size() && ok; j++) {
          auto u = pages[i], v = pages[j];
          if (std::find(rules[v].begin(), rules[v].end(), u) != rules[v].end())
            ok = false;
        }
      }
      acc_silver += ok ? pages[pages.size() / 2] : 0;
      pages.clear();
    }
  }
  std::printf("%" PRIu32 "\n", acc_silver);
}
