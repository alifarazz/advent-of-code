#include <bits/stdc++.h>

template <int offset> std::uint8_t tonum(const char *const s) {
  return (std::uint8_t)(s[offset + 0] - '0') * 10 +
         (std::uint8_t)(s[offset + 1] - '0');
}

int main() {
  std::array<std::vector<std::uint8_t>, 100> rules;

  std::array<char, 7> rule_line;
  while (fgets(rule_line.data(), rule_line.size(), stdin)) {
    if (rule_line[0] == '\n')
      break;
    std::uint8_t a = tonum<0>(rule_line.data());
    std::uint8_t b = tonum<3>(rule_line.data());
    rules[a].push_back(b);
  }

  std::uint_fast32_t acc_silver = 0, acc_gold = 0;
  std::vector<std::uint8_t> pages;
  while (fgets(rule_line.data(), 4, stdin)) {
    if (rule_line[0] == '\n')
      break;

    pages.push_back(tonum<0>(rule_line.data()));

    if (rule_line[2] == '\n') { // calculate if its valid
      bool ok = false;
      for (size_t i = 0; i < pages.size(); i++) {
        for (size_t j = i + 1; j < pages.size(); j++) {
          auto &u = pages[i], &v = pages[j];
          if (std::find(rules[v].begin(), rules[v].end(), u) !=
              rules[v].end()) {
            ok = true;
            std::swap(u, v);
          }
        }
      }
      acc_silver += !ok ? pages[pages.size() / 2] : 0;
      acc_gold   +=  ok ? pages[pages.size() / 2] : 0;
      pages.clear();
    }
  }
  std::printf("%" PRIuFAST32 "\n"
              "%" PRIuFAST32 "\n",
              acc_silver, acc_gold);
}
