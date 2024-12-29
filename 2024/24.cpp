#include <ranges>

#include "24.h"

#define BITSIZE 46

unsigned long long runSystem(
        const std::vector<std::vector<std::string>> &wires,
        std::map<std::string, int> gateMap,
        std::set<std::string> outputs
) {
    while (!outputs.empty()) {
        for (int i = 0; i < wires.size(); i++) {
            const auto [gate1, gate2, operand, result] = std::tie(
                    wires[i][0],
                    wires[i][1],
                    wires[i][2],
                    wires[i][3]
            );
            if (!gateMap.contains(gate1) || !gateMap.contains(gate2)) continue;
            outputs.erase(result);
            if (operand == "XOR") {
                gateMap[result] = gateMap[gate1] ^ gateMap[gate2];
            } else if (operand == "OR") {
                gateMap[result] = gateMap[gate1] | gateMap[gate2];
            } else if (operand == "AND") {
                gateMap[result] = gateMap[gate1] & gateMap[gate2];
            }
        }
    }

    std::string bits;

    for (auto &it: std::ranges::reverse_view(gateMap)) {
        if (it.first[0] != 'z') continue;
        bits += std::to_string(it.second);
    }

    return std::bitset<BITSIZE>(bits).to_ullong();
}

void pp(std::string result,
        std::map<std::string, std::vector<std::string>> &map,
        int depth = 0) {
    if (result[0] == 'x' || result[0] == 'y') {
        for (int i = 0; i < depth; i++) {
            std::cout << " ";
        }
        std::cout << result;
        return;
    }

    auto item = map[result];

    for (int i = 0; i < depth; i++) {
        std::cout << " ";
    }
    std::cout << item[2] << " (" << result << ")\n";
    pp(item[0], map, depth + 1);
    std::cout << "\n";
    pp(item[1], map, depth + 1);
}

std::string makeWire(char character, int num) {
    std::string result(1, character);

    if (num < 10) {
        result += "0";
    }

    result += std::to_string(num);

    return result;
}

bool verify_intermediate_xor(
        std::string wire,
        std::map<std::string, std::vector<std::string>> &map,
        int num
) {
    if (!map.contains(wire)) return false;
    const auto [x, y, operand] =
            std::tie(map[wire][0], map[wire][1], map[wire][2]);
    std::vector sorted{x, y};
    std::sort(sorted.begin(), sorted.end());

    if (operand != "XOR") {
        return false;
    }

    if (sorted[0] == makeWire('x', num) && sorted[1] == makeWire('y', num)) {
        return true;
    }

    return false;
}

bool verify_direct_carry(
        std::string wire,
        std::map<std::string, std::vector<std::string>> &map,
        int num
) {
    if (!map.contains(wire)) return false;
    const auto [x, y, operand] =
            std::tie(map[wire][0], map[wire][1], map[wire][2]);
    std::vector sorted{x, y};
    std::sort(sorted.begin(), sorted.end());

    if (operand != "AND") {
        return false;
    }

    if (sorted[0] == makeWire('x', num) && sorted[1] == makeWire('y', num)) {
        return true;
    }

    return false;

}
bool verify_recarry(
        std::string wire,
        std::map<std::string, std::vector<std::string>> &map,
        int num
) {
    if (!map.contains(wire)) return false;
    const auto [x, y, operand] =
            std::tie(map[wire][0], map[wire][1], map[wire][2]);
    std::vector sorted{x, y};
    std::sort(sorted.begin(), sorted.end());

    if (operand != "AND") return false;

    return (verify_intermediate_xor(x, map, num) &&
            verify_carry_bit(y, map, num)) ||
           (verify_intermediate_xor(y, map, num) &&
            verify_carry_bit(x, map, num));
}

bool verify_carry_bit(
        std::string wire,
        std::map<std::string, std::vector<std::string>> &map,
        int num
) {
    if (!map.contains(wire)) return false;
    const auto [x, y, operand] =
            std::tie(map[wire][0], map[wire][1], map[wire][2]);
    std::vector sorted{x, y};
    std::sort(sorted.begin(), sorted.end());

    if (num == 1) {
        return operand == "AND" && sorted[0] == "x00" && sorted[1] == "y00";
    }

    if (operand != "OR") return false;

    return (verify_direct_carry(x, map, num - 1) &&
            verify_recarry(y, map, num - 1)) ||
           (verify_direct_carry(y, map, num - 1) &&
            verify_recarry(x, map, num - 1));
}

bool verify_z(
        std::string wire,
        std::map<std::string, std::vector<std::string>> &map,
        int num
) {
    if (!map.contains(wire)) return false;

    const auto [x, y, operand] =
            std::tie(map[wire][0], map[wire][1], map[wire][2]);
    std::vector sorted{x, y};
    std::sort(sorted.begin(), sorted.end());

    if (num == 0) {
        return operand == "XOR" && x == "x00" && y == "y00";
    }
    return (verify_intermediate_xor(x, map, num) &&
            verify_carry_bit(y, map, num)) ||
           (verify_intermediate_xor(y, map, num) &&
            verify_carry_bit(x, map, num));
}

bool verify(int num, std::map<std::string, std::vector<std::string>> &map) {
    return verify_z(makeWire('z', num), map, num);
}

int progress(std::map<std::string, std::vector<std::string>> &map) {
    int i = 0;

    while (true) {
        if (!verify(i, map)) break;
        i++;
    }

    return i;
}

int main() {
    std::ifstream file = getStream("24.txt");
    std::string content(
            (std::istreambuf_iterator(file)),
            std::istreambuf_iterator<char>()
    );
    auto split = content.find("\n\n");
    auto gateMapString = content.substr(0, split);
    auto programString = content.substr(split + 2);

    std::stringstream gateMapStream(gateMapString);
    std::stringstream programStringStream(programString);
    std::string line;

    std::map<std::string, int> gateMap;
    std::set<std::string> initial;

    while (getline(gateMapStream, line)) {
        auto split = line.find(": ");
        auto gate = line.substr(0, split);
        auto number = line.substr(split + 2);
        gateMap[gate] = stoi(number);
        initial.insert(gate);
    }

    std::string bits;
    std::vector<std::vector<std::string>> wires;
    std::set<std::string> outputs;
    std::map<std::string, std::vector<std::string>> wireMap;

    while (getline(programStringStream, line)) {
        std::string gate1, operand, gate2, arrow, result;
        std::istringstream lineStream(line);
        lineStream >> gate1 >> operand >> gate2 >> arrow >> result;
        std::vector gates = {gate1, gate2};
        wireMap[result] = {gates[0], gates[1], operand};
        wires.push_back({gates[0], gates[1], operand, result});
        outputs.insert(result);
    }

    std::cout << runSystem(wires, gateMap, outputs) << std::endl;

    std::vector<std::string> results;

    for (int i = 0; i < 4; i++) {
        auto baseline = progress(wireMap);
        bool reset = false;
        for (auto pair : wireMap) {
            for (auto other : wireMap) {
                if (pair.first == other.first) continue;
                auto mapCopy = wireMap;
                mapCopy[pair.first] = wireMap[other.first];
                mapCopy[other.first] = wireMap[pair.first];

                if (progress(mapCopy) > baseline) {
                    results.push_back(pair.first);
                    results.push_back(other.first);
                    reset = true;
                    wireMap = mapCopy;
                    break;
                }
            }
            if (reset) break;
        }
    }

    std::sort(results.begin(), results.end());

    std::string answer;

    for (auto result : results) {
        answer += result + ",";
    }

    answer.pop_back();

    std::cout << answer << std::endl;
}
