#include <iostream>
#include <vector>
#include <map>
#include <set>

#include "helpers.h"

long long partOne(std::vector<int> nums) {
    std::vector<int> compacted;

    int startIndex = 0;
    int endIndex = static_cast<int>(nums.size()) - 1;
    long long answer = 0;

    while (startIndex <= endIndex) {
        if (startIndex % 2 == 0) {
            int fileId = startIndex / 2;

            for (int j = 0; j < nums[startIndex]; j++) {
                compacted.push_back(fileId);
            }

            startIndex++;
            continue;
        }

        if (endIndex % 2 == 0) {
            int fileId = endIndex / 2;

            while (nums[startIndex] > 0 && nums[endIndex] > 0) {
                compacted.push_back(fileId);
                nums[startIndex]--;
                nums[endIndex]--;
            }

            if (nums[startIndex] == 0) {
                startIndex++;
            }

            if (nums[endIndex] == 0) {
                endIndex--;
            }

            continue;
        }

        if (nums[startIndex] == 0) {
            startIndex++;
        }

        endIndex--;
    }

    for (int i = 0; i < compacted.size(); i++) {
        answer += i * compacted[i];
    }

    return answer;
}

// 0,2, -1,3, 1,3
long long partTwo(std::vector<int> nums) {
    long long answer = 0;

    struct Block {
        int size;
        int id;

        Block(int size, int id): size(size), id(id) {
        };
    };

    std::vector<Block> blocks;
    std::stack<int> maxHeap;

    for (int i = 0; i < nums.size(); i++) {
        if (nums[i] == 0) continue;

        if (i % 2 == 0) {
            blocks.emplace_back(nums[i], i / 2);
            maxHeap.push(i / 2);
        } else {
            blocks.emplace_back(nums[i], -1);
        }
    }

    while (!maxHeap.empty()) {
        int id = maxHeap.top();
        maxHeap.pop();

        for (auto i = static_cast<int>(blocks.size() - 1); i >= 0; i--) {
            if (blocks[i].id != id) continue;

            Block file = blocks[i];

            for (auto j = 0; j < static_cast<int>(blocks.size()); j++) {
                if (j >= i) break;
                if (blocks[j].id != -1) continue;
                if (blocks[j].size < file.size) continue;

                blocks[j].size -= file.size;
                blocks[i].id = -1;
                blocks.insert(blocks.begin() + j, {file.size, file.id});

                break;
            }

            break;
        }
    }

    int currentIndex = 0;

    for (auto block = blocks.begin(); block != blocks.end(); ++block) {
        if (block->id == -1) {
            currentIndex += block->size;
            continue;
        }

        for (int i = 0; i < block->size; i++) {
            answer += block->id * currentIndex;
            currentIndex++;
        }
    }

    return answer;
}

int main() {
    std::ifstream input = getStream("9.txt");
    const std::string line((std::istreambuf_iterator(input)), std::istreambuf_iterator<char>());
    std::vector<int> nums;

    for (int i = 0; i < line.size(); i++) {
        nums.push_back(line[i] - '0');
    }

    std::println("{}", partOne(nums));
    std::cout << partTwo(nums) << std::endl;
    return 0;
}
