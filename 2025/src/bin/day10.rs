use std::collections::{HashSet, VecDeque};

use aoc_2025::run_day;
use good_lp::{
    Expression, Solution, SolverModel, Variable, constraint, default_solver, variable, variables,
};
use regex::Regex;

struct QueuItem<T> {
    state: T,
    steps: u32,
}
// a b c d e f
//
// (e + f) % 2 = 0
// (b + f) % 2 = 1
// (c + d + e) % 2 = 1
// (a + b + d) % 2 = 0
//
fn part1(input: &str) -> u32 {
    // target, buttons, ignoreing the joltage
    let re = Regex::new(r"\[(?P<target>.*)\] (?P<buttons>[^{]+)\{(?P<jolts>.*)+}").unwrap();
    let machines: Vec<(u16, Vec<u16>, &str)> = input
        .lines()
        .map(|line| {
            let chunks = re.captures(line).unwrap();
            (
                chunks.name("target").unwrap().as_str(),
                chunks.name("buttons").unwrap().as_str(),
                chunks.name("jolts").unwrap().as_str(),
            )
        })
        .map(|(target, buttons, jolts)| {
            let string_binary = target.replace(".", "0").replace("#", "1");
            let mut target_binary = u16::from_str_radix(&string_binary, 2).unwrap();
            target_binary = target_binary << 10 - string_binary.len();

            let mut button_vec: Vec<u16> = Vec::new();
            for button in buttons.trim().split_whitespace() {
                let mut num: u16 = 0;
                for char in button.chars() {
                    if char == '(' || char == ')' || char == ',' {
                        continue;
                    }

                    let index = 9 - (char.to_digit(10).unwrap() as u16);
                    let clear_mask: u16 = !(1 << index);
                    num = (num & clear_mask) | (1 << index);
                }
                button_vec.push(num);
            }
            (target_binary, button_vec, jolts)
        })
        .collect();

    let mut sum = 0;

    for (target, buttons, _) in machines.iter() {
        let mut queue: VecDeque<QueuItem<u16>> = VecDeque::new();
        queue.push_back(QueuItem { state: 0, steps: 0 });

        let mut visited: HashSet<u16> = HashSet::new();

        while let Some(item) = queue.pop_front() {
            visited.insert(item.state);

            if item.state == *target {
                sum += item.steps;
                break;
            }

            for button in buttons.iter() {
                let new_lights = item.state ^ button;

                if visited.contains(&new_lights) {
                    continue;
                }

                queue.push_back(QueuItem {
                    state: new_lights,
                    steps: item.steps + 1,
                });
            }
        }
    }

    sum
}

fn solve_buttons(buttons: &[Vec<usize>], target: &[i32]) -> Option<u32> {
    let mut vars = variables!();

    let button_vars: Vec<Variable> = buttons
        .iter()
        .map(|_| vars.add(variable().integer().min(0)))
        .collect();

    let objective: Expression = button_vars.iter().copied().sum();

    let mut problem = vars.minimise(objective).using(default_solver);

    for (pos, &target_val) in target.iter().enumerate() {
        let mut expr: Expression = 0.into();
        for (button_idx, button) in buttons.iter().enumerate() {
            if button.contains(&pos) {
                expr += button_vars[button_idx];
            }
        }
        problem = problem.with(constraint!(expr == target_val));
    }

    match problem.solve() {
        Ok(solution) => {
            let total: f64 = button_vars.iter().map(|&v| solution.value(v)).sum();
            Some(total.round() as u32)
        }
        Err(_) => None,
    }
}

fn part2(input: &str) -> u32 {
    let re = Regex::new(r"\[(?P<target>.*)\] (?P<buttons>[^{]+)\{(?P<jolts>.*)+}").unwrap();
    let machines: Vec<(Vec<Vec<usize>>, Vec<i32>)> = input
        .lines()
        .map(|line| {
            let chunks = re.captures(line).unwrap();
            (
                chunks.name("buttons").unwrap().as_str(),
                chunks.name("jolts").unwrap().as_str(),
            )
        })
        .map(|(buttons, jolts)| {
            let jolt_targets: Vec<i32> = jolts
                .split(',')
                .map(|num| num.parse::<i32>().expect("invalid number"))
                .collect();

            let mut button_vec = Vec::new();

            for button in buttons.trim().split_whitespace() {
                let mut indexes = vec![];

                for ch in button.chars() {
                    if ch == '(' || ch == ')' || ch == ',' {
                        continue;
                    }

                    let digit = ch.to_digit(10).expect("invalid digit") as usize;
                    indexes.push(digit);
                }
                button_vec.push(indexes);
            }

            (button_vec, jolt_targets)
        })
        .collect();

    let mut sum: u32 = 0;

    for (buttons, target) in machines.iter() {
        if let Some(presses) = solve_buttons(buttons, target) {
            sum += presses;
        }
    }

    sum
}

fn main() {
    run_day(10, part1, part2);
}
