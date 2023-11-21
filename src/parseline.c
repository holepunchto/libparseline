#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../include/parseline.h"

void
parseline_init (parseline_t *ln, size_t default_buffer_size) {
  ln->len = default_buffer_size;
  ln->base = malloc(ln->len);
  ln->consumed = 0;
  ln->buffered = 0;
}

static inline void
compress_buffer (parseline_t *ln, size_t buffer_size) {
  if (ln->consumed) {
    if (ln->buffered == ln->consumed) {
      ln->consumed = ln->buffered = 0;
    } else {
      memmove(ln->base, ln->base + ln->consumed, ln->buffered - ln->consumed);
      ln->buffered -= ln->consumed;
      ln->consumed = 0;
    }
  }

  // ensure space
  while ((ln->len - ln->buffered) < buffer_size) {
    ln->base = realloc(ln->base, ln->len *= 2);
  }
}

void
parseline_buffer (parseline_t *ln, char **buf, size_t *len) {
  compress_buffer(ln, ln->len);

  *buf = ln->base + ln->buffered;
  *len = ln->len - ln->buffered;
}

void
parseline_push (parseline_t *ln, const char *buf, size_t read) {
  compress_buffer(ln, read);

  char *head = ln->base + ln->buffered;

  // if the reader used the buffer directly no need for a memcopy
  if (buf != head) memcpy(head, buf, read);
  ln->buffered += read;
}

bool
parseline_shift (parseline_t *ln, char **buf, size_t *len) {
  size_t buffered = ln->buffered - ln->consumed;
  char *base = ln->base + ln->consumed;

  for (int i = 0; i < buffered; i++) {
    char ch = base[i];
    if (ch == '\n') {
      *buf = base;
      *len = i + 1;
      ln->consumed += (i + 1);
      return true;
    }
  }

  *buf = NULL;
  *len = 0;

  return false;
}

void
parseline_flush (parseline_t *ln, char **buf, size_t *len) {
  *len = ln->buffered - ln->consumed;
  *buf = ln->base + ln->consumed;
}

void
parseline_destroy (parseline_t *ln) {
  if (ln->base) free(ln->base);
  ln->base = NULL;
  ln->len = 0;
}
