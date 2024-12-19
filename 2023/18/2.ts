
import { readFileSync } from "fs"
import { Answer } from "../index.js"

const directions = {
  'R': [0, 1],
  'L': [0, -1],
  'U': [-1, 0],
  'D': [1, 0],
} as const

const hexDirections = {
  '0': 'R',
  '1': 'D',
  '2': 'L',
  '3': 'U'
}

type Direction = keyof typeof directions
type HexDirection = keyof typeof hexDirections

export default class Solution implements Answer {

  solve() {
    type Instruction = [Direction, number]

    const instructions: Instruction[] = readFileSync(import.meta.dirname + '/input.txt', 'utf8')
      .split("\n")
      .filter(line => line.trim().length > 0)
      .map((line) => line.split(" "))
      .map((line) => line[2].slice(2, line[2].length - 1))
      .map((data: string) => [
        hexDirections[data.slice(-1) as HexDirection] as Direction,
        parseInt(data.slice(0, 5), 16),
      ])

    let points: [number, number][] = [[0, 0]]
    let boundryPoints = 0

    for (const [direction, length] of instructions) {
      const [dr, dc] = directions[direction]
      const [row, col] = points.slice(-1)[0]
      boundryPoints += length
      points.push([row + dr * length, col + dc * length])
    }

    // first use shoelace formula to get the area of the polygon.
    // it gets the area from the middle of the point though, which is why 
    // picks theorem is used. 
    // Once we have the area, we can use picks theorom to get the interior points.
    // the interior points + the boundry points equals the total amount of points
    // in our polygon.
    //
    //
    // shoelace formula
    // Sum of x[i] * (y[i - 1] - y[i + 1])
    // y's loop.
    const exteriorArea = points.reduce((accum, value, index) => {
      const nextPoint = points[(index + 1) % points.length]
      const previousPoint = index === 0 ? points[points.length - 1] : points[index - 1]
      return accum + (value[0] * (nextPoint[1] - previousPoint[1]))
    }, 0)

    const area = Math.abs(exteriorArea) / 2

    // picks theorem
    // interiorPoints = (Area - (boundryPoints / 2)) + 1
    const interior = (area - (boundryPoints / 2)) + 1
    console.log(boundryPoints + interior)
  }
}
