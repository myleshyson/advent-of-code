import { Answer } from "../index.js";
import { getReadLineStream, resolveStream } from "../input.js";

export default class Solution implements Answer {
  matrix: string[][] = []

  async solve(): Promise<void> {
    const stream = getReadLineStream(import.meta.dirname + '/input.txt')
    const galaxyMap: Map<number, [number, number]> = new Map()
    const columnsWithouGalaxy: Map<number, true> = new Map()
    const rowsWithoutGalaxy: Map<number, true> = new Map()
    let galaxyIncrementor = 0
    let rowI = 0

    stream.on('line', line => {
      let noGalaxies = true
      let chars: string[] = []
      let i = 0

      if (rowI === 0) {
        for (let i = 0; i < line.length; i++) {
          columnsWithouGalaxy.set(i, true)
        }
      }

      for (const char of line) {
        if (char !== '.') {
          noGalaxies = false
          galaxyIncrementor++
          galaxyMap.set(galaxyIncrementor, [rowI, i])
          if (columnsWithouGalaxy.has(i)) {
            columnsWithouGalaxy.delete(i)
          }
        }
        chars.push(char)
        i++
      }

      this.matrix.push(chars)

      if (noGalaxies) {
        rowsWithoutGalaxy.set(rowI, true)
      }
      rowI++
    })

    await resolveStream(stream)

    let galaxyMapCopy = new Map(galaxyMap)

    for (const [column,] of columnsWithouGalaxy) {
      for (const [galaxy, [x, y]] of galaxyMapCopy) {
        if (y > column) {
          galaxyMap.set(galaxy, [x, galaxyMap.get(galaxy)[1] + 1])
        }
      }
    }

    galaxyMapCopy = new Map(galaxyMap)

    for (const [row,] of rowsWithoutGalaxy) {
      for (const [galaxy, [x, y]] of galaxyMapCopy) {
        if (x > row) {
          galaxyMap.set(galaxy, [galaxyMap.get(galaxy)[0] + 1, y])
        }
      }
    }

    const combinations = this.makeCombinations([...galaxyMap.keys()], 2)
    let sum = 0
    for (const [a, b] of combinations) {
      const aCoords = galaxyMap.get(a)
      const bCoords = galaxyMap.get(b)
      const distance = this.getSlopeDistance(aCoords!, bCoords!)
      sum += distance
    }

    console.log(sum)
  }


  getSlopeDistance(a: [number, number], b: [number, number]): number {
    return Math.abs(a[0] - b[0]) + Math.abs(a[1] - b[1])
  }

  makeCombinations(numbers: number[], r: number): number[][] {
    const combos: number[][] = []
    let head: number[] = []
    let tail: number[][] | number[] = []

    if (r === 1) {
      return [numbers]
    }

    for (let i = 0; i < numbers.length; i++) {
      head = numbers.slice(i, i + 1)
      tail = this.makeCombinations(numbers.slice(i + 1), r - 1)

      if ((r - 1) === 1) {
        tail = tail[0] as number[]
      }

      for (let j = 0; j < tail.length; j++) {
        let combo = head.concat(tail[j])
        combos.push(combo)
      }
    }

    return combos
  }
}
