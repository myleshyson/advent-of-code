import { Answer } from "../index.js";
import { getReadLineStream, resolveStream } from "../input.js";

export default class Solution implements Answer {
  async solve(): Promise<void> {
    const stream = getReadLineStream(import.meta.dirname + '/input.txt')
    const gameRegex = /Game ([0-9]+)/
    const handRegex = /([0-9]+) (blue|red|green)/g
    const maxMap: Map<string, number> = new Map([
      ['red', 12],
      ['green', 13],
      ['blue', 14]
    ])
    let numGames = 0

    stream.on('line', function(line) {
      const gameMatch = line.match(gameRegex)
      const resultsMatch = [...line.matchAll(handRegex)]
      const gameId = parseInt(gameMatch[1], 10)
      let valid = true

      resultsMatch.forEach(match => {
        if (maxMap.get(match[2]) < parseInt(match[1], 10)) {
          valid = false
        }
      })

      if (valid) numGames += gameId
    })

    await resolveStream(stream)

    console.log(numGames)
  }
}
