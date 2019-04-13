/////////////////////////////////////////////////////////////////////
//          Copyright Yibo Zhu 2019
// Distributed under the MIT License
//    (See https://opensource.org/licenses/MIT)
/////////////////////////////////////////////////////////////////////
#pragma once
#include <array>

class SimpleSudoku final {
public:
  constexpr static int bound = 9;
  using Board = std::array<std::array<int, bound>, bound>;
  SimpleSudoku() : steps(0) {}
  void reset() { steps = 0; }
  int getSteps() { return steps; }
  bool solve(Board &board) {
    for (int i = 0; i < bound; ++i) {
      for (int j = 0; j < bound; ++j) {
        if (board[i][j] == 0) {
          for (int k = 0; k < bound; ++k) {
            board[i][j] = 1 + k;
            ++steps;
            if (isValid(board, i, j) && solve(board))
              return true;
            board[i][j] = 0;
          }
          return false;
        }
      }
    }
  }

protected:
  bool isValid(Board &board, int x, int y) {
    for (int i = 0; i < bound; ++i) {
      if (i != x && board[i][y] == board[x][y])
        return false;
      if (i != y && board[x][i] == board[x][y])
        return false;
    }
    for (int i = 3 * (x / 3); i < 3 * (x / 3 + 1); ++i) {
      for (int j = 3 * (y / 3); j < 3 * (y / 3 + 1); ++j)
        if ((i != x || j != y) && board[i][j] == board[x][y])
          return false;
    }
    return true;
  }
  int steps;
};