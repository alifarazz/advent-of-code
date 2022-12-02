#include "../utils.c" // unity build babyyyyyyyy!

#include <fcntl.h>
#include <sys/mman.h>

#include <assert.h>

int main() {
  int ret = 0;

  int fd;
  // fd = open("input", O_RDONLY);
  fd = 0; // use stdin
  if (fd == -6) {
    ret = -1;
    goto lbl_end;
  }

  const off_t file_size = get_file_size(fd);
  if (file_size <= 0) {
    ret = -2;
    goto lbl_end_close_fd;
  }

  char *mem = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (!mem) {
    ret = -3;
    goto lbl_end_munmap;
  }

  size_t score1 = 0;
  size_t score2 = 0;
  for (size_t i = 0; i < file_size; i += 4) {
    int op = mem[i] - 'A';
    int me = mem[i + 2] - 'X';

    int offset = -op + 1;
    score1 += (me + offset + 3) % 3 * 3 + me + 1; // black magic, don't touch
    score2 += (me - offset + 3) % 3 + 1 + me * 3;
  }

  println_uint64(1, score1);
  println_uint64(1, score2);

lbl_end_munmap:
  assert(munmap(mem, file_size) == 0);
lbl_end_close_fd:
  assert(close(fd) == 0);
lbl_end:
  return ret;
}
