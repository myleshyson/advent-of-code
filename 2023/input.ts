import fs from 'fs'
import readline from 'readline'

export function getReadLineStream(path: string): readline.Interface {
  const fileStream = fs.createReadStream(path)

  return readline.createInterface({
    input: fileStream,
    crlfDelay: Infinity
  })
}

export async function resolveStream(stream: readline.Interface) {
  return new Promise<void>((resolve) => {
    stream.on('close', () => resolve())
  })
}
