#include <iostream>

int main()
{
  std::istreambuf_iterator<char> begin(std::cin), end;
  std::string s = std::string(begin, end);

  std::uint64_t acc_silver = 0, acc_gold = 0;
  size_t idx = 0;
  do {
    std::int64_t xa, ya, xb, yb;
    idx += 12;
    std::from_chars(s.data() + idx, s.data() + idx + 2, xa);
    idx += 6;
    std::from_chars(s.data() + idx, s.data() + idx + 2, ya);
    idx += 12 + 3;
    std::from_chars(s.data() + idx, s.data() + idx + 2, xb);
    idx += 6;
    std::from_chars(s.data() + idx, s.data() + idx + 2, yb);
    idx += 9 + 3;

    std::int64_t X, Y;
    auto fc_res = std::from_chars(s.data() + idx, s.data() + idx + 1337, X);
    idx = fc_res.ptr - s.data() + 4;
    size_t idx_nl = s.begin() - std::find(s.begin() + idx, s.end(), '\n');
    fc_res = std::from_chars(s.data() + idx, s.data() + idx_nl, Y);
    idx = fc_res.ptr - s.data() + 2;

    auto cramer = [&](std::int64_t X, std::int64_t Y, std::uint64_t& acc) {
      if (std::int64_t det = xa * yb - xb * ya; det) {
        std::int64_t na = X * yb - xb * Y;
        std::int64_t nb = xa * Y - X * ya;
        if (na % det == 0 && nb % det == 0) {
          na /= det;
          nb /= det;
          acc += na * 3 + nb;
        }
      }
    };
    cramer(X, Y, acc_silver);
    cramer(X + 10000000000000, Y + 10000000000000, acc_gold);
  } while (idx < s.size());
  std::cout << acc_silver << "\n" << acc_gold << std::endl;
}
