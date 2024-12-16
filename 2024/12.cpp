#include <complex>
#include <iostream>
#include <set>
#include "helpers.h"

typedef std::pair<int, int> direction;
typedef std::pair<int, int> point;

const std::complex<double> N = std::complex<double>(-1, 0);
const std::complex<double> S = std::complex<double>(1, 0);
const std::complex<double> E = std::complex<double>(0, 1);
const std::complex<double> W = std::complex<double>(0, -1);
const std::complex<double> NE = std::complex<double>(-1, 1);
const std::complex<double> NW = std::complex<double>(-1, -1);
const std::complex<double> SE = std::complex<double>(1, 1);
const std::complex<double> SW = std::complex<double>(1, -1);

std::complex<double> DIRECTIONS[] = {N, S, E, W};

struct ComplexComparator {
    bool operator()(const std::complex<double> &a, const std::complex<double> &b) const {
        if (a.real() != b.real()) {
            return a.real() < b.real();
        }

        return a.imag() < b.imag();
    }
};

struct Result {
    long area;
    long perimeter;

    Result(const long area, const long perimeter) : area(area), perimeter(perimeter) {}
};

bool is(char target, std::complex<double> point, const std::vector<std::vector<char>> &matrix) {
    const int row = static_cast<int>(point.real());
    const int col = static_cast<int>(point.imag());

    return row >= 0 && row < matrix.size() && col >= 0 && col < matrix[0].size() && matrix[row][col] == target;
}

// area, perimeter
Result dfs(
        std::complex<double> node,
        char target,
        std::vector<std::vector<char>> &matrix,
        std::set<std::complex<double>, ComplexComparator> &seen,
        std::set<std::complex<double>, ComplexComparator> &perimeter,
        std::set<std::complex<double>, ComplexComparator> &area
) {
    Result result(1, 0);

    const int row = static_cast<int>(node.real());
    const int col = static_cast<int>(node.imag());

    for (auto direction: DIRECTIONS) {
        auto newPoint = node + direction;

        if (!is(matrix[row][col], newPoint, matrix)) {
            result.perimeter++;
            perimeter.insert(newPoint);
            continue;
        }

        if (!seen.contains(newPoint)) {
            seen.insert(newPoint);
            area.insert(newPoint);
            auto value = dfs(newPoint, target, matrix, seen, perimeter, area);
            result.area += value.area;
            result.perimeter += value.perimeter;
        }
    }

    return result;
}

long long getSides(
        char target,
        std::set<std::complex<double>, ComplexComparator> &perimeter,
        const std::set<std::complex<double>, ComplexComparator> &area,
        const std::vector<std::vector<char>> &matrix
) {
    long long corners = 0;

    for (auto it = perimeter.begin(); it != perimeter.end(); ++it) {
        if (perimeter.contains(*it + S) && perimeter.contains(*it + E) && area.contains(*it + SE)) {
            corners++;
        }

        if (perimeter.contains(*it + S) && perimeter.contains(*it + W) && area.contains(*it + SW)) {
            corners++;
        }

        if (perimeter.contains(*it + N) && perimeter.contains(*it + E) && area.contains(*it + NE)) {
            corners++;
        }

        if (perimeter.contains(*it + N) && perimeter.contains(*it + W) && area.contains(*it + NW)) {
            corners++;
        }

        if (area.contains(*it + S) && area.contains(*it + E)) {
            corners++;
        }

        if (area.contains(*it + S) && area.contains(*it + W)) {
            corners++;
        }

        if (area.contains(*it + N) && area.contains(*it + E)) {
            corners++;
        }

        if (area.contains(*it + N) && area.contains(*it + W)) {
            corners++;
        }
    }

    return corners;
}

std::vector<long long> getCost(std::vector<std::vector<char>> &matrix) {
    std::set<std::complex<double>, ComplexComparator> seen;
    std::vector<long long> answer = {0, 0};

    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[0].size(); j++) {
            if (!seen.contains(std::complex<double>(i, j))) {
                seen.insert(std::complex<double>(i, j));
                std::set<std::complex<double>, ComplexComparator> area = {std::complex<double>(i, j)};
                std::set<std::complex<double>, ComplexComparator> perimeter;

                auto result = dfs(std::complex<double>(i, j), matrix[i][j], matrix, seen, perimeter, area);
                answer[0] += result.area * result.perimeter;

                for (auto point: area) {
                    for (auto direction: {NE, NW, SE, SW}) {
                        if (!area.contains(point + direction)) {
                            perimeter.insert(point + direction);
                        }
                    }
                }

                auto sides = getSides(matrix[i][j], perimeter, area, matrix);

                answer[1] += sides * result.area;
            }
        }
    }

    return answer;
}

int main() {
    std::ifstream file = getStream("12.txt");
    std::string line;
    std::vector<std::vector<char>> matrix;

    while (getline(file, line)) {
        std::vector<char> row;

        for (auto character: line) {
            row.push_back(character);
        }

        matrix.push_back(row);
    }

    std::vector<long long> cost;
    cost = getCost(matrix);
    std::println("Answers are: {}", cost);
}
