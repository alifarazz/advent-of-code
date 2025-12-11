// hope you have fmt installed: https://github.com/fmtlib/fmt/
// g++ -Wall -std=c++23 -Ofast -march=native j.cc -o j.elf -lfmt
// ./j.elf <input
#include <algorithm>
#include <cassert>
#include <charconv>
#include <cstdint>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

#include <fmt/base.h>

int main() {
  std::ios::sync_with_stdio(false);
  std::string input((std::istreambuf_iterator<char>(std::cin)),
                    std::istreambuf_iterator<char>());

  std::vector<std::uint32_t> trgts, masks_flat;
  std::vector<size_t> masks_offset;

  const auto input_end = input.data() + input.size();
  for (const char *cur = input.data(); cur < input_end;) {
    uint32_t trgt{};
    assert(*cur == '[');
    while (*++cur != ']')
      ;
    assert(*cur == ']');
    auto end = cur - 1;
    while (*end != '[') {
      trgt <<= 1;
      trgt |= *end-- == '#';
    }
    trgts.push_back(trgt);
    masks_offset.push_back(masks_flat.size());
    assert(*++cur == ' ');
    assert(*++cur == '(');
    while (*cur == '(') {
      uint32_t mask{};
      assert(*cur++ == '(');
      while (*cur != ' ') {
        int bit_idx{};
        cur = std::from_chars(cur, input_end, bit_idx).ptr + 1;
        mask |= (1 << bit_idx);
      }
      masks_flat.push_back(mask);
      cur++;
    }
    while (*++cur != '\n')
      ;
    cur++;
  }
  masks_offset.push_back(masks_flat.size());

  size_t silver{};
  for (size_t i = 0; i < trgts.size(); i++) {
    auto trgt = trgts[i];
    size_t depth{};
    std::queue<uint32_t> q;
    q.push(trgt);
    q.push(~0u);
    auto found = false;
    while (!found) {
      auto front = q.front();
      q.pop();
      if (front == ~0u) {
        depth++;
        q.push(~0u);
        continue;
      }
      for (size_t j = masks_offset[i]; j < masks_offset[i + 1]; j++) {
        auto mask = masks_flat[j];
        auto candidate = front ^ mask;
        if (candidate == 0) {
          found = true;
          break;
        } else {
          q.push(candidate);
        }
      }
    }
    silver += depth + 1;
  }
  fmt::println("{}", silver);
}
