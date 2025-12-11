use std::ops::Add;

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
    fn x(&self) -> u64 {
        self.0
    }
    fn y(&self) -> u64 {
        self.1
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
    let mut max_area = 0;

    for point_1 in points.iter() {
        'outerLoop: for point_2 in points.iter().filter(|&p| p != point_1) {
            let point_3 = Point(point_1.x(), point_2.y());
            let point_4 = Point(point_2.x(), point_1.y());

            if !point_in_polygon(&point_3, &edges) || !point_in_polygon(&point_4, &edges) {
                continue;
            }

            let rect_horizontal_edges = vec![(*point_1, point_4), (*point_2, point_3)];
            let rect_vertical_edges = vec![(*point_1, point_3), (*point_2, point_4)];

            for vertical_edge in edges.iter().filter(|e| e.0.x() == e.1.x()) {
                let top_edge = rect_horizontal_edges
                    .iter()
                    .sorted_by(|a, b| a.0.y().cmp(&b.0.y()))
                    .next()
                    .unwrap();
                let bottom_edge = rect_horizontal_edges
                    .iter()
                    .sorted_by(|a, b| b.0.y().cmp(&a.0.y()))
                    .next()
                    .unwrap();

                let min_x = top_edge.0.x().min(top_edge.1.x());
                let max_x = top_edge.0.x().max(top_edge.1.x());

                let min_vert_y = vertical_edge.0.y().min(vertical_edge.1.y());
                let max_vert_y = vertical_edge.0.y().max(vertical_edge.1.y());

                if vertical_edge.0.x() > min_x
                    && vertical_edge.0.x() < max_x
                    && (
                        // crosses top line
                        (min_vert_y < top_edge.0.y() && max_vert_y > top_edge.0.y())
                        // crosses bottom line
                        || (min_vert_y < bottom_edge.0.y() && max_vert_y > bottom_edge.0.y())
                        // crosses both lines
                        || (min_vert_y <= top_edge.0.y() && max_vert_y >= bottom_edge.0.y())
                        // contained within the lines
                        || (min_vert_y >= top_edge.0.y() && max_vert_y <= bottom_edge.0.y())
                    )
                {
                    break 'outerLoop;
                }
            }

            // for all edges in our shape that are horizontal, see if any pass through our square
            for horizontal_edge in edges.iter().filter(|e| e.0.y() == e.1.y()) {
                let left_edge = rect_vertical_edges
                    .iter()
                    .sorted_by(|a, b| a.0.x().cmp(&b.0.x()))
                    .next()
                    .unwrap();
                let right_edge = rect_vertical_edges
                    .iter()
                    .sorted_by(|a, b| b.0.x().cmp(&a.0.x()))
                    .next()
                    .unwrap();

                let min_y = left_edge.0.y().min(left_edge.1.y());
                let max_y = left_edge.0.y().max(left_edge.1.y());

                let min_horz_x = horizontal_edge.0.x().min(horizontal_edge.1.x());
                let max_horz_x = horizontal_edge.0.x().max(horizontal_edge.1.x());

                if horizontal_edge.0.y() > min_y
                    && horizontal_edge.0.y() < max_y
                    && (
                        // crosses left
                        (min_horz_x < left_edge.0.x() && max_horz_x > left_edge.0.x())
                   // crosses right
                   || (min_horz_x < right_edge.0.x() && max_horz_x > right_edge.0.x())
                   // crosses both
                   || (min_horz_x <= left_edge.0.x() && max_horz_x >= right_edge.0.x())
                   // contains
                   || (min_horz_x >= left_edge.0.x() && max_horz_x <= right_edge.0.x())
                    )
                {
                    break 'outerLoop;
                }
            }
            let area = point_1.0.abs_diff(point_2.0).add(1) * point_1.1.abs_diff(point_2.1).add(1);
            max_area = max_area.max(area);
        }
    }
    max_area
}

// AI had to help me with this. Definitely out of my depth with this problem
fn point_in_polygon(point: &Point, edges: &[(Point, Point)]) -> bool {
    let mut inside = false;
    let x = point.x();
    let y = point.y();

    for edge in edges {
        let x1 = edge.0.x();
        let y1 = edge.0.y();
        let x2 = edge.1.x();
        let y2 = edge.1.y();

        // on the vertical line. definitely inside
        if x == x1 && x == x2 && y >= y1.min(y2) && y <= y1.max(y2) {
            return true;
        }

        // on the horizontal line. definitely inside.
        if y == y1 && y == y2 && x >= x1.min(x2) && x <= x1.max(x2) {
            return true;
        }

        if x1 == x2 && (y > y1) != (y > y2) && x < x1 {
            inside = !inside;
        }
    }

    inside
}

fn main() {
    run_day(9, part1, part2);
}
