
import { readFileSync } from "fs"
import { Answer } from "../index.js"

const directions: { [key: string]: [number, number] } = {
  'R': [0, 1],
  'L': [0, -1],
  'U': [-1, 0],
  'D': [1, 0],
}
type Direction = keyof typeof directions

export default class Solution implements Answer {

  solve() {
    type Instruction = [Direction, number, string]

    const instructions: Instruction[] = readFileSync(import.meta.dirname + '/input.txt', 'utf8')
      .split("\n")
      .filter(line => line.trim().length > 0)
      .map((line) => line.split(" "))
      .map((data: string[]) => [
        data[0] as Direction,
        parseInt(data[1], 10),
        data[2].substr(1, data[2].length - 1)
      ])

    let points: [number, number][] = [[0, 0]]
    let boundryPoints = 0

    for (const [direction, length,] of instructions) {
      const [dr, dc] = directions[direction]
      const [row, col] = points.slice(-1)[0]
      boundryPoints += length
      points.push([row + dr * length, col + dc * length])
    }

    // shoelace formula
    const exteriorArea = points.reduce((accum, value, index) => {
      const nextPoint = points[(index + 1) % points.length]
      const previousPoint = index === 0 ? points[points.length - 1] : points[index - 1]
      return accum + (value[0] * (nextPoint[1] - previousPoint[1]))
    }, 0)

    const area = Math.abs(exteriorArea) / 2

    // picks theorem
    const interior = (area - (boundryPoints / 2)) + 1
    console.log(boundryPoints + interior)
  }
}
