#include "../utils.c" // unity build babyyyyyyyy!

#include <assert.h>

#define MAX_COMPARTMENT_LEN 64 // NOTE: input-dependent, == max line length

#define FOR_len(j) FR(j, len)
#define FOR_26(j) FR(j, 26 * 2)

internal inline size_t P(char p) { return (p >= 'a') ? p - 'a' : p - 'A' + 26; }

int main() {
  struct state _ = init(0);
  if (_.fd < 0)
    return _.fd;

  int sum1 = 0;
  int sum2 = 0;
  int elf = 0;
  int ss[2 * 26] = {0}; // scores part 2
  for (size_t i = 0; i < _.file_size; i++) {
    int len = 0;
    char line[MAX_COMPARTMENT_LEN];

    while (_.mem[i] != '\n')
      line[len++] = _.mem[i++];

    // part one
    int s[2 * 26] = {0};              // keep scores part 1
    FR(j, len / 2) s[P(line[j])] = 1; // mark seen as 1
    FOR(j, len / 2, len)
    s[P(line[j])] =
        (s[P(line[j])] == 1)
            ? -1
            : s[P(line[j])]; // mark seen and prev seen as -1, keep val if seen
    FOR_26(j) sum1 += (j + 1) * (s[j] == -1);

    // part two
    if (elf % 3 == 0) {              // first time
      FOR_len(j) ss[P(line[j])] = 1; // mark seen as 1
    } else {
      FOR_len(j) ss[P(line[j])] =
          (ss[P(line[j])] == 1)
              ? -1
              : ss[P(line[j])];        // mark seen and prev seen as -1
      FOR_26(j) ss[j] = (ss[j] == -1); // kick out not seen
    }

    if ((++elf % 3) == 0) {
      FOR_26(j) sum2 += (j + 1) * (ss[j] == 1);
      FOR_26(j) ss[j] = 0; // memset
    }
  }

  println_uint64(1, sum1);
  println_uint64(1, sum2);

  deinit(&_);
  return 0;
}
