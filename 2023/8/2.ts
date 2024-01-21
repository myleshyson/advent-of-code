import { readFileSync } from "fs";
import { Answer } from "../index.js";

type CacheItem = {
  [key: string]: number
}
type Cache = {
  [direction: string]: CacheItem
}

export default class Solution implements Answer {
  instructions: string[] = []
  directions: Map<string, { [direction: string]: string }> | null = null
  cache: Cache = {
    L: {},
    R: {}
  }
  solve(): void {
    const content = readFileSync(import.meta.dirname + '/input.txt', 'utf8')
    this.instructions = content.match(/^([RL]+)/g)[0].split('')
    this.directions = new Map([...content.matchAll(/(\w{3})\s+=\s+\((\w{3}),\s+(\w{3})\)/g)].map(match => [match[1], { L: match[2], R: match[3] }]))

    let navigation = [...this.directions.keys()].filter(key => key.charAt(2) === 'A')
    let primeMap = navigation.map((key) => this.getStepsToZ(key, 0)).map(number => this.primeFactorization(number))

    const map = new Map()

    for (const primes of primeMap) {
      const localMap = new Map()
      for (const number of primes) {
        localMap.set(number, (localMap.get(number) || 0) + 1)
      }
      for (const [number, count] of localMap) {
        if (!map.has(number)) {
          map.set(number, count)
          continue
        }

        if (map.get(number) < count) {
          const diff = count - map.get(number)
          const newCount = map.get(number) + diff
          map.set(number, newCount)
        }
      }
    }

    let lcm = 1

    for (const [number, count] of map) {
      lcm *= (number * count)
    }
    console.log(lcm)
  }

  getStepsToZ(key: string, instructionIndex: number): number {
    let instruction: string = this.instructions[instructionIndex]
    let result = key
    let currentIndex = instructionIndex
    let count = 0

    while (result.indexOf('Z') === -1) {
      let currentInstruction: string = this.instructions[currentIndex]
      result = this.directions.get(result)[currentInstruction]

      if ((currentIndex + 1) >= this.instructions.length) {
        currentIndex = 0
      } else {
        currentIndex++
      }

      count++
    }

    return count
  }

  primeFactorization(number) {
    let nums = []

    while (number % 2 === 0) {
      nums.push(2)
      number = number / 2
    }

    for (let i = 3; i <= Math.sqrt(number); i = i + 2) {
      while (number % i === 0) {
        nums.push(i)
        number = number / i
      }
    }

    if (number > 2) {
      nums.push(number)
    }

    return nums
  }
}
