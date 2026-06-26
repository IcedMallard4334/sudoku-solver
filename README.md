# Sudoku Solver (DLX)

A C++ Sudoku solver using Donald Knuth's Dancing Links (DLX) algorithm,
which models Sudoku as an exact cover problem for efficient backtracking.

## Configuration

The grid size is controlled by two constants at the top of the file:

```cpp
const int N = 25;        // Grid size (N x N)
const int BOX_SIZE = 5;  // Box size (BOX_SIZE x BOX_SIZE)
```

Change these to match your desired puzzle size:

| Puzzle | N  | BOX_SIZE |
|--------|----|----------|
| 9×9    | 9  | 3        |
| 16×16  | 16 | 4        |
| 25×25  | 25 | 5        |

> Note: N must equal BOX_SIZE².

## Usage

1. Set `N` and `BOX_SIZE` to your desired puzzle size
2. Fill in the `puzzle` array in `main()` — use `0` for empty cells
3. For 16×16 and 25×25, values above 9 are printed as letters (A, B, C...)

## Build & Run

```bash
g++ -O2 -o sudoku sudoku.cpp
./sudoku
```

## Algorithm

Sudoku is modeled as an exact cover problem with four constraint types:
- Each cell must have exactly one digit
- Each row must contain each digit once
- Each column must contain each digit once
- Each box must contain each digit once

DLX solves this efficiently using linked list column operations (cover/uncover).
