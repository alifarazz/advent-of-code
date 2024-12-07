#include <bits/stdc++.h>

int main() {
  std::istreambuf_iterator<char> sbegin(std::cin), send;
  std::string s(sbegin, send);

  std::vector<std::uint8_t> mark;
  std::vector<std::uint64_t> linenums;
  mark.reserve(128);
  linenums.reserve(128);

  std::uint64_t acc_silver = 0;

  auto charbuftoll = [](auto begin, auto end) -> std::uint64_t {
    std::uint64_t ret = 0;
    for (auto c = begin; c != end; ++c) {
      ret = ret * 10 + *c - '0';
    }
    return ret;
  };

  auto cur = s.begin();
  while (cur != s.end()) {
    auto startofline = cur;
    auto endofline = std::find(startofline, s.end(), '\n');
    auto lvalue_end = std::find(startofline, endofline, ':');
    auto lvalue = charbuftoll(startofline, lvalue_end);

    linenums.clear();
    auto numstart = lvalue_end + 2;
    auto numend = numstart;
    while (*numend != '\n') {
      numend++;
      for (; (*numend >= '0' && *numend <= '9'); ++numend)
        ;
      linenums.push_back(charbuftoll(numstart, numend));
      numstart = numend + 1;
    }
    cur = numstart;
    // std::cout << lvalue << ":";
    // for (auto v : linenums)
    //   std::cout << "\t" << v;
    // std::cout << '\n';
    bool ok = false;
    auto backtrack = [&](this const auto &backtrack, std::uint64_t left, int i) {
      if (ok) return;
      if (left == lvalue) {
        ok = true;
        return;
      }
      if (i == linenums.size() || left >= lvalue)
        return;
      backtrack(left + linenums[i] , i+ 1);
      backtrack(left * linenums[i], i + 1);
    };
    backtrack(0, 0);
    if (ok)
      acc_silver += lvalue;
  }
  std::cout << acc_silver;
}
