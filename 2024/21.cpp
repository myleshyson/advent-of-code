#include "helpers.h"

std::vector<std::string> numPad = {"789", "456", "123", "_0A"};

std::vector<std::string> arrowPad = {"_^A", "<v>"};

std::vector<std::vector<int>> directions({{0, 1}, {-1, 0}, {1, 0}, {0, -1}});

std::map<std::vector<int>, char> directionMap = {{{0, 1}, '>'}, {{0, -1}, '<'}, {{1, 0}, 'v'}, {{-1, 0}, '^'}};

struct Point {
    int row;
    int col;

    Point(const int row, const int col) : row(row), col(col) {}
    Point() : row(0), col(0) {}
    Point operator+(const Point& other) const { return {row + other.row, col + other.col}; }
    Point operator+(const std::vector<int>& other) const { return {row + other[0], col + other[1]}; }
    auto operator<=>(const Point&) const = default;

    [[nodiscard]] std::string toString() const { return "(" + std::to_string(row) + "," + std::to_string(col) + ")"; }
};

std::vector<std::vector<std::string>> cartesian_product_iterative(const std::vector<std::vector<std::string>>& vectors
) {
    std::vector<std::vector<std::string>> result = {{}};
    for (const auto& v: vectors) {
        std::vector<std::vector<std::string>> tempResult;
        for (const auto& res: result) {
            for (const auto& elem: v) {
                auto temp = res;
                temp.push_back(elem);
                tempResult.push_back(temp);
            }
        }
        result.swap(tempResult);
    }
    return result;
}

std::vector<std::string> getOptions(
        std::string sequence, std::map<std::pair<char, char>, std::vector<std::string>>& seqs
) {
    sequence = "A" + sequence;
    std::vector<std::vector<std::string>> options;

    for (int i = 0; i < sequence.size(); i++) {
        if (i + 1 >= sequence.size()) break;
        options.push_back(seqs[{sequence[i], sequence[i + 1]}]);
    }

    auto results = cartesian_product_iterative(options);

    std::vector<std::string> strings;

    for (const auto& result: results) {
        std::string option;

        for (const auto& part: result) {
            option += part;
        }
        strings.push_back(option);
    }

    return strings;
}

std::map<std::pair<char, char>, std::vector<std::string>> getSeqMap(const std::vector<std::string>& pad) {
    std::unordered_map<char, Point> padMap;

    for (int i = 0; i < pad.size(); i++) {
        for (int j = 0; j < pad[0].size(); j++) {
            if (pad[i][j] == '_') continue;
            padMap[pad[i][j]] = Point(i, j);
        }
    }

    std::map<std::pair<char, char>, std::vector<std::string>> seqs;

    for (const auto& pair: padMap) {
        for (const auto& otherPair: padMap) {
            if (pair.first == otherPair.first) {
                seqs[std::pair(pair.first, otherPair.first)].emplace_back("A");
                continue;
            }
            std::vector<std::string> possibilities;
            std::queue<std::pair<Point, std::string>> queue;
            queue.emplace(padMap[pair.first], "");

            int max = INT_MAX;

            while (!queue.empty()) {
                auto [current, string] = queue.front();
                queue.pop();

                if (string.size() > max) continue;

                if (pad[current.row][current.col] == otherPair.first) {
                    possibilities.push_back(string + "A");
                    max = std::min(static_cast<int>(string.size()), max);
                    continue;
                }

                for (const auto& direction: directions) {
                    auto newPosition = current + direction;

                    // if not in bounds, continue
                    if (newPosition.row < 0 || newPosition.row >= pad.size() || newPosition.col < 0 ||
                        newPosition.col >= pad[0].size())
                        continue;
                    if (pad[newPosition.row][newPosition.col] == '_') continue;
                    if (string.size() + 1 > max) break;

                    queue.emplace(newPosition, string + directionMap[direction]);
                }
            }
            seqs[{pair.first, otherPair.first}] = possibilities;
        }
    }

    return seqs;
}

std::map<std::tuple<char, char, int>, long long> cache;
long long computeLength(char x, char y, int depth, std::map<std::pair<char, char>, std::vector<std::string>>& seqs) {
    if (depth == 1) return static_cast<long long>(seqs[{x, y}][0].size());

    if (cache.contains({x, y, depth})) {
        return cache[{x, y, depth}];
    }

    long long optimal = LONG_LONG_MAX;

    for (const auto& seq: seqs[{x, y}]) {
        long long length = 0;
        std::string expanded = "A" + seq;

        for (int i = 0; i < expanded.size(); i++) {
            if (i + 1 >= expanded.size()) break;
            length += computeLength(expanded[i], expanded[i + 1], depth - 1, seqs);
        }

        optimal = std::min(optimal, length);
    }

    cache[{x, y, depth}] = optimal;

    return optimal;
}

int main() {
    std::ifstream file = getStream("21.txt");
    std::string line;
    std::vector<long long> answer = {0, 0};

    auto numPadSeqs = getSeqMap(numPad);
    auto arrowPadSeqs = getSeqMap(arrowPad);

    while (std::getline(file, line)) {
        auto inputs = getOptions(line, numPadSeqs);
        long long firstThree = std::stoll(line.substr(0, 3));

        for (int i = 0; i < 2; i++) {
            int depth = i == 0 ? 2 : 25;
            long long optimal = LONG_LONG_MAX;

            for (const auto& seq: inputs) {
                long long length = 0;
                std::string expanded = "A" + seq;
                for (int i = 0; i < expanded.size(); i++) {
                    if (i + 1 >= expanded.size()) break;
                    length += computeLength(expanded[i], expanded[i + 1], depth, arrowPadSeqs);
                }

                optimal = std::min(length, optimal);
            }

            answer[i] += firstThree * optimal;
        }
    }

    std::println("{}", answer);
}
