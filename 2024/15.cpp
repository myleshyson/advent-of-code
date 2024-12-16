#include <fstream>
#include <map>
#include <iostream>

#include "helpers.h"

constexpr std::pair UP = {-1, 0};
constexpr std::pair DOWN = {1, 0};
constexpr std::pair LEFT = {0, -1};
constexpr std::pair RIGHT = {0, 1};

std::map<char, std::pair<int, int> > arrowMap = {
        {'^', UP},
        {'v', DOWN},
        {'>', RIGHT},
        {'<', LEFT}
};

struct Point {
    int row;
    int col;

    Point(const int row, const int col):
        row(row), col(col) {
    }

    Point():
        row(0), col(0) {
    }

    Point operator+(const Point &other) const {
        return {row + other.row, col + other.col};
    }

    Point operator+(const std::pair<int, int> &other) const {
        return {row + other.first, col + other.second};
    }
};

struct BigBox {
    Point left;
    Point right;

    BigBox(const Point left, const Point right):
        left(left), right(right) {
    }
};

void printMap(std::vector<std::string> content) {
    for (const auto &row: content) {
        std::cout << row << std::endl;
    }

    std::cout << std::endl;
}

int warehouseOne(std::vector<std::string> content, const std::string &directions) {
    std::pair<int, int> pos;

    // start position
    for (int i = 0; i < content.size(); i++) {
        for (int j = 0; j < content[i].size(); j++) {
            if (content[i][j] == '@') {
                pos = {i, j};
            }
        }
    }

    for (auto direction: directions) {
        auto change = arrowMap[direction];
        std::pair newPos = {pos.first + change.first, pos.second + change.second};

        if (content[newPos.first][newPos.second] == '.') {
            content[pos.first][pos.second] = '.';
            content[newPos.first][newPos.second] = '@';
            pos = newPos;
            continue;
        }

        if (content[newPos.first][newPos.second] == '#') {
            continue;
        }

        // box time
        std::vector boxes = {newPos};
        std::pair boxPos = newPos;

        while (content[boxPos.first][boxPos.second] == 'O') {
            boxPos = {boxPos.first + change.first, boxPos.second + change.second};
            if (content[boxPos.first][boxPos.second] == 'O') {
                boxes.push_back(boxPos);
            }
        }

        // they all run into a wall
        if (content[boxPos.first][boxPos.second] == '#') {
            continue;
        }

        for (int i = static_cast<int>(boxes.size()) - 1; i >= 0; i--) {
            auto box = boxes[i];
            content[box.first][box.second] = '.';
            content[box.first + change.first][box.second + change.second] = 'O';
        }

        content[pos.first][pos.second] = '.';
        content[newPos.first][newPos.second] = '@';
        pos = newPos;
    }

    int answer = 0;

    for (int i = 0; i < content.size(); i++) {
        for (int j = 0; j < content[i].size(); j++) {
            if (content[i][j] == 'O') {
                answer += 100 * i + j;
            }
        }
    }

    printMap(content);

    return answer;
}

int warehouseTwo(std::vector<std::string> content, const std::string &directions) {
    Point pos;

    std::vector<std::string> newContent;

    // start position
    for (int i = 0; i < content.size(); i++) {
        std::string row;

        for (int j = 0; j < content[i].size(); j++) {
            if (content[i][j] == '#') {
                row += "##";
            }
            if (content[i][j] == 'O') {
                row += "[]";
            }
            if (content[i][j] == '.') {
                row += "..";
            }
            if (content[i][j] == '@') {
                row += "@.";
            }
        }
        newContent.push_back(row);
    }

    for (int i = 0; i < newContent.size(); i++) {
        for (int j = 0; j < newContent[0].size(); j++) {
            if (newContent[i][j] == '@') {
                pos = {i, j};
            }
        }
    }

    for (auto direction: directions) {
        auto change = arrowMap[direction];
        Point newPos = pos + change;

        if (newContent[newPos.row][newPos.col] == '.') {
            newContent[pos.row][pos.col] = '.';
            newContent[newPos.row][newPos.col] = '@';
            pos = newPos;
            continue;
        }

        if (newContent[newPos.row][newPos.col] == '#') {
            continue;
        }

        std::vector<BigBox> boxes;

        if (newContent[newPos.row][newPos.col] == '[') {
            boxes.emplace_back(newPos, newPos + RIGHT);
        } else {
            boxes.emplace_back(newPos + LEFT, newPos);
        }

        std::deque boxQueue = {boxes[0]};

        bool canMove = true;

        while (!boxQueue.empty()) {
            auto box = boxQueue.front();
            auto left = box.left;
            auto right = box.right;
            auto changeLeft = left + change;
            auto changeRight = right + change;

            boxQueue.pop_front();

            // #
            // []
            // OR
            // #
            //[]
            if (newContent[changeLeft.row][changeLeft.col] == '#' || newContent[changeRight.row][changeRight.col] ==
                '#') {
                canMove = false;
                break;
            }

            // [][]@
            if (
                newContent[changeLeft.row][changeLeft.col] == ']' &&
                newContent[changeRight.row][changeRight.col] == '[' &&
                change == LEFT
            ) {
                BigBox newBox = {changeLeft + change, changeLeft};
                boxQueue.push_back(newBox);
                boxes.push_back(newBox);
                continue;
            }

            // @[][]
            if (
                newContent[changeLeft.row][changeLeft.col] == ']' &&
                newContent[changeRight.row][changeRight.col] == '[' &&
                change == RIGHT
            ) {
                BigBox newBox = {changeRight, changeRight + change};
                boxQueue.push_back(newBox);
                boxes.push_back(newBox);
                continue;
            }

            // []
            // []
            //  @
            if (
                newContent[changeLeft.row][changeLeft.col] == '[' &&
                newContent[changeRight.row][changeRight.col] == ']' &&
                (change == UP || change == DOWN)
            ) {
                BigBox newBox = {changeLeft, changeRight};

                boxQueue.push_back(newBox);
                boxes.push_back(newBox);
                continue;
            }

            // [].
            //  []
            //  @
            // OR
            // [][]
            //  []
            //  @
            if (
                newContent[changeLeft.row][changeLeft.col] == ']' &&
                (
                    newContent[changeRight.row][changeRight.col] == '[' ||
                    newContent[changeRight.row][changeRight.col] == '.'
                ) &&
                (change == UP || change == DOWN)
            ) {
                BigBox newBox = {changeLeft + LEFT, changeLeft};
                boxQueue.push_back(newBox);
                boxes.push_back(newBox);
            }

            //  []
            // []
            // @
            // OR
            // [][]
            //  []
            //  @
            if (
                newContent[changeRight.row][changeRight.col] == '[' &&
                (
                    newContent[changeLeft.row][changeLeft.col] == '.' ||
                    newContent[changeLeft.row][changeLeft.col] == ']'
                ) &&
                (change == UP || change == DOWN)
            ) {
                BigBox newBox = {changeRight, changeRight + RIGHT};
                boxQueue.push_back(newBox);
                boxes.push_back(newBox);
            }
        }

        if (!canMove) {
            continue;
        }

        for (int i = static_cast<int>(boxes.size()) - 1; i >= 0; i--) {
            auto curBox = boxes[i];
            auto newPointLeft = curBox.left + change;
            auto newPointRight = curBox.right + change;
            newContent[curBox.left.row][curBox.left.col] = '.';
            newContent[curBox.right.row][curBox.right.col] = '.';
            newContent[newPointLeft.row][newPointLeft.col] = '[';
            newContent[newPointRight.row][newPointRight.col] = ']';
        }

        newContent[pos.row][pos.col] = '.';
        newContent[newPos.row][newPos.col] = '@';
        pos = newPos;
    }

    int answer = 0;

    for (int i = 0; i < newContent.size(); i++) {
        for (int j = 0; j < newContent[0].size(); j++) {
            if (newContent[i][j] == '[') {
                answer += 100 * i + j;
            }
        }
    }

    printMap(newContent);

    return answer;
}

int main() {
    std::ifstream input = getStream("15.txt");
    std::string line;
    std::vector<std::string> content;
    std::string directions;

    // map
    while (getline(input, line)) {
        if (line.empty()) break;
        content.push_back(line);
    }

    while (getline(input, line)) {
        directions += line;
    }

    std::println("{}", warehouseOne(content, directions));
    std::println("{}", warehouseTwo(content, directions));
}
