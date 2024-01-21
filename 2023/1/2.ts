import { warn } from "console";
import { Answer } from "../index.js";
import { getReadLineStream, resolveStream } from "../input.js"

export default class Solution implements Answer {
  async solve(): Promise<void> {
    let sum = 0
    const stream = getReadLineStream(import.meta.dirname + '/input.txt')
    const wordMap: Map<string, number> = new Map([
      ['one', 1],
      ['two', 2],
      ['three', 3],
      ['four', 4],
      ['five', 5],
      ['six', 6],
      ['seven', 7],
      ['eight', 8],
      ['nine', 9],
    ])

    stream.on('line', (line) => {
      let numberArr = []
      const regex = /((?<!(\S*)?([0-9])|(one|two|three|four|five|six|seven|eight|nine)(\S*)?)(([0-9])|(one|two|three|four|five|six|seven|eight|nine)))((\S*)?((([0-9])|(one|two|three|four|five|six|seven|eight|nine))(?!(([0-9])|(one|two|three|four|five|six|seven|eight|nine)))))?/
      const match = line.match(regex)
      let firstMatch: string | number | undefined = match?.at(1)
      let secondMatch: string | number | undefined = match?.at(11)

      if (firstMatch && wordMap.has(firstMatch)) {
        firstMatch = wordMap.get(firstMatch)
      }

      if (secondMatch && wordMap.has(secondMatch)) {
        secondMatch = wordMap.get(secondMatch)
      }

      if (!firstMatch && !secondMatch) {
        console.log('nah dawg')
        process.exit(1)
      }

      if (!firstMatch) {
        numberArr.push(secondMatch, secondMatch)
      } else if (!secondMatch) {
        numberArr.push(firstMatch, firstMatch)
      } else {
        numberArr.push(firstMatch, secondMatch)
      }
      sum += parseInt(numberArr.join(''), 10)
    })

    await resolveStream(stream)

    console.log(sum)
  }
}
