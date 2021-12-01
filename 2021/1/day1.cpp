#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

int main() {
  std::vector<int_fast32_t> v{2000};
  for (std::string line; std::getline(std::cin, line);) {
    v.push_back(std::stoi(line));
  }

  int_fast32_t c{};
  for (std::size_t i = 1; i < v.size(); i++)
    if (v[i - 1] < v[i])
      c++;
  std::cout << c << '\n';

  c = 0;
  for (std::size_t i = 2; i < v.size() - 1; i++)
    if (v[i - 2] + v[i - 1] + v[i] < v[i - 1] + v[i] + v[i + 1])
      c++;
  std::cout << c << '\n';
}
