#include <algorithm>
#include <cassert>
#include <iostream>
#include <flat_map>
#include <utility>
#include <vector>

#include <fmt/base.h>

constexpr size_t MAXN = 1'000'001;

using vertex_type = int;

std::flat_map<std::array<char, 4>, vertex_type> NameToV;
std::array<std::vector<vertex_type>, MAXN> vs, revs;
std::vector<long> lbls;
std::vector<std::array<long, 4>> lblg;
vertex_type you, out, svr, dac, fft;

long dfs(vertex_type me) {
  if (lbls[me] != -1)
    return lbls[me];

  long lbl_me{};
  for (auto p : revs[me])
    lbl_me += dfs(p);
  return lbls[me] = lbl_me;
}

enum path_type : int { kPlain = 0, kFft, kDac, kBoth = 3 };

inline path_type& operator|=(path_type& lhs, int rhs) {
    return lhs = static_cast<path_type>(std::to_underlying(lhs) | rhs);
}

long dfsg(vertex_type me, path_type pt) {
  pt |= (me == dac) * kDac;
  pt |= (me == fft) * kFft;

  if (lblg[me][pt] != -1)
    return lblg[me][pt];

  size_t lbl_me = 0;
  for (auto v : vs[me])
    lbl_me += dfsg(v, pt);
  return lblg[me][pt] = lbl_me;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::string input((std::istreambuf_iterator<char>(std::cin)),
                    std::istreambuf_iterator<char>());

  const auto input_end = input.data() + input.size();
  for (const char *cur = input.data(); cur < input_end;) {
    std::array<char, 4> c = {};
    std::copy_n(cur, 3, c.data());
    NameToV.try_emplace(c, (int)NameToV.size());
    vertex_type u = NameToV[c];

    assert(*(cur += 4) == ' ');
    while (*cur != '\n') {
      std::copy_n(++cur, 3, c.data());
      NameToV.try_emplace(c, (int)NameToV.size());
      vertex_type v = NameToV[c];
      vs[u].push_back(v);
      revs[v].push_back(u);
      cur += 3;
    }
    cur++;
  }
  const size_t V = NameToV.size();
  out = NameToV[{"out"}];
  you = NameToV[{"you"}];
  svr = NameToV[{"svr"}];
  dac = NameToV[{"dac"}];
  fft = NameToV[{"fft"}];
  NameToV = {};

  lbls.resize(V);
  std::fill(lbls.begin(), lbls.end(), -1);
  lbls[you] = 1;
  fmt::println("{}", dfs(out));
  lbls = {};

  lblg.resize(V);
  std::fill_n(reinterpret_cast<long *>(lblg.data()), lblg.size() * 4, -1);
  lblg[out] = {{0, 0, 0, 1}};
  fmt::println("{}", dfsg(svr, kPlain));
}
