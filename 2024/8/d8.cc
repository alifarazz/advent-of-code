#include <bits/stdc++.h>

int main()
{
  std::istreambuf_iterator<char> begin(std::cin), end;
  std::string s(begin, end);
  const size_t clen = std::find(s.begin(), s.end(), '\n') - s.begin();
  const size_t rlen = std::ranges::count(s | std::views::drop(clen) | std::views::stride(clen + 1), '\n');

  std::cout << clen << std::endl;
  std::cout << rlen << std::endl;

  std::array<std::vector<std::pair<int, int>>, 256> antenna_coords {};
  for (size_t i = 0; i < rlen; i++) {
    for (size_t j = 0; j < clen; j++) {
      if (auto idx = i * (clen + 1) + j; (s[idx]) != '.')
        antenna_coords[(std::uint8_t)(s[idx])].emplace_back(i, j);
    }
  }

  auto ss = s;
  for (auto xys : antenna_coords) {
    for (size_t i = 0; i < xys.size(); i++) {
      auto trace_anti = [&](auto anti, auto diff) {
        auto is_inside = [=](auto xy) {
          auto row = xy.first, col = xy.second;
          return 0 <= row && row < rlen && 0 <= col && col < clen;
        };
        if (auto silver_loc = std::make_pair(anti.first + diff.first, anti.second + diff.second);
            is_inside(silver_loc))
          s[silver_loc.first * (clen + 1) + silver_loc.second] = '#';
        for (;
             is_inside(anti);
             anti = std::make_pair(anti.first + diff.first, anti.second + diff.second))
          ss[anti.first * (clen + 1) + anti.second] = '#';
      };
      for (size_t j = i + 1; j < xys.size(); j++) {
        auto diff = std::make_pair(xys[i].first - xys[j].first, xys[i].second - xys[j].second);
        trace_anti(xys[i], diff);
        trace_anti(xys[j], decltype(diff) { -diff.first, -diff.second });
      }
    }
  }

  std::cout << std::ranges::count(s, '#') << '\n'
            << std::ranges::count(ss, '#') << std::endl;
}
