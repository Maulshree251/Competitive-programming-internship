#ifndef MAZE_HPP
#define MAZE_HPP

#include <vector>
#include <string>
#include <utility>
#include <iostream>

class Maze {
public:
    int rows;
    int cols;
    std::pair<int, int> start;
    std::pair<int, int> exit;
    std::vector<std::vector<int>> grid; // 1 = open, 0 = wall

    Maze();
    Maze(int r, int c, const std::vector<std::vector<int>>& customGrid,
         std::pair<int, int> startPos = {0, 0},
         std::pair<int, int> exitPos = {-1, -1});

    bool isValid(int r, int c) const;
    bool isWalkable(int r, int c) const;

    // Display functions
    void print(const std::vector<std::pair<int, int>>& path = {}, bool useColor = true) const;

    // Factory methods for preset mazes
    static Maze getClassic4x4();
    static Maze getMedium8x8();
    static Maze getLarge15x15();
    static Maze generateRandom(int r, int c, double wallDensity = 0.28);
    static Maze fromStringInput(const std::vector<std::string>& lines);
};

#endif // MAZE_HPP
