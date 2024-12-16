#include <iostream>
#include <regex>
#include <map>
#include <sstream>
#include <ostream>
#include "helpers.h"

int isValid(const std::vector<int> &pages, const std::map<int, std::vector<int>> &graph) {
    int valid = -1;

    for (int i = 1; i < pages.size(); i++) {
        int prev = pages[i - 1];

        if (!graph.contains(prev)) {
            valid = i;
            break;
        }
        auto it = std::find(graph.at(prev).begin(), graph.at(prev).end(), pages[i]);

        if (it == graph.at(prev).end()) {
            valid = i;
            break;
        }
    }

    return valid;
}

int first() {
    std::ifstream input = getStream("5.txt");
    std::string line;
    std::map<int, std::vector<int> > graph;
    int answer = 0;

    bool parsingRules = true;

    while (getline(input, line)) {
        if (line.empty()) {
            parsingRules = false;
            continue;
        }

        if (parsingRules) {
            std::regex re(R"((\d+)\|(\d+))");
            auto matchesBegin = std::sregex_iterator(line.begin(), line.end(), re);
            auto matchesEnd = std::sregex_iterator();

            for (auto i = matchesBegin; i != matchesEnd; ++i) {
                auto match = *i;
                int first = std::stoi(match[1]);
                int second = std::stoi(match[2]);

                if (!graph.contains(first)) {
                    std::vector neighbors = {second};
                    graph.emplace(first, neighbors);
                } else {
                    auto neighbors = graph.at(first);
                    neighbors.push_back(second);
                    graph.insert_or_assign(first, neighbors);
                }
            }
        } else {
            std::stringstream ss(line);
            std::string num;
            char delim = ',';
            std::vector<int> pages;

            while (!ss.eof()) {
                getline(ss, num, delim);
                pages.push_back(std::stoi(num));
            }

            if (isValid(pages, graph) == -1) answer += pages[pages.size() / 2];
        }
    }

    return answer;
}

int second() {
    std::ifstream input = getStream("5.txt");
    std::string line;
    std::map<int, std::vector<int> > graph;
    int answer = 0;

    bool parsingRules = true;

    while (getline(input, line)) {
        if (line.empty()) {
            parsingRules = false;
            continue;
        }

        if (parsingRules) {
            std::regex re(R"((\d+)\|(\d+))");
            auto matchesBegin = std::sregex_iterator(line.begin(), line.end(), re);
            auto matchesEnd = std::sregex_iterator();

            for (auto i = matchesBegin; i != matchesEnd; ++i) {
                auto match = *i;
                int first = std::stoi(match[1]);
                int second = std::stoi(match[2]);

                if (!graph.contains(first)) {
                    std::vector neighbors = {second};
                    graph.emplace(first, neighbors);
                } else {
                    auto neighbors = graph.at(first);
                    neighbors.push_back(second);
                    graph.insert_or_assign(first, neighbors);
                }
            }
        } else {
            std::stringstream ss(line);
            std::string num;
            char delim = ',';
            std::vector<int> pages;

            while (!ss.eof()) {
                getline(ss, num, delim);
                pages.push_back(std::stoi(num));
            }

            int index = isValid(pages, graph);

            if (index == -1) continue;

            while (index != -1) {
                std::iter_swap(pages.begin() + index, pages.begin() + index - 1);
                index = isValid(pages, graph);
            }

            answer += pages[pages.size() / 2];
        }
    }

    return answer;
}

int main() {
    std::cout << first() << std::endl;
    std::cout << second() << std::endl;

    return 0;
}
