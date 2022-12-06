#include "../utils.c" // unity build

// I could do it without a looping over the 26 cell array.
// Just union it with two uint64, and check for equality.
// Also, align the buffer so that cpu doens't have to do
// unaligned loads.

#define P(x) (x - 'a')

typedef struct {
  unsigned char c : 4;
} ccchar;

int main() {
  struct state _ = init(0);
  if (_.fd < 0)
    return _.fd;
  const char *mem = _.mem;

  ccchar gold_buf[26] = {0};
  FR(i, _.file_size) {
    size_t line_start_i = i;
    FR(j, 13) { gold_buf[P(mem[i + j])].c++; }
    i += 13;
    for (; mem[i] != '\n'; i++) {
      gold_buf[P(mem[i])].c++; // 14 chars read into gold_buf
      FR(k, 26) {
        if (gold_buf[k].c > 1)
          goto lbl_next;
      }
      println_uint64(1, i - line_start_i + 1);
      goto lbl_done;
    lbl_next:
      gold_buf[P(mem[i - 13])].c--;
    }
#ifndef ONCE
  lbl_done:
#endif
    while (mem[i] != '\n')
      i++;
    FR(j, 26) { gold_buf[j].c = 0; } // memset
  }
#ifdef ONCE
lbl_done:
#endif
  deinit(&_);
  return 0;
}
