import { Answer } from "../index.js";
import { getReadLineStream, resolveStream } from "../input.js";

export default class Solution implements Answer {
  cardMap: Map<string, number> | null = null

  async solve(): Promise<void> {
    const stream = getReadLineStream(import.meta.dirname + '/input.txt')
    let handScoreMap: Map<string, number> = new Map()
    this.cardMap = new Map([
      ['A', 14],
      ['K', 13],
      ['Q', 12],
      ['J', 11],
      ['T', 10],
      ['9', 9],
      ['8', 8],
      ['7', 7],
      ['6', 6],
      ['5', 5],
      ['4', 4],
      ['3', 3],
      ['2', 2],
    ])

    stream.on('line', line => {
      const match = line.match(/([^ ]+)\s+(\d+)/)
      handScoreMap.set(match[1], parseInt(match[2], 10))
    })

    await resolveStream(stream)

    handScoreMap = new Map([...handScoreMap.entries()].sort(this.handleHandSort.bind(this)))
    let sorted = [...handScoreMap.entries()]
    sorted.sort(this.handleHandSort.bind(this))

    console.log(sorted.reduce((prevHand: [string, number], curHand: [string, number], i: number): [string, number] => {
      prevHand[1] = prevHand[1] + (curHand[1] * (i + 1))
      return prevHand
    }, ['answer', 0])[1])
  }

  handleHandSort(a: [string, number], b: [string, number]): number {
    const aRank = this.getHandRank(a)
    const bRank = this.getHandRank(b)

    if (aRank === bRank) {
      for (let i = 0; i < a[0].length; i++) {
        if (a[0].charAt(i) !== b[0].charAt(i)) {
          return this.cardMap.get(a[0].charAt(i))! - this.cardMap.get(b[0].charAt(i))!
        }
      }
    }

    return bRank - aRank
  }

  getHandRank(hand: [string, number]): number {
    const cardCount: Map<string, number> = new Map()

    for (const char of hand[0]) {
      cardCount.set(char, (cardCount.get(char) || 0) + 1)
    }

    switch (cardCount.size) {
      // 5 of a kind
      case 1:
        return 1
      // 4 of a kind or full house
      case 2:
        return [...cardCount.values()].includes(4) ? 2 : 3
      // 3 of a kind or two-pair 
      case 3:
        return [...cardCount.values()].includes(3) ? 4 : 5
      // 1 pair 
      case 4:
        return 6
      // high card 
      default:
        return 7
    }
  }
}
