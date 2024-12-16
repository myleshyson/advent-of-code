#include <iostream>
#include <sstream>
#include <map>
#include <ranges>
#include <set>

#include "helpers.h"

// credit to this new solution goes to
// https://www.reddit.com/r/adventofcode/comments/1h8l3z5/comment/m0tv6di/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button
// originally I was using a graph and it was kinda slow.
// going backwards and using math is way faster.

typedef std::map<std::pair<long, int>, std::vector<std::pair<std::string, std::pair<long, int> > > > graph;

template <typename T>
int digits(T number) {
    return static_cast<int>(log10(number)) + 1;
}

template <typename T>
bool endsWith(long long target, const T number) {
    const int d = digits(number);
    target -= number;

    return target % static_cast<int>(pow(10, d)) == 0;
}

bool isValid(std::vector<int> &numbers, const long long &target, const bool checkConcat = false) {
    std::vector head(numbers.begin(), numbers.begin() + static_cast<int>(numbers.size()) - 1);

    const int number = numbers.back();

    if (head.empty()) {
        return number == target;
    }

    const long long mod = target % number;
    long long quotient = target / number;

    if (mod == 0 && isValid(head, quotient, checkConcat)) {
        return true;
    }

    if (checkConcat && endsWith(target, number) && isValid(head, target / pow(10, digits(number)), checkConcat)) {
        return true;
    }

    return isValid(head, target - number, checkConcat);
}

int main() {
    std::ifstream file = getStream("7.txt");
    std::string line;
    long long answer[2] = {0, 0};

    while (getline(file, line)) {
        graph graph;
        auto split = line.find(": ");
        long long equals = stoll(line.substr(0, split));
        auto numbers = line.substr(split + 2);

        std::stringstream ss(numbers);
        std::string number;
        std::vector<int> numberVec;

        while (getline(ss, number, ' ')) {
            numberVec.push_back(stoi(number));
        }

        if (isValid(numberVec, equals)) {
            answer[0] += equals;
        }

        if (isValid(numberVec, equals, true)) {
            answer[1] += equals;
        }
    }

    std::println("{}", answer);

    return 0;
}
