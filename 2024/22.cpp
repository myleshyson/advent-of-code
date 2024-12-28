#include "helpers.h"

void calculate(long long &number) {
    long long result = number * 64;

    number = number ^ result;
    number = number % 16777216;
    result = number / 32;
    number ^= result;
    number = number % 16777216;
    result = number * 2048;
    number ^= result;
    number = number % 16777216;
}

int main() {
    std::ifstream file = getStream("22.txt");
    std::string line;
    std::unordered_map<std::string, long long> overallMap;

    long long part1 = 0;

    while (getline(file, line)) {
        long long secretNumber = stoll(line);
        long long lastDigit = secretNumber % 10;
        std::queue<long long> differenceQueue;
        std::unordered_set<std::string> seen;

        for (int i = 0; i < 2000; i++) {
            calculate(secretNumber);
            differenceQueue.push((secretNumber % 10) - lastDigit);
            lastDigit = secretNumber % 10;

            if (differenceQueue.size() == 4) {
                auto copy = differenceQueue;
                std::string key;

                while (!copy.empty()) {
                    key += std::to_string(copy.front());
                    copy.pop();
                }

                if (!seen.contains(key)) {
                    overallMap[key] += lastDigit;
                    seen.insert(key);
                }

                differenceQueue.pop();
            }
        }

        part1 += secretNumber;
    }

    long long max = LONG_LONG_MIN;

    for (const auto &pair : overallMap) {
        max = std::max(max, pair.second);
    }

    std::cout << part1 << std::endl;
    std::cout << max << std::endl;
}