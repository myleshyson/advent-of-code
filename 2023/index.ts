#!/usr/bin/env ./node_modules/.bin/tsx

const [day, level] = process.argv.slice(2)

if (!day) {
  console.log("Please provide the day you want to run!")
  process.exit(1)
}

if (!level) {
  console.log(`Please provide the level you want to run for day ${day}!`)
  process.exit(1)
}

export interface Answer {
  solve(): void,
}

const module = await import(`./${day}/${level}`)
const answer: Answer = new module.default()

const start = performance.now()
answer.solve()
const end = performance.now()
console.log(`execution time: ${end - start}ms`)
