#include "../utils.c" // unity build

#define LINE_LEN 40
#define LINE_COUNT 6

internal char buf[LINE_COUNT][LINE_LEN + 1];

internal i64 silver_calc(u64 cycle, i64 X) {
  switch (cycle) {
  case 20:
    return 20 * X;
  case 60:
    return 60 * X;
  case 100:
    return 100 * X;
  case 140:
    return 140 * X;
  case 180:
    return 180 * X;
  case 220:
    return 220 * X;
  }
  return 0;
}

internal void draw(char fb[][LINE_LEN + 1], u64 cycle, i64 X) {
  cycle--;
  u64 i = (cycle /* % (LINE_LEN * LINE_COUNT) */) / LINE_LEN;
  u64 j = cycle % LINE_LEN;
  bool shoot = ABS(X - (i64)j) < 2;
  fb[i][j] = shoot ? '$' : ' ';
}

int main() {
  struct state _ = init(0);
  if (_.fd < 0)
    return _.fd;
  /* const char *mem = __builtin_assume_aligned(_.mem, 64); */
  const char *mem = _.mem;

  u64 cycle = 1;
  i64 X = 1;
  i64 V = 0;
  i64 silver = 0;
  for (u64 i = 0; i < _.file_size - 1;) {
    i += 5;
    draw(buf, cycle, X);
    cycle++;
    silver += silver_calc(cycle, X);
    if (mem[i - 5] == 'a') {
      const bool v_is_neg = mem[i] == '-';
      const struct read_int_result num =
          v_is_neg ? read_int(mem + (++i)) : read_int(mem + i);
      V = v_is_neg ? -num.integer : num.integer;
      i += num.offset + 1;

      draw(buf, cycle, X);
      cycle++;
      X += V;
      silver += silver_calc(cycle, X);
    }
  }
  println_uint64(1, silver);

  FR(i, LINE_COUNT) { buf[i][LINE_LEN] = '\n'; }
  write(1, buf, LINE_COUNT * (LINE_LEN + 1));

  deinit(&_);
  return 0;
}
