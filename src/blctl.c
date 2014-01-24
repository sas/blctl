#include "utils.h"

#include <err.h>
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void usage_exit(int status, const char *argv0)
{
  FILE *output = status == EXIT_SUCCESS ? stdout : stderr;

  fprintf(output, "usage: %s [ list | DEVICE [ up | down | get | set VAL ]]\n", argv0);
  exit(status);
}

static void validate_device(const char *dev)
{
  glob_t globres;
  size_t i;

  glob("{backlight,leds}/*", GLOB_BRACE, NULL, &globres);

  /*
   * If `dev` is `list`, we are not performing any actual operation, just trying
   * to list compatible devices.
   */
  if (strcmp(dev, "list") == 0) {
    if (globres.gl_pathc == 0) {
      printf("No compatible device found.\n");
    } else {
      printf("List of compatible devices:\n");
      for (i = 0; i < globres.gl_pathc; ++i)
        printf("%s\n", globres.gl_pathv[i]);
    }

    exit(EXIT_SUCCESS);
  }

  for (i = 0; i < globres.gl_pathc; ++i)
    if (strcmp(dev, globres.gl_pathv[i]) == 0)
      break;

  if (i == globres.gl_pathc)
    errx(EXIT_FAILURE, "%s is not a valid device.", dev);

  globfree(&globres);
}

int main(int argc, const char **argv)
{
#define MIN_ARGS(NUM) do { if (argc < NUM + 1) usage_exit(EXIT_FAILURE, argv[0]); } while (0)
  char *cur_path;
  char *max_path;
  int cur_level;
  int max_level;
  int cur_perc;
  int new_perc;

  MIN_ARGS(1);

  if (chdir("/sys/class") == -1)
    err(EXIT_FAILURE, "%s", "/sys/class");

  validate_device(argv[1]);

  MIN_ARGS(2);

  asprintf(&cur_path, "%s/brightness", argv[1]);
  asprintf(&max_path, "%s/max_brightness", argv[1]);

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
    MIN_ARGS(3);
    new_perc = strtoul(argv[3], NULL, 0);
  } else {
    usage_exit(EXIT_FAILURE, argv[0]);
  }

  if (new_perc < 0)
    new_perc = 0;

  if (new_perc > 100)
    new_perc = 100;

  while (cur_perc != new_perc) {
    cur_perc += cur_perc < new_perc ? 1 : -1;
    write_val(cur_path, cur_perc * max_level / 100);
    millisleep(5);
  }

  free(max_path);
  free(cur_path);

  return EXIT_SUCCESS;
#undef MIN_ARGS
}
