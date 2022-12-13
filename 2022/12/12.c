#include "../utils.c" // unity build

// #define W 4999 // 77
// #define H 5999 // 41
#define W 77
#define H 41

#define STEPS_MAX 0xFFFFFFu // == U24_MAX
// #define STEPS_MAX U32_MAX

typedef struct
{
  char c;
  u32 d : 24;
  // u32 d;
} Cell;

typedef struct
{
  u32 i, j;
} PairU32;

typedef struct
{
  PairU32 m[H * W];
  u64 sz;
} Queue;

static Cell grid[H + 2][W + 2];
static Queue Q1, Q2;

void
Tick(Queue* Qsrc, Queue* Qtrgt)
{
  local const struct
  {
    int x, y;
  } displacemnts[] = { { -1, 0 }, { 0, -1 }, { 0, 1 }, { 1, 0 } };

  FR (qi, Qsrc->sz) {
    const u32 i = Qsrc->m[qi].i;
    const u32 j = Qsrc->m[qi].j;
    _Pragma("GCC unroll 4") FR (k, 4)
    {
      const u64 ii = i + displacemnts[k].x, jj = j + displacemnts[k].y;
      const char IAmAtHeight = grid[i][j].c + 1;
      if (grid[ii][jj].d == STEPS_MAX && grid[ii][jj].c <= IAmAtHeight) {
        grid[ii][jj].d = grid[i][j].d + 1;
        Qtrgt->m[Qtrgt->sz++] = (PairU32){ ii, jj };
      }
    }
  }
}

int
main()
{
  struct state _ = init(0);
  if (_.fd < 0)
    return _.fd;
  const char* mem = _.mem;

  u64 Si, Sj, Ei, Ej;
  { // parse input
    FR (i, H) {
      FR (j, W) {
        const u64 idx = i * (W + 1) + j;
        const char c = mem[idx];
        grid[i + 1][j + 1].c = c;
        grid[i + 1][j + 1].d = STEPS_MAX;
        {
          const bool cond = c == 'S';
          Si = cond ? i + 1 : Si;
          Sj = cond ? j + 1 : Sj;
        }
        {
          const bool cond = c == 'E';
          Ei = cond ? i + 1 : Ei;
          Ej = cond ? j + 1 : Ej;
        }
      }
    }
  }
  const char MountOlympus = 'z' + 2;
  {
    FR (i, W + 2)
      grid[0][i].c = MountOlympus;
    FOR (i, 1, H + 1)
      grid[i][W + 1].c = (grid[i][0].c = MountOlympus);
    FR (i, W + 2)
      grid[H + 1][i].c = MountOlympus;
  }

  Queue *Qsrc = &Q1, *Qtrgt = &Q2;

  { // silver
    grid[Si][Sj].c = MountOlympus;
    grid[Si][Sj].d = 0;
    grid[Ei][Ej].c = 'z' + 1;
    Qsrc->m[Qsrc->sz++] = (PairU32){ Si, Sj };
    do {
      Qtrgt->sz = 0;
      Tick(Qsrc, Qtrgt);
      SWAP(Qsrc, Qtrgt);
    } while (grid[Ei][Ej].d == STEPS_MAX && Qsrc->sz > 0); // O(N**2)
    println_uint64(1, grid[Ei][Ej].d);
  }

  { // gold
    Qsrc->sz = Qtrgt->sz = 0;
    FOR (i, 1, H + 1) { // init grid
      FOR (j, 1, W + 1) {
        const bool cond = grid[i][j].c == 'a';
        grid[i][j].d = cond ? 0 : STEPS_MAX;
        if (cond) {
          Qsrc->m[Qsrc->sz++] = (PairU32){ i, j };
        }
      }
    }
    grid[Si][Sj].c = MountOlympus;
    do {
      Qtrgt->sz = 0;
      Tick(Qsrc, Qtrgt);
      SWAP(Qsrc, Qtrgt);
    } while (grid[Ei][Ej].d == STEPS_MAX && Qsrc->sz > 0); // O(N**2)
    println_uint64(1, grid[Ei][Ej].d);
  }

  deinit(&_);
  return 0;
}
