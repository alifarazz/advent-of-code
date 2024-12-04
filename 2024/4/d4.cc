#include <bits/stdc++.h>

static std::array<std::array<std::pair<int8_t, int8_t>, 3>, 8> offsets = {{
    {{{1, 0}, {2, 0}, {3, 0}}},
    {{{-1, 0}, {-2, 0}, {-3, 0}}},
    {{{0, 1}, {0, 2}, {0, 3}}},
    {{{1, 1}, {2, 2}, {3, 3}}},
    {{{-1, 1}, {-2, 2}, {-3, 3}}},
    {{{0, -1}, {0, -2}, {0, -3}}},
    {{{1, -1}, {2, -2}, {3, -3}}},
    {{{-1, -1}, {-2, -2}, {-3, -3}}},
}};

static size_t has_xmas_silver(const char *const s,
                              std::pair<size_t, size_t> row_col, size_t clen,
                              size_t rlen, int d) {
  auto &off = offsets[d];
  auto str = "MAS";
  for (size_t i = 0; i < 3; i++) {
    auto &o = off[i];
    ssize_t row = o.second + (ssize_t)row_col.first;
    ssize_t col = o.first + (ssize_t)row_col.second;
    if (!((row >= 0 && row < rlen) && (col >= 0 && col < clen) &&
          s[(clen + 1) * row + col] == str[i]))
      return 0;
  }
  return 1;
}

constexpr uint32_t hash4(const char *const s) {
  uint32_t result = 0;
  for (int i = 0; i < 4; i++)
    result |= (uint32_t)s[i] << (i * 8);
  return result;
}

inline std::array<uint32_t, 4> init_gold_hashs() {
  std::array<uint32_t, 4> gold_hashs;
  std::array<const char *, 4> gold_strs{{"MMSS", "SSMM", "SMSM", "MSMS"}};
  std::transform(std::begin(gold_strs), std::end(gold_strs),
                 std::begin(gold_hashs), hash4);
  return gold_hashs;
}

int main() {
  std::istreambuf_iterator<char> begin(std::cin), end;
  std::string s(begin, end);
  const size_t clen = std::find(s.begin(), s.end(), '\n') - s.begin();
  const size_t rlen = std::count(s.begin(), s.end(), '\n');

  std::array<uint32_t, 4> gold_hashs = init_gold_hashs();

  size_t acc_silver = 0, acc_gold = 0;
  for (size_t i = 0; i < rlen; i++) {
    for (size_t j = 0; j < clen; j++) {
      if (const size_t offset = i * (clen + 1) + j; s[offset] == 'X')
        for (int k = 0; k < 8; k++)
          acc_silver += has_xmas_silver(s.data(), {i, j}, clen, rlen, k);
      if (i < rlen - 2 && j < clen - 2 &&
          s[(i + 1) * (clen + 1) + j + 1] == 'A') {
        std::array<char, 4> gather_buf;
        gather_buf[0] = s[(i + 0) * (clen + 1) + j + 0];
        gather_buf[1] = s[(i + 0) * (clen + 1) + j + 2];
        gather_buf[2] = s[(i + 2) * (clen + 1) + j + 0];
        gather_buf[3] = s[(i + 2) * (clen + 1) + j + 2];
        acc_gold += std::find(std::begin(gold_hashs), std::end(gold_hashs),
                              hash4(gather_buf.data())) != std::end(gold_hashs);
      }
    }
  }
  std::cout << acc_silver << '\n' << acc_gold << '\n';
}
