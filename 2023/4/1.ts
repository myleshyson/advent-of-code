import { Answer } from "../index.js";
import { getReadLineStream, resolveStream } from "../input.js";
import { getWinningNumbers } from "./utils.js";

export default class Solution implements Answer {
  async solve() {
    const stream = getReadLineStream(import.meta.dirname + '/input.txt')
    const numberRegex = /Card +[0-9]+: +([0-9 ]+) +\| +([0-9 ]+)/
    let answer = 0

    stream.on('line', (line) => {
      const lineIterator: IterableIterator<string> = line[Symbol.iterator]()
      const winningSet: Set<string> = getWinningNumbers(lineIterator)
      let currentNumber: string[] = []
      let pow = 0.5

      for (const char of lineIterator) {
        if (char === ' ' && currentNumber.length) {
          if (winningSet.has(currentNumber.join(''))) {
            pow *= 2
          }
          currentNumber = []
        } else if (char !== ' ' && char !== '|') {
          currentNumber.push(char)
        }
      }

      if (winningSet.has(currentNumber.join(''))) {
        pow *= 2
      }

      if (pow > 0.5) {
        answer += pow
      }
    })

    await resolveStream(stream)

    console.log(answer)
  }
}
