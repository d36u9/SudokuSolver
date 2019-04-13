/////////////////////////////////////////////////////////////////////
//          Copyright Yibo Zhu 2019
// Distributed under the MIT License
//    (See https://opensource.org/licenses/MIT)
/////////////////////////////////////////////////////////////////////
#pragma once
#include <array>
#include <vector>

class Sudoku final {
public:
  // Strategies to scan the board
  enum class Strategy { AccurateScan, FastFullScan, FastRowColScan };
  constexpr static int bound = 9;
  using Board = std::array<std::array<int, bound>, bound>;
  //TODO: replace "unsigned short" with std::bitset
  using BitSets = std::array<unsigned short, bound>;

  constexpr static std::array<unsigned short, bound> digits = {
      2, 4, 8, 16, 32, 64, 128, 256, 512}; // 1, 2, 3, 4, 5, 6, 7, 8, 9
  struct MinPoint {
    int x;
    int y;
    int min;
  };

  Sudoku() { reset(); }
  Sudoku(const Sudoku &) = delete;
  void reset() {
    filledcnt = 0, steps = 0;
    board.fill({0, 0, 0, 0, 0, 0, 0, 0, 0});
    rows.fill(0), cols.fill(0), grids.fill(0);
    rowsCadCnt.fill(bound), colsCadCnt.fill(bound), gridsCadCnt.fill(bound);
  }

  template <typename T> void loadBoard(T &t) {
    reset();
    for (int i = 0; i < bound; ++i)
      for (int j = 0; j < bound; ++j)
        fillCell(i, j, t[i][j]);
  }

  template <typename T> void exportBoard(T &t) {
    for (int i = 0; i < bound; ++i)
      for (int j = 0; j < bound; ++j)
        t[i][j] = board[i][j];
  }

  // template<>
  // void exportBoard(Board& b){
  //   b.swap(board);
  // }

  int getSteps() { return steps; }
  /*
   * x: index of row
   * y: index of col
   */
  // get potential candidate count at cell [x,y]
  int countCandidates(int x, int y) {
    int cnt = 0;
    unsigned short filled = rows[x] | cols[y] | grids[3 * (x / 3) + (y / 3)];
    for (int i = 0; i < bound; ++i) {
      if ((filled & digits[i]) == 0)
        ++cnt;
    }
    return cnt;
  }

  // construct potential candidates at cell [x,y]
  std::vector<int> getCandidates(int x, int y) {
    std::vector<int> possibles;
    unsigned short filled = rows[x] | cols[y] | grids[3 * (x / 3) + (y / 3)];
    for (int i = 0; i < bound; ++i) {
      if ((filled & digits[i]) == 0)
        possibles.push_back(i + 1);
    }
    return possibles;
  }

  // accurate full scan, costly, return cell [x,y] with minimal potential
  // candidate count within the board
  std::pair<int, int> accurateScan() {
    int min = 10;
    int x = -1, y = -1;
    for (int i = 0; i < bound; ++i) {
      for (int j = 0; j < bound; ++j) {
        if (board[i][j] == 0) {
          if (auto cnt = countCandidates(i, j); min > cnt) {
            min = cnt;
            x = i;
            y = j;
          }
        }
      }
    }
    return {x, y};
  }

  // fast full (row, col, grid) scan , return cell[x,y] with minimal pontential
  // candidate count across(rows, cols, grids)
  std::pair<int, int> fastFullScan() {
    auto [rx, ry, rmin] = fastRowScan();
    if (rx < 0 || ry < 0)
      return {rx, ry};
    auto [cx, cy, cmin] = fastColScan();
    if (cx < 0 || cy < 0)
      return {cx, cy};
    auto [gx, gy, gmin] = fastGridScan();
    if (gx < 0 || gy < 0)
      return {gx, gy};

    if (rmin <= cmin && rmin <= gmin)
      return {rx, ry};
    else if (cmin <= rmin && cmin <= gmin)
      return {cx, cy};
    else
      return {gx, gy};
  }

  // fast semi-full (row, col) scan , return cell[x,y] with minimal pontential
  // candidate count across(rows, cols, grids)
  std::pair<int, int> fastRowColScan() {
    auto [rx, ry, rmin] = fastRowScan();
    if (rx < 0 || ry < 0)
      return {rx, ry};
    auto [cx, cy, cmin] = fastColScan();
    if (cx < 0 || cy < 0)
      return {cx, cy};

    if (rmin <= cmin)
      return {rx, ry};
    else
      return {cx, cy};
  }

  bool fillCell(int x, int y, int v) {
    unsigned short bitDigit = (1 << v);
    if (bitDigit > 1 && colsCadCnt[y] >= 1 && rowsCadCnt[x] >= 1 &&
        gridsCadCnt[3 * (x / 3) + (y / 3)] >= 1) {
      cols[y] |= bitDigit;
      --colsCadCnt[y];
      rows[x] |= bitDigit;
      --rowsCadCnt[x];
      grids[3 * (x / 3) + (y / 3)] |= bitDigit;
      --gridsCadCnt[3 * (x / 3) + (y / 3)];
    } else
      return false;
    board[x][y] = v;
    ++filledcnt;
    return true;
  }

  void clearCell(int x, int y) {
    auto v = board[x][y];
    unsigned short bitClear = ~(static_cast<unsigned short>(1) << v);
    cols[y] &= bitClear;
    ++colsCadCnt[y];
    rows[x] &= bitClear;
    ++rowsCadCnt[x];
    grids[3 * (x / 3) + (y / 3)] &= bitClear;
    ++gridsCadCnt[3 * (x / 3) + (y / 3)];
    board[x][y] = 0;
    --filledcnt;
  }

  bool solve(Strategy strategy = Strategy::AccurateScan) {
    if (filledcnt == 81)
      return true;
    else {
      ++steps;
      std::pair<int, int> point;
      switch (strategy) {
      case Strategy::AccurateScan:
        point = accurateScan();
        break;
      case Strategy::FastFullScan:
        point = fastFullScan();
        break;
      case Strategy::FastRowColScan:
        point = fastRowColScan();
        break;
      default:
        point = accurateScan();
        break;
      }
      if (point.first < 0 || point.second < 0) {
        return false;
      }
      auto candidates = getCandidates(point.first, point.second);

      for (auto candi : candidates) {
        if (!fillCell(point.first, point.second, candi))
          return false;
        if (solve())
          return true;
        clearCell(point.first, point.second);
      }
    }
    return false;
  }

private:
  // fast Grid (3x3) scan, return cell[x,y] with minimal potential candidate
  // count within the Grid with minimal candidate count
  MinPoint fastGridScan() {
    MinPoint min = {-1, -1, 10};
    int igrd = -1;
    for (int i = 0; i < bound; ++i) {
      if (gridsCadCnt[i] >= 1 && gridsCadCnt[i] < min.min) {
        igrd = i;
        min.min = gridsCadCnt[i];
      }
    }
    min.min = 10;
    for (int i = 3 * (igrd / 3); i < 3 * (igrd / 3 + 1); ++i) {
      for (int j = 3 * (igrd % 3); j < 3 * (igrd % 3 + 1); ++j)
        if (board[i][j] == 0) {
          if (auto cnt = countCandidates(i, j); cnt < min.min) {
            min.min = cnt;
            min.x = i;
            min.y = j;
          }
        }
    }
    return min;
  }

  // fast row scan, return cell [x,y] with minimal potential candidate count
  // within the row with minimal candidate count
  MinPoint fastRowScan() {
    MinPoint min = {-1, -1, 10};
    // find row with minimal candidate count
    for (int i = 0; i < bound; ++i) {
      if (rowsCadCnt[i] >= 1 && rowsCadCnt[i] < min.min) {
        min.x = i;
        min.min = rowsCadCnt[i];
      }
    }
    min.min = 10;
    for (int i = 0; i < bound; ++i) {
      if (board[min.x][i] == 0) {
        if (auto cnt = countCandidates(min.x, i); cnt < min.min) {
          min.min = cnt;
          min.y = i;
        }
      }
    }
    return min;
  }

  // fast column scan, return cell [x,y] with minimal potential candidate count
  // within the column with minimal candidate count
  MinPoint fastColScan() {
    MinPoint min = {-1, -1, 10};
    // find column with minimal candidate count
    for (int i = 0; i < bound; ++i) {
      if (colsCadCnt[i] >= 1 && colsCadCnt[i] < min.min) {
        min.y = i;
        min.min = colsCadCnt[i];
      }
    }
    min.min = 10;
    for (int i = 0; i < bound; ++i) {
      if (board[i][min.y] == 0) {
        if (auto cnt = countCandidates(i, min.y); cnt < min.min) {
          min.min = cnt;
          min.x = i;
        }
      }
    }
    return min;
  }
  // sudoku board
  Board board;
  // bitsets to record the status of filled digits for each row, col and grid
  // (3x3)
  BitSets rows, cols, grids;
  // remaining candidate count for each row, col and grid
  std::array<int, bound> rowsCadCnt, colsCadCnt, gridsCadCnt;
  // filled cell count
  int filledcnt;
  // trail steps count
  int steps;
};
