/*
*Test function to Check how shmsbrk works.
*/

#include "types.h"
#include "stat.h"
#include "user.h"


void valueinChild();
void multipledeclaration();
void multipleforktest();
int
main(int argc, char **argv)
{

  printf(1,"Testing shmbrk\n");

    valueinChild();
    multipledeclaration();
  multipleforktest();
  return 0;
}

void multipleforktest(){
  int * shm = (int *)shmbrk(4096);
  int i =0;
  for(i =0; i <3; i++){
   fork();
  }
  //wait();
  printf(1,"Hello: %d\n", *shm);
  exit();
}

void valueinChild(){
  int * shm = (int *)shmbrk(sizeof(int *));
  uint pid = fork();
  *shm = 10;
  if(!pid){
    *shm = 200;
    printf(1,"Value of SHM variable in child :%d\n",*shm);
    exit();
  }

  wait();
  printf(1,"Value of SHM variable in parent:%d\n",*shm);
  if(*shm == 200)
  printf(1,"Value of SHM variable remains consistent\n");
}

void multipledeclaration(){
  int * shm = (int *)shmbrk(sizeof(int *));
  int * shm2 = (int *)shmbrk(sizeof(int *));
  int * shm3 = (int *)shmbrk(sizeof(int *));
  int * shm4 = (int *)shmbrk(sizeof(int *));
  int * shm5 = (int *)shmbrk(sizeof(int *));
  *shm = 10;
  *shm2 = 100;
  *shm3 = 1000;
  *shm4 = 10000;
  *shm5 = 100000;

  printf(1,"Value of SHM1 variable in parent:%d\n",*shm);
  printf(1,"Value of SHM2 variable in parent:%d\n",*shm2);
  printf(1,"Value of shm3 variable in parent:%d\n",*shm3);
  printf(1,"Value of SHM4 variable in parent:%d\n",*shm4);
  printf(1,"Value of SHM5 variable in parent:%d\n",*shm5);
}
