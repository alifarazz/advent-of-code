from std/sequtils import toSeq, foldl, map
from std/strutils import split, splitWhitespace, parseInt
import std/intsets

const
  sz = 5
  sz2 = sz * sz

func idx_to_freq_idx(idx: int): (int, int) =
  let
    idx_in_mat = (idx mod sz2)
    offset = (idx div sz2) * (sz * 2)
    row_idx = idx_in_mat div sz
    col_idx = idx_in_mat mod sz
  result = (offset + row_idx, offset + col_idx + sz)

func freq_idx_to_mat_offset(idx: int): int =
  result = (idx div (sz * 2)) * sz2

func find_bingo_board_score(bingos: openArray[int],
                            numbers: openArray[int],
                            n_mats: int,
                            num_to_idx: openArray[seq[int]],
                            nth: int = 1): int =
  var
    nth = nth
    called_bingo_idx: int
    called_mat_offset: int
    freqs = newSeq[int](2 * sz * n_mats)
  block findBingo:
    for bingoIdx in 0..<bingos.len:
      let bingo = bingos[bingoIdx]
      for row_col in num_to_idx[bingo].map(idx_to_freqIdx):
        for freqIdx in row_col.fields:
          inc freqs[freqIdx]
          if freqs[freqIdx] == 5: # did the board win?
            # discard all cols and rows of the `win` board
            let begin_freqIdx = freqIdx - (freqIdx mod (sz * 2))
            for i in begin_freqIdx..<(begin_freqIdx + sz * 2):
              freqs[i] = 5
            if nth == 1: # In reverse chronological order, which `win` do we want?
              called_bingo_idx = bingoIdx
              called_mat_offset = freq_idx_to_mat_offset(freqIdx)
              break findBingo
            dec nth
  let unmarked = numbers[called_mat_offset..<(called_mat_offset + sz2)]
    .toIntSet() - bingos[0..called_bingo_idx].toIntSet()
  result = unmarked.toSeq().foldl(a+b) * bingos[called_bingo_idx]

when isMainModule:
  let
    bingos = stdin.readLine().split(',').map(parseInt)
    numbers = stdin.readAll()[1..^2].splitWhitespace().map(parseInt)
    n_mats = numbers.len div sz2
  var num_to_idx = newSeq[seq[int]](bingos.max + 1)

  for i, num in numbers:
    num_to_idx[num].add i

  echo find_bingo_board_score(bingos, numbers, n_mats, num_to_idx), '\n',
       find_bingo_board_score(bingos, numbers, n_mats, num_to_idx, n_mats)
