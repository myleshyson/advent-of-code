import { readFileSync } from "fs";
import { Answer } from "../index.js";

export default class Solution implements Answer {
  content = ''
  matrix: string[][] = []
  cache = new Map()
  originalSnapshot = ''

  solve(): void {
    const content = readFileSync(import.meta.dirname + '/input.txt', 'utf8')
    const matrix = content.split('\n').filter(line => line.length).map(line => line.split(''))

    console.log(this.cycle(matrix, 1000000000))
  }

  cycle(matrix: string[][], times: number) {
    for (let count = 0; count < times; count++) {
      for (let index = 0; index < 4; index++) {
        const beforeSnap = this.getSnapshot(matrix)
        const hit = this.cache.get(beforeSnap)

        if (hit && hit[1] === 0) {
          // (count - hit[0])x = 24 - count
          // x = (100 - count) / (count - hit[0])
          const whenRepeats = (times - count) / (count - hit[0])

          if (Number.isInteger(whenRepeats)) {
            return this.getLoad(matrix)
          }
        }

        if (!this.cache.has(beforeSnap)) {
          this.cache.set(beforeSnap, [count, index])
        }

        this.tilt(matrix)

        this.rotateMatrix(matrix)
      }
    }
  }

  getLoad(matrix: string[][]) {
    let n = matrix.length
    let sum = 0

    for (let i = 0; i < n; i++) {
      for (let j = 0; j < n; j++) {
        let load = n - j
        const char = matrix[j][i]

        if (char === 'O') {
          sum += load
        }
      }
    }
    return sum
  }

  tilt(matrix: string[][]) {
    let sum = 0
    let n = matrix.length

    for (let i = 0; i < n; i++) {
      let availableSpace = 0

      for (let j = 0; j < n; j++) {
        let load = n - j
        const char = matrix[j][i]

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
          matrix[j][i] = '.'
          matrix[j - availableSpace][i] = char
        }

        sum += load
      }
    }

    return sum
  }

  rotateMatrix(matrix: string[][]) {
    const n = matrix.length

    for (let i = 0; i < n / 2; i++) {
      for (let j = i; j < n - i - 1; j++) {
        const temp = matrix[i][j]
        matrix[i][j] = matrix[n - j - 1][i]
        matrix[n - j - 1][i] = matrix[n - i - 1][n - j - 1]
        matrix[n - i - 1][n - j - 1] = matrix[j][n - i - 1]
        matrix[j][n - i - 1] = temp
      }
    }
  }

  getSnapshot(matrix: string[][]) {
    let string = []

    for (const row of matrix) {
      for (const char of row) {
        string.push(char)
      }
      string.push('\n')
    }

    return string.join('')
  }

}
