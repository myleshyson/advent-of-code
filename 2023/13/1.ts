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
      let one = this.getVerticalReflectionLength(matrix)
      let two = this.getHorizontalReflectionLength(matrix, index)

      index++
      sum += one + two
    }

    console.log(sum)
  }

  getHorizontalReflectionLength(matrix: string[][]) {
    const columnLength = matrix[0].length
    const rowLength = matrix.length
    let horizontalPoints = []

    for (let row = 0; row < (rowLength - 1); row++) {
      let reflection = true
      const nextRow = row + 1
      for (let column = 0; column < columnLength; column++) {
        if (matrix[row][column] !== matrix[nextRow][column]) {
          reflection = false
          break
        }
      }

      if (reflection) {
        horizontalPoints.push([row, nextRow])
      }
    }

    if (!horizontalPoints.length) return 0

    mirrorLoop: for (const [north, south] of horizontalPoints) {
      const mirrorLength = Math.min(north, matrix.length - south - 1)

      let start = north - mirrorLength
      let end = south + mirrorLength
      let perfectMirror = true

      while (start < end) {
        for (let column = 0; column < columnLength; column++) {
          if (matrix[start][column] !== matrix[end][column]) {
            continue mirrorLoop
          }
        }

        start++
        end--
      }

      return (north + 1) * 100
    }

    return 0
  }

  getVerticalReflectionLength(matrix: string[][]) {
    const columnLength = matrix[0].length
    const rowLength = matrix.length
    let verticalPoints = []

    for (let column = 0; column < (columnLength - 1); column++) {
      let reflection = true
      const nextColumn = column + 1

      for (let row = 0; row < rowLength; row++) {
        if (matrix[row][column] !== matrix[row][nextColumn]) {
          reflection = false
          break
        }
      }

      if (reflection) {
        verticalPoints.push([column, nextColumn])
      }
    }

    if (!verticalPoints.length) return 0

    mirrorLoop: for (const [east, west] of verticalPoints) {
      const mirrorLength = Math.min(east, matrix[0].length - west - 1)
      let start = east - mirrorLength
      let end = west + mirrorLength

      while (start < end) {
        for (let row = 0; row < rowLength; row++) {
          if (matrix[row][start] !== matrix[row][end]) {
            continue mirrorLoop
          }
        }

        start++
        end--
      }

      return east + 1
    }

    return 0
  }
}
