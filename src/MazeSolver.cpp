#include "MazeSolver.hpp"
#include <queue>
#include <stack>
#include <algorithm>
#include <cmath>
#include <limits>

// Lexicographical order: 'D', 'L', 'R', 'U'
const std::vector<MazeSolver::Direction> MazeSolver::DIRECTIONS = {
    {1, 0, 'D'},   // Down
    {0, -1, 'L'},  // Left
    {0, 1, 'R'},   // Right
    {-1, 0, 'U'}   // Up
};

std::string MazeSolver::pathToDirectionString(const std::vector<std::pair<int, int>>& path) {
    if (path.size() < 2) return "";
    std::string directions = "";
    for (size_t i = 1; i < path.size(); ++i) {
        int dr = path[i].first - path[i - 1].first;
        int dc = path[i].second - path[i - 1].second;
        if (dr == 1 && dc == 0) directions += 'D';
        else if (dr == -1 && dc == 0) directions += 'U';
        else if (dr == 0 && dc == 1) directions += 'R';
        else if (dr == 0 && dc == -1) directions += 'L';
    }
    return directions;
}

// -------------------------------------------------------------
// 1. DFS - Find All Paths (Classic Backtracking)
// -------------------------------------------------------------
void MazeSolver::dfsAllPathsHelper(
    const Maze& maze,
    int r, int c,
    std::vector<std::vector<bool>>& visited,
    std::string& currentPath,
    std::vector<std::string>& allPaths,
    int& cellsVisited
) {
    cellsVisited++;
    if (r == maze.exit.first && c == maze.exit.second) {
        allPaths.push_back(currentPath);
        return;
    }

    visited[r][c] = true;

    for (const auto& dir : DIRECTIONS) {
        int nextR = r + dir.dr;
        int nextC = c + dir.dc;

        if (maze.isWalkable(nextR, nextC) && !visited[nextR][nextC]) {
            currentPath.push_back(dir.code);
            dfsAllPathsHelper(maze, nextR, nextC, visited, currentPath, allPaths, cellsVisited);
            currentPath.pop_back(); // Backtrack
        }
    }

    visited[r][c] = false; // Backtrack
}

SolverResult MazeSolver::solveDFSAllPaths(const Maze& maze) {
    SolverResult result;
    result.algorithmName = "DFS (All Paths Backtracking)";

    auto startTime = std::chrono::high_resolution_clock::now();

    // Check if start or exit is blocked
    if (!maze.isWalkable(maze.start.first, maze.start.second) ||
        !maze.isWalkable(maze.exit.first, maze.exit.second)) {
        auto endTime = std::chrono::high_resolution_clock::now();
        result.timeElapsedMs = std::chrono::duration<double, std::milli>(endTime - startTime).count();
        return result;
    }

    std::vector<std::vector<bool>> visited(maze.rows, std::vector<bool>(maze.cols, false));
    std::string currentPath = "";

    dfsAllPathsHelper(maze, maze.start.first, maze.start.second, visited, currentPath, result.allPaths, result.cellsVisited);

    std::sort(result.allPaths.begin(), result.allPaths.end());

    if (!result.allPaths.empty()) {
        result.found = true;
        result.directionString = result.allPaths[0];

        // Reconstruct coordinate path for the first lexicographical solution
        int r = maze.start.first;
        int c = maze.start.second;
        result.path.push_back({r, c});
        for (char move : result.directionString) {
            if (move == 'D') r++;
            else if (move == 'U') r--;
            else if (move == 'R') c++;
            else if (move == 'L') c--;
            result.path.push_back({r, c});
        }
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    result.timeElapsedMs = std::chrono::duration<double, std::milli>(endTime - startTime).count();
    return result;
}

// -------------------------------------------------------------
// 2. DFS - Find Single Path (Early Exit)
// -------------------------------------------------------------
bool MazeSolver::dfsSinglePathHelper(
    const Maze& maze,
    int r, int c,
    std::vector<std::vector<bool>>& visited,
    std::vector<std::pair<int, int>>& currentPath,
    int& cellsVisited
) {
    cellsVisited++;
    visited[r][c] = true;
    currentPath.push_back({r, c});

    if (r == maze.exit.first && c == maze.exit.second) {
        return true;
    }

    for (const auto& dir : DIRECTIONS) {
        int nextR = r + dir.dr;
        int nextC = c + dir.dc;

        if (maze.isWalkable(nextR, nextC) && !visited[nextR][nextC]) {
            if (dfsSinglePathHelper(maze, nextR, nextC, visited, currentPath, cellsVisited)) {
                return true;
            }
        }
    }

    currentPath.pop_back(); // Backtrack
    return false;
}

SolverResult MazeSolver::solveDFSSinglePath(const Maze& maze) {
    SolverResult result;
    result.algorithmName = "DFS (Single Path Search)";

    auto startTime = std::chrono::high_resolution_clock::now();

    if (maze.isWalkable(maze.start.first, maze.start.second) &&
        maze.isWalkable(maze.exit.first, maze.exit.second)) {
        std::vector<std::vector<bool>> visited(maze.rows, std::vector<bool>(maze.cols, false));
        result.found = dfsSinglePathHelper(maze, maze.start.first, maze.start.second, visited, result.path, result.cellsVisited);
        if (result.found) {
            result.directionString = pathToDirectionString(result.path);
        }
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    result.timeElapsedMs = std::chrono::duration<double, std::milli>(endTime - startTime).count();
    return result;
}

// -------------------------------------------------------------
// 3. BFS - Guaranteed Shortest Path (Queue based)
// -------------------------------------------------------------
SolverResult MazeSolver::solveBFS(const Maze& maze) {
    SolverResult result;
    result.algorithmName = "BFS (Shortest Path)";

    auto startTime = std::chrono::high_resolution_clock::now();

    if (!maze.isWalkable(maze.start.first, maze.start.second) ||
        !maze.isWalkable(maze.exit.first, maze.exit.second)) {
        auto endTime = std::chrono::high_resolution_clock::now();
        result.timeElapsedMs = std::chrono::duration<double, std::milli>(endTime - startTime).count();
        return result;
    }

    std::queue<std::pair<int, int>> q;
    std::vector<std::vector<bool>> visited(maze.rows, std::vector<bool>(maze.cols, false));
    std::vector<std::vector<std::pair<int, int>>> parent(
        maze.rows, std::vector<std::pair<int, int>>(maze.cols, {-1, -1})
    );

    q.push(maze.start);
    visited[maze.start.first][maze.start.second] = true;
    bool reached = false;

    while (!q.empty()) {
        auto current = q.front();
        q.pop();
        result.cellsVisited++;

        if (current == maze.exit) {
            reached = true;
            break;
        }

        for (const auto& dir : DIRECTIONS) {
            int nr = current.first + dir.dr;
            int nc = current.second + dir.dc;

            if (maze.isWalkable(nr, nc) && !visited[nr][nc]) {
                visited[nr][nc] = true;
                parent[nr][nc] = current;
                q.push({nr, nc});
            }
        }
    }

    if (reached) {
        result.found = true;
        // Reconstruct path backwards from exit to start
        std::pair<int, int> curr = maze.exit;
        while (curr != std::make_pair(-1, -1)) {
            result.path.push_back(curr);
            curr = parent[curr.first][curr.second];
        }
        std::reverse(result.path.begin(), result.path.end());
        result.directionString = pathToDirectionString(result.path);
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    result.timeElapsedMs = std::chrono::duration<double, std::milli>(endTime - startTime).count();
    return result;
}

// -------------------------------------------------------------
// 4. A* Search Algorithm - Optimal Path with Manhattan Distance
// -------------------------------------------------------------
namespace {
    struct AStarNode {
        int r, c;
        int g; // Cost from start
        int h; // Heuristic estimate to exit
        int f; // f = g + h

        bool operator>(const AStarNode& other) const {
            if (f != other.f) return f > other.f;
            return h > other.h; // tie breaker towards target
        }
    };

    inline int manhattanDistance(int r1, int c1, int r2, int c2) {
        return std::abs(r1 - r2) + std::abs(c1 - c2);
    }
}

SolverResult MazeSolver::solveAStar(const Maze& maze) {
    SolverResult result;
    result.algorithmName = "A* Search (Manhattan Heuristic)";

    auto startTime = std::chrono::high_resolution_clock::now();

    if (!maze.isWalkable(maze.start.first, maze.start.second) ||
        !maze.isWalkable(maze.exit.first, maze.exit.second)) {
        auto endTime = std::chrono::high_resolution_clock::now();
        result.timeElapsedMs = std::chrono::duration<double, std::milli>(endTime - startTime).count();
        return result;
    }

    const int INF = std::numeric_limits<int>::max();
    std::vector<std::vector<int>> gScore(maze.rows, std::vector<int>(maze.cols, INF));
    std::vector<std::vector<std::pair<int, int>>> parent(
        maze.rows, std::vector<std::pair<int, int>>(maze.cols, {-1, -1})
    );
    std::vector<std::vector<bool>> closedSet(maze.rows, std::vector<bool>(maze.cols, false));

    std::priority_queue<AStarNode, std::vector<AStarNode>, std::greater<AStarNode>> openSet;

    int startR = maze.start.first;
    int startC = maze.start.second;
    int exitR = maze.exit.first;
    int exitC = maze.exit.second;

    gScore[startR][startC] = 0;
    int hStart = manhattanDistance(startR, startC, exitR, exitC);
    openSet.push({startR, startC, 0, hStart, hStart});

    bool reached = false;

    while (!openSet.empty()) {
        AStarNode current = openSet.top();
        openSet.pop();

        if (closedSet[current.r][current.c]) continue;
        closedSet[current.r][current.c] = true;
        result.cellsVisited++;

        if (current.r == exitR && current.c == exitC) {
            reached = true;
            break;
        }

        for (const auto& dir : DIRECTIONS) {
            int nr = current.r + dir.dr;
            int nc = current.c + dir.dc;

            if (maze.isWalkable(nr, nc) && !closedSet[nr][nc]) {
                int tentativeG = current.g + 1;
                if (tentativeG < gScore[nr][nc]) {
                    gScore[nr][nc] = tentativeG;
                    parent[nr][nc] = {current.r, current.c};
                    int h = manhattanDistance(nr, nc, exitR, exitC);
                    openSet.push({nr, nc, tentativeG, h, tentativeG + h});
                }
            }
        }
    }

    if (reached) {
        result.found = true;
        std::pair<int, int> curr = maze.exit;
        while (curr != std::make_pair(-1, -1)) {
            result.path.push_back(curr);
            curr = parent[curr.first][curr.second];
        }
        std::reverse(result.path.begin(), result.path.end());
        result.directionString = pathToDirectionString(result.path);
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    result.timeElapsedMs = std::chrono::duration<double, std::milli>(endTime - startTime).count();
    return result;
}
