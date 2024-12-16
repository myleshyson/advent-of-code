#include <algorithm>
#include <sstream>
#include <iostream>
#include <map>
#include <set>
#include <complex>
#include <ranges>
#include "helpers.h"

struct ComplexCompare {
    bool operator()(const std::complex<double>& a, const std::complex<double>& b) const {
        if (a.real() != b.real()) {
            return a.real() < b.real();
        }
        return a.imag() < b.imag();
    }
};

bool inBounds(const std::complex<double> &point, const std::vector<std::string> &grid) {
    if (grid.empty() || grid[0].empty()) return false;
    const int row = static_cast<int>(point.imag());
    const int col = static_cast<int>(point.real());

    return row >= 0 && row < grid.size() && col >= 0 && col < grid[0].size();
}

int main() {
    std::ifstream file = getStream("8.txt");
    std::string line;
    std::vector<std::string> grid;
    std::map<char, std::vector<std::complex<double>>> antennaMap;

    while (getline(file, line)) {
        for (int i = 0; i < line.size(); i++) {
            if (line[i] != '.') {
                antennaMap[line[i]].emplace_back(i, grid.size());
            }
        }
        grid.push_back(line);
    }

    std::set<std::complex<double>, ComplexCompare> answer[2];

    for (const auto &points: antennaMap | std::views::values) {
        for (auto point: points) {
            for (auto otherPoint: points) {
                if (point == otherPoint) continue;
                std::complex<double> distance = point - otherPoint;

                for (auto antinode : {point, otherPoint}) {
                    std::complex<double> one = antinode + distance;
                    std::complex<double> two = antinode - distance;

                    if (inBounds(one, grid) && one != point && one != otherPoint) answer[0].insert(one);
                    if (inBounds(two, grid) && two != point && two != otherPoint) answer[0].insert(two);

                    while (inBounds(one, grid) || inBounds(two, grid)) {
                        if (inBounds(one, grid)) answer[1].insert(one);
                        if (inBounds(two, grid)) answer[1].insert(two);
                        one = one + distance;
                        two = two - distance;
                    }
                }
            }
        }
    }

    std::cout << answer[0].size() << std::endl;
    std::cout << answer[1].size() << std::endl;

    return 0;
}
