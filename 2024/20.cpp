#include <format>
#include <fstream>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <valarray>

#include "helpers.h"

std::vector<std::vector<int>> directions({{0, 1}, {0, -1}, {1, 0}, {-1, 0}});

struct Point {
    int row;
    int col;

    Point(const int row, const int col) : row(row), col(col) {}
    Point() : row(0), col(0) {}
    Point operator+(const Point &other) const { return {row + other.row, col + other.col}; }
    Point operator+(const std::vector<int> &other) const { return {row + other[0], col + other[1]}; }
    auto operator<=>(const Point &) const = default;

    [[nodiscard]] std::string toString() const { return "(" + std::to_string(row) + "," + std::to_string(col) + ")"; }
};

// Add an `operator<<` for `Point`
std::ostream &operator<<(std::ostream &os, const Point &point) {
    os << "(" << point.row << ", " << point.col << ")";
    return os;
}

template<>
struct std::hash<Point> {
    std::size_t operator()(const Point &p) const noexcept {
        std::size_t h1 = std::hash<int>{}(p.row);
        std::size_t h2 = std::hash<int>{}(p.col);
        return h1 + 0x9e3779b9 + (h2 << 6) + (h2 >> 2);
    }
};

bool inBounds(const Point &point, const std::vector<std::string> &content) {
    return point.row >= 0 && point.row < content.size() && point.col >= 0 && point.col < content[0].size();
}

int getManhattanDistance(const Point &p1, const Point &p2) {
    return std::abs(p1.row - p2.row) + std::abs(p1.col - p2.col);
}

int getAnswer(
        const int &maxDistance,
        const int &minSave,
        const std::vector<Point> &path,
        const std::unordered_map<Point, int> &distances
) {
    int answer = 0;
    
    // Precompute vector sizes to avoid reallocations
    #pragma omp parallel for reduction(+:answer)
    for (const auto &point: path) {
        const auto firstDist = distances.at(point);

        // Skip points that are too far along to save enough steps
        if (firstDist > distances.size() - minSave) continue;

        // Generate and check points in one pass without storing them
        for (int dist = 1; dist <= maxDistance; dist++) {
            for (int dx = -dist; dx <= dist; dx++) {
                int dy = dist - std::abs(dx);
                if (dy < 0) continue;

                // Process first point
                auto p1 = Point(point.row + dx, point.col + dy);
                if (distances.contains(p1)) {
                    const auto secondDist = distances.at(p1);
                    const int difference = secondDist - firstDist;
                    if (difference - dist >= minSave) answer++;
                }

                // Process second point if needed
                if (dy != 0) {
                    auto p2 = Point(point.row + dx, point.col - dy);
                    if (distances.contains(p2)) {
                        const auto secondDist = distances.at(p2);
                        const int difference = secondDist - firstDist;
                        if (difference - dist >= minSave) answer++;
                    }
                }
            }
        }
    }

    return answer;
}

int main() {
    std::ifstream input = getStream("20.txt");
    std::string line;
    std::vector<std::string> content;
    Point start;
    Point end;
    std::unordered_map<Point, char> grid;

    while (getline(input, line)) {
        for (int i = 0; i < line.size(); i++) {
            if (line[i] != '#') grid[{static_cast<int>(content.size()), i}] = line[i];
            if (line[i] == 'S') {
                start = {static_cast<int>(content.size()), i};
            }
            if (line[i] == 'E') {
                end = {static_cast<int>(content.size()), i};
            }
        }
        content.push_back(line);
    }

    std::queue<Point> queue;
    std::unordered_map<Point, int> distances;
    std::vector<Point> path;

    path.push_back(start);
    distances.insert_or_assign(start, 0);
    queue.push(start);

    while (!queue.empty()) {
        auto point = queue.front();
        queue.pop();

        if (point == end) break;

        for (const auto &direction: directions) {
            auto newPoint = point + direction;
            if (!inBounds(newPoint, content)) continue;
            if (!grid.contains(newPoint)) continue;
            if (distances.contains(newPoint)) continue;
            distances[newPoint] = distances[point] + 1;
            queue.push(newPoint);
            path.push_back(newPoint);
        }
    }

    std::vector answer = {0, 0};
    std::unordered_map<Point, std::vector<Point>> graph;

    std::println("{}", getAnswer(2, 100, path, distances));
    std::println("{}", getAnswer(20, 100, path, distances));
}
