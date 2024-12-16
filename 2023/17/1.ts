import { readFileSync } from "fs";
import { Answer } from "../index.js";

export default class Solution implements Answer {
  solve(): void {
    const contentMatrix: number[][] = readFileSync(import.meta.dirname + '/input.txt', 'utf8')
      .split('\n')
      .filter(line => line.length)
      .map(line => line.split('').map(value => parseInt(value, 10)))


  }

  djikstra(matrix) {
    let distances = new Array(matrix.length)
    let sptSet = new Array(matrix.length)

    for (let index = 0; index < matrix.length; index++) {
      distances[index] = Number.MAX_VALUE
      sptSet[index] = false
    }

    distances[0] = 0
  }
}
