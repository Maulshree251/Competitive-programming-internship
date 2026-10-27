#include "Maze.hpp"
#include <iomanip>
#include <random>
#include <algorithm>
#include <set>

Maze::Maze() : rows(0), cols(0), start({0, 0}), exit({0, 0}) {}

Maze::Maze(int r, int c, const std::vector<std::vector<int>>& customGrid,
           std::pair<int, int> startPos,
           std::pair<int, int> exitPos)
    : rows(r), cols(c), start(startPos), grid(customGrid) {
    if (exitPos.first == -1 && exitPos.second == -1) {
        exit = {r - 1, c - 1};
    } else {
        exit = exitPos;
    }
}

bool Maze::isValid(int r, int c) const {
    return (r >= 0 && r < rows && c >= 0 && c < cols);
}

bool Maze::isWalkable(int r, int c) const {
    return isValid(r, c) && (grid[r][c] == 1);
}

void Maze::print(const std::vector<std::pair<int, int>>& path, bool useColor) const {
    // Put path coordinates in a fast-lookup set
    std::set<std::pair<int, int>> pathSet(path.begin(), path.end());

    // ANSI escape sequences
    const std::string RESET = "\033[0m";
    const std::string BOLD = "\033[1m";
    const std::string GREEN = "\033[1;32m";
    const std::string YELLOW = "\033[1;33m";
    const std::string CYAN = "\033[1;36m";
    const std::string RED = "\033[31m";
    const std::string GRAY = "\033[90m";

    std::cout << "\n    ";
    for (int c = 0; c < cols; ++c) {
        std::cout << std::setw(3) << c;
    }
    std::cout << "\n   +" << std::string(cols * 3 + 1, '-') << "+\n";

    for (int r = 0; r < rows; ++r) {
        std::cout << std::setw(2) << r << " | ";
        for (int c = 0; c < cols; ++c) {
            std::pair<int, int> pos = {r, c};
            if (pos == start) {
                if (useColor) std::cout << GREEN << " S " << RESET;
                else std::cout << " S ";
            } else if (pos == exit) {
                if (useColor) std::cout << YELLOW << " E " << RESET;
                else std::cout << " E ";
            } else if (pathSet.count(pos)) {
                if (useColor) std::cout << CYAN << " * " << RESET;
                else std::cout << " * ";
            } else if (grid[r][c] == 0) {
                if (useColor) std::cout << GRAY << "###" << RESET;
                else std::cout << "###";
            } else {
                std::cout << " . ";
            }
        }
        std::cout << "|\n";
    }

    std::cout << "   +" << std::string(cols * 3 + 1, '-') << "+\n";
    std::cout << "Legend: "
              << (useColor ? GREEN + "S" + RESET : "S") << " = Start (Rat), "
              << (useColor ? YELLOW + "E" + RESET : "E") << " = Exit (Cheese), "
              << (useColor ? CYAN + "*" + RESET : "*") << " = Solution Path, "
              << (useColor ? GRAY + "###" + RESET : "###") << " = Obstacle/Wall, "
              << ". = Open Cell\n\n";
}

Maze Maze::getClassic4x4() {
    // Standard GeeksforGeeks / LeetCode classical Rat in a Maze
    // 1: Open, 0: Wall
    std::vector<std::vector<int>> g = {
        {1, 0, 0, 0},
        {1, 1, 0, 1},
        {1, 1, 0, 0},
        {0, 1, 1, 1}
    };
    return Maze(4, 4, g, {0, 0}, {3, 3});
}

Maze Maze::getMedium8x8() {
    std::vector<std::vector<int>> g = {
        {1, 1, 1, 0, 1, 1, 1, 1},
        {0, 0, 1, 0, 1, 0, 0, 1},
        {1, 1, 1, 1, 1, 0, 1, 1},
        {1, 0, 0, 0, 0, 0, 1, 0},
        {1, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 1, 0, 0, 0, 1, 1},
        {1, 1, 1, 0, 1, 1, 0, 1},
        {1, 0, 1, 1, 1, 0, 1, 1}
    };
    return Maze(8, 8, g, {0, 0}, {7, 7});
}

Maze Maze::getLarge15x15() {
    std::vector<std::vector<int>> g = {
        {1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1},
        {1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1},
        {0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1},
        {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1},
        {1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1},
        {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
        {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1},
        {0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1}
    };
    return Maze(15, 15, g, {0, 0}, {14, 14});
}

Maze Maze::generateRandom(int r, int c, double wallDensity) {
    if (r < 2) r = 2;
    if (c < 2) c = 2;

    std::vector<std::vector<int>> g(r, std::vector<int>(c, 1));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            if (dis(gen) < wallDensity) {
                g[i][j] = 0;
            }
        }
    }

    // Ensure start and exit are always open
    g[0][0] = 1;
    g[r - 1][c - 1] = 1;

    // Carve a guaranteed path from (0,0) to (r-1, c-1) to ensure solvability
    int currR = 0, currC = 0;
    while (currR < r - 1 || currC < c - 1) {
        g[currR][currC] = 1;
        if (currR == r - 1) {
            currC++;
        } else if (currC == c - 1) {
            currR++;
        } else {
            // Randomly move Down or Right
            if (dis(gen) < 0.5) currR++;
            else currC++;
        }
    }
    g[r - 1][c - 1] = 1;

    return Maze(r, c, g, {0, 0}, {r - 1, c - 1});
}

Maze Maze::fromStringInput(const std::vector<std::string>& lines) {
    int r = static_cast<int>(lines.size());
    if (r == 0) return Maze();
    int c = static_cast<int>(lines[0].size());

    std::vector<std::vector<int>> g(r, std::vector<int>(c, 1));
    std::pair<int, int> startPos = {0, 0};
    std::pair<int, int> exitPos = {r - 1, c - 1};

    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c && j < static_cast<int>(lines[i].size()); ++j) {
            char ch = lines[i][j];
            if (ch == '0' || ch == '#' || ch == 'X' || ch == 'x') {
                g[i][j] = 0;
            } else if (ch == 'S' || ch == 's') {
                g[i][j] = 1;
                startPos = {i, j};
            } else if (ch == 'E' || ch == 'e') {
                g[i][j] = 1;
                exitPos = {i, j};
            } else {
                g[i][j] = 1;
            }
        }
    }

    return Maze(r, c, g, startPos, exitPos);
}
