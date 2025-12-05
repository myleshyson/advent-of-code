use std::{cmp::max, collections::BTreeSet};

use aoc_2025::run_day;

fn part1(input: &str) -> u64 {
    let (ranges, ids) = input.split_once("\n\n").expect("invalid input format");
    let intervals: Vec<(u64, u64)> = ranges
        .lines()
        .skip_while(|line| line.trim().is_empty())
        .map(|line| line.split_once("-").expect("invalid num"))
        .map(|pair| {
            (
                pair.0.parse::<u64>().expect("invalid"),
                pair.1.parse::<u64>().expect("invalid"),
            )
        })
        .collect();

    ids.lines()
        .filter(|line| !line.trim().is_empty())
        .map(|line| line.parse::<u64>().expect("invalid id"))
        .filter(|id| {
            intervals
                .iter()
                .any(|(start, end)| start <= id && end >= id)
        })
        .count() as u64
}

fn part2(input: &str) -> u64 {
    let (ranges, _) = input.split_once("\n\n").expect("invalid input format");
    let intervals: BTreeSet<(u64, u64)> = ranges
        .lines()
        .skip_while(|line| line.trim().is_empty())
        .map(|line| line.split_once("-").expect("invalid num"))
        .map(|pair| {
            (
                pair.0.parse::<u64>().expect("invalid"),
                pair.1.parse::<u64>().expect("invalid"),
            )
        })
        .collect();

    intervals
        .iter()
        .fold(Vec::new(), |mut accum: Vec<(u64, u64)>, &val| {
            if let Some(last_item) = accum.last_mut() {
                if val.0 <= last_item.1 {
                    last_item.1 = max(last_item.1, val.1)
                } else {
                    accum.push(val)
                }
                return accum;
            }

            accum.push(val);
            return accum;
        })
        .iter()
        .map(|(start, end)| end - start + 1)
        .sum()
}

fn main() {
    run_day(5, part1, part2);
}
