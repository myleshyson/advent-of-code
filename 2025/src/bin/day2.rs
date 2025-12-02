use std::collections::{HashMap, HashSet};

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
            if it_repeats(num, 2) {
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

        let mut cache = HashSet::new();
        let mut prime_cache = HashMap::new();

        for num in range {
            if cache.contains(&num) {
                sum += num;
                continue;
            }

            let digits = num_digits(num);
            let primes = prime_cache.entry(num).or_insert(prime_factors(digits));

            for prime in primes.iter() {
                if it_repeats(num, *prime) {
                    cache.insert(num);
                    sum += num;
                    break;
                }
            }
        }
    }

    sum
}

fn num_digits(num: u64) -> u64 {
    ((num as f64).log10().floor()) as u64 + 1
}

fn prime_factors(num: u64) -> HashSet<u64> {
    let mut num = num;
    let mut factors = HashSet::new();
    let mut d = 2;

    while d * d <= num {
        while num % d == 0 {
            factors.insert(d);
            num /= d;
        }
        d += 1;
    }

    if num > 1 {
        factors.insert(num);
    }

    factors
}

fn it_repeats(num: u64, times: u64) -> bool {
    let digits = num_digits(num);

    if digits % times != 0 {
        return false;
    }

    let chunk_size = digits / times;
    let mut first_chunk = None;
    let mut num = num;

    for i in 0..times {
        let chunk = num % 10_u64.pow(chunk_size as u32);
        num /= 10_u64.pow(chunk_size as u32);

        if i == 0 {
            first_chunk = Some(chunk);
            continue;
        }

        if first_chunk.unwrap() != chunk {
            break;
        }

        if i == times - 1 {
            return true;
        }
    }

    return false;
}

fn main() {
    run_day(2, part1, part2);
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::collections::HashSet;

    #[test]
    fn test_prime_factors() {
        assert_eq!(HashSet::from([2, 3]), prime_factors(6));
        assert_eq!(HashSet::from([3]), prime_factors(9));
        assert_eq!(HashSet::from([7]), prime_factors(7));
    }

    #[test]
    fn test_it_repeats() {
        assert_eq!(true, it_repeats(111, 3));
        assert_eq!(true, it_repeats(11, 2));
        assert_eq!(true, it_repeats(123123123, 3));
        assert_eq!(false, it_repeats(115, 1));
    }
}
