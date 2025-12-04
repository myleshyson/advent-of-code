use aoc_2025::run_day;
use std::ops::Add;

#[derive(Clone, Copy)]
struct Direction(i8, i8);

#[derive(Clone, Copy)]
struct Point(i64, i64);

impl Add<Direction> for Point {
    type Output = Self;

    fn add(self, rhs: Direction) -> Self::Output {
        Point(self.0 + rhs.0 as i64, self.1 + rhs.1 as i64)
    }
}

const NORTH: Direction = Direction(-1, 0);
const SOUTH: Direction = Direction(1, 0);
const EAST: Direction = Direction(0, 1);
const WEST: Direction = Direction(0, -1);
const NORTHEAST: Direction = Direction(-1, 1);
const NORTHWEST: Direction = Direction(-1, -1);
const SOUTHEAST: Direction = Direction(1, 1);
const SOUTHWEST: Direction = Direction(1, -1);

const ALL_DIRECTIONS: [Direction; 8] = [
    NORTH, SOUTH, EAST, WEST, NORTHEAST, NORTHWEST, SOUTHEAST, SOUTHWEST,
];

fn part1(input: &str) -> u64 {
    let graph: Vec<Vec<char>> = input.lines().map(|line| line.chars().collect()).collect();
    let mut ans = 0;

    for (row, line) in graph.iter().enumerate() {
        for (col, &ch) in line.iter().enumerate() {
            if ch == '.' {
                continue;
            }

            let mut count = 0;
            let point = Point(row as i64, col as i64);

            for d in ALL_DIRECTIONS {
                let new_point = point + d;

                if let Some(&ch) = graph
                    .get(new_point.0 as usize)
                    .and_then(|col| col.get(new_point.1 as usize))
                {
                    if ch == '@' {
                        count += 1;
                    }
                }
            }

            if count < 4 {
                ans += 1;
            }
        }
    }

    ans
}

fn part2(input: &str) -> u64 {
    let mut graph: Vec<Vec<char>> = input.lines().map(|line| line.chars().collect()).collect();
    let mut remove = 0;
    let mut cur_removed = 0;
    loop {
        for row in 0..graph.len() {
            for col in 0..graph[row].len() {
                let ch = &graph[row][col];

                if *ch == '.' {
                    continue;
                }

                let mut count = 0;
                let point = Point(row as i64, col as i64);

                for d in ALL_DIRECTIONS {
                    let new_point = point + d;

                    if let Some(ch) = graph
                        .get(new_point.0 as usize)
                        .and_then(|col| col.get(new_point.1 as usize))
                    {
                        if *ch == '@' {
                            count += 1;
                        }
                    }
                }

                if count < 4 {
                    cur_removed += 1;
                    graph[row][col] = '.';
                }
            }
        }

        if cur_removed == 0 {
            break;
        }

        remove += cur_removed;
        cur_removed = 0;
    }
    remove
}

fn main() {
    run_day(4, part1, part2);
}
