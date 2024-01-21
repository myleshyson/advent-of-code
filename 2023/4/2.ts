import { Answer } from "../index.js";
import { getReadLineStream, resolveStream } from "../input.js";
import { getWinningNumbers } from "./utils.js";

export default class Solution implements Answer {
  async solve() {
    const stream = getReadLineStream(import.meta.dirname + '/input.txt')
    const cardRegex = /Card +([0-9]+):/
    const cardMap: Map<number, number> = new Map()

    stream.on('line', line => {
      const iterator: IterableIterator<string> = line[Symbol.iterator]()
      const cardNum: number = parseInt(line.match(cardRegex)[1], 10)
      const winningNumbers = getWinningNumbers(iterator)
      let currentNumber: string[] = []
      let cardCount = 0

      cardMap.set(cardNum, (cardMap.get(cardNum) || 0) + 1)

      let multiplier = cardMap.get(cardNum)

      for (const char of iterator) {
        if (char === ' ' && currentNumber.length) {
          if (winningNumbers.has(currentNumber.join(''))) {
            cardCount++
          }
          currentNumber = []
        } else if (char !== ' ' && char !== '|') {
          currentNumber.push(char)
        }
      }

      if (winningNumbers.has(currentNumber.join(''))) {
        cardCount++
      }

      for (let i = 1; i <= cardCount; i++) {
        const current = (cardMap.get(cardNum + i) || 0) + multiplier
        cardMap.set(cardNum + i, current)
      }
    })

    await resolveStream(stream)

    let sum = 0

    for (const [cardNum, count] of cardMap) {
      sum += count
    }

    console.log(sum)
  }
}
