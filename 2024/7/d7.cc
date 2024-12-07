#include <bits/stdc++.h>

auto log_10 = [](std::uint64_t u) {
  unsigned int ret = 0;
  for (; u != 0; ret++)
    u /= 10;
  return ret;
};

auto exp_10 = [](std::uint64_t u, unsigned int v) {
  for (unsigned int i = 0; i < v; i++)
    u *= 10;
  return u;
};

auto charbuftou64 = [](auto begin, auto end) -> std::uint64_t {
  std::uint64_t ret = 0;
  for (auto c = begin; c != end; ++c)
    ret = ret * 10 + *c - '0';
  return ret;
};

int main() {
  std::istreambuf_iterator<char> sbegin(std::cin), send;
  std::string s(sbegin, send);

  std::vector<std::uint64_t> linenums;
  linenums.reserve(128);

  std::uint64_t acc_silver = 0, acc_gold = 0;
  auto cur = s.begin();
  while (cur != s.end()) {
    auto startofline = cur;
    auto endofline = std::find(startofline, s.end(), '\n');
    auto lvalue_end = std::find(startofline, endofline, ':');
    auto lvalue = charbuftou64(startofline, lvalue_end);

    linenums.clear();
    auto numstart = lvalue_end + 2;
    auto numend = numstart;
    while (*numend != '\n') {
      numend++;
      for (; (*numend >= '0' && *numend <= '9'); ++numend)
        ;
      linenums.push_back(charbuftou64(numstart, numend));
      numstart = numend + 1;
    }
    cur = numstart;

    bool ok = false;
    auto backtrack = [=, &ok]<bool is_gold>(this const auto &backtrack,
                                            std::uint64_t left, int i) {
      if (ok)
        return;
      if (left == lvalue && i == linenums.size()) {
        ok = true;
        return;
      }
      if (i == linenums.size() || left > lvalue)
        return;
      backtrack.template operator()<is_gold>(left + linenums[i], i + 1);
      backtrack.template operator()<is_gold>(left * linenums[i], i + 1);
      if constexpr (is_gold) {
        auto len = log_10(linenums[i]);
        auto newleft = exp_10(left, len);
        if (newleft <= left) // overflow check
          return;
        newleft += linenums[i];
        if (left < newleft) // overflow check
          backtrack.template operator()<is_gold>(newleft, i + 1);
      }
    };
    backtrack.template operator()<false>(linenums[0], 1);
    acc_silver += ok == true ? lvalue : 0;
    ok = false;
    backtrack.template operator()<true>(linenums[0], 1);
    acc_gold += ok == true ? lvalue : 0;
  }

  std::cout << acc_silver << '\n' << acc_gold << std::endl;
}
