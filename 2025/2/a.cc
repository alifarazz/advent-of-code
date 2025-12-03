// hope you have fmt installed: https://github.com/fmtlib/fmt/
// g++ -Ofast -march=native -std=c++17 a.cc -o a.elf -lfmt
// ./a.elf <input
#include <array>
#include <charconv>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <string>
#include <string_view>

#include <fmt/format.h>

int main() {
  std::ios::sync_with_stdio(false);

  std::string buffer((std::istreambuf_iterator<char>(std::cin)),
                     (std::istreambuf_iterator<char>()));

  size_t gold = 0, silver = 0;
  const char *cur = buffer.data();
  const char *bufend = buffer.data() + buffer.size();
  while (cur < bufend) {
    size_t a = 0, b = 0;
    cur = std::from_chars(cur, bufend, a).ptr + 1;
    cur = std::from_chars(cur, bufend, b).ptr + 1;

    for (size_t num = a; num <= b; num++) {
      std::array<char, 21> buf;
      auto res = fmt::format_to_n(buf.begin(), buf.size(), "{}", num);
      if (res.size % 2 == 0) {
        size_t mid = res.size / 2;
        std::string_view left(buf.data(), mid);
        std::string_view right(buf.data() + (res.size - mid), mid);
        silver += num * (left == right);
      }

      for (size_t f = 1; f <= res.size / 2; f++) {
        if (res.size % f == 0) {
          bool ok = true;
          for (size_t j = 0; j < res.size - f; j += f) {
            std::string_view l(buf.data() + j, f);
            std::string_view r(buf.data() + j + f, f);
            if (l != r) {
              ok = false;
              break;
            }
          }
          if (ok) {
            gold += num;
            break;
          }
        }
      }
    }
  }

  fmt::println("{}\n{}", silver, gold);
}
