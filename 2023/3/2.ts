import { Answer } from "../index.js";
import { getFullNumber, isNumber, makeMatrix, searchAround } from "./1.js";

export default class Solution implements Answer {
  async solve() {
    const matrix = await makeMatrix()
    let ratio = 0

    for (let row = 0; row < matrix.length; row++) {
      for (let column = 0; column < matrix[row].length; column++) {
        const character = matrix[row][column]

        if (character !== '*') {
          continue
        }

        let numbers = []

        // mini matrix
        for (let i = row - 1; i <= (row + 1); i++) {
          if (!matrix[i]) {
            continue
          }
          for (let j = column - 1; j <= (column + 1); j++) {
            if (!matrix[i][j]) {
              continue
            }

            if (isNumber(matrix[i][j])) {
              const [number, leftBoundry, rightBoundry] = getFullNumber(matrix, i, j)
              numbers.push(number)
              j = rightBoundry + 1
            }
          }
        }

        if (numbers.length === 2) {
          ratio += numbers[0] * numbers[1]
        }
      }
    }

    console.log(ratio)
  }
}
