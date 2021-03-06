// Test that fork fails gracefully.
// Tiny executable so that the limit can be filling the proc table.

#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"
#include "memlayout.h" 

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

//check to see if the child process can edit a parent variable.
//if not then cow-fork works.
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
    if(i == 0)
      printf(1,"parent copy did not change\n");
    else
      printf(1,"parent copy changed\n");
  }
}


//check to see if the child process has different fd from the parent
void
forktest3(void)
{
  int fd;
  char buf[10]={'\0'};
  int pid;
  int n=0;
  fd = open("./wolfie.txt",O_RDONLY);
  
  if (fd < 0)
    printf(1, "fd not valid\n");
  pid = fork();
  if(pid > 0)
    close(fd);

  
  if(pid == 0){
    wait();
    n=read(fd, buf, sizeof(buf));
    if(n<0)
      printf(1,"couldn't read a closed file, the child closed correctly\n");
    else
      printf(1,"somehow read from a closed file\n");
  }
}

//read a file in a parent and read in the child and see if the contents are the same.
void
forktest4(void)
{

  int fd;
  fd = open("wolfietest.c",O_RDONLY);
  char buf[10]={'\0'};
  int n = 0;
  int pid;
    
  if (fd < 0)
    printf(1, "fd not valid\n");
  n=read(fd, buf, sizeof(buf));
  if (n<0){
    printf(1,"couldn't read from file\n");
  }
  pid = fork();
  if (pid > 0){
    char buf2[10]={'\0'};
    int t = 0;
    t = read(fd, buf, sizeof(buf));

    if (t != n){
      printf(1, "same number of bytes were not read\n");
      exit();
    }
    int  i = 0;
    while (i<sizeof(buf)){
      if(buf[i]!=buf2[i]){
        printf(1, "same bytes were not read\n");
        exit();
      }
      i++;
    }
  }
  if (pid == 0)
    wait();

}



int
main(void)
{
  forktest4();
  forktest3();
  forktest2();
  forktest();
  exit();
}
