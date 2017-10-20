// Test that fork fails gracefully.
// Tiny executable so that the limit can be filling the proc table.

#include "types.h"
#include "stat.h"
#include "user.h"

#define N  1000

void
printf(int fd, char *s, ...)
{
  write(fd, s, strlen(s));
}

void
forktest(void)
{
  int n, pid;

  printf(1, "fork test\n");

  for(n=0; n<N; n++){
    pid = fork();
    if(pid < 0)
      break;
    if(pid == 0)
      exit();
  }

  if(n == N){
    printf(1, "fork claimed to work N times!\n", N);
    exit();
  }

  for(; n > 0; n--){
    if(wait() < 0){
      printf(1, "wait stopped early\n");
      exit();
    }
  }

  if(wait() != -1){
    printf(1, "wait got too many\n");
    exit();
  }

  printf(1, "fork test OK\n");
}


void
forktest2(void)
{

  int i = 0;
  int pid;
  pid = fork();
  if(pid >0){
    i=1;
  }
  wait();
  if (pid == 0) {
    if(i == 0){
      printf(1,"parent copy did not change");
    }
    else{
      printf(1,"parent copy changed");
    }
  }
}


int
main(void)
{
  forktest2();
  forktest();
  exit();
}
