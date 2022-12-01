#ifndef UTILS_H_
#define UTILS_H_

#include <linux/fs.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>

/* #include <stdio.h> // perror */

#define MAX(a, b)                                                              \
  ({                                                                           \
    __auto_type _a = (a), _b = (b);                                            \
    _a > _b ? _a : _b;                                                         \
  })

#define MIN(a, b)                                                              \
  ({                                                                           \
    __auto_type _a = (a), _b = (b);                                            \
    _a < _b ? _a : _b;                                                         \
  })

struct read_int_result {
  int integer;
  int offset;
};

off_t get_file_size(int fd);
struct read_int_result read_int(const char *p);
ssize_t println_uint64(int fd, size_t num);

#endif // UTILS_H_
