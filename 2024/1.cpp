
#include <iostream>
#include <fstream>
#include "helpers.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>

using namespace std;

int first() {
    ifstream infile = getStream("1.txt");
    vector<int> left;
    vector<int> right;

    string a, b;
    int answer = 0;

    while (infile >> a >> b) {
        left.push_back(stoi(a));
        right.push_back(stoi(b));
    }

    ranges::sort(left);
    ranges::sort(right);

    for (int i = 0; i < left.size(); i++) {
        answer += abs(left.at(i) - right.at(i));
    }

    return answer;
}

int second() {
    ifstream infile = getStream("1.txt");
    vector<int> left;
    vector<int> right;
    map<int, int> numMap;

    string a, b;
    int answer = 0;

    while (infile >> a >> b) {
        left.push_back(stoi(a));
        right.push_back(stoi(b));
        numMap[stoi(a)] = 0;
    }

    for (auto num : right) {
        if (numMap.contains(num)) {
            numMap[num]++;
        }
    }

    for (const auto &[fst, snd] : numMap) {
        answer += fst * snd;
    }

    return answer;
}

int main() {
    cout << first() << endl;
    cout << second() << endl;
    return 0;
}
