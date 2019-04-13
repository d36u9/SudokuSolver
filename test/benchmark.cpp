/////////////////////////////////////////////////////////////////////
//          Copyright Yibo Zhu 2019
// Distributed under the MIT License
//    (See https://opensource.org/licenses/MIT)
/////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#define NOMINMAX
#endif

#include "SudokuSolver.h"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <string_view>
#include <vector>

void printMetrics(std::string_view name, std::uint64_t avgsteps,
                  std::uint64_t maxsteps, std::uint64_t minsteps,
                  std::uint64_t avgtime, std::uint64_t maxtime,
                  std::uint64_t mintime) {
  std::cout << name << " ";
  std::cout << "steps [ avg: " << avgsteps << " ";
  std::cout << "  max: " << maxsteps << " ";
  std::cout << "  min: " << minsteps << " ]  ";
  std::cout << "time [ avg: " << avgtime << " ns";
  std::cout << "  max: " << maxtime << " ns";
  std::cout << "  min: " << mintime << " ns]" << std::endl;
}

template <typename S>
void benchmark(std::string_view name, Sudoku::Strategy strategy,
               std::vector<Sudoku::Board> &boards) {
  S s;

  std::chrono::high_resolution_clock::time_point start, end;
  uint64_t sumSteps(0);
  uint64_t maxSteps(0);
  uint64_t minSteps = std::numeric_limits<uint64_t>::max();
  uint64_t sumTime(0);
  uint64_t maxTime(0);
  uint64_t minTime = std::numeric_limits<uint64_t>::max();
  const int n = boards.size();
  if (n < 1)
    return;
  for (auto const &board : boards) {
    s.loadBoard(board);
    start = std::chrono::high_resolution_clock::now();
    s.solve(strategy);
    end = std::chrono::high_resolution_clock::now();
    uint64_t time =
        std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
            .count();
    uint64_t steps = s.getSteps();
    sumSteps += steps;
    maxSteps = std::max(maxSteps, steps);
    minSteps = std::min(minSteps, steps);
    sumTime += time;
    maxTime = std::max(maxTime, time);
    minTime = std::min(minTime, time);
  }
  printMetrics(name, sumSteps / n, maxSteps, minSteps, sumTime / n, maxTime,
               minTime);
}

int main(int argc, char **argv) {

  if (argc < 2) {
    // print usage
    std::cout << argv[0] << " "
              << "sudoku_txt_file_one_line_for_each_puzzle";
    return 0;
  }
  // load file
  std::cout << "loading... " << argv[1] << std::endl;
  std::string filename(argv[1]);
  std::ifstream file(filename);

  // initialize the benchmark test cases
  std::vector<Sudoku::Board> boards;

  std::cout << std::endl;
  if (file.is_open()) {
    std::string line;
    while (std::getline(file, line)) {
      if (line.size() >= 81) {
        Sudoku::Board board;
        board.fill({0, 0, 0, 0, 0, 0, 0, 0, 0});
        int i = 0;
        for (auto const &c : line) {
          if (i < 81 && isdigit(static_cast<unsigned char>(c)))
            board[i / 9][i % 9] = c - '0';
          ++i;
        }
        boards.push_back(board);
      }
    }
    file.close();
  } else {
    std::cout << "file doesn't exist" << std::endl;
    return 1;
  }

  benchmark<Sudoku>("Sudoku::Strategy::AccurateScan",
                    Sudoku::Strategy::AccurateScan, boards);
  benchmark<Sudoku>("Sudoku::Strategy::FastFullScan",
                    Sudoku::Strategy::FastFullScan, boards);
  benchmark<Sudoku>("Sudoku::Strategy::FastRowColScan",
                    Sudoku::Strategy::FastRowColScan, boards);
  return 0;
}