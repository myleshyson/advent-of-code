import { readFileSync } from "fs"
import { Answer } from "../index.js"
import inRange from 'lodash/inRange.js'
import min from 'lodash/min.js'

export default class Solution implements Answer {
  async solve(): Promise<void> {
    const content = readFileSync(import.meta.dirname + '/input.txt', 'utf8')
    let sections = content.split(/\n\n/)
    const seeds = [...sections[0].matchAll(/([0-9]+)/g)].map(match => parseInt(match[0], 10))
    sections = sections.slice(1)
    const cleanedSections = sections.map(string => {
      const matches = [...string.matchAll(/(\d+)\s+(\d+)\s+(\d+)/g)]
      return matches.map(([, destination, source, length]) => {
        return [parseInt(destination, 10), parseInt(source, 10), parseInt(length, 10)]
      })
    })
    let locationNumbers = []

    for (let seed of seeds) {
      for (let section of cleanedSections) {
        for (let [destination, source, length] of section) {
          if (inRange(seed, source, source + length)) {
            seed += destination - source
            break
          }
        }
      }
      locationNumbers.push(seed)
    }

    console.log(min(locationNumbers))
  }
}
