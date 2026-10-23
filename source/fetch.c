#include <stdio.h>
#include <sys/utsname.h>

void os(void) {
  struct utsname info;
  if (uname(&info) != 0) {
    perror("uname");
    return;
  }
  printf("System: %s %s\n", info.sysname, info.release);
}
