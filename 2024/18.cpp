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
    std::vector<std::vector<int>> bytes;
    std::ifstream input = getStream("18.txt");
    std::string line;

    while (getline(input, line)) {
        int x, y;

        sscanf(line.c_str(), "%d,%d", &x, &y);

        if (inBounds(x, y, matrix)) {
            bytes.push_back({x, y});
        }
    }

    for (int i = 0; i < BYTES_TO_READ; i++) {
        matrix[bytes[i][1]][bytes[i][0]] = '#';
    }

    std::println("{}", simulate(matrix));

    int left = BYTES_TO_READ;
    int right = bytes.size() - 1;
    int mid;

    while (right - left > 1) {
        mid = (left + right) / 2;

        matrix = std::vector(MATRIX_SIZE, std::vector(MATRIX_SIZE, '.'));

        for (int i = 0; i <= mid; i++) {
            matrix[bytes[i][1]][bytes[i][0]] = '#';
        }

        if (simulate(matrix) != -1) {
            left = mid;
        }
        else {
            right = mid;
        }
    }

    std::println("{}", bytes[right]);

}
