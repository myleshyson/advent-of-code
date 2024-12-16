#include <fstream>
#include <map>
#include <iostream>
#include <chrono>
#include <thread>
#include "helpers.h"

int partOne(int time) {
    const char *name = "14.txt";
    FILE* file = getScan(name);

    int width = 101;
    int height = 103;
    int midx = width / 2;
    int midy = height / 2;
    int quadrants[] = {0, 0, 0, 0};
    int answer;

    while (!feof(file)) {
        int posx,posy,vx,vy;

        fscanf(file, "p=%d,%d v=%d,%d ", &posx, &posy, &vx, &vy);

        int finalx = (((posx + time*vx) % width) + width) % width;
        int finaly = (((posy + time*vy) % height) + height) % height;

        if (finalx == midx || finaly == midy) continue;

        if (finalx < midx && finaly < midy) quadrants[0]++;
        if (finalx > midx && finaly < midy) quadrants[1]++;
        if (finalx < midx && finaly > midy) quadrants[2]++;
        if (finalx > midx && finaly > midy) quadrants[3]++;
    }

    fclose(file);

    answer = quadrants[0] * quadrants[1] * quadrants[2] * quadrants[3];

    return answer;
}

void print(int time) {
    const char *name = "14.txt";
    FILE* file = getScan(name);

    int width = 101;
    int height = 103;
    int midx = width / 2;
    int midy = height / 2;
    char grid[height][width];

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < height; j++) {
            grid[i][j] = '.';
        }
    }

    while (!feof(file)) {
        int posx,posy,vx,vy;

        fscanf(file, "p=%d,%d v=%d,%d ", &posx, &posy, &vx, &vy);

        int finalx = (((posx + time*vx) % width) + width) % width;
        int finaly = (((posy + time*vy) % height) + height) % height;

        if (finalx == midx || finaly == midy) continue;

        grid[finaly][finalx] = '#';
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < height; j++) {
            std::cout << grid[i][j];
        }
        std::cout << std::endl;
    }

    fclose(file);
}

int main() {
    std::println("{}", partOne(100));

    int min = INT_MAX;
    int answer = 0;

    for (int i = 0; i < 10000; i++) {
        int score = partOne(i);

        if (score < min) {
            answer = i;
        }

        min = std::min(min, score);
    }
    std::println("{}", answer);
    print(answer);

}
