// hope you have fmt installed: https://github.com/fmtlib/fmt/
// g++ -Wall -std=c++23 -Ofast -march=native i.cc -o i.elf -lfmt
// ./i.elf <input
#include <algorithm>
#include <charconv>
#include <ios>
#include <iostream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include <fmt/format.h>

using utype = uint;
using Vertex = std::tuple<long, long, long>;
using Edge = std::pair<long, std::pair<utype, utype>>;
std::vector<Vertex> vs;
std::vector<Edge> es;

std::vector<utype> dsprnt;
std::vector<utype> dsrank;
std::vector<utype> dssize;

utype ds_find(utype u) {
  if (dsprnt[u] == u)
    return u;
  return dsprnt[u] = ds_find(dsprnt[u]);
}

void ds_union(utype u, utype v) {
  utype uu = ds_find(u);
  utype vv = ds_find(v);
  if (dsrank[uu] == dsrank[vv]) {
    ++dsrank[uu];
    dsprnt[uu] = vv;
  } else if (dsrank[uu] < dsrank[vv])
    dsprnt[vv] = uu;
  else // same height, select one at random
    dsprnt[uu] = vv;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::string input((std::istreambuf_iterator<char>(std::cin)),
                    std::istreambuf_iterator<char>());

  auto input_last = input.data() + input.size();
  for (const char *cur = input.data(); cur < input_last;) {
    uint x{}, y{}, z{};
    cur = std::from_chars(cur, input_last, x).ptr + 1;
    cur = std::from_chars(cur, input_last, y).ptr + 1;
    cur = std::from_chars(cur, input_last, z).ptr + 1;
    vs.emplace_back(x, y, z);
  }
  es.reserve(vs.size() * (vs.size() - 1) / 2);
  for (size_t i = 0; i < vs.size(); i++) { // make edges
    auto [x, y, z] = vs[i];
    for (size_t j = i + 1; j < vs.size(); j++) {
      auto [a, b, c] = vs[j];
      a -= x;
      b -= y;
      c -= z;
      auto ij = std::make_pair<utype, utype>(i, j);
      es.emplace_back(a * a + b * b + c * c, ij);
    }
  }
  dsprnt.resize(vs.size());
  dsrank.resize(vs.size());
  dssize.resize(vs.size());
  for (size_t i = 0; i < vs.size(); i++)
    dsprnt[i] = i, dssize[i] = 1;

  std::sort(es.begin(), es.end());

  size_t mst_size = 0;
  size_t upper = 1000;
  for (size_t i = 0; i < upper; i++) {
    auto [d, uv] = es[i];
    auto [u, v] = uv;
    if (auto fu = ds_find(u), fv = ds_find(v); fu != fv) {
      ds_union(u, v);
      mst_size++;
      bool u_chosen = ds_find(u) == fu;
      auto &dst = u_chosen ? dssize[fu] : dssize[fv];
      auto &src = u_chosen ? dssize[fv] : dssize[fu];
      dst += src;
      src = 0;
    }
  }
  std::sort(dssize.begin(), dssize.end(), std::greater<utype>());
  fmt::println("{}", dssize[0] * dssize[1] * dssize[2]);

  utype u{}, v{};
  for (size_t i = upper; mst_size < vs.size() - 1; i++) {
    auto [d, uv] = es[i];
    std::tie(u, v) = uv;
    if (auto fu = ds_find(u), fv = ds_find(v); fu != fv) {
      ds_union(u, v);
      mst_size++;
    }
  }
  fmt::println("{}", std::get<0>(vs[u]) * std::get<0>(vs[v]));
}
