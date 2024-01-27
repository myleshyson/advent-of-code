import { readFileSync } from "fs";
import { Answer } from "../index.js";

export default class Solution implements Answer {
  solve(): void {
    const steps = readFileSync(import.meta.dirname + '/input.txt', 'utf8').split(',')
    const hashMap = Array.from({ length: 256 }, () => new Map())

    for (const step of steps) {
      const [, key, operation, value] = step.match(/([^-=]+)(-|=)(\d)?/) || []
      const hash = this.hash(key)

      switch (operation) {
        case '=':
          hashMap[hash].set(key, value)
          break
        case '-':
          if (hashMap[hash].has(key)) {
            hashMap[hash].delete(key)
          }
          break
      }
    }

    let sum = 0

    for (let index = 0; index < hashMap.length; index++) {
      if (!hashMap[index].size) continue
      let boxNumber = index + 1

      let boxSlot = 0

      for (const focalLength of hashMap[index].values()) {
        boxSlot++
        sum += boxNumber * boxSlot * focalLength
      }
    }

    console.log(sum)
  }

  hash(string: string) {
    let sum = 0

    for (const char of string) {
      if (char === '\n') continue
      sum += char.charCodeAt(0)
      sum *= 17
      sum = sum % 256
    }

    return sum
  }
}
