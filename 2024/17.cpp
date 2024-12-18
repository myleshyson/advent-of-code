#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include "helpers.h"

std::vector<long long> generate(long long A, long long B, long long C, std::vector<long long> &instructions) {
    long long instructionPointer = 0;
    std::map<long long, long long> operandMap;
    operandMap[0] = 0;
    operandMap[1] = 1;
    operandMap[2] = 2;
    operandMap[3] = 3;
    operandMap[4] = A;
    operandMap[5] = B;
    operandMap[6] = C;

    std::vector<long long> output;

    while (instructionPointer < instructions.size()) {
        long long opcode = instructions[instructionPointer];
        long long operand = instructions[instructionPointer + 1];

        long long newInstructionPointer = instructionPointer + 2;

        if (opcode == 0) A = A >> operandMap[operand];
        if (opcode == 1) B = B ^ operand;
        if (opcode == 2) B = operandMap[operand] % 8;
        if (opcode == 3 && A != 0) newInstructionPointer = operand;
        if (opcode == 4) B = B ^ C;
        if (opcode == 5) output.push_back(operandMap[operand] % 8);
        if (opcode == 6) B = A >> operandMap[operand];
        if (opcode == 7) C = A >> operandMap[operand];

        operandMap[4] = A;
        operandMap[5] = B;
        operandMap[6] = C;

        instructionPointer = newInstructionPointer;
    }

    return output;
}

long long find(long long num, std::vector<long long> instructions) {
    if (instructions.empty()) return num;

    for (long long i = 0; i < 8; i++) {
        long long a = (num << 3) + i;
        long long b = a % 8;
        b = b ^ 1;
        long long c = a >> b;
        b = b ^ 4;
        b = b ^ c;
        long long ans = b % 8;
        if (ans == instructions[instructions.size() - 1]) {
            auto sub = find(a, std::vector(instructions.begin(), instructions.end() - 1));
            if (sub == -1) continue;
            return sub;
        }
    }

    return -1;
}

int main() {
    std::ifstream input = getStream("17.txt");

    long long A = 0, B = 0, C = 0;

    std::vector<long long> instructions;

    // Read registers
    std::string line;
    std::getline(input, line);
    sscanf(line.c_str(), "Register A: %lld", &A);
    std::getline(input, line);
    sscanf(line.c_str(), "Register B: %lld", &B);
    std::getline(input, line);
    sscanf(line.c_str(), "Register C: %lld", &C);

    std::getline(input, line);

    std::getline(input, line);
    line = line.substr(9);

    // Parse the program instructions
    std::stringstream ss(line);

    std::string temp;

    while (std::getline(ss, temp, ',')) {
        instructions.push_back(std::stoll(temp));
    }

    auto output = generate(A, B, C, instructions);

    for (int i = 0; i < output.size(); i++) {
        if (i == 0) std::cout << output[i];
        else std::cout << "," << output[i];
    }

    std::cout << std::endl;

    // B = A % 8
    // B = B ^ 1
    // C = A >> B
    // A = A >> 3
    // B = B ^ 4
    // B = B ^ C
    // print B % 8
    // restart if A != 0

    // A >> 3 each iteration. Means that the lowest 3 bits (0-7) are removed each iteration.
    // working backwards, we know that A has to be a value between 0 and 7 to end the loop. If it were more, the loop
    // would restart again since >> 3 would still leave at least 1 bit.
    // so starting with 0, we << 3 bits and add 0-7, and see which one gives us the last item in the array.
    // once we have that, we pass in what we have so far and cut off the last instruction.
    // couldn't of done this problem without this link. hopefully I can fully understand it one day.
    // @link https://www.youtube.com/watch?v=y-UPxMAh2N8
    std::cout << find(0, instructions) << std::endl;

    // 5 = 101
    // ? = 101000
    std::println("{}", ((5 << 3) + 0));

    return 0;
}
