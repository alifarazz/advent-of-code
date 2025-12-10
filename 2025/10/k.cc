// hope you have fmt installed: https://github.com/fmtlib/fmt/
// and COIN-OR's CBC solver: https://github.com/coin-or/Cbc
// g++ -Wall -std=c++23 -Ofast -march=native k.cc -o k.elf \
//                            -lfmt $(pkg-config --libs cbc)
// ./k.elf <input
#include <cassert>
#include <charconv>
#include <cstdint>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

#include <fmt/base.h>

#include <coin/CbcModel.hpp>
#include <coin/OsiClpSolverInterface.hpp>

long SolveIP(std::vector<uint32_t> &jolts, std::vector<std::uint32_t> &masks) {

  // min dot(c, x)
  // where c is a vector of constants,
  // x is a vector of variables where lb_i <= x_i <= ub
  // subject to
  // [x_0 + x_1 + x_2 + x_3] * A_0 = jolt_0
  // [x_0 + x_1 + x_2 + x_3] * A_1 = jolt_1
  // [x_0 + x_1 + x_2 + x_3] * A_2 = jolt_2
  //  ...
  // A_i is the ith column of integer coefficients matrix A

  const auto nButtons = masks.size();
  const auto nJolts = jolts.size();

  const auto cols = nButtons; // number of variables x_i
  const auto rows = nJolts;   // number of constraints

  // objective func is: min dot(c, x). c_i should be 1.0
  std::vector<double> obj_coeffs(cols, 1.0); // c_i

  // variable bounds (lb <= x_i <ub)
  std::vector<double> colUpper(cols, COIN_DBL_MAX), colLower(cols, 0.0);

  std::vector<double> rowRHS;
  rowRHS.reserve(nJolts);
  for (auto jolt : jolts)
    rowRHS.push_back((double)jolt);

  // It expectes lowerRHS <= Ax <= upperRHS
  auto &RHSlower = rowRHS;
  auto &RHSupper = rowRHS;

  // Fill-out column-major sparse matrix A for coefficients of x, per each row
  std::vector<CoinBigIndex> colStarts(cols + 1, 0); //
  std::vector<double> elems;                        // non-zero elem values
  std::vector<int> rowIdxs; // row index of each non-zero elem
  for (size_t mid = 0; mid < nButtons; mid++) { // walk on var x_mid
    auto mask = masks[mid];
    for (size_t i = 0; i < nJolts; i++) { // Is the coeff of x_mid on
                                          // constraint i 1 or 0? if 1
                                          // include it, if 0 skip.
      if ((1 << i) & mask) {              // Button will inc ith jolt.
        elems.push_back(1.0);
        rowIdxs.push_back(i);
      }
    }
    colStarts[mid + 1] = rowIdxs.size();
  }

  OsiClpSolverInterface solver; // structure data into clp's format
  solver.setLogLevel(-1);       // shut up

  // clang-format off
  solver.loadProblem( 
      cols, 
      rows,
      colStarts.data(),
      rowIdxs.data(),
      elems.data(),
      colLower.data(),
      colUpper.data(),
      obj_coeffs.data(),
      RHSlower.data(),
      RHSupper.data()
  );
  // clang-format on

  solver.setObjSense(1.0); //  1.0 for miniize, -1.0 for maximize

  for (size_t i = 0; i < cols; i++)
    solver.setInteger(i); // var x_i is an integer

  CbcModel model(solver);
  model.setLogLevel(-1); // shut up 2

  model.initialSolve();   // solve LP
  model.branchAndBound(); // IP branch and bound

  if (model.isProvenOptimal()) {
    return std::lroundl(model.getObjValue());
  }

  fmt::println("Solver didn't find an optimal answer, status: {}",
               model.status());
  exit(-3);
}

int main() {
  std::ios::sync_with_stdio(false);
  std::string input((std::istreambuf_iterator<char>(std::cin)),
                    std::istreambuf_iterator<char>());

  size_t silver{}, gold{};
  const auto input_end = input.data() + input.size();
  for (const char *cur = input.data(); cur < input_end;) {
    // parse line
    uint32_t trgt{};
    assert(*cur == '[');
    // auto begin = cur;
    while (*++cur != ']')
      ;
    assert(*cur == ']');
    auto end = cur - 1;
    while (*end != '[') {
      trgt <<= 1;
      trgt |= *end-- == '#';
    }
    assert(*++cur == ' ');
    assert(*++cur == '(');
    std::vector<std::uint32_t> masks;
    while (*cur == '(') {
      uint32_t mask{};
      assert(*cur++ == '(');
      while (*cur != ' ') {
        int bit_idx{};
        cur = std::from_chars(cur, input_end, bit_idx).ptr + 1;
        mask |= (1 << bit_idx);
      }
      masks.push_back(mask);
      cur++;
    }
    assert(*cur == '{');
    std::vector<uint32_t> jolts;
    while (*++cur != '\n') {
      int amnt{};
      cur = std::from_chars(cur, input_end, amnt).ptr;
      jolts.push_back(amnt);
    }
    // fmt::println("{}", jolts);
    cur++;
    // SILVER
    size_t depth{};
    std::queue<uint32_t> q;
    q.push(trgt);
    q.push(~0u);
    auto found = false;
    while (!found) {
      auto front = q.front();
      q.pop();
      if (front == ~0u) {
        depth++;
        q.push(~0u);
        continue;
      }
      for (auto mask : masks) {
        auto candidate = front ^ mask;
        if (candidate == 0) {
          found = true;
          break;
        } else {
          q.push(candidate);
        }
      }
    }
    silver += depth + 1;
    // GOLD
    gold += SolveIP(jolts, masks);
  }
  fmt::println("{}", silver);
  fmt::println("{}", gold);
}
