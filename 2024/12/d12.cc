#include <algorithm>
#include <array>
#include <iostream>
#include <ranges>
#include <set>
#include <string>
#include <vector>

std::string s;
size_t clen, rlen;
std::vector<size_t> mark;
std::size_t acc_silver, acc_gold;
std::vector<std::tuple<ssize_t, ssize_t>> stk;

const std::array<const std::tuple<ssize_t, ssize_t>, 4> off_edge_neigh
    = { { { 0, -1 }, { 0, 1 }, { -1, 0 }, { 1, 0 } } };

const std::array<const std::tuple<ssize_t, ssize_t>, 4> off_corner_neigh
    = { { { -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 1 } } };

const std::array<const std::tuple<ssize_t, ssize_t>, 4> off_corner
    = { { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } } };

size_t s_at(size_t row, size_t col) { return row * (clen + 1) + col; }

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

[[nodiscard]] size_t look_around_gold(ssize_t ur, ssize_t uc, char c,
                                      std::set<size_t>& corners_set)
{
  size_t sides = 0;
  for (size_t i = 0; i < 4; i++) {
    auto [off_r, off_c] = off_corner_neigh[i];
    auto hrz_ok = cell_exists(ur, uc + off_c);
    auto vrt_ok = cell_exists(ur + off_r, uc);
    auto crn_ok = cell_exists(ur + off_r, uc + off_c);
    auto* hrz_cntp = s.data() + (ur) * (clen + 1) + uc + off_c;
    auto* vrt_cntp = s.data() + (ur + off_r) * (clen + 1) + uc;
    auto* crn_cntp = s.data() + (ur + off_r) * (clen + 1) + uc + off_c;

    if ((hrz_ok & vrt_ok & crn_ok) && *vrt_cntp == c && *hrz_cntp == c
        && *crn_cntp == c) // All 4 are the same
      continue;
    if (hrz_ok && *hrz_cntp == c
        && (!(vrt_ok | crn_ok)
            || ((vrt_ok & crn_ok) && *vrt_cntp != c && c != *crn_cntp))) // horiz line
      continue;
    if (vrt_ok && *vrt_cntp == c
        && (!(hrz_ok | crn_ok)
            || ((hrz_ok & crn_ok) && *hrz_cntp != c && c != *crn_cntp))) // verti line
      continue;

    // let's insert corner so that we never count it again
    auto [off_corner_r, off_corner_c] = off_corner[i];
    size_t corner_idx = (ur + off_corner_r) * (clen + 1) + uc + off_corner_c;
    if (corners_set.contains(corner_idx)) { // corner seen before?
      if ((hrz_ok & vrt_ok & crn_ok) && *vrt_cntp != c && *hrz_cntp != c
          && *crn_cntp == c) { // handle Mobius
        sides++; // corner already seen, but count it agian cuz Mobius
        continue;
      }
      continue; // don't count the seen corner again
    }
    corners_set.insert(corner_idx);
    sides++;
  }
  return sides;
}

[[nodiscard]] size_t look_around_silver(ssize_t ur, ssize_t uc, char c, size_t conuter)
{
  size_t peri = 0;
  for (size_t i = 0; i < 4; i++) {
    auto [r_off, c_off] = off_edge_neigh[i];
    ssize_t vr = ur + r_off, vc = uc + c_off;
    std::array<bool, 4> vok = get_edge_ok(vr, vc);
    auto idx = vr * clen + vc;
    if (vok[i] && mark[idx] == conuter) // cell already seen
      continue;
    if (vok[i] && s[vr * (clen + 1) + vc] == c) {
      mark[idx] = conuter;
      stk.emplace_back(vr, vc);
    } else {
      peri++;
    }
  }
  return peri;
}

int main()
{
  std::istreambuf_iterator<char> begin(std::cin), end;
  s = std::string(begin, end);
  clen = std::find(s.begin(), s.end(), '\n') - s.begin();
  rlen = std::ranges::count(s | std::views::drop(clen) | std::views::stride(clen + 1),
                            '\n');

  mark.resize(clen * rlen, 0);

  // flood-fill
  size_t counter = 0;
  for (size_t row = 0; row < rlen; row++) {
    for (size_t col = 0; col < clen; col++) {
      auto idx = row * clen + col;
      if (mark[idx] == 0) { // fresh cell/vertex
        const char c = s[row * (clen + 1) + col];
        stk.clear();
        stk.emplace_back(row, col);
        mark[idx] = ++counter;
        std::set<size_t> corners_set;
        size_t area = 0, peri = 0, corners = 0;
        while (!stk.empty()) {
          const auto [ur, uc] = stk.back();
          stk.pop_back();
          area++;
          peri += look_around_silver(ur, uc, c, counter);
          corners += look_around_gold(ur, uc, c, corners_set);
        }
        acc_silver += peri * area;
        acc_gold += corners * area;
      }
    }
  }
  std::cout << acc_silver << '\n' << acc_gold << std::endl;
}
