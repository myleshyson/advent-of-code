use aoc_2025::run_day;

fn part1(input: &str) -> u64 {
    let mut sum = 0;

    for pair in input.split(",").map(|id| id.trim()) {
        let separator = pair.find("-").expect("no - found in id");
        let first: &str = &pair[0..separator];
        let second: &str = &pair[separator + 1..];
        let range = first.parse::<u64>().expect("invalid first number")
            ..=second
                .parse::<u64>()
                .expect(format!("invalid second number: {}", second).as_str());

        for num in range {
            let string_num = num.to_string();

            if string_num.len() % 2 != 0 {
                continue;
            }
            let half_point = string_num.len() / 2;
            let first_half: &str = &string_num[0..half_point];
            let second_half: &str = &string_num[half_point..];

            if first_half == second_half {
                sum += num;
            }
        }
    }

    sum
}

fn part2(input: &str) -> u64 {
    let mut sum = 0;

    for pair in input.split(",").map(|id| id.trim()) {
        let separator = pair.find("-").expect("no - found in id");
        let first: &str = &pair[0..separator];
        let second: &str = &pair[separator + 1..];
        let range = first.parse::<u64>().expect("invalid first number")
            ..=second
                .parse::<u64>()
                .expect(format!("invalid second number: {}", second).as_str());

        for num in range {
            let string_num = num.to_string();
            let repeated = string_num.clone().repeat(2);

            if repeated.len() > 2 && repeated[1..repeated.len() - 1].contains(&string_num) {
                sum += num;
            }
        }
    }

    sum
}

fn main() {
    run_day(2, part1, part2);
}
