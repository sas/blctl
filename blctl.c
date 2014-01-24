#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static void usage_exit(int status, const char *argv0)
{
  FILE *output = status == EXIT_SUCCESS ? stdout : stderr;

  fprintf(output, "usage: %s DEVICE [ up | down | get | set VAL ]\n"
                  "       up:   increase level by 5%%\n"
                  "       down: decrease level by 5%%\n"
                  "       get:  display current level\n"
                  "       set:  set level to VAL\n",
          argv0);

  exit(status);
}

static unsigned int read_val(const char *path)
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

static void write_val(const char *path, unsigned int val)
{
  FILE *f;

  f = fopen(path, "w");
  if (f == NULL)
    err(EXIT_FAILURE, "%s", path);

  fprintf(f, "%u", val);

  fclose(f);
}

static inline void millisleep(size_t ms)
{
  struct timespec ts = {
    .tv_sec = 0,
    .tv_nsec = 1000000 * ms,
  };

  nanosleep(&ts, NULL);
}

int main(int argc, const char **argv)
{
  char *dev_path;
  char *cur_path;
  char *max_path;
  unsigned int cur_level;
  unsigned int max_level;
  unsigned int cur_perc;
  unsigned int new_perc;

  if (argc < 3 || argc > 4)
    usage_exit(EXIT_FAILURE, argv[0]);

  asprintf(&dev_path, "/sys/class/%s", argv[1]);
  asprintf(&cur_path, "%s/brightness", dev_path);
  asprintf(&max_path, "%s/max_brightness", dev_path);

  cur_level = read_val(cur_path);
  max_level = read_val(max_path);
  cur_perc = cur_level * 100 / max_level;

  if (strcmp(argv[2], "up") == 0) {
    new_perc = cur_perc + 5;
  } else if (strcmp(argv[2], "down") == 0) {
    new_perc = cur_perc - 5;
  } else if (strcmp(argv[2], "get") == 0) {
    printf("current level for %s: %d%%\n", argv[1], cur_perc);
    new_perc = cur_perc;
  } else if (strcmp(argv[2], "set") == 0) {
    new_perc = strtoul(argv[3], NULL, 0);
  }

  while (cur_perc != new_perc) {
    cur_perc += cur_perc < new_perc ? 1 : -1;
    write_val(cur_path, cur_perc * max_level / 100);
    millisleep(5);
  }

  free(dev_path);
  free(max_path);
  free(cur_path);

  return EXIT_SUCCESS;
}
