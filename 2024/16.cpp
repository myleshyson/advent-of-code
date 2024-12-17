#include "helpers.h"
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <set>

std::vector<std::vector<int>> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

int findShortestPath(std::vector<std::string> &content, std::vector<int> start, std::vector<int> initialDirection) {
  int rows = content.size();
  int cols = content[0].size();

  using MinHeap = std::priority_queue<
      std::vector<int>, std::vector<std::vector<int>>,
      std::function<bool(const std::vector<int> &, const std::vector<int> &)>>;

  // Define the custom comparator lambda
  auto comparator = [](const std::vector<int> &a, const std::vector<int> &b) {
    return a[0] >
           b[0]; // Min-heap comparator: smallest element has highest priority
  };

  MinHeap priorityQueue(comparator);
  std::set<std::vector<int>> seen;

  priorityQueue.push({0, start[0], start[1], initialDirection[0], initialDirection[1]});

  int minScore = INT_MAX;
  std::set<std::pair<int, int>> nodesInMinPath;

  while (!priorityQueue.empty()) {
    auto queueItem = priorityQueue.top();
    priorityQueue.pop();
    int score = queueItem[0];
    int row = queueItem[1];
    int col = queueItem[2];
    int dr = queueItem[3];
    int dc = queueItem[4];

    if (score > minScore) continue;

    if (content[row][col] == 'E') {
      return score;
    }

    if (seen.contains({row, col, dr, dc})) continue;

    seen.insert({row, col, dr, dc});

    for (auto &direction : directions) {
      int newRow = row + direction[0];
      int newCol = col + direction[1];

      if (newRow < 0 || newRow >= rows || newCol < 0 || newCol >= cols) continue;
      if (content[newRow][newCol] == '#') continue;
      if (direction[0] == -dr && direction[1] == -dc) continue;

      int pointsToAdd = (dr == direction[0] && dc == direction[1]) ? 1 : 1001;

      priorityQueue.push({score + pointsToAdd, newRow, newCol, direction[0], direction[1]});
    }
  }

  return -1;
}

int main() {
  std::ifstream input = getStream("16.txt");
  std::string line;
  std::vector<std::string> content;

  while (getline(input, line)) {
    content.push_back(line);
  }

  std::vector<std::string> copy = content;
  std::set<std::vector<int>> seen;
  auto minScore = findShortestPath(content, {static_cast<int>(content.size()) - 2, 1}, {0, 1});
  int rows = content.size();
  int cols = content[0].size();

  std::queue<std::vector<int>> queue;

  queue.push({static_cast<int>(content.size()) - 2, 1, 0, 0, 1});

  while (!queue.empty()) {
    auto item = queue.front();
    queue.pop();
    int row = item[0];
    int col = item[1];
    int score = item[2];
    int dr = item[3];
    int dc = item[4];

    if (seen.contains({row, col})) continue;
    if (findShortestPath(content, {row, col}, {dr, dc}) + score == minScore) {
      seen.insert({row, col});
    } else {
      continue;
    }

    for (auto direction : directions) {
      int newRow = row + direction[0];
      int newCol = col + direction[1];

      if (newRow < 0 || newRow >= rows || newCol < 0 || newCol >= cols) continue;
      if (content[newRow][newCol] == '#') continue;
      if (direction[0] == -dr && direction[1] == -dc) continue;
      if (seen.contains({newRow, newCol})) continue;

      int pointsToAdd = (dr == direction[0] && dc == direction[1]) ? 1 : 1001;
      queue.push({newRow, newCol, score + pointsToAdd, direction[0], direction[1]});
    }
  }

  std::cout << minScore << std::endl;
  std::cout << seen.size() << std::endl;
}