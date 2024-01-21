import { Answer } from "../index.js";
import { getReadLineStream, resolveStream } from "../input.js";

export async function makeMatrix(): Promise<string[][]> {
  const stream = getReadLineStream(import.meta.dirname + '/input.txt')
  let currentRow = -1
  let matrix: string[][] = []

  stream.on('line', line => {
    currentRow++
    matrix[currentRow] = []

    for (let index = 0; index < line.length; index++) {
      const character = line[index]
      matrix[currentRow].push(character)
    }
  })

  await resolveStream(stream)

  return matrix
}

export function isNumber(character: string | undefined): boolean {
  if (typeof character === 'undefined') {
    return false
  }

  return character.charCodeAt(0) >= 48 && character.charCodeAt(0) <= 57
}

export function isSymbol(character: string | undefined): boolean {
  if (typeof character === 'undefined') {
    return false
  }

  return !isNumber(character) && character !== '.'
}

export function getFullNumber(matrix: string[][], row: number, column: number): number[] {
  let numberArray: string[] = [matrix[row][column]]
  let leftSide = matrix[row][column - 1]
  let leftColumn = column - 1
  let rightColumn = column + 1
  let rightSide = matrix[row][column + 1]

  while (isNumber(leftSide)) {
    numberArray = [
      leftSide,
      ...numberArray
    ]

    leftColumn--
    leftSide = matrix[row][leftColumn]
  }

  while (isNumber(rightSide)) {
    numberArray.push(rightSide)
    rightColumn++
    rightSide = matrix[row][rightColumn]
  }

  return [parseInt(numberArray.join(''), 10), leftColumn - 1, rightColumn - 1]
}

export function searchAround(matrix: string[][], row: number, column: number, search: (character: string | undefined) => boolean): boolean {
  let hasThing = false

  for (let i = row - 1; i <= (row + 1); i++) {
    if (!matrix[i]) {
      continue
    }

    for (let j = column - 1; j <= (column + 1); j++) {
      if (!matrix[i][j]) {
        continue
      }

      if (search(matrix[i][j])) {
        hasThing = true
        break
      }
    }
  }

  return hasThing
}

export default class Solution implements Answer {
  async solve() {
    let matrix: (string)[][] = await makeMatrix()
    let partNumberSum = 0

    for (let row = 0; row < matrix.length; row++) {
      for (let column = 0; column < matrix[row].length; column++) {
        const character = matrix[row][column]
        if (!isNumber(character) || character === '.') {
          continue
        }

        let left = matrix[row][column - 1]
        let right = matrix[row][column + 1]
        let bottom
        let top
        let topRight
        let topLeft
        let bottomRight
        let bottomLeft

        if (matrix[row - 1]) {
          top = matrix[row - 1][column]
          topRight = matrix[row - 1][column + 1]
          topLeft = matrix[row - 1][column - 1]
        }

        if (matrix[row + 1]) {
          bottomRight = matrix[row + 1][column + 1]
          bottomLeft = matrix[row + 1][column - 1]
          bottom = matrix[row + 1][column]
        }

        if (searchAround(matrix, row, column, (char) => isSymbol(char))) {
          const [number, leftBoundry, rightBoundry] = getFullNumber(matrix, row, column)
          column = rightBoundry
          partNumberSum += number
        }
      }
    }

    console.log(partNumberSum)
  }
}
