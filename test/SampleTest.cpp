/////////////////////////////////////////////////////////////////////
//          Copyright Yibo Zhu 2019
// Distributed under the MIT License
//    (See https://opensource.org/licenses/MIT)
/////////////////////////////////////////////////////////////////////
#include "SimpleSudoku.h"
#include "SudokuSolver.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

template <typename T> std::pair<int, uint64_t> testRun(T &&t) {
  std::chrono::high_resolution_clock::time_point s, e;
  s = std::chrono::high_resolution_clock::now();
  auto steps = t();
  e = std::chrono::high_resolution_clock::now();
  uint64_t timelapsed =
      std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count();
  return {steps, timelapsed};
}

void printBoard(Sudoku::Board &board) {
  std::cout << "<<<<<<<<<<<<<<<<<<<<" << std::endl;
  for (auto const &row : board) {
    for (auto const &c : row)
      std::cout << c;
    std::cout << std::endl;
  }
  std::cout << "<<<<<<<<<<<<<<<<<<<<" << std::endl;
}

int main(int argc, char **argv) {

  if (argc < 2) {
    // print usage
    std::cout << argv[0] << " "
              << "sudoku_txt_file";
    return 0;
  }
  // load file
  std::cout << "loading... " << argv[1] << std::endl;
  std::string filename(argv[1]);
  std::ifstream file(filename);
  // initialize the board
  Sudoku::Board board;
  board.fill({0, 0, 0, 0, 0, 0, 0, 0, 0});
  std::cout << std::endl;
  if (file.is_open()) {
    std::string line;
    int i = 0, j = 0;
    while (std::getline(file, line)) {
      std::cout << line << std::endl;
      j = 0;
      for (auto const &c : line) {
        if (isdigit(static_cast<unsigned char>(c)))
          board[i][j] = c - '0';
        ++j;
      }
      ++i;
    }
    file.close();
  } else {
    std::cout << "file doesn't exist" << std::endl;
    return 1;
  }

  std::cout << std::endl << std::endl << std::endl;
  Sudoku::Board copyBoard;
  Sudoku doku;
  SimpleSudoku simpleDoku;
  doku.loadBoard(board);
  doku.exportBoard(copyBoard);
  auto [s, t] = testRun([&]() {
    simpleDoku.solve(copyBoard);
    return simpleDoku.getSteps();
  });
  printBoard(copyBoard);
  std::cout << "SimpleSudoku takes " << s << " steps " << t << " ns"
            << std::endl;
  std::cout << std::endl;

  // accuratescan
  auto [s1, t1] = testRun([&]() {
    doku.solve(Sudoku::Strategy::AccurateScan);
    return doku.getSteps();
  });
  Sudoku::Board boardOut1;
  doku.exportBoard(boardOut1);
  printBoard(boardOut1);
  std::cout << "Sudoku::Strategy::AccurateScan takes " << s1 << " steps " << t1
            << " ns" << std::endl;
  std::cout << std::endl;

  // FastFullScan
  doku.loadBoard(board);
  auto [s2, t2] = testRun([&]() {
    doku.solve(Sudoku::Strategy::FastFullScan);
    return doku.getSteps();
  });
  Sudoku::Board boardOut2;
  doku.exportBoard(boardOut2);
  printBoard(boardOut2);
  std::cout << "Sudoku::Strategy::FastFullScan takes " << s2 << " steps " << t2
            << " ns" << std::endl;
  std::cout << std::endl;

  // FastRowColScan
  doku.loadBoard(board);
  auto [s3, t3] = testRun([&]() {
    doku.solve(Sudoku::Strategy::FastRowColScan);
    return doku.getSteps();
  });
  Sudoku::Board boardOut3;
  doku.exportBoard(boardOut3);
  printBoard(boardOut3);
  std::cout << "Sudoku::Strategy::FastRowColScan takes " << s3 << " steps "
            << t3 << " ns" << std::endl;
}
