#include "../utils.c" // unity build

int main() {
  struct state _ = init(0);
  if (_.fd < 0)
    return _.fd;
  const char *mem = _.mem;

  char silver_buf[4];
  size_t silver = 0;
  FR(i, _.file_size) {
    FR(j, 3) { silver_buf[(i + j) % 4] = mem[(i + j)]; }
    i += 3;
    for (; mem[i] != '\n'; i++) {
      __auto_type j = i % 4;
      silver_buf[j] = mem[i];
      FR(k, 4) {
        FOR(w, k + 1, 4) {
          if (silver_buf[k] == silver_buf[w]) {
            goto lbl_next;
          }
        }
      }
      println_uint64(1, i - silver + 1);
      goto lbl_done;
    lbl_next:;
    }
#ifndef ONCE
  lbl_done:
#endif
    while (mem[i] != '\n')
      i++;
    silver = i + 1;
  }

#ifdef ONCE
  lbl_done:
#endif
  deinit(&_);
  return 0;
}
