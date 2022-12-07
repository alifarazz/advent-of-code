#include "../utils.c" // unity build

typedef struct {
  u64 silver, gold;
} IDK;

__attribute__((optimize("unroll-loops"))) internal IDK solve(const char *mem,
                                                             size_t l,
                                                             size_t p1,
                                                             size_t p2) {
  assert(p1 < p2);
  mem = __builtin_assume_aligned(mem, 64);

// WTF: adding i < l makes it 30ms faster for some god forsaken reason!!!!
#define SOLVE(p1)                                                              \
  for (; /*mem[i] != '\n'*/ i < l; i++) {                                      \
    u32 seen = 0;                                                              \
    FOR(j, i - p1, i) { seen |= ((u32)1) << mem[j]; }                          \
    if (__builtin_popcount(seen) == p1) {                                      \
      p1 = i;                                                                  \
      break;                                                                   \
    }                                                                          \
  }

  size_t i = p1;
  SOLVE(p1)
  SOLVE(p2)
  return (IDK){.silver = p1, .gold = p2};
#undef SOLVE
}

int main() {
  struct state _ = init(0);
  if (_.fd < 0)
    return _.fd;
  /* const char *mem = __builtin_assume_aligned(_.mem, 64); */
  const char *mem = _.mem;

  local _Alignas(64) char buf[100000000];
  FR(i, _.file_size - 1) {
    buf[i] = mem[i] - 'a'; // doing this conversion once helps performance
                           // doing it on mem itself, however, hurts perf.
  }
  __auto_type ans = solve(buf, _.file_size - 1, 4, 14);
  println_uint64(1, ans.silver);
  println_uint64(1, ans.gold);

  deinit(&_);
  return 0;
}
