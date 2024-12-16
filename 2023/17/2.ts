import { readFileSync } from "fs";
import { Answer } from "../index.js";
const DIRECTIONS = [
  [0, -1],
  [0, 1],
  [1, 0],
  [-1, 0]
]

class MinHeap {
  private heap: [number, number, number, number, number, number][] = [];

  push(item: [number, number, number, number, number, number]): void {
    this.heap.push(item);
    this.bubbleUp(this.heap.length - 1);
  }

  pop(): [number, number, number, number, number, number] | undefined {
    if (this.heap.length === 0) return undefined;
    const result = this.heap[0];
    const last = this.heap.pop()!;
    if (this.heap.length > 0) {
      this.heap[0] = last;
      this.bubbleDown(0);
    }
    return result;
  }

  private bubbleUp(index: number): void {
    while (index > 0) {
      const parentIndex = Math.floor((index - 1) / 2);
      if (this.heap[parentIndex][0] <= this.heap[index][0]) break;
      [this.heap[parentIndex], this.heap[index]] = [this.heap[index], this.heap[parentIndex]];
      index = parentIndex;
    }
  }

  private bubbleDown(index: number): void {
    while (true) {
      let smallest = index;
      const leftChild = 2 * index + 1;
      const rightChild = 2 * index + 2;

      if (leftChild < this.heap.length && this.heap[leftChild][0] < this.heap[smallest][0]) {
        smallest = leftChild;
      }
      if (rightChild < this.heap.length && this.heap[rightChild][0] < this.heap[smallest][0]) {
        smallest = rightChild;
      }

      if (smallest === index) break;

      [this.heap[index], this.heap[smallest]] = [this.heap[smallest], this.heap[index]];
      index = smallest;
    }
  }

  get length(): number {
    return this.heap.length;
  }
}

export default class Solution implements Answer {
  solve(): void {
    const contentMatrix: number[][] = readFileSync(import.meta.dirname + '/input.txt', 'utf8')
      .split('\n')
      .filter(line => line.length)
      .map(line => line.split('').map(value => parseInt(value, 10)))

    this.djikstra(contentMatrix)
  }

  djikstra(matrix: number[][]) {
    const rows = matrix.length
    const cols = matrix[0].length
    let seen = new Set<string>()
    let pq = new MinHeap()
    pq.push([0, 0, 0, 0, 0, 0])

    while (pq.length > 0) {
      const [hl, row, col, dr, dc, streak] = pq.pop()!

      if (row == rows - 1 && col == cols - 1 && streak >= 4) {
        console.log(hl)
        break
      }

      if (row < 0 || row >= rows || col < 0 || col >= cols) continue

      if (seen.has(`${row}-${col}-${dr}-${dc}-${streak}`)) {
        continue
      }

      seen.add(`${row}-${col}-${dr}-${dc}-${streak}`)

      if (streak < 10 && (dr != 0 || dc != 0)) {
        const newRow = row + dr
        const newCol = col + dc

        if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols) {
          pq.push([hl + matrix[newRow][newCol], newRow, newCol, dr, dc, streak + 1])
        }
      }

      if (streak >= 4 || (dr == 0 && dc == 0)) {
        for (const direction of DIRECTIONS) {
          if ((direction[0] !== dr || direction[1] !== dc) && (direction[0] != -dr || direction[1] != -dc)) {
            const newRow = row + direction[0]
            const newCol = col + direction[1]

            if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols) {
              pq.push([hl + matrix[newRow][newCol], newRow, newCol, direction[0], direction[1], 1])
            }
          }
        }
      }
    }
  }
}
