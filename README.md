# Sudoku Solver (DLX)

A C++ Sudoku solver using Donald Knuth's Dancing Links (DLX) algorithm,
which models Sudoku as an exact cover problem for efficient backtracking.

## Supported Sizes

| Puzzle | Grid  | Box Size |
|--------|-------|----------|
| Small  | 9×9   | 3×3      |
| Medium | 16×16 | 4×4      |
| Large  | 25×25 | 5×5      |

## Build & Run

```bash
g++ -O2 -o sudoku sudoku.cpp
./sudoku
```

## Usage

1. Select your puzzle size from the menu:
```
Select puzzle size:
1. 9x9
2. 16x16
3. 25x25
Choice:
```

2. Enter the puzzle row by row, using `0` for empty cells:
```
Row 1: 5 3 0 0 7 0 0 0 0
Row 2: 6 0 0 1 9 5 0 0 0
...
```

3. For 16×16 and 25×25, values above 9 are entered and displayed as
   letters (A=10, B=11, C=12, ...)

## Example Input (9×9)

```
5 3 0 0 7 0 0 0 0
6 0 0 1 9 5 0 0 0
0 9 8 0 0 0 0 6 0
8 0 0 0 6 0 0 0 3
4 0 0 8 0 3 0 0 1
7 0 0 0 2 0 0 0 6
0 6 0 0 0 0 2 8 0
0 0 0 4 1 9 0 0 5
0 0 0 0 8 0 0 7 9
```

## Algorithm

Sudoku is modeled as an exact cover problem with four constraint types:
- Each cell must contain exactly one digit
- Each row must contain each digit exactly once
- Each column must contain each digit exactly once
- Each box must contain each digit exactly once

DLX solves this by efficiently removing and restoring columns
in a circular doubly linked list (cover/uncover operations).
