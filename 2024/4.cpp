#include <map>
#include <set>
#include <iostream>
#include "helpers.h"

int first() {
    int answer = 0;
    std::ifstream file = getStream("4.txt");
    std::vector<std::vector<char>> input;
    std::map<std::pair<int, int>, std::vector<std::pair<int, int> > > graph;
    std::string line;
    std::vector directions = {
        std::make_pair(0, -1),
        std::make_pair(0, 1),
        std::make_pair(-1, 0),
        std::make_pair(1, 0),
        std::make_pair(-1, -1),
        std::make_pair(-1, 1),
        std::make_pair(1, -1),
        std::make_pair(1, 1)
    };

    while (getline(file, line)) {
        std::vector<char> row;

        for (char i: line) {
            row.push_back(i);
        }

        input.push_back(row);
    }

    std::vector letters = {'X', 'M', 'A', 'S'};

    int curLetter = 0;

    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input[i].size(); j++) {
            if (input[i][j] != letters[curLetter]) continue;

            for (auto direction: directions) {
                int newRow = i + direction.first;
                int newCol = j + direction.second;

                if (newRow < 0 || newRow >= input.size() || newCol < 0 || newCol >= input[i].size()) {
                    continue;
                }

                curLetter++;

                while (curLetter < 4 && input[newRow][newCol] == letters[curLetter]) {
                    newRow = newRow + direction.first;
                    newCol = newCol + direction.second;


                    curLetter++;

                    if (newRow < 0 || newRow >= input.size() || newCol < 0 || newCol >= input[i].size()) {
                        break;
                    }
                }

                if (curLetter == 4) answer++;

                curLetter = 0;
            }
        }
    }

    return answer;
}

int second() {
    int answer = 0;
    std::ifstream file = getStream("4.txt");
    std::vector<std::vector<char>> input;
    std::map<std::pair<int, int>, std::vector<std::pair<int, int> > > graph;
    std::string line;
    std::vector directions = {
        std::make_pair(-1, -1),
        std::make_pair(-1, 1),
        std::make_pair(1, -1),
        std::make_pair(1, 1)
    };

    while (getline(file, line)) {
        std::vector<char> row;

        for (char i: line) {
            row.push_back(i);
        }

        input.push_back(row);
    }

    std::vector letters = {'M', 'A', 'S'};
    std::set<std::pair<int, int>> intersectingPoints;
    int curLetter = 0;

    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input[i].size(); j++) {

            if (input[i][j] != letters[curLetter]) continue;

            for (auto direction: directions) {
                std::set<std::pair<int, int>> seen;

                seen.emplace(i, j);

                int newRow = i + direction.first;
                int newCol = j + direction.second;

                if (newRow < 0 || newRow >= input.size() || newCol < 0 || newCol >= input[i].size()) {
                    continue;
                }

                curLetter++;

                while (curLetter < 3 && input[newRow][newCol] == letters[curLetter]) {
                    seen.emplace(newRow, newCol);
                    newRow = newRow + direction.first;
                    newCol = newCol + direction.second;


                    curLetter++;

                    if (newRow < 0 || newRow >= input.size() || newCol < 0 || newCol >= input[i].size()) {
                        break;
                    }
                }

                if (curLetter == 3) {
                    for (auto point : seen) {
                        if (intersectingPoints.contains(point) && input[point.first][point.second] == 'A') answer++;
                        intersectingPoints.emplace(point.first, point.second);
                    }
                }

                curLetter = 0;
            }
        }
    }

    return answer;
}

int main() {
    std::cout << first() << std::endl;
    std::cout << second() << std::endl;
}
