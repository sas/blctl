#include "utils.h"

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned int read_val(const char *path)
{
  FILE *f;
  unsigned int res;

  f = fopen(path, "r");
  if (f == NULL)
    err(EXIT_FAILURE, "%s", path);

  fscanf(f, "%u", &res);

  fclose(f);
  return res;
}

void write_val(const char *path, unsigned int val)
{
  FILE *f;

  f = fopen(path, "w");
  if (f == NULL)
    err(EXIT_FAILURE, "%s", path);

  fprintf(f, "%u", val);

  fclose(f);
}

void millisleep(unsigned int ms)
{
  struct timespec ts = {
    .tv_sec = 0,
    .tv_nsec = 1000000 * ms,
  };

  nanosleep(&ts, NULL);
}
