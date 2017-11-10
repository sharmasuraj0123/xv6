/*
*Simple test function to write at Memory Address 0.
*/

#include "types.h"
#include "stat.h"
#include "user.h"
int
main(int argc, char **argv)
{
  char *np =0;
  printf(1,"About t write at memory Adress 0\n");
  *np = 'a';
  printf(1,"The character at memory Address 0 is: %c\n", *np);
  return 0;
}
