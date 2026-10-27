#ifndef MAZE_SOLVER_HPP
#define MAZE_SOLVER_HPP

#include "Maze.hpp"
#include <vector>
#include <string>
#include <utility>
#include <chrono>

struct SolverResult {
    bool found = false;
    std::vector<std::pair<int, int>> path; // Coordinates [(r0, c0), (r1, c1), ...]
    std::string directionString;           // e.g., "DDRR"
    std::vector<std::string> allPaths;     // All paths found (for DFS all-paths)
    int cellsVisited = 0;                  // Metric: Number of cells visited/explored
    double timeElapsedMs = 0.0;            // Execution time in milliseconds
    std::string algorithmName;
};

class MazeSolver {
public:
    // 1. Classical Backtracking DFS - Find All Paths (Lexicographically: D, L, R, U)
    static SolverResult solveDFSAllPaths(const Maze& maze);

    // 2. DFS - Find Single Path (Fast first-path search)
    static SolverResult solveDFSSinglePath(const Maze& maze);

    // 3. Breadth-First Search (BFS) - Guaranteed Shortest Path (Unweighted)
    static SolverResult solveBFS(const Maze& maze);

    // 4. A* Search Algorithm - Optimal Path with Manhattan Distance Heuristic
    static SolverResult solveAStar(const Maze& maze);

    // Helper: converts a sequence of coordinate moves to a string of directions ('D','L','R','U')
    static std::string pathToDirectionString(const std::vector<std::pair<int, int>>& path);

private:
    struct Direction {
        int dr;
        int dc;
        char code;
    };

    // Standard directions in alphabetical order: Down, Left, Right, Up
    static const std::vector<Direction> DIRECTIONS;

    // Recursive helper for DFS all-paths
    static void dfsAllPathsHelper(
        const Maze& maze,
        int r, int c,
        std::vector<std::vector<bool>>& visited,
        std::string& currentPath,
        std::vector<std::string>& allPaths,
        int& cellsVisited
    );

    // Recursive helper for DFS single-path
    static bool dfsSinglePathHelper(
        const Maze& maze,
        int r, int c,
        std::vector<std::vector<bool>>& visited,
        std::vector<std::pair<int, int>>& currentPath,
        int& cellsVisited
    );
};

#endif // MAZE_SOLVER_HPP
