import { Answer } from "../index.js";
import { getReadLineStream, resolveStream } from "../input.js";

export default class Solution implements Answer {
  pipeMap: { [char: string]: string[] } = {}
  matrix: string[][] = []
  startCol: number = 0
  startRow: number = 0
  visited: Map<string, boolean> = new Map()

  async solve(): Promise<void> {
    const stream = getReadLineStream(import.meta.dirname + '/input.txt')
    this.startRow = 0
    this.startCol = 0
    this.pipeMap = {
      '|': ['N', 'S'],
      '-': ['E', 'W'],
      'L': ['N', 'E'],
      'J': ['N', 'W'],
      '7': ['S', 'W'],
      'F': ['S', 'E'],
      '.': [],
      'S': ['N', 'S', 'E', 'W']
    }

    let curLine = 0
    stream.on('line', line => {
      for (let i = 0; i < line.length; i++) {
        const char = line[i];
        if (!this.matrix[curLine]) {
          this.matrix[curLine] = []
        }
        this.matrix[curLine].push(char)
        if (char === 'S') {
          this.startRow = curLine
          this.startCol = i
        }
      }
      curLine++
    })

    await resolveStream(stream)

    let answer = this.traverseLoop(this.startRow, this.startCol)
    console.log(answer / 2)
  }

  traverseLoop(row: number, col: number): number {
    this.visited.set(`${row}:${col}`, true)
    let nextPipe = this.getNextPipe(row, col)
    let count = 1

    while (nextPipe.length) {
      count++
      this.visited.set(`${nextPipe[0]}:${nextPipe[1]}`, true)
      nextPipe = this.getNextPipe(nextPipe[0]!, nextPipe[1]!)
    }

    return count
  }

  getNextPipe(row: number, col: number): [number?, number?] {
    const matrixHeight = this.matrix.length
    const matrixLength = this.matrix[0].length
    const currentPipe = this.pipeMap[this.matrix[row][col]]
    const pipes = new Map([
      // our north pipe needs to connect south
      ['N', row > 0 ? [row - 1, col] : null],
      // our south pipe needs to connect north 
      ['S', row < matrixHeight - 1 ? [row + 1, col] : null],
      // our east pipe needs to connect west 
      ['E', col < matrixLength - 1 ? [row, col + 1] : null],
      // our west pipe needs to connect east
      ['W', col > 0 ? [row, col - 1] : null]
    ])

    for (const [direction, coords] of pipes) {
      if (!coords) continue
      if (!currentPipe.includes(direction)) continue
      const pipe = this.matrix[coords[0]][coords[1]]
      const pipeMap = this.pipeMap[pipe]
      if (!pipeMap.length) continue
      if (this.visited.get(`${coords[0]}:${coords[1]}`)) continue
      if (
        direction === 'N' && pipeMap.includes('S') ||
        direction === 'S' && pipeMap.includes('N') ||
        direction === 'E' && pipeMap.includes('W') ||
        direction === 'W' && pipeMap.includes('E')
      ) {
        return [coords[0], coords[1]]
      }
    }

    return []
  }
}
