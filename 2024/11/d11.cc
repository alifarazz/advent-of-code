#include <algorithm>
#include <cassert>
#include <charconv>
#include <iomanip>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

using namespace std::literals;

using StoneLine = std::vector<size_t>;

size_t powi(size_t base, unsigned int exp)
{
  size_t res = 1;
  while (exp) {
    if (exp & 1)
      res *= base;
    exp >>= 1;
    base *= base;
  }
  return res;
}

size_t digit_count(size_t num)
{
  size_t counter = 0;
  for (; num != 0; num /= 10)
    counter++;
  return counter;
}

std::tuple<size_t, size_t> split_half(size_t num, size_t digit_len)
{
  size_t halfer = powi(10, digit_len / 2);
  return { num / halfer, num % halfer };
}

void blink(const StoneLine& src, StoneLine& dst)
{
  for (auto s : src) {
    if (s == 0) {
      dst.push_back(1);
      continue;
    }
    if (auto digit_len = digit_count(s); digit_len % 2 == 0) {
      auto [l, r] = split_half(s, digit_len);
      dst.push_back(l);
      dst.push_back(r);
      continue;
    }
    dst.push_back(s * 2024);
  }
}

int main()
{
  std::istreambuf_iterator<char> begin(std::cin), end;
  std::string s = std::string(begin, end);
  s.pop_back();

  StoneLine line1, line2;
  line1.reserve(1'000'000);
  line2.reserve(1'000'000);
  for (const auto word : std::views::split(std::string_view(s), " "sv)) {
    auto word_sv = std::string_view(word);
    size_t result;
    auto res = std::from_chars(word_sv.data(), word_sv.data() + word_sv.length(), result);
    assert(res.ec == std::errc());
    assert(res.ptr == word_sv.data() + word_sv.length());
    line1.push_back(result);
  }

  for (auto u : line1)
    std::cout << u << " ";
  std::cout << std::endl;
  StoneLine& a = line1, &b = line2;
  for (size_t i = 0; i < 25; i++) {
    blink(a, b);
    std::swap(a, b);
    b.clear();
    // for (auto u : line1)
    //   std::cout << u << " ";
    // std::cout << std::endl;
    std::cout << a.size() << std::endl;
  }
}
