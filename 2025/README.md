# Advent of Code 2025 🦀

## Setup

Export an environment variable called `AOC_SESSION` in your shell:
```shell
export AOC_SESSION=<your aoc session cookie value>
```

You can find your session cookie by logging into [adventofcode.com](https://adventofcode.com) and checking your browser's cookies.

## How to Run

The first time you run a day, it will automatically download the input from Advent of Code and save it to `inputs/{day}.txt`. Subsequent runs will read from the cached file.

Run a solution:
```shell
cargo run --release --bin day1
```

Run specific parts:
```shell
cargo run --release --bin day1 -- 1  # part 1
cargo run --release --bin day1 -- 2  # part 2
```

Run with test input (requires `inputs/{day}_test.txt` to exist.):
```shell
cargo run --release --bin day1 -- --test
```
