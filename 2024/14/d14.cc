#include <algorithm>
#include <bitset>
#include <cstdint>
#include <iostream>
#include <unordered_set>
#include <utility>
#include <vector>

int main()
{
  constexpr int32_t ROW = 103, COL = 101;

  std::istreambuf_iterator<char> begin(std::cin), end;
  std::string s = std::string(begin, end);
  const char* sptr = s.data();

  std::array<int64_t, 4> quad = { 0 };
  std::vector<std::pair<int32_t, int32_t>> pos, vel;
  do {
    int64_t pc {}, pr {}, vc {}, vr {}; // just to shut gcc up
    sptr += 2;
    auto res = std::from_chars(sptr, sptr + 4, pc); // fsck scanf
    sptr = res.ptr + 1;
    res = std::from_chars(sptr, sptr + 4, pr);
    sptr = res.ptr + 3;
    res = std::from_chars(sptr, sptr + 5, vc);
    sptr = res.ptr + 1;
    res = std::from_chars(sptr, sptr + 5, vr);
    sptr = res.ptr + 1;
    pos.emplace_back(pc, pr);
    vel.emplace_back(vc, vr);
  } while (sptr < s.data() + s.length());

  for (size_t i = 0; i < pos.size(); i++) {
    auto [pc, pr] = pos[i];
    auto [vc, vr] = vel[i];
    pr = (pr + (ROW + vr) * 100) % ROW;
    pc = (pc + (COL + vc) * 100) % COL;

    if (pr == ROW / 2 || pc == COL / 2)
      continue;
    quad[(pr < ROW / 2 ? 2 : 0) + (pc < COL / 2 ? 1 : 0)]++;
  }
  auto silver = std::ranges::fold_left(quad, 1, [](auto u, auto v) { return u * v; });

  size_t gold = 0;
  std::bitset<COL * ROW> mark;
  do {
    mark.reset();
    for (size_t i = 0; i < pos.size(); i++) {
      auto& [pc, pr] = pos[i];
      auto [vc, vr] = vel[i];
      pr = (pr + ROW + vr) % ROW;
      pc = (pc + COL + vc) % COL;
      mark[pr * COL + pc] = 1;
    }
    gold++;
  } while (mark.count() != pos.size());

  std::cout << silver << '\n' << gold << '\n';
}
