// hope you have fmt installed: https://github.com/fmtlib/fmt/
// and Sandia's mdspan library: https://github.com/kokkos/mdspan
// g++ -Wall -std=c++23 -Ofast -march=native g.cc -o g.elf -lfmt
// ./g.elf <input
#include <cstdint>
#include <iostream>
#include <map>
#include <memory_resource>
#include <set>
#include <string>
#include <vector>

#include <experimental/mdspan>
#include <fmt/format.h>

int main() {
  std::ios::sync_with_stdio(false);

  std::string buffer((std::istreambuf_iterator<char>(std::cin)),
                     (std::istreambuf_iterator<char>()));
  size_t stride = buffer.find('\n');
  size_t row = buffer.size() / stride - 1;

  std::uint8_t sloc = buffer.find('S');

  std::pmr::monotonic_buffer_resource mono_arena_sets{};

  auto buf = std::experimental::mdspan{buffer.data(), row, stride + 1};
  std::vector<std::pmr::set<uint8_t>> lines;
  lines.reserve(row);
  lines.emplace_back();
  lines.emplace_back(&mono_arena_sets);
  lines[1].emplace(sloc);
  size_t silver = 0;
  for (uint8_t r = 1; r < row - 1; r++) {
    std::pmr::set<std::uint8_t> nxtline(&mono_arena_sets);
    std::pmr::set<std::uint8_t> &line = lines[lines.size() - 1];
    for (uint8_t beam : line) {
      if (buf[r + 1, beam] == '^') {
        nxtline.insert(beam - 1);
        nxtline.insert(beam + 1);
        silver++;
      } else if (buf[r + 1, beam] == '.') {
        nxtline.insert(beam);
      }
    }
    lines.emplace_back(std::move(nxtline));
  }
  fmt::println("{}", silver);

  std::array<std::pmr::unsynchronized_pool_resource, 2> pool_maps{};
  std::pmr::map<std::uint8_t, std::uint64_t> line_world{&pool_maps[0]},
      prvline_world{&pool_maps[1]};
  for (auto l : lines[row - 2])
    line_world.emplace(l, 1);

  for (std::uint8_t r = row - 1; r > 1; r--) {
    for (auto [beam, score] : line_world) {
      if (beam > 0 && buf[r - 1, beam - 1] == '^')
        prvline_world[beam - 1] += score;
      if (beam < stride - 1 && buf[r - 1, beam + 1] == '^')
        prvline_world[beam + 1] += score;
      if (auto &prvline = lines[r - 1];
          prvline.find(beam) != prvline.end()) // is it in the prev line?
        prvline_world.emplace(beam, score);
    }
    std::swap(line_world, prvline_world);
    prvline_world.clear();
  }

  fmt::println("{}", line_world[sloc]);
}
