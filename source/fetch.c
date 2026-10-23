#include <stdio.h>
#include <sys/utsname.h>
#include <string.h>

void os(void) {
  struct utsname info;
  if (uname(&info) != 0) {
    perror("uname");
    return;
  }
  printf("System: %s %s\n", info.sysname, info.release);
}

void pretty(void) {
  FILE *release = fopen("/etc/os-release", "r");
  char line[256];
  if (release != NULL) {
    while (fgets(line, sizeof line, release) != NULL) {
      if (strncmp(line, "PRETTY_NAME=", 12) == 0) {
	printf("%s", line + 12);
	break;
      }
    }
    fclose(release);
  }
}
