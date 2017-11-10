/*
*Simple test function to READ from Memory Address 0.
*/

#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char **argv)
{
  printf(1,"About to read a character from  memory Adress 0\n");
  char *np;
  np = 0x0;
  char a = *np;
  printf(1,"The character at memory Address 0 is: %c\n", a);


  return 0;
}
