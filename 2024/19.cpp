#include <fstream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>

#include "helpers.h"

/**
 * It's helpful to think of this like a tree with branches.
 *
 * The root node is the full design. is a valid substr that starts the design.
 *
 * For example:
 *                     brwrr
                      /     \
           use "br"  /       \ use "b"
                   wrr       rwrr
                  /           \
        use "wr" /             \ use "r"
               r                wrr
              /                  \
     use "r" /                    \ use "wr"
           ""                      r
                                  |
                            use "r"|
                                  ""
 * @param design
 * @param maxLength
 * @param towels
 * @param cache
 * @return
 */
long long possibilities(std::string design, int &maxLength, std::unordered_set<std::string> &towels, std::unordered_map<std::string, long long> &cache) {
    if (cache.contains(design)) return cache[design];

    if (design.empty()) return 1;

    long long count = 0;

    for (int i = 0; i < std::min(maxLength, static_cast<int>(design.size())); i++) {
        if (towels.contains(design.substr(0, i + 1))) {
            count += possibilities(design.substr(i + 1), maxLength, towels, cache);
        }
    }

    cache[design] = count;

    return count;
}

int main() {
    std::ifstream input = getStream("19.txt");
    std::string content((std::istreambuf_iterator(input)), std::istreambuf_iterator<char>());
    auto splitPos = content.find("\n\n");
    auto towelString = content.substr(0, splitPos);
    auto designs = content.substr(splitPos + 2);
    std::stringstream towelStream(towelString);
    std::string towel;
    std::unordered_set<std::string> towels;
    int maxLength = 0;

    while (getline(towelStream, towel, ',')) {
        std::string temp;

        if (towel.at(0) == ' ') {
            temp = towel.substr(1);
        } else {
            temp = towel;
        }

        maxLength = std::max(maxLength, static_cast<int>(temp.size()));
        towels.insert(temp);
    }

    std::stringstream designStream(designs);
    std::string design;

    long long answer[] = {0, 0};

    while (getline(designStream, design, '\n')) {
        std::unordered_map<std::string, long long> cache;
        long long count = possibilities(design, maxLength, towels, cache);
        answer[0] += count > 0 ? 1 : 0;
        answer[1] += count;
    }

    std::println("{}", answer);
}
