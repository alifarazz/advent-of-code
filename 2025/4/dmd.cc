// hope you have fmt installed: https://github.com/fmtlib/fmt/
// and Sandia's mdspan library: https://github.com/kokkos/mdspan
// g++ -Wall -std=c++23 -Ofast -march=native dmd.cc -o dmd.elf -lfmt
// ./dmd.elf input
#include <algorithm>
#include <fstream>
#include <memory_resource>
#include <vector>

#include <fmt/format.h>

#include <experimental/mdspan>

namespace EX = std::experimental;

std::array<std::pair<size_t, size_t>, 8> offsets{
    {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}}};

template <class ExtsA, class LayA, class AccA>
size_t fast_pass(EX::mdspan<char, ExtsA, LayA, AccA> buf,
                 EX::mdspan<char, ExtsA, LayA, AccA> buf2,
                 std::pair<size_t, size_t>&& rc) {
  size_t ok = 0;
  for (size_t j = 1; j < rc.second - 1; j++) { // first row [1:-1]
    if (buf[0, j] == '@' && ((buf[0, j - 1] == '@') + (buf[0, j + 1] == '@') +
                             (buf[1, j - 1] == '@') + (buf[1, j] == '@') +
                             (buf[1, j + 1] == '@')) < 4) {
      buf2[0, j] = 'x';
      ok++;
    }
  }
  for (size_t i = 1; i < rc.first - 1; i++) {
    if (buf[i, 0] == '@' &&
        ((buf[i - 1, 0] == '@') + (buf[i - 1, 1] == '@') + (buf[i, 1] == '@') +
         (buf[i + 1, 0] == '@') + (buf[i + 1, 1] == '@')) <
            4) { // first col of the row
      buf2[i, 0] = 'x';
      ok++;
    }
    for (size_t j = 1; j < rc.second - 1; j++) {
      if (buf[i, j] != '@')
        continue;
      int c{};
      for (auto [di, dj] : offsets) {
        c += (buf[i + di, j + dj] == '@');
        if (c == 4) {
          break;
        }
      }
      if (c < 4) {
        buf2[i, j] = 'x';
        ok++;
      }
    }
    if (buf[i, rc.second - 1] == '@' &&
        ((buf[i, rc.second - 2] == '@') + (buf[i - 1, rc.second - 2] == '@') +
         (buf[i - 1, rc.second - 1] == '@') +
         (buf[i + 1, rc.second - 2] == '@') +
         (buf[i + 1, rc.second - 1] == '@')) < 4) { // last col of the row
      buf2[i, rc.second - 1] = 'x';
      ok++;
    }
  }
  for (size_t j = 1; j < rc.second - 1; j++) { // last row [1:-1]
    if (buf[rc.first - 1, j] == '@' &&
        ((buf[rc.first - 1, j - 1] == '@') + (buf[rc.first - 1, j + 1] == '@') +
         (buf[rc.first - 2, j - 1] == '@') + (buf[rc.first - 2, j] == '@') +
         (buf[rc.first - 2, j + 1] == '@')) < 4) {
      buf2[rc.first - 1, j] = 'x';
      ok++;
    }
  }

  // four corners
  std::array<std::tuple<size_t, size_t>, 4> corners{{
      {0zu, 0zu},
      {0zu, rc.second - 1},
      {rc.first - 1, 0zu},
      {rc.first - 1, rc.second - 1}}};
  for (auto [i, j] : corners) {
    if (buf[i, j] == '@') {
      buf2[i, j] = 'x';
      ok++;
    }
  }
  return ok;
}

int main(int argc, char *argv[]) {
  std::ios::sync_with_stdio(false);
  if (argc != 2)
    return 69;

  std::ifstream f(argv[1]);
  f.seekg(0, std::ios::end);

  size_t file_size = f.tellg();
  f.seekg(0);

  std::pmr::monotonic_buffer_resource heap_resource(
      file_size * 2); // won't reallocate, pinky promise

  std::pmr::vector<char> buf{&heap_resource}, buf2{&heap_resource};
  buf.resize(file_size);
  buf2.resize(file_size);

  f.read(buf.data(), buf.size());

  auto col = std::find(buf.begin(), buf.end(), '\n').base() - buf.data();
  auto row = buf.size() / col - 1;

  buf2 = buf;

  auto sp1 = EX::mdspan{buf.data(), row, col + 1};
  auto sp2 = EX::mdspan{buf2.data(), row, col + 1};

  auto silver = fast_pass(sp1, sp2, {row, col});
  fmt::println("{}", silver);

  size_t removed, gold = silver;
  do {
    buf = buf2;
    removed = fast_pass(sp1, sp2, {row, col});
    gold += removed;
  } while (removed);

  fmt::println("{}", gold);
}
