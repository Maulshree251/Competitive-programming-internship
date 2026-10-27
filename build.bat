@echo off
echo ========================================================
echo Building Rat in a Maze Solver (C++ / MinGW / GCC)
echo ========================================================

g++ -std=c++14 -O2 -Iinclude src\Maze.cpp src\MazeSolver.cpp src\main.cpp -o rat_in_maze.exe

if %ERRORLEVEL% EQU 0 (
    echo.
    echo [SUCCESS] Compilation succeeded! Created rat_in_maze.exe
    echo Run it using: .\rat_in_maze.exe
) else (
    echo.
    echo [ERROR] Compilation failed.
)
