import { readFileSync } from "fs";
import { Answer } from "../index.js";

export default class Solution implements Answer {
  solve(): void {
    const steps = readFileSync(import.meta.dirname + '/input.txt', 'utf8').split(',')
    let answer = 0

    for (const step of steps) {
      let sum = 0
      for (const char of step) {
        if (char === '\n') continue
        sum += char.charCodeAt(0)
        sum *= 17
        sum = sum % 256
      }
      answer += sum
    }

    console.log(answer)
  }
}
