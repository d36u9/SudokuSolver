# SudokuSolver
[[MIT](https://opensource.org/licenses/MIT)


## Welcome
This is just a product of cheating-via-programming when I played Sudoku with some kids :)
It's not fully completed, just for fun.
NO fancy algorithms here, like [SA](https://en.wikipedia.org/wiki/Simulated_annealing) or [GA](https://en.wikipedia.org/wiki/Genetic_algorithm).
It's simple backtracking + constraint programming.
NOTE: only return one result even if there are multiple solutions

## TODO
* polish the code
* more features
* more benchmarks

## It's simple
### C++17 required& build
```
cmake -H. -Bbuild
cmake --build build --config Release
```

## SampleTest& benchmark
### SampleTest
accepts single sudoku puzzle in a plain text format.
all sample txt files in "samples" folder
.e.g
```
<<<<<<<<<<<<<<<<<<<<
534678912
672195348
198342567
859761423
426853791
713924856
961537284
287419635
345286179
<<<<<<<<<<<<<<<<<<<<
Sudoku::Strategy::FastFullScan takes 51 steps 24804 ns
```
### benchmark
accepts sudoku puzzles in a plain txt (one dimension/ single line) format.
all puzzle txt files in "benchmark" folder.
e.g
```
Sudoku::Strategy::AccurateScan steps [ avg: 134   max: 480   min: 56 ]  time [ avg: 61846 ns  max: 357234 ns  min: 22238 ns]
Sudoku::Strategy::FastFullScan steps [ avg: 137   max: 2166   min: 56 ]  time [ avg: 60912 ns  max: 848752 ns  min: 21667 ns]
Sudoku::Strategy::FastRowColScan steps [ avg: 138   max: 2331   min: 56 ]  time [ avg: 61439 ns  max: 996435 ns  min: 21383 ns]
```

Source of benchmark test causes and Many Thanks to: [http://printable-sudoku-puzzles.com](http://printable-sudoku-puzzles.com/wfiles/)

