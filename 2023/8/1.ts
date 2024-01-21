import { readFileSync } from "fs";
import { Answer } from "../index.js";

export default class Solution implements Answer {
  solve(): void {
    const content = readFileSync(import.meta.dirname + '/input.txt', 'utf8')
    const instructions: string[] = content.match(/^([RL]+)/g)[0].split('')
    const directions: Map<string, [string, string]> = new Map([...content.matchAll(/(\w{3})\s+=\s+\((\w{3}),\s+(\w{3})\)/g)].map(match => [match[1], [match[2], match[3]]]))

    // start with first place on map
    let destination = 'AAA'
    let steps = 0

    while (destination !== 'ZZZ') {
      for (const instruction of instructions) {
        if (instruction === 'R') {
          destination = directions.get(destination)[1]
        } else {
          destination = directions.get(destination)[0]
        }
        steps++
        if (destination === 'ZZZ') break
      }
    }

    console.log(steps)
  }
}
