#include <bits/stdc++.h>

int main() {
  std::istreambuf_iterator<char> begin(std::cin), end;
  std::string s(begin, end);
  const size_t clen = std::find(s.begin(), s.end(), '\n') - s.begin();
  const size_t rlen = std::count(s.begin(), s.end(), '\n');

  std::vector<uint8_t> mark(clen * rlen, 0);

  enum Directions : std::uint_fast8_t { UP, RIGHT, DOWN, LEFT };
  std::array<std::pair<ssize_t, ssize_t>, 4> dirs = {
      {{-1, 0}, {0, 1}, {1, 0}, {0, -1}}};

  auto loc = s.find("^");
  ssize_t grow = loc / (clen + 1), gcol = loc % (clen + 1);

  auto simulate = [&] {
    auto OK = [=](ssize_t row, ssize_t col) {
      return col >= 0 && col < static_cast<decltype(col)>(clen) && //
             row >= 0 && row < static_cast<decltype(row)>(rlen);
    };

    std::fill(mark.begin(), mark.end(), 0);
    auto dir = UP;
    auto row = grow, col = gcol;
    mark[row * clen + col] = (1 << dir);
    while (OK(row, col)) {
      auto &offset = dirs[dir];
      auto newrow = row + offset.first, newcol = col + offset.second;
      if (!OK(newrow, newcol))
        break;
      if (mark[newrow * clen + newcol] & (1 << dir))
        return 1;
      if (s[newrow * (clen + 1) + newcol] == '#') {
        dir = static_cast<Directions>(
            static_cast<std::uint_fast8_t>((dir) + 1u) % 4u);
        continue;
      }
      row = newrow, col = newcol;
      mark[row * clen + col] |= (1 << dir);
    }
    return 0;
  };

  assert(simulate() == 0);
  const auto acc_silver = std::ranges::fold_left(
      mark | std::views::transform([](auto x) -> size_t { return !!x; }), 0,
      std::plus<size_t>());
  size_t acc_gold = 0;
  for (size_t r : std::views::iota(0u, rlen)) {
    for (size_t c : std::views::iota(0u, clen)) {
      auto &cell = s[r * (clen + 1) + c];
      if (cell != '.')
        continue;
      cell = '#';
      acc_gold += simulate();
      cell = '.';
    }
  }
  std::cout << acc_silver << '\n' << acc_gold << std::endl;
}
