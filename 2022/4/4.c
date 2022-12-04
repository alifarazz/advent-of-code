#include "../utils.c" // unity build babyyyyyyyy!


internal inline bool fullcontain(int a, int b, int c, int d) {
  return a <= c && d <= b || c <= a && b <= d;
}
internal inline bool overlap(int a, int b, int c, int d) {
  // return a <= c && c <= b && b <= d || c <= a && a <= d && d <= b ||
  //        a <= c && d <= b || c <= a && b <= d;

  return c <= b && a <= d;
}

int main() {
  struct state _ = init(0);
  if (_.fd < 0)
    return _.fd;

  size_t p1 = 0;
  size_t p2 = 0;
  for (size_t cur = 0; cur < _.file_size;) {
    int a[4];
    FR(j, 4) {
      struct read_int_result rir;
      rir = read_int(_.mem + cur);
      cur += rir.offset + 1;
      a[j] = rir.integer;
    }

    p1 += fullcontain(a[0], a[1], a[2], a[3]);
    p2 += overlap(a[0], a[1], a[2], a[3]);
  }

  println_uint64(1, p1);
  println_uint64(1, p2);

  deinit(&_);
  return 0;
}
