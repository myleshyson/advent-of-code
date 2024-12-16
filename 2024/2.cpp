#include <fstream>
#include <sstream>
#include <iostream>
#include "helpers.h"

using namespace std;

bool inRange(int value) {
    return value >= 1 && value <= 3;
}

bool isSafe(const vector<int> &value) {
    bool increasing = true;
    bool decreasing = true;
    bool gradual = true;

    for (int i = 0; i < (value.size() - 1); i++) {
        if (value[i] > value[i + 1]) {
            increasing = false;
        }

        if (value[i] < value[i + 1]) {
            decreasing = false;
        }

        int difference = abs(value[i] - value[i + 1]);

        if (difference < 1 || difference > 3) {
            gradual = false;
            break;
        }
    }

    if (gradual && (increasing || decreasing)) {
        return true;
    }

    return false;
}

int first() {
    ifstream file = getStream("2.txt");
    string line;
    int answer = 0;

    while (getline(file, line)) {
        stringstream ss(line);
        char delim = *" ";
        string split;
        vector<int> levels;

        while (getline(ss, split, delim)) {
            levels.push_back(stoi(split));
        }

        if (isSafe(levels)) answer++;
    }

    return answer;
}

int second() {
    ifstream file = getStream("2.txt");
    string line;
    int answer = 0;

    while (getline(file, line)) {
        stringstream ss(line);
        char delim = *" ";
        string split;
        vector<int> levels;
        vector<int> diffs;

        while (getline(ss, split, delim)) {
            levels.push_back(stoi(split));
        }

        if (isSafe(levels)) {
            answer++;
            continue;
        }

        for (int i = 0; i < levels.size(); i++) {
            vector thing(levels.begin(), levels.end());
            thing.erase(thing.begin() + i);

            if (isSafe(thing)) {
                answer++;
                break;
            }
        }
    }

    return answer;
}

int main() {
    cout << first() << endl;
    cout << second() << endl;

    return 0;
}
