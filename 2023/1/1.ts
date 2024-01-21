import { Answer } from "../index.js";
import { getReadLineStream, resolveStream } from "../input.js";

export default class Solution implements Answer {
  async getSum() {
    const stream = getReadLineStream(import.meta.dirname + '/input.txt')
    let sum = 0

    stream.on('line', (line) => {
      let start = 0
      let end = line.length - 1
      let firstDigit = null
      let lastDigit = null

      while (start < line.length && end >= 0 && !(firstDigit && lastDigit)) {
        const firstCharacterCode = line.charCodeAt(start)
        const lastCharacterCode = line.charCodeAt(end)

        if (!firstDigit && firstCharacterCode >= 48 && firstCharacterCode <= 57) {
          firstDigit = String.fromCharCode(firstCharacterCode)
        }

        if (!lastDigit && lastCharacterCode >= 48 && lastCharacterCode <= 57) {
          lastDigit = String.fromCharCode(lastCharacterCode)
        }

        start++
        end--
      }
      sum += parseInt(firstDigit + lastDigit, 10)
    })

    await resolveStream(stream)

    return sum
  }

  async solve() {
    const sum = await this.getSum()
    console.log(sum)
  }
}
