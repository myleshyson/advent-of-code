import { readFileSync } from "fs"
import { Answer } from "../index.js"
import min from 'lodash/min.js'

export default class Solution implements Answer {
  async solve(): Promise<void> {
    const content = readFileSync(import.meta.dirname + '/input.txt', 'utf8')
    let sections = content.split(/\n\n/)
    type Job = [number, number, number]
    let queue: Job[] = []

    for (const match of [...sections[0].matchAll(/((\d+) (\d+))/g)]) {
      const start = parseInt(match[2], 10)
      const end = start + parseInt(match[3], 10)
      queue.push([start, end, 0])
    }

    sections = sections.slice(1)

    const cleanedSections = sections.map(string => {
      const matches = [...string.matchAll(/(\d+)\s+(\d+)\s+(\d+)/g)]
      return matches.map(([, destination, source, length]) => {
        return [parseInt(destination, 10), parseInt(source, 10), parseInt(length, 10)]
      })
    })

    let locationNums = []
    let queueCount = 0

    while (queue.length) {
      let [start, end, sectionNum] = queue.pop()!
      queueCount++

      let noMatch = true

      if (sectionNum === 7) {
        locationNums.push(start)
        continue
      }

      for (const [destination, source, length] of cleanedSections[sectionNum]!) {
        const diff = destination - source
        const sourceUpperBound = source + length

        if (start >= sourceUpperBound || end <= source) {
          continue
        }

        noMatch = false

        if (start < source) {
          queue.push([start, source, sectionNum])
          start = source
        }

        if (end > sourceUpperBound) {
          queue.push([sourceUpperBound, end, sectionNum])
          end = sourceUpperBound
        }

        queue.push([start + diff, end + diff, sectionNum + 1])
        break
      }

      if (noMatch) {
        queue.push([start, end, sectionNum + 1])
      }
    }
    console.log(queueCount)
    console.log(min(locationNums))
  }
}

