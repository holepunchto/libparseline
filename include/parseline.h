#ifndef PARSELINE_H
#define PARSELINE_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  char* base;
  size_t len;
  size_t consumed;
  size_t buffered;
} parseline_t;

void
parseline_init (parseline_t *ln, size_t default_buffer_size);

void
parseline_buffer (parseline_t *ln, char **buf, size_t *len);

void
parseline_push (parseline_t *ln, const char *buf, size_t read);

bool
parseline_shift (parseline_t *ln, char **buf, size_t *len);

void
parseline_flush (parseline_t *ln, char **buf, size_t *len);

void
parseline_destroy (parseline_t *ln);

#ifdef __cplusplus
}
#endif

#endif // PARSELINE_H
