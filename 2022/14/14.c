#include "../utils.c" // unity build


// use `idk.py` to find MAX_W and MAX_H
#define MAX_W 573
#define MAX_H 182

#define GRID_WIDTH (MAX_H + MAX_W + 1)
#define GRID_HEIGHT (MAX_H + 2)

internal char grid[GRID_HEIGHT ][GRID_WIDTH];

/*
internal void
DumpGrid(PairU16_t RangeHeight, PairU16_t RangeWidth)
{
  for (int i = RangeHeight.x; i <= RangeHeight.y; i++) {
    for (int j = RangeWidth.x; j <= RangeWidth.y; j++) {
      if (grid[i][j])
        printf("%c", grid[i][j]);
      else
        putchar('.');
    }
    puts("");
  }
  puts("\n===========");
}
*/

internal PairU16_t
ParsePair(const char* mem, u64* i)
{
  PairU16_t result;
  __auto_type rs = read_int(mem + *i);
  *i += rs.offset + 1;
  result.x = rs.integer;
  rs = read_int(mem + *i);
  *i += rs.offset;
  result.y = rs.integer;
  return result;
}

internal void
ParseInput(const char* mem, const u64 file_size, u16* MaxHeight)
{
  u16 MaxHeight_ = 0;
  for (u64 i = 0; i < file_size; i++) {
    __auto_type prev = ParsePair(mem, &i);
    do {
      i += 4;
      __auto_type next = ParsePair(mem, &i);

      __auto_type RangeWidth = (PairU16_t){ MIN(prev.x, next.x), MAX(prev.x, next.x) };
      __auto_type RangeHeight = (PairU16_t){ MIN(prev.y, next.y), MAX(prev.y, next.y) };

      // O(N)
      for (u16 ii = RangeHeight.x; ii <= RangeHeight.y; ii++) {
        for (u16 jj = RangeWidth.x; jj <= RangeWidth.y; jj++) {
          grid[ii][jj] = '#';
          MaxHeight_ = MAX(MaxHeight_, ii);
        }
      }
      prev = next;
    } while (mem[i] != '\n');
  }
  *MaxHeight = MaxHeight_;
}

internal bool
MoveSand(PairU16_t* p)
{
  PairU16_t newp = *p;
  newp.x++;
  if (grid[newp.x][newp.y] == 0u) {
  } else if (grid[newp.x][newp.y - 1u] == 0u) {
    newp.y -= 1u;
  } else if (grid[newp.x][newp.y + 1u] == 0u) {
    newp.y += 1u;
  } else {
    return 0;
  }
  *p = newp;
  return 1;
}
internal PairU64_t
Sim(const u16 MaxBrickHeight)
{
  const PairU16_t src = { 0, 500 };
  PairU16_t s = src; // current sand location
  u64 silver = 0;
  bool SandIsOutOfBounds = 0;
  while (!SandIsOutOfBounds) {
    /* DumpGrid((PairU16_t){ 0, 9 }, (PairU16_t){ 494, 503 }); // Maybe C++ isn't that bad? */
    while (!SandIsOutOfBounds && MoveSand(&s)) {
      SandIsOutOfBounds = s.x >= MaxBrickHeight;
    }
    grid[s.x][s.y] = 'o';
    s = src;
    silver++;
  }
  u64 gold = silver;
  do {
    /* DumpGrid((PairU16_t){ 0, 11 }, (PairU16_t){ 494 - 5, 503 + 8 }); */
    s = src;
    while (s.x != MaxBrickHeight + 1 && MoveSand(&s)) {
    }
    grid[s.x][s.y] = 'o';
    gold++;
  } while (!(s.x == src.x && s.y == src.y));
  /* DumpGrid((PairU16_t){ 0, 11 }, (PairU16_t){ 494 - 5, 503 + 8 });  */
  return (PairU64_t){ silver - 1, gold + 1};
}

int
main()
{
  struct state _ = init(0);
  if (_.fd < 0)
    return _.fd;

  u16 MaxBrickHeight;
  ParseInput(_.mem, _.file_size, &MaxBrickHeight);
  __auto_type ans = Sim(MaxBrickHeight);
  println_uint64(1, ans.x);
  println_uint64(1, ans.y);

  deinit(&_);
  return 0;
}
