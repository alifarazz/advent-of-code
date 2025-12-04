// hope you have fmt installed: https://github.com/fmtlib/fmt/
// g++ -Wall -std=c++23 -Ofast -march=native d.cc -o d.elf -lfmt
// ./d.elf <input
#include <array>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <span>
#include <vector>
#include <string>

#include <fmt/format.h>
#include <fmt/ranges.h>

std::array<std::pair<size_t, size_t>, 8> offsets{
    {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}}};

size_t fast_pass(std::span<char> buf, std::span<char> buf2,
                 std::pair<size_t, size_t> rc) {
  size_t ok = 0;
  for (size_t j = 1; j < rc.second - 1; j++) { // first row [1:-1]
    if (buf[j] == '@' && ((buf[j - 1            ] == '@') + 
                          (buf[j + 1            ] == '@') +
                          (buf[j - 1 + rc.second] == '@') + 
                          (buf[j     + rc.second] == '@') +
                          (buf[j + 1 + rc.second] == '@')) < 4) {
      buf2[j] = 'x';
      ok++;
    }
  }
  for (size_t i = 1; i < rc.first - 1; i++) {
    if (buf[i * rc.second] == '@' && ((buf[(i - 1) * rc.second    ] == '@') +
                                      (buf[(i - 1) * rc.second + 1] == '@') +
                                      (buf[(i    ) * rc.second + 1] == '@') +
                                      (buf[(i + 1) * rc.second    ] == '@') +
                                      (buf[(i + 1) * rc.second + 1] == '@')) < 4) { // first col of the row
        buf2[i * rc.second] = 'x';
        ok++;
    }
    for (size_t j = 1; j < rc.second - 1; j++) {
      if (buf[i * rc.second + j] != '@')
        continue;
      int c{};
      for (auto [di, dj] : offsets) {
        c += (buf[(i + di) * rc.second + (j + dj)] == '@');
        if (c == 4) {
          break;
        }
      }
      if (c < 4) {
        buf2[i * rc.second + j] = 'x';
        ok++;
      }
    }
    if (buf[(i + 1) * rc.second - 1] == '@' && ((buf[(i    ) * rc.second - 2] == '@') +
                                                (buf[(i    ) * rc.second - 1] == '@') +
                                                (buf[(i + 1) * rc.second - 2] == '@') +
                                                (buf[(i + 2) * rc.second - 2] == '@') +
                                                (buf[(i + 2) * rc.second - 1] == '@')) < 4) {
      // last col of the row
      buf2[(i + 1) * rc.second - 1] = 'x';
      ok++;
    }
  }
  for (size_t j = 1; j < rc.second - 1; j++) { // last row [1:-1]
    if (buf[(rc.first - 1) * rc.second + j] == '@' &&
        ((buf[j - 1 + (rc.first - 1) * rc.second] == '@') +
         (buf[j + 1 + (rc.first - 1) * rc.second] == '@') +
         (buf[j - 1 + (rc.first - 2) * rc.second] == '@') +
         (buf[j     + (rc.first - 2) * rc.second] == '@') +
         (buf[j + 1 + (rc.first - 2) * rc.second] == '@')) < 4) {
      buf2[(rc.first - 1) * rc.second + j] = 'x';
      ok++;
    }
  }
  
  // four corners
  for (auto i : {0zu, rc.second - 1, (rc.first - 1) * rc.second,
                 rc.first * rc.second - 1}) {
    if (buf[i] == '@') {
      buf2[i] = 'x';
      ok++;
    }
  }
  return ok;
}

int main() {
  std::ios::sync_with_stdio(false);

  std::string buffer((std::istreambuf_iterator<char>(std::cin)),
                     (std::istreambuf_iterator<char>()));

  auto col = buffer.find('\n');
  auto row = buffer.size() / col - 1;

  std::vector<char> buf;
  buf.reserve(row * col);
  for (size_t i = 0; i < row; i++) {
    for (size_t j = 0; j < col; j++) {
      buf.push_back(buffer[i * (col + 1) + j]);
    }
  }

  auto buf2 = buf;
  auto silver = fast_pass(buf, buf2, {row, col});
  fmt::println("{}", silver);

  size_t removed, gold = silver;
  do {
    buf = buf2;
    removed = fast_pass(buf, buf2, {row, col});
    gold += removed;
  } while (removed);

  fmt::println("{}", gold);
}
