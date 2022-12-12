#ifndef UTILS_H_
#define UTILS_H_

#include <linux/fs.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/mman.h>

/* #include <stdio.h> // perror */

// NOTE: platform-dependent, for gcc-linux-x86_64
typedef signed char i8;
typedef short i16;
typedef int i32;
typedef long long i64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;

typedef int bool;

#define U8_MAX  0xFFu
#define U16_MAX 0xFFFFu
#define U32_MAX 0xFFFFFFFu
#define U64_MAX 0xFFFFFFFFFFFFFFu

#define I8_MAX  0x7Fu
#define I16_MAX 0x7FFFu
#define I32_MAX 0x7FFFFFFu
#define I64_MAX 0x7FFFFFFFFFFFFFu

#define U8_MIN  0x80u
#define U16_MIN 0x8000u
#define U32_MIN 0x8000000u
#define U64_MIN 0x80000000000000u

#define internal static
#define local static
#define global

#define FOR(j, z, N) for (size_t j = z; j < N; j++)
#define FR(j, N) FOR(j, 0, N)

#define MAX(a, b)                                                              \
  ({                                                                           \
    __auto_type _a = (a);                                                      \
    __auto_type _b = (b);                                                      \
    _a > _b ? _a : _b;                                                         \
  })

#define MIN(a, b)                                                              \
  ({                                                                           \
    __auto_type _a = (a);                                                      \
    __auto_type _b = (b);                                                      \
    _a < _b ? _a : _b;                                                         \
  })

#define ABS(a)                                                                 \
  ({                                                                           \
    __auto_type _a = (a);                                                      \
    _a < 0 ? -_a : _a;                                                         \
  })

#define CLAMP(x, a, b)                                                         \
  ({                                                                           \
    __auto_type __x = (x);                                                     \
    __auto_type __a = (a);                                                     \
    __auto_type __b = (b);                                                     \
    MIN(MAX(__x, __a), __b);                                                   \
  })

struct state {
  int fd;
  off_t file_size;
  char *mem; // should be void*, but who cares
};
struct read_int_result {
  int integer;
  int offset;
};

struct state init(const char *path);
void deinit(struct state *s);

off_t get_file_size(int fd);
struct read_int_result read_int(const char *p);
ssize_t println_uint64(int fd, size_t num);

#endif // UTILS_H_
