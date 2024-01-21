export function getWinningNumbers(lineIterator: IterableIterator<string>): Set<string> {
  const winningSet: Set<string> = new Set()
  let startWinningSet = false
  let currentNumber: string[] = []

  for (const char of lineIterator) {
    if (char === ':') {
      startWinningSet = true

    } else if (!startWinningSet) {
      continue

    } else if (char === ' ' && currentNumber.length) {
      winningSet.add(currentNumber.join(''))
      currentNumber = []

    } else if (char !== ' ' && char !== '|') {
      currentNumber.push(char)
    } else if (char === '|') {
      break
    }
  }

  return winningSet
} 
