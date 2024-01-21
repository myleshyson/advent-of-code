import { Answer } from "../index.js"
import { getReadLineStream, resolveStream } from "../input.js"

export default class Solution implements Answer {
  async solve(): Promise<void> {
    const stream = getReadLineStream(import.meta.dirname + '/input.txt')
    let times: number[] = []
    let distances: number[] = []

    stream.on('line', line => {
      if (!times.length) {
        times = [...line.matchAll(/(\d+)/g)].map(match => parseInt(match[1], 10))
        return
      }

      if (!distances.length) {
        distances = [...line.matchAll(/(\d+)/g)].map(match => parseInt(match[1], 10))
        return
      }
    })

    await resolveStream(stream)


    let answers = []
    for (let i = 0; i < times.length; i++) {
      const totalTime = times[i]
      const record = distances[i]

      let quad = (totalTime - Math.sqrt((Math.pow(totalTime, 2) - 4 * record))) / 2
      let min = Math.trunc(quad + 1)

      let half = Math.floor(totalTime / 2)
      let numWaysToBeat = 0

      while (half > 0) {
        let buttonPressedTime = half
        const distanceTravled = this.getDistanceTraveld(totalTime, buttonPressedTime)

        if (distanceTravled > record) {
          numWaysToBeat += 2
        }

        half--
      }
      // odd nubmers have 2 middles, even numbers just have one
      // since we're only counting from floor(number / 2) -> 1
      if (totalTime % 2 === 0) {
        numWaysToBeat--
      }

      if (numWaysToBeat > 0) {
        answers.push(numWaysToBeat)
      }
    }

    console.log(answers.reduce((prev, current) => current * prev))
  }

  getDistanceTraveld(totalTime: number, buttonPressedTime: number): number {
    return (totalTime - buttonPressedTime) * buttonPressedTime
  }
}
