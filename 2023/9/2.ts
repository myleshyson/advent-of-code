import { Answer } from "../index.js";
import { getReadLineStream, resolveStream } from "../input.js";

export default class Solution implements Answer {
  async solve(): Promise<void> {
    const stream = getReadLineStream(import.meta.dirname + '/input.txt')
    let answer = 0

    stream.on('line', line => {
      let numbers: number[] = [...line.matchAll(/(-?\d+)/g)].map(match => parseInt(match[1], 10))
      let numbersStack = [numbers]
      let onlyZeros = false
      let currentIndex = 0

      while (!onlyZeros) {
        let differences = []
        let hasNonZeros = false
        for (let i = 0; i < numbersStack[currentIndex].length - 1; i++) {
          const num1: number = numbersStack[currentIndex][i]
          const num2: number = numbersStack[currentIndex][i + 1]
          const difference = num2 - num1
          if (difference !== 0) {
            hasNonZeros = true
          }
          differences.push(difference)
        }

        onlyZeros = !hasNonZeros
        numbersStack.push(differences)
        currentIndex++
      }
      let prevValue = 0

      while (numbersStack.length) {
        let numbers = numbersStack.pop()
        prevValue = numbers[0] - prevValue
      }
      answer += prevValue
    })

    await resolveStream(stream)

    console.log(answer)
  }
}
