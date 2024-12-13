#include <algorithm>
#include <array>
#include <charconv>
#include <cstdint>
#include <iostream>
#include <ranges>
#include <set>
#include <string>
#include <vector>

std::string s;

int main()
{
  std::istreambuf_iterator<char> begin(std::cin), end;
  s = std::string(begin, end);

  size_t acc = 0;
  size_t idx = 0;
  do {
    size_t xa, ya, xb, yb;
    idx += 12;
    std::from_chars(s.data() + idx, s.data() + idx + 2, xa);
    idx += 6;
    std::from_chars(s.data() + idx, s.data() + idx + 2, ya);
    idx += 12 + 3;
    std::from_chars(s.data() + idx, s.data() + idx + 2, xb);
    idx += 6;
    std::from_chars(s.data() + idx, s.data() + idx + 2, yb);
    idx += 9 + 3;

    size_t X, Y;
    auto fc_res = std::from_chars(s.data() + idx, s.data() + idx + 1337, X);
    idx = fc_res.ptr - s.data() + 4;
    size_t idx_nl = s.begin() - std::find(s.begin() + idx, s.end(), '\n');
    fc_res = std::from_chars(s.data() + idx, s.data() + idx_nl, Y);
    idx = fc_res.ptr - s.data() + 2;

    // std::cout << xa << "\n" << ya << "\n" << xb << "\n" << yb << "\n\n";
    // std::cout << X << "\n" << Y << "\n";
    // std::cout << "--------------------\n";

    size_t na = 0, nb = 0;
    uint64_t cost = UINT64_MAX;
    for (size_t i = 1; i <= 100; i++) {
      for (size_t j = 1; j <= 100; j++) {
        uint64_t newcost = i * 3 + j;
        size_t x = i * xa + j * xb;
        size_t y = i * ya + j * yb;
        if (x == X && y == Y && cost > newcost) {
          std::cout << "AH" << std::endl;
          na = i;
          nb = j;
          cost = newcost;
        }
      }
    }
    // std::cout << na << '\t' << nb << "\n";
    acc += na  * 3+ nb;
  } while (idx < s.size());
  std::cout << acc;
}
