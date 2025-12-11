use std::{
    collections::{HashMap, VecDeque},
    ops::Add,
};

use aoc_2025::run_day;
use itertools::Itertools;

fn part1(input: &str) -> u64 {
    let points: Vec<(u64, u64)> = input
        .lines()
        .map(|line| {
            let mut iter = line
                .split(",")
                .map(|num| num.parse::<u64>().expect("invalid num"));
            (iter.next().unwrap(), iter.next().unwrap())
        })
        .collect();

    let mut max_area = 0;

    for (i, point) in points.iter().enumerate() {
        for (j, other) in points.iter().enumerate() {
            if i == j {
                continue;
            }
            let area = point.0.abs_diff(other.0).add(1) * point.1.abs_diff(other.1).add(1);
            max_area = max_area.max(area);
        }
    }

    max_area
}

#[derive(Clone, PartialEq, PartialOrd, Copy, Debug, Eq)]
struct Point(u64, u64);

impl Point {
    fn from_usize(x: usize, y: usize) -> Self {
        Point(x as u64, y as u64)
    }
    fn x(&self) -> u64 {
        self.0
    }

    fn y(&self) -> u64 {
        self.1
    }

    fn x_usize(&self) -> usize {
        self.0 as usize
    }

    fn y_usize(&self) -> usize {
        self.1 as usize
    }

    fn neighbors(&self) -> impl Iterator<Item = Point> {
        let mut result = vec![Point(self.x() + 1, self.y()), Point(self.x(), self.y() + 1)];

        if self.x() > 0 {
            result.push(Point(self.x() - 1, self.y()));
        }

        if self.y() > 0 {
            result.push(Point(self.x(), self.y() - 1));
        }

        return result.into_iter();
    }
}

fn part2(input: &str) -> u64 {
    let points: Vec<Point> = input
        .lines()
        .map(|line| {
            let mut iter = line
                .split(",")
                .map(|num| num.parse::<u64>().expect("invalid num"));
            Point(iter.next().unwrap(), iter.next().unwrap())
        })
        .collect();

    let mut edges: Vec<(Point, Point)> = Vec::new();
    let mut prev = points.get(0).unwrap();

    for point in points.iter().skip(1) {
        edges.push((prev.clone(), point.clone()));
        prev = point;
    }

    edges.push((points[0], *prev));

    let xs: Vec<u64> = points.iter().map(|p| p.x()).sorted().dedup().collect();
    let ys: Vec<u64> = points.iter().map(|p| p.y()).sorted().dedup().collect();

    // 0, 1, 2, 3
    // for (x,y) like (90000, 800000)
    // something like:
    // 90000 -> 200
    // 80000 -> 100
    let x_to_idx: HashMap<u64, usize> = xs.iter().enumerate().map(|(i, &x)| (x, i)).collect();
    let y_to_idx: HashMap<u64, usize> = ys.iter().enumerate().map(|(i, &y)| (y, i)).collect();

    let width = xs.len();
    let height = ys.len();

    let mut grid: Vec<Vec<Option<bool>>> = vec![vec![None; width]; height];

    let mut prev: Option<Point> = None;
    for point in points.iter().chain(std::iter::once(&points[0])) {
        let compressed_point = Point::from_usize(x_to_idx[&point.x()], y_to_idx[&point.y()]);

        grid[compressed_point.y_usize()][compressed_point.x_usize()] = Some(true);

        if prev == None {
            prev = Some(compressed_point);
            continue;
        }

        let prev_unwrapped = prev.unwrap();

        for y in compressed_point.y().min(prev_unwrapped.y())
            ..=compressed_point.y().max(prev_unwrapped.y())
        {
            for x in compressed_point.x().min(prev_unwrapped.x())
                ..=compressed_point.x().max(prev_unwrapped.x())
            {
                grid[y as usize][x as usize] = Some(true);
            }
        }

        prev = Some(compressed_point);
    }

    // fill outside
    for y in 0..height {
        for x in 0..width {
            if (x != 0 && x != width - 1) && (y != 0 && y != height - 1) {
                continue;
            }

            if grid[y][x] == None {
                grid[y][x] = Some(false);

                let mut queue: VecDeque<Point> = VecDeque::new();
                queue.push_back(Point::from_usize(x, y));
                while let Some(point) = queue.pop_front() {
                    for neighbor in point.neighbors() {
                        if grid.get(neighbor.y_usize()).is_none()
                            || grid[neighbor.y_usize()].get(neighbor.x_usize()).is_none()
                        {
                            continue;
                        }
                        if grid[neighbor.y_usize()][neighbor.x_usize()] == None {
                            grid[neighbor.y_usize()][neighbor.x_usize()] = Some(false);
                            queue.push_back(neighbor);
                        }
                    }
                }
            }
        }
    }

    // now try to fill in a rectangle without hitting point thats on the outside
    let mut max_area = 0;
    for point_1 in points.iter() {
        'outerloop: for point_2 in points.iter().filter(|p| *p != point_1) {
            let point_1_x = x_to_idx[&point_1.x()];
            let point_1_y = y_to_idx[&point_1.y()];
            let point_2_x = x_to_idx[&point_2.x()];
            let point_2_y = y_to_idx[&point_2.y()];

            for y in point_1_y.min(point_2_y)..=point_1_y.max(point_2_y) {
                for x in point_1_x.min(point_2_x)..=point_1_x.max(point_2_x) {
                    if grid[y][x] == Some(false) {
                        continue 'outerloop;
                    }
                }
            }

            let area = point_1.0.abs_diff(point_2.0).add(1) * point_1.1.abs_diff(point_2.1).add(1);
            max_area = max_area.max(area);
        }
    }

    max_area
}

fn main() {
    run_day(9, part1, part2);
}
