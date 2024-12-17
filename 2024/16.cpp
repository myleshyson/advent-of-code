#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <queue>
#include <unordered_set>
#include <utility>
#include <vector>
#include "helpers.h"

std::vector<std::vector<int>> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

struct VectorHash {
    std::size_t operator()(const std::vector<int> &vec) const {
        std::size_t seed = 0;
        for (const int num: vec) {
            // Combine the current hash with the number's hash
            seed ^= std::hash<int>()(num) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

// Define a custom equality function for std::vector<int> (optional, but makes explicit how vectors are compared)
struct VectorEqual {
    bool operator()(const std::vector<int> &a, const std::vector<int> &b) const {
        return a == b; // Vectors are equal if their elements are equal
    }
};

struct QueueItem {
    int score;
    int row;
    int col;
    int dr;
    int dc;
    std::shared_ptr<const QueueItem> parent;

    QueueItem(
            const int score,
            const int row,
            const int col,
            const int dr,
            const int dc,
            std::shared_ptr<const QueueItem> parent
    ) : score(score), row(row), col(col), dr(dr), dc(dc), parent(std::move(parent)) {}
};

std::vector<std::pair<int, int>> reconstructPath(std::shared_ptr<const QueueItem> item) {
    std::vector<std::pair<int, int>> path;
    while (item) {
        path.emplace_back(item->row, item->col);
        item = item->parent; // Traverse parent pointers
    }
    return path;
}

int main() {
    std::ifstream input = getStream("16.txt");
    std::string line;
    std::vector<std::string> content;

    while (getline(input, line)) {
        content.push_back(line);
    }

    using MinHeap = std::priority_queue<
            std::shared_ptr<QueueItem>, std::vector<std::shared_ptr<QueueItem>>,
            std::function<bool(const std::shared_ptr<QueueItem> &, const std::shared_ptr<QueueItem> &)>>;

    auto comparator = [](const std::shared_ptr<QueueItem> a, const std::shared_ptr<QueueItem> &b) {
        return a->score > b->score;
    };

    int rows = static_cast<int>(content.size());
    int cols = static_cast<int>(content[0].size());
    int minVal = INT_MAX;
    MinHeap priorityQueue(comparator);
    std::unordered_set<std::vector<int>, VectorHash, VectorEqual> seen;
    std::unordered_set<std::vector<int>, VectorHash, VectorEqual> good;
    QueueItem initial = {0, rows - 2, 1, 0, 1, nullptr};
    priorityQueue.push(std::make_shared<QueueItem>(initial));

    while (!priorityQueue.empty()) {
        auto item = priorityQueue.top();
        priorityQueue.pop();
        seen.insert({item->row, item->col, item->dr, item->dc});

        if (content[item->row][item->col] == 'E' && item->score <= minVal) {
            minVal = std::min(minVal, item->score);
            auto path = reconstructPath(item->parent);
            for (auto &pair: path) {
                good.insert({pair.first, pair.second});
            }
        }

        for (auto direction: directions) {
            int newRow = item->row + direction[0];
            int newCol = item->col + direction[1];

            if (newRow < 0 || newRow >= rows || newCol < 0 || newCol >= cols) continue;
            if (content[newRow][newCol] == '#') continue;
            if (direction[0] == -item->dr && direction[1] == -item->dc) continue;
            if (seen.contains({newRow, newCol, direction[0], direction[1]})) continue;
            int newScore = ((item->dr == direction[0] && item->dc == direction[1]) ? 1 : 1001) + item->score;
            QueueItem newItem({newScore, newRow, newCol, direction[0], direction[1], item});
            priorityQueue.push(std::make_shared<QueueItem>(newItem));
        }
    }

    std::cout << minVal << std::endl;
    // we don't add the destination in our loop, so that's why we have to add a 1 here.
    std::cout << good.size() + 1 << std::endl;
}
