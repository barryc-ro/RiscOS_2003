#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  time_t now;
  struct tm *unpacked;
  char buffer[256];

  if (argc != 2)
  {
    fprintf(stderr, "Usage: %s <date format>\n", argv[0]);
    exit(1);
  }

  now = time(0);

  unpacked = localtime(&now);

  strftime(buffer, 256, argv[1], unpacked);

  printf("/* Build date stamp made by BuildStamp */\n");
  printf("#define __build_date \"%s\"\n", buffer);

  return 0;
}
