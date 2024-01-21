import { Answer } from "../index.js";
import { getReadLineStream, resolveStream } from "../input.js";

export default class Solution implements Answer {
  pipeMap: { [char: string]: string[] } = {}
  matrix: string[][] = []
  startCol: number = 0
  startRow: number = 0
  visited: Map<string, boolean> = new Map()
  inside: Map<string, boolean> = new Map()

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
        let char = line[i];
        if (!this.matrix[curLine]) {
          this.matrix[curLine] = []
        }
        if (char === 'S') {
          this.startRow = curLine
          this.startCol = i
        }
        this.matrix[curLine].push(char)
      }
      curLine++
    })

    await resolveStream(stream)

    this.calculateInside()
  }

  traverseLoop(row: number, col: number): void {
    this.visited.set(`${row}:${col}`, true)
    let nextPipe = this.getNextPipe(row, col)

    while (nextPipe.length) {
      this.visited.set(`${nextPipe[0]}:${nextPipe[1]}`, true)
      nextPipe = this.getNextPipe(nextPipe[0]!, nextPipe[1]!)
    }
  }

  /**
   * @link https://en.m.wikipedia.org/wiki/Shoelace_formula
   * @link https://en.wikipedia.org/wiki/Pick%27s_theorem
   */
  calculateInside() {
    this.traverseLoop(this.startRow, this.startCol)
    const keys = [...this.visited.keys()].map(key => key.split(':').map(key => parseInt(key, 10)))
    let shoelace = 0

    for (let i = 0; i < keys.length; i++) {
      const [nextY, nextX] = keys[(i + 1) % keys.length]
      const [y, x] = keys[i]
      shoelace += (x * nextY) - (y * nextX)
    }
    shoelace = shoelace / 2
    console.log(shoelace - (keys.length / 2) + 1)
  }

  getNextPipe(row: number, col: number): [number?, number?] {
    const getSurroundingPipes = this.getSurroundingPipes(row, col)
    return getSurroundingPipes[0] || []
  }

  getSurroundingPipes(row: number, col: number): [number?, number?][] {
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

    let results: [number?, number?][] = []
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
        results.push([coords[0], coords[1]])
      }
    }

    return results
  }

  // debugging what counts as a boundry
  printMap() {
    for (let i = 0; i < this.matrix.length; i++) {
      for (let j = 0; j < this.matrix[i].length; j++) {
        let string = []
        const char = this.matrix[i][j]
        if (this.visited.get(`${i}:${j}`)) {
          string.push('\u001b[31m')
        } else if (this.inside.get(`${i}:${j}`)) {
          string.push('\u001b[36m')
        }
        switch (char) {
          case '|':
            string.push('║')
            break
          case '-':
            string.push('═')
            break
          case 'L':
            string.push('╚')
            break
          case 'J':
            string.push('╝')
            break
          case 'F':
            string.push('╔')
            break
          case '7':
            string.push('╗')
            break
          case 'S':
            string.push('$')
            break
          default:
            string.push('.')
            break
        }
        if (this.visited.get(`${i}:${j}`)) {
          string.push('\u001b[0m')
        }
        process.stdout.write(string.join(''))
      }
      process.stdout.write("\n")
    }
  }
}
