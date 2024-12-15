#include <algorithm>
#include <cstdint>
#include <iostream>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

using RowCol = std::pair<int64_t, int64_t>;

std::string s;
std::string_view grid;
int64_t clen, rlen;

enum Direction : int { BAD_DIR = -1, LEFT, RIGHT, UP, DOWN };
const std::array<const std::pair<int64_t, int64_t>, 4> dir_offset
    = { { { 0, -1 }, { 0, 1 }, { -1, 0 }, { 1, 0 } } };

Direction dir;
bool push(RowCol bot)
{
  int64_t idx_me = bot.first * (clen + 1) + bot.second;
  RowCol next = { bot.first + dir_offset[dir].first, //
                  bot.second + dir_offset[dir].second };
  int64_t idx_next = (next.first) * (clen + 1) + next.second;
  if (s[idx_me] == 'O') {
    if (push(next)) {
      s[idx_next] = 'O';
      s[idx_me] = '.';
      return true;
    }
    return false;
  }
  if (s[idx_me] == '.') {
    return true;
  }
  if (s[idx_me] == '#') {
    return false;
  }
  if (s[idx_me] == '@') {
    if (push(next)) {
      s[idx_next] = '@';
      s[idx_me] = '.';
      return true;
    }
    return false;
  }
  exit(-3);
}

int main()
{
  // read input
  std::istreambuf_iterator<char> begin(std::cin), end;
  s = std::string(begin, end);
  std::string_view sv = s;

  // parse input
  auto input_div_idx = sv.find("\n\n");
  grid = sv.substr(0, input_div_idx);
  clen = std::ranges::find(grid, '\n') - sv.begin();
  rlen = std::ranges::count(sv | std::views::drop(clen) | std::views::stride(clen + 1),
                            '\n');
  std::string_view movesv = sv.substr(input_div_idx + 1, sv.size());

  std::uint64_t bot_idx = std::ranges::find(grid, '@') - grid.begin();
  RowCol bot = { bot_idx / (clen + 1), bot_idx % (clen + 1) };
  // std::cout << bot.first << "\t" << bot.second << '\n';

  int c = 0;
  for (auto move_char : movesv) {
    if (move_char == '\n')
      continue;
    // std::cout << "---------------------------\n";
    std::string e[4] = { { "left" }, { "right" }, "up", "down" };
    dir = move_char == '<' ? LEFT
        : move_char == '>' ? RIGHT
        : move_char == '^' ? UP
        : move_char == 'v' ? DOWN
                           : BAD_DIR;

    if (dir == -1) {
      return -2;
    }
    auto pushed = push(bot);
    // std::cout << e[int(dir)] << std::endl;
    // std::cout << c << "\t" << pushed << '\n';
    // std::cout << s.substr(0, input_div_idx) << '\n';
    if (pushed) {
      bot.first += dir_offset[dir].first;
      bot.second += dir_offset[dir].second;
    }
    c++;
  }
  int64_t silver = 0;
  for (size_t i = 0; i < rlen; i++) {
    for (size_t j = 0; j < clen; j++) {
      auto idx = i * (clen + 1) + j;
      if(s[idx] == 'O') {
        silver += i * 100 + j;
      }
    }
  }
  std::cout << silver << '\n';
}
