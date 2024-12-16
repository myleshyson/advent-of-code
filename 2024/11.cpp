#include <fstream>
#include <sstream>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <iostream>
#include <benchmark/benchmark.h>

#include "helpers.h"

long long countRecursive(long long stone, int blinks, std::map<std::pair<long long, int>, long long> &cache) {
    auto key = std::make_pair(stone, blinks);

    if (cache.contains(key)) {
        return cache.at(key);
    }

    long long count = 0;

    if (blinks == 0) {
        count = 1;
    } else if (stone == 0) {
        count = countRecursive(1, blinks - 1, cache);
    } else if (int digits = static_cast<int>(log10(stone)) + 1; digits % 2 == 0) {
        std::string num = std::to_string(stone);
        long long firstHalf = std::stoll(num.substr(0, digits / 2));
        long long secondHalf = std::stoll(num.substr(digits / 2));

        count = countRecursive(firstHalf, blinks - 1, cache) + countRecursive(secondHalf, blinks - 1, cache);
    } else {
        count = countRecursive(stone * 2024, blinks - 1, cache);
    }

    cache.insert_or_assign(key, count);

    return cache.at(key);
}

int main() {
    std::ifstream file = getStream("11.txt");
    std::string line((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::stringstream ss(line);
    std::string string;
    std::vector<long long> stones;
    std::map<std::pair<long long, int>, long long> cache;

    while (getline(ss, string, ' ')) {
        stones.push_back(std::stoll(string));
    }

    long long answer[] = {0, 0};

    for (auto stone : stones) {
        answer[0] += countRecursive(stone, 25, cache);
        answer[1] += countRecursive(stone, 75, cache);
    }

    std::println("{}", answer);

    return 0;
}