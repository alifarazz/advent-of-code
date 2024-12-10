#include <algorithm>
#include <cstdint>
#include <iostream>
#include <ranges>
#include <string>
#include <sys/types.h>
#include <vector>

size_t clen, rlen;
std::string s;
std::vector<int> mark;
std::vector<size_t> dirty_mark_locs;
size_t acc_silver;

std::array<std::tuple<ssize_t, ssize_t>, 4> offsets {
  { { 0, -1 }, { 0, 1 }, { -1, 0 }, { 1, 0 } }
};
char counter = '1';

bool is_inside(ssize_t row, ssize_t col)
{
  return 0 <= row && 0 <= col && row < rlen && col < clen;
}

void dfs_silver(ssize_t row, ssize_t col)
{
  if (s[row * (clen + 1) + col] == '9') {
    acc_silver++;
    return;
  }
  for (auto offset : offsets) {
    auto [offrow, offcol] = offset;
    auto nextrow = offrow + row, nextcol = offcol + col;
    if (is_inside(nextrow, nextcol) && s[nextrow * (clen + 1) + nextcol] == counter
        && !mark[nextrow * (clen) + nextcol]) {
      counter++;
      mark[nextrow * (clen) + nextcol] = 1;
      dirty_mark_locs.push_back(nextrow * (clen) + nextcol);
      dfs_silver(nextrow, nextcol);
      counter--;
    }
  }
}

std::size_t dfs_gold(ssize_t row, ssize_t col)
{
  if (s[row * (clen + 1) + col] == '9') {
    return 1;
  }
  std::size_t acc = 0;
  for (auto offset : offsets) {
    auto [offrow, offcol] = offset;
    auto nextrow = offrow + row, nextcol = offcol + col;
    if (is_inside(nextrow, nextcol) && s[nextrow * (clen + 1) + nextcol] == counter) {
      counter++;
      acc += dfs_gold(nextrow, nextcol);
      counter--;
    }
  }
  return acc;
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

  std::size_t acc_gold = 0;
  for (size_t i = 0; i < rlen; i++) {
    for (size_t j = 0; j < clen; j++) {
      if (s[i * (clen + 1) + j] != '0')
        continue;
      dfs_silver(i, j);
      for (auto loc : dirty_mark_locs)
        mark[loc] = 0;
      dirty_mark_locs.clear();
      acc_gold += dfs_gold(i, j);
    }
  }
  std::cout << acc_silver << '\n' << acc_gold << std::endl;
}
