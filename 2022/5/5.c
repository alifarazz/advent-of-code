#include "../utils.c" // unity build

#define MAX_LINE_BUFFER_LENGTH 128 // 512
#define MAX_STACK_COUNT 16         // 200
#define MAX_STACK_CAPACITY 128     // 4096

typedef struct {
  char stacks[MAX_STACK_COUNT][MAX_STACK_CAPACITY];
  short stack_len[MAX_STACK_COUNT];
} crane_state;

char line_buf[MAX_LINE_BUFFER_LENGTH];

internal size_t get_line_size(const char *f) {
  size_t cur = 0;
  while (f[cur++] != '\n')
    ;
  return cur;
}

internal size_t read_in_stacks(size_t line_size, const char *f, size_t nstks,
                               crane_state *crane) {
  size_t cur = 0;
  for (; f[cur + 1] != '1'; cur += line_size) {
    for (size_t i = 0; i < nstks; i++) {
      size_t line_cur = i * 4 + 1;
      char c = f[cur + line_cur];
      if (c != ' ') {
        crane->stacks[i][crane->stack_len[i]++] = c;
      }
    }
  }
  return cur;
}

internal void reverse_each_stack(size_t nstks, crane_state *crane) {
  FR(i, nstks) {
    __auto_type n = crane->stack_len[i];
    FR(j, n / 2) {
      char tmp = crane->stacks[i][j];
      crane->stacks[i][j] = crane->stacks[i][n - j - 1];
      crane->stacks[i][n - j - 1] = tmp;
    }
    // write(1, crane->stacks[i], crane->stack_len[i]);
    // write(1, "\n", 1);
  }
}

internal size_t parse_crane_cmnd(size_t cur, const char *f, size_t *ijn) {
  cur += 5; // len("move ");
  __auto_type ri = read_int(f + cur);
  ijn[2] = ri.integer;

  cur += ri.offset + 1 + 5; // num + spc + len("from ")
  ri = read_int(f + cur);
  ijn[0] = ri.integer - 1;

  cur += ri.offset + 1 + 3; // num + spc + len("to ")
  ri = read_int(f + cur);
  ijn[1] = ri.integer - 1;

  cur += ri.offset + 1; // num + \n
  return cur;
}

internal inline void silver_move(crane_state *crane, size_t i, size_t j,
                                 size_t n) {
  FR(k, n) { // a.push(b.pop())
    crane->stacks[j][crane->stack_len[j]++] =
        crane->stacks[i][--crane->stack_len[i]];
  }
}
internal inline void gold_move(crane_state *crane, size_t i, size_t j,
                               size_t n) {
  FR(k, n) { // memmove
    crane->stacks[j][crane->stack_len[j]++] =
        crane->stacks[i][crane->stack_len[i] - n + k];
  }
  crane->stack_len[i] -= n;
}

internal void print_tops(size_t nstks, const crane_state *s,
                         const crane_state *g) {
  FR(i, nstks) { line_buf[i] = s->stacks[i][s->stack_len[i] - 1]; }
  line_buf[nstks] = '\n';
  FR(i, nstks) { line_buf[nstks + 1 + i] = g->stacks[i][g->stack_len[i] - 1]; }
  line_buf[nstks * 2 + 1] = '\n';
  write(1, line_buf, 2 * (nstks + 1));
}

int main() {
  struct state _ = init(0);
  if (_.fd < 0)
    return _.fd;
  const char *mem = _.mem;

  // NOTE: two pass on the first line of input
  const size_t line_size = get_line_size(mem);
  const size_t nstks = line_size / 4;

  // read in stacks
  static crane_state silver = {.stack_len = {0}};
  size_t cur = read_in_stacks(line_size, mem, nstks, &silver);
  cur += line_size + 1; // skip next two lines

  // reverse each stack, so that the top element is the last elememnt
  reverse_each_stack(nstks, &silver);

  static crane_state gold;
  gold = silver;
  while (cur < _.file_size) {
    size_t ijn[3];
    cur = parse_crane_cmnd(cur, mem, ijn);

    silver_move(&silver, ijn[0], ijn[1], ijn[2]);
    gold_move(&gold, ijn[0], ijn[1], ijn[2]);
  }

  print_tops(nstks, &silver, &gold);

  deinit(&_);
  return 0;
}
