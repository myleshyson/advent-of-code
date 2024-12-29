#include "helpers.h"

int main() {
    std::ifstream file = getStream("25.txt");
    std::string content((std::istreambuf_iterator(file)), std::istreambuf_iterator<char>());
    std::vector<std::vector<std::string>> keys;
    std::vector<std::vector<std::string>> locks;
    auto blocks = explode(content, "\n\n");

    for (const auto &block : blocks) {
        std::stringstream ss(block);
        std::string line;
        std::vector<std::string> matrix;

        while (getline(ss, line)) {
            matrix.push_back(line);
        }

        if (matrix[0][0] == '#') locks.push_back(matrix);
        else keys.push_back(matrix);
    }

    std::vector<std::vector<int>> pinHeights;
    std::vector<std::vector<int>> keyHeights;

    for (auto key : keys) {
        std::vector<int> heights(key[0].size());
        std::unordered_set<int> found;

        for (int i = 0; i < key.size(); i++) {
            for (int j = 0; j < key[0].size(); j++) {
                if (found.contains(j)) continue;
                if (key[i][j] == '#') {
                    heights[j] = key.size() - 1 - i;
                    found.insert(j);
                }
            }
        }

        keyHeights.push_back(heights);
    }


    for (auto lock : locks) {
        std::vector<int> heights(lock[0].size());
        std::unordered_set<int> found;

        for (int i = 0; i < lock.size(); i++) {
            for (int j = 0; j < lock[0].size(); j++) {
                if (found.contains(j)) continue;
                if (lock[i][j] == '.') {
                    heights[j] = i - 1;
                    found.insert(j);
                }
            }
        }
        pinHeights.push_back(heights);
    }

    int answer = 0;

    for (const auto &key : keyHeights) {
        for (const auto &lock : pinHeights) {
            if (key.size() != lock.size()) continue;
            bool valid = true;
            for (int i = 0; i < key.size(); i++) {
                if (key[i] + lock[i] > locks[0].size() - 2) {
                    valid = false;
                    break;
                }
            }

            if (valid) answer++;
        }
    }

    std::cout << answer << std::endl;
}