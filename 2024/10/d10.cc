#include <algorithm>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

using size_t = std::size_t;

size_t clen, rlen;
std::string s;
std::vector<int> mark;
std::vector<size_t> dirty_mark_locs;
size_t acc_silver;
size_t acc_gold;

std::array<std::tuple<ssize_t, ssize_t>, 4> offsets {
  { { 0, -1 }, { 0, 1 }, { -1, 0 }, { 1, 0 } }
};
char counter = '1';

bool is_inside(ssize_t row, ssize_t col)
{
  return 0 <= row && 0 <= col && row < rlen && col < clen;
}

size_t tosidx(size_t row, size_t col) { return row * (clen + 1) + col; }

template <bool is_silver> void dfs(ssize_t row, ssize_t col)
{
  if (s[tosidx(row, col)] == '9') {
    if constexpr (is_silver)
      acc_silver++;
    else
      acc_gold++;
    return;
  }
  for (auto offset : offsets) {
    auto [offrow, offcol] = offset;
    auto nextrow = offrow + row, nextcol = offcol + col;
    if (is_inside(nextrow, nextcol) && s[tosidx(nextrow, nextcol)] == counter
        && (!is_silver || !mark[nextrow * (clen) + nextcol])) {
      counter++;
      if constexpr (is_silver) {
        mark[nextrow * (clen) + nextcol] = 1;
        dirty_mark_locs.push_back(nextrow * (clen) + nextcol);
      }
      dfs<is_silver>(nextrow, nextcol);
      counter--;
    }
  }
}

int main()
{
  std::istreambuf_iterator<char> begin(std::cin), end;
  s = std::string(begin, end);
  clen = std::find(s.begin(), s.end(), '\n') - s.begin();
  rlen = std::ranges::count(s | std::views::drop(clen) | std::views::stride(clen + 1),
                            '\n');

  mark.resize(clen * rlen);
  std::ranges::fill(mark, 0);
  dirty_mark_locs.reserve(256); // hopfully it's enough

  for (size_t i = 0; i < rlen; i++) {
    for (size_t j = 0; j < clen; j++) {
      if (s[tosidx(i, j)] != '0')
        continue;
      dfs<true>(i, j);
      dfs<false>(i, j);
      for (auto loc : dirty_mark_locs)
        mark[loc] = 0;
      dirty_mark_locs.clear();
    }
  }
  std::cout << acc_silver << '\n' << acc_gold << std::endl;
}
