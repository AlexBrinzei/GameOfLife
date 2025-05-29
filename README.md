## General Description

This project implements extensions for Conway’s Game of Life, according to the requirements of the “Algorithm Design” course. The program manages generations of living and dead cells and offers four distinct tasks that:

1. Simulate the evolution of the game.
2. Compress the differences between generations.
3. Build a difference tree.
4. Search for Hamiltonian paths within generations.

For full specifications, see:
[https://site-pa.netlify.app/proiecte/game\_of\_life/](https://site-pa.netlify.app/proiecte/game_of_life/)

## Build and Run

### Requirements

* A standard C compiler (e.g., `gcc`)
* POSIX-compliant system (Linux/macOS/WSL)
* Source files: `main.c`, `header.c`, `header.h`

### Compilation

```bash
gcc -o game main.c header.c
```

### Execution

```bash
./game input.txt output.txt
```

* **`input.txt`** contains the task number, grid dimensions, number of steps, and the initial matrix.
* **`output.txt`** will contain the requested result.

## Project Structure

| File       | Role                                                                          |
| ---------- | ----------------------------------------------------------------------------- |
| `main.c`   | Controls program flow: reads input, invokes tasks, writes output              |
| `header.c` | Implements core logic and processing functions                                |
| `header.h` | Declares all functions and data structures (Tree, Generation, CellNode, etc.) |

## Features / Tasks

### Task 1: Game of Life Simulation

* Outputs all **k+1** generations produced by applying the standard rules.
* **Time complexity:** O(k · n · m)

### Task 2: Generation Differences

* Computes differences between consecutive generations and stores them in a stack.
* Output includes only the cells that changed state.
* **Time complexity:** O(k · n · m)

### Task 3: Difference Tree

* Builds a binary tree of generations up to depth k, using both the standard rule and “Rule B.”
* Traverses the tree in preorder and prints each generation.
* **Number of nodes:** 2ᵏ
* **Time complexity:** O(2ᵏ · n · m)

### Task 4: Hamiltonian Path

* For each generation in the tree, constructs a graph of live-cell clusters and searches for the longest Hamiltonian path using backtracking.
* Selects the longest path; in ties, picks the lexicographically smallest.
* **Worst-case time complexity:** O(N!) per connected component (with pre-check optimizations)

## Data Structures

* **CellNode** – linked list node for active cells
* **Generation** – stack of cell-difference lists between generations
* **Tree** – binary tree of generation differences (standard vs. Rule B)
* **`char **mat`** – 2D array representing the full grid

## Key Functions

| Function             | Description                                                     | Complexity           |
| -------------------- | --------------------------------------------------------------- | -------------------- |
| `read_file`          | Reads input from file                                           | O(n · m)             |
| `rules`              | Applies standard Game of Life rules                             | O(n · m)             |
| `apply_rule_B`       | Alternative rule: revives dead cells with exactly two neighbors | O(n · m)             |
| `gen_differences`    | Builds a list of changed cells between two generations          | O(n · m)             |
| `build_diff_tree`    | Constructs the binary tree according to the two rules           | O(2ᵏ · n · m)        |
| `dfs_hamilton`       | Backtracking search for the longest Hamiltonian path            | O(N!) worst case     |
| `solve_task4_file`   | Manages graph creation and path search for each component       | O(N! + N²) approx.   |
| `find_components`    | Identifies connected components via DFS                         | O(N²)                |
| `is_valid_component` | Quickly checks if a component can have a Hamiltonian path       | O(N²)                |
| `find_best_from_all` | Attempts backtracking from every node in a component            | O(N · N!) worst case |

## Additional Notes

* Pre-checks (`is_valid_component`) avoid unnecessary expensive computations.
* All data structures are manually managed to prevent memory leaks.
* The code is modular, facilitating easy extension.
* Outputs are consistent and easy to validate automatically.
