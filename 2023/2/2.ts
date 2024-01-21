import { Answer } from "../index.js";
import { getReadLineStream, resolveStream } from "../input.js";

export default class Solution implements Answer {
  async solve(): Promise<void> {
    const stream = getReadLineStream(import.meta.dirname + '/input.txt')

    const handRegex = /([0-9]+) (blue|red|green)/g

    let powerSum = 0

    stream.on('line', function(line) {
      const resultsMatch = [...line.matchAll(handRegex)]
      const minMap: Map<string, number> = new Map([
        ['red', 0],
        ['green', 0],
        ['blue', 0]
      ])

      resultsMatch.forEach(match => {
        const numCubes = parseInt(match[1], 10)
        const color = match[2]

        if (minMap.get(color) < numCubes) {
          minMap.set(color, numCubes)
        }
      })

      let gamePower = 1
      for (const [color, minCubes] of minMap) {
        gamePower *= minCubes
      }
      powerSum += gamePower
    })

    await resolveStream(stream)

    console.log(powerSum)
  }
}
