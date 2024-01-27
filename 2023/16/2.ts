import { readFileSync } from "fs";
import { Answer } from "../index.js";

export default class Solution implements Answer {
  pathCache = new Map()
  visited = new Map()
  test = 0

  solve(): void {
    const matrix = readFileSync(import.meta.dirname + '/input.txt', 'utf8')
      .split('\n')
      .filter(line => line.length)
      .map(line => line.split(''))

    this.traverseHorizontal(matrix, 'right', 0, 0)

    console.log(this.visited.size)
  }

  traverseHorizontal(matrix: any[][], direction: string, rowStart: number, columnStart: number) {
    const key = `${rowStart}-${columnStart}-${direction}`

    if (this.pathCache.has(key)) return

    this.pathCache.set(key, true)

    let nums = 0

    for (
      let index = columnStart;
      direction === 'right'
        ? index < matrix[rowStart].length
        : index >= 0;
      direction === 'right'
        ? index++
        : index--
    ) {
      const char = matrix[rowStart][index]
      const visitedKey = `${rowStart}-${index}`

      if (!this.visited.has(visitedKey)) {
        this.visited.set(visitedKey, true)
        nums++
      }

      const numsFromSwitching = this.switchDirections(matrix, char, direction, rowStart, index)

      if (numsFromSwitching > 0) return nums + numsFromSwitching
    }
  }

  traverseVertical(matrix: any[][], direction: string, rowStart: number, columnStart: number) {
    const key = `${rowStart}-${columnStart}-${direction}`

    if (this.pathCache.has(key)) return

    this.pathCache.set(key, true)

    let nums = 0

    for (
      let index = rowStart;
      direction === 'down'
        ? index < matrix.length
        : index >= 0;
      direction === 'down'
        ? index++
        : index--
    ) {
      const char = matrix[index][columnStart]
      const visitedKey = `${index}-${columnStart}`

      if (!this.visited.has(visitedKey)) {
        this.visited.set(visitedKey, true)
        nums++
      }

      const numsFromSwitching = this.switchDirections(matrix, char, direction, index, columnStart)
      if (numsFromSwitching > 0) return nums + numsFromSwitching
    }
  }

  switchDirections(matrix: any[][], char: string, direction: string, row: number, column: number) {
    if (char === '|' && (direction === 'right' || direction === 'left')) {
      this.traverseVertical(matrix, 'up', row - 1, column)
      this.traverseVertical(matrix, 'down', row + 1, column)
      return true
    }

    if (char === '-' && (direction === 'up' || direction === 'down')) {
      this.traverseHorizontal(matrix, 'left', row, column - 1)
      this.traverseHorizontal(matrix, 'right', row, column + 1)
      return true
    }

    if (char === '/' && direction === 'left') {
      this.traverseVertical(matrix, 'down', row + 1, column)
      return true
    }

    if (char === '\\' && direction === 'left') {
      this.traverseVertical(matrix, 'up', row - 1, column)
      return true
    }

    if (char === '/' && direction === 'right') {
      this.traverseVertical(matrix, 'up', row - 1, column)
      return true
    }

    if (char === '\\' && direction === 'right') {
      this.traverseVertical(matrix, 'down', row + 1, column)
      return true
    }

    if (char === '/' && direction === 'up') {
      this.traverseHorizontal(matrix, 'right', row, column + 1)
      return true
    }

    if (char === '\\' && direction === 'up') {
      this.traverseHorizontal(matrix, 'left', row, column - 1)
      return true
    }

    if (char === '/' && direction === 'down') {
      this.traverseHorizontal(matrix, 'left', row, column - 1)
      return true
    }

    if (char === '\\' && direction === 'down') {
      this.traverseHorizontal(matrix, 'right', row, column + 1)
      return true
    }

    return false
  }
}
