#include <algorithm>
#include <array>
#include <cstdint>
#include <ios>
#include <iostream>
#include <numeric>
#include <ranges>
#include <stack>
#include <string>
#include <sys/types.h>
#include <vector>

std::string s;
size_t clen, rlen;
std::vector<char> mark;
std::size_t acc_silver, acc_gold;
std::vector<std::tuple<ssize_t, ssize_t>> stk;

const std::array<const std::tuple<ssize_t, ssize_t>, 4> off_edge_neigh
    = { { { 0, -1 }, { 0, 1 }, { -1, 0 }, { 1, 0 } } };

const std::array<const std::tuple<ssize_t, ssize_t>, 4> off_corner_neigh
    = { { { -1, -1 }, { -1, 1 }, { -1, 1 }, { 1, 1 } } };

bool cell_exists(ssize_t r, ssize_t c)
{
  auto row = r >= 0 && r < (ssize_t)rlen;
  auto col = c >= 0 && c < (ssize_t)clen;
  return row && col;
}

std::array<bool, 4> get_edge_ok(ssize_t r, ssize_t c)
{
  return { { c >= 0, c < (ssize_t)clen, r >= 0, r < (ssize_t)rlen } };
}

struct LookAroundResult {
  size_t sides;
  size_t peri;
};

[[nodiscard]] LookAroundResult look_around(ssize_t ur, ssize_t uc, char c)
{
  size_t peri = 0, sides = 0;
  for (size_t i = 0; i < 4; i++) {
    auto [r_off, c_off] = off_edge_neigh[i];
    ssize_t vr = ur + r_off, vc = uc + c_off;
    std::array<bool, 4> vok = get_edge_ok(vr, vc);
    auto idx = vr * clen + vc;
    if (vok[i] && mark[idx] == c) // cell already seen
      continue;
    if (vok[i] && s[vr * (clen + 1) + vc] == c) {
      mark[idx] = c;
      stk.emplace_back(vr, vc);
    } else {
      peri++;
    }
  }
  return { .sides = sides, .peri = peri };
}

int main()
{
  std::istreambuf_iterator<char> begin(std::cin), end;
  s = std::string(begin, end);
  clen = std::find(s.begin(), s.end(), '\n') - s.begin();
  rlen = std::ranges::count(s | std::views::drop(clen) | std::views::stride(clen + 1),
                            '\n');

  std::cout << rlen << std::endl;
  std::cout << clen << std::endl;
  mark.resize(clen * rlen, 0);
  std::cout << " " << "\t" << "area" << "\t" << "peri" << "\t" << "sides" << '\n';

  // flood-fill
  for (size_t row = 0; row < rlen; row++) {
    for (size_t col = 0; col < clen; col++) {
      if (mark[row * clen + col] == 0) { // fresh meat
        const char c = s[row * (clen + 1) + col];
        stk.clear();
        stk.emplace_back(row, col);
        mark[row * clen + col] = c;
        size_t area = 0, peri = 0, corners = 4;
        while (!stk.empty()) {
          const auto [ur, uc] = stk.back();
          // std::cout << "(" << ur << ", " << uc << ")" << "\n";
          stk.pop_back();
          area++;
          auto res = look_around(ur, uc, c);
          peri += res.peri;
          corners += res.sides;
        }
        std::cout << c << "\t" << area << "\t" << peri << "\t" << corners << '\n';
        acc_silver += peri * area;
        acc_gold += corners * area;
      }
    }
  }
  std::cout << acc_silver << '\n' << acc_gold << std::endl;
}
