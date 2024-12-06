#include <bits/stdc++.h>
#include <cstdint>

int main() {
  std::istreambuf_iterator<char> begin(std::cin), end;
  std::string s(begin, end);
  const size_t clen = std::find(s.begin(), s.end(), '\n') - s.begin();
  const size_t rlen = std::count(s.begin(), s.end(), '\n');

  std::vector<uint8_t> mark(clen * rlen, 0);

  enum Directions : int { UP, RIGHT, DOWN, LEFT };
  std::array<std::pair<ssize_t, ssize_t>, 4> dirs = {
      {{-1, 0}, {0, 1}, {1, 0}, {0, -1}}};

  auto loc = s.find("^");
  auto OK = [=](ssize_t row, ssize_t col) {
    return col >= 0 && col < clen && row < rlen && row >= 0;
  };

  size_t acc_gold = 0;
  for (size_t i = 0; i < clen * rlen; i++) {
    auto r = i / (clen), c = i % (clen);
    auto &cell = s[r * (clen + 1) + c];
    if (cell != '.')
      continue;
    cell = '#';
    ssize_t row = loc / (clen + 1), col = loc % (clen + 1);
    std::fill(mark.begin(), mark.end(), 0);
    auto dir = UP;
    mark[row * clen + col] = (1 << dir);
    while (OK(row, col)) {
      auto &offset = dirs.at(dir);
      auto newrow = row + offset.first, newcol = col + offset.second;
      if (!OK(newrow, newcol))
        break;
      if (mark[newrow * clen + newcol] & (1 << dir)) {
        acc_gold++;
        std::cout << r << '\t' << c << '\n';
        break;
      }
      if (s[newrow * (clen + 1) + newcol] == '#') {
        dir = Directions{((int)dir + 1) % 4};
        continue;
      }
      row = newrow, col = newcol;
      mark[row * clen + col] |= (1 << dir);
    }
    cell = '.';
  }
  std::cout << acc_gold << std::endl;
}
