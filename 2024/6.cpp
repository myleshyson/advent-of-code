#include <sstream>
#include <map>
#include <set>
#include <utility>
#include <vector>
#include <iostream>
#include <future>
#include <optional>
#include "helpers.h"

typedef std::pair<int, int> position;
typedef std::pair<int, int> direction;
typedef std::vector<std::vector<char> > content;
typedef std::set<position> visited;
typedef std::map<position, position> skip;
typedef std::map<std::pair<int, int>, std::vector<std::pair<int, int> > > graph;

struct Result {
    std::optional<visited> visited;
    explicit Result(std::optional<::visited> visited): visited(std::move(visited)) {};
};

const std::pair<int, int> LEFT = std::make_pair(0, -1);
const std::pair<int, int> RIGHT = std::make_pair(0, 1);
const std::pair<int, int> UP = std::make_pair(-1, 0);
const std::pair<int, int> DOWN = std::make_pair(1, 0);

bool inBounds(const std::pair<int, int> position, const std::vector<std::vector<char> > &content) {
    return position.first >= 0 && position.first < content.size() && position.second >= 0 && position.second < content[
               0].size();
}

bool isBoundry(const std::pair<int, int> position, const std::vector<std::vector<char> > &content) {
    return content[position.first][position.second] == '#';
}

direction getNewDirection(const direction &direction) {
    if (direction == UP) return RIGHT;
    if (direction == DOWN) return LEFT;
    if (direction == RIGHT) return DOWN;
    if (direction == LEFT) return UP;

    return direction;
}

position getNewPosition(const position &position, const direction &direction) {
    return std::make_pair(position.first + direction.first, position.second + direction.second);
}

Result simulateRoute(position position, const content &content, std::optional<::position> newObstruction = std::nullopt) {
    visited seen;
    direction direction = UP;
    std::set<std::pair<::position, ::direction> > pointDirectionMap;

    while (inBounds(position, content)) {
        if (pointDirectionMap.contains(std::make_pair(position, direction))) return Result(std::nullopt);

        seen.insert(position);
        pointDirectionMap.emplace(position, direction);

        ::position newPosition = getNewPosition(position, direction);

        if (inBounds(newPosition, content) && isBoundry(newPosition, content)) {
            direction = getNewDirection(direction);
            continue;
        }

        if (newObstruction && newPosition == newObstruction) {
            direction = getNewDirection(direction);
            continue;
        }

        position = newPosition;
    }

    return Result(seen);
}

int main() {
    std::ifstream file = getStream("6.txt");
    std::string line;
    content content;
    graph graph;
    position initialPosition;
    skip skipMap;

    while (getline(file, line)) {
        std::vector<char> characters;

        for (int i = 0; i < line.size(); i++) {
            characters.push_back(line[i]);

            if (line[i] == '^') {
                initialPosition = std::make_pair(content.size(), i);
            }
        }

        content.push_back(characters);
    }

    int answer[] = {0, 0};
    auto thread = std::async(simulateRoute, initialPosition, std::ref(content), std::nullopt);

    if (thread.valid()) {
        auto originalPath = thread.get().visited.value();
        answer[0] = originalPath.size();
        std::vector<std::future<Result>> threads;

        for (auto point: originalPath) {
            if (point == initialPosition) continue;
            threads.push_back(std::async(simulateRoute, initialPosition, content, point));
        }

        for (auto &task: threads) {
            if (task.valid()) {
                auto result = task.get();
                if (result.visited == std::nullopt) answer[1]++;
            }
        }
    }


    std::println("{}", answer);

    return 0;
}
