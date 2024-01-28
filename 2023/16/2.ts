import { readFileSync } from "fs";
import { Answer } from "../index.js";

export default class Solution implements Answer {
  pathCache = new Map()
  visited = new Map()
  edgeCache = new Map()

  solve(): void {
    const matrix = readFileSync(import.meta.dirname + '/input.txt', 'utf8')
      .split('\n')
      .filter(line => line.length)
      .map(line => line.split(''))

    let max = -Infinity

    for (let row = 0; row < matrix.length; row++) {
      for (let column = 0; column < matrix[row].length; column++) {
        // if its internal, skip
        if (row > 0 && row < (matrix.length - 1) && column > 0 && column < (matrix[row].length - 1)) continue

        if (row === 0 && column === 0) {
          // right/down
          max = Math.max(this.getTotalEdgeTiles(matrix, 'down', row, column), max)
          max = Math.max(this.getTotalEdgeTiles(matrix, 'right', row, column), max)
          continue
        } else if (row === 0 && column === (matrix[row].length - 1)) {
          // left/down
          max = Math.max(this.getTotalEdgeTiles(matrix, 'down', row, column), max)
          max = Math.max(this.getTotalEdgeTiles(matrix, 'left', row, column), max)
          continue
        } else if (row === (matrix.length - 1) && column === 0) {
          // up/right
          max = Math.max(this.getTotalEdgeTiles(matrix, 'up', row, column), max)
          max = Math.max(this.getTotalEdgeTiles(matrix, 'right', row, column), max)
          continue
        } else if (row === (matrix.length - 1) && column === (matrix[row].length - 1)) {
          // up/left
          max = Math.max(this.getTotalEdgeTiles(matrix, 'up', row, column), max)
          max = Math.max(this.getTotalEdgeTiles(matrix, 'left', row, column), max)
          continue
        } else if (row === 0) {
          // down
          max = Math.max(this.getTotalEdgeTiles(matrix, 'down', row, column), max)
          continue
        } else if (row === (matrix.length - 1)) {
          // up
          max = Math.max(this.getTotalEdgeTiles(matrix, 'up', row, column), max)
          continue
        } else if (column === 0) {
          // right
          max = Math.max(this.getTotalEdgeTiles(matrix, 'right', row, column), max)
          continue
        } else if (column === (matrix[row].length - 1)) {
          // left
          max = Math.max(this.getTotalEdgeTiles(matrix, 'left', row, column), max)
          continue
        }
      }
    }

    console.log(max)
  }

  getTotalEdgeTiles(matrix, direction, row, column) {
    const key = `${row}-${column}-${direction}`

    this.pathCache = new Map()
    this.visited = new Map()

    if (this.edgeCache.has(key))
      return this.edgeCache.get(key)


    switch (direction) {
      case 'down':
      case 'up':
        return this.traverseVertical(matrix, direction, row, column)
      case 'left':
      case 'right':
        return this.traverseHorizontal(matrix, direction, row, column)
    }
  }

  traverseHorizontal(matrix: any[][], direction: string, rowStart: number, columnStart: number): number {
    const key = `${rowStart}-${columnStart}-${direction}`

    if (this.pathCache.has(key)) return 0

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

      if (numsFromSwitching !== -1) {
        this.edgeCache.set(key, nums + numsFromSwitching)
        return nums + numsFromSwitching
      }
    }

    this.edgeCache.set(key, nums)
    return nums
  }

  traverseVertical(matrix: any[][], direction: string, rowStart: number, columnStart: number): number {
    const key = `${rowStart}-${columnStart}-${direction}`

    if (this.pathCache.has(key)) return 0

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

      if (numsFromSwitching !== -1) {
        return nums + numsFromSwitching
      }
    }

    return nums
  }

  switchDirections(matrix: any[][], char: string, direction: string, row: number, column: number): number {
    if (char === '|' && (direction === 'right' || direction === 'left')) {
      let count = 0
      count += this.traverseVertical(matrix, 'up', row - 1, column)
      count += this.traverseVertical(matrix, 'down', row + 1, column)
      return count
    }

    if (char === '-' && (direction === 'up' || direction === 'down')) {
      let count = 0
      count += this.traverseHorizontal(matrix, 'left', row, column - 1)
      count += this.traverseHorizontal(matrix, 'right', row, column + 1)
      return count
    }

    if (char === '/' && direction === 'left') {
      return this.traverseVertical(matrix, 'down', row + 1, column)
    }

    if (char === '\\' && direction === 'left') {
      return this.traverseVertical(matrix, 'up', row - 1, column)
    }

    if (char === '/' && direction === 'right') {
      return this.traverseVertical(matrix, 'up', row - 1, column)
    }

    if (char === '\\' && direction === 'right') {
      return this.traverseVertical(matrix, 'down', row + 1, column)
    }

    if (char === '/' && direction === 'up') {
      return this.traverseHorizontal(matrix, 'right', row, column + 1)
    }

    if (char === '\\' && direction === 'up') {
      return this.traverseHorizontal(matrix, 'left', row, column - 1)
    }

    if (char === '/' && direction === 'down') {
      return this.traverseHorizontal(matrix, 'left', row, column - 1)
    }

    if (char === '\\' && direction === 'down') {
      return this.traverseHorizontal(matrix, 'right', row, column + 1)
    }

    return -1
  }
}
