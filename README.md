# 🐀 Rat in a Maze - C++ & STL Solver

A high-performance, modular **Rat in a Maze** puzzle solver written in **C++** using the **Standard Template Library (STL)**. It demonstrates core computer science search paradigms including **Backtracking (DFS)**, **Breadth-First Search (BFS)**, and **Heuristic A\* Search**.

---

## 🌟 Key Features

1. **Multiple Graph & Backtracking Algorithms**:
   - **DFS (All Paths Backtracking)**: Finds and lists all possible paths from source to destination in lexicographical order (`'D'`, `'L'`, `'R'`, `'U'`), standard in competitive programming.
   - **DFS (Single Path Search)**: Fast search with early-exit as soon as the destination is reached.
   - **BFS (Breadth-First Search)**: Guarantees the **shortest unweighted path** using a queue and parent pointer back-tracking.
   - **A\* Search Algorithm**: Evaluates path cost using the **Manhattan Distance heuristic** ($h(r, c) = |r - exit_r| + |c - exit_c|$) with a priority queue (min-heap) for optimal search efficiency.
2. **Side-by-Side Algorithm Benchmark**:
   - Compares all 4 algorithms on identical mazes, measuring:
     - Path length (optimality)
     - Explored cell count
     - Execution time (in milliseconds using `<chrono>`)
3. **Rich Terminal Visualization**:
   - Formatted ASCII / ANSI color-coded grid output highlighting:
     - `S`: Start Position (Rat)
     - `E`: Exit / Target (Cheese)
     - `*`: Solution Path Trail
     - `###`: Obstacles / Walls
     - `.`: Open Cells
4. **Flexible Maze Modes**:
   - **Preset Mazes**: Classic 4x4 (standard GeeksforGeeks/LeetCode testcase), Medium 8x8, Large 15x15.
   - **Procedural Generator**: Generates random $N \times M$ mazes with customizable wall densities and guaranteed solvability.
   - **Custom Input**: Enter custom text or binary matrix grids directly.
5. **Automated Verification Suite**:
   - Built-in self-testing suite validating correctness against known solutions and edge cases.

---

## 📚 C++ STL Components Used

| STL Component | Header | Usage in Project |
| :--- | :--- | :--- |
| `std::vector` | `<vector>` | 2D dynamic grid representation, path coordinates, and dynamic lists |
| `std::queue` | `<queue>` | FIFO frontier queue for Breadth-First Search (BFS) |
| `std::priority_queue` | `<queue>` | Min-heap open set for A\* Search |
| `std::pair` | `<utility>` | Row-column `(r, c)` grid coordinates |
| `std::string` | `<string>` | Direction sequences (`"DDRDRR"`) and CLI parsing |
| `std::set` | `<set>` | $O(\log N)$ fast lookup for rendering path overlay on the grid |
| `std::sort` | `<algorithm>` | Lexicographical ordering of DFS paths |
| `std::reverse` | `<algorithm>` | Reconstructing path from destination to source via parent pointers |
| `std::chrono` | `<chrono>` | High-resolution microsecond/millisecond performance benchmarks |
| `std::mt19937` | `<random>` | Mersenne Twister pseudo-random number generator for procedural mazes |

---

## 📂 Project Architecture

```
Rat in a Maze/
├── include/
│   ├── Maze.hpp          # Maze class definition, presets, random generator
│   └── MazeSolver.hpp    # Solver algorithms declarations and result structs
├── src/
│   ├── Maze.cpp          # Grid management, ANSI visualizer, and generator
│   ├── MazeSolver.cpp    # Implementation of DFS, BFS, A*, and helpers
│   └── main.cpp          # CLI interface, benchmarks, and automated tests
├── build.bat             # Windows one-click compilation script
├── Makefile              # Makefile for GCC/Clang/MinGW
└── README.md             # Documentation
```

---

## ⚡ Algorithmic Complexity

| Algorithm | Data Structure | Time Complexity | Space Complexity | Guarantees Shortest Path? |
| :--- | :--- | :--- | :--- | :--- |
| **DFS (All Paths)** | Recursion + Backtracking | $O(4^{N \times M})$ | $O(N \times M)$ | ❌ No |
| **DFS (Single Path)**| Recursion + Visited Set | $O(N \times M)$ | $O(N \times M)$ | ❌ No |
| **BFS** | `std::queue` + Parent Map | $O(N \times M)$ | $O(N \times M)$ | ✅ **Yes** |
| **A\* Search** | `std::priority_queue` | $O(N \times M \log(NM))$ | $O(N \times M)$ | ✅ **Yes** (with admissible heuristic) |

---

## 🚀 How to Build and Run

### Option 1: Using `build.bat` (Windows)
Double-click `build.bat` or run:
```cmd
build.bat
.\rat_in_maze.exe
```

### Option 2: Using `g++` directly
```cmd
g++ -std=c++14 -O2 -Iinclude src/Maze.cpp src/MazeSolver.cpp src/main.cpp -o rat_in_maze.exe
.\rat_in_maze.exe
```

### Option 3: Using `make` (Linux / macOS / MSYS2)
```bash
make
./rat_in_maze.exe
```

---

## 🧪 Example Output

### Classic 4x4 Maze Solution
```
    0  1  2  3  
   +-------------+
0  |  S #########|
1  |  *  . ### . |
2  |  *  * ######|
3  | ### *  *  E |
   +-------------+
Legend: S = Start (Rat), E = Exit (Cheese), * = Solution Path, ### = Obstacle/Wall, . = Open Cell

Algorithm: BFS (Shortest Path)
Status:    PATH FOUND!
Time:      0.0000 ms
Length:    7 steps (including start & destination)
Direction: DDRDRR
Steps:     (0,0) -> (1,0) -> (2,0) -> (2,1) -> (3,1) -> (3,2) -> (3,3)
```

### Multi-Algorithm Benchmark Table
```
=== Multi-Algorithm Performance Benchmark ===

+--------------------------------+----------+-------------+----------------+--------------+
| Algorithm                      | Found?   | Path Length | Cells Visited  | Time (ms)    |
+--------------------------------+----------+-------------+----------------+--------------+
| DFS (All Paths Backtracking)   | YES      | 7           | 14             | 0.0000       |
| DFS (Single Path Search)       | YES      | 7           | 7              | 0.0000       |
| BFS (Shortest Path)            | YES      | 7           | 8              | 0.0000       |
| A* Search (Manhattan Heuristic)| YES      | 7           | 7              | 0.0000       |
+--------------------------------+----------+-------------+----------------+--------------+
```
