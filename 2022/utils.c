#include "utils.h"

// #pragma push_macro("assert")
#undef assert
#define assert(x) ((void)(x))

struct state init(const char *path) {
  struct state S;
  if (path) {
    S.fd = open(path, O_RDONLY);
  } else {
    S.fd = 0; // use stdin
  }
  if (S.fd < 0) {
    S.fd = -1;
    return S;
  }

  if ((S.file_size = get_file_size(S.fd)) <= 0) {
    S.fd = -2;
    return S;
  }

  if (!(S.mem = mmap(NULL, S.file_size, PROT_READ, MAP_PRIVATE, S.fd, 0))) {
    S.fd = -3;
    return S;
  }
  return S;
}

void deinit(struct state *s) {
  assert(munmap(s->mem, s->file_size) == 0);
  assert(close(s->fd) == 0);
}

off_t get_file_size(int fd) {
  struct stat st;

  if (fstat(fd, &st) < 0) {
    /* perror("fstat"); */
    return -1;
  }
  if (S_ISBLK(st.st_mode)) {
    unsigned long long bytes;
    if (ioctl(fd, BLKGETSIZE64, &bytes) != 0) {
      /* perror("ioctl"); */
      return -1;
    }
    return bytes;
  } else if (S_ISREG(st.st_mode))
    return st.st_size;

  return -1;
}

struct read_int_result read_int(const char *p) {
  int num = 0;
  size_t i = 0;
  for (; *p <= '9' && *p >= '0'; i++, p++) {
    num = num * 10 + *p - '0';
  }
  struct read_int_result result = {.integer = num, .offset = i};
  return result;
}

ssize_t println_uint64(int fd, size_t num) {
  static char buffer[21]; // len(str(2**64)) + 1 == 20 + 1 == 21
  int len = 0;
  do { // convert to char buf
    buffer[len++] = num % 10 + '0';
    num /= 10;
  } while (num != 0);
  for (int j = 0; j < len / 2; j++) { // reverse buffer
    char _ = buffer[j];
    buffer[j] = buffer[len - j - 1];
    buffer[len - j - 1] = _;
  }
  buffer[len++] = '\n';
  return write(fd, buffer, len);
}
// #pragma pop_macro("assert")
