#include <complex>
#include <iostream>
#include <queue>
#include <set>
#include <vector>

#include "helpers.h"

#define MATRIX_SIZE 71
#define BYTES_TO_READ 1024

struct ComplexCompare {
    bool operator()(const std::complex<int>& a, const std::complex<int>& b) const {
        if (real(a) != real(b)) {
            return real(a) < real(b);
        }
        return imag(a) < imag(b);
    }
};

bool inBounds(const int &x, const int &y, const std::vector<std::vector<char>> &matrix) {
    return y >= 0 && y < matrix.size() && x >= 0 && x < matrix[0].size();
}

int simulate(const std::vector<std::vector<char>> &matrix) {
    std::vector directions = {std::complex(0, 1), std::complex(0, -1), std::complex(1, 0), std::complex(-1, 0)};
    std::set<std::complex<int>, ComplexCompare> seen;
    std::queue<std::pair<std::complex<int>, int>> q;
    q.push({std::complex(0, 0), 0});

    while (!q.empty()) {
        auto item = q.front();

        q.pop();

        if (item.first.real() == MATRIX_SIZE - 1 && item.first.imag() == MATRIX_SIZE - 1) {
            return item.second;
        }

        for (auto direction: directions) {
            auto newSpot = item.first + direction;

            if (!inBounds(newSpot.real(), newSpot.imag(), matrix)) continue;
            if (matrix[newSpot.imag()][newSpot.real()] == '#') continue;

            if (!seen.contains({newSpot})) {
                seen.insert({newSpot});
                q.push({newSpot, item.second + 1});
            }
        }
    }

    return -1;
}

int main() {
    std::vector matrix(MATRIX_SIZE, std::vector(MATRIX_SIZE, '.'));
    std::ifstream input = getStream("18.txt");
    std::string line;

    int count = 0;

    while (getline(input, line) && count < BYTES_TO_READ) {
        int x, y;

        sscanf(line.c_str(), "%d,%d", &x, &y);

        if (inBounds(x, y, matrix)) {
            matrix[y][x] = '#';
        }

        count++;
    }

    std::println("{}", simulate(matrix));

    while (getline(input, line)) {
        int x, y;

        sscanf(line.c_str(), "%d,%d", &x, &y);

        if (!inBounds(x, y, matrix)) continue;

        matrix[y][x] = '#';

        if (simulate(matrix) == -1) {
            std::println("{},{}", x, y);
            break;
        }
    }
}
