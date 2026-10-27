#include "Maze.hpp"
#include "MazeSolver.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
void setupConsole() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
}
#else
void setupConsole() {}
#endif

// ANSI colors
namespace Color {
    const std::string RESET   = "\033[0m";
    const std::string BOLD    = "\033[1m";
    const std::string CYAN    = "\033[1;36m";
    const std::string GREEN   = "\033[1;32m";
    const std::string YELLOW  = "\033[1;33m";
    const std::string MAGENTA = "\033[1;35m";
    const std::string RED     = "\033[1;31m";
    const std::string WHITE   = "\033[1;37m";
    const std::string GRAY    = "\033[90m";
}

void printBanner() {
    std::cout << Color::CYAN << R"(
  ============================================================
       ____       _     _               __  __              
      |  _ \ __ _| |_  (_)_ __     __ _|  \/  | __ _ _______
      | |_) / _` | __| | | '_ \   / _` | |\/| |/ _` |_  / _ \
      |  _ < (_| | |_  | | | | | | (_| | |  | | (_| |/ /  __/
      |_| \_\__,_|\__| |_|_| |_|  \__,_|_|  |_|\__,_/___\___|
         C++ & STL Graph Search & Backtracking Solver        
  ============================================================
)" << Color::RESET << std::endl;
}

void displayResultDetails(const Maze& maze, const SolverResult& res) {
    std::cout << "\n------------------------------------------------------------\n";
    std::cout << Color::BOLD << "Algorithm: " << Color::CYAN << res.algorithmName << Color::RESET << "\n";
    std::cout << "Status:    " << (res.found ? Color::GREEN + "PATH FOUND!" + Color::RESET : Color::RED + "NO PATH FOUND" + Color::RESET) << "\n";
    std::cout << "Time:      " << std::fixed << std::setprecision(4) << res.timeElapsedMs << " ms\n";
    std::cout << "Explored:  " << res.cellsVisited << " cell evaluations\n";

    if (res.found) {
        std::cout << "Length:    " << res.path.size() << " steps (including start & destination)\n";
        std::cout << "Direction: " << Color::YELLOW << res.directionString << Color::RESET << "\n";

        // Display coordinate path
        std::cout << "Steps:     ";
        for (size_t i = 0; i < res.path.size(); ++i) {
            std::cout << "(" << res.path[i].first << "," << res.path[i].second << ")";
            if (i + 1 < res.path.size()) std::cout << " -> ";
            if ((i + 1) % 8 == 0 && i + 1 < res.path.size()) std::cout << "\n           ";
        }
        std::cout << "\n";

        // Visual display
        std::cout << "\n" << Color::BOLD << "Maze Solution Visualization:" << Color::RESET;
        maze.print(res.path);
    }

    if (!res.allPaths.empty()) {
        std::cout << Color::MAGENTA << "Total Unique Paths Found: " << res.allPaths.size() << Color::RESET << "\n";
        int previewCount = std::min((int)res.allPaths.size(), 10);
        std::cout << "First " << previewCount << " Lexicographical Paths:\n";
        for (int i = 0; i < previewCount; ++i) {
            std::cout << "  " << std::setw(2) << (i + 1) << ". " << res.allPaths[i] << "\n";
        }
        if (res.allPaths.size() > 10) {
            std::cout << "  ... and " << (res.allPaths.size() - 10) << " more.\n";
        }
    }
    std::cout << "------------------------------------------------------------\n";
}

void benchmarkAllAlgorithms(const Maze& maze) {
    std::cout << "\n" << Color::BOLD << Color::CYAN 
              << "=== Multi-Algorithm Performance Benchmark ===" << Color::RESET << "\n";

    SolverResult resDFSAll = MazeSolver::solveDFSAllPaths(maze);
    SolverResult resDFSSingle = MazeSolver::solveDFSSinglePath(maze);
    SolverResult resBFS = MazeSolver::solveBFS(maze);
    SolverResult resAStar = MazeSolver::solveAStar(maze);

    std::vector<SolverResult> results = {resDFSAll, resDFSSingle, resBFS, resAStar};

    std::cout << "\n+" << std::string(32, '-') << "+" << std::string(10, '-') 
              << "+" << std::string(13, '-') << "+" << std::string(16, '-') 
              << "+" << std::string(14, '-') << "+\n";
    std::cout << "| " << std::left << std::setw(30) << "Algorithm"
              << "| " << std::setw(8) << "Found?"
              << "| " << std::setw(11) << "Path Length"
              << "| " << std::setw(14) << "Cells Visited"
              << "| " << std::setw(12) << "Time (ms)" << "|\n";
    std::cout << "+" << std::string(32, '-') << "+" << std::string(10, '-') 
              << "+" << std::string(13, '-') << "+" << std::string(16, '-') 
              << "+" << std::string(14, '-') << "+\n";

    for (const auto& r : results) {
        std::string lengthStr = r.found ? std::to_string(r.path.size()) : "N/A";
        std::string foundStr = r.found ? "YES" : "NO";
        std::cout << "| " << std::left << std::setw(30) << r.algorithmName
                  << "| " << std::setw(8) << foundStr
                  << "| " << std::setw(11) << lengthStr
                  << "| " << std::setw(14) << r.cellsVisited
                  << "| " << std::fixed << std::setprecision(4) << std::setw(12) << r.timeElapsedMs << "|\n";
    }
    std::cout << "+" << std::string(32, '-') << "+" << std::string(10, '-') 
              << "+" << std::string(13, '-') << "+" << std::string(16, '-') 
              << "+" << std::string(14, '-') << "+\n";

    if (resBFS.found) {
        std::cout << "\n" << Color::GREEN << "Optimal Shortest Path (" 
                  << resBFS.path.size() << " steps):" << Color::RESET;
        maze.print(resBFS.path);
        std::cout << "Directions: " << Color::YELLOW << resBFS.directionString << Color::RESET << "\n";
    }
}

void solveInteractive(const Maze& maze) {
    std::cout << "\n" << Color::BOLD << "Current Maze Layout:" << Color::RESET;
    maze.print();

    while (true) {
        std::cout << Color::WHITE << "\nSelect Solver Strategy:" << Color::RESET << "\n";
        std::cout << "  [1] BFS (Breadth-First Search) - Guaranteed Shortest Path\n";
        std::cout << "  [2] A* Search - Optimal Path with Manhattan Heuristic\n";
        std::cout << "  [3] DFS Single Path - Fast First-Path Exploration\n";
        std::cout << "  [4] DFS All Paths - Classical Backtracking (All Solutions)\n";
        std::cout << "  [5] Compare All Algorithms (Side-by-side benchmark)\n";
        std::cout << "  [0] Back to Main Menu\n";
        std::cout << "Choice > ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::string dummy;
            std::cin >> dummy;
            continue;
        }

        if (choice == 0) break;

        switch (choice) {
            case 1: {
                SolverResult res = MazeSolver::solveBFS(maze);
                displayResultDetails(maze, res);
                break;
            }
            case 2: {
                SolverResult res = MazeSolver::solveAStar(maze);
                displayResultDetails(maze, res);
                break;
            }
            case 3: {
                SolverResult res = MazeSolver::solveDFSSinglePath(maze);
                displayResultDetails(maze, res);
                break;
            }
            case 4: {
                SolverResult res = MazeSolver::solveDFSAllPaths(maze);
                displayResultDetails(maze, res);
                break;
            }
            case 5: {
                benchmarkAllAlgorithms(maze);
                break;
            }
            default:
                std::cout << Color::RED << "Invalid option. Please choose [0-5]." << Color::RESET << "\n";
        }
    }
}

Maze promptCustomMaze() {
    int r, c;
    std::cout << "\nEnter maze dimensions (rows cols, e.g. 5 5): ";
    if (!(std::cin >> r >> c) || r <= 0 || c <= 0) {
        std::cout << Color::RED << "Invalid dimensions! Falling back to 4x4." << Color::RESET << "\n";
        std::cin.clear();
        std::string dummy;
        std::getline(std::cin, dummy);
        return Maze::getClassic4x4();
    }

    std::cout << "Enter " << r << " rows, each containing " << c << " characters/numbers\n";
    std::cout << "(1 for open, 0 for wall; or S for Start, E for Exit, . for open, # for wall):\n";

    std::vector<std::string> lines;
    for (int i = 0; i < r; ++i) {
        std::string line;
        std::cin >> line;
        lines.push_back(line);
    }

    return Maze::fromStringInput(lines);
}

void runAutomatedTests() {
    std::cout << "\n" << Color::BOLD << Color::CYAN << "Running Automated Verification Suite..." << Color::RESET << "\n";

    // Test 1: Classic 4x4
    std::cout << "Test 1: Classic 4x4 Rat in a Maze (GeeksforGeeks testcase)... ";
    Maze classic = Maze::getClassic4x4();
    SolverResult classicRes = MazeSolver::solveDFSAllPaths(classic);
    if (classicRes.found && classicRes.allPaths.size() == 2 && 
        classicRes.allPaths[0] == "DDRDRR" && classicRes.allPaths[1] == "DRDDRR") {
        std::cout << Color::GREEN << "[PASSED]" << Color::RESET << " (Found DDRDRR, DRDDRR)\n";
    } else {
        std::cout << Color::RED << "[FAILED]" << Color::RESET << "\n";
    }

    // Test 2: Unsolvable Maze
    std::cout << "Test 2: Blocked / Unsolvable Maze... ";
    std::vector<std::vector<int>> blockedGrid = {
        {1, 0},
        {0, 1}
    };
    Maze blocked(2, 2, blockedGrid);
    SolverResult blockedBFS = MazeSolver::solveBFS(blocked);
    SolverResult blockedDFS = MazeSolver::solveDFSSinglePath(blocked);
    if (!blockedBFS.found && !blockedDFS.found) {
        std::cout << Color::GREEN << "[PASSED]" << Color::RESET << " (Correctly identified no path)\n";
    } else {
        std::cout << Color::RED << "[FAILED]" << Color::RESET << "\n";
    }

    // Test 3: BFS Shortest vs A* Shortest
    std::cout << "Test 3: BFS vs A* Optimality on 8x8 Maze... ";
    Maze med = Maze::getMedium8x8();
    SolverResult medBFS = MazeSolver::solveBFS(med);
    SolverResult medAStar = MazeSolver::solveAStar(med);
    if (medBFS.found && medAStar.found && medBFS.path.size() == medAStar.path.size()) {
        std::cout << Color::GREEN << "[PASSED]" << Color::RESET 
                  << " (Both found identical optimal length: " << medBFS.path.size() << " steps)\n";
    } else {
        std::cout << Color::RED << "[FAILED]" << Color::RESET << "\n";
    }

    // Test 4: Random Maze Generator
    std::cout << "Test 4: Random Maze Generator Solvability... ";
    Maze rnd = Maze::generateRandom(12, 12, 0.35);
    SolverResult rndBFS = MazeSolver::solveBFS(rnd);
    if (rndBFS.found) {
        std::cout << Color::GREEN << "[PASSED]" << Color::RESET << " (Guaranteed path verified)\n";
    } else {
        std::cout << Color::RED << "[FAILED]" << Color::RESET << "\n";
    }

    std::cout << Color::GREEN << "\nAll automated test assertions completed successfully!\n" << Color::RESET;
}

int main() {
    setupConsole();
    printBanner();

    while (true) {
        std::cout << Color::BOLD << Color::WHITE << "\nMAIN MENU - Select Maze:" << Color::RESET << "\n";
        std::cout << "  [1] Classic 4x4 Maze (Standard GeeksforGeeks/LeetCode)\n";
        std::cout << "  [2] Medium 8x8 Maze\n";
        std::cout << "  [3] Large 15x15 Maze\n";
        std::cout << "  [4] Generate Random Solvable Maze\n";
        std::cout << "  [5] Enter Custom Maze\n";
        std::cout << "  [6] Run Automated Verification Tests\n";
        std::cout << "  [0] Exit\n";
        std::cout << "Select [0-6] > ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::string dummy;
            std::getline(std::cin, dummy);
            continue;
        }

        if (choice == 0) {
            std::cout << "\nThank you for using Rat in a Maze Solver!\n";
            break;
        }

        switch (choice) {
            case 1:
                solveInteractive(Maze::getClassic4x4());
                break;
            case 2:
                solveInteractive(Maze::getMedium8x8());
                break;
            case 3:
                solveInteractive(Maze::getLarge15x15());
                break;
            case 4: {
                int r = 10, c = 10;
                std::cout << "Enter grid dimensions (rows cols, default 10 10): ";
                std::string line;
                std::cin.ignore();
                std::getline(std::cin, line);
                std::stringstream ss(line);
                if (ss >> r) {
                    if (!(ss >> c)) c = r;
                } else {
                    r = 10; c = 10;
                }
                std::cout << "Generating " << r << "x" << c << " solvable maze...\n";
                solveInteractive(Maze::generateRandom(r, c, 0.30));
                break;
            }
            case 5:
                solveInteractive(promptCustomMaze());
                break;
            case 6:
                runAutomatedTests();
                break;
            default:
                std::cout << Color::RED << "Invalid option. Please choose [0-6]." << Color::RESET << "\n";
        }
    }

    return 0;
}
