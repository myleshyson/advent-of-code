use std::{fs, path::Path};

const BASE_URL: &str = "https://adventofcode.com/2025/day/";

pub fn input(day: i32, test: bool) -> Result<String, Box<dyn std::error::Error>> {
    let filename = match test {
        true => format!("inputs/{day}_test.txt"),
        false => format!("inputs/{day}.txt"),
    };
    let path = Path::new(&filename);

    if path.exists() {
        return Ok(fs::read_to_string(path)?);
    }

    if test {
        return Err("need to provide a test input in inputs/{day}_test.txt".into());
    }

    let session =
        std::env::var("AOC_SESSION").expect("please define an AOC_SESSION variable in your shell");

    let content = tokio::runtime::Runtime::new().unwrap().block_on(async {
        let client = reqwest::Client::new();
        let resp = client
            .get(format!("{}{}/input", BASE_URL, day))
            .header("Cookie", format!("session={}", session))
            .send()
            .await?
            .text()
            .await?;
        Ok::<String, reqwest::Error>(resp)
    })?;

    fs::create_dir_all("inputs")?;

    fs::write(path, &content)?;

    Ok(content)
}

pub fn run_day<T: std::fmt::Display>(
    day: i32,
    part1: impl FnOnce(&str) -> T,
    part2: impl FnOnce(&str) -> T,
) {
    let args: Vec<String> = std::env::args().collect();
    let test = args.contains(&"--test".to_string());
    let part = args
        .iter()
        .filter(|&a| !a.contains("--"))
        .map(|s| s.as_str())
        .nth(1)
        .unwrap_or("both");

    let input = input(day, test).expect("failed to load input");

    match part {
        "1" => println!("{}", part1(&input)),
        "2" => println!("{}", part2(&input)),
        _ => {
            println!("{}\n{}", part1(&input), part2(&input));
        }
    }
}
