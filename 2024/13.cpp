#include <regex>
#include <sstream>
#include "helpers.h"

/**
 * px = a*ax + b*bx
 * py = a*ay + b*by
 *
 * [ ax bx ]
 * [ ay by ]
 *
 * a = dx / d
 * b = dy / d
 *
 * d = ax*by - bx*ay
 * dx = px*by - bx*py
 * dy = ax*py - px*ay
 *
 * tokens = 3*a + 1*b
 *
 * All credit to this person. I'm just using this to learn more about how to work with linear algebra!
 * @link https://github.com/PaigePalisade/AdventOfCode2024/blob/main/Solutions/day13part2.c
 * @param ax
 * @param ay
 * @param bx
 * @param by
 * @param px
 * @param py
 * @return
 */
long long getTokens
(
    const long long ax,
    const long long ay,
    const long long bx,
    const long long by,
    const long long px,
    const long long py
) {
    long long tokens = 0;
    const long long dx = px * by - py * bx;
    const long long dy = py * ax - px * ay;
    const long long d = ax * by - ay * bx;

    // if dx/d or dy/d is a fraction, that means there's no integer solution.
    if (dx % d == 0 && dy % d == 0) {
        const long long a = dx / d;
        const long long b = dy / d;
        tokens += 3 * a + b;
    }

    return tokens;
}

int main() {
    std::string projectPath = std::filesystem::path(__FILE__).parent_path();
    std::string filename(projectPath + "/" + "13.txt");
    FILE *f = fopen(filename.c_str(), "r");

    long long answer[] = {0, 0};

    while (!feof(f)) {
        long long ax, ay, bx, by, px, py;
        fscanf(f, "Button A: X+%lld, Y+%lld ", &ax, &ay);
        fscanf(f, "Button B: X+%lld, Y+%lld ", &bx, &by);
        fscanf(f, "Prize: X=%lld, Y=%lld ", &px, &py);

        answer[0] += getTokens(ax, ay, bx, by, px, py);
        answer[1] += getTokens(ax, ay, bx, by, px + 10000000000000, py + 10000000000000);
    }

    std::println("{}", answer);
}
