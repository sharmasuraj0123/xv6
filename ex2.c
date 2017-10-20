#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"
#include "memlayout.h"

//we try to access DEVSPACE.

int main()
{

  int *k = (void *)(0xFE000000);
  printf("%d\n", *k);

  return 0;
}
