/*
*Simple Function that cause a stack overflow in xv6
*/

#include "types.h"
#include "stat.h"
#include "user.h"

uint factorial(int n);

int
main(int argc, char **argv)
{

  printf(1,"The result of 10 factorial is %d\n", factorial(500));
  return 0;
}

uint factorial(int n){
  if(n ==0)
    return 1;
  return n* factorial(n-1);
}
