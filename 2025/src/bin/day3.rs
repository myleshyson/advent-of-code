use aoc_2025::run_day;

fn part1(input: &str) -> u64 {
    input
        .lines()
        .filter(|line| !line.is_empty())
        .map(|line| {
            let chars = line.trim().as_bytes();
            let mut first_idx = 0;

            for i in 0..chars.len() - 1 {
                if chars[i] > chars[first_idx] {
                    first_idx = i;
                }
            }

            let mut second_idx = first_idx + 1;
            for i in first_idx + 1..chars.len() {
                if chars[i] > chars[second_idx] {
                    second_idx = i;
                }
            }

            let num = String::from(chars[first_idx] as char)
                + (chars[second_idx] as char).to_string().as_ref();
            num.parse::<u64>().expect("couldn't parse num")
        })
        .sum()
}

fn part2(input: &str) -> u64 {
    let iter = input
        .lines()
        .filter(|line| !line.is_empty())
        .map(|line| line.as_bytes())
        .into_iter();

    let mut sum = 0;

    for chars in iter {
        let mut idx = 0;
        let mut iteration = 0;
        let mut digits = Vec::with_capacity(12);
        while iteration < 12 {
            let start = if idx == 0 { 0 } else { idx + 1 };
            for i in start..=chars.len() - (12 - iteration) {
                if chars[i] > *chars.get(idx).unwrap_or(&b'0') {
                    idx = i;
                }
            }

            digits.push((chars[idx] as char).to_string());
            idx += 1;
            iteration += 1;
        }
        sum += digits
            .join("")
            .parse::<u64>()
            .expect("could not parse digit");
    }

    sum
}

fn main() {
    run_day(3, part1, part2);
}
