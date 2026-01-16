#define _GNU_SOURCE
#include <dlfcn.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <err.h>
#include <seccomp.h>

int gen_new_passwd() {
  int passwd = 0;
  FILE *fd = fopen("/dev/random", "r");
  if (!fd)
    err(1, "Failed to open /dev/random");
  fread(&passwd, 1, sizeof(passwd), fd);
  fclose(fd);

  /* intentionally inserted for student */
  printf("[!] Generating a new passwd");
  fflush(stdout);

  for (int i = 0; i < 10; i ++) {
    printf(".");
    usleep(300000);
  }
  printf("Ready!\n");

  return passwd;
}

void save_passwd_into_vault(int passwd) {
  char tmpfile[100];
  snprintf(tmpfile, sizeof(tmpfile), "/tmp/.lock-%d", getpid());
  if (access(tmpfile, F_OK) != -1) {
    printf("the lock file exists, please first clean up\n");
    exit(1);
  }

  FILE *fp = fopen(tmpfile, "w");
  if (!fp)
    err(1, "failed to create %s", tmpfile);
  fprintf(fp, "%d", passwd);
  fclose(fp);

  /* DELETED! */
  unlink(tmpfile);
}

extern void snake_main();

void round2() {
  int passwd = gen_new_passwd();
  save_passwd_into_vault(passwd);

  printf("IOLI Crackme Level 0x01\n");
  printf("Password:");

  char buf[32];
  scanf("%31s", buf);

  if (atoi(buf) == passwd) {
    printf("Password OK :)\n");
    printf("[!] Have a great fun!\n");
    snake_main();
  } else {
    printf("Invalid Password!\n");
  }
}

// to avoid gcc's optimization
int absolute(int i) {
  if (i<0)
    return -i;
  else
    return i;
}

void start() {
  setreuid(geteuid(), geteuid());
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stdin, NULL, _IONBF, 0);

  int passwd;
  printf("IOLI Crackme Level 0x00\n");
  printf("Password: ");
  scanf("%d", &passwd);
  if (absolute(passwd) < 0) {
    printf("Password OK :)\n");
    round2();
  } else {
    printf("Invalid Password!\n");
  }
}

int main(int argc, char *argv[])
{
  start();
  return 0;
}
