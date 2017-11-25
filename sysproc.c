#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_wolfie(void){

  void *buffer;
  int size;

  char *art = "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM8MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n"
  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM888MMMMMMMMMMMMMMMMMMM8MMMMMMMMMMMMMMMM\n"
  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM88888MMMMMMMMMMMMMMMMM888MMMMMMMMMMMMMM\n"
  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM88D888MMMMMMMMMMMMMMMM8888MMMMMMMMMMMMM\n"
  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM88DD8888MMMMMMMMMMMMM888D88MMMMMMMMMMMM\n"
  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM888DDDD88MMMMMMMMMMMMM88DDD88MMMMMMMMMMM\n"
  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM888DDDD8888888MMMMMMMM88DDDD88MMMMMMMMMM\n"
  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM88DDD.DD888888888888888DDDDD888MMMMMMMMM\n"
  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMM8888888888888DD  DDD888DDDDDDD8888DDDDDD88MMMMMMMMM\n"
  "MMMMMMMMMMMMMMMMMMMMMMMM888888888DDDDDDDDDDD   DD88888DDDDDDDDD8DDDDD88MMMMMMMMM\n"
  "MMMMMMMMMMMMMMMMMMMMMMMMMM8888DDDDDDDDDDDDDD   DDDDDDD88DDDDDDDDDDDDD88MMMMMMMMM\n"
  "MMMMMMMMMMMMMMMMMMMMMMMM888888888DDDDDDDDDD    DDDDDDDDDDDDDDDDDDDDDD88MMMMMMMMM\n"
  "MMMMMMMMMMMMMMMMMMM8888888888DDDDDDDDDDDDD.     DDDDDDDDDDDDDDDDDDDDD88MMMMMMMMM\n"
  "MMMMMMMMMMMMMMMM8888888DDDDDDDDDDDDDDDDDDDDD DD DDDDDDDDDDDDDDDDDDDD888MMMMMMMMM\n"
  "MMMMMMMMMMMMM888888DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD+DDDDDDDD888MMMMMMMM\n"
  "MMMMMMMMMMM888888DDDDDDDDDDD++++++++++++++++DDDDDDDDDD+++DDDD+DDDDDDDD88MMMMMMMM\n"
  "MMMMMMMMM88888DDDDDDDDZ+++++++++++++++++++,  DDDDDDDDDD++++++++DDDDDDD88MMMMMMMM\n"
  "MMMMMMM88888DDDDDDDDDDDDDDDD$+++++.     DDDDDDDDDDDDDDDD++DDDD+DDDDDDDD88MMMMMMM\n"
  "MMMMM88888DDDDDDDDDDDDDDD++++        DDDDDDDDDDDDDDDD+DD++D88D++DDDDDD888MMMMMMM\n"
  "MMM888888888DDDDDDDDD++++      +++++++++DDDDDDD++++DDDDDDD+7DDDDDDDDD888MMMMMMMM\n"
  "MMMMMM8888DDDDDDDDD+++          ++++++DDDDDD+++++DDD+++++?DDDDD+++DDDD88MMMMMMMM\n"
  "MMMMM888DDDDDDDD+++ ++++++++++++++++8DDDDD++++++++++++++++++8++++DDDDD888MMMMMMM\n"
  "MMM8888D88DDDD++++++DDDDDDDDDDDDDDDDDDDDD++I++++++++DDDDDDDD?++++++ZDDD8888MMMMM\n"
  "MM8888888DDD+$DDDDDDDDD888888888DDDDDDDDDDD+++++++DD88D8 DDDDD+++++DDDDDD888MMMM\n"
  "M8888888DDDDDDD8888888888888888888DDDDDDD++++++++DDDD  88DDD  D+++++DDDDDD8888MM\n"
  "88MM888DDDD88888888MMMMMMMMMMM888DDD88DDDDDD++++++++++DD  D888 D+++++++?$DDDD88M\n"
  "MMM888D8888888MMMMMMMMMMMMM MM8888888DDDDDD+++++++++++++DDDDD8  D+++++DDDDDD888M\n"
  "MM88888888MMMMMMMMMMMMMMMMMMM88888888DDDD++++++DDD++++++++D D888DDD++++I+DDD88MM\n"
  "M88888MMMMMMMMMMMMMMMMMMMMMMM8888M88DDD$+DDDDDDDDDDDDD+++++DD D8D  D8DDDD8888MMM\n"
  "M88DMMMMMMMMMMMMMMMMMMMMMMMM8MMMM88DDDDDDDD888888888DDDD+++++DDD  DD D8D888MMMMM\n"
  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM888DDD88888888888888888DDD++++DD8  DDDD888MMMMMM\n"
  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM88D88888MMMMMMMMMMMMM8888DD+++++DDDDDD888MMMMMMM\n"
  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM8888MMMMMMMMMMMMMMMMMMMM888DD+++++++DDD88MMMMMMMM\n"
  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM88MMMMMMMMMMMMMMMMMMMMMMMMM888DDDDDDDD888MMMMMMMMM\n"
  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM888888D88888MMMMMMMMMM\n"
  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM8888888MMMMMMMMMMMM\n"
  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n";


  if(argint(1,&size)<0){
    return -1;
  }
  if(argptr(0,(char**)&buffer,size)<0){
    return -1;
  }

  //count how big the picture is.
  int artsize = 0;
  for (artsize = 0; art[artsize] != '\0' ; artsize ++){
  }

  if (artsize > size){
    return -1;
  }
  else{
    int i = 0;
    for (i = 0; i < artsize; i ++){
      ((char*)buffer)[i] = art[i];

    }
    return artsize;
  }

}

int sys_shmbrk(void)
{
  // LAB 4: Your Code Here
  return 0xDEADBEAF;
}

int
sys_futex_wait(void)
{
  // LAB 4: Your Code Here
  return 0xDEADBEAF;
}

int
sys_futex_wake(void)
{
  // LAB 4: Your Code Here
  return 0xDEADBEAF;
}
