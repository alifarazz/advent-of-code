#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>

#define MAXN (10 * 1000 * 1000)

int_fast32_t n;
int_fast32_t nums[MAXN];

inline static void read_input(char const *fname) {
  static const size_t BUFFER_SIZE = 63 * 1024; /* L1 data cache size */
  int fd = open(fname, O_RDONLY);
  if (fd == -1)
    exit(-1);

  /* Advise the kernel of our access pattern.  */
  posix_fadvise(fd, 0, 0, 1); // FDADVICE_SEQUENTIAL

  char buf[1204 + BUFFER_SIZE];
  char *bufp = buf + 1024;
  size_t bytes_read, offset = 0;
  while ((bytes_read = read(fd, bufp, BUFFER_SIZE))) {
    if (bytes_read == -1)
      exit(-2);
    if (!bytes_read)
      break;

    char *p_old = bufp - offset - 1;
    for (char *p = bufp; (p = memchr(p, '\n', (bufp + bytes_read) - p));
         p_old = p++) {
      *p = '\0';
      nums[n++] = atoi(p_old + 1);
    }

    offset = bytes_read - (p_old - bufp) - 1;
    memmove(bufp - offset, p_old + 1, offset);
  }
}

int main() {
  read_input("/proc/self/fd/0");

  int_fast32_t c1 = 0, c2 = 0;
  c1 += (nums[0] < nums[1]) ? 1 : 0;
  c1 += (nums[1] < nums[2]) ? 1 : 0;
  for (int i = 3; i < n; i++) {
    c2 += (nums[i - 3] < nums[i]) ? 1 : 0;
    c1 += (nums[i - 1] < nums[i]) ? 1 : 0;
  }
  printf("%" PRIdFAST32 "\n%" PRIdFAST32 "\n", c1, c2);
}
