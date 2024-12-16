#include "helpers.h"
#include <regex>
#include <iostream>

int first() {
    std::ifstream file = getStream("3.txt");
    std::string line;
    int answer = 0;

    while (getline(file, line)) {
        std::regex regex("mul\\(([0-9]+),([0-9]+)\\)");
        auto matchesBegin = std::sregex_iterator(line.begin(), line.end(), regex);
        auto matchesEnd = std::sregex_iterator();

        for (auto i = matchesBegin; i != matchesEnd; ++i) {
            std::smatch match = *i;
            auto first = match[1];
            auto second = match[2];

            answer += std::stoi(first.str()) * std::stoi(second.str());
        }
    }

    return answer;
}

int second() {
    std::ifstream file = getStream("3.txt");
    std::string line;
    int answer = 0;
    bool disabled = false;

    while (getline(file, line)) {
        std::regex regex(R"(mul\(([0-9]+),([0-9]+)\)|don't\(\)|do\(\))");
        auto matchesBegin = std::sregex_iterator(line.begin(), line.end(), regex);
        auto matchesEnd = std::sregex_iterator();

        for (auto i = matchesBegin; i != matchesEnd; ++i) {
            std::smatch match = *i;

            if (match.str() == "don't()") {
                disabled = true;
                continue;
            }
            if (match.str() == "do()") {
                disabled = false;
                continue;
            }

            if (disabled) {
                continue;
            }

            auto first = match[1];
            auto second = match[2];

            answer += std::stoi(first.str()) * std::stoi(second.str());
        }
    }

    return answer;
}

int main() {
    std::cout << first() << std::endl;
    std::cout << second() << std::endl;
}
