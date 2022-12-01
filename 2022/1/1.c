#include "../utils.c" // unity build babyyyyyyyy!

#include <fcntl.h>
#include <sys/mman.h>

#include <assert.h>

int main() {
  int fd;
  // fd = open("input", O_RDONLY);
  fd = 0; // use stdin
  if (fd != -1) {
    {
      const off_t file_size = get_file_size(fd);
      if (file_size > 0) {
        char *mem = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
        if (mem) {
          {
            size_t max[3] = {0, 0, 0};
            size_t current = 0;
            for (size_t i = 0; i < file_size;) {
              struct read_int_result rs = read_int(mem + i);

              current += rs.integer;

              // on a newline or on the 1st digit of next num
              // + 1 is ok becuase there is always a newline at the
              // end of the input file
              i += rs.offset + 1;

              if (*(mem + i) == '\n') { // did I finish a chunk?
                if (current > max[0]) { // set correct position within max[]
                  max[2] = max[1];
                  max[1] = max[0];
                  max[0] = current;
                } else if (current > max[1]) {
                  max[2] = max[1];
                  max[1] = current;
                } else if (current > max[2]) {
                  max[2] = current;
                }
                current = 0;
              }
            }

            write(1, "part one: ", 10);
            println_uint64(1, max[0]);
            write(1, "part two: ", 10);
            println_uint64(1, max[0] + max[1] + max[2]);
          }
          assert(munmap(mem, file_size) == 0);
        } else {
          return -1;
        }
      } else {
        return -2;
      }
    }
    assert(close(fd) == 0);
  } else {
    return -3;
  }

  return 0;
}
