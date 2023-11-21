#include <assert.h>
#include <string.h>

#include "../include/parseline.h"

#define test_parse(chunks, chunks_len, lines, lines_len) \
  { \
    parseline_t ln; \
    parseline_init(&ln, 16); \
    for (int i = 0; i < chunks_len; i++) { \
      parseline_push(&ln, chunks[i], strlen(chunks[i])); \
    } \
    char *buf; \
    size_t len; \
    for (int i = 0; i < lines_len; i++) { \
      bool shifted = parseline_shift(&ln, &buf, &len); \
      assert(shifted); \
      char *line = lines[i]; \
      assert(len == strlen(line)); \
      assert(memcmp(buf, line, len) == 0); \
    } \
    bool shifted = parseline_shift(&ln, &buf, &len); \
    assert(!shifted); \
    parseline_destroy(&ln); \
  };

int
main () {
  {
    char* chunks[1] = { "a\n" };
    char* lines[1] = { "a\n" };
    test_parse(chunks, 1, lines, 1);
  }

  {
    char* chunks[2] = { "bigger\n", "chunks\n" };
    char* lines[2] = { "bigger\n", "chunks\n" };
    test_parse(chunks, 2, lines, 2);
  }

  {
    char* chunks[2] = { "bigger\nchu", "nks\n" };
    char* lines[2] = { "bigger\n", "chunks\n" };
    test_parse(chunks, 2, lines, 2);
  }

  {
    char* chunks[2] = { "bigger\nchu", "nks\nandpartial" };
    char* lines[2] = { "bigger\n", "chunks\n" };
    test_parse(chunks, 2, lines, 2);
  }

  {
    char* chunks[3] = { "bigger\nchu", "nks\nandpartialthatiswaybiggerthanthebuffer", "\n" };
    char* lines[3] = { "bigger\n", "chunks\n", "andpartialthatiswaybiggerthanthebuffer\n" };
    test_parse(chunks, 3, lines, 3);
  }
}
