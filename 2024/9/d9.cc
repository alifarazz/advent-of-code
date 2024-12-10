#include <algorithm>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

int main()
{
  std::istreambuf_iterator<char> begin(std::cin), end;
  std::string s(begin, end);
  s.pop_back();

  const auto nums = s | std::views::transform([](char c) -> uint32_t {
    return c == '.' ? UINT32_MAX : c - '0';
  });

  std::vector<uint32_t> disk;
  disk.reserve(
      std::ranges::fold_left(nums,
          size_t { 0 },
          std::plus<size_t>()));
  const auto compact_size = std::ranges::fold_left(nums | std::views::stride(2),
      size_t { 0 },
      std::plus<size_t>());

  auto show_disk = [&]() {
    for (auto c : disk) {
      if (c == UINT32_MAX)
        std::cout << ".";
      else
        std::cout << c;
    }
    std::cout << std::endl;
  };

  uint32_t counter = 0;
  for (size_t i = 0; i < nums.size(); i++) {
    uint32_t c = i % 2 == 1 ? UINT32_MAX : counter++;
    for (uint32_t j = 0; j < nums[i]; j++)
      disk.push_back(c);
  }

  {
    size_t i = 0, j = disk.size() - 1;
    while (i < j) {
      if (disk[j] == UINT32_MAX) {
        j--;
        continue;
      }
      if (disk[i] == UINT32_MAX) {
        disk[i] = disk[j];
        j--;
        i++;
        continue;
      }
      i++;
    }
  }

  std::uint64_t acc_silver = 0;
  for (size_t i = 0; i < compact_size; i++) {
    acc_silver += i * disk[i];
  }
  std::cout << acc_silver << std::endl;
}
