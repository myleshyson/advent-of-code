use std::{
    collections::{HashMap, VecDeque},
    ops::Add,
};

use aoc_2025::run_day;
use itertools::Itertools;

fn part1(input: &str) -> u32 {
    let points: Vec<(u32, u32)> = input
        .lines()
        .map(|line| {
            let mut iter = line
                .split(",")
                .map(|num| num.parse::<u32>().expect("invalid num"));
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
struct Point(u32, u32);

impl Point {
    fn from_usize(x: usize, y: usize) -> Self {
        Point(x as u32, y as u32)
    }
    fn x(&self) -> u32 {
        self.0
    }

    fn y(&self) -> u32 {
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

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(u8)]
pub enum Cellstate {
    Empty = 0,
    Inside = 1,
    Outside = 2,
}

pub struct Grid {
    data: Vec<u8>,
    width: usize,
    height: usize,
}

impl Grid {
    pub fn new(width: usize, height: usize) -> Self {
        let total_cells = width * height;
        let bytes_needed = (total_cells / 4) + if total_cells % 4 == 0 { 0 } else { 1 };

        Grid {
            data: vec![0; bytes_needed],
            width,
            height,
        }
    }

    pub fn get(&self, x: usize, y: usize) -> Cellstate {
        if x >= self.width || y >= self.height {
            return Cellstate::Outside;
        }

        let pos = y * self.width + x;
        let byte_idx = pos / 4;
        let shift = (pos % 4) * 2;

        match (self.data[byte_idx] >> shift) & 0b11 {
            0 => Cellstate::Empty,
            1 => Cellstate::Inside,
            2 => Cellstate::Outside,
            _ => unreachable!(),
        }
    }

    pub fn set(&mut self, x: usize, y: usize, state: Cellstate) {
        if x >= self.width || y >= self.height {
            panic!("Out of bounds!");
        }

        let pos = y * self.width + x;
        let byte_idx = pos / 4;
        let shift = (pos % 4) * 2;
        let clear_mask = !(0b11 << shift);
        let new_val = (state as u8) << shift;
        self.data[byte_idx] = (self.data[byte_idx] & clear_mask) | new_val;
    }
}

fn part2(input: &str) -> u32 {
    let points: Vec<Point> = input
        .lines()
        .map(|line| {
            let mut iter = line
                .split(",")
                .map(|num| num.parse::<u32>().expect("invalid num"));
            Point(iter.next().unwrap(), iter.next().unwrap())
        })
        .collect();

    let xs: Vec<u32> = points.iter().map(|p| p.x()).sorted().dedup().collect();
    let ys: Vec<u32> = points.iter().map(|p| p.y()).sorted().dedup().collect();

    let x_to_idx: HashMap<u32, usize> = xs.iter().enumerate().map(|(i, &x)| (x, i)).collect();
    let y_to_idx: HashMap<u32, usize> = ys.iter().enumerate().map(|(i, &y)| (y, i)).collect();

    let width = xs.len();
    let height = ys.len();
    let mut grid = Grid::new(width, height);

    let mut prev: Option<Point> = None;
    for point in points.iter().chain(std::iter::once(&points[0])) {
        let compressed_point = Point::from_usize(x_to_idx[&point.x()], y_to_idx[&point.y()]);

        grid.set(
            compressed_point.x_usize(),
            compressed_point.y_usize(),
            Cellstate::Inside,
        );

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
                grid.set(x as usize, y as usize, Cellstate::Inside);
            }
        }

        prev = Some(compressed_point);
    }

    // fill outside. we're gonna go around the edge of the grid. since we already
    // filled the borders, now we'll just mark if anything is outside of the borders
    for y in 0..height {
        for x in 0..width {
            if (x != 0 && x != width - 1) && (y != 0 && y != height - 1) {
                continue;
            }

            if grid.get(x, y) == Cellstate::Empty {
                grid.set(x as usize, y as usize, Cellstate::Outside);

                let mut queue: VecDeque<Point> = VecDeque::new();
                queue.push_back(Point::from_usize(x, y));
                while let Some(point) = queue.pop_front() {
                    for neighbor in point.neighbors() {
                        let state = grid.get(neighbor.x_usize(), neighbor.y_usize());

                        if state == Cellstate::Outside {
                            continue;
                        }

                        if state == Cellstate::Empty {
                            grid.set(neighbor.x_usize(), neighbor.y_usize(), Cellstate::Outside);
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
                    if grid.get(x, y) == Cellstate::Outside {
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
