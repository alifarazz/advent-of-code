#include <algorithm>
#include <cstdint>
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

  std::vector<std::pair<std::uint32_t, std::uint32_t>> freespaces, files; // begin idx, len
  freespaces.reserve(s.length() / 2);
  files.reserve(s.length() / 2);
  uint32_t counter = 0;
  for (size_t i = 0; i < nums.size(); i++) {
    uint32_t c = i % 2 == 1 ? UINT32_MAX : counter++;
    if (c == UINT32_MAX) // can be reduced
      freespaces.emplace_back(disk.size(), nums[i]);
    else
      files.emplace_back(disk.size(), nums[i]);
    for (uint32_t j = 0; j < nums[i]; j++)
      disk.push_back(c);
  }

  auto silver = [&]() {
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
    std::uint64_t acc_silver = 0;
    for (i = 0; i < compact_size; i++) {
      acc_silver += i * disk[i];
    }
    return acc_silver;
  };

  auto gold = [&]() {
    for (ssize_t j = s.size() / 2; j >= 0; j--) {
      auto& [filestart, filelen] = files[j];
      for (size_t i = 0; i < s.size() / 2; i++) {
        auto& [freestart, freelen] = freespaces[i];
        if (freestart > filestart)
          break;
        if (filelen <= freelen) {
          freelen -= filelen;
          filestart = freestart;
          freestart += filelen;
          break;
        }
      }
    }
    size_t acc_gold = 0;
    for (size_t i = 0; i < files.size(); i++) {
      for (size_t j = files[i].first; j < files[i].first + files[i].second; j++) {
        acc_gold += i * j;
      }
    }
    return acc_gold;
  };

  auto g = gold();
  std::cout << silver() << '\n'
            << g << std::endl;
}
// auto show_disk = [&]() {
//   for (auto c : disk) {
//     if (c == UINT32_MAX)
//       std::cout << ".";
//     else
//       std::cout << c;
//   }
//   std::cout << std::endl;
// };
