#include "../utils.c" // unity build

#define W 77
#define H 41

#define STEPS_MAX 0xFFFFFFu // U24_MAX

typedef struct
{
  char c;
  u32 d : 24;
} Cell;

static Cell grid[H + 2][W + 2];

void
Tick()
{
  local const struct { int x, y; } displacemnts[] = { { -1, 0 }, { 0, -1 }, { 0, 1 }, { 1, 0 } };

  FOR (i, 1, H + 1) {
    FOR (j, 1, W + 1) {
      _Pragma("GCC unroll 4") FR (k, 4)
      {
        const u64 ii = i + displacemnts[k].x, jj = j + displacemnts[k].y;
        const char IAmAtHeight = grid[ii][jj].c + 1;
        if (grid[ii][jj].d != STEPS_MAX && grid[i][j].c <= IAmAtHeight) {
          const u32 newD = grid[ii][jj].d + 1;
          const u32 minD = grid[i][j].d < newD ? grid[i][j].d : newD;
          grid[i][j].d = minD;
        }
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

  { // silver
    grid[Si][Sj].c = 'z';
    grid[Si][Sj].d = 0;
    grid[Ei][Ej].c = 'z' + 1;

    while (grid[Ei][Ej].d == STEPS_MAX) { // O(N**4) LOL!
      Tick();
    }
    println_uint64(1, grid[Ei][Ej].d);
  }

  { // gold
    FOR (i, 1, H + 1) {
      FOR (j, 1, W + 1) {
        grid[i][j].d = grid[i][j].c == 'a' ? 0 : STEPS_MAX;
      }
    }
    grid[Si][Sj].c = 'z' + 2;            // Mount Olympus
    while (grid[Ei][Ej].d == STEPS_MAX) { // O(N**4) LOLOLOLOLOLOLOL!
      Tick();
    }
    println_uint64(1, grid[Ei][Ej].d);
  }

  deinit(&_);
  return 0;
}
