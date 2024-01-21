import { readFileSync } from "fs";
import { Answer } from "../index.js";

export default class Solution implements Answer {
  solve(): void {
    let fileContent: any = readFileSync(import.meta.dirname + '/input.txt', 'utf8')

    fileContent = fileContent.split('\n\n').map((block: string) => {
      return block.split('\n').map((line: string) => line.split('')).filter((line: string[]) => line.length)
    })

    let sum = 0
    let index = 0

    for (const matrix of (fileContent as string[][][])) {
      let one = this.getVerticalReflectionLength(matrix, index)
      let two = this.getHorizontalReflectionLength(matrix, index)

      if (!one && !two) {
        console.log('index: ' + index + ' results: ' + one + ':' + two)
      }

      index++
      sum += one + two

    }

    console.log(sum)
  }

  getHorizontalReflectionLength(matrix: string[][], index = 10) {
    const columnLength = matrix[0].length
    const rowLength = matrix.length
    let horizontalPoints = []

    for (let row = 0; row < (rowLength - 1); row++) {
      let differences = 0
      const nextRow = row + 1

      for (let column = 0; column < columnLength; column++) {
        if (matrix[row][column] !== matrix[nextRow][column]) {
          differences++
        }
      }

      if (differences === 0 || differences === 1) {
        horizontalPoints.push([row, nextRow])
      }
    }

    if (!horizontalPoints.length) return 0

    mirrorLoop: for (const [north, south] of horizontalPoints) {
      const mirrorLength = Math.min(north, matrix.length - south - 1)

      let start = north - mirrorLength
      let end = south + mirrorLength
      let differences = 0

      while (start < end) {
        for (let column = 0; column < columnLength; column++) {
          if (matrix[start][column] !== matrix[end][column]) {
            differences++
          }
        }

        start++
        end--
      }

      if (differences !== 1) continue mirrorLoop

      return (north + 1) * 100
    }

    return 0
  }

  getVerticalReflectionLength(matrix: string[][], index) {
    const columnLength = matrix[0].length
    const rowLength = matrix.length
    let verticalPoints = []

    for (let column = 0; column < (columnLength - 1); column++) {
      let differences = 0
      const nextColumn = column + 1

      for (let row = 0; row < rowLength; row++) {
        if (matrix[row][column] !== matrix[row][nextColumn]) {
          differences++
        }
      }

      if (differences === 0 || differences === 1) {
        verticalPoints.push([column, nextColumn])
      }
    }

    if (!verticalPoints.length) return 0

    mirrorLoop: for (const [east, west] of verticalPoints) {
      const mirrorLength = Math.min(east, matrix[0].length - west - 1)
      let start = east - mirrorLength
      let end = west + mirrorLength
      let differences = 0

      while (start < end) {
        for (let row = 0; row < rowLength; row++) {
          if (matrix[row][start] !== matrix[row][end]) {
            differences++
          }
        }

        start++
        end--
      }

      if (differences !== 1) continue mirrorLoop

      return east + 1
    }

    return 0
  }
}
