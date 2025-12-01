use aoc_2025::run_day;

fn parse_input(input: &str) -> Vec<(&str, u32)> {
    input
        .lines()
        .filter(|line| !line.is_empty())
        .filter_map(|line| line.split_at_checked(1))
        .map(|(direction, num)| (direction, num.parse::<u32>().expect("can't parse number")))
        .collect()
}

fn part1(input: &str) -> u32 {
    let mut hit_zero = 0;
    let mut current_num = 50;

    let turns = parse_input(input);
    for (turn, num) in turns {
        match turn {
            "L" => current_num = (current_num - num) % 100,
            "R" => current_num = (current_num + num) % 100,
            _ => panic!("invalid direction"),
        };

        if current_num == 0 {
            hit_zero += 1;
        }
    }

    hit_zero
}
// 50
//
fn part2(input: &str) -> u32 {
    let mut hit_zero: u32 = 0;
    let mut current_num: i32 = 50;

    let turns = parse_input(input);
    for (turn, num) in turns {
        let at_zero = current_num == 0;

        match turn {
            "L" => current_num = (current_num - (num as i32)),
            "R" => current_num = (current_num + (num as i32)),
            _ => panic!("invalid direction"),
        };

        if current_num >= 100 {
            hit_zero += (current_num / 100) as u32;
        } else if current_num <= 0 {
            hit_zero += ((current_num / 100).abs() + 1) as u32;
            // edge case. if the dial is at zero, and we go backwards, then we don't want to double count the
            // 0 here as a "hit". turning the dial didn't cause us to hit 0 in this case...we just started there.
            if at_zero {
                hit_zero -= 1;
            }
        }

        current_num = current_num.rem_euclid(100);
    }

    hit_zero
}

fn main() {
    run_day(1, part1, part2);
}
