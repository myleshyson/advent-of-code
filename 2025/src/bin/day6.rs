use std::collections::HashMap;

use aoc_2025::run_day;

fn part1(input: &str) -> u64 {
    let operator_idx = input
        .find(&['+', '*'])
        .expect("no operators for some reason");
    let num_str: &str = &input[0..operator_idx];
    let op_str: &str = &input[operator_idx..];
    let op_vec: Vec<char> = op_str.chars().filter(|ch| !ch.is_whitespace()).collect();
    let num_iter = num_str.lines();
    let mut sums: Vec<u64> = Vec::new();

    for line in num_iter {
        for (j, ch) in line.split_whitespace().enumerate() {
            let num: u64 = ch.parse().unwrap();

            if !sums.get(j).is_some() {
                sums.push(num);
                continue;
            }

            let op = op_vec.get(j).expect("something is wrong. can't get op");
            match op {
                '+' => sums[j] += num,
                '*' => sums[j] *= num,
                _ => unreachable!(),
            }
        }
    }

    sums.iter().sum()
}

fn part2(input: &str) -> u64 {
    let operator_idx = input
        .find(&['+', '*'])
        .expect("no operators for some reason");
    let num_str: &str = &input[0..operator_idx].trim_matches(&['\n']);
    let op_str: &str = &input[operator_idx..].trim_matches(&['\n']);
    let op_vec: Vec<(usize, usize, char)> = op_str
        .char_indices()
        .rev()
        .enumerate()
        .filter(|(_, (_, ch))| !ch.is_whitespace())
        .scan(op_str.len(), |range_end, (_, (i, ch))| {
            let result = (i, *range_end, ch);
            *range_end = i - 1;
            Some(result)
        })
        .collect();

    let mut num_vec: HashMap<usize, Option<u64>> = HashMap::new();

    for line in num_str.lines().rev() {
        for (start, end, _) in op_vec.iter() {
            for j in *start..*end {
                let ch = line.chars().nth(j).expect("expected character here");
                num_vec
                    .entry(j)
                    .and_modify(|val| {
                        if ch.is_whitespace() {
                            return;
                        }
                        let digit = ch.to_digit(10).expect("invalid digit") as u64;
                        if val.is_none() {
                            *val = Some(digit);
                            return;
                        }

                        let num_digits = val.unwrap().ilog10() + 1;
                        *val = Some(val.unwrap() + digit * 10_u64.pow(num_digits));
                    })
                    .or_insert(if ch.is_whitespace() {
                        None
                    } else {
                        Some(ch.to_digit(10).expect("invalid digit") as u64)
                    });
            }
        }
    }

    let mut ans = 0;
    for (start, end, op) in op_vec.iter() {
        let mut cur_sum = 0;
        for i in *start..*end {
            if let Some(num) = num_vec.get(&i) {
                if cur_sum == 0 {
                    cur_sum = num.unwrap();
                    continue;
                }

                cur_sum = match op {
                    '+' => cur_sum + num.unwrap(),
                    '*' => cur_sum * num.unwrap(),
                    _ => unreachable!(),
                }
            }
        }
        ans += cur_sum;
    }

    ans
}

fn main() {
    run_day(6, part1, part2);
}
