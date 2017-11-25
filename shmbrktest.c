/*
*Test function to Check how shmsbrk works.
*/

#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char **argv)
{

  printf(1,"Testing shmbrk\n");

  int * shm = (int *)shmbrk(sizeof(int *));
  uint pid = fork();
  *shm = 10;
  if(!pid){
    *shm = 89;
    printf(1,"Value of SHM variable in child :%d\n",*shm);
    exit();
  }

  wait();
  printf(1,"Value of SHM variable in parent:%d\n",*shm);

  pid =fork();
  if(!pid){
    printf(1,"Value of SHM variable in child :%d\n",*shm);
    *shm = 8921;
    printf(1,"Updated Value of SHM variable in child :%d\n",*shm);
    exit();
  }
  wait();
  printf(1,"New Value of SHM variable in parent:%d\n",*shm);


  return 0;
}
