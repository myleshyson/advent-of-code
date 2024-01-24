import { readFileSync } from "fs";
import { Answer } from "../index.js";

export default class Solution implements Answer {
  matrix: string[][] = []

  solve(): void {
    const content = readFileSync(import.meta.dirname + '/input.txt', 'utf8')
    const matrix = content.split('\n').filter(line => line.length).map(line => line.split(''))
    let sum = 0

    for (let columnIndex = 0; columnIndex < matrix[0].length; columnIndex++) {
      let availableSpace = 0

      for (let rowIndex = 0; rowIndex < matrix.length; rowIndex++) {
        let load = matrix.length - rowIndex
        const char = matrix[rowIndex][columnIndex]

        if (char === '.') {
          availableSpace++
          continue
        }

        if (char === '#') {
          availableSpace = 0
          continue
        }

        if (availableSpace > 0) {
          load += availableSpace
          matrix[rowIndex][columnIndex] = '.'
          matrix[rowIndex - availableSpace][columnIndex] = char
        }

        sum += load
      }
    }

    console.log(sum)
  }
}
