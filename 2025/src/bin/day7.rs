use std::{
    cmp::max,
    collections::{HashSet, VecDeque},
};

use aoc_2025::run_day;

fn part1(input: &str) -> u64 {
    let mut queue = VecDeque::new();
    let grid_height = input.lines().count();
    let grid_width = input.lines().next().unwrap().len();

    'find_s: for (y, line) in input.lines().enumerate() {
        for (x, ch) in line.chars().enumerate() {
            if ch == 'S' {
                queue.push_back((x, y));
                break 'find_s;
            }
        }
    }

    let mut count = 0;
    let mut visited: HashSet<(usize, usize)> = HashSet::new();

    while let Some((x, y)) = queue.pop_front() {
        if visited.contains(&(x, y)) {
            continue;
        }

        let flat_idx = (y * (grid_width + 1)) + x;
        let ch = input
            .get(flat_idx..flat_idx + 1)
            .expect("uh oh. this shouldn't happen");

        if ch == "S" || ch == "." {
            let next_y = y + 1;
            if next_y < grid_height {
                queue.push_back((x, next_y));
            }
            continue;
        }

        visited.insert((x, y));

        let mut split = false;

        for dir in [-1_isize, 1_isize] {
            let new_x = x as isize + dir;
            if new_x < 0 || new_x >= grid_width as isize {
                continue;
            }
            split = true;
            queue.push_back((new_x as usize, y));
        }

        if split {
            count += 1;
        }
    }

    count
}

fn part2(input: &str) -> u64 {
    let grid_height = input.lines().count();
    let grid_width = input.lines().next().unwrap().len();
    let mut dp: Vec<Vec<u64>> = Vec::new();
    let mut default_col = Vec::with_capacity(grid_width);
    default_col.resize(grid_width, 0);
    dp.resize(grid_height, default_col);

    for (y, line) in input.lines().enumerate() {
        for (x, ch) in line.chars().enumerate() {
            if ch == 'S' {
                dp[y][x] = 1;
                continue;
            }

            if ch == '.' && y as isize - 1 >= 0 {
                dp[y][x] += dp[y - 1][x];
                continue;
            }

            if ch == '^' {
                for j in [-1_isize, 1_isize] {
                    let new_col = (x as isize + j) as usize;
                    dp[y][new_col] = max(dp[y - 1][x] + dp[y][new_col], dp[y][new_col]);
                }
            }
        }
    }

    dp[grid_height - 1].iter().sum()
}

fn main() {
    run_day(7, part1, part2);
}
