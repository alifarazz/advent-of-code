#include <algorithm>
#include <cassert>
#include <charconv>
#include <iostream>
#include <ranges>
#include <string>
#include <unordered_map>

using namespace std::literals;

using StoneBag = std::unordered_map<size_t, size_t>;

size_t powi(size_t base, size_t exp)
{
  size_t res = 1;
  while (exp) {
    if (exp % 2 == 1)
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

void inc_or_insert_1(StoneBag& bag, size_t key, size_t inc_by = 1ULL)
{
  auto kv_ok = bag.try_emplace(key, inc_by);
  if (!kv_ok.second) // did not insert value
    kv_ok.first->second += inc_by; // incrase count of value (=[key])
}

void blink(const StoneBag& src, StoneBag& dst)
{
  for (auto [sk, sv] : src) {
    if (sk == 0) {
      inc_or_insert_1(dst, 1, sv);
      continue;
    }
    if (auto digit_len = digit_count(sk); digit_len % 2 == 0) {
      auto [l, r] = split_half(sk, digit_len);
      inc_or_insert_1(dst, l, sv);
      inc_or_insert_1(dst, r, sv);
      continue;
    }
    inc_or_insert_1(dst, sk * 2024ULL, sv);
  }
}

int main()
{
  std::istreambuf_iterator<char> begin(std::cin), end;
  std::string s = std::string(begin, end);
  s.pop_back();

  StoneBag bag1, bag2;
  for (const auto word : std::views::split(std::string_view(s), " "sv)) {
    auto word_sv = std::string_view(word);
    size_t num;
    auto res = std::from_chars(word_sv.data(), word_sv.data() + word_sv.length(), num);
    assert(res.ec == std::errc());
    assert(res.ptr == word_sv.data() + word_sv.length());
    inc_or_insert_1(bag1, num);
  }

  StoneBag &src = bag1, &dst = bag2;
  for (size_t i = 0; i < 75; i++) {
    blink(src, dst);
    std::swap(src, dst);
    if (i == 24 || i == 74) {
      std::cout << std::ranges::fold_left(src, 0ULL, [](size_t acc, auto& kv) {
        auto [k, v] = kv;
        return acc + v;
      }) << '\n';
    }
    dst.clear();
  }
}
