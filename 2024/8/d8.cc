#include <bits/stdc++.h>
#include <ranges>

int main()
{
  std::istreambuf_iterator<char> begin(std::cin), end;
  std::string s(begin, end);
  const size_t clen = std::find(s.begin(), s.end(), '\n') - s.begin();
  const size_t rlen = std::count(s.begin(), s.end(), '\n');
  std::cout << rlen << std::endl;
  std::cout << clen << std::endl;

  std::array<std::vector<std::pair<int, int>>, 256> antenna_coords {};
  for (size_t i = 0; i < rlen; i++) {
    for (size_t j = 0; j < clen; j++) {
      auto idx = i * (clen + 1) + j;
      auto is_antenna = [](const char c) {
        return ('0' <= c && c <= '9') || ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z');
      };
      if (is_antenna(s[idx])) {
        antenna_coords[(std::uint8_t)(s[idx])].emplace_back(i, j);
      }
    }
  }

  for (auto xys : antenna_coords) {
    for (size_t i = 0; i < xys.size(); i++) {
      for (size_t j = i + 1; j < xys.size(); j++) {
        auto diff = std::make_pair(xys[i].first - xys[j].first, xys[i].second - xys[j].second);
        auto anti1 = std::make_pair(xys[i].first + diff.first, xys[i].second + diff.second);
        auto anti2 = std::make_pair(xys[j].first - diff.first, xys[j].second - diff.second);
        auto is_inside = [=](auto xy) {
          auto row = xy.first, col = xy.second;
          return 0 <= row && row < rlen && 0 <= col && col < clen;
        };
        if (is_inside(anti1))
          s[anti1.first * (clen + 1) + anti1.second] = '#';
        if (is_inside(anti2))
          s[anti2.first * (clen + 1) + anti2.second] = '#';
      }
    }
  }

  std::cout << s;
  const auto silver = std::count(s.begin(), s.end(), '#');
  std::cout << silver << std::endl;
}
