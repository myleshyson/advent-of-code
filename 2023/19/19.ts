import { readFileSync } from "fs";
import { Answer } from "../index.js";

export default class Solution implements Answer {
  solve() {
    const input = readFileSync("19.txt", "utf8")
      .split("\n\n")
  }
}
