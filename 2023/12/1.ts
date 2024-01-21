import { Answer } from "../index.js";
import { getReadLineStream, resolveStream } from "../input.js";

enum HashState {
  IS_HASHING,
  IS_WAITING
}

export default class Solution implements Answer {
  failCache = new Map()
  cacheHit = 0

  async solve(): Promise<void> {
    const stream = getReadLineStream(import.meta.dirname + '/input.txt')
    let answer = 0

    stream.on('line', line => {
      const match = line.match(/([#.?]+)\s+([0-9,]+)/)
      let [, pattern, numbers] = match!
      let expandedPatternArray = []
      let expandedGroupsArray = []
      let groups = numbers.split(',').map(num => parseInt(num, 10))

      for (let index = 0; index < 5; index++) {
        expandedPatternArray.push(pattern)
        groups.forEach(num => expandedGroupsArray.push(num))
      }

      const expandedPattern = expandedPatternArray.join('?')
      // we're starting at pattern[-1], which is just an empty string.
      answer += this.traverseDecisionTree('', -1, expandedGroupsArray[0], 0, expandedPattern, expandedGroupsArray)
    })

    await resolveStream(stream)

    console.log(answer)
  }

  /**
   * If we view the pattern like a binary decision tree, we can do a depth-first-search on it to see 
   * if there's any valid paths. Building a tree then traversing would be wasting memory so instead 
   * we'll recursively traverse down each decision, only incrementing the count if we hit a leaf 
   * and there's no hashes left to add.
   *
   * @param {string} currentChar The current character we're evaluating 
   * @param {number} index The index of the pattern string we're currently on 
   * @param {number} hashCount A count to keep track of how many hashes we've encountered per group. 
   * @param {number} groupIndex The index of the current group we're evaluating.
   * @param {string} pattern The full pattern we're looking at e.g ???.###
   * @param {number[]} groups The groups of contigous hashes present in the pattern
   * @param {HashState} hashState The state of whether we're in a contigous group or looking for the next one 
   */
  traverseDecisionTree(
    currentChar: string,
    index: number,
    hashCount: number,
    groupIndex: number,
    pattern: string,
    groups: number[],
    hashState: HashState = HashState.IS_WAITING,
  ) {
    const key = `${currentChar}+${index}+${hashCount}+${groupIndex}+${hashState}+${pattern}+${groups}`

    if (this.failCache.has(key)) {
      return this.failCache.get(key)
    }

    // get the next character
    const nextCharInPattern = pattern[index + 1]

    // if the currentChar is "." and we've traversed 1 contigous group of hashes,
    // reset our hash count and hash state.
    if (currentChar === '.' && hashCount === 0) {
      groupIndex += 1
      hashCount = groups[groupIndex] || 0
      hashState = HashState.IS_WAITING
    }

    // if we hit a "." while hashing through a group, we know that's invalid. abort
    if (currentChar === '.' && hashState === HashState.IS_HASHING) {
      if (!this.failCache.has(key)) {
        this.failCache.set(key, 0)
      }
      return 0
    }

    // hashcount will only be undefined if we go out-of-bounds of our groups array.
    // if that happens, and this tried decrement it, it means we're trying to add 
    // an extra non-existant "#". Because of that abort
    if (currentChar === '#' && typeof hashCount === 'undefined') {
      if (!this.failCache.has(key)) {
        this.failCache.set(key, 0)
      }
      return 0
    }

    // if we hit a hash but haven't set our state to IS_HASHING yet, go ahead and do that.
    if (currentChar === '#' && hashState === HashState.IS_WAITING) hashState = HashState.IS_HASHING

    // decrement the hash count for the current group. 
    if (currentChar === '#') {
      hashCount -= 1
    }

    // if we decrement the hash count and it's below 0, that's because we ran
    // into another hashing group without hitting a "." first. we know that's invalid
    // so abort
    if (hashCount < 0) {
      if (!this.failCache.has(key)) {
        this.failCache.set(key, 0)
      }
      return 0
    }

    // the only way we get a valid path is if we reach a "leaf" of our tree.
    // even then we need to do some checks to account for edge cases.

    // if we're at a leaf but there's still groups to handle, it means
    // we don't have enough hashes. abort
    if (!nextCharInPattern && groups[groupIndex + 1]) {
      if (!this.failCache.has(key)) {
        this.failCache.set(key, 0)
      }
      return 0
    }
    // if we're at a leaf and the hashcount is above 0, means we didn't
    // add enough hashes to the pattern. that's invalid so abort 
    else if (!nextCharInPattern && hashCount && hashCount > 0) {
      if (!this.failCache.has(key)) {
        this.failCache.set(key, 0)
      }
      return 0
    }

    // otherwise, we're good! increment our count, this is a valid path
    else if (!nextCharInPattern) {
      if (!this.failCache.has(key)) {
        this.failCache.set(key, 1)
      }
      return 1
    }

    let count = 0

    // if the next character in the pattern is a "?" then we have two choices. "." or "#". try both to see if there's
    // any valid paths in eiter.
    if (nextCharInPattern === '?') {
      count += this.traverseDecisionTree('.', index + 1, hashCount, groupIndex, pattern, groups, hashState)
      count += this.traverseDecisionTree('#', index + 1, hashCount, groupIndex, pattern, groups, hashState)
    } else {
      count += this.traverseDecisionTree(pattern[index + 1], index + 1, hashCount, groupIndex, pattern, groups, hashState)
    }

    if (!this.failCache.has(key)) {
      this.failCache.set(key, count)
    }

    return this.failCache.get(key)
  }
}
